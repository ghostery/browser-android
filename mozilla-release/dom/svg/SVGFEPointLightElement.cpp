/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGFEPointLightElement.h"
#include "mozilla/dom/SVGFEPointLightElementBinding.h"
#include "nsSVGFilterInstance.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(FEPointLight)

using namespace mozilla::gfx;

namespace mozilla {
namespace dom {

JSObject* SVGFEPointLightElement::WrapNode(JSContext* aCx,
                                           JS::Handle<JSObject*> aGivenProto) {
  return SVGFEPointLightElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::NumberInfo SVGFEPointLightElement::sNumberInfo[3] = {
    {nsGkAtoms::x, 0, false},
    {nsGkAtoms::y, 0, false},
    {nsGkAtoms::z, 0, false}};
||||||| merged common ancestors
nsSVGElement::NumberInfo SVGFEPointLightElement::sNumberInfo[3] =
{
  { nsGkAtoms::x, 0, false },
  { nsGkAtoms::y, 0, false },
  { nsGkAtoms::z, 0, false }
};
=======
SVGElement::NumberInfo SVGFEPointLightElement::sNumberInfo[3] = {
    {nsGkAtoms::x, 0, false},
    {nsGkAtoms::y, 0, false},
    {nsGkAtoms::z, 0, false}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGFEPointLightElement)

//----------------------------------------------------------------------
// nsFEUnstyledElement methods

bool SVGFEPointLightElement::AttributeAffectsRendering(
    int32_t aNameSpaceID, nsAtom* aAttribute) const {
  return aNameSpaceID == kNameSpaceID_None &&
         (aAttribute == nsGkAtoms::x || aAttribute == nsGkAtoms::y ||
          aAttribute == nsGkAtoms::z);
}

//----------------------------------------------------------------------

LightType SVGFEPointLightElement::ComputeLightAttributes(
    nsSVGFilterInstance* aInstance, nsTArray<float>& aFloatAttributes) {
  Point3D lightPos;
  GetAnimatedNumberValues(&lightPos.x, &lightPos.y, &lightPos.z, nullptr);
  lightPos = aInstance->ConvertLocation(lightPos);
  aFloatAttributes.SetLength(kPointLightNumAttributes);
  aFloatAttributes[kPointLightPositionXIndex] = lightPos.x;
  aFloatAttributes[kPointLightPositionYIndex] = lightPos.y;
  aFloatAttributes[kPointLightPositionZIndex] = lightPos.z;
  return LightType::Point;
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFEPointLightElement::X() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEPointLightElement::X()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFEPointLightElement::X() {
>>>>>>> upstream-releases
  return mNumberAttributes[ATTR_X].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFEPointLightElement::Y() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEPointLightElement::Y()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFEPointLightElement::Y() {
>>>>>>> upstream-releases
  return mNumberAttributes[ATTR_Y].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFEPointLightElement::Z() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEPointLightElement::Z()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFEPointLightElement::Z() {
>>>>>>> upstream-releases
  return mNumberAttributes[ATTR_Z].ToDOMAnimatedNumber(this);
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::NumberAttributesInfo SVGFEPointLightElement::GetNumberInfo() {
||||||| merged common ancestors
nsSVGElement::NumberAttributesInfo
SVGFEPointLightElement::GetNumberInfo()
{
=======
SVGElement::NumberAttributesInfo SVGFEPointLightElement::GetNumberInfo() {
>>>>>>> upstream-releases
  return NumberAttributesInfo(mNumberAttributes, sNumberInfo,
                              ArrayLength(sNumberInfo));
}

}  // namespace dom
}  // namespace mozilla
