/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SMILInterval.h"

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.cpp
nsSMILInterval::nsSMILInterval() : mBeginFixed(false), mEndFixed(false) {}
||||||| merged common ancestors
nsSMILInterval::nsSMILInterval()
:
  mBeginFixed(false),
  mEndFixed(false)
{
}
=======
#include "mozilla/DebugOnly.h"

namespace mozilla {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.cpp

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.cpp
nsSMILInterval::nsSMILInterval(const nsSMILInterval& aOther)
    : mBegin(aOther.mBegin),
      mEnd(aOther.mEnd),
      mBeginFixed(false),
      mEndFixed(false) {
||||||| merged common ancestors
nsSMILInterval::nsSMILInterval(const nsSMILInterval& aOther)
:
  mBegin(aOther.mBegin),
  mEnd(aOther.mEnd),
  mBeginFixed(false),
  mEndFixed(false)
{
=======
SMILInterval::SMILInterval() : mBeginFixed(false), mEndFixed(false) {}

SMILInterval::SMILInterval(const SMILInterval& aOther)
    : mBegin(aOther.mBegin),
      mEnd(aOther.mEnd),
      mBeginFixed(false),
      mEndFixed(false) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.cpp
  MOZ_ASSERT(aOther.mDependentTimes.IsEmpty(),
             "Attempt to copy-construct an interval with dependent times; this "
             "will lead to instance times being shared between intervals.");

  // For the time being we don't allow intervals with fixed endpoints to be
  // copied since we only ever copy-construct to establish a new current
  // interval. If we ever need to copy historical intervals we may need to move
  // the ReleaseFixedEndpoint calls from Unlink to the dtor.
  MOZ_ASSERT(!aOther.mBeginFixed && !aOther.mEndFixed,
             "Attempt to copy-construct an interval with fixed endpoints");
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.cpp
nsSMILInterval::~nsSMILInterval() {
||||||| merged common ancestors
nsSMILInterval::~nsSMILInterval()
{
=======
SMILInterval::~SMILInterval() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.cpp
  MOZ_ASSERT(mDependentTimes.IsEmpty(),
             "Destroying interval without disassociating dependent instance "
             "times. Unlink was not called");
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.cpp
void nsSMILInterval::Unlink(bool aFiltered) {
||||||| merged common ancestors
void
nsSMILInterval::Unlink(bool aFiltered)
{
=======
void SMILInterval::Unlink(bool aFiltered) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.cpp
  for (int32_t i = mDependentTimes.Length() - 1; i >= 0; --i) {
    if (aFiltered) {
      mDependentTimes[i]->HandleFilteredInterval();
    } else {
      mDependentTimes[i]->HandleDeletedInterval();
    }
  }
  mDependentTimes.Clear();
  if (mBegin && mBeginFixed) {
    mBegin->ReleaseFixedEndpoint();
  }
  mBegin = nullptr;
  if (mEnd && mEndFixed) {
    mEnd->ReleaseFixedEndpoint();
  }
  mEnd = nullptr;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.cpp
nsSMILInstanceTime* nsSMILInterval::Begin() {
  MOZ_ASSERT(mBegin && mEnd, "Requesting Begin() on un-initialized interval.");
||||||| merged common ancestors
nsSMILInstanceTime*
nsSMILInterval::Begin()
{
  MOZ_ASSERT(mBegin && mEnd,
             "Requesting Begin() on un-initialized interval.");
=======
SMILInstanceTime* SMILInterval::Begin() {
  MOZ_ASSERT(mBegin && mEnd, "Requesting Begin() on un-initialized interval.");
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.cpp
  return mBegin;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.cpp
nsSMILInstanceTime* nsSMILInterval::End() {
  MOZ_ASSERT(mBegin && mEnd, "Requesting End() on un-initialized interval.");
||||||| merged common ancestors
nsSMILInstanceTime*
nsSMILInterval::End()
{
  MOZ_ASSERT(mBegin && mEnd,
             "Requesting End() on un-initialized interval.");
=======
SMILInstanceTime* SMILInterval::End() {
  MOZ_ASSERT(mBegin && mEnd, "Requesting End() on un-initialized interval.");
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.cpp
  return mEnd;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.cpp
void nsSMILInterval::SetBegin(nsSMILInstanceTime& aBegin) {
||||||| merged common ancestors
void
nsSMILInterval::SetBegin(nsSMILInstanceTime& aBegin)
{
=======
void SMILInterval::SetBegin(SMILInstanceTime& aBegin) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.cpp
  MOZ_ASSERT(aBegin.Time().IsDefinite(),
             "Attempt to set unresolved or indefinite begin time on interval");
  MOZ_ASSERT(!mBeginFixed,
             "Attempt to set begin time but the begin point is fixed");
  // Check that we're not making an instance time dependent on itself. Such an
  // arrangement does not make intuitive sense and should be detected when
  // creating or updating intervals.
  MOZ_ASSERT(!mBegin || aBegin.GetBaseTime() != mBegin,
             "Attempt to make self-dependent instance time");

  mBegin = &aBegin;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.cpp
void nsSMILInterval::SetEnd(nsSMILInstanceTime& aEnd) {
  MOZ_ASSERT(!mEndFixed, "Attempt to set end time but the end point is fixed");
||||||| merged common ancestors
void
nsSMILInterval::SetEnd(nsSMILInstanceTime& aEnd)
{
  MOZ_ASSERT(!mEndFixed,
             "Attempt to set end time but the end point is fixed");
=======
void SMILInterval::SetEnd(SMILInstanceTime& aEnd) {
  MOZ_ASSERT(!mEndFixed, "Attempt to set end time but the end point is fixed");
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.cpp
  // As with SetBegin, check we're not making an instance time dependent on
  // itself.
  MOZ_ASSERT(!mEnd || aEnd.GetBaseTime() != mEnd,
             "Attempting to make self-dependent instance time");

  mEnd = &aEnd;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.cpp
void nsSMILInterval::FixBegin() {
  MOZ_ASSERT(mBegin && mEnd, "Fixing begin point on un-initialized interval");
||||||| merged common ancestors
void
nsSMILInterval::FixBegin()
{
  MOZ_ASSERT(mBegin && mEnd,
             "Fixing begin point on un-initialized interval");
=======
void SMILInterval::FixBegin() {
  MOZ_ASSERT(mBegin && mEnd, "Fixing begin point on un-initialized interval");
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.cpp
  MOZ_ASSERT(!mBeginFixed, "Duplicate calls to FixBegin()");
  mBeginFixed = true;
  mBegin->AddRefFixedEndpoint();
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.cpp
void nsSMILInterval::FixEnd() {
  MOZ_ASSERT(mBegin && mEnd, "Fixing end point on un-initialized interval");
||||||| merged common ancestors
void
nsSMILInterval::FixEnd()
{
  MOZ_ASSERT(mBegin && mEnd,
             "Fixing end point on un-initialized interval");
=======
void SMILInterval::FixEnd() {
  MOZ_ASSERT(mBegin && mEnd, "Fixing end point on un-initialized interval");
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.cpp
  MOZ_ASSERT(mBeginFixed,
             "Fixing the end of an interval without a fixed begin");
  MOZ_ASSERT(!mEndFixed, "Duplicate calls to FixEnd()");
  mEndFixed = true;
  mEnd->AddRefFixedEndpoint();
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.cpp
void nsSMILInterval::AddDependentTime(nsSMILInstanceTime& aTime) {
  RefPtr<nsSMILInstanceTime>* inserted =
      mDependentTimes.InsertElementSorted(&aTime);
||||||| merged common ancestors
void
nsSMILInterval::AddDependentTime(nsSMILInstanceTime& aTime)
{
  RefPtr<nsSMILInstanceTime>* inserted =
    mDependentTimes.InsertElementSorted(&aTime);
=======
void SMILInterval::AddDependentTime(SMILInstanceTime& aTime) {
  RefPtr<SMILInstanceTime>* inserted =
      mDependentTimes.InsertElementSorted(&aTime);
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.cpp
  if (!inserted) {
    NS_WARNING("Insufficient memory to insert instance time.");
  }
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.cpp
void nsSMILInterval::RemoveDependentTime(const nsSMILInstanceTime& aTime) {
#ifdef DEBUG
  bool found =
#endif
      mDependentTimes.RemoveElementSorted(&aTime);
||||||| merged common ancestors
void
nsSMILInterval::RemoveDependentTime(const nsSMILInstanceTime& aTime)
{
#ifdef DEBUG
  bool found =
#endif
    mDependentTimes.RemoveElementSorted(&aTime);
=======
void SMILInterval::RemoveDependentTime(const SMILInstanceTime& aTime) {
  DebugOnly<bool> found = mDependentTimes.RemoveElementSorted(&aTime);
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.cpp
  MOZ_ASSERT(found, "Couldn't find instance time to delete.");
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.cpp
void nsSMILInterval::GetDependentTimes(InstanceTimeList& aTimes) {
||||||| merged common ancestors
void
nsSMILInterval::GetDependentTimes(InstanceTimeList& aTimes)
{
=======
void SMILInterval::GetDependentTimes(InstanceTimeList& aTimes) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.cpp
  aTimes = mDependentTimes;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.cpp
bool nsSMILInterval::IsDependencyChainLink() const {
||||||| merged common ancestors
bool
nsSMILInterval::IsDependencyChainLink() const
{
=======
bool SMILInterval::IsDependencyChainLink() const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.cpp
  if (!mBegin || !mEnd)
    return false;  // Not yet initialised so it can't be part of a chain

  if (mDependentTimes.IsEmpty()) return false;  // No dependents, chain end

  // So we have dependents, but we're still only a link in the chain (as opposed
  // to the end of the chain) if one of our endpoints is dependent on an
  // interval other than ourselves.
  return (mBegin->IsDependent() && mBegin->GetBaseInterval() != this) ||
         (mEnd->IsDependent() && mEnd->GetBaseInterval() != this);
}

}  // namespace mozilla
