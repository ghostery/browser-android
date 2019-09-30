/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/layers/APZCTreeManagerChild.h"

<<<<<<< HEAD
#include "InputData.h"                               // for InputData
#include "mozilla/dom/TabParent.h"                   // for TabParent
#include "mozilla/layers/APZCCallbackHelper.h"       // for APZCCallbackHelper
#include "mozilla/layers/APZInputBridgeChild.h"      // for APZInputBridgeChild
#include "mozilla/layers/RemoteCompositorSession.h"  // for RemoteCompositorSession
||||||| merged common ancestors
#include "InputData.h"                  // for InputData
#include "mozilla/dom/TabParent.h"      // for TabParent
#include "mozilla/layers/APZCCallbackHelper.h" // for APZCCallbackHelper
#include "mozilla/layers/APZInputBridgeChild.h" // for APZInputBridgeChild
#include "mozilla/layers/RemoteCompositorSession.h" // for RemoteCompositorSession
=======
#include "InputData.h"                               // for InputData
#include "mozilla/dom/BrowserParent.h"               // for BrowserParent
#include "mozilla/layers/APZCCallbackHelper.h"       // for APZCCallbackHelper
#include "mozilla/layers/APZInputBridgeChild.h"      // for APZInputBridgeChild
#include "mozilla/layers/RemoteCompositorSession.h"  // for RemoteCompositorSession
>>>>>>> upstream-releases

namespace mozilla {
namespace layers {

<<<<<<< HEAD
APZCTreeManagerChild::APZCTreeManagerChild() : mCompositorSession(nullptr) {}
||||||| merged common ancestors
APZCTreeManagerChild::APZCTreeManagerChild()
  : mCompositorSession(nullptr)
{
}
=======
APZCTreeManagerChild::APZCTreeManagerChild()
    : mCompositorSession(nullptr), mIPCOpen(false) {}
>>>>>>> upstream-releases

APZCTreeManagerChild::~APZCTreeManagerChild() {}

void APZCTreeManagerChild::SetCompositorSession(
    RemoteCompositorSession* aSession) {
  // Exactly one of mCompositorSession and aSession must be null (i.e. either
  // we're setting mCompositorSession or we're clearing it).
  MOZ_ASSERT(!mCompositorSession ^ !aSession);
  mCompositorSession = aSession;
}

void APZCTreeManagerChild::SetInputBridge(APZInputBridgeChild* aInputBridge) {
  // The input bridge only exists from the UI process to the GPU process.
  MOZ_ASSERT(XRE_IsParentProcess());
  MOZ_ASSERT(!mInputBridge);

  mInputBridge = aInputBridge;
}

void APZCTreeManagerChild::Destroy() {
  MOZ_ASSERT(NS_IsMainThread());
  if (mInputBridge) {
    mInputBridge->Destroy();
    mInputBridge = nullptr;
  }
}

void APZCTreeManagerChild::SetKeyboardMap(const KeyboardMap& aKeyboardMap) {
  SendSetKeyboardMap(aKeyboardMap);
}

<<<<<<< HEAD
void APZCTreeManagerChild::ZoomToRect(const ScrollableLayerGuid& aGuid,
                                      const CSSRect& aRect,
                                      const uint32_t aFlags) {
||||||| merged common ancestors
void
APZCTreeManagerChild::ZoomToRect(
    const ScrollableLayerGuid& aGuid,
    const CSSRect& aRect,
    const uint32_t aFlags)
{
=======
void APZCTreeManagerChild::ZoomToRect(const SLGuidAndRenderRoot& aGuid,
                                      const CSSRect& aRect,
                                      const uint32_t aFlags) {
>>>>>>> upstream-releases
  SendZoomToRect(aGuid, aRect, aFlags);
}

void APZCTreeManagerChild::ContentReceivedInputBlock(uint64_t aInputBlockId,
                                                     bool aPreventDefault) {
  SendContentReceivedInputBlock(aInputBlockId, aPreventDefault);
}

<<<<<<< HEAD
void APZCTreeManagerChild::SetTargetAPZC(
    uint64_t aInputBlockId, const nsTArray<ScrollableLayerGuid>& aTargets) {
||||||| merged common ancestors
void
APZCTreeManagerChild::SetTargetAPZC(
    uint64_t aInputBlockId,
    const nsTArray<ScrollableLayerGuid>& aTargets)
{
=======
void APZCTreeManagerChild::SetTargetAPZC(
    uint64_t aInputBlockId, const nsTArray<SLGuidAndRenderRoot>& aTargets) {
>>>>>>> upstream-releases
  SendSetTargetAPZC(aInputBlockId, aTargets);
}

<<<<<<< HEAD
void APZCTreeManagerChild::UpdateZoomConstraints(
    const ScrollableLayerGuid& aGuid,
    const Maybe<ZoomConstraints>& aConstraints) {
  SendUpdateZoomConstraints(aGuid, aConstraints);
||||||| merged common ancestors
void
APZCTreeManagerChild::UpdateZoomConstraints(
    const ScrollableLayerGuid& aGuid,
    const Maybe<ZoomConstraints>& aConstraints)
{
  SendUpdateZoomConstraints(aGuid, aConstraints);
=======
void APZCTreeManagerChild::UpdateZoomConstraints(
    const SLGuidAndRenderRoot& aGuid,
    const Maybe<ZoomConstraints>& aConstraints) {
  if (mIPCOpen) {
    SendUpdateZoomConstraints(aGuid, aConstraints);
  }
>>>>>>> upstream-releases
}

void APZCTreeManagerChild::SetDPI(float aDpiValue) { SendSetDPI(aDpiValue); }

void APZCTreeManagerChild::SetAllowedTouchBehavior(
    uint64_t aInputBlockId, const nsTArray<TouchBehaviorFlags>& aValues) {
  SendSetAllowedTouchBehavior(aInputBlockId, aValues);
}

<<<<<<< HEAD
void APZCTreeManagerChild::StartScrollbarDrag(
    const ScrollableLayerGuid& aGuid, const AsyncDragMetrics& aDragMetrics) {
||||||| merged common ancestors
void
APZCTreeManagerChild::StartScrollbarDrag(
    const ScrollableLayerGuid& aGuid,
    const AsyncDragMetrics& aDragMetrics)
{
=======
void APZCTreeManagerChild::StartScrollbarDrag(
    const SLGuidAndRenderRoot& aGuid, const AsyncDragMetrics& aDragMetrics) {
>>>>>>> upstream-releases
  SendStartScrollbarDrag(aGuid, aDragMetrics);
}

<<<<<<< HEAD
bool APZCTreeManagerChild::StartAutoscroll(const ScrollableLayerGuid& aGuid,
                                           const ScreenPoint& aAnchorLocation) {
||||||| merged common ancestors
bool
APZCTreeManagerChild::StartAutoscroll(
    const ScrollableLayerGuid& aGuid,
    const ScreenPoint& aAnchorLocation)
{
=======
bool APZCTreeManagerChild::StartAutoscroll(const SLGuidAndRenderRoot& aGuid,
                                           const ScreenPoint& aAnchorLocation) {
>>>>>>> upstream-releases
  return SendStartAutoscroll(aGuid, aAnchorLocation);
}

<<<<<<< HEAD
void APZCTreeManagerChild::StopAutoscroll(const ScrollableLayerGuid& aGuid) {
||||||| merged common ancestors
void
APZCTreeManagerChild::StopAutoscroll(const ScrollableLayerGuid& aGuid)
{
=======
void APZCTreeManagerChild::StopAutoscroll(const SLGuidAndRenderRoot& aGuid) {
>>>>>>> upstream-releases
  SendStopAutoscroll(aGuid);
}

void APZCTreeManagerChild::SetLongTapEnabled(bool aTapGestureEnabled) {
  SendSetLongTapEnabled(aTapGestureEnabled);
}

APZInputBridge* APZCTreeManagerChild::InputBridge() {
  MOZ_ASSERT(XRE_IsParentProcess());
  MOZ_ASSERT(mInputBridge);

  return mInputBridge.get();
}

<<<<<<< HEAD
mozilla::ipc::IPCResult APZCTreeManagerChild::RecvHandleTap(
    const TapType& aType, const LayoutDevicePoint& aPoint,
    const Modifiers& aModifiers, const ScrollableLayerGuid& aGuid,
    const uint64_t& aInputBlockId) {
||||||| merged common ancestors
mozilla::ipc::IPCResult
APZCTreeManagerChild::RecvHandleTap(const TapType& aType,
                                    const LayoutDevicePoint& aPoint,
                                    const Modifiers& aModifiers,
                                    const ScrollableLayerGuid& aGuid,
                                    const uint64_t& aInputBlockId)
{
=======
void APZCTreeManagerChild::AddIPDLReference() {
  MOZ_ASSERT(mIPCOpen == false);
  mIPCOpen = true;
  AddRef();
}

void APZCTreeManagerChild::ReleaseIPDLReference() {
  mIPCOpen = false;
  Release();
}

void APZCTreeManagerChild::ActorDestroy(ActorDestroyReason aWhy) {
  mIPCOpen = false;
}

mozilla::ipc::IPCResult APZCTreeManagerChild::RecvHandleTap(
    const TapType& aType, const LayoutDevicePoint& aPoint,
    const Modifiers& aModifiers, const ScrollableLayerGuid& aGuid,
    const uint64_t& aInputBlockId) {
>>>>>>> upstream-releases
  MOZ_ASSERT(XRE_IsParentProcess());
  if (mCompositorSession &&
      mCompositorSession->RootLayerTreeId() == aGuid.mLayersId &&
      mCompositorSession->GetContentController()) {
<<<<<<< HEAD
    mCompositorSession->GetContentController()->HandleTap(
        aType, aPoint, aModifiers, aGuid, aInputBlockId);
||||||| merged common ancestors
    mCompositorSession->GetContentController()->HandleTap(aType, aPoint,
        aModifiers, aGuid, aInputBlockId);
=======
    RefPtr<GeckoContentController> controller =
        mCompositorSession->GetContentController();
    controller->HandleTap(aType, aPoint, aModifiers, aGuid, aInputBlockId);
>>>>>>> upstream-releases
    return IPC_OK();
  }
<<<<<<< HEAD
  dom::TabParent* tab =
      dom::TabParent::GetTabParentFromLayersId(aGuid.mLayersId);
||||||| merged common ancestors
  dom::TabParent* tab = dom::TabParent::GetTabParentFromLayersId(aGuid.mLayersId);
=======
  dom::BrowserParent* tab =
      dom::BrowserParent::GetBrowserParentFromLayersId(aGuid.mLayersId);
>>>>>>> upstream-releases
  if (tab) {
    tab->SendHandleTap(aType, aPoint, aModifiers, aGuid, aInputBlockId);
  }
  return IPC_OK();
}

mozilla::ipc::IPCResult APZCTreeManagerChild::RecvNotifyPinchGesture(
    const PinchGestureType& aType, const ScrollableLayerGuid& aGuid,
    const LayoutDeviceCoord& aSpanChange, const Modifiers& aModifiers) {
  // This will only get sent from the GPU process to the parent process, so
  // this function should never get called in the content process.
  MOZ_ASSERT(XRE_IsParentProcess());
  MOZ_ASSERT(NS_IsMainThread());

  // We want to handle it in this process regardless of what the target guid
  // of the pinch is. This may change in the future.
  if (mCompositorSession && mCompositorSession->GetWidget()) {
    APZCCallbackHelper::NotifyPinchGesture(aType, aSpanChange, aModifiers,
                                           mCompositorSession->GetWidget());
  }
  return IPC_OK();
}

mozilla::ipc::IPCResult APZCTreeManagerChild::RecvCancelAutoscroll(
    const ScrollableLayerGuid::ViewID& aScrollId) {
  // This will only get sent from the GPU process to the parent process, so
  // this function should never get called in the content process.
  MOZ_ASSERT(XRE_IsParentProcess());
  MOZ_ASSERT(NS_IsMainThread());

  APZCCallbackHelper::CancelAutoscroll(aScrollId);
  return IPC_OK();
}

}  // namespace layers
}  // namespace mozilla
