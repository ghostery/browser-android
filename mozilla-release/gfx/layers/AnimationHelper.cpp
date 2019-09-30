/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "AnimationHelper.h"
<<<<<<< HEAD
#include "mozilla/ComputedTimingFunction.h"      // for ComputedTimingFunction
#include "mozilla/dom/AnimationEffectBinding.h"  // for dom::FillMode
#include "mozilla/dom/KeyframeEffectBinding.h"   // for dom::IterationComposite
#include "mozilla/dom/KeyframeEffect.h"       // for dom::KeyFrameEffectReadOnly
#include "mozilla/dom/Nullable.h"             // for dom::Nullable
#include "mozilla/layers/CompositorThread.h"  // for CompositorThreadHolder
#include "mozilla/layers/LayerAnimationUtils.h"  // for TimingFunctionToComputedTimingFunction
#include "mozilla/ServoBindings.h"  // for Servo_ComposeAnimationSegment, etc
#include "mozilla/StyleAnimationValue.h"  // for StyleAnimationValue, etc
#include "nsDeviceContext.h"              // for AppUnitsPerCSSPixel
#include "nsDisplayList.h"                // for nsDisplayTransform, etc
||||||| merged common ancestors
#include "mozilla/ComputedTimingFunction.h" // for ComputedTimingFunction
#include "mozilla/dom/AnimationEffectBinding.h" // for dom::FillMode
#include "mozilla/dom/KeyframeEffectBinding.h" // for dom::IterationComposite
#include "mozilla/dom/KeyframeEffect.h" // for dom::KeyFrameEffectReadOnly
#include "mozilla/dom/Nullable.h" // for dom::Nullable
#include "mozilla/layers/CompositorThread.h" // for CompositorThreadHolder
#include "mozilla/layers/LayerAnimationUtils.h" // for TimingFunctionToComputedTimingFunction
#include "mozilla/ServoBindings.h" // for Servo_ComposeAnimationSegment, etc
#include "mozilla/StyleAnimationValue.h" // for StyleAnimationValue, etc
#include "nsDeviceContext.h"            // for AppUnitsPerCSSPixel
#include "nsDisplayList.h"              // for nsDisplayTransform, etc
=======
#include "mozilla/ComputedTimingFunction.h"      // for ComputedTimingFunction
#include "mozilla/dom/AnimationEffectBinding.h"  // for dom::FillMode
#include "mozilla/dom/KeyframeEffectBinding.h"   // for dom::IterationComposite
#include "mozilla/dom/KeyframeEffect.h"       // for dom::KeyFrameEffectReadOnly
#include "mozilla/dom/Nullable.h"             // for dom::Nullable
#include "mozilla/layers/CompositorThread.h"  // for CompositorThreadHolder
#include "mozilla/layers/LayerAnimationUtils.h"  // for TimingFunctionToComputedTimingFunction
#include "mozilla/LayerAnimationInfo.h"  // for GetCSSPropertiesFor()
#include "mozilla/ServoBindings.h"  // for Servo_ComposeAnimationSegment, etc
#include "mozilla/StyleAnimationValue.h"  // for StyleAnimationValue, etc
#include "nsDeviceContext.h"              // for AppUnitsPerCSSPixel
#include "nsDisplayList.h"                // for nsDisplayTransform, etc
>>>>>>> upstream-releases

namespace mozilla {
namespace layers {

void CompositorAnimationStorage::Clear() {
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());

  mAnimatedValues.Clear();
  mAnimations.Clear();
  mAnimationRenderRoots.Clear();
}

void CompositorAnimationStorage::ClearById(const uint64_t& aId) {
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());

  mAnimatedValues.Remove(aId);
  mAnimations.Remove(aId);
  mAnimationRenderRoots.Remove(aId);
}

AnimatedValue* CompositorAnimationStorage::GetAnimatedValue(
    const uint64_t& aId) const {
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  return mAnimatedValues.Get(aId);
}

OMTAValue CompositorAnimationStorage::GetOMTAValue(const uint64_t& aId) const {
  OMTAValue omtaValue = mozilla::null_t();
  auto animatedValue = GetAnimatedValue(aId);
  if (!animatedValue) {
    return omtaValue;
  }

<<<<<<< HEAD
  switch (animatedValue->mType) {
    case AnimatedValue::COLOR:
      omtaValue = animatedValue->mColor;
      break;
    case AnimatedValue::OPACITY:
      omtaValue = animatedValue->mOpacity;
      break;
    case AnimatedValue::TRANSFORM: {
      gfx::Matrix4x4 transform = animatedValue->mTransform.mFrameTransform;
      const TransformData& data = animatedValue->mTransform.mData;
      float scale = data.appUnitsPerDevPixel();
      gfx::Point3D transformOrigin = data.transformOrigin();

      // Undo the rebasing applied by
      // nsDisplayTransform::GetResultingTransformMatrixInternal
      transform.ChangeBasis(-transformOrigin);

      // Convert to CSS pixels (this undoes the operations performed by
      // nsStyleTransformMatrix::ProcessTranslatePart which is called from
      // nsDisplayTransform::GetResultingTransformMatrix)
      double devPerCss = double(scale) / double(AppUnitsPerCSSPixel());
      transform._41 *= devPerCss;
      transform._42 *= devPerCss;
      transform._43 *= devPerCss;
      omtaValue = transform;
      break;
    }
    case AnimatedValue::NONE:
      break;
  }

||||||| merged common ancestors
  switch (animatedValue->mType) {
    case AnimatedValue::OPACITY:
      omtaValue = animatedValue->mOpacity;
      break;
    case AnimatedValue::TRANSFORM: {
      gfx::Matrix4x4 transform = animatedValue->mTransform.mFrameTransform;
      const TransformData& data = animatedValue->mTransform.mData;
      float scale = data.appUnitsPerDevPixel();
      gfx::Point3D transformOrigin = data.transformOrigin();

      // Undo the rebasing applied by
      // nsDisplayTransform::GetResultingTransformMatrixInternal
      transform.ChangeBasis(-transformOrigin);

      // Convert to CSS pixels (this undoes the operations performed by
      // nsStyleTransformMatrix::ProcessTranslatePart which is called from
      // nsDisplayTransform::GetResultingTransformMatrix)
      double devPerCss =
        double(scale) / double(AppUnitsPerCSSPixel());
      transform._41 *= devPerCss;
      transform._42 *= devPerCss;
      transform._43 *= devPerCss;
      omtaValue = transform;
      break;
    }
    case AnimatedValue::NONE:
      break;
  }

=======
  animatedValue->Value().match(
      [&](const AnimationTransform& aTransform) {
        gfx::Matrix4x4 transform = aTransform.mFrameTransform;
        const TransformData& data = aTransform.mData;
        float scale = data.appUnitsPerDevPixel();
        gfx::Point3D transformOrigin = data.transformOrigin();

        // Undo the rebasing applied by
        // nsDisplayTransform::GetResultingTransformMatrixInternal
        transform.ChangeBasis(-transformOrigin);

        // Convert to CSS pixels (this undoes the operations performed by
        // nsStyleTransformMatrix::ProcessTranslatePart which is called from
        // nsDisplayTransform::GetResultingTransformMatrix)
        double devPerCss = double(scale) / double(AppUnitsPerCSSPixel());
        transform._41 *= devPerCss;
        transform._42 *= devPerCss;
        transform._43 *= devPerCss;
        omtaValue = transform;
      },
      [&](const float& aOpacity) { omtaValue = aOpacity; },
      [&](const nscolor& aColor) { omtaValue = aColor; });
>>>>>>> upstream-releases
  return omtaValue;
}

void CompositorAnimationStorage::SetAnimatedValue(
    uint64_t aId, gfx::Matrix4x4&& aTransformInDevSpace,
    gfx::Matrix4x4&& aFrameTransform, const TransformData& aData) {
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  auto count = mAnimatedValues.Count();
  AnimatedValue* value = mAnimatedValues.LookupOrAdd(
      aId, std::move(aTransformInDevSpace), std::move(aFrameTransform), aData);
  if (count == mAnimatedValues.Count()) {
    MOZ_ASSERT(value->Is<AnimationTransform>());
    *value = AnimatedValue(std::move(aTransformInDevSpace),
                           std::move(aFrameTransform), aData);
  }
}

void CompositorAnimationStorage::SetAnimatedValue(
    uint64_t aId, gfx::Matrix4x4&& aTransformInDevSpace) {
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  const TransformData dontCare = {};
  SetAnimatedValue(aId, std::move(aTransformInDevSpace), gfx::Matrix4x4(),
                   dontCare);
}

<<<<<<< HEAD
void CompositorAnimationStorage::SetAnimatedValue(uint64_t aId,
                                                  nscolor aColor) {
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  auto count = mAnimatedValues.Count();
  AnimatedValue* value = mAnimatedValues.LookupOrAdd(aId, aColor);
  if (count == mAnimatedValues.Count()) {
    MOZ_ASSERT(value->mType == AnimatedValue::COLOR);
    value->mColor = aColor;
  }
}

void CompositorAnimationStorage::SetAnimatedValue(uint64_t aId,
                                                  const float& aOpacity) {
||||||| merged common ancestors
void
CompositorAnimationStorage::SetAnimatedValue(uint64_t aId,
                                             const float& aOpacity)
{
=======
void CompositorAnimationStorage::SetAnimatedValue(uint64_t aId,
                                                  nscolor aColor) {
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  auto count = mAnimatedValues.Count();
  AnimatedValue* value = mAnimatedValues.LookupOrAdd(aId, aColor);
  if (count == mAnimatedValues.Count()) {
    MOZ_ASSERT(value->Is<nscolor>());
    *value = AnimatedValue(aColor);
  }
}

void CompositorAnimationStorage::SetAnimatedValue(uint64_t aId,
                                                  const float& aOpacity) {
>>>>>>> upstream-releases
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  auto count = mAnimatedValues.Count();
  AnimatedValue* value = mAnimatedValues.LookupOrAdd(aId, aOpacity);
  if (count == mAnimatedValues.Count()) {
    MOZ_ASSERT(value->Is<float>());
    *value = AnimatedValue(aOpacity);
  }
}

<<<<<<< HEAD
AnimationArray* CompositorAnimationStorage::GetAnimations(
    const uint64_t& aId) const {
||||||| merged common ancestors
AnimationArray*
CompositorAnimationStorage::GetAnimations(const uint64_t& aId) const
{
=======
nsTArray<PropertyAnimationGroup>* CompositorAnimationStorage::GetAnimations(
    const uint64_t& aId) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  return mAnimations.Get(aId);
}

<<<<<<< HEAD
void CompositorAnimationStorage::SetAnimations(uint64_t aId,
                                               const AnimationArray& aValue) {
||||||| merged common ancestors
void
CompositorAnimationStorage::SetAnimations(uint64_t aId, const AnimationArray& aValue)
{
=======
void CompositorAnimationStorage::SetAnimations(uint64_t aId,
                                               const AnimationArray& aValue,
                                               wr::RenderRoot aRenderRoot) {
>>>>>>> upstream-releases
  MOZ_ASSERT(CompositorThreadHolder::IsInCompositorThread());
  mAnimations.Put(aId, new nsTArray<PropertyAnimationGroup>(
                           AnimationHelper::ExtractAnimations(aValue)));
  mAnimationRenderRoots.Put(aId, aRenderRoot);
}

<<<<<<< HEAD
AnimationHelper::SampleResult AnimationHelper::SampleAnimationForEachNode(
    TimeStamp aPreviousFrameTime, TimeStamp aCurrentFrameTime,
    AnimationArray& aAnimations, InfallibleTArray<AnimData>& aAnimationData,
    RefPtr<RawServoAnimationValue>& aAnimationValue,
    const AnimatedValue* aPreviousValue) {
  MOZ_ASSERT(!aAnimations.IsEmpty(), "Should be called with animations");

||||||| merged common ancestors

AnimationHelper::SampleResult
AnimationHelper::SampleAnimationForEachNode(
  TimeStamp aPreviousFrameTime,
  TimeStamp aCurrentFrameTime,
  AnimationArray& aAnimations,
  InfallibleTArray<AnimData>& aAnimationData,
  RefPtr<RawServoAnimationValue>& aAnimationValue,
  const AnimatedValue* aPreviousValue)
{
  MOZ_ASSERT(!aAnimations.IsEmpty(), "Should be called with animations");

=======
enum class CanSkipCompose {
  IfPossible,
  No,
};
static AnimationHelper::SampleResult SampleAnimationForProperty(
    TimeStamp aPreviousFrameTime, TimeStamp aCurrentFrameTime,
    const AnimatedValue* aPreviousValue, CanSkipCompose aCanSkipCompose,
    nsTArray<PropertyAnimation>& aPropertyAnimations,
    RefPtr<RawServoAnimationValue>& aAnimationValue) {
  MOZ_ASSERT(!aPropertyAnimations.IsEmpty(), "Should have animations");
>>>>>>> upstream-releases
  bool hasInEffectAnimations = false;
#ifdef DEBUG
  // In cases where this function returns a SampleResult::Skipped, we actually
  // do populate aAnimationValue in debug mode, so that we can MOZ_ASSERT at the
  // call site that the value that would have been computed matches the stored
  // value that we end up using. This flag is used to ensure we populate
  // aAnimationValue in this scenario.
  bool shouldBeSkipped = false;
#endif
<<<<<<< HEAD
  // Process in order, since later aAnimations override earlier ones.
  for (size_t i = 0, iEnd = aAnimations.Length(); i < iEnd; ++i) {
    Animation& animation = aAnimations[i];
    AnimData& animData = aAnimationData[i];

    MOZ_ASSERT(
        (!animation.originTime().IsNull() &&
         animation.startTime().type() == MaybeTimeDuration::TTimeDuration) ||
            animation.isNotPlaying(),
        "If we are playing, we should have an origin time and a start"
        " time");
||||||| merged common ancestors
  // Process in order, since later aAnimations override earlier ones.
  for (size_t i = 0, iEnd = aAnimations.Length(); i < iEnd; ++i) {
    Animation& animation = aAnimations[i];
    AnimData& animData = aAnimationData[i];

    MOZ_ASSERT((!animation.originTime().IsNull() &&
                animation.startTime().type() ==
                  MaybeTimeDuration::TTimeDuration) ||
               animation.isNotPlaying(),
               "If we are playing, we should have an origin time and a start"
               " time");
=======
  // Process in order, since later animations override earlier ones.
  for (PropertyAnimation& animation : aPropertyAnimations) {
    MOZ_ASSERT(
        (!animation.mOriginTime.IsNull() && animation.mStartTime.isSome()) ||
            animation.mIsNotPlaying,
        "If we are playing, we should have an origin time and a start time");
>>>>>>> upstream-releases

    // Determine if the animation was play-pending and used a ready time later
    // than the previous frame time.
    //
    // To determine this, _all_ of the following conditions need to hold:
    //
    // * There was no previous animation value (i.e. this is the first frame for
    //   the animation since it was sent to the compositor), and
    // * The animation is playing, and
    // * There is a previous frame time, and
    // * The ready time of the animation is ahead of the previous frame time.
    //
    bool hasFutureReadyTime = false;
<<<<<<< HEAD
    if (!aPreviousValue && !animation.isNotPlaying() &&
||||||| merged common ancestors
    if (!aPreviousValue &&
        !animation.isNotPlaying() &&
=======
    if (!aPreviousValue && !animation.mIsNotPlaying &&
>>>>>>> upstream-releases
        !aPreviousFrameTime.IsNull()) {
      // This is the inverse of the calculation performed in
      // AnimationInfo::StartPendingAnimations to calculate the start time of
      // play-pending animations.
      // Note that we have to calculate (TimeStamp + TimeDuration) last to avoid
      // underflow in the middle of the calulation.
      const TimeStamp readyTime =
<<<<<<< HEAD
          animation.originTime() +
          (animation.startTime().get_TimeDuration() +
           animation.holdTime().MultDouble(1.0 / animation.playbackRate()));
||||||| merged common ancestors
        animation.originTime() +
        (animation.startTime().get_TimeDuration() +
         animation.holdTime().MultDouble(1.0 / animation.playbackRate()));
=======
          animation.mOriginTime +
          (animation.mStartTime.ref() +
           animation.mHoldTime.MultDouble(1.0 / animation.mPlaybackRate));
>>>>>>> upstream-releases
      hasFutureReadyTime =
          !readyTime.IsNull() && readyTime > aPreviousFrameTime;
    }
    // Use the previous vsync time to make main thread animations and compositor
    // more closely aligned.
    //
    // On the first frame where we have animations the previous timestamp will
    // not be set so we simply use the current timestamp.  As a result we will
    // end up painting the first frame twice.  That doesn't appear to be
    // noticeable, however.
    //
    // Likewise, if the animation is play-pending, it may have a ready time that
    // is *after* |aPreviousFrameTime| (but *before* |aCurrentFrameTime|).
    // To avoid flicker we need to use |aCurrentFrameTime| to avoid temporarily
    // jumping backwards into the range prior to when the animation starts.
    const TimeStamp& timeStamp =
        aPreviousFrameTime.IsNull() || hasFutureReadyTime ? aCurrentFrameTime
                                                          : aPreviousFrameTime;

    // If the animation is not currently playing, e.g. paused or
    // finished, then use the hold time to stay at the same position.
    TimeDuration elapsedDuration =
<<<<<<< HEAD
        animation.isNotPlaying() ||
                animation.startTime().type() != MaybeTimeDuration::TTimeDuration
            ? animation.holdTime()
            : (timeStamp - animation.originTime() -
               animation.startTime().get_TimeDuration())
                  .MultDouble(animation.playbackRate());

    ComputedTiming computedTiming = dom::AnimationEffect::GetComputedTimingAt(
        dom::Nullable<TimeDuration>(elapsedDuration), animData.mTiming,
        animation.playbackRate());
||||||| merged common ancestors
      animation.isNotPlaying() ||
      animation.startTime().type() != MaybeTimeDuration::TTimeDuration
      ? animation.holdTime()
      : (timeStamp - animation.originTime() -
         animation.startTime().get_TimeDuration())
        .MultDouble(animation.playbackRate());

    ComputedTiming computedTiming =
      dom::AnimationEffect::GetComputedTimingAt(
        dom::Nullable<TimeDuration>(elapsedDuration), animData.mTiming,
        animation.playbackRate());
=======
        animation.mIsNotPlaying || animation.mStartTime.isNothing()
            ? animation.mHoldTime
            : (timeStamp - animation.mOriginTime - animation.mStartTime.ref())
                  .MultDouble(animation.mPlaybackRate);

    ComputedTiming computedTiming = dom::AnimationEffect::GetComputedTimingAt(
        dom::Nullable<TimeDuration>(elapsedDuration), animation.mTiming,
        animation.mPlaybackRate);
>>>>>>> upstream-releases

    if (computedTiming.mProgress.IsNull()) {
      continue;
    }

    dom::IterationCompositeOperation iterCompositeOperation =
<<<<<<< HEAD
        static_cast<dom::IterationCompositeOperation>(
            animation.iterationComposite());
||||||| merged common ancestors
        static_cast<dom::IterationCompositeOperation>(
          animation.iterationComposite());
=======
        animation.mIterationComposite;
>>>>>>> upstream-releases

    // Skip calculation if the progress hasn't changed since the last
    // calculation.
    // Note that we don't skip calculate this animation if there is another
    // animation since the other animation might be 'accumulate' or 'add', or
    // might have a missing keyframe (i.e. this animation value will be used in
    // the missing keyframe).
    // FIXME Bug 1455476: We should do this optimizations for the case where
<<<<<<< HEAD
    // the layer has multiple animations.
    if (iEnd == 1 && !dom::KeyframeEffect::HasComputedTimingChanged(
                         computedTiming, iterCompositeOperation,
                         animData.mProgressOnLastCompose,
                         animData.mCurrentIterationOnLastCompose)) {
||||||| merged common ancestors
    // the layer has multiple animations.
    if (iEnd == 1 &&
        !dom::KeyframeEffect::HasComputedTimingChanged(
          computedTiming,
          iterCompositeOperation,
          animData.mProgressOnLastCompose,
          animData.mCurrentIterationOnLastCompose)) {
=======
    // the layer has multiple animations and multiple properties.
    if (aCanSkipCompose == CanSkipCompose::IfPossible &&
        !dom::KeyframeEffect::HasComputedTimingChanged(
            computedTiming, iterCompositeOperation,
            animation.mProgressOnLastCompose,
            animation.mCurrentIterationOnLastCompose)) {
>>>>>>> upstream-releases
#ifdef DEBUG
      shouldBeSkipped = true;
#else
      return AnimationHelper::SampleResult::Skipped;
#endif
    }

    uint32_t segmentIndex = 0;
    size_t segmentSize = animation.mSegments.Length();
    PropertyAnimation::SegmentData* segment = animation.mSegments.Elements();
    while (segment->mEndPortion < computedTiming.mProgress.Value() &&
           segmentIndex < segmentSize - 1) {
      ++segment;
      ++segmentIndex;
    }

    double positionInSegment =
<<<<<<< HEAD
        (computedTiming.mProgress.Value() - segment->startPortion()) /
        (segment->endPortion() - segment->startPortion());
||||||| merged common ancestors
      (computedTiming.mProgress.Value() - segment->startPortion()) /
      (segment->endPortion() - segment->startPortion());
=======
        (computedTiming.mProgress.Value() - segment->mStartPortion) /
        (segment->mEndPortion - segment->mStartPortion);
>>>>>>> upstream-releases

<<<<<<< HEAD
    double portion = ComputedTimingFunction::GetPortion(
        animData.mFunctions[segmentIndex], positionInSegment,
        computedTiming.mBeforeFlag);
||||||| merged common ancestors
    double portion =
      ComputedTimingFunction::GetPortion(animData.mFunctions[segmentIndex],
                                         positionInSegment,
                                     computedTiming.mBeforeFlag);
=======
    double portion = ComputedTimingFunction::GetPortion(
        segment->mFunction, positionInSegment, computedTiming.mBeforeFlag);
>>>>>>> upstream-releases

    // Like above optimization, skip calculation if the target segment isn't
    // changed and if the portion in the segment isn't changed.
    // This optimization is needed for CSS animations/transitions with step
    // timing functions (e.g. the throbber animation on tabs or frame based
    // animations).
    // FIXME Bug 1455476: Like the above optimization, we should apply this
<<<<<<< HEAD
    // optimizations for multiple animation cases as well.
    if (iEnd == 1 && animData.mSegmentIndexOnLastCompose == segmentIndex &&
        !animData.mPortionInSegmentOnLastCompose.IsNull() &&
        animData.mPortionInSegmentOnLastCompose.Value() == portion) {
||||||| merged common ancestors
    // optimizations for multiple animation cases as well.
    if (iEnd == 1 &&
        animData.mSegmentIndexOnLastCompose == segmentIndex &&
        !animData.mPortionInSegmentOnLastCompose.IsNull() &&
        animData.mPortionInSegmentOnLastCompose.Value() == portion) {
=======
    // optimizations for multiple animation cases and multiple properties as
    // well.
    if (aCanSkipCompose == CanSkipCompose::IfPossible &&
        animation.mSegmentIndexOnLastCompose == segmentIndex &&
        !animation.mPortionInSegmentOnLastCompose.IsNull() &&
        animation.mPortionInSegmentOnLastCompose.Value() == portion) {
>>>>>>> upstream-releases
#ifdef DEBUG
      shouldBeSkipped = true;
#else
      return AnimationHelper::SampleResult::Skipped;
#endif
    }

    AnimationPropertySegment animSegment;
    animSegment.mFromKey = 0.0;
    animSegment.mToKey = 1.0;
<<<<<<< HEAD
    animSegment.mFromValue =
        AnimationValue(animData.mStartValues[segmentIndex]);
    animSegment.mToValue = AnimationValue(animData.mEndValues[segmentIndex]);
    animSegment.mFromComposite =
        static_cast<dom::CompositeOperation>(segment->startComposite());
    animSegment.mToComposite =
        static_cast<dom::CompositeOperation>(segment->endComposite());
||||||| merged common ancestors
    animSegment.mFromValue =
      AnimationValue(animData.mStartValues[segmentIndex]);
    animSegment.mToValue =
      AnimationValue(animData.mEndValues[segmentIndex]);
    animSegment.mFromComposite =
      static_cast<dom::CompositeOperation>(segment->startComposite());
    animSegment.mToComposite =
      static_cast<dom::CompositeOperation>(segment->endComposite());
=======
    animSegment.mFromValue = AnimationValue(segment->mStartValue);
    animSegment.mToValue = AnimationValue(segment->mEndValue);
    animSegment.mFromComposite = segment->mStartComposite;
    animSegment.mToComposite = segment->mEndComposite;
>>>>>>> upstream-releases

    // interpolate the property
    aAnimationValue =
<<<<<<< HEAD
        Servo_ComposeAnimationSegment(
            &animSegment, aAnimationValue, animData.mEndValues.LastElement(),
            iterCompositeOperation, portion, computedTiming.mCurrentIteration)
            .Consume();
||||||| merged common ancestors
      Servo_ComposeAnimationSegment(
        &animSegment,
        aAnimationValue,
        animData.mEndValues.LastElement(),
        iterCompositeOperation,
        portion,
        computedTiming.mCurrentIteration).Consume();
=======
        Servo_ComposeAnimationSegment(
            &animSegment, aAnimationValue,
            animation.mSegments.LastElement().mEndValue, iterCompositeOperation,
            portion, computedTiming.mCurrentIteration)
            .Consume();
>>>>>>> upstream-releases

#ifdef DEBUG
    if (shouldBeSkipped) {
      return AnimationHelper::SampleResult::Skipped;
    }
#endif

    hasInEffectAnimations = true;
    animation.mProgressOnLastCompose = computedTiming.mProgress;
    animation.mCurrentIterationOnLastCompose = computedTiming.mCurrentIteration;
    animation.mSegmentIndexOnLastCompose = segmentIndex;
    animation.mPortionInSegmentOnLastCompose.SetValue(portion);
  }

  return hasInEffectAnimations ? AnimationHelper::SampleResult::Sampled
                               : AnimationHelper::SampleResult::None;
}

AnimationHelper::SampleResult AnimationHelper::SampleAnimationForEachNode(
    TimeStamp aPreviousFrameTime, TimeStamp aCurrentFrameTime,
    const AnimatedValue* aPreviousValue,
    nsTArray<PropertyAnimationGroup>& aPropertyAnimationGroups,
    nsTArray<RefPtr<RawServoAnimationValue>>& aAnimationValues /* out */) {
  MOZ_ASSERT(!aPropertyAnimationGroups.IsEmpty(),
             "Should be called with animation data");
  MOZ_ASSERT(aAnimationValues.IsEmpty(),
             "Should be called with empty aAnimationValues");

  nsTArray<RefPtr<RawServoAnimationValue>> nonAnimatingValues;
  for (PropertyAnimationGroup& group : aPropertyAnimationGroups) {
    // Initialize animation value with base style.
    RefPtr<RawServoAnimationValue> currValue = group.mBaseStyle;

    CanSkipCompose canSkipCompose = aPropertyAnimationGroups.Length() == 1 &&
                                            group.mAnimations.Length() == 1
                                        ? CanSkipCompose::IfPossible
                                        : CanSkipCompose::No;

    MOZ_ASSERT(
        !group.mAnimations.IsEmpty() ||
            nsCSSPropertyIDSet::TransformLikeProperties().HasProperty(
                group.mProperty),
        "Only transform-like properties can have empty PropertyAnimation list");

    // For properties which are not animating (i.e. their values are always the
    // same), we store them in a different array, and then merge them into the
    // final result (a.k.a. aAnimationValues) because we shouldn't take them
    // into account for SampleResult. (In other words, these properties
    // shouldn't affect the optimization.)
    if (group.mAnimations.IsEmpty()) {
      nonAnimatingValues.AppendElement(std::move(currValue));
      continue;
    }

    SampleResult result = SampleAnimationForProperty(
        aPreviousFrameTime, aCurrentFrameTime, aPreviousValue, canSkipCompose,
        group.mAnimations, currValue);

    // FIXME: Bug 1455476: Do optimization for multiple properties. For now,
    // the result is skipped only if the property count == 1.
    if (result == SampleResult::Skipped) {
#ifdef DEBUG
      aAnimationValues.AppendElement(std::move(currValue));
#endif
      return SampleResult::Skipped;
    }

    if (result != SampleResult::Sampled) {
      continue;
    }

    // Insert the interpolation result into the output array.
    MOZ_ASSERT(currValue);
    aAnimationValues.AppendElement(std::move(currValue));
  }

#ifdef DEBUG
  // Sanity check that all of animation data are the same.
  const Maybe<TransformData>& lastData =
      aPropertyAnimationGroups.LastElement().mAnimationData;
  for (const PropertyAnimationGroup& group : aPropertyAnimationGroups) {
    const Maybe<TransformData>& data = group.mAnimationData;
    MOZ_ASSERT(data.isSome() == lastData.isSome(),
               "The type of AnimationData should be the same");
    if (data.isNothing()) {
      continue;
    }

    MOZ_ASSERT(data.isSome());
    const TransformData& transformData = data.ref();
    const TransformData& lastTransformData = lastData.ref();
    MOZ_ASSERT(transformData.origin() == lastTransformData.origin() &&
                   transformData.transformOrigin() ==
                       lastTransformData.transformOrigin() &&
                   transformData.bounds() == lastTransformData.bounds() &&
                   transformData.appUnitsPerDevPixel() ==
                       lastTransformData.appUnitsPerDevPixel(),
               "All of members of TransformData should be the same");
  }
#endif

<<<<<<< HEAD
  return hasInEffectAnimations ? SampleResult::Sampled : SampleResult::None;
}

struct BogusAnimation {};

static inline Result<Ok, BogusAnimation> SetCSSAngle(const CSSAngle& aAngle,
                                                     nsCSSValue& aValue) {
  aValue.SetFloatValue(aAngle.value(), nsCSSUnit(aAngle.unit()));
  if (!aValue.IsAngularUnit()) {
    NS_ERROR("Bogus animation from IPC");
    return Err(BogusAnimation{});
||||||| merged common ancestors
  return hasInEffectAnimations ? SampleResult::Sampled : SampleResult::None;
}

struct BogusAnimation {};

static inline Result<Ok, BogusAnimation>
SetCSSAngle(const CSSAngle& aAngle, nsCSSValue& aValue)
{
  aValue.SetFloatValue(aAngle.value(), nsCSSUnit(aAngle.unit()));
  if (!aValue.IsAngularUnit()) {
    NS_ERROR("Bogus animation from IPC");
    return Err(BogusAnimation { });
=======
  SampleResult rv =
      aAnimationValues.IsEmpty() ? SampleResult::None : SampleResult::Sampled;
  if (rv == SampleResult::Sampled) {
    aAnimationValues.AppendElements(nonAnimatingValues);
>>>>>>> upstream-releases
  }
  return rv;
}

<<<<<<< HEAD
static Result<nsCSSValueSharedList*, BogusAnimation> CreateCSSValueList(
    const InfallibleTArray<TransformFunction>& aFunctions) {
  nsAutoPtr<nsCSSValueList> result;
  nsCSSValueList** resultTail = getter_Transfers(result);
  for (uint32_t i = 0; i < aFunctions.Length(); i++) {
    RefPtr<nsCSSValue::Array> arr;
    switch (aFunctions[i].type()) {
      case TransformFunction::TRotationX: {
        const CSSAngle& angle = aFunctions[i].get_RotationX().angle();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_rotatex,
                                                      resultTail);
        MOZ_TRY(SetCSSAngle(angle, arr->Item(1)));
        break;
      }
      case TransformFunction::TRotationY: {
        const CSSAngle& angle = aFunctions[i].get_RotationY().angle();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_rotatey,
                                                      resultTail);
        MOZ_TRY(SetCSSAngle(angle, arr->Item(1)));
        break;
      }
      case TransformFunction::TRotationZ: {
        const CSSAngle& angle = aFunctions[i].get_RotationZ().angle();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_rotatez,
                                                      resultTail);
        MOZ_TRY(SetCSSAngle(angle, arr->Item(1)));
        break;
      }
      case TransformFunction::TRotation: {
        const CSSAngle& angle = aFunctions[i].get_Rotation().angle();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_rotate,
                                                      resultTail);
        MOZ_TRY(SetCSSAngle(angle, arr->Item(1)));
        break;
      }
      case TransformFunction::TRotation3D: {
        float x = aFunctions[i].get_Rotation3D().x();
        float y = aFunctions[i].get_Rotation3D().y();
        float z = aFunctions[i].get_Rotation3D().z();
        const CSSAngle& angle = aFunctions[i].get_Rotation3D().angle();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_rotate3d,
                                                      resultTail);
        arr->Item(1).SetFloatValue(x, eCSSUnit_Number);
        arr->Item(2).SetFloatValue(y, eCSSUnit_Number);
        arr->Item(3).SetFloatValue(z, eCSSUnit_Number);
        MOZ_TRY(SetCSSAngle(angle, arr->Item(4)));
        break;
      }
      case TransformFunction::TScale: {
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_scale3d,
                                                      resultTail);
        arr->Item(1).SetFloatValue(aFunctions[i].get_Scale().x(),
                                   eCSSUnit_Number);
        arr->Item(2).SetFloatValue(aFunctions[i].get_Scale().y(),
                                   eCSSUnit_Number);
        arr->Item(3).SetFloatValue(aFunctions[i].get_Scale().z(),
                                   eCSSUnit_Number);
        break;
      }
      case TransformFunction::TTranslation: {
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_translate3d,
                                                      resultTail);
        arr->Item(1).SetFloatValue(aFunctions[i].get_Translation().x(),
                                   eCSSUnit_Pixel);
        arr->Item(2).SetFloatValue(aFunctions[i].get_Translation().y(),
                                   eCSSUnit_Pixel);
        arr->Item(3).SetFloatValue(aFunctions[i].get_Translation().z(),
                                   eCSSUnit_Pixel);
        break;
      }
      case TransformFunction::TSkewX: {
        const CSSAngle& x = aFunctions[i].get_SkewX().x();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_skewx,
                                                      resultTail);
        MOZ_TRY(SetCSSAngle(x, arr->Item(1)));
        break;
      }
      case TransformFunction::TSkewY: {
        const CSSAngle& y = aFunctions[i].get_SkewY().y();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_skewy,
                                                      resultTail);
        MOZ_TRY(SetCSSAngle(y, arr->Item(1)));
        break;
      }
      case TransformFunction::TSkew: {
        const CSSAngle& x = aFunctions[i].get_Skew().x();
        const CSSAngle& y = aFunctions[i].get_Skew().y();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_skew,
                                                      resultTail);
        MOZ_TRY(SetCSSAngle(x, arr->Item(1)));
        MOZ_TRY(SetCSSAngle(y, arr->Item(2)));
        break;
      }
      case TransformFunction::TTransformMatrix: {
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_matrix3d,
                                                      resultTail);
        const gfx::Matrix4x4& matrix =
            aFunctions[i].get_TransformMatrix().value();
        arr->Item(1).SetFloatValue(matrix._11, eCSSUnit_Number);
        arr->Item(2).SetFloatValue(matrix._12, eCSSUnit_Number);
        arr->Item(3).SetFloatValue(matrix._13, eCSSUnit_Number);
        arr->Item(4).SetFloatValue(matrix._14, eCSSUnit_Number);
        arr->Item(5).SetFloatValue(matrix._21, eCSSUnit_Number);
        arr->Item(6).SetFloatValue(matrix._22, eCSSUnit_Number);
        arr->Item(7).SetFloatValue(matrix._23, eCSSUnit_Number);
        arr->Item(8).SetFloatValue(matrix._24, eCSSUnit_Number);
        arr->Item(9).SetFloatValue(matrix._31, eCSSUnit_Number);
        arr->Item(10).SetFloatValue(matrix._32, eCSSUnit_Number);
        arr->Item(11).SetFloatValue(matrix._33, eCSSUnit_Number);
        arr->Item(12).SetFloatValue(matrix._34, eCSSUnit_Number);
        arr->Item(13).SetFloatValue(matrix._41, eCSSUnit_Number);
        arr->Item(14).SetFloatValue(matrix._42, eCSSUnit_Number);
        arr->Item(15).SetFloatValue(matrix._43, eCSSUnit_Number);
        arr->Item(16).SetFloatValue(matrix._44, eCSSUnit_Number);
        break;
||||||| merged common ancestors
static Result<nsCSSValueSharedList*, BogusAnimation>
CreateCSSValueList(const InfallibleTArray<TransformFunction>& aFunctions)
{
  nsAutoPtr<nsCSSValueList> result;
  nsCSSValueList** resultTail = getter_Transfers(result);
  for (uint32_t i = 0; i < aFunctions.Length(); i++) {
    RefPtr<nsCSSValue::Array> arr;
    switch (aFunctions[i].type()) {
      case TransformFunction::TRotationX:
      {
        const CSSAngle& angle = aFunctions[i].get_RotationX().angle();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_rotatex,
                                                      resultTail);
        MOZ_TRY(SetCSSAngle(angle, arr->Item(1)));
        break;
      }
      case TransformFunction::TRotationY:
      {
        const CSSAngle& angle = aFunctions[i].get_RotationY().angle();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_rotatey,
                                                      resultTail);
        MOZ_TRY(SetCSSAngle(angle, arr->Item(1)));
        break;
      }
      case TransformFunction::TRotationZ:
      {
        const CSSAngle& angle = aFunctions[i].get_RotationZ().angle();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_rotatez,
                                                      resultTail);
        MOZ_TRY(SetCSSAngle(angle, arr->Item(1)));
        break;
      }
      case TransformFunction::TRotation:
      {
        const CSSAngle& angle = aFunctions[i].get_Rotation().angle();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_rotate,
                                                      resultTail);
        MOZ_TRY(SetCSSAngle(angle, arr->Item(1)));
        break;
      }
      case TransformFunction::TRotation3D:
      {
        float x = aFunctions[i].get_Rotation3D().x();
        float y = aFunctions[i].get_Rotation3D().y();
        float z = aFunctions[i].get_Rotation3D().z();
        const CSSAngle& angle = aFunctions[i].get_Rotation3D().angle();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_rotate3d,
                                                      resultTail);
        arr->Item(1).SetFloatValue(x, eCSSUnit_Number);
        arr->Item(2).SetFloatValue(y, eCSSUnit_Number);
        arr->Item(3).SetFloatValue(z, eCSSUnit_Number);
        MOZ_TRY(SetCSSAngle(angle, arr->Item(4)));
        break;
      }
      case TransformFunction::TScale:
      {
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_scale3d,
                                                      resultTail);
        arr->Item(1).SetFloatValue(aFunctions[i].get_Scale().x(), eCSSUnit_Number);
        arr->Item(2).SetFloatValue(aFunctions[i].get_Scale().y(), eCSSUnit_Number);
        arr->Item(3).SetFloatValue(aFunctions[i].get_Scale().z(), eCSSUnit_Number);
        break;
      }
      case TransformFunction::TTranslation:
      {
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_translate3d,
                                                      resultTail);
        arr->Item(1).SetFloatValue(aFunctions[i].get_Translation().x(), eCSSUnit_Pixel);
        arr->Item(2).SetFloatValue(aFunctions[i].get_Translation().y(), eCSSUnit_Pixel);
        arr->Item(3).SetFloatValue(aFunctions[i].get_Translation().z(), eCSSUnit_Pixel);
        break;
      }
      case TransformFunction::TSkewX:
      {
        const CSSAngle& x = aFunctions[i].get_SkewX().x();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_skewx,
                                                      resultTail);
        MOZ_TRY(SetCSSAngle(x, arr->Item(1)));
        break;
      }
      case TransformFunction::TSkewY:
      {
        const CSSAngle& y = aFunctions[i].get_SkewY().y();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_skewy,
                                                      resultTail);
        MOZ_TRY(SetCSSAngle(y, arr->Item(1)));
        break;
      }
      case TransformFunction::TSkew:
      {
        const CSSAngle& x = aFunctions[i].get_Skew().x();
        const CSSAngle& y = aFunctions[i].get_Skew().y();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_skew,
                                                      resultTail);
        MOZ_TRY(SetCSSAngle(x, arr->Item(1)));
        MOZ_TRY(SetCSSAngle(y, arr->Item(2)));
        break;
      }
      case TransformFunction::TTransformMatrix:
      {
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_matrix3d,
                                                      resultTail);
        const gfx::Matrix4x4& matrix = aFunctions[i].get_TransformMatrix().value();
        arr->Item(1).SetFloatValue(matrix._11, eCSSUnit_Number);
        arr->Item(2).SetFloatValue(matrix._12, eCSSUnit_Number);
        arr->Item(3).SetFloatValue(matrix._13, eCSSUnit_Number);
        arr->Item(4).SetFloatValue(matrix._14, eCSSUnit_Number);
        arr->Item(5).SetFloatValue(matrix._21, eCSSUnit_Number);
        arr->Item(6).SetFloatValue(matrix._22, eCSSUnit_Number);
        arr->Item(7).SetFloatValue(matrix._23, eCSSUnit_Number);
        arr->Item(8).SetFloatValue(matrix._24, eCSSUnit_Number);
        arr->Item(9).SetFloatValue(matrix._31, eCSSUnit_Number);
        arr->Item(10).SetFloatValue(matrix._32, eCSSUnit_Number);
        arr->Item(11).SetFloatValue(matrix._33, eCSSUnit_Number);
        arr->Item(12).SetFloatValue(matrix._34, eCSSUnit_Number);
        arr->Item(13).SetFloatValue(matrix._41, eCSSUnit_Number);
        arr->Item(14).SetFloatValue(matrix._42, eCSSUnit_Number);
        arr->Item(15).SetFloatValue(matrix._43, eCSSUnit_Number);
        arr->Item(16).SetFloatValue(matrix._44, eCSSUnit_Number);
        break;
=======
static dom::FillMode GetAdjustedFillMode(const Animation& aAnimation) {
  // Adjust fill mode so that if the main thread is delayed in clearing
  // this animation we don't introduce flicker by jumping back to the old
  // underlying value.
  auto fillMode = static_cast<dom::FillMode>(aAnimation.fillMode());
  float playbackRate = aAnimation.playbackRate();
  switch (fillMode) {
    case dom::FillMode::None:
      if (playbackRate > 0) {
        fillMode = dom::FillMode::Forwards;
      } else if (playbackRate < 0) {
        fillMode = dom::FillMode::Backwards;
>>>>>>> upstream-releases
      }
<<<<<<< HEAD
      case TransformFunction::TPerspective: {
        float perspective = aFunctions[i].get_Perspective().value();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_perspective,
                                                      resultTail);
        arr->Item(1).SetFloatValue(perspective, eCSSUnit_Pixel);
        break;
      }
      default:
        NS_ASSERTION(false, "All functions should be implemented?");
    }
  }
  if (aFunctions.Length() == 0) {
    result = new nsCSSValueList();
    result->mValue.SetNoneValue();
  }
  return new nsCSSValueSharedList(result.forget());
}

static already_AddRefed<RawServoAnimationValue> ToAnimationValue(
    nsCSSPropertyID aProperty, const Animatable& aAnimatable) {
  RefPtr<RawServoAnimationValue> result;

  switch (aAnimatable.type()) {
    case Animatable::Tnull_t:
||||||| merged common ancestors
      case TransformFunction::TPerspective:
      {
        float perspective = aFunctions[i].get_Perspective().value();
        arr = AnimationValue::AppendTransformFunction(eCSSKeyword_perspective,
                                                      resultTail);
        arr->Item(1).SetFloatValue(perspective, eCSSUnit_Pixel);
        break;
      }
      default:
        NS_ASSERTION(false, "All functions should be implemented?");
    }
  }
  if (aFunctions.Length() == 0) {
    result = new nsCSSValueList();
    result->mValue.SetNoneValue();
  }
  return new nsCSSValueSharedList(result.forget());
}

static already_AddRefed<RawServoAnimationValue>
ToAnimationValue(const Animatable& aAnimatable)
{
  RefPtr<RawServoAnimationValue> result;

  switch (aAnimatable.type()) {
    case Animatable::Tnull_t:
=======
>>>>>>> upstream-releases
      break;
<<<<<<< HEAD
    case Animatable::TArrayOfTransformFunction: {
      const InfallibleTArray<TransformFunction>& transforms =
          aAnimatable.get_ArrayOfTransformFunction();
      auto listOrError = CreateCSSValueList(transforms);
      if (listOrError.isOk()) {
        RefPtr<nsCSSValueSharedList> list = listOrError.unwrap();
        MOZ_ASSERT(list, "Transform list should be non null");
        result = Servo_AnimationValue_Transform(*list).Consume();
||||||| merged common ancestors
    case Animatable::TArrayOfTransformFunction: {
      const InfallibleTArray<TransformFunction>& transforms =
        aAnimatable.get_ArrayOfTransformFunction();
      auto listOrError = CreateCSSValueList(transforms);
      if (listOrError.isOk()) {
        RefPtr<nsCSSValueSharedList> list = listOrError.unwrap();
        MOZ_ASSERT(list, "Transform list should be non null");
        result = Servo_AnimationValue_Transform(*list).Consume();
=======
    case dom::FillMode::Backwards:
      if (playbackRate > 0) {
        fillMode = dom::FillMode::Both;
>>>>>>> upstream-releases
      }
      break;
    case dom::FillMode::Forwards:
      if (playbackRate < 0) {
        fillMode = dom::FillMode::Both;
      }
      break;
    case Animatable::Tnscolor:
      result = Servo_AnimationValue_Color(aProperty, aAnimatable.get_nscolor())
                   .Consume();
      break;
    default:
      break;
  }
  return fillMode;
}

<<<<<<< HEAD
void AnimationHelper::SetAnimations(
    AnimationArray& aAnimations, InfallibleTArray<AnimData>& aAnimData,
    RefPtr<RawServoAnimationValue>& aBaseAnimationStyle) {
  for (uint32_t i = 0; i < aAnimations.Length(); i++) {
    Animation& animation = aAnimations[i];
    // Adjust fill mode so that if the main thread is delayed in clearing
    // this animation we don't introduce flicker by jumping back to the old
    // underlying value.
    switch (static_cast<dom::FillMode>(animation.fillMode())) {
      case dom::FillMode::None:
        if (animation.playbackRate() > 0) {
          animation.fillMode() = static_cast<uint8_t>(dom::FillMode::Forwards);
        } else if (animation.playbackRate() < 0) {
          animation.fillMode() = static_cast<uint8_t>(dom::FillMode::Backwards);
        }
        break;
      case dom::FillMode::Backwards:
        if (animation.playbackRate() > 0) {
          animation.fillMode() = static_cast<uint8_t>(dom::FillMode::Both);
        }
        break;
      case dom::FillMode::Forwards:
        if (animation.playbackRate() < 0) {
          animation.fillMode() = static_cast<uint8_t>(dom::FillMode::Both);
        }
        break;
      default:
        break;
||||||| merged common ancestors
void
AnimationHelper::SetAnimations(
  AnimationArray& aAnimations,
  InfallibleTArray<AnimData>& aAnimData,
  RefPtr<RawServoAnimationValue>& aBaseAnimationStyle)
{
  for (uint32_t i = 0; i < aAnimations.Length(); i++) {
    Animation& animation = aAnimations[i];
    // Adjust fill mode so that if the main thread is delayed in clearing
    // this animation we don't introduce flicker by jumping back to the old
    // underlying value.
    switch (static_cast<dom::FillMode>(animation.fillMode())) {
      case dom::FillMode::None:
        if (animation.playbackRate() > 0) {
          animation.fillMode() = static_cast<uint8_t>(dom::FillMode::Forwards);
        } else if (animation.playbackRate() < 0) {
          animation.fillMode() = static_cast<uint8_t>(dom::FillMode::Backwards);
        }
        break;
      case dom::FillMode::Backwards:
        if (animation.playbackRate() > 0) {
          animation.fillMode() = static_cast<uint8_t>(dom::FillMode::Both);
        }
        break;
      case dom::FillMode::Forwards:
        if (animation.playbackRate() < 0) {
          animation.fillMode() = static_cast<uint8_t>(dom::FillMode::Both);
        }
        break;
      default:
        break;
=======
nsTArray<PropertyAnimationGroup> AnimationHelper::ExtractAnimations(
    const AnimationArray& aAnimations) {
  nsTArray<PropertyAnimationGroup> propertyAnimationGroupArray;

  nsCSSPropertyID prevID = eCSSProperty_UNKNOWN;
  PropertyAnimationGroup* currData = nullptr;
  DebugOnly<const layers::Animatable*> currBaseStyle = nullptr;

  for (const Animation& animation : aAnimations) {
    // Animations with same property are grouped together, so we can just
    // check if the current property is the same as the previous one for
    // knowing this is a new group.
    if (prevID != animation.property()) {
      // Got a different group, we should create a different array.
      currData = propertyAnimationGroupArray.AppendElement();
      currData->mProperty = animation.property();
      currData->mAnimationData = animation.data();
      prevID = animation.property();

      // Reset the debug pointer.
      currBaseStyle = nullptr;
>>>>>>> upstream-releases
    }

    MOZ_ASSERT(currData);
    if (animation.baseStyle().type() != Animatable::Tnull_t) {
<<<<<<< HEAD
      aBaseAnimationStyle =
          ToAnimationValue(animation.property(), animation.baseStyle());
||||||| merged common ancestors
      aBaseAnimationStyle = ToAnimationValue(animation.baseStyle());
=======
      MOZ_ASSERT(!currBaseStyle || *currBaseStyle == animation.baseStyle(),
                 "Should be the same base style");

      currData->mBaseStyle = AnimationValue::FromAnimatable(
          animation.property(), animation.baseStyle());
      currBaseStyle = &animation.baseStyle();
    }

    // If this layers::Animation sets isNotAnimating to true, it only has
    // base style and doesn't have any animation information, so we can skip
    // the rest steps. (And so its PropertyAnimationGroup::mAnimation will be
    // an empty array.)
    if (animation.isNotAnimating()) {
      MOZ_ASSERT(nsCSSPropertyIDSet::TransformLikeProperties().HasProperty(
                     animation.property()),
                 "Only transform-like properties could set this true");
      continue;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    AnimData* data = aAnimData.AppendElement();

    data->mTiming =
        TimingParams{animation.duration(),
                     animation.delay(),
                     animation.endDelay(),
                     animation.iterations(),
                     animation.iterationStart(),
                     static_cast<dom::PlaybackDirection>(animation.direction()),
                     static_cast<dom::FillMode>(animation.fillMode()),
                     AnimationUtils::TimingFunctionToComputedTimingFunction(
                         animation.easingFunction())};
    InfallibleTArray<Maybe<ComputedTimingFunction>>& functions =
        data->mFunctions;
    InfallibleTArray<RefPtr<RawServoAnimationValue>>& startValues =
        data->mStartValues;
    InfallibleTArray<RefPtr<RawServoAnimationValue>>& endValues =
        data->mEndValues;

    const InfallibleTArray<AnimationSegment>& segments = animation.segments();
    for (const AnimationSegment& segment : segments) {
      startValues.AppendElement(
          ToAnimationValue(animation.property(), segment.startState()));
      endValues.AppendElement(
          ToAnimationValue(animation.property(), segment.endState()));

      TimingFunction tf = segment.sampleFn();
      Maybe<ComputedTimingFunction> ctf =
          AnimationUtils::TimingFunctionToComputedTimingFunction(tf);
      functions.AppendElement(ctf);
||||||| merged common ancestors
    AnimData* data = aAnimData.AppendElement();

    data->mTiming = TimingParams {
      animation.duration(),
      animation.delay(),
      animation.endDelay(),
      animation.iterations(),
      animation.iterationStart(),
      static_cast<dom::PlaybackDirection>(animation.direction()),
      static_cast<dom::FillMode>(animation.fillMode()),
      AnimationUtils::TimingFunctionToComputedTimingFunction(
           animation.easingFunction())
    };
    InfallibleTArray<Maybe<ComputedTimingFunction>>& functions =
      data->mFunctions;
    InfallibleTArray<RefPtr<RawServoAnimationValue>>& startValues =
      data->mStartValues;
    InfallibleTArray<RefPtr<RawServoAnimationValue>>& endValues =
      data->mEndValues;

    const InfallibleTArray<AnimationSegment>& segments = animation.segments();
    for (const AnimationSegment& segment : segments) {
      startValues.AppendElement(ToAnimationValue(segment.startState()));
      endValues.AppendElement(ToAnimationValue(segment.endState()));

      TimingFunction tf = segment.sampleFn();
      Maybe<ComputedTimingFunction> ctf =
        AnimationUtils::TimingFunctionToComputedTimingFunction(tf);
      functions.AppendElement(ctf);
=======
    PropertyAnimation* propertyAnimation =
        currData->mAnimations.AppendElement();

    propertyAnimation->mOriginTime = animation.originTime();
    propertyAnimation->mStartTime = animation.startTime();
    propertyAnimation->mHoldTime = animation.holdTime();
    propertyAnimation->mPlaybackRate = animation.playbackRate();
    propertyAnimation->mIterationComposite =
        static_cast<dom::IterationCompositeOperation>(
            animation.iterationComposite());
    propertyAnimation->mIsNotPlaying = animation.isNotPlaying();
    propertyAnimation->mTiming =
        TimingParams{animation.duration(),
                     animation.delay(),
                     animation.endDelay(),
                     animation.iterations(),
                     animation.iterationStart(),
                     static_cast<dom::PlaybackDirection>(animation.direction()),
                     GetAdjustedFillMode(animation),
                     AnimationUtils::TimingFunctionToComputedTimingFunction(
                         animation.easingFunction())};

    nsTArray<PropertyAnimation::SegmentData>& segmentData =
        propertyAnimation->mSegments;
    for (const AnimationSegment& segment : animation.segments()) {
      segmentData.AppendElement(PropertyAnimation::SegmentData{
          AnimationValue::FromAnimatable(animation.property(),
                                         segment.startState()),
          AnimationValue::FromAnimatable(animation.property(),
                                         segment.endState()),
          AnimationUtils::TimingFunctionToComputedTimingFunction(
              segment.sampleFn()),
          segment.startPortion(), segment.endPortion(),
          static_cast<dom::CompositeOperation>(segment.startComposite()),
          static_cast<dom::CompositeOperation>(segment.endComposite())});
    }
  }

#ifdef DEBUG
  // Sanity check that the grouped animation data is correct by looking at the
  // property set.
  if (!propertyAnimationGroupArray.IsEmpty()) {
    nsCSSPropertyIDSet seenProperties;
    for (const auto& group : propertyAnimationGroupArray) {
      nsCSSPropertyID id = group.mProperty;

      MOZ_ASSERT(!seenProperties.HasProperty(id), "Should be a new property");
      seenProperties.AddProperty(id);
>>>>>>> upstream-releases
    }

    MOZ_ASSERT(
        seenProperties.IsSubsetOf(LayerAnimationInfo::GetCSSPropertiesFor(
            DisplayItemType::TYPE_TRANSFORM)) ||
            seenProperties.IsSubsetOf(LayerAnimationInfo::GetCSSPropertiesFor(
                DisplayItemType::TYPE_OPACITY)) ||
            seenProperties.IsSubsetOf(LayerAnimationInfo::GetCSSPropertiesFor(
                DisplayItemType::TYPE_BACKGROUND_COLOR)),
        "The property set of output should be the subset of transform-like "
        "properties, opacity, or background_color.");
  }
#endif

  return propertyAnimationGroupArray;
}

uint64_t AnimationHelper::GetNextCompositorAnimationsId() {
  static uint32_t sNextId = 0;
  ++sNextId;

  uint32_t procId = static_cast<uint32_t>(base::GetCurrentProcId());
  uint64_t nextId = procId;
  nextId = nextId << 32 | sNextId;
  return nextId;
}

bool AnimationHelper::SampleAnimations(CompositorAnimationStorage* aStorage,
                                       TimeStamp aPreviousFrameTime,
                                       TimeStamp aCurrentFrameTime) {
  MOZ_ASSERT(aStorage);
  bool isAnimating = false;

  // Do nothing if there are no compositor animations
  if (!aStorage->AnimationsCount()) {
    return isAnimating;
  }

<<<<<<< HEAD
  // Sample the animations in CompositorAnimationStorage
  for (auto iter = aStorage->ConstAnimationsTableIter(); !iter.Done();
       iter.Next()) {
    AnimationArray* animations = iter.UserData();
    if (animations->IsEmpty()) {
||||||| merged common ancestors
  //Sample the animations in CompositorAnimationStorage
  for (auto iter = aStorage->ConstAnimationsTableIter();
       !iter.Done(); iter.Next()) {
    AnimationArray* animations = iter.UserData();
    if (animations->IsEmpty()) {
=======
  // Sample the animations in CompositorAnimationStorage
  for (auto iter = aStorage->ConstAnimationsTableIter(); !iter.Done();
       iter.Next()) {
    auto& propertyAnimationGroups = *iter.UserData();
    if (propertyAnimationGroups.IsEmpty()) {
>>>>>>> upstream-releases
      continue;
    }

    isAnimating = true;
<<<<<<< HEAD
    RefPtr<RawServoAnimationValue> animationValue;
    InfallibleTArray<AnimData> animationData;
    AnimationHelper::SetAnimations(*animations, animationData, animationValue);
||||||| merged common ancestors
    RefPtr<RawServoAnimationValue> animationValue;
    InfallibleTArray<AnimData> animationData;
    AnimationHelper::SetAnimations(*animations,
                                   animationData,
                                   animationValue);
=======
    nsTArray<RefPtr<RawServoAnimationValue>> animationValues;
>>>>>>> upstream-releases
    AnimatedValue* previousValue = aStorage->GetAnimatedValue(iter.Key());
    AnimationHelper::SampleResult sampleResult =
<<<<<<< HEAD
        AnimationHelper::SampleAnimationForEachNode(
            aPreviousFrameTime, aCurrentFrameTime, *animations, animationData,
            animationValue, previousValue);
||||||| merged common ancestors
      AnimationHelper::SampleAnimationForEachNode(aPreviousFrameTime,
                                                  aCurrentFrameTime,
                                                  *animations,
                                                  animationData,
                                                  animationValue,
                                                  previousValue);
=======
        AnimationHelper::SampleAnimationForEachNode(
            aPreviousFrameTime, aCurrentFrameTime, previousValue,
            propertyAnimationGroups, animationValues);
>>>>>>> upstream-releases

    if (sampleResult != AnimationHelper::SampleResult::Sampled) {
      continue;
    }

    const PropertyAnimationGroup& lastPropertyAnimationGroup =
        propertyAnimationGroups.LastElement();

    // Store the AnimatedValue
    switch (lastPropertyAnimationGroup.mProperty) {
      case eCSSProperty_opacity: {
        MOZ_ASSERT(animationValues.Length() == 1);
        aStorage->SetAnimatedValue(
<<<<<<< HEAD
            iter.Key(), Servo_AnimationValue_GetOpacity(animationValue));
||||||| merged common ancestors
          iter.Key(),
          Servo_AnimationValue_GetOpacity(animationValue));
=======
            iter.Key(), Servo_AnimationValue_GetOpacity(animationValues[0]));
>>>>>>> upstream-releases
        break;
      }
      case eCSSProperty_rotate:
      case eCSSProperty_scale:
      case eCSSProperty_translate:
      case eCSSProperty_transform: {
<<<<<<< HEAD
        RefPtr<nsCSSValueSharedList> list;
        Servo_AnimationValue_GetTransform(animationValue, &list);
        const TransformData& transformData =
            animation.data().get_TransformData();
        nsPoint origin = transformData.origin();
        // we expect all our transform data to arrive in device pixels
        gfx::Point3D transformOrigin = transformData.transformOrigin();
        nsDisplayTransform::FrameTransformProperties props(std::move(list),
                                                           transformOrigin);
||||||| merged common ancestors
        RefPtr<nsCSSValueSharedList> list;
        Servo_AnimationValue_GetTransform(animationValue, &list);
        const TransformData& transformData = animation.data().get_TransformData();
        nsPoint origin = transformData.origin();
        // we expect all our transform data to arrive in device pixels
        gfx::Point3D transformOrigin = transformData.transformOrigin();
        nsDisplayTransform::FrameTransformProperties props(std::move(list),
                                                           transformOrigin);
=======
        const TransformData& transformData =
            lastPropertyAnimationGroup.mAnimationData.ref();
>>>>>>> upstream-releases

        gfx::Matrix4x4 transform =
<<<<<<< HEAD
            nsDisplayTransform::GetResultingTransformMatrix(
                props, origin, transformData.appUnitsPerDevPixel(), 0,
                &transformData.bounds());
||||||| merged common ancestors
          nsDisplayTransform::GetResultingTransformMatrix(props, origin,
                                                          transformData.appUnitsPerDevPixel(),
                                                          0, &transformData.bounds());
=======
            ServoAnimationValueToMatrix4x4(animationValues, transformData);
>>>>>>> upstream-releases
        gfx::Matrix4x4 frameTransform = transform;
        // If the parent has perspective transform, then the offset into
        // reference frame coordinates is already on this transform. If not,
        // then we need to ask for it to be added here.
        if (!transformData.hasPerspectiveParent()) {
<<<<<<< HEAD
          nsLayoutUtils::PostTranslate(
              transform, origin, transformData.appUnitsPerDevPixel(), true);
||||||| merged common ancestors
           nsLayoutUtils::PostTranslate(transform, origin,
                                        transformData.appUnitsPerDevPixel(),
                                        true);
=======
          nsLayoutUtils::PostTranslate(transform, transformData.origin(),
                                       transformData.appUnitsPerDevPixel(),
                                       true);
>>>>>>> upstream-releases
        }

        transform.PostScale(transformData.inheritedXScale(),
                            transformData.inheritedYScale(), 1);

        aStorage->SetAnimatedValue(iter.Key(), std::move(transform),
                                   std::move(frameTransform), transformData);
        break;
      }
      default:
        MOZ_ASSERT_UNREACHABLE("Unhandled animated property");
    }
  }

  return isAnimating;
}

<<<<<<< HEAD
}  // namespace layers
}  // namespace mozilla
||||||| merged common ancestors
} // namespace layers
} // namespace mozilla
=======
gfx::Matrix4x4 AnimationHelper::ServoAnimationValueToMatrix4x4(
    const nsTArray<RefPtr<RawServoAnimationValue>>& aValues,
    const TransformData& aTransformData) {
  // This is a bit silly just to avoid the transform list copy from the
  // animation transform list.
  auto noneTranslate = StyleTranslate::None();
  auto noneRotate = StyleRotate::None();
  auto noneScale = StyleScale::None();
  const StyleTransform noneTransform;

  const StyleTranslate* translate = nullptr;
  const StyleRotate* rotate = nullptr;
  const StyleScale* scale = nullptr;
  const StyleTransform* transform = nullptr;

  // TODO: Bug 1429305: Support compositor animations for motion-path.
  for (const auto& value : aValues) {
    MOZ_ASSERT(value);
    nsCSSPropertyID id = Servo_AnimationValue_GetPropertyId(value);
    switch (id) {
      case eCSSProperty_transform:
        MOZ_ASSERT(!transform);
        transform = Servo_AnimationValue_GetTransform(value);
        break;
      case eCSSProperty_translate:
        MOZ_ASSERT(!translate);
        translate = Servo_AnimationValue_GetTranslate(value);
        break;
      case eCSSProperty_rotate:
        MOZ_ASSERT(!rotate);
        rotate = Servo_AnimationValue_GetRotate(value);
        break;
      case eCSSProperty_scale:
        MOZ_ASSERT(!scale);
        scale = Servo_AnimationValue_GetScale(value);
        break;
      default:
        MOZ_ASSERT_UNREACHABLE("Unsupported transform-like property");
    }
  }
  // We expect all our transform data to arrive in device pixels
  gfx::Point3D transformOrigin = aTransformData.transformOrigin();
  nsDisplayTransform::FrameTransformProperties props(
      translate ? *translate : noneTranslate, rotate ? *rotate : noneRotate,
      scale ? *scale : noneScale, transform ? *transform : noneTransform,
      transformOrigin);

  return nsDisplayTransform::GetResultingTransformMatrix(
      props, aTransformData.origin(), aTransformData.appUnitsPerDevPixel(), 0,
      &aTransformData.bounds());
}

}  // namespace layers
}  // namespace mozilla
>>>>>>> upstream-releases
