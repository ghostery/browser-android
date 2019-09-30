/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "WorkletImpl.h"

#include "Worklet.h"
#include "WorkletThread.h"

#include "mozilla/BasePrincipal.h"
#include "mozilla/dom/RegisterWorkletBindings.h"
#include "mozilla/dom/ScriptSettings.h"
#include "mozilla/dom/WorkletBinding.h"

namespace mozilla {

// ---------------------------------------------------------------------------
// WorkletLoadInfo

WorkletLoadInfo::WorkletLoadInfo(nsPIDOMWindowInner* aWindow,
                                 nsIPrincipal* aPrincipal)
    : mInnerWindowID(aWindow->WindowID()),
      mOriginAttributes(BasePrincipal::Cast(aPrincipal)->OriginAttributesRef()),
      mPrincipal(aPrincipal) {
  MOZ_ASSERT(NS_IsMainThread());
  nsPIDOMWindowOuter* outerWindow = aWindow->GetOuterWindow();
  if (outerWindow) {
    mOuterWindowID = outerWindow->WindowID();
  } else {
    mOuterWindowID = 0;
  }
}

WorkletLoadInfo::~WorkletLoadInfo() {
  MOZ_ASSERT(!mPrincipal || NS_IsMainThread());
}

// ---------------------------------------------------------------------------
// WorkletImpl

WorkletImpl::WorkletImpl(nsPIDOMWindowInner* aWindow, nsIPrincipal* aPrincipal)
    : mWorkletLoadInfo(aWindow, aPrincipal), mTerminated(false) {}

WorkletImpl::~WorkletImpl() { MOZ_ASSERT(!mGlobalScope); }

JSObject* WorkletImpl::WrapWorklet(JSContext* aCx, dom::Worklet* aWorklet,
                                   JS::Handle<JSObject*> aGivenProto) {
  MOZ_ASSERT(NS_IsMainThread());
  return dom::Worklet_Binding::Wrap(aCx, aWorklet, aGivenProto);
}

<<<<<<< HEAD
already_AddRefed<dom::WorkletGlobalScope> WorkletImpl::CreateGlobalScope(
    JSContext* aCx) {
||||||| merged common ancestors
already_AddRefed<dom::WorkletGlobalScope>
WorkletImpl::CreateGlobalScope(JSContext* aCx)
{
=======
dom::WorkletGlobalScope* WorkletImpl::GetGlobalScope() {
>>>>>>> upstream-releases
  dom::WorkletThread::AssertIsOnWorkletThread();

  if (mGlobalScope) {
    return mGlobalScope;
  }

  dom::AutoJSAPI jsapi;
  jsapi.Init();
  JSContext* cx = jsapi.cx();

  mGlobalScope = ConstructGlobalScope();

  JS::Rooted<JSObject*> global(cx);
  NS_ENSURE_TRUE(mGlobalScope->WrapGlobalObject(cx, &global), nullptr);

  JSAutoRealm ar(cx, global);

  // Init Web IDL bindings
  if (!dom::RegisterWorkletBindings(cx, global)) {
    return nullptr;
  }

  JS_FireOnNewGlobalObject(cx, global);

  return mGlobalScope;
}

void WorkletImpl::NotifyWorkletFinished() {
  MOZ_ASSERT(NS_IsMainThread());

<<<<<<< HEAD
  mTerminated = true;
  if (mWorkletThread) {
    mWorkletThread->Terminate();
    mWorkletThread = nullptr;
||||||| merged common ancestors
  if (!mWorkletThread) {
    // Thread creation. FIXME: this will change.
    mWorkletThread = dom::WorkletThread::Create();
=======
  if (mTerminated) {
    return;
>>>>>>> upstream-releases
  }
<<<<<<< HEAD
  mWorkletLoadInfo.mPrincipal = nullptr;
}
||||||| merged common ancestors

  return mWorkletThread;
}
=======

  // Release global scope on its thread.
  SendControlMessage(NS_NewRunnableFunction(
      "WorkletImpl::NotifyWorkletFinished",
      [self = RefPtr<WorkletImpl>(this)]() { self->mGlobalScope = nullptr; }));
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult WorkletImpl::SendControlMessage(
    already_AddRefed<nsIRunnable> aRunnable) {
  MOZ_ASSERT(NS_IsMainThread());

  // TODO: bug 1492011 re ConsoleWorkletRunnable.
  if (mTerminated) {
    return NS_ERROR_ILLEGAL_DURING_SHUTDOWN;
||||||| merged common ancestors
void
WorkletImpl::TerminateThread()
{
  MOZ_ASSERT(NS_IsMainThread());
  if (!mWorkletThread) {
    return;
=======
  mTerminated = true;
  if (mWorkletThread) {
    mWorkletThread->Terminate();
    mWorkletThread = nullptr;
>>>>>>> upstream-releases
  }
<<<<<<< HEAD

  if (!mWorkletThread) {
    // Thread creation. FIXME: this will change.
    mWorkletThread = dom::WorkletThread::Create();
    if (!mWorkletThread) {
      return NS_ERROR_UNEXPECTED;
    }
  }
||||||| merged common ancestors

  mWorkletThread->Terminate();
  mWorkletThread = nullptr;
  mWorkletLoadInfo.mPrincipal = nullptr;
}
=======
  mWorkletLoadInfo.mPrincipal = nullptr;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
||||||| merged common ancestors
nsresult
WorkletImpl::DispatchRunnable(already_AddRefed<nsIRunnable> aRunnable)
{
  // TODO: bug 1492011 re ConsoleWorkletRunnable.
  MOZ_ASSERT(mWorkletThread);
=======
nsresult WorkletImpl::SendControlMessage(
    already_AddRefed<nsIRunnable> aRunnable) {
  MOZ_ASSERT(NS_IsMainThread());

  // TODO: bug 1492011 re ConsoleWorkletRunnable.
  if (mTerminated) {
    return NS_ERROR_ILLEGAL_DURING_SHUTDOWN;
  }

  if (!mWorkletThread) {
    // Thread creation. FIXME: this will change.
    mWorkletThread = dom::WorkletThread::Create(this);
    if (!mWorkletThread) {
      return NS_ERROR_UNEXPECTED;
    }
  }

>>>>>>> upstream-releases
  return mWorkletThread->DispatchRunnable(std::move(aRunnable));
}

}  // namespace mozilla
