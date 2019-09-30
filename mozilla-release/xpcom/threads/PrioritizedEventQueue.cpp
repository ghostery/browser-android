/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "PrioritizedEventQueue.h"
#include "mozilla/EventQueue.h"
#include "mozilla/ScopeExit.h"
#include "mozilla/StaticPrefs.h"
#include "nsThreadManager.h"
#include "nsXPCOMPrivate.h"  // for gXPCOMThreadsShutDown
#include "InputEventStatistics.h"

using namespace mozilla;

<<<<<<< HEAD
template <class InnerQueueT>
PrioritizedEventQueue<InnerQueueT>::PrioritizedEventQueue(
    UniquePtr<InnerQueueT> aHighQueue, UniquePtr<InnerQueueT> aInputQueue,
    UniquePtr<InnerQueueT> aNormalQueue, UniquePtr<InnerQueueT> aIdleQueue,
    already_AddRefed<nsIIdlePeriod> aIdlePeriod)
    : mHighQueue(std::move(aHighQueue)),
      mInputQueue(std::move(aInputQueue)),
      mNormalQueue(std::move(aNormalQueue)),
      mIdleQueue(std::move(aIdleQueue)),
      mIdlePeriod(aIdlePeriod) {
  static_assert(IsBaseOf<AbstractEventQueue, InnerQueueT>::value,
                "InnerQueueT must be an AbstractEventQueue subclass");
}

template <class InnerQueueT>
void PrioritizedEventQueue<InnerQueueT>::PutEvent(
    already_AddRefed<nsIRunnable>&& aEvent, EventPriority aPriority,
    const MutexAutoLock& aProofOfLock) {
||||||| merged common ancestors
template<class InnerQueueT>
PrioritizedEventQueue<InnerQueueT>::PrioritizedEventQueue(UniquePtr<InnerQueueT> aHighQueue,
                                                          UniquePtr<InnerQueueT> aInputQueue,
                                                          UniquePtr<InnerQueueT> aNormalQueue,
                                                          UniquePtr<InnerQueueT> aIdleQueue,
                                                          already_AddRefed<nsIIdlePeriod> aIdlePeriod)
  : mHighQueue(std::move(aHighQueue))
  , mInputQueue(std::move(aInputQueue))
  , mNormalQueue(std::move(aNormalQueue))
  , mIdleQueue(std::move(aIdleQueue))
  , mIdlePeriod(aIdlePeriod)
{
  static_assert(IsBaseOf<AbstractEventQueue, InnerQueueT>::value,
                "InnerQueueT must be an AbstractEventQueue subclass");
}

template<class InnerQueueT>
void
PrioritizedEventQueue<InnerQueueT>::PutEvent(already_AddRefed<nsIRunnable>&& aEvent,
                                             EventPriority aPriority,
                                             const MutexAutoLock& aProofOfLock)
{
=======
void PrioritizedEventQueue::PutEvent(already_AddRefed<nsIRunnable>&& aEvent,
                                     EventQueuePriority aPriority,
                                     const MutexAutoLock& aProofOfLock) {
>>>>>>> upstream-releases
  // Double check the priority with a QI.
  RefPtr<nsIRunnable> event(aEvent);
  EventQueuePriority priority = aPriority;

  if (priority == EventQueuePriority::Input &&
      mInputQueueState == STATE_DISABLED) {
    priority = EventQueuePriority::Normal;
  } else if (priority == EventQueuePriority::MediumHigh &&
             !StaticPrefs::threads_medium_high_event_queue_enabled()) {
    priority = EventQueuePriority::Normal;
  }

  switch (priority) {
<<<<<<< HEAD
    case EventPriority::High:
      mHighQueue->PutEvent(event.forget(), priority, aProofOfLock);
      break;
    case EventPriority::Input:
      mInputQueue->PutEvent(event.forget(), priority, aProofOfLock);
      break;
    case EventPriority::Normal:
      mNormalQueue->PutEvent(event.forget(), priority, aProofOfLock);
      break;
    case EventPriority::Idle:
      mIdleQueue->PutEvent(event.forget(), priority, aProofOfLock);
      break;
    case EventPriority::Count:
      MOZ_CRASH("EventPriority::Count isn't a valid priority");
      break;
||||||| merged common ancestors
  case EventPriority::High:
    mHighQueue->PutEvent(event.forget(), priority, aProofOfLock);
    break;
  case EventPriority::Input:
    mInputQueue->PutEvent(event.forget(), priority, aProofOfLock);
    break;
  case EventPriority::Normal:
    mNormalQueue->PutEvent(event.forget(), priority, aProofOfLock);
    break;
  case EventPriority::Idle:
    mIdleQueue->PutEvent(event.forget(), priority, aProofOfLock);
    break;
  case EventPriority::Count:
    MOZ_CRASH("EventPriority::Count isn't a valid priority");
    break;
=======
    case EventQueuePriority::High:
      mHighQueue->PutEvent(event.forget(), priority, aProofOfLock);
      break;
    case EventQueuePriority::Input:
      mInputQueue->PutEvent(event.forget(), priority, aProofOfLock);
      break;
    case EventQueuePriority::MediumHigh:
      mMediumHighQueue->PutEvent(event.forget(), priority, aProofOfLock);
      break;
    case EventQueuePriority::Normal:
      mNormalQueue->PutEvent(event.forget(), priority, aProofOfLock);
      break;
    case EventQueuePriority::DeferredTimers:
      mDeferredTimersQueue->PutEvent(event.forget(), priority, aProofOfLock);
      break;
    case EventQueuePriority::Idle:
      mIdleQueue->PutEvent(event.forget(), priority, aProofOfLock);
      break;
    case EventQueuePriority::Count:
      MOZ_CRASH("EventQueuePriority::Count isn't a valid priority");
      break;
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
template <class InnerQueueT>
TimeStamp PrioritizedEventQueue<InnerQueueT>::GetIdleDeadline() {
||||||| merged common ancestors
template<class InnerQueueT>
TimeStamp
PrioritizedEventQueue<InnerQueueT>::GetIdleDeadline()
{
=======
TimeStamp PrioritizedEventQueue::GetIdleDeadline() {
>>>>>>> upstream-releases
  // If we are shutting down, we won't honor the idle period, and we will
  // always process idle runnables.  This will ensure that the idle queue
  // gets exhausted at shutdown time to prevent intermittently leaking
  // some runnables inside that queue and even worse potentially leaving
  // some important cleanup work unfinished.
  if (gXPCOMThreadsShutDown ||
      nsThreadManager::get().GetCurrentThread()->ShuttingDown()) {
    return TimeStamp::Now();
  }

  TimeStamp idleDeadline;
  {
    // Releasing the lock temporarily since getting the idle period
    // might need to lock the timer thread. Unlocking here might make
    // us receive an event on the main queue, but we've committed to
    // run an idle event anyhow.
    MutexAutoUnlock unlock(*mMutex);
    mIdlePeriod->GetIdlePeriodHint(&idleDeadline);
  }

  // If HasPendingEvents() has been called and it has returned true because of
  // pending idle events, there is a risk that we may decide here that we aren't
  // idle and return null, in which case HasPendingEvents() has effectively
  // lied.  Since we can't go back and fix the past, we have to adjust what we
  // do here and forcefully pick the idle queue task here.  Note that this means
  // that we are choosing to run a task from the idle queue when we would
  // normally decide that we aren't in an idle period, but this can only happen
  // if we fall out of the idle period in between the call to HasPendingEvents()
  // and here, which should hopefully be quite rare.  We are effectively
  // choosing to prioritize the sanity of our API semantics over the optimal
  // scheduling.
  if (!mHasPendingEventsPromisedIdleEvent &&
      (!idleDeadline || idleDeadline < TimeStamp::Now())) {
    return TimeStamp();
  }
  if (mHasPendingEventsPromisedIdleEvent && !idleDeadline) {
    // If HasPendingEvents() has been called and it has returned true, but we're
    // no longer in the idle period, we must return a valid timestamp to pretend
    // that we are still in the idle period.
    return TimeStamp::Now();
  }
  return idleDeadline;
}

<<<<<<< HEAD
template <class InnerQueueT>
EventPriority PrioritizedEventQueue<InnerQueueT>::SelectQueue(
    bool aUpdateState, const MutexAutoLock& aProofOfLock) {
  bool highPending = !mHighQueue->IsEmpty(aProofOfLock);
  bool normalPending = !mNormalQueue->IsEmpty(aProofOfLock);
||||||| merged common ancestors
template<class InnerQueueT>
EventPriority
PrioritizedEventQueue<InnerQueueT>::SelectQueue(bool aUpdateState,
                                                const MutexAutoLock& aProofOfLock)
{
  bool highPending = !mHighQueue->IsEmpty(aProofOfLock);
  bool normalPending = !mNormalQueue->IsEmpty(aProofOfLock);
=======
EventQueuePriority PrioritizedEventQueue::SelectQueue(
    bool aUpdateState, const MutexAutoLock& aProofOfLock) {
>>>>>>> upstream-releases
  size_t inputCount = mInputQueue->Count(aProofOfLock);

  if (aUpdateState && mInputQueueState == STATE_ENABLED &&
      mInputHandlingStartTime.IsNull() && inputCount > 0) {
    mInputHandlingStartTime =
        InputEventStatistics::Get().GetInputHandlingStartTime(inputCount);
  }

  // We check the different queues in the following order. The conditions we use
  // are meant to avoid starvation and to ensure that we don't process an event
  // at the wrong time.
  //
  // HIGH: if mProcessHighPriorityQueue
  // INPUT: if inputCount > 0 && TimeStamp::Now() > mInputHandlingStartTime
  // MEDIUMHIGH: if medium high pending
  // NORMAL: if normal pending
  //
  // If we still don't have an event, then we take events from the queues
  // in the following order:
  //
  // HIGH
  // INPUT
  // DEFERREDTIMERS: if GetIdleDeadline()
  // IDLE: if GetIdleDeadline()
  //
  // If we don't get an event in this pass, then we return null since no events
  // are ready.

  // This variable determines which queue we will take an event from.
  EventQueuePriority queue;
  bool highPending = !mHighQueue->IsEmpty(aProofOfLock);

  if (mProcessHighPriorityQueue) {
    queue = EventQueuePriority::High;
  } else if (inputCount > 0 && (mInputQueueState == STATE_FLUSHING ||
                                (mInputQueueState == STATE_ENABLED &&
                                 !mInputHandlingStartTime.IsNull() &&
                                 TimeStamp::Now() > mInputHandlingStartTime))) {
    queue = EventQueuePriority::Input;
  } else if (!mMediumHighQueue->IsEmpty(aProofOfLock)) {
    MOZ_ASSERT(
        mInputQueueState != STATE_FLUSHING,
        "Shouldn't consume medium high event when flushing input events");
    queue = EventQueuePriority::MediumHigh;
  } else if (!mNormalQueue->IsEmpty(aProofOfLock)) {
    MOZ_ASSERT(mInputQueueState != STATE_FLUSHING,
               "Shouldn't consume normal event when flushing input events");
    queue = EventQueuePriority::Normal;
  } else if (highPending) {
    queue = EventQueuePriority::High;
  } else if (inputCount > 0 && mInputQueueState != STATE_SUSPEND) {
<<<<<<< HEAD
    MOZ_ASSERT(
        mInputQueueState != STATE_DISABLED,
        "Shouldn't consume input events when the input queue is disabled");
    queue = EventPriority::Input;
||||||| merged common ancestors
    MOZ_ASSERT(mInputQueueState != STATE_DISABLED,
               "Shouldn't consume input events when the input queue is disabled");
    queue = EventPriority::Input;
=======
    MOZ_ASSERT(
        mInputQueueState != STATE_DISABLED,
        "Shouldn't consume input events when the input queue is disabled");
    queue = EventQueuePriority::Input;
  } else if (!mDeferredTimersQueue->IsEmpty(aProofOfLock)) {
    // We may not actually return an idle event in this case.
    queue = EventQueuePriority::DeferredTimers;
>>>>>>> upstream-releases
  } else {
    // We may not actually return an idle event in this case.
    queue = EventQueuePriority::Idle;
  }

<<<<<<< HEAD
  MOZ_ASSERT_IF(
      queue == EventPriority::Input,
      mInputQueueState != STATE_DISABLED && mInputQueueState != STATE_SUSPEND);
||||||| merged common ancestors
  MOZ_ASSERT_IF(queue == EventPriority::Input,
                mInputQueueState != STATE_DISABLED && mInputQueueState != STATE_SUSPEND);
=======
  MOZ_ASSERT_IF(
      queue == EventQueuePriority::Input,
      mInputQueueState != STATE_DISABLED && mInputQueueState != STATE_SUSPEND);
>>>>>>> upstream-releases

  if (aUpdateState) {
    mProcessHighPriorityQueue = highPending;
  }

  return queue;
}

<<<<<<< HEAD
template <class InnerQueueT>
already_AddRefed<nsIRunnable> PrioritizedEventQueue<InnerQueueT>::GetEvent(
    EventPriority* aPriority, const MutexAutoLock& aProofOfLock) {
  auto guard =
      MakeScopeExit([&] { mHasPendingEventsPromisedIdleEvent = false; });
||||||| merged common ancestors
template<class InnerQueueT>
already_AddRefed<nsIRunnable>
PrioritizedEventQueue<InnerQueueT>::GetEvent(EventPriority* aPriority,
                                             const MutexAutoLock& aProofOfLock)
{
  auto guard = MakeScopeExit([&] {
    mHasPendingEventsPromisedIdleEvent = false;
  });
=======
already_AddRefed<nsIRunnable> PrioritizedEventQueue::GetEvent(
    EventQueuePriority* aPriority, const MutexAutoLock& aProofOfLock) {
  auto guard =
      MakeScopeExit([&] { mHasPendingEventsPromisedIdleEvent = false; });
>>>>>>> upstream-releases

#ifndef RELEASE_OR_BETA
  // Clear mNextIdleDeadline so that it is possible to determine that
  // we're running an idle runnable in ProcessNextEvent.
  *mNextIdleDeadline = TimeStamp();
#endif

  EventQueuePriority queue = SelectQueue(true, aProofOfLock);

  if (aPriority) {
    *aPriority = queue;
  }

  if (queue == EventQueuePriority::High) {
    nsCOMPtr<nsIRunnable> event = mHighQueue->GetEvent(aPriority, aProofOfLock);
    MOZ_ASSERT(event);
    mInputHandlingStartTime = TimeStamp();
    mProcessHighPriorityQueue = false;
    return event.forget();
  }

<<<<<<< HEAD
  if (queue == EventPriority::Input) {
    nsCOMPtr<nsIRunnable> event =
        mInputQueue->GetEvent(aPriority, aProofOfLock);
||||||| merged common ancestors
  if (queue == EventPriority::Input) {
    nsCOMPtr<nsIRunnable> event = mInputQueue->GetEvent(aPriority, aProofOfLock);
=======
  if (queue == EventQueuePriority::Input) {
    nsCOMPtr<nsIRunnable> event =
        mInputQueue->GetEvent(aPriority, aProofOfLock);
>>>>>>> upstream-releases
    MOZ_ASSERT(event);
    return event.forget();
  }

<<<<<<< HEAD
  if (queue == EventPriority::Normal) {
    nsCOMPtr<nsIRunnable> event =
        mNormalQueue->GetEvent(aPriority, aProofOfLock);
||||||| merged common ancestors
  if (queue == EventPriority::Normal) {
    nsCOMPtr<nsIRunnable> event = mNormalQueue->GetEvent(aPriority, aProofOfLock);
=======
  if (queue == EventQueuePriority::MediumHigh) {
    nsCOMPtr<nsIRunnable> event =
        mMediumHighQueue->GetEvent(aPriority, aProofOfLock);
    return event.forget();
  }

  if (queue == EventQueuePriority::Normal) {
    nsCOMPtr<nsIRunnable> event =
        mNormalQueue->GetEvent(aPriority, aProofOfLock);
>>>>>>> upstream-releases
    return event.forget();
  }

  // If we get here, then all queues except deferredtimers and idle are empty.
  MOZ_ASSERT(queue == EventQueuePriority::Idle ||
             queue == EventQueuePriority::DeferredTimers);

  if (mIdleQueue->IsEmpty(aProofOfLock) &&
      mDeferredTimersQueue->IsEmpty(aProofOfLock)) {
    MOZ_ASSERT(!mHasPendingEventsPromisedIdleEvent);
    return nullptr;
  }

  TimeStamp idleDeadline = GetIdleDeadline();
  if (!idleDeadline) {
    return nullptr;
  }

  nsCOMPtr<nsIRunnable> event =
      mDeferredTimersQueue->GetEvent(aPriority, aProofOfLock);
  if (!event) {
    event = mIdleQueue->GetEvent(aPriority, aProofOfLock);
  }
  if (event) {
    nsCOMPtr<nsIIdleRunnable> idleEvent = do_QueryInterface(event);
    if (idleEvent) {
      idleEvent->SetDeadline(idleDeadline);
    }

#ifndef RELEASE_OR_BETA
    // Store the next idle deadline to be able to determine budget use
    // in ProcessNextEvent.
    *mNextIdleDeadline = idleDeadline;
#endif
  }

  return event.forget();
}

<<<<<<< HEAD
template <class InnerQueueT>
bool PrioritizedEventQueue<InnerQueueT>::IsEmpty(
    const MutexAutoLock& aProofOfLock) {
||||||| merged common ancestors
template<class InnerQueueT>
bool
PrioritizedEventQueue<InnerQueueT>::IsEmpty(const MutexAutoLock& aProofOfLock)
{
=======
bool PrioritizedEventQueue::IsEmpty(const MutexAutoLock& aProofOfLock) {
>>>>>>> upstream-releases
  // Just check IsEmpty() on the sub-queues. Don't bother checking the idle
  // deadline since that only determines whether an idle event is ready or not.
<<<<<<< HEAD
  return mHighQueue->IsEmpty(aProofOfLock) &&
         mInputQueue->IsEmpty(aProofOfLock) &&
         mNormalQueue->IsEmpty(aProofOfLock) &&
         mIdleQueue->IsEmpty(aProofOfLock);
||||||| merged common ancestors
  return mHighQueue->IsEmpty(aProofOfLock)
      && mInputQueue->IsEmpty(aProofOfLock)
      && mNormalQueue->IsEmpty(aProofOfLock)
      && mIdleQueue->IsEmpty(aProofOfLock);
=======
  return mHighQueue->IsEmpty(aProofOfLock) &&
         mInputQueue->IsEmpty(aProofOfLock) &&
         mMediumHighQueue->IsEmpty(aProofOfLock) &&
         mNormalQueue->IsEmpty(aProofOfLock) &&
         mDeferredTimersQueue->IsEmpty(aProofOfLock) &&
         mIdleQueue->IsEmpty(aProofOfLock);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <class InnerQueueT>
bool PrioritizedEventQueue<InnerQueueT>::HasReadyEvent(
    const MutexAutoLock& aProofOfLock) {
||||||| merged common ancestors
template<class InnerQueueT>
bool
PrioritizedEventQueue<InnerQueueT>::HasReadyEvent(const MutexAutoLock& aProofOfLock)
{
=======
bool PrioritizedEventQueue::HasReadyEvent(const MutexAutoLock& aProofOfLock) {
>>>>>>> upstream-releases
  mHasPendingEventsPromisedIdleEvent = false;

  EventQueuePriority queue = SelectQueue(false, aProofOfLock);

  if (queue == EventQueuePriority::High) {
    return mHighQueue->HasReadyEvent(aProofOfLock);
  } else if (queue == EventQueuePriority::Input) {
    return mInputQueue->HasReadyEvent(aProofOfLock);
  } else if (queue == EventQueuePriority::MediumHigh) {
    return mMediumHighQueue->HasReadyEvent(aProofOfLock);
  } else if (queue == EventQueuePriority::Normal) {
    return mNormalQueue->HasReadyEvent(aProofOfLock);
  }

  MOZ_ASSERT(queue == EventQueuePriority::Idle ||
             queue == EventQueuePriority::DeferredTimers);

  // If we get here, then both the high and normal queues are empty.

  if (mDeferredTimersQueue->IsEmpty(aProofOfLock) &&
      mIdleQueue->IsEmpty(aProofOfLock)) {
    return false;
  }

  TimeStamp idleDeadline = GetIdleDeadline();
  if (idleDeadline && (mDeferredTimersQueue->HasReadyEvent(aProofOfLock) ||
                       mIdleQueue->HasReadyEvent(aProofOfLock))) {
    mHasPendingEventsPromisedIdleEvent = true;
    return true;
  }

  return false;
}

<<<<<<< HEAD
template <class InnerQueueT>
size_t PrioritizedEventQueue<InnerQueueT>::Count(
    const MutexAutoLock& aProofOfLock) const {
||||||| merged common ancestors
template<class InnerQueueT>
size_t
PrioritizedEventQueue<InnerQueueT>::Count(const MutexAutoLock& aProofOfLock) const
{
=======
bool PrioritizedEventQueue::HasPendingHighPriorityEvents(
    const MutexAutoLock& aProofOfLock) {
  return !mHighQueue->IsEmpty(aProofOfLock);
}

size_t PrioritizedEventQueue::Count(const MutexAutoLock& aProofOfLock) const {
>>>>>>> upstream-releases
  MOZ_CRASH("unimplemented");
}

<<<<<<< HEAD
template <class InnerQueueT>
void PrioritizedEventQueue<InnerQueueT>::EnableInputEventPrioritization(
    const MutexAutoLock& aProofOfLock) {
||||||| merged common ancestors
template<class InnerQueueT>
void
PrioritizedEventQueue<InnerQueueT>::EnableInputEventPrioritization(const MutexAutoLock& aProofOfLock)
{
=======
void PrioritizedEventQueue::EnableInputEventPrioritization(
    const MutexAutoLock& aProofOfLock) {
>>>>>>> upstream-releases
  MOZ_ASSERT(mInputQueueState == STATE_DISABLED);
  mInputQueueState = STATE_ENABLED;
  mInputHandlingStartTime = TimeStamp();
}

<<<<<<< HEAD
template <class InnerQueueT>
void PrioritizedEventQueue<InnerQueueT>::FlushInputEventPrioritization(
    const MutexAutoLock& aProofOfLock) {
  MOZ_ASSERT(mInputQueueState == STATE_ENABLED ||
             mInputQueueState == STATE_SUSPEND);
||||||| merged common ancestors
template<class InnerQueueT>
void
PrioritizedEventQueue<InnerQueueT>::
FlushInputEventPrioritization(const MutexAutoLock& aProofOfLock)
{
  MOZ_ASSERT(mInputQueueState == STATE_ENABLED || mInputQueueState == STATE_SUSPEND);
=======
void PrioritizedEventQueue::FlushInputEventPrioritization(
    const MutexAutoLock& aProofOfLock) {
  MOZ_ASSERT(mInputQueueState == STATE_ENABLED ||
             mInputQueueState == STATE_SUSPEND);
>>>>>>> upstream-releases
  mInputQueueState =
      mInputQueueState == STATE_ENABLED ? STATE_FLUSHING : STATE_SUSPEND;
}

<<<<<<< HEAD
template <class InnerQueueT>
void PrioritizedEventQueue<InnerQueueT>::SuspendInputEventPrioritization(
    const MutexAutoLock& aProofOfLock) {
  MOZ_ASSERT(mInputQueueState == STATE_ENABLED ||
             mInputQueueState == STATE_FLUSHING);
||||||| merged common ancestors
template<class InnerQueueT>
void
PrioritizedEventQueue<InnerQueueT>::
SuspendInputEventPrioritization(const MutexAutoLock& aProofOfLock)
{
  MOZ_ASSERT(mInputQueueState == STATE_ENABLED || mInputQueueState == STATE_FLUSHING);
=======
void PrioritizedEventQueue::SuspendInputEventPrioritization(
    const MutexAutoLock& aProofOfLock) {
  MOZ_ASSERT(mInputQueueState == STATE_ENABLED ||
             mInputQueueState == STATE_FLUSHING);
>>>>>>> upstream-releases
  mInputQueueState = STATE_SUSPEND;
}

<<<<<<< HEAD
template <class InnerQueueT>
void PrioritizedEventQueue<InnerQueueT>::ResumeInputEventPrioritization(
    const MutexAutoLock& aProofOfLock) {
||||||| merged common ancestors
template<class InnerQueueT>
void
PrioritizedEventQueue<InnerQueueT>::
ResumeInputEventPrioritization(const MutexAutoLock& aProofOfLock)
{
=======
void PrioritizedEventQueue::ResumeInputEventPrioritization(
    const MutexAutoLock& aProofOfLock) {
>>>>>>> upstream-releases
  MOZ_ASSERT(mInputQueueState == STATE_SUSPEND);
  mInputQueueState = STATE_ENABLED;
}
<<<<<<< HEAD

namespace mozilla {
template class PrioritizedEventQueue<EventQueue>;
template class PrioritizedEventQueue<LabeledEventQueue>;
}  // namespace mozilla
||||||| merged common ancestors

namespace mozilla {
template class PrioritizedEventQueue<EventQueue>;
template class PrioritizedEventQueue<LabeledEventQueue>;
}
=======
>>>>>>> upstream-releases
