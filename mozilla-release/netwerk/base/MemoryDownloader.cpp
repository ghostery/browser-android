/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MemoryDownloader.h"

#include "mozilla/Assertions.h"
#include "nsIInputStream.h"

namespace mozilla {
namespace net {

NS_IMPL_ISUPPORTS(MemoryDownloader, nsIStreamListener, nsIRequestObserver)

MemoryDownloader::MemoryDownloader(IObserver* aObserver)
    : mObserver(aObserver), mStatus(NS_ERROR_NOT_INITIALIZED) {}

NS_IMETHODIMP
<<<<<<< HEAD
MemoryDownloader::OnStartRequest(nsIRequest* aRequest, nsISupports* aCtxt) {
||||||| merged common ancestors
MemoryDownloader::OnStartRequest(nsIRequest* aRequest, nsISupports* aCtxt)
{
=======
MemoryDownloader::OnStartRequest(nsIRequest* aRequest) {
>>>>>>> upstream-releases
  MOZ_ASSERT(!mData);
  mData.reset(new FallibleTArray<uint8_t>());
  mStatus = NS_OK;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
MemoryDownloader::OnStopRequest(nsIRequest* aRequest, nsISupports* aCtxt,
                                nsresult aStatus) {
||||||| merged common ancestors
MemoryDownloader::OnStopRequest(nsIRequest* aRequest,
                                nsISupports* aCtxt,
                                nsresult aStatus)
{
=======
MemoryDownloader::OnStopRequest(nsIRequest* aRequest, nsresult aStatus) {
>>>>>>> upstream-releases
  MOZ_ASSERT_IF(NS_FAILED(mStatus), NS_FAILED(aStatus));
  MOZ_ASSERT(!mData == NS_FAILED(mStatus));
  Data data;
  data.swap(mData);
  RefPtr<IObserver> observer;
  observer.swap(mObserver);
<<<<<<< HEAD
  observer->OnDownloadComplete(this, aRequest, aCtxt, aStatus, std::move(data));
||||||| merged common ancestors
  observer->OnDownloadComplete(this, aRequest, aCtxt, aStatus,
                               std::move(data));
=======
  observer->OnDownloadComplete(this, aRequest, nullptr, aStatus,
                               std::move(data));
>>>>>>> upstream-releases
  return NS_OK;
}

nsresult MemoryDownloader::ConsumeData(nsIInputStream* aIn, void* aClosure,
                                       const char* aFromRawSegment,
                                       uint32_t aToOffset, uint32_t aCount,
                                       uint32_t* aWriteCount) {
  MemoryDownloader* self = static_cast<MemoryDownloader*>(aClosure);
  if (!self->mData->AppendElements(aFromRawSegment, aCount, fallible)) {
    // The error returned by ConsumeData isn't propagated to the
    // return of ReadSegments, so it has to be passed as state.
    self->mStatus = NS_ERROR_OUT_OF_MEMORY;
    return NS_ERROR_OUT_OF_MEMORY;
  }
  *aWriteCount = aCount;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
MemoryDownloader::OnDataAvailable(nsIRequest* aRequest, nsISupports* aCtxt,
                                  nsIInputStream* aInStr,
                                  uint64_t aSourceOffset, uint32_t aCount) {
||||||| merged common ancestors
MemoryDownloader::OnDataAvailable(nsIRequest* aRequest,
                                  nsISupports* aCtxt,
                                  nsIInputStream* aInStr,
                                  uint64_t aSourceOffset,
                                  uint32_t aCount)
{
=======
MemoryDownloader::OnDataAvailable(nsIRequest* aRequest, nsIInputStream* aInStr,
                                  uint64_t aSourceOffset, uint32_t aCount) {
>>>>>>> upstream-releases
  uint32_t n;
  MOZ_ASSERT(mData);
  nsresult rv = aInStr->ReadSegments(ConsumeData, this, aCount, &n);
  if (NS_SUCCEEDED(mStatus) && NS_FAILED(rv)) {
    mStatus = rv;
  }
  if (NS_WARN_IF(NS_FAILED(mStatus))) {
    mData.reset(nullptr);
    return mStatus;
  }
  return NS_OK;
}

}  // namespace net
}  // namespace mozilla
