/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGFEMergeElement.h"
#include "mozilla/dom/SVGFEMergeElementBinding.h"
#include "mozilla/dom/SVGFEMergeNodeElement.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(FEMerge)

using namespace mozilla::gfx;

namespace mozilla {
namespace dom {

JSObject* SVGFEMergeElement::WrapNode(JSContext* aCx,
                                      JS::Handle<JSObject*> aGivenProto) {
  return SVGFEMergeElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::StringInfo SVGFEMergeElement::sStringInfo[1] = {
    {nsGkAtoms::result, kNameSpaceID_None, true}};
||||||| merged common ancestors
nsSVGElement::StringInfo SVGFEMergeElement::sStringInfo[1] =
{
  { nsGkAtoms::result, kNameSpaceID_None, true }
};
=======
SVGElement::StringInfo SVGFEMergeElement::sStringInfo[1] = {
    {nsGkAtoms::result, kNameSpaceID_None, true}};
>>>>>>> upstream-releases

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGFEMergeElement)

FilterPrimitiveDescription SVGFEMergeElement::GetPrimitiveDescription(
    nsSVGFilterInstance* aInstance, const IntRect& aFilterSubregion,
    const nsTArray<bool>& aInputsAreTainted,
    nsTArray<RefPtr<SourceSurface>>& aInputImages) {
  return FilterPrimitiveDescription(AsVariant(MergeAttributes()));
}

<<<<<<< HEAD
void SVGFEMergeElement::GetSourceImageNames(
    nsTArray<nsSVGStringInfo>& aSources) {
  for (nsIContent* child = nsINode::GetFirstChild(); child;
||||||| merged common ancestors
void
SVGFEMergeElement::GetSourceImageNames(nsTArray<nsSVGStringInfo>& aSources)
{
  for (nsIContent* child = nsINode::GetFirstChild();
       child;
=======
void SVGFEMergeElement::GetSourceImageNames(nsTArray<SVGStringInfo>& aSources) {
  for (nsIContent* child = nsINode::GetFirstChild(); child;
>>>>>>> upstream-releases
       child = child->GetNextSibling()) {
    if (child->IsSVGElement(nsGkAtoms::feMergeNode)) {
      SVGFEMergeNodeElement* node = static_cast<SVGFEMergeNodeElement*>(child);
      aSources.AppendElement(SVGStringInfo(node->GetIn1(), node));
    }
  }
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::StringAttributesInfo SVGFEMergeElement::GetStringInfo() {
||||||| merged common ancestors
nsSVGElement::StringAttributesInfo
SVGFEMergeElement::GetStringInfo()
{
=======
SVGElement::StringAttributesInfo SVGFEMergeElement::GetStringInfo() {
>>>>>>> upstream-releases
  return StringAttributesInfo(mStringAttributes, sStringInfo,
                              ArrayLength(sStringInfo));
}

}  // namespace dom
}  // namespace mozilla
