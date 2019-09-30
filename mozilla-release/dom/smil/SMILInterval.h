/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_SMILInterval_h
#define mozilla_SMILInterval_h

#include "mozilla/SMILInstanceTime.h"
#include "nsTArray.h"

namespace mozilla {

//----------------------------------------------------------------------
// SMILInterval class
//
// A structure consisting of a begin and end time. The begin time must be
// resolved (i.e. not indefinite or unresolved).
//
// For an overview of how this class is related to other SMIL time classes see
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.h
// the documentation in nsSMILTimeValue.h

class nsSMILInterval {
 public:
  nsSMILInterval();
  nsSMILInterval(const nsSMILInterval& aOther);
  ~nsSMILInterval();
||||||| merged common ancestors
// the documentation in nsSMILTimeValue.h

class nsSMILInterval
{
public:
  nsSMILInterval();
  nsSMILInterval(const nsSMILInterval& aOther);
  ~nsSMILInterval();
=======
// the documentation in SMILTimeValue.h

class SMILInterval {
 public:
  SMILInterval();
  SMILInterval(const SMILInterval& aOther);
  ~SMILInterval();
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.h
  void Unlink(bool aFiltered = false);

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.h
  const nsSMILInstanceTime* Begin() const {
||||||| merged common ancestors
  const nsSMILInstanceTime* Begin() const
  {
=======
  const SMILInstanceTime* Begin() const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.h
    MOZ_ASSERT(mBegin && mEnd,
               "Requesting Begin() on un-initialized instance time");
    return mBegin;
  }
  SMILInstanceTime* Begin();

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.h
  const nsSMILInstanceTime* End() const {
||||||| merged common ancestors
  const nsSMILInstanceTime* End() const
  {
=======
  const SMILInstanceTime* End() const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.h
    MOZ_ASSERT(mBegin && mEnd,
               "Requesting End() on un-initialized instance time");
    return mEnd;
  }
  SMILInstanceTime* End();

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.h
  void SetBegin(nsSMILInstanceTime& aBegin);
  void SetEnd(nsSMILInstanceTime& aEnd);
  void Set(nsSMILInstanceTime& aBegin, nsSMILInstanceTime& aEnd) {
||||||| merged common ancestors
  void SetBegin(nsSMILInstanceTime& aBegin);
  void SetEnd(nsSMILInstanceTime& aEnd);
  void Set(nsSMILInstanceTime& aBegin, nsSMILInstanceTime& aEnd)
  {
=======
  void SetBegin(SMILInstanceTime& aBegin);
  void SetEnd(SMILInstanceTime& aEnd);
  void Set(SMILInstanceTime& aBegin, SMILInstanceTime& aEnd) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.h
    SetBegin(aBegin);
    SetEnd(aEnd);
  }

  void FixBegin();
  void FixEnd();

  typedef nsTArray<RefPtr<SMILInstanceTime> > InstanceTimeList;

  void AddDependentTime(SMILInstanceTime& aTime);
  void RemoveDependentTime(const SMILInstanceTime& aTime);
  void GetDependentTimes(InstanceTimeList& aTimes);

  // Cue for assessing if this interval can be filtered
  bool IsDependencyChainLink() const;

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.h
 private:
  RefPtr<nsSMILInstanceTime> mBegin;
  RefPtr<nsSMILInstanceTime> mEnd;
||||||| merged common ancestors
private:
  RefPtr<nsSMILInstanceTime> mBegin;
  RefPtr<nsSMILInstanceTime> mEnd;
=======
 private:
  RefPtr<SMILInstanceTime> mBegin;
  RefPtr<SMILInstanceTime> mEnd;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.h

  // SMILInstanceTimes to notify when this interval is changed or deleted.
  InstanceTimeList mDependentTimes;

  // Indicates if the end points of the interval are fixed or not.
  //
  // Note that this is not the same as having an end point whose TIME is fixed
  // (i.e. SMILInstanceTime::IsFixed() returns true). This is because it is
  // possible to have an end point with a fixed TIME and yet still update the
  // end point to refer to a different SMILInstanceTime object.
  //
  // However, if mBegin/EndFixed is true, then BOTH the SMILInstanceTime
  // OBJECT returned for that end point and its TIME value will not change.
  bool mBeginFixed;
  bool mEndFixed;
};

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILInterval.h
#endif  // NS_SMILINTERVAL_H_
||||||| merged common ancestors
#endif // NS_SMILINTERVAL_H_
=======
}  // namespace mozilla

#endif  // mozilla_SMILInterval_h
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILInterval.h
