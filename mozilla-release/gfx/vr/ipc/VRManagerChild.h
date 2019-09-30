/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_VR_VRMANAGERCHILD_H
#define MOZILLA_GFX_VR_VRMANAGERCHILD_H

<<<<<<< HEAD
#include "mozilla/dom/WindowBinding.h"  // For FrameRequestCallback
||||||| merged common ancestors
#include "mozilla/dom/WindowBinding.h" // For FrameRequestCallback
=======
#include "mozilla/Attributes.h"
#include "mozilla/dom/WindowBinding.h"  // For FrameRequestCallback
>>>>>>> upstream-releases
#include "mozilla/gfx/PVRManagerChild.h"
#include "mozilla/ipc/SharedMemory.h"  // for SharedMemory, etc
#include "ThreadSafeRefcountingWithMainThreadDestruction.h"
#include "mozilla/layers/ISurfaceAllocator.h"  // for ISurfaceAllocator
#include "mozilla/layers/LayersTypes.h"        // for LayersBackend
#include "mozilla/layers/TextureForwarder.h"

namespace mozilla {
namespace dom {
class Promise;
class GamepadManager;
class Navigator;
class VRDisplay;
class VREventObserver;
<<<<<<< HEAD
class VRMockDisplay;
}  // namespace dom
||||||| merged common ancestors
class VRMockDisplay;
} // namespace dom
=======
}  // namespace dom
>>>>>>> upstream-releases
namespace layers {
class SyncObjectClient;
}
namespace gfx {
class VRLayerChild;
class VRDisplayClient;

<<<<<<< HEAD
class VRManagerChild : public PVRManagerChild {
 public:
||||||| merged common ancestors
class VRManagerChild : public PVRManagerChild
{
public:
=======
class VRManagerChild : public PVRManagerChild {
  friend class PVRManagerChild;

 public:
>>>>>>> upstream-releases
  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(VRManagerChild);

  static VRManagerChild* Get();

  // Indicate that an observer wants to receive VR events.
  void AddListener(dom::VREventObserver* aObserver);
  // Indicate that an observer should no longer receive VR events.
  void RemoveListener(dom::VREventObserver* aObserver);
  void StartActivity();
  void StopActivity();

  bool GetVRDisplays(nsTArray<RefPtr<VRDisplayClient>>& aDisplays);
  bool RefreshVRDisplaysWithCallback(uint64_t aWindowId);
  void AddPromise(const uint32_t& aID, dom::Promise* aPromise);

<<<<<<< HEAD
  void CreateVRServiceTestDisplay(const nsCString& aID, dom::Promise* aPromise);
  void CreateVRServiceTestController(const nsCString& aID,
                                     dom::Promise* aPromise);

||||||| merged common ancestors
  void CreateVRServiceTestDisplay(const nsCString& aID, dom::Promise* aPromise);
  void CreateVRServiceTestController(const nsCString& aID, dom::Promise* aPromise);

=======
>>>>>>> upstream-releases
  static void InitSameProcess();
  static void InitWithGPUProcess(Endpoint<PVRManagerChild>&& aEndpoint);
  static bool InitForContent(Endpoint<PVRManagerChild>&& aEndpoint);
  static bool ReinitForContent(Endpoint<PVRManagerChild>&& aEndpoint);
  static void ShutDown();

  static bool IsCreated();

  PVRLayerChild* CreateVRLayer(uint32_t aDisplayID, nsIEventTarget* aTarget,
                               uint32_t aGroup);

  static void IdentifyTextureHost(
      const layers::TextureFactoryIdentifier& aIdentifier);
  layers::LayersBackend GetBackendType() const;
  layers::SyncObjectClient* GetSyncObject() { return mSyncObject; }

  nsresult ScheduleFrameRequestCallback(
      mozilla::dom::FrameRequestCallback& aCallback, int32_t* aHandle);
  void CancelFrameRequestCallback(int32_t aHandle);
  MOZ_CAN_RUN_SCRIPT
  void RunFrameRequestCallbacks();
  void NotifyPresentationGenerationChanged(uint32_t aDisplayID);

  MOZ_CAN_RUN_SCRIPT
  void UpdateDisplayInfo(nsTArray<VRDisplayInfo>& aDisplayUpdates);
  void FireDOMVRDisplayMountedEvent(uint32_t aDisplayID);
  void FireDOMVRDisplayUnmountedEvent(uint32_t aDisplayID);
  void FireDOMVRDisplayConnectEvent(uint32_t aDisplayID);
  void FireDOMVRDisplayDisconnectEvent(uint32_t aDisplayID);
  void FireDOMVRDisplayPresentChangeEvent(uint32_t aDisplayID);
  void FireDOMVRDisplayConnectEventsForLoad(dom::VREventObserver* aObserver);

  virtual void HandleFatalError(const char* aMsg) const override;

<<<<<<< HEAD
 protected:
||||||| merged common ancestors
protected:
=======
  void RunPuppet(const InfallibleTArray<uint64_t>& aBuffer,
                 dom::Promise* aPromise, ErrorResult& aRv);
  void ResetPuppet(dom::Promise* aPromise, ErrorResult& aRv);

 protected:
>>>>>>> upstream-releases
  explicit VRManagerChild();
  ~VRManagerChild();
  void Destroy();
  static void DeferredDestroy(RefPtr<VRManagerChild> aVRManagerChild);

  PVRLayerChild* AllocPVRLayerChild(const uint32_t& aDisplayID,
                                    const uint32_t& aGroup);
  bool DeallocPVRLayerChild(PVRLayerChild* actor);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvUpdateDisplayInfo(
      nsTArray<VRDisplayInfo>&& aDisplayUpdates) override;

  virtual mozilla::ipc::IPCResult RecvDispatchSubmitFrameResult(
      const uint32_t& aDisplayID,
      const VRSubmitFrameResultInfo& aResult) override;
  virtual mozilla::ipc::IPCResult RecvGamepadUpdate(
      const GamepadChangeEvent& aGamepadEvent) override;
  virtual mozilla::ipc::IPCResult RecvReplyGamepadVibrateHaptic(
      const uint32_t& aPromiseID) override;

  virtual mozilla::ipc::IPCResult RecvReplyCreateVRServiceTestDisplay(
      const nsCString& aID, const uint32_t& aPromiseID,
      const uint32_t& aDeviceID) override;
  virtual mozilla::ipc::IPCResult RecvReplyCreateVRServiceTestController(
      const nsCString& aID, const uint32_t& aPromiseID,
      const uint32_t& aDeviceID) override;
  bool IsSameProcess() const { return OtherPid() == base::GetCurrentProcId(); }

 private:
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvUpdateDisplayInfo(nsTArray<VRDisplayInfo>&& aDisplayUpdates) override;

  virtual mozilla::ipc::IPCResult RecvDispatchSubmitFrameResult(const uint32_t& aDisplayID, const VRSubmitFrameResultInfo& aResult) override;
  virtual mozilla::ipc::IPCResult RecvGamepadUpdate(const GamepadChangeEvent& aGamepadEvent) override;
  virtual mozilla::ipc::IPCResult RecvReplyGamepadVibrateHaptic(const uint32_t& aPromiseID) override;

  virtual mozilla::ipc::IPCResult RecvReplyCreateVRServiceTestDisplay(const nsCString& aID,
                                                                      const uint32_t& aPromiseID,
                                                                      const uint32_t& aDeviceID) override;
  virtual mozilla::ipc::IPCResult RecvReplyCreateVRServiceTestController(const nsCString& aID,
                                                                         const uint32_t& aPromiseID,
                                                                         const uint32_t& aDeviceID) override;
  bool IsSameProcess() const
  {
    return OtherPid() == base::GetCurrentProcId();
  }
private:

=======
  // MOZ_CAN_RUN_SCRIPT_BOUNDARY until we can mark ipdl-generated things as
  // MOZ_CAN_RUN_SCRIPT.
  MOZ_CAN_RUN_SCRIPT_BOUNDARY
  mozilla::ipc::IPCResult RecvUpdateDisplayInfo(
      nsTArray<VRDisplayInfo>&& aDisplayUpdates);
  mozilla::ipc::IPCResult RecvReplyGamepadVibrateHaptic(
      const uint32_t& aPromiseID);

  MOZ_CAN_RUN_SCRIPT_BOUNDARY
  mozilla::ipc::IPCResult RecvNotifyPuppetCommandBufferCompleted(bool aSuccess);
  MOZ_CAN_RUN_SCRIPT_BOUNDARY
  mozilla::ipc::IPCResult RecvNotifyPuppetResetComplete();

  bool IsSameProcess() const { return OtherPid() == base::GetCurrentProcId(); }

 private:
>>>>>>> upstream-releases
  void FireDOMVRDisplayMountedEventInternal(uint32_t aDisplayID);
  void FireDOMVRDisplayUnmountedEventInternal(uint32_t aDisplayID);
  void FireDOMVRDisplayConnectEventInternal(uint32_t aDisplayID);
  void FireDOMVRDisplayDisconnectEventInternal(uint32_t aDisplayID);
  void FireDOMVRDisplayPresentChangeEventInternal(uint32_t aDisplayID);
  void FireDOMVRDisplayConnectEventsForLoadInternal(
      uint32_t aDisplayID, dom::VREventObserver* aObserver);
  void NotifyPresentationGenerationChangedInternal(uint32_t aDisplayID);

  nsTArray<RefPtr<VRDisplayClient>> mDisplays;
  bool mDisplaysInitialized;
  nsTArray<uint64_t> mNavigatorCallbacks;

  MessageLoop* mMessageLoop;

  struct FrameRequest;

  nsTArray<FrameRequest> mFrameRequestCallbacks;
  /**
   * The current frame request callback handle
   */
  int32_t mFrameRequestCallbackCounter;
  mozilla::TimeStamp mStartTimeStamp;

  nsTArray<RefPtr<dom::VREventObserver>> mListeners;

  layers::LayersBackend mBackend;
  RefPtr<layers::SyncObjectClient> mSyncObject;
  nsRefPtrHashtable<nsUint32HashKey, dom::Promise> mGamepadPromiseList;
  RefPtr<dom::Promise> mRunPuppetPromise;
  nsTArray<RefPtr<dom::Promise>> mResetPuppetPromises;

  DISALLOW_COPY_AND_ASSIGN(VRManagerChild);
};

}  // namespace gfx
}  // namespace mozilla

#endif  // MOZILLA_GFX_VR_VRMANAGERCHILD_H
