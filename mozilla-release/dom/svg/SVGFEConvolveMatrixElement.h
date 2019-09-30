/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_SVGFEConvolveMatrixElement_h
#define mozilla_dom_SVGFEConvolveMatrixElement_h

#include "DOMSVGAnimatedNumberList.h"
#include "SVGAnimatedBoolean.h"
#include "SVGAnimatedEnumeration.h"
#include "SVGAnimatedInteger.h"
#include "SVGAnimatedIntegerPair.h"
#include "SVGAnimatedNumber.h"
#include "SVGAnimatedNumberList.h"
#include "SVGAnimatedString.h"
#include "SVGFilters.h"

nsresult NS_NewSVGFEConvolveMatrixElement(
    nsIContent** aResult, already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo);

namespace mozilla {

namespace dom {
<<<<<<< HEAD
class SVGAnimatedBoolean;

typedef nsSVGFE SVGFEConvolveMatrixElementBase;

class SVGFEConvolveMatrixElement : public SVGFEConvolveMatrixElementBase {
  friend nsresult(::NS_NewSVGFEConvolveMatrixElement(
      nsIContent** aResult,
      already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo));

 protected:
  explicit SVGFEConvolveMatrixElement(
      already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
      : SVGFEConvolveMatrixElementBase(std::move(aNodeInfo)) {}
  virtual JSObject* WrapNode(JSContext* aCx,
                             JS::Handle<JSObject*> aGivenProto) override;

 public:
  virtual FilterPrimitiveDescription GetPrimitiveDescription(
      nsSVGFilterInstance* aInstance, const IntRect& aFilterSubregion,
      const nsTArray<bool>& aInputsAreTainted,
      nsTArray<RefPtr<SourceSurface>>& aInputImages) override;
  virtual bool AttributeAffectsRendering(int32_t aNameSpaceID,
                                         nsAtom* aAttribute) const override;
  virtual nsSVGString& GetResultImageName() override {
    return mStringAttributes[RESULT];
||||||| merged common ancestors
class SVGAnimatedBoolean;

typedef nsSVGFE SVGFEConvolveMatrixElementBase;

class SVGFEConvolveMatrixElement : public SVGFEConvolveMatrixElementBase
{
  friend nsresult (::NS_NewSVGFEConvolveMatrixElement(nsIContent **aResult,
                                                      already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo));
protected:
  explicit SVGFEConvolveMatrixElement(already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGFEConvolveMatrixElementBase(std::move(aNodeInfo))
  {
=======
class DOMSVGAnimatedNumberList;
class DOMSVGAnimatedBoolean;

typedef SVGFE SVGFEConvolveMatrixElementBase;

class SVGFEConvolveMatrixElement : public SVGFEConvolveMatrixElementBase {
  friend nsresult(::NS_NewSVGFEConvolveMatrixElement(
      nsIContent** aResult,
      already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo));

 protected:
  explicit SVGFEConvolveMatrixElement(
      already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
      : SVGFEConvolveMatrixElementBase(std::move(aNodeInfo)) {}
  virtual JSObject* WrapNode(JSContext* aCx,
                             JS::Handle<JSObject*> aGivenProto) override;

 public:
  virtual FilterPrimitiveDescription GetPrimitiveDescription(
      nsSVGFilterInstance* aInstance, const IntRect& aFilterSubregion,
      const nsTArray<bool>& aInputsAreTainted,
      nsTArray<RefPtr<SourceSurface>>& aInputImages) override;
  virtual bool AttributeAffectsRendering(int32_t aNameSpaceID,
                                         nsAtom* aAttribute) const override;
  virtual SVGAnimatedString& GetResultImageName() override {
    return mStringAttributes[RESULT];
>>>>>>> upstream-releases
  }
<<<<<<< HEAD
  virtual void GetSourceImageNames(
      nsTArray<nsSVGStringInfo>& aSources) override;
||||||| merged common ancestors
  virtual JSObject* WrapNode(JSContext* aCx, JS::Handle<JSObject*> aGivenProto) override;

public:
  virtual FilterPrimitiveDescription
    GetPrimitiveDescription(nsSVGFilterInstance* aInstance,
                            const IntRect& aFilterSubregion,
                            const nsTArray<bool>& aInputsAreTainted,
                            nsTArray<RefPtr<SourceSurface>>& aInputImages) override;
  virtual bool AttributeAffectsRendering(
          int32_t aNameSpaceID, nsAtom* aAttribute) const override;
  virtual nsSVGString& GetResultImageName() override { return mStringAttributes[RESULT]; }
  virtual void GetSourceImageNames(nsTArray<nsSVGStringInfo>& aSources) override;
=======
  virtual void GetSourceImageNames(nsTArray<SVGStringInfo>& aSources) override;
>>>>>>> upstream-releases

  virtual nsresult Clone(dom::NodeInfo*, nsINode** aResult) const override;

  // WebIDL
  already_AddRefed<DOMSVGAnimatedString> In1();
  already_AddRefed<DOMSVGAnimatedInteger> OrderX();
  already_AddRefed<DOMSVGAnimatedInteger> OrderY();
  already_AddRefed<DOMSVGAnimatedNumberList> KernelMatrix();
<<<<<<< HEAD
  already_AddRefed<SVGAnimatedInteger> TargetX();
  already_AddRefed<SVGAnimatedInteger> TargetY();
  already_AddRefed<SVGAnimatedEnumeration> EdgeMode();
  already_AddRefed<SVGAnimatedBoolean> PreserveAlpha();
  already_AddRefed<SVGAnimatedNumber> Divisor();
  already_AddRefed<SVGAnimatedNumber> Bias();
  already_AddRefed<SVGAnimatedNumber> KernelUnitLengthX();
  already_AddRefed<SVGAnimatedNumber> KernelUnitLengthY();

 protected:
||||||| merged common ancestors
  already_AddRefed<SVGAnimatedInteger> TargetX();
  already_AddRefed<SVGAnimatedInteger> TargetY();
  already_AddRefed<SVGAnimatedEnumeration> EdgeMode();
  already_AddRefed<SVGAnimatedBoolean> PreserveAlpha();
  already_AddRefed<SVGAnimatedNumber> Divisor();
  already_AddRefed<SVGAnimatedNumber> Bias();
  already_AddRefed<SVGAnimatedNumber> KernelUnitLengthX();
  already_AddRefed<SVGAnimatedNumber> KernelUnitLengthY();

protected:
=======
  already_AddRefed<DOMSVGAnimatedInteger> TargetX();
  already_AddRefed<DOMSVGAnimatedInteger> TargetY();
  already_AddRefed<DOMSVGAnimatedEnumeration> EdgeMode();
  already_AddRefed<DOMSVGAnimatedBoolean> PreserveAlpha();
  already_AddRefed<DOMSVGAnimatedNumber> Divisor();
  already_AddRefed<DOMSVGAnimatedNumber> Bias();
  already_AddRefed<DOMSVGAnimatedNumber> KernelUnitLengthX();
  already_AddRefed<DOMSVGAnimatedNumber> KernelUnitLengthY();

 protected:
>>>>>>> upstream-releases
  virtual NumberAttributesInfo GetNumberInfo() override;
  virtual NumberPairAttributesInfo GetNumberPairInfo() override;
  virtual IntegerAttributesInfo GetIntegerInfo() override;
  virtual IntegerPairAttributesInfo GetIntegerPairInfo() override;
  virtual BooleanAttributesInfo GetBooleanInfo() override;
  virtual EnumAttributesInfo GetEnumInfo() override;
  virtual StringAttributesInfo GetStringInfo() override;
  virtual NumberListAttributesInfo GetNumberListInfo() override;

  enum { DIVISOR, BIAS };
  SVGAnimatedNumber mNumberAttributes[2];
  static NumberInfo sNumberInfo[2];

  enum { KERNEL_UNIT_LENGTH };
  SVGAnimatedNumberPair mNumberPairAttributes[1];
  static NumberPairInfo sNumberPairInfo[1];

  enum { TARGET_X, TARGET_Y };
  SVGAnimatedInteger mIntegerAttributes[2];
  static IntegerInfo sIntegerInfo[2];

  enum { ORDER };
  SVGAnimatedIntegerPair mIntegerPairAttributes[1];
  static IntegerPairInfo sIntegerPairInfo[1];

  enum { PRESERVEALPHA };
  SVGAnimatedBoolean mBooleanAttributes[1];
  static BooleanInfo sBooleanInfo[1];

  enum { EDGEMODE };
  SVGAnimatedEnumeration mEnumAttributes[1];
  static SVGEnumMapping sEdgeModeMap[];
  static EnumInfo sEnumInfo[1];

  enum { RESULT, IN1 };
  SVGAnimatedString mStringAttributes[2];
  static StringInfo sStringInfo[2];

  enum { KERNELMATRIX };
  SVGAnimatedNumberList mNumberListAttributes[1];
  static NumberListInfo sNumberListInfo[1];
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_SVGFEConvolveMatrixElement_h
