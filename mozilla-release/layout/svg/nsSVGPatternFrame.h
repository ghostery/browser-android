/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __NS_SVGPATTERNFRAME_H__
#define __NS_SVGPATTERNFRAME_H__

#include "mozilla/Attributes.h"
#include "gfxMatrix.h"
#include "mozilla/gfx/2D.h"
#include "mozilla/RefPtr.h"
#include "nsAutoPtr.h"
#include "nsSVGPaintServerFrame.h"

class nsIFrame;

namespace mozilla {
class PresShell;
class SVGAnimatedLength;
class SVGAnimatedPreserveAspectRatio;
class SVGAnimatedTransformList;
class SVGAnimatedViewBox;
class SVGGeometryFrame;
<<<<<<< HEAD
class nsSVGAnimatedTransformList;
}  // namespace mozilla
||||||| merged common ancestors
class nsSVGAnimatedTransformList;
} // namespace mozilla
=======
}  // namespace mozilla
>>>>>>> upstream-releases

class nsSVGPatternFrame final : public nsSVGPaintServerFrame {
  typedef mozilla::gfx::SourceSurface SourceSurface;

 public:
  NS_DECL_FRAMEARENA_HELPERS(nsSVGPatternFrame)

  friend nsIFrame* NS_NewSVGPatternFrame(mozilla::PresShell* aPresShell,
                                         ComputedStyle* aStyle);

  explicit nsSVGPatternFrame(ComputedStyle* aStyle,
                             nsPresContext* aPresContext);

  // nsSVGPaintServerFrame methods:
<<<<<<< HEAD
  virtual already_AddRefed<gfxPattern> GetPaintServerPattern(
      nsIFrame* aSource, const DrawTarget* aDrawTarget,
      const gfxMatrix& aContextMatrix,
      nsStyleSVGPaint nsStyleSVG::*aFillOrStroke, float aOpacity,
      imgDrawingParams& aImgParams, const gfxRect* aOverrideBounds) override;

 public:
  typedef mozilla::SVGAnimatedPreserveAspectRatio
      SVGAnimatedPreserveAspectRatio;
||||||| merged common ancestors
  virtual already_AddRefed<gfxPattern>
    GetPaintServerPattern(nsIFrame *aSource,
                          const DrawTarget* aDrawTarget,
                          const gfxMatrix& aContextMatrix,
                          nsStyleSVGPaint nsStyleSVG::*aFillOrStroke,
                          float aOpacity,
                          imgDrawingParams& aImgParams,
                          const gfxRect* aOverrideBounds) override;

public:
  typedef mozilla::SVGAnimatedPreserveAspectRatio SVGAnimatedPreserveAspectRatio;
=======
  virtual already_AddRefed<gfxPattern> GetPaintServerPattern(
      nsIFrame* aSource, const DrawTarget* aDrawTarget,
      const gfxMatrix& aContextMatrix,
      mozilla::StyleSVGPaint nsStyleSVG::*aFillOrStroke, float aGraphicOpacity,
      imgDrawingParams& aImgParams, const gfxRect* aOverrideBounds) override;

 public:
  typedef mozilla::SVGAnimatedPreserveAspectRatio
      SVGAnimatedPreserveAspectRatio;
>>>>>>> upstream-releases

  // nsSVGContainerFrame methods:
  virtual gfxMatrix GetCanvasTM() override;

  // nsIFrame interface:
  virtual nsresult AttributeChanged(int32_t aNameSpaceID, nsAtom* aAttribute,
                                    int32_t aModType) override;

#ifdef DEBUG
  virtual void Init(nsIContent* aContent, nsContainerFrame* aParent,
                    nsIFrame* aPrevInFlow) override;
#endif

#ifdef DEBUG_FRAME_DUMP
  virtual nsresult GetFrameName(nsAString& aResult) const override {
    return MakeFrameName(NS_LITERAL_STRING("SVGPattern"), aResult);
  }
#endif  // DEBUG

 protected:
  /**
   * Parses this frame's href and - if it references another pattern - returns
   * it.  It also makes this frame a rendering observer of the specified ID.
   */
  nsSVGPatternFrame* GetReferencedPattern();

  // Accessors to lookup pattern attributes
  uint16_t GetEnumValue(uint32_t aIndex, nsIContent* aDefault);
  uint16_t GetEnumValue(uint32_t aIndex) {
    return GetEnumValue(aIndex, mContent);
  }
  mozilla::SVGAnimatedTransformList* GetPatternTransformList(
      nsIContent* aDefault);
  gfxMatrix GetPatternTransform();
<<<<<<< HEAD
  const nsSVGViewBox& GetViewBox(nsIContent* aDefault);
  const nsSVGViewBox& GetViewBox() { return GetViewBox(mContent); }
  const SVGAnimatedPreserveAspectRatio& GetPreserveAspectRatio(
      nsIContent* aDefault);
  const SVGAnimatedPreserveAspectRatio& GetPreserveAspectRatio() {
||||||| merged common ancestors
  const nsSVGViewBox &GetViewBox(nsIContent *aDefault);
  const nsSVGViewBox &GetViewBox() { return GetViewBox(mContent); }
  const SVGAnimatedPreserveAspectRatio &GetPreserveAspectRatio(
      nsIContent *aDefault);
  const SVGAnimatedPreserveAspectRatio &GetPreserveAspectRatio()
  {
=======
  const SVGAnimatedViewBox& GetViewBox(nsIContent* aDefault);
  const SVGAnimatedViewBox& GetViewBox() { return GetViewBox(mContent); }
  const SVGAnimatedPreserveAspectRatio& GetPreserveAspectRatio(
      nsIContent* aDefault);
  const SVGAnimatedPreserveAspectRatio& GetPreserveAspectRatio() {
>>>>>>> upstream-releases
    return GetPreserveAspectRatio(mContent);
  }
<<<<<<< HEAD
  const nsSVGLength2* GetLengthValue(uint32_t aIndex, nsIContent* aDefault);
  const nsSVGLength2* GetLengthValue(uint32_t aIndex) {
||||||| merged common ancestors
  const nsSVGLength2 *GetLengthValue(uint32_t aIndex, nsIContent *aDefault);
  const nsSVGLength2 *GetLengthValue(uint32_t aIndex)
  {
=======
  const SVGAnimatedLength* GetLengthValue(uint32_t aIndex,
                                          nsIContent* aDefault);
  const SVGAnimatedLength* GetLengthValue(uint32_t aIndex) {
>>>>>>> upstream-releases
    return GetLengthValue(aIndex, mContent);
  }

<<<<<<< HEAD
  already_AddRefed<SourceSurface> PaintPattern(
      const DrawTarget* aDrawTarget, Matrix* patternMatrix,
      const Matrix& aContextMatrix, nsIFrame* aSource,
      nsStyleSVGPaint nsStyleSVG::*aFillOrStroke, float aGraphicOpacity,
      const gfxRect* aOverrideBounds, imgDrawingParams& aImgParams);
||||||| merged common ancestors
  already_AddRefed<SourceSurface>
  PaintPattern(const DrawTarget* aDrawTarget,
               Matrix *patternMatrix,
               const Matrix &aContextMatrix,
               nsIFrame *aSource,
               nsStyleSVGPaint nsStyleSVG::*aFillOrStroke,
               float aGraphicOpacity,
               const gfxRect *aOverrideBounds,
               imgDrawingParams& aImgParams);
=======
  already_AddRefed<SourceSurface> PaintPattern(
      const DrawTarget* aDrawTarget, Matrix* patternMatrix,
      const Matrix& aContextMatrix, nsIFrame* aSource,
      mozilla::StyleSVGPaint nsStyleSVG::*aFillOrStroke, float aGraphicOpacity,
      const gfxRect* aOverrideBounds, imgDrawingParams& aImgParams);
>>>>>>> upstream-releases

  /**
   * A <pattern> element may reference another <pattern> element using
   * xlink:href and, if it doesn't have any child content of its own, then it
   * will "inherit" the children of the referenced pattern (which may itself be
   * inheriting its children if it references another <pattern>).  This
   * function returns this nsSVGPatternFrame or the first pattern along the
   * reference chain (if there is one) to have children.
   */
  nsSVGPatternFrame* GetPatternWithChildren();

<<<<<<< HEAD
  gfxRect GetPatternRect(uint16_t aPatternUnits, const gfxRect& bbox,
                         const Matrix& callerCTM, nsIFrame* aTarget);
  gfxMatrix ConstructCTM(const nsSVGViewBox& aViewBox,
                         uint16_t aPatternContentUnits, uint16_t aPatternUnits,
                         const gfxRect& callerBBox, const Matrix& callerCTM,
                         nsIFrame* aTarget);

 private:
||||||| merged common ancestors
  gfxRect    GetPatternRect(uint16_t aPatternUnits,
                            const gfxRect &bbox,
                            const Matrix &callerCTM,
                            nsIFrame *aTarget);
  gfxMatrix  ConstructCTM(const nsSVGViewBox& aViewBox,
                          uint16_t aPatternContentUnits,
                          uint16_t aPatternUnits,
                          const gfxRect &callerBBox,
                          const Matrix &callerCTM,
                          nsIFrame *aTarget);

private:
=======
  gfxRect GetPatternRect(uint16_t aPatternUnits, const gfxRect& bbox,
                         const Matrix& aTargetCTM, nsIFrame* aTarget);
  gfxMatrix ConstructCTM(const SVGAnimatedViewBox& aViewBox,
                         uint16_t aPatternContentUnits, uint16_t aPatternUnits,
                         const gfxRect& callerBBox, const Matrix& callerCTM,
                         nsIFrame* aTarget);

 private:
>>>>>>> upstream-releases
  // this is a *temporary* reference to the frame of the element currently
  // referencing our pattern.  This must be temporary because different
  // referencing frames will all reference this one frame
  mozilla::SVGGeometryFrame* mSource;
  nsAutoPtr<gfxMatrix> mCTM;

 protected:
  // This flag is used to detect loops in xlink:href processing
  bool mLoopFlag;
  bool mNoHRefURI;
};

#endif
