/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGFEBlendElement.h"
#include "mozilla/dom/SVGFEBlendElementBinding.h"
#include "nsSVGUtils.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(FEBlend)

using namespace mozilla::gfx;

namespace mozilla {
namespace dom {

JSObject* SVGFEBlendElement::WrapNode(JSContext* aCx,
                                      JS::Handle<JSObject*> aGivenProto) {
  return SVGFEBlendElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGEnumMapping SVGFEBlendElement::sModeMap[] = {
    {nsGkAtoms::normal, SVG_FEBLEND_MODE_NORMAL},
    {nsGkAtoms::multiply, SVG_FEBLEND_MODE_MULTIPLY},
    {nsGkAtoms::screen, SVG_FEBLEND_MODE_SCREEN},
    {nsGkAtoms::darken, SVG_FEBLEND_MODE_DARKEN},
    {nsGkAtoms::lighten, SVG_FEBLEND_MODE_LIGHTEN},
    {nsGkAtoms::overlay, SVG_FEBLEND_MODE_OVERLAY},
    {nsGkAtoms::colorDodge, SVG_FEBLEND_MODE_COLOR_DODGE},
    {nsGkAtoms::colorBurn, SVG_FEBLEND_MODE_COLOR_BURN},
    {nsGkAtoms::hardLight, SVG_FEBLEND_MODE_HARD_LIGHT},
    {nsGkAtoms::softLight, SVG_FEBLEND_MODE_SOFT_LIGHT},
    {nsGkAtoms::difference, SVG_FEBLEND_MODE_DIFFERENCE},
    {nsGkAtoms::exclusion, SVG_FEBLEND_MODE_EXCLUSION},
    {nsGkAtoms::hue, SVG_FEBLEND_MODE_HUE},
    {nsGkAtoms::saturation, SVG_FEBLEND_MODE_SATURATION},
    {nsGkAtoms::color, SVG_FEBLEND_MODE_COLOR},
    {nsGkAtoms::luminosity, SVG_FEBLEND_MODE_LUMINOSITY},
    {nullptr, 0}};

nsSVGElement::EnumInfo SVGFEBlendElement::sEnumInfo[1] = {
    {nsGkAtoms::mode, sModeMap, SVG_FEBLEND_MODE_NORMAL}};

nsSVGElement::StringInfo SVGFEBlendElement::sStringInfo[3] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true},
    {nsGkAtoms::in2, kNameSpaceID_None, true}};
||||||| merged common ancestors
nsSVGEnumMapping SVGFEBlendElement::sModeMap[] = {
  {nsGkAtoms::normal, SVG_FEBLEND_MODE_NORMAL},
  {nsGkAtoms::multiply, SVG_FEBLEND_MODE_MULTIPLY},
  {nsGkAtoms::screen, SVG_FEBLEND_MODE_SCREEN},
  {nsGkAtoms::darken, SVG_FEBLEND_MODE_DARKEN},
  {nsGkAtoms::lighten, SVG_FEBLEND_MODE_LIGHTEN},
  {nsGkAtoms::overlay, SVG_FEBLEND_MODE_OVERLAY},
  {nsGkAtoms::colorDodge, SVG_FEBLEND_MODE_COLOR_DODGE},
  {nsGkAtoms::colorBurn, SVG_FEBLEND_MODE_COLOR_BURN},
  {nsGkAtoms::hardLight, SVG_FEBLEND_MODE_HARD_LIGHT},
  {nsGkAtoms::softLight, SVG_FEBLEND_MODE_SOFT_LIGHT},
  {nsGkAtoms::difference, SVG_FEBLEND_MODE_DIFFERENCE},
  {nsGkAtoms::exclusion, SVG_FEBLEND_MODE_EXCLUSION},
  {nsGkAtoms::hue, SVG_FEBLEND_MODE_HUE},
  {nsGkAtoms::saturation, SVG_FEBLEND_MODE_SATURATION},
  {nsGkAtoms::color, SVG_FEBLEND_MODE_COLOR},
  {nsGkAtoms::luminosity, SVG_FEBLEND_MODE_LUMINOSITY},
  {nullptr, 0}
};

nsSVGElement::EnumInfo SVGFEBlendElement::sEnumInfo[1] =
{
  { nsGkAtoms::mode,
    sModeMap,
    SVG_FEBLEND_MODE_NORMAL
  }
};

nsSVGElement::StringInfo SVGFEBlendElement::sStringInfo[3] =
{
  { nsGkAtoms::result, kNameSpaceID_None, true },
  { nsGkAtoms::in, kNameSpaceID_None, true },
  { nsGkAtoms::in2, kNameSpaceID_None, true }
};
=======
SVGEnumMapping SVGFEBlendElement::sModeMap[] = {
    {nsGkAtoms::normal, SVG_FEBLEND_MODE_NORMAL},
    {nsGkAtoms::multiply, SVG_FEBLEND_MODE_MULTIPLY},
    {nsGkAtoms::screen, SVG_FEBLEND_MODE_SCREEN},
    {nsGkAtoms::darken, SVG_FEBLEND_MODE_DARKEN},
    {nsGkAtoms::lighten, SVG_FEBLEND_MODE_LIGHTEN},
    {nsGkAtoms::overlay, SVG_FEBLEND_MODE_OVERLAY},
    {nsGkAtoms::colorDodge, SVG_FEBLEND_MODE_COLOR_DODGE},
    {nsGkAtoms::colorBurn, SVG_FEBLEND_MODE_COLOR_BURN},
    {nsGkAtoms::hardLight, SVG_FEBLEND_MODE_HARD_LIGHT},
    {nsGkAtoms::softLight, SVG_FEBLEND_MODE_SOFT_LIGHT},
    {nsGkAtoms::difference, SVG_FEBLEND_MODE_DIFFERENCE},
    {nsGkAtoms::exclusion, SVG_FEBLEND_MODE_EXCLUSION},
    {nsGkAtoms::hue, SVG_FEBLEND_MODE_HUE},
    {nsGkAtoms::saturation, SVG_FEBLEND_MODE_SATURATION},
    {nsGkAtoms::color, SVG_FEBLEND_MODE_COLOR},
    {nsGkAtoms::luminosity, SVG_FEBLEND_MODE_LUMINOSITY},
    {nullptr, 0}};

SVGElement::EnumInfo SVGFEBlendElement::sEnumInfo[1] = {
    {nsGkAtoms::mode, sModeMap, SVG_FEBLEND_MODE_NORMAL}};

SVGElement::StringInfo SVGFEBlendElement::sStringInfo[3] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true},
    {nsGkAtoms::in2, kNameSpaceID_None, true}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGFEBlendElement)

//----------------------------------------------------------------------
// nsIDOMSVGFEBlendElement methods

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGFEBlendElement::In1() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGFEBlendElement::In1()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGFEBlendElement::In1() {
>>>>>>> upstream-releases
  return mStringAttributes[IN1].ToDOMAnimatedString(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGFEBlendElement::In2() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGFEBlendElement::In2()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGFEBlendElement::In2() {
>>>>>>> upstream-releases
  return mStringAttributes[IN2].ToDOMAnimatedString(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration> SVGFEBlendElement::Mode() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGFEBlendElement::Mode()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration> SVGFEBlendElement::Mode() {
>>>>>>> upstream-releases
  return mEnumAttributes[MODE].ToDOMAnimatedEnum(this);
}

FilterPrimitiveDescription SVGFEBlendElement::GetPrimitiveDescription(
    nsSVGFilterInstance* aInstance, const IntRect& aFilterSubregion,
    const nsTArray<bool>& aInputsAreTainted,
    nsTArray<RefPtr<SourceSurface>>& aInputImages) {
  uint32_t mode = mEnumAttributes[MODE].GetAnimValue();
  BlendAttributes attributes;
  attributes.mBlendMode = mode;
  return FilterPrimitiveDescription(AsVariant(std::move(attributes)));
}

bool SVGFEBlendElement::AttributeAffectsRendering(int32_t aNameSpaceID,
                                                  nsAtom* aAttribute) const {
  return SVGFEBlendElementBase::AttributeAffectsRendering(aNameSpaceID,
                                                          aAttribute) ||
         (aNameSpaceID == kNameSpaceID_None &&
          (aAttribute == nsGkAtoms::in || aAttribute == nsGkAtoms::in2 ||
           aAttribute == nsGkAtoms::mode));
}

<<<<<<< HEAD
void SVGFEBlendElement::GetSourceImageNames(
    nsTArray<nsSVGStringInfo>& aSources) {
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN2], this));
||||||| merged common ancestors
void
SVGFEBlendElement::GetSourceImageNames(nsTArray<nsSVGStringInfo>& aSources)
{
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN2], this));
=======
void SVGFEBlendElement::GetSourceImageNames(nsTArray<SVGStringInfo>& aSources) {
  aSources.AppendElement(SVGStringInfo(&mStringAttributes[IN1], this));
  aSources.AppendElement(SVGStringInfo(&mStringAttributes[IN2], this));
>>>>>>> upstream-releases
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::EnumAttributesInfo SVGFEBlendElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
||||||| merged common ancestors
nsSVGElement::EnumAttributesInfo
SVGFEBlendElement::GetEnumInfo()
{
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo,
                            ArrayLength(sEnumInfo));
=======
SVGElement::EnumAttributesInfo SVGFEBlendElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsSVGElement::StringAttributesInfo SVGFEBlendElement::GetStringInfo() {
||||||| merged common ancestors
nsSVGElement::StringAttributesInfo
SVGFEBlendElement::GetStringInfo()
{
=======
SVGElement::StringAttributesInfo SVGFEBlendElement::GetStringInfo() {
>>>>>>> upstream-releases
  return StringAttributesInfo(mStringAttributes, sStringInfo,
                              ArrayLength(sStringInfo));
}

}  // namespace dom
}  // namespace mozilla
