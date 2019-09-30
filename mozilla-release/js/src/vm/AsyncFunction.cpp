/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "vm/AsyncFunction.h"

#include "mozilla/Maybe.h"

#include "builtin/Promise.h"
#include "vm/GeneratorObject.h"
#include "vm/GlobalObject.h"
#include "vm/Interpreter.h"
#include "vm/Realm.h"
#include "vm/SelfHosting.h"

#include "vm/JSObject-inl.h"

using namespace js;

using mozilla::Maybe;

<<<<<<< HEAD
/* static */ bool GlobalObject::initAsyncFunction(
    JSContext* cx, Handle<GlobalObject*> global) {
  if (global->getReservedSlot(ASYNC_FUNCTION_PROTO).isObject()) {
||||||| merged common ancestors
/* static */ bool
GlobalObject::initAsyncFunction(JSContext* cx, Handle<GlobalObject*> global)
{
    if (global->getReservedSlot(ASYNC_FUNCTION_PROTO).isObject()) {
        return true;
    }

    RootedObject asyncFunctionProto(cx, NewSingletonObjectWithFunctionPrototype(cx, global));
    if (!asyncFunctionProto) {
        return false;
    }

    if (!DefineToStringTag(cx, asyncFunctionProto, cx->names().AsyncFunction)) {
        return false;
    }

    RootedValue function(cx, global->getConstructor(JSProto_Function));
    if (!function.toObjectOrNull()) {
        return false;
    }
    RootedObject proto(cx, &function.toObject());
    RootedAtom name(cx, cx->names().AsyncFunction);
    RootedObject asyncFunction(cx, NewFunctionWithProto(cx, AsyncFunctionConstructor, 1,
                                                        JSFunction::NATIVE_CTOR, nullptr, name,
                                                        proto));
    if (!asyncFunction) {
        return false;
    }
    if (!LinkConstructorAndPrototype(cx, asyncFunction, asyncFunctionProto,
                                     JSPROP_PERMANENT | JSPROP_READONLY, JSPROP_READONLY))
    {
        return false;
    }

    global->setReservedSlot(ASYNC_FUNCTION, ObjectValue(*asyncFunction));
    global->setReservedSlot(ASYNC_FUNCTION_PROTO, ObjectValue(*asyncFunctionProto));
=======
/* static */
bool GlobalObject::initAsyncFunction(JSContext* cx,
                                     Handle<GlobalObject*> global) {
  if (global->getReservedSlot(ASYNC_FUNCTION_PROTO).isObject()) {
>>>>>>> upstream-releases
    return true;
<<<<<<< HEAD
  }

  RootedObject asyncFunctionProto(
      cx, NewSingletonObjectWithFunctionPrototype(cx, global));
  if (!asyncFunctionProto) {
    return false;
  }

  if (!DefineToStringTag(cx, asyncFunctionProto, cx->names().AsyncFunction)) {
    return false;
  }

  RootedValue function(cx, global->getConstructor(JSProto_Function));
  if (!function.toObjectOrNull()) {
    return false;
  }
  RootedObject proto(cx, &function.toObject());
  RootedAtom name(cx, cx->names().AsyncFunction);
  RootedObject asyncFunction(
      cx, NewFunctionWithProto(cx, AsyncFunctionConstructor, 1,
                               JSFunction::NATIVE_CTOR, nullptr, name, proto));
  if (!asyncFunction) {
    return false;
  }
  if (!LinkConstructorAndPrototype(cx, asyncFunction, asyncFunctionProto,
                                   JSPROP_PERMANENT | JSPROP_READONLY,
                                   JSPROP_READONLY)) {
    return false;
  }

  global->setReservedSlot(ASYNC_FUNCTION, ObjectValue(*asyncFunction));
  global->setReservedSlot(ASYNC_FUNCTION_PROTO,
                          ObjectValue(*asyncFunctionProto));
  return true;
||||||| merged common ancestors
=======
  }

  RootedObject asyncFunctionProto(
      cx, NewSingletonObjectWithFunctionPrototype(cx, global));
  if (!asyncFunctionProto) {
    return false;
  }

  if (!DefineToStringTag(cx, asyncFunctionProto, cx->names().AsyncFunction)) {
    return false;
  }

  RootedObject proto(
      cx, GlobalObject::getOrCreateFunctionConstructor(cx, cx->global()));
  if (!proto) {
    return false;
  }
  HandlePropertyName name = cx->names().AsyncFunction;
  RootedObject asyncFunction(
      cx, NewFunctionWithProto(cx, AsyncFunctionConstructor, 1,
                               JSFunction::NATIVE_CTOR, nullptr, name, proto));
  if (!asyncFunction) {
    return false;
  }
  if (!LinkConstructorAndPrototype(cx, asyncFunction, asyncFunctionProto,
                                   JSPROP_PERMANENT | JSPROP_READONLY,
                                   JSPROP_READONLY)) {
    return false;
  }

  global->setReservedSlot(ASYNC_FUNCTION, ObjectValue(*asyncFunction));
  global->setReservedSlot(ASYNC_FUNCTION_PROTO,
                          ObjectValue(*asyncFunctionProto));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static MOZ_MUST_USE bool AsyncFunctionStart(
    JSContext* cx, Handle<PromiseObject*> resultPromise,
    HandleValue generatorVal);

#define UNWRAPPED_ASYNC_WRAPPED_SLOT 1
#define WRAPPED_ASYNC_UNWRAPPED_SLOT 0

// Async Functions proposal 1.1.8 and 1.2.14.
static bool WrappedAsyncFunction(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  RootedValue unwrappedVal(cx);
  unwrappedVal = args.callee().as<JSFunction>().getExtendedSlot(
      WRAPPED_ASYNC_UNWRAPPED_SLOT);

  // Step 2.
  // Also does a part of 2.2 steps 1-2.
  InvokeArgs args2(cx);
  if (!FillArgumentsFromArraylike(cx, args2, args)) {
    return false;
  }

  RootedValue generatorVal(cx);
  if (Call(cx, unwrappedVal, args.thisv(), args2, &generatorVal)) {
    // Step 1.
    Rooted<PromiseObject*> resultPromise(
        cx, CreatePromiseObjectForAsync(cx, generatorVal));
    if (!resultPromise) {
      return false;
    }
||||||| merged common ancestors
static MOZ_MUST_USE bool AsyncFunctionStart(JSContext* cx, Handle<PromiseObject*> resultPromise,
                                            HandleValue generatorVal);

#define UNWRAPPED_ASYNC_WRAPPED_SLOT 1
#define WRAPPED_ASYNC_UNWRAPPED_SLOT 0

// Async Functions proposal 1.1.8 and 1.2.14.
static bool
WrappedAsyncFunction(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);

    RootedValue unwrappedVal(cx);
    unwrappedVal = args.callee().as<JSFunction>().getExtendedSlot(WRAPPED_ASYNC_UNWRAPPED_SLOT);

    // Step 2.
    // Also does a part of 2.2 steps 1-2.
    InvokeArgs args2(cx);
    if (!FillArgumentsFromArraylike(cx, args2, args)) {
        return false;
    }

    RootedValue generatorVal(cx);
    if (Call(cx, unwrappedVal, args.thisv(), args2, &generatorVal)) {
        // Step 1.
        Rooted<PromiseObject*> resultPromise(cx, CreatePromiseObjectForAsync(cx, generatorVal));
        if (!resultPromise) {
            return false;
        }

        // Step 3.
        if (!AsyncFunctionStart(cx, resultPromise, generatorVal)) {
            return false;
        }

        // Step 5.
        args.rval().setObject(*resultPromise);
        return true;
    }
=======
enum class ResumeKind { Normal, Throw };

// ES2020 draft rev a09fc232c137800dbf51b6204f37fdede4ba1646
// 6.2.3.1.1 Await Fulfilled Functions
// 6.2.3.1.2 Await Rejected Functions
static bool AsyncFunctionResume(JSContext* cx,
                                Handle<AsyncFunctionGeneratorObject*> generator,
                                ResumeKind kind, HandleValue valueOrReason) {
  // We're enqueuing the promise job for Await before suspending the execution
  // of the async function. So when either the debugger or OOM errors terminate
  // the execution after JSOP_ASYNCAWAIT, but before JSOP_AWAIT, we're in an
  // inconsistent state, because we don't have a resume index set and therefore
  // don't know where to resume the async function. Return here in that case.
  if (generator->isClosed()) {
    return true;
  }

  // The debugger sets the async function's generator object into the "running"
  // state while firing debugger events to ensure the debugger can't re-enter
  // the async function, cf. |AutoSetGeneratorRunning| in Debugger.cpp. Catch
  // this case here by checking if the generator is already runnning.
  if (generator->isRunning()) {
    return true;
  }

  Rooted<PromiseObject*> resultPromise(cx, generator->promise());
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step 3.
    if (!AsyncFunctionStart(cx, resultPromise, generatorVal)) {
      return false;
    }
||||||| merged common ancestors
    if (!cx->isExceptionPending()) {
        return false;
    }

    // Steps 1, 4.
    RootedValue exc(cx);
    if (!GetAndClearException(cx, &exc)) {
        return false;
    }
    JSObject* rejectPromise = PromiseObject::unforgeableReject(cx, exc);
    if (!rejectPromise) {
        return false;
    }
=======
  RootedObject stack(cx);
  Maybe<JS::AutoSetAsyncStackForNewCalls> asyncStack;
  if (JSObject* allocationSite = resultPromise->allocationSite()) {
    // The promise is created within the activation of the async function, so
    // use the parent frame as the starting point for async stacks.
    stack = allocationSite->as<SavedFrame>().getParent();
    if (stack) {
      asyncStack.emplace(
          cx, stack, "async",
          JS::AutoSetAsyncStackForNewCalls::AsyncCallKind::EXPLICIT);
    }
  }

  MOZ_ASSERT(generator->isSuspended(),
             "non-suspended generator when resuming async function");

  // Execution context switching is handled in generator.
  HandlePropertyName funName = kind == ResumeKind::Normal
                                   ? cx->names().AsyncFunctionNext
                                   : cx->names().AsyncFunctionThrow;
  FixedInvokeArgs<1> args(cx);
  args[0].set(valueOrReason);
  RootedValue generatorOrValue(cx, ObjectValue(*generator));
  if (!CallSelfHostedFunction(cx, funName, generatorOrValue, args,
                              &generatorOrValue)) {
    if (!generator->isClosed()) {
      generator->setClosed();
    }

    // Handle the OOM case mentioned above.
    if (resultPromise->state() == JS::PromiseState::Pending &&
        cx->isExceptionPending()) {
      RootedValue exn(cx);
      if (!GetAndClearException(cx, &exn)) {
        return false;
      }
      return AsyncFunctionThrown(cx, resultPromise, exn);
    }
    return false;
  }

  MOZ_ASSERT_IF(generator->isClosed(), generatorOrValue.isObject());
  MOZ_ASSERT_IF(generator->isClosed(),
                &generatorOrValue.toObject() == resultPromise);
  MOZ_ASSERT_IF(!generator->isClosed(), generator->isAfterAwait());
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step 5.
    args.rval().setObject(*resultPromise);
    return true;
  }

  if (!cx->isExceptionPending()) {
    return false;
  }

  // Steps 1, 4.
  RootedValue exc(cx);
  if (!GetAndClearException(cx, &exc)) {
    return false;
  }
  JSObject* rejectPromise = PromiseObject::unforgeableReject(cx, exc);
  if (!rejectPromise) {
    return false;
  }

  // Step 5.
  args.rval().setObject(*rejectPromise);
  return true;
||||||| merged common ancestors
    // Step 5.
    args.rval().setObject(*rejectPromise);
    return true;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Async Functions proposal 2.1 steps 1, 3 (partially).
// In the spec it creates a function, but we create 2 functions `unwrapped` and
// `wrapped`.  `unwrapped` is a generator that corresponds to
//  the async function's body, replacing `await` with `yield`.  `wrapped` is a
// function that is visible to the outside, and handles yielded values.
JSObject* js::WrapAsyncFunctionWithProto(JSContext* cx,
                                         HandleFunction unwrapped,
                                         HandleObject proto) {
  MOZ_ASSERT(unwrapped->isAsync());
  MOZ_ASSERT(proto,
             "We need an explicit prototype to avoid the default"
             "%FunctionPrototype% fallback in NewFunctionWithProto().");

  // Create a new function with AsyncFunctionPrototype, reusing the name and
  // the length of `unwrapped`.

  RootedAtom funName(cx, unwrapped->explicitName());
  uint16_t length;
  if (!JSFunction::getLength(cx, unwrapped, &length)) {
    return nullptr;
  }

  // Steps 3 (partially).
  JSFunction* wrapped = NewFunctionWithProto(
      cx, WrappedAsyncFunction, length, JSFunction::NATIVE_FUN, nullptr,
      funName, proto, gc::AllocKind::FUNCTION_EXTENDED);
  if (!wrapped) {
    return nullptr;
  }

  if (unwrapped->hasInferredName()) {
    wrapped->setInferredName(unwrapped->inferredName());
  }

  // Link them to each other to make GetWrappedAsyncFunction and
  // GetUnwrappedAsyncFunction work.
  unwrapped->setExtendedSlot(UNWRAPPED_ASYNC_WRAPPED_SLOT,
                             ObjectValue(*wrapped));
  wrapped->setExtendedSlot(WRAPPED_ASYNC_UNWRAPPED_SLOT,
                           ObjectValue(*unwrapped));

  return wrapped;
||||||| merged common ancestors
// Async Functions proposal 2.1 steps 1, 3 (partially).
// In the spec it creates a function, but we create 2 functions `unwrapped` and
// `wrapped`.  `unwrapped` is a generator that corresponds to
//  the async function's body, replacing `await` with `yield`.  `wrapped` is a
// function that is visible to the outside, and handles yielded values.
JSObject*
js::WrapAsyncFunctionWithProto(JSContext* cx, HandleFunction unwrapped, HandleObject proto)
{
    MOZ_ASSERT(unwrapped->isAsync());
    MOZ_ASSERT(proto, "We need an explicit prototype to avoid the default"
                      "%FunctionPrototype% fallback in NewFunctionWithProto().");

    // Create a new function with AsyncFunctionPrototype, reusing the name and
    // the length of `unwrapped`.

    RootedAtom funName(cx, unwrapped->explicitName());
    uint16_t length;
    if (!JSFunction::getLength(cx, unwrapped, &length)) {
        return nullptr;
    }

    // Steps 3 (partially).
    JSFunction* wrapped = NewFunctionWithProto(cx, WrappedAsyncFunction, length,
                                               JSFunction::NATIVE_FUN, nullptr, funName, proto,
                                               gc::AllocKind::FUNCTION_EXTENDED);
    if (!wrapped) {
        return nullptr;
    }

    if (unwrapped->hasInferredName()) {
        wrapped->setInferredName(unwrapped->inferredName());
    }

    // Link them to each other to make GetWrappedAsyncFunction and
    // GetUnwrappedAsyncFunction work.
    unwrapped->setExtendedSlot(UNWRAPPED_ASYNC_WRAPPED_SLOT, ObjectValue(*wrapped));
    wrapped->setExtendedSlot(WRAPPED_ASYNC_UNWRAPPED_SLOT, ObjectValue(*unwrapped));

    return wrapped;
=======
// ES2020 draft rev a09fc232c137800dbf51b6204f37fdede4ba1646
// 6.2.3.1.1 Await Fulfilled Functions
MOZ_MUST_USE bool js::AsyncFunctionAwaitedFulfilled(
    JSContext* cx, Handle<AsyncFunctionGeneratorObject*> generator,
    HandleValue value) {
  return AsyncFunctionResume(cx, generator, ResumeKind::Normal, value);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSObject* js::WrapAsyncFunction(JSContext* cx, HandleFunction unwrapped) {
  RootedObject proto(
      cx, GlobalObject::getOrCreateAsyncFunctionPrototype(cx, cx->global()));
  if (!proto) {
    return nullptr;
  }

  return WrapAsyncFunctionWithProto(cx, unwrapped, proto);
||||||| merged common ancestors
JSObject*
js::WrapAsyncFunction(JSContext* cx, HandleFunction unwrapped)
{
    RootedObject proto(cx, GlobalObject::getOrCreateAsyncFunctionPrototype(cx, cx->global()));
    if (!proto) {
        return nullptr;
    }

    return WrapAsyncFunctionWithProto(cx, unwrapped, proto);
=======
// ES2020 draft rev a09fc232c137800dbf51b6204f37fdede4ba1646
// 6.2.3.1.2 Await Rejected Functions
MOZ_MUST_USE bool js::AsyncFunctionAwaitedRejected(
    JSContext* cx, Handle<AsyncFunctionGeneratorObject*> generator,
    HandleValue reason) {
  return AsyncFunctionResume(cx, generator, ResumeKind::Throw, reason);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
enum class ResumeKind { Normal, Throw };

// Async Functions proposal 2.2 steps 3.f, 3.g.
// Async Functions proposal 2.2 steps 3.d-e, 3.g.
// Implemented in js/src/builtin/Promise.cpp

// Async Functions proposal 2.2 steps 3-8, 2.4 steps 2-7, 2.5 steps 2-7.
static bool AsyncFunctionResume(JSContext* cx,
                                Handle<PromiseObject*> resultPromise,
                                HandleValue generatorVal, ResumeKind kind,
                                HandleValue valueOrReason) {
  RootedObject stack(cx, resultPromise->allocationSite());
  Maybe<JS::AutoSetAsyncStackForNewCalls> asyncStack;
  if (stack) {
    asyncStack.emplace(
        cx, stack, "async",
        JS::AutoSetAsyncStackForNewCalls::AsyncCallKind::EXPLICIT);
  }

  // Execution context switching is handled in generator.
  HandlePropertyName funName = kind == ResumeKind::Normal
                                   ? cx->names().GeneratorNext
                                   : cx->names().GeneratorThrow;
  FixedInvokeArgs<1> args(cx);
  args[0].set(valueOrReason);
  RootedValue value(cx);
  if (!CallSelfHostedFunction(cx, funName, generatorVal, args, &value)) {
    return AsyncFunctionThrown(cx, resultPromise);
  }

  if (generatorVal.toObject().as<GeneratorObject>().isAfterAwait()) {
    return AsyncFunctionAwait(cx, resultPromise, value);
  }

  return AsyncFunctionReturned(cx, resultPromise, value);
}

// Async Functions proposal 2.2 steps 3-8.
static MOZ_MUST_USE bool AsyncFunctionStart(
    JSContext* cx, Handle<PromiseObject*> resultPromise,
    HandleValue generatorVal) {
  return AsyncFunctionResume(cx, resultPromise, generatorVal,
                             ResumeKind::Normal, UndefinedHandleValue);
||||||| merged common ancestors
enum class ResumeKind {
    Normal,
    Throw
};

// Async Functions proposal 2.2 steps 3.f, 3.g.
// Async Functions proposal 2.2 steps 3.d-e, 3.g.
// Implemented in js/src/builtin/Promise.cpp

// Async Functions proposal 2.2 steps 3-8, 2.4 steps 2-7, 2.5 steps 2-7.
static bool
AsyncFunctionResume(JSContext* cx, Handle<PromiseObject*> resultPromise, HandleValue generatorVal,
                    ResumeKind kind, HandleValue valueOrReason)
{
    RootedObject stack(cx, resultPromise->allocationSite());
    Maybe<JS::AutoSetAsyncStackForNewCalls> asyncStack;
    if (stack) {
        asyncStack.emplace(cx, stack, "async",
                           JS::AutoSetAsyncStackForNewCalls::AsyncCallKind::EXPLICIT);
    }

    // Execution context switching is handled in generator.
    HandlePropertyName funName = kind == ResumeKind::Normal
                                 ? cx->names().GeneratorNext
                                 : cx->names().GeneratorThrow;
    FixedInvokeArgs<1> args(cx);
    args[0].set(valueOrReason);
    RootedValue value(cx);
    if (!CallSelfHostedFunction(cx, funName, generatorVal, args, &value)) {
        return AsyncFunctionThrown(cx, resultPromise);
    }

    if (generatorVal.toObject().as<GeneratorObject>().isAfterAwait()) {
        return AsyncFunctionAwait(cx, resultPromise, value);
    }

    return AsyncFunctionReturned(cx, resultPromise, value);
}

// Async Functions proposal 2.2 steps 3-8.
static MOZ_MUST_USE bool
AsyncFunctionStart(JSContext* cx, Handle<PromiseObject*> resultPromise, HandleValue generatorVal)
{
    return AsyncFunctionResume(cx, resultPromise, generatorVal, ResumeKind::Normal, UndefinedHandleValue);
=======
JSObject* js::AsyncFunctionResolve(
    JSContext* cx, Handle<AsyncFunctionGeneratorObject*> generator,
    HandleValue valueOrReason, AsyncFunctionResolveKind resolveKind) {
  Rooted<PromiseObject*> promise(cx, generator->promise());
  if (resolveKind == AsyncFunctionResolveKind::Fulfill) {
    if (!AsyncFunctionReturned(cx, promise, valueOrReason)) {
      return nullptr;
    }
  } else {
    if (!AsyncFunctionThrown(cx, promise, valueOrReason)) {
      return nullptr;
    }
  }
  return promise;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Async Functions proposal 2.3 steps 1-8.
// Implemented in js/src/builtin/Promise.cpp

// Async Functions proposal 2.4.
MOZ_MUST_USE bool js::AsyncFunctionAwaitedFulfilled(
    JSContext* cx, Handle<PromiseObject*> resultPromise,
    HandleValue generatorVal, HandleValue value) {
  // Step 1 (implicit).

  // Steps 2-7.
  return AsyncFunctionResume(cx, resultPromise, generatorVal,
                             ResumeKind::Normal, value);
}
||||||| merged common ancestors
// Async Functions proposal 2.3 steps 1-8.
// Implemented in js/src/builtin/Promise.cpp

// Async Functions proposal 2.4.
MOZ_MUST_USE bool
js::AsyncFunctionAwaitedFulfilled(JSContext* cx, Handle<PromiseObject*> resultPromise,
                                  HandleValue generatorVal, HandleValue value)
{
    // Step 1 (implicit).

    // Steps 2-7.
    return AsyncFunctionResume(cx, resultPromise, generatorVal, ResumeKind::Normal, value);
}
=======
const Class AsyncFunctionGeneratorObject::class_ = {
    "AsyncFunctionGenerator",
    JSCLASS_HAS_RESERVED_SLOTS(AsyncFunctionGeneratorObject::RESERVED_SLOTS)};

AsyncFunctionGeneratorObject* AsyncFunctionGeneratorObject::create(
    JSContext* cx, HandleFunction fun) {
  MOZ_ASSERT(fun->isAsync() && !fun->isGenerator());
>>>>>>> upstream-releases

<<<<<<< HEAD
// Async Functions proposal 2.5.
MOZ_MUST_USE bool js::AsyncFunctionAwaitedRejected(
    JSContext* cx, Handle<PromiseObject*> resultPromise,
    HandleValue generatorVal, HandleValue reason) {
  // Step 1 (implicit).

  // Step 2-7.
  return AsyncFunctionResume(cx, resultPromise, generatorVal, ResumeKind::Throw,
                             reason);
}
||||||| merged common ancestors
// Async Functions proposal 2.5.
MOZ_MUST_USE bool
js::AsyncFunctionAwaitedRejected(JSContext* cx, Handle<PromiseObject*> resultPromise,
                                 HandleValue generatorVal, HandleValue reason)
{
    // Step 1 (implicit).

    // Step 2-7.
    return AsyncFunctionResume(cx, resultPromise, generatorVal, ResumeKind::Throw, reason);
}
=======
  Rooted<PromiseObject*> resultPromise(cx, CreatePromiseObjectForAsync(cx));
  if (!resultPromise) {
    return nullptr;
  }

  auto* obj = NewBuiltinClassInstance<AsyncFunctionGeneratorObject>(cx);
  if (!obj) {
    return nullptr;
  }
  obj->initFixedSlot(PROMISE_SLOT, ObjectValue(*resultPromise));
>>>>>>> upstream-releases

<<<<<<< HEAD
JSFunction* js::GetWrappedAsyncFunction(JSFunction* unwrapped) {
  MOZ_ASSERT(unwrapped->isAsync());
  return &unwrapped->getExtendedSlot(UNWRAPPED_ASYNC_WRAPPED_SLOT)
              .toObject()
              .as<JSFunction>();
}

JSFunction* js::GetUnwrappedAsyncFunction(JSFunction* wrapped) {
  MOZ_ASSERT(IsWrappedAsyncFunction(wrapped));
  JSFunction* unwrapped =
      &wrapped->getExtendedSlot(WRAPPED_ASYNC_UNWRAPPED_SLOT)
           .toObject()
           .as<JSFunction>();
  MOZ_ASSERT(unwrapped->isAsync());
  return unwrapped;
}
||||||| merged common ancestors
JSFunction*
js::GetWrappedAsyncFunction(JSFunction* unwrapped)
{
    MOZ_ASSERT(unwrapped->isAsync());
    return &unwrapped->getExtendedSlot(UNWRAPPED_ASYNC_WRAPPED_SLOT).toObject().as<JSFunction>();
}

JSFunction*
js::GetUnwrappedAsyncFunction(JSFunction* wrapped)
{
    MOZ_ASSERT(IsWrappedAsyncFunction(wrapped));
    JSFunction* unwrapped = &wrapped->getExtendedSlot(WRAPPED_ASYNC_UNWRAPPED_SLOT).toObject().as<JSFunction>();
    MOZ_ASSERT(unwrapped->isAsync());
    return unwrapped;
}
=======
  // Starts in the running state.
  obj->setResumeIndex(AbstractGeneratorObject::RESUME_INDEX_RUNNING);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool js::IsWrappedAsyncFunction(JSFunction* fun) {
  return fun->maybeNative() == WrappedAsyncFunction;
||||||| merged common ancestors
bool
js::IsWrappedAsyncFunction(JSFunction* fun)
{
    return fun->maybeNative() == WrappedAsyncFunction;
=======
  return obj;
>>>>>>> upstream-releases
}
