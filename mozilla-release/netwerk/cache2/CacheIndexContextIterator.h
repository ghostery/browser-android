/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CacheIndexContextIterator__h__
#define CacheIndexContextIterator__h__

#include "CacheIndexIterator.h"

class nsILoadContextInfo;

namespace mozilla {
namespace net {

<<<<<<< HEAD
class CacheIndexContextIterator : public CacheIndexIterator {
 public:
  CacheIndexContextIterator(CacheIndex *aIndex, bool aAddNew,
                            nsILoadContextInfo *aInfo);
||||||| merged common ancestors
class CacheIndexContextIterator : public CacheIndexIterator
{
public:
  CacheIndexContextIterator(CacheIndex *aIndex, bool aAddNew,
                            nsILoadContextInfo *aInfo);
=======
class CacheIndexContextIterator : public CacheIndexIterator {
 public:
  CacheIndexContextIterator(CacheIndex* aIndex, bool aAddNew,
                            nsILoadContextInfo* aInfo);
>>>>>>> upstream-releases
  virtual ~CacheIndexContextIterator() = default;

<<<<<<< HEAD
 private:
  virtual void AddRecord(CacheIndexRecord *aRecord) override;
  virtual void AddRecords(const nsTArray<CacheIndexRecord *> &aRecords);
||||||| merged common ancestors
private:
  virtual void AddRecord(CacheIndexRecord *aRecord) override;
  virtual void AddRecords(const nsTArray<CacheIndexRecord *> &aRecords);
=======
 private:
  virtual void AddRecord(CacheIndexRecord* aRecord) override;
  virtual void AddRecords(const nsTArray<CacheIndexRecord*>& aRecords);
>>>>>>> upstream-releases

  nsCOMPtr<nsILoadContextInfo> mInfo;
};

}  // namespace net
}  // namespace mozilla

#endif
