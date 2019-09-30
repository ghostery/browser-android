/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=4 sw=2 sts=2 et tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "DataChannelChild.h"

#include "mozilla/Unused.h"
#include "mozilla/dom/ContentChild.h"
#include "mozilla/net/NeckoChild.h"

namespace mozilla {
namespace net {

NS_IMPL_ISUPPORTS_INHERITED(DataChannelChild, nsDataChannel, nsIChildChannel)

DataChannelChild::DataChannelChild(nsIURI* aURI)
    : nsDataChannel(aURI), mIPCOpen(false) {}

NS_IMETHODIMP
DataChannelChild::ConnectParent(uint32_t aId) {
  mozilla::dom::ContentChild* cc =
      static_cast<mozilla::dom::ContentChild*>(gNeckoChild->Manager());
  if (cc->IsShuttingDown()) {
    return NS_ERROR_FAILURE;
  }

  if (!gNeckoChild->SendPDataChannelConstructor(this, aId)) {
    return NS_ERROR_FAILURE;
  }

  // IPC now has a ref to us.
  AddIPDLReference();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
DataChannelChild::CompleteRedirectSetup(nsIStreamListener* aListener,
                                        nsISupports* aContext) {
  nsresult rv;
  if (mLoadInfo && mLoadInfo->GetEnforceSecurity()) {
    MOZ_ASSERT(!aContext, "aContext should be null!");
    rv = AsyncOpen2(aListener);
  } else {
    rv = AsyncOpen(aListener, aContext);
  }
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
||||||| merged common ancestors
DataChannelChild::CompleteRedirectSetup(nsIStreamListener *aListener,
                                        nsISupports *aContext)
{
    nsresult rv;
    if (mLoadInfo && mLoadInfo->GetEnforceSecurity()) {
        MOZ_ASSERT(!aContext, "aContext should be null!");
        rv = AsyncOpen2(aListener);
    }
    else {
        rv = AsyncOpen(aListener, aContext);
    }
    if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
    }
=======
DataChannelChild::CompleteRedirectSetup(nsIStreamListener* aListener,
                                        nsISupports* aContext) {
  nsresult rv;
  MOZ_ASSERT(!aContext, "aContext should be null!");
  rv = AsyncOpen(aListener);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
>>>>>>> upstream-releases

  if (mIPCOpen) {
    Unused << Send__delete__(this);
  }
  return NS_OK;
}

<<<<<<< HEAD
void DataChannelChild::AddIPDLReference() {
  AddRef();
  mIPCOpen = true;
||||||| merged common ancestors
void
DataChannelChild::AddIPDLReference()
{
    AddRef();
    mIPCOpen = true;
=======
void DataChannelChild::AddIPDLReference() {
  AddRef();  // Released in NeckoChild::DeallocPDataChannelChild.
  mIPCOpen = true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void DataChannelChild::ActorDestroy(ActorDestroyReason why) {
  MOZ_ASSERT(mIPCOpen);
  mIPCOpen = false;
  Release();
||||||| merged common ancestors
void
DataChannelChild::ActorDestroy(ActorDestroyReason why)
{
    MOZ_ASSERT(mIPCOpen);
    mIPCOpen = false;
    Release();
=======
void DataChannelChild::ActorDestroy(ActorDestroyReason why) {
  MOZ_ASSERT(mIPCOpen);
  mIPCOpen = false;
>>>>>>> upstream-releases
}

}  // namespace net
}  // namespace mozilla
