/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef NS_SVGATTRTEAROFFTABLE_H_
#define NS_SVGATTRTEAROFFTABLE_H_

#include "nsDataHashtable.h"
#include "nsDebug.h"
#include "nsHashKeys.h"

namespace mozilla {

/**
 * Global hashmap to associate internal SVG data types (e.g. SVGAnimatedLength)
 * with DOM tear-off objects (e.g. DOMSVGLength). This allows us to always
 * return the same object for subsequent requests for DOM objects.
 *
 * We don't keep an owning reference to the tear-off objects so they are
 * responsible for removing themselves from this table when they die.
 */
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGAttrTearoffTable.h
template <class SimpleType, class TearoffType>
class nsSVGAttrTearoffTable {
 public:
||||||| merged common ancestors
template<class SimpleType, class TearoffType>
class nsSVGAttrTearoffTable
{
public:
=======
template <class SimpleType, class TearoffType>
class SVGAttrTearoffTable {
 public:
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAttrTearoffTable.h
#ifdef DEBUG
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGAttrTearoffTable.h
  ~nsSVGAttrTearoffTable() {
||||||| merged common ancestors
  ~nsSVGAttrTearoffTable()
  {
=======
  ~SVGAttrTearoffTable() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAttrTearoffTable.h
    MOZ_ASSERT(!mTable, "Tear-off objects remain in hashtable at shutdown.");
  }
#endif

  TearoffType* GetTearoff(SimpleType* aSimple);

  void AddTearoff(SimpleType* aSimple, TearoffType* aTearoff);

  void RemoveTearoff(SimpleType* aSimple);

 private:
  typedef nsPtrHashKey<SimpleType> SimpleTypePtrKey;
  typedef nsDataHashtable<SimpleTypePtrKey, TearoffType*> TearoffTable;

  TearoffTable* mTable;
};

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGAttrTearoffTable.h
template <class SimpleType, class TearoffType>
TearoffType* nsSVGAttrTearoffTable<SimpleType, TearoffType>::GetTearoff(
    SimpleType* aSimple) {
  if (!mTable) return nullptr;
||||||| merged common ancestors
template<class SimpleType, class TearoffType>
TearoffType*
nsSVGAttrTearoffTable<SimpleType, TearoffType>::GetTearoff(SimpleType* aSimple)
{
  if (!mTable)
    return nullptr;
=======
template <class SimpleType, class TearoffType>
TearoffType* SVGAttrTearoffTable<SimpleType, TearoffType>::GetTearoff(
    SimpleType* aSimple) {
  if (!mTable) return nullptr;
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAttrTearoffTable.h

  TearoffType* tearoff = nullptr;

#ifdef DEBUG
  bool found =
#endif
      mTable->Get(aSimple, &tearoff);
  MOZ_ASSERT(!found || tearoff,
             "null pointer stored in attribute tear-off map");

  return tearoff;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGAttrTearoffTable.h
template <class SimpleType, class TearoffType>
void nsSVGAttrTearoffTable<SimpleType, TearoffType>::AddTearoff(
    SimpleType* aSimple, TearoffType* aTearoff) {
||||||| merged common ancestors
template<class SimpleType, class TearoffType>
void
nsSVGAttrTearoffTable<SimpleType, TearoffType>::AddTearoff(SimpleType* aSimple,
                                                          TearoffType* aTearoff)
{
=======
template <class SimpleType, class TearoffType>
void SVGAttrTearoffTable<SimpleType, TearoffType>::AddTearoff(
    SimpleType* aSimple, TearoffType* aTearoff) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAttrTearoffTable.h
  if (!mTable) {
    mTable = new TearoffTable;
  }

  // We shouldn't be adding a tear-off if there already is one. If that happens,
  // something is wrong.
  if (mTable->Get(aSimple, nullptr)) {
    MOZ_ASSERT(false, "There is already a tear-off for this object.");
    return;
  }

  mTable->Put(aSimple, aTearoff);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGAttrTearoffTable.h
template <class SimpleType, class TearoffType>
void nsSVGAttrTearoffTable<SimpleType, TearoffType>::RemoveTearoff(
    SimpleType* aSimple) {
||||||| merged common ancestors
template<class SimpleType, class TearoffType>
void
nsSVGAttrTearoffTable<SimpleType, TearoffType>::RemoveTearoff(
    SimpleType* aSimple)
{
=======
template <class SimpleType, class TearoffType>
void SVGAttrTearoffTable<SimpleType, TearoffType>::RemoveTearoff(
    SimpleType* aSimple) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAttrTearoffTable.h
  if (!mTable) {
    // Perhaps something happened in between creating the SimpleType object and
    // registering it
    return;
  }

  mTable->Remove(aSimple);
  if (mTable->Count() == 0) {
    delete mTable;
    mTable = nullptr;
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGAttrTearoffTable.h
#endif  // NS_SVGATTRTEAROFFTABLE_H_
||||||| merged common ancestors
#endif // NS_SVGATTRTEAROFFTABLE_H_
=======
}  // namespace mozilla

#endif  // NS_SVGATTRTEAROFFTABLE_H_
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAttrTearoffTable.h
