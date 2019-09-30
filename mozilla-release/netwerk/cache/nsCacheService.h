/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _nsCacheService_h_
#define _nsCacheService_h_

#include "nsICacheService.h"
#include "nsCacheSession.h"
#include "nsCacheDevice.h"
#include "nsCacheEntry.h"
#include "nsThreadUtils.h"
#include "nsICacheListener.h"

#include "prthread.h"
#include "nsIObserver.h"
#include "nsString.h"
#include "nsTArray.h"
#include "nsRefPtrHashtable.h"
#include "mozilla/CondVar.h"
#include "mozilla/Mutex.h"
#include "mozilla/Telemetry.h"

class nsCacheRequest;
class nsCacheProfilePrefObserver;
class nsOfflineCacheDevice;
class nsCacheServiceAutoLock;
class nsITimer;
class mozIStorageService;

/******************************************************************************
 * nsNotifyDoomListener
 *****************************************************************************/

class nsNotifyDoomListener : public mozilla::Runnable {
<<<<<<< HEAD
 public:
  nsNotifyDoomListener(nsICacheListener *listener, nsresult status)
      : mozilla::Runnable("nsNotifyDoomListener"),
        mListener(listener)  // transfers reference
        ,
        mStatus(status) {}

  NS_IMETHOD Run() override {
||||||| merged common ancestors
public:
  nsNotifyDoomListener(nsICacheListener* listener, nsresult status)
    : mozilla::Runnable("nsNotifyDoomListener")
    , mListener(listener) // transfers reference
    , mStatus(status)
  {
  }

  NS_IMETHOD Run() override
  {
=======
 public:
  nsNotifyDoomListener(nsICacheListener* listener, nsresult status)
      : mozilla::Runnable("nsNotifyDoomListener"),
        mListener(listener)  // transfers reference
        ,
        mStatus(status) {}

  NS_IMETHOD Run() override {
>>>>>>> upstream-releases
    mListener->OnCacheEntryDoomed(mStatus);
    NS_RELEASE(mListener);
    return NS_OK;
  }

<<<<<<< HEAD
 private:
  nsICacheListener *mListener;
  nsresult mStatus;
||||||| merged common ancestors
private:
    nsICacheListener *mListener;
    nsresult          mStatus;
=======
 private:
  nsICacheListener* mListener;
  nsresult mStatus;
>>>>>>> upstream-releases
};

/******************************************************************************
 *  nsCacheService
 ******************************************************************************/

<<<<<<< HEAD
class nsCacheService final : public nsICacheServiceInternal,
                             public nsIMemoryReporter {
  virtual ~nsCacheService();

 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSICACHESERVICE
  NS_DECL_NSICACHESERVICEINTERNAL
  NS_DECL_NSIMEMORYREPORTER

  nsCacheService();

  // Define a Create method to be used with a factory:
  static nsresult Create(nsISupports *outer, const nsIID &iid, void **result);

  /**
   * Methods called by nsCacheSession
   */
  static nsresult OpenCacheEntry(nsCacheSession *session, const nsACString &key,
                                 nsCacheAccessMode accessRequested,
                                 bool blockingMode, nsICacheListener *listener,
                                 nsICacheEntryDescriptor **result);
||||||| merged common ancestors
class nsCacheService final : public nsICacheServiceInternal,
                             public nsIMemoryReporter
{
    virtual ~nsCacheService();

public:
    NS_DECL_THREADSAFE_ISUPPORTS
    NS_DECL_NSICACHESERVICE
    NS_DECL_NSICACHESERVICEINTERNAL
    NS_DECL_NSIMEMORYREPORTER

    nsCacheService();

    // Define a Create method to be used with a factory:
    static nsresult
    Create(nsISupports* outer, const nsIID& iid, void* *result);


    /**
     * Methods called by nsCacheSession
     */
    static nsresult  OpenCacheEntry(nsCacheSession *           session,
                                    const nsACString &         key,
                                    nsCacheAccessMode          accessRequested,
                                    bool                       blockingMode,
                                    nsICacheListener *         listener,
                                    nsICacheEntryDescriptor ** result);
=======
class nsCacheService final : public nsICacheServiceInternal {
  virtual ~nsCacheService();
>>>>>>> upstream-releases

<<<<<<< HEAD
  static nsresult EvictEntriesForSession(nsCacheSession *session);
||||||| merged common ancestors
    static nsresult  EvictEntriesForSession(nsCacheSession *   session);
=======
 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSICACHESERVICE
  NS_DECL_NSICACHESERVICEINTERNAL
>>>>>>> upstream-releases

<<<<<<< HEAD
  static nsresult IsStorageEnabledForPolicy(nsCacheStoragePolicy storagePolicy,
                                            bool *result);
||||||| merged common ancestors
    static nsresult  IsStorageEnabledForPolicy(nsCacheStoragePolicy  storagePolicy,
                                               bool *              result);
=======
  nsCacheService();
>>>>>>> upstream-releases

<<<<<<< HEAD
  static nsresult DoomEntry(nsCacheSession *session, const nsACString &key,
                            nsICacheListener *listener);
||||||| merged common ancestors
    static nsresult  DoomEntry(nsCacheSession   *session,
                               const nsACString &key,
                               nsICacheListener *listener);
=======
  // Define a Create method to be used with a factory:
  static nsresult Create(nsISupports* outer, const nsIID& iid, void** result);
>>>>>>> upstream-releases

<<<<<<< HEAD
  /**
   * Methods called by nsCacheEntryDescriptor
   */
||||||| merged common ancestors
    /**
     * Methods called by nsCacheEntryDescriptor
     */
=======
  /**
   * Methods called by nsCacheSession
   */
  static nsresult OpenCacheEntry(nsCacheSession* session, const nsACString& key,
                                 nsCacheAccessMode accessRequested,
                                 bool blockingMode, nsICacheListener* listener,
                                 nsICacheEntryDescriptor** result);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void CloseDescriptor(nsCacheEntryDescriptor *descriptor);
||||||| merged common ancestors
    static void      CloseDescriptor(nsCacheEntryDescriptor * descriptor);
=======
  static nsresult EvictEntriesForSession(nsCacheSession* session);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static nsresult GetFileForEntry(nsCacheEntry *entry, nsIFile **result);
||||||| merged common ancestors
    static nsresult  GetFileForEntry(nsCacheEntry *         entry,
                                     nsIFile **             result);
=======
  static nsresult IsStorageEnabledForPolicy(nsCacheStoragePolicy storagePolicy,
                                            bool* result);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static nsresult OpenInputStreamForEntry(nsCacheEntry *entry,
                                          nsCacheAccessMode mode,
                                          uint32_t offset,
                                          nsIInputStream **result);
||||||| merged common ancestors
    static nsresult  OpenInputStreamForEntry(nsCacheEntry *     entry,
                                             nsCacheAccessMode  mode,
                                             uint32_t           offset,
                                             nsIInputStream **  result);
=======
  static nsresult DoomEntry(nsCacheSession* session, const nsACString& key,
                            nsICacheListener* listener);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static nsresult OpenOutputStreamForEntry(nsCacheEntry *entry,
                                           nsCacheAccessMode mode,
                                           uint32_t offset,
                                           nsIOutputStream **result);
||||||| merged common ancestors
    static nsresult  OpenOutputStreamForEntry(nsCacheEntry *     entry,
                                              nsCacheAccessMode  mode,
                                              uint32_t           offset,
                                              nsIOutputStream ** result);
=======
  /**
   * Methods called by nsCacheEntryDescriptor
   */
>>>>>>> upstream-releases

<<<<<<< HEAD
  static nsresult OnDataSizeChange(nsCacheEntry *entry, int32_t deltaSize);
||||||| merged common ancestors
    static nsresult  OnDataSizeChange(nsCacheEntry * entry, int32_t deltaSize);
=======
  static void CloseDescriptor(nsCacheEntryDescriptor* descriptor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static nsresult SetCacheElement(nsCacheEntry *entry, nsISupports *element);
||||||| merged common ancestors
    static nsresult  SetCacheElement(nsCacheEntry * entry, nsISupports * element);
=======
  static nsresult GetFileForEntry(nsCacheEntry* entry, nsIFile** result);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static nsresult ValidateEntry(nsCacheEntry *entry);
||||||| merged common ancestors
    static nsresult  ValidateEntry(nsCacheEntry * entry);
=======
  static nsresult OpenInputStreamForEntry(nsCacheEntry* entry,
                                          nsCacheAccessMode mode,
                                          uint32_t offset,
                                          nsIInputStream** result);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static int32_t CacheCompressionLevel();
||||||| merged common ancestors
    static int32_t   CacheCompressionLevel();
=======
  static nsresult OpenOutputStreamForEntry(nsCacheEntry* entry,
                                           nsCacheAccessMode mode,
                                           uint32_t offset,
                                           nsIOutputStream** result);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool GetClearingEntries();
||||||| merged common ancestors
    static bool      GetClearingEntries();
=======
  static nsresult OnDataSizeChange(nsCacheEntry* entry, int32_t deltaSize);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void GetCacheBaseDirectoty(nsIFile **result);
  static void GetDiskCacheDirectory(nsIFile **result);
  static void GetAppCacheDirectory(nsIFile **result);
||||||| merged common ancestors
    static void      GetCacheBaseDirectoty(nsIFile ** result);
    static void      GetDiskCacheDirectory(nsIFile ** result);
    static void      GetAppCacheDirectory(nsIFile ** result);
=======
  static nsresult SetCacheElement(nsCacheEntry* entry, nsISupports* element);
>>>>>>> upstream-releases

<<<<<<< HEAD
  /**
   * Methods called by any cache classes
   */
||||||| merged common ancestors
    /**
     * Methods called by any cache classes
     */
=======
  static nsresult ValidateEntry(nsCacheEntry* entry);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static nsCacheService *GlobalInstance() { return gService; }
||||||| merged common ancestors
    static
    nsCacheService * GlobalInstance()   { return gService; }
=======
  static int32_t CacheCompressionLevel();
>>>>>>> upstream-releases

<<<<<<< HEAD
  static nsresult DoomEntry(nsCacheEntry *entry);
||||||| merged common ancestors
    static nsresult  DoomEntry(nsCacheEntry * entry);
=======
  static bool GetClearingEntries();
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool IsStorageEnabledForPolicy_Locked(nsCacheStoragePolicy policy);
||||||| merged common ancestors
    static bool      IsStorageEnabledForPolicy_Locked(nsCacheStoragePolicy policy);
=======
  static void GetCacheBaseDirectoty(nsIFile** result);
  static void GetDiskCacheDirectory(nsIFile** result);
  static void GetAppCacheDirectory(nsIFile** result);
>>>>>>> upstream-releases

<<<<<<< HEAD
  /**
   * Called by disk cache to notify us to use the new max smart size
   */
  static void MarkStartingFresh();
||||||| merged common ancestors
    /**
     * Called by disk cache to notify us to use the new max smart size
     */
    static void      MarkStartingFresh();
=======
  /**
   * Methods called by any cache classes
   */
>>>>>>> upstream-releases

<<<<<<< HEAD
  /**
   * Methods called by nsApplicationCacheService
   */
||||||| merged common ancestors
    /**
     * Methods called by nsApplicationCacheService
     */
=======
  static nsCacheService* GlobalInstance() { return gService; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult GetOfflineDevice(nsOfflineCacheDevice **aDevice);
||||||| merged common ancestors
    nsresult GetOfflineDevice(nsOfflineCacheDevice ** aDevice);
=======
  static nsresult DoomEntry(nsCacheEntry* entry);
>>>>>>> upstream-releases

<<<<<<< HEAD
  /**
   * Creates an offline cache device that works over a specific profile
   * directory. A tool to preload offline cache for profiles different from the
   * current application's profile directory.
   */
  nsresult GetCustomOfflineDevice(nsIFile *aProfileDir, int32_t aQuota,
                                  nsOfflineCacheDevice **aDevice);
||||||| merged common ancestors
    /**
     * Creates an offline cache device that works over a specific profile directory.
     * A tool to preload offline cache for profiles different from the current
     * application's profile directory.
     */
    nsresult GetCustomOfflineDevice(nsIFile *aProfileDir,
                                    int32_t aQuota,
                                    nsOfflineCacheDevice **aDevice);
=======
  static bool IsStorageEnabledForPolicy_Locked(nsCacheStoragePolicy policy);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // This method may be called to release an object while the cache service
  // lock is being held.  If a non-null target is specified and the target
  // does not correspond to the current thread, then the release will be
  // proxied to the specified target.  Otherwise, the object will be added to
  // the list of objects to be released when the cache service is unlocked.
  static void ReleaseObject_Locked(nsISupports *object,
                                   nsIEventTarget *target = nullptr);
||||||| merged common ancestors
    // This method may be called to release an object while the cache service
    // lock is being held.  If a non-null target is specified and the target
    // does not correspond to the current thread, then the release will be
    // proxied to the specified target.  Otherwise, the object will be added to
    // the list of objects to be released when the cache service is unlocked.
    static void      ReleaseObject_Locked(nsISupports *    object,
                                          nsIEventTarget * target = nullptr);
=======
  /**
   * Methods called by nsApplicationCacheService
   */
>>>>>>> upstream-releases

<<<<<<< HEAD
  static nsresult DispatchToCacheIOThread(nsIRunnable *event);
||||||| merged common ancestors
    static nsresult DispatchToCacheIOThread(nsIRunnable* event);
=======
  nsresult GetOfflineDevice(nsOfflineCacheDevice** aDevice);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Calling this method will block the calling thread until all pending
  // events on the cache-io thread has finished. The calling thread must
  // hold the cache-lock
  static nsresult SyncWithCacheIOThread();
||||||| merged common ancestors
    // Calling this method will block the calling thread until all pending
    // events on the cache-io thread has finished. The calling thread must
    // hold the cache-lock
    static nsresult SyncWithCacheIOThread();
=======
  /**
   * Creates an offline cache device that works over a specific profile
   * directory. A tool to preload offline cache for profiles different from the
   * current application's profile directory.
   */
  nsresult GetCustomOfflineDevice(nsIFile* aProfileDir, int32_t aQuota,
                                  nsOfflineCacheDevice** aDevice);
>>>>>>> upstream-releases

<<<<<<< HEAD
  /**
   * Methods called by nsCacheProfilePrefObserver
   */
  static void OnProfileShutdown();
  static void OnProfileChanged();
||||||| merged common ancestors
=======
  // This method may be called to release an object while the cache service
  // lock is being held.  If a non-null target is specified and the target
  // does not correspond to the current thread, then the release will be
  // proxied to the specified target.  Otherwise, the object will be added to
  // the list of objects to be released when the cache service is unlocked.
  static void ReleaseObject_Locked(nsISupports* object,
                                   nsIEventTarget* target = nullptr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void SetDiskCacheEnabled(bool enabled);
  // Sets the disk cache capacity (in kilobytes)
  static void SetDiskCacheCapacity(int32_t capacity);
  // Set max size for a disk-cache entry (in KB). -1 disables limit up to
  // 1/8th of disk cache size
  static void SetDiskCacheMaxEntrySize(int32_t maxSize);
  // Set max size for a memory-cache entry (in kilobytes). -1 disables
  // limit up to 90% of memory cache size
  static void SetMemoryCacheMaxEntrySize(int32_t maxSize);
||||||| merged common ancestors
    /**
     * Methods called by nsCacheProfilePrefObserver
     */
    static void      OnProfileShutdown();
    static void      OnProfileChanged();
=======
  static nsresult DispatchToCacheIOThread(nsIRunnable* event);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void SetOfflineCacheEnabled(bool enabled);
  // Sets the offline cache capacity (in kilobytes)
  static void SetOfflineCacheCapacity(int32_t capacity);
||||||| merged common ancestors
    static void      SetDiskCacheEnabled(bool    enabled);
    // Sets the disk cache capacity (in kilobytes)
    static void      SetDiskCacheCapacity(int32_t  capacity);
    // Set max size for a disk-cache entry (in KB). -1 disables limit up to
    // 1/8th of disk cache size
    static void      SetDiskCacheMaxEntrySize(int32_t  maxSize);
    // Set max size for a memory-cache entry (in kilobytes). -1 disables
    // limit up to 90% of memory cache size
    static void      SetMemoryCacheMaxEntrySize(int32_t  maxSize);
=======
  // Calling this method will block the calling thread until all pending
  // events on the cache-io thread has finished. The calling thread must
  // hold the cache-lock
  static nsresult SyncWithCacheIOThread();
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void SetMemoryCache();
||||||| merged common ancestors
    static void      SetOfflineCacheEnabled(bool    enabled);
    // Sets the offline cache capacity (in kilobytes)
    static void      SetOfflineCacheCapacity(int32_t  capacity);
=======
  /**
   * Methods called by nsCacheProfilePrefObserver
   */
  static void OnProfileShutdown();
  static void OnProfileChanged();
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void SetCacheCompressionLevel(int32_t level);
||||||| merged common ancestors
    static void      SetMemoryCache();
=======
  static void SetOfflineCacheEnabled(bool enabled);
  // Sets the offline cache capacity (in kilobytes)
  static void SetOfflineCacheCapacity(int32_t capacity);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Starts smart cache size computation if disk device is available
  static nsresult SetDiskSmartSize();
||||||| merged common ancestors
    static void      SetCacheCompressionLevel(int32_t level);
=======
  static void MoveOrRemoveDiskCache(nsIFile* aOldCacheDir,
                                    nsIFile* aNewCacheDir,
                                    const char* aCacheSubdir);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void MoveOrRemoveDiskCache(nsIFile *aOldCacheDir,
                                    nsIFile *aNewCacheDir,
                                    const char *aCacheSubdir);
||||||| merged common ancestors
    // Starts smart cache size computation if disk device is available
    static nsresult  SetDiskSmartSize();
=======
  nsresult Init();
  void Shutdown();
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult Init();
  void Shutdown();

  static bool IsInitialized() {
    if (!gService) {
      return false;
||||||| merged common ancestors
    static void      MoveOrRemoveDiskCache(nsIFile *aOldCacheDir,
                                           nsIFile *aNewCacheDir,
                                           const char *aCacheSubdir);

    nsresult         Init();
    void             Shutdown();

    static bool      IsInitialized()
    {
      if (!gService) {
          return false;
      }
      return gService->mInitialized;
=======
  static bool IsInitialized() {
    if (!gService) {
      return false;
>>>>>>> upstream-releases
    }
    return gService->mInitialized;
  }

<<<<<<< HEAD
  static void AssertOwnsLock() { gService->mLock.AssertCurrentThreadOwns(); }

  static void LeavePrivateBrowsing();
  bool IsDoomListEmpty();
||||||| merged common ancestors
    static void      AssertOwnsLock()
    { gService->mLock.AssertCurrentThreadOwns(); }

    static void      LeavePrivateBrowsing();
    bool             IsDoomListEmpty();
=======
  static void AssertOwnsLock() { gService->mLock.AssertCurrentThreadOwns(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  typedef bool (*DoomCheckFn)(nsCacheEntry *entry);
||||||| merged common ancestors
    typedef bool (*DoomCheckFn)(nsCacheEntry* entry);
=======
  static void LeavePrivateBrowsing();
  bool IsDoomListEmpty();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Accessors to the disabled functionality
  nsresult CreateSessionInternal(const char *clientID,
                                 nsCacheStoragePolicy storagePolicy,
                                 bool streamBased, nsICacheSession **result);
  nsresult VisitEntriesInternal(nsICacheVisitor *visitor);
  nsresult EvictEntriesInternal(nsCacheStoragePolicy storagePolicy);
||||||| merged common ancestors
    // Accessors to the disabled functionality
    nsresult CreateSessionInternal(const char *          clientID,
                                   nsCacheStoragePolicy  storagePolicy,
                                   bool                  streamBased,
                                   nsICacheSession     **result);
    nsresult VisitEntriesInternal(nsICacheVisitor *visitor);
    nsresult EvictEntriesInternal(nsCacheStoragePolicy storagePolicy);
=======
  typedef bool (*DoomCheckFn)(nsCacheEntry* entry);
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  friend class nsCacheServiceAutoLock;
  friend class nsOfflineCacheDevice;
  friend class nsProcessRequestEvent;
  friend class nsSetSmartSizeEvent;
  friend class nsBlockOnCacheThreadEvent;
  friend class nsSetDiskSmartSizeCallback;
  friend class nsDoomEvent;
  friend class nsDisableOldMaxSmartSizePrefEvent;
  friend class nsDiskCacheMap;
  friend class nsAsyncDoomEvent;
  friend class nsCacheEntryDescriptor;
||||||| merged common ancestors
private:
    friend class nsCacheServiceAutoLock;
    friend class nsOfflineCacheDevice;
    friend class nsProcessRequestEvent;
    friend class nsSetSmartSizeEvent;
    friend class nsBlockOnCacheThreadEvent;
    friend class nsSetDiskSmartSizeCallback;
    friend class nsDoomEvent;
    friend class nsDisableOldMaxSmartSizePrefEvent;
    friend class nsDiskCacheMap;
    friend class nsAsyncDoomEvent;
    friend class nsCacheEntryDescriptor;
=======
  // Accessors to the disabled functionality
  nsresult CreateSessionInternal(const char* clientID,
                                 nsCacheStoragePolicy storagePolicy,
                                 bool streamBased, nsICacheSession** result);
  nsresult VisitEntriesInternal(nsICacheVisitor* visitor);
  nsresult EvictEntriesInternal(nsCacheStoragePolicy storagePolicy);
>>>>>>> upstream-releases

<<<<<<< HEAD
  /**
   * Internal Methods
   */
||||||| merged common ancestors
    /**
     * Internal Methods
     */
=======
 private:
  friend class nsCacheServiceAutoLock;
  friend class nsOfflineCacheDevice;
  friend class nsProcessRequestEvent;
  friend class nsBlockOnCacheThreadEvent;
  friend class nsDoomEvent;
  friend class nsAsyncDoomEvent;
  friend class nsCacheEntryDescriptor;
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void Lock();
  static void Lock(::mozilla::Telemetry::HistogramID mainThreadLockerID);
  static void Unlock();
  void LockAcquired();
  void LockReleased();
||||||| merged common ancestors
    static void      Lock();
    static void      Lock(::mozilla::Telemetry::HistogramID mainThreadLockerID);
    static void      Unlock();
    void             LockAcquired();
    void             LockReleased();
=======
  /**
   * Internal Methods
   */
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult CreateDiskDevice();
  nsresult CreateOfflineDevice();
  nsresult CreateCustomOfflineDevice(nsIFile *aProfileDir, int32_t aQuota,
                                     nsOfflineCacheDevice **aDevice);
  nsresult CreateMemoryDevice();
||||||| merged common ancestors
    nsresult         CreateDiskDevice();
    nsresult         CreateOfflineDevice();
    nsresult         CreateCustomOfflineDevice(nsIFile *aProfileDir,
                                               int32_t aQuota,
                                               nsOfflineCacheDevice **aDevice);
    nsresult         CreateMemoryDevice();
=======
  static void Lock();
  static void Lock(::mozilla::Telemetry::HistogramID mainThreadLockerID);
  static void Unlock();
  void LockAcquired();
  void LockReleased();
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult RemoveCustomOfflineDevice(nsOfflineCacheDevice *aDevice);
||||||| merged common ancestors
    nsresult         RemoveCustomOfflineDevice(nsOfflineCacheDevice *aDevice);
=======
  nsresult CreateDiskDevice();
  nsresult CreateOfflineDevice();
  nsresult CreateCustomOfflineDevice(nsIFile* aProfileDir, int32_t aQuota,
                                     nsOfflineCacheDevice** aDevice);
  nsresult CreateMemoryDevice();
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult CreateRequest(nsCacheSession *session, const nsACString &clientKey,
                         nsCacheAccessMode accessRequested, bool blockingMode,
                         nsICacheListener *listener, nsCacheRequest **request);
||||||| merged common ancestors
    nsresult         CreateRequest(nsCacheSession *   session,
                                   const nsACString & clientKey,
                                   nsCacheAccessMode  accessRequested,
                                   bool               blockingMode,
                                   nsICacheListener * listener,
                                   nsCacheRequest **  request);
=======
  nsresult RemoveCustomOfflineDevice(nsOfflineCacheDevice* aDevice);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult DoomEntry_Internal(nsCacheEntry *entry,
                              bool doProcessPendingRequests);
||||||| merged common ancestors
    nsresult         DoomEntry_Internal(nsCacheEntry * entry,
                                        bool doProcessPendingRequests);
=======
  nsresult CreateRequest(nsCacheSession* session, const nsACString& clientKey,
                         nsCacheAccessMode accessRequested, bool blockingMode,
                         nsICacheListener* listener, nsCacheRequest** request);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult EvictEntriesForClient(const char *clientID,
                                 nsCacheStoragePolicy storagePolicy);
||||||| merged common ancestors
    nsresult         EvictEntriesForClient(const char *          clientID,
                                           nsCacheStoragePolicy  storagePolicy);
=======
  nsresult DoomEntry_Internal(nsCacheEntry* entry,
                              bool doProcessPendingRequests);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Notifies request listener asynchronously on the request's thread, and
  // releases the descriptor on the request's thread.  If this method fails,
  // the descriptor is not released.
  nsresult NotifyListener(nsCacheRequest *request,
                          nsICacheEntryDescriptor *descriptor,
                          nsCacheAccessMode accessGranted, nsresult error);
||||||| merged common ancestors
    // Notifies request listener asynchronously on the request's thread, and
    // releases the descriptor on the request's thread.  If this method fails,
    // the descriptor is not released.
    nsresult         NotifyListener(nsCacheRequest *          request,
                                    nsICacheEntryDescriptor * descriptor,
                                    nsCacheAccessMode         accessGranted,
                                    nsresult                  error);
=======
  nsresult EvictEntriesForClient(const char* clientID,
                                 nsCacheStoragePolicy storagePolicy);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult ActivateEntry(nsCacheRequest *request, nsCacheEntry **entry,
                         nsCacheEntry **doomedEntry);
||||||| merged common ancestors
    nsresult         ActivateEntry(nsCacheRequest * request,
                                   nsCacheEntry ** entry,
                                   nsCacheEntry ** doomedEntry);
=======
  // Notifies request listener asynchronously on the request's thread, and
  // releases the descriptor on the request's thread.  If this method fails,
  // the descriptor is not released.
  nsresult NotifyListener(nsCacheRequest* request,
                          nsICacheEntryDescriptor* descriptor,
                          nsCacheAccessMode accessGranted, nsresult error);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCacheDevice *EnsureEntryHasDevice(nsCacheEntry *entry);
||||||| merged common ancestors
    nsCacheDevice *  EnsureEntryHasDevice(nsCacheEntry * entry);
=======
  nsresult ActivateEntry(nsCacheRequest* request, nsCacheEntry** entry,
                         nsCacheEntry** doomedEntry);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCacheEntry *SearchCacheDevices(nsCString *key, nsCacheStoragePolicy policy,
                                   bool *collision);
||||||| merged common ancestors
    nsCacheEntry *   SearchCacheDevices(nsCString * key, nsCacheStoragePolicy policy, bool *collision);
=======
  nsCacheDevice* EnsureEntryHasDevice(nsCacheEntry* entry);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void DeactivateEntry(nsCacheEntry *entry);
||||||| merged common ancestors
    void             DeactivateEntry(nsCacheEntry * entry);
=======
  nsCacheEntry* SearchCacheDevices(nsCString* key, nsCacheStoragePolicy policy,
                                   bool* collision);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult ProcessRequest(nsCacheRequest *request,
                          bool calledFromOpenCacheEntry,
                          nsICacheEntryDescriptor **result);
||||||| merged common ancestors
    nsresult         ProcessRequest(nsCacheRequest *           request,
                                    bool                       calledFromOpenCacheEntry,
                                    nsICacheEntryDescriptor ** result);
=======
  void DeactivateEntry(nsCacheEntry* entry);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult ProcessPendingRequests(nsCacheEntry *entry);
||||||| merged common ancestors
    nsresult         ProcessPendingRequests(nsCacheEntry * entry);
=======
  nsresult ProcessRequest(nsCacheRequest* request,
                          bool calledFromOpenCacheEntry,
                          nsICacheEntryDescriptor** result);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void ClearDoomList(void);
  void DoomActiveEntries(DoomCheckFn check);
  void CloseAllStreams();
  void FireClearNetworkCacheStoredAnywhereNotification();
||||||| merged common ancestors
    void             ClearDoomList(void);
    void             DoomActiveEntries(DoomCheckFn check);
    void             CloseAllStreams();
    void             FireClearNetworkCacheStoredAnywhereNotification();
=======
  nsresult ProcessPendingRequests(nsCacheEntry* entry);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void LogCacheStatistics();
||||||| merged common ancestors
    void LogCacheStatistics();
=======
  void ClearDoomList(void);
  void DoomActiveEntries(DoomCheckFn check);
  void CloseAllStreams();
  void FireClearNetworkCacheStoredAnywhereNotification();
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult SetDiskSmartSize_Locked();
||||||| merged common ancestors
    nsresult         SetDiskSmartSize_Locked();
=======
  void LogCacheStatistics();
>>>>>>> upstream-releases

  /**
   *  Data Members
   */

<<<<<<< HEAD
  static nsCacheService *gService;  // there can be only one...
||||||| merged common ancestors
    static nsCacheService *         gService;  // there can be only one...
=======
  static nsCacheService* gService;  // there can be only one...
>>>>>>> upstream-releases

  nsCOMPtr<mozIStorageService> mStorageService;

<<<<<<< HEAD
  nsCacheProfilePrefObserver *mObserver;
||||||| merged common ancestors
    nsCacheProfilePrefObserver *    mObserver;
=======
  nsCacheProfilePrefObserver* mObserver;
>>>>>>> upstream-releases

  mozilla::Mutex mLock;
  mozilla::CondVar mCondVar;
  bool mNotified;

  mozilla::Mutex mTimeStampLock;
  mozilla::TimeStamp mLockAcquiredTimeStamp;

  nsCOMPtr<nsIThread> mCacheIOThread;

<<<<<<< HEAD
  nsTArray<nsISupports *> mDoomedObjects;
  nsCOMPtr<nsITimer> mSmartSizeTimer;
||||||| merged common ancestors
    nsTArray<nsISupports*>          mDoomedObjects;
    nsCOMPtr<nsITimer>              mSmartSizeTimer;
=======
  nsTArray<nsISupports*> mDoomedObjects;
>>>>>>> upstream-releases

  bool mInitialized;
  bool mClearingEntries;

<<<<<<< HEAD
  bool mEnableMemoryDevice;
  bool mEnableDiskDevice;
  bool mEnableOfflineDevice;
||||||| merged common ancestors
    bool                            mEnableMemoryDevice;
    bool                            mEnableDiskDevice;
    bool                            mEnableOfflineDevice;
=======
  bool mEnableOfflineDevice;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsMemoryCacheDevice *mMemoryDevice;
  nsDiskCacheDevice *mDiskDevice;
  nsOfflineCacheDevice *mOfflineDevice;
||||||| merged common ancestors
    nsMemoryCacheDevice *           mMemoryDevice;
    nsDiskCacheDevice *             mDiskDevice;
    nsOfflineCacheDevice *          mOfflineDevice;
=======
  nsOfflineCacheDevice* mOfflineDevice;
>>>>>>> upstream-releases

  nsRefPtrHashtable<nsStringHashKey, nsOfflineCacheDevice>
      mCustomOfflineDevices;

  nsCacheEntryHashTable mActiveEntries;
  PRCList mDoomedEntries;

  // stats

  uint32_t mTotalEntries;
  uint32_t mCacheHits;
  uint32_t mCacheMisses;
  uint32_t mMaxKeyLength;
  uint32_t mMaxDataSize;
  uint32_t mMaxMetaSize;

  // Unexpected error totals
  uint32_t mDeactivateFailures;
  uint32_t mDeactivatedUnboundEntries;
};

/******************************************************************************
 *  nsCacheServiceAutoLock
 ******************************************************************************/

#define LOCK_TELEM(x) \
  (::mozilla::Telemetry::CACHE_SERVICE_LOCK_WAIT_MAINTHREAD_##x)

// Instantiate this class to acquire the cache service lock for a particular
// execution scope.
class nsCacheServiceAutoLock {
 public:
  nsCacheServiceAutoLock() { nsCacheService::Lock(); }
  explicit nsCacheServiceAutoLock(
      mozilla::Telemetry::HistogramID mainThreadLockerID) {
    nsCacheService::Lock(mainThreadLockerID);
  }
  ~nsCacheServiceAutoLock() { nsCacheService::Unlock(); }
};

#endif  // _nsCacheService_h_
