/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/layers/APZThreadUtils.h"

namespace mozilla {
namespace layers {

static bool sThreadAssertionsEnabled = true;
static MessageLoop* sControllerThread;

<<<<<<< HEAD
/*static*/ void APZThreadUtils::SetThreadAssertionsEnabled(bool aEnabled) {
||||||| merged common ancestors
/*static*/ void
APZThreadUtils::SetThreadAssertionsEnabled(bool aEnabled) {
=======
/*static*/
void APZThreadUtils::SetThreadAssertionsEnabled(bool aEnabled) {
>>>>>>> upstream-releases
  sThreadAssertionsEnabled = aEnabled;
}

<<<<<<< HEAD
/*static*/ bool APZThreadUtils::GetThreadAssertionsEnabled() {
||||||| merged common ancestors
/*static*/ bool
APZThreadUtils::GetThreadAssertionsEnabled() {
=======
/*static*/
bool APZThreadUtils::GetThreadAssertionsEnabled() {
>>>>>>> upstream-releases
  return sThreadAssertionsEnabled;
}

<<<<<<< HEAD
/*static*/ void APZThreadUtils::SetControllerThread(MessageLoop* aLoop) {
||||||| merged common ancestors
/*static*/ void
APZThreadUtils::SetControllerThread(MessageLoop* aLoop)
{
=======
/*static*/
void APZThreadUtils::SetControllerThread(MessageLoop* aLoop) {
>>>>>>> upstream-releases
  // We must either be setting the initial controller thread, or removing it,
  // or re-using an existing controller thread.
  MOZ_ASSERT(!sControllerThread || !aLoop || sControllerThread == aLoop);
  sControllerThread = aLoop;
}

<<<<<<< HEAD
/*static*/ void APZThreadUtils::AssertOnControllerThread() {
||||||| merged common ancestors
/*static*/ void
APZThreadUtils::AssertOnControllerThread() {
=======
/*static*/
void APZThreadUtils::AssertOnControllerThread() {
>>>>>>> upstream-releases
  if (!GetThreadAssertionsEnabled()) {
    return;
  }

  MOZ_ASSERT(sControllerThread == MessageLoop::current());
}

<<<<<<< HEAD
/*static*/ void APZThreadUtils::RunOnControllerThread(
    RefPtr<Runnable>&& aTask) {
  RefPtr<Runnable> task = std::move(aTask);
||||||| merged common ancestors
/*static*/ void
APZThreadUtils::RunOnControllerThread(already_AddRefed<Runnable> aTask)
{
  RefPtr<Runnable> task = aTask;
=======
/*static*/
void APZThreadUtils::RunOnControllerThread(RefPtr<Runnable>&& aTask) {
  RefPtr<Runnable> task = std::move(aTask);
>>>>>>> upstream-releases

  if (!sControllerThread) {
    // Could happen on startup
    NS_WARNING("Dropping task posted to controller thread");
    return;
  }

  if (sControllerThread == MessageLoop::current()) {
    task->Run();
  } else {
    sControllerThread->PostTask(task.forget());
  }
}

<<<<<<< HEAD
/*static*/ bool APZThreadUtils::IsControllerThread() {
||||||| merged common ancestors
/*static*/ bool
APZThreadUtils::IsControllerThread()
{
=======
/*static*/
bool APZThreadUtils::IsControllerThread() {
>>>>>>> upstream-releases
  return sControllerThread == MessageLoop::current();
}

NS_IMPL_ISUPPORTS(GenericNamedTimerCallbackBase, nsITimerCallback, nsINamed)

}  // namespace layers
}  // namespace mozilla
