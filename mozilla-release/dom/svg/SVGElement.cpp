/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGElement.h"

#include "mozilla/dom/MutationEventBinding.h"
#include "mozilla/dom/SVGElementBinding.h"
#include "mozilla/dom/SVGGeometryElement.h"
#include "mozilla/dom/SVGLengthBinding.h"
#include "mozilla/dom/SVGSVGElement.h"
#include "mozilla/dom/SVGTests.h"
#include "mozilla/dom/SVGUnitTypesBinding.h"

#include "mozilla/ArrayUtils.h"
#include "mozilla/DebugOnly.h"
#include "mozilla/DeclarationBlock.h"
#include "mozilla/EventListenerManager.h"
#include "mozilla/InternalMutationEvent.h"
#include "mozilla/PresShell.h"
#include "mozilla/RestyleManager.h"
#include "mozilla/SMILAnimationController.h"
#include "mozilla/SVGContentUtils.h"
#include "mozilla/Unused.h"

#include "DOMSVGAnimatedEnumeration.h"
#include "mozAutoDocUpdate.h"
#include "nsAttrValueOrString.h"
#include "nsCSSProps.h"
#include "nsContentUtils.h"
#include "nsDOMCSSAttrDeclaration.h"
#include "nsICSSDeclaration.h"
#include "nsIContentInlines.h"
#include "mozilla/dom/Document.h"
#include "nsError.h"
#include "nsGkAtoms.h"
#include "nsIFrame.h"
#include "nsQueryObject.h"
#include "nsLayoutUtils.h"
#include "SVGAnimatedNumberList.h"
#include "SVGAnimatedLengthList.h"
#include "SVGAnimatedPointList.h"
#include "SVGAnimatedPathSegList.h"
#include "SVGAnimatedTransformList.h"
#include "SVGAnimatedBoolean.h"
#include "SVGAnimatedEnumeration.h"
#include "SVGAnimatedInteger.h"
#include "SVGAnimatedIntegerPair.h"
#include "SVGAnimatedLength.h"
#include "SVGAnimatedNumber.h"
#include "SVGAnimatedNumberPair.h"
#include "SVGAnimatedOrient.h"
#include "SVGAnimatedString.h"
#include "SVGAnimatedViewBox.h"
#include "SVGGeometryProperty.h"
#include "SVGMotionSMILAttr.h"
#include <stdarg.h>

// This is needed to ensure correct handling of calls to the
// vararg-list methods in this file:
//   SVGElement::GetAnimated{Length,Number,Integer}Values
// See bug 547964 for details:
static_assert(sizeof(void*) == sizeof(nullptr),
              "nullptr should be the correct size");

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsresult NS_NewSVGElement(
    Element** aResult, already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo) {
  RefPtr<nsSVGElement> it = new nsSVGElement(std::move(aNodeInfo));
||||||| merged common ancestors
nsresult
NS_NewSVGElement(Element **aResult, already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
{
  RefPtr<nsSVGElement> it = new nsSVGElement(std::move(aNodeInfo));
=======
nsresult NS_NewSVGElement(
    Element** aResult, already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo) {
  RefPtr<mozilla::dom::SVGElement> it =
      new mozilla::dom::SVGElement(std::move(aNodeInfo));
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsresult rv = it->Init();

  if (NS_FAILED(rv)) {
    return rv;
  }

  it.forget(aResult);
  return rv;
}

namespace mozilla {
namespace dom {
using namespace SVGUnitTypes_Binding;

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGEnumMapping nsSVGElement::sSVGUnitTypesMap[] = {
    {nsGkAtoms::userSpaceOnUse, SVG_UNIT_TYPE_USERSPACEONUSE},
    {nsGkAtoms::objectBoundingBox, SVG_UNIT_TYPE_OBJECTBOUNDINGBOX},
    {nullptr, 0}};
||||||| merged common ancestors
nsSVGEnumMapping nsSVGElement::sSVGUnitTypesMap[] = {
  {nsGkAtoms::userSpaceOnUse, SVG_UNIT_TYPE_USERSPACEONUSE},
  {nsGkAtoms::objectBoundingBox, SVG_UNIT_TYPE_OBJECTBOUNDINGBOX},
  {nullptr, 0}
};
=======
NS_IMPL_ELEMENT_CLONE_WITH_INIT(SVGElement)
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement::nsSVGElement(already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : nsSVGElementBase(std::move(aNodeInfo)) {}
||||||| merged common ancestors
nsSVGElement::nsSVGElement(already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
  : nsSVGElementBase(std::move(aNodeInfo))
{
}
=======
SVGEnumMapping SVGElement::sSVGUnitTypesMap[] = {
    {nsGkAtoms::userSpaceOnUse, SVG_UNIT_TYPE_USERSPACEONUSE},
    {nsGkAtoms::objectBoundingBox, SVG_UNIT_TYPE_OBJECTBOUNDINGBOX},
    {nullptr, 0}};

SVGElement::SVGElement(already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGElementBase(std::move(aNodeInfo)) {}
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement::~nsSVGElement() {
||||||| merged common ancestors
nsSVGElement::~nsSVGElement()
{
=======
SVGElement::~SVGElement() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  OwnerDoc()->UnscheduleSVGForPresAttrEvaluation(this);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
JSObject* nsSVGElement::WrapNode(JSContext* aCx,
                                 JS::Handle<JSObject*> aGivenProto) {
||||||| merged common ancestors
JSObject*
nsSVGElement::WrapNode(JSContext *aCx, JS::Handle<JSObject*> aGivenProto)
{
=======
JSObject* SVGElement::WrapNode(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return SVGElement_Binding::Wrap(aCx, this, aGivenProto);
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimateClass() {
||||||| merged common ancestors
void
nsSVGElement::DidAnimateClass()
{
=======
void SVGElement::DidAnimateClass() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  // For Servo, snapshot the element before we change it.
  PresShell* presShell = OwnerDoc()->GetPresShell();
  if (presShell) {
    if (nsPresContext* presContext = presShell->GetPresContext()) {
      presContext->RestyleManager()->ClassAttributeWillBeChangedBySMIL(this);
    }
  }

  nsAutoString src;
  mClassAttribute.GetAnimValue(src, this);
  if (!mClassAnimAttr) {
    mClassAnimAttr = new nsAttrValue();
  }
  mClassAnimAttr->ParseAtomArray(src);

  // FIXME(emilio): This re-selector-matches, but we do the snapshot stuff right
  // above... Is this needed anymore?
  if (presShell) {
    presShell->RestyleForAnimation(this, StyleRestyleHint_RESTYLE_SELF);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsresult nsSVGElement::Init() {
||||||| merged common ancestors
nsresult
nsSVGElement::Init()
{
=======
nsresult SVGElement::Init() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  // Set up length attributes - can't do this in the constructor
  // because we can't do a virtual call at that point

  LengthAttributesInfo lengthInfo = GetLengthInfo();

  uint32_t i;
  for (i = 0; i < lengthInfo.mLengthCount; i++) {
    lengthInfo.Reset(i);
  }

  NumberAttributesInfo numberInfo = GetNumberInfo();

  for (i = 0; i < numberInfo.mNumberCount; i++) {
    numberInfo.Reset(i);
  }

  NumberPairAttributesInfo numberPairInfo = GetNumberPairInfo();

  for (i = 0; i < numberPairInfo.mNumberPairCount; i++) {
    numberPairInfo.Reset(i);
  }

  IntegerAttributesInfo integerInfo = GetIntegerInfo();

  for (i = 0; i < integerInfo.mIntegerCount; i++) {
    integerInfo.Reset(i);
  }

  IntegerPairAttributesInfo integerPairInfo = GetIntegerPairInfo();

  for (i = 0; i < integerPairInfo.mIntegerPairCount; i++) {
    integerPairInfo.Reset(i);
  }

  BooleanAttributesInfo booleanInfo = GetBooleanInfo();

  for (i = 0; i < booleanInfo.mBooleanCount; i++) {
    booleanInfo.Reset(i);
  }

  EnumAttributesInfo enumInfo = GetEnumInfo();

  for (i = 0; i < enumInfo.mEnumCount; i++) {
    enumInfo.Reset(i);
  }

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
  nsSVGViewBox* viewBox = GetViewBox();
||||||| merged common ancestors
  nsSVGViewBox *viewBox = GetViewBox();
=======
  SVGAnimatedOrient* orient = GetAnimatedOrient();

  if (orient) {
    orient->Init();
  }

  SVGAnimatedViewBox* viewBox = GetAnimatedViewBox();
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

  if (viewBox) {
    viewBox->Init();
  }

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
  SVGAnimatedPreserveAspectRatio* preserveAspectRatio =
      GetPreserveAspectRatio();
||||||| merged common ancestors
  SVGAnimatedPreserveAspectRatio *preserveAspectRatio =
    GetPreserveAspectRatio();
=======
  SVGAnimatedPreserveAspectRatio* preserveAspectRatio =
      GetAnimatedPreserveAspectRatio();
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

  if (preserveAspectRatio) {
    preserveAspectRatio->Init();
  }

  LengthListAttributesInfo lengthListInfo = GetLengthListInfo();

  for (i = 0; i < lengthListInfo.mLengthListCount; i++) {
    lengthListInfo.Reset(i);
  }

  NumberListAttributesInfo numberListInfo = GetNumberListInfo();

  for (i = 0; i < numberListInfo.mNumberListCount; i++) {
    numberListInfo.Reset(i);
  }

  // No need to reset SVGPointList since the default value is always the same
  // (an empty list).

  // No need to reset SVGPathData since the default value is always the same
  // (an empty list).

  StringAttributesInfo stringInfo = GetStringInfo();

  for (i = 0; i < stringInfo.mStringCount; i++) {
    stringInfo.Reset(i);
  }

  return NS_OK;
}

//----------------------------------------------------------------------
// Implementation

//----------------------------------------------------------------------
// nsIContent methods

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsresult nsSVGElement::BindToTree(nsIDocument* aDocument, nsIContent* aParent,
                                  nsIContent* aBindingParent) {
  nsresult rv =
      nsSVGElementBase::BindToTree(aDocument, aParent, aBindingParent);
||||||| merged common ancestors
nsresult
nsSVGElement::BindToTree(nsIDocument* aDocument, nsIContent* aParent,
                         nsIContent* aBindingParent)
{
  nsresult rv = nsSVGElementBase::BindToTree(aDocument, aParent,
                                             aBindingParent);
=======
nsresult SVGElement::BindToTree(BindContext& aContext, nsINode& aParent) {
  nsresult rv = SVGElementBase::BindToTree(aContext, aParent);
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  NS_ENSURE_SUCCESS(rv, rv);

  if (!MayHaveStyle()) {
    return NS_OK;
  }
  const nsAttrValue* oldVal = mAttrs.GetAttr(nsGkAtoms::style);

  if (oldVal && oldVal->Type() == nsAttrValue::eCSSDeclaration) {
    // we need to force a reparse because the baseURI of the document
    // may have changed, and in particular because we may be clones of
    // XBL anonymous content now being bound to the document we should
    // render in and due to the hacky way in which we implement the
    // interaction of XBL and SVG resources.  Once we have a sane
    // ownerDocument on XBL anonymous content, this can all go away.
    nsAttrValue attrValue;
    nsAutoString stringValue;
    oldVal->ToString(stringValue);
    // Force in data doc, since we already have a style rule
    ParseStyleAttribute(stringValue, nullptr, attrValue, true);
    // Don't bother going through SetInlineStyleDeclaration; we don't
    // want to fire off mutation events or document notifications anyway
    bool oldValueSet;
    rv = mAttrs.SetAndSwapAttr(nsGkAtoms::style, attrValue, &oldValueSet);
    NS_ENSURE_SUCCESS(rv, rv);
  }

  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsresult nsSVGElement::AfterSetAttr(int32_t aNamespaceID, nsAtom* aName,
                                    const nsAttrValue* aValue,
                                    const nsAttrValue* aOldValue,
                                    nsIPrincipal* aSubjectPrincipal,
                                    bool aNotify) {
||||||| merged common ancestors
nsresult
nsSVGElement::AfterSetAttr(int32_t aNamespaceID, nsAtom* aName,
                           const nsAttrValue* aValue,
                           const nsAttrValue* aOldValue,
                           nsIPrincipal* aSubjectPrincipal,
                           bool aNotify)
{
=======
nsresult SVGElement::AfterSetAttr(int32_t aNamespaceID, nsAtom* aName,
                                  const nsAttrValue* aValue,
                                  const nsAttrValue* aOldValue,
                                  nsIPrincipal* aSubjectPrincipal,
                                  bool aNotify) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  // We don't currently use nsMappedAttributes within SVG. If this changes, we
  // need to be very careful because some nsAttrValues used by SVG point to
  // member data of SVG elements and if an nsAttrValue outlives the SVG element
  // whose data it points to (by virtue of being stored in
  // mAttrs->mMappedAttributes, meaning it's shared between
  // elements), the pointer will dangle. See bug 724680.
  MOZ_ASSERT(!mAttrs.HasMappedAttrs(),
             "Unexpected use of nsMappedAttributes within SVG");

  // If this is an svg presentation attribute we need to map it into
  // the content declaration block.
  // XXX For some reason incremental mapping doesn't work, so for now
  // just delete the style rule and lazily reconstruct it as needed).
  if (aNamespaceID == kNameSpaceID_None && IsAttributeMapped(aName)) {
    mContentDeclarationBlock = nullptr;
    OwnerDoc()->ScheduleSVGForPresAttrEvaluation(this);
  }

  if (IsEventAttributeName(aName) && aValue) {
    MOZ_ASSERT(aValue->Type() == nsAttrValue::eString,
               "Expected string value for script body");
    nsresult rv =
        SetEventHandler(GetEventNameForAttr(aName), aValue->GetStringValue());
    NS_ENSURE_SUCCESS(rv, rv);
  }

  return SVGElementBase::AfterSetAttr(aNamespaceID, aName, aValue, aOldValue,
                                      aSubjectPrincipal, aNotify);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
bool nsSVGElement::ParseAttribute(int32_t aNamespaceID, nsAtom* aAttribute,
                                  const nsAString& aValue,
                                  nsIPrincipal* aMaybeScriptedPrincipal,
                                  nsAttrValue& aResult) {
||||||| merged common ancestors
bool
nsSVGElement::ParseAttribute(int32_t aNamespaceID,
                             nsAtom* aAttribute,
                             const nsAString& aValue,
                             nsIPrincipal* aMaybeScriptedPrincipal,
                             nsAttrValue& aResult)
{
=======
bool SVGElement::ParseAttribute(int32_t aNamespaceID, nsAtom* aAttribute,
                                const nsAString& aValue,
                                nsIPrincipal* aMaybeScriptedPrincipal,
                                nsAttrValue& aResult) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsresult rv = NS_OK;
  bool foundMatch = false;
  bool didSetResult = false;

  if (aNamespaceID == kNameSpaceID_None) {
    // Check for SVGAnimatedLength attribute
    LengthAttributesInfo lengthInfo = GetLengthInfo();

    uint32_t i;
    for (i = 0; i < lengthInfo.mLengthCount; i++) {
      if (aAttribute == lengthInfo.mLengthInfo[i].mName) {
        rv = lengthInfo.mLengths[i].SetBaseValueString(aValue, this, false);
        if (NS_FAILED(rv)) {
          lengthInfo.Reset(i);
        } else {
          aResult.SetTo(lengthInfo.mLengths[i], &aValue);
          didSetResult = true;
        }
        foundMatch = true;
        break;
      }
    }

    if (!foundMatch) {
      // Check for SVGAnimatedLengthList attribute
      LengthListAttributesInfo lengthListInfo = GetLengthListInfo();
      for (i = 0; i < lengthListInfo.mLengthListCount; i++) {
        if (aAttribute == lengthListInfo.mLengthListInfo[i].mName) {
          rv = lengthListInfo.mLengthLists[i].SetBaseValueString(aValue);
          if (NS_FAILED(rv)) {
            lengthListInfo.Reset(i);
          } else {
            aResult.SetTo(lengthListInfo.mLengthLists[i].GetBaseValue(),
                          &aValue);
            didSetResult = true;
          }
          foundMatch = true;
          break;
        }
      }
    }

    if (!foundMatch) {
      // Check for SVGAnimatedNumberList attribute
      NumberListAttributesInfo numberListInfo = GetNumberListInfo();
      for (i = 0; i < numberListInfo.mNumberListCount; i++) {
        if (aAttribute == numberListInfo.mNumberListInfo[i].mName) {
          rv = numberListInfo.mNumberLists[i].SetBaseValueString(aValue);
          if (NS_FAILED(rv)) {
            numberListInfo.Reset(i);
          } else {
            aResult.SetTo(numberListInfo.mNumberLists[i].GetBaseValue(),
                          &aValue);
            didSetResult = true;
          }
          foundMatch = true;
          break;
        }
      }
    }

    if (!foundMatch) {
      // Check for SVGAnimatedPointList attribute
      if (GetPointListAttrName() == aAttribute) {
        SVGAnimatedPointList* pointList = GetAnimatedPointList();
        if (pointList) {
          pointList->SetBaseValueString(aValue);
          // The spec says we parse everything up to the failure, so we DON'T
          // need to check the result of SetBaseValueString or call
          // pointList->ClearBaseValue() if it fails
          aResult.SetTo(pointList->GetBaseValue(), &aValue);
          didSetResult = true;
          foundMatch = true;
        }
      }
    }

    if (!foundMatch) {
      // Check for SVGAnimatedPathSegList attribute
      if (GetPathDataAttrName() == aAttribute) {
        SVGAnimatedPathSegList* segList = GetAnimPathSegList();
        if (segList) {
          segList->SetBaseValueString(aValue);
          // The spec says we parse everything up to the failure, so we DON'T
          // need to check the result of SetBaseValueString or call
          // segList->ClearBaseValue() if it fails
          aResult.SetTo(segList->GetBaseValue(), &aValue);
          didSetResult = true;
          foundMatch = true;
        }
      }
    }

    if (!foundMatch) {
      // Check for SVGAnimatedNumber attribute
      NumberAttributesInfo numberInfo = GetNumberInfo();
      for (i = 0; i < numberInfo.mNumberCount; i++) {
        if (aAttribute == numberInfo.mNumberInfo[i].mName) {
          rv = numberInfo.mNumbers[i].SetBaseValueString(aValue, this);
          if (NS_FAILED(rv)) {
            numberInfo.Reset(i);
          } else {
            aResult.SetTo(numberInfo.mNumbers[i].GetBaseValue(), &aValue);
            didSetResult = true;
          }
          foundMatch = true;
          break;
        }
      }
    }

    if (!foundMatch) {
      // Check for SVGAnimatedNumberPair attribute
      NumberPairAttributesInfo numberPairInfo = GetNumberPairInfo();
      for (i = 0; i < numberPairInfo.mNumberPairCount; i++) {
        if (aAttribute == numberPairInfo.mNumberPairInfo[i].mName) {
          rv = numberPairInfo.mNumberPairs[i].SetBaseValueString(aValue, this);
          if (NS_FAILED(rv)) {
            numberPairInfo.Reset(i);
          } else {
            aResult.SetTo(numberPairInfo.mNumberPairs[i], &aValue);
            didSetResult = true;
          }
          foundMatch = true;
          break;
        }
      }
    }

    if (!foundMatch) {
      // Check for SVGAnimatedInteger attribute
      IntegerAttributesInfo integerInfo = GetIntegerInfo();
      for (i = 0; i < integerInfo.mIntegerCount; i++) {
        if (aAttribute == integerInfo.mIntegerInfo[i].mName) {
          rv = integerInfo.mIntegers[i].SetBaseValueString(aValue, this);
          if (NS_FAILED(rv)) {
            integerInfo.Reset(i);
          } else {
            aResult.SetTo(integerInfo.mIntegers[i].GetBaseValue(), &aValue);
            didSetResult = true;
          }
          foundMatch = true;
          break;
        }
      }
    }

    if (!foundMatch) {
      // Check for SVGAnimatedIntegerPair attribute
      IntegerPairAttributesInfo integerPairInfo = GetIntegerPairInfo();
      for (i = 0; i < integerPairInfo.mIntegerPairCount; i++) {
        if (aAttribute == integerPairInfo.mIntegerPairInfo[i].mName) {
          rv =
              integerPairInfo.mIntegerPairs[i].SetBaseValueString(aValue, this);
          if (NS_FAILED(rv)) {
            integerPairInfo.Reset(i);
          } else {
            aResult.SetTo(integerPairInfo.mIntegerPairs[i], &aValue);
            didSetResult = true;
          }
          foundMatch = true;
          break;
        }
      }
    }

    if (!foundMatch) {
      // Check for SVGAnimatedBoolean attribute
      BooleanAttributesInfo booleanInfo = GetBooleanInfo();
      for (i = 0; i < booleanInfo.mBooleanCount; i++) {
        if (aAttribute == booleanInfo.mBooleanInfo[i].mName) {
          nsAtom* valAtom = NS_GetStaticAtom(aValue);
          rv = valAtom
                   ? booleanInfo.mBooleans[i].SetBaseValueAtom(valAtom, this)
                   : NS_ERROR_DOM_SYNTAX_ERR;
          if (NS_FAILED(rv)) {
            booleanInfo.Reset(i);
          } else {
            aResult.SetTo(valAtom);
            didSetResult = true;
          }
          foundMatch = true;
          break;
        }
      }
    }

    if (!foundMatch) {
      // Check for SVGAnimatedEnumeration attribute
      EnumAttributesInfo enumInfo = GetEnumInfo();
      for (i = 0; i < enumInfo.mEnumCount; i++) {
        if (aAttribute == enumInfo.mEnumInfo[i].mName) {
          RefPtr<nsAtom> valAtom = NS_Atomize(aValue);
          rv = enumInfo.mEnums[i].SetBaseValueAtom(valAtom, this);
          if (NS_FAILED(rv)) {
            enumInfo.SetUnknownValue(i);
          } else {
            aResult.SetTo(valAtom);
            didSetResult = true;
          }
          foundMatch = true;
          break;
        }
      }
    }

    if (!foundMatch) {
      // Check for conditional processing attributes
      nsCOMPtr<SVGTests> tests = do_QueryObject(this);
      if (tests && tests->ParseConditionalProcessingAttribute(
                       aAttribute, aValue, aResult)) {
        foundMatch = true;
      }
    }

    if (!foundMatch) {
      // Check for StringList attribute
      StringListAttributesInfo stringListInfo = GetStringListInfo();
      for (i = 0; i < stringListInfo.mStringListCount; i++) {
        if (aAttribute == stringListInfo.mStringListInfo[i].mName) {
          rv = stringListInfo.mStringLists[i].SetValue(aValue);
          if (NS_FAILED(rv)) {
            stringListInfo.Reset(i);
          } else {
            aResult.SetTo(stringListInfo.mStringLists[i], &aValue);
            didSetResult = true;
          }
          foundMatch = true;
          break;
        }
      }
    }

    if (!foundMatch) {
      // Check for orient attribute
      if (aAttribute == nsGkAtoms::orient) {
        SVGAnimatedOrient* orient = GetAnimatedOrient();
        if (orient) {
          rv = orient->SetBaseValueString(aValue, this, false);
          if (NS_FAILED(rv)) {
            orient->Init();
          } else {
            aResult.SetTo(*orient, &aValue);
            didSetResult = true;
          }
          foundMatch = true;
        }
        // Check for viewBox attribute
      } else if (aAttribute == nsGkAtoms::viewBox) {
        SVGAnimatedViewBox* viewBox = GetAnimatedViewBox();
        if (viewBox) {
          rv = viewBox->SetBaseValueString(aValue, this, false);
          if (NS_FAILED(rv)) {
            viewBox->Init();
          } else {
            aResult.SetTo(*viewBox, &aValue);
            didSetResult = true;
          }
          foundMatch = true;
        }
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
        // Check for SVGAnimatedPreserveAspectRatio attribute
||||||| merged common ancestors
      // Check for SVGAnimatedPreserveAspectRatio attribute
=======
        // Check for preserveAspectRatio attribute
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
      } else if (aAttribute == nsGkAtoms::preserveAspectRatio) {
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
        SVGAnimatedPreserveAspectRatio* preserveAspectRatio =
            GetPreserveAspectRatio();
||||||| merged common ancestors
        SVGAnimatedPreserveAspectRatio *preserveAspectRatio =
          GetPreserveAspectRatio();
=======
        SVGAnimatedPreserveAspectRatio* preserveAspectRatio =
            GetAnimatedPreserveAspectRatio();
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
        if (preserveAspectRatio) {
          rv = preserveAspectRatio->SetBaseValueString(aValue, this, false);
          if (NS_FAILED(rv)) {
            preserveAspectRatio->Init();
          } else {
            aResult.SetTo(*preserveAspectRatio, &aValue);
            didSetResult = true;
          }
          foundMatch = true;
        }
        // Check for SVGAnimatedTransformList attribute
      } else if (GetTransformListAttrName() == aAttribute) {
        // The transform attribute is being set, so we must ensure that the
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
        // nsSVGAnimatedTransformList is/has been allocated:
        nsSVGAnimatedTransformList* transformList =
            GetAnimatedTransformList(DO_ALLOCATE);
||||||| merged common ancestors
        // nsSVGAnimatedTransformList is/has been allocated:
        nsSVGAnimatedTransformList *transformList =
          GetAnimatedTransformList(DO_ALLOCATE);
=======
        // SVGAnimatedTransformList is/has been allocated:
        SVGAnimatedTransformList* transformList =
            GetAnimatedTransformList(DO_ALLOCATE);
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
        rv = transformList->SetBaseValueString(aValue, this);
        if (NS_FAILED(rv)) {
          transformList->ClearBaseValue();
        } else {
          aResult.SetTo(transformList->GetBaseValue(), &aValue);
          didSetResult = true;
        }
        foundMatch = true;
      } else if (aAttribute == nsGkAtoms::tabindex) {
        didSetResult = aResult.ParseIntValue(aValue);
        foundMatch = true;
      }
    }

    if (aAttribute == nsGkAtoms::_class) {
      mClassAttribute.SetBaseValue(aValue, this, false);
      aResult.ParseAtomArray(aValue);
      return true;
    }

    if (aAttribute == nsGkAtoms::rel) {
      aResult.ParseAtomArray(aValue);
      return true;
    }
  }

  if (!foundMatch) {
    // Check for SVGAnimatedString attribute
    StringAttributesInfo stringInfo = GetStringInfo();
    for (uint32_t i = 0; i < stringInfo.mStringCount; i++) {
      if (aNamespaceID == stringInfo.mStringInfo[i].mNamespaceID &&
          aAttribute == stringInfo.mStringInfo[i].mName) {
        stringInfo.mStrings[i].SetBaseValue(aValue, this, false);
        foundMatch = true;
        break;
      }
    }
  }

  if (foundMatch) {
    if (NS_FAILED(rv)) {
      ReportAttributeParseFailure(OwnerDoc(), aAttribute, aValue);
      return false;
    }
    if (!didSetResult) {
      aResult.SetTo(aValue);
    }
    return true;
  }

  return SVGElementBase::ParseAttribute(aNamespaceID, aAttribute, aValue,
                                        aMaybeScriptedPrincipal, aResult);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::UnsetAttrInternal(int32_t aNamespaceID, nsAtom* aName,
                                     bool aNotify) {
||||||| merged common ancestors
void
nsSVGElement::UnsetAttrInternal(int32_t aNamespaceID, nsAtom* aName,
                                bool aNotify)
{
=======
void SVGElement::UnsetAttrInternal(int32_t aNamespaceID, nsAtom* aName,
                                   bool aNotify) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  // XXXbz there's a bunch of redundancy here with AfterSetAttr.
  // Maybe consolidate?

  if (aNamespaceID == kNameSpaceID_None) {
    // If this is an svg presentation attribute, remove declaration block to
    // force an update
    if (IsAttributeMapped(aName)) {
      mContentDeclarationBlock = nullptr;
    }

    if (IsEventAttributeName(aName)) {
      EventListenerManager* manager = GetExistingListenerManager();
      if (manager) {
        nsAtom* eventName = GetEventNameForAttr(aName);
        manager->RemoveEventHandler(eventName);
      }
      return;
    }

    // Check if this is a length attribute going away
    LengthAttributesInfo lenInfo = GetLengthInfo();

    for (uint32_t i = 0; i < lenInfo.mLengthCount; i++) {
      if (aName == lenInfo.mLengthInfo[i].mName) {
        MaybeSerializeAttrBeforeRemoval(aName, aNotify);
        lenInfo.Reset(i);
        return;
      }
    }

    // Check if this is a length list attribute going away
    LengthListAttributesInfo lengthListInfo = GetLengthListInfo();

    for (uint32_t i = 0; i < lengthListInfo.mLengthListCount; i++) {
      if (aName == lengthListInfo.mLengthListInfo[i].mName) {
        MaybeSerializeAttrBeforeRemoval(aName, aNotify);
        lengthListInfo.Reset(i);
        return;
      }
    }

    // Check if this is a number list attribute going away
    NumberListAttributesInfo numberListInfo = GetNumberListInfo();

    for (uint32_t i = 0; i < numberListInfo.mNumberListCount; i++) {
      if (aName == numberListInfo.mNumberListInfo[i].mName) {
        MaybeSerializeAttrBeforeRemoval(aName, aNotify);
        numberListInfo.Reset(i);
        return;
      }
    }

    // Check if this is a point list attribute going away
    if (GetPointListAttrName() == aName) {
      SVGAnimatedPointList* pointList = GetAnimatedPointList();
      if (pointList) {
        MaybeSerializeAttrBeforeRemoval(aName, aNotify);
        pointList->ClearBaseValue();
        return;
      }
    }

    // Check if this is a path segment list attribute going away
    if (GetPathDataAttrName() == aName) {
      SVGAnimatedPathSegList* segList = GetAnimPathSegList();
      if (segList) {
        MaybeSerializeAttrBeforeRemoval(aName, aNotify);
        segList->ClearBaseValue();
        return;
      }
    }

    // Check if this is a number attribute going away
    NumberAttributesInfo numInfo = GetNumberInfo();

    for (uint32_t i = 0; i < numInfo.mNumberCount; i++) {
      if (aName == numInfo.mNumberInfo[i].mName) {
        numInfo.Reset(i);
        return;
      }
    }

    // Check if this is a number pair attribute going away
    NumberPairAttributesInfo numPairInfo = GetNumberPairInfo();

    for (uint32_t i = 0; i < numPairInfo.mNumberPairCount; i++) {
      if (aName == numPairInfo.mNumberPairInfo[i].mName) {
        MaybeSerializeAttrBeforeRemoval(aName, aNotify);
        numPairInfo.Reset(i);
        return;
      }
    }

    // Check if this is an integer attribute going away
    IntegerAttributesInfo intInfo = GetIntegerInfo();

    for (uint32_t i = 0; i < intInfo.mIntegerCount; i++) {
      if (aName == intInfo.mIntegerInfo[i].mName) {
        intInfo.Reset(i);
        return;
      }
    }

    // Check if this is an integer pair attribute going away
    IntegerPairAttributesInfo intPairInfo = GetIntegerPairInfo();

    for (uint32_t i = 0; i < intPairInfo.mIntegerPairCount; i++) {
      if (aName == intPairInfo.mIntegerPairInfo[i].mName) {
        MaybeSerializeAttrBeforeRemoval(aName, aNotify);
        intPairInfo.Reset(i);
        return;
      }
    }

    // Check if this is a boolean attribute going away
    BooleanAttributesInfo boolInfo = GetBooleanInfo();

    for (uint32_t i = 0; i < boolInfo.mBooleanCount; i++) {
      if (aName == boolInfo.mBooleanInfo[i].mName) {
        boolInfo.Reset(i);
        return;
      }
    }

    // Check if this is an enum attribute going away
    EnumAttributesInfo enumInfo = GetEnumInfo();

    for (uint32_t i = 0; i < enumInfo.mEnumCount; i++) {
      if (aName == enumInfo.mEnumInfo[i].mName) {
        enumInfo.Reset(i);
        return;
      }
    }

    // Check if this is an orient attribute going away
    if (aName == nsGkAtoms::orient) {
      SVGAnimatedOrient* orient = GetAnimatedOrient();
      if (orient) {
        MaybeSerializeAttrBeforeRemoval(aName, aNotify);
        orient->Init();
        return;
      }
    }

    // Check if this is a viewBox attribute going away
    if (aName == nsGkAtoms::viewBox) {
      SVGAnimatedViewBox* viewBox = GetAnimatedViewBox();
      if (viewBox) {
        MaybeSerializeAttrBeforeRemoval(aName, aNotify);
        viewBox->Init();
        return;
      }
    }

    // Check if this is a preserveAspectRatio attribute going away
    if (aName == nsGkAtoms::preserveAspectRatio) {
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
      SVGAnimatedPreserveAspectRatio* preserveAspectRatio =
          GetPreserveAspectRatio();
||||||| merged common ancestors
      SVGAnimatedPreserveAspectRatio *preserveAspectRatio =
        GetPreserveAspectRatio();
=======
      SVGAnimatedPreserveAspectRatio* preserveAspectRatio =
          GetAnimatedPreserveAspectRatio();
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
      if (preserveAspectRatio) {
        MaybeSerializeAttrBeforeRemoval(aName, aNotify);
        preserveAspectRatio->Init();
        return;
      }
    }

    // Check if this is a transform list attribute going away
    if (GetTransformListAttrName() == aName) {
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
      nsSVGAnimatedTransformList* transformList = GetAnimatedTransformList();
||||||| merged common ancestors
      nsSVGAnimatedTransformList *transformList = GetAnimatedTransformList();
=======
      SVGAnimatedTransformList* transformList = GetAnimatedTransformList();
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
      if (transformList) {
        MaybeSerializeAttrBeforeRemoval(aName, aNotify);
        transformList->ClearBaseValue();
        return;
      }
    }

    // Check for conditional processing attributes
    nsCOMPtr<SVGTests> tests = do_QueryObject(this);
    if (tests && tests->IsConditionalProcessingAttribute(aName)) {
      MaybeSerializeAttrBeforeRemoval(aName, aNotify);
      tests->UnsetAttr(aName);
      return;
    }

    // Check if this is a string list attribute going away
    StringListAttributesInfo stringListInfo = GetStringListInfo();

    for (uint32_t i = 0; i < stringListInfo.mStringListCount; i++) {
      if (aName == stringListInfo.mStringListInfo[i].mName) {
        MaybeSerializeAttrBeforeRemoval(aName, aNotify);
        stringListInfo.Reset(i);
        return;
      }
    }

    if (aName == nsGkAtoms::_class) {
      mClassAttribute.Init();
      return;
    }
  }

  // Check if this is a string attribute going away
  StringAttributesInfo stringInfo = GetStringInfo();

  for (uint32_t i = 0; i < stringInfo.mStringCount; i++) {
    if (aNamespaceID == stringInfo.mStringInfo[i].mNamespaceID &&
        aName == stringInfo.mStringInfo[i].mName) {
      stringInfo.Reset(i);
      return;
    }
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsresult nsSVGElement::BeforeSetAttr(int32_t aNamespaceID, nsAtom* aName,
                                     const nsAttrValueOrString* aValue,
                                     bool aNotify) {
||||||| merged common ancestors
nsresult
nsSVGElement::BeforeSetAttr(int32_t aNamespaceID, nsAtom* aName,
                            const nsAttrValueOrString* aValue,
                            bool aNotify)
{
=======
nsresult SVGElement::BeforeSetAttr(int32_t aNamespaceID, nsAtom* aName,
                                   const nsAttrValueOrString* aValue,
                                   bool aNotify) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  if (!aValue) {
    UnsetAttrInternal(aNamespaceID, aName, aNotify);
  }
  return SVGElementBase::BeforeSetAttr(aNamespaceID, aName, aValue, aNotify);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsChangeHint nsSVGElement::GetAttributeChangeHint(const nsAtom* aAttribute,
                                                  int32_t aModType) const {
||||||| merged common ancestors
nsChangeHint
nsSVGElement::GetAttributeChangeHint(const nsAtom* aAttribute,
                                     int32_t aModType) const
{
=======
nsChangeHint SVGElement::GetAttributeChangeHint(const nsAtom* aAttribute,
                                                int32_t aModType) const {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsChangeHint retval =
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
      nsSVGElementBase::GetAttributeChangeHint(aAttribute, aModType);
||||||| merged common ancestors
    nsSVGElementBase::GetAttributeChangeHint(aAttribute, aModType);
=======
      SVGElementBase::GetAttributeChangeHint(aAttribute, aModType);
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

  nsCOMPtr<SVGTests> tests = do_QueryObject(const_cast<SVGElement*>(this));
  if (tests && tests->IsConditionalProcessingAttribute(aAttribute)) {
    // It would be nice to only reconstruct the frame if the value returned by
    // SVGTests::PassesConditionalProcessingTests has changed, but we don't
    // know that
    retval |= nsChangeHint_ReconstructFrame;
  }
  return retval;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
bool nsSVGElement::IsNodeOfType(uint32_t aFlags) const { return false; }
||||||| merged common ancestors
bool
nsSVGElement::IsNodeOfType(uint32_t aFlags) const
{
  return false;
}
=======
bool SVGElement::IsNodeOfType(uint32_t aFlags) const { return false; }
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::NodeInfoChanged(nsIDocument* aOldDoc) {
  nsSVGElementBase::NodeInfoChanged(aOldDoc);
||||||| merged common ancestors
void
nsSVGElement::NodeInfoChanged(nsIDocument* aOldDoc)
{
  nsSVGElementBase::NodeInfoChanged(aOldDoc);
=======
void SVGElement::NodeInfoChanged(Document* aOldDoc) {
  SVGElementBase::NodeInfoChanged(aOldDoc);
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  aOldDoc->UnscheduleSVGForPresAttrEvaluation(this);
  mContentDeclarationBlock = nullptr;
  OwnerDoc()->ScheduleSVGForPresAttrEvaluation(this);
}

NS_IMETHODIMP_(bool)
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement::IsAttributeMapped(const nsAtom* name) const {
||||||| merged common ancestors
nsSVGElement::IsAttributeMapped(const nsAtom* name) const
{
=======
SVGElement::IsAttributeMapped(const nsAtom* name) const {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  if (name == nsGkAtoms::lang) {
    return true;
  }
  return SVGElementBase::IsAttributeMapped(name);
}

// PresentationAttributes-FillStroke
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
/* static */ const Element::MappedAttributeEntry
    nsSVGElement::sFillStrokeMap[] = {{nsGkAtoms::fill},
                                      {nsGkAtoms::fill_opacity},
                                      {nsGkAtoms::fill_rule},
                                      {nsGkAtoms::paint_order},
                                      {nsGkAtoms::stroke},
                                      {nsGkAtoms::stroke_dasharray},
                                      {nsGkAtoms::stroke_dashoffset},
                                      {nsGkAtoms::stroke_linecap},
                                      {nsGkAtoms::stroke_linejoin},
                                      {nsGkAtoms::stroke_miterlimit},
                                      {nsGkAtoms::stroke_opacity},
                                      {nsGkAtoms::stroke_width},
                                      {nsGkAtoms::vector_effect},
                                      {nullptr}};
||||||| merged common ancestors
/* static */ const Element::MappedAttributeEntry
nsSVGElement::sFillStrokeMap[] = {
  { nsGkAtoms::fill },
  { nsGkAtoms::fill_opacity },
  { nsGkAtoms::fill_rule },
  { nsGkAtoms::paint_order },
  { nsGkAtoms::stroke },
  { nsGkAtoms::stroke_dasharray },
  { nsGkAtoms::stroke_dashoffset },
  { nsGkAtoms::stroke_linecap },
  { nsGkAtoms::stroke_linejoin },
  { nsGkAtoms::stroke_miterlimit },
  { nsGkAtoms::stroke_opacity },
  { nsGkAtoms::stroke_width },
  { nsGkAtoms::vector_effect },
  { nullptr }
};
=======
/* static */
const Element::MappedAttributeEntry SVGElement::sFillStrokeMap[] = {
    {nsGkAtoms::fill},
    {nsGkAtoms::fill_opacity},
    {nsGkAtoms::fill_rule},
    {nsGkAtoms::paint_order},
    {nsGkAtoms::stroke},
    {nsGkAtoms::stroke_dasharray},
    {nsGkAtoms::stroke_dashoffset},
    {nsGkAtoms::stroke_linecap},
    {nsGkAtoms::stroke_linejoin},
    {nsGkAtoms::stroke_miterlimit},
    {nsGkAtoms::stroke_opacity},
    {nsGkAtoms::stroke_width},
    {nsGkAtoms::vector_effect},
    {nullptr}};
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

// PresentationAttributes-Graphics
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
/* static */ const Element::MappedAttributeEntry nsSVGElement::sGraphicsMap[] =
    {{nsGkAtoms::clip_path},
     {nsGkAtoms::clip_rule},
     {nsGkAtoms::colorInterpolation},
     {nsGkAtoms::cursor},
     {nsGkAtoms::display},
     {nsGkAtoms::filter},
     {nsGkAtoms::image_rendering},
     {nsGkAtoms::mask},
     {nsGkAtoms::opacity},
     {nsGkAtoms::pointer_events},
     {nsGkAtoms::shape_rendering},
     {nsGkAtoms::text_rendering},
     {nsGkAtoms::visibility},
     {nullptr}};
||||||| merged common ancestors
/* static */ const Element::MappedAttributeEntry
nsSVGElement::sGraphicsMap[] = {
  { nsGkAtoms::clip_path },
  { nsGkAtoms::clip_rule },
  { nsGkAtoms::colorInterpolation },
  { nsGkAtoms::cursor },
  { nsGkAtoms::display },
  { nsGkAtoms::filter },
  { nsGkAtoms::image_rendering },
  { nsGkAtoms::mask },
  { nsGkAtoms::opacity },
  { nsGkAtoms::pointer_events },
  { nsGkAtoms::shape_rendering },
  { nsGkAtoms::text_rendering },
  { nsGkAtoms::visibility },
  { nullptr }
};
=======
/* static */
const Element::MappedAttributeEntry SVGElement::sGraphicsMap[] = {
    {nsGkAtoms::clip_path},
    {nsGkAtoms::clip_rule},
    {nsGkAtoms::colorInterpolation},
    {nsGkAtoms::cursor},
    {nsGkAtoms::display},
    {nsGkAtoms::filter},
    {nsGkAtoms::image_rendering},
    {nsGkAtoms::mask},
    {nsGkAtoms::opacity},
    {nsGkAtoms::pointer_events},
    {nsGkAtoms::shape_rendering},
    {nsGkAtoms::text_rendering},
    {nsGkAtoms::visibility},
    {nullptr}};
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

// PresentationAttributes-TextContentElements
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
/* static */ const Element::MappedAttributeEntry
    nsSVGElement::sTextContentElementsMap[] = {
        // Properties that we don't support are commented out.
        // { nsGkAtoms::alignment_baseline },
        // { nsGkAtoms::baseline_shift },
        {nsGkAtoms::direction},
        {nsGkAtoms::dominant_baseline},
        {nsGkAtoms::letter_spacing},
        {nsGkAtoms::text_anchor},
        {nsGkAtoms::text_decoration},
        {nsGkAtoms::unicode_bidi},
        {nsGkAtoms::word_spacing},
        {nsGkAtoms::writing_mode},
        {nullptr}};
||||||| merged common ancestors
/* static */ const Element::MappedAttributeEntry
nsSVGElement::sTextContentElementsMap[] = {
  // Properties that we don't support are commented out.
  // { nsGkAtoms::alignment_baseline },
  // { nsGkAtoms::baseline_shift },
  { nsGkAtoms::direction },
  { nsGkAtoms::dominant_baseline },
  { nsGkAtoms::letter_spacing },
  { nsGkAtoms::text_anchor },
  { nsGkAtoms::text_decoration },
  { nsGkAtoms::unicode_bidi },
  { nsGkAtoms::word_spacing },
  { nsGkAtoms::writing_mode },
  { nullptr }
};
=======
/* static */
const Element::MappedAttributeEntry SVGElement::sTextContentElementsMap[] = {
    // Properties that we don't support are commented out.
    // { nsGkAtoms::alignment_baseline },
    // { nsGkAtoms::baseline_shift },
    {nsGkAtoms::direction},
    {nsGkAtoms::dominant_baseline},
    {nsGkAtoms::letter_spacing},
    {nsGkAtoms::text_anchor},
    {nsGkAtoms::text_decoration},
    {nsGkAtoms::unicode_bidi},
    {nsGkAtoms::word_spacing},
    {nsGkAtoms::writing_mode},
    {nullptr}};
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

// PresentationAttributes-FontSpecification
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
/* static */ const Element::MappedAttributeEntry
    nsSVGElement::sFontSpecificationMap[] = {
        {nsGkAtoms::font_family},      {nsGkAtoms::font_size},
        {nsGkAtoms::font_size_adjust}, {nsGkAtoms::font_stretch},
        {nsGkAtoms::font_style},       {nsGkAtoms::font_variant},
        {nsGkAtoms::fontWeight},       {nullptr}};
||||||| merged common ancestors
/* static */ const Element::MappedAttributeEntry
nsSVGElement::sFontSpecificationMap[] = {
  { nsGkAtoms::font_family },
  { nsGkAtoms::font_size },
  { nsGkAtoms::font_size_adjust },
  { nsGkAtoms::font_stretch },
  { nsGkAtoms::font_style },
  { nsGkAtoms::font_variant },
  { nsGkAtoms::fontWeight },
  { nullptr }
};
=======
/* static */
const Element::MappedAttributeEntry SVGElement::sFontSpecificationMap[] = {
    {nsGkAtoms::font_family},      {nsGkAtoms::font_size},
    {nsGkAtoms::font_size_adjust}, {nsGkAtoms::font_stretch},
    {nsGkAtoms::font_style},       {nsGkAtoms::font_variant},
    {nsGkAtoms::fontWeight},       {nullptr}};
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

// PresentationAttributes-GradientStop
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
/* static */ const Element::MappedAttributeEntry
    nsSVGElement::sGradientStopMap[] = {
        {nsGkAtoms::stop_color}, {nsGkAtoms::stop_opacity}, {nullptr}};
||||||| merged common ancestors
/* static */ const Element::MappedAttributeEntry
nsSVGElement::sGradientStopMap[] = {
  { nsGkAtoms::stop_color },
  { nsGkAtoms::stop_opacity },
  { nullptr }
};
=======
/* static */
const Element::MappedAttributeEntry SVGElement::sGradientStopMap[] = {
    {nsGkAtoms::stop_color}, {nsGkAtoms::stop_opacity}, {nullptr}};
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

// PresentationAttributes-Viewports
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
/* static */ const Element::MappedAttributeEntry nsSVGElement::sViewportsMap[] =
    {{nsGkAtoms::overflow}, {nsGkAtoms::clip}, {nullptr}};
||||||| merged common ancestors
/* static */ const Element::MappedAttributeEntry
nsSVGElement::sViewportsMap[] = {
  { nsGkAtoms::overflow },
  { nsGkAtoms::clip },
  { nullptr }
};
=======
/* static */
const Element::MappedAttributeEntry SVGElement::sViewportsMap[] = {
    {nsGkAtoms::overflow}, {nsGkAtoms::clip}, {nullptr}};
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

// PresentationAttributes-Makers
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
/* static */ const Element::MappedAttributeEntry nsSVGElement::sMarkersMap[] = {
    {nsGkAtoms::marker_end},
    {nsGkAtoms::marker_mid},
    {nsGkAtoms::marker_start},
    {nullptr}};
||||||| merged common ancestors
/* static */ const Element::MappedAttributeEntry
nsSVGElement::sMarkersMap[] = {
  { nsGkAtoms::marker_end },
  { nsGkAtoms::marker_mid },
  { nsGkAtoms::marker_start },
  { nullptr }
};
=======
/* static */
const Element::MappedAttributeEntry SVGElement::sMarkersMap[] = {
    {nsGkAtoms::marker_end},
    {nsGkAtoms::marker_mid},
    {nsGkAtoms::marker_start},
    {nullptr}};
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

// PresentationAttributes-Color
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
/* static */ const Element::MappedAttributeEntry nsSVGElement::sColorMap[] = {
    {nsGkAtoms::color}, {nullptr}};
||||||| merged common ancestors
/* static */ const Element::MappedAttributeEntry
nsSVGElement::sColorMap[] = {
  { nsGkAtoms::color },
  { nullptr }
};
=======
/* static */
const Element::MappedAttributeEntry SVGElement::sColorMap[] = {
    {nsGkAtoms::color}, {nullptr}};
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

// PresentationAttributes-Filters
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
/* static */ const Element::MappedAttributeEntry nsSVGElement::sFiltersMap[] = {
    {nsGkAtoms::colorInterpolationFilters}, {nullptr}};
||||||| merged common ancestors
/* static */ const Element::MappedAttributeEntry
nsSVGElement::sFiltersMap[] = {
  { nsGkAtoms::colorInterpolationFilters },
  { nullptr }
};
=======
/* static */
const Element::MappedAttributeEntry SVGElement::sFiltersMap[] = {
    {nsGkAtoms::colorInterpolationFilters}, {nullptr}};
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

// PresentationAttributes-feFlood
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
/* static */ const Element::MappedAttributeEntry nsSVGElement::sFEFloodMap[] = {
    {nsGkAtoms::flood_color}, {nsGkAtoms::flood_opacity}, {nullptr}};
||||||| merged common ancestors
/* static */ const Element::MappedAttributeEntry
nsSVGElement::sFEFloodMap[] = {
  { nsGkAtoms::flood_color },
  { nsGkAtoms::flood_opacity },
  { nullptr }
};
=======
/* static */
const Element::MappedAttributeEntry SVGElement::sFEFloodMap[] = {
    {nsGkAtoms::flood_color}, {nsGkAtoms::flood_opacity}, {nullptr}};
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

// PresentationAttributes-LightingEffects
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
/* static */ const Element::MappedAttributeEntry
    nsSVGElement::sLightingEffectsMap[] = {{nsGkAtoms::lighting_color},
                                           {nullptr}};
||||||| merged common ancestors
/* static */ const Element::MappedAttributeEntry
nsSVGElement::sLightingEffectsMap[] = {
  { nsGkAtoms::lighting_color },
  { nullptr }
};
=======
/* static */
const Element::MappedAttributeEntry SVGElement::sLightingEffectsMap[] = {
    {nsGkAtoms::lighting_color}, {nullptr}};
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

// PresentationAttributes-mask
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
/* static */ const Element::MappedAttributeEntry nsSVGElement::sMaskMap[] = {
    {nsGkAtoms::mask_type}, {nullptr}};
||||||| merged common ancestors
/* static */ const Element::MappedAttributeEntry
nsSVGElement::sMaskMap[] = {
  { nsGkAtoms::mask_type },
  { nullptr }
};
=======
/* static */
const Element::MappedAttributeEntry SVGElement::sMaskMap[] = {
    {nsGkAtoms::mask_type}, {nullptr}};
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

//----------------------------------------------------------------------
// Element methods

// forwarded to Element implementations

//----------------------------------------------------------------------

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
SVGSVGElement* nsSVGElement::GetOwnerSVGElement() {
||||||| merged common ancestors
SVGSVGElement*
nsSVGElement::GetOwnerSVGElement()
{
=======
SVGSVGElement* SVGElement::GetOwnerSVGElement() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsIContent* ancestor = GetFlattenedTreeParent();

  while (ancestor && ancestor->IsSVGElement()) {
    if (ancestor->IsSVGElement(nsGkAtoms::foreignObject)) {
      return nullptr;
    }
    if (ancestor->IsSVGElement(nsGkAtoms::svg)) {
      return static_cast<SVGSVGElement*>(ancestor);
    }
    ancestor = ancestor->GetFlattenedTreeParent();
  }

  // we don't have an ancestor <svg> element...
  return nullptr;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement* nsSVGElement::GetViewportElement() {
||||||| merged common ancestors
nsSVGElement*
nsSVGElement::GetViewportElement()
{
=======
SVGElement* SVGElement::GetViewportElement() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return SVGContentUtils::GetNearestViewportElement(this);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
already_AddRefed<SVGAnimatedString> nsSVGElement::ClassName() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
nsSVGElement::ClassName()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGElement::ClassName() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return mClassAttribute.ToDOMAnimatedString(this);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
bool nsSVGElement::IsSVGFocusable(bool* aIsFocusable, int32_t* aTabIndex) {
  nsIDocument* doc = GetComposedDoc();
  if (!doc || doc->HasFlag(NODE_IS_EDITABLE)) {
    // In designMode documents we only allow focusing the document.
    if (aTabIndex) {
      *aTabIndex = -1;
    }

    *aIsFocusable = false;

    return true;
||||||| merged common ancestors
bool
nsSVGElement::IsSVGFocusable(bool* aIsFocusable, int32_t* aTabIndex)
{
  nsIDocument* doc = GetComposedDoc();
  if (!doc || doc->HasFlag(NODE_IS_EDITABLE)) {
    // In designMode documents we only allow focusing the document.
    if (aTabIndex) {
      *aTabIndex = -1;
    }

    *aIsFocusable = false;

    return true;
=======
/* static */
bool SVGElement::UpdateDeclarationBlockFromLength(
    DeclarationBlock& aBlock, nsCSSPropertyID aPropId,
    const SVGAnimatedLength& aLength, ValToUse aValToUse) {
  aBlock.AssertMutable();

  float value;
  if (aValToUse == ValToUse::Anim) {
    value = aLength.GetAnimValInSpecifiedUnits();
  } else {
    MOZ_ASSERT(aValToUse == ValToUse::Base);
    value = aLength.GetBaseValInSpecifiedUnits();
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  }

  // SVG parser doesn't check non-negativity of some parsed value,
  // we should not pass those to CSS side.
  if (value < 0 &&
      SVGGeometryProperty::IsNonNegativeGeometryProperty(aPropId)) {
    return false;
  }

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
  // If a tabindex is specified at all, or the default tabindex is 0, we're
  // focusable
  *aIsFocusable = tabIndex >= 0 || HasAttr(nsGkAtoms::tabindex);
||||||| merged common ancestors
  // If a tabindex is specified at all, or the default tabindex is 0, we're focusable
  *aIsFocusable = tabIndex >= 0 || HasAttr(nsGkAtoms::tabindex);
=======
  nsCSSUnit cssUnit = SVGGeometryProperty::SpecifiedUnitTypeToCSSUnit(
      aLength.GetSpecifiedUnitType());
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

  if (cssUnit == eCSSUnit_Percent) {
    Servo_DeclarationBlock_SetPercentValue(aBlock.Raw(), aPropId,
                                           value / 100.f);
  } else {
    Servo_DeclarationBlock_SetLengthValue(aBlock.Raw(), aPropId, value,
                                          cssUnit);
  }

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
bool nsSVGElement::IsFocusableInternal(int32_t* aTabIndex, bool aWithMouse) {
  bool isFocusable = false;
  IsSVGFocusable(&isFocusable, aTabIndex);
  return isFocusable;
||||||| merged common ancestors
bool
nsSVGElement::IsFocusableInternal(int32_t* aTabIndex, bool aWithMouse)
{
  bool isFocusable = false;
  IsSVGFocusable(&isFocusable, aTabIndex);
  return isFocusable;
=======
  return true;
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
}

//------------------------------------------------------------------------
// Helper class: MappedAttrParser, for parsing values of mapped attributes

namespace {

class MOZ_STACK_CLASS MappedAttrParser {
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
 public:
  MappedAttrParser(css::Loader* aLoader, nsIURI* aDocURI,
                   already_AddRefed<nsIURI> aBaseURI, nsSVGElement* aElement);
||||||| merged common ancestors
public:
  MappedAttrParser(css::Loader* aLoader,
                   nsIURI* aDocURI,
                   already_AddRefed<nsIURI> aBaseURI,
                   nsSVGElement* aElement);
=======
 public:
  MappedAttrParser(css::Loader* aLoader, nsIURI* aDocURI,
                   already_AddRefed<nsIURI> aBaseURI, SVGElement* aElement);
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  ~MappedAttrParser();

  // Parses a mapped attribute value.
  void ParseMappedAttrValue(nsAtom* aMappedAttrName,
                            const nsAString& aMappedAttrValue);

  void TellStyleAlreadyParsedResult(nsAtom const* aAtom,
                                    SVGAnimatedLength const& aLength);

  // If we've parsed any values for mapped attributes, this method returns the
  // already_AddRefed css::Declaration that incorporates the parsed
  // values. Otherwise, this method returns null.
  already_AddRefed<DeclarationBlock> GetDeclarationBlock();

 private:
  // MEMBER DATA
  // -----------
  css::Loader* mLoader;

  // Arguments for nsCSSParser::ParseProperty
  nsIURI* mDocURI;
  nsCOMPtr<nsIURI> mBaseURI;

  // Declaration for storing parsed values (lazily initialized)
  RefPtr<DeclarationBlock> mDecl;

  // For reporting use counters
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
  nsSVGElement* mElement;
||||||| merged common ancestors
  nsSVGElement*     mElement;
=======
  SVGElement* mElement;
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
};

MappedAttrParser::MappedAttrParser(css::Loader* aLoader, nsIURI* aDocURI,
                                   already_AddRefed<nsIURI> aBaseURI,
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
                                   nsSVGElement* aElement)
    : mLoader(aLoader),
      mDocURI(aDocURI),
      mBaseURI(aBaseURI),
      mElement(aElement) {}
||||||| merged common ancestors
                                   nsSVGElement* aElement)
  : mLoader(aLoader)
  , mDocURI(aDocURI)
  , mBaseURI(aBaseURI)
  , mElement(aElement)
{
}
=======
                                   SVGElement* aElement)
    : mLoader(aLoader),
      mDocURI(aDocURI),
      mBaseURI(aBaseURI),
      mElement(aElement) {}
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

MappedAttrParser::~MappedAttrParser() {
  MOZ_ASSERT(!mDecl,
             "If mDecl was initialized, it should have been returned via "
             "GetDeclarationBlock (and had its pointer cleared)");
}

void MappedAttrParser::ParseMappedAttrValue(nsAtom* aMappedAttrName,
                                            const nsAString& aMappedAttrValue) {
  if (!mDecl) {
    mDecl = new DeclarationBlock();
  }

  // Get the nsCSSPropertyID ID for our mapped attribute.
  nsCSSPropertyID propertyID =
      nsCSSProps::LookupProperty(nsDependentAtomString(aMappedAttrName));
  if (propertyID != eCSSProperty_UNKNOWN) {
    bool changed = false;  // outparam for ParseProperty.
    NS_ConvertUTF16toUTF8 value(aMappedAttrValue);
    // FIXME (bug 1343964): Figure out a better solution for sending the base
    // uri to servo
    RefPtr<URLExtraData> data =
        new URLExtraData(mBaseURI, mDocURI, mElement->NodePrincipal(),
                         mElement->OwnerDoc()->GetReferrerPolicy());
    changed = Servo_DeclarationBlock_SetPropertyById(
        mDecl->Raw(), propertyID, &value, false, data,
        ParsingMode::AllowUnitlessLength,
        mElement->OwnerDoc()->GetCompatibilityMode(), mLoader, {});

    if (changed) {
      // The normal reporting of use counters by the nsCSSParser won't happen
      // since it doesn't have a sheet.
      if (nsCSSProps::IsShorthand(propertyID)) {
        CSSPROPS_FOR_SHORTHAND_SUBPROPERTIES(subprop, propertyID,
                                             CSSEnabledState::ForAllContent) {
          UseCounter useCounter = nsCSSProps::UseCounterFor(*subprop);
          if (useCounter != eUseCounter_UNKNOWN) {
            mElement->OwnerDoc()->SetDocumentAndPageUseCounter(useCounter);
          }
        }
      } else {
        UseCounter useCounter = nsCSSProps::UseCounterFor(propertyID);
        if (useCounter != eUseCounter_UNKNOWN) {
          mElement->OwnerDoc()->SetDocumentAndPageUseCounter(useCounter);
        }
      }
    }
    return;
  }
  MOZ_ASSERT(aMappedAttrName == nsGkAtoms::lang,
             "Only 'lang' should be unrecognized!");
  // nsCSSParser doesn't know about 'lang', so we need to handle it specially.
  if (aMappedAttrName == nsGkAtoms::lang) {
    propertyID = eCSSProperty__x_lang;
    RefPtr<nsAtom> atom = NS_Atomize(aMappedAttrValue);
    Servo_DeclarationBlock_SetIdentStringValue(mDecl->Raw(), propertyID, atom);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
already_AddRefed<DeclarationBlock> MappedAttrParser::GetDeclarationBlock() {
||||||| merged common ancestors
already_AddRefed<DeclarationBlock>
MappedAttrParser::GetDeclarationBlock()
{
=======
void MappedAttrParser::TellStyleAlreadyParsedResult(
    nsAtom const* aAtom, SVGAnimatedLength const& aLength) {
  if (!mDecl) {
    mDecl = new DeclarationBlock();
  }
  nsCSSPropertyID propertyID =
      nsCSSProps::LookupProperty(nsDependentAtomString(aAtom));

  SVGElement::UpdateDeclarationBlockFromLength(*mDecl, propertyID, aLength,
                                               SVGElement::ValToUse::Base);
}

already_AddRefed<DeclarationBlock> MappedAttrParser::GetDeclarationBlock() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return mDecl.forget();
}

}  // namespace

//----------------------------------------------------------------------
// Implementation Helpers:

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::UpdateContentDeclarationBlock() {
||||||| merged common ancestors
void
nsSVGElement::UpdateContentDeclarationBlock()
{
=======
void SVGElement::UpdateContentDeclarationBlock() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  NS_ASSERTION(!mContentDeclarationBlock,
               "we already have a content declaration block");

  uint32_t attrCount = mAttrs.AttrCount();
  if (!attrCount) {
    // nothing to do
    return;
  }

  Document* doc = OwnerDoc();
  MappedAttrParser mappedAttrParser(doc->CSSLoader(), doc->GetDocumentURI(),
                                    GetBaseURI(), this);

  bool lengthAffectsStyle =
      SVGGeometryProperty::ElementMapsLengthsToStyle(this);

  for (uint32_t i = 0; i < attrCount; ++i) {
    const nsAttrName* attrName = mAttrs.AttrNameAt(i);
    if (!attrName->IsAtom() || !IsAttributeMapped(attrName->Atom())) continue;

    if (attrName->NamespaceID() != kNameSpaceID_None &&
        !attrName->Equals(nsGkAtoms::lang, kNameSpaceID_XML)) {
      continue;
    }

    if (attrName->Equals(nsGkAtoms::lang, kNameSpaceID_None) &&
        HasAttr(kNameSpaceID_XML, nsGkAtoms::lang)) {
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
      continue;  // xml:lang has precedence
    }

    if (IsSVGElement(nsGkAtoms::svg)) {
      // Special case: we don't want <svg> 'width'/'height' mapped into style
      // if the attribute value isn't a valid <length> according to SVG (which
      // only supports a subset of the CSS <length> values). We don't enforce
      // this by checking the attribute value in SVGSVGElement::
      // IsAttributeMapped since we don't want that method to depend on the
      // value of the attribute that is being checked. Rather we just prevent
      // the actual mapping here, as necessary.
      if (attrName->Atom() == nsGkAtoms::width &&
          !GetAnimatedLength(nsGkAtoms::width)->HasBaseVal()) {
        continue;
      }
      if (attrName->Atom() == nsGkAtoms::height &&
          !GetAnimatedLength(nsGkAtoms::height)->HasBaseVal()) {
||||||| merged common ancestors
      continue; // xml:lang has precedence
    }

    if (IsSVGElement(nsGkAtoms::svg)) {
      // Special case: we don't want <svg> 'width'/'height' mapped into style
      // if the attribute value isn't a valid <length> according to SVG (which
      // only supports a subset of the CSS <length> values). We don't enforce
      // this by checking the attribute value in SVGSVGElement::
      // IsAttributeMapped since we don't want that method to depend on the
      // value of the attribute that is being checked. Rather we just prevent
      // the actual mapping here, as necessary.
      if (attrName->Atom() == nsGkAtoms::width &&
          !GetAnimatedLength(nsGkAtoms::width)->HasBaseVal()) {
        continue;
      }
      if (attrName->Atom() == nsGkAtoms::height &&
          !GetAnimatedLength(nsGkAtoms::height)->HasBaseVal()) {
=======
      continue;  // xml:lang has precedence
    }

    if (lengthAffectsStyle) {
      auto const* length = GetAnimatedLength(attrName->Atom());

      if (length && length->HasBaseVal()) {
        // This is an element with geometry property set via SVG attribute,
        // and the attribute is already successfully parsed. We want to go
        // through the optimized path to tell the style system the result
        // directly, rather than let it parse the same thing again.
        mappedAttrParser.TellStyleAlreadyParsedResult(attrName->Atom(),
                                                      *length);
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
        continue;
      }
    }

    nsAutoString value;
    mAttrs.AttrAt(i)->ToString(value);
    mappedAttrParser.ParseMappedAttrValue(attrName->Atom(), value);
  }
  mContentDeclarationBlock = mappedAttrParser.GetDeclarationBlock();
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
const DeclarationBlock* nsSVGElement::GetContentDeclarationBlock() const {
||||||| merged common ancestors
const DeclarationBlock*
nsSVGElement::GetContentDeclarationBlock() const
{
=======
const DeclarationBlock* SVGElement::GetContentDeclarationBlock() const {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return mContentDeclarationBlock;
}

/**
 * Helper methods for the type-specific WillChangeXXX methods.
 *
 * This method sends out appropriate pre-change notifications so that selector
 * restyles (e.g. due to changes that cause |elem[attr="val"]| to start/stop
 * matching) work, and it returns an nsAttrValue that _may_ contain the
 * attribute's pre-change value.
 *
 * The nsAttrValue returned by this method depends on whether there are
 * mutation event listeners listening for changes to this element's attributes.
 * If not, then the object returned is empty. If there are, then the
 * nsAttrValue returned contains a serialized copy of the attribute's value
 * prior to the change, and this object should be passed to the corresponding
 * DidChangeXXX method call (assuming a WillChangeXXX call is required for the
 * SVG type - see comment below). This is necessary so that the 'prevValue'
 * property of the mutation event that is dispatched will correctly contain the
 * old value.
 *
 * The reason we need to serialize the old value if there are mutation
 * event listeners is because the underlying nsAttrValue for the attribute
 * points directly to a parsed representation of the attribute (e.g. an
 * SVGAnimatedLengthList*) that is a member of the SVG element. That object
 * will have changed by the time DidChangeXXX has been called, so without the
 * serialization of the old attribute value that we provide, DidChangeXXX
 * would have no way to get the old value to pass to SetAttrAndNotify.
 *
 * We only return the old value when there are mutation event listeners because
 * it's not needed otherwise, and because it's expensive to serialize the old
 * value. This is especially true for list type attributes, which may be built
 * up via the SVG DOM resulting in a large number of Will/DidModifyXXX calls
 * before the script finally finishes setting the attribute.
 *
 * Note that unlike using SetParsedAttr, using Will/DidChangeXXX does NOT check
 * and filter out redundant changes. Before calling WillChangeXXX, the caller
 * should check whether the new and old values are actually the same, and skip
 * calling Will/DidChangeXXX if they are.
 *
 * Also note that not all SVG types use this scheme. For types that can be
 * represented by an nsAttrValue without pointing back to an SVG object (e.g.
 * enums, booleans, integers) we can simply use SetParsedAttr which will do all
 * of the above for us. For such types there is no matching WillChangeXXX
 * method, only DidChangeXXX which calls SetParsedAttr.
 */
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsAttrValue nsSVGElement::WillChangeValue(nsAtom* aName) {
||||||| merged common ancestors
nsAttrValue
nsSVGElement::WillChangeValue(nsAtom* aName)
{
=======
nsAttrValue SVGElement::WillChangeValue(nsAtom* aName) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  // We need an empty attr value:
  //   a) to pass to BeforeSetAttr when GetParsedAttr returns nullptr
  //   b) to store the old value in the case we have mutation listeners
  //
  // We can use the same value for both purposes, because if GetParsedAttr
  // returns non-null its return value is what will get passed to BeforeSetAttr,
  // not matter what our mutation listener situation is.
  //
  // Also, we should be careful to always return this value to benefit from
  // return value optimization.
  nsAttrValue emptyOrOldAttrValue;
  const nsAttrValue* attrValue = GetParsedAttr(aName);

  // We only need to set the old value if we have listeners since otherwise it
  // isn't used.
  if (attrValue && nsContentUtils::HasMutationListeners(
                       this, NS_EVENT_BITS_MUTATION_ATTRMODIFIED, this)) {
    emptyOrOldAttrValue.SetToSerialized(*attrValue);
  }

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
  uint8_t modType =
      attrValue ? static_cast<uint8_t>(MutationEvent_Binding::MODIFICATION)
                : static_cast<uint8_t>(MutationEvent_Binding::ADDITION);
  nsNodeUtils::AttributeWillChange(this, kNameSpaceID_None, aName, modType,
                                   nullptr);
||||||| merged common ancestors
  uint8_t modType = attrValue
                  ? static_cast<uint8_t>(MutationEvent_Binding::MODIFICATION)
                  : static_cast<uint8_t>(MutationEvent_Binding::ADDITION);
  nsNodeUtils::AttributeWillChange(this, kNameSpaceID_None, aName, modType,
                                   nullptr);
=======
  uint8_t modType =
      attrValue ? static_cast<uint8_t>(MutationEvent_Binding::MODIFICATION)
                : static_cast<uint8_t>(MutationEvent_Binding::ADDITION);
  nsNodeUtils::AttributeWillChange(this, kNameSpaceID_None, aName, modType);
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

  // This is not strictly correct--the attribute value parameter for
  // BeforeSetAttr should reflect the value that *will* be set but that implies
  // allocating, e.g. an extra SVGAnimatedLength, and isn't necessary at the
  // moment since no SVG elements overload BeforeSetAttr. For now we just pass
  // the current value.
  nsAttrValueOrString attrStringOrValue(attrValue ? *attrValue
                                                  : emptyOrOldAttrValue);
  DebugOnly<nsresult> rv = BeforeSetAttr(
      kNameSpaceID_None, aName, &attrStringOrValue, kNotifyDocumentObservers);
  // SVG elements aren't expected to overload BeforeSetAttr in such a way that
  // it may fail. So long as this is the case we don't need to check and pass on
  // the return value which simplifies the calling code significantly.
  MOZ_ASSERT(NS_SUCCEEDED(rv), "Unexpected failure from BeforeSetAttr");

  return emptyOrOldAttrValue;
}

/**
 * Helper methods for the type-specific DidChangeXXX methods.
 *
 * aEmptyOrOldValue will normally be the object returned from the corresponding
 * WillChangeXXX call. This is because:
 * a) WillChangeXXX will ensure the object is set when we have mutation
 *    listeners, and
 * b) WillChangeXXX will ensure the object represents a serialized version of
 *    the old attribute value so that the value doesn't change when the
 *    underlying SVG type is updated.
 *
 * aNewValue is replaced with the old value.
 */
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangeValue(nsAtom* aName,
                                  const nsAttrValue& aEmptyOrOldValue,
                                  nsAttrValue& aNewValue) {
  bool hasListeners = nsContentUtils::HasMutationListeners(
      this, NS_EVENT_BITS_MUTATION_ATTRMODIFIED, this);
  uint8_t modType =
      HasAttr(kNameSpaceID_None, aName)
          ? static_cast<uint8_t>(MutationEvent_Binding::MODIFICATION)
          : static_cast<uint8_t>(MutationEvent_Binding::ADDITION);

  nsIDocument* document = GetComposedDoc();
||||||| merged common ancestors
void
nsSVGElement::DidChangeValue(nsAtom* aName,
                             const nsAttrValue& aEmptyOrOldValue,
                             nsAttrValue& aNewValue)
{
  bool hasListeners =
    nsContentUtils::HasMutationListeners(this,
                                         NS_EVENT_BITS_MUTATION_ATTRMODIFIED,
                                         this);
  uint8_t modType = HasAttr(kNameSpaceID_None, aName)
                  ? static_cast<uint8_t>(MutationEvent_Binding::MODIFICATION)
                  : static_cast<uint8_t>(MutationEvent_Binding::ADDITION);

  nsIDocument* document = GetComposedDoc();
=======
void SVGElement::DidChangeValue(nsAtom* aName,
                                const nsAttrValue& aEmptyOrOldValue,
                                nsAttrValue& aNewValue) {
  bool hasListeners = nsContentUtils::HasMutationListeners(
      this, NS_EVENT_BITS_MUTATION_ATTRMODIFIED, this);
  uint8_t modType =
      HasAttr(kNameSpaceID_None, aName)
          ? static_cast<uint8_t>(MutationEvent_Binding::MODIFICATION)
          : static_cast<uint8_t>(MutationEvent_Binding::ADDITION);

  Document* document = GetComposedDoc();
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  mozAutoDocUpdate updateBatch(document, kNotifyDocumentObservers);
  // XXX Really, the fourth argument to SetAttrAndNotify should be null if
  // aEmptyOrOldValue does not represent the actual previous value of the
  // attribute, but currently SVG elements do not even use the old attribute
  // value in |AfterSetAttr|, so this should be ok.
  SetAttrAndNotify(kNameSpaceID_None, aName, nullptr, &aEmptyOrOldValue,
                   aNewValue, nullptr, modType, hasListeners,
                   kNotifyDocumentObservers, kCallAfterSetAttr, document,
                   updateBatch);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::MaybeSerializeAttrBeforeRemoval(nsAtom* aName,
                                                   bool aNotify) {
  if (!aNotify || !nsContentUtils::HasMutationListeners(
                      this, NS_EVENT_BITS_MUTATION_ATTRMODIFIED, this)) {
||||||| merged common ancestors
void
nsSVGElement::MaybeSerializeAttrBeforeRemoval(nsAtom* aName, bool aNotify)
{
  if (!aNotify ||
      !nsContentUtils::HasMutationListeners(this,
                                            NS_EVENT_BITS_MUTATION_ATTRMODIFIED,
                                            this)) {
=======
void SVGElement::MaybeSerializeAttrBeforeRemoval(nsAtom* aName, bool aNotify) {
  if (!aNotify || !nsContentUtils::HasMutationListeners(
                      this, NS_EVENT_BITS_MUTATION_ATTRMODIFIED, this)) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
    return;
  }

  const nsAttrValue* attrValue = mAttrs.GetAttr(aName);
  if (!attrValue) return;

  nsAutoString serializedValue;
  attrValue->ToString(serializedValue);
  nsAttrValue oldAttrValue(serializedValue);
  bool oldValueSet;
  mAttrs.SetAndSwapAttr(aName, oldAttrValue, &oldValueSet);
}

/* static */
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsAtom* nsSVGElement::GetEventNameForAttr(nsAtom* aAttr) {
  if (aAttr == nsGkAtoms::onload) return nsGkAtoms::onSVGLoad;
  if (aAttr == nsGkAtoms::onunload) return nsGkAtoms::onSVGUnload;
  if (aAttr == nsGkAtoms::onresize) return nsGkAtoms::onSVGResize;
  if (aAttr == nsGkAtoms::onscroll) return nsGkAtoms::onSVGScroll;
  if (aAttr == nsGkAtoms::onzoom) return nsGkAtoms::onSVGZoom;
  if (aAttr == nsGkAtoms::onbegin) return nsGkAtoms::onbeginEvent;
  if (aAttr == nsGkAtoms::onrepeat) return nsGkAtoms::onrepeatEvent;
  if (aAttr == nsGkAtoms::onend) return nsGkAtoms::onendEvent;
||||||| merged common ancestors
nsAtom* nsSVGElement::GetEventNameForAttr(nsAtom* aAttr)
{
  if (aAttr == nsGkAtoms::onload)
    return nsGkAtoms::onSVGLoad;
  if (aAttr == nsGkAtoms::onunload)
    return nsGkAtoms::onSVGUnload;
  if (aAttr == nsGkAtoms::onresize)
    return nsGkAtoms::onSVGResize;
  if (aAttr == nsGkAtoms::onscroll)
    return nsGkAtoms::onSVGScroll;
  if (aAttr == nsGkAtoms::onzoom)
    return nsGkAtoms::onSVGZoom;
  if (aAttr == nsGkAtoms::onbegin)
    return nsGkAtoms::onbeginEvent;
  if (aAttr == nsGkAtoms::onrepeat)
    return nsGkAtoms::onrepeatEvent;
  if (aAttr == nsGkAtoms::onend)
    return nsGkAtoms::onendEvent;
=======
nsAtom* SVGElement::GetEventNameForAttr(nsAtom* aAttr) {
  if (aAttr == nsGkAtoms::onload) return nsGkAtoms::onSVGLoad;
  if (aAttr == nsGkAtoms::onunload) return nsGkAtoms::onSVGUnload;
  if (aAttr == nsGkAtoms::onresize) return nsGkAtoms::onSVGResize;
  if (aAttr == nsGkAtoms::onscroll) return nsGkAtoms::onSVGScroll;
  if (aAttr == nsGkAtoms::onzoom) return nsGkAtoms::onSVGZoom;
  if (aAttr == nsGkAtoms::onbegin) return nsGkAtoms::onbeginEvent;
  if (aAttr == nsGkAtoms::onrepeat) return nsGkAtoms::onrepeatEvent;
  if (aAttr == nsGkAtoms::onend) return nsGkAtoms::onendEvent;
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

  return aAttr;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
SVGViewportElement* nsSVGElement::GetCtx() const {
||||||| merged common ancestors
SVGViewportElement *
nsSVGElement::GetCtx() const
{
=======
SVGViewportElement* SVGElement::GetCtx() const {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return SVGContentUtils::GetNearestViewportElement(this);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
/* virtual */ gfxMatrix nsSVGElement::PrependLocalTransformsTo(
    const gfxMatrix& aMatrix, SVGTransformTypes aWhich) const {
||||||| merged common ancestors
/* virtual */ gfxMatrix
nsSVGElement::PrependLocalTransformsTo(
  const gfxMatrix &aMatrix, SVGTransformTypes aWhich) const
{
=======
/* virtual */
gfxMatrix SVGElement::PrependLocalTransformsTo(const gfxMatrix& aMatrix,
                                               SVGTransformTypes aWhich) const {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return aMatrix;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement::LengthAttributesInfo nsSVGElement::GetLengthInfo() {
||||||| merged common ancestors
nsSVGElement::LengthAttributesInfo
nsSVGElement::GetLengthInfo()
{
=======
SVGElement::LengthAttributesInfo SVGElement::GetLengthInfo() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return LengthAttributesInfo(nullptr, nullptr, 0);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::LengthAttributesInfo::Reset(uint8_t aAttrEnum) {
  mLengths[aAttrEnum].Init(mLengthInfo[aAttrEnum].mCtxType, aAttrEnum,
||||||| merged common ancestors
void
nsSVGElement::LengthAttributesInfo::Reset(uint8_t aAttrEnum)
{
  mLengths[aAttrEnum].Init(mLengthInfo[aAttrEnum].mCtxType,
                           aAttrEnum,
=======
void SVGElement::LengthAttributesInfo::Reset(uint8_t aAttrEnum) {
  mLengths[aAttrEnum].Init(mLengthInfo[aAttrEnum].mCtxType, aAttrEnum,
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
                           mLengthInfo[aAttrEnum].mDefaultValue,
                           mLengthInfo[aAttrEnum].mDefaultUnitType);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::SetLength(nsAtom* aName, const nsSVGLength2& aLength) {
||||||| merged common ancestors
void
nsSVGElement::SetLength(nsAtom* aName, const nsSVGLength2 &aLength)
{
=======
void SVGElement::SetLength(nsAtom* aName, const SVGAnimatedLength& aLength) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  LengthAttributesInfo lengthInfo = GetLengthInfo();

  for (uint32_t i = 0; i < lengthInfo.mLengthCount; i++) {
    if (aName == lengthInfo.mLengthInfo[i].mName) {
      lengthInfo.mLengths[i] = aLength;
      DidAnimateLength(i);
      return;
    }
  }
  MOZ_ASSERT(false, "no length found to set");
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsAttrValue nsSVGElement::WillChangeLength(uint8_t aAttrEnum) {
||||||| merged common ancestors
nsAttrValue
nsSVGElement::WillChangeLength(uint8_t aAttrEnum)
{
=======
nsAttrValue SVGElement::WillChangeLength(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return WillChangeValue(GetLengthInfo().mLengthInfo[aAttrEnum].mName);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangeLength(uint8_t aAttrEnum,
                                   const nsAttrValue& aEmptyOrOldValue) {
||||||| merged common ancestors
void
nsSVGElement::DidChangeLength(uint8_t aAttrEnum,
                              const nsAttrValue& aEmptyOrOldValue)
{
=======
void SVGElement::DidChangeLength(uint8_t aAttrEnum,
                                 const nsAttrValue& aEmptyOrOldValue) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  LengthAttributesInfo info = GetLengthInfo();

  NS_ASSERTION(info.mLengthCount > 0,
               "DidChangeLength on element with no length attribs");
  NS_ASSERTION(aAttrEnum < info.mLengthCount, "aAttrEnum out of range");

  nsAttrValue newValue;
  newValue.SetTo(info.mLengths[aAttrEnum], nullptr);

  DidChangeValue(info.mLengthInfo[aAttrEnum].mName, aEmptyOrOldValue, newValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimateLength(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::DidAnimateLength(uint8_t aAttrEnum)
{
=======
void SVGElement::DidAnimateLength(uint8_t aAttrEnum) {
  if (SVGGeometryProperty::ElementMapsLengthsToStyle(this)) {
    nsCSSPropertyID propId =
        SVGGeometryProperty::AttrEnumToCSSPropId(this, aAttrEnum);

    SMILOverrideStyle()->SetSMILValue(propId,
                                      GetLengthInfo().mLengths[aAttrEnum]);
    return;
  }

>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  ClearAnyCachedPath();

  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    LengthAttributesInfo info = GetLengthInfo();
    frame->AttributeChanged(kNameSpaceID_None,
                            info.mLengthInfo[aAttrEnum].mName,
                            MutationEvent_Binding::SMIL);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGLength2* nsSVGElement::GetAnimatedLength(const nsAtom* aAttrName) {
||||||| merged common ancestors
nsSVGLength2*
nsSVGElement::GetAnimatedLength(const nsAtom *aAttrName)
{
=======
SVGAnimatedLength* SVGElement::GetAnimatedLength(const nsAtom* aAttrName) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  LengthAttributesInfo lengthInfo = GetLengthInfo();

  for (uint32_t i = 0; i < lengthInfo.mLengthCount; i++) {
    if (aAttrName == lengthInfo.mLengthInfo[i].mName) {
      return &lengthInfo.mLengths[i];
    }
  }
  return nullptr;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::GetAnimatedLengthValues(float* aFirst, ...) {
||||||| merged common ancestors
void
nsSVGElement::GetAnimatedLengthValues(float *aFirst, ...)
{
=======
void SVGElement::GetAnimatedLengthValues(float* aFirst, ...) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  LengthAttributesInfo info = GetLengthInfo();

  NS_ASSERTION(info.mLengthCount > 0,
               "GetAnimatedLengthValues on element with no length attribs");

  SVGViewportElement* ctx = nullptr;

  float* f = aFirst;
  uint32_t i = 0;

  va_list args;
  va_start(args, aFirst);

  while (f && i < info.mLengthCount) {
    uint8_t type = info.mLengths[i].GetSpecifiedUnitType();
    if (!ctx) {
      if (type != SVGLength_Binding::SVG_LENGTHTYPE_NUMBER &&
          type != SVGLength_Binding::SVG_LENGTHTYPE_PX)
        ctx = GetCtx();
    }
    if (type == SVGLength_Binding::SVG_LENGTHTYPE_EMS ||
        type == SVGLength_Binding::SVG_LENGTHTYPE_EXS)
      *f = info.mLengths[i++].GetAnimValue(this);
    else
      *f = info.mLengths[i++].GetAnimValue(ctx);
    f = va_arg(args, float*);
  }

  va_end(args);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement::LengthListAttributesInfo nsSVGElement::GetLengthListInfo() {
||||||| merged common ancestors
nsSVGElement::LengthListAttributesInfo
nsSVGElement::GetLengthListInfo()
{
=======
SVGElement::LengthListAttributesInfo SVGElement::GetLengthListInfo() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return LengthListAttributesInfo(nullptr, nullptr, 0);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::LengthListAttributesInfo::Reset(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::LengthListAttributesInfo::Reset(uint8_t aAttrEnum)
{
=======
void SVGElement::LengthListAttributesInfo::Reset(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  mLengthLists[aAttrEnum].ClearBaseValue(aAttrEnum);
  // caller notifies
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsAttrValue nsSVGElement::WillChangeLengthList(uint8_t aAttrEnum) {
||||||| merged common ancestors
nsAttrValue
nsSVGElement::WillChangeLengthList(uint8_t aAttrEnum)
{
=======
nsAttrValue SVGElement::WillChangeLengthList(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return WillChangeValue(GetLengthListInfo().mLengthListInfo[aAttrEnum].mName);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangeLengthList(uint8_t aAttrEnum,
                                       const nsAttrValue& aEmptyOrOldValue) {
||||||| merged common ancestors
void
nsSVGElement::DidChangeLengthList(uint8_t aAttrEnum,
                                  const nsAttrValue& aEmptyOrOldValue)
{
=======
void SVGElement::DidChangeLengthList(uint8_t aAttrEnum,
                                     const nsAttrValue& aEmptyOrOldValue) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  LengthListAttributesInfo info = GetLengthListInfo();

  NS_ASSERTION(info.mLengthListCount > 0,
               "DidChangeLengthList on element with no length list attribs");
  NS_ASSERTION(aAttrEnum < info.mLengthListCount, "aAttrEnum out of range");

  nsAttrValue newValue;
  newValue.SetTo(info.mLengthLists[aAttrEnum].GetBaseValue(), nullptr);

  DidChangeValue(info.mLengthListInfo[aAttrEnum].mName, aEmptyOrOldValue,
                 newValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimateLengthList(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::DidAnimateLengthList(uint8_t aAttrEnum)
{
=======
void SVGElement::DidAnimateLengthList(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    LengthListAttributesInfo info = GetLengthListInfo();
    frame->AttributeChanged(kNameSpaceID_None,
                            info.mLengthListInfo[aAttrEnum].mName,
                            MutationEvent_Binding::SMIL);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::GetAnimatedLengthListValues(SVGUserUnitList* aFirst, ...) {
||||||| merged common ancestors
void
nsSVGElement::GetAnimatedLengthListValues(SVGUserUnitList *aFirst, ...)
{
=======
void SVGElement::GetAnimatedLengthListValues(SVGUserUnitList* aFirst, ...) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  LengthListAttributesInfo info = GetLengthListInfo();

  NS_ASSERTION(
      info.mLengthListCount > 0,
      "GetAnimatedLengthListValues on element with no length list attribs");

  SVGUserUnitList* list = aFirst;
  uint32_t i = 0;

  va_list args;
  va_start(args, aFirst);

  while (list && i < info.mLengthListCount) {
    list->Init(&(info.mLengthLists[i].GetAnimValue()), this,
               info.mLengthListInfo[i].mAxis);
    ++i;
    list = va_arg(args, SVGUserUnitList*);
  }

  va_end(args);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
SVGAnimatedLengthList* nsSVGElement::GetAnimatedLengthList(uint8_t aAttrEnum) {
||||||| merged common ancestors
SVGAnimatedLengthList*
nsSVGElement::GetAnimatedLengthList(uint8_t aAttrEnum)
{
=======
SVGAnimatedLengthList* SVGElement::GetAnimatedLengthList(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  LengthListAttributesInfo info = GetLengthListInfo();
  if (aAttrEnum < info.mLengthListCount) {
    return &(info.mLengthLists[aAttrEnum]);
  }
  MOZ_ASSERT_UNREACHABLE("Bad attrEnum");
  return nullptr;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement::NumberListAttributesInfo nsSVGElement::GetNumberListInfo() {
||||||| merged common ancestors

nsSVGElement::NumberListAttributesInfo
nsSVGElement::GetNumberListInfo()
{
=======
SVGElement::NumberListAttributesInfo SVGElement::GetNumberListInfo() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return NumberListAttributesInfo(nullptr, nullptr, 0);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::NumberListAttributesInfo::Reset(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::NumberListAttributesInfo::Reset(uint8_t aAttrEnum)
{
=======
void SVGElement::NumberListAttributesInfo::Reset(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  MOZ_ASSERT(aAttrEnum < mNumberListCount, "Bad attr enum");
  mNumberLists[aAttrEnum].ClearBaseValue(aAttrEnum);
  // caller notifies
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsAttrValue nsSVGElement::WillChangeNumberList(uint8_t aAttrEnum) {
||||||| merged common ancestors
nsAttrValue
nsSVGElement::WillChangeNumberList(uint8_t aAttrEnum)
{
=======
nsAttrValue SVGElement::WillChangeNumberList(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return WillChangeValue(GetNumberListInfo().mNumberListInfo[aAttrEnum].mName);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangeNumberList(uint8_t aAttrEnum,
                                       const nsAttrValue& aEmptyOrOldValue) {
||||||| merged common ancestors
void
nsSVGElement::DidChangeNumberList(uint8_t aAttrEnum,
                                  const nsAttrValue& aEmptyOrOldValue)
{
=======
void SVGElement::DidChangeNumberList(uint8_t aAttrEnum,
                                     const nsAttrValue& aEmptyOrOldValue) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  NumberListAttributesInfo info = GetNumberListInfo();

  MOZ_ASSERT(info.mNumberListCount > 0,
             "DidChangeNumberList on element with no number list attribs");
  MOZ_ASSERT(aAttrEnum < info.mNumberListCount, "aAttrEnum out of range");

  nsAttrValue newValue;
  newValue.SetTo(info.mNumberLists[aAttrEnum].GetBaseValue(), nullptr);

  DidChangeValue(info.mNumberListInfo[aAttrEnum].mName, aEmptyOrOldValue,
                 newValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimateNumberList(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::DidAnimateNumberList(uint8_t aAttrEnum)
{
=======
void SVGElement::DidAnimateNumberList(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    NumberListAttributesInfo info = GetNumberListInfo();
    MOZ_ASSERT(aAttrEnum < info.mNumberListCount, "aAttrEnum out of range");

    frame->AttributeChanged(kNameSpaceID_None,
                            info.mNumberListInfo[aAttrEnum].mName,
                            MutationEvent_Binding::SMIL);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
SVGAnimatedNumberList* nsSVGElement::GetAnimatedNumberList(uint8_t aAttrEnum) {
||||||| merged common ancestors
SVGAnimatedNumberList*
nsSVGElement::GetAnimatedNumberList(uint8_t aAttrEnum)
{
=======
SVGAnimatedNumberList* SVGElement::GetAnimatedNumberList(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  NumberListAttributesInfo info = GetNumberListInfo();
  if (aAttrEnum < info.mNumberListCount) {
    return &(info.mNumberLists[aAttrEnum]);
  }
  MOZ_ASSERT(false, "Bad attrEnum");
  return nullptr;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
SVGAnimatedNumberList* nsSVGElement::GetAnimatedNumberList(nsAtom* aAttrName) {
||||||| merged common ancestors
SVGAnimatedNumberList*
nsSVGElement::GetAnimatedNumberList(nsAtom *aAttrName)
{
=======
SVGAnimatedNumberList* SVGElement::GetAnimatedNumberList(nsAtom* aAttrName) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  NumberListAttributesInfo info = GetNumberListInfo();
  for (uint32_t i = 0; i < info.mNumberListCount; i++) {
    if (aAttrName == info.mNumberListInfo[i].mName) {
      return &info.mNumberLists[i];
    }
  }
  MOZ_ASSERT(false, "Bad caller");
  return nullptr;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsAttrValue nsSVGElement::WillChangePointList() {
  MOZ_ASSERT(GetPointListAttrName(), "Changing non-existent point list?");
||||||| merged common ancestors
nsAttrValue
nsSVGElement::WillChangePointList()
{
  MOZ_ASSERT(GetPointListAttrName(),
             "Changing non-existent point list?");
=======
nsAttrValue SVGElement::WillChangePointList() {
  MOZ_ASSERT(GetPointListAttrName(), "Changing non-existent point list?");
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return WillChangeValue(GetPointListAttrName());
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangePointList(const nsAttrValue& aEmptyOrOldValue) {
  MOZ_ASSERT(GetPointListAttrName(), "Changing non-existent point list?");
||||||| merged common ancestors
void
nsSVGElement::DidChangePointList(const nsAttrValue& aEmptyOrOldValue)
{
  MOZ_ASSERT(GetPointListAttrName(),
             "Changing non-existent point list?");
=======
void SVGElement::DidChangePointList(const nsAttrValue& aEmptyOrOldValue) {
  MOZ_ASSERT(GetPointListAttrName(), "Changing non-existent point list?");
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

  nsAttrValue newValue;
  newValue.SetTo(GetAnimatedPointList()->GetBaseValue(), nullptr);

  DidChangeValue(GetPointListAttrName(), aEmptyOrOldValue, newValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimatePointList() {
  MOZ_ASSERT(GetPointListAttrName(), "Animating non-existent path data?");
||||||| merged common ancestors
void
nsSVGElement::DidAnimatePointList()
{
  MOZ_ASSERT(GetPointListAttrName(),
             "Animating non-existent path data?");
=======
void SVGElement::DidAnimatePointList() {
  MOZ_ASSERT(GetPointListAttrName(), "Animating non-existent path data?");
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

  ClearAnyCachedPath();

  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    frame->AttributeChanged(kNameSpaceID_None, GetPointListAttrName(),
                            MutationEvent_Binding::SMIL);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsAttrValue nsSVGElement::WillChangePathSegList() {
  MOZ_ASSERT(GetPathDataAttrName(), "Changing non-existent path seg list?");
||||||| merged common ancestors
nsAttrValue
nsSVGElement::WillChangePathSegList()
{
  MOZ_ASSERT(GetPathDataAttrName(),
             "Changing non-existent path seg list?");
=======
nsAttrValue SVGElement::WillChangePathSegList() {
  MOZ_ASSERT(GetPathDataAttrName(), "Changing non-existent path seg list?");
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return WillChangeValue(GetPathDataAttrName());
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangePathSegList(const nsAttrValue& aEmptyOrOldValue) {
  MOZ_ASSERT(GetPathDataAttrName(), "Changing non-existent path seg list?");
||||||| merged common ancestors
void
nsSVGElement::DidChangePathSegList(const nsAttrValue& aEmptyOrOldValue)
{
  MOZ_ASSERT(GetPathDataAttrName(),
             "Changing non-existent path seg list?");
=======
void SVGElement::DidChangePathSegList(const nsAttrValue& aEmptyOrOldValue) {
  MOZ_ASSERT(GetPathDataAttrName(), "Changing non-existent path seg list?");
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

  nsAttrValue newValue;
  newValue.SetTo(GetAnimPathSegList()->GetBaseValue(), nullptr);

  DidChangeValue(GetPathDataAttrName(), aEmptyOrOldValue, newValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimatePathSegList() {
  MOZ_ASSERT(GetPathDataAttrName(), "Animating non-existent path data?");
||||||| merged common ancestors
void
nsSVGElement::DidAnimatePathSegList()
{
  MOZ_ASSERT(GetPathDataAttrName(),
             "Animating non-existent path data?");
=======
void SVGElement::DidAnimatePathSegList() {
  MOZ_ASSERT(GetPathDataAttrName(), "Animating non-existent path data?");
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

  ClearAnyCachedPath();

  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    frame->AttributeChanged(kNameSpaceID_None, GetPathDataAttrName(),
                            MutationEvent_Binding::SMIL);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement::NumberAttributesInfo nsSVGElement::GetNumberInfo() {
||||||| merged common ancestors
nsSVGElement::NumberAttributesInfo
nsSVGElement::GetNumberInfo()
{
=======
SVGElement::NumberAttributesInfo SVGElement::GetNumberInfo() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return NumberAttributesInfo(nullptr, nullptr, 0);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::NumberAttributesInfo::Reset(uint8_t aAttrEnum) {
  mNumbers[aAttrEnum].Init(aAttrEnum, mNumberInfo[aAttrEnum].mDefaultValue);
||||||| merged common ancestors
void
nsSVGElement::NumberAttributesInfo::Reset(uint8_t aAttrEnum)
{
  mNumbers[aAttrEnum].Init(aAttrEnum,
                           mNumberInfo[aAttrEnum].mDefaultValue);
=======
void SVGElement::NumberAttributesInfo::Reset(uint8_t aAttrEnum) {
  mNumbers[aAttrEnum].Init(aAttrEnum, mNumberInfo[aAttrEnum].mDefaultValue);
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangeNumber(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::DidChangeNumber(uint8_t aAttrEnum)
{
=======
void SVGElement::DidChangeNumber(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  NumberAttributesInfo info = GetNumberInfo();

  NS_ASSERTION(info.mNumberCount > 0,
               "DidChangeNumber on element with no number attribs");
  NS_ASSERTION(aAttrEnum < info.mNumberCount, "aAttrEnum out of range");

  nsAttrValue attrValue;
  attrValue.SetTo(info.mNumbers[aAttrEnum].GetBaseValue(), nullptr);

  SetParsedAttr(kNameSpaceID_None, info.mNumberInfo[aAttrEnum].mName, nullptr,
                attrValue, true);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimateNumber(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::DidAnimateNumber(uint8_t aAttrEnum)
{
=======
void SVGElement::DidAnimateNumber(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    NumberAttributesInfo info = GetNumberInfo();
    frame->AttributeChanged(kNameSpaceID_None,
                            info.mNumberInfo[aAttrEnum].mName,
                            MutationEvent_Binding::SMIL);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::GetAnimatedNumberValues(float* aFirst, ...) {
||||||| merged common ancestors
void
nsSVGElement::GetAnimatedNumberValues(float *aFirst, ...)
{
=======
void SVGElement::GetAnimatedNumberValues(float* aFirst, ...) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  NumberAttributesInfo info = GetNumberInfo();

  NS_ASSERTION(info.mNumberCount > 0,
               "GetAnimatedNumberValues on element with no number attribs");

  float* f = aFirst;
  uint32_t i = 0;

  va_list args;
  va_start(args, aFirst);

  while (f && i < info.mNumberCount) {
    *f = info.mNumbers[i++].GetAnimValue();
    f = va_arg(args, float*);
  }
  va_end(args);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement::NumberPairAttributesInfo nsSVGElement::GetNumberPairInfo() {
||||||| merged common ancestors
nsSVGElement::NumberPairAttributesInfo
nsSVGElement::GetNumberPairInfo()
{
=======
SVGElement::NumberPairAttributesInfo SVGElement::GetNumberPairInfo() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return NumberPairAttributesInfo(nullptr, nullptr, 0);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::NumberPairAttributesInfo::Reset(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::NumberPairAttributesInfo::Reset(uint8_t aAttrEnum)
{
=======
void SVGElement::NumberPairAttributesInfo::Reset(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  mNumberPairs[aAttrEnum].Init(aAttrEnum,
                               mNumberPairInfo[aAttrEnum].mDefaultValue1,
                               mNumberPairInfo[aAttrEnum].mDefaultValue2);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsAttrValue nsSVGElement::WillChangeNumberPair(uint8_t aAttrEnum) {
||||||| merged common ancestors
nsAttrValue
nsSVGElement::WillChangeNumberPair(uint8_t aAttrEnum)
{
=======
nsAttrValue SVGElement::WillChangeNumberPair(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return WillChangeValue(GetNumberPairInfo().mNumberPairInfo[aAttrEnum].mName);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangeNumberPair(uint8_t aAttrEnum,
                                       const nsAttrValue& aEmptyOrOldValue) {
||||||| merged common ancestors
void
nsSVGElement::DidChangeNumberPair(uint8_t aAttrEnum,
                                  const nsAttrValue& aEmptyOrOldValue)
{
=======
void SVGElement::DidChangeNumberPair(uint8_t aAttrEnum,
                                     const nsAttrValue& aEmptyOrOldValue) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  NumberPairAttributesInfo info = GetNumberPairInfo();

  NS_ASSERTION(info.mNumberPairCount > 0,
               "DidChangePairNumber on element with no number pair attribs");
  NS_ASSERTION(aAttrEnum < info.mNumberPairCount, "aAttrEnum out of range");

  nsAttrValue newValue;
  newValue.SetTo(info.mNumberPairs[aAttrEnum], nullptr);

  DidChangeValue(info.mNumberPairInfo[aAttrEnum].mName, aEmptyOrOldValue,
                 newValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimateNumberPair(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::DidAnimateNumberPair(uint8_t aAttrEnum)
{
=======
void SVGElement::DidAnimateNumberPair(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    NumberPairAttributesInfo info = GetNumberPairInfo();
    frame->AttributeChanged(kNameSpaceID_None,
                            info.mNumberPairInfo[aAttrEnum].mName,
                            MutationEvent_Binding::SMIL);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement::IntegerAttributesInfo nsSVGElement::GetIntegerInfo() {
||||||| merged common ancestors
nsSVGElement::IntegerAttributesInfo
nsSVGElement::GetIntegerInfo()
{
=======
SVGElement::IntegerAttributesInfo SVGElement::GetIntegerInfo() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return IntegerAttributesInfo(nullptr, nullptr, 0);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::IntegerAttributesInfo::Reset(uint8_t aAttrEnum) {
  mIntegers[aAttrEnum].Init(aAttrEnum, mIntegerInfo[aAttrEnum].mDefaultValue);
||||||| merged common ancestors
void
nsSVGElement::IntegerAttributesInfo::Reset(uint8_t aAttrEnum)
{
  mIntegers[aAttrEnum].Init(aAttrEnum,
                            mIntegerInfo[aAttrEnum].mDefaultValue);
=======
void SVGElement::IntegerAttributesInfo::Reset(uint8_t aAttrEnum) {
  mIntegers[aAttrEnum].Init(aAttrEnum, mIntegerInfo[aAttrEnum].mDefaultValue);
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangeInteger(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::DidChangeInteger(uint8_t aAttrEnum)
{
=======
void SVGElement::DidChangeInteger(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  IntegerAttributesInfo info = GetIntegerInfo();

  NS_ASSERTION(info.mIntegerCount > 0,
               "DidChangeInteger on element with no integer attribs");
  NS_ASSERTION(aAttrEnum < info.mIntegerCount, "aAttrEnum out of range");

  nsAttrValue attrValue;
  attrValue.SetTo(info.mIntegers[aAttrEnum].GetBaseValue(), nullptr);

  SetParsedAttr(kNameSpaceID_None, info.mIntegerInfo[aAttrEnum].mName, nullptr,
                attrValue, true);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimateInteger(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::DidAnimateInteger(uint8_t aAttrEnum)
{
=======
void SVGElement::DidAnimateInteger(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    IntegerAttributesInfo info = GetIntegerInfo();
    frame->AttributeChanged(kNameSpaceID_None,
                            info.mIntegerInfo[aAttrEnum].mName,
                            MutationEvent_Binding::SMIL);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::GetAnimatedIntegerValues(int32_t* aFirst, ...) {
||||||| merged common ancestors
void
nsSVGElement::GetAnimatedIntegerValues(int32_t *aFirst, ...)
{
=======
void SVGElement::GetAnimatedIntegerValues(int32_t* aFirst, ...) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  IntegerAttributesInfo info = GetIntegerInfo();

  NS_ASSERTION(info.mIntegerCount > 0,
               "GetAnimatedIntegerValues on element with no integer attribs");

  int32_t* n = aFirst;
  uint32_t i = 0;

  va_list args;
  va_start(args, aFirst);

  while (n && i < info.mIntegerCount) {
    *n = info.mIntegers[i++].GetAnimValue();
    n = va_arg(args, int32_t*);
  }
  va_end(args);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement::IntegerPairAttributesInfo nsSVGElement::GetIntegerPairInfo() {
||||||| merged common ancestors
nsSVGElement::IntegerPairAttributesInfo
nsSVGElement::GetIntegerPairInfo()
{
=======
SVGElement::IntegerPairAttributesInfo SVGElement::GetIntegerPairInfo() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return IntegerPairAttributesInfo(nullptr, nullptr, 0);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::IntegerPairAttributesInfo::Reset(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::IntegerPairAttributesInfo::Reset(uint8_t aAttrEnum)
{
=======
void SVGElement::IntegerPairAttributesInfo::Reset(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  mIntegerPairs[aAttrEnum].Init(aAttrEnum,
                                mIntegerPairInfo[aAttrEnum].mDefaultValue1,
                                mIntegerPairInfo[aAttrEnum].mDefaultValue2);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsAttrValue nsSVGElement::WillChangeIntegerPair(uint8_t aAttrEnum) {
||||||| merged common ancestors
nsAttrValue
nsSVGElement::WillChangeIntegerPair(uint8_t aAttrEnum)
{
=======
nsAttrValue SVGElement::WillChangeIntegerPair(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return WillChangeValue(
      GetIntegerPairInfo().mIntegerPairInfo[aAttrEnum].mName);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangeIntegerPair(uint8_t aAttrEnum,
                                        const nsAttrValue& aEmptyOrOldValue) {
||||||| merged common ancestors
void
nsSVGElement::DidChangeIntegerPair(uint8_t aAttrEnum,
                                   const nsAttrValue& aEmptyOrOldValue)
{
=======
void SVGElement::DidChangeIntegerPair(uint8_t aAttrEnum,
                                      const nsAttrValue& aEmptyOrOldValue) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  IntegerPairAttributesInfo info = GetIntegerPairInfo();

  NS_ASSERTION(info.mIntegerPairCount > 0,
               "DidChangeIntegerPair on element with no integer pair attribs");
  NS_ASSERTION(aAttrEnum < info.mIntegerPairCount, "aAttrEnum out of range");

  nsAttrValue newValue;
  newValue.SetTo(info.mIntegerPairs[aAttrEnum], nullptr);

  DidChangeValue(info.mIntegerPairInfo[aAttrEnum].mName, aEmptyOrOldValue,
                 newValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimateIntegerPair(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::DidAnimateIntegerPair(uint8_t aAttrEnum)
{
=======
void SVGElement::DidAnimateIntegerPair(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    IntegerPairAttributesInfo info = GetIntegerPairInfo();
    frame->AttributeChanged(kNameSpaceID_None,
                            info.mIntegerPairInfo[aAttrEnum].mName,
                            MutationEvent_Binding::SMIL);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement::AngleAttributesInfo nsSVGElement::GetAngleInfo() {
  return AngleAttributesInfo(nullptr, nullptr, 0);
}

void nsSVGElement::AngleAttributesInfo::Reset(uint8_t aAttrEnum) {
  mAngles[aAttrEnum].Init(aAttrEnum, mAngleInfo[aAttrEnum].mDefaultValue,
                          mAngleInfo[aAttrEnum].mDefaultUnitType);
}

nsAttrValue nsSVGElement::WillChangeAngle(uint8_t aAttrEnum) {
  return WillChangeValue(GetAngleInfo().mAngleInfo[aAttrEnum].mName);
}

void nsSVGElement::DidChangeAngle(uint8_t aAttrEnum,
                                  const nsAttrValue& aEmptyOrOldValue) {
  AngleAttributesInfo info = GetAngleInfo();

  NS_ASSERTION(info.mAngleCount > 0,
               "DidChangeAngle on element with no angle attribs");
  NS_ASSERTION(aAttrEnum < info.mAngleCount, "aAttrEnum out of range");

  nsAttrValue newValue;
  newValue.SetTo(info.mAngles[aAttrEnum], nullptr);

  DidChangeValue(info.mAngleInfo[aAttrEnum].mName, aEmptyOrOldValue, newValue);
}

void nsSVGElement::DidAnimateAngle(uint8_t aAttrEnum) {
  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    AngleAttributesInfo info = GetAngleInfo();
    frame->AttributeChanged(kNameSpaceID_None, info.mAngleInfo[aAttrEnum].mName,
                            MutationEvent_Binding::SMIL);
  }
}

nsSVGElement::BooleanAttributesInfo nsSVGElement::GetBooleanInfo() {
||||||| merged common ancestors
nsSVGElement::AngleAttributesInfo
nsSVGElement::GetAngleInfo()
{
  return AngleAttributesInfo(nullptr, nullptr, 0);
}

void
nsSVGElement::AngleAttributesInfo::Reset(uint8_t aAttrEnum)
{
  mAngles[aAttrEnum].Init(aAttrEnum,
                          mAngleInfo[aAttrEnum].mDefaultValue,
                          mAngleInfo[aAttrEnum].mDefaultUnitType);
}

nsAttrValue
nsSVGElement::WillChangeAngle(uint8_t aAttrEnum)
{
  return WillChangeValue(GetAngleInfo().mAngleInfo[aAttrEnum].mName);
}

void
nsSVGElement::DidChangeAngle(uint8_t aAttrEnum,
                             const nsAttrValue& aEmptyOrOldValue)
{
  AngleAttributesInfo info = GetAngleInfo();

  NS_ASSERTION(info.mAngleCount > 0,
               "DidChangeAngle on element with no angle attribs");
  NS_ASSERTION(aAttrEnum < info.mAngleCount, "aAttrEnum out of range");

  nsAttrValue newValue;
  newValue.SetTo(info.mAngles[aAttrEnum], nullptr);

  DidChangeValue(info.mAngleInfo[aAttrEnum].mName, aEmptyOrOldValue, newValue);
}

void
nsSVGElement::DidAnimateAngle(uint8_t aAttrEnum)
{
  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    AngleAttributesInfo info = GetAngleInfo();
    frame->AttributeChanged(kNameSpaceID_None,
                            info.mAngleInfo[aAttrEnum].mName,
                            MutationEvent_Binding::SMIL);
  }
}

nsSVGElement::BooleanAttributesInfo
nsSVGElement::GetBooleanInfo()
{
=======
SVGElement::BooleanAttributesInfo SVGElement::GetBooleanInfo() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return BooleanAttributesInfo(nullptr, nullptr, 0);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::BooleanAttributesInfo::Reset(uint8_t aAttrEnum) {
  mBooleans[aAttrEnum].Init(aAttrEnum, mBooleanInfo[aAttrEnum].mDefaultValue);
||||||| merged common ancestors
void
nsSVGElement::BooleanAttributesInfo::Reset(uint8_t aAttrEnum)
{
  mBooleans[aAttrEnum].Init(aAttrEnum,
                            mBooleanInfo[aAttrEnum].mDefaultValue);
=======
void SVGElement::BooleanAttributesInfo::Reset(uint8_t aAttrEnum) {
  mBooleans[aAttrEnum].Init(aAttrEnum, mBooleanInfo[aAttrEnum].mDefaultValue);
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangeBoolean(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::DidChangeBoolean(uint8_t aAttrEnum)
{
=======
void SVGElement::DidChangeBoolean(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  BooleanAttributesInfo info = GetBooleanInfo();

  NS_ASSERTION(info.mBooleanCount > 0,
               "DidChangeBoolean on element with no boolean attribs");
  NS_ASSERTION(aAttrEnum < info.mBooleanCount, "aAttrEnum out of range");

  nsAttrValue attrValue(info.mBooleans[aAttrEnum].GetBaseValueAtom());
  SetParsedAttr(kNameSpaceID_None, info.mBooleanInfo[aAttrEnum].mName, nullptr,
                attrValue, true);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimateBoolean(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::DidAnimateBoolean(uint8_t aAttrEnum)
{
=======
void SVGElement::DidAnimateBoolean(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    BooleanAttributesInfo info = GetBooleanInfo();
    frame->AttributeChanged(kNameSpaceID_None,
                            info.mBooleanInfo[aAttrEnum].mName,
                            MutationEvent_Binding::SMIL);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement::EnumAttributesInfo nsSVGElement::GetEnumInfo() {
||||||| merged common ancestors
nsSVGElement::EnumAttributesInfo
nsSVGElement::GetEnumInfo()
{
=======
SVGElement::EnumAttributesInfo SVGElement::GetEnumInfo() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return EnumAttributesInfo(nullptr, nullptr, 0);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::EnumAttributesInfo::Reset(uint8_t aAttrEnum) {
  mEnums[aAttrEnum].Init(aAttrEnum, mEnumInfo[aAttrEnum].mDefaultValue);
||||||| merged common ancestors
void
nsSVGElement::EnumAttributesInfo::Reset(uint8_t aAttrEnum)
{
  mEnums[aAttrEnum].Init(aAttrEnum,
                         mEnumInfo[aAttrEnum].mDefaultValue);
=======
void SVGElement::EnumAttributesInfo::Reset(uint8_t aAttrEnum) {
  mEnums[aAttrEnum].Init(aAttrEnum, mEnumInfo[aAttrEnum].mDefaultValue);
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::EnumAttributesInfo::SetUnknownValue(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::EnumAttributesInfo::SetUnknownValue(uint8_t aAttrEnum)
{
=======
void SVGElement::EnumAttributesInfo::SetUnknownValue(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  // Fortunately in SVG every enum's unknown value is 0
  mEnums[aAttrEnum].Init(aAttrEnum, 0);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangeEnum(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::DidChangeEnum(uint8_t aAttrEnum)
{
=======
void SVGElement::DidChangeEnum(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  EnumAttributesInfo info = GetEnumInfo();

  NS_ASSERTION(info.mEnumCount > 0,
               "DidChangeEnum on element with no enum attribs");
  NS_ASSERTION(aAttrEnum < info.mEnumCount, "aAttrEnum out of range");

  nsAttrValue attrValue(info.mEnums[aAttrEnum].GetBaseValueAtom(this));
  SetParsedAttr(kNameSpaceID_None, info.mEnumInfo[aAttrEnum].mName, nullptr,
                attrValue, true);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimateEnum(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::DidAnimateEnum(uint8_t aAttrEnum)
{
=======
void SVGElement::DidAnimateEnum(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    EnumAttributesInfo info = GetEnumInfo();
    frame->AttributeChanged(kNameSpaceID_None, info.mEnumInfo[aAttrEnum].mName,
                            MutationEvent_Binding::SMIL);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGViewBox* nsSVGElement::GetViewBox() { return nullptr; }
||||||| merged common ancestors
nsSVGViewBox *
nsSVGElement::GetViewBox()
{
  return nullptr;
}
=======
SVGAnimatedOrient* SVGElement::GetAnimatedOrient() { return nullptr; }

nsAttrValue SVGElement::WillChangeOrient() {
  return WillChangeValue(nsGkAtoms::orient);
}

void SVGElement::DidChangeOrient(const nsAttrValue& aEmptyOrOldValue) {
  SVGAnimatedOrient* orient = GetAnimatedOrient();

  NS_ASSERTION(orient, "DidChangeOrient on element with no orient attrib");

  nsAttrValue newValue;
  newValue.SetTo(*orient, nullptr);

  DidChangeValue(nsGkAtoms::orient, aEmptyOrOldValue, newValue);
}
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsAttrValue nsSVGElement::WillChangeViewBox() {
||||||| merged common ancestors
nsAttrValue
nsSVGElement::WillChangeViewBox()
{
=======
void SVGElement::DidAnimateOrient() {
  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    frame->AttributeChanged(kNameSpaceID_None, nsGkAtoms::orient,
                            MutationEvent_Binding::SMIL);
  }
}

SVGAnimatedViewBox* SVGElement::GetAnimatedViewBox() { return nullptr; }

nsAttrValue SVGElement::WillChangeViewBox() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return WillChangeValue(nsGkAtoms::viewBox);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangeViewBox(const nsAttrValue& aEmptyOrOldValue) {
  nsSVGViewBox* viewBox = GetViewBox();
||||||| merged common ancestors
void
nsSVGElement::DidChangeViewBox(const nsAttrValue& aEmptyOrOldValue)
{
  nsSVGViewBox *viewBox = GetViewBox();
=======
void SVGElement::DidChangeViewBox(const nsAttrValue& aEmptyOrOldValue) {
  SVGAnimatedViewBox* viewBox = GetAnimatedViewBox();
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

  NS_ASSERTION(viewBox, "DidChangeViewBox on element with no viewBox attrib");

  nsAttrValue newValue;
  newValue.SetTo(*viewBox, nullptr);

  DidChangeValue(nsGkAtoms::viewBox, aEmptyOrOldValue, newValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimateViewBox() {
||||||| merged common ancestors
void
nsSVGElement::DidAnimateViewBox()
{
=======
void SVGElement::DidAnimateViewBox() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    frame->AttributeChanged(kNameSpaceID_None, nsGkAtoms::viewBox,
                            MutationEvent_Binding::SMIL);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
SVGAnimatedPreserveAspectRatio* nsSVGElement::GetPreserveAspectRatio() {
||||||| merged common ancestors
SVGAnimatedPreserveAspectRatio *
nsSVGElement::GetPreserveAspectRatio()
{
=======
SVGAnimatedPreserveAspectRatio* SVGElement::GetAnimatedPreserveAspectRatio() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return nullptr;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsAttrValue nsSVGElement::WillChangePreserveAspectRatio() {
||||||| merged common ancestors
nsAttrValue
nsSVGElement::WillChangePreserveAspectRatio()
{
=======
nsAttrValue SVGElement::WillChangePreserveAspectRatio() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return WillChangeValue(nsGkAtoms::preserveAspectRatio);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangePreserveAspectRatio(
    const nsAttrValue& aEmptyOrOldValue) {
  SVGAnimatedPreserveAspectRatio* preserveAspectRatio =
      GetPreserveAspectRatio();
||||||| merged common ancestors
void
nsSVGElement::DidChangePreserveAspectRatio(const nsAttrValue& aEmptyOrOldValue)
{
  SVGAnimatedPreserveAspectRatio *preserveAspectRatio =
    GetPreserveAspectRatio();
=======
void SVGElement::DidChangePreserveAspectRatio(
    const nsAttrValue& aEmptyOrOldValue) {
  SVGAnimatedPreserveAspectRatio* preserveAspectRatio =
      GetAnimatedPreserveAspectRatio();
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

  NS_ASSERTION(preserveAspectRatio,
               "DidChangePreserveAspectRatio on element with no "
               "preserveAspectRatio attrib");

  nsAttrValue newValue;
  newValue.SetTo(*preserveAspectRatio, nullptr);

  DidChangeValue(nsGkAtoms::preserveAspectRatio, aEmptyOrOldValue, newValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimatePreserveAspectRatio() {
||||||| merged common ancestors
void
nsSVGElement::DidAnimatePreserveAspectRatio()
{
=======
void SVGElement::DidAnimatePreserveAspectRatio() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    frame->AttributeChanged(kNameSpaceID_None, nsGkAtoms::preserveAspectRatio,
                            MutationEvent_Binding::SMIL);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsAttrValue nsSVGElement::WillChangeTransformList() {
||||||| merged common ancestors
nsAttrValue
nsSVGElement::WillChangeTransformList()
{
=======
nsAttrValue SVGElement::WillChangeTransformList() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return WillChangeValue(GetTransformListAttrName());
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangeTransformList(const nsAttrValue& aEmptyOrOldValue) {
||||||| merged common ancestors
void
nsSVGElement::DidChangeTransformList(const nsAttrValue& aEmptyOrOldValue)
{
=======
void SVGElement::DidChangeTransformList(const nsAttrValue& aEmptyOrOldValue) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  MOZ_ASSERT(GetTransformListAttrName(),
             "Changing non-existent transform list?");

  // The transform attribute is being set, so we must ensure that the
  // SVGAnimatedTransformList is/has been allocated:
  nsAttrValue newValue;
  newValue.SetTo(GetAnimatedTransformList(DO_ALLOCATE)->GetBaseValue(),
                 nullptr);

  DidChangeValue(GetTransformListAttrName(), aEmptyOrOldValue, newValue);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimateTransformList(int32_t aModType) {
||||||| merged common ancestors
void
nsSVGElement::DidAnimateTransformList(int32_t aModType)
{
=======
void SVGElement::DidAnimateTransformList(int32_t aModType) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  MOZ_ASSERT(GetTransformListAttrName(),
             "Animating non-existent transform data?");

  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    nsAtom* transformAttr = GetTransformListAttrName();
    frame->AttributeChanged(kNameSpaceID_None, transformAttr, aModType);
    // When script changes the 'transform' attribute, Element::SetAttrAndNotify
    // will call nsNodeUtils::AttributeChanged, under which
    // SVGTransformableElement::GetAttributeChangeHint will be called and an
    // appropriate change event posted to update our frame's overflow rects.
    // The SetAttrAndNotify doesn't happen for transform changes caused by
    // 'animateTransform' though (and sending out the mutation events that
    // nsNodeUtils::AttributeChanged dispatches would be inappropriate
    // anyway), so we need to post the change event ourself.
    nsChangeHint changeHint = GetAttributeChangeHint(transformAttr, aModType);
    if (changeHint) {
      nsLayoutUtils::PostRestyleEvent(this, RestyleHint{0}, changeHint);
    }
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement::StringAttributesInfo nsSVGElement::GetStringInfo() {
||||||| merged common ancestors
nsSVGElement::StringAttributesInfo
nsSVGElement::GetStringInfo()
{
=======
SVGElement::StringAttributesInfo SVGElement::GetStringInfo() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return StringAttributesInfo(nullptr, nullptr, 0);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::StringAttributesInfo::Reset(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::StringAttributesInfo::Reset(uint8_t aAttrEnum)
{
=======
void SVGElement::StringAttributesInfo::Reset(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  mStrings[aAttrEnum].Init(aAttrEnum);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::GetStringBaseValue(uint8_t aAttrEnum,
                                      nsAString& aResult) const {
  nsSVGElement::StringAttributesInfo info =
      const_cast<nsSVGElement*>(this)->GetStringInfo();
||||||| merged common ancestors
void
nsSVGElement::GetStringBaseValue(uint8_t aAttrEnum, nsAString& aResult) const
{
  nsSVGElement::StringAttributesInfo info = const_cast<nsSVGElement*>(this)->GetStringInfo();
=======
void SVGElement::GetStringBaseValue(uint8_t aAttrEnum,
                                    nsAString& aResult) const {
  SVGElement::StringAttributesInfo info =
      const_cast<SVGElement*>(this)->GetStringInfo();
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

  NS_ASSERTION(info.mStringCount > 0,
               "GetBaseValue on element with no string attribs");

  NS_ASSERTION(aAttrEnum < info.mStringCount, "aAttrEnum out of range");

  GetAttr(info.mStringInfo[aAttrEnum].mNamespaceID,
          info.mStringInfo[aAttrEnum].mName, aResult);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::SetStringBaseValue(uint8_t aAttrEnum,
                                      const nsAString& aValue) {
  nsSVGElement::StringAttributesInfo info = GetStringInfo();
||||||| merged common ancestors
void
nsSVGElement::SetStringBaseValue(uint8_t aAttrEnum, const nsAString& aValue)
{
  nsSVGElement::StringAttributesInfo info = GetStringInfo();
=======
void SVGElement::SetStringBaseValue(uint8_t aAttrEnum,
                                    const nsAString& aValue) {
  SVGElement::StringAttributesInfo info = GetStringInfo();
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp

  NS_ASSERTION(info.mStringCount > 0,
               "SetBaseValue on element with no string attribs");

  NS_ASSERTION(aAttrEnum < info.mStringCount, "aAttrEnum out of range");

  SetAttr(info.mStringInfo[aAttrEnum].mNamespaceID,
          info.mStringInfo[aAttrEnum].mName, aValue, true);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidAnimateString(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::DidAnimateString(uint8_t aAttrEnum)
{
=======
void SVGElement::DidAnimateString(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsIFrame* frame = GetPrimaryFrame();

  if (frame) {
    StringAttributesInfo info = GetStringInfo();
    frame->AttributeChanged(info.mStringInfo[aAttrEnum].mNamespaceID,
                            info.mStringInfo[aAttrEnum].mName,
                            MutationEvent_Binding::SMIL);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsSVGElement::StringListAttributesInfo nsSVGElement::GetStringListInfo() {
||||||| merged common ancestors
nsSVGElement::StringListAttributesInfo
nsSVGElement::GetStringListInfo()
{
=======
SVGElement::StringListAttributesInfo SVGElement::GetStringListInfo() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  return StringListAttributesInfo(nullptr, nullptr, 0);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsAttrValue nsSVGElement::WillChangeStringList(
    bool aIsConditionalProcessingAttribute, uint8_t aAttrEnum) {
||||||| merged common ancestors
nsAttrValue
nsSVGElement::WillChangeStringList(bool aIsConditionalProcessingAttribute,
                                   uint8_t aAttrEnum)
{
=======
nsAttrValue SVGElement::WillChangeStringList(
    bool aIsConditionalProcessingAttribute, uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsStaticAtom* name;
  if (aIsConditionalProcessingAttribute) {
    nsCOMPtr<SVGTests> tests(do_QueryInterface(this));
    name = tests->GetAttrName(aAttrEnum);
  } else {
    name = GetStringListInfo().mStringListInfo[aAttrEnum].mName;
  }
  return WillChangeValue(name);
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::DidChangeStringList(bool aIsConditionalProcessingAttribute,
                                       uint8_t aAttrEnum,
                                       const nsAttrValue& aEmptyOrOldValue) {
||||||| merged common ancestors
void
nsSVGElement::DidChangeStringList(bool aIsConditionalProcessingAttribute,
                                  uint8_t aAttrEnum,
                                  const nsAttrValue& aEmptyOrOldValue)
{
=======
void SVGElement::DidChangeStringList(bool aIsConditionalProcessingAttribute,
                                     uint8_t aAttrEnum,
                                     const nsAttrValue& aEmptyOrOldValue) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  nsStaticAtom* name;
  nsAttrValue newValue;
  nsCOMPtr<SVGTests> tests;

  if (aIsConditionalProcessingAttribute) {
    tests = do_QueryObject(this);
    name = tests->GetAttrName(aAttrEnum);
    tests->GetAttrValue(aAttrEnum, newValue);
  } else {
    StringListAttributesInfo info = GetStringListInfo();

    NS_ASSERTION(info.mStringListCount > 0,
                 "DidChangeStringList on element with no string list attribs");
    NS_ASSERTION(aAttrEnum < info.mStringListCount, "aAttrEnum out of range");

    name = info.mStringListInfo[aAttrEnum].mName;
    newValue.SetTo(info.mStringLists[aAttrEnum], nullptr);
  }

  DidChangeValue(name, aEmptyOrOldValue, newValue);

  if (aIsConditionalProcessingAttribute) {
    tests->MaybeInvalidate();
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::StringListAttributesInfo::Reset(uint8_t aAttrEnum) {
||||||| merged common ancestors
void
nsSVGElement::StringListAttributesInfo::Reset(uint8_t aAttrEnum)
{
=======
void SVGElement::StringListAttributesInfo::Reset(uint8_t aAttrEnum) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  mStringLists[aAttrEnum].Clear();
  // caller notifies
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
nsresult nsSVGElement::ReportAttributeParseFailure(nsIDocument* aDocument,
                                                   nsAtom* aAttribute,
                                                   const nsAString& aValue) {
  const nsString& attributeValue = PromiseFlatString(aValue);
  const char16_t* strings[] = {aAttribute->GetUTF16String(),
                               attributeValue.get()};
  return SVGContentUtils::ReportToConsole(aDocument, "AttributeParseWarning",
                                          strings, ArrayLength(strings));
||||||| merged common ancestors
nsresult
nsSVGElement::ReportAttributeParseFailure(nsIDocument* aDocument,
                                          nsAtom* aAttribute,
                                          const nsAString& aValue)
{
  const nsString& attributeValue = PromiseFlatString(aValue);
  const char16_t *strings[] = { aAttribute->GetUTF16String(),
                                 attributeValue.get() };
  return SVGContentUtils::ReportToConsole(aDocument,
                                          "AttributeParseWarning",
                                          strings, ArrayLength(strings));
=======
nsresult SVGElement::ReportAttributeParseFailure(Document* aDocument,
                                                 nsAtom* aAttribute,
                                                 const nsAString& aValue) {
  AutoTArray<nsString, 2> strings;
  strings.AppendElement(nsDependentAtomString(aAttribute));
  strings.AppendElement(aValue);
  return SVGContentUtils::ReportToConsole(aDocument, "AttributeParseWarning",
                                          strings);
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::RecompileScriptEventListeners() {
||||||| merged common ancestors
void
nsSVGElement::RecompileScriptEventListeners()
{
=======
void SVGElement::RecompileScriptEventListeners() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  int32_t i, count = mAttrs.AttrCount();
  for (i = 0; i < count; ++i) {
    const nsAttrName* name = mAttrs.AttrNameAt(i);

    // Eventlistenener-attributes are always in the null namespace
    if (!name->IsAtom()) {
      continue;
    }

    nsAtom* attr = name->Atom();
    if (!IsEventAttributeName(attr)) {
      continue;
    }

    nsAutoString value;
    GetAttr(attr, value);
    SetEventHandler(GetEventNameForAttr(attr), value, true);
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
UniquePtr<nsISMILAttr> nsSVGElement::GetAnimatedAttr(int32_t aNamespaceID,
                                                     nsAtom* aName) {
||||||| merged common ancestors
UniquePtr<nsISMILAttr>
nsSVGElement::GetAnimatedAttr(int32_t aNamespaceID, nsAtom* aName)
{
=======
UniquePtr<SMILAttr> SVGElement::GetAnimatedAttr(int32_t aNamespaceID,
                                                nsAtom* aName) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  if (aNamespaceID == kNameSpaceID_None) {
    // Transforms:
    if (GetTransformListAttrName() == aName) {
      // The transform attribute is being animated, so we must ensure that the
      // SVGAnimatedTransformList is/has been allocated:
      return GetAnimatedTransformList(DO_ALLOCATE)->ToSMILAttr(this);
    }

    // Motion (fake 'attribute' for animateMotion)
    if (aName == nsGkAtoms::mozAnimateMotionDummyAttr) {
      return MakeUnique<SVGMotionSMILAttr>(this);
    }

    // Lengths:
    LengthAttributesInfo info = GetLengthInfo();
    for (uint32_t i = 0; i < info.mLengthCount; i++) {
      if (aName == info.mLengthInfo[i].mName) {
        return info.mLengths[i].ToSMILAttr(this);
      }
    }

    // Numbers:
    {
      NumberAttributesInfo info = GetNumberInfo();
      for (uint32_t i = 0; i < info.mNumberCount; i++) {
        if (aName == info.mNumberInfo[i].mName) {
          return info.mNumbers[i].ToSMILAttr(this);
        }
      }
    }

    // Number Pairs:
    {
      NumberPairAttributesInfo info = GetNumberPairInfo();
      for (uint32_t i = 0; i < info.mNumberPairCount; i++) {
        if (aName == info.mNumberPairInfo[i].mName) {
          return info.mNumberPairs[i].ToSMILAttr(this);
        }
      }
    }

    // Integers:
    {
      IntegerAttributesInfo info = GetIntegerInfo();
      for (uint32_t i = 0; i < info.mIntegerCount; i++) {
        if (aName == info.mIntegerInfo[i].mName) {
          return info.mIntegers[i].ToSMILAttr(this);
        }
      }
    }

    // Integer Pairs:
    {
      IntegerPairAttributesInfo info = GetIntegerPairInfo();
      for (uint32_t i = 0; i < info.mIntegerPairCount; i++) {
        if (aName == info.mIntegerPairInfo[i].mName) {
          return info.mIntegerPairs[i].ToSMILAttr(this);
        }
      }
    }

    // Enumerations:
    {
      EnumAttributesInfo info = GetEnumInfo();
      for (uint32_t i = 0; i < info.mEnumCount; i++) {
        if (aName == info.mEnumInfo[i].mName) {
          return info.mEnums[i].ToSMILAttr(this);
        }
      }
    }

    // Booleans:
    {
      BooleanAttributesInfo info = GetBooleanInfo();
      for (uint32_t i = 0; i < info.mBooleanCount; i++) {
        if (aName == info.mBooleanInfo[i].mName) {
          return info.mBooleans[i].ToSMILAttr(this);
        }
      }
    }

    // orient:
    if (aName == nsGkAtoms::orient) {
      SVGAnimatedOrient* orient = GetAnimatedOrient();
      return orient ? orient->ToSMILAttr(this) : nullptr;
    }

    // viewBox:
    if (aName == nsGkAtoms::viewBox) {
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
      nsSVGViewBox* viewBox = GetViewBox();
||||||| merged common ancestors
      nsSVGViewBox *viewBox = GetViewBox();
=======
      SVGAnimatedViewBox* viewBox = GetAnimatedViewBox();
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
      return viewBox ? viewBox->ToSMILAttr(this) : nullptr;
    }

    // preserveAspectRatio:
    if (aName == nsGkAtoms::preserveAspectRatio) {
<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
      SVGAnimatedPreserveAspectRatio* preserveAspectRatio =
          GetPreserveAspectRatio();
      return preserveAspectRatio ? preserveAspectRatio->ToSMILAttr(this)
                                 : nullptr;
||||||| merged common ancestors
      SVGAnimatedPreserveAspectRatio *preserveAspectRatio =
        GetPreserveAspectRatio();
      return preserveAspectRatio ?
        preserveAspectRatio->ToSMILAttr(this) : nullptr;
=======
      SVGAnimatedPreserveAspectRatio* preserveAspectRatio =
          GetAnimatedPreserveAspectRatio();
      return preserveAspectRatio ? preserveAspectRatio->ToSMILAttr(this)
                                 : nullptr;
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
    }

    // NumberLists:
    {
      NumberListAttributesInfo info = GetNumberListInfo();
      for (uint32_t i = 0; i < info.mNumberListCount; i++) {
        if (aName == info.mNumberListInfo[i].mName) {
          MOZ_ASSERT(i <= UCHAR_MAX, "Too many attributes");
          return info.mNumberLists[i].ToSMILAttr(this, uint8_t(i));
        }
      }
    }

    // LengthLists:
    {
      LengthListAttributesInfo info = GetLengthListInfo();
      for (uint32_t i = 0; i < info.mLengthListCount; i++) {
        if (aName == info.mLengthListInfo[i].mName) {
          MOZ_ASSERT(i <= UCHAR_MAX, "Too many attributes");
          return info.mLengthLists[i].ToSMILAttr(
              this, uint8_t(i), info.mLengthListInfo[i].mAxis,
              info.mLengthListInfo[i].mCouldZeroPadList);
        }
      }
    }

    // PointLists:
    {
      if (GetPointListAttrName() == aName) {
        SVGAnimatedPointList* pointList = GetAnimatedPointList();
        if (pointList) {
          return pointList->ToSMILAttr(this);
        }
      }
    }

    // PathSegLists:
    {
      if (GetPathDataAttrName() == aName) {
        SVGAnimatedPathSegList* segList = GetAnimPathSegList();
        if (segList) {
          return segList->ToSMILAttr(this);
        }
      }
    }

    if (aName == nsGkAtoms::_class) {
      return mClassAttribute.ToSMILAttr(this);
    }
  }

  // Strings
  {
    StringAttributesInfo info = GetStringInfo();
    for (uint32_t i = 0; i < info.mStringCount; i++) {
      if (aNamespaceID == info.mStringInfo[i].mNamespaceID &&
          aName == info.mStringInfo[i].mName) {
        return info.mStrings[i].ToSMILAttr(this);
      }
    }
  }

  return nullptr;
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::AnimationNeedsResample() {
  nsIDocument* doc = GetComposedDoc();
||||||| merged common ancestors
void
nsSVGElement::AnimationNeedsResample()
{
  nsIDocument* doc = GetComposedDoc();
=======
void SVGElement::AnimationNeedsResample() {
  Document* doc = GetComposedDoc();
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  if (doc && doc->HasAnimationController()) {
    doc->GetAnimationController()->SetResampleNeeded();
  }
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGElement.cpp
void nsSVGElement::FlushAnimations() {
  nsIDocument* doc = GetComposedDoc();
||||||| merged common ancestors
void
nsSVGElement::FlushAnimations()
{
  nsIDocument* doc = GetComposedDoc();
=======
void SVGElement::FlushAnimations() {
  Document* doc = GetComposedDoc();
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGElement.cpp
  if (doc && doc->HasAnimationController()) {
    doc->GetAnimationController()->FlushResampleRequests();
  }
}

void SVGElement::AddSizeOfExcludingThis(nsWindowSizes& aSizes,
                                        size_t* aNodeSize) const {
  Element::AddSizeOfExcludingThis(aSizes, aNodeSize);

  // These are owned by the element and not referenced from the stylesheets.
  // They're referenced from the rule tree, but the rule nodes don't measure
  // their style source (since they're non-owning), so unconditionally reporting
  // them even though it's a refcounted object is ok.
  if (mContentDeclarationBlock) {
    aSizes.mLayoutSvgMappedDeclarations +=
        mContentDeclarationBlock->SizeofIncludingThis(
            aSizes.mState.mMallocSizeOf);
  }
}

}  // namespace dom
}  // namespace mozilla
