/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_VRServiceTest_h_
#define mozilla_dom_VRServiceTest_h_

#include "mozilla/DOMEventTargetHelper.h"
#include "mozilla/dom/VRServiceTestBinding.h"

#include "gfxVR.h"

namespace mozilla {
namespace gfx {
enum class VRDisplayCapabilityFlags : uint16_t;
enum class VRPuppet_Command : uint64_t;
}  // namespace gfx
namespace dom {
enum class GamepadCapabilityFlags : uint16_t;

class VRMockDisplay final : public DOMEventTargetHelper {
 public:
  NS_DECL_ISUPPORTS_INHERITED
  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(VRMockDisplay, DOMEventTargetHelper)

<<<<<<< HEAD
  VRMockDisplay(const nsCString& aID, uint32_t aDeviceID);
  void SetEyeParameter(VREye aEye, double aOffsetX, double aOffsetY,
                       double aOffsetZ, double aUpDegree, double aRightDegree,
                       double aDownDegree, double aLeftDegree);
  void SetEyeResolution(unsigned long aRenderWidth,
                        unsigned long aRenderHeight);
  void SetPose(const Nullable<Float32Array>& aPosition,
               const Nullable<Float32Array>& aLinearVelocity,
               const Nullable<Float32Array>& aLinearAcceleration,
               const Nullable<Float32Array>& aOrientation,
               const Nullable<Float32Array>& aAngularVelocity,
               const Nullable<Float32Array>& aAngularAcceleration);
  void SetMountState(bool aIsMounted) {
    mDisplayInfo.mDisplayState.mIsMounted = aIsMounted;
  }
  void Update();
  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) override;

 private:
||||||| merged common ancestors
  VRMockDisplay(const nsCString& aID, uint32_t aDeviceID);
  void SetEyeParameter(VREye aEye, double aOffsetX, double aOffsetY, double aOffsetZ,
                       double aUpDegree, double aRightDegree,
                       double aDownDegree, double aLeftDegree);
  void SetEyeResolution(unsigned long aRenderWidth, unsigned long aRenderHeight);
  void SetPose(const Nullable<Float32Array>& aPosition, const Nullable<Float32Array>& aLinearVelocity,
               const Nullable<Float32Array>& aLinearAcceleration, const Nullable<Float32Array>& aOrientation,
               const Nullable<Float32Array>& aAngularVelocity, const Nullable<Float32Array>& aAngularAcceleration);
  void SetMountState(bool aIsMounted) { mDisplayInfo.mDisplayState.mIsMounted = aIsMounted; }
  void Update();
  virtual JSObject* WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto) override;

private:
=======
  explicit VRMockDisplay(VRServiceTest* aVRServiceTest);

  void Create();
  void Clear();

  void SetConnected(bool aConnected);
  bool Connected() const;
  void SetMounted(bool aMounted);
  bool Mounted() const;
  void SetCapPosition(bool aEnabled);
  bool CapPosition() const;
  void SetCapOrientation(bool aEnabled);
  bool CapOrientation() const;
  void SetCapPresent(bool aEnabled);
  bool CapPresent() const;
  void SetCapExternal(bool aEnabled);
  bool CapExternal() const;
  void SetCapAngularAcceleration(bool aEnabled);
  bool CapAngularAcceleration() const;
  void SetCapLinearAcceleration(bool aEnabled);
  bool CapLinearAcceleration() const;
  void SetCapStageParameters(bool aEnabled);
  bool CapStageParameters() const;
  void SetCapMountDetection(bool aEnabled);
  bool CapMountDetection() const;
  void SetCapPositionEmulated(bool aEnabled);
  bool CapPositionEmulated() const;
  void SetEyeFOV(VREye aEye, double aUpDegree, double aRightDegree,
                 double aDownDegree, double aLeftDegree);
  void SetEyeOffset(VREye aEye, double aOffsetX, double aOffsetY,
                    double aOffsetZ);
  void SetEyeResolution(uint32_t aRenderWidth, uint32_t aRenderHeight);
  void SetStageSize(double aWidth, double aHeight);
  void SetSittingToStandingTransform(const Float32Array& aTransform,
                                     ErrorResult& aRv);
  void SetPose(const Nullable<Float32Array>& aPosition,
               const Nullable<Float32Array>& aLinearVelocity,
               const Nullable<Float32Array>& aLinearAcceleration,
               const Nullable<Float32Array>& aOrientation,
               const Nullable<Float32Array>& aAngularVelocity,
               const Nullable<Float32Array>& aAngularAcceleration,
               ErrorResult& aRv);
  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) override;

 private:
>>>>>>> upstream-releases
  ~VRMockDisplay() = default;
  gfx::VRDisplayState& DisplayState() const;
  gfx::VRHMDSensorState& SensorState() const;
  void SetCapFlag(gfx::VRDisplayCapabilityFlags aFlag, bool aEnabled);
  bool GetCapFlag(gfx::VRDisplayCapabilityFlags aFlag) const;

  RefPtr<VRServiceTest> mVRServiceTest;
};

class VRMockController : public DOMEventTargetHelper {
 public:
  NS_DECL_ISUPPORTS_INHERITED
<<<<<<< HEAD
  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(VRMockController,
                                           DOMEventTargetHelper)

  VRMockController(const nsCString& aID, uint32_t aDeviceID);
  void NewButtonEvent(unsigned long aButton, bool aPressed);
  void NewAxisMoveEvent(unsigned long aAxis, double aValue);
  void NewPoseMove(const Nullable<Float32Array>& aPosition,
                   const Nullable<Float32Array>& aLinearVelocity,
                   const Nullable<Float32Array>& aLinearAcceleration,
                   const Nullable<Float32Array>& aOrientation,
                   const Nullable<Float32Array>& aAngularVelocity,
                   const Nullable<Float32Array>& aAngularAcceleration);
  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) override;

 private:
||||||| merged common ancestors
  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(VRMockController, DOMEventTargetHelper)

  VRMockController(const nsCString& aID, uint32_t aDeviceID);
  void NewButtonEvent(unsigned long aButton, bool aPressed);
  void NewAxisMoveEvent(unsigned long aAxis, double aValue);
  void NewPoseMove(const Nullable<Float32Array>& aPosition, const Nullable<Float32Array>& aLinearVelocity,
                   const Nullable<Float32Array>& aLinearAcceleration, const Nullable<Float32Array>& aOrientation,
                   const Nullable<Float32Array>& aAngularVelocity, const Nullable<Float32Array>& aAngularAcceleration);
  virtual JSObject* WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto) override;

private:
=======
  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(VRMockController,
                                           DOMEventTargetHelper)

  VRMockController(VRServiceTest* aVRServiceTest, uint32_t aControllerIdx);
  void Create();
  void Clear();
  void SetHand(GamepadHand aHand);
  GamepadHand Hand() const;
  void SetCapPosition(bool aEnabled);
  bool CapPosition() const;
  void SetCapOrientation(bool aEnabled);
  bool CapOrientation() const;
  void SetCapAngularAcceleration(bool aEnabled);
  bool CapAngularAcceleration() const;
  void SetCapLinearAcceleration(bool aEnabled);
  bool CapLinearAcceleration() const;
  void SetAxisCount(uint32_t aCount);
  uint32_t AxisCount() const;
  void SetButtonCount(uint32_t aCount);
  uint32_t ButtonCount() const;
  void SetHapticCount(uint32_t aCount);
  uint32_t HapticCount() const;
  void SetButtonPressed(uint32_t aButtonIdx, bool aPressed);
  void SetButtonTouched(uint32_t aButtonIdx, bool aTouched);
  void SetButtonTrigger(uint32_t aButtonIdx, double aTrigger);
  void SetAxisValue(uint32_t aAxisIdx, double aValue);
  void SetPose(const Nullable<Float32Array>& aPosition,
               const Nullable<Float32Array>& aLinearVelocity,
               const Nullable<Float32Array>& aLinearAcceleration,
               const Nullable<Float32Array>& aOrientation,
               const Nullable<Float32Array>& aAngularVelocity,
               const Nullable<Float32Array>& aAngularAcceleration,
               ErrorResult& aRv);

  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) override;

 private:
>>>>>>> upstream-releases
  ~VRMockController() = default;
  gfx::VRControllerState& ControllerState() const;
  void SetCapFlag(GamepadCapabilityFlags aFlag, bool aEnabled);
  bool GetCapFlag(GamepadCapabilityFlags aFlag) const;
  RefPtr<VRServiceTest> mVRServiceTest;
  uint32_t mControllerIdx;
};

class VRServiceTest final : public DOMEventTargetHelper {
 public:
  NS_DECL_ISUPPORTS_INHERITED
  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(VRServiceTest, DOMEventTargetHelper)

<<<<<<< HEAD
  already_AddRefed<Promise> AttachVRDisplay(const nsAString& aID,
                                            ErrorResult& aRv);
  already_AddRefed<Promise> AttachVRController(const nsAString& aID,
                                               ErrorResult& aRv);
||||||| merged common ancestors
  already_AddRefed<Promise> AttachVRDisplay(const nsAString& aID, ErrorResult& aRv);
  already_AddRefed<Promise> AttachVRController(const nsAString& aID, ErrorResult& aRv);
=======
  // WebIDL interface

  void ClearAll();
  void ClearController(uint32_t aControllerIdx);
  void Commit();
  void End();
  already_AddRefed<Promise> Run(ErrorResult& aRv);
  already_AddRefed<Promise> Reset(ErrorResult& aRv);
  VRMockDisplay* GetVRDisplay();
  VRMockController* GetVRController(uint32_t aControllerIdx, ErrorResult& aRv);
  void Timeout(uint32_t aDuration);
  void Wait(uint32_t aDuration);
  void WaitSubmit();
  void WaitPresentationStart();
  void WaitPresentationEnd();
  void WaitHapticIntensity(uint32_t aControllerIdx, uint32_t aHapticIdx,
                           double aIntensity, ErrorResult& aRv);
  void CaptureFrame();
  void AcknowledgeFrame();
  void RejectFrame();
  void StartTimer();
  void StopTimer();

  // Implementation
>>>>>>> upstream-releases
  void Shutdown();
<<<<<<< HEAD

  static already_AddRefed<VRServiceTest> CreateTestService(
      nsPIDOMWindowInner* aWindow);
  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) override;

 private:
||||||| merged common ancestors

  static already_AddRefed<VRServiceTest> CreateTestService(nsPIDOMWindowInner* aWindow);
  virtual JSObject* WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto) override;

private:
=======
  void AddCommand(uint64_t aCommand);
  static already_AddRefed<VRServiceTest> CreateTestService(
      nsPIDOMWindowInner* aWindow);
  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) override;
  gfx::VRSystemState& SystemState();

 private:
>>>>>>> upstream-releases
  explicit VRServiceTest(nsPIDOMWindowInner* aWindow);
  ~VRServiceTest() = default;
  void EncodeData();

  RefPtr<VRMockDisplay> mDisplay;
  nsTArray<RefPtr<VRMockController>> mControllers;
  nsCOMPtr<nsPIDOMWindowInner> mWindow;
  // mPendingState records the state of the emulated VR hardware, including
  // changes that have not yet been committed to the command buffer.
  gfx::VRSystemState mPendingState;
  // mEncodedState records the state of the emulate VR hardware at the end
  // of the last committed transaction, submitted with VRServiceTest::Commit().
  // mPendingState represents the resulting state if all of the commands in
  // mCommandBuffer have been played back.
  gfx::VRSystemState mEncodedState;
  // mCommandBuffer encodes a sequence of steps to be executed asynchronously by
  // the simulated VR hardware.  The steps are encoded as a stream of uint64's,
  // using the format described in gfx/vr/VRPuppetCommandBuffer.h
  // mCommandBuffer includes only complete transactions, which will be played
  // back such that multiple values in VRSystemState will be updated atomically.
  // When the command buffer is submitted to the PuppetSession, with
  // VRServiceTest::Run(), it is cleared  to ensure that the commands are not
  // sent redundantly in subsequent VRServicetest::Run() calls.
  // VRServiceTest::Commit() will perform a binary comparison of mPendingState
  // and mEncodedState to determine what instructions need to be appended to
  // mCommandBuffer.
  // VRServiceTest::Reset() will effectively cancel all transactions and clear
  // mCommandBuffer before submitting the reset request to the PuppetSession.
  InfallibleTArray<uint64_t> mCommandBuffer;
  bool mShuttingDown;
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_VRServiceTest_h_
