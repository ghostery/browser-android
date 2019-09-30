/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGFEConvolveMatrixElement.h"
#include "mozilla/dom/SVGFEConvolveMatrixElementBinding.h"
#include "mozilla/UniquePtr.h"
#include "mozilla/UniquePtrExtensions.h"
#include "DOMSVGAnimatedNumberList.h"
#include "nsSVGUtils.h"
#include "nsSVGFilterInstance.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(FEConvolveMatrix)

using namespace mozilla::gfx;

namespace mozilla {
namespace dom {

JSObject* SVGFEConvolveMatrixElement::WrapNode(
    JSContext* aCx, JS::Handle<JSObject*> aGivenProto) {
  return SVGFEConvolveMatrixElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::NumberInfo SVGFEConvolveMatrixElement::sNumberInfo[2] = {
    {nsGkAtoms::divisor, 1, false}, {nsGkAtoms::bias, 0, false}};

nsSVGElement::NumberPairInfo SVGFEConvolveMatrixElement::sNumberPairInfo[1] = {
    {nsGkAtoms::kernelUnitLength, 0, 0}};

nsSVGElement::IntegerInfo SVGFEConvolveMatrixElement::sIntegerInfo[2] = {
    {nsGkAtoms::targetX, 0}, {nsGkAtoms::targetY, 0}};

nsSVGElement::IntegerPairInfo SVGFEConvolveMatrixElement::sIntegerPairInfo[1] =
    {{nsGkAtoms::order, 3, 3}};

nsSVGElement::BooleanInfo SVGFEConvolveMatrixElement::sBooleanInfo[1] = {
    {nsGkAtoms::preserveAlpha, false}};

nsSVGEnumMapping SVGFEConvolveMatrixElement::sEdgeModeMap[] = {
    {nsGkAtoms::duplicate, SVG_EDGEMODE_DUPLICATE},
    {nsGkAtoms::wrap, SVG_EDGEMODE_WRAP},
    {nsGkAtoms::none, SVG_EDGEMODE_NONE},
    {nullptr, 0}};

nsSVGElement::EnumInfo SVGFEConvolveMatrixElement::sEnumInfo[1] = {
    {nsGkAtoms::edgeMode, sEdgeModeMap, SVG_EDGEMODE_DUPLICATE}};
||||||| merged common ancestors
nsSVGElement::NumberInfo SVGFEConvolveMatrixElement::sNumberInfo[2] =
{
  { nsGkAtoms::divisor, 1, false },
  { nsGkAtoms::bias, 0, false }
};

nsSVGElement::NumberPairInfo SVGFEConvolveMatrixElement::sNumberPairInfo[1] =
{
  { nsGkAtoms::kernelUnitLength, 0, 0 }
};

nsSVGElement::IntegerInfo SVGFEConvolveMatrixElement::sIntegerInfo[2] =
{
  { nsGkAtoms::targetX, 0 },
  { nsGkAtoms::targetY, 0 }
};

nsSVGElement::IntegerPairInfo SVGFEConvolveMatrixElement::sIntegerPairInfo[1] =
{
  { nsGkAtoms::order, 3, 3 }
};

nsSVGElement::BooleanInfo SVGFEConvolveMatrixElement::sBooleanInfo[1] =
{
  { nsGkAtoms::preserveAlpha, false }
};

nsSVGEnumMapping SVGFEConvolveMatrixElement::sEdgeModeMap[] = {
  {nsGkAtoms::duplicate, SVG_EDGEMODE_DUPLICATE},
  {nsGkAtoms::wrap, SVG_EDGEMODE_WRAP},
  {nsGkAtoms::none, SVG_EDGEMODE_NONE},
  {nullptr, 0}
};

nsSVGElement::EnumInfo SVGFEConvolveMatrixElement::sEnumInfo[1] =
{
  { nsGkAtoms::edgeMode,
    sEdgeModeMap,
    SVG_EDGEMODE_DUPLICATE
  }
};
=======
SVGElement::NumberInfo SVGFEConvolveMatrixElement::sNumberInfo[2] = {
    {nsGkAtoms::divisor, 1, false}, {nsGkAtoms::bias, 0, false}};

SVGElement::NumberPairInfo SVGFEConvolveMatrixElement::sNumberPairInfo[1] = {
    {nsGkAtoms::kernelUnitLength, 0, 0}};

SVGElement::IntegerInfo SVGFEConvolveMatrixElement::sIntegerInfo[2] = {
    {nsGkAtoms::targetX, 0}, {nsGkAtoms::targetY, 0}};

SVGElement::IntegerPairInfo SVGFEConvolveMatrixElement::sIntegerPairInfo[1] = {
    {nsGkAtoms::order, 3, 3}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::StringInfo SVGFEConvolveMatrixElement::sStringInfo[2] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true}};
||||||| merged common ancestors
nsSVGElement::StringInfo SVGFEConvolveMatrixElement::sStringInfo[2] =
{
  { nsGkAtoms::result, kNameSpaceID_None, true },
  { nsGkAtoms::in, kNameSpaceID_None, true }
};
=======
SVGElement::BooleanInfo SVGFEConvolveMatrixElement::sBooleanInfo[1] = {
    {nsGkAtoms::preserveAlpha, false}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::NumberListInfo SVGFEConvolveMatrixElement::sNumberListInfo[1] = {
    {nsGkAtoms::kernelMatrix}};
||||||| merged common ancestors
nsSVGElement::NumberListInfo SVGFEConvolveMatrixElement::sNumberListInfo[1] =
{
  { nsGkAtoms::kernelMatrix }
};
=======
SVGEnumMapping SVGFEConvolveMatrixElement::sEdgeModeMap[] = {
    {nsGkAtoms::duplicate, SVG_EDGEMODE_DUPLICATE},
    {nsGkAtoms::wrap, SVG_EDGEMODE_WRAP},
    {nsGkAtoms::none, SVG_EDGEMODE_NONE},
    {nullptr, 0}};

SVGElement::EnumInfo SVGFEConvolveMatrixElement::sEnumInfo[1] = {
    {nsGkAtoms::edgeMode, sEdgeModeMap, SVG_EDGEMODE_DUPLICATE}};

SVGElement::StringInfo SVGFEConvolveMatrixElement::sStringInfo[2] = {
    {nsGkAtoms::result, kNameSpaceID_None, true},
    {nsGkAtoms::in, kNameSpaceID_None, true}};

SVGElement::NumberListInfo SVGFEConvolveMatrixElement::sNumberListInfo[1] = {
    {nsGkAtoms::kernelMatrix}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGFEConvolveMatrixElement)

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGFEConvolveMatrixElement::In1() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGFEConvolveMatrixElement::In1()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGFEConvolveMatrixElement::In1() {
>>>>>>> upstream-releases
  return mStringAttributes[IN1].ToDOMAnimatedString(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedInteger> SVGFEConvolveMatrixElement::OrderX() {
  return mIntegerPairAttributes[ORDER].ToDOMAnimatedInteger(
      nsSVGIntegerPair::eFirst, this);
||||||| merged common ancestors
already_AddRefed<SVGAnimatedInteger>
SVGFEConvolveMatrixElement::OrderX()
{
  return mIntegerPairAttributes[ORDER].ToDOMAnimatedInteger(nsSVGIntegerPair::eFirst, this);
=======
already_AddRefed<DOMSVGAnimatedInteger> SVGFEConvolveMatrixElement::OrderX() {
  return mIntegerPairAttributes[ORDER].ToDOMAnimatedInteger(
      SVGAnimatedIntegerPair::eFirst, this);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedInteger> SVGFEConvolveMatrixElement::OrderY() {
  return mIntegerPairAttributes[ORDER].ToDOMAnimatedInteger(
      nsSVGIntegerPair::eSecond, this);
||||||| merged common ancestors
already_AddRefed<SVGAnimatedInteger>
SVGFEConvolveMatrixElement::OrderY()
{
  return mIntegerPairAttributes[ORDER].ToDOMAnimatedInteger(nsSVGIntegerPair::eSecond, this);
=======
already_AddRefed<DOMSVGAnimatedInteger> SVGFEConvolveMatrixElement::OrderY() {
  return mIntegerPairAttributes[ORDER].ToDOMAnimatedInteger(
      SVGAnimatedIntegerPair::eSecond, this);
>>>>>>> upstream-releases
}

already_AddRefed<DOMSVGAnimatedNumberList>
SVGFEConvolveMatrixElement::KernelMatrix() {
  return DOMSVGAnimatedNumberList::GetDOMWrapper(
      &mNumberListAttributes[KERNELMATRIX], this, KERNELMATRIX);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedInteger> SVGFEConvolveMatrixElement::TargetX() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedInteger>
SVGFEConvolveMatrixElement::TargetX()
{
=======
already_AddRefed<DOMSVGAnimatedInteger> SVGFEConvolveMatrixElement::TargetX() {
>>>>>>> upstream-releases
  return mIntegerAttributes[TARGET_X].ToDOMAnimatedInteger(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedInteger> SVGFEConvolveMatrixElement::TargetY() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedInteger>
SVGFEConvolveMatrixElement::TargetY()
{
=======
already_AddRefed<DOMSVGAnimatedInteger> SVGFEConvolveMatrixElement::TargetY() {
>>>>>>> upstream-releases
  return mIntegerAttributes[TARGET_Y].ToDOMAnimatedInteger(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration>
SVGFEConvolveMatrixElement::EdgeMode() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGFEConvolveMatrixElement::EdgeMode()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration>
SVGFEConvolveMatrixElement::EdgeMode() {
>>>>>>> upstream-releases
  return mEnumAttributes[EDGEMODE].ToDOMAnimatedEnum(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedBoolean>
SVGFEConvolveMatrixElement::PreserveAlpha() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedBoolean>
SVGFEConvolveMatrixElement::PreserveAlpha()
{
=======
already_AddRefed<DOMSVGAnimatedBoolean>
SVGFEConvolveMatrixElement::PreserveAlpha() {
>>>>>>> upstream-releases
  return mBooleanAttributes[PRESERVEALPHA].ToDOMAnimatedBoolean(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFEConvolveMatrixElement::Divisor() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEConvolveMatrixElement::Divisor()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFEConvolveMatrixElement::Divisor() {
>>>>>>> upstream-releases
  return mNumberAttributes[DIVISOR].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber> SVGFEConvolveMatrixElement::Bias() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEConvolveMatrixElement::Bias()
{
=======
already_AddRefed<DOMSVGAnimatedNumber> SVGFEConvolveMatrixElement::Bias() {
>>>>>>> upstream-releases
  return mNumberAttributes[BIAS].ToDOMAnimatedNumber(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber>
SVGFEConvolveMatrixElement::KernelUnitLengthX() {
  return mNumberPairAttributes[KERNEL_UNIT_LENGTH].ToDOMAnimatedNumber(
      nsSVGNumberPair::eFirst, this);
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEConvolveMatrixElement::KernelUnitLengthX()
{
  return mNumberPairAttributes[KERNEL_UNIT_LENGTH].ToDOMAnimatedNumber(nsSVGNumberPair::eFirst,
                                                                       this);
=======
already_AddRefed<DOMSVGAnimatedNumber>
SVGFEConvolveMatrixElement::KernelUnitLengthX() {
  return mNumberPairAttributes[KERNEL_UNIT_LENGTH].ToDOMAnimatedNumber(
      SVGAnimatedNumberPair::eFirst, this);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedNumber>
SVGFEConvolveMatrixElement::KernelUnitLengthY() {
  return mNumberPairAttributes[KERNEL_UNIT_LENGTH].ToDOMAnimatedNumber(
      nsSVGNumberPair::eSecond, this);
||||||| merged common ancestors
already_AddRefed<SVGAnimatedNumber>
SVGFEConvolveMatrixElement::KernelUnitLengthY()
{
  return mNumberPairAttributes[KERNEL_UNIT_LENGTH].ToDOMAnimatedNumber(nsSVGNumberPair::eSecond,
                                                                       this);
=======
already_AddRefed<DOMSVGAnimatedNumber>
SVGFEConvolveMatrixElement::KernelUnitLengthY() {
  return mNumberPairAttributes[KERNEL_UNIT_LENGTH].ToDOMAnimatedNumber(
      SVGAnimatedNumberPair::eSecond, this);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void SVGFEConvolveMatrixElement::GetSourceImageNames(
    nsTArray<nsSVGStringInfo>& aSources) {
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
||||||| merged common ancestors
void
SVGFEConvolveMatrixElement::GetSourceImageNames(nsTArray<nsSVGStringInfo>& aSources)
{
  aSources.AppendElement(nsSVGStringInfo(&mStringAttributes[IN1], this));
=======
void SVGFEConvolveMatrixElement::GetSourceImageNames(
    nsTArray<SVGStringInfo>& aSources) {
  aSources.AppendElement(SVGStringInfo(&mStringAttributes[IN1], this));
>>>>>>> upstream-releases
}

FilterPrimitiveDescription SVGFEConvolveMatrixElement::GetPrimitiveDescription(
    nsSVGFilterInstance* aInstance, const IntRect& aFilterSubregion,
    const nsTArray<bool>& aInputsAreTainted,
    nsTArray<RefPtr<SourceSurface>>& aInputImages) {
  FilterPrimitiveDescription failureDescription;

  const SVGNumberList& kernelMatrix =
      mNumberListAttributes[KERNELMATRIX].GetAnimValue();
  uint32_t kmLength = kernelMatrix.Length();

<<<<<<< HEAD
  int32_t orderX =
      mIntegerPairAttributes[ORDER].GetAnimValue(nsSVGIntegerPair::eFirst);
  int32_t orderY =
      mIntegerPairAttributes[ORDER].GetAnimValue(nsSVGIntegerPair::eSecond);
||||||| merged common ancestors
  int32_t orderX = mIntegerPairAttributes[ORDER].GetAnimValue(nsSVGIntegerPair::eFirst);
  int32_t orderY = mIntegerPairAttributes[ORDER].GetAnimValue(nsSVGIntegerPair::eSecond);
=======
  int32_t orderX = mIntegerPairAttributes[ORDER].GetAnimValue(
      SVGAnimatedIntegerPair::eFirst);
  int32_t orderY = mIntegerPairAttributes[ORDER].GetAnimValue(
      SVGAnimatedIntegerPair::eSecond);
>>>>>>> upstream-releases

  if (orderX <= 0 || orderY <= 0 ||
      static_cast<uint32_t>(orderX * orderY) != kmLength) {
    return failureDescription;
  }

  int32_t targetX, targetY;
  GetAnimatedIntegerValues(&targetX, &targetY, nullptr);

  if (mIntegerAttributes[TARGET_X].IsExplicitlySet()) {
    if (targetX < 0 || targetX >= orderX) return failureDescription;
  } else {
    targetX = orderX / 2;
  }
  if (mIntegerAttributes[TARGET_Y].IsExplicitlySet()) {
    if (targetY < 0 || targetY >= orderY) return failureDescription;
  } else {
    targetY = orderY / 2;
  }

  if (orderX > NS_SVG_OFFSCREEN_MAX_DIMENSION ||
      orderY > NS_SVG_OFFSCREEN_MAX_DIMENSION)
    return failureDescription;
  UniquePtr<float[]> kernel = MakeUniqueFallible<float[]>(orderX * orderY);
  if (!kernel) return failureDescription;
  for (uint32_t i = 0; i < kmLength; i++) {
    kernel[kmLength - 1 - i] = kernelMatrix[i];
  }

  float divisor;
  if (mNumberAttributes[DIVISOR].IsExplicitlySet()) {
    divisor = mNumberAttributes[DIVISOR].GetAnimValue();
    if (divisor == 0) return failureDescription;
  } else {
    divisor = kernel[0];
    for (uint32_t i = 1; i < kmLength; i++) divisor += kernel[i];
    if (divisor == 0) divisor = 1;
  }

  uint32_t edgeMode = mEnumAttributes[EDGEMODE].GetAnimValue();
  bool preserveAlpha = mBooleanAttributes[PRESERVEALPHA].GetAnimValue();
  float bias = mNumberAttributes[BIAS].GetAnimValue();

  Size kernelUnitLength = GetKernelUnitLength(
      aInstance, &mNumberPairAttributes[KERNEL_UNIT_LENGTH]);

  if (kernelUnitLength.width <= 0 || kernelUnitLength.height <= 0) {
    // According to spec, A negative or zero value is an error. See link below
    // for details.
    // https://www.w3.org/TR/SVG/filters.html#feConvolveMatrixElementKernelUnitLengthAttribute
    return failureDescription;
  }

  ConvolveMatrixAttributes atts;
  atts.mKernelSize = IntSize(orderX, orderY);
  atts.mKernelMatrix.AppendElements(&kernelMatrix[0], kmLength);
  atts.mDivisor = divisor;
  atts.mBias = bias;
  atts.mTarget = IntPoint(targetX, targetY);
  atts.mEdgeMode = edgeMode;
  atts.mKernelUnitLength = kernelUnitLength;
  atts.mPreserveAlpha = preserveAlpha;

  return FilterPrimitiveDescription(AsVariant(std::move(atts)));
}

bool SVGFEConvolveMatrixElement::AttributeAffectsRendering(
    int32_t aNameSpaceID, nsAtom* aAttribute) const {
  return SVGFEConvolveMatrixElementBase::AttributeAffectsRendering(
             aNameSpaceID, aAttribute) ||
         (aNameSpaceID == kNameSpaceID_None &&
          (aAttribute == nsGkAtoms::in || aAttribute == nsGkAtoms::divisor ||
           aAttribute == nsGkAtoms::bias ||
           aAttribute == nsGkAtoms::kernelUnitLength ||
           aAttribute == nsGkAtoms::targetX ||
           aAttribute == nsGkAtoms::targetY || aAttribute == nsGkAtoms::order ||
           aAttribute == nsGkAtoms::preserveAlpha ||
           aAttribute == nsGkAtoms::edgeMode ||
           aAttribute == nsGkAtoms::kernelMatrix));
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
nsSVGElement::NumberAttributesInfo SVGFEConvolveMatrixElement::GetNumberInfo() {
||||||| merged common ancestors
nsSVGElement::NumberAttributesInfo
SVGFEConvolveMatrixElement::GetNumberInfo()
{
=======
SVGElement::NumberAttributesInfo SVGFEConvolveMatrixElement::GetNumberInfo() {
>>>>>>> upstream-releases
  return NumberAttributesInfo(mNumberAttributes, sNumberInfo,
                              ArrayLength(sNumberInfo));
}

<<<<<<< HEAD
nsSVGElement::NumberPairAttributesInfo
SVGFEConvolveMatrixElement::GetNumberPairInfo() {
||||||| merged common ancestors
nsSVGElement::NumberPairAttributesInfo
SVGFEConvolveMatrixElement::GetNumberPairInfo()
{
=======
SVGElement::NumberPairAttributesInfo
SVGFEConvolveMatrixElement::GetNumberPairInfo() {
>>>>>>> upstream-releases
  return NumberPairAttributesInfo(mNumberPairAttributes, sNumberPairInfo,
                                  ArrayLength(sNumberPairInfo));
}

<<<<<<< HEAD
nsSVGElement::IntegerAttributesInfo
SVGFEConvolveMatrixElement::GetIntegerInfo() {
||||||| merged common ancestors
nsSVGElement::IntegerAttributesInfo
SVGFEConvolveMatrixElement::GetIntegerInfo()
{
=======
SVGElement::IntegerAttributesInfo SVGFEConvolveMatrixElement::GetIntegerInfo() {
>>>>>>> upstream-releases
  return IntegerAttributesInfo(mIntegerAttributes, sIntegerInfo,
                               ArrayLength(sIntegerInfo));
}

<<<<<<< HEAD
nsSVGElement::IntegerPairAttributesInfo
SVGFEConvolveMatrixElement::GetIntegerPairInfo() {
||||||| merged common ancestors
nsSVGElement::IntegerPairAttributesInfo
SVGFEConvolveMatrixElement::GetIntegerPairInfo()
{
=======
SVGElement::IntegerPairAttributesInfo
SVGFEConvolveMatrixElement::GetIntegerPairInfo() {
>>>>>>> upstream-releases
  return IntegerPairAttributesInfo(mIntegerPairAttributes, sIntegerPairInfo,
                                   ArrayLength(sIntegerPairInfo));
}

<<<<<<< HEAD
nsSVGElement::BooleanAttributesInfo
SVGFEConvolveMatrixElement::GetBooleanInfo() {
||||||| merged common ancestors
nsSVGElement::BooleanAttributesInfo
SVGFEConvolveMatrixElement::GetBooleanInfo()
{
=======
SVGElement::BooleanAttributesInfo SVGFEConvolveMatrixElement::GetBooleanInfo() {
>>>>>>> upstream-releases
  return BooleanAttributesInfo(mBooleanAttributes, sBooleanInfo,
                               ArrayLength(sBooleanInfo));
}

<<<<<<< HEAD
nsSVGElement::EnumAttributesInfo SVGFEConvolveMatrixElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
||||||| merged common ancestors
nsSVGElement::EnumAttributesInfo
SVGFEConvolveMatrixElement::GetEnumInfo()
{
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo,
                            ArrayLength(sEnumInfo));
=======
SVGElement::EnumAttributesInfo SVGFEConvolveMatrixElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsSVGElement::StringAttributesInfo SVGFEConvolveMatrixElement::GetStringInfo() {
||||||| merged common ancestors
nsSVGElement::StringAttributesInfo
SVGFEConvolveMatrixElement::GetStringInfo()
{
=======
SVGElement::StringAttributesInfo SVGFEConvolveMatrixElement::GetStringInfo() {
>>>>>>> upstream-releases
  return StringAttributesInfo(mStringAttributes, sStringInfo,
                              ArrayLength(sStringInfo));
}

<<<<<<< HEAD
nsSVGElement::NumberListAttributesInfo
SVGFEConvolveMatrixElement::GetNumberListInfo() {
||||||| merged common ancestors
nsSVGElement::NumberListAttributesInfo
SVGFEConvolveMatrixElement::GetNumberListInfo()
{
=======
SVGElement::NumberListAttributesInfo
SVGFEConvolveMatrixElement::GetNumberListInfo() {
>>>>>>> upstream-releases
  return NumberListAttributesInfo(mNumberListAttributes, sNumberListInfo,
                                  ArrayLength(sNumberListInfo));
}

}  // namespace dom
}  // namespace mozilla
