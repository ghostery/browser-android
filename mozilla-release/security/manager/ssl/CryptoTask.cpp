/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "CryptoTask.h"
#include "nsNSSComponent.h"
#include "nsNetCID.h"

namespace mozilla {

<<<<<<< HEAD
nsresult CryptoTask::Dispatch(const nsACString& taskThreadName) {
  MOZ_ASSERT(taskThreadName.Length() <= 15);

||||||| merged common ancestors
nsresult
CryptoTask::Dispatch(const nsACString& taskThreadName)
{
  MOZ_ASSERT(taskThreadName.Length() <= 15);

=======
nsresult CryptoTask::Dispatch() {
>>>>>>> upstream-releases
  // Ensure that NSS is initialized, since presumably CalculateResult
  // will use NSS functions
  if (!EnsureNSSInitializedChromeOrContent()) {
    return NS_ERROR_FAILURE;
  }

<<<<<<< HEAD
  // Can't add 'this' as the event to run, since mThread may not be set yet
  nsresult rv =
      NS_NewNamedThread(taskThreadName, getter_AddRefs(mThread), nullptr,
                        nsIThreadManager::DEFAULT_STACK_SIZE);
  if (NS_FAILED(rv)) {
    return rv;
||||||| merged common ancestors
  // Can't add 'this' as the event to run, since mThread may not be set yet
  nsresult rv = NS_NewNamedThread(taskThreadName, getter_AddRefs(mThread),
                                  nullptr,
                                  nsIThreadManager::DEFAULT_STACK_SIZE);
  if (NS_FAILED(rv)) {
    return rv;
=======
  // The stream transport service (note: not the socket transport service) can
  // be used to perform background tasks or I/O that would otherwise block the
  // main thread.
  nsCOMPtr<nsIEventTarget> target(
      do_GetService(NS_STREAMTRANSPORTSERVICE_CONTRACTID));
  if (!target) {
    return NS_ERROR_FAILURE;
>>>>>>> upstream-releases
  }
  return target->Dispatch(this, NS_DISPATCH_NORMAL);
}

NS_IMETHODIMP
CryptoTask::Run() {
  if (!NS_IsMainThread()) {
    mRv = CalculateResult();
    NS_DispatchToMainThread(this);
  } else {
    // back on the main thread
    CallCallback(mRv);
<<<<<<< HEAD

    // Not all uses of CryptoTask use a transient thread
    if (mThread) {
      // Don't leak threads!
      mThread->Shutdown();  // can't Shutdown from the thread itself, darn
      // Don't null out mThread!
      // See bug 999104.  We must hold a ref to the thread across Dispatch()
      // since the internal mThread ref could be released while processing
      // the Dispatch(), and Dispatch/PutEvent itself doesn't hold a ref; it
      // assumes the caller does.
    }
||||||| merged common ancestors

    // Not all uses of CryptoTask use a transient thread
    if (mThread) {
      // Don't leak threads!
      mThread->Shutdown(); // can't Shutdown from the thread itself, darn
      // Don't null out mThread!
      // See bug 999104.  We must hold a ref to the thread across Dispatch()
      // since the internal mThread ref could be released while processing
      // the Dispatch(), and Dispatch/PutEvent itself doesn't hold a ref; it
      // assumes the caller does.
    }
=======
>>>>>>> upstream-releases
  }
  return NS_OK;
}

}  // namespace mozilla
