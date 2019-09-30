/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "PathCairo.h"
#include <math.h>
#include "DrawTargetCairo.h"
#include "Logging.h"
#include "PathHelpers.h"
#include "HelpersCairo.h"

namespace mozilla {
namespace gfx {

PathBuilderCairo::PathBuilderCairo(FillRule aFillRule) : mFillRule(aFillRule) {}

<<<<<<< HEAD
void PathBuilderCairo::MoveTo(const Point &aPoint) {
||||||| merged common ancestors
void
PathBuilderCairo::MoveTo(const Point &aPoint)
{
=======
void PathBuilderCairo::MoveTo(const Point& aPoint) {
>>>>>>> upstream-releases
  cairo_path_data_t data;
  data.header.type = CAIRO_PATH_MOVE_TO;
  data.header.length = 2;
  mPathData.push_back(data);
  data.point.x = aPoint.x;
  data.point.y = aPoint.y;
  mPathData.push_back(data);

  mBeginPoint = mCurrentPoint = aPoint;
}

<<<<<<< HEAD
void PathBuilderCairo::LineTo(const Point &aPoint) {
||||||| merged common ancestors
void
PathBuilderCairo::LineTo(const Point &aPoint)
{
=======
void PathBuilderCairo::LineTo(const Point& aPoint) {
>>>>>>> upstream-releases
  cairo_path_data_t data;
  data.header.type = CAIRO_PATH_LINE_TO;
  data.header.length = 2;
  mPathData.push_back(data);
  data.point.x = aPoint.x;
  data.point.y = aPoint.y;
  mPathData.push_back(data);

  mCurrentPoint = aPoint;
}

<<<<<<< HEAD
void PathBuilderCairo::BezierTo(const Point &aCP1, const Point &aCP2,
                                const Point &aCP3) {
||||||| merged common ancestors
void
PathBuilderCairo::BezierTo(const Point &aCP1,
                           const Point &aCP2,
                           const Point &aCP3)
{
=======
void PathBuilderCairo::BezierTo(const Point& aCP1, const Point& aCP2,
                                const Point& aCP3) {
>>>>>>> upstream-releases
  cairo_path_data_t data;
  data.header.type = CAIRO_PATH_CURVE_TO;
  data.header.length = 4;
  mPathData.push_back(data);
  data.point.x = aCP1.x;
  data.point.y = aCP1.y;
  mPathData.push_back(data);
  data.point.x = aCP2.x;
  data.point.y = aCP2.y;
  mPathData.push_back(data);
  data.point.x = aCP3.x;
  data.point.y = aCP3.y;
  mPathData.push_back(data);

  mCurrentPoint = aCP3;
}

<<<<<<< HEAD
void PathBuilderCairo::QuadraticBezierTo(const Point &aCP1, const Point &aCP2) {
||||||| merged common ancestors
void
PathBuilderCairo::QuadraticBezierTo(const Point &aCP1,
                                    const Point &aCP2)
{
=======
void PathBuilderCairo::QuadraticBezierTo(const Point& aCP1, const Point& aCP2) {
>>>>>>> upstream-releases
  // We need to elevate the degree of this quadratic Bézier to cubic, so we're
  // going to add an intermediate control point, and recompute control point 1.
  // The first and last control points remain the same.
  // This formula can be found on http://fontforge.sourceforge.net/bezier.html
  Point CP0 = CurrentPoint();
  Point CP1 = (CP0 + aCP1 * 2.0) / 3.0;
  Point CP2 = (aCP2 + aCP1 * 2.0) / 3.0;
  Point CP3 = aCP2;

  cairo_path_data_t data;
  data.header.type = CAIRO_PATH_CURVE_TO;
  data.header.length = 4;
  mPathData.push_back(data);
  data.point.x = CP1.x;
  data.point.y = CP1.y;
  mPathData.push_back(data);
  data.point.x = CP2.x;
  data.point.y = CP2.y;
  mPathData.push_back(data);
  data.point.x = CP3.x;
  data.point.y = CP3.y;
  mPathData.push_back(data);

  mCurrentPoint = aCP2;
}

void PathBuilderCairo::Close() {
  cairo_path_data_t data;
  data.header.type = CAIRO_PATH_CLOSE_PATH;
  data.header.length = 1;
  mPathData.push_back(data);

  mCurrentPoint = mBeginPoint;
}

<<<<<<< HEAD
void PathBuilderCairo::Arc(const Point &aOrigin, float aRadius,
                           float aStartAngle, float aEndAngle,
                           bool aAntiClockwise) {
  ArcToBezier(this, aOrigin, Size(aRadius, aRadius), aStartAngle, aEndAngle,
              aAntiClockwise);
}

Point PathBuilderCairo::CurrentPoint() const { return mCurrentPoint; }
||||||| merged common ancestors
void
PathBuilderCairo::Arc(const Point &aOrigin, float aRadius, float aStartAngle,
                     float aEndAngle, bool aAntiClockwise)
{
  ArcToBezier(this, aOrigin, Size(aRadius, aRadius), aStartAngle, aEndAngle, aAntiClockwise);
}

Point
PathBuilderCairo::CurrentPoint() const
{
  return mCurrentPoint;
}
=======
void PathBuilderCairo::Arc(const Point& aOrigin, float aRadius,
                           float aStartAngle, float aEndAngle,
                           bool aAntiClockwise) {
  ArcToBezier(this, aOrigin, Size(aRadius, aRadius), aStartAngle, aEndAngle,
              aAntiClockwise);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
already_AddRefed<Path> PathBuilderCairo::Finish() {
  return MakeAndAddRef<PathCairo>(mFillRule, mPathData, mCurrentPoint);
||||||| merged common ancestors
already_AddRefed<Path>
PathBuilderCairo::Finish()
{
  return MakeAndAddRef<PathCairo>(mFillRule, mPathData, mCurrentPoint);
=======
already_AddRefed<Path> PathBuilderCairo::Finish() {
  return MakeAndAddRef<PathCairo>(mFillRule, mPathData, mCurrentPoint,
                                  mBeginPoint);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
PathCairo::PathCairo(FillRule aFillRule,
                     std::vector<cairo_path_data_t> &aPathData,
                     const Point &aCurrentPoint)
    : mFillRule(aFillRule),
      mContainingContext(nullptr),
      mCurrentPoint(aCurrentPoint) {
||||||| merged common ancestors
PathCairo::PathCairo(FillRule aFillRule, std::vector<cairo_path_data_t> &aPathData, const Point &aCurrentPoint)
  : mFillRule(aFillRule)
  , mContainingContext(nullptr)
  , mCurrentPoint(aCurrentPoint)
{
=======
PathCairo::PathCairo(FillRule aFillRule,
                     std::vector<cairo_path_data_t>& aPathData,
                     const Point& aCurrentPoint, const Point& aBeginPoint)
    : mFillRule(aFillRule),
      mContainingContext(nullptr),
      mCurrentPoint(aCurrentPoint),
      mBeginPoint(aBeginPoint) {
>>>>>>> upstream-releases
  mPathData.swap(aPathData);
}

<<<<<<< HEAD
PathCairo::PathCairo(cairo_t *aContext)
    : mFillRule(FillRule::FILL_WINDING), mContainingContext(nullptr) {
  cairo_path_t *path = cairo_copy_path(aContext);
||||||| merged common ancestors
PathCairo::PathCairo(cairo_t *aContext)
  : mFillRule(FillRule::FILL_WINDING)
  , mContainingContext(nullptr)
{
  cairo_path_t *path = cairo_copy_path(aContext);
=======
PathCairo::PathCairo(cairo_t* aContext)
    : mFillRule(FillRule::FILL_WINDING), mContainingContext(nullptr) {
  cairo_path_t* path = cairo_copy_path(aContext);
>>>>>>> upstream-releases

  // XXX - mCurrentPoint is not properly set here, the same is true for the
  // D2D Path code, we never require current point when hitting this codepath
  // but this should be fixed.
  for (int i = 0; i < path->num_data; i++) {
    mPathData.push_back(path->data[i]);
  }

  cairo_path_destroy(path);
}

PathCairo::~PathCairo() {
  if (mContainingContext) {
    cairo_destroy(mContainingContext);
  }
}

already_AddRefed<PathBuilder> PathCairo::CopyToBuilder(
    FillRule aFillRule) const {
  RefPtr<PathBuilderCairo> builder = new PathBuilderCairo(aFillRule);

  builder->mPathData = mPathData;
  builder->mCurrentPoint = mCurrentPoint;
  builder->mBeginPoint = mBeginPoint;

  return builder.forget();
}

<<<<<<< HEAD
already_AddRefed<PathBuilder> PathCairo::TransformedCopyToBuilder(
    const Matrix &aTransform, FillRule aFillRule) const {
||||||| merged common ancestors
already_AddRefed<PathBuilder>
PathCairo::TransformedCopyToBuilder(const Matrix &aTransform, FillRule aFillRule) const
{
=======
already_AddRefed<PathBuilder> PathCairo::TransformedCopyToBuilder(
    const Matrix& aTransform, FillRule aFillRule) const {
>>>>>>> upstream-releases
  RefPtr<PathBuilderCairo> builder = new PathBuilderCairo(aFillRule);

  AppendPathToBuilder(builder, &aTransform);
  builder->mCurrentPoint = aTransform.TransformPoint(mCurrentPoint);
  builder->mBeginPoint = aTransform.TransformPoint(mBeginPoint);

  return builder.forget();
}

<<<<<<< HEAD
bool PathCairo::ContainsPoint(const Point &aPoint,
                              const Matrix &aTransform) const {
||||||| merged common ancestors
bool
PathCairo::ContainsPoint(const Point &aPoint, const Matrix &aTransform) const
{
=======
bool PathCairo::ContainsPoint(const Point& aPoint,
                              const Matrix& aTransform) const {
>>>>>>> upstream-releases
  Matrix inverse = aTransform;
  inverse.Invert();
  Point transformed = inverse.TransformPoint(aPoint);

  EnsureContainingContext(aTransform);

  return cairo_in_fill(mContainingContext, transformed.x, transformed.y);
}

<<<<<<< HEAD
bool PathCairo::StrokeContainsPoint(const StrokeOptions &aStrokeOptions,
                                    const Point &aPoint,
                                    const Matrix &aTransform) const {
||||||| merged common ancestors
bool
PathCairo::StrokeContainsPoint(const StrokeOptions &aStrokeOptions,
                               const Point &aPoint,
                               const Matrix &aTransform) const
{
=======
bool PathCairo::StrokeContainsPoint(const StrokeOptions& aStrokeOptions,
                                    const Point& aPoint,
                                    const Matrix& aTransform) const {
>>>>>>> upstream-releases
  Matrix inverse = aTransform;
  inverse.Invert();
  Point transformed = inverse.TransformPoint(aPoint);

  EnsureContainingContext(aTransform);

  SetCairoStrokeOptions(mContainingContext, aStrokeOptions);

  return cairo_in_stroke(mContainingContext, transformed.x, transformed.y);
}

<<<<<<< HEAD
Rect PathCairo::GetBounds(const Matrix &aTransform) const {
||||||| merged common ancestors
Rect
PathCairo::GetBounds(const Matrix &aTransform) const
{
=======
Rect PathCairo::GetBounds(const Matrix& aTransform) const {
>>>>>>> upstream-releases
  EnsureContainingContext(aTransform);

  double x1, y1, x2, y2;

  cairo_path_extents(mContainingContext, &x1, &y1, &x2, &y2);
  Rect bounds(Float(x1), Float(y1), Float(x2 - x1), Float(y2 - y1));
  return aTransform.TransformBounds(bounds);
}

<<<<<<< HEAD
Rect PathCairo::GetStrokedBounds(const StrokeOptions &aStrokeOptions,
                                 const Matrix &aTransform) const {
||||||| merged common ancestors
Rect
PathCairo::GetStrokedBounds(const StrokeOptions &aStrokeOptions,
                            const Matrix &aTransform) const
{
=======
Rect PathCairo::GetStrokedBounds(const StrokeOptions& aStrokeOptions,
                                 const Matrix& aTransform) const {
>>>>>>> upstream-releases
  EnsureContainingContext(aTransform);

  double x1, y1, x2, y2;

  SetCairoStrokeOptions(mContainingContext, aStrokeOptions);

  cairo_stroke_extents(mContainingContext, &x1, &y1, &x2, &y2);
  Rect bounds((Float)x1, (Float)y1, (Float)(x2 - x1), (Float)(y2 - y1));
  return aTransform.TransformBounds(bounds);
}

<<<<<<< HEAD
void PathCairo::StreamToSink(PathSink *aSink) const {
||||||| merged common ancestors
void
PathCairo::StreamToSink(PathSink *aSink) const
{
=======
void PathCairo::StreamToSink(PathSink* aSink) const {
>>>>>>> upstream-releases
  for (size_t i = 0; i < mPathData.size(); i++) {
    switch (mPathData[i].header.type) {
      case CAIRO_PATH_MOVE_TO:
        i++;
        aSink->MoveTo(Point(mPathData[i].point.x, mPathData[i].point.y));
        break;
      case CAIRO_PATH_LINE_TO:
        i++;
        aSink->LineTo(Point(mPathData[i].point.x, mPathData[i].point.y));
        break;
      case CAIRO_PATH_CURVE_TO:
        aSink->BezierTo(
            Point(mPathData[i + 1].point.x, mPathData[i + 1].point.y),
            Point(mPathData[i + 2].point.x, mPathData[i + 2].point.y),
            Point(mPathData[i + 3].point.x, mPathData[i + 3].point.y));
        i += 3;
        break;
      case CAIRO_PATH_CLOSE_PATH:
        aSink->Close();
        break;
      default:
        // Corrupt path data!
        MOZ_ASSERT(false);
    }
  }
}

<<<<<<< HEAD
void PathCairo::EnsureContainingContext(const Matrix &aTransform) const {
||||||| merged common ancestors
void
PathCairo::EnsureContainingContext(const Matrix &aTransform) const
{
=======
void PathCairo::EnsureContainingContext(const Matrix& aTransform) const {
>>>>>>> upstream-releases
  if (mContainingContext) {
    if (mContainingTransform.ExactlyEquals(aTransform)) {
      return;
    }
  } else {
    mContainingContext = cairo_create(DrawTargetCairo::GetDummySurface());
  }

  mContainingTransform = aTransform;

  cairo_matrix_t mat;
  GfxMatrixToCairoMatrix(mContainingTransform, mat);
  cairo_set_matrix(mContainingContext, &mat);

  SetPathOnContext(mContainingContext);
}

<<<<<<< HEAD
void PathCairo::SetPathOnContext(cairo_t *aContext) const {
||||||| merged common ancestors
void
PathCairo::SetPathOnContext(cairo_t *aContext) const
{
=======
void PathCairo::SetPathOnContext(cairo_t* aContext) const {
>>>>>>> upstream-releases
  // Needs the correct fill rule set.
  cairo_set_fill_rule(aContext, GfxFillRuleToCairoFillRule(mFillRule));

  cairo_new_path(aContext);

  if (!mPathData.empty()) {
    cairo_path_t path;
    path.data = const_cast<cairo_path_data_t *>(&mPathData.front());
    path.num_data = mPathData.size();
    path.status = CAIRO_STATUS_SUCCESS;
    cairo_append_path(aContext, &path);
  }
}

<<<<<<< HEAD
void PathCairo::AppendPathToBuilder(PathBuilderCairo *aBuilder,
                                    const Matrix *aTransform) const {
||||||| merged common ancestors
void
PathCairo::AppendPathToBuilder(PathBuilderCairo *aBuilder, const Matrix *aTransform) const
{
=======
void PathCairo::AppendPathToBuilder(PathBuilderCairo* aBuilder,
                                    const Matrix* aTransform) const {
>>>>>>> upstream-releases
  if (aTransform) {
    size_t i = 0;
    while (i < mPathData.size()) {
      uint32_t pointCount = mPathData[i].header.length - 1;
      aBuilder->mPathData.push_back(mPathData[i]);
      i++;
      for (uint32_t c = 0; c < pointCount; c++) {
        cairo_path_data_t data;
        Point newPoint = aTransform->TransformPoint(
            Point(mPathData[i].point.x, mPathData[i].point.y));
        data.point.x = newPoint.x;
        data.point.y = newPoint.y;
        aBuilder->mPathData.push_back(data);
        i++;
      }
    }
  } else {
    for (size_t i = 0; i < mPathData.size(); i++) {
      aBuilder->mPathData.push_back(mPathData[i]);
    }
  }
}

}  // namespace gfx
}  // namespace mozilla
