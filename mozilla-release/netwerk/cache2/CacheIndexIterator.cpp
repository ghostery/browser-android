/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "CacheLog.h"
#include "CacheIndexIterator.h"
#include "CacheIndex.h"
#include "nsString.h"
#include "mozilla/DebugOnly.h"

namespace mozilla {
namespace net {

<<<<<<< HEAD
CacheIndexIterator::CacheIndexIterator(CacheIndex *aIndex, bool aAddNew)
    : mStatus(NS_OK), mIndex(aIndex), mAddNew(aAddNew) {
||||||| merged common ancestors
CacheIndexIterator::CacheIndexIterator(CacheIndex *aIndex, bool aAddNew)
  : mStatus(NS_OK)
  , mIndex(aIndex)
  , mAddNew(aAddNew)
{
=======
CacheIndexIterator::CacheIndexIterator(CacheIndex* aIndex, bool aAddNew)
    : mStatus(NS_OK), mIndex(aIndex), mAddNew(aAddNew) {
>>>>>>> upstream-releases
  LOG(("CacheIndexIterator::CacheIndexIterator() [this=%p]", this));
}

CacheIndexIterator::~CacheIndexIterator() {
  LOG(("CacheIndexIterator::~CacheIndexIterator() [this=%p]", this));

  Close();
}

<<<<<<< HEAD
nsresult CacheIndexIterator::GetNextHash(SHA1Sum::Hash *aHash) {
||||||| merged common ancestors
nsresult
CacheIndexIterator::GetNextHash(SHA1Sum::Hash *aHash)
{
=======
nsresult CacheIndexIterator::GetNextHash(SHA1Sum::Hash* aHash) {
>>>>>>> upstream-releases
  LOG(("CacheIndexIterator::GetNextHash() [this=%p]", this));

  StaticMutexAutoLock lock(CacheIndex::sLock);

  if (NS_FAILED(mStatus)) {
    return mStatus;
  }

  if (!mRecords.Length()) {
    CloseInternal(NS_ERROR_NOT_AVAILABLE);
    return mStatus;
  }

  memcpy(aHash, mRecords[mRecords.Length() - 1]->mHash, sizeof(SHA1Sum::Hash));
  mRecords.RemoveLastElement();

  return NS_OK;
}

nsresult CacheIndexIterator::Close() {
  LOG(("CacheIndexIterator::Close() [this=%p]", this));

  StaticMutexAutoLock lock(CacheIndex::sLock);

  return CloseInternal(NS_ERROR_NOT_AVAILABLE);
}

nsresult CacheIndexIterator::CloseInternal(nsresult aStatus) {
  LOG(("CacheIndexIterator::CloseInternal() [this=%p, status=0x%08" PRIx32 "]",
       this, static_cast<uint32_t>(aStatus)));

  // Make sure status will be a failure
  MOZ_ASSERT(NS_FAILED(aStatus));
  if (NS_SUCCEEDED(aStatus)) {
    aStatus = NS_ERROR_UNEXPECTED;
  }

  if (NS_FAILED(mStatus)) {
    return NS_ERROR_NOT_AVAILABLE;
  }

  DebugOnly<bool> removed = mIndex->mIterators.RemoveElement(this);
  MOZ_ASSERT(removed);
  mStatus = aStatus;

  return NS_OK;
}

<<<<<<< HEAD
void CacheIndexIterator::AddRecord(CacheIndexRecord *aRecord) {
||||||| merged common ancestors
void
CacheIndexIterator::AddRecord(CacheIndexRecord *aRecord)
{
=======
void CacheIndexIterator::AddRecord(CacheIndexRecord* aRecord) {
>>>>>>> upstream-releases
  LOG(("CacheIndexIterator::AddRecord() [this=%p, record=%p]", this, aRecord));

  mRecords.AppendElement(aRecord);
}

<<<<<<< HEAD
bool CacheIndexIterator::RemoveRecord(CacheIndexRecord *aRecord) {
||||||| merged common ancestors
bool
CacheIndexIterator::RemoveRecord(CacheIndexRecord *aRecord)
{
=======
bool CacheIndexIterator::RemoveRecord(CacheIndexRecord* aRecord) {
>>>>>>> upstream-releases
  LOG(("CacheIndexIterator::RemoveRecord() [this=%p, record=%p]", this,
       aRecord));

  return mRecords.RemoveElement(aRecord);
}

<<<<<<< HEAD
bool CacheIndexIterator::ReplaceRecord(CacheIndexRecord *aOldRecord,
                                       CacheIndexRecord *aNewRecord) {
  LOG(
      ("CacheIndexIterator::ReplaceRecord() [this=%p, oldRecord=%p, "
       "newRecord=%p]",
       this, aOldRecord, aNewRecord));
||||||| merged common ancestors
bool
CacheIndexIterator::ReplaceRecord(CacheIndexRecord *aOldRecord,
                                  CacheIndexRecord *aNewRecord)
{
  LOG(("CacheIndexIterator::ReplaceRecord() [this=%p, oldRecord=%p, "
       "newRecord=%p]", this, aOldRecord, aNewRecord));
=======
bool CacheIndexIterator::ReplaceRecord(CacheIndexRecord* aOldRecord,
                                       CacheIndexRecord* aNewRecord) {
  LOG(
      ("CacheIndexIterator::ReplaceRecord() [this=%p, oldRecord=%p, "
       "newRecord=%p]",
       this, aOldRecord, aNewRecord));
>>>>>>> upstream-releases

  if (RemoveRecord(aOldRecord)) {
    AddRecord(aNewRecord);
    return true;
  }

  return false;
}

}  // namespace net
}  // namespace mozilla
