/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SVGAnimatedClass.h"

#include "mozilla/dom/SVGElement.h"
#include "mozilla/Move.h"
#include "mozilla/SMILValue.h"
#include "DOMSVGAnimatedString.h"
#include "SMILStringType.h"

namespace mozilla {
namespace dom {

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGClass.cpp
struct DOMAnimatedString final : public SVGAnimatedString {
||||||| merged common ancestors
struct DOMAnimatedString final : public SVGAnimatedString
{
=======
// DOM wrapper class for the (DOM)SVGAnimatedString interface where the
// wrapped class is SVGAnimatedClass.
struct DOMAnimatedString final : public DOMSVGAnimatedString {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedClass.cpp
  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(DOMAnimatedString)

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGClass.cpp
  DOMAnimatedString(nsSVGClass* aVal, nsSVGElement* aSVGElement)
      : SVGAnimatedString(aSVGElement), mVal(aVal) {}
||||||| merged common ancestors
  DOMAnimatedString(nsSVGClass* aVal, nsSVGElement* aSVGElement)
    : SVGAnimatedString(aSVGElement)
    , mVal(aVal)
  {}
=======
  DOMAnimatedString(SVGAnimatedClass* aVal, SVGElement* aSVGElement)
      : DOMSVGAnimatedString(aSVGElement), mVal(aVal) {}
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedClass.cpp

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGClass.cpp
  nsSVGClass* mVal;  // kept alive because it belongs to content
||||||| merged common ancestors
  nsSVGClass* mVal; // kept alive because it belongs to content
=======
  SVGAnimatedClass* mVal;  // kept alive because it belongs to content
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedClass.cpp

  void GetBaseVal(nsAString& aResult) override {
    mVal->GetBaseValue(aResult, mSVGElement);
  }

  void SetBaseVal(const nsAString& aValue) override {
    mVal->SetBaseValue(aValue, mSVGElement, true);
  }

  void GetAnimVal(nsAString& aResult) override;

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGClass.cpp
 private:
  ~DOMAnimatedString() {}
||||||| merged common ancestors
private:
  ~DOMAnimatedString() {}
=======
 private:
  ~DOMAnimatedString() = default;
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedClass.cpp
};

NS_SVG_VAL_IMPL_CYCLE_COLLECTION_WRAPPERCACHED(DOMAnimatedString, mSVGElement)

NS_IMPL_CYCLE_COLLECTING_ADDREF(DOMAnimatedString)
NS_IMPL_CYCLE_COLLECTING_RELEASE(DOMAnimatedString)

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(DOMAnimatedString)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGClass.cpp
already_AddRefed<SVGAnimatedString> nsSVGClass::ToDOMAnimatedString(
    nsSVGElement* aSVGElement) {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
nsSVGClass::ToDOMAnimatedString(nsSVGElement* aSVGElement)
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGAnimatedClass::ToDOMAnimatedString(
    SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedClass.cpp
  RefPtr<DOMAnimatedString> result = new DOMAnimatedString(this, aSVGElement);
  return result.forget();
}

/* Implementation */

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGClass.cpp
void nsSVGClass::SetBaseValue(const nsAString& aValue,
                              nsSVGElement* aSVGElement, bool aDoSetAttr) {
||||||| merged common ancestors
void
nsSVGClass::SetBaseValue(const nsAString& aValue,
                         nsSVGElement *aSVGElement,
                         bool aDoSetAttr)
{
=======
void SVGAnimatedClass::SetBaseValue(const nsAString& aValue,
                                    SVGElement* aSVGElement, bool aDoSetAttr) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedClass.cpp
  NS_ASSERTION(aSVGElement, "Null element passed to SetBaseValue");

  aSVGElement->SetMayHaveClass();
  if (aDoSetAttr) {
    aSVGElement->SetAttr(kNameSpaceID_None, nsGkAtoms::_class, aValue, true);
  }
  if (mAnimVal) {
    aSVGElement->AnimationNeedsResample();
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGClass.cpp
void nsSVGClass::GetBaseValue(nsAString& aValue,
                              const nsSVGElement* aSVGElement) const {
||||||| merged common ancestors
void
nsSVGClass::GetBaseValue(nsAString& aValue, const nsSVGElement *aSVGElement) const
{
=======
void SVGAnimatedClass::GetBaseValue(nsAString& aValue,
                                    const SVGElement* aSVGElement) const {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedClass.cpp
  aSVGElement->GetAttr(kNameSpaceID_None, nsGkAtoms::_class, aValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGClass.cpp
void nsSVGClass::GetAnimValue(nsAString& aResult,
                              const nsSVGElement* aSVGElement) const {
||||||| merged common ancestors
void
nsSVGClass::GetAnimValue(nsAString& aResult, const nsSVGElement *aSVGElement) const
{
=======
void SVGAnimatedClass::GetAnimValue(nsAString& aResult,
                                    const SVGElement* aSVGElement) const {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedClass.cpp
  if (mAnimVal) {
    aResult = *mAnimVal;
    return;
  }

  aSVGElement->GetAttr(kNameSpaceID_None, nsGkAtoms::_class, aResult);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGClass.cpp
void nsSVGClass::SetAnimValue(const nsAString& aValue,
                              nsSVGElement* aSVGElement) {
||||||| merged common ancestors
void
nsSVGClass::SetAnimValue(const nsAString& aValue, nsSVGElement *aSVGElement)
{
=======
void SVGAnimatedClass::SetAnimValue(const nsAString& aValue,
                                    SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedClass.cpp
  if (mAnimVal && mAnimVal->Equals(aValue)) {
    return;
  }
  if (!mAnimVal) {
    mAnimVal = new nsString();
  }
  *mAnimVal = aValue;
  aSVGElement->SetMayHaveClass();
  aSVGElement->DidAnimateClass();
}

void DOMAnimatedString::GetAnimVal(nsAString& aResult) {
  mSVGElement->FlushAnimations();
  mVal->GetAnimValue(aResult, mSVGElement);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGClass.cpp
UniquePtr<nsISMILAttr> nsSVGClass::ToSMILAttr(nsSVGElement* aSVGElement) {
||||||| merged common ancestors
UniquePtr<nsISMILAttr>
nsSVGClass::ToSMILAttr(nsSVGElement *aSVGElement)
{
=======
UniquePtr<SMILAttr> SVGAnimatedClass::ToSMILAttr(SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedClass.cpp
  return MakeUnique<SMILString>(this, aSVGElement);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGClass.cpp
nsresult nsSVGClass::SMILString::ValueFromString(
    const nsAString& aStr, const dom::SVGAnimationElement* /*aSrcElement*/,
    nsSMILValue& aValue, bool& aPreventCachingOfSandwich) const {
  nsSMILValue val(SMILStringType::Singleton());
||||||| merged common ancestors
nsresult
nsSVGClass::SMILString::ValueFromString(const nsAString& aStr,
                                        const dom::SVGAnimationElement* /*aSrcElement*/,
                                        nsSMILValue& aValue,
                                        bool& aPreventCachingOfSandwich) const
{
  nsSMILValue val(SMILStringType::Singleton());
=======
nsresult SVGAnimatedClass::SMILString::ValueFromString(
    const nsAString& aStr, const dom::SVGAnimationElement* /*aSrcElement*/,
    SMILValue& aValue, bool& aPreventCachingOfSandwich) const {
  SMILValue val(SMILStringType::Singleton());
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedClass.cpp

  *static_cast<nsAString*>(val.mU.mPtr) = aStr;
  aValue = std::move(val);
  aPreventCachingOfSandwich = false;
  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGClass.cpp
nsSMILValue nsSVGClass::SMILString::GetBaseValue() const {
  nsSMILValue val(SMILStringType::Singleton());
||||||| merged common ancestors
nsSMILValue
nsSVGClass::SMILString::GetBaseValue() const
{
  nsSMILValue val(SMILStringType::Singleton());
=======
SMILValue SVGAnimatedClass::SMILString::GetBaseValue() const {
  SMILValue val(SMILStringType::Singleton());
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedClass.cpp
  mSVGElement->GetAttr(kNameSpaceID_None, nsGkAtoms::_class,
                       *static_cast<nsAString*>(val.mU.mPtr));
  return val;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGClass.cpp
void nsSVGClass::SMILString::ClearAnimValue() {
||||||| merged common ancestors
void
nsSVGClass::SMILString::ClearAnimValue()
{
=======
void SVGAnimatedClass::SMILString::ClearAnimValue() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedClass.cpp
  if (mVal->mAnimVal) {
    mVal->mAnimVal = nullptr;
    mSVGElement->DidAnimateClass();
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGClass.cpp
nsresult nsSVGClass::SMILString::SetAnimValue(const nsSMILValue& aValue) {
||||||| merged common ancestors
nsresult
nsSVGClass::SMILString::SetAnimValue(const nsSMILValue& aValue)
{
=======
nsresult SVGAnimatedClass::SMILString::SetAnimValue(const SMILValue& aValue) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedClass.cpp
  NS_ASSERTION(aValue.mType == SMILStringType::Singleton(),
               "Unexpected type to assign animated value");
  if (aValue.mType == SMILStringType::Singleton()) {
    mVal->SetAnimValue(*static_cast<nsAString*>(aValue.mU.mPtr), mSVGElement);
  }
  return NS_OK;
}

}  // namespace dom
}  // namespace mozilla
