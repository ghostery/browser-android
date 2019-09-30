/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "CacheLog.h"
#include "CacheIndexContextIterator.h"
#include "CacheIndex.h"
#include "nsString.h"

namespace mozilla {
namespace net {

CacheIndexContextIterator::CacheIndexContextIterator(CacheIndex* aIndex,
                                                     bool aAddNew,
<<<<<<< HEAD
                                                     nsILoadContextInfo *aInfo)
    : CacheIndexIterator(aIndex, aAddNew), mInfo(aInfo) {}
||||||| merged common ancestors
                                                     nsILoadContextInfo *aInfo)
  : CacheIndexIterator(aIndex, aAddNew)
  , mInfo(aInfo)
{
}
=======
                                                     nsILoadContextInfo* aInfo)
    : CacheIndexIterator(aIndex, aAddNew), mInfo(aInfo) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
void CacheIndexContextIterator::AddRecord(CacheIndexRecord *aRecord) {
||||||| merged common ancestors
void
CacheIndexContextIterator::AddRecord(CacheIndexRecord *aRecord)
{
=======
void CacheIndexContextIterator::AddRecord(CacheIndexRecord* aRecord) {
>>>>>>> upstream-releases
  if (CacheIndexEntry::RecordMatchesLoadContextInfo(aRecord, mInfo)) {
    CacheIndexIterator::AddRecord(aRecord);
  }
}

<<<<<<< HEAD
void CacheIndexContextIterator::AddRecords(
    const nsTArray<CacheIndexRecord *> &aRecords) {
||||||| merged common ancestors
void
CacheIndexContextIterator::AddRecords(
  const nsTArray<CacheIndexRecord *> &aRecords)
{
=======
void CacheIndexContextIterator::AddRecords(
    const nsTArray<CacheIndexRecord*>& aRecords) {
>>>>>>> upstream-releases
  // We need to add one by one so that those with wrong context are ignored.
  for (uint32_t i = 0; i < aRecords.Length(); ++i) {
    AddRecord(aRecords[i]);
  }
}

}  // namespace net
}  // namespace mozilla
