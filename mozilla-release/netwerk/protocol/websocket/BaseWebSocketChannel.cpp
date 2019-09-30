/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=8 et tw=80 : */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "WebSocketLog.h"
#include "BaseWebSocketChannel.h"
#include "MainThreadUtils.h"
#include "nsILoadGroup.h"
#include "nsINode.h"
#include "nsIInterfaceRequestor.h"
#include "nsAutoPtr.h"
#include "nsProxyRelease.h"
#include "nsStandardURL.h"
#include "LoadInfo.h"
#include "mozilla/dom/ContentChild.h"
#include "nsITransportProvider.h"

using mozilla::dom::ContentChild;

namespace mozilla {
namespace net {

LazyLogModule webSocketLog("nsWebSocket");
static uint64_t gNextWebSocketID = 0;

// We use only 53 bits for the WebSocket serial ID so that it can be converted
// to and from a JS value without loss of precision. The upper bits of the
// WebSocket serial ID hold the process ID. The lower bits identify the
// WebSocket.
static const uint64_t kWebSocketIDTotalBits = 53;
static const uint64_t kWebSocketIDProcessBits = 22;
static const uint64_t kWebSocketIDWebSocketBits =
    kWebSocketIDTotalBits - kWebSocketIDProcessBits;

BaseWebSocketChannel::BaseWebSocketChannel()
<<<<<<< HEAD
    : mWasOpened(0),
      mClientSetPingInterval(0),
      mClientSetPingTimeout(0),
      mEncrypted(false),
      mPingForced(false),
      mIsServerSide(false),
      mPingInterval(0),
      mPingResponseTimeout(10000) {
||||||| merged common ancestors
  : mWasOpened(0)
  , mClientSetPingInterval(0)
  , mClientSetPingTimeout(0)
  , mEncrypted(false)
  , mPingForced(false)
  , mIsServerSide(false)
  , mPingInterval(0)
  , mPingResponseTimeout(10000)
{
=======
    : mWasOpened(0),
      mClientSetPingInterval(0),
      mClientSetPingTimeout(0),
      mEncrypted(false),
      mPingForced(false),
      mIsServerSide(false),
      mPingInterval(0),
      mPingResponseTimeout(10000),
      mHttpChannelId(0) {
>>>>>>> upstream-releases
  // Generation of a unique serial ID.
  uint64_t processID = 0;
  if (XRE_IsContentProcess()) {
    ContentChild *cc = ContentChild::GetSingleton();
    processID = cc->GetID();
  }

  uint64_t processBits =
      processID & ((uint64_t(1) << kWebSocketIDProcessBits) - 1);

  // Make sure no actual webSocket ends up with mWebSocketID == 0 but less then
  // what the kWebSocketIDProcessBits allows.
  if (++gNextWebSocketID >= (uint64_t(1) << kWebSocketIDWebSocketBits)) {
    gNextWebSocketID = 1;
  }

  uint64_t webSocketBits =
      gNextWebSocketID & ((uint64_t(1) << kWebSocketIDWebSocketBits) - 1);
  mSerial = (processBits << kWebSocketIDWebSocketBits) | webSocketBits;
}

//-----------------------------------------------------------------------------
// BaseWebSocketChannel::nsIWebSocketChannel
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::GetOriginalURI(nsIURI **aOriginalURI) {
||||||| merged common ancestors
BaseWebSocketChannel::GetOriginalURI(nsIURI **aOriginalURI)
{
=======
BaseWebSocketChannel::GetOriginalURI(nsIURI** aOriginalURI) {
>>>>>>> upstream-releases
  LOG(("BaseWebSocketChannel::GetOriginalURI() %p\n", this));

  if (!mOriginalURI) return NS_ERROR_NOT_INITIALIZED;
  NS_ADDREF(*aOriginalURI = mOriginalURI);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::GetURI(nsIURI **aURI) {
||||||| merged common ancestors
BaseWebSocketChannel::GetURI(nsIURI **aURI)
{
=======
BaseWebSocketChannel::GetURI(nsIURI** aURI) {
>>>>>>> upstream-releases
  LOG(("BaseWebSocketChannel::GetURI() %p\n", this));

  if (!mOriginalURI) return NS_ERROR_NOT_INITIALIZED;
  if (mURI)
    NS_ADDREF(*aURI = mURI);
  else
    NS_ADDREF(*aURI = mOriginalURI);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::GetNotificationCallbacks(
    nsIInterfaceRequestor **aNotificationCallbacks) {
||||||| merged common ancestors
BaseWebSocketChannel::
GetNotificationCallbacks(nsIInterfaceRequestor **aNotificationCallbacks)
{
=======
BaseWebSocketChannel::GetNotificationCallbacks(
    nsIInterfaceRequestor** aNotificationCallbacks) {
>>>>>>> upstream-releases
  LOG(("BaseWebSocketChannel::GetNotificationCallbacks() %p\n", this));
  NS_IF_ADDREF(*aNotificationCallbacks = mCallbacks);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::SetNotificationCallbacks(
    nsIInterfaceRequestor *aNotificationCallbacks) {
||||||| merged common ancestors
BaseWebSocketChannel::
SetNotificationCallbacks(nsIInterfaceRequestor *aNotificationCallbacks)
{
=======
BaseWebSocketChannel::SetNotificationCallbacks(
    nsIInterfaceRequestor* aNotificationCallbacks) {
>>>>>>> upstream-releases
  LOG(("BaseWebSocketChannel::SetNotificationCallbacks() %p\n", this));
  mCallbacks = aNotificationCallbacks;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::GetLoadGroup(nsILoadGroup **aLoadGroup) {
||||||| merged common ancestors
BaseWebSocketChannel::GetLoadGroup(nsILoadGroup **aLoadGroup)
{
=======
BaseWebSocketChannel::GetLoadGroup(nsILoadGroup** aLoadGroup) {
>>>>>>> upstream-releases
  LOG(("BaseWebSocketChannel::GetLoadGroup() %p\n", this));
  NS_IF_ADDREF(*aLoadGroup = mLoadGroup);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::SetLoadGroup(nsILoadGroup *aLoadGroup) {
||||||| merged common ancestors
BaseWebSocketChannel::SetLoadGroup(nsILoadGroup *aLoadGroup)
{
=======
BaseWebSocketChannel::SetLoadGroup(nsILoadGroup* aLoadGroup) {
>>>>>>> upstream-releases
  LOG(("BaseWebSocketChannel::SetLoadGroup() %p\n", this));
  mLoadGroup = aLoadGroup;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::SetLoadInfo(nsILoadInfo *aLoadInfo) {
||||||| merged common ancestors
BaseWebSocketChannel::SetLoadInfo(nsILoadInfo* aLoadInfo)
{
=======
BaseWebSocketChannel::SetLoadInfo(nsILoadInfo* aLoadInfo) {
  MOZ_RELEASE_ASSERT(aLoadInfo, "loadinfo can't be null");
>>>>>>> upstream-releases
  mLoadInfo = aLoadInfo;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::GetLoadInfo(nsILoadInfo **aLoadInfo) {
||||||| merged common ancestors
BaseWebSocketChannel::GetLoadInfo(nsILoadInfo** aLoadInfo)
{
=======
BaseWebSocketChannel::GetLoadInfo(nsILoadInfo** aLoadInfo) {
>>>>>>> upstream-releases
  NS_IF_ADDREF(*aLoadInfo = mLoadInfo);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::GetExtensions(nsACString &aExtensions) {
||||||| merged common ancestors
BaseWebSocketChannel::GetExtensions(nsACString &aExtensions)
{
=======
BaseWebSocketChannel::GetExtensions(nsACString& aExtensions) {
>>>>>>> upstream-releases
  LOG(("BaseWebSocketChannel::GetExtensions() %p\n", this));
  aExtensions = mNegotiatedExtensions;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::GetProtocol(nsACString &aProtocol) {
||||||| merged common ancestors
BaseWebSocketChannel::GetProtocol(nsACString &aProtocol)
{
=======
BaseWebSocketChannel::GetProtocol(nsACString& aProtocol) {
>>>>>>> upstream-releases
  LOG(("BaseWebSocketChannel::GetProtocol() %p\n", this));
  aProtocol = mProtocol;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::SetProtocol(const nsACString &aProtocol) {
||||||| merged common ancestors
BaseWebSocketChannel::SetProtocol(const nsACString &aProtocol)
{
=======
BaseWebSocketChannel::SetProtocol(const nsACString& aProtocol) {
>>>>>>> upstream-releases
  LOG(("BaseWebSocketChannel::SetProtocol() %p\n", this));
  mProtocol = aProtocol; /* the sub protocol */
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::GetPingInterval(uint32_t *aSeconds) {
||||||| merged common ancestors
BaseWebSocketChannel::GetPingInterval(uint32_t *aSeconds)
{
=======
BaseWebSocketChannel::GetPingInterval(uint32_t* aSeconds) {
>>>>>>> upstream-releases
  // stored in ms but should only have second resolution
  MOZ_ASSERT(!(mPingInterval % 1000));

  *aSeconds = mPingInterval / 1000;
  return NS_OK;
}

NS_IMETHODIMP
BaseWebSocketChannel::SetPingInterval(uint32_t aSeconds) {
  MOZ_ASSERT(NS_IsMainThread());

  if (mWasOpened) {
    return NS_ERROR_IN_PROGRESS;
  }

  mPingInterval = aSeconds * 1000;
  mClientSetPingInterval = 1;

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::GetPingTimeout(uint32_t *aSeconds) {
||||||| merged common ancestors
BaseWebSocketChannel::GetPingTimeout(uint32_t *aSeconds)
{
=======
BaseWebSocketChannel::GetPingTimeout(uint32_t* aSeconds) {
>>>>>>> upstream-releases
  // stored in ms but should only have second resolution
  MOZ_ASSERT(!(mPingResponseTimeout % 1000));

  *aSeconds = mPingResponseTimeout / 1000;
  return NS_OK;
}

NS_IMETHODIMP
BaseWebSocketChannel::SetPingTimeout(uint32_t aSeconds) {
  MOZ_ASSERT(NS_IsMainThread());

  if (mWasOpened) {
    return NS_ERROR_IN_PROGRESS;
  }

  mPingResponseTimeout = aSeconds * 1000;
  mClientSetPingTimeout = 1;

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::InitLoadInfo(nsINode *aLoadingNode,
                                   nsIPrincipal *aLoadingPrincipal,
                                   nsIPrincipal *aTriggeringPrincipal,
||||||| merged common ancestors
BaseWebSocketChannel::InitLoadInfo(nsINode* aLoadingNode,
                                   nsIPrincipal* aLoadingPrincipal,
                                   nsIPrincipal* aTriggeringPrincipal,
=======
BaseWebSocketChannel::InitLoadInfoNative(nsINode* aLoadingNode,
                                         nsIPrincipal* aLoadingPrincipal,
                                         nsIPrincipal* aTriggeringPrincipal,
                                         nsICookieSettings* aCookieSettings,
                                         uint32_t aSecurityFlags,
                                         uint32_t aContentPolicyType) {
  mLoadInfo = new LoadInfo(aLoadingPrincipal, aTriggeringPrincipal,
                           aLoadingNode, aSecurityFlags, aContentPolicyType);
  if (aCookieSettings) {
    mLoadInfo->SetCookieSettings(aCookieSettings);
  }
  return NS_OK;
}

NS_IMETHODIMP
BaseWebSocketChannel::InitLoadInfo(nsINode* aLoadingNode,
                                   nsIPrincipal* aLoadingPrincipal,
                                   nsIPrincipal* aTriggeringPrincipal,
>>>>>>> upstream-releases
                                   uint32_t aSecurityFlags,
<<<<<<< HEAD
                                   uint32_t aContentPolicyType) {
  mLoadInfo = new LoadInfo(aLoadingPrincipal, aTriggeringPrincipal,
                           aLoadingNode, aSecurityFlags, aContentPolicyType);
  return NS_OK;
||||||| merged common ancestors
                                   uint32_t aContentPolicyType)
{
  mLoadInfo = new LoadInfo(aLoadingPrincipal, aTriggeringPrincipal,
                           aLoadingNode, aSecurityFlags, aContentPolicyType);
  return NS_OK;
=======
                                   uint32_t aContentPolicyType) {
  return InitLoadInfoNative(aLoadingNode, aLoadingPrincipal,
                            aTriggeringPrincipal, nullptr, aSecurityFlags,
                            aContentPolicyType);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::GetSerial(uint32_t *aSerial) {
||||||| merged common ancestors
BaseWebSocketChannel::GetSerial(uint32_t* aSerial)
{
=======
BaseWebSocketChannel::GetSerial(uint32_t* aSerial) {
>>>>>>> upstream-releases
  if (!aSerial) {
    return NS_ERROR_FAILURE;
  }

  *aSerial = mSerial;
  return NS_OK;
}

NS_IMETHODIMP
BaseWebSocketChannel::SetSerial(uint32_t aSerial) {
  mSerial = aSerial;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::SetServerParameters(
    nsITransportProvider *aProvider, const nsACString &aNegotiatedExtensions) {
||||||| merged common ancestors
BaseWebSocketChannel::SetServerParameters(nsITransportProvider* aProvider,
                                          const nsACString& aNegotiatedExtensions)
{
=======
BaseWebSocketChannel::SetServerParameters(
    nsITransportProvider* aProvider, const nsACString& aNegotiatedExtensions) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aProvider);
  mServerTransportProvider = aProvider;
  mNegotiatedExtensions = aNegotiatedExtensions;
  mIsServerSide = true;
  return NS_OK;
}

NS_IMETHODIMP
BaseWebSocketChannel::GetHttpChannelId(uint64_t* aHttpChannelId) {
  *aHttpChannelId = mHttpChannelId;
  return NS_OK;
}

//-----------------------------------------------------------------------------
// BaseWebSocketChannel::nsIProtocolHandler
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::GetScheme(nsACString &aScheme) {
||||||| merged common ancestors
BaseWebSocketChannel::GetScheme(nsACString &aScheme)
{
=======
BaseWebSocketChannel::GetScheme(nsACString& aScheme) {
>>>>>>> upstream-releases
  LOG(("BaseWebSocketChannel::GetScheme() %p\n", this));

  if (mEncrypted)
    aScheme.AssignLiteral("wss");
  else
    aScheme.AssignLiteral("ws");
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::GetDefaultPort(int32_t *aDefaultPort) {
||||||| merged common ancestors
BaseWebSocketChannel::GetDefaultPort(int32_t *aDefaultPort)
{
=======
BaseWebSocketChannel::GetDefaultPort(int32_t* aDefaultPort) {
>>>>>>> upstream-releases
  LOG(("BaseWebSocketChannel::GetDefaultPort() %p\n", this));

  if (mEncrypted)
    *aDefaultPort = kDefaultWSSPort;
  else
    *aDefaultPort = kDefaultWSPort;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::GetProtocolFlags(uint32_t *aProtocolFlags) {
||||||| merged common ancestors
BaseWebSocketChannel::GetProtocolFlags(uint32_t *aProtocolFlags)
{
=======
BaseWebSocketChannel::GetProtocolFlags(uint32_t* aProtocolFlags) {
>>>>>>> upstream-releases
  LOG(("BaseWebSocketChannel::GetProtocolFlags() %p\n", this));

  *aProtocolFlags = URI_NORELATIVE | URI_NON_PERSISTABLE | ALLOWS_PROXY |
                    ALLOWS_PROXY_HTTP | URI_DOES_NOT_RETURN_DATA |
                    URI_DANGEROUS_TO_LOAD;
  if (mEncrypted) {
    *aProtocolFlags |= URI_IS_POTENTIALLY_TRUSTWORTHY;
  }
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::NewURI(const nsACString &aSpec,
                             const char *aOriginCharset, nsIURI *aBaseURI,
                             nsIURI **_retval) {
  LOG(("BaseWebSocketChannel::NewURI() %p\n", this));

  int32_t port;
  nsresult rv = GetDefaultPort(&port);
  if (NS_FAILED(rv)) return rv;

  nsCOMPtr<nsIURI> base(aBaseURI);
  return NS_MutateURI(new nsStandardURL::Mutator())
      .Apply(NS_MutatorMethod(&nsIStandardURLMutator::Init,
                              nsIStandardURL::URLTYPE_AUTHORITY, port,
                              nsCString(aSpec), aOriginCharset, base, nullptr))
      .Finalize(_retval);
}

NS_IMETHODIMP
BaseWebSocketChannel::NewChannel2(nsIURI *aURI, nsILoadInfo *aLoadInfo,
                                  nsIChannel **outChannel) {
  LOG(("BaseWebSocketChannel::NewChannel2() %p\n", this));
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
BaseWebSocketChannel::NewChannel(nsIURI *aURI, nsIChannel **_retval) {
||||||| merged common ancestors
BaseWebSocketChannel::NewURI(const nsACString & aSpec, const char *aOriginCharset,
                             nsIURI *aBaseURI, nsIURI **_retval)
{
  LOG(("BaseWebSocketChannel::NewURI() %p\n", this));

  int32_t port;
  nsresult rv = GetDefaultPort(&port);
  if (NS_FAILED(rv))
    return rv;

  nsCOMPtr<nsIURI> base(aBaseURI);
  return NS_MutateURI(new nsStandardURL::Mutator())
    .Apply(NS_MutatorMethod(&nsIStandardURLMutator::Init,
                            nsIStandardURL::URLTYPE_AUTHORITY,
                            port, nsCString(aSpec), aOriginCharset,
                            base, nullptr))
    .Finalize(_retval);
}

NS_IMETHODIMP
BaseWebSocketChannel::NewChannel2(nsIURI* aURI,
                                  nsILoadInfo* aLoadInfo,
                                  nsIChannel** outChannel)
{
  LOG(("BaseWebSocketChannel::NewChannel2() %p\n", this));
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
BaseWebSocketChannel::NewChannel(nsIURI *aURI, nsIChannel **_retval)
{
=======
BaseWebSocketChannel::NewChannel(nsIURI* aURI, nsILoadInfo* aLoadInfo,
                                 nsIChannel** outChannel) {
>>>>>>> upstream-releases
  LOG(("BaseWebSocketChannel::NewChannel() %p\n", this));
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::AllowPort(int32_t port, const char *scheme,
                                bool *_retval) {
||||||| merged common ancestors
BaseWebSocketChannel::AllowPort(int32_t port, const char *scheme,
                                bool *_retval)
{
=======
BaseWebSocketChannel::AllowPort(int32_t port, const char* scheme,
                                bool* _retval) {
>>>>>>> upstream-releases
  LOG(("BaseWebSocketChannel::AllowPort() %p\n", this));

  // do not override any blacklisted ports
  *_retval = false;
  return NS_OK;
}

//-----------------------------------------------------------------------------
// BaseWebSocketChannel::nsIThreadRetargetableRequest
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::RetargetDeliveryTo(nsIEventTarget *aTargetThread) {
||||||| merged common ancestors
BaseWebSocketChannel::RetargetDeliveryTo(nsIEventTarget* aTargetThread)
{
=======
BaseWebSocketChannel::RetargetDeliveryTo(nsIEventTarget* aTargetThread) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(aTargetThread);
  MOZ_ASSERT(!mTargetThread,
             "Delivery target should be set once, before AsyncOpen");
  MOZ_ASSERT(!mWasOpened, "Should not be called after AsyncOpen!");

  mTargetThread = aTargetThread;
  MOZ_ASSERT(mTargetThread);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BaseWebSocketChannel::GetDeliveryTarget(nsIEventTarget **aTargetThread) {
||||||| merged common ancestors
BaseWebSocketChannel::GetDeliveryTarget(nsIEventTarget** aTargetThread)
{
=======
BaseWebSocketChannel::GetDeliveryTarget(nsIEventTarget** aTargetThread) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());

  nsCOMPtr<nsIEventTarget> target = mTargetThread;
  if (!target) {
    target = GetCurrentThreadEventTarget();
  }
  target.forget(aTargetThread);
  return NS_OK;
}

BaseWebSocketChannel::ListenerAndContextContainer::ListenerAndContextContainer(
<<<<<<< HEAD
    nsIWebSocketListener *aListener, nsISupports *aContext)
    : mListener(aListener), mContext(aContext) {
||||||| merged common ancestors
                                               nsIWebSocketListener* aListener,
                                               nsISupports* aContext)
  : mListener(aListener)
  , mContext(aContext)
{
=======
    nsIWebSocketListener* aListener, nsISupports* aContext)
    : mListener(aListener), mContext(aContext) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(mListener);
}

BaseWebSocketChannel::ListenerAndContextContainer::
    ~ListenerAndContextContainer() {
  MOZ_ASSERT(mListener);

  NS_ReleaseOnMainThreadSystemGroup(
      "BaseWebSocketChannel::ListenerAndContextContainer::mListener",
      mListener.forget());
  NS_ReleaseOnMainThreadSystemGroup(
      "BaseWebSocketChannel::ListenerAndContextContainer::mContext",
      mContext.forget());
}

}  // namespace net
}  // namespace mozilla
