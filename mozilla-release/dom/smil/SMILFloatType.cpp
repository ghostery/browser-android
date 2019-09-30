/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SMILFloatType.h"

#include "mozilla/SMILValue.h"
#include "nsDebug.h"
#include <math.h>

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILFloatType.cpp
void nsSMILFloatType::Init(nsSMILValue& aValue) const {
||||||| merged common ancestors
void
nsSMILFloatType::Init(nsSMILValue& aValue) const
{
=======
namespace mozilla {

void SMILFloatType::Init(SMILValue& aValue) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILFloatType.cpp
  MOZ_ASSERT(aValue.IsNull(), "Unexpected value type");
  aValue.mU.mDouble = 0.0;
  aValue.mType = this;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILFloatType.cpp
void nsSMILFloatType::Destroy(nsSMILValue& aValue) const {
||||||| merged common ancestors
void
nsSMILFloatType::Destroy(nsSMILValue& aValue) const
{
=======
void SMILFloatType::Destroy(SMILValue& aValue) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILFloatType.cpp
  MOZ_ASSERT(aValue.mType == this, "Unexpected SMIL value");
  aValue.mU.mDouble = 0.0;
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILFloatType.cpp
  aValue.mType = nsSMILNullType::Singleton();
||||||| merged common ancestors
  aValue.mType      = nsSMILNullType::Singleton();
=======
  aValue.mType = SMILNullType::Singleton();
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILFloatType.cpp
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILFloatType.cpp
nsresult nsSMILFloatType::Assign(nsSMILValue& aDest,
                                 const nsSMILValue& aSrc) const {
||||||| merged common ancestors
nsresult
nsSMILFloatType::Assign(nsSMILValue& aDest, const nsSMILValue& aSrc) const
{
=======
nsresult SMILFloatType::Assign(SMILValue& aDest, const SMILValue& aSrc) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILFloatType.cpp
  MOZ_ASSERT(aDest.mType == aSrc.mType, "Incompatible SMIL types");
  MOZ_ASSERT(aDest.mType == this, "Unexpected SMIL value");
  aDest.mU.mDouble = aSrc.mU.mDouble;
  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILFloatType.cpp
bool nsSMILFloatType::IsEqual(const nsSMILValue& aLeft,
                              const nsSMILValue& aRight) const {
||||||| merged common ancestors
bool
nsSMILFloatType::IsEqual(const nsSMILValue& aLeft,
                         const nsSMILValue& aRight) const
{
=======
bool SMILFloatType::IsEqual(const SMILValue& aLeft,
                            const SMILValue& aRight) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILFloatType.cpp
  MOZ_ASSERT(aLeft.mType == aRight.mType, "Incompatible SMIL types");
  MOZ_ASSERT(aLeft.mType == this, "Unexpected type for SMIL value");

  return aLeft.mU.mDouble == aRight.mU.mDouble;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILFloatType.cpp
nsresult nsSMILFloatType::Add(nsSMILValue& aDest,
                              const nsSMILValue& aValueToAdd,
                              uint32_t aCount) const {
  MOZ_ASSERT(aValueToAdd.mType == aDest.mType, "Trying to add invalid types");
||||||| merged common ancestors
nsresult
nsSMILFloatType::Add(nsSMILValue& aDest, const nsSMILValue& aValueToAdd,
                     uint32_t aCount) const
{
  MOZ_ASSERT(aValueToAdd.mType == aDest.mType,
                  "Trying to add invalid types");
=======
nsresult SMILFloatType::Add(SMILValue& aDest, const SMILValue& aValueToAdd,
                            uint32_t aCount) const {
  MOZ_ASSERT(aValueToAdd.mType == aDest.mType, "Trying to add invalid types");
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILFloatType.cpp
  MOZ_ASSERT(aValueToAdd.mType == this, "Unexpected source type");
  aDest.mU.mDouble += aValueToAdd.mU.mDouble * aCount;
  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILFloatType.cpp
nsresult nsSMILFloatType::ComputeDistance(const nsSMILValue& aFrom,
                                          const nsSMILValue& aTo,
                                          double& aDistance) const {
||||||| merged common ancestors
nsresult
nsSMILFloatType::ComputeDistance(const nsSMILValue& aFrom,
                                 const nsSMILValue& aTo,
                                 double& aDistance) const
{
=======
nsresult SMILFloatType::ComputeDistance(const SMILValue& aFrom,
                                        const SMILValue& aTo,
                                        double& aDistance) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILFloatType.cpp
  MOZ_ASSERT(aFrom.mType == aTo.mType, "Trying to compare different types");
  MOZ_ASSERT(aFrom.mType == this, "Unexpected source type");

  const double& from = aFrom.mU.mDouble;
  const double& to = aTo.mU.mDouble;

  aDistance = fabs(to - from);

  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILFloatType.cpp
nsresult nsSMILFloatType::Interpolate(const nsSMILValue& aStartVal,
                                      const nsSMILValue& aEndVal,
                                      double aUnitDistance,
                                      nsSMILValue& aResult) const {
||||||| merged common ancestors
nsresult
nsSMILFloatType::Interpolate(const nsSMILValue& aStartVal,
                             const nsSMILValue& aEndVal,
                             double aUnitDistance,
                             nsSMILValue& aResult) const
{
=======
nsresult SMILFloatType::Interpolate(const SMILValue& aStartVal,
                                    const SMILValue& aEndVal,
                                    double aUnitDistance,
                                    SMILValue& aResult) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILFloatType.cpp
  MOZ_ASSERT(aStartVal.mType == aEndVal.mType,
             "Trying to interpolate different types");
  MOZ_ASSERT(aStartVal.mType == this, "Unexpected types for interpolation");
  MOZ_ASSERT(aResult.mType == this, "Unexpected result type");

  const double& startVal = aStartVal.mU.mDouble;
  const double& endVal = aEndVal.mU.mDouble;

  aResult.mU.mDouble = (startVal + (endVal - startVal) * aUnitDistance);

  return NS_OK;
}

}  // namespace mozilla
