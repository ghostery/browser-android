/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGMaskElement.h"

#include "nsGkAtoms.h"
#include "mozilla/AlreadyAddRefed.h"
#include "mozilla/ArrayUtils.h"
#include "mozilla/dom/SVGLengthBinding.h"
#include "mozilla/dom/SVGMaskElementBinding.h"
#include "mozilla/dom/SVGUnitTypesBinding.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(Mask)

namespace mozilla {
namespace dom {

using namespace SVGUnitTypes_Binding;

JSObject* SVGMaskElement::WrapNode(JSContext* aCx,
                                   JS::Handle<JSObject*> aGivenProto) {
  return SVGMaskElement_Binding::Wrap(aCx, this, aGivenProto);
}

//--------------------- Masks ------------------------

<<<<<<< HEAD
nsSVGElement::LengthInfo SVGMaskElement::sLengthInfo[4] = {
    {nsGkAtoms::x, -10, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::y, -10, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
    {nsGkAtoms::width, 120, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::height, 120, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
||||||| merged common ancestors
nsSVGElement::LengthInfo SVGMaskElement::sLengthInfo[4] =
{
  { nsGkAtoms::x, -10, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::X },
  { nsGkAtoms::y, -10, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::Y },
  { nsGkAtoms::width, 120, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::X },
  { nsGkAtoms::height, 120, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::Y },
=======
SVGElement::LengthInfo SVGMaskElement::sLengthInfo[4] = {
    {nsGkAtoms::x, -10, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::y, -10, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
    {nsGkAtoms::width, 120, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::height, 120, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
>>>>>>> upstream-releases
};

<<<<<<< HEAD
nsSVGElement::EnumInfo SVGMaskElement::sEnumInfo[2] = {
    {nsGkAtoms::maskUnits, sSVGUnitTypesMap, SVG_UNIT_TYPE_OBJECTBOUNDINGBOX},
    {nsGkAtoms::maskContentUnits, sSVGUnitTypesMap,
     SVG_UNIT_TYPE_USERSPACEONUSE}};
||||||| merged common ancestors
nsSVGElement::EnumInfo SVGMaskElement::sEnumInfo[2] =
{
  { nsGkAtoms::maskUnits,
    sSVGUnitTypesMap,
    SVG_UNIT_TYPE_OBJECTBOUNDINGBOX
  },
  { nsGkAtoms::maskContentUnits,
    sSVGUnitTypesMap,
    SVG_UNIT_TYPE_USERSPACEONUSE
  }
};
=======
SVGElement::EnumInfo SVGMaskElement::sEnumInfo[2] = {
    {nsGkAtoms::maskUnits, sSVGUnitTypesMap, SVG_UNIT_TYPE_OBJECTBOUNDINGBOX},
    {nsGkAtoms::maskContentUnits, sSVGUnitTypesMap,
     SVG_UNIT_TYPE_USERSPACEONUSE}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// Implementation

SVGMaskElement::SVGMaskElement(
    already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGMaskElementBase(std::move(aNodeInfo)) {}

//----------------------------------------------------------------------
// nsINode method

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGMaskElement)

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration> SVGMaskElement::MaskUnits() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGMaskElement::MaskUnits()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration> SVGMaskElement::MaskUnits() {
>>>>>>> upstream-releases
  return mEnumAttributes[MASKUNITS].ToDOMAnimatedEnum(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration> SVGMaskElement::MaskContentUnits() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGMaskElement::MaskContentUnits()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration> SVGMaskElement::MaskContentUnits() {
>>>>>>> upstream-releases
  return mEnumAttributes[MASKCONTENTUNITS].ToDOMAnimatedEnum(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGMaskElement::X() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGMaskElement::X()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGMaskElement::X() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_X].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGMaskElement::Y() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGMaskElement::Y()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGMaskElement::Y() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_Y].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGMaskElement::Width() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGMaskElement::Width()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGMaskElement::Width() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_WIDTH].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGMaskElement::Height() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGMaskElement::Height()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGMaskElement::Height() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_HEIGHT].ToDOMAnimatedLength(this);
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
/* virtual */ bool SVGMaskElement::HasValidDimensions() const {
||||||| merged common ancestors
/* virtual */ bool
SVGMaskElement::HasValidDimensions() const
{
=======
/* virtual */
bool SVGMaskElement::HasValidDimensions() const {
>>>>>>> upstream-releases
  return (!mLengthAttributes[ATTR_WIDTH].IsExplicitlySet() ||
          mLengthAttributes[ATTR_WIDTH].GetAnimValInSpecifiedUnits() > 0) &&
         (!mLengthAttributes[ATTR_HEIGHT].IsExplicitlySet() ||
          mLengthAttributes[ATTR_HEIGHT].GetAnimValInSpecifiedUnits() > 0);
}

<<<<<<< HEAD
nsSVGElement::LengthAttributesInfo SVGMaskElement::GetLengthInfo() {
||||||| merged common ancestors
nsSVGElement::LengthAttributesInfo
SVGMaskElement::GetLengthInfo()
{
=======
SVGElement::LengthAttributesInfo SVGMaskElement::GetLengthInfo() {
>>>>>>> upstream-releases
  return LengthAttributesInfo(mLengthAttributes, sLengthInfo,
                              ArrayLength(sLengthInfo));
}

<<<<<<< HEAD
nsSVGElement::EnumAttributesInfo SVGMaskElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
||||||| merged common ancestors
nsSVGElement::EnumAttributesInfo
SVGMaskElement::GetEnumInfo()
{
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo,
                            ArrayLength(sEnumInfo));
=======
SVGElement::EnumAttributesInfo SVGMaskElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
>>>>>>> upstream-releases
}

//----------------------------------------------------------------------
// nsIContent methods

NS_IMETHODIMP_(bool)
SVGMaskElement::IsAttributeMapped(const nsAtom* name) const {
  static const MappedAttributeEntry* const map[] = {sColorMap,
                                                    sFEFloodMap,
                                                    sFillStrokeMap,
                                                    sFiltersMap,
                                                    sFontSpecificationMap,
                                                    sGradientStopMap,
                                                    sGraphicsMap,
                                                    sMarkersMap,
                                                    sMaskMap,
                                                    sTextContentElementsMap,
                                                    sViewportsMap};

  return FindAttributeDependence(name, map) ||
         SVGMaskElementBase::IsAttributeMapped(name);
}

}  // namespace dom
}  // namespace mozilla
