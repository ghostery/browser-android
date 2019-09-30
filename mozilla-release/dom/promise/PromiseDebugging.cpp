/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "js/Value.h"
#include "nsThreadUtils.h"

#include "mozilla/CycleCollectedJSContext.h"
#include "mozilla/RefPtr.h"
#include "mozilla/SystemGroup.h"
#include "mozilla/ThreadLocal.h"
#include "mozilla/TimeStamp.h"

#include "mozilla/dom/BindingDeclarations.h"
#include "mozilla/dom/ContentChild.h"
#include "mozilla/dom/Promise.h"
#include "mozilla/dom/PromiseBinding.h"
#include "mozilla/dom/PromiseDebugging.h"
#include "mozilla/dom/PromiseDebuggingBinding.h"

namespace mozilla {
namespace dom {

class FlushRejections : public CancelableRunnable {
 public:
  FlushRejections() : CancelableRunnable("dom::FlushRejections") {}

  static void Init() {
    if (!sDispatched.init()) {
      MOZ_CRASH("Could not initialize FlushRejections::sDispatched");
    }
    sDispatched.set(false);
  }

  static void DispatchNeeded() {
    if (sDispatched.get()) {
      // An instance of `FlushRejections` has already been dispatched
      // and not run yet. No need to dispatch another one.
      return;
    }
    sDispatched.set(true);
    SystemGroup::Dispatch(TaskCategory::Other,
                          do_AddRef(new FlushRejections()));
  }

  static void FlushSync() {
    sDispatched.set(false);

    // Call the callbacks if necessary.
    // Note that these callbacks may in turn cause Promise to turn
    // uncaught or consumed. Since `sDispatched` is `false`,
    // `FlushRejections` will be called once again, on an ulterior
    // tick.
    PromiseDebugging::FlushUncaughtRejectionsInternal();
  }

  NS_IMETHOD Run() override {
    FlushSync();
    return NS_OK;
  }

 private:
  // `true` if an instance of `FlushRejections` is currently dispatched
  // and has not been executed yet.
  static MOZ_THREAD_LOCAL(bool) sDispatched;
};

/* static */ MOZ_THREAD_LOCAL(bool) FlushRejections::sDispatched;

<<<<<<< HEAD
/* static */ void PromiseDebugging::GetState(
    GlobalObject& aGlobal, JS::Handle<JSObject*> aPromise,
    PromiseDebuggingStateHolder& aState, ErrorResult& aRv) {
||||||| merged common ancestors
/* static */ void
PromiseDebugging::GetState(GlobalObject& aGlobal, JS::Handle<JSObject*> aPromise,
                           PromiseDebuggingStateHolder& aState,
                           ErrorResult& aRv)
{
=======
/* static */
void PromiseDebugging::GetState(GlobalObject& aGlobal,
                                JS::Handle<JSObject*> aPromise,
                                PromiseDebuggingStateHolder& aState,
                                ErrorResult& aRv) {
>>>>>>> upstream-releases
  JSContext* cx = aGlobal.Context();
  // CheckedUnwrapStatic is fine, since we're looking for promises only.
  JS::Rooted<JSObject*> obj(cx, js::CheckedUnwrapStatic(aPromise));
  if (!obj || !JS::IsPromiseObject(obj)) {
    aRv.ThrowTypeError<MSG_IS_NOT_PROMISE>(
        NS_LITERAL_STRING("Argument of PromiseDebugging.getState"));
    return;
  }
  switch (JS::GetPromiseState(obj)) {
    case JS::PromiseState::Pending:
      aState.mState = PromiseDebuggingState::Pending;
      break;
    case JS::PromiseState::Fulfilled:
      aState.mState = PromiseDebuggingState::Fulfilled;
      aState.mValue = JS::GetPromiseResult(obj);
      break;
    case JS::PromiseState::Rejected:
      aState.mState = PromiseDebuggingState::Rejected;
      aState.mReason = JS::GetPromiseResult(obj);
      break;
  }
}

<<<<<<< HEAD
/* static */ void PromiseDebugging::GetPromiseID(GlobalObject& aGlobal,
                                                 JS::Handle<JSObject*> aPromise,
                                                 nsString& aID,
                                                 ErrorResult& aRv) {
||||||| merged common ancestors
/* static */ void
PromiseDebugging::GetPromiseID(GlobalObject& aGlobal,
                               JS::Handle<JSObject*> aPromise,
                               nsString& aID,
                               ErrorResult& aRv)
{
=======
/* static */
void PromiseDebugging::GetPromiseID(GlobalObject& aGlobal,
                                    JS::Handle<JSObject*> aPromise,
                                    nsString& aID, ErrorResult& aRv) {
>>>>>>> upstream-releases
  JSContext* cx = aGlobal.Context();
  // CheckedUnwrapStatic is fine, since we're looking for promises only.
  JS::Rooted<JSObject*> obj(cx, js::CheckedUnwrapStatic(aPromise));
  if (!obj || !JS::IsPromiseObject(obj)) {
    aRv.ThrowTypeError<MSG_IS_NOT_PROMISE>(
        NS_LITERAL_STRING("Argument of PromiseDebugging.getState"));
    return;
  }
  uint64_t promiseID = JS::GetPromiseID(obj);
  aID = sIDPrefix;
  aID.AppendInt(promiseID);
}

<<<<<<< HEAD
/* static */ void PromiseDebugging::GetAllocationStack(
    GlobalObject& aGlobal, JS::Handle<JSObject*> aPromise,
    JS::MutableHandle<JSObject*> aStack, ErrorResult& aRv) {
||||||| merged common ancestors
/* static */ void
PromiseDebugging::GetAllocationStack(GlobalObject& aGlobal,
                                     JS::Handle<JSObject*> aPromise,
                                     JS::MutableHandle<JSObject*> aStack,
                                     ErrorResult& aRv)
{
=======
/* static */
void PromiseDebugging::GetAllocationStack(GlobalObject& aGlobal,
                                          JS::Handle<JSObject*> aPromise,
                                          JS::MutableHandle<JSObject*> aStack,
                                          ErrorResult& aRv) {
>>>>>>> upstream-releases
  JSContext* cx = aGlobal.Context();
  // CheckedUnwrapStatic is fine, since we're looking for promises only.
  JS::Rooted<JSObject*> obj(cx, js::CheckedUnwrapStatic(aPromise));
  if (!obj || !JS::IsPromiseObject(obj)) {
    aRv.ThrowTypeError<MSG_IS_NOT_PROMISE>(
        NS_LITERAL_STRING("Argument of PromiseDebugging.getAllocationStack"));
    return;
  }
  aStack.set(JS::GetPromiseAllocationSite(obj));
}

<<<<<<< HEAD
/* static */ void PromiseDebugging::GetRejectionStack(
    GlobalObject& aGlobal, JS::Handle<JSObject*> aPromise,
    JS::MutableHandle<JSObject*> aStack, ErrorResult& aRv) {
||||||| merged common ancestors
/* static */ void
PromiseDebugging::GetRejectionStack(GlobalObject& aGlobal,
                                    JS::Handle<JSObject*> aPromise,
                                    JS::MutableHandle<JSObject*> aStack,
                                    ErrorResult& aRv)
{
=======
/* static */
void PromiseDebugging::GetRejectionStack(GlobalObject& aGlobal,
                                         JS::Handle<JSObject*> aPromise,
                                         JS::MutableHandle<JSObject*> aStack,
                                         ErrorResult& aRv) {
>>>>>>> upstream-releases
  JSContext* cx = aGlobal.Context();
  // CheckedUnwrapStatic is fine, since we're looking for promises only.
  JS::Rooted<JSObject*> obj(cx, js::CheckedUnwrapStatic(aPromise));
  if (!obj || !JS::IsPromiseObject(obj)) {
    aRv.ThrowTypeError<MSG_IS_NOT_PROMISE>(
        NS_LITERAL_STRING("Argument of PromiseDebugging.getRejectionStack"));
    return;
  }
  aStack.set(JS::GetPromiseResolutionSite(obj));
}

<<<<<<< HEAD
/* static */ void PromiseDebugging::GetFullfillmentStack(
    GlobalObject& aGlobal, JS::Handle<JSObject*> aPromise,
    JS::MutableHandle<JSObject*> aStack, ErrorResult& aRv) {
||||||| merged common ancestors
/* static */ void
PromiseDebugging::GetFullfillmentStack(GlobalObject& aGlobal,
                                       JS::Handle<JSObject*> aPromise,
                                       JS::MutableHandle<JSObject*> aStack,
                                       ErrorResult& aRv)
{
=======
/* static */
void PromiseDebugging::GetFullfillmentStack(GlobalObject& aGlobal,
                                            JS::Handle<JSObject*> aPromise,
                                            JS::MutableHandle<JSObject*> aStack,
                                            ErrorResult& aRv) {
>>>>>>> upstream-releases
  JSContext* cx = aGlobal.Context();
  // CheckedUnwrapStatic is fine, since we're looking for promises only.
  JS::Rooted<JSObject*> obj(cx, js::CheckedUnwrapStatic(aPromise));
  if (!obj || !JS::IsPromiseObject(obj)) {
    aRv.ThrowTypeError<MSG_IS_NOT_PROMISE>(
        NS_LITERAL_STRING("Argument of PromiseDebugging.getFulfillmentStack"));
    return;
  }
  aStack.set(JS::GetPromiseResolutionSite(obj));
}

<<<<<<< HEAD
/*static */ nsString PromiseDebugging::sIDPrefix;
||||||| merged common ancestors
/*static */ nsString
PromiseDebugging::sIDPrefix;
=======
/*static */
nsString PromiseDebugging::sIDPrefix;
>>>>>>> upstream-releases

<<<<<<< HEAD
/* static */ void PromiseDebugging::Init() {
||||||| merged common ancestors
/* static */ void
PromiseDebugging::Init()
{
=======
/* static */
void PromiseDebugging::Init() {
>>>>>>> upstream-releases
  FlushRejections::Init();

  // Generate a prefix for identifiers: "PromiseDebugging.$processid."
  sIDPrefix = NS_LITERAL_STRING("PromiseDebugging.");
  if (XRE_IsContentProcess()) {
    sIDPrefix.AppendInt(ContentChild::GetSingleton()->GetID());
    sIDPrefix.Append('.');
  } else {
    sIDPrefix.AppendLiteral("0.");
  }
}

<<<<<<< HEAD
/* static */ void PromiseDebugging::Shutdown() { sIDPrefix.SetIsVoid(true); }
||||||| merged common ancestors
/* static */ void
PromiseDebugging::Shutdown()
{
  sIDPrefix.SetIsVoid(true);
}
=======
/* static */
void PromiseDebugging::Shutdown() { sIDPrefix.SetIsVoid(true); }
>>>>>>> upstream-releases

<<<<<<< HEAD
/* static */ void PromiseDebugging::FlushUncaughtRejections() {
||||||| merged common ancestors
/* static */ void
PromiseDebugging::FlushUncaughtRejections()
{
=======
/* static */
void PromiseDebugging::FlushUncaughtRejections() {
>>>>>>> upstream-releases
  MOZ_ASSERT(!NS_IsMainThread());
  FlushRejections::FlushSync();
}

<<<<<<< HEAD
/* static */ void PromiseDebugging::AddUncaughtRejectionObserver(
    GlobalObject&, UncaughtRejectionObserver& aObserver) {
||||||| merged common ancestors
/* static */ void
PromiseDebugging::AddUncaughtRejectionObserver(GlobalObject&,
                                               UncaughtRejectionObserver& aObserver)
{
=======
/* static */
void PromiseDebugging::AddUncaughtRejectionObserver(
    GlobalObject&, UncaughtRejectionObserver& aObserver) {
>>>>>>> upstream-releases
  CycleCollectedJSContext* storage = CycleCollectedJSContext::Get();
  nsTArray<nsCOMPtr<nsISupports>>& observers =
      storage->mUncaughtRejectionObservers;
  observers.AppendElement(&aObserver);
}

<<<<<<< HEAD
/* static */ bool PromiseDebugging::RemoveUncaughtRejectionObserver(
    GlobalObject&, UncaughtRejectionObserver& aObserver) {
||||||| merged common ancestors
/* static */ bool
PromiseDebugging::RemoveUncaughtRejectionObserver(GlobalObject&,
                                                  UncaughtRejectionObserver& aObserver)
{
=======
/* static */
bool PromiseDebugging::RemoveUncaughtRejectionObserver(
    GlobalObject&, UncaughtRejectionObserver& aObserver) {
>>>>>>> upstream-releases
  CycleCollectedJSContext* storage = CycleCollectedJSContext::Get();
  nsTArray<nsCOMPtr<nsISupports>>& observers =
      storage->mUncaughtRejectionObservers;
  for (size_t i = 0; i < observers.Length(); ++i) {
    UncaughtRejectionObserver* observer =
        static_cast<UncaughtRejectionObserver*>(observers[i].get());
    if (*observer == aObserver) {
      observers.RemoveElementAt(i);
      return true;
    }
  }
  return false;
}

<<<<<<< HEAD
/* static */ void PromiseDebugging::AddUncaughtRejection(
    JS::HandleObject aPromise) {
||||||| merged common ancestors
/* static */ void
PromiseDebugging::AddUncaughtRejection(JS::HandleObject aPromise)
{
=======
/* static */
void PromiseDebugging::AddUncaughtRejection(JS::HandleObject aPromise) {
>>>>>>> upstream-releases
  // This might OOM, but won't set a pending exception, so we'll just ignore it.
  if (CycleCollectedJSContext::Get()->mUncaughtRejections.append(aPromise)) {
    FlushRejections::DispatchNeeded();
  }
}

<<<<<<< HEAD
/* void */ void PromiseDebugging::AddConsumedRejection(
    JS::HandleObject aPromise) {
||||||| merged common ancestors
/* void */ void
PromiseDebugging::AddConsumedRejection(JS::HandleObject aPromise)
{
=======
/* void */
void PromiseDebugging::AddConsumedRejection(JS::HandleObject aPromise) {
>>>>>>> upstream-releases
  // If the promise is in our list of uncaught rejections, we haven't yet
  // reported it as unhandled. In that case, just remove it from the list
  // and don't add it to the list of consumed rejections.
  auto& uncaughtRejections =
      CycleCollectedJSContext::Get()->mUncaughtRejections;
  for (size_t i = 0; i < uncaughtRejections.length(); i++) {
    if (uncaughtRejections[i] == aPromise) {
      // To avoid large amounts of memmoves, we don't shrink the vector here.
      // Instead, we filter out nullptrs when iterating over the vector later.
      uncaughtRejections[i].set(nullptr);
      return;
    }
  }
  // This might OOM, but won't set a pending exception, so we'll just ignore it.
  if (CycleCollectedJSContext::Get()->mConsumedRejections.append(aPromise)) {
    FlushRejections::DispatchNeeded();
  }
}

<<<<<<< HEAD
/* static */ void PromiseDebugging::FlushUncaughtRejectionsInternal() {
||||||| merged common ancestors
/* static */ void
PromiseDebugging::FlushUncaughtRejectionsInternal()
{
=======
/* static */
void PromiseDebugging::FlushUncaughtRejectionsInternal() {
>>>>>>> upstream-releases
  CycleCollectedJSContext* storage = CycleCollectedJSContext::Get();

  auto& uncaught = storage->mUncaughtRejections;
  auto& consumed = storage->mConsumedRejections;

  AutoJSAPI jsapi;
  jsapi.Init();
  JSContext* cx = jsapi.cx();

  // Notify observers of uncaught Promise.
  auto& observers = storage->mUncaughtRejectionObservers;

  for (size_t i = 0; i < uncaught.length(); i++) {
    JS::RootedObject promise(cx, uncaught[i]);
    // Filter out nullptrs which might've been added by
    // PromiseDebugging::AddConsumedRejection.
    if (!promise) {
      continue;
    }

    for (size_t j = 0; j < observers.Length(); ++j) {
      RefPtr<UncaughtRejectionObserver> obs =
          static_cast<UncaughtRejectionObserver*>(observers[j].get());

      obs->OnLeftUncaught(promise, IgnoreErrors());
    }
    JSAutoRealm ar(cx, promise);
    Promise::ReportRejectedPromise(cx, promise);
  }
  storage->mUncaughtRejections.clear();

  // Notify observers of consumed Promise.

  for (size_t i = 0; i < consumed.length(); i++) {
    JS::RootedObject promise(cx, consumed[i]);

    for (size_t j = 0; j < observers.Length(); ++j) {
      RefPtr<UncaughtRejectionObserver> obs =
          static_cast<UncaughtRejectionObserver*>(observers[j].get());

      obs->OnConsumed(promise, IgnoreErrors());
    }
  }
  storage->mConsumedRejections.clear();
}

}  // namespace dom
}  // namespace mozilla
