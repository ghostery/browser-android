/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SMILTimeContainer.h"

#include "mozilla/AutoRestore.h"
#include "mozilla/SMILTimedElement.h"
#include "mozilla/SMILTimeValue.h"
#include <algorithm>

namespace mozilla {

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
using namespace mozilla;

nsSMILTimeContainer::nsSMILTimeContainer()
    : mParent(nullptr),
      mCurrentTime(0L),
      mParentOffset(0L),
      mPauseStart(0L),
      mNeedsPauseSample(false),
      mNeedsRewind(false),
      mIsSeeking(false),
||||||| merged common ancestors
using namespace mozilla;

nsSMILTimeContainer::nsSMILTimeContainer()
:
  mParent(nullptr),
  mCurrentTime(0L),
  mParentOffset(0L),
  mPauseStart(0L),
  mNeedsPauseSample(false),
  mNeedsRewind(false),
  mIsSeeking(false),
=======
SMILTimeContainer::SMILTimeContainer()
    : mParent(nullptr),
      mCurrentTime(0L),
      mParentOffset(0L),
      mPauseStart(0L),
      mNeedsPauseSample(false),
      mNeedsRewind(false),
      mIsSeeking(false),
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
#ifdef DEBUG
      mHoldingEntries(false),
#endif
      mPauseState(PAUSE_BEGIN) {
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
nsSMILTimeContainer::~nsSMILTimeContainer() {
||||||| merged common ancestors
nsSMILTimeContainer::~nsSMILTimeContainer()
{
=======
SMILTimeContainer::~SMILTimeContainer() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
  if (mParent) {
    mParent->RemoveChild(*this);
  }
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
nsSMILTimeValue nsSMILTimeContainer::ContainerToParentTime(
    nsSMILTime aContainerTime) const {
||||||| merged common ancestors
nsSMILTimeValue
nsSMILTimeContainer::ContainerToParentTime(nsSMILTime aContainerTime) const
{
=======
SMILTimeValue SMILTimeContainer::ContainerToParentTime(
    SMILTime aContainerTime) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
  // If we're paused, then future times are indefinite
  if (IsPaused() && aContainerTime > mCurrentTime)
    return SMILTimeValue::Indefinite();

  return SMILTimeValue(aContainerTime + mParentOffset);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
nsSMILTimeValue nsSMILTimeContainer::ParentToContainerTime(
    nsSMILTime aParentTime) const {
||||||| merged common ancestors
nsSMILTimeValue
nsSMILTimeContainer::ParentToContainerTime(nsSMILTime aParentTime) const
{
=======
SMILTimeValue SMILTimeContainer::ParentToContainerTime(
    SMILTime aParentTime) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
  // If we're paused, then any time after when we paused is indefinite
  if (IsPaused() && aParentTime > mPauseStart)
    return SMILTimeValue::Indefinite();

  return SMILTimeValue(aParentTime - mParentOffset);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
void nsSMILTimeContainer::Begin() {
||||||| merged common ancestors
void
nsSMILTimeContainer::Begin()
{
=======
void SMILTimeContainer::Begin() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
  Resume(PAUSE_BEGIN);
  if (mPauseState) {
    mNeedsPauseSample = true;
  }

  // This is a little bit complicated here. Ideally we'd just like to call
  // Sample() and force an initial sample but this turns out to be a bad idea
  // because this may mean that NeedsSample() no longer reports true and so when
  // we come to the first real sample our parent will skip us over altogether.
  // So we force the time to be updated and adopt the policy to never call
  // Sample() ourselves but to always leave that to our parent or client.

  UpdateCurrentTime();
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
void nsSMILTimeContainer::Pause(uint32_t aType) {
||||||| merged common ancestors
void
nsSMILTimeContainer::Pause(uint32_t aType)
{
=======
void SMILTimeContainer::Pause(uint32_t aType) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
  bool didStartPause = false;

  if (!mPauseState && aType) {
    mPauseStart = GetParentTime();
    mNeedsPauseSample = true;
    didStartPause = true;
  }

  mPauseState |= aType;

  if (didStartPause) {
    NotifyTimeChange();
  }
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
void nsSMILTimeContainer::Resume(uint32_t aType) {
  if (!mPauseState) return;
||||||| merged common ancestors
void
nsSMILTimeContainer::Resume(uint32_t aType)
{
  if (!mPauseState)
    return;
=======
void SMILTimeContainer::Resume(uint32_t aType) {
  if (!mPauseState) return;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp

  mPauseState &= ~aType;

  if (!mPauseState) {
    SMILTime extraOffset = GetParentTime() - mPauseStart;
    mParentOffset += extraOffset;
    NotifyTimeChange();
  }
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
nsSMILTime nsSMILTimeContainer::GetCurrentTime() const {
||||||| merged common ancestors
nsSMILTime
nsSMILTimeContainer::GetCurrentTime() const
{
=======
SMILTime SMILTimeContainer::GetCurrentTimeAsSMILTime() const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
  // The following behaviour is consistent with:
  // http://www.w3.org/2003/01/REC-SVG11-20030114-errata
  //  #getCurrentTime_setCurrentTime_undefined_before_document_timeline_begin
  // which says that if GetCurrentTime is called before the document timeline
  // has begun we should just return 0.
  if (IsPausedByType(PAUSE_BEGIN)) return 0L;

  return mCurrentTime;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
void nsSMILTimeContainer::SetCurrentTime(nsSMILTime aSeekTo) {
||||||| merged common ancestors
void
nsSMILTimeContainer::SetCurrentTime(nsSMILTime aSeekTo)
{
=======
void SMILTimeContainer::SetCurrentTime(SMILTime aSeekTo) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
  // SVG 1.1 doesn't specify what to do for negative times so we adopt SVGT1.2's
  // behaviour of clamping negative times to 0.
  aSeekTo = std::max<SMILTime>(0, aSeekTo);

  // The following behaviour is consistent with:
  // http://www.w3.org/2003/01/REC-SVG11-20030114-errata
  //  #getCurrentTime_setCurrentTime_undefined_before_document_timeline_begin
  // which says that if SetCurrentTime is called before the document timeline
  // has begun we should still adjust the offset.
  SMILTime parentTime = GetParentTime();
  mParentOffset = parentTime - aSeekTo;
  mIsSeeking = true;

  if (IsPaused()) {
    mNeedsPauseSample = true;
    mPauseStart = parentTime;
  }

  if (aSeekTo < mCurrentTime) {
    // Backwards seek
    mNeedsRewind = true;
    ClearMilestones();
  }

  // Force an update to the current time in case we get a call to GetCurrentTime
  // before another call to Sample().
  UpdateCurrentTime();

  NotifyTimeChange();
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
nsSMILTime nsSMILTimeContainer::GetParentTime() const {
  if (mParent) return mParent->GetCurrentTime();
||||||| merged common ancestors
nsSMILTime
nsSMILTimeContainer::GetParentTime() const
{
  if (mParent)
    return mParent->GetCurrentTime();
=======
SMILTime SMILTimeContainer::GetParentTime() const {
  if (mParent) return mParent->GetCurrentTimeAsSMILTime();
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp

  return 0L;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
void nsSMILTimeContainer::SyncPauseTime() {
||||||| merged common ancestors
void
nsSMILTimeContainer::SyncPauseTime()
{
=======
void SMILTimeContainer::SyncPauseTime() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
  if (IsPaused()) {
    SMILTime parentTime = GetParentTime();
    SMILTime extraOffset = parentTime - mPauseStart;
    mParentOffset += extraOffset;
    mPauseStart = parentTime;
  }
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
void nsSMILTimeContainer::Sample() {
  if (!NeedsSample()) return;
||||||| merged common ancestors
void
nsSMILTimeContainer::Sample()
{
  if (!NeedsSample())
    return;
=======
void SMILTimeContainer::Sample() {
  if (!NeedsSample()) return;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp

  UpdateCurrentTime();
  DoSample();

  mNeedsPauseSample = false;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
nsresult nsSMILTimeContainer::SetParent(nsSMILTimeContainer* aParent) {
||||||| merged common ancestors
nsresult
nsSMILTimeContainer::SetParent(nsSMILTimeContainer* aParent)
{
=======
nsresult SMILTimeContainer::SetParent(SMILTimeContainer* aParent) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
  if (mParent) {
    mParent->RemoveChild(*this);
    // When we're not attached to a parent time container, GetParentTime() will
    // return 0. We need to adjust our pause state information to be relative to
    // this new time base.
    // Note that since "current time = parent time - parent offset" setting the
    // parent offset and pause start as follows preserves our current time even
    // while parent time = 0.
    mParentOffset = -mCurrentTime;
    mPauseStart = 0L;
  }

  mParent = aParent;

  nsresult rv = NS_OK;
  if (mParent) {
    rv = mParent->AddChild(*this);
  }

  return rv;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
bool nsSMILTimeContainer::AddMilestone(
    const nsSMILMilestone& aMilestone,
    mozilla::dom::SVGAnimationElement& aElement) {
||||||| merged common ancestors
bool
nsSMILTimeContainer::AddMilestone(const nsSMILMilestone& aMilestone,
                                  mozilla::dom::SVGAnimationElement& aElement)
{
=======
bool SMILTimeContainer::AddMilestone(
    const SMILMilestone& aMilestone,
    mozilla::dom::SVGAnimationElement& aElement) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
  // We record the milestone time and store it along with the element but this
  // time may change (e.g. if attributes are changed on the timed element in
  // between samples). If this happens, then we may do an unecessary sample
  // but that's pretty cheap.
  MOZ_ASSERT(!mHoldingEntries);
  return mMilestoneEntries.Push(MilestoneEntry(aMilestone, aElement));
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
void nsSMILTimeContainer::ClearMilestones() {
||||||| merged common ancestors
void
nsSMILTimeContainer::ClearMilestones()
{
=======
void SMILTimeContainer::ClearMilestones() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
  MOZ_ASSERT(!mHoldingEntries);
  mMilestoneEntries.Clear();
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
bool nsSMILTimeContainer::GetNextMilestoneInParentTime(
    nsSMILMilestone& aNextMilestone) const {
  if (mMilestoneEntries.IsEmpty()) return false;
||||||| merged common ancestors
bool
nsSMILTimeContainer::GetNextMilestoneInParentTime(
    nsSMILMilestone& aNextMilestone) const
{
  if (mMilestoneEntries.IsEmpty())
    return false;
=======
bool SMILTimeContainer::GetNextMilestoneInParentTime(
    SMILMilestone& aNextMilestone) const {
  if (mMilestoneEntries.IsEmpty()) return false;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
  nsSMILTimeValue parentTime =
      ContainerToParentTime(mMilestoneEntries.Top().mMilestone.mTime);
  if (!parentTime.IsDefinite()) return false;
||||||| merged common ancestors
  nsSMILTimeValue parentTime =
    ContainerToParentTime(mMilestoneEntries.Top().mMilestone.mTime);
  if (!parentTime.IsDefinite())
    return false;
=======
  SMILTimeValue parentTime =
      ContainerToParentTime(mMilestoneEntries.Top().mMilestone.mTime);
  if (!parentTime.IsDefinite()) return false;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp

  aNextMilestone = SMILMilestone(parentTime.GetMillis(),
                                 mMilestoneEntries.Top().mMilestone.mIsEnd);

  return true;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
bool nsSMILTimeContainer::PopMilestoneElementsAtMilestone(
    const nsSMILMilestone& aMilestone, AnimElemArray& aMatchedElements) {
  if (mMilestoneEntries.IsEmpty()) return false;
||||||| merged common ancestors
bool
nsSMILTimeContainer::PopMilestoneElementsAtMilestone(
      const nsSMILMilestone& aMilestone,
      AnimElemArray& aMatchedElements)
{
  if (mMilestoneEntries.IsEmpty())
    return false;
=======
bool SMILTimeContainer::PopMilestoneElementsAtMilestone(
    const SMILMilestone& aMilestone, AnimElemArray& aMatchedElements) {
  if (mMilestoneEntries.IsEmpty()) return false;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
  nsSMILTimeValue containerTime = ParentToContainerTime(aMilestone.mTime);
  if (!containerTime.IsDefinite()) return false;
||||||| merged common ancestors
  nsSMILTimeValue containerTime = ParentToContainerTime(aMilestone.mTime);
  if (!containerTime.IsDefinite())
    return false;
=======
  SMILTimeValue containerTime = ParentToContainerTime(aMilestone.mTime);
  if (!containerTime.IsDefinite()) return false;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp

  SMILMilestone containerMilestone(containerTime.GetMillis(),
                                   aMilestone.mIsEnd);

  MOZ_ASSERT(mMilestoneEntries.Top().mMilestone >= containerMilestone,
             "Trying to pop off earliest times but we have earlier ones that "
             "were overlooked");

  MOZ_ASSERT(!mHoldingEntries);

  bool gotOne = false;
  while (!mMilestoneEntries.IsEmpty() &&
         mMilestoneEntries.Top().mMilestone == containerMilestone) {
    aMatchedElements.AppendElement(mMilestoneEntries.Pop().mTimebase);
    gotOne = true;
  }

  return gotOne;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
void nsSMILTimeContainer::Traverse(
    nsCycleCollectionTraversalCallback* aCallback) {
||||||| merged common ancestors
void
nsSMILTimeContainer::Traverse(nsCycleCollectionTraversalCallback* aCallback)
{
=======
void SMILTimeContainer::Traverse(
    nsCycleCollectionTraversalCallback* aCallback) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
#ifdef DEBUG
  AutoRestore<bool> saveHolding(mHoldingEntries);
  mHoldingEntries = true;
#endif
  const MilestoneEntry* p = mMilestoneEntries.Elements();
  while (p < mMilestoneEntries.Elements() + mMilestoneEntries.Length()) {
    NS_CYCLE_COLLECTION_NOTE_EDGE_NAME(*aCallback, "mTimebase");
    aCallback->NoteXPCOMChild(static_cast<nsIContent*>(p->mTimebase.get()));
    ++p;
  }
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
void nsSMILTimeContainer::Unlink() {
||||||| merged common ancestors
void
nsSMILTimeContainer::Unlink()
{
=======
void SMILTimeContainer::Unlink() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
  MOZ_ASSERT(!mHoldingEntries);
  mMilestoneEntries.Clear();
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
void nsSMILTimeContainer::UpdateCurrentTime() {
  nsSMILTime now = IsPaused() ? mPauseStart : GetParentTime();
||||||| merged common ancestors
void
nsSMILTimeContainer::UpdateCurrentTime()
{
  nsSMILTime now = IsPaused() ? mPauseStart : GetParentTime();
=======
void SMILTimeContainer::UpdateCurrentTime() {
  SMILTime now = IsPaused() ? mPauseStart : GetParentTime();
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
  mCurrentTime = now - mParentOffset;
  MOZ_ASSERT(mCurrentTime >= 0, "Container has negative time");
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeContainer.cpp
void nsSMILTimeContainer::NotifyTimeChange() {
||||||| merged common ancestors
void
nsSMILTimeContainer::NotifyTimeChange()
{
=======
void SMILTimeContainer::NotifyTimeChange() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeContainer.cpp
  // Called when the container time is changed with respect to the document
  // time. When this happens time dependencies in other time containers need to
  // re-resolve their times because begin and end times are stored in container
  // time.
  //
  // To get the list of timed elements with dependencies we simply re-use the
  // milestone elements. This is because any timed element with dependents and
  // with significant transitions yet to fire should have their next milestone
  // registered. Other timed elements don't matter.

  // Copy the timed elements to a separate array before calling
  // HandleContainerTimeChange on each of them in case doing so mutates
  // mMilestoneEntries.
  nsTArray<RefPtr<mozilla::dom::SVGAnimationElement>> elems;

  {
#ifdef DEBUG
    AutoRestore<bool> saveHolding(mHoldingEntries);
    mHoldingEntries = true;
#endif
    for (const MilestoneEntry* p = mMilestoneEntries.Elements();
         p < mMilestoneEntries.Elements() + mMilestoneEntries.Length(); ++p) {
      elems.AppendElement(p->mTimebase.get());
    }
  }

  for (auto& elem : elems) {
    elem->TimedElement().HandleContainerTimeChange();
  }
}

}  // namespace mozilla
