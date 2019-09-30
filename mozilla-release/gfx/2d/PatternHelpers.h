/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _MOZILLA_GFX_PATTERNHELPERS_H
#define _MOZILLA_GFX_PATTERNHELPERS_H

#include "mozilla/Alignment.h"
#include "mozilla/gfx/2D.h"

namespace mozilla {
namespace gfx {

/**
 * This class is used to allow general pattern creation functions to return
 * any type of pattern via an out-paramater without allocating a pattern
 * instance on the free-store (an instance of this class being created on the
 * stack before passing it in to the creation function). Without this class
 * writing pattern creation functions would be a pain since Pattern objects are
 * not reference counted, making lifetime management of instances created on
 * the free-store and returned from a creation function hazardous. Besides
 * that, in the case that ColorPattern's are expected to be common, it is
 * particularly desirable to avoid the overhead of allocating on the
 * free-store.
 */
<<<<<<< HEAD
class GeneralPattern {
 public:
  explicit GeneralPattern() : mPattern(nullptr) {}
||||||| merged common ancestors
class GeneralPattern
{
public:
  explicit GeneralPattern()
    : mPattern(nullptr)
  {}
=======
class GeneralPattern final {
 public:
  explicit GeneralPattern() = default;
>>>>>>> upstream-releases

<<<<<<< HEAD
  GeneralPattern(const GeneralPattern &aOther) : mPattern(nullptr) {}
||||||| merged common ancestors
  GeneralPattern(const GeneralPattern& aOther)
    : mPattern(nullptr)
  {}
=======
  GeneralPattern(const GeneralPattern& aOther) {}
>>>>>>> upstream-releases

  ~GeneralPattern() {
    if (mPattern) {
      mPattern->~Pattern();
    }
  }

  Pattern *Init(const Pattern &aPattern) {
    MOZ_ASSERT(!mPattern);
    switch (aPattern.GetType()) {
<<<<<<< HEAD
      case PatternType::COLOR:
        mPattern = new (mColorPattern.addr())
            ColorPattern(static_cast<const ColorPattern &>(aPattern));
        break;
      case PatternType::LINEAR_GRADIENT:
        mPattern = new (mLinearGradientPattern.addr()) LinearGradientPattern(
            static_cast<const LinearGradientPattern &>(aPattern));
        break;
      case PatternType::RADIAL_GRADIENT:
        mPattern = new (mRadialGradientPattern.addr()) RadialGradientPattern(
            static_cast<const RadialGradientPattern &>(aPattern));
        break;
      case PatternType::SURFACE:
        mPattern = new (mSurfacePattern.addr())
            SurfacePattern(static_cast<const SurfacePattern &>(aPattern));
        break;
      default:
        MOZ_MAKE_COMPILER_ASSUME_IS_UNREACHABLE("Unknown pattern type");
||||||| merged common ancestors
    case PatternType::COLOR:
      mPattern = new (mColorPattern.addr())
        ColorPattern(static_cast<const ColorPattern&>(aPattern));
      break;
    case PatternType::LINEAR_GRADIENT:
      mPattern = new (mLinearGradientPattern.addr())
        LinearGradientPattern(static_cast<const LinearGradientPattern&>(aPattern));
      break;
    case PatternType::RADIAL_GRADIENT:
      mPattern = new (mRadialGradientPattern.addr())
        RadialGradientPattern(static_cast<const RadialGradientPattern&>(aPattern));
      break;
    case PatternType::SURFACE:
      mPattern = new (mSurfacePattern.addr())
        SurfacePattern(static_cast<const SurfacePattern&>(aPattern));
      break;
    default:
      MOZ_MAKE_COMPILER_ASSUME_IS_UNREACHABLE("Unknown pattern type");
=======
      case PatternType::COLOR:
        mPattern = new (mColorPattern.addr())
            ColorPattern(static_cast<const ColorPattern&>(aPattern));
        break;
      case PatternType::LINEAR_GRADIENT:
        mPattern = new (mLinearGradientPattern.addr()) LinearGradientPattern(
            static_cast<const LinearGradientPattern&>(aPattern));
        break;
      case PatternType::RADIAL_GRADIENT:
        mPattern = new (mRadialGradientPattern.addr()) RadialGradientPattern(
            static_cast<const RadialGradientPattern&>(aPattern));
        break;
      case PatternType::SURFACE:
        mPattern = new (mSurfacePattern.addr())
            SurfacePattern(static_cast<const SurfacePattern&>(aPattern));
        break;
      default:
        MOZ_MAKE_COMPILER_ASSUME_IS_UNREACHABLE("Unknown pattern type");
>>>>>>> upstream-releases
    }
    return mPattern;
  }

<<<<<<< HEAD
  ColorPattern *InitColorPattern(const Color &aColor) {
||||||| merged common ancestors
  ColorPattern* InitColorPattern(const Color &aColor) {
=======
  ColorPattern* InitColorPattern(const Color& aColor) {
>>>>>>> upstream-releases
    MOZ_ASSERT(!mPattern);
    mPattern = new (mColorPattern.addr()) ColorPattern(aColor);
    return mColorPattern.addr();
  }

<<<<<<< HEAD
  LinearGradientPattern *InitLinearGradientPattern(
      const Point &aBegin, const Point &aEnd, GradientStops *aStops,
      const Matrix &aMatrix = Matrix()) {
||||||| merged common ancestors
  LinearGradientPattern* InitLinearGradientPattern(const Point &aBegin,
                                                   const Point &aEnd,
                                                   GradientStops *aStops,
                                                   const Matrix &aMatrix = Matrix()) {
=======
  LinearGradientPattern* InitLinearGradientPattern(
      const Point& aBegin, const Point& aEnd, GradientStops* aStops,
      const Matrix& aMatrix = Matrix()) {
>>>>>>> upstream-releases
    MOZ_ASSERT(!mPattern);
    mPattern = new (mLinearGradientPattern.addr())
        LinearGradientPattern(aBegin, aEnd, aStops, aMatrix);
    return mLinearGradientPattern.addr();
  }

<<<<<<< HEAD
  RadialGradientPattern *InitRadialGradientPattern(
      const Point &aCenter1, const Point &aCenter2, Float aRadius1,
      Float aRadius2, GradientStops *aStops, const Matrix &aMatrix = Matrix()) {
||||||| merged common ancestors
  RadialGradientPattern* InitRadialGradientPattern(const Point &aCenter1,
                                                   const Point &aCenter2,
                                                   Float aRadius1,
                                                   Float aRadius2,
                                                   GradientStops *aStops,
                                                   const Matrix &aMatrix = Matrix()) {
=======
  RadialGradientPattern* InitRadialGradientPattern(
      const Point& aCenter1, const Point& aCenter2, Float aRadius1,
      Float aRadius2, GradientStops* aStops, const Matrix& aMatrix = Matrix()) {
>>>>>>> upstream-releases
    MOZ_ASSERT(!mPattern);
    mPattern = new (mRadialGradientPattern.addr()) RadialGradientPattern(
        aCenter1, aCenter2, aRadius1, aRadius2, aStops, aMatrix);
    return mRadialGradientPattern.addr();
  }

<<<<<<< HEAD
  SurfacePattern *InitSurfacePattern(
      SourceSurface *aSourceSurface, ExtendMode aExtendMode,
      const Matrix &aMatrix = Matrix(),
      SamplingFilter aSamplingFilter = SamplingFilter::GOOD,
      const IntRect &aSamplingRect = IntRect()) {
||||||| merged common ancestors
  SurfacePattern* InitSurfacePattern(SourceSurface *aSourceSurface,
                                     ExtendMode aExtendMode,
                                     const Matrix &aMatrix = Matrix(),
                                     SamplingFilter aSamplingFilter = SamplingFilter::GOOD,
                                     const IntRect &aSamplingRect = IntRect()) {
=======
  SurfacePattern* InitSurfacePattern(
      SourceSurface* aSourceSurface, ExtendMode aExtendMode,
      const Matrix& aMatrix = Matrix(),
      SamplingFilter aSamplingFilter = SamplingFilter::GOOD,
      const IntRect& aSamplingRect = IntRect()) {
>>>>>>> upstream-releases
    MOZ_ASSERT(!mPattern);
    mPattern = new (mSurfacePattern.addr()) SurfacePattern(
        aSourceSurface, aExtendMode, aMatrix, aSamplingFilter, aSamplingRect);
    return mSurfacePattern.addr();
  }

<<<<<<< HEAD
  Pattern *GetPattern() { return mPattern; }
||||||| merged common ancestors
  Pattern* GetPattern() {
    return mPattern;
  }
=======
  Pattern* GetPattern() { return mPattern; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const Pattern *GetPattern() const { return mPattern; }
||||||| merged common ancestors
  const Pattern* GetPattern() const {
    return mPattern;
  }
=======
  const Pattern* GetPattern() const { return mPattern; }
>>>>>>> upstream-releases

  operator Pattern &() {
    if (!mPattern) {
      MOZ_CRASH("GFX: GeneralPattern not initialized");
    }
    return *mPattern;
  }

 private:
  union {
    AlignedStorage2<ColorPattern> mColorPattern;
    AlignedStorage2<LinearGradientPattern> mLinearGradientPattern;
    AlignedStorage2<RadialGradientPattern> mRadialGradientPattern;
    AlignedStorage2<SurfacePattern> mSurfacePattern;
  };
  Pattern* mPattern = nullptr;
};

}  // namespace gfx
}  // namespace mozilla

#endif  //  _MOZILLA_GFX_PATTERNHELPERS_H
