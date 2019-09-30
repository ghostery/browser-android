/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/net/AltDataOutputStreamChild.h"
#include "mozilla/Unused.h"
#include "nsIInputStream.h"
#include "nsStreamUtils.h"

namespace mozilla {
namespace net {

NS_IMPL_ADDREF(AltDataOutputStreamChild)

NS_IMETHODIMP_(MozExternalRefCountType) AltDataOutputStreamChild::Release() {
  MOZ_ASSERT(0 != mRefCnt, "dup release");
  MOZ_ASSERT(NS_IsMainThread(), "Main thread only");
  --mRefCnt;
  NS_LOG_RELEASE(this, mRefCnt, "AltDataOutputStreamChild");

  if (mRefCnt == 1 && mIPCOpen) {
    // The only reference left is the IPDL one. After the parent replies back
    // with a DeleteSelf message, the child will call Send__delete__(this),
    // decrementing the ref count and triggering the destructor.
    SendDeleteSelf();
    return 1;
  }

  if (mRefCnt == 0) {
    mRefCnt = 1; /* stabilize */
    delete this;
    return 0;
  }
  return mRefCnt;
}

NS_INTERFACE_MAP_BEGIN(AltDataOutputStreamChild)
  NS_INTERFACE_MAP_ENTRY(nsIAsyncOutputStream)
  NS_INTERFACE_MAP_ENTRY(nsIOutputStream)
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

AltDataOutputStreamChild::AltDataOutputStreamChild()
<<<<<<< HEAD
    : mIPCOpen(false), mError(NS_OK) {
||||||| merged common ancestors
  : mIPCOpen(false)
  , mError(NS_OK)
{
=======
    : mIPCOpen(false), mError(NS_OK), mCallbackFlags(0) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread(), "Main thread only");
}

void AltDataOutputStreamChild::AddIPDLReference() {
  MOZ_ASSERT(!mIPCOpen, "Attempt to retain more than one IPDL reference");
  mIPCOpen = true;
  AddRef();
}

void AltDataOutputStreamChild::ReleaseIPDLReference() {
  MOZ_ASSERT(mIPCOpen, "Attempt to release nonexistent IPDL reference");
  mIPCOpen = false;

  if (mCallback) {
    NotifyListener();
  }

  Release();
}

<<<<<<< HEAD
bool AltDataOutputStreamChild::WriteDataInChunks(
    const nsDependentCSubstring &data) {
  const uint32_t kChunkSize = 128 * 1024;
||||||| merged common ancestors
bool
AltDataOutputStreamChild::WriteDataInChunks(const nsDependentCSubstring& data)
{
  const uint32_t kChunkSize = 128*1024;
=======
bool AltDataOutputStreamChild::WriteDataInChunks(
    const nsDependentCSubstring& data) {
  const uint32_t kChunkSize = 128 * 1024;
>>>>>>> upstream-releases
  uint32_t next = std::min(data.Length(), kChunkSize);
  for (uint32_t i = 0; i < data.Length();
       i = next, next = std::min(data.Length(), next + kChunkSize)) {
    nsCString chunk(Substring(data, i, kChunkSize));
    if (mIPCOpen && !SendWriteData(chunk)) {
      mIPCOpen = false;
      return false;
    }
  }
  return true;
}

NS_IMETHODIMP
<<<<<<< HEAD
AltDataOutputStreamChild::Close() {
  if (!mIPCOpen) {
    return NS_ERROR_NOT_AVAILABLE;
  }
  if (NS_FAILED(mError)) {
    return mError;
  }
  Unused << SendClose();
  return NS_OK;
}
||||||| merged common ancestors
AltDataOutputStreamChild::Close()
{
  if (!mIPCOpen) {
    return NS_ERROR_NOT_AVAILABLE;
  }
  if (NS_FAILED(mError)) {
    return mError;
  }
  Unused << SendClose();
  return NS_OK;
}
=======
AltDataOutputStreamChild::Close() { return CloseWithStatus(NS_OK); }
>>>>>>> upstream-releases

NS_IMETHODIMP
AltDataOutputStreamChild::Flush() {
  if (!mIPCOpen) {
    return NS_ERROR_NOT_AVAILABLE;
  }
  if (NS_FAILED(mError)) {
    return mError;
  }

  // This is a no-op
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
AltDataOutputStreamChild::Write(const char *aBuf, uint32_t aCount,
                                uint32_t *_retval) {
||||||| merged common ancestors
AltDataOutputStreamChild::Write(const char * aBuf, uint32_t aCount, uint32_t *_retval)
{
=======
AltDataOutputStreamChild::Write(const char* aBuf, uint32_t aCount,
                                uint32_t* _retval) {
>>>>>>> upstream-releases
  if (!mIPCOpen) {
    return NS_ERROR_NOT_AVAILABLE;
  }
  if (NS_FAILED(mError)) {
    return mError;
  }
  if (WriteDataInChunks(nsDependentCSubstring(aBuf, aCount))) {
    *_retval = aCount;
    return NS_OK;
  }
  return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
<<<<<<< HEAD
AltDataOutputStreamChild::WriteFrom(nsIInputStream *aFromStream,
                                    uint32_t aCount, uint32_t *_retval) {
||||||| merged common ancestors
AltDataOutputStreamChild::WriteFrom(nsIInputStream *aFromStream, uint32_t aCount, uint32_t *_retval)
{
=======
AltDataOutputStreamChild::WriteFrom(nsIInputStream* aFromStream,
                                    uint32_t aCount, uint32_t* _retval) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
AltDataOutputStreamChild::WriteSegments(nsReadSegmentFun aReader,
                                        void *aClosure, uint32_t aCount,
                                        uint32_t *_retval) {
||||||| merged common ancestors
AltDataOutputStreamChild::WriteSegments(nsReadSegmentFun aReader, void *aClosure, uint32_t aCount, uint32_t *_retval)
{
=======
AltDataOutputStreamChild::WriteSegments(nsReadSegmentFun aReader,
                                        void* aClosure, uint32_t aCount,
                                        uint32_t* _retval) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
AltDataOutputStreamChild::IsNonBlocking(bool *_retval) {
||||||| merged common ancestors
AltDataOutputStreamChild::IsNonBlocking(bool *_retval)
{
=======
AltDataOutputStreamChild::IsNonBlocking(bool* _retval) {
>>>>>>> upstream-releases
  *_retval = false;
  return NS_OK;
}

<<<<<<< HEAD
mozilla::ipc::IPCResult AltDataOutputStreamChild::RecvError(
    const nsresult &err) {
||||||| merged common ancestors
mozilla::ipc::IPCResult
AltDataOutputStreamChild::RecvError(const nsresult& err)
{
=======
mozilla::ipc::IPCResult AltDataOutputStreamChild::RecvError(
    const nsresult& err) {
>>>>>>> upstream-releases
  mError = err;
  return IPC_OK();
}

mozilla::ipc::IPCResult AltDataOutputStreamChild::RecvDeleteSelf() {
  PAltDataOutputStreamChild::Send__delete__(this);
  return IPC_OK();
}

<<<<<<< HEAD
}  // namespace net
}  // namespace mozilla
||||||| merged common ancestors
} // namespace net
} // namespace mozilla
=======
// nsIAsyncOutputStream

NS_IMETHODIMP
AltDataOutputStreamChild::CloseWithStatus(nsresult aStatus) {
  if (!mIPCOpen) {
    return NS_ERROR_NOT_AVAILABLE;
  }
  if (NS_FAILED(mError)) {
    return mError;
  }
  Unused << SendClose(aStatus);

  return NS_OK;
}

NS_IMETHODIMP
AltDataOutputStreamChild::AsyncWait(nsIOutputStreamCallback* aCallback,
                                    uint32_t aFlags, uint32_t aRequestedCount,
                                    nsIEventTarget* aEventTarget) {
  mCallback = aCallback;
  mCallbackFlags = aFlags;
  mCallbackTarget = aEventTarget;

  if (!mCallback) {
    return NS_OK;
  }

  // The stream is blocking so it is writable at any time
  if (!mIPCOpen || !(aFlags & WAIT_CLOSURE_ONLY)) {
    NotifyListener();
  }

  return NS_OK;
}

void AltDataOutputStreamChild::NotifyListener() {
  MOZ_ASSERT(mCallback);

  if (!mCallbackTarget) {
    mCallbackTarget = GetMainThreadEventTarget();
  }

  nsCOMPtr<nsIOutputStreamCallback> asyncCallback =
      NS_NewOutputStreamReadyEvent(mCallback, mCallbackTarget);

  mCallback = nullptr;
  mCallbackTarget = nullptr;

  asyncCallback->OnOutputStreamReady(this);
}

}  // namespace net
}  // namespace mozilla
>>>>>>> upstream-releases
