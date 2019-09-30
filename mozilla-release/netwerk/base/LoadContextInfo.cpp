/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "LoadContextInfo.h"

#include "mozilla/dom/ToJSValue.h"
#include "nsDocShell.h"
#include "nsIChannel.h"
#include "nsILoadContext.h"
#include "nsIWebNavigation.h"
#include "nsNetUtil.h"

using namespace mozilla::dom;
namespace mozilla {
namespace net {

// LoadContextInfo

NS_IMPL_ISUPPORTS(LoadContextInfo, nsILoadContextInfo)

LoadContextInfo::LoadContextInfo(bool aIsAnonymous,
                                 OriginAttributes aOriginAttributes)
    : mIsAnonymous(aIsAnonymous),
      mOriginAttributes(std::move(aOriginAttributes)) {}

<<<<<<< HEAD
NS_IMETHODIMP LoadContextInfo::GetIsPrivate(bool *aIsPrivate) {
||||||| merged common ancestors
NS_IMETHODIMP LoadContextInfo::GetIsPrivate(bool *aIsPrivate)
{
=======
NS_IMETHODIMP LoadContextInfo::GetIsPrivate(bool* aIsPrivate) {
>>>>>>> upstream-releases
  *aIsPrivate = mOriginAttributes.mPrivateBrowsingId > 0;
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP LoadContextInfo::GetIsAnonymous(bool *aIsAnonymous) {
||||||| merged common ancestors
NS_IMETHODIMP LoadContextInfo::GetIsAnonymous(bool *aIsAnonymous)
{
=======
NS_IMETHODIMP LoadContextInfo::GetIsAnonymous(bool* aIsAnonymous) {
>>>>>>> upstream-releases
  *aIsAnonymous = mIsAnonymous;
  return NS_OK;
}

<<<<<<< HEAD
OriginAttributes const *LoadContextInfo::OriginAttributesPtr() {
||||||| merged common ancestors
OriginAttributes const* LoadContextInfo::OriginAttributesPtr()
{
=======
OriginAttributes const* LoadContextInfo::OriginAttributesPtr() {
>>>>>>> upstream-releases
  return &mOriginAttributes;
}

<<<<<<< HEAD
NS_IMETHODIMP LoadContextInfo::GetOriginAttributes(
    JSContext *aCx, JS::MutableHandle<JS::Value> aVal) {
||||||| merged common ancestors
NS_IMETHODIMP LoadContextInfo::GetOriginAttributes(JSContext *aCx,
                                                   JS::MutableHandle<JS::Value> aVal)
{
=======
NS_IMETHODIMP LoadContextInfo::GetOriginAttributes(
    JSContext* aCx, JS::MutableHandle<JS::Value> aVal) {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!ToJSValue(aCx, mOriginAttributes, aVal))) {
    return NS_ERROR_FAILURE;
  }
  return NS_OK;
}

// LoadContextInfoFactory

NS_IMPL_ISUPPORTS(LoadContextInfoFactory, nsILoadContextInfoFactory)

<<<<<<< HEAD
NS_IMETHODIMP LoadContextInfoFactory::GetDefault(
    nsILoadContextInfo **aDefault) {
||||||| merged common ancestors
NS_IMETHODIMP LoadContextInfoFactory::GetDefault(nsILoadContextInfo * *aDefault)
{
=======
NS_IMETHODIMP LoadContextInfoFactory::GetDefault(
    nsILoadContextInfo** aDefault) {
>>>>>>> upstream-releases
  nsCOMPtr<nsILoadContextInfo> info =
      GetLoadContextInfo(false, OriginAttributes());
  info.forget(aDefault);
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP LoadContextInfoFactory::GetPrivate(
    nsILoadContextInfo **aPrivate) {
||||||| merged common ancestors
NS_IMETHODIMP LoadContextInfoFactory::GetPrivate(nsILoadContextInfo * *aPrivate)
{
=======
NS_IMETHODIMP LoadContextInfoFactory::GetPrivate(
    nsILoadContextInfo** aPrivate) {
>>>>>>> upstream-releases
  OriginAttributes attrs;
  attrs.SyncAttributesWithPrivateBrowsing(true);
  nsCOMPtr<nsILoadContextInfo> info = GetLoadContextInfo(false, attrs);
  info.forget(aPrivate);
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP LoadContextInfoFactory::GetAnonymous(
    nsILoadContextInfo **aAnonymous) {
||||||| merged common ancestors
NS_IMETHODIMP LoadContextInfoFactory::GetAnonymous(nsILoadContextInfo * *aAnonymous)
{
=======
NS_IMETHODIMP LoadContextInfoFactory::GetAnonymous(
    nsILoadContextInfo** aAnonymous) {
>>>>>>> upstream-releases
  nsCOMPtr<nsILoadContextInfo> info =
      GetLoadContextInfo(true, OriginAttributes());
  info.forget(aAnonymous);
  return NS_OK;
}

NS_IMETHODIMP LoadContextInfoFactory::Custom(bool aAnonymous,
<<<<<<< HEAD
                                             JS::HandleValue aOriginAttributes,
                                             JSContext *cx,
                                             nsILoadContextInfo **_retval) {
||||||| merged common ancestors
                                             JS::HandleValue aOriginAttributes, JSContext *cx,
                                             nsILoadContextInfo * *_retval)
{
=======
                                             JS::HandleValue aOriginAttributes,
                                             JSContext* cx,
                                             nsILoadContextInfo** _retval) {
>>>>>>> upstream-releases
  OriginAttributes attrs;
  bool status = attrs.Init(cx, aOriginAttributes);
  NS_ENSURE_TRUE(status, NS_ERROR_FAILURE);

  nsCOMPtr<nsILoadContextInfo> info = GetLoadContextInfo(aAnonymous, attrs);
  info.forget(_retval);
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP LoadContextInfoFactory::FromLoadContext(
    nsILoadContext *aLoadContext, bool aAnonymous,
    nsILoadContextInfo **_retval) {
  nsCOMPtr<nsILoadContextInfo> info =
      GetLoadContextInfo(aLoadContext, aAnonymous);
||||||| merged common ancestors
NS_IMETHODIMP LoadContextInfoFactory::FromLoadContext(nsILoadContext *aLoadContext, bool aAnonymous,
                                                      nsILoadContextInfo * *_retval)
{
  nsCOMPtr<nsILoadContextInfo> info = GetLoadContextInfo(aLoadContext, aAnonymous);
=======
NS_IMETHODIMP LoadContextInfoFactory::FromLoadContext(
    nsILoadContext* aLoadContext, bool aAnonymous,
    nsILoadContextInfo** _retval) {
  nsCOMPtr<nsILoadContextInfo> info =
      GetLoadContextInfo(aLoadContext, aAnonymous);
>>>>>>> upstream-releases
  info.forget(_retval);
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP LoadContextInfoFactory::FromWindow(nsIDOMWindow *aWindow,
                                                 bool aAnonymous,
                                                 nsILoadContextInfo **_retval) {
||||||| merged common ancestors
NS_IMETHODIMP LoadContextInfoFactory::FromWindow(nsIDOMWindow *aWindow, bool aAnonymous,
                                                 nsILoadContextInfo * *_retval)
{
=======
NS_IMETHODIMP LoadContextInfoFactory::FromWindow(nsIDOMWindow* aWindow,
                                                 bool aAnonymous,
                                                 nsILoadContextInfo** _retval) {
>>>>>>> upstream-releases
  nsCOMPtr<nsILoadContextInfo> info = GetLoadContextInfo(aWindow, aAnonymous);
  info.forget(_retval);
  return NS_OK;
}

// Helper functions

<<<<<<< HEAD
LoadContextInfo *GetLoadContextInfo(nsIChannel *aChannel) {
||||||| merged common ancestors
LoadContextInfo *
GetLoadContextInfo(nsIChannel * aChannel)
{
=======
LoadContextInfo* GetLoadContextInfo(nsIChannel* aChannel) {
>>>>>>> upstream-releases
  nsresult rv;

  DebugOnly<bool> pb = NS_UsePrivateBrowsing(aChannel);

  bool anon = false;
  nsLoadFlags loadFlags;
  rv = aChannel->GetLoadFlags(&loadFlags);
  if (NS_SUCCEEDED(rv)) {
    anon = !!(loadFlags & nsIChannel::LOAD_ANONYMOUS);
  }

  OriginAttributes oa;
  NS_GetOriginAttributes(aChannel, oa);
  MOZ_ASSERT(pb == (oa.mPrivateBrowsingId > 0));

  return new LoadContextInfo(anon, oa);
}

<<<<<<< HEAD
LoadContextInfo *GetLoadContextInfo(nsILoadContext *aLoadContext,
                                    bool aIsAnonymous) {
||||||| merged common ancestors
LoadContextInfo *
GetLoadContextInfo(nsILoadContext *aLoadContext, bool aIsAnonymous)
{
=======
LoadContextInfo* GetLoadContextInfo(nsILoadContext* aLoadContext,
                                    bool aIsAnonymous) {
>>>>>>> upstream-releases
  if (!aLoadContext) {
    return new LoadContextInfo(aIsAnonymous, OriginAttributes());
  }

  OriginAttributes oa;
  aLoadContext->GetOriginAttributes(oa);

#ifdef DEBUG
  nsCOMPtr<nsIDocShellTreeItem> docShell = do_QueryInterface(aLoadContext);
  if (!docShell || docShell->ItemType() != nsIDocShellTreeItem::typeChrome) {
    MOZ_ASSERT(aLoadContext->UsePrivateBrowsing() ==
               (oa.mPrivateBrowsingId > 0));
  }
#endif

  return new LoadContextInfo(aIsAnonymous, oa);
}

<<<<<<< HEAD
LoadContextInfo *GetLoadContextInfo(nsIDOMWindow *aWindow, bool aIsAnonymous) {
||||||| merged common ancestors
LoadContextInfo*
GetLoadContextInfo(nsIDOMWindow *aWindow,
                   bool aIsAnonymous)
{
=======
LoadContextInfo* GetLoadContextInfo(nsIDOMWindow* aWindow, bool aIsAnonymous) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIWebNavigation> webNav = do_GetInterface(aWindow);
  nsCOMPtr<nsILoadContext> loadContext = do_QueryInterface(webNav);

  return GetLoadContextInfo(loadContext, aIsAnonymous);
}

<<<<<<< HEAD
LoadContextInfo *GetLoadContextInfo(nsILoadContextInfo *aInfo) {
||||||| merged common ancestors
LoadContextInfo *
GetLoadContextInfo(nsILoadContextInfo *aInfo)
{
=======
LoadContextInfo* GetLoadContextInfo(nsILoadContextInfo* aInfo) {
>>>>>>> upstream-releases
  return new LoadContextInfo(aInfo->IsAnonymous(),
                             *aInfo->OriginAttributesPtr());
}

<<<<<<< HEAD
LoadContextInfo *GetLoadContextInfo(bool const aIsAnonymous,
                                    OriginAttributes const &aOriginAttributes) {
||||||| merged common ancestors
LoadContextInfo *
GetLoadContextInfo(bool const aIsAnonymous,
                   OriginAttributes const &aOriginAttributes)
{
=======
LoadContextInfo* GetLoadContextInfo(bool const aIsAnonymous,
                                    OriginAttributes const& aOriginAttributes) {
>>>>>>> upstream-releases
  return new LoadContextInfo(aIsAnonymous, aOriginAttributes);
}

}  // namespace net
}  // namespace mozilla
