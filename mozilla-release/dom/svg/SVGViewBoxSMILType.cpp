/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SVGViewBoxSMILType.h"

#include "mozilla/SMILValue.h"
#include "nsDebug.h"
#include "SVGAnimatedViewBox.h"
#include <math.h>

namespace mozilla {

/*static*/
SVGViewBoxSMILType SVGViewBoxSMILType::sSingleton;

<<<<<<< HEAD
void SVGViewBoxSMILType::Init(nsSMILValue& aValue) const {
||||||| merged common ancestors
void
SVGViewBoxSMILType::Init(nsSMILValue& aValue) const
{
=======
void SVGViewBoxSMILType::Init(SMILValue& aValue) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(aValue.IsNull(), "Unexpected value type");

  aValue.mU.mPtr = new SVGViewBox();
  aValue.mType = this;
}

<<<<<<< HEAD
void SVGViewBoxSMILType::Destroy(nsSMILValue& aValue) const {
||||||| merged common ancestors
void
SVGViewBoxSMILType::Destroy(nsSMILValue& aValue) const
{
=======
void SVGViewBoxSMILType::Destroy(SMILValue& aValue) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(aValue.mType == this, "Unexpected SMIL value");
  delete static_cast<SVGViewBox*>(aValue.mU.mPtr);
  aValue.mU.mPtr = nullptr;
  aValue.mType = SMILNullType::Singleton();
}

<<<<<<< HEAD
nsresult SVGViewBoxSMILType::Assign(nsSMILValue& aDest,
                                    const nsSMILValue& aSrc) const {
||||||| merged common ancestors
nsresult
SVGViewBoxSMILType::Assign(nsSMILValue& aDest, const nsSMILValue& aSrc) const
{
=======
nsresult SVGViewBoxSMILType::Assign(SMILValue& aDest,
                                    const SMILValue& aSrc) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(aDest.mType == aSrc.mType, "Incompatible SMIL types");
  MOZ_ASSERT(aDest.mType == this, "Unexpected SMIL value");

<<<<<<< HEAD
  const nsSVGViewBoxRect* src =
      static_cast<const nsSVGViewBoxRect*>(aSrc.mU.mPtr);
  nsSVGViewBoxRect* dst = static_cast<nsSVGViewBoxRect*>(aDest.mU.mPtr);
||||||| merged common ancestors
  const nsSVGViewBoxRect* src = static_cast<const nsSVGViewBoxRect*>(aSrc.mU.mPtr);
  nsSVGViewBoxRect* dst = static_cast<nsSVGViewBoxRect*>(aDest.mU.mPtr);
=======
  const SVGViewBox* src = static_cast<const SVGViewBox*>(aSrc.mU.mPtr);
  SVGViewBox* dst = static_cast<SVGViewBox*>(aDest.mU.mPtr);
>>>>>>> upstream-releases
  *dst = *src;
  return NS_OK;
}

<<<<<<< HEAD
bool SVGViewBoxSMILType::IsEqual(const nsSMILValue& aLeft,
                                 const nsSMILValue& aRight) const {
||||||| merged common ancestors
bool
SVGViewBoxSMILType::IsEqual(const nsSMILValue& aLeft,
                            const nsSMILValue& aRight) const
{
=======
bool SVGViewBoxSMILType::IsEqual(const SMILValue& aLeft,
                                 const SMILValue& aRight) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(aLeft.mType == aRight.mType, "Incompatible SMIL types");
  MOZ_ASSERT(aLeft.mType == this, "Unexpected type for SMIL value");

<<<<<<< HEAD
  const nsSVGViewBoxRect* leftBox =
      static_cast<const nsSVGViewBoxRect*>(aLeft.mU.mPtr);
  const nsSVGViewBoxRect* rightBox =
      static_cast<nsSVGViewBoxRect*>(aRight.mU.mPtr);
||||||| merged common ancestors
  const nsSVGViewBoxRect* leftBox =
    static_cast<const nsSVGViewBoxRect*>(aLeft.mU.mPtr);
  const nsSVGViewBoxRect* rightBox =
    static_cast<nsSVGViewBoxRect*>(aRight.mU.mPtr);
=======
  const SVGViewBox* leftBox = static_cast<const SVGViewBox*>(aLeft.mU.mPtr);
  const SVGViewBox* rightBox = static_cast<SVGViewBox*>(aRight.mU.mPtr);
>>>>>>> upstream-releases
  return *leftBox == *rightBox;
}

<<<<<<< HEAD
nsresult SVGViewBoxSMILType::Add(nsSMILValue& aDest,
                                 const nsSMILValue& aValueToAdd,
                                 uint32_t aCount) const {
  MOZ_ASSERT(aValueToAdd.mType == aDest.mType, "Trying to add invalid types");
||||||| merged common ancestors
nsresult
SVGViewBoxSMILType::Add(nsSMILValue& aDest, const nsSMILValue& aValueToAdd,
                        uint32_t aCount) const
{
  MOZ_ASSERT(aValueToAdd.mType == aDest.mType,
                  "Trying to add invalid types");
=======
nsresult SVGViewBoxSMILType::Add(SMILValue& aDest, const SMILValue& aValueToAdd,
                                 uint32_t aCount) const {
  MOZ_ASSERT(aValueToAdd.mType == aDest.mType, "Trying to add invalid types");
>>>>>>> upstream-releases
  MOZ_ASSERT(aValueToAdd.mType == this, "Unexpected source type");

  // See https://bugzilla.mozilla.org/show_bug.cgi?id=541884#c3 and the two
  // comments that follow that one for arguments for and against allowing
  // viewBox to be additive.

  return NS_ERROR_FAILURE;
}

<<<<<<< HEAD
nsresult SVGViewBoxSMILType::ComputeDistance(const nsSMILValue& aFrom,
                                             const nsSMILValue& aTo,
                                             double& aDistance) const {
  MOZ_ASSERT(aFrom.mType == aTo.mType, "Trying to compare different types");
||||||| merged common ancestors
nsresult
SVGViewBoxSMILType::ComputeDistance(const nsSMILValue& aFrom,
                                    const nsSMILValue& aTo,
                                    double& aDistance) const
{
  MOZ_ASSERT(aFrom.mType == aTo.mType,"Trying to compare different types");
=======
nsresult SVGViewBoxSMILType::ComputeDistance(const SMILValue& aFrom,
                                             const SMILValue& aTo,
                                             double& aDistance) const {
  MOZ_ASSERT(aFrom.mType == aTo.mType, "Trying to compare different types");
>>>>>>> upstream-releases
  MOZ_ASSERT(aFrom.mType == this, "Unexpected source type");

<<<<<<< HEAD
  const nsSVGViewBoxRect* from =
      static_cast<const nsSVGViewBoxRect*>(aFrom.mU.mPtr);
  const nsSVGViewBoxRect* to =
      static_cast<const nsSVGViewBoxRect*>(aTo.mU.mPtr);
||||||| merged common ancestors
  const nsSVGViewBoxRect* from = static_cast<const nsSVGViewBoxRect*>(aFrom.mU.mPtr);
  const nsSVGViewBoxRect* to = static_cast<const nsSVGViewBoxRect*>(aTo.mU.mPtr);
=======
  const SVGViewBox* from = static_cast<const SVGViewBox*>(aFrom.mU.mPtr);
  const SVGViewBox* to = static_cast<const SVGViewBox*>(aTo.mU.mPtr);
>>>>>>> upstream-releases

  if (from->none || to->none) {
    return NS_ERROR_FAILURE;
  }

  // We use the distances between the edges rather than the difference between
  // the x, y, width and height for the "distance". This is necessary in
  // order for the "distance" result that we calculate to be the same for a
  // given change in the left side as it is for an equal change in the opposite
  // side. See https://bugzilla.mozilla.org/show_bug.cgi?id=541884#c12

  float dLeft = to->x - from->x;
  float dTop = to->y - from->y;
  float dRight = (to->x + to->width) - (from->x + from->width);
  float dBottom = (to->y + to->height) - (from->y + from->height);

<<<<<<< HEAD
  aDistance =
      sqrt(dLeft * dLeft + dTop * dTop + dRight * dRight + dBottom * dBottom);
||||||| merged common ancestors
  aDistance = sqrt(dLeft*dLeft + dTop*dTop + dRight*dRight + dBottom*dBottom);
=======
  aDistance = std::sqrt(dLeft * dLeft + dTop * dTop + dRight * dRight +
                        dBottom * dBottom);
>>>>>>> upstream-releases

  return NS_OK;
}

<<<<<<< HEAD
nsresult SVGViewBoxSMILType::Interpolate(const nsSMILValue& aStartVal,
                                         const nsSMILValue& aEndVal,
                                         double aUnitDistance,
                                         nsSMILValue& aResult) const {
||||||| merged common ancestors
nsresult
SVGViewBoxSMILType::Interpolate(const nsSMILValue& aStartVal,
                                const nsSMILValue& aEndVal,
                                double aUnitDistance,
                                nsSMILValue& aResult) const
{
=======
nsresult SVGViewBoxSMILType::Interpolate(const SMILValue& aStartVal,
                                         const SMILValue& aEndVal,
                                         double aUnitDistance,
                                         SMILValue& aResult) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(aStartVal.mType == aEndVal.mType,
             "Trying to interpolate different types");
  MOZ_ASSERT(aStartVal.mType == this, "Unexpected types for interpolation");
  MOZ_ASSERT(aResult.mType == this, "Unexpected result type");

<<<<<<< HEAD
  const nsSVGViewBoxRect* start =
      static_cast<const nsSVGViewBoxRect*>(aStartVal.mU.mPtr);
  const nsSVGViewBoxRect* end =
      static_cast<const nsSVGViewBoxRect*>(aEndVal.mU.mPtr);
||||||| merged common ancestors
  const nsSVGViewBoxRect* start = static_cast<const nsSVGViewBoxRect*>(aStartVal.mU.mPtr);
  const nsSVGViewBoxRect* end = static_cast<const nsSVGViewBoxRect*>(aEndVal.mU.mPtr);
=======
  const SVGViewBox* start = static_cast<const SVGViewBox*>(aStartVal.mU.mPtr);
  const SVGViewBox* end = static_cast<const SVGViewBox*>(aEndVal.mU.mPtr);
>>>>>>> upstream-releases

  if (start->none || end->none) {
    return NS_ERROR_FAILURE;
  }

  SVGViewBox* current = static_cast<SVGViewBox*>(aResult.mU.mPtr);

  float x = (start->x + (end->x - start->x) * aUnitDistance);
  float y = (start->y + (end->y - start->y) * aUnitDistance);
  float width = (start->width + (end->width - start->width) * aUnitDistance);
  float height =
      (start->height + (end->height - start->height) * aUnitDistance);

  *current = SVGViewBox(x, y, width, height);

  return NS_OK;
}

}  // namespace mozilla
