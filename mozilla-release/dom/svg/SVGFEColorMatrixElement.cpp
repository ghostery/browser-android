/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGFEColorMatrixElement.h"

#include "DOMSVGAnimatedNumberList.h"
#include "mozilla/dom/SVGFEColorMatrixElementBinding.h"
#include "nsSVGUtils.h"

#define NUM_ENTRIES_IN_4x5_MATRIX 20

NS_IMPL_NS_NEW_SVG_ELEMENT(FEColorMatrix)

using namespace mozilla::gfx;

namespace mozilla {
namespace dom {

JSObject* SVGFEColorMatrixElement::WrapNode(JSContext* aCx,
                                            JS::Handle<JSObject*> aGivenProto) {
  return SVGFEColorMatrixElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGEnumMapping SVGFEColorMatrixElement::sTypeMap[] = {
    {nsGkAtoms::matrix, SVG_FECOLORMATRIX_TYPE_MATRIX},
    {nsGkAtoms::saturate, SVG_FECOLORMATRIX_TYPE_SATURATE},
    {nsGkAtoms::hueRotate, SVG_FECOLORMATRIX_TYPE_HUE_ROTATE},
    {nsGkAtoms::luminanceToAlpha, SVG_FECOLORMATRIX_TYPE_LUMINANCE_TO_ALPHA},
    {nullptr, 0}};

nsSVGElement::EnumInfo SVGFEColorMatrixElement::sEnumInfo[1] = {
    {nsGkAtoms::type, sTypeMap, SVG_FECOLORMATRIX_TYPE_MATRIX}};
||||||| merged common ancestors
nsSVGEnumMapping SVGFEColorMatrixElement::sTypeMap[] = {
  {nsGkAtoms::matrix, SVG_FECOLORMATRIX_TYPE_MATRIX},
  {nsGkAtoms::saturate, SVG_FECOLORMATRIX_TYPE_SATURATE},
  {nsGkAtoms::hueRotate, SVG_FECOLORMATRIX_TYPE_HUE_ROTATE},
  {nsGkAtoms::luminanceToAlpha, SVG_FECOLORMATRIX_TYPE_LUMINANCE_TO_ALPHA},
  {nullptr, 0}
};

nsSVGElement::EnumInfo SVGFEColorMatrixElement::sEnumInfo[1] =
{
  { nsGkAtoms::type,
    sTypeMap,
    SVG_FECOLORMATRIX_TYPE_MATRIX
  }
};
=======
SVGEnumMapping SVGFEColorMatrixElement::sTypeMap[] = {
    {nsGkAtoms::matrix, SVG_FECOLORMATRIX_TYPE_MATRIX},
    {nsGkAtoms::saturate, SVG_FECOLORMATRIX_TYPE_SATURATE},
    {nsGkAtoms::hueRotate, SVG_FECOLORMATRIX_TYPE_HUE_ROTATE},
    {nsGkAtoms::luminanceToAlpha, SVG_FECOLORMATRIX_TYPE_LUMINANCE_TO_ALPHA},
    {nullptr, 0}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::StringInfo SVGFEColorMatrixElement::sStringInfo[2] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true}};
||||||| merged common ancestors
nsSVGElement::StringInfo SVGFEColorMatrixElement::sStringInfo[2] =
{
  { nsGkAtoms::result, kNameSpaceID_None, true },
  { nsGkAtoms::in, kNameSpaceID_None, true }
};
=======
SVGElement::EnumInfo SVGFEColorMatrixElement::sEnumInfo[1] = {
    {nsGkAtoms::type, sTypeMap, SVG_FECOLORMATRIX_TYPE_MATRIX}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::NumberListInfo SVGFEColorMatrixElement::sNumberListInfo[1] = {
    {nsGkAtoms::values}};
||||||| merged common ancestors
nsSVGElement::NumberListInfo SVGFEColorMatrixElement::sNumberListInfo[1] =
{
  { nsGkAtoms::values }
};
=======
SVGElement::StringInfo SVGFEColorMatrixElement::sStringInfo[2] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true}};

SVGElement::NumberListInfo SVGFEColorMatrixElement::sNumberListInfo[1] = {
    {nsGkAtoms::values}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGFEColorMatrixElement)

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGFEColorMatrixElement::In1() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGFEColorMatrixElement::In1()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGFEColorMatrixElement::In1() {
>>>>>>> upstream-releases
  return mStringAttributes[IN1].ToDOMAnimatedString(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration> SVGFEColorMatrixElement::Type() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGFEColorMatrixElement::Type()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration> SVGFEColorMatrixElement::Type() {
>>>>>>> upstream-releases
  return mEnumAttributes[TYPE].ToDOMAnimatedEnum(this);
}

already_AddRefed<DOMSVGAnimatedNumberList> SVGFEColorMatrixElement::Values() {
  return DOMSVGAnimatedNumberList::GetDOMWrapper(&mNumberListAttributes[VALUES],
                                                 this, VALUES);
}

<<<<<<< HEAD
void SVGFEColorMatrixElement::GetSourceImageNames(
    nsTArray<nsSVGStringInfo>& aSources) {
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
||||||| merged common ancestors
void
SVGFEColorMatrixElement::GetSourceImageNames(nsTArray<nsSVGStringInfo>& aSources)
{
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
=======
void SVGFEColorMatrixElement::GetSourceImageNames(
    nsTArray<SVGStringInfo>& aSources) {
  aSources.AppendElement(SVGStringInfo(&mStringAttributes[IN1], this));
>>>>>>> upstream-releases
}

FilterPrimitiveDescription SVGFEColorMatrixElement::GetPrimitiveDescription(
    nsSVGFilterInstance* aInstance, const IntRect& aFilterSubregion,
    const nsTArray<bool>& aInputsAreTainted,
    nsTArray<RefPtr<SourceSurface>>& aInputImages) {
  uint32_t type = mEnumAttributes[TYPE].GetAnimValue();
  const SVGNumberList& values = mNumberListAttributes[VALUES].GetAnimValue();

  ColorMatrixAttributes atts;
  if (!mNumberListAttributes[VALUES].IsExplicitlySet() &&
      (type == SVG_FECOLORMATRIX_TYPE_MATRIX ||
       type == SVG_FECOLORMATRIX_TYPE_SATURATE ||
       type == SVG_FECOLORMATRIX_TYPE_HUE_ROTATE)) {
    atts.mType = (uint32_t)SVG_FECOLORMATRIX_TYPE_MATRIX;
    static const float identityMatrix[] = {
        // clang-format off
        1, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 1, 0
        // clang-format on
    };
    atts.mValues.AppendElements(identityMatrix, 20);
  } else {
    atts.mType = type;
    if (values.Length()) {
      atts.mValues.AppendElements(&values[0], values.Length());
    }
  }

  return FilterPrimitiveDescription(AsVariant(std::move(atts)));
}

bool SVGFEColorMatrixElement::AttributeAffectsRendering(
    int32_t aNameSpaceID, nsAtom* aAttribute) const {
  return SVGFEColorMatrixElementBase::AttributeAffectsRendering(aNameSpaceID,
                                                                aAttribute) ||
         (aNameSpaceID == kNameSpaceID_None &&
          (aAttribute == nsGkAtoms::in || aAttribute == nsGkAtoms::type ||
           aAttribute == nsGkAtoms::values));
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::EnumAttributesInfo SVGFEColorMatrixElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
||||||| merged common ancestors
nsSVGElement::EnumAttributesInfo
SVGFEColorMatrixElement::GetEnumInfo()
{
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo,
                            ArrayLength(sEnumInfo));
=======
SVGElement::EnumAttributesInfo SVGFEColorMatrixElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsSVGElement::StringAttributesInfo SVGFEColorMatrixElement::GetStringInfo() {
||||||| merged common ancestors
nsSVGElement::StringAttributesInfo
SVGFEColorMatrixElement::GetStringInfo()
{
=======
SVGElement::StringAttributesInfo SVGFEColorMatrixElement::GetStringInfo() {
>>>>>>> upstream-releases
  return StringAttributesInfo(mStringAttributes, sStringInfo,
                              ArrayLength(sStringInfo));
}

<<<<<<< HEAD
nsSVGElement::NumberListAttributesInfo
SVGFEColorMatrixElement::GetNumberListInfo() {
||||||| merged common ancestors
nsSVGElement::NumberListAttributesInfo
SVGFEColorMatrixElement::GetNumberListInfo()
{
=======
SVGElement::NumberListAttributesInfo
SVGFEColorMatrixElement::GetNumberListInfo() {
>>>>>>> upstream-releases
  return NumberListAttributesInfo(mNumberListAttributes, sNumberListInfo,
                                  ArrayLength(sNumberListInfo));
}

}  // namespace dom
}  // namespace mozilla
