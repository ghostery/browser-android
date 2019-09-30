/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGFEDiffuseLightingElement.h"
#include "mozilla/dom/SVGFEDiffuseLightingElementBinding.h"
#include "nsSVGUtils.h"
#include "nsSVGFilterInstance.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(FEDiffuseLighting)

using namespace mozilla::gfx;

namespace mozilla {
namespace dom {

JSObject* SVGFEDiffuseLightingElement::WrapNode(
    JSContext* aCx, JS::Handle<JSObject*> aGivenProto) {
  return SVGFEDiffuseLightingElement_Binding::Wrap(aCx, this, aGivenProto);
}

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGFEDiffuseLightingElement)

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGFEDiffuseLightingElement::In1() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGFEDiffuseLightingElement::In1()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGFEDiffuseLightingElement::In1() {
>>>>>>> upstream-releases
  return mStringAttributes[IN1].ToDOMAnimatedString(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber>
SVGFEDiffuseLightingElement::SurfaceScale() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEDiffuseLightingElement::SurfaceScale()
{
=======
already_AddRefed<DOMSVGAnimatedNumber>
SVGFEDiffuseLightingElement::SurfaceScale() {
>>>>>>> upstream-releases
  return mNumberAttributes[SURFACE_SCALE].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber>
SVGFEDiffuseLightingElement::DiffuseConstant() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEDiffuseLightingElement::DiffuseConstant()
{
=======
already_AddRefed<DOMSVGAnimatedNumber>
SVGFEDiffuseLightingElement::DiffuseConstant() {
>>>>>>> upstream-releases
  return mNumberAttributes[DIFFUSE_CONSTANT].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber>
SVGFEDiffuseLightingElement::KernelUnitLengthX() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEDiffuseLightingElement::KernelUnitLengthX()
{
=======
already_AddRefed<DOMSVGAnimatedNumber>
SVGFEDiffuseLightingElement::KernelUnitLengthX() {
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
SVGFEDiffuseLightingElement::KernelUnitLengthY() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEDiffuseLightingElement::KernelUnitLengthY()
{
=======
already_AddRefed<DOMSVGAnimatedNumber>
SVGFEDiffuseLightingElement::KernelUnitLengthY() {
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

FilterPrimitiveDescription SVGFEDiffuseLightingElement::GetPrimitiveDescription(
    nsSVGFilterInstance* aInstance, const IntRect& aFilterSubregion,
    const nsTArray<bool>& aInputsAreTainted,
    nsTArray<RefPtr<SourceSurface>>& aInputImages) {
  float diffuseConstant = mNumberAttributes[DIFFUSE_CONSTANT].GetAnimValue();

  DiffuseLightingAttributes atts;
  atts.mLightingConstant = diffuseConstant;
  if (!AddLightingAttributes(&atts, aInstance)) {
    return FilterPrimitiveDescription();
  }

  return FilterPrimitiveDescription(AsVariant(std::move(atts)));
}

bool SVGFEDiffuseLightingElement::AttributeAffectsRendering(
    int32_t aNameSpaceID, nsAtom* aAttribute) const {
  return SVGFEDiffuseLightingElementBase::AttributeAffectsRendering(
             aNameSpaceID, aAttribute) ||
         (aNameSpaceID == kNameSpaceID_None &&
          aAttribute == nsGkAtoms::diffuseConstant);
}

}  // namespace dom
}  // namespace mozilla
