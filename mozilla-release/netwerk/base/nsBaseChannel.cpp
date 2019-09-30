/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 sts=2 ts=8 et tw=80 : */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsBaseChannel.h"
#include "nsContentUtils.h"
#include "nsURLHelper.h"
#include "nsNetCID.h"
#include "nsMimeTypes.h"
#include "nsUnknownDecoder.h"
#include "nsIScriptSecurityManager.h"
#include "nsMimeTypes.h"
#include "nsIHttpChannel.h"
#include "nsIChannelEventSink.h"
#include "nsIStreamConverterService.h"
#include "nsChannelClassifier.h"
#include "nsAsyncRedirectVerifyHelper.h"
#include "nsProxyRelease.h"
#include "nsXULAppAPI.h"
#include "nsContentSecurityManager.h"
#include "LoadInfo.h"
#include "nsServiceManagerUtils.h"
#include "nsRedirectHistoryEntry.h"

// This class is used to suspend a request across a function scope.
class ScopedRequestSuspender {
<<<<<<< HEAD
 public:
  explicit ScopedRequestSuspender(nsIRequest *request) : mRequest(request) {
||||||| merged common ancestors
public:
  explicit ScopedRequestSuspender(nsIRequest *request)
    : mRequest(request) {
=======
 public:
  explicit ScopedRequestSuspender(nsIRequest* request) : mRequest(request) {
>>>>>>> upstream-releases
    if (mRequest && NS_FAILED(mRequest->Suspend())) {
      NS_WARNING("Couldn't suspend pump");
      mRequest = nullptr;
    }
  }
  ~ScopedRequestSuspender() {
    if (mRequest) mRequest->Resume();
  }
<<<<<<< HEAD

 private:
  nsIRequest *mRequest;
||||||| merged common ancestors
private:
  nsIRequest *mRequest;
=======

 private:
  nsIRequest* mRequest;
>>>>>>> upstream-releases
};

// Used to suspend data events from mRequest within a function scope.  This is
// usually needed when a function makes callbacks that could process events.
#define SUSPEND_PUMP_FOR_SCOPE() \
  ScopedRequestSuspender pump_suspender__(mRequest)

//-----------------------------------------------------------------------------
// nsBaseChannel

nsBaseChannel::nsBaseChannel()
    : NeckoTargetHolder(nullptr),
      mPumpingData(false),
      mLoadFlags(LOAD_NORMAL),
      mQueriedProgressSink(true),
      mSynthProgressEvents(false),
      mAllowThreadRetargeting(true),
      mWaitingOnAsyncRedirect(false),
      mOpenRedirectChannel(false),
      mRedirectFlags{0},
      mStatus(NS_OK),
      mContentDispositionHint(UINT32_MAX),
      mContentLength(-1),
      mWasOpened(false) {
  mContentType.AssignLiteral(UNKNOWN_CONTENT_TYPE);
}

nsBaseChannel::~nsBaseChannel() {
  NS_ReleaseOnMainThreadSystemGroup("nsBaseChannel::mLoadInfo",
                                    mLoadInfo.forget());
}

<<<<<<< HEAD
nsresult nsBaseChannel::Redirect(nsIChannel *newChannel, uint32_t redirectFlags,
                                 bool openNewChannel) {
||||||| merged common ancestors
nsresult
nsBaseChannel::Redirect(nsIChannel *newChannel, uint32_t redirectFlags,
                        bool openNewChannel)
{
=======
nsresult nsBaseChannel::Redirect(nsIChannel* newChannel, uint32_t redirectFlags,
                                 bool openNewChannel) {
>>>>>>> upstream-releases
  SUSPEND_PUMP_FOR_SCOPE();

  // Transfer properties

  newChannel->SetLoadGroup(mLoadGroup);
  newChannel->SetNotificationCallbacks(mCallbacks);
  newChannel->SetLoadFlags(mLoadFlags | LOAD_REPLACE);

  // make a copy of the loadinfo, append to the redirectchain
  // and set it on the new channel
<<<<<<< HEAD
  if (mLoadInfo) {
    nsSecurityFlags secFlags = mLoadInfo->GetSecurityFlags() &
                               ~nsILoadInfo::SEC_FORCE_INHERIT_PRINCIPAL;
    nsCOMPtr<nsILoadInfo> newLoadInfo =
        static_cast<mozilla::net::LoadInfo *>(mLoadInfo.get())
            ->CloneWithNewSecFlags(secFlags);

    nsCOMPtr<nsIPrincipal> uriPrincipal;
    nsIScriptSecurityManager *sm = nsContentUtils::GetSecurityManager();
    sm->GetChannelURIPrincipal(this, getter_AddRefs(uriPrincipal));
    bool isInternalRedirect =
        (redirectFlags & (nsIChannelEventSink::REDIRECT_INTERNAL |
                          nsIChannelEventSink::REDIRECT_STS_UPGRADE));
||||||| merged common ancestors
  if (mLoadInfo) {
    nsSecurityFlags secFlags = mLoadInfo->GetSecurityFlags() &
                               ~nsILoadInfo::SEC_FORCE_INHERIT_PRINCIPAL;
    nsCOMPtr<nsILoadInfo> newLoadInfo =
      static_cast<mozilla::net::LoadInfo*>(mLoadInfo.get())->CloneWithNewSecFlags(secFlags);

    nsCOMPtr<nsIPrincipal> uriPrincipal;
    nsIScriptSecurityManager *sm = nsContentUtils::GetSecurityManager();
    sm->GetChannelURIPrincipal(this, getter_AddRefs(uriPrincipal));
    bool isInternalRedirect =
      (redirectFlags & (nsIChannelEventSink::REDIRECT_INTERNAL |
                        nsIChannelEventSink::REDIRECT_STS_UPGRADE));
=======
  nsSecurityFlags secFlags =
      mLoadInfo->GetSecurityFlags() & ~nsILoadInfo::SEC_FORCE_INHERIT_PRINCIPAL;
  nsCOMPtr<nsILoadInfo> newLoadInfo =
      static_cast<mozilla::net::LoadInfo*>(mLoadInfo.get())
          ->CloneWithNewSecFlags(secFlags);

  nsCOMPtr<nsIPrincipal> uriPrincipal;
  nsIScriptSecurityManager* sm = nsContentUtils::GetSecurityManager();
  sm->GetChannelURIPrincipal(this, getter_AddRefs(uriPrincipal));
  bool isInternalRedirect =
      (redirectFlags & (nsIChannelEventSink::REDIRECT_INTERNAL |
                        nsIChannelEventSink::REDIRECT_STS_UPGRADE));
>>>>>>> upstream-releases

<<<<<<< HEAD
    // nsBaseChannel hst no thing to do with HttpBaseChannel, we would not care
    // about referrer and remote address in this case
    nsCOMPtr<nsIRedirectHistoryEntry> entry =
        new nsRedirectHistoryEntry(uriPrincipal, nullptr, EmptyCString());
||||||| merged common ancestors
    // nsBaseChannel hst no thing to do with HttpBaseChannel, we would not care
    // about referrer and remote address in this case
    nsCOMPtr<nsIRedirectHistoryEntry> entry =
      new nsRedirectHistoryEntry(uriPrincipal, nullptr, EmptyCString());
=======
  // nsBaseChannel hst no thing to do with HttpBaseChannel, we would not care
  // about referrer and remote address in this case
  nsCOMPtr<nsIRedirectHistoryEntry> entry =
      new nsRedirectHistoryEntry(uriPrincipal, nullptr, EmptyCString());
>>>>>>> upstream-releases

<<<<<<< HEAD
    newLoadInfo->AppendRedirectHistoryEntry(entry, isInternalRedirect);

    // Ensure the channel's loadInfo's result principal URI so that it's
    // either non-null or updated to the redirect target URI.
    // We must do this because in case the loadInfo's result principal URI
    // is null, it would be taken from OriginalURI of the channel.  But we
    // overwrite it with the whole redirect chain first URI before opening
    // the target channel, hence the information would be lost.
    // If the protocol handler that created the channel wants to use
    // the originalURI of the channel as the principal URI, it has left
    // the result principal URI on the load info null.
    nsCOMPtr<nsIURI> resultPrincipalURI;

    nsCOMPtr<nsILoadInfo> existingLoadInfo = newChannel->GetLoadInfo();
    if (existingLoadInfo) {
      existingLoadInfo->GetResultPrincipalURI(
          getter_AddRefs(resultPrincipalURI));
    }
    if (!resultPrincipalURI) {
      newChannel->GetOriginalURI(getter_AddRefs(resultPrincipalURI));
    }
||||||| merged common ancestors
    newLoadInfo->AppendRedirectHistoryEntry(entry, isInternalRedirect);

    // Ensure the channel's loadInfo's result principal URI so that it's
    // either non-null or updated to the redirect target URI.
    // We must do this because in case the loadInfo's result principal URI
    // is null, it would be taken from OriginalURI of the channel.  But we
    // overwrite it with the whole redirect chain first URI before opening
    // the target channel, hence the information would be lost.
    // If the protocol handler that created the channel wants to use
    // the originalURI of the channel as the principal URI, it has left
    // the result principal URI on the load info null.
    nsCOMPtr<nsIURI> resultPrincipalURI;

    nsCOMPtr<nsILoadInfo> existingLoadInfo = newChannel->GetLoadInfo();
    if (existingLoadInfo) {
      existingLoadInfo->GetResultPrincipalURI(getter_AddRefs(resultPrincipalURI));
    }
    if (!resultPrincipalURI) {
      newChannel->GetOriginalURI(getter_AddRefs(resultPrincipalURI));
    }
=======
  newLoadInfo->AppendRedirectHistoryEntry(entry, isInternalRedirect);
>>>>>>> upstream-releases

  // Ensure the channel's loadInfo's result principal URI so that it's
  // either non-null or updated to the redirect target URI.
  // We must do this because in case the loadInfo's result principal URI
  // is null, it would be taken from OriginalURI of the channel.  But we
  // overwrite it with the whole redirect chain first URI before opening
  // the target channel, hence the information would be lost.
  // If the protocol handler that created the channel wants to use
  // the originalURI of the channel as the principal URI, it has left
  // the result principal URI on the load info null.
  nsCOMPtr<nsIURI> resultPrincipalURI;

<<<<<<< HEAD
    newChannel->SetLoadInfo(newLoadInfo);
  } else {
    // the newChannel was created with a dummy loadInfo, we should clear
    // it in case the original channel does not have a loadInfo
    newChannel->SetLoadInfo(nullptr);
||||||| merged common ancestors
    newChannel->SetLoadInfo(newLoadInfo);
  }
  else {
    // the newChannel was created with a dummy loadInfo, we should clear
    // it in case the original channel does not have a loadInfo
    newChannel->SetLoadInfo(nullptr);
=======
  nsCOMPtr<nsILoadInfo> existingLoadInfo = newChannel->LoadInfo();
  if (existingLoadInfo) {
    existingLoadInfo->GetResultPrincipalURI(getter_AddRefs(resultPrincipalURI));
  }
  if (!resultPrincipalURI) {
    newChannel->GetOriginalURI(getter_AddRefs(resultPrincipalURI));
>>>>>>> upstream-releases
  }

  newLoadInfo->SetResultPrincipalURI(resultPrincipalURI);

  newChannel->SetLoadInfo(newLoadInfo);

  // Preserve the privacy bit if it has been overridden
  if (mPrivateBrowsingOverriden) {
    nsCOMPtr<nsIPrivateBrowsingChannel> newPBChannel =
        do_QueryInterface(newChannel);
    if (newPBChannel) {
      newPBChannel->SetPrivate(mPrivateBrowsing);
    }
  }

  nsCOMPtr<nsIWritablePropertyBag> bag = ::do_QueryInterface(newChannel);
  if (bag) {
    for (auto iter = mPropertyHash.Iter(); !iter.Done(); iter.Next()) {
      bag->SetProperty(iter.Key(), iter.UserData());
    }
  }

  // Notify consumer, giving chance to cancel redirect.

  RefPtr<nsAsyncRedirectVerifyHelper> redirectCallbackHelper =
      new nsAsyncRedirectVerifyHelper();

  bool checkRedirectSynchronously = !openNewChannel;
  nsCOMPtr<nsIEventTarget> target = GetNeckoTarget();

  mRedirectChannel = newChannel;
  mRedirectFlags = redirectFlags;
  mOpenRedirectChannel = openNewChannel;
  nsresult rv = redirectCallbackHelper->Init(
      this, newChannel, redirectFlags, target, checkRedirectSynchronously);
  if (NS_FAILED(rv)) return rv;

  if (checkRedirectSynchronously && NS_FAILED(mStatus)) return mStatus;

  return NS_OK;
}

nsresult nsBaseChannel::ContinueRedirect() {
  // Make sure to do this _after_ making all the OnChannelRedirect calls
  mRedirectChannel->SetOriginalURI(OriginalURI());

  // If we fail to open the new channel, then we want to leave this channel
  // unaffected, so we defer tearing down our channel until we have succeeded
  // with the redirect.

  if (mOpenRedirectChannel) {
    nsresult rv = NS_OK;
<<<<<<< HEAD
    if (mLoadInfo && mLoadInfo->GetEnforceSecurity()) {
      MOZ_ASSERT(!mListenerContext, "mListenerContext should be null!");
      rv = mRedirectChannel->AsyncOpen2(mListener);
    } else {
      rv = mRedirectChannel->AsyncOpen(mListener, mListenerContext);
    }
||||||| merged common ancestors
    if (mLoadInfo && mLoadInfo->GetEnforceSecurity()) {
      MOZ_ASSERT(!mListenerContext, "mListenerContext should be null!");
      rv = mRedirectChannel->AsyncOpen2(mListener);
    }
    else {
      rv = mRedirectChannel->AsyncOpen(mListener, mListenerContext);
    }
=======
    rv = mRedirectChannel->AsyncOpen(mListener);
>>>>>>> upstream-releases
    NS_ENSURE_SUCCESS(rv, rv);
  }

  mRedirectChannel = nullptr;

  // close down this channel
  Cancel(NS_BINDING_REDIRECTED);
  ChannelDone();

  return NS_OK;
}

bool nsBaseChannel::HasContentTypeHint() const {
  NS_ASSERTION(!Pending(), "HasContentTypeHint called too late");
  return !mContentType.EqualsLiteral(UNKNOWN_CONTENT_TYPE);
}

<<<<<<< HEAD
nsresult nsBaseChannel::PushStreamConverter(const char *fromType,
                                            const char *toType,
                                            bool invalidatesContentLength,
                                            nsIStreamListener **result) {
||||||| merged common ancestors
nsresult
nsBaseChannel::PushStreamConverter(const char *fromType,
                                   const char *toType,
                                   bool invalidatesContentLength,
                                   nsIStreamListener **result)
{
=======
nsresult nsBaseChannel::PushStreamConverter(const char* fromType,
                                            const char* toType,
                                            bool invalidatesContentLength,
                                            nsIStreamListener** result) {
>>>>>>> upstream-releases
  NS_ASSERTION(mListener, "no listener");

  nsresult rv;
  nsCOMPtr<nsIStreamConverterService> scs =
      do_GetService(NS_STREAMCONVERTERSERVICE_CONTRACTID, &rv);
  if (NS_FAILED(rv)) return rv;

  nsCOMPtr<nsIStreamListener> converter;
  rv = scs->AsyncConvertData(fromType, toType, mListener, nullptr,
                             getter_AddRefs(converter));
  if (NS_SUCCEEDED(rv)) {
    mListener = converter;
    if (invalidatesContentLength) mContentLength = -1;
    if (result) {
      *result = nullptr;
      converter.swap(*result);
    }
  }
  return rv;
}

nsresult nsBaseChannel::BeginPumpingData() {
  nsresult rv;

  rv = BeginAsyncRead(this, getter_AddRefs(mRequest));
  if (NS_SUCCEEDED(rv)) {
    mPumpingData = true;
    return NS_OK;
  }
  if (rv != NS_ERROR_NOT_IMPLEMENTED) {
    return rv;
  }

  nsCOMPtr<nsIInputStream> stream;
  nsCOMPtr<nsIChannel> channel;
  rv = OpenContentStream(true, getter_AddRefs(stream), getter_AddRefs(channel));
  if (NS_FAILED(rv)) return rv;

  NS_ASSERTION(!stream || !channel, "Got both a channel and a stream?");

  if (channel) {
    nsCOMPtr<nsIRunnable> runnable = new RedirectRunnable(this, channel);
    rv = Dispatch(runnable.forget());
    if (NS_SUCCEEDED(rv)) mWaitingOnAsyncRedirect = true;
    return rv;
  }

  // By assigning mPump, we flag this channel as pending (see Pending).  It's
  // important that the pending flag is set when we call into the stream (the
  // call to AsyncRead results in the stream's AsyncWait method being called)
  // and especially when we call into the loadgroup.  Our caller takes care to
  // release mPump if we return an error.

  nsCOMPtr<nsIEventTarget> target = GetNeckoTarget();
  rv = nsInputStreamPump::Create(getter_AddRefs(mPump), stream, 0, 0, true,
                                 target);
  if (NS_FAILED(rv)) {
    return rv;
  }

  mPumpingData = true;
  mRequest = mPump;
  rv = mPump->AsyncRead(this, nullptr);
  if (NS_FAILED(rv)) {
    return rv;
  }

  RefPtr<BlockingPromise> promise;
  rv = ListenerBlockingPromise(getter_AddRefs(promise));
  if (NS_FAILED(rv)) {
    return rv;
  }

  if (promise) {
    mPump->Suspend();

    RefPtr<nsBaseChannel> self(this);
    nsCOMPtr<nsISerialEventTarget> serialTarget(do_QueryInterface(target));
    MOZ_ASSERT(serialTarget);

    promise->Then(
        serialTarget, __func__,
        [self, this](nsresult rv) {
          MOZ_ASSERT(mPump);
          MOZ_ASSERT(NS_SUCCEEDED(rv));
          mPump->Resume();
        },
        [self, this](nsresult rv) {
          MOZ_ASSERT(mPump);
          MOZ_ASSERT(NS_FAILED(rv));
          Cancel(rv);
          mPump->Resume();
        });
  }

  return NS_OK;
}

<<<<<<< HEAD
void nsBaseChannel::HandleAsyncRedirect(nsIChannel *newChannel) {
||||||| merged common ancestors
void
nsBaseChannel::HandleAsyncRedirect(nsIChannel* newChannel)
{
=======
void nsBaseChannel::HandleAsyncRedirect(nsIChannel* newChannel) {
>>>>>>> upstream-releases
  NS_ASSERTION(!mPumpingData, "Shouldn't have gotten here");

  nsresult rv = mStatus;
  if (NS_SUCCEEDED(mStatus)) {
    rv = Redirect(newChannel, nsIChannelEventSink::REDIRECT_TEMPORARY, true);
    if (NS_SUCCEEDED(rv)) {
      // OnRedirectVerifyCallback will be called asynchronously
      return;
    }
  }

  ContinueHandleAsyncRedirect(rv);
}

void nsBaseChannel::ContinueHandleAsyncRedirect(nsresult result) {
  mWaitingOnAsyncRedirect = false;

  if (NS_FAILED(result)) Cancel(result);

  if (NS_FAILED(result) && mListener) {
    // Notify our consumer ourselves
    mListener->OnStartRequest(this);
    mListener->OnStopRequest(this, mStatus);
    ChannelDone();
  }

  if (mLoadGroup) mLoadGroup->RemoveRequest(this, nullptr, mStatus);

  // Drop notification callbacks to prevent cycles.
  mCallbacks = nullptr;
  CallbacksChanged();
}

void nsBaseChannel::ClassifyURI() {
  // For channels created in the child process, delegate to the parent to
  // classify URIs.
  if (!XRE_IsParentProcess()) {
    return;
  }

  if (NS_ShouldClassifyChannel(this)) {
    RefPtr<nsChannelClassifier> classifier = new nsChannelClassifier(this);
    if (classifier) {
      classifier->Start();
    } else {
      Cancel(NS_ERROR_OUT_OF_MEMORY);
    }
  }
}

//-----------------------------------------------------------------------------
// nsBaseChannel::nsISupports

NS_IMPL_ISUPPORTS_INHERITED(nsBaseChannel, nsHashPropertyBag, nsIRequest,
                            nsIChannel, nsIThreadRetargetableRequest,
                            nsIInterfaceRequestor, nsITransportEventSink,
                            nsIRequestObserver, nsIStreamListener,
                            nsIThreadRetargetableStreamListener,
                            nsIAsyncVerifyRedirectCallback,
                            nsIPrivateBrowsingChannel)

//-----------------------------------------------------------------------------
// nsBaseChannel::nsIRequest

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetName(nsACString &result) {
||||||| merged common ancestors
nsBaseChannel::GetName(nsACString &result)
{
=======
nsBaseChannel::GetName(nsACString& result) {
>>>>>>> upstream-releases
  if (!mURI) {
    result.Truncate();
    return NS_OK;
  }
  return mURI->GetSpec(result);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::IsPending(bool *result) {
||||||| merged common ancestors
nsBaseChannel::IsPending(bool *result)
{
=======
nsBaseChannel::IsPending(bool* result) {
>>>>>>> upstream-releases
  *result = Pending();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetStatus(nsresult *status) {
||||||| merged common ancestors
nsBaseChannel::GetStatus(nsresult *status)
{
=======
nsBaseChannel::GetStatus(nsresult* status) {
>>>>>>> upstream-releases
  if (mRequest && NS_SUCCEEDED(mStatus)) {
    mRequest->GetStatus(status);
  } else {
    *status = mStatus;
  }
  return NS_OK;
}

NS_IMETHODIMP
nsBaseChannel::Cancel(nsresult status) {
  // Ignore redundant cancelation
  if (NS_FAILED(mStatus)) return NS_OK;

  mStatus = status;

  if (mRequest) mRequest->Cancel(status);

  return NS_OK;
}

NS_IMETHODIMP
nsBaseChannel::Suspend() {
  NS_ENSURE_TRUE(mPumpingData, NS_ERROR_NOT_INITIALIZED);
  NS_ENSURE_TRUE(mRequest, NS_ERROR_NOT_IMPLEMENTED);
  return mRequest->Suspend();
}

NS_IMETHODIMP
nsBaseChannel::Resume() {
  NS_ENSURE_TRUE(mPumpingData, NS_ERROR_NOT_INITIALIZED);
  NS_ENSURE_TRUE(mRequest, NS_ERROR_NOT_IMPLEMENTED);
  return mRequest->Resume();
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetLoadFlags(nsLoadFlags *aLoadFlags) {
||||||| merged common ancestors
nsBaseChannel::GetLoadFlags(nsLoadFlags *aLoadFlags)
{
=======
nsBaseChannel::GetLoadFlags(nsLoadFlags* aLoadFlags) {
>>>>>>> upstream-releases
  *aLoadFlags = mLoadFlags;
  return NS_OK;
}

NS_IMETHODIMP
nsBaseChannel::SetLoadFlags(nsLoadFlags aLoadFlags) {
  mLoadFlags = aLoadFlags;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetLoadGroup(nsILoadGroup **aLoadGroup) {
||||||| merged common ancestors
nsBaseChannel::GetLoadGroup(nsILoadGroup **aLoadGroup)
{
=======
nsBaseChannel::GetLoadGroup(nsILoadGroup** aLoadGroup) {
>>>>>>> upstream-releases
  NS_IF_ADDREF(*aLoadGroup = mLoadGroup);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::SetLoadGroup(nsILoadGroup *aLoadGroup) {
||||||| merged common ancestors
nsBaseChannel::SetLoadGroup(nsILoadGroup *aLoadGroup)
{
=======
nsBaseChannel::SetLoadGroup(nsILoadGroup* aLoadGroup) {
>>>>>>> upstream-releases
  if (!CanSetLoadGroup(aLoadGroup)) {
    return NS_ERROR_FAILURE;
  }

  mLoadGroup = aLoadGroup;
  CallbacksChanged();
  UpdatePrivateBrowsing();
  return NS_OK;
}

//-----------------------------------------------------------------------------
// nsBaseChannel::nsIChannel

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetOriginalURI(nsIURI **aURI) {
||||||| merged common ancestors
nsBaseChannel::GetOriginalURI(nsIURI **aURI)
{
=======
nsBaseChannel::GetOriginalURI(nsIURI** aURI) {
>>>>>>> upstream-releases
  *aURI = OriginalURI();
  NS_ADDREF(*aURI);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::SetOriginalURI(nsIURI *aURI) {
||||||| merged common ancestors
nsBaseChannel::SetOriginalURI(nsIURI *aURI)
{
=======
nsBaseChannel::SetOriginalURI(nsIURI* aURI) {
>>>>>>> upstream-releases
  NS_ENSURE_ARG_POINTER(aURI);
  mOriginalURI = aURI;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetURI(nsIURI **aURI) {
||||||| merged common ancestors
nsBaseChannel::GetURI(nsIURI **aURI)
{
=======
nsBaseChannel::GetURI(nsIURI** aURI) {
>>>>>>> upstream-releases
  NS_IF_ADDREF(*aURI = mURI);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetOwner(nsISupports **aOwner) {
||||||| merged common ancestors
nsBaseChannel::GetOwner(nsISupports **aOwner)
{
=======
nsBaseChannel::GetOwner(nsISupports** aOwner) {
>>>>>>> upstream-releases
  NS_IF_ADDREF(*aOwner = mOwner);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::SetOwner(nsISupports *aOwner) {
||||||| merged common ancestors
nsBaseChannel::SetOwner(nsISupports *aOwner)
{
=======
nsBaseChannel::SetOwner(nsISupports* aOwner) {
>>>>>>> upstream-releases
  mOwner = aOwner;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::SetLoadInfo(nsILoadInfo *aLoadInfo) {
||||||| merged common ancestors
nsBaseChannel::SetLoadInfo(nsILoadInfo* aLoadInfo)
{
=======
nsBaseChannel::SetLoadInfo(nsILoadInfo* aLoadInfo) {
  MOZ_RELEASE_ASSERT(aLoadInfo, "loadinfo can't be null");
>>>>>>> upstream-releases
  mLoadInfo = aLoadInfo;

  // Need to update |mNeckoTarget| when load info has changed.
  SetupNeckoTarget();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetLoadInfo(nsILoadInfo **aLoadInfo) {
||||||| merged common ancestors
nsBaseChannel::GetLoadInfo(nsILoadInfo** aLoadInfo)
{
=======
nsBaseChannel::GetLoadInfo(nsILoadInfo** aLoadInfo) {
>>>>>>> upstream-releases
  NS_IF_ADDREF(*aLoadInfo = mLoadInfo);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetIsDocument(bool *aIsDocument) {
||||||| merged common ancestors
nsBaseChannel::GetIsDocument(bool *aIsDocument)
{
=======
nsBaseChannel::GetIsDocument(bool* aIsDocument) {
>>>>>>> upstream-releases
  return NS_GetIsDocumentChannel(this, aIsDocument);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetNotificationCallbacks(nsIInterfaceRequestor **aCallbacks) {
||||||| merged common ancestors
nsBaseChannel::GetNotificationCallbacks(nsIInterfaceRequestor **aCallbacks)
{
=======
nsBaseChannel::GetNotificationCallbacks(nsIInterfaceRequestor** aCallbacks) {
>>>>>>> upstream-releases
  NS_IF_ADDREF(*aCallbacks = mCallbacks);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::SetNotificationCallbacks(nsIInterfaceRequestor *aCallbacks) {
||||||| merged common ancestors
nsBaseChannel::SetNotificationCallbacks(nsIInterfaceRequestor *aCallbacks)
{
=======
nsBaseChannel::SetNotificationCallbacks(nsIInterfaceRequestor* aCallbacks) {
>>>>>>> upstream-releases
  if (!CanSetCallbacks(aCallbacks)) {
    return NS_ERROR_FAILURE;
  }

  mCallbacks = aCallbacks;
  CallbacksChanged();
  UpdatePrivateBrowsing();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetSecurityInfo(nsISupports **aSecurityInfo) {
||||||| merged common ancestors
nsBaseChannel::GetSecurityInfo(nsISupports **aSecurityInfo)
{
=======
nsBaseChannel::GetSecurityInfo(nsISupports** aSecurityInfo) {
>>>>>>> upstream-releases
  NS_IF_ADDREF(*aSecurityInfo = mSecurityInfo);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetContentType(nsACString &aContentType) {
||||||| merged common ancestors
nsBaseChannel::GetContentType(nsACString &aContentType)
{
=======
nsBaseChannel::GetContentType(nsACString& aContentType) {
>>>>>>> upstream-releases
  aContentType = mContentType;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::SetContentType(const nsACString &aContentType) {
||||||| merged common ancestors
nsBaseChannel::SetContentType(const nsACString &aContentType)
{
=======
nsBaseChannel::SetContentType(const nsACString& aContentType) {
>>>>>>> upstream-releases
  // mContentCharset is unchanged if not parsed
  bool dummy;
  net_ParseContentType(aContentType, mContentType, mContentCharset, &dummy);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetContentCharset(nsACString &aContentCharset) {
||||||| merged common ancestors
nsBaseChannel::GetContentCharset(nsACString &aContentCharset)
{
=======
nsBaseChannel::GetContentCharset(nsACString& aContentCharset) {
>>>>>>> upstream-releases
  aContentCharset = mContentCharset;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::SetContentCharset(const nsACString &aContentCharset) {
||||||| merged common ancestors
nsBaseChannel::SetContentCharset(const nsACString &aContentCharset)
{
=======
nsBaseChannel::SetContentCharset(const nsACString& aContentCharset) {
>>>>>>> upstream-releases
  mContentCharset = aContentCharset;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetContentDisposition(uint32_t *aContentDisposition) {
||||||| merged common ancestors
nsBaseChannel::GetContentDisposition(uint32_t *aContentDisposition)
{
=======
nsBaseChannel::GetContentDisposition(uint32_t* aContentDisposition) {
>>>>>>> upstream-releases
  // preserve old behavior, fail unless explicitly set.
  if (mContentDispositionHint == UINT32_MAX) {
    return NS_ERROR_NOT_AVAILABLE;
  }

  *aContentDisposition = mContentDispositionHint;
  return NS_OK;
}

NS_IMETHODIMP
nsBaseChannel::SetContentDisposition(uint32_t aContentDisposition) {
  mContentDispositionHint = aContentDisposition;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetContentDispositionFilename(
    nsAString &aContentDispositionFilename) {
||||||| merged common ancestors
nsBaseChannel::GetContentDispositionFilename(nsAString &aContentDispositionFilename)
{
=======
nsBaseChannel::GetContentDispositionFilename(
    nsAString& aContentDispositionFilename) {
>>>>>>> upstream-releases
  if (!mContentDispositionFilename) {
    return NS_ERROR_NOT_AVAILABLE;
  }

  aContentDispositionFilename = *mContentDispositionFilename;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::SetContentDispositionFilename(
    const nsAString &aContentDispositionFilename) {
||||||| merged common ancestors
nsBaseChannel::SetContentDispositionFilename(const nsAString &aContentDispositionFilename)
{
=======
nsBaseChannel::SetContentDispositionFilename(
    const nsAString& aContentDispositionFilename) {
>>>>>>> upstream-releases
  mContentDispositionFilename = new nsString(aContentDispositionFilename);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetContentDispositionHeader(
    nsACString &aContentDispositionHeader) {
||||||| merged common ancestors
nsBaseChannel::GetContentDispositionHeader(nsACString &aContentDispositionHeader)
{
=======
nsBaseChannel::GetContentDispositionHeader(
    nsACString& aContentDispositionHeader) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_AVAILABLE;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetContentLength(int64_t *aContentLength) {
||||||| merged common ancestors
nsBaseChannel::GetContentLength(int64_t *aContentLength)
{
=======
nsBaseChannel::GetContentLength(int64_t* aContentLength) {
>>>>>>> upstream-releases
  *aContentLength = mContentLength;
  return NS_OK;
}

NS_IMETHODIMP
nsBaseChannel::SetContentLength(int64_t aContentLength) {
  mContentLength = aContentLength;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::Open(nsIInputStream **result) {
||||||| merged common ancestors
nsBaseChannel::Open(nsIInputStream **result)
{
=======
nsBaseChannel::Open(nsIInputStream** aStream) {
  nsCOMPtr<nsIStreamListener> listener;
  nsresult rv =
      nsContentSecurityManager::doContentSecurityCheck(this, listener);
  NS_ENSURE_SUCCESS(rv, rv);

>>>>>>> upstream-releases
  NS_ENSURE_TRUE(mURI, NS_ERROR_NOT_INITIALIZED);
  NS_ENSURE_TRUE(!mPumpingData, NS_ERROR_IN_PROGRESS);
  NS_ENSURE_TRUE(!mWasOpened, NS_ERROR_IN_PROGRESS);

  nsCOMPtr<nsIChannel> chan;
  rv = OpenContentStream(false, aStream, getter_AddRefs(chan));
  NS_ASSERTION(!chan || !*aStream, "Got both a channel and a stream?");
  if (NS_SUCCEEDED(rv) && chan) {
<<<<<<< HEAD
    rv = Redirect(chan, nsIChannelEventSink::REDIRECT_INTERNAL, false);
    if (NS_FAILED(rv)) return rv;
    rv = chan->Open(result);
||||||| merged common ancestors
      rv = Redirect(chan, nsIChannelEventSink::REDIRECT_INTERNAL, false);
      if (NS_FAILED(rv))
          return rv;
      rv = chan->Open(result);
=======
    rv = Redirect(chan, nsIChannelEventSink::REDIRECT_INTERNAL, false);
    if (NS_FAILED(rv)) return rv;
    rv = chan->Open(aStream);
>>>>>>> upstream-releases
  } else if (rv == NS_ERROR_NOT_IMPLEMENTED)
    return NS_ImplementChannelOpen(this, aStream);

  if (NS_SUCCEEDED(rv)) {
    mWasOpened = true;
    ClassifyURI();
  }

  return rv;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::Open2(nsIInputStream **aStream) {
  nsCOMPtr<nsIStreamListener> listener;
  nsresult rv =
      nsContentSecurityManager::doContentSecurityCheck(this, listener);
  NS_ENSURE_SUCCESS(rv, rv);
  return Open(aStream);
}
||||||| merged common ancestors
nsBaseChannel::Open2(nsIInputStream** aStream)
{
  nsCOMPtr<nsIStreamListener> listener;
  nsresult rv = nsContentSecurityManager::doContentSecurityCheck(this, listener);
  NS_ENSURE_SUCCESS(rv, rv);
  return Open(aStream);
}
=======
nsBaseChannel::AsyncOpen(nsIStreamListener* aListener) {
  nsCOMPtr<nsIStreamListener> listener = aListener;
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
nsBaseChannel::AsyncOpen(nsIStreamListener *listener, nsISupports *ctxt) {
  MOZ_ASSERT(
      !mLoadInfo || mLoadInfo->GetSecurityMode() == 0 ||
          mLoadInfo->GetInitialSecurityCheckDone() ||
          (mLoadInfo->GetSecurityMode() ==
               nsILoadInfo::SEC_ALLOW_CROSS_ORIGIN_DATA_IS_NULL &&
           nsContentUtils::IsSystemPrincipal(mLoadInfo->LoadingPrincipal())),
      "security flags in loadInfo but asyncOpen2() not called");
||||||| merged common ancestors
NS_IMETHODIMP
nsBaseChannel::AsyncOpen(nsIStreamListener *listener, nsISupports *ctxt)
{
  MOZ_ASSERT(!mLoadInfo ||
             mLoadInfo->GetSecurityMode() == 0 ||
             mLoadInfo->GetInitialSecurityCheckDone() ||
             (mLoadInfo->GetSecurityMode() == nsILoadInfo::SEC_ALLOW_CROSS_ORIGIN_DATA_IS_NULL &&
              nsContentUtils::IsSystemPrincipal(mLoadInfo->LoadingPrincipal())),
             "security flags in loadInfo but asyncOpen2() not called");
=======
  nsresult rv =
      nsContentSecurityManager::doContentSecurityCheck(this, listener);
  if (NS_FAILED(rv)) {
    mCallbacks = nullptr;
    return rv;
  }

  MOZ_ASSERT(
      !mLoadInfo || mLoadInfo->GetSecurityMode() == 0 ||
          mLoadInfo->GetInitialSecurityCheckDone() ||
          (mLoadInfo->GetSecurityMode() ==
               nsILoadInfo::SEC_ALLOW_CROSS_ORIGIN_DATA_IS_NULL &&
           nsContentUtils::IsSystemPrincipal(mLoadInfo->LoadingPrincipal())),
      "security flags in loadInfo but doContentSecurityCheck() not called");
>>>>>>> upstream-releases

  NS_ENSURE_TRUE(mURI, NS_ERROR_NOT_INITIALIZED);
  NS_ENSURE_TRUE(!mPumpingData, NS_ERROR_IN_PROGRESS);
  NS_ENSURE_TRUE(!mWasOpened, NS_ERROR_ALREADY_OPENED);
  NS_ENSURE_ARG(listener);

  SetupNeckoTarget();

  // Skip checking for chrome:// sub-resources.
  nsAutoCString scheme;
  mURI->GetScheme(scheme);
  if (!scheme.EqualsLiteral("file")) {
    NS_CompareLoadInfoAndLoadContext(this);
  }

  // Ensure that this is an allowed port before proceeding.
  rv = NS_CheckPortSafety(mURI);
  if (NS_FAILED(rv)) {
    mCallbacks = nullptr;
    return rv;
  }

  // Store the listener and context early so that OpenContentStream and the
  // stream's AsyncWait method (called by AsyncRead) can have access to them
  // via PushStreamConverter and the StreamListener methods.  However, since
  // this typically introduces a reference cycle between this and the listener,
  // we need to be sure to break the reference if this method does not succeed.
  mListener = listener;

  // This method assigns mPump as a side-effect.  We need to clear mPump if
  // this method fails.
  rv = BeginPumpingData();
  if (NS_FAILED(rv)) {
    mPump = nullptr;
    mRequest = nullptr;
    mPumpingData = false;
    ChannelDone();
    mCallbacks = nullptr;
    return rv;
  }

  // At this point, we are going to return success no matter what.

  mWasOpened = true;

  SUSPEND_PUMP_FOR_SCOPE();

  if (mLoadGroup) mLoadGroup->AddRequest(this, nullptr);

  ClassifyURI();

  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP
nsBaseChannel::AsyncOpen2(nsIStreamListener *aListener) {
  nsCOMPtr<nsIStreamListener> listener = aListener;
  nsresult rv =
      nsContentSecurityManager::doContentSecurityCheck(this, listener);
  if (NS_FAILED(rv)) {
    mCallbacks = nullptr;
    return rv;
  }
  return AsyncOpen(listener, nullptr);
}

||||||| merged common ancestors
NS_IMETHODIMP
nsBaseChannel::AsyncOpen2(nsIStreamListener *aListener)
{
  nsCOMPtr<nsIStreamListener> listener = aListener;
  nsresult rv = nsContentSecurityManager::doContentSecurityCheck(this, listener);
  if (NS_FAILED(rv)) {
    mCallbacks = nullptr;
    return rv;
  }
  return AsyncOpen(listener, nullptr);
}

=======
>>>>>>> upstream-releases
//-----------------------------------------------------------------------------
// nsBaseChannel::nsITransportEventSink

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::OnTransportStatus(nsITransport *transport, nsresult status,
                                 int64_t progress, int64_t progressMax) {
||||||| merged common ancestors
nsBaseChannel::OnTransportStatus(nsITransport *transport, nsresult status,
                                 int64_t progress, int64_t progressMax)
{
=======
nsBaseChannel::OnTransportStatus(nsITransport* transport, nsresult status,
                                 int64_t progress, int64_t progressMax) {
>>>>>>> upstream-releases
  // In some cases, we may wish to suppress transport-layer status events.

  if (!mPumpingData || NS_FAILED(mStatus)) {
    return NS_OK;
  }

  SUSPEND_PUMP_FOR_SCOPE();

  // Lazily fetch mProgressSink
  if (!mProgressSink) {
    if (mQueriedProgressSink) {
      return NS_OK;
    }
    GetCallback(mProgressSink);
    mQueriedProgressSink = true;
    if (!mProgressSink) {
      return NS_OK;
    }
  }

  if (!HasLoadFlag(LOAD_BACKGROUND)) {
    nsAutoString statusArg;
    if (GetStatusArg(status, statusArg)) {
      mProgressSink->OnStatus(this, nullptr, status, statusArg.get());
    }
  }

  if (progress) {
    mProgressSink->OnProgress(this, nullptr, progress, progressMax);
  }

  return NS_OK;
}

//-----------------------------------------------------------------------------
// nsBaseChannel::nsIInterfaceRequestor

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetInterface(const nsIID &iid, void **result) {
||||||| merged common ancestors
nsBaseChannel::GetInterface(const nsIID &iid, void **result)
{
=======
nsBaseChannel::GetInterface(const nsIID& iid, void** result) {
>>>>>>> upstream-releases
  NS_QueryNotificationCallbacks(mCallbacks, mLoadGroup, iid, result);
  return *result ? NS_OK : NS_ERROR_NO_INTERFACE;
}

//-----------------------------------------------------------------------------
// nsBaseChannel::nsIRequestObserver

<<<<<<< HEAD
static void CallTypeSniffers(void *aClosure, const uint8_t *aData,
                             uint32_t aCount) {
  nsIChannel *chan = static_cast<nsIChannel *>(aClosure);
||||||| merged common ancestors
static void
CallTypeSniffers(void *aClosure, const uint8_t *aData, uint32_t aCount)
{
  nsIChannel *chan = static_cast<nsIChannel*>(aClosure);
=======
static void CallTypeSniffers(void* aClosure, const uint8_t* aData,
                             uint32_t aCount) {
  nsIChannel* chan = static_cast<nsIChannel*>(aClosure);
>>>>>>> upstream-releases

  nsAutoCString newType;
  NS_SniffContent(NS_CONTENT_SNIFFER_CATEGORY, chan, aData, aCount, newType);
  if (!newType.IsEmpty()) {
    chan->SetContentType(newType);
  }
}

<<<<<<< HEAD
static void CallUnknownTypeSniffer(void *aClosure, const uint8_t *aData,
                                   uint32_t aCount) {
  nsIChannel *chan = static_cast<nsIChannel *>(aClosure);
||||||| merged common ancestors
static void
CallUnknownTypeSniffer(void *aClosure, const uint8_t *aData, uint32_t aCount)
{
  nsIChannel *chan = static_cast<nsIChannel*>(aClosure);
=======
static void CallUnknownTypeSniffer(void* aClosure, const uint8_t* aData,
                                   uint32_t aCount) {
  nsIChannel* chan = static_cast<nsIChannel*>(aClosure);
>>>>>>> upstream-releases

  RefPtr<nsUnknownDecoder> sniffer = new nsUnknownDecoder();

  nsAutoCString detected;
  nsresult rv = sniffer->GetMIMETypeFromContent(chan, aData, aCount, detected);
  if (NS_SUCCEEDED(rv)) chan->SetContentType(detected);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::OnStartRequest(nsIRequest *request, nsISupports *ctxt) {
||||||| merged common ancestors
nsBaseChannel::OnStartRequest(nsIRequest *request, nsISupports *ctxt)
{
=======
nsBaseChannel::OnStartRequest(nsIRequest* request) {
>>>>>>> upstream-releases
  MOZ_ASSERT_IF(mRequest, request == mRequest);

  if (mPump) {
    // If our content type is unknown, use the content type
    // sniffer. If the sniffer is not available for some reason, then we just
    // keep going as-is.
    if (NS_SUCCEEDED(mStatus) &&
        mContentType.EqualsLiteral(UNKNOWN_CONTENT_TYPE)) {
      mPump->PeekStream(CallUnknownTypeSniffer,
                        static_cast<nsIChannel *>(this));
    }

    // Now, the general type sniffers. Skip this if we have none.
    if (mLoadFlags & LOAD_CALL_CONTENT_SNIFFERS)
      mPump->PeekStream(CallTypeSniffers, static_cast<nsIChannel *>(this));
  }

  SUSPEND_PUMP_FOR_SCOPE();

<<<<<<< HEAD
  if (mListener)  // null in case of redirect
    return mListener->OnStartRequest(this, mListenerContext);
||||||| merged common ancestors
  if (mListener) // null in case of redirect
      return mListener->OnStartRequest(this, mListenerContext);
=======
  if (mListener)  // null in case of redirect
    return mListener->OnStartRequest(this);
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::OnStopRequest(nsIRequest *request, nsISupports *ctxt,
                             nsresult status) {
||||||| merged common ancestors
nsBaseChannel::OnStopRequest(nsIRequest *request, nsISupports *ctxt,
                             nsresult status)
{
=======
nsBaseChannel::OnStopRequest(nsIRequest* request, nsresult status) {
>>>>>>> upstream-releases
  // If both mStatus and status are failure codes, we keep mStatus as-is since
  // that is consistent with our GetStatus and Cancel methods.
  if (NS_SUCCEEDED(mStatus)) mStatus = status;

  // Cause Pending to return false.
  mPump = nullptr;
  mRequest = nullptr;
  mPumpingData = false;

<<<<<<< HEAD
  if (mListener)  // null in case of redirect
    mListener->OnStopRequest(this, mListenerContext, mStatus);
||||||| merged common ancestors
  if (mListener) // null in case of redirect
      mListener->OnStopRequest(this, mListenerContext, mStatus);
=======
  if (mListener)  // null in case of redirect
    mListener->OnStopRequest(this, mStatus);
>>>>>>> upstream-releases
  ChannelDone();

  // No need to suspend pump in this scope since we will not be receiving
  // any more events from it.

  if (mLoadGroup) mLoadGroup->RemoveRequest(this, nullptr, mStatus);

  // Drop notification callbacks to prevent cycles.
  mCallbacks = nullptr;
  CallbacksChanged();

  return NS_OK;
}

//-----------------------------------------------------------------------------
// nsBaseChannel::nsIStreamListener

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::OnDataAvailable(nsIRequest *request, nsISupports *ctxt,
                               nsIInputStream *stream, uint64_t offset,
                               uint32_t count) {
||||||| merged common ancestors
nsBaseChannel::OnDataAvailable(nsIRequest *request, nsISupports *ctxt,
                               nsIInputStream *stream, uint64_t offset,
                               uint32_t count)
{
=======
nsBaseChannel::OnDataAvailable(nsIRequest* request, nsIInputStream* stream,
                               uint64_t offset, uint32_t count) {
>>>>>>> upstream-releases
  SUSPEND_PUMP_FOR_SCOPE();

<<<<<<< HEAD
  nsresult rv =
      mListener->OnDataAvailable(this, mListenerContext, stream, offset, count);
||||||| merged common ancestors
  nsresult rv = mListener->OnDataAvailable(this, mListenerContext, stream,
                                           offset, count);
=======
  nsresult rv = mListener->OnDataAvailable(this, stream, offset, count);
>>>>>>> upstream-releases
  if (mSynthProgressEvents && NS_SUCCEEDED(rv)) {
    int64_t prog = offset + count;
    if (NS_IsMainThread()) {
      OnTransportStatus(nullptr, NS_NET_STATUS_READING, prog, mContentLength);
    } else {
      class OnTransportStatusAsyncEvent : public mozilla::Runnable {
        RefPtr<nsBaseChannel> mChannel;
        int64_t mProgress;
        int64_t mContentLength;
<<<<<<< HEAD

       public:
        OnTransportStatusAsyncEvent(nsBaseChannel *aChannel, int64_t aProgress,
||||||| merged common ancestors
      public:
        OnTransportStatusAsyncEvent(nsBaseChannel* aChannel,
                                    int64_t aProgress,
=======

       public:
        OnTransportStatusAsyncEvent(nsBaseChannel* aChannel, int64_t aProgress,
>>>>>>> upstream-releases
                                    int64_t aContentLength)
            : mozilla::Runnable("OnTransportStatusAsyncEvent"),
              mChannel(aChannel),
              mProgress(aProgress),
              mContentLength(aContentLength) {}

        NS_IMETHOD Run() override {
          return mChannel->OnTransportStatus(nullptr, NS_NET_STATUS_READING,
                                             mProgress, mContentLength);
        }
      };

      nsCOMPtr<nsIRunnable> runnable =
          new OnTransportStatusAsyncEvent(this, prog, mContentLength);
      Dispatch(runnable.forget());
    }
  }

  return rv;
}

NS_IMETHODIMP
nsBaseChannel::OnRedirectVerifyCallback(nsresult result) {
  if (NS_SUCCEEDED(result)) result = ContinueRedirect();

  if (NS_FAILED(result) && !mWaitingOnAsyncRedirect) {
    if (NS_SUCCEEDED(mStatus)) mStatus = result;
    return NS_OK;
  }

  if (mWaitingOnAsyncRedirect) ContinueHandleAsyncRedirect(result);

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::RetargetDeliveryTo(nsIEventTarget *aEventTarget) {
||||||| merged common ancestors
nsBaseChannel::RetargetDeliveryTo(nsIEventTarget* aEventTarget)
{
=======
nsBaseChannel::RetargetDeliveryTo(nsIEventTarget* aEventTarget) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());

  NS_ENSURE_TRUE(mRequest, NS_ERROR_NOT_INITIALIZED);

  nsCOMPtr<nsIThreadRetargetableRequest> req;
  if (mAllowThreadRetargeting) {
    req = do_QueryInterface(mRequest);
  }

  NS_ENSURE_TRUE(req, NS_ERROR_NOT_IMPLEMENTED);

  return req->RetargetDeliveryTo(aEventTarget);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseChannel::GetDeliveryTarget(nsIEventTarget **aEventTarget) {
||||||| merged common ancestors
nsBaseChannel::GetDeliveryTarget(nsIEventTarget** aEventTarget)
{
=======
nsBaseChannel::GetDeliveryTarget(nsIEventTarget** aEventTarget) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());

  NS_ENSURE_TRUE(mRequest, NS_ERROR_NOT_INITIALIZED);

  nsCOMPtr<nsIThreadRetargetableRequest> req;
  req = do_QueryInterface(mRequest);

  NS_ENSURE_TRUE(req, NS_ERROR_NOT_IMPLEMENTED);
  return req->GetDeliveryTarget(aEventTarget);
}

NS_IMETHODIMP
nsBaseChannel::CheckListenerChain() {
  MOZ_ASSERT(NS_IsMainThread());

  if (!mAllowThreadRetargeting) {
    return NS_ERROR_NOT_IMPLEMENTED;
  }

  nsCOMPtr<nsIThreadRetargetableStreamListener> listener =
      do_QueryInterface(mListener);
  if (!listener) {
    return NS_ERROR_NO_INTERFACE;
  }

  return listener->CheckListenerChain();
}

void nsBaseChannel::SetupNeckoTarget() {
  mNeckoTarget =
      nsContentUtils::GetEventTargetByLoadInfo(mLoadInfo, TaskCategory::Other);
}
