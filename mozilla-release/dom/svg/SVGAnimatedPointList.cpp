/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SVGAnimatedPointList.h"

#include "mozilla/Move.h"
#include "mozilla/SMILValue.h"
#include "mozilla/dom/SVGElement.h"
#include "DOMSVGPointList.h"
#include "SVGPointListSMILType.h"

using namespace mozilla::dom;

// See the comments in this file's header!

namespace mozilla {

<<<<<<< HEAD
nsresult SVGAnimatedPointList::SetBaseValueString(const nsAString &aValue) {
||||||| merged common ancestors
nsresult
SVGAnimatedPointList::SetBaseValueString(const nsAString& aValue)
{
=======
nsresult SVGAnimatedPointList::SetBaseValueString(const nsAString& aValue) {
>>>>>>> upstream-releases
  SVGPointList newBaseValue;

  // The spec says that the point data is parsed and accepted up to the first
  // error encountered, so we don't return early if an error occurs. However,
  // we do want to throw any error code from setAttribute if there's a problem.

  nsresult rv = newBaseValue.SetValueFromString(aValue);

  // We must send these notifications *before* changing mBaseVal! Our baseVal's
  // DOM wrapper list may have to remove DOM items from itself, and any removed
  // DOM items need to copy their internal counterpart's values *before* we
  // change them. See the comments in
  // DOMSVGPointList::InternalListWillChangeTo().

<<<<<<< HEAD
  DOMSVGPointList *baseValWrapper =
      DOMSVGPointList::GetDOMWrapperIfExists(GetBaseValKey());
||||||| merged common ancestors
  DOMSVGPointList *baseValWrapper =
    DOMSVGPointList::GetDOMWrapperIfExists(GetBaseValKey());
=======
  DOMSVGPointList* baseValWrapper =
      DOMSVGPointList::GetDOMWrapperIfExists(GetBaseValKey());
>>>>>>> upstream-releases
  if (baseValWrapper) {
    baseValWrapper->InternalListWillChangeTo(newBaseValue);
  }

  DOMSVGPointList *animValWrapper = nullptr;
  if (!IsAnimating()) {  // DOM anim val wraps our base val too!
    animValWrapper = DOMSVGPointList::GetDOMWrapperIfExists(GetAnimValKey());
    if (animValWrapper) {
      animValWrapper->InternalListWillChangeTo(newBaseValue);
    }
  }

  // Only now may we modify mBaseVal!

  // We don't need to call DidChange* here - we're only called by
  // SVGElement::ParseAttribute under Element::SetAttr,
  // which takes care of notifying.

  nsresult rv2 = mBaseVal.CopyFrom(newBaseValue);
  if (NS_FAILED(rv2)) {
    // Attempting to increase mBaseVal's length failed (mBaseVal is left
    // unmodified). We MUST keep any DOM wrappers in sync:
    if (baseValWrapper) {
      baseValWrapper->InternalListWillChangeTo(mBaseVal);
    }
    if (animValWrapper) {
      animValWrapper->InternalListWillChangeTo(mBaseVal);
    }
    return rv2;
  }
  return rv;
}

void SVGAnimatedPointList::ClearBaseValue() {
  // We must send these notifications *before* changing mBaseVal! (See above.)

<<<<<<< HEAD
  DOMSVGPointList *baseValWrapper =
      DOMSVGPointList::GetDOMWrapperIfExists(GetBaseValKey());
||||||| merged common ancestors
  DOMSVGPointList *baseValWrapper =
    DOMSVGPointList::GetDOMWrapperIfExists(GetBaseValKey());
=======
  DOMSVGPointList* baseValWrapper =
      DOMSVGPointList::GetDOMWrapperIfExists(GetBaseValKey());
>>>>>>> upstream-releases
  if (baseValWrapper) {
    baseValWrapper->InternalListWillChangeTo(SVGPointList());
  }

<<<<<<< HEAD
  if (!IsAnimating()) {  // DOM anim val wraps our base val too!
    DOMSVGPointList *animValWrapper =
        DOMSVGPointList::GetDOMWrapperIfExists(GetAnimValKey());
||||||| merged common ancestors
  if (!IsAnimating()) { // DOM anim val wraps our base val too!
    DOMSVGPointList *animValWrapper =
      DOMSVGPointList::GetDOMWrapperIfExists(GetAnimValKey());
=======
  if (!IsAnimating()) {  // DOM anim val wraps our base val too!
    DOMSVGPointList* animValWrapper =
        DOMSVGPointList::GetDOMWrapperIfExists(GetAnimValKey());
>>>>>>> upstream-releases
    if (animValWrapper) {
      animValWrapper->InternalListWillChangeTo(SVGPointList());
    }
  }

  mBaseVal.Clear();
  // Caller notifies
}

<<<<<<< HEAD
nsresult SVGAnimatedPointList::SetAnimValue(const SVGPointList &aNewAnimValue,
                                            nsSVGElement *aElement) {
||||||| merged common ancestors
nsresult
SVGAnimatedPointList::SetAnimValue(const SVGPointList& aNewAnimValue,
                                   nsSVGElement *aElement)
{
=======
nsresult SVGAnimatedPointList::SetAnimValue(const SVGPointList& aNewAnimValue,
                                            SVGElement* aElement) {
>>>>>>> upstream-releases
  // Note that a new animation may totally change the number of items in the
  // animVal list, either replacing what was essentially a mirror of the
  // baseVal list, or else replacing and overriding an existing animation.
  // It is not possible for us to reliably distinguish between calls to this
  // method that are setting a new sample for an existing animation (in which
  // case our list length isn't changing and we wouldn't need to notify our DOM
  // wrapper to keep its length in sync), and calls to this method that are
  // setting the first sample of a new animation that will override the base
  // value/an existing animation (in which case our length may be changing and
  // our DOM wrapper may need to be notified). Happily though, it's cheap to
  // just blindly notify our animVal's DOM wrapper of our new value each time
  // this method is called, so that's what we do.

  // We must send this notification *before* changing mAnimVal! (See above.)

<<<<<<< HEAD
  DOMSVGPointList *domWrapper =
      DOMSVGPointList::GetDOMWrapperIfExists(GetAnimValKey());
||||||| merged common ancestors
  DOMSVGPointList *domWrapper =
    DOMSVGPointList::GetDOMWrapperIfExists(GetAnimValKey());
=======
  DOMSVGPointList* domWrapper =
      DOMSVGPointList::GetDOMWrapperIfExists(GetAnimValKey());
>>>>>>> upstream-releases
  if (domWrapper) {
    domWrapper->InternalListWillChangeTo(aNewAnimValue);
  }
  if (!mAnimVal) {
    mAnimVal = new SVGPointList();
  }
  nsresult rv = mAnimVal->CopyFrom(aNewAnimValue);
  if (NS_FAILED(rv)) {
    // OOM. We clear the animation and, importantly, ClearAnimValue() ensures
    // that mAnimVal's DOM wrapper (if any) is kept in sync!
    ClearAnimValue(aElement);
    return rv;
  }
  aElement->DidAnimatePointList();
  return NS_OK;
}

<<<<<<< HEAD
void SVGAnimatedPointList::ClearAnimValue(nsSVGElement *aElement) {
||||||| merged common ancestors
void
SVGAnimatedPointList::ClearAnimValue(nsSVGElement *aElement)
{
=======
void SVGAnimatedPointList::ClearAnimValue(SVGElement* aElement) {
>>>>>>> upstream-releases
  // We must send these notifications *before* changing mAnimVal! (See above.)

<<<<<<< HEAD
  DOMSVGPointList *domWrapper =
      DOMSVGPointList::GetDOMWrapperIfExists(GetAnimValKey());
||||||| merged common ancestors
  DOMSVGPointList *domWrapper =
    DOMSVGPointList::GetDOMWrapperIfExists(GetAnimValKey());
=======
  DOMSVGPointList* domWrapper =
      DOMSVGPointList::GetDOMWrapperIfExists(GetAnimValKey());
>>>>>>> upstream-releases
  if (domWrapper) {
    // When all animation ends, animVal simply mirrors baseVal, which may have
    // a different number of items to the last active animated value.
    //
    domWrapper->InternalListWillChangeTo(mBaseVal);
  }
  mAnimVal = nullptr;
  aElement->DidAnimatePointList();
}

<<<<<<< HEAD
UniquePtr<nsISMILAttr> SVGAnimatedPointList::ToSMILAttr(
    nsSVGElement *aElement) {
||||||| merged common ancestors
UniquePtr<nsISMILAttr>
SVGAnimatedPointList::ToSMILAttr(nsSVGElement *aElement)
{
=======
UniquePtr<SMILAttr> SVGAnimatedPointList::ToSMILAttr(SVGElement* aElement) {
>>>>>>> upstream-releases
  return MakeUnique<SMILAnimatedPointList>(this, aElement);
}

<<<<<<< HEAD
nsresult SVGAnimatedPointList::SMILAnimatedPointList::ValueFromString(
    const nsAString &aStr, const dom::SVGAnimationElement * /*aSrcElement*/,
    nsSMILValue &aValue, bool &aPreventCachingOfSandwich) const {
  nsSMILValue val(&SVGPointListSMILType::sSingleton);
  SVGPointListAndInfo *list = static_cast<SVGPointListAndInfo *>(val.mU.mPtr);
||||||| merged common ancestors
nsresult
SVGAnimatedPointList::
  SMILAnimatedPointList::ValueFromString(const nsAString& aStr,
                               const dom::SVGAnimationElement* /*aSrcElement*/,
                               nsSMILValue& aValue,
                               bool& aPreventCachingOfSandwich) const
{
  nsSMILValue val(&SVGPointListSMILType::sSingleton);
  SVGPointListAndInfo *list = static_cast<SVGPointListAndInfo*>(val.mU.mPtr);
=======
nsresult SVGAnimatedPointList::SMILAnimatedPointList::ValueFromString(
    const nsAString& aStr, const dom::SVGAnimationElement* /*aSrcElement*/,
    SMILValue& aValue, bool& aPreventCachingOfSandwich) const {
  SMILValue val(&SVGPointListSMILType::sSingleton);
  SVGPointListAndInfo* list = static_cast<SVGPointListAndInfo*>(val.mU.mPtr);
>>>>>>> upstream-releases
  nsresult rv = list->SetValueFromString(aStr);
  if (NS_SUCCEEDED(rv)) {
    list->SetInfo(mElement);
    aValue = std::move(val);
  }
  aPreventCachingOfSandwich = false;
  return rv;
}

<<<<<<< HEAD
nsSMILValue SVGAnimatedPointList::SMILAnimatedPointList::GetBaseValue() const {
||||||| merged common ancestors
nsSMILValue
SVGAnimatedPointList::SMILAnimatedPointList::GetBaseValue() const
{
=======
SMILValue SVGAnimatedPointList::SMILAnimatedPointList::GetBaseValue() const {
>>>>>>> upstream-releases
  // To benefit from Return Value Optimization and avoid copy constructor calls
  // due to our use of return-by-value, we must return the exact same object
  // from ALL return points. This function must only return THIS variable:
  SMILValue val;

<<<<<<< HEAD
  nsSMILValue tmp(&SVGPointListSMILType::sSingleton);
  SVGPointListAndInfo *list = static_cast<SVGPointListAndInfo *>(tmp.mU.mPtr);
||||||| merged common ancestors
  nsSMILValue tmp(&SVGPointListSMILType::sSingleton);
  SVGPointListAndInfo *list = static_cast<SVGPointListAndInfo*>(tmp.mU.mPtr);
=======
  SMILValue tmp(&SVGPointListSMILType::sSingleton);
  SVGPointListAndInfo* list = static_cast<SVGPointListAndInfo*>(tmp.mU.mPtr);
>>>>>>> upstream-releases
  nsresult rv = list->CopyFrom(mVal->mBaseVal);
  if (NS_SUCCEEDED(rv)) {
    list->SetInfo(mElement);
    Swap(val, tmp);
  }
  return val;
}

<<<<<<< HEAD
nsresult SVGAnimatedPointList::SMILAnimatedPointList::SetAnimValue(
    const nsSMILValue &aValue) {
||||||| merged common ancestors
nsresult
SVGAnimatedPointList::SMILAnimatedPointList::SetAnimValue(const nsSMILValue& aValue)
{
=======
nsresult SVGAnimatedPointList::SMILAnimatedPointList::SetAnimValue(
    const SMILValue& aValue) {
>>>>>>> upstream-releases
  NS_ASSERTION(aValue.mType == &SVGPointListSMILType::sSingleton,
               "Unexpected type to assign animated value");
  if (aValue.mType == &SVGPointListSMILType::sSingleton) {
    mVal->SetAnimValue(*static_cast<SVGPointListAndInfo *>(aValue.mU.mPtr),
                       mElement);
  }
  return NS_OK;
}

void SVGAnimatedPointList::SMILAnimatedPointList::ClearAnimValue() {
  if (mVal->mAnimVal) {
    mVal->ClearAnimValue(mElement);
  }
}

}  // namespace mozilla
