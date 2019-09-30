/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "VRManager.h"

#include "VRManagerParent.h"
#include "VRThread.h"
#include "gfxVR.h"
#include "mozilla/ClearOnShutdown.h"
#include "mozilla/dom/VRDisplay.h"
#include "mozilla/dom/GamepadEventTypes.h"
#include "mozilla/layers/TextureHost.h"
#include "mozilla/layers/CompositorThread.h"
#include "mozilla/StaticPrefs.h"
#include "mozilla/Telemetry.h"
#include "mozilla/Unused.h"

#include "gfxVR.h"
<<<<<<< HEAD
#include "gfxVRExternal.h"
||||||| merged common ancestors
#include "gfxVRExternal.h"
#if defined(XP_WIN)
#include "gfxVROculus.h"
#endif
#if defined(XP_WIN) || defined(XP_MACOSX) || (defined(XP_LINUX) && !defined(MOZ_WIDGET_ANDROID))
#include "gfxVROpenVR.h"
#endif
=======
#include "gfxVRMutex.h"
#include <cstring>
>>>>>>> upstream-releases

#include "ipc/VRLayerParent.h"
#if !defined(MOZ_WIDGET_ANDROID)
#  include "VRServiceHost.h"
#endif

#ifdef XP_WIN
#  include "CompositorD3D11.h"
#  include "TextureD3D11.h"
#  include <d3d11.h>
#  include "gfxWindowsPlatform.h"
#  include "mozilla/gfx/DeviceManagerDx.h"
static const char* kShmemName = "moz.gecko.vr_ext.0.0.1";
#elif defined(XP_MACOSX)
#  include "mozilla/gfx/MacIOSurface.h"
#  include <sys/mman.h>
#  include <sys/stat.h> /* For mode constants */
#  include <fcntl.h>    /* For O_* constants */
#  include <errno.h>
static const char* kShmemName = "/moz.gecko.vr_ext.0.0.1";
#elif defined(MOZ_WIDGET_ANDROID)
#  include <string.h>
#  include <pthread.h>
#  include "GeckoVRManager.h"
#  include "mozilla/layers/CompositorThread.h"
#endif  // defined(MOZ_WIDGET_ANDROID)

using namespace mozilla;
using namespace mozilla::gfx;
using namespace mozilla::layers;
using namespace mozilla::gl;

#if !defined(MOZ_WIDGET_ANDROID)
namespace {
void YieldThread() {
#  if defined(XP_WIN)
  ::Sleep(0);
#  else
  ::sleep(0);
#  endif
}
}  // anonymous namespace
#endif  // !defined(MOZ_WIDGET_ANDROID)

namespace mozilla {
namespace gfx {

/**
 * When VR content is active, we run the tasks at 1ms
 * intervals, enabling multiple events to be processed
 * per frame, such as haptic feedback pulses.
 */
const uint32_t kVRActiveTaskInterval = 1;  // milliseconds

/**
 * When VR content is inactive, we run the tasks at 100ms
 * intervals, enabling VR display enumeration and
 * presentation startup to be relatively responsive
 * while not consuming unnecessary resources.
 */
const uint32_t kVRIdleTaskInterval = 100;  // milliseconds

/**
 * Max frame duration before the watchdog submits a new one.
 * Probably we can get rid of this when we enforce that SubmitFrame can only be
 * called in a VRDisplay loop.
 */
const double kVRMaxFrameSubmitDuration = 4000.0f;  // milliseconds

static StaticRefPtr<VRManager> sVRManagerSingleton;

<<<<<<< HEAD
/**
 * When VR content is active, we run the tasks at 1ms
 * intervals, enabling multiple events to be processed
 * per frame, such as haptic feedback pulses.
 */
const uint32_t kVRActiveTaskInterval = 1;  // milliseconds

/**
 * When VR content is inactive, we run the tasks at 100ms
 * intervals, enabling VR display enumeration and
 * presentation startup to be relatively responsive
 * while not consuming unnecessary resources.
 */
const uint32_t kVRIdleTaskInterval = 100;  // milliseconds

/*static*/ void VRManager::ManagerInit() {
||||||| merged common ancestors
 /**
  * When VR content is active, we run the tasks at 1ms
  * intervals, enabling multiple events to be processed
  * per frame, such as haptic feedback pulses.
  */
const uint32_t kVRActiveTaskInterval = 1; // milliseconds

 /**
  * When VR content is inactive, we run the tasks at 100ms
  * intervals, enabling VR display enumeration and
  * presentation startup to be relatively responsive
  * while not consuming unnecessary resources.
  */
const uint32_t kVRIdleTaskInterval = 100; // milliseconds

/*static*/ void
VRManager::ManagerInit()
{
=======
/* static */
VRManager* VRManager::Get() {
  MOZ_ASSERT(sVRManagerSingleton != nullptr);

  return sVRManagerSingleton;
}

Atomic<uint32_t> VRManager::sDisplayBase(0);

/* static */
uint32_t VRManager::AllocateDisplayID() { return ++sDisplayBase; }

/*static*/
void VRManager::ManagerInit() {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());

  // Enable gamepad extensions while VR is enabled.
  // Preference only can be set at the Parent process.
  if (StaticPrefs::dom_vr_enabled() && XRE_IsParentProcess()) {
    Preferences::SetBool("dom.gamepad.extensions.enabled", true);
  }

  if (sVRManagerSingleton == nullptr) {
    sVRManagerSingleton = new VRManager();
    ClearOnShutdown(&sVRManagerSingleton);
  }
}

VRManager::VRManager()
<<<<<<< HEAD
    : mInitialized(false),
      mAccumulator100ms(0.0f),
      mVRDisplaysRequested(false),
      mVRDisplaysRequestedNonFocus(false),
      mVRControllersRequested(false),
      mVRServiceStarted(false),
      mTaskInterval(0) {
||||||| merged common ancestors
  : mInitialized(false)
  , mAccumulator100ms(0.0f)
  , mVRDisplaysRequested(false)
  , mVRControllersRequested(false)
  , mVRServiceStarted(false)
  , mTaskInterval(0)
{
=======
    : mState(VRManagerState::Disabled),
      mAccumulator100ms(0.0f),
      mVRDisplaysRequested(false),
      mVRDisplaysRequestedNonFocus(false),
      mVRControllersRequested(false),
      mFrameStarted(false),
      mExternalShmem(nullptr),
      mTaskInterval(0),
      mCurrentSubmitTaskMonitor("CurrentSubmitTaskMonitor"),
      mCurrentSubmitTask(nullptr),
      mLastSubmittedFrameId(0),
      mLastStartedFrame(0),
      mEnumerationCompleted(false),
#if defined(XP_MACOSX)
      mShmemFD(0),
#elif defined(XP_WIN)
      mShmemFile(NULL),
      mMutex(NULL),
#endif
      mHapticPulseRemaining{},
      mDisplayInfo{},
      mLastUpdateDisplayInfo{},
      mBrowserState{},
      mLastSensorState{} {
>>>>>>> upstream-releases
  MOZ_COUNT_CTOR(VRManager);
  MOZ_ASSERT(sVRManagerSingleton == nullptr);
  MOZ_ASSERT(NS_IsMainThread());

<<<<<<< HEAD
  RefPtr<VRSystemManager> mgr;

||||||| merged common ancestors
  RefPtr<VRSystemManager> mgr;

  /**
   * We must add the VRDisplayManager's to mManagers in a careful order to
   * ensure that we don't detect the same VRDisplay from multiple API's.
   *
   * Oculus comes first, as it will only enumerate Oculus HMD's and is the
   * native interface for Oculus HMD's.
   *
   * OpenvR comes second, as it is the native interface for HTC Vive
   * which is the most common HMD at this time.
   *
   * OSVR will be used if Oculus SDK and OpenVR don't detect any HMDS,
   * to support everyone else.
   */

=======
#if !defined(MOZ_WIDGET_ANDROID)
  // XRE_IsGPUProcess() is helping us to check some platforms like
  // Win 7 try which are not using GPU process but VR process is enabled.
  mVRProcessEnabled =
      StaticPrefs::dom_vr_process_enabled() && XRE_IsGPUProcess();
  VRServiceHost::Init(mVRProcessEnabled);
  mServiceHost = VRServiceHost::Get();
  // We must shutdown before VRServiceHost, which is cleared
  // on ShutdownPhase::ShutdownFinal, potentially before VRManager.
  // We hold a reference to VRServiceHost to ensure it stays
  // alive until we have shut down.
#endif  // !defined(MOZ_WIDGET_ANDROID)
}

void VRManager::OpenShmem() {
  if (mExternalShmem) {
    mExternalShmem->Clear();
    return;
  }
#if defined(XP_WIN)
  if (mMutex == NULL) {
    mMutex = CreateMutex(NULL,   // default security descriptor
                         false,  // mutex not owned
                         TEXT("mozilla::vr::ShmemMutex"));  // object name
    if (mMutex == NULL) {
      nsAutoCString msg;
      msg.AppendPrintf("VRManager CreateMutex error \"%lu\".", GetLastError());
      NS_WARNING(msg.get());
      MOZ_ASSERT(false);
      return;
    }
    // At xpcshell extension tests, it creates multiple VRManager
    // instances in plug-contrainer.exe. It causes GetLastError() return
    // `ERROR_ALREADY_EXISTS`. However, even though `ERROR_ALREADY_EXISTS`, it
    // still returns the same mutex handle.
    //
    // https://docs.microsoft.com/en-us/windows/desktop/api/synchapi/nf-synchapi-createmutexa
    MOZ_ASSERT(GetLastError() == 0 || GetLastError() == ERROR_ALREADY_EXISTS);
  }
#endif  // XP_WIN
>>>>>>> upstream-releases
#if !defined(MOZ_WIDGET_ANDROID)
  // The VR Service accesses all hardware from a separate process
  // and replaces the other VRManager when enabled.
  // If the VR process is not enabled, create an in-process VRService.
  if (!mVRProcessEnabled) {
    // If the VR process is disabled, attempt to create a
    // VR service within the current process
    mExternalShmem = new VRExternalShmem();
    // VRExternalShmem is asserted to be POD
    mExternalShmem->Clear();
    mServiceHost->CreateService(mExternalShmem);
    return;
  }
<<<<<<< HEAD
  if (mVRService) {
    mExternalManager =
        VRSystemManagerExternal::Create(mVRService->GetAPIShmem());
||||||| merged common ancestors
  if (mVRService) {
    mExternalManager = VRSystemManagerExternal::Create(mVRService->GetAPIShmem());
=======
#endif

#if defined(XP_MACOSX)
  if (mShmemFD == 0) {
    mShmemFD =
        shm_open(kShmemName, O_RDWR, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
>>>>>>> upstream-releases
  }
  if (mShmemFD <= 0) {
    mShmemFD = 0;
    return;
  }

  struct stat sb;
  fstat(mShmemFD, &sb);
  off_t length = sb.st_size;
  if (length < (off_t)sizeof(VRExternalShmem)) {
    // TODO - Implement logging (Bug 1558912)
    CloseShmem();
    return;
  }

<<<<<<< HEAD
  // Enable gamepad extensions while VR is enabled.
  // Preference only can be set at the Parent process.
  if (XRE_IsParentProcess() && gfxPrefs::VREnabled()) {
    Preferences::SetBool("dom.gamepad.extensions.enabled", true);
||||||| merged common ancestors
#if defined(XP_WIN)
  if (!mVRService) {
    // The Oculus runtime is supported only on Windows
    mgr = VRSystemManagerOculus::Create();
    if (mgr) {
      mManagers.AppendElement(mgr);
    }
  }
#endif

#if defined(XP_WIN) || defined(XP_MACOSX) || (defined(XP_LINUX) && !defined(MOZ_WIDGET_ANDROID))
  if (!mVRService) {
    // OpenVR is cross platform compatible
    mgr = VRSystemManagerOpenVR::Create();
    if (mgr) {
      mManagers.AppendElement(mgr);
    }
  } // !mVRService
#endif

  // Enable gamepad extensions while VR is enabled.
  // Preference only can be set at the Parent process.
  if (XRE_IsParentProcess() && gfxPrefs::VREnabled()) {
    Preferences::SetBool("dom.gamepad.extensions.enabled", true);
=======
  mExternalShmem = (VRExternalShmem*)mmap(NULL, length, PROT_READ | PROT_WRITE,
                                          MAP_SHARED, mShmemFD, 0);
  if (mExternalShmem == MAP_FAILED) {
    // TODO - Implement logging (Bug 1558912)
    mExternalShmem = NULL;
    CloseShmem();
    return;
  }

#elif defined(XP_WIN)
  if (mShmemFile == NULL) {
    mShmemFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
                                    0, sizeof(VRExternalShmem), kShmemName);
    MOZ_ASSERT(GetLastError() == 0 || GetLastError() == ERROR_ALREADY_EXISTS);
    MOZ_ASSERT(mShmemFile);
    if (mShmemFile == NULL) {
      // TODO - Implement logging (Bug 1558912)
      CloseShmem();
      return;
    }
>>>>>>> upstream-releases
  }
  LARGE_INTEGER length;
  length.QuadPart = sizeof(VRExternalShmem);
  mExternalShmem = (VRExternalShmem*)MapViewOfFile(
      mShmemFile,           // handle to map object
      FILE_MAP_ALL_ACCESS,  // read/write permission
      0, 0, length.QuadPart);

<<<<<<< HEAD
VRManager::~VRManager() {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(!mInitialized);
  MOZ_COUNT_DTOR(VRManager);
}

void VRManager::Destroy() {
  StopTasks();
  mVRDisplays.Clear();
  mVRControllers.Clear();
  for (uint32_t i = 0; i < mManagers.Length(); ++i) {
    mManagers[i]->Destroy();
||||||| merged common ancestors
VRManager::~VRManager()
{
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(!mInitialized);
  MOZ_COUNT_DTOR(VRManager);
}

void
VRManager::Destroy()
{
  StopTasks();
  mVRDisplays.Clear();
  mVRControllers.Clear();
  for (uint32_t i = 0; i < mManagers.Length(); ++i) {
    mManagers[i]->Destroy();
=======
  if (mExternalShmem == NULL) {
    // TODO - Implement logging (Bug 1558912)
    CloseShmem();
    return;
>>>>>>> upstream-releases
  }
#elif defined(MOZ_WIDGET_ANDROID)
  mExternalShmem =
      (VRExternalShmem*)mozilla::GeckoVRManager::GetExternalContext();
  if (!mExternalShmem) {
    return;
  }
  int32_t version = -1;
  int32_t size = 0;
  if (pthread_mutex_lock((pthread_mutex_t*)&(mExternalShmem->systemMutex)) ==
      0) {
    version = mExternalShmem->version;
    size = mExternalShmem->size;
    pthread_mutex_unlock((pthread_mutex_t*)&(mExternalShmem->systemMutex));
  } else {
    return;
  }
  if (version != kVRExternalVersion) {
    mExternalShmem = nullptr;
    return;
  }
  if (size != sizeof(VRExternalShmem)) {
    mExternalShmem = nullptr;
    return;
  }
#endif
}

<<<<<<< HEAD
void VRManager::Shutdown() {
  mVRDisplays.Clear();
  mVRControllers.Clear();
  for (uint32_t i = 0; i < mManagers.Length(); ++i) {
    mManagers[i]->Shutdown();
  }
||||||| merged common ancestors
void
VRManager::Shutdown()
{
  mVRDisplays.Clear();
  mVRControllers.Clear();
  for (uint32_t i = 0; i < mManagers.Length(); ++i) {
    mManagers[i]->Shutdown();
  }
=======
void VRManager::CloseShmem() {
>>>>>>> upstream-releases
#if !defined(MOZ_WIDGET_ANDROID)
  if (!mVRProcessEnabled) {
    if (mExternalShmem) {
      delete mExternalShmem;
      mExternalShmem = nullptr;
    }
    return;
  }
<<<<<<< HEAD
  if (gfxPrefs::VRProcessEnabled() && VRGPUChild::IsCreated()) {
    RefPtr<Runnable> task =
        NS_NewRunnableFunction("VRGPUChild::SendStopVRService", []() -> void {
          VRGPUChild* vrGPUChild = VRGPUChild::Get();
          vrGPUChild->SendStopVRService();
        });
||||||| merged common ancestors
  if (gfxPrefs::VRProcessEnabled() &&
      VRGPUChild::IsCreated()) {
    RefPtr<Runnable> task = NS_NewRunnableFunction(
      "VRGPUChild::SendStopVRService",
      [] () -> void {
        VRGPUChild* vrGPUChild = VRGPUChild::Get();
        vrGPUChild->SendStopVRService();
    });
=======
#endif
#if defined(XP_MACOSX)
  if (mExternalShmem) {
    munmap((void*)mExternalShmem, sizeof(VRExternalShmem));
    mExternalShmem = NULL;
  }
  if (mShmemFD) {
    close(mShmemFD);
    mShmemFD = 0;
  }
#elif defined(XP_WIN)
  if (mExternalShmem) {
    UnmapViewOfFile((void*)mExternalShmem);
    mExternalShmem = NULL;
  }
  if (mShmemFile) {
    CloseHandle(mShmemFile);
    mShmemFile = NULL;
  }
#elif defined(MOZ_WIDGET_ANDROID)
  mExternalShmem = NULL;
#endif
}
>>>>>>> upstream-releases

VRManager::~VRManager() {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(mState == VRManagerState::Disabled);
#if !defined(MOZ_WIDGET_ANDROID)
  mServiceHost->Shutdown();
#endif
  CloseShmem();
#if defined(XP_WIN)
  if (mMutex) {
    CloseHandle(mMutex);
    mMutex = NULL;
  }
#endif
  MOZ_COUNT_DTOR(VRManager);
}

<<<<<<< HEAD
void VRManager::Init() { mInitialized = true; }
||||||| merged common ancestors
void
VRManager::Init()
{
  StartTasks();
  mInitialized = true;
}
=======
void VRManager::AddLayer(VRLayerParent* aLayer) {
  mLayers.AppendElement(aLayer);
  mDisplayInfo.mPresentingGroups |= aLayer->GetGroup();
  if (mLayers.Length() == 1) {
    StartPresentation();
  }

  // Ensure that the content process receives the change immediately
  RefreshVRDisplays();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
/* static */ VRManager* VRManager::Get() {
  MOZ_ASSERT(sVRManagerSingleton != nullptr);
||||||| merged common ancestors
/* static */VRManager*
VRManager::Get()
{
  MOZ_ASSERT(sVRManagerSingleton != nullptr);
=======
void VRManager::RemoveLayer(VRLayerParent* aLayer) {
  mLayers.RemoveElement(aLayer);
  if (mLayers.Length() == 0) {
    StopPresentation();
  }
  mDisplayInfo.mPresentingGroups = 0;
  for (auto layer : mLayers) {
    mDisplayInfo.mPresentingGroups |= layer->GetGroup();
  }
>>>>>>> upstream-releases

  // Ensure that the content process receives the change immediately
  RefreshVRDisplays();
}

<<<<<<< HEAD
void VRManager::AddVRManagerParent(VRManagerParent* aVRManagerParent) {
  if (mVRManagerParents.IsEmpty()) {
    Init();
  }
||||||| merged common ancestors
void
VRManager::AddVRManagerParent(VRManagerParent* aVRManagerParent)
{
  if (mVRManagerParents.IsEmpty()) {
    Init();
  }
=======
void VRManager::AddVRManagerParent(VRManagerParent* aVRManagerParent) {
>>>>>>> upstream-releases
  mVRManagerParents.PutEntry(aVRManagerParent);
}

void VRManager::RemoveVRManagerParent(VRManagerParent* aVRManagerParent) {
  mVRManagerParents.RemoveEntry(aVRManagerParent);
  if (mVRManagerParents.IsEmpty()) {
    Destroy();
  }
}

void VRManager::UpdateRequestedDevices() {
  bool bHaveEventListener = false;
  bool bHaveEventListenerNonFocus = false;
  bool bHaveControllerListener = false;

  for (auto iter = mVRManagerParents.Iter(); !iter.Done(); iter.Next()) {
    VRManagerParent* vmp = iter.Get()->GetKey();
    bHaveEventListener |= vmp->HaveEventListener() && vmp->GetVRActiveStatus();
    bHaveEventListenerNonFocus |=
        vmp->HaveEventListener() && !vmp->GetVRActiveStatus();
    bHaveControllerListener |= vmp->HaveControllerListener();
  }

  mVRDisplaysRequested = bHaveEventListener;
  mVRDisplaysRequestedNonFocus = bHaveEventListenerNonFocus;
  // We only currently allow controllers to be used when
  // also activating a VR display
  mVRControllersRequested = mVRDisplaysRequested && bHaveControllerListener;
}

/**
 * VRManager::NotifyVsync must be called on every 2d vsync (usually at 60hz).
 * This must be called even when no WebVR site is active.
 * If we don't have a 2d display attached to the system, we can call this
 * at the VR display's native refresh rate.
 **/
<<<<<<< HEAD
void VRManager::NotifyVsync(const TimeStamp& aVsyncTimestamp) {
  for (const auto& manager : mManagers) {
    manager->NotifyVSync();
||||||| merged common ancestors
void
VRManager::NotifyVsync(const TimeStamp& aVsyncTimestamp)
{
  for (const auto& manager : mManagers) {
    manager->NotifyVSync();
=======
void VRManager::NotifyVsync(const TimeStamp& aVsyncTimestamp) {
  if (mState != VRManagerState::Active) {
    return;
  }
  /**
   * If the display isn't presenting, refresh the sensors and trigger
   * VRDisplay.requestAnimationFrame at the normal 2d display refresh rate.
   */
  if (mDisplayInfo.mPresentingGroups == 0) {
    StartFrame();
>>>>>>> upstream-releases
  }
}

void VRManager::StartTasks() {
  if (!mTaskTimer) {
    mTaskInterval = GetOptimalTaskInterval();
    mTaskTimer = NS_NewTimer();
    mTaskTimer->SetTarget(CompositorThreadHolder::Loop()->SerialEventTarget());
    mTaskTimer->InitWithNamedFuncCallback(
        TaskTimerCallback, this, mTaskInterval,
        nsITimer::TYPE_REPEATING_PRECISE_CAN_SKIP,
        "VRManager::TaskTimerCallback");
  }
}

void VRManager::StopTasks() {
  if (mTaskTimer) {
    mTaskTimer->Cancel();
    mTaskTimer = nullptr;
  }
}

<<<<<<< HEAD
/*static*/ void VRManager::TaskTimerCallback(nsITimer* aTimer, void* aClosure) {
||||||| merged common ancestors
/*static*/ void
VRManager::TaskTimerCallback(nsITimer* aTimer, void* aClosure)
{
=======
/*static*/
void VRManager::TaskTimerCallback(nsITimer* aTimer, void* aClosure) {
>>>>>>> upstream-releases
  /**
   * It is safe to use the pointer passed in aClosure to reference the
   * VRManager object as the timer is canceled in VRManager::Destroy.
   * VRManager::Destroy set mState to VRManagerState::Disabled, which
   * is asserted in the VRManager destructor, guaranteeing that this
   * functions runs if and only if the VRManager object is valid.
   */
  VRManager* self = static_cast<VRManager*>(aClosure);
  self->RunTasks();
}

void VRManager::RunTasks() {
  // Will be called once every 1ms when a VR presentation
  // is active or once per vsync when a VR presentation is
  // not active.

  if (mState == VRManagerState::Disabled) {
    // We may have been destroyed but still have messages
    // in the queue from mTaskTimer.  Bail out to avoid
    // running them.
    return;
  }

  TimeStamp now = TimeStamp::Now();
  double lastTickMs = mAccumulator100ms;
  double deltaTime = 0.0f;
  if (!mLastTickTime.IsNull()) {
    deltaTime = (now - mLastTickTime).ToMilliseconds();
  }
  mAccumulator100ms += deltaTime;
  mLastTickTime = now;

  if (deltaTime > 0.0f && floor(mAccumulator100ms) != floor(lastTickMs)) {
    // Even if more than 1 ms has passed, we will only
    // execute Run1msTasks() once.
    Run1msTasks(deltaTime);
  }

  if (floor(mAccumulator100ms * 0.1f) != floor(lastTickMs * 0.1f)) {
    // Even if more than 10 ms has passed, we will only
    // execute Run10msTasks() once.
    Run10msTasks();
  }

  if (mAccumulator100ms >= 100.0f) {
    // Even if more than 100 ms has passed, we will only
    // execute Run100msTasks() once.
    Run100msTasks();
    mAccumulator100ms = fmod(mAccumulator100ms, 100.0f);
  }

  uint32_t optimalTaskInterval = GetOptimalTaskInterval();
  if (mTaskTimer && optimalTaskInterval != mTaskInterval) {
    mTaskTimer->SetDelay(optimalTaskInterval);
    mTaskInterval = optimalTaskInterval;
  }
}

uint32_t VRManager::GetOptimalTaskInterval() {
  /**
   * When either VR content is detected or VR hardware
   * has already been activated, we schedule tasks more
   * frequently.
   */
<<<<<<< HEAD
  bool wantGranularTasks = mVRDisplaysRequested || mVRControllersRequested ||
                           mVRDisplays.Count() || mVRControllers.Count();
||||||| merged common ancestors
  bool wantGranularTasks = mVRDisplaysRequested ||
                           mVRControllersRequested ||
                           mVRDisplays.Count() ||
                           mVRControllers.Count();
=======
  bool wantGranularTasks = mVRDisplaysRequested || mVRControllersRequested ||
                           mDisplayInfo.mDisplayID != 0;
>>>>>>> upstream-releases
  if (wantGranularTasks) {
    return kVRActiveTaskInterval;
  }

  return kVRIdleTaskInterval;
}

/**
 * Run1msTasks() is guaranteed not to be
 * called more than once within 1ms.
 * When VR is not active, this will be
 * called once per VSync if it wasn't
 * called within the last 1ms.
 */
<<<<<<< HEAD
void VRManager::Run1msTasks(double aDeltaTime) {
  for (const auto& manager : mManagers) {
    manager->Run1msTasks(aDeltaTime);
  }

  for (auto iter = mVRDisplays.Iter(); !iter.Done(); iter.Next()) {
    gfx::VRDisplayHost* display = iter.UserData();
    display->Run1msTasks(aDeltaTime);
  }
}
||||||| merged common ancestors
void
VRManager::Run1msTasks(double aDeltaTime)
{
  for (const auto& manager : mManagers) {
    manager->Run1msTasks(aDeltaTime);
  }

  for (auto iter = mVRDisplays.Iter(); !iter.Done(); iter.Next()) {
    gfx::VRDisplayHost* display = iter.UserData();
    display->Run1msTasks(aDeltaTime);
  }
}
=======
void VRManager::Run1msTasks(double aDeltaTime) { UpdateHaptics(aDeltaTime); }
>>>>>>> upstream-releases

/**
 * Run10msTasks() is guaranteed not to be
 * called more than once within 10ms.
 * When VR is not active, this will be
 * called once per VSync if it wasn't
 * called within the last 10ms.
 */
void VRManager::Run10msTasks() {
  UpdateRequestedDevices();
  CheckWatchDog();
  ExpireNavigationTransition();
  PullState();
  PushState();
}

/**
 * Run100msTasks() is guaranteed not to be
 * called more than once within 100ms.
 * When VR is not active, this will be
 * called once per VSync if it wasn't
 * called within the last 100ms.
 */
void VRManager::Run100msTasks() {
  // We must continually refresh the VR display enumeration to check
  // for events that we must fire such as Window.onvrdisplayconnect
  // Note that enumeration itself may activate display hardware, such
  // as Oculus, so we only do this when we know we are displaying content
  // that is looking for VR displays.
#if !defined(MOZ_WIDGET_ANDROID)
  mServiceHost->Refresh();
  CheckForPuppetCompletion();
#endif
  RefreshVRDisplays();
  CheckForInactiveTimeout();
  CheckForShutdown();
}

void VRManager::CheckForInactiveTimeout() {
  // Shut down the VR devices when not in use
  if (mVRDisplaysRequested || mVRDisplaysRequestedNonFocus ||
      mVRControllersRequested) {
    // We are using a VR device, keep it alive
    mLastActiveTime = TimeStamp::Now();
  } else if (mLastActiveTime.IsNull()) {
    Shutdown();
  } else {
    TimeDuration duration = TimeStamp::Now() - mLastActiveTime;
    if (duration.ToMilliseconds() > StaticPrefs::dom_vr_inactive_timeout()) {
      Shutdown();
      // We must not throttle the next enumeration request
      // after an idle timeout, as it may result in the
      // user needing to refresh the browser to detect
      // VR hardware when leaving and returning to a VR
      // site.
      mLastDisplayEnumerationTime = TimeStamp();
    }
  }
}

<<<<<<< HEAD
void VRManager::NotifyVRVsync(const uint32_t& aDisplayID) {
  for (const auto& manager : mManagers) {
    if (manager->GetIsPresenting()) {
      manager->HandleInput();
    }
||||||| merged common ancestors
void
VRManager::NotifyVRVsync(const uint32_t& aDisplayID)
{
  for (const auto& manager: mManagers) {
    if (manager->GetIsPresenting()) {
      manager->HandleInput();
    }
=======
void VRManager::CheckForShutdown() {
  // Check for remote end shutdown
  if (mState != VRManagerState::Disabled && mState != VRManagerState::Idle &&
      mDisplayInfo.mDisplayState.shutdown) {
    Shutdown();
>>>>>>> upstream-releases
  }
}

#if !defined(MOZ_WIDGET_ANDROID)
void VRManager::CheckForPuppetCompletion() {
  // Notify content process about completion of puppet test resets
  if (mState != VRManagerState::Active) {
    for (auto iter = mManagerParentsWaitingForPuppetReset.Iter(); !iter.Done();
         iter.Next()) {
      Unused << iter.Get()->GetKey()->SendNotifyPuppetResetComplete();
    }
    mManagerParentsWaitingForPuppetReset.Clear();
  }
  // Notify content process about completion of puppet test scripts
  if (mManagerParentRunningPuppet) {
    if (mServiceHost->PuppetHasEnded()) {
      Unused << mManagerParentRunningPuppet
                    ->SendNotifyPuppetCommandBufferCompleted(true);
      mManagerParentRunningPuppet = nullptr;
    }
  }
}
#endif  // !defined(MOZ_WIDGET_ANDROID)

<<<<<<< HEAD
void VRManager::EnumerateVRDisplays() {
  StartTasks();
  /**
   * Throttle the rate of enumeration to the interval set in
   * VRDisplayEnumerateInterval
   */
  if (!mLastDisplayEnumerationTime.IsNull()) {
    TimeDuration duration = TimeStamp::Now() - mLastDisplayEnumerationTime;
    if (duration.ToMilliseconds() < gfxPrefs::VRDisplayEnumerateInterval()) {
      return;
    }
||||||| merged common ancestors
void
VRManager::EnumerateVRDisplays()
{
  /**
   * Throttle the rate of enumeration to the interval set in
   * VRDisplayEnumerateInterval
   */
  if (!mLastDisplayEnumerationTime.IsNull()) {
    TimeDuration duration = TimeStamp::Now() - mLastDisplayEnumerationTime;
    if (duration.ToMilliseconds() < gfxPrefs::VRDisplayEnumerateInterval()) {
      return;
    }
=======
void VRManager::StartFrame() {
  if (mState != VRManagerState::Active) {
    return;
>>>>>>> upstream-releases
  }
  AUTO_PROFILER_TRACING("VR", "GetSensorState", OTHER);

  /**
   * Do not start more VR frames until the last submitted frame is already
   * processed, or the last has stalled for more than
   * kVRMaxFrameSubmitDuration milliseconds.
   */
  TimeStamp now = TimeStamp::Now();
  const TimeStamp lastFrameStart =
      mLastFrameStart[mDisplayInfo.mFrameId % kVRMaxLatencyFrames];
  const bool isPresenting = mLastUpdateDisplayInfo.GetPresentingGroups() != 0;
  double duration =
      lastFrameStart.IsNull() ? 0.0 : (now - lastFrameStart).ToMilliseconds();
  if (isPresenting && mLastStartedFrame > 0 &&
      mDisplayInfo.mDisplayState.lastSubmittedFrameId < mLastStartedFrame &&
      duration < kVRMaxFrameSubmitDuration) {
    return;
  }

  mDisplayInfo.mFrameId++;
  size_t bufferIndex = mDisplayInfo.mFrameId % kVRMaxLatencyFrames;
  mDisplayInfo.mLastSensorState[bufferIndex] = mLastSensorState;
  mLastFrameStart[bufferIndex] = now;
  mFrameStarted = true;
  mLastStartedFrame = mDisplayInfo.mFrameId;

<<<<<<< HEAD
  /**
   * We must start the VR Service thread
   * and VR Process before enumeration.
   * We don't want to start this until we will
   * actualy enumerate, to avoid continuously
   * re-launching the thread/process when
   * no hardware is found or a VR software update
   * is in progress
   */
#if !defined(MOZ_WIDGET_ANDROID)
  // Tell VR process to start VR service.
  if (gfxPrefs::VRProcessEnabled() && !mVRServiceStarted) {
    RefPtr<Runnable> task =
        NS_NewRunnableFunction("VRGPUChild::SendStartVRService", []() -> void {
          VRGPUChild* vrGPUChild = VRGPUChild::Get();
          vrGPUChild->SendStartVRService();
        });

    NS_DispatchToMainThread(task.forget());
    mVRServiceStarted = true;
  } else if (!gfxPrefs::VRProcessEnabled()) {
    if (mVRService) {
      mVRService->Start();
      mVRServiceStarted = true;
||||||| merged common ancestors
  /**
   * We must start the VR Service thread
   * and VR Process before enumeration.
   * We don't want to start this until we will
   * actualy enumerate, to avoid continuously
   * re-launching the thread/process when
   * no hardware is found or a VR software update
   * is in progress
   */
#if !defined(MOZ_WIDGET_ANDROID)
    // Tell VR process to start VR service.
    if (gfxPrefs::VRProcessEnabled() && !mVRServiceStarted) {
      RefPtr<Runnable> task = NS_NewRunnableFunction(
        "VRGPUChild::SendStartVRService",
        [] () -> void {
          VRGPUChild* vrGPUChild = VRGPUChild::Get();
          vrGPUChild->SendStartVRService();
      });

      NS_DispatchToMainThread(task.forget());
      mVRServiceStarted = true;
    } else if (!gfxPrefs::VRProcessEnabled()){
      if (mVRService) {
        mVRService->Start();
        mVRServiceStarted = true;
      }
=======
  DispatchVRDisplayInfoUpdate();
}

void VRManager::EnumerateVRDisplays() {
  if (!StaticPrefs::dom_vr_enabled()) {
    return;
  }

  if (mState == VRManagerState::Disabled) {
    StartTasks();
    mState = VRManagerState::Idle;
  }

  if (mState == VRManagerState::Idle) {
    /**
     * Throttle the rate of enumeration to the interval set in
     * VRDisplayEnumerateInterval
     */
    if (!mLastDisplayEnumerationTime.IsNull()) {
      TimeDuration duration = TimeStamp::Now() - mLastDisplayEnumerationTime;
      if (duration.ToMilliseconds() <
          StaticPrefs::dom_vr_display_enumerate_interval()) {
        return;
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
#endif
||||||| merged common ancestors
#endif
=======
>>>>>>> upstream-releases

    if (!mEarliestRestartTime.IsNull() &&
        mEarliestRestartTime > TimeStamp::Now()) {
      // When the VR Service shuts down it informs us of how long we
      // must wait until we can re-start it.
      // We must wait until mEarliestRestartTime before attempting
      // to enumerate again.
      return;
    }

    /**
     * If we get this far, don't try again until
     * the VRDisplayEnumerateInterval elapses
     */
    mLastDisplayEnumerationTime = TimeStamp::Now();

    OpenShmem();

    /**
     * We must start the VR Service thread
     * and VR Process before enumeration.
     * We don't want to start this until we will
     * actualy enumerate, to avoid continuously
     * re-launching the thread/process when
     * no hardware is found or a VR software update
     * is in progress
     */
#if !defined(MOZ_WIDGET_ANDROID)
    mServiceHost->StartService();
#endif
    if (mExternalShmem) {
      mDisplayInfo.Clear();
      mLastUpdateDisplayInfo.Clear();
      mFrameStarted = false;
      mBrowserState.Clear();
      mLastSensorState.Clear();
      mDisplayInfo.mGroupMask = kVRGroupContent;
      // We must block until enumeration has completed in order
      // to signal that the WebVR promise should be resolved at the
      // right time.
      mState = VRManagerState::Enumeration;
    }
  }  // if (mState == VRManagerState::Idle)

  if (mState == VRManagerState::Enumeration) {
    MOZ_ASSERT(mExternalShmem != nullptr);

    PullState();
    if (mEnumerationCompleted) {
      if (mDisplayInfo.mDisplayState.isConnected) {
        mDisplayInfo.mDisplayID = VRManager::AllocateDisplayID();
        mState = VRManagerState::Active;
      } else {
        Shutdown();
      }
    }
  }  // if (mState == VRManagerState::Enumeration)
}

<<<<<<< HEAD
void VRManager::RefreshVRDisplays(bool aMustDispatch) {
||||||| merged common ancestors
void
VRManager::RefreshVRDisplays(bool aMustDispatch)
{
=======
void VRManager::RefreshVRDisplays(bool aMustDispatch) {
  uint32_t previousDisplayID = mDisplayInfo.GetDisplayID();

>>>>>>> upstream-releases
  /**
   * If we aren't viewing WebVR content, don't enumerate
   * new hardware, as it will cause some devices to power on
   * or interrupt other VR activities.
   */
  if (mVRDisplaysRequested || aMustDispatch) {
    EnumerateVRDisplays();
  }
  if (mState == VRManagerState::Enumeration) {
    // If we are enumerating VR Displays, do not dispatch
    // updates until the enumeration has completed.
    return;
  }
  bool changed = false;

<<<<<<< HEAD
  /**
   * VRSystemManager::GetHMDs will not activate new hardware
   * or result in interruption of other VR activities.
   * We can call it even when suppressing enumeration to get
   * the already-enumerated displays.
   */
  nsTArray<RefPtr<gfx::VRDisplayHost>> displays;
  for (const auto& manager : mManagers) {
    manager->GetHMDs(displays);
||||||| merged common ancestors
  /**
   * VRSystemManager::GetHMDs will not activate new hardware
   * or result in interruption of other VR activities.
   * We can call it even when suppressing enumeration to get
   * the already-enumerated displays.
   */
  nsTArray<RefPtr<gfx::VRDisplayHost> > displays;
  for (const auto& manager: mManagers) {
    manager->GetHMDs(displays);
=======
  if (previousDisplayID != mDisplayInfo.GetDisplayID()) {
    changed = true;
>>>>>>> upstream-releases
  }

  if (mState == VRManagerState::Active &&
      mDisplayInfo != mLastUpdateDisplayInfo) {
    // This display's info has changed
    changed = true;
  }

  if (changed || aMustDispatch) {
    DispatchVRDisplayInfoUpdate();
  }
}

<<<<<<< HEAD
  for (const auto& display : displays) {
    if (!GetDisplay(display->GetDisplayInfo().GetDisplayID())) {
      // This is a new display
      displaySetChanged = true;
      break;
    }
||||||| merged common ancestors
  for (const auto& display: displays) {
    if (!GetDisplay(display->GetDisplayInfo().GetDisplayID())) {
      // This is a new display
      displaySetChanged = true;
      break;
    }
=======
void VRManager::DispatchVRDisplayInfoUpdate() {
  // This could be simplified further by only supporting one display
  nsTArray<VRDisplayInfo> displayUpdates;
  if (mState == VRManagerState::Active) {
    MOZ_ASSERT(mDisplayInfo.mDisplayID != 0);
    displayUpdates.AppendElement(mDisplayInfo);
  }
  for (auto iter = mVRManagerParents.Iter(); !iter.Done(); iter.Next()) {
    Unused << iter.Get()->GetKey()->SendUpdateDisplayInfo(displayUpdates);
  }
  mLastUpdateDisplayInfo = mDisplayInfo;
}

void VRManager::StopAllHaptics() {
  for (size_t i = 0; i < mozilla::ArrayLength(mBrowserState.hapticState); i++) {
    ClearHapticSlot(i);
  }
  PushState();
}
>>>>>>> upstream-releases

void VRManager::VibrateHaptic(uint32_t aControllerIdx, uint32_t aHapticIndex,
                              double aIntensity, double aDuration,
                              const VRManagerPromise& aPromise)

{
  if (mState != VRManagerState::Active) {
    return;
  }
  // VRDisplayClient::FireGamepadEvents() assigns a controller ID with
  // ranges based on displayID.  We must translate this to the indexes
  // understood by VRDisplayExternal.
  uint32_t controllerBaseIndex =
      kVRControllerMaxCount * mDisplayInfo.mDisplayID;
  uint32_t controllerIndex = aControllerIdx - controllerBaseIndex;

  TimeStamp now = TimeStamp::Now();
  size_t bestSlotIndex = 0;
  // Default to an empty slot, or the slot holding the oldest haptic pulse
  for (size_t i = 0; i < mozilla::ArrayLength(mBrowserState.hapticState); i++) {
    const VRHapticState& state = mBrowserState.hapticState[i];
    if (state.inputFrameID == 0) {
      // Unused slot, use it
      bestSlotIndex = i;
      break;
    }
    if (mHapticPulseRemaining[i] < mHapticPulseRemaining[bestSlotIndex]) {
      // If no empty slots are available, fall back to overriding
      // the pulse which is ending soonest.
      bestSlotIndex = i;
    }
  }
<<<<<<< HEAD

  // Rebuild the HashMap if there are additions or removals
  if (displaySetChanged) {
    mVRDisplays.Clear();
    for (const auto& display : displays) {
      mVRDisplays.Put(display->GetDisplayInfo().GetDisplayID(), display);
||||||| merged common ancestors

  // Rebuild the HashMap if there are additions or removals
  if (displaySetChanged) {
    mVRDisplays.Clear();
    for (const auto& display: displays) {
      mVRDisplays.Put(display->GetDisplayInfo().GetDisplayID(), display);
=======
  // Override the last pulse on the same actuator if present.
  for (size_t i = 0; i < mozilla::ArrayLength(mBrowserState.hapticState); i++) {
    const VRHapticState& state = mBrowserState.hapticState[i];
    if (state.inputFrameID == 0) {
      // This is an empty slot -- no match
      continue;
    }
    if (state.controllerIndex == controllerIndex &&
        state.hapticIndex == aHapticIndex) {
      // Found pulse on same actuator -- let's override it.
      bestSlotIndex = i;
>>>>>>> upstream-releases
    }
  }
  ClearHapticSlot(bestSlotIndex);

  // Populate the selected slot with new haptic state
  size_t bufferIndex = mDisplayInfo.mFrameId % kVRMaxLatencyFrames;
  VRHapticState& bestSlot = mBrowserState.hapticState[bestSlotIndex];
  bestSlot.inputFrameID =
      mDisplayInfo.mLastSensorState[bufferIndex].inputFrameID;
  bestSlot.controllerIndex = controllerIndex;
  bestSlot.hapticIndex = aHapticIndex;
  bestSlot.pulseStart = (float)(now - mLastFrameStart[bufferIndex]).ToSeconds();
  bestSlot.pulseDuration =
      (float)aDuration * 0.001f;  // Convert from ms to seconds
  bestSlot.pulseIntensity = (float)aIntensity;

  mHapticPulseRemaining[bestSlotIndex] = aDuration;
  MOZ_ASSERT(bestSlotIndex <= mHapticPromises.Length());
  if (bestSlotIndex == mHapticPromises.Length()) {
    mHapticPromises.AppendElement(
        UniquePtr<VRManagerPromise>(new VRManagerPromise(aPromise)));
  } else {
    mHapticPromises[bestSlotIndex] =
        UniquePtr<VRManagerPromise>(new VRManagerPromise(aPromise));
  }
  PushState();
}

void VRManager::StopVibrateHaptic(uint32_t aControllerIdx) {
  if (mState != VRManagerState::Active) {
    return;
  }
  // VRDisplayClient::FireGamepadEvents() assigns a controller ID with
  // ranges based on displayID.  We must translate this to the indexes
  // understood by VRDisplayExternal.
  uint32_t controllerBaseIndex =
      kVRControllerMaxCount * mDisplayInfo.mDisplayID;
  uint32_t controllerIndex = aControllerIdx - controllerBaseIndex;

  for (size_t i = 0; i < mozilla::ArrayLength(mBrowserState.hapticState); i++) {
    VRHapticState& state = mBrowserState.hapticState[i];
    if (state.controllerIndex == controllerIndex) {
      memset(&state, 0, sizeof(VRHapticState));
    }
  }
  PushState();
}

<<<<<<< HEAD
void VRManager::DispatchVRDisplayInfoUpdate() {
  nsTArray<VRDisplayInfo> update;
  GetVRDisplayInfo(update);
||||||| merged common ancestors
void
VRManager::DispatchVRDisplayInfoUpdate()
{
  nsTArray<VRDisplayInfo> update;
  GetVRDisplayInfo(update);
=======
void VRManager::NotifyVibrateHapticCompleted(const VRManagerPromise& aPromise) {
  aPromise.mParent->SendReplyGamepadVibrateHaptic(aPromise.mPromiseID);
}
>>>>>>> upstream-releases

void VRManager::StartVRNavigation(const uint32_t& aDisplayID) {
  if (mState != VRManagerState::Active) {
    return;
  }
  /**
   * We only support a single VRSession with a single VR display at a
   * time; however, due to the asynchronous nature of the API, it's possible
   * that the previously used VR display was a different one than the one now
   * allocated. We catch these cases to avoid automatically activating the new
   * VR displays. This situation is expected to be very rare and possibly never
   * seen. Perhaps further simplification could be made in the content process
   * code which passes around displayID's that may no longer be needed.
   **/
  if (mDisplayInfo.GetDisplayID() != aDisplayID) {
    return;
  }
  mBrowserState.navigationTransitionActive = true;
  mVRNavigationTransitionEnd = TimeStamp();
  PushState();
}

<<<<<<< HEAD
/**
 * Get any VR displays that have already been enumerated without
 * activating any new devices.
 */
void VRManager::GetVRDisplayInfo(nsTArray<VRDisplayInfo>& aDisplayInfo) {
  aDisplayInfo.Clear();
  for (auto iter = mVRDisplays.Iter(); !iter.Done(); iter.Next()) {
    gfx::VRDisplayHost* display = iter.UserData();
    aDisplayInfo.AppendElement(VRDisplayInfo(display->GetDisplayInfo()));
||||||| merged common ancestors

/**
 * Get any VR displays that have already been enumerated without
 * activating any new devices.
 */
void
VRManager::GetVRDisplayInfo(nsTArray<VRDisplayInfo>& aDisplayInfo)
{
  aDisplayInfo.Clear();
  for (auto iter = mVRDisplays.Iter(); !iter.Done(); iter.Next()) {
    gfx::VRDisplayHost* display = iter.UserData();
    aDisplayInfo.AppendElement(VRDisplayInfo(display->GetDisplayInfo()));
=======
void VRManager::StopVRNavigation(const uint32_t& aDisplayID,
                                 const TimeDuration& aTimeout) {
  if (mState != VRManagerState::Active) {
    return;
  }
  if (mDisplayInfo.GetDisplayID() != aDisplayID) {
    return;
  }
  if (aTimeout.ToMilliseconds() <= 0) {
    mBrowserState.navigationTransitionActive = false;
    mVRNavigationTransitionEnd = TimeStamp();
    PushState();
  }
  mVRNavigationTransitionEnd = TimeStamp::Now() + aTimeout;
}

#if !defined(MOZ_WIDGET_ANDROID)

bool VRManager::RunPuppet(const InfallibleTArray<uint64_t>& aBuffer,
                          VRManagerParent* aManagerParent) {
  if (!StaticPrefs::dom_vr_puppet_enabled()) {
    // Sanity check to ensure that a compromised content process
    // can't use this to escalate permissions.
    return false;
  }
  if (mManagerParentRunningPuppet != nullptr) {
    // Only one parent may run a puppet at a time
    return false;
>>>>>>> upstream-releases
  }
  mManagerParentRunningPuppet = aManagerParent;
  mServiceHost->PuppetSubmit(aBuffer);
  return true;
}

<<<<<<< HEAD
RefPtr<gfx::VRDisplayHost> VRManager::GetDisplay(const uint32_t& aDisplayID) {
  RefPtr<gfx::VRDisplayHost> display;
  if (mVRDisplays.Get(aDisplayID, getter_AddRefs(display))) {
    return display;
||||||| merged common ancestors
RefPtr<gfx::VRDisplayHost>
VRManager::GetDisplay(const uint32_t& aDisplayID)
{
  RefPtr<gfx::VRDisplayHost> display;
  if (mVRDisplays.Get(aDisplayID, getter_AddRefs(display))) {
    return display;
=======
void VRManager::ResetPuppet(VRManagerParent* aManagerParent) {
  mManagerParentsWaitingForPuppetReset.PutEntry(aManagerParent);
  if (mManagerParentRunningPuppet != nullptr) {
    Unused << mManagerParentRunningPuppet
                  ->SendNotifyPuppetCommandBufferCompleted(false);
    mManagerParentRunningPuppet = nullptr;
  }
  mServiceHost->PuppetReset();
  // In the event that we are shut down, the task timer won't be running
  // to trigger CheckForPuppetCompletion.
  // In this case, CheckForPuppetCompletion() would immediately resolve
  // the promises for mManagerParentsWaitingForPuppetReset.
  // We can simply call it once here to handle that case.
  CheckForPuppetCompletion();
}

#endif  // !defined(MOZ_WIDGET_ANDROID)

#if defined(MOZ_WIDGET_ANDROID)
void VRManager::PullState(
    const std::function<bool()>& aWaitCondition /* = nullptr */) {
  if (!mExternalShmem) {
    return;
>>>>>>> upstream-releases
  }
  bool done = false;
  while (!done) {
    if (pthread_mutex_lock((pthread_mutex_t*)&(mExternalShmem->systemMutex)) ==
        0) {
      while (true) {
        memcpy(&mDisplayInfo.mDisplayState,
               (void*)&(mExternalShmem->state.displayState),
               sizeof(VRDisplayState));
        memcpy(&mLastSensorState, (void*)&(mExternalShmem->state.sensorState),
               sizeof(VRHMDSensorState));
        memcpy(mDisplayInfo.mControllerState,
               (void*)&(mExternalShmem->state.controllerState),
               sizeof(VRControllerState) * kVRControllerMaxCount);
        mEnumerationCompleted = mExternalShmem->state.enumerationCompleted;
        if (!aWaitCondition || aWaitCondition()) {
          done = true;
          break;
        }
        // Block current thead using the condition variable until data
        // changes
        pthread_cond_wait((pthread_cond_t*)&mExternalShmem->systemCond,
                          (pthread_mutex_t*)&mExternalShmem->systemMutex);
      }  // while (true)
      pthread_mutex_unlock((pthread_mutex_t*)&(mExternalShmem->systemMutex));
    } else if (!aWaitCondition) {
      // pthread_mutex_lock failed and we are not waiting for a condition to
      // exit from PullState call.
      return;
    }
  }  // while (!done) {
}
#else

<<<<<<< HEAD
RefPtr<gfx::VRControllerHost> VRManager::GetController(
    const uint32_t& aControllerID) {
  RefPtr<gfx::VRControllerHost> controller;
  if (mVRControllers.Get(aControllerID, getter_AddRefs(controller))) {
    return controller;
||||||| merged common ancestors
RefPtr<gfx::VRControllerHost>
VRManager::GetController(const uint32_t& aControllerID)
{
  RefPtr<gfx::VRControllerHost> controller;
  if (mVRControllers.Get(aControllerID, getter_AddRefs(controller))) {
    return controller;
=======
void VRManager::PullState(
    const std::function<bool()>& aWaitCondition /* = nullptr */) {
  MOZ_ASSERT(mExternalShmem);
  if (!mExternalShmem) {
    return;
>>>>>>> upstream-releases
  }
  while (true) {
    {  // Scope for WaitForMutex
#  if defined(XP_WIN)
      bool status = true;
      WaitForMutex lock(mMutex);
      status = lock.GetStatus();
      if (status) {
#  endif  // defined(XP_WIN)
        VRExternalShmem tmp;
        memcpy(&tmp, (void*)mExternalShmem, sizeof(VRExternalShmem));
        bool isCleanCopy =
            tmp.generationA == tmp.generationB && tmp.generationA != 0;
        if (isCleanCopy) {
          memcpy(&mDisplayInfo.mDisplayState, &tmp.state.displayState,
                 sizeof(VRDisplayState));
          memcpy(&mLastSensorState, &tmp.state.sensorState,
                 sizeof(VRHMDSensorState));
          memcpy(mDisplayInfo.mControllerState,
                 (void*)&(mExternalShmem->state.controllerState),
                 sizeof(VRControllerState) * kVRControllerMaxCount);
          mEnumerationCompleted = mExternalShmem->state.enumerationCompleted;
          // Check for wait condition
          if (!aWaitCondition || aWaitCondition()) {
            return;
          }
        }  // if (isCleanCopy)
        // Yield the thread while polling
        YieldThread();
#  if defined(XP_WIN)
      } else if (!aWaitCondition) {
        // WaitForMutex failed and we are not waiting for a condition to
        // exit from PullState call.
        return;
      }
#  endif  // defined(XP_WIN)
    }  // End: Scope for WaitForMutex
    // Yield the thread while polling
    YieldThread();
  }  // while (!true)
}
#endif    // defined(MOZ_WIDGET_ANDROID)

<<<<<<< HEAD
void VRManager::GetVRControllerInfo(
    nsTArray<VRControllerInfo>& aControllerInfo) {
  aControllerInfo.Clear();
  for (auto iter = mVRControllers.Iter(); !iter.Done(); iter.Next()) {
    gfx::VRControllerHost* controller = iter.UserData();
    aControllerInfo.AppendElement(
        VRControllerInfo(controller->GetControllerInfo()));
||||||| merged common ancestors
void
VRManager::GetVRControllerInfo(nsTArray<VRControllerInfo>& aControllerInfo)
{
  aControllerInfo.Clear();
  for (auto iter = mVRControllers.Iter(); !iter.Done(); iter.Next()) {
    gfx::VRControllerHost* controller = iter.UserData();
    aControllerInfo.AppendElement(VRControllerInfo(controller->GetControllerInfo()));
=======
void VRManager::PushState(bool aNotifyCond) {
  if (!mExternalShmem) {
    return;
>>>>>>> upstream-releases
  }
#if defined(MOZ_WIDGET_ANDROID)
  if (pthread_mutex_lock((pthread_mutex_t*)&(mExternalShmem->geckoMutex)) ==
      0) {
    memcpy((void*)&(mExternalShmem->geckoState), (void*)&mBrowserState,
           sizeof(VRBrowserState));
    if (aNotifyCond) {
      pthread_cond_signal((pthread_cond_t*)&(mExternalShmem->geckoCond));
    }
    pthread_mutex_unlock((pthread_mutex_t*)&(mExternalShmem->geckoMutex));
  }
#else
  bool status = true;
#  if defined(XP_WIN)
  WaitForMutex lock(mMutex);
  status = lock.GetStatus();
#  endif  // defined(XP_WIN)
  if (status) {
    mExternalShmem->geckoGenerationA++;
    memcpy((void*)&(mExternalShmem->geckoState), (void*)&mBrowserState,
           sizeof(VRBrowserState));
    mExternalShmem->geckoGenerationB++;
  }
#endif    // defined(MOZ_WIDGET_ANDROID)
}

<<<<<<< HEAD
void VRManager::RefreshVRControllers() {
  ScanForControllers();
||||||| merged common ancestors
void
VRManager::RefreshVRControllers()
{
  ScanForControllers();
=======
void VRManager::Destroy() {
  if (mState == VRManagerState::Disabled) {
    return;
  }
  Shutdown();
  StopTasks();
  mState = VRManagerState::Disabled;
}
>>>>>>> upstream-releases

void VRManager::Shutdown() {
  if (mState == VRManagerState::Disabled || mState == VRManagerState::Idle) {
    return;
  }

<<<<<<< HEAD
  for (uint32_t i = 0; i < mManagers.Length() && controllers.Length() == 0;
       ++i) {
    mManagers[i]->GetControllers(controllers);
||||||| merged common ancestors
  for (uint32_t i = 0; i < mManagers.Length()
      && controllers.Length() == 0; ++i) {
    mManagers[i]->GetControllers(controllers);
=======
  if (mDisplayInfo.mDisplayState.shutdown) {
    // Shutdown was requested by VR Service, so we must throttle
    // as requested by the VR Service
    TimeStamp now = TimeStamp::Now();
    mEarliestRestartTime =
        now + TimeDuration::FromMilliseconds(
                  (double)mDisplayInfo.mDisplayState.minRestartInterval);
>>>>>>> upstream-releases
  }

  StopAllHaptics();
  StopPresentation();
  CancelCurrentSubmitTask();
  ShutdownSubmitThread();
  mDisplayInfo.Clear();
  mEnumerationCompleted = false;

  if (mState == VRManagerState::Enumeration) {
    // Ensure that enumeration promises are resolved
    DispatchVRDisplayInfoUpdate();
  }

<<<<<<< HEAD
  for (const auto& controller : controllers) {
    if (!GetController(controller->GetControllerInfo().GetControllerID())) {
      // This is a new controller
      controllerInfoChanged = true;
      break;
||||||| merged common ancestors
  for (const auto& controller: controllers) {
    if (!GetController(controller->GetControllerInfo().GetControllerID())) {
      // This is a new controller
      controllerInfoChanged = true;
      break;
=======
#if !defined(MOZ_WIDGET_ANDROID)
  mServiceHost->StopService();
#endif
  mState = VRManagerState::Idle;

  // We will close Shmem in the DTOR to avoid
  // mSubmitThread is still running but its shmem
  // has been released.
}

void VRManager::CheckWatchDog() {
  /**
   * We will trigger a new frame immediately after a successful frame
   * texture submission.  If content fails to call VRDisplay.submitFrame
   * after dom.vr.display.rafMaxDuration milliseconds has elapsed since the
   * last VRDisplay.requestAnimationFrame, we act as a "watchdog" and
   * kick-off a new VRDisplay.requestAnimationFrame to avoid a render loop
   * stall and to give content a chance to recover.
   *
   * If the lower level VR platform API's are rejecting submitted frames,
   * such as when the Oculus "Health and Safety Warning" is displayed,
   * we will not kick off the next frame immediately after
   * VRDisplay.submitFrame as it would result in an unthrottled render loop
   * that would free run at potentially extreme frame rates.  To ensure that
   * content has a chance to resume its presentation when the frames are
   * accepted once again, we rely on this "watchdog" to act as a VR refresh
   * driver cycling at a rate defined by dom.vr.display.rafMaxDuration.
   *
   * This number must be larger than the slowest expected frame time during
   * normal VR presentation, but small enough not to break content that
   * makes assumptions of reasonably minimal VSync rate.
   *
   * The slowest expected refresh rate for a VR display currently is an
   * Oculus CV1 when ASW (Asynchronous Space Warp) is enabled, at 45hz.
   * A dom.vr.display.rafMaxDuration value of 50 milliseconds results in a
   * 20hz rate, which avoids inadvertent triggering of the watchdog during
   * Oculus ASW even if every second frame is dropped.
   */
  if (mState != VRManagerState::Active) {
    return;
  }
  bool bShouldStartFrame = false;

  // If content fails to call VRDisplay.submitFrame, we must eventually
  // time-out and trigger a new frame.
  TimeStamp lastFrameStart =
      mLastFrameStart[mDisplayInfo.mFrameId % kVRMaxLatencyFrames];
  if (lastFrameStart.IsNull()) {
    bShouldStartFrame = true;
  } else {
    TimeDuration duration = TimeStamp::Now() - lastFrameStart;
    if (duration.ToMilliseconds() >
        StaticPrefs::dom_vr_display_rafMaxDuration()) {
      bShouldStartFrame = true;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (controllerInfoChanged) {
    mVRControllers.Clear();
    for (const auto& controller : controllers) {
      mVRControllers.Put(controller->GetControllerInfo().GetControllerID(),
                         controller);
    }
||||||| merged common ancestors
  if (controllerInfoChanged) {
    mVRControllers.Clear();
    for (const auto& controller: controllers) {
      mVRControllers.Put(controller->GetControllerInfo().GetControllerID(),
                         controller);
    }
=======
  if (bShouldStartFrame) {
    StartFrame();
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
void VRManager::ScanForControllers() {
  // We don't have to do this every frame, so check if we
  // have enumerated recently
  if (!mLastControllerEnumerationTime.IsNull()) {
    TimeDuration duration = TimeStamp::Now() - mLastControllerEnumerationTime;
    if (duration.ToMilliseconds() < gfxPrefs::VRControllerEnumerateInterval()) {
      return;
    }
||||||| merged common ancestors
void
VRManager::ScanForControllers()
{
  // We don't have to do this every frame, so check if we
  // have enumerated recently
  if (!mLastControllerEnumerationTime.IsNull()) {
    TimeDuration duration = TimeStamp::Now() - mLastControllerEnumerationTime;
    if (duration.ToMilliseconds() < gfxPrefs::VRControllerEnumerateInterval()) {
      return;
    }
=======
void VRManager::ExpireNavigationTransition() {
  if (mState != VRManagerState::Active) {
    return;
  }
  if (!mVRNavigationTransitionEnd.IsNull() &&
      TimeStamp::Now() > mVRNavigationTransitionEnd) {
    mBrowserState.navigationTransitionActive = false;
>>>>>>> upstream-releases
  }
}

void VRManager::UpdateHaptics(double aDeltaTime) {
  if (mState != VRManagerState::Active) {
    return;
  }
  bool bNeedPush = false;
  // Check for any haptic pulses that have ended and clear them
  for (size_t i = 0; i < mozilla::ArrayLength(mBrowserState.hapticState); i++) {
    const VRHapticState& state = mBrowserState.hapticState[i];
    if (state.inputFrameID == 0) {
      // Nothing in this slot
      continue;
    }
    mHapticPulseRemaining[i] -= aDeltaTime;
    if (mHapticPulseRemaining[i] <= 0.0f) {
      // The pulse has finished
      ClearHapticSlot(i);
      bNeedPush = true;
    }
  }
  if (bNeedPush) {
    PushState();
  }
}

void VRManager::ClearHapticSlot(size_t aSlot) {
  MOZ_ASSERT(aSlot < mozilla::ArrayLength(mBrowserState.hapticState));
  memset(&mBrowserState.hapticState[aSlot], 0, sizeof(VRHapticState));
  mHapticPulseRemaining[aSlot] = 0.0f;
  if (aSlot < mHapticPromises.Length() && mHapticPromises[aSlot]) {
    NotifyVibrateHapticCompleted(*(mHapticPromises[aSlot]));
    mHapticPromises[aSlot] = nullptr;
  }
}

void VRManager::ShutdownSubmitThread() {
  if (mSubmitThread) {
    mSubmitThread->Shutdown();
    mSubmitThread = nullptr;
  }
}

<<<<<<< HEAD
void VRManager::RemoveControllers() {
  for (uint32_t i = 0; i < mManagers.Length(); ++i) {
    mManagers[i]->RemoveControllers();
||||||| merged common ancestors
void
VRManager::RemoveControllers()
{
  for (uint32_t i = 0; i < mManagers.Length(); ++i) {
    mManagers[i]->RemoveControllers();
=======
void VRManager::StartPresentation() {
  if (mState != VRManagerState::Active) {
    return;
>>>>>>> upstream-releases
  }
  if (mBrowserState.presentationActive) {
    return;
  }
  mTelemetry.Clear();
  mTelemetry.mPresentationStart = TimeStamp::Now();

  // Indicate that we are ready to start immersive mode
  mBrowserState.presentationActive = true;
  mBrowserState.layerState[0].type = VRLayerType::LayerType_Stereo_Immersive;
  PushState();

  mDisplayInfo.mDisplayState.lastSubmittedFrameId = 0;
  if (mDisplayInfo.mDisplayState.reportsDroppedFrames) {
    mTelemetry.mLastDroppedFrameCount =
        mDisplayInfo.mDisplayState.droppedFrameCount;
  }

  mLastSubmittedFrameId = 0;
  mLastStartedFrame = 0;
}

<<<<<<< HEAD
void VRManager::CreateVRTestSystem() {
  if (mPuppetManager) {
    mPuppetManager->ClearTestDisplays();
||||||| merged common ancestors
void
VRManager::CreateVRTestSystem()
{
  if (mPuppetManager) {
    mPuppetManager->ClearTestDisplays();
=======
void VRManager::StopPresentation() {
  if (mState != VRManagerState::Active) {
    return;
  }
  if (!mBrowserState.presentationActive) {
>>>>>>> upstream-releases
    return;
  }

  // Indicate that we have stopped immersive mode
  mBrowserState.presentationActive = false;
  memset(mBrowserState.layerState, 0,
         sizeof(VRLayerState) * mozilla::ArrayLength(mBrowserState.layerState));

  PushState(true);

  Telemetry::HistogramID timeSpentID = Telemetry::HistogramCount;
  Telemetry::HistogramID droppedFramesID = Telemetry::HistogramCount;
  int viewIn = 0;

  if (mDisplayInfo.mDisplayState.eightCC ==
      GFX_VR_EIGHTCC('O', 'c', 'u', 'l', 'u', 's', ' ', 'D')) {
    // Oculus Desktop API
    timeSpentID = Telemetry::WEBVR_TIME_SPENT_VIEWING_IN_OCULUS;
    droppedFramesID = Telemetry::WEBVR_DROPPED_FRAMES_IN_OCULUS;
    viewIn = 1;
  } else if (mDisplayInfo.mDisplayState.eightCC ==
             GFX_VR_EIGHTCC('O', 'p', 'e', 'n', 'V', 'R', ' ', ' ')) {
    // OpenVR API
    timeSpentID = Telemetry::WEBVR_TIME_SPENT_VIEWING_IN_OPENVR;
    droppedFramesID = Telemetry::WEBVR_DROPPED_FRAMES_IN_OPENVR;
    viewIn = 2;
  }

  if (viewIn) {
    const TimeDuration duration =
        TimeStamp::Now() - mTelemetry.mPresentationStart;
    Telemetry::Accumulate(Telemetry::WEBVR_USERS_VIEW_IN, viewIn);
    Telemetry::Accumulate(timeSpentID, duration.ToMilliseconds());
    const uint32_t droppedFramesPerSec =
        (uint32_t)((double)(mDisplayInfo.mDisplayState.droppedFrameCount -
                            mTelemetry.mLastDroppedFrameCount) /
                   duration.ToSeconds());
    Telemetry::Accumulate(droppedFramesID, droppedFramesPerSec);
  }
}

<<<<<<< HEAD
VRSystemManagerPuppet* VRManager::GetPuppetManager() {
  MOZ_ASSERT(mPuppetManager);
  return mPuppetManager;
||||||| merged common ancestors
VRSystemManagerPuppet*
VRManager::GetPuppetManager()
{
  MOZ_ASSERT(mPuppetManager);
  return mPuppetManager;
=======
bool VRManager::IsPresenting() {
  if (mExternalShmem) {
    return mDisplayInfo.mPresentingGroups != 0;
  }
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
VRSystemManagerExternal* VRManager::GetExternalManager() {
  MOZ_ASSERT(mExternalManager);
  return mExternalManager;
||||||| merged common ancestors
VRSystemManagerExternal*
VRManager::GetExternalManager()
{
  MOZ_ASSERT(mExternalManager);
  return mExternalManager;
=======
void VRManager::SetGroupMask(uint32_t aGroupMask) {
  if (mState != VRManagerState::Active) {
    return;
  }
  mDisplayInfo.mGroupMask = aGroupMask;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <class T>
void VRManager::NotifyGamepadChange(uint32_t aIndex, const T& aInfo) {
  dom::GamepadChangeEventBody body(aInfo);
  dom::GamepadChangeEvent e(aIndex, dom::GamepadServiceType::VR, body);
||||||| merged common ancestors
template<class T>
void
VRManager::NotifyGamepadChange(uint32_t aIndex, const T& aInfo)
{
  dom::GamepadChangeEventBody body(aInfo);
  dom::GamepadChangeEvent e(aIndex, dom::GamepadServiceType::VR, body);
=======
void VRManager::SubmitFrame(VRLayerParent* aLayer,
                            const layers::SurfaceDescriptor& aTexture,
                            uint64_t aFrameId, const gfx::Rect& aLeftEyeRect,
                            const gfx::Rect& aRightEyeRect) {
  if (mState != VRManagerState::Active) {
    return;
  }
  MonitorAutoLock lock(mCurrentSubmitTaskMonitor);
  if ((mDisplayInfo.mGroupMask & aLayer->GetGroup()) == 0) {
    // Suppress layers hidden by the group mask
    return;
  }
>>>>>>> upstream-releases

  // Ensure that we only accept the first SubmitFrame call per RAF cycle.
  if (!mFrameStarted || aFrameId != mDisplayInfo.mFrameId) {
    return;
  }

<<<<<<< HEAD
void VRManager::VibrateHaptic(uint32_t aControllerIdx, uint32_t aHapticIndex,
                              double aIntensity, double aDuration,
                              const VRManagerPromise& aPromise)
||||||| merged common ancestors
void
VRManager::VibrateHaptic(uint32_t aControllerIdx, uint32_t aHapticIndex,
                         double aIntensity, double aDuration,
                         const VRManagerPromise& aPromise)
=======
  /**
   * Do not queue more submit frames until the last submitted frame is
   * already processed and the new WebGL texture is ready.
   */
  if (mLastSubmittedFrameId > 0 &&
      mLastSubmittedFrameId !=
          mDisplayInfo.mDisplayState.lastSubmittedFrameId) {
    mLastStartedFrame = 0;
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
{
  for (uint32_t i = 0; i < mManagers.Length(); ++i) {
    mManagers[i]->VibrateHaptic(aControllerIdx, aHapticIndex, aIntensity,
                                aDuration, aPromise);
||||||| merged common ancestors
{
  for (uint32_t i = 0; i < mManagers.Length(); ++i) {
    mManagers[i]->VibrateHaptic(aControllerIdx, aHapticIndex,
                                aIntensity, aDuration, aPromise);
=======
  mLastSubmittedFrameId = aFrameId;

  mFrameStarted = false;

  RefPtr<CancelableRunnable> task = NewCancelableRunnableMethod<
      StoreCopyPassByConstLRef<layers::SurfaceDescriptor>, uint64_t,
      StoreCopyPassByConstLRef<gfx::Rect>, StoreCopyPassByConstLRef<gfx::Rect>>(
      "gfx::VRManager::SubmitFrameInternal", this,
      &VRManager::SubmitFrameInternal, aTexture, aFrameId, aLeftEyeRect,
      aRightEyeRect);

  if (!mCurrentSubmitTask) {
    mCurrentSubmitTask = task;
#if !defined(MOZ_WIDGET_ANDROID)
    if (!mSubmitThread) {
      mSubmitThread = new VRThread(NS_LITERAL_CSTRING("VR_SubmitFrame"));
    }
    mSubmitThread->Start();
    mSubmitThread->PostTask(task.forget());
#else
    CompositorThreadHolder::Loop()->PostTask(task.forget());
#endif  // defined(MOZ_WIDGET_ANDROID)
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
void VRManager::StopVibrateHaptic(uint32_t aControllerIdx) {
  for (const auto& manager : mManagers) {
    manager->StopVibrateHaptic(aControllerIdx);
||||||| merged common ancestors
void
VRManager::StopVibrateHaptic(uint32_t aControllerIdx)
{
  for (const auto& manager: mManagers) {
    manager->StopVibrateHaptic(aControllerIdx);
=======
bool VRManager::SubmitFrame(const layers::SurfaceDescriptor& aTexture,
                            uint64_t aFrameId, const gfx::Rect& aLeftEyeRect,
                            const gfx::Rect& aRightEyeRect) {
  if (mState != VRManagerState::Active) {
    return false;
  }
#if defined(XP_WIN) || defined(XP_MACOSX) || defined(MOZ_WIDGET_ANDROID)
  MOZ_ASSERT(mBrowserState.layerState[0].type ==
             VRLayerType::LayerType_Stereo_Immersive);
  VRLayer_Stereo_Immersive& layer =
      mBrowserState.layerState[0].layer_stereo_immersive;

  switch (aTexture.type()) {
#  if defined(XP_WIN)
    case SurfaceDescriptor::TSurfaceDescriptorD3D10: {
      const SurfaceDescriptorD3D10& surf =
          aTexture.get_SurfaceDescriptorD3D10();
      layer.textureType =
          VRLayerTextureType::LayerTextureType_D3D10SurfaceDescriptor;
      layer.textureHandle = (void*)surf.handle();
      layer.textureSize.width = surf.size().width;
      layer.textureSize.height = surf.size().height;
    } break;
#  elif defined(XP_MACOSX)
    case SurfaceDescriptor::TSurfaceDescriptorMacIOSurface: {
      // MacIOSurface ptr can't be fetched or used at different threads.
      // Both of fetching and using this MacIOSurface are at the VRService
      // thread.
      const auto& desc = aTexture.get_SurfaceDescriptorMacIOSurface();
      layer.textureType = VRLayerTextureType::LayerTextureType_MacIOSurface;
      layer.textureHandle = desc.surfaceId();
      RefPtr<MacIOSurface> surf =
          MacIOSurface::LookupSurface(desc.surfaceId(), desc.scaleFactor(),
                                      !desc.isOpaque(), desc.yUVColorSpace());
      if (surf) {
        layer.textureSize.width = surf->GetDevicePixelWidth();
        layer.textureSize.height = surf->GetDevicePixelHeight();
      }
    } break;
#  elif defined(MOZ_WIDGET_ANDROID)
    case SurfaceDescriptor::TSurfaceTextureDescriptor: {
      const SurfaceTextureDescriptor& desc =
          aTexture.get_SurfaceTextureDescriptor();
      java::GeckoSurfaceTexture::LocalRef surfaceTexture =
          java::GeckoSurfaceTexture::Lookup(desc.handle());
      if (!surfaceTexture) {
        NS_WARNING("VRManager::SubmitFrame failed to get a SurfaceTexture");
        return false;
      }
      layer.textureType =
          VRLayerTextureType::LayerTextureType_GeckoSurfaceTexture;
      layer.textureHandle = desc.handle();
      layer.textureSize.width = desc.size().width;
      layer.textureSize.height = desc.size().height;
    } break;
#  endif
    default: {
      MOZ_ASSERT(false);
      return false;
    }
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
void VRManager::NotifyVibrateHapticCompleted(const VRManagerPromise& aPromise) {
  aPromise.mParent->SendReplyGamepadVibrateHaptic(aPromise.mPromiseID);
}
||||||| merged common ancestors
void
VRManager::NotifyVibrateHapticCompleted(const VRManagerPromise& aPromise)
{
  aPromise.mParent->SendReplyGamepadVibrateHaptic(aPromise.mPromiseID);
}
=======
  layer.frameId = aFrameId;
  layer.inputFrameId =
      mDisplayInfo.mLastSensorState[mDisplayInfo.mFrameId % kVRMaxLatencyFrames]
          .inputFrameID;
>>>>>>> upstream-releases

<<<<<<< HEAD
void VRManager::DispatchSubmitFrameResult(
    uint32_t aDisplayID, const VRSubmitFrameResultInfo& aResult) {
  for (auto iter = mVRManagerParents.Iter(); !iter.Done(); iter.Next()) {
    Unused << iter.Get()->GetKey()->SendDispatchSubmitFrameResult(aDisplayID,
                                                                  aResult);
||||||| merged common ancestors
void
VRManager::DispatchSubmitFrameResult(uint32_t aDisplayID, const VRSubmitFrameResultInfo& aResult)
{
  for (auto iter = mVRManagerParents.Iter(); !iter.Done(); iter.Next()) {
    Unused << iter.Get()->GetKey()->SendDispatchSubmitFrameResult(aDisplayID, aResult);
=======
  layer.leftEyeRect.x = aLeftEyeRect.x;
  layer.leftEyeRect.y = aLeftEyeRect.y;
  layer.leftEyeRect.width = aLeftEyeRect.width;
  layer.leftEyeRect.height = aLeftEyeRect.height;
  layer.rightEyeRect.x = aRightEyeRect.x;
  layer.rightEyeRect.y = aRightEyeRect.y;
  layer.rightEyeRect.width = aRightEyeRect.width;
  layer.rightEyeRect.height = aRightEyeRect.height;

  PushState(true);

  PullState([&]() {
    return (mDisplayInfo.mDisplayState.lastSubmittedFrameId >= aFrameId) ||
           mDisplayInfo.mDisplayState.suppressFrames ||
           !mDisplayInfo.mDisplayState.isConnected;
  });

  if (mDisplayInfo.mDisplayState.suppressFrames ||
      !mDisplayInfo.mDisplayState.isConnected) {
    // External implementation wants to supress frames, service has shut
    // down or hardware has been disconnected.
    return false;
>>>>>>> upstream-releases
  }

  return mDisplayInfo.mDisplayState.lastSubmittedFrameSuccessful;
#else
  MOZ_ASSERT(false);  // Not implmented for this platform
  return false;
#endif
}

<<<<<<< HEAD
void VRManager::StartVRNavigation(const uint32_t& aDisplayID) {
  RefPtr<VRDisplayHost> display = GetDisplay(aDisplayID);
  if (display) {
    display->StartVRNavigation();
||||||| merged common ancestors
void
VRManager::StartVRNavigation(const uint32_t& aDisplayID)
{
  RefPtr<VRDisplayHost> display = GetDisplay(aDisplayID);
  if (display) {
    display->StartVRNavigation();
=======
void VRManager::SubmitFrameInternal(const layers::SurfaceDescriptor& aTexture,
                                    uint64_t aFrameId,
                                    const gfx::Rect& aLeftEyeRect,
                                    const gfx::Rect& aRightEyeRect) {
#if !defined(MOZ_WIDGET_ANDROID)
  MOZ_ASSERT(mSubmitThread->GetThread() == NS_GetCurrentThread());
#endif  // !defined(MOZ_WIDGET_ANDROID)
  AUTO_PROFILER_TRACING("VR", "SubmitFrameAtVRDisplayExternal", OTHER);

  {  // scope lock
    MonitorAutoLock lock(mCurrentSubmitTaskMonitor);

    if (!SubmitFrame(aTexture, aFrameId, aLeftEyeRect, aRightEyeRect)) {
      mCurrentSubmitTask = nullptr;
      return;
    }
    mCurrentSubmitTask = nullptr;
>>>>>>> upstream-releases
  }

#if defined(XP_WIN) || defined(XP_MACOSX) || defined(MOZ_WIDGET_ANDROID)

  /**
   * Trigger the next VSync immediately after we are successfully
   * submitting frames.  As SubmitFrame is responsible for throttling
   * the render loop, if we don't successfully call it, we shouldn't trigger
   * StartFrame immediately, as it will run unbounded.
   * If StartFrame is not called here due to SubmitFrame failing, the
   * fallback "watchdog" code in VRManager::NotifyVSync() will cause
   * frames to continue at a lower refresh rate until frame submission
   * succeeds again.
   */
  MessageLoop* loop = CompositorThreadHolder::Loop();

  loop->PostTask(NewRunnableMethod("gfx::VRManager::StartFrame", this,
                                   &VRManager::StartFrame));
#endif
}

<<<<<<< HEAD
void VRManager::StopVRNavigation(const uint32_t& aDisplayID,
                                 const TimeDuration& aTimeout) {
  RefPtr<VRDisplayHost> display = GetDisplay(aDisplayID);
  if (display) {
    display->StopVRNavigation(aTimeout);
||||||| merged common ancestors
void
VRManager::StopVRNavigation(const uint32_t& aDisplayID, const TimeDuration& aTimeout)
{
  RefPtr<VRDisplayHost> display = GetDisplay(aDisplayID);
  if (display) {
    display->StopVRNavigation(aTimeout);
=======
void VRManager::CancelCurrentSubmitTask() {
  MonitorAutoLock lock(mCurrentSubmitTaskMonitor);
  if (mCurrentSubmitTask) {
    mCurrentSubmitTask->Cancel();
    mCurrentSubmitTask = nullptr;
>>>>>>> upstream-releases
  }
}

}  // namespace gfx
}  // namespace mozilla
