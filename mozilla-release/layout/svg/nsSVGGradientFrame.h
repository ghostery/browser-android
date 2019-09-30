/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __NS_SVGGRADIENTFRAME_H__
#define __NS_SVGGRADIENTFRAME_H__

#include "mozilla/Attributes.h"
#include "gfxMatrix.h"
#include "gfxRect.h"
#include "nsCOMPtr.h"
#include "nsFrame.h"
#include "nsLiteralString.h"
#include "nsSVGPaintServerFrame.h"

class gfxPattern;
class nsAtom;
class nsIContent;

namespace mozilla {
class PresShell;
class SVGAnimatedTransformList;

namespace dom {
class SVGLinearGradientElement;
class SVGRadialGradientElement;
}  // namespace dom
}  // namespace mozilla

class nsSVGGradientFrame : public nsSVGPaintServerFrame {
  typedef mozilla::gfx::ExtendMode ExtendMode;

<<<<<<< HEAD
 protected:
  nsSVGGradientFrame(ComputedStyle* aStyle, ClassID aID);
||||||| merged common ancestors
protected:
  nsSVGGradientFrame(ComputedStyle* aStyle, ClassID aID);
=======
 protected:
  nsSVGGradientFrame(ComputedStyle* aStyle, nsPresContext* aPresContext,
                     ClassID aID);
>>>>>>> upstream-releases

 public:
  NS_DECL_ABSTRACT_FRAME(nsSVGGradientFrame)

  // nsSVGPaintServerFrame methods:
<<<<<<< HEAD
  virtual already_AddRefed<gfxPattern> GetPaintServerPattern(
      nsIFrame* aSource, const DrawTarget* aDrawTarget,
      const gfxMatrix& aContextMatrix,
      nsStyleSVGPaint nsStyleSVG::*aFillOrStroke, float aOpacity,
      imgDrawingParams& aImgParams, const gfxRect* aOverrideBounds) override;
||||||| merged common ancestors
  virtual already_AddRefed<gfxPattern>
    GetPaintServerPattern(nsIFrame *aSource,
                          const DrawTarget* aDrawTarget,
                          const gfxMatrix& aContextMatrix,
                          nsStyleSVGPaint nsStyleSVG::*aFillOrStroke,
                          float aOpacity,
                          imgDrawingParams& aImgParams,
                          const gfxRect* aOverrideBounds) override;
=======
  virtual already_AddRefed<gfxPattern> GetPaintServerPattern(
      nsIFrame* aSource, const DrawTarget* aDrawTarget,
      const gfxMatrix& aContextMatrix,
      mozilla::StyleSVGPaint nsStyleSVG::*aFillOrStroke, float aGraphicOpacity,
      imgDrawingParams& aImgParams, const gfxRect* aOverrideBounds) override;
>>>>>>> upstream-releases

  // nsIFrame interface:
  virtual nsresult AttributeChanged(int32_t aNameSpaceID, nsAtom* aAttribute,
                                    int32_t aModType) override;

#ifdef DEBUG_FRAME_DUMP
  virtual nsresult GetFrameName(nsAString& aResult) const override {
    return MakeFrameName(NS_LITERAL_STRING("SVGGradient"), aResult);
  }
#endif  // DEBUG

 private:
  /**
   * Parses this frame's href and - if it references another gradient - returns
   * it.  It also makes this frame a rendering observer of the specified ID.
   */
  nsSVGGradientFrame* GetReferencedGradient();

  // Optionally get a stop frame (returns stop index/count)
  void GetStopFrames(nsTArray<nsIFrame*>* aStopFrames);

<<<<<<< HEAD
  const mozilla::nsSVGAnimatedTransformList* GetGradientTransformList(
      nsIContent* aDefault);
||||||| merged common ancestors
  const mozilla::nsSVGAnimatedTransformList* GetGradientTransformList(
    nsIContent* aDefault);
=======
  const mozilla::SVGAnimatedTransformList* GetGradientTransformList(
      nsIContent* aDefault);
>>>>>>> upstream-releases
  // Will be singular for gradientUnits="objectBoundingBox" with an empty bbox.
  gfxMatrix GetGradientTransform(nsIFrame* aSource,
                                 const gfxRect* aOverrideBounds);

 protected:
  virtual bool GradientVectorLengthIsZero() = 0;
  virtual already_AddRefed<gfxPattern> CreateGradient() = 0;

  // Accessors to lookup gradient attributes
  uint16_t GetEnumValue(uint32_t aIndex, nsIContent* aDefault);
  uint16_t GetEnumValue(uint32_t aIndex) {
    return GetEnumValue(aIndex, mContent);
  }
  uint16_t GetGradientUnits();
  uint16_t GetSpreadMethod();

  // Gradient-type-specific lookups since the length values differ between
  // linear and radial gradients
  virtual mozilla::dom::SVGLinearGradientElement* GetLinearGradientWithLength(
      uint32_t aIndex, mozilla::dom::SVGLinearGradientElement* aDefault);
  virtual mozilla::dom::SVGRadialGradientElement* GetRadialGradientWithLength(
      uint32_t aIndex, mozilla::dom::SVGRadialGradientElement* aDefault);

  // The frame our gradient is (currently) being applied to
  nsIFrame* mSource;

 private:
  // Flag to mark this frame as "in use" during recursive calls along our
  // gradient's reference chain so we can detect reference loops. See:
  // http://www.w3.org/TR/SVG11/pservers.html#LinearGradientElementHrefAttribute
  bool mLoopFlag;
  // Gradients often don't reference other gradients, so here we cache
  // the fact that that isn't happening.
  bool mNoHRefURI;
};

// -------------------------------------------------------------------------
// Linear Gradients
// -------------------------------------------------------------------------

<<<<<<< HEAD
class nsSVGLinearGradientFrame final : public nsSVGGradientFrame {
  friend nsIFrame* NS_NewSVGLinearGradientFrame(nsIPresShell* aPresShell,
||||||| merged common ancestors
class nsSVGLinearGradientFrame final : public nsSVGGradientFrame
{
  friend nsIFrame* NS_NewSVGLinearGradientFrame(nsIPresShell* aPresShell,
=======
class nsSVGLinearGradientFrame final : public nsSVGGradientFrame {
  friend nsIFrame* NS_NewSVGLinearGradientFrame(mozilla::PresShell* aPresShell,
>>>>>>> upstream-releases
                                                ComputedStyle* aStyle);
<<<<<<< HEAD

 protected:
  explicit nsSVGLinearGradientFrame(ComputedStyle* aStyle)
      : nsSVGGradientFrame(aStyle, kClassID) {}
||||||| merged common ancestors
protected:
  explicit nsSVGLinearGradientFrame(ComputedStyle* aStyle)
    : nsSVGGradientFrame(aStyle, kClassID)
  {}
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
||||||| merged common ancestors
public:
=======
 protected:
  explicit nsSVGLinearGradientFrame(ComputedStyle* aStyle,
                                    nsPresContext* aPresContext)
      : nsSVGGradientFrame(aStyle, aPresContext, kClassID) {}

 public:
>>>>>>> upstream-releases
  NS_DECL_FRAMEARENA_HELPERS(nsSVGLinearGradientFrame)

  // nsIFrame interface:
#ifdef DEBUG
  virtual void Init(nsIContent* aContent, nsContainerFrame* aParent,
                    nsIFrame* aPrevInFlow) override;
#endif

  virtual nsresult AttributeChanged(int32_t aNameSpaceID, nsAtom* aAttribute,
                                    int32_t aModType) override;

#ifdef DEBUG_FRAME_DUMP
  virtual nsresult GetFrameName(nsAString& aResult) const override {
    return MakeFrameName(NS_LITERAL_STRING("SVGLinearGradient"), aResult);
  }
#endif  // DEBUG

 protected:
  float GetLengthValue(uint32_t aIndex);
  virtual mozilla::dom::SVGLinearGradientElement* GetLinearGradientWithLength(
      uint32_t aIndex,
      mozilla::dom::SVGLinearGradientElement* aDefault) override;
  virtual bool GradientVectorLengthIsZero() override;
  virtual already_AddRefed<gfxPattern> CreateGradient() override;
};

// -------------------------------------------------------------------------
// Radial Gradients
// -------------------------------------------------------------------------

<<<<<<< HEAD
class nsSVGRadialGradientFrame final : public nsSVGGradientFrame {
  friend nsIFrame* NS_NewSVGRadialGradientFrame(nsIPresShell* aPresShell,
||||||| merged common ancestors
class nsSVGRadialGradientFrame final : public nsSVGGradientFrame
{
  friend nsIFrame* NS_NewSVGRadialGradientFrame(nsIPresShell* aPresShell,
=======
class nsSVGRadialGradientFrame final : public nsSVGGradientFrame {
  friend nsIFrame* NS_NewSVGRadialGradientFrame(mozilla::PresShell* aPresShell,
>>>>>>> upstream-releases
                                                ComputedStyle* aStyle);
<<<<<<< HEAD

 protected:
  explicit nsSVGRadialGradientFrame(ComputedStyle* aStyle)
      : nsSVGGradientFrame(aStyle, kClassID) {}
||||||| merged common ancestors
protected:
  explicit nsSVGRadialGradientFrame(ComputedStyle* aStyle)
    : nsSVGGradientFrame(aStyle, kClassID)
  {}
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
||||||| merged common ancestors
public:
=======
 protected:
  explicit nsSVGRadialGradientFrame(ComputedStyle* aStyle,
                                    nsPresContext* aPresContext)
      : nsSVGGradientFrame(aStyle, aPresContext, kClassID) {}

 public:
>>>>>>> upstream-releases
  NS_DECL_FRAMEARENA_HELPERS(nsSVGRadialGradientFrame)

  // nsIFrame interface:
#ifdef DEBUG
  virtual void Init(nsIContent* aContent, nsContainerFrame* aParent,
                    nsIFrame* aPrevInFlow) override;
#endif

  virtual nsresult AttributeChanged(int32_t aNameSpaceID, nsAtom* aAttribute,
                                    int32_t aModType) override;

#ifdef DEBUG_FRAME_DUMP
  virtual nsresult GetFrameName(nsAString& aResult) const override {
    return MakeFrameName(NS_LITERAL_STRING("SVGRadialGradient"), aResult);
  }
#endif  // DEBUG

 protected:
  float GetLengthValue(uint32_t aIndex);
  float GetLengthValue(uint32_t aIndex, float aDefaultValue);
  float GetLengthValueFromElement(
      uint32_t aIndex, mozilla::dom::SVGRadialGradientElement& aElement);
  virtual mozilla::dom::SVGRadialGradientElement* GetRadialGradientWithLength(
      uint32_t aIndex,
      mozilla::dom::SVGRadialGradientElement* aDefault) override;
  virtual bool GradientVectorLengthIsZero() override;
  virtual already_AddRefed<gfxPattern> CreateGradient() override;
};

#endif  // __NS_SVGGRADIENTFRAME_H__
