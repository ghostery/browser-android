/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsProxyInfo.h"
#include "nsCOMPtr.h"

namespace mozilla {
namespace net {

// Yes, we support QI to nsProxyInfo
NS_IMPL_ISUPPORTS(nsProxyInfo, nsProxyInfo, nsIProxyInfo)

NS_IMETHODIMP
<<<<<<< HEAD
nsProxyInfo::GetHost(nsACString &result) {
||||||| merged common ancestors
nsProxyInfo::GetHost(nsACString &result)
{
=======
nsProxyInfo::GetHost(nsACString& result) {
>>>>>>> upstream-releases
  result = mHost;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsProxyInfo::GetPort(int32_t *result) {
||||||| merged common ancestors
nsProxyInfo::GetPort(int32_t *result)
{
=======
nsProxyInfo::GetPort(int32_t* result) {
>>>>>>> upstream-releases
  *result = mPort;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsProxyInfo::GetType(nsACString &result) {
||||||| merged common ancestors
nsProxyInfo::GetType(nsACString &result)
{
=======
nsProxyInfo::GetType(nsACString& result) {
>>>>>>> upstream-releases
  result = mType;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsProxyInfo::GetFlags(uint32_t *result) {
||||||| merged common ancestors
nsProxyInfo::GetFlags(uint32_t *result)
{
=======
nsProxyInfo::GetFlags(uint32_t* result) {
>>>>>>> upstream-releases
  *result = mFlags;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsProxyInfo::GetResolveFlags(uint32_t *result) {
||||||| merged common ancestors
nsProxyInfo::GetResolveFlags(uint32_t *result)
{
=======
nsProxyInfo::GetResolveFlags(uint32_t* result) {
>>>>>>> upstream-releases
  *result = mResolveFlags;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsProxyInfo::GetUsername(nsACString &result) {
||||||| merged common ancestors
nsProxyInfo::GetUsername(nsACString &result)
{
=======
nsProxyInfo::GetUsername(nsACString& result) {
>>>>>>> upstream-releases
  result = mUsername;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsProxyInfo::GetPassword(nsACString &result) {
||||||| merged common ancestors
nsProxyInfo::GetPassword(nsACString &result)
{
=======
nsProxyInfo::GetPassword(nsACString& result) {
>>>>>>> upstream-releases
  result = mPassword;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsProxyInfo::GetFailoverTimeout(uint32_t *result) {
||||||| merged common ancestors
nsProxyInfo::GetFailoverTimeout(uint32_t *result)
{
=======
nsProxyInfo::GetProxyAuthorizationHeader(nsACString& result) {
  result = mProxyAuthorizationHeader;
  return NS_OK;
}

NS_IMETHODIMP
nsProxyInfo::GetConnectionIsolationKey(nsACString& result) {
  result = mConnectionIsolationKey;
  return NS_OK;
}

NS_IMETHODIMP
nsProxyInfo::GetFailoverTimeout(uint32_t* result) {
>>>>>>> upstream-releases
  *result = mTimeout;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsProxyInfo::GetFailoverProxy(nsIProxyInfo **result) {
||||||| merged common ancestors
nsProxyInfo::GetFailoverProxy(nsIProxyInfo **result)
{
=======
nsProxyInfo::GetFailoverProxy(nsIProxyInfo** result) {
>>>>>>> upstream-releases
  NS_IF_ADDREF(*result = mNext);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsProxyInfo::SetFailoverProxy(nsIProxyInfo *proxy) {
||||||| merged common ancestors
nsProxyInfo::SetFailoverProxy(nsIProxyInfo *proxy)
{
=======
nsProxyInfo::SetFailoverProxy(nsIProxyInfo* proxy) {
>>>>>>> upstream-releases
  nsCOMPtr<nsProxyInfo> pi = do_QueryInterface(proxy);
  NS_ENSURE_ARG(pi);

  pi.swap(mNext);
  return NS_OK;
}

// These pointers are declared in nsProtocolProxyService.cpp and
// comparison of mType by string pointer is valid within necko
extern const char kProxyType_HTTP[];
extern const char kProxyType_HTTPS[];
extern const char kProxyType_SOCKS[];
extern const char kProxyType_SOCKS4[];
extern const char kProxyType_SOCKS5[];
extern const char kProxyType_DIRECT[];

bool nsProxyInfo::IsDirect() {
  if (!mType) return true;
  return mType == kProxyType_DIRECT;
}

bool nsProxyInfo::IsHTTP() { return mType == kProxyType_HTTP; }

bool nsProxyInfo::IsHTTPS() { return mType == kProxyType_HTTPS; }

bool nsProxyInfo::IsSOCKS() {
  return mType == kProxyType_SOCKS || mType == kProxyType_SOCKS4 ||
         mType == kProxyType_SOCKS5;
}

}  // namespace net
}  // namespace mozilla
