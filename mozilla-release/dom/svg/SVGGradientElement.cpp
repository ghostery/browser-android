/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGGradientElement.h"

#include "mozilla/AlreadyAddRefed.h"
#include "mozilla/ArrayUtils.h"
#include "mozilla/dom/SVGElement.h"
#include "mozilla/dom/SVGGradientElementBinding.h"
#include "mozilla/dom/SVGLengthBinding.h"
#include "mozilla/dom/SVGLinearGradientElementBinding.h"
#include "mozilla/dom/SVGRadialGradientElementBinding.h"
#include "mozilla/dom/SVGUnitTypesBinding.h"
#include "DOMSVGAnimatedTransformList.h"
#include "nsGkAtoms.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(LinearGradient)
NS_IMPL_NS_NEW_SVG_ELEMENT(RadialGradient)

namespace mozilla {
namespace dom {

using namespace SVGGradientElement_Binding;
using namespace SVGUnitTypes_Binding;

//--------------------- Gradients------------------------

<<<<<<< HEAD
nsSVGEnumMapping SVGGradientElement::sSpreadMethodMap[] = {
    {nsGkAtoms::pad, SVG_SPREADMETHOD_PAD},
    {nsGkAtoms::reflect, SVG_SPREADMETHOD_REFLECT},
    {nsGkAtoms::repeat, SVG_SPREADMETHOD_REPEAT},
    {nullptr, 0}};
||||||| merged common ancestors
nsSVGEnumMapping SVGGradientElement::sSpreadMethodMap[] = {
  {nsGkAtoms::pad, SVG_SPREADMETHOD_PAD},
  {nsGkAtoms::reflect, SVG_SPREADMETHOD_REFLECT},
  {nsGkAtoms::repeat, SVG_SPREADMETHOD_REPEAT},
  {nullptr, 0}
};
=======
SVGEnumMapping SVGGradientElement::sSpreadMethodMap[] = {
    {nsGkAtoms::pad, SVG_SPREADMETHOD_PAD},
    {nsGkAtoms::reflect, SVG_SPREADMETHOD_REFLECT},
    {nsGkAtoms::repeat, SVG_SPREADMETHOD_REPEAT},
    {nullptr, 0}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::EnumInfo SVGGradientElement::sEnumInfo[2] = {
    {nsGkAtoms::gradientUnits, sSVGUnitTypesMap,
     SVG_UNIT_TYPE_OBJECTBOUNDINGBOX},
    {nsGkAtoms::spreadMethod, sSpreadMethodMap, SVG_SPREADMETHOD_PAD}};
||||||| merged common ancestors
nsSVGElement::EnumInfo SVGGradientElement::sEnumInfo[2] =
{
  { nsGkAtoms::gradientUnits,
    sSVGUnitTypesMap,
    SVG_UNIT_TYPE_OBJECTBOUNDINGBOX
  },
  { nsGkAtoms::spreadMethod,
    sSpreadMethodMap,
    SVG_SPREADMETHOD_PAD
  }
};
=======
SVGElement::EnumInfo SVGGradientElement::sEnumInfo[2] = {
    {nsGkAtoms::gradientUnits, sSVGUnitTypesMap,
     SVG_UNIT_TYPE_OBJECTBOUNDINGBOX},
    {nsGkAtoms::spreadMethod, sSpreadMethodMap, SVG_SPREADMETHOD_PAD}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::StringInfo SVGGradientElement::sStringInfo[2] = {
    {nsGkAtoms::href, kNameSpaceID_None, true},
    {nsGkAtoms::href, kNameSpaceID_XLink, true}};
||||||| merged common ancestors
nsSVGElement::StringInfo SVGGradientElement::sStringInfo[2] =
{
  { nsGkAtoms::href, kNameSpaceID_None, true },
  { nsGkAtoms::href, kNameSpaceID_XLink, true }
};
=======
SVGElement::StringInfo SVGGradientElement::sStringInfo[2] = {
    {nsGkAtoms::href, kNameSpaceID_None, true},
    {nsGkAtoms::href, kNameSpaceID_XLink, true}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// Implementation

SVGGradientElement::SVGGradientElement(
    already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGGradientElementBase(std::move(aNodeInfo)) {}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::EnumAttributesInfo SVGGradientElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
||||||| merged common ancestors
nsSVGElement::EnumAttributesInfo
SVGGradientElement::GetEnumInfo()
{
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo,
                            ArrayLength(sEnumInfo));
=======
SVGElement::EnumAttributesInfo SVGGradientElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsSVGElement::StringAttributesInfo SVGGradientElement::GetStringInfo() {
||||||| merged common ancestors
nsSVGElement::StringAttributesInfo
SVGGradientElement::GetStringInfo()
{
=======
SVGElement::StringAttributesInfo SVGGradientElement::GetStringInfo() {
>>>>>>> upstream-releases
  return StringAttributesInfo(mStringAttributes, sStringInfo,
                              ArrayLength(sStringInfo));
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration> SVGGradientElement::GradientUnits() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGGradientElement::GradientUnits()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration>
SVGGradientElement::GradientUnits() {
>>>>>>> upstream-releases
  return mEnumAttributes[GRADIENTUNITS].ToDOMAnimatedEnum(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedTransformList>
SVGGradientElement::GradientTransform() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedTransformList>
SVGGradientElement::GradientTransform()
{
=======
already_AddRefed<DOMSVGAnimatedTransformList>
SVGGradientElement::GradientTransform() {
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
already_AddRefed<SVGAnimatedEnumeration> SVGGradientElement::SpreadMethod() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGGradientElement::SpreadMethod()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration> SVGGradientElement::SpreadMethod() {
>>>>>>> upstream-releases
  return mEnumAttributes[SPREADMETHOD].ToDOMAnimatedEnum(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGGradientElement::Href() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGGradientElement::Href()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGGradientElement::Href() {
>>>>>>> upstream-releases
  return mStringAttributes[HREF].IsExplicitlySet()
             ? mStringAttributes[HREF].ToDOMAnimatedString(this)
             : mStringAttributes[XLINK_HREF].ToDOMAnimatedString(this);
}

//----------------------------------------------------------------------
// nsIContent methods

NS_IMETHODIMP_(bool)
SVGGradientElement::IsAttributeMapped(const nsAtom* name) const {
  static const MappedAttributeEntry* const map[] = {sColorMap,
                                                    sGradientStopMap};

  return FindAttributeDependence(name, map) ||
         SVGGradientElementBase::IsAttributeMapped(name);
}

//---------------------Linear Gradients------------------------

JSObject* SVGLinearGradientElement::WrapNode(
    JSContext* aCx, JS::Handle<JSObject*> aGivenProto) {
  return SVGLinearGradientElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::LengthInfo SVGLinearGradientElement::sLengthInfo[4] = {
    {nsGkAtoms::x1, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::y1, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
    {nsGkAtoms::x2, 100, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::y2, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
||||||| merged common ancestors
nsSVGElement::LengthInfo SVGLinearGradientElement::sLengthInfo[4] =
{
  { nsGkAtoms::x1, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::X },
  { nsGkAtoms::y1, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::Y },
  { nsGkAtoms::x2, 100, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::X },
  { nsGkAtoms::y2, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::Y },
=======
SVGElement::LengthInfo SVGLinearGradientElement::sLengthInfo[4] = {
    {nsGkAtoms::x1, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::y1, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
    {nsGkAtoms::x2, 100, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::y2, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
>>>>>>> upstream-releases
};

//----------------------------------------------------------------------
// Implementation

SVGLinearGradientElement::SVGLinearGradientElement(
    already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGLinearGradientElementBase(std::move(aNodeInfo)) {}

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGLinearGradientElement)

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGLinearGradientElement::X1() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGLinearGradientElement::X1()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGLinearGradientElement::X1() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_X1].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGLinearGradientElement::Y1() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGLinearGradientElement::Y1()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGLinearGradientElement::Y1() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_Y1].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGLinearGradientElement::X2() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGLinearGradientElement::X2()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGLinearGradientElement::X2() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_X2].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGLinearGradientElement::Y2() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGLinearGradientElement::Y2()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGLinearGradientElement::Y2() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_Y2].ToDOMAnimatedLength(this);
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGAnimatedTransformList* SVGGradientElement::GetAnimatedTransformList(
    uint32_t aFlags) {
||||||| merged common ancestors
nsSVGAnimatedTransformList*
SVGGradientElement::GetAnimatedTransformList(uint32_t aFlags)
{
=======
SVGAnimatedTransformList* SVGGradientElement::GetAnimatedTransformList(
    uint32_t aFlags) {
>>>>>>> upstream-releases
  if (!mGradientTransform && (aFlags & DO_ALLOCATE)) {
    mGradientTransform = new SVGAnimatedTransformList();
  }
  return mGradientTransform;
}

<<<<<<< HEAD
nsSVGElement::LengthAttributesInfo SVGLinearGradientElement::GetLengthInfo() {
||||||| merged common ancestors
nsSVGElement::LengthAttributesInfo
SVGLinearGradientElement::GetLengthInfo()
{
=======
SVGElement::LengthAttributesInfo SVGLinearGradientElement::GetLengthInfo() {
>>>>>>> upstream-releases
  return LengthAttributesInfo(mLengthAttributes, sLengthInfo,
                              ArrayLength(sLengthInfo));
}

//-------------------------- Radial Gradients ----------------------------

JSObject* SVGRadialGradientElement::WrapNode(
    JSContext* aCx, JS::Handle<JSObject*> aGivenProto) {
  return SVGRadialGradientElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::LengthInfo SVGRadialGradientElement::sLengthInfo[6] = {
    {nsGkAtoms::cx, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::cy, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
    {nsGkAtoms::r, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::XY},
    {nsGkAtoms::fx, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::fy, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
    {nsGkAtoms::fr, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::XY},
||||||| merged common ancestors
nsSVGElement::LengthInfo SVGRadialGradientElement::sLengthInfo[6] =
{
  { nsGkAtoms::cx, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::X },
  { nsGkAtoms::cy, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::Y },
  { nsGkAtoms::r, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::XY },
  { nsGkAtoms::fx, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::X },
  { nsGkAtoms::fy, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::Y },
  { nsGkAtoms::fr, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE, SVGContentUtils::XY },
=======
SVGElement::LengthInfo SVGRadialGradientElement::sLengthInfo[6] = {
    {nsGkAtoms::cx, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::cy, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
    {nsGkAtoms::r, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::XY},
    {nsGkAtoms::fx, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::X},
    {nsGkAtoms::fy, 50, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::Y},
    {nsGkAtoms::fr, 0, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE,
     SVGContentUtils::XY},
>>>>>>> upstream-releases
};

//----------------------------------------------------------------------
// Implementation

SVGRadialGradientElement::SVGRadialGradientElement(
    already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGRadialGradientElementBase(std::move(aNodeInfo)) {}

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGRadialGradientElement)

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGRadialGradientElement::Cx() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGRadialGradientElement::Cx()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGRadialGradientElement::Cx() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_CX].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGRadialGradientElement::Cy() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGRadialGradientElement::Cy()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGRadialGradientElement::Cy() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_CY].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGRadialGradientElement::R() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGRadialGradientElement::R()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGRadialGradientElement::R() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_R].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGRadialGradientElement::Fx() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGRadialGradientElement::Fx()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGRadialGradientElement::Fx() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_FX].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGRadialGradientElement::Fy() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGRadialGradientElement::Fy()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGRadialGradientElement::Fy() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_FY].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGRadialGradientElement::Fr() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGRadialGradientElement::Fr()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGRadialGradientElement::Fr() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_FR].ToDOMAnimatedLength(this);
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::LengthAttributesInfo SVGRadialGradientElement::GetLengthInfo() {
||||||| merged common ancestors
nsSVGElement::LengthAttributesInfo
SVGRadialGradientElement::GetLengthInfo()
{
=======
SVGElement::LengthAttributesInfo SVGRadialGradientElement::GetLengthInfo() {
>>>>>>> upstream-releases
  return LengthAttributesInfo(mLengthAttributes, sLengthInfo,
                              ArrayLength(sLengthInfo));
}

}  // namespace dom
}  // namespace mozilla
