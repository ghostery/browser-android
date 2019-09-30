/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "PathSkia.h"
#include <math.h>
#include "DrawTargetSkia.h"
#include "Logging.h"
#include "HelpersSkia.h"
#include "PathHelpers.h"

namespace mozilla {
namespace gfx {

<<<<<<< HEAD
PathBuilderSkia::PathBuilderSkia(const Matrix &aTransform, const SkPath &aPath,
                                 FillRule aFillRule)
    : mPath(aPath) {
||||||| merged common ancestors
PathBuilderSkia::PathBuilderSkia(const Matrix& aTransform, const SkPath& aPath, FillRule aFillRule)
  : mPath(aPath)
{
=======
PathBuilderSkia::PathBuilderSkia(const Matrix& aTransform, const SkPath& aPath,
                                 FillRule aFillRule)
    : mPath(aPath) {
>>>>>>> upstream-releases
  SkMatrix matrix;
  GfxMatrixToSkiaMatrix(aTransform, matrix);
  mPath.transform(matrix);
  SetFillRule(aFillRule);
}

PathBuilderSkia::PathBuilderSkia(FillRule aFillRule) { SetFillRule(aFillRule); }

void PathBuilderSkia::SetFillRule(FillRule aFillRule) {
  mFillRule = aFillRule;
  if (mFillRule == FillRule::FILL_WINDING) {
    mPath.setFillType(SkPath::kWinding_FillType);
  } else {
    mPath.setFillType(SkPath::kEvenOdd_FillType);
  }
}

<<<<<<< HEAD
void PathBuilderSkia::MoveTo(const Point &aPoint) {
||||||| merged common ancestors
void
PathBuilderSkia::MoveTo(const Point &aPoint)
{
=======
void PathBuilderSkia::MoveTo(const Point& aPoint) {
>>>>>>> upstream-releases
  mPath.moveTo(SkFloatToScalar(aPoint.x), SkFloatToScalar(aPoint.y));
  mCurrentPoint = aPoint;
  mBeginPoint = aPoint;
}

<<<<<<< HEAD
void PathBuilderSkia::LineTo(const Point &aPoint) {
||||||| merged common ancestors
void
PathBuilderSkia::LineTo(const Point &aPoint)
{
=======
void PathBuilderSkia::LineTo(const Point& aPoint) {
>>>>>>> upstream-releases
  if (!mPath.countPoints()) {
    MoveTo(aPoint);
  } else {
    mPath.lineTo(SkFloatToScalar(aPoint.x), SkFloatToScalar(aPoint.y));
  }
  mCurrentPoint = aPoint;
}

<<<<<<< HEAD
void PathBuilderSkia::BezierTo(const Point &aCP1, const Point &aCP2,
                               const Point &aCP3) {
||||||| merged common ancestors
void
PathBuilderSkia::BezierTo(const Point &aCP1,
                          const Point &aCP2,
                          const Point &aCP3)
{
=======
void PathBuilderSkia::BezierTo(const Point& aCP1, const Point& aCP2,
                               const Point& aCP3) {
>>>>>>> upstream-releases
  if (!mPath.countPoints()) {
    MoveTo(aCP1);
  }
  mPath.cubicTo(SkFloatToScalar(aCP1.x), SkFloatToScalar(aCP1.y),
                SkFloatToScalar(aCP2.x), SkFloatToScalar(aCP2.y),
                SkFloatToScalar(aCP3.x), SkFloatToScalar(aCP3.y));
  mCurrentPoint = aCP3;
}

<<<<<<< HEAD
void PathBuilderSkia::QuadraticBezierTo(const Point &aCP1, const Point &aCP2) {
||||||| merged common ancestors
void
PathBuilderSkia::QuadraticBezierTo(const Point &aCP1,
                                   const Point &aCP2)
{
=======
void PathBuilderSkia::QuadraticBezierTo(const Point& aCP1, const Point& aCP2) {
>>>>>>> upstream-releases
  if (!mPath.countPoints()) {
    MoveTo(aCP1);
  }
  mPath.quadTo(SkFloatToScalar(aCP1.x), SkFloatToScalar(aCP1.y),
               SkFloatToScalar(aCP2.x), SkFloatToScalar(aCP2.y));
  mCurrentPoint = aCP2;
}

<<<<<<< HEAD
void PathBuilderSkia::Close() { mPath.close(); }
||||||| merged common ancestors
void
PathBuilderSkia::Close()
{
  mPath.close();
}
=======
void PathBuilderSkia::Close() {
  mPath.close();
  mCurrentPoint = mBeginPoint;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
void PathBuilderSkia::Arc(const Point &aOrigin, float aRadius,
                          float aStartAngle, float aEndAngle,
                          bool aAntiClockwise) {
  ArcToBezier(this, aOrigin, Size(aRadius, aRadius), aStartAngle, aEndAngle,
              aAntiClockwise);
}

Point PathBuilderSkia::CurrentPoint() const {
  int pointCount = mPath.countPoints();
  if (!pointCount) {
    return Point(0, 0);
  }
  SkPoint point = mPath.getPoint(pointCount - 1);
  return Point(SkScalarToFloat(point.fX), SkScalarToFloat(point.fY));
||||||| merged common ancestors
void
PathBuilderSkia::Arc(const Point &aOrigin, float aRadius, float aStartAngle,
                     float aEndAngle, bool aAntiClockwise)
{
  ArcToBezier(this, aOrigin, Size(aRadius, aRadius), aStartAngle, aEndAngle, aAntiClockwise);
}

Point
PathBuilderSkia::CurrentPoint() const
{
  int pointCount = mPath.countPoints();
  if (!pointCount) {
    return Point(0, 0);
  }
  SkPoint point = mPath.getPoint(pointCount - 1);
  return Point(SkScalarToFloat(point.fX), SkScalarToFloat(point.fY));
=======
void PathBuilderSkia::Arc(const Point& aOrigin, float aRadius,
                          float aStartAngle, float aEndAngle,
                          bool aAntiClockwise) {
  ArcToBezier(this, aOrigin, Size(aRadius, aRadius), aStartAngle, aEndAngle,
              aAntiClockwise);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
already_AddRefed<Path> PathBuilderSkia::Finish() {
  return MakeAndAddRef<PathSkia>(mPath, mFillRule);
||||||| merged common ancestors
already_AddRefed<Path>
PathBuilderSkia::Finish()
{
  return MakeAndAddRef<PathSkia>(mPath, mFillRule);
=======
already_AddRefed<Path> PathBuilderSkia::Finish() {
  RefPtr<Path> path =
      MakeAndAddRef<PathSkia>(mPath, mFillRule, mCurrentPoint, mBeginPoint);
  mCurrentPoint = Point(0.0, 0.0);
  mBeginPoint = Point(0.0, 0.0);
  return path.forget();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void PathBuilderSkia::AppendPath(const SkPath &aPath) { mPath.addPath(aPath); }
||||||| merged common ancestors
void
PathBuilderSkia::AppendPath(const SkPath &aPath)
{
  mPath.addPath(aPath);
}
=======
void PathBuilderSkia::AppendPath(const SkPath& aPath) { mPath.addPath(aPath); }
>>>>>>> upstream-releases

already_AddRefed<PathBuilder> PathSkia::CopyToBuilder(
    FillRule aFillRule) const {
  return TransformedCopyToBuilder(Matrix(), aFillRule);
}

<<<<<<< HEAD
already_AddRefed<PathBuilder> PathSkia::TransformedCopyToBuilder(
    const Matrix &aTransform, FillRule aFillRule) const {
  return MakeAndAddRef<PathBuilderSkia>(aTransform, mPath, aFillRule);
||||||| merged common ancestors
already_AddRefed<PathBuilder>
PathSkia::TransformedCopyToBuilder(const Matrix &aTransform, FillRule aFillRule) const
{
  return MakeAndAddRef<PathBuilderSkia>(aTransform, mPath, aFillRule);
=======
already_AddRefed<PathBuilder> PathSkia::TransformedCopyToBuilder(
    const Matrix& aTransform, FillRule aFillRule) const {
  RefPtr<PathBuilderSkia> builder =
      MakeAndAddRef<PathBuilderSkia>(aTransform, mPath, aFillRule);

  builder->mCurrentPoint = aTransform.TransformPoint(mCurrentPoint);
  builder->mBeginPoint = aTransform.TransformPoint(mBeginPoint);

  return builder.forget();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool SkPathContainsPoint(const SkPath &aPath, const Point &aPoint,
                                const Matrix &aTransform) {
||||||| merged common ancestors
static bool
SkPathContainsPoint(const SkPath& aPath, const Point& aPoint, const Matrix& aTransform)
{
=======
static bool SkPathContainsPoint(const SkPath& aPath, const Point& aPoint,
                                const Matrix& aTransform) {
>>>>>>> upstream-releases
  Matrix inverse = aTransform;
  if (!inverse.Invert()) {
    return false;
  }

  SkPoint point = PointToSkPoint(inverse.TransformPoint(aPoint));
  return aPath.contains(point.fX, point.fY);
}

<<<<<<< HEAD
bool PathSkia::ContainsPoint(const Point &aPoint,
                             const Matrix &aTransform) const {
||||||| merged common ancestors
bool
PathSkia::ContainsPoint(const Point &aPoint, const Matrix &aTransform) const
{
=======
bool PathSkia::ContainsPoint(const Point& aPoint,
                             const Matrix& aTransform) const {
>>>>>>> upstream-releases
  if (!mPath.isFinite()) {
    return false;
  }

  return SkPathContainsPoint(mPath, aPoint, aTransform);
}

<<<<<<< HEAD
bool PathSkia::StrokeContainsPoint(const StrokeOptions &aStrokeOptions,
                                   const Point &aPoint,
                                   const Matrix &aTransform) const {
||||||| merged common ancestors
bool
PathSkia::StrokeContainsPoint(const StrokeOptions &aStrokeOptions,
                              const Point &aPoint,
                              const Matrix &aTransform) const
{
=======
bool PathSkia::StrokeContainsPoint(const StrokeOptions& aStrokeOptions,
                                   const Point& aPoint,
                                   const Matrix& aTransform) const {
>>>>>>> upstream-releases
  if (!mPath.isFinite()) {
    return false;
  }

  SkPaint paint;
  if (!StrokeOptionsToPaint(paint, aStrokeOptions)) {
    return false;
  }

  SkPath strokePath;
  paint.getFillPath(mPath, &strokePath);

  return SkPathContainsPoint(strokePath, aPoint, aTransform);
}

<<<<<<< HEAD
Rect PathSkia::GetBounds(const Matrix &aTransform) const {
||||||| merged common ancestors
Rect
PathSkia::GetBounds(const Matrix &aTransform) const
{
=======
Rect PathSkia::GetBounds(const Matrix& aTransform) const {
>>>>>>> upstream-releases
  if (!mPath.isFinite()) {
    return Rect();
  }

  Rect bounds = SkRectToRect(mPath.computeTightBounds());
  return aTransform.TransformBounds(bounds);
}

<<<<<<< HEAD
Rect PathSkia::GetStrokedBounds(const StrokeOptions &aStrokeOptions,
                                const Matrix &aTransform) const {
||||||| merged common ancestors
Rect
PathSkia::GetStrokedBounds(const StrokeOptions &aStrokeOptions,
                           const Matrix &aTransform) const
{
=======
Rect PathSkia::GetStrokedBounds(const StrokeOptions& aStrokeOptions,
                                const Matrix& aTransform) const {
>>>>>>> upstream-releases
  if (!mPath.isFinite()) {
    return Rect();
  }

  SkPaint paint;
  if (!StrokeOptionsToPaint(paint, aStrokeOptions)) {
    return Rect();
  }

  SkPath result;
  paint.getFillPath(mPath, &result);

  Rect bounds = SkRectToRect(result.computeTightBounds());
  return aTransform.TransformBounds(bounds);
}

<<<<<<< HEAD
void PathSkia::StreamToSink(PathSink *aSink) const {
||||||| merged common ancestors
void
PathSkia::StreamToSink(PathSink *aSink) const
{
=======
void PathSkia::StreamToSink(PathSink* aSink) const {
>>>>>>> upstream-releases
  SkPath::RawIter iter(mPath);

  SkPoint points[4];
  SkPath::Verb currentVerb;
  while ((currentVerb = iter.next(points)) != SkPath::kDone_Verb) {
    switch (currentVerb) {
      case SkPath::kMove_Verb:
        aSink->MoveTo(SkPointToPoint(points[0]));
        break;
      case SkPath::kLine_Verb:
        aSink->LineTo(SkPointToPoint(points[1]));
        break;
      case SkPath::kCubic_Verb:
        aSink->BezierTo(SkPointToPoint(points[1]), SkPointToPoint(points[2]),
                        SkPointToPoint(points[3]));
        break;
      case SkPath::kQuad_Verb:
        aSink->QuadraticBezierTo(SkPointToPoint(points[1]),
                                 SkPointToPoint(points[2]));
        break;
      case SkPath::kClose_Verb:
        aSink->Close();
        break;
      default:
        MOZ_ASSERT(false);
        // Unexpected verb found in path!
    }
  }
}

}  // namespace gfx
}  // namespace mozilla
