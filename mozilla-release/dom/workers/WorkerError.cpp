/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "WorkerError.h"

#include "mozilla/DOMEventTargetHelper.h"
#include "mozilla/dom/ErrorEvent.h"
#include "mozilla/dom/ErrorEventBinding.h"
#include "mozilla/dom/RemoteWorkerChild.h"
#include "mozilla/dom/ServiceWorkerManager.h"
#include "mozilla/dom/SimpleGlobalObject.h"
#include "mozilla/dom/WorkerDebuggerGlobalScopeBinding.h"
#include "mozilla/dom/WorkerGlobalScopeBinding.h"
#include "mozilla/EventDispatcher.h"
#include "nsGlobalWindowInner.h"
#include "nsIConsoleService.h"
#include "nsScriptError.h"
#include "WorkerRunnable.h"
#include "WorkerPrivate.h"
#include "WorkerScope.h"

namespace mozilla {
namespace dom {

namespace {

<<<<<<< HEAD
class ReportErrorRunnable final : public WorkerDebuggeeRunnable {
  WorkerErrorReport mReport;

 public:
  // aWorkerPrivate is the worker thread we're on (or the main thread, if null)
  // aTarget is the worker object that we are going to fire an error at
  // (if any).
  static void ReportError(
      JSContext* aCx, WorkerPrivate* aWorkerPrivate, bool aFireAtScope,
      DOMEventTargetHelper* aTarget, const WorkerErrorReport& aReport,
      uint64_t aInnerWindowId,
      JS::Handle<JS::Value> aException = JS::NullHandleValue) {
    if (aWorkerPrivate) {
      aWorkerPrivate->AssertIsOnWorkerThread();
    } else {
      AssertIsOnMainThread();
    }

    // We should not fire error events for warnings but instead make sure that
    // they show up in the error console.
    if (!JSREPORT_IS_WARNING(aReport.mFlags)) {
      // First fire an ErrorEvent at the worker.
      RootedDictionary<ErrorEventInit> init(aCx);

      if (aReport.mMutedError) {
        init.mMessage.AssignLiteral("Script error.");
      } else {
        init.mMessage = aReport.mMessage;
        init.mFilename = aReport.mFilename;
        init.mLineno = aReport.mLineNumber;
        init.mError = aException;
      }

      init.mCancelable = true;
      init.mBubbles = false;

      if (aTarget) {
        RefPtr<ErrorEvent> event =
            ErrorEvent::Constructor(aTarget, NS_LITERAL_STRING("error"), init);
        event->SetTrusted(true);

        bool defaultActionEnabled =
            aTarget->DispatchEvent(*event, CallerType::System, IgnoreErrors());
        if (!defaultActionEnabled) {
          return;
        }
      }

      // Now fire an event at the global object, but don't do that if the error
      // code is too much recursion and this is the same script threw the error.
      // XXXbz the interaction of this with worker errors seems kinda broken.
      // An overrecursion in the debugger or debugger sandbox will get turned
      // into an error event on our parent worker!
      // https://bugzilla.mozilla.org/show_bug.cgi?id=1271441 tracks making this
      // better.
      if (aFireAtScope &&
          (aTarget || aReport.mErrorNumber != JSMSG_OVER_RECURSED)) {
        JS::Rooted<JSObject*> global(aCx, JS::CurrentGlobalOrNull(aCx));
        NS_ASSERTION(global, "This should never be null!");

        nsEventStatus status = nsEventStatus_eIgnore;

        if (aWorkerPrivate) {
          WorkerGlobalScope* globalScope = nullptr;
          UNWRAP_OBJECT(WorkerGlobalScope, &global, globalScope);

          if (!globalScope) {
            WorkerDebuggerGlobalScope* globalScope = nullptr;
            UNWRAP_OBJECT(WorkerDebuggerGlobalScope, &global, globalScope);

            MOZ_ASSERT_IF(globalScope,
                          globalScope->GetWrapperPreserveColor() == global);
            if (globalScope || IsWorkerDebuggerSandbox(global)) {
              aWorkerPrivate->ReportErrorToDebugger(
                  aReport.mFilename, aReport.mLineNumber, aReport.mMessage);
              return;
            }

            MOZ_ASSERT(SimpleGlobalObject::SimpleGlobalType(global) ==
                       SimpleGlobalObject::GlobalType::BindingDetail);
            // XXXbz We should really log this to console, but unwinding out of
            // this stuff without ending up firing any events is ... hard.  Just
            // return for now.
            // https://bugzilla.mozilla.org/show_bug.cgi?id=1271441 tracks
            // making this better.
            return;
          }

          MOZ_ASSERT(globalScope->GetWrapperPreserveColor() == global);

          RefPtr<ErrorEvent> event = ErrorEvent::Constructor(
              aTarget, NS_LITERAL_STRING("error"), init);
          event->SetTrusted(true);

          if (NS_FAILED(EventDispatcher::DispatchDOMEvent(
                  ToSupports(globalScope), nullptr, event, nullptr, &status))) {
            NS_WARNING("Failed to dispatch worker thread error event!");
            status = nsEventStatus_eIgnore;
          }
        } else if (nsGlobalWindowInner* win = xpc::WindowOrNull(global)) {
          MOZ_ASSERT(NS_IsMainThread());

          if (!win->HandleScriptError(init, &status)) {
            NS_WARNING("Failed to dispatch main thread error event!");
            status = nsEventStatus_eIgnore;
          }
        }

        // Was preventDefault() called?
        if (status == nsEventStatus_eConsumeNoDefault) {
          return;
        }
      }
    }

    // Now fire a runnable to do the same on the parent's thread if we can.
    if (aWorkerPrivate) {
      RefPtr<ReportErrorRunnable> runnable =
          new ReportErrorRunnable(aWorkerPrivate, aReport);
      runnable->Dispatch();
      return;
    }

    // Otherwise log an error to the error console.
    WorkerErrorReport::LogErrorToConsole(aReport, aInnerWindowId);
  }
||||||| merged common ancestors
class ReportErrorRunnable final : public WorkerRunnable
{
  WorkerErrorReport mReport;

public:
  // aWorkerPrivate is the worker thread we're on (or the main thread, if null)
  // aTarget is the worker object that we are going to fire an error at
  // (if any).
  static void
  ReportError(JSContext* aCx, WorkerPrivate* aWorkerPrivate,
              bool aFireAtScope, DOMEventTargetHelper* aTarget,
              const WorkerErrorReport& aReport, uint64_t aInnerWindowId,
              JS::Handle<JS::Value> aException = JS::NullHandleValue)
  {
    if (aWorkerPrivate) {
      aWorkerPrivate->AssertIsOnWorkerThread();
    } else {
      AssertIsOnMainThread();
    }

    // We should not fire error events for warnings but instead make sure that
    // they show up in the error console.
    if (!JSREPORT_IS_WARNING(aReport.mFlags)) {
      // First fire an ErrorEvent at the worker.
      RootedDictionary<ErrorEventInit> init(aCx);

      if (aReport.mMutedError) {
        init.mMessage.AssignLiteral("Script error.");
      } else {
        init.mMessage = aReport.mMessage;
        init.mFilename = aReport.mFilename;
        init.mLineno = aReport.mLineNumber;
        init.mError = aException;
      }

      init.mCancelable = true;
      init.mBubbles = false;

      if (aTarget) {
        RefPtr<ErrorEvent> event =
          ErrorEvent::Constructor(aTarget, NS_LITERAL_STRING("error"), init);
        event->SetTrusted(true);

        bool defaultActionEnabled =
          aTarget->DispatchEvent(*event, CallerType::System, IgnoreErrors());
        if (!defaultActionEnabled) {
          return;
        }
      }

      // Now fire an event at the global object, but don't do that if the error
      // code is too much recursion and this is the same script threw the error.
      // XXXbz the interaction of this with worker errors seems kinda broken.
      // An overrecursion in the debugger or debugger sandbox will get turned
      // into an error event on our parent worker!
      // https://bugzilla.mozilla.org/show_bug.cgi?id=1271441 tracks making this
      // better.
      if (aFireAtScope &&
          (aTarget || aReport.mErrorNumber != JSMSG_OVER_RECURSED)) {
        JS::Rooted<JSObject*> global(aCx, JS::CurrentGlobalOrNull(aCx));
        NS_ASSERTION(global, "This should never be null!");

        nsEventStatus status = nsEventStatus_eIgnore;

        if (aWorkerPrivate) {
          WorkerGlobalScope* globalScope = nullptr;
          UNWRAP_OBJECT(WorkerGlobalScope, &global, globalScope);

          if (!globalScope) {
            WorkerDebuggerGlobalScope* globalScope = nullptr;
            UNWRAP_OBJECT(WorkerDebuggerGlobalScope, &global, globalScope);

            MOZ_ASSERT_IF(globalScope, globalScope->GetWrapperPreserveColor() == global);
            if (globalScope || IsWorkerDebuggerSandbox(global)) {
              aWorkerPrivate->ReportErrorToDebugger(aReport.mFilename, aReport.mLineNumber,
                                                    aReport.mMessage);
              return;
            }

            MOZ_ASSERT(SimpleGlobalObject::SimpleGlobalType(global) ==
                         SimpleGlobalObject::GlobalType::BindingDetail);
            // XXXbz We should really log this to console, but unwinding out of
            // this stuff without ending up firing any events is ... hard.  Just
            // return for now.
            // https://bugzilla.mozilla.org/show_bug.cgi?id=1271441 tracks
            // making this better.
            return;
          }

          MOZ_ASSERT(globalScope->GetWrapperPreserveColor() == global);

          RefPtr<ErrorEvent> event =
            ErrorEvent::Constructor(aTarget, NS_LITERAL_STRING("error"), init);
          event->SetTrusted(true);

          if (NS_FAILED(EventDispatcher::DispatchDOMEvent(ToSupports(globalScope),
                                                          nullptr,
                                                          event, nullptr,
                                                          &status))) {
            NS_WARNING("Failed to dispatch worker thread error event!");
            status = nsEventStatus_eIgnore;
          }
        }
        else if (nsGlobalWindowInner* win = xpc::WindowOrNull(global)) {
          MOZ_ASSERT(NS_IsMainThread());

          if (!win->HandleScriptError(init, &status)) {
            NS_WARNING("Failed to dispatch main thread error event!");
            status = nsEventStatus_eIgnore;
          }
        }

        // Was preventDefault() called?
        if (status == nsEventStatus_eConsumeNoDefault) {
          return;
        }
      }
    }

    // Now fire a runnable to do the same on the parent's thread if we can.
    if (aWorkerPrivate) {
      RefPtr<ReportErrorRunnable> runnable =
        new ReportErrorRunnable(aWorkerPrivate, aReport);
      runnable->Dispatch();
      return;
    }

    // Otherwise log an error to the error console.
    WorkerErrorReport::LogErrorToConsole(aReport, aInnerWindowId);
  }
=======
class ReportErrorRunnable final : public WorkerDebuggeeRunnable {
  UniquePtr<WorkerErrorReport> mReport;
>>>>>>> upstream-releases

 public:
  ReportErrorRunnable(WorkerPrivate* aWorkerPrivate,
<<<<<<< HEAD
                      const WorkerErrorReport& aReport)
      : WorkerDebuggeeRunnable(aWorkerPrivate), mReport(aReport) {}

 private:
  virtual void PostDispatch(WorkerPrivate* aWorkerPrivate,
                            bool aDispatchResult) override {
||||||| merged common ancestors
                      const WorkerErrorReport& aReport)
  : WorkerRunnable(aWorkerPrivate, ParentThreadUnchangedBusyCount),
    mReport(aReport)
  { }

private:
  virtual void
  PostDispatch(WorkerPrivate* aWorkerPrivate, bool aDispatchResult) override
  {
=======
                      UniquePtr<WorkerErrorReport> aReport)
      : WorkerDebuggeeRunnable(aWorkerPrivate), mReport(std::move(aReport)) {}

 private:
  virtual void PostDispatch(WorkerPrivate* aWorkerPrivate,
                            bool aDispatchResult) override {
>>>>>>> upstream-releases
    aWorkerPrivate->AssertIsOnWorkerThread();

    // Dispatch may fail if the worker was canceled, no need to report that as
    // an error, so don't call base class PostDispatch.
  }

  virtual bool WorkerRun(JSContext* aCx,
                         WorkerPrivate* aWorkerPrivate) override {
    uint64_t innerWindowId;
    bool fireAtScope = true;

    bool workerIsAcceptingEvents = aWorkerPrivate->IsAcceptingEvents();

    WorkerPrivate* parent = aWorkerPrivate->GetParent();
    if (parent) {
      innerWindowId = 0;
    } else {
      AssertIsOnMainThread();

      // Once a window has frozen its workers, their
      // mMainThreadDebuggeeEventTargets should be paused, and their
      // WorkerDebuggeeRunnables should not be being executed. The same goes for
      // WorkerDebuggeeRunnables sent from child to parent workers, but since a
      // frozen parent worker runs only control runnables anyway, that is taken
      // care of naturally.
      MOZ_ASSERT(!aWorkerPrivate->IsFrozen());

      // Similarly for paused windows; all its workers should have been
      // informed. (Subworkers are unaffected by paused windows.)
      MOZ_ASSERT(!aWorkerPrivate->IsParentWindowPaused());

      if (aWorkerPrivate->IsSharedWorker()) {
<<<<<<< HEAD
        aWorkerPrivate->GetRemoteWorkerController()
            ->ErrorPropagationOnMainThread(&mReport,
                                           /* isErrorEvent */ true);
||||||| merged common ancestors
        aWorkerPrivate->BroadcastErrorToSharedWorkers(aCx, &mReport,
                                                      /* isErrorEvent */ true);
=======
        aWorkerPrivate->GetRemoteWorkerController()
            ->ErrorPropagationOnMainThread(mReport.get(),
                                           /* isErrorEvent */ true);
>>>>>>> upstream-releases
        return true;
      }

      // Service workers do not have a main thread parent global, so normal
      // worker error reporting will crash.  Instead, pass the error to
      // the ServiceWorkerManager to report on any controlled documents.
      if (aWorkerPrivate->IsServiceWorker()) {
        RefPtr<ServiceWorkerManager> swm = ServiceWorkerManager::GetInstance();
        if (swm) {
          swm->HandleError(aCx, aWorkerPrivate->GetPrincipal(),
                           aWorkerPrivate->ServiceWorkerScope(),
<<<<<<< HEAD
                           aWorkerPrivate->ScriptURL(), mReport.mMessage,
                           mReport.mFilename, mReport.mLine,
                           mReport.mLineNumber, mReport.mColumnNumber,
                           mReport.mFlags, mReport.mExnType);
||||||| merged common ancestors
                           aWorkerPrivate->ScriptURL(),
                           mReport.mMessage,
                           mReport.mFilename, mReport.mLine, mReport.mLineNumber,
                           mReport.mColumnNumber, mReport.mFlags,
                           mReport.mExnType);
=======
                           aWorkerPrivate->ScriptURL(), mReport->mMessage,
                           mReport->mFilename, mReport->mLine,
                           mReport->mLineNumber, mReport->mColumnNumber,
                           mReport->mFlags, mReport->mExnType);
>>>>>>> upstream-releases
        }
        return true;
      }

      // The innerWindowId is only required if we are going to ReportError
      // below, which is gated on this condition. The inner window correctness
      // check is only going to succeed when the worker is accepting events.
      if (workerIsAcceptingEvents) {
        aWorkerPrivate->AssertInnerWindowIsCorrect();
        innerWindowId = aWorkerPrivate->WindowID();
      }
    }

    // Don't fire this event if the JS object has been disconnected from the
    // private object.
    if (!workerIsAcceptingEvents) {
      return true;
    }

<<<<<<< HEAD
    ReportError(aCx, parent, fireAtScope,
                aWorkerPrivate->ParentEventTargetRef(), mReport, innerWindowId);
||||||| merged common ancestors
    ReportError(aCx, parent, fireAtScope,
                aWorkerPrivate->ParentEventTargetRef(),
                mReport, innerWindowId);
=======
    WorkerErrorReport::ReportError(aCx, parent, fireAtScope,
                                   aWorkerPrivate->ParentEventTargetRef(),
                                   std::move(mReport), innerWindowId);
>>>>>>> upstream-releases
    return true;
  }
};

class ReportGenericErrorRunnable final : public WorkerDebuggeeRunnable {
 public:
  static void CreateAndDispatch(WorkerPrivate* aWorkerPrivate) {
    MOZ_ASSERT(aWorkerPrivate);
    aWorkerPrivate->AssertIsOnWorkerThread();

    RefPtr<ReportGenericErrorRunnable> runnable =
        new ReportGenericErrorRunnable(aWorkerPrivate);
    runnable->Dispatch();
  }

 private:
  explicit ReportGenericErrorRunnable(WorkerPrivate* aWorkerPrivate)
      : WorkerDebuggeeRunnable(aWorkerPrivate) {
    aWorkerPrivate->AssertIsOnWorkerThread();
  }

  void PostDispatch(WorkerPrivate* aWorkerPrivate,
                    bool aDispatchResult) override {
    aWorkerPrivate->AssertIsOnWorkerThread();

    // Dispatch may fail if the worker was canceled, no need to report that as
    // an error, so don't call base class PostDispatch.
  }

  bool WorkerRun(JSContext* aCx, WorkerPrivate* aWorkerPrivate) override {
    // Once a window has frozen its workers, their
    // mMainThreadDebuggeeEventTargets should be paused, and their
    // WorkerDebuggeeRunnables should not be being executed. The same goes for
    // WorkerDebuggeeRunnables sent from child to parent workers, but since a
    // frozen parent worker runs only control runnables anyway, that is taken
    // care of naturally.
    MOZ_ASSERT(!aWorkerPrivate->IsFrozen());

    // Similarly for paused windows; all its workers should have been informed.
    // (Subworkers are unaffected by paused windows.)
    MOZ_ASSERT(!aWorkerPrivate->IsParentWindowPaused());

    if (aWorkerPrivate->IsSharedWorker()) {
      aWorkerPrivate->GetRemoteWorkerController()->ErrorPropagationOnMainThread(
          nullptr, false);
      return true;
    }

    if (aWorkerPrivate->IsServiceWorker()) {
      RefPtr<ServiceWorkerManager> swm = ServiceWorkerManager::GetInstance();
      if (swm) {
        swm->HandleError(aCx, aWorkerPrivate->GetPrincipal(),
                         aWorkerPrivate->ServiceWorkerScope(),
                         aWorkerPrivate->ScriptURL(), EmptyString(),
                         EmptyString(), EmptyString(), 0, 0, JSREPORT_ERROR,
                         JSEXN_ERR);
      }
      return true;
    }

    if (!aWorkerPrivate->IsAcceptingEvents()) {
      return true;
    }

    RefPtr<mozilla::dom::EventTarget> parentEventTarget =
        aWorkerPrivate->ParentEventTargetRef();
    RefPtr<Event> event = Event::Constructor(
        parentEventTarget, NS_LITERAL_STRING("error"), EventInit());
    event->SetTrusted(true);

    parentEventTarget->DispatchEvent(*event);
    return true;
  }
};

}  // namespace

void WorkerErrorBase::AssignErrorBase(JSErrorBase* aReport) {
  mFilename = NS_ConvertUTF8toUTF16(aReport->filename);
  mLineNumber = aReport->lineno;
  mColumnNumber = aReport->column;
  mErrorNumber = aReport->errorNumber;
}

void WorkerErrorNote::AssignErrorNote(JSErrorNotes::Note* aNote) {
  WorkerErrorBase::AssignErrorBase(aNote);
  xpc::ErrorNote::ErrorNoteToMessageString(aNote, mMessage);
}

<<<<<<< HEAD
void WorkerErrorReport::AssignErrorReport(JSErrorReport* aReport) {
||||||| merged common ancestors
void
WorkerErrorReport::AssignErrorReport(JSErrorReport* aReport)
{
=======
WorkerErrorReport::WorkerErrorReport()
    : mFlags(0), mExnType(JSEXN_ERR), mMutedError(false) {}

void WorkerErrorReport::AssignErrorReport(JSErrorReport* aReport) {
>>>>>>> upstream-releases
  WorkerErrorBase::AssignErrorBase(aReport);
  xpc::ErrorReport::ErrorReportToMessageString(aReport, mMessage);

  mLine.Assign(aReport->linebuf(), aReport->linebufLength());
  mFlags = aReport->flags;
  MOZ_ASSERT(aReport->exnType >= JSEXN_FIRST && aReport->exnType < JSEXN_LIMIT);
  mExnType = JSExnType(aReport->exnType);
  mMutedError = aReport->isMuted;

  if (aReport->notes) {
    if (!mNotes.SetLength(aReport->notes->length(), fallible)) {
      return;
    }

    size_t i = 0;
    for (auto&& note : *aReport->notes) {
      mNotes.ElementAt(i).AssignErrorNote(note.get());
      i++;
    }
  }
}

// aWorkerPrivate is the worker thread we're on (or the main thread, if null)
// aTarget is the worker object that we are going to fire an error at
// (if any).
<<<<<<< HEAD
/* static */ void WorkerErrorReport::ReportError(
    JSContext* aCx, WorkerPrivate* aWorkerPrivate, bool aFireAtScope,
    DOMEventTargetHelper* aTarget, const WorkerErrorReport& aReport,
    uint64_t aInnerWindowId, JS::Handle<JS::Value> aException) {
||||||| merged common ancestors
/* static */ void
WorkerErrorReport::ReportError(JSContext* aCx, WorkerPrivate* aWorkerPrivate,
                               bool aFireAtScope, DOMEventTargetHelper* aTarget,
                               const WorkerErrorReport& aReport,
                               uint64_t aInnerWindowId,
                               JS::Handle<JS::Value> aException)
{
=======
/* static */
void WorkerErrorReport::ReportError(
    JSContext* aCx, WorkerPrivate* aWorkerPrivate, bool aFireAtScope,
    DOMEventTargetHelper* aTarget, UniquePtr<WorkerErrorReport> aReport,
    uint64_t aInnerWindowId, JS::Handle<JS::Value> aException) {
>>>>>>> upstream-releases
  if (aWorkerPrivate) {
    aWorkerPrivate->AssertIsOnWorkerThread();
  } else {
    AssertIsOnMainThread();
  }

  // We should not fire error events for warnings but instead make sure that
  // they show up in the error console.
  if (!JSREPORT_IS_WARNING(aReport->mFlags)) {
    // First fire an ErrorEvent at the worker.
    RootedDictionary<ErrorEventInit> init(aCx);

    if (aReport->mMutedError) {
      init.mMessage.AssignLiteral("Script error.");
    } else {
      init.mMessage = aReport->mMessage;
      init.mFilename = aReport->mFilename;
      init.mLineno = aReport->mLineNumber;
      init.mError = aException;
    }

    init.mCancelable = true;
    init.mBubbles = false;

    if (aTarget) {
      RefPtr<ErrorEvent> event =
          ErrorEvent::Constructor(aTarget, NS_LITERAL_STRING("error"), init);
      event->SetTrusted(true);

      bool defaultActionEnabled =
          aTarget->DispatchEvent(*event, CallerType::System, IgnoreErrors());
      if (!defaultActionEnabled) {
        return;
      }
    }

    // Now fire an event at the global object, but don't do that if the error
    // code is too much recursion and this is the same script threw the error.
    // XXXbz the interaction of this with worker errors seems kinda broken.
    // An overrecursion in the debugger or debugger sandbox will get turned
    // into an error event on our parent worker!
    // https://bugzilla.mozilla.org/show_bug.cgi?id=1271441 tracks making this
    // better.
    if (aFireAtScope &&
        (aTarget || aReport->mErrorNumber != JSMSG_OVER_RECURSED)) {
      JS::Rooted<JSObject*> global(aCx, JS::CurrentGlobalOrNull(aCx));
      NS_ASSERTION(global, "This should never be null!");

      nsEventStatus status = nsEventStatus_eIgnore;

      if (aWorkerPrivate) {
        WorkerGlobalScope* globalScope = nullptr;
        UNWRAP_OBJECT(WorkerGlobalScope, &global, globalScope);

        if (!globalScope) {
          WorkerDebuggerGlobalScope* globalScope = nullptr;
          UNWRAP_OBJECT(WorkerDebuggerGlobalScope, &global, globalScope);

          MOZ_ASSERT_IF(globalScope,
                        globalScope->GetWrapperPreserveColor() == global);
          if (globalScope || IsWorkerDebuggerSandbox(global)) {
<<<<<<< HEAD
            aWorkerPrivate->ReportErrorToDebugger(
                aReport.mFilename, aReport.mLineNumber, aReport.mMessage);
||||||| merged common ancestors
            aWorkerPrivate->ReportErrorToDebugger(aReport.mFilename, aReport.mLineNumber,
                                                  aReport.mMessage);
=======
            aWorkerPrivate->ReportErrorToDebugger(
                aReport->mFilename, aReport->mLineNumber, aReport->mMessage);
>>>>>>> upstream-releases
            return;
          }

          MOZ_ASSERT(SimpleGlobalObject::SimpleGlobalType(global) ==
                     SimpleGlobalObject::GlobalType::BindingDetail);
          // XXXbz We should really log this to console, but unwinding out of
          // this stuff without ending up firing any events is ... hard.  Just
          // return for now.
          // https://bugzilla.mozilla.org/show_bug.cgi?id=1271441 tracks
          // making this better.
          return;
        }

        MOZ_ASSERT(globalScope->GetWrapperPreserveColor() == global);

        RefPtr<ErrorEvent> event =
            ErrorEvent::Constructor(aTarget, NS_LITERAL_STRING("error"), init);
        event->SetTrusted(true);

        if (NS_FAILED(EventDispatcher::DispatchDOMEvent(
                ToSupports(globalScope), nullptr, event, nullptr, &status))) {
          NS_WARNING("Failed to dispatch worker thread error event!");
          status = nsEventStatus_eIgnore;
        }
      } else if (nsGlobalWindowInner* win = xpc::WindowOrNull(global)) {
        MOZ_ASSERT(NS_IsMainThread());

        if (!win->HandleScriptError(init, &status)) {
          NS_WARNING("Failed to dispatch main thread error event!");
          status = nsEventStatus_eIgnore;
        }
      }

      // Was preventDefault() called?
      if (status == nsEventStatus_eConsumeNoDefault) {
        return;
      }
    }
  }

  // Now fire a runnable to do the same on the parent's thread if we can.
  if (aWorkerPrivate) {
    RefPtr<ReportErrorRunnable> runnable =
<<<<<<< HEAD
        new ReportErrorRunnable(aWorkerPrivate, aReport);
||||||| merged common ancestors
      new ReportErrorRunnable(aWorkerPrivate, aReport);
=======
        new ReportErrorRunnable(aWorkerPrivate, std::move(aReport));
>>>>>>> upstream-releases
    runnable->Dispatch();
    return;
  }

  // Otherwise log an error to the error console.
  WorkerErrorReport::LogErrorToConsole(aCx, *aReport, aInnerWindowId);
}

/* static */
void WorkerErrorReport::LogErrorToConsole(JSContext* aCx,
                                          WorkerErrorReport& aReport,
                                          uint64_t aInnerWindowId) {
  nsTArray<ErrorDataNote> notes;
  for (size_t i = 0, len = aReport.mNotes.Length(); i < len; i++) {
    const WorkerErrorNote& note = aReport.mNotes.ElementAt(i);
    notes.AppendElement(ErrorDataNote(note.mLineNumber, note.mColumnNumber,
                                      note.mMessage, note.mFilename));
  }

  JS::RootedObject stack(aCx, aReport.ReadStack(aCx));
  JS::RootedObject stackGlobal(aCx, JS::CurrentGlobalOrNull(aCx));

  ErrorData errorData(aReport.mLineNumber, aReport.mColumnNumber,
                      aReport.mFlags, aReport.mMessage, aReport.mFilename,
                      aReport.mLine, notes);
  LogErrorToConsole(errorData, aInnerWindowId, stack, stackGlobal);
}

<<<<<<< HEAD
/* static */ void WorkerErrorReport::LogErrorToConsole(
    const WorkerErrorReport& aReport, uint64_t aInnerWindowId) {
  nsTArray<ErrorDataNote> notes;
  for (size_t i = 0, len = aReport.mNotes.Length(); i < len; i++) {
    const WorkerErrorNote& note = aReport.mNotes.ElementAt(i);
    notes.AppendElement(ErrorDataNote(note.mLineNumber, note.mColumnNumber,
                                      note.mMessage, note.mFilename));
  }

  ErrorData errorData(aReport.mLineNumber, aReport.mColumnNumber,
                      aReport.mFlags, aReport.mMessage, aReport.mFilename,
                      aReport.mLine, notes);
  LogErrorToConsole(errorData, aInnerWindowId);
}

/* static */ void WorkerErrorReport::LogErrorToConsole(
    const ErrorData& aReport, uint64_t aInnerWindowId) {
||||||| merged common ancestors
/* static */ void
WorkerErrorReport::LogErrorToConsole(const WorkerErrorReport& aReport,
                                     uint64_t aInnerWindowId)
{
=======
/* static */
void WorkerErrorReport::LogErrorToConsole(const ErrorData& aReport,
                                          uint64_t aInnerWindowId,
                                          JS::HandleObject aStack,
                                          JS::HandleObject aStackGlobal) {
>>>>>>> upstream-releases
  AssertIsOnMainThread();

  RefPtr<nsScriptErrorBase> scriptError;
  if (aStack) {
    scriptError = new nsScriptErrorWithStack(aStack, aStackGlobal);
  } else {
    scriptError = new nsScriptError();
  }

  NS_WARNING_ASSERTION(scriptError, "Failed to create script error!");

  if (scriptError) {
    nsAutoCString category("Web Worker");
<<<<<<< HEAD
    if (NS_FAILED(scriptError->InitWithWindowID(
            aReport.message(), aReport.filename(), aReport.line(),
            aReport.lineNumber(), aReport.columnNumber(), aReport.flags(),
            category, aInnerWindowId))) {
||||||| merged common ancestors
    if (NS_FAILED(scriptError->InitWithWindowID(aReport.mMessage,
                                                aReport.mFilename,
                                                aReport.mLine,
                                                aReport.mLineNumber,
                                                aReport.mColumnNumber,
                                                aReport.mFlags,
                                                category,
                                                aInnerWindowId))) {
=======
    if (NS_FAILED(scriptError->nsIScriptError::InitWithWindowID(
            aReport.message(), aReport.filename(), aReport.line(),
            aReport.lineNumber(), aReport.columnNumber(), aReport.flags(),
            category, aInnerWindowId))) {
>>>>>>> upstream-releases
      NS_WARNING("Failed to init script error!");
      scriptError = nullptr;
    }

    for (size_t i = 0, len = aReport.notes().Length(); i < len; i++) {
      const ErrorDataNote& note = aReport.notes().ElementAt(i);

      nsScriptErrorNote* noteObject = new nsScriptErrorNote();
<<<<<<< HEAD
      noteObject->Init(note.message(), note.filename(), note.lineNumber(),
                       note.columnNumber());
||||||| merged common ancestors
      noteObject->Init(note.mMessage, note.mFilename,
                       note.mLineNumber, note.mColumnNumber);
=======
      noteObject->Init(note.message(), note.filename(), 0, note.lineNumber(),
                       note.columnNumber());
>>>>>>> upstream-releases
      scriptError->AddNote(noteObject);
    }
  }

  nsCOMPtr<nsIConsoleService> consoleService =
      do_GetService(NS_CONSOLESERVICE_CONTRACTID);
  NS_WARNING_ASSERTION(consoleService, "Failed to get console service!");

  if (consoleService) {
    if (scriptError) {
      if (NS_SUCCEEDED(consoleService->LogMessage(scriptError))) {
        return;
      }
      NS_WARNING("LogMessage failed!");
    } else if (NS_SUCCEEDED(consoleService->LogStringMessage(
                   aReport.message().BeginReading()))) {
      return;
    }
    NS_WARNING("LogStringMessage failed!");
  }

  NS_ConvertUTF16toUTF8 msg(aReport.message());
  NS_ConvertUTF16toUTF8 filename(aReport.filename());

  static const char kErrorString[] = "JS error in Web Worker: %s [%s:%u]";

#ifdef ANDROID
  __android_log_print(ANDROID_LOG_INFO, "Gecko", kErrorString, msg.get(),
                      filename.get(), aReport.lineNumber());
#endif

  fprintf(stderr, kErrorString, msg.get(), filename.get(),
          aReport.lineNumber());
  fflush(stderr);
}

<<<<<<< HEAD
/* static */ void
WorkerErrorReport::CreateAndDispatchGenericErrorRunnableToParent(
    WorkerPrivate* aWorkerPrivate) {
||||||| merged common ancestors
/* static */ void
WorkerErrorReport::CreateAndDispatchGenericErrorRunnableToParent(WorkerPrivate* aWorkerPrivate)
{
=======
/* static */
void WorkerErrorReport::CreateAndDispatchGenericErrorRunnableToParent(
    WorkerPrivate* aWorkerPrivate) {
>>>>>>> upstream-releases
  ReportGenericErrorRunnable::CreateAndDispatch(aWorkerPrivate);
}

}  // namespace dom
}  // namespace mozilla
