/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGFESpecularLightingElement.h"
#include "mozilla/dom/SVGFESpecularLightingElementBinding.h"
#include "nsSVGUtils.h"
#include "nsSVGFilterInstance.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(FESpecularLighting)

using namespace mozilla::gfx;

namespace mozilla {
namespace dom {

JSObject* SVGFESpecularLightingElement::WrapNode(
    JSContext* aCx, JS::Handle<JSObject*> aGivenProto) {
  return SVGFESpecularLightingElement_Binding::Wrap(aCx, this, aGivenProto);
}

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGFESpecularLightingElement)

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGFESpecularLightingElement::In1() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGFESpecularLightingElement::In1()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGFESpecularLightingElement::In1() {
>>>>>>> upstream-releases
  return mStringAttributes[IN1].ToDOMAnimatedString(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber>
SVGFESpecularLightingElement::SurfaceScale() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFESpecularLightingElement::SurfaceScale()
{
=======
already_AddRefed<DOMSVGAnimatedNumber>
SVGFESpecularLightingElement::SurfaceScale() {
>>>>>>> upstream-releases
  return mNumberAttributes[SURFACE_SCALE].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber>
SVGFESpecularLightingElement::SpecularConstant() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFESpecularLightingElement::SpecularConstant()
{
=======
already_AddRefed<DOMSVGAnimatedNumber>
SVGFESpecularLightingElement::SpecularConstant() {
>>>>>>> upstream-releases
  return mNumberAttributes[SPECULAR_CONSTANT].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber>
SVGFESpecularLightingElement::SpecularExponent() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFESpecularLightingElement::SpecularExponent()
{
=======
already_AddRefed<DOMSVGAnimatedNumber>
SVGFESpecularLightingElement::SpecularExponent() {
>>>>>>> upstream-releases
  return mNumberAttributes[SPECULAR_EXPONENT].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber>
SVGFESpecularLightingElement::KernelUnitLengthX() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFESpecularLightingElement::KernelUnitLengthX()
{
=======
already_AddRefed<DOMSVGAnimatedNumber>
SVGFESpecularLightingElement::KernelUnitLengthX() {
>>>>>>> upstream-releases
  return mNumberPairAttributes[KERNEL_UNIT_LENGTH].ToDOMAnimatedNumber(
<<<<<<< HEAD
      nsSVGNumberPair::eFirst, this);
||||||| merged common ancestors
    nsSVGNumberPair::eFirst, this);
=======
      SVGAnimatedNumberPair::eFirst, this);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber>
SVGFESpecularLightingElement::KernelUnitLengthY() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFESpecularLightingElement::KernelUnitLengthY()
{
=======
already_AddRefed<DOMSVGAnimatedNumber>
SVGFESpecularLightingElement::KernelUnitLengthY() {
>>>>>>> upstream-releases
  return mNumberPairAttributes[KERNEL_UNIT_LENGTH].ToDOMAnimatedNumber(
<<<<<<< HEAD
      nsSVGNumberPair::eSecond, this);
||||||| merged common ancestors
    nsSVGNumberPair::eSecond, this);
=======
      SVGAnimatedNumberPair::eSecond, this);
>>>>>>> upstream-releases
}

//----------------------------------------------------------------------
// SVGElement methods

FilterPrimitiveDescription
SVGFESpecularLightingElement::GetPrimitiveDescription(
    nsSVGFilterInstance* aInstance, const IntRect& aFilterSubregion,
    const nsTArray<bool>& aInputsAreTainted,
    nsTArray<RefPtr<SourceSurface>>& aInputImages) {
  float specularExponent = mNumberAttributes[SPECULAR_EXPONENT].GetAnimValue();
  float specularConstant = mNumberAttributes[SPECULAR_CONSTANT].GetAnimValue();

  // specification defined range (15.22)
  if (specularExponent < 1 || specularExponent > 128) {
    return FilterPrimitiveDescription();
  }

  SpecularLightingAttributes atts;
  atts.mLightingConstant = specularConstant;
  atts.mSpecularExponent = specularExponent;
  if (!AddLightingAttributes(static_cast<DiffuseLightingAttributes*>(&atts),
                             aInstance)) {
    return FilterPrimitiveDescription();
  }

  return FilterPrimitiveDescription(AsVariant(std::move(atts)));
}

bool SVGFESpecularLightingElement::AttributeAffectsRendering(
    int32_t aNameSpaceID, nsAtom* aAttribute) const {
  return SVGFESpecularLightingElementBase::AttributeAffectsRendering(
             aNameSpaceID, aAttribute) ||
         (aNameSpaceID == kNameSpaceID_None &&
          (aAttribute == nsGkAtoms::specularConstant ||
           aAttribute == nsGkAtoms::specularExponent));
}

}  // namespace dom
}  // namespace mozilla
