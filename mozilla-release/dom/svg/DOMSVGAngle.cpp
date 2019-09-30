/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "DOMSVGAngle.h"
#include "SVGAnimatedOrient.h"
#include "mozilla/dom/SVGAngleBinding.h"
#include "mozilla/dom/SVGSVGElement.h"

using namespace mozilla;
using namespace mozilla::dom;

NS_SVG_VAL_IMPL_CYCLE_COLLECTION_WRAPPERCACHED(DOMSVGAngle, mSVGElement)

NS_IMPL_CYCLE_COLLECTION_ROOT_NATIVE(DOMSVGAngle, AddRef)
NS_IMPL_CYCLE_COLLECTION_UNROOT_NATIVE(DOMSVGAngle, Release)

<<<<<<< HEAD:mozilla-release/dom/svg/SVGAngle.cpp
JSObject* SVGAngle::WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) {
||||||| merged common ancestors
JSObject*
SVGAngle::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto)
{
=======
DOMSVGAngle::DOMSVGAngle(SVGSVGElement* aSVGElement)
    : mSVGElement(aSVGElement), mType(DOMSVGAngle::CreatedValue) {
  mVal = new SVGAnimatedOrient();
  mVal->Init();
}

JSObject* DOMSVGAngle::WrapObject(JSContext* aCx,
                                  JS::Handle<JSObject*> aGivenProto) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/DOMSVGAngle.cpp
  return SVGAngle_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD:mozilla-release/dom/svg/SVGAngle.cpp
uint16_t SVGAngle::UnitType() const {
||||||| merged common ancestors
uint16_t
SVGAngle::UnitType() const
{
=======
uint16_t DOMSVGAngle::UnitType() const {
>>>>>>> upstream-releases:mozilla-release/dom/svg/DOMSVGAngle.cpp
  if (mType == AnimValue) {
    mSVGElement->FlushAnimations();
    return mVal->mAnimValUnit;
  }
  return mVal->mBaseValUnit;
}

<<<<<<< HEAD:mozilla-release/dom/svg/SVGAngle.cpp
float SVGAngle::Value() const {
||||||| merged common ancestors
float
SVGAngle::Value() const
{
=======
float DOMSVGAngle::Value() const {
>>>>>>> upstream-releases:mozilla-release/dom/svg/DOMSVGAngle.cpp
  if (mType == AnimValue) {
    mSVGElement->FlushAnimations();
    return mVal->GetAnimValue();
  }
  return mVal->GetBaseValue();
}

<<<<<<< HEAD:mozilla-release/dom/svg/SVGAngle.cpp
void SVGAngle::SetValue(float aValue, ErrorResult& rv) {
||||||| merged common ancestors
void
SVGAngle::SetValue(float aValue, ErrorResult& rv)
{
=======
void DOMSVGAngle::SetValue(float aValue, ErrorResult& rv) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/DOMSVGAngle.cpp
  if (mType == AnimValue) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }
  bool isBaseVal = mType == BaseValue;
  mVal->SetBaseValue(aValue, mVal->mBaseValUnit,
                     isBaseVal ? mSVGElement.get() : nullptr, isBaseVal);
}

<<<<<<< HEAD:mozilla-release/dom/svg/SVGAngle.cpp
float SVGAngle::ValueInSpecifiedUnits() const {
||||||| merged common ancestors
float
SVGAngle::ValueInSpecifiedUnits() const
{
=======
float DOMSVGAngle::ValueInSpecifiedUnits() const {
>>>>>>> upstream-releases:mozilla-release/dom/svg/DOMSVGAngle.cpp
  if (mType == AnimValue) {
    return mVal->mAnimVal;
  }
  return mVal->mBaseVal;
}

<<<<<<< HEAD:mozilla-release/dom/svg/SVGAngle.cpp
void SVGAngle::SetValueInSpecifiedUnits(float aValue, ErrorResult& rv) {
||||||| merged common ancestors
void
SVGAngle::SetValueInSpecifiedUnits(float aValue, ErrorResult& rv)
{
=======
void DOMSVGAngle::SetValueInSpecifiedUnits(float aValue, ErrorResult& rv) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/DOMSVGAngle.cpp
  if (mType == AnimValue) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }
  if (mType == BaseValue) {
    mVal->SetBaseValueInSpecifiedUnits(aValue, mSVGElement);
  } else {
    mVal->mBaseVal = aValue;
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/SVGAngle.cpp
void SVGAngle::NewValueSpecifiedUnits(uint16_t unitType,
                                      float valueInSpecifiedUnits,
                                      ErrorResult& rv) {
||||||| merged common ancestors
void
SVGAngle::NewValueSpecifiedUnits(uint16_t unitType,
                                 float valueInSpecifiedUnits,
                                 ErrorResult& rv)
{
=======
void DOMSVGAngle::NewValueSpecifiedUnits(uint16_t unitType,
                                         float valueInSpecifiedUnits,
                                         ErrorResult& rv) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/DOMSVGAngle.cpp
  if (mType == AnimValue) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }
  rv = mVal->NewValueSpecifiedUnits(
      unitType, valueInSpecifiedUnits,
      mType == BaseValue ? mSVGElement.get() : nullptr);
}

<<<<<<< HEAD:mozilla-release/dom/svg/SVGAngle.cpp
void SVGAngle::ConvertToSpecifiedUnits(uint16_t unitType, ErrorResult& rv) {
||||||| merged common ancestors
void
SVGAngle::ConvertToSpecifiedUnits(uint16_t unitType, ErrorResult& rv)
{
=======
void DOMSVGAngle::ConvertToSpecifiedUnits(uint16_t unitType, ErrorResult& rv) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/DOMSVGAngle.cpp
  if (mType == AnimValue) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }
  rv = mVal->ConvertToSpecifiedUnits(
      unitType, mType == BaseValue ? mSVGElement.get() : nullptr);
}

<<<<<<< HEAD:mozilla-release/dom/svg/SVGAngle.cpp
void SVGAngle::SetValueAsString(const nsAString& aValue, ErrorResult& rv) {
||||||| merged common ancestors
void
SVGAngle::SetValueAsString(const nsAString& aValue, ErrorResult& rv)
{
=======
void DOMSVGAngle::SetValueAsString(const nsAString& aValue, ErrorResult& rv) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/DOMSVGAngle.cpp
  if (mType == AnimValue) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }
  bool isBaseVal = mType == BaseValue;
  rv = mVal->SetBaseValueString(aValue, isBaseVal ? mSVGElement.get() : nullptr,
                                isBaseVal);
}

<<<<<<< HEAD:mozilla-release/dom/svg/SVGAngle.cpp
void SVGAngle::GetValueAsString(nsAString& aValue) {
||||||| merged common ancestors
void
SVGAngle::GetValueAsString(nsAString& aValue)
{
=======
void DOMSVGAngle::GetValueAsString(nsAString& aValue) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/DOMSVGAngle.cpp
  if (mType == AnimValue) {
    mSVGElement->FlushAnimations();
    mVal->GetAnimAngleValueString(aValue);
  } else {
    mVal->GetBaseAngleValueString(aValue);
  }
}
