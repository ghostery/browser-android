/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsCacheService.h"

#include "mozilla/ArrayUtils.h"
#include "mozilla/Attributes.h"
#include "mozilla/Assertions.h"
#include "mozilla/DebugOnly.h"
#include "mozilla/FileUtils.h"

#include "necko-config.h"

#include "nsCache.h"
#include "nsCacheRequest.h"
#include "nsCacheEntry.h"
#include "nsCacheEntryDescriptor.h"
#include "nsCacheDevice.h"
#include "nsICacheVisitor.h"
#include "nsDiskCacheDeviceSQL.h"
#include "nsCacheUtils.h"
#include "../cache2/CacheObserver.h"
#include "nsINamed.h"
#include "nsIObserverService.h"
#include "nsIPrefService.h"
#include "nsIPrefBranch.h"
#include "nsIFile.h"
#include "nsIOService.h"
#include "nsDirectoryServiceDefs.h"
#include "nsAppDirectoryServiceDefs.h"
#include "nsThreadUtils.h"
#include "nsProxyRelease.h"
#include "nsDeleteDir.h"
#include "nsNetCID.h"
#include <math.h>  // for log()
#include "mozilla/Services.h"
#include "nsITimer.h"
#include "mozIStorageService.h"

#include "mozilla/net/NeckoCommon.h"
#include <algorithm>

using namespace mozilla;
using namespace mozilla::net;

/******************************************************************************
 * nsCacheProfilePrefObserver
 *****************************************************************************/
<<<<<<< HEAD
#define DISK_CACHE_ENABLE_PREF "browser.cache.disk.enable"
#define DISK_CACHE_DIR_PREF "browser.cache.disk.parent_directory"
#define DISK_CACHE_SMART_SIZE_FIRST_RUN_PREF \
  "browser.cache.disk.smart_size.first_run"
#define DISK_CACHE_SMART_SIZE_ENABLED_PREF \
  "browser.cache.disk.smart_size.enabled"
#define DISK_CACHE_SMART_SIZE_PREF "browser.cache.disk.smart_size_cached_value"
#define DISK_CACHE_CAPACITY_PREF "browser.cache.disk.capacity"
#define DISK_CACHE_MAX_ENTRY_SIZE_PREF "browser.cache.disk.max_entry_size"
#define DISK_CACHE_CAPACITY 256000

#define DISK_CACHE_USE_OLD_MAX_SMART_SIZE_PREF \
  "browser.cache.disk.smart_size.use_old_max"

#define OFFLINE_CACHE_ENABLE_PREF "browser.cache.offline.enable"
#define OFFLINE_CACHE_DIR_PREF "browser.cache.offline.parent_directory"
||||||| merged common ancestors
#define DISK_CACHE_ENABLE_PREF      "browser.cache.disk.enable"
#define DISK_CACHE_DIR_PREF         "browser.cache.disk.parent_directory"
#define DISK_CACHE_SMART_SIZE_FIRST_RUN_PREF\
    "browser.cache.disk.smart_size.first_run"
#define DISK_CACHE_SMART_SIZE_ENABLED_PREF \
    "browser.cache.disk.smart_size.enabled"
#define DISK_CACHE_SMART_SIZE_PREF "browser.cache.disk.smart_size_cached_value"
#define DISK_CACHE_CAPACITY_PREF    "browser.cache.disk.capacity"
#define DISK_CACHE_MAX_ENTRY_SIZE_PREF "browser.cache.disk.max_entry_size"
#define DISK_CACHE_CAPACITY         256000

#define DISK_CACHE_USE_OLD_MAX_SMART_SIZE_PREF \
    "browser.cache.disk.smart_size.use_old_max"

#define OFFLINE_CACHE_ENABLE_PREF   "browser.cache.offline.enable"
#define OFFLINE_CACHE_DIR_PREF      "browser.cache.offline.parent_directory"
=======
#define OFFLINE_CACHE_ENABLE_PREF "browser.cache.offline.enable"
#define OFFLINE_CACHE_DIR_PREF "browser.cache.offline.parent_directory"
>>>>>>> upstream-releases
#define OFFLINE_CACHE_CAPACITY_PREF "browser.cache.offline.capacity"
#define OFFLINE_CACHE_CAPACITY 512000

<<<<<<< HEAD
#define MEMORY_CACHE_ENABLE_PREF "browser.cache.memory.enable"
#define MEMORY_CACHE_CAPACITY_PREF "browser.cache.memory.capacity"
#define MEMORY_CACHE_MAX_ENTRY_SIZE_PREF "browser.cache.memory.max_entry_size"
||||||| merged common ancestors
#define MEMORY_CACHE_ENABLE_PREF    "browser.cache.memory.enable"
#define MEMORY_CACHE_CAPACITY_PREF  "browser.cache.memory.capacity"
#define MEMORY_CACHE_MAX_ENTRY_SIZE_PREF "browser.cache.memory.max_entry_size"
=======
#define CACHE_COMPRESSION_LEVEL 1
>>>>>>> upstream-releases

<<<<<<< HEAD
#define CACHE_COMPRESSION_LEVEL_PREF "browser.cache.compression_level"
#define CACHE_COMPRESSION_LEVEL 1

#define SANITIZE_ON_SHUTDOWN_PREF "privacy.sanitize.sanitizeOnShutdown"
#define CLEAR_ON_SHUTDOWN_PREF "privacy.clearOnShutdown.cache"

static const char *observerList[] = {
    "profile-before-change",        "profile-do-change",
    NS_XPCOM_SHUTDOWN_OBSERVER_ID,  "last-pb-context-exited",
    "suspend_process_notification", "resume_process_notification"};

static const char *prefList[] = {
    DISK_CACHE_ENABLE_PREF,         DISK_CACHE_SMART_SIZE_ENABLED_PREF,
    DISK_CACHE_CAPACITY_PREF,       DISK_CACHE_DIR_PREF,
    DISK_CACHE_MAX_ENTRY_SIZE_PREF, DISK_CACHE_USE_OLD_MAX_SMART_SIZE_PREF,
    OFFLINE_CACHE_ENABLE_PREF,      OFFLINE_CACHE_CAPACITY_PREF,
    OFFLINE_CACHE_DIR_PREF,         MEMORY_CACHE_ENABLE_PREF,
    MEMORY_CACHE_CAPACITY_PREF,     MEMORY_CACHE_MAX_ENTRY_SIZE_PREF,
    CACHE_COMPRESSION_LEVEL_PREF,   SANITIZE_ON_SHUTDOWN_PREF,
    CLEAR_ON_SHUTDOWN_PREF,         nullptr,
||||||| merged common ancestors
#define CACHE_COMPRESSION_LEVEL_PREF "browser.cache.compression_level"
#define CACHE_COMPRESSION_LEVEL     1

#define SANITIZE_ON_SHUTDOWN_PREF   "privacy.sanitize.sanitizeOnShutdown"
#define CLEAR_ON_SHUTDOWN_PREF      "privacy.clearOnShutdown.cache"

static const char * observerList[] = {
    "profile-before-change",
    "profile-do-change",
    NS_XPCOM_SHUTDOWN_OBSERVER_ID,
    "last-pb-context-exited",
    "suspend_process_notification",
    "resume_process_notification"
};

static const char * prefList[] = {
    DISK_CACHE_ENABLE_PREF,
    DISK_CACHE_SMART_SIZE_ENABLED_PREF,
    DISK_CACHE_CAPACITY_PREF,
    DISK_CACHE_DIR_PREF,
    DISK_CACHE_MAX_ENTRY_SIZE_PREF,
    DISK_CACHE_USE_OLD_MAX_SMART_SIZE_PREF,
    OFFLINE_CACHE_ENABLE_PREF,
    OFFLINE_CACHE_CAPACITY_PREF,
    OFFLINE_CACHE_DIR_PREF,
    MEMORY_CACHE_ENABLE_PREF,
    MEMORY_CACHE_CAPACITY_PREF,
    MEMORY_CACHE_MAX_ENTRY_SIZE_PREF,
    CACHE_COMPRESSION_LEVEL_PREF,
    SANITIZE_ON_SHUTDOWN_PREF,
    CLEAR_ON_SHUTDOWN_PREF,
    nullptr,
=======
static const char* observerList[] = {
    "profile-before-change",        "profile-do-change",
    NS_XPCOM_SHUTDOWN_OBSERVER_ID,  "last-pb-context-exited",
    "suspend_process_notification", "resume_process_notification"};

static const char* prefList[] = {
    OFFLINE_CACHE_ENABLE_PREF,
    OFFLINE_CACHE_CAPACITY_PREF,
    OFFLINE_CACHE_DIR_PREF,
    nullptr,
>>>>>>> upstream-releases
};

<<<<<<< HEAD
// Cache sizes, in KB
const int32_t DEFAULT_CACHE_SIZE = 250 * 1024;  // 250 MB
#ifdef ANDROID
const int32_t MAX_CACHE_SIZE = 200 * 1024;      // 200 MB
const int32_t OLD_MAX_CACHE_SIZE = 200 * 1024;  // 200 MB
#else
const int32_t MAX_CACHE_SIZE = 350 * 1024;       // 350 MB
const int32_t OLD_MAX_CACHE_SIZE = 1024 * 1024;  //   1 GB
#endif
// Default cache size was 50 MB for many years until FF 4:
const int32_t PRE_GECKO_2_0_DEFAULT_CACHE_SIZE = 50 * 1024;

class nsCacheProfilePrefObserver : public nsIObserver {
  virtual ~nsCacheProfilePrefObserver() = default;

 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIOBSERVER

  nsCacheProfilePrefObserver()
      : mHaveProfile(false),
        mDiskCacheEnabled(false),
        mDiskCacheCapacity(0),
        mDiskCacheMaxEntrySize(-1)  // -1 means "no limit"
        ,
        mSmartSizeEnabled(false),
        mShouldUseOldMaxSmartSize(false),
        mOfflineCacheEnabled(false),
        mOfflineCacheCapacity(0),
        mMemoryCacheEnabled(true),
        mMemoryCacheCapacity(-1),
        mMemoryCacheMaxEntrySize(-1)  // -1 means "no limit"
        ,
        mCacheCompressionLevel(CACHE_COMPRESSION_LEVEL),
        mSanitizeOnShutdown(false),
        mClearCacheOnShutdown(false) {}

  nsresult Install();
  void Remove();
  nsresult ReadPrefs(nsIPrefBranch *branch);

  bool DiskCacheEnabled();
  int32_t DiskCacheCapacity() { return mDiskCacheCapacity; }
  void SetDiskCacheCapacity(int32_t);
  int32_t DiskCacheMaxEntrySize() { return mDiskCacheMaxEntrySize; }
  nsIFile *DiskCacheParentDirectory() { return mDiskCacheParentDirectory; }
  bool SmartSizeEnabled() { return mSmartSizeEnabled; }

  bool ShouldUseOldMaxSmartSize() { return mShouldUseOldMaxSmartSize; }
  void SetUseNewMaxSmartSize(bool useNew) {
    mShouldUseOldMaxSmartSize = !useNew;
  }
||||||| merged common ancestors
// Cache sizes, in KB
const int32_t DEFAULT_CACHE_SIZE = 250 * 1024;  // 250 MB
#ifdef ANDROID
const int32_t MAX_CACHE_SIZE = 200 * 1024;      // 200 MB
const int32_t OLD_MAX_CACHE_SIZE = 200 * 1024;  // 200 MB
#else
const int32_t MAX_CACHE_SIZE = 350 * 1024;      // 350 MB
const int32_t OLD_MAX_CACHE_SIZE = 1024 * 1024; //   1 GB
#endif
// Default cache size was 50 MB for many years until FF 4:
const int32_t PRE_GECKO_2_0_DEFAULT_CACHE_SIZE = 50 * 1024;

class nsCacheProfilePrefObserver : public nsIObserver
{
    virtual ~nsCacheProfilePrefObserver() = default;

public:
    NS_DECL_THREADSAFE_ISUPPORTS
    NS_DECL_NSIOBSERVER

    nsCacheProfilePrefObserver()
        : mHaveProfile(false)
        , mDiskCacheEnabled(false)
        , mDiskCacheCapacity(0)
        , mDiskCacheMaxEntrySize(-1) // -1 means "no limit"
        , mSmartSizeEnabled(false)
        , mShouldUseOldMaxSmartSize(false)
        , mOfflineCacheEnabled(false)
        , mOfflineCacheCapacity(0)
        , mMemoryCacheEnabled(true)
        , mMemoryCacheCapacity(-1)
        , mMemoryCacheMaxEntrySize(-1) // -1 means "no limit"
        , mCacheCompressionLevel(CACHE_COMPRESSION_LEVEL)
        , mSanitizeOnShutdown(false)
        , mClearCacheOnShutdown(false)
    {
    }

    nsresult        Install();
    void            Remove();
    nsresult        ReadPrefs(nsIPrefBranch* branch);

    bool            DiskCacheEnabled();
    int32_t         DiskCacheCapacity()         { return mDiskCacheCapacity; }
    void            SetDiskCacheCapacity(int32_t);
    int32_t         DiskCacheMaxEntrySize()     { return mDiskCacheMaxEntrySize; }
    nsIFile *       DiskCacheParentDirectory()  { return mDiskCacheParentDirectory; }
    bool            SmartSizeEnabled()          { return mSmartSizeEnabled; }

    bool            ShouldUseOldMaxSmartSize()        { return mShouldUseOldMaxSmartSize; }
    void            SetUseNewMaxSmartSize(bool useNew)     { mShouldUseOldMaxSmartSize = !useNew; }
=======
class nsCacheProfilePrefObserver : public nsIObserver {
  virtual ~nsCacheProfilePrefObserver() = default;

 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIOBSERVER

  nsCacheProfilePrefObserver()
      : mHaveProfile(false),
        mOfflineCacheEnabled(false),
        mOfflineCacheCapacity(0),
        mCacheCompressionLevel(CACHE_COMPRESSION_LEVEL),
        mSanitizeOnShutdown(false),
        mClearCacheOnShutdown(false) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool OfflineCacheEnabled();
  int32_t OfflineCacheCapacity() { return mOfflineCacheCapacity; }
  nsIFile *OfflineCacheParentDirectory() {
    return mOfflineCacheParentDirectory;
  }
||||||| merged common ancestors
    bool            OfflineCacheEnabled();
    int32_t         OfflineCacheCapacity()         { return mOfflineCacheCapacity; }
    nsIFile *       OfflineCacheParentDirectory()  { return mOfflineCacheParentDirectory; }
=======
  nsresult Install();
  void Remove();
  nsresult ReadPrefs(nsIPrefBranch* branch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool MemoryCacheEnabled();
  int32_t MemoryCacheCapacity();
  int32_t MemoryCacheMaxEntrySize() { return mMemoryCacheMaxEntrySize; }
||||||| merged common ancestors
    bool            MemoryCacheEnabled();
    int32_t         MemoryCacheCapacity();
    int32_t         MemoryCacheMaxEntrySize()     { return mMemoryCacheMaxEntrySize; }
=======
  nsIFile* DiskCacheParentDirectory() { return mDiskCacheParentDirectory; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  int32_t CacheCompressionLevel();

  bool SanitizeAtShutdown() {
    return mSanitizeOnShutdown && mClearCacheOnShutdown;
  }

  static uint32_t GetSmartCacheSize(const nsAString &cachePath,
                                    uint32_t currentSize,
                                    bool shouldUseOldMaxSmartSize);

  bool PermittedToSmartSize(nsIPrefBranch *, bool firstRun);
||||||| merged common ancestors
    int32_t         CacheCompressionLevel();

    bool            SanitizeAtShutdown() { return mSanitizeOnShutdown && mClearCacheOnShutdown; }

    static uint32_t GetSmartCacheSize(const nsAString& cachePath,
                                      uint32_t currentSize,
                                      bool shouldUseOldMaxSmartSize);

    bool                    PermittedToSmartSize(nsIPrefBranch*, bool firstRun);
=======
  bool OfflineCacheEnabled();
  int32_t OfflineCacheCapacity() { return mOfflineCacheCapacity; }
  nsIFile* OfflineCacheParentDirectory() {
    return mOfflineCacheParentDirectory;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void PrefChanged(const char *aPref);
||||||| merged common ancestors
    void PrefChanged(const char* aPref);
=======
  int32_t CacheCompressionLevel();
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  bool mHaveProfile;
||||||| merged common ancestors
private:
    bool                    mHaveProfile;
=======
  bool SanitizeAtShutdown() {
    return mSanitizeOnShutdown && mClearCacheOnShutdown;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool mDiskCacheEnabled;
  int32_t mDiskCacheCapacity;      // in kilobytes
  int32_t mDiskCacheMaxEntrySize;  // in kilobytes
  nsCOMPtr<nsIFile> mDiskCacheParentDirectory;
  bool mSmartSizeEnabled;
||||||| merged common ancestors
    bool                    mDiskCacheEnabled;
    int32_t                 mDiskCacheCapacity; // in kilobytes
    int32_t                 mDiskCacheMaxEntrySize; // in kilobytes
    nsCOMPtr<nsIFile>       mDiskCacheParentDirectory;
    bool                    mSmartSizeEnabled;
=======
  void PrefChanged(const char* aPref);
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool mShouldUseOldMaxSmartSize;
||||||| merged common ancestors
    bool                    mShouldUseOldMaxSmartSize;
=======
 private:
  bool mHaveProfile;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool mOfflineCacheEnabled;
  int32_t mOfflineCacheCapacity;  // in kilobytes
  nsCOMPtr<nsIFile> mOfflineCacheParentDirectory;
||||||| merged common ancestors
    bool                    mOfflineCacheEnabled;
    int32_t                 mOfflineCacheCapacity; // in kilobytes
    nsCOMPtr<nsIFile>       mOfflineCacheParentDirectory;
=======
  nsCOMPtr<nsIFile> mDiskCacheParentDirectory;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool mMemoryCacheEnabled;
  int32_t mMemoryCacheCapacity;      // in kilobytes
  int32_t mMemoryCacheMaxEntrySize;  // in kilobytes
||||||| merged common ancestors
    bool                    mMemoryCacheEnabled;
    int32_t                 mMemoryCacheCapacity; // in kilobytes
    int32_t                 mMemoryCacheMaxEntrySize; // in kilobytes
=======
  bool mOfflineCacheEnabled;
  int32_t mOfflineCacheCapacity;  // in kilobytes
  nsCOMPtr<nsIFile> mOfflineCacheParentDirectory;
>>>>>>> upstream-releases

  int32_t mCacheCompressionLevel;

  bool mSanitizeOnShutdown;
  bool mClearCacheOnShutdown;
};

NS_IMPL_ISUPPORTS(nsCacheProfilePrefObserver, nsIObserver)

<<<<<<< HEAD
class nsSetDiskSmartSizeCallback final : public nsITimerCallback,
                                         public nsINamed {
  ~nsSetDiskSmartSizeCallback() = default;

 public:
  NS_DECL_THREADSAFE_ISUPPORTS

  NS_IMETHOD Notify(nsITimer *aTimer) override {
    if (nsCacheService::gService) {
      nsCacheServiceAutoLock autoLock(
          LOCK_TELEM(NSSETDISKSMARTSIZECALLBACK_NOTIFY));
      nsCacheService::gService->SetDiskSmartSize_Locked();
      nsCacheService::gService->mSmartSizeTimer = nullptr;
    }
    return NS_OK;
  }

  NS_IMETHOD GetName(nsACString &aName) override {
    aName.AssignLiteral("nsSetDiskSmartSizeCallback");
    return NS_OK;
  }
};

NS_IMPL_ISUPPORTS(nsSetDiskSmartSizeCallback, nsITimerCallback, nsINamed)

// Runnable sent to main thread after the cache IO thread calculates available
// disk space, so that there is no race in setting mDiskCacheCapacity.
class nsSetSmartSizeEvent : public Runnable {
 public:
  explicit nsSetSmartSizeEvent(int32_t smartSize)
      : mozilla::Runnable("nsSetSmartSizeEvent"), mSmartSize(smartSize) {}

  NS_IMETHOD Run() override {
    NS_ASSERTION(NS_IsMainThread(),
                 "Setting smart size data off the main thread");

    // Main thread may have already called nsCacheService::Shutdown
    if (!nsCacheService::IsInitialized()) return NS_ERROR_NOT_AVAILABLE;

    // Ensure smart sizing wasn't switched off while event was pending.
    // It is safe to access the observer without the lock since we are
    // on the main thread and the value changes only on the main thread.
    if (!nsCacheService::gService->mObserver->SmartSizeEnabled()) return NS_OK;

    nsCacheService::SetDiskCacheCapacity(mSmartSize);

    nsCOMPtr<nsIPrefBranch> ps = do_GetService(NS_PREFSERVICE_CONTRACTID);
    if (!ps ||
        NS_FAILED(ps->SetIntPref(DISK_CACHE_SMART_SIZE_PREF, mSmartSize)))
      NS_WARNING("Failed to set smart size pref");

||||||| merged common ancestors
class nsSetDiskSmartSizeCallback final : public nsITimerCallback
                                       , public nsINamed
{
    ~nsSetDiskSmartSizeCallback() = default;

public:
    NS_DECL_THREADSAFE_ISUPPORTS

    NS_IMETHOD Notify(nsITimer* aTimer) override {
        if (nsCacheService::gService) {
            nsCacheServiceAutoLock autoLock(LOCK_TELEM(NSSETDISKSMARTSIZECALLBACK_NOTIFY));
            nsCacheService::gService->SetDiskSmartSize_Locked();
            nsCacheService::gService->mSmartSizeTimer = nullptr;
        }
        return NS_OK;
    }

    NS_IMETHOD GetName(nsACString& aName) override {
      aName.AssignLiteral("nsSetDiskSmartSizeCallback");
      return NS_OK;
    }
};

NS_IMPL_ISUPPORTS(nsSetDiskSmartSizeCallback, nsITimerCallback, nsINamed)

// Runnable sent to main thread after the cache IO thread calculates available
// disk space, so that there is no race in setting mDiskCacheCapacity.
class nsSetSmartSizeEvent: public Runnable
{
public:
  explicit nsSetSmartSizeEvent(int32_t smartSize)
    : mozilla::Runnable("nsSetSmartSizeEvent")
    , mSmartSize(smartSize)
  {
  }

  NS_IMETHOD Run() override
  {
    NS_ASSERTION(NS_IsMainThread(),
                 "Setting smart size data off the main thread");

    // Main thread may have already called nsCacheService::Shutdown
    if (!nsCacheService::IsInitialized())
      return NS_ERROR_NOT_AVAILABLE;

    // Ensure smart sizing wasn't switched off while event was pending.
    // It is safe to access the observer without the lock since we are
    // on the main thread and the value changes only on the main thread.
    if (!nsCacheService::gService->mObserver->SmartSizeEnabled())
      return NS_OK;

    nsCacheService::SetDiskCacheCapacity(mSmartSize);

    nsCOMPtr<nsIPrefBranch> ps = do_GetService(NS_PREFSERVICE_CONTRACTID);
    if (!ps ||
        NS_FAILED(ps->SetIntPref(DISK_CACHE_SMART_SIZE_PREF, mSmartSize)))
      NS_WARNING("Failed to set smart size pref");

=======
class nsBlockOnCacheThreadEvent : public Runnable {
 public:
  nsBlockOnCacheThreadEvent()
      : mozilla::Runnable("nsBlockOnCacheThreadEvent") {}
  NS_IMETHOD Run() override {
    nsCacheServiceAutoLock autoLock(LOCK_TELEM(NSBLOCKONCACHETHREADEVENT_RUN));
    CACHE_LOG_DEBUG(("nsBlockOnCacheThreadEvent [%p]\n", this));
    nsCacheService::gService->mNotified = true;
    nsCacheService::gService->mCondVar.Notify();
>>>>>>> upstream-releases
    return NS_OK;
<<<<<<< HEAD
  }

 private:
  int32_t mSmartSize;
};

// Runnable sent from main thread to cacheIO thread
class nsGetSmartSizeEvent : public Runnable {
 public:
  nsGetSmartSizeEvent(const nsAString &cachePath, uint32_t currentSize,
                      bool shouldUseOldMaxSmartSize)
      : mozilla::Runnable("nsGetSmartSizeEvent"),
        mCachePath(cachePath),
        mCurrentSize(currentSize),
        mShouldUseOldMaxSmartSize(shouldUseOldMaxSmartSize) {}

  // Calculates user's disk space available on a background thread and
  // dispatches this value back to the main thread.
  NS_IMETHOD Run() override {
    uint32_t size;
    size = nsCacheProfilePrefObserver::GetSmartCacheSize(
        mCachePath, mCurrentSize, mShouldUseOldMaxSmartSize);
    NS_DispatchToMainThread(new nsSetSmartSizeEvent(size));
    return NS_OK;
  }

 private:
  nsString mCachePath;
  uint32_t mCurrentSize;
  bool mShouldUseOldMaxSmartSize;
||||||| merged common ancestors
    }

private:
    int32_t mSmartSize;
};


// Runnable sent from main thread to cacheIO thread
class nsGetSmartSizeEvent: public Runnable
{
public:
  nsGetSmartSizeEvent(const nsAString& cachePath,
                      uint32_t currentSize,
                      bool shouldUseOldMaxSmartSize)
    : mozilla::Runnable("nsGetSmartSizeEvent")
    , mCachePath(cachePath)
    , mCurrentSize(currentSize)
    , mShouldUseOldMaxSmartSize(shouldUseOldMaxSmartSize)
  {
  }

  // Calculates user's disk space available on a background thread and
  // dispatches this value back to the main thread.
  NS_IMETHOD Run() override
  {
    uint32_t size;
    size = nsCacheProfilePrefObserver::GetSmartCacheSize(
      mCachePath, mCurrentSize, mShouldUseOldMaxSmartSize);
    NS_DispatchToMainThread(new nsSetSmartSizeEvent(size));
    return NS_OK;
    }

private:
    nsString mCachePath;
    uint32_t mCurrentSize;
    bool     mShouldUseOldMaxSmartSize;
=======
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class nsBlockOnCacheThreadEvent : public Runnable {
 public:
  nsBlockOnCacheThreadEvent()
      : mozilla::Runnable("nsBlockOnCacheThreadEvent") {}
  NS_IMETHOD Run() override {
    nsCacheServiceAutoLock autoLock(LOCK_TELEM(NSBLOCKONCACHETHREADEVENT_RUN));
    CACHE_LOG_DEBUG(("nsBlockOnCacheThreadEvent [%p]\n", this));
    nsCacheService::gService->mNotified = true;
    nsCacheService::gService->mCondVar.Notify();
    return NS_OK;
  }
};
||||||| merged common ancestors
class nsBlockOnCacheThreadEvent : public Runnable {
public:
  nsBlockOnCacheThreadEvent()
    : mozilla::Runnable("nsBlockOnCacheThreadEvent")
  {
    }
    NS_IMETHOD Run() override
    {
        nsCacheServiceAutoLock autoLock(LOCK_TELEM(NSBLOCKONCACHETHREADEVENT_RUN));
        CACHE_LOG_DEBUG(("nsBlockOnCacheThreadEvent [%p]\n", this));
        nsCacheService::gService->mNotified = true;
        nsCacheService::gService->mCondVar.Notify();
        return NS_OK;
    }
};
=======
nsresult nsCacheProfilePrefObserver::Install() {
  // install profile-change observer
  nsCOMPtr<nsIObserverService> observerService =
      mozilla::services::GetObserverService();
  if (!observerService) return NS_ERROR_FAILURE;
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsCacheProfilePrefObserver::Install() {
  // install profile-change observer
  nsCOMPtr<nsIObserverService> observerService =
      mozilla::services::GetObserverService();
  if (!observerService) return NS_ERROR_FAILURE;
||||||| merged common ancestors
=======
  nsresult rv, rv2 = NS_OK;
  for (auto& observer : observerList) {
    rv = observerService->AddObserver(this, observer, false);
    if (NS_FAILED(rv)) rv2 = rv;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult rv, rv2 = NS_OK;
  for (auto &observer : observerList) {
    rv = observerService->AddObserver(this, observer, false);
    if (NS_FAILED(rv)) rv2 = rv;
  }
||||||| merged common ancestors
nsresult
nsCacheProfilePrefObserver::Install()
{
    // install profile-change observer
    nsCOMPtr<nsIObserverService> observerService =
        mozilla::services::GetObserverService();
    if (!observerService)
        return NS_ERROR_FAILURE;

    nsresult rv, rv2 = NS_OK;
    for (auto& observer : observerList) {
        rv = observerService->AddObserver(this, observer, false);
        if (NS_FAILED(rv))
            rv2 = rv;
    }
=======
  // install preferences observer
  nsCOMPtr<nsIPrefBranch> branch = do_GetService(NS_PREFSERVICE_CONTRACTID);
  if (!branch) return NS_ERROR_FAILURE;

  Preferences::RegisterCallbacks(
      PREF_CHANGE_METHOD(nsCacheProfilePrefObserver::PrefChanged), prefList,
      this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // install preferences observer
  nsCOMPtr<nsIPrefBranch> branch = do_GetService(NS_PREFSERVICE_CONTRACTID);
  if (!branch) return NS_ERROR_FAILURE;
||||||| merged common ancestors
    // install preferences observer
    nsCOMPtr<nsIPrefBranch> branch = do_GetService(NS_PREFSERVICE_CONTRACTID);
    if (!branch) return NS_ERROR_FAILURE;
=======
  // Determine if we have a profile already
  //     Install() is called *after* the profile-after-change notification
  //     when there is only a single profile, or it is specified on the
  //     commandline at startup.
  //     In that case, we detect the presence of a profile by the existence
  //     of the NS_APP_USER_PROFILE_50_DIR directory.
>>>>>>> upstream-releases

<<<<<<< HEAD
  Preferences::RegisterCallbacks(
      PREF_CHANGE_METHOD(nsCacheProfilePrefObserver::PrefChanged), prefList,
      this);
||||||| merged common ancestors
    Preferences::RegisterCallbacks(
        PREF_CHANGE_METHOD(nsCacheProfilePrefObserver::PrefChanged),
        prefList, this);
=======
  nsCOMPtr<nsIFile> directory;
  rv = NS_GetSpecialDirectory(NS_APP_USER_PROFILE_50_DIR,
                              getter_AddRefs(directory));
  if (NS_SUCCEEDED(rv)) mHaveProfile = true;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Determine if we have a profile already
  //     Install() is called *after* the profile-after-change notification
  //     when there is only a single profile, or it is specified on the
  //     commandline at startup.
  //     In that case, we detect the presence of a profile by the existence
  //     of the NS_APP_USER_PROFILE_50_DIR directory.
||||||| merged common ancestors
    // Determine if we have a profile already
    //     Install() is called *after* the profile-after-change notification
    //     when there is only a single profile, or it is specified on the
    //     commandline at startup.
    //     In that case, we detect the presence of a profile by the existence
    //     of the NS_APP_USER_PROFILE_50_DIR directory.
=======
  rv = ReadPrefs(branch);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIFile> directory;
  rv = NS_GetSpecialDirectory(NS_APP_USER_PROFILE_50_DIR,
                              getter_AddRefs(directory));
  if (NS_SUCCEEDED(rv)) mHaveProfile = true;

  rv = ReadPrefs(branch);
  NS_ENSURE_SUCCESS(rv, rv);

  return rv2;
||||||| merged common ancestors
    nsCOMPtr<nsIFile> directory;
    rv = NS_GetSpecialDirectory(NS_APP_USER_PROFILE_50_DIR,
                                getter_AddRefs(directory));
    if (NS_SUCCEEDED(rv))
        mHaveProfile = true;

    rv = ReadPrefs(branch);
    NS_ENSURE_SUCCESS(rv, rv);

    return rv2;
=======
  return rv2;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsCacheProfilePrefObserver::Remove() {
  // remove Observer Service observers
  nsCOMPtr<nsIObserverService> obs = mozilla::services::GetObserverService();
  if (obs) {
    for (auto &observer : observerList) {
      obs->RemoveObserver(this, observer);
||||||| merged common ancestors

void
nsCacheProfilePrefObserver::Remove()
{
    // remove Observer Service observers
    nsCOMPtr<nsIObserverService> obs =
        mozilla::services::GetObserverService();
    if (obs) {
        for (auto& observer : observerList) {
            obs->RemoveObserver(this, observer);
        }
=======
void nsCacheProfilePrefObserver::Remove() {
  // remove Observer Service observers
  nsCOMPtr<nsIObserverService> obs = mozilla::services::GetObserverService();
  if (obs) {
    for (auto& observer : observerList) {
      obs->RemoveObserver(this, observer);
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  // remove Pref Service observers
  nsCOMPtr<nsIPrefBranch> prefs = do_GetService(NS_PREFSERVICE_CONTRACTID);
  if (!prefs) return;
  Preferences::UnregisterCallbacks(
      PREF_CHANGE_METHOD(nsCacheProfilePrefObserver::PrefChanged), prefList,
      this);
}

void nsCacheProfilePrefObserver::SetDiskCacheCapacity(int32_t capacity) {
  mDiskCacheCapacity = std::max(0, capacity);
||||||| merged common ancestors
    // remove Pref Service observers
    nsCOMPtr<nsIPrefBranch> prefs =
        do_GetService(NS_PREFSERVICE_CONTRACTID);
    if (!prefs)
        return;
    Preferences::UnregisterCallbacks(
        PREF_CHANGE_METHOD(nsCacheProfilePrefObserver::PrefChanged),
        prefList, this);
}

void
nsCacheProfilePrefObserver::SetDiskCacheCapacity(int32_t capacity)
{
    mDiskCacheCapacity = std::max(0, capacity);
=======
  // remove Pref Service observers
  nsCOMPtr<nsIPrefBranch> prefs = do_GetService(NS_PREFSERVICE_CONTRACTID);
  if (!prefs) return;
  Preferences::UnregisterCallbacks(
      PREF_CHANGE_METHOD(nsCacheProfilePrefObserver::PrefChanged), prefList,
      this);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsCacheProfilePrefObserver::Observe(nsISupports *subject, const char *topic,
                                    const char16_t *data_unicode) {
  NS_ConvertUTF16toUTF8 data(data_unicode);
  CACHE_LOG_INFO(("Observe [topic=%s data=%s]\n", topic, data.get()));

  if (!nsCacheService::IsInitialized()) {
    if (!strcmp("resume_process_notification", topic)) {
      // A suspended process has a closed cache, so re-open it here.
      nsCacheService::GlobalInstance()->Init();
||||||| merged common ancestors
nsCacheProfilePrefObserver::Observe(nsISupports *     subject,
                                    const char *      topic,
                                    const char16_t * data_unicode)
{
    NS_ConvertUTF16toUTF8 data(data_unicode);
    CACHE_LOG_INFO(("Observe [topic=%s data=%s]\n", topic, data.get()));

    if (!nsCacheService::IsInitialized()) {
        if (!strcmp("resume_process_notification", topic)) {
            // A suspended process has a closed cache, so re-open it here.
            nsCacheService::GlobalInstance()->Init();
        }
        return NS_OK;
    }

    if (!strcmp(NS_XPCOM_SHUTDOWN_OBSERVER_ID, topic)) {
        // xpcom going away, shutdown cache service
        nsCacheService::GlobalInstance()->Shutdown();
    } else if (!strcmp("profile-before-change", topic)) {
        // profile before change
        mHaveProfile = false;

        // XXX shutdown devices
        nsCacheService::OnProfileShutdown();
    } else if (!strcmp("suspend_process_notification", topic)) {
        // A suspended process may never return, so shutdown the cache to reduce
        // cache corruption.
        nsCacheService::GlobalInstance()->Shutdown();
    } else if (!strcmp("profile-do-change", topic)) {
        // profile after change
        mHaveProfile = true;
        nsCOMPtr<nsIPrefBranch> branch = do_GetService(NS_PREFSERVICE_CONTRACTID);
        if (!branch) {
            return NS_ERROR_FAILURE;
        }
        (void)ReadPrefs(branch);
        nsCacheService::OnProfileChanged();

    } else if (!strcmp("last-pb-context-exited", topic)) {
        nsCacheService::LeavePrivateBrowsing();
=======
nsCacheProfilePrefObserver::Observe(nsISupports* subject, const char* topic,
                                    const char16_t* data_unicode) {
  NS_ConvertUTF16toUTF8 data(data_unicode);
  CACHE_LOG_INFO(("Observe [topic=%s data=%s]\n", topic, data.get()));

  if (!nsCacheService::IsInitialized()) {
    if (!strcmp("resume_process_notification", topic)) {
      // A suspended process has a closed cache, so re-open it here.
      nsCacheService::GlobalInstance()->Init();
>>>>>>> upstream-releases
    }
    return NS_OK;
<<<<<<< HEAD
  }

  if (!strcmp(NS_XPCOM_SHUTDOWN_OBSERVER_ID, topic)) {
    // xpcom going away, shutdown cache service
    nsCacheService::GlobalInstance()->Shutdown();
  } else if (!strcmp("profile-before-change", topic)) {
    // profile before change
    mHaveProfile = false;

    // XXX shutdown devices
    nsCacheService::OnProfileShutdown();
  } else if (!strcmp("suspend_process_notification", topic)) {
    // A suspended process may never return, so shutdown the cache to reduce
    // cache corruption.
    nsCacheService::GlobalInstance()->Shutdown();
  } else if (!strcmp("profile-do-change", topic)) {
    // profile after change
    mHaveProfile = true;
    nsCOMPtr<nsIPrefBranch> branch = do_GetService(NS_PREFSERVICE_CONTRACTID);
    if (!branch) {
      return NS_ERROR_FAILURE;
    }
    (void)ReadPrefs(branch);
    nsCacheService::OnProfileChanged();

  } else if (!strcmp("last-pb-context-exited", topic)) {
    nsCacheService::LeavePrivateBrowsing();
  }

  return NS_OK;
}

void nsCacheProfilePrefObserver::PrefChanged(const char *aPref) {
  // ignore pref changes until we're done switch profiles
  if (!mHaveProfile) return;
  // which preference changed?
  nsresult rv;
  if (!strcmp(DISK_CACHE_ENABLE_PREF, aPref)) {
    rv = Preferences::GetBool(DISK_CACHE_ENABLE_PREF, &mDiskCacheEnabled);
    if (NS_FAILED(rv)) return;
    nsCacheService::SetDiskCacheEnabled(DiskCacheEnabled());

  } else if (!strcmp(DISK_CACHE_CAPACITY_PREF, aPref)) {
    int32_t capacity = 0;
    rv = Preferences::GetInt(DISK_CACHE_CAPACITY_PREF, &capacity);
    if (NS_FAILED(rv)) return;
    mDiskCacheCapacity = std::max(0, capacity);
    nsCacheService::SetDiskCacheCapacity(mDiskCacheCapacity);

    // Update the cache capacity when smart sizing is turned on/off
  } else if (!strcmp(DISK_CACHE_SMART_SIZE_ENABLED_PREF, aPref)) {
    // Is the update because smartsizing was turned on, or off?
    rv = Preferences::GetBool(DISK_CACHE_SMART_SIZE_ENABLED_PREF,
                              &mSmartSizeEnabled);
    if (NS_FAILED(rv)) return;
    int32_t newCapacity = 0;
    if (mSmartSizeEnabled) {
      nsCacheService::SetDiskSmartSize();
    } else {
      // Smart sizing switched off: use user specified size
      rv = Preferences::GetInt(DISK_CACHE_CAPACITY_PREF, &newCapacity);
      if (NS_FAILED(rv)) return;
      mDiskCacheCapacity = std::max(0, newCapacity);
      nsCacheService::SetDiskCacheCapacity(mDiskCacheCapacity);
    }
  } else if (!strcmp(DISK_CACHE_USE_OLD_MAX_SMART_SIZE_PREF, aPref)) {
    rv = Preferences::GetBool(DISK_CACHE_USE_OLD_MAX_SMART_SIZE_PREF,
                              &mShouldUseOldMaxSmartSize);
    if (NS_FAILED(rv)) return;
  } else if (!strcmp(DISK_CACHE_MAX_ENTRY_SIZE_PREF, aPref)) {
    int32_t newMaxSize;
    rv = Preferences::GetInt(DISK_CACHE_MAX_ENTRY_SIZE_PREF, &newMaxSize);
    if (NS_FAILED(rv)) return;

    mDiskCacheMaxEntrySize = std::max(-1, newMaxSize);
    nsCacheService::SetDiskCacheMaxEntrySize(mDiskCacheMaxEntrySize);

#if 0
    } else if (!strcmp(DISK_CACHE_DIR_PREF, aPref)) {
        // XXX We probaby don't want to respond to this pref except after
        // XXX profile changes.  Ideally, there should be somekind of user
        // XXX notification that the pref change won't take effect until
        // XXX the next time the profile changes (browser launch)
#endif
  } else

      // which preference changed?
      if (!strcmp(OFFLINE_CACHE_ENABLE_PREF, aPref)) {
    rv = Preferences::GetBool(OFFLINE_CACHE_ENABLE_PREF, &mOfflineCacheEnabled);
    if (NS_FAILED(rv)) return;
    nsCacheService::SetOfflineCacheEnabled(OfflineCacheEnabled());

  } else if (!strcmp(OFFLINE_CACHE_CAPACITY_PREF, aPref)) {
    int32_t capacity = 0;
    rv = Preferences::GetInt(OFFLINE_CACHE_CAPACITY_PREF, &capacity);
    if (NS_FAILED(rv)) return;
    mOfflineCacheCapacity = std::max(0, capacity);
    nsCacheService::SetOfflineCacheCapacity(mOfflineCacheCapacity);
||||||| merged common ancestors
}

void
nsCacheProfilePrefObserver::PrefChanged(const char* aPref)
{
    // ignore pref changes until we're done switch profiles
    if (!mHaveProfile)
        return;
    // which preference changed?
    nsresult rv;
    if (!strcmp(DISK_CACHE_ENABLE_PREF, aPref)) {

        rv = Preferences::GetBool(DISK_CACHE_ENABLE_PREF,
                                  &mDiskCacheEnabled);
        if (NS_FAILED(rv))
            return;
        nsCacheService::SetDiskCacheEnabled(DiskCacheEnabled());

    } else if (!strcmp(DISK_CACHE_CAPACITY_PREF, aPref)) {

        int32_t capacity = 0;
        rv = Preferences::GetInt(DISK_CACHE_CAPACITY_PREF, &capacity);
        if (NS_FAILED(rv))
            return;
        mDiskCacheCapacity = std::max(0, capacity);
        nsCacheService::SetDiskCacheCapacity(mDiskCacheCapacity);

    // Update the cache capacity when smart sizing is turned on/off
    } else if (!strcmp(DISK_CACHE_SMART_SIZE_ENABLED_PREF, aPref)) {
        // Is the update because smartsizing was turned on, or off?
        rv = Preferences::GetBool(DISK_CACHE_SMART_SIZE_ENABLED_PREF,
                                  &mSmartSizeEnabled);
        if (NS_FAILED(rv))
            return;
        int32_t newCapacity = 0;
        if (mSmartSizeEnabled) {
            nsCacheService::SetDiskSmartSize();
        } else {
            // Smart sizing switched off: use user specified size
            rv = Preferences::GetInt(DISK_CACHE_CAPACITY_PREF, &newCapacity);
            if (NS_FAILED(rv))
                return;
            mDiskCacheCapacity = std::max(0, newCapacity);
            nsCacheService::SetDiskCacheCapacity(mDiskCacheCapacity);
        }
    } else if (!strcmp(DISK_CACHE_USE_OLD_MAX_SMART_SIZE_PREF, aPref)) {
        rv = Preferences::GetBool(DISK_CACHE_USE_OLD_MAX_SMART_SIZE_PREF,
                                  &mShouldUseOldMaxSmartSize);
        if (NS_FAILED(rv))
            return;
    } else if (!strcmp(DISK_CACHE_MAX_ENTRY_SIZE_PREF, aPref)) {
        int32_t newMaxSize;
        rv = Preferences::GetInt(DISK_CACHE_MAX_ENTRY_SIZE_PREF,
                                 &newMaxSize);
        if (NS_FAILED(rv))
            return;

        mDiskCacheMaxEntrySize = std::max(-1, newMaxSize);
        nsCacheService::SetDiskCacheMaxEntrySize(mDiskCacheMaxEntrySize);

#if 0
    } else if (!strcmp(DISK_CACHE_DIR_PREF, aPref)) {
        // XXX We probaby don't want to respond to this pref except after
        // XXX profile changes.  Ideally, there should be somekind of user
        // XXX notification that the pref change won't take effect until
        // XXX the next time the profile changes (browser launch)
#endif
    } else

    // which preference changed?
    if (!strcmp(OFFLINE_CACHE_ENABLE_PREF, aPref)) {

        rv = Preferences::GetBool(OFFLINE_CACHE_ENABLE_PREF,
                                  &mOfflineCacheEnabled);
        if (NS_FAILED(rv))  return;
        nsCacheService::SetOfflineCacheEnabled(OfflineCacheEnabled());

    } else if (!strcmp(OFFLINE_CACHE_CAPACITY_PREF, aPref)) {

        int32_t capacity = 0;
        rv = Preferences::GetInt(OFFLINE_CACHE_CAPACITY_PREF, &capacity);
        if (NS_FAILED(rv))  return;
        mOfflineCacheCapacity = std::max(0, capacity);
        nsCacheService::SetOfflineCacheCapacity(mOfflineCacheCapacity);
=======
  }

  if (!strcmp(NS_XPCOM_SHUTDOWN_OBSERVER_ID, topic)) {
    // xpcom going away, shutdown cache service
    nsCacheService::GlobalInstance()->Shutdown();
  } else if (!strcmp("profile-before-change", topic)) {
    // profile before change
    mHaveProfile = false;

    // XXX shutdown devices
    nsCacheService::OnProfileShutdown();
  } else if (!strcmp("suspend_process_notification", topic)) {
    // A suspended process may never return, so shutdown the cache to reduce
    // cache corruption.
    nsCacheService::GlobalInstance()->Shutdown();
  } else if (!strcmp("profile-do-change", topic)) {
    // profile after change
    mHaveProfile = true;
    nsCOMPtr<nsIPrefBranch> branch = do_GetService(NS_PREFSERVICE_CONTRACTID);
    if (!branch) {
      return NS_ERROR_FAILURE;
    }
    (void)ReadPrefs(branch);
    nsCacheService::OnProfileChanged();

  } else if (!strcmp("last-pb-context-exited", topic)) {
    nsCacheService::LeavePrivateBrowsing();
  }

  return NS_OK;
}

void nsCacheProfilePrefObserver::PrefChanged(const char* aPref) {
  // ignore pref changes until we're done switch profiles
  if (!mHaveProfile) return;
  // which preference changed?
  nsresult rv;
  if (!strcmp(OFFLINE_CACHE_ENABLE_PREF, aPref)) {
    rv = Preferences::GetBool(OFFLINE_CACHE_ENABLE_PREF, &mOfflineCacheEnabled);
    if (NS_FAILED(rv)) return;
    nsCacheService::SetOfflineCacheEnabled(OfflineCacheEnabled());

  } else if (!strcmp(OFFLINE_CACHE_CAPACITY_PREF, aPref)) {
    int32_t capacity = 0;
    rv = Preferences::GetInt(OFFLINE_CACHE_CAPACITY_PREF, &capacity);
    if (NS_FAILED(rv)) return;
    mOfflineCacheCapacity = std::max(0, capacity);
    nsCacheService::SetOfflineCacheCapacity(mOfflineCacheCapacity);
>>>>>>> upstream-releases
#if 0
    } else if (!strcmp(OFFLINE_CACHE_DIR_PREF, aPref)) {
        // XXX We probaby don't want to respond to this pref except after
        // XXX profile changes.  Ideally, there should be some kind of user
        // XXX notification that the pref change won't take effect until
        // XXX the next time the profile changes (browser launch)
#endif
<<<<<<< HEAD
  } else

      if (!strcmp(MEMORY_CACHE_ENABLE_PREF, aPref)) {
    rv = Preferences::GetBool(MEMORY_CACHE_ENABLE_PREF, &mMemoryCacheEnabled);
    if (NS_FAILED(rv)) return;
    nsCacheService::SetMemoryCache();

  } else if (!strcmp(MEMORY_CACHE_CAPACITY_PREF, aPref)) {
    mMemoryCacheCapacity = -1;
    (void)Preferences::GetInt(MEMORY_CACHE_CAPACITY_PREF,
                              &mMemoryCacheCapacity);
    nsCacheService::SetMemoryCache();
  } else if (!strcmp(MEMORY_CACHE_MAX_ENTRY_SIZE_PREF, aPref)) {
    int32_t newMaxSize;
    rv = Preferences::GetInt(MEMORY_CACHE_MAX_ENTRY_SIZE_PREF, &newMaxSize);
    if (NS_FAILED(rv)) return;

    mMemoryCacheMaxEntrySize = std::max(-1, newMaxSize);
    nsCacheService::SetMemoryCacheMaxEntrySize(mMemoryCacheMaxEntrySize);
  } else if (!strcmp(CACHE_COMPRESSION_LEVEL_PREF, aPref)) {
    mCacheCompressionLevel = CACHE_COMPRESSION_LEVEL;
    (void)Preferences::GetInt(CACHE_COMPRESSION_LEVEL_PREF,
                              &mCacheCompressionLevel);
    mCacheCompressionLevel = std::max(0, mCacheCompressionLevel);
    mCacheCompressionLevel = std::min(9, mCacheCompressionLevel);
  } else if (!strcmp(SANITIZE_ON_SHUTDOWN_PREF, aPref)) {
    rv = Preferences::GetBool(SANITIZE_ON_SHUTDOWN_PREF, &mSanitizeOnShutdown);
    if (NS_FAILED(rv)) return;
    nsCacheService::SetDiskCacheEnabled(DiskCacheEnabled());
  } else if (!strcmp(CLEAR_ON_SHUTDOWN_PREF, aPref)) {
    rv = Preferences::GetBool(CLEAR_ON_SHUTDOWN_PREF, &mClearCacheOnShutdown);
    if (NS_FAILED(rv)) return;
    nsCacheService::SetDiskCacheEnabled(DiskCacheEnabled());
  }
||||||| merged common ancestors
    } else

    if (!strcmp(MEMORY_CACHE_ENABLE_PREF, aPref)) {

        rv = Preferences::GetBool(MEMORY_CACHE_ENABLE_PREF,
                                  &mMemoryCacheEnabled);
        if (NS_FAILED(rv))
            return;
        nsCacheService::SetMemoryCache();

    } else if (!strcmp(MEMORY_CACHE_CAPACITY_PREF, aPref)) {

        mMemoryCacheCapacity = -1;
        (void) Preferences::GetInt(MEMORY_CACHE_CAPACITY_PREF,
                                   &mMemoryCacheCapacity);
        nsCacheService::SetMemoryCache();
    } else if (!strcmp(MEMORY_CACHE_MAX_ENTRY_SIZE_PREF, aPref)) {
        int32_t newMaxSize;
        rv = Preferences::GetInt(MEMORY_CACHE_MAX_ENTRY_SIZE_PREF,
                                 &newMaxSize);
        if (NS_FAILED(rv))
            return;

        mMemoryCacheMaxEntrySize = std::max(-1, newMaxSize);
        nsCacheService::SetMemoryCacheMaxEntrySize(mMemoryCacheMaxEntrySize);
    } else if (!strcmp(CACHE_COMPRESSION_LEVEL_PREF, aPref)) {
        mCacheCompressionLevel = CACHE_COMPRESSION_LEVEL;
        (void)Preferences::GetInt(CACHE_COMPRESSION_LEVEL_PREF,
                                 &mCacheCompressionLevel);
        mCacheCompressionLevel = std::max(0, mCacheCompressionLevel);
        mCacheCompressionLevel = std::min(9, mCacheCompressionLevel);
    } else if (!strcmp(SANITIZE_ON_SHUTDOWN_PREF, aPref)) {
        rv = Preferences::GetBool(SANITIZE_ON_SHUTDOWN_PREF,
                                  &mSanitizeOnShutdown);
        if (NS_FAILED(rv))
            return;
        nsCacheService::SetDiskCacheEnabled(DiskCacheEnabled());
    } else if (!strcmp(CLEAR_ON_SHUTDOWN_PREF, aPref)) {
        rv = Preferences::GetBool(CLEAR_ON_SHUTDOWN_PREF,
                                  &mClearCacheOnShutdown);
        if (NS_FAILED(rv))
            return;
        nsCacheService::SetDiskCacheEnabled(DiskCacheEnabled());
    }
=======
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Returns default ("smart") size (in KB) of cache, given available disk space
// (also in KB)
static uint32_t SmartCacheSize(const uint32_t availKB,
                               bool shouldUseOldMaxSmartSize) {
  uint32_t maxSize =
      shouldUseOldMaxSmartSize ? OLD_MAX_CACHE_SIZE : MAX_CACHE_SIZE;

  if (availKB > 100 * 1024 * 1024)
    return maxSize;  // skip computing if we're over 100 GB

  // Grow/shrink in 10 MB units, deliberately, so that in the common case we
  // don't shrink cache and evict items every time we startup (it's important
  // that we don't slow down startup benchmarks).
  uint32_t sz10MBs = 0;
  uint32_t avail10MBs = availKB / (1024 * 10);

  // .5% of space above 25 GB
  if (avail10MBs > 2500) {
    sz10MBs += static_cast<uint32_t>((avail10MBs - 2500) * .005);
    avail10MBs = 2500;
  }
  // 1% of space between 7GB -> 25 GB
  if (avail10MBs > 700) {
    sz10MBs += static_cast<uint32_t>((avail10MBs - 700) * .01);
    avail10MBs = 700;
  }
  // 5% of space between 500 MB -> 7 GB
  if (avail10MBs > 50) {
    sz10MBs += static_cast<uint32_t>((avail10MBs - 50) * .05);
    avail10MBs = 50;
  }

#ifdef ANDROID
  // On Android, smaller/older devices may have very little storage and
  // device owners may be sensitive to storage footprint: Use a smaller
  // percentage of available space and a smaller minimum.

  // 20% of space up to 500 MB (10 MB min)
  sz10MBs += std::max<uint32_t>(1, static_cast<uint32_t>(avail10MBs * .2));
#else
  // 40% of space up to 500 MB (50 MB min)
  sz10MBs += std::max<uint32_t>(5, static_cast<uint32_t>(avail10MBs * .4));
#endif

  return std::min<uint32_t>(maxSize, sz10MBs * 10 * 1024);
}

/* Computes our best guess for the default size of the user's disk cache,
 * based on the amount of space they have free on their hard drive.
 * We use a tiered scheme: the more space available,
 * the larger the disk cache will be. However, we do not want
 * to enable the disk cache to grow to an unbounded size, so the larger the
 * user's available space is, the smaller of a percentage we take. We set a
 * lower bound of 50MB and an upper bound of 1GB.
 *
 *@param:  None.
 *@return: The size that the user's disk cache should default to, in kBytes.
 */
uint32_t nsCacheProfilePrefObserver::GetSmartCacheSize(
    const nsAString &cachePath, uint32_t currentSize,
    bool shouldUseOldMaxSmartSize) {
  // Check for free space on device where cache directory lives
  nsresult rv;
  nsCOMPtr<nsIFile> cacheDirectory(
      do_CreateInstance(NS_LOCAL_FILE_CONTRACTID, &rv));
  if (NS_FAILED(rv) || !cacheDirectory) return DEFAULT_CACHE_SIZE;
  rv = cacheDirectory->InitWithPath(cachePath);
  if (NS_FAILED(rv)) return DEFAULT_CACHE_SIZE;
  int64_t bytesAvailable;
  rv = cacheDirectory->GetDiskSpaceAvailable(&bytesAvailable);
  if (NS_FAILED(rv)) return DEFAULT_CACHE_SIZE;

  return SmartCacheSize(
      static_cast<uint32_t>((bytesAvailable / 1024) + currentSize),
      shouldUseOldMaxSmartSize);
}

/* Determine if we are permitted to dynamically size the user's disk cache based
 * on their disk space available. We may do this so long as the pref
 * smart_size.enabled is true.
 */
bool nsCacheProfilePrefObserver::PermittedToSmartSize(nsIPrefBranch *branch,
                                                      bool firstRun) {
  nsresult rv;
  if (firstRun) {
    // check if user has set cache size in the past
    bool userSet;
    rv = branch->PrefHasUserValue(DISK_CACHE_CAPACITY_PREF, &userSet);
    if (NS_FAILED(rv)) userSet = true;
    if (userSet) {
      int32_t oldCapacity;
      // If user explicitly set cache size to be smaller than old default
      // of 50 MB, then keep user's value. Otherwise use smart sizing.
      rv = branch->GetIntPref(DISK_CACHE_CAPACITY_PREF, &oldCapacity);
      if (oldCapacity < PRE_GECKO_2_0_DEFAULT_CACHE_SIZE) {
        mSmartSizeEnabled = false;
        branch->SetBoolPref(DISK_CACHE_SMART_SIZE_ENABLED_PREF,
                            mSmartSizeEnabled);
        return mSmartSizeEnabled;
      }
||||||| merged common ancestors
// Returns default ("smart") size (in KB) of cache, given available disk space
// (also in KB)
static uint32_t
SmartCacheSize(const uint32_t availKB, bool shouldUseOldMaxSmartSize)
{
    uint32_t maxSize = shouldUseOldMaxSmartSize ? OLD_MAX_CACHE_SIZE : MAX_CACHE_SIZE;

    if (availKB > 100 * 1024 * 1024)
        return maxSize;  // skip computing if we're over 100 GB

    // Grow/shrink in 10 MB units, deliberately, so that in the common case we
    // don't shrink cache and evict items every time we startup (it's important
    // that we don't slow down startup benchmarks).
    uint32_t sz10MBs = 0;
    uint32_t avail10MBs = availKB / (1024*10);

    // .5% of space above 25 GB
    if (avail10MBs > 2500) {
        sz10MBs += static_cast<uint32_t>((avail10MBs - 2500)*.005);
        avail10MBs = 2500;
    }
    // 1% of space between 7GB -> 25 GB
    if (avail10MBs > 700) {
        sz10MBs += static_cast<uint32_t>((avail10MBs - 700)*.01);
        avail10MBs = 700;
    }
    // 5% of space between 500 MB -> 7 GB
    if (avail10MBs > 50) {
        sz10MBs += static_cast<uint32_t>((avail10MBs - 50)*.05);
        avail10MBs = 50;
    }

#ifdef ANDROID
    // On Android, smaller/older devices may have very little storage and
    // device owners may be sensitive to storage footprint: Use a smaller
    // percentage of available space and a smaller minimum.

    // 20% of space up to 500 MB (10 MB min)
    sz10MBs += std::max<uint32_t>(1, static_cast<uint32_t>(avail10MBs * .2));
#else
    // 40% of space up to 500 MB (50 MB min)
    sz10MBs += std::max<uint32_t>(5, static_cast<uint32_t>(avail10MBs * .4));
#endif

    return std::min<uint32_t>(maxSize, sz10MBs * 10 * 1024);
}

 /* Computes our best guess for the default size of the user's disk cache,
  * based on the amount of space they have free on their hard drive.
  * We use a tiered scheme: the more space available,
  * the larger the disk cache will be. However, we do not want
  * to enable the disk cache to grow to an unbounded size, so the larger the
  * user's available space is, the smaller of a percentage we take. We set a
  * lower bound of 50MB and an upper bound of 1GB.
  *
  *@param:  None.
  *@return: The size that the user's disk cache should default to, in kBytes.
  */
uint32_t
nsCacheProfilePrefObserver::GetSmartCacheSize(const nsAString& cachePath,
                                              uint32_t currentSize,
                                              bool shouldUseOldMaxSmartSize)
{
    // Check for free space on device where cache directory lives
    nsresult rv;
    nsCOMPtr<nsIFile>
        cacheDirectory (do_CreateInstance(NS_LOCAL_FILE_CONTRACTID, &rv));
    if (NS_FAILED(rv) || !cacheDirectory)
        return DEFAULT_CACHE_SIZE;
    rv = cacheDirectory->InitWithPath(cachePath);
    if (NS_FAILED(rv))
        return DEFAULT_CACHE_SIZE;
    int64_t bytesAvailable;
    rv = cacheDirectory->GetDiskSpaceAvailable(&bytesAvailable);
    if (NS_FAILED(rv))
        return DEFAULT_CACHE_SIZE;

    return SmartCacheSize(static_cast<uint32_t>((bytesAvailable / 1024) +
                                                currentSize),
                          shouldUseOldMaxSmartSize);
}

/* Determine if we are permitted to dynamically size the user's disk cache based
 * on their disk space available. We may do this so long as the pref
 * smart_size.enabled is true.
 */
bool
nsCacheProfilePrefObserver::PermittedToSmartSize(nsIPrefBranch* branch, bool
                                                 firstRun)
{
    nsresult rv;
    if (firstRun) {
        // check if user has set cache size in the past
        bool userSet;
        rv = branch->PrefHasUserValue(DISK_CACHE_CAPACITY_PREF, &userSet);
        if (NS_FAILED(rv)) userSet = true;
        if (userSet) {
            int32_t oldCapacity;
            // If user explicitly set cache size to be smaller than old default
            // of 50 MB, then keep user's value. Otherwise use smart sizing.
            rv = branch->GetIntPref(DISK_CACHE_CAPACITY_PREF, &oldCapacity);
            if (oldCapacity < PRE_GECKO_2_0_DEFAULT_CACHE_SIZE) {
                mSmartSizeEnabled = false;
                branch->SetBoolPref(DISK_CACHE_SMART_SIZE_ENABLED_PREF,
                                    mSmartSizeEnabled);
                return mSmartSizeEnabled;
            }
        }
        // Set manual setting to MAX cache size as starting val for any
        // adjustment by user: (bug 559942 comment 65)
        int32_t maxSize = mShouldUseOldMaxSmartSize ? OLD_MAX_CACHE_SIZE : MAX_CACHE_SIZE;
        branch->SetIntPref(DISK_CACHE_CAPACITY_PREF, maxSize);
=======
nsresult nsCacheProfilePrefObserver::ReadPrefs(nsIPrefBranch* branch) {
  nsresult rv = NS_OK;

  if (!mDiskCacheParentDirectory) {
    nsCOMPtr<nsIFile> directory;

    // try to get the disk cache parent directory
    rv = NS_GetSpecialDirectory(NS_APP_CACHE_PARENT_DIR,
                                getter_AddRefs(directory));
    if (NS_FAILED(rv)) {
      // try to get the profile directory (there may not be a profile yet)
      nsCOMPtr<nsIFile> profDir;
      NS_GetSpecialDirectory(NS_APP_USER_PROFILE_50_DIR,
                             getter_AddRefs(profDir));
      NS_GetSpecialDirectory(NS_APP_USER_PROFILE_LOCAL_50_DIR,
                             getter_AddRefs(directory));
      if (!directory)
        directory = profDir;
      else if (profDir) {
        nsCacheService::MoveOrRemoveDiskCache(profDir, directory, "Cache");
      }
    }
    // use file cache in build tree only if asked, to avoid cache dir litter
    if (!directory && PR_GetEnv("NECKO_DEV_ENABLE_DISK_CACHE")) {
      rv = NS_GetSpecialDirectory(NS_XPCOM_CURRENT_PROCESS_DIR,
                                  getter_AddRefs(directory));
    }
    if (directory) {
      mDiskCacheParentDirectory = directory;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    // Set manual setting to MAX cache size as starting val for any
    // adjustment by user: (bug 559942 comment 65)
    int32_t maxSize =
        mShouldUseOldMaxSmartSize ? OLD_MAX_CACHE_SIZE : MAX_CACHE_SIZE;
    branch->SetIntPref(DISK_CACHE_CAPACITY_PREF, maxSize);
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  rv = branch->GetBoolPref(DISK_CACHE_SMART_SIZE_ENABLED_PREF,
                           &mSmartSizeEnabled);
  if (NS_FAILED(rv)) mSmartSizeEnabled = false;
  return mSmartSizeEnabled;
}

nsresult nsCacheProfilePrefObserver::ReadPrefs(nsIPrefBranch *branch) {
  nsresult rv = NS_OK;

  // read disk cache device prefs
  mDiskCacheEnabled = true;  // presume disk cache is enabled
  (void)branch->GetBoolPref(DISK_CACHE_ENABLE_PREF, &mDiskCacheEnabled);

  mDiskCacheCapacity = DISK_CACHE_CAPACITY;
  (void)branch->GetIntPref(DISK_CACHE_CAPACITY_PREF, &mDiskCacheCapacity);
  mDiskCacheCapacity = std::max(0, mDiskCacheCapacity);

  (void)branch->GetIntPref(DISK_CACHE_MAX_ENTRY_SIZE_PREF,
                           &mDiskCacheMaxEntrySize);
  mDiskCacheMaxEntrySize = std::max(-1, mDiskCacheMaxEntrySize);

  (void)branch->GetComplexValue(DISK_CACHE_DIR_PREF,  // ignore error
                                NS_GET_IID(nsIFile),
                                getter_AddRefs(mDiskCacheParentDirectory));

  (void)branch->GetBoolPref(DISK_CACHE_USE_OLD_MAX_SMART_SIZE_PREF,
                            &mShouldUseOldMaxSmartSize);
||||||| merged common ancestors
    rv = branch->GetBoolPref(DISK_CACHE_SMART_SIZE_ENABLED_PREF,
                             &mSmartSizeEnabled);
    if (NS_FAILED(rv))
        mSmartSizeEnabled = false;
    return mSmartSizeEnabled;
}


nsresult
nsCacheProfilePrefObserver::ReadPrefs(nsIPrefBranch* branch)
{
    nsresult rv = NS_OK;

    // read disk cache device prefs
    mDiskCacheEnabled = true;  // presume disk cache is enabled
    (void) branch->GetBoolPref(DISK_CACHE_ENABLE_PREF, &mDiskCacheEnabled);

    mDiskCacheCapacity = DISK_CACHE_CAPACITY;
    (void)branch->GetIntPref(DISK_CACHE_CAPACITY_PREF, &mDiskCacheCapacity);
    mDiskCacheCapacity = std::max(0, mDiskCacheCapacity);

    (void) branch->GetIntPref(DISK_CACHE_MAX_ENTRY_SIZE_PREF,
                              &mDiskCacheMaxEntrySize);
    mDiskCacheMaxEntrySize = std::max(-1, mDiskCacheMaxEntrySize);

    (void) branch->GetComplexValue(DISK_CACHE_DIR_PREF,     // ignore error
                                   NS_GET_IID(nsIFile),
                                   getter_AddRefs(mDiskCacheParentDirectory));
=======
  // read offline cache device prefs
  mOfflineCacheEnabled = true;  // presume offline cache is enabled
  (void)branch->GetBoolPref(OFFLINE_CACHE_ENABLE_PREF, &mOfflineCacheEnabled);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!mDiskCacheParentDirectory) {
    nsCOMPtr<nsIFile> directory;
||||||| merged common ancestors
    (void) branch->GetBoolPref(DISK_CACHE_USE_OLD_MAX_SMART_SIZE_PREF,
                               &mShouldUseOldMaxSmartSize);

    if (!mDiskCacheParentDirectory) {
        nsCOMPtr<nsIFile>  directory;
=======
  mOfflineCacheCapacity = OFFLINE_CACHE_CAPACITY;
  (void)branch->GetIntPref(OFFLINE_CACHE_CAPACITY_PREF, &mOfflineCacheCapacity);
  mOfflineCacheCapacity = std::max(0, mOfflineCacheCapacity);

  (void)branch->GetComplexValue(OFFLINE_CACHE_DIR_PREF,  // ignore error
                                NS_GET_IID(nsIFile),
                                getter_AddRefs(mOfflineCacheParentDirectory));
>>>>>>> upstream-releases

<<<<<<< HEAD
    // try to get the disk cache parent directory
    rv = NS_GetSpecialDirectory(NS_APP_CACHE_PARENT_DIR,
                                getter_AddRefs(directory));
    if (NS_FAILED(rv)) {
      // try to get the profile directory (there may not be a profile yet)
      nsCOMPtr<nsIFile> profDir;
      NS_GetSpecialDirectory(NS_APP_USER_PROFILE_50_DIR,
                             getter_AddRefs(profDir));
      NS_GetSpecialDirectory(NS_APP_USER_PROFILE_LOCAL_50_DIR,
                             getter_AddRefs(directory));
      if (!directory)
        directory = profDir;
      else if (profDir) {
        nsCacheService::MoveOrRemoveDiskCache(profDir, directory, "Cache");
      }
    }
    // use file cache in build tree only if asked, to avoid cache dir litter
    if (!directory && PR_GetEnv("NECKO_DEV_ENABLE_DISK_CACHE")) {
      rv = NS_GetSpecialDirectory(NS_XPCOM_CURRENT_PROCESS_DIR,
                                  getter_AddRefs(directory));
||||||| merged common ancestors
        // try to get the disk cache parent directory
        rv = NS_GetSpecialDirectory(NS_APP_CACHE_PARENT_DIR,
                                    getter_AddRefs(directory));
        if (NS_FAILED(rv)) {
            // try to get the profile directory (there may not be a profile yet)
            nsCOMPtr<nsIFile> profDir;
            NS_GetSpecialDirectory(NS_APP_USER_PROFILE_50_DIR,
                                   getter_AddRefs(profDir));
            NS_GetSpecialDirectory(NS_APP_USER_PROFILE_LOCAL_50_DIR,
                                   getter_AddRefs(directory));
            if (!directory)
                directory = profDir;
            else if (profDir) {
                nsCacheService::MoveOrRemoveDiskCache(profDir, directory,
                                                      "Cache");
            }
        }
        // use file cache in build tree only if asked, to avoid cache dir litter
        if (!directory && PR_GetEnv("NECKO_DEV_ENABLE_DISK_CACHE")) {
            rv = NS_GetSpecialDirectory(NS_XPCOM_CURRENT_PROCESS_DIR,
                                        getter_AddRefs(directory));
        }
        if (directory) {
            mDiskCacheParentDirectory = directory;
        }
    }
    if (mDiskCacheParentDirectory) {
        bool firstSmartSizeRun;
        rv = branch->GetBoolPref(DISK_CACHE_SMART_SIZE_FIRST_RUN_PREF,
                                 &firstSmartSizeRun);
        if (NS_FAILED(rv))
            firstSmartSizeRun = false;
        if (PermittedToSmartSize(branch, firstSmartSizeRun)) {
            // Avoid evictions: use previous cache size until smart size event
            // updates mDiskCacheCapacity
            rv = branch->GetIntPref(firstSmartSizeRun ?
                                    DISK_CACHE_CAPACITY_PREF :
                                    DISK_CACHE_SMART_SIZE_PREF,
                                    &mDiskCacheCapacity);
            if (NS_FAILED(rv))
                mDiskCacheCapacity = DEFAULT_CACHE_SIZE;
        }

        if (firstSmartSizeRun) {
            // It is no longer our first run
            rv = branch->SetBoolPref(DISK_CACHE_SMART_SIZE_FIRST_RUN_PREF,
                                     false);
            if (NS_FAILED(rv))
                NS_WARNING("Failed setting first_run pref in ReadPrefs.");
        }
=======
  if (!mOfflineCacheParentDirectory) {
    nsCOMPtr<nsIFile> directory;

    // try to get the offline cache parent directory
    rv = NS_GetSpecialDirectory(NS_APP_CACHE_PARENT_DIR,
                                getter_AddRefs(directory));
    if (NS_FAILED(rv)) {
      // try to get the profile directory (there may not be a profile yet)
      nsCOMPtr<nsIFile> profDir;
      NS_GetSpecialDirectory(NS_APP_USER_PROFILE_50_DIR,
                             getter_AddRefs(profDir));
      NS_GetSpecialDirectory(NS_APP_USER_PROFILE_LOCAL_50_DIR,
                             getter_AddRefs(directory));
      if (!directory)
        directory = profDir;
      else if (profDir) {
        nsCacheService::MoveOrRemoveDiskCache(profDir, directory,
                                              "OfflineCache");
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (directory) {
      mDiskCacheParentDirectory = directory;
    }
  }
  if (mDiskCacheParentDirectory) {
    bool firstSmartSizeRun;
    rv = branch->GetBoolPref(DISK_CACHE_SMART_SIZE_FIRST_RUN_PREF,
                             &firstSmartSizeRun);
    if (NS_FAILED(rv)) firstSmartSizeRun = false;
    if (PermittedToSmartSize(branch, firstSmartSizeRun)) {
      // Avoid evictions: use previous cache size until smart size event
      // updates mDiskCacheCapacity
      rv = branch->GetIntPref(firstSmartSizeRun ? DISK_CACHE_CAPACITY_PREF
                                                : DISK_CACHE_SMART_SIZE_PREF,
                              &mDiskCacheCapacity);
      if (NS_FAILED(rv)) mDiskCacheCapacity = DEFAULT_CACHE_SIZE;
    }

    if (firstSmartSizeRun) {
      // It is no longer our first run
      rv = branch->SetBoolPref(DISK_CACHE_SMART_SIZE_FIRST_RUN_PREF, false);
      if (NS_FAILED(rv))
        NS_WARNING("Failed setting first_run pref in ReadPrefs.");
    }
  }

  // read offline cache device prefs
  mOfflineCacheEnabled = true;  // presume offline cache is enabled
  (void)branch->GetBoolPref(OFFLINE_CACHE_ENABLE_PREF, &mOfflineCacheEnabled);

  mOfflineCacheCapacity = OFFLINE_CACHE_CAPACITY;
  (void)branch->GetIntPref(OFFLINE_CACHE_CAPACITY_PREF, &mOfflineCacheCapacity);
  mOfflineCacheCapacity = std::max(0, mOfflineCacheCapacity);

  (void)branch->GetComplexValue(OFFLINE_CACHE_DIR_PREF,  // ignore error
                                NS_GET_IID(nsIFile),
                                getter_AddRefs(mOfflineCacheParentDirectory));

  if (!mOfflineCacheParentDirectory) {
    nsCOMPtr<nsIFile> directory;

    // try to get the offline cache parent directory
    rv = NS_GetSpecialDirectory(NS_APP_CACHE_PARENT_DIR,
                                getter_AddRefs(directory));
    if (NS_FAILED(rv)) {
      // try to get the profile directory (there may not be a profile yet)
      nsCOMPtr<nsIFile> profDir;
      NS_GetSpecialDirectory(NS_APP_USER_PROFILE_50_DIR,
                             getter_AddRefs(profDir));
      NS_GetSpecialDirectory(NS_APP_USER_PROFILE_LOCAL_50_DIR,
                             getter_AddRefs(directory));
      if (!directory)
        directory = profDir;
      else if (profDir) {
        nsCacheService::MoveOrRemoveDiskCache(profDir, directory,
                                              "OfflineCache");
      }
    }
||||||| merged common ancestors

    // read offline cache device prefs
    mOfflineCacheEnabled = true;  // presume offline cache is enabled
    (void) branch->GetBoolPref(OFFLINE_CACHE_ENABLE_PREF,
                              &mOfflineCacheEnabled);

    mOfflineCacheCapacity = OFFLINE_CACHE_CAPACITY;
    (void)branch->GetIntPref(OFFLINE_CACHE_CAPACITY_PREF,
                             &mOfflineCacheCapacity);
    mOfflineCacheCapacity = std::max(0, mOfflineCacheCapacity);

    (void) branch->GetComplexValue(OFFLINE_CACHE_DIR_PREF,     // ignore error
                                   NS_GET_IID(nsIFile),
                                   getter_AddRefs(mOfflineCacheParentDirectory));

    if (!mOfflineCacheParentDirectory) {
        nsCOMPtr<nsIFile>  directory;

        // try to get the offline cache parent directory
        rv = NS_GetSpecialDirectory(NS_APP_CACHE_PARENT_DIR,
                                    getter_AddRefs(directory));
        if (NS_FAILED(rv)) {
            // try to get the profile directory (there may not be a profile yet)
            nsCOMPtr<nsIFile> profDir;
            NS_GetSpecialDirectory(NS_APP_USER_PROFILE_50_DIR,
                                   getter_AddRefs(profDir));
            NS_GetSpecialDirectory(NS_APP_USER_PROFILE_LOCAL_50_DIR,
                                   getter_AddRefs(directory));
            if (!directory)
                directory = profDir;
            else if (profDir) {
                nsCacheService::MoveOrRemoveDiskCache(profDir, directory,
                                                      "OfflineCache");
            }
        }
=======
>>>>>>> upstream-releases
#if DEBUG
<<<<<<< HEAD
    if (!directory) {
      // use current process directory during development
      rv = NS_GetSpecialDirectory(NS_XPCOM_CURRENT_PROCESS_DIR,
                                  getter_AddRefs(directory));
    }
#endif
    if (directory) {
      mOfflineCacheParentDirectory = directory;
||||||| merged common ancestors
        if (!directory) {
            // use current process directory during development
            rv = NS_GetSpecialDirectory(NS_XPCOM_CURRENT_PROCESS_DIR,
                                        getter_AddRefs(directory));
        }
#endif
        if (directory) {
            mOfflineCacheParentDirectory = directory;
        }
=======
    if (!directory) {
      // use current process directory during development
      rv = NS_GetSpecialDirectory(NS_XPCOM_CURRENT_PROCESS_DIR,
                                  getter_AddRefs(directory));
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

  // read memory cache device prefs
  (void)branch->GetBoolPref(MEMORY_CACHE_ENABLE_PREF, &mMemoryCacheEnabled);

  mMemoryCacheCapacity = -1;
  (void)branch->GetIntPref(MEMORY_CACHE_CAPACITY_PREF, &mMemoryCacheCapacity);

  (void)branch->GetIntPref(MEMORY_CACHE_MAX_ENTRY_SIZE_PREF,
                           &mMemoryCacheMaxEntrySize);
  mMemoryCacheMaxEntrySize = std::max(-1, mMemoryCacheMaxEntrySize);

  // read cache compression level pref
  mCacheCompressionLevel = CACHE_COMPRESSION_LEVEL;
  (void)branch->GetIntPref(CACHE_COMPRESSION_LEVEL_PREF,
                           &mCacheCompressionLevel);
  mCacheCompressionLevel = std::max(0, mCacheCompressionLevel);
  mCacheCompressionLevel = std::min(9, mCacheCompressionLevel);

  // read cache shutdown sanitization prefs
  (void)branch->GetBoolPref(SANITIZE_ON_SHUTDOWN_PREF, &mSanitizeOnShutdown);
  (void)branch->GetBoolPref(CLEAR_ON_SHUTDOWN_PREF, &mClearCacheOnShutdown);

  return rv;
}

nsresult nsCacheService::DispatchToCacheIOThread(nsIRunnable *event) {
  if (!gService || !gService->mCacheIOThread) return NS_ERROR_NOT_AVAILABLE;
  return gService->mCacheIOThread->Dispatch(event, NS_DISPATCH_NORMAL);
}

nsresult nsCacheService::SyncWithCacheIOThread() {
  if (!gService || !gService->mCacheIOThread) return NS_ERROR_NOT_AVAILABLE;
  gService->mLock.AssertCurrentThreadOwns();

  nsCOMPtr<nsIRunnable> event = new nsBlockOnCacheThreadEvent();

  // dispatch event - it will notify the monitor when it's done
  nsresult rv = gService->mCacheIOThread->Dispatch(event, NS_DISPATCH_NORMAL);
  if (NS_FAILED(rv)) {
    NS_WARNING("Failed dispatching block-event");
    return NS_ERROR_UNEXPECTED;
  }

  // wait until notified, then return
  gService->mNotified = false;
  while (!gService->mNotified) {
    gService->mCondVar.Wait();
  }
||||||| merged common ancestors

    // read memory cache device prefs
    (void) branch->GetBoolPref(MEMORY_CACHE_ENABLE_PREF, &mMemoryCacheEnabled);

    mMemoryCacheCapacity = -1;
    (void) branch->GetIntPref(MEMORY_CACHE_CAPACITY_PREF,
                              &mMemoryCacheCapacity);

    (void) branch->GetIntPref(MEMORY_CACHE_MAX_ENTRY_SIZE_PREF,
                              &mMemoryCacheMaxEntrySize);
    mMemoryCacheMaxEntrySize = std::max(-1, mMemoryCacheMaxEntrySize);

    // read cache compression level pref
    mCacheCompressionLevel = CACHE_COMPRESSION_LEVEL;
    (void)branch->GetIntPref(CACHE_COMPRESSION_LEVEL_PREF,
                             &mCacheCompressionLevel);
    mCacheCompressionLevel = std::max(0, mCacheCompressionLevel);
    mCacheCompressionLevel = std::min(9, mCacheCompressionLevel);

    // read cache shutdown sanitization prefs
    (void) branch->GetBoolPref(SANITIZE_ON_SHUTDOWN_PREF,
                               &mSanitizeOnShutdown);
    (void) branch->GetBoolPref(CLEAR_ON_SHUTDOWN_PREF,
                               &mClearCacheOnShutdown);

    return rv;
}

nsresult
nsCacheService::DispatchToCacheIOThread(nsIRunnable* event)
{
    if (!gService || !gService->mCacheIOThread) return NS_ERROR_NOT_AVAILABLE;
    return gService->mCacheIOThread->Dispatch(event, NS_DISPATCH_NORMAL);
}

nsresult
nsCacheService::SyncWithCacheIOThread()
{
    if (!gService || !gService->mCacheIOThread) return NS_ERROR_NOT_AVAILABLE;
    gService->mLock.AssertCurrentThreadOwns();

    nsCOMPtr<nsIRunnable> event = new nsBlockOnCacheThreadEvent();

    // dispatch event - it will notify the monitor when it's done
    nsresult rv =
        gService->mCacheIOThread->Dispatch(event, NS_DISPATCH_NORMAL);
    if (NS_FAILED(rv)) {
        NS_WARNING("Failed dispatching block-event");
        return NS_ERROR_UNEXPECTED;
    }

    // wait until notified, then return
    gService->mNotified = false;
    while (!gService->mNotified) {
      gService->mCondVar.Wait();
    }
=======
#endif
    if (directory) {
      mOfflineCacheParentDirectory = directory;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return NS_OK;
||||||| merged common ancestors
    return NS_OK;
=======
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool nsCacheProfilePrefObserver::DiskCacheEnabled() {
  if ((mDiskCacheCapacity == 0) || (!mDiskCacheParentDirectory)) return false;
  return mDiskCacheEnabled && (!mSanitizeOnShutdown || !mClearCacheOnShutdown);
||||||| merged common ancestors

bool
nsCacheProfilePrefObserver::DiskCacheEnabled()
{
    if ((mDiskCacheCapacity == 0) || (!mDiskCacheParentDirectory))  return false;
    return mDiskCacheEnabled && (!mSanitizeOnShutdown || !mClearCacheOnShutdown);
=======
nsresult nsCacheService::DispatchToCacheIOThread(nsIRunnable* event) {
  if (!gService || !gService->mCacheIOThread) return NS_ERROR_NOT_AVAILABLE;
  return gService->mCacheIOThread->Dispatch(event, NS_DISPATCH_NORMAL);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool nsCacheProfilePrefObserver::OfflineCacheEnabled() {
  if ((mOfflineCacheCapacity == 0) || (!mOfflineCacheParentDirectory))
    return false;
||||||| merged common ancestors
=======
nsresult nsCacheService::SyncWithCacheIOThread() {
  if (!gService || !gService->mCacheIOThread) return NS_ERROR_NOT_AVAILABLE;
  gService->mLock.AssertCurrentThreadOwns();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return mOfflineCacheEnabled;
}
||||||| merged common ancestors
bool
nsCacheProfilePrefObserver::OfflineCacheEnabled()
{
    if ((mOfflineCacheCapacity == 0) || (!mOfflineCacheParentDirectory))
        return false;

    return mOfflineCacheEnabled;
}
=======
  nsCOMPtr<nsIRunnable> event = new nsBlockOnCacheThreadEvent();

  // dispatch event - it will notify the monitor when it's done
  nsresult rv = gService->mCacheIOThread->Dispatch(event, NS_DISPATCH_NORMAL);
  if (NS_FAILED(rv)) {
    NS_WARNING("Failed dispatching block-event");
    return NS_ERROR_UNEXPECTED;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool nsCacheProfilePrefObserver::MemoryCacheEnabled() {
  if (mMemoryCacheCapacity == 0) return false;
  return mMemoryCacheEnabled;
||||||| merged common ancestors

bool
nsCacheProfilePrefObserver::MemoryCacheEnabled()
{
    if (mMemoryCacheCapacity == 0)  return false;
    return mMemoryCacheEnabled;
=======
  // wait until notified, then return
  gService->mNotified = false;
  while (!gService->mNotified) {
    gService->mCondVar.Wait();
  }

  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/**
 * MemoryCacheCapacity
 *
 * If the browser.cache.memory.capacity preference is positive, we use that
 * value for the amount of memory available for the cache.
 *
 * If browser.cache.memory.capacity is zero, the memory cache is disabled.
 *
 * If browser.cache.memory.capacity is negative or not present, we use a
 * formula that grows less than linearly with the amount of system memory,
 * with an upper limit on the cache size. No matter how much physical RAM is
 * present, the default cache size would not exceed 32 MB. This maximum would
 * apply only to systems with more than 4 GB of RAM (e.g. terminal servers)
 *
 *   RAM   Cache
 *   ---   -----
 *   32 Mb   2 Mb
 *   64 Mb   4 Mb
 *  128 Mb   6 Mb
 *  256 Mb  10 Mb
 *  512 Mb  14 Mb
 * 1024 Mb  18 Mb
 * 2048 Mb  24 Mb
 * 4096 Mb  30 Mb
 *
 * The equation for this is (for cache size C and memory size K (kbytes)):
 *  x = log2(K) - 14
 *  C = x^2/3 + x + 2/3 + 0.1 (0.1 for rounding)
 *  if (C > 32) C = 32
 */

int32_t nsCacheProfilePrefObserver::MemoryCacheCapacity() {
  int32_t capacity = mMemoryCacheCapacity;
  if (capacity >= 0) {
    CACHE_LOG_DEBUG(("Memory cache capacity forced to %d\n", capacity));
    return capacity;
  }

  static uint64_t bytes = PR_GetPhysicalMemorySize();
  CACHE_LOG_DEBUG(("Physical Memory size is %" PRIu64 "\n", bytes));

  // If getting the physical memory failed, arbitrarily assume
  // 32 MB of RAM. We use a low default to have a reasonable
  // size on all the devices we support.
  if (bytes == 0) bytes = 32 * 1024 * 1024;

  // Conversion from unsigned int64_t to double doesn't work on all platforms.
  // We need to truncate the value at INT64_MAX to make sure we don't
  // overflow.
  if (bytes > INT64_MAX) bytes = INT64_MAX;

  uint64_t kbytes = bytes >> 10;

  double kBytesD = double(kbytes);

  double x = log(kBytesD) / log(2.0) - 14;
  if (x > 0) {
    capacity = (int32_t)(x * x / 3.0 + x + 2.0 / 3 + 0.1);  // 0.1 for rounding
    if (capacity > 32) capacity = 32;
    capacity *= 1024;
  } else {
    capacity = 0;
  }

  return capacity;
||||||| merged common ancestors

/**
 * MemoryCacheCapacity
 *
 * If the browser.cache.memory.capacity preference is positive, we use that
 * value for the amount of memory available for the cache.
 *
 * If browser.cache.memory.capacity is zero, the memory cache is disabled.
 *
 * If browser.cache.memory.capacity is negative or not present, we use a
 * formula that grows less than linearly with the amount of system memory,
 * with an upper limit on the cache size. No matter how much physical RAM is
 * present, the default cache size would not exceed 32 MB. This maximum would
 * apply only to systems with more than 4 GB of RAM (e.g. terminal servers)
 *
 *   RAM   Cache
 *   ---   -----
 *   32 Mb   2 Mb
 *   64 Mb   4 Mb
 *  128 Mb   6 Mb
 *  256 Mb  10 Mb
 *  512 Mb  14 Mb
 * 1024 Mb  18 Mb
 * 2048 Mb  24 Mb
 * 4096 Mb  30 Mb
 *
 * The equation for this is (for cache size C and memory size K (kbytes)):
 *  x = log2(K) - 14
 *  C = x^2/3 + x + 2/3 + 0.1 (0.1 for rounding)
 *  if (C > 32) C = 32
 */

int32_t
nsCacheProfilePrefObserver::MemoryCacheCapacity()
{
    int32_t capacity = mMemoryCacheCapacity;
    if (capacity >= 0) {
        CACHE_LOG_DEBUG(("Memory cache capacity forced to %d\n", capacity));
        return capacity;
    }

    static uint64_t bytes = PR_GetPhysicalMemorySize();
    CACHE_LOG_DEBUG(("Physical Memory size is %" PRIu64 "\n", bytes));

    // If getting the physical memory failed, arbitrarily assume
    // 32 MB of RAM. We use a low default to have a reasonable
    // size on all the devices we support.
    if (bytes == 0)
        bytes = 32 * 1024 * 1024;

    // Conversion from unsigned int64_t to double doesn't work on all platforms.
    // We need to truncate the value at INT64_MAX to make sure we don't
    // overflow.
    if (bytes > INT64_MAX)
        bytes = INT64_MAX;

    uint64_t kbytes = bytes >> 10;

    double kBytesD = double(kbytes);

    double x = log(kBytesD)/log(2.0) - 14;
    if (x > 0) {
        capacity = (int32_t)(x * x / 3.0 + x + 2.0 / 3 + 0.1); // 0.1 for rounding
        if (capacity > 32)
            capacity = 32;
        capacity   *= 1024;
    } else {
        capacity    = 0;
    }

    return capacity;
=======
bool nsCacheProfilePrefObserver::OfflineCacheEnabled() {
  if ((mOfflineCacheCapacity == 0) || (!mOfflineCacheParentDirectory))
    return false;

  return mOfflineCacheEnabled;
>>>>>>> upstream-releases
}

int32_t nsCacheProfilePrefObserver::CacheCompressionLevel() {
  return mCacheCompressionLevel;
}

/******************************************************************************
 * nsProcessRequestEvent
 *****************************************************************************/

class nsProcessRequestEvent : public Runnable {
<<<<<<< HEAD
 public:
  explicit nsProcessRequestEvent(nsCacheRequest *aRequest)
      : mozilla::Runnable("nsProcessRequestEvent") {
||||||| merged common ancestors
public:
  explicit nsProcessRequestEvent(nsCacheRequest* aRequest)
    : mozilla::Runnable("nsProcessRequestEvent")
  {
=======
 public:
  explicit nsProcessRequestEvent(nsCacheRequest* aRequest)
      : mozilla::Runnable("nsProcessRequestEvent") {
>>>>>>> upstream-releases
    mRequest = aRequest;
  }

  NS_IMETHOD Run() override {
    nsresult rv;

    NS_ASSERTION(mRequest->mListener,
                 "Sync OpenCacheEntry() posted to background thread!");

    nsCacheServiceAutoLock lock(LOCK_TELEM(NSPROCESSREQUESTEVENT_RUN));
    rv = nsCacheService::gService->ProcessRequest(mRequest, false, nullptr);

    // Don't delete the request if it was queued
    if (!(mRequest->IsBlocking() && rv == NS_ERROR_CACHE_WAIT_FOR_VALIDATION))
      delete mRequest;

    return NS_OK;
  }

 protected:
  virtual ~nsProcessRequestEvent() = default;

<<<<<<< HEAD
 private:
  nsCacheRequest *mRequest;
||||||| merged common ancestors
private:
    nsCacheRequest *mRequest;
=======
 private:
  nsCacheRequest* mRequest;
>>>>>>> upstream-releases
};

/******************************************************************************
 * nsDoomEvent
 *****************************************************************************/

class nsDoomEvent : public Runnable {
<<<<<<< HEAD
 public:
  nsDoomEvent(nsCacheSession *session, const nsACString &key,
              nsICacheListener *listener)
      : mozilla::Runnable("nsDoomEvent") {
    mKey = *session->ClientID();
    mKey.Append(':');
    mKey.Append(key);
    mStoragePolicy = session->StoragePolicy();
    mListener = listener;
    mEventTarget = GetCurrentThreadEventTarget();
    // We addref the listener here and release it in nsNotifyDoomListener
    // on the callers thread. If posting of nsNotifyDoomListener event fails
    // we leak the listener which is better than releasing it on a wrong
    // thread.
    NS_IF_ADDREF(mListener);
  }
||||||| merged common ancestors
public:
    nsDoomEvent(nsCacheSession *session,
                const nsACString &key,
                nsICacheListener *listener)
      : mozilla::Runnable("nsDoomEvent")
    {
        mKey = *session->ClientID();
        mKey.Append(':');
        mKey.Append(key);
        mStoragePolicy = session->StoragePolicy();
        mListener = listener;
        mEventTarget = GetCurrentThreadEventTarget();
        // We addref the listener here and release it in nsNotifyDoomListener
        // on the callers thread. If posting of nsNotifyDoomListener event fails
        // we leak the listener which is better than releasing it on a wrong
        // thread.
        NS_IF_ADDREF(mListener);
    }
=======
 public:
  nsDoomEvent(nsCacheSession* session, const nsACString& key,
              nsICacheListener* listener)
      : mozilla::Runnable("nsDoomEvent") {
    mKey = *session->ClientID();
    mKey.Append(':');
    mKey.Append(key);
    mStoragePolicy = session->StoragePolicy();
    mListener = listener;
    mEventTarget = GetCurrentThreadEventTarget();
    // We addref the listener here and release it in nsNotifyDoomListener
    // on the callers thread. If posting of nsNotifyDoomListener event fails
    // we leak the listener which is better than releasing it on a wrong
    // thread.
    NS_IF_ADDREF(mListener);
  }
>>>>>>> upstream-releases

  NS_IMETHOD Run() override {
    nsCacheServiceAutoLock lock;

<<<<<<< HEAD
    bool foundActive = true;
    nsresult status = NS_ERROR_NOT_AVAILABLE;
    nsCacheEntry *entry;
    entry = nsCacheService::gService->mActiveEntries.GetEntry(&mKey);
    if (!entry) {
      bool collision = false;
      foundActive = false;
      entry = nsCacheService::gService->SearchCacheDevices(
          &mKey, mStoragePolicy, &collision);
    }
||||||| merged common ancestors
        if (entry) {
            status = NS_OK;
            nsCacheService::gService->DoomEntry_Internal(entry, foundActive);
        }
=======
    bool foundActive = true;
    nsresult status = NS_ERROR_NOT_AVAILABLE;
    nsCacheEntry* entry;
    entry = nsCacheService::gService->mActiveEntries.GetEntry(&mKey);
    if (!entry) {
      bool collision = false;
      foundActive = false;
      entry = nsCacheService::gService->SearchCacheDevices(
          &mKey, mStoragePolicy, &collision);
    }
>>>>>>> upstream-releases

    if (entry) {
      status = NS_OK;
      nsCacheService::gService->DoomEntry_Internal(entry, foundActive);
    }

    if (mListener) {
      mEventTarget->Dispatch(new nsNotifyDoomListener(mListener, status),
                             NS_DISPATCH_NORMAL);
      // posted event will release the reference on the correct thread
      mListener = nullptr;
    }

<<<<<<< HEAD
    return NS_OK;
  }

 private:
  nsCString mKey;
  nsCacheStoragePolicy mStoragePolicy;
  nsICacheListener *mListener;
  nsCOMPtr<nsIEventTarget> mEventTarget;
||||||| merged common ancestors
private:
    nsCString             mKey;
    nsCacheStoragePolicy  mStoragePolicy;
    nsICacheListener     *mListener;
    nsCOMPtr<nsIEventTarget> mEventTarget;
=======
    return NS_OK;
  }

 private:
  nsCString mKey;
  nsCacheStoragePolicy mStoragePolicy;
  nsICacheListener* mListener;
  nsCOMPtr<nsIEventTarget> mEventTarget;
>>>>>>> upstream-releases
};

/******************************************************************************
 * nsCacheService
 *****************************************************************************/
<<<<<<< HEAD
nsCacheService *nsCacheService::gService = nullptr;
||||||| merged common ancestors
nsCacheService *   nsCacheService::gService = nullptr;
=======
nsCacheService* nsCacheService::gService = nullptr;
>>>>>>> upstream-releases

NS_IMPL_ISUPPORTS(nsCacheService, nsICacheService, nsICacheServiceInternal)

nsCacheService::nsCacheService()
    : mObserver(nullptr),
      mLock("nsCacheService.mLock"),
      mCondVar(mLock, "nsCacheService.mCondVar"),
      mNotified(false),
      mTimeStampLock("nsCacheService.mTimeStampLock"),
      mInitialized(false),
      mClearingEntries(false),
      mEnableOfflineDevice(false),
      mOfflineDevice(nullptr),
      mDoomedEntries{},
      mTotalEntries(0),
      mCacheHits(0),
      mCacheMisses(0),
      mMaxKeyLength(0),
      mMaxDataSize(0),
      mMaxMetaSize(0),
      mDeactivateFailures(0),
      mDeactivatedUnboundEntries(0) {
  NS_ASSERTION(gService == nullptr, "multiple nsCacheService instances!");
  gService = this;

  // create list of cache devices
  PR_INIT_CLIST(&mDoomedEntries);
}

nsCacheService::~nsCacheService() {
  if (mInitialized)  // Shutdown hasn't been called yet.
    (void)Shutdown();

  if (mObserver) {
    mObserver->Remove();
    NS_RELEASE(mObserver);
  }

  gService = nullptr;
}

nsresult nsCacheService::Init() {
  // Thie method must be called on the main thread because mCacheIOThread must
  // only be modified on the main thread.
  if (!NS_IsMainThread()) {
    NS_ERROR("nsCacheService::Init called off the main thread");
    return NS_ERROR_NOT_SAME_THREAD;
  }

<<<<<<< HEAD
  NS_ASSERTION(!mInitialized, "nsCacheService already initialized.");
  if (mInitialized) return NS_ERROR_ALREADY_INITIALIZED;

  if (mozilla::net::IsNeckoChild()) {
    return NS_ERROR_UNEXPECTED;
  }
||||||| merged common ancestors
nsresult
nsCacheService::Init()
{
    // Thie method must be called on the main thread because mCacheIOThread must
    // only be modified on the main thread.
    if (!NS_IsMainThread()) {
        NS_ERROR("nsCacheService::Init called off the main thread");
        return NS_ERROR_NOT_SAME_THREAD;
    }

    NS_ASSERTION(!mInitialized, "nsCacheService already initialized.");
    if (mInitialized)
        return NS_ERROR_ALREADY_INITIALIZED;

    if (mozilla::net::IsNeckoChild()) {
        return NS_ERROR_UNEXPECTED;
    }
=======
  NS_ASSERTION(!mInitialized, "nsCacheService already initialized.");
  if (mInitialized) return NS_ERROR_ALREADY_INITIALIZED;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult rv;
||||||| merged common ancestors
    nsresult rv;
=======
  if (mozilla::net::IsNeckoChild()) {
    return NS_ERROR_UNEXPECTED;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  mStorageService = do_GetService("@mozilla.org/storage/service;1", &rv);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
    mStorageService = do_GetService("@mozilla.org/storage/service;1", &rv);
    NS_ENSURE_SUCCESS(rv, rv);
=======
  nsresult rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
  rv = NS_NewNamedThread("Cache I/O", getter_AddRefs(mCacheIOThread));
  if (NS_FAILED(rv)) {
    NS_WARNING("Can't create cache IO thread");
  }
||||||| merged common ancestors
    rv = NS_NewNamedThread("Cache I/O",
                           getter_AddRefs(mCacheIOThread));
    if (NS_FAILED(rv)) {
        MOZ_CRASH("Can't create cache IO thread");
    }
=======
  mStorageService = do_GetService("@mozilla.org/storage/service;1", &rv);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  rv = nsDeleteDir::Init();
  if (NS_FAILED(rv)) {
    NS_WARNING("Can't initialize nsDeleteDir");
  }
||||||| merged common ancestors
    rv = nsDeleteDir::Init();
    if (NS_FAILED(rv)) {
        NS_WARNING("Can't initialize nsDeleteDir");
    }
=======
  rv = NS_NewNamedThread("Cache I/O", getter_AddRefs(mCacheIOThread));
  if (NS_FAILED(rv)) {
    NS_WARNING("Can't create cache IO thread");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // initialize hashtable for active cache entries
  mActiveEntries.Init();
||||||| merged common ancestors
    // initialize hashtable for active cache entries
    mActiveEntries.Init();
=======
  rv = nsDeleteDir::Init();
  if (NS_FAILED(rv)) {
    NS_WARNING("Can't initialize nsDeleteDir");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // create profile/preference observer
  if (!mObserver) {
    mObserver = new nsCacheProfilePrefObserver();
    NS_ADDREF(mObserver);
    mObserver->Install();
  }
||||||| merged common ancestors
    // create profile/preference observer
    if (!mObserver) {
      mObserver = new nsCacheProfilePrefObserver();
      NS_ADDREF(mObserver);
      mObserver->Install();
    }
=======
  // initialize hashtable for active cache entries
  mActiveEntries.Init();
>>>>>>> upstream-releases

<<<<<<< HEAD
  mEnableDiskDevice = mObserver->DiskCacheEnabled();
  mEnableOfflineDevice = mObserver->OfflineCacheEnabled();
  mEnableMemoryDevice = mObserver->MemoryCacheEnabled();
||||||| merged common ancestors
    mEnableDiskDevice    = mObserver->DiskCacheEnabled();
    mEnableOfflineDevice = mObserver->OfflineCacheEnabled();
    mEnableMemoryDevice  = mObserver->MemoryCacheEnabled();
=======
  // create profile/preference observer
  if (!mObserver) {
    mObserver = new nsCacheProfilePrefObserver();
    NS_ADDREF(mObserver);
    mObserver->Install();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RegisterWeakMemoryReporter(this);
||||||| merged common ancestors
    RegisterWeakMemoryReporter(this);
=======
  mEnableOfflineDevice = mObserver->OfflineCacheEnabled();
>>>>>>> upstream-releases

  mInitialized = true;
  return NS_OK;
}

<<<<<<< HEAD
void nsCacheService::Shutdown() {
  // This method must be called on the main thread because mCacheIOThread must
  // only be modified on the main thread.
  if (!NS_IsMainThread()) {
    MOZ_CRASH("nsCacheService::Shutdown called off the main thread");
  }

  nsCOMPtr<nsIThread> cacheIOThread;
  Telemetry::AutoTimer<Telemetry::NETWORK_DISK_CACHE_SHUTDOWN> totalTimer;

  bool shouldSanitize = false;
  nsCOMPtr<nsIFile> parentDir;

  {
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SHUTDOWN));
    NS_ASSERTION(
        mInitialized,
        "can't shutdown nsCacheService unless it has been initialized.");
    if (!mInitialized) return;

    mClearingEntries = true;
    DoomActiveEntries(nullptr);
  }

  CloseAllStreams();

  UnregisterWeakMemoryReporter(this);

  {
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SHUTDOWN));
    NS_ASSERTION(mInitialized, "Bad state");
||||||| merged common ancestors
void
nsCacheService::Shutdown()
{
    // This method must be called on the main thread because mCacheIOThread must
    // only be modified on the main thread.
    if (!NS_IsMainThread()) {
        MOZ_CRASH("nsCacheService::Shutdown called off the main thread");
    }

    nsCOMPtr<nsIThread> cacheIOThread;
    Telemetry::AutoTimer<Telemetry::NETWORK_DISK_CACHE_SHUTDOWN> totalTimer;

    bool shouldSanitize = false;
    nsCOMPtr<nsIFile> parentDir;

    {
        nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SHUTDOWN));
        NS_ASSERTION(mInitialized,
            "can't shutdown nsCacheService unless it has been initialized.");
        if (!mInitialized)
            return;

        mClearingEntries = true;
        DoomActiveEntries(nullptr);
    }

    CloseAllStreams();

    UnregisterWeakMemoryReporter(this);

    {
        nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SHUTDOWN));
        NS_ASSERTION(mInitialized, "Bad state");
=======
void nsCacheService::Shutdown() {
  // This method must be called on the main thread because mCacheIOThread must
  // only be modified on the main thread.
  if (!NS_IsMainThread()) {
    MOZ_CRASH("nsCacheService::Shutdown called off the main thread");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    mInitialized = false;
||||||| merged common ancestors
        mInitialized = false;
=======
  nsCOMPtr<nsIThread> cacheIOThread;
  Telemetry::AutoTimer<Telemetry::NETWORK_DISK_CACHE_SHUTDOWN> totalTimer;
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Clear entries
    ClearDoomList();
||||||| merged common ancestors
        // Clear entries
        ClearDoomList();
=======
  bool shouldSanitize = false;
  nsCOMPtr<nsIFile> parentDir;
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (mSmartSizeTimer) {
      mSmartSizeTimer->Cancel();
      mSmartSizeTimer = nullptr;
    }
||||||| merged common ancestors
        if (mSmartSizeTimer) {
            mSmartSizeTimer->Cancel();
            mSmartSizeTimer = nullptr;
        }
=======
  {
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SHUTDOWN));
    NS_ASSERTION(
        mInitialized,
        "can't shutdown nsCacheService unless it has been initialized.");
    if (!mInitialized) return;

    mClearingEntries = true;
    DoomActiveEntries(nullptr);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Make sure to wait for any pending cache-operations before
    // proceeding with destructive actions (bug #620660)
    (void)SyncWithCacheIOThread();
    mActiveEntries.Shutdown();
||||||| merged common ancestors
        // Make sure to wait for any pending cache-operations before
        // proceeding with destructive actions (bug #620660)
        (void) SyncWithCacheIOThread();
        mActiveEntries.Shutdown();
=======
  CloseAllStreams();
>>>>>>> upstream-releases

<<<<<<< HEAD
    // obtain the disk cache directory in case we need to sanitize it
    parentDir = mObserver->DiskCacheParentDirectory();
    shouldSanitize = mObserver->SanitizeAtShutdown();
||||||| merged common ancestors
        // obtain the disk cache directory in case we need to sanitize it
        parentDir = mObserver->DiskCacheParentDirectory();
        shouldSanitize = mObserver->SanitizeAtShutdown();
=======
  {
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SHUTDOWN));
    NS_ASSERTION(mInitialized, "Bad state");
>>>>>>> upstream-releases

<<<<<<< HEAD
    // deallocate memory and disk caches
    delete mMemoryDevice;
    mMemoryDevice = nullptr;
||||||| merged common ancestors
        // deallocate memory and disk caches
        delete mMemoryDevice;
        mMemoryDevice = nullptr;
=======
    mInitialized = false;
>>>>>>> upstream-releases

<<<<<<< HEAD
    delete mDiskDevice;
    mDiskDevice = nullptr;
||||||| merged common ancestors
        delete mDiskDevice;
        mDiskDevice = nullptr;
=======
    // Clear entries
    ClearDoomList();
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (mOfflineDevice) mOfflineDevice->Shutdown();
||||||| merged common ancestors
        if (mOfflineDevice)
            mOfflineDevice->Shutdown();
=======
    // Make sure to wait for any pending cache-operations before
    // proceeding with destructive actions (bug #620660)
    (void)SyncWithCacheIOThread();
    mActiveEntries.Shutdown();
>>>>>>> upstream-releases

<<<<<<< HEAD
    NS_IF_RELEASE(mOfflineDevice);
||||||| merged common ancestors
        NS_IF_RELEASE(mOfflineDevice);
=======
    // obtain the disk cache directory in case we need to sanitize it
    parentDir = mObserver->DiskCacheParentDirectory();
    shouldSanitize = mObserver->SanitizeAtShutdown();
>>>>>>> upstream-releases

<<<<<<< HEAD
    for (auto iter = mCustomOfflineDevices.Iter(); !iter.Done(); iter.Next()) {
      iter.Data()->Shutdown();
      iter.Remove();
    }
||||||| merged common ancestors
        for (auto iter = mCustomOfflineDevices.Iter();
             !iter.Done(); iter.Next()) {
            iter.Data()->Shutdown();
            iter.Remove();
        }
=======
    if (mOfflineDevice) mOfflineDevice->Shutdown();
>>>>>>> upstream-releases

<<<<<<< HEAD
    LogCacheStatistics();
||||||| merged common ancestors
        LogCacheStatistics();
=======
    NS_IF_RELEASE(mOfflineDevice);
>>>>>>> upstream-releases

<<<<<<< HEAD
    mClearingEntries = false;
    mCacheIOThread.swap(cacheIOThread);
  }
||||||| merged common ancestors
        mClearingEntries = false;
        mCacheIOThread.swap(cacheIOThread);
    }
=======
    for (auto iter = mCustomOfflineDevices.Iter(); !iter.Done(); iter.Next()) {
      iter.Data()->Shutdown();
      iter.Remove();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (cacheIOThread) nsShutdownThread::BlockingShutdown(cacheIOThread);
||||||| merged common ancestors
    if (cacheIOThread)
        nsShutdownThread::BlockingShutdown(cacheIOThread);
=======
    LogCacheStatistics();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (shouldSanitize) {
    nsresult rv = parentDir->AppendNative(NS_LITERAL_CSTRING("Cache"));
    if (NS_SUCCEEDED(rv)) {
      bool exists;
      if (NS_SUCCEEDED(parentDir->Exists(&exists)) && exists)
        nsDeleteDir::DeleteDir(parentDir, false);
    }
    Telemetry::AutoTimer<Telemetry::NETWORK_DISK_CACHE_SHUTDOWN_CLEAR_PRIVATE>
        timer;
    nsDeleteDir::Shutdown(shouldSanitize);
  } else {
    Telemetry::AutoTimer<Telemetry::NETWORK_DISK_CACHE_DELETEDIR_SHUTDOWN>
        timer;
    nsDeleteDir::Shutdown(shouldSanitize);
  }
}
||||||| merged common ancestors
    if (shouldSanitize) {
        nsresult rv = parentDir->AppendNative(NS_LITERAL_CSTRING("Cache"));
        if (NS_SUCCEEDED(rv)) {
            bool exists;
            if (NS_SUCCEEDED(parentDir->Exists(&exists)) && exists)
                nsDeleteDir::DeleteDir(parentDir, false);
        }
        Telemetry::AutoTimer<Telemetry::NETWORK_DISK_CACHE_SHUTDOWN_CLEAR_PRIVATE> timer;
        nsDeleteDir::Shutdown(shouldSanitize);
    } else {
        Telemetry::AutoTimer<Telemetry::NETWORK_DISK_CACHE_DELETEDIR_SHUTDOWN> timer;
        nsDeleteDir::Shutdown(shouldSanitize);
    }
}
=======
    mClearingEntries = false;
    mCacheIOThread.swap(cacheIOThread);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsCacheService::Create(nsISupports *aOuter, const nsIID &aIID,
                                void **aResult) {
  nsresult rv;
||||||| merged common ancestors
=======
  if (cacheIOThread) nsShutdownThread::BlockingShutdown(cacheIOThread);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (aOuter != nullptr) return NS_ERROR_NO_AGGREGATION;
||||||| merged common ancestors
nsresult
nsCacheService::Create(nsISupports* aOuter, const nsIID& aIID, void* *aResult)
{
    nsresult  rv;
=======
  if (shouldSanitize) {
    nsresult rv = parentDir->AppendNative(NS_LITERAL_CSTRING("Cache"));
    if (NS_SUCCEEDED(rv)) {
      bool exists;
      if (NS_SUCCEEDED(parentDir->Exists(&exists)) && exists)
        nsDeleteDir::DeleteDir(parentDir, false);
    }
    Telemetry::AutoTimer<Telemetry::NETWORK_DISK_CACHE_SHUTDOWN_CLEAR_PRIVATE>
        timer;
    nsDeleteDir::Shutdown(shouldSanitize);
  } else {
    Telemetry::AutoTimer<Telemetry::NETWORK_DISK_CACHE_DELETEDIR_SHUTDOWN>
        timer;
    nsDeleteDir::Shutdown(shouldSanitize);
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCacheService *cacheService = new nsCacheService();
  if (cacheService == nullptr) return NS_ERROR_OUT_OF_MEMORY;
||||||| merged common ancestors
    if (aOuter != nullptr)
        return NS_ERROR_NO_AGGREGATION;
=======
nsresult nsCacheService::Create(nsISupports* aOuter, const nsIID& aIID,
                                void** aResult) {
  nsresult rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
  NS_ADDREF(cacheService);
  rv = cacheService->Init();
  if (NS_SUCCEEDED(rv)) {
    rv = cacheService->QueryInterface(aIID, aResult);
  }
  NS_RELEASE(cacheService);
  return rv;
||||||| merged common ancestors
    nsCacheService * cacheService = new nsCacheService();
    if (cacheService == nullptr)
        return NS_ERROR_OUT_OF_MEMORY;

    NS_ADDREF(cacheService);
    rv = cacheService->Init();
    if (NS_SUCCEEDED(rv)) {
        rv = cacheService->QueryInterface(aIID, aResult);
    }
    NS_RELEASE(cacheService);
    return rv;
=======
  if (aOuter != nullptr) return NS_ERROR_NO_AGGREGATION;

  RefPtr<nsCacheService> cacheService = new nsCacheService();
  rv = cacheService->Init();
  if (NS_SUCCEEDED(rv)) {
    rv = cacheService->QueryInterface(aIID, aResult);
  }
  return rv;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsCacheService::CreateSession(const char *clientID,
                              nsCacheStoragePolicy storagePolicy,
                              bool streamBased, nsICacheSession **result) {
  *result = nullptr;
||||||| merged common ancestors
nsCacheService::CreateSession(const char *          clientID,
                              nsCacheStoragePolicy  storagePolicy,
                              bool                  streamBased,
                              nsICacheSession     **result)
{
    *result = nullptr;

    return NS_ERROR_NOT_IMPLEMENTED;
}

nsresult
nsCacheService::CreateSessionInternal(const char *          clientID,
                                      nsCacheStoragePolicy  storagePolicy,
                                      bool                  streamBased,
                                      nsICacheSession     **result)
{
    RefPtr<nsCacheSession> session =
        new nsCacheSession(clientID, storagePolicy, streamBased);
    session.forget(result);
=======
nsCacheService::CreateSession(const char* clientID,
                              nsCacheStoragePolicy storagePolicy,
                              bool streamBased, nsICacheSession** result) {
  *result = nullptr;
>>>>>>> upstream-releases

  return NS_ERROR_NOT_IMPLEMENTED;
}

<<<<<<< HEAD
nsresult nsCacheService::CreateSessionInternal(
    const char *clientID, nsCacheStoragePolicy storagePolicy, bool streamBased,
    nsICacheSession **result) {
  RefPtr<nsCacheSession> session =
      new nsCacheSession(clientID, storagePolicy, streamBased);
  session.forget(result);

  return NS_OK;
}
||||||| merged common ancestors
=======
nsresult nsCacheService::CreateSessionInternal(
    const char* clientID, nsCacheStoragePolicy storagePolicy, bool streamBased,
    nsICacheSession** result) {
  RefPtr<nsCacheSession> session =
      new nsCacheSession(clientID, storagePolicy, streamBased);
  session.forget(result);

  return NS_OK;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsCacheService::EvictEntriesForSession(nsCacheSession *session) {
  NS_ASSERTION(gService, "nsCacheService::gService is null.");
  return gService->EvictEntriesForClient(session->ClientID()->get(),
                                         session->StoragePolicy());
||||||| merged common ancestors
nsresult
nsCacheService::EvictEntriesForSession(nsCacheSession * session)
{
    NS_ASSERTION(gService, "nsCacheService::gService is null.");
    return gService->EvictEntriesForClient(session->ClientID()->get(),
                                 session->StoragePolicy());
=======
nsresult nsCacheService::EvictEntriesForSession(nsCacheSession* session) {
  NS_ASSERTION(gService, "nsCacheService::gService is null.");
  return gService->EvictEntriesForClient(session->ClientID()->get(),
                                         session->StoragePolicy());
>>>>>>> upstream-releases
}

namespace {

<<<<<<< HEAD
class EvictionNotifierRunnable : public Runnable {
 public:
  explicit EvictionNotifierRunnable(nsISupports *aSubject)
      : mozilla::Runnable("EvictionNotifierRunnable"), mSubject(aSubject) {}
||||||| merged common ancestors
class EvictionNotifierRunnable : public Runnable
{
public:
  explicit EvictionNotifierRunnable(nsISupports* aSubject)
    : mozilla::Runnable("EvictionNotifierRunnable")
    , mSubject(aSubject)
  {
  }
=======
class EvictionNotifierRunnable : public Runnable {
 public:
  explicit EvictionNotifierRunnable(nsISupports* aSubject)
      : mozilla::Runnable("EvictionNotifierRunnable"), mSubject(aSubject) {}
>>>>>>> upstream-releases

  NS_DECL_NSIRUNNABLE

 private:
  nsCOMPtr<nsISupports> mSubject;
};

NS_IMETHODIMP
EvictionNotifierRunnable::Run() {
  nsCOMPtr<nsIObserverService> obsSvc = mozilla::services::GetObserverService();
  if (obsSvc) {
    obsSvc->NotifyObservers(mSubject, NS_CACHESERVICE_EMPTYCACHE_TOPIC_ID,
                            nullptr);
  }
  return NS_OK;
}

<<<<<<< HEAD
}  // namespace

nsresult nsCacheService::EvictEntriesForClient(
    const char *clientID, nsCacheStoragePolicy storagePolicy) {
  RefPtr<EvictionNotifierRunnable> r =
      new EvictionNotifierRunnable(NS_ISUPPORTS_CAST(nsICacheService *, this));
  NS_DispatchToMainThread(r);
||||||| merged common ancestors
} // namespace

nsresult
nsCacheService::EvictEntriesForClient(const char *          clientID,
                                      nsCacheStoragePolicy  storagePolicy)
{
    RefPtr<EvictionNotifierRunnable> r =
        new EvictionNotifierRunnable(NS_ISUPPORTS_CAST(nsICacheService*, this));
    NS_DispatchToMainThread(r);
=======
}  // namespace
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_EVICTENTRIESFORCLIENT));
  nsresult res = NS_OK;

  if (storagePolicy == nsICache::STORE_ANYWHERE ||
      storagePolicy == nsICache::STORE_ON_DISK) {
    if (mEnableDiskDevice) {
      nsresult rv = NS_OK;
      if (!mDiskDevice) rv = CreateDiskDevice();
      if (mDiskDevice) rv = mDiskDevice->EvictEntries(clientID);
      if (NS_FAILED(rv)) res = rv;
    }
  }
||||||| merged common ancestors
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_EVICTENTRIESFORCLIENT));
    nsresult res = NS_OK;

    if (storagePolicy == nsICache::STORE_ANYWHERE ||
        storagePolicy == nsICache::STORE_ON_DISK) {

        if (mEnableDiskDevice) {
            nsresult rv = NS_OK;
            if (!mDiskDevice)
                rv = CreateDiskDevice();
            if (mDiskDevice)
                rv = mDiskDevice->EvictEntries(clientID);
            if (NS_FAILED(rv))
                res = rv;
        }
    }
=======
nsresult nsCacheService::EvictEntriesForClient(
    const char* clientID, nsCacheStoragePolicy storagePolicy) {
  RefPtr<EvictionNotifierRunnable> r =
      new EvictionNotifierRunnable(NS_ISUPPORTS_CAST(nsICacheService*, this));
  NS_DispatchToMainThread(r);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Only clear the offline cache if it has been specifically asked for.
  if (storagePolicy == nsICache::STORE_OFFLINE) {
    if (mEnableOfflineDevice) {
      nsresult rv = NS_OK;
      if (!mOfflineDevice) rv = CreateOfflineDevice();
      if (mOfflineDevice) rv = mOfflineDevice->EvictEntries(clientID);
      if (NS_FAILED(rv)) res = rv;
    }
  }
||||||| merged common ancestors
    // Only clear the offline cache if it has been specifically asked for.
    if (storagePolicy == nsICache::STORE_OFFLINE) {
        if (mEnableOfflineDevice) {
            nsresult rv = NS_OK;
            if (!mOfflineDevice)
                rv = CreateOfflineDevice();
            if (mOfflineDevice)
                rv = mOfflineDevice->EvictEntries(clientID);
            if (NS_FAILED(rv))
                res = rv;
        }
    }
=======
  nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_EVICTENTRIESFORCLIENT));
  nsresult res = NS_OK;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (storagePolicy == nsICache::STORE_ANYWHERE ||
      storagePolicy == nsICache::STORE_IN_MEMORY) {
    // If there is no memory device, there is no need to evict it...
    if (mMemoryDevice) {
      nsresult rv = mMemoryDevice->EvictEntries(clientID);
      if (NS_FAILED(rv)) res = rv;
||||||| merged common ancestors
    if (storagePolicy == nsICache::STORE_ANYWHERE ||
        storagePolicy == nsICache::STORE_IN_MEMORY) {
        // If there is no memory device, there is no need to evict it...
        if (mMemoryDevice) {
            nsresult rv = mMemoryDevice->EvictEntries(clientID);
            if (NS_FAILED(rv))
                res = rv;
        }
=======
  // Only clear the offline cache if it has been specifically asked for.
  if (storagePolicy == nsICache::STORE_OFFLINE) {
    if (mEnableOfflineDevice) {
      nsresult rv = NS_OK;
      if (!mOfflineDevice) rv = CreateOfflineDevice();
      if (mOfflineDevice) rv = mOfflineDevice->EvictEntries(clientID);
      if (NS_FAILED(rv)) res = rv;
>>>>>>> upstream-releases
    }
  }

  return res;
}

<<<<<<< HEAD
nsresult nsCacheService::IsStorageEnabledForPolicy(
    nsCacheStoragePolicy storagePolicy, bool *result) {
  if (gService == nullptr) return NS_ERROR_NOT_AVAILABLE;
  nsCacheServiceAutoLock lock(
      LOCK_TELEM(NSCACHESERVICE_ISSTORAGEENABLEDFORPOLICY));
||||||| merged common ancestors
=======
nsresult nsCacheService::IsStorageEnabledForPolicy(
    nsCacheStoragePolicy storagePolicy, bool* result) {
  if (gService == nullptr) return NS_ERROR_NOT_AVAILABLE;
  nsCacheServiceAutoLock lock(
      LOCK_TELEM(NSCACHESERVICE_ISSTORAGEENABLEDFORPOLICY));
>>>>>>> upstream-releases

  *result = nsCacheService::IsStorageEnabledForPolicy_Locked(storagePolicy);
  return NS_OK;
}

<<<<<<< HEAD
nsresult nsCacheService::DoomEntry(nsCacheSession *session,
                                   const nsACString &key,
                                   nsICacheListener *listener) {
  CACHE_LOG_DEBUG(("Dooming entry for session %p, key %s\n", session,
                   PromiseFlatCString(key).get()));
  if (!gService || !gService->mInitialized) return NS_ERROR_NOT_INITIALIZED;
||||||| merged common ancestors
=======
nsresult nsCacheService::DoomEntry(nsCacheSession* session,
                                   const nsACString& key,
                                   nsICacheListener* listener) {
  CACHE_LOG_DEBUG(("Dooming entry for session %p, key %s\n", session,
                   PromiseFlatCString(key).get()));
  if (!gService || !gService->mInitialized) return NS_ERROR_NOT_INITIALIZED;
>>>>>>> upstream-releases

  return DispatchToCacheIOThread(new nsDoomEvent(session, key, listener));
}

<<<<<<< HEAD
bool nsCacheService::IsStorageEnabledForPolicy_Locked(
    nsCacheStoragePolicy storagePolicy) {
  if (gService->mEnableMemoryDevice &&
      (storagePolicy == nsICache::STORE_ANYWHERE ||
       storagePolicy == nsICache::STORE_IN_MEMORY)) {
    return true;
  }
  if (gService->mEnableDiskDevice &&
      (storagePolicy == nsICache::STORE_ANYWHERE ||
       storagePolicy == nsICache::STORE_ON_DISK)) {
    return true;
  }
  if (gService->mEnableOfflineDevice &&
      storagePolicy == nsICache::STORE_OFFLINE) {
    return true;
  }
||||||| merged common ancestors
=======
bool nsCacheService::IsStorageEnabledForPolicy_Locked(
    nsCacheStoragePolicy storagePolicy) {
  if (gService->mEnableOfflineDevice &&
      storagePolicy == nsICache::STORE_OFFLINE) {
    return true;
  }
>>>>>>> upstream-releases

  return false;
}

<<<<<<< HEAD
NS_IMETHODIMP nsCacheService::VisitEntries(nsICacheVisitor *visitor) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
NS_IMETHODIMP nsCacheService::VisitEntries(nsICacheVisitor *visitor)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
NS_IMETHODIMP nsCacheService::VisitEntries(nsICacheVisitor* visitor) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsCacheService::VisitEntriesInternal(nsICacheVisitor *visitor) {
  NS_ENSURE_ARG_POINTER(visitor);
||||||| merged common ancestors
nsresult nsCacheService::VisitEntriesInternal(nsICacheVisitor *visitor)
{
    NS_ENSURE_ARG_POINTER(visitor);
=======
nsresult nsCacheService::VisitEntriesInternal(nsICacheVisitor* visitor) {
  NS_ENSURE_ARG_POINTER(visitor);
>>>>>>> upstream-releases

  nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_VISITENTRIES));

<<<<<<< HEAD
  if (!(mEnableDiskDevice || mEnableMemoryDevice))
    return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
    if (!(mEnableDiskDevice || mEnableMemoryDevice))
        return NS_ERROR_NOT_AVAILABLE;
=======
  if (!mEnableOfflineDevice) return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases

  // XXX record the fact that a visitation is in progress,
  // XXX i.e. keep list of visitors in progress.

<<<<<<< HEAD
  nsresult rv = NS_OK;
  // If there is no memory device, there are then also no entries to visit...
  if (mMemoryDevice) {
    rv = mMemoryDevice->Visit(visitor);
    if (NS_FAILED(rv)) return rv;
  }
||||||| merged common ancestors
    nsresult rv = NS_OK;
    // If there is no memory device, there are then also no entries to visit...
    if (mMemoryDevice) {
        rv = mMemoryDevice->Visit(visitor);
        if (NS_FAILED(rv)) return rv;
    }
=======
  nsresult rv = NS_OK;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mEnableDiskDevice) {
    if (!mDiskDevice) {
      rv = CreateDiskDevice();
      if (NS_FAILED(rv)) return rv;
    }
    rv = mDiskDevice->Visit(visitor);
    if (NS_FAILED(rv)) return rv;
  }

  if (mEnableOfflineDevice) {
    if (!mOfflineDevice) {
      rv = CreateOfflineDevice();
      if (NS_FAILED(rv)) return rv;
||||||| merged common ancestors
    if (mEnableDiskDevice) {
        if (!mDiskDevice) {
            rv = CreateDiskDevice();
            if (NS_FAILED(rv)) return rv;
        }
        rv = mDiskDevice->Visit(visitor);
        if (NS_FAILED(rv)) return rv;
    }

    if (mEnableOfflineDevice) {
        if (!mOfflineDevice) {
            rv = CreateOfflineDevice();
            if (NS_FAILED(rv)) return rv;
        }
        rv = mOfflineDevice->Visit(visitor);
        if (NS_FAILED(rv)) return rv;
=======
  if (mEnableOfflineDevice) {
    if (!mOfflineDevice) {
      rv = CreateOfflineDevice();
      if (NS_FAILED(rv)) return rv;
>>>>>>> upstream-releases
    }
    rv = mOfflineDevice->Visit(visitor);
    if (NS_FAILED(rv)) return rv;
  }

  // XXX notify any shutdown process that visitation is complete for THIS
  // visitor.
  // XXX keep queue of visitors

  return NS_OK;
}

void nsCacheService::FireClearNetworkCacheStoredAnywhereNotification() {
  MOZ_ASSERT(NS_IsMainThread());
  nsCOMPtr<nsIObserverService> obsvc = mozilla::services::GetObserverService();
  if (obsvc) {
    obsvc->NotifyObservers(nullptr, "network-clear-cache-stored-anywhere",
                           nullptr);
  }
}

NS_IMETHODIMP nsCacheService::EvictEntries(nsCacheStoragePolicy storagePolicy) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

nsresult nsCacheService::EvictEntriesInternal(
    nsCacheStoragePolicy storagePolicy) {
  if (storagePolicy == nsICache::STORE_ANYWHERE) {
    // if not called on main thread, dispatch the notification to the main
    // thread to notify observers
    if (!NS_IsMainThread()) {
      nsCOMPtr<nsIRunnable> event = NewRunnableMethod(
          "nsCacheService::FireClearNetworkCacheStoredAnywhereNotification",
          this,
          &nsCacheService::FireClearNetworkCacheStoredAnywhereNotification);
      NS_DispatchToMainThread(event);
    } else {
      // else you're already on main thread - notify observers
      FireClearNetworkCacheStoredAnywhereNotification();
    }
<<<<<<< HEAD
  }
  return EvictEntriesForClient(nullptr, storagePolicy);
}

NS_IMETHODIMP nsCacheService::GetCacheIOTarget(
    nsIEventTarget **aCacheIOTarget) {
  NS_ENSURE_ARG_POINTER(aCacheIOTarget);

  // Because mCacheIOThread can only be changed on the main thread, it can be
  // read from the main thread without the lock. This is useful to prevent
  // blocking the main thread on other cache operations.
  if (!NS_IsMainThread()) {
    Lock(LOCK_TELEM(NSCACHESERVICE_GETCACHEIOTARGET));
  }

  nsresult rv;
  if (mCacheIOThread) {
    NS_ADDREF(*aCacheIOTarget = mCacheIOThread);
    rv = NS_OK;
  } else {
    *aCacheIOTarget = nullptr;
    rv = NS_ERROR_NOT_AVAILABLE;
  }

  if (!NS_IsMainThread()) {
    Unlock();
  }

  return rv;
}

NS_IMETHODIMP nsCacheService::GetLockHeldTime(double *aLockHeldTime) {
  MutexAutoLock lock(mTimeStampLock);

  if (mLockAcquiredTimeStamp.IsNull()) {
    *aLockHeldTime = 0.0;
  } else {
    *aLockHeldTime =
        (TimeStamp::Now() - mLockAcquiredTimeStamp).ToMilliseconds();
  }

  return NS_OK;
||||||| merged common ancestors

    if (!NS_IsMainThread()) {
        Unlock();
    }

    return rv;
}

NS_IMETHODIMP nsCacheService::GetLockHeldTime(double *aLockHeldTime)
{
    MutexAutoLock lock(mTimeStampLock);

    if (mLockAcquiredTimeStamp.IsNull()) {
        *aLockHeldTime = 0.0;
    }
    else {
        *aLockHeldTime =
            (TimeStamp::Now() - mLockAcquiredTimeStamp).ToMilliseconds();
    }

    return NS_OK;
=======
  }
  return EvictEntriesForClient(nullptr, storagePolicy);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/**
 * Internal Methods
 */
nsresult nsCacheService::CreateDiskDevice() {
  if (!mInitialized) return NS_ERROR_NOT_AVAILABLE;
  if (!mEnableDiskDevice) return NS_ERROR_NOT_AVAILABLE;
  if (mDiskDevice) return NS_OK;

  mDiskDevice = new nsDiskCacheDevice;
  if (!mDiskDevice) return NS_ERROR_OUT_OF_MEMORY;

  // set the preferences
  mDiskDevice->SetCacheParentDirectory(mObserver->DiskCacheParentDirectory());
  mDiskDevice->SetCapacity(mObserver->DiskCacheCapacity());
  mDiskDevice->SetMaxEntrySize(mObserver->DiskCacheMaxEntrySize());

  nsresult rv = mDiskDevice->Init();
  if (NS_FAILED(rv)) {
#if DEBUG
    printf("###\n");
    printf("### mDiskDevice->Init() failed (0x%.8x)\n",
           static_cast<uint32_t>(rv));
    printf("###    - disabling disk cache for this session.\n");
    printf("###\n");
#endif
    mEnableDiskDevice = false;
    delete mDiskDevice;
    mDiskDevice = nullptr;
    return rv;
  }

  NS_ASSERTION(!mSmartSizeTimer, "Smartsize timer was already fired!");
||||||| merged common ancestors
/**
 * Internal Methods
 */
nsresult
nsCacheService::CreateDiskDevice()
{
    if (!mInitialized)      return NS_ERROR_NOT_AVAILABLE;
    if (!mEnableDiskDevice) return NS_ERROR_NOT_AVAILABLE;
    if (mDiskDevice)        return NS_OK;

    mDiskDevice = new nsDiskCacheDevice;
    if (!mDiskDevice)       return NS_ERROR_OUT_OF_MEMORY;

    // set the preferences
    mDiskDevice->SetCacheParentDirectory(mObserver->DiskCacheParentDirectory());
    mDiskDevice->SetCapacity(mObserver->DiskCacheCapacity());
    mDiskDevice->SetMaxEntrySize(mObserver->DiskCacheMaxEntrySize());

    nsresult rv = mDiskDevice->Init();
    if (NS_FAILED(rv)) {
#if DEBUG
        printf("###\n");
        printf("### mDiskDevice->Init() failed (0x%.8x)\n",
               static_cast<uint32_t>(rv));
        printf("###    - disabling disk cache for this session.\n");
        printf("###\n");
#endif
        mEnableDiskDevice = false;
        delete mDiskDevice;
        mDiskDevice = nullptr;
        return rv;
    }

    NS_ASSERTION(!mSmartSizeTimer, "Smartsize timer was already fired!");
=======
NS_IMETHODIMP nsCacheService::GetCacheIOTarget(
    nsIEventTarget** aCacheIOTarget) {
  NS_ENSURE_ARG_POINTER(aCacheIOTarget);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Disk device is usually created during the startup. Delay smart size
  // calculation to avoid possible massive IO caused by eviction of entries
  // in case the new smart size is smaller than current cache usage.
  rv = NS_NewTimerWithCallback(getter_AddRefs(mSmartSizeTimer),
                               new nsSetDiskSmartSizeCallback(), 1000 * 60 * 3,
                               nsITimer::TYPE_ONE_SHOT);
  if (NS_FAILED(rv)) {
    NS_WARNING("Failed to post smart size timer");
  }
  // Ignore state of the timer and return success since the purpose of the
  // method (create the disk-device) has been fulfilled

  return NS_OK;
}

// Runnable sent from cache thread to main thread
class nsDisableOldMaxSmartSizePrefEvent : public Runnable {
 public:
  nsDisableOldMaxSmartSizePrefEvent()
      : mozilla::Runnable("nsDisableOldMaxSmartSizePrefEvent") {}
||||||| merged common ancestors
    // Disk device is usually created during the startup. Delay smart size
    // calculation to avoid possible massive IO caused by eviction of entries
    // in case the new smart size is smaller than current cache usage.
    rv = NS_NewTimerWithCallback(getter_AddRefs(mSmartSizeTimer),
                                 new nsSetDiskSmartSizeCallback(),
                                 1000*60*3,
                                 nsITimer::TYPE_ONE_SHOT);
    if (NS_FAILED(rv)) {
        NS_WARNING("Failed to post smart size timer");
    }
    // Ignore state of the timer and return success since the purpose of the
    // method (create the disk-device) has been fulfilled

    return NS_OK;
}

// Runnable sent from cache thread to main thread
class nsDisableOldMaxSmartSizePrefEvent: public Runnable
{
public:
  nsDisableOldMaxSmartSizePrefEvent()
    : mozilla::Runnable("nsDisableOldMaxSmartSizePrefEvent")
  {
  }
=======
  // Because mCacheIOThread can only be changed on the main thread, it can be
  // read from the main thread without the lock. This is useful to prevent
  // blocking the main thread on other cache operations.
  if (!NS_IsMainThread()) {
    Lock(LOCK_TELEM(NSCACHESERVICE_GETCACHEIOTARGET));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  NS_IMETHOD Run() override {
    // Main thread may have already called nsCacheService::Shutdown
    if (!nsCacheService::IsInitialized()) return NS_ERROR_NOT_AVAILABLE;

    nsCOMPtr<nsIPrefBranch> branch = do_GetService(NS_PREFSERVICE_CONTRACTID);
    if (!branch) {
      return NS_ERROR_NOT_AVAILABLE;
    }

    nsresult rv =
        branch->SetBoolPref(DISK_CACHE_USE_OLD_MAX_SMART_SIZE_PREF, false);
    if (NS_FAILED(rv)) {
      NS_WARNING("Failed to disable old max smart size");
      return rv;
    }

    // It is safe to call SetDiskSmartSize_Locked() without holding the lock
    // when we are on main thread and nsCacheService is initialized.
    nsCacheService::gService->SetDiskSmartSize_Locked();

    if (nsCacheService::gService->mObserver->PermittedToSmartSize(branch,
                                                                  false)) {
      rv = branch->SetIntPref(DISK_CACHE_CAPACITY_PREF, MAX_CACHE_SIZE);
      if (NS_FAILED(rv)) {
        NS_WARNING("Failed to set cache capacity pref");
      }
    }

    return NS_OK;
  }
};

void nsCacheService::MarkStartingFresh() {
  if (!gService || !gService->mObserver->ShouldUseOldMaxSmartSize()) {
    // Already using new max, nothing to do here
    return;
  }

  gService->mObserver->SetUseNewMaxSmartSize(true);

  // We always dispatch an event here because we don't want to deal with lock
  // reentrance issues.
  NS_DispatchToMainThread(new nsDisableOldMaxSmartSizePrefEvent());
}
||||||| merged common ancestors
  NS_IMETHOD Run() override
  {
    // Main thread may have already called nsCacheService::Shutdown
    if (!nsCacheService::IsInitialized())
      return NS_ERROR_NOT_AVAILABLE;

    nsCOMPtr<nsIPrefBranch> branch = do_GetService(NS_PREFSERVICE_CONTRACTID);
    if (!branch) {
      return NS_ERROR_NOT_AVAILABLE;
    }

    nsresult rv =
      branch->SetBoolPref(DISK_CACHE_USE_OLD_MAX_SMART_SIZE_PREF, false);
    if (NS_FAILED(rv)) {
      NS_WARNING("Failed to disable old max smart size");
      return rv;
    }

    // It is safe to call SetDiskSmartSize_Locked() without holding the lock
    // when we are on main thread and nsCacheService is initialized.
    nsCacheService::gService->SetDiskSmartSize_Locked();

    if (nsCacheService::gService->mObserver->PermittedToSmartSize(branch,
                                                                  false)) {
      rv = branch->SetIntPref(DISK_CACHE_CAPACITY_PREF, MAX_CACHE_SIZE);
      if (NS_FAILED(rv)) {
        NS_WARNING("Failed to set cache capacity pref");
      }
    }

    return NS_OK;
    }
};

void
nsCacheService::MarkStartingFresh()
{
    if (!gService || !gService->mObserver->ShouldUseOldMaxSmartSize()) {
        // Already using new max, nothing to do here
        return;
    }

    gService->mObserver->SetUseNewMaxSmartSize(true);

    // We always dispatch an event here because we don't want to deal with lock
    // reentrance issues.
    NS_DispatchToMainThread(new nsDisableOldMaxSmartSizePrefEvent());
}
=======
  nsresult rv;
  if (mCacheIOThread) {
    NS_ADDREF(*aCacheIOTarget = mCacheIOThread);
    rv = NS_OK;
  } else {
    *aCacheIOTarget = nullptr;
    rv = NS_ERROR_NOT_AVAILABLE;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsCacheService::GetOfflineDevice(nsOfflineCacheDevice **aDevice) {
  if (!mOfflineDevice) {
    nsresult rv = CreateOfflineDevice();
    NS_ENSURE_SUCCESS(rv, rv);
  }
||||||| merged common ancestors
nsresult
nsCacheService::GetOfflineDevice(nsOfflineCacheDevice **aDevice)
{
    if (!mOfflineDevice) {
        nsresult rv = CreateOfflineDevice();
        NS_ENSURE_SUCCESS(rv, rv);
    }
=======
  if (!NS_IsMainThread()) {
    Unlock();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  NS_ADDREF(*aDevice = mOfflineDevice);
  return NS_OK;
||||||| merged common ancestors
    NS_ADDREF(*aDevice = mOfflineDevice);
    return NS_OK;
=======
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsCacheService::GetCustomOfflineDevice(
    nsIFile *aProfileDir, int32_t aQuota, nsOfflineCacheDevice **aDevice) {
  nsresult rv;

  nsAutoString profilePath;
  rv = aProfileDir->GetPath(profilePath);
  NS_ENSURE_SUCCESS(rv, rv);

  if (!mCustomOfflineDevices.Get(profilePath, aDevice)) {
    rv = CreateCustomOfflineDevice(aProfileDir, aQuota, aDevice);
    NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
nsresult
nsCacheService::GetCustomOfflineDevice(nsIFile *aProfileDir,
                                       int32_t aQuota,
                                       nsOfflineCacheDevice **aDevice)
{
    nsresult rv;

    nsAutoString profilePath;
    rv = aProfileDir->GetPath(profilePath);
    NS_ENSURE_SUCCESS(rv, rv);

    if (!mCustomOfflineDevices.Get(profilePath, aDevice)) {
        rv = CreateCustomOfflineDevice(aProfileDir, aQuota, aDevice);
        NS_ENSURE_SUCCESS(rv, rv);
=======
NS_IMETHODIMP nsCacheService::GetLockHeldTime(double* aLockHeldTime) {
  MutexAutoLock lock(mTimeStampLock);
>>>>>>> upstream-releases

<<<<<<< HEAD
    (*aDevice)->SetAutoShutdown();
    mCustomOfflineDevices.Put(profilePath, *aDevice);
  }
||||||| merged common ancestors
        (*aDevice)->SetAutoShutdown();
        mCustomOfflineDevices.Put(profilePath, *aDevice);
    }
=======
  if (mLockAcquiredTimeStamp.IsNull()) {
    *aLockHeldTime = 0.0;
  } else {
    *aLockHeldTime =
        (TimeStamp::Now() - mLockAcquiredTimeStamp).ToMilliseconds();
  }
>>>>>>> upstream-releases

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsCacheService::CreateOfflineDevice() {
  CACHE_LOG_INFO(("Creating default offline device"));

  if (mOfflineDevice) return NS_OK;
  if (!nsCacheService::IsInitialized()) {
    return NS_ERROR_NOT_AVAILABLE;
  }

  nsresult rv = CreateCustomOfflineDevice(
      mObserver->OfflineCacheParentDirectory(),
      mObserver->OfflineCacheCapacity(), &mOfflineDevice);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
nsresult
nsCacheService::CreateOfflineDevice()
{
    CACHE_LOG_INFO(("Creating default offline device"));

    if (mOfflineDevice)        return NS_OK;
    if (!nsCacheService::IsInitialized()) {
        return NS_ERROR_NOT_AVAILABLE;
    }

    nsresult rv = CreateCustomOfflineDevice(
        mObserver->OfflineCacheParentDirectory(),
        mObserver->OfflineCacheCapacity(),
        &mOfflineDevice);
    NS_ENSURE_SUCCESS(rv, rv);
=======
/**
 * Internal Methods
 */
nsresult nsCacheService::GetOfflineDevice(nsOfflineCacheDevice** aDevice) {
  if (!mOfflineDevice) {
    nsresult rv = CreateOfflineDevice();
    NS_ENSURE_SUCCESS(rv, rv);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return NS_OK;
}

nsresult nsCacheService::CreateCustomOfflineDevice(
    nsIFile *aProfileDir, int32_t aQuota, nsOfflineCacheDevice **aDevice) {
  NS_ENSURE_ARG(aProfileDir);

  if (MOZ_LOG_TEST(gCacheLog, LogLevel::Info)) {
    CACHE_LOG_INFO(("Creating custom offline device, %s, %d",
                    aProfileDir->HumanReadablePath().get(), aQuota));
  }

  if (!mInitialized) return NS_ERROR_NOT_AVAILABLE;
  if (!mEnableOfflineDevice) return NS_ERROR_NOT_AVAILABLE;

  *aDevice = new nsOfflineCacheDevice;

  NS_ADDREF(*aDevice);

  // set the preferences
  (*aDevice)->SetCacheParentDirectory(aProfileDir);
  (*aDevice)->SetCapacity(aQuota);

  nsresult rv = (*aDevice)->InitWithSqlite(mStorageService);
  if (NS_FAILED(rv)) {
    CACHE_LOG_DEBUG(("OfflineDevice->InitWithSqlite() failed (0x%.8" PRIx32
                     ")\n",
                     static_cast<uint32_t>(rv)));
    CACHE_LOG_DEBUG(("    - disabling offline cache for this session.\n"));

    NS_RELEASE(*aDevice);
  }
  return rv;
||||||| merged common ancestors
    return NS_OK;
}

nsresult
nsCacheService::CreateCustomOfflineDevice(nsIFile *aProfileDir,
                                          int32_t aQuota,
                                          nsOfflineCacheDevice **aDevice)
{
    NS_ENSURE_ARG(aProfileDir);

    if (MOZ_LOG_TEST(gCacheLog, LogLevel::Info)) {
      CACHE_LOG_INFO(("Creating custom offline device, %s, %d",
                      aProfileDir->HumanReadablePath().get(), aQuota));
    }

    if (!mInitialized)         return NS_ERROR_NOT_AVAILABLE;
    if (!mEnableOfflineDevice) return NS_ERROR_NOT_AVAILABLE;

    *aDevice = new nsOfflineCacheDevice;

    NS_ADDREF(*aDevice);

    // set the preferences
    (*aDevice)->SetCacheParentDirectory(aProfileDir);
    (*aDevice)->SetCapacity(aQuota);

    nsresult rv = (*aDevice)->InitWithSqlite(mStorageService);
    if (NS_FAILED(rv)) {
        CACHE_LOG_DEBUG(("OfflineDevice->InitWithSqlite() failed (0x%.8" PRIx32 ")\n",
                         static_cast<uint32_t>(rv)));
        CACHE_LOG_DEBUG(("    - disabling offline cache for this session.\n"));

        NS_RELEASE(*aDevice);
    }
    return rv;
=======
  NS_ADDREF(*aDevice = mOfflineDevice);
  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsCacheService::CreateMemoryDevice() {
  if (!mInitialized) return NS_ERROR_NOT_AVAILABLE;
  if (!mEnableMemoryDevice) return NS_ERROR_NOT_AVAILABLE;
  if (mMemoryDevice) return NS_OK;

  mMemoryDevice = new nsMemoryCacheDevice;
  if (!mMemoryDevice) return NS_ERROR_OUT_OF_MEMORY;

  // set preference
  int32_t capacity = mObserver->MemoryCacheCapacity();
  CACHE_LOG_DEBUG(("Creating memory device with capacity %d\n", capacity));
  mMemoryDevice->SetCapacity(capacity);
  mMemoryDevice->SetMaxEntrySize(mObserver->MemoryCacheMaxEntrySize());

  nsresult rv = mMemoryDevice->Init();
  if (NS_FAILED(rv)) {
    NS_WARNING("Initialization of Memory Cache failed.");
    delete mMemoryDevice;
    mMemoryDevice = nullptr;
  }

  return rv;
}
||||||| merged common ancestors
nsresult
nsCacheService::CreateMemoryDevice()
{
    if (!mInitialized)        return NS_ERROR_NOT_AVAILABLE;
    if (!mEnableMemoryDevice) return NS_ERROR_NOT_AVAILABLE;
    if (mMemoryDevice)        return NS_OK;

    mMemoryDevice = new nsMemoryCacheDevice;
    if (!mMemoryDevice)       return NS_ERROR_OUT_OF_MEMORY;

    // set preference
    int32_t capacity = mObserver->MemoryCacheCapacity();
    CACHE_LOG_DEBUG(("Creating memory device with capacity %d\n", capacity));
    mMemoryDevice->SetCapacity(capacity);
    mMemoryDevice->SetMaxEntrySize(mObserver->MemoryCacheMaxEntrySize());

    nsresult rv = mMemoryDevice->Init();
    if (NS_FAILED(rv)) {
        NS_WARNING("Initialization of Memory Cache failed.");
        delete mMemoryDevice;
        mMemoryDevice = nullptr;
    }

    return rv;
}
=======
nsresult nsCacheService::GetCustomOfflineDevice(
    nsIFile* aProfileDir, int32_t aQuota, nsOfflineCacheDevice** aDevice) {
  nsresult rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsCacheService::RemoveCustomOfflineDevice(
    nsOfflineCacheDevice *aDevice) {
  nsCOMPtr<nsIFile> profileDir = aDevice->BaseDirectory();
  if (!profileDir) return NS_ERROR_UNEXPECTED;
||||||| merged common ancestors
nsresult
nsCacheService::RemoveCustomOfflineDevice(nsOfflineCacheDevice *aDevice)
{
    nsCOMPtr<nsIFile> profileDir = aDevice->BaseDirectory();
    if (!profileDir)
        return NS_ERROR_UNEXPECTED;
=======
  nsAutoString profilePath;
  rv = aProfileDir->GetPath(profilePath);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsAutoString profilePath;
  nsresult rv = profileDir->GetPath(profilePath);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
    nsAutoString profilePath;
    nsresult rv = profileDir->GetPath(profilePath);
    NS_ENSURE_SUCCESS(rv, rv);
=======
  if (!mCustomOfflineDevices.Get(profilePath, aDevice)) {
    rv = CreateCustomOfflineDevice(aProfileDir, aQuota, aDevice);
    NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mCustomOfflineDevices.Remove(profilePath);
  return NS_OK;
}

nsresult nsCacheService::CreateRequest(nsCacheSession *session,
                                       const nsACString &clientKey,
                                       nsCacheAccessMode accessRequested,
                                       bool blockingMode,
                                       nsICacheListener *listener,
                                       nsCacheRequest **request) {
  NS_ASSERTION(request, "CreateRequest: request is null");

  nsAutoCString key(*session->ClientID());
  key.Append(':');
  key.Append(clientKey);

  if (mMaxKeyLength < key.Length()) mMaxKeyLength = key.Length();

  // create request
  *request =
      new nsCacheRequest(key, listener, accessRequested, blockingMode, session);

  if (!listener) return NS_OK;  // we're sync, we're done.

  // get the request's thread
  (*request)->mEventTarget = GetCurrentThreadEventTarget();

  return NS_OK;
}

class nsCacheListenerEvent : public Runnable {
 public:
  nsCacheListenerEvent(nsICacheListener *listener,
                       nsICacheEntryDescriptor *descriptor,
                       nsCacheAccessMode accessGranted, nsresult status)
      : mozilla::Runnable("nsCacheListenerEvent"),
        mListener(listener)  // transfers reference
        ,
        mDescriptor(descriptor)  // transfers reference (may be null)
        ,
        mAccessGranted(accessGranted),
        mStatus(status) {}

  NS_IMETHOD Run() override {
    mListener->OnCacheEntryAvailable(mDescriptor, mAccessGranted, mStatus);

    NS_RELEASE(mListener);
    NS_IF_RELEASE(mDescriptor);
    return NS_OK;
  }

 private:
  // We explicitly leak mListener or mDescriptor if Run is not called
  // because otherwise we cannot guarantee that they are destroyed on
  // the right thread.

  nsICacheListener *mListener;
  nsICacheEntryDescriptor *mDescriptor;
  nsCacheAccessMode mAccessGranted;
  nsresult mStatus;
};

nsresult nsCacheService::NotifyListener(nsCacheRequest *request,
                                        nsICacheEntryDescriptor *descriptor,
                                        nsCacheAccessMode accessGranted,
                                        nsresult status) {
  NS_ASSERTION(request->mEventTarget, "no thread set in async request!");

  // Swap ownership, and release listener on target thread...
  nsICacheListener *listener = request->mListener;
  request->mListener = nullptr;

  nsCOMPtr<nsIRunnable> ev =
      new nsCacheListenerEvent(listener, descriptor, accessGranted, status);
  if (!ev) {
    // Better to leak listener and descriptor if we fail because we don't
    // want to destroy them inside the cache service lock or on potentially
    // the wrong thread.
    return NS_ERROR_OUT_OF_MEMORY;
  }

  return request->mEventTarget->Dispatch(ev, NS_DISPATCH_NORMAL);
}

nsresult nsCacheService::ProcessRequest(nsCacheRequest *request,
                                        bool calledFromOpenCacheEntry,
                                        nsICacheEntryDescriptor **result) {
  // !!! must be called with mLock held !!!
  nsresult rv;
  nsCacheEntry *entry = nullptr;
  nsCacheEntry *doomedEntry = nullptr;
  nsCacheAccessMode accessGranted = nsICache::ACCESS_NONE;
  if (result) *result = nullptr;

  while (true) {  // Activate entry loop
    rv = ActivateEntry(request, &entry,
                       &doomedEntry);  // get the entry for this request
    if (NS_FAILED(rv)) break;

    while (true) {  // Request Access loop
      NS_ASSERTION(entry, "no entry in Request Access loop!");
      // entry->RequestAccess queues request on entry
      rv = entry->RequestAccess(request, &accessGranted);
      if (rv != NS_ERROR_CACHE_WAIT_FOR_VALIDATION) break;

      if (request->IsBlocking()) {
        if (request->mListener) {
          // async exits - validate, doom, or close will resume
          return rv;
        }

        // XXX this is probably wrong...
        Unlock();
        rv = request->WaitForValidation();
        Lock(LOCK_TELEM(NSCACHESERVICE_PROCESSREQUEST));
      }

      PR_REMOVE_AND_INIT_LINK(request);
      if (NS_FAILED(rv))
        break;  // non-blocking mode returns WAIT_FOR_VALIDATION error
      // okay, we're ready to process this request, request access again
    }
    if (rv != NS_ERROR_CACHE_ENTRY_DOOMED) break;
||||||| merged common ancestors
    mCustomOfflineDevices.Remove(profilePath);
    return NS_OK;
}

nsresult
nsCacheService::CreateRequest(nsCacheSession *   session,
                              const nsACString & clientKey,
                              nsCacheAccessMode  accessRequested,
                              bool               blockingMode,
                              nsICacheListener * listener,
                              nsCacheRequest **  request)
{
    NS_ASSERTION(request, "CreateRequest: request is null");

    nsAutoCString key(*session->ClientID());
    key.Append(':');
    key.Append(clientKey);

    if (mMaxKeyLength < key.Length()) mMaxKeyLength = key.Length();

    // create request
    *request = new nsCacheRequest(key, listener, accessRequested,
                                  blockingMode, session);

    if (!listener)  return NS_OK;  // we're sync, we're done.

    // get the request's thread
    (*request)->mEventTarget = GetCurrentThreadEventTarget();

    return NS_OK;
}


class nsCacheListenerEvent : public Runnable
{
public:
  nsCacheListenerEvent(nsICacheListener* listener,
                       nsICacheEntryDescriptor* descriptor,
                       nsCacheAccessMode accessGranted,
                       nsresult status)
    : mozilla::Runnable("nsCacheListenerEvent")
    , mListener(listener)     // transfers reference
    , mDescriptor(descriptor) // transfers reference (may be null)
    , mAccessGranted(accessGranted)
    , mStatus(status)
  {
  }

  NS_IMETHOD Run() override
  {
    mListener->OnCacheEntryAvailable(mDescriptor, mAccessGranted, mStatus);

    NS_RELEASE(mListener);
    NS_IF_RELEASE(mDescriptor);
    return NS_OK;
    }

private:
    // We explicitly leak mListener or mDescriptor if Run is not called
    // because otherwise we cannot guarantee that they are destroyed on
    // the right thread.

    nsICacheListener        *mListener;
    nsICacheEntryDescriptor *mDescriptor;
    nsCacheAccessMode        mAccessGranted;
    nsresult                 mStatus;
};


nsresult
nsCacheService::NotifyListener(nsCacheRequest *          request,
                               nsICacheEntryDescriptor * descriptor,
                               nsCacheAccessMode         accessGranted,
                               nsresult                  status)
{
    NS_ASSERTION(request->mEventTarget, "no thread set in async request!");

    // Swap ownership, and release listener on target thread...
    nsICacheListener *listener = request->mListener;
    request->mListener = nullptr;

    nsCOMPtr<nsIRunnable> ev =
            new nsCacheListenerEvent(listener, descriptor,
                                     accessGranted, status);
    if (!ev) {
        // Better to leak listener and descriptor if we fail because we don't
        // want to destroy them inside the cache service lock or on potentially
        // the wrong thread.
        return NS_ERROR_OUT_OF_MEMORY;
    }

    return request->mEventTarget->Dispatch(ev, NS_DISPATCH_NORMAL);
}


nsresult
nsCacheService::ProcessRequest(nsCacheRequest *           request,
                               bool                       calledFromOpenCacheEntry,
                               nsICacheEntryDescriptor ** result)
{
    // !!! must be called with mLock held !!!
    nsresult           rv;
    nsCacheEntry *     entry = nullptr;
    nsCacheEntry *     doomedEntry = nullptr;
    nsCacheAccessMode  accessGranted = nsICache::ACCESS_NONE;
    if (result) *result = nullptr;

    while(true) {  // Activate entry loop
        rv = ActivateEntry(request, &entry, &doomedEntry);  // get the entry for this request
        if (NS_FAILED(rv))  break;

        while (true) { // Request Access loop
            NS_ASSERTION(entry, "no entry in Request Access loop!");
            // entry->RequestAccess queues request on entry
            rv = entry->RequestAccess(request, &accessGranted);
            if (rv != NS_ERROR_CACHE_WAIT_FOR_VALIDATION) break;

            if (request->IsBlocking()) {
                if (request->mListener) {
                    // async exits - validate, doom, or close will resume
                    return rv;
                }

                // XXX this is probably wrong...
                Unlock();
                rv = request->WaitForValidation();
                Lock(LOCK_TELEM(NSCACHESERVICE_PROCESSREQUEST));
            }

            PR_REMOVE_AND_INIT_LINK(request);
            if (NS_FAILED(rv)) break;   // non-blocking mode returns WAIT_FOR_VALIDATION error
            // okay, we're ready to process this request, request access again
        }
        if (rv != NS_ERROR_CACHE_ENTRY_DOOMED)  break;

        if (entry->IsNotInUse()) {
            // this request was the last one keeping it around, so get rid of it
            DeactivateEntry(entry);
        }
        // loop back around to look for another entry
    }

    if (NS_SUCCEEDED(rv) && request->mProfileDir) {
        // Custom cache directory has been demanded.  Preset the cache device.
        if (entry->StoragePolicy() != nsICache::STORE_OFFLINE) {
            // Failsafe check: this is implemented only for offline cache atm.
            rv = NS_ERROR_FAILURE;
        } else {
            RefPtr<nsOfflineCacheDevice> customCacheDevice;
            rv = GetCustomOfflineDevice(request->mProfileDir, -1,
                                        getter_AddRefs(customCacheDevice));
            if (NS_SUCCEEDED(rv))
                entry->SetCustomCacheDevice(customCacheDevice);
        }
    }

    nsICacheEntryDescriptor *descriptor = nullptr;

    if (NS_SUCCEEDED(rv))
        rv = entry->CreateDescriptor(request, accessGranted, &descriptor);

    // If doomedEntry is set, ActivatEntry() doomed an existing entry and
    // created a new one for that cache-key. However, any pending requests
    // on the doomed entry were not processed and we need to do that here.
    // This must be done after adding the created entry to list of active
    // entries (which is done in ActivateEntry()) otherwise the hashkeys crash
    // (see bug ##561313). It is also important to do this after creating a
    // descriptor for this request, or some other request may end up being
    // executed first for the newly created entry.
    // Finally, it is worth to emphasize that if doomedEntry is set,
    // ActivateEntry() created a new entry for the request, which will be
    // initialized by RequestAccess() and they both should have returned NS_OK.
    if (doomedEntry) {
        (void) ProcessPendingRequests(doomedEntry);
        if (doomedEntry->IsNotInUse())
            DeactivateEntry(doomedEntry);
        doomedEntry = nullptr;
    }

    if (request->mListener) {  // Asynchronous

        if (NS_FAILED(rv) && calledFromOpenCacheEntry && request->IsBlocking())
            return rv;  // skip notifying listener, just return rv to caller

        // call listener to report error or descriptor
        nsresult rv2 = NotifyListener(request, descriptor, accessGranted, rv);
        if (NS_FAILED(rv2) && NS_SUCCEEDED(rv)) {
            rv = rv2;  // trigger delete request
        }
    } else {        // Synchronous
        *result = descriptor;
    }
    return rv;
}


nsresult
nsCacheService::OpenCacheEntry(nsCacheSession *           session,
                               const nsACString &         key,
                               nsCacheAccessMode          accessRequested,
                               bool                       blockingMode,
                               nsICacheListener *         listener,
                               nsICacheEntryDescriptor ** result)
{
    CACHE_LOG_DEBUG(("Opening entry for session %p, key %s, mode %d, blocking %d\n",
                     session, PromiseFlatCString(key).get(), accessRequested,
                     blockingMode));
    if (result)
        *result = nullptr;

    if (!gService || !gService->mInitialized)
        return NS_ERROR_NOT_INITIALIZED;

    nsCacheRequest * request = nullptr;

    nsresult rv = gService->CreateRequest(session,
                                          key,
                                          accessRequested,
                                          blockingMode,
                                          listener,
                                          &request);
    if (NS_FAILED(rv))  return rv;

    CACHE_LOG_DEBUG(("Created request %p\n", request));

    // Process the request on the background thread if we are on the main thread
    // and the the request is asynchronous
    if (NS_IsMainThread() && listener && gService->mCacheIOThread) {
        nsCOMPtr<nsIRunnable> ev =
            new nsProcessRequestEvent(request);
        rv = DispatchToCacheIOThread(ev);

        // delete request if we didn't post the event
        if (NS_FAILED(rv))
            delete request;
    }
    else {

        nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_OPENCACHEENTRY));
        rv = gService->ProcessRequest(request, true, result);
=======
    (*aDevice)->SetAutoShutdown();
    mCustomOfflineDevices.Put(profilePath, *aDevice);
  }

  return NS_OK;
}

nsresult nsCacheService::CreateOfflineDevice() {
  CACHE_LOG_INFO(("Creating default offline device"));

  if (mOfflineDevice) return NS_OK;
  if (!nsCacheService::IsInitialized()) {
    return NS_ERROR_NOT_AVAILABLE;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (entry->IsNotInUse()) {
      // this request was the last one keeping it around, so get rid of it
      DeactivateEntry(entry);
    }
    // loop back around to look for another entry
  }
||||||| merged common ancestors
        // delete requests that have completed
        if (!(listener && blockingMode &&
            (rv == NS_ERROR_CACHE_WAIT_FOR_VALIDATION)))
            delete request;
    }
=======
  nsresult rv = CreateCustomOfflineDevice(
      mObserver->OfflineCacheParentDirectory(),
      mObserver->OfflineCacheCapacity(), &mOfflineDevice);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (NS_SUCCEEDED(rv) && request->mProfileDir) {
    // Custom cache directory has been demanded.  Preset the cache device.
    if (entry->StoragePolicy() != nsICache::STORE_OFFLINE) {
      // Failsafe check: this is implemented only for offline cache atm.
      rv = NS_ERROR_FAILURE;
    } else {
      RefPtr<nsOfflineCacheDevice> customCacheDevice;
      rv = GetCustomOfflineDevice(request->mProfileDir, -1,
                                  getter_AddRefs(customCacheDevice));
      if (NS_SUCCEEDED(rv)) entry->SetCustomCacheDevice(customCacheDevice);
    }
  }
||||||| merged common ancestors
    return rv;
}
=======
  return NS_OK;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsICacheEntryDescriptor *descriptor = nullptr;

  if (NS_SUCCEEDED(rv))
    rv = entry->CreateDescriptor(request, accessGranted, &descriptor);

  // If doomedEntry is set, ActivatEntry() doomed an existing entry and
  // created a new one for that cache-key. However, any pending requests
  // on the doomed entry were not processed and we need to do that here.
  // This must be done after adding the created entry to list of active
  // entries (which is done in ActivateEntry()) otherwise the hashkeys crash
  // (see bug ##561313). It is also important to do this after creating a
  // descriptor for this request, or some other request may end up being
  // executed first for the newly created entry.
  // Finally, it is worth to emphasize that if doomedEntry is set,
  // ActivateEntry() created a new entry for the request, which will be
  // initialized by RequestAccess() and they both should have returned NS_OK.
  if (doomedEntry) {
    (void)ProcessPendingRequests(doomedEntry);
    if (doomedEntry->IsNotInUse()) DeactivateEntry(doomedEntry);
    doomedEntry = nullptr;
  }
||||||| merged common ancestors
=======
nsresult nsCacheService::CreateCustomOfflineDevice(
    nsIFile* aProfileDir, int32_t aQuota, nsOfflineCacheDevice** aDevice) {
  NS_ENSURE_ARG(aProfileDir);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (request->mListener) {  // Asynchronous
||||||| merged common ancestors
nsresult
nsCacheService::ActivateEntry(nsCacheRequest * request,
                              nsCacheEntry ** result,
                              nsCacheEntry ** doomedEntry)
{
    CACHE_LOG_DEBUG(("Activate entry for request %p\n", request));
    if (!mInitialized || mClearingEntries)
        return NS_ERROR_NOT_AVAILABLE;
=======
  if (MOZ_LOG_TEST(gCacheLog, LogLevel::Info)) {
    CACHE_LOG_INFO(("Creating custom offline device, %s, %d",
                    aProfileDir->HumanReadablePath().get(), aQuota));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (NS_FAILED(rv) && calledFromOpenCacheEntry && request->IsBlocking())
      return rv;  // skip notifying listener, just return rv to caller
||||||| merged common ancestors
    nsresult        rv = NS_OK;
=======
  if (!mInitialized) return NS_ERROR_NOT_AVAILABLE;
  if (!mEnableOfflineDevice) return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases

<<<<<<< HEAD
    // call listener to report error or descriptor
    nsresult rv2 = NotifyListener(request, descriptor, accessGranted, rv);
    if (NS_FAILED(rv2) && NS_SUCCEEDED(rv)) {
      rv = rv2;  // trigger delete request
    }
  } else {  // Synchronous
    *result = descriptor;
  }
  return rv;
}

nsresult nsCacheService::OpenCacheEntry(nsCacheSession *session,
                                        const nsACString &key,
                                        nsCacheAccessMode accessRequested,
                                        bool blockingMode,
                                        nsICacheListener *listener,
                                        nsICacheEntryDescriptor **result) {
  CACHE_LOG_DEBUG(
      ("Opening entry for session %p, key %s, mode %d, blocking %d\n", session,
       PromiseFlatCString(key).get(), accessRequested, blockingMode));
  if (result) *result = nullptr;

  if (!gService || !gService->mInitialized) return NS_ERROR_NOT_INITIALIZED;

  nsCacheRequest *request = nullptr;

  nsresult rv = gService->CreateRequest(session, key, accessRequested,
                                        blockingMode, listener, &request);
  if (NS_FAILED(rv)) return rv;

  CACHE_LOG_DEBUG(("Created request %p\n", request));

  // Process the request on the background thread if we are on the main thread
  // and the the request is asynchronous
  if (NS_IsMainThread() && listener && gService->mCacheIOThread) {
    nsCOMPtr<nsIRunnable> ev = new nsProcessRequestEvent(request);
    rv = DispatchToCacheIOThread(ev);

    // delete request if we didn't post the event
    if (NS_FAILED(rv)) delete request;
  } else {
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_OPENCACHEENTRY));
    rv = gService->ProcessRequest(request, true, result);

    // delete requests that have completed
    if (!(listener && blockingMode &&
          (rv == NS_ERROR_CACHE_WAIT_FOR_VALIDATION)))
      delete request;
  }
||||||| merged common ancestors
    NS_ASSERTION(request != nullptr, "ActivateEntry called with no request");
    if (result) *result = nullptr;
    if (doomedEntry) *doomedEntry = nullptr;
    if ((!request) || (!result) || (!doomedEntry))
        return NS_ERROR_NULL_POINTER;
=======
  RefPtr<nsOfflineCacheDevice> device = new nsOfflineCacheDevice();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return rv;
}

nsresult nsCacheService::ActivateEntry(nsCacheRequest *request,
                                       nsCacheEntry **result,
                                       nsCacheEntry **doomedEntry) {
  CACHE_LOG_DEBUG(("Activate entry for request %p\n", request));
  if (!mInitialized || mClearingEntries) return NS_ERROR_NOT_AVAILABLE;

  nsresult rv = NS_OK;

  NS_ASSERTION(request != nullptr, "ActivateEntry called with no request");
  if (result) *result = nullptr;
  if (doomedEntry) *doomedEntry = nullptr;
  if ((!request) || (!result) || (!doomedEntry)) return NS_ERROR_NULL_POINTER;

  // check if the request can be satisfied
  if (!mEnableMemoryDevice && !request->IsStreamBased())
    return NS_ERROR_FAILURE;
  if (!IsStorageEnabledForPolicy_Locked(request->StoragePolicy()))
    return NS_ERROR_FAILURE;

  // search active entries (including those not bound to device)
  nsCacheEntry *entry = mActiveEntries.GetEntry(&(request->mKey));
  CACHE_LOG_DEBUG(("Active entry for request %p is %p\n", request, entry));

  if (!entry) {
    // search cache devices for entry
    bool collision = false;
    entry = SearchCacheDevices(&(request->mKey), request->StoragePolicy(),
                               &collision);
    CACHE_LOG_DEBUG(
        ("Device search for request %p returned %p\n", request, entry));
    // When there is a hashkey collision just refuse to cache it...
    if (collision) return NS_ERROR_CACHE_IN_USE;

    if (entry) entry->MarkInitialized();
  } else {
    NS_ASSERTION(entry->IsActive(), "Inactive entry found in mActiveEntries!");
  }
||||||| merged common ancestors
    // check if the request can be satisfied
    if (!mEnableMemoryDevice && !request->IsStreamBased())
        return NS_ERROR_FAILURE;
    if (!IsStorageEnabledForPolicy_Locked(request->StoragePolicy()))
        return NS_ERROR_FAILURE;
=======
  // set the preferences
  device->SetCacheParentDirectory(aProfileDir);
  device->SetCapacity(aQuota);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (entry) {
    ++mCacheHits;
    entry->Fetched();
  } else {
    ++mCacheMisses;
  }
||||||| merged common ancestors
    // search active entries (including those not bound to device)
    nsCacheEntry *entry = mActiveEntries.GetEntry(&(request->mKey));
    CACHE_LOG_DEBUG(("Active entry for request %p is %p\n", request, entry));
=======
  nsresult rv = device->InitWithSqlite(mStorageService);
  if (NS_FAILED(rv)) {
    CACHE_LOG_DEBUG(("OfflineDevice->InitWithSqlite() failed (0x%.8" PRIx32
                     ")\n",
                     static_cast<uint32_t>(rv)));
    CACHE_LOG_DEBUG(("    - disabling offline cache for this session.\n"));
    device = nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (entry && ((request->AccessRequested() == nsICache::ACCESS_WRITE) ||
                ((request->StoragePolicy() != nsICache::STORE_OFFLINE) &&
                 (entry->mExpirationTime <= SecondsFromPRTime(PR_Now()) &&
                  request->WillDoomEntriesIfExpired()))))
||||||| merged common ancestors
    if (!entry) {
        // search cache devices for entry
        bool collision = false;
        entry = SearchCacheDevices(&(request->mKey), request->StoragePolicy(), &collision);
        CACHE_LOG_DEBUG(("Device search for request %p returned %p\n",
                         request, entry));
        // When there is a hashkey collision just refuse to cache it...
        if (collision) return NS_ERROR_CACHE_IN_USE;

        if (entry)  entry->MarkInitialized();
    } else {
        NS_ASSERTION(entry->IsActive(), "Inactive entry found in mActiveEntries!");
    }
=======
  device.forget(aDevice);
  return rv;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  {
    // this is FORCE-WRITE request or the entry has expired
    // we doom entry without processing pending requests, but store it in
    // doomedEntry which causes pending requests to be processed below
    rv = DoomEntry_Internal(entry, false);
    *doomedEntry = entry;
    if (NS_FAILED(rv)) {
      // XXX what to do?  Increment FailedDooms counter?
    }
    entry = nullptr;
  }
||||||| merged common ancestors
    if (entry) {
        ++mCacheHits;
        entry->Fetched();
    } else {
        ++mCacheMisses;
    }
=======
nsresult nsCacheService::RemoveCustomOfflineDevice(
    nsOfflineCacheDevice* aDevice) {
  nsCOMPtr<nsIFile> profileDir = aDevice->BaseDirectory();
  if (!profileDir) return NS_ERROR_UNEXPECTED;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!entry) {
    if (!(request->AccessRequested() & nsICache::ACCESS_WRITE)) {
      // this is a READ-ONLY request
      rv = NS_ERROR_CACHE_KEY_NOT_FOUND;
      goto error;
    }
||||||| merged common ancestors
    if (entry &&
        ((request->AccessRequested() == nsICache::ACCESS_WRITE) ||
         ((request->StoragePolicy() != nsICache::STORE_OFFLINE) &&
          (entry->mExpirationTime <= SecondsFromPRTime(PR_Now()) &&
           request->WillDoomEntriesIfExpired()))))

    {
        // this is FORCE-WRITE request or the entry has expired
        // we doom entry without processing pending requests, but store it in
        // doomedEntry which causes pending requests to be processed below
        rv = DoomEntry_Internal(entry, false);
        *doomedEntry = entry;
        if (NS_FAILED(rv)) {
            // XXX what to do?  Increment FailedDooms counter?
        }
        entry = nullptr;
    }
=======
  nsAutoString profilePath;
  nsresult rv = profileDir->GetPath(profilePath);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
    entry = new nsCacheEntry(request->mKey, request->IsStreamBased(),
                             request->StoragePolicy());
    if (!entry) return NS_ERROR_OUT_OF_MEMORY;
||||||| merged common ancestors
    if (!entry) {
        if (! (request->AccessRequested() & nsICache::ACCESS_WRITE)) {
            // this is a READ-ONLY request
            rv = NS_ERROR_CACHE_KEY_NOT_FOUND;
            goto error;
        }

        entry = new nsCacheEntry(request->mKey,
                                 request->IsStreamBased(),
                                 request->StoragePolicy());
        if (!entry)
            return NS_ERROR_OUT_OF_MEMORY;
=======
  mCustomOfflineDevices.Remove(profilePath);
  return NS_OK;
}

nsresult nsCacheService::CreateRequest(nsCacheSession* session,
                                       const nsACString& clientKey,
                                       nsCacheAccessMode accessRequested,
                                       bool blockingMode,
                                       nsICacheListener* listener,
                                       nsCacheRequest** request) {
  NS_ASSERTION(request, "CreateRequest: request is null");
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (request->IsPrivate()) entry->MarkPrivate();
||||||| merged common ancestors
        if (request->IsPrivate())
            entry->MarkPrivate();
=======
  nsAutoCString key(*session->ClientID());
  key.Append(':');
  key.Append(clientKey);
>>>>>>> upstream-releases

<<<<<<< HEAD
    entry->Fetched();
    ++mTotalEntries;
||||||| merged common ancestors
        entry->Fetched();
        ++mTotalEntries;
=======
  if (mMaxKeyLength < key.Length()) mMaxKeyLength = key.Length();
>>>>>>> upstream-releases

<<<<<<< HEAD
    // XXX  we could perform an early bind in some cases based on storage policy
  }
||||||| merged common ancestors
        // XXX  we could perform an early bind in some cases based on storage policy
    }
=======
  // create request
  *request =
      new nsCacheRequest(key, listener, accessRequested, blockingMode, session);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!entry->IsActive()) {
    rv = mActiveEntries.AddEntry(entry);
    if (NS_FAILED(rv)) goto error;
    CACHE_LOG_DEBUG(("Added entry %p to mActiveEntries\n", entry));
    entry->MarkActive();  // mark entry active, because it's now in
                          // mActiveEntries
  }
  *result = entry;
  return NS_OK;
||||||| merged common ancestors
    if (!entry->IsActive()) {
        rv = mActiveEntries.AddEntry(entry);
        if (NS_FAILED(rv)) goto error;
        CACHE_LOG_DEBUG(("Added entry %p to mActiveEntries\n", entry));
        entry->MarkActive();  // mark entry active, because it's now in mActiveEntries
    }
    *result = entry;
    return NS_OK;
=======
  if (!listener) return NS_OK;  // we're sync, we're done.
>>>>>>> upstream-releases

<<<<<<< HEAD
error:
  *result = nullptr;
  delete entry;
  return rv;
||||||| merged common ancestors
 error:
    *result = nullptr;
    delete entry;
    return rv;
=======
  // get the request's thread
  (*request)->mEventTarget = GetCurrentThreadEventTarget();

  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsCacheEntry *nsCacheService::SearchCacheDevices(nsCString *key,
                                                 nsCacheStoragePolicy policy,
                                                 bool *collision) {
  Telemetry::AutoTimer<Telemetry::CACHE_DEVICE_SEARCH_2> timer;
  nsCacheEntry *entry = nullptr;
||||||| merged common ancestors
=======
class nsCacheListenerEvent : public Runnable {
 public:
  nsCacheListenerEvent(nsICacheListener* listener,
                       nsICacheEntryDescriptor* descriptor,
                       nsCacheAccessMode accessGranted, nsresult status)
      : mozilla::Runnable("nsCacheListenerEvent"),
        mListener(listener)  // transfers reference
        ,
        mDescriptor(descriptor)  // transfers reference (may be null)
        ,
        mAccessGranted(accessGranted),
        mStatus(status) {}

  NS_IMETHOD Run() override {
    mListener->OnCacheEntryAvailable(mDescriptor, mAccessGranted, mStatus);
>>>>>>> upstream-releases

<<<<<<< HEAD
  CACHE_LOG_DEBUG(("mMemoryDevice: 0x%p\n", mMemoryDevice));
||||||| merged common ancestors
nsCacheEntry *
nsCacheService::SearchCacheDevices(nsCString * key, nsCacheStoragePolicy policy, bool *collision)
{
    Telemetry::AutoTimer<Telemetry::CACHE_DEVICE_SEARCH_2> timer;
    nsCacheEntry * entry = nullptr;

    CACHE_LOG_DEBUG(("mMemoryDevice: 0x%p\n", mMemoryDevice));
=======
    NS_RELEASE(mListener);
    NS_IF_RELEASE(mDescriptor);
    return NS_OK;
  }

 private:
  // We explicitly leak mListener or mDescriptor if Run is not called
  // because otherwise we cannot guarantee that they are destroyed on
  // the right thread.
>>>>>>> upstream-releases

<<<<<<< HEAD
  *collision = false;
  if ((policy == nsICache::STORE_ANYWHERE) ||
      (policy == nsICache::STORE_IN_MEMORY)) {
    // If there is no memory device, then there is nothing to search...
    if (mMemoryDevice) {
      entry = mMemoryDevice->FindEntry(key, collision);
      CACHE_LOG_DEBUG(
          ("Searching mMemoryDevice for key %s found: 0x%p, "
           "collision: %d\n",
           key->get(), entry, *collision));
    }
  }
||||||| merged common ancestors
    *collision = false;
    if ((policy == nsICache::STORE_ANYWHERE) || (policy == nsICache::STORE_IN_MEMORY)) {
        // If there is no memory device, then there is nothing to search...
        if (mMemoryDevice) {
            entry = mMemoryDevice->FindEntry(key, collision);
            CACHE_LOG_DEBUG(("Searching mMemoryDevice for key %s found: 0x%p, "
                             "collision: %d\n", key->get(), entry, *collision));
        }
    }
=======
  nsICacheListener* mListener;
  nsICacheEntryDescriptor* mDescriptor;
  nsCacheAccessMode mAccessGranted;
  nsresult mStatus;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!entry && ((policy == nsICache::STORE_ANYWHERE) ||
                 (policy == nsICache::STORE_ON_DISK))) {
    if (mEnableDiskDevice) {
      if (!mDiskDevice) {
        nsresult rv = CreateDiskDevice();
        if (NS_FAILED(rv)) return nullptr;
      }
||||||| merged common ancestors
    if (!entry &&
        ((policy == nsICache::STORE_ANYWHERE) || (policy == nsICache::STORE_ON_DISK))) {

        if (mEnableDiskDevice) {
            if (!mDiskDevice) {
                nsresult rv = CreateDiskDevice();
                if (NS_FAILED(rv))
                    return nullptr;
            }
=======
nsresult nsCacheService::NotifyListener(nsCacheRequest* request,
                                        nsICacheEntryDescriptor* descriptor,
                                        nsCacheAccessMode accessGranted,
                                        nsresult status) {
  NS_ASSERTION(request->mEventTarget, "no thread set in async request!");

  // Swap ownership, and release listener on target thread...
  nsICacheListener* listener = request->mListener;
  request->mListener = nullptr;

  nsCOMPtr<nsIRunnable> ev =
      new nsCacheListenerEvent(listener, descriptor, accessGranted, status);
  if (!ev) {
    // Better to leak listener and descriptor if we fail because we don't
    // want to destroy them inside the cache service lock or on potentially
    // the wrong thread.
    return NS_ERROR_OUT_OF_MEMORY;
  }

  return request->mEventTarget->Dispatch(ev, NS_DISPATCH_NORMAL);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
      entry = mDiskDevice->FindEntry(key, collision);
    }
  }
||||||| merged common ancestors
            entry = mDiskDevice->FindEntry(key, collision);
        }
    }
=======
nsresult nsCacheService::ProcessRequest(nsCacheRequest* request,
                                        bool calledFromOpenCacheEntry,
                                        nsICacheEntryDescriptor** result) {
  // !!! must be called with mLock held !!!
  nsresult rv;
  nsCacheEntry* entry = nullptr;
  nsCacheEntry* doomedEntry = nullptr;
  nsCacheAccessMode accessGranted = nsICache::ACCESS_NONE;
  if (result) *result = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!entry &&
      (policy == nsICache::STORE_OFFLINE ||
       (policy == nsICache::STORE_ANYWHERE && gIOService->IsOffline()))) {
    if (mEnableOfflineDevice) {
      if (!mOfflineDevice) {
        nsresult rv = CreateOfflineDevice();
        if (NS_FAILED(rv)) return nullptr;
      }
||||||| merged common ancestors
    if (!entry && (policy == nsICache::STORE_OFFLINE ||
                   (policy == nsICache::STORE_ANYWHERE &&
                    gIOService->IsOffline()))) {

        if (mEnableOfflineDevice) {
            if (!mOfflineDevice) {
                nsresult rv = CreateOfflineDevice();
                if (NS_FAILED(rv))
                    return nullptr;
            }
=======
  while (true) {  // Activate entry loop
    rv = ActivateEntry(request, &entry,
                       &doomedEntry);  // get the entry for this request
    if (NS_FAILED(rv)) break;

    while (true) {  // Request Access loop
      NS_ASSERTION(entry, "no entry in Request Access loop!");
      // entry->RequestAccess queues request on entry
      rv = entry->RequestAccess(request, &accessGranted);
      if (rv != NS_ERROR_CACHE_WAIT_FOR_VALIDATION) break;
>>>>>>> upstream-releases

<<<<<<< HEAD
      entry = mOfflineDevice->FindEntry(key, collision);
    }
  }

  return entry;
}

nsCacheDevice *nsCacheService::EnsureEntryHasDevice(nsCacheEntry *entry) {
  nsCacheDevice *device = entry->CacheDevice();
  // return device if found, possibly null if the entry is doomed i.e prevent
  // doomed entries to bind to a device (see e.g. bugs #548406 and #596443)
  if (device || entry->IsDoomed()) return device;
||||||| merged common ancestors
            entry = mOfflineDevice->FindEntry(key, collision);
        }
    }

    return entry;
}

=======
      if (request->IsBlocking()) {
        if (request->mListener) {
          // async exits - validate, doom, or close will resume
          return rv;
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
  int64_t predictedDataSize = entry->PredictedDataSize();
  if (entry->IsStreamData() && entry->IsAllowedOnDisk() && mEnableDiskDevice) {
    // this is the default
    if (!mDiskDevice) {
      (void)CreateDiskDevice();  // ignore the error (check for mDiskDevice
                                 // instead)
    }
||||||| merged common ancestors
nsCacheDevice *
nsCacheService::EnsureEntryHasDevice(nsCacheEntry * entry)
{
    nsCacheDevice * device = entry->CacheDevice();
    // return device if found, possibly null if the entry is doomed i.e prevent
    // doomed entries to bind to a device (see e.g. bugs #548406 and #596443)
    if (device || entry->IsDoomed())  return device;
=======
        // XXX this is probably wrong...
        Unlock();
        rv = request->WaitForValidation();
        Lock(LOCK_TELEM(NSCACHESERVICE_PROCESSREQUEST));
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (mDiskDevice) {
      // Bypass the cache if Content-Length says the entry will be too big
      if (predictedDataSize != -1 &&
          mDiskDevice->EntryIsTooBig(predictedDataSize)) {
        DebugOnly<nsresult> rv = nsCacheService::DoomEntry(entry);
        NS_ASSERTION(NS_SUCCEEDED(rv), "DoomEntry() failed.");
        return nullptr;
      }
||||||| merged common ancestors
    int64_t predictedDataSize = entry->PredictedDataSize();
    if (entry->IsStreamData() && entry->IsAllowedOnDisk() && mEnableDiskDevice) {
        // this is the default
        if (!mDiskDevice) {
            (void)CreateDiskDevice();  // ignore the error (check for mDiskDevice instead)
        }
=======
      PR_REMOVE_AND_INIT_LINK(request);
      if (NS_FAILED(rv))
        break;  // non-blocking mode returns WAIT_FOR_VALIDATION error
      // okay, we're ready to process this request, request access again
    }
    if (rv != NS_ERROR_CACHE_ENTRY_DOOMED) break;
>>>>>>> upstream-releases

<<<<<<< HEAD
      entry->MarkBinding();  // enter state of binding
      nsresult rv = mDiskDevice->BindEntry(entry);
      entry->ClearBinding();  // exit state of binding
      if (NS_SUCCEEDED(rv)) device = mDiskDevice;
||||||| merged common ancestors
        if (mDiskDevice) {
            // Bypass the cache if Content-Length says the entry will be too big
            if (predictedDataSize != -1 &&
                mDiskDevice->EntryIsTooBig(predictedDataSize)) {
                DebugOnly<nsresult> rv = nsCacheService::DoomEntry(entry);
                NS_ASSERTION(NS_SUCCEEDED(rv),"DoomEntry() failed.");
                return nullptr;
            }

            entry->MarkBinding();  // enter state of binding
            nsresult rv = mDiskDevice->BindEntry(entry);
            entry->ClearBinding(); // exit state of binding
            if (NS_SUCCEEDED(rv))
                device = mDiskDevice;
        }
=======
    if (entry->IsNotInUse()) {
      // this request was the last one keeping it around, so get rid of it
      DeactivateEntry(entry);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
    // loop back around to look for another entry
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // if we can't use mDiskDevice, try mMemoryDevice
  if (!device && mEnableMemoryDevice && entry->IsAllowedInMemory()) {
    if (!mMemoryDevice) {
      (void)CreateMemoryDevice();  // ignore the error (check for mMemoryDevice
                                   // instead)
||||||| merged common ancestors
    // if we can't use mDiskDevice, try mMemoryDevice
    if (!device && mEnableMemoryDevice && entry->IsAllowedInMemory()) {
        if (!mMemoryDevice) {
            (void)CreateMemoryDevice();  // ignore the error (check for mMemoryDevice instead)
        }
        if (mMemoryDevice) {
            // Bypass the cache if Content-Length says entry will be too big
            if (predictedDataSize != -1 &&
                mMemoryDevice->EntryIsTooBig(predictedDataSize)) {
                DebugOnly<nsresult> rv = nsCacheService::DoomEntry(entry);
                NS_ASSERTION(NS_SUCCEEDED(rv),"DoomEntry() failed.");
                return nullptr;
            }

            entry->MarkBinding();  // enter state of binding
            nsresult rv = mMemoryDevice->BindEntry(entry);
            entry->ClearBinding(); // exit state of binding
            if (NS_SUCCEEDED(rv))
                device = mMemoryDevice;
        }
=======
  if (NS_SUCCEEDED(rv) && request->mProfileDir) {
    // Custom cache directory has been demanded.  Preset the cache device.
    if (entry->StoragePolicy() != nsICache::STORE_OFFLINE) {
      // Failsafe check: this is implemented only for offline cache atm.
      rv = NS_ERROR_FAILURE;
    } else {
      RefPtr<nsOfflineCacheDevice> customCacheDevice;
      rv = GetCustomOfflineDevice(request->mProfileDir, -1,
                                  getter_AddRefs(customCacheDevice));
      if (NS_SUCCEEDED(rv)) entry->SetCustomCacheDevice(customCacheDevice);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (mMemoryDevice) {
      // Bypass the cache if Content-Length says entry will be too big
      if (predictedDataSize != -1 &&
          mMemoryDevice->EntryIsTooBig(predictedDataSize)) {
        DebugOnly<nsresult> rv = nsCacheService::DoomEntry(entry);
        NS_ASSERTION(NS_SUCCEEDED(rv), "DoomEntry() failed.");
        return nullptr;
      }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      entry->MarkBinding();  // enter state of binding
      nsresult rv = mMemoryDevice->BindEntry(entry);
      entry->ClearBinding();  // exit state of binding
      if (NS_SUCCEEDED(rv)) device = mMemoryDevice;
    }
  }
||||||| merged common ancestors
    if (!device && entry->IsStreamData() &&
        entry->IsAllowedOffline() && mEnableOfflineDevice) {
        if (!mOfflineDevice) {
            (void)CreateOfflineDevice(); // ignore the error (check for mOfflineDevice instead)
        }
=======
  nsICacheEntryDescriptor* descriptor = nullptr;

  if (NS_SUCCEEDED(rv))
    rv = entry->CreateDescriptor(request, accessGranted, &descriptor);

  // If doomedEntry is set, ActivatEntry() doomed an existing entry and
  // created a new one for that cache-key. However, any pending requests
  // on the doomed entry were not processed and we need to do that here.
  // This must be done after adding the created entry to list of active
  // entries (which is done in ActivateEntry()) otherwise the hashkeys crash
  // (see bug ##561313). It is also important to do this after creating a
  // descriptor for this request, or some other request may end up being
  // executed first for the newly created entry.
  // Finally, it is worth to emphasize that if doomedEntry is set,
  // ActivateEntry() created a new entry for the request, which will be
  // initialized by RequestAccess() and they both should have returned NS_OK.
  if (doomedEntry) {
    (void)ProcessPendingRequests(doomedEntry);
    if (doomedEntry->IsNotInUse()) DeactivateEntry(doomedEntry);
    doomedEntry = nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!device && entry->IsStreamData() && entry->IsAllowedOffline() &&
      mEnableOfflineDevice) {
    if (!mOfflineDevice) {
      (void)CreateOfflineDevice();  // ignore the error (check for
                                    // mOfflineDevice instead)
    }
||||||| merged common ancestors
        device = entry->CustomCacheDevice()
               ? entry->CustomCacheDevice()
               : mOfflineDevice;
=======
  if (request->mListener) {  // Asynchronous
>>>>>>> upstream-releases

<<<<<<< HEAD
    device = entry->CustomCacheDevice() ? entry->CustomCacheDevice()
                                        : mOfflineDevice;

    if (device) {
      entry->MarkBinding();
      nsresult rv = device->BindEntry(entry);
      entry->ClearBinding();
      if (NS_FAILED(rv)) device = nullptr;
    }
  }
||||||| merged common ancestors
        if (device) {
            entry->MarkBinding();
            nsresult rv = device->BindEntry(entry);
            entry->ClearBinding();
            if (NS_FAILED(rv))
                device = nullptr;
        }
    }
=======
    if (NS_FAILED(rv) && calledFromOpenCacheEntry && request->IsBlocking())
      return rv;  // skip notifying listener, just return rv to caller
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (device) entry->SetCacheDevice(device);
  return device;
}
||||||| merged common ancestors
    if (device)
        entry->SetCacheDevice(device);
    return device;
}
=======
    // call listener to report error or descriptor
    nsresult rv2 = NotifyListener(request, descriptor, accessGranted, rv);
    if (NS_FAILED(rv2) && NS_SUCCEEDED(rv)) {
      rv = rv2;  // trigger delete request
    }
  } else {  // Synchronous
    *result = descriptor;
  }
  return rv;
}

nsresult nsCacheService::OpenCacheEntry(nsCacheSession* session,
                                        const nsACString& key,
                                        nsCacheAccessMode accessRequested,
                                        bool blockingMode,
                                        nsICacheListener* listener,
                                        nsICacheEntryDescriptor** result) {
  CACHE_LOG_DEBUG(
      ("Opening entry for session %p, key %s, mode %d, blocking %d\n", session,
       PromiseFlatCString(key).get(), accessRequested, blockingMode));
  if (result) *result = nullptr;

  if (!gService || !gService->mInitialized) return NS_ERROR_NOT_INITIALIZED;

  nsCacheRequest* request = nullptr;

  nsresult rv = gService->CreateRequest(session, key, accessRequested,
                                        blockingMode, listener, &request);
  if (NS_FAILED(rv)) return rv;

  CACHE_LOG_DEBUG(("Created request %p\n", request));

  // Process the request on the background thread if we are on the main thread
  // and the the request is asynchronous
  if (NS_IsMainThread() && listener && gService->mCacheIOThread) {
    nsCOMPtr<nsIRunnable> ev = new nsProcessRequestEvent(request);
    rv = DispatchToCacheIOThread(ev);

    // delete request if we didn't post the event
    if (NS_FAILED(rv)) delete request;
  } else {
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_OPENCACHEENTRY));
    rv = gService->ProcessRequest(request, true, result);

    // delete requests that have completed
    if (!(listener && blockingMode &&
          (rv == NS_ERROR_CACHE_WAIT_FOR_VALIDATION)))
      delete request;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsCacheService::DoomEntry(nsCacheEntry *entry) {
  return gService->DoomEntry_Internal(entry, true);
||||||| merged common ancestors
nsresult
nsCacheService::DoomEntry(nsCacheEntry * entry)
{
    return gService->DoomEntry_Internal(entry, true);
=======
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsCacheService::DoomEntry_Internal(nsCacheEntry *entry,
                                            bool doProcessPendingRequests) {
  if (entry->IsDoomed()) return NS_OK;
||||||| merged common ancestors
=======
nsresult nsCacheService::ActivateEntry(nsCacheRequest* request,
                                       nsCacheEntry** result,
                                       nsCacheEntry** doomedEntry) {
  CACHE_LOG_DEBUG(("Activate entry for request %p\n", request));
  if (!mInitialized || mClearingEntries) return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases

<<<<<<< HEAD
  CACHE_LOG_DEBUG(("Dooming entry %p\n", entry));
  nsresult rv = NS_OK;
  entry->MarkDoomed();
||||||| merged common ancestors
nsresult
nsCacheService::DoomEntry_Internal(nsCacheEntry * entry,
                                   bool doProcessPendingRequests)
{
    if (entry->IsDoomed())  return NS_OK;

    CACHE_LOG_DEBUG(("Dooming entry %p\n", entry));
    nsresult  rv = NS_OK;
    entry->MarkDoomed();
=======
  nsresult rv = NS_OK;

  NS_ASSERTION(request != nullptr, "ActivateEntry called with no request");
  if (result) *result = nullptr;
  if (doomedEntry) *doomedEntry = nullptr;
  if ((!request) || (!result) || (!doomedEntry)) return NS_ERROR_NULL_POINTER;
>>>>>>> upstream-releases

<<<<<<< HEAD
  NS_ASSERTION(!entry->IsBinding(), "Dooming entry while binding device.");
  nsCacheDevice *device = entry->CacheDevice();
  if (device) device->DoomEntry(entry);
||||||| merged common ancestors
    NS_ASSERTION(!entry->IsBinding(), "Dooming entry while binding device.");
    nsCacheDevice * device = entry->CacheDevice();
    if (device)  device->DoomEntry(entry);
=======
  // check if the request can be satisfied
  if (!request->IsStreamBased()) return NS_ERROR_FAILURE;
  if (!IsStorageEnabledForPolicy_Locked(request->StoragePolicy()))
    return NS_ERROR_FAILURE;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (entry->IsActive()) {
    // remove from active entries
    mActiveEntries.RemoveEntry(entry);
    CACHE_LOG_DEBUG(("Removed entry %p from mActiveEntries\n", entry));
    entry->MarkInactive();
  }
||||||| merged common ancestors
    if (entry->IsActive()) {
        // remove from active entries
        mActiveEntries.RemoveEntry(entry);
        CACHE_LOG_DEBUG(("Removed entry %p from mActiveEntries\n", entry));
        entry->MarkInactive();
     }
=======
  // search active entries (including those not bound to device)
  nsCacheEntry* entry = mActiveEntries.GetEntry(&(request->mKey));
  CACHE_LOG_DEBUG(("Active entry for request %p is %p\n", request, entry));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // put on doom list to wait for descriptors to close
  NS_ASSERTION(PR_CLIST_IS_EMPTY(entry), "doomed entry still on device list");
  PR_APPEND_LINK(entry, &mDoomedEntries);
||||||| merged common ancestors
    // put on doom list to wait for descriptors to close
    NS_ASSERTION(PR_CLIST_IS_EMPTY(entry), "doomed entry still on device list");
    PR_APPEND_LINK(entry, &mDoomedEntries);
=======
  if (!entry) {
    // search cache devices for entry
    bool collision = false;
    entry = SearchCacheDevices(&(request->mKey), request->StoragePolicy(),
                               &collision);
    CACHE_LOG_DEBUG(
        ("Device search for request %p returned %p\n", request, entry));
    // When there is a hashkey collision just refuse to cache it...
    if (collision) return NS_ERROR_CACHE_IN_USE;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // handle pending requests only if we're supposed to
  if (doProcessPendingRequests) {
    // tell pending requests to get on with their lives...
    rv = ProcessPendingRequests(entry);
||||||| merged common ancestors
    // handle pending requests only if we're supposed to
    if (doProcessPendingRequests) {
        // tell pending requests to get on with their lives...
        rv = ProcessPendingRequests(entry);
=======
    if (entry) entry->MarkInitialized();
  } else {
    NS_ASSERTION(entry->IsActive(), "Inactive entry found in mActiveEntries!");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // All requests have been removed, but there may still be open descriptors
    if (entry->IsNotInUse()) {
      DeactivateEntry(entry);  // tell device to get rid of it
    }
  }
  return rv;
}
||||||| merged common ancestors
        // All requests have been removed, but there may still be open descriptors
        if (entry->IsNotInUse()) {
            DeactivateEntry(entry); // tell device to get rid of it
        }
    }
    return rv;
}
=======
  if (entry) {
    ++mCacheHits;
    entry->Fetched();
  } else {
    ++mCacheMisses;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void nsCacheService::OnProfileShutdown() {
  if (!gService || !gService->mInitialized) {
    // The cache service has been shut down, but someone is still holding
    // a reference to it. Ignore this call.
    return;
  }
||||||| merged common ancestors
=======
  if (entry && ((request->AccessRequested() == nsICache::ACCESS_WRITE) ||
                ((request->StoragePolicy() != nsICache::STORE_OFFLINE) &&
                 (entry->mExpirationTime <= SecondsFromPRTime(PR_Now()) &&
                  request->WillDoomEntriesIfExpired()))))
>>>>>>> upstream-releases

<<<<<<< HEAD
  {
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_ONPROFILESHUTDOWN));
    gService->mClearingEntries = true;
    gService->DoomActiveEntries(nullptr);
  }
||||||| merged common ancestors
void
nsCacheService::OnProfileShutdown()
{
    if (!gService || !gService->mInitialized) {
        // The cache service has been shut down, but someone is still holding
        // a reference to it. Ignore this call.
        return;
    }

    {
        nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_ONPROFILESHUTDOWN));
        gService->mClearingEntries = true;
        gService->DoomActiveEntries(nullptr);
    }
=======
  {
    // this is FORCE-WRITE request or the entry has expired
    // we doom entry without processing pending requests, but store it in
    // doomedEntry which causes pending requests to be processed below
    rv = DoomEntry_Internal(entry, false);
    *doomedEntry = entry;
    if (NS_FAILED(rv)) {
      // XXX what to do?  Increment FailedDooms counter?
    }
    entry = nullptr;
  }

  if (!entry) {
    if (!(request->AccessRequested() & nsICache::ACCESS_WRITE)) {
      // this is a READ-ONLY request
      rv = NS_ERROR_CACHE_KEY_NOT_FOUND;
      goto error;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  gService->CloseAllStreams();

  nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_ONPROFILESHUTDOWN));
  gService->ClearDoomList();

  // Make sure to wait for any pending cache-operations before
  // proceeding with destructive actions (bug #620660)
  (void)SyncWithCacheIOThread();
||||||| merged common ancestors
    gService->CloseAllStreams();

    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_ONPROFILESHUTDOWN));
    gService->ClearDoomList();

    // Make sure to wait for any pending cache-operations before
    // proceeding with destructive actions (bug #620660)
    (void) SyncWithCacheIOThread();
=======
    entry = new nsCacheEntry(request->mKey, request->IsStreamBased(),
                             request->StoragePolicy());
    if (!entry) return NS_ERROR_OUT_OF_MEMORY;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (gService->mDiskDevice && gService->mEnableDiskDevice) {
    gService->mDiskDevice->Shutdown();
  }
  gService->mEnableDiskDevice = false;
||||||| merged common ancestors
    if (gService->mDiskDevice && gService->mEnableDiskDevice) {
        gService->mDiskDevice->Shutdown();
    }
    gService->mEnableDiskDevice = false;
=======
    if (request->IsPrivate()) entry->MarkPrivate();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (gService->mOfflineDevice && gService->mEnableOfflineDevice) {
    gService->mOfflineDevice->Shutdown();
  }
  for (auto iter = gService->mCustomOfflineDevices.Iter(); !iter.Done();
       iter.Next()) {
    iter.Data()->Shutdown();
    iter.Remove();
  }
||||||| merged common ancestors
    if (gService->mOfflineDevice && gService->mEnableOfflineDevice) {
        gService->mOfflineDevice->Shutdown();
    }
    for (auto iter = gService->mCustomOfflineDevices.Iter();
         !iter.Done(); iter.Next()) {
        iter.Data()->Shutdown();
        iter.Remove();
    }
=======
    entry->Fetched();
    ++mTotalEntries;
>>>>>>> upstream-releases

<<<<<<< HEAD
  gService->mEnableOfflineDevice = false;
||||||| merged common ancestors
    gService->mEnableOfflineDevice = false;
=======
    // XXX  we could perform an early bind in some cases based on storage policy
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (gService->mMemoryDevice) {
    // clear memory cache
    gService->mMemoryDevice->EvictEntries(nullptr);
  }
||||||| merged common ancestors
    if (gService->mMemoryDevice) {
        // clear memory cache
        gService->mMemoryDevice->EvictEntries(nullptr);
    }
=======
  if (!entry->IsActive()) {
    rv = mActiveEntries.AddEntry(entry);
    if (NS_FAILED(rv)) goto error;
    CACHE_LOG_DEBUG(("Added entry %p to mActiveEntries\n", entry));
    entry->MarkActive();  // mark entry active, because it's now in
                          // mActiveEntries
  }
  *result = entry;
  return NS_OK;
>>>>>>> upstream-releases

<<<<<<< HEAD
  gService->mClearingEntries = false;
||||||| merged common ancestors
    gService->mClearingEntries = false;
=======
error:
  *result = nullptr;
  delete entry;
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsCacheService::OnProfileChanged() {
  if (!gService) return;
||||||| merged common ancestors
=======
nsCacheEntry* nsCacheService::SearchCacheDevices(nsCString* key,
                                                 nsCacheStoragePolicy policy,
                                                 bool* collision) {
  Telemetry::AutoTimer<Telemetry::CACHE_DEVICE_SEARCH_2> timer;
  nsCacheEntry* entry = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  CACHE_LOG_DEBUG(("nsCacheService::OnProfileChanged"));
||||||| merged common ancestors
void
nsCacheService::OnProfileChanged()
{
    if (!gService)  return;

    CACHE_LOG_DEBUG(("nsCacheService::OnProfileChanged"));
=======
  *collision = false;
  if (policy == nsICache::STORE_OFFLINE ||
      (policy == nsICache::STORE_ANYWHERE && gIOService->IsOffline())) {
    if (mEnableOfflineDevice) {
      if (!mOfflineDevice) {
        nsresult rv = CreateOfflineDevice();
        if (NS_FAILED(rv)) return nullptr;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_ONPROFILECHANGED));
||||||| merged common ancestors
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_ONPROFILECHANGED));
=======
      entry = mOfflineDevice->FindEntry(key, collision);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  gService->mEnableDiskDevice = gService->mObserver->DiskCacheEnabled();
  gService->mEnableOfflineDevice = gService->mObserver->OfflineCacheEnabled();
  gService->mEnableMemoryDevice = gService->mObserver->MemoryCacheEnabled();
||||||| merged common ancestors
    gService->mEnableDiskDevice    = gService->mObserver->DiskCacheEnabled();
    gService->mEnableOfflineDevice = gService->mObserver->OfflineCacheEnabled();
    gService->mEnableMemoryDevice  = gService->mObserver->MemoryCacheEnabled();
=======
  return entry;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (gService->mDiskDevice) {
    gService->mDiskDevice->SetCacheParentDirectory(
        gService->mObserver->DiskCacheParentDirectory());
    gService->mDiskDevice->SetCapacity(
        gService->mObserver->DiskCacheCapacity());
||||||| merged common ancestors
    if (gService->mDiskDevice) {
        gService->mDiskDevice->SetCacheParentDirectory(gService->mObserver->DiskCacheParentDirectory());
        gService->mDiskDevice->SetCapacity(gService->mObserver->DiskCacheCapacity());
=======
nsCacheDevice* nsCacheService::EnsureEntryHasDevice(nsCacheEntry* entry) {
  nsCacheDevice* device = entry->CacheDevice();
  // return device if found, possibly null if the entry is doomed i.e prevent
  // doomed entries to bind to a device (see e.g. bugs #548406 and #596443)
  if (device || entry->IsDoomed()) return device;
>>>>>>> upstream-releases

<<<<<<< HEAD
    // XXX initialization of mDiskDevice could be made lazily, if
    // mEnableDiskDevice is false
    nsresult rv = gService->mDiskDevice->Init();
    if (NS_FAILED(rv)) {
      NS_ERROR(
          "nsCacheService::OnProfileChanged: Re-initializing disk device "
          "failed");
      gService->mEnableDiskDevice = false;
      // XXX delete mDiskDevice?
||||||| merged common ancestors
        // XXX initialization of mDiskDevice could be made lazily, if mEnableDiskDevice is false
        nsresult rv = gService->mDiskDevice->Init();
        if (NS_FAILED(rv)) {
            NS_ERROR("nsCacheService::OnProfileChanged: Re-initializing disk device failed");
            gService->mEnableDiskDevice = false;
            // XXX delete mDiskDevice?
        }
=======
  if (!device && entry->IsStreamData() && entry->IsAllowedOffline() &&
      mEnableOfflineDevice) {
    if (!mOfflineDevice) {
      (void)CreateOfflineDevice();  // ignore the error (check for
                                    // mOfflineDevice instead)
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (gService->mOfflineDevice) {
    gService->mOfflineDevice->SetCacheParentDirectory(
        gService->mObserver->OfflineCacheParentDirectory());
    gService->mOfflineDevice->SetCapacity(
        gService->mObserver->OfflineCacheCapacity());
||||||| merged common ancestors
    if (gService->mOfflineDevice) {
        gService->mOfflineDevice->SetCacheParentDirectory(gService->mObserver->OfflineCacheParentDirectory());
        gService->mOfflineDevice->SetCapacity(gService->mObserver->OfflineCacheCapacity());
=======
    device = entry->CustomCacheDevice() ? entry->CustomCacheDevice()
                                        : mOfflineDevice;
>>>>>>> upstream-releases

<<<<<<< HEAD
    // XXX initialization of mOfflineDevice could be made lazily, if
    // mEnableOfflineDevice is false
    nsresult rv =
        gService->mOfflineDevice->InitWithSqlite(gService->mStorageService);
    if (NS_FAILED(rv)) {
      NS_ERROR(
          "nsCacheService::OnProfileChanged: Re-initializing offline device "
          "failed");
      gService->mEnableOfflineDevice = false;
      // XXX delete mOfflineDevice?
||||||| merged common ancestors
        // XXX initialization of mOfflineDevice could be made lazily, if mEnableOfflineDevice is false
        nsresult rv = gService->mOfflineDevice->InitWithSqlite(gService->mStorageService);
        if (NS_FAILED(rv)) {
            NS_ERROR("nsCacheService::OnProfileChanged: Re-initializing offline device failed");
            gService->mEnableOfflineDevice = false;
            // XXX delete mOfflineDevice?
        }
=======
    if (device) {
      entry->MarkBinding();
      nsresult rv = device->BindEntry(entry);
      entry->ClearBinding();
      if (NS_FAILED(rv)) device = nullptr;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  // If memoryDevice exists, reset its size to the new profile
  if (gService->mMemoryDevice) {
    if (gService->mEnableMemoryDevice) {
      // make sure that capacity is reset to the right value
      int32_t capacity = gService->mObserver->MemoryCacheCapacity();
      CACHE_LOG_DEBUG(("Resetting memory device capacity to %d\n", capacity));
      gService->mMemoryDevice->SetCapacity(capacity);
    } else {
      // tell memory device to evict everything
      CACHE_LOG_DEBUG(("memory device disabled\n"));
      gService->mMemoryDevice->SetCapacity(0);
      // Don't delete memory device, because some entries may be active still...
    }
  }
||||||| merged common ancestors
    // If memoryDevice exists, reset its size to the new profile
    if (gService->mMemoryDevice) {
        if (gService->mEnableMemoryDevice) {
            // make sure that capacity is reset to the right value
            int32_t capacity = gService->mObserver->MemoryCacheCapacity();
            CACHE_LOG_DEBUG(("Resetting memory device capacity to %d\n",
                             capacity));
            gService->mMemoryDevice->SetCapacity(capacity);
        } else {
            // tell memory device to evict everything
            CACHE_LOG_DEBUG(("memory device disabled\n"));
            gService->mMemoryDevice->SetCapacity(0);
            // Don't delete memory device, because some entries may be active still...
        }
    }
=======
  if (device) entry->SetCacheDevice(device);
  return device;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsCacheService::SetDiskCacheEnabled(bool enabled) {
  if (!gService) return;
  nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SETDISKCACHEENABLED));
  gService->mEnableDiskDevice = enabled;
||||||| merged common ancestors

void
nsCacheService::SetDiskCacheEnabled(bool    enabled)
{
    if (!gService)  return;
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SETDISKCACHEENABLED));
    gService->mEnableDiskDevice = enabled;
=======
nsresult nsCacheService::DoomEntry(nsCacheEntry* entry) {
  return gService->DoomEntry_Internal(entry, true);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsCacheService::SetDiskCacheCapacity(int32_t capacity) {
  if (!gService) return;
  nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SETDISKCACHECAPACITY));
||||||| merged common ancestors
=======
nsresult nsCacheService::DoomEntry_Internal(nsCacheEntry* entry,
                                            bool doProcessPendingRequests) {
  if (entry->IsDoomed()) return NS_OK;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (gService->mDiskDevice) {
    gService->mDiskDevice->SetCapacity(capacity);
  }
||||||| merged common ancestors
void
nsCacheService::SetDiskCacheCapacity(int32_t  capacity)
{
    if (!gService)  return;
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SETDISKCACHECAPACITY));

    if (gService->mDiskDevice) {
        gService->mDiskDevice->SetCapacity(capacity);
    }
=======
  CACHE_LOG_DEBUG(("Dooming entry %p\n", entry));
  nsresult rv = NS_OK;
  entry->MarkDoomed();

  NS_ASSERTION(!entry->IsBinding(), "Dooming entry while binding device.");
  nsCacheDevice* device = entry->CacheDevice();
  if (device) device->DoomEntry(entry);
>>>>>>> upstream-releases

<<<<<<< HEAD
  gService->mEnableDiskDevice = gService->mObserver->DiskCacheEnabled();
}
||||||| merged common ancestors
    gService->mEnableDiskDevice = gService->mObserver->DiskCacheEnabled();
}
=======
  if (entry->IsActive()) {
    // remove from active entries
    mActiveEntries.RemoveEntry(entry);
    CACHE_LOG_DEBUG(("Removed entry %p from mActiveEntries\n", entry));
    entry->MarkInactive();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void nsCacheService::SetDiskCacheMaxEntrySize(int32_t maxSize) {
  if (!gService) return;
  nsCacheServiceAutoLock lock(
      LOCK_TELEM(NSCACHESERVICE_SETDISKCACHEMAXENTRYSIZE));
||||||| merged common ancestors
void
nsCacheService::SetDiskCacheMaxEntrySize(int32_t  maxSize)
{
    if (!gService)  return;
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SETDISKCACHEMAXENTRYSIZE));
=======
  // put on doom list to wait for descriptors to close
  NS_ASSERTION(PR_CLIST_IS_EMPTY(entry), "doomed entry still on device list");
  PR_APPEND_LINK(entry, &mDoomedEntries);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (gService->mDiskDevice) {
    gService->mDiskDevice->SetMaxEntrySize(maxSize);
  }
||||||| merged common ancestors
    if (gService->mDiskDevice) {
        gService->mDiskDevice->SetMaxEntrySize(maxSize);
    }
=======
  // handle pending requests only if we're supposed to
  if (doProcessPendingRequests) {
    // tell pending requests to get on with their lives...
    rv = ProcessPendingRequests(entry);

    // All requests have been removed, but there may still be open descriptors
    if (entry->IsNotInUse()) {
      DeactivateEntry(entry);  // tell device to get rid of it
    }
  }
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsCacheService::SetMemoryCacheMaxEntrySize(int32_t maxSize) {
  if (!gService) return;
  nsCacheServiceAutoLock lock(
      LOCK_TELEM(NSCACHESERVICE_SETMEMORYCACHEMAXENTRYSIZE));
||||||| merged common ancestors
void
nsCacheService::SetMemoryCacheMaxEntrySize(int32_t  maxSize)
{
    if (!gService)  return;
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SETMEMORYCACHEMAXENTRYSIZE));
=======
void nsCacheService::OnProfileShutdown() {
  if (!gService || !gService->mInitialized) {
    // The cache service has been shut down, but someone is still holding
    // a reference to it. Ignore this call.
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (gService->mMemoryDevice) {
    gService->mMemoryDevice->SetMaxEntrySize(maxSize);
  }
}
||||||| merged common ancestors
    if (gService->mMemoryDevice) {
        gService->mMemoryDevice->SetMaxEntrySize(maxSize);
    }
}
=======
  {
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_ONPROFILESHUTDOWN));
    gService->mClearingEntries = true;
    gService->DoomActiveEntries(nullptr);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void nsCacheService::SetOfflineCacheEnabled(bool enabled) {
  if (!gService) return;
  nsCacheServiceAutoLock lock(
      LOCK_TELEM(NSCACHESERVICE_SETOFFLINECACHEENABLED));
  gService->mEnableOfflineDevice = enabled;
}
||||||| merged common ancestors
void
nsCacheService::SetOfflineCacheEnabled(bool    enabled)
{
    if (!gService)  return;
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SETOFFLINECACHEENABLED));
    gService->mEnableOfflineDevice = enabled;
}
=======
  gService->CloseAllStreams();
>>>>>>> upstream-releases

<<<<<<< HEAD
void nsCacheService::SetOfflineCacheCapacity(int32_t capacity) {
  if (!gService) return;
  nsCacheServiceAutoLock lock(
      LOCK_TELEM(NSCACHESERVICE_SETOFFLINECACHECAPACITY));
||||||| merged common ancestors
void
nsCacheService::SetOfflineCacheCapacity(int32_t  capacity)
{
    if (!gService)  return;
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SETOFFLINECACHECAPACITY));
=======
  nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_ONPROFILESHUTDOWN));
  gService->ClearDoomList();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (gService->mOfflineDevice) {
    gService->mOfflineDevice->SetCapacity(capacity);
  }
||||||| merged common ancestors
    if (gService->mOfflineDevice) {
        gService->mOfflineDevice->SetCapacity(capacity);
    }
=======
  // Make sure to wait for any pending cache-operations before
  // proceeding with destructive actions (bug #620660)
  (void)SyncWithCacheIOThread();

  if (gService->mOfflineDevice && gService->mEnableOfflineDevice) {
    gService->mOfflineDevice->Shutdown();
  }
  for (auto iter = gService->mCustomOfflineDevices.Iter(); !iter.Done();
       iter.Next()) {
    iter.Data()->Shutdown();
    iter.Remove();
  }

  gService->mEnableOfflineDevice = false;
>>>>>>> upstream-releases

<<<<<<< HEAD
  gService->mEnableOfflineDevice = gService->mObserver->OfflineCacheEnabled();
||||||| merged common ancestors
    gService->mEnableOfflineDevice = gService->mObserver->OfflineCacheEnabled();
=======
  gService->mClearingEntries = false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsCacheService::SetMemoryCache() {
  if (!gService) return;
||||||| merged common ancestors
=======
void nsCacheService::OnProfileChanged() {
  if (!gService) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  CACHE_LOG_DEBUG(("nsCacheService::SetMemoryCache"));
||||||| merged common ancestors
void
nsCacheService::SetMemoryCache()
{
    if (!gService)  return;

    CACHE_LOG_DEBUG(("nsCacheService::SetMemoryCache"));
=======
  CACHE_LOG_DEBUG(("nsCacheService::OnProfileChanged"));

  nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_ONPROFILECHANGED));
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SETMEMORYCACHE));
||||||| merged common ancestors
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SETMEMORYCACHE));
=======
  gService->mEnableOfflineDevice = gService->mObserver->OfflineCacheEnabled();
>>>>>>> upstream-releases

<<<<<<< HEAD
  gService->mEnableMemoryDevice = gService->mObserver->MemoryCacheEnabled();
||||||| merged common ancestors
    gService->mEnableMemoryDevice = gService->mObserver->MemoryCacheEnabled();
=======
  if (gService->mOfflineDevice) {
    gService->mOfflineDevice->SetCacheParentDirectory(
        gService->mObserver->OfflineCacheParentDirectory());
    gService->mOfflineDevice->SetCapacity(
        gService->mObserver->OfflineCacheCapacity());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (gService->mEnableMemoryDevice) {
    if (gService->mMemoryDevice) {
      int32_t capacity = gService->mObserver->MemoryCacheCapacity();
      // make sure that capacity is reset to the right value
      CACHE_LOG_DEBUG(("Resetting memory device capacity to %d\n", capacity));
      gService->mMemoryDevice->SetCapacity(capacity);
||||||| merged common ancestors
    if (gService->mEnableMemoryDevice) {
        if (gService->mMemoryDevice) {
            int32_t capacity = gService->mObserver->MemoryCacheCapacity();
            // make sure that capacity is reset to the right value
            CACHE_LOG_DEBUG(("Resetting memory device capacity to %d\n",
                             capacity));
            gService->mMemoryDevice->SetCapacity(capacity);
        }
    } else {
        if (gService->mMemoryDevice) {
            // tell memory device to evict everything
            CACHE_LOG_DEBUG(("memory device disabled\n"));
            gService->mMemoryDevice->SetCapacity(0);
            // Don't delete memory device, because some entries may be active still...
        }
=======
    // XXX initialization of mOfflineDevice could be made lazily, if
    // mEnableOfflineDevice is false
    nsresult rv =
        gService->mOfflineDevice->InitWithSqlite(gService->mStorageService);
    if (NS_FAILED(rv)) {
      NS_ERROR(
          "nsCacheService::OnProfileChanged: Re-initializing offline device "
          "failed");
      gService->mEnableOfflineDevice = false;
      // XXX delete mOfflineDevice?
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  } else {
    if (gService->mMemoryDevice) {
      // tell memory device to evict everything
      CACHE_LOG_DEBUG(("memory device disabled\n"));
      gService->mMemoryDevice->SetCapacity(0);
      // Don't delete memory device, because some entries may be active still...
    }
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
||||||| merged common ancestors

=======
void nsCacheService::SetOfflineCacheEnabled(bool enabled) {
  if (!gService) return;
  nsCacheServiceAutoLock lock(
      LOCK_TELEM(NSCACHESERVICE_SETOFFLINECACHEENABLED));
  gService->mEnableOfflineDevice = enabled;
}

void nsCacheService::SetOfflineCacheCapacity(int32_t capacity) {
  if (!gService) return;
  nsCacheServiceAutoLock lock(
      LOCK_TELEM(NSCACHESERVICE_SETOFFLINECACHECAPACITY));

  if (gService->mOfflineDevice) {
    gService->mOfflineDevice->SetCapacity(capacity);
  }

  gService->mEnableOfflineDevice = gService->mObserver->OfflineCacheEnabled();
}

>>>>>>> upstream-releases
/******************************************************************************
 * static methods for nsCacheEntryDescriptor
 *****************************************************************************/
<<<<<<< HEAD
void nsCacheService::CloseDescriptor(nsCacheEntryDescriptor *descriptor) {
  // ask entry to remove descriptor
  nsCacheEntry *entry = descriptor->CacheEntry();
  bool doomEntry;
  bool stillActive = entry->RemoveDescriptor(descriptor, &doomEntry);

  if (!entry->IsValid()) {
    gService->ProcessPendingRequests(entry);
  }
||||||| merged common ancestors
void
nsCacheService::CloseDescriptor(nsCacheEntryDescriptor * descriptor)
{
    // ask entry to remove descriptor
    nsCacheEntry * entry = descriptor->CacheEntry();
    bool doomEntry;
    bool stillActive = entry->RemoveDescriptor(descriptor, &doomEntry);

    if (!entry->IsValid()) {
        gService->ProcessPendingRequests(entry);
    }
=======
void nsCacheService::CloseDescriptor(nsCacheEntryDescriptor* descriptor) {
  // ask entry to remove descriptor
  nsCacheEntry* entry = descriptor->CacheEntry();
  bool doomEntry;
  bool stillActive = entry->RemoveDescriptor(descriptor, &doomEntry);

  if (!entry->IsValid()) {
    gService->ProcessPendingRequests(entry);
  }
>>>>>>> upstream-releases

  if (doomEntry) {
    gService->DoomEntry_Internal(entry, true);
    return;
  }

  if (!stillActive) {
    gService->DeactivateEntry(entry);
  }
}

<<<<<<< HEAD
nsresult nsCacheService::GetFileForEntry(nsCacheEntry *entry,
                                         nsIFile **result) {
  nsCacheDevice *device = gService->EnsureEntryHasDevice(entry);
  if (!device) return NS_ERROR_UNEXPECTED;
||||||| merged common ancestors
=======
nsresult nsCacheService::GetFileForEntry(nsCacheEntry* entry,
                                         nsIFile** result) {
  nsCacheDevice* device = gService->EnsureEntryHasDevice(entry);
  if (!device) return NS_ERROR_UNEXPECTED;
>>>>>>> upstream-releases

  return device->GetFileForEntry(entry, result);
}

<<<<<<< HEAD
nsresult nsCacheService::OpenInputStreamForEntry(nsCacheEntry *entry,
                                                 nsCacheAccessMode mode,
                                                 uint32_t offset,
                                                 nsIInputStream **result) {
  nsCacheDevice *device = gService->EnsureEntryHasDevice(entry);
  if (!device) return NS_ERROR_UNEXPECTED;
||||||| merged common ancestors
=======
nsresult nsCacheService::OpenInputStreamForEntry(nsCacheEntry* entry,
                                                 nsCacheAccessMode mode,
                                                 uint32_t offset,
                                                 nsIInputStream** result) {
  nsCacheDevice* device = gService->EnsureEntryHasDevice(entry);
  if (!device) return NS_ERROR_UNEXPECTED;
>>>>>>> upstream-releases

  return device->OpenInputStreamForEntry(entry, mode, offset, result);
}

<<<<<<< HEAD
nsresult nsCacheService::OpenOutputStreamForEntry(nsCacheEntry *entry,
                                                  nsCacheAccessMode mode,
                                                  uint32_t offset,
                                                  nsIOutputStream **result) {
  nsCacheDevice *device = gService->EnsureEntryHasDevice(entry);
  if (!device) return NS_ERROR_UNEXPECTED;
||||||| merged common ancestors
nsresult
nsCacheService::OpenOutputStreamForEntry(nsCacheEntry *     entry,
                                         nsCacheAccessMode  mode,
                                         uint32_t           offset,
                                         nsIOutputStream ** result)
{
    nsCacheDevice * device = gService->EnsureEntryHasDevice(entry);
    if (!device)  return  NS_ERROR_UNEXPECTED;
=======
nsresult nsCacheService::OpenOutputStreamForEntry(nsCacheEntry* entry,
                                                  nsCacheAccessMode mode,
                                                  uint32_t offset,
                                                  nsIOutputStream** result) {
  nsCacheDevice* device = gService->EnsureEntryHasDevice(entry);
  if (!device) return NS_ERROR_UNEXPECTED;
>>>>>>> upstream-releases

  return device->OpenOutputStreamForEntry(entry, mode, offset, result);
}

<<<<<<< HEAD
nsresult nsCacheService::OnDataSizeChange(nsCacheEntry *entry,
                                          int32_t deltaSize) {
  nsCacheDevice *device = gService->EnsureEntryHasDevice(entry);
  if (!device) return NS_ERROR_UNEXPECTED;
||||||| merged common ancestors
=======
nsresult nsCacheService::OnDataSizeChange(nsCacheEntry* entry,
                                          int32_t deltaSize) {
  nsCacheDevice* device = gService->EnsureEntryHasDevice(entry);
  if (!device) return NS_ERROR_UNEXPECTED;
>>>>>>> upstream-releases

  return device->OnDataSizeChange(entry, deltaSize);
}

void nsCacheService::LockAcquired() {
  MutexAutoLock lock(mTimeStampLock);
  mLockAcquiredTimeStamp = TimeStamp::Now();
}

void nsCacheService::LockReleased() {
  MutexAutoLock lock(mTimeStampLock);
  mLockAcquiredTimeStamp = TimeStamp();
}

void nsCacheService::Lock() {
  gService->mLock.Lock();
  gService->LockAcquired();
}

void nsCacheService::Lock(mozilla::Telemetry::HistogramID mainThreadLockerID) {
  mozilla::Telemetry::HistogramID lockerID;
  mozilla::Telemetry::HistogramID generalID;

<<<<<<< HEAD
  if (NS_IsMainThread()) {
    lockerID = mainThreadLockerID;
    generalID = mozilla::Telemetry::CACHE_SERVICE_LOCK_WAIT_MAINTHREAD_2;
  } else {
    lockerID = mozilla::Telemetry::HistogramCount;
    generalID = mozilla::Telemetry::CACHE_SERVICE_LOCK_WAIT_2;
  }

  TimeStamp start(TimeStamp::Now());

  nsCacheService::Lock();

  TimeStamp stop(TimeStamp::Now());

  // Telemetry isn't thread safe on its own, but this is OK because we're
  // protecting it with the cache lock.
  if (lockerID != mozilla::Telemetry::HistogramCount) {
    mozilla::Telemetry::AccumulateTimeDelta(lockerID, start, stop);
  }
  mozilla::Telemetry::AccumulateTimeDelta(generalID, start, stop);
}
||||||| merged common ancestors
    if (NS_IsMainThread()) {
        lockerID = mainThreadLockerID;
        generalID = mozilla::Telemetry::CACHE_SERVICE_LOCK_WAIT_MAINTHREAD_2;
    } else {
        lockerID = mozilla::Telemetry::HistogramCount;
        generalID = mozilla::Telemetry::CACHE_SERVICE_LOCK_WAIT_2;
    }

    TimeStamp start(TimeStamp::Now());

    nsCacheService::Lock();

    TimeStamp stop(TimeStamp::Now());

    // Telemetry isn't thread safe on its own, but this is OK because we're
    // protecting it with the cache lock.
    if (lockerID != mozilla::Telemetry::HistogramCount) {
        mozilla::Telemetry::AccumulateTimeDelta(lockerID, start, stop);
    }
    mozilla::Telemetry::AccumulateTimeDelta(generalID, start, stop);
}
=======
  if (NS_IsMainThread()) {
    lockerID = mainThreadLockerID;
    generalID = mozilla::Telemetry::CACHE_SERVICE_LOCK_WAIT_MAINTHREAD_2;
  } else {
    lockerID = mozilla::Telemetry::HistogramCount;
    generalID = mozilla::Telemetry::CACHE_SERVICE_LOCK_WAIT_2;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void nsCacheService::Unlock() {
  gService->mLock.AssertCurrentThreadOwns();
||||||| merged common ancestors
void
nsCacheService::Unlock()
{
    gService->mLock.AssertCurrentThreadOwns();
=======
  TimeStamp start(TimeStamp::Now());
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsTArray<nsISupports *> doomed;
  doomed.SwapElements(gService->mDoomedObjects);
||||||| merged common ancestors
    nsTArray<nsISupports*> doomed;
    doomed.SwapElements(gService->mDoomedObjects);
=======
  nsCacheService::Lock();
>>>>>>> upstream-releases

<<<<<<< HEAD
  gService->LockReleased();
  gService->mLock.Unlock();
||||||| merged common ancestors
    gService->LockReleased();
    gService->mLock.Unlock();
=======
  TimeStamp stop(TimeStamp::Now());
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (uint32_t i = 0; i < doomed.Length(); ++i) doomed[i]->Release();
||||||| merged common ancestors
    for (uint32_t i = 0; i < doomed.Length(); ++i)
        doomed[i]->Release();
=======
  // Telemetry isn't thread safe on its own, but this is OK because we're
  // protecting it with the cache lock.
  if (lockerID != mozilla::Telemetry::HistogramCount) {
    mozilla::Telemetry::AccumulateTimeDelta(lockerID, start, stop);
  }
  mozilla::Telemetry::AccumulateTimeDelta(generalID, start, stop);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsCacheService::ReleaseObject_Locked(nsISupports *obj,
                                          nsIEventTarget *target) {
  gService->mLock.AssertCurrentThreadOwns();
||||||| merged common ancestors
void
nsCacheService::ReleaseObject_Locked(nsISupports * obj,
                                     nsIEventTarget * target)
{
    gService->mLock.AssertCurrentThreadOwns();
=======
void nsCacheService::Unlock() {
  gService->mLock.AssertCurrentThreadOwns();
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isCur;
  if (!target || (NS_SUCCEEDED(target->IsOnCurrentThread(&isCur)) && isCur)) {
    gService->mDoomedObjects.AppendElement(obj);
  } else {
    NS_ProxyRelease("nsCacheService::ReleaseObject_Locked::obj", target,
                    dont_AddRef(obj));
  }
}
||||||| merged common ancestors
    bool isCur;
    if (!target || (NS_SUCCEEDED(target->IsOnCurrentThread(&isCur)) && isCur)) {
        gService->mDoomedObjects.AppendElement(obj);
    } else {
        NS_ProxyRelease(
          "nsCacheService::ReleaseObject_Locked::obj", target, dont_AddRef(obj));
    }
}
=======
  nsTArray<nsISupports*> doomed;
  doomed.SwapElements(gService->mDoomedObjects);
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsCacheService::SetCacheElement(nsCacheEntry *entry,
                                         nsISupports *element) {
  entry->SetData(element);
  entry->TouchData();
  return NS_OK;
}

nsresult nsCacheService::ValidateEntry(nsCacheEntry *entry) {
  nsCacheDevice *device = gService->EnsureEntryHasDevice(entry);
  if (!device) return NS_ERROR_UNEXPECTED;

  entry->MarkValid();
  nsresult rv = gService->ProcessPendingRequests(entry);
  NS_ASSERTION(rv == NS_OK, "ProcessPendingRequests failed.");
  // XXX what else should be done?

  return rv;
}

int32_t nsCacheService::CacheCompressionLevel() {
  int32_t level = gService->mObserver->CacheCompressionLevel();
  return level;
}

void nsCacheService::DeactivateEntry(nsCacheEntry *entry) {
  CACHE_LOG_DEBUG(("Deactivating entry %p\n", entry));
  nsresult rv = NS_OK;
  NS_ASSERTION(entry->IsNotInUse(), "### deactivating an entry while in use!");
  nsCacheDevice *device = nullptr;

  if (mMaxDataSize < entry->DataSize()) mMaxDataSize = entry->DataSize();
  if (mMaxMetaSize < entry->MetaDataSize())
    mMaxMetaSize = entry->MetaDataSize();

  if (entry->IsDoomed()) {
    // remove from Doomed list
    PR_REMOVE_AND_INIT_LINK(entry);
  } else if (entry->IsActive()) {
    // remove from active entries
    mActiveEntries.RemoveEntry(entry);
    CACHE_LOG_DEBUG(
        ("Removed deactivated entry %p from mActiveEntries\n", entry));
    entry->MarkInactive();

    // bind entry if necessary to store meta-data
    device = EnsureEntryHasDevice(entry);
    if (!device) {
      CACHE_LOG_DEBUG(
          ("DeactivateEntry: unable to bind active "
           "entry %p\n",
           entry));
      NS_WARNING("DeactivateEntry: unable to bind active entry\n");
      return;
    }
  } else {
    // if mInitialized == false,
    // then we're shutting down and this state is okay.
    NS_ASSERTION(!mInitialized, "DeactivateEntry: bad cache entry state.");
  }
||||||| merged common ancestors
=======
  gService->LockReleased();
  gService->mLock.Unlock();
>>>>>>> upstream-releases

<<<<<<< HEAD
  device = entry->CacheDevice();
  if (device) {
    rv = device->DeactivateEntry(entry);
    if (NS_FAILED(rv)) {
      // increment deactivate failure count
      ++mDeactivateFailures;
    }
  } else {
    // increment deactivating unbound entry statistic
    ++mDeactivatedUnboundEntries;
    delete entry;  // because no one else will
  }
||||||| merged common ancestors
nsresult
nsCacheService::SetCacheElement(nsCacheEntry * entry, nsISupports * element)
{
    entry->SetData(element);
    entry->TouchData();
    return NS_OK;
=======
  for (uint32_t i = 0; i < doomed.Length(); ++i) doomed[i]->Release();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsCacheService::ProcessPendingRequests(nsCacheEntry *entry) {
  nsresult rv = NS_OK;
  nsCacheRequest *request = (nsCacheRequest *)PR_LIST_HEAD(&entry->mRequestQ);
  nsCacheRequest *nextRequest;
  bool newWriter = false;
||||||| merged common ancestors
=======
void nsCacheService::ReleaseObject_Locked(nsISupports* obj,
                                          nsIEventTarget* target) {
  gService->mLock.AssertCurrentThreadOwns();
>>>>>>> upstream-releases

<<<<<<< HEAD
  CACHE_LOG_DEBUG((
      "ProcessPendingRequests for %sinitialized %s %salid entry %p\n",
      (entry->IsInitialized() ? "" : "Un"), (entry->IsDoomed() ? "DOOMED" : ""),
      (entry->IsValid() ? "V" : "Inv"), entry));

  if (request == &entry->mRequestQ) return NS_OK;  // no queued requests
||||||| merged common ancestors
nsresult
nsCacheService::ValidateEntry(nsCacheEntry * entry)
{
    nsCacheDevice * device = gService->EnsureEntryHasDevice(entry);
    if (!device)  return  NS_ERROR_UNEXPECTED;

    entry->MarkValid();
    nsresult rv = gService->ProcessPendingRequests(entry);
    NS_ASSERTION(rv == NS_OK, "ProcessPendingRequests failed.");
    // XXX what else should be done?

    return rv;
}
=======
  bool isCur;
  if (!target || (NS_SUCCEEDED(target->IsOnCurrentThread(&isCur)) && isCur)) {
    gService->mDoomedObjects.AppendElement(obj);
  } else {
    NS_ProxyRelease("nsCacheService::ReleaseObject_Locked::obj", target,
                    dont_AddRef(obj));
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!entry->IsDoomed() && entry->IsInvalid()) {
    // 1st descriptor closed w/o MarkValid()
    NS_ASSERTION(PR_CLIST_IS_EMPTY(&entry->mDescriptorQ),
                 "shouldn't be here with open descriptors");
||||||| merged common ancestors
=======
nsresult nsCacheService::SetCacheElement(nsCacheEntry* entry,
                                         nsISupports* element) {
  entry->SetData(element);
  entry->TouchData();
  return NS_OK;
}

nsresult nsCacheService::ValidateEntry(nsCacheEntry* entry) {
  nsCacheDevice* device = gService->EnsureEntryHasDevice(entry);
  if (!device) return NS_ERROR_UNEXPECTED;

  entry->MarkValid();
  nsresult rv = gService->ProcessPendingRequests(entry);
  NS_ASSERTION(rv == NS_OK, "ProcessPendingRequests failed.");
  // XXX what else should be done?

  return rv;
}

int32_t nsCacheService::CacheCompressionLevel() {
  int32_t level = gService->mObserver->CacheCompressionLevel();
  return level;
}

void nsCacheService::DeactivateEntry(nsCacheEntry* entry) {
  CACHE_LOG_DEBUG(("Deactivating entry %p\n", entry));
  nsresult rv = NS_OK;
  NS_ASSERTION(entry->IsNotInUse(), "### deactivating an entry while in use!");
  nsCacheDevice* device = nullptr;

  if (mMaxDataSize < entry->DataSize()) mMaxDataSize = entry->DataSize();
  if (mMaxMetaSize < entry->MetaDataSize())
    mMaxMetaSize = entry->MetaDataSize();

  if (entry->IsDoomed()) {
    // remove from Doomed list
    PR_REMOVE_AND_INIT_LINK(entry);
  } else if (entry->IsActive()) {
    // remove from active entries
    mActiveEntries.RemoveEntry(entry);
    CACHE_LOG_DEBUG(
        ("Removed deactivated entry %p from mActiveEntries\n", entry));
    entry->MarkInactive();

    // bind entry if necessary to store meta-data
    device = EnsureEntryHasDevice(entry);
    if (!device) {
      CACHE_LOG_DEBUG(
          ("DeactivateEntry: unable to bind active "
           "entry %p\n",
           entry));
      NS_WARNING("DeactivateEntry: unable to bind active entry\n");
      return;
    }
  } else {
    // if mInitialized == false,
    // then we're shutting down and this state is okay.
    NS_ASSERTION(!mInitialized, "DeactivateEntry: bad cache entry state.");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
#if DEBUG
    // verify no ACCESS_WRITE requests(shouldn't have any of these)
    while (request != &entry->mRequestQ) {
      NS_ASSERTION(request->AccessRequested() != nsICache::ACCESS_WRITE,
                   "ACCESS_WRITE request should have been given a new entry");
      request = (nsCacheRequest *)PR_NEXT_LINK(request);
||||||| merged common ancestors
int32_t
nsCacheService::CacheCompressionLevel()
{
    int32_t level = gService->mObserver->CacheCompressionLevel();
    return level;
}


void
nsCacheService::DeactivateEntry(nsCacheEntry * entry)
{
    CACHE_LOG_DEBUG(("Deactivating entry %p\n", entry));
    nsresult  rv = NS_OK;
    NS_ASSERTION(entry->IsNotInUse(), "### deactivating an entry while in use!");
    nsCacheDevice * device = nullptr;

    if (mMaxDataSize < entry->DataSize() )     mMaxDataSize = entry->DataSize();
    if (mMaxMetaSize < entry->MetaDataSize() ) mMaxMetaSize = entry->MetaDataSize();

    if (entry->IsDoomed()) {
        // remove from Doomed list
        PR_REMOVE_AND_INIT_LINK(entry);
    } else if (entry->IsActive()) {
        // remove from active entries
        mActiveEntries.RemoveEntry(entry);
        CACHE_LOG_DEBUG(("Removed deactivated entry %p from mActiveEntries\n",
                         entry));
        entry->MarkInactive();

        // bind entry if necessary to store meta-data
        device = EnsureEntryHasDevice(entry);
        if (!device) {
            CACHE_LOG_DEBUG(("DeactivateEntry: unable to bind active "
                             "entry %p\n",
                             entry));
            NS_WARNING("DeactivateEntry: unable to bind active entry\n");
            return;
        }
    } else {
        // if mInitialized == false,
        // then we're shutting down and this state is okay.
        NS_ASSERTION(!mInitialized, "DeactivateEntry: bad cache entry state.");
=======
  device = entry->CacheDevice();
  if (device) {
    rv = device->DeactivateEntry(entry);
    if (NS_FAILED(rv)) {
      // increment deactivate failure count
      ++mDeactivateFailures;
    }
  } else {
    // increment deactivating unbound entry statistic
    ++mDeactivatedUnboundEntries;
    delete entry;  // because no one else will
  }
}

nsresult nsCacheService::ProcessPendingRequests(nsCacheEntry* entry) {
  nsresult rv = NS_OK;
  nsCacheRequest* request = (nsCacheRequest*)PR_LIST_HEAD(&entry->mRequestQ);
  nsCacheRequest* nextRequest;
  bool newWriter = false;

  CACHE_LOG_DEBUG((
      "ProcessPendingRequests for %sinitialized %s %salid entry %p\n",
      (entry->IsInitialized() ? "" : "Un"), (entry->IsDoomed() ? "DOOMED" : ""),
      (entry->IsValid() ? "V" : "Inv"), entry));

  if (request == &entry->mRequestQ) return NS_OK;  // no queued requests

  if (!entry->IsDoomed() && entry->IsInvalid()) {
    // 1st descriptor closed w/o MarkValid()
    NS_ASSERTION(PR_CLIST_IS_EMPTY(&entry->mDescriptorQ),
                 "shouldn't be here with open descriptors");

#if DEBUG
    // verify no ACCESS_WRITE requests(shouldn't have any of these)
    while (request != &entry->mRequestQ) {
      NS_ASSERTION(request->AccessRequested() != nsICache::ACCESS_WRITE,
                   "ACCESS_WRITE request should have been given a new entry");
      request = (nsCacheRequest*)PR_NEXT_LINK(request);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    request = (nsCacheRequest *)PR_LIST_HEAD(&entry->mRequestQ);
#endif
    // find first request with ACCESS_READ_WRITE (if any) and promote it to 1st
    // writer
    while (request != &entry->mRequestQ) {
      if (request->AccessRequested() == nsICache::ACCESS_READ_WRITE) {
        newWriter = true;
        CACHE_LOG_DEBUG(("  promoting request %p to 1st writer\n", request));
        break;
      }
||||||| merged common ancestors
=======
    request = (nsCacheRequest*)PR_LIST_HEAD(&entry->mRequestQ);
#endif
    // find first request with ACCESS_READ_WRITE (if any) and promote it to 1st
    // writer
    while (request != &entry->mRequestQ) {
      if (request->AccessRequested() == nsICache::ACCESS_READ_WRITE) {
        newWriter = true;
        CACHE_LOG_DEBUG(("  promoting request %p to 1st writer\n", request));
        break;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      request = (nsCacheRequest *)PR_NEXT_LINK(request);
||||||| merged common ancestors
    device = entry->CacheDevice();
    if (device) {
        rv = device->DeactivateEntry(entry);
        if (NS_FAILED(rv)) {
            // increment deactivate failure count
            ++mDeactivateFailures;
        }
    } else {
        // increment deactivating unbound entry statistic
        ++mDeactivatedUnboundEntries;
        delete entry; // because no one else will
=======
      request = (nsCacheRequest*)PR_NEXT_LINK(request);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (request == &entry->mRequestQ)  // no requests asked for
                                       // ACCESS_READ_WRITE, back to top
      request = (nsCacheRequest *)PR_LIST_HEAD(&entry->mRequestQ);
||||||| merged common ancestors
    if (request == &entry->mRequestQ)  return NS_OK;    // no queued requests
=======
    if (request == &entry->mRequestQ)  // no requests asked for
                                       // ACCESS_READ_WRITE, back to top
      request = (nsCacheRequest*)PR_LIST_HEAD(&entry->mRequestQ);
>>>>>>> upstream-releases

    // XXX what should we do if there are only READ requests in queue?
    // XXX serialize their accesses, give them only read access, but force them
    // to check validate flag?
    // XXX or do readers simply presume the entry is valid
    // See fix for bug #467392 below
  }

  nsCacheAccessMode accessGranted = nsICache::ACCESS_NONE;

<<<<<<< HEAD
  while (request != &entry->mRequestQ) {
    nextRequest = (nsCacheRequest *)PR_NEXT_LINK(request);
    CACHE_LOG_DEBUG(("  %sync request %p for %p\n",
                     (request->mListener ? "As" : "S"), request, entry));
||||||| merged common ancestors
        if (request == &entry->mRequestQ)   // no requests asked for ACCESS_READ_WRITE, back to top
            request = (nsCacheRequest *)PR_LIST_HEAD(&entry->mRequestQ);
=======
  while (request != &entry->mRequestQ) {
    nextRequest = (nsCacheRequest*)PR_NEXT_LINK(request);
    CACHE_LOG_DEBUG(("  %sync request %p for %p\n",
                     (request->mListener ? "As" : "S"), request, entry));
>>>>>>> upstream-releases

    if (request->mListener) {
      // Async request
      PR_REMOVE_AND_INIT_LINK(request);

      if (entry->IsDoomed()) {
        rv = ProcessRequest(request, false, nullptr);
        if (rv == NS_ERROR_CACHE_WAIT_FOR_VALIDATION)
          rv = NS_OK;
        else
          delete request;

<<<<<<< HEAD
        if (NS_FAILED(rv)) {
          // XXX what to do?
        }
      } else if (entry->IsValid() || newWriter) {
        rv = entry->RequestAccess(request, &accessGranted);
        NS_ASSERTION(NS_SUCCEEDED(rv),
                     "if entry is valid, RequestAccess must succeed.");
        // XXX if (newWriter) {
        //       NS_ASSERTION( accessGranted ==
        //                     request->AccessRequested(), "why not?");
        //     }

        // entry->CreateDescriptor dequeues request, and queues descriptor
        nsICacheEntryDescriptor *descriptor = nullptr;
        rv = entry->CreateDescriptor(request, accessGranted, &descriptor);
||||||| merged common ancestors
    while (request != &entry->mRequestQ) {
        nextRequest = (nsCacheRequest *)PR_NEXT_LINK(request);
        CACHE_LOG_DEBUG(("  %sync request %p for %p\n",
                        (request->mListener?"As":"S"), request, entry));
=======
        if (NS_FAILED(rv)) {
          // XXX what to do?
        }
      } else if (entry->IsValid() || newWriter) {
        rv = entry->RequestAccess(request, &accessGranted);
        NS_ASSERTION(NS_SUCCEEDED(rv),
                     "if entry is valid, RequestAccess must succeed.");
        // XXX if (newWriter) {
        //       NS_ASSERTION( accessGranted ==
        //                     request->AccessRequested(), "why not?");
        //     }

        // entry->CreateDescriptor dequeues request, and queues descriptor
        nsICacheEntryDescriptor* descriptor = nullptr;
        rv = entry->CreateDescriptor(request, accessGranted, &descriptor);
>>>>>>> upstream-releases

        // post call to listener to report error or descriptor
        rv = NotifyListener(request, descriptor, accessGranted, rv);
        delete request;
        if (NS_FAILED(rv)) {
          // XXX what to do?
        }

      } else {
        // read-only request to an invalid entry - need to wait for
        // the entry to become valid so we post an event to process
        // the request again later (bug #467392)
        nsCOMPtr<nsIRunnable> ev = new nsProcessRequestEvent(request);
        rv = DispatchToCacheIOThread(ev);
        if (NS_FAILED(rv)) {
          delete request;  // avoid leak
        }
      }
    } else {
      // Synchronous request
      request->WakeUp();
    }
    if (newWriter) break;  // process remaining requests after validation
    request = nextRequest;
  }

  return NS_OK;
}

<<<<<<< HEAD
bool nsCacheService::IsDoomListEmpty() {
  nsCacheEntry *entry = (nsCacheEntry *)PR_LIST_HEAD(&mDoomedEntries);
  return &mDoomedEntries == entry;
||||||| merged common ancestors
bool
nsCacheService::IsDoomListEmpty()
{
    nsCacheEntry * entry = (nsCacheEntry *)PR_LIST_HEAD(&mDoomedEntries);
    return &mDoomedEntries == entry;
=======
bool nsCacheService::IsDoomListEmpty() {
  nsCacheEntry* entry = (nsCacheEntry*)PR_LIST_HEAD(&mDoomedEntries);
  return &mDoomedEntries == entry;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsCacheService::ClearDoomList() {
  nsCacheEntry *entry = (nsCacheEntry *)PR_LIST_HEAD(&mDoomedEntries);
||||||| merged common ancestors
void
nsCacheService::ClearDoomList()
{
    nsCacheEntry * entry = (nsCacheEntry *)PR_LIST_HEAD(&mDoomedEntries);
=======
void nsCacheService::ClearDoomList() {
  nsCacheEntry* entry = (nsCacheEntry*)PR_LIST_HEAD(&mDoomedEntries);
>>>>>>> upstream-releases

<<<<<<< HEAD
  while (entry != &mDoomedEntries) {
    nsCacheEntry *next = (nsCacheEntry *)PR_NEXT_LINK(entry);
||||||| merged common ancestors
    while (entry != &mDoomedEntries) {
        nsCacheEntry * next = (nsCacheEntry *)PR_NEXT_LINK(entry);
=======
  while (entry != &mDoomedEntries) {
    nsCacheEntry* next = (nsCacheEntry*)PR_NEXT_LINK(entry);
>>>>>>> upstream-releases

    entry->DetachDescriptors();
    DeactivateEntry(entry);
    entry = next;
  }
}

<<<<<<< HEAD
void nsCacheService::DoomActiveEntries(DoomCheckFn check) {
  AutoTArray<nsCacheEntry *, 8> array;
||||||| merged common ancestors
void
nsCacheService::DoomActiveEntries(DoomCheckFn check)
{
    AutoTArray<nsCacheEntry*, 8> array;
=======
void nsCacheService::DoomActiveEntries(DoomCheckFn check) {
  AutoTArray<nsCacheEntry*, 8> array;
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (auto iter = mActiveEntries.Iter(); !iter.Done(); iter.Next()) {
    nsCacheEntry *entry =
        static_cast<nsCacheEntryHashTableEntry *>(iter.Get())->cacheEntry;
||||||| merged common ancestors
    for (auto iter = mActiveEntries.Iter(); !iter.Done(); iter.Next()) {
        nsCacheEntry* entry =
            static_cast<nsCacheEntryHashTableEntry*>(iter.Get())->cacheEntry;
=======
  for (auto iter = mActiveEntries.Iter(); !iter.Done(); iter.Next()) {
    nsCacheEntry* entry =
        static_cast<nsCacheEntryHashTableEntry*>(iter.Get())->cacheEntry;
>>>>>>> upstream-releases

    if (check && !check(entry)) {
      continue;
    }

    array.AppendElement(entry);

    // entry is being removed from the active entry list
    entry->MarkInactive();
    iter.Remove();
  }

  uint32_t count = array.Length();
  for (uint32_t i = 0; i < count; ++i) {
    DoomEntry_Internal(array[i], true);
  }
}

void nsCacheService::CloseAllStreams() {
  nsTArray<RefPtr<nsCacheEntryDescriptor::nsInputStreamWrapper> > inputs;
  nsTArray<RefPtr<nsCacheEntryDescriptor::nsOutputStreamWrapper> > outputs;

  {
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_CLOSEALLSTREAMS));

<<<<<<< HEAD
    nsTArray<nsCacheEntry *> entries;
||||||| merged common ancestors
        nsTArray<nsCacheEntry*> entries;
=======
    nsTArray<nsCacheEntry*> entries;
>>>>>>> upstream-releases

#if DEBUG
<<<<<<< HEAD
    // make sure there is no active entry
    for (auto iter = mActiveEntries.Iter(); !iter.Done(); iter.Next()) {
      auto entry = static_cast<nsCacheEntryHashTableEntry *>(iter.Get());
      entries.AppendElement(entry->cacheEntry);
    }
    NS_ASSERTION(entries.IsEmpty(), "Bad state");
||||||| merged common ancestors
        // make sure there is no active entry
        for (auto iter = mActiveEntries.Iter(); !iter.Done(); iter.Next()) {
            auto entry = static_cast<nsCacheEntryHashTableEntry*>(iter.Get());
            entries.AppendElement(entry->cacheEntry);
        }
        NS_ASSERTION(entries.IsEmpty(), "Bad state");
=======
    // make sure there is no active entry
    for (auto iter = mActiveEntries.Iter(); !iter.Done(); iter.Next()) {
      auto entry = static_cast<nsCacheEntryHashTableEntry*>(iter.Get());
      entries.AppendElement(entry->cacheEntry);
    }
    NS_ASSERTION(entries.IsEmpty(), "Bad state");
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
    // Get doomed entries
    nsCacheEntry *entry = (nsCacheEntry *)PR_LIST_HEAD(&mDoomedEntries);
    while (entry != &mDoomedEntries) {
      nsCacheEntry *next = (nsCacheEntry *)PR_NEXT_LINK(entry);
      entries.AppendElement(entry);
      entry = next;
    }
||||||| merged common ancestors
        // Get doomed entries
        nsCacheEntry * entry = (nsCacheEntry *)PR_LIST_HEAD(&mDoomedEntries);
        while (entry != &mDoomedEntries) {
            nsCacheEntry * next = (nsCacheEntry *)PR_NEXT_LINK(entry);
            entries.AppendElement(entry);
            entry = next;
        }
=======
    // Get doomed entries
    nsCacheEntry* entry = (nsCacheEntry*)PR_LIST_HEAD(&mDoomedEntries);
    while (entry != &mDoomedEntries) {
      nsCacheEntry* next = (nsCacheEntry*)PR_NEXT_LINK(entry);
      entries.AppendElement(entry);
      entry = next;
    }
>>>>>>> upstream-releases

    // Iterate through all entries and collect input and output streams
    for (size_t i = 0; i < entries.Length(); i++) {
      entry = entries.ElementAt(i);

      nsTArray<RefPtr<nsCacheEntryDescriptor> > descs;
      entry->GetDescriptors(descs);

      for (uint32_t j = 0; j < descs.Length(); j++) {
        if (descs[j]->mOutputWrapper)
          outputs.AppendElement(descs[j]->mOutputWrapper);

        for (size_t k = 0; k < descs[j]->mInputWrappers.Length(); k++)
          inputs.AppendElement(descs[j]->mInputWrappers[k]);
      }
    }
  }

  uint32_t i;
  for (i = 0; i < inputs.Length(); i++) inputs[i]->Close();

  for (i = 0; i < outputs.Length(); i++) outputs[i]->Close();
}

bool nsCacheService::GetClearingEntries() {
  AssertOwnsLock();
  return gService->mClearingEntries;
}

// static
<<<<<<< HEAD
void nsCacheService::GetCacheBaseDirectoty(nsIFile **result) {
  *result = nullptr;
  if (!gService || !gService->mObserver) return;
||||||| merged common ancestors
void nsCacheService::GetCacheBaseDirectoty(nsIFile ** result)
{
    *result = nullptr;
    if (!gService || !gService->mObserver)
        return;
=======
void nsCacheService::GetCacheBaseDirectoty(nsIFile** result) {
  *result = nullptr;
  if (!gService || !gService->mObserver) return;
>>>>>>> upstream-releases

  nsCOMPtr<nsIFile> directory = gService->mObserver->DiskCacheParentDirectory();
  if (!directory) return;

  directory->Clone(result);
}

// static
<<<<<<< HEAD
void nsCacheService::GetDiskCacheDirectory(nsIFile **result) {
  nsCOMPtr<nsIFile> directory;
  GetCacheBaseDirectoty(getter_AddRefs(directory));
  if (!directory) return;
||||||| merged common ancestors
void nsCacheService::GetDiskCacheDirectory(nsIFile ** result)
{
    nsCOMPtr<nsIFile> directory;
    GetCacheBaseDirectoty(getter_AddRefs(directory));
    if (!directory)
        return;
=======
void nsCacheService::GetDiskCacheDirectory(nsIFile** result) {
  nsCOMPtr<nsIFile> directory;
  GetCacheBaseDirectoty(getter_AddRefs(directory));
  if (!directory) return;
>>>>>>> upstream-releases

  nsresult rv = directory->AppendNative(NS_LITERAL_CSTRING("Cache"));
  if (NS_FAILED(rv)) return;

  directory.forget(result);
}

// static
<<<<<<< HEAD
void nsCacheService::GetAppCacheDirectory(nsIFile **result) {
  nsCOMPtr<nsIFile> directory;
  GetCacheBaseDirectoty(getter_AddRefs(directory));
  if (!directory) return;

  nsresult rv = directory->AppendNative(NS_LITERAL_CSTRING("OfflineCache"));
  if (NS_FAILED(rv)) return;

  directory.forget(result);
}
||||||| merged common ancestors
void nsCacheService::GetAppCacheDirectory(nsIFile ** result)
{
    nsCOMPtr<nsIFile> directory;
    GetCacheBaseDirectoty(getter_AddRefs(directory));
    if (!directory)
        return;

    nsresult rv = directory->AppendNative(NS_LITERAL_CSTRING("OfflineCache"));
    if (NS_FAILED(rv))
        return;

    directory.forget(result);
}
=======
void nsCacheService::GetAppCacheDirectory(nsIFile** result) {
  nsCOMPtr<nsIFile> directory;
  GetCacheBaseDirectoty(getter_AddRefs(directory));
  if (!directory) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
void nsCacheService::LogCacheStatistics() {
  uint32_t hitPercentage = 0;
  double sum = (double)(mCacheHits + mCacheMisses);
  if (sum != 0) {
    hitPercentage = (uint32_t)((((double)mCacheHits) / sum) * 100);
  }
  CACHE_LOG_INFO(("\nCache Service Statistics:\n\n"));
  CACHE_LOG_INFO(("    TotalEntries   = %d\n", mTotalEntries));
  CACHE_LOG_INFO(("    Cache Hits     = %d\n", mCacheHits));
  CACHE_LOG_INFO(("    Cache Misses   = %d\n", mCacheMisses));
  CACHE_LOG_INFO(("    Cache Hit %%    = %d%%\n", hitPercentage));
  CACHE_LOG_INFO(("    Max Key Length = %d\n", mMaxKeyLength));
  CACHE_LOG_INFO(("    Max Meta Size  = %d\n", mMaxMetaSize));
  CACHE_LOG_INFO(("    Max Data Size  = %d\n", mMaxDataSize));
  CACHE_LOG_INFO(("\n"));
  CACHE_LOG_INFO(
      ("    Deactivate Failures         = %d\n", mDeactivateFailures));
  CACHE_LOG_INFO(
      ("    Deactivated Unbound Entries = %d\n", mDeactivatedUnboundEntries));
}
||||||| merged common ancestors
=======
  nsresult rv = directory->AppendNative(NS_LITERAL_CSTRING("OfflineCache"));
  if (NS_FAILED(rv)) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsCacheService::SetDiskSmartSize() {
  nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SETDISKSMARTSIZE));

  if (!gService) return NS_ERROR_NOT_AVAILABLE;

  return gService->SetDiskSmartSize_Locked();
||||||| merged common ancestors
void
nsCacheService::LogCacheStatistics()
{
    uint32_t hitPercentage = 0;
    double sum = (double)(mCacheHits + mCacheMisses);
    if (sum != 0) {
        hitPercentage = (uint32_t)((((double)mCacheHits) / sum) * 100);
    }
    CACHE_LOG_INFO(("\nCache Service Statistics:\n\n"));
    CACHE_LOG_INFO(("    TotalEntries   = %d\n", mTotalEntries));
    CACHE_LOG_INFO(("    Cache Hits     = %d\n", mCacheHits));
    CACHE_LOG_INFO(("    Cache Misses   = %d\n", mCacheMisses));
    CACHE_LOG_INFO(("    Cache Hit %%    = %d%%\n", hitPercentage));
    CACHE_LOG_INFO(("    Max Key Length = %d\n", mMaxKeyLength));
    CACHE_LOG_INFO(("    Max Meta Size  = %d\n", mMaxMetaSize));
    CACHE_LOG_INFO(("    Max Data Size  = %d\n", mMaxDataSize));
    CACHE_LOG_INFO(("\n"));
    CACHE_LOG_INFO(("    Deactivate Failures         = %d\n",
                      mDeactivateFailures));
    CACHE_LOG_INFO(("    Deactivated Unbound Entries = %d\n",
                      mDeactivatedUnboundEntries));
}

nsresult
nsCacheService::SetDiskSmartSize()
{
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_SETDISKSMARTSIZE));

    if (!gService) return NS_ERROR_NOT_AVAILABLE;

    return gService->SetDiskSmartSize_Locked();
}

nsresult
nsCacheService::SetDiskSmartSize_Locked()
{
    return NS_ERROR_NOT_AVAILABLE;
=======
  directory.forget(result);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsCacheService::SetDiskSmartSize_Locked() {
  return NS_ERROR_NOT_AVAILABLE;
}

void nsCacheService::MoveOrRemoveDiskCache(nsIFile *aOldCacheDir,
                                           nsIFile *aNewCacheDir,
                                           const char *aCacheSubdir) {
  bool same;
  if (NS_FAILED(aOldCacheDir->Equals(aNewCacheDir, &same)) || same) return;

  nsCOMPtr<nsIFile> aOldCacheSubdir;
  aOldCacheDir->Clone(getter_AddRefs(aOldCacheSubdir));

  nsresult rv = aOldCacheSubdir->AppendNative(nsDependentCString(aCacheSubdir));
  if (NS_FAILED(rv)) return;

  bool exists;
  if (NS_FAILED(aOldCacheSubdir->Exists(&exists)) || !exists) return;

  nsCOMPtr<nsIFile> aNewCacheSubdir;
  aNewCacheDir->Clone(getter_AddRefs(aNewCacheSubdir));

  rv = aNewCacheSubdir->AppendNative(nsDependentCString(aCacheSubdir));
  if (NS_FAILED(rv)) return;

  PathString newPath = aNewCacheSubdir->NativePath();

  if (NS_SUCCEEDED(aNewCacheSubdir->Exists(&exists)) && !exists) {
    // New cache directory does not exist, try to move the old one here
    // rename needs an empty target directory

    // Make sure the parent of the target sub-dir exists
    rv = aNewCacheDir->Create(nsIFile::DIRECTORY_TYPE, 0777);
    if (NS_SUCCEEDED(rv) || NS_ERROR_FILE_ALREADY_EXISTS == rv) {
      PathString oldPath = aOldCacheSubdir->NativePath();
||||||| merged common ancestors
void
nsCacheService::MoveOrRemoveDiskCache(nsIFile *aOldCacheDir,
                                      nsIFile *aNewCacheDir,
                                      const char *aCacheSubdir)
{
    bool same;
    if (NS_FAILED(aOldCacheDir->Equals(aNewCacheDir, &same)) || same)
        return;

    nsCOMPtr<nsIFile> aOldCacheSubdir;
    aOldCacheDir->Clone(getter_AddRefs(aOldCacheSubdir));

    nsresult rv = aOldCacheSubdir->AppendNative(
        nsDependentCString(aCacheSubdir));
    if (NS_FAILED(rv))
        return;

    bool exists;
    if (NS_FAILED(aOldCacheSubdir->Exists(&exists)) || !exists)
        return;

    nsCOMPtr<nsIFile> aNewCacheSubdir;
    aNewCacheDir->Clone(getter_AddRefs(aNewCacheSubdir));

    rv = aNewCacheSubdir->AppendNative(nsDependentCString(aCacheSubdir));
    if (NS_FAILED(rv))
        return;

    PathString newPath = aNewCacheSubdir->NativePath();

    if (NS_SUCCEEDED(aNewCacheSubdir->Exists(&exists)) && !exists) {
        // New cache directory does not exist, try to move the old one here
        // rename needs an empty target directory

        // Make sure the parent of the target sub-dir exists
        rv = aNewCacheDir->Create(nsIFile::DIRECTORY_TYPE, 0777);
        if (NS_SUCCEEDED(rv) || NS_ERROR_FILE_ALREADY_EXISTS == rv) {
            PathString oldPath = aOldCacheSubdir->NativePath();
=======
void nsCacheService::LogCacheStatistics() {
  uint32_t hitPercentage = 0;
  double sum = (double)(mCacheHits + mCacheMisses);
  if (sum != 0) {
    hitPercentage = (uint32_t)((((double)mCacheHits) / sum) * 100);
  }
  CACHE_LOG_INFO(("\nCache Service Statistics:\n\n"));
  CACHE_LOG_INFO(("    TotalEntries   = %d\n", mTotalEntries));
  CACHE_LOG_INFO(("    Cache Hits     = %d\n", mCacheHits));
  CACHE_LOG_INFO(("    Cache Misses   = %d\n", mCacheMisses));
  CACHE_LOG_INFO(("    Cache Hit %%    = %d%%\n", hitPercentage));
  CACHE_LOG_INFO(("    Max Key Length = %d\n", mMaxKeyLength));
  CACHE_LOG_INFO(("    Max Meta Size  = %d\n", mMaxMetaSize));
  CACHE_LOG_INFO(("    Max Data Size  = %d\n", mMaxDataSize));
  CACHE_LOG_INFO(("\n"));
  CACHE_LOG_INFO(
      ("    Deactivate Failures         = %d\n", mDeactivateFailures));
  CACHE_LOG_INFO(
      ("    Deactivated Unbound Entries = %d\n", mDeactivatedUnboundEntries));
}

void nsCacheService::MoveOrRemoveDiskCache(nsIFile* aOldCacheDir,
                                           nsIFile* aNewCacheDir,
                                           const char* aCacheSubdir) {
  bool same;
  if (NS_FAILED(aOldCacheDir->Equals(aNewCacheDir, &same)) || same) return;

  nsCOMPtr<nsIFile> aOldCacheSubdir;
  aOldCacheDir->Clone(getter_AddRefs(aOldCacheSubdir));

  nsresult rv = aOldCacheSubdir->AppendNative(nsDependentCString(aCacheSubdir));
  if (NS_FAILED(rv)) return;

  bool exists;
  if (NS_FAILED(aOldCacheSubdir->Exists(&exists)) || !exists) return;

  nsCOMPtr<nsIFile> aNewCacheSubdir;
  aNewCacheDir->Clone(getter_AddRefs(aNewCacheSubdir));

  rv = aNewCacheSubdir->AppendNative(nsDependentCString(aCacheSubdir));
  if (NS_FAILED(rv)) return;

  PathString newPath = aNewCacheSubdir->NativePath();

  if (NS_SUCCEEDED(aNewCacheSubdir->Exists(&exists)) && !exists) {
    // New cache directory does not exist, try to move the old one here
    // rename needs an empty target directory

    // Make sure the parent of the target sub-dir exists
    rv = aNewCacheDir->Create(nsIFile::DIRECTORY_TYPE, 0777);
    if (NS_SUCCEEDED(rv) || NS_ERROR_FILE_ALREADY_EXISTS == rv) {
      PathString oldPath = aOldCacheSubdir->NativePath();
>>>>>>> upstream-releases
#ifdef XP_WIN
      if (MoveFileW(oldPath.get(), newPath.get()))
#else
      if (rename(oldPath.get(), newPath.get()) == 0)
#endif
      {
        return;
      }
    }
  }

<<<<<<< HEAD
  // Delay delete by 1 minute to avoid IO thrash on startup.
  nsDeleteDir::DeleteDir(aOldCacheSubdir, false, 60000);
}

static bool IsEntryPrivate(nsCacheEntry *entry) { return entry->IsPrivate(); }

void nsCacheService::LeavePrivateBrowsing() {
  nsCacheServiceAutoLock lock;

  gService->DoomActiveEntries(IsEntryPrivate);

  if (gService->mMemoryDevice) {
    // clear memory cache
    gService->mMemoryDevice->EvictPrivateEntries();
  }
||||||| merged common ancestors
    // Delay delete by 1 minute to avoid IO thrash on startup.
    nsDeleteDir::DeleteDir(aOldCacheSubdir, false, 60000);
}

static bool
IsEntryPrivate(nsCacheEntry* entry)
{
    return entry->IsPrivate();
}

void
nsCacheService::LeavePrivateBrowsing()
{
    nsCacheServiceAutoLock lock;

    gService->DoomActiveEntries(IsEntryPrivate);

    if (gService->mMemoryDevice) {
        // clear memory cache
        gService->mMemoryDevice->EvictPrivateEntries();
    }
=======
  // Delay delete by 1 minute to avoid IO thrash on startup.
  nsDeleteDir::DeleteDir(aOldCacheSubdir, false, 60000);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
MOZ_DEFINE_MALLOC_SIZE_OF(DiskCacheDeviceMallocSizeOf)

NS_IMETHODIMP
nsCacheService::CollectReports(nsIHandleReportCallback *aHandleReport,
                               nsISupports *aData, bool aAnonymize) {
  size_t disk = 0;
  if (mDiskDevice) {
    nsCacheServiceAutoLock lock(LOCK_TELEM(NSCACHESERVICE_DISKDEVICEHEAPSIZE));
    disk = mDiskDevice->SizeOfIncludingThis(DiskCacheDeviceMallocSizeOf);
  }

  size_t memory = mMemoryDevice ? mMemoryDevice->TotalSize() : 0;
||||||| merged common ancestors
MOZ_DEFINE_MALLOC_SIZE_OF(DiskCacheDeviceMallocSizeOf)

NS_IMETHODIMP
nsCacheService::CollectReports(nsIHandleReportCallback* aHandleReport,
                               nsISupports* aData, bool aAnonymize)
{
    size_t disk = 0;
    if (mDiskDevice) {
        nsCacheServiceAutoLock
            lock(LOCK_TELEM(NSCACHESERVICE_DISKDEVICEHEAPSIZE));
        disk = mDiskDevice->SizeOfIncludingThis(DiskCacheDeviceMallocSizeOf);
    }

    size_t memory = mMemoryDevice ? mMemoryDevice->TotalSize() : 0;
=======
static bool IsEntryPrivate(nsCacheEntry* entry) { return entry->IsPrivate(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_COLLECT_REPORT("explicit/network/disk-cache", KIND_HEAP, UNITS_BYTES,
                     disk, "Memory used by the network disk cache.");
||||||| merged common ancestors
    MOZ_COLLECT_REPORT(
        "explicit/network/disk-cache", KIND_HEAP, UNITS_BYTES, disk,
        "Memory used by the network disk cache.");
=======
void nsCacheService::LeavePrivateBrowsing() {
  nsCacheServiceAutoLock lock;
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_COLLECT_REPORT("explicit/network/memory-cache", KIND_HEAP, UNITS_BYTES,
                     memory, "Memory used by the network memory cache.");

  return NS_OK;
||||||| merged common ancestors
    MOZ_COLLECT_REPORT(
        "explicit/network/memory-cache", KIND_HEAP, UNITS_BYTES, memory,
        "Memory used by the network memory cache.");

    return NS_OK;
=======
  gService->DoomActiveEntries(IsEntryPrivate);
>>>>>>> upstream-releases
}
