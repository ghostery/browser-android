/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGFEDropShadowElement.h"
#include "mozilla/dom/SVGFEDropShadowElementBinding.h"
#include "nsIFrame.h"
#include "nsSVGFilterInstance.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(FEDropShadow)

using namespace mozilla::gfx;

namespace mozilla {
namespace dom {

JSObject* SVGFEDropShadowElement::WrapNode(JSContext* aCx,
                                           JS::Handle<JSObject*> aGivenProto) {
  return SVGFEDropShadowElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::NumberInfo SVGFEDropShadowElement::sNumberInfo[2] = {
    {nsGkAtoms::dx, 2, false}, {nsGkAtoms::dy, 2, false}};
||||||| merged common ancestors
nsSVGElement::NumberInfo SVGFEDropShadowElement::sNumberInfo[2] =
{
  { nsGkAtoms::dx, 2, false },
  { nsGkAtoms::dy, 2, false }
};
=======
SVGElement::NumberInfo SVGFEDropShadowElement::sNumberInfo[2] = {
    {nsGkAtoms::dx, 2, false}, {nsGkAtoms::dy, 2, false}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::NumberPairInfo SVGFEDropShadowElement::sNumberPairInfo[1] = {
    {nsGkAtoms::stdDeviation, 2, 2}};
||||||| merged common ancestors
nsSVGElement::NumberPairInfo SVGFEDropShadowElement::sNumberPairInfo[1] =
{
  { nsGkAtoms::stdDeviation, 2, 2 }
};
=======
SVGElement::NumberPairInfo SVGFEDropShadowElement::sNumberPairInfo[1] = {
    {nsGkAtoms::stdDeviation, 2, 2}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::StringInfo SVGFEDropShadowElement::sStringInfo[2] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true}};
||||||| merged common ancestors
nsSVGElement::StringInfo SVGFEDropShadowElement::sStringInfo[2] =
{
  { nsGkAtoms::result, kNameSpaceID_None, true },
  { nsGkAtoms::in, kNameSpaceID_None, true }
};
=======
SVGElement::StringInfo SVGFEDropShadowElement::sStringInfo[2] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGFEDropShadowElement)

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGFEDropShadowElement::In1() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGFEDropShadowElement::In1()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGFEDropShadowElement::In1() {
>>>>>>> upstream-releases
  return mStringAttributes[IN1].ToDOMAnimatedString(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFEDropShadowElement::Dx() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEDropShadowElement::Dx()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFEDropShadowElement::Dx() {
>>>>>>> upstream-releases
  return mNumberAttributes[DX].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFEDropShadowElement::Dy() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEDropShadowElement::Dy()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFEDropShadowElement::Dy() {
>>>>>>> upstream-releases
  return mNumberAttributes[DY].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFEDropShadowElement::StdDeviationX() {
  return mNumberPairAttributes[STD_DEV].ToDOMAnimatedNumber(
      nsSVGNumberPair::eFirst, this);
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEDropShadowElement::StdDeviationX()
{
  return mNumberPairAttributes[STD_DEV].ToDOMAnimatedNumber(nsSVGNumberPair::eFirst, this);
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFEDropShadowElement::StdDeviationX() {
  return mNumberPairAttributes[STD_DEV].ToDOMAnimatedNumber(
      SVGAnimatedNumberPair::eFirst, this);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFEDropShadowElement::StdDeviationY() {
  return mNumberPairAttributes[STD_DEV].ToDOMAnimatedNumber(
      nsSVGNumberPair::eSecond, this);
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEDropShadowElement::StdDeviationY()
{
  return mNumberPairAttributes[STD_DEV].ToDOMAnimatedNumber(nsSVGNumberPair::eSecond, this);
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFEDropShadowElement::StdDeviationY() {
  return mNumberPairAttributes[STD_DEV].ToDOMAnimatedNumber(
      SVGAnimatedNumberPair::eSecond, this);
>>>>>>> upstream-releases
}

void SVGFEDropShadowElement::SetStdDeviation(float stdDeviationX,
                                             float stdDeviationY) {
  mNumberPairAttributes[STD_DEV].SetBaseValues(stdDeviationX, stdDeviationY,
                                               this);
}

FilterPrimitiveDescription SVGFEDropShadowElement::GetPrimitiveDescription(
    nsSVGFilterInstance* aInstance, const IntRect& aFilterSubregion,
    const nsTArray<bool>& aInputsAreTainted,
    nsTArray<RefPtr<SourceSurface>>& aInputImages) {
  float stdX = aInstance->GetPrimitiveNumber(SVGContentUtils::X,
                                             &mNumberPairAttributes[STD_DEV],
                                             SVGAnimatedNumberPair::eFirst);
  float stdY = aInstance->GetPrimitiveNumber(SVGContentUtils::Y,
                                             &mNumberPairAttributes[STD_DEV],
                                             SVGAnimatedNumberPair::eSecond);
  if (stdX < 0 || stdY < 0) {
    return FilterPrimitiveDescription();
  }

  IntPoint offset(int32_t(aInstance->GetPrimitiveNumber(
                      SVGContentUtils::X, &mNumberAttributes[DX])),
                  int32_t(aInstance->GetPrimitiveNumber(
                      SVGContentUtils::Y, &mNumberAttributes[DY])));

  DropShadowAttributes atts;
  atts.mStdDeviation = Size(stdX, stdY);
  atts.mOffset = offset;

  nsIFrame* frame = GetPrimaryFrame();
  if (frame) {
    const nsStyleSVGReset* styleSVGReset = frame->Style()->StyleSVGReset();
    Color color(Color::FromABGR(styleSVGReset->mFloodColor.CalcColor(frame)));
    color.a *= styleSVGReset->mFloodOpacity;
    atts.mColor = color;
  } else {
    atts.mColor = Color();
  }
  return FilterPrimitiveDescription(AsVariant(std::move(atts)));
}

bool SVGFEDropShadowElement::AttributeAffectsRendering(
    int32_t aNameSpaceID, nsAtom* aAttribute) const {
  return SVGFEDropShadowElementBase::AttributeAffectsRendering(aNameSpaceID,
                                                               aAttribute) ||
         (aNameSpaceID == kNameSpaceID_None &&
          (aAttribute == nsGkAtoms::in ||
           aAttribute == nsGkAtoms::stdDeviation ||
           aAttribute == nsGkAtoms::dx || aAttribute == nsGkAtoms::dy));
}

<<<<<<< HEAD
void SVGFEDropShadowElement::GetSourceImageNames(
    nsTArray<nsSVGStringInfo>& aSources) {
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
||||||| merged common ancestors
void
SVGFEDropShadowElement::GetSourceImageNames(nsTArray<nsSVGStringInfo>& aSources)
{
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
=======
void SVGFEDropShadowElement::GetSourceImageNames(
    nsTArray<SVGStringInfo>& aSources) {
  aSources.AppendElement(SVGStringInfo(&mStringAttributes[IN1], this));
>>>>>>> upstream-releases
}

//----------------------------------------------------------------------
// nsIContent methods

NS_IMETHODIMP_(bool)
SVGFEDropShadowElement::IsAttributeMapped(const nsAtom* name) const {
  static const MappedAttributeEntry* const map[] = {sFEFloodMap};

  return FindAttributeDependence(name, map) ||
         SVGFEDropShadowElementBase::IsAttributeMapped(name);
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::NumberAttributesInfo SVGFEDropShadowElement::GetNumberInfo() {
||||||| merged common ancestors
nsSVGElement::NumberAttributesInfo
SVGFEDropShadowElement::GetNumberInfo()
{
=======
SVGElement::NumberAttributesInfo SVGFEDropShadowElement::GetNumberInfo() {
>>>>>>> upstream-releases
  return NumberAttributesInfo(mNumberAttributes, sNumberInfo,
                              ArrayLength(sNumberInfo));
}

<<<<<<< HEAD
nsSVGElement::NumberPairAttributesInfo
SVGFEDropShadowElement::GetNumberPairInfo() {
||||||| merged common ancestors
nsSVGElement::NumberPairAttributesInfo
SVGFEDropShadowElement::GetNumberPairInfo()
{
=======
SVGElement::NumberPairAttributesInfo
SVGFEDropShadowElement::GetNumberPairInfo() {
>>>>>>> upstream-releases
  return NumberPairAttributesInfo(mNumberPairAttributes, sNumberPairInfo,
                                  ArrayLength(sNumberPairInfo));
}

<<<<<<< HEAD
nsSVGElement::StringAttributesInfo SVGFEDropShadowElement::GetStringInfo() {
||||||| merged common ancestors
nsSVGElement::StringAttributesInfo
SVGFEDropShadowElement::GetStringInfo()
{
=======
SVGElement::StringAttributesInfo SVGFEDropShadowElement::GetStringInfo() {
>>>>>>> upstream-releases
  return StringAttributesInfo(mStringAttributes, sStringInfo,
                              ArrayLength(sStringInfo));
}

}  // namespace dom
}  // namespace mozilla
