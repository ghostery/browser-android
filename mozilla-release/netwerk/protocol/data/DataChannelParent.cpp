/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=4 sw=2 sts=2 et tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "DataChannelParent.h"
#include "mozilla/Assertions.h"
#include "nsNetUtil.h"
#include "nsIChannel.h"

namespace mozilla {
namespace net {

NS_IMPL_ISUPPORTS(DataChannelParent, nsIParentChannel, nsIStreamListener)

<<<<<<< HEAD
bool DataChannelParent::Init(const uint32_t &channelId) {
  nsCOMPtr<nsIChannel> channel;
  MOZ_ALWAYS_SUCCEEDS(
      NS_LinkRedirectChannels(channelId, this, getter_AddRefs(channel)));
||||||| merged common ancestors
bool
DataChannelParent::Init(const uint32_t &channelId)
{
    nsCOMPtr<nsIChannel> channel;
    MOZ_ALWAYS_SUCCEEDS(
        NS_LinkRedirectChannels(channelId, this, getter_AddRefs(channel)));
=======
bool DataChannelParent::Init(const uint32_t& channelId) {
  nsCOMPtr<nsIChannel> channel;
  MOZ_ALWAYS_SUCCEEDS(
      NS_LinkRedirectChannels(channelId, this, getter_AddRefs(channel)));
>>>>>>> upstream-releases

  return true;
}

NS_IMETHODIMP
<<<<<<< HEAD
DataChannelParent::SetParentListener(HttpChannelParentListener *aListener) {
  // Nothing to do.
  return NS_OK;
||||||| merged common ancestors
DataChannelParent::SetParentListener(HttpChannelParentListener* aListener)
{
    // Nothing to do.
    return NS_OK;
=======
DataChannelParent::SetParentListener(HttpChannelParentListener* aListener) {
  // Nothing to do.
  return NS_OK;
}

NS_IMETHODIMP
DataChannelParent::NotifyChannelClassifierProtectionDisabled(
    uint32_t aAcceptedReason) {
  // Nothing to do.
  return NS_OK;
}

NS_IMETHODIMP
DataChannelParent::NotifyCookieAllowed() {
  // Nothing to do.
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
DataChannelParent::NotifyTrackingProtectionDisabled() {
  // Nothing to do.
  return NS_OK;
||||||| merged common ancestors
DataChannelParent::NotifyTrackingProtectionDisabled()
{
    // Nothing to do.
    return NS_OK;
=======
DataChannelParent::NotifyCookieBlocked(uint32_t aRejectedReason) {
  // Nothing to do.
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
DataChannelParent::NotifyCookieAllowed() {
  // Nothing to do.
  return NS_OK;
||||||| merged common ancestors
DataChannelParent::NotifyTrackingCookieBlocked(uint32_t aRejectedReason)
{
    // Nothing to do.
    return NS_OK;
=======
DataChannelParent::NotifyClassificationFlags(uint32_t aClassificationFlags,
                                             bool aIsThirdParty) {
  // Nothing to do.
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
DataChannelParent::NotifyTrackingCookieBlocked(uint32_t aRejectedReason) {
  // Nothing to do.
  return NS_OK;
||||||| merged common ancestors
DataChannelParent::NotifyTrackingResource(bool aIsThirdParty)
{
    // Nothing to do.
    return NS_OK;
=======
DataChannelParent::NotifyFlashPluginStateChanged(
    nsIHttpChannel::FlashPluginState aState) {
  // Nothing to do.
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
DataChannelParent::NotifyTrackingResource(bool aIsThirdParty) {
  // Nothing to do.
||||||| merged common ancestors
DataChannelParent::SetClassifierMatchedInfo(const nsACString& aList,
                                            const nsACString& aProvider,
                                            const nsACString& aFullHash)
{
  // nothing to do
=======
DataChannelParent::SetClassifierMatchedInfo(const nsACString& aList,
                                            const nsACString& aProvider,
                                            const nsACString& aFullHash) {
  // nothing to do
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
DataChannelParent::SetClassifierMatchedInfo(const nsACString &aList,
                                            const nsACString &aProvider,
                                            const nsACString &aFullHash) {
  // nothing to do
  return NS_OK;
||||||| merged common ancestors
DataChannelParent::Delete()
{
    // Nothing to do.
    return NS_OK;
=======
DataChannelParent::SetClassifierMatchedTrackingInfo(
    const nsACString& aLists, const nsACString& aFullHashes) {
  // nothing to do
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
DataChannelParent::Delete() {
  // Nothing to do.
  return NS_OK;
}

void DataChannelParent::ActorDestroy(ActorDestroyReason why) {}

NS_IMETHODIMP
<<<<<<< HEAD
DataChannelParent::OnStartRequest(nsIRequest *aRequest, nsISupports *aContext) {
  // We don't have a way to prevent nsBaseChannel from calling AsyncOpen on
  // the created nsDataChannel. We don't have anywhere to send the data in the
  // parent, so abort the binding.
  return NS_BINDING_ABORTED;
||||||| merged common ancestors
DataChannelParent::OnStartRequest(nsIRequest *aRequest,
                                  nsISupports *aContext)
{
    // We don't have a way to prevent nsBaseChannel from calling AsyncOpen on
    // the created nsDataChannel. We don't have anywhere to send the data in the
    // parent, so abort the binding.
    return NS_BINDING_ABORTED;
=======
DataChannelParent::OnStartRequest(nsIRequest* aRequest) {
  // We don't have a way to prevent nsBaseChannel from calling AsyncOpen on
  // the created nsDataChannel. We don't have anywhere to send the data in the
  // parent, so abort the binding.
  return NS_BINDING_ABORTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
DataChannelParent::OnStopRequest(nsIRequest *aRequest, nsISupports *aContext,
                                 nsresult aStatusCode) {
  // See above.
  MOZ_ASSERT(NS_FAILED(aStatusCode));
  return NS_OK;
||||||| merged common ancestors
DataChannelParent::OnStopRequest(nsIRequest *aRequest,
                                 nsISupports *aContext,
                                 nsresult aStatusCode)
{
    // See above.
    MOZ_ASSERT(NS_FAILED(aStatusCode));
    return NS_OK;
=======
DataChannelParent::OnStopRequest(nsIRequest* aRequest, nsresult aStatusCode) {
  // See above.
  MOZ_ASSERT(NS_FAILED(aStatusCode));
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
DataChannelParent::OnDataAvailable(nsIRequest *aRequest, nsISupports *aContext,
                                   nsIInputStream *aInputStream,
                                   uint64_t aOffset, uint32_t aCount) {
  // See above.
  MOZ_CRASH("Should never be called");
||||||| merged common ancestors
DataChannelParent::OnDataAvailable(nsIRequest *aRequest,
                                   nsISupports *aContext,
                                   nsIInputStream *aInputStream,
                                   uint64_t aOffset,
                                   uint32_t aCount)
{
    // See above.
    MOZ_CRASH("Should never be called");
=======
DataChannelParent::OnDataAvailable(nsIRequest* aRequest,
                                   nsIInputStream* aInputStream,
                                   uint64_t aOffset, uint32_t aCount) {
  // See above.
  MOZ_CRASH("Should never be called");
>>>>>>> upstream-releases
}

}  // namespace net
}  // namespace mozilla
