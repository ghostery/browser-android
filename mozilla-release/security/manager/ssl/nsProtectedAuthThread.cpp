/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "PSMRunnable.h"
#include "mozilla/Assertions.h"
#include "mozilla/DebugOnly.h"
#include "mozilla/RefPtr.h"
#include "nsCOMPtr.h"
#include "GeckoProfiler.h"
#include "nsPKCS11Slot.h"
#include "nsProtectedAuthThread.h"
#include "nsReadableUtils.h"
#include "nsString.h"
#include "nsThreadUtils.h"
#include "pk11func.h"

using namespace mozilla;
using namespace mozilla::psm;

NS_IMPL_ISUPPORTS(nsProtectedAuthThread, nsIProtectedAuthThread)

<<<<<<< HEAD
static void nsProtectedAuthThreadRunner(void *arg) {
  AUTO_PROFILER_REGISTER_THREAD("Protected Auth");
  NS_SetCurrentThreadName("Protected Auth");
||||||| merged common ancestors
static void nsProtectedAuthThreadRunner(void *arg)
{
    AUTO_PROFILER_REGISTER_THREAD("Protected Auth");
    NS_SetCurrentThreadName("Protected Auth");
=======
static void nsProtectedAuthThreadRunner(void* arg) {
  AUTO_PROFILER_REGISTER_THREAD("Protected Auth");
  NS_SetCurrentThreadName("Protected Auth");
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsProtectedAuthThread *self = static_cast<nsProtectedAuthThread *>(arg);
  self->Run();
||||||| merged common ancestors
    nsProtectedAuthThread *self = static_cast<nsProtectedAuthThread *>(arg);
    self->Run();
=======
  nsProtectedAuthThread* self = static_cast<nsProtectedAuthThread*>(arg);
  self->Run();
>>>>>>> upstream-releases
}

nsProtectedAuthThread::nsProtectedAuthThread()
    : mMutex("nsProtectedAuthThread.mMutex"),
      mIAmRunning(false),
      mLoginReady(false),
      mThreadHandle(nullptr),
      mSlot(0),
      mLoginResult(SECFailure) {}

nsProtectedAuthThread::~nsProtectedAuthThread() {}

<<<<<<< HEAD
NS_IMETHODIMP nsProtectedAuthThread::Login(nsIObserver *aObserver) {
  NS_ENSURE_ARG(aObserver);
||||||| merged common ancestors
    if (mIAmRunning || mLoginReady) {
        return NS_OK;
    }
=======
NS_IMETHODIMP nsProtectedAuthThread::Login(nsIObserver* aObserver) {
  NS_ENSURE_ARG(aObserver);
>>>>>>> upstream-releases

  if (!mSlot)
    // We need pointer to the slot
    return NS_ERROR_FAILURE;

  MutexAutoLock lock(mMutex);

  if (mIAmRunning || mLoginReady) {
    return NS_OK;
<<<<<<< HEAD
  }

  if (aObserver) {
    // We must AddRef aObserver here on the main thread, because it probably
    // does not implement a thread-safe AddRef.
    mNotifyObserver =
        new NotifyObserverRunnable(aObserver, "operation-completed");
  }

  mIAmRunning = true;

  mThreadHandle = PR_CreateThread(PR_USER_THREAD, nsProtectedAuthThreadRunner,
                                  static_cast<void *>(this), PR_PRIORITY_NORMAL,
                                  PR_GLOBAL_THREAD, PR_JOINABLE_THREAD, 0);

  // bool thread_started_ok = (threadHandle != nullptr);
  // we might want to return "thread started ok" to caller in the future
  MOZ_ASSERT(mThreadHandle,
             "Could not create nsProtectedAuthThreadRunner thread");
  return NS_OK;
||||||| merged common ancestors
=======
  }

  if (aObserver) {
    // We must AddRef aObserver here on the main thread, because it probably
    // does not implement a thread-safe AddRef.
    mNotifyObserver =
        new NotifyObserverRunnable(aObserver, "operation-completed");
  }

  mIAmRunning = true;

  mThreadHandle = PR_CreateThread(PR_USER_THREAD, nsProtectedAuthThreadRunner,
                                  static_cast<void*>(this), PR_PRIORITY_NORMAL,
                                  PR_GLOBAL_THREAD, PR_JOINABLE_THREAD, 0);

  // bool thread_started_ok = (threadHandle != nullptr);
  // we might want to return "thread started ok" to caller in the future
  MOZ_ASSERT(mThreadHandle,
             "Could not create nsProtectedAuthThreadRunner thread");
  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP nsProtectedAuthThread::GetTokenName(nsAString &_retval) {
  MutexAutoLock lock(mMutex);
||||||| merged common ancestors
NS_IMETHODIMP nsProtectedAuthThread::GetTokenName(nsAString &_retval)
{
    MutexAutoLock lock(mMutex);

    // Get token name
    CopyUTF8toUTF16(nsDependentCString(PK11_GetTokenName(mSlot)), _retval);
=======
NS_IMETHODIMP nsProtectedAuthThread::GetTokenName(nsAString& _retval) {
  MutexAutoLock lock(mMutex);
>>>>>>> upstream-releases

  // Get token name
  CopyUTF8toUTF16(nsDependentCString(PK11_GetTokenName(mSlot)), _retval);

  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsProtectedAuthThread::GetSlot(nsIPKCS11Slot **_retval) {
  RefPtr<nsPKCS11Slot> slot;
  {
||||||| merged common ancestors
void nsProtectedAuthThread::SetParams(PK11SlotInfo* aSlot)
{
=======
NS_IMETHODIMP nsProtectedAuthThread::GetSlot(nsIPKCS11Slot** _retval) {
  RefPtr<nsPKCS11Slot> slot;
  {
>>>>>>> upstream-releases
    MutexAutoLock lock(mMutex);
    slot = new nsPKCS11Slot(mSlot);
  }

  slot.forget(_retval);
  return NS_OK;
}

<<<<<<< HEAD
void nsProtectedAuthThread::SetParams(PK11SlotInfo *aSlot) {
  MutexAutoLock lock(mMutex);

  mSlot = (aSlot) ? PK11_ReferenceSlot(aSlot) : 0;
||||||| merged common ancestors
SECStatus nsProtectedAuthThread::GetResult()
{
    return mLoginResult;
=======
void nsProtectedAuthThread::SetParams(PK11SlotInfo* aSlot) {
  MutexAutoLock lock(mMutex);

  mSlot = (aSlot) ? PK11_ReferenceSlot(aSlot) : 0;
>>>>>>> upstream-releases
}

SECStatus nsProtectedAuthThread::GetResult() { return mLoginResult; }

void nsProtectedAuthThread::Run(void) {
  // Login with null password. This call will also do C_Logout() but
  // it is harmless here
  mLoginResult = PK11_CheckUserPassword(mSlot, 0);

  nsCOMPtr<nsIRunnable> notifyObserver;
  {
    MutexAutoLock lock(mMutex);

    mLoginReady = true;
    mIAmRunning = false;

    // Forget the slot
    if (mSlot) {
      PK11_FreeSlot(mSlot);
      mSlot = 0;
    }

    notifyObserver.swap(mNotifyObserver);
  }

  if (notifyObserver) {
    DebugOnly<nsresult> rv = NS_DispatchToMainThread(notifyObserver);
    MOZ_ASSERT(NS_SUCCEEDED(rv),
               "Failed to dispatch protected auth observer to main thread");
  }
}

void nsProtectedAuthThread::Join() {
  if (!mThreadHandle) return;

  PR_JoinThread(mThreadHandle);
  mThreadHandle = nullptr;
}
