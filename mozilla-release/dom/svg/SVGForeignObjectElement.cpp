/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGForeignObjectElement.h"

#include "mozilla/AlreadyAddRefed.h"
#include "mozilla/ArrayUtils.h"
#include "mozilla/dom/SVGDocument.h"
#include "mozilla/dom/SVGForeignObjectElementBinding.h"
#include "mozilla/dom/SVGLengthBinding.h"
#include "SVGGeometryProperty.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(ForeignObject)

namespace mozilla {
namespace dom {

JSObject* SVGForeignObjectElement::WrapNode(JSContext* aCx,
                                            JS::Handle<JSObject*> aGivenProto) {
  return SVGForeignObjectElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::LengthInfo SVGForeignObjectElement::sLengthInfo[4] = {
    {nsGkAtoms::x, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::X},
    {nsGkAtoms::y, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::Y},
    {nsGkAtoms::width, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::X},
    {nsGkAtoms::height, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::Y},
||||||| merged common ancestors
nsSVGElement::LengthInfo SVGForeignObjectElement::sLengthInfo[4] =
{
  { nsGkAtoms::x, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER, SVGContentUtils::X },
  { nsGkAtoms::y, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER, SVGContentUtils::Y },
  { nsGkAtoms::width, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER, SVGContentUtils::X },
  { nsGkAtoms::height, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER, SVGContentUtils::Y },
=======
SVGElement::LengthInfo SVGForeignObjectElement::sLengthInfo[4] = {
    {nsGkAtoms::x, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::X},
    {nsGkAtoms::y, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::Y},
    {nsGkAtoms::width, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::X},
    {nsGkAtoms::height, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::Y},
>>>>>>> upstream-releases
};

//----------------------------------------------------------------------
// Implementation

<<<<<<< HEAD
SVGForeignObjectElement::SVGForeignObjectElement(
    already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGGraphicsElement(std::move(aNodeInfo)) {}
||||||| merged common ancestors
SVGForeignObjectElement::SVGForeignObjectElement(already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
  : SVGGraphicsElement(std::move(aNodeInfo))
{
}
=======
SVGForeignObjectElement::SVGForeignObjectElement(
    already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGGraphicsElement(std::move(aNodeInfo)) {}

namespace SVGT = SVGGeometryProperty::Tags;
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGForeignObjectElement)

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGForeignObjectElement::X() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGForeignObjectElement::X()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGForeignObjectElement::X() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_X].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGForeignObjectElement::Y() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGForeignObjectElement::Y()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGForeignObjectElement::Y() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_Y].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGForeignObjectElement::Width() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGForeignObjectElement::Width()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGForeignObjectElement::Width() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_WIDTH].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGForeignObjectElement::Height() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGForeignObjectElement::Height()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGForeignObjectElement::Height() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_HEIGHT].ToDOMAnimatedLength(this);
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
/* virtual */ gfxMatrix SVGForeignObjectElement::PrependLocalTransformsTo(
    const gfxMatrix& aMatrix, SVGTransformTypes aWhich) const {
||||||| merged common ancestors
/* virtual */ gfxMatrix
SVGForeignObjectElement::PrependLocalTransformsTo(
  const gfxMatrix &aMatrix, SVGTransformTypes aWhich) const
{
=======
/* virtual */
gfxMatrix SVGForeignObjectElement::PrependLocalTransformsTo(
    const gfxMatrix& aMatrix, SVGTransformTypes aWhich) const {
>>>>>>> upstream-releases
  // 'transform' attribute:
  gfxMatrix fromUserSpace =
      SVGGraphicsElement::PrependLocalTransformsTo(aMatrix, aWhich);
  if (aWhich == eUserSpaceToParent) {
    return fromUserSpace;
  }
  // our 'x' and 'y' attributes:
  float x, y;
<<<<<<< HEAD
  const_cast<SVGForeignObjectElement*>(this)->GetAnimatedLengthValues(&x, &y,
                                                                      nullptr);
||||||| merged common ancestors
  const_cast<SVGForeignObjectElement*>(this)->
    GetAnimatedLengthValues(&x, &y, nullptr);
=======

  if (GetPrimaryFrame()) {
    SVGGeometryProperty::ResolveAll<SVGT::X, SVGT::Y>(this, &x, &y);
  } else {
    // This function might be called for element in display:none subtree
    // (e.g. getScreenCTM), we fall back to use SVG attributes.
    const_cast<SVGForeignObjectElement*>(this)->GetAnimatedLengthValues(
        &x, &y, nullptr);
  }

>>>>>>> upstream-releases
  gfxMatrix toUserSpace = gfxMatrix::Translation(x, y);
  if (aWhich == eChildToUserSpace) {
    return toUserSpace * aMatrix;
  }
  MOZ_ASSERT(aWhich == eAllTransforms, "Unknown TransformTypes");
  return toUserSpace * fromUserSpace;
}

<<<<<<< HEAD
/* virtual */ bool SVGForeignObjectElement::HasValidDimensions() const {
  return mLengthAttributes[ATTR_WIDTH].IsExplicitlySet() &&
         mLengthAttributes[ATTR_WIDTH].GetAnimValInSpecifiedUnits() > 0 &&
         mLengthAttributes[ATTR_HEIGHT].IsExplicitlySet() &&
         mLengthAttributes[ATTR_HEIGHT].GetAnimValInSpecifiedUnits() > 0;
||||||| merged common ancestors
/* virtual */ bool
SVGForeignObjectElement::HasValidDimensions() const
{
  return mLengthAttributes[ATTR_WIDTH].IsExplicitlySet() &&
         mLengthAttributes[ATTR_WIDTH].GetAnimValInSpecifiedUnits() > 0 &&
         mLengthAttributes[ATTR_HEIGHT].IsExplicitlySet() &&
         mLengthAttributes[ATTR_HEIGHT].GetAnimValInSpecifiedUnits() > 0;
=======
/* virtual */
bool SVGForeignObjectElement::HasValidDimensions() const {
  float width, height;

  MOZ_ASSERT(GetPrimaryFrame());
  SVGGeometryProperty::ResolveAll<SVGT::Width, SVGT::Height>(
      const_cast<SVGForeignObjectElement*>(this), &width, &height);
  return width > 0 && height > 0;
>>>>>>> upstream-releases
}

//----------------------------------------------------------------------
// nsIContent methods

NS_IMETHODIMP_(bool)
<<<<<<< HEAD
SVGForeignObjectElement::IsAttributeMapped(const nsAtom* name) const {
  static const MappedAttributeEntry* const map[] = {sFEFloodMap,
                                                    sFiltersMap,
                                                    sFontSpecificationMap,
                                                    sGradientStopMap,
                                                    sLightingEffectsMap,
                                                    sMarkersMap,
                                                    sTextContentElementsMap,
                                                    sViewportsMap};

  return FindAttributeDependence(name, map) ||
         SVGGraphicsElement::IsAttributeMapped(name);
||||||| merged common ancestors
SVGForeignObjectElement::IsAttributeMapped(const nsAtom* name) const
{
  static const MappedAttributeEntry* const map[] = {
    sFEFloodMap,
    sFiltersMap,
    sFontSpecificationMap,
    sGradientStopMap,
    sLightingEffectsMap,
    sMarkersMap,
    sTextContentElementsMap,
    sViewportsMap
  };

  return FindAttributeDependence(name, map) ||
    SVGGraphicsElement::IsAttributeMapped(name);
=======
SVGForeignObjectElement::IsAttributeMapped(const nsAtom* name) const {
  static const MappedAttributeEntry* const map[] = {sFEFloodMap,
                                                    sFiltersMap,
                                                    sFontSpecificationMap,
                                                    sGradientStopMap,
                                                    sLightingEffectsMap,
                                                    sMarkersMap,
                                                    sTextContentElementsMap,
                                                    sViewportsMap};

  return IsInLengthInfo(name, sLengthInfo) ||
         FindAttributeDependence(name, map) ||
         SVGGraphicsElement::IsAttributeMapped(name);
>>>>>>> upstream-releases
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::LengthAttributesInfo SVGForeignObjectElement::GetLengthInfo() {
||||||| merged common ancestors
nsSVGElement::LengthAttributesInfo
SVGForeignObjectElement::GetLengthInfo()
{
=======
SVGElement::LengthAttributesInfo SVGForeignObjectElement::GetLengthInfo() {
>>>>>>> upstream-releases
  return LengthAttributesInfo(mLengthAttributes, sLengthInfo,
                              ArrayLength(sLengthInfo));
}

<<<<<<< HEAD
}  // namespace dom
}  // namespace mozilla
||||||| merged common ancestors
} // namespace dom
} // namespace mozilla

=======
nsCSSPropertyID SVGForeignObjectElement::GetCSSPropertyIdForAttrEnum(
    uint8_t aAttrEnum) {
  switch (aAttrEnum) {
    case ATTR_X:
      return eCSSProperty_x;
    case ATTR_Y:
      return eCSSProperty_y;
    case ATTR_WIDTH:
      return eCSSProperty_width;
    case ATTR_HEIGHT:
      return eCSSProperty_height;
    default:
      MOZ_ASSERT_UNREACHABLE("Unknown attr enum");
      return eCSSProperty_UNKNOWN;
  }
}

}  // namespace dom
}  // namespace mozilla
>>>>>>> upstream-releases
