/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGTextContentElement.h"

#include "mozilla/dom/SVGLengthBinding.h"
#include "mozilla/dom/SVGTextContentElementBinding.h"
#include "mozilla/dom/SVGRect.h"
#include "nsBidiUtils.h"
#include "nsISVGPoint.h"
#include "nsTextFragment.h"
#include "nsTextFrameUtils.h"
#include "nsTextNode.h"
#include "SVGTextFrame.h"

namespace mozilla {
namespace dom {

using namespace SVGTextContentElement_Binding;

<<<<<<< HEAD
nsSVGEnumMapping SVGTextContentElement::sLengthAdjustMap[] = {
    {nsGkAtoms::spacing, LENGTHADJUST_SPACING},
    {nsGkAtoms::spacingAndGlyphs, LENGTHADJUST_SPACINGANDGLYPHS},
    {nullptr, 0}};
||||||| merged common ancestors
nsSVGEnumMapping SVGTextContentElement::sLengthAdjustMap[] = {
  { nsGkAtoms::spacing, LENGTHADJUST_SPACING },
  { nsGkAtoms::spacingAndGlyphs, LENGTHADJUST_SPACINGANDGLYPHS },
  { nullptr, 0 }
};
=======
SVGEnumMapping SVGTextContentElement::sLengthAdjustMap[] = {
    {nsGkAtoms::spacing, LENGTHADJUST_SPACING},
    {nsGkAtoms::spacingAndGlyphs, LENGTHADJUST_SPACINGANDGLYPHS},
    {nullptr, 0}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::EnumInfo SVGTextContentElement::sEnumInfo[1] = {
    {nsGkAtoms::lengthAdjust, sLengthAdjustMap, LENGTHADJUST_SPACING}};
||||||| merged common ancestors
nsSVGElement::EnumInfo SVGTextContentElement::sEnumInfo[1] =
{
  { nsGkAtoms::lengthAdjust, sLengthAdjustMap, LENGTHADJUST_SPACING }
};
=======
SVGElement::EnumInfo SVGTextContentElement::sEnumInfo[1] = {
    {nsGkAtoms::lengthAdjust, sLengthAdjustMap, LENGTHADJUST_SPACING}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::LengthInfo SVGTextContentElement::sLengthInfo[1] = {
    {nsGkAtoms::textLength, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::XY}};
||||||| merged common ancestors
nsSVGElement::LengthInfo SVGTextContentElement::sLengthInfo[1] =
{
  { nsGkAtoms::textLength, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER, SVGContentUtils::XY }
};
=======
SVGElement::LengthInfo SVGTextContentElement::sLengthInfo[1] = {
    {nsGkAtoms::textLength, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::XY}};
>>>>>>> upstream-releases

SVGTextFrame* SVGTextContentElement::GetSVGTextFrame() {
  nsIFrame* frame = GetPrimaryFrame(FlushType::Layout);
  nsIFrame* textFrame =
      nsLayoutUtils::GetClosestFrameOfType(frame, LayoutFrameType::SVGText);
  return static_cast<SVGTextFrame*>(textFrame);
}

SVGTextFrame*
SVGTextContentElement::GetSVGTextFrameForNonLayoutDependentQuery() {
  nsIFrame* frame = GetPrimaryFrame(FlushType::Frames);
  nsIFrame* textFrame =
      nsLayoutUtils::GetClosestFrameOfType(frame, LayoutFrameType::SVGText);
  return static_cast<SVGTextFrame*>(textFrame);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGTextContentElement::TextLength() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGTextContentElement::TextLength()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGTextContentElement::TextLength() {
>>>>>>> upstream-releases
  return LengthAttributes()[TEXTLENGTH].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration> SVGTextContentElement::LengthAdjust() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGTextContentElement::LengthAdjust()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration>
SVGTextContentElement::LengthAdjust() {
>>>>>>> upstream-releases
  return EnumAttributes()[LENGTHADJUST].ToDOMAnimatedEnum(this);
}

//----------------------------------------------------------------------

template <typename T>
static bool FragmentHasSkippableCharacter(const T* aBuffer, uint32_t aLength) {
  for (uint32_t i = 0; i < aLength; i++) {
    if (nsTextFrameUtils::IsSkippableCharacterForTransformText(aBuffer[i])) {
      return true;
    }
  }
  return false;
}

Maybe<int32_t> SVGTextContentElement::GetNonLayoutDependentNumberOfChars() {
  SVGTextFrame* frame = GetSVGTextFrameForNonLayoutDependentQuery();
  if (!frame || frame != GetPrimaryFrame()) {
    // Only support this fast path on <text>, not child <tspan>s, etc.
    return Some(0);
  }

  uint32_t num = 0;

  for (nsINode* n = Element::GetFirstChild(); n; n = n->GetNextSibling()) {
    if (!n->IsText()) {
      return Nothing();
    }

    const nsTextFragment* text = &n->AsText()->TextFragment();
    uint32_t length = text->GetLength();

    if (text->Is2b()) {
      if (FragmentHasSkippableCharacter(text->Get2b(), length)) {
        return Nothing();
      }
    } else {
      auto buffer = reinterpret_cast<const uint8_t*>(text->Get1b());
      if (FragmentHasSkippableCharacter(buffer, length)) {
        return Nothing();
      }
    }

    num += length;
  }

  return Some(num);
}

int32_t SVGTextContentElement::GetNumberOfChars() {
  Maybe<int32_t> num = GetNonLayoutDependentNumberOfChars();
  if (num) {
    return *num;
  }

  SVGTextFrame* textFrame = GetSVGTextFrame();
  return textFrame ? textFrame->GetNumberOfChars(this) : 0;
}

float SVGTextContentElement::GetComputedTextLength() {
  SVGTextFrame* textFrame = GetSVGTextFrame();
  return textFrame ? textFrame->GetComputedTextLength(this) : 0.0f;
}

void SVGTextContentElement::SelectSubString(uint32_t charnum, uint32_t nchars,
                                            ErrorResult& rv) {
  SVGTextFrame* textFrame = GetSVGTextFrame();
  if (!textFrame) return;

  rv = textFrame->SelectSubString(this, charnum, nchars);
}

float SVGTextContentElement::GetSubStringLength(uint32_t charnum,
                                                uint32_t nchars,
                                                ErrorResult& rv) {
  SVGTextFrame* textFrame = GetSVGTextFrameForNonLayoutDependentQuery();
  if (!textFrame) return 0.0f;

  float length = 0.0f;
  rv = textFrame->GetSubStringLength(this, charnum, nchars, &length);
  return length;
}

already_AddRefed<nsISVGPoint> SVGTextContentElement::GetStartPositionOfChar(
    uint32_t charnum, ErrorResult& rv) {
  SVGTextFrame* textFrame = GetSVGTextFrame();
  if (!textFrame) {
    rv.Throw(NS_ERROR_FAILURE);
    return nullptr;
  }

  nsCOMPtr<nsISVGPoint> point;
  rv = textFrame->GetStartPositionOfChar(this, charnum, getter_AddRefs(point));
  return point.forget();
}

already_AddRefed<nsISVGPoint> SVGTextContentElement::GetEndPositionOfChar(
    uint32_t charnum, ErrorResult& rv) {
  SVGTextFrame* textFrame = GetSVGTextFrame();
  if (!textFrame) {
    rv.Throw(NS_ERROR_FAILURE);
    return nullptr;
  }

  nsCOMPtr<nsISVGPoint> point;
  rv = textFrame->GetEndPositionOfChar(this, charnum, getter_AddRefs(point));
  return point.forget();
}

<<<<<<< HEAD
already_AddRefed<SVGIRect> SVGTextContentElement::GetExtentOfChar(
    uint32_t charnum, ErrorResult& rv) {
||||||| merged common ancestors
already_AddRefed<SVGIRect>
SVGTextContentElement::GetExtentOfChar(uint32_t charnum, ErrorResult& rv)
{
=======
already_AddRefed<SVGRect> SVGTextContentElement::GetExtentOfChar(
    uint32_t charnum, ErrorResult& rv) {
>>>>>>> upstream-releases
  SVGTextFrame* textFrame = GetSVGTextFrame();

  if (!textFrame) {
    rv.Throw(NS_ERROR_FAILURE);
    return nullptr;
  }

  RefPtr<SVGRect> rect;
  rv = textFrame->GetExtentOfChar(this, charnum, getter_AddRefs(rect));
  return rect.forget();
}

float SVGTextContentElement::GetRotationOfChar(uint32_t charnum,
                                               ErrorResult& rv) {
  SVGTextFrame* textFrame = GetSVGTextFrame();

  if (!textFrame) {
    rv.Throw(NS_ERROR_FAILURE);
    return 0.0f;
  }

  float rotation;
  rv = textFrame->GetRotationOfChar(this, charnum, &rotation);
  return rotation;
}

<<<<<<< HEAD
int32_t SVGTextContentElement::GetCharNumAtPosition(nsISVGPoint& aPoint) {
||||||| merged common ancestors
int32_t
SVGTextContentElement::GetCharNumAtPosition(nsISVGPoint& aPoint)
{
=======
int32_t SVGTextContentElement::GetCharNumAtPosition(
    const DOMPointInit& aPoint) {
>>>>>>> upstream-releases
  SVGTextFrame* textFrame = GetSVGTextFrame();
  return textFrame ? textFrame->GetCharNumAtPosition(this, aPoint) : -1;
}

}  // namespace dom
}  // namespace mozilla
