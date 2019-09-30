/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsPreloadedStream.h"
#include "nsIRunnable.h"

#include "nsThreadUtils.h"
#include <algorithm>

namespace mozilla {
namespace net {

<<<<<<< HEAD
NS_IMPL_ISUPPORTS(nsPreloadedStream, nsIInputStream, nsIAsyncInputStream)

nsPreloadedStream::nsPreloadedStream(nsIAsyncInputStream *aStream,
                                     const char *data, uint32_t datalen)
    : mStream(aStream), mOffset(0), mLen(datalen) {
  mBuf = (char *)moz_xmalloc(datalen);
  memcpy(mBuf, data, datalen);
}
||||||| merged common ancestors
NS_IMPL_ISUPPORTS(nsPreloadedStream,
                  nsIInputStream,
                  nsIAsyncInputStream)

nsPreloadedStream::nsPreloadedStream(nsIAsyncInputStream *aStream,
                                     const char *data, uint32_t datalen)
    : mStream(aStream),
      mOffset(0),
      mLen(datalen)
{
    mBuf = (char *) moz_xmalloc(datalen);
    memcpy(mBuf, data, datalen);
}
=======
NS_IMPL_ISUPPORTS(nsPreloadedStream, nsIInputStream, nsIAsyncInputStream)
>>>>>>> upstream-releases

<<<<<<< HEAD
nsPreloadedStream::~nsPreloadedStream() { free(mBuf); }
||||||| merged common ancestors
nsPreloadedStream::~nsPreloadedStream()
{
    free(mBuf);
}
=======
nsPreloadedStream::nsPreloadedStream(nsIAsyncInputStream* aStream,
                                     const char* data, uint32_t datalen)
    : mStream(aStream), mOffset(0), mLen(datalen) {
  mBuf = (char*)moz_xmalloc(datalen);
  memcpy(mBuf, data, datalen);
}
>>>>>>> upstream-releases

nsPreloadedStream::~nsPreloadedStream() { free(mBuf); }

NS_IMETHODIMP
nsPreloadedStream::Close() {
  mLen = 0;
  return mStream->Close();
}

<<<<<<< HEAD
NS_IMETHODIMP
nsPreloadedStream::Available(uint64_t *_retval) {
  uint64_t avail = 0;

  nsresult rv = mStream->Available(&avail);
  if (NS_FAILED(rv)) return rv;
  *_retval = avail + mLen;
  return NS_OK;
}
||||||| merged common ancestors
=======
NS_IMETHODIMP
nsPreloadedStream::Available(uint64_t* _retval) {
  uint64_t avail = 0;

  nsresult rv = mStream->Available(&avail);
  if (NS_FAILED(rv)) return rv;
  *_retval = avail + mLen;
  return NS_OK;
}
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
nsPreloadedStream::Read(char *aBuf, uint32_t aCount, uint32_t *_retval) {
  if (!mLen) return mStream->Read(aBuf, aCount, _retval);

  uint32_t toRead = std::min(mLen, aCount);
  memcpy(aBuf, mBuf + mOffset, toRead);
  mOffset += toRead;
  mLen -= toRead;
  *_retval = toRead;
  return NS_OK;
||||||| merged common ancestors
nsPreloadedStream::Available(uint64_t *_retval)
{
    uint64_t avail = 0;

    nsresult rv = mStream->Available(&avail);
    if (NS_FAILED(rv))
        return rv;
    *_retval = avail + mLen;
    return NS_OK;
=======
nsPreloadedStream::Read(char* aBuf, uint32_t aCount, uint32_t* _retval) {
  if (!mLen) return mStream->Read(aBuf, aCount, _retval);

  uint32_t toRead = std::min(mLen, aCount);
  memcpy(aBuf, mBuf + mOffset, toRead);
  mOffset += toRead;
  mLen -= toRead;
  *_retval = toRead;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsPreloadedStream::ReadSegments(nsWriteSegmentFun aWriter, void *aClosure,
                                uint32_t aCount, uint32_t *result) {
  if (!mLen) return mStream->ReadSegments(aWriter, aClosure, aCount, result);
||||||| merged common ancestors
nsPreloadedStream::Read(char *aBuf, uint32_t aCount,
                        uint32_t *_retval)
{
    if (!mLen)
        return mStream->Read(aBuf, aCount, _retval);
=======
nsPreloadedStream::ReadSegments(nsWriteSegmentFun aWriter, void* aClosure,
                                uint32_t aCount, uint32_t* result) {
  if (!mLen) return mStream->ReadSegments(aWriter, aClosure, aCount, result);
>>>>>>> upstream-releases

  *result = 0;
  while (mLen > 0 && aCount > 0) {
    uint32_t toRead = std::min(mLen, aCount);
    uint32_t didRead = 0;
    nsresult rv;

    rv = aWriter(this, aClosure, mBuf + mOffset, *result, toRead, &didRead);

    if (NS_FAILED(rv)) return NS_OK;

    *result += didRead;
    mOffset += didRead;
    mLen -= didRead;
    aCount -= didRead;
  }

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsPreloadedStream::IsNonBlocking(bool *_retval) {
  return mStream->IsNonBlocking(_retval);
||||||| merged common ancestors
nsPreloadedStream::IsNonBlocking(bool *_retval)
{
    return mStream->IsNonBlocking(_retval);
=======
nsPreloadedStream::IsNonBlocking(bool* _retval) {
  return mStream->IsNonBlocking(_retval);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
nsPreloadedStream::CloseWithStatus(nsresult aStatus) {
  mLen = 0;
  return mStream->CloseWithStatus(aStatus);
}

<<<<<<< HEAD
class RunOnThread : public Runnable {
 public:
  RunOnThread(nsIAsyncInputStream *aStream, nsIInputStreamCallback *aCallback)
      : Runnable("net::RunOnThread"), mStream(aStream), mCallback(aCallback) {}
||||||| merged common ancestors
class RunOnThread : public Runnable
{
public:
  RunOnThread(nsIAsyncInputStream* aStream, nsIInputStreamCallback* aCallback)
    : Runnable("net::RunOnThread")
    , mStream(aStream)
    , mCallback(aCallback)
  {
  }
=======
class RunOnThread : public Runnable {
 public:
  RunOnThread(nsIAsyncInputStream* aStream, nsIInputStreamCallback* aCallback)
      : Runnable("net::RunOnThread"), mStream(aStream), mCallback(aCallback) {}
>>>>>>> upstream-releases

  virtual ~RunOnThread() = default;

  NS_IMETHOD Run() override {
    mCallback->OnInputStreamReady(mStream);
    return NS_OK;
  }

 private:
  nsCOMPtr<nsIAsyncInputStream> mStream;
  nsCOMPtr<nsIInputStreamCallback> mCallback;
};

NS_IMETHODIMP
<<<<<<< HEAD
nsPreloadedStream::AsyncWait(nsIInputStreamCallback *aCallback, uint32_t aFlags,
||||||| merged common ancestors
nsPreloadedStream::AsyncWait(nsIInputStreamCallback *aCallback,
                             uint32_t aFlags,
=======
nsPreloadedStream::AsyncWait(nsIInputStreamCallback* aCallback, uint32_t aFlags,
>>>>>>> upstream-releases
                             uint32_t aRequestedCount,
<<<<<<< HEAD
                             nsIEventTarget *aEventTarget) {
  if (!mLen)
    return mStream->AsyncWait(aCallback, aFlags, aRequestedCount, aEventTarget);
||||||| merged common ancestors
                             nsIEventTarget *aEventTarget)
{
    if (!mLen)
        return mStream->AsyncWait(aCallback, aFlags, aRequestedCount,
                                  aEventTarget);
=======
                             nsIEventTarget* aEventTarget) {
  if (!mLen)
    return mStream->AsyncWait(aCallback, aFlags, aRequestedCount, aEventTarget);
>>>>>>> upstream-releases

  if (!aCallback) return NS_OK;

  if (!aEventTarget) return aCallback->OnInputStreamReady(this);

  nsCOMPtr<nsIRunnable> event = new RunOnThread(this, aCallback);
  return aEventTarget->Dispatch(event, nsIEventTarget::DISPATCH_NORMAL);
}

}  // namespace net
}  // namespace mozilla
