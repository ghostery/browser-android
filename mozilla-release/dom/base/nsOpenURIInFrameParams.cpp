/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsOpenURIInFrameParams.h"
#include "mozilla/BasePrincipal.h"
#include "mozilla/dom/Element.h"
#include "mozilla/dom/ToJSValue.h"
#include "mozilla/net/ReferrerPolicy.h"

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(nsOpenURIInFrameParams)
  NS_INTERFACE_MAP_ENTRY(nsIOpenURIInFrameParams)
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

NS_IMPL_CYCLE_COLLECTION(nsOpenURIInFrameParams, mOpenerBrowser)

NS_IMPL_CYCLE_COLLECTING_ADDREF(nsOpenURIInFrameParams)
NS_IMPL_CYCLE_COLLECTING_RELEASE(nsOpenURIInFrameParams)

<<<<<<< HEAD
nsOpenURIInFrameParams::nsOpenURIInFrameParams(
    const mozilla::OriginAttributes& aOriginAttributes,
    nsIFrameLoaderOwner* aOpener)
    : mOpenerOriginAttributes(aOriginAttributes),
      mOpenerBrowser(aOpener),
      mReferrerPolicy(mozilla::net::RP_Unset) {}
||||||| merged common ancestors
nsOpenURIInFrameParams::nsOpenURIInFrameParams(const mozilla::OriginAttributes& aOriginAttributes,
                                               nsIFrameLoaderOwner* aOpener)
  : mOpenerOriginAttributes(aOriginAttributes)
  , mOpenerBrowser(aOpener)
  , mReferrerPolicy(mozilla::net::RP_Unset)
{
}
=======
nsOpenURIInFrameParams::nsOpenURIInFrameParams(
    const mozilla::OriginAttributes& aOriginAttributes,
    mozilla::dom::Element* aOpener)
    : mOpenerOriginAttributes(aOriginAttributes), mOpenerBrowser(aOpener) {}
>>>>>>> upstream-releases

nsOpenURIInFrameParams::~nsOpenURIInFrameParams() {}

NS_IMETHODIMP
<<<<<<< HEAD
nsOpenURIInFrameParams::GetReferrer(nsAString& aReferrer) {
  aReferrer = mReferrer;
||||||| merged common ancestors
nsOpenURIInFrameParams::GetReferrer(nsAString& aReferrer)
{
  aReferrer = mReferrer;
=======
nsOpenURIInFrameParams::GetReferrerInfo(nsIReferrerInfo** aReferrerInfo) {
  NS_IF_ADDREF(*aReferrerInfo = mReferrerInfo);
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOpenURIInFrameParams::SetReferrer(const nsAString& aReferrer) {
  mReferrer = aReferrer;
||||||| merged common ancestors
nsOpenURIInFrameParams::SetReferrer(const nsAString& aReferrer)
{
  mReferrer = aReferrer;
=======
nsOpenURIInFrameParams::SetReferrerInfo(nsIReferrerInfo* aReferrerInfo) {
  mReferrerInfo = aReferrerInfo;
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOpenURIInFrameParams::GetReferrerPolicy(uint32_t* aReferrerPolicy) {
  *aReferrerPolicy = mReferrerPolicy;
||||||| merged common ancestors
nsOpenURIInFrameParams::GetReferrerPolicy(uint32_t* aReferrerPolicy)
{
  *aReferrerPolicy = mReferrerPolicy;
=======
nsOpenURIInFrameParams::GetIsPrivate(bool* aIsPrivate) {
  NS_ENSURE_ARG_POINTER(aIsPrivate);
  *aIsPrivate = mOpenerOriginAttributes.mPrivateBrowsingId > 0;
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOpenURIInFrameParams::SetReferrerPolicy(uint32_t aReferrerPolicy) {
  mReferrerPolicy = aReferrerPolicy;
||||||| merged common ancestors
nsOpenURIInFrameParams::SetReferrerPolicy(uint32_t aReferrerPolicy)
{
  mReferrerPolicy = aReferrerPolicy;
=======
nsOpenURIInFrameParams::GetTriggeringPrincipal(
    nsIPrincipal** aTriggeringPrincipal) {
  NS_ADDREF(*aTriggeringPrincipal = mTriggeringPrincipal);
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOpenURIInFrameParams::GetIsPrivate(bool* aIsPrivate) {
  NS_ENSURE_ARG_POINTER(aIsPrivate);
  *aIsPrivate = mOpenerOriginAttributes.mPrivateBrowsingId > 0;
||||||| merged common ancestors
nsOpenURIInFrameParams::GetIsPrivate(bool* aIsPrivate)
{
  NS_ENSURE_ARG_POINTER(aIsPrivate);
  *aIsPrivate = mOpenerOriginAttributes.mPrivateBrowsingId > 0;
=======
nsOpenURIInFrameParams::SetTriggeringPrincipal(
    nsIPrincipal* aTriggeringPrincipal) {
  NS_ENSURE_TRUE(aTriggeringPrincipal, NS_ERROR_INVALID_ARG);
  mTriggeringPrincipal = aTriggeringPrincipal;
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOpenURIInFrameParams::GetTriggeringPrincipal(
    nsIPrincipal** aTriggeringPrincipal) {
  NS_ADDREF(*aTriggeringPrincipal = mTriggeringPrincipal);
||||||| merged common ancestors
nsOpenURIInFrameParams::GetTriggeringPrincipal(nsIPrincipal** aTriggeringPrincipal)
{
  NS_ADDREF(*aTriggeringPrincipal = mTriggeringPrincipal);
=======
nsOpenURIInFrameParams::GetCsp(nsIContentSecurityPolicy** aCsp) {
  NS_IF_ADDREF(*aCsp = mCsp);
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsOpenURIInFrameParams::SetTriggeringPrincipal(
    nsIPrincipal* aTriggeringPrincipal) {
  NS_ENSURE_TRUE(aTriggeringPrincipal, NS_ERROR_INVALID_ARG);
  mTriggeringPrincipal = aTriggeringPrincipal;
||||||| merged common ancestors
nsOpenURIInFrameParams::SetTriggeringPrincipal(nsIPrincipal* aTriggeringPrincipal)
{
  NS_ENSURE_TRUE(aTriggeringPrincipal, NS_ERROR_INVALID_ARG);
  mTriggeringPrincipal = aTriggeringPrincipal;
=======
nsOpenURIInFrameParams::SetCsp(nsIContentSecurityPolicy* aCsp) {
  NS_ENSURE_TRUE(aCsp, NS_ERROR_INVALID_ARG);
  mCsp = aCsp;
>>>>>>> upstream-releases
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP
nsOpenURIInFrameParams::GetOpenerBrowser(nsIFrameLoaderOwner** aOpenerBrowser) {
  nsCOMPtr<nsIFrameLoaderOwner> owner = mOpenerBrowser;
||||||| merged common ancestors
NS_IMETHODIMP
nsOpenURIInFrameParams::GetOpenerBrowser(nsIFrameLoaderOwner** aOpenerBrowser)
{
  nsCOMPtr<nsIFrameLoaderOwner> owner = mOpenerBrowser;
=======
nsresult nsOpenURIInFrameParams::GetOpenerBrowser(
    mozilla::dom::Element** aOpenerBrowser) {
  RefPtr<mozilla::dom::Element> owner = mOpenerBrowser;
>>>>>>> upstream-releases
  owner.forget(aOpenerBrowser);
  return NS_OK;
}

NS_IMETHODIMP
nsOpenURIInFrameParams::GetOpenerOriginAttributes(
    JSContext* aCx, JS::MutableHandle<JS::Value> aValue) {
  bool ok = ToJSValue(aCx, mOpenerOriginAttributes, aValue);
  NS_ENSURE_TRUE(ok, NS_ERROR_FAILURE);
  return NS_OK;
}
