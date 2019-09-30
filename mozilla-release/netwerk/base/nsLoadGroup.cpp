/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=4 sts=2 et cin: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/DebugOnly.h"

#include "nsLoadGroup.h"

#include "nsArrayEnumerator.h"
#include "nsCOMArray.h"
#include "nsCOMPtr.h"
#include "mozilla/Logging.h"
#include "nsString.h"
#include "nsTArray.h"
#include "mozilla/Telemetry.h"
#include "nsITimedChannel.h"
#include "nsIInterfaceRequestor.h"
#include "nsIRequestObserver.h"
#include "nsIRequestContext.h"
#include "CacheObserver.h"
#include "MainThreadUtils.h"
#include "RequestContextService.h"
#include "mozilla/Unused.h"

namespace mozilla {
namespace net {

//
// Log module for nsILoadGroup logging...
//
// To enable logging (see prlog.h for full details):
//
//    set MOZ_LOG=LoadGroup:5
//    set MOZ_LOG_FILE=network.log
//
// This enables LogLevel::Debug level information and places all output in
// the file network.log.
//
static LazyLogModule gLoadGroupLog("LoadGroup");
#undef LOG
#define LOG(args) MOZ_LOG(gLoadGroupLog, mozilla::LogLevel::Debug, args)

////////////////////////////////////////////////////////////////////////////////

<<<<<<< HEAD
class RequestMapEntry : public PLDHashEntryHdr {
 public:
  explicit RequestMapEntry(nsIRequest *aRequest) : mKey(aRequest) {}
||||||| merged common ancestors
class RequestMapEntry : public PLDHashEntryHdr
{
public:
    explicit RequestMapEntry(nsIRequest *aRequest) :
        mKey(aRequest)
    {
    }
=======
class RequestMapEntry : public PLDHashEntryHdr {
 public:
  explicit RequestMapEntry(nsIRequest* aRequest) : mKey(aRequest) {}
>>>>>>> upstream-releases

  nsCOMPtr<nsIRequest> mKey;
};

<<<<<<< HEAD
static bool RequestHashMatchEntry(const PLDHashEntryHdr *entry,
                                  const void *key) {
  const RequestMapEntry *e = static_cast<const RequestMapEntry *>(entry);
  const nsIRequest *request = static_cast<const nsIRequest *>(key);
||||||| merged common ancestors
static bool
RequestHashMatchEntry(const PLDHashEntryHdr *entry, const void *key)
{
    const RequestMapEntry *e =
        static_cast<const RequestMapEntry *>(entry);
    const nsIRequest *request = static_cast<const nsIRequest *>(key);
=======
static bool RequestHashMatchEntry(const PLDHashEntryHdr* entry,
                                  const void* key) {
  const RequestMapEntry* e = static_cast<const RequestMapEntry*>(entry);
  const nsIRequest* request = static_cast<const nsIRequest*>(key);
>>>>>>> upstream-releases

  return e->mKey == request;
}

<<<<<<< HEAD
static void RequestHashClearEntry(PLDHashTable *table, PLDHashEntryHdr *entry) {
  RequestMapEntry *e = static_cast<RequestMapEntry *>(entry);
||||||| merged common ancestors
static void
RequestHashClearEntry(PLDHashTable *table, PLDHashEntryHdr *entry)
{
    RequestMapEntry *e = static_cast<RequestMapEntry *>(entry);
=======
static void RequestHashClearEntry(PLDHashTable* table, PLDHashEntryHdr* entry) {
  RequestMapEntry* e = static_cast<RequestMapEntry*>(entry);
>>>>>>> upstream-releases

  // An entry is being cleared, let the entry do its own cleanup.
  e->~RequestMapEntry();
}

<<<<<<< HEAD
static void RequestHashInitEntry(PLDHashEntryHdr *entry, const void *key) {
  const nsIRequest *const_request = static_cast<const nsIRequest *>(key);
  nsIRequest *request = const_cast<nsIRequest *>(const_request);
||||||| merged common ancestors
static void
RequestHashInitEntry(PLDHashEntryHdr *entry, const void *key)
{
    const nsIRequest *const_request = static_cast<const nsIRequest *>(key);
    nsIRequest *request = const_cast<nsIRequest *>(const_request);
=======
static void RequestHashInitEntry(PLDHashEntryHdr* entry, const void* key) {
  const nsIRequest* const_request = static_cast<const nsIRequest*>(key);
  nsIRequest* request = const_cast<nsIRequest*>(const_request);
>>>>>>> upstream-releases

  // Initialize the entry with placement new
  new (entry) RequestMapEntry(request);
}

static const PLDHashTableOps sRequestHashOps = {
    PLDHashTable::HashVoidPtrKeyStub, RequestHashMatchEntry,
    PLDHashTable::MoveEntryStub, RequestHashClearEntry, RequestHashInitEntry};

<<<<<<< HEAD
static void RescheduleRequest(nsIRequest *aRequest, int32_t delta) {
  nsCOMPtr<nsISupportsPriority> p = do_QueryInterface(aRequest);
  if (p) p->AdjustPriority(delta);
||||||| merged common ancestors
static void
RescheduleRequest(nsIRequest *aRequest, int32_t delta)
{
    nsCOMPtr<nsISupportsPriority> p = do_QueryInterface(aRequest);
    if (p)
        p->AdjustPriority(delta);
=======
static void RescheduleRequest(nsIRequest* aRequest, int32_t delta) {
  nsCOMPtr<nsISupportsPriority> p = do_QueryInterface(aRequest);
  if (p) p->AdjustPriority(delta);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsLoadGroup::nsLoadGroup(nsISupports *outer)
    : mForegroundCount(0),
      mLoadFlags(LOAD_NORMAL),
      mDefaultLoadFlags(0),
      mRequests(&sRequestHashOps, sizeof(RequestMapEntry)),
      mStatus(NS_OK),
      mPriority(PRIORITY_NORMAL),
      mIsCanceling(false),
      mDefaultLoadIsTimed(false),
      mTimedRequests(0),
      mCachedRequests(0),
      mTimedNonCachedRequestsUntilOnEndPageLoad(0) {
  NS_INIT_AGGREGATED(outer);
  LOG(("LOADGROUP [%p]: Created.\n", this));
||||||| merged common ancestors
nsLoadGroup::nsLoadGroup(nsISupports* outer)
    : mForegroundCount(0)
    , mLoadFlags(LOAD_NORMAL)
    , mDefaultLoadFlags(0)
    , mRequests(&sRequestHashOps, sizeof(RequestMapEntry))
    , mStatus(NS_OK)
    , mPriority(PRIORITY_NORMAL)
    , mIsCanceling(false)
    , mDefaultLoadIsTimed(false)
    , mTimedRequests(0)
    , mCachedRequests(0)
    , mTimedNonCachedRequestsUntilOnEndPageLoad(0)
{
    NS_INIT_AGGREGATED(outer);
    LOG(("LOADGROUP [%p]: Created.\n", this));
=======
nsLoadGroup::nsLoadGroup()
    : mForegroundCount(0),
      mLoadFlags(LOAD_NORMAL),
      mDefaultLoadFlags(0),
      mPriority(PRIORITY_NORMAL),
      mRequests(&sRequestHashOps, sizeof(RequestMapEntry)),
      mStatus(NS_OK),
      mIsCanceling(false),
      mDefaultLoadIsTimed(false),
      mTimedRequests(0),
      mCachedRequests(0) {
  LOG(("LOADGROUP [%p]: Created.\n", this));
>>>>>>> upstream-releases
}

nsLoadGroup::~nsLoadGroup() {
  DebugOnly<nsresult> rv = Cancel(NS_BINDING_ABORTED);
  NS_ASSERTION(NS_SUCCEEDED(rv), "Cancel failed");

  mDefaultLoadRequest = nullptr;

  if (mRequestContext) {
    mRequestContextService->RemoveRequestContext(mRequestContext->GetID());
  }

  LOG(("LOADGROUP [%p]: Destroyed.\n", this));
}

////////////////////////////////////////////////////////////////////////////////
// nsISupports methods:

<<<<<<< HEAD
NS_IMPL_AGGREGATED(nsLoadGroup)
NS_INTERFACE_MAP_BEGIN_AGGREGATED(nsLoadGroup)
  NS_INTERFACE_MAP_ENTRY(nsILoadGroup)
  NS_INTERFACE_MAP_ENTRY(nsPILoadGroupInternal)
  NS_INTERFACE_MAP_ENTRY(nsILoadGroupChild)
  NS_INTERFACE_MAP_ENTRY(nsIRequest)
  NS_INTERFACE_MAP_ENTRY(nsISupportsPriority)
  NS_INTERFACE_MAP_ENTRY(nsISupportsWeakReference)
NS_INTERFACE_MAP_END
||||||| merged common ancestors
NS_IMPL_AGGREGATED(nsLoadGroup)
NS_INTERFACE_MAP_BEGIN_AGGREGATED(nsLoadGroup)
    NS_INTERFACE_MAP_ENTRY(nsILoadGroup)
    NS_INTERFACE_MAP_ENTRY(nsPILoadGroupInternal)
    NS_INTERFACE_MAP_ENTRY(nsILoadGroupChild)
    NS_INTERFACE_MAP_ENTRY(nsIRequest)
    NS_INTERFACE_MAP_ENTRY(nsISupportsPriority)
    NS_INTERFACE_MAP_ENTRY(nsISupportsWeakReference)
NS_INTERFACE_MAP_END
=======
NS_IMPL_ISUPPORTS(nsLoadGroup, nsILoadGroup, nsILoadGroupChild, nsIRequest,
                  nsISupportsPriority, nsISupportsWeakReference)
>>>>>>> upstream-releases

////////////////////////////////////////////////////////////////////////////////
// nsIRequest methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetName(nsACString &result) {
  // XXX is this the right "name" for a load group?
||||||| merged common ancestors
nsLoadGroup::GetName(nsACString &result)
{
    // XXX is this the right "name" for a load group?
=======
nsLoadGroup::GetName(nsACString& result) {
  // XXX is this the right "name" for a load group?
>>>>>>> upstream-releases

  if (!mDefaultLoadRequest) {
    result.Truncate();
    return NS_OK;
  }

  return mDefaultLoadRequest->GetName(result);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::IsPending(bool *aResult) {
  *aResult = (mForegroundCount > 0) ? true : false;
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::IsPending(bool *aResult)
{
    *aResult = (mForegroundCount > 0) ? true : false;
    return NS_OK;
=======
nsLoadGroup::IsPending(bool* aResult) {
  *aResult = (mForegroundCount > 0) ? true : false;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetStatus(nsresult *status) {
  if (NS_SUCCEEDED(mStatus) && mDefaultLoadRequest)
    return mDefaultLoadRequest->GetStatus(status);
||||||| merged common ancestors
nsLoadGroup::GetStatus(nsresult *status)
{
    if (NS_SUCCEEDED(mStatus) && mDefaultLoadRequest)
        return mDefaultLoadRequest->GetStatus(status);
=======
nsLoadGroup::GetStatus(nsresult* status) {
  if (NS_SUCCEEDED(mStatus) && mDefaultLoadRequest)
    return mDefaultLoadRequest->GetStatus(status);
>>>>>>> upstream-releases

  *status = mStatus;
  return NS_OK;
}

<<<<<<< HEAD
static bool AppendRequestsToArray(PLDHashTable *aTable,
                                  nsTArray<nsIRequest *> *aArray) {
  for (auto iter = aTable->Iter(); !iter.Done(); iter.Next()) {
    auto e = static_cast<RequestMapEntry *>(iter.Get());
    nsIRequest *request = e->mKey;
    NS_ASSERTION(request, "What? Null key in PLDHashTable entry?");

    bool ok = !!aArray->AppendElement(request);
    if (!ok) {
      break;
||||||| merged common ancestors
static bool
AppendRequestsToArray(PLDHashTable* aTable, nsTArray<nsIRequest*> *aArray)
{
    for (auto iter = aTable->Iter(); !iter.Done(); iter.Next()) {
        auto e = static_cast<RequestMapEntry*>(iter.Get());
        nsIRequest *request = e->mKey;
        NS_ASSERTION(request, "What? Null key in PLDHashTable entry?");

        bool ok = !!aArray->AppendElement(request);
        if (!ok) {
           break;
        }
        NS_ADDREF(request);
=======
static bool AppendRequestsToArray(PLDHashTable* aTable,
                                  nsTArray<nsIRequest*>* aArray) {
  for (auto iter = aTable->Iter(); !iter.Done(); iter.Next()) {
    auto e = static_cast<RequestMapEntry*>(iter.Get());
    nsIRequest* request = e->mKey;
    NS_ASSERTION(request, "What? Null key in PLDHashTable entry?");

    bool ok = !!aArray->AppendElement(request);
    if (!ok) {
      break;
>>>>>>> upstream-releases
    }
    NS_ADDREF(request);
  }

  if (aArray->Length() != aTable->EntryCount()) {
    for (uint32_t i = 0, len = aArray->Length(); i < len; ++i) {
      NS_RELEASE((*aArray)[i]);
    }
    return false;
  }
  return true;
}

NS_IMETHODIMP
nsLoadGroup::Cancel(nsresult status) {
  MOZ_ASSERT(NS_IsMainThread());

  NS_ASSERTION(NS_FAILED(status), "shouldn't cancel with a success code");
  nsresult rv;
  uint32_t count = mRequests.EntryCount();

<<<<<<< HEAD
  AutoTArray<nsIRequest *, 8> requests;
||||||| merged common ancestors
    AutoTArray<nsIRequest*, 8> requests;
=======
  AutoTArray<nsIRequest*, 8> requests;
>>>>>>> upstream-releases

  if (!AppendRequestsToArray(&mRequests, &requests)) {
    return NS_ERROR_OUT_OF_MEMORY;
  }

  // set the load group status to our cancel status while we cancel
  // all our requests...once the cancel is done, we'll reset it...
  //
  mStatus = status;

  // Set the flag indicating that the loadgroup is being canceled...  This
  // prevents any new channels from being added during the operation.
  //
  mIsCanceling = true;

  nsresult firstError = NS_OK;

<<<<<<< HEAD
  while (count > 0) {
    nsIRequest *request = requests.ElementAt(--count);
||||||| merged common ancestors
    while (count > 0) {
        nsIRequest* request = requests.ElementAt(--count);
=======
  while (count > 0) {
    nsCOMPtr<nsIRequest> request = dont_AddRef(requests.ElementAt(--count));
>>>>>>> upstream-releases

    NS_ASSERTION(request, "NULL request found in list.");

<<<<<<< HEAD
    if (!mRequests.Search(request)) {
      // |request| was removed already
      NS_RELEASE(request);
      continue;
    }

    if (MOZ_LOG_TEST(gLoadGroupLog, LogLevel::Debug)) {
      nsAutoCString nameStr;
      request->GetName(nameStr);
      LOG(("LOADGROUP [%p]: Canceling request %p %s.\n", this, request,
           nameStr.get()));
    }
||||||| merged common ancestors
        if (!mRequests.Search(request)) {
            // |request| was removed already
            NS_RELEASE(request);
            continue;
        }

        if (MOZ_LOG_TEST(gLoadGroupLog, LogLevel::Debug)) {
            nsAutoCString nameStr;
            request->GetName(nameStr);
            LOG(("LOADGROUP [%p]: Canceling request %p %s.\n",
                 this, request, nameStr.get()));
        }
=======
    if (!mRequests.Search(request)) {
      // |request| was removed already
      continue;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    //
    // Remove the request from the load group...  This may cause
    // the OnStopRequest notification to fire...
    //
    // XXX: What should the context be?
    //
    (void)RemoveRequest(request, nullptr, status);
||||||| merged common ancestors
        //
        // Remove the request from the load group...  This may cause
        // the OnStopRequest notification to fire...
        //
        // XXX: What should the context be?
        //
        (void)RemoveRequest(request, nullptr, status);
=======
    if (MOZ_LOG_TEST(gLoadGroupLog, LogLevel::Debug)) {
      nsAutoCString nameStr;
      request->GetName(nameStr);
      LOG(("LOADGROUP [%p]: Canceling request %p %s.\n", this, request.get(),
           nameStr.get()));
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Cancel the request...
    rv = request->Cancel(status);
||||||| merged common ancestors
        // Cancel the request...
        rv = request->Cancel(status);
=======
    //
    // Remove the request from the load group...  This may cause
    // the OnStopRequest notification to fire...
    //
    // XXX: What should the context be?
    //
    (void)RemoveRequest(request, nullptr, status);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Remember the first failure and return it...
    if (NS_FAILED(rv) && NS_SUCCEEDED(firstError)) firstError = rv;
||||||| merged common ancestors
        // Remember the first failure and return it...
        if (NS_FAILED(rv) && NS_SUCCEEDED(firstError))
            firstError = rv;
=======
    // Cancel the request...
    rv = request->Cancel(status);
>>>>>>> upstream-releases

<<<<<<< HEAD
    NS_RELEASE(request);
  }
||||||| merged common ancestors
        NS_RELEASE(request);
    }
=======
    // Remember the first failure and return it...
    if (NS_FAILED(rv) && NS_SUCCEEDED(firstError)) firstError = rv;
  }
>>>>>>> upstream-releases

  if (mRequestContext) {
    Unused << mRequestContext->CancelTailPendingRequests(status);
  }

#if defined(DEBUG)
  NS_ASSERTION(mRequests.EntryCount() == 0, "Request list is not empty.");
  NS_ASSERTION(mForegroundCount == 0, "Foreground URLs are active.");
#endif

  mStatus = NS_OK;
  mIsCanceling = false;

  return firstError;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::Suspend() {
  nsresult rv, firstError;
  uint32_t count = mRequests.EntryCount();

  AutoTArray<nsIRequest *, 8> requests;
||||||| merged common ancestors
nsLoadGroup::Suspend()
{
    nsresult rv, firstError;
    uint32_t count = mRequests.EntryCount();

    AutoTArray<nsIRequest*, 8> requests;
=======
nsLoadGroup::Suspend() {
  nsresult rv, firstError;
  uint32_t count = mRequests.EntryCount();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!AppendRequestsToArray(&mRequests, &requests)) {
    return NS_ERROR_OUT_OF_MEMORY;
  }
||||||| merged common ancestors
    if (!AppendRequestsToArray(&mRequests, &requests)) {
        return NS_ERROR_OUT_OF_MEMORY;
    }
=======
  AutoTArray<nsIRequest*, 8> requests;
>>>>>>> upstream-releases

<<<<<<< HEAD
  firstError = NS_OK;
  //
  // Operate the elements from back to front so that if items get
  // get removed from the list it won't affect our iteration
  //
  while (count > 0) {
    nsIRequest *request = requests.ElementAt(--count);
||||||| merged common ancestors
    firstError = NS_OK;
    //
    // Operate the elements from back to front so that if items get
    // get removed from the list it won't affect our iteration
    //
    while (count > 0) {
        nsIRequest* request = requests.ElementAt(--count);
=======
  if (!AppendRequestsToArray(&mRequests, &requests)) {
    return NS_ERROR_OUT_OF_MEMORY;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    NS_ASSERTION(request, "NULL request found in list.");
    if (!request) continue;
||||||| merged common ancestors
        NS_ASSERTION(request, "NULL request found in list.");
        if (!request)
            continue;
=======
  firstError = NS_OK;
  //
  // Operate the elements from back to front so that if items get
  // get removed from the list it won't affect our iteration
  //
  while (count > 0) {
    nsCOMPtr<nsIRequest> request = dont_AddRef(requests.ElementAt(--count));
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (MOZ_LOG_TEST(gLoadGroupLog, LogLevel::Debug)) {
      nsAutoCString nameStr;
      request->GetName(nameStr);
      LOG(("LOADGROUP [%p]: Suspending request %p %s.\n", this, request,
           nameStr.get()));
    }
||||||| merged common ancestors
        if (MOZ_LOG_TEST(gLoadGroupLog, LogLevel::Debug)) {
            nsAutoCString nameStr;
            request->GetName(nameStr);
            LOG(("LOADGROUP [%p]: Suspending request %p %s.\n",
                this, request, nameStr.get()));
        }
=======
    NS_ASSERTION(request, "NULL request found in list.");
    if (!request) continue;
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Suspend the request...
    rv = request->Suspend();
||||||| merged common ancestors
        // Suspend the request...
        rv = request->Suspend();
=======
    if (MOZ_LOG_TEST(gLoadGroupLog, LogLevel::Debug)) {
      nsAutoCString nameStr;
      request->GetName(nameStr);
      LOG(("LOADGROUP [%p]: Suspending request %p %s.\n", this, request.get(),
           nameStr.get()));
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Remember the first failure and return it...
    if (NS_FAILED(rv) && NS_SUCCEEDED(firstError)) firstError = rv;
||||||| merged common ancestors
        // Remember the first failure and return it...
        if (NS_FAILED(rv) && NS_SUCCEEDED(firstError))
            firstError = rv;
=======
    // Suspend the request...
    rv = request->Suspend();
>>>>>>> upstream-releases

<<<<<<< HEAD
    NS_RELEASE(request);
  }
||||||| merged common ancestors
        NS_RELEASE(request);
    }
=======
    // Remember the first failure and return it...
    if (NS_FAILED(rv) && NS_SUCCEEDED(firstError)) firstError = rv;
  }
>>>>>>> upstream-releases

  return firstError;
}

NS_IMETHODIMP
nsLoadGroup::Resume() {
  nsresult rv, firstError;
  uint32_t count = mRequests.EntryCount();

<<<<<<< HEAD
  AutoTArray<nsIRequest *, 8> requests;
||||||| merged common ancestors
    AutoTArray<nsIRequest*, 8> requests;
=======
  AutoTArray<nsIRequest*, 8> requests;
>>>>>>> upstream-releases

  if (!AppendRequestsToArray(&mRequests, &requests)) {
    return NS_ERROR_OUT_OF_MEMORY;
  }

<<<<<<< HEAD
  firstError = NS_OK;
  //
  // Operate the elements from back to front so that if items get
  // get removed from the list it won't affect our iteration
  //
  while (count > 0) {
    nsIRequest *request = requests.ElementAt(--count);
||||||| merged common ancestors
    firstError = NS_OK;
    //
    // Operate the elements from back to front so that if items get
    // get removed from the list it won't affect our iteration
    //
    while (count > 0) {
        nsIRequest* request = requests.ElementAt(--count);
=======
  firstError = NS_OK;
  //
  // Operate the elements from back to front so that if items get
  // get removed from the list it won't affect our iteration
  //
  while (count > 0) {
    nsCOMPtr<nsIRequest> request = dont_AddRef(requests.ElementAt(--count));
>>>>>>> upstream-releases

    NS_ASSERTION(request, "NULL request found in list.");
    if (!request) continue;

<<<<<<< HEAD
    if (MOZ_LOG_TEST(gLoadGroupLog, LogLevel::Debug)) {
      nsAutoCString nameStr;
      request->GetName(nameStr);
      LOG(("LOADGROUP [%p]: Resuming request %p %s.\n", this, request,
           nameStr.get()));
    }
||||||| merged common ancestors
        if (MOZ_LOG_TEST(gLoadGroupLog, LogLevel::Debug)) {
            nsAutoCString nameStr;
            request->GetName(nameStr);
            LOG(("LOADGROUP [%p]: Resuming request %p %s.\n",
                this, request, nameStr.get()));
        }
=======
    if (MOZ_LOG_TEST(gLoadGroupLog, LogLevel::Debug)) {
      nsAutoCString nameStr;
      request->GetName(nameStr);
      LOG(("LOADGROUP [%p]: Resuming request %p %s.\n", this, request.get(),
           nameStr.get()));
    }
>>>>>>> upstream-releases

    // Resume the request...
    rv = request->Resume();

<<<<<<< HEAD
    // Remember the first failure and return it...
    if (NS_FAILED(rv) && NS_SUCCEEDED(firstError)) firstError = rv;
||||||| merged common ancestors
        // Remember the first failure and return it...
        if (NS_FAILED(rv) && NS_SUCCEEDED(firstError))
            firstError = rv;
=======
    // Remember the first failure and return it...
    if (NS_FAILED(rv) && NS_SUCCEEDED(firstError)) firstError = rv;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    NS_RELEASE(request);
  }

  return firstError;
||||||| merged common ancestors
        NS_RELEASE(request);
    }

    return firstError;
=======
  return firstError;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetLoadFlags(uint32_t *aLoadFlags) {
  *aLoadFlags = mLoadFlags;
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::GetLoadFlags(uint32_t *aLoadFlags)
{
    *aLoadFlags = mLoadFlags;
    return NS_OK;
=======
nsLoadGroup::GetLoadFlags(uint32_t* aLoadFlags) {
  *aLoadFlags = mLoadFlags;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
nsLoadGroup::SetLoadFlags(uint32_t aLoadFlags) {
  mLoadFlags = aLoadFlags;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetLoadGroup(nsILoadGroup **loadGroup) {
  *loadGroup = mLoadGroup;
  NS_IF_ADDREF(*loadGroup);
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::GetLoadGroup(nsILoadGroup **loadGroup)
{
    *loadGroup = mLoadGroup;
    NS_IF_ADDREF(*loadGroup);
    return NS_OK;
=======
nsLoadGroup::GetLoadGroup(nsILoadGroup** loadGroup) {
  nsCOMPtr<nsILoadGroup> result = mLoadGroup;
  result.forget(loadGroup);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::SetLoadGroup(nsILoadGroup *loadGroup) {
  mLoadGroup = loadGroup;
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::SetLoadGroup(nsILoadGroup *loadGroup)
{
    mLoadGroup = loadGroup;
    return NS_OK;
=======
nsLoadGroup::SetLoadGroup(nsILoadGroup* loadGroup) {
  mLoadGroup = loadGroup;
  return NS_OK;
>>>>>>> upstream-releases
}

////////////////////////////////////////////////////////////////////////////////
// nsILoadGroup methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetDefaultLoadRequest(nsIRequest **aRequest) {
  *aRequest = mDefaultLoadRequest;
  NS_IF_ADDREF(*aRequest);
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::GetDefaultLoadRequest(nsIRequest * *aRequest)
{
    *aRequest = mDefaultLoadRequest;
    NS_IF_ADDREF(*aRequest);
    return NS_OK;
=======
nsLoadGroup::GetDefaultLoadRequest(nsIRequest** aRequest) {
  nsCOMPtr<nsIRequest> result = mDefaultLoadRequest;
  result.forget(aRequest);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::SetDefaultLoadRequest(nsIRequest *aRequest) {
  LOG(("nsLoadGroup::SetDefaultLoadRequest this=%p default-request=%p", this,
       aRequest));

  mDefaultLoadRequest = aRequest;
  // Inherit the group load flags from the default load request
  if (mDefaultLoadRequest) {
    mDefaultLoadRequest->GetLoadFlags(&mLoadFlags);
    //
    // Mask off any bits that are not part of the nsIRequest flags.
    // in particular, nsIChannel::LOAD_DOCUMENT_URI...
    //
    mLoadFlags &= nsIRequest::LOAD_REQUESTMASK;

    nsCOMPtr<nsITimedChannel> timedChannel = do_QueryInterface(aRequest);
    mDefaultLoadIsTimed = timedChannel != nullptr;
    if (mDefaultLoadIsTimed) {
      timedChannel->GetChannelCreation(&mDefaultRequestCreationTime);
      timedChannel->SetTimingEnabled(true);
||||||| merged common ancestors
nsLoadGroup::SetDefaultLoadRequest(nsIRequest *aRequest)
{
    LOG(("nsLoadGroup::SetDefaultLoadRequest this=%p default-request=%p",
         this, aRequest));

    mDefaultLoadRequest = aRequest;
    // Inherit the group load flags from the default load request
    if (mDefaultLoadRequest) {
        mDefaultLoadRequest->GetLoadFlags(&mLoadFlags);
        //
        // Mask off any bits that are not part of the nsIRequest flags.
        // in particular, nsIChannel::LOAD_DOCUMENT_URI...
        //
        mLoadFlags &= nsIRequest::LOAD_REQUESTMASK;

        nsCOMPtr<nsITimedChannel> timedChannel = do_QueryInterface(aRequest);
        mDefaultLoadIsTimed = timedChannel != nullptr;
        if (mDefaultLoadIsTimed) {
            timedChannel->GetChannelCreation(&mDefaultRequestCreationTime);
            timedChannel->SetTimingEnabled(true);
        }
=======
nsLoadGroup::SetDefaultLoadRequest(nsIRequest* aRequest) {
  LOG(("nsLoadGroup::SetDefaultLoadRequest this=%p default-request=%p", this,
       aRequest));

  mDefaultLoadRequest = aRequest;
  // Inherit the group load flags from the default load request
  if (mDefaultLoadRequest) {
    mDefaultLoadRequest->GetLoadFlags(&mLoadFlags);
    //
    // Mask off any bits that are not part of the nsIRequest flags.
    // in particular, nsIChannel::LOAD_DOCUMENT_URI...
    //
    mLoadFlags &= nsIRequest::LOAD_REQUESTMASK;

    nsCOMPtr<nsITimedChannel> timedChannel = do_QueryInterface(aRequest);
    mDefaultLoadIsTimed = timedChannel != nullptr;
    if (mDefaultLoadIsTimed) {
      timedChannel->GetChannelCreation(&mDefaultRequestCreationTime);
      timedChannel->SetTimingEnabled(true);
>>>>>>> upstream-releases
    }
  }
  // Else, do not change the group's load flags (see bug 95981)
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::AddRequest(nsIRequest *request, nsISupports *ctxt) {
  nsresult rv;
||||||| merged common ancestors
nsLoadGroup::AddRequest(nsIRequest *request, nsISupports* ctxt)
{
    nsresult rv;
=======
nsLoadGroup::AddRequest(nsIRequest* request, nsISupports* ctxt) {
  nsresult rv;
>>>>>>> upstream-releases

  if (MOZ_LOG_TEST(gLoadGroupLog, LogLevel::Debug)) {
    nsAutoCString nameStr;
    request->GetName(nameStr);
    LOG(("LOADGROUP [%p]: Adding request %p %s (count=%d).\n", this, request,
         nameStr.get(), mRequests.EntryCount()));
  }

  NS_ASSERTION(!mRequests.Search(request),
               "Entry added to loadgroup twice, don't do that");

  //
  // Do not add the channel, if the loadgroup is being canceled...
  //
  if (mIsCanceling) {
    LOG(
        ("LOADGROUP [%p]: AddChannel() ABORTED because LoadGroup is"
         " being canceled!!\n",
         this));

    return NS_BINDING_ABORTED;
  }

  nsLoadFlags flags;
  // if the request is the default load request or if the default load
  // request is null, then the load group should inherit its load flags from
  // the request, but also we need to enforce defaultLoadFlags.
  if (mDefaultLoadRequest == request || !mDefaultLoadRequest) {
    rv = MergeDefaultLoadFlags(request, flags);
  } else {
    rv = MergeLoadFlags(request, flags);
  }
  if (NS_FAILED(rv)) return rv;

  //
  // Add the request to the list of active requests...
  //

<<<<<<< HEAD
  auto entry = static_cast<RequestMapEntry *>(mRequests.Add(request, fallible));
  if (!entry) {
    return NS_ERROR_OUT_OF_MEMORY;
  }
||||||| merged common ancestors
    auto entry =
        static_cast<RequestMapEntry*>(mRequests.Add(request, fallible));
    if (!entry) {
        return NS_ERROR_OUT_OF_MEMORY;
    }
=======
  auto entry = static_cast<RequestMapEntry*>(mRequests.Add(request, fallible));
  if (!entry) {
    return NS_ERROR_OUT_OF_MEMORY;
  }
>>>>>>> upstream-releases

  if (mPriority != 0) RescheduleRequest(request, mPriority);

  nsCOMPtr<nsITimedChannel> timedChannel = do_QueryInterface(request);
  if (timedChannel) timedChannel->SetTimingEnabled(true);

  if (!(flags & nsIRequest::LOAD_BACKGROUND)) {
    // Update the count of foreground URIs..
    mForegroundCount += 1;

<<<<<<< HEAD
    //
    // Fire the OnStartRequest notification out to the observer...
    //
    // If the notification fails then DO NOT add the request to
    // the load group.
    //
    nsCOMPtr<nsIRequestObserver> observer = do_QueryReferent(mObserver);
    if (observer) {
      LOG(
          ("LOADGROUP [%p]: Firing OnStartRequest for request %p."
           "(foreground count=%d).\n",
           this, request, mForegroundCount));

      rv = observer->OnStartRequest(request, ctxt);
      if (NS_FAILED(rv)) {
        LOG(("LOADGROUP [%p]: OnStartRequest for request %p FAILED.\n", this,
             request));
||||||| merged common ancestors
=======
    //
    // Fire the OnStartRequest notification out to the observer...
    //
    // If the notification fails then DO NOT add the request to
    // the load group.
    //
    nsCOMPtr<nsIRequestObserver> observer = do_QueryReferent(mObserver);
    if (observer) {
      LOG(
          ("LOADGROUP [%p]: Firing OnStartRequest for request %p."
           "(foreground count=%d).\n",
           this, request, mForegroundCount));

      rv = observer->OnStartRequest(request);
      if (NS_FAILED(rv)) {
        LOG(("LOADGROUP [%p]: OnStartRequest for request %p FAILED.\n", this,
             request));
>>>>>>> upstream-releases
        //
        // The URI load has been canceled by the observer.  Clean up
        // the damage...
        //

        mRequests.Remove(request);

        rv = NS_OK;

        mForegroundCount -= 1;
      }
    }

    // Ensure that we're part of our loadgroup while pending
    if (mForegroundCount == 1 && mLoadGroup) {
      mLoadGroup->AddRequest(this, nullptr);
    }
  }

  return rv;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::RemoveRequest(nsIRequest *request, nsISupports *ctxt,
                           nsresult aStatus) {
  NS_ENSURE_ARG_POINTER(request);
  nsresult rv;
||||||| merged common ancestors
nsLoadGroup::RemoveRequest(nsIRequest *request, nsISupports* ctxt,
                           nsresult aStatus)
{
    NS_ENSURE_ARG_POINTER(request);
    nsresult rv;

    if (MOZ_LOG_TEST(gLoadGroupLog, LogLevel::Debug)) {
        nsAutoCString nameStr;
        request->GetName(nameStr);
        LOG(("LOADGROUP [%p]: Removing request %p %s status %" PRIx32 " (count=%d).\n",
             this, request, nameStr.get(), static_cast<uint32_t>(aStatus),
             mRequests.EntryCount() - 1));
    }

    // Make sure we have a owning reference to the request we're about
    // to remove.

    nsCOMPtr<nsIRequest> kungFuDeathGrip(request);

    //
    // Remove the request from the group.  If this fails, it means that
    // the request was *not* in the group so do not update the foreground
    // count or it will get messed up...
    //
    auto entry = static_cast<RequestMapEntry*>(mRequests.Search(request));
=======
nsLoadGroup::RemoveRequest(nsIRequest* request, nsISupports* ctxt,
                           nsresult aStatus) {
  NS_ENSURE_ARG_POINTER(request);
  nsresult rv;

  if (MOZ_LOG_TEST(gLoadGroupLog, LogLevel::Debug)) {
    nsAutoCString nameStr;
    request->GetName(nameStr);
    LOG(("LOADGROUP [%p]: Removing request %p %s status %" PRIx32
         " (count=%d).\n",
         this, request, nameStr.get(), static_cast<uint32_t>(aStatus),
         mRequests.EntryCount() - 1));
  }

  // Make sure we have a owning reference to the request we're about
  // to remove.

  nsCOMPtr<nsIRequest> kungFuDeathGrip(request);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (MOZ_LOG_TEST(gLoadGroupLog, LogLevel::Debug)) {
    nsAutoCString nameStr;
    request->GetName(nameStr);
    LOG(("LOADGROUP [%p]: Removing request %p %s status %" PRIx32
         " (count=%d).\n",
         this, request, nameStr.get(), static_cast<uint32_t>(aStatus),
         mRequests.EntryCount() - 1));
  }
||||||| merged common ancestors
    if (!entry) {
        LOG(("LOADGROUP [%p]: Unable to remove request %p. Not in group!\n",
            this, request));
=======
  //
  // Remove the request from the group.  If this fails, it means that
  // the request was *not* in the group so do not update the foreground
  // count or it will get messed up...
  //
  auto entry = static_cast<RequestMapEntry*>(mRequests.Search(request));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Make sure we have a owning reference to the request we're about
  // to remove.
||||||| merged common ancestors
        return NS_ERROR_FAILURE;
    }
=======
  if (!entry) {
    LOG(("LOADGROUP [%p]: Unable to remove request %p. Not in group!\n", this,
         request));
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIRequest> kungFuDeathGrip(request);
||||||| merged common ancestors
    mRequests.RemoveEntry(entry);

    // Collect telemetry stats only when default request is a timed channel.
    // Don't include failed requests in the timing statistics.
    if (mDefaultLoadIsTimed && NS_SUCCEEDED(aStatus)) {
        nsCOMPtr<nsITimedChannel> timedChannel = do_QueryInterface(request);
        if (timedChannel) {
            // Figure out if this request was served from the cache
            ++mTimedRequests;
            TimeStamp timeStamp;
            rv = timedChannel->GetCacheReadStart(&timeStamp);
            if (NS_SUCCEEDED(rv) && !timeStamp.IsNull()) {
                ++mCachedRequests;
            }
            else {
                mTimedNonCachedRequestsUntilOnEndPageLoad++;
            }

            rv = timedChannel->GetAsyncOpen(&timeStamp);
            if (NS_SUCCEEDED(rv) && !timeStamp.IsNull()) {
                Telemetry::AccumulateTimeDelta(
                    Telemetry::HTTP_SUBITEM_OPEN_LATENCY_TIME,
                    mDefaultRequestCreationTime, timeStamp);
            }

            rv = timedChannel->GetResponseStart(&timeStamp);
            if (NS_SUCCEEDED(rv) && !timeStamp.IsNull()) {
                Telemetry::AccumulateTimeDelta(
                    Telemetry::HTTP_SUBITEM_FIRST_BYTE_LATENCY_TIME,
                    mDefaultRequestCreationTime, timeStamp);
            }

            TelemetryReportChannel(timedChannel, false);
        }
    }
=======
    return NS_ERROR_FAILURE;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  //
  // Remove the request from the group.  If this fails, it means that
  // the request was *not* in the group so do not update the foreground
  // count or it will get messed up...
  //
  auto entry = static_cast<RequestMapEntry *>(mRequests.Search(request));

  if (!entry) {
    LOG(("LOADGROUP [%p]: Unable to remove request %p. Not in group!\n", this,
         request));

    return NS_ERROR_FAILURE;
  }

  mRequests.RemoveEntry(entry);
||||||| merged common ancestors
    if (mRequests.EntryCount() == 0) {
        TelemetryReport();
    }

    // Undo any group priority delta...
    if (mPriority != 0)
        RescheduleRequest(request, -mPriority);

    nsLoadFlags flags;
    rv = request->GetLoadFlags(&flags);
    if (NS_FAILED(rv)) return rv;

    if (!(flags & nsIRequest::LOAD_BACKGROUND)) {
        NS_ASSERTION(mForegroundCount > 0, "ForegroundCount messed up");
        mForegroundCount -= 1;
=======
  mRequests.RemoveEntry(entry);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Collect telemetry stats only when default request is a timed channel.
  // Don't include failed requests in the timing statistics.
  if (mDefaultLoadIsTimed && NS_SUCCEEDED(aStatus)) {
    nsCOMPtr<nsITimedChannel> timedChannel = do_QueryInterface(request);
    if (timedChannel) {
      // Figure out if this request was served from the cache
      ++mTimedRequests;
      TimeStamp timeStamp;
      rv = timedChannel->GetCacheReadStart(&timeStamp);
      if (NS_SUCCEEDED(rv) && !timeStamp.IsNull()) {
        ++mCachedRequests;
      } else {
        mTimedNonCachedRequestsUntilOnEndPageLoad++;
      }

      rv = timedChannel->GetAsyncOpen(&timeStamp);
      if (NS_SUCCEEDED(rv) && !timeStamp.IsNull()) {
        Telemetry::AccumulateTimeDelta(
            Telemetry::HTTP_SUBITEM_OPEN_LATENCY_TIME,
            mDefaultRequestCreationTime, timeStamp);
      }

      rv = timedChannel->GetResponseStart(&timeStamp);
      if (NS_SUCCEEDED(rv) && !timeStamp.IsNull()) {
        Telemetry::AccumulateTimeDelta(
            Telemetry::HTTP_SUBITEM_FIRST_BYTE_LATENCY_TIME,
            mDefaultRequestCreationTime, timeStamp);
      }

      TelemetryReportChannel(timedChannel, false);
    }
  }

  if (mRequests.EntryCount() == 0) {
    TelemetryReport();
  }

  // Undo any group priority delta...
  if (mPriority != 0) RescheduleRequest(request, -mPriority);

  nsLoadFlags flags;
  rv = request->GetLoadFlags(&flags);
  if (NS_FAILED(rv)) return rv;

  if (!(flags & nsIRequest::LOAD_BACKGROUND)) {
    NS_ASSERTION(mForegroundCount > 0, "ForegroundCount messed up");
    mForegroundCount -= 1;

    // Fire the OnStopRequest out to the observer...
    nsCOMPtr<nsIRequestObserver> observer = do_QueryReferent(mObserver);
    if (observer) {
      LOG(
          ("LOADGROUP [%p]: Firing OnStopRequest for request %p."
           "(foreground count=%d).\n",
           this, request, mForegroundCount));
||||||| merged common ancestors
        // Fire the OnStopRequest out to the observer...
        nsCOMPtr<nsIRequestObserver> observer = do_QueryReferent(mObserver);
        if (observer) {
            LOG(("LOADGROUP [%p]: Firing OnStopRequest for request %p."
                 "(foreground count=%d).\n", this, request, mForegroundCount));
=======
  // Collect telemetry stats only when default request is a timed channel.
  // Don't include failed requests in the timing statistics.
  if (mDefaultLoadIsTimed && NS_SUCCEEDED(aStatus)) {
    nsCOMPtr<nsITimedChannel> timedChannel = do_QueryInterface(request);
    if (timedChannel) {
      // Figure out if this request was served from the cache
      ++mTimedRequests;
      TimeStamp timeStamp;
      rv = timedChannel->GetCacheReadStart(&timeStamp);
      if (NS_SUCCEEDED(rv) && !timeStamp.IsNull()) {
        ++mCachedRequests;
      }

      rv = timedChannel->GetAsyncOpen(&timeStamp);
      if (NS_SUCCEEDED(rv) && !timeStamp.IsNull()) {
        Telemetry::AccumulateTimeDelta(
            Telemetry::HTTP_SUBITEM_OPEN_LATENCY_TIME,
            mDefaultRequestCreationTime, timeStamp);
      }

      rv = timedChannel->GetResponseStart(&timeStamp);
      if (NS_SUCCEEDED(rv) && !timeStamp.IsNull()) {
        Telemetry::AccumulateTimeDelta(
            Telemetry::HTTP_SUBITEM_FIRST_BYTE_LATENCY_TIME,
            mDefaultRequestCreationTime, timeStamp);
      }

      TelemetryReportChannel(timedChannel, false);
    }
  }

  if (mRequests.EntryCount() == 0) {
    TelemetryReport();
  }

  // Undo any group priority delta...
  if (mPriority != 0) RescheduleRequest(request, -mPriority);

  nsLoadFlags flags;
  rv = request->GetLoadFlags(&flags);
  if (NS_FAILED(rv)) return rv;

  if (!(flags & nsIRequest::LOAD_BACKGROUND)) {
    NS_ASSERTION(mForegroundCount > 0, "ForegroundCount messed up");
    mForegroundCount -= 1;

    // Fire the OnStopRequest out to the observer...
    nsCOMPtr<nsIRequestObserver> observer = do_QueryReferent(mObserver);
    if (observer) {
      LOG(
          ("LOADGROUP [%p]: Firing OnStopRequest for request %p."
           "(foreground count=%d).\n",
           this, request, mForegroundCount));
>>>>>>> upstream-releases

<<<<<<< HEAD
      rv = observer->OnStopRequest(request, ctxt, aStatus);
||||||| merged common ancestors
            rv = observer->OnStopRequest(request, ctxt, aStatus);
=======
      rv = observer->OnStopRequest(request, aStatus);
>>>>>>> upstream-releases

      if (NS_FAILED(rv)) {
        LOG(("LOADGROUP [%p]: OnStopRequest for request %p FAILED.\n", this,
             request));
      }
    }

    // If that was the last request -> remove ourselves from loadgroup
    if (mForegroundCount == 0 && mLoadGroup) {
      mLoadGroup->RemoveRequest(this, nullptr, aStatus);
    }
  }

  return rv;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetRequests(nsISimpleEnumerator **aRequests) {
  nsCOMArray<nsIRequest> requests;
  requests.SetCapacity(mRequests.EntryCount());
||||||| merged common ancestors
nsLoadGroup::GetRequests(nsISimpleEnumerator * *aRequests)
{
    nsCOMArray<nsIRequest> requests;
    requests.SetCapacity(mRequests.EntryCount());
=======
nsLoadGroup::GetRequests(nsISimpleEnumerator** aRequests) {
  nsCOMArray<nsIRequest> requests;
  requests.SetCapacity(mRequests.EntryCount());
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (auto iter = mRequests.Iter(); !iter.Done(); iter.Next()) {
    auto e = static_cast<RequestMapEntry *>(iter.Get());
    requests.AppendObject(e->mKey);
  }
||||||| merged common ancestors
    for (auto iter = mRequests.Iter(); !iter.Done(); iter.Next()) {
      auto e = static_cast<RequestMapEntry*>(iter.Get());
      requests.AppendObject(e->mKey);
    }
=======
  for (auto iter = mRequests.Iter(); !iter.Done(); iter.Next()) {
    auto e = static_cast<RequestMapEntry*>(iter.Get());
    requests.AppendObject(e->mKey);
  }
>>>>>>> upstream-releases

  return NS_NewArrayEnumerator(aRequests, requests, NS_GET_IID(nsIRequest));
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::SetGroupObserver(nsIRequestObserver *aObserver) {
  mObserver = do_GetWeakReference(aObserver);
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::SetGroupObserver(nsIRequestObserver* aObserver)
{
    mObserver = do_GetWeakReference(aObserver);
    return NS_OK;
=======
nsLoadGroup::SetGroupObserver(nsIRequestObserver* aObserver) {
  mObserver = do_GetWeakReference(aObserver);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetGroupObserver(nsIRequestObserver **aResult) {
  nsCOMPtr<nsIRequestObserver> observer = do_QueryReferent(mObserver);
  *aResult = observer;
  NS_IF_ADDREF(*aResult);
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::GetGroupObserver(nsIRequestObserver* *aResult)
{
    nsCOMPtr<nsIRequestObserver> observer = do_QueryReferent(mObserver);
    *aResult = observer;
    NS_IF_ADDREF(*aResult);
    return NS_OK;
=======
nsLoadGroup::GetGroupObserver(nsIRequestObserver** aResult) {
  nsCOMPtr<nsIRequestObserver> observer = do_QueryReferent(mObserver);
  observer.forget(aResult);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetActiveCount(uint32_t *aResult) {
  *aResult = mForegroundCount;
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::GetActiveCount(uint32_t* aResult)
{
    *aResult = mForegroundCount;
    return NS_OK;
=======
nsLoadGroup::GetActiveCount(uint32_t* aResult) {
  *aResult = mForegroundCount;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetNotificationCallbacks(nsIInterfaceRequestor **aCallbacks) {
  NS_ENSURE_ARG_POINTER(aCallbacks);
  *aCallbacks = mCallbacks;
  NS_IF_ADDREF(*aCallbacks);
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::GetNotificationCallbacks(nsIInterfaceRequestor **aCallbacks)
{
    NS_ENSURE_ARG_POINTER(aCallbacks);
    *aCallbacks = mCallbacks;
    NS_IF_ADDREF(*aCallbacks);
    return NS_OK;
=======
nsLoadGroup::GetNotificationCallbacks(nsIInterfaceRequestor** aCallbacks) {
  NS_ENSURE_ARG_POINTER(aCallbacks);
  nsCOMPtr<nsIInterfaceRequestor> callbacks = mCallbacks;
  callbacks.forget(aCallbacks);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::SetNotificationCallbacks(nsIInterfaceRequestor *aCallbacks) {
  mCallbacks = aCallbacks;
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::SetNotificationCallbacks(nsIInterfaceRequestor *aCallbacks)
{
    mCallbacks = aCallbacks;
    return NS_OK;
=======
nsLoadGroup::SetNotificationCallbacks(nsIInterfaceRequestor* aCallbacks) {
  mCallbacks = aCallbacks;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetRequestContextID(uint64_t *aRCID) {
  if (!mRequestContext) {
    return NS_ERROR_NOT_AVAILABLE;
  }
  *aRCID = mRequestContext->GetID();
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::GetRequestContextID(uint64_t *aRCID)
{
    if (!mRequestContext) {
        return NS_ERROR_NOT_AVAILABLE;
    }
    return mRequestContext->GetID(aRCID);
=======
nsLoadGroup::GetRequestContextID(uint64_t* aRCID) {
  if (!mRequestContext) {
    return NS_ERROR_NOT_AVAILABLE;
  }
  *aRCID = mRequestContext->GetID();
  return NS_OK;
>>>>>>> upstream-releases
}

////////////////////////////////////////////////////////////////////////////////
// nsILoadGroupChild methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetParentLoadGroup(nsILoadGroup **aParentLoadGroup) {
  *aParentLoadGroup = nullptr;
  nsCOMPtr<nsILoadGroup> parent = do_QueryReferent(mParentLoadGroup);
  if (!parent) return NS_OK;
  parent.forget(aParentLoadGroup);
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::GetParentLoadGroup(nsILoadGroup * *aParentLoadGroup)
{
    *aParentLoadGroup = nullptr;
    nsCOMPtr<nsILoadGroup> parent = do_QueryReferent(mParentLoadGroup);
    if (!parent)
        return NS_OK;
    parent.forget(aParentLoadGroup);
    return NS_OK;
=======
nsLoadGroup::GetParentLoadGroup(nsILoadGroup** aParentLoadGroup) {
  *aParentLoadGroup = nullptr;
  nsCOMPtr<nsILoadGroup> parent = do_QueryReferent(mParentLoadGroup);
  if (!parent) return NS_OK;
  parent.forget(aParentLoadGroup);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::SetParentLoadGroup(nsILoadGroup *aParentLoadGroup) {
  mParentLoadGroup = do_GetWeakReference(aParentLoadGroup);
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::SetParentLoadGroup(nsILoadGroup *aParentLoadGroup)
{
    mParentLoadGroup = do_GetWeakReference(aParentLoadGroup);
    return NS_OK;
=======
nsLoadGroup::SetParentLoadGroup(nsILoadGroup* aParentLoadGroup) {
  mParentLoadGroup = do_GetWeakReference(aParentLoadGroup);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetChildLoadGroup(nsILoadGroup **aChildLoadGroup) {
  NS_ADDREF(*aChildLoadGroup = this);
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::GetChildLoadGroup(nsILoadGroup * *aChildLoadGroup)
{
    NS_ADDREF(*aChildLoadGroup = this);
    return NS_OK;
=======
nsLoadGroup::GetChildLoadGroup(nsILoadGroup** aChildLoadGroup) {
  NS_ADDREF(*aChildLoadGroup = this);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetRootLoadGroup(nsILoadGroup **aRootLoadGroup) {
  // first recursively try the root load group of our parent
  nsCOMPtr<nsILoadGroupChild> ancestor = do_QueryReferent(mParentLoadGroup);
  if (ancestor) return ancestor->GetRootLoadGroup(aRootLoadGroup);

  // next recursively try the root load group of our own load grop
  ancestor = do_QueryInterface(mLoadGroup);
  if (ancestor) return ancestor->GetRootLoadGroup(aRootLoadGroup);

  // finally just return this
  NS_ADDREF(*aRootLoadGroup = this);
  return NS_OK;
}
||||||| merged common ancestors
nsLoadGroup::GetRootLoadGroup(nsILoadGroup * *aRootLoadGroup)
{
    // first recursively try the root load group of our parent
    nsCOMPtr<nsILoadGroupChild> ancestor = do_QueryReferent(mParentLoadGroup);
    if (ancestor)
        return ancestor->GetRootLoadGroup(aRootLoadGroup);

    // next recursively try the root load group of our own load grop
    ancestor = do_QueryInterface(mLoadGroup);
    if (ancestor)
        return ancestor->GetRootLoadGroup(aRootLoadGroup);

    // finally just return this
    NS_ADDREF(*aRootLoadGroup = this);
    return NS_OK;
}
=======
nsLoadGroup::GetRootLoadGroup(nsILoadGroup** aRootLoadGroup) {
  // first recursively try the root load group of our parent
  nsCOMPtr<nsILoadGroupChild> ancestor = do_QueryReferent(mParentLoadGroup);
  if (ancestor) return ancestor->GetRootLoadGroup(aRootLoadGroup);
>>>>>>> upstream-releases

  // next recursively try the root load group of our own load grop
  ancestor = do_QueryInterface(mLoadGroup);
  if (ancestor) return ancestor->GetRootLoadGroup(aRootLoadGroup);

<<<<<<< HEAD
NS_IMETHODIMP
nsLoadGroup::OnEndPageLoad(nsIChannel *aDefaultChannel) {
  LOG(("nsLoadGroup::OnEndPageLoad this=%p default-request=%p", this,
       aDefaultChannel));

  // for the moment, nothing to do here.
  return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
nsLoadGroup::OnEndPageLoad(nsIChannel *aDefaultChannel)
{
    LOG(("nsLoadGroup::OnEndPageLoad this=%p default-request=%p",
         this, aDefaultChannel));

    // for the moment, nothing to do here.
    return NS_OK;
=======
  // finally just return this
  NS_ADDREF(*aRootLoadGroup = this);
  return NS_OK;
>>>>>>> upstream-releases
}

////////////////////////////////////////////////////////////////////////////////
// nsISupportsPriority methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetPriority(int32_t *aValue) {
  *aValue = mPriority;
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::GetPriority(int32_t *aValue)
{
    *aValue = mPriority;
    return NS_OK;
=======
nsLoadGroup::GetPriority(int32_t* aValue) {
  *aValue = mPriority;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
nsLoadGroup::SetPriority(int32_t aValue) {
  return AdjustPriority(aValue - mPriority);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::AdjustPriority(int32_t aDelta) {
  // Update the priority for each request that supports nsISupportsPriority
  if (aDelta != 0) {
    mPriority += aDelta;
    for (auto iter = mRequests.Iter(); !iter.Done(); iter.Next()) {
      auto e = static_cast<RequestMapEntry *>(iter.Get());
      RescheduleRequest(e->mKey, aDelta);
||||||| merged common ancestors
nsLoadGroup::AdjustPriority(int32_t aDelta)
{
    // Update the priority for each request that supports nsISupportsPriority
    if (aDelta != 0) {
        mPriority += aDelta;
        for (auto iter = mRequests.Iter(); !iter.Done(); iter.Next()) {
          auto e = static_cast<RequestMapEntry*>(iter.Get());
          RescheduleRequest(e->mKey, aDelta);
        }
=======
nsLoadGroup::AdjustPriority(int32_t aDelta) {
  // Update the priority for each request that supports nsISupportsPriority
  if (aDelta != 0) {
    mPriority += aDelta;
    for (auto iter = mRequests.Iter(); !iter.Done(); iter.Next()) {
      auto e = static_cast<RequestMapEntry*>(iter.Get());
      RescheduleRequest(e->mKey, aDelta);
>>>>>>> upstream-releases
    }
  }
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetDefaultLoadFlags(uint32_t *aFlags) {
  *aFlags = mDefaultLoadFlags;
  return NS_OK;
||||||| merged common ancestors
nsLoadGroup::GetDefaultLoadFlags(uint32_t *aFlags)
{
    *aFlags = mDefaultLoadFlags;
    return NS_OK;
=======
nsLoadGroup::GetDefaultLoadFlags(uint32_t* aFlags) {
  *aFlags = mDefaultLoadFlags;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
nsLoadGroup::SetDefaultLoadFlags(uint32_t aFlags) {
  mDefaultLoadFlags = aFlags;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::GetUserAgentOverrideCache(nsACString &aUserAgentOverrideCache) {
||||||| merged common ancestors
nsLoadGroup::GetUserAgentOverrideCache(nsACString & aUserAgentOverrideCache)
{
=======
nsLoadGroup::GetUserAgentOverrideCache(nsACString& aUserAgentOverrideCache) {
>>>>>>> upstream-releases
  aUserAgentOverrideCache = mUserAgentOverrideCache;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLoadGroup::SetUserAgentOverrideCache(
    const nsACString &aUserAgentOverrideCache) {
||||||| merged common ancestors
nsLoadGroup::SetUserAgentOverrideCache(const nsACString & aUserAgentOverrideCache)
{
=======
nsLoadGroup::SetUserAgentOverrideCache(
    const nsACString& aUserAgentOverrideCache) {
>>>>>>> upstream-releases
  mUserAgentOverrideCache = aUserAgentOverrideCache;
  return NS_OK;
}

////////////////////////////////////////////////////////////////////////////////

<<<<<<< HEAD
void nsLoadGroup::TelemetryReport() {
  nsresult defaultStatus = NS_ERROR_INVALID_ARG;
  // We should only report HTTP_PAGE_* telemetry if the defaultRequest was
  // actually successful.
  if (mDefaultLoadRequest) {
    mDefaultLoadRequest->GetStatus(&defaultStatus);
  }
  if (mDefaultLoadIsTimed && NS_SUCCEEDED(defaultStatus)) {
    Telemetry::Accumulate(Telemetry::HTTP_REQUEST_PER_PAGE, mTimedRequests);
    if (mTimedRequests) {
      Telemetry::Accumulate(Telemetry::HTTP_REQUEST_PER_PAGE_FROM_CACHE,
                            mCachedRequests * 100 / mTimedRequests);
    }

    nsCOMPtr<nsITimedChannel> timedChannel =
        do_QueryInterface(mDefaultLoadRequest);
    if (timedChannel) TelemetryReportChannel(timedChannel, true);
  }

  mTimedRequests = 0;
  mCachedRequests = 0;
  mDefaultLoadIsTimed = false;
}

void nsLoadGroup::TelemetryReportChannel(nsITimedChannel *aTimedChannel,
                                         bool aDefaultRequest) {
  nsresult rv;
  bool timingEnabled;
  rv = aTimedChannel->GetTimingEnabled(&timingEnabled);
  if (NS_FAILED(rv) || !timingEnabled) return;

  TimeStamp asyncOpen;
  rv = aTimedChannel->GetAsyncOpen(&asyncOpen);
  // We do not check !asyncOpen.IsNull() bellow, prevent ASSERTIONs this way
  if (NS_FAILED(rv) || asyncOpen.IsNull()) return;

  TimeStamp cacheReadStart;
  rv = aTimedChannel->GetCacheReadStart(&cacheReadStart);
  if (NS_FAILED(rv)) return;

  TimeStamp cacheReadEnd;
  rv = aTimedChannel->GetCacheReadEnd(&cacheReadEnd);
  if (NS_FAILED(rv)) return;

  TimeStamp domainLookupStart;
  rv = aTimedChannel->GetDomainLookupStart(&domainLookupStart);
  if (NS_FAILED(rv)) return;

  TimeStamp domainLookupEnd;
  rv = aTimedChannel->GetDomainLookupEnd(&domainLookupEnd);
  if (NS_FAILED(rv)) return;

  TimeStamp connectStart;
  rv = aTimedChannel->GetConnectStart(&connectStart);
  if (NS_FAILED(rv)) return;

  TimeStamp secureConnectionStart;
  rv = aTimedChannel->GetSecureConnectionStart(&secureConnectionStart);
  if (NS_FAILED(rv)) return;

  TimeStamp connectEnd;
  rv = aTimedChannel->GetConnectEnd(&connectEnd);
  if (NS_FAILED(rv)) return;

  TimeStamp requestStart;
  rv = aTimedChannel->GetRequestStart(&requestStart);
  if (NS_FAILED(rv)) return;

  TimeStamp responseStart;
  rv = aTimedChannel->GetResponseStart(&responseStart);
  if (NS_FAILED(rv)) return;

  TimeStamp responseEnd;
  rv = aTimedChannel->GetResponseEnd(&responseEnd);
  if (NS_FAILED(rv)) return;
||||||| merged common ancestors
void
nsLoadGroup::TelemetryReport()
{
    nsresult defaultStatus = NS_ERROR_INVALID_ARG;
    // We should only report HTTP_PAGE_* telemetry if the defaultRequest was
    // actually successful.
    if (mDefaultLoadRequest) {
        mDefaultLoadRequest->GetStatus(&defaultStatus);
    }
    if (mDefaultLoadIsTimed && NS_SUCCEEDED(defaultStatus)) {
        Telemetry::Accumulate(Telemetry::HTTP_REQUEST_PER_PAGE, mTimedRequests);
        if (mTimedRequests) {
            Telemetry::Accumulate(Telemetry::HTTP_REQUEST_PER_PAGE_FROM_CACHE,
                                  mCachedRequests * 100 / mTimedRequests);
        }

        nsCOMPtr<nsITimedChannel> timedChannel =
            do_QueryInterface(mDefaultLoadRequest);
        if (timedChannel)
            TelemetryReportChannel(timedChannel, true);
    }

    mTimedRequests = 0;
    mCachedRequests = 0;
    mDefaultLoadIsTimed = false;
}

void
nsLoadGroup::TelemetryReportChannel(nsITimedChannel *aTimedChannel,
                                    bool aDefaultRequest)
{
    nsresult rv;
    bool timingEnabled;
    rv = aTimedChannel->GetTimingEnabled(&timingEnabled);
    if (NS_FAILED(rv) || !timingEnabled)
        return;

    TimeStamp asyncOpen;
    rv = aTimedChannel->GetAsyncOpen(&asyncOpen);
    // We do not check !asyncOpen.IsNull() bellow, prevent ASSERTIONs this way
    if (NS_FAILED(rv) || asyncOpen.IsNull())
        return;

    TimeStamp cacheReadStart;
    rv = aTimedChannel->GetCacheReadStart(&cacheReadStart);
    if (NS_FAILED(rv))
        return;

    TimeStamp cacheReadEnd;
    rv = aTimedChannel->GetCacheReadEnd(&cacheReadEnd);
    if (NS_FAILED(rv))
        return;

    TimeStamp domainLookupStart;
    rv = aTimedChannel->GetDomainLookupStart(&domainLookupStart);
    if (NS_FAILED(rv))
        return;

    TimeStamp domainLookupEnd;
    rv = aTimedChannel->GetDomainLookupEnd(&domainLookupEnd);
    if (NS_FAILED(rv))
        return;

    TimeStamp connectStart;
    rv = aTimedChannel->GetConnectStart(&connectStart);
    if (NS_FAILED(rv))
        return;

    TimeStamp secureConnectionStart;
    rv = aTimedChannel->GetSecureConnectionStart(&secureConnectionStart);
    if (NS_FAILED(rv))
        return;

    TimeStamp connectEnd;
    rv = aTimedChannel->GetConnectEnd(&connectEnd);
    if (NS_FAILED(rv))
        return;

    TimeStamp requestStart;
    rv = aTimedChannel->GetRequestStart(&requestStart);
    if (NS_FAILED(rv))
        return;

    TimeStamp responseStart;
    rv = aTimedChannel->GetResponseStart(&responseStart);
    if (NS_FAILED(rv))
        return;

    TimeStamp responseEnd;
    rv = aTimedChannel->GetResponseEnd(&responseEnd);
    if (NS_FAILED(rv))
        return;
=======
void nsLoadGroup::TelemetryReport() {
  nsresult defaultStatus = NS_ERROR_INVALID_ARG;
  // We should only report HTTP_PAGE_* telemetry if the defaultRequest was
  // actually successful.
  if (mDefaultLoadRequest) {
    mDefaultLoadRequest->GetStatus(&defaultStatus);
  }
  if (mDefaultLoadIsTimed && NS_SUCCEEDED(defaultStatus)) {
    Telemetry::Accumulate(Telemetry::HTTP_REQUEST_PER_PAGE, mTimedRequests);
    if (mTimedRequests) {
      Telemetry::Accumulate(Telemetry::HTTP_REQUEST_PER_PAGE_FROM_CACHE,
                            mCachedRequests * 100 / mTimedRequests);
    }

    nsCOMPtr<nsITimedChannel> timedChannel =
        do_QueryInterface(mDefaultLoadRequest);
    if (timedChannel) TelemetryReportChannel(timedChannel, true);
  }

  mTimedRequests = 0;
  mCachedRequests = 0;
  mDefaultLoadIsTimed = false;
}

void nsLoadGroup::TelemetryReportChannel(nsITimedChannel* aTimedChannel,
                                         bool aDefaultRequest) {
  nsresult rv;
  bool timingEnabled;
  rv = aTimedChannel->GetTimingEnabled(&timingEnabled);
  if (NS_FAILED(rv) || !timingEnabled) return;

  TimeStamp asyncOpen;
  rv = aTimedChannel->GetAsyncOpen(&asyncOpen);
  // We do not check !asyncOpen.IsNull() bellow, prevent ASSERTIONs this way
  if (NS_FAILED(rv) || asyncOpen.IsNull()) return;

  TimeStamp cacheReadStart;
  rv = aTimedChannel->GetCacheReadStart(&cacheReadStart);
  if (NS_FAILED(rv)) return;

  TimeStamp cacheReadEnd;
  rv = aTimedChannel->GetCacheReadEnd(&cacheReadEnd);
  if (NS_FAILED(rv)) return;

  TimeStamp domainLookupStart;
  rv = aTimedChannel->GetDomainLookupStart(&domainLookupStart);
  if (NS_FAILED(rv)) return;

  TimeStamp domainLookupEnd;
  rv = aTimedChannel->GetDomainLookupEnd(&domainLookupEnd);
  if (NS_FAILED(rv)) return;

  TimeStamp connectStart;
  rv = aTimedChannel->GetConnectStart(&connectStart);
  if (NS_FAILED(rv)) return;

  TimeStamp secureConnectionStart;
  rv = aTimedChannel->GetSecureConnectionStart(&secureConnectionStart);
  if (NS_FAILED(rv)) return;

  TimeStamp connectEnd;
  rv = aTimedChannel->GetConnectEnd(&connectEnd);
  if (NS_FAILED(rv)) return;

  TimeStamp requestStart;
  rv = aTimedChannel->GetRequestStart(&requestStart);
  if (NS_FAILED(rv)) return;

  TimeStamp responseStart;
  rv = aTimedChannel->GetResponseStart(&responseStart);
  if (NS_FAILED(rv)) return;

  TimeStamp responseEnd;
  rv = aTimedChannel->GetResponseEnd(&responseEnd);
  if (NS_FAILED(rv)) return;
>>>>>>> upstream-releases

#define HTTP_REQUEST_HISTOGRAMS(prefix)                                        \
  if (!domainLookupStart.IsNull()) {                                           \
    Telemetry::AccumulateTimeDelta(Telemetry::HTTP_##prefix##_DNS_ISSUE_TIME,  \
                                   asyncOpen, domainLookupStart);              \
  }                                                                            \
                                                                               \
  if (!domainLookupStart.IsNull() && !domainLookupEnd.IsNull()) {              \
    Telemetry::AccumulateTimeDelta(Telemetry::HTTP_##prefix##_DNS_LOOKUP_TIME, \
                                   domainLookupStart, domainLookupEnd);        \
  }                                                                            \
                                                                               \
  if (!secureConnectionStart.IsNull() && !connectEnd.IsNull()) {               \
    Telemetry::AccumulateTimeDelta(Telemetry::HTTP_##prefix##_TLS_HANDSHAKE,   \
                                   secureConnectionStart, connectEnd);         \
  }                                                                            \
                                                                               \
  if (!connectStart.IsNull() && !connectEnd.IsNull()) {                        \
    Telemetry::AccumulateTimeDelta(                                            \
        Telemetry::HTTP_##prefix##_TCP_CONNECTION_2, connectStart,             \
        connectEnd);                                                           \
  }                                                                            \
                                                                               \
  if (!requestStart.IsNull() && !responseEnd.IsNull()) {                       \
    Telemetry::AccumulateTimeDelta(                                            \
        Telemetry::HTTP_##prefix##_OPEN_TO_FIRST_SENT, asyncOpen,              \
        requestStart);                                                         \
                                                                               \
    Telemetry::AccumulateTimeDelta(                                            \
        Telemetry::HTTP_##prefix##_FIRST_SENT_TO_LAST_RECEIVED, requestStart,  \
        responseEnd);                                                          \
                                                                               \
    if (cacheReadStart.IsNull() && !responseStart.IsNull()) {                  \
      Telemetry::AccumulateTimeDelta(                                          \
          Telemetry::HTTP_##prefix##_OPEN_TO_FIRST_RECEIVED, asyncOpen,        \
          responseStart);                                                      \
    }                                                                          \
  }                                                                            \
                                                                               \
  if (!cacheReadStart.IsNull() && !cacheReadEnd.IsNull()) {                    \
    Telemetry::AccumulateTimeDelta(                                            \
        Telemetry::HTTP_##prefix##_OPEN_TO_FIRST_FROM_CACHE_V2, asyncOpen,     \
        cacheReadStart);                                                       \
                                                                               \
    Telemetry::AccumulateTimeDelta(                                            \
        Telemetry::HTTP_##prefix##_CACHE_READ_TIME_V2, cacheReadStart,         \
        cacheReadEnd);                                                         \
                                                                               \
    if (!requestStart.IsNull() && !responseEnd.IsNull()) {                     \
      Telemetry::AccumulateTimeDelta(Telemetry::HTTP_##prefix##_REVALIDATION,  \
                                     requestStart, responseEnd);               \
    }                                                                          \
  }                                                                            \
                                                                               \
  if (!cacheReadEnd.IsNull()) {                                                \
    Telemetry::AccumulateTimeDelta(                                            \
        Telemetry::HTTP_##prefix##_COMPLETE_LOAD_V2, asyncOpen, cacheReadEnd); \
    Telemetry::AccumulateTimeDelta(                                            \
        Telemetry::HTTP_##prefix##_COMPLETE_LOAD_CACHED_V2, asyncOpen,         \
        cacheReadEnd);                                                         \
  } else if (!responseEnd.IsNull()) {                                          \
    Telemetry::AccumulateTimeDelta(                                            \
        Telemetry::HTTP_##prefix##_COMPLETE_LOAD_V2, asyncOpen, responseEnd);  \
    Telemetry::AccumulateTimeDelta(                                            \
        Telemetry::HTTP_##prefix##_COMPLETE_LOAD_NET_V2, asyncOpen,            \
        responseEnd);                                                          \
  }

  if (aDefaultRequest) {
    HTTP_REQUEST_HISTOGRAMS(PAGE)
  } else {
    HTTP_REQUEST_HISTOGRAMS(SUB)
  }
#undef HTTP_REQUEST_HISTOGRAMS
}

<<<<<<< HEAD
nsresult nsLoadGroup::MergeLoadFlags(nsIRequest *aRequest,
                                     nsLoadFlags &outFlags) {
  nsresult rv;
  nsLoadFlags flags, oldFlags;
||||||| merged common ancestors
nsresult nsLoadGroup::MergeLoadFlags(nsIRequest *aRequest,
                                     nsLoadFlags& outFlags)
{
    nsresult rv;
    nsLoadFlags flags, oldFlags;

    rv = aRequest->GetLoadFlags(&flags);
    if (NS_FAILED(rv)) {
        return rv;
    }
=======
nsresult nsLoadGroup::MergeLoadFlags(nsIRequest* aRequest,
                                     nsLoadFlags& outFlags) {
  nsresult rv;
  nsLoadFlags flags, oldFlags;

  rv = aRequest->GetLoadFlags(&flags);
  if (NS_FAILED(rv)) {
    return rv;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  rv = aRequest->GetLoadFlags(&flags);
  if (NS_FAILED(rv)) {
    return rv;
  }
||||||| merged common ancestors
    oldFlags = flags;
=======
  oldFlags = flags;
>>>>>>> upstream-releases

<<<<<<< HEAD
  oldFlags = flags;
||||||| merged common ancestors
    // Inherit the following bits...
    flags |= (mLoadFlags & (LOAD_BACKGROUND |
                            LOAD_BYPASS_CACHE |
                            LOAD_FROM_CACHE |
                            VALIDATE_ALWAYS |
                            VALIDATE_ONCE_PER_SESSION |
                            VALIDATE_NEVER));
=======
  // Inherit the following bits...
  flags |= (mLoadFlags &
            (LOAD_BACKGROUND | LOAD_BYPASS_CACHE | LOAD_FROM_CACHE |
             VALIDATE_ALWAYS | VALIDATE_ONCE_PER_SESSION | VALIDATE_NEVER));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Inherit the following bits...
  flags |= (mLoadFlags &
            (LOAD_BACKGROUND | LOAD_BYPASS_CACHE | LOAD_FROM_CACHE |
             VALIDATE_ALWAYS | VALIDATE_ONCE_PER_SESSION | VALIDATE_NEVER));
||||||| merged common ancestors
    // ... and force the default flags.
    flags |= mDefaultLoadFlags;
=======
  // ... and force the default flags.
  flags |= mDefaultLoadFlags;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // ... and force the default flags.
  flags |= mDefaultLoadFlags;
||||||| merged common ancestors
    if (flags != oldFlags) {
        rv = aRequest->SetLoadFlags(flags);
    }
=======
  if (flags != oldFlags) {
    rv = aRequest->SetLoadFlags(flags);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (flags != oldFlags) {
    rv = aRequest->SetLoadFlags(flags);
  }

  outFlags = flags;
  return rv;
||||||| merged common ancestors
    outFlags = flags;
    return rv;
=======
  outFlags = flags;
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsLoadGroup::MergeDefaultLoadFlags(nsIRequest *aRequest,
                                            nsLoadFlags &outFlags) {
  nsresult rv;
  nsLoadFlags flags, oldFlags;
||||||| merged common ancestors
nsresult nsLoadGroup::MergeDefaultLoadFlags(nsIRequest *aRequest,
                                            nsLoadFlags& outFlags)
{
    nsresult rv;
    nsLoadFlags flags, oldFlags;

    rv = aRequest->GetLoadFlags(&flags);
    if (NS_FAILED(rv)) {
        return rv;
    }

    oldFlags = flags;
    // ... and force the default flags.
    flags |= mDefaultLoadFlags;
=======
nsresult nsLoadGroup::MergeDefaultLoadFlags(nsIRequest* aRequest,
                                            nsLoadFlags& outFlags) {
  nsresult rv;
  nsLoadFlags flags, oldFlags;

  rv = aRequest->GetLoadFlags(&flags);
  if (NS_FAILED(rv)) {
    return rv;
  }

  oldFlags = flags;
  // ... and force the default flags.
  flags |= mDefaultLoadFlags;
>>>>>>> upstream-releases

<<<<<<< HEAD
  rv = aRequest->GetLoadFlags(&flags);
  if (NS_FAILED(rv)) {
    return rv;
  }

  oldFlags = flags;
  // ... and force the default flags.
  flags |= mDefaultLoadFlags;

  if (flags != oldFlags) {
    rv = aRequest->SetLoadFlags(flags);
  }
  outFlags = flags;
  return rv;
||||||| merged common ancestors
    if (flags != oldFlags) {
        rv = aRequest->SetLoadFlags(flags);
    }
    outFlags = flags;
    return rv;
=======
  if (flags != oldFlags) {
    rv = aRequest->SetLoadFlags(flags);
  }
  outFlags = flags;
  return rv;
>>>>>>> upstream-releases
}

nsresult nsLoadGroup::Init() {
  mRequestContextService = RequestContextService::GetOrCreate();
  if (mRequestContextService) {
    Unused << mRequestContextService->NewRequestContext(
        getter_AddRefs(mRequestContext));
  }

  return NS_OK;
}

}  // namespace net
}  // namespace mozilla

#undef LOG
