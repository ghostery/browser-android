/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGFESpotLightElement.h"
#include "mozilla/dom/SVGFESpotLightElementBinding.h"
#include "nsSVGFilterInstance.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(FESpotLight)

using namespace mozilla::gfx;

namespace mozilla {
namespace dom {

JSObject* SVGFESpotLightElement::WrapNode(JSContext* aCx,
                                          JS::Handle<JSObject*> aGivenProto) {
  return SVGFESpotLightElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::NumberInfo SVGFESpotLightElement::sNumberInfo[8] = {
    {nsGkAtoms::x, 0, false},
    {nsGkAtoms::y, 0, false},
    {nsGkAtoms::z, 0, false},
    {nsGkAtoms::pointsAtX, 0, false},
    {nsGkAtoms::pointsAtY, 0, false},
    {nsGkAtoms::pointsAtZ, 0, false},
    {nsGkAtoms::specularExponent, 1, false},
    {nsGkAtoms::limitingConeAngle, 0, false}};
||||||| merged common ancestors
nsSVGElement::NumberInfo SVGFESpotLightElement::sNumberInfo[8] =
{
  { nsGkAtoms::x, 0, false },
  { nsGkAtoms::y, 0, false },
  { nsGkAtoms::z, 0, false },
  { nsGkAtoms::pointsAtX, 0, false },
  { nsGkAtoms::pointsAtY, 0, false },
  { nsGkAtoms::pointsAtZ, 0, false },
  { nsGkAtoms::specularExponent, 1, false },
  { nsGkAtoms::limitingConeAngle, 0, false }
};
=======
SVGElement::NumberInfo SVGFESpotLightElement::sNumberInfo[8] = {
    {nsGkAtoms::x, 0, false},
    {nsGkAtoms::y, 0, false},
    {nsGkAtoms::z, 0, false},
    {nsGkAtoms::pointsAtX, 0, false},
    {nsGkAtoms::pointsAtY, 0, false},
    {nsGkAtoms::pointsAtZ, 0, false},
    {nsGkAtoms::specularExponent, 1, false},
    {nsGkAtoms::limitingConeAngle, 0, false}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGFESpotLightElement)

//----------------------------------------------------------------------
// nsFEUnstyledElement methods

bool SVGFESpotLightElement::AttributeAffectsRendering(
    int32_t aNameSpaceID, nsAtom* aAttribute) const {
  return aNameSpaceID == kNameSpaceID_None &&
         (aAttribute == nsGkAtoms::x || aAttribute == nsGkAtoms::y ||
          aAttribute == nsGkAtoms::z || aAttribute == nsGkAtoms::pointsAtX ||
          aAttribute == nsGkAtoms::pointsAtY ||
          aAttribute == nsGkAtoms::pointsAtZ ||
          aAttribute == nsGkAtoms::specularExponent ||
          aAttribute == nsGkAtoms::limitingConeAngle);
}

//----------------------------------------------------------------------

LightType SVGFESpotLightElement::ComputeLightAttributes(
    nsSVGFilterInstance* aInstance, nsTArray<float>& aFloatAttributes) {
  aFloatAttributes.SetLength(kSpotLightNumAttributes);
  GetAnimatedNumberValues(&aFloatAttributes[kSpotLightPositionXIndex],
                          &aFloatAttributes[kSpotLightPositionYIndex],
                          &aFloatAttributes[kSpotLightPositionZIndex],
                          &aFloatAttributes[kSpotLightPointsAtXIndex],
                          &aFloatAttributes[kSpotLightPointsAtYIndex],
                          &aFloatAttributes[kSpotLightPointsAtZIndex],
                          &aFloatAttributes[kSpotLightFocusIndex],
                          &aFloatAttributes[kSpotLightLimitingConeAngleIndex],
                          nullptr);
  if (!mNumberAttributes[SVGFESpotLightElement::LIMITING_CONE_ANGLE]
           .IsExplicitlySet()) {
    aFloatAttributes[kSpotLightLimitingConeAngleIndex] = 90;
  }

  return LightType::Spot;
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFESpotLightElement::X() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFESpotLightElement::X()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFESpotLightElement::X() {
>>>>>>> upstream-releases
  return mNumberAttributes[ATTR_X].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFESpotLightElement::Y() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFESpotLightElement::Y()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFESpotLightElement::Y() {
>>>>>>> upstream-releases
  return mNumberAttributes[ATTR_Y].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFESpotLightElement::Z() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFESpotLightElement::Z()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFESpotLightElement::Z() {
>>>>>>> upstream-releases
  return mNumberAttributes[ATTR_Z].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFESpotLightElement::PointsAtX() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFESpotLightElement::PointsAtX()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFESpotLightElement::PointsAtX() {
>>>>>>> upstream-releases
  return mNumberAttributes[POINTS_AT_X].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFESpotLightElement::PointsAtY() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFESpotLightElement::PointsAtY()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFESpotLightElement::PointsAtY() {
>>>>>>> upstream-releases
  return mNumberAttributes[POINTS_AT_Y].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFESpotLightElement::PointsAtZ() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFESpotLightElement::PointsAtZ()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFESpotLightElement::PointsAtZ() {
>>>>>>> upstream-releases
  return mNumberAttributes[POINTS_AT_Z].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFESpotLightElement::SpecularExponent() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFESpotLightElement::SpecularExponent()
{
=======
already_AddRefed<DOMSVGAnimatedNumber>
SVGFESpotLightElement::SpecularExponent() {
>>>>>>> upstream-releases
  return mNumberAttributes[SPECULAR_EXPONENT].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFESpotLightElement::LimitingConeAngle() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFESpotLightElement::LimitingConeAngle()
{
=======
already_AddRefed<DOMSVGAnimatedNumber>
SVGFESpotLightElement::LimitingConeAngle() {
>>>>>>> upstream-releases
  return mNumberAttributes[LIMITING_CONE_ANGLE].ToDOMAnimatedNumber(this);
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::NumberAttributesInfo SVGFESpotLightElement::GetNumberInfo() {
||||||| merged common ancestors
nsSVGElement::NumberAttributesInfo
SVGFESpotLightElement::GetNumberInfo()
{
=======
SVGElement::NumberAttributesInfo SVGFESpotLightElement::GetNumberInfo() {
>>>>>>> upstream-releases
  return NumberAttributesInfo(mNumberAttributes, sNumberInfo,
                              ArrayLength(sNumberInfo));
}

}  // namespace dom
}  // namespace mozilla
