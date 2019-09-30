/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGFEDisplacementMapElement.h"
#include "mozilla/dom/SVGFEDisplacementMapElementBinding.h"
#include "nsSVGFilterInstance.h"
#include "nsSVGUtils.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(FEDisplacementMap)

using namespace mozilla::gfx;

namespace mozilla {
namespace dom {

JSObject* SVGFEDisplacementMapElement::WrapNode(
    JSContext* aCx, JS::Handle<JSObject*> aGivenProto) {
  return SVGFEDisplacementMapElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::NumberInfo SVGFEDisplacementMapElement::sNumberInfo[1] = {
    {nsGkAtoms::scale, 0, false},
||||||| merged common ancestors
nsSVGElement::NumberInfo SVGFEDisplacementMapElement::sNumberInfo[1] =
{
  { nsGkAtoms::scale, 0, false },
=======
SVGElement::NumberInfo SVGFEDisplacementMapElement::sNumberInfo[1] = {
    {nsGkAtoms::scale, 0, false},
>>>>>>> upstream-releases
};

<<<<<<< HEAD
nsSVGEnumMapping SVGFEDisplacementMapElement::sChannelMap[] = {
    {nsGkAtoms::R, SVG_CHANNEL_R},
    {nsGkAtoms::G, SVG_CHANNEL_G},
    {nsGkAtoms::B, SVG_CHANNEL_B},
    {nsGkAtoms::A, SVG_CHANNEL_A},
    {nullptr, 0}};
||||||| merged common ancestors
nsSVGEnumMapping SVGFEDisplacementMapElement::sChannelMap[] = {
  {nsGkAtoms::R, SVG_CHANNEL_R},
  {nsGkAtoms::G, SVG_CHANNEL_G},
  {nsGkAtoms::B, SVG_CHANNEL_B},
  {nsGkAtoms::A, SVG_CHANNEL_A},
  {nullptr, 0}
};
=======
SVGEnumMapping SVGFEDisplacementMapElement::sChannelMap[] = {
    {nsGkAtoms::R, SVG_CHANNEL_R},
    {nsGkAtoms::G, SVG_CHANNEL_G},
    {nsGkAtoms::B, SVG_CHANNEL_B},
    {nsGkAtoms::A, SVG_CHANNEL_A},
    {nullptr, 0}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::EnumInfo SVGFEDisplacementMapElement::sEnumInfo[2] = {
    {nsGkAtoms::xChannelSelector, sChannelMap, SVG_CHANNEL_A},
    {nsGkAtoms::yChannelSelector, sChannelMap, SVG_CHANNEL_A}};
||||||| merged common ancestors
nsSVGElement::EnumInfo SVGFEDisplacementMapElement::sEnumInfo[2] =
{
  { nsGkAtoms::xChannelSelector,
    sChannelMap,
    SVG_CHANNEL_A
  },
  { nsGkAtoms::yChannelSelector,
    sChannelMap,
    SVG_CHANNEL_A
  }
};
=======
SVGElement::EnumInfo SVGFEDisplacementMapElement::sEnumInfo[2] = {
    {nsGkAtoms::xChannelSelector, sChannelMap, SVG_CHANNEL_A},
    {nsGkAtoms::yChannelSelector, sChannelMap, SVG_CHANNEL_A}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::StringInfo SVGFEDisplacementMapElement::sStringInfo[3] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true},
    {nsGkAtoms::in2, kNameSpaceID_None, true}};
||||||| merged common ancestors
nsSVGElement::StringInfo SVGFEDisplacementMapElement::sStringInfo[3] =
{
  { nsGkAtoms::result, kNameSpaceID_None, true },
  { nsGkAtoms::in, kNameSpaceID_None, true },
  { nsGkAtoms::in2, kNameSpaceID_None, true }
};
=======
SVGElement::StringInfo SVGFEDisplacementMapElement::sStringInfo[3] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true},
    {nsGkAtoms::in2, kNameSpaceID_None, true}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGFEDisplacementMapElement)

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGFEDisplacementMapElement::In1() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGFEDisplacementMapElement::In1()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGFEDisplacementMapElement::In1() {
>>>>>>> upstream-releases
  return mStringAttributes[IN1].ToDOMAnimatedString(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGFEDisplacementMapElement::In2() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGFEDisplacementMapElement::In2()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGFEDisplacementMapElement::In2() {
>>>>>>> upstream-releases
  return mStringAttributes[IN2].ToDOMAnimatedString(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFEDisplacementMapElement::Scale() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEDisplacementMapElement::Scale()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFEDisplacementMapElement::Scale() {
>>>>>>> upstream-releases
  return mNumberAttributes[SCALE].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration>
SVGFEDisplacementMapElement::XChannelSelector() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGFEDisplacementMapElement::XChannelSelector()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration>
SVGFEDisplacementMapElement::XChannelSelector() {
>>>>>>> upstream-releases
  return mEnumAttributes[CHANNEL_X].ToDOMAnimatedEnum(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration>
SVGFEDisplacementMapElement::YChannelSelector() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGFEDisplacementMapElement::YChannelSelector()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration>
SVGFEDisplacementMapElement::YChannelSelector() {
>>>>>>> upstream-releases
  return mEnumAttributes[CHANNEL_Y].ToDOMAnimatedEnum(this);
}

FilterPrimitiveDescription SVGFEDisplacementMapElement::GetPrimitiveDescription(
    nsSVGFilterInstance* aInstance, const IntRect& aFilterSubregion,
    const nsTArray<bool>& aInputsAreTainted,
    nsTArray<RefPtr<SourceSurface>>& aInputImages) {
  if (aInputsAreTainted[1]) {
    // If the map is tainted, refuse to apply the effect and act as a
    // pass-through filter instead, as required by the spec.
    OffsetAttributes atts;
    atts.mValue = IntPoint(0, 0);
    return FilterPrimitiveDescription(AsVariant(std::move(atts)));
  }

  float scale = aInstance->GetPrimitiveNumber(SVGContentUtils::XY,
                                              &mNumberAttributes[SCALE]);
  uint32_t xChannel = mEnumAttributes[CHANNEL_X].GetAnimValue();
  uint32_t yChannel = mEnumAttributes[CHANNEL_Y].GetAnimValue();
  DisplacementMapAttributes atts;
  atts.mScale = scale;
  atts.mXChannel = xChannel;
  atts.mYChannel = yChannel;
  return FilterPrimitiveDescription(AsVariant(std::move(atts)));
}

bool SVGFEDisplacementMapElement::AttributeAffectsRendering(
    int32_t aNameSpaceID, nsAtom* aAttribute) const {
  return SVGFEDisplacementMapElementBase::AttributeAffectsRendering(
             aNameSpaceID, aAttribute) ||
         (aNameSpaceID == kNameSpaceID_None &&
          (aAttribute == nsGkAtoms::in || aAttribute == nsGkAtoms::in2 ||
           aAttribute == nsGkAtoms::scale ||
           aAttribute == nsGkAtoms::xChannelSelector ||
           aAttribute == nsGkAtoms::yChannelSelector));
}

<<<<<<< HEAD
void SVGFEDisplacementMapElement::GetSourceImageNames(
    nsTArray<nsSVGStringInfo>& aSources) {
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN2], this));
||||||| merged common ancestors
void
SVGFEDisplacementMapElement::GetSourceImageNames(nsTArray<nsSVGStringInfo>& aSources)
{
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN2], this));
=======
void SVGFEDisplacementMapElement::GetSourceImageNames(
    nsTArray<SVGStringInfo>& aSources) {
  aSources.AppendElement(SVGStringInfo(&mStringAttributes[IN1], this));
  aSources.AppendElement(SVGStringInfo(&mStringAttributes[IN2], this));
>>>>>>> upstream-releases
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::NumberAttributesInfo
SVGFEDisplacementMapElement::GetNumberInfo() {
||||||| merged common ancestors
nsSVGElement::NumberAttributesInfo
SVGFEDisplacementMapElement::GetNumberInfo()
{
=======
SVGElement::NumberAttributesInfo SVGFEDisplacementMapElement::GetNumberInfo() {
>>>>>>> upstream-releases
  return NumberAttributesInfo(mNumberAttributes, sNumberInfo,
                              ArrayLength(sNumberInfo));
}

<<<<<<< HEAD
nsSVGElement::EnumAttributesInfo SVGFEDisplacementMapElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
||||||| merged common ancestors
nsSVGElement::EnumAttributesInfo
SVGFEDisplacementMapElement::GetEnumInfo()
{
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo,
                            ArrayLength(sEnumInfo));
=======
SVGElement::EnumAttributesInfo SVGFEDisplacementMapElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsSVGElement::StringAttributesInfo
SVGFEDisplacementMapElement::GetStringInfo() {
||||||| merged common ancestors
nsSVGElement::StringAttributesInfo
SVGFEDisplacementMapElement::GetStringInfo()
{
=======
SVGElement::StringAttributesInfo SVGFEDisplacementMapElement::GetStringInfo() {
>>>>>>> upstream-releases
  return StringAttributesInfo(mStringAttributes, sStringInfo,
                              ArrayLength(sStringInfo));
}

}  // namespace dom
}  // namespace mozilla
