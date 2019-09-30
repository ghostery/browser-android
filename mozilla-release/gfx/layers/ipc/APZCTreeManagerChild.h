/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_layers_APZCTreeManagerChild_h
#define mozilla_layers_APZCTreeManagerChild_h

#include "mozilla/layers/APZInputBridge.h"
#include "mozilla/layers/IAPZCTreeManager.h"
#include "mozilla/layers/PAPZCTreeManagerChild.h"

namespace mozilla {
namespace layers {

class APZInputBridgeChild;
class RemoteCompositorSession;

<<<<<<< HEAD
class APZCTreeManagerChild : public IAPZCTreeManager,
                             public PAPZCTreeManagerChild {
 public:
||||||| merged common ancestors
class APZCTreeManagerChild
  : public IAPZCTreeManager
  , public PAPZCTreeManagerChild
{
public:
=======
class APZCTreeManagerChild : public IAPZCTreeManager,
                             public PAPZCTreeManagerChild {
  friend class PAPZCTreeManagerChild;

 public:
>>>>>>> upstream-releases
  APZCTreeManagerChild();

  void SetCompositorSession(RemoteCompositorSession* aSession);
  void SetInputBridge(APZInputBridgeChild* aInputBridge);
  void Destroy();

<<<<<<< HEAD
  void SetKeyboardMap(const KeyboardMap& aKeyboardMap) override;
||||||| merged common ancestors
  void
  SetKeyboardMap(const KeyboardMap& aKeyboardMap) override;
=======
  void SetKeyboardMap(const KeyboardMap& aKeyboardMap) override;

  void ZoomToRect(const SLGuidAndRenderRoot& aGuid, const CSSRect& aRect,
                  const uint32_t aFlags = DEFAULT_BEHAVIOR) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void ZoomToRect(const ScrollableLayerGuid& aGuid, const CSSRect& aRect,
                  const uint32_t aFlags = DEFAULT_BEHAVIOR) override;
||||||| merged common ancestors
  void
  ZoomToRect(
          const ScrollableLayerGuid& aGuid,
          const CSSRect& aRect,
          const uint32_t aFlags = DEFAULT_BEHAVIOR) override;
=======
  void ContentReceivedInputBlock(uint64_t aInputBlockId,
                                 bool aPreventDefault) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void ContentReceivedInputBlock(uint64_t aInputBlockId,
                                 bool aPreventDefault) override;
||||||| merged common ancestors
  void
  ContentReceivedInputBlock(
          uint64_t aInputBlockId,
          bool aPreventDefault) override;
=======
  void SetTargetAPZC(uint64_t aInputBlockId,
                     const nsTArray<SLGuidAndRenderRoot>& aTargets) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetTargetAPZC(uint64_t aInputBlockId,
                     const nsTArray<ScrollableLayerGuid>& aTargets) override;
||||||| merged common ancestors
  void
  SetTargetAPZC(
          uint64_t aInputBlockId,
          const nsTArray<ScrollableLayerGuid>& aTargets) override;
=======
  void UpdateZoomConstraints(
      const SLGuidAndRenderRoot& aGuid,
      const Maybe<ZoomConstraints>& aConstraints) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void UpdateZoomConstraints(
      const ScrollableLayerGuid& aGuid,
      const Maybe<ZoomConstraints>& aConstraints) override;
||||||| merged common ancestors
  void
  UpdateZoomConstraints(
          const ScrollableLayerGuid& aGuid,
          const Maybe<ZoomConstraints>& aConstraints) override;
=======
  void SetDPI(float aDpiValue) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetDPI(float aDpiValue) override;
||||||| merged common ancestors
  void
  SetDPI(float aDpiValue) override;
=======
  void SetAllowedTouchBehavior(
      uint64_t aInputBlockId,
      const nsTArray<TouchBehaviorFlags>& aValues) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetAllowedTouchBehavior(
      uint64_t aInputBlockId,
      const nsTArray<TouchBehaviorFlags>& aValues) override;
||||||| merged common ancestors
  void
  SetAllowedTouchBehavior(
          uint64_t aInputBlockId,
          const nsTArray<TouchBehaviorFlags>& aValues) override;
=======
  void StartScrollbarDrag(const SLGuidAndRenderRoot& aGuid,
                          const AsyncDragMetrics& aDragMetrics) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void StartScrollbarDrag(const ScrollableLayerGuid& aGuid,
                          const AsyncDragMetrics& aDragMetrics) override;
||||||| merged common ancestors
  void
  StartScrollbarDrag(
          const ScrollableLayerGuid& aGuid,
          const AsyncDragMetrics& aDragMetrics) override;
=======
  bool StartAutoscroll(const SLGuidAndRenderRoot& aGuid,
                       const ScreenPoint& aAnchorLocation) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool StartAutoscroll(const ScrollableLayerGuid& aGuid,
                       const ScreenPoint& aAnchorLocation) override;
||||||| merged common ancestors
  bool
  StartAutoscroll(
          const ScrollableLayerGuid& aGuid,
          const ScreenPoint& aAnchorLocation) override;
=======
  void StopAutoscroll(const SLGuidAndRenderRoot& aGuid) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void StopAutoscroll(const ScrollableLayerGuid& aGuid) override;
||||||| merged common ancestors
  void
  StopAutoscroll(const ScrollableLayerGuid& aGuid) override;
=======
  void SetLongTapEnabled(bool aTapGestureEnabled) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetLongTapEnabled(bool aTapGestureEnabled) override;
||||||| merged common ancestors
  void
  SetLongTapEnabled(bool aTapGestureEnabled) override;
=======
  APZInputBridge* InputBridge() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  APZInputBridge* InputBridge() override;
||||||| merged common ancestors
  APZInputBridge*
  InputBridge() override;
=======
  void AddIPDLReference();
  void ReleaseIPDLReference();
  void ActorDestroy(ActorDestroyReason aWhy) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
||||||| merged common ancestors
protected:
=======
 protected:
  MOZ_CAN_RUN_SCRIPT_BOUNDARY
>>>>>>> upstream-releases
  mozilla::ipc::IPCResult RecvHandleTap(const TapType& aType,
                                        const LayoutDevicePoint& aPoint,
                                        const Modifiers& aModifiers,
                                        const ScrollableLayerGuid& aGuid,
                                        const uint64_t& aInputBlockId);

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvNotifyPinchGesture(
      const PinchGestureType& aType, const ScrollableLayerGuid& aGuid,
      const LayoutDeviceCoord& aSpanChange,
      const Modifiers& aModifiers) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult RecvNotifyPinchGesture(const PinchGestureType& aType,
                                                 const ScrollableLayerGuid& aGuid,
                                                 const LayoutDeviceCoord& aSpanChange,
                                                 const Modifiers& aModifiers) override;
=======
  mozilla::ipc::IPCResult RecvNotifyPinchGesture(
      const PinchGestureType& aType, const ScrollableLayerGuid& aGuid,
      const LayoutDeviceCoord& aSpanChange, const Modifiers& aModifiers);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvCancelAutoscroll(
      const ScrollableLayerGuid::ViewID& aScrollId) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult RecvCancelAutoscroll(const FrameMetrics::ViewID& aScrollId) override;
=======
  mozilla::ipc::IPCResult RecvCancelAutoscroll(
      const ScrollableLayerGuid::ViewID& aScrollId);
>>>>>>> upstream-releases

  virtual ~APZCTreeManagerChild();

 private:
  MOZ_NON_OWNING_REF RemoteCompositorSession* mCompositorSession;
  RefPtr<APZInputBridgeChild> mInputBridge;
  bool mIPCOpen;
};

}  // namespace layers
}  // namespace mozilla

#endif  // mozilla_layers_APZCTreeManagerChild_h
