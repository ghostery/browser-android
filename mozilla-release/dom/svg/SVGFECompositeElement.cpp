/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGFECompositeElement.h"
#include "mozilla/dom/SVGFECompositeElementBinding.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(FEComposite)

using namespace mozilla::gfx;

namespace mozilla {
namespace dom {

JSObject* SVGFECompositeElement::WrapNode(JSContext* aCx,
                                          JS::Handle<JSObject*> aGivenProto) {
  return SVGFECompositeElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::NumberInfo SVGFECompositeElement::sNumberInfo[4] = {
    {nsGkAtoms::k1, 0, false},
    {nsGkAtoms::k2, 0, false},
    {nsGkAtoms::k3, 0, false},
    {nsGkAtoms::k4, 0, false}};

nsSVGEnumMapping SVGFECompositeElement::sOperatorMap[] = {
    {nsGkAtoms::over, SVG_FECOMPOSITE_OPERATOR_OVER},
    {nsGkAtoms::in, SVG_FECOMPOSITE_OPERATOR_IN},
    {nsGkAtoms::out, SVG_FECOMPOSITE_OPERATOR_OUT},
    {nsGkAtoms::atop, SVG_FECOMPOSITE_OPERATOR_ATOP},
    {nsGkAtoms::xor_, SVG_FECOMPOSITE_OPERATOR_XOR},
    {nsGkAtoms::arithmetic, SVG_FECOMPOSITE_OPERATOR_ARITHMETIC},
    {nullptr, 0}};

nsSVGElement::EnumInfo SVGFECompositeElement::sEnumInfo[1] = {
    {nsGkAtoms::_operator, sOperatorMap, SVG_FECOMPOSITE_OPERATOR_OVER}};

nsSVGElement::StringInfo SVGFECompositeElement::sStringInfo[3] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true},
    {nsGkAtoms::in2, kNameSpaceID_None, true}};
||||||| merged common ancestors
nsSVGElement::NumberInfo SVGFECompositeElement::sNumberInfo[4] =
{
  { nsGkAtoms::k1, 0, false },
  { nsGkAtoms::k2, 0, false },
  { nsGkAtoms::k3, 0, false },
  { nsGkAtoms::k4, 0, false }
};

nsSVGEnumMapping SVGFECompositeElement::sOperatorMap[] = {
  {nsGkAtoms::over, SVG_FECOMPOSITE_OPERATOR_OVER},
  {nsGkAtoms::in, SVG_FECOMPOSITE_OPERATOR_IN},
  {nsGkAtoms::out, SVG_FECOMPOSITE_OPERATOR_OUT},
  {nsGkAtoms::atop, SVG_FECOMPOSITE_OPERATOR_ATOP},
  {nsGkAtoms::xor_, SVG_FECOMPOSITE_OPERATOR_XOR},
  {nsGkAtoms::arithmetic, SVG_FECOMPOSITE_OPERATOR_ARITHMETIC},
  {nullptr, 0}
};

nsSVGElement::EnumInfo SVGFECompositeElement::sEnumInfo[1] =
{
  { nsGkAtoms::_operator,
    sOperatorMap,
    SVG_FECOMPOSITE_OPERATOR_OVER
  }
};

nsSVGElement::StringInfo SVGFECompositeElement::sStringInfo[3] =
{
  { nsGkAtoms::result, kNameSpaceID_None, true },
  { nsGkAtoms::in, kNameSpaceID_None, true },
  { nsGkAtoms::in2, kNameSpaceID_None, true }
};
=======
SVGElement::NumberInfo SVGFECompositeElement::sNumberInfo[4] = {
    {nsGkAtoms::k1, 0, false},
    {nsGkAtoms::k2, 0, false},
    {nsGkAtoms::k3, 0, false},
    {nsGkAtoms::k4, 0, false}};

SVGEnumMapping SVGFECompositeElement::sOperatorMap[] = {
    {nsGkAtoms::over, SVG_FECOMPOSITE_OPERATOR_OVER},
    {nsGkAtoms::in, SVG_FECOMPOSITE_OPERATOR_IN},
    {nsGkAtoms::out, SVG_FECOMPOSITE_OPERATOR_OUT},
    {nsGkAtoms::atop, SVG_FECOMPOSITE_OPERATOR_ATOP},
    {nsGkAtoms::xor_, SVG_FECOMPOSITE_OPERATOR_XOR},
    {nsGkAtoms::arithmetic, SVG_FECOMPOSITE_OPERATOR_ARITHMETIC},
    {nullptr, 0}};

SVGElement::EnumInfo SVGFECompositeElement::sEnumInfo[1] = {
    {nsGkAtoms::_operator, sOperatorMap, SVG_FECOMPOSITE_OPERATOR_OVER}};

SVGElement::StringInfo SVGFECompositeElement::sStringInfo[3] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true},
    {nsGkAtoms::in2, kNameSpaceID_None, true}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGFECompositeElement)

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGFECompositeElement::In1() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGFECompositeElement::In1()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGFECompositeElement::In1() {
>>>>>>> upstream-releases
  return mStringAttributes[IN1].ToDOMAnimatedString(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGFECompositeElement::In2() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGFECompositeElement::In2()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGFECompositeElement::In2() {
>>>>>>> upstream-releases
  return mStringAttributes[IN2].ToDOMAnimatedString(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration> SVGFECompositeElement::Operator() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGFECompositeElement::Operator()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration> SVGFECompositeElement::Operator() {
>>>>>>> upstream-releases
  return mEnumAttributes[OPERATOR].ToDOMAnimatedEnum(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFECompositeElement::K1() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFECompositeElement::K1()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFECompositeElement::K1() {
>>>>>>> upstream-releases
  return mNumberAttributes[ATTR_K1].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFECompositeElement::K2() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFECompositeElement::K2()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFECompositeElement::K2() {
>>>>>>> upstream-releases
  return mNumberAttributes[ATTR_K2].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFECompositeElement::K3() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFECompositeElement::K3()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFECompositeElement::K3() {
>>>>>>> upstream-releases
  return mNumberAttributes[ATTR_K3].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFECompositeElement::K4() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFECompositeElement::K4()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFECompositeElement::K4() {
>>>>>>> upstream-releases
  return mNumberAttributes[ATTR_K4].ToDOMAnimatedNumber(this);
}

void SVGFECompositeElement::SetK(float k1, float k2, float k3, float k4) {
  mNumberAttributes[ATTR_K1].SetBaseValue(k1, this);
  mNumberAttributes[ATTR_K2].SetBaseValue(k2, this);
  mNumberAttributes[ATTR_K3].SetBaseValue(k3, this);
  mNumberAttributes[ATTR_K4].SetBaseValue(k4, this);
}

FilterPrimitiveDescription SVGFECompositeElement::GetPrimitiveDescription(
    nsSVGFilterInstance* aInstance, const IntRect& aFilterSubregion,
    const nsTArray<bool>& aInputsAreTainted,
    nsTArray<RefPtr<SourceSurface>>& aInputImages) {
  CompositeAttributes atts;
  uint32_t op = mEnumAttributes[OPERATOR].GetAnimValue();
  atts.mOperator = op;

  if (op == SVG_FECOMPOSITE_OPERATOR_ARITHMETIC) {
    float k[4];
    GetAnimatedNumberValues(k, k + 1, k + 2, k + 3, nullptr);
    atts.mCoefficients.AppendElements(k, 4);
  }

  return FilterPrimitiveDescription(AsVariant(std::move(atts)));
}

bool SVGFECompositeElement::AttributeAffectsRendering(
    int32_t aNameSpaceID, nsAtom* aAttribute) const {
  return SVGFECompositeElementBase::AttributeAffectsRendering(aNameSpaceID,
                                                              aAttribute) ||
         (aNameSpaceID == kNameSpaceID_None &&
          (aAttribute == nsGkAtoms::in || aAttribute == nsGkAtoms::in2 ||
           aAttribute == nsGkAtoms::k1 || aAttribute == nsGkAtoms::k2 ||
           aAttribute == nsGkAtoms::k3 || aAttribute == nsGkAtoms::k4 ||
           aAttribute == nsGkAtoms::_operator));
}

<<<<<<< HEAD
void SVGFECompositeElement::GetSourceImageNames(
    nsTArray<nsSVGStringInfo>& aSources) {
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN2], this));
||||||| merged common ancestors
void
SVGFECompositeElement::GetSourceImageNames(nsTArray<nsSVGStringInfo>& aSources)
{
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN2], this));
=======
void SVGFECompositeElement::GetSourceImageNames(
    nsTArray<SVGStringInfo>& aSources) {
  aSources.AppendElement(SVGStringInfo(&mStringAttributes[IN1], this));
  aSources.AppendElement(SVGStringInfo(&mStringAttributes[IN2], this));
>>>>>>> upstream-releases
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::NumberAttributesInfo SVGFECompositeElement::GetNumberInfo() {
||||||| merged common ancestors
nsSVGElement::NumberAttributesInfo
SVGFECompositeElement::GetNumberInfo()
{
=======
SVGElement::NumberAttributesInfo SVGFECompositeElement::GetNumberInfo() {
>>>>>>> upstream-releases
  return NumberAttributesInfo(mNumberAttributes, sNumberInfo,
                              ArrayLength(sNumberInfo));
}

<<<<<<< HEAD
nsSVGElement::EnumAttributesInfo SVGFECompositeElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
||||||| merged common ancestors
nsSVGElement::EnumAttributesInfo
SVGFECompositeElement::GetEnumInfo()
{
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo,
                            ArrayLength(sEnumInfo));
=======
SVGElement::EnumAttributesInfo SVGFECompositeElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsSVGElement::StringAttributesInfo SVGFECompositeElement::GetStringInfo() {
||||||| merged common ancestors
nsSVGElement::StringAttributesInfo
SVGFECompositeElement::GetStringInfo()
{
=======
SVGElement::StringAttributesInfo SVGFECompositeElement::GetStringInfo() {
>>>>>>> upstream-releases
  return StringAttributesInfo(mStringAttributes, sStringInfo,
                              ArrayLength(sStringInfo));
}

}  // namespace dom
}  // namespace mozilla
