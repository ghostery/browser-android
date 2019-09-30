/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGPatternElement.h"

#include "mozilla/AlreadyAddRefed.h"
#include "mozilla/ArrayUtils.h"
#include "mozilla/dom/SVGLengthBinding.h"
#include "mozilla/dom/SVGPatternElementBinding.h"
#include "mozilla/dom/SVGUnitTypesBinding.h"
#include "DOMSVGAnimatedTransformList.h"
#include "nsGkAtoms.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(Pattern)

namespace mozilla {
namespace dom {

using namespace SVGUnitTypes_Binding;

JSObject* SVGPatternElement::WrapNode(JSContext* aCx,
                                      JS::Handle<JSObject*> aGivenProto) {
  return SVGPatternElement_Binding::Wrap(aCx, this, aGivenProto);
}

//--------------------- Patterns ------------------------

<<<<<<< HEAD
nsSVGElement::LengthInfo SVGPatternElement::sLengthInfo[4] = {
    {nsGkAtoms::x, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::y, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
    {nsGkAtoms::width, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::height, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
||||||| merged common ancestors
nsSVGElement::LengthInfo SVGPatternElement::sLengthInfo[4] =
{
  { nsGkAtoms::x, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::X },
  { nsGkAtoms::y, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::Y },
  { nsGkAtoms::width, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::X },
  { nsGkAtoms::height, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::Y },
=======
SVGElement::LengthInfo SVGPatternElement::sLengthInfo[4] = {
    {nsGkAtoms::x, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::y, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
    {nsGkAtoms::width, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::height, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
>>>>>>> upstream-releases
};

<<<<<<< HEAD
nsSVGElement::EnumInfo SVGPatternElement::sEnumInfo[2] = {
    {nsGkAtoms::patternUnits, sSVGUnitTypesMap,
     SVG_UNIT_TYPE_OBJECTBOUNDINGBOX},
    {nsGkAtoms::patternContentUnits, sSVGUnitTypesMap,
     SVG_UNIT_TYPE_USERSPACEONUSE}};
||||||| merged common ancestors
nsSVGElement::EnumInfo SVGPatternElement::sEnumInfo[2] =
{
  { nsGkAtoms::patternUnits,
    sSVGUnitTypesMap,
    SVG_UNIT_TYPE_OBJECTBOUNDINGBOX
  },
  { nsGkAtoms::patternContentUnits,
    sSVGUnitTypesMap,
    SVG_UNIT_TYPE_USERSPACEONUSE
  }
};
=======
SVGElement::EnumInfo SVGPatternElement::sEnumInfo[2] = {
    {nsGkAtoms::patternUnits, sSVGUnitTypesMap,
     SVG_UNIT_TYPE_OBJECTBOUNDINGBOX},
    {nsGkAtoms::patternContentUnits, sSVGUnitTypesMap,
     SVG_UNIT_TYPE_USERSPACEONUSE}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::StringInfo SVGPatternElement::sStringInfo[2] = {
    {nsGkAtoms::href, kNameSpaceID_None, true},
    {nsGkAtoms::href, kNameSpaceID_XLink, true}};
||||||| merged common ancestors
nsSVGElement::StringInfo SVGPatternElement::sStringInfo[2] =
{
  { nsGkAtoms::href, kNameSpaceID_None, true },
  { nsGkAtoms::href, kNameSpaceID_XLink, true }
};
=======
SVGElement::StringInfo SVGPatternElement::sStringInfo[2] = {
    {nsGkAtoms::href, kNameSpaceID_None, true},
    {nsGkAtoms::href, kNameSpaceID_XLink, true}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// Implementation

SVGPatternElement::SVGPatternElement(
    already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGPatternElementBase(std::move(aNodeInfo)) {}

//----------------------------------------------------------------------
// nsINode method

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGPatternElement)

//----------------------------------------------------------------------

already_AddRefed<SVGAnimatedRect> SVGPatternElement::ViewBox() {
  return mViewBox.ToSVGAnimatedRect(this);
}

already_AddRefed<DOMSVGAnimatedPreserveAspectRatio>
SVGPatternElement::PreserveAspectRatio() {
  return mPreserveAspectRatio.ToDOMAnimatedPreserveAspectRatio(this);
}

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration> SVGPatternElement::PatternUnits() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGPatternElement::PatternUnits()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration> SVGPatternElement::PatternUnits() {
>>>>>>> upstream-releases
  return mEnumAttributes[PATTERNUNITS].ToDOMAnimatedEnum(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration>
SVGPatternElement::PatternContentUnits() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGPatternElement::PatternContentUnits()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration>
SVGPatternElement::PatternContentUnits() {
>>>>>>> upstream-releases
  return mEnumAttributes[PATTERNCONTENTUNITS].ToDOMAnimatedEnum(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedTransformList>
SVGPatternElement::PatternTransform() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedTransformList>
SVGPatternElement::PatternTransform()
{
=======
already_AddRefed<DOMSVGAnimatedTransformList>
SVGPatternElement::PatternTransform() {
>>>>>>> upstream-releases
  // We're creating a DOM wrapper, so we must tell GetAnimatedTransformList
<<<<<<< HEAD
  // to allocate the SVGAnimatedTransformList if it hasn't already done so:
  return SVGAnimatedTransformList::GetDOMWrapper(
      GetAnimatedTransformList(DO_ALLOCATE), this);
||||||| merged common ancestors
  // to allocate the SVGAnimatedTransformList if it hasn't already done so:
  return SVGAnimatedTransformList::GetDOMWrapper(
           GetAnimatedTransformList(DO_ALLOCATE), this);
=======
  // to allocate the DOMSVGAnimatedTransformList if it hasn't already done so:
  return DOMSVGAnimatedTransformList::GetDOMWrapper(
      GetAnimatedTransformList(DO_ALLOCATE), this);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGPatternElement::X() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGPatternElement::X()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGPatternElement::X() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_X].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGPatternElement::Y() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGPatternElement::Y()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGPatternElement::Y() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_Y].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGPatternElement::Width() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGPatternElement::Width()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGPatternElement::Width() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_WIDTH].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGPatternElement::Height() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGPatternElement::Height()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGPatternElement::Height() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_HEIGHT].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGPatternElement::Href() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGPatternElement::Href()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGPatternElement::Href() {
>>>>>>> upstream-releases
  return mStringAttributes[HREF].IsExplicitlySet()
             ? mStringAttributes[HREF].ToDOMAnimatedString(this)
             : mStringAttributes[XLINK_HREF].ToDOMAnimatedString(this);
}

//----------------------------------------------------------------------
// nsIContent methods

NS_IMETHODIMP_(bool)
SVGPatternElement::IsAttributeMapped(const nsAtom* name) const {
  static const MappedAttributeEntry* const map[] = {sColorMap,
                                                    sFEFloodMap,
                                                    sFillStrokeMap,
                                                    sFiltersMap,
                                                    sFontSpecificationMap,
                                                    sGradientStopMap,
                                                    sGraphicsMap,
                                                    sLightingEffectsMap,
                                                    sMarkersMap,
                                                    sTextContentElementsMap,
                                                    sViewportsMap};

  return FindAttributeDependence(name, map) ||
         SVGPatternElementBase::IsAttributeMapped(name);
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGAnimatedTransformList* SVGPatternElement::GetAnimatedTransformList(
    uint32_t aFlags) {
||||||| merged common ancestors
nsSVGAnimatedTransformList*
SVGPatternElement::GetAnimatedTransformList(uint32_t aFlags)
{
=======
SVGAnimatedTransformList* SVGPatternElement::GetAnimatedTransformList(
    uint32_t aFlags) {
>>>>>>> upstream-releases
  if (!mPatternTransform && (aFlags & DO_ALLOCATE)) {
    mPatternTransform = new SVGAnimatedTransformList();
  }
  return mPatternTransform;
}

<<<<<<< HEAD
/* virtual */ bool SVGPatternElement::HasValidDimensions() const {
||||||| merged common ancestors
/* virtual */ bool
SVGPatternElement::HasValidDimensions() const
{
=======
/* virtual */
bool SVGPatternElement::HasValidDimensions() const {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_WIDTH].IsExplicitlySet() &&
         mLengthAttributes[ATTR_WIDTH].GetAnimValInSpecifiedUnits() > 0 &&
         mLengthAttributes[ATTR_HEIGHT].IsExplicitlySet() &&
         mLengthAttributes[ATTR_HEIGHT].GetAnimValInSpecifiedUnits() > 0;
}

<<<<<<< HEAD
nsSVGElement::LengthAttributesInfo SVGPatternElement::GetLengthInfo() {
||||||| merged common ancestors
nsSVGElement::LengthAttributesInfo
SVGPatternElement::GetLengthInfo()
{
=======
SVGElement::LengthAttributesInfo SVGPatternElement::GetLengthInfo() {
>>>>>>> upstream-releases
  return LengthAttributesInfo(mLengthAttributes, sLengthInfo,
                              ArrayLength(sLengthInfo));
}

<<<<<<< HEAD
nsSVGElement::EnumAttributesInfo SVGPatternElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
||||||| merged common ancestors
nsSVGElement::EnumAttributesInfo
SVGPatternElement::GetEnumInfo()
{
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo,
                            ArrayLength(sEnumInfo));
=======
SVGElement::EnumAttributesInfo SVGPatternElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsSVGViewBox* SVGPatternElement::GetViewBox() { return &mViewBox; }
||||||| merged common ancestors
nsSVGViewBox *
SVGPatternElement::GetViewBox()
{
  return &mViewBox;
}
=======
SVGAnimatedViewBox* SVGPatternElement::GetAnimatedViewBox() {
  return &mViewBox;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
SVGAnimatedPreserveAspectRatio* SVGPatternElement::GetPreserveAspectRatio() {
||||||| merged common ancestors
SVGAnimatedPreserveAspectRatio *
SVGPatternElement::GetPreserveAspectRatio()
{
=======
SVGAnimatedPreserveAspectRatio*
SVGPatternElement::GetAnimatedPreserveAspectRatio() {
>>>>>>> upstream-releases
  return &mPreserveAspectRatio;
}

<<<<<<< HEAD
nsSVGElement::StringAttributesInfo SVGPatternElement::GetStringInfo() {
||||||| merged common ancestors
nsSVGElement::StringAttributesInfo
SVGPatternElement::GetStringInfo()
{
=======
SVGElement::StringAttributesInfo SVGPatternElement::GetStringInfo() {
>>>>>>> upstream-releases
  return StringAttributesInfo(mStringAttributes, sStringInfo,
                              ArrayLength(sStringInfo));
}

}  // namespace dom
}  // namespace mozilla
