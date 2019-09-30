/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_SVGTRANSFORMLIST_H__
#define MOZILLA_SVGTRANSFORMLIST_H__

#include "gfxMatrix.h"
#include "SVGTransform.h"
#include "nsTArray.h"

namespace mozilla {

namespace dom {
<<<<<<< HEAD
class SVGTransform;
}  // namespace dom
||||||| merged common ancestors
class SVGTransform;
} // namespace dom
=======
class DOMSVGTransform;
}  // namespace dom
>>>>>>> upstream-releases

/**
 * ATTENTION! WARNING! WATCH OUT!!
 *
 * Consumers that modify objects of this type absolutely MUST keep the DOM
 * wrappers for those lists (if any) in sync!! That's why this class is so
 * locked down.
 *
 * The DOM wrapper class for this class is DOMSVGTransformList.
 */
<<<<<<< HEAD
class SVGTransformList {
  friend class nsSVGAnimatedTransformList;
||||||| merged common ancestors
class SVGTransformList
{
  friend class nsSVGAnimatedTransformList;
=======
class SVGTransformList {
  friend class SVGAnimatedTransformList;
>>>>>>> upstream-releases
  friend class DOMSVGTransformList;
  friend class dom::DOMSVGTransform;

<<<<<<< HEAD
 public:
  SVGTransformList() {}
  ~SVGTransformList() {}
||||||| merged common ancestors
public:
  SVGTransformList() {}
  ~SVGTransformList() {}
=======
 public:
  SVGTransformList() = default;
  ~SVGTransformList() = default;
>>>>>>> upstream-releases

  // Only methods that don't make/permit modification to this list are public.
  // Only our friend classes can access methods that may change us.

  /// This may return an incomplete string on OOM, but that's acceptable.
  void GetValueAsString(nsAString& aValue) const;

  bool IsEmpty() const { return mItems.IsEmpty(); }

  uint32_t Length() const { return mItems.Length(); }

  const SVGTransform& operator[](uint32_t aIndex) const {
    return mItems[aIndex];
  }

  bool operator==(const SVGTransformList& rhs) const {
    return mItems == rhs.mItems;
  }

  bool SetCapacity(uint32_t size) { return mItems.SetCapacity(size, fallible); }

  void Compact() { mItems.Compact(); }

  gfxMatrix GetConsolidationMatrix() const;

  // Access to methods that can modify objects of this type is deliberately
  // limited. This is to reduce the chances of someone modifying objects of
  // this type without taking the necessary steps to keep DOM wrappers in sync.
  // If you need wider access to these methods, consider adding a method to
<<<<<<< HEAD
  // SVGAnimatedTransformList and having that class act as an intermediary so it
  // can take care of keeping DOM wrappers in sync.

 protected:
||||||| merged common ancestors
  // SVGAnimatedTransformList and having that class act as an intermediary so it
  // can take care of keeping DOM wrappers in sync.

protected:

=======
  // DOMSVGAnimatedTransformList and having that class act as an intermediary so
  // it can take care of keeping DOM wrappers in sync.

 protected:
>>>>>>> upstream-releases
  /**
   * These may fail on OOM if the internal capacity needs to be increased, in
   * which case the list will be left unmodified.
   */
  nsresult CopyFrom(const SVGTransformList& rhs);
  nsresult CopyFrom(const nsTArray<SVGTransform>& aTransformArray);

<<<<<<< HEAD
  nsSVGTransform& operator[](uint32_t aIndex) { return mItems[aIndex]; }
||||||| merged common ancestors
  nsSVGTransform& operator[](uint32_t aIndex) {
    return mItems[aIndex];
  }
=======
  SVGTransform& operator[](uint32_t aIndex) { return mItems[aIndex]; }
>>>>>>> upstream-releases

  /**
   * This may fail (return false) on OOM if the internal capacity is being
   * increased, in which case the list will be left unmodified.
   */
  bool SetLength(uint32_t aNumberOfItems) {
    return mItems.SetLength(aNumberOfItems, fallible);
  }

 private:
  // Marking the following private only serves to show which methods are only
  // used by our friend classes (as opposed to our subclasses) - it doesn't
  // really provide additional safety.

  nsresult SetValueFromString(const nsAString& aValue);

  void Clear() { mItems.Clear(); }

  bool InsertItem(uint32_t aIndex, const SVGTransform& aTransform) {
    if (aIndex >= mItems.Length()) {
      aIndex = mItems.Length();
    }
    return !!mItems.InsertElementAt(aIndex, aTransform, fallible);
  }

  void ReplaceItem(uint32_t aIndex, const SVGTransform& aTransform) {
    MOZ_ASSERT(aIndex < mItems.Length(),
               "DOM wrapper caller should have raised INDEX_SIZE_ERR");
    mItems[aIndex] = aTransform;
  }

  void RemoveItem(uint32_t aIndex) {
    MOZ_ASSERT(aIndex < mItems.Length(),
               "DOM wrapper caller should have raised INDEX_SIZE_ERR");
    mItems.RemoveElementAt(aIndex);
  }

  bool AppendItem(const SVGTransform& aTransform) {
    return !!mItems.AppendElement(aTransform, fallible);
  }

 protected:
  /*
<<<<<<< HEAD
   * See SVGLengthList for the rationale for using
   * FallibleTArray<nsSVGTransform> instead of FallibleTArray<nsSVGTransform,
   * 1>.
||||||| merged common ancestors
   * See SVGLengthList for the rationale for using FallibleTArray<nsSVGTransform>
   * instead of FallibleTArray<nsSVGTransform, 1>.
=======
   * See SVGLengthList for the rationale for using
   * FallibleTArray<SVGTransform> instead of FallibleTArray<SVGTransform,
   * 1>.
>>>>>>> upstream-releases
   */
  FallibleTArray<SVGTransform> mItems;
};

}  // namespace mozilla

#endif  // MOZILLA_SVGTRANSFORMLIST_H__
