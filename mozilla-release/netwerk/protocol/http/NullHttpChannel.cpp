/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "NullHttpChannel.h"
#include "nsContentUtils.h"
#include "nsContentSecurityManager.h"
#include "nsIScriptSecurityManager.h"
#include "nsIStreamListener.h"

namespace mozilla {
namespace net {

NS_IMPL_ISUPPORTS(NullHttpChannel, nsINullChannel, nsIHttpChannel,
                  nsITimedChannel)

NullHttpChannel::NullHttpChannel()
    : mAllRedirectsSameOrigin(false), mAllRedirectsPassTimingAllowCheck(false) {
  mChannelCreationTime = PR_Now();
  mChannelCreationTimestamp = TimeStamp::Now();
  mAsyncOpenTime = TimeStamp::Now();
}

<<<<<<< HEAD
NullHttpChannel::NullHttpChannel(nsIHttpChannel *chan)
    : mAllRedirectsSameOrigin(false), mAllRedirectsPassTimingAllowCheck(false) {
  nsIScriptSecurityManager *ssm = nsContentUtils::GetSecurityManager();
||||||| merged common ancestors
NullHttpChannel::NullHttpChannel(nsIHttpChannel * chan)
  : mAllRedirectsSameOrigin(false)
  , mAllRedirectsPassTimingAllowCheck(false)
{
  nsIScriptSecurityManager* ssm = nsContentUtils::GetSecurityManager();
=======
NullHttpChannel::NullHttpChannel(nsIHttpChannel* chan)
    : mAllRedirectsSameOrigin(false), mAllRedirectsPassTimingAllowCheck(false) {
  nsIScriptSecurityManager* ssm = nsContentUtils::GetSecurityManager();
>>>>>>> upstream-releases
  ssm->GetChannelURIPrincipal(chan, getter_AddRefs(mResourcePrincipal));

  Unused << chan->GetResponseHeader(NS_LITERAL_CSTRING("Timing-Allow-Origin"),
                                    mTimingAllowOriginHeader);
  chan->GetURI(getter_AddRefs(mURI));
  chan->GetOriginalURI(getter_AddRefs(mOriginalURI));

  mChannelCreationTime = PR_Now();
  mChannelCreationTimestamp = TimeStamp::Now();
  mAsyncOpenTime = TimeStamp::Now();

  nsCOMPtr<nsITimedChannel> timedChanel(do_QueryInterface(chan));
  if (timedChanel) {
    timedChanel->GetInitiatorType(mInitiatorType);
  }
}

<<<<<<< HEAD
nsresult NullHttpChannel::Init(nsIURI *aURI, uint32_t aCaps,
                               nsProxyInfo *aProxyInfo,
                               uint32_t aProxyResolveFlags, nsIURI *aProxyURI) {
||||||| merged common ancestors
nsresult
NullHttpChannel::Init(nsIURI *aURI,
                      uint32_t aCaps,
                      nsProxyInfo *aProxyInfo,
                      uint32_t aProxyResolveFlags,
                      nsIURI *aProxyURI)
{
=======
nsresult NullHttpChannel::Init(nsIURI* aURI, uint32_t aCaps,
                               nsProxyInfo* aProxyInfo,
                               uint32_t aProxyResolveFlags, nsIURI* aProxyURI) {
>>>>>>> upstream-releases
  mURI = aURI;
  mOriginalURI = aURI;
  return NS_OK;
}

//-----------------------------------------------------------------------------
// NullHttpChannel::nsIHttpChannel
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetChannelId(uint64_t *aChannelId) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
NullHttpChannel::GetChannelId(uint64_t *aChannelId)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
NullHttpChannel::GetChannelId(uint64_t* aChannelId) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
NullHttpChannel::SetChannelId(uint64_t aChannelId) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetTopLevelContentWindowId(uint64_t *aWindowId) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
NullHttpChannel::GetTopLevelContentWindowId(uint64_t *aWindowId)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
NullHttpChannel::GetTopLevelContentWindowId(uint64_t* aWindowId) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
NullHttpChannel::SetTopLevelContentWindowId(uint64_t aWindowId) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetTopLevelOuterContentWindowId(uint64_t *aWindowId) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
NullHttpChannel::GetTopLevelOuterContentWindowId(uint64_t *aWindowId)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
NullHttpChannel::GetTopLevelOuterContentWindowId(uint64_t* aWindowId) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
NullHttpChannel::SetTopLevelOuterContentWindowId(uint64_t aWindowId) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetIsTrackingResource(bool *aIsTrackingResource) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
NullHttpChannel::GetIsTrackingResource(bool* aIsTrackingResource)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
NullHttpChannel::IsTrackingResource(bool* aIsTrackingResource) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetIsThirdPartyTrackingResource(bool *aIsTrackingResource) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
NullHttpChannel::GetIsThirdPartyTrackingResource(bool* aIsTrackingResource)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
NullHttpChannel::IsThirdPartyTrackingResource(bool* aIsTrackingResource) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::OverrideTrackingFlagsForDocumentCookieAccessor(
    nsIHttpChannel *aDocumentChannel) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
NullHttpChannel::OverrideTrackingFlagsForDocumentCookieAccessor(nsIHttpChannel* aDocumentChannel)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
NullHttpChannel::GetClassificationFlags(uint32_t* aClassificationFlags) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetTransferSize(uint64_t *aTransferSize) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
NullHttpChannel::GetTransferSize(uint64_t *aTransferSize)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
NullHttpChannel::GetFlashPluginState(
    nsIHttpChannel::FlashPluginState* aResult) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetDecodedBodySize(uint64_t *aDecodedBodySize) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
NullHttpChannel::GetDecodedBodySize(uint64_t *aDecodedBodySize)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
NullHttpChannel::GetFirstPartyClassificationFlags(
    uint32_t* aClassificationFlags) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetRequestMethod(nsACString &aRequestMethod) {
||||||| merged common ancestors
NullHttpChannel::GetRequestMethod(nsACString & aRequestMethod)
{
=======
NullHttpChannel::GetThirdPartyClassificationFlags(
    uint32_t* aClassificationFlags) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetRequestMethod(const nsACString &aRequestMethod) {
||||||| merged common ancestors
NullHttpChannel::SetRequestMethod(const nsACString & aRequestMethod)
{
=======
NullHttpChannel::GetTransferSize(uint64_t* aTransferSize) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetReferrer(nsIURI **aReferrer) {
||||||| merged common ancestors
NullHttpChannel::GetReferrer(nsIURI * *aReferrer)
{
=======
NullHttpChannel::GetDecodedBodySize(uint64_t* aDecodedBodySize) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetReferrer(nsIURI *aReferrer) {
||||||| merged common ancestors
NullHttpChannel::SetReferrer(nsIURI *aReferrer)
{
=======
NullHttpChannel::GetRequestMethod(nsACString& aRequestMethod) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetReferrerPolicy(uint32_t *aReferrerPolicy) {
||||||| merged common ancestors
NullHttpChannel::GetReferrerPolicy(uint32_t *aReferrerPolicy)
{
=======
NullHttpChannel::SetRequestMethod(const nsACString& aRequestMethod) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetReferrerWithPolicy(nsIURI *referrer,
                                       uint32_t referrerPolicy) {
||||||| merged common ancestors
NullHttpChannel::SetReferrerWithPolicy(nsIURI *referrer, uint32_t referrerPolicy)
{
=======
NullHttpChannel::GetReferrerInfo(nsIReferrerInfo** aReferrerInfo) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetRequestHeader(const nsACString &aHeader,
                                  nsACString &_retval) {
||||||| merged common ancestors
NullHttpChannel::GetRequestHeader(const nsACString & aHeader, nsACString & _retval)
{
=======
NullHttpChannel::SetReferrerInfo(nsIReferrerInfo* aReferrerInfo) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::SetReferrerInfoWithoutClone(nsIReferrerInfo* aReferrerInfo) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::GetRequestHeader(const nsACString& aHeader,
                                  nsACString& _retval) {
>>>>>>> upstream-releases
  _retval.Truncate();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetRequestHeader(const nsACString &aHeader,
                                  const nsACString &aValue, bool aMerge) {
||||||| merged common ancestors
NullHttpChannel::SetRequestHeader(const nsACString & aHeader, const nsACString & aValue, bool aMerge)
{
=======
NullHttpChannel::SetRequestHeader(const nsACString& aHeader,
                                  const nsACString& aValue, bool aMerge) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetEmptyRequestHeader(const nsACString &aHeader) {
||||||| merged common ancestors
NullHttpChannel::SetEmptyRequestHeader(const nsACString & aHeader)
{
=======
NullHttpChannel::SetEmptyRequestHeader(const nsACString& aHeader) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::VisitRequestHeaders(nsIHttpHeaderVisitor *aVisitor) {
||||||| merged common ancestors
NullHttpChannel::VisitRequestHeaders(nsIHttpHeaderVisitor *aVisitor)
{
=======
NullHttpChannel::VisitRequestHeaders(nsIHttpHeaderVisitor* aVisitor) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::VisitNonDefaultRequestHeaders(nsIHttpHeaderVisitor *aVisitor) {
||||||| merged common ancestors
NullHttpChannel::VisitNonDefaultRequestHeaders(nsIHttpHeaderVisitor *aVisitor)
{
=======
NullHttpChannel::VisitNonDefaultRequestHeaders(nsIHttpHeaderVisitor* aVisitor) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetAllowPipelining(bool *aAllowPipelining) {
||||||| merged common ancestors
NullHttpChannel::GetAllowPipelining(bool *aAllowPipelining)
{
=======
NullHttpChannel::GetAllowPipelining(bool* aAllowPipelining) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::SetAllowPipelining(bool aAllowPipelining) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetAllowSTS(bool *aAllowSTS) {
||||||| merged common ancestors
NullHttpChannel::GetAllowSTS(bool *aAllowSTS)
{
=======
NullHttpChannel::GetAllowSTS(bool* aAllowSTS) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::SetAllowSTS(bool aAllowSTS) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetRedirectionLimit(uint32_t *aRedirectionLimit) {
||||||| merged common ancestors
NullHttpChannel::GetRedirectionLimit(uint32_t *aRedirectionLimit)
{
=======
NullHttpChannel::GetRedirectionLimit(uint32_t* aRedirectionLimit) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::SetRedirectionLimit(uint32_t aRedirectionLimit) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetResponseStatus(uint32_t *aResponseStatus) {
||||||| merged common ancestors
NullHttpChannel::GetResponseStatus(uint32_t *aResponseStatus)
{
=======
NullHttpChannel::GetResponseStatus(uint32_t* aResponseStatus) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetResponseStatusText(nsACString &aResponseStatusText) {
||||||| merged common ancestors
NullHttpChannel::GetResponseStatusText(nsACString & aResponseStatusText)
{
=======
NullHttpChannel::GetResponseStatusText(nsACString& aResponseStatusText) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetRequestSucceeded(bool *aRequestSucceeded) {
||||||| merged common ancestors
NullHttpChannel::GetRequestSucceeded(bool *aRequestSucceeded)
{
=======
NullHttpChannel::GetRequestSucceeded(bool* aRequestSucceeded) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetResponseHeader(const nsACString &header,
                                   nsACString &_retval) {
||||||| merged common ancestors
NullHttpChannel::GetResponseHeader(const nsACString & header, nsACString & _retval)
{
=======
NullHttpChannel::GetResponseHeader(const nsACString& header,
                                   nsACString& _retval) {
>>>>>>> upstream-releases
  _retval.Truncate();
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetResponseHeader(const nsACString &header,
                                   const nsACString &value, bool merge) {
||||||| merged common ancestors
NullHttpChannel::SetResponseHeader(const nsACString & header, const nsACString & value, bool merge)
{
=======
NullHttpChannel::SetResponseHeader(const nsACString& header,
                                   const nsACString& value, bool merge) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::VisitResponseHeaders(nsIHttpHeaderVisitor *aVisitor) {
||||||| merged common ancestors
NullHttpChannel::VisitResponseHeaders(nsIHttpHeaderVisitor *aVisitor)
{
=======
NullHttpChannel::VisitResponseHeaders(nsIHttpHeaderVisitor* aVisitor) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetOriginalResponseHeader(const nsACString &header,
                                           nsIHttpHeaderVisitor *aVisitor) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
NullHttpChannel::GetOriginalResponseHeader(const nsACString & header,
                                            nsIHttpHeaderVisitor *aVisitor)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
NullHttpChannel::GetOriginalResponseHeader(const nsACString& header,
                                           nsIHttpHeaderVisitor* aVisitor) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::VisitOriginalResponseHeaders(nsIHttpHeaderVisitor *aVisitor) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
NullHttpChannel::VisitOriginalResponseHeaders(nsIHttpHeaderVisitor *aVisitor)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
NullHttpChannel::VisitOriginalResponseHeaders(nsIHttpHeaderVisitor* aVisitor) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::IsNoStoreResponse(bool *_retval) {
||||||| merged common ancestors
NullHttpChannel::IsNoStoreResponse(bool *_retval)
{
=======
NullHttpChannel::IsNoStoreResponse(bool* _retval) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::IsNoCacheResponse(bool *_retval) {
||||||| merged common ancestors
NullHttpChannel::IsNoCacheResponse(bool *_retval)
{
=======
NullHttpChannel::IsNoCacheResponse(bool* _retval) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::IsPrivateResponse(bool *_retval) {
||||||| merged common ancestors
NullHttpChannel::IsPrivateResponse(bool *_retval)
{
=======
NullHttpChannel::IsPrivateResponse(bool* _retval) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::RedirectTo(nsIURI *aNewURI) {
||||||| merged common ancestors
NullHttpChannel::RedirectTo(nsIURI *aNewURI)
{
=======
NullHttpChannel::RedirectTo(nsIURI* aNewURI) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::UpgradeToSecure() { return NS_ERROR_NOT_IMPLEMENTED; }
||||||| merged common ancestors
NullHttpChannel::UpgradeToSecure()
{
  return NS_ERROR_NOT_IMPLEMENTED;
}
=======
NullHttpChannel::SwitchProcessTo(mozilla::dom::Promise* aBrowserParent,
                                 uint64_t aIdentifier) {
  return NS_ERROR_NOT_AVAILABLE;
}

NS_IMETHODIMP
NullHttpChannel::HasCrossOriginOpenerPolicyMismatch(bool* aMismatch) {
  return NS_ERROR_NOT_AVAILABLE;
}
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetRequestContextID(uint64_t *_retval) {
||||||| merged common ancestors
NullHttpChannel::GetRequestContextID(uint64_t *_retval)
{
=======
NullHttpChannel::UpgradeToSecure() { return NS_ERROR_NOT_IMPLEMENTED; }

NS_IMETHODIMP
NullHttpChannel::GetRequestContextID(uint64_t* _retval) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::SetRequestContextID(uint64_t rcID) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetProtocolVersion(nsACString &aProtocolVersion) {
||||||| merged common ancestors
NullHttpChannel::GetProtocolVersion(nsACString& aProtocolVersion)
{
=======
NullHttpChannel::GetProtocolVersion(nsACString& aProtocolVersion) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetEncodedBodySize(uint64_t *aEncodedBodySize) {
||||||| merged common ancestors
NullHttpChannel::GetEncodedBodySize(uint64_t *aEncodedBodySize)
{
=======
NullHttpChannel::GetEncodedBodySize(uint64_t* aEncodedBodySize) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
// NullHttpChannel::nsIChannel
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetOriginalURI(nsIURI **aOriginalURI) {
||||||| merged common ancestors
NullHttpChannel::GetOriginalURI(nsIURI * *aOriginalURI)
{
=======
NullHttpChannel::GetOriginalURI(nsIURI** aOriginalURI) {
>>>>>>> upstream-releases
  NS_IF_ADDREF(*aOriginalURI = mOriginalURI);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetOriginalURI(nsIURI *aOriginalURI) {
||||||| merged common ancestors
NullHttpChannel::SetOriginalURI(nsIURI *aOriginalURI)
{
=======
NullHttpChannel::SetOriginalURI(nsIURI* aOriginalURI) {
>>>>>>> upstream-releases
  mOriginalURI = aOriginalURI;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetURI(nsIURI **aURI) {
||||||| merged common ancestors
NullHttpChannel::GetURI(nsIURI * *aURI)
{
=======
NullHttpChannel::GetURI(nsIURI** aURI) {
>>>>>>> upstream-releases
  NS_IF_ADDREF(*aURI = mURI);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetOwner(nsISupports **aOwner) {
||||||| merged common ancestors
NullHttpChannel::GetOwner(nsISupports * *aOwner)
{
=======
NullHttpChannel::GetOwner(nsISupports** aOwner) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetOwner(nsISupports *aOwner) {
||||||| merged common ancestors
NullHttpChannel::SetOwner(nsISupports *aOwner)
{
=======
NullHttpChannel::SetOwner(nsISupports* aOwner) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetNotificationCallbacks(
    nsIInterfaceRequestor **aNotificationCallbacks) {
||||||| merged common ancestors
NullHttpChannel::GetNotificationCallbacks(nsIInterfaceRequestor * *aNotificationCallbacks)
{
=======
NullHttpChannel::GetNotificationCallbacks(
    nsIInterfaceRequestor** aNotificationCallbacks) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetNotificationCallbacks(
    nsIInterfaceRequestor *aNotificationCallbacks) {
||||||| merged common ancestors
NullHttpChannel::SetNotificationCallbacks(nsIInterfaceRequestor *aNotificationCallbacks)
{
=======
NullHttpChannel::SetNotificationCallbacks(
    nsIInterfaceRequestor* aNotificationCallbacks) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetSecurityInfo(nsISupports **aSecurityInfo) {
||||||| merged common ancestors
NullHttpChannel::GetSecurityInfo(nsISupports * *aSecurityInfo)
{
=======
NullHttpChannel::GetSecurityInfo(nsISupports** aSecurityInfo) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetContentType(nsACString &aContentType) {
||||||| merged common ancestors
NullHttpChannel::GetContentType(nsACString & aContentType)
{
=======
NullHttpChannel::GetContentType(nsACString& aContentType) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetContentType(const nsACString &aContentType) {
||||||| merged common ancestors
NullHttpChannel::SetContentType(const nsACString & aContentType)
{
=======
NullHttpChannel::SetContentType(const nsACString& aContentType) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetContentCharset(nsACString &aContentCharset) {
||||||| merged common ancestors
NullHttpChannel::GetContentCharset(nsACString & aContentCharset)
{
=======
NullHttpChannel::GetContentCharset(nsACString& aContentCharset) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetContentCharset(const nsACString &aContentCharset) {
||||||| merged common ancestors
NullHttpChannel::SetContentCharset(const nsACString & aContentCharset)
{
=======
NullHttpChannel::SetContentCharset(const nsACString& aContentCharset) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetContentLength(int64_t *aContentLength) {
||||||| merged common ancestors
NullHttpChannel::GetContentLength(int64_t *aContentLength)
{
=======
NullHttpChannel::GetContentLength(int64_t* aContentLength) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::SetContentLength(int64_t aContentLength) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::Open(nsIInputStream **_retval) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::Open2(nsIInputStream **aStream) {
||||||| merged common ancestors
NullHttpChannel::Open(nsIInputStream * *_retval)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::Open2(nsIInputStream** aStream)
{
=======
NullHttpChannel::Open(nsIInputStream** aStream) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIStreamListener> listener;
  nsresult rv =
      nsContentSecurityManager::doContentSecurityCheck(this, listener);
  NS_ENSURE_SUCCESS(rv, rv);

<<<<<<< HEAD
NS_IMETHODIMP
NullHttpChannel::AsyncOpen(nsIStreamListener *aListener,
                           nsISupports *aContext) {
||||||| merged common ancestors
NS_IMETHODIMP
NullHttpChannel::AsyncOpen(nsIStreamListener *aListener, nsISupports *aContext)
{
=======
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::AsyncOpen2(nsIStreamListener *aListener) {
||||||| merged common ancestors
NullHttpChannel::AsyncOpen2(nsIStreamListener *aListener)
{
=======
NullHttpChannel::AsyncOpen(nsIStreamListener* aListener) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIStreamListener> listener = aListener;
  nsresult rv =
      nsContentSecurityManager::doContentSecurityCheck(this, listener);
  NS_ENSURE_SUCCESS(rv, rv);
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetContentDisposition(uint32_t *aContentDisposition) {
||||||| merged common ancestors
NullHttpChannel::GetContentDisposition(uint32_t *aContentDisposition)
{
=======
NullHttpChannel::GetContentDisposition(uint32_t* aContentDisposition) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::SetContentDisposition(uint32_t aContentDisposition) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetContentDispositionFilename(
    nsAString &aContentDispositionFilename) {
||||||| merged common ancestors
NullHttpChannel::GetContentDispositionFilename(nsAString & aContentDispositionFilename)
{
=======
NullHttpChannel::GetContentDispositionFilename(
    nsAString& aContentDispositionFilename) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetContentDispositionFilename(
    const nsAString &aContentDispositionFilename) {
||||||| merged common ancestors
NullHttpChannel::SetContentDispositionFilename(const nsAString & aContentDispositionFilename)
{
=======
NullHttpChannel::SetContentDispositionFilename(
    const nsAString& aContentDispositionFilename) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetContentDispositionHeader(
    nsACString &aContentDispositionHeader) {
||||||| merged common ancestors
NullHttpChannel::GetContentDispositionHeader(nsACString & aContentDispositionHeader)
{
=======
NullHttpChannel::GetContentDispositionHeader(
    nsACString& aContentDispositionHeader) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetLoadInfo(nsILoadInfo **aLoadInfo) {
||||||| merged common ancestors
NullHttpChannel::GetLoadInfo(nsILoadInfo * *aLoadInfo)
{
=======
NullHttpChannel::GetLoadInfo(nsILoadInfo** aLoadInfo) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetLoadInfo(nsILoadInfo *aLoadInfo) {
||||||| merged common ancestors
NullHttpChannel::SetLoadInfo(nsILoadInfo *aLoadInfo)
{
=======
NullHttpChannel::SetLoadInfo(nsILoadInfo* aLoadInfo) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
// NullHttpChannel::nsIRequest
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetName(nsACString &aName) { return NS_ERROR_NOT_IMPLEMENTED; }
||||||| merged common ancestors
NullHttpChannel::GetName(nsACString & aName)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}
=======
NullHttpChannel::GetName(nsACString& aName) { return NS_ERROR_NOT_IMPLEMENTED; }
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::IsPending(bool *_retval) { return NS_ERROR_NOT_IMPLEMENTED; }
||||||| merged common ancestors
NullHttpChannel::IsPending(bool *_retval)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}
=======
NullHttpChannel::IsPending(bool* _retval) { return NS_ERROR_NOT_IMPLEMENTED; }
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetStatus(nsresult *aStatus) {
||||||| merged common ancestors
NullHttpChannel::GetStatus(nsresult *aStatus)
{
=======
NullHttpChannel::GetStatus(nsresult* aStatus) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::Cancel(nsresult aStatus) { return NS_ERROR_NOT_IMPLEMENTED; }

NS_IMETHODIMP
NullHttpChannel::Suspend() { return NS_ERROR_NOT_IMPLEMENTED; }

NS_IMETHODIMP
NullHttpChannel::Resume() { return NS_ERROR_NOT_IMPLEMENTED; }

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetLoadGroup(nsILoadGroup **aLoadGroup) {
||||||| merged common ancestors
NullHttpChannel::GetLoadGroup(nsILoadGroup * *aLoadGroup)
{
=======
NullHttpChannel::GetLoadGroup(nsILoadGroup** aLoadGroup) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetLoadGroup(nsILoadGroup *aLoadGroup) {
||||||| merged common ancestors
NullHttpChannel::SetLoadGroup(nsILoadGroup *aLoadGroup)
{
=======
NullHttpChannel::SetLoadGroup(nsILoadGroup* aLoadGroup) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetLoadFlags(nsLoadFlags *aLoadFlags) {
||||||| merged common ancestors
NullHttpChannel::GetLoadFlags(nsLoadFlags *aLoadFlags)
{
=======
NullHttpChannel::GetLoadFlags(nsLoadFlags* aLoadFlags) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::SetLoadFlags(nsLoadFlags aLoadFlags) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetIsDocument(bool *aIsDocument) {
||||||| merged common ancestors
NullHttpChannel::GetIsDocument(bool *aIsDocument)
{
=======
NullHttpChannel::GetIsDocument(bool* aIsDocument) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
// NullHttpChannel::nsITimedChannel
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetTimingEnabled(bool *aTimingEnabled) {
||||||| merged common ancestors
NullHttpChannel::GetTimingEnabled(bool *aTimingEnabled)
{
=======
NullHttpChannel::GetTimingEnabled(bool* aTimingEnabled) {
>>>>>>> upstream-releases
  // We don't want to report timing for null channels.
  *aTimingEnabled = false;
  return NS_OK;
}

NS_IMETHODIMP
NullHttpChannel::SetTimingEnabled(bool aTimingEnabled) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetRedirectCount(uint8_t *aRedirectCount) {
||||||| merged common ancestors
NullHttpChannel::GetRedirectCount(uint8_t *aRedirectCount)
{
=======
NullHttpChannel::GetRedirectCount(uint8_t* aRedirectCount) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::SetRedirectCount(uint8_t aRedirectCount) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetInternalRedirectCount(uint8_t *aRedirectCount) {
||||||| merged common ancestors
NullHttpChannel::GetInternalRedirectCount(uint8_t *aRedirectCount)
{
=======
NullHttpChannel::GetInternalRedirectCount(uint8_t* aRedirectCount) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::SetInternalRedirectCount(uint8_t aRedirectCount) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetChannelCreation(mozilla::TimeStamp *aChannelCreation) {
||||||| merged common ancestors
NullHttpChannel::GetChannelCreation(mozilla::TimeStamp *aChannelCreation)
{
=======
NullHttpChannel::GetChannelCreation(mozilla::TimeStamp* aChannelCreation) {
>>>>>>> upstream-releases
  *aChannelCreation = mChannelCreationTimestamp;
  return NS_OK;
}

NS_IMETHODIMP
NullHttpChannel::SetChannelCreation(TimeStamp aValue) {
  MOZ_DIAGNOSTIC_ASSERT(!aValue.IsNull());
  TimeDuration adjust = aValue - mChannelCreationTimestamp;
  mChannelCreationTimestamp = aValue;
  mChannelCreationTime += (PRTime)adjust.ToMicroseconds();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetAsyncOpen(mozilla::TimeStamp *aAsyncOpen) {
||||||| merged common ancestors
NullHttpChannel::GetAsyncOpen(mozilla::TimeStamp *aAsyncOpen)
{
=======
NullHttpChannel::GetAsyncOpen(mozilla::TimeStamp* aAsyncOpen) {
>>>>>>> upstream-releases
  *aAsyncOpen = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
NullHttpChannel::SetAsyncOpen(TimeStamp aValue) {
  MOZ_DIAGNOSTIC_ASSERT(!aValue.IsNull());
  mAsyncOpenTime = aValue;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetLaunchServiceWorkerStart(mozilla::TimeStamp *_retval) {
||||||| merged common ancestors
NullHttpChannel::GetLaunchServiceWorkerStart(mozilla::TimeStamp *_retval)
{
=======
NullHttpChannel::GetLaunchServiceWorkerStart(mozilla::TimeStamp* _retval) {
>>>>>>> upstream-releases
  MOZ_ASSERT(_retval);
  *_retval = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
NullHttpChannel::SetLaunchServiceWorkerStart(mozilla::TimeStamp aTimeStamp) {
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetLaunchServiceWorkerEnd(mozilla::TimeStamp *_retval) {
||||||| merged common ancestors
NullHttpChannel::GetLaunchServiceWorkerEnd(mozilla::TimeStamp *_retval)
{
=======
NullHttpChannel::GetLaunchServiceWorkerEnd(mozilla::TimeStamp* _retval) {
>>>>>>> upstream-releases
  MOZ_ASSERT(_retval);
  *_retval = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
NullHttpChannel::SetLaunchServiceWorkerEnd(mozilla::TimeStamp aTimeStamp) {
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetDispatchFetchEventStart(mozilla::TimeStamp *_retval) {
||||||| merged common ancestors
NullHttpChannel::GetDispatchFetchEventStart(mozilla::TimeStamp *_retval)
{
=======
NullHttpChannel::GetDispatchFetchEventStart(mozilla::TimeStamp* _retval) {
>>>>>>> upstream-releases
  MOZ_ASSERT(_retval);
  *_retval = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
NullHttpChannel::SetDispatchFetchEventStart(mozilla::TimeStamp aTimeStamp) {
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetDispatchFetchEventEnd(mozilla::TimeStamp *_retval) {
||||||| merged common ancestors
NullHttpChannel::GetDispatchFetchEventEnd(mozilla::TimeStamp *_retval)
{
=======
NullHttpChannel::GetDispatchFetchEventEnd(mozilla::TimeStamp* _retval) {
>>>>>>> upstream-releases
  MOZ_ASSERT(_retval);
  *_retval = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
NullHttpChannel::SetDispatchFetchEventEnd(mozilla::TimeStamp aTimeStamp) {
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetHandleFetchEventStart(mozilla::TimeStamp *_retval) {
||||||| merged common ancestors
NullHttpChannel::GetHandleFetchEventStart(mozilla::TimeStamp *_retval)
{
=======
NullHttpChannel::GetHandleFetchEventStart(mozilla::TimeStamp* _retval) {
>>>>>>> upstream-releases
  MOZ_ASSERT(_retval);
  *_retval = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
NullHttpChannel::SetHandleFetchEventStart(mozilla::TimeStamp aTimeStamp) {
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetHandleFetchEventEnd(mozilla::TimeStamp *_retval) {
||||||| merged common ancestors
NullHttpChannel::GetHandleFetchEventEnd(mozilla::TimeStamp *_retval)
{
=======
NullHttpChannel::GetHandleFetchEventEnd(mozilla::TimeStamp* _retval) {
>>>>>>> upstream-releases
  MOZ_ASSERT(_retval);
  *_retval = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
NullHttpChannel::SetHandleFetchEventEnd(mozilla::TimeStamp aTimeStamp) {
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetDomainLookupStart(mozilla::TimeStamp *aDomainLookupStart) {
||||||| merged common ancestors
NullHttpChannel::GetDomainLookupStart(mozilla::TimeStamp *aDomainLookupStart)
{
=======
NullHttpChannel::GetDomainLookupStart(mozilla::TimeStamp* aDomainLookupStart) {
>>>>>>> upstream-releases
  *aDomainLookupStart = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetDomainLookupEnd(mozilla::TimeStamp *aDomainLookupEnd) {
||||||| merged common ancestors
NullHttpChannel::GetDomainLookupEnd(mozilla::TimeStamp *aDomainLookupEnd)
{
=======
NullHttpChannel::GetDomainLookupEnd(mozilla::TimeStamp* aDomainLookupEnd) {
>>>>>>> upstream-releases
  *aDomainLookupEnd = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetConnectStart(mozilla::TimeStamp *aConnectStart) {
||||||| merged common ancestors
NullHttpChannel::GetConnectStart(mozilla::TimeStamp *aConnectStart)
{
=======
NullHttpChannel::GetConnectStart(mozilla::TimeStamp* aConnectStart) {
>>>>>>> upstream-releases
  *aConnectStart = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetTcpConnectEnd(mozilla::TimeStamp *aTcpConnectEnd) {
||||||| merged common ancestors
NullHttpChannel::GetTcpConnectEnd(mozilla::TimeStamp *aTcpConnectEnd)
{
=======
NullHttpChannel::GetTcpConnectEnd(mozilla::TimeStamp* aTcpConnectEnd) {
>>>>>>> upstream-releases
  *aTcpConnectEnd = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetSecureConnectionStart(
    mozilla::TimeStamp *aSecureConnectionStart) {
||||||| merged common ancestors
NullHttpChannel::GetSecureConnectionStart(mozilla::TimeStamp *aSecureConnectionStart)
{
=======
NullHttpChannel::GetSecureConnectionStart(
    mozilla::TimeStamp* aSecureConnectionStart) {
>>>>>>> upstream-releases
  *aSecureConnectionStart = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetConnectEnd(mozilla::TimeStamp *aConnectEnd) {
||||||| merged common ancestors
NullHttpChannel::GetConnectEnd(mozilla::TimeStamp *aConnectEnd)
{
=======
NullHttpChannel::GetConnectEnd(mozilla::TimeStamp* aConnectEnd) {
>>>>>>> upstream-releases
  *aConnectEnd = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetRequestStart(mozilla::TimeStamp *aRequestStart) {
||||||| merged common ancestors
NullHttpChannel::GetRequestStart(mozilla::TimeStamp *aRequestStart)
{
=======
NullHttpChannel::GetRequestStart(mozilla::TimeStamp* aRequestStart) {
>>>>>>> upstream-releases
  *aRequestStart = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetResponseStart(mozilla::TimeStamp *aResponseStart) {
||||||| merged common ancestors
NullHttpChannel::GetResponseStart(mozilla::TimeStamp *aResponseStart)
{
=======
NullHttpChannel::GetResponseStart(mozilla::TimeStamp* aResponseStart) {
>>>>>>> upstream-releases
  *aResponseStart = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetResponseEnd(mozilla::TimeStamp *aResponseEnd) {
||||||| merged common ancestors
NullHttpChannel::GetResponseEnd(mozilla::TimeStamp *aResponseEnd)
{
=======
NullHttpChannel::GetResponseEnd(mozilla::TimeStamp* aResponseEnd) {
>>>>>>> upstream-releases
  *aResponseEnd = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetRedirectStart(mozilla::TimeStamp *aRedirectStart) {
||||||| merged common ancestors
NullHttpChannel::GetRedirectStart(mozilla::TimeStamp *aRedirectStart)
{
=======
NullHttpChannel::GetRedirectStart(mozilla::TimeStamp* aRedirectStart) {
>>>>>>> upstream-releases
  *aRedirectStart = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
NullHttpChannel::SetRedirectStart(mozilla::TimeStamp aRedirectStart) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetRedirectEnd(mozilla::TimeStamp *aRedirectEnd) {
||||||| merged common ancestors
NullHttpChannel::GetRedirectEnd(mozilla::TimeStamp *aRedirectEnd)
{
=======
NullHttpChannel::GetRedirectEnd(mozilla::TimeStamp* aRedirectEnd) {
>>>>>>> upstream-releases
  *aRedirectEnd = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
NullHttpChannel::SetRedirectEnd(mozilla::TimeStamp aRedirectEnd) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetInitiatorType(nsAString &aInitiatorType) {
||||||| merged common ancestors
NullHttpChannel::GetInitiatorType(nsAString & aInitiatorType)
{
=======
NullHttpChannel::GetInitiatorType(nsAString& aInitiatorType) {
>>>>>>> upstream-releases
  aInitiatorType = mInitiatorType;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::SetInitiatorType(const nsAString &aInitiatorType) {
||||||| merged common ancestors
NullHttpChannel::SetInitiatorType(const nsAString & aInitiatorType)
{
=======
NullHttpChannel::SetInitiatorType(const nsAString& aInitiatorType) {
>>>>>>> upstream-releases
  mInitiatorType = aInitiatorType;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetAllRedirectsSameOrigin(bool *aAllRedirectsSameOrigin) {
||||||| merged common ancestors
NullHttpChannel::GetAllRedirectsSameOrigin(bool *aAllRedirectsSameOrigin)
{
=======
NullHttpChannel::GetAllRedirectsSameOrigin(bool* aAllRedirectsSameOrigin) {
>>>>>>> upstream-releases
  *aAllRedirectsSameOrigin = mAllRedirectsSameOrigin;
  return NS_OK;
}

NS_IMETHODIMP
NullHttpChannel::SetAllRedirectsSameOrigin(bool aAllRedirectsSameOrigin) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetAllRedirectsPassTimingAllowCheck(
    bool *aAllRedirectsPassTimingAllowCheck) {
||||||| merged common ancestors
NullHttpChannel::GetAllRedirectsPassTimingAllowCheck(bool *aAllRedirectsPassTimingAllowCheck)
{
=======
NullHttpChannel::GetAllRedirectsPassTimingAllowCheck(
    bool* aAllRedirectsPassTimingAllowCheck) {
>>>>>>> upstream-releases
  *aAllRedirectsPassTimingAllowCheck = mAllRedirectsPassTimingAllowCheck;
  return NS_OK;
}

NS_IMETHODIMP
NullHttpChannel::SetAllRedirectsPassTimingAllowCheck(
    bool aAllRedirectsPassTimingAllowCheck) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::TimingAllowCheck(nsIPrincipal *aOrigin, bool *_retval) {
||||||| merged common ancestors
NullHttpChannel::TimingAllowCheck(nsIPrincipal *aOrigin, bool *_retval)
{
=======
NullHttpChannel::TimingAllowCheck(nsIPrincipal* aOrigin, bool* _retval) {
>>>>>>> upstream-releases
  if (!mResourcePrincipal || !aOrigin) {
    *_retval = false;
    return NS_OK;
  }

  bool sameOrigin = false;
  nsresult rv = mResourcePrincipal->Equals(aOrigin, &sameOrigin);
  if (NS_SUCCEEDED(rv) && sameOrigin) {
    *_retval = true;
    return NS_OK;
  }

  if (mTimingAllowOriginHeader == "*") {
    *_retval = true;
    return NS_OK;
  }

  nsAutoCString origin;
  nsContentUtils::GetASCIIOrigin(aOrigin, origin);

  if (mTimingAllowOriginHeader == origin) {
    *_retval = true;
    return NS_OK;
  }

  *_retval = false;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetCacheReadStart(mozilla::TimeStamp *aCacheReadStart) {
||||||| merged common ancestors
NullHttpChannel::GetCacheReadStart(mozilla::TimeStamp *aCacheReadStart)
{
=======
NullHttpChannel::GetCacheReadStart(mozilla::TimeStamp* aCacheReadStart) {
>>>>>>> upstream-releases
  *aCacheReadStart = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetCacheReadEnd(mozilla::TimeStamp *aCacheReadEnd) {
||||||| merged common ancestors
NullHttpChannel::GetCacheReadEnd(mozilla::TimeStamp *aCacheReadEnd)
{
=======
NullHttpChannel::GetCacheReadEnd(mozilla::TimeStamp* aCacheReadEnd) {
>>>>>>> upstream-releases
  *aCacheReadEnd = mAsyncOpenTime;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetIsMainDocumentChannel(bool *aValue) {
||||||| merged common ancestors
NullHttpChannel::GetIsMainDocumentChannel(bool* aValue)
{
=======
NullHttpChannel::GetIsMainDocumentChannel(bool* aValue) {
>>>>>>> upstream-releases
  *aValue = false;
  return NS_OK;
}

NS_IMETHODIMP
NullHttpChannel::SetIsMainDocumentChannel(bool aValue) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::LogBlockedCORSRequest(const nsAString &aMessage,
                                       const nsACString &aCategory) {
||||||| merged common ancestors
NullHttpChannel::LogBlockedCORSRequest(const nsAString& aMessage,
                                       const nsACString& aCategory)
{
=======
NullHttpChannel::LogBlockedCORSRequest(const nsAString& aMessage,
                                       const nsACString& aCategory) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::LogMimeTypeMismatch(const nsACString& aMessageName,
                                     bool aWarning, const nsAString& aURL,
                                     const nsAString& aContentType) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::SetReportResourceTiming(bool enabled) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
NullHttpChannel::GetReportResourceTiming(bool *_retval) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetServerTiming(nsIArray **aServerTiming) {
||||||| merged common ancestors
NullHttpChannel::GetServerTiming(nsIArray **aServerTiming)
{
=======
NullHttpChannel::GetServerTiming(nsIArray** aServerTiming) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
NullHttpChannel::GetNativeServerTiming(
    nsTArray<nsCOMPtr<nsIServerTiming>> &aServerTiming) {
||||||| merged common ancestors
NullHttpChannel::GetNativeServerTiming(nsTArray<nsCOMPtr<nsIServerTiming>>& aServerTiming)
{
=======
NullHttpChannel::GetNativeServerTiming(
    nsTArray<nsCOMPtr<nsIServerTiming>>& aServerTiming) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

<<<<<<< HEAD
#define IMPL_TIMING_ATTR(name)                                           \
  NS_IMETHODIMP                                                          \
  NullHttpChannel::Get##name##Time(PRTime *_retval) {                    \
    TimeStamp stamp;                                                     \
    Get##name(&stamp);                                                   \
    if (stamp.IsNull()) {                                                \
      *_retval = 0;                                                      \
      return NS_OK;                                                      \
    }                                                                    \
    *_retval =                                                           \
        mChannelCreationTime +                                           \
        (PRTime)((stamp - mChannelCreationTimestamp).ToSeconds() * 1e6); \
    return NS_OK;                                                        \
  }
||||||| merged common ancestors
#define IMPL_TIMING_ATTR(name)                                 \
NS_IMETHODIMP                                                  \
NullHttpChannel::Get##name##Time(PRTime* _retval) {            \
    TimeStamp stamp;                                           \
    Get##name(&stamp);                                         \
    if (stamp.IsNull()) {                                      \
        *_retval = 0;                                          \
        return NS_OK;                                          \
    }                                                          \
    *_retval = mChannelCreationTime +                          \
        (PRTime) ((stamp - mChannelCreationTimestamp).ToSeconds() * 1e6); \
    return NS_OK;                                              \
}
=======
#define IMPL_TIMING_ATTR(name)                                           \
  NS_IMETHODIMP                                                          \
  NullHttpChannel::Get##name##Time(PRTime* _retval) {                    \
    TimeStamp stamp;                                                     \
    Get##name(&stamp);                                                   \
    if (stamp.IsNull()) {                                                \
      *_retval = 0;                                                      \
      return NS_OK;                                                      \
    }                                                                    \
    *_retval =                                                           \
        mChannelCreationTime +                                           \
        (PRTime)((stamp - mChannelCreationTimestamp).ToSeconds() * 1e6); \
    return NS_OK;                                                        \
  }
>>>>>>> upstream-releases

IMPL_TIMING_ATTR(ChannelCreation)
IMPL_TIMING_ATTR(AsyncOpen)
IMPL_TIMING_ATTR(LaunchServiceWorkerStart)
IMPL_TIMING_ATTR(LaunchServiceWorkerEnd)
IMPL_TIMING_ATTR(DispatchFetchEventStart)
IMPL_TIMING_ATTR(DispatchFetchEventEnd)
IMPL_TIMING_ATTR(HandleFetchEventStart)
IMPL_TIMING_ATTR(HandleFetchEventEnd)
IMPL_TIMING_ATTR(DomainLookupStart)
IMPL_TIMING_ATTR(DomainLookupEnd)
IMPL_TIMING_ATTR(ConnectStart)
IMPL_TIMING_ATTR(TcpConnectEnd)
IMPL_TIMING_ATTR(SecureConnectionStart)
IMPL_TIMING_ATTR(ConnectEnd)
IMPL_TIMING_ATTR(RequestStart)
IMPL_TIMING_ATTR(ResponseStart)
IMPL_TIMING_ATTR(ResponseEnd)
IMPL_TIMING_ATTR(CacheReadStart)
IMPL_TIMING_ATTR(CacheReadEnd)
IMPL_TIMING_ATTR(RedirectStart)
IMPL_TIMING_ATTR(RedirectEnd)

#undef IMPL_TIMING_ATTR

}  // namespace net
}  // namespace mozilla
