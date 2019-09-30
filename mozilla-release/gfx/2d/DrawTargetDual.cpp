/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "DrawTargetDual.h"
#include "Tools.h"
#include "Logging.h"

namespace mozilla {
namespace gfx {

<<<<<<< HEAD
class DualSurface {
 public:
  inline explicit DualSurface(SourceSurface *aSurface) {
||||||| merged common ancestors
class DualSurface
{
public:
  inline explicit DualSurface(SourceSurface *aSurface)
  {
=======
class DualSurface {
 public:
  inline explicit DualSurface(SourceSurface* aSurface) {
>>>>>>> upstream-releases
    if (!aSurface) {
      mA = mB = nullptr;
      return;
    }

    if (aSurface->GetType() != SurfaceType::DUAL_DT) {
      mA = mB = aSurface;
      return;
    }

<<<<<<< HEAD
    SourceSurfaceDual *ssDual = static_cast<SourceSurfaceDual *>(aSurface);
||||||| merged common ancestors
    SourceSurfaceDual *ssDual =
      static_cast<SourceSurfaceDual*>(aSurface);
=======
    SourceSurfaceDual* ssDual = static_cast<SourceSurfaceDual*>(aSurface);
>>>>>>> upstream-releases
    mA = ssDual->mA;
    mB = ssDual->mB;
  }

  SourceSurface* mA;
  SourceSurface* mB;
};

/* This only needs to split patterns up for SurfacePatterns. Only in that
 * case can we be dealing with a 'dual' source (SourceSurfaceDual) and do
 * we need to pass separate patterns into our destination DrawTargets.
 */
<<<<<<< HEAD
class DualPattern {
 public:
  inline explicit DualPattern(const Pattern &aPattern)
      : mPatternsInitialized(false) {
||||||| merged common ancestors
class DualPattern
{
public:
  inline explicit DualPattern(const Pattern &aPattern)
    : mPatternsInitialized(false)
  {
=======
class DualPattern final {
 public:
  inline explicit DualPattern(const Pattern& aPattern)
      : mPatternsInitialized(false) {
>>>>>>> upstream-releases
    if (aPattern.GetType() != PatternType::SURFACE) {
      mA = mB = &aPattern;
      return;
    }

<<<<<<< HEAD
    const SurfacePattern *surfPat =
        static_cast<const SurfacePattern *>(&aPattern);
||||||| merged common ancestors
    const SurfacePattern *surfPat =
      static_cast<const SurfacePattern*>(&aPattern);
=======
    const SurfacePattern* surfPat =
        static_cast<const SurfacePattern*>(&aPattern);
>>>>>>> upstream-releases

    if (surfPat->mSurface->GetType() != SurfaceType::DUAL_DT) {
      mA = mB = &aPattern;
      return;
    }

<<<<<<< HEAD
    const SourceSurfaceDual *ssDual =
        static_cast<const SourceSurfaceDual *>(surfPat->mSurface.get());
    mA = new (mSurfPatA.addr())
        SurfacePattern(ssDual->mA, surfPat->mExtendMode, surfPat->mMatrix,
                       surfPat->mSamplingFilter);
    mB = new (mSurfPatB.addr())
        SurfacePattern(ssDual->mB, surfPat->mExtendMode, surfPat->mMatrix,
                       surfPat->mSamplingFilter);
||||||| merged common ancestors
    const SourceSurfaceDual *ssDual =
      static_cast<const SourceSurfaceDual*>(surfPat->mSurface.get());
    mA = new (mSurfPatA.addr()) SurfacePattern(ssDual->mA, surfPat->mExtendMode,
                                               surfPat->mMatrix,
                                               surfPat->mSamplingFilter);
    mB = new (mSurfPatB.addr()) SurfacePattern(ssDual->mB, surfPat->mExtendMode,
                                               surfPat->mMatrix,
                                               surfPat->mSamplingFilter);
=======
    const SourceSurfaceDual* ssDual =
        static_cast<const SourceSurfaceDual*>(surfPat->mSurface.get());
    mA = new (mSurfPatA.addr())
        SurfacePattern(ssDual->mA, surfPat->mExtendMode, surfPat->mMatrix,
                       surfPat->mSamplingFilter);
    mB = new (mSurfPatB.addr())
        SurfacePattern(ssDual->mB, surfPat->mExtendMode, surfPat->mMatrix,
                       surfPat->mSamplingFilter);
>>>>>>> upstream-releases
    mPatternsInitialized = true;
  }

  inline ~DualPattern() {
    if (mPatternsInitialized) {
      mA->~Pattern();
      mB->~Pattern();
    }
  }

  ClassStorage<SurfacePattern> mSurfPatA;
  ClassStorage<SurfacePattern> mSurfPatB;

  const Pattern* mA;
  const Pattern* mB;

  bool mPatternsInitialized;
};

void DrawTargetDual::DetachAllSnapshots() {
  mA->DetachAllSnapshots();
  mB->DetachAllSnapshots();
}

<<<<<<< HEAD
void DrawTargetDual::DrawSurface(SourceSurface *aSurface, const Rect &aDest,
                                 const Rect &aSource,
                                 const DrawSurfaceOptions &aSurfOptions,
                                 const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetDual::DrawSurface(SourceSurface *aSurface, const Rect &aDest, const Rect &aSource,
                            const DrawSurfaceOptions &aSurfOptions, const DrawOptions &aOptions)
{
=======
void DrawTargetDual::DrawSurface(SourceSurface* aSurface, const Rect& aDest,
                                 const Rect& aSource,
                                 const DrawSurfaceOptions& aSurfOptions,
                                 const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  DualSurface surface(aSurface);
  mA->DrawSurface(surface.mA, aDest, aSource, aSurfOptions, aOptions);
  mB->DrawSurface(surface.mB, aDest, aSource, aSurfOptions, aOptions);
}

<<<<<<< HEAD
void DrawTargetDual::DrawSurfaceWithShadow(SourceSurface *aSurface,
                                           const Point &aDest,
                                           const Color &aColor,
                                           const Point &aOffset, Float aSigma,
                                           CompositionOp aOp) {
||||||| merged common ancestors
void
DrawTargetDual::DrawSurfaceWithShadow(SourceSurface *aSurface, const Point &aDest,
                                      const Color &aColor, const Point &aOffset,
                                      Float aSigma, CompositionOp aOp)
{
=======
void DrawTargetDual::DrawSurfaceWithShadow(SourceSurface* aSurface,
                                           const Point& aDest,
                                           const Color& aColor,
                                           const Point& aOffset, Float aSigma,
                                           CompositionOp aOp) {
>>>>>>> upstream-releases
  DualSurface surface(aSurface);
  mA->DrawSurfaceWithShadow(surface.mA, aDest, aColor, aOffset, aSigma, aOp);
  mB->DrawSurfaceWithShadow(surface.mB, aDest, aColor, aOffset, aSigma, aOp);
}

<<<<<<< HEAD
void DrawTargetDual::MaskSurface(const Pattern &aSource, SourceSurface *aMask,
                                 Point aOffset, const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetDual::MaskSurface(const Pattern &aSource,
                           SourceSurface *aMask,
                           Point aOffset,
                           const DrawOptions &aOptions)
{
=======
void DrawTargetDual::MaskSurface(const Pattern& aSource, SourceSurface* aMask,
                                 Point aOffset, const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  DualPattern source(aSource);
  DualSurface mask(aMask);
  mA->MaskSurface(*source.mA, mask.mA, aOffset, aOptions);
  mB->MaskSurface(*source.mB, mask.mB, aOffset, aOptions);
}

<<<<<<< HEAD
void DrawTargetDual::ClearRect(const Rect &aRect) {
||||||| merged common ancestors
void
DrawTargetDual::ClearRect(const Rect &aRect)
{
=======
void DrawTargetDual::ClearRect(const Rect& aRect) {
>>>>>>> upstream-releases
  mA->FillRect(aRect, ColorPattern(Color(0.0, 0.0, 0.0, 1.0)));
  mB->FillRect(aRect, ColorPattern(Color(1.0, 1.0, 1.0, 1.0)));
}

<<<<<<< HEAD
void DrawTargetDual::CopySurface(SourceSurface *aSurface,
                                 const IntRect &aSourceRect,
                                 const IntPoint &aDestination) {
||||||| merged common ancestors
void
DrawTargetDual::CopySurface(SourceSurface *aSurface, const IntRect &aSourceRect,
                            const IntPoint &aDestination)
{
=======
void DrawTargetDual::CopySurface(SourceSurface* aSurface,
                                 const IntRect& aSourceRect,
                                 const IntPoint& aDestination) {
>>>>>>> upstream-releases
  DualSurface surface(aSurface);
  mA->CopySurface(surface.mA, aSourceRect, aDestination);
  mB->CopySurface(surface.mB, aSourceRect, aDestination);
}

<<<<<<< HEAD
void DrawTargetDual::FillRect(const Rect &aRect, const Pattern &aPattern,
                              const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetDual::FillRect(const Rect &aRect, const Pattern &aPattern, const DrawOptions &aOptions)
{
=======
void DrawTargetDual::FillRect(const Rect& aRect, const Pattern& aPattern,
                              const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  DualPattern pattern(aPattern);
  mA->FillRect(aRect, *pattern.mA, aOptions);
  mB->FillRect(aRect, *pattern.mB, aOptions);
}

<<<<<<< HEAD
void DrawTargetDual::StrokeRect(const Rect &aRect, const Pattern &aPattern,
                                const StrokeOptions &aStrokeOptions,
                                const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetDual::StrokeRect(const Rect &aRect, const Pattern &aPattern,
                           const StrokeOptions &aStrokeOptions, const DrawOptions &aOptions)
{
=======
void DrawTargetDual::StrokeRect(const Rect& aRect, const Pattern& aPattern,
                                const StrokeOptions& aStrokeOptions,
                                const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  DualPattern pattern(aPattern);
  mA->StrokeRect(aRect, *pattern.mA, aStrokeOptions, aOptions);
  mB->StrokeRect(aRect, *pattern.mB, aStrokeOptions, aOptions);
}

<<<<<<< HEAD
void DrawTargetDual::StrokeLine(const Point &aStart, const Point &aEnd,
                                const Pattern &aPattern,
                                const StrokeOptions &aStrokeOptions,
                                const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetDual::StrokeLine(const Point &aStart, const Point &aEnd, const Pattern &aPattern,
                           const StrokeOptions &aStrokeOptions, const DrawOptions &aOptions)
{
=======
void DrawTargetDual::StrokeLine(const Point& aStart, const Point& aEnd,
                                const Pattern& aPattern,
                                const StrokeOptions& aStrokeOptions,
                                const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  DualPattern pattern(aPattern);
  mA->StrokeLine(aStart, aEnd, *pattern.mA, aStrokeOptions, aOptions);
  mB->StrokeLine(aStart, aEnd, *pattern.mB, aStrokeOptions, aOptions);
}

<<<<<<< HEAD
void DrawTargetDual::Stroke(const Path *aPath, const Pattern &aPattern,
                            const StrokeOptions &aStrokeOptions,
                            const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetDual::Stroke(const Path *aPath, const Pattern &aPattern,
                       const StrokeOptions &aStrokeOptions, const DrawOptions &aOptions)
{
=======
void DrawTargetDual::Stroke(const Path* aPath, const Pattern& aPattern,
                            const StrokeOptions& aStrokeOptions,
                            const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  DualPattern pattern(aPattern);
  mA->Stroke(aPath, *pattern.mA, aStrokeOptions, aOptions);
  mB->Stroke(aPath, *pattern.mB, aStrokeOptions, aOptions);
}

<<<<<<< HEAD
void DrawTargetDual::Fill(const Path *aPath, const Pattern &aPattern,
                          const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetDual::Fill(const Path *aPath, const Pattern &aPattern, const DrawOptions &aOptions)
{
=======
void DrawTargetDual::Fill(const Path* aPath, const Pattern& aPattern,
                          const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  DualPattern pattern(aPattern);
  mA->Fill(aPath, *pattern.mA, aOptions);
  mB->Fill(aPath, *pattern.mB, aOptions);
}

<<<<<<< HEAD
void DrawTargetDual::FillGlyphs(ScaledFont *aScaledFont,
                                const GlyphBuffer &aBuffer,
                                const Pattern &aPattern,
                                const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetDual::FillGlyphs(ScaledFont *aScaledFont, const GlyphBuffer &aBuffer,
                           const Pattern &aPattern, const DrawOptions &aOptions)
{
=======
void DrawTargetDual::FillGlyphs(ScaledFont* aScaledFont,
                                const GlyphBuffer& aBuffer,
                                const Pattern& aPattern,
                                const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  DualPattern pattern(aPattern);
  mA->FillGlyphs(aScaledFont, aBuffer, *pattern.mA, aOptions);
  mB->FillGlyphs(aScaledFont, aBuffer, *pattern.mB, aOptions);
}

<<<<<<< HEAD
void DrawTargetDual::Mask(const Pattern &aSource, const Pattern &aMask,
                          const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetDual::Mask(const Pattern &aSource, const Pattern &aMask, const DrawOptions &aOptions)
{
=======
void DrawTargetDual::Mask(const Pattern& aSource, const Pattern& aMask,
                          const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  DualPattern source(aSource);
  DualPattern mask(aMask);
  mA->Mask(*source.mA, *mask.mA, aOptions);
  mB->Mask(*source.mB, *mask.mB, aOptions);
}

<<<<<<< HEAD
void DrawTargetDual::PushLayer(bool aOpaque, Float aOpacity,
                               SourceSurface *aMask,
                               const Matrix &aMaskTransform,
                               const IntRect &aBounds, bool aCopyBackground) {
||||||| merged common ancestors
void
DrawTargetDual::PushLayer(bool aOpaque, Float aOpacity, SourceSurface* aMask,
                          const Matrix& aMaskTransform, const IntRect& aBounds,
                          bool aCopyBackground)
{
=======
void DrawTargetDual::PushLayer(bool aOpaque, Float aOpacity,
                               SourceSurface* aMask,
                               const Matrix& aMaskTransform,
                               const IntRect& aBounds, bool aCopyBackground) {
>>>>>>> upstream-releases
  DualSurface mask(aMask);
  mA->PushLayer(aOpaque, aOpacity, mask.mA, aMaskTransform, aBounds,
                aCopyBackground);
  mB->PushLayer(aOpaque, aOpacity, mask.mB, aMaskTransform, aBounds,
                aCopyBackground);
}

<<<<<<< HEAD
already_AddRefed<DrawTarget> DrawTargetDual::CreateSimilarDrawTarget(
    const IntSize &aSize, SurfaceFormat aFormat) const {
  /* Now that we have PushLayer there a very few cases where a user of
   * DrawTargetDual wants to have a DualTarget when creating a similar one. */
||||||| merged common ancestors
already_AddRefed<DrawTarget>
DrawTargetDual::CreateSimilarDrawTarget(const IntSize &aSize, SurfaceFormat aFormat) const
{
  /* Now that we have PushLayer there a very few cases where a user of DrawTargetDual
   * wants to have a DualTarget when creating a similar one. */
=======
already_AddRefed<DrawTarget> DrawTargetDual::CreateSimilarDrawTarget(
    const IntSize& aSize, SurfaceFormat aFormat) const {
  /* Now that we have PushLayer there a very few cases where a user of
   * DrawTargetDual wants to have a DualTarget when creating a similar one. */
>>>>>>> upstream-releases
  return mA->CreateSimilarDrawTarget(aSize, aFormat);
}

<<<<<<< HEAD
bool DrawTargetDual::CanCreateSimilarDrawTarget(const IntSize &aSize,
                                                SurfaceFormat aFormat) const {
  return mA->CanCreateSimilarDrawTarget(aSize, aFormat);
}

}  // namespace gfx
}  // namespace mozilla
||||||| merged common ancestors
} // namespace gfx
} // namespace mozilla
=======
RefPtr<DrawTarget> DrawTargetDual::CreateClippedDrawTarget(
    const Rect& aBounds, SurfaceFormat aFormat) {
  /* The user probably doesn't want a DualDrawTarget here. */
  return mA->CreateClippedDrawTarget(aBounds, aFormat);
}

bool DrawTargetDual::CanCreateSimilarDrawTarget(const IntSize& aSize,
                                                SurfaceFormat aFormat) const {
  return mA->CanCreateSimilarDrawTarget(aSize, aFormat);
}

}  // namespace gfx
}  // namespace mozilla
>>>>>>> upstream-releases
