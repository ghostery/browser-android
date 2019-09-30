/* -*- mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsOfflineCacheUpdate.h"

#include "nsCURILoader.h"
#include "nsIApplicationCacheContainer.h"
#include "nsIApplicationCacheChannel.h"
#include "nsIApplicationCacheService.h"
#include "nsICachingChannel.h"
#include "nsIContent.h"
#include "mozilla/dom/Element.h"
#include "mozilla/dom/OfflineResourceListBinding.h"
#include "nsIDocumentLoader.h"
#include "nsIDOMWindow.h"
#include "mozilla/dom/Document.h"
#include "nsIObserverService.h"
#include "nsIURL.h"
#include "nsIURIMutator.h"
#include "nsIWebProgress.h"
#include "nsICryptoHash.h"
#include "nsICacheEntry.h"
#include "nsIPermissionManager.h"
#include "nsIPrincipal.h"
#include "nsNetCID.h"
#include "nsNetUtil.h"
#include "nsServiceManagerUtils.h"
#include "nsStreamUtils.h"
#include "nsThreadUtils.h"
#include "nsProxyRelease.h"
#include "nsIConsoleService.h"
#include "mozilla/Logging.h"
#include "nsIAsyncVerifyRedirectCallback.h"
#include "mozilla/Preferences.h"
#include "mozilla/Attributes.h"
#include "nsContentUtils.h"
#include "nsIPrincipal.h"
#include "nsDiskCacheDeviceSQL.h"
#include "ReferrerInfo.h"

#include "nsXULAppAPI.h"

using namespace mozilla;

static const uint32_t kRescheduleLimit = 3;
// Max number of retries for every entry of pinned app.
static const uint32_t kPinnedEntryRetriesLimit = 3;
// Maximum number of parallel items loads
static const uint32_t kParallelLoadLimit = 15;

// Quota for offline apps when preloading
static const int32_t kCustomProfileQuota = 512000;

//
// To enable logging (see mozilla/Logging.h for full details):
//
//    set MOZ_LOG=nsOfflineCacheUpdate:5
//    set MOZ_LOG_FILE=offlineupdate.log
//
// this enables LogLevel::Debug level information and places all output in
// the file offlineupdate.log
//
extern LazyLogModule gOfflineCacheUpdateLog;

#undef LOG
#define LOG(args) \
  MOZ_LOG(gOfflineCacheUpdateLog, mozilla::LogLevel::Debug, args)

#undef LOG_ENABLED
<<<<<<< HEAD
#define LOG_ENABLED() \
  MOZ_LOG_TEST(gOfflineCacheUpdateLog, mozilla::LogLevel::Debug)

class AutoFreeArray {
 public:
  AutoFreeArray(uint32_t count, char **values)
      : mCount(count), mValues(values){};
  ~AutoFreeArray() { NS_FREE_XPCOM_ALLOCATED_POINTER_ARRAY(mCount, mValues); }

 private:
  uint32_t mCount;
  char **mValues;
};
||||||| merged common ancestors
#define LOG_ENABLED() MOZ_LOG_TEST(gOfflineCacheUpdateLog, mozilla::LogLevel::Debug)

class AutoFreeArray {
public:
    AutoFreeArray(uint32_t count, char **values)
        : mCount(count), mValues(values) {};
    ~AutoFreeArray() { NS_FREE_XPCOM_ALLOCATED_POINTER_ARRAY(mCount, mValues); }
private:
    uint32_t mCount;
    char **mValues;
};
=======
#define LOG_ENABLED() \
  MOZ_LOG_TEST(gOfflineCacheUpdateLog, mozilla::LogLevel::Debug)
>>>>>>> upstream-releases

namespace {

<<<<<<< HEAD
nsresult DropReferenceFromURL(nsCOMPtr<nsIURI> &aURI) {
  // XXXdholbert If this SetRef fails, callers of this method probably
  // want to call aURI->CloneIgnoringRef() and use the result of that.
  nsCOMPtr<nsIURI> uri(aURI);
  return NS_GetURIWithoutRef(uri, getter_AddRefs(aURI));
}

void LogToConsole(const char *message,
                  nsOfflineCacheUpdateItem *item = nullptr) {
  nsCOMPtr<nsIConsoleService> consoleService =
      do_GetService(NS_CONSOLESERVICE_CONTRACTID);
  if (consoleService) {
    nsAutoString messageUTF16 = NS_ConvertUTF8toUTF16(message);
    if (item && item->mURI) {
      messageUTF16.AppendLiteral(", URL=");
      messageUTF16.Append(
          NS_ConvertUTF8toUTF16(item->mURI->GetSpecOrDefault()));
    }
    consoleService->LogStringMessage(messageUTF16.get());
  }
||||||| merged common ancestors
nsresult
DropReferenceFromURL(nsCOMPtr<nsIURI>& aURI)
{
    // XXXdholbert If this SetRef fails, callers of this method probably
    // want to call aURI->CloneIgnoringRef() and use the result of that.
    nsCOMPtr<nsIURI> uri(aURI);
    return NS_GetURIWithoutRef(uri, getter_AddRefs(aURI));
}

void
LogToConsole(const char * message, nsOfflineCacheUpdateItem * item = nullptr)
{
    nsCOMPtr<nsIConsoleService> consoleService =
        do_GetService(NS_CONSOLESERVICE_CONTRACTID);
    if (consoleService)
    {
        nsAutoString messageUTF16 = NS_ConvertUTF8toUTF16(message);
        if (item && item->mURI) {
            messageUTF16.AppendLiteral(", URL=");
            messageUTF16.Append(
                NS_ConvertUTF8toUTF16(item->mURI->GetSpecOrDefault()));
        }
        consoleService->LogStringMessage(messageUTF16.get());
    }
=======
nsresult DropReferenceFromURL(nsCOMPtr<nsIURI>& aURI) {
  // XXXdholbert If this SetRef fails, callers of this method probably
  // want to call aURI->CloneIgnoringRef() and use the result of that.
  nsCOMPtr<nsIURI> uri(aURI);
  return NS_GetURIWithoutRef(uri, getter_AddRefs(aURI));
}

void LogToConsole(const char* message,
                  nsOfflineCacheUpdateItem* item = nullptr) {
  nsCOMPtr<nsIConsoleService> consoleService =
      do_GetService(NS_CONSOLESERVICE_CONTRACTID);
  if (consoleService) {
    nsAutoString messageUTF16 = NS_ConvertUTF8toUTF16(message);
    if (item && item->mURI) {
      messageUTF16.AppendLiteral(", URL=");
      messageUTF16.Append(
          NS_ConvertUTF8toUTF16(item->mURI->GetSpecOrDefault()));
    }
    consoleService->LogStringMessage(messageUTF16.get());
  }
>>>>>>> upstream-releases
}

}  // namespace

//-----------------------------------------------------------------------------
// nsManifestCheck
//-----------------------------------------------------------------------------

<<<<<<< HEAD
class nsManifestCheck final : public nsIStreamListener,
                              public nsIChannelEventSink,
                              public nsIInterfaceRequestor {
 public:
  nsManifestCheck(nsOfflineCacheUpdate *aUpdate, nsIURI *aURI,
                  nsIURI *aReferrerURI, nsIPrincipal *aLoadingPrincipal)
      : mUpdate(aUpdate),
        mURI(aURI),
        mReferrerURI(aReferrerURI),
        mLoadingPrincipal(aLoadingPrincipal) {}

  NS_DECL_ISUPPORTS
  NS_DECL_NSIREQUESTOBSERVER
  NS_DECL_NSISTREAMLISTENER
  NS_DECL_NSICHANNELEVENTSINK
  NS_DECL_NSIINTERFACEREQUESTOR

  nsresult Begin();

 private:
  ~nsManifestCheck() {}

  static nsresult ReadManifest(nsIInputStream *aInputStream, void *aClosure,
                               const char *aFromSegment, uint32_t aOffset,
                               uint32_t aCount, uint32_t *aBytesConsumed);

  RefPtr<nsOfflineCacheUpdate> mUpdate;
  nsCOMPtr<nsIURI> mURI;
  nsCOMPtr<nsIURI> mReferrerURI;
  nsCOMPtr<nsIPrincipal> mLoadingPrincipal;
  nsCOMPtr<nsICryptoHash> mManifestHash;
  nsCOMPtr<nsIChannel> mChannel;
||||||| merged common ancestors
class nsManifestCheck final : public nsIStreamListener
                            , public nsIChannelEventSink
                            , public nsIInterfaceRequestor
{
public:
    nsManifestCheck(nsOfflineCacheUpdate *aUpdate,
                    nsIURI *aURI,
                    nsIURI *aReferrerURI,
                    nsIPrincipal* aLoadingPrincipal)
        : mUpdate(aUpdate)
        , mURI(aURI)
        , mReferrerURI(aReferrerURI)
        , mLoadingPrincipal(aLoadingPrincipal)
        {}

    NS_DECL_ISUPPORTS
    NS_DECL_NSIREQUESTOBSERVER
    NS_DECL_NSISTREAMLISTENER
    NS_DECL_NSICHANNELEVENTSINK
    NS_DECL_NSIINTERFACEREQUESTOR

    nsresult Begin();

private:

    ~nsManifestCheck() {}

    static nsresult ReadManifest(nsIInputStream *aInputStream,
                                 void *aClosure,
                                 const char *aFromSegment,
                                 uint32_t aOffset,
                                 uint32_t aCount,
                                 uint32_t *aBytesConsumed);

    RefPtr<nsOfflineCacheUpdate> mUpdate;
    nsCOMPtr<nsIURI> mURI;
    nsCOMPtr<nsIURI> mReferrerURI;
    nsCOMPtr<nsIPrincipal> mLoadingPrincipal;
    nsCOMPtr<nsICryptoHash> mManifestHash;
    nsCOMPtr<nsIChannel> mChannel;
=======
class nsManifestCheck final : public nsIStreamListener,
                              public nsIChannelEventSink,
                              public nsIInterfaceRequestor {
 public:
  nsManifestCheck(nsOfflineCacheUpdate* aUpdate, nsIURI* aURI,
                  nsIURI* aReferrerURI, nsIPrincipal* aLoadingPrincipal)
      : mUpdate(aUpdate),
        mURI(aURI),
        mReferrerURI(aReferrerURI),
        mLoadingPrincipal(aLoadingPrincipal) {}

  NS_DECL_ISUPPORTS
  NS_DECL_NSIREQUESTOBSERVER
  NS_DECL_NSISTREAMLISTENER
  NS_DECL_NSICHANNELEVENTSINK
  NS_DECL_NSIINTERFACEREQUESTOR

  nsresult Begin();

 private:
  ~nsManifestCheck() {}

  static nsresult ReadManifest(nsIInputStream* aInputStream, void* aClosure,
                               const char* aFromSegment, uint32_t aOffset,
                               uint32_t aCount, uint32_t* aBytesConsumed);

  RefPtr<nsOfflineCacheUpdate> mUpdate;
  nsCOMPtr<nsIURI> mURI;
  nsCOMPtr<nsIURI> mReferrerURI;
  nsCOMPtr<nsIPrincipal> mLoadingPrincipal;
  nsCOMPtr<nsICryptoHash> mManifestHash;
  nsCOMPtr<nsIChannel> mChannel;
>>>>>>> upstream-releases
};

//-----------------------------------------------------------------------------
// nsManifestCheck::nsISupports
//-----------------------------------------------------------------------------
NS_IMPL_ISUPPORTS(nsManifestCheck, nsIRequestObserver, nsIStreamListener,
                  nsIChannelEventSink, nsIInterfaceRequestor)

//-----------------------------------------------------------------------------
// nsManifestCheck <public>
//-----------------------------------------------------------------------------

nsresult nsManifestCheck::Begin() {
  nsresult rv;
  mManifestHash = do_CreateInstance("@mozilla.org/security/hash;1", &rv);
  NS_ENSURE_SUCCESS(rv, rv);

<<<<<<< HEAD
  rv = mManifestHash->Init(nsICryptoHash::MD5);
  NS_ENSURE_SUCCESS(rv, rv);
  rv = NS_NewChannel(getter_AddRefs(mChannel), mURI, mLoadingPrincipal,
                     nsILoadInfo::SEC_REQUIRE_SAME_ORIGIN_DATA_IS_BLOCKED,
                     nsIContentPolicy::TYPE_OTHER,
                     nullptr,  // PerformanceStorage
                     nullptr,  // loadGroup
                     nullptr,  // aCallbacks
                     nsIRequest::LOAD_BYPASS_CACHE);
||||||| merged common ancestors
    rv = mManifestHash->Init(nsICryptoHash::MD5);
    NS_ENSURE_SUCCESS(rv, rv);
    rv = NS_NewChannel(getter_AddRefs(mChannel),
                       mURI,
                       mLoadingPrincipal,
                       nsILoadInfo::SEC_REQUIRE_SAME_ORIGIN_DATA_IS_BLOCKED,
                       nsIContentPolicy::TYPE_OTHER,
                       nullptr,   // PerformanceStorage
                       nullptr,   // loadGroup
                       nullptr,   // aCallbacks
                       nsIRequest::LOAD_BYPASS_CACHE);
=======
  rv = mManifestHash->Init(nsICryptoHash::MD5);
  NS_ENSURE_SUCCESS(rv, rv);
  rv = NS_NewChannel(getter_AddRefs(mChannel), mURI, mLoadingPrincipal,
                     nsILoadInfo::SEC_REQUIRE_SAME_ORIGIN_DATA_IS_BLOCKED,
                     nsIContentPolicy::TYPE_OTHER,
                     nullptr,  // nsICookieSettings
                     nullptr,  // PerformanceStorage
                     nullptr,  // loadGroup
                     nullptr,  // aCallbacks
                     nsIRequest::LOAD_BYPASS_CACHE);
>>>>>>> upstream-releases

  NS_ENSURE_SUCCESS(rv, rv);

<<<<<<< HEAD
  // configure HTTP specific stuff
  nsCOMPtr<nsIHttpChannel> httpChannel = do_QueryInterface(mChannel);
  if (httpChannel) {
    rv = httpChannel->SetReferrer(mReferrerURI);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    rv = httpChannel->SetRequestHeader(NS_LITERAL_CSTRING("X-Moz"),
                                       NS_LITERAL_CSTRING("offline-resource"),
                                       false);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
  }
||||||| merged common ancestors
    // configure HTTP specific stuff
    nsCOMPtr<nsIHttpChannel> httpChannel =
        do_QueryInterface(mChannel);
    if (httpChannel) {
        rv = httpChannel->SetReferrer(mReferrerURI);
        MOZ_ASSERT(NS_SUCCEEDED(rv));
        rv = httpChannel->SetRequestHeader(NS_LITERAL_CSTRING("X-Moz"),
                                           NS_LITERAL_CSTRING("offline-resource"),
                                           false);
        MOZ_ASSERT(NS_SUCCEEDED(rv));
    }
=======
  // configure HTTP specific stuff
  nsCOMPtr<nsIHttpChannel> httpChannel = do_QueryInterface(mChannel);
  if (httpChannel) {
    nsCOMPtr<nsIReferrerInfo> referrerInfo =
        new mozilla::dom::ReferrerInfo(mReferrerURI);
    rv = httpChannel->SetReferrerInfoWithoutClone(referrerInfo);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    rv = httpChannel->SetRequestHeader(NS_LITERAL_CSTRING("X-Moz"),
                                       NS_LITERAL_CSTRING("offline-resource"),
                                       false);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return mChannel->AsyncOpen2(this);
||||||| merged common ancestors
    return mChannel->AsyncOpen2(this);
=======
  return mChannel->AsyncOpen(this);
>>>>>>> upstream-releases
}

//-----------------------------------------------------------------------------
// nsManifestCheck <public>
//-----------------------------------------------------------------------------

<<<<<<< HEAD
/* static */ nsresult nsManifestCheck::ReadManifest(
    nsIInputStream *aInputStream, void *aClosure, const char *aFromSegment,
    uint32_t aOffset, uint32_t aCount, uint32_t *aBytesConsumed) {
  nsManifestCheck *manifestCheck = static_cast<nsManifestCheck *>(aClosure);
||||||| merged common ancestors
/* static */ nsresult
nsManifestCheck::ReadManifest(nsIInputStream *aInputStream,
                              void *aClosure,
                              const char *aFromSegment,
                              uint32_t aOffset,
                              uint32_t aCount,
                              uint32_t *aBytesConsumed)
{
    nsManifestCheck *manifestCheck =
        static_cast<nsManifestCheck*>(aClosure);
=======
/* static */
nsresult nsManifestCheck::ReadManifest(nsIInputStream* aInputStream,
                                       void* aClosure, const char* aFromSegment,
                                       uint32_t aOffset, uint32_t aCount,
                                       uint32_t* aBytesConsumed) {
  nsManifestCheck* manifestCheck = static_cast<nsManifestCheck*>(aClosure);
>>>>>>> upstream-releases

  nsresult rv;
  *aBytesConsumed = aCount;

<<<<<<< HEAD
  rv = manifestCheck->mManifestHash->Update(
      reinterpret_cast<const uint8_t *>(aFromSegment), aCount);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
    rv = manifestCheck->mManifestHash->Update(
        reinterpret_cast<const uint8_t *>(aFromSegment), aCount);
    NS_ENSURE_SUCCESS(rv, rv);
=======
  rv = manifestCheck->mManifestHash->Update(
      reinterpret_cast<const uint8_t*>(aFromSegment), aCount);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

  return NS_OK;
}

//-----------------------------------------------------------------------------
// nsManifestCheck::nsIStreamListener
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
nsManifestCheck::OnStartRequest(nsIRequest *aRequest, nsISupports *aContext) {
  return NS_OK;
}
||||||| merged common ancestors
nsManifestCheck::OnStartRequest(nsIRequest *aRequest,
                                nsISupports *aContext)
{
    return NS_OK;
}
=======
nsManifestCheck::OnStartRequest(nsIRequest* aRequest) { return NS_OK; }
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
nsManifestCheck::OnDataAvailable(nsIRequest *aRequest, nsISupports *aContext,
                                 nsIInputStream *aStream, uint64_t aOffset,
                                 uint32_t aCount) {
  uint32_t bytesRead;
  aStream->ReadSegments(ReadManifest, this, aCount, &bytesRead);
  return NS_OK;
||||||| merged common ancestors
nsManifestCheck::OnDataAvailable(nsIRequest *aRequest,
                                 nsISupports *aContext,
                                 nsIInputStream *aStream,
                                 uint64_t aOffset,
                                 uint32_t aCount)
{
    uint32_t bytesRead;
    aStream->ReadSegments(ReadManifest, this, aCount, &bytesRead);
    return NS_OK;
=======
nsManifestCheck::OnDataAvailable(nsIRequest* aRequest, nsIInputStream* aStream,
                                 uint64_t aOffset, uint32_t aCount) {
  uint32_t bytesRead;
  aStream->ReadSegments(ReadManifest, this, aCount, &bytesRead);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsManifestCheck::OnStopRequest(nsIRequest *aRequest, nsISupports *aContext,
                               nsresult aStatus) {
  nsAutoCString manifestHash;
  if (NS_SUCCEEDED(aStatus)) {
    mManifestHash->Finish(true, manifestHash);
  }
||||||| merged common ancestors
nsManifestCheck::OnStopRequest(nsIRequest *aRequest,
                               nsISupports *aContext,
                               nsresult aStatus)
{
    nsAutoCString manifestHash;
    if (NS_SUCCEEDED(aStatus)) {
        mManifestHash->Finish(true, manifestHash);
    }
=======
nsManifestCheck::OnStopRequest(nsIRequest* aRequest, nsresult aStatus) {
  nsAutoCString manifestHash;
  if (NS_SUCCEEDED(aStatus)) {
    mManifestHash->Finish(true, manifestHash);
  }
>>>>>>> upstream-releases

  mUpdate->ManifestCheckCompleted(aStatus, manifestHash);

  return NS_OK;
}

//-----------------------------------------------------------------------------
// nsManifestCheck::nsIInterfaceRequestor
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
nsManifestCheck::GetInterface(const nsIID &aIID, void **aResult) {
  if (aIID.Equals(NS_GET_IID(nsIChannelEventSink))) {
    NS_ADDREF_THIS();
    *aResult = static_cast<nsIChannelEventSink *>(this);
    return NS_OK;
  }
||||||| merged common ancestors
nsManifestCheck::GetInterface(const nsIID &aIID, void **aResult)
{
    if (aIID.Equals(NS_GET_IID(nsIChannelEventSink))) {
        NS_ADDREF_THIS();
        *aResult = static_cast<nsIChannelEventSink *>(this);
        return NS_OK;
    }
=======
nsManifestCheck::GetInterface(const nsIID& aIID, void** aResult) {
  if (aIID.Equals(NS_GET_IID(nsIChannelEventSink))) {
    NS_ADDREF_THIS();
    *aResult = static_cast<nsIChannelEventSink*>(this);
    return NS_OK;
  }
>>>>>>> upstream-releases

  return NS_ERROR_NO_INTERFACE;
}

//-----------------------------------------------------------------------------
// nsManifestCheck::nsIChannelEventSink
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
nsManifestCheck::AsyncOnChannelRedirect(
    nsIChannel *aOldChannel, nsIChannel *aNewChannel, uint32_t aFlags,
    nsIAsyncVerifyRedirectCallback *callback) {
  // Redirects should cause the load (and therefore the update) to fail.
  if (aFlags & nsIChannelEventSink::REDIRECT_INTERNAL) {
    callback->OnRedirectVerifyCallback(NS_OK);
    return NS_OK;
  }
||||||| merged common ancestors
nsManifestCheck::AsyncOnChannelRedirect(nsIChannel *aOldChannel,
                                        nsIChannel *aNewChannel,
                                        uint32_t aFlags,
                                        nsIAsyncVerifyRedirectCallback *callback)
{
    // Redirects should cause the load (and therefore the update) to fail.
    if (aFlags & nsIChannelEventSink::REDIRECT_INTERNAL) {
        callback->OnRedirectVerifyCallback(NS_OK);
        return NS_OK;
    }
=======
nsManifestCheck::AsyncOnChannelRedirect(
    nsIChannel* aOldChannel, nsIChannel* aNewChannel, uint32_t aFlags,
    nsIAsyncVerifyRedirectCallback* callback) {
  // Redirects should cause the load (and therefore the update) to fail.
  if (aFlags & nsIChannelEventSink::REDIRECT_INTERNAL) {
    callback->OnRedirectVerifyCallback(NS_OK);
    return NS_OK;
  }
>>>>>>> upstream-releases

  LogToConsole("Manifest check failed because its response is a redirect");

  aOldChannel->Cancel(NS_ERROR_ABORT);
  return NS_ERROR_ABORT;
}

//-----------------------------------------------------------------------------
// nsOfflineCacheUpdateItem::nsISupports
//-----------------------------------------------------------------------------

NS_IMPL_ISUPPORTS(nsOfflineCacheUpdateItem, nsIRequestObserver,
                  nsIStreamListener, nsIRunnable, nsIInterfaceRequestor,
                  nsIChannelEventSink)

//-----------------------------------------------------------------------------
// nsOfflineCacheUpdateItem <public>
//-----------------------------------------------------------------------------

<<<<<<< HEAD
nsOfflineCacheUpdateItem::nsOfflineCacheUpdateItem(
    nsIURI *aURI, nsIURI *aReferrerURI, nsIPrincipal *aLoadingPrincipal,
    nsIApplicationCache *aApplicationCache,
    nsIApplicationCache *aPreviousApplicationCache, uint32_t type,
    uint32_t loadFlags)
    : mURI(aURI),
      mReferrerURI(aReferrerURI),
      mLoadingPrincipal(aLoadingPrincipal),
      mApplicationCache(aApplicationCache),
      mPreviousApplicationCache(aPreviousApplicationCache),
      mItemType(type),
      mLoadFlags(loadFlags),
      mChannel(nullptr),
      mState(LoadStatus::UNINITIALIZED),
      mBytesRead(0) {}

nsOfflineCacheUpdateItem::~nsOfflineCacheUpdateItem() {}

nsresult nsOfflineCacheUpdateItem::OpenChannel(nsOfflineCacheUpdate *aUpdate) {
  if (LOG_ENABLED()) {
    LOG(("%p: Opening channel for %s", this, mURI->GetSpecOrDefault().get()));
  }
||||||| merged common ancestors
nsOfflineCacheUpdateItem::nsOfflineCacheUpdateItem(nsIURI *aURI,
                                                   nsIURI *aReferrerURI,
                                                   nsIPrincipal* aLoadingPrincipal,
                                                   nsIApplicationCache *aApplicationCache,
                                                   nsIApplicationCache *aPreviousApplicationCache,
                                                   uint32_t type,
                                                   uint32_t loadFlags)
    : mURI(aURI)
    , mReferrerURI(aReferrerURI)
    , mLoadingPrincipal(aLoadingPrincipal)
    , mApplicationCache(aApplicationCache)
    , mPreviousApplicationCache(aPreviousApplicationCache)
    , mItemType(type)
    , mLoadFlags(loadFlags)
    , mChannel(nullptr)
    , mState(LoadStatus::UNINITIALIZED)
    , mBytesRead(0)
{
}

nsOfflineCacheUpdateItem::~nsOfflineCacheUpdateItem()
{
}

nsresult
nsOfflineCacheUpdateItem::OpenChannel(nsOfflineCacheUpdate *aUpdate)
{
    if (LOG_ENABLED()) {
        LOG(("%p: Opening channel for %s", this,
             mURI->GetSpecOrDefault().get()));
    }
=======
nsOfflineCacheUpdateItem::nsOfflineCacheUpdateItem(
    nsIURI* aURI, nsIURI* aReferrerURI, nsIPrincipal* aLoadingPrincipal,
    nsIApplicationCache* aApplicationCache,
    nsIApplicationCache* aPreviousApplicationCache, uint32_t type,
    uint32_t loadFlags)
    : mURI(aURI),
      mReferrerURI(aReferrerURI),
      mLoadingPrincipal(aLoadingPrincipal),
      mApplicationCache(aApplicationCache),
      mPreviousApplicationCache(aPreviousApplicationCache),
      mItemType(type),
      mLoadFlags(loadFlags),
      mChannel(nullptr),
      mState(LoadStatus::UNINITIALIZED),
      mBytesRead(0) {}

nsOfflineCacheUpdateItem::~nsOfflineCacheUpdateItem() {}

nsresult nsOfflineCacheUpdateItem::OpenChannel(nsOfflineCacheUpdate* aUpdate) {
  if (LOG_ENABLED()) {
    LOG(("%p: Opening channel for %s", this, mURI->GetSpecOrDefault().get()));
  }
>>>>>>> upstream-releases

  if (mUpdate) {
    // Holding a reference to the update means this item is already
    // in progress (has a channel, or is just in between OnStopRequest()
    // and its Run() call.  We must never open channel on this item again.
    LOG(("  %p is already running! ignoring", this));
    return NS_ERROR_ALREADY_OPENED;
  }

  nsresult rv = nsOfflineCacheUpdate::GetCacheKey(mURI, mCacheKey);
  NS_ENSURE_SUCCESS(rv, rv);

  uint32_t flags =
      nsIRequest::LOAD_BACKGROUND | nsICachingChannel::LOAD_ONLY_IF_MODIFIED;

  if (mApplicationCache == mPreviousApplicationCache) {
    // Same app cache to read from and to write to is used during
    // an only-update-check procedure.  Here we protect the existing
    // cache from being modified.
    flags |= nsIRequest::INHIBIT_CACHING;
  }

  flags |= mLoadFlags;

<<<<<<< HEAD
  rv = NS_NewChannel(getter_AddRefs(mChannel), mURI, mLoadingPrincipal,
                     nsILoadInfo::SEC_ALLOW_CROSS_ORIGIN_DATA_IS_NULL,
                     nsIContentPolicy::TYPE_OTHER,
                     nullptr,  // PerformanceStorage
                     nullptr,  // aLoadGroup
                     this,     // aCallbacks
                     flags);
||||||| merged common ancestors
    rv = NS_NewChannel(getter_AddRefs(mChannel),
                       mURI,
                       mLoadingPrincipal,
                       nsILoadInfo::SEC_ALLOW_CROSS_ORIGIN_DATA_IS_NULL,
                       nsIContentPolicy::TYPE_OTHER,
                       nullptr,   // PerformanceStorage
                       nullptr,  // aLoadGroup
                       this,     // aCallbacks
                       flags);
=======
  rv = NS_NewChannel(getter_AddRefs(mChannel), mURI, mLoadingPrincipal,
                     nsILoadInfo::SEC_ALLOW_CROSS_ORIGIN_DATA_IS_NULL,
                     nsIContentPolicy::TYPE_OTHER,
                     nullptr,  // nsICookieSettings
                     nullptr,  // PerformanceStorage
                     nullptr,  // aLoadGroup
                     this,     // aCallbacks
                     flags);
>>>>>>> upstream-releases

  NS_ENSURE_SUCCESS(rv, rv);

  nsCOMPtr<nsIApplicationCacheChannel> appCacheChannel =
      do_QueryInterface(mChannel, &rv);

  // Support for nsIApplicationCacheChannel is required.
  NS_ENSURE_SUCCESS(rv, rv);

  // Use the existing application cache as the cache to check.
  rv = appCacheChannel->SetApplicationCache(mPreviousApplicationCache);
  NS_ENSURE_SUCCESS(rv, rv);

  // Set the new application cache as the target for write.
  rv = appCacheChannel->SetApplicationCacheForWrite(mApplicationCache);
  NS_ENSURE_SUCCESS(rv, rv);

<<<<<<< HEAD
  // configure HTTP specific stuff
  nsCOMPtr<nsIHttpChannel> httpChannel = do_QueryInterface(mChannel);
  if (httpChannel) {
    rv = httpChannel->SetReferrer(mReferrerURI);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    rv = httpChannel->SetRequestHeader(NS_LITERAL_CSTRING("X-Moz"),
                                       NS_LITERAL_CSTRING("offline-resource"),
                                       false);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
  }
||||||| merged common ancestors
    // configure HTTP specific stuff
    nsCOMPtr<nsIHttpChannel> httpChannel =
        do_QueryInterface(mChannel);
    if (httpChannel) {
        rv = httpChannel->SetReferrer(mReferrerURI);
        MOZ_ASSERT(NS_SUCCEEDED(rv));
        rv = httpChannel->SetRequestHeader(NS_LITERAL_CSTRING("X-Moz"),
                                           NS_LITERAL_CSTRING("offline-resource"),
                                           false);
        MOZ_ASSERT(NS_SUCCEEDED(rv));
    }
=======
  // configure HTTP specific stuff
  nsCOMPtr<nsIHttpChannel> httpChannel = do_QueryInterface(mChannel);
  if (httpChannel) {
    nsCOMPtr<nsIReferrerInfo> referrerInfo =
        new mozilla::dom::ReferrerInfo(mReferrerURI);
    rv = httpChannel->SetReferrerInfoWithoutClone(referrerInfo);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    rv = httpChannel->SetRequestHeader(NS_LITERAL_CSTRING("X-Moz"),
                                       NS_LITERAL_CSTRING("offline-resource"),
                                       false);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  rv = mChannel->AsyncOpen2(this);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
    rv = mChannel->AsyncOpen2(this);
    NS_ENSURE_SUCCESS(rv, rv);
=======
  rv = mChannel->AsyncOpen(this);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

  mUpdate = aUpdate;

  mState = LoadStatus::REQUESTED;

  return NS_OK;
}

nsresult nsOfflineCacheUpdateItem::Cancel() {
  if (mChannel) {
    mChannel->Cancel(NS_ERROR_ABORT);
    mChannel = nullptr;
  }

  mState = LoadStatus::UNINITIALIZED;

  return NS_OK;
}

//-----------------------------------------------------------------------------
// nsOfflineCacheUpdateItem::nsIStreamListener
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdateItem::OnStartRequest(nsIRequest *aRequest,
                                         nsISupports *aContext) {
  mState = LoadStatus::RECEIVING;
||||||| merged common ancestors
nsOfflineCacheUpdateItem::OnStartRequest(nsIRequest *aRequest,
                                         nsISupports *aContext)
{
    mState = LoadStatus::RECEIVING;
=======
nsOfflineCacheUpdateItem::OnStartRequest(nsIRequest* aRequest) {
  mState = LoadStatus::RECEIVING;
>>>>>>> upstream-releases

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdateItem::OnDataAvailable(nsIRequest *aRequest,
                                          nsISupports *aContext,
                                          nsIInputStream *aStream,
                                          uint64_t aOffset, uint32_t aCount) {
  uint32_t bytesRead = 0;
  aStream->ReadSegments(NS_DiscardSegment, nullptr, aCount, &bytesRead);
  mBytesRead += bytesRead;
  LOG(("loaded %u bytes into offline cache [offset=%" PRIu64 "]\n", bytesRead,
       aOffset));
||||||| merged common ancestors
nsOfflineCacheUpdateItem::OnDataAvailable(nsIRequest *aRequest,
                                          nsISupports *aContext,
                                          nsIInputStream *aStream,
                                          uint64_t aOffset,
                                          uint32_t aCount)
{
    uint32_t bytesRead = 0;
    aStream->ReadSegments(NS_DiscardSegment, nullptr, aCount, &bytesRead);
    mBytesRead += bytesRead;
    LOG(("loaded %u bytes into offline cache [offset=%" PRIu64 "]\n",
         bytesRead, aOffset));
=======
nsOfflineCacheUpdateItem::OnDataAvailable(nsIRequest* aRequest,
                                          nsIInputStream* aStream,
                                          uint64_t aOffset, uint32_t aCount) {
  uint32_t bytesRead = 0;
  aStream->ReadSegments(NS_DiscardSegment, nullptr, aCount, &bytesRead);
  mBytesRead += bytesRead;
  LOG(("loaded %u bytes into offline cache [offset=%" PRIu64 "]\n", bytesRead,
       aOffset));
>>>>>>> upstream-releases

  mUpdate->OnByteProgress(bytesRead);

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdateItem::OnStopRequest(nsIRequest *aRequest,
                                        nsISupports *aContext,
                                        nsresult aStatus) {
  if (LOG_ENABLED()) {
    LOG(("%p: Done fetching offline item %s [status=%" PRIx32 "]\n", this,
         mURI->GetSpecOrDefault().get(), static_cast<uint32_t>(aStatus)));
  }
||||||| merged common ancestors
nsOfflineCacheUpdateItem::OnStopRequest(nsIRequest *aRequest,
                                        nsISupports *aContext,
                                        nsresult aStatus)
{
    if (LOG_ENABLED()) {
        LOG(("%p: Done fetching offline item %s [status=%" PRIx32 "]\n",
             this, mURI->GetSpecOrDefault().get(), static_cast<uint32_t>(aStatus)));
    }
=======
nsOfflineCacheUpdateItem::OnStopRequest(nsIRequest* aRequest,
                                        nsresult aStatus) {
  if (LOG_ENABLED()) {
    LOG(("%p: Done fetching offline item %s [status=%" PRIx32 "]\n", this,
         mURI->GetSpecOrDefault().get(), static_cast<uint32_t>(aStatus)));
  }
>>>>>>> upstream-releases

  if (mBytesRead == 0 && aStatus == NS_OK) {
    // we didn't need to read (because LOAD_ONLY_IF_MODIFIED was
    // specified), but the object should report loadedSize as if it
    // did.
    mChannel->GetContentLength(&mBytesRead);
    mUpdate->OnByteProgress(mBytesRead);
  }

  if (NS_FAILED(aStatus)) {
    nsCOMPtr<nsIHttpChannel> httpChannel = do_QueryInterface(mChannel);
    if (httpChannel) {
      bool isNoStore;
      if (NS_SUCCEEDED(httpChannel->IsNoStoreResponse(&isNoStore)) &&
          isNoStore) {
        LogToConsole(
            "Offline cache manifest item has Cache-control: no-store header",
            this);
      }
    }
  }

  // We need to notify the update that the load is complete, but we
  // want to give the channel a chance to close the cache entries.
  NS_DispatchToCurrentThread(this);

  return NS_OK;
}

//-----------------------------------------------------------------------------
// nsOfflineCacheUpdateItem::nsIRunnable
//-----------------------------------------------------------------------------
NS_IMETHODIMP
nsOfflineCacheUpdateItem::Run() {
  // Set mState to LOADED here rather than in OnStopRequest to prevent
  // race condition when checking state of all mItems in ProcessNextURI().
  // If state would have been set in OnStopRequest we could mistakenly
  // take this item as already finished and finish the update process too
  // early when ProcessNextURI() would get called between OnStopRequest()
  // and Run() of this item.  Finish() would then have been called twice.
  mState = LoadStatus::LOADED;

  RefPtr<nsOfflineCacheUpdate> update;
  update.swap(mUpdate);
  update->LoadCompleted(this);

  return NS_OK;
}

//-----------------------------------------------------------------------------
// nsOfflineCacheUpdateItem::nsIInterfaceRequestor
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdateItem::GetInterface(const nsIID &aIID, void **aResult) {
  if (aIID.Equals(NS_GET_IID(nsIChannelEventSink))) {
    NS_ADDREF_THIS();
    *aResult = static_cast<nsIChannelEventSink *>(this);
    return NS_OK;
  }
||||||| merged common ancestors
nsOfflineCacheUpdateItem::GetInterface(const nsIID &aIID, void **aResult)
{
    if (aIID.Equals(NS_GET_IID(nsIChannelEventSink))) {
        NS_ADDREF_THIS();
        *aResult = static_cast<nsIChannelEventSink *>(this);
        return NS_OK;
    }
=======
nsOfflineCacheUpdateItem::GetInterface(const nsIID& aIID, void** aResult) {
  if (aIID.Equals(NS_GET_IID(nsIChannelEventSink))) {
    NS_ADDREF_THIS();
    *aResult = static_cast<nsIChannelEventSink*>(this);
    return NS_OK;
  }
>>>>>>> upstream-releases

  return NS_ERROR_NO_INTERFACE;
}

//-----------------------------------------------------------------------------
// nsOfflineCacheUpdateItem::nsIChannelEventSink
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdateItem::AsyncOnChannelRedirect(
    nsIChannel *aOldChannel, nsIChannel *aNewChannel, uint32_t aFlags,
    nsIAsyncVerifyRedirectCallback *cb) {
  if (!(aFlags & nsIChannelEventSink::REDIRECT_INTERNAL)) {
    // Don't allow redirect in case of non-internal redirect and cancel
    // the channel to clean the cache entry.
    LogToConsole("Offline cache manifest failed because an item redirects",
                 this);
||||||| merged common ancestors
nsOfflineCacheUpdateItem::AsyncOnChannelRedirect(nsIChannel *aOldChannel,
                                                 nsIChannel *aNewChannel,
                                                 uint32_t aFlags,
                                                 nsIAsyncVerifyRedirectCallback *cb)
{
    if (!(aFlags & nsIChannelEventSink::REDIRECT_INTERNAL)) {
        // Don't allow redirect in case of non-internal redirect and cancel
        // the channel to clean the cache entry.
        LogToConsole("Offline cache manifest failed because an item redirects", this);
=======
nsOfflineCacheUpdateItem::AsyncOnChannelRedirect(
    nsIChannel* aOldChannel, nsIChannel* aNewChannel, uint32_t aFlags,
    nsIAsyncVerifyRedirectCallback* cb) {
  if (!(aFlags & nsIChannelEventSink::REDIRECT_INTERNAL)) {
    // Don't allow redirect in case of non-internal redirect and cancel
    // the channel to clean the cache entry.
    LogToConsole("Offline cache manifest failed because an item redirects",
                 this);
>>>>>>> upstream-releases

    aOldChannel->Cancel(NS_ERROR_ABORT);
    return NS_ERROR_ABORT;
  }

  nsCOMPtr<nsIURI> newURI;
  nsresult rv = aNewChannel->GetURI(getter_AddRefs(newURI));
  if (NS_FAILED(rv)) return rv;

  nsCOMPtr<nsIApplicationCacheChannel> appCacheChannel =
      do_QueryInterface(aNewChannel);
  if (appCacheChannel) {
    rv = appCacheChannel->SetApplicationCacheForWrite(mApplicationCache);
    NS_ENSURE_SUCCESS(rv, rv);
  }

  nsAutoCString oldScheme;
  mURI->GetScheme(oldScheme);

  bool match;
  if (NS_FAILED(newURI->SchemeIs(oldScheme.get(), &match)) || !match) {
    LOG(("rejected: redirected to a different scheme\n"));
    return NS_ERROR_ABORT;
  }

  // HTTP request headers are not automatically forwarded to the new channel.
  nsCOMPtr<nsIHttpChannel> httpChannel = do_QueryInterface(aNewChannel);
  NS_ENSURE_STATE(httpChannel);

  rv = httpChannel->SetRequestHeader(NS_LITERAL_CSTRING("X-Moz"),
                                     NS_LITERAL_CSTRING("offline-resource"),
                                     false);
  MOZ_ASSERT(NS_SUCCEEDED(rv));

  mChannel = aNewChannel;

  cb->OnRedirectVerifyCallback(NS_OK);
  return NS_OK;
}

<<<<<<< HEAD
nsresult nsOfflineCacheUpdateItem::GetRequestSucceeded(bool *succeeded) {
  *succeeded = false;
||||||| merged common ancestors
nsresult
nsOfflineCacheUpdateItem::GetRequestSucceeded(bool * succeeded)
{
    *succeeded = false;

    if (!mChannel)
        return NS_OK;
=======
nsresult nsOfflineCacheUpdateItem::GetRequestSucceeded(bool* succeeded) {
  *succeeded = false;
>>>>>>> upstream-releases

  if (!mChannel) return NS_OK;

  nsresult rv;
  nsCOMPtr<nsIHttpChannel> httpChannel = do_QueryInterface(mChannel, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  bool reqSucceeded;
  rv = httpChannel->GetRequestSucceeded(&reqSucceeded);
  if (NS_ERROR_NOT_AVAILABLE == rv) return NS_OK;
  NS_ENSURE_SUCCESS(rv, rv);

  if (!reqSucceeded) {
    LOG(("Request failed"));
    return NS_OK;
  }

  nsresult channelStatus;
  rv = httpChannel->GetStatus(&channelStatus);
  NS_ENSURE_SUCCESS(rv, rv);

  if (NS_FAILED(channelStatus)) {
    LOG(("Channel status=0x%08" PRIx32, static_cast<uint32_t>(channelStatus)));
    return NS_OK;
  }

  *succeeded = true;
  return NS_OK;
}

bool nsOfflineCacheUpdateItem::IsScheduled() {
  return mState == LoadStatus::UNINITIALIZED;
}

bool nsOfflineCacheUpdateItem::IsInProgress() {
  return mState == LoadStatus::REQUESTED || mState == LoadStatus::RECEIVING;
}

bool nsOfflineCacheUpdateItem::IsCompleted() {
  return mState == LoadStatus::LOADED;
}

<<<<<<< HEAD
nsresult nsOfflineCacheUpdateItem::GetStatus(uint16_t *aStatus) {
  if (!mChannel) {
    *aStatus = 0;
    return NS_OK;
  }
||||||| merged common ancestors
    nsresult rv;
    nsCOMPtr<nsIHttpChannel> httpChannel = do_QueryInterface(mChannel, &rv);
    NS_ENSURE_SUCCESS(rv, rv);
=======
nsresult nsOfflineCacheUpdateItem::GetStatus(uint16_t* aStatus) {
  if (!mChannel) {
    *aStatus = 0;
    return NS_OK;
  }
>>>>>>> upstream-releases

  nsresult rv;
  nsCOMPtr<nsIHttpChannel> httpChannel = do_QueryInterface(mChannel, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  uint32_t httpStatus;
  rv = httpChannel->GetResponseStatus(&httpStatus);
  if (rv == NS_ERROR_NOT_AVAILABLE) {
    *aStatus = 0;
    return NS_OK;
  }

  NS_ENSURE_SUCCESS(rv, rv);
  *aStatus = uint16_t(httpStatus);
  return NS_OK;
}

//-----------------------------------------------------------------------------
// nsOfflineManifestItem
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// nsOfflineManifestItem <public>
//-----------------------------------------------------------------------------

<<<<<<< HEAD
nsOfflineManifestItem::nsOfflineManifestItem(
    nsIURI *aURI, nsIURI *aReferrerURI, nsIPrincipal *aLoadingPrincipal,
    nsIApplicationCache *aApplicationCache,
    nsIApplicationCache *aPreviousApplicationCache)
||||||| merged common ancestors
nsOfflineManifestItem::nsOfflineManifestItem(nsIURI *aURI,
                                             nsIURI *aReferrerURI,
                                             nsIPrincipal* aLoadingPrincipal,
                                             nsIApplicationCache *aApplicationCache,
                                             nsIApplicationCache *aPreviousApplicationCache)
=======
nsOfflineManifestItem::nsOfflineManifestItem(
    nsIURI* aURI, nsIURI* aReferrerURI, nsIPrincipal* aLoadingPrincipal,
    nsIApplicationCache* aApplicationCache,
    nsIApplicationCache* aPreviousApplicationCache)
>>>>>>> upstream-releases
    : nsOfflineCacheUpdateItem(aURI, aReferrerURI, aLoadingPrincipal,
                               aApplicationCache, aPreviousApplicationCache,
                               nsIApplicationCache::ITEM_MANIFEST, 0),
      mParserState(PARSE_INIT),
      mNeedsUpdate(true),
      mStrictFileOriginPolicy(false),
      mManifestHashInitialized(false) {
  ReadStrictFileOriginPolicyPref();
}

nsOfflineManifestItem::~nsOfflineManifestItem() {}

//-----------------------------------------------------------------------------
// nsOfflineManifestItem <private>
//-----------------------------------------------------------------------------

/* static */
<<<<<<< HEAD
nsresult nsOfflineManifestItem::ReadManifest(nsIInputStream *aInputStream,
                                             void *aClosure,
                                             const char *aFromSegment,
                                             uint32_t aOffset, uint32_t aCount,
                                             uint32_t *aBytesConsumed) {
  nsOfflineManifestItem *manifest =
      static_cast<nsOfflineManifestItem *>(aClosure);
||||||| merged common ancestors
nsresult
nsOfflineManifestItem::ReadManifest(nsIInputStream *aInputStream,
                                    void *aClosure,
                                    const char *aFromSegment,
                                    uint32_t aOffset,
                                    uint32_t aCount,
                                    uint32_t *aBytesConsumed)
{
    nsOfflineManifestItem *manifest =
        static_cast<nsOfflineManifestItem*>(aClosure);
=======
nsresult nsOfflineManifestItem::ReadManifest(nsIInputStream* aInputStream,
                                             void* aClosure,
                                             const char* aFromSegment,
                                             uint32_t aOffset, uint32_t aCount,
                                             uint32_t* aBytesConsumed) {
  nsOfflineManifestItem* manifest =
      static_cast<nsOfflineManifestItem*>(aClosure);
>>>>>>> upstream-releases

  nsresult rv;

  *aBytesConsumed = aCount;

  if (manifest->mParserState == PARSE_ERROR) {
    // parse already failed, ignore this
    return NS_OK;
  }

  if (!manifest->mManifestHashInitialized) {
    // Avoid re-creation of crypto hash when it fails from some reason the first
    // time
    manifest->mManifestHashInitialized = true;

    manifest->mManifestHash =
        do_CreateInstance("@mozilla.org/security/hash;1", &rv);
    if (NS_SUCCEEDED(rv)) {
      rv = manifest->mManifestHash->Init(nsICryptoHash::MD5);
      if (NS_FAILED(rv)) {
        manifest->mManifestHash = nullptr;
        LOG(
            ("Could not initialize manifest hash for byte-to-byte check, "
             "rv=%08" PRIx32,
             static_cast<uint32_t>(rv)));
      }
    }
  }

<<<<<<< HEAD
  if (manifest->mManifestHash) {
    rv = manifest->mManifestHash->Update(
        reinterpret_cast<const uint8_t *>(aFromSegment), aCount);
    if (NS_FAILED(rv)) {
      manifest->mManifestHash = nullptr;
      LOG(("Could not update manifest hash, rv=%08" PRIx32,
           static_cast<uint32_t>(rv)));
||||||| merged common ancestors
    if (manifest->mManifestHash) {
        rv = manifest->mManifestHash->Update(reinterpret_cast<const uint8_t *>(aFromSegment), aCount);
        if (NS_FAILED(rv)) {
            manifest->mManifestHash = nullptr;
            LOG(("Could not update manifest hash, rv=%08" PRIx32, static_cast<uint32_t>(rv)));
        }
=======
  if (manifest->mManifestHash) {
    rv = manifest->mManifestHash->Update(
        reinterpret_cast<const uint8_t*>(aFromSegment), aCount);
    if (NS_FAILED(rv)) {
      manifest->mManifestHash = nullptr;
      LOG(("Could not update manifest hash, rv=%08" PRIx32,
           static_cast<uint32_t>(rv)));
>>>>>>> upstream-releases
    }
  }

  manifest->mReadBuf.Append(aFromSegment, aCount);

  nsCString::const_iterator begin, iter, end;
  manifest->mReadBuf.BeginReading(begin);
  manifest->mReadBuf.EndReading(end);

  for (iter = begin; iter != end; iter++) {
    if (*iter == '\r' || *iter == '\n') {
      rv = manifest->HandleManifestLine(begin, iter);

      if (NS_FAILED(rv)) {
        LOG(("HandleManifestLine failed with 0x%08" PRIx32,
             static_cast<uint32_t>(rv)));
        *aBytesConsumed = 0;  // Avoid assertion failure in stream tee
        return NS_ERROR_ABORT;
      }

      begin = iter;
      begin++;
    }
  }

  // any leftovers are saved for next time
  manifest->mReadBuf = Substring(begin, end);

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsOfflineManifestItem::AddNamespace(uint32_t namespaceType,
                                             const nsCString &namespaceSpec,
                                             const nsCString &data)
||||||| merged common ancestors
nsresult
nsOfflineManifestItem::AddNamespace(uint32_t namespaceType,
                                    const nsCString &namespaceSpec,
                                    const nsCString &data)
=======
nsresult nsOfflineManifestItem::AddNamespace(uint32_t namespaceType,
                                             const nsCString& namespaceSpec,
                                             const nsCString& data)
>>>>>>> upstream-releases

{
  nsresult rv;
  if (!mNamespaces) {
    mNamespaces = do_CreateInstance(NS_ARRAY_CONTRACTID, &rv);
    NS_ENSURE_SUCCESS(rv, rv);
  }

  nsCOMPtr<nsIApplicationCacheNamespace> ns = new nsApplicationCacheNamespace();

  rv = ns->Init(namespaceType, namespaceSpec, data);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = mNamespaces->AppendElement(ns);
  NS_ENSURE_SUCCESS(rv, rv);

  return NS_OK;
}

<<<<<<< HEAD
static nsresult GetURIDirectory(nsIURI *uri, nsACString &directory) {
||||||| merged common ancestors
static nsresult
GetURIDirectory(nsIURI* uri, nsACString &directory)
{
=======
static nsresult GetURIDirectory(nsIURI* uri, nsACString& directory) {
>>>>>>> upstream-releases
  nsresult rv;

  nsCOMPtr<nsIURL> url(do_QueryInterface(uri, &rv));
  NS_ENSURE_SUCCESS(rv, rv);

  rv = url->GetDirectory(directory);
  NS_ENSURE_SUCCESS(rv, rv);

  return NS_OK;
}

<<<<<<< HEAD
static nsresult CheckFileContainedInPath(nsIURI *file,
                                         nsACString const &masterDirectory) {
||||||| merged common ancestors
static nsresult
CheckFileContainedInPath(nsIURI* file, nsACString const &masterDirectory)
{
=======
static nsresult CheckFileContainedInPath(nsIURI* file,
                                         nsACString const& masterDirectory) {
>>>>>>> upstream-releases
  nsresult rv;

  nsAutoCString directory;
  rv = GetURIDirectory(file, directory);
  NS_ENSURE_SUCCESS(rv, rv);

  bool contains = StringBeginsWith(directory, masterDirectory);
  if (!contains) {
    return NS_ERROR_DOM_BAD_URI;
  }

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsOfflineManifestItem::HandleManifestLine(
    const nsCString::const_iterator &aBegin,
    const nsCString::const_iterator &aEnd) {
  nsCString::const_iterator begin = aBegin;
  nsCString::const_iterator end = aEnd;
||||||| merged common ancestors
nsresult
nsOfflineManifestItem::HandleManifestLine(const nsCString::const_iterator &aBegin,
                                          const nsCString::const_iterator &aEnd)
{
    nsCString::const_iterator begin = aBegin;
    nsCString::const_iterator end = aEnd;
=======
nsresult nsOfflineManifestItem::HandleManifestLine(
    const nsCString::const_iterator& aBegin,
    const nsCString::const_iterator& aEnd) {
  nsCString::const_iterator begin = aBegin;
  nsCString::const_iterator end = aEnd;
>>>>>>> upstream-releases

  // all lines ignore trailing spaces and tabs
  nsCString::const_iterator last = end;
  --last;
  while (end != begin && (*last == ' ' || *last == '\t')) {
    --end;
    --last;
  }

  if (mParserState == PARSE_INIT) {
    // Allow a UTF-8 BOM
    if (begin != end && static_cast<unsigned char>(*begin) == 0xef) {
      if (++begin == end || static_cast<unsigned char>(*begin) != 0xbb ||
          ++begin == end || static_cast<unsigned char>(*begin) != 0xbf) {
        mParserState = PARSE_ERROR;
        LogToConsole("Offline cache manifest BOM error", this);
        return NS_OK;
      }
      ++begin;
    }

<<<<<<< HEAD
    const nsACString &magic = Substring(begin, end);
||||||| merged common ancestors
        if (!magic.EqualsLiteral("CACHE MANIFEST")) {
            mParserState = PARSE_ERROR;
            LogToConsole("Offline cache manifest magic incorrect", this);
            return NS_OK;
        }
=======
    const nsACString& magic = Substring(begin, end);
>>>>>>> upstream-releases

    if (!magic.EqualsLiteral("CACHE MANIFEST")) {
      mParserState = PARSE_ERROR;
      LogToConsole("Offline cache manifest magic incorrect", this);
      return NS_OK;
    }

    mParserState = PARSE_CACHE_ENTRIES;
    return NS_OK;
  }

  // lines other than the first ignore leading spaces and tabs
  while (begin != end && (*begin == ' ' || *begin == '\t')) begin++;

  // ignore blank lines and comments
  if (begin == end || *begin == '#') return NS_OK;

<<<<<<< HEAD
  const nsACString &line = Substring(begin, end);
||||||| merged common ancestors
    if (line.EqualsLiteral("CACHE:")) {
        mParserState = PARSE_CACHE_ENTRIES;
        return NS_OK;
    }
=======
  const nsACString& line = Substring(begin, end);
>>>>>>> upstream-releases

  if (line.EqualsLiteral("CACHE:")) {
    mParserState = PARSE_CACHE_ENTRIES;
    return NS_OK;
  }

  if (line.EqualsLiteral("FALLBACK:")) {
    mParserState = PARSE_FALLBACK_ENTRIES;
    return NS_OK;
  }

  if (line.EqualsLiteral("NETWORK:")) {
    mParserState = PARSE_BYPASS_ENTRIES;
    return NS_OK;
  }

  // Every other section type we don't know must be silently ignored.
  nsCString::const_iterator lastChar = end;
  if (*(--lastChar) == ':') {
    mParserState = PARSE_UNKNOWN_SECTION;
    return NS_OK;
  }

  nsresult rv;

  switch (mParserState) {
    case PARSE_INIT:
    case PARSE_ERROR: {
      // this should have been dealt with earlier
      return NS_ERROR_FAILURE;
    }

    case PARSE_UNKNOWN_SECTION: {
      // just jump over
      return NS_OK;
    }

    case PARSE_CACHE_ENTRIES: {
      nsCOMPtr<nsIURI> uri;
      rv = NS_NewURI(getter_AddRefs(uri), line, nullptr, mURI);
      if (NS_FAILED(rv)) break;
      if (NS_FAILED(DropReferenceFromURL(uri))) break;

      nsAutoCString scheme;
      uri->GetScheme(scheme);

      // Manifest URIs must have the same scheme as the manifest.
      bool match;
      if (NS_FAILED(mURI->SchemeIs(scheme.get(), &match)) || !match) break;

      mExplicitURIs.AppendObject(uri);

      if (!NS_SecurityCompareURIs(mURI, uri, mStrictFileOriginPolicy)) {
        mAnonymousURIs.AppendObject(uri);
      }

      break;
    }

    case PARSE_FALLBACK_ENTRIES: {
      int32_t separator = line.FindChar(' ');
      if (separator == kNotFound) {
        separator = line.FindChar('\t');
        if (separator == kNotFound) break;
      }

      nsCString namespaceSpec(Substring(line, 0, separator));
      nsCString fallbackSpec(Substring(line, separator + 1));
      namespaceSpec.CompressWhitespace();
      fallbackSpec.CompressWhitespace();

      nsCOMPtr<nsIURI> namespaceURI;
      rv =
          NS_NewURI(getter_AddRefs(namespaceURI), namespaceSpec, nullptr, mURI);
      if (NS_FAILED(rv)) break;
      if (NS_FAILED(DropReferenceFromURL(namespaceURI))) break;
      rv = namespaceURI->GetAsciiSpec(namespaceSpec);
      if (NS_FAILED(rv)) break;

      nsCOMPtr<nsIURI> fallbackURI;
      rv = NS_NewURI(getter_AddRefs(fallbackURI), fallbackSpec, nullptr, mURI);
      if (NS_FAILED(rv)) break;
      if (NS_FAILED(DropReferenceFromURL(fallbackURI))) break;
      rv = fallbackURI->GetAsciiSpec(fallbackSpec);
      if (NS_FAILED(rv)) break;

      // The following set of checks is preventing a website under
      // a subdirectory to add fallback pages for the whole origin
      // (or a parent directory) to prevent fallback attacks.
      nsAutoCString manifestDirectory;
      rv = GetURIDirectory(mURI, manifestDirectory);
      if (NS_FAILED(rv)) {
        break;
      }

      rv = CheckFileContainedInPath(namespaceURI, manifestDirectory);
      if (NS_FAILED(rv)) {
        break;
      }

      rv = CheckFileContainedInPath(fallbackURI, manifestDirectory);
      if (NS_FAILED(rv)) {
        break;
      }

      // Manifest and namespace must be same origin
      if (!NS_SecurityCompareURIs(mURI, namespaceURI, mStrictFileOriginPolicy))
        break;

      // Fallback and namespace must be same origin
      if (!NS_SecurityCompareURIs(namespaceURI, fallbackURI,
                                  mStrictFileOriginPolicy))
        break;

      mFallbackURIs.AppendObject(fallbackURI);

      AddNamespace(nsIApplicationCacheNamespace::NAMESPACE_FALLBACK,
                   namespaceSpec, fallbackSpec);
      break;
    }

    case PARSE_BYPASS_ENTRIES: {
      if (line[0] == '*' &&
          (line.Length() == 1 || line[1] == ' ' || line[1] == '\t')) {
        // '*' indicates to make the online whitelist wildcard flag open,
        // i.e. do allow load of resources not present in the offline cache
        // or not conforming any namespace.
        // We achive that simply by adding an 'empty' - i.e. universal
        // namespace of BYPASS type into the cache.
        AddNamespace(nsIApplicationCacheNamespace::NAMESPACE_BYPASS,
                     EmptyCString(), EmptyCString());
        break;
      }

      nsCOMPtr<nsIURI> bypassURI;
      rv = NS_NewURI(getter_AddRefs(bypassURI), line, nullptr, mURI);
      if (NS_FAILED(rv)) break;

      nsAutoCString scheme;
      bypassURI->GetScheme(scheme);
      bool equals;
      if (NS_FAILED(mURI->SchemeIs(scheme.get(), &equals)) || !equals) break;
      if (NS_FAILED(DropReferenceFromURL(bypassURI))) break;
      nsCString spec;
      if (NS_FAILED(bypassURI->GetAsciiSpec(spec))) break;

      AddNamespace(nsIApplicationCacheNamespace::NAMESPACE_BYPASS, spec,
                   EmptyCString());
      break;
    }
  }

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsOfflineManifestItem::GetOldManifestContentHash(
    nsIRequest *aRequest) {
  nsresult rv;

  nsCOMPtr<nsICachingChannel> cachingChannel = do_QueryInterface(aRequest, &rv);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
nsresult
nsOfflineManifestItem::GetOldManifestContentHash(nsIRequest *aRequest)
{
    nsresult rv;
=======
nsresult nsOfflineManifestItem::GetOldManifestContentHash(
    nsIRequest* aRequest) {
  nsresult rv;

  nsCOMPtr<nsICachingChannel> cachingChannel = do_QueryInterface(aRequest, &rv);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

  // load the main cache token that is actually the old offline cache token and
  // read previous manifest content hash value
  nsCOMPtr<nsISupports> cacheToken;
  cachingChannel->GetCacheToken(getter_AddRefs(cacheToken));
  if (cacheToken) {
    nsCOMPtr<nsICacheEntry> cacheDescriptor(do_QueryInterface(cacheToken, &rv));
    NS_ENSURE_SUCCESS(rv, rv);

    rv = cacheDescriptor->GetMetaDataElement(
        "offline-manifest-hash", getter_Copies(mOldManifestHashValue));
    if (NS_FAILED(rv)) mOldManifestHashValue.Truncate();
  }

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsOfflineManifestItem::CheckNewManifestContentHash(
    nsIRequest *aRequest) {
  nsresult rv;
||||||| merged common ancestors
nsresult
nsOfflineManifestItem::CheckNewManifestContentHash(nsIRequest *aRequest)
{
    nsresult rv;
=======
nsresult nsOfflineManifestItem::CheckNewManifestContentHash(
    nsIRequest* aRequest) {
  nsresult rv;
>>>>>>> upstream-releases

  if (!mManifestHash) {
    // Nothing to compare against...
    return NS_OK;
  }

  nsCString newManifestHashValue;
  rv = mManifestHash->Finish(true, mManifestHashValue);
  mManifestHash = nullptr;

  if (NS_FAILED(rv)) {
    LOG(("Could not finish manifest hash, rv=%08" PRIx32,
         static_cast<uint32_t>(rv)));
    // This is not critical error
    return NS_OK;
  }

  if (!ParseSucceeded()) {
    // Parsing failed, the hash is not valid
    return NS_OK;
  }

  if (mOldManifestHashValue == mManifestHashValue) {
    LOG(
        ("Update not needed, downloaded manifest content is byte-for-byte "
         "identical"));
    mNeedsUpdate = false;
  }

  // Store the manifest content hash value to the new
  // offline cache token
  nsCOMPtr<nsICachingChannel> cachingChannel = do_QueryInterface(aRequest, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  nsCOMPtr<nsISupports> cacheToken;
  cachingChannel->GetOfflineCacheToken(getter_AddRefs(cacheToken));
  if (cacheToken) {
    nsCOMPtr<nsICacheEntry> cacheDescriptor(do_QueryInterface(cacheToken, &rv));
    NS_ENSURE_SUCCESS(rv, rv);

    rv = cacheDescriptor->SetMetaDataElement("offline-manifest-hash",
                                             mManifestHashValue.get());
    NS_ENSURE_SUCCESS(rv, rv);
  }

  return NS_OK;
}

void nsOfflineManifestItem::ReadStrictFileOriginPolicyPref() {
  mStrictFileOriginPolicy =
      Preferences::GetBool("security.fileuri.strict_origin_policy", true);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineManifestItem::OnStartRequest(nsIRequest *aRequest,
                                      nsISupports *aContext) {
  nsresult rv;
||||||| merged common ancestors
nsOfflineManifestItem::OnStartRequest(nsIRequest *aRequest,
                                      nsISupports *aContext)
{
    nsresult rv;
=======
nsOfflineManifestItem::OnStartRequest(nsIRequest* aRequest) {
  nsresult rv;
>>>>>>> upstream-releases

  nsCOMPtr<nsIHttpChannel> channel = do_QueryInterface(aRequest, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  bool succeeded;
  rv = channel->GetRequestSucceeded(&succeeded);
  NS_ENSURE_SUCCESS(rv, rv);

  if (!succeeded) {
    LOG(("HTTP request failed"));
    LogToConsole("Offline cache manifest HTTP request failed", this);
    mParserState = PARSE_ERROR;
    return NS_ERROR_ABORT;
  }

  rv = GetOldManifestContentHash(aRequest);
  NS_ENSURE_SUCCESS(rv, rv);

<<<<<<< HEAD
  return nsOfflineCacheUpdateItem::OnStartRequest(aRequest, aContext);
||||||| merged common ancestors
    return nsOfflineCacheUpdateItem::OnStartRequest(aRequest, aContext);
=======
  return nsOfflineCacheUpdateItem::OnStartRequest(aRequest);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineManifestItem::OnDataAvailable(nsIRequest *aRequest,
                                       nsISupports *aContext,
                                       nsIInputStream *aStream,
                                       uint64_t aOffset, uint32_t aCount) {
  uint32_t bytesRead = 0;
  aStream->ReadSegments(ReadManifest, this, aCount, &bytesRead);
  mBytesRead += bytesRead;

  if (mParserState == PARSE_ERROR) {
    LOG(("OnDataAvailable is canceling the request due a parse error\n"));
    return NS_ERROR_ABORT;
  }
||||||| merged common ancestors
nsOfflineManifestItem::OnDataAvailable(nsIRequest *aRequest,
                                       nsISupports *aContext,
                                       nsIInputStream *aStream,
                                       uint64_t aOffset,
                                       uint32_t aCount)
{
    uint32_t bytesRead = 0;
    aStream->ReadSegments(ReadManifest, this, aCount, &bytesRead);
    mBytesRead += bytesRead;

    if (mParserState == PARSE_ERROR) {
        LOG(("OnDataAvailable is canceling the request due a parse error\n"));
        return NS_ERROR_ABORT;
    }
=======
nsOfflineManifestItem::OnDataAvailable(nsIRequest* aRequest,
                                       nsIInputStream* aStream,
                                       uint64_t aOffset, uint32_t aCount) {
  uint32_t bytesRead = 0;
  aStream->ReadSegments(ReadManifest, this, aCount, &bytesRead);
  mBytesRead += bytesRead;

  if (mParserState == PARSE_ERROR) {
    LOG(("OnDataAvailable is canceling the request due a parse error\n"));
    return NS_ERROR_ABORT;
  }
>>>>>>> upstream-releases

  LOG(("loaded %u bytes into offline cache [offset=%" PRIu64 "]\n", bytesRead,
       aOffset));

  // All the parent method does is read and discard, don't bother
  // chaining up.

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineManifestItem::OnStopRequest(nsIRequest *aRequest,
                                     nsISupports *aContext, nsresult aStatus) {
  if (mBytesRead == 0) {
    // We didn't need to read (because LOAD_ONLY_IF_MODIFIED was
    // specified).
    mNeedsUpdate = false;
  } else {
    // Handle any leftover manifest data.
    nsCString::const_iterator begin, end;
    mReadBuf.BeginReading(begin);
    mReadBuf.EndReading(end);
    nsresult rv = HandleManifestLine(begin, end);
    NS_ENSURE_SUCCESS(rv, rv);

    rv = CheckNewManifestContentHash(aRequest);
    NS_ENSURE_SUCCESS(rv, rv);
  }
||||||| merged common ancestors
nsOfflineManifestItem::OnStopRequest(nsIRequest *aRequest,
                                     nsISupports *aContext,
                                     nsresult aStatus)
{
    if (mBytesRead == 0) {
        // We didn't need to read (because LOAD_ONLY_IF_MODIFIED was
        // specified).
        mNeedsUpdate = false;
    } else {
        // Handle any leftover manifest data.
        nsCString::const_iterator begin, end;
        mReadBuf.BeginReading(begin);
        mReadBuf.EndReading(end);
        nsresult rv = HandleManifestLine(begin, end);
        NS_ENSURE_SUCCESS(rv, rv);

        rv = CheckNewManifestContentHash(aRequest);
        NS_ENSURE_SUCCESS(rv, rv);
    }
=======
nsOfflineManifestItem::OnStopRequest(nsIRequest* aRequest, nsresult aStatus) {
  if (mBytesRead == 0) {
    // We didn't need to read (because LOAD_ONLY_IF_MODIFIED was
    // specified).
    mNeedsUpdate = false;
  } else {
    // Handle any leftover manifest data.
    nsCString::const_iterator begin, end;
    mReadBuf.BeginReading(begin);
    mReadBuf.EndReading(end);
    nsresult rv = HandleManifestLine(begin, end);
    NS_ENSURE_SUCCESS(rv, rv);

    rv = CheckNewManifestContentHash(aRequest);
    NS_ENSURE_SUCCESS(rv, rv);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return nsOfflineCacheUpdateItem::OnStopRequest(aRequest, aContext, aStatus);
||||||| merged common ancestors
    return nsOfflineCacheUpdateItem::OnStopRequest(aRequest, aContext, aStatus);
=======
  return nsOfflineCacheUpdateItem::OnStopRequest(aRequest, aStatus);
>>>>>>> upstream-releases
}

//-----------------------------------------------------------------------------
// nsOfflineCacheUpdate::nsISupports
//-----------------------------------------------------------------------------

NS_IMPL_ISUPPORTS(nsOfflineCacheUpdate, nsIOfflineCacheUpdateObserver,
                  nsIOfflineCacheUpdate, nsIRunnable)

//-----------------------------------------------------------------------------
// nsOfflineCacheUpdate <public>
//-----------------------------------------------------------------------------

nsOfflineCacheUpdate::nsOfflineCacheUpdate()
    : mState(STATE_UNINITIALIZED),
      mAddedItems(false),
      mPartialUpdate(false),
      mOnlyCheckUpdate(false),
      mSucceeded(true),
      mObsolete(false),
      mItemsInProgress(0),
      mRescheduleCount(0),
      mPinnedEntryRetriesCount(0),
      mPinned(false),
      mByteProgress(0) {}

nsOfflineCacheUpdate::~nsOfflineCacheUpdate() {
  LOG(("nsOfflineCacheUpdate::~nsOfflineCacheUpdate [%p]", this));
}

<<<<<<< HEAD
/* static */
nsresult nsOfflineCacheUpdate::GetCacheKey(nsIURI *aURI, nsACString &aKey) {
  aKey.Truncate();

  nsCOMPtr<nsIURI> newURI;
  nsresult rv = NS_GetURIWithoutRef(aURI, getter_AddRefs(newURI));
  NS_ENSURE_SUCCESS(rv, rv);

  rv = newURI->GetAsciiSpec(aKey);
  NS_ENSURE_SUCCESS(rv, rv);

  return NS_OK;
||||||| merged common ancestors
nsOfflineCacheUpdate::~nsOfflineCacheUpdate()
{
    LOG(("nsOfflineCacheUpdate::~nsOfflineCacheUpdate [%p]", this));
=======
/* static */
nsresult nsOfflineCacheUpdate::GetCacheKey(nsIURI* aURI, nsACString& aKey) {
  aKey.Truncate();

  nsCOMPtr<nsIURI> newURI;
  nsresult rv = NS_GetURIWithoutRef(aURI, getter_AddRefs(newURI));
  NS_ENSURE_SUCCESS(rv, rv);

  rv = newURI->GetAsciiSpec(aKey);
  NS_ENSURE_SUCCESS(rv, rv);

  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsOfflineCacheUpdate::InitInternal(nsIURI *aManifestURI,
                                            nsIPrincipal *aLoadingPrincipal) {
  nsresult rv;
||||||| merged common ancestors
/* static */
nsresult
nsOfflineCacheUpdate::GetCacheKey(nsIURI *aURI, nsACString &aKey)
{
    aKey.Truncate();
=======
nsresult nsOfflineCacheUpdate::InitInternal(nsIURI* aManifestURI,
                                            nsIPrincipal* aLoadingPrincipal) {
  nsresult rv;
>>>>>>> upstream-releases

  // Only http and https applications are supported.
  bool match;
  rv = aManifestURI->SchemeIs("http", &match);
  NS_ENSURE_SUCCESS(rv, rv);

  if (!match) {
    rv = aManifestURI->SchemeIs("https", &match);
    NS_ENSURE_SUCCESS(rv, rv);
    if (!match) return NS_ERROR_ABORT;
  }

  mManifestURI = aManifestURI;
  mLoadingPrincipal = aLoadingPrincipal;

  rv = mManifestURI->GetAsciiHost(mUpdateDomain);
  NS_ENSURE_SUCCESS(rv, rv);

  mPartialUpdate = false;

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsOfflineCacheUpdate::Init(nsIURI *aManifestURI, nsIURI *aDocumentURI,
                                    nsIPrincipal *aLoadingPrincipal,
                                    nsIDocument *aDocument,
                                    nsIFile *aCustomProfileDir) {
  nsresult rv;
||||||| merged common ancestors
nsresult
nsOfflineCacheUpdate::InitInternal(nsIURI *aManifestURI,
                                   nsIPrincipal* aLoadingPrincipal)
{
    nsresult rv;
=======
nsresult nsOfflineCacheUpdate::Init(nsIURI* aManifestURI, nsIURI* aDocumentURI,
                                    nsIPrincipal* aLoadingPrincipal,
                                    dom::Document* aDocument,
                                    nsIFile* aCustomProfileDir) {
  nsresult rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Make sure the service has been initialized
  nsOfflineCacheUpdateService *service =
      nsOfflineCacheUpdateService::EnsureService();
  if (!service) return NS_ERROR_FAILURE;
||||||| merged common ancestors
    // Only http and https applications are supported.
    bool match;
    rv = aManifestURI->SchemeIs("http", &match);
    NS_ENSURE_SUCCESS(rv, rv);
=======
  // Make sure the service has been initialized
  nsOfflineCacheUpdateService* service =
      nsOfflineCacheUpdateService::EnsureService();
  if (!service) return NS_ERROR_FAILURE;
>>>>>>> upstream-releases

<<<<<<< HEAD
  LOG(("nsOfflineCacheUpdate::Init [%p]", this));
||||||| merged common ancestors
    if (!match) {
        rv = aManifestURI->SchemeIs("https", &match);
        NS_ENSURE_SUCCESS(rv, rv);
        if (!match)
            return NS_ERROR_ABORT;
    }
=======
  LOG(("nsOfflineCacheUpdate::Init [%p]", this));

  rv = InitInternal(aManifestURI, aLoadingPrincipal);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  rv = InitInternal(aManifestURI, aLoadingPrincipal);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
    mManifestURI = aManifestURI;
    mLoadingPrincipal = aLoadingPrincipal;
=======
  nsCOMPtr<nsIApplicationCacheService> cacheService =
      do_GetService(NS_APPLICATIONCACHESERVICE_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIApplicationCacheService> cacheService =
      do_GetService(NS_APPLICATIONCACHESERVICE_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
    rv = mManifestURI->GetAsciiHost(mUpdateDomain);
    NS_ENSURE_SUCCESS(rv, rv);
=======
  nsAutoCString originSuffix;
  rv = aLoadingPrincipal->GetOriginSuffix(originSuffix);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsAutoCString originSuffix;
  rv = aLoadingPrincipal->GetOriginSuffix(originSuffix);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
    mPartialUpdate = false;
=======
  mDocumentURI = aDocumentURI;
>>>>>>> upstream-releases

<<<<<<< HEAD
  mDocumentURI = aDocumentURI;
||||||| merged common ancestors
    return NS_OK;
}
=======
  if (aCustomProfileDir) {
    rv = cacheService->BuildGroupIDForSuffix(aManifestURI, originSuffix,
                                             mGroupID);
    NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (aCustomProfileDir) {
    rv = cacheService->BuildGroupIDForSuffix(aManifestURI, originSuffix,
                                             mGroupID);
    NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
nsresult
nsOfflineCacheUpdate::Init(nsIURI *aManifestURI,
                           nsIURI *aDocumentURI,
                           nsIPrincipal* aLoadingPrincipal,
                           nsIDocument *aDocument,
                           nsIFile *aCustomProfileDir)
{
    nsresult rv;
=======
    // Create only a new offline application cache in the custom profile
    // This is a preload of a new cache.
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Create only a new offline application cache in the custom profile
    // This is a preload of a new cache.
||||||| merged common ancestors
    // Make sure the service has been initialized
    nsOfflineCacheUpdateService* service =
        nsOfflineCacheUpdateService::EnsureService();
    if (!service)
        return NS_ERROR_FAILURE;
=======
    // XXX Custom updates don't support "updating" of an existing cache
    // in the custom profile at the moment.  This support can be, though,
    // simply added as well when needed.
    mPreviousApplicationCache = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
    // XXX Custom updates don't support "updating" of an existing cache
    // in the custom profile at the moment.  This support can be, though,
    // simply added as well when needed.
    mPreviousApplicationCache = nullptr;
||||||| merged common ancestors
    LOG(("nsOfflineCacheUpdate::Init [%p]", this));
=======
    rv = cacheService->CreateCustomApplicationCache(
        mGroupID, aCustomProfileDir, kCustomProfileQuota,
        getter_AddRefs(mApplicationCache));
    NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
    rv = cacheService->CreateCustomApplicationCache(
        mGroupID, aCustomProfileDir, kCustomProfileQuota,
        getter_AddRefs(mApplicationCache));
||||||| merged common ancestors
    rv = InitInternal(aManifestURI, aLoadingPrincipal);
=======
    mCustomProfileDir = aCustomProfileDir;
  } else {
    rv = cacheService->BuildGroupIDForSuffix(aManifestURI, originSuffix,
                                             mGroupID);
>>>>>>> upstream-releases
    NS_ENSURE_SUCCESS(rv, rv);

<<<<<<< HEAD
    mCustomProfileDir = aCustomProfileDir;
  } else {
    rv = cacheService->BuildGroupIDForSuffix(aManifestURI, originSuffix,
                                             mGroupID);
||||||| merged common ancestors
    nsCOMPtr<nsIApplicationCacheService> cacheService =
        do_GetService(NS_APPLICATIONCACHESERVICE_CONTRACTID, &rv);
=======
    rv = cacheService->GetActiveCache(
        mGroupID, getter_AddRefs(mPreviousApplicationCache));
>>>>>>> upstream-releases
    NS_ENSURE_SUCCESS(rv, rv);

<<<<<<< HEAD
    rv = cacheService->GetActiveCache(
        mGroupID, getter_AddRefs(mPreviousApplicationCache));
||||||| merged common ancestors
    nsAutoCString originSuffix;
    rv = aLoadingPrincipal->GetOriginSuffix(originSuffix);
=======
    rv = cacheService->CreateApplicationCache(
        mGroupID, getter_AddRefs(mApplicationCache));
>>>>>>> upstream-releases
    NS_ENSURE_SUCCESS(rv, rv);
  }

<<<<<<< HEAD
    rv = cacheService->CreateApplicationCache(
        mGroupID, getter_AddRefs(mApplicationCache));
    NS_ENSURE_SUCCESS(rv, rv);
  }
||||||| merged common ancestors
    mDocumentURI = aDocumentURI;
=======
  rv = nsOfflineCacheUpdateService::OfflineAppPinnedForURI(aDocumentURI,
                                                           nullptr, &mPinned);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  rv = nsOfflineCacheUpdateService::OfflineAppPinnedForURI(aDocumentURI,
                                                           nullptr, &mPinned);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
    if (aCustomProfileDir) {
        rv = cacheService->BuildGroupIDForSuffix(aManifestURI, originSuffix, mGroupID);
        NS_ENSURE_SUCCESS(rv, rv);
=======
  mState = STATE_INITIALIZED;
  return NS_OK;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  mState = STATE_INITIALIZED;
  return NS_OK;
}
||||||| merged common ancestors
        // Create only a new offline application cache in the custom profile
        // This is a preload of a new cache.
=======
nsresult nsOfflineCacheUpdate::InitForUpdateCheck(
    nsIURI* aManifestURI, nsIPrincipal* aLoadingPrincipal,
    nsIObserver* aObserver) {
  nsresult rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsOfflineCacheUpdate::InitForUpdateCheck(
    nsIURI *aManifestURI, nsIPrincipal *aLoadingPrincipal,
    nsIObserver *aObserver) {
  nsresult rv;
||||||| merged common ancestors
        // XXX Custom updates don't support "updating" of an existing cache
        // in the custom profile at the moment.  This support can be, though,
        // simply added as well when needed.
        mPreviousApplicationCache = nullptr;
=======
  // Make sure the service has been initialized
  nsOfflineCacheUpdateService* service =
      nsOfflineCacheUpdateService::EnsureService();
  if (!service) return NS_ERROR_FAILURE;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Make sure the service has been initialized
  nsOfflineCacheUpdateService *service =
      nsOfflineCacheUpdateService::EnsureService();
  if (!service) return NS_ERROR_FAILURE;
||||||| merged common ancestors
        rv = cacheService->CreateCustomApplicationCache(mGroupID,
                                                        aCustomProfileDir,
                                                        kCustomProfileQuota,
                                                        getter_AddRefs(mApplicationCache));
        NS_ENSURE_SUCCESS(rv, rv);
=======
  LOG(("nsOfflineCacheUpdate::InitForUpdateCheck [%p]", this));
>>>>>>> upstream-releases

<<<<<<< HEAD
  LOG(("nsOfflineCacheUpdate::InitForUpdateCheck [%p]", this));
||||||| merged common ancestors
        mCustomProfileDir = aCustomProfileDir;
    }
    else {
        rv = cacheService->BuildGroupIDForSuffix(aManifestURI, originSuffix, mGroupID);
        NS_ENSURE_SUCCESS(rv, rv);
=======
  rv = InitInternal(aManifestURI, aLoadingPrincipal);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  rv = InitInternal(aManifestURI, aLoadingPrincipal);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
        rv = cacheService->GetActiveCache(mGroupID,
                                          getter_AddRefs(mPreviousApplicationCache));
        NS_ENSURE_SUCCESS(rv, rv);
=======
  nsCOMPtr<nsIApplicationCacheService> cacheService =
      do_GetService(NS_APPLICATIONCACHESERVICE_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIApplicationCacheService> cacheService =
      do_GetService(NS_APPLICATIONCACHESERVICE_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
        rv = cacheService->CreateApplicationCache(mGroupID,
                                                  getter_AddRefs(mApplicationCache));
        NS_ENSURE_SUCCESS(rv, rv);
    }
=======
  nsAutoCString originSuffix;
  rv = aLoadingPrincipal->GetOriginSuffix(originSuffix);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsAutoCString originSuffix;
  rv = aLoadingPrincipal->GetOriginSuffix(originSuffix);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
    rv = nsOfflineCacheUpdateService::OfflineAppPinnedForURI(aDocumentURI,
                                                             nullptr,
                                                             &mPinned);
    NS_ENSURE_SUCCESS(rv, rv);
=======
  rv =
      cacheService->BuildGroupIDForSuffix(aManifestURI, originSuffix, mGroupID);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  rv =
      cacheService->BuildGroupIDForSuffix(aManifestURI, originSuffix, mGroupID);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = cacheService->GetActiveCache(mGroupID,
                                    getter_AddRefs(mPreviousApplicationCache));
  NS_ENSURE_SUCCESS(rv, rv);

  // To load the manifest properly using current app cache to satisfy and
  // also to compare the cached content hash value we have to set 'some'
  // app cache to write to on the channel.  Otherwise the cached version will
  // be used and no actual network request will be made.  We use the same
  // app cache here.  OpenChannel prevents caching in this case using
  // INHIBIT_CACHING load flag.
  mApplicationCache = mPreviousApplicationCache;

  rv = nsOfflineCacheUpdateService::OfflineAppPinnedForURI(aManifestURI,
                                                           nullptr, &mPinned);
  NS_ENSURE_SUCCESS(rv, rv);

  mUpdateAvailableObserver = aObserver;
  mOnlyCheckUpdate = true;

  mState = STATE_INITIALIZED;
  return NS_OK;
||||||| merged common ancestors
    mState = STATE_INITIALIZED;
    return NS_OK;
=======
  rv = cacheService->GetActiveCache(mGroupID,
                                    getter_AddRefs(mPreviousApplicationCache));
  NS_ENSURE_SUCCESS(rv, rv);

  // To load the manifest properly using current app cache to satisfy and
  // also to compare the cached content hash value we have to set 'some'
  // app cache to write to on the channel.  Otherwise the cached version will
  // be used and no actual network request will be made.  We use the same
  // app cache here.  OpenChannel prevents caching in this case using
  // INHIBIT_CACHING load flag.
  mApplicationCache = mPreviousApplicationCache;

  rv = nsOfflineCacheUpdateService::OfflineAppPinnedForURI(aManifestURI,
                                                           nullptr, &mPinned);
  NS_ENSURE_SUCCESS(rv, rv);

  mUpdateAvailableObserver = aObserver;
  mOnlyCheckUpdate = true;

  mState = STATE_INITIALIZED;
  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsOfflineCacheUpdate::InitPartial(nsIURI *aManifestURI,
                                           const nsACString &clientID,
                                           nsIURI *aDocumentURI,
                                           nsIPrincipal *aLoadingPrincipal) {
  nsresult rv;
||||||| merged common ancestors
nsresult
nsOfflineCacheUpdate::InitForUpdateCheck(nsIURI *aManifestURI,
                                         nsIPrincipal* aLoadingPrincipal,
                                         nsIObserver *aObserver)
{
    nsresult rv;
=======
nsresult nsOfflineCacheUpdate::InitPartial(nsIURI* aManifestURI,
                                           const nsACString& clientID,
                                           nsIURI* aDocumentURI,
                                           nsIPrincipal* aLoadingPrincipal) {
  nsresult rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Make sure the service has been initialized
  nsOfflineCacheUpdateService *service =
      nsOfflineCacheUpdateService::EnsureService();
  if (!service) return NS_ERROR_FAILURE;
||||||| merged common ancestors
    // Make sure the service has been initialized
    nsOfflineCacheUpdateService* service =
        nsOfflineCacheUpdateService::EnsureService();
    if (!service)
        return NS_ERROR_FAILURE;
=======
  // Make sure the service has been initialized
  nsOfflineCacheUpdateService* service =
      nsOfflineCacheUpdateService::EnsureService();
  if (!service) return NS_ERROR_FAILURE;
>>>>>>> upstream-releases

  LOG(("nsOfflineCacheUpdate::InitPartial [%p]", this));

  mPartialUpdate = true;
  mDocumentURI = aDocumentURI;
  mLoadingPrincipal = aLoadingPrincipal;

  mManifestURI = aManifestURI;
  rv = mManifestURI->GetAsciiHost(mUpdateDomain);
  NS_ENSURE_SUCCESS(rv, rv);

  nsCOMPtr<nsIApplicationCacheService> cacheService =
      do_GetService(NS_APPLICATIONCACHESERVICE_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = cacheService->GetApplicationCache(clientID,
                                         getter_AddRefs(mApplicationCache));
  NS_ENSURE_SUCCESS(rv, rv);

  if (!mApplicationCache) {
    nsAutoCString manifestSpec;
    rv = GetCacheKey(mManifestURI, manifestSpec);
    NS_ENSURE_SUCCESS(rv, rv);

    rv = cacheService->CreateApplicationCache(
        manifestSpec, getter_AddRefs(mApplicationCache));
    NS_ENSURE_SUCCESS(rv, rv);
  }

  rv = mApplicationCache->GetManifestURI(getter_AddRefs(mManifestURI));
  NS_ENSURE_SUCCESS(rv, rv);

  nsAutoCString groupID;
  rv = mApplicationCache->GetGroupID(groupID);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = nsOfflineCacheUpdateService::OfflineAppPinnedForURI(aDocumentURI,
                                                           nullptr, &mPinned);
  NS_ENSURE_SUCCESS(rv, rv);

  mState = STATE_INITIALIZED;
  return NS_OK;
}

<<<<<<< HEAD
nsresult nsOfflineCacheUpdate::HandleManifest(bool *aDoUpdate) {
  // Be pessimistic
  *aDoUpdate = false;
||||||| merged common ancestors
nsresult
nsOfflineCacheUpdate::InitPartial(nsIURI *aManifestURI,
                                  const nsACString& clientID,
                                  nsIURI *aDocumentURI,
                                  nsIPrincipal *aLoadingPrincipal)
{
    nsresult rv;
=======
nsresult nsOfflineCacheUpdate::HandleManifest(bool* aDoUpdate) {
  // Be pessimistic
  *aDoUpdate = false;
>>>>>>> upstream-releases

  bool succeeded;
  nsresult rv = mManifestItem->GetRequestSucceeded(&succeeded);
  NS_ENSURE_SUCCESS(rv, rv);

  if (!succeeded || !mManifestItem->ParseSucceeded()) {
    return NS_ERROR_FAILURE;
  }

  if (!mManifestItem->NeedsUpdate()) {
    return NS_OK;
  }

<<<<<<< HEAD
  // Add items requested by the manifest.
  const nsCOMArray<nsIURI> &manifestURIs = mManifestItem->GetExplicitURIs();
  for (int32_t i = 0; i < manifestURIs.Count(); i++) {
    rv = AddURI(manifestURIs[i], nsIApplicationCache::ITEM_EXPLICIT);
||||||| merged common ancestors
    mManifestURI = aManifestURI;
    rv = mManifestURI->GetAsciiHost(mUpdateDomain);
=======
  // Add items requested by the manifest.
  const nsCOMArray<nsIURI>& manifestURIs = mManifestItem->GetExplicitURIs();
  for (int32_t i = 0; i < manifestURIs.Count(); i++) {
    rv = AddURI(manifestURIs[i], nsIApplicationCache::ITEM_EXPLICIT);
>>>>>>> upstream-releases
    NS_ENSURE_SUCCESS(rv, rv);
  }

<<<<<<< HEAD
  const nsCOMArray<nsIURI> &anonURIs = mManifestItem->GetAnonymousURIs();
  for (int32_t i = 0; i < anonURIs.Count(); i++) {
    rv = AddURI(anonURIs[i], nsIApplicationCache::ITEM_EXPLICIT,
                nsIRequest::LOAD_ANONYMOUS);
||||||| merged common ancestors
    nsCOMPtr<nsIApplicationCacheService> cacheService =
        do_GetService(NS_APPLICATIONCACHESERVICE_CONTRACTID, &rv);
=======
  const nsCOMArray<nsIURI>& anonURIs = mManifestItem->GetAnonymousURIs();
  for (int32_t i = 0; i < anonURIs.Count(); i++) {
    rv = AddURI(anonURIs[i], nsIApplicationCache::ITEM_EXPLICIT,
                nsIRequest::LOAD_ANONYMOUS);
>>>>>>> upstream-releases
    NS_ENSURE_SUCCESS(rv, rv);
  }

<<<<<<< HEAD
  const nsCOMArray<nsIURI> &fallbackURIs = mManifestItem->GetFallbackURIs();
  for (int32_t i = 0; i < fallbackURIs.Count(); i++) {
    rv = AddURI(fallbackURIs[i], nsIApplicationCache::ITEM_FALLBACK);
||||||| merged common ancestors
    rv = cacheService->GetApplicationCache(clientID,
                                           getter_AddRefs(mApplicationCache));
=======
  const nsCOMArray<nsIURI>& fallbackURIs = mManifestItem->GetFallbackURIs();
  for (int32_t i = 0; i < fallbackURIs.Count(); i++) {
    rv = AddURI(fallbackURIs[i], nsIApplicationCache::ITEM_FALLBACK);
>>>>>>> upstream-releases
    NS_ENSURE_SUCCESS(rv, rv);
  }

  // The document that requested the manifest is implicitly included
  // as part of that manifest update.
  rv = AddURI(mDocumentURI, nsIApplicationCache::ITEM_IMPLICIT);
  NS_ENSURE_SUCCESS(rv, rv);

  // Add items previously cached implicitly
  rv = AddExistingItems(nsIApplicationCache::ITEM_IMPLICIT);
  NS_ENSURE_SUCCESS(rv, rv);

  // Add items requested by the script API
  rv = AddExistingItems(nsIApplicationCache::ITEM_DYNAMIC);
  NS_ENSURE_SUCCESS(rv, rv);

  // Add opportunistically cached items conforming current opportunistic
  // namespace list
  rv = AddExistingItems(nsIApplicationCache::ITEM_OPPORTUNISTIC,
                        &mManifestItem->GetOpportunisticNamespaces());
  NS_ENSURE_SUCCESS(rv, rv);

  *aDoUpdate = true;

  return NS_OK;
}

<<<<<<< HEAD
bool nsOfflineCacheUpdate::CheckUpdateAvailability() {
  nsresult rv;

  bool succeeded;
  rv = mManifestItem->GetRequestSucceeded(&succeeded);
  NS_ENSURE_SUCCESS(rv, false);
||||||| merged common ancestors
nsresult
nsOfflineCacheUpdate::HandleManifest(bool *aDoUpdate)
{
    // Be pessimistic
    *aDoUpdate = false;

    bool succeeded;
    nsresult rv = mManifestItem->GetRequestSucceeded(&succeeded);
    NS_ENSURE_SUCCESS(rv, rv);

    if (!succeeded || !mManifestItem->ParseSucceeded()) {
        return NS_ERROR_FAILURE;
    }
=======
bool nsOfflineCacheUpdate::CheckUpdateAvailability() {
  nsresult rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!succeeded || !mManifestItem->ParseSucceeded()) {
    return false;
  }
||||||| merged common ancestors
    if (!mManifestItem->NeedsUpdate()) {
        return NS_OK;
    }

    // Add items requested by the manifest.
    const nsCOMArray<nsIURI> &manifestURIs = mManifestItem->GetExplicitURIs();
    for (int32_t i = 0; i < manifestURIs.Count(); i++) {
        rv = AddURI(manifestURIs[i], nsIApplicationCache::ITEM_EXPLICIT);
        NS_ENSURE_SUCCESS(rv, rv);
    }
=======
  bool succeeded;
  rv = mManifestItem->GetRequestSucceeded(&succeeded);
  NS_ENSURE_SUCCESS(rv, false);

  if (!succeeded || !mManifestItem->ParseSucceeded()) {
    return false;
  }
>>>>>>> upstream-releases

  if (!mPinned) {
    uint16_t status;
    rv = mManifestItem->GetStatus(&status);
    NS_ENSURE_SUCCESS(rv, false);

    // Treat these as there would be an update available,
    // since this is indication of demand to remove this
    // offline cache.
    if (status == 404 || status == 410) {
      return true;
    }
  }

  return mManifestItem->NeedsUpdate();
}

<<<<<<< HEAD
void nsOfflineCacheUpdate::LoadCompleted(nsOfflineCacheUpdateItem *aItem) {
  nsresult rv;
||||||| merged common ancestors
    // Add items requested by the script API
    rv = AddExistingItems(nsIApplicationCache::ITEM_DYNAMIC);
    NS_ENSURE_SUCCESS(rv, rv);
=======
void nsOfflineCacheUpdate::LoadCompleted(nsOfflineCacheUpdateItem* aItem) {
  nsresult rv;
>>>>>>> upstream-releases

  LOG(("nsOfflineCacheUpdate::LoadCompleted [%p]", this));

  if (mState == STATE_FINISHED) {
    LOG(("  after completion, ignoring"));
    return;
  }

  // Keep the object alive through a Finish() call.
  nsCOMPtr<nsIOfflineCacheUpdate> kungFuDeathGrip(this);

  if (mState == STATE_CANCELLED) {
    NotifyState(nsIOfflineCacheUpdateObserver::STATE_ERROR);
    Finish();
    return;
  }

  if (mState == STATE_CHECKING) {
    // Manifest load finished.

    if (mOnlyCheckUpdate) {
      Finish();
      NotifyUpdateAvailability(CheckUpdateAvailability());
      return;
    }

    NS_ASSERTION(mManifestItem, "Must have a manifest item in STATE_CHECKING.");
    NS_ASSERTION(mManifestItem == aItem,
                 "Unexpected aItem in nsOfflineCacheUpdate::LoadCompleted");

    // A 404 or 410 is interpreted as an intentional removal of
    // the manifest file, rather than a transient server error.
    // Obsolete this cache group if one of these is returned.
    uint16_t status;
    rv = mManifestItem->GetStatus(&status);
    if (status == 404 || status == 410) {
      LogToConsole("Offline cache manifest removed, cache cleared",
                   mManifestItem);
      mSucceeded = false;
      if (mPreviousApplicationCache) {
        if (mPinned) {
          // Do not obsolete a pinned application.
          NotifyState(nsIOfflineCacheUpdateObserver::STATE_NOUPDATE);
        } else {
          NotifyState(nsIOfflineCacheUpdateObserver::STATE_OBSOLETE);
          mObsolete = true;
        }
      } else {
        NotifyState(nsIOfflineCacheUpdateObserver::STATE_ERROR);
        mObsolete = true;
      }
      Finish();
      return;
    }

    bool doUpdate;
    if (NS_FAILED(HandleManifest(&doUpdate))) {
      mSucceeded = false;
      NotifyState(nsIOfflineCacheUpdateObserver::STATE_ERROR);
      Finish();
      return;
    }

    if (!doUpdate) {
      LogToConsole("Offline cache doesn't need to update", mManifestItem);

      mSucceeded = false;

      AssociateDocuments(mPreviousApplicationCache);

      ScheduleImplicit();

      // If we didn't need an implicit update, we can
      // send noupdate and end the update now.
      if (!mImplicitUpdate) {
        NotifyState(nsIOfflineCacheUpdateObserver::STATE_NOUPDATE);
        Finish();
      }
      return;
    }

    rv = mApplicationCache->MarkEntry(mManifestItem->mCacheKey,
                                      mManifestItem->mItemType);
    if (NS_FAILED(rv)) {
      mSucceeded = false;
      NotifyState(nsIOfflineCacheUpdateObserver::STATE_ERROR);
      Finish();
      return;
    }

    mState = STATE_DOWNLOADING;
    NotifyState(nsIOfflineCacheUpdateObserver::STATE_DOWNLOADING);

    // Start fetching resources.
    ProcessNextURI();

    return;
  }

  // Normal load finished.
  if (mItemsInProgress)  // Just to be safe here!
    --mItemsInProgress;

  bool succeeded;
  rv = aItem->GetRequestSucceeded(&succeeded);

  if (mPinned && NS_SUCCEEDED(rv) && succeeded) {
    uint32_t dummy_cache_type;
    rv = mApplicationCache->GetTypes(aItem->mCacheKey, &dummy_cache_type);
    bool item_doomed = NS_FAILED(rv);  // can not find it? -> doomed

    if (item_doomed && mPinnedEntryRetriesCount < kPinnedEntryRetriesLimit &&
        (aItem->mItemType & (nsIApplicationCache::ITEM_EXPLICIT |
                             nsIApplicationCache::ITEM_FALLBACK))) {
      rv = EvictOneNonPinned();
      if (NS_FAILED(rv)) {
        mSucceeded = false;
        NotifyState(nsIOfflineCacheUpdateObserver::STATE_ERROR);
        Finish();
        return;
      }

      // This reverts the item state to UNINITIALIZED that makes it to
      // be scheduled for download again.
      rv = aItem->Cancel();
      if (NS_FAILED(rv)) {
        mSucceeded = false;
        NotifyState(nsIOfflineCacheUpdateObserver::STATE_ERROR);
        Finish();
        return;
      }

      mPinnedEntryRetriesCount++;

      LogToConsole("An unpinned offline cache deleted");

      // Retry this item.
      ProcessNextURI();
      return;
    }
  }

  // According to parallelism this may imply more pinned retries count,
  // but that is not critical, since at one moment the algorithm will
  // stop anyway.  Also, this code may soon be completely removed
  // after we have a separate storage for pinned apps.
  mPinnedEntryRetriesCount = 0;

  // Check for failures.  3XX, 4XX and 5XX errors on items explicitly
  // listed in the manifest will cause the update to fail.
  if (NS_FAILED(rv) || !succeeded) {
    if (aItem->mItemType & (nsIApplicationCache::ITEM_EXPLICIT |
                            nsIApplicationCache::ITEM_FALLBACK)) {
      LogToConsole("Offline cache manifest item failed to load", aItem);
      mSucceeded = false;
    }
  } else {
    rv = mApplicationCache->MarkEntry(aItem->mCacheKey, aItem->mItemType);
    if (NS_FAILED(rv)) {
      mSucceeded = false;
    }
  }

  if (!mSucceeded) {
    NotifyState(nsIOfflineCacheUpdateObserver::STATE_ERROR);
    Finish();
    return;
  }

  NotifyState(nsIOfflineCacheUpdateObserver::STATE_ITEMCOMPLETED);

  ProcessNextURI();
}

<<<<<<< HEAD
void nsOfflineCacheUpdate::ManifestCheckCompleted(
    nsresult aStatus, const nsCString &aManifestHash) {
  // Keep the object alive through a Finish() call.
  nsCOMPtr<nsIOfflineCacheUpdate> kungFuDeathGrip(this);
||||||| merged common ancestors
void
nsOfflineCacheUpdate::ManifestCheckCompleted(nsresult aStatus,
                                             const nsCString &aManifestHash)
{
    // Keep the object alive through a Finish() call.
    nsCOMPtr<nsIOfflineCacheUpdate> kungFuDeathGrip(this);

    if (NS_SUCCEEDED(aStatus)) {
        nsAutoCString firstManifestHash;
        mManifestItem->GetManifestHash(firstManifestHash);
        if (aManifestHash != firstManifestHash) {
            LOG(("Manifest has changed during cache items download [%p]", this));
            LogToConsole("Offline cache manifest changed during update", mManifestItem);
            aStatus = NS_ERROR_FAILURE;
        }
    }
=======
void nsOfflineCacheUpdate::ManifestCheckCompleted(
    nsresult aStatus, const nsCString& aManifestHash) {
  // Keep the object alive through a Finish() call.
  nsCOMPtr<nsIOfflineCacheUpdate> kungFuDeathGrip(this);
>>>>>>> upstream-releases

  if (NS_SUCCEEDED(aStatus)) {
    nsAutoCString firstManifestHash;
    mManifestItem->GetManifestHash(firstManifestHash);
    if (aManifestHash != firstManifestHash) {
      LOG(("Manifest has changed during cache items download [%p]", this));
      LogToConsole("Offline cache manifest changed during update",
                   mManifestItem);
      aStatus = NS_ERROR_FAILURE;
    }
  }

  if (NS_FAILED(aStatus)) {
    mSucceeded = false;
    NotifyState(nsIOfflineCacheUpdateObserver::STATE_ERROR);
  }

  if (NS_FAILED(aStatus) && mRescheduleCount < kRescheduleLimit) {
    // Do the final stuff but prevent notification of STATE_FINISHED.
    // That would disconnect listeners that are responsible for document
    // association after a successful update. Forwarding notifications
    // from a new update through this dead update to them is absolutely
    // correct.
    FinishNoNotify();

    RefPtr<nsOfflineCacheUpdate> newUpdate = new nsOfflineCacheUpdate();
    // Leave aDocument argument null. Only glues and children keep
    // document instances.
    newUpdate->Init(mManifestURI, mDocumentURI, mLoadingPrincipal, nullptr,
                    mCustomProfileDir);

    // In a rare case the manifest will not be modified on the next refetch
    // transfer all master document URIs to the new update to ensure that
    // all documents refering it will be properly cached.
    for (int32_t i = 0; i < mDocumentURIs.Count(); i++) {
      newUpdate->StickDocument(mDocumentURIs[i]);
    }

    newUpdate->mRescheduleCount = mRescheduleCount + 1;
    newUpdate->AddObserver(this, false);
    newUpdate->Schedule();
  } else {
    LogToConsole("Offline cache update done", mManifestItem);
    Finish();
  }
}

nsresult nsOfflineCacheUpdate::Begin() {
  LOG(("nsOfflineCacheUpdate::Begin [%p]", this));

  // Keep the object alive through a ProcessNextURI()/Finish() call.
  nsCOMPtr<nsIOfflineCacheUpdate> kungFuDeathGrip(this);

  mItemsInProgress = 0;

  if (mState == STATE_CANCELLED) {
    nsresult rv = NS_DispatchToMainThread(
        NewRunnableMethod("nsOfflineCacheUpdate::AsyncFinishWithError", this,
                          &nsOfflineCacheUpdate::AsyncFinishWithError));
    NS_ENSURE_SUCCESS(rv, rv);

    return NS_OK;
  }

  if (mPartialUpdate) {
    mState = STATE_DOWNLOADING;
    NotifyState(nsIOfflineCacheUpdateObserver::STATE_DOWNLOADING);
    ProcessNextURI();
    return NS_OK;
  }

  // Start checking the manifest.
  mManifestItem =
      new nsOfflineManifestItem(mManifestURI, mDocumentURI, mLoadingPrincipal,
                                mApplicationCache, mPreviousApplicationCache);
  if (!mManifestItem) {
    return NS_ERROR_OUT_OF_MEMORY;
  }

  mState = STATE_CHECKING;
  mByteProgress = 0;
  NotifyState(nsIOfflineCacheUpdateObserver::STATE_CHECKING);

  nsresult rv = mManifestItem->OpenChannel(this);
  if (NS_FAILED(rv)) {
    LoadCompleted(mManifestItem);
  }

  return NS_OK;
}

//-----------------------------------------------------------------------------
// nsOfflineCacheUpdate <private>
//-----------------------------------------------------------------------------

<<<<<<< HEAD
nsresult nsOfflineCacheUpdate::AddExistingItems(
    uint32_t aType, nsTArray<nsCString> *namespaceFilter) {
  if (!mPreviousApplicationCache) {
    return NS_OK;
  }

  if (namespaceFilter && namespaceFilter->Length() == 0) {
    // Don't bother to walk entries when there are no namespaces
    // defined.
    return NS_OK;
  }
||||||| merged common ancestors
nsresult
nsOfflineCacheUpdate::AddExistingItems(uint32_t aType,
                                       nsTArray<nsCString>* namespaceFilter)
{
    if (!mPreviousApplicationCache) {
        return NS_OK;
    }

    if (namespaceFilter && namespaceFilter->Length() == 0) {
        // Don't bother to walk entries when there are no namespaces
        // defined.
        return NS_OK;
    }
=======
nsresult nsOfflineCacheUpdate::AddExistingItems(
    uint32_t aType, nsTArray<nsCString>* namespaceFilter) {
  if (!mPreviousApplicationCache) {
    return NS_OK;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t count = 0;
  char **keys = nullptr;
  nsresult rv = mPreviousApplicationCache->GatherEntries(aType, &count, &keys);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
    uint32_t count = 0;
    char **keys = nullptr;
    nsresult rv = mPreviousApplicationCache->GatherEntries(aType,
                                                           &count, &keys);
    NS_ENSURE_SUCCESS(rv, rv);
=======
  if (namespaceFilter && namespaceFilter->Length() == 0) {
    // Don't bother to walk entries when there are no namespaces
    // defined.
    return NS_OK;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoFreeArray autoFree(count, keys);
||||||| merged common ancestors
    AutoFreeArray autoFree(count, keys);
=======
  nsTArray<nsCString> keys;
  nsresult rv = mPreviousApplicationCache->GatherEntries(aType, keys);
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (uint32_t i = 0; i < count; i++) {
    if (namespaceFilter) {
      bool found = false;
      for (uint32_t j = 0; j < namespaceFilter->Length() && !found; j++) {
        found = StringBeginsWith(nsDependentCString(keys[i]),
                                 namespaceFilter->ElementAt(j));
      }
||||||| merged common ancestors
    for (uint32_t i = 0; i < count; i++) {
        if (namespaceFilter) {
            bool found = false;
            for (uint32_t j = 0; j < namespaceFilter->Length() && !found; j++) {
                found = StringBeginsWith(nsDependentCString(keys[i]),
                                         namespaceFilter->ElementAt(j));
            }
=======
  for (auto& key : keys) {
    if (namespaceFilter) {
      bool found = false;
      for (uint32_t j = 0; j < namespaceFilter->Length() && !found; j++) {
        found = StringBeginsWith(key, namespaceFilter->ElementAt(j));
      }
>>>>>>> upstream-releases

      if (!found) continue;
    }

<<<<<<< HEAD
    nsCOMPtr<nsIURI> uri;
    if (NS_SUCCEEDED(NS_NewURI(getter_AddRefs(uri), keys[i]))) {
      rv = AddURI(uri, aType);
      NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
        nsCOMPtr<nsIURI> uri;
        if (NS_SUCCEEDED(NS_NewURI(getter_AddRefs(uri), keys[i]))) {
            rv = AddURI(uri, aType);
            NS_ENSURE_SUCCESS(rv, rv);
        }
=======
    nsCOMPtr<nsIURI> uri;
    if (NS_SUCCEEDED(NS_NewURI(getter_AddRefs(uri), key))) {
      rv = AddURI(uri, aType);
      NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases
    }
  }

  return NS_OK;
}

nsresult nsOfflineCacheUpdate::ProcessNextURI() {
  // Keep the object alive through a Finish() call.
  nsCOMPtr<nsIOfflineCacheUpdate> kungFuDeathGrip(this);

  LOG(("nsOfflineCacheUpdate::ProcessNextURI [%p, inprogress=%d, numItems=%zu]",
       this, mItemsInProgress, mItems.Length()));

  if (mState != STATE_DOWNLOADING) {
    LOG(("  should only be called from the DOWNLOADING state, ignoring"));
    return NS_ERROR_UNEXPECTED;
  }

<<<<<<< HEAD
  nsOfflineCacheUpdateItem *runItem = nullptr;
  uint32_t completedItems = 0;
  for (uint32_t i = 0; i < mItems.Length(); ++i) {
    nsOfflineCacheUpdateItem *item = mItems[i];
||||||| merged common ancestors
        if (item->IsScheduled()) {
            runItem = item;
            break;
        }
=======
  nsOfflineCacheUpdateItem* runItem = nullptr;
  uint32_t completedItems = 0;
  for (uint32_t i = 0; i < mItems.Length(); ++i) {
    nsOfflineCacheUpdateItem* item = mItems[i];
>>>>>>> upstream-releases

    if (item->IsScheduled()) {
      runItem = item;
      break;
    }

    if (item->IsCompleted()) ++completedItems;
  }

  if (completedItems == mItems.Length()) {
    LOG(("nsOfflineCacheUpdate::ProcessNextURI [%p]: all items loaded", this));

    if (mPartialUpdate) {
      return Finish();
    } else {
      // Verify that the manifest wasn't changed during the
      // update, to prevent capturing a cache while the server
      // is being updated.  The check will call
      // ManifestCheckCompleted() when it's done.
      RefPtr<nsManifestCheck> manifestCheck = new nsManifestCheck(
          this, mManifestURI, mDocumentURI, mLoadingPrincipal);
      if (NS_FAILED(manifestCheck->Begin())) {
        mSucceeded = false;
        NotifyState(nsIOfflineCacheUpdateObserver::STATE_ERROR);
        return Finish();
      }

      return NS_OK;
    }
  }

  if (!runItem) {
    LOG(
        ("nsOfflineCacheUpdate::ProcessNextURI [%p]:"
         " No more items to include in parallel load",
         this));
    return NS_OK;
  }

  if (LOG_ENABLED()) {
    LOG(("%p: Opening channel for %s", this,
         runItem->mURI->GetSpecOrDefault().get()));
  }

  ++mItemsInProgress;
  NotifyState(nsIOfflineCacheUpdateObserver::STATE_ITEMSTARTED);

  nsresult rv = runItem->OpenChannel(this);
  if (NS_FAILED(rv)) {
    LoadCompleted(runItem);
    return rv;
  }

  if (mItemsInProgress >= kParallelLoadLimit) {
    LOG(
        ("nsOfflineCacheUpdate::ProcessNextURI [%p]:"
         " At parallel load limit",
         this));
    return NS_OK;
  }

  // This calls this method again via a post triggering
  // a parallel item load
  return NS_DispatchToCurrentThread(this);
}

<<<<<<< HEAD
void nsOfflineCacheUpdate::GatherObservers(
    nsCOMArray<nsIOfflineCacheUpdateObserver> &aObservers) {
  for (int32_t i = 0; i < mWeakObservers.Count(); i++) {
    nsCOMPtr<nsIOfflineCacheUpdateObserver> observer =
        do_QueryReferent(mWeakObservers[i]);
    if (observer)
      aObservers.AppendObject(observer);
    else
      mWeakObservers.RemoveObjectAt(i--);
  }
||||||| merged common ancestors
void
nsOfflineCacheUpdate::GatherObservers(nsCOMArray<nsIOfflineCacheUpdateObserver> &aObservers)
{
    for (int32_t i = 0; i < mWeakObservers.Count(); i++) {
        nsCOMPtr<nsIOfflineCacheUpdateObserver> observer =
            do_QueryReferent(mWeakObservers[i]);
        if (observer)
            aObservers.AppendObject(observer);
        else
            mWeakObservers.RemoveObjectAt(i--);
    }
=======
void nsOfflineCacheUpdate::GatherObservers(
    nsCOMArray<nsIOfflineCacheUpdateObserver>& aObservers) {
  for (int32_t i = 0; i < mWeakObservers.Count(); i++) {
    nsCOMPtr<nsIOfflineCacheUpdateObserver> observer =
        do_QueryReferent(mWeakObservers[i]);
    if (observer)
      aObservers.AppendObject(observer);
    else
      mWeakObservers.RemoveObjectAt(i--);
  }
>>>>>>> upstream-releases

  for (int32_t i = 0; i < mObservers.Count(); i++) {
    aObservers.AppendObject(mObservers[i]);
  }
}

void nsOfflineCacheUpdate::NotifyState(uint32_t state) {
  LOG(("nsOfflineCacheUpdate::NotifyState [%p, %d]", this, state));

  if (state == STATE_ERROR) {
    LogToConsole("Offline cache update error", mManifestItem);
  }

  nsCOMArray<nsIOfflineCacheUpdateObserver> observers;
  GatherObservers(observers);

  for (int32_t i = 0; i < observers.Count(); i++) {
    observers[i]->UpdateStateChanged(this, state);
  }
}

void nsOfflineCacheUpdate::NotifyUpdateAvailability(bool updateAvailable) {
  if (!mUpdateAvailableObserver) return;

  LOG(("nsOfflineCacheUpdate::NotifyUpdateAvailability [this=%p, avail=%d]",
       this, updateAvailable));

<<<<<<< HEAD
  const char *topic = updateAvailable ? "offline-cache-update-available"
                                      : "offline-cache-update-unavailable";
||||||| merged common ancestors
    const char* topic = updateAvailable
                      ? "offline-cache-update-available"
                      : "offline-cache-update-unavailable";
=======
  const char* topic = updateAvailable ? "offline-cache-update-available"
                                      : "offline-cache-update-unavailable";
>>>>>>> upstream-releases

  nsCOMPtr<nsIObserver> observer;
  observer.swap(mUpdateAvailableObserver);
  observer->Observe(mManifestURI, topic, nullptr);
}

<<<<<<< HEAD
void nsOfflineCacheUpdate::AssociateDocuments(nsIApplicationCache *cache) {
  if (!cache) {
    LOG(
        ("nsOfflineCacheUpdate::AssociateDocuments bypassed"
         ", no cache provided [this=%p]",
         this));
    return;
  }
||||||| merged common ancestors
void
nsOfflineCacheUpdate::AssociateDocuments(nsIApplicationCache* cache)
{
    if (!cache) {
        LOG(("nsOfflineCacheUpdate::AssociateDocuments bypassed"
             ", no cache provided [this=%p]", this));
        return;
    }
=======
void nsOfflineCacheUpdate::AssociateDocuments(nsIApplicationCache* cache) {
  if (!cache) {
    LOG(
        ("nsOfflineCacheUpdate::AssociateDocuments bypassed"
         ", no cache provided [this=%p]",
         this));
    return;
  }
>>>>>>> upstream-releases

  nsCOMArray<nsIOfflineCacheUpdateObserver> observers;
  GatherObservers(observers);

  for (int32_t i = 0; i < observers.Count(); i++) {
    observers[i]->ApplicationCacheAvailable(cache);
  }
}

<<<<<<< HEAD
void nsOfflineCacheUpdate::StickDocument(nsIURI *aDocumentURI) {
  if (!aDocumentURI) return;
||||||| merged common ancestors
void
nsOfflineCacheUpdate::StickDocument(nsIURI *aDocumentURI)
{
    if (!aDocumentURI)
      return;
=======
void nsOfflineCacheUpdate::StickDocument(nsIURI* aDocumentURI) {
  if (!aDocumentURI) return;
>>>>>>> upstream-releases

  mDocumentURIs.AppendObject(aDocumentURI);
}

<<<<<<< HEAD
void nsOfflineCacheUpdate::SetOwner(nsOfflineCacheUpdateOwner *aOwner) {
  NS_ASSERTION(!mOwner, "Tried to set cache update owner twice.");
  mOwner = aOwner;
||||||| merged common ancestors
void
nsOfflineCacheUpdate::SetOwner(nsOfflineCacheUpdateOwner *aOwner)
{
    NS_ASSERTION(!mOwner, "Tried to set cache update owner twice.");
    mOwner = aOwner;
=======
void nsOfflineCacheUpdate::SetOwner(nsOfflineCacheUpdateOwner* aOwner) {
  NS_ASSERTION(!mOwner, "Tried to set cache update owner twice.");
  mOwner = aOwner;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool nsOfflineCacheUpdate::IsForGroupID(const nsACString &groupID) {
  return mGroupID == groupID;
||||||| merged common ancestors
bool
nsOfflineCacheUpdate::IsForGroupID(const nsACString& groupID)
{
    return mGroupID == groupID;
=======
bool nsOfflineCacheUpdate::IsForGroupID(const nsACString& groupID) {
  return mGroupID == groupID;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool nsOfflineCacheUpdate::IsForProfile(nsIFile *aCustomProfileDir) {
  if (!mCustomProfileDir && !aCustomProfileDir) return true;
  if (!mCustomProfileDir || !aCustomProfileDir) return false;
||||||| merged common ancestors
bool
nsOfflineCacheUpdate::IsForProfile(nsIFile* aCustomProfileDir)
{
    if (!mCustomProfileDir && !aCustomProfileDir)
        return true;
    if (!mCustomProfileDir || !aCustomProfileDir)
        return false;
=======
bool nsOfflineCacheUpdate::IsForProfile(nsIFile* aCustomProfileDir) {
  if (!mCustomProfileDir && !aCustomProfileDir) return true;
  if (!mCustomProfileDir || !aCustomProfileDir) return false;
>>>>>>> upstream-releases

  bool equals;
  nsresult rv = mCustomProfileDir->Equals(aCustomProfileDir, &equals);

  return NS_SUCCEEDED(rv) && equals;
}

<<<<<<< HEAD
nsresult nsOfflineCacheUpdate::UpdateFinished(nsOfflineCacheUpdate *aUpdate) {
  // Keep the object alive through a Finish() call.
  nsCOMPtr<nsIOfflineCacheUpdate> kungFuDeathGrip(this);
||||||| merged common ancestors
nsresult
nsOfflineCacheUpdate::UpdateFinished(nsOfflineCacheUpdate *aUpdate)
{
    // Keep the object alive through a Finish() call.
    nsCOMPtr<nsIOfflineCacheUpdate> kungFuDeathGrip(this);
=======
nsresult nsOfflineCacheUpdate::UpdateFinished(nsOfflineCacheUpdate* aUpdate) {
  // Keep the object alive through a Finish() call.
  nsCOMPtr<nsIOfflineCacheUpdate> kungFuDeathGrip(this);
>>>>>>> upstream-releases

  mImplicitUpdate = nullptr;

  NotifyState(nsIOfflineCacheUpdateObserver::STATE_NOUPDATE);
  Finish();

  return NS_OK;
}

void nsOfflineCacheUpdate::OnByteProgress(uint64_t byteIncrement) {
  mByteProgress += byteIncrement;
  NotifyState(nsIOfflineCacheUpdateObserver::STATE_ITEMPROGRESS);
}

nsresult nsOfflineCacheUpdate::ScheduleImplicit() {
  if (mDocumentURIs.Count() == 0) return NS_OK;

  nsresult rv;

  RefPtr<nsOfflineCacheUpdate> update = new nsOfflineCacheUpdate();
  NS_ENSURE_TRUE(update, NS_ERROR_OUT_OF_MEMORY);

<<<<<<< HEAD
  nsAutoCString clientID;
  if (mPreviousApplicationCache) {
    rv = mPreviousApplicationCache->GetClientID(clientID);
||||||| merged common ancestors
    rv = update->InitPartial(mManifestURI, clientID, mDocumentURI, mLoadingPrincipal);
=======
  nsAutoCString clientID;
  if (mPreviousApplicationCache) {
    rv = mPreviousApplicationCache->GetClientID(clientID);
    NS_ENSURE_SUCCESS(rv, rv);
  } else if (mApplicationCache) {
    rv = mApplicationCache->GetClientID(clientID);
>>>>>>> upstream-releases
    NS_ENSURE_SUCCESS(rv, rv);
<<<<<<< HEAD
  } else if (mApplicationCache) {
    rv = mApplicationCache->GetClientID(clientID);
    NS_ENSURE_SUCCESS(rv, rv);
  } else {
    NS_ERROR("Offline cache update not having set mApplicationCache?");
  }
||||||| merged common ancestors
=======
  } else {
    NS_ERROR("Offline cache update not having set mApplicationCache?");
  }
>>>>>>> upstream-releases

  rv = update->InitPartial(mManifestURI, clientID, mDocumentURI,
                           mLoadingPrincipal);
  NS_ENSURE_SUCCESS(rv, rv);

  for (int32_t i = 0; i < mDocumentURIs.Count(); i++) {
    rv = update->AddURI(mDocumentURIs[i], nsIApplicationCache::ITEM_IMPLICIT);
    NS_ENSURE_SUCCESS(rv, rv);
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
  }

  update->SetOwner(this);
  rv = update->Begin();
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  update->SetOwner(this);
  rv = update->Begin();
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
    mImplicitUpdate = update;
=======
  mImplicitUpdate = update;
>>>>>>> upstream-releases

<<<<<<< HEAD
  mImplicitUpdate = update;

  return NS_OK;
||||||| merged common ancestors
    return NS_OK;
=======
  return NS_OK;
>>>>>>> upstream-releases
}

nsresult nsOfflineCacheUpdate::FinishNoNotify() {
  LOG(("nsOfflineCacheUpdate::Finish [%p]", this));

<<<<<<< HEAD
  mState = STATE_FINISHED;
||||||| merged common ancestors
        if (mObsolete) {
            nsCOMPtr<nsIApplicationCacheService> appCacheService =
                do_GetService(NS_APPLICATIONCACHESERVICE_CONTRACTID);
            if (appCacheService) {
                nsAutoCString groupID;
                mApplicationCache->GetGroupID(groupID);
                appCacheService->DeactivateGroup(groupID);
             }
        }
=======
  mState = STATE_FINISHED;

  if (!mPartialUpdate && !mOnlyCheckUpdate) {
    if (mSucceeded) {
      nsIArray* namespaces = mManifestItem->GetNamespaces();
      nsresult rv = mApplicationCache->AddNamespaces(namespaces);
      if (NS_FAILED(rv)) {
        NotifyState(nsIOfflineCacheUpdateObserver::STATE_ERROR);
        mSucceeded = false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!mPartialUpdate && !mOnlyCheckUpdate) {
    if (mSucceeded) {
      nsIArray *namespaces = mManifestItem->GetNamespaces();
      nsresult rv = mApplicationCache->AddNamespaces(namespaces);
      if (NS_FAILED(rv)) {
        NotifyState(nsIOfflineCacheUpdateObserver::STATE_ERROR);
        mSucceeded = false;
      }
||||||| merged common ancestors
        if (!mSucceeded) {
            // Update was not merged, mark all the loads as failures
            for (uint32_t i = 0; i < mItems.Length(); i++) {
                mItems[i]->Cancel();
            }
=======
      rv = mApplicationCache->Activate();
      if (NS_FAILED(rv)) {
        NotifyState(nsIOfflineCacheUpdateObserver::STATE_ERROR);
        mSucceeded = false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      rv = mApplicationCache->Activate();
      if (NS_FAILED(rv)) {
        NotifyState(nsIOfflineCacheUpdateObserver::STATE_ERROR);
        mSucceeded = false;
      }

      AssociateDocuments(mApplicationCache);
    }

    if (mObsolete) {
      nsCOMPtr<nsIApplicationCacheService> appCacheService =
          do_GetService(NS_APPLICATIONCACHESERVICE_CONTRACTID);
      if (appCacheService) {
        nsAutoCString groupID;
        mApplicationCache->GetGroupID(groupID);
        appCacheService->DeactivateGroup(groupID);
      }
||||||| merged common ancestors
            mApplicationCache->Discard();
        }
=======
      AssociateDocuments(mApplicationCache);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (!mSucceeded) {
      // Update was not merged, mark all the loads as failures
      for (uint32_t i = 0; i < mItems.Length(); i++) {
        mItems[i]->Cancel();
      }
||||||| merged common ancestors
    nsresult rv = NS_OK;
=======
    if (mObsolete) {
      nsCOMPtr<nsIApplicationCacheService> appCacheService =
          do_GetService(NS_APPLICATIONCACHESERVICE_CONTRACTID);
      if (appCacheService) {
        nsAutoCString groupID;
        mApplicationCache->GetGroupID(groupID);
        appCacheService->DeactivateGroup(groupID);
      }
    }

    if (!mSucceeded) {
      // Update was not merged, mark all the loads as failures
      for (uint32_t i = 0; i < mItems.Length(); i++) {
        mItems[i]->Cancel();
      }
>>>>>>> upstream-releases

      mApplicationCache->Discard();
    }
  }

  nsresult rv = NS_OK;

  if (mOwner) {
    rv = mOwner->UpdateFinished(this);
    // mozilla::WeakPtr is missing some key features, like setting it to
    // null explicitly.
    mOwner = mozilla::WeakPtr<nsOfflineCacheUpdateOwner>();
  }

  return rv;
}

nsresult nsOfflineCacheUpdate::Finish() {
  nsresult rv = FinishNoNotify();

  NotifyState(nsIOfflineCacheUpdateObserver::STATE_FINISHED);

  return rv;
}

void nsOfflineCacheUpdate::AsyncFinishWithError() {
  NotifyState(nsOfflineCacheUpdate::STATE_ERROR);
  Finish();
}

<<<<<<< HEAD
static nsresult EvictOneOfCacheGroups(nsIApplicationCacheService *cacheService,
                                      uint32_t count,
                                      const char *const *groups) {
  nsresult rv;
  unsigned int i;

  for (i = 0; i < count; i++) {
    nsCOMPtr<nsIURI> uri;
    rv = NS_NewURI(getter_AddRefs(uri), groups[i]);
    NS_ENSURE_SUCCESS(rv, rv);

    nsDependentCString group_name(groups[i]);
    nsCOMPtr<nsIApplicationCache> cache;
    rv = cacheService->GetActiveCache(group_name, getter_AddRefs(cache));
    // Maybe someone in another thread or process have deleted it.
    if (NS_FAILED(rv) || !cache) continue;

    bool pinned;
    rv = nsOfflineCacheUpdateService::OfflineAppPinnedForURI(uri, nullptr,
                                                             &pinned);
    NS_ENSURE_SUCCESS(rv, rv);

    if (!pinned) {
      rv = cache->Discard();
      return NS_OK;
    }
  }
||||||| merged common ancestors
static nsresult
EvictOneOfCacheGroups(nsIApplicationCacheService *cacheService,
                      uint32_t count, const char * const *groups)
{
    nsresult rv;
    unsigned int i;

    for (i = 0; i < count; i++) {
        nsCOMPtr<nsIURI> uri;
        rv = NS_NewURI(getter_AddRefs(uri), groups[i]);
        NS_ENSURE_SUCCESS(rv, rv);

        nsDependentCString group_name(groups[i]);
        nsCOMPtr<nsIApplicationCache> cache;
        rv = cacheService->GetActiveCache(group_name, getter_AddRefs(cache));
        // Maybe someone in another thread or process have deleted it.
        if (NS_FAILED(rv) || !cache)
            continue;

        bool pinned;
        rv = nsOfflineCacheUpdateService::OfflineAppPinnedForURI(uri,
                                                                 nullptr,
                                                                 &pinned);
        NS_ENSURE_SUCCESS(rv, rv);

        if (!pinned) {
            rv = cache->Discard();
            return NS_OK;
        }
    }
=======
static nsresult EvictOneOfCacheGroups(nsIApplicationCacheService* cacheService,
                                      const nsTArray<nsCString>& groups) {
  nsresult rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return NS_ERROR_FILE_NOT_FOUND;
}
||||||| merged common ancestors
    return NS_ERROR_FILE_NOT_FOUND;
}
=======
  for (auto& group : groups) {
    nsCOMPtr<nsIURI> uri;
    rv = NS_NewURI(getter_AddRefs(uri), group);
    NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsOfflineCacheUpdate::EvictOneNonPinned() {
  nsresult rv;
||||||| merged common ancestors
nsresult
nsOfflineCacheUpdate::EvictOneNonPinned()
{
    nsresult rv;
=======
    nsCOMPtr<nsIApplicationCache> cache;
    rv = cacheService->GetActiveCache(group, getter_AddRefs(cache));
    // Maybe someone in another thread or process have deleted it.
    if (NS_FAILED(rv) || !cache) continue;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIApplicationCacheService> cacheService =
      do_GetService(NS_APPLICATIONCACHESERVICE_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
    nsCOMPtr<nsIApplicationCacheService> cacheService =
        do_GetService(NS_APPLICATIONCACHESERVICE_CONTRACTID, &rv);
    NS_ENSURE_SUCCESS(rv, rv);
=======
    bool pinned;
    rv = nsOfflineCacheUpdateService::OfflineAppPinnedForURI(uri, nullptr,
                                                             &pinned);
    NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t count;
  char **groups;
  rv = cacheService->GetGroupsTimeOrdered(&count, &groups);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
    uint32_t count;
    char **groups;
    rv = cacheService->GetGroupsTimeOrdered(&count, &groups);
    NS_ENSURE_SUCCESS(rv, rv);
=======
    if (!pinned) {
      rv = cache->Discard();
      return NS_OK;
    }
  }

  return NS_ERROR_FILE_NOT_FOUND;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  rv = EvictOneOfCacheGroups(cacheService, count, groups);
||||||| merged common ancestors
    rv = EvictOneOfCacheGroups(cacheService, count, groups);
=======
nsresult nsOfflineCacheUpdate::EvictOneNonPinned() {
  nsresult rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
  NS_FREE_XPCOM_ALLOCATED_POINTER_ARRAY(count, groups);
  return rv;
||||||| merged common ancestors
    NS_FREE_XPCOM_ALLOCATED_POINTER_ARRAY(count, groups);
    return rv;
=======
  nsCOMPtr<nsIApplicationCacheService> cacheService =
      do_GetService(NS_APPLICATIONCACHESERVICE_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  nsTArray<nsCString> groups;
  rv = cacheService->GetGroupsTimeOrdered(groups);
  NS_ENSURE_SUCCESS(rv, rv);

  return EvictOneOfCacheGroups(cacheService, groups);
>>>>>>> upstream-releases
}

//-----------------------------------------------------------------------------
// nsOfflineCacheUpdate::nsIOfflineCacheUpdate
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdate::GetUpdateDomain(nsACString &aUpdateDomain) {
  NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);
||||||| merged common ancestors
nsOfflineCacheUpdate::GetUpdateDomain(nsACString &aUpdateDomain)
{
    NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);
=======
nsOfflineCacheUpdate::GetUpdateDomain(nsACString& aUpdateDomain) {
  NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);
>>>>>>> upstream-releases

  aUpdateDomain = mUpdateDomain;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdate::GetStatus(uint16_t *aStatus) {
  switch (mState) {
    case STATE_CHECKING:
      *aStatus = dom::OfflineResourceList_Binding::CHECKING;
      return NS_OK;
    case STATE_DOWNLOADING:
      *aStatus = dom::OfflineResourceList_Binding::DOWNLOADING;
      return NS_OK;
    default:
      *aStatus = dom::OfflineResourceList_Binding::IDLE;
      return NS_OK;
  }
||||||| merged common ancestors
nsOfflineCacheUpdate::GetStatus(uint16_t *aStatus)
{
    switch (mState) {
    case STATE_CHECKING :
        *aStatus = dom::OfflineResourceList_Binding::CHECKING;
        return NS_OK;
    case STATE_DOWNLOADING :
        *aStatus = dom::OfflineResourceList_Binding::DOWNLOADING;
        return NS_OK;
    default :
        *aStatus = dom::OfflineResourceList_Binding::IDLE;
        return NS_OK;
    }
=======
nsOfflineCacheUpdate::GetStatus(uint16_t* aStatus) {
  switch (mState) {
    case STATE_CHECKING:
      *aStatus = dom::OfflineResourceList_Binding::CHECKING;
      return NS_OK;
    case STATE_DOWNLOADING:
      *aStatus = dom::OfflineResourceList_Binding::DOWNLOADING;
      return NS_OK;
    default:
      *aStatus = dom::OfflineResourceList_Binding::IDLE;
      return NS_OK;
  }
>>>>>>> upstream-releases

  return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdate::GetPartial(bool *aPartial) {
  *aPartial = mPartialUpdate || mOnlyCheckUpdate;
  return NS_OK;
||||||| merged common ancestors
nsOfflineCacheUpdate::GetPartial(bool *aPartial)
{
    *aPartial = mPartialUpdate || mOnlyCheckUpdate;
    return NS_OK;
=======
nsOfflineCacheUpdate::GetPartial(bool* aPartial) {
  *aPartial = mPartialUpdate || mOnlyCheckUpdate;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdate::GetManifestURI(nsIURI **aManifestURI) {
  NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);
||||||| merged common ancestors
nsOfflineCacheUpdate::GetManifestURI(nsIURI **aManifestURI)
{
    NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);
=======
nsOfflineCacheUpdate::GetManifestURI(nsIURI** aManifestURI) {
  NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);
>>>>>>> upstream-releases

  NS_IF_ADDREF(*aManifestURI = mManifestURI);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdate::GetSucceeded(bool *aSucceeded) {
  NS_ENSURE_TRUE(mState == STATE_FINISHED, NS_ERROR_NOT_AVAILABLE);
||||||| merged common ancestors
nsOfflineCacheUpdate::GetSucceeded(bool *aSucceeded)
{
    NS_ENSURE_TRUE(mState == STATE_FINISHED, NS_ERROR_NOT_AVAILABLE);
=======
nsOfflineCacheUpdate::GetSucceeded(bool* aSucceeded) {
  NS_ENSURE_TRUE(mState == STATE_FINISHED, NS_ERROR_NOT_AVAILABLE);
>>>>>>> upstream-releases

  *aSucceeded = mSucceeded;

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdate::GetIsUpgrade(bool *aIsUpgrade) {
  NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);
||||||| merged common ancestors
nsOfflineCacheUpdate::GetIsUpgrade(bool *aIsUpgrade)
{
    NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);
=======
nsOfflineCacheUpdate::GetIsUpgrade(bool* aIsUpgrade) {
  NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);
>>>>>>> upstream-releases

  *aIsUpgrade = (mPreviousApplicationCache != nullptr);

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsOfflineCacheUpdate::AddURI(nsIURI *aURI, uint32_t aType,
                                      uint32_t aLoadFlags) {
  NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);

  if (mState >= STATE_DOWNLOADING) return NS_ERROR_NOT_AVAILABLE;

  // Resource URIs must have the same scheme as the manifest.
  nsAutoCString scheme;
  aURI->GetScheme(scheme);

  bool match;
  if (NS_FAILED(mManifestURI->SchemeIs(scheme.get(), &match)) || !match)
    return NS_ERROR_FAILURE;

  // Don't fetch the same URI twice.
  for (uint32_t i = 0; i < mItems.Length(); i++) {
    bool equals;
    if (NS_SUCCEEDED(mItems[i]->mURI->Equals(aURI, &equals)) && equals &&
        mItems[i]->mLoadFlags == aLoadFlags) {
      // retain both types.
      mItems[i]->mItemType |= aType;
      return NS_OK;
||||||| merged common ancestors
nsresult
nsOfflineCacheUpdate::AddURI(nsIURI *aURI, uint32_t aType, uint32_t aLoadFlags)
{
    NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);

    if (mState >= STATE_DOWNLOADING)
        return NS_ERROR_NOT_AVAILABLE;

    // Resource URIs must have the same scheme as the manifest.
    nsAutoCString scheme;
    aURI->GetScheme(scheme);

    bool match;
    if (NS_FAILED(mManifestURI->SchemeIs(scheme.get(), &match)) || !match)
        return NS_ERROR_FAILURE;

    // Don't fetch the same URI twice.
    for (uint32_t i = 0; i < mItems.Length(); i++) {
        bool equals;
        if (NS_SUCCEEDED(mItems[i]->mURI->Equals(aURI, &equals)) && equals &&
            mItems[i]->mLoadFlags == aLoadFlags) {
            // retain both types.
            mItems[i]->mItemType |= aType;
            return NS_OK;
        }
=======
nsresult nsOfflineCacheUpdate::AddURI(nsIURI* aURI, uint32_t aType,
                                      uint32_t aLoadFlags) {
  NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);

  if (mState >= STATE_DOWNLOADING) return NS_ERROR_NOT_AVAILABLE;

  // Resource URIs must have the same scheme as the manifest.
  nsAutoCString scheme;
  aURI->GetScheme(scheme);

  bool match;
  if (NS_FAILED(mManifestURI->SchemeIs(scheme.get(), &match)) || !match)
    return NS_ERROR_FAILURE;

  // Don't fetch the same URI twice.
  for (uint32_t i = 0; i < mItems.Length(); i++) {
    bool equals;
    if (NS_SUCCEEDED(mItems[i]->mURI->Equals(aURI, &equals)) && equals &&
        mItems[i]->mLoadFlags == aLoadFlags) {
      // retain both types.
      mItems[i]->mItemType |= aType;
      return NS_OK;
>>>>>>> upstream-releases
    }
  }

  RefPtr<nsOfflineCacheUpdateItem> item = new nsOfflineCacheUpdateItem(
      aURI, mDocumentURI, mLoadingPrincipal, mApplicationCache,
      mPreviousApplicationCache, aType, aLoadFlags);
  if (!item) return NS_ERROR_OUT_OF_MEMORY;

  mItems.AppendElement(item);
  mAddedItems = true;

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdate::AddDynamicURI(nsIURI *aURI) {
  if (GeckoProcessType_Default != XRE_GetProcessType())
    return NS_ERROR_NOT_IMPLEMENTED;

  // If this is a partial update and the resource is already in the
  // cache, we should only mark the entry, not fetch it again.
  if (mPartialUpdate) {
    nsAutoCString key;
    GetCacheKey(aURI, key);

    uint32_t types;
    nsresult rv = mApplicationCache->GetTypes(key, &types);
    if (NS_SUCCEEDED(rv)) {
      if (!(types & nsIApplicationCache::ITEM_DYNAMIC)) {
        mApplicationCache->MarkEntry(key, nsIApplicationCache::ITEM_DYNAMIC);
      }
      return NS_OK;
||||||| merged common ancestors
nsOfflineCacheUpdate::AddDynamicURI(nsIURI *aURI)
{
    if (GeckoProcessType_Default != XRE_GetProcessType())
        return NS_ERROR_NOT_IMPLEMENTED;

    // If this is a partial update and the resource is already in the
    // cache, we should only mark the entry, not fetch it again.
    if (mPartialUpdate) {
        nsAutoCString key;
        GetCacheKey(aURI, key);

        uint32_t types;
        nsresult rv = mApplicationCache->GetTypes(key, &types);
        if (NS_SUCCEEDED(rv)) {
            if (!(types & nsIApplicationCache::ITEM_DYNAMIC)) {
                mApplicationCache->MarkEntry
                    (key, nsIApplicationCache::ITEM_DYNAMIC);
            }
            return NS_OK;
        }
=======
nsOfflineCacheUpdate::AddDynamicURI(nsIURI* aURI) {
  if (GeckoProcessType_Default != XRE_GetProcessType())
    return NS_ERROR_NOT_IMPLEMENTED;

  // If this is a partial update and the resource is already in the
  // cache, we should only mark the entry, not fetch it again.
  if (mPartialUpdate) {
    nsAutoCString key;
    GetCacheKey(aURI, key);

    uint32_t types;
    nsresult rv = mApplicationCache->GetTypes(key, &types);
    if (NS_SUCCEEDED(rv)) {
      if (!(types & nsIApplicationCache::ITEM_DYNAMIC)) {
        mApplicationCache->MarkEntry(key, nsIApplicationCache::ITEM_DYNAMIC);
      }
      return NS_OK;
>>>>>>> upstream-releases
    }
  }

  return AddURI(aURI, nsIApplicationCache::ITEM_DYNAMIC);
}

NS_IMETHODIMP
nsOfflineCacheUpdate::Cancel() {
  LOG(("nsOfflineCacheUpdate::Cancel [%p]", this));

  if ((mState == STATE_FINISHED) || (mState == STATE_CANCELLED)) {
    return NS_ERROR_NOT_AVAILABLE;
  }

  mState = STATE_CANCELLED;
  mSucceeded = false;

<<<<<<< HEAD
  // Cancel all running downloads
  for (uint32_t i = 0; i < mItems.Length(); ++i) {
    nsOfflineCacheUpdateItem *item = mItems[i];
||||||| merged common ancestors
    // Cancel all running downloads
    for (uint32_t i = 0; i < mItems.Length(); ++i) {
        nsOfflineCacheUpdateItem * item = mItems[i];
=======
  // Cancel all running downloads
  for (uint32_t i = 0; i < mItems.Length(); ++i) {
    nsOfflineCacheUpdateItem* item = mItems[i];
>>>>>>> upstream-releases

    if (item->IsInProgress()) item->Cancel();
  }

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdate::AddObserver(nsIOfflineCacheUpdateObserver *aObserver,
                                  bool aHoldWeak) {
  LOG(("nsOfflineCacheUpdate::AddObserver [%p] to update [%p]", aObserver,
       this));

  NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);

  if (aHoldWeak) {
    nsWeakPtr weakRef = do_GetWeakReference(aObserver);
    mWeakObservers.AppendObject(weakRef);
  } else {
    mObservers.AppendObject(aObserver);
  }
||||||| merged common ancestors
nsOfflineCacheUpdate::AddObserver(nsIOfflineCacheUpdateObserver *aObserver,
                                  bool aHoldWeak)
{
    LOG(("nsOfflineCacheUpdate::AddObserver [%p] to update [%p]", aObserver, this));

    NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);

    if (aHoldWeak) {
        nsCOMPtr<nsIWeakReference> weakRef = do_GetWeakReference(aObserver);
        mWeakObservers.AppendObject(weakRef);
    } else {
        mObservers.AppendObject(aObserver);
    }
=======
nsOfflineCacheUpdate::AddObserver(nsIOfflineCacheUpdateObserver* aObserver,
                                  bool aHoldWeak) {
  LOG(("nsOfflineCacheUpdate::AddObserver [%p] to update [%p]", aObserver,
       this));

  NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);

  if (aHoldWeak) {
    nsWeakPtr weakRef = do_GetWeakReference(aObserver);
    mWeakObservers.AppendObject(weakRef);
  } else {
    mObservers.AppendObject(aObserver);
  }
>>>>>>> upstream-releases

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdate::RemoveObserver(nsIOfflineCacheUpdateObserver *aObserver) {
  LOG(("nsOfflineCacheUpdate::RemoveObserver [%p] from update [%p]", aObserver,
       this));
||||||| merged common ancestors
nsOfflineCacheUpdate::RemoveObserver(nsIOfflineCacheUpdateObserver *aObserver)
{
    LOG(("nsOfflineCacheUpdate::RemoveObserver [%p] from update [%p]", aObserver, this));
=======
nsOfflineCacheUpdate::RemoveObserver(nsIOfflineCacheUpdateObserver* aObserver) {
  LOG(("nsOfflineCacheUpdate::RemoveObserver [%p] from update [%p]", aObserver,
       this));
>>>>>>> upstream-releases

  NS_ENSURE_TRUE(mState >= STATE_INITIALIZED, NS_ERROR_NOT_INITIALIZED);

  for (int32_t i = 0; i < mWeakObservers.Count(); i++) {
    nsCOMPtr<nsIOfflineCacheUpdateObserver> observer =
        do_QueryReferent(mWeakObservers[i]);
    if (observer == aObserver) {
      mWeakObservers.RemoveObjectAt(i);
      return NS_OK;
    }
  }

  for (int32_t i = 0; i < mObservers.Count(); i++) {
    if (mObservers[i] == aObserver) {
      mObservers.RemoveObjectAt(i);
      return NS_OK;
    }
  }

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdate::GetByteProgress(uint64_t *_result) {
  NS_ENSURE_ARG(_result);
||||||| merged common ancestors
nsOfflineCacheUpdate::GetByteProgress(uint64_t * _result)
{
    NS_ENSURE_ARG(_result);
=======
nsOfflineCacheUpdate::GetByteProgress(uint64_t* _result) {
  NS_ENSURE_ARG(_result);
>>>>>>> upstream-releases

  *_result = mByteProgress;
  return NS_OK;
}

NS_IMETHODIMP
nsOfflineCacheUpdate::Schedule() {
  LOG(("nsOfflineCacheUpdate::Schedule [%p]", this));

<<<<<<< HEAD
  nsOfflineCacheUpdateService *service =
      nsOfflineCacheUpdateService::EnsureService();
||||||| merged common ancestors
    nsOfflineCacheUpdateService* service =
        nsOfflineCacheUpdateService::EnsureService();
=======
  nsOfflineCacheUpdateService* service =
      nsOfflineCacheUpdateService::EnsureService();
>>>>>>> upstream-releases

  if (!service) {
    return NS_ERROR_FAILURE;
  }

  return service->ScheduleUpdate(this);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdate::UpdateStateChanged(nsIOfflineCacheUpdate *aUpdate,
                                         uint32_t aState) {
  if (aState == nsIOfflineCacheUpdateObserver::STATE_FINISHED) {
    // Take the mSucceeded flag from the underlying update, we will be
    // queried for it soon. mSucceeded of this update is false (manifest
    // check failed) but the subsequent re-fetch update might succeed
    bool succeeded;
    aUpdate->GetSucceeded(&succeeded);
    mSucceeded = succeeded;
  }
||||||| merged common ancestors
nsOfflineCacheUpdate::UpdateStateChanged(nsIOfflineCacheUpdate *aUpdate,
                                         uint32_t aState)
{
    if (aState == nsIOfflineCacheUpdateObserver::STATE_FINISHED) {
        // Take the mSucceeded flag from the underlying update, we will be
        // queried for it soon. mSucceeded of this update is false (manifest
        // check failed) but the subsequent re-fetch update might succeed
        bool succeeded;
        aUpdate->GetSucceeded(&succeeded);
        mSucceeded = succeeded;
    }
=======
nsOfflineCacheUpdate::UpdateStateChanged(nsIOfflineCacheUpdate* aUpdate,
                                         uint32_t aState) {
  if (aState == nsIOfflineCacheUpdateObserver::STATE_FINISHED) {
    // Take the mSucceeded flag from the underlying update, we will be
    // queried for it soon. mSucceeded of this update is false (manifest
    // check failed) but the subsequent re-fetch update might succeed
    bool succeeded;
    aUpdate->GetSucceeded(&succeeded);
    mSucceeded = succeeded;
  }
>>>>>>> upstream-releases

  NotifyState(aState);
  if (aState == nsIOfflineCacheUpdateObserver::STATE_FINISHED)
    aUpdate->RemoveObserver(this);

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOfflineCacheUpdate::ApplicationCacheAvailable(
    nsIApplicationCache *applicationCache) {
  AssociateDocuments(applicationCache);
  return NS_OK;
||||||| merged common ancestors
nsOfflineCacheUpdate::ApplicationCacheAvailable(nsIApplicationCache *applicationCache)
{
    AssociateDocuments(applicationCache);
    return NS_OK;
=======
nsOfflineCacheUpdate::ApplicationCacheAvailable(
    nsIApplicationCache* applicationCache) {
  AssociateDocuments(applicationCache);
  return NS_OK;
>>>>>>> upstream-releases
}

//-----------------------------------------------------------------------------
// nsOfflineCacheUpdate::nsIRunable
//-----------------------------------------------------------------------------

NS_IMETHODIMP
nsOfflineCacheUpdate::Run() {
  ProcessNextURI();
  return NS_OK;
}
