/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_ScrollStyles_h
#define mozilla_ScrollStyles_h

#include <stdint.h>
<<<<<<< HEAD
#include "nsStyleConsts.h"
#include "nsStyleCoord.h"   // for nsStyleCoord
||||||| merged common ancestors
#include "nsStyleConsts.h" // for NS_STYLE_SCROLL_SNAP_*
#include "nsStyleCoord.h" // for nsStyleCoord
=======
#include "nsStyleConsts.h"
>>>>>>> upstream-releases
#include "mozilla/dom/WindowBinding.h"

// Forward declarations
struct nsStyleDisplay;

namespace mozilla {

<<<<<<< HEAD
struct ScrollStyles {
  // Always one of NS_STYLE_OVERFLOW_SCROLL, NS_STYLE_OVERFLOW_HIDDEN,
  // or NS_STYLE_OVERFLOW_AUTO.
  uint8_t mHorizontal;
  uint8_t mVertical;
||||||| merged common ancestors
struct ScrollStyles
{
  // Always one of NS_STYLE_OVERFLOW_SCROLL, NS_STYLE_OVERFLOW_HIDDEN,
  // or NS_STYLE_OVERFLOW_AUTO.
  uint8_t mHorizontal;
  uint8_t mVertical;
=======
struct ScrollStyles {
  // Always one of Scroll, Hidden, or Auto
  StyleOverflow mHorizontal;
  StyleOverflow mVertical;
>>>>>>> upstream-releases
  // Always one of NS_STYLE_SCROLL_BEHAVIOR_AUTO or
  // NS_STYLE_SCROLL_BEHAVIOR_SMOOTH
<<<<<<< HEAD
  uint8_t mScrollBehavior;
  mozilla::StyleOverscrollBehavior mOverscrollBehaviorX;
  mozilla::StyleOverscrollBehavior mOverscrollBehaviorY;
  mozilla::StyleScrollSnapType mScrollSnapTypeX;
  mozilla::StyleScrollSnapType mScrollSnapTypeY;
  nsStyleCoord mScrollSnapPointsX;
  nsStyleCoord mScrollSnapPointsY;
  nsStyleCoord::CalcValue mScrollSnapDestinationX;
  nsStyleCoord::CalcValue mScrollSnapDestinationY;

  ScrollStyles(uint8_t aH, uint8_t aV)
      : mHorizontal(aH),
        mVertical(aV),
        mScrollBehavior(NS_STYLE_SCROLL_BEHAVIOR_AUTO),
        mOverscrollBehaviorX(StyleOverscrollBehavior::Auto),
        mOverscrollBehaviorY(StyleOverscrollBehavior::Auto),
        mScrollSnapTypeX(mozilla::StyleScrollSnapType::None),
        mScrollSnapTypeY(mozilla::StyleScrollSnapType::None),
        mScrollSnapPointsX(nsStyleCoord(eStyleUnit_None)),
        mScrollSnapPointsY(nsStyleCoord(eStyleUnit_None)) {
    mScrollSnapDestinationX.mPercent = 0;
    mScrollSnapDestinationX.mLength = nscoord(0.0f);
    mScrollSnapDestinationX.mHasPercent = false;
    mScrollSnapDestinationY.mPercent = 0;
    mScrollSnapDestinationY.mLength = nscoord(0.0f);
    mScrollSnapDestinationY.mHasPercent = false;
  }

  explicit ScrollStyles(const nsStyleDisplay* aDisplay);
  ScrollStyles(uint8_t aH, uint8_t aV, const nsStyleDisplay* aDisplay);
||||||| merged common ancestors
  uint8_t mScrollBehavior;
  mozilla::StyleOverscrollBehavior mOverscrollBehaviorX;
  mozilla::StyleOverscrollBehavior mOverscrollBehaviorY;
  // Always one of NS_STYLE_SCROLL_SNAP_NONE, NS_STYLE_SCROLL_SNAP_MANDATORY,
  // or NS_STYLE_SCROLL_SNAP_PROXIMITY.
  uint8_t mScrollSnapTypeX;
  uint8_t mScrollSnapTypeY;
  nsStyleCoord mScrollSnapPointsX;
  nsStyleCoord mScrollSnapPointsY;
  nsStyleCoord::CalcValue mScrollSnapDestinationX;
  nsStyleCoord::CalcValue mScrollSnapDestinationY;

  ScrollStyles(uint8_t aH, uint8_t aV)
    : mHorizontal(aH), mVertical(aV),
      mScrollBehavior(NS_STYLE_SCROLL_BEHAVIOR_AUTO),
      mOverscrollBehaviorX(StyleOverscrollBehavior::Auto),
      mOverscrollBehaviorY(StyleOverscrollBehavior::Auto),
      mScrollSnapTypeX(NS_STYLE_SCROLL_SNAP_TYPE_NONE),
      mScrollSnapTypeY(NS_STYLE_SCROLL_SNAP_TYPE_NONE),
      mScrollSnapPointsX(nsStyleCoord(eStyleUnit_None)),
      mScrollSnapPointsY(nsStyleCoord(eStyleUnit_None)) {

    mScrollSnapDestinationX.mPercent = 0;
    mScrollSnapDestinationX.mLength = nscoord(0.0f);
    mScrollSnapDestinationX.mHasPercent = false;
    mScrollSnapDestinationY.mPercent = 0;
    mScrollSnapDestinationY.mLength = nscoord(0.0f);
    mScrollSnapDestinationY.mHasPercent = false;
  }

  explicit ScrollStyles(const nsStyleDisplay* aDisplay);
  ScrollStyles(uint8_t aH, uint8_t aV, const nsStyleDisplay* aDisplay);
=======
  StyleOverscrollBehavior mOverscrollBehaviorX;
  StyleOverscrollBehavior mOverscrollBehaviorY;
  StyleScrollSnapStrictness mScrollSnapStrictnessX;
  StyleScrollSnapStrictness mScrollSnapStrictnessY;

  ScrollStyles(StyleOverflow aH, StyleOverflow aV)
      : mHorizontal(aH),
        mVertical(aV),
        mOverscrollBehaviorX(StyleOverscrollBehavior::Auto),
        mOverscrollBehaviorY(StyleOverscrollBehavior::Auto),
        mScrollSnapStrictnessX(StyleScrollSnapStrictness::None),
        mScrollSnapStrictnessY(StyleScrollSnapStrictness::None) {}

  ScrollStyles(WritingMode aWritingMode, const nsStyleDisplay* aDisplay);
  ScrollStyles(WritingMode aWritingMode, StyleOverflow aH, StyleOverflow aV,
               const nsStyleDisplay* aDisplay);
  void InitializeScrollSnapStrictness(WritingMode aWritingMode,
                                      const nsStyleDisplay* aDisplay);
>>>>>>> upstream-releases
  bool operator==(const ScrollStyles& aStyles) const {
<<<<<<< HEAD
    return aStyles.mHorizontal == mHorizontal &&
           aStyles.mVertical == mVertical &&
           aStyles.mScrollBehavior == mScrollBehavior &&
||||||| merged common ancestors
    return aStyles.mHorizontal == mHorizontal && aStyles.mVertical == mVertical &&
           aStyles.mScrollBehavior == mScrollBehavior &&
=======
    return aStyles.mHorizontal == mHorizontal &&
           aStyles.mVertical == mVertical &&
>>>>>>> upstream-releases
           aStyles.mOverscrollBehaviorX == mOverscrollBehaviorX &&
           aStyles.mOverscrollBehaviorY == mOverscrollBehaviorY &&
           aStyles.mScrollSnapStrictnessX == mScrollSnapStrictnessX &&
           aStyles.mScrollSnapStrictnessY == mScrollSnapStrictnessY;
  }
  bool operator!=(const ScrollStyles& aStyles) const {
    return !(*this == aStyles);
  }
  bool IsHiddenInBothDirections() const {
<<<<<<< HEAD
    return mHorizontal == NS_STYLE_OVERFLOW_HIDDEN &&
           mVertical == NS_STYLE_OVERFLOW_HIDDEN;
  }
  bool IsSmoothScroll(dom::ScrollBehavior aBehavior) const {
    return aBehavior == dom::ScrollBehavior::Smooth ||
           (aBehavior == dom::ScrollBehavior::Auto &&
            mScrollBehavior == NS_STYLE_SCROLL_BEHAVIOR_SMOOTH);
||||||| merged common ancestors
    return mHorizontal == NS_STYLE_OVERFLOW_HIDDEN &&
           mVertical == NS_STYLE_OVERFLOW_HIDDEN;
  }
  bool IsSmoothScroll(dom::ScrollBehavior aBehavior) const {
    return aBehavior == dom::ScrollBehavior::Smooth ||
             (aBehavior == dom::ScrollBehavior::Auto &&
               mScrollBehavior == NS_STYLE_SCROLL_BEHAVIOR_SMOOTH);
=======
    return mHorizontal == StyleOverflow::Hidden &&
           mVertical == StyleOverflow::Hidden;
>>>>>>> upstream-releases
  }
};

}  // namespace mozilla

#endif  // mozilla_ScrollStyles_h
