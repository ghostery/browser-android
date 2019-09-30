/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/ScrollStyles.h"
<<<<<<< HEAD
#include "nsStyleStruct.h"  // for nsStyleDisplay and nsStyleBackground::Position
||||||| merged common ancestors
#include "nsStyleStruct.h" // for nsStyleDisplay and nsStyleBackground::Position
=======
#include "mozilla/WritingModes.h"
#include "nsStyleStruct.h"  // for nsStyleDisplay & nsStyleBackground::Position
>>>>>>> upstream-releases

namespace mozilla {

<<<<<<< HEAD
ScrollStyles::ScrollStyles(uint8_t aH, uint8_t aV,
                           const nsStyleDisplay* aDisplay)
    : mHorizontal(aH),
      mVertical(aV),
      mScrollBehavior(aDisplay->mScrollBehavior),
||||||| merged common ancestors
  ScrollStyles::ScrollStyles(uint8_t aH, uint8_t aV,
                             const nsStyleDisplay* aDisplay)
    : mHorizontal(aH), mVertical(aV),
      mScrollBehavior(aDisplay->mScrollBehavior),
=======
void ScrollStyles::InitializeScrollSnapStrictness(
    WritingMode aWritingMode, const nsStyleDisplay* aDisplay) {
  mScrollSnapStrictnessX = StyleScrollSnapStrictness::None;
  mScrollSnapStrictnessY = StyleScrollSnapStrictness::None;

  if (aDisplay->mScrollSnapType.strictness == StyleScrollSnapStrictness::None) {
    return;
  }

  switch (aDisplay->mScrollSnapType.axis) {
    case StyleScrollSnapAxis::X:
      mScrollSnapStrictnessX = aDisplay->mScrollSnapType.strictness;
      break;
    case StyleScrollSnapAxis::Y:
      mScrollSnapStrictnessY = aDisplay->mScrollSnapType.strictness;
      break;
    case StyleScrollSnapAxis::Block:
      if (aWritingMode.IsVertical()) {
        mScrollSnapStrictnessX = aDisplay->mScrollSnapType.strictness;
      } else {
        mScrollSnapStrictnessY = aDisplay->mScrollSnapType.strictness;
      }
      break;
    case StyleScrollSnapAxis::Inline:
      if (aWritingMode.IsVertical()) {
        mScrollSnapStrictnessY = aDisplay->mScrollSnapType.strictness;
      } else {
        mScrollSnapStrictnessX = aDisplay->mScrollSnapType.strictness;
      }
      break;
    case StyleScrollSnapAxis::Both:
      mScrollSnapStrictnessX = aDisplay->mScrollSnapType.strictness;
      mScrollSnapStrictnessY = aDisplay->mScrollSnapType.strictness;
      break;
  }
}

ScrollStyles::ScrollStyles(WritingMode aWritingMode, StyleOverflow aH,
                           StyleOverflow aV, const nsStyleDisplay* aDisplay)
    : mHorizontal(aH),
      mVertical(aV),
>>>>>>> upstream-releases
      mOverscrollBehaviorX(aDisplay->mOverscrollBehaviorX),
<<<<<<< HEAD
      mOverscrollBehaviorY(aDisplay->mOverscrollBehaviorY),
      mScrollSnapTypeX(aDisplay->mScrollSnapTypeX),
      mScrollSnapTypeY(aDisplay->mScrollSnapTypeY),
      mScrollSnapPointsX(aDisplay->mScrollSnapPointsX),
      mScrollSnapPointsY(aDisplay->mScrollSnapPointsY),
      mScrollSnapDestinationX(aDisplay->mScrollSnapDestination.mXPosition),
      mScrollSnapDestinationY(aDisplay->mScrollSnapDestination.mYPosition) {}

ScrollStyles::ScrollStyles(const nsStyleDisplay* aDisplay)
    : mHorizontal(aDisplay->mOverflowX),
      mVertical(aDisplay->mOverflowY),
      mScrollBehavior(aDisplay->mScrollBehavior),
||||||| merged common ancestors
      mOverscrollBehaviorY(aDisplay->mOverscrollBehaviorY),
      mScrollSnapTypeX(aDisplay->mScrollSnapTypeX),
      mScrollSnapTypeY(aDisplay->mScrollSnapTypeY),
      mScrollSnapPointsX(aDisplay->mScrollSnapPointsX),
      mScrollSnapPointsY(aDisplay->mScrollSnapPointsY),
      mScrollSnapDestinationX(aDisplay->mScrollSnapDestination.mXPosition),
      mScrollSnapDestinationY(aDisplay->mScrollSnapDestination.mYPosition) {}

  ScrollStyles::ScrollStyles(const nsStyleDisplay* aDisplay)
    : mHorizontal(aDisplay->mOverflowX), mVertical(aDisplay->mOverflowY),
      mScrollBehavior(aDisplay->mScrollBehavior),
=======
      mOverscrollBehaviorY(aDisplay->mOverscrollBehaviorY) {
  InitializeScrollSnapStrictness(aWritingMode, aDisplay);
}

ScrollStyles::ScrollStyles(WritingMode aWritingMode,
                           const nsStyleDisplay* aDisplay)
    : mHorizontal(aDisplay->mOverflowX),
      mVertical(aDisplay->mOverflowY),
>>>>>>> upstream-releases
      mOverscrollBehaviorX(aDisplay->mOverscrollBehaviorX),
<<<<<<< HEAD
      mOverscrollBehaviorY(aDisplay->mOverscrollBehaviorY),
      mScrollSnapTypeX(aDisplay->mScrollSnapTypeX),
      mScrollSnapTypeY(aDisplay->mScrollSnapTypeY),
      mScrollSnapPointsX(aDisplay->mScrollSnapPointsX),
      mScrollSnapPointsY(aDisplay->mScrollSnapPointsY),
      mScrollSnapDestinationX(aDisplay->mScrollSnapDestination.mXPosition),
      mScrollSnapDestinationY(aDisplay->mScrollSnapDestination.mYPosition) {}

}  // namespace mozilla
||||||| merged common ancestors
      mOverscrollBehaviorY(aDisplay->mOverscrollBehaviorY),
      mScrollSnapTypeX(aDisplay->mScrollSnapTypeX),
      mScrollSnapTypeY(aDisplay->mScrollSnapTypeY),
      mScrollSnapPointsX(aDisplay->mScrollSnapPointsX),
      mScrollSnapPointsY(aDisplay->mScrollSnapPointsY),
      mScrollSnapDestinationX(aDisplay->mScrollSnapDestination.mXPosition),
      mScrollSnapDestinationY(aDisplay->mScrollSnapDestination.mYPosition) {}

} // namespace mozilla
=======
      mOverscrollBehaviorY(aDisplay->mOverscrollBehaviorY) {
  InitializeScrollSnapStrictness(aWritingMode, aDisplay);
}

}  // namespace mozilla
>>>>>>> upstream-releases
