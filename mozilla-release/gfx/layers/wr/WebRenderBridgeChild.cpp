/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/layers/WebRenderBridgeChild.h"

#include "gfxPlatform.h"
#include "mozilla/StaticPrefs.h"
#include "mozilla/dom/TabGroup.h"
#include "mozilla/layers/CompositableClient.h"
#include "mozilla/layers/CompositorBridgeChild.h"
#include "mozilla/layers/ImageDataSerializer.h"
#include "mozilla/layers/IpcResourceUpdateQueue.h"
#include "mozilla/layers/StackingContextHelper.h"
#include "mozilla/layers/PTextureChild.h"
#include "mozilla/layers/WebRenderLayerManager.h"
#include "mozilla/webrender/WebRenderAPI.h"

namespace mozilla {
namespace layers {

using namespace mozilla::gfx;

WebRenderBridgeChild::WebRenderBridgeChild(const wr::PipelineId& aPipelineId)
<<<<<<< HEAD
    : mIsInTransaction(false),
      mIsInClearCachedResources(false),
      mIdNamespace{0},
      mResourceId(0),
      mPipelineId(aPipelineId),
      mManager(nullptr),
      mIPCOpen(false),
      mDestroyed(false),
      mFontKeysDeleted(0),
      mFontInstanceKeysDeleted(0) {}

WebRenderBridgeChild::~WebRenderBridgeChild() {
||||||| merged common ancestors
  : mIsInTransaction(false)
  , mIsInClearCachedResources(false)
  , mIdNamespace{0}
  , mResourceId(0)
  , mPipelineId(aPipelineId)
  , mManager(nullptr)
  , mIPCOpen(false)
  , mDestroyed(false)
  , mFontKeysDeleted(0)
  , mFontInstanceKeysDeleted(0)
{
}

WebRenderBridgeChild::~WebRenderBridgeChild()
{
=======
    : mIsInTransaction(false),
      mIsInClearCachedResources(false),
      mIdNamespace{0},
      mResourceId(0),
      mPipelineId(aPipelineId),
      mManager(nullptr),
      mIPCOpen(false),
      mDestroyed(false),
      mFontKeysDeleted(),
      mFontInstanceKeysDeleted() {}

WebRenderBridgeChild::~WebRenderBridgeChild() {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(mDestroyed);
}

void WebRenderBridgeChild::Destroy(bool aIsSync) {
  if (!IPCOpen()) {
    return;
  }

  DoDestroy();

  if (aIsSync) {
    SendShutdownSync();
  } else {
    SendShutdown();
  }
}

void WebRenderBridgeChild::ActorDestroy(ActorDestroyReason why) { DoDestroy(); }

void WebRenderBridgeChild::DoDestroy() {
  if (RefCountedShm::IsValid(mResourceShm) &&
      RefCountedShm::Release(mResourceShm) == 0) {
    RefCountedShm::Dealloc(this, mResourceShm);
    mResourceShm = RefCountedShmem();
  }

  // mDestroyed is used to prevent calling Send__delete__() twice.
  // When this function is called from CompositorBridgeChild::Destroy().
  // mActiveResourceTracker is not cleared here, since it is
  // used by PersistentBufferProviderShared.
  mDestroyed = true;
  mManager = nullptr;
}

<<<<<<< HEAD
void WebRenderBridgeChild::AddWebRenderParentCommand(
    const WebRenderParentCommand& aCmd) {
  mParentCommands.AppendElement(aCmd);
||||||| merged common ancestors
void
WebRenderBridgeChild::AddWebRenderParentCommand(const WebRenderParentCommand& aCmd)
{
  mParentCommands.AppendElement(aCmd);
=======
void WebRenderBridgeChild::AddWebRenderParentCommand(
    const WebRenderParentCommand& aCmd, wr::RenderRoot aRenderRoot) {
  MOZ_ASSERT(aRenderRoot == wr::RenderRoot::Default ||
             (XRE_IsParentProcess() &&
              StaticPrefs::gfx_webrender_split_render_roots()));
  mParentCommands[aRenderRoot].AppendElement(aCmd);
>>>>>>> upstream-releases
}

void WebRenderBridgeChild::BeginTransaction() {
  MOZ_ASSERT(!mDestroyed);

  UpdateFwdTransactionId();
  mIsInTransaction = true;
}

<<<<<<< HEAD
void WebRenderBridgeChild::UpdateResources(
    wr::IpcResourceUpdateQueue& aResources) {
||||||| merged common ancestors
void
WebRenderBridgeChild::UpdateResources(wr::IpcResourceUpdateQueue& aResources)
{
=======
void WebRenderBridgeChild::UpdateResources(
    wr::IpcResourceUpdateQueue& aResources, wr::RenderRoot aRenderRoot) {
>>>>>>> upstream-releases
  if (!IPCOpen()) {
    aResources.Clear();
    return;
  }

  if (aResources.IsEmpty()) {
    return;
  }

  nsTArray<OpUpdateResource> resourceUpdates;
  nsTArray<RefCountedShmem> smallShmems;
  nsTArray<ipc::Shmem> largeShmems;
  aResources.Flush(resourceUpdates, smallShmems, largeShmems);

<<<<<<< HEAD
  this->SendUpdateResources(resourceUpdates, smallShmems, largeShmems);
||||||| merged common ancestors
  this->SendUpdateResources(resourceUpdates, std::move(smallShmems), largeShmems);
=======
  this->SendUpdateResources(resourceUpdates, smallShmems,
                            std::move(largeShmems), aRenderRoot);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void WebRenderBridgeChild::EndTransaction(
    const wr::LayoutSize& aContentSize, wr::BuiltDisplayList& aDL,
    wr::IpcResourceUpdateQueue& aResources, const gfx::IntSize& aSize,
    TransactionId aTransactionId, const WebRenderScrollData& aScrollData,
    bool aContainsSVGGroup, const mozilla::VsyncId& aVsyncId,
    const mozilla::TimeStamp& aRefreshStartTime,
    const mozilla::TimeStamp& aTxnStartTime, const nsCString& aTxnURL) {
||||||| merged common ancestors
void
WebRenderBridgeChild::EndTransaction(const wr::LayoutSize& aContentSize,
                                     wr::BuiltDisplayList& aDL,
                                     wr::IpcResourceUpdateQueue& aResources,
                                     const gfx::IntSize& aSize,
                                     TransactionId aTransactionId,
                                     const WebRenderScrollData& aScrollData,
                                     bool aContainsSVGGroup,
                                     const mozilla::TimeStamp& aRefreshStartTime,
                                     const mozilla::TimeStamp& aTxnStartTime)
{
=======
void WebRenderBridgeChild::EndTransaction(
    nsTArray<RenderRootDisplayListData>& aRenderRoots,
    TransactionId aTransactionId, bool aContainsSVGGroup,
    const mozilla::VsyncId& aVsyncId, const mozilla::TimeStamp& aVsyncStartTime,
    const mozilla::TimeStamp& aRefreshStartTime,
    const mozilla::TimeStamp& aTxnStartTime, const nsCString& aTxnURL) {
>>>>>>> upstream-releases
  MOZ_ASSERT(!mDestroyed);
  MOZ_ASSERT(mIsInTransaction);

  TimeStamp fwdTime = TimeStamp::Now();

<<<<<<< HEAD
  TimeStamp fwdTime = TimeStamp::Now();
||||||| merged common ancestors
  TimeStamp fwdTime;
#if defined(ENABLE_FRAME_LATENCY_LOG)
  fwdTime = TimeStamp::Now();
#endif
=======
  for (auto& renderRoot : aRenderRoots) {
    MOZ_ASSERT(renderRoot.mRenderRoot == wr::RenderRoot::Default ||
               (XRE_IsParentProcess() &&
                StaticPrefs::gfx_webrender_split_render_roots()));
    renderRoot.mCommands = std::move(mParentCommands[renderRoot.mRenderRoot]);
  }
>>>>>>> upstream-releases

  nsTArray<CompositionPayload> payloads;
  if (mManager) {
    mManager->TakeCompositionPayloads(payloads);
  }

<<<<<<< HEAD
  this->SendSetDisplayList(aSize, mParentCommands, mDestroyedActors,
                           GetFwdTransactionId(), aTransactionId, aContentSize,
                           dlData, aDL.dl_desc, aScrollData, resourceUpdates,
                           smallShmems, largeShmems, mIdNamespace,
                           aContainsSVGGroup, aVsyncId, aRefreshStartTime,
                           aTxnStartTime, aTxnURL, fwdTime);
||||||| merged common ancestors
  this->SendSetDisplayList(aSize, mParentCommands, mDestroyedActors,
                           GetFwdTransactionId(), aTransactionId,
                           aContentSize, dlData, aDL.dl_desc, aScrollData,
                           std::move(resourceUpdates), std::move(smallShmems), largeShmems,
                           mIdNamespace, aContainsSVGGroup, aRefreshStartTime, aTxnStartTime, fwdTime);
=======
  this->SendSetDisplayList(std::move(aRenderRoots), mDestroyedActors,
                           GetFwdTransactionId(), aTransactionId, mIdNamespace,
                           aContainsSVGGroup, aVsyncId, aVsyncStartTime,
                           aRefreshStartTime, aTxnStartTime, aTxnURL, fwdTime,
                           payloads);
>>>>>>> upstream-releases

  // With multiple render roots, we may not have sent all of our
  // mParentCommands, so go ahead and go through our mParentCommands and ensure
  // they get sent.
  ProcessWebRenderParentCommands();
  mDestroyedActors.Clear();
  mIsInTransaction = false;
}

<<<<<<< HEAD
void WebRenderBridgeChild::EndEmptyTransaction(
    const FocusTarget& aFocusTarget, const ScrollUpdatesMap& aUpdates,
    Maybe<wr::IpcResourceUpdateQueue>& aResources,
    uint32_t aPaintSequenceNumber, TransactionId aTransactionId,
    const mozilla::VsyncId& aVsyncId,
    const mozilla::TimeStamp& aRefreshStartTime,
    const mozilla::TimeStamp& aTxnStartTime, const nsCString& aTxnURL) {
||||||| merged common ancestors
void
WebRenderBridgeChild::EndEmptyTransaction(const FocusTarget& aFocusTarget,
                                          const ScrollUpdatesMap& aUpdates,
                                          uint32_t aPaintSequenceNumber,
                                          TransactionId aTransactionId,
                                          const mozilla::TimeStamp& aRefreshStartTime,
                                          const mozilla::TimeStamp& aTxnStartTime)
{
=======
void WebRenderBridgeChild::EndEmptyTransaction(
    const FocusTarget& aFocusTarget,
    nsTArray<RenderRootUpdates>& aRenderRootUpdates,
    uint32_t aPaintSequenceNumber, TransactionId aTransactionId,
    const mozilla::VsyncId& aVsyncId, const mozilla::TimeStamp& aVsyncStartTime,
    const mozilla::TimeStamp& aRefreshStartTime,
    const mozilla::TimeStamp& aTxnStartTime, const nsCString& aTxnURL) {
>>>>>>> upstream-releases
  MOZ_ASSERT(!mDestroyed);
  MOZ_ASSERT(mIsInTransaction);

<<<<<<< HEAD
  TimeStamp fwdTime = TimeStamp::Now();

  nsTArray<OpUpdateResource> resourceUpdates;
  nsTArray<RefCountedShmem> smallShmems;
  nsTArray<ipc::Shmem> largeShmems;
  if (aResources) {
    aResources->Flush(resourceUpdates, smallShmems, largeShmems);
    aResources.reset();
  }
||||||| merged common ancestors
  TimeStamp fwdTime;
#if defined(ENABLE_FRAME_LATENCY_LOG)
  fwdTime = TimeStamp::Now();
#endif
=======
  TimeStamp fwdTime = TimeStamp::Now();

  for (auto& update : aRenderRootUpdates) {
    MOZ_ASSERT(update.mRenderRoot == wr::RenderRoot::Default ||
               (XRE_IsParentProcess() &&
                StaticPrefs::gfx_webrender_split_render_roots()));
    update.mCommands = std::move(mParentCommands[update.mRenderRoot]);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  this->SendEmptyTransaction(
      aFocusTarget, aUpdates, aPaintSequenceNumber, mParentCommands,
      mDestroyedActors, GetFwdTransactionId(), aTransactionId, resourceUpdates,
      smallShmems, largeShmems, mIdNamespace, aVsyncId, aRefreshStartTime,
      aTxnStartTime, aTxnURL, fwdTime);
  mParentCommands.Clear();
||||||| merged common ancestors
  this->SendEmptyTransaction(aFocusTarget, aUpdates, aPaintSequenceNumber,
                             mParentCommands, mDestroyedActors,
                             GetFwdTransactionId(), aTransactionId,
                             mIdNamespace, aRefreshStartTime, aTxnStartTime, fwdTime);
  mParentCommands.Clear();
=======
  nsTArray<CompositionPayload> payloads;
  if (mManager) {
    mManager->TakeCompositionPayloads(payloads);
  }

  this->SendEmptyTransaction(
      aFocusTarget, aPaintSequenceNumber, std::move(aRenderRootUpdates),
      mDestroyedActors, GetFwdTransactionId(), aTransactionId, mIdNamespace,
      aVsyncId, aVsyncStartTime, aRefreshStartTime, aTxnStartTime, aTxnURL,
      fwdTime, payloads);

  // With multiple render roots, we may not have sent all of our
  // mParentCommands, so go ahead and go through our mParentCommands and ensure
  // they get sent.
  ProcessWebRenderParentCommands();
>>>>>>> upstream-releases
  mDestroyedActors.Clear();
  mIsInTransaction = false;
}

void WebRenderBridgeChild::ProcessWebRenderParentCommands() {
  MOZ_ASSERT(!mDestroyed);

  for (auto renderRoot : wr::kRenderRoots) {
    if (!mParentCommands[renderRoot].IsEmpty()) {
      MOZ_ASSERT(renderRoot == wr::RenderRoot::Default ||
                 StaticPrefs::gfx_webrender_split_render_roots());
      this->SendParentCommands(mParentCommands[renderRoot], renderRoot);
      mParentCommands[renderRoot].Clear();
    }
  }
}

<<<<<<< HEAD
void WebRenderBridgeChild::AddPipelineIdForAsyncCompositable(
    const wr::PipelineId& aPipelineId, const CompositableHandle& aHandle) {
||||||| merged common ancestors
void
WebRenderBridgeChild::AddPipelineIdForAsyncCompositable(const wr::PipelineId& aPipelineId,
                                                        const CompositableHandle& aHandle)
{
=======
void WebRenderBridgeChild::AddPipelineIdForAsyncCompositable(
    const wr::PipelineId& aPipelineId, const CompositableHandle& aHandle,
    wr::RenderRoot aRenderRoot) {
>>>>>>> upstream-releases
  AddWebRenderParentCommand(
<<<<<<< HEAD
      OpAddPipelineIdForCompositable(aPipelineId, aHandle, /* isAsync */ true));
||||||| merged common ancestors
    OpAddPipelineIdForCompositable(aPipelineId, aHandle, /* isAsync */ true));
=======
      OpAddPipelineIdForCompositable(aPipelineId, aHandle, /* isAsync */ true),
      aRenderRoot);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void WebRenderBridgeChild::AddPipelineIdForCompositable(
    const wr::PipelineId& aPipelineId, const CompositableHandle& aHandle) {
  AddWebRenderParentCommand(OpAddPipelineIdForCompositable(
      aPipelineId, aHandle, /* isAsync */ false));
||||||| merged common ancestors
void
WebRenderBridgeChild::AddPipelineIdForCompositable(const wr::PipelineId& aPipelineId,
                                                   const CompositableHandle& aHandle)
{
  AddWebRenderParentCommand(
    OpAddPipelineIdForCompositable(aPipelineId, aHandle, /* isAsync */ false));
=======
void WebRenderBridgeChild::AddPipelineIdForCompositable(
    const wr::PipelineId& aPipelineId, const CompositableHandle& aHandle,
    wr::RenderRoot aRenderRoot) {
  AddWebRenderParentCommand(
      OpAddPipelineIdForCompositable(aPipelineId, aHandle, /* isAsync */ false),
      aRenderRoot);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void WebRenderBridgeChild::RemovePipelineIdForCompositable(
    const wr::PipelineId& aPipelineId) {
  AddWebRenderParentCommand(OpRemovePipelineIdForCompositable(aPipelineId));
||||||| merged common ancestors
void
WebRenderBridgeChild::RemovePipelineIdForCompositable(const wr::PipelineId& aPipelineId)
{
  AddWebRenderParentCommand(
    OpRemovePipelineIdForCompositable(aPipelineId));
=======
void WebRenderBridgeChild::RemovePipelineIdForCompositable(
    const wr::PipelineId& aPipelineId, wr::RenderRoot aRenderRoot) {
  AddWebRenderParentCommand(OpRemovePipelineIdForCompositable(aPipelineId),
                            aRenderRoot);
>>>>>>> upstream-releases
}

wr::ExternalImageId WebRenderBridgeChild::GetNextExternalImageId() {
  wr::MaybeExternalImageId id =
      GetCompositorBridgeChild()->GetNextExternalImageId();
  MOZ_RELEASE_ASSERT(id.isSome());
  return id.value();
}

<<<<<<< HEAD
void WebRenderBridgeChild::ReleaseTextureOfImage(const wr::ImageKey& aKey) {
  AddWebRenderParentCommand(OpReleaseTextureOfImage(aKey));
||||||| merged common ancestors
void
WebRenderBridgeChild::DeallocExternalImageId(const wr::ExternalImageId& aImageId)
{
  AddWebRenderParentCommand(
    OpRemoveExternalImageId(aImageId));
}

void
WebRenderBridgeChild::ReleaseTextureOfImage(const wr::ImageKey& aKey)
{
  AddWebRenderParentCommand(
    OpReleaseTextureOfImage(aKey));
=======
void WebRenderBridgeChild::ReleaseTextureOfImage(const wr::ImageKey& aKey,
                                                 wr::RenderRoot aRenderRoot) {
  AddWebRenderParentCommand(OpReleaseTextureOfImage(aKey), aRenderRoot);
>>>>>>> upstream-releases
}

struct FontFileDataSink {
  wr::FontKey* mFontKey;
  WebRenderBridgeChild* mWrBridge;
  wr::IpcResourceUpdateQueue* mResources;
};

static void WriteFontFileData(const uint8_t* aData, uint32_t aLength,
                              uint32_t aIndex, void* aBaton) {
  FontFileDataSink* sink = static_cast<FontFileDataSink*>(aBaton);

  *sink->mFontKey = sink->mWrBridge->GetNextFontKey();

  sink->mResources->AddRawFont(
      *sink->mFontKey, Range<uint8_t>(const_cast<uint8_t*>(aData), aLength),
      aIndex);
}

static void WriteFontDescriptor(const uint8_t* aData, uint32_t aLength,
                                uint32_t aIndex, void* aBaton) {
  FontFileDataSink* sink = static_cast<FontFileDataSink*>(aBaton);

  *sink->mFontKey = sink->mWrBridge->GetNextFontKey();

  sink->mResources->AddFontDescriptor(
      *sink->mFontKey, Range<uint8_t>(const_cast<uint8_t*>(aData), aLength),
      aIndex);
}

void WebRenderBridgeChild::PushGlyphs(
    wr::DisplayListBuilder& aBuilder, Range<const wr::GlyphInstance> aGlyphs,
    gfx::ScaledFont* aFont, const wr::ColorF& aColor,
    const StackingContextHelper& aSc, const wr::LayoutRect& aBounds,
    const wr::LayoutRect& aClip, bool aBackfaceVisible,
    const wr::GlyphOptions* aGlyphOptions) {
  MOZ_ASSERT(aFont);

  Maybe<wr::WrFontInstanceKey> key =
      GetFontKeyForScaledFont(aFont, aBuilder.GetRenderRoot());
  MOZ_ASSERT(key.isSome());

<<<<<<< HEAD
  aBuilder.PushText(aBounds, aClip, aBackfaceVisible, aColor, key, aGlyphs,
                    aGlyphOptions);
||||||| merged common ancestors
  aBuilder.PushText(aBounds,
                    aClip,
                    aBackfaceVisible,
                    aColor,
                    key,
                    aGlyphs,
                    aGlyphOptions);
=======
  if (key.isSome()) {
    aBuilder.PushText(aBounds, aClip, aBackfaceVisible, aColor, key.value(),
                      aGlyphs, aGlyphOptions);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
wr::FontInstanceKey WebRenderBridgeChild::GetFontKeyForScaledFont(
    gfx::ScaledFont* aScaledFont, wr::IpcResourceUpdateQueue* aResources) {
||||||| merged common ancestors
wr::FontInstanceKey
WebRenderBridgeChild::GetFontKeyForScaledFont(gfx::ScaledFont* aScaledFont,
                                              wr::IpcResourceUpdateQueue* aResources)
{
=======
Maybe<wr::FontInstanceKey> WebRenderBridgeChild::GetFontKeyForScaledFont(
    gfx::ScaledFont* aScaledFont, wr::RenderRoot aRenderRoot,
    wr::IpcResourceUpdateQueue* aResources) {
>>>>>>> upstream-releases
  MOZ_ASSERT(!mDestroyed);
  MOZ_ASSERT(aScaledFont);
  MOZ_ASSERT(aScaledFont->CanSerialize());

<<<<<<< HEAD
  wr::FontInstanceKey instanceKey = {wr::IdNamespace{0}, 0};
  if (mFontInstanceKeys.Get(aScaledFont, &instanceKey)) {
    return instanceKey;
||||||| merged common ancestors
  wr::FontInstanceKey instanceKey = { wr::IdNamespace { 0 }, 0 };
  if (mFontInstanceKeys.Get(aScaledFont, &instanceKey)) {
    return instanceKey;
=======
  auto& fontInstanceKeys = mFontInstanceKeys[aRenderRoot];
  wr::FontInstanceKey instanceKey = {wr::IdNamespace{0}, 0};
  if (fontInstanceKeys.Get(aScaledFont, &instanceKey)) {
    return Some(instanceKey);
>>>>>>> upstream-releases
  }

  Maybe<wr::IpcResourceUpdateQueue> resources =
      aResources ? Nothing() : Some(wr::IpcResourceUpdateQueue(this));
  aResources = resources.ptrOr(aResources);

<<<<<<< HEAD
  wr::FontKey fontKey =
      GetFontKeyForUnscaledFont(aScaledFont->GetUnscaledFont(), aResources);
  wr::FontKey nullKey = {wr::IdNamespace{0}, 0};
  if (fontKey == nullKey) {
    return instanceKey;
||||||| merged common ancestors
  wr::FontKey fontKey = GetFontKeyForUnscaledFont(aScaledFont->GetUnscaledFont(), aResources);
  wr::FontKey nullKey = { wr::IdNamespace { 0 }, 0};
  if (fontKey == nullKey) {
    return instanceKey;
=======
  Maybe<wr::FontKey> fontKey = GetFontKeyForUnscaledFont(
      aScaledFont->GetUnscaledFont(), aRenderRoot, aResources);
  if (fontKey.isNothing()) {
    return Nothing();
>>>>>>> upstream-releases
  }

  instanceKey = GetNextFontInstanceKey();

  Maybe<wr::FontInstanceOptions> options;
  Maybe<wr::FontInstancePlatformOptions> platformOptions;
  std::vector<FontVariation> variations;
  aScaledFont->GetWRFontInstanceOptions(&options, &platformOptions,
                                        &variations);

<<<<<<< HEAD
  aResources->AddFontInstance(
      instanceKey, fontKey, aScaledFont->GetSize(), options.ptrOr(nullptr),
      platformOptions.ptrOr(nullptr),
      Range<const FontVariation>(variations.data(), variations.size()));
||||||| merged common ancestors
  aResources->AddFontInstance(instanceKey, fontKey, aScaledFont->GetSize(),
                              options.ptrOr(nullptr), platformOptions.ptrOr(nullptr),
                              Range<const FontVariation>(variations.data(), variations.size()));
=======
  aResources->AddFontInstance(
      instanceKey, fontKey.value(), aScaledFont->GetSize(),
      options.ptrOr(nullptr), platformOptions.ptrOr(nullptr),
      Range<const FontVariation>(variations.data(), variations.size()));
>>>>>>> upstream-releases
  if (resources.isSome()) {
    UpdateResources(resources.ref(), aRenderRoot);
  }

<<<<<<< HEAD
  mFontInstanceKeys.Put(aScaledFont, instanceKey);

  return instanceKey;
||||||| merged common ancestors
  mFontInstanceKeys.Put(aScaledFont, instanceKey);

  return instanceKey;

=======
  fontInstanceKeys.Put(aScaledFont, instanceKey);

  return Some(instanceKey);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
wr::FontKey WebRenderBridgeChild::GetFontKeyForUnscaledFont(
    gfx::UnscaledFont* aUnscaled, wr::IpcResourceUpdateQueue* aResources) {
||||||| merged common ancestors
wr::FontKey
WebRenderBridgeChild::GetFontKeyForUnscaledFont(gfx::UnscaledFont* aUnscaled,
                                                wr::IpcResourceUpdateQueue* aResources)
{
=======
Maybe<wr::FontKey> WebRenderBridgeChild::GetFontKeyForUnscaledFont(
    gfx::UnscaledFont* aUnscaled, wr::RenderRoot aRenderRoot,
    wr::IpcResourceUpdateQueue* aResources) {
>>>>>>> upstream-releases
  MOZ_ASSERT(!mDestroyed);

<<<<<<< HEAD
  wr::FontKey fontKey = {wr::IdNamespace{0}, 0};
  if (!mFontKeys.Get(aUnscaled, &fontKey)) {
||||||| merged common ancestors
  wr::FontKey fontKey = { wr::IdNamespace { 0 }, 0};
  if (!mFontKeys.Get(aUnscaled, &fontKey)) {
=======
  auto& fontKeys = mFontKeys[aRenderRoot];
  wr::FontKey fontKey = {wr::IdNamespace{0}, 0};
  if (!fontKeys.Get(aUnscaled, &fontKey)) {
>>>>>>> upstream-releases
    Maybe<wr::IpcResourceUpdateQueue> resources =
        aResources ? Nothing() : Some(wr::IpcResourceUpdateQueue(this));

    FontFileDataSink sink = {&fontKey, this, resources.ptrOr(aResources)};
    // First try to retrieve a descriptor for the font, as this is much cheaper
    // to send over IPC than the full raw font data. If this is not possible,
    // then and only then fall back to getting the raw font file data. If that
    // fails, then the only thing left to do is signal failure by returning a
    // null font key.
    if (!aUnscaled->GetWRFontDescriptor(WriteFontDescriptor, &sink) &&
        !aUnscaled->GetFontFileData(WriteFontFileData, &sink)) {
      return Nothing();
    }

    if (resources.isSome()) {
      UpdateResources(resources.ref(), aRenderRoot);
    }

    fontKeys.Put(aUnscaled, fontKey);
  }

  return Some(fontKey);
}

<<<<<<< HEAD
void WebRenderBridgeChild::RemoveExpiredFontKeys(
    wr::IpcResourceUpdateQueue& aResourceUpdates) {
||||||| merged common ancestors
void
WebRenderBridgeChild::RemoveExpiredFontKeys(wr::IpcResourceUpdateQueue& aResourceUpdates)
{
=======
void WebRenderBridgeChild::RemoveExpiredFontKeys(
    wr::IpcResourceUpdateQueue& aResourceUpdates) {
  auto& fontInstanceKeys = mFontInstanceKeys[aResourceUpdates.GetRenderRoot()];
  auto& fontKeys = mFontKeys[aResourceUpdates.GetRenderRoot()];
  auto& fontInstanceKeysDeleted =
      mFontInstanceKeysDeleted[aResourceUpdates.GetRenderRoot()];
  auto& fontKeysDeleted = mFontKeysDeleted[aResourceUpdates.GetRenderRoot()];

>>>>>>> upstream-releases
  uint32_t counter = gfx::ScaledFont::DeletionCounter();
  if (fontInstanceKeysDeleted != counter) {
    fontInstanceKeysDeleted = counter;
    for (auto iter = fontInstanceKeys.Iter(); !iter.Done(); iter.Next()) {
      if (!iter.Key()) {
        aResourceUpdates.DeleteFontInstance(iter.Data());
        iter.Remove();
      }
    }
  }
  counter = gfx::UnscaledFont::DeletionCounter();
  if (fontKeysDeleted != counter) {
    fontKeysDeleted = counter;
    for (auto iter = fontKeys.Iter(); !iter.Done(); iter.Next()) {
      if (!iter.Key()) {
        aResourceUpdates.DeleteFont(iter.Data());
        iter.Remove();
      }
    }
  }
}

CompositorBridgeChild* WebRenderBridgeChild::GetCompositorBridgeChild() {
  return static_cast<CompositorBridgeChild*>(Manager());
}

TextureForwarder* WebRenderBridgeChild::GetTextureForwarder() {
  return static_cast<TextureForwarder*>(GetCompositorBridgeChild());
}

LayersIPCActor* WebRenderBridgeChild::GetLayersIPCActor() {
  return static_cast<LayersIPCActor*>(GetCompositorBridgeChild());
}

void WebRenderBridgeChild::SyncWithCompositor() {
  if (!IPCOpen()) {
    return;
  }
  SendSyncWithCompositor();
}

void WebRenderBridgeChild::Connect(CompositableClient* aCompositable,
                                   ImageContainer* aImageContainer) {
  MOZ_ASSERT(!mDestroyed);
  MOZ_ASSERT(aCompositable);

  static uint64_t sNextID = 1;
  uint64_t id = sNextID++;

  mCompositables.Put(id, aCompositable);

  CompositableHandle handle(id);
  aCompositable->InitIPDL(handle);
  SendNewCompositable(handle, aCompositable->GetTextureInfo());
}

void WebRenderBridgeChild::UseTiledLayerBuffer(
    CompositableClient* aCompositable,
    const SurfaceDescriptorTiles& aTiledDescriptor) {}

void WebRenderBridgeChild::UpdateTextureRegion(
    CompositableClient* aCompositable,
    const ThebesBufferData& aThebesBufferData,
    const nsIntRegion& aUpdatedRegion) {}

bool WebRenderBridgeChild::AddOpDestroy(const OpDestroy& aOp) {
  if (!mIsInTransaction) {
    return false;
  }

  mDestroyedActors.AppendElement(aOp);
  return true;
}

void WebRenderBridgeChild::ReleaseCompositable(
    const CompositableHandle& aHandle) {
  if (!IPCOpen()) {
    // This can happen if the IPC connection was torn down, because, e.g.
    // the GPU process died.
    return;
  }
  if (!DestroyInTransaction(aHandle)) {
    SendReleaseCompositable(aHandle);
  }
  mCompositables.Remove(aHandle.Value());
}

bool WebRenderBridgeChild::DestroyInTransaction(PTextureChild* aTexture) {
  return AddOpDestroy(OpDestroy(aTexture));
}

bool WebRenderBridgeChild::DestroyInTransaction(
    const CompositableHandle& aHandle) {
  return AddOpDestroy(OpDestroy(aHandle));
}

<<<<<<< HEAD
void WebRenderBridgeChild::RemoveTextureFromCompositable(
    CompositableClient* aCompositable, TextureClient* aTexture) {
||||||| merged common ancestors
void
WebRenderBridgeChild::RemoveTextureFromCompositable(CompositableClient* aCompositable,
                                                    TextureClient* aTexture)
{
=======
void WebRenderBridgeChild::RemoveTextureFromCompositable(
    CompositableClient* aCompositable, TextureClient* aTexture,
    const Maybe<wr::RenderRoot>& aRenderRoot) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aCompositable);
  MOZ_ASSERT(aTexture);
  MOZ_ASSERT(aTexture->GetIPDLActor());
  MOZ_RELEASE_ASSERT(aTexture->GetIPDLActor()->GetIPCChannel() ==
                     GetIPCChannel());
  if (!aCompositable->IsConnected() || !aTexture->GetIPDLActor()) {
    // We don't have an actor anymore, don't try to use it!
    return;
  }

<<<<<<< HEAD
  AddWebRenderParentCommand(CompositableOperation(
      aCompositable->GetIPCHandle(),
      OpRemoveTexture(nullptr, aTexture->GetIPDLActor())));
||||||| merged common ancestors
  AddWebRenderParentCommand(
    CompositableOperation(
      aCompositable->GetIPCHandle(),
      OpRemoveTexture(nullptr, aTexture->GetIPDLActor())));
=======
  AddWebRenderParentCommand(
      CompositableOperation(aCompositable->GetIPCHandle(),
                            OpRemoveTexture(nullptr, aTexture->GetIPDLActor())),
      *aRenderRoot);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void WebRenderBridgeChild::UseTextures(
    CompositableClient* aCompositable,
    const nsTArray<TimedTextureClient>& aTextures) {
||||||| merged common ancestors
void
WebRenderBridgeChild::UseTextures(CompositableClient* aCompositable,
                                  const nsTArray<TimedTextureClient>& aTextures)
{
=======
void WebRenderBridgeChild::UseTextures(
    CompositableClient* aCompositable,
    const nsTArray<TimedTextureClient>& aTextures,
    const Maybe<wr::RenderRoot>& aRenderRoot) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aCompositable);

  if (!aCompositable->IsConnected()) {
    return;
  }

  AutoTArray<TimedTexture, 4> textures;

  for (auto& t : aTextures) {
    MOZ_ASSERT(t.mTextureClient);
    MOZ_ASSERT(t.mTextureClient->GetIPDLActor());
    MOZ_RELEASE_ASSERT(t.mTextureClient->GetIPDLActor()->GetIPCChannel() ==
                       GetIPCChannel());
    bool readLocked = t.mTextureClient->OnForwardedToHost();

    textures.AppendElement(
        TimedTexture(nullptr, t.mTextureClient->GetIPDLActor(), t.mTimeStamp,
                     t.mPictureRect, t.mFrameID, t.mProducerID, readLocked));
    GetCompositorBridgeChild()->HoldUntilCompositableRefReleasedIfNecessary(
        t.mTextureClient);
  }
  AddWebRenderParentCommand(CompositableOperation(aCompositable->GetIPCHandle(),
<<<<<<< HEAD
                                                  OpUseTexture(textures)));
||||||| merged common ancestors
                                            OpUseTexture(textures)));
=======
                                                  OpUseTexture(textures)),
                            *aRenderRoot);
>>>>>>> upstream-releases
}

void WebRenderBridgeChild::UseComponentAlphaTextures(
    CompositableClient* aCompositable, TextureClient* aClientOnBlack,
    TextureClient* aClientOnWhite) {}

void WebRenderBridgeChild::UpdateFwdTransactionId() {
  GetCompositorBridgeChild()->UpdateFwdTransactionId();
}

uint64_t WebRenderBridgeChild::GetFwdTransactionId() {
  return GetCompositorBridgeChild()->GetFwdTransactionId();
}

bool WebRenderBridgeChild::InForwarderThread() { return NS_IsMainThread(); }

mozilla::ipc::IPCResult WebRenderBridgeChild::RecvWrUpdated(
    const wr::IdNamespace& aNewIdNamespace,
    const TextureFactoryIdentifier& textureFactoryIdentifier) {
  if (mManager) {
    mManager->WrUpdated();
  }
  IdentifyTextureHost(textureFactoryIdentifier);
  // Update mIdNamespace to identify obsolete keys and messages by
  // WebRenderBridgeParent. Since usage of invalid keys could cause crash in
  // webrender.
  mIdNamespace = aNewIdNamespace;
<<<<<<< HEAD
  // Just clear FontInstaceKeys/FontKeys, they are removed during WebRenderAPI
  // destruction.
  mFontInstanceKeys.Clear();
  mFontKeys.Clear();
||||||| merged common ancestors
  // Just clear FontInstaceKeys/FontKeys, they are removed during WebRenderAPI destruction.
  mFontInstanceKeys.Clear();
  mFontKeys.Clear();
=======
  // Just clear FontInstaceKeys/FontKeys, they are removed during WebRenderAPI
  // destruction.
  for (auto renderRoot : wr::kRenderRoots) {
    mFontInstanceKeys[renderRoot].Clear();
    mFontKeys[renderRoot].Clear();
  }
>>>>>>> upstream-releases
  return IPC_OK();
}

mozilla::ipc::IPCResult WebRenderBridgeChild::RecvWrReleasedImages(
    nsTArray<wr::ExternalImageKeyPair>&& aPairs) {
  if (mManager) {
    mManager->WrReleasedImages(aPairs);
  }
  return IPC_OK();
}

void WebRenderBridgeChild::BeginClearCachedResources() {
  mIsInClearCachedResources = true;
  // Clear display list and animtaions at parent side before clearing cached
  // resources on client side. It prevents to clear resources before clearing
  // display list at parent side.
  SendClearCachedResources();
}

void WebRenderBridgeChild::EndClearCachedResources() {
  if (!IPCOpen()) {
    mIsInClearCachedResources = false;
    return;
  }
  ProcessWebRenderParentCommands();
  mIsInClearCachedResources = false;
}

void WebRenderBridgeChild::SetWebRenderLayerManager(
    WebRenderLayerManager* aManager) {
  MOZ_ASSERT(aManager && !mManager);
  mManager = aManager;

  nsCOMPtr<nsIEventTarget> eventTarget = nullptr;
  if (dom::TabGroup* tabGroup = mManager->GetTabGroup()) {
    eventTarget = tabGroup->EventTargetFor(TaskCategory::Other);
  }
  MOZ_ASSERT(eventTarget || !XRE_IsContentProcess());
  mActiveResourceTracker = MakeUnique<ActiveResourceTracker>(
      1000, "CompositableForwarder", eventTarget);
}

ipc::IShmemAllocator* WebRenderBridgeChild::GetShmemAllocator() {
  return static_cast<CompositorBridgeChild*>(Manager());
}

RefPtr<KnowsCompositor> WebRenderBridgeChild::GetForMedia() {
  MOZ_ASSERT(NS_IsMainThread());
<<<<<<< HEAD
  return MakeAndAddRef<KnowsCompositorMediaProxy>(
      GetTextureFactoryIdentifier());
||||||| merged common ancestors
  return MakeAndAddRef<KnowsCompositorMediaProxy>(GetTextureFactoryIdentifier());
=======

  // Ensure devices initialization for video playback. The devices are lazily
  // initialized with WebRender to reduce memory usage.
  gfxPlatform::GetPlatform()->EnsureDevicesInitialized();

  return MakeAndAddRef<KnowsCompositorMediaProxy>(
      GetTextureFactoryIdentifier());
>>>>>>> upstream-releases
}

bool WebRenderBridgeChild::AllocResourceShmem(size_t aSize,
                                              RefCountedShmem& aShm) {
  // We keep a single shmem around to reuse later if it is reference count has
  // dropped back to 1 (the reference held by the WebRenderBridgeChild).

  // If the cached shmem exists, has the correct size and isn't held by anything
  // other than us, recycle it.
  bool alreadyAllocated = RefCountedShm::IsValid(mResourceShm);
  if (alreadyAllocated) {
    if (RefCountedShm::GetSize(mResourceShm) == aSize &&
        RefCountedShm::GetReferenceCount(mResourceShm) <= 1) {
      MOZ_ASSERT(RefCountedShm::GetReferenceCount(mResourceShm) == 1);
      aShm = mResourceShm;
      return true;
    }
  }

  // If there was no cached shmem or we couldn't recycle it, alloc a new one.
  if (!RefCountedShm::Alloc(this, aSize, aShm)) {
    return false;
  }

  // Now that we have a valid shmem, put it in the cache if we don't have one
  // yet.
  if (!alreadyAllocated) {
    mResourceShm = aShm;
    RefCountedShm::AddRef(aShm);
  }

  return true;
}

void WebRenderBridgeChild::DeallocResourceShmem(RefCountedShmem& aShm) {
  if (!RefCountedShm::IsValid(aShm)) {
    return;
  }
  MOZ_ASSERT(RefCountedShm::GetReferenceCount(aShm) == 0);

  RefCountedShm::Dealloc(this, aShm);
}

void WebRenderBridgeChild::Capture() { this->SendCapture(); }

}  // namespace layers
}  // namespace mozilla
