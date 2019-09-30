/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/layers/APZUpdater.h"

#include "APZCTreeManager.h"
#include "AsyncPanZoomController.h"
#include "base/task.h"
#include "mozilla/ClearOnShutdown.h"
#include "mozilla/layers/APZThreadUtils.h"
#include "mozilla/layers/CompositorThread.h"
#include "mozilla/layers/SynchronousTask.h"
#include "mozilla/layers/WebRenderScrollDataWrapper.h"
#include "mozilla/webrender/WebRenderAPI.h"

namespace mozilla {
namespace layers {

StaticMutex APZUpdater::sWindowIdLock;
StaticAutoPtr<std::unordered_map<uint64_t, APZUpdater*>>
    APZUpdater::sWindowIdMap;

APZUpdater::APZUpdater(const RefPtr<APZCTreeManager>& aApz,
                       bool aIsUsingWebRender)
<<<<<<< HEAD
    : mApz(aApz),
      mIsUsingWebRender(aIsUsingWebRender),
      mThreadIdLock("APZUpdater::ThreadIdLock"),
      mQueueLock("APZUpdater::QueueLock") {
||||||| merged common ancestors
  : mApz(aApz)
  , mIsUsingWebRender(aIsUsingWebRender)
  , mThreadIdLock("APZUpdater::ThreadIdLock")
  , mQueueLock("APZUpdater::QueueLock")
{
=======
    : mApz(aApz),
      mDestroyed(false),
      mIsUsingWebRender(aIsUsingWebRender),
      mThreadIdLock("APZUpdater::ThreadIdLock"),
      mQueueLock("APZUpdater::QueueLock") {
>>>>>>> upstream-releases
  MOZ_ASSERT(aApz);
  mApz->SetUpdater(this);
}

APZUpdater::~APZUpdater() {
  mApz->SetUpdater(nullptr);

  StaticMutexAutoLock lock(sWindowIdLock);
  if (mWindowId) {
    MOZ_ASSERT(sWindowIdMap);
    // Ensure that ClearTree was called and the task got run
    MOZ_ASSERT(sWindowIdMap->find(wr::AsUint64(*mWindowId)) ==
               sWindowIdMap->end());
  }
}

bool APZUpdater::HasTreeManager(const RefPtr<APZCTreeManager>& aApz) {
  return aApz.get() == mApz.get();
}

void APZUpdater::SetWebRenderWindowId(const wr::WindowId& aWindowId) {
  StaticMutexAutoLock lock(sWindowIdLock);
  MOZ_ASSERT(!mWindowId);
  mWindowId = Some(aWindowId);
  if (!sWindowIdMap) {
    sWindowIdMap = new std::unordered_map<uint64_t, APZUpdater*>();
    NS_DispatchToMainThread(NS_NewRunnableFunction(
        "APZUpdater::ClearOnShutdown", [] { ClearOnShutdown(&sWindowIdMap); }));
  }
  (*sWindowIdMap)[wr::AsUint64(aWindowId)] = this;
}

<<<<<<< HEAD
/*static*/ void APZUpdater::SetUpdaterThread(const wr::WrWindowId& aWindowId) {
||||||| merged common ancestors
/*static*/ void
APZUpdater::SetUpdaterThread(const wr::WrWindowId& aWindowId)
{
=======
/*static*/
void APZUpdater::SetUpdaterThread(const wr::WrWindowId& aWindowId) {
>>>>>>> upstream-releases
  if (RefPtr<APZUpdater> updater = GetUpdater(aWindowId)) {
    MutexAutoLock lock(updater->mThreadIdLock);
    updater->mUpdaterThreadId = Some(PlatformThread::CurrentId());
  }
}

<<<<<<< HEAD
/*static*/ void APZUpdater::PrepareForSceneSwap(
    const wr::WrWindowId& aWindowId) {
||||||| merged common ancestors
/*static*/ void
APZUpdater::PrepareForSceneSwap(const wr::WrWindowId& aWindowId)
{
=======
/*static*/
void APZUpdater::PrepareForSceneSwap(const wr::WrWindowId& aWindowId) {
>>>>>>> upstream-releases
  if (RefPtr<APZUpdater> updater = GetUpdater(aWindowId)) {
    updater->mApz->LockTree();
  }
}

<<<<<<< HEAD
/*static*/ void APZUpdater::CompleteSceneSwap(const wr::WrWindowId& aWindowId,
                                              const wr::WrPipelineInfo& aInfo) {
||||||| merged common ancestors
/*static*/ void
APZUpdater::CompleteSceneSwap(const wr::WrWindowId& aWindowId,
                              const wr::WrPipelineInfo& aInfo)
{
=======
/*static*/
void APZUpdater::CompleteSceneSwap(const wr::WrWindowId& aWindowId,
                                   const wr::WrPipelineInfo& aInfo) {
>>>>>>> upstream-releases
  RefPtr<APZUpdater> updater = GetUpdater(aWindowId);
  if (!updater) {
    // This should only happen in cases where PrepareForSceneSwap also got a
    // null updater. No updater-thread tasks get run between PrepareForSceneSwap
    // and this function, so there is no opportunity for the updater mapping
    // to have gotten removed from sWindowIdMap in between the two calls.
    return;
  }

  for (uintptr_t i = 0; i < aInfo.removed_pipelines.length; i++) {
    WRRootId layersId = WRRootId(aInfo.removed_pipelines.data[i].pipeline_id,
                                 aInfo.removed_pipelines.data[i].document_id);
    updater->mEpochData.erase(layersId);
  }
  // Reset the built info for all pipelines, then put it back for the ones
  // that got built in this scene swap.
  for (auto& i : updater->mEpochData) {
    i.second.mBuilt = Nothing();
  }
  for (uintptr_t i = 0; i < aInfo.epochs.length; i++) {
    WRRootId layersId = WRRootId(aInfo.epochs.data[i].pipeline_id,
                                 aInfo.epochs.data[i].document_id);
    updater->mEpochData[layersId].mBuilt = Some(aInfo.epochs.data[i].epoch);
  }

  // Run any tasks that got unblocked, then unlock the tree. The order is
  // important because we want to run all the tasks up to and including the
  // UpdateHitTestingTree calls corresponding to the built epochs, and we
  // want to run those before we release the lock (i.e. atomically with the
  // scene swap). This ensures that any hit-tests always encounter a consistent
  // state between the APZ tree and the built scene in WR.
  //
  // While we could add additional information to the queued tasks to figure
  // out the minimal set of tasks we want to run here, it's easier and harmless
  // to just run all the queued and now-unblocked tasks inside the lock.
  //
  // Note that the ProcessQueue here might remove the window id -> APZUpdater
  // mapping from sWindowIdMap, but we still unlock the tree successfully to
  // leave things in a good state.
  updater->ProcessQueue();

  updater->mApz->UnlockTree();
}

<<<<<<< HEAD
/*static*/ void APZUpdater::ProcessPendingTasks(
    const wr::WrWindowId& aWindowId) {
||||||| merged common ancestors
/*static*/ void
APZUpdater::ProcessPendingTasks(const wr::WrWindowId& aWindowId)
{
=======
/*static*/
void APZUpdater::ProcessPendingTasks(const wr::WrWindowId& aWindowId) {
>>>>>>> upstream-releases
  if (RefPtr<APZUpdater> updater = GetUpdater(aWindowId)) {
    updater->ProcessQueue();
  }
}

void APZUpdater::ClearTree(LayersId aRootLayersId) {
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  RefPtr<APZUpdater> self = this;
<<<<<<< HEAD
  RunOnUpdaterThread(aRootLayersId,
                     NS_NewRunnableFunction("APZUpdater::ClearTree", [=]() {
                       self->mApz->ClearTree();

                       // Once ClearTree is called on the APZCTreeManager, we
                       // are in a shutdown phase. After this point it's ok if
                       // WebRender cannot get a hold of the updater via the
                       // window id, and it's a good point to remove the mapping
                       // and avoid leaving a dangling pointer to this object.
                       StaticMutexAutoLock lock(sWindowIdLock);
                       if (self->mWindowId) {
                         MOZ_ASSERT(sWindowIdMap);
                         sWindowIdMap->erase(wr::AsUint64(*(self->mWindowId)));
                       }
                     }));
}

void APZUpdater::UpdateFocusState(LayersId aRootLayerTreeId,
                                  LayersId aOriginatingLayersId,
                                  const FocusTarget& aFocusTarget) {
||||||| merged common ancestors
  RunOnUpdaterThread(aRootLayersId, NS_NewRunnableFunction(
    "APZUpdater::ClearTree",
    [=]() {
      self->mApz->ClearTree();

      // Once ClearTree is called on the APZCTreeManager, we are in a shutdown
      // phase. After this point it's ok if WebRender cannot get a hold of the
      // updater via the window id, and it's a good point to remove the mapping
      // and avoid leaving a dangling pointer to this object.
      StaticMutexAutoLock lock(sWindowIdLock);
      if (self->mWindowId) {
        MOZ_ASSERT(sWindowIdMap);
        sWindowIdMap->erase(wr::AsUint64(*(self->mWindowId)));
      }
    }
  ));
}

void
APZUpdater::UpdateFocusState(LayersId aRootLayerTreeId,
                             LayersId aOriginatingLayersId,
                             const FocusTarget& aFocusTarget)
{
=======
  RunOnUpdaterThread(
      UpdaterQueueSelector(aRootLayersId, wr::RenderRoot::Default),
      NS_NewRunnableFunction("APZUpdater::ClearTree", [=]() {
        self->mApz->ClearTree();
        self->mDestroyed = true;

        // Once ClearTree is called on the APZCTreeManager, we
        // are in a shutdown phase. After this point it's ok if
        // WebRender cannot get a hold of the updater via the
        // window id, and it's a good point to remove the mapping
        // and avoid leaving a dangling pointer to this object.
        StaticMutexAutoLock lock(sWindowIdLock);
        if (self->mWindowId) {
          MOZ_ASSERT(sWindowIdMap);
          sWindowIdMap->erase(wr::AsUint64(*(self->mWindowId)));
        }
      }));
}

void APZUpdater::UpdateFocusState(LayersId aRootLayerTreeId,
                                  WRRootId aOriginatingWrRootId,
                                  const FocusTarget& aFocusTarget) {
>>>>>>> upstream-releases
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
<<<<<<< HEAD
  RunOnUpdaterThread(aOriginatingLayersId,
                     NewRunnableMethod<LayersId, LayersId, FocusTarget>(
                         "APZUpdater::UpdateFocusState", mApz,
                         &APZCTreeManager::UpdateFocusState, aRootLayerTreeId,
                         aOriginatingLayersId, aFocusTarget));
}

void APZUpdater::UpdateHitTestingTree(LayersId aRootLayerTreeId, Layer* aRoot,
                                      bool aIsFirstPaint,
                                      LayersId aOriginatingLayersId,
                                      uint32_t aPaintSequenceNumber) {
||||||| merged common ancestors
  RunOnUpdaterThread(aOriginatingLayersId, NewRunnableMethod<LayersId, LayersId, FocusTarget>(
      "APZUpdater::UpdateFocusState",
      mApz,
      &APZCTreeManager::UpdateFocusState,
      aRootLayerTreeId,
      aOriginatingLayersId,
      aFocusTarget));
}

void
APZUpdater::UpdateHitTestingTree(LayersId aRootLayerTreeId,
                                 Layer* aRoot,
                                 bool aIsFirstPaint,
                                 LayersId aOriginatingLayersId,
                                 uint32_t aPaintSequenceNumber)
{
=======
  UpdaterQueueSelector selector(aOriginatingWrRootId.mLayersId);
  if (aFocusTarget.mData.is<FocusTarget::ScrollTargets>()) {
    const FocusTarget::ScrollTargets& targets =
        aFocusTarget.mData.as<FocusTarget::ScrollTargets>();
    if (targets.mHorizontalRenderRoot) {
      selector.mRenderRoots += *targets.mHorizontalRenderRoot;
    }
    if (targets.mVerticalRenderRoot) {
      selector.mRenderRoots += *targets.mVerticalRenderRoot;
    }
  } else {
    selector.mRenderRoots += aOriginatingWrRootId.mRenderRoot;
  }
  RunOnUpdaterThread(selector,
                     NewRunnableMethod<LayersId, LayersId, FocusTarget>(
                         "APZUpdater::UpdateFocusState", mApz,
                         &APZCTreeManager::UpdateFocusState, aRootLayerTreeId,
                         aOriginatingWrRootId.mLayersId, aFocusTarget));
}

void APZUpdater::UpdateHitTestingTree(Layer* aRoot, bool aIsFirstPaint,
                                      LayersId aOriginatingWrRootId,
                                      uint32_t aPaintSequenceNumber) {
>>>>>>> upstream-releases
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  AssertOnUpdaterThread();
<<<<<<< HEAD
  mApz->UpdateHitTestingTree(aRootLayerTreeId, aRoot, aIsFirstPaint,
                             aOriginatingLayersId, aPaintSequenceNumber);
||||||| merged common ancestors
  mApz->UpdateHitTestingTree(aRootLayerTreeId, aRoot, aIsFirstPaint,
      aOriginatingLayersId, aPaintSequenceNumber);
=======
  mApz->UpdateHitTestingTree(aRoot, aIsFirstPaint, aOriginatingWrRootId,
                             aPaintSequenceNumber);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void APZUpdater::UpdateScrollDataAndTreeState(
    LayersId aRootLayerTreeId, LayersId aOriginatingLayersId,
    const wr::Epoch& aEpoch, WebRenderScrollData&& aScrollData) {
||||||| merged common ancestors
void
APZUpdater::UpdateScrollDataAndTreeState(LayersId aRootLayerTreeId,
                                         LayersId aOriginatingLayersId,
                                         const wr::Epoch& aEpoch,
                                         WebRenderScrollData&& aScrollData)
{
=======
void APZUpdater::UpdateScrollDataAndTreeState(
    WRRootId aRootLayerTreeId, WRRootId aOriginatingWrRootId,
    const wr::Epoch& aEpoch, WebRenderScrollData&& aScrollData) {
>>>>>>> upstream-releases
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  RefPtr<APZUpdater> self = this;
  // Insert an epoch requirement update into the queue, so that
  // tasks inserted into the queue after this point only get executed
  // once the epoch requirement is satisfied. In particular, the
  // UpdateHitTestingTree call below needs to wait until the epoch requirement
  // is satisfied, which is why it is a separate task in the queue.
<<<<<<< HEAD
  RunOnUpdaterThread(
      aOriginatingLayersId,
      NS_NewRunnableFunction("APZUpdater::UpdateEpochRequirement", [=]() {
        if (aRootLayerTreeId == aOriginatingLayersId) {
          self->mEpochData[aOriginatingLayersId].mIsRoot = true;
        }
        self->mEpochData[aOriginatingLayersId].mRequired = aEpoch;
      }));
  RunOnUpdaterThread(
      aOriginatingLayersId,
      NS_NewRunnableFunction(
          "APZUpdater::UpdateHitTestingTree",
          [=, aScrollData = std::move(aScrollData)]() {
            self->mApz->UpdateFocusState(aRootLayerTreeId, aOriginatingLayersId,
                                         aScrollData.GetFocusTarget());

            self->mScrollData[aOriginatingLayersId] = aScrollData;
            auto root = self->mScrollData.find(aRootLayerTreeId);
            if (root == self->mScrollData.end()) {
              return;
            }
            self->mApz->UpdateHitTestingTree(
                aRootLayerTreeId,
                WebRenderScrollDataWrapper(*self, &(root->second)),
                aScrollData.IsFirstPaint(), aOriginatingLayersId,
                aScrollData.GetPaintSequenceNumber());
          }));
}

void APZUpdater::UpdateScrollOffsets(LayersId aRootLayerTreeId,
                                     LayersId aOriginatingLayersId,
                                     ScrollUpdatesMap&& aUpdates,
                                     uint32_t aPaintSequenceNumber) {
||||||| merged common ancestors
  RunOnUpdaterThread(aOriginatingLayersId, NS_NewRunnableFunction(
    "APZUpdater::UpdateEpochRequirement",
    [=]() {
      if (aRootLayerTreeId == aOriginatingLayersId) {
        self->mEpochData[aOriginatingLayersId].mIsRoot = true;
      }
      self->mEpochData[aOriginatingLayersId].mRequired = aEpoch;
    }
  ));
  RunOnUpdaterThread(aOriginatingLayersId, NS_NewRunnableFunction(
    "APZUpdater::UpdateHitTestingTree",
    [=,aScrollData=std::move(aScrollData)]() {
      self->mApz->UpdateFocusState(aRootLayerTreeId,
          aOriginatingLayersId, aScrollData.GetFocusTarget());

      self->mScrollData[aOriginatingLayersId] = aScrollData;
      auto root = self->mScrollData.find(aRootLayerTreeId);
      if (root == self->mScrollData.end()) {
        return;
      }
      self->mApz->UpdateHitTestingTree(aRootLayerTreeId,
          WebRenderScrollDataWrapper(*self, &(root->second)),
          aScrollData.IsFirstPaint(), aOriginatingLayersId,
          aScrollData.GetPaintSequenceNumber());
    }
  ));
}

void
APZUpdater::UpdateScrollOffsets(LayersId aRootLayerTreeId,
                                LayersId aOriginatingLayersId,
                                ScrollUpdatesMap&& aUpdates,
                                uint32_t aPaintSequenceNumber)
{
=======
  RunOnUpdaterThread(
      UpdaterQueueSelector(aOriginatingWrRootId),
      NS_NewRunnableFunction("APZUpdater::UpdateEpochRequirement", [=]() {
        if (aRootLayerTreeId == aOriginatingWrRootId) {
          self->mEpochData[aOriginatingWrRootId].mIsRoot = true;
        }
        self->mEpochData[aOriginatingWrRootId].mRequired = aEpoch;
      }));
  RunOnUpdaterThread(UpdaterQueueSelector(aOriginatingWrRootId),
                     NS_NewRunnableFunction(
                         "APZUpdater::UpdateHitTestingTree",
                         [=, aScrollData = std::move(aScrollData)]() {
                           self->mScrollData[aOriginatingWrRootId] =
                               aScrollData;
                           auto root = self->mScrollData.find(aRootLayerTreeId);
                           if (root == self->mScrollData.end()) {
                             return;
                           }
                           self->mApz->UpdateHitTestingTree(
                               WebRenderScrollDataWrapper(
                                   *self, aRootLayerTreeId, &(root->second)),
                               aScrollData.IsFirstPaint(), aOriginatingWrRootId,
                               aScrollData.GetPaintSequenceNumber());
                         }));
}

void APZUpdater::UpdateScrollOffsets(WRRootId aRootLayerTreeId,
                                     WRRootId aOriginatingWrRootId,
                                     ScrollUpdatesMap&& aUpdates,
                                     uint32_t aPaintSequenceNumber) {
>>>>>>> upstream-releases
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  RefPtr<APZUpdater> self = this;
<<<<<<< HEAD
  RunOnUpdaterThread(
      aOriginatingLayersId,
      NS_NewRunnableFunction(
          "APZUpdater::UpdateScrollOffsets",
          [=, updates = std::move(aUpdates)]() {
            self->mScrollData[aOriginatingLayersId].ApplyUpdates(
                updates, aPaintSequenceNumber);
            auto root = self->mScrollData.find(aRootLayerTreeId);
            if (root == self->mScrollData.end()) {
              return;
            }
            self->mApz->UpdateHitTestingTree(
                aRootLayerTreeId,
                WebRenderScrollDataWrapper(*self, &(root->second)),
                /*isFirstPaint*/ false, aOriginatingLayersId,
                aPaintSequenceNumber);
          }));
}

void APZUpdater::NotifyLayerTreeAdopted(LayersId aLayersId,
                                        const RefPtr<APZUpdater>& aOldUpdater) {
||||||| merged common ancestors
  RunOnUpdaterThread(aOriginatingLayersId, NS_NewRunnableFunction(
    "APZUpdater::UpdateScrollOffsets",
    [=,updates=std::move(aUpdates)]() {
      self->mScrollData[aOriginatingLayersId].ApplyUpdates(updates, aPaintSequenceNumber);
      auto root = self->mScrollData.find(aRootLayerTreeId);
      if (root == self->mScrollData.end()) {
        return;
      }
      self->mApz->UpdateHitTestingTree(aRootLayerTreeId,
          WebRenderScrollDataWrapper(*self, &(root->second)),
          /*isFirstPaint*/ false, aOriginatingLayersId,
          aPaintSequenceNumber);
    }
  ));
}

void
APZUpdater::NotifyLayerTreeAdopted(LayersId aLayersId,
                                   const RefPtr<APZUpdater>& aOldUpdater)
{
=======
  RunOnUpdaterThread(UpdaterQueueSelector(aOriginatingWrRootId),
                     NS_NewRunnableFunction(
                         "APZUpdater::UpdateScrollOffsets",
                         [=, updates = std::move(aUpdates)]() {
                           self->mScrollData[aOriginatingWrRootId].ApplyUpdates(
                               updates, aPaintSequenceNumber);
                           auto root = self->mScrollData.find(aRootLayerTreeId);
                           if (root == self->mScrollData.end()) {
                             return;
                           }
                           self->mApz->UpdateHitTestingTree(
                               WebRenderScrollDataWrapper(
                                   *self, aRootLayerTreeId, &(root->second)),
                               /*isFirstPaint*/ false, aOriginatingWrRootId,
                               aPaintSequenceNumber);
                         }));
}

void APZUpdater::NotifyLayerTreeAdopted(WRRootId aWrRootId,
                                        const RefPtr<APZUpdater>& aOldUpdater) {
>>>>>>> upstream-releases
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
<<<<<<< HEAD
  RunOnUpdaterThread(aLayersId,
                     NewRunnableMethod<LayersId, RefPtr<APZCTreeManager>>(
                         "APZUpdater::NotifyLayerTreeAdopted", mApz,
                         &APZCTreeManager::NotifyLayerTreeAdopted, aLayersId,
                         aOldUpdater ? aOldUpdater->mApz : nullptr));
||||||| merged common ancestors
  RunOnUpdaterThread(aLayersId, NewRunnableMethod<LayersId, RefPtr<APZCTreeManager>>(
      "APZUpdater::NotifyLayerTreeAdopted",
      mApz,
      &APZCTreeManager::NotifyLayerTreeAdopted,
      aLayersId,
      aOldUpdater ? aOldUpdater->mApz : nullptr));
=======
  RunOnUpdaterThread(
      UpdaterQueueSelector(aWrRootId),
      NewRunnableMethod<LayersId, RefPtr<APZCTreeManager>>(
          "APZUpdater::NotifyLayerTreeAdopted", mApz,
          &APZCTreeManager::NotifyLayerTreeAdopted, aWrRootId.mLayersId,
          aOldUpdater ? aOldUpdater->mApz : nullptr));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void APZUpdater::NotifyLayerTreeRemoved(LayersId aLayersId) {
||||||| merged common ancestors
void
APZUpdater::NotifyLayerTreeRemoved(LayersId aLayersId)
{
=======
void APZUpdater::NotifyLayerTreeRemoved(WRRootId aWrRootId) {
>>>>>>> upstream-releases
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  RefPtr<APZUpdater> self = this;
<<<<<<< HEAD
  RunOnUpdaterThread(
      aLayersId,
      NS_NewRunnableFunction("APZUpdater::NotifyLayerTreeRemoved", [=]() {
        self->mEpochData.erase(aLayersId);
        self->mScrollData.erase(aLayersId);
        self->mApz->NotifyLayerTreeRemoved(aLayersId);
      }));
||||||| merged common ancestors
  RunOnUpdaterThread(aLayersId, NS_NewRunnableFunction(
    "APZUpdater::NotifyLayerTreeRemoved",
    [=]() {
      self->mEpochData.erase(aLayersId);
      self->mScrollData.erase(aLayersId);
      self->mApz->NotifyLayerTreeRemoved(aLayersId);
    }
  ));
=======
  RunOnUpdaterThread(
      UpdaterQueueSelector(aWrRootId),
      NS_NewRunnableFunction("APZUpdater::NotifyLayerTreeRemoved", [=]() {
        self->mEpochData.erase(aWrRootId);
        self->mScrollData.erase(aWrRootId);
        self->mApz->NotifyLayerTreeRemoved(aWrRootId.mLayersId);
      }));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool APZUpdater::GetAPZTestData(LayersId aLayersId, APZTestData* aOutData) {
||||||| merged common ancestors
bool
APZUpdater::GetAPZTestData(LayersId aLayersId,
                           APZTestData* aOutData)
{
=======
bool APZUpdater::GetAPZTestData(WRRootId aWrRootId, APZTestData* aOutData) {
>>>>>>> upstream-releases
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());

  RefPtr<APZCTreeManager> apz = mApz;
  bool ret = false;
  SynchronousTask waiter("APZUpdater::GetAPZTestData");
<<<<<<< HEAD
  RunOnUpdaterThread(
      aLayersId, NS_NewRunnableFunction("APZUpdater::GetAPZTestData", [&]() {
        AutoCompleteTask notifier(&waiter);
        ret = apz->GetAPZTestData(aLayersId, aOutData);
      }));
||||||| merged common ancestors
  RunOnUpdaterThread(aLayersId, NS_NewRunnableFunction(
    "APZUpdater::GetAPZTestData",
    [&]() {
      AutoCompleteTask notifier(&waiter);
      ret = apz->GetAPZTestData(aLayersId, aOutData);
    }
  ));
=======
  RunOnUpdaterThread(
      UpdaterQueueSelector(aWrRootId),
      NS_NewRunnableFunction("APZUpdater::GetAPZTestData", [&]() {
        AutoCompleteTask notifier(&waiter);
        ret = apz->GetAPZTestData(aWrRootId.mLayersId, aOutData);
      }));
>>>>>>> upstream-releases

  // Wait until the task posted above has run and populated aOutData and ret
  waiter.Wait();

  return ret;
}

<<<<<<< HEAD
void APZUpdater::SetTestAsyncScrollOffset(
    LayersId aLayersId, const ScrollableLayerGuid::ViewID& aScrollId,
    const CSSPoint& aOffset) {
||||||| merged common ancestors
void
APZUpdater::SetTestAsyncScrollOffset(LayersId aLayersId,
                                     const FrameMetrics::ViewID& aScrollId,
                                     const CSSPoint& aOffset)
{
=======
void APZUpdater::SetTestAsyncScrollOffset(
    WRRootId aWrRootId, const ScrollableLayerGuid::ViewID& aScrollId,
    const CSSPoint& aOffset) {
>>>>>>> upstream-releases
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  RefPtr<APZCTreeManager> apz = mApz;
<<<<<<< HEAD
  RunOnUpdaterThread(
      aLayersId,
      NS_NewRunnableFunction("APZUpdater::SetTestAsyncScrollOffset", [=]() {
        RefPtr<AsyncPanZoomController> apzc =
            apz->GetTargetAPZC(aLayersId, aScrollId);
        if (apzc) {
          apzc->SetTestAsyncScrollOffset(aOffset);
        } else {
          NS_WARNING("Unable to find APZC in SetTestAsyncScrollOffset");
        }
      }));
||||||| merged common ancestors
  RunOnUpdaterThread(aLayersId, NS_NewRunnableFunction(
    "APZUpdater::SetTestAsyncScrollOffset",
    [=]() {
      RefPtr<AsyncPanZoomController> apzc = apz->GetTargetAPZC(aLayersId, aScrollId);
      if (apzc) {
        apzc->SetTestAsyncScrollOffset(aOffset);
      } else {
        NS_WARNING("Unable to find APZC in SetTestAsyncScrollOffset");
      }
    }
  ));
=======
  RunOnUpdaterThread(
      UpdaterQueueSelector(aWrRootId),
      NS_NewRunnableFunction("APZUpdater::SetTestAsyncScrollOffset", [=]() {
        RefPtr<AsyncPanZoomController> apzc =
            apz->GetTargetAPZC(aWrRootId.mLayersId, aScrollId);
        if (apzc) {
          apzc->SetTestAsyncScrollOffset(aOffset);
        } else {
          NS_WARNING("Unable to find APZC in SetTestAsyncScrollOffset");
        }
      }));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void APZUpdater::SetTestAsyncZoom(LayersId aLayersId,
                                  const ScrollableLayerGuid::ViewID& aScrollId,
                                  const LayerToParentLayerScale& aZoom) {
||||||| merged common ancestors
void
APZUpdater::SetTestAsyncZoom(LayersId aLayersId,
                             const FrameMetrics::ViewID& aScrollId,
                             const LayerToParentLayerScale& aZoom)
{
=======
void APZUpdater::SetTestAsyncZoom(WRRootId aWrRootId,
                                  const ScrollableLayerGuid::ViewID& aScrollId,
                                  const LayerToParentLayerScale& aZoom) {
>>>>>>> upstream-releases
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  RefPtr<APZCTreeManager> apz = mApz;
<<<<<<< HEAD
  RunOnUpdaterThread(
      aLayersId, NS_NewRunnableFunction("APZUpdater::SetTestAsyncZoom", [=]() {
        RefPtr<AsyncPanZoomController> apzc =
            apz->GetTargetAPZC(aLayersId, aScrollId);
        if (apzc) {
          apzc->SetTestAsyncZoom(aZoom);
        } else {
          NS_WARNING("Unable to find APZC in SetTestAsyncZoom");
        }
      }));
||||||| merged common ancestors
  RunOnUpdaterThread(aLayersId, NS_NewRunnableFunction(
    "APZUpdater::SetTestAsyncZoom",
    [=]() {
      RefPtr<AsyncPanZoomController> apzc = apz->GetTargetAPZC(aLayersId, aScrollId);
      if (apzc) {
        apzc->SetTestAsyncZoom(aZoom);
      } else {
        NS_WARNING("Unable to find APZC in SetTestAsyncZoom");
      }
    }
  ));
=======
  RunOnUpdaterThread(
      UpdaterQueueSelector(aWrRootId),
      NS_NewRunnableFunction("APZUpdater::SetTestAsyncZoom", [=]() {
        RefPtr<AsyncPanZoomController> apzc =
            apz->GetTargetAPZC(aWrRootId.mLayersId, aScrollId);
        if (apzc) {
          apzc->SetTestAsyncZoom(aZoom);
        } else {
          NS_WARNING("Unable to find APZC in SetTestAsyncZoom");
        }
      }));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
const WebRenderScrollData* APZUpdater::GetScrollData(LayersId aLayersId) const {
||||||| merged common ancestors
const WebRenderScrollData*
APZUpdater::GetScrollData(LayersId aLayersId) const
{
=======
const WebRenderScrollData* APZUpdater::GetScrollData(WRRootId aWrRootId) const {
>>>>>>> upstream-releases
  AssertOnUpdaterThread();
  auto it = mScrollData.find(aWrRootId);
  return (it == mScrollData.end() ? nullptr : &(it->second));
}

void APZUpdater::AssertOnUpdaterThread() const {
  if (APZThreadUtils::GetThreadAssertionsEnabled()) {
    MOZ_ASSERT(IsUpdaterThread());
  }
}

<<<<<<< HEAD
void APZUpdater::RunOnUpdaterThread(LayersId aLayersId,
                                    already_AddRefed<Runnable> aTask) {
||||||| merged common ancestors
void
APZUpdater::RunOnUpdaterThread(LayersId aLayersId, already_AddRefed<Runnable> aTask)
{
=======
void APZUpdater::RunOnUpdaterThread(UpdaterQueueSelector aSelector,
                                    already_AddRefed<Runnable> aTask) {
>>>>>>> upstream-releases
  RefPtr<Runnable> task = aTask;

  // In the scenario where UsingWebRenderUpdaterThread() is true, this function
  // might get called early (before mUpdaterThreadId is set). In that case
  // IsUpdaterThread() will return false and we'll queue the task onto
  // mUpdaterQueue. This is fine; the task is still guaranteed to run (barring
  // catastrophic failure) because the WakeSceneBuilder call will still trigger
  // the callback to run tasks.

  if (IsUpdaterThread()) {
    task->Run();
    return;
  }

  if (UsingWebRenderUpdaterThread()) {
    // If the updater thread is a WebRender thread, and we're not on it
    // right now, save the task in the queue. We will run tasks from the queue
    // during the callback from the updater thread, which we trigger by the
    // call to WakeSceneBuilder.

    bool sendWakeMessage = true;
    {  // scope lock
      MutexAutoLock lock(mQueueLock);
      wr::RenderRootSet alreadyWoken;

      // What we're doing here is trying to avoid sending redundant
      // WakeSceneBuilder messages. If another task exists with our
      // layersId/renderRoots combination, then we know that either that message
      // is going to be processed soon, or it's blocked on an epoch update, and
      // in both of those cases sending a WakeSceneBuilder message won't do
      // anything, and our message is destined to be blocked behind it. However,
      // imagine the following queue (assume everything as the same layersId):
      //
      // q[0]    (A)
      // q[1]    (A,B)
      // q[2] -> (B,C) // This is what we want to put in the queue
      //
      // We could go two routes in this case: (I) elide the WakeSceneBuilder
      // message if *any* of our renderRoots are already present in q, since we
      // won't go until they're all unblocked, or (II) elide it only if *all* of
      // our renderRoots are already present in q.
      //
      // If we go with (I), then if A needs an epoch update, adding (B,C)
      // wouldn't send a WakeSceneBuilder message because (A,B) is in the queue
      // (intersecting on B). But since (A,B) is only blocked on an epoch update
      // for A, (B,C) *could* run if we sent a wake message. Thus, (I) means:
      // - Fewer spurious WakeSceneBuilder messages
      // - Potential for unnecessarily blocking certain messages
      //
      // If we went with (II), then (B,C) would run as early as possible, but we
      // would send WakeSceneBuilder messages for all three items in the queue.
      // Thus, (II) means:
      // - More spurious WakeSceneBuilderMessages
      // - Potential for violating people's ordering assumptions (i.e., that
      // q[2] would run
      //   after q[1])
      //
      // We're electing to go with option (II), but we might revisit it if/when
      // we have more than two documents. (Because it doesn't matter much before
      // then.)
      for (const auto& queuedTask : mUpdaterQueue) {
        if (queuedTask.mSelector.mLayersId == aSelector.mLayersId) {
          alreadyWoken +=
              (queuedTask.mSelector.mRenderRoots & aSelector.mRenderRoots);
          break;
        }
      }
<<<<<<< HEAD
      mUpdaterQueue.push_back(QueuedTask{aLayersId, task});
||||||| merged common ancestors
      mUpdaterQueue.push_back(QueuedTask { aLayersId, task });
=======
      if (alreadyWoken == aSelector.mRenderRoots) {
        sendWakeMessage = false;
      }
      mUpdaterQueue.push_back(QueuedTask{aSelector, task});
>>>>>>> upstream-releases
    }
    if (sendWakeMessage) {
      // All the RenderRoots share a single scene builder thread, so we can
      // just send the message to the default RenderRoot's API instead of
      // sending one for each unwoken RenderRoot.
      RefPtr<wr::WebRenderAPI> api =
          mApz->GetWebRenderAPI(wr::RenderRoot::Default);
      if (api) {
        api->WakeSceneBuilder();
      } else {
        // Not sure if this can happen, but it might be possible. If it does,
        // the task is in the queue, but if we didn't get a WebRenderAPI it
        // might never run, or it might run later if we manage to get a
        // WebRenderAPI later. For now let's just emit a warning, this can
        // probably be upgraded to an assert later.
        NS_WARNING("Possibly dropping task posted to updater thread");
      }
    }
    return;
  }

  if (MessageLoop* loop = CompositorThreadHolder::Loop()) {
    loop->PostTask(task.forget());
  } else {
    // Could happen during startup
    NS_WARNING("Dropping task posted to updater thread");
  }
}

bool APZUpdater::IsUpdaterThread() const {
  if (UsingWebRenderUpdaterThread()) {
    // If the updater thread id isn't set yet then we cannot be running on the
    // updater thread (because we will have the thread id before we run any
    // C++ code on it, and this function is only ever invoked from C++ code),
    // so return false in that scenario.
    MutexAutoLock lock(mThreadIdLock);
    return mUpdaterThreadId && PlatformThread::CurrentId() == *mUpdaterThreadId;
  }
  return CompositorThreadHolder::IsInCompositorThread();
}

<<<<<<< HEAD
void APZUpdater::RunOnControllerThread(LayersId aLayersId,
                                       already_AddRefed<Runnable> aTask) {
||||||| merged common ancestors
void
APZUpdater::RunOnControllerThread(LayersId aLayersId, already_AddRefed<Runnable> aTask)
{
=======
void APZUpdater::RunOnControllerThread(UpdaterQueueSelector aSelector,
                                       already_AddRefed<Runnable> aTask) {
>>>>>>> upstream-releases
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());

<<<<<<< HEAD
  RefPtr<Runnable> task = aTask;

  RunOnUpdaterThread(aLayersId,
                     NewRunnableFunction("APZUpdater::RunOnControllerThread",
                                         &APZThreadUtils::RunOnControllerThread,
                                         std::move(task)));
||||||| merged common ancestors
  RunOnUpdaterThread(aLayersId, NewRunnableFunction(
      "APZUpdater::RunOnControllerThread",
      &APZThreadUtils::RunOnControllerThread,
      std::move(aTask)));
=======
  RefPtr<Runnable> task = aTask;

  RunOnUpdaterThread(aSelector,
                     NewRunnableFunction("APZUpdater::RunOnControllerThread",
                                         &APZThreadUtils::RunOnControllerThread,
                                         std::move(task)));
>>>>>>> upstream-releases
}

bool APZUpdater::UsingWebRenderUpdaterThread() const {
  return mIsUsingWebRender;
}

<<<<<<< HEAD
/*static*/ already_AddRefed<APZUpdater> APZUpdater::GetUpdater(
    const wr::WrWindowId& aWindowId) {
||||||| merged common ancestors
/*static*/ already_AddRefed<APZUpdater>
APZUpdater::GetUpdater(const wr::WrWindowId& aWindowId)
{
=======
/*static*/
already_AddRefed<APZUpdater> APZUpdater::GetUpdater(
    const wr::WrWindowId& aWindowId) {
>>>>>>> upstream-releases
  RefPtr<APZUpdater> updater;
  StaticMutexAutoLock lock(sWindowIdLock);
  if (sWindowIdMap) {
    auto it = sWindowIdMap->find(wr::AsUint64(aWindowId));
    if (it != sWindowIdMap->end()) {
      updater = it->second;
    }
  }
  return updater.forget();
}

<<<<<<< HEAD
void APZUpdater::ProcessQueue() {
  {  // scope lock to check for emptiness
||||||| merged common ancestors
void
APZUpdater::ProcessQueue()
{
  { // scope lock to check for emptiness
=======
void APZUpdater::ProcessQueue() {
  MOZ_ASSERT(!mDestroyed);

  {  // scope lock to check for emptiness
>>>>>>> upstream-releases
    MutexAutoLock lock(mQueueLock);
    if (mUpdaterQueue.empty()) {
      return;
    }
  }

  std::deque<QueuedTask> blockedTasks;
  while (true) {
    QueuedTask task;

    {  // scope lock to extract a task
      MutexAutoLock lock(mQueueLock);
      if (mUpdaterQueue.empty()) {
        // If we're done processing mUpdaterQueue, swap the tasks that are
        // still blocked back in and finish
        std::swap(mUpdaterQueue, blockedTasks);
        break;
      }
      task = mUpdaterQueue.front();
      mUpdaterQueue.pop_front();
    }

    // We check the task to see if it is blocked. Note that while this
    // ProcessQueue function is executing, a particular WRRoot id cannot go
    // from blocked to unblocked, because only CompleteSceneSwap can unblock
    // a WRRoot id, and that also runs on the updater thread. If somehow
    // a WRRoot id gets unblocked while we're processing the queue, then it
    // might result in tasks getting executed out of order.

    bool blocked = false;
    for (wr::RenderRoot root : task.mSelector.mRenderRoots) {
      WRRootId selector = WRRootId(task.mSelector.mLayersId, root);
      auto it = mEpochData.find(selector);
      if (it != mEpochData.end() && it->second.IsBlocked()) {
        blocked = true;
        break;
      }
    }
    if (blocked) {
      // If this task is blocked, put it into the blockedTasks queue that
      // we will replace mUpdaterQueue with
      blockedTasks.push_back(task);
    } else {
      // Run and discard the task
      task.mRunnable->Run();
    }
  }

<<<<<<< HEAD
APZUpdater::EpochState::EpochState() : mRequired{0}, mIsRoot(false) {}
||||||| merged common ancestors
APZUpdater::EpochState::EpochState()
  : mRequired{0}
  , mIsRoot(false)
{
}
=======
  if (mDestroyed) {
    // If we get here, then we must have just run the ClearTree task for
    // this updater. There might be tasks in the queue from content subtrees
    // of this window that are blocked due to stale epochs. This can happen
    // if the tasks were queued after the root pipeline was removed in
    // WebRender, which prevents scene builds (and therefore prevents us
    // from getting updated epochs via CompleteSceneSwap). See bug 1465658
    // comment 43 for some more context.
    // To avoid leaking these tasks, we discard the contents of the queue.
    // This happens during window shutdown so if we don't run the tasks it's
    // not going to matter much.
    MutexAutoLock lock(mQueueLock);
    if (!mUpdaterQueue.empty()) {
      mUpdaterQueue.clear();
    }
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool APZUpdater::EpochState::IsBlocked() const {
||||||| merged common ancestors
bool
APZUpdater::EpochState::IsBlocked() const
{
=======
APZUpdater::EpochState::EpochState() : mRequired{0}, mIsRoot(false) {}

bool APZUpdater::EpochState::IsBlocked() const {
>>>>>>> upstream-releases
  // The root is a special case because we basically assume it is "visible"
  // even before it is built for the first time. This is because building the
  // scene automatically makes it visible, and we need to make sure the APZ
  // scroll data gets applied atomically with that happening.
  //
  // Layer subtrees on the other hand do not automatically become visible upon
  // being built, because there must be a another layer tree update to change
  // the visibility (i.e. an ancestor layer tree update that adds the necessary
  // reflayer to complete the chain of reflayers).
  //
  // So in the case of non-visible subtrees, we know that no hit-test will
  // actually end up hitting that subtree either before or after the scene swap,
  // because the subtree will remain non-visible. That in turns means that we
  // can apply the APZ scroll data for that subtree epoch before the scene is
  // built, because it's not going to get used anyway. And that means we don't
  // need to block the queue for non-visible subtrees. Which is a good thing,
  // because in practice it seems like we often have non-visible subtrees sent
  // to the compositor from content.
  if (mIsRoot && !mBuilt) {
    return true;
  }
  return mBuilt && (*mBuilt < mRequired);
}

}  // namespace layers
}  // namespace mozilla

// Rust callback implementations

void apz_register_updater(mozilla::wr::WrWindowId aWindowId) {
  mozilla::layers::APZUpdater::SetUpdaterThread(aWindowId);
}

void apz_pre_scene_swap(mozilla::wr::WrWindowId aWindowId) {
  mozilla::layers::APZUpdater::PrepareForSceneSwap(aWindowId);
}

void apz_post_scene_swap(mozilla::wr::WrWindowId aWindowId,
                         mozilla::wr::WrPipelineInfo aInfo) {
  mozilla::layers::APZUpdater::CompleteSceneSwap(aWindowId, aInfo);
  wr_pipeline_info_delete(aInfo);
}

void apz_run_updater(mozilla::wr::WrWindowId aWindowId) {
  mozilla::layers::APZUpdater::ProcessPendingTasks(aWindowId);
}

void apz_deregister_updater(mozilla::wr::WrWindowId aWindowId) {
  // Run anything that's still left.
  mozilla::layers::APZUpdater::ProcessPendingTasks(aWindowId);
}
