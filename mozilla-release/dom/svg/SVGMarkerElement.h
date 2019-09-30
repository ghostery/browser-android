/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_SVGMarkerElement_h
#define mozilla_dom_SVGMarkerElement_h

#include "DOMSVGAnimatedAngle.h"
#include "DOMSVGAnimatedEnumeration.h"
#include "nsAutoPtr.h"
#include "SVGAnimatedEnumeration.h"
#include "SVGAnimatedLength.h"
#include "SVGAnimatedOrient.h"
#include "SVGAnimatedPreserveAspectRatio.h"
#include "SVGAnimatedViewBox.h"
#include "mozilla/Attributes.h"
#include "mozilla/dom/SVGElement.h"
#include "mozilla/dom/SVGMarkerElementBinding.h"

class nsSVGMarkerFrame;

nsresult NS_NewSVGMarkerElement(
    nsIContent** aResult, already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo);

namespace mozilla {

struct SVGMark;

namespace dom {

// Non-Exposed Marker Orientation Types
static const uint16_t SVG_MARKER_ORIENT_AUTO_START_REVERSE = 3;

<<<<<<< HEAD
class nsSVGOrientType {
 public:
  nsSVGOrientType()
      : mAnimVal(SVGMarkerElement_Binding::SVG_MARKER_ORIENT_ANGLE),
        mBaseVal(SVGMarkerElement_Binding::SVG_MARKER_ORIENT_ANGLE) {}

  nsresult SetBaseValue(uint16_t aValue, nsSVGElement* aSVGElement);

  // XXX FIXME like https://bugzilla.mozilla.org/show_bug.cgi?id=545550 but
  // without adding an mIsAnimated member...?
  void SetBaseValue(uint16_t aValue) { mAnimVal = mBaseVal = uint8_t(aValue); }
  // no need to notify, since nsSVGAngle does that
  void SetAnimValue(uint16_t aValue) { mAnimVal = uint8_t(aValue); }

  // we want to avoid exposing SVG_MARKER_ORIENT_AUTO_START_REVERSE to
  // Web content
  uint16_t GetBaseValue() const {
    return mAnimVal == SVG_MARKER_ORIENT_AUTO_START_REVERSE
               ? SVGMarkerElement_Binding::SVG_MARKER_ORIENT_UNKNOWN
               : mBaseVal;
  }
  uint16_t GetAnimValue() const {
    return mAnimVal == SVG_MARKER_ORIENT_AUTO_START_REVERSE
               ? SVGMarkerElement_Binding::SVG_MARKER_ORIENT_UNKNOWN
               : mAnimVal;
  }
  uint16_t GetAnimValueInternal() const { return mAnimVal; }

  already_AddRefed<SVGAnimatedEnumeration> ToDOMAnimatedEnum(
      nsSVGElement* aSVGElement);

 private:
  nsSVGEnumValue mAnimVal;
  nsSVGEnumValue mBaseVal;

  struct DOMAnimatedEnum final : public SVGAnimatedEnumeration {
    DOMAnimatedEnum(nsSVGOrientType* aVal, nsSVGElement* aSVGElement)
        : SVGAnimatedEnumeration(aSVGElement), mVal(aVal) {}

    nsSVGOrientType* mVal;  // kept alive because it belongs to content

    using mozilla::dom::SVGAnimatedEnumeration::SetBaseVal;
    virtual uint16_t BaseVal() override { return mVal->GetBaseValue(); }
    virtual void SetBaseVal(uint16_t aBaseVal, ErrorResult& aRv) override {
      aRv = mVal->SetBaseValue(aBaseVal, mSVGElement);
    }
    virtual uint16_t AnimVal() override { return mVal->GetAnimValue(); }
  };
};
||||||| merged common ancestors
class nsSVGOrientType
{
public:
  nsSVGOrientType()
   : mAnimVal(SVGMarkerElement_Binding::SVG_MARKER_ORIENT_ANGLE),
     mBaseVal(SVGMarkerElement_Binding::SVG_MARKER_ORIENT_ANGLE) {}

  nsresult SetBaseValue(uint16_t aValue,
                        nsSVGElement *aSVGElement);

  // XXX FIXME like https://bugzilla.mozilla.org/show_bug.cgi?id=545550 but
  // without adding an mIsAnimated member...?
  void SetBaseValue(uint16_t aValue)
    { mAnimVal = mBaseVal = uint8_t(aValue); }
  // no need to notify, since nsSVGAngle does that
  void SetAnimValue(uint16_t aValue)
    { mAnimVal = uint8_t(aValue); }

  // we want to avoid exposing SVG_MARKER_ORIENT_AUTO_START_REVERSE to
  // Web content
  uint16_t GetBaseValue() const
    { return mAnimVal == SVG_MARKER_ORIENT_AUTO_START_REVERSE ?
               SVGMarkerElement_Binding::SVG_MARKER_ORIENT_UNKNOWN : mBaseVal; }
  uint16_t GetAnimValue() const
    { return mAnimVal == SVG_MARKER_ORIENT_AUTO_START_REVERSE ?
               SVGMarkerElement_Binding::SVG_MARKER_ORIENT_UNKNOWN : mAnimVal; }
  uint16_t GetAnimValueInternal() const
    { return mAnimVal; }

  already_AddRefed<SVGAnimatedEnumeration>
    ToDOMAnimatedEnum(nsSVGElement* aSVGElement);

private:
  nsSVGEnumValue mAnimVal;
  nsSVGEnumValue mBaseVal;

  struct DOMAnimatedEnum final : public SVGAnimatedEnumeration
  {
    DOMAnimatedEnum(nsSVGOrientType* aVal,
                    nsSVGElement *aSVGElement)
      : SVGAnimatedEnumeration(aSVGElement)
      , mVal(aVal)
    {}

    nsSVGOrientType *mVal; // kept alive because it belongs to content

    using mozilla::dom::SVGAnimatedEnumeration::SetBaseVal;
    virtual uint16_t BaseVal() override
    {
      return mVal->GetBaseValue();
    }
    virtual void SetBaseVal(uint16_t aBaseVal, ErrorResult& aRv) override
    {
      aRv = mVal->SetBaseValue(aBaseVal, mSVGElement);
    }
    virtual uint16_t AnimVal() override
    {
      return mVal->GetAnimValue();
    }
  };
};
=======
typedef SVGElement SVGMarkerElementBase;
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef nsSVGElement SVGMarkerElementBase;

class SVGMarkerElement : public SVGMarkerElementBase {
||||||| merged common ancestors
typedef nsSVGElement SVGMarkerElementBase;

class SVGMarkerElement : public SVGMarkerElementBase
{
=======
class SVGMarkerElement : public SVGMarkerElementBase {
>>>>>>> upstream-releases
  friend class ::nsSVGMarkerFrame;

 protected:
  friend nsresult(::NS_NewSVGMarkerElement(
      nsIContent** aResult,
      already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo));
  explicit SVGMarkerElement(
      already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo);
  virtual JSObject* WrapNode(JSContext* cx,
                             JS::Handle<JSObject*> aGivenProto) override;

 public:
  // nsIContent interface
  NS_IMETHOD_(bool) IsAttributeMapped(const nsAtom* name) const override;

  // nsSVGSVGElement methods:
  virtual bool HasValidDimensions() const override;

  // public helpers
  gfx::Matrix GetMarkerTransform(float aStrokeWidth, const SVGMark& aMark);
  SVGViewBox GetViewBox();
  gfx::Matrix GetViewBoxTransform();

  virtual nsresult Clone(dom::NodeInfo*, nsINode** aResult) const override;

  // WebIDL
  already_AddRefed<SVGAnimatedRect> ViewBox();
  already_AddRefed<DOMSVGAnimatedPreserveAspectRatio> PreserveAspectRatio();
  already_AddRefed<DOMSVGAnimatedLength> RefX();
  already_AddRefed<DOMSVGAnimatedLength> RefY();
  already_AddRefed<DOMSVGAnimatedEnumeration> MarkerUnits();
  already_AddRefed<DOMSVGAnimatedLength> MarkerWidth();
  already_AddRefed<DOMSVGAnimatedLength> MarkerHeight();
  already_AddRefed<DOMSVGAnimatedEnumeration> OrientType();
  already_AddRefed<DOMSVGAnimatedAngle> OrientAngle();
  void SetOrientToAuto();
<<<<<<< HEAD
  void SetOrientToAngle(SVGAngle& angle, ErrorResult& rv);

 protected:
  virtual bool ParseAttribute(int32_t aNameSpaceID, nsAtom* aName,
                              const nsAString& aValue,
                              nsIPrincipal* aMaybeScriptedPrincipal,
                              nsAttrValue& aResult) override;
||||||| merged common ancestors
  void SetOrientToAngle(SVGAngle& angle, ErrorResult& rv);

protected:

  virtual bool ParseAttribute(int32_t aNameSpaceID, nsAtom* aName,
                                const nsAString& aValue,
                                nsIPrincipal* aMaybeScriptedPrincipal,
                                nsAttrValue& aResult) override;
=======
  void SetOrientToAngle(DOMSVGAngle& angle, ErrorResult& rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetParentCoordCtxProvider(SVGViewportElement* aContext);
||||||| merged common ancestors
  void SetParentCoordCtxProvider(SVGViewportElement *aContext);
=======
 protected:
  void SetParentCoordCtxProvider(SVGViewportElement* aContext);
>>>>>>> upstream-releases

  virtual LengthAttributesInfo GetLengthInfo() override;
  virtual EnumAttributesInfo GetEnumInfo() override;
<<<<<<< HEAD
  virtual nsSVGViewBox* GetViewBox() override;
  virtual SVGAnimatedPreserveAspectRatio* GetPreserveAspectRatio() override;
||||||| merged common ancestors
  virtual nsSVGViewBox *GetViewBox() override;
  virtual SVGAnimatedPreserveAspectRatio *GetPreserveAspectRatio() override;
=======
  virtual SVGAnimatedOrient* GetAnimatedOrient() override;
  virtual SVGAnimatedPreserveAspectRatio* GetAnimatedPreserveAspectRatio()
      override;
  virtual SVGAnimatedViewBox* GetAnimatedViewBox() override;
>>>>>>> upstream-releases

  enum { REFX, REFY, MARKERWIDTH, MARKERHEIGHT };
  SVGAnimatedLength mLengthAttributes[4];
  static LengthInfo sLengthInfo[4];

  enum { MARKERUNITS };
  SVGAnimatedEnumeration mEnumAttributes[1];
  static SVGEnumMapping sUnitsMap[];
  static EnumInfo sEnumInfo[1];

<<<<<<< HEAD
  enum { ORIENT };
  nsSVGAngle mAngleAttributes[1];
  static AngleInfo sAngleInfo[1];

  nsSVGViewBox mViewBox;
||||||| merged common ancestors
  enum { ORIENT };
  nsSVGAngle mAngleAttributes[1];
  static AngleInfo sAngleInfo[1];

  nsSVGViewBox             mViewBox;
=======
  SVGAnimatedOrient mOrient;
  SVGAnimatedViewBox mViewBox;
>>>>>>> upstream-releases
  SVGAnimatedPreserveAspectRatio mPreserveAspectRatio;

<<<<<<< HEAD
  // derived properties (from 'orient') handled separately
  nsSVGOrientType mOrientType;

  SVGViewportElement* mCoordCtx;
  nsAutoPtr<gfx::Matrix> mViewBoxToViewportTransform;
||||||| merged common ancestors
  // derived properties (from 'orient') handled separately
  nsSVGOrientType                        mOrientType;

  SVGViewportElement*                    mCoordCtx;
  nsAutoPtr<gfx::Matrix>                 mViewBoxToViewportTransform;
=======
  SVGViewportElement* mCoordCtx;
  nsAutoPtr<gfx::Matrix> mViewBoxToViewportTransform;
>>>>>>> upstream-releases
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_SVGMarkerElement_h
