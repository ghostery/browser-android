/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsCache.h"
#include "nspr.h"
#include "nsCacheEntry.h"
#include "nsCacheEntryDescriptor.h"
#include "nsCacheMetaData.h"
#include "nsCacheRequest.h"
#include "nsThreadUtils.h"
#include "nsError.h"
#include "nsICacheService.h"
#include "nsCacheService.h"
#include "nsCacheDevice.h"
#include "nsHashKeys.h"

using namespace mozilla;

<<<<<<< HEAD
nsCacheEntry::nsCacheEntry(const nsACString &key, bool streamBased,
||||||| merged common ancestors
nsCacheEntry::nsCacheEntry(const nsACString &   key,
                           bool                 streamBased,
=======
nsCacheEntry::nsCacheEntry(const nsACString& key, bool streamBased,
>>>>>>> upstream-releases
                           nsCacheStoragePolicy storagePolicy)
    : mKey(key),
      mFetchCount(0),
      mLastFetched(0),
      mLastModified(0),
      mLastValidated(0),
      mExpirationTime(nsICache::NO_EXPIRATION_TIME),
      mFlags(0),
      mPredictedDataSize(-1),
      mDataSize(0),
      mCacheDevice(nullptr),
      mCustomDevice(nullptr),
      mData(nullptr),
      mRequestQ{},
      mDescriptorQ{} {
  MOZ_COUNT_CTOR(nsCacheEntry);
  PR_INIT_CLIST(this);
  PR_INIT_CLIST(&mRequestQ);
  PR_INIT_CLIST(&mDescriptorQ);

  if (streamBased) MarkStreamBased();
  SetStoragePolicy(storagePolicy);

  MarkPublic();
}

nsCacheEntry::~nsCacheEntry() {
  MOZ_COUNT_DTOR(nsCacheEntry);

  if (mData) nsCacheService::ReleaseObject_Locked(mData, mEventTarget);
}

<<<<<<< HEAD
nsresult nsCacheEntry::Create(const char *key, bool streamBased,
                              nsCacheStoragePolicy storagePolicy,
                              nsCacheDevice *device, nsCacheEntry **result) {
  nsCacheEntry *entry =
      new nsCacheEntry(nsCString(key), streamBased, storagePolicy);
  entry->SetCacheDevice(device);
  *result = entry;
  return NS_OK;
||||||| merged common ancestors

nsresult
nsCacheEntry::Create( const char *          key,
                      bool                  streamBased,
                      nsCacheStoragePolicy  storagePolicy,
                      nsCacheDevice *       device,
                      nsCacheEntry **       result)
{
    nsCacheEntry* entry = new nsCacheEntry(nsCString(key),
                                           streamBased,
                                           storagePolicy);
    entry->SetCacheDevice(device);
    *result = entry;
    return NS_OK;
=======
nsresult nsCacheEntry::Create(const char* key, bool streamBased,
                              nsCacheStoragePolicy storagePolicy,
                              nsCacheDevice* device, nsCacheEntry** result) {
  nsCacheEntry* entry =
      new nsCacheEntry(nsCString(key), streamBased, storagePolicy);
  entry->SetCacheDevice(device);
  *result = entry;
  return NS_OK;
>>>>>>> upstream-releases
}

void nsCacheEntry::Fetched() {
  mLastFetched = SecondsFromPRTime(PR_Now());
  ++mFetchCount;
  MarkEntryDirty();
}

<<<<<<< HEAD
const char *nsCacheEntry::GetDeviceID() {
  if (mCacheDevice) return mCacheDevice->GetDeviceID();
  return nullptr;
||||||| merged common ancestors

const char *
nsCacheEntry::GetDeviceID()
{
    if (mCacheDevice)  return mCacheDevice->GetDeviceID();
    return nullptr;
=======
const char* nsCacheEntry::GetDeviceID() {
  if (mCacheDevice) return mCacheDevice->GetDeviceID();
  return nullptr;
>>>>>>> upstream-releases
}

void nsCacheEntry::TouchData() {
  mLastModified = SecondsFromPRTime(PR_Now());
  MarkDataDirty();
}

<<<<<<< HEAD
void nsCacheEntry::SetData(nsISupports *data) {
  if (mData) {
    nsCacheService::ReleaseObject_Locked(mData, mEventTarget);
    mData = nullptr;
  }
||||||| merged common ancestors
=======
void nsCacheEntry::SetData(nsISupports* data) {
  if (mData) {
    nsCacheService::ReleaseObject_Locked(mData, mEventTarget);
    mData = nullptr;
  }
>>>>>>> upstream-releases

  if (data) {
    NS_ADDREF(mData = data);
    mEventTarget = GetCurrentThreadEventTarget();
  }
}

void nsCacheEntry::TouchMetaData() {
  mLastModified = SecondsFromPRTime(PR_Now());
  MarkMetaDataDirty();
}

/**
 *  cache entry states
 *      0 descriptors (new entry)
 *      0 descriptors (existing, bound entry)
 *      n descriptors (existing, bound entry) valid
 *      n descriptors (existing, bound entry) not valid (wait until valid or
 *                    doomed)
 */

<<<<<<< HEAD
nsresult nsCacheEntry::RequestAccess(nsCacheRequest *request,
                                     nsCacheAccessMode *accessGranted) {
  nsresult rv = NS_OK;
||||||| merged common ancestors
nsresult
nsCacheEntry::RequestAccess(nsCacheRequest * request, nsCacheAccessMode *accessGranted)
{
    nsresult  rv = NS_OK;

    if (IsDoomed()) return NS_ERROR_CACHE_ENTRY_DOOMED;
=======
nsresult nsCacheEntry::RequestAccess(nsCacheRequest* request,
                                     nsCacheAccessMode* accessGranted) {
  nsresult rv = NS_OK;
>>>>>>> upstream-releases

  if (IsDoomed()) return NS_ERROR_CACHE_ENTRY_DOOMED;

  if (!IsInitialized()) {
    // brand new, unbound entry
    if (request->IsStreamBased()) MarkStreamBased();
    MarkInitialized();

    *accessGranted = request->AccessRequested() & nsICache::ACCESS_WRITE;
    NS_ASSERTION(*accessGranted, "new cache entry for READ-ONLY request");
    PR_APPEND_LINK(request, &mRequestQ);
    return rv;
  }

  if (IsStreamData() != request->IsStreamBased()) {
    *accessGranted = nsICache::ACCESS_NONE;
    return request->IsStreamBased() ? NS_ERROR_CACHE_DATA_IS_NOT_STREAM
                                    : NS_ERROR_CACHE_DATA_IS_STREAM;
  }

  if (PR_CLIST_IS_EMPTY(&mDescriptorQ)) {
    // 1st descriptor for existing bound entry
    *accessGranted = request->AccessRequested();
    if (*accessGranted & nsICache::ACCESS_WRITE) {
      MarkInvalid();
    } else {
      MarkValid();
    }
  } else {
    // nth request for existing, bound entry
    *accessGranted = request->AccessRequested() & ~nsICache::ACCESS_WRITE;
    if (!IsValid()) rv = NS_ERROR_CACHE_WAIT_FOR_VALIDATION;
  }
  PR_APPEND_LINK(request, &mRequestQ);

  return rv;
}

<<<<<<< HEAD
nsresult nsCacheEntry::CreateDescriptor(nsCacheRequest *request,
                                        nsCacheAccessMode accessGranted,
                                        nsICacheEntryDescriptor **result) {
  NS_ENSURE_ARG_POINTER(request && result);
||||||| merged common ancestors
=======
nsresult nsCacheEntry::CreateDescriptor(nsCacheRequest* request,
                                        nsCacheAccessMode accessGranted,
                                        nsICacheEntryDescriptor** result) {
  NS_ENSURE_ARG_POINTER(request && result);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCacheEntryDescriptor *descriptor =
      new nsCacheEntryDescriptor(this, accessGranted);
||||||| merged common ancestors
nsresult
nsCacheEntry::CreateDescriptor(nsCacheRequest *           request,
                               nsCacheAccessMode          accessGranted,
                               nsICacheEntryDescriptor ** result)
{
    NS_ENSURE_ARG_POINTER(request && result);

    nsCacheEntryDescriptor * descriptor =
        new nsCacheEntryDescriptor(this, accessGranted);
=======
  nsCacheEntryDescriptor* descriptor =
      new nsCacheEntryDescriptor(this, accessGranted);
>>>>>>> upstream-releases

  // XXX check request is on q
  PR_REMOVE_AND_INIT_LINK(request);  // remove request regardless of success

  if (descriptor == nullptr) return NS_ERROR_OUT_OF_MEMORY;

  PR_APPEND_LINK(descriptor, &mDescriptorQ);

  CACHE_LOG_DEBUG(("  descriptor %p created for request %p on entry %p\n",
                   descriptor, request, this));

  NS_ADDREF(*result = descriptor);
  return NS_OK;
}

<<<<<<< HEAD
bool nsCacheEntry::RemoveRequest(nsCacheRequest *request) {
  // XXX if debug: verify this request belongs to this entry
  PR_REMOVE_AND_INIT_LINK(request);
||||||| merged common ancestors
=======
bool nsCacheEntry::RemoveRequest(nsCacheRequest* request) {
  // XXX if debug: verify this request belongs to this entry
  PR_REMOVE_AND_INIT_LINK(request);
>>>>>>> upstream-releases

  // return true if this entry should stay active
  return !((PR_CLIST_IS_EMPTY(&mRequestQ)) &&
           (PR_CLIST_IS_EMPTY(&mDescriptorQ)));
}

<<<<<<< HEAD
bool nsCacheEntry::RemoveDescriptor(nsCacheEntryDescriptor *descriptor,
                                    bool *doomEntry) {
  NS_ASSERTION(descriptor->CacheEntry() == this, "### Wrong cache entry!!");
||||||| merged common ancestors
=======
bool nsCacheEntry::RemoveDescriptor(nsCacheEntryDescriptor* descriptor,
                                    bool* doomEntry) {
  NS_ASSERTION(descriptor->CacheEntry() == this, "### Wrong cache entry!!");
>>>>>>> upstream-releases

  *doomEntry = descriptor->ClearCacheEntry();

  PR_REMOVE_AND_INIT_LINK(descriptor);

  if (!PR_CLIST_IS_EMPTY(&mDescriptorQ))
    return true;  // stay active if we still have open descriptors

  if (PR_CLIST_IS_EMPTY(&mRequestQ))
    return false;  // no descriptors or requests, we can deactivate

  return true;  // find next best request to give a descriptor to
}

<<<<<<< HEAD
void nsCacheEntry::DetachDescriptors() {
  nsCacheEntryDescriptor *descriptor =
      (nsCacheEntryDescriptor *)PR_LIST_HEAD(&mDescriptorQ);
||||||| merged common ancestors
=======
void nsCacheEntry::DetachDescriptors() {
  nsCacheEntryDescriptor* descriptor =
      (nsCacheEntryDescriptor*)PR_LIST_HEAD(&mDescriptorQ);
>>>>>>> upstream-releases

<<<<<<< HEAD
  while (descriptor != &mDescriptorQ) {
    nsCacheEntryDescriptor *nextDescriptor =
        (nsCacheEntryDescriptor *)PR_NEXT_LINK(descriptor);
||||||| merged common ancestors
void
nsCacheEntry::DetachDescriptors()
{
    nsCacheEntryDescriptor * descriptor =
        (nsCacheEntryDescriptor *)PR_LIST_HEAD(&mDescriptorQ);

    while (descriptor != &mDescriptorQ) {
        nsCacheEntryDescriptor * nextDescriptor =
            (nsCacheEntryDescriptor *)PR_NEXT_LINK(descriptor);
=======
  while (descriptor != &mDescriptorQ) {
    nsCacheEntryDescriptor* nextDescriptor =
        (nsCacheEntryDescriptor*)PR_NEXT_LINK(descriptor);
>>>>>>> upstream-releases

    descriptor->ClearCacheEntry();
    PR_REMOVE_AND_INIT_LINK(descriptor);
    descriptor = nextDescriptor;
  }
}

<<<<<<< HEAD
void nsCacheEntry::GetDescriptors(
    nsTArray<RefPtr<nsCacheEntryDescriptor> > &outDescriptors) {
  nsCacheEntryDescriptor *descriptor =
      (nsCacheEntryDescriptor *)PR_LIST_HEAD(&mDescriptorQ);
||||||| merged common ancestors
=======
void nsCacheEntry::GetDescriptors(
    nsTArray<RefPtr<nsCacheEntryDescriptor> >& outDescriptors) {
  nsCacheEntryDescriptor* descriptor =
      (nsCacheEntryDescriptor*)PR_LIST_HEAD(&mDescriptorQ);
>>>>>>> upstream-releases

<<<<<<< HEAD
  while (descriptor != &mDescriptorQ) {
    nsCacheEntryDescriptor *nextDescriptor =
        (nsCacheEntryDescriptor *)PR_NEXT_LINK(descriptor);
||||||| merged common ancestors
void
nsCacheEntry::GetDescriptors(
    nsTArray<RefPtr<nsCacheEntryDescriptor> > &outDescriptors)
{
    nsCacheEntryDescriptor * descriptor =
        (nsCacheEntryDescriptor *)PR_LIST_HEAD(&mDescriptorQ);

    while (descriptor != &mDescriptorQ) {
        nsCacheEntryDescriptor * nextDescriptor =
            (nsCacheEntryDescriptor *)PR_NEXT_LINK(descriptor);
=======
  while (descriptor != &mDescriptorQ) {
    nsCacheEntryDescriptor* nextDescriptor =
        (nsCacheEntryDescriptor*)PR_NEXT_LINK(descriptor);
>>>>>>> upstream-releases

    outDescriptors.AppendElement(descriptor);
    descriptor = nextDescriptor;
  }
}

/******************************************************************************
 * nsCacheEntryInfo - for implementing about:cache
 *****************************************************************************/

NS_IMPL_ISUPPORTS(nsCacheEntryInfo, nsICacheEntryInfo)

NS_IMETHODIMP
<<<<<<< HEAD
nsCacheEntryInfo::GetClientID(nsACString &aClientID) {
  if (!mCacheEntry) {
    aClientID.Truncate();
    return NS_ERROR_NOT_AVAILABLE;
  }
||||||| merged common ancestors
nsCacheEntryInfo::GetClientID(nsACString& aClientID)
{
    if (!mCacheEntry) {
        aClientID.Truncate();
        return NS_ERROR_NOT_AVAILABLE;
    }
=======
nsCacheEntryInfo::GetClientID(nsACString& aClientID) {
  if (!mCacheEntry) {
    aClientID.Truncate();
    return NS_ERROR_NOT_AVAILABLE;
  }
>>>>>>> upstream-releases

  return ClientIDFromCacheKey(*mCacheEntry->Key(), aClientID);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsCacheEntryInfo::GetDeviceID(nsACString &aDeviceID) {
  if (!mCacheEntry) {
    aDeviceID.Truncate();
    return NS_ERROR_NOT_AVAILABLE;
  }
||||||| merged common ancestors
nsCacheEntryInfo::GetDeviceID(nsACString& aDeviceID)
{
    if (!mCacheEntry) {
        aDeviceID.Truncate();
        return NS_ERROR_NOT_AVAILABLE;
    }
=======
nsCacheEntryInfo::GetDeviceID(nsACString& aDeviceID) {
  if (!mCacheEntry) {
    aDeviceID.Truncate();
    return NS_ERROR_NOT_AVAILABLE;
  }
>>>>>>> upstream-releases

  aDeviceID.Assign(mCacheEntry->GetDeviceID());
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsCacheEntryInfo::GetKey(nsACString &key) {
  if (!mCacheEntry) return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsCacheEntryInfo::GetKey(nsACString &key)
{
    if (!mCacheEntry)  return NS_ERROR_NOT_AVAILABLE;
=======
nsCacheEntryInfo::GetKey(nsACString& key) {
  if (!mCacheEntry) return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases

  return ClientKeyFromCacheKey(*mCacheEntry->Key(), key);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsCacheEntryInfo::GetFetchCount(int32_t *fetchCount) {
  NS_ENSURE_ARG_POINTER(fetchCount);
  if (!mCacheEntry) return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsCacheEntryInfo::GetFetchCount(int32_t * fetchCount)
{
    NS_ENSURE_ARG_POINTER(fetchCount);
    if (!mCacheEntry)  return NS_ERROR_NOT_AVAILABLE;
=======
nsCacheEntryInfo::GetFetchCount(int32_t* fetchCount) {
  NS_ENSURE_ARG_POINTER(fetchCount);
  if (!mCacheEntry) return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases

  *fetchCount = mCacheEntry->FetchCount();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsCacheEntryInfo::GetLastFetched(uint32_t *lastFetched) {
  NS_ENSURE_ARG_POINTER(lastFetched);
  if (!mCacheEntry) return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsCacheEntryInfo::GetLastFetched(uint32_t * lastFetched)
{
    NS_ENSURE_ARG_POINTER(lastFetched);
    if (!mCacheEntry)  return NS_ERROR_NOT_AVAILABLE;
=======
nsCacheEntryInfo::GetLastFetched(uint32_t* lastFetched) {
  NS_ENSURE_ARG_POINTER(lastFetched);
  if (!mCacheEntry) return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases

  *lastFetched = mCacheEntry->LastFetched();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsCacheEntryInfo::GetLastModified(uint32_t *lastModified) {
  NS_ENSURE_ARG_POINTER(lastModified);
  if (!mCacheEntry) return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsCacheEntryInfo::GetLastModified(uint32_t * lastModified)
{
    NS_ENSURE_ARG_POINTER(lastModified);
    if (!mCacheEntry)  return NS_ERROR_NOT_AVAILABLE;
=======
nsCacheEntryInfo::GetLastModified(uint32_t* lastModified) {
  NS_ENSURE_ARG_POINTER(lastModified);
  if (!mCacheEntry) return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases

  *lastModified = mCacheEntry->LastModified();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsCacheEntryInfo::GetExpirationTime(uint32_t *expirationTime) {
  NS_ENSURE_ARG_POINTER(expirationTime);
  if (!mCacheEntry) return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsCacheEntryInfo::GetExpirationTime(uint32_t * expirationTime)
{
    NS_ENSURE_ARG_POINTER(expirationTime);
    if (!mCacheEntry)  return NS_ERROR_NOT_AVAILABLE;
=======
nsCacheEntryInfo::GetExpirationTime(uint32_t* expirationTime) {
  NS_ENSURE_ARG_POINTER(expirationTime);
  if (!mCacheEntry) return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases

  *expirationTime = mCacheEntry->ExpirationTime();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsCacheEntryInfo::GetDataSize(uint32_t *dataSize) {
  NS_ENSURE_ARG_POINTER(dataSize);
  if (!mCacheEntry) return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsCacheEntryInfo::GetDataSize(uint32_t * dataSize)
{
    NS_ENSURE_ARG_POINTER(dataSize);
    if (!mCacheEntry)  return NS_ERROR_NOT_AVAILABLE;
=======
nsCacheEntryInfo::GetDataSize(uint32_t* dataSize) {
  NS_ENSURE_ARG_POINTER(dataSize);
  if (!mCacheEntry) return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases

  *dataSize = mCacheEntry->DataSize();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsCacheEntryInfo::IsStreamBased(bool *result) {
  NS_ENSURE_ARG_POINTER(result);
  if (!mCacheEntry) return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsCacheEntryInfo::IsStreamBased(bool * result)
{
    NS_ENSURE_ARG_POINTER(result);
    if (!mCacheEntry)  return NS_ERROR_NOT_AVAILABLE;
=======
nsCacheEntryInfo::IsStreamBased(bool* result) {
  NS_ENSURE_ARG_POINTER(result);
  if (!mCacheEntry) return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases

  *result = mCacheEntry->IsStreamData();
  return NS_OK;
}

/******************************************************************************
 *  nsCacheEntryHashTable
 *****************************************************************************/

const PLDHashTableOps nsCacheEntryHashTable::ops = {HashKey, MatchEntry,
                                                    MoveEntry, ClearEntry};

nsCacheEntryHashTable::nsCacheEntryHashTable()
    : table(&ops, sizeof(nsCacheEntryHashTableEntry), kInitialTableLength),
      initialized(false) {
  MOZ_COUNT_CTOR(nsCacheEntryHashTable);
}

nsCacheEntryHashTable::~nsCacheEntryHashTable() {
  MOZ_COUNT_DTOR(nsCacheEntryHashTable);
  if (initialized) Shutdown();
}

void nsCacheEntryHashTable::Init() {
  table.ClearAndPrepareForLength(kInitialTableLength);
  initialized = true;
}

void nsCacheEntryHashTable::Shutdown() {
  if (initialized) {
    table.ClearAndPrepareForLength(kInitialTableLength);
    initialized = false;
  }
}

<<<<<<< HEAD
nsCacheEntry *nsCacheEntryHashTable::GetEntry(const nsCString *key) const {
  NS_ASSERTION(initialized, "nsCacheEntryHashTable not initialized");
  if (!initialized) return nullptr;
||||||| merged common ancestors
=======
nsCacheEntry* nsCacheEntryHashTable::GetEntry(const nsCString* key) const {
  NS_ASSERTION(initialized, "nsCacheEntryHashTable not initialized");
  if (!initialized) return nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  PLDHashEntryHdr *hashEntry = table.Search(key);
  return hashEntry ? ((nsCacheEntryHashTableEntry *)hashEntry)->cacheEntry
                   : nullptr;
||||||| merged common ancestors
nsCacheEntry *
nsCacheEntryHashTable::GetEntry( const nsCString * key) const
{
    NS_ASSERTION(initialized, "nsCacheEntryHashTable not initialized");
    if (!initialized)  return nullptr;

    PLDHashEntryHdr *hashEntry = table.Search(key);
    return hashEntry ? ((nsCacheEntryHashTableEntry *)hashEntry)->cacheEntry
                     : nullptr;
=======
  PLDHashEntryHdr* hashEntry = table.Search(key);
  return hashEntry ? ((nsCacheEntryHashTableEntry*)hashEntry)->cacheEntry
                   : nullptr;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsCacheEntryHashTable::AddEntry(nsCacheEntry *cacheEntry) {
  PLDHashEntryHdr *hashEntry;
||||||| merged common ancestors
=======
nsresult nsCacheEntryHashTable::AddEntry(nsCacheEntry* cacheEntry) {
  PLDHashEntryHdr* hashEntry;
>>>>>>> upstream-releases

  NS_ASSERTION(initialized, "nsCacheEntryHashTable not initialized");
  if (!initialized) return NS_ERROR_NOT_INITIALIZED;
  if (!cacheEntry) return NS_ERROR_NULL_POINTER;

  hashEntry = table.Add(&(cacheEntry->mKey), fallible);

<<<<<<< HEAD
  if (!hashEntry) return NS_ERROR_FAILURE;
  NS_ASSERTION(((nsCacheEntryHashTableEntry *)hashEntry)->cacheEntry == nullptr,
               "### nsCacheEntryHashTable::AddEntry - entry already used");
  ((nsCacheEntryHashTableEntry *)hashEntry)->cacheEntry = cacheEntry;
||||||| merged common ancestors
    if (!hashEntry)
        return NS_ERROR_FAILURE;
    NS_ASSERTION(((nsCacheEntryHashTableEntry *)hashEntry)->cacheEntry == nullptr,
                 "### nsCacheEntryHashTable::AddEntry - entry already used");
    ((nsCacheEntryHashTableEntry *)hashEntry)->cacheEntry = cacheEntry;
=======
  if (!hashEntry) return NS_ERROR_FAILURE;
  NS_ASSERTION(((nsCacheEntryHashTableEntry*)hashEntry)->cacheEntry == nullptr,
               "### nsCacheEntryHashTable::AddEntry - entry already used");
  ((nsCacheEntryHashTableEntry*)hashEntry)->cacheEntry = cacheEntry;
>>>>>>> upstream-releases

  return NS_OK;
}

<<<<<<< HEAD
void nsCacheEntryHashTable::RemoveEntry(nsCacheEntry *cacheEntry) {
  NS_ASSERTION(initialized, "nsCacheEntryHashTable not initialized");
  NS_ASSERTION(cacheEntry, "### cacheEntry == nullptr");
||||||| merged common ancestors
=======
void nsCacheEntryHashTable::RemoveEntry(nsCacheEntry* cacheEntry) {
  NS_ASSERTION(initialized, "nsCacheEntryHashTable not initialized");
  NS_ASSERTION(cacheEntry, "### cacheEntry == nullptr");
>>>>>>> upstream-releases

  if (!initialized) return;  // NS_ERROR_NOT_INITIALIZED

#if DEBUG
<<<<<<< HEAD
  // XXX debug code to make sure we have the entry we're trying to remove
  nsCacheEntry *check = GetEntry(&(cacheEntry->mKey));
  NS_ASSERTION(check == cacheEntry,
               "### Attempting to remove unknown cache entry!!!");
||||||| merged common ancestors
    // XXX debug code to make sure we have the entry we're trying to remove
    nsCacheEntry *check = GetEntry(&(cacheEntry->mKey));
    NS_ASSERTION(check == cacheEntry, "### Attempting to remove unknown cache entry!!!");
=======
  // XXX debug code to make sure we have the entry we're trying to remove
  nsCacheEntry* check = GetEntry(&(cacheEntry->mKey));
  NS_ASSERTION(check == cacheEntry,
               "### Attempting to remove unknown cache entry!!!");
>>>>>>> upstream-releases
#endif
  table.Remove(&(cacheEntry->mKey));
}

PLDHashTable::Iterator nsCacheEntryHashTable::Iter() {
  return PLDHashTable::Iterator(&table);
}

/**
 *  hash table operation callback functions
 */

<<<<<<< HEAD
PLDHashNumber nsCacheEntryHashTable::HashKey(const void *key) {
  return HashString(*static_cast<const nsCString *>(key));
||||||| merged common ancestors
PLDHashNumber
nsCacheEntryHashTable::HashKey(const void *key)
{
    return HashString(*static_cast<const nsCString *>(key));
=======
PLDHashNumber nsCacheEntryHashTable::HashKey(const void* key) {
  return HashString(*static_cast<const nsCString*>(key));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool nsCacheEntryHashTable::MatchEntry(const PLDHashEntryHdr *hashEntry,
                                       const void *key) {
  NS_ASSERTION(key != nullptr,
               "### nsCacheEntryHashTable::MatchEntry : null key");
  nsCacheEntry *cacheEntry =
      ((nsCacheEntryHashTableEntry *)hashEntry)->cacheEntry;
||||||| merged common ancestors
bool
nsCacheEntryHashTable::MatchEntry(const PLDHashEntryHdr * hashEntry,
                                  const void *            key)
{
    NS_ASSERTION(key !=  nullptr, "### nsCacheEntryHashTable::MatchEntry : null key");
    nsCacheEntry *cacheEntry = ((nsCacheEntryHashTableEntry *)hashEntry)->cacheEntry;
=======
bool nsCacheEntryHashTable::MatchEntry(const PLDHashEntryHdr* hashEntry,
                                       const void* key) {
  NS_ASSERTION(key != nullptr,
               "### nsCacheEntryHashTable::MatchEntry : null key");
  nsCacheEntry* cacheEntry =
      ((nsCacheEntryHashTableEntry*)hashEntry)->cacheEntry;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return cacheEntry->mKey.Equals(*(nsCString *)key);
||||||| merged common ancestors
    return cacheEntry->mKey.Equals(*(nsCString *)key);
=======
  return cacheEntry->mKey.Equals(*(nsCString*)key);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsCacheEntryHashTable::MoveEntry(PLDHashTable * /* table */,
                                      const PLDHashEntryHdr *from,
                                      PLDHashEntryHdr *to) {
  new (KnownNotNull, to) nsCacheEntryHashTableEntry(
      std::move(*((nsCacheEntryHashTableEntry *)from)));
  // No need to destroy `from`.
||||||| merged common ancestors

void
nsCacheEntryHashTable::MoveEntry(PLDHashTable * /* table */,
                                 const PLDHashEntryHdr *from,
                                 PLDHashEntryHdr       *to)
{
    new (KnownNotNull, to) nsCacheEntryHashTableEntry(std::move(*((nsCacheEntryHashTableEntry *)from)));
    // No need to destroy `from`.
=======
void nsCacheEntryHashTable::MoveEntry(PLDHashTable* /* table */,
                                      const PLDHashEntryHdr* from,
                                      PLDHashEntryHdr* to) {
  new (KnownNotNull, to) nsCacheEntryHashTableEntry(
      std::move(*((nsCacheEntryHashTableEntry*)from)));
  // No need to destroy `from`.
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsCacheEntryHashTable::ClearEntry(PLDHashTable * /* table */,
                                       PLDHashEntryHdr *hashEntry) {
  ((nsCacheEntryHashTableEntry *)hashEntry)->cacheEntry = nullptr;
||||||| merged common ancestors

void
nsCacheEntryHashTable::ClearEntry(PLDHashTable * /* table */,
                                  PLDHashEntryHdr * hashEntry)
{
    ((nsCacheEntryHashTableEntry *)hashEntry)->cacheEntry = nullptr;
=======
void nsCacheEntryHashTable::ClearEntry(PLDHashTable* /* table */,
                                       PLDHashEntryHdr* hashEntry) {
  ((nsCacheEntryHashTableEntry*)hashEntry)->cacheEntry = nullptr;
>>>>>>> upstream-releases
}
