/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGFEOffsetElement.h"
#include "mozilla/dom/SVGFEOffsetElementBinding.h"
#include "nsSVGFilterInstance.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(FEOffset)

using namespace mozilla::gfx;

namespace mozilla {
namespace dom {

JSObject* SVGFEOffsetElement::WrapNode(JSContext* aCx,
                                       JS::Handle<JSObject*> aGivenProto) {
  return SVGFEOffsetElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::NumberInfo SVGFEOffsetElement::sNumberInfo[2] = {
    {nsGkAtoms::dx, 0, false}, {nsGkAtoms::dy, 0, false}};
||||||| merged common ancestors
nsSVGElement::NumberInfo SVGFEOffsetElement::sNumberInfo[2] =
{
  { nsGkAtoms::dx, 0, false },
  { nsGkAtoms::dy, 0, false }
};
=======
SVGElement::NumberInfo SVGFEOffsetElement::sNumberInfo[2] = {
    {nsGkAtoms::dx, 0, false}, {nsGkAtoms::dy, 0, false}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::StringInfo SVGFEOffsetElement::sStringInfo[2] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true}};
||||||| merged common ancestors
nsSVGElement::StringInfo SVGFEOffsetElement::sStringInfo[2] =
{
  { nsGkAtoms::result, kNameSpaceID_None, true },
  { nsGkAtoms::in, kNameSpaceID_None, true }
};
=======
SVGElement::StringInfo SVGFEOffsetElement::sStringInfo[2] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGFEOffsetElement)

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGFEOffsetElement::In1() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGFEOffsetElement::In1()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGFEOffsetElement::In1() {
>>>>>>> upstream-releases
  return mStringAttributes[IN1].ToDOMAnimatedString(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFEOffsetElement::Dx() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEOffsetElement::Dx()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFEOffsetElement::Dx() {
>>>>>>> upstream-releases
  return mNumberAttributes[DX].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFEOffsetElement::Dy() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEOffsetElement::Dy()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFEOffsetElement::Dy() {
>>>>>>> upstream-releases
  return mNumberAttributes[DY].ToDOMAnimatedNumber(this);
}

FilterPrimitiveDescription SVGFEOffsetElement::GetPrimitiveDescription(
    nsSVGFilterInstance* aInstance, const IntRect& aFilterSubregion,
    const nsTArray<bool>& aInputsAreTainted,
    nsTArray<RefPtr<SourceSurface>>& aInputImages) {
  OffsetAttributes atts;
  IntPoint offset(int32_t(aInstance->GetPrimitiveNumber(
                      SVGContentUtils::X, &mNumberAttributes[DX])),
                  int32_t(aInstance->GetPrimitiveNumber(
                      SVGContentUtils::Y, &mNumberAttributes[DY])));
  atts.mValue = offset;
  return FilterPrimitiveDescription(AsVariant(std::move(atts)));
}

bool SVGFEOffsetElement::AttributeAffectsRendering(int32_t aNameSpaceID,
                                                   nsAtom* aAttribute) const {
  return SVGFEOffsetElementBase::AttributeAffectsRendering(aNameSpaceID,
                                                           aAttribute) ||
         (aNameSpaceID == kNameSpaceID_None &&
          (aAttribute == nsGkAtoms::in || aAttribute == nsGkAtoms::dx ||
           aAttribute == nsGkAtoms::dy));
}

<<<<<<< HEAD
void SVGFEOffsetElement::GetSourceImageNames(
    nsTArray<nsSVGStringInfo>& aSources) {
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
||||||| merged common ancestors
void
SVGFEOffsetElement::GetSourceImageNames(nsTArray<nsSVGStringInfo>& aSources)
{
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
=======
void SVGFEOffsetElement::GetSourceImageNames(
    nsTArray<SVGStringInfo>& aSources) {
  aSources.AppendElement(SVGStringInfo(&mStringAttributes[IN1], this));
>>>>>>> upstream-releases
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::NumberAttributesInfo SVGFEOffsetElement::GetNumberInfo() {
||||||| merged common ancestors
nsSVGElement::NumberAttributesInfo
SVGFEOffsetElement::GetNumberInfo()
{
=======
SVGElement::NumberAttributesInfo SVGFEOffsetElement::GetNumberInfo() {
>>>>>>> upstream-releases
  return NumberAttributesInfo(mNumberAttributes, sNumberInfo,
                              ArrayLength(sNumberInfo));
}

<<<<<<< HEAD
nsSVGElement::StringAttributesInfo SVGFEOffsetElement::GetStringInfo() {
||||||| merged common ancestors
nsSVGElement::StringAttributesInfo
SVGFEOffsetElement::GetStringInfo()
{
=======
SVGElement::StringAttributesInfo SVGFEOffsetElement::GetStringInfo() {
>>>>>>> upstream-releases
  return StringAttributesInfo(mStringAttributes, sStringInfo,
                              ArrayLength(sStringInfo));
}

}  // namespace dom
}  // namespace mozilla
