/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_PATH_CAIRO_H_
#define MOZILLA_GFX_PATH_CAIRO_H_

#include "2D.h"
#include "cairo.h"
#include <vector>

namespace mozilla {
namespace gfx {

class PathCairo;

class PathBuilderCairo : public PathBuilder {
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(PathBuilderCairo, override)

  explicit PathBuilderCairo(FillRule aFillRule);

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

  virtual BackendType GetBackendType() const override {
    return BackendType::CAIRO;
  }

 private:  // data
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

  virtual BackendType GetBackendType() const override { return BackendType::CAIRO; }

private: // data
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

  BackendType GetBackendType() const override { return BackendType::CAIRO; }

 private:  // data
>>>>>>> upstream-releases
  friend class PathCairo;

  FillRule mFillRule;
  std::vector<cairo_path_data_t> mPathData;
};

class PathCairo : public Path {
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(PathCairo, override)

<<<<<<< HEAD
  PathCairo(FillRule aFillRule, std::vector<cairo_path_data_t> &aPathData,
            const Point &aCurrentPoint);
  explicit PathCairo(cairo_t *aContext);
  ~PathCairo();
||||||| merged common ancestors
  PathCairo(FillRule aFillRule, std::vector<cairo_path_data_t> &aPathData, const Point &aCurrentPoint);
  explicit PathCairo(cairo_t *aContext);
  ~PathCairo();
=======
  PathCairo(FillRule aFillRule, std::vector<cairo_path_data_t>& aPathData,
            const Point& aCurrentPoint, const Point& aBeginPoint);
  explicit PathCairo(cairo_t* aContext);
  virtual ~PathCairo();

  BackendType GetBackendType() const override { return BackendType::CAIRO; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual BackendType GetBackendType() const override {
    return BackendType::CAIRO;
  }
||||||| merged common ancestors
  virtual BackendType GetBackendType() const override { return BackendType::CAIRO; }
=======
  already_AddRefed<PathBuilder> CopyToBuilder(
      FillRule aFillRule) const override;
  already_AddRefed<PathBuilder> TransformedCopyToBuilder(
      const Matrix& aTransform, FillRule aFillRule) const override;
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
  bool ContainsPoint(const Point& aPoint,
                     const Matrix& aTransform) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool ContainsPoint(const Point &aPoint,
                             const Matrix &aTransform) const override;
||||||| merged common ancestors
  virtual bool ContainsPoint(const Point &aPoint, const Matrix &aTransform) const override;
=======
  bool StrokeContainsPoint(const StrokeOptions& aStrokeOptions,
                           const Point& aPoint,
                           const Matrix& aTransform) const override;
>>>>>>> upstream-releases

  Rect GetBounds(const Matrix& aTransform = Matrix()) const override;

  Rect GetStrokedBounds(const StrokeOptions& aStrokeOptions,
                        const Matrix& aTransform = Matrix()) const override;

<<<<<<< HEAD
  virtual Rect GetStrokedBounds(
      const StrokeOptions &aStrokeOptions,
      const Matrix &aTransform = Matrix()) const override;
||||||| merged common ancestors
  virtual Rect GetStrokedBounds(const StrokeOptions &aStrokeOptions,
                                const Matrix &aTransform = Matrix()) const override;
=======
  void StreamToSink(PathSink* aSink) const override;
>>>>>>> upstream-releases

  FillRule GetFillRule() const override { return mFillRule; }

  void SetPathOnContext(cairo_t* aContext) const;

  void AppendPathToBuilder(PathBuilderCairo* aBuilder,
                           const Matrix* aTransform = nullptr) const;

<<<<<<< HEAD
  void AppendPathToBuilder(PathBuilderCairo *aBuilder,
                           const Matrix *aTransform = nullptr) const;

 private:
  void EnsureContainingContext(const Matrix &aTransform) const;
||||||| merged common ancestors
  void AppendPathToBuilder(PathBuilderCairo *aBuilder, const Matrix *aTransform = nullptr) const;
private:
  void EnsureContainingContext(const Matrix &aTransform) const;
=======
 private:
  void EnsureContainingContext(const Matrix& aTransform) const;
>>>>>>> upstream-releases

  FillRule mFillRule;
  std::vector<cairo_path_data_t> mPathData;
  mutable cairo_t* mContainingContext;
  mutable Matrix mContainingTransform;
  Point mCurrentPoint;
  Point mBeginPoint;
};

}  // namespace gfx
}  // namespace mozilla

#endif /* MOZILLA_GFX_PATH_CAIRO_H_ */
