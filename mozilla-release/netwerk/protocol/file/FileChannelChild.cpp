/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 sw=2 sts=2 et tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "FileChannelChild.h"

#include "mozilla/Unused.h"
#include "mozilla/dom/ContentChild.h"
#include "mozilla/net/NeckoChild.h"

namespace mozilla {
namespace net {

NS_IMPL_ISUPPORTS_INHERITED(FileChannelChild, nsFileChannel, nsIChildChannel)

<<<<<<< HEAD
FileChannelChild::FileChannelChild(nsIURI *uri)
    : nsFileChannel(uri), mIPCOpen(false) {}
||||||| merged common ancestors
FileChannelChild::FileChannelChild(nsIURI *uri)
  : nsFileChannel(uri)
  , mIPCOpen(false)
{
}
=======
FileChannelChild::FileChannelChild(nsIURI* uri)
    : nsFileChannel(uri), mIPCOpen(false) {}
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
FileChannelChild::ConnectParent(uint32_t id) {
  mozilla::dom::ContentChild *cc =
      static_cast<mozilla::dom::ContentChild *>(gNeckoChild->Manager());
||||||| merged common ancestors
FileChannelChild::ConnectParent(uint32_t id)
{
  mozilla::dom::ContentChild* cc =
    static_cast<mozilla::dom::ContentChild*>(gNeckoChild->Manager());
=======
FileChannelChild::ConnectParent(uint32_t id) {
  mozilla::dom::ContentChild* cc =
      static_cast<mozilla::dom::ContentChild*>(gNeckoChild->Manager());
>>>>>>> upstream-releases
  if (cc->IsShuttingDown()) {
    return NS_ERROR_FAILURE;
  }

  if (!gNeckoChild->SendPFileChannelConstructor(this, id)) {
    return NS_ERROR_FAILURE;
  }

  AddIPDLReference();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
FileChannelChild::CompleteRedirectSetup(nsIStreamListener *listener,
                                        nsISupports *ctx) {
||||||| merged common ancestors
FileChannelChild::CompleteRedirectSetup(nsIStreamListener *listener,
                                        nsISupports *ctx)
{
=======
FileChannelChild::CompleteRedirectSetup(nsIStreamListener* listener,
                                        nsISupports* ctx) {
>>>>>>> upstream-releases
  nsresult rv;

  rv = AsyncOpen(listener);

  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

  if (mIPCOpen) {
    Unused << Send__delete__(this);
  }

  return NS_OK;
}

<<<<<<< HEAD
void FileChannelChild::AddIPDLReference() {
  AddRef();
||||||| merged common ancestors
void
FileChannelChild::AddIPDLReference()
{
  AddRef();
=======
void FileChannelChild::AddIPDLReference() {
  AddRef();  // Released in NeckoChild::DeallocPFileChannelChild.
>>>>>>> upstream-releases
  mIPCOpen = true;
}

void FileChannelChild::ActorDestroy(ActorDestroyReason why) {
  MOZ_ASSERT(mIPCOpen);
  mIPCOpen = false;
}

}  // namespace net
}  // namespace mozilla
