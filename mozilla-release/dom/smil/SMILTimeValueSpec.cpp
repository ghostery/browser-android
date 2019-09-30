/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/EventListenerManager.h"
#include "mozilla/SMILInstanceTime.h"
#include "mozilla/SMILInterval.h"
#include "mozilla/SMILParserUtils.h"
#include "mozilla/SMILTimeContainer.h"
#include "mozilla/SMILTimedElement.h"
#include "mozilla/SMILTimeValueSpec.h"
#include "mozilla/SMILTimeValue.h"
#include "mozilla/dom/Event.h"
#include "mozilla/dom/SVGAnimationElement.h"
#include "mozilla/dom/TimeEvent.h"
#include "nsString.h"
#include <limits>

using namespace mozilla::dom;

namespace mozilla {

//----------------------------------------------------------------------
// Nested class: EventListener

NS_IMPL_ISUPPORTS(SMILTimeValueSpec::EventListener, nsIDOMEventListener)

NS_IMETHODIMP
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
nsSMILTimeValueSpec::EventListener::HandleEvent(Event* aEvent) {
||||||| merged common ancestors
nsSMILTimeValueSpec::EventListener::HandleEvent(Event* aEvent)
{
=======
SMILTimeValueSpec::EventListener::HandleEvent(Event* aEvent) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  if (mSpec) {
    mSpec->HandleEvent(aEvent);
  }
  return NS_OK;
}

//----------------------------------------------------------------------
// Implementation

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
nsSMILTimeValueSpec::nsSMILTimeValueSpec(nsSMILTimedElement& aOwner,
                                         bool aIsBegin)
    : mOwner(&aOwner), mIsBegin(aIsBegin), mReferencedElement(this) {}
||||||| merged common ancestors
nsSMILTimeValueSpec::nsSMILTimeValueSpec(nsSMILTimedElement& aOwner,
                                         bool aIsBegin)
  : mOwner(&aOwner),
    mIsBegin(aIsBegin),
    mReferencedElement(this)
{
}
=======
SMILTimeValueSpec::SMILTimeValueSpec(SMILTimedElement& aOwner, bool aIsBegin)
    : mOwner(&aOwner), mIsBegin(aIsBegin), mReferencedElement(this) {}
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
nsSMILTimeValueSpec::~nsSMILTimeValueSpec() {
||||||| merged common ancestors
nsSMILTimeValueSpec::~nsSMILTimeValueSpec()
{
=======
SMILTimeValueSpec::~SMILTimeValueSpec() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  UnregisterFromReferencedElement(mReferencedElement.get());
  if (mEventListener) {
    mEventListener->Disconnect();
    mEventListener = nullptr;
  }
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
nsresult nsSMILTimeValueSpec::SetSpec(const nsAString& aStringSpec,
                                      Element& aContextElement) {
  nsSMILTimeValueSpecParams params;
||||||| merged common ancestors
nsresult
nsSMILTimeValueSpec::SetSpec(const nsAString& aStringSpec,
                             Element& aContextElement)
{
  nsSMILTimeValueSpecParams params;
=======
nsresult SMILTimeValueSpec::SetSpec(const nsAString& aStringSpec,
                                    Element& aContextElement) {
  SMILTimeValueSpecParams params;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp

  if (!SMILParserUtils::ParseTimeValueSpecParams(aStringSpec, params))
    return NS_ERROR_FAILURE;

  mParams = params;

  // According to SMIL 3.0:
  //   The special value "indefinite" does not yield an instance time in the
  //   begin list. It will, however yield a single instance with the value
  //   "indefinite" in an end list. This value is not removed by a reset.
  if (mParams.mType == SMILTimeValueSpecParams::OFFSET ||
      (!mIsBegin && mParams.mType == SMILTimeValueSpecParams::INDEFINITE)) {
    mOwner->AddInstanceTime(new SMILInstanceTime(mParams.mOffset), mIsBegin);
  }

  // Fill in the event symbol to simplify handling later
  if (mParams.mType == SMILTimeValueSpecParams::REPEAT) {
    mParams.mEventSymbol = nsGkAtoms::repeatEvent;
  }

  ResolveReferences(aContextElement);

  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
void nsSMILTimeValueSpec::ResolveReferences(Element& aContextElement) {
  if (mParams.mType != nsSMILTimeValueSpecParams::SYNCBASE && !IsEventBased()) {
||||||| merged common ancestors
void
nsSMILTimeValueSpec::ResolveReferences(Element& aContextElement)
{
  if (mParams.mType != nsSMILTimeValueSpecParams::SYNCBASE && !IsEventBased()) {
=======
void SMILTimeValueSpec::ResolveReferences(Element& aContextElement) {
  if (mParams.mType != SMILTimeValueSpecParams::SYNCBASE && !IsEventBased()) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
    return;
  }

  // If we're not bound to the document yet, don't worry, we'll get called again
  // when that happens
  if (!aContextElement.IsInComposedDoc()) return;

  // Hold ref to the old element so that it isn't destroyed in between resetting
  // the referenced element and using the pointer to update the referenced
  // element.
  RefPtr<Element> oldReferencedElement = mReferencedElement.get();

  if (mParams.mDependentElemID) {
    mReferencedElement.ResetWithID(aContextElement, mParams.mDependentElemID);
  } else if (mParams.mType == SMILTimeValueSpecParams::EVENT) {
    Element* target = mOwner->GetTargetElement();
    mReferencedElement.ResetWithElement(target);
  } else {
    MOZ_ASSERT(false, "Syncbase or repeat spec without ID");
  }
  UpdateReferencedElement(oldReferencedElement, mReferencedElement.get());
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
bool nsSMILTimeValueSpec::IsEventBased() const {
  return mParams.mType == nsSMILTimeValueSpecParams::EVENT ||
         mParams.mType == nsSMILTimeValueSpecParams::REPEAT;
||||||| merged common ancestors
bool
nsSMILTimeValueSpec::IsEventBased() const
{
  return mParams.mType == nsSMILTimeValueSpecParams::EVENT ||
         mParams.mType == nsSMILTimeValueSpecParams::REPEAT;
=======
bool SMILTimeValueSpec::IsEventBased() const {
  return mParams.mType == SMILTimeValueSpecParams::EVENT ||
         mParams.mType == SMILTimeValueSpecParams::REPEAT;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
void nsSMILTimeValueSpec::HandleNewInterval(
    nsSMILInterval& aInterval, const nsSMILTimeContainer* aSrcContainer) {
  const nsSMILInstanceTime& baseInstance =
      mParams.mSyncBegin ? *aInterval.Begin() : *aInterval.End();
  nsSMILTimeValue newTime =
      ConvertBetweenTimeContainers(baseInstance.Time(), aSrcContainer);
||||||| merged common ancestors
void
nsSMILTimeValueSpec::HandleNewInterval(nsSMILInterval& aInterval,
                                       const nsSMILTimeContainer* aSrcContainer)
{
  const nsSMILInstanceTime& baseInstance = mParams.mSyncBegin
    ? *aInterval.Begin() : *aInterval.End();
  nsSMILTimeValue newTime =
    ConvertBetweenTimeContainers(baseInstance.Time(), aSrcContainer);
=======
void SMILTimeValueSpec::HandleNewInterval(
    SMILInterval& aInterval, const SMILTimeContainer* aSrcContainer) {
  const SMILInstanceTime& baseInstance =
      mParams.mSyncBegin ? *aInterval.Begin() : *aInterval.End();
  SMILTimeValue newTime =
      ConvertBetweenTimeContainers(baseInstance.Time(), aSrcContainer);
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp

  // Apply offset
  if (!ApplyOffset(newTime)) {
    NS_WARNING("New time overflows SMILTime, ignoring");
    return;
  }

  // Create the instance time and register it with the interval
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
  RefPtr<nsSMILInstanceTime> newInstance = new nsSMILInstanceTime(
      newTime, nsSMILInstanceTime::SOURCE_SYNCBASE, this, &aInterval);
||||||| merged common ancestors
  RefPtr<nsSMILInstanceTime> newInstance =
    new nsSMILInstanceTime(newTime, nsSMILInstanceTime::SOURCE_SYNCBASE, this,
                           &aInterval);
=======
  RefPtr<SMILInstanceTime> newInstance = new SMILInstanceTime(
      newTime, SMILInstanceTime::SOURCE_SYNCBASE, this, &aInterval);
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  mOwner->AddInstanceTime(newInstance, mIsBegin);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
void nsSMILTimeValueSpec::HandleTargetElementChange(Element* aNewTarget) {
  if (!IsEventBased() || mParams.mDependentElemID) return;
||||||| merged common ancestors
void
nsSMILTimeValueSpec::HandleTargetElementChange(Element* aNewTarget)
{
  if (!IsEventBased() || mParams.mDependentElemID)
    return;
=======
void SMILTimeValueSpec::HandleTargetElementChange(Element* aNewTarget) {
  if (!IsEventBased() || mParams.mDependentElemID) return;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp

  mReferencedElement.ResetWithElement(aNewTarget);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
void nsSMILTimeValueSpec::HandleChangedInstanceTime(
    const nsSMILInstanceTime& aBaseTime,
    const nsSMILTimeContainer* aSrcContainer,
    nsSMILInstanceTime& aInstanceTimeToUpdate, bool aObjectChanged) {
||||||| merged common ancestors
void
nsSMILTimeValueSpec::HandleChangedInstanceTime(
    const nsSMILInstanceTime& aBaseTime,
    const nsSMILTimeContainer* aSrcContainer,
    nsSMILInstanceTime& aInstanceTimeToUpdate,
    bool aObjectChanged)
{
=======
void SMILTimeValueSpec::HandleChangedInstanceTime(
    const SMILInstanceTime& aBaseTime, const SMILTimeContainer* aSrcContainer,
    SMILInstanceTime& aInstanceTimeToUpdate, bool aObjectChanged) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  // If the instance time is fixed (e.g. because it's being used as the begin
  // time of an active or postactive interval) we just ignore the change.
  if (aInstanceTimeToUpdate.IsFixedTime()) return;

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
  nsSMILTimeValue updatedTime =
      ConvertBetweenTimeContainers(aBaseTime.Time(), aSrcContainer);
||||||| merged common ancestors
  nsSMILTimeValue updatedTime =
    ConvertBetweenTimeContainers(aBaseTime.Time(), aSrcContainer);
=======
  SMILTimeValue updatedTime =
      ConvertBetweenTimeContainers(aBaseTime.Time(), aSrcContainer);
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp

  // Apply offset
  if (!ApplyOffset(updatedTime)) {
    NS_WARNING("Updated time overflows SMILTime, ignoring");
    return;
  }

  // The timed element that owns the instance time does the updating so it can
  // re-sort its array of instance times more efficiently
  if (aInstanceTimeToUpdate.Time() != updatedTime || aObjectChanged) {
    mOwner->UpdateInstanceTime(&aInstanceTimeToUpdate, updatedTime, mIsBegin);
  }
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
void nsSMILTimeValueSpec::HandleDeletedInstanceTime(
    nsSMILInstanceTime& aInstanceTime) {
||||||| merged common ancestors
void
nsSMILTimeValueSpec::HandleDeletedInstanceTime(
    nsSMILInstanceTime &aInstanceTime)
{
=======
void SMILTimeValueSpec::HandleDeletedInstanceTime(
    SMILInstanceTime& aInstanceTime) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  mOwner->RemoveInstanceTime(&aInstanceTime, mIsBegin);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
bool nsSMILTimeValueSpec::DependsOnBegin() const { return mParams.mSyncBegin; }
||||||| merged common ancestors
bool
nsSMILTimeValueSpec::DependsOnBegin() const
{
  return mParams.mSyncBegin;
}
=======
bool SMILTimeValueSpec::DependsOnBegin() const { return mParams.mSyncBegin; }
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
void nsSMILTimeValueSpec::Traverse(
    nsCycleCollectionTraversalCallback* aCallback) {
||||||| merged common ancestors
void
nsSMILTimeValueSpec::Traverse(nsCycleCollectionTraversalCallback* aCallback)
{
=======
void SMILTimeValueSpec::Traverse(
    nsCycleCollectionTraversalCallback* aCallback) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  mReferencedElement.Traverse(aCallback);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
void nsSMILTimeValueSpec::Unlink() {
||||||| merged common ancestors
void
nsSMILTimeValueSpec::Unlink()
{
=======
void SMILTimeValueSpec::Unlink() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  UnregisterFromReferencedElement(mReferencedElement.get());
  mReferencedElement.Unlink();
}

//----------------------------------------------------------------------
// Implementation helpers

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
void nsSMILTimeValueSpec::UpdateReferencedElement(Element* aFrom,
                                                  Element* aTo) {
  if (aFrom == aTo) return;
||||||| merged common ancestors
void
nsSMILTimeValueSpec::UpdateReferencedElement(Element* aFrom, Element* aTo)
{
  if (aFrom == aTo)
    return;
=======
void SMILTimeValueSpec::UpdateReferencedElement(Element* aFrom, Element* aTo) {
  if (aFrom == aTo) return;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp

  UnregisterFromReferencedElement(aFrom);

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
  switch (mParams.mType) {
    case nsSMILTimeValueSpecParams::SYNCBASE: {
      nsSMILTimedElement* to = GetTimedElement(aTo);
||||||| merged common ancestors
  switch (mParams.mType)
  {
  case nsSMILTimeValueSpecParams::SYNCBASE:
    {
      nsSMILTimedElement* to = GetTimedElement(aTo);
=======
  switch (mParams.mType) {
    case SMILTimeValueSpecParams::SYNCBASE: {
      SMILTimedElement* to = GetTimedElement(aTo);
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
      if (to) {
        to->AddDependent(*this);
      }
    } break;

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
    case nsSMILTimeValueSpecParams::EVENT:
    case nsSMILTimeValueSpecParams::REPEAT:
      RegisterEventListener(aTo);
      break;
||||||| merged common ancestors
  case nsSMILTimeValueSpecParams::EVENT:
  case nsSMILTimeValueSpecParams::REPEAT:
    RegisterEventListener(aTo);
    break;
=======
    case SMILTimeValueSpecParams::EVENT:
    case SMILTimeValueSpecParams::REPEAT:
      RegisterEventListener(aTo);
      break;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp

    default:
      // not a referencing-type
      break;
  }
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
void nsSMILTimeValueSpec::UnregisterFromReferencedElement(Element* aElement) {
  if (!aElement) return;
||||||| merged common ancestors
void
nsSMILTimeValueSpec::UnregisterFromReferencedElement(Element* aElement)
{
  if (!aElement)
    return;
=======
void SMILTimeValueSpec::UnregisterFromReferencedElement(Element* aElement) {
  if (!aElement) return;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp

  if (mParams.mType == SMILTimeValueSpecParams::SYNCBASE) {
    SMILTimedElement* timedElement = GetTimedElement(aElement);
    if (timedElement) {
      timedElement->RemoveDependent(*this);
    }
    mOwner->RemoveInstanceTimesForCreator(this, mIsBegin);
  } else if (IsEventBased()) {
    UnregisterEventListener(aElement);
  }
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
nsSMILTimedElement* nsSMILTimeValueSpec::GetTimedElement(Element* aElement) {
  return aElement && aElement->IsNodeOfType(nsINode::eANIMATION)
             ? &static_cast<SVGAnimationElement*>(aElement)->TimedElement()
             : nullptr;
||||||| merged common ancestors
nsSMILTimedElement*
nsSMILTimeValueSpec::GetTimedElement(Element* aElement)
{
  return aElement && aElement->IsNodeOfType(nsINode::eANIMATION) ?
    &static_cast<SVGAnimationElement*>(aElement)->TimedElement() : nullptr;
=======
SMILTimedElement* SMILTimeValueSpec::GetTimedElement(Element* aElement) {
  return aElement && aElement->IsNodeOfType(nsINode::eANIMATION)
             ? &static_cast<SVGAnimationElement*>(aElement)->TimedElement()
             : nullptr;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
}

// Indicates whether we're allowed to register an event-listener
// when scripting is disabled.
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
bool nsSMILTimeValueSpec::IsWhitelistedEvent() {
||||||| merged common ancestors
bool
nsSMILTimeValueSpec::IsWhitelistedEvent()
{
=======
bool SMILTimeValueSpec::IsWhitelistedEvent() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  // The category of (SMIL-specific) "repeat(n)" events are allowed.
  if (mParams.mType == SMILTimeValueSpecParams::REPEAT) {
    return true;
  }

  // A specific list of other SMIL-related events are allowed, too.
  if (mParams.mType == SMILTimeValueSpecParams::EVENT &&
      (mParams.mEventSymbol == nsGkAtoms::repeat ||
       mParams.mEventSymbol == nsGkAtoms::repeatEvent ||
       mParams.mEventSymbol == nsGkAtoms::beginEvent ||
       mParams.mEventSymbol == nsGkAtoms::endEvent)) {
    return true;
  }

  return false;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
void nsSMILTimeValueSpec::RegisterEventListener(Element* aTarget) {
||||||| merged common ancestors
void
nsSMILTimeValueSpec::RegisterEventListener(Element* aTarget)
{
=======
void SMILTimeValueSpec::RegisterEventListener(Element* aTarget) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  MOZ_ASSERT(IsEventBased(),
             "Attempting to register event-listener for unexpected "
             "SMILTimeValueSpec type");
  MOZ_ASSERT(mParams.mEventSymbol,
             "Attempting to register event-listener but there is no event "
             "name");

  if (!aTarget) return;

  // When script is disabled, only allow registration for whitelisted events.
  if (!aTarget->GetOwnerDocument()->IsScriptEnabled() &&
      !IsWhitelistedEvent()) {
    return;
  }

  if (!mEventListener) {
    mEventListener = new EventListener(this);
  }

  EventListenerManager* elm = aTarget->GetOrCreateListenerManager();
  if (!elm) {
    return;
  }

  elm->AddEventListenerByType(mEventListener,
                              nsDependentAtomString(mParams.mEventSymbol),
                              AllEventsAtSystemGroupBubble());
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
void nsSMILTimeValueSpec::UnregisterEventListener(Element* aTarget) {
||||||| merged common ancestors
void
nsSMILTimeValueSpec::UnregisterEventListener(Element* aTarget)
{
=======
void SMILTimeValueSpec::UnregisterEventListener(Element* aTarget) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  if (!aTarget || !mEventListener) {
    return;
  }

  EventListenerManager* elm = aTarget->GetOrCreateListenerManager();
  if (!elm) {
    return;
  }

  elm->RemoveEventListenerByType(mEventListener,
                                 nsDependentAtomString(mParams.mEventSymbol),
                                 AllEventsAtSystemGroupBubble());
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
void nsSMILTimeValueSpec::HandleEvent(Event* aEvent) {
||||||| merged common ancestors
void
nsSMILTimeValueSpec::HandleEvent(Event* aEvent)
{
=======
void SMILTimeValueSpec::HandleEvent(Event* aEvent) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  MOZ_ASSERT(mEventListener, "Got event without an event listener");
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
  MOZ_ASSERT(IsEventBased(), "Got event for non-event nsSMILTimeValueSpec");
||||||| merged common ancestors
  MOZ_ASSERT(IsEventBased(),
             "Got event for non-event nsSMILTimeValueSpec");
=======
  MOZ_ASSERT(IsEventBased(), "Got event for non-event SMILTimeValueSpec");
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  MOZ_ASSERT(aEvent, "No event supplied");

  // XXX In the long run we should get the time from the event itself which will
  // store the time in global document time which we'll need to convert to our
  // time container
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
  nsSMILTimeContainer* container = mOwner->GetTimeContainer();
  if (!container) return;
||||||| merged common ancestors
  nsSMILTimeContainer* container = mOwner->GetTimeContainer();
  if (!container)
    return;
=======
  SMILTimeContainer* container = mOwner->GetTimeContainer();
  if (!container) return;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp

  if (mParams.mType == SMILTimeValueSpecParams::REPEAT &&
      !CheckRepeatEventDetail(aEvent)) {
    return;
  }

  SMILTime currentTime = container->GetCurrentTimeAsSMILTime();
  SMILTimeValue newTime(currentTime);
  if (!ApplyOffset(newTime)) {
    NS_WARNING("New time generated from event overflows SMILTime, ignoring");
    return;
  }

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
  RefPtr<nsSMILInstanceTime> newInstance =
      new nsSMILInstanceTime(newTime, nsSMILInstanceTime::SOURCE_EVENT);
||||||| merged common ancestors
  RefPtr<nsSMILInstanceTime> newInstance =
    new nsSMILInstanceTime(newTime, nsSMILInstanceTime::SOURCE_EVENT);
=======
  RefPtr<SMILInstanceTime> newInstance =
      new SMILInstanceTime(newTime, SMILInstanceTime::SOURCE_EVENT);
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  mOwner->AddInstanceTime(newInstance, mIsBegin);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
bool nsSMILTimeValueSpec::CheckRepeatEventDetail(Event* aEvent) {
||||||| merged common ancestors
bool
nsSMILTimeValueSpec::CheckRepeatEventDetail(Event *aEvent)
{
=======
bool SMILTimeValueSpec::CheckRepeatEventDetail(Event* aEvent) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  TimeEvent* timeEvent = aEvent->AsTimeEvent();
  if (!timeEvent) {
    NS_WARNING("Received a repeat event that was not a DOMTimeEvent");
    return false;
  }

  int32_t detail = timeEvent->Detail();
  return detail > 0 && (uint32_t)detail == mParams.mRepeatIteration;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
nsSMILTimeValue nsSMILTimeValueSpec::ConvertBetweenTimeContainers(
    const nsSMILTimeValue& aSrcTime, const nsSMILTimeContainer* aSrcContainer) {
||||||| merged common ancestors
nsSMILTimeValue
nsSMILTimeValueSpec::ConvertBetweenTimeContainers(
    const nsSMILTimeValue& aSrcTime,
    const nsSMILTimeContainer* aSrcContainer)
{
=======
SMILTimeValue SMILTimeValueSpec::ConvertBetweenTimeContainers(
    const SMILTimeValue& aSrcTime, const SMILTimeContainer* aSrcContainer) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  // If the source time is either indefinite or unresolved the result is going
  // to be the same
  if (!aSrcTime.IsDefinite()) return aSrcTime;

  // Convert from source time container to our parent time container
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
  const nsSMILTimeContainer* dstContainer = mOwner->GetTimeContainer();
  if (dstContainer == aSrcContainer) return aSrcTime;
||||||| merged common ancestors
  const nsSMILTimeContainer* dstContainer = mOwner->GetTimeContainer();
  if (dstContainer == aSrcContainer)
    return aSrcTime;
=======
  const SMILTimeContainer* dstContainer = mOwner->GetTimeContainer();
  if (dstContainer == aSrcContainer) return aSrcTime;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp

  // If one of the elements is not attached to a time container then we can't do
  // any meaningful conversion
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
  if (!aSrcContainer || !dstContainer) return nsSMILTimeValue();  // unresolved
||||||| merged common ancestors
  if (!aSrcContainer || !dstContainer)
    return nsSMILTimeValue(); // unresolved
=======
  if (!aSrcContainer || !dstContainer) return SMILTimeValue();  // unresolved
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
  nsSMILTimeValue docTime =
      aSrcContainer->ContainerToParentTime(aSrcTime.GetMillis());
||||||| merged common ancestors
  nsSMILTimeValue docTime =
    aSrcContainer->ContainerToParentTime(aSrcTime.GetMillis());
=======
  SMILTimeValue docTime =
      aSrcContainer->ContainerToParentTime(aSrcTime.GetMillis());
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp

  if (docTime.IsIndefinite())
    // This will happen if the source container is paused and we have a future
    // time. Just return the indefinite time.
    return docTime;

  MOZ_ASSERT(docTime.IsDefinite(),
             "ContainerToParentTime gave us an unresolved or indefinite time");

  return dstContainer->ParentToContainerTime(docTime.GetMillis());
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
bool nsSMILTimeValueSpec::ApplyOffset(nsSMILTimeValue& aTime) const {
||||||| merged common ancestors
bool
nsSMILTimeValueSpec::ApplyOffset(nsSMILTimeValue& aTime) const
{
=======
bool SMILTimeValueSpec::ApplyOffset(SMILTimeValue& aTime) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
  // indefinite + offset = indefinite. Likewise for unresolved times.
  if (!aTime.IsDefinite()) {
    return true;
  }

  double resultAsDouble =
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTimeValueSpec.cpp
      (double)aTime.GetMillis() + mParams.mOffset.GetMillis();
  if (resultAsDouble > std::numeric_limits<nsSMILTime>::max() ||
      resultAsDouble < std::numeric_limits<nsSMILTime>::min()) {
||||||| merged common ancestors
    (double)aTime.GetMillis() + mParams.mOffset.GetMillis();
  if (resultAsDouble > std::numeric_limits<nsSMILTime>::max() ||
      resultAsDouble < std::numeric_limits<nsSMILTime>::min()) {
=======
      (double)aTime.GetMillis() + mParams.mOffset.GetMillis();
  if (resultAsDouble > std::numeric_limits<SMILTime>::max() ||
      resultAsDouble < std::numeric_limits<SMILTime>::min()) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTimeValueSpec.cpp
    return false;
  }
  aTime.SetMillis(aTime.GetMillis() + mParams.mOffset.GetMillis());
  return true;
}

}  // namespace mozilla
