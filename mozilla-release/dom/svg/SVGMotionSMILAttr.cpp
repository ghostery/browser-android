/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* representation of a dummy attribute targeted by <animateMotion> element */

#include "SVGMotionSMILAttr.h"

#include "mozilla/dom/SVGAnimationElement.h"
#include "mozilla/dom/SVGElement.h"
#include "mozilla/SMILValue.h"
#include "SVGMotionSMILType.h"
#include "nsDebug.h"
#include "gfx2DGlue.h"

namespace mozilla {

<<<<<<< HEAD
nsresult SVGMotionSMILAttr::ValueFromString(
    const nsAString& aStr, const dom::SVGAnimationElement* aSrcElement,
    nsSMILValue& aValue, bool& aPreventCachingOfSandwich) const {
  MOZ_ASSERT_UNREACHABLE(
      "Shouldn't using nsISMILAttr::ValueFromString for "
      "parsing animateMotion's SMIL values.");
||||||| merged common ancestors
nsresult
SVGMotionSMILAttr::ValueFromString(const nsAString& aStr,
                                   const dom::SVGAnimationElement* aSrcElement,
                                   nsSMILValue& aValue,
                                   bool& aPreventCachingOfSandwich) const
{
  MOZ_ASSERT_UNREACHABLE("Shouldn't using nsISMILAttr::ValueFromString for "
                         "parsing animateMotion's SMIL values.");
=======
nsresult SVGMotionSMILAttr::ValueFromString(
    const nsAString& aStr, const dom::SVGAnimationElement* aSrcElement,
    SMILValue& aValue, bool& aPreventCachingOfSandwich) const {
  MOZ_ASSERT_UNREACHABLE(
      "Shouldn't using SMILAttr::ValueFromString for "
      "parsing animateMotion's SMIL values.");
>>>>>>> upstream-releases
  return NS_ERROR_FAILURE;
}

<<<<<<< HEAD
nsSMILValue SVGMotionSMILAttr::GetBaseValue() const {
  return nsSMILValue(&SVGMotionSMILType::sSingleton);
||||||| merged common ancestors
nsSMILValue
SVGMotionSMILAttr::GetBaseValue() const
{
  return nsSMILValue(&SVGMotionSMILType::sSingleton);
=======
SMILValue SVGMotionSMILAttr::GetBaseValue() const {
  return SMILValue(&SVGMotionSMILType::sSingleton);
>>>>>>> upstream-releases
}

void SVGMotionSMILAttr::ClearAnimValue() {
  mSVGElement->SetAnimateMotionTransform(nullptr);
}

<<<<<<< HEAD
nsresult SVGMotionSMILAttr::SetAnimValue(const nsSMILValue& aValue) {
||||||| merged common ancestors
nsresult
SVGMotionSMILAttr::SetAnimValue(const nsSMILValue& aValue)
{
=======
nsresult SVGMotionSMILAttr::SetAnimValue(const SMILValue& aValue) {
>>>>>>> upstream-releases
  gfx::Matrix matrix = SVGMotionSMILType::CreateMatrix(aValue);
  mSVGElement->SetAnimateMotionTransform(&matrix);
  return NS_OK;
}

const nsIContent* SVGMotionSMILAttr::GetTargetNode() const {
  return mSVGElement;
}

}  // namespace mozilla
