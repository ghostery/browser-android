/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim:set ts=2 sts=2 sw=2 et cin:
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/ContentChild.h"
#include "nsIURI.h"
#include "nsIURL.h"
#include "nsExternalProtocolHandler.h"
#include "nsString.h"
#include "nsReadableUtils.h"
#include "nsCOMPtr.h"
#include "nsContentUtils.h"
#include "nsIServiceManager.h"
#include "nsServiceManagerUtils.h"
#include "nsIInterfaceRequestor.h"
#include "nsIInterfaceRequestorUtils.h"
#include "nsIStringBundle.h"
#include "nsIPrefService.h"
#include "nsIPrompt.h"
#include "nsIURIMutator.h"
#include "nsNetUtil.h"
#include "nsContentSecurityManager.h"
#include "nsExternalHelperAppService.h"

// used to dispatch urls to default protocol handlers
#include "nsCExternalHandlerService.h"
#include "nsIExternalProtocolService.h"
#include "nsIChildChannel.h"
#include "nsIParentChannel.h"

class nsILoadInfo;

////////////////////////////////////////////////////////////////////////
// a stub channel implemenation which will map calls to AsyncRead and
// OpenInputStream to calls in the OS for loading the url.
////////////////////////////////////////////////////////////////////////

class nsExtProtocolChannel : public nsIChannel,
                             public nsIChildChannel,
<<<<<<< HEAD
                             public nsIParentChannel {
 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSICHANNEL
  NS_DECL_NSIREQUESTOBSERVER
  NS_DECL_NSISTREAMLISTENER
  NS_DECL_NSIREQUEST
  NS_DECL_NSICHILDCHANNEL
  NS_DECL_NSIPARENTCHANNEL

  nsExtProtocolChannel(nsIURI *aURI, nsILoadInfo *aLoadInfo);

 private:
  virtual ~nsExtProtocolChannel();

  nsresult OpenURL();
  void Finish(nsresult aResult);

  nsCOMPtr<nsIURI> mUrl;
  nsCOMPtr<nsIURI> mOriginalURI;
  nsresult mStatus;
  nsLoadFlags mLoadFlags;
  bool mWasOpened;
  // Set true (as a result of ConnectParent invoked from child process)
  // when this channel is on the parent process and is being used as
  // a redirect target channel.  It turns AsyncOpen into a no-op since
  // we do it on the child.
  bool mConnectedParent;

  nsCOMPtr<nsIInterfaceRequestor> mCallbacks;
  nsCOMPtr<nsILoadGroup> mLoadGroup;
  nsCOMPtr<nsILoadInfo> mLoadInfo;
  nsCOMPtr<nsIStreamListener> mListener;
||||||| merged common ancestors
                             public nsIParentChannel
{
public:
    NS_DECL_THREADSAFE_ISUPPORTS
    NS_DECL_NSICHANNEL
    NS_DECL_NSIREQUESTOBSERVER
    NS_DECL_NSISTREAMLISTENER
    NS_DECL_NSIREQUEST
    NS_DECL_NSICHILDCHANNEL
    NS_DECL_NSIPARENTCHANNEL

    nsExtProtocolChannel(nsIURI* aURI, nsILoadInfo* aLoadInfo);

private:
    virtual ~nsExtProtocolChannel();

    nsresult OpenURL();
    void Finish(nsresult aResult);
    
    nsCOMPtr<nsIURI> mUrl;
    nsCOMPtr<nsIURI> mOriginalURI;
    nsresult mStatus;
    nsLoadFlags mLoadFlags;
    bool mWasOpened;
    // Set true (as a result of ConnectParent invoked from child process)
    // when this channel is on the parent process and is being used as
    // a redirect target channel.  It turns AsyncOpen into a no-op since
    // we do it on the child.
    bool mConnectedParent;
    
    nsCOMPtr<nsIInterfaceRequestor> mCallbacks;
    nsCOMPtr<nsILoadGroup> mLoadGroup;
    nsCOMPtr<nsILoadInfo> mLoadInfo;
    nsCOMPtr<nsIStreamListener> mListener;
=======
                             public nsIParentChannel {
 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSICHANNEL
  NS_DECL_NSIREQUESTOBSERVER
  NS_DECL_NSISTREAMLISTENER
  NS_DECL_NSIREQUEST
  NS_DECL_NSICHILDCHANNEL
  NS_DECL_NSIPARENTCHANNEL

  nsExtProtocolChannel(nsIURI* aURI, nsILoadInfo* aLoadInfo);

 private:
  virtual ~nsExtProtocolChannel();

  nsresult OpenURL();
  void Finish(nsresult aResult);

  nsCOMPtr<nsIURI> mUrl;
  nsCOMPtr<nsIURI> mOriginalURI;
  nsresult mStatus;
  nsLoadFlags mLoadFlags;
  bool mWasOpened;
  // Set true (as a result of ConnectParent invoked from child process)
  // when this channel is on the parent process and is being used as
  // a redirect target channel.  It turns AsyncOpen into a no-op since
  // we do it on the child.
  bool mConnectedParent;

  nsCOMPtr<nsIInterfaceRequestor> mCallbacks;
  nsCOMPtr<nsILoadGroup> mLoadGroup;
  nsCOMPtr<nsILoadInfo> mLoadInfo;
  nsCOMPtr<nsIStreamListener> mListener;
>>>>>>> upstream-releases
};

NS_IMPL_ADDREF(nsExtProtocolChannel)
NS_IMPL_RELEASE(nsExtProtocolChannel)

NS_INTERFACE_MAP_BEGIN(nsExtProtocolChannel)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIChannel)
  NS_INTERFACE_MAP_ENTRY(nsIChannel)
  NS_INTERFACE_MAP_ENTRY(nsIRequest)
  NS_INTERFACE_MAP_ENTRY(nsIChildChannel)
  NS_INTERFACE_MAP_ENTRY(nsIParentChannel)
  NS_INTERFACE_MAP_ENTRY(nsIStreamListener)
  NS_INTERFACE_MAP_ENTRY(nsIRequestObserver)
NS_INTERFACE_MAP_END

<<<<<<< HEAD
nsExtProtocolChannel::nsExtProtocolChannel(nsIURI *aURI, nsILoadInfo *aLoadInfo)
    : mUrl(aURI),
      mOriginalURI(aURI),
      mStatus(NS_OK),
      mLoadFlags(nsIRequest::LOAD_NORMAL),
      mWasOpened(false),
      mConnectedParent(false),
      mLoadInfo(aLoadInfo) {}
||||||| merged common ancestors
nsExtProtocolChannel::nsExtProtocolChannel(nsIURI* aURI,
                                           nsILoadInfo* aLoadInfo)
  : mUrl(aURI)
  , mOriginalURI(aURI)
  , mStatus(NS_OK)
  , mLoadFlags(nsIRequest::LOAD_NORMAL)
  , mWasOpened(false)
  , mConnectedParent(false)
  , mLoadInfo(aLoadInfo)
{
}
=======
nsExtProtocolChannel::nsExtProtocolChannel(nsIURI* aURI, nsILoadInfo* aLoadInfo)
    : mUrl(aURI),
      mOriginalURI(aURI),
      mStatus(NS_OK),
      mLoadFlags(nsIRequest::LOAD_NORMAL),
      mWasOpened(false),
      mConnectedParent(false),
      mLoadInfo(aLoadInfo) {}
>>>>>>> upstream-releases

nsExtProtocolChannel::~nsExtProtocolChannel() {}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::GetLoadGroup(nsILoadGroup **aLoadGroup) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::GetLoadGroup(nsILoadGroup * *aLoadGroup)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetLoadGroup(nsILoadGroup** aLoadGroup) {
>>>>>>> upstream-releases
  NS_IF_ADDREF(*aLoadGroup = mLoadGroup);
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::SetLoadGroup(nsILoadGroup *aLoadGroup) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::SetLoadGroup(nsILoadGroup * aLoadGroup)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::SetLoadGroup(nsILoadGroup* aLoadGroup) {
>>>>>>> upstream-releases
  mLoadGroup = aLoadGroup;
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::GetNotificationCallbacks(
    nsIInterfaceRequestor **aCallbacks) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::GetNotificationCallbacks(nsIInterfaceRequestor* *aCallbacks)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetNotificationCallbacks(
    nsIInterfaceRequestor** aCallbacks) {
>>>>>>> upstream-releases
  NS_IF_ADDREF(*aCallbacks = mCallbacks);
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::SetNotificationCallbacks(
    nsIInterfaceRequestor *aCallbacks) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::SetNotificationCallbacks(nsIInterfaceRequestor* aCallbacks)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::SetNotificationCallbacks(
    nsIInterfaceRequestor* aCallbacks) {
>>>>>>> upstream-releases
  mCallbacks = aCallbacks;
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP
nsExtProtocolChannel::GetSecurityInfo(nsISupports **aSecurityInfo) {
||||||| merged common ancestors
NS_IMETHODIMP 
nsExtProtocolChannel::GetSecurityInfo(nsISupports * *aSecurityInfo)
{
=======
NS_IMETHODIMP
nsExtProtocolChannel::GetSecurityInfo(nsISupports** aSecurityInfo) {
>>>>>>> upstream-releases
  *aSecurityInfo = nullptr;
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::GetOriginalURI(nsIURI **aURI) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::GetOriginalURI(nsIURI* *aURI)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetOriginalURI(nsIURI** aURI) {
>>>>>>> upstream-releases
  NS_ADDREF(*aURI = mOriginalURI);
  return NS_OK;
}
<<<<<<< HEAD

NS_IMETHODIMP nsExtProtocolChannel::SetOriginalURI(nsIURI *aURI) {
||||||| merged common ancestors
 
NS_IMETHODIMP nsExtProtocolChannel::SetOriginalURI(nsIURI* aURI)
{
=======

NS_IMETHODIMP nsExtProtocolChannel::SetOriginalURI(nsIURI* aURI) {
>>>>>>> upstream-releases
  NS_ENSURE_ARG_POINTER(aURI);
  mOriginalURI = aURI;
  return NS_OK;
}
<<<<<<< HEAD

NS_IMETHODIMP nsExtProtocolChannel::GetURI(nsIURI **aURI) {
||||||| merged common ancestors
 
NS_IMETHODIMP nsExtProtocolChannel::GetURI(nsIURI* *aURI)
{
=======

NS_IMETHODIMP nsExtProtocolChannel::GetURI(nsIURI** aURI) {
>>>>>>> upstream-releases
  *aURI = mUrl;
  NS_IF_ADDREF(*aURI);
  return NS_OK;
}

nsresult nsExtProtocolChannel::OpenURL() {
  nsresult rv = NS_ERROR_FAILURE;
  nsCOMPtr<nsIExternalProtocolService> extProtService(
      do_GetService(NS_EXTERNALPROTOCOLSERVICE_CONTRACTID));

  if (extProtService) {
#ifdef DEBUG
    nsAutoCString urlScheme;
    mUrl->GetScheme(urlScheme);
    bool haveHandler = false;
    extProtService->ExternalProtocolHandlerExists(urlScheme.get(),
                                                  &haveHandler);
    NS_ASSERTION(haveHandler,
                 "Why do we have a channel for this url if we don't support "
                 "the protocol?");
#endif

    nsCOMPtr<nsIInterfaceRequestor> aggCallbacks;
    rv = NS_NewNotificationCallbacksAggregation(mCallbacks, mLoadGroup,
                                                getter_AddRefs(aggCallbacks));
    if (NS_FAILED(rv)) {
      goto finish;
    }

    rv = extProtService->LoadURI(mUrl, aggCallbacks);

    if (NS_SUCCEEDED(rv) && mListener) {
      Cancel(NS_ERROR_NO_CONTENT);

      RefPtr<nsExtProtocolChannel> self = this;
      nsCOMPtr<nsIStreamListener> listener = mListener;
<<<<<<< HEAD
      MessageLoop::current()->PostTask(NS_NewRunnableFunction(
          "nsExtProtocolChannel::OpenURL", [self, listener]() {
            listener->OnStartRequest(self, nullptr);
            listener->OnStopRequest(self, nullptr, self->mStatus);
||||||| merged common ancestors
      MessageLoop::current()->PostTask(
        NS_NewRunnableFunction(
          "nsExtProtocolChannel::OpenURL",
          [self, listener]() {
            listener->OnStartRequest(self, nullptr);
            listener->OnStopRequest(self, nullptr, self->mStatus);
=======
      MessageLoop::current()->PostTask(NS_NewRunnableFunction(
          "nsExtProtocolChannel::OpenURL", [self, listener]() {
            listener->OnStartRequest(self);
            listener->OnStopRequest(self, self->mStatus);
>>>>>>> upstream-releases
          }));
    }
  }

finish:
  mCallbacks = nullptr;
  mListener = nullptr;
  return rv;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::Open(nsIInputStream **_retval) {
  return OpenURL();
}

NS_IMETHODIMP nsExtProtocolChannel::Open2(nsIInputStream **aStream) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::Open(nsIInputStream **_retval)
{
  return OpenURL();
}

NS_IMETHODIMP nsExtProtocolChannel::Open2(nsIInputStream** aStream)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::Open(nsIInputStream** aStream) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIStreamListener> listener;
  nsresult rv =
      nsContentSecurityManager::doContentSecurityCheck(this, listener);
  NS_ENSURE_SUCCESS(rv, rv);

  return OpenURL();
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::AsyncOpen(nsIStreamListener *listener,
                                              nsISupports *ctxt) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::AsyncOpen(nsIStreamListener *listener, nsISupports *ctxt)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::AsyncOpen(nsIStreamListener* aListener) {
  nsCOMPtr<nsIStreamListener> listener = aListener;
  nsresult rv =
      nsContentSecurityManager::doContentSecurityCheck(this, listener);
  if (NS_FAILED(rv)) {
    mCallbacks = nullptr;
    return rv;
  }

>>>>>>> upstream-releases
  if (mConnectedParent) {
    return NS_OK;
  }

<<<<<<< HEAD
  MOZ_ASSERT(
      !mLoadInfo || mLoadInfo->GetSecurityMode() == 0 ||
          mLoadInfo->GetInitialSecurityCheckDone() ||
          (mLoadInfo->GetSecurityMode() ==
               nsILoadInfo::SEC_ALLOW_CROSS_ORIGIN_DATA_IS_NULL &&
           nsContentUtils::IsSystemPrincipal(mLoadInfo->LoadingPrincipal())),
      "security flags in loadInfo but asyncOpen2() not called");
||||||| merged common ancestors
  MOZ_ASSERT(!mLoadInfo ||
             mLoadInfo->GetSecurityMode() == 0 ||
             mLoadInfo->GetInitialSecurityCheckDone() ||
             (mLoadInfo->GetSecurityMode() == nsILoadInfo::SEC_ALLOW_CROSS_ORIGIN_DATA_IS_NULL &&
              nsContentUtils::IsSystemPrincipal(mLoadInfo->LoadingPrincipal())),
             "security flags in loadInfo but asyncOpen2() not called");
=======
  MOZ_ASSERT(
      !mLoadInfo || mLoadInfo->GetSecurityMode() == 0 ||
          mLoadInfo->GetInitialSecurityCheckDone() ||
          (mLoadInfo->GetSecurityMode() ==
               nsILoadInfo::SEC_ALLOW_CROSS_ORIGIN_DATA_IS_NULL &&
           nsContentUtils::IsSystemPrincipal(mLoadInfo->LoadingPrincipal())),
      "security flags in loadInfo but doContentSecurityCheck() not called");
>>>>>>> upstream-releases

  NS_ENSURE_ARG_POINTER(listener);
  NS_ENSURE_TRUE(!mWasOpened, NS_ERROR_ALREADY_OPENED);

  mWasOpened = true;
  mListener = listener;

  return OpenURL();
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::AsyncOpen2(nsIStreamListener *aListener) {
  nsCOMPtr<nsIStreamListener> listener = aListener;
  nsresult rv =
      nsContentSecurityManager::doContentSecurityCheck(this, listener);
  if (NS_FAILED(rv)) {
    mCallbacks = nullptr;
    return rv;
  }
  return AsyncOpen(listener, nullptr);
}

NS_IMETHODIMP nsExtProtocolChannel::GetLoadFlags(nsLoadFlags *aLoadFlags) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::AsyncOpen2(nsIStreamListener *aListener)
{
  nsCOMPtr<nsIStreamListener> listener = aListener;
  nsresult rv = nsContentSecurityManager::doContentSecurityCheck(this, listener);
  if (NS_FAILED(rv)) {
    mCallbacks = nullptr;
    return rv;
  }
  return AsyncOpen(listener, nullptr);
}

NS_IMETHODIMP nsExtProtocolChannel::GetLoadFlags(nsLoadFlags *aLoadFlags)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetLoadFlags(nsLoadFlags* aLoadFlags) {
>>>>>>> upstream-releases
  *aLoadFlags = mLoadFlags;
  return NS_OK;
}

NS_IMETHODIMP nsExtProtocolChannel::SetLoadFlags(nsLoadFlags aLoadFlags) {
  mLoadFlags = aLoadFlags;
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::GetIsDocument(bool *aIsDocument) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::GetIsDocument(bool *aIsDocument)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetIsDocument(bool* aIsDocument) {
>>>>>>> upstream-releases
  return NS_GetIsDocumentChannel(this, aIsDocument);
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::GetContentType(nsACString &aContentType) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::GetContentType(nsACString &aContentType)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetContentType(nsACString& aContentType) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::SetContentType(
    const nsACString &aContentType) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::SetContentType(const nsACString &aContentType)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::SetContentType(
    const nsACString& aContentType) {
>>>>>>> upstream-releases
  return NS_ERROR_FAILURE;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::GetContentCharset(
    nsACString &aContentCharset) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::GetContentCharset(nsACString &aContentCharset)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetContentCharset(
    nsACString& aContentCharset) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::SetContentCharset(
    const nsACString &aContentCharset) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::SetContentCharset(const nsACString &aContentCharset)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::SetContentCharset(
    const nsACString& aContentCharset) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::GetContentDisposition(
    uint32_t *aContentDisposition) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::GetContentDisposition(uint32_t *aContentDisposition)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetContentDisposition(
    uint32_t* aContentDisposition) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_AVAILABLE;
}

NS_IMETHODIMP nsExtProtocolChannel::SetContentDisposition(
    uint32_t aContentDisposition) {
  return NS_ERROR_NOT_AVAILABLE;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::GetContentDispositionFilename(
    nsAString &aContentDispositionFilename) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::GetContentDispositionFilename(nsAString &aContentDispositionFilename)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetContentDispositionFilename(
    nsAString& aContentDispositionFilename) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_AVAILABLE;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::SetContentDispositionFilename(
    const nsAString &aContentDispositionFilename) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::SetContentDispositionFilename(const nsAString &aContentDispositionFilename)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::SetContentDispositionFilename(
    const nsAString& aContentDispositionFilename) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_AVAILABLE;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::GetContentDispositionHeader(
    nsACString &aContentDispositionHeader) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::GetContentDispositionHeader(nsACString &aContentDispositionHeader)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetContentDispositionHeader(
    nsACString& aContentDispositionHeader) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_AVAILABLE;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::GetContentLength(int64_t *aContentLength) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::GetContentLength(int64_t * aContentLength)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetContentLength(int64_t* aContentLength) {
>>>>>>> upstream-releases
  *aContentLength = -1;
  return NS_OK;
}

NS_IMETHODIMP
nsExtProtocolChannel::SetContentLength(int64_t aContentLength) {
  MOZ_ASSERT_UNREACHABLE("SetContentLength");
  return NS_ERROR_NOT_IMPLEMENTED;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::GetOwner(nsISupports **aPrincipal) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::GetOwner(nsISupports * *aPrincipal)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetOwner(nsISupports** aPrincipal) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::SetOwner(nsISupports *aPrincipal) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::SetOwner(nsISupports * aPrincipal)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::SetOwner(nsISupports* aPrincipal) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::GetLoadInfo(nsILoadInfo **aLoadInfo) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::GetLoadInfo(nsILoadInfo **aLoadInfo)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetLoadInfo(nsILoadInfo** aLoadInfo) {
>>>>>>> upstream-releases
  NS_IF_ADDREF(*aLoadInfo = mLoadInfo);
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::SetLoadInfo(nsILoadInfo *aLoadInfo) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::SetLoadInfo(nsILoadInfo *aLoadInfo)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::SetLoadInfo(nsILoadInfo* aLoadInfo) {
  MOZ_RELEASE_ASSERT(aLoadInfo, "loadinfo can't be null");
>>>>>>> upstream-releases
  mLoadInfo = aLoadInfo;
  return NS_OK;
}

////////////////////////////////////////////////////////////////////////////////
// From nsIRequest
////////////////////////////////////////////////////////////////////////////////

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::GetName(nsACString &result) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::GetName(nsACString &result)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetName(nsACString& result) {
>>>>>>> upstream-releases
  return mUrl->GetSpec(result);
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::IsPending(bool *result) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::IsPending(bool *result)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::IsPending(bool* result) {
>>>>>>> upstream-releases
  *result = false;
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::GetStatus(nsresult *status) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::GetStatus(nsresult *status)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::GetStatus(nsresult* status) {
>>>>>>> upstream-releases
  *status = mStatus;
  return NS_OK;
}

NS_IMETHODIMP nsExtProtocolChannel::Cancel(nsresult status) {
  mStatus = status;
  return NS_OK;
}

NS_IMETHODIMP nsExtProtocolChannel::Suspend() {
  MOZ_ASSERT_UNREACHABLE("Suspend");
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP nsExtProtocolChannel::Resume() {
  MOZ_ASSERT_UNREACHABLE("Resume");
  return NS_ERROR_NOT_IMPLEMENTED;
}

///////////////////////////////////////////////////////////////////////
// From nsIChildChannel
//////////////////////////////////////////////////////////////////////

NS_IMETHODIMP nsExtProtocolChannel::ConnectParent(uint32_t registrarId) {
  mozilla::dom::ContentChild::GetSingleton()
      ->SendExtProtocolChannelConnectParent(registrarId);
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::CompleteRedirectSetup(
    nsIStreamListener *listener, nsISupports *context) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::CompleteRedirectSetup(nsIStreamListener *listener,
                                                          nsISupports *context)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::CompleteRedirectSetup(
    nsIStreamListener* listener, nsISupports* context) {
>>>>>>> upstream-releases
  // For redirects to external protocols we AsyncOpen on the child
  // (not the parent) because child channel has the right docshell
  // (which is needed for the select dialog).
  return AsyncOpen(listener);
}

///////////////////////////////////////////////////////////////////////
// From nsIParentChannel (derives from nsIStreamListener)
//////////////////////////////////////////////////////////////////////

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::SetParentListener(
    mozilla::net::HttpChannelParentListener *aListener) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::SetParentListener(mozilla::net::HttpChannelParentListener* aListener)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::SetParentListener(
    mozilla::net::HttpChannelParentListener* aListener) {
>>>>>>> upstream-releases
  // This is called as part of the connect parent operation from
  // ContentParent::RecvExtProtocolChannelConnectParent.  Setting
  // this flag tells this channel to not proceed and makes AsyncOpen
  // just no-op.  Actual operation will happen from the child process
  // via CompleteRedirectSetup call on the child channel.
  mConnectedParent = true;
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::NotifyTrackingProtectionDisabled() {
  // nothing to do
  return NS_OK;
}

NS_IMETHODIMP nsExtProtocolChannel::NotifyCookieAllowed() {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::NotifyTrackingProtectionDisabled()
{
=======
NS_IMETHODIMP nsExtProtocolChannel::NotifyChannelClassifierProtectionDisabled(
    uint32_t aAcceptedReason) {
  // nothing to do
  return NS_OK;
}

NS_IMETHODIMP nsExtProtocolChannel::NotifyCookieAllowed() {
  // nothing to do
  return NS_OK;
}

NS_IMETHODIMP nsExtProtocolChannel::NotifyCookieBlocked(
    uint32_t aRejectedReason) {
  // nothing to do
  return NS_OK;
}

NS_IMETHODIMP nsExtProtocolChannel::SetClassifierMatchedInfo(
    const nsACString& aList, const nsACString& aProvider,
    const nsACString& aFullHash) {
>>>>>>> upstream-releases
  // nothing to do
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::NotifyTrackingCookieBlocked(
    uint32_t aRejectedReason) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::NotifyTrackingCookieBlocked(uint32_t aRejectedReason)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::SetClassifierMatchedTrackingInfo(
    const nsACString& aLists, const nsACString& aFullHashes) {
>>>>>>> upstream-releases
  // nothing to do
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::SetClassifierMatchedInfo(
    const nsACString &aList, const nsACString &aProvider,
    const nsACString &aFullHash) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::SetClassifierMatchedInfo(const nsACString& aList,
                                                             const nsACString& aProvider,
                                                             const nsACString& aFullHash)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::NotifyClassificationFlags(
    uint32_t aClassificationFlags, bool aIsThirdParty) {
>>>>>>> upstream-releases
  // nothing to do
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::NotifyTrackingResource(bool aIsThirdParty) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::NotifyTrackingResource(bool aIsThirdParty)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::NotifyFlashPluginStateChanged(
    nsIHttpChannel::FlashPluginState aState) {
>>>>>>> upstream-releases
  // nothing to do
  return NS_OK;
}

NS_IMETHODIMP nsExtProtocolChannel::Delete() {
  // nothing to do
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::OnStartRequest(nsIRequest *aRequest,
                                                   nsISupports *aContext) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::OnStartRequest(nsIRequest *aRequest,
                                                   nsISupports *aContext)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::OnStartRequest(nsIRequest* aRequest) {
>>>>>>> upstream-releases
  // no data is expected
  MOZ_CRASH("No data expected from external protocol channel");
  return NS_ERROR_UNEXPECTED;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::OnStopRequest(nsIRequest *aRequest,
                                                  nsISupports *aContext,
                                                  nsresult aStatusCode) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::OnStopRequest(nsIRequest *aRequest,
                                                  nsISupports *aContext,
                                                  nsresult aStatusCode)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::OnStopRequest(nsIRequest* aRequest,
                                                  nsresult aStatusCode) {
>>>>>>> upstream-releases
  // no data is expected
  MOZ_CRASH("No data expected from external protocol channel");
  return NS_ERROR_UNEXPECTED;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExtProtocolChannel::OnDataAvailable(
    nsIRequest *aRequest, nsISupports *aContext, nsIInputStream *aInputStream,
    uint64_t aOffset, uint32_t aCount) {
||||||| merged common ancestors
NS_IMETHODIMP nsExtProtocolChannel::OnDataAvailable(nsIRequest *aRequest,
                                                    nsISupports *aContext,
                                                    nsIInputStream *aInputStream,
                                                    uint64_t aOffset,
                                                    uint32_t aCount)
{
=======
NS_IMETHODIMP nsExtProtocolChannel::OnDataAvailable(
    nsIRequest* aRequest, nsIInputStream* aInputStream, uint64_t aOffset,
    uint32_t aCount) {
>>>>>>> upstream-releases
  // no data is expected
  MOZ_CRASH("No data expected from external protocol channel");
  return NS_ERROR_UNEXPECTED;
}

///////////////////////////////////////////////////////////////////////
// the default protocol handler implementation
//////////////////////////////////////////////////////////////////////

nsExternalProtocolHandler::nsExternalProtocolHandler() {
  m_schemeName = "default";
}

nsExternalProtocolHandler::~nsExternalProtocolHandler() {}

NS_IMPL_ADDREF(nsExternalProtocolHandler)
NS_IMPL_RELEASE(nsExternalProtocolHandler)

NS_INTERFACE_MAP_BEGIN(nsExternalProtocolHandler)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIProtocolHandler)
  NS_INTERFACE_MAP_ENTRY(nsIProtocolHandler)
  NS_INTERFACE_MAP_ENTRY(nsIExternalProtocolHandler)
  NS_INTERFACE_MAP_ENTRY(nsISupportsWeakReference)
NS_INTERFACE_MAP_END

<<<<<<< HEAD
NS_IMETHODIMP nsExternalProtocolHandler::GetScheme(nsACString &aScheme) {
||||||| merged common ancestors
NS_IMETHODIMP nsExternalProtocolHandler::GetScheme(nsACString &aScheme)
{
=======
NS_IMETHODIMP nsExternalProtocolHandler::GetScheme(nsACString& aScheme) {
>>>>>>> upstream-releases
  aScheme = m_schemeName;
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExternalProtocolHandler::GetDefaultPort(int32_t *aDefaultPort) {
||||||| merged common ancestors
NS_IMETHODIMP nsExternalProtocolHandler::GetDefaultPort(int32_t *aDefaultPort)
{
=======
NS_IMETHODIMP nsExternalProtocolHandler::GetDefaultPort(int32_t* aDefaultPort) {
>>>>>>> upstream-releases
  *aDefaultPort = 0;
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP
nsExternalProtocolHandler::AllowPort(int32_t port, const char *scheme,
                                     bool *_retval) {
  // don't override anything.
  *_retval = false;
  return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP 
nsExternalProtocolHandler::AllowPort(int32_t port, const char *scheme, bool *_retval)
{
    // don't override anything.  
    *_retval = false;
    return NS_OK;
=======
NS_IMETHODIMP
nsExternalProtocolHandler::AllowPort(int32_t port, const char* scheme,
                                     bool* _retval) {
  // don't override anything.
  *_retval = false;
  return NS_OK;
>>>>>>> upstream-releases
}
// returns TRUE if the OS can handle this protocol scheme and false otherwise.
<<<<<<< HEAD
bool nsExternalProtocolHandler::HaveExternalProtocolHandler(nsIURI *aURI) {
||||||| merged common ancestors
bool nsExternalProtocolHandler::HaveExternalProtocolHandler(nsIURI * aURI)
{
=======
bool nsExternalProtocolHandler::HaveExternalProtocolHandler(nsIURI* aURI) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aURI);
  nsAutoCString scheme;
  aURI->GetScheme(scheme);

  nsCOMPtr<nsIExternalProtocolService> extProtSvc(
      do_GetService(NS_EXTERNALPROTOCOLSERVICE_CONTRACTID));
  if (!extProtSvc) {
    return false;
  }

  bool haveHandler = false;
  extProtSvc->ExternalProtocolHandlerExists(scheme.get(), &haveHandler);
  return haveHandler;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExternalProtocolHandler::GetProtocolFlags(uint32_t *aUritype) {
  // Make it norelative since it is a simple uri
  *aUritype = URI_NORELATIVE | URI_NOAUTH | URI_LOADABLE_BY_ANYONE |
              URI_NON_PERSISTABLE | URI_DOES_NOT_RETURN_DATA;
  return NS_OK;
}

NS_IMETHODIMP nsExternalProtocolHandler::NewURI(
    const nsACString &aSpec,
    const char *aCharset,  // ignore charset info
    nsIURI *aBaseURI, nsIURI **_retval) {
  return NS_MutateURI(NS_SIMPLEURIMUTATOR_CONTRACTID)
      .SetSpec(aSpec)
      .Finalize(_retval);
||||||| merged common ancestors
NS_IMETHODIMP nsExternalProtocolHandler::GetProtocolFlags(uint32_t *aUritype)
{
    // Make it norelative since it is a simple uri
    *aUritype = URI_NORELATIVE | URI_NOAUTH | URI_LOADABLE_BY_ANYONE |
        URI_NON_PERSISTABLE | URI_DOES_NOT_RETURN_DATA;
    return NS_OK;
}

NS_IMETHODIMP nsExternalProtocolHandler::NewURI(const nsACString &aSpec,
                                                const char *aCharset, // ignore charset info
                                                nsIURI *aBaseURI,
                                                nsIURI **_retval)
{
  return NS_MutateURI(NS_SIMPLEURIMUTATOR_CONTRACTID)
           .SetSpec(aSpec)
           .Finalize(_retval);
=======
NS_IMETHODIMP nsExternalProtocolHandler::GetProtocolFlags(uint32_t* aUritype) {
  // Make it norelative since it is a simple uri
  *aUritype = URI_NORELATIVE | URI_NOAUTH | URI_LOADABLE_BY_ANYONE |
              URI_NON_PERSISTABLE | URI_DOES_NOT_RETURN_DATA;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsExternalProtocolHandler::NewChannel2(nsIURI *aURI, nsILoadInfo *aLoadInfo,
                                       nsIChannel **aRetval) {
||||||| merged common ancestors
nsExternalProtocolHandler::NewChannel2(nsIURI* aURI,
                                       nsILoadInfo* aLoadInfo,
                                       nsIChannel** aRetval)
{
=======
nsExternalProtocolHandler::NewChannel(nsIURI* aURI, nsILoadInfo* aLoadInfo,
                                      nsIChannel** aRetval) {
>>>>>>> upstream-releases
  NS_ENSURE_TRUE(aURI, NS_ERROR_UNKNOWN_PROTOCOL);
  NS_ENSURE_TRUE(aRetval, NS_ERROR_UNKNOWN_PROTOCOL);

  // Only try to return a channel if we have a protocol handler for the url.
  // nsOSHelperAppService::LoadUriInternal relies on this to check trustedness
  // for some platforms at least.  (win uses ::ShellExecute and unix uses
  // gnome_url_show.)
  if (!HaveExternalProtocolHandler(aURI)) {
    return NS_ERROR_UNKNOWN_PROTOCOL;
  }

  nsCOMPtr<nsIChannel> channel = new nsExtProtocolChannel(aURI, aLoadInfo);
  channel.forget(aRetval);
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsExternalProtocolHandler::NewChannel(nsIURI *aURI,
                                                    nsIChannel **_retval) {
  return NewChannel2(aURI, nullptr, _retval);
}

||||||| merged common ancestors
NS_IMETHODIMP nsExternalProtocolHandler::NewChannel(nsIURI *aURI, nsIChannel **_retval)
{
  return NewChannel2(aURI, nullptr, _retval);
}

=======
>>>>>>> upstream-releases
///////////////////////////////////////////////////////////////////////
// External protocol handler interface implementation
//////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
NS_IMETHODIMP nsExternalProtocolHandler::ExternalAppExistsForScheme(
    const nsACString &aScheme, bool *_retval) {
  nsCOMPtr<nsIExternalProtocolService> extProtSvc(
      do_GetService(NS_EXTERNALPROTOCOLSERVICE_CONTRACTID));
||||||| merged common ancestors
NS_IMETHODIMP nsExternalProtocolHandler::ExternalAppExistsForScheme(const nsACString& aScheme, bool *_retval)
{
  nsCOMPtr<nsIExternalProtocolService> extProtSvc(do_GetService(NS_EXTERNALPROTOCOLSERVICE_CONTRACTID));
=======
NS_IMETHODIMP nsExternalProtocolHandler::ExternalAppExistsForScheme(
    const nsACString& aScheme, bool* _retval) {
  nsCOMPtr<nsIExternalProtocolService> extProtSvc(
      do_GetService(NS_EXTERNALPROTOCOLSERVICE_CONTRACTID));
>>>>>>> upstream-releases
  if (extProtSvc)
    return extProtSvc->ExternalProtocolHandlerExists(
        PromiseFlatCString(aScheme).get(), _retval);

  // In case we don't have external protocol service.
  *_retval = false;
  return NS_OK;
}
