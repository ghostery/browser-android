/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGFilterElement.h"

#include "nsGkAtoms.h"
#include "mozilla/AlreadyAddRefed.h"
#include "mozilla/ArrayUtils.h"
#include "mozilla/dom/SVGFilterElementBinding.h"
#include "mozilla/dom/SVGLengthBinding.h"
#include "mozilla/dom/SVGUnitTypesBinding.h"
#include "nsQueryObject.h"
#include "nsSVGUtils.h"
#include "SVGObserverUtils.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(Filter)

namespace mozilla {
namespace dom {

using namespace SVGUnitTypes_Binding;

JSObject* SVGFilterElement::WrapNode(JSContext* aCx,
                                     JS::Handle<JSObject*> aGivenProto) {
  return SVGFilterElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::LengthInfo SVGFilterElement::sLengthInfo[4] = {
    {nsGkAtoms::x, -10, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::y, -10, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
    {nsGkAtoms::width, 120, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::height, 120, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
||||||| merged common ancestors
nsSVGElement::LengthInfo SVGFilterElement::sLengthInfo[4] =
{
  { nsGkAtoms::x, -10, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::X },
  { nsGkAtoms::y, -10, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::Y },
  { nsGkAtoms::width, 120, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::X },
  { nsGkAtoms::height, 120, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::Y },
=======
SVGElement::LengthInfo SVGFilterElement::sLengthInfo[4] = {
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
nsSVGElement::EnumInfo SVGFilterElement::sEnumInfo[2] = {
    {nsGkAtoms::filterUnits, sSVGUnitTypesMap, SVG_UNIT_TYPE_OBJECTBOUNDINGBOX},
    {nsGkAtoms::primitiveUnits, sSVGUnitTypesMap,
     SVG_UNIT_TYPE_USERSPACEONUSE}};
||||||| merged common ancestors
nsSVGElement::EnumInfo SVGFilterElement::sEnumInfo[2] =
{
  { nsGkAtoms::filterUnits,
    sSVGUnitTypesMap,
    SVG_UNIT_TYPE_OBJECTBOUNDINGBOX
  },
  { nsGkAtoms::primitiveUnits,
    sSVGUnitTypesMap,
    SVG_UNIT_TYPE_USERSPACEONUSE
  }
};
=======
SVGElement::EnumInfo SVGFilterElement::sEnumInfo[2] = {
    {nsGkAtoms::filterUnits, sSVGUnitTypesMap, SVG_UNIT_TYPE_OBJECTBOUNDINGBOX},
    {nsGkAtoms::primitiveUnits, sSVGUnitTypesMap,
     SVG_UNIT_TYPE_USERSPACEONUSE}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::StringInfo SVGFilterElement::sStringInfo[2] = {
    {nsGkAtoms::href, kNameSpaceID_None, true},
    {nsGkAtoms::href, kNameSpaceID_XLink, true}};
||||||| merged common ancestors
nsSVGElement::StringInfo SVGFilterElement::sStringInfo[2] =
{
  { nsGkAtoms::href, kNameSpaceID_None, true },
  { nsGkAtoms::href, kNameSpaceID_XLink, true }
};
=======
SVGElement::StringInfo SVGFilterElement::sStringInfo[2] = {
    {nsGkAtoms::href, kNameSpaceID_None, true},
    {nsGkAtoms::href, kNameSpaceID_XLink, true}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// Implementation

SVGFilterElement::SVGFilterElement(
    already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGFilterElementBase(std::move(aNodeInfo)) {}

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGFilterElement)

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGFilterElement::X() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGFilterElement::X()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGFilterElement::X() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_X].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGFilterElement::Y() {
  return mLengthAttributes[ATTR_Y].ToDOMAnimatedLength(this);
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGFilterElement::Y()
{
   return mLengthAttributes[ATTR_Y].ToDOMAnimatedLength(this);
=======
already_AddRefed<DOMSVGAnimatedLength> SVGFilterElement::Y() {
  return mLengthAttributes[ATTR_Y].ToDOMAnimatedLength(this);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGFilterElement::Width() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGFilterElement::Width()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGFilterElement::Width() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_WIDTH].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGFilterElement::Height() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGFilterElement::Height()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGFilterElement::Height() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_HEIGHT].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration> SVGFilterElement::FilterUnits() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGFilterElement::FilterUnits()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration> SVGFilterElement::FilterUnits() {
>>>>>>> upstream-releases
  return mEnumAttributes[FILTERUNITS].ToDOMAnimatedEnum(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration> SVGFilterElement::PrimitiveUnits() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGFilterElement::PrimitiveUnits()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration> SVGFilterElement::PrimitiveUnits() {
>>>>>>> upstream-releases
  return mEnumAttributes[PRIMITIVEUNITS].ToDOMAnimatedEnum(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGFilterElement::Href() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGFilterElement::Href()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGFilterElement::Href() {
>>>>>>> upstream-releases
  return mStringAttributes[HREF].IsExplicitlySet()
             ? mStringAttributes[HREF].ToDOMAnimatedString(this)
             : mStringAttributes[XLINK_HREF].ToDOMAnimatedString(this);
}

//----------------------------------------------------------------------
// nsIContent methods

NS_IMETHODIMP_(bool)
SVGFilterElement::IsAttributeMapped(const nsAtom* name) const {
  static const MappedAttributeEntry* const map[] = {
      sColorMap,        sFEFloodMap,
      sFiltersMap,      sFontSpecificationMap,
      sGradientStopMap, sLightingEffectsMap,
      sMarkersMap,      sTextContentElementsMap,
      sViewportsMap};
  return FindAttributeDependence(name, map) ||
         SVGFilterElementBase::IsAttributeMapped(name);
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
/* virtual */ bool SVGFilterElement::HasValidDimensions() const {
||||||| merged common ancestors
/* virtual */ bool
SVGFilterElement::HasValidDimensions() const
{
=======
/* virtual */
bool SVGFilterElement::HasValidDimensions() const {
>>>>>>> upstream-releases
  return (!mLengthAttributes[ATTR_WIDTH].IsExplicitlySet() ||
          mLengthAttributes[ATTR_WIDTH].GetAnimValInSpecifiedUnits() > 0) &&
         (!mLengthAttributes[ATTR_HEIGHT].IsExplicitlySet() ||
          mLengthAttributes[ATTR_HEIGHT].GetAnimValInSpecifiedUnits() > 0);
}

<<<<<<< HEAD
nsSVGElement::LengthAttributesInfo SVGFilterElement::GetLengthInfo() {
||||||| merged common ancestors
nsSVGElement::LengthAttributesInfo
SVGFilterElement::GetLengthInfo()
{
=======
SVGElement::LengthAttributesInfo SVGFilterElement::GetLengthInfo() {
>>>>>>> upstream-releases
  return LengthAttributesInfo(mLengthAttributes, sLengthInfo,
                              ArrayLength(sLengthInfo));
}

<<<<<<< HEAD
nsSVGElement::EnumAttributesInfo SVGFilterElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
||||||| merged common ancestors
nsSVGElement::EnumAttributesInfo
SVGFilterElement::GetEnumInfo()
{
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo,
                            ArrayLength(sEnumInfo));
=======
SVGElement::EnumAttributesInfo SVGFilterElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsSVGElement::StringAttributesInfo SVGFilterElement::GetStringInfo() {
||||||| merged common ancestors
nsSVGElement::StringAttributesInfo
SVGFilterElement::GetStringInfo()
{
=======
SVGElement::StringAttributesInfo SVGFilterElement::GetStringInfo() {
>>>>>>> upstream-releases
  return StringAttributesInfo(mStringAttributes, sStringInfo,
                              ArrayLength(sStringInfo));
}

}  // namespace dom
}  // namespace mozilla
