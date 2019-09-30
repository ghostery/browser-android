/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_workers_WorkerCommon_h
#define mozilla_dom_workers_WorkerCommon_h

#include "jsapi.h"
#include "mozilla/Attributes.h"
#include "mozilla/dom/ServiceWorkerDescriptor.h"

class nsPIDOMWindowInner;

namespace mozilla {
namespace dom {

class WorkerPrivate;

// All of these are implemented in RuntimeService.cpp

WorkerPrivate* GetWorkerPrivateFromContext(JSContext* aCx);

WorkerPrivate* GetCurrentThreadWorkerPrivate();

bool IsCurrentThreadRunningWorker();

bool IsCurrentThreadRunningChromeWorker();

JSContext* GetCurrentWorkerThreadJSContext();

JSObject* GetCurrentThreadWorkerGlobal();

<<<<<<< HEAD
void CancelWorkersForWindow(nsPIDOMWindowInner* aWindow);
||||||| merged common ancestors
void
CancelWorkersForWindow(nsPIDOMWindowInner* aWindow);
=======
JSObject* GetCurrentThreadWorkerDebuggerGlobal();
>>>>>>> upstream-releases

<<<<<<< HEAD
void FreezeWorkersForWindow(nsPIDOMWindowInner* aWindow);
||||||| merged common ancestors
void
FreezeWorkersForWindow(nsPIDOMWindowInner* aWindow);
=======
void CancelWorkersForWindow(nsPIDOMWindowInner* aWindow);
>>>>>>> upstream-releases

<<<<<<< HEAD
void ThawWorkersForWindow(nsPIDOMWindowInner* aWindow);
||||||| merged common ancestors
void
ThawWorkersForWindow(nsPIDOMWindowInner* aWindow);
=======
void FreezeWorkersForWindow(nsPIDOMWindowInner* aWindow);
>>>>>>> upstream-releases

<<<<<<< HEAD
void SuspendWorkersForWindow(nsPIDOMWindowInner* aWindow);
||||||| merged common ancestors
void
SuspendWorkersForWindow(nsPIDOMWindowInner* aWindow);
=======
void ThawWorkersForWindow(nsPIDOMWindowInner* aWindow);
>>>>>>> upstream-releases

<<<<<<< HEAD
void ResumeWorkersForWindow(nsPIDOMWindowInner* aWindow);
||||||| merged common ancestors
void
ResumeWorkersForWindow(nsPIDOMWindowInner* aWindow);
=======
void SuspendWorkersForWindow(nsPIDOMWindowInner* aWindow);
>>>>>>> upstream-releases

<<<<<<< HEAD
void PropagateFirstPartyStorageAccessGrantedToWorkers(
    nsPIDOMWindowInner* aWindow);
||||||| merged common ancestors
void
PropagateFirstPartyStorageAccessGrantedToWorkers(nsPIDOMWindowInner* aWindow);
=======
void ResumeWorkersForWindow(nsPIDOMWindowInner* aWindow);

void PropagateFirstPartyStorageAccessGrantedToWorkers(
    nsPIDOMWindowInner* aWindow);
>>>>>>> upstream-releases

// All of these are implemented in WorkerScope.cpp

bool IsWorkerGlobal(JSObject* global);

bool IsWorkerDebuggerGlobal(JSObject* global);

bool IsWorkerDebuggerSandbox(JSObject* object);

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_workers_WorkerCommon_h
