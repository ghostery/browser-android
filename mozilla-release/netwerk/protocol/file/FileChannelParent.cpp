/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 sw=2 sts=2 et tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "FileChannelParent.h"
#include "mozilla/Assertions.h"
#include "nsNetUtil.h"
#include "nsIChannel.h"

namespace mozilla {
namespace net {

NS_IMPL_ISUPPORTS(FileChannelParent, nsIParentChannel, nsIStreamListener)

<<<<<<< HEAD
bool FileChannelParent::Init(const uint32_t &channelId) {
||||||| merged common ancestors
bool
FileChannelParent::Init(const uint32_t &channelId)
{
=======
bool FileChannelParent::Init(const uint32_t& channelId) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIChannel> channel;
  MOZ_ALWAYS_SUCCEEDS(
      NS_LinkRedirectChannels(channelId, this, getter_AddRefs(channel)));

  return true;
}

NS_IMETHODIMP
<<<<<<< HEAD
FileChannelParent::SetParentListener(HttpChannelParentListener *aListener) {
||||||| merged common ancestors
FileChannelParent::SetParentListener(HttpChannelParentListener* aListener)
{
=======
FileChannelParent::SetParentListener(HttpChannelParentListener* aListener) {
>>>>>>> upstream-releases
  // Nothing to do.
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
FileChannelParent::NotifyTrackingProtectionDisabled() {
||||||| merged common ancestors
FileChannelParent::NotifyTrackingProtectionDisabled()
{
=======
FileChannelParent::NotifyChannelClassifierProtectionDisabled(
    uint32_t aAcceptedReason) {
>>>>>>> upstream-releases
  // Nothing to do.
  return NS_OK;
}

NS_IMETHODIMP
FileChannelParent::NotifyCookieAllowed() {
  // Nothing to do.
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
FileChannelParent::NotifyTrackingCookieBlocked(uint32_t aRejectedReason) {
||||||| merged common ancestors
FileChannelParent::NotifyTrackingResource(bool aIsThirdParty)
{
=======
FileChannelParent::NotifyCookieBlocked(uint32_t aRejectedReason) {
  // Nothing to do.
  return NS_OK;
}

NS_IMETHODIMP
FileChannelParent::NotifyClassificationFlags(uint32_t aClassificationFlags,
                                             bool aIsThirdParty) {
  // Nothing to do.
  return NS_OK;
}

NS_IMETHODIMP
FileChannelParent::NotifyFlashPluginStateChanged(
    nsIHttpChannel::FlashPluginState aState) {
>>>>>>> upstream-releases
  // Nothing to do.
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
FileChannelParent::NotifyTrackingResource(bool aIsThirdParty) {
  // Nothing to do.
  return NS_OK;
}

NS_IMETHODIMP
FileChannelParent::SetClassifierMatchedInfo(const nsACString &aList,
                                            const nsACString &aProvider,
                                            const nsACString &aFullHash) {
||||||| merged common ancestors
FileChannelParent::SetClassifierMatchedInfo(const nsACString& aList,
                                            const nsACString& aProvider,
                                            const nsACString& aFullHash)
{
=======
FileChannelParent::SetClassifierMatchedInfo(const nsACString& aList,
                                            const nsACString& aProvider,
                                            const nsACString& aFullHash) {
>>>>>>> upstream-releases
  // nothing to do
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
FileChannelParent::Delete() {
  // Nothing to do.
||||||| merged common ancestors
FileChannelParent::Delete()
{
  // Nothing to do.
=======
FileChannelParent::SetClassifierMatchedTrackingInfo(
    const nsACString& aLists, const nsACString& aFullHashes) {
  // nothing to do
>>>>>>> upstream-releases
  return NS_OK;
}

<<<<<<< HEAD
void FileChannelParent::ActorDestroy(ActorDestroyReason why) {}
||||||| merged common ancestors
void
FileChannelParent::ActorDestroy(ActorDestroyReason why)
{
}
=======
NS_IMETHODIMP
FileChannelParent::Delete() {
  // Nothing to do.
  return NS_OK;
}
>>>>>>> upstream-releases

void FileChannelParent::ActorDestroy(ActorDestroyReason why) {}

NS_IMETHODIMP
<<<<<<< HEAD
FileChannelParent::OnStartRequest(nsIRequest *aRequest, nsISupports *aContext) {
||||||| merged common ancestors
FileChannelParent::OnStartRequest(nsIRequest *aRequest,
                                  nsISupports *aContext)
{
=======
FileChannelParent::OnStartRequest(nsIRequest* aRequest) {
>>>>>>> upstream-releases
  // We don't have a way to prevent nsBaseChannel from calling AsyncOpen on
  // the created nsDataChannel. We don't have anywhere to send the data in the
  // parent, so abort the binding.
  return NS_BINDING_ABORTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
FileChannelParent::OnStopRequest(nsIRequest *aRequest, nsISupports *aContext,
                                 nsresult aStatusCode) {
||||||| merged common ancestors
FileChannelParent::OnStopRequest(nsIRequest *aRequest,
                                 nsISupports *aContext,
                                 nsresult aStatusCode)
{
=======
FileChannelParent::OnStopRequest(nsIRequest* aRequest, nsresult aStatusCode) {
>>>>>>> upstream-releases
  // See above.
  MOZ_ASSERT(NS_FAILED(aStatusCode));
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
FileChannelParent::OnDataAvailable(nsIRequest *aRequest, nsISupports *aContext,
                                   nsIInputStream *aInputStream,
                                   uint64_t aOffset, uint32_t aCount) {
||||||| merged common ancestors
FileChannelParent::OnDataAvailable(nsIRequest *aRequest,
                                   nsISupports *aContext,
                                   nsIInputStream *aInputStream,
                                   uint64_t aOffset,
                                   uint32_t aCount)
{
=======
FileChannelParent::OnDataAvailable(nsIRequest* aRequest,
                                   nsIInputStream* aInputStream,
                                   uint64_t aOffset, uint32_t aCount) {
>>>>>>> upstream-releases
  // See above.
  MOZ_CRASH("Should never be called");
}

}  // namespace net
}  // namespace mozilla
