/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SMILAnimationFunction.h"

#include "mozilla/dom/SVGAnimationElement.h"
#include "mozilla/DebugOnly.h"
#include "mozilla/Move.h"
#include "mozilla/SMILAttr.h"
#include "mozilla/SMILCSSValueType.h"
#include "mozilla/SMILNullType.h"
#include "mozilla/SMILParserUtils.h"
#include "mozilla/SMILTimedElement.h"
#include "nsAttrValueInlines.h"
#include "nsGkAtoms.h"
#include "nsCOMPtr.h"
#include "nsCOMArray.h"
#include "nsIContent.h"
#include "nsContentUtils.h"
#include "nsReadableUtils.h"
#include "nsString.h"
#include <math.h>
#include <algorithm>

using namespace mozilla::dom;

namespace mozilla {

//----------------------------------------------------------------------
// Static members

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
nsAttrValue::EnumTable nsSMILAnimationFunction::sAccumulateTable[] = {
    {"none", false}, {"sum", true}, {nullptr, 0}};

nsAttrValue::EnumTable nsSMILAnimationFunction::sAdditiveTable[] = {
    {"replace", false}, {"sum", true}, {nullptr, 0}};

nsAttrValue::EnumTable nsSMILAnimationFunction::sCalcModeTable[] = {
    {"linear", CALC_LINEAR},
    {"discrete", CALC_DISCRETE},
    {"paced", CALC_PACED},
    {"spline", CALC_SPLINE},
    {nullptr, 0}};
||||||| merged common ancestors
nsAttrValue::EnumTable nsSMILAnimationFunction::sAccumulateTable[] = {
      {"none", false},
      {"sum", true},
      {nullptr, 0}
};

nsAttrValue::EnumTable nsSMILAnimationFunction::sAdditiveTable[] = {
      {"replace", false},
      {"sum", true},
      {nullptr, 0}
};

nsAttrValue::EnumTable nsSMILAnimationFunction::sCalcModeTable[] = {
      {"linear", CALC_LINEAR},
      {"discrete", CALC_DISCRETE},
      {"paced", CALC_PACED},
      {"spline", CALC_SPLINE},
      {nullptr, 0}
};
=======
nsAttrValue::EnumTable SMILAnimationFunction::sAccumulateTable[] = {
    {"none", false}, {"sum", true}, {nullptr, 0}};

nsAttrValue::EnumTable SMILAnimationFunction::sAdditiveTable[] = {
    {"replace", false}, {"sum", true}, {nullptr, 0}};

nsAttrValue::EnumTable SMILAnimationFunction::sCalcModeTable[] = {
    {"linear", CALC_LINEAR},
    {"discrete", CALC_DISCRETE},
    {"paced", CALC_PACED},
    {"spline", CALC_SPLINE},
    {nullptr, 0}};
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp

// Any negative number should be fine as a sentinel here,
// because valid distances are non-negative.
#define COMPUTE_DISTANCE_ERROR (-1)

//----------------------------------------------------------------------
// Constructors etc.

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
nsSMILAnimationFunction::nsSMILAnimationFunction()
    : mSampleTime(-1),
      mRepeatIteration(0),
      mBeginTime(INT64_MIN),
      mAnimationElement(nullptr),
      mErrorFlags(0),
      mIsActive(false),
      mIsFrozen(false),
      mLastValue(false),
      mHasChanged(true),
      mValueNeedsReparsingEverySample(false),
      mPrevSampleWasSingleValueAnimation(false),
      mWasSkippedInPrevSample(false) {}

void nsSMILAnimationFunction::SetAnimationElement(
    SVGAnimationElement* aAnimationElement) {
||||||| merged common ancestors
nsSMILAnimationFunction::nsSMILAnimationFunction()
  : mSampleTime(-1),
    mRepeatIteration(0),
    mBeginTime(INT64_MIN),
    mAnimationElement(nullptr),
    mErrorFlags(0),
    mIsActive(false),
    mIsFrozen(false),
    mLastValue(false),
    mHasChanged(true),
    mValueNeedsReparsingEverySample(false),
    mPrevSampleWasSingleValueAnimation(false),
    mWasSkippedInPrevSample(false)
{
}

void
nsSMILAnimationFunction::SetAnimationElement(
    SVGAnimationElement* aAnimationElement)
{
=======
SMILAnimationFunction::SMILAnimationFunction()
    : mSampleTime(-1),
      mRepeatIteration(0),
      mBeginTime(INT64_MIN),
      mAnimationElement(nullptr),
      mErrorFlags(0),
      mIsActive(false),
      mIsFrozen(false),
      mLastValue(false),
      mHasChanged(true),
      mValueNeedsReparsingEverySample(false),
      mPrevSampleWasSingleValueAnimation(false),
      mWasSkippedInPrevSample(false) {}

void SMILAnimationFunction::SetAnimationElement(
    SVGAnimationElement* aAnimationElement) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  mAnimationElement = aAnimationElement;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
bool nsSMILAnimationFunction::SetAttr(nsAtom* aAttribute,
                                      const nsAString& aValue,
                                      nsAttrValue& aResult,
                                      nsresult* aParseResult) {
||||||| merged common ancestors
bool
nsSMILAnimationFunction::SetAttr(nsAtom* aAttribute, const nsAString& aValue,
                                 nsAttrValue& aResult, nsresult* aParseResult)
{
=======
bool SMILAnimationFunction::SetAttr(nsAtom* aAttribute, const nsAString& aValue,
                                    nsAttrValue& aResult,
                                    nsresult* aParseResult) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  bool foundMatch = true;
  nsresult parseResult = NS_OK;

  // The attributes 'by', 'from', 'to', and 'values' may be parsed differently
  // depending on the element & attribute we're animating.  So instead of
  // parsing them now we re-parse them at every sample.
  if (aAttribute == nsGkAtoms::by || aAttribute == nsGkAtoms::from ||
      aAttribute == nsGkAtoms::to || aAttribute == nsGkAtoms::values) {
    // We parse to, from, by, values at sample time.
    // XXX Need to flag which attribute has changed and then when we parse it at
    // sample time, report any errors and reset the flag
    mHasChanged = true;
    aResult.SetTo(aValue);
  } else if (aAttribute == nsGkAtoms::accumulate) {
    parseResult = SetAccumulate(aValue, aResult);
  } else if (aAttribute == nsGkAtoms::additive) {
    parseResult = SetAdditive(aValue, aResult);
  } else if (aAttribute == nsGkAtoms::calcMode) {
    parseResult = SetCalcMode(aValue, aResult);
  } else if (aAttribute == nsGkAtoms::keyTimes) {
    parseResult = SetKeyTimes(aValue, aResult);
  } else if (aAttribute == nsGkAtoms::keySplines) {
    parseResult = SetKeySplines(aValue, aResult);
  } else {
    foundMatch = false;
  }

  if (foundMatch && aParseResult) {
    *aParseResult = parseResult;
  }

  return foundMatch;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
bool nsSMILAnimationFunction::UnsetAttr(nsAtom* aAttribute) {
||||||| merged common ancestors
bool
nsSMILAnimationFunction::UnsetAttr(nsAtom* aAttribute)
{
=======
bool SMILAnimationFunction::UnsetAttr(nsAtom* aAttribute) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  bool foundMatch = true;

  if (aAttribute == nsGkAtoms::by || aAttribute == nsGkAtoms::from ||
      aAttribute == nsGkAtoms::to || aAttribute == nsGkAtoms::values) {
    mHasChanged = true;
  } else if (aAttribute == nsGkAtoms::accumulate) {
    UnsetAccumulate();
  } else if (aAttribute == nsGkAtoms::additive) {
    UnsetAdditive();
  } else if (aAttribute == nsGkAtoms::calcMode) {
    UnsetCalcMode();
  } else if (aAttribute == nsGkAtoms::keyTimes) {
    UnsetKeyTimes();
  } else if (aAttribute == nsGkAtoms::keySplines) {
    UnsetKeySplines();
  } else {
    foundMatch = false;
  }

  return foundMatch;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
void nsSMILAnimationFunction::SampleAt(nsSMILTime aSampleTime,
                                       const nsSMILTimeValue& aSimpleDuration,
                                       uint32_t aRepeatIteration) {
||||||| merged common ancestors
void
nsSMILAnimationFunction::SampleAt(nsSMILTime aSampleTime,
                                  const nsSMILTimeValue& aSimpleDuration,
                                  uint32_t aRepeatIteration)
{
=======
void SMILAnimationFunction::SampleAt(SMILTime aSampleTime,
                                     const SMILTimeValue& aSimpleDuration,
                                     uint32_t aRepeatIteration) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  // * Update mHasChanged ("Might this sample be different from prev one?")
  // Were we previously sampling a fill="freeze" final val? (We're not anymore.)
  mHasChanged |= mLastValue;

  // Are we sampling at a new point in simple duration? And does that matter?
  mHasChanged |=
      (mSampleTime != aSampleTime || mSimpleDuration != aSimpleDuration) &&
      !IsValueFixedForSimpleDuration();

  // Are we on a new repeat and accumulating across repeats?
  if (!mErrorFlags) {  // (can't call GetAccumulate() if we've had parse errors)
    mHasChanged |= (mRepeatIteration != aRepeatIteration) && GetAccumulate();
  }

  mSampleTime = aSampleTime;
  mSimpleDuration = aSimpleDuration;
  mRepeatIteration = aRepeatIteration;
  mLastValue = false;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
void nsSMILAnimationFunction::SampleLastValue(uint32_t aRepeatIteration) {
||||||| merged common ancestors
void
nsSMILAnimationFunction::SampleLastValue(uint32_t aRepeatIteration)
{
=======
void SMILAnimationFunction::SampleLastValue(uint32_t aRepeatIteration) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  if (mHasChanged || !mLastValue || mRepeatIteration != aRepeatIteration) {
    mHasChanged = true;
  }

  mRepeatIteration = aRepeatIteration;
  mLastValue = true;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
void nsSMILAnimationFunction::Activate(nsSMILTime aBeginTime) {
||||||| merged common ancestors
void
nsSMILAnimationFunction::Activate(nsSMILTime aBeginTime)
{
=======
void SMILAnimationFunction::Activate(SMILTime aBeginTime) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  mBeginTime = aBeginTime;
  mIsActive = true;
  mIsFrozen = false;
  mHasChanged = true;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
void nsSMILAnimationFunction::Inactivate(bool aIsFrozen) {
||||||| merged common ancestors
void
nsSMILAnimationFunction::Inactivate(bool aIsFrozen)
{
=======
void SMILAnimationFunction::Inactivate(bool aIsFrozen) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  mIsActive = false;
  mIsFrozen = aIsFrozen;
  mHasChanged = true;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
void nsSMILAnimationFunction::ComposeResult(const nsISMILAttr& aSMILAttr,
                                            nsSMILValue& aResult) {
||||||| merged common ancestors
void
nsSMILAnimationFunction::ComposeResult(const nsISMILAttr& aSMILAttr,
                                       nsSMILValue& aResult)
{
=======
void SMILAnimationFunction::ComposeResult(const SMILAttr& aSMILAttr,
                                          SMILValue& aResult) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  mHasChanged = false;
  mPrevSampleWasSingleValueAnimation = false;
  mWasSkippedInPrevSample = false;

  // Skip animations that are inactive or in error
  if (!IsActiveOrFrozen() || mErrorFlags != 0) return;

  // Get the animation values
  SMILValueArray values;
  nsresult rv = GetValues(aSMILAttr, values);
  if (NS_FAILED(rv)) return;

  // Check that we have the right number of keySplines and keyTimes
  CheckValueListDependentAttrs(values.Length());
  if (mErrorFlags != 0) return;

  // If this interval is active, we must have a non-negative mSampleTime
  MOZ_ASSERT(mSampleTime >= 0 || !mIsActive,
             "Negative sample time for active animation");
  MOZ_ASSERT(mSimpleDuration.IsResolved() || mLastValue,
             "Unresolved simple duration for active or frozen animation");

  // If we want to add but don't have a base value then just fail outright.
  // This can happen when we skipped getting the base value because there's an
  // animation function in the sandwich that should replace it but that function
  // failed unexpectedly.
  bool isAdditive = IsAdditive();
  if (isAdditive && aResult.IsNull()) return;

  SMILValue result;

  if (values.Length() == 1 && !IsToAnimation()) {
    // Single-valued animation
    result = values[0];
    mPrevSampleWasSingleValueAnimation = true;

  } else if (mLastValue) {
    // Sampling last value
    const SMILValue& last = values[values.Length() - 1];
    result = last;

    // See comment in AccumulateResult: to-animation does not accumulate
    if (!IsToAnimation() && GetAccumulate() && mRepeatIteration) {
      // If the target attribute type doesn't support addition Add will
      // fail leaving result = last
      result.Add(last, mRepeatIteration);
    }

  } else {
    // Interpolation
    if (NS_FAILED(InterpolateResult(values, result, aResult))) return;

    if (NS_FAILED(AccumulateResult(values, result))) return;
  }

  // If additive animation isn't required or isn't supported, set the value.
  if (!isAdditive || NS_FAILED(aResult.SandwichAdd(result))) {
    aResult = std::move(result);
  }
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
int8_t nsSMILAnimationFunction::CompareTo(
    const nsSMILAnimationFunction* aOther) const {
||||||| merged common ancestors
int8_t
nsSMILAnimationFunction::CompareTo(const nsSMILAnimationFunction* aOther) const
{
=======
int8_t SMILAnimationFunction::CompareTo(
    const SMILAnimationFunction* aOther) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  NS_ENSURE_TRUE(aOther, 0);

  NS_ASSERTION(aOther != this, "Trying to compare to self");

  // Inactive animations sort first
  if (!IsActiveOrFrozen() && aOther->IsActiveOrFrozen()) return -1;

  if (IsActiveOrFrozen() && !aOther->IsActiveOrFrozen()) return 1;

  // Sort based on begin time
  if (mBeginTime != aOther->GetBeginTime())
    return mBeginTime > aOther->GetBeginTime() ? 1 : -1;

  // Next sort based on syncbase dependencies: the dependent element sorts after
  // its syncbase
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
  const nsSMILTimedElement& thisTimedElement =
      mAnimationElement->TimedElement();
  const nsSMILTimedElement& otherTimedElement =
      aOther->mAnimationElement->TimedElement();
  if (thisTimedElement.IsTimeDependent(otherTimedElement)) return 1;
  if (otherTimedElement.IsTimeDependent(thisTimedElement)) return -1;
||||||| merged common ancestors
  const nsSMILTimedElement& thisTimedElement =
    mAnimationElement->TimedElement();
  const nsSMILTimedElement& otherTimedElement =
    aOther->mAnimationElement->TimedElement();
  if (thisTimedElement.IsTimeDependent(otherTimedElement))
    return 1;
  if (otherTimedElement.IsTimeDependent(thisTimedElement))
    return -1;
=======
  const SMILTimedElement& thisTimedElement = mAnimationElement->TimedElement();
  const SMILTimedElement& otherTimedElement =
      aOther->mAnimationElement->TimedElement();
  if (thisTimedElement.IsTimeDependent(otherTimedElement)) return 1;
  if (otherTimedElement.IsTimeDependent(thisTimedElement)) return -1;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp

  // Animations that appear later in the document sort after those earlier in
  // the document
  MOZ_ASSERT(mAnimationElement != aOther->mAnimationElement,
             "Two animations cannot have the same animation content element!");

  return (nsContentUtils::PositionIsBefore(mAnimationElement,
                                           aOther->mAnimationElement))
             ? -1
             : 1;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
bool nsSMILAnimationFunction::WillReplace() const {
||||||| merged common ancestors
bool
nsSMILAnimationFunction::WillReplace() const
{
=======
bool SMILAnimationFunction::WillReplace() const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  /*
   * In IsAdditive() we don't consider to-animation to be additive as it is
   * a special case that is dealt with differently in the compositing method.
   * Here, however, we return FALSE for to-animation (i.e. it will NOT replace
   * the underlying value) as it builds on the underlying value.
   */
  return !mErrorFlags && !(IsAdditive() || IsToAnimation());
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
bool nsSMILAnimationFunction::HasChanged() const {
||||||| merged common ancestors
bool
nsSMILAnimationFunction::HasChanged() const
{
=======
bool SMILAnimationFunction::HasChanged() const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  return mHasChanged || mValueNeedsReparsingEverySample;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
bool nsSMILAnimationFunction::UpdateCachedTarget(
    const nsSMILTargetIdentifier& aNewTarget) {
||||||| merged common ancestors
bool
nsSMILAnimationFunction::UpdateCachedTarget(
  const nsSMILTargetIdentifier& aNewTarget)
{
=======
bool SMILAnimationFunction::UpdateCachedTarget(
    const SMILTargetIdentifier& aNewTarget) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  if (!mLastTarget.Equals(aNewTarget)) {
    mLastTarget = aNewTarget;
    return true;
  }
  return false;
}

//----------------------------------------------------------------------
// Implementation helpers

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
nsresult nsSMILAnimationFunction::InterpolateResult(
    const nsSMILValueArray& aValues, nsSMILValue& aResult,
    nsSMILValue& aBaseValue) {
||||||| merged common ancestors
nsresult
nsSMILAnimationFunction::InterpolateResult(const nsSMILValueArray& aValues,
                                           nsSMILValue& aResult,
                                           nsSMILValue& aBaseValue)
{
=======
nsresult SMILAnimationFunction::InterpolateResult(const SMILValueArray& aValues,
                                                  SMILValue& aResult,
                                                  SMILValue& aBaseValue) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  // Sanity check animation values
  if ((!IsToAnimation() && aValues.Length() < 2) ||
      (IsToAnimation() && aValues.Length() != 1)) {
    NS_ERROR("Unexpected number of values");
    return NS_ERROR_FAILURE;
  }

  if (IsToAnimation() && aBaseValue.IsNull()) {
    return NS_ERROR_FAILURE;
  }

  // Get the normalised progress through the simple duration.
  //
  // If we have an indefinite simple duration, just set the progress to be
  // 0 which will give us the expected behaviour of the animation being fixed at
  // its starting point.
  double simpleProgress = 0.0;

  if (mSimpleDuration.IsDefinite()) {
    SMILTime dur = mSimpleDuration.GetMillis();

    MOZ_ASSERT(dur >= 0, "Simple duration should not be negative");
    MOZ_ASSERT(mSampleTime >= 0, "Sample time should not be negative");

    if (mSampleTime >= dur || mSampleTime < 0) {
      NS_ERROR("Animation sampled outside interval");
      return NS_ERROR_FAILURE;
    }

    if (dur > 0) {
      simpleProgress = (double)mSampleTime / dur;
    }  // else leave simpleProgress at 0.0 (e.g. if mSampleTime == dur == 0)
  }

  nsresult rv = NS_OK;
  SMILCalcMode calcMode = GetCalcMode();

  // Force discrete calcMode for visibility since StyleAnimationValue will
  // try to interpolate it using the special clamping behavior defined for
  // CSS.
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
  if (nsSMILCSSValueType::PropertyFromValue(aValues[0]) ==
      eCSSProperty_visibility) {
||||||| merged common ancestors
  if (nsSMILCSSValueType::PropertyFromValue(aValues[0])
        == eCSSProperty_visibility) {
=======
  if (SMILCSSValueType::PropertyFromValue(aValues[0]) ==
      eCSSProperty_visibility) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
    calcMode = CALC_DISCRETE;
  }

  if (calcMode != CALC_DISCRETE) {
    // Get the normalised progress between adjacent values
    const SMILValue* from = nullptr;
    const SMILValue* to = nullptr;
    // Init to -1 to make sure that if we ever forget to set this, the
    // MOZ_ASSERT that tests that intervalProgress is in range will fail.
    double intervalProgress = -1.f;
    if (IsToAnimation()) {
      from = &aBaseValue;
      to = &aValues[0];
      if (calcMode == CALC_PACED) {
        // Note: key[Times/Splines/Points] are ignored for calcMode="paced"
        intervalProgress = simpleProgress;
      } else {
        double scaledSimpleProgress =
            ScaleSimpleProgress(simpleProgress, calcMode);
        intervalProgress = ScaleIntervalProgress(scaledSimpleProgress, 0);
      }
    } else if (calcMode == CALC_PACED) {
      rv = ComputePacedPosition(aValues, simpleProgress, intervalProgress, from,
                                to);
      // Note: If the above call fails, we'll skip the "from->Interpolate"
      // call below, and we'll drop into the CALC_DISCRETE section
      // instead. (as the spec says we should, because our failure was
      // presumably due to the values being non-additive)
    } else {  // calcMode == CALC_LINEAR or calcMode == CALC_SPLINE
      double scaledSimpleProgress =
          ScaleSimpleProgress(simpleProgress, calcMode);
      uint32_t index =
          (uint32_t)floor(scaledSimpleProgress * (aValues.Length() - 1));
      from = &aValues[index];
      to = &aValues[index + 1];
      intervalProgress = scaledSimpleProgress * (aValues.Length() - 1) - index;
      intervalProgress = ScaleIntervalProgress(intervalProgress, index);
    }

    if (NS_SUCCEEDED(rv)) {
      MOZ_ASSERT(from, "NULL from-value during interpolation");
      MOZ_ASSERT(to, "NULL to-value during interpolation");
      MOZ_ASSERT(0.0f <= intervalProgress && intervalProgress < 1.0f,
                 "Interval progress should be in the range [0, 1)");
      rv = from->Interpolate(*to, intervalProgress, aResult);
    }
  }

  // Discrete-CalcMode case
  // Note: If interpolation failed (isn't supported for this type), the SVG
  // spec says to force discrete mode.
  if (calcMode == CALC_DISCRETE || NS_FAILED(rv)) {
    double scaledSimpleProgress =
        ScaleSimpleProgress(simpleProgress, CALC_DISCRETE);

    // Floating-point errors can mean that, for example, a sample time of 29s in
    // a 100s duration animation gives us a simple progress of 0.28999999999
    // instead of the 0.29 we'd expect. Normally this isn't a noticeable
    // problem, but when we have sudden jumps in animation values (such as is
    // the case here with discrete animation) we can get unexpected results.
    //
    // To counteract this, before we perform a floor() on the animation
    // progress, we add a tiny fudge factor to push us into the correct interval
    // in cases where floating-point errors might cause us to fall short.
    static const double kFloatingPointFudgeFactor = 1.0e-16;
    if (scaledSimpleProgress + kFloatingPointFudgeFactor <= 1.0) {
      scaledSimpleProgress += kFloatingPointFudgeFactor;
    }

    if (IsToAnimation()) {
      // We don't follow SMIL 3, 12.6.4, where discrete to animations
      // are the same as <set> animations.  Instead, we treat it as a
      // discrete animation with two values (the underlying value and
      // the to="" value), and honor keyTimes="" as well.
      uint32_t index = (uint32_t)floor(scaledSimpleProgress * 2);
      aResult = index == 0 ? aBaseValue : aValues[0];
    } else {
      uint32_t index = (uint32_t)floor(scaledSimpleProgress * aValues.Length());
      aResult = aValues[index];

      // For animation of CSS properties, normally when interpolating we perform
      // a zero-value fixup which means that empty values (values with type
      // SMILCSSValueType but a null pointer value) are converted into
      // a suitable zero value based on whatever they're being interpolated
      // with. For discrete animation, however, since we don't interpolate,
      // that never happens. In some rare cases, such as discrete non-additive
      // by-animation, we can arrive here with |aResult| being such an empty
      // value so we need to manually perform the fixup.
      //
      // We could define a generic method for this on SMILValue but its faster
      // and simpler to just special case SMILCSSValueType.
      if (aResult.mType == &SMILCSSValueType::sSingleton) {
        // We have currently only ever encountered this case for the first
        // value of a by-animation (which has two values) and since we have no
        // way of testing other cases we just skip them (but assert if we
        // ever do encounter them so that we can add code to handle them).
        if (index + 1 >= aValues.Length()) {
          MOZ_ASSERT(aResult.mU.mPtr, "The last value should not be empty");
        } else {
          // Base the type of the zero value on the next element in the series.
          SMILCSSValueType::FinalizeValue(aResult, aValues[index + 1]);
        }
      }
    }
    rv = NS_OK;
  }
  return rv;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
nsresult nsSMILAnimationFunction::AccumulateResult(
    const nsSMILValueArray& aValues, nsSMILValue& aResult) {
||||||| merged common ancestors
nsresult
nsSMILAnimationFunction::AccumulateResult(const nsSMILValueArray& aValues,
                                          nsSMILValue& aResult)
{
=======
nsresult SMILAnimationFunction::AccumulateResult(const SMILValueArray& aValues,
                                                 SMILValue& aResult) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  if (!IsToAnimation() && GetAccumulate() && mRepeatIteration) {
    const SMILValue& lastValue = aValues[aValues.Length() - 1];

    // If the target attribute type doesn't support addition, Add will
    // fail and we leave aResult untouched.
    aResult.Add(lastValue, mRepeatIteration);
  }

  return NS_OK;
}

/*
 * Given the simple progress for a paced animation, this method:
 *  - determines which two elements of the values array we're in between
 *    (returned as aFrom and aTo)
 *  - determines where we are between them
 *    (returned as aIntervalProgress)
 *
 * Returns NS_OK, or NS_ERROR_FAILURE if our values don't support distance
 * computation.
 */
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
nsresult nsSMILAnimationFunction::ComputePacedPosition(
    const nsSMILValueArray& aValues, double aSimpleProgress,
    double& aIntervalProgress, const nsSMILValue*& aFrom,
    const nsSMILValue*& aTo) {
||||||| merged common ancestors
nsresult
nsSMILAnimationFunction::ComputePacedPosition(const nsSMILValueArray& aValues,
                                              double aSimpleProgress,
                                              double& aIntervalProgress,
                                              const nsSMILValue*& aFrom,
                                              const nsSMILValue*& aTo)
{
=======
nsresult SMILAnimationFunction::ComputePacedPosition(
    const SMILValueArray& aValues, double aSimpleProgress,
    double& aIntervalProgress, const SMILValue*& aFrom, const SMILValue*& aTo) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  NS_ASSERTION(0.0f <= aSimpleProgress && aSimpleProgress < 1.0f,
               "aSimpleProgress is out of bounds");
  NS_ASSERTION(GetCalcMode() == CALC_PACED,
               "Calling paced-specific function, but not in paced mode");
  MOZ_ASSERT(aValues.Length() >= 2, "Unexpected number of values");

  // Trivial case: If we have just 2 values, then there's only one interval
  // for us to traverse, and our progress across that interval is the exact
  // same as our overall progress.
  if (aValues.Length() == 2) {
    aIntervalProgress = aSimpleProgress;
    aFrom = &aValues[0];
    aTo = &aValues[1];
    return NS_OK;
  }

  double totalDistance = ComputePacedTotalDistance(aValues);
  if (totalDistance == COMPUTE_DISTANCE_ERROR) return NS_ERROR_FAILURE;

  // If we have 0 total distance, then it's unclear where our "paced" position
  // should be.  We can just fail, which drops us into discrete animation mode.
  // (That's fine, since our values are apparently indistinguishable anyway.)
  if (totalDistance == 0.0) {
    return NS_ERROR_FAILURE;
  }

  // total distance we should have moved at this point in time.
  // (called 'remainingDist' due to how it's used in loop below)
  double remainingDist = aSimpleProgress * totalDistance;

  // Must be satisfied, because totalDistance is a sum of (non-negative)
  // distances, and aSimpleProgress is non-negative
  NS_ASSERTION(remainingDist >= 0, "distance values must be non-negative");

  // Find where remainingDist puts us in the list of values
  // Note: We could optimize this next loop by caching the
  // interval-distances in an array, but maybe that's excessive.
  for (uint32_t i = 0; i < aValues.Length() - 1; i++) {
    // Note: The following assertion is valid because remainingDist should
    // start out non-negative, and this loop never shaves off more than its
    // current value.
    NS_ASSERTION(remainingDist >= 0, "distance values must be non-negative");

    double curIntervalDist;

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
#ifdef DEBUG
    nsresult rv =
#endif
        aValues[i].ComputeDistance(aValues[i + 1], curIntervalDist);
||||||| merged common ancestors
#ifdef DEBUG
    nsresult rv =
#endif
      aValues[i].ComputeDistance(aValues[i+1], curIntervalDist);
=======
    DebugOnly<nsresult> rv =
        aValues[i].ComputeDistance(aValues[i + 1], curIntervalDist);
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
    MOZ_ASSERT(NS_SUCCEEDED(rv),
               "If we got through ComputePacedTotalDistance, we should "
               "be able to recompute each sub-distance without errors");

    NS_ASSERTION(curIntervalDist >= 0, "distance values must be non-negative");
    // Clamp distance value at 0, just in case ComputeDistance is evil.
    curIntervalDist = std::max(curIntervalDist, 0.0);

    if (remainingDist >= curIntervalDist) {
      remainingDist -= curIntervalDist;
    } else {
      // NOTE: If we get here, then curIntervalDist necessarily is not 0. Why?
      // Because this clause is only hit when remainingDist < curIntervalDist,
      // and if curIntervalDist were 0, that would mean remainingDist would
      // have to be < 0.  But that can't happen, because remainingDist (as
      // a distance) is non-negative by definition.
      NS_ASSERTION(curIntervalDist != 0,
                   "We should never get here with this set to 0...");

      // We found the right spot -- an interpolated position between
      // values i and i+1.
      aFrom = &aValues[i];
      aTo = &aValues[i + 1];
      aIntervalProgress = remainingDist / curIntervalDist;
      return NS_OK;
    }
  }

  MOZ_ASSERT_UNREACHABLE(
      "shouldn't complete loop & get here -- if we do, "
      "then aSimpleProgress was probably out of bounds");
  return NS_ERROR_FAILURE;
}

/*
 * Computes the total distance to be travelled by a paced animation.
 *
 * Returns the total distance, or returns COMPUTE_DISTANCE_ERROR if
 * our values don't support distance computation.
 */
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
double nsSMILAnimationFunction::ComputePacedTotalDistance(
    const nsSMILValueArray& aValues) const {
||||||| merged common ancestors
double
nsSMILAnimationFunction::ComputePacedTotalDistance(
    const nsSMILValueArray& aValues) const
{
=======
double SMILAnimationFunction::ComputePacedTotalDistance(
    const SMILValueArray& aValues) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  NS_ASSERTION(GetCalcMode() == CALC_PACED,
               "Calling paced-specific function, but not in paced mode");

  double totalDistance = 0.0;
  for (uint32_t i = 0; i < aValues.Length() - 1; i++) {
    double tmpDist;
    nsresult rv = aValues[i].ComputeDistance(aValues[i + 1], tmpDist);
    if (NS_FAILED(rv)) {
      return COMPUTE_DISTANCE_ERROR;
    }

    // Clamp distance value to 0, just in case we have an evil ComputeDistance
    // implementation somewhere
    MOZ_ASSERT(tmpDist >= 0.0f, "distance values must be non-negative");
    tmpDist = std::max(tmpDist, 0.0);

    totalDistance += tmpDist;
  }

  return totalDistance;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
double nsSMILAnimationFunction::ScaleSimpleProgress(double aProgress,
                                                    nsSMILCalcMode aCalcMode) {
  if (!HasAttr(nsGkAtoms::keyTimes)) return aProgress;
||||||| merged common ancestors
double
nsSMILAnimationFunction::ScaleSimpleProgress(double aProgress,
                                             nsSMILCalcMode aCalcMode)
{
  if (!HasAttr(nsGkAtoms::keyTimes))
    return aProgress;
=======
double SMILAnimationFunction::ScaleSimpleProgress(double aProgress,
                                                  SMILCalcMode aCalcMode) {
  if (!HasAttr(nsGkAtoms::keyTimes)) return aProgress;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp

  uint32_t numTimes = mKeyTimes.Length();

  if (numTimes < 2) return aProgress;

  uint32_t i = 0;
  for (; i < numTimes - 2 && aProgress >= mKeyTimes[i + 1]; ++i) {
  }

  if (aCalcMode == CALC_DISCRETE) {
    // discrete calcMode behaviour differs in that each keyTime defines the time
    // from when the corresponding value is set, and therefore the last value
    // needn't be 1. So check if we're in the last 'interval', that is, the
    // space between the final value and 1.0.
    if (aProgress >= mKeyTimes[i + 1]) {
      MOZ_ASSERT(i == numTimes - 2,
                 "aProgress is not in range of the current interval, yet the "
                 "current interval is not the last bounded interval either.");
      ++i;
    }
    return (double)i / numTimes;
  }

  double& intervalStart = mKeyTimes[i];
  double& intervalEnd = mKeyTimes[i + 1];

  double intervalLength = intervalEnd - intervalStart;
  if (intervalLength <= 0.0) return intervalStart;

  return (i + (aProgress - intervalStart) / intervalLength) /
         double(numTimes - 1);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
double nsSMILAnimationFunction::ScaleIntervalProgress(double aProgress,
                                                      uint32_t aIntervalIndex) {
  if (GetCalcMode() != CALC_SPLINE) return aProgress;
||||||| merged common ancestors
double
nsSMILAnimationFunction::ScaleIntervalProgress(double aProgress,
                                               uint32_t aIntervalIndex)
{
  if (GetCalcMode() != CALC_SPLINE)
    return aProgress;
=======
double SMILAnimationFunction::ScaleIntervalProgress(double aProgress,
                                                    uint32_t aIntervalIndex) {
  if (GetCalcMode() != CALC_SPLINE) return aProgress;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp

  if (!HasAttr(nsGkAtoms::keySplines)) return aProgress;

  MOZ_ASSERT(aIntervalIndex < mKeySplines.Length(), "Invalid interval index");

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
  nsSMILKeySpline const& spline = mKeySplines[aIntervalIndex];
||||||| merged common ancestors
  nsSMILKeySpline const &spline = mKeySplines[aIntervalIndex];
=======
  SMILKeySpline const& spline = mKeySplines[aIntervalIndex];
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  return spline.GetSplineValue(aProgress);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
bool nsSMILAnimationFunction::HasAttr(nsAtom* aAttName) const {
||||||| merged common ancestors
bool
nsSMILAnimationFunction::HasAttr(nsAtom* aAttName) const
{
=======
bool SMILAnimationFunction::HasAttr(nsAtom* aAttName) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  return mAnimationElement->HasAttr(aAttName);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
const nsAttrValue* nsSMILAnimationFunction::GetAttr(nsAtom* aAttName) const {
||||||| merged common ancestors
const nsAttrValue*
nsSMILAnimationFunction::GetAttr(nsAtom* aAttName) const
{
=======
const nsAttrValue* SMILAnimationFunction::GetAttr(nsAtom* aAttName) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  return mAnimationElement->GetParsedAttr(aAttName);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
bool nsSMILAnimationFunction::GetAttr(nsAtom* aAttName,
                                      nsAString& aResult) const {
||||||| merged common ancestors
bool
nsSMILAnimationFunction::GetAttr(nsAtom* aAttName, nsAString& aResult) const
{
=======
bool SMILAnimationFunction::GetAttr(nsAtom* aAttName,
                                    nsAString& aResult) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  return mAnimationElement->GetAttr(aAttName, aResult);
}

/*
 * A utility function to make querying an attribute that corresponds to an
 * SMILValue a little neater.
 *
 * @param aAttName    The attribute name (in the global namespace).
 * @param aSMILAttr   The SMIL attribute to perform the parsing.
 * @param[out] aResult        The resulting SMILValue.
 * @param[out] aPreventCachingOfSandwich
 *                    If |aResult| contains dependencies on its context that
 *                    should prevent the result of the animation sandwich from
 *                    being cached and reused in future samples (as reported
 *                    by SMILAttr::ValueFromString), then this outparam
 *                    will be set to true. Otherwise it is left unmodified.
 *
 * Returns false if a parse error occurred, otherwise returns true.
 */
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
bool nsSMILAnimationFunction::ParseAttr(nsAtom* aAttName,
                                        const nsISMILAttr& aSMILAttr,
                                        nsSMILValue& aResult,
                                        bool& aPreventCachingOfSandwich) const {
||||||| merged common ancestors
bool
nsSMILAnimationFunction::ParseAttr(nsAtom* aAttName,
                                   const nsISMILAttr& aSMILAttr,
                                   nsSMILValue& aResult,
                                   bool& aPreventCachingOfSandwich) const
{
=======
bool SMILAnimationFunction::ParseAttr(nsAtom* aAttName,
                                      const SMILAttr& aSMILAttr,
                                      SMILValue& aResult,
                                      bool& aPreventCachingOfSandwich) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  nsAutoString attValue;
  if (GetAttr(aAttName, attValue)) {
    bool preventCachingOfSandwich = false;
    nsresult rv = aSMILAttr.ValueFromString(attValue, mAnimationElement,
                                            aResult, preventCachingOfSandwich);
    if (NS_FAILED(rv)) return false;

    if (preventCachingOfSandwich) {
      aPreventCachingOfSandwich = true;
    }
  }
  return true;
}

/*
 * SMILANIM specifies the following rules for animation function values:
 *
 * (1) if values is set, it overrides everything
 * (2) for from/to/by animation at least to or by must be specified, from on its
 *     own (or nothing) is an error--which we will ignore
 * (3) if both by and to are specified only to will be used, by will be ignored
 * (4) if by is specified without from (by animation), forces additive behaviour
 * (5) if to is specified without from (to animation), special care needs to be
 *     taken when compositing animation as such animations are composited last.
 *
 * This helper method applies these rules to fill in the values list and to set
 * some internal state.
 */
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
nsresult nsSMILAnimationFunction::GetValues(const nsISMILAttr& aSMILAttr,
                                            nsSMILValueArray& aResult) {
  if (!mAnimationElement) return NS_ERROR_FAILURE;
||||||| merged common ancestors
nsresult
nsSMILAnimationFunction::GetValues(const nsISMILAttr& aSMILAttr,
                                   nsSMILValueArray& aResult)
{
  if (!mAnimationElement)
    return NS_ERROR_FAILURE;
=======
nsresult SMILAnimationFunction::GetValues(const SMILAttr& aSMILAttr,
                                          SMILValueArray& aResult) {
  if (!mAnimationElement) return NS_ERROR_FAILURE;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp

  mValueNeedsReparsingEverySample = false;
  SMILValueArray result;

  // If "values" is set, use it
  if (HasAttr(nsGkAtoms::values)) {
    nsAutoString attValue;
    GetAttr(nsGkAtoms::values, attValue);
    bool preventCachingOfSandwich = false;
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
    if (!nsSMILParserUtils::ParseValues(attValue, mAnimationElement, aSMILAttr,
                                        result, preventCachingOfSandwich)) {
||||||| merged common ancestors
    if (!nsSMILParserUtils::ParseValues(attValue, mAnimationElement,
                                        aSMILAttr, result,
                                        preventCachingOfSandwich)) {
=======
    if (!SMILParserUtils::ParseValues(attValue, mAnimationElement, aSMILAttr,
                                      result, preventCachingOfSandwich)) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
      return NS_ERROR_FAILURE;
    }

    if (preventCachingOfSandwich) {
      mValueNeedsReparsingEverySample = true;
    }
    // Else try to/from/by
  } else {
    bool preventCachingOfSandwich = false;
    bool parseOk = true;
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
    nsSMILValue to, from, by;
    parseOk &=
        ParseAttr(nsGkAtoms::to, aSMILAttr, to, preventCachingOfSandwich);
    parseOk &=
        ParseAttr(nsGkAtoms::from, aSMILAttr, from, preventCachingOfSandwich);
    parseOk &=
        ParseAttr(nsGkAtoms::by, aSMILAttr, by, preventCachingOfSandwich);
||||||| merged common ancestors
    nsSMILValue to, from, by;
    parseOk &= ParseAttr(nsGkAtoms::to,   aSMILAttr, to,
                         preventCachingOfSandwich);
    parseOk &= ParseAttr(nsGkAtoms::from, aSMILAttr, from,
                         preventCachingOfSandwich);
    parseOk &= ParseAttr(nsGkAtoms::by,   aSMILAttr, by,
                         preventCachingOfSandwich);
=======
    SMILValue to, from, by;
    parseOk &=
        ParseAttr(nsGkAtoms::to, aSMILAttr, to, preventCachingOfSandwich);
    parseOk &=
        ParseAttr(nsGkAtoms::from, aSMILAttr, from, preventCachingOfSandwich);
    parseOk &=
        ParseAttr(nsGkAtoms::by, aSMILAttr, by, preventCachingOfSandwich);
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp

    if (preventCachingOfSandwich) {
      mValueNeedsReparsingEverySample = true;
    }

    if (!parseOk || !result.SetCapacity(2, fallible)) {
      return NS_ERROR_FAILURE;
    }

    // AppendElement() below must succeed, because SetCapacity() succeeded.
    if (!to.IsNull()) {
      if (!from.IsNull()) {
        MOZ_ALWAYS_TRUE(result.AppendElement(from, fallible));
        MOZ_ALWAYS_TRUE(result.AppendElement(to, fallible));
      } else {
        MOZ_ALWAYS_TRUE(result.AppendElement(to, fallible));
      }
    } else if (!by.IsNull()) {
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
      nsSMILValue effectiveFrom(by.mType);
      if (!from.IsNull()) effectiveFrom = from;
||||||| merged common ancestors
      nsSMILValue effectiveFrom(by.mType);
      if (!from.IsNull())
        effectiveFrom = from;
=======
      SMILValue effectiveFrom(by.mType);
      if (!from.IsNull()) effectiveFrom = from;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
      // Set values to 'from; from + by'
      MOZ_ALWAYS_TRUE(result.AppendElement(effectiveFrom, fallible));
      SMILValue effectiveTo(effectiveFrom);
      if (!effectiveTo.IsNull() && NS_SUCCEEDED(effectiveTo.Add(by))) {
        MOZ_ALWAYS_TRUE(result.AppendElement(effectiveTo, fallible));
      } else {
        // Using by-animation with non-additive type or bad base-value
        return NS_ERROR_FAILURE;
      }
    } else {
      // No values, no to, no by -- call it a day
      return NS_ERROR_FAILURE;
    }
  }

  result.SwapElements(aResult);

  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
void nsSMILAnimationFunction::CheckValueListDependentAttrs(
    uint32_t aNumValues) {
||||||| merged common ancestors
void
nsSMILAnimationFunction::CheckValueListDependentAttrs(uint32_t aNumValues)
{
=======
void SMILAnimationFunction::CheckValueListDependentAttrs(uint32_t aNumValues) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  CheckKeyTimes(aNumValues);
  CheckKeySplines(aNumValues);
}

/**
 * Performs checks for the keyTimes attribute required by the SMIL spec but
 * which depend on other attributes and therefore needs to be updated as
 * dependent attributes are set.
 */
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
void nsSMILAnimationFunction::CheckKeyTimes(uint32_t aNumValues) {
  if (!HasAttr(nsGkAtoms::keyTimes)) return;
||||||| merged common ancestors
void
nsSMILAnimationFunction::CheckKeyTimes(uint32_t aNumValues)
{
  if (!HasAttr(nsGkAtoms::keyTimes))
    return;
=======
void SMILAnimationFunction::CheckKeyTimes(uint32_t aNumValues) {
  if (!HasAttr(nsGkAtoms::keyTimes)) return;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp

  SMILCalcMode calcMode = GetCalcMode();

  // attribute is ignored for calcMode = paced
  if (calcMode == CALC_PACED) {
    SetKeyTimesErrorFlag(false);
    return;
  }

  uint32_t numKeyTimes = mKeyTimes.Length();
  if (numKeyTimes < 1) {
    // keyTimes isn't set or failed preliminary checks
    SetKeyTimesErrorFlag(true);
    return;
  }

  // no. keyTimes == no. values
  // For to-animation the number of values is considered to be 2.
  bool matchingNumOfValues = numKeyTimes == (IsToAnimation() ? 2 : aNumValues);
  if (!matchingNumOfValues) {
    SetKeyTimesErrorFlag(true);
    return;
  }

  // first value must be 0
  if (mKeyTimes[0] != 0.0) {
    SetKeyTimesErrorFlag(true);
    return;
  }

  // last value must be 1 for linear or spline calcModes
  if (calcMode != CALC_DISCRETE && numKeyTimes > 1 &&
      mKeyTimes[numKeyTimes - 1] != 1.0) {
    SetKeyTimesErrorFlag(true);
    return;
  }

  SetKeyTimesErrorFlag(false);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
void nsSMILAnimationFunction::CheckKeySplines(uint32_t aNumValues) {
||||||| merged common ancestors
void
nsSMILAnimationFunction::CheckKeySplines(uint32_t aNumValues)
{
=======
void SMILAnimationFunction::CheckKeySplines(uint32_t aNumValues) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  // attribute is ignored if calc mode is not spline
  if (GetCalcMode() != CALC_SPLINE) {
    SetKeySplinesErrorFlag(false);
    return;
  }

  // calc mode is spline but the attribute is not set
  if (!HasAttr(nsGkAtoms::keySplines)) {
    SetKeySplinesErrorFlag(false);
    return;
  }

  if (mKeySplines.Length() < 1) {
    // keyTimes isn't set or failed preliminary checks
    SetKeySplinesErrorFlag(true);
    return;
  }

  // ignore splines if there's only one value
  if (aNumValues == 1 && !IsToAnimation()) {
    SetKeySplinesErrorFlag(false);
    return;
  }

  // no. keySpline specs == no. values - 1
  uint32_t splineSpecs = mKeySplines.Length();
  if ((splineSpecs != aNumValues - 1 && !IsToAnimation()) ||
      (IsToAnimation() && splineSpecs != 1)) {
    SetKeySplinesErrorFlag(true);
    return;
  }

  SetKeySplinesErrorFlag(false);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
bool nsSMILAnimationFunction::IsValueFixedForSimpleDuration() const {
||||||| merged common ancestors
bool
nsSMILAnimationFunction::IsValueFixedForSimpleDuration() const
{
=======
bool SMILAnimationFunction::IsValueFixedForSimpleDuration() const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  return mSimpleDuration.IsIndefinite() ||
         (!mHasChanged && mPrevSampleWasSingleValueAnimation);
}

//----------------------------------------------------------------------
// Property getters

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
bool nsSMILAnimationFunction::GetAccumulate() const {
||||||| merged common ancestors
bool
nsSMILAnimationFunction::GetAccumulate() const
{
=======
bool SMILAnimationFunction::GetAccumulate() const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  const nsAttrValue* value = GetAttr(nsGkAtoms::accumulate);
  if (!value) return false;

  return value->GetEnumValue();
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
bool nsSMILAnimationFunction::GetAdditive() const {
||||||| merged common ancestors
bool
nsSMILAnimationFunction::GetAdditive() const
{
=======
bool SMILAnimationFunction::GetAdditive() const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  const nsAttrValue* value = GetAttr(nsGkAtoms::additive);
  if (!value) return false;

  return value->GetEnumValue();
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
nsSMILAnimationFunction::nsSMILCalcMode nsSMILAnimationFunction::GetCalcMode()
    const {
||||||| merged common ancestors
nsSMILAnimationFunction::nsSMILCalcMode
nsSMILAnimationFunction::GetCalcMode() const
{
=======
SMILAnimationFunction::SMILCalcMode SMILAnimationFunction::GetCalcMode() const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  const nsAttrValue* value = GetAttr(nsGkAtoms::calcMode);
  if (!value) return CALC_LINEAR;

  return SMILCalcMode(value->GetEnumValue());
}

//----------------------------------------------------------------------
// Property setters / un-setters:

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
nsresult nsSMILAnimationFunction::SetAccumulate(const nsAString& aAccumulate,
                                                nsAttrValue& aResult) {
||||||| merged common ancestors
nsresult
nsSMILAnimationFunction::SetAccumulate(const nsAString& aAccumulate,
                                       nsAttrValue& aResult)
{
=======
nsresult SMILAnimationFunction::SetAccumulate(const nsAString& aAccumulate,
                                              nsAttrValue& aResult) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  mHasChanged = true;
  bool parseResult =
      aResult.ParseEnumValue(aAccumulate, sAccumulateTable, true);
  SetAccumulateErrorFlag(!parseResult);
  return parseResult ? NS_OK : NS_ERROR_FAILURE;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
void nsSMILAnimationFunction::UnsetAccumulate() {
||||||| merged common ancestors
void
nsSMILAnimationFunction::UnsetAccumulate()
{
=======
void SMILAnimationFunction::UnsetAccumulate() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  SetAccumulateErrorFlag(false);
  mHasChanged = true;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
nsresult nsSMILAnimationFunction::SetAdditive(const nsAString& aAdditive,
                                              nsAttrValue& aResult) {
||||||| merged common ancestors
nsresult
nsSMILAnimationFunction::SetAdditive(const nsAString& aAdditive,
                                     nsAttrValue& aResult)
{
=======
nsresult SMILAnimationFunction::SetAdditive(const nsAString& aAdditive,
                                            nsAttrValue& aResult) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  mHasChanged = true;
  bool parseResult = aResult.ParseEnumValue(aAdditive, sAdditiveTable, true);
  SetAdditiveErrorFlag(!parseResult);
  return parseResult ? NS_OK : NS_ERROR_FAILURE;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
void nsSMILAnimationFunction::UnsetAdditive() {
||||||| merged common ancestors
void
nsSMILAnimationFunction::UnsetAdditive()
{
=======
void SMILAnimationFunction::UnsetAdditive() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  SetAdditiveErrorFlag(false);
  mHasChanged = true;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
nsresult nsSMILAnimationFunction::SetCalcMode(const nsAString& aCalcMode,
                                              nsAttrValue& aResult) {
||||||| merged common ancestors
nsresult
nsSMILAnimationFunction::SetCalcMode(const nsAString& aCalcMode,
                                     nsAttrValue& aResult)
{
=======
nsresult SMILAnimationFunction::SetCalcMode(const nsAString& aCalcMode,
                                            nsAttrValue& aResult) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  mHasChanged = true;
  bool parseResult = aResult.ParseEnumValue(aCalcMode, sCalcModeTable, true);
  SetCalcModeErrorFlag(!parseResult);
  return parseResult ? NS_OK : NS_ERROR_FAILURE;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
void nsSMILAnimationFunction::UnsetCalcMode() {
||||||| merged common ancestors
void
nsSMILAnimationFunction::UnsetCalcMode()
{
=======
void SMILAnimationFunction::UnsetCalcMode() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  SetCalcModeErrorFlag(false);
  mHasChanged = true;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
nsresult nsSMILAnimationFunction::SetKeySplines(const nsAString& aKeySplines,
                                                nsAttrValue& aResult) {
||||||| merged common ancestors
nsresult
nsSMILAnimationFunction::SetKeySplines(const nsAString& aKeySplines,
                                       nsAttrValue& aResult)
{
=======
nsresult SMILAnimationFunction::SetKeySplines(const nsAString& aKeySplines,
                                              nsAttrValue& aResult) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  mKeySplines.Clear();
  aResult.SetTo(aKeySplines);

  mHasChanged = true;

  if (!SMILParserUtils::ParseKeySplines(aKeySplines, mKeySplines)) {
    mKeySplines.Clear();
    return NS_ERROR_FAILURE;
  }

  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
void nsSMILAnimationFunction::UnsetKeySplines() {
||||||| merged common ancestors
void
nsSMILAnimationFunction::UnsetKeySplines()
{
=======
void SMILAnimationFunction::UnsetKeySplines() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  mKeySplines.Clear();
  SetKeySplinesErrorFlag(false);
  mHasChanged = true;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
nsresult nsSMILAnimationFunction::SetKeyTimes(const nsAString& aKeyTimes,
                                              nsAttrValue& aResult) {
||||||| merged common ancestors
nsresult
nsSMILAnimationFunction::SetKeyTimes(const nsAString& aKeyTimes,
                                     nsAttrValue& aResult)
{
=======
nsresult SMILAnimationFunction::SetKeyTimes(const nsAString& aKeyTimes,
                                            nsAttrValue& aResult) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  mKeyTimes.Clear();
  aResult.SetTo(aKeyTimes);

  mHasChanged = true;

  if (!SMILParserUtils::ParseSemicolonDelimitedProgressList(aKeyTimes, true,
                                                            mKeyTimes)) {
    mKeyTimes.Clear();
    return NS_ERROR_FAILURE;
  }

  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILAnimationFunction.cpp
void nsSMILAnimationFunction::UnsetKeyTimes() {
||||||| merged common ancestors
void
nsSMILAnimationFunction::UnsetKeyTimes()
{
=======
void SMILAnimationFunction::UnsetKeyTimes() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILAnimationFunction.cpp
  mKeyTimes.Clear();
  SetKeyTimesErrorFlag(false);
  mHasChanged = true;
}

}  // namespace mozilla
