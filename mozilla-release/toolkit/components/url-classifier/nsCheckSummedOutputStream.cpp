//* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsCRT.h"
#include "nsIFile.h"
#include "nsISupportsImpl.h"
#include "nsCheckSummedOutputStream.h"
#include "crc32c.h"

////////////////////////////////////////////////////////////////////////////////
// nsCheckSummedOutputStream

NS_IMPL_ISUPPORTS_INHERITED(nsCheckSummedOutputStream, nsBufferedOutputStream,
                            nsISafeOutputStream)

NS_IMETHODIMP
<<<<<<< HEAD
nsCheckSummedOutputStream::Init(nsIOutputStream *stream, uint32_t bufferSize) {
||||||| merged common ancestors
nsCheckSummedOutputStream::Init(nsIOutputStream* stream, uint32_t bufferSize)
{
=======
nsCheckSummedOutputStream::Init(nsIOutputStream* stream, uint32_t bufferSize) {
>>>>>>> upstream-releases
  nsresult rv;
  mHash = do_CreateInstance(NS_CRYPTO_HASH_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = mHash->Init(nsICryptoHash::MD5);
  NS_ENSURE_SUCCESS(rv, rv);

  return nsBufferedOutputStream::Init(stream, bufferSize);
}

NS_IMETHODIMP
nsCheckSummedOutputStream::Finish() {
  nsresult rv = mHash->Finish(false, mCheckSum);
  NS_ENSURE_SUCCESS(rv, rv);

  uint32_t written;
<<<<<<< HEAD
  rv = nsBufferedOutputStream::Write(
      reinterpret_cast<const char *>(mCheckSum.BeginReading()),
      mCheckSum.Length(), &written);
||||||| merged common ancestors
  rv = nsBufferedOutputStream::Write(reinterpret_cast<const char*>(mCheckSum.BeginReading()),
                                     mCheckSum.Length(), &written);
=======
  rv = nsBufferedOutputStream::Write(
      reinterpret_cast<const char*>(mCheckSum.BeginReading()),
      mCheckSum.Length(), &written);
>>>>>>> upstream-releases
  NS_ASSERTION(written == mCheckSum.Length(), "Error writing stream checksum");
  NS_ENSURE_SUCCESS(rv, rv);

  return nsBufferedOutputStream::Finish();
}

NS_IMETHODIMP
<<<<<<< HEAD
nsCheckSummedOutputStream::Write(const char *buf, uint32_t count,
                                 uint32_t *result) {
  nsresult rv = mHash->Update(reinterpret_cast<const uint8_t *>(buf), count);
||||||| merged common ancestors
nsCheckSummedOutputStream::Write(const char *buf, uint32_t count, uint32_t *result)
{
  nsresult rv = mHash->Update(reinterpret_cast<const uint8_t*>(buf), count);
=======
nsCheckSummedOutputStream::Write(const char* buf, uint32_t count,
                                 uint32_t* result) {
  nsresult rv = mHash->Update(reinterpret_cast<const uint8_t*>(buf), count);
>>>>>>> upstream-releases
  NS_ENSURE_SUCCESS(rv, rv);

  return nsBufferedOutputStream::Write(buf, count, result);
}

////////////////////////////////////////////////////////////////////////////////
// nsCrc32CheckSumedOutputStream
NS_IMPL_ISUPPORTS_INHERITED(nsCrc32CheckSumedOutputStream,
                            nsBufferedOutputStream, nsISafeOutputStream)

NS_IMETHODIMP
nsCrc32CheckSumedOutputStream::Init(nsIOutputStream* stream,
                                    uint32_t bufferSize) {
  mCheckSum = ~0;

  return nsBufferedOutputStream::Init(stream, bufferSize);
}

NS_IMETHODIMP
nsCrc32CheckSumedOutputStream::Finish() {
  uint32_t written;
  nsresult rv = nsBufferedOutputStream::Write(
      reinterpret_cast<const char*>(&mCheckSum), sizeof(mCheckSum), &written);
  NS_ASSERTION(written == sizeof(mCheckSum), "Error writing stream checksum");
  NS_ENSURE_SUCCESS(rv, rv);

  return nsBufferedOutputStream::Finish();
}

NS_IMETHODIMP
nsCrc32CheckSumedOutputStream::Write(const char* buf, uint32_t count,
                                     uint32_t* result) {
  mCheckSum =
      ComputeCrc32c(mCheckSum, reinterpret_cast<const uint8_t*>(buf), count);

  return nsBufferedOutputStream::Write(buf, count, result);
}

////////////////////////////////////////////////////////////////////////////////
