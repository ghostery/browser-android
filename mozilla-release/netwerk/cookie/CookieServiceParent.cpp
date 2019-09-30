/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/net/CookieServiceParent.h"
#include "mozilla/dom/PContentParent.h"
#include "mozilla/net/NeckoParent.h"

#include "mozilla/BasePrincipal.h"
#include "mozilla/ipc/URIUtils.h"
#include "mozilla/StoragePrincipalHelper.h"
#include "nsArrayUtils.h"
#include "nsCookieService.h"
#include "nsIChannel.h"
#include "nsIEffectiveTLDService.h"
#include "nsIScriptSecurityManager.h"
#include "nsIPrivateBrowsingChannel.h"
#include "nsNetCID.h"
#include "nsPrintfCString.h"

using namespace mozilla::ipc;
using mozilla::BasePrincipal;
using mozilla::OriginAttributes;
using mozilla::dom::PContentParent;
using mozilla::net::NeckoParent;

namespace {

// Ignore failures from this function, as they only affect whether we do or
// don't show a dialog box in private browsing mode if the user sets a pref.
<<<<<<< HEAD
void CreateDummyChannel(nsIURI *aHostURI, nsIURI *aChannelURI,
                        OriginAttributes &aAttrs, nsIChannel **aChannel) {
  nsCOMPtr<nsIPrincipal> principal =
      BasePrincipal::CreateCodebasePrincipal(aHostURI, aAttrs);
  if (!principal) {
    return;
  }

  // The following channel is never openend, so it does not matter what
  // securityFlags we pass; let's follow the principle of least privilege.
||||||| merged common ancestors
void
CreateDummyChannel(nsIURI* aHostURI, nsIURI* aChannelURI,
                   OriginAttributes& aAttrs, nsIChannel** aChannel)
{
  nsCOMPtr<nsIPrincipal> principal =
    BasePrincipal::CreateCodebasePrincipal(aHostURI, aAttrs);
  if (!principal) {
    return;
  }

  // The following channel is never openend, so it does not matter what
  // securityFlags we pass; let's follow the principle of least privilege.
=======
nsresult CreateDummyChannel(nsIURI* aHostURI, nsILoadInfo* aLoadInfo,
                            nsIChannel** aChannel) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIChannel> dummyChannel;
<<<<<<< HEAD
  NS_NewChannel(getter_AddRefs(dummyChannel), aChannelURI, principal,
                nsILoadInfo::SEC_REQUIRE_SAME_ORIGIN_DATA_IS_BLOCKED,
                nsIContentPolicy::TYPE_INVALID);
  nsCOMPtr<nsIPrivateBrowsingChannel> pbChannel =
      do_QueryInterface(dummyChannel);
  if (!pbChannel) {
    return;
||||||| merged common ancestors
  NS_NewChannel(getter_AddRefs(dummyChannel), aChannelURI, principal,
                nsILoadInfo::SEC_REQUIRE_SAME_ORIGIN_DATA_IS_BLOCKED,
                nsIContentPolicy::TYPE_INVALID);
  nsCOMPtr<nsIPrivateBrowsingChannel> pbChannel = do_QueryInterface(dummyChannel);
  if (!pbChannel) {
    return;
=======
  nsresult rv =
      NS_NewChannelInternal(getter_AddRefs(dummyChannel), aHostURI, aLoadInfo);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
>>>>>>> upstream-releases
  }

  dummyChannel.forget(aChannel);
  return NS_OK;
}

}  // namespace

namespace mozilla {
namespace net {

CookieServiceParent::CookieServiceParent() {
  // Instantiate the cookieservice via the service manager, so it sticks around
  // until shutdown.
  nsCOMPtr<nsICookieService> cs = do_GetService(NS_COOKIESERVICE_CONTRACTID);

  // Get the nsCookieService instance directly, so we can call internal methods.
  mCookieService = nsCookieService::GetSingleton();
  NS_ASSERTION(mCookieService, "couldn't get nsICookieService");
  mProcessingCookie = false;
}

<<<<<<< HEAD
void GetInfoFromCookie(nsCookie *aCookie, CookieStruct &aCookieStruct) {
  aCookieStruct.name() = aCookie->Name();
  aCookieStruct.value() = aCookie->Value();
  aCookieStruct.host() = aCookie->Host();
  aCookieStruct.path() = aCookie->Path();
  aCookieStruct.expiry() = aCookie->Expiry();
  aCookieStruct.lastAccessed() = aCookie->LastAccessed();
  aCookieStruct.creationTime() = aCookie->CreationTime();
  aCookieStruct.isSession() = aCookie->IsSession();
  aCookieStruct.isSecure() = aCookie->IsSecure();
  aCookieStruct.isHttpOnly() = aCookie->IsHttpOnly();
  aCookieStruct.sameSite() = aCookie->SameSite();
}

void CookieServiceParent::RemoveBatchDeletedCookies(nsIArray *aCookieList) {
||||||| merged common ancestors
void
GetInfoFromCookie(nsCookie         *aCookie,
                  CookieStruct     &aCookieStruct)
{
  aCookieStruct.name() = aCookie->Name();
  aCookieStruct.value() = aCookie->Value();
  aCookieStruct.host() = aCookie->Host();
  aCookieStruct.path() = aCookie->Path();
  aCookieStruct.expiry() = aCookie->Expiry();
  aCookieStruct.lastAccessed() = aCookie->LastAccessed();
  aCookieStruct.creationTime() = aCookie->CreationTime();
  aCookieStruct.isSession() = aCookie->IsSession();
  aCookieStruct.isSecure() = aCookie->IsSecure();
  aCookieStruct.isHttpOnly() = aCookie->IsHttpOnly();
  aCookieStruct.sameSite() = aCookie->SameSite();
}

void
CookieServiceParent::RemoveBatchDeletedCookies(nsIArray *aCookieList) {
=======
void CookieServiceParent::RemoveBatchDeletedCookies(nsIArray* aCookieList) {
>>>>>>> upstream-releases
  uint32_t len = 0;
  aCookieList->GetLength(&len);
  OriginAttributes attrs;
  CookieStruct cookieStruct;
  nsTArray<CookieStruct> cookieStructList;
  nsTArray<OriginAttributes> attrsList;
  for (uint32_t i = 0; i < len; i++) {
    nsCOMPtr<nsICookie> xpcCookie = do_QueryElementAt(aCookieList, i);
    auto cookie = static_cast<nsCookie *>(xpcCookie.get());
    attrs = cookie->OriginAttributesRef();
    cookieStruct = cookie->ToIPC();
    if (cookie->IsHttpOnly()) {
      // Child only needs to exist if an HttpOnly cookie exists, not its value
      cookieStruct.value() = "";
    }
    cookieStructList.AppendElement(cookieStruct);
    attrsList.AppendElement(attrs);
  }
  Unused << SendRemoveBatchDeletedCookies(cookieStructList, attrsList);
}

void CookieServiceParent::RemoveAll() { Unused << SendRemoveAll(); }

<<<<<<< HEAD
void CookieServiceParent::RemoveCookie(nsICookie *aCookie) {
  auto cookie = static_cast<nsCookie *>(aCookie);
||||||| merged common ancestors
void
CookieServiceParent::RemoveCookie(nsICookie *aCookie)
{
  auto cookie = static_cast<nsCookie*>(aCookie);
=======
void CookieServiceParent::RemoveCookie(nsICookie* aCookie) {
  auto cookie = static_cast<nsCookie*>(aCookie);
>>>>>>> upstream-releases
  OriginAttributes attrs = cookie->OriginAttributesRef();
  CookieStruct cookieStruct = cookie->ToIPC();
  if (cookie->IsHttpOnly()) {
    cookieStruct.value() = "";
  }
  Unused << SendRemoveCookie(cookieStruct, attrs);
}

<<<<<<< HEAD
void CookieServiceParent::AddCookie(nsICookie *aCookie) {
  auto cookie = static_cast<nsCookie *>(aCookie);
||||||| merged common ancestors
void
CookieServiceParent::AddCookie(nsICookie *aCookie)
{
  auto cookie = static_cast<nsCookie*>(aCookie);
=======
void CookieServiceParent::AddCookie(nsICookie* aCookie) {
  auto cookie = static_cast<nsCookie*>(aCookie);
>>>>>>> upstream-releases
  OriginAttributes attrs = cookie->OriginAttributesRef();
  CookieStruct cookieStruct = cookie->ToIPC();
  if (cookie->IsHttpOnly()) {
    cookieStruct.value() = "";
  }
  Unused << SendAddCookie(cookieStruct, attrs);
}

<<<<<<< HEAD
void CookieServiceParent::TrackCookieLoad(nsIChannel *aChannel) {
||||||| merged common ancestors
void
CookieServiceParent::TrackCookieLoad(nsIChannel *aChannel)
{
=======
void CookieServiceParent::TrackCookieLoad(nsIChannel* aChannel) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIURI> uri;
  aChannel->GetURI(getter_AddRefs(uri));

  nsCOMPtr<nsILoadInfo> loadInfo = aChannel->LoadInfo();
  mozilla::OriginAttributes attrs = loadInfo->GetOriginAttributes();
  bool isSafeTopLevelNav = NS_IsSafeTopLevelNav(aChannel);
  bool aIsSameSiteForeign = NS_IsSameSiteForeign(aChannel, uri);

  StoragePrincipalHelper::PrepareOriginAttributes(aChannel, attrs);

  // Send matching cookies to Child.
  nsCOMPtr<mozIThirdPartyUtil> thirdPartyUtil;
  thirdPartyUtil = do_GetService(THIRDPARTYUTIL_CONTRACTID);
  bool isForeign = true;
  thirdPartyUtil->IsThirdPartyChannel(aChannel, uri, &isForeign);

  bool isTrackingResource = false;
  bool storageAccessGranted = false;
  uint32_t rejectedReason = 0;
  nsCOMPtr<nsIHttpChannel> httpChannel = do_QueryInterface(aChannel);
  if (httpChannel) {
    isTrackingResource = httpChannel->IsTrackingResource();
    // Check first-party storage access even for non-tracking resources, since
    // we will need the result when computing the access rights for the reject
    // foreign cookie behavior mode.
<<<<<<< HEAD
    if (isForeign && AntiTrackingCommon::IsFirstPartyStorageAccessGrantedFor(
                         httpChannel, uri, nullptr)) {
||||||| merged common ancestors
    if (isForeign &&
        AntiTrackingCommon::IsFirstPartyStorageAccessGrantedFor(httpChannel,
                                                                uri,
                                                                nullptr)) {
=======
    if (AntiTrackingCommon::IsFirstPartyStorageAccessGrantedFor(
            httpChannel, uri, &rejectedReason)) {
>>>>>>> upstream-releases
      storageAccessGranted = true;
    }
  }

<<<<<<< HEAD
  nsTArray<nsCookie *> foundCookieList;
  mCookieService->GetCookiesForURI(
      uri, isForeign, isTrackingResource, storageAccessGranted,
      isSafeTopLevelNav, aIsSameSiteForeign, false, attrs, foundCookieList);
||||||| merged common ancestors
  nsTArray<nsCookie*> foundCookieList;
  mCookieService->GetCookiesForURI(uri, isForeign, isTrackingResource,
                                   storageAccessGranted, isSafeTopLevelNav,
                                   aIsSameSiteForeign, false, attrs,
                                   foundCookieList);
=======
  nsTArray<nsCookie*> foundCookieList;
  mCookieService->GetCookiesForURI(uri, aChannel, isForeign, isTrackingResource,
                                   storageAccessGranted, rejectedReason,
                                   isSafeTopLevelNav, aIsSameSiteForeign, false,
                                   attrs, foundCookieList);
>>>>>>> upstream-releases
  nsTArray<CookieStruct> matchingCookiesList;
  SerialializeCookieList(foundCookieList, matchingCookiesList, uri);
  Unused << SendTrackCookiesLoad(matchingCookiesList, attrs);
}

<<<<<<< HEAD
void CookieServiceParent::SerialializeCookieList(
    const nsTArray<nsCookie *> &aFoundCookieList,
    nsTArray<CookieStruct> &aCookiesList, nsIURI *aHostURI) {
||||||| merged common ancestors
void
CookieServiceParent::SerialializeCookieList(const nsTArray<nsCookie*> &aFoundCookieList,
                                            nsTArray<CookieStruct>    &aCookiesList,
                                            nsIURI                    *aHostURI)
{
=======
void CookieServiceParent::SerialializeCookieList(
    const nsTArray<nsCookie*>& aFoundCookieList,
    nsTArray<CookieStruct>& aCookiesList, nsIURI* aHostURI) {
>>>>>>> upstream-releases
  for (uint32_t i = 0; i < aFoundCookieList.Length(); i++) {
<<<<<<< HEAD
    nsCookie *cookie = aFoundCookieList.ElementAt(i);
    CookieStruct *cookieStruct = aCookiesList.AppendElement();
    cookieStruct->name() = cookie->Name();
    if (!cookie->IsHttpOnly()) {
      cookieStruct->value() = cookie->Value();
||||||| merged common ancestors
    nsCookie *cookie = aFoundCookieList.ElementAt(i);
    CookieStruct* cookieStruct = aCookiesList.AppendElement();
    cookieStruct->name() = cookie->Name();
    if (!cookie->IsHttpOnly()) {
      cookieStruct->value() = cookie->Value();
=======
    nsCookie* cookie = aFoundCookieList.ElementAt(i);
    CookieStruct* cookieStruct = aCookiesList.AppendElement();
    *cookieStruct = cookie->ToIPC();
    if (cookie->IsHttpOnly()) {
      // Value only needs to exist if an HttpOnly cookie exists.
      cookieStruct->value() = "";
>>>>>>> upstream-releases
    }
  }
}

<<<<<<< HEAD
mozilla::ipc::IPCResult CookieServiceParent::RecvPrepareCookieList(
    const URIParams &aHost, const bool &aIsForeign,
    const bool &aIsTrackingResource,
    const bool &aFirstPartyStorageAccessGranted, const bool &aIsSafeTopLevelNav,
    const bool &aIsSameSiteForeign, const OriginAttributes &aAttrs) {
||||||| merged common ancestors
mozilla::ipc::IPCResult
CookieServiceParent::RecvPrepareCookieList(const URIParams        &aHost,
                                           const bool             &aIsForeign,
                                           const bool             &aIsTrackingResource,
                                           const bool             &aFirstPartyStorageAccessGranted,
                                           const bool             &aIsSafeTopLevelNav,
                                           const bool             &aIsSameSiteForeign,
                                           const OriginAttributes &aAttrs)
{
=======
mozilla::ipc::IPCResult CookieServiceParent::RecvPrepareCookieList(
    const URIParams& aHost, const bool& aIsForeign,
    const bool& aIsTrackingResource,
    const bool& aFirstPartyStorageAccessGranted,
    const uint32_t& aRejectedReason, const bool& aIsSafeTopLevelNav,
    const bool& aIsSameSiteForeign, const OriginAttributes& aAttrs) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIURI> hostURI = DeserializeURI(aHost);

  // Send matching cookies to Child.
<<<<<<< HEAD
  nsTArray<nsCookie *> foundCookieList;
  mCookieService->GetCookiesForURI(
      hostURI, aIsForeign, aIsTrackingResource, aFirstPartyStorageAccessGranted,
      aIsSafeTopLevelNav, aIsSameSiteForeign, false, aAttrs, foundCookieList);
||||||| merged common ancestors
  nsTArray<nsCookie*> foundCookieList;
  mCookieService->GetCookiesForURI(hostURI, aIsForeign, aIsTrackingResource,
                                   aFirstPartyStorageAccessGranted, aIsSafeTopLevelNav,
                                   aIsSameSiteForeign, false, aAttrs,
                                   foundCookieList);
=======
  nsTArray<nsCookie*> foundCookieList;
  // Note: passing nullptr as aChannel to GetCookiesForURI() here is fine since
  // this argument is only used for proper reporting of cookie loads, but the
  // child process already does the necessary reporting in this case for us.
  mCookieService->GetCookiesForURI(
      hostURI, nullptr, aIsForeign, aIsTrackingResource,
      aFirstPartyStorageAccessGranted, aRejectedReason, aIsSafeTopLevelNav,
      aIsSameSiteForeign, false, aAttrs, foundCookieList);
>>>>>>> upstream-releases
  nsTArray<CookieStruct> matchingCookiesList;
  SerialializeCookieList(foundCookieList, matchingCookiesList, hostURI);
  Unused << SendTrackCookiesLoad(matchingCookiesList, aAttrs);
  return IPC_OK();
}

void CookieServiceParent::ActorDestroy(ActorDestroyReason aWhy) {
  // Nothing needed here. Called right before destructor since this is a
  // non-refcounted class.
}

<<<<<<< HEAD
mozilla::ipc::IPCResult CookieServiceParent::RecvSetCookieString(
    const URIParams &aHost, const OptionalURIParams &aChannelURI,
    const bool &aIsForeign, const bool &aIsTrackingResource,
    const bool &aFirstPartyStorageAccessGranted, const nsCString &aCookieString,
    const nsCString &aServerTime, const OriginAttributes &aAttrs,
    const bool &aFromHttp) {
  if (!mCookieService) return IPC_OK();
||||||| merged common ancestors
mozilla::ipc::IPCResult
CookieServiceParent::RecvSetCookieString(const URIParams& aHost,
                                         const OptionalURIParams& aChannelURI,
                                         const bool& aIsForeign,
                                         const bool& aIsTrackingResource,
                                         const bool& aFirstPartyStorageAccessGranted,
                                         const nsCString& aCookieString,
                                         const nsCString& aServerTime,
                                         const OriginAttributes& aAttrs,
                                         const bool& aFromHttp)
{
  if (!mCookieService)
    return IPC_OK();
=======
mozilla::ipc::IPCResult CookieServiceParent::RecvSetCookieString(
    const URIParams& aHost, const Maybe<URIParams>& aChannelURI,
    const Maybe<LoadInfoArgs>& aLoadInfoArgs, const bool& aIsForeign,
    const bool& aIsTrackingResource,
    const bool& aFirstPartyStorageAccessGranted,
    const uint32_t& aRejectedReason, const OriginAttributes& aAttrs,
    const nsCString& aCookieString, const nsCString& aServerTime,
    const bool& aFromHttp) {
  if (!mCookieService) return IPC_OK();
>>>>>>> upstream-releases

  // Deserialize URI. Having a host URI is mandatory and should always be
  // provided by the child; thus we consider failure fatal.
  nsCOMPtr<nsIURI> hostURI = DeserializeURI(aHost);
  if (!hostURI) return IPC_FAIL_NO_REASON(this);

  nsCOMPtr<nsIURI> channelURI = DeserializeURI(aChannelURI);

  nsCOMPtr<nsILoadInfo> loadInfo;
  Unused << NS_WARN_IF(NS_FAILED(
      LoadInfoArgsToLoadInfo(aLoadInfoArgs, getter_AddRefs(loadInfo))));

  // This is a gross hack. We've already computed everything we need to know
  // for whether to set this cookie or not, but we need to communicate all of
  // this information through to nsICookiePermission, which indirectly
  // computes the information from the channel. We only care about the
  // aIsPrivate argument as nsCookieService::SetCookieStringInternal deals
  // with aIsForeign before we have to worry about nsCookiePermission trying
  // to use the channel to inspect it.
  nsCOMPtr<nsIChannel> dummyChannel;
<<<<<<< HEAD
  CreateDummyChannel(hostURI, channelURI,
                     const_cast<OriginAttributes &>(aAttrs),
                     getter_AddRefs(dummyChannel));
||||||| merged common ancestors
  CreateDummyChannel(hostURI, channelURI,
                     const_cast<OriginAttributes&>(aAttrs),
                     getter_AddRefs(dummyChannel));
=======
  nsresult rv =
      CreateDummyChannel(channelURI, loadInfo, getter_AddRefs(dummyChannel));
  if (NS_WARN_IF(NS_FAILED(rv))) {
    // No reason to kill the content process.
    return IPC_OK();
  }
>>>>>>> upstream-releases

  // NB: dummyChannel could be null if something failed in CreateDummyChannel.
  nsDependentCString cookieString(aCookieString, 0);

  // We set this to true while processing this cookie update, to make sure
  // we don't send it back to the same content process.
  mProcessingCookie = true;
<<<<<<< HEAD
  mCookieService->SetCookieStringInternal(
      hostURI, aIsForeign, aIsTrackingResource, aFirstPartyStorageAccessGranted,
      cookieString, aServerTime, aFromHttp, aAttrs, dummyChannel);
||||||| merged common ancestors
  mCookieService->SetCookieStringInternal(hostURI, aIsForeign,
                                          aIsTrackingResource,
                                          aFirstPartyStorageAccessGranted,
                                          cookieString, aServerTime, aFromHttp,
                                          aAttrs, dummyChannel);
=======
  mCookieService->SetCookieStringInternal(
      hostURI, aIsForeign, aIsTrackingResource, aFirstPartyStorageAccessGranted,
      aRejectedReason, cookieString, aServerTime, aFromHttp, aAttrs,
      dummyChannel);
>>>>>>> upstream-releases
  mProcessingCookie = false;
  return IPC_OK();
}

}  // namespace net
}  // namespace mozilla
