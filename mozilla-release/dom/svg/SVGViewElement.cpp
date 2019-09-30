/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGViewElement.h"
#include "mozilla/dom/SVGViewElementBinding.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(View)

namespace mozilla {
namespace dom {

using namespace SVGViewElement_Binding;

JSObject* SVGViewElement::WrapNode(JSContext* aCx,
                                   JS::Handle<JSObject*> aGivenProto) {
  return SVGViewElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGEnumMapping SVGViewElement::sZoomAndPanMap[] = {
    {nsGkAtoms::disable, SVG_ZOOMANDPAN_DISABLE},
    {nsGkAtoms::magnify, SVG_ZOOMANDPAN_MAGNIFY},
    {nullptr, 0}};

nsSVGElement::EnumInfo SVGViewElement::sEnumInfo[1] = {
    {nsGkAtoms::zoomAndPan, sZoomAndPanMap, SVG_ZOOMANDPAN_MAGNIFY}};
||||||| merged common ancestors
nsSVGEnumMapping SVGViewElement::sZoomAndPanMap[] = {
  {nsGkAtoms::disable, SVG_ZOOMANDPAN_DISABLE},
  {nsGkAtoms::magnify, SVG_ZOOMANDPAN_MAGNIFY},
  {nullptr, 0}
};

nsSVGElement::EnumInfo SVGViewElement::sEnumInfo[1] =
{
  { nsGkAtoms::zoomAndPan,
    sZoomAndPanMap,
    SVG_ZOOMANDPAN_MAGNIFY
  }
};
=======
SVGEnumMapping SVGViewElement::sZoomAndPanMap[] = {
    {nsGkAtoms::disable, SVG_ZOOMANDPAN_DISABLE},
    {nsGkAtoms::magnify, SVG_ZOOMANDPAN_MAGNIFY},
    {nullptr, 0}};

SVGElement::EnumInfo SVGViewElement::sEnumInfo[1] = {
    {nsGkAtoms::zoomAndPan, sZoomAndPanMap, SVG_ZOOMANDPAN_MAGNIFY}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// Implementation

SVGViewElement::SVGViewElement(
    already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGViewElementBase(std::move(aNodeInfo)) {}

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGViewElement)

void SVGViewElement::SetZoomAndPan(uint16_t aZoomAndPan, ErrorResult& rv) {
  if (aZoomAndPan == SVG_ZOOMANDPAN_DISABLE ||
      aZoomAndPan == SVG_ZOOMANDPAN_MAGNIFY) {
    mEnumAttributes[ZOOMANDPAN].SetBaseValue(aZoomAndPan, this);
    return;
  }

  rv.ThrowRangeError<MSG_INVALID_ZOOMANDPAN_VALUE_ERROR>();
}

//----------------------------------------------------------------------

already_AddRefed<SVGAnimatedRect> SVGViewElement::ViewBox() {
  return mViewBox.ToSVGAnimatedRect(this);
}

already_AddRefed<DOMSVGAnimatedPreserveAspectRatio>
SVGViewElement::PreserveAspectRatio() {
  return mPreserveAspectRatio.ToDOMAnimatedPreserveAspectRatio(this);
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::EnumAttributesInfo SVGViewElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
||||||| merged common ancestors
nsSVGElement::EnumAttributesInfo
SVGViewElement::GetEnumInfo()
{
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo,
                            ArrayLength(sEnumInfo));
=======
SVGElement::EnumAttributesInfo SVGViewElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsSVGViewBox* SVGViewElement::GetViewBox() { return &mViewBox; }
||||||| merged common ancestors
nsSVGViewBox *
SVGViewElement::GetViewBox()
{
  return &mViewBox;
}
=======
SVGAnimatedViewBox* SVGViewElement::GetAnimatedViewBox() { return &mViewBox; }
>>>>>>> upstream-releases

<<<<<<< HEAD
SVGAnimatedPreserveAspectRatio* SVGViewElement::GetPreserveAspectRatio() {
||||||| merged common ancestors
SVGAnimatedPreserveAspectRatio *
SVGViewElement::GetPreserveAspectRatio()
{
=======
SVGAnimatedPreserveAspectRatio*
SVGViewElement::GetAnimatedPreserveAspectRatio() {
>>>>>>> upstream-releases
  return &mPreserveAspectRatio;
}

}  // namespace dom
}  // namespace mozilla
