/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SMILValue.h"

#include "nsDebug.h"
#include <string.h>

namespace mozilla {

//----------------------------------------------------------------------
// Public methods

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILValue.cpp
nsSMILValue::nsSMILValue(const nsISMILType* aType)
    : mType(nsSMILNullType::Singleton()) {
||||||| merged common ancestors
nsSMILValue::nsSMILValue(const nsISMILType* aType)
  : mType(nsSMILNullType::Singleton())
{
=======
SMILValue::SMILValue(const SMILType* aType) : mType(SMILNullType::Singleton()) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILValue.cpp
  mU.mBool = false;
  if (!aType) {
    NS_ERROR("Trying to construct SMILValue with null mType pointer");
    return;
  }

  InitAndCheckPostcondition(aType);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILValue.cpp
nsSMILValue::nsSMILValue(const nsSMILValue& aVal)
    : mType(nsSMILNullType::Singleton()) {
||||||| merged common ancestors
nsSMILValue::nsSMILValue(const nsSMILValue& aVal)
  : mType(nsSMILNullType::Singleton())
{
=======
SMILValue::SMILValue(const SMILValue& aVal) : mType(SMILNullType::Singleton()) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILValue.cpp
  InitAndCheckPostcondition(aVal.mType);
  mType->Assign(*this, aVal);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILValue.cpp
const nsSMILValue& nsSMILValue::operator=(const nsSMILValue& aVal) {
  if (&aVal == this) return *this;
||||||| merged common ancestors
const nsSMILValue&
nsSMILValue::operator=(const nsSMILValue& aVal)
{
  if (&aVal == this)
    return *this;
=======
const SMILValue& SMILValue::operator=(const SMILValue& aVal) {
  if (&aVal == this) return *this;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILValue.cpp

  if (mType != aVal.mType) {
    DestroyAndReinit(aVal.mType);
  }

  mType->Assign(*this, aVal);

  return *this;
}

// Move constructor / reassignment operator:
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILValue.cpp
nsSMILValue::nsSMILValue(nsSMILValue&& aVal)
    : mU(aVal.mU),  // Copying union is only OK because we clear aVal.mType
                    // below.
      mType(aVal.mType) {
||||||| merged common ancestors
nsSMILValue::nsSMILValue(nsSMILValue&& aVal)
  : mU(aVal.mU), // Copying union is only OK because we clear aVal.mType below.
    mType(aVal.mType)
{
=======
SMILValue::SMILValue(SMILValue&& aVal) noexcept
    : mU(aVal.mU),  // Copying union is only OK because we clear aVal.mType
                    // below.
      mType(aVal.mType) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILValue.cpp
  // Leave aVal with a null type, so that it's safely destructible (and won't
  // mess with anything referenced by its union, which we've copied).
  aVal.mType = SMILNullType::Singleton();
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILValue.cpp
nsSMILValue& nsSMILValue::operator=(nsSMILValue&& aVal) {
||||||| merged common ancestors
nsSMILValue&
nsSMILValue::operator=(nsSMILValue&& aVal)
{
=======
SMILValue& SMILValue::operator=(SMILValue&& aVal) noexcept {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILValue.cpp
  if (!IsNull()) {
    // Clean up any data we're currently tracking.
    DestroyAndCheckPostcondition();
  }

  // Copy the union (which could include a pointer to external memory) & mType:
  mU = aVal.mU;
  mType = aVal.mType;

  // Leave aVal with a null type, so that it's safely destructible (and won't
  // mess with anything referenced by its union, which we've now copied).
  aVal.mType = SMILNullType::Singleton();

  return *this;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILValue.cpp
bool nsSMILValue::operator==(const nsSMILValue& aVal) const {
  if (&aVal == this) return true;
||||||| merged common ancestors
bool
nsSMILValue::operator==(const nsSMILValue& aVal) const
{
  if (&aVal == this)
    return true;
=======
bool SMILValue::operator==(const SMILValue& aVal) const {
  if (&aVal == this) return true;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILValue.cpp

  return mType == aVal.mType && mType->IsEqual(*this, aVal);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILValue.cpp
nsresult nsSMILValue::Add(const nsSMILValue& aValueToAdd, uint32_t aCount) {
||||||| merged common ancestors
nsresult
nsSMILValue::Add(const nsSMILValue& aValueToAdd, uint32_t aCount)
{
=======
nsresult SMILValue::Add(const SMILValue& aValueToAdd, uint32_t aCount) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILValue.cpp
  if (aValueToAdd.mType != mType) {
    NS_ERROR("Trying to add incompatible types");
    return NS_ERROR_FAILURE;
  }

  return mType->Add(*this, aValueToAdd, aCount);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILValue.cpp
nsresult nsSMILValue::SandwichAdd(const nsSMILValue& aValueToAdd) {
||||||| merged common ancestors
nsresult
nsSMILValue::SandwichAdd(const nsSMILValue& aValueToAdd)
{
=======
nsresult SMILValue::SandwichAdd(const SMILValue& aValueToAdd) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILValue.cpp
  if (aValueToAdd.mType != mType) {
    NS_ERROR("Trying to add incompatible types");
    return NS_ERROR_FAILURE;
  }

  return mType->SandwichAdd(*this, aValueToAdd);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILValue.cpp
nsresult nsSMILValue::ComputeDistance(const nsSMILValue& aTo,
                                      double& aDistance) const {
||||||| merged common ancestors
nsresult
nsSMILValue::ComputeDistance(const nsSMILValue& aTo, double& aDistance) const
{
=======
nsresult SMILValue::ComputeDistance(const SMILValue& aTo,
                                    double& aDistance) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILValue.cpp
  if (aTo.mType != mType) {
    NS_ERROR("Trying to calculate distance between incompatible types");
    return NS_ERROR_FAILURE;
  }

  return mType->ComputeDistance(*this, aTo, aDistance);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILValue.cpp
nsresult nsSMILValue::Interpolate(const nsSMILValue& aEndVal,
                                  double aUnitDistance,
                                  nsSMILValue& aResult) const {
||||||| merged common ancestors
nsresult
nsSMILValue::Interpolate(const nsSMILValue& aEndVal,
                         double aUnitDistance,
                         nsSMILValue& aResult) const
{
=======
nsresult SMILValue::Interpolate(const SMILValue& aEndVal, double aUnitDistance,
                                SMILValue& aResult) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILValue.cpp
  if (aEndVal.mType != mType) {
    NS_ERROR("Trying to interpolate between incompatible types");
    return NS_ERROR_FAILURE;
  }

  if (aResult.mType != mType) {
    // Outparam has wrong type
    aResult.DestroyAndReinit(mType);
  }

  return mType->Interpolate(*this, aEndVal, aUnitDistance, aResult);
}

//----------------------------------------------------------------------
// Helper methods

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILValue.cpp
// Wrappers for nsISMILType::Init & ::Destroy that verify their postconditions
void nsSMILValue::InitAndCheckPostcondition(const nsISMILType* aNewType) {
||||||| merged common ancestors
// Wrappers for nsISMILType::Init & ::Destroy that verify their postconditions
void
nsSMILValue::InitAndCheckPostcondition(const nsISMILType* aNewType)
{
=======
// Wrappers for SMILType::Init & ::Destroy that verify their postconditions
void SMILValue::InitAndCheckPostcondition(const SMILType* aNewType) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILValue.cpp
  aNewType->Init(*this);
  MOZ_ASSERT(mType == aNewType,
             "Post-condition of Init failed. SMILValue is invalid");
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILValue.cpp
void nsSMILValue::DestroyAndCheckPostcondition() {
||||||| merged common ancestors
void
nsSMILValue::DestroyAndCheckPostcondition()
{
=======
void SMILValue::DestroyAndCheckPostcondition() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILValue.cpp
  mType->Destroy(*this);
  MOZ_ASSERT(IsNull(),
             "Post-condition of Destroy failed. "
             "SMILValue not null after destroying");
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILValue.cpp
void nsSMILValue::DestroyAndReinit(const nsISMILType* aNewType) {
||||||| merged common ancestors
void
nsSMILValue::DestroyAndReinit(const nsISMILType* aNewType)
{
=======
void SMILValue::DestroyAndReinit(const SMILType* aNewType) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILValue.cpp
  DestroyAndCheckPostcondition();
  InitAndCheckPostcondition(aNewType);
}

}  // namespace mozilla
