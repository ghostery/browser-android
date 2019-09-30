/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_layers_APZCTreeManagerParent_h
#define mozilla_layers_APZCTreeManagerParent_h

#include "mozilla/layers/PAPZCTreeManagerParent.h"

namespace mozilla {
namespace layers {

class APZCTreeManager;
class APZUpdater;

<<<<<<< HEAD
class APZCTreeManagerParent : public PAPZCTreeManagerParent {
 public:
  APZCTreeManagerParent(LayersId aLayersId,
||||||| merged common ancestors
class APZCTreeManagerParent
    : public PAPZCTreeManagerParent
{
public:

  APZCTreeManagerParent(LayersId aLayersId,
=======
class APZCTreeManagerParent : public PAPZCTreeManagerParent {
 public:
  APZCTreeManagerParent(WRRootId aWrRootId,
>>>>>>> upstream-releases
                        RefPtr<APZCTreeManager> aAPZCTreeManager,
                        RefPtr<APZUpdater> mAPZUpdater);
  virtual ~APZCTreeManagerParent();

  LayersId GetLayersId() const { return mWrRootId.mLayersId; }

  /**
   * Called when the layer tree that this protocol is connected to
   * is adopted by another compositor, and we need to switch APZCTreeManagers.
   */
  void ChildAdopted(RefPtr<APZCTreeManager> aAPZCTreeManager,
                    RefPtr<APZUpdater> aAPZUpdater);

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvSetKeyboardMap(
      const KeyboardMap& aKeyboardMap) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvSetKeyboardMap(const KeyboardMap& aKeyboardMap) override;
=======
  mozilla::ipc::IPCResult RecvSetKeyboardMap(const KeyboardMap& aKeyboardMap);

  mozilla::ipc::IPCResult RecvZoomToRect(const SLGuidAndRenderRoot& aGuid,
                                         const CSSRect& aRect,
                                         const uint32_t& aFlags);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvZoomToRect(const ScrollableLayerGuid& aGuid,
                                         const CSSRect& aRect,
                                         const uint32_t& aFlags) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvZoomToRect(
          const ScrollableLayerGuid& aGuid,
          const CSSRect& aRect,
          const uint32_t& aFlags) override;
=======
  mozilla::ipc::IPCResult RecvContentReceivedInputBlock(
      const uint64_t& aInputBlockId, const bool& aPreventDefault);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvContentReceivedInputBlock(
      const uint64_t& aInputBlockId, const bool& aPreventDefault) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvContentReceivedInputBlock(
          const uint64_t& aInputBlockId,
          const bool& aPreventDefault) override;
=======
  mozilla::ipc::IPCResult RecvSetTargetAPZC(
      const uint64_t& aInputBlockId, nsTArray<SLGuidAndRenderRoot>&& aTargets);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvSetTargetAPZC(
      const uint64_t& aInputBlockId,
      nsTArray<ScrollableLayerGuid>&& aTargets) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvSetTargetAPZC(
          const uint64_t& aInputBlockId,
          nsTArray<ScrollableLayerGuid>&& aTargets) override;
=======
  mozilla::ipc::IPCResult RecvUpdateZoomConstraints(
      const SLGuidAndRenderRoot& aGuid,
      const MaybeZoomConstraints& aConstraints);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvUpdateZoomConstraints(
      const ScrollableLayerGuid& aGuid,
      const MaybeZoomConstraints& aConstraints) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvUpdateZoomConstraints(
          const ScrollableLayerGuid& aGuid,
          const MaybeZoomConstraints& aConstraints) override;
=======
  mozilla::ipc::IPCResult RecvSetDPI(const float& aDpiValue);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvSetDPI(const float& aDpiValue) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvSetDPI(const float& aDpiValue) override;
=======
  mozilla::ipc::IPCResult RecvSetAllowedTouchBehavior(
      const uint64_t& aInputBlockId, nsTArray<TouchBehaviorFlags>&& aValues);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvSetAllowedTouchBehavior(
      const uint64_t& aInputBlockId,
      nsTArray<TouchBehaviorFlags>&& aValues) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvSetAllowedTouchBehavior(
          const uint64_t& aInputBlockId,
          nsTArray<TouchBehaviorFlags>&& aValues) override;
=======
  mozilla::ipc::IPCResult RecvStartScrollbarDrag(
      const SLGuidAndRenderRoot& aGuid, const AsyncDragMetrics& aDragMetrics);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvStartScrollbarDrag(
      const ScrollableLayerGuid& aGuid,
      const AsyncDragMetrics& aDragMetrics) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvStartScrollbarDrag(
          const ScrollableLayerGuid& aGuid,
          const AsyncDragMetrics& aDragMetrics) override;
=======
  mozilla::ipc::IPCResult RecvStartAutoscroll(
      const SLGuidAndRenderRoot& aGuid, const ScreenPoint& aAnchorLocation);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvStartAutoscroll(
      const ScrollableLayerGuid& aGuid,
      const ScreenPoint& aAnchorLocation) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvStartAutoscroll(
          const ScrollableLayerGuid& aGuid,
          const ScreenPoint& aAnchorLocation) override;
=======
  mozilla::ipc::IPCResult RecvStopAutoscroll(const SLGuidAndRenderRoot& aGuid);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvStopAutoscroll(
      const ScrollableLayerGuid& aGuid) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvStopAutoscroll(const ScrollableLayerGuid& aGuid) override;
=======
  mozilla::ipc::IPCResult RecvSetLongTapEnabled(const bool& aTapGestureEnabled);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvSetLongTapEnabled(
      const bool& aTapGestureEnabled) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvSetLongTapEnabled(const bool& aTapGestureEnabled) override;
=======
  void ActorDestroy(ActorDestroyReason aWhy) override {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  void ActorDestroy(ActorDestroyReason aWhy) override {}
||||||| merged common ancestors
  void
  ActorDestroy(ActorDestroyReason aWhy) override { }
=======
 private:
  bool IsGuidValid(const SLGuidAndRenderRoot& aGuid);
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  LayersId mLayersId;
||||||| merged common ancestors
private:
  LayersId mLayersId;
=======
  WRRootId mWrRootId;
>>>>>>> upstream-releases
  RefPtr<APZCTreeManager> mTreeManager;
  RefPtr<APZUpdater> mUpdater;
};

}  // namespace layers
}  // namespace mozilla

#endif  // mozilla_layers_APZCTreeManagerParent_h
