/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SVGIntegerPairSMILType.h"

#include "mozilla/SMILValue.h"
#include "nsMathUtils.h"
#include "nsDebug.h"

namespace mozilla {

<<<<<<< HEAD
void SVGIntegerPairSMILType::Init(nsSMILValue& aValue) const {
||||||| merged common ancestors
void
SVGIntegerPairSMILType::Init(nsSMILValue& aValue) const
{
=======
void SVGIntegerPairSMILType::Init(SMILValue& aValue) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(aValue.IsNull(), "Unexpected value type");

  aValue.mU.mIntPair[0] = 0;
  aValue.mU.mIntPair[1] = 0;
  aValue.mType = this;
}

<<<<<<< HEAD
void SVGIntegerPairSMILType::Destroy(nsSMILValue& aValue) const {
||||||| merged common ancestors
void
SVGIntegerPairSMILType::Destroy(nsSMILValue& aValue) const
{
=======
void SVGIntegerPairSMILType::Destroy(SMILValue& aValue) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(aValue.mType == this, "Unexpected SMIL value");
  aValue.mU.mIntPair[0] = 0;
  aValue.mU.mIntPair[1] = 0;
  aValue.mType = SMILNullType::Singleton();
}

<<<<<<< HEAD
nsresult SVGIntegerPairSMILType::Assign(nsSMILValue& aDest,
                                        const nsSMILValue& aSrc) const {
||||||| merged common ancestors
nsresult
SVGIntegerPairSMILType::Assign(nsSMILValue& aDest, const nsSMILValue& aSrc) const
{
=======
nsresult SVGIntegerPairSMILType::Assign(SMILValue& aDest,
                                        const SMILValue& aSrc) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(aDest.mType == aSrc.mType, "Incompatible SMIL types");
  MOZ_ASSERT(aDest.mType == this, "Unexpected SMIL value");

  aDest.mU.mIntPair[0] = aSrc.mU.mIntPair[0];
  aDest.mU.mIntPair[1] = aSrc.mU.mIntPair[1];
  return NS_OK;
}

<<<<<<< HEAD
bool SVGIntegerPairSMILType::IsEqual(const nsSMILValue& aLeft,
                                     const nsSMILValue& aRight) const {
||||||| merged common ancestors
bool
SVGIntegerPairSMILType::IsEqual(const nsSMILValue& aLeft,
                                const nsSMILValue& aRight) const
{
=======
bool SVGIntegerPairSMILType::IsEqual(const SMILValue& aLeft,
                                     const SMILValue& aRight) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(aLeft.mType == aRight.mType, "Incompatible SMIL types");
  MOZ_ASSERT(aLeft.mType == this, "Unexpected type for SMIL value");

  return aLeft.mU.mIntPair[0] == aRight.mU.mIntPair[0] &&
         aLeft.mU.mIntPair[1] == aRight.mU.mIntPair[1];
}

<<<<<<< HEAD
nsresult SVGIntegerPairSMILType::Add(nsSMILValue& aDest,
                                     const nsSMILValue& aValueToAdd,
                                     uint32_t aCount) const {
  MOZ_ASSERT(aValueToAdd.mType == aDest.mType, "Trying to add invalid types");
||||||| merged common ancestors
nsresult
SVGIntegerPairSMILType::Add(nsSMILValue& aDest, const nsSMILValue& aValueToAdd,
                            uint32_t aCount) const
{
  MOZ_ASSERT(aValueToAdd.mType == aDest.mType,
                  "Trying to add invalid types");
=======
nsresult SVGIntegerPairSMILType::Add(SMILValue& aDest,
                                     const SMILValue& aValueToAdd,
                                     uint32_t aCount) const {
  MOZ_ASSERT(aValueToAdd.mType == aDest.mType, "Trying to add invalid types");
>>>>>>> upstream-releases
  MOZ_ASSERT(aValueToAdd.mType == this, "Unexpected source type");

  aDest.mU.mIntPair[0] += aValueToAdd.mU.mIntPair[0] * aCount;
  aDest.mU.mIntPair[1] += aValueToAdd.mU.mIntPair[1] * aCount;

  return NS_OK;
}

<<<<<<< HEAD
nsresult SVGIntegerPairSMILType::ComputeDistance(const nsSMILValue& aFrom,
                                                 const nsSMILValue& aTo,
                                                 double& aDistance) const {
  MOZ_ASSERT(aFrom.mType == aTo.mType, "Trying to compare different types");
||||||| merged common ancestors
nsresult
SVGIntegerPairSMILType::ComputeDistance(const nsSMILValue& aFrom,
                                        const nsSMILValue& aTo,
                                        double& aDistance) const
{
  MOZ_ASSERT(aFrom.mType == aTo.mType,"Trying to compare different types");
=======
nsresult SVGIntegerPairSMILType::ComputeDistance(const SMILValue& aFrom,
                                                 const SMILValue& aTo,
                                                 double& aDistance) const {
  MOZ_ASSERT(aFrom.mType == aTo.mType, "Trying to compare different types");
>>>>>>> upstream-releases
  MOZ_ASSERT(aFrom.mType == this, "Unexpected source type");

  double delta[2];
  delta[0] = aTo.mU.mIntPair[0] - aFrom.mU.mIntPair[0];
  delta[1] = aTo.mU.mIntPair[1] - aFrom.mU.mIntPair[1];

  aDistance = NS_hypot(delta[0], delta[1]);
  return NS_OK;
}

<<<<<<< HEAD
nsresult SVGIntegerPairSMILType::Interpolate(const nsSMILValue& aStartVal,
                                             const nsSMILValue& aEndVal,
                                             double aUnitDistance,
                                             nsSMILValue& aResult) const {
||||||| merged common ancestors
nsresult
SVGIntegerPairSMILType::Interpolate(const nsSMILValue& aStartVal,
                                    const nsSMILValue& aEndVal,
                                    double aUnitDistance,
                                    nsSMILValue& aResult) const
{
=======
nsresult SVGIntegerPairSMILType::Interpolate(const SMILValue& aStartVal,
                                             const SMILValue& aEndVal,
                                             double aUnitDistance,
                                             SMILValue& aResult) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(aStartVal.mType == aEndVal.mType,
             "Trying to interpolate different types");
  MOZ_ASSERT(aStartVal.mType == this, "Unexpected types for interpolation");
  MOZ_ASSERT(aResult.mType == this, "Unexpected result type");

  double currentVal[2];
  currentVal[0] =
      aStartVal.mU.mIntPair[0] +
      (aEndVal.mU.mIntPair[0] - aStartVal.mU.mIntPair[0]) * aUnitDistance;
  currentVal[1] =
      aStartVal.mU.mIntPair[1] +
      (aEndVal.mU.mIntPair[1] - aStartVal.mU.mIntPair[1]) * aUnitDistance;

  aResult.mU.mIntPair[0] = NS_lround(currentVal[0]);
  aResult.mU.mIntPair[1] = NS_lround(currentVal[1]);
  return NS_OK;
}

}  // namespace mozilla
