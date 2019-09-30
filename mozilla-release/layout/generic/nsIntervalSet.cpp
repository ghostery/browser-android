/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
// vim:cindent:ts=8:et:sw=4:
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* a set of ranges on a number-line */

#include "nsIntervalSet.h"
#include <new>
#include <algorithm>
<<<<<<< HEAD
#include "nsIPresShell.h"  // for allocation
||||||| merged common ancestors
#include "nsIPresShell.h" // for allocation
=======
#include "mozilla/PresShell.h"  // for allocation
>>>>>>> upstream-releases

using namespace mozilla;

<<<<<<< HEAD
nsIntervalSet::nsIntervalSet(nsIPresShell *aPresShell)
    : mList(nullptr), mPresShell(aPresShell) {}
||||||| merged common ancestors
nsIntervalSet::nsIntervalSet(nsIPresShell* aPresShell)
    : mList(nullptr),
      mPresShell(aPresShell)
{
}
=======
nsIntervalSet::nsIntervalSet(PresShell* aPresShell)
    : mList(nullptr), mPresShell(aPresShell) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
nsIntervalSet::~nsIntervalSet() {
  Interval *current = mList;
  while (current) {
    Interval *trash = current;
    current = current->mNext;
    FreeInterval(trash);
  }
||||||| merged common ancestors
nsIntervalSet::~nsIntervalSet()
{
    Interval *current = mList;
    while (current) {
        Interval *trash = current;
        current = current->mNext;
        FreeInterval(trash);
    }
=======
nsIntervalSet::~nsIntervalSet() {
  Interval* current = mList;
  while (current) {
    Interval* trash = current;
    current = current->mNext;
    FreeInterval(trash);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void *nsIntervalSet::AllocateInterval() {
  return mPresShell->AllocateByObjectID(eArenaObjectID_nsIntervalSet_Interval,
                                        sizeof(Interval));
||||||| merged common ancestors
void*
nsIntervalSet::AllocateInterval()
{
    return mPresShell->AllocateByObjectID(
        eArenaObjectID_nsIntervalSet_Interval, sizeof(Interval));
=======
void* nsIntervalSet::AllocateInterval() {
  return mPresShell->AllocateByObjectID(eArenaObjectID_nsIntervalSet_Interval,
                                        sizeof(Interval));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void nsIntervalSet::FreeInterval(nsIntervalSet::Interval *aInterval) {
  NS_ASSERTION(aInterval, "null interval");
||||||| merged common ancestors
void nsIntervalSet::FreeInterval(nsIntervalSet::Interval *aInterval)
{
    NS_ASSERTION(aInterval, "null interval");
=======
void nsIntervalSet::FreeInterval(nsIntervalSet::Interval* aInterval) {
  NS_ASSERTION(aInterval, "null interval");
>>>>>>> upstream-releases

  aInterval->Interval::~Interval();
  mPresShell->FreeByObjectID(eArenaObjectID_nsIntervalSet_Interval, aInterval);
}

<<<<<<< HEAD
void nsIntervalSet::IncludeInterval(coord_type aBegin, coord_type aEnd) {
  auto newInterval = static_cast<Interval *>(AllocateInterval());
  new (newInterval) Interval(aBegin, aEnd);
||||||| merged common ancestors
void nsIntervalSet::IncludeInterval(coord_type aBegin, coord_type aEnd)
{
    auto newInterval = static_cast<Interval*>(AllocateInterval());
    new(newInterval) Interval(aBegin, aEnd);
=======
void nsIntervalSet::IncludeInterval(coord_type aBegin, coord_type aEnd) {
  auto newInterval = static_cast<Interval*>(AllocateInterval());
  new (newInterval) Interval(aBegin, aEnd);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Interval **current = &mList;
  while (*current && (*current)->mEnd < aBegin) current = &(*current)->mNext;
||||||| merged common ancestors
    Interval **current = &mList;
    while (*current && (*current)->mEnd < aBegin)
        current = &(*current)->mNext;
=======
  Interval** current = &mList;
  while (*current && (*current)->mEnd < aBegin) current = &(*current)->mNext;
>>>>>>> upstream-releases

  newInterval->mNext = *current;
  *current = newInterval;

<<<<<<< HEAD
  Interval *subsumed = newInterval->mNext;
  while (subsumed && subsumed->mBegin <= aEnd) {
    newInterval->mBegin = std::min(newInterval->mBegin, subsumed->mBegin);
    newInterval->mEnd = std::max(newInterval->mEnd, subsumed->mEnd);
    newInterval->mNext = subsumed->mNext;
    FreeInterval(subsumed);
    subsumed = newInterval->mNext;
  }
||||||| merged common ancestors
    Interval *subsumed = newInterval->mNext;
    while (subsumed && subsumed->mBegin <= aEnd) {
        newInterval->mBegin = std::min(newInterval->mBegin, subsumed->mBegin);
        newInterval->mEnd = std::max(newInterval->mEnd, subsumed->mEnd);
        newInterval->mNext = subsumed->mNext;
        FreeInterval(subsumed);
        subsumed = newInterval->mNext;
    }
=======
  Interval* subsumed = newInterval->mNext;
  while (subsumed && subsumed->mBegin <= aEnd) {
    newInterval->mBegin = std::min(newInterval->mBegin, subsumed->mBegin);
    newInterval->mEnd = std::max(newInterval->mEnd, subsumed->mEnd);
    newInterval->mNext = subsumed->mNext;
    FreeInterval(subsumed);
    subsumed = newInterval->mNext;
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool nsIntervalSet::Intersects(coord_type aBegin, coord_type aEnd) const {
  Interval *current = mList;
  while (current && current->mBegin <= aEnd) {
    if (current->mEnd >= aBegin) return true;
    current = current->mNext;
  }
  return false;
||||||| merged common ancestors
bool nsIntervalSet::Intersects(coord_type aBegin, coord_type aEnd) const
{
    Interval *current = mList;
    while (current && current->mBegin <= aEnd) {
        if (current->mEnd >= aBegin)
            return true;
        current = current->mNext;
    }
    return false;
=======
bool nsIntervalSet::Intersects(coord_type aBegin, coord_type aEnd) const {
  Interval* current = mList;
  while (current && current->mBegin <= aEnd) {
    if (current->mEnd >= aBegin) return true;
    current = current->mNext;
  }
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool nsIntervalSet::Contains(coord_type aBegin, coord_type aEnd) const {
  Interval *current = mList;
  while (current && current->mBegin <= aBegin) {
    if (current->mEnd >= aEnd) return true;
    current = current->mNext;
  }
  return false;
||||||| merged common ancestors
bool nsIntervalSet::Contains(coord_type aBegin, coord_type aEnd) const
{
    Interval *current = mList;
    while (current && current->mBegin <= aBegin) {
        if (current->mEnd >= aEnd)
            return true;
        current = current->mNext;
    }
    return false;
=======
bool nsIntervalSet::Contains(coord_type aBegin, coord_type aEnd) const {
  Interval* current = mList;
  while (current && current->mBegin <= aBegin) {
    if (current->mEnd >= aEnd) return true;
    current = current->mNext;
  }
  return false;
>>>>>>> upstream-releases
}
