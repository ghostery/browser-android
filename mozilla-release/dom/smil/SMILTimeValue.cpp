/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SMILTimeValue.h"

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValue.cpp
const nsSMILTime nsSMILTimeValue::kUnresolvedMillis =
    std::numeric_limits<nsSMILTime>::max();
||||||| merged common ancestors
const nsSMILTime nsSMILTimeValue::kUnresolvedMillis =
  std::numeric_limits<nsSMILTime>::max();
=======
namespace mozilla {

const SMILTime SMILTimeValue::kUnresolvedMillis =
    std::numeric_limits<SMILTime>::max();
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValue.cpp

//----------------------------------------------------------------------
// SMILTimeValue methods:

static inline int8_t Cmp(int64_t aA, int64_t aB) {
  return aA == aB ? 0 : (aA > aB ? 1 : -1);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValue.cpp
int8_t nsSMILTimeValue::CompareTo(const nsSMILTimeValue& aOther) const {
||||||| merged common ancestors
int8_t
nsSMILTimeValue::CompareTo(const nsSMILTimeValue& aOther) const
{
=======
int8_t SMILTimeValue::CompareTo(const SMILTimeValue& aOther) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValue.cpp
  int8_t result;

  if (mState == STATE_DEFINITE) {
    result = (aOther.mState == STATE_DEFINITE)
                 ? Cmp(mMilliseconds, aOther.mMilliseconds)
                 : -1;
  } else if (mState == STATE_INDEFINITE) {
    if (aOther.mState == STATE_DEFINITE)
      result = 1;
    else if (aOther.mState == STATE_INDEFINITE)
      result = 0;
    else
      result = -1;
  } else {
    result = (aOther.mState != STATE_UNRESOLVED) ? 1 : 0;
  }

  return result;
}

}  // namespace mozilla
