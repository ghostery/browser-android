/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_DRAWTARGETTILED_H_
#define MOZILLA_GFX_DRAWTARGETTILED_H_

#include "2D.h"

#include "mozilla/Vector.h"

#include "Filters.h"
#include "Logging.h"

#include <vector>

namespace mozilla {
namespace gfx {

struct TileInternal : public Tile {
  TileInternal() : mClippedOut(false) {}

<<<<<<< HEAD
  explicit TileInternal(const Tile &aOther)
      : Tile(aOther), mClippedOut(false) {}
||||||| merged common ancestors
  explicit TileInternal(const Tile& aOther)
    : Tile(aOther)
    , mClippedOut(false)
  {}
=======
  explicit TileInternal(const Tile& aOther)
      : Tile(aOther), mClippedOut(false) {}
>>>>>>> upstream-releases

  bool mClippedOut;
};

class DrawTargetTiled : public DrawTarget {
 public:
  DrawTargetTiled();

  bool Init(const TileSet &mTiles);

  bool IsTiledDrawTarget() const override { return true; }

<<<<<<< HEAD
  virtual bool IsCaptureDT() const override {
    return mTiles[0].mDrawTarget->IsCaptureDT();
  }
  virtual DrawTargetType GetType() const override {
    return mTiles[0].mDrawTarget->GetType();
  }
  virtual BackendType GetBackendType() const override {
    return mTiles[0].mDrawTarget->GetBackendType();
  }
  virtual already_AddRefed<SourceSurface> Snapshot() override;
  virtual void DetachAllSnapshots() override;
  virtual IntSize GetSize() const override {
||||||| merged common ancestors
  virtual bool IsCaptureDT() const override { return mTiles[0].mDrawTarget->IsCaptureDT(); }
  virtual DrawTargetType GetType() const override { return mTiles[0].mDrawTarget->GetType(); }
  virtual BackendType GetBackendType() const override { return mTiles[0].mDrawTarget->GetBackendType(); }
  virtual already_AddRefed<SourceSurface> Snapshot() override;
  virtual void DetachAllSnapshots() override;
  virtual IntSize GetSize() const override {
=======
  bool IsCaptureDT() const override {
    return mTiles[0].mDrawTarget->IsCaptureDT();
  }
  DrawTargetType GetType() const override {
    return mTiles[0].mDrawTarget->GetType();
  }
  BackendType GetBackendType() const override {
    return mTiles[0].mDrawTarget->GetBackendType();
  }
  already_AddRefed<SourceSurface> Snapshot() override;
  void DetachAllSnapshots() override;
  IntSize GetSize() const override {
>>>>>>> upstream-releases
    MOZ_ASSERT(mRect.Width() > 0 && mRect.Height() > 0);
    return IntSize(mRect.XMost(), mRect.YMost());
  }
<<<<<<< HEAD
  virtual IntRect GetRect() const override { return mRect; }
||||||| merged common ancestors
  virtual IntRect GetRect() const override {
    return mRect;
  }
=======
  IntRect GetRect() const override { return mRect; }

  void Flush() override;
  void DrawSurface(SourceSurface* aSurface, const Rect& aDest,
                   const Rect& aSource, const DrawSurfaceOptions& aSurfOptions,
                   const DrawOptions& aOptions) override;
  void DrawFilter(FilterNode* aNode, const Rect& aSourceRect,
                  const Point& aDestPoint,
                  const DrawOptions& aOptions = DrawOptions()) override;
  void DrawSurfaceWithShadow(
      SourceSurface* aSurface, const Point& aDest, const Color& aColor,
      const Point& aOffset, Float aSigma,
      CompositionOp aOperator) override { /* Not implemented */
    MOZ_CRASH("GFX: DrawSurfaceWithShadow");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void Flush() override;
  virtual void DrawSurface(SourceSurface *aSurface, const Rect &aDest,
                           const Rect &aSource,
                           const DrawSurfaceOptions &aSurfOptions,
                           const DrawOptions &aOptions) override;
  virtual void DrawFilter(FilterNode *aNode, const Rect &aSourceRect,
                          const Point &aDestPoint,
                          const DrawOptions &aOptions = DrawOptions()) override;
  virtual void DrawSurfaceWithShadow(
      SourceSurface *aSurface, const Point &aDest, const Color &aColor,
      const Point &aOffset, Float aSigma,
      CompositionOp aOperator) override { /* Not implemented */
    MOZ_CRASH("GFX: DrawSurfaceWithShadow");
  }

  virtual void ClearRect(const Rect &aRect) override;
  virtual void MaskSurface(
      const Pattern &aSource, SourceSurface *aMask, Point aOffset,
      const DrawOptions &aOptions = DrawOptions()) override;

  virtual void CopySurface(SourceSurface *aSurface, const IntRect &aSourceRect,
                           const IntPoint &aDestination) override;

  virtual void FillRect(const Rect &aRect, const Pattern &aPattern,
                        const DrawOptions &aOptions = DrawOptions()) override;
  virtual void StrokeRect(const Rect &aRect, const Pattern &aPattern,
                          const StrokeOptions &aStrokeOptions = StrokeOptions(),
                          const DrawOptions &aOptions = DrawOptions()) override;
  virtual void StrokeLine(const Point &aStart, const Point &aEnd,
                          const Pattern &aPattern,
                          const StrokeOptions &aStrokeOptions = StrokeOptions(),
                          const DrawOptions &aOptions = DrawOptions()) override;
  virtual void Stroke(const Path *aPath, const Pattern &aPattern,
                      const StrokeOptions &aStrokeOptions = StrokeOptions(),
                      const DrawOptions &aOptions = DrawOptions()) override;
  virtual void Fill(const Path *aPath, const Pattern &aPattern,
                    const DrawOptions &aOptions = DrawOptions()) override;
  virtual void FillGlyphs(ScaledFont *aFont, const GlyphBuffer &aBuffer,
                          const Pattern &aPattern,
                          const DrawOptions &aOptions = DrawOptions()) override;
  virtual void Mask(const Pattern &aSource, const Pattern &aMask,
                    const DrawOptions &aOptions = DrawOptions()) override;
  virtual void PushClip(const Path *aPath) override;
  virtual void PushClipRect(const Rect &aRect) override;
  virtual void PopClip() override;
  virtual void PushLayer(bool aOpaque, Float aOpacity, SourceSurface *aMask,
                         const Matrix &aMaskTransform,
                         const IntRect &aBounds = IntRect(),
                         bool aCopyBackground = false) override;
  virtual void PushLayerWithBlend(
      bool aOpaque, Float aOpacity, SourceSurface *aMask,
      const Matrix &aMaskTransform, const IntRect &aBounds = IntRect(),
      bool aCopyBackground = false,
      CompositionOp = CompositionOp::OP_OVER) override;
  virtual void PopLayer() override;

  virtual void PadEdges(const IntRegion &aRegion) override;

  virtual void SetTransform(const Matrix &aTransform) override;

  virtual void SetPermitSubpixelAA(bool aPermitSubpixelAA) override;

  virtual already_AddRefed<SourceSurface> CreateSourceSurfaceFromData(
      unsigned char *aData, const IntSize &aSize, int32_t aStride,
      SurfaceFormat aFormat) const override {
    return mTiles[0].mDrawTarget->CreateSourceSurfaceFromData(aData, aSize,
                                                              aStride, aFormat);
||||||| merged common ancestors
  virtual void Flush() override;
  virtual void DrawSurface(SourceSurface *aSurface,
                           const Rect &aDest,
                           const Rect &aSource,
                           const DrawSurfaceOptions &aSurfOptions,
                           const DrawOptions &aOptions) override;
  virtual void DrawFilter(FilterNode *aNode,
                          const Rect &aSourceRect,
                          const Point &aDestPoint,
                          const DrawOptions &aOptions = DrawOptions()) override;
  virtual void DrawSurfaceWithShadow(SourceSurface *aSurface,
                                     const Point &aDest,
                                     const Color &aColor,
                                     const Point &aOffset,
                                     Float aSigma,
                                     CompositionOp aOperator) override { /* Not implemented */ MOZ_CRASH("GFX: DrawSurfaceWithShadow"); }

  virtual void ClearRect(const Rect &aRect) override;
  virtual void MaskSurface(const Pattern &aSource,
                           SourceSurface *aMask,
                           Point aOffset,
                           const DrawOptions &aOptions = DrawOptions()) override;

  virtual void CopySurface(SourceSurface *aSurface,
                           const IntRect &aSourceRect,
                           const IntPoint &aDestination) override;

  virtual void FillRect(const Rect &aRect,
                        const Pattern &aPattern,
                        const DrawOptions &aOptions = DrawOptions()) override;
  virtual void StrokeRect(const Rect &aRect,
                          const Pattern &aPattern,
                          const StrokeOptions &aStrokeOptions = StrokeOptions(),
                          const DrawOptions &aOptions = DrawOptions()) override;
  virtual void StrokeLine(const Point &aStart,
                          const Point &aEnd,
                          const Pattern &aPattern,
                          const StrokeOptions &aStrokeOptions = StrokeOptions(),
                          const DrawOptions &aOptions = DrawOptions()) override;
  virtual void Stroke(const Path *aPath,
                      const Pattern &aPattern,
                      const StrokeOptions &aStrokeOptions = StrokeOptions(),
                      const DrawOptions &aOptions = DrawOptions()) override;
  virtual void Fill(const Path *aPath,
                    const Pattern &aPattern,
                    const DrawOptions &aOptions = DrawOptions()) override;
  virtual void FillGlyphs(ScaledFont *aFont,
                          const GlyphBuffer &aBuffer,
                          const Pattern &aPattern,
                          const DrawOptions &aOptions = DrawOptions()) override;
  virtual void Mask(const Pattern &aSource,
                    const Pattern &aMask,
                    const DrawOptions &aOptions = DrawOptions()) override;
  virtual void PushClip(const Path *aPath) override;
  virtual void PushClipRect(const Rect &aRect) override;
  virtual void PopClip() override;
  virtual void PushLayer(bool aOpaque, Float aOpacity,
                         SourceSurface* aMask,
                         const Matrix& aMaskTransform,
                         const IntRect& aBounds = IntRect(),
                         bool aCopyBackground = false) override;
  virtual void PushLayerWithBlend(bool aOpaque, Float aOpacity,
                         SourceSurface* aMask,
                         const Matrix& aMaskTransform,
                         const IntRect& aBounds = IntRect(),
                         bool aCopyBackground = false,
                         CompositionOp = CompositionOp::OP_OVER) override;
  virtual void PopLayer() override;

  virtual void PadEdges(const IntRegion& aRegion) override;

  virtual void SetTransform(const Matrix &aTransform) override;

  virtual void SetPermitSubpixelAA(bool aPermitSubpixelAA) override;

  virtual already_AddRefed<SourceSurface> CreateSourceSurfaceFromData(unsigned char *aData,
                                                                  const IntSize &aSize,
                                                                  int32_t aStride,
                                                                  SurfaceFormat aFormat) const override
  {
    return mTiles[0].mDrawTarget->CreateSourceSurfaceFromData(aData, aSize, aStride, aFormat);
=======
  void ClearRect(const Rect& aRect) override;
  void MaskSurface(const Pattern& aSource, SourceSurface* aMask, Point aOffset,
                   const DrawOptions& aOptions = DrawOptions()) override;

  void CopySurface(SourceSurface* aSurface, const IntRect& aSourceRect,
                   const IntPoint& aDestination) override;

  void FillRect(const Rect& aRect, const Pattern& aPattern,
                const DrawOptions& aOptions = DrawOptions()) override;
  void StrokeRect(const Rect& aRect, const Pattern& aPattern,
                  const StrokeOptions& aStrokeOptions = StrokeOptions(),
                  const DrawOptions& aOptions = DrawOptions()) override;
  void StrokeLine(const Point& aStart, const Point& aEnd,
                  const Pattern& aPattern,
                  const StrokeOptions& aStrokeOptions = StrokeOptions(),
                  const DrawOptions& aOptions = DrawOptions()) override;
  void Stroke(const Path* aPath, const Pattern& aPattern,
              const StrokeOptions& aStrokeOptions = StrokeOptions(),
              const DrawOptions& aOptions = DrawOptions()) override;
  void Fill(const Path* aPath, const Pattern& aPattern,
            const DrawOptions& aOptions = DrawOptions()) override;
  void FillGlyphs(ScaledFont* aFont, const GlyphBuffer& aBuffer,
                  const Pattern& aPattern,
                  const DrawOptions& aOptions = DrawOptions()) override;
  void Mask(const Pattern& aSource, const Pattern& aMask,
            const DrawOptions& aOptions = DrawOptions()) override;
  void PushClip(const Path* aPath) override;
  void PushClipRect(const Rect& aRect) override;
  void PopClip() override;
  void PushLayer(bool aOpaque, Float aOpacity, SourceSurface* aMask,
                 const Matrix& aMaskTransform,
                 const IntRect& aBounds = IntRect(),
                 bool aCopyBackground = false) override;
  void PushLayerWithBlend(bool aOpaque, Float aOpacity, SourceSurface* aMask,
                          const Matrix& aMaskTransform,
                          const IntRect& aBounds = IntRect(),
                          bool aCopyBackground = false,
                          CompositionOp = CompositionOp::OP_OVER) override;
  void PopLayer() override;

  void PadEdges(const IntRegion& aRegion) override;

  void SetTransform(const Matrix& aTransform) override;

  void SetPermitSubpixelAA(bool aPermitSubpixelAA) override;

  already_AddRefed<SourceSurface> CreateSourceSurfaceFromData(
      unsigned char* aData, const IntSize& aSize, int32_t aStride,
      SurfaceFormat aFormat) const override {
    return mTiles[0].mDrawTarget->CreateSourceSurfaceFromData(aData, aSize,
                                                              aStride, aFormat);
>>>>>>> upstream-releases
  }
<<<<<<< HEAD
  virtual already_AddRefed<SourceSurface> OptimizeSourceSurface(
      SourceSurface *aSurface) const override {
||||||| merged common ancestors
  virtual already_AddRefed<SourceSurface> OptimizeSourceSurface(SourceSurface *aSurface) const override
  {
=======
  already_AddRefed<SourceSurface> OptimizeSourceSurface(
      SourceSurface* aSurface) const override {
>>>>>>> upstream-releases
    return mTiles[0].mDrawTarget->OptimizeSourceSurface(aSurface);
  }

<<<<<<< HEAD
  virtual already_AddRefed<SourceSurface> CreateSourceSurfaceFromNativeSurface(
      const NativeSurface &aSurface) const override {
    return mTiles[0].mDrawTarget->CreateSourceSurfaceFromNativeSurface(
        aSurface);
||||||| merged common ancestors
  virtual already_AddRefed<SourceSurface>
    CreateSourceSurfaceFromNativeSurface(const NativeSurface &aSurface) const override
  {
    return mTiles[0].mDrawTarget->CreateSourceSurfaceFromNativeSurface(aSurface);
=======
  already_AddRefed<SourceSurface> CreateSourceSurfaceFromNativeSurface(
      const NativeSurface& aSurface) const override {
    return mTiles[0].mDrawTarget->CreateSourceSurfaceFromNativeSurface(
        aSurface);
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  virtual already_AddRefed<DrawTarget> CreateSimilarDrawTarget(
      const IntSize &aSize, SurfaceFormat aFormat) const override {
||||||| merged common ancestors
  virtual already_AddRefed<DrawTarget>
    CreateSimilarDrawTarget(const IntSize &aSize, SurfaceFormat aFormat) const override
  {
=======
  already_AddRefed<DrawTarget> CreateSimilarDrawTarget(
      const IntSize& aSize, SurfaceFormat aFormat) const override {
>>>>>>> upstream-releases
    return mTiles[0].mDrawTarget->CreateSimilarDrawTarget(aSize, aFormat);
  }

<<<<<<< HEAD
  virtual bool CanCreateSimilarDrawTarget(
      const IntSize &aSize, SurfaceFormat aFormat) const override {
    return mTiles[0].mDrawTarget->CanCreateSimilarDrawTarget(aSize, aFormat);
  }

  virtual already_AddRefed<PathBuilder> CreatePathBuilder(
      FillRule aFillRule = FillRule::FILL_WINDING) const override {
||||||| merged common ancestors
  virtual already_AddRefed<PathBuilder> CreatePathBuilder(FillRule aFillRule = FillRule::FILL_WINDING) const override
  {
=======
  bool CanCreateSimilarDrawTarget(const IntSize& aSize,
                                  SurfaceFormat aFormat) const override {
    return mTiles[0].mDrawTarget->CanCreateSimilarDrawTarget(aSize, aFormat);
  }

  RefPtr<DrawTarget> CreateClippedDrawTarget(const Rect& aBounds,
                                             SurfaceFormat aFormat) override;

  already_AddRefed<PathBuilder> CreatePathBuilder(
      FillRule aFillRule = FillRule::FILL_WINDING) const override {
>>>>>>> upstream-releases
    return mTiles[0].mDrawTarget->CreatePathBuilder(aFillRule);
  }

<<<<<<< HEAD
  virtual already_AddRefed<GradientStops> CreateGradientStops(
      GradientStop *aStops, uint32_t aNumStops,
      ExtendMode aExtendMode = ExtendMode::CLAMP) const override {
    return mTiles[0].mDrawTarget->CreateGradientStops(aStops, aNumStops,
                                                      aExtendMode);
||||||| merged common ancestors
  virtual already_AddRefed<GradientStops>
    CreateGradientStops(GradientStop *aStops,
                        uint32_t aNumStops,
                        ExtendMode aExtendMode = ExtendMode::CLAMP) const override
  {
    return mTiles[0].mDrawTarget->CreateGradientStops(aStops, aNumStops, aExtendMode);
=======
  already_AddRefed<GradientStops> CreateGradientStops(
      GradientStop* aStops, uint32_t aNumStops,
      ExtendMode aExtendMode = ExtendMode::CLAMP) const override {
    return mTiles[0].mDrawTarget->CreateGradientStops(aStops, aNumStops,
                                                      aExtendMode);
>>>>>>> upstream-releases
  }
<<<<<<< HEAD
  virtual already_AddRefed<FilterNode> CreateFilter(FilterType aType) override {
||||||| merged common ancestors
  virtual already_AddRefed<FilterNode> CreateFilter(FilterType aType) override
  {
=======
  already_AddRefed<FilterNode> CreateFilter(FilterType aType) override {
>>>>>>> upstream-releases
    return mTiles[0].mDrawTarget->CreateFilter(aType);
  }

 private:
  std::vector<TileInternal> mTiles;

  // mClippedOutTilesStack[clipIndex][tileIndex] is true if the tile at
  // tileIndex has become completely clipped out at the clip stack depth
  // clipIndex.
  Vector<std::vector<bool>, 8> mClippedOutTilesStack;

  IntRect mRect;

  struct PushedLayer {
    explicit PushedLayer(bool aOldPermitSubpixelAA)
        : mOldPermitSubpixelAA(aOldPermitSubpixelAA) {}
    bool mOldPermitSubpixelAA;
  };
  std::vector<PushedLayer> mPushedLayers;
};

<<<<<<< HEAD
class SnapshotTiled : public SourceSurface {
 public:
  SnapshotTiled(const std::vector<TileInternal> &aTiles, const IntRect &aRect)
      : mRect(aRect) {
||||||| merged common ancestors
class SnapshotTiled : public SourceSurface
{
public:
  SnapshotTiled(const std::vector<TileInternal>& aTiles, const IntRect& aRect)
    : mRect(aRect)
  {
=======
class SnapshotTiled : public SourceSurface {
 public:
  SnapshotTiled(const std::vector<TileInternal>& aTiles, const IntRect& aRect)
      : mRect(aRect) {
>>>>>>> upstream-releases
    for (size_t i = 0; i < aTiles.size(); i++) {
      mSnapshots.push_back(aTiles[i].mDrawTarget->Snapshot());
      mOrigins.push_back(aTiles[i].mTileOrigin);
    }
  }

  SurfaceType GetType() const override { return SurfaceType::TILED; }
  IntSize GetSize() const override {
    MOZ_ASSERT(mRect.Width() > 0 && mRect.Height() > 0);
    return IntSize(mRect.XMost(), mRect.YMost());
  }
<<<<<<< HEAD
  virtual IntRect GetRect() const override { return mRect; }
  virtual SurfaceFormat GetFormat() const override {
    return mSnapshots[0]->GetFormat();
||||||| merged common ancestors
  virtual IntRect GetRect() const override {
    return mRect;
=======
  IntRect GetRect() const override { return mRect; }
  SurfaceFormat GetFormat() const override {
    return mSnapshots[0]->GetFormat();
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  virtual already_AddRefed<DataSourceSurface> GetDataSurface() override {
    RefPtr<DataSourceSurface> surf =
        Factory::CreateDataSourceSurface(mRect.Size(), GetFormat());
||||||| merged common ancestors
  virtual already_AddRefed<DataSourceSurface> GetDataSurface() override
  {
    RefPtr<DataSourceSurface> surf = Factory::CreateDataSourceSurface(mRect.Size(), GetFormat());
=======
  already_AddRefed<DataSourceSurface> GetDataSurface() override {
    RefPtr<DataSourceSurface> surf =
        Factory::CreateDataSourceSurface(mRect.Size(), GetFormat());
>>>>>>> upstream-releases
    if (!surf) {
      gfxCriticalError()
          << "DrawTargetTiled::GetDataSurface failed to allocate surface";
      return nullptr;
    }

    DataSourceSurface::MappedSurface mappedSurf;
    if (!surf->Map(DataSourceSurface::MapType::WRITE, &mappedSurf)) {
      gfxCriticalError()
          << "DrawTargetTiled::GetDataSurface failed to map surface";
      return nullptr;
    }

    {
      RefPtr<DrawTarget> dt = Factory::CreateDrawTargetForData(
          BackendType::CAIRO, mappedSurf.mData, mRect.Size(),
          mappedSurf.mStride, GetFormat());

      if (!dt) {
        gfxWarning() << "DrawTargetTiled::GetDataSurface failed in "
                        "CreateDrawTargetForData";
        surf->Unmap();
        return nullptr;
      }
      for (size_t i = 0; i < mSnapshots.size(); i++) {
        RefPtr<DataSourceSurface> dataSurf = mSnapshots[i]->GetDataSurface();
        dt->CopySurface(dataSurf,
                        IntRect(IntPoint(0, 0), mSnapshots[i]->GetSize()),
                        mOrigins[i] - mRect.TopLeft());
      }
    }
    surf->Unmap();

    return surf.forget();
  }

  std::vector<RefPtr<SourceSurface>> mSnapshots;
  std::vector<IntPoint> mOrigins;
  IntRect mRect;
};

}  // namespace gfx
}  // namespace mozilla

#endif
