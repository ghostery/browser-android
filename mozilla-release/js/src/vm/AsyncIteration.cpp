/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "vm/AsyncIteration.h"

#include "builtin/Array.h"

#include "builtin/Promise.h"
#include "js/PropertySpec.h"
#include "vm/GeneratorObject.h"
#include "vm/GlobalObject.h"
#include "vm/Interpreter.h"
#include "vm/Realm.h"
#include "vm/SelfHosting.h"

#include "vm/JSContext-inl.h"
#include "vm/JSObject-inl.h"
#include "vm/List-inl.h"

using namespace js;

<<<<<<< HEAD
#define UNWRAPPED_ASYNC_WRAPPED_SLOT 1
#define WRAPPED_ASYNC_UNWRAPPED_SLOT 0

// Async Iteration proposal 8.3.10 Runtime Semantics: EvaluateBody.
static bool WrappedAsyncGenerator(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  RootedFunction wrapped(cx, &args.callee().as<JSFunction>());
  RootedValue unwrappedVal(
      cx, wrapped->getExtendedSlot(WRAPPED_ASYNC_UNWRAPPED_SLOT));

  // Step 1.
  InvokeArgs args2(cx);
  if (!FillArgumentsFromArraylike(cx, args2, args)) {
    return false;
  }

  RootedValue generatorVal(cx);
  if (!Call(cx, unwrappedVal, args.thisv(), args2, &generatorVal)) {
    return false;
  }

  // Step 2.
  AsyncGeneratorObject* asyncGenObj =
      AsyncGeneratorObject::create(cx, wrapped, generatorVal);
  if (!asyncGenObj) {
    return false;
  }

  // Step 3 (skipped).
  // Done in AsyncGeneratorObject::create and generator.

  // Step 4.
  args.rval().setObject(*asyncGenObj);
  return true;
}

JSObject* js::WrapAsyncGeneratorWithProto(JSContext* cx,
                                          HandleFunction unwrapped,
                                          HandleObject proto) {
  MOZ_ASSERT(unwrapped->isAsync());
  MOZ_ASSERT(proto,
             "We need an explicit prototype to avoid the default"
             "%FunctionPrototype% fallback in NewFunctionWithProto().");

  // Create a new function with AsyncGeneratorPrototype, reusing the name and
  // the length of `unwrapped`.

  RootedAtom funName(cx, unwrapped->explicitName());
  uint16_t length;
  if (!JSFunction::getLength(cx, unwrapped, &length)) {
    return nullptr;
  }

  JSFunction* wrapped = NewFunctionWithProto(
      cx, WrappedAsyncGenerator, length, JSFunction::NATIVE_FUN, nullptr,
      funName, proto, gc::AllocKind::FUNCTION_EXTENDED);
  if (!wrapped) {
    return nullptr;
  }

  if (unwrapped->hasInferredName()) {
    wrapped->setInferredName(unwrapped->inferredName());
  }

  // Link them to each other to make GetWrappedAsyncGenerator and
  // GetUnwrappedAsyncGenerator work.
  unwrapped->setExtendedSlot(UNWRAPPED_ASYNC_WRAPPED_SLOT,
                             ObjectValue(*wrapped));
  wrapped->setExtendedSlot(WRAPPED_ASYNC_UNWRAPPED_SLOT,
                           ObjectValue(*unwrapped));

  return wrapped;
}

JSObject* js::WrapAsyncGenerator(JSContext* cx, HandleFunction unwrapped) {
  RootedObject proto(cx,
                     GlobalObject::getOrCreateAsyncGenerator(cx, cx->global()));
  if (!proto) {
    return nullptr;
  }

  return WrapAsyncGeneratorWithProto(cx, unwrapped, proto);
}

bool js::IsWrappedAsyncGenerator(JSFunction* fun) {
  return fun->maybeNative() == WrappedAsyncGenerator;
}

JSFunction* js::GetWrappedAsyncGenerator(JSFunction* unwrapped) {
  MOZ_ASSERT(unwrapped->isAsync());
  return &unwrapped->getExtendedSlot(UNWRAPPED_ASYNC_WRAPPED_SLOT)
              .toObject()
              .as<JSFunction>();
}

JSFunction* js::GetUnwrappedAsyncGenerator(JSFunction* wrapped) {
  MOZ_ASSERT(IsWrappedAsyncGenerator(wrapped));
  JSFunction* unwrapped =
      &wrapped->getExtendedSlot(WRAPPED_ASYNC_UNWRAPPED_SLOT)
           .toObject()
           .as<JSFunction>();
  MOZ_ASSERT(unwrapped->isAsync());
  return unwrapped;
}

// Async Iteration proposal 4.1.1 Await Fulfilled Functions.
MOZ_MUST_USE bool js::AsyncGeneratorAwaitedFulfilled(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    HandleValue value) {
  return AsyncGeneratorResume(cx, asyncGenObj, CompletionKind::Normal, value);
||||||| merged common ancestors
#define UNWRAPPED_ASYNC_WRAPPED_SLOT 1
#define WRAPPED_ASYNC_UNWRAPPED_SLOT 0

// Async Iteration proposal 8.3.10 Runtime Semantics: EvaluateBody.
static bool
WrappedAsyncGenerator(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);

    RootedFunction wrapped(cx, &args.callee().as<JSFunction>());
    RootedValue unwrappedVal(cx, wrapped->getExtendedSlot(WRAPPED_ASYNC_UNWRAPPED_SLOT));

    // Step 1.
    InvokeArgs args2(cx);
    if (!FillArgumentsFromArraylike(cx, args2, args)) {
        return false;
    }

    RootedValue generatorVal(cx);
    if (!Call(cx, unwrappedVal, args.thisv(), args2, &generatorVal)) {
        return false;
    }

    // Step 2.
    AsyncGeneratorObject* asyncGenObj = AsyncGeneratorObject::create(cx, wrapped, generatorVal);
    if (!asyncGenObj) {
        return false;
    }

    // Step 3 (skipped).
    // Done in AsyncGeneratorObject::create and generator.

    // Step 4.
    args.rval().setObject(*asyncGenObj);
    return true;
}

JSObject*
js::WrapAsyncGeneratorWithProto(JSContext* cx, HandleFunction unwrapped, HandleObject proto)
{
    MOZ_ASSERT(unwrapped->isAsync());
    MOZ_ASSERT(proto, "We need an explicit prototype to avoid the default"
                      "%FunctionPrototype% fallback in NewFunctionWithProto().");

    // Create a new function with AsyncGeneratorPrototype, reusing the name and
    // the length of `unwrapped`.

    RootedAtom funName(cx, unwrapped->explicitName());
    uint16_t length;
    if (!JSFunction::getLength(cx, unwrapped, &length)) {
        return nullptr;
    }

    JSFunction* wrapped = NewFunctionWithProto(cx, WrappedAsyncGenerator, length,
                                               JSFunction::NATIVE_FUN, nullptr, funName, proto,
                                               gc::AllocKind::FUNCTION_EXTENDED);
    if (!wrapped) {
        return nullptr;
    }

    if (unwrapped->hasInferredName()) {
        wrapped->setInferredName(unwrapped->inferredName());
    }

    // Link them to each other to make GetWrappedAsyncGenerator and
    // GetUnwrappedAsyncGenerator work.
    unwrapped->setExtendedSlot(UNWRAPPED_ASYNC_WRAPPED_SLOT, ObjectValue(*wrapped));
    wrapped->setExtendedSlot(WRAPPED_ASYNC_UNWRAPPED_SLOT, ObjectValue(*unwrapped));

    return wrapped;
}

JSObject*
js::WrapAsyncGenerator(JSContext* cx, HandleFunction unwrapped)
{
    RootedObject proto(cx, GlobalObject::getOrCreateAsyncGenerator(cx, cx->global()));
    if (!proto) {
        return nullptr;
    }

    return WrapAsyncGeneratorWithProto(cx, unwrapped, proto);
}

bool
js::IsWrappedAsyncGenerator(JSFunction* fun)
{
    return fun->maybeNative() == WrappedAsyncGenerator;
}

JSFunction*
js::GetWrappedAsyncGenerator(JSFunction* unwrapped)
{
    MOZ_ASSERT(unwrapped->isAsync());
    return &unwrapped->getExtendedSlot(UNWRAPPED_ASYNC_WRAPPED_SLOT).toObject().as<JSFunction>();
}

JSFunction*
js::GetUnwrappedAsyncGenerator(JSFunction* wrapped)
{
    MOZ_ASSERT(IsWrappedAsyncGenerator(wrapped));
    JSFunction* unwrapped = &wrapped->getExtendedSlot(WRAPPED_ASYNC_UNWRAPPED_SLOT)
                            .toObject().as<JSFunction>();
    MOZ_ASSERT(unwrapped->isAsync());
    return unwrapped;
}

// Async Iteration proposal 4.1.1 Await Fulfilled Functions.
MOZ_MUST_USE bool
js::AsyncGeneratorAwaitedFulfilled(JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
                                   HandleValue value)
{
    return AsyncGeneratorResume(cx, asyncGenObj, CompletionKind::Normal, value);
=======
// ES2019 draft rev 49b781ec80117b60f73327ef3054703a3111e40c
// 6.2.3.1.1 Await Fulfilled Functions
MOZ_MUST_USE bool js::AsyncGeneratorAwaitedFulfilled(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    HandleValue value) {
  return AsyncGeneratorResume(cx, asyncGenObj, CompletionKind::Normal, value);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Async Iteration proposal 4.1.2 Await Rejected Functions.
MOZ_MUST_USE bool js::AsyncGeneratorAwaitedRejected(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    HandleValue reason) {
  return AsyncGeneratorResume(cx, asyncGenObj, CompletionKind::Throw, reason);
||||||| merged common ancestors
// Async Iteration proposal 4.1.2 Await Rejected Functions.
MOZ_MUST_USE bool
js::AsyncGeneratorAwaitedRejected(JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
                                  HandleValue reason)
{
    return AsyncGeneratorResume(cx, asyncGenObj, CompletionKind::Throw, reason);
=======
// ES2019 draft rev 49b781ec80117b60f73327ef3054703a3111e40c
// 6.2.3.1.2 Await Rejected Functions
MOZ_MUST_USE bool js::AsyncGeneratorAwaitedRejected(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    HandleValue reason) {
  return AsyncGeneratorResume(cx, asyncGenObj, CompletionKind::Throw, reason);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Async Iteration proposal 11.4.3.7 step 8.d-e.
MOZ_MUST_USE bool js::AsyncGeneratorYieldReturnAwaitedFulfilled(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    HandleValue value) {
  return AsyncGeneratorResume(cx, asyncGenObj, CompletionKind::Return, value);
||||||| merged common ancestors
// Async Iteration proposal 11.4.3.7 step 8.d-e.
MOZ_MUST_USE bool
js::AsyncGeneratorYieldReturnAwaitedFulfilled(JSContext* cx,
                                              Handle<AsyncGeneratorObject*> asyncGenObj,
                                              HandleValue value)
{
    return AsyncGeneratorResume(cx, asyncGenObj, CompletionKind::Return, value);
=======
// ES2019 draft rev 49b781ec80117b60f73327ef3054703a3111e40c
// 25.5.3.7 AsyncGeneratorYield, step 8.e.
MOZ_MUST_USE bool js::AsyncGeneratorYieldReturnAwaitedFulfilled(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    HandleValue value) {
  return AsyncGeneratorResume(cx, asyncGenObj, CompletionKind::Return, value);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Async Iteration proposal 11.4.3.7 step 8.d-e.
MOZ_MUST_USE bool js::AsyncGeneratorYieldReturnAwaitedRejected(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    HandleValue reason) {
  return AsyncGeneratorResume(cx, asyncGenObj, CompletionKind::Throw, reason);
||||||| merged common ancestors
// Async Iteration proposal 11.4.3.7 step 8.d-e.
MOZ_MUST_USE bool
js::AsyncGeneratorYieldReturnAwaitedRejected(JSContext* cx,
                                             Handle<AsyncGeneratorObject*> asyncGenObj,
                                             HandleValue reason)
{
    return AsyncGeneratorResume(cx, asyncGenObj, CompletionKind::Throw, reason);
=======
// ES2019 draft rev 49b781ec80117b60f73327ef3054703a3111e40c
// 25.5.3.7 AsyncGeneratorYield, step 8.c.
MOZ_MUST_USE bool js::AsyncGeneratorYieldReturnAwaitedRejected(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    HandleValue reason) {
  return AsyncGeneratorResume(cx, asyncGenObj, CompletionKind::Throw, reason);
>>>>>>> upstream-releases
}

const Class AsyncFromSyncIteratorObject::class_ = {
    "AsyncFromSyncIteratorObject",
<<<<<<< HEAD
    JSCLASS_HAS_RESERVED_SLOTS(AsyncFromSyncIteratorObject::Slots)};

// Async Iteration proposal 11.1.3.1.
JSObject* js::CreateAsyncFromSyncIterator(JSContext* cx, HandleObject iter,
                                          HandleValue nextMethod) {
  // Step 1 (implicit).
  // Done in bytecode emitted by emitAsyncIterator.

  // Steps 2-4.
  return AsyncFromSyncIteratorObject::create(cx, iter, nextMethod);
||||||| merged common ancestors
    JSCLASS_HAS_RESERVED_SLOTS(AsyncFromSyncIteratorObject::Slots)
};

// Async Iteration proposal 11.1.3.1.
JSObject*
js::CreateAsyncFromSyncIterator(JSContext* cx, HandleObject iter, HandleValue nextMethod)
{
    // Step 1 (implicit).
    // Done in bytecode emitted by emitAsyncIterator.

    // Steps 2-4.
    return AsyncFromSyncIteratorObject::create(cx, iter, nextMethod);
=======
    JSCLASS_HAS_RESERVED_SLOTS(AsyncFromSyncIteratorObject::Slots)};

// ES2019 draft rev c012f9c70847559a1d9dc0d35d35b27fec42911e
// 25.1.4.1 CreateAsyncFromSyncIterator
JSObject* js::CreateAsyncFromSyncIterator(JSContext* cx, HandleObject iter,
                                          HandleValue nextMethod) {
  // Steps 1-3.
  return AsyncFromSyncIteratorObject::create(cx, iter, nextMethod);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Async Iteration proposal 11.1.3.1 steps 2-4.
/* static */ JSObject* AsyncFromSyncIteratorObject::create(
    JSContext* cx, HandleObject iter, HandleValue nextMethod) {
  // Step 2.
  RootedObject proto(cx,
                     GlobalObject::getOrCreateAsyncFromSyncIteratorPrototype(
                         cx, cx->global()));
  if (!proto) {
    return nullptr;
  }

  AsyncFromSyncIteratorObject* asyncIter =
      NewObjectWithGivenProto<AsyncFromSyncIteratorObject>(cx, proto);
  if (!asyncIter) {
    return nullptr;
  }

  // Step 3.
  asyncIter->init(iter, nextMethod);

  // Step 4.
  return asyncIter;
||||||| merged common ancestors
// Async Iteration proposal 11.1.3.1 steps 2-4.
/* static */ JSObject*
AsyncFromSyncIteratorObject::create(JSContext* cx, HandleObject iter, HandleValue nextMethod)
{
    // Step 2.
    RootedObject proto(cx, GlobalObject::getOrCreateAsyncFromSyncIteratorPrototype(cx,
                                                                                   cx->global()));
    if (!proto) {
        return nullptr;
    }

    AsyncFromSyncIteratorObject* asyncIter =
        NewObjectWithGivenProto<AsyncFromSyncIteratorObject>(cx, proto);
    if (!asyncIter) {
        return nullptr;
    }

    // Step 3.
    asyncIter->init(iter, nextMethod);

    // Step 4.
    return asyncIter;
=======
// ES2019 draft rev c012f9c70847559a1d9dc0d35d35b27fec42911e
// 25.1.4.1 CreateAsyncFromSyncIterator
/* static */
JSObject* AsyncFromSyncIteratorObject::create(JSContext* cx, HandleObject iter,
                                              HandleValue nextMethod) {
  // Step 1.
  RootedObject proto(cx,
                     GlobalObject::getOrCreateAsyncFromSyncIteratorPrototype(
                         cx, cx->global()));
  if (!proto) {
    return nullptr;
  }

  AsyncFromSyncIteratorObject* asyncIter =
      NewObjectWithGivenProto<AsyncFromSyncIteratorObject>(cx, proto);
  if (!asyncIter) {
    return nullptr;
  }

  // Step 2.
  asyncIter->init(iter, nextMethod);

  // Step 3 (Call to 7.4.1 GetIterator).
  // 7.4.1 GetIterator, steps 1-5 are a no-op (*).
  // 7.4.1 GetIterator, steps 6-8 are implemented in bytecode.
  //
  // (*) With <https://github.com/tc39/ecma262/issues/1172> fixed.
  return asyncIter;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Async Iteration proposal 11.1.3.2.1 %AsyncFromSyncIteratorPrototype%.next.
static bool AsyncFromSyncIteratorNext(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  return AsyncFromSyncIteratorMethod(cx, args, CompletionKind::Normal);
||||||| merged common ancestors
// Async Iteration proposal 11.1.3.2.1 %AsyncFromSyncIteratorPrototype%.next.
static bool
AsyncFromSyncIteratorNext(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
    return AsyncFromSyncIteratorMethod(cx, args, CompletionKind::Normal);
=======
// ES2019 draft rev c012f9c70847559a1d9dc0d35d35b27fec42911e
// 25.1.4.2.1 %AsyncFromSyncIteratorPrototype%.next
static bool AsyncFromSyncIteratorNext(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  return AsyncFromSyncIteratorMethod(cx, args, CompletionKind::Normal);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Async Iteration proposal 11.1.3.2.2 %AsyncFromSyncIteratorPrototype%.return.
static bool AsyncFromSyncIteratorReturn(JSContext* cx, unsigned argc,
                                        Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  return AsyncFromSyncIteratorMethod(cx, args, CompletionKind::Return);
||||||| merged common ancestors
// Async Iteration proposal 11.1.3.2.2 %AsyncFromSyncIteratorPrototype%.return.
static bool
AsyncFromSyncIteratorReturn(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
    return AsyncFromSyncIteratorMethod(cx, args, CompletionKind::Return);
=======
// ES2019 draft rev c012f9c70847559a1d9dc0d35d35b27fec42911e
// 25.1.4.2.2 %AsyncFromSyncIteratorPrototype%.return
static bool AsyncFromSyncIteratorReturn(JSContext* cx, unsigned argc,
                                        Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  return AsyncFromSyncIteratorMethod(cx, args, CompletionKind::Return);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Async Iteration proposal 11.1.3.2.3 %AsyncFromSyncIteratorPrototype%.throw.
static bool AsyncFromSyncIteratorThrow(JSContext* cx, unsigned argc,
                                       Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  return AsyncFromSyncIteratorMethod(cx, args, CompletionKind::Throw);
||||||| merged common ancestors
// Async Iteration proposal 11.1.3.2.3 %AsyncFromSyncIteratorPrototype%.throw.
static bool
AsyncFromSyncIteratorThrow(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
    return AsyncFromSyncIteratorMethod(cx, args, CompletionKind::Throw);
=======
// ES2019 draft rev c012f9c70847559a1d9dc0d35d35b27fec42911e
// 25.1.4.2.3 %AsyncFromSyncIteratorPrototype%.throw
static bool AsyncFromSyncIteratorThrow(JSContext* cx, unsigned argc,
                                       Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  return AsyncFromSyncIteratorMethod(cx, args, CompletionKind::Throw);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Async Iteration proposal 11.4.1.2 AsyncGenerator.prototype.next.
static bool AsyncGeneratorNext(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Steps 1-3.
  return AsyncGeneratorEnqueue(cx, args.thisv(), CompletionKind::Normal,
                               args.get(0), args.rval());
||||||| merged common ancestors
// Async Iteration proposal 11.4.1.2 AsyncGenerator.prototype.next.
static bool
AsyncGeneratorNext(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);

    // Steps 1-3.
    return AsyncGeneratorEnqueue(cx, args.thisv(), CompletionKind::Normal, args.get(0),
                                 args.rval());
=======
// ES2019 draft rev c012f9c70847559a1d9dc0d35d35b27fec42911e
// 25.5.1.2 AsyncGenerator.prototype.next
static bool AsyncGeneratorNext(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Steps 1-3.
  return AsyncGeneratorEnqueue(cx, args.thisv(), CompletionKind::Normal,
                               args.get(0), args.rval());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Async Iteration proposal 11.4.1.3 AsyncGenerator.prototype.return.
static bool AsyncGeneratorReturn(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Steps 1-3.
  return AsyncGeneratorEnqueue(cx, args.thisv(), CompletionKind::Return,
                               args.get(0), args.rval());
||||||| merged common ancestors
// Async Iteration proposal 11.4.1.3 AsyncGenerator.prototype.return.
static bool
AsyncGeneratorReturn(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);

    // Steps 1-3.
    return AsyncGeneratorEnqueue(cx, args.thisv(), CompletionKind::Return, args.get(0),
                                 args.rval());
=======
// ES2019 draft rev c012f9c70847559a1d9dc0d35d35b27fec42911e
// 25.5.1.3 AsyncGenerator.prototype.return
static bool AsyncGeneratorReturn(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Steps 1-3.
  return AsyncGeneratorEnqueue(cx, args.thisv(), CompletionKind::Return,
                               args.get(0), args.rval());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Async Iteration proposal 11.4.1.4 AsyncGenerator.prototype.throw.
static bool AsyncGeneratorThrow(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Steps 1-3.
  return AsyncGeneratorEnqueue(cx, args.thisv(), CompletionKind::Throw,
                               args.get(0), args.rval());
||||||| merged common ancestors
// Async Iteration proposal 11.4.1.4 AsyncGenerator.prototype.throw.
static bool
AsyncGeneratorThrow(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);

    // Steps 1-3.
    return AsyncGeneratorEnqueue(cx, args.thisv(), CompletionKind::Throw, args.get(0),
                                 args.rval());
=======
// ES2019 draft rev c012f9c70847559a1d9dc0d35d35b27fec42911e
// 25.5.1.4 AsyncGenerator.prototype.throw
static bool AsyncGeneratorThrow(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Steps 1-3.
  return AsyncGeneratorEnqueue(cx, args.thisv(), CompletionKind::Throw,
                               args.get(0), args.rval());
>>>>>>> upstream-releases
}

const Class AsyncGeneratorObject::class_ = {
    "AsyncGenerator", JSCLASS_HAS_RESERVED_SLOTS(AsyncGeneratorObject::Slots)};

// ES 2017 draft 9.1.13.
// OrdinaryCreateFromConstructor specialized for AsyncGeneratorObjects.
static AsyncGeneratorObject* OrdinaryCreateFromConstructorAsynGen(
    JSContext* cx, HandleFunction fun) {
  // Step 1 (skipped).

  // Step 2.
  RootedValue protoVal(cx);
  if (!GetProperty(cx, fun, fun, cx->names().prototype, &protoVal)) {
    return nullptr;
  }

  RootedObject proto(cx, protoVal.isObject() ? &protoVal.toObject() : nullptr);
  if (!proto) {
    proto = GlobalObject::getOrCreateAsyncGeneratorPrototype(cx, cx->global());
    if (!proto) {
      return nullptr;
    }
  }

  // Step 3.
  return NewObjectWithGivenProto<AsyncGeneratorObject>(cx, proto);
}

<<<<<<< HEAD
/* static */ AsyncGeneratorObject* AsyncGeneratorObject::create(
    JSContext* cx, HandleFunction asyncGen, HandleValue generatorVal) {
  MOZ_ASSERT(generatorVal.isObject());
  MOZ_ASSERT(generatorVal.toObject().is<GeneratorObject>());
||||||| merged common ancestors
/* static */ AsyncGeneratorObject*
AsyncGeneratorObject::create(JSContext* cx, HandleFunction asyncGen, HandleValue generatorVal)
{
    MOZ_ASSERT(generatorVal.isObject());
    MOZ_ASSERT(generatorVal.toObject().is<GeneratorObject>());
=======
/* static */
AsyncGeneratorObject* AsyncGeneratorObject::create(JSContext* cx,
                                                   HandleFunction asyncGen) {
  MOZ_ASSERT(asyncGen->isAsync() && asyncGen->isGenerator());
>>>>>>> upstream-releases

  AsyncGeneratorObject* asyncGenObj =
      OrdinaryCreateFromConstructorAsynGen(cx, asyncGen);
  if (!asyncGenObj) {
    return nullptr;
  }

<<<<<<< HEAD
  // Async Iteration proposal 6.4.3.2 AsyncGeneratorStart.
  // Step 6.
  asyncGenObj->setGenerator(generatorVal);
||||||| merged common ancestors
    // Async Iteration proposal 6.4.3.2 AsyncGeneratorStart.
    // Step 6.
    asyncGenObj->setGenerator(generatorVal);
=======
  // ES2019 draft rev c2aad21fee7f5ddc89fdf7d3d305618ca3a13242
  // 25.5.3.2 AsyncGeneratorStart.
>>>>>>> upstream-releases

  // Step 7.
  asyncGenObj->setSuspendedStart();

  // Step 8.
  asyncGenObj->clearSingleQueueRequest();

<<<<<<< HEAD
  asyncGenObj->clearCachedRequest();

  return asyncGenObj;
}
||||||| merged common ancestors
    asyncGenObj->clearCachedRequest();
=======
  asyncGenObj->clearCachedRequest();
>>>>>>> upstream-releases

<<<<<<< HEAD
/* static */ AsyncGeneratorRequest* AsyncGeneratorObject::createRequest(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    CompletionKind completionKind, HandleValue completionValue,
    Handle<PromiseObject*> promise) {
  if (!asyncGenObj->hasCachedRequest()) {
    return AsyncGeneratorRequest::create(cx, completionKind, completionValue,
                                         promise);
  }

  AsyncGeneratorRequest* request = asyncGenObj->takeCachedRequest();
  request->init(completionKind, completionValue, promise);
  return request;
||||||| merged common ancestors
    return asyncGenObj;
=======
  return asyncGenObj;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ MOZ_MUST_USE bool AsyncGeneratorObject::enqueueRequest(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    Handle<AsyncGeneratorRequest*> request) {
  if (asyncGenObj->isSingleQueue()) {
    if (asyncGenObj->isSingleQueueEmpty()) {
      asyncGenObj->setSingleQueueRequest(request);
      return true;
||||||| merged common ancestors
/* static */ AsyncGeneratorRequest*
AsyncGeneratorObject::createRequest(JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
                                    CompletionKind completionKind, HandleValue completionValue,
                                    Handle<PromiseObject*> promise)
{
    if (!asyncGenObj->hasCachedRequest()) {
        return AsyncGeneratorRequest::create(cx, completionKind, completionValue, promise);
=======
/* static */
AsyncGeneratorRequest* AsyncGeneratorObject::createRequest(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    CompletionKind completionKind, HandleValue completionValue,
    Handle<PromiseObject*> promise) {
  if (!asyncGenObj->hasCachedRequest()) {
    return AsyncGeneratorRequest::create(cx, completionKind, completionValue,
                                         promise);
  }

  AsyncGeneratorRequest* request = asyncGenObj->takeCachedRequest();
  request->init(completionKind, completionValue, promise);
  return request;
}

/* static */ MOZ_MUST_USE bool AsyncGeneratorObject::enqueueRequest(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    Handle<AsyncGeneratorRequest*> request) {
  if (asyncGenObj->isSingleQueue()) {
    if (asyncGenObj->isSingleQueueEmpty()) {
      asyncGenObj->setSingleQueueRequest(request);
      return true;
>>>>>>> upstream-releases
    }

    Rooted<ListObject*> queue(cx, ListObject::create(cx));
    if (!queue) {
      return false;
    }

<<<<<<< HEAD
    RootedValue requestVal(cx, ObjectValue(*asyncGenObj->singleQueueRequest()));
    if (!queue->append(cx, requestVal)) {
      return false;
||||||| merged common ancestors
/* static */ MOZ_MUST_USE bool
AsyncGeneratorObject::enqueueRequest(JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
                                     Handle<AsyncGeneratorRequest*> request)
{
    if (asyncGenObj->isSingleQueue()) {
        if (asyncGenObj->isSingleQueueEmpty()) {
            asyncGenObj->setSingleQueueRequest(request);
            return true;
        }

        RootedNativeObject queue(cx, NewList(cx));
        if (!queue) {
            return false;
        }

        RootedValue requestVal(cx, ObjectValue(*asyncGenObj->singleQueueRequest()));
        if (!AppendToList(cx, queue, requestVal)) {
            return false;
        }
        requestVal = ObjectValue(*request);
        if (!AppendToList(cx, queue, requestVal)) {
            return false;
        }

        asyncGenObj->setQueue(queue);
        return true;
=======
    RootedValue requestVal(cx, ObjectValue(*asyncGenObj->singleQueueRequest()));
    if (!queue->append(cx, requestVal)) {
      return false;
    }
    requestVal = ObjectValue(*request);
    if (!queue->append(cx, requestVal)) {
      return false;
>>>>>>> upstream-releases
    }
    requestVal = ObjectValue(*request);
    if (!queue->append(cx, requestVal)) {
      return false;
    }

    asyncGenObj->setQueue(queue);
    return true;
  }

<<<<<<< HEAD
  Rooted<ListObject*> queue(cx, asyncGenObj->queue());
  RootedValue requestVal(cx, ObjectValue(*request));
  return queue->append(cx, requestVal);
||||||| merged common ancestors
    RootedNativeObject queue(cx, asyncGenObj->queue());
    RootedValue requestVal(cx, ObjectValue(*request));
    return AppendToList(cx, queue, requestVal);
=======
    asyncGenObj->setQueue(queue);
    return true;
  }

  Rooted<ListObject*> queue(cx, asyncGenObj->queue());
  RootedValue requestVal(cx, ObjectValue(*request));
  return queue->append(cx, requestVal);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ AsyncGeneratorRequest* AsyncGeneratorObject::dequeueRequest(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj) {
  if (asyncGenObj->isSingleQueue()) {
    AsyncGeneratorRequest* request = asyncGenObj->singleQueueRequest();
    asyncGenObj->clearSingleQueueRequest();
    return request;
  }
||||||| merged common ancestors
/* static */ AsyncGeneratorRequest*
AsyncGeneratorObject::dequeueRequest(JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj)
{
    if (asyncGenObj->isSingleQueue()) {
        AsyncGeneratorRequest* request = asyncGenObj->singleQueueRequest();
        asyncGenObj->clearSingleQueueRequest();
        return request;
    }
=======
/* static */
AsyncGeneratorRequest* AsyncGeneratorObject::dequeueRequest(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj) {
  if (asyncGenObj->isSingleQueue()) {
    AsyncGeneratorRequest* request = asyncGenObj->singleQueueRequest();
    asyncGenObj->clearSingleQueueRequest();
    return request;
  }
>>>>>>> upstream-releases

  Rooted<ListObject*> queue(cx, asyncGenObj->queue());
  return &queue->popFirstAs<AsyncGeneratorRequest>(cx);
}

<<<<<<< HEAD
/* static */ AsyncGeneratorRequest* AsyncGeneratorObject::peekRequest(
    Handle<AsyncGeneratorObject*> asyncGenObj) {
  if (asyncGenObj->isSingleQueue()) {
    return asyncGenObj->singleQueueRequest();
  }
||||||| merged common ancestors
/* static */ AsyncGeneratorRequest*
AsyncGeneratorObject::peekRequest(Handle<AsyncGeneratorObject*> asyncGenObj)
{
    if (asyncGenObj->isSingleQueue()) {
        return asyncGenObj->singleQueueRequest();
    }
=======
/* static */
AsyncGeneratorRequest* AsyncGeneratorObject::peekRequest(
    Handle<AsyncGeneratorObject*> asyncGenObj) {
  if (asyncGenObj->isSingleQueue()) {
    return asyncGenObj->singleQueueRequest();
  }
>>>>>>> upstream-releases

  return &asyncGenObj->queue()->getAs<AsyncGeneratorRequest>(0);
}

const Class AsyncGeneratorRequest::class_ = {
    "AsyncGeneratorRequest",
<<<<<<< HEAD
    JSCLASS_HAS_RESERVED_SLOTS(AsyncGeneratorRequest::Slots)};

// Async Iteration proposal 11.4.3.1.
/* static */ AsyncGeneratorRequest* AsyncGeneratorRequest::create(
    JSContext* cx, CompletionKind completionKind, HandleValue completionValue,
    Handle<PromiseObject*> promise) {
  AsyncGeneratorRequest* request =
      NewObjectWithGivenProto<AsyncGeneratorRequest>(cx, nullptr);
  if (!request) {
    return nullptr;
  }

  request->init(completionKind, completionValue, promise);
  return request;
||||||| merged common ancestors
    JSCLASS_HAS_RESERVED_SLOTS(AsyncGeneratorRequest::Slots)
};

// Async Iteration proposal 11.4.3.1.
/* static */ AsyncGeneratorRequest*
AsyncGeneratorRequest::create(JSContext* cx, CompletionKind completionKind,
                              HandleValue completionValue, Handle<PromiseObject*> promise)
{
    AsyncGeneratorRequest* request = NewObjectWithGivenProto<AsyncGeneratorRequest>(cx, nullptr);
    if (!request) {
        return nullptr;
    }

    request->init(completionKind, completionValue, promise);
    return request;
=======
    JSCLASS_HAS_RESERVED_SLOTS(AsyncGeneratorRequest::Slots)};

// ES2019 draft rev c012f9c70847559a1d9dc0d35d35b27fec42911e
// 25.5.3.1 AsyncGeneratorRequest Records
/* static */
AsyncGeneratorRequest* AsyncGeneratorRequest::create(
    JSContext* cx, CompletionKind completionKind, HandleValue completionValue,
    Handle<PromiseObject*> promise) {
  AsyncGeneratorRequest* request =
      NewObjectWithGivenProto<AsyncGeneratorRequest>(cx, nullptr);
  if (!request) {
    return nullptr;
  }

  request->init(completionKind, completionValue, promise);
  return request;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Async Iteration proposal 11.4.3.2 AsyncGeneratorStart steps 5.d-g.
static MOZ_MUST_USE bool AsyncGeneratorReturned(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    HandleValue value) {
  // Step 5.d.
  asyncGenObj->setCompleted();
||||||| merged common ancestors
// Async Iteration proposal 11.4.3.2 AsyncGeneratorStart steps 5.d-g.
static MOZ_MUST_USE bool
AsyncGeneratorReturned(JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
                       HandleValue value)
{
    // Step 5.d.
    asyncGenObj->setCompleted();

    // Step 5.e (done in bytecode).
    // Step 5.f.i (implicit).

    // Step 5.g.
    return AsyncGeneratorResolve(cx, asyncGenObj, value, true);
}
=======
// ES2019 draft rev c012f9c70847559a1d9dc0d35d35b27fec42911e
// 25.5.3.2 AsyncGeneratorStart
static MOZ_MUST_USE bool AsyncGeneratorReturned(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    HandleValue value) {
  // Step 5.d.
  asyncGenObj->setCompleted();

  // Step 5.e (done in bytecode).
  // Step 5.f.i (implicit).

  // Step 5.g.
  return AsyncGeneratorResolve(cx, asyncGenObj, value, true);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 5.e (done in bytecode).
  // Step 5.f.i (implicit).

  // Step 5.g.
  return AsyncGeneratorResolve(cx, asyncGenObj, value, true);
}

// Async Iteration proposal 11.4.3.2 AsyncGeneratorStart steps 5.d, f.
static MOZ_MUST_USE bool AsyncGeneratorThrown(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj) {
  // Step 5.d.
  asyncGenObj->setCompleted();

  // Not much we can do about uncatchable exceptions, so just bail.
  if (!cx->isExceptionPending()) {
    return false;
  }

  // Step 5.f.i.
  RootedValue value(cx);
  if (!GetAndClearException(cx, &value)) {
    return false;
  }

  // Step 5.f.ii.
  return AsyncGeneratorReject(cx, asyncGenObj, value);
||||||| merged common ancestors
// Async Iteration proposal 11.4.3.2 AsyncGeneratorStart steps 5.d, f.
static MOZ_MUST_USE bool
AsyncGeneratorThrown(JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj)
{
    // Step 5.d.
    asyncGenObj->setCompleted();

    // Not much we can do about uncatchable exceptions, so just bail.
    if (!cx->isExceptionPending()) {
        return false;
    }

    // Step 5.f.i.
    RootedValue value(cx);
    if (!GetAndClearException(cx, &value)) {
        return false;
    }

    // Step 5.f.ii.
    return AsyncGeneratorReject(cx, asyncGenObj, value);
=======
// ES2019 draft rev c012f9c70847559a1d9dc0d35d35b27fec42911e
// 25.5.3.2 AsyncGeneratorStart
static MOZ_MUST_USE bool AsyncGeneratorThrown(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj) {
  // Step 5.d.
  asyncGenObj->setCompleted();

  // Not much we can do about uncatchable exceptions, so just bail.
  if (!cx->isExceptionPending()) {
    return false;
  }

  // Step 5.f.i.
  RootedValue value(cx);
  if (!GetAndClearException(cx, &value)) {
    return false;
  }

  // Step 5.f.ii.
  return AsyncGeneratorReject(cx, asyncGenObj, value);
>>>>>>> upstream-releases
}

// ES2019 draft rev c012f9c70847559a1d9dc0d35d35b27fec42911e
// 25.5.3.7 AsyncGeneratorYield (partially)
// Most steps are done in generator.
static MOZ_MUST_USE bool AsyncGeneratorYield(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    HandleValue value) {
  // Step 5 is done in bytecode.

  // Step 6.
  asyncGenObj->setSuspendedYield();

  // Step 9.
  return AsyncGeneratorResolve(cx, asyncGenObj, value, false);
}

<<<<<<< HEAD
// Async Iteration proposal 4.1 Await steps 2-9.
// Async Iteration proposal 8.2.1 yield* steps 6.a.vii, 6.b.ii.7, 6.c.ix.
// Async Iteration proposal 11.4.3.2 AsyncGeneratorStart step 5.f-g.
// Async Iteration proposal 11.4.3.5 AsyncGeneratorResumeNext
//   steps 12-14, 16-20.
// Execution context switching is handled in generator.
MOZ_MUST_USE bool js::AsyncGeneratorResume(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    CompletionKind completionKind, HandleValue argument) {
  RootedValue generatorVal(cx, asyncGenObj->generatorVal());

  // 11.4.3.5 steps 12-14, 16-20.
  HandlePropertyName funName = completionKind == CompletionKind::Normal
                                   ? cx->names().GeneratorNext
                                   : completionKind == CompletionKind::Throw
                                         ? cx->names().GeneratorThrow
                                         : cx->names().GeneratorReturn;
  FixedInvokeArgs<1> args(cx);
  args[0].set(argument);
  RootedValue result(cx);
  if (!CallSelfHostedFunction(cx, funName, generatorVal, args, &result)) {
    // 11.4.3.2 step 5.d, f.
    return AsyncGeneratorThrown(cx, asyncGenObj);
  }

  // 4.1 steps 2-9.
  if (asyncGenObj->generatorObj()->isAfterAwait()) {
    return AsyncGeneratorAwait(cx, asyncGenObj, result);
  }

  // The following code corresponds to the following 3 cases:
  //   * yield
  //   * yield*
  //   * return
  // For yield and return, property access is done on an internal result
  // object and it's not observable.
  // For yield*, it's done on a possibly user-provided result object, and
  // it's observable.
  //
  // Note that IteratorComplete steps in 8.2.1 are done in bytecode.

  // 8.2.1 yield* steps 6.a.vii, 6.b.ii.7, 6.c.ix.
  RootedObject resultObj(cx, &result.toObject());
  RootedValue value(cx);
  if (!GetProperty(cx, resultObj, resultObj, cx->names().value, &value)) {
    return false;
  }

  if (asyncGenObj->generatorObj()->isAfterYield()) {
    return AsyncGeneratorYield(cx, asyncGenObj, value);
  }

  // 11.4.3.2 step 5.d-g.
  return AsyncGeneratorReturned(cx, asyncGenObj, value);
||||||| merged common ancestors
// Async Iteration proposal 4.1 Await steps 2-9.
// Async Iteration proposal 8.2.1 yield* steps 6.a.vii, 6.b.ii.7, 6.c.ix.
// Async Iteration proposal 11.4.3.2 AsyncGeneratorStart step 5.f-g.
// Async Iteration proposal 11.4.3.5 AsyncGeneratorResumeNext
//   steps 12-14, 16-20.
// Execution context switching is handled in generator.
MOZ_MUST_USE bool
js::AsyncGeneratorResume(JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
                         CompletionKind completionKind, HandleValue argument)
{
    RootedValue generatorVal(cx, asyncGenObj->generatorVal());

    // 11.4.3.5 steps 12-14, 16-20.
    HandlePropertyName funName = completionKind == CompletionKind::Normal
                                 ? cx->names().GeneratorNext
                                 : completionKind == CompletionKind::Throw
                                 ? cx->names().GeneratorThrow
                                 : cx->names().GeneratorReturn;
    FixedInvokeArgs<1> args(cx);
    args[0].set(argument);
    RootedValue result(cx);
    if (!CallSelfHostedFunction(cx, funName, generatorVal, args, &result)) {
        // 11.4.3.2 step 5.d, f.
        return AsyncGeneratorThrown(cx, asyncGenObj);
    }

    // 4.1 steps 2-9.
    if (asyncGenObj->generatorObj()->isAfterAwait()) {
        return AsyncGeneratorAwait(cx, asyncGenObj, result);
    }

    // The following code corresponds to the following 3 cases:
    //   * yield
    //   * yield*
    //   * return
    // For yield and return, property access is done on an internal result
    // object and it's not observable.
    // For yield*, it's done on a possibly user-provided result object, and
    // it's observable.
    //
    // Note that IteratorComplete steps in 8.2.1 are done in bytecode.

    // 8.2.1 yield* steps 6.a.vii, 6.b.ii.7, 6.c.ix.
    RootedObject resultObj(cx, &result.toObject());
    RootedValue value(cx);
    if (!GetProperty(cx, resultObj, resultObj, cx->names().value, &value)) {
        return false;
    }

    if (asyncGenObj->generatorObj()->isAfterYield()) {
        return AsyncGeneratorYield(cx, asyncGenObj, value);
    }

    // 11.4.3.2 step 5.d-g.
    return AsyncGeneratorReturned(cx, asyncGenObj, value);
=======
// ES2019 draft rev c012f9c70847559a1d9dc0d35d35b27fec42911e
// 6.2.3.1 Await, steps 2-9.
// 14.4.13 RS: Evaluation, yield*, steps 7.a.vi, 7.b.ii.7, 7.c.ix.
// 25.5.3.2 AsyncGeneratorStart, steps 5.d-g.
// 25.5.3.5 AsyncGeneratorResumeNext, steps 12-20.
// 25.5.3.7 AsyncGeneratorYield, steps 5-6, 9.
//
// Note: Execution context switching is handled in generator.
MOZ_MUST_USE bool js::AsyncGeneratorResume(
    JSContext* cx, Handle<AsyncGeneratorObject*> asyncGenObj,
    CompletionKind completionKind, HandleValue argument) {
  MOZ_ASSERT(!asyncGenObj->isClosed(),
             "closed generator when resuming async generator");
  MOZ_ASSERT(asyncGenObj->isSuspended(),
             "non-suspended generator when resuming async generator");
  MOZ_ASSERT(asyncGenObj->isExecuting(),
             "async generator not set into 'executing' state");

  // 25.5.3.5, steps 12-14, 16-20.
  HandlePropertyName funName = completionKind == CompletionKind::Normal
                                   ? cx->names().AsyncGeneratorNext
                                   : completionKind == CompletionKind::Throw
                                         ? cx->names().AsyncGeneratorThrow
                                         : cx->names().AsyncGeneratorReturn;
  FixedInvokeArgs<1> args(cx);
  args[0].set(argument);
  RootedValue thisOrRval(cx, ObjectValue(*asyncGenObj));
  if (!CallSelfHostedFunction(cx, funName, thisOrRval, args, &thisOrRval)) {
    // 25.5.3.2, steps 5.f, 5.g.
    if (!asyncGenObj->isClosed()) {
      asyncGenObj->setClosed();
    }
    return AsyncGeneratorThrown(cx, asyncGenObj);
  }

  // 6.2.3.1, steps 2-9.
  if (asyncGenObj->isAfterAwait()) {
    return AsyncGeneratorAwait(cx, asyncGenObj, thisOrRval);
  }

  // 25.5.3.7, steps 5-6, 9.
  if (asyncGenObj->isAfterYield()) {
    return AsyncGeneratorYield(cx, asyncGenObj, thisOrRval);
  }

  // 25.5.3.2, steps 5.d-g.
  return AsyncGeneratorReturned(cx, asyncGenObj, thisOrRval);
>>>>>>> upstream-releases
}

static const JSFunctionSpec async_iterator_proto_methods[] = {
    JS_SELF_HOSTED_SYM_FN(asyncIterator, "AsyncIteratorIdentity", 0, 0),
    JS_FS_END};

static const JSFunctionSpec async_from_sync_iter_methods[] = {
    JS_FN("next", AsyncFromSyncIteratorNext, 1, 0),
    JS_FN("throw", AsyncFromSyncIteratorThrow, 1, 0),
    JS_FN("return", AsyncFromSyncIteratorReturn, 1, 0), JS_FS_END};

static const JSFunctionSpec async_generator_methods[] = {
    JS_FN("next", AsyncGeneratorNext, 1, 0),
    JS_FN("throw", AsyncGeneratorThrow, 1, 0),
    JS_FN("return", AsyncGeneratorReturn, 1, 0), JS_FS_END};

/* static */ MOZ_MUST_USE bool GlobalObject::initAsyncGenerators(
    JSContext* cx, Handle<GlobalObject*> global) {
  if (global->getReservedSlot(ASYNC_ITERATOR_PROTO).isObject()) {
    return true;
<<<<<<< HEAD
  }

  // Async Iteration proposal 11.1.2 %AsyncIteratorPrototype%.
  RootedObject asyncIterProto(
      cx, GlobalObject::createBlankPrototype<PlainObject>(cx, global));
  if (!asyncIterProto) {
    return false;
  }
  if (!DefinePropertiesAndFunctions(cx, asyncIterProto, nullptr,
                                    async_iterator_proto_methods)) {
    return false;
  }

  // Async Iteration proposal 11.1.3.2 %AsyncFromSyncIteratorPrototype%.
  RootedObject asyncFromSyncIterProto(
      cx, GlobalObject::createBlankPrototypeInheriting(cx, &PlainObject::class_,
                                                       asyncIterProto));
  if (!asyncFromSyncIterProto) {
    return false;
  }
  if (!DefinePropertiesAndFunctions(cx, asyncFromSyncIterProto, nullptr,
                                    async_from_sync_iter_methods) ||
      !DefineToStringTag(cx, asyncFromSyncIterProto,
                         cx->names().AsyncFromSyncIterator)) {
    return false;
  }

  // Async Iteration proposal 11.4.1 %AsyncGeneratorPrototype%.
  RootedObject asyncGenProto(cx, GlobalObject::createBlankPrototypeInheriting(
                                     cx, &PlainObject::class_, asyncIterProto));
  if (!asyncGenProto) {
    return false;
  }
  if (!DefinePropertiesAndFunctions(cx, asyncGenProto, nullptr,
                                    async_generator_methods) ||
      !DefineToStringTag(cx, asyncGenProto, cx->names().AsyncGenerator)) {
    return false;
  }

  // Async Iteration proposal 11.3.3 %AsyncGenerator%.
  RootedObject asyncGenerator(
      cx, NewSingletonObjectWithFunctionPrototype(cx, global));
  if (!asyncGenerator) {
    return false;
  }
  if (!JSObject::setDelegate(cx, asyncGenerator)) {
    return false;
  }
  if (!LinkConstructorAndPrototype(cx, asyncGenerator, asyncGenProto,
                                   JSPROP_READONLY, JSPROP_READONLY) ||
      !DefineToStringTag(cx, asyncGenerator,
                         cx->names().AsyncGeneratorFunction)) {
    return false;
  }

  RootedValue function(cx, global->getConstructor(JSProto_Function));
  if (!function.toObjectOrNull()) {
    return false;
  }
  RootedObject proto(cx, &function.toObject());
  RootedAtom name(cx, cx->names().AsyncGeneratorFunction);

  // Async Iteration proposal 11.3.2 %AsyncGeneratorFunction%.
  RootedObject asyncGenFunction(
      cx, NewFunctionWithProto(cx, AsyncGeneratorConstructor, 1,
                               JSFunction::NATIVE_CTOR, nullptr, name, proto,
                               gc::AllocKind::FUNCTION, SingletonObject));
  if (!asyncGenFunction) {
    return false;
  }
  if (!LinkConstructorAndPrototype(cx, asyncGenFunction, asyncGenerator,
                                   JSPROP_PERMANENT | JSPROP_READONLY,
                                   JSPROP_READONLY)) {
    return false;
  }

  global->setReservedSlot(ASYNC_ITERATOR_PROTO, ObjectValue(*asyncIterProto));
  global->setReservedSlot(ASYNC_FROM_SYNC_ITERATOR_PROTO,
                          ObjectValue(*asyncFromSyncIterProto));
  global->setReservedSlot(ASYNC_GENERATOR, ObjectValue(*asyncGenerator));
  global->setReservedSlot(ASYNC_GENERATOR_FUNCTION,
                          ObjectValue(*asyncGenFunction));
  global->setReservedSlot(ASYNC_GENERATOR_PROTO, ObjectValue(*asyncGenProto));
  return true;
||||||| merged common ancestors
=======
  }

  // 25.1.3 The %AsyncIteratorPrototype% Object
  RootedObject asyncIterProto(
      cx, GlobalObject::createBlankPrototype<PlainObject>(cx, global));
  if (!asyncIterProto) {
    return false;
  }
  if (!DefinePropertiesAndFunctions(cx, asyncIterProto, nullptr,
                                    async_iterator_proto_methods)) {
    return false;
  }

  // 25.1.4.2 The %AsyncFromSyncIteratorPrototype% Object
  RootedObject asyncFromSyncIterProto(
      cx, GlobalObject::createBlankPrototypeInheriting(cx, &PlainObject::class_,
                                                       asyncIterProto));
  if (!asyncFromSyncIterProto) {
    return false;
  }
  if (!DefinePropertiesAndFunctions(cx, asyncFromSyncIterProto, nullptr,
                                    async_from_sync_iter_methods) ||
      !DefineToStringTag(cx, asyncFromSyncIterProto,
                         cx->names().AsyncFromSyncIterator)) {
    return false;
  }

  // 25.5 AsyncGenerator Objects
  RootedObject asyncGenProto(cx, GlobalObject::createBlankPrototypeInheriting(
                                     cx, &PlainObject::class_, asyncIterProto));
  if (!asyncGenProto) {
    return false;
  }
  if (!DefinePropertiesAndFunctions(cx, asyncGenProto, nullptr,
                                    async_generator_methods) ||
      !DefineToStringTag(cx, asyncGenProto, cx->names().AsyncGenerator)) {
    return false;
  }

  // 25.3.3 Properties of the AsyncGeneratorFunction Prototype Object
  RootedObject asyncGenerator(
      cx, NewSingletonObjectWithFunctionPrototype(cx, global));
  if (!asyncGenerator) {
    return false;
  }
  if (!LinkConstructorAndPrototype(cx, asyncGenerator, asyncGenProto,
                                   JSPROP_READONLY, JSPROP_READONLY) ||
      !DefineToStringTag(cx, asyncGenerator,
                         cx->names().AsyncGeneratorFunction)) {
    return false;
  }

  RootedObject proto(
      cx, GlobalObject::getOrCreateFunctionConstructor(cx, cx->global()));
  if (!proto) {
    return false;
  }
  HandlePropertyName name = cx->names().AsyncGeneratorFunction;

  // 25.3.1 The AsyncGeneratorFunction Constructor
  RootedObject asyncGenFunction(
      cx, NewFunctionWithProto(cx, AsyncGeneratorConstructor, 1,
                               JSFunction::NATIVE_CTOR, nullptr, name, proto,
                               gc::AllocKind::FUNCTION, SingletonObject));
  if (!asyncGenFunction) {
    return false;
  }
  if (!LinkConstructorAndPrototype(cx, asyncGenFunction, asyncGenerator,
                                   JSPROP_PERMANENT | JSPROP_READONLY,
                                   JSPROP_READONLY)) {
    return false;
  }

  global->setReservedSlot(ASYNC_ITERATOR_PROTO, ObjectValue(*asyncIterProto));
  global->setReservedSlot(ASYNC_FROM_SYNC_ITERATOR_PROTO,
                          ObjectValue(*asyncFromSyncIterProto));
  global->setReservedSlot(ASYNC_GENERATOR, ObjectValue(*asyncGenerator));
  global->setReservedSlot(ASYNC_GENERATOR_FUNCTION,
                          ObjectValue(*asyncGenFunction));
  global->setReservedSlot(ASYNC_GENERATOR_PROTO, ObjectValue(*asyncGenProto));
  return true;
>>>>>>> upstream-releases
}
