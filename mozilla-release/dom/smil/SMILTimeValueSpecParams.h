/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_SMILTimeValueSpecParams_h
#define mozilla_SMILTimeValueSpecParams_h

#include "mozilla/SMILTimeValue.h"
#include "nsAtom.h"

namespace mozilla {

//----------------------------------------------------------------------
// SMILTimeValueSpecParams
//
// A simple data type for storing the result of parsing a single begin or end
// value (e.g. the '5s' in begin="5s; indefinite; a.begin+2s").

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpecParams.h
class nsSMILTimeValueSpecParams {
 public:
  nsSMILTimeValueSpecParams()
      : mType(INDEFINITE), mSyncBegin(false), mRepeatIteration(0) {}
||||||| merged common ancestors
class nsSMILTimeValueSpecParams
{
public:
  nsSMILTimeValueSpecParams()
    : mType(INDEFINITE)
    , mSyncBegin(false)
    , mRepeatIteration(0)
  { }
=======
class SMILTimeValueSpecParams {
 public:
  SMILTimeValueSpecParams()
      : mType(INDEFINITE), mSyncBegin(false), mRepeatIteration(0) {}
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpecParams.h

  // The type of value this specification describes
  enum { OFFSET, SYNCBASE, EVENT, REPEAT, WALLCLOCK, INDEFINITE } mType;

  // A clock value that is added to:
  // - type OFFSET: the document begin
  // - type SYNCBASE: the timebase's begin or end time
  // - type EVENT: the event time
  // - type REPEAT: the repeat time
  // It is not used for WALLCLOCK or INDEFINITE times
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpecParams.h
  nsSMILTimeValue mOffset;
||||||| merged common ancestors
  nsSMILTimeValue   mOffset;
=======
  SMILTimeValue mOffset;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpecParams.h

  // The base element that this specification refers to.
  // For SYNCBASE types, this is the timebase
  // For EVENT and REPEAT types, this is the eventbase
  RefPtr<nsAtom> mDependentElemID;

  // The event to respond to.
  // Only used for EVENT types.
  RefPtr<nsAtom> mEventSymbol;

  // Indicates if this specification refers to the begin or end of the dependent
  // element.
  // Only used for SYNCBASE types.
  bool mSyncBegin;

  // The repeat iteration to respond to.
  // Only used for mType=REPEAT.
  uint32_t mRepeatIteration;
};

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpecParams.h
#endif  // NS_SMILTIMEVALUESPECPARAMS_H_
||||||| merged common ancestors
#endif // NS_SMILTIMEVALUESPECPARAMS_H_
=======
}  // namespace mozilla

#endif  // mozilla_SMILTimeValueSpecParams_h
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpecParams.h
