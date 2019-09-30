/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGTextPositioningElement.h"

#include "mozilla/ArrayUtils.h"
#include "SVGAnimatedLengthList.h"
#include "DOMSVGAnimatedLengthList.h"
#include "DOMSVGAnimatedNumberList.h"
#include "SVGContentUtils.h"

namespace mozilla {
namespace dom {

<<<<<<< HEAD
nsSVGElement::LengthListInfo SVGTextPositioningElement::sLengthListInfo[4] = {
    {nsGkAtoms::x, SVGContentUtils::X, false},
    {nsGkAtoms::y, SVGContentUtils::Y, false},
    {nsGkAtoms::dx, SVGContentUtils::X, true},
    {nsGkAtoms::dy, SVGContentUtils::Y, true}};
||||||| merged common ancestors
nsSVGElement::LengthListInfo SVGTextPositioningElement::sLengthListInfo[4] =
{
  { nsGkAtoms::x,  SVGContentUtils::X, false },
  { nsGkAtoms::y,  SVGContentUtils::Y, false },
  { nsGkAtoms::dx, SVGContentUtils::X, true },
  { nsGkAtoms::dy, SVGContentUtils::Y, true }
};
=======
SVGElement::LengthListInfo SVGTextPositioningElement::sLengthListInfo[4] = {
    {nsGkAtoms::x, SVGContentUtils::X, false},
    {nsGkAtoms::y, SVGContentUtils::Y, false},
    {nsGkAtoms::dx, SVGContentUtils::X, true},
    {nsGkAtoms::dy, SVGContentUtils::Y, true}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::LengthListAttributesInfo
SVGTextPositioningElement::GetLengthListInfo() {
||||||| merged common ancestors
nsSVGElement::LengthListAttributesInfo
SVGTextPositioningElement::GetLengthListInfo()
{
=======
SVGElement::LengthListAttributesInfo
SVGTextPositioningElement::GetLengthListInfo() {
>>>>>>> upstream-releases
  return LengthListAttributesInfo(mLengthListAttributes, sLengthListInfo,
                                  ArrayLength(sLengthListInfo));
}

<<<<<<< HEAD
nsSVGElement::NumberListInfo SVGTextPositioningElement::sNumberListInfo[1] = {
    {nsGkAtoms::rotate}};

nsSVGElement::NumberListAttributesInfo
SVGTextPositioningElement::GetNumberListInfo() {
||||||| merged common ancestors

nsSVGElement::NumberListInfo SVGTextPositioningElement::sNumberListInfo[1] =
{
  { nsGkAtoms::rotate }
};

nsSVGElement::NumberListAttributesInfo
SVGTextPositioningElement::GetNumberListInfo()
{
=======
SVGElement::NumberListInfo SVGTextPositioningElement::sNumberListInfo[1] = {
    {nsGkAtoms::rotate}};

SVGElement::NumberListAttributesInfo
SVGTextPositioningElement::GetNumberListInfo() {
>>>>>>> upstream-releases
  return NumberListAttributesInfo(mNumberListAttributes, sNumberListInfo,
                                  ArrayLength(sNumberListInfo));
}

//----------------------------------------------------------------------

already_AddRefed<DOMSVGAnimatedLengthList> SVGTextPositioningElement::X() {
  return DOMSVGAnimatedLengthList::GetDOMWrapper(
      &mLengthListAttributes[ATTR_X], this, ATTR_X, SVGContentUtils::X);
}

already_AddRefed<DOMSVGAnimatedLengthList> SVGTextPositioningElement::Y() {
  return DOMSVGAnimatedLengthList::GetDOMWrapper(
      &mLengthListAttributes[ATTR_Y], this, ATTR_Y, SVGContentUtils::Y);
}

already_AddRefed<DOMSVGAnimatedLengthList> SVGTextPositioningElement::Dx() {
  return DOMSVGAnimatedLengthList::GetDOMWrapper(
      &mLengthListAttributes[ATTR_DX], this, ATTR_DX, SVGContentUtils::X);
}

already_AddRefed<DOMSVGAnimatedLengthList> SVGTextPositioningElement::Dy() {
  return DOMSVGAnimatedLengthList::GetDOMWrapper(
      &mLengthListAttributes[ATTR_DY], this, ATTR_DY, SVGContentUtils::Y);
}

already_AddRefed<DOMSVGAnimatedNumberList> SVGTextPositioningElement::Rotate() {
  return DOMSVGAnimatedNumberList::GetDOMWrapper(&mNumberListAttributes[ROTATE],
                                                 this, ROTATE);
}

}  // namespace dom
}  // namespace mozilla
