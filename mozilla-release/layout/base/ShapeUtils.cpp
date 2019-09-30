/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/ShapeUtils.h"

#include <cstdlib>

#include "nsCSSRendering.h"
#include "nsMargin.h"
#include "nsStyleStruct.h"
#include "SVGContentUtils.h"

namespace mozilla {

<<<<<<< HEAD
nscoord ShapeUtils::ComputeShapeRadius(const StyleShapeRadius aType,
                                       const nscoord aCenter,
                                       const nscoord aPosMin,
                                       const nscoord aPosMax) {
||||||| merged common ancestors
nscoord
ShapeUtils::ComputeShapeRadius(const StyleShapeRadius aType,
                               const nscoord aCenter,
                               const nscoord aPosMin,
                               const nscoord aPosMax)
{
=======
nscoord ShapeUtils::ComputeShapeRadius(const StyleShapeRadius& aType,
                                       const nscoord aCenter,
                                       const nscoord aPosMin,
                                       const nscoord aPosMax) {
  MOZ_ASSERT(aType.IsFarthestSide() || aType.IsClosestSide());
>>>>>>> upstream-releases
  nscoord dist1 = std::abs(aPosMin - aCenter);
  nscoord dist2 = std::abs(aPosMax - aCenter);
  nscoord length = 0;
  if (aType.IsFarthestSide()) {
    length = dist1 > dist2 ? dist1 : dist2;
  } else {
    length = dist1 > dist2 ? dist2 : dist1;
  }
  return length;
}

<<<<<<< HEAD
nsPoint ShapeUtils::ComputeCircleOrEllipseCenter(
    const StyleBasicShape& aBasicShape, const nsRect& aRefBox) {
  MOZ_ASSERT(aBasicShape.GetShapeType() == StyleBasicShapeType::Circle ||
                 aBasicShape.GetShapeType() == StyleBasicShapeType::Ellipse,
||||||| merged common ancestors
nsPoint
ShapeUtils::ComputeCircleOrEllipseCenter(const UniquePtr<StyleBasicShape>& aBasicShape,
                                         const nsRect& aRefBox)
{
  MOZ_ASSERT(aBasicShape->GetShapeType() == StyleBasicShapeType::Circle ||
             aBasicShape->GetShapeType() == StyleBasicShapeType::Ellipse,
=======
nsPoint ShapeUtils::ComputeCircleOrEllipseCenter(
    const StyleBasicShape& aBasicShape, const nsRect& aRefBox) {
  MOZ_ASSERT(aBasicShape.IsCircle() || aBasicShape.IsEllipse(),
>>>>>>> upstream-releases
             "The basic shape must be circle() or ellipse!");

  const auto& position = aBasicShape.IsCircle()
                             ? aBasicShape.AsCircle().position
                             : aBasicShape.AsEllipse().position;

  nsPoint topLeft, anchor;
  nsSize size(aRefBox.Size());
<<<<<<< HEAD
  nsImageRenderer::ComputeObjectAnchorPoint(aBasicShape.GetPosition(), size,
                                            size, &topLeft, &anchor);
||||||| merged common ancestors
  nsImageRenderer::ComputeObjectAnchorPoint(aBasicShape->GetPosition(),
                                            size, size,
                                            &topLeft, &anchor);
=======
  nsImageRenderer::ComputeObjectAnchorPoint(position, size, size, &topLeft,
                                            &anchor);
>>>>>>> upstream-releases
  return anchor + aRefBox.TopLeft();
}

<<<<<<< HEAD
nscoord ShapeUtils::ComputeCircleRadius(const StyleBasicShape& aBasicShape,
                                        const nsPoint& aCenter,
                                        const nsRect& aRefBox) {
  MOZ_ASSERT(aBasicShape.GetShapeType() == StyleBasicShapeType::Circle,
             "The basic shape must be circle()!");

  const nsTArray<nsStyleCoord>& coords = aBasicShape.Coordinates();
  MOZ_ASSERT(coords.Length() == 1, "wrong number of arguments");
  nscoord r = 0;
  if (coords[0].GetUnit() == eStyleUnit_Enumerated) {
    const auto styleShapeRadius = coords[0].GetEnumValue<StyleShapeRadius>();
    nscoord horizontal = ComputeShapeRadius(styleShapeRadius, aCenter.x,
                                            aRefBox.x, aRefBox.XMost());
    nscoord vertical = ComputeShapeRadius(styleShapeRadius, aCenter.y,
                                          aRefBox.y, aRefBox.YMost());
    r = styleShapeRadius == StyleShapeRadius::FarthestSide
            ? std::max(horizontal, vertical)
            : std::min(horizontal, vertical);
  } else {
    // We resolve percent <shape-radius> value for circle() as defined here:
    // https://drafts.csswg.org/css-shapes/#funcdef-circle
    double referenceLength = SVGContentUtils::ComputeNormalizedHypotenuse(
        aRefBox.width, aRefBox.height);
    r = coords[0].ComputeCoordPercentCalc(NSToCoordRound(referenceLength));
||||||| merged common ancestors
nscoord
ShapeUtils::ComputeCircleRadius(const UniquePtr<StyleBasicShape>& aBasicShape,
                                const nsPoint& aCenter,
                                const nsRect& aRefBox)
{
  MOZ_ASSERT(aBasicShape->GetShapeType() == StyleBasicShapeType::Circle,
             "The basic shape must be circle()!");

  const nsTArray<nsStyleCoord>& coords = aBasicShape->Coordinates();
  MOZ_ASSERT(coords.Length() == 1, "wrong number of arguments");
  nscoord r = 0;
  if (coords[0].GetUnit() == eStyleUnit_Enumerated) {
    const auto styleShapeRadius = coords[0].GetEnumValue<StyleShapeRadius>();
    nscoord horizontal =
      ComputeShapeRadius(styleShapeRadius, aCenter.x, aRefBox.x, aRefBox.XMost());
    nscoord vertical =
      ComputeShapeRadius(styleShapeRadius, aCenter.y, aRefBox.y, aRefBox.YMost());
    r = styleShapeRadius == StyleShapeRadius::FarthestSide
          ? std::max(horizontal, vertical)
          : std::min(horizontal, vertical);
  } else {
    // We resolve percent <shape-radius> value for circle() as defined here:
    // https://drafts.csswg.org/css-shapes/#funcdef-circle
    double referenceLength =
      SVGContentUtils::ComputeNormalizedHypotenuse(aRefBox.width,
                                                   aRefBox.height);
    r = coords[0].ComputeCoordPercentCalc(NSToCoordRound(referenceLength));
=======
nscoord ShapeUtils::ComputeCircleRadius(const StyleBasicShape& aBasicShape,
                                        const nsPoint& aCenter,
                                        const nsRect& aRefBox) {
  MOZ_ASSERT(aBasicShape.IsCircle(), "The basic shape must be circle()!");
  const auto& radius = aBasicShape.AsCircle().radius;
  if (radius.IsLength()) {
    return radius.AsLength().Resolve([&] {
      // We resolve percent <shape-radius> value for circle() as defined here:
      // https://drafts.csswg.org/css-shapes/#funcdef-circle
      double referenceLength = SVGContentUtils::ComputeNormalizedHypotenuse(
          aRefBox.width, aRefBox.height);
      return NSToCoordRound(referenceLength);
    });
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
nsSize ShapeUtils::ComputeEllipseRadii(const StyleBasicShape& aBasicShape,
                                       const nsPoint& aCenter,
                                       const nsRect& aRefBox) {
  MOZ_ASSERT(aBasicShape.GetShapeType() == StyleBasicShapeType::Ellipse,
             "The basic shape must be ellipse()!");
||||||| merged common ancestors
nsSize
ShapeUtils::ComputeEllipseRadii(const UniquePtr<StyleBasicShape>& aBasicShape,
                                const nsPoint& aCenter,
                                const nsRect& aRefBox)
{
  MOZ_ASSERT(aBasicShape->GetShapeType() == StyleBasicShapeType::Ellipse,
             "The basic shape must be ellipse()!");
=======
  nscoord horizontal =
      ComputeShapeRadius(radius, aCenter.x, aRefBox.x, aRefBox.XMost());
  nscoord vertical =
      ComputeShapeRadius(radius, aCenter.y, aRefBox.y, aRefBox.YMost());
  return radius.IsFarthestSide() ? std::max(horizontal, vertical)
                                 : std::min(horizontal, vertical);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  const nsTArray<nsStyleCoord>& coords = aBasicShape.Coordinates();
  MOZ_ASSERT(coords.Length() == 2, "wrong number of arguments");
||||||| merged common ancestors
  const nsTArray<nsStyleCoord>& coords = aBasicShape->Coordinates();
  MOZ_ASSERT(coords.Length() == 2, "wrong number of arguments");
=======
nsSize ShapeUtils::ComputeEllipseRadii(const StyleBasicShape& aBasicShape,
                                       const nsPoint& aCenter,
                                       const nsRect& aRefBox) {
  MOZ_ASSERT(aBasicShape.IsEllipse(), "The basic shape must be ellipse()!");
  const auto& ellipse = aBasicShape.AsEllipse();
>>>>>>> upstream-releases
  nsSize radii;
<<<<<<< HEAD

  if (coords[0].GetUnit() == eStyleUnit_Enumerated) {
    const StyleShapeRadius radiusX = coords[0].GetEnumValue<StyleShapeRadius>();
    radii.width =
        ComputeShapeRadius(radiusX, aCenter.x, aRefBox.x, aRefBox.XMost());
||||||| merged common ancestors

  if (coords[0].GetUnit() == eStyleUnit_Enumerated) {
    const StyleShapeRadius radiusX = coords[0].GetEnumValue<StyleShapeRadius>();
    radii.width = ComputeShapeRadius(radiusX, aCenter.x, aRefBox.x,
                                     aRefBox.XMost());
=======
  if (ellipse.semiaxis_x.IsLength()) {
    radii.width = ellipse.semiaxis_x.AsLength().Resolve(aRefBox.width);
>>>>>>> upstream-releases
  } else {
    radii.width = ComputeShapeRadius(ellipse.semiaxis_x, aCenter.x, aRefBox.x,
                                     aRefBox.XMost());
  }

<<<<<<< HEAD
  if (coords[1].GetUnit() == eStyleUnit_Enumerated) {
    const StyleShapeRadius radiusY = coords[1].GetEnumValue<StyleShapeRadius>();
    radii.height =
        ComputeShapeRadius(radiusY, aCenter.y, aRefBox.y, aRefBox.YMost());
||||||| merged common ancestors
  if (coords[1].GetUnit() == eStyleUnit_Enumerated) {
    const StyleShapeRadius radiusY = coords[1].GetEnumValue<StyleShapeRadius>();
    radii.height = ComputeShapeRadius(radiusY, aCenter.y, aRefBox.y,
                                      aRefBox.YMost());
=======
  if (ellipse.semiaxis_y.IsLength()) {
    radii.height = ellipse.semiaxis_y.AsLength().Resolve(aRefBox.height);
>>>>>>> upstream-releases
  } else {
    radii.height = ComputeShapeRadius(ellipse.semiaxis_y, aCenter.y, aRefBox.y,
                                      aRefBox.YMost());
  }

  return radii;
}

<<<<<<< HEAD
/* static */ nsRect ShapeUtils::ComputeInsetRect(
    const StyleBasicShape& aBasicShape, const nsRect& aRefBox) {
  MOZ_ASSERT(aBasicShape.GetShapeType() == StyleBasicShapeType::Inset,
             "The basic shape must be inset()!");

  const nsTArray<nsStyleCoord>& coords = aBasicShape.Coordinates();
  MOZ_ASSERT(coords.Length() == 4, "wrong number of arguments");

  nsMargin inset(coords[0].ComputeCoordPercentCalc(aRefBox.Height()),
                 coords[1].ComputeCoordPercentCalc(aRefBox.Width()),
                 coords[2].ComputeCoordPercentCalc(aRefBox.Height()),
                 coords[3].ComputeCoordPercentCalc(aRefBox.Width()));
||||||| merged common ancestors
/* static */ nsRect
ShapeUtils::ComputeInsetRect(const UniquePtr<StyleBasicShape>& aBasicShape,
                             const nsRect& aRefBox)
{
  MOZ_ASSERT(aBasicShape->GetShapeType() == StyleBasicShapeType::Inset,
             "The basic shape must be inset()!");

  const nsTArray<nsStyleCoord>& coords = aBasicShape->Coordinates();
  MOZ_ASSERT(coords.Length() == 4, "wrong number of arguments");

  nsMargin inset(coords[0].ComputeCoordPercentCalc(aRefBox.Height()),
                 coords[1].ComputeCoordPercentCalc(aRefBox.Width()),
                 coords[2].ComputeCoordPercentCalc(aRefBox.Height()),
                 coords[3].ComputeCoordPercentCalc(aRefBox.Width()));
=======
/* static */
nsRect ShapeUtils::ComputeInsetRect(const StyleBasicShape& aBasicShape,
                                    const nsRect& aRefBox) {
  MOZ_ASSERT(aBasicShape.IsInset(), "The basic shape must be inset()!");
  const auto& rect = aBasicShape.AsInset().rect;
  nsMargin inset(
      rect._0.Resolve(aRefBox.Height()), rect._1.Resolve(aRefBox.Width()),
      rect._2.Resolve(aRefBox.Height()), rect._3.Resolve(aRefBox.Width()));
>>>>>>> upstream-releases

  nscoord x = aRefBox.X() + inset.left;
  nscoord width = aRefBox.Width() - inset.LeftRight();
  nscoord y = aRefBox.Y() + inset.top;
  nscoord height = aRefBox.Height() - inset.TopBottom();

  // Invert left and right, if necessary.
  if (width < 0) {
    width *= -1;
    x -= width;
  }

  // Invert top and bottom, if necessary.
  if (height < 0) {
    height *= -1;
    y -= height;
  }

  return nsRect(x, y, width, height);
}

<<<<<<< HEAD
/* static */ bool ShapeUtils::ComputeInsetRadii(
    const StyleBasicShape& aBasicShape, const nsRect& aInsetRect,
    const nsRect& aRefBox, nscoord aRadii[8]) {
  const nsStyleCorners& radius = aBasicShape.GetRadius();
||||||| merged common ancestors
/* static */ bool
ShapeUtils::ComputeInsetRadii(const UniquePtr<StyleBasicShape>& aBasicShape,
                              const nsRect& aInsetRect,
                              const nsRect& aRefBox,
                              nscoord aRadii[8])
{
  const nsStyleCorners& radius = aBasicShape->GetRadius();
=======
/* static */
bool ShapeUtils::ComputeInsetRadii(const StyleBasicShape& aBasicShape,
                                   const nsRect& aInsetRect,
                                   const nsRect& aRefBox, nscoord aRadii[8]) {
  const auto& radius = aBasicShape.AsInset().round;
>>>>>>> upstream-releases
  return nsIFrame::ComputeBorderRadii(radius, aInsetRect.Size(), aRefBox.Size(),
                                      Sides(), aRadii);
}

<<<<<<< HEAD
/* static */ nsTArray<nsPoint> ShapeUtils::ComputePolygonVertices(
    const StyleBasicShape& aBasicShape, const nsRect& aRefBox) {
  MOZ_ASSERT(aBasicShape.GetShapeType() == StyleBasicShapeType::Polygon,
             "The basic shape must be polygon()!");

  const nsTArray<nsStyleCoord>& coords = aBasicShape.Coordinates();
  MOZ_ASSERT(coords.Length() % 2 == 0 && coords.Length() >= 2,
             "Wrong number of arguments!");

  nsTArray<nsPoint> vertices(coords.Length() / 2);
  for (size_t i = 0; i + 1 < coords.Length(); i += 2) {
    vertices.AppendElement(
        nsPoint(coords[i].ComputeCoordPercentCalc(aRefBox.width),
                coords[i + 1].ComputeCoordPercentCalc(aRefBox.height)) +
        aRefBox.TopLeft());
||||||| merged common ancestors
/* static */ nsTArray<nsPoint>
ShapeUtils::ComputePolygonVertices(const UniquePtr<StyleBasicShape>& aBasicShape,
                                   const nsRect& aRefBox)
{
  MOZ_ASSERT(aBasicShape->GetShapeType() == StyleBasicShapeType::Polygon,
             "The basic shape must be polygon()!");

  const nsTArray<nsStyleCoord>& coords = aBasicShape->Coordinates();
  MOZ_ASSERT(coords.Length() % 2 == 0 &&
             coords.Length() >= 2, "Wrong number of arguments!");

  nsTArray<nsPoint> vertices(coords.Length() / 2);
  for (size_t i = 0; i + 1 < coords.Length(); i += 2) {
    vertices.AppendElement(
      nsPoint(coords[i].ComputeCoordPercentCalc(aRefBox.width),
              coords[i + 1].ComputeCoordPercentCalc(aRefBox.height))
      + aRefBox.TopLeft());
=======
/* static */
nsTArray<nsPoint> ShapeUtils::ComputePolygonVertices(
    const StyleBasicShape& aBasicShape, const nsRect& aRefBox) {
  MOZ_ASSERT(aBasicShape.IsPolygon(), "The basic shape must be polygon()!");

  auto coords = aBasicShape.AsPolygon().coordinates.AsSpan();
  nsTArray<nsPoint> vertices(coords.Length());
  for (const StylePolygonCoord<LengthPercentage>& point : coords) {
    vertices.AppendElement(nsPoint(point._0.Resolve(aRefBox.width),
                                   point._1.Resolve(aRefBox.height)) +
                           aRefBox.TopLeft());
>>>>>>> upstream-releases
  }
  return vertices;
}

}  // namespace mozilla
