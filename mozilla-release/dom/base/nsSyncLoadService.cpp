/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * A service that provides methods for synchronously loading a DOM in various
 * ways.
 */

#include "nsSyncLoadService.h"
#include "nsCOMPtr.h"
#include "nsIChannel.h"
#include "nsIChannelEventSink.h"
#include "nsIAsyncVerifyRedirectCallback.h"
#include "nsIInterfaceRequestor.h"
#include "nsIStreamListener.h"
#include "nsIURI.h"
#include "nsString.h"
#include "nsWeakReference.h"
#include "mozilla/dom/Document.h"
#include "nsIPrincipal.h"
#include "nsContentUtils.h"  // for kLoadAsData
#include "nsThreadUtils.h"
#include "nsNetUtil.h"
#include "nsStreamUtils.h"
#include "ReferrerInfo.h"
#include <algorithm>

using namespace mozilla;
using namespace mozilla::dom;

using mozilla::net::ReferrerPolicy;

/**
 * This class manages loading a single XML document
 */

class nsSyncLoader : public nsIStreamListener,
                     public nsIChannelEventSink,
                     public nsIInterfaceRequestor,
                     public nsSupportsWeakReference {
 public:
  nsSyncLoader()
      : mLoading(false), mAsyncLoadStatus(NS_ERROR_NOT_INITIALIZED) {}

  NS_DECL_ISUPPORTS

<<<<<<< HEAD
  nsresult LoadDocument(nsIChannel *aChannel, bool aChannelIsSync,
                        bool aForceToXML, ReferrerPolicy aReferrerPolicy,
                        nsIDocument **aResult);
||||||| merged common ancestors
    nsresult LoadDocument(nsIChannel* aChannel,
                          bool aChannelIsSync, bool aForceToXML,
                          ReferrerPolicy aReferrerPolicy,
                          nsIDocument** aResult);
=======
  nsresult LoadDocument(nsIChannel* aChannel, bool aChannelIsSync,
                        bool aForceToXML, ReferrerPolicy aReferrerPolicy,
                        Document** aResult);
>>>>>>> upstream-releases

  NS_FORWARD_NSISTREAMLISTENER(mListener->)
  NS_DECL_NSIREQUESTOBSERVER

  NS_DECL_NSICHANNELEVENTSINK

  NS_DECL_NSIINTERFACEREQUESTOR

 private:
  virtual ~nsSyncLoader();

<<<<<<< HEAD
  nsresult PushAsyncStream(nsIStreamListener *aListener);
  nsresult PushSyncStream(nsIStreamListener *aListener);
||||||| merged common ancestors
    nsresult PushAsyncStream(nsIStreamListener* aListener);
    nsresult PushSyncStream(nsIStreamListener* aListener);
=======
  nsresult PushAsyncStream(nsIStreamListener* aListener);
  nsresult PushSyncStream(nsIStreamListener* aListener);
>>>>>>> upstream-releases

  nsCOMPtr<nsIChannel> mChannel;
  nsCOMPtr<nsIStreamListener> mListener;
  bool mLoading;
  nsresult mAsyncLoadStatus;
};

class nsForceXMLListener : public nsIStreamListener {
  virtual ~nsForceXMLListener();

<<<<<<< HEAD
 public:
  explicit nsForceXMLListener(nsIStreamListener *aListener);
||||||| merged common ancestors
public:
    explicit nsForceXMLListener(nsIStreamListener* aListener);
=======
 public:
  explicit nsForceXMLListener(nsIStreamListener* aListener);
>>>>>>> upstream-releases

  NS_DECL_ISUPPORTS
  NS_FORWARD_NSISTREAMLISTENER(mListener->)
  NS_DECL_NSIREQUESTOBSERVER

 private:
  nsCOMPtr<nsIStreamListener> mListener;
};

<<<<<<< HEAD
nsForceXMLListener::nsForceXMLListener(nsIStreamListener *aListener)
    : mListener(aListener) {}
||||||| merged common ancestors
nsForceXMLListener::nsForceXMLListener(nsIStreamListener* aListener)
    : mListener(aListener)
{
}
=======
nsForceXMLListener::nsForceXMLListener(nsIStreamListener* aListener)
    : mListener(aListener) {}
>>>>>>> upstream-releases

nsForceXMLListener::~nsForceXMLListener() {}

NS_IMPL_ISUPPORTS(nsForceXMLListener, nsIStreamListener, nsIRequestObserver)

NS_IMETHODIMP
<<<<<<< HEAD
nsForceXMLListener::OnStartRequest(nsIRequest *aRequest,
                                   nsISupports *aContext) {
  nsresult status;
  aRequest->GetStatus(&status);
  nsCOMPtr<nsIChannel> channel = do_QueryInterface(aRequest);
  if (channel && NS_SUCCEEDED(status)) {
    channel->SetContentType(NS_LITERAL_CSTRING("text/xml"));
  }

  return mListener->OnStartRequest(aRequest, aContext);
||||||| merged common ancestors
nsForceXMLListener::OnStartRequest(nsIRequest *aRequest, nsISupports *aContext)
{
    nsresult status;
    aRequest->GetStatus(&status);
    nsCOMPtr<nsIChannel> channel = do_QueryInterface(aRequest);
    if (channel && NS_SUCCEEDED(status)) {
      channel->SetContentType(NS_LITERAL_CSTRING("text/xml"));
    }

    return mListener->OnStartRequest(aRequest, aContext);
=======
nsForceXMLListener::OnStartRequest(nsIRequest* aRequest) {
  nsresult status;
  aRequest->GetStatus(&status);
  nsCOMPtr<nsIChannel> channel = do_QueryInterface(aRequest);
  if (channel && NS_SUCCEEDED(status)) {
    channel->SetContentType(NS_LITERAL_CSTRING("text/xml"));
  }

  return mListener->OnStartRequest(aRequest);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsForceXMLListener::OnStopRequest(nsIRequest *aRequest, nsISupports *aContext,
                                  nsresult aStatusCode) {
  return mListener->OnStopRequest(aRequest, aContext, aStatusCode);
||||||| merged common ancestors
nsForceXMLListener::OnStopRequest(nsIRequest *aRequest, nsISupports *aContext,
                                  nsresult aStatusCode)
{
    return mListener->OnStopRequest(aRequest, aContext, aStatusCode);
=======
nsForceXMLListener::OnStopRequest(nsIRequest* aRequest, nsresult aStatusCode) {
  return mListener->OnStopRequest(aRequest, aStatusCode);
>>>>>>> upstream-releases
}

nsSyncLoader::~nsSyncLoader() {
  if (mLoading && mChannel) {
    mChannel->Cancel(NS_BINDING_ABORTED);
  }
}

NS_IMPL_ISUPPORTS(nsSyncLoader, nsIStreamListener, nsIRequestObserver,
                  nsIChannelEventSink, nsIInterfaceRequestor,
                  nsISupportsWeakReference)

<<<<<<< HEAD
nsresult nsSyncLoader::LoadDocument(nsIChannel *aChannel, bool aChannelIsSync,
                                    bool aForceToXML,
                                    ReferrerPolicy aReferrerPolicy,
                                    nsIDocument **aResult) {
  NS_ENSURE_ARG(aChannel);
  NS_ENSURE_ARG_POINTER(aResult);
  *aResult = nullptr;
  nsresult rv = NS_OK;

  mChannel = aChannel;
  nsCOMPtr<nsIHttpChannel> http = do_QueryInterface(mChannel);
  if (http) {
    rv = http->SetRequestHeader(
        NS_LITERAL_CSTRING("Accept"),
        NS_LITERAL_CSTRING(
            "text/xml,application/xml,application/xhtml+xml,*/*;q=0.1"),
        false);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    nsCOMPtr<nsILoadInfo> loadInfo = aChannel->GetLoadInfo();
    if (loadInfo) {
      nsCOMPtr<nsIURI> loaderUri;
      loadInfo->TriggeringPrincipal()->GetURI(getter_AddRefs(loaderUri));
      if (loaderUri) {
        rv = http->SetReferrerWithPolicy(loaderUri, aReferrerPolicy);
        MOZ_ASSERT(NS_SUCCEEDED(rv));
      }
||||||| merged common ancestors
nsresult
nsSyncLoader::LoadDocument(nsIChannel* aChannel,
                           bool aChannelIsSync,
                           bool aForceToXML,
                           ReferrerPolicy aReferrerPolicy,
                           nsIDocument **aResult)
{
    NS_ENSURE_ARG(aChannel);
    NS_ENSURE_ARG_POINTER(aResult);
    *aResult = nullptr;
    nsresult rv = NS_OK;

    mChannel = aChannel;
    nsCOMPtr<nsIHttpChannel> http = do_QueryInterface(mChannel);
    if (http) {
        rv = http->SetRequestHeader(NS_LITERAL_CSTRING("Accept"),
                                    NS_LITERAL_CSTRING("text/xml,application/xml,application/xhtml+xml,*/*;q=0.1"),
                                    false);
        MOZ_ASSERT(NS_SUCCEEDED(rv));
        nsCOMPtr<nsILoadInfo> loadInfo = aChannel->GetLoadInfo();
        if (loadInfo) {
            nsCOMPtr<nsIURI> loaderUri;
            loadInfo->TriggeringPrincipal()->GetURI(getter_AddRefs(loaderUri));
            if (loaderUri) {
                rv = http->SetReferrerWithPolicy(loaderUri, aReferrerPolicy);
                MOZ_ASSERT(NS_SUCCEEDED(rv));
            }
        }
=======
nsresult nsSyncLoader::LoadDocument(nsIChannel* aChannel, bool aChannelIsSync,
                                    bool aForceToXML,
                                    ReferrerPolicy aReferrerPolicy,
                                    Document** aResult) {
  NS_ENSURE_ARG(aChannel);
  NS_ENSURE_ARG_POINTER(aResult);
  *aResult = nullptr;
  nsresult rv = NS_OK;

  mChannel = aChannel;
  nsCOMPtr<nsIHttpChannel> http = do_QueryInterface(mChannel);
  if (http) {
    rv = http->SetRequestHeader(
        NS_LITERAL_CSTRING("Accept"),
        NS_LITERAL_CSTRING(
            "text/xml,application/xml,application/xhtml+xml,*/*;q=0.1"),
        false);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
    nsCOMPtr<nsILoadInfo> loadInfo = aChannel->LoadInfo();
    nsCOMPtr<nsIURI> loaderUri;
    loadInfo->TriggeringPrincipal()->GetURI(getter_AddRefs(loaderUri));
    if (loaderUri) {
      nsCOMPtr<nsIReferrerInfo> referrerInfo =
          new ReferrerInfo(loaderUri, aReferrerPolicy);
      rv = http->SetReferrerInfoWithoutClone(referrerInfo);
      MOZ_ASSERT(NS_SUCCEEDED(rv));
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

  // Hook us up to listen to redirects and the like.
  // Do this before setting up the cross-site proxy since
  // that installs its own proxies.
  mChannel->SetNotificationCallbacks(this);

  // Get the loadgroup of the channel
  nsCOMPtr<nsILoadGroup> loadGroup;
  rv = aChannel->GetLoadGroup(getter_AddRefs(loadGroup));
  NS_ENSURE_SUCCESS(rv, rv);

  // Create document
  nsCOMPtr<nsIDocument> document;
  rv = NS_NewXMLDocument(getter_AddRefs(document));
  NS_ENSURE_SUCCESS(rv, rv);

  // Start the document load. Do this before we attach the load listener
  // since we reset the document which drops all observers.
  nsCOMPtr<nsIStreamListener> listener;
  rv = document->StartDocumentLoad(kLoadAsData, mChannel, loadGroup, nullptr,
                                   getter_AddRefs(listener), true);
  NS_ENSURE_SUCCESS(rv, rv);

  if (aForceToXML) {
    nsCOMPtr<nsIStreamListener> forceListener =
        new nsForceXMLListener(listener);
    listener.swap(forceListener);
  }

  if (aChannelIsSync) {
    rv = PushSyncStream(listener);
  } else {
    rv = PushAsyncStream(listener);
  }

  http = do_QueryInterface(mChannel);
  if (NS_SUCCEEDED(rv) && http) {
    bool succeeded;
    if (NS_FAILED(http->GetRequestSucceeded(&succeeded)) || !succeeded) {
      rv = NS_ERROR_FAILURE;
||||||| merged common ancestors

    // Hook us up to listen to redirects and the like.
    // Do this before setting up the cross-site proxy since
    // that installs its own proxies.
    mChannel->SetNotificationCallbacks(this);

    // Get the loadgroup of the channel
    nsCOMPtr<nsILoadGroup> loadGroup;
    rv = aChannel->GetLoadGroup(getter_AddRefs(loadGroup));
    NS_ENSURE_SUCCESS(rv, rv);

    // Create document
    nsCOMPtr<nsIDocument> document;
    rv = NS_NewXMLDocument(getter_AddRefs(document));
    NS_ENSURE_SUCCESS(rv, rv);

    // Start the document load. Do this before we attach the load listener
    // since we reset the document which drops all observers.
    nsCOMPtr<nsIStreamListener> listener;
    rv = document->StartDocumentLoad(kLoadAsData, mChannel,
                                     loadGroup, nullptr,
                                     getter_AddRefs(listener),
                                     true);
    NS_ENSURE_SUCCESS(rv, rv);

    if (aForceToXML) {
        nsCOMPtr<nsIStreamListener> forceListener =
            new nsForceXMLListener(listener);
        listener.swap(forceListener);
=======
  }

  // Hook us up to listen to redirects and the like.
  // Do this before setting up the cross-site proxy since
  // that installs its own proxies.
  mChannel->SetNotificationCallbacks(this);

  // Get the loadgroup of the channel
  nsCOMPtr<nsILoadGroup> loadGroup;
  rv = aChannel->GetLoadGroup(getter_AddRefs(loadGroup));
  NS_ENSURE_SUCCESS(rv, rv);

  // Create document
  nsCOMPtr<Document> document;
  rv = NS_NewXMLDocument(getter_AddRefs(document));
  NS_ENSURE_SUCCESS(rv, rv);

  // Start the document load. Do this before we attach the load listener
  // since we reset the document which drops all observers.
  nsCOMPtr<nsIStreamListener> listener;
  rv = document->StartDocumentLoad(kLoadAsData, mChannel, loadGroup, nullptr,
                                   getter_AddRefs(listener), true);
  NS_ENSURE_SUCCESS(rv, rv);

  if (aForceToXML) {
    nsCOMPtr<nsIStreamListener> forceListener =
        new nsForceXMLListener(listener);
    listener.swap(forceListener);
  }

  if (aChannelIsSync) {
    rv = PushSyncStream(listener);
  } else {
    rv = PushAsyncStream(listener);
  }

  http = do_QueryInterface(mChannel);
  if (NS_SUCCEEDED(rv) && http) {
    bool succeeded;
    if (NS_FAILED(http->GetRequestSucceeded(&succeeded)) || !succeeded) {
      rv = NS_ERROR_FAILURE;
>>>>>>> upstream-releases
    }
  }
  mChannel = nullptr;

  // check that the load succeeded
  NS_ENSURE_SUCCESS(rv, rv);

  NS_ENSURE_TRUE(document->GetRootElement(), NS_ERROR_FAILURE);

  document.forget(aResult);

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsSyncLoader::PushAsyncStream(nsIStreamListener *aListener) {
  mListener = aListener;
||||||| merged common ancestors
    document.forget(aResult);
=======
nsresult nsSyncLoader::PushAsyncStream(nsIStreamListener* aListener) {
  mListener = aListener;
>>>>>>> upstream-releases

  mAsyncLoadStatus = NS_OK;

<<<<<<< HEAD
  // Start reading from the channel
  nsresult rv = mChannel->AsyncOpen2(this);

  if (NS_SUCCEEDED(rv)) {
    // process events until we're finished.
    mLoading = true;
    nsIThread *thread = NS_GetCurrentThread();
    while (mLoading && NS_SUCCEEDED(rv)) {
      bool processedEvent;
      rv = thread->ProcessNextEvent(true, &processedEvent);
      if (NS_SUCCEEDED(rv) && !processedEvent) rv = NS_ERROR_UNEXPECTED;
||||||| merged common ancestors
nsresult
nsSyncLoader::PushAsyncStream(nsIStreamListener* aListener)
{
    mListener = aListener;

    mAsyncLoadStatus = NS_OK;

    // Start reading from the channel
    nsresult rv = mChannel->AsyncOpen2(this);

    if (NS_SUCCEEDED(rv)) {
        // process events until we're finished.
        mLoading = true;
        nsIThread *thread = NS_GetCurrentThread();
        while (mLoading && NS_SUCCEEDED(rv)) {
            bool processedEvent;
            rv = thread->ProcessNextEvent(true, &processedEvent);
            if (NS_SUCCEEDED(rv) && !processedEvent)
                rv = NS_ERROR_UNEXPECTED;
        }
=======
  // Start reading from the channel
  nsresult rv = mChannel->AsyncOpen(this);

  if (NS_SUCCEEDED(rv)) {
    // process events until we're finished.
    mLoading = true;
    nsIThread* thread = NS_GetCurrentThread();
    while (mLoading && NS_SUCCEEDED(rv)) {
      bool processedEvent;
      rv = thread->ProcessNextEvent(true, &processedEvent);
      if (NS_SUCCEEDED(rv) && !processedEvent) rv = NS_ERROR_UNEXPECTED;
>>>>>>> upstream-releases
    }
  }

  mListener = nullptr;

  NS_ENSURE_SUCCESS(rv, rv);

  // Note that if AsyncOpen failed that's ok -- the only caller of
  // this method nulls out mChannel immediately after we return.

  return mAsyncLoadStatus;
}

<<<<<<< HEAD
nsresult nsSyncLoader::PushSyncStream(nsIStreamListener *aListener) {
  nsCOMPtr<nsIInputStream> in;
  nsresult rv = mChannel->Open2(getter_AddRefs(in));
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
nsresult
nsSyncLoader::PushSyncStream(nsIStreamListener* aListener)
{
    nsCOMPtr<nsIInputStream> in;
    nsresult rv = mChannel->Open2(getter_AddRefs(in));
    NS_ENSURE_SUCCESS(rv, rv);
=======
nsresult nsSyncLoader::PushSyncStream(nsIStreamListener* aListener) {
  nsCOMPtr<nsIInputStream> in;
  nsresult rv = mChannel->Open(getter_AddRefs(in));
  NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

  mLoading = true;
  rv = nsSyncLoadService::PushSyncStreamToListener(in.forget(), aListener,
                                                   mChannel);
  mLoading = false;

  return rv;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsSyncLoader::OnStartRequest(nsIRequest *aRequest, nsISupports *aContext) {
  return mListener->OnStartRequest(aRequest, aContext);
||||||| merged common ancestors
nsSyncLoader::OnStartRequest(nsIRequest *aRequest, nsISupports *aContext)
{
    return mListener->OnStartRequest(aRequest, aContext);
=======
nsSyncLoader::OnStartRequest(nsIRequest* aRequest) {
  return mListener->OnStartRequest(aRequest);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsSyncLoader::OnStopRequest(nsIRequest *aRequest, nsISupports *aContext,
                            nsresult aStatusCode) {
  if (NS_SUCCEEDED(mAsyncLoadStatus) && NS_FAILED(aStatusCode)) {
    mAsyncLoadStatus = aStatusCode;
  }
  nsresult rv = mListener->OnStopRequest(aRequest, aContext, aStatusCode);
  if (NS_SUCCEEDED(mAsyncLoadStatus) && NS_FAILED(rv)) {
    mAsyncLoadStatus = rv;
  }
  mLoading = false;

  return rv;
||||||| merged common ancestors
nsSyncLoader::OnStopRequest(nsIRequest *aRequest, nsISupports *aContext,
                            nsresult aStatusCode)
{
    if (NS_SUCCEEDED(mAsyncLoadStatus) && NS_FAILED(aStatusCode)) {
        mAsyncLoadStatus = aStatusCode;
    }
    nsresult rv = mListener->OnStopRequest(aRequest, aContext, aStatusCode);
    if (NS_SUCCEEDED(mAsyncLoadStatus) && NS_FAILED(rv)) {
        mAsyncLoadStatus = rv;
    }
    mLoading = false;

    return rv;
=======
nsSyncLoader::OnStopRequest(nsIRequest* aRequest, nsresult aStatusCode) {
  if (NS_SUCCEEDED(mAsyncLoadStatus) && NS_FAILED(aStatusCode)) {
    mAsyncLoadStatus = aStatusCode;
  }
  nsresult rv = mListener->OnStopRequest(aRequest, aStatusCode);
  if (NS_SUCCEEDED(mAsyncLoadStatus) && NS_FAILED(rv)) {
    mAsyncLoadStatus = rv;
  }
  mLoading = false;

  return rv;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsSyncLoader::AsyncOnChannelRedirect(nsIChannel *aOldChannel,
                                     nsIChannel *aNewChannel, uint32_t aFlags,
                                     nsIAsyncVerifyRedirectCallback *callback) {
  MOZ_ASSERT(aNewChannel, "Redirecting to null channel?");
||||||| merged common ancestors
nsSyncLoader::AsyncOnChannelRedirect(nsIChannel *aOldChannel,
                                     nsIChannel *aNewChannel,
                                     uint32_t aFlags,
                                     nsIAsyncVerifyRedirectCallback *callback)
{
    MOZ_ASSERT(aNewChannel, "Redirecting to null channel?");
=======
nsSyncLoader::AsyncOnChannelRedirect(nsIChannel* aOldChannel,
                                     nsIChannel* aNewChannel, uint32_t aFlags,
                                     nsIAsyncVerifyRedirectCallback* callback) {
  MOZ_ASSERT(aNewChannel, "Redirecting to null channel?");
>>>>>>> upstream-releases

  mChannel = aNewChannel;

  callback->OnRedirectVerifyCallback(NS_OK);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsSyncLoader::GetInterface(const nsIID &aIID, void **aResult) {
  return QueryInterface(aIID, aResult);
||||||| merged common ancestors
nsSyncLoader::GetInterface(const nsIID & aIID,
                           void **aResult)
{
    return QueryInterface(aIID, aResult);
=======
nsSyncLoader::GetInterface(const nsIID& aIID, void** aResult) {
  return QueryInterface(aIID, aResult);
>>>>>>> upstream-releases
}

/* static */
<<<<<<< HEAD
nsresult nsSyncLoadService::LoadDocument(
    nsIURI *aURI, nsContentPolicyType aContentPolicyType,
    nsIPrincipal *aLoaderPrincipal, nsSecurityFlags aSecurityFlags,
    nsILoadGroup *aLoadGroup, bool aForceToXML, ReferrerPolicy aReferrerPolicy,
    nsIDocument **aResult) {
  nsCOMPtr<nsIChannel> channel;
  nsresult rv = NS_NewChannel(getter_AddRefs(channel), aURI, aLoaderPrincipal,
                              aSecurityFlags, aContentPolicyType,
                              nullptr,  // PerformanceStorage
                              aLoadGroup);
  NS_ENSURE_SUCCESS(rv, rv);

  if (!aForceToXML) {
    channel->SetContentType(NS_LITERAL_CSTRING("text/xml"));
  }

  bool isChrome = false, isResource = false;
  // if the load needs to enforce CORS, then force the load to be async
  bool isSync =
||||||| merged common ancestors
nsresult
nsSyncLoadService::LoadDocument(nsIURI *aURI,
                                nsContentPolicyType aContentPolicyType,
                                nsIPrincipal *aLoaderPrincipal,
                                nsSecurityFlags aSecurityFlags,
                                nsILoadGroup *aLoadGroup,
                                bool aForceToXML,
                                ReferrerPolicy aReferrerPolicy,
                                nsIDocument** aResult)
{
    nsCOMPtr<nsIChannel> channel;
    nsresult rv = NS_NewChannel(getter_AddRefs(channel),
                                aURI,
                                aLoaderPrincipal,
                                aSecurityFlags,
                                aContentPolicyType,
                                nullptr, // PerformanceStorage
                                aLoadGroup);
    NS_ENSURE_SUCCESS(rv, rv);

    if (!aForceToXML) {
        channel->SetContentType(NS_LITERAL_CSTRING("text/xml"));
    }

    bool isChrome = false, isResource = false;
    // if the load needs to enforce CORS, then force the load to be async
    bool isSync =
=======
nsresult nsSyncLoadService::LoadDocument(
    nsIURI* aURI, nsContentPolicyType aContentPolicyType,
    nsIPrincipal* aLoaderPrincipal, nsSecurityFlags aSecurityFlags,
    nsILoadGroup* aLoadGroup, nsICookieSettings* aCookieSettings,
    bool aForceToXML, ReferrerPolicy aReferrerPolicy, Document** aResult) {
  nsCOMPtr<nsIChannel> channel;
  nsresult rv =
      NS_NewChannel(getter_AddRefs(channel), aURI, aLoaderPrincipal,
                    aSecurityFlags, aContentPolicyType, aCookieSettings,
                    nullptr,  // PerformanceStorage
                    aLoadGroup);
  NS_ENSURE_SUCCESS(rv, rv);

  if (!aForceToXML) {
    channel->SetContentType(NS_LITERAL_CSTRING("text/xml"));
  }

  bool isChrome = false, isResource = false;
  // if the load needs to enforce CORS, then force the load to be async
  bool isSync =
>>>>>>> upstream-releases
      !(aSecurityFlags & nsILoadInfo::SEC_REQUIRE_CORS_DATA_INHERITS) &&
      ((NS_SUCCEEDED(aURI->SchemeIs("chrome", &isChrome)) && isChrome) ||
       (NS_SUCCEEDED(aURI->SchemeIs("resource", &isResource)) && isResource));
  RefPtr<nsSyncLoader> loader = new nsSyncLoader();
  return loader->LoadDocument(channel, isSync, aForceToXML, aReferrerPolicy,
                              aResult);
}

/* static */
<<<<<<< HEAD
nsresult nsSyncLoadService::PushSyncStreamToListener(
    already_AddRefed<nsIInputStream> aIn, nsIStreamListener *aListener,
    nsIChannel *aChannel) {
  nsCOMPtr<nsIInputStream> in = std::move(aIn);

  // Set up buffering stream
  nsresult rv;
  nsCOMPtr<nsIInputStream> bufferedStream;
  if (!NS_InputStreamIsBuffered(in)) {
    int64_t chunkSize;
    rv = aChannel->GetContentLength(&chunkSize);
    if (NS_FAILED(rv) || chunkSize < 1) {
      chunkSize = 4096;
||||||| merged common ancestors
nsresult
nsSyncLoadService::PushSyncStreamToListener(already_AddRefed<nsIInputStream> aIn,
                                            nsIStreamListener* aListener,
                                            nsIChannel* aChannel)
{
    nsCOMPtr<nsIInputStream> in = std::move(aIn);

    // Set up buffering stream
    nsresult rv;
    nsCOMPtr<nsIInputStream> bufferedStream;
    if (!NS_InputStreamIsBuffered(in)) {
        int64_t chunkSize;
        rv = aChannel->GetContentLength(&chunkSize);
        if (NS_FAILED(rv) || chunkSize < 1) {
            chunkSize = 4096;
        }
        chunkSize = std::min(int64_t(UINT16_MAX), chunkSize);

        rv = NS_NewBufferedInputStream(getter_AddRefs(bufferedStream),
                                       in.forget(), chunkSize);
        NS_ENSURE_SUCCESS(rv, rv);

        in = bufferedStream;
=======
nsresult nsSyncLoadService::PushSyncStreamToListener(
    already_AddRefed<nsIInputStream> aIn, nsIStreamListener* aListener,
    nsIChannel* aChannel) {
  nsCOMPtr<nsIInputStream> in = std::move(aIn);

  // Set up buffering stream
  nsresult rv;
  nsCOMPtr<nsIInputStream> bufferedStream;
  if (!NS_InputStreamIsBuffered(in)) {
    int64_t chunkSize;
    rv = aChannel->GetContentLength(&chunkSize);
    if (NS_FAILED(rv) || chunkSize < 1) {
      chunkSize = 4096;
>>>>>>> upstream-releases
    }
    chunkSize = std::min(int64_t(UINT16_MAX), chunkSize);

<<<<<<< HEAD
    rv = NS_NewBufferedInputStream(getter_AddRefs(bufferedStream), in.forget(),
                                   chunkSize);
    NS_ENSURE_SUCCESS(rv, rv);

    in = bufferedStream;
  }

  // Load
  rv = aListener->OnStartRequest(aChannel, nullptr);
  if (NS_SUCCEEDED(rv)) {
    uint64_t sourceOffset = 0;
    while (1) {
      uint64_t readCount = 0;
      rv = in->Available(&readCount);
      if (NS_FAILED(rv) || !readCount) {
        if (rv == NS_BASE_STREAM_CLOSED) {
          // End of file, but not an error
          rv = NS_OK;
||||||| merged common ancestors
    // Load
    rv = aListener->OnStartRequest(aChannel, nullptr);
    if (NS_SUCCEEDED(rv)) {
        uint64_t sourceOffset = 0;
        while (1) {
            uint64_t readCount = 0;
            rv = in->Available(&readCount);
            if (NS_FAILED(rv) || !readCount) {
                if (rv == NS_BASE_STREAM_CLOSED) {
                    // End of file, but not an error
                    rv = NS_OK;
                }
                break;
            }

            if (readCount > UINT32_MAX)
                readCount = UINT32_MAX;

            rv = aListener->OnDataAvailable(aChannel, nullptr, in,
                                            (uint32_t)std::min(sourceOffset, (uint64_t)UINT32_MAX),
                                            (uint32_t)readCount);
            if (NS_FAILED(rv)) {
                break;
            }
            sourceOffset += readCount;
=======
    rv = NS_NewBufferedInputStream(getter_AddRefs(bufferedStream), in.forget(),
                                   chunkSize);
    NS_ENSURE_SUCCESS(rv, rv);

    in = bufferedStream;
  }

  // Load
  rv = aListener->OnStartRequest(aChannel);
  if (NS_SUCCEEDED(rv)) {
    uint64_t sourceOffset = 0;
    while (1) {
      uint64_t readCount = 0;
      rv = in->Available(&readCount);
      if (NS_FAILED(rv) || !readCount) {
        if (rv == NS_BASE_STREAM_CLOSED) {
          // End of file, but not an error
          rv = NS_OK;
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        break;
      }

      if (readCount > UINT32_MAX) readCount = UINT32_MAX;

      rv = aListener->OnDataAvailable(
          aChannel, nullptr, in,
          (uint32_t)std::min(sourceOffset, (uint64_t)UINT32_MAX),
          (uint32_t)readCount);
      if (NS_FAILED(rv)) {
        break;
      }
      sourceOffset += readCount;
||||||| merged common ancestors
=======
        break;
      }

      if (readCount > UINT32_MAX) readCount = UINT32_MAX;

      rv = aListener->OnDataAvailable(
          aChannel, in, (uint32_t)std::min(sourceOffset, (uint64_t)UINT32_MAX),
          (uint32_t)readCount);
      if (NS_FAILED(rv)) {
        break;
      }
      sourceOffset += readCount;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
  if (NS_FAILED(rv)) {
    aChannel->Cancel(rv);
  }
  aListener->OnStopRequest(aChannel, nullptr, rv);
||||||| merged common ancestors
    if (NS_FAILED(rv)) {
        aChannel->Cancel(rv);
    }
    aListener->OnStopRequest(aChannel, nullptr, rv);
=======
  }
  if (NS_FAILED(rv)) {
    aChannel->Cancel(rv);
  }
  aListener->OnStopRequest(aChannel, rv);
>>>>>>> upstream-releases

  return rv;
}
