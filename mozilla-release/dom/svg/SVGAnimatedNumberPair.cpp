/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SVGAnimatedNumberPair.h"

#include "nsCharSeparatedTokenizer.h"
#include "SVGAttrTearoffTable.h"
#include "SVGNumberPairSMILType.h"
#include "mozilla/SMILValue.h"
#include "mozilla/SVGContentUtils.h"

using namespace mozilla::dom;

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGNumberPair.cpp
static nsSVGAttrTearoffTable<nsSVGNumberPair,
                             nsSVGNumberPair::DOMAnimatedNumber>
    sSVGFirstAnimatedNumberTearoffTable;
static nsSVGAttrTearoffTable<nsSVGNumberPair,
                             nsSVGNumberPair::DOMAnimatedNumber>
    sSVGSecondAnimatedNumberTearoffTable;

static nsresult ParseNumberOptionalNumber(const nsAString& aValue,
                                          float aValues[2]) {
  nsCharSeparatedTokenizerTemplate<nsContentUtils::IsHTMLWhitespace> tokenizer(
      aValue, ',', nsCharSeparatedTokenizer::SEPARATOR_OPTIONAL);
  if (tokenizer.whitespaceBeforeFirstToken()) {
    return NS_ERROR_DOM_SYNTAX_ERR;
  }
||||||| merged common ancestors
static nsSVGAttrTearoffTable<nsSVGNumberPair, nsSVGNumberPair::DOMAnimatedNumber>
  sSVGFirstAnimatedNumberTearoffTable;
static nsSVGAttrTearoffTable<nsSVGNumberPair, nsSVGNumberPair::DOMAnimatedNumber>
  sSVGSecondAnimatedNumberTearoffTable;

static nsresult
ParseNumberOptionalNumber(const nsAString& aValue,
                          float aValues[2])
{
  nsCharSeparatedTokenizerTemplate<nsContentUtils::IsHTMLWhitespace>
    tokenizer(aValue, ',',
              nsCharSeparatedTokenizer::SEPARATOR_OPTIONAL);
  if (tokenizer.whitespaceBeforeFirstToken()) {
    return NS_ERROR_DOM_SYNTAX_ERR;
  }
=======
namespace mozilla {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedNumberPair.cpp

static SVGAttrTearoffTable<SVGAnimatedNumberPair,
                           SVGAnimatedNumberPair::DOMAnimatedNumber>
    sSVGFirstAnimatedNumberTearoffTable;
static SVGAttrTearoffTable<SVGAnimatedNumberPair,
                           SVGAnimatedNumberPair::DOMAnimatedNumber>
    sSVGSecondAnimatedNumberTearoffTable;

static nsresult ParseNumberOptionalNumber(const nsAString& aValue,
                                          float aValues[2]) {
  nsCharSeparatedTokenizerTemplate<nsContentUtils::IsHTMLWhitespace> tokenizer(
      aValue, ',', nsCharSeparatedTokenizer::SEPARATOR_OPTIONAL);
  uint32_t i;
  for (i = 0; i < 2 && tokenizer.hasMoreTokens(); ++i) {
    if (!SVGContentUtils::ParseNumber(tokenizer.nextToken(), aValues[i])) {
      return NS_ERROR_DOM_SYNTAX_ERR;
    }
  }
  if (i == 1) {
    aValues[1] = aValues[0];
  }

  if (i == 0 ||                                  // Too few values.
      tokenizer.hasMoreTokens() ||               // Too many values.
      tokenizer.separatorAfterCurrentToken()) {  // Trailing comma.
    return NS_ERROR_DOM_SYNTAX_ERR;
  }

  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGNumberPair.cpp
nsresult nsSVGNumberPair::SetBaseValueString(const nsAString& aValueAsString,
                                             nsSVGElement* aSVGElement) {
||||||| merged common ancestors
nsresult
nsSVGNumberPair::SetBaseValueString(const nsAString &aValueAsString,
                                    nsSVGElement *aSVGElement)
{
=======
nsresult SVGAnimatedNumberPair::SetBaseValueString(
    const nsAString& aValueAsString, SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedNumberPair.cpp
  float val[2];

  nsresult rv = ParseNumberOptionalNumber(aValueAsString, val);
  if (NS_FAILED(rv)) {
    return rv;
  }

  mBaseVal[0] = val[0];
  mBaseVal[1] = val[1];
  mIsBaseSet = true;
  if (!mIsAnimated) {
    mAnimVal[0] = mBaseVal[0];
    mAnimVal[1] = mBaseVal[1];
  } else {
    aSVGElement->AnimationNeedsResample();
  }

  // We don't need to call Will/DidChange* here - we're only called by
  // SVGElement::ParseAttribute under Element::SetAttr,
  // which takes care of notifying.
  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGNumberPair.cpp
void nsSVGNumberPair::GetBaseValueString(nsAString& aValueAsString) const {
||||||| merged common ancestors
void
nsSVGNumberPair::GetBaseValueString(nsAString &aValueAsString) const
{
=======
void SVGAnimatedNumberPair::GetBaseValueString(
    nsAString& aValueAsString) const {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedNumberPair.cpp
  aValueAsString.Truncate();
  aValueAsString.AppendFloat(mBaseVal[0]);
  if (mBaseVal[0] != mBaseVal[1]) {
    aValueAsString.AppendLiteral(", ");
    aValueAsString.AppendFloat(mBaseVal[1]);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGNumberPair.cpp
void nsSVGNumberPair::SetBaseValue(float aValue, PairIndex aPairIndex,
                                   nsSVGElement* aSVGElement) {
||||||| merged common ancestors
void
nsSVGNumberPair::SetBaseValue(float aValue, PairIndex aPairIndex,
                              nsSVGElement *aSVGElement)
{
=======
void SVGAnimatedNumberPair::SetBaseValue(float aValue, PairIndex aPairIndex,
                                         SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedNumberPair.cpp
  uint32_t index = (aPairIndex == eFirst ? 0 : 1);
  if (mIsBaseSet && mBaseVal[index] == aValue) {
    return;
  }
  nsAttrValue emptyOrOldValue = aSVGElement->WillChangeNumberPair(mAttrEnum);
  mBaseVal[index] = aValue;
  mIsBaseSet = true;
  if (!mIsAnimated) {
    mAnimVal[index] = aValue;
  } else {
    aSVGElement->AnimationNeedsResample();
  }
  aSVGElement->DidChangeNumberPair(mAttrEnum, emptyOrOldValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGNumberPair.cpp
void nsSVGNumberPair::SetBaseValues(float aValue1, float aValue2,
                                    nsSVGElement* aSVGElement) {
||||||| merged common ancestors
void
nsSVGNumberPair::SetBaseValues(float aValue1, float aValue2,
                               nsSVGElement *aSVGElement)
{
=======
void SVGAnimatedNumberPair::SetBaseValues(float aValue1, float aValue2,
                                          SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedNumberPair.cpp
  if (mIsBaseSet && mBaseVal[0] == aValue1 && mBaseVal[1] == aValue2) {
    return;
  }
  nsAttrValue emptyOrOldValue = aSVGElement->WillChangeNumberPair(mAttrEnum);
  mBaseVal[0] = aValue1;
  mBaseVal[1] = aValue2;
  mIsBaseSet = true;
  if (!mIsAnimated) {
    mAnimVal[0] = aValue1;
    mAnimVal[1] = aValue2;
  } else {
    aSVGElement->AnimationNeedsResample();
  }
  aSVGElement->DidChangeNumberPair(mAttrEnum, emptyOrOldValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGNumberPair.cpp
void nsSVGNumberPair::SetAnimValue(const float aValue[2],
                                   nsSVGElement* aSVGElement) {
||||||| merged common ancestors
void
nsSVGNumberPair::SetAnimValue(const float aValue[2], nsSVGElement *aSVGElement)
{
=======
void SVGAnimatedNumberPair::SetAnimValue(const float aValue[2],
                                         SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedNumberPair.cpp
  if (mIsAnimated && mAnimVal[0] == aValue[0] && mAnimVal[1] == aValue[1]) {
    return;
  }
  mAnimVal[0] = aValue[0];
  mAnimVal[1] = aValue[1];
  mIsAnimated = true;
  aSVGElement->DidAnimateNumberPair(mAttrEnum);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGNumberPair.cpp
already_AddRefed<SVGAnimatedNumber> nsSVGNumberPair::ToDOMAnimatedNumber(
    PairIndex aIndex, nsSVGElement* aSVGElement) {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
nsSVGNumberPair::ToDOMAnimatedNumber(PairIndex aIndex,
                                     nsSVGElement* aSVGElement)
{
=======
already_AddRefed<DOMSVGAnimatedNumber>
SVGAnimatedNumberPair::ToDOMAnimatedNumber(PairIndex aIndex,
                                           SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedNumberPair.cpp
  RefPtr<DOMAnimatedNumber> domAnimatedNumber =
      aIndex == eFirst ? sSVGFirstAnimatedNumberTearoffTable.GetTearoff(this)
                       : sSVGSecondAnimatedNumberTearoffTable.GetTearoff(this);
  if (!domAnimatedNumber) {
    domAnimatedNumber = new DOMAnimatedNumber(this, aIndex, aSVGElement);
    if (aIndex == eFirst) {
      sSVGFirstAnimatedNumberTearoffTable.AddTearoff(this, domAnimatedNumber);
    } else {
      sSVGSecondAnimatedNumberTearoffTable.AddTearoff(this, domAnimatedNumber);
    }
  }

  return domAnimatedNumber.forget();
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGNumberPair.cpp
nsSVGNumberPair::DOMAnimatedNumber::~DOMAnimatedNumber() {
||||||| merged common ancestors
nsSVGNumberPair::DOMAnimatedNumber::~DOMAnimatedNumber()
{
=======
SVGAnimatedNumberPair::DOMAnimatedNumber::~DOMAnimatedNumber() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedNumberPair.cpp
  if (mIndex == eFirst) {
    sSVGFirstAnimatedNumberTearoffTable.RemoveTearoff(mVal);
  } else {
    sSVGSecondAnimatedNumberTearoffTable.RemoveTearoff(mVal);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGNumberPair.cpp
UniquePtr<nsISMILAttr> nsSVGNumberPair::ToSMILAttr(nsSVGElement* aSVGElement) {
||||||| merged common ancestors
UniquePtr<nsISMILAttr>
nsSVGNumberPair::ToSMILAttr(nsSVGElement *aSVGElement)
{
=======
UniquePtr<SMILAttr> SVGAnimatedNumberPair::ToSMILAttr(SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedNumberPair.cpp
  return MakeUnique<SMILNumberPair>(this, aSVGElement);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGNumberPair.cpp
nsresult nsSVGNumberPair::SMILNumberPair::ValueFromString(
    const nsAString& aStr, const dom::SVGAnimationElement* /*aSrcElement*/,
    nsSMILValue& aValue, bool& aPreventCachingOfSandwich) const {
||||||| merged common ancestors
nsresult
nsSVGNumberPair::SMILNumberPair::ValueFromString(const nsAString& aStr,
                                                 const dom::SVGAnimationElement* /*aSrcElement*/,
                                                 nsSMILValue& aValue,
                                                 bool& aPreventCachingOfSandwich) const
{
=======
nsresult SVGAnimatedNumberPair::SMILNumberPair::ValueFromString(
    const nsAString& aStr, const dom::SVGAnimationElement* /*aSrcElement*/,
    SMILValue& aValue, bool& aPreventCachingOfSandwich) const {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedNumberPair.cpp
  float values[2];

  nsresult rv = ParseNumberOptionalNumber(aStr, values);
  if (NS_FAILED(rv)) {
    return rv;
  }

  SMILValue val(&SVGNumberPairSMILType::sSingleton);
  val.mU.mNumberPair[0] = values[0];
  val.mU.mNumberPair[1] = values[1];
  aValue = val;
  aPreventCachingOfSandwich = false;

  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGNumberPair.cpp
nsSMILValue nsSVGNumberPair::SMILNumberPair::GetBaseValue() const {
  nsSMILValue val(&SVGNumberPairSMILType::sSingleton);
||||||| merged common ancestors
nsSMILValue
nsSVGNumberPair::SMILNumberPair::GetBaseValue() const
{
  nsSMILValue val(&SVGNumberPairSMILType::sSingleton);
=======
SMILValue SVGAnimatedNumberPair::SMILNumberPair::GetBaseValue() const {
  SMILValue val(&SVGNumberPairSMILType::sSingleton);
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedNumberPair.cpp
  val.mU.mNumberPair[0] = mVal->mBaseVal[0];
  val.mU.mNumberPair[1] = mVal->mBaseVal[1];
  return val;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGNumberPair.cpp
void nsSVGNumberPair::SMILNumberPair::ClearAnimValue() {
||||||| merged common ancestors
void
nsSVGNumberPair::SMILNumberPair::ClearAnimValue()
{
=======
void SVGAnimatedNumberPair::SMILNumberPair::ClearAnimValue() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedNumberPair.cpp
  if (mVal->mIsAnimated) {
    mVal->mIsAnimated = false;
    mVal->mAnimVal[0] = mVal->mBaseVal[0];
    mVal->mAnimVal[1] = mVal->mBaseVal[1];
    mSVGElement->DidAnimateNumberPair(mVal->mAttrEnum);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGNumberPair.cpp
nsresult nsSVGNumberPair::SMILNumberPair::SetAnimValue(
    const nsSMILValue& aValue) {
||||||| merged common ancestors
nsresult
nsSVGNumberPair::SMILNumberPair::SetAnimValue(const nsSMILValue& aValue)
{
=======
nsresult SVGAnimatedNumberPair::SMILNumberPair::SetAnimValue(
    const SMILValue& aValue) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedNumberPair.cpp
  NS_ASSERTION(aValue.mType == &SVGNumberPairSMILType::sSingleton,
               "Unexpected type to assign animated value");
  if (aValue.mType == &SVGNumberPairSMILType::sSingleton) {
    mVal->SetAnimValue(aValue.mU.mNumberPair, mSVGElement);
  }
  return NS_OK;
}

}  // namespace mozilla
