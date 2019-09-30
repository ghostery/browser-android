/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_TimeoutManager_h__
#define mozilla_dom_TimeoutManager_h__

#include "mozilla/dom/Timeout.h"
#include "nsTArray.h"

class nsIEventTarget;
class nsITimer;
class nsGlobalWindowInner;

namespace mozilla {

class PerformanceCounter;

namespace dom {

class TimeoutExecutor;
class TimeoutHandler;

// This class manages the timeouts in a Window's setTimeout/setInterval pool.
<<<<<<< HEAD
class TimeoutManager final {
 public:
  explicit TimeoutManager(nsGlobalWindowInner& aWindow);
||||||| merged common ancestors
class TimeoutManager final
{
public:
  explicit TimeoutManager(nsGlobalWindowInner& aWindow);
=======
class TimeoutManager final {
 private:
  struct Timeouts;

 public:
  TimeoutManager(nsGlobalWindowInner& aWindow, uint32_t aMaxIdleDeferMS);
>>>>>>> upstream-releases
  ~TimeoutManager();
  TimeoutManager(const TimeoutManager& rhs) = delete;
  void operator=(const TimeoutManager& rhs) = delete;

  bool IsRunningTimeout() const;

  static uint32_t GetNestingLevel() { return sNestingLevel; }
  static void SetNestingLevel(uint32_t aLevel) { sNestingLevel = aLevel; }

<<<<<<< HEAD
  bool HasTimeouts() const { return !mTimeouts.IsEmpty(); }
||||||| merged common ancestors
  bool HasTimeouts() const
  {
    return !mTimeouts.IsEmpty();
  }
=======
  bool HasTimeouts() const {
    return !mTimeouts.IsEmpty() || !mIdleTimeouts.IsEmpty();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult SetTimeout(nsITimeoutHandler* aHandler, int32_t interval,
                      bool aIsInterval, mozilla::dom::Timeout::Reason aReason,
||||||| merged common ancestors
  nsresult SetTimeout(nsITimeoutHandler* aHandler,
                      int32_t interval, bool aIsInterval,
                      mozilla::dom::Timeout::Reason aReason,
=======
  nsresult SetTimeout(TimeoutHandler* aHandler, int32_t interval,
                      bool aIsInterval, mozilla::dom::Timeout::Reason aReason,
>>>>>>> upstream-releases
                      int32_t* aReturn);
<<<<<<< HEAD
  void ClearTimeout(int32_t aTimerId, mozilla::dom::Timeout::Reason aReason);
||||||| merged common ancestors
  void ClearTimeout(int32_t aTimerId,
                    mozilla::dom::Timeout::Reason aReason);
=======
  void ClearTimeout(int32_t aTimerId, mozilla::dom::Timeout::Reason aReason);
  bool ClearTimeoutInternal(int32_t aTimerId,
                            mozilla::dom::Timeout::Reason aReason,
                            bool aIsIdle);
>>>>>>> upstream-releases

  // The timeout implementation functions.
  MOZ_CAN_RUN_SCRIPT
  void RunTimeout(const TimeStamp& aNow, const TimeStamp& aTargetDeadline,
                  bool aProcessIdle);

  void ClearAllTimeouts();
  uint32_t GetTimeoutId(mozilla::dom::Timeout::Reason aReason);

  TimeDuration CalculateDelay(Timeout* aTimeout) const;

  // aTimeout is the timeout that we're about to start running.  This function
  // returns the current timeout.
  mozilla::dom::Timeout* BeginRunningTimeout(mozilla::dom::Timeout* aTimeout);
  // aTimeout is the last running timeout.
  void EndRunningTimeout(mozilla::dom::Timeout* aTimeout);

  void UnmarkGrayTimers();

  // These four methods are intended to be called from the corresponding methods
  // on nsGlobalWindow.
  void Suspend();
  void Resume();
  void Freeze();
  void Thaw();

  // This should be called by nsGlobalWindow when the window might have moved
  // to the background or foreground.
  void UpdateBackgroundState();

  // Initialize TimeoutManager before the first time it is accessed.
  static void Initialize();

  // The document finished loading
  void OnDocumentLoaded();
  void StartThrottlingTimeouts();

  // Run some code for each Timeout in our list.  Note that this function
  // doesn't guarantee that Timeouts are iterated in any particular order.
  template <class Callable>
<<<<<<< HEAD
  void ForEachUnorderedTimeout(Callable c) {
||||||| merged common ancestors
  void ForEachUnorderedTimeout(Callable c)
  {
=======
  void ForEachUnorderedTimeout(Callable c) {
    mIdleTimeouts.ForEach(c);
>>>>>>> upstream-releases
    mTimeouts.ForEach(c);
  }

  void BeginSyncOperation();
  void EndSyncOperation();

  nsIEventTarget* EventTarget();

  bool BudgetThrottlingEnabled(bool aIsBackground) const;

  static const uint32_t InvalidFiringId;

<<<<<<< HEAD
 private:
||||||| merged common ancestors
private:
=======
  void SetLoading(bool value);

 private:
>>>>>>> upstream-releases
  void MaybeStartThrottleTimeout();

  // Return true if |aTimeout| needs to be reinserted into the timeout list.
  bool RescheduleTimeout(mozilla::dom::Timeout* aTimeout,
                         const TimeStamp& aLastCallbackTime,
                         const TimeStamp& aCurrentNow);

  void MoveIdleToActive();

  bool IsBackground() const;

  bool IsActive() const;

  uint32_t CreateFiringId();

  void DestroyFiringId(uint32_t aFiringId);

  bool IsValidFiringId(uint32_t aFiringId) const;

  bool IsInvalidFiringId(uint32_t aFiringId) const;

  TimeDuration MinSchedulingDelay() const;

  nsresult MaybeSchedule(const TimeStamp& aWhen,
                         const TimeStamp& aNow = TimeStamp::Now());

  void RecordExecution(Timeout* aRunningTimeout, Timeout* aTimeout);

  void UpdateBudget(const TimeStamp& aNow,
                    const TimeDuration& aDuration = TimeDuration());

  mozilla::PerformanceCounter* GetPerformanceCounter();

 private:
  struct Timeouts {
    explicit Timeouts(const TimeoutManager& aManager) : mManager(aManager) {}

    // Insert aTimeout into the list, before all timeouts that would
    // fire after it, but no earlier than the last Timeout with a
    // valid FiringId.
    enum class SortBy { TimeRemaining, TimeWhen };
    void Insert(mozilla::dom::Timeout* aTimeout, SortBy aSortBy);

    const Timeout* GetFirst() const { return mTimeoutList.getFirst(); }
    Timeout* GetFirst() { return mTimeoutList.getFirst(); }
    const Timeout* GetLast() const { return mTimeoutList.getLast(); }
    Timeout* GetLast() { return mTimeoutList.getLast(); }
    bool IsEmpty() const { return mTimeoutList.isEmpty(); }
    void InsertFront(Timeout* aTimeout) { mTimeoutList.insertFront(aTimeout); }
    void InsertBack(Timeout* aTimeout) { mTimeoutList.insertBack(aTimeout); }
    void Clear() { mTimeoutList.clear(); }

    template <class Callable>
    void ForEach(Callable c) {
      for (Timeout* timeout = GetFirst(); timeout;
           timeout = timeout->getNext()) {
        c(timeout);
      }
    }

    // Returns true when a callback aborts iteration.
    template <class Callable>
    bool ForEachAbortable(Callable c) {
      for (Timeout* timeout = GetFirst(); timeout;
           timeout = timeout->getNext()) {
        if (c(timeout)) {
          return true;
        }
      }
      return false;
    }

   private:
    // The TimeoutManager that owns this Timeouts structure.  This is
    // mainly used to call state inspecting methods like IsValidFiringId().
    const TimeoutManager& mManager;

    typedef mozilla::LinkedList<RefPtr<Timeout>> TimeoutList;

    // mTimeoutList is generally sorted by mWhen, but new values are always
    // inserted after any Timeouts with a valid FiringId.
    TimeoutList mTimeoutList;
  };

  // Each nsGlobalWindowInner object has a TimeoutManager member.  This
  // reference points to that holder object.
  nsGlobalWindowInner& mWindow;
  // The executor is specific to the nsGlobalWindow/TimeoutManager, but it
  // can live past the destruction of the window if its scheduled.  Therefore
  // it must be a separate ref-counted object.
<<<<<<< HEAD
  RefPtr<TimeoutExecutor> mExecutor;
||||||| merged common ancestors
  RefPtr<TimeoutExecutor>     mExecutor;
=======
  RefPtr<TimeoutExecutor> mExecutor;
  // For timeouts run off the idle queue
  RefPtr<TimeoutExecutor> mIdleExecutor;
>>>>>>> upstream-releases
  // The list of timeouts coming from non-tracking scripts.
<<<<<<< HEAD
  Timeouts mTimeouts;
  uint32_t mTimeoutIdCounter;
  uint32_t mNextFiringId;
  AutoTArray<uint32_t, 2> mFiringIdStack;
  mozilla::dom::Timeout* mRunningTimeout;
||||||| merged common ancestors
  Timeouts                    mTimeouts;
  uint32_t                    mTimeoutIdCounter;
  uint32_t                    mNextFiringId;
  AutoTArray<uint32_t, 2>     mFiringIdStack;
  mozilla::dom::Timeout*      mRunningTimeout;
=======
  Timeouts mTimeouts;
  uint32_t mTimeoutIdCounter;
  uint32_t mNextFiringId;
#ifdef DEBUG
  int64_t mFiringIndex;
  int64_t mLastFiringIndex;
#endif
  AutoTArray<uint32_t, 2> mFiringIdStack;
  mozilla::dom::Timeout* mRunningTimeout;

  // Timeouts that would have fired but are being deferred until MainThread
  // is idle (because we're loading)
  Timeouts mIdleTimeouts;

  // The current idle request callback timeout handle
  uint32_t mIdleCallbackTimeoutCounter;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The current idle request callback timeout handle
  uint32_t mIdleCallbackTimeoutCounter;
||||||| merged common ancestors
   // The current idle request callback timeout handle
  uint32_t                    mIdleCallbackTimeoutCounter;
=======
  nsCOMPtr<nsITimer> mThrottleTimeoutsTimer;
  mozilla::TimeStamp mLastBudgetUpdate;
  mozilla::TimeDuration mExecutionBudget;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsITimer> mThrottleTimeoutsTimer;
  mozilla::TimeStamp mLastBudgetUpdate;
  mozilla::TimeDuration mExecutionBudget;
||||||| merged common ancestors
  nsCOMPtr<nsITimer>          mThrottleTimeoutsTimer;
  mozilla::TimeStamp          mLastBudgetUpdate;
  mozilla::TimeDuration       mExecutionBudget;
=======
  bool mThrottleTimeouts;
  bool mThrottleTrackingTimeouts;
  bool mBudgetThrottleTimeouts;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool mThrottleTimeouts;
  bool mThrottleTrackingTimeouts;
  bool mBudgetThrottleTimeouts;
||||||| merged common ancestors
  bool                        mThrottleTimeouts;
  bool                        mThrottleTrackingTimeouts;
  bool                        mBudgetThrottleTimeouts;
=======
  bool mIsLoading;
>>>>>>> upstream-releases

  static uint32_t sNestingLevel;
};

}  // namespace dom
}  // namespace mozilla

#endif
