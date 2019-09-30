/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ProcessPriorityManager.h"
#include "mozilla/ClearOnShutdown.h"
#include "mozilla/dom/ContentParent.h"
#include "mozilla/dom/Element.h"
#include "mozilla/dom/BrowserHost.h"
#include "mozilla/dom/BrowserParent.h"
#include "mozilla/Hal.h"
#include "mozilla/IntegerPrintfMacros.h"
#include "mozilla/Preferences.h"
#include "mozilla/Services.h"
#include "mozilla/Telemetry.h"
#include "mozilla/Unused.h"
#include "mozilla/Logging.h"
#include "nsPrintfCString.h"
#include "nsXULAppAPI.h"
#include "nsFrameLoader.h"
#include "nsINamed.h"
#include "nsIObserverService.h"
#include "StaticPtr.h"
#include "nsIMozBrowserFrame.h"
#include "nsIObserver.h"
#include "nsITimer.h"
#include "nsIPropertyBag2.h"
#include "nsComponentManagerUtils.h"
#include "nsCRT.h"
#include "nsTHashtable.h"
#include "nsQueryObject.h"

using namespace mozilla;
using namespace mozilla::dom;
using namespace mozilla::hal;

#ifdef XP_WIN
#  include <process.h>
#  define getpid _getpid
#else
#  include <unistd.h>
#endif

#ifdef LOG
#  undef LOG
#endif

// Use LOGP inside a ParticularProcessPriorityManager method; use LOG
// everywhere else.  LOGP prints out information about the particular process
// priority manager.
//
// (Wow, our logging story is a huge mess.)

// #define ENABLE_LOGGING 1

#if defined(ANDROID) && defined(ENABLE_LOGGING)
<<<<<<< HEAD
#include <android/log.h>
#define LOG(fmt, ...)                                                        \
  __android_log_print(ANDROID_LOG_INFO, "Gecko:ProcessPriorityManager", fmt, \
                      ##__VA_ARGS__)
#define LOGP(fmt, ...)                                                         \
  __android_log_print(ANDROID_LOG_INFO, "Gecko:ProcessPriorityManager",        \
                      "[%schild-id=%" PRIu64 ", pid=%d] " fmt,                 \
                      NameWithComma().get(), static_cast<uint64_t>(ChildID()), \
                      Pid(), ##__VA_ARGS__)
||||||| merged common ancestors
#  include <android/log.h>
#  define LOG(fmt, ...) \
     __android_log_print(ANDROID_LOG_INFO, \
       "Gecko:ProcessPriorityManager", \
       fmt, ## __VA_ARGS__)
#  define LOGP(fmt, ...) \
    __android_log_print(ANDROID_LOG_INFO, \
      "Gecko:ProcessPriorityManager", \
      "[%schild-id=%" PRIu64 ", pid=%d] " fmt, \
      NameWithComma().get(), \
      static_cast<uint64_t>(ChildID()), Pid(), ## __VA_ARGS__)
=======
#  include <android/log.h>
#  define LOG(fmt, ...)                                                        \
    __android_log_print(ANDROID_LOG_INFO, "Gecko:ProcessPriorityManager", fmt, \
                        ##__VA_ARGS__)
#  define LOGP(fmt, ...)                                                \
    __android_log_print(                                                \
        ANDROID_LOG_INFO, "Gecko:ProcessPriorityManager",               \
        "[%schild-id=%" PRIu64 ", pid=%d] " fmt, NameWithComma().get(), \
        static_cast<uint64_t>(ChildID()), Pid(), ##__VA_ARGS__)
>>>>>>> upstream-releases

#elif defined(ENABLE_LOGGING)
<<<<<<< HEAD
#define LOG(fmt, ...) \
  printf("ProcessPriorityManager - " fmt "\n", ##__VA_ARGS__)
#define LOGP(fmt, ...)                                                         \
  printf("ProcessPriorityManager[%schild-id=%" PRIu64 ", pid=%d] - " fmt "\n", \
         NameWithComma().get(), static_cast<uint64_t>(ChildID()), Pid(),       \
         ##__VA_ARGS__)
||||||| merged common ancestors
#  define LOG(fmt, ...) \
     printf("ProcessPriorityManager - " fmt "\n", ##__VA_ARGS__)
#  define LOGP(fmt, ...) \
     printf("ProcessPriorityManager[%schild-id=%" PRIu64 ", pid=%d] - " \
       fmt "\n", \
       NameWithComma().get(), \
       static_cast<uint64_t>(ChildID()), Pid(), ##__VA_ARGS__)
=======
#  define LOG(fmt, ...) \
    printf("ProcessPriorityManager - " fmt "\n", ##__VA_ARGS__)
#  define LOGP(fmt, ...)                                                   \
    printf("ProcessPriorityManager[%schild-id=%" PRIu64 ", pid=%d] - " fmt \
           "\n",                                                           \
           NameWithComma().get(), static_cast<uint64_t>(ChildID()), Pid(), \
           ##__VA_ARGS__)
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
static LogModule* GetPPMLog() {
  static LazyLogModule sLog("ProcessPriorityManager");
  return sLog;
}
#define LOG(fmt, ...)                   \
  MOZ_LOG(GetPPMLog(), LogLevel::Debug, \
          ("ProcessPriorityManager - " fmt, ##__VA_ARGS__))
#define LOGP(fmt, ...)                                                      \
  MOZ_LOG(GetPPMLog(), LogLevel::Debug,                                     \
          ("ProcessPriorityManager[%schild-id=%" PRIu64 ", pid=%d] - " fmt, \
           NameWithComma().get(), static_cast<uint64_t>(ChildID()), Pid(),  \
           ##__VA_ARGS__))
||||||| merged common ancestors
  static LogModule*
  GetPPMLog()
  {
    static LazyLogModule sLog("ProcessPriorityManager");
    return sLog;
  }
#  define LOG(fmt, ...) \
     MOZ_LOG(GetPPMLog(), LogLevel::Debug, \
            ("ProcessPriorityManager - " fmt, ##__VA_ARGS__))
#  define LOGP(fmt, ...) \
     MOZ_LOG(GetPPMLog(), LogLevel::Debug, \
            ("ProcessPriorityManager[%schild-id=%" PRIu64 ", pid=%d] - " fmt, \
            NameWithComma().get(), \
            static_cast<uint64_t>(ChildID()), Pid(), ##__VA_ARGS__))
=======
static LogModule* GetPPMLog() {
  static LazyLogModule sLog("ProcessPriorityManager");
  return sLog;
}
#  define LOG(fmt, ...)                   \
    MOZ_LOG(GetPPMLog(), LogLevel::Debug, \
            ("ProcessPriorityManager - " fmt, ##__VA_ARGS__))
#  define LOGP(fmt, ...)                                                      \
    MOZ_LOG(GetPPMLog(), LogLevel::Debug,                                     \
            ("ProcessPriorityManager[%schild-id=%" PRIu64 ", pid=%d] - " fmt, \
             NameWithComma().get(), static_cast<uint64_t>(ChildID()), Pid(),  \
             ##__VA_ARGS__))
>>>>>>> upstream-releases
#endif

namespace {

class ParticularProcessPriorityManager;

/**
 * This singleton class does the work to implement the process priority manager
 * in the main process.  This class may not be used in child processes.  (You
 * can call StaticInit, but it won't do anything, and GetSingleton() will
 * return null.)
 *
 * ProcessPriorityManager::CurrentProcessIsForeground() and
 * ProcessPriorityManager::AnyProcessHasHighPriority() which can be called in
 * any process, are handled separately, by the ProcessPriorityManagerChild
 * class.
 */
<<<<<<< HEAD
class ProcessPriorityManagerImpl final : public nsIObserver,
                                         public WakeLockObserver,
                                         public nsSupportsWeakReference {
 public:
||||||| merged common ancestors
class ProcessPriorityManagerImpl final
  : public nsIObserver
  , public WakeLockObserver
  , public nsSupportsWeakReference
{
public:
=======
class ProcessPriorityManagerImpl final : public nsIObserver,
                                         public nsSupportsWeakReference {
 public:
>>>>>>> upstream-releases
  /**
   * If we're in the main process, get the ProcessPriorityManagerImpl
   * singleton.  If we're in a child process, return null.
   */
  static ProcessPriorityManagerImpl* GetSingleton();

  static void StaticInit();
  static bool PrefsEnabled();
  static bool TestMode();

  NS_DECL_ISUPPORTS
  NS_DECL_NSIOBSERVER

  /**
   * This function implements ProcessPriorityManager::SetProcessPriority.
   */
  void SetProcessPriority(ContentParent* aContentParent,
                          ProcessPriority aPriority);

  /**
   * If a magic testing-only pref is set, notify the observer service on the
   * given topic with the given data.  This is used for testing
   */
  void FireTestOnlyObserverNotification(
      const char* aTopic, const nsACString& aData = EmptyCString());

  /**
   * This must be called by a ParticularProcessPriorityManager when it changes
   * its priority.
   */
  void NotifyProcessPriorityChanged(
<<<<<<< HEAD
      ParticularProcessPriorityManager* aParticularManager,
      hal::ProcessPriority aOldPriority);

  /**
   * Implements WakeLockObserver, used to monitor wake lock changes in the
   * main process.
   */
  virtual void Notify(const WakeLockInformation& aInfo) override;
||||||| merged common ancestors
    ParticularProcessPriorityManager* aParticularManager,
    hal::ProcessPriority aOldPriority);

  /**
   * Implements WakeLockObserver, used to monitor wake lock changes in the
   * main process.
   */
  virtual void Notify(const WakeLockInformation& aInfo) override;
=======
      ParticularProcessPriorityManager* aParticularManager,
      hal::ProcessPriority aOldPriority);
>>>>>>> upstream-releases

  void TabActivityChanged(BrowserParent* aBrowserParent, bool aIsActive);

<<<<<<< HEAD
  /**
   * Call ShutDown before destroying the ProcessPriorityManager because
   * WakeLockObserver hols a strong reference to it.
   */
  void ShutDown();

 private:
||||||| merged common ancestors
  /**
   * Call ShutDown before destroying the ProcessPriorityManager because
   * WakeLockObserver hols a strong reference to it.
   */
  void ShutDown();

private:
=======
 private:
>>>>>>> upstream-releases
  static bool sPrefsEnabled;
  static bool sRemoteTabsDisabled;
  static bool sTestMode;
  static bool sPrefListenersRegistered;
  static bool sInitialized;
  static StaticRefPtr<ProcessPriorityManagerImpl> sSingleton;

  static void PrefChangedCallback(const char* aPref, void* aClosure);

  ProcessPriorityManagerImpl();
  ~ProcessPriorityManagerImpl();
  DISALLOW_EVIL_CONSTRUCTORS(ProcessPriorityManagerImpl);

  void Init();

  already_AddRefed<ParticularProcessPriorityManager>
  GetParticularProcessPriorityManager(ContentParent* aContentParent);

  void ObserveContentParentCreated(nsISupports* aContentParent);
  void ObserveContentParentDestroyed(nsISupports* aSubject);

  nsDataHashtable<nsUint64HashKey, RefPtr<ParticularProcessPriorityManager> >
<<<<<<< HEAD
      mParticularManagers;

  /** True if the main process is holding a high-priority wakelock */
  bool mHighPriority;
||||||| merged common ancestors
    mParticularManagers;

  /** True if the main process is holding a high-priority wakelock */
  bool mHighPriority;
=======
      mParticularManagers;
>>>>>>> upstream-releases

  /** Contains the PIDs of child processes holding high-priority wakelocks */
  nsTHashtable<nsUint64HashKey> mHighPriorityChildIDs;
};

/**
 * This singleton class implements the parts of the process priority manager
 * that are available from all processes.
 */
class ProcessPriorityManagerChild final : public nsIObserver {
 public:
  static void StaticInit();
  static ProcessPriorityManagerChild* Singleton();

  NS_DECL_ISUPPORTS
  NS_DECL_NSIOBSERVER

  bool CurrentProcessIsForeground();

 private:
  static StaticRefPtr<ProcessPriorityManagerChild> sSingleton;

  ProcessPriorityManagerChild();
  ~ProcessPriorityManagerChild() {}
  DISALLOW_EVIL_CONSTRUCTORS(ProcessPriorityManagerChild);

  void Init();

  hal::ProcessPriority mCachedPriority;
};

/**
 * This class manages the priority of one particular process.  It is
 * main-process only.
 */
class ParticularProcessPriorityManager final : public WakeLockObserver,
                                               public nsIObserver,
                                               public nsITimerCallback,
                                               public nsINamed,
                                               public nsSupportsWeakReference {
  ~ParticularProcessPriorityManager();

 public:
  explicit ParticularProcessPriorityManager(ContentParent* aContentParent);

  NS_DECL_ISUPPORTS
  NS_DECL_NSIOBSERVER
  NS_DECL_NSITIMERCALLBACK

  virtual void Notify(const WakeLockInformation& aInfo) override;
  static void StaticInit();
  void Init();

  int32_t Pid() const;
  uint64_t ChildID() const;

  /**
   * Used in logging, this method returns the ContentParent's name followed by
   * ", ".  If we can't get the ContentParent's name for some reason, it
   * returns an empty string.
   *
   * The reference returned here is guaranteed to be live until the next call
   * to NameWithComma() or until the ParticularProcessPriorityManager is
   * destroyed, whichever comes first.
   */
  const nsAutoCString& NameWithComma();

  void OnRemoteBrowserFrameShown(nsISupports* aSubject);
  void OnBrowserParentDestroyed(nsISupports* aSubject);

  ProcessPriority CurrentPriority();
  ProcessPriority ComputePriority();

  enum TimeoutPref {
    BACKGROUND_PERCEIVABLE_GRACE_PERIOD,
    BACKGROUND_GRACE_PERIOD,
  };

  void ScheduleResetPriority(TimeoutPref aTimeoutPref);
  void ResetPriority();
  void ResetPriorityNow();
  void SetPriorityNow(ProcessPriority aPriority);

  void TabActivityChanged(BrowserParent* aBrowserParent, bool aIsActive);

  void ShutDown();

  NS_IMETHOD GetName(nsACString& aName) override {
    aName.AssignLiteral("ParticularProcessPriorityManager");
    return NS_OK;
  }

 private:
  static uint32_t sBackgroundPerceivableGracePeriodMS;
  static uint32_t sBackgroundGracePeriodMS;

  void FireTestOnlyObserverNotification(
      const char* aTopic, const nsACString& aData = EmptyCString());

<<<<<<< HEAD
  void FireTestOnlyObserverNotification(const char* aTopic,
                                        const char* aData = nullptr);
||||||| merged common ancestors
  void FireTestOnlyObserverNotification(
    const char* aTopic,
    const char* aData = nullptr);
=======
  void FireTestOnlyObserverNotification(const char* aTopic,
                                        const char* aData = nullptr);

  bool IsHoldingWakeLock(const nsAString& aTopic);
>>>>>>> upstream-releases

  ContentParent* mContentParent;
  uint64_t mChildID;
  ProcessPriority mPriority;
  bool mHoldsCPUWakeLock;
  bool mHoldsHighPriorityWakeLock;
  bool mHoldsPlayingAudioWakeLock;
  bool mHoldsPlayingVideoWakeLock;

  /**
   * Used to implement NameWithComma().
   */
  nsAutoCString mNameWithComma;

  nsCOMPtr<nsITimer> mResetPriorityTimer;

  // This hashtable contains the list of active TabId for this process.
  nsTHashtable<nsUint64HashKey> mActiveBrowserParents;
};

<<<<<<< HEAD
/* static */ bool ProcessPriorityManagerImpl::sInitialized = false;
/* static */ bool ProcessPriorityManagerImpl::sPrefsEnabled = false;
/* static */ bool ProcessPriorityManagerImpl::sRemoteTabsDisabled = true;
/* static */ bool ProcessPriorityManagerImpl::sTestMode = false;
/* static */ bool ProcessPriorityManagerImpl::sPrefListenersRegistered = false;
/* static */ StaticRefPtr<ProcessPriorityManagerImpl>
    ProcessPriorityManagerImpl::sSingleton;
/* static */ uint32_t
    ParticularProcessPriorityManager::sBackgroundPerceivableGracePeriodMS = 0;
/* static */ uint32_t
    ParticularProcessPriorityManager::sBackgroundGracePeriodMS = 0;

NS_IMPL_ISUPPORTS(ProcessPriorityManagerImpl, nsIObserver,
||||||| merged common ancestors
/* static */ bool ProcessPriorityManagerImpl::sInitialized = false;
/* static */ bool ProcessPriorityManagerImpl::sPrefsEnabled = false;
/* static */ bool ProcessPriorityManagerImpl::sRemoteTabsDisabled = true;
/* static */ bool ProcessPriorityManagerImpl::sTestMode = false;
/* static */ bool ProcessPriorityManagerImpl::sPrefListenersRegistered = false;
/* static */ StaticRefPtr<ProcessPriorityManagerImpl>
  ProcessPriorityManagerImpl::sSingleton;
/* static */ uint32_t ParticularProcessPriorityManager::sBackgroundPerceivableGracePeriodMS = 0;
/* static */ uint32_t ParticularProcessPriorityManager::sBackgroundGracePeriodMS = 0;

NS_IMPL_ISUPPORTS(ProcessPriorityManagerImpl,
                  nsIObserver,
=======
/* static */
bool ProcessPriorityManagerImpl::sInitialized = false;
/* static */
bool ProcessPriorityManagerImpl::sPrefsEnabled = false;
/* static */
bool ProcessPriorityManagerImpl::sRemoteTabsDisabled = true;
/* static */
bool ProcessPriorityManagerImpl::sTestMode = false;
/* static */
bool ProcessPriorityManagerImpl::sPrefListenersRegistered = false;
/* static */
StaticRefPtr<ProcessPriorityManagerImpl> ProcessPriorityManagerImpl::sSingleton;
/* static */
uint32_t ParticularProcessPriorityManager::sBackgroundPerceivableGracePeriodMS =
    0;
/* static */
uint32_t ParticularProcessPriorityManager::sBackgroundGracePeriodMS = 0;

NS_IMPL_ISUPPORTS(ProcessPriorityManagerImpl, nsIObserver,
>>>>>>> upstream-releases
                  nsISupportsWeakReference);

<<<<<<< HEAD
/* static */ void ProcessPriorityManagerImpl::PrefChangedCallback(
    const char* aPref, void* aClosure) {
||||||| merged common ancestors
/* static */ void
ProcessPriorityManagerImpl::PrefChangedCallback(const char* aPref,
                                                void* aClosure)
{
=======
/* static */
void ProcessPriorityManagerImpl::PrefChangedCallback(const char* aPref,
                                                     void* aClosure) {
>>>>>>> upstream-releases
  StaticInit();
  if (!PrefsEnabled() && sSingleton) {
    sSingleton = nullptr;
    sInitialized = false;
  }
}

<<<<<<< HEAD
/* static */ bool ProcessPriorityManagerImpl::PrefsEnabled() {
  return sPrefsEnabled && hal::SetProcessPrioritySupported() &&
         !sRemoteTabsDisabled;
||||||| merged common ancestors
/* static */ bool
ProcessPriorityManagerImpl::PrefsEnabled()
{
  return sPrefsEnabled && hal::SetProcessPrioritySupported() && !sRemoteTabsDisabled;
=======
/* static */
bool ProcessPriorityManagerImpl::PrefsEnabled() {
  return sPrefsEnabled && hal::SetProcessPrioritySupported() &&
         !sRemoteTabsDisabled;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ bool ProcessPriorityManagerImpl::TestMode() { return sTestMode; }
||||||| merged common ancestors
/* static */ bool
ProcessPriorityManagerImpl::TestMode()
{
  return sTestMode;
}
=======
/* static */
bool ProcessPriorityManagerImpl::TestMode() { return sTestMode; }
>>>>>>> upstream-releases

<<<<<<< HEAD
/* static */ void ProcessPriorityManagerImpl::StaticInit() {
||||||| merged common ancestors
/* static */ void
ProcessPriorityManagerImpl::StaticInit()
{
=======
/* static */
void ProcessPriorityManagerImpl::StaticInit() {
>>>>>>> upstream-releases
  if (sInitialized) {
    return;
  }

  // The process priority manager is main-process only.
  if (!XRE_IsParentProcess()) {
    sInitialized = true;
    return;
  }

  if (!sPrefListenersRegistered) {
    Preferences::AddBoolVarCache(&sPrefsEnabled,
                                 "dom.ipc.processPriorityManager.enabled");
    Preferences::AddBoolVarCache(&sRemoteTabsDisabled, "dom.ipc.tabs.disabled");
    Preferences::AddBoolVarCache(&sTestMode,
                                 "dom.ipc.processPriorityManager.testMode");
  }

  // If IPC tabs aren't enabled at startup, don't bother with any of this.
  if (!PrefsEnabled()) {
    LOG("InitProcessPriorityManager bailing due to prefs.");

    // Run StaticInit() again if the prefs change.  We don't expect this to
    // happen in normal operation, but it happens during testing.
    if (!sPrefListenersRegistered) {
      sPrefListenersRegistered = true;
      Preferences::RegisterCallback(PrefChangedCallback,
                                    "dom.ipc.processPriorityManager.enabled");
      Preferences::RegisterCallback(PrefChangedCallback,
                                    "dom.ipc.tabs.disabled");
    }
    return;
  }

  sInitialized = true;

  sSingleton = new ProcessPriorityManagerImpl();
  sSingleton->Init();
  ClearOnShutdown(&sSingleton);
}

<<<<<<< HEAD
/* static */ ProcessPriorityManagerImpl*
ProcessPriorityManagerImpl::GetSingleton() {
||||||| merged common ancestors
/* static */ ProcessPriorityManagerImpl*
ProcessPriorityManagerImpl::GetSingleton()
{
=======
/* static */
ProcessPriorityManagerImpl* ProcessPriorityManagerImpl::GetSingleton() {
>>>>>>> upstream-releases
  if (!sSingleton) {
    StaticInit();
  }

  return sSingleton;
}

<<<<<<< HEAD
ProcessPriorityManagerImpl::ProcessPriorityManagerImpl()
    : mHighPriority(false) {
||||||| merged common ancestors
ProcessPriorityManagerImpl::ProcessPriorityManagerImpl()
  : mHighPriority(false)
{
=======
ProcessPriorityManagerImpl::ProcessPriorityManagerImpl() {
>>>>>>> upstream-releases
  MOZ_ASSERT(XRE_IsParentProcess());
}

<<<<<<< HEAD
ProcessPriorityManagerImpl::~ProcessPriorityManagerImpl() { ShutDown(); }

void ProcessPriorityManagerImpl::ShutDown() {
  UnregisterWakeLockObserver(this);
}
||||||| merged common ancestors
ProcessPriorityManagerImpl::~ProcessPriorityManagerImpl()
{
  ShutDown();
}

void
ProcessPriorityManagerImpl::ShutDown()
{
  UnregisterWakeLockObserver(this);
}
=======
ProcessPriorityManagerImpl::~ProcessPriorityManagerImpl() = default;
>>>>>>> upstream-releases

void ProcessPriorityManagerImpl::Init() {
  LOG("Starting up.  This is the master process.");

  // The master process's priority never changes; set it here and then forget
  // about it.  We'll manage only subprocesses' priorities using the process
  // priority manager.
  hal::SetProcessPriority(getpid(), PROCESS_PRIORITY_MASTER);

  nsCOMPtr<nsIObserverService> os = services::GetObserverService();
  if (os) {
    os->AddObserver(this, "ipc:content-created", /* ownsWeak */ true);
    os->AddObserver(this, "ipc:content-shutdown", /* ownsWeak */ true);
  }
}

NS_IMETHODIMP
ProcessPriorityManagerImpl::Observe(nsISupports* aSubject, const char* aTopic,
                                    const char16_t* aData) {
  nsDependentCString topic(aTopic);
  if (topic.EqualsLiteral("ipc:content-created")) {
    ObserveContentParentCreated(aSubject);
  } else if (topic.EqualsLiteral("ipc:content-shutdown")) {
    ObserveContentParentDestroyed(aSubject);
  } else {
    MOZ_ASSERT(false);
  }

  return NS_OK;
}

already_AddRefed<ParticularProcessPriorityManager>
ProcessPriorityManagerImpl::GetParticularProcessPriorityManager(
    ContentParent* aContentParent) {
  uint64_t cpId = aContentParent->ChildID();
  auto entry = mParticularManagers.LookupForAdd(cpId);
  RefPtr<ParticularProcessPriorityManager> pppm =
      entry.OrInsert([aContentParent]() {
        return new ParticularProcessPriorityManager(aContentParent);
      });

  if (!entry) {
    // We created a new entry.
    pppm->Init();
    FireTestOnlyObserverNotification("process-created",
                                     nsPrintfCString("%" PRIu64, cpId));
  }

  return pppm.forget();
}

void ProcessPriorityManagerImpl::SetProcessPriority(
    ContentParent* aContentParent, ProcessPriority aPriority) {
  MOZ_ASSERT(aContentParent);
  RefPtr<ParticularProcessPriorityManager> pppm =
      GetParticularProcessPriorityManager(aContentParent);
  if (pppm) {
    pppm->SetPriorityNow(aPriority);
  }
}

void ProcessPriorityManagerImpl::ObserveContentParentCreated(
    nsISupports* aContentParent) {
  // Do nothing; it's sufficient to get the PPPM.  But assign to nsRefPtr so we
  // don't leak the already_AddRefed object.
  RefPtr<ContentParent> cp = do_QueryObject(aContentParent);
  RefPtr<ParticularProcessPriorityManager> pppm =
<<<<<<< HEAD
      GetParticularProcessPriorityManager(cp->AsContentParent());
||||||| merged common ancestors
    GetParticularProcessPriorityManager(cp->AsContentParent());
=======
      GetParticularProcessPriorityManager(cp);
>>>>>>> upstream-releases
}

void ProcessPriorityManagerImpl::ObserveContentParentDestroyed(
    nsISupports* aSubject) {
  nsCOMPtr<nsIPropertyBag2> props = do_QueryInterface(aSubject);
  NS_ENSURE_TRUE_VOID(props);

  uint64_t childID = CONTENT_PROCESS_ID_UNKNOWN;
  props->GetPropertyAsUint64(NS_LITERAL_STRING("childID"), &childID);
  NS_ENSURE_TRUE_VOID(childID != CONTENT_PROCESS_ID_UNKNOWN);

  if (auto entry = mParticularManagers.Lookup(childID)) {
    entry.Data()->ShutDown();
    mHighPriorityChildIDs.RemoveEntry(childID);
    entry.Remove();
  }
}

void ProcessPriorityManagerImpl::NotifyProcessPriorityChanged(
    ParticularProcessPriorityManager* aParticularManager,
    ProcessPriority aOldPriority) {
  ProcessPriority newPriority = aParticularManager->CurrentPriority();

  if (newPriority >= PROCESS_PRIORITY_FOREGROUND_HIGH &&
      aOldPriority < PROCESS_PRIORITY_FOREGROUND_HIGH) {
    mHighPriorityChildIDs.PutEntry(aParticularManager->ChildID());
  } else if (newPriority < PROCESS_PRIORITY_FOREGROUND_HIGH &&
             aOldPriority >= PROCESS_PRIORITY_FOREGROUND_HIGH) {
    mHighPriorityChildIDs.RemoveEntry(aParticularManager->ChildID());
  }
}

<<<<<<< HEAD
/* virtual */ void ProcessPriorityManagerImpl::Notify(
    const WakeLockInformation& aInfo) {
  /* The main process always has an ID of 0, if it is present in the wake-lock
   * information then we explicitly requested a high-priority wake-lock for the
   * main process. */
  if (aInfo.topic().EqualsLiteral("high-priority")) {
    if (aInfo.lockingProcesses().Contains((uint64_t)0)) {
      mHighPriority = true;
    } else {
      mHighPriority = false;
    }

    LOG("Got wake lock changed event. "
        "Now mHighPriorityParent = %d\n",
        mHighPriority);
  }
}

void ProcessPriorityManagerImpl::TabActivityChanged(TabParent* aTabParent,
                                                    bool aIsActive) {
  ContentParent* cp = aTabParent->Manager()->AsContentParent();
||||||| merged common ancestors
/* virtual */ void
ProcessPriorityManagerImpl::Notify(const WakeLockInformation& aInfo)
{
  /* The main process always has an ID of 0, if it is present in the wake-lock
   * information then we explicitly requested a high-priority wake-lock for the
   * main process. */
  if (aInfo.topic().EqualsLiteral("high-priority")) {
    if (aInfo.lockingProcesses().Contains((uint64_t)0)) {
      mHighPriority = true;
    } else {
      mHighPriority = false;
    }

    LOG("Got wake lock changed event. "
        "Now mHighPriorityParent = %d\n", mHighPriority);
  }
}

void
ProcessPriorityManagerImpl::TabActivityChanged(TabParent* aTabParent,
                                               bool aIsActive)
{
  ContentParent* cp = aTabParent->Manager()->AsContentParent();
=======
void ProcessPriorityManagerImpl::TabActivityChanged(
    BrowserParent* aBrowserParent, bool aIsActive) {
>>>>>>> upstream-releases
  RefPtr<ParticularProcessPriorityManager> pppm =
<<<<<<< HEAD
      GetParticularProcessPriorityManager(cp);
||||||| merged common ancestors
    GetParticularProcessPriorityManager(cp);
=======
      GetParticularProcessPriorityManager(aBrowserParent->Manager());
>>>>>>> upstream-releases
  if (!pppm) {
    return;
  }

  Telemetry::ScalarAdd(
      Telemetry::ScalarID::DOM_CONTENTPROCESS_OS_PRIORITY_CHANGE_CONSIDERED, 1);

  pppm->TabActivityChanged(aBrowserParent, aIsActive);
}

NS_IMPL_ISUPPORTS(ParticularProcessPriorityManager, nsIObserver,
                  nsITimerCallback, nsISupportsWeakReference, nsINamed);

ParticularProcessPriorityManager::ParticularProcessPriorityManager(
<<<<<<< HEAD
    ContentParent* aContentParent)
    : mContentParent(aContentParent),
      mChildID(aContentParent->ChildID()),
      mPriority(PROCESS_PRIORITY_UNKNOWN),
      mHoldsCPUWakeLock(false),
      mHoldsHighPriorityWakeLock(false) {
||||||| merged common ancestors
  ContentParent* aContentParent)
  : mContentParent(aContentParent)
  , mChildID(aContentParent->ChildID())
  , mPriority(PROCESS_PRIORITY_UNKNOWN)
  , mHoldsCPUWakeLock(false)
  , mHoldsHighPriorityWakeLock(false)
{
=======
    ContentParent* aContentParent)
    : mContentParent(aContentParent),
      mChildID(aContentParent->ChildID()),
      mPriority(PROCESS_PRIORITY_UNKNOWN),
      mHoldsCPUWakeLock(false),
      mHoldsHighPriorityWakeLock(false),
      mHoldsPlayingAudioWakeLock(false),
      mHoldsPlayingVideoWakeLock(false) {
>>>>>>> upstream-releases
  MOZ_ASSERT(XRE_IsParentProcess());
  LOGP("Creating ParticularProcessPriorityManager.");
}

void ParticularProcessPriorityManager::StaticInit() {
  Preferences::AddUintVarCache(
      &sBackgroundPerceivableGracePeriodMS,
      "dom.ipc.processPriorityManager.backgroundPerceivableGracePeriodMS");
  Preferences::AddUintVarCache(
      &sBackgroundGracePeriodMS,
      "dom.ipc.processPriorityManager.backgroundGracePeriodMS");
}

void ParticularProcessPriorityManager::Init() {
  RegisterWakeLockObserver(this);

  nsCOMPtr<nsIObserverService> os = services::GetObserverService();
  if (os) {
    os->AddObserver(this, "remote-browser-shown", /* ownsWeak */ true);
    os->AddObserver(this, "ipc:browser-destroyed", /* ownsWeak */ true);
  }

  // This process may already hold the CPU lock; for example, our parent may
  // have acquired it on our behalf.
<<<<<<< HEAD
  WakeLockInformation info1, info2;
  GetWakeLockInfo(NS_LITERAL_STRING("cpu"), &info1);
  mHoldsCPUWakeLock = info1.lockingProcesses().Contains(ChildID());

  GetWakeLockInfo(NS_LITERAL_STRING("high-priority"), &info2);
  mHoldsHighPriorityWakeLock = info2.lockingProcesses().Contains(ChildID());
  LOGP("Done starting up.  mHoldsCPUWakeLock=%d, mHoldsHighPriorityWakeLock=%d",
       mHoldsCPUWakeLock, mHoldsHighPriorityWakeLock);
}

ParticularProcessPriorityManager::~ParticularProcessPriorityManager() {
||||||| merged common ancestors
  WakeLockInformation info1, info2;
  GetWakeLockInfo(NS_LITERAL_STRING("cpu"), &info1);
  mHoldsCPUWakeLock = info1.lockingProcesses().Contains(ChildID());

  GetWakeLockInfo(NS_LITERAL_STRING("high-priority"), &info2);
  mHoldsHighPriorityWakeLock = info2.lockingProcesses().Contains(ChildID());
  LOGP("Done starting up.  mHoldsCPUWakeLock=%d, mHoldsHighPriorityWakeLock=%d",
       mHoldsCPUWakeLock, mHoldsHighPriorityWakeLock);
}

ParticularProcessPriorityManager::~ParticularProcessPriorityManager()
{
=======
  mHoldsCPUWakeLock = IsHoldingWakeLock(NS_LITERAL_STRING("cpu"));
  mHoldsHighPriorityWakeLock =
      IsHoldingWakeLock(NS_LITERAL_STRING("high-priority"));
  mHoldsPlayingAudioWakeLock =
      IsHoldingWakeLock(NS_LITERAL_STRING("audio-playing"));
  mHoldsPlayingVideoWakeLock =
      IsHoldingWakeLock(NS_LITERAL_STRING("video-playing"));

  LOGP(
      "Done starting up.  mHoldsCPUWakeLock=%d, "
      "mHoldsHighPriorityWakeLock=%d, mHoldsPlayingAudioWakeLock=%d, "
      "mHoldsPlayingVideoWakeLock=%d",
      mHoldsCPUWakeLock, mHoldsHighPriorityWakeLock, mHoldsPlayingAudioWakeLock,
      mHoldsPlayingVideoWakeLock);
}

bool ParticularProcessPriorityManager::IsHoldingWakeLock(
    const nsAString& aTopic) {
  WakeLockInformation info;
  GetWakeLockInfo(aTopic, &info);
  return info.lockingProcesses().Contains(ChildID());
}

ParticularProcessPriorityManager::~ParticularProcessPriorityManager() {
>>>>>>> upstream-releases
  LOGP("Destroying ParticularProcessPriorityManager.");

  // Unregister our wake lock observer if ShutDown hasn't been called.  (The
  // wake lock observer takes raw refs, so we don't want to take chances here!)
  // We don't call UnregisterWakeLockObserver unconditionally because the code
  // will print a warning if it's called unnecessarily.

  if (mContentParent) {
    UnregisterWakeLockObserver(this);
  }
}

<<<<<<< HEAD
/* virtual */ void ParticularProcessPriorityManager::Notify(
    const WakeLockInformation& aInfo) {
||||||| merged common ancestors
/* virtual */ void
ParticularProcessPriorityManager::Notify(const WakeLockInformation& aInfo)
{
=======
/* virtual */
void ParticularProcessPriorityManager::Notify(
    const WakeLockInformation& aInfo) {
>>>>>>> upstream-releases
  if (!mContentParent) {
    // We've been shut down.
    return;
  }

  bool* dest = nullptr;
  if (aInfo.topic().EqualsLiteral("cpu")) {
    dest = &mHoldsCPUWakeLock;
  } else if (aInfo.topic().EqualsLiteral("high-priority")) {
    dest = &mHoldsHighPriorityWakeLock;
  } else if (aInfo.topic().EqualsLiteral("audio-playing")) {
    dest = &mHoldsPlayingAudioWakeLock;
  } else if (aInfo.topic().EqualsLiteral("video-playing")) {
    dest = &mHoldsPlayingVideoWakeLock;
  }

  if (dest) {
    bool thisProcessLocks = aInfo.lockingProcesses().Contains(ChildID());
    if (thisProcessLocks != *dest) {
      *dest = thisProcessLocks;
<<<<<<< HEAD
      LOGP(
          "Got wake lock changed event. "
          "Now mHoldsCPUWakeLock=%d, mHoldsHighPriorityWakeLock=%d",
          mHoldsCPUWakeLock, mHoldsHighPriorityWakeLock);
||||||| merged common ancestors
      LOGP("Got wake lock changed event. "
           "Now mHoldsCPUWakeLock=%d, mHoldsHighPriorityWakeLock=%d",
           mHoldsCPUWakeLock, mHoldsHighPriorityWakeLock);
=======
      LOGP(
          "Got wake lock changed event. "
          "Now mHoldsCPUWakeLock=%d, mHoldsHighPriorityWakeLock=%d, "
          "mHoldsPlayingAudioWakeLock=%d, mHoldsPlayingVideoWakeLock=%d",
          mHoldsCPUWakeLock, mHoldsHighPriorityWakeLock,
          mHoldsPlayingAudioWakeLock, mHoldsPlayingVideoWakeLock);
>>>>>>> upstream-releases
      ResetPriority();
    }
  }
}

NS_IMETHODIMP
ParticularProcessPriorityManager::Observe(nsISupports* aSubject,
                                          const char* aTopic,
                                          const char16_t* aData) {
  if (!mContentParent) {
    // We've been shut down.
    return NS_OK;
  }

  nsDependentCString topic(aTopic);

  if (topic.EqualsLiteral("remote-browser-shown")) {
    OnRemoteBrowserFrameShown(aSubject);
  } else if (topic.EqualsLiteral("ipc:browser-destroyed")) {
    OnBrowserParentDestroyed(aSubject);
  } else {
    MOZ_ASSERT(false);
  }

  return NS_OK;
}

uint64_t ParticularProcessPriorityManager::ChildID() const {
  // We have to cache mContentParent->ChildID() instead of getting it from the
  // ContentParent each time because after ShutDown() is called, mContentParent
  // is null.  If we didn't cache ChildID(), then we wouldn't be able to run
  // LOGP() after ShutDown().
  return mChildID;
}

int32_t ParticularProcessPriorityManager::Pid() const {
  return mContentParent ? mContentParent->Pid() : -1;
}

const nsAutoCString& ParticularProcessPriorityManager::NameWithComma() {
  mNameWithComma.Truncate();
  if (!mContentParent) {
    return mNameWithComma;  // empty string
  }

  nsAutoString name;
  mContentParent->FriendlyName(name);
  if (name.IsEmpty()) {
    return mNameWithComma;  // empty string
  }

  mNameWithComma = NS_ConvertUTF16toUTF8(name);
  mNameWithComma.AppendLiteral(", ");
  return mNameWithComma;
}

void ParticularProcessPriorityManager::OnRemoteBrowserFrameShown(
    nsISupports* aSubject) {
  RefPtr<nsFrameLoader> fl = do_QueryObject(aSubject);
  NS_ENSURE_TRUE_VOID(fl);

  BrowserParent* tp = BrowserParent::GetFrom(fl);
  NS_ENSURE_TRUE_VOID(tp);

  MOZ_ASSERT(XRE_IsParentProcess());
  if (tp->Manager() != mContentParent) {
    return;
  }

  // Ignore notifications that aren't from a Browser
  if (fl->OwnerIsMozBrowserFrame()) {
    ResetPriority();
  }

  nsCOMPtr<nsIObserverService> os = services::GetObserverService();
  if (os) {
    os->RemoveObserver(this, "remote-browser-shown");
  }
}

<<<<<<< HEAD
void ParticularProcessPriorityManager::OnTabParentDestroyed(
    nsISupports* aSubject) {
  nsCOMPtr<nsITabParent> tp = do_QueryInterface(aSubject);
  NS_ENSURE_TRUE_VOID(tp);
||||||| merged common ancestors
void
ParticularProcessPriorityManager::OnTabParentDestroyed(nsISupports* aSubject)
{
  nsCOMPtr<nsITabParent> tp = do_QueryInterface(aSubject);
  NS_ENSURE_TRUE_VOID(tp);
=======
void ParticularProcessPriorityManager::OnBrowserParentDestroyed(
    nsISupports* aSubject) {
  nsCOMPtr<nsIRemoteTab> remoteTab = do_QueryInterface(aSubject);
  NS_ENSURE_TRUE_VOID(remoteTab);
  BrowserHost* browserHost = BrowserHost::GetFrom(remoteTab.get());
>>>>>>> upstream-releases

  MOZ_ASSERT(XRE_IsParentProcess());
  if (browserHost->GetContentParent() &&
      browserHost->GetContentParent() != mContentParent) {
    return;
  }

  mActiveBrowserParents.RemoveEntry(browserHost->GetTabId());

  ResetPriority();
}

void ParticularProcessPriorityManager::ResetPriority() {
  ProcessPriority processPriority = ComputePriority();
  if (mPriority == PROCESS_PRIORITY_UNKNOWN || mPriority > processPriority) {
    // Apps set at a perceivable background priority are often playing media.
    // Most media will have short gaps while changing tracks between songs,
    // switching videos, etc.  Give these apps a longer grace period so they
    // can get their next track started, if there is one, before getting
    // downgraded.
    if (mPriority == PROCESS_PRIORITY_BACKGROUND_PERCEIVABLE) {
      ScheduleResetPriority(BACKGROUND_PERCEIVABLE_GRACE_PERIOD);
    } else {
      ScheduleResetPriority(BACKGROUND_GRACE_PERIOD);
    }
    return;
  }

  SetPriorityNow(processPriority);
}

void ParticularProcessPriorityManager::ResetPriorityNow() {
  SetPriorityNow(ComputePriority());
}

void ParticularProcessPriorityManager::ScheduleResetPriority(
    TimeoutPref aTimeoutPref) {
  if (mResetPriorityTimer) {
    LOGP("ScheduleResetPriority bailing; the timer is already running.");
    return;
  }

  uint32_t timeout = 0;
  switch (aTimeoutPref) {
    case BACKGROUND_PERCEIVABLE_GRACE_PERIOD:
      timeout = sBackgroundPerceivableGracePeriodMS;
      break;
    case BACKGROUND_GRACE_PERIOD:
      timeout = sBackgroundGracePeriodMS;
      break;
    default:
      MOZ_ASSERT(false, "Unrecognized timeout pref");
      break;
  }

  LOGP("Scheduling reset timer to fire in %dms.", timeout);
  NS_NewTimerWithCallback(getter_AddRefs(mResetPriorityTimer), this, timeout,
                          nsITimer::TYPE_ONE_SHOT);
}

NS_IMETHODIMP
ParticularProcessPriorityManager::Notify(nsITimer* aTimer) {
  LOGP("Reset priority timer callback; about to ResetPriorityNow.");
  ResetPriorityNow();
  mResetPriorityTimer = nullptr;
  return NS_OK;
}

ProcessPriority ParticularProcessPriorityManager::CurrentPriority() {
  return mPriority;
}

<<<<<<< HEAD
ProcessPriority ParticularProcessPriorityManager::ComputePriority() {
  if (!mActiveTabParents.IsEmpty() ||
      mContentParent->GetRemoteType().EqualsLiteral(EXTENSION_REMOTE_TYPE)) {
||||||| merged common ancestors
ProcessPriority
ParticularProcessPriorityManager::ComputePriority()
{
  if (!mActiveTabParents.IsEmpty() ||
      mContentParent->GetRemoteType().EqualsLiteral(EXTENSION_REMOTE_TYPE)) {
=======
ProcessPriority ParticularProcessPriorityManager::ComputePriority() {
  if (!mActiveBrowserParents.IsEmpty() ||
      mContentParent->GetRemoteType().EqualsLiteral(EXTENSION_REMOTE_TYPE) ||
      mHoldsPlayingAudioWakeLock) {
>>>>>>> upstream-releases
    return PROCESS_PRIORITY_FOREGROUND;
  }

  if (mHoldsCPUWakeLock || mHoldsHighPriorityWakeLock ||
      mHoldsPlayingVideoWakeLock) {
    return PROCESS_PRIORITY_BACKGROUND_PERCEIVABLE;
  }

  return PROCESS_PRIORITY_BACKGROUND;
}

void ParticularProcessPriorityManager::SetPriorityNow(
    ProcessPriority aPriority) {
  if (aPriority == PROCESS_PRIORITY_UNKNOWN) {
    MOZ_ASSERT(false);
    return;
  }

  if (!ProcessPriorityManagerImpl::PrefsEnabled() || !mContentParent ||
      mPriority == aPriority) {
    return;
  }

  if (mPriority == aPriority) {
    hal::SetProcessPriority(Pid(), mPriority);
    return;
  }

  LOGP("Changing priority from %s to %s.", ProcessPriorityToString(mPriority),
       ProcessPriorityToString(aPriority));

  ProcessPriority oldPriority = mPriority;

  mPriority = aPriority;

  // We skip incrementing the DOM_CONTENTPROCESS_OS_PRIORITY_RAISED if we're
  // transitioning from the PROCESS_PRIORITY_UNKNOWN level, which is where
  // we initialize at.
  if (oldPriority < mPriority && oldPriority != PROCESS_PRIORITY_UNKNOWN) {
    Telemetry::ScalarAdd(
        Telemetry::ScalarID::DOM_CONTENTPROCESS_OS_PRIORITY_RAISED, 1);
  } else if (oldPriority > mPriority) {
    Telemetry::ScalarAdd(
        Telemetry::ScalarID::DOM_CONTENTPROCESS_OS_PRIORITY_LOWERED, 1);
  }

  hal::SetProcessPriority(Pid(), mPriority);

  if (oldPriority != mPriority) {
    ProcessPriorityManagerImpl::GetSingleton()->NotifyProcessPriorityChanged(
        this, oldPriority);

    Unused << mContentParent->SendNotifyProcessPriorityChanged(mPriority);
  }

  FireTestOnlyObserverNotification("process-priority-set",
                                   ProcessPriorityToString(mPriority));
}

<<<<<<< HEAD
void ParticularProcessPriorityManager::TabActivityChanged(TabParent* aTabParent,
                                                          bool aIsActive) {
  MOZ_ASSERT(aTabParent);
||||||| merged common ancestors
void
ParticularProcessPriorityManager::TabActivityChanged(TabParent* aTabParent,
                                                     bool aIsActive)
{
  MOZ_ASSERT(aTabParent);
=======
void ParticularProcessPriorityManager::TabActivityChanged(
    BrowserParent* aBrowserParent, bool aIsActive) {
  MOZ_ASSERT(aBrowserParent);
>>>>>>> upstream-releases

  if (!aIsActive) {
    mActiveBrowserParents.RemoveEntry(aBrowserParent->GetTabId());
  } else {
    mActiveBrowserParents.PutEntry(aBrowserParent->GetTabId());
  }

  ResetPriority();
}

void ParticularProcessPriorityManager::ShutDown() {
  MOZ_ASSERT(mContentParent);

  UnregisterWakeLockObserver(this);

  if (mResetPriorityTimer) {
    mResetPriorityTimer->Cancel();
    mResetPriorityTimer = nullptr;
  }

  mContentParent = nullptr;
}

void ProcessPriorityManagerImpl::FireTestOnlyObserverNotification(
    const char* aTopic, const nsACString& aData /* = EmptyCString() */) {
  if (!TestMode()) {
    return;
  }

  nsCOMPtr<nsIObserverService> os = services::GetObserverService();
  NS_ENSURE_TRUE_VOID(os);

  nsPrintfCString topic("process-priority-manager:TEST-ONLY:%s", aTopic);

  LOG("Notifying observer %s, data %s", topic.get(),
      PromiseFlatCString(aData).get());
  os->NotifyObservers(nullptr, topic.get(), NS_ConvertUTF8toUTF16(aData).get());
}

void ParticularProcessPriorityManager::FireTestOnlyObserverNotification(
    const char* aTopic, const char* aData /* = nullptr */) {
  if (!ProcessPriorityManagerImpl::TestMode()) {
    return;
  }

  nsAutoCString data;
  if (aData) {
    data.AppendASCII(aData);
  }

  FireTestOnlyObserverNotification(aTopic, data);
}

void ParticularProcessPriorityManager::FireTestOnlyObserverNotification(
    const char* aTopic, const nsACString& aData /* = EmptyCString() */) {
  if (!ProcessPriorityManagerImpl::TestMode()) {
    return;
  }

  nsAutoCString data(nsPrintfCString("%" PRIu64, ChildID()));
  if (!aData.IsEmpty()) {
    data.Append(':');
    data.Append(aData);
  }

  // ProcessPriorityManagerImpl::GetSingleton() is guaranteed not to return
  // null, since ProcessPriorityManagerImpl is the only class which creates
  // ParticularProcessPriorityManagers.

  ProcessPriorityManagerImpl::GetSingleton()->FireTestOnlyObserverNotification(
      aTopic, data);
}

StaticRefPtr<ProcessPriorityManagerChild>
    ProcessPriorityManagerChild::sSingleton;

<<<<<<< HEAD
/* static */ void ProcessPriorityManagerChild::StaticInit() {
||||||| merged common ancestors
/* static */ void
ProcessPriorityManagerChild::StaticInit()
{
=======
/* static */
void ProcessPriorityManagerChild::StaticInit() {
>>>>>>> upstream-releases
  if (!sSingleton) {
    sSingleton = new ProcessPriorityManagerChild();
    sSingleton->Init();
    ClearOnShutdown(&sSingleton);
  }
}

<<<<<<< HEAD
/* static */ ProcessPriorityManagerChild*
ProcessPriorityManagerChild::Singleton() {
||||||| merged common ancestors
/* static */ ProcessPriorityManagerChild*
ProcessPriorityManagerChild::Singleton()
{
=======
/* static */
ProcessPriorityManagerChild* ProcessPriorityManagerChild::Singleton() {
>>>>>>> upstream-releases
  StaticInit();
  return sSingleton;
}

NS_IMPL_ISUPPORTS(ProcessPriorityManagerChild, nsIObserver)

ProcessPriorityManagerChild::ProcessPriorityManagerChild() {
  if (XRE_IsParentProcess()) {
    mCachedPriority = PROCESS_PRIORITY_MASTER;
  } else {
    mCachedPriority = PROCESS_PRIORITY_UNKNOWN;
  }
}

void ProcessPriorityManagerChild::Init() {
  // The process priority should only be changed in child processes; don't even
  // bother listening for changes if we're in the main process.
  if (!XRE_IsParentProcess()) {
    nsCOMPtr<nsIObserverService> os = services::GetObserverService();
    NS_ENSURE_TRUE_VOID(os);
    os->AddObserver(this, "ipc:process-priority-changed", /* weak = */ false);
  }
}

NS_IMETHODIMP
ProcessPriorityManagerChild::Observe(nsISupports* aSubject, const char* aTopic,
                                     const char16_t* aData) {
  MOZ_ASSERT(!strcmp(aTopic, "ipc:process-priority-changed"));

  nsCOMPtr<nsIPropertyBag2> props = do_QueryInterface(aSubject);
  NS_ENSURE_TRUE(props, NS_OK);

  int32_t priority = static_cast<int32_t>(PROCESS_PRIORITY_UNKNOWN);
  props->GetPropertyAsInt32(NS_LITERAL_STRING("priority"), &priority);
  NS_ENSURE_TRUE(ProcessPriority(priority) != PROCESS_PRIORITY_UNKNOWN, NS_OK);

  mCachedPriority = static_cast<ProcessPriority>(priority);

  return NS_OK;
}

bool ProcessPriorityManagerChild::CurrentProcessIsForeground() {
  return mCachedPriority == PROCESS_PRIORITY_UNKNOWN ||
         mCachedPriority >= PROCESS_PRIORITY_FOREGROUND;
}

}  // namespace

namespace mozilla {

<<<<<<< HEAD
/* static */ void ProcessPriorityManager::Init() {
||||||| merged common ancestors
/* static */ void
ProcessPriorityManager::Init()
{
=======
/* static */
void ProcessPriorityManager::Init() {
>>>>>>> upstream-releases
  ProcessPriorityManagerImpl::StaticInit();
  ProcessPriorityManagerChild::StaticInit();
  ParticularProcessPriorityManager::StaticInit();
}

<<<<<<< HEAD
/* static */ void ProcessPriorityManager::SetProcessPriority(
    ContentParent* aContentParent, ProcessPriority aPriority) {
||||||| merged common ancestors
/* static */ void
ProcessPriorityManager::SetProcessPriority(ContentParent* aContentParent,
                                           ProcessPriority aPriority)
{
=======
/* static */
void ProcessPriorityManager::SetProcessPriority(ContentParent* aContentParent,
                                                ProcessPriority aPriority) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aContentParent);

  ProcessPriorityManagerImpl* singleton =
      ProcessPriorityManagerImpl::GetSingleton();
  if (singleton) {
    singleton->SetProcessPriority(aContentParent, aPriority);
  }
}

<<<<<<< HEAD
/* static */ bool ProcessPriorityManager::CurrentProcessIsForeground() {
  return ProcessPriorityManagerChild::Singleton()->CurrentProcessIsForeground();
||||||| merged common ancestors
/* static */ bool
ProcessPriorityManager::CurrentProcessIsForeground()
{
  return ProcessPriorityManagerChild::Singleton()->
    CurrentProcessIsForeground();
=======
/* static */
bool ProcessPriorityManager::CurrentProcessIsForeground() {
  return ProcessPriorityManagerChild::Singleton()->CurrentProcessIsForeground();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ void ProcessPriorityManager::TabActivityChanged(
    TabParent* aTabParent, bool aIsActive) {
  MOZ_ASSERT(aTabParent);
||||||| merged common ancestors
/* static */ void
ProcessPriorityManager::TabActivityChanged(TabParent* aTabParent,
                                           bool aIsActive)
{
  MOZ_ASSERT(aTabParent);
=======
/* static */
void ProcessPriorityManager::TabActivityChanged(BrowserParent* aBrowserParent,
                                                bool aIsActive) {
  MOZ_ASSERT(aBrowserParent);
>>>>>>> upstream-releases

  ProcessPriorityManagerImpl* singleton =
      ProcessPriorityManagerImpl::GetSingleton();
  if (!singleton) {
    return;
  }

  singleton->TabActivityChanged(aBrowserParent, aIsActive);
}

}  // namespace mozilla
