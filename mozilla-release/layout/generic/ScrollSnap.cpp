/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ScrollSnap.h"

#include "FrameMetrics.h"

#include "mozilla/Maybe.h"
#include "mozilla/Preferences.h"
#include "nsLineLayout.h"

namespace mozilla {

using layers::ScrollSnapInfo;

/**
<<<<<<< HEAD
 * Stores candidate snapping edges.
 */
class SnappingEdgeCallback {
 public:
  virtual void AddHorizontalEdge(nscoord aEdge) = 0;
  virtual void AddVerticalEdge(nscoord aEdge) = 0;
  virtual void AddHorizontalEdgeInterval(const nsRect& aScrollRange,
                                         nscoord aInterval,
                                         nscoord aOffset) = 0;
  virtual void AddVerticalEdgeInterval(const nsRect& aScrollRange,
                                       nscoord aInterval, nscoord aOffset) = 0;
};

/**
||||||| merged common ancestors
 * Stores candidate snapping edges.
 */
class SnappingEdgeCallback {
public:
  virtual void AddHorizontalEdge(nscoord aEdge) = 0;
  virtual void AddVerticalEdge(nscoord aEdge) = 0;
  virtual void AddHorizontalEdgeInterval(const nsRect &aScrollRange,
                                         nscoord aInterval,
                                         nscoord aOffset) = 0;
  virtual void AddVerticalEdgeInterval(const nsRect &aScrollRange,
                                       nscoord aInterval,
                                       nscoord aOffset) = 0;
};

/**
=======
>>>>>>> upstream-releases
 * Keeps track of the current best edge to snap to. The criteria for
 * adding an edge depends on the scrolling unit.
 */
<<<<<<< HEAD
class CalcSnapPoints : public SnappingEdgeCallback {
 public:
||||||| merged common ancestors
class CalcSnapPoints : public SnappingEdgeCallback {
public:
=======
class CalcSnapPoints final {
 public:
>>>>>>> upstream-releases
  CalcSnapPoints(nsIScrollableFrame::ScrollUnit aUnit,
<<<<<<< HEAD
                 const nsPoint& aDestination, const nsPoint& aStartPos);
  virtual void AddHorizontalEdge(nscoord aEdge) override;
  virtual void AddVerticalEdge(nscoord aEdge) override;
  virtual void AddHorizontalEdgeInterval(const nsRect& aScrollRange,
                                         nscoord aInterval,
                                         nscoord aOffset) override;
  virtual void AddVerticalEdgeInterval(const nsRect& aScrollRange,
                                       nscoord aInterval,
                                       nscoord aOffset) override;
  void AddEdge(nscoord aEdge, nscoord aDestination, nscoord aStartPos,
               nscoord aScrollingDirection, nscoord* aBestEdge,
               bool* aEdgeFound);
  void AddEdgeInterval(nscoord aInterval, nscoord aMinPos, nscoord aMaxPos,
                       nscoord aOffset, nscoord aDestination, nscoord aStartPos,
                       nscoord aScrollingDirection, nscoord* aBestEdge,
                       bool* aEdgeFound);
||||||| merged common ancestors
                 const nsPoint& aDestination,
                 const nsPoint& aStartPos);
  virtual void AddHorizontalEdge(nscoord aEdge) override;
  virtual void AddVerticalEdge(nscoord aEdge) override;
  virtual void AddHorizontalEdgeInterval(const nsRect &aScrollRange,
                                         nscoord aInterval, nscoord aOffset)
                                         override;
  virtual void AddVerticalEdgeInterval(const nsRect &aScrollRange,
                                       nscoord aInterval, nscoord aOffset)
                                       override;
  void AddEdge(nscoord aEdge,
               nscoord aDestination,
               nscoord aStartPos,
               nscoord aScrollingDirection,
               nscoord* aBestEdge,
               bool* aEdgeFound);
  void AddEdgeInterval(nscoord aInterval,
                       nscoord aMinPos,
                       nscoord aMaxPos,
                       nscoord aOffset,
                       nscoord aDestination,
                       nscoord aStartPos,
                       nscoord aScrollingDirection,
                       nscoord* aBestEdge,
                       bool* aEdgeFound);
=======
                 const nsPoint& aDestination, const nsPoint& aStartPos);
  void AddHorizontalEdge(nscoord aEdge);
  void AddVerticalEdge(nscoord aEdge);
  void AddHorizontalEdgeInterval(const nsRect& aScrollRange, nscoord aInterval,
                                 nscoord aOffset);
  void AddVerticalEdgeInterval(const nsRect& aScrollRange, nscoord aInterval,
                               nscoord aOffset);
  void AddEdge(nscoord aEdge, nscoord aDestination, nscoord aStartPos,
               nscoord aScrollingDirection, nscoord* aBestEdge,
               nscoord* aSecondBestEdge, bool* aEdgeFound);
  void AddEdgeInterval(nscoord aInterval, nscoord aMinPos, nscoord aMaxPos,
                       nscoord aOffset, nscoord aDestination, nscoord aStartPos,
                       nscoord aScrollingDirection, nscoord* aBestEdge,
                       nscoord* aSecondBestEdge, bool* aEdgeFound);
>>>>>>> upstream-releases
  nsPoint GetBestEdge() const;
<<<<<<< HEAD

 protected:
||||||| merged common ancestors
protected:
=======
  nscoord XDistanceBetweenBestAndSecondEdge() const {
    return std::abs(mBestEdge.x - mSecondBestEdge.x);
  }
  nscoord YDistanceBetweenBestAndSecondEdge() const {
    return std::abs(mBestEdge.y - mSecondBestEdge.y);
  }

 protected:
>>>>>>> upstream-releases
  nsIScrollableFrame::ScrollUnit mUnit;
  nsPoint mDestination;  // gives the position after scrolling but before
                         // snapping
  nsPoint mStartPos;     // gives the position before scrolling
  nsIntPoint mScrollingDirection;  // always -1, 0, or 1
<<<<<<< HEAD
  nsPoint mBestEdge;  // keeps track of the position of the current best edge
  bool mHorizontalEdgeFound;  // true if mBestEdge.x is storing a valid
                              // horizontal edge
  bool mVerticalEdgeFound;    // true if mBestEdge.y is storing a valid vertical
                              // edge
||||||| merged common ancestors
  nsPoint mBestEdge;               // keeps track of the position of the current best edge
  bool mHorizontalEdgeFound;       // true if mBestEdge.x is storing a valid horizontal edge
  bool mVerticalEdgeFound;         // true if mBestEdge.y is storing a valid vertical edge
=======
  nsPoint mBestEdge;  // keeps track of the position of the current best edge
  nsPoint mSecondBestEdge;    // keeps track of the position of the current
                              // second best edge
  bool mHorizontalEdgeFound;  // true if mBestEdge.x is storing a valid
                              // horizontal edge
  bool mVerticalEdgeFound;    // true if mBestEdge.y is storing a valid vertical
                              // edge
>>>>>>> upstream-releases
};

CalcSnapPoints::CalcSnapPoints(nsIScrollableFrame::ScrollUnit aUnit,
                               const nsPoint& aDestination,
                               const nsPoint& aStartPos) {
  mUnit = aUnit;
  mDestination = aDestination;
  mStartPos = aStartPos;

  nsPoint direction = aDestination - aStartPos;
  mScrollingDirection = nsIntPoint(0, 0);
  if (direction.x < 0) {
    mScrollingDirection.x = -1;
  }
  if (direction.x > 0) {
    mScrollingDirection.x = 1;
  }
  if (direction.y < 0) {
    mScrollingDirection.y = -1;
  }
  if (direction.y > 0) {
    mScrollingDirection.y = 1;
  }
  mBestEdge = aDestination;
  mSecondBestEdge = nsPoint(nscoord_MAX, nscoord_MAX);
  mHorizontalEdgeFound = false;
  mVerticalEdgeFound = false;
}

nsPoint CalcSnapPoints::GetBestEdge() const {
  return nsPoint(mVerticalEdgeFound ? mBestEdge.x : mStartPos.x,
                 mHorizontalEdgeFound ? mBestEdge.y : mStartPos.y);
}

<<<<<<< HEAD
void CalcSnapPoints::AddHorizontalEdge(nscoord aEdge) {
  AddEdge(aEdge, mDestination.y, mStartPos.y, mScrollingDirection.y,
          &mBestEdge.y, &mHorizontalEdgeFound);
||||||| merged common ancestors
void
CalcSnapPoints::AddHorizontalEdge(nscoord aEdge)
{
  AddEdge(aEdge, mDestination.y, mStartPos.y, mScrollingDirection.y, &mBestEdge.y,
          &mHorizontalEdgeFound);
=======
void CalcSnapPoints::AddHorizontalEdge(nscoord aEdge) {
  AddEdge(aEdge, mDestination.y, mStartPos.y, mScrollingDirection.y,
          &mBestEdge.y, &mSecondBestEdge.y, &mHorizontalEdgeFound);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void CalcSnapPoints::AddVerticalEdge(nscoord aEdge) {
  AddEdge(aEdge, mDestination.x, mStartPos.x, mScrollingDirection.x,
          &mBestEdge.x, &mVerticalEdgeFound);
||||||| merged common ancestors
void
CalcSnapPoints::AddVerticalEdge(nscoord aEdge)
{
  AddEdge(aEdge, mDestination.x, mStartPos.x, mScrollingDirection.x, &mBestEdge.x,
          &mVerticalEdgeFound);
=======
void CalcSnapPoints::AddVerticalEdge(nscoord aEdge) {
  AddEdge(aEdge, mDestination.x, mStartPos.x, mScrollingDirection.x,
          &mBestEdge.x, &mSecondBestEdge.x, &mVerticalEdgeFound);
>>>>>>> upstream-releases
}

void CalcSnapPoints::AddHorizontalEdgeInterval(const nsRect& aScrollRange,
                                               nscoord aInterval,
                                               nscoord aOffset) {
  AddEdgeInterval(aInterval, aScrollRange.y, aScrollRange.YMost(), aOffset,
                  mDestination.y, mStartPos.y, mScrollingDirection.y,
                  &mBestEdge.y, &mSecondBestEdge.y, &mHorizontalEdgeFound);
}

void CalcSnapPoints::AddVerticalEdgeInterval(const nsRect& aScrollRange,
                                             nscoord aInterval,
                                             nscoord aOffset) {
  AddEdgeInterval(aInterval, aScrollRange.x, aScrollRange.XMost(), aOffset,
                  mDestination.x, mStartPos.x, mScrollingDirection.x,
                  &mBestEdge.x, &mSecondBestEdge.x, &mVerticalEdgeFound);
}

<<<<<<< HEAD
void CalcSnapPoints::AddEdge(nscoord aEdge, nscoord aDestination,
                             nscoord aStartPos, nscoord aScrollingDirection,
                             nscoord* aBestEdge, bool* aEdgeFound) {
||||||| merged common ancestors
void
CalcSnapPoints::AddEdge(nscoord aEdge, nscoord aDestination, nscoord aStartPos,
                        nscoord aScrollingDirection, nscoord* aBestEdge,
                        bool *aEdgeFound)
{
=======
void CalcSnapPoints::AddEdge(nscoord aEdge, nscoord aDestination,
                             nscoord aStartPos, nscoord aScrollingDirection,
                             nscoord* aBestEdge, nscoord* aSecondBestEdge,
                             bool* aEdgeFound) {
>>>>>>> upstream-releases
  // nsIScrollableFrame::DEVICE_PIXELS indicates that we are releasing a drag
  // gesture or any other user input event that sets an absolute scroll
  // position.  In this case, scroll snapping is expected to travel in any
  // direction.  Otherwise, we will restrict the direction of the scroll
  // snapping movement based on aScrollingDirection.
  if (mUnit != nsIScrollableFrame::DEVICE_PIXELS) {
    // Unless DEVICE_PIXELS, we only want to snap to points ahead of the
    // direction we are scrolling
    if (aScrollingDirection == 0) {
      // The scroll direction is neutral - will not hit a snap point.
      return;
    }
    // nsIScrollableFrame::WHOLE indicates that we are navigating to "home" or
    // "end".  In this case, we will always select the first or last snap point
    // regardless of the direction of the scroll.  Otherwise, we will select
    // scroll snapping points only in the direction specified by
    // aScrollingDirection.
    if (mUnit != nsIScrollableFrame::WHOLE) {
      // Direction of the edge from the current position (before scrolling) in
      // the direction of scrolling
      nscoord direction = (aEdge - aStartPos) * aScrollingDirection;
      if (direction <= 0) {
        // The edge is not in the direction we are scrolling, skip it.
        return;
      }
    }
  }
  if (!*aEdgeFound) {
    *aBestEdge = aEdge;
    *aEdgeFound = true;
    return;
  }

  // A utility function to update the best and the second best edges in the
  // given conditions.
  // |aIsCloserThanBest| True if the current candidate is closer than the best
  // edge.
  // |aIsCloserThanSecond| True if the current candidate is closer than
  // the second best edge.
  auto updateBestEdges = [&](bool aIsCloserThanBest, bool aIsCloserThanSecond) {
    if (aIsCloserThanBest) {
      *aSecondBestEdge = *aBestEdge;
      *aBestEdge = aEdge;
    } else if (aIsCloserThanSecond) {
      *aSecondBestEdge = aEdge;
    }
  };

  if (mUnit == nsIScrollableFrame::DEVICE_PIXELS ||
      mUnit == nsIScrollableFrame::LINES) {
    nscoord distance = std::abs(aEdge - aDestination);
    updateBestEdges(distance < std::abs(*aBestEdge - aDestination),
                    distance < std::abs(*aSecondBestEdge - aDestination));
  } else if (mUnit == nsIScrollableFrame::PAGES) {
    // distance to the edge from the scrolling destination in the direction of
    // scrolling
    nscoord overshoot = (aEdge - aDestination) * aScrollingDirection;
    // distance to the current best edge from the scrolling destination in the
    // direction of scrolling
    nscoord curOvershoot = (*aBestEdge - aDestination) * aScrollingDirection;

<<<<<<< HEAD
    // edges between the current position and the scrolling destination are
    // favoured to preserve context
    if (overshoot < 0 && (overshoot > curOvershoot || curOvershoot >= 0)) {
      *aBestEdge = aEdge;
||||||| merged common ancestors
    // edges between the current position and the scrolling destination are favoured
    // to preserve context
    if (overshoot < 0 && (overshoot > curOvershoot || curOvershoot >= 0)) {
      *aBestEdge = aEdge;
=======
    nscoord secondOvershoot =
        (*aSecondBestEdge - aDestination) * aScrollingDirection;

    // edges between the current position and the scrolling destination are
    // favoured to preserve context
    if (overshoot < 0) {
      updateBestEdges(overshoot > curOvershoot || curOvershoot >= 0,
                      overshoot > secondOvershoot || secondOvershoot >= 0);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    // if there are no edges between the current position and the scrolling
    // destination the closest edge beyond the destination is used
    if (overshoot > 0 && overshoot < curOvershoot) {
      *aBestEdge = aEdge;
||||||| merged common ancestors
    // if there are no edges between the current position and the scrolling destination
    // the closest edge beyond the destination is used
    if (overshoot > 0 && overshoot < curOvershoot) {
      *aBestEdge = aEdge;
=======
    // if there are no edges between the current position and the scrolling
    // destination the closest edge beyond the destination is used
    if (overshoot > 0) {
      updateBestEdges(overshoot < curOvershoot, overshoot < secondOvershoot);
>>>>>>> upstream-releases
    }
  } else if (mUnit == nsIScrollableFrame::WHOLE) {
<<<<<<< HEAD
    // the edge closest to the top/bottom/left/right is used, depending on
    // scrolling direction
    if (aScrollingDirection > 0 && aEdge > *aBestEdge) {
      *aBestEdge = aEdge;
    } else if (aScrollingDirection < 0 && aEdge < *aBestEdge) {
      *aBestEdge = aEdge;
||||||| merged common ancestors
    // the edge closest to the top/bottom/left/right is used, depending on scrolling direction
    if (aScrollingDirection > 0 && aEdge > *aBestEdge) {
      *aBestEdge = aEdge;
    } else if (aScrollingDirection < 0 && aEdge < *aBestEdge) {
      *aBestEdge = aEdge;
=======
    // the edge closest to the top/bottom/left/right is used, depending on
    // scrolling direction
    if (aScrollingDirection > 0) {
      updateBestEdges(aEdge > *aBestEdge, aEdge > *aSecondBestEdge);
    } else if (aScrollingDirection < 0) {
      updateBestEdges(aEdge < *aBestEdge, aEdge < *aSecondBestEdge);
>>>>>>> upstream-releases
    }
  } else {
    NS_ERROR("Invalid scroll mode");
    return;
  }
}

<<<<<<< HEAD
void CalcSnapPoints::AddEdgeInterval(nscoord aInterval, nscoord aMinPos,
                                     nscoord aMaxPos, nscoord aOffset,
                                     nscoord aDestination, nscoord aStartPos,
                                     nscoord aScrollingDirection,
                                     nscoord* aBestEdge, bool* aEdgeFound) {
||||||| merged common ancestors
void
CalcSnapPoints::AddEdgeInterval(nscoord aInterval, nscoord aMinPos,
                                nscoord aMaxPos, nscoord aOffset,
                                nscoord aDestination, nscoord aStartPos,
                                nscoord aScrollingDirection,
                                nscoord* aBestEdge, bool *aEdgeFound)
{
=======
void CalcSnapPoints::AddEdgeInterval(
    nscoord aInterval, nscoord aMinPos, nscoord aMaxPos, nscoord aOffset,
    nscoord aDestination, nscoord aStartPos, nscoord aScrollingDirection,
    nscoord* aBestEdge, nscoord* aSecondBestEdge, bool* aEdgeFound) {
>>>>>>> upstream-releases
  if (aInterval == 0) {
    // When interval is 0, there are no scroll snap points.
    // Avoid division by zero and bail.
    return;
  }

  // The only possible candidate interval snap points are the edges immediately
  // surrounding aDestination.

  // aDestination must be clamped to the scroll
  // range in order to handle cases where the best matching snap point would
  // result in scrolling out of bounds.  This clamping must be prior to
  // selecting the two interval edges.
  nscoord clamped = std::max(std::min(aDestination, aMaxPos), aMinPos);

  // Add each edge in the interval immediately before aTarget and after aTarget
  // Do not add edges that are out of range.
  nscoord r = (clamped + aOffset) % aInterval;
  if (r < aMinPos) {
    r += aInterval;
  }
  nscoord edge = clamped - r;
  if (edge >= aMinPos && edge <= aMaxPos) {
    AddEdge(edge, aDestination, aStartPos, aScrollingDirection, aBestEdge,
            aSecondBestEdge, aEdgeFound);
  }
  edge += aInterval;
  if (edge >= aMinPos && edge <= aMaxPos) {
    AddEdge(edge, aDestination, aStartPos, aScrollingDirection, aBestEdge,
            aSecondBestEdge, aEdgeFound);
  }
}

static void ProcessSnapPositions(CalcSnapPoints& aCalcSnapPoints,
                                 const ScrollSnapInfo& aSnapInfo) {
  for (auto position : aSnapInfo.mSnapPositionX) {
    aCalcSnapPoints.AddVerticalEdge(position);
  }
  for (auto position : aSnapInfo.mSnapPositionY) {
    aCalcSnapPoints.AddHorizontalEdge(position);
  }
}

<<<<<<< HEAD
static void ProcessScrollSnapCoordinates(
    SnappingEdgeCallback& aCallback,
    const nsTArray<nsPoint>& aScrollSnapCoordinates,
    const nsPoint& aScrollSnapDestination) {
||||||| merged common ancestors
static void
ProcessScrollSnapCoordinates(SnappingEdgeCallback& aCallback,
                             const nsTArray<nsPoint>& aScrollSnapCoordinates,
                             const nsPoint& aScrollSnapDestination) {
=======
static void ProcessScrollSnapCoordinates(
    CalcSnapPoints& aCalcSnapPoint,
    const nsTArray<nsPoint>& aScrollSnapCoordinates,
    const nsPoint& aScrollSnapDestination) {
>>>>>>> upstream-releases
  for (nsPoint snapCoords : aScrollSnapCoordinates) {
    // Make them relative to the scroll snap destination.
    snapCoords -= aScrollSnapDestination;

    aCalcSnapPoint.AddVerticalEdge(snapCoords.x);
    aCalcSnapPoint.AddHorizontalEdge(snapCoords.y);
  }
}

Maybe<nsPoint> ScrollSnapUtils::GetSnapPointForDestination(
<<<<<<< HEAD
    const ScrollSnapInfo& aSnapInfo, nsIScrollableFrame::ScrollUnit aUnit,
    const nsSize& aScrollPortSize, const nsRect& aScrollRange,
    const nsPoint& aStartPos, const nsPoint& aDestination) {
  if (aSnapInfo.mScrollSnapTypeY == StyleScrollSnapType::None &&
      aSnapInfo.mScrollSnapTypeX == StyleScrollSnapType::None) {
||||||| merged common ancestors
    const ScrollSnapInfo& aSnapInfo,
    nsIScrollableFrame::ScrollUnit aUnit,
    const nsSize& aScrollPortSize,
    const nsRect& aScrollRange,
    const nsPoint& aStartPos,
    const nsPoint& aDestination)
{
  if (aSnapInfo.mScrollSnapTypeY == NS_STYLE_SCROLL_SNAP_TYPE_NONE &&
      aSnapInfo.mScrollSnapTypeX == NS_STYLE_SCROLL_SNAP_TYPE_NONE) {
=======
    const ScrollSnapInfo& aSnapInfo, nsIScrollableFrame::ScrollUnit aUnit,
    const nsRect& aScrollRange, const nsPoint& aStartPos,
    const nsPoint& aDestination) {
  if (aSnapInfo.mScrollSnapStrictnessY == StyleScrollSnapStrictness::None &&
      aSnapInfo.mScrollSnapStrictnessX == StyleScrollSnapStrictness::None) {
>>>>>>> upstream-releases
    return Nothing();
  }

  if (StaticPrefs::layout_css_scroll_snap_v1_enabled() &&
      !aSnapInfo.HasSnapPositions()) {
    return Nothing();
  }

  CalcSnapPoints calcSnapPoints(aUnit, aDestination, aStartPos);

  if (StaticPrefs::layout_css_scroll_snap_v1_enabled()) {
    ProcessSnapPositions(calcSnapPoints, aSnapInfo);

    // If the distance between the first and the second candidate snap points
    // is larger than the snapport size and the snapport is covered by larger
    // elements, any points inside the covering area should be valid snap
    // points.
    // https://drafts.csswg.org/css-scroll-snap-1/#snap-overflow
    // NOTE: |aDestination| sometimes points outside of the scroll range, e.g.
    // by the APZC fling, so for the overflow checks we need to clamp it.
    nsPoint clampedDestination = aScrollRange.ClampPoint(aDestination);
    for (auto range : aSnapInfo.mXRangeWiderThanSnapport) {
      if (range.IsValid(clampedDestination.x, aSnapInfo.mSnapportSize.width) &&
          calcSnapPoints.XDistanceBetweenBestAndSecondEdge() >
              aSnapInfo.mSnapportSize.width) {
        calcSnapPoints.AddVerticalEdge(clampedDestination.x);
        break;
      }
    }
    for (auto range : aSnapInfo.mYRangeWiderThanSnapport) {
      if (range.IsValid(clampedDestination.y, aSnapInfo.mSnapportSize.height) &&
          calcSnapPoints.YDistanceBetweenBestAndSecondEdge() >
              aSnapInfo.mSnapportSize.height) {
        calcSnapPoints.AddHorizontalEdge(clampedDestination.y);
        break;
      }
    }
  } else {
    nsPoint destPos = aSnapInfo.mScrollSnapDestination;

    if (aSnapInfo.mScrollSnapIntervalX.isSome()) {
      nscoord interval = aSnapInfo.mScrollSnapIntervalX.value();
      calcSnapPoints.AddVerticalEdgeInterval(aScrollRange, interval, destPos.x);
    }
    if (aSnapInfo.mScrollSnapIntervalY.isSome()) {
      nscoord interval = aSnapInfo.mScrollSnapIntervalY.value();
      calcSnapPoints.AddHorizontalEdgeInterval(aScrollRange, interval,
                                               destPos.y);
    }

    ProcessScrollSnapCoordinates(calcSnapPoints,
                                 aSnapInfo.mScrollSnapCoordinates, destPos);
  }

<<<<<<< HEAD
  ProcessScrollSnapCoordinates(calcSnapPoints, aSnapInfo.mScrollSnapCoordinates,
                               destPos);
||||||| merged common ancestors
  ProcessScrollSnapCoordinates(calcSnapPoints, aSnapInfo.mScrollSnapCoordinates, destPos);
=======
>>>>>>> upstream-releases
  bool snapped = false;
  nsPoint finalPos = calcSnapPoints.GetBestEdge();
  nscoord proximityThreshold =
      StaticPrefs::layout_css_scroll_snap_proximity_threshold();
  proximityThreshold = nsPresContext::CSSPixelsToAppUnits(proximityThreshold);
<<<<<<< HEAD
  if (aSnapInfo.mScrollSnapTypeY == StyleScrollSnapType::Proximity &&
||||||| merged common ancestors
  if (aSnapInfo.mScrollSnapTypeY == NS_STYLE_SCROLL_SNAP_TYPE_PROXIMITY &&
=======
  if (aSnapInfo.mScrollSnapStrictnessY ==
          StyleScrollSnapStrictness::Proximity &&
>>>>>>> upstream-releases
      std::abs(aDestination.y - finalPos.y) > proximityThreshold) {
    finalPos.y = aDestination.y;
  } else {
    snapped = true;
  }
<<<<<<< HEAD
  if (aSnapInfo.mScrollSnapTypeX == StyleScrollSnapType::Proximity &&
||||||| merged common ancestors
  if (aSnapInfo.mScrollSnapTypeX == NS_STYLE_SCROLL_SNAP_TYPE_PROXIMITY &&
=======
  if (aSnapInfo.mScrollSnapStrictnessX ==
          StyleScrollSnapStrictness::Proximity &&
>>>>>>> upstream-releases
      std::abs(aDestination.x - finalPos.x) > proximityThreshold) {
    finalPos.x = aDestination.x;
  } else {
    snapped = true;
  }
  return snapped ? Some(finalPos) : Nothing();
}

}  // namespace mozilla
