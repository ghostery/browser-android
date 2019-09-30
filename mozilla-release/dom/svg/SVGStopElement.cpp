/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGStopElement.h"
#include "mozilla/dom/SVGStopElementBinding.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(Stop)

namespace mozilla {
namespace dom {

JSObject* SVGStopElement::WrapNode(JSContext* aCx,
                                   JS::Handle<JSObject*> aGivenProto) {
  return SVGStopElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::NumberInfo SVGStopElement::sNumberInfo = {nsGkAtoms::offset, 0,
                                                        true};
||||||| merged common ancestors
nsSVGElement::NumberInfo SVGStopElement::sNumberInfo =
{ nsGkAtoms::offset, 0, true };
=======
SVGElement::NumberInfo SVGStopElement::sNumberInfo = {nsGkAtoms::offset, 0,
                                                      true};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// Implementation

SVGStopElement::SVGStopElement(
    already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGStopElementBase(std::move(aNodeInfo)) {}

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGStopElement)

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGStopElement::Offset() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGStopElement::Offset()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGStopElement::Offset() {
>>>>>>> upstream-releases
  return mOffset.ToDOMAnimatedNumber(this);
}

//----------------------------------------------------------------------
// sSVGElement methods

<<<<<<< HEAD
nsSVGElement::NumberAttributesInfo SVGStopElement::GetNumberInfo() {
||||||| merged common ancestors
nsSVGElement::NumberAttributesInfo
SVGStopElement::GetNumberInfo()
{
=======
SVGElement::NumberAttributesInfo SVGStopElement::GetNumberInfo() {
>>>>>>> upstream-releases
  return NumberAttributesInfo(&mOffset, &sNumberInfo, 1);
}

//----------------------------------------------------------------------
// nsIContent methods

NS_IMETHODIMP_(bool)
SVGStopElement::IsAttributeMapped(const nsAtom* name) const {
  static const MappedAttributeEntry* const map[] = {sGradientStopMap};

  return FindAttributeDependence(name, map) ||
         SVGStopElementBase::IsAttributeMapped(name);
}

}  // namespace dom
}  // namespace mozilla
