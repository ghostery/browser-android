/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_PATH_SKIA_H_
#define MOZILLA_GFX_PATH_SKIA_H_

#include "2D.h"
#include "skia/include/core/SkPath.h"

namespace mozilla {
namespace gfx {

class PathSkia;

class PathBuilderSkia : public PathBuilder {
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(PathBuilderSkia, override)

<<<<<<< HEAD
  PathBuilderSkia(const Matrix &aTransform, const SkPath &aPath,
                  FillRule aFillRule);
||||||| merged common ancestors
  PathBuilderSkia(const Matrix& aTransform, const SkPath& aPath, FillRule aFillRule);
=======
  PathBuilderSkia(const Matrix& aTransform, const SkPath& aPath,
                  FillRule aFillRule);
>>>>>>> upstream-releases
  explicit PathBuilderSkia(FillRule aFillRule);

<<<<<<< HEAD
  virtual void MoveTo(const Point &aPoint) override;
  virtual void LineTo(const Point &aPoint) override;
  virtual void BezierTo(const Point &aCP1, const Point &aCP2,
                        const Point &aCP3) override;
  virtual void QuadraticBezierTo(const Point &aCP1, const Point &aCP2) override;
  virtual void Close() override;
  virtual void Arc(const Point &aOrigin, float aRadius, float aStartAngle,
                   float aEndAngle, bool aAntiClockwise = false) override;
  virtual Point CurrentPoint() const override;
  virtual already_AddRefed<Path> Finish() override;
||||||| merged common ancestors
  virtual void MoveTo(const Point &aPoint) override;
  virtual void LineTo(const Point &aPoint) override;
  virtual void BezierTo(const Point &aCP1,
                        const Point &aCP2,
                        const Point &aCP3) override;
  virtual void QuadraticBezierTo(const Point &aCP1,
                                 const Point &aCP2) override;
  virtual void Close() override;
  virtual void Arc(const Point &aOrigin, float aRadius, float aStartAngle,
                   float aEndAngle, bool aAntiClockwise = false) override;
  virtual Point CurrentPoint() const override;
  virtual already_AddRefed<Path> Finish() override;
=======
  void MoveTo(const Point& aPoint) override;
  void LineTo(const Point& aPoint) override;
  void BezierTo(const Point& aCP1, const Point& aCP2,
                const Point& aCP3) override;
  void QuadraticBezierTo(const Point& aCP1, const Point& aCP2) override;
  void Close() override;
  void Arc(const Point& aOrigin, float aRadius, float aStartAngle,
           float aEndAngle, bool aAntiClockwise = false) override;
  already_AddRefed<Path> Finish() override;
>>>>>>> upstream-releases

  void AppendPath(const SkPath& aPath);

<<<<<<< HEAD
  virtual BackendType GetBackendType() const override {
    return BackendType::SKIA;
  }
||||||| merged common ancestors
  virtual BackendType GetBackendType() const override { return BackendType::SKIA; }

private:
=======
  BackendType GetBackendType() const override { return BackendType::SKIA; }

 private:
  friend class PathSkia;
>>>>>>> upstream-releases

 private:
  void SetFillRule(FillRule aFillRule);

  SkPath mPath;
  FillRule mFillRule;
};

class PathSkia : public Path {
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(PathSkia, override)

<<<<<<< HEAD
  PathSkia(SkPath &aPath, FillRule aFillRule) : mFillRule(aFillRule) {
||||||| merged common ancestors
  PathSkia(SkPath& aPath, FillRule aFillRule)
    : mFillRule(aFillRule)
  {
=======
  PathSkia(SkPath& aPath, FillRule aFillRule, Point aCurrentPoint = Point(),
           Point aBeginPoint = Point())
      : mFillRule(aFillRule),
        mCurrentPoint(aCurrentPoint),
        mBeginPoint(aBeginPoint) {
>>>>>>> upstream-releases
    mPath.swap(aPath);
  }

<<<<<<< HEAD
  virtual BackendType GetBackendType() const override {
    return BackendType::SKIA;
  }
||||||| merged common ancestors
  virtual BackendType GetBackendType() const override { return BackendType::SKIA; }
=======
  BackendType GetBackendType() const override { return BackendType::SKIA; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual already_AddRefed<PathBuilder> CopyToBuilder(
      FillRule aFillRule) const override;
  virtual already_AddRefed<PathBuilder> TransformedCopyToBuilder(
      const Matrix &aTransform, FillRule aFillRule) const override;
||||||| merged common ancestors
  virtual already_AddRefed<PathBuilder> CopyToBuilder(FillRule aFillRule) const override;
  virtual already_AddRefed<PathBuilder> TransformedCopyToBuilder(const Matrix &aTransform,
                                                             FillRule aFillRule) const override;
=======
  already_AddRefed<PathBuilder> CopyToBuilder(
      FillRule aFillRule) const override;
  already_AddRefed<PathBuilder> TransformedCopyToBuilder(
      const Matrix& aTransform, FillRule aFillRule) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool ContainsPoint(const Point &aPoint,
                             const Matrix &aTransform) const override;
||||||| merged common ancestors
  virtual bool ContainsPoint(const Point &aPoint, const Matrix &aTransform) const override;
=======
  bool ContainsPoint(const Point& aPoint,
                     const Matrix& aTransform) const override;
>>>>>>> upstream-releases

  bool StrokeContainsPoint(const StrokeOptions& aStrokeOptions,
                           const Point& aPoint,
                           const Matrix& aTransform) const override;

  Rect GetBounds(const Matrix& aTransform = Matrix()) const override;

<<<<<<< HEAD
  virtual Rect GetStrokedBounds(
      const StrokeOptions &aStrokeOptions,
      const Matrix &aTransform = Matrix()) const override;
||||||| merged common ancestors
  virtual Rect GetStrokedBounds(const StrokeOptions &aStrokeOptions,
                                const Matrix &aTransform = Matrix()) const override;
=======
  Rect GetStrokedBounds(const StrokeOptions& aStrokeOptions,
                        const Matrix& aTransform = Matrix()) const override;
>>>>>>> upstream-releases

  void StreamToSink(PathSink* aSink) const override;

  FillRule GetFillRule() const override { return mFillRule; }

  const SkPath &GetPath() const { return mPath; }

 private:
  friend class DrawTargetSkia;

  SkPath mPath;
  FillRule mFillRule;
  Point mCurrentPoint;
  Point mBeginPoint;
};

}  // namespace gfx
}  // namespace mozilla

#endif /* MOZILLA_GFX_PATH_SKIA_H_ */
