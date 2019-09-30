/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGMarkerElement.h"

#include "nsGkAtoms.h"
#include "DOMSVGAngle.h"
#include "SVGAnimatedPreserveAspectRatio.h"
#include "nsError.h"
#include "mozilla/ArrayUtils.h"
#include "mozilla/dom/SVGGeometryElement.h"
#include "mozilla/dom/SVGLengthBinding.h"
#include "mozilla/dom/SVGMarkerElementBinding.h"
#include "mozilla/gfx/Matrix.h"
#include "mozilla/FloatingPoint.h"
#include "mozilla/RefPtr.h"
#include "SVGContentUtils.h"

using namespace mozilla::gfx;
using namespace mozilla::dom::SVGMarkerElement_Binding;

NS_IMPL_NS_NEW_SVG_ELEMENT(Marker)

namespace mozilla {
namespace dom {

using namespace SVGAngle_Binding;

JSObject* SVGMarkerElement::WrapNode(JSContext* aCx,
                                     JS::Handle<JSObject*> aGivenProto) {
  return SVGMarkerElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
nsSVGElement::LengthInfo SVGMarkerElement::sLengthInfo[4] = {
    {nsGkAtoms::refX, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::X},
    {nsGkAtoms::refY, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::Y},
    {nsGkAtoms::markerWidth, 3, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::X},
    {nsGkAtoms::markerHeight, 3, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::Y},
||||||| merged common ancestors
nsSVGElement::LengthInfo SVGMarkerElement::sLengthInfo[4] =
{
  { nsGkAtoms::refX, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER, SVGContentUtils::X },
  { nsGkAtoms::refY, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER, SVGContentUtils::Y },
  { nsGkAtoms::markerWidth, 3, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER, SVGContentUtils::X },
  { nsGkAtoms::markerHeight, 3, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER, SVGContentUtils::Y },
=======
SVGElement::LengthInfo SVGMarkerElement::sLengthInfo[4] = {
    {nsGkAtoms::refX, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::X},
    {nsGkAtoms::refY, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::Y},
    {nsGkAtoms::markerWidth, 3, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::X},
    {nsGkAtoms::markerHeight, 3, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::Y},
>>>>>>> upstream-releases
};

<<<<<<< HEAD
nsSVGEnumMapping SVGMarkerElement::sUnitsMap[] = {
    {nsGkAtoms::strokeWidth, SVG_MARKERUNITS_STROKEWIDTH},
    {nsGkAtoms::userSpaceOnUse, SVG_MARKERUNITS_USERSPACEONUSE},
    {nullptr, 0}};
||||||| merged common ancestors
nsSVGEnumMapping SVGMarkerElement::sUnitsMap[] = {
  {nsGkAtoms::strokeWidth, SVG_MARKERUNITS_STROKEWIDTH},
  {nsGkAtoms::userSpaceOnUse, SVG_MARKERUNITS_USERSPACEONUSE},
  {nullptr, 0}
};
=======
SVGEnumMapping SVGMarkerElement::sUnitsMap[] = {
    {nsGkAtoms::strokeWidth, SVG_MARKERUNITS_STROKEWIDTH},
    {nsGkAtoms::userSpaceOnUse, SVG_MARKERUNITS_USERSPACEONUSE},
    {nullptr, 0}};
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGElement::EnumInfo SVGMarkerElement::sEnumInfo[1] = {
    {nsGkAtoms::markerUnits, sUnitsMap, SVG_MARKERUNITS_STROKEWIDTH}};

nsSVGElement::AngleInfo SVGMarkerElement::sAngleInfo[1] = {
    {nsGkAtoms::orient, 0, SVG_ANGLETYPE_UNSPECIFIED}};
||||||| merged common ancestors
nsSVGElement::EnumInfo SVGMarkerElement::sEnumInfo[1] =
{
  { nsGkAtoms::markerUnits,
    sUnitsMap,
    SVG_MARKERUNITS_STROKEWIDTH
  }
};

nsSVGElement::AngleInfo SVGMarkerElement::sAngleInfo[1] =
{
  { nsGkAtoms::orient, 0, SVG_ANGLETYPE_UNSPECIFIED }
};
=======
SVGElement::EnumInfo SVGMarkerElement::sEnumInfo[1] = {
    {nsGkAtoms::markerUnits, sUnitsMap, SVG_MARKERUNITS_STROKEWIDTH}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// Implementation

<<<<<<< HEAD
nsresult nsSVGOrientType::SetBaseValue(uint16_t aValue,
                                       nsSVGElement* aSVGElement) {
  if (aValue == SVG_MARKER_ORIENT_AUTO || aValue == SVG_MARKER_ORIENT_ANGLE ||
      aValue == SVG_MARKER_ORIENT_AUTO_START_REVERSE) {
    SetBaseValue(aValue);
    aSVGElement->SetAttr(kNameSpaceID_None, nsGkAtoms::orient, nullptr,
                         (aValue == SVG_MARKER_ORIENT_AUTO
                              ? NS_LITERAL_STRING("auto")
                              : aValue == SVG_MARKER_ORIENT_ANGLE
                                    ? NS_LITERAL_STRING("0")
                                    : NS_LITERAL_STRING("auto-start-reverse")),
                         true);
    return NS_OK;
  }
  return NS_ERROR_DOM_TYPE_ERR;
}

already_AddRefed<SVGAnimatedEnumeration> nsSVGOrientType::ToDOMAnimatedEnum(
    nsSVGElement* aSVGElement) {
  RefPtr<SVGAnimatedEnumeration> toReturn =
      new DOMAnimatedEnum(this, aSVGElement);
  return toReturn.forget();
}

SVGMarkerElement::SVGMarkerElement(
    already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGMarkerElementBase(std::move(aNodeInfo)), mCoordCtx(nullptr) {}
||||||| merged common ancestors
nsresult
nsSVGOrientType::SetBaseValue(uint16_t aValue,
                              nsSVGElement *aSVGElement)
{
  if (aValue == SVG_MARKER_ORIENT_AUTO ||
      aValue == SVG_MARKER_ORIENT_ANGLE ||
      aValue == SVG_MARKER_ORIENT_AUTO_START_REVERSE) {
    SetBaseValue(aValue);
    aSVGElement->SetAttr(
      kNameSpaceID_None, nsGkAtoms::orient, nullptr,
      (aValue == SVG_MARKER_ORIENT_AUTO ?
        NS_LITERAL_STRING("auto") :
        aValue == SVG_MARKER_ORIENT_ANGLE ?
          NS_LITERAL_STRING("0") :
          NS_LITERAL_STRING("auto-start-reverse")),
      true);
    return NS_OK;
  }
  return NS_ERROR_DOM_TYPE_ERR;
}

already_AddRefed<SVGAnimatedEnumeration>
nsSVGOrientType::ToDOMAnimatedEnum(nsSVGElement *aSVGElement)
{
  RefPtr<SVGAnimatedEnumeration> toReturn =
    new DOMAnimatedEnum(this, aSVGElement);
  return toReturn.forget();
}

SVGMarkerElement::SVGMarkerElement(already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
  : SVGMarkerElementBase(std::move(aNodeInfo)), mCoordCtx(nullptr)
{
}
=======
SVGMarkerElement::SVGMarkerElement(
    already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGMarkerElementBase(std::move(aNodeInfo)), mCoordCtx(nullptr) {}
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// nsINode methods

NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGMarkerElement)

//----------------------------------------------------------------------

already_AddRefed<SVGAnimatedRect> SVGMarkerElement::ViewBox() {
  return mViewBox.ToSVGAnimatedRect(this);
}

already_AddRefed<DOMSVGAnimatedPreserveAspectRatio>
SVGMarkerElement::PreserveAspectRatio() {
  return mPreserveAspectRatio.ToDOMAnimatedPreserveAspectRatio(this);
}

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGMarkerElement::RefX() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGMarkerElement::RefX()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGMarkerElement::RefX() {
>>>>>>> upstream-releases
  return mLengthAttributes[REFX].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGMarkerElement::RefY() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGMarkerElement::RefY()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGMarkerElement::RefY() {
>>>>>>> upstream-releases
  return mLengthAttributes[REFY].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration> SVGMarkerElement::MarkerUnits() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGMarkerElement::MarkerUnits()
{
=======
already_AddRefed<DOMSVGAnimatedEnumeration> SVGMarkerElement::MarkerUnits() {
>>>>>>> upstream-releases
  return mEnumAttributes[MARKERUNITS].ToDOMAnimatedEnum(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGMarkerElement::MarkerWidth() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGMarkerElement::MarkerWidth()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGMarkerElement::MarkerWidth() {
>>>>>>> upstream-releases
  return mLengthAttributes[MARKERWIDTH].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGMarkerElement::MarkerHeight() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGMarkerElement::MarkerHeight()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGMarkerElement::MarkerHeight() {
>>>>>>> upstream-releases
  return mLengthAttributes[MARKERHEIGHT].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedEnumeration> SVGMarkerElement::OrientType() {
  return mOrientType.ToDOMAnimatedEnum(this);
||||||| merged common ancestors
already_AddRefed<SVGAnimatedEnumeration>
SVGMarkerElement::OrientType()
{
  return mOrientType.ToDOMAnimatedEnum(this);
=======
already_AddRefed<DOMSVGAnimatedEnumeration> SVGMarkerElement::OrientType() {
  return mOrient.ToDOMAnimatedEnum(this);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedAngle> SVGMarkerElement::OrientAngle() {
  return mAngleAttributes[ORIENT].ToDOMAnimatedAngle(this);
||||||| merged common ancestors
already_AddRefed<SVGAnimatedAngle>
SVGMarkerElement::OrientAngle()
{
  return mAngleAttributes[ORIENT].ToDOMAnimatedAngle(this);
=======
already_AddRefed<DOMSVGAnimatedAngle> SVGMarkerElement::OrientAngle() {
  return mOrient.ToDOMAnimatedAngle(this);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void SVGMarkerElement::SetOrientToAuto() {
  SetAttr(kNameSpaceID_None, nsGkAtoms::orient, nullptr,
          NS_LITERAL_STRING("auto"), true);
||||||| merged common ancestors
void SVGMarkerElement::SetOrientToAuto()
{
  SetAttr(kNameSpaceID_None, nsGkAtoms::orient, nullptr,
          NS_LITERAL_STRING("auto"), true);
=======
void SVGMarkerElement::SetOrientToAuto() {
  mOrient.SetBaseType(SVG_MARKER_ORIENT_AUTO, this);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void SVGMarkerElement::SetOrientToAngle(SVGAngle& angle, ErrorResult& rv) {
||||||| merged common ancestors
void
SVGMarkerElement::SetOrientToAngle(SVGAngle& angle, ErrorResult& rv)
{
=======
void SVGMarkerElement::SetOrientToAngle(DOMSVGAngle& angle, ErrorResult& rv) {
>>>>>>> upstream-releases
  float f = angle.Value();
  if (!IsFinite(f)) {
    rv.Throw(NS_ERROR_DOM_SVG_WRONG_TYPE_ERR);
    return;
  }
  mOrient.SetBaseValue(f, angle.UnitType(), this, true);
}

//----------------------------------------------------------------------
// nsIContent methods

NS_IMETHODIMP_(bool)
SVGMarkerElement::IsAttributeMapped(const nsAtom* name) const {
  static const MappedAttributeEntry* const map[] = {sFEFloodMap,
                                                    sFiltersMap,
                                                    sFontSpecificationMap,
                                                    sGradientStopMap,
                                                    sLightingEffectsMap,
                                                    sMarkersMap,
                                                    sTextContentElementsMap,
                                                    sViewportsMap,
                                                    sColorMap,
                                                    sFillStrokeMap,
                                                    sGraphicsMap};

  return FindAttributeDependence(name, map) ||
<<<<<<< HEAD
         SVGMarkerElementBase::IsAttributeMapped(name);
}

//----------------------------------------------------------------------
// nsSVGElement methods

bool SVGMarkerElement::ParseAttribute(int32_t aNameSpaceID, nsAtom* aName,
                                      const nsAString& aValue,
                                      nsIPrincipal* aMaybeScriptedPrincipal,
                                      nsAttrValue& aResult) {
  if (aNameSpaceID == kNameSpaceID_None && aName == nsGkAtoms::orient) {
    if (aValue.EqualsLiteral("auto")) {
      mOrientType.SetBaseValue(SVG_MARKER_ORIENT_AUTO);
      aResult.SetTo(aValue);
      mAngleAttributes[ORIENT].SetBaseValue(0.f, SVG_ANGLETYPE_UNSPECIFIED,
                                            this, false);
      return true;
    }
    if (aValue.EqualsLiteral("auto-start-reverse")) {
      mOrientType.SetBaseValue(SVG_MARKER_ORIENT_AUTO_START_REVERSE);
      aResult.SetTo(aValue);
      mAngleAttributes[ORIENT].SetBaseValue(0.f, SVG_ANGLETYPE_UNSPECIFIED,
                                            this, false);
      return true;
    }
    mOrientType.SetBaseValue(SVG_MARKER_ORIENT_ANGLE);
  }
  return SVGMarkerElementBase::ParseAttribute(aNameSpaceID, aName, aValue,
                                              aMaybeScriptedPrincipal, aResult);
}

nsresult SVGMarkerElement::AfterSetAttr(int32_t aNamespaceID, nsAtom* aName,
                                        const nsAttrValue* aValue,
                                        const nsAttrValue* aOldValue,
                                        nsIPrincipal* aMaybeScriptedPrincipal,
                                        bool aNotify) {
  if (!aValue && aNamespaceID == kNameSpaceID_None &&
      aName == nsGkAtoms::orient) {
    mOrientType.SetBaseValue(SVG_MARKER_ORIENT_ANGLE);
  }

  return SVGMarkerElementBase::AfterSetAttr(
      aNamespaceID, aName, aValue, aOldValue, aMaybeScriptedPrincipal, aNotify);
||||||| merged common ancestors
    SVGMarkerElementBase::IsAttributeMapped(name);
}

//----------------------------------------------------------------------
// nsSVGElement methods

bool
SVGMarkerElement::ParseAttribute(int32_t aNameSpaceID, nsAtom* aName,
                                 const nsAString& aValue,
                                 nsIPrincipal* aMaybeScriptedPrincipal,
                                 nsAttrValue& aResult)
{
  if (aNameSpaceID == kNameSpaceID_None && aName == nsGkAtoms::orient) {
    if (aValue.EqualsLiteral("auto")) {
      mOrientType.SetBaseValue(SVG_MARKER_ORIENT_AUTO);
      aResult.SetTo(aValue);
      mAngleAttributes[ORIENT].SetBaseValue(0.f, SVG_ANGLETYPE_UNSPECIFIED,
                                            this, false);
      return true;
    }
    if (aValue.EqualsLiteral("auto-start-reverse")) {
      mOrientType.SetBaseValue(SVG_MARKER_ORIENT_AUTO_START_REVERSE);
      aResult.SetTo(aValue);
      mAngleAttributes[ORIENT].SetBaseValue(0.f, SVG_ANGLETYPE_UNSPECIFIED,
                                            this, false);
      return true;
    }
    mOrientType.SetBaseValue(SVG_MARKER_ORIENT_ANGLE);
  }
  return SVGMarkerElementBase::ParseAttribute(aNameSpaceID, aName,
                                              aValue,
                                              aMaybeScriptedPrincipal,
                                              aResult);
}

nsresult
SVGMarkerElement::AfterSetAttr(int32_t aNamespaceID, nsAtom* aName,
                                const nsAttrValue* aValue,
                                const nsAttrValue* aOldValue,
                                nsIPrincipal* aMaybeScriptedPrincipal,
                                bool aNotify)
{
  if (!aValue && aNamespaceID == kNameSpaceID_None &&
      aName == nsGkAtoms::orient) {
    mOrientType.SetBaseValue(SVG_MARKER_ORIENT_ANGLE);
  }

  return SVGMarkerElementBase::AfterSetAttr(aNamespaceID, aName,
                                            aValue, aOldValue,
                                            aMaybeScriptedPrincipal,
                                            aNotify);
=======
         SVGMarkerElementBase::IsAttributeMapped(name);
>>>>>>> upstream-releases
}

//----------------------------------------------------------------------
// SVGElement methods

void SVGMarkerElement::SetParentCoordCtxProvider(SVGViewportElement* aContext) {
  mCoordCtx = aContext;
  mViewBoxToViewportTransform = nullptr;
}

<<<<<<< HEAD
/* virtual */ bool SVGMarkerElement::HasValidDimensions() const {
||||||| merged common ancestors
/* virtual */ bool
SVGMarkerElement::HasValidDimensions() const
{
=======
/* virtual */
bool SVGMarkerElement::HasValidDimensions() const {
>>>>>>> upstream-releases
  return (!mLengthAttributes[MARKERWIDTH].IsExplicitlySet() ||
          mLengthAttributes[MARKERWIDTH].GetAnimValInSpecifiedUnits() > 0) &&
         (!mLengthAttributes[MARKERHEIGHT].IsExplicitlySet() ||
          mLengthAttributes[MARKERHEIGHT].GetAnimValInSpecifiedUnits() > 0);
}

<<<<<<< HEAD
nsSVGElement::LengthAttributesInfo SVGMarkerElement::GetLengthInfo() {
||||||| merged common ancestors
nsSVGElement::LengthAttributesInfo
SVGMarkerElement::GetLengthInfo()
{
=======
SVGElement::LengthAttributesInfo SVGMarkerElement::GetLengthInfo() {
>>>>>>> upstream-releases
  return LengthAttributesInfo(mLengthAttributes, sLengthInfo,
                              ArrayLength(sLengthInfo));
}

<<<<<<< HEAD
nsSVGElement::AngleAttributesInfo SVGMarkerElement::GetAngleInfo() {
  return AngleAttributesInfo(mAngleAttributes, sAngleInfo,
                             ArrayLength(sAngleInfo));
||||||| merged common ancestors
nsSVGElement::AngleAttributesInfo
SVGMarkerElement::GetAngleInfo()
{
  return AngleAttributesInfo(mAngleAttributes, sAngleInfo,
                             ArrayLength(sAngleInfo));
=======
SVGElement::EnumAttributesInfo SVGMarkerElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsSVGElement::EnumAttributesInfo SVGMarkerElement::GetEnumInfo() {
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo, ArrayLength(sEnumInfo));
}
||||||| merged common ancestors
nsSVGElement::EnumAttributesInfo
SVGMarkerElement::GetEnumInfo()
{
  return EnumAttributesInfo(mEnumAttributes, sEnumInfo,
                            ArrayLength(sEnumInfo));
}
=======
SVGAnimatedOrient* SVGMarkerElement::GetAnimatedOrient() { return &mOrient; }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGViewBox* SVGMarkerElement::GetViewBox() { return &mViewBox; }
||||||| merged common ancestors
nsSVGViewBox *
SVGMarkerElement::GetViewBox()
{
  return &mViewBox;
}
=======
SVGAnimatedViewBox* SVGMarkerElement::GetAnimatedViewBox() { return &mViewBox; }
>>>>>>> upstream-releases

<<<<<<< HEAD
SVGAnimatedPreserveAspectRatio* SVGMarkerElement::GetPreserveAspectRatio() {
||||||| merged common ancestors
SVGAnimatedPreserveAspectRatio *
SVGMarkerElement::GetPreserveAspectRatio()
{
=======
SVGAnimatedPreserveAspectRatio*
SVGMarkerElement::GetAnimatedPreserveAspectRatio() {
>>>>>>> upstream-releases
  return &mPreserveAspectRatio;
}

//----------------------------------------------------------------------
// public helpers

<<<<<<< HEAD
gfx::Matrix SVGMarkerElement::GetMarkerTransform(float aStrokeWidth,
                                                 const nsSVGMark& aMark) {
  float scale =
      mEnumAttributes[MARKERUNITS].GetAnimValue() == SVG_MARKERUNITS_STROKEWIDTH
          ? aStrokeWidth
          : 1.0f;
||||||| merged common ancestors
gfx::Matrix
SVGMarkerElement::GetMarkerTransform(float aStrokeWidth,
                                     const nsSVGMark& aMark)
{
  float scale = mEnumAttributes[MARKERUNITS].GetAnimValue() ==
                     SVG_MARKERUNITS_STROKEWIDTH ? aStrokeWidth : 1.0f;
=======
gfx::Matrix SVGMarkerElement::GetMarkerTransform(float aStrokeWidth,
                                                 const SVGMark& aMark) {
  float scale =
      mEnumAttributes[MARKERUNITS].GetAnimValue() == SVG_MARKERUNITS_STROKEWIDTH
          ? aStrokeWidth
          : 1.0f;
>>>>>>> upstream-releases

  float angle;
  switch (mOrient.GetAnimType()) {
    case SVG_MARKER_ORIENT_AUTO:
      angle = aMark.angle;
      break;
    case SVG_MARKER_ORIENT_AUTO_START_REVERSE:
      angle = aMark.angle + (aMark.type == SVGMark::eStart ? M_PI : 0.0f);
      break;
<<<<<<< HEAD
    default:  // SVG_MARKER_ORIENT_ANGLE
      angle = mAngleAttributes[ORIENT].GetAnimValue() * M_PI / 180.0f;
||||||| merged common ancestors
    default: // SVG_MARKER_ORIENT_ANGLE
      angle = mAngleAttributes[ORIENT].GetAnimValue() * M_PI / 180.0f;
=======
    default:  // SVG_MARKER_ORIENT_ANGLE
      angle = mOrient.GetAnimValue() * M_PI / 180.0f;
>>>>>>> upstream-releases
      break;
  }

  return gfx::Matrix(cos(angle) * scale, sin(angle) * scale,
                     -sin(angle) * scale, cos(angle) * scale, aMark.x, aMark.y);
}

<<<<<<< HEAD
nsSVGViewBoxRect SVGMarkerElement::GetViewBoxRect() {
||||||| merged common ancestors
nsSVGViewBoxRect
SVGMarkerElement::GetViewBoxRect()
{
=======
SVGViewBox SVGMarkerElement::GetViewBox() {
>>>>>>> upstream-releases
  if (mViewBox.HasRect()) {
    return mViewBox.GetAnimValue();
  }
<<<<<<< HEAD
  return nsSVGViewBoxRect(
      0, 0, mLengthAttributes[MARKERWIDTH].GetAnimValue(mCoordCtx),
      mLengthAttributes[MARKERHEIGHT].GetAnimValue(mCoordCtx));
||||||| merged common ancestors
  return nsSVGViewBoxRect(
           0, 0,
           mLengthAttributes[MARKERWIDTH].GetAnimValue(mCoordCtx),
           mLengthAttributes[MARKERHEIGHT].GetAnimValue(mCoordCtx));
=======
  return SVGViewBox(0, 0,
                    mLengthAttributes[MARKERWIDTH].GetAnimValue(mCoordCtx),
                    mLengthAttributes[MARKERHEIGHT].GetAnimValue(mCoordCtx));
>>>>>>> upstream-releases
}

gfx::Matrix SVGMarkerElement::GetViewBoxTransform() {
  if (!mViewBoxToViewportTransform) {
    float viewportWidth =
        mLengthAttributes[MARKERWIDTH].GetAnimValue(mCoordCtx);
    float viewportHeight =
        mLengthAttributes[MARKERHEIGHT].GetAnimValue(mCoordCtx);

    SVGViewBox viewbox = GetViewBox();

    MOZ_ASSERT(viewbox.width > 0.0f && viewbox.height > 0.0f,
               "Rendering should be disabled");

    gfx::Matrix viewBoxTM = SVGContentUtils::GetViewBoxTransform(
        viewportWidth, viewportHeight, viewbox.x, viewbox.y, viewbox.width,
        viewbox.height, mPreserveAspectRatio);

    float refX = mLengthAttributes[REFX].GetAnimValue(mCoordCtx);
    float refY = mLengthAttributes[REFY].GetAnimValue(mCoordCtx);

    gfx::Point ref = viewBoxTM.TransformPoint(gfx::Point(refX, refY));

    Matrix TM = viewBoxTM;
    TM.PostTranslate(-ref.x, -ref.y);

    mViewBoxToViewportTransform = new gfx::Matrix(TM);
  }

  return *mViewBoxToViewportTransform;
}

}  // namespace dom
}  // namespace mozilla
