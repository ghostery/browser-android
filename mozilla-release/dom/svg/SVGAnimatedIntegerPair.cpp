/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SVGAnimatedIntegerPair.h"

#include "nsCharSeparatedTokenizer.h"
#include "nsError.h"
#include "nsMathUtils.h"
#include "SVGAttrTearoffTable.h"
#include "SVGIntegerPairSMILType.h"
#include "mozilla/SMILValue.h"
#include "mozilla/SVGContentUtils.h"

using namespace mozilla::dom;

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
static nsSVGAttrTearoffTable<nsSVGIntegerPair,
                             nsSVGIntegerPair::DOMAnimatedInteger>
    sSVGFirstAnimatedIntegerTearoffTable;
static nsSVGAttrTearoffTable<nsSVGIntegerPair,
                             nsSVGIntegerPair::DOMAnimatedInteger>
    sSVGSecondAnimatedIntegerTearoffTable;
||||||| merged common ancestors
static nsSVGAttrTearoffTable<nsSVGIntegerPair, nsSVGIntegerPair::DOMAnimatedInteger>
  sSVGFirstAnimatedIntegerTearoffTable;
static nsSVGAttrTearoffTable<nsSVGIntegerPair, nsSVGIntegerPair::DOMAnimatedInteger>
  sSVGSecondAnimatedIntegerTearoffTable;
=======
namespace mozilla {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp

static SVGAttrTearoffTable<SVGAnimatedIntegerPair,
                           SVGAnimatedIntegerPair::DOMAnimatedInteger>
    sSVGFirstAnimatedIntegerTearoffTable;
static SVGAttrTearoffTable<SVGAnimatedIntegerPair,
                           SVGAnimatedIntegerPair::DOMAnimatedInteger>
    sSVGSecondAnimatedIntegerTearoffTable;

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
static nsresult ParseIntegerOptionalInteger(const nsAString& aValue,
                                            int32_t aValues[2]) {
  nsCharSeparatedTokenizerTemplate<nsContentUtils::IsHTMLWhitespace> tokenizer(
      aValue, ',', nsCharSeparatedTokenizer::SEPARATOR_OPTIONAL);
  if (tokenizer.whitespaceBeforeFirstToken()) {
    return NS_ERROR_DOM_SYNTAX_ERR;
  }
||||||| merged common ancestors
static nsresult
ParseIntegerOptionalInteger(const nsAString& aValue,
                            int32_t aValues[2])
{
  nsCharSeparatedTokenizerTemplate<nsContentUtils::IsHTMLWhitespace>
    tokenizer(aValue, ',', nsCharSeparatedTokenizer::SEPARATOR_OPTIONAL);
  if (tokenizer.whitespaceBeforeFirstToken()) {
    return NS_ERROR_DOM_SYNTAX_ERR;
  }
=======
/* Implementation */
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp

static nsresult ParseIntegerOptionalInteger(const nsAString& aValue,
                                            int32_t aValues[2]) {
  nsCharSeparatedTokenizerTemplate<nsContentUtils::IsHTMLWhitespace> tokenizer(
      aValue, ',', nsCharSeparatedTokenizer::SEPARATOR_OPTIONAL);
  uint32_t i;
  for (i = 0; i < 2 && tokenizer.hasMoreTokens(); ++i) {
    if (!SVGContentUtils::ParseInteger(tokenizer.nextToken(), aValues[i])) {
      return NS_ERROR_DOM_SYNTAX_ERR;
    }
  }
  if (i == 1) {
    aValues[1] = aValues[0];
  }

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
  if (i == 0 ||                                   // Too few values.
      tokenizer.hasMoreTokens() ||                // Too many values.
      tokenizer.whitespaceAfterCurrentToken() ||  // Trailing whitespace.
      tokenizer.separatorAfterCurrentToken()) {   // Trailing comma.
||||||| merged common ancestors
  if (i == 0                    ||                // Too few values.
      tokenizer.hasMoreTokens() ||                // Too many values.
      tokenizer.whitespaceAfterCurrentToken() ||  // Trailing whitespace.
      tokenizer.separatorAfterCurrentToken()) {   // Trailing comma.
=======
  if (i == 0 ||                                  // Too few values.
      tokenizer.hasMoreTokens() ||               // Too many values.
      tokenizer.separatorAfterCurrentToken()) {  // Trailing comma.
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp
    return NS_ERROR_DOM_SYNTAX_ERR;
  }

  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
nsresult nsSVGIntegerPair::SetBaseValueString(const nsAString& aValueAsString,
                                              nsSVGElement* aSVGElement) {
||||||| merged common ancestors
nsresult
nsSVGIntegerPair::SetBaseValueString(const nsAString &aValueAsString,
                                     nsSVGElement *aSVGElement)
{
=======
nsresult SVGAnimatedIntegerPair::SetBaseValueString(
    const nsAString& aValueAsString, SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp
  int32_t val[2];

  nsresult rv = ParseIntegerOptionalInteger(aValueAsString, val);

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

  // We don't need to call DidChange* here - we're only called by
  // SVGElement::ParseAttribute under Element::SetAttr,
  // which takes care of notifying.
  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
void nsSVGIntegerPair::GetBaseValueString(nsAString& aValueAsString) const {
||||||| merged common ancestors
void
nsSVGIntegerPair::GetBaseValueString(nsAString &aValueAsString) const
{
=======
void SVGAnimatedIntegerPair::GetBaseValueString(
    nsAString& aValueAsString) const {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp
  aValueAsString.Truncate();
  aValueAsString.AppendInt(mBaseVal[0]);
  if (mBaseVal[0] != mBaseVal[1]) {
    aValueAsString.AppendLiteral(", ");
    aValueAsString.AppendInt(mBaseVal[1]);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
void nsSVGIntegerPair::SetBaseValue(int32_t aValue, PairIndex aPairIndex,
                                    nsSVGElement* aSVGElement) {
||||||| merged common ancestors
void
nsSVGIntegerPair::SetBaseValue(int32_t aValue, PairIndex aPairIndex,
                               nsSVGElement *aSVGElement)
{
=======
void SVGAnimatedIntegerPair::SetBaseValue(int32_t aValue, PairIndex aPairIndex,
                                          SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp
  uint32_t index = (aPairIndex == eFirst ? 0 : 1);
  if (mIsBaseSet && mBaseVal[index] == aValue) {
    return;
  }

  nsAttrValue emptyOrOldValue = aSVGElement->WillChangeIntegerPair(mAttrEnum);
  mBaseVal[index] = aValue;
  mIsBaseSet = true;
  if (!mIsAnimated) {
    mAnimVal[index] = aValue;
  } else {
    aSVGElement->AnimationNeedsResample();
  }
  aSVGElement->DidChangeIntegerPair(mAttrEnum, emptyOrOldValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
void nsSVGIntegerPair::SetBaseValues(int32_t aValue1, int32_t aValue2,
                                     nsSVGElement* aSVGElement) {
||||||| merged common ancestors
void
nsSVGIntegerPair::SetBaseValues(int32_t aValue1, int32_t aValue2,
                                nsSVGElement *aSVGElement)
{
=======
void SVGAnimatedIntegerPair::SetBaseValues(int32_t aValue1, int32_t aValue2,
                                           SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp
  if (mIsBaseSet && mBaseVal[0] == aValue1 && mBaseVal[1] == aValue2) {
    return;
  }

  nsAttrValue emptyOrOldValue = aSVGElement->WillChangeIntegerPair(mAttrEnum);
  mBaseVal[0] = aValue1;
  mBaseVal[1] = aValue2;
  mIsBaseSet = true;
  if (!mIsAnimated) {
    mAnimVal[0] = aValue1;
    mAnimVal[1] = aValue2;
  } else {
    aSVGElement->AnimationNeedsResample();
  }
  aSVGElement->DidChangeIntegerPair(mAttrEnum, emptyOrOldValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
void nsSVGIntegerPair::SetAnimValue(const int32_t aValue[2],
                                    nsSVGElement* aSVGElement) {
||||||| merged common ancestors
void
nsSVGIntegerPair::SetAnimValue(const int32_t aValue[2], nsSVGElement *aSVGElement)
{
=======
void SVGAnimatedIntegerPair::SetAnimValue(const int32_t aValue[2],
                                          SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp
  if (mIsAnimated && mAnimVal[0] == aValue[0] && mAnimVal[1] == aValue[1]) {
    return;
  }
  mAnimVal[0] = aValue[0];
  mAnimVal[1] = aValue[1];
  mIsAnimated = true;
  aSVGElement->DidAnimateIntegerPair(mAttrEnum);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
already_AddRefed<SVGAnimatedInteger> nsSVGIntegerPair::ToDOMAnimatedInteger(
    PairIndex aIndex, nsSVGElement* aSVGElement) {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedInteger>
nsSVGIntegerPair::ToDOMAnimatedInteger(PairIndex aIndex,
                                       nsSVGElement* aSVGElement)
{
=======
already_AddRefed<DOMSVGAnimatedInteger>
SVGAnimatedIntegerPair::ToDOMAnimatedInteger(PairIndex aIndex,
                                             SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp
  RefPtr<DOMAnimatedInteger> domAnimatedInteger =
      aIndex == eFirst ? sSVGFirstAnimatedIntegerTearoffTable.GetTearoff(this)
                       : sSVGSecondAnimatedIntegerTearoffTable.GetTearoff(this);
  if (!domAnimatedInteger) {
    domAnimatedInteger = new DOMAnimatedInteger(this, aIndex, aSVGElement);
    if (aIndex == eFirst) {
      sSVGFirstAnimatedIntegerTearoffTable.AddTearoff(this, domAnimatedInteger);
    } else {
      sSVGSecondAnimatedIntegerTearoffTable.AddTearoff(this,
                                                       domAnimatedInteger);
    }
  }

  return domAnimatedInteger.forget();
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
nsSVGIntegerPair::DOMAnimatedInteger::~DOMAnimatedInteger() {
||||||| merged common ancestors
nsSVGIntegerPair::DOMAnimatedInteger::~DOMAnimatedInteger()
{
=======
SVGAnimatedIntegerPair::DOMAnimatedInteger::~DOMAnimatedInteger() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp
  if (mIndex == eFirst) {
    sSVGFirstAnimatedIntegerTearoffTable.RemoveTearoff(mVal);
  } else {
    sSVGSecondAnimatedIntegerTearoffTable.RemoveTearoff(mVal);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
UniquePtr<nsISMILAttr> nsSVGIntegerPair::ToSMILAttr(nsSVGElement* aSVGElement) {
||||||| merged common ancestors
UniquePtr<nsISMILAttr>
nsSVGIntegerPair::ToSMILAttr(nsSVGElement *aSVGElement)
{
=======
UniquePtr<SMILAttr> SVGAnimatedIntegerPair::ToSMILAttr(
    SVGElement* aSVGElement) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp
  return MakeUnique<SMILIntegerPair>(this, aSVGElement);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
nsresult nsSVGIntegerPair::SMILIntegerPair::ValueFromString(
    const nsAString& aStr, const dom::SVGAnimationElement* /*aSrcElement*/,
    nsSMILValue& aValue, bool& aPreventCachingOfSandwich) const {
||||||| merged common ancestors
nsresult
nsSVGIntegerPair::SMILIntegerPair::ValueFromString(const nsAString& aStr,
                                                   const dom::SVGAnimationElement* /*aSrcElement*/,
                                                   nsSMILValue& aValue,
                                                   bool& aPreventCachingOfSandwich) const
{
=======
nsresult SVGAnimatedIntegerPair::SMILIntegerPair::ValueFromString(
    const nsAString& aStr, const dom::SVGAnimationElement* /*aSrcElement*/,
    SMILValue& aValue, bool& aPreventCachingOfSandwich) const {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp
  int32_t values[2];

  nsresult rv = ParseIntegerOptionalInteger(aStr, values);
  if (NS_FAILED(rv)) {
    return rv;
  }

  SMILValue val(SVGIntegerPairSMILType::Singleton());
  val.mU.mIntPair[0] = values[0];
  val.mU.mIntPair[1] = values[1];
  aValue = val;
  aPreventCachingOfSandwich = false;

  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
nsSMILValue nsSVGIntegerPair::SMILIntegerPair::GetBaseValue() const {
  nsSMILValue val(SVGIntegerPairSMILType::Singleton());
||||||| merged common ancestors
nsSMILValue
nsSVGIntegerPair::SMILIntegerPair::GetBaseValue() const
{
  nsSMILValue val(SVGIntegerPairSMILType::Singleton());
=======
SMILValue SVGAnimatedIntegerPair::SMILIntegerPair::GetBaseValue() const {
  SMILValue val(SVGIntegerPairSMILType::Singleton());
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp
  val.mU.mIntPair[0] = mVal->mBaseVal[0];
  val.mU.mIntPair[1] = mVal->mBaseVal[1];
  return val;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
void nsSVGIntegerPair::SMILIntegerPair::ClearAnimValue() {
||||||| merged common ancestors
void
nsSVGIntegerPair::SMILIntegerPair::ClearAnimValue()
{
=======
void SVGAnimatedIntegerPair::SMILIntegerPair::ClearAnimValue() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp
  if (mVal->mIsAnimated) {
    mVal->mIsAnimated = false;
    mVal->mAnimVal[0] = mVal->mBaseVal[0];
    mVal->mAnimVal[1] = mVal->mBaseVal[1];
    mSVGElement->DidAnimateIntegerPair(mVal->mAttrEnum);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGIntegerPair.cpp
nsresult nsSVGIntegerPair::SMILIntegerPair::SetAnimValue(
    const nsSMILValue& aValue) {
||||||| merged common ancestors
nsresult
nsSVGIntegerPair::SMILIntegerPair::SetAnimValue(const nsSMILValue& aValue)
{
=======
nsresult SVGAnimatedIntegerPair::SMILIntegerPair::SetAnimValue(
    const SMILValue& aValue) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGAnimatedIntegerPair.cpp
  NS_ASSERTION(aValue.mType == SVGIntegerPairSMILType::Singleton(),
               "Unexpected type to assign animated value");
  if (aValue.mType == SVGIntegerPairSMILType::Singleton()) {
    mVal->SetAnimValue(aValue.mU.mIntPair, mSVGElement);
  }
  return NS_OK;
}

}  // namespace mozilla
