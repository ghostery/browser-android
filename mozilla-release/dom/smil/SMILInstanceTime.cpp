/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SMILInstanceTime.h"

#include "mozilla/AutoRestore.h"
#include "mozilla/SMILInterval.h"
#include "mozilla/SMILTimeValueSpec.h"

namespace mozilla {

//----------------------------------------------------------------------
// Implementation

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
nsSMILInstanceTime::nsSMILInstanceTime(const nsSMILTimeValue& aTime,
                                       nsSMILInstanceTimeSource aSource,
                                       nsSMILTimeValueSpec* aCreator,
                                       nsSMILInterval* aBaseInterval)
    : mTime(aTime),
      mFlags(0),
      mVisited(false),
      mFixedEndpointRefCnt(0),
      mSerial(0),
      mCreator(aCreator),
      mBaseInterval(nullptr)  // This will get set to aBaseInterval in a call to
                              // SetBaseInterval() at end of constructor
||||||| merged common ancestors
nsSMILInstanceTime::nsSMILInstanceTime(const nsSMILTimeValue& aTime,
                                       nsSMILInstanceTimeSource aSource,
                                       nsSMILTimeValueSpec* aCreator,
                                       nsSMILInterval* aBaseInterval)
  : mTime(aTime),
    mFlags(0),
    mVisited(false),
    mFixedEndpointRefCnt(0),
    mSerial(0),
    mCreator(aCreator),
    mBaseInterval(nullptr) // This will get set to aBaseInterval in a call to
                          // SetBaseInterval() at end of constructor
=======
SMILInstanceTime::SMILInstanceTime(const SMILTimeValue& aTime,
                                   SMILInstanceTimeSource aSource,
                                   SMILTimeValueSpec* aCreator,
                                   SMILInterval* aBaseInterval)
    : mTime(aTime),
      mFlags(0),
      mVisited(false),
      mFixedEndpointRefCnt(0),
      mSerial(0),
      mCreator(aCreator),
      mBaseInterval(nullptr)  // This will get set to aBaseInterval in a call to
                              // SetBaseInterval() at end of constructor
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp
{
  switch (aSource) {
    case SOURCE_NONE:
      // No special flags
      break;

    case SOURCE_DOM:
      mFlags = kDynamic | kFromDOM;
      break;

    case SOURCE_SYNCBASE:
      mFlags = kMayUpdate;
      break;

    case SOURCE_EVENT:
      mFlags = kDynamic;
      break;
  }

  SetBaseInterval(aBaseInterval);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
nsSMILInstanceTime::~nsSMILInstanceTime() {
||||||| merged common ancestors
nsSMILInstanceTime::~nsSMILInstanceTime()
{
=======
SMILInstanceTime::~SMILInstanceTime() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp
  MOZ_ASSERT(!mBaseInterval,
             "Destroying instance time without first calling Unlink()");
  MOZ_ASSERT(mFixedEndpointRefCnt == 0,
             "Destroying instance time that is still used as the fixed "
             "endpoint of an interval");
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
void nsSMILInstanceTime::Unlink() {
  RefPtr<nsSMILInstanceTime> deathGrip(this);
||||||| merged common ancestors
void
nsSMILInstanceTime::Unlink()
{
  RefPtr<nsSMILInstanceTime> deathGrip(this);
=======
void SMILInstanceTime::Unlink() {
  RefPtr<SMILInstanceTime> deathGrip(this);
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp
  if (mBaseInterval) {
    mBaseInterval->RemoveDependentTime(*this);
    mBaseInterval = nullptr;
  }
  mCreator = nullptr;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
void nsSMILInstanceTime::HandleChangedInterval(
    const nsSMILTimeContainer* aSrcContainer, bool aBeginObjectChanged,
    bool aEndObjectChanged) {
||||||| merged common ancestors
void
nsSMILInstanceTime::HandleChangedInterval(
    const nsSMILTimeContainer* aSrcContainer,
    bool aBeginObjectChanged,
    bool aEndObjectChanged)
{
=======
void SMILInstanceTime::HandleChangedInterval(
    const SMILTimeContainer* aSrcContainer, bool aBeginObjectChanged,
    bool aEndObjectChanged) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp
  // It's possible a sequence of notifications might cause our base interval to
  // be updated and then deleted. Furthermore, the delete might happen whilst
  // we're still in the queue to be notified of the change. In any case, if we
  // don't have a base interval, just ignore the change.
  if (!mBaseInterval) return;

  MOZ_ASSERT(mCreator, "Base interval is set but creator is not.");

  if (mVisited) {
    // Break the cycle here
    Unlink();
    return;
  }

  bool objectChanged =
      mCreator->DependsOnBegin() ? aBeginObjectChanged : aEndObjectChanged;

  RefPtr<SMILInstanceTime> deathGrip(this);
  mozilla::AutoRestore<bool> setVisited(mVisited);
  mVisited = true;

  mCreator->HandleChangedInstanceTime(*GetBaseTime(), aSrcContainer, *this,
                                      objectChanged);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
void nsSMILInstanceTime::HandleDeletedInterval() {
||||||| merged common ancestors
void
nsSMILInstanceTime::HandleDeletedInterval()
{
=======
void SMILInstanceTime::HandleDeletedInterval() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp
  MOZ_ASSERT(mBaseInterval,
             "Got call to HandleDeletedInterval on an independent instance "
             "time");
  MOZ_ASSERT(mCreator, "Base interval is set but creator is not");

  mBaseInterval = nullptr;
  mFlags &= ~kMayUpdate;  // Can't update without a base interval

  RefPtr<SMILInstanceTime> deathGrip(this);
  mCreator->HandleDeletedInstanceTime(*this);
  mCreator = nullptr;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
void nsSMILInstanceTime::HandleFilteredInterval() {
||||||| merged common ancestors
void
nsSMILInstanceTime::HandleFilteredInterval()
{
=======
void SMILInstanceTime::HandleFilteredInterval() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp
  MOZ_ASSERT(mBaseInterval,
             "Got call to HandleFilteredInterval on an independent instance "
             "time");

  mBaseInterval = nullptr;
  mFlags &= ~kMayUpdate;  // Can't update without a base interval
  mCreator = nullptr;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
bool nsSMILInstanceTime::ShouldPreserve() const {
||||||| merged common ancestors
bool
nsSMILInstanceTime::ShouldPreserve() const
{
=======
bool SMILInstanceTime::ShouldPreserve() const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp
  return mFixedEndpointRefCnt > 0 || (mFlags & kWasDynamicEndpoint);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
void nsSMILInstanceTime::UnmarkShouldPreserve() {
||||||| merged common ancestors
void
nsSMILInstanceTime::UnmarkShouldPreserve()
{
=======
void SMILInstanceTime::UnmarkShouldPreserve() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp
  mFlags &= ~kWasDynamicEndpoint;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
void nsSMILInstanceTime::AddRefFixedEndpoint() {
||||||| merged common ancestors
void
nsSMILInstanceTime::AddRefFixedEndpoint()
{
=======
void SMILInstanceTime::AddRefFixedEndpoint() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp
  MOZ_ASSERT(mFixedEndpointRefCnt < UINT16_MAX,
             "Fixed endpoint reference count upper limit reached");
  ++mFixedEndpointRefCnt;
  mFlags &= ~kMayUpdate;  // Once fixed, always fixed
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
void nsSMILInstanceTime::ReleaseFixedEndpoint() {
||||||| merged common ancestors
void
nsSMILInstanceTime::ReleaseFixedEndpoint()
{
=======
void SMILInstanceTime::ReleaseFixedEndpoint() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp
  MOZ_ASSERT(mFixedEndpointRefCnt > 0, "Duplicate release");
  --mFixedEndpointRefCnt;
  if (mFixedEndpointRefCnt == 0 && IsDynamic()) {
    mFlags |= kWasDynamicEndpoint;
  }
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
bool nsSMILInstanceTime::IsDependentOn(const nsSMILInstanceTime& aOther) const {
  if (mVisited) return false;
||||||| merged common ancestors
bool
nsSMILInstanceTime::IsDependentOn(const nsSMILInstanceTime& aOther) const
{
  if (mVisited)
    return false;
=======
bool SMILInstanceTime::IsDependentOn(const SMILInstanceTime& aOther) const {
  if (mVisited) return false;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
  const nsSMILInstanceTime* myBaseTime = GetBaseTime();
  if (!myBaseTime) return false;
||||||| merged common ancestors
  const nsSMILInstanceTime* myBaseTime = GetBaseTime();
  if (!myBaseTime)
    return false;
=======
  const SMILInstanceTime* myBaseTime = GetBaseTime();
  if (!myBaseTime) return false;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp

  if (myBaseTime == &aOther) return true;

  mozilla::AutoRestore<bool> setVisited(mVisited);
  mVisited = true;
  return myBaseTime->IsDependentOn(aOther);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
const nsSMILInstanceTime* nsSMILInstanceTime::GetBaseTime() const {
||||||| merged common ancestors
const nsSMILInstanceTime*
nsSMILInstanceTime::GetBaseTime() const
{
=======
const SMILInstanceTime* SMILInstanceTime::GetBaseTime() const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp
  if (!mBaseInterval) {
    return nullptr;
  }

  MOZ_ASSERT(mCreator, "Base interval is set but there is no creator.");
  if (!mCreator) {
    return nullptr;
  }

  return mCreator->DependsOnBegin() ? mBaseInterval->Begin()
                                    : mBaseInterval->End();
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
void nsSMILInstanceTime::SetBaseInterval(nsSMILInterval* aBaseInterval) {
||||||| merged common ancestors
void
nsSMILInstanceTime::SetBaseInterval(nsSMILInterval* aBaseInterval)
{
=======
void SMILInstanceTime::SetBaseInterval(SMILInterval* aBaseInterval) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp
  MOZ_ASSERT(!mBaseInterval,
             "Attempting to reassociate an instance time with a different "
             "interval.");

  if (aBaseInterval) {
    MOZ_ASSERT(mCreator,
               "Attempting to create a dependent instance time without "
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInstanceTime.cpp
               "reference to the creating nsSMILTimeValueSpec object.");
    if (!mCreator) return;
||||||| merged common ancestors
               "reference to the creating nsSMILTimeValueSpec object.");
    if (!mCreator)
      return;
=======
               "reference to the creating SMILTimeValueSpec object.");
    if (!mCreator) return;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInstanceTime.cpp

    aBaseInterval->AddDependentTime(*this);
  }

  mBaseInterval = aBaseInterval;
}

}  // namespace mozilla
