/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// Main header first:
#include "nsSVGMaskFrame.h"

// Keep others in (case-insensitive) order:
#include "AutoReferenceChainGuard.h"
#include "gfx2DGlue.h"
#include "gfxContext.h"
#include "mozilla/PresShell.h"
#include "mozilla/RefPtr.h"
#include "mozilla/dom/SVGMaskElement.h"
#include "mozilla/dom/SVGUnitTypesBinding.h"
#include "mozilla/gfx/2D.h"
#include "SVGObserverUtils.h"

using namespace mozilla;
using namespace mozilla::dom;
using namespace mozilla::dom::SVGUnitTypes_Binding;
using namespace mozilla::gfx;
using namespace mozilla::image;

static LuminanceType GetLuminanceType(uint8_t aNSMaskType) {
  switch (aNSMaskType) {
    case NS_STYLE_MASK_TYPE_LUMINANCE:
      return LuminanceType::LUMINANCE;
    case NS_STYLE_COLOR_INTERPOLATION_LINEARRGB:
      return LuminanceType::LINEARRGB;
    default: {
      NS_WARNING("Unknown SVG mask type, defaulting to luminance");
      return LuminanceType::LUMINANCE;
    }
  }
}

<<<<<<< HEAD
nsIFrame* NS_NewSVGMaskFrame(nsIPresShell* aPresShell, ComputedStyle* aStyle) {
  return new (aPresShell) nsSVGMaskFrame(aStyle);
||||||| merged common ancestors
nsIFrame*
NS_NewSVGMaskFrame(nsIPresShell* aPresShell, ComputedStyle* aStyle)
{
  return new (aPresShell) nsSVGMaskFrame(aStyle);
=======
nsIFrame* NS_NewSVGMaskFrame(PresShell* aPresShell, ComputedStyle* aStyle) {
  return new (aPresShell) nsSVGMaskFrame(aStyle, aPresShell->GetPresContext());
>>>>>>> upstream-releases
}

NS_IMPL_FRAMEARENA_HELPERS(nsSVGMaskFrame)

already_AddRefed<SourceSurface> nsSVGMaskFrame::GetMaskForMaskedFrame(
    MaskParams& aParams) {
  // Make sure we break reference loops and over long reference chains:
  static int16_t sRefChainLengthCounter = AutoReferenceChainGuard::noChain;
  AutoReferenceChainGuard refChainGuard(this, &mInUse, &sRefChainLengthCounter);
  if (MOZ_UNLIKELY(!refChainGuard.Reference())) {
    // Break reference chain
    return nullptr;
  }

  gfxRect maskArea = GetMaskArea(aParams.maskedFrame);
  if (maskArea.IsEmpty()) {
    return nullptr;
  }
  gfxContext* context = aParams.ctx;
  // Get the clip extents in device space:
  // Minimizing the mask surface extents (using both the current clip extents
  // and maskArea) is important for performance.
  //
  gfxRect maskSurfaceRectDouble = aParams.toUserSpace.TransformBounds(maskArea);
  Rect maskSurfaceRect = ToRect(maskSurfaceRectDouble);
  maskSurfaceRect.RoundOut();
<<<<<<< HEAD
  context->Restore();

  bool resultOverflows;
  IntSize maskSurfaceSize = nsSVGUtils::ConvertToSurfaceSize(
      maskSurfaceRect.Size(), &resultOverflows);

  if (resultOverflows || maskSurfaceSize.IsEmpty()) {
    // Return value other then ImgDrawResult::SUCCESS, so the caller can skip
    // painting the masked frame(aParams.maskedFrame).
    return nullptr;
  }
||||||| merged common ancestors
  context->Restore();

  bool resultOverflows;
  IntSize maskSurfaceSize =
    nsSVGUtils::ConvertToSurfaceSize(maskSurfaceRect.Size(), &resultOverflows);

  if (resultOverflows || maskSurfaceSize.IsEmpty()) {
    // Return value other then ImgDrawResult::SUCCESS, so the caller can skip
    // painting the masked frame(aParams.maskedFrame).
    return nullptr;
  }
=======
>>>>>>> upstream-releases

  uint8_t maskType;
  if (aParams.maskMode == StyleMaskMode::MatchSource) {
    maskType = StyleSVGReset()->mMaskType;
  } else {
<<<<<<< HEAD
    maskType = aParams.maskMode == NS_STYLE_MASK_MODE_LUMINANCE
                   ? NS_STYLE_MASK_TYPE_LUMINANCE
                   : NS_STYLE_MASK_TYPE_ALPHA;
||||||| merged common ancestors
    maskType = aParams.maskMode == NS_STYLE_MASK_MODE_LUMINANCE
               ? NS_STYLE_MASK_TYPE_LUMINANCE : NS_STYLE_MASK_TYPE_ALPHA;
=======
    maskType = aParams.maskMode == StyleMaskMode::Luminance
                   ? NS_STYLE_MASK_TYPE_LUMINANCE
                   : NS_STYLE_MASK_TYPE_ALPHA;
>>>>>>> upstream-releases
  }

  RefPtr<DrawTarget> maskDT;
  if (maskType == NS_STYLE_MASK_TYPE_LUMINANCE) {
<<<<<<< HEAD
    maskDT = context->GetDrawTarget()->CreateSimilarDrawTarget(
        maskSurfaceSize, SurfaceFormat::B8G8R8A8);
||||||| merged common ancestors
    maskDT = context->GetDrawTarget()->CreateSimilarDrawTarget(
               maskSurfaceSize, SurfaceFormat::B8G8R8A8);
=======
    maskDT = context->GetDrawTarget()->CreateClippedDrawTarget(
        maskSurfaceRect, SurfaceFormat::B8G8R8A8);
>>>>>>> upstream-releases
  } else {
<<<<<<< HEAD
    maskDT = context->GetDrawTarget()->CreateSimilarDrawTarget(
        maskSurfaceSize, SurfaceFormat::A8);
||||||| merged common ancestors
    maskDT = context->GetDrawTarget()->CreateSimilarDrawTarget(
               maskSurfaceSize, SurfaceFormat::A8);
=======
    maskDT = context->GetDrawTarget()->CreateClippedDrawTarget(
        maskSurfaceRect, SurfaceFormat::A8);
>>>>>>> upstream-releases
  }

  if (!maskDT || !maskDT->IsValid()) {
    return nullptr;
  }

<<<<<<< HEAD
  Matrix maskSurfaceMatrix =
      context->CurrentMatrix() *
      ToMatrix(gfxMatrix::Translation(-maskSurfaceRect.TopLeft()));
||||||| merged common ancestors
  Matrix maskSurfaceMatrix =
    context->CurrentMatrix() * ToMatrix(gfxMatrix::Translation(-maskSurfaceRect.TopLeft()));
=======
  RefPtr<gfxContext> tmpCtx =
      gfxContext::CreatePreservingTransformOrNull(maskDT);
  MOZ_ASSERT(tmpCtx);  // already checked the draw target above
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<gfxContext> tmpCtx = gfxContext::CreateOrNull(maskDT);
  MOZ_ASSERT(tmpCtx);  // already checked the draw target above
  tmpCtx->SetMatrix(maskSurfaceMatrix);
||||||| merged common ancestors
  RefPtr<gfxContext> tmpCtx = gfxContext::CreateOrNull(maskDT);
  MOZ_ASSERT(tmpCtx); // already checked the draw target above
  tmpCtx->SetMatrix(maskSurfaceMatrix);
=======
  mMatrixForChildren =
      GetMaskTransform(aParams.maskedFrame) * aParams.toUserSpace;
>>>>>>> upstream-releases

<<<<<<< HEAD
  mMatrixForChildren =
      GetMaskTransform(aParams.maskedFrame) * aParams.toUserSpace;
||||||| merged common ancestors
  mMatrixForChildren = GetMaskTransform(aParams.maskedFrame) *
                       aParams.toUserSpace;
=======
  for (nsIFrame* kid = mFrames.FirstChild(); kid; kid = kid->GetNextSibling()) {
    gfxMatrix m = mMatrixForChildren;
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (nsIFrame* kid = mFrames.FirstChild(); kid; kid = kid->GetNextSibling()) {
||||||| merged common ancestors
  for (nsIFrame* kid = mFrames.FirstChild(); kid;
       kid = kid->GetNextSibling()) {
=======
>>>>>>> upstream-releases
    // The CTM of each frame referencing us can be different
    nsSVGDisplayableFrame* SVGFrame = do_QueryFrame(kid);
    if (SVGFrame) {
      SVGFrame->NotifySVGChanged(nsSVGDisplayableFrame::TRANSFORM_CHANGED);
      m = nsSVGUtils::GetTransformMatrixInUserSpace(kid) * m;
    }
<<<<<<< HEAD
    gfxMatrix m = mMatrixForChildren;
    if (kid->GetContent()->IsSVGElement()) {
      m = static_cast<nsSVGElement*>(kid->GetContent())
              ->PrependLocalTransformsTo(m, eUserSpaceToParent);
    }
||||||| merged common ancestors
    gfxMatrix m = mMatrixForChildren;
    if (kid->GetContent()->IsSVGElement()) {
      m = static_cast<nsSVGElement*>(kid->GetContent())->
            PrependLocalTransformsTo(m, eUserSpaceToParent);
    }
=======

>>>>>>> upstream-releases
    nsSVGUtils::PaintFrameWithEffects(kid, *tmpCtx, m, aParams.imgParams);
  }

  RefPtr<SourceSurface> surface;
  if (maskType == NS_STYLE_MASK_TYPE_LUMINANCE) {
    if (StyleSVG()->mColorInterpolation ==
        NS_STYLE_COLOR_INTERPOLATION_LINEARRGB) {
      maskType = NS_STYLE_COLOR_INTERPOLATION_LINEARRGB;
    }

    RefPtr<SourceSurface> maskSnapshot = maskDT->IntoLuminanceSource(
        GetLuminanceType(maskType), aParams.opacity);
    if (!maskSnapshot) {
      return nullptr;
    }
    surface = maskSnapshot.forget();
  } else {
<<<<<<< HEAD
    maskDT->SetTransform(Matrix());
    maskDT->FillRect(Rect(0, 0, maskSurfaceSize.width, maskSurfaceSize.height),
                     ColorPattern(Color(1.0f, 1.0f, 1.0f, aParams.opacity)),
                     DrawOptions(1, CompositionOp::OP_IN));
||||||| merged common ancestors
    maskDT->SetTransform(Matrix());
    maskDT->FillRect(Rect(0, 0, maskSurfaceSize.width, maskSurfaceSize.height), ColorPattern(Color(1.0f, 1.0f, 1.0f, aParams.opacity)), DrawOptions(1, CompositionOp::OP_IN));
=======
    maskDT->FillRect(maskSurfaceRect,
                     ColorPattern(Color(1.0f, 1.0f, 1.0f, aParams.opacity)),
                     DrawOptions(1, CompositionOp::OP_IN));
>>>>>>> upstream-releases
    RefPtr<SourceSurface> maskSnapshot = maskDT->Snapshot();
    if (!maskSnapshot) {
      return nullptr;
    }
    surface = maskSnapshot.forget();
  }

  return surface.forget();
}

gfxRect nsSVGMaskFrame::GetMaskArea(nsIFrame* aMaskedFrame) {
  SVGMaskElement* maskElem = static_cast<SVGMaskElement*>(GetContent());

  uint16_t units =
      maskElem->mEnumAttributes[SVGMaskElement::MASKUNITS].GetAnimValue();
  gfxRect bbox;
  if (units == SVG_UNIT_TYPE_OBJECTBOUNDINGBOX) {
    bbox = nsSVGUtils::GetBBox(aMaskedFrame,
                               nsSVGUtils::eUseFrameBoundsForOuterSVG |
                                   nsSVGUtils::eBBoxIncludeFillGeometry);
  }

  // Bounds in the user space of aMaskedFrame
  gfxRect maskArea = nsSVGUtils::GetRelativeRect(
      units, &maskElem->mLengthAttributes[SVGMaskElement::ATTR_X], bbox,
      aMaskedFrame);

  return maskArea;
}

nsresult nsSVGMaskFrame::AttributeChanged(int32_t aNameSpaceID,
                                          nsAtom* aAttribute,
                                          int32_t aModType) {
  if (aNameSpaceID == kNameSpaceID_None &&
      (aAttribute == nsGkAtoms::x || aAttribute == nsGkAtoms::y ||
       aAttribute == nsGkAtoms::width || aAttribute == nsGkAtoms::height ||
       aAttribute == nsGkAtoms::maskUnits ||
       aAttribute == nsGkAtoms::maskContentUnits)) {
    SVGObserverUtils::InvalidateDirectRenderingObservers(this);
  }

  return nsSVGContainerFrame::AttributeChanged(aNameSpaceID, aAttribute,
                                               aModType);
}

#ifdef DEBUG
void nsSVGMaskFrame::Init(nsIContent* aContent, nsContainerFrame* aParent,
                          nsIFrame* aPrevInFlow) {
  NS_ASSERTION(aContent->IsSVGElement(nsGkAtoms::mask),
               "Content is not an SVG mask");

  nsSVGContainerFrame::Init(aContent, aParent, aPrevInFlow);
}
#endif /* DEBUG */

gfxMatrix nsSVGMaskFrame::GetCanvasTM() { return mMatrixForChildren; }

gfxMatrix nsSVGMaskFrame::GetMaskTransform(nsIFrame* aMaskedFrame) {
  SVGMaskElement* content = static_cast<SVGMaskElement*>(GetContent());

<<<<<<< HEAD
  nsSVGEnum* maskContentUnits =
      &content->mEnumAttributes[SVGMaskElement::MASKCONTENTUNITS];
||||||| merged common ancestors
  nsSVGEnum* maskContentUnits =
    &content->mEnumAttributes[SVGMaskElement::MASKCONTENTUNITS];
=======
  SVGAnimatedEnumeration* maskContentUnits =
      &content->mEnumAttributes[SVGMaskElement::MASKCONTENTUNITS];
>>>>>>> upstream-releases

  uint32_t flags = nsSVGUtils::eBBoxIncludeFillGeometry |
                   (aMaskedFrame->StyleBorder()->mBoxDecorationBreak ==
                            StyleBoxDecorationBreak::Clone
                        ? nsSVGUtils::eIncludeOnlyCurrentFrameForNonSVGElement
                        : 0);

  return nsSVGUtils::AdjustMatrixForUnits(gfxMatrix(), maskContentUnits,
                                          aMaskedFrame, flags);
}
