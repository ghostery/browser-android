/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef vm_AsyncFunction_h
#define vm_AsyncFunction_h

#include "builtin/Promise.h"
#include "js/Class.h"
#include "vm/GeneratorObject.h"
#include "vm/JSContext.h"
#include "vm/JSObject.h"

namespace js {

<<<<<<< HEAD
// An async function is implemented using two function objects, which are
// referred to as the "unwrapped" and the "wrapped" async function object.
// The unwrapped function is a generator function compiled from the async
// function's script. |await| expressions within the async function are
// compiled like |yield| expression for the generator function with dedicated
// opcode,. The unwrapped function is never exposed to user script.
// The wrapped function is a native function which wraps the generator function,
// hence its name, and is the publicly exposed object of the async function.
//
// The unwrapped async function is created while compiling the async function,
// and the wrapped async function is created while executing the async function
// declaration or expression.

// Returns a wrapped async function from an unwrapped async function.
JSFunction* GetWrappedAsyncFunction(JSFunction* unwrapped);

// Returns an unwrapped async function from a wrapped async function.
JSFunction* GetUnwrappedAsyncFunction(JSFunction* wrapped);

// Returns true if the given function is a wrapped async function.
bool IsWrappedAsyncFunction(JSFunction* fun);

// Create a wrapped async function from unwrapped async function with given
// prototype object.
JSObject* WrapAsyncFunctionWithProto(JSContext* cx, HandleFunction unwrapped,
                                     HandleObject proto);

// Create a wrapped async function from unwrapped async function with default
// prototype object.
JSObject* WrapAsyncFunction(JSContext* cx, HandleFunction unwrapped);
||||||| merged common ancestors
// An async function is implemented using two function objects, which are
// referred to as the "unwrapped" and the "wrapped" async function object.
// The unwrapped function is a generator function compiled from the async
// function's script. |await| expressions within the async function are
// compiled like |yield| expression for the generator function with dedicated
// opcode,. The unwrapped function is never exposed to user script.
// The wrapped function is a native function which wraps the generator function,
// hence its name, and is the publicly exposed object of the async function.
//
// The unwrapped async function is created while compiling the async function,
// and the wrapped async function is created while executing the async function
// declaration or expression.

// Returns a wrapped async function from an unwrapped async function.
JSFunction*
GetWrappedAsyncFunction(JSFunction* unwrapped);

// Returns an unwrapped async function from a wrapped async function.
JSFunction*
GetUnwrappedAsyncFunction(JSFunction* wrapped);

// Returns true if the given function is a wrapped async function.
bool
IsWrappedAsyncFunction(JSFunction* fun);

// Create a wrapped async function from unwrapped async function with given
// prototype object.
JSObject*
WrapAsyncFunctionWithProto(JSContext* cx, HandleFunction unwrapped, HandleObject proto);

// Create a wrapped async function from unwrapped async function with default
// prototype object.
JSObject*
WrapAsyncFunction(JSContext* cx, HandleFunction unwrapped);
=======
class AsyncFunctionGeneratorObject;
>>>>>>> upstream-releases

// Resume the async function when the `await` operand resolves.
// Split into two functions depending on whether the awaited value was
// fulfilled or rejected.
<<<<<<< HEAD
MOZ_MUST_USE bool AsyncFunctionAwaitedFulfilled(
    JSContext* cx, Handle<PromiseObject*> resultPromise,
    HandleValue generatorVal, HandleValue value);
||||||| merged common ancestors
MOZ_MUST_USE bool
AsyncFunctionAwaitedFulfilled(JSContext* cx, Handle<PromiseObject*> resultPromise,
                              HandleValue generatorVal, HandleValue value);
=======
MOZ_MUST_USE bool AsyncFunctionAwaitedFulfilled(
    JSContext* cx, Handle<AsyncFunctionGeneratorObject*> generator,
    HandleValue value);
>>>>>>> upstream-releases

<<<<<<< HEAD
MOZ_MUST_USE bool AsyncFunctionAwaitedRejected(
    JSContext* cx, Handle<PromiseObject*> resultPromise,
    HandleValue generatorVal, HandleValue reason);
||||||| merged common ancestors
MOZ_MUST_USE bool
AsyncFunctionAwaitedRejected(JSContext* cx, Handle<PromiseObject*> resultPromise,
                             HandleValue generatorVal, HandleValue reason);
=======
MOZ_MUST_USE bool AsyncFunctionAwaitedRejected(
    JSContext* cx, Handle<AsyncFunctionGeneratorObject*> generator,
    HandleValue reason);
>>>>>>> upstream-releases

<<<<<<< HEAD
}  // namespace js
||||||| merged common ancestors
} // namespace js
=======
enum class AsyncFunctionResolveKind { Fulfill, Reject };

// Resolve the async function's promise object with the given value and then
// return the promise object.
JSObject* AsyncFunctionResolve(JSContext* cx,
                               Handle<AsyncFunctionGeneratorObject*> generator,
                               HandleValue valueOrReason,
                               AsyncFunctionResolveKind resolveKind);

class AsyncFunctionGeneratorObject : public AbstractGeneratorObject {
 public:
  enum {
    PROMISE_SLOT = AbstractGeneratorObject::RESERVED_SLOTS,

    RESERVED_SLOTS
  };

  static const Class class_;

  static AsyncFunctionGeneratorObject* create(JSContext* cx,
                                              HandleFunction asyncGen);

  PromiseObject* promise() {
    return &getFixedSlot(PROMISE_SLOT).toObject().as<PromiseObject>();
  }
};

}  // namespace js
>>>>>>> upstream-releases

#endif /* vm_AsyncFunction_h */
