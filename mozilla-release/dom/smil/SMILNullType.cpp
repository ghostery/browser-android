/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SMILNullType.h"

#include "mozilla/SMILValue.h"
#include "nsDebug.h"

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILNullType.cpp
/*static*/ nsSMILNullType* nsSMILNullType::Singleton() {
  static nsSMILNullType sSingleton;
||||||| merged common ancestors
/*static*/ nsSMILNullType*
nsSMILNullType::Singleton()
{
  static nsSMILNullType sSingleton;
=======
namespace mozilla {

/*static*/
SMILNullType* SMILNullType::Singleton() {
  static SMILNullType sSingleton;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILNullType.cpp
  return &sSingleton;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILNullType.cpp
nsresult nsSMILNullType::Assign(nsSMILValue& aDest,
                                const nsSMILValue& aSrc) const {
||||||| merged common ancestors
nsresult
nsSMILNullType::Assign(nsSMILValue& aDest, const nsSMILValue& aSrc) const
{
=======
nsresult SMILNullType::Assign(SMILValue& aDest, const SMILValue& aSrc) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILNullType.cpp
  MOZ_ASSERT(aDest.mType == aSrc.mType, "Incompatible SMIL types");
  MOZ_ASSERT(aSrc.mType == this, "Unexpected source type");
  aDest.mU = aSrc.mU;
  aDest.mType = Singleton();
  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILNullType.cpp
bool nsSMILNullType::IsEqual(const nsSMILValue& aLeft,
                             const nsSMILValue& aRight) const {
||||||| merged common ancestors
bool
nsSMILNullType::IsEqual(const nsSMILValue& aLeft,
                        const nsSMILValue& aRight) const
{
=======
bool SMILNullType::IsEqual(const SMILValue& aLeft,
                           const SMILValue& aRight) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILNullType.cpp
  MOZ_ASSERT(aLeft.mType == aRight.mType, "Incompatible SMIL types");
  MOZ_ASSERT(aLeft.mType == this, "Unexpected type for SMIL value");

  return true;  // All null-typed values are equivalent.
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILNullType.cpp
nsresult nsSMILNullType::Add(nsSMILValue& aDest, const nsSMILValue& aValueToAdd,
                             uint32_t aCount) const {
||||||| merged common ancestors
nsresult
nsSMILNullType::Add(nsSMILValue& aDest, const nsSMILValue& aValueToAdd,
                    uint32_t aCount) const
{
=======
nsresult SMILNullType::Add(SMILValue& aDest, const SMILValue& aValueToAdd,
                           uint32_t aCount) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILNullType.cpp
  MOZ_ASSERT_UNREACHABLE("Adding NULL type");
  return NS_ERROR_FAILURE;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILNullType.cpp
nsresult nsSMILNullType::ComputeDistance(const nsSMILValue& aFrom,
                                         const nsSMILValue& aTo,
                                         double& aDistance) const {
||||||| merged common ancestors
nsresult
nsSMILNullType::ComputeDistance(const nsSMILValue& aFrom,
                                const nsSMILValue& aTo,
                                double& aDistance) const
{
=======
nsresult SMILNullType::ComputeDistance(const SMILValue& aFrom,
                                       const SMILValue& aTo,
                                       double& aDistance) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILNullType.cpp
  MOZ_ASSERT_UNREACHABLE("Computing distance for NULL type");
  return NS_ERROR_FAILURE;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILNullType.cpp
nsresult nsSMILNullType::Interpolate(const nsSMILValue& aStartVal,
                                     const nsSMILValue& aEndVal,
                                     double aUnitDistance,
                                     nsSMILValue& aResult) const {
||||||| merged common ancestors
nsresult
nsSMILNullType::Interpolate(const nsSMILValue& aStartVal,
                            const nsSMILValue& aEndVal,
                            double aUnitDistance,
                            nsSMILValue& aResult) const
{
=======
nsresult SMILNullType::Interpolate(const SMILValue& aStartVal,
                                   const SMILValue& aEndVal,
                                   double aUnitDistance,
                                   SMILValue& aResult) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILNullType.cpp
  MOZ_ASSERT_UNREACHABLE("Interpolating NULL type");
  return NS_ERROR_FAILURE;
}

}  // namespace mozilla
