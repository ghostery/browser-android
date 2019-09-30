/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGFEMorphologyElement.h"
#include "mozilla/dom/SVGFEMorphologyElementBinding.h"
#include "nsSVGFilterInstance.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(FEMorphology)

using namespace mozilla::gfx;

namespace mozilla {
namespace dom {

JSObject* SVGFEMorphologyElement::WrapNode(JSContext* aCx,
                                           JS::Handle<JSObject*> aGivenProto) {
  return SVGFEMorphologyElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::NumberPairInfo SVGFEMorphologyElement::sNumberPairInfo[1] = {
    {nsGkAtoms::radius, 0, 0}};

nsSVGEnumMapping SVGFEMorphologyElement::sOperatorMap[] = {
    {nsGkAtoms::erode, SVG_OPERATOR_ERODE},
    {nsGkAtoms::dilate, SVG_OPERATOR_DILATE},
    {nullptr, 0}};

nsSVGElement::EnumInfo SVGFEMorphologyElement::sEnumInfo[1] = {
    {nsGkAtoms::_operator, sOperatorMap, SVG_OPERATOR_ERODE}};

nsSVGElement::StringInfo SVGFEMorphologyElement::sStringInfo[2] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true}};
||||||| merged common ancestors
nsSVGElement::NumberPairInfo SVGFEMorphologyElement::sNumberPairInfo[1] =
{
  { nsGkAtoms::radius, 0, 0 }
};

nsSVGEnumMapping SVGFEMorphologyElement::sOperatorMap[] = {
  {nsGkAtoms::erode, SVG_OPERATOR_ERODE},
  {nsGkAtoms::dilate, SVG_OPERATOR_DILATE},
  {nullptr, 0}
};

nsSVGElement::EnumInfo SVGFEMorphologyElement::sEnumInfo[1] =
{
  { nsGkAtoms::_operator,
    sOperatorMap,
    SVG_OPERATOR_ERODE
  }
};

nsSVGElement::StringInfo SVGFEMorphologyElement::sStringInfo[2] =
{
  { nsGkAtoms::result, kNameSpaceID_None, true },
  { nsGkAtoms::in, kNameSpaceID_None, true }
};
=======
SVGElement::NumberPairInfo SVGFEMorphologyElement::sNumberPairInfo[1] = {
    {nsGkAtoms::radius, 0, 0}};

SVGEnumMapping SVGFEMorphologyElement::sOperatorMap[] = {
    {nsGkAtoms::erode, SVG_OPERATOR_ERODE},
    {nsGkAtoms::dilate, SVG_OPERATOR_DILATE},
    {nullptr, 0}};

SVGElement::EnumInfo SVGFEMorphologyElement::sEnumInfo[1] = {
    {nsGkAtoms::_operator, sOperatorMap, SVG_OPERATOR_ERODE}};

SVGElement::StringInfo SVGFEMorphologyElement::sStringInfo[2] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGFEMorphologyElement)

//----------------------------------------------------------------------
// SVGFEMorphologyElement methods

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGFEMorphologyElement::In1() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGFEMorphologyElement::In1()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGFEMorphologyElement::In1() {
>>>>>>> upstream-releases
  return mStringAttributes[IN1].ToDOMAnimatedString(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration> SVGFEMorphologyElement::Operator() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGFEMorphologyElement::Operator()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration> SVGFEMorphologyElement::Operator() {
>>>>>>> upstream-releases
  return mEnumAttributes[OPERATOR].ToDOMAnimatedEnum(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFEMorphologyElement::RadiusX() {
  return mNumberPairAttributes[RADIUS].ToDOMAnimatedNumber(
      nsSVGNumberPair::eFirst, this);
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEMorphologyElement::RadiusX()
{
  return mNumberPairAttributes[RADIUS].ToDOMAnimatedNumber(nsSVGNumberPair::eFirst, this);
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFEMorphologyElement::RadiusX() {
  return mNumberPairAttributes[RADIUS].ToDOMAnimatedNumber(
      SVGAnimatedNumberPair::eFirst, this);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFEMorphologyElement::RadiusY() {
  return mNumberPairAttributes[RADIUS].ToDOMAnimatedNumber(
      nsSVGNumberPair::eSecond, this);
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEMorphologyElement::RadiusY()
{
  return mNumberPairAttributes[RADIUS].ToDOMAnimatedNumber(nsSVGNumberPair::eSecond, this);
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFEMorphologyElement::RadiusY() {
  return mNumberPairAttributes[RADIUS].ToDOMAnimatedNumber(
      SVGAnimatedNumberPair::eSecond, this);
>>>>>>> upstream-releases
}

void SVGFEMorphologyElement::SetRadius(float rx, float ry) {
  mNumberPairAttributes[RADIUS].SetBaseValues(rx, ry, this);
}

<<<<<<< HEAD
void SVGFEMorphologyElement::GetSourceImageNames(
    nsTArray<nsSVGStringInfo>& aSources) {
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
||||||| merged common ancestors
void
SVGFEMorphologyElement::GetSourceImageNames(nsTArray<nsSVGStringInfo>& aSources)
{
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
=======
void SVGFEMorphologyElement::GetSourceImageNames(
    nsTArray<SVGStringInfo>& aSources) {
  aSources.AppendElement(SVGStringInfo(&mStringAttributes[IN1], this));
>>>>>>> upstream-releases
}

#define MORPHOLOGY_EPSILON 0.0001

void SVGFEMorphologyElement::GetRXY(int32_t* aRX, int32_t* aRY,
                                    const nsSVGFilterInstance& aInstance) {
  // Subtract an epsilon here because we don't want a value that's just
  // slightly larger than an integer to round up to the next integer; it's
  // probably meant to be the integer it's close to, modulo machine precision
  // issues.
<<<<<<< HEAD
  *aRX = NSToIntCeil(aInstance.GetPrimitiveNumber(
                         SVGContentUtils::X, &mNumberPairAttributes[RADIUS],
                         nsSVGNumberPair::eFirst) -
||||||| merged common ancestors
  *aRX = NSToIntCeil(aInstance.GetPrimitiveNumber(SVGContentUtils::X,
                                                  &mNumberPairAttributes[RADIUS],
                                                  nsSVGNumberPair::eFirst) -
=======
  *aRX = NSToIntCeil(aInstance.GetPrimitiveNumber(
                         SVGContentUtils::X, &mNumberPairAttributes[RADIUS],
                         SVGAnimatedNumberPair::eFirst) -
>>>>>>> upstream-releases
                     MORPHOLOGY_EPSILON);
<<<<<<< HEAD
  *aRY = NSToIntCeil(aInstance.GetPrimitiveNumber(
                         SVGContentUtils::Y, &mNumberPairAttributes[RADIUS],
                         nsSVGNumberPair::eSecond) -
||||||| merged common ancestors
  *aRY = NSToIntCeil(aInstance.GetPrimitiveNumber(SVGContentUtils::Y,
                                                  &mNumberPairAttributes[RADIUS],
                                                  nsSVGNumberPair::eSecond) -
=======
  *aRY = NSToIntCeil(aInstance.GetPrimitiveNumber(
                         SVGContentUtils::Y, &mNumberPairAttributes[RADIUS],
                         SVGAnimatedNumberPair::eSecond) -
>>>>>>> upstream-releases
                     MORPHOLOGY_EPSILON);
}

FilterPrimitiveDescription SVGFEMorphologyElement::GetPrimitiveDescription(
    nsSVGFilterInstance* aInstance, const IntRect& aFilterSubregion,
    const nsTArray<bool>& aInputsAreTainted,
    nsTArray<RefPtr<SourceSurface>>& aInputImages) {
  int32_t rx, ry;
  GetRXY(&rx, &ry, *aInstance);
  MorphologyAttributes atts;
  atts.mRadii = Size(rx, ry);
  atts.mOperator = (uint32_t)mEnumAttributes[OPERATOR].GetAnimValue();
  return FilterPrimitiveDescription(AsVariant(std::move(atts)));
}

bool SVGFEMorphologyElement::AttributeAffectsRendering(
    int32_t aNameSpaceID, nsAtom* aAttribute) const {
  return SVGFEMorphologyElementBase::AttributeAffectsRendering(aNameSpaceID,
                                                               aAttribute) ||
         (aNameSpaceID == kNameSpaceID_None &&
          (aAttribute == nsGkAtoms::in || aAttribute == nsGkAtoms::radius ||
           aAttribute == nsGkAtoms::_operator));
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::NumberPairAttributesInfo
SVGFEMorphologyElement::GetNumberPairInfo() {
||||||| merged common ancestors
nsSVGElement::NumberPairAttributesInfo
SVGFEMorphologyElement::GetNumberPairInfo()
{
=======
SVGElement::NumberPairAttributesInfo
SVGFEMorphologyElement::GetNumberPairInfo() {
>>>>>>> upstream-releases
  return NumberPairAttributesInfo(mNumberPairAttributes, sNumberPairInfo,
                                  ArrayLength(sNumberPairInfo));
}

<<<<<<< HEAD
nsSVGElement::EnumAttributesInfo SVGFEMorphologyElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
||||||| merged common ancestors
nsSVGElement::EnumAttributesInfo
SVGFEMorphologyElement::GetEnumInfo()
{
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo,
                            ArrayLength(sEnumInfo));
=======
SVGElement::EnumAttributesInfo SVGFEMorphologyElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsSVGElement::StringAttributesInfo SVGFEMorphologyElement::GetStringInfo() {
||||||| merged common ancestors
nsSVGElement::StringAttributesInfo
SVGFEMorphologyElement::GetStringInfo()
{
=======
SVGElement::StringAttributesInfo SVGFEMorphologyElement::GetStringInfo() {
>>>>>>> upstream-releases
  return StringAttributesInfo(mStringAttributes, sStringInfo,
                              ArrayLength(sStringInfo));
}

}  // namespace dom
}  // namespace mozilla
