/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_worklet_WorkletThread_h
#define mozilla_dom_worklet_WorkletThread_h

#include "mozilla/Attributes.h"
#include "mozilla/CondVar.h"
#include "mozilla/RefPtr.h"
#include "mozilla/UniquePtr.h"
<<<<<<< HEAD
||||||| merged common ancestors
#include "mozilla/TimeStamp.h"
=======
#include "mozilla/dom/WorkletImpl.h"
>>>>>>> upstream-releases
#include "nsThread.h"

class nsIRunnable;

namespace mozilla {
namespace dom {

class WorkletThread final : public nsThread, public nsIObserver {
 public:
  NS_DECL_ISUPPORTS_INHERITED
  NS_DECL_NSIOBSERVER

  static already_AddRefed<WorkletThread> Create(WorkletImpl* aWorkletImpl);

<<<<<<< HEAD
  static bool IsOnWorkletThread();
||||||| merged common ancestors
  static WorkletThread*
  Get();
=======
  // Threads that call EnsureCycleCollectedJSContext must call
  // DeleteCycleCollectedJSContext::Get() before terminating.  Clients of
  // Create() do not need to do this as Terminate() will ensure this happens.
  static void EnsureCycleCollectedJSContext(JSRuntime* aParentRuntime);
  static void DeleteCycleCollectedJSContext();
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void AssertIsOnWorkletThread();
||||||| merged common ancestors
  static bool
  IsOnWorkletThread();
=======
  static bool IsOnWorkletThread();
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult DispatchRunnable(already_AddRefed<nsIRunnable> aRunnable);
||||||| merged common ancestors
  static void
  AssertIsOnWorkletThread();
=======
  static void AssertIsOnWorkletThread();
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Terminate();
||||||| merged common ancestors
  JSContext*
  GetJSContext() const;
=======
  nsresult DispatchRunnable(already_AddRefed<nsIRunnable> aRunnable);
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  WorkletThread();
||||||| merged common ancestors
  nsresult
  DispatchRunnable(already_AddRefed<nsIRunnable> aRunnable);

  void
  Terminate();

  DOMHighResTimeStamp
  TimeStampToDOMHighRes(const TimeStamp& aTimeStamp) const
  {
    MOZ_ASSERT(!aTimeStamp.IsNull());
    TimeDuration duration = aTimeStamp - mCreationTimeStamp;
    return duration.ToMilliseconds();
  }

private:
  WorkletThread();
=======
  void Terminate();

 private:
  explicit WorkletThread(WorkletImpl* aWorkletImpl);
>>>>>>> upstream-releases
  ~WorkletThread();

<<<<<<< HEAD
  void RunEventLoop(JSRuntime* aParentRuntime);
||||||| merged common ancestors
  void
  RunEventLoop(JSRuntime* aParentRuntime);
=======
  void RunEventLoop();
>>>>>>> upstream-releases
  class PrimaryRunnable;

  void TerminateInternal();
  class TerminateRunnable;

  // This should only be called by consumers that have an
  // nsIEventTarget/nsIThread pointer.
  NS_IMETHOD
  Dispatch(already_AddRefed<nsIRunnable> aRunnable, uint32_t aFlags) override;

  NS_IMETHOD
  DispatchFromScript(nsIRunnable* aRunnable, uint32_t aFlags) override;

  NS_IMETHOD
  DelayedDispatch(already_AddRefed<nsIRunnable>, uint32_t) override;

<<<<<<< HEAD
  bool mExitLoop;  // worklet execution thread
||||||| merged common ancestors
  TimeStamp mCreationTimeStamp;

  // Touched only on the worklet thread. This is a raw pointer because it's set
  // and nullified by RunEventLoop().
  JSContext* mJSContext;
=======
  const RefPtr<WorkletImpl> mWorkletImpl;

  bool mExitLoop;  // worklet execution thread
>>>>>>> upstream-releases

  bool mIsTerminating;  // main thread
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_worklet_WorkletThread_h
