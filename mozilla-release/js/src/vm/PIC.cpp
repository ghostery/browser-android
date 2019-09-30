/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "vm/PIC.h"

#include "gc/FreeOp.h"
#include "gc/Marking.h"
#include "vm/GlobalObject.h"
#include "vm/JSContext.h"
#include "vm/JSObject.h"
#include "vm/Realm.h"
#include "vm/SelfHosting.h"

#include "vm/JSObject-inl.h"
#include "vm/NativeObject-inl.h"

using namespace js;

<<<<<<< HEAD
bool js::ForOfPIC::Chain::initialize(JSContext* cx) {
  MOZ_ASSERT(!initialized_);
||||||| merged common ancestors
bool
js::ForOfPIC::Chain::initialize(JSContext* cx)
{
    MOZ_ASSERT(!initialized_);
=======
template <typename Category>
void PICChain<Category>::addStub(JSObject* obj, CatStub* stub) {
  MOZ_ASSERT(stub);
  MOZ_ASSERT(!stub->next());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Get the canonical Array.prototype
  RootedNativeObject arrayProto(
      cx, GlobalObject::getOrCreateArrayPrototype(cx, cx->global()));
  if (!arrayProto) {
    return false;
  }
||||||| merged common ancestors
    // Get the canonical Array.prototype
    RootedNativeObject arrayProto(cx, GlobalObject::getOrCreateArrayPrototype(cx, cx->global()));
    if (!arrayProto) {
        return false;
    }

    // Get the canonical ArrayIterator.prototype
    RootedNativeObject arrayIteratorProto(cx,
        GlobalObject::getOrCreateArrayIteratorPrototype(cx, cx->global()));
    if (!arrayIteratorProto) {
        return false;
    }

    // From this point on, we can't fail.  Set initialized and fill the fields
    // for the canonical Array.prototype and ArrayIterator.prototype objects.
    initialized_ = true;
    arrayProto_ = arrayProto;
    arrayIteratorProto_ = arrayIteratorProto;

    // Shortcut returns below means Array for-of will never be optimizable,
    // do set disabled_ now, and clear it later when we succeed.
    disabled_ = true;

    // Look up Array.prototype[@@iterator], ensure it's a slotful shape.
    Shape* iterShape = arrayProto->lookup(cx, SYMBOL_TO_JSID(cx->wellKnownSymbols().iterator));
    if (!iterShape || !iterShape->isDataProperty()) {
        return true;
    }
=======
  AddCellMemory(obj, sizeof(CatStub), MemoryUse::ForOfPICStub);

  if (!stubs_) {
    stubs_ = stub;
    return;
  }

  CatStub* cur = stubs_;
  while (cur->next()) {
    cur = cur->next();
  }
  cur->append(stub);
}

bool js::ForOfPIC::Chain::initialize(JSContext* cx) {
  MOZ_ASSERT(!initialized_);

  // Get the canonical Array.prototype
  RootedNativeObject arrayProto(
      cx, GlobalObject::getOrCreateArrayPrototype(cx, cx->global()));
  if (!arrayProto) {
    return false;
  }
>>>>>>> upstream-releases

  // Get the canonical ArrayIterator.prototype
  RootedNativeObject arrayIteratorProto(
      cx, GlobalObject::getOrCreateArrayIteratorPrototype(cx, cx->global()));
  if (!arrayIteratorProto) {
    return false;
  }

  // From this point on, we can't fail.  Set initialized and fill the fields
  // for the canonical Array.prototype and ArrayIterator.prototype objects.
  initialized_ = true;
  arrayProto_ = arrayProto;
  arrayIteratorProto_ = arrayIteratorProto;

  // Shortcut returns below means Array for-of will never be optimizable,
  // do set disabled_ now, and clear it later when we succeed.
  disabled_ = true;

  // Look up Array.prototype[@@iterator], ensure it's a slotful shape.
  Shape* iterShape =
      arrayProto->lookup(cx, SYMBOL_TO_JSID(cx->wellKnownSymbols().iterator));
  if (!iterShape || !iterShape->isDataProperty()) {
    return true;
  }

  // Get the referred value, and ensure it holds the canonical ArrayValues
  // function.
  Value iterator = arrayProto->getSlot(iterShape->slot());
  JSFunction* iterFun;
  if (!IsFunctionObject(iterator, &iterFun)) {
    return true;
  }
  if (!IsSelfHostedFunctionWithName(iterFun, cx->names().ArrayValues)) {
    return true;
  }

  // Look up the 'next' value on ArrayIterator.prototype
  Shape* nextShape = arrayIteratorProto->lookup(cx, cx->names().next);
  if (!nextShape || !nextShape->isDataProperty()) {
    return true;
  }

<<<<<<< HEAD
  // Get the referred value, ensure it holds the canonical ArrayIteratorNext
  // function.
  Value next = arrayIteratorProto->getSlot(nextShape->slot());
  JSFunction* nextFun;
  if (!IsFunctionObject(next, &nextFun)) {
||||||| merged common ancestors
    disabled_ = false;
    arrayProtoShape_ = arrayProto->lastProperty();
    arrayProtoIteratorSlot_ = iterShape->slot();
    canonicalIteratorFunc_ = iterator;
    arrayIteratorProtoShape_ = arrayIteratorProto->lastProperty();
    arrayIteratorProtoNextSlot_ = nextShape->slot();
    canonicalNextFunc_ = next;
=======
  // Get the referred value, ensure it holds the canonical ArrayIteratorNext
  // function.
  Value next = arrayIteratorProto->getSlot(nextShape->slot());
  JSFunction* nextFun;
  if (!IsFunctionObject(next, &nextFun)) {
    return true;
  }
  if (!IsSelfHostedFunctionWithName(nextFun, cx->names().ArrayIteratorNext)) {
>>>>>>> upstream-releases
    return true;
<<<<<<< HEAD
  }
  if (!IsSelfHostedFunctionWithName(nextFun, cx->names().ArrayIteratorNext)) {
    return true;
  }

  disabled_ = false;
  arrayProtoShape_ = arrayProto->lastProperty();
  arrayProtoIteratorSlot_ = iterShape->slot();
  canonicalIteratorFunc_ = iterator;
  arrayIteratorProtoShape_ = arrayIteratorProto->lastProperty();
  arrayIteratorProtoNextSlot_ = nextShape->slot();
  canonicalNextFunc_ = next;
  return true;
||||||| merged common ancestors
=======
  }

  disabled_ = false;
  arrayProtoShape_ = arrayProto->lastProperty();
  arrayProtoIteratorSlot_ = iterShape->slot();
  canonicalIteratorFunc_ = iterator;
  arrayIteratorProtoShape_ = arrayIteratorProto->lastProperty();
  arrayIteratorProtoNextSlot_ = nextShape->slot();
  canonicalNextFunc_ = next;
  return true;
>>>>>>> upstream-releases
}

bool js::ForOfPIC::Chain::tryOptimizeArray(JSContext* cx,
                                           HandleArrayObject array,
                                           bool* optimized) {
  MOZ_ASSERT(optimized);

  *optimized = false;

  if (!initialized_) {
    // If PIC is not initialized, initialize it.
    if (!initialize(cx)) {
      return false;
    }

<<<<<<< HEAD
  } else if (!disabled_ && !isArrayStateStillSane()) {
    // Otherwise, if array state is no longer sane, reinitialize.
    reset();
||||||| merged common ancestors
    } else if (!disabled_ && !isArrayStateStillSane()) {
        // Otherwise, if array state is no longer sane, reinitialize.
        reset();
=======
  } else if (!disabled_ && !isArrayStateStillSane()) {
    // Otherwise, if array state is no longer sane, reinitialize.
    reset(cx);
>>>>>>> upstream-releases

    if (!initialize(cx)) {
      return false;
    }
  }
  MOZ_ASSERT(initialized_);

  // If PIC is disabled, don't bother trying to optimize.
  if (disabled_) {
    return true;
  }

  // By the time we get here, we should have a sane array state to work with.
  MOZ_ASSERT(isArrayStateStillSane());

  // Ensure array's prototype is the actual Array.prototype
  if (array->staticPrototype() != arrayProto_) {
    return true;
  }

  // Check if stub already exists.
  if (hasMatchingStub(array)) {
    *optimized = true;
    return true;
  }

<<<<<<< HEAD
  // Ensure array doesn't define @@iterator directly.
  if (array->lookup(cx, SYMBOL_TO_JSID(cx->wellKnownSymbols().iterator))) {
    return true;
  }
||||||| merged common ancestors
    // Ensure array doesn't define @@iterator directly.
    if (array->lookup(cx, SYMBOL_TO_JSID(cx->wellKnownSymbols().iterator))) {
        return true;
    }
=======
  // Ensure array doesn't define @@iterator directly.
  if (array->lookup(cx, SYMBOL_TO_JSID(cx->wellKnownSymbols().iterator))) {
    return true;
  }

  // If the number of stubs is about to exceed the limit, throw away entire
  // existing cache before adding new stubs.  We shouldn't really have heavy
  // churn on these.
  if (numStubs() >= MAX_STUBS) {
    eraseChain(cx);
  }

  // Good to optimize now, create stub to add.
  RootedShape shape(cx, array->lastProperty());
  Stub* stub = cx->new_<Stub>(shape);
  if (!stub) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If the number of stubs is about to exceed the limit, throw away entire
  // existing cache before adding new stubs.  We shouldn't really have heavy
  // churn on these.
  if (numStubs() >= MAX_STUBS) {
    eraseChain();
  }
||||||| merged common ancestors
    // If the number of stubs is about to exceed the limit, throw away entire
    // existing cache before adding new stubs.  We shouldn't really have heavy
    // churn on these.
    if (numStubs() >= MAX_STUBS) {
        eraseChain();
    }
=======
  // Add the stub.
  addStub(picObject_, stub);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Good to optimize now, create stub to add.
  RootedShape shape(cx, array->lastProperty());
  Stub* stub = cx->new_<Stub>(shape);
  if (!stub) {
    return false;
  }
||||||| merged common ancestors
    // Good to optimize now, create stub to add.
    RootedShape shape(cx, array->lastProperty());
    Stub* stub = cx->new_<Stub>(shape);
    if (!stub) {
        return false;
    }
=======
  *optimized = true;
  return true;
}

bool js::ForOfPIC::Chain::tryOptimizeArrayIteratorNext(JSContext* cx,
                                                       bool* optimized) {
  MOZ_ASSERT(optimized);

  *optimized = false;

  if (!initialized_) {
    // If PIC is not initialized, initialize it.
    if (!initialize(cx)) {
      return false;
    }
  } else if (!disabled_ && !isArrayNextStillSane()) {
    // Otherwise, if array iterator state is no longer sane, reinitialize.
    reset(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Add the stub.
  addStub(stub);
||||||| merged common ancestors
    // Add the stub.
    addStub(stub);
=======
    if (!initialize(cx)) {
      return false;
    }
  }
  MOZ_ASSERT(initialized_);
>>>>>>> upstream-releases

<<<<<<< HEAD
  *optimized = true;
  return true;
||||||| merged common ancestors
    *optimized = true;
    return true;
=======
  // If PIC is disabled, don't bother trying to optimize.
  if (disabled_) {
    return true;
  }

  // By the time we get here, we should have a sane iterator state to work with.
  MOZ_ASSERT(isArrayNextStillSane());

  *optimized = true;
  return true;
>>>>>>> upstream-releases
}

bool js::ForOfPIC::Chain::hasMatchingStub(ArrayObject* obj) {
  // Ensure PIC is initialized and not disabled.
  MOZ_ASSERT(initialized_ && !disabled_);

  // Check if there is a matching stub.
  for (Stub* stub = stubs(); stub != nullptr; stub = stub->next()) {
    if (stub->shape() == obj->shape()) {
      return true;
    }
  }

  return false;
}

bool js::ForOfPIC::Chain::isArrayStateStillSane() {
  // Ensure that canonical Array.prototype has matching shape.
  if (arrayProto_->lastProperty() != arrayProtoShape_) {
    return false;
  }

  // Ensure that Array.prototype[@@iterator] contains the
  // canonical iterator function.
  if (arrayProto_->getSlot(arrayProtoIteratorSlot_) != canonicalIteratorFunc_) {
    return false;
  }

  // Chain to isArrayNextStillSane.
  return isArrayNextStillSane();
}

<<<<<<< HEAD
void js::ForOfPIC::Chain::reset() {
  // Should never reset a disabled_ stub.
  MOZ_ASSERT(!disabled_);
||||||| merged common ancestors
void
js::ForOfPIC::Chain::reset()
{
    // Should never reset a disabled_ stub.
    MOZ_ASSERT(!disabled_);
=======
void js::ForOfPIC::Chain::reset(JSContext* cx) {
  // Should never reset a disabled_ stub.
  MOZ_ASSERT(!disabled_);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Erase the chain.
  eraseChain();
||||||| merged common ancestors
    // Erase the chain.
    eraseChain();
=======
  // Erase the chain.
  eraseChain(cx);
>>>>>>> upstream-releases

  arrayProto_ = nullptr;
  arrayIteratorProto_ = nullptr;

  arrayProtoShape_ = nullptr;
  arrayProtoIteratorSlot_ = -1;
  canonicalIteratorFunc_ = UndefinedValue();

  arrayIteratorProtoShape_ = nullptr;
  arrayIteratorProtoNextSlot_ = -1;
  canonicalNextFunc_ = UndefinedValue();

  initialized_ = false;
}

<<<<<<< HEAD
void js::ForOfPIC::Chain::eraseChain() {
  // Should never need to clear the chain of a disabled stub.
  MOZ_ASSERT(!disabled_);

  // Free all stubs.
  Stub* stub = stubs_;
  while (stub) {
    Stub* next = stub->next();
    js_delete(stub);
    stub = next;
  }
  stubs_ = nullptr;
||||||| merged common ancestors
void
js::ForOfPIC::Chain::eraseChain()
{
    // Should never need to clear the chain of a disabled stub.
    MOZ_ASSERT(!disabled_);

    // Free all stubs.
    Stub* stub = stubs_;
    while (stub) {
        Stub* next = stub->next();
        js_delete(stub);
        stub = next;
    }
    stubs_ = nullptr;
=======
void js::ForOfPIC::Chain::eraseChain(JSContext* cx) {
  // Should never need to clear the chain of a disabled stub.
  MOZ_ASSERT(!disabled_);
  freeAllStubs(cx->defaultFreeOp());
>>>>>>> upstream-releases
}

// Trace the pointers stored directly on the stub.
<<<<<<< HEAD
void js::ForOfPIC::Chain::trace(JSTracer* trc) {
  if (!initialized_ || disabled_) {
    return;
  }

  TraceEdge(trc, &arrayProto_, "ForOfPIC Array.prototype.");
  TraceEdge(trc, &arrayIteratorProto_, "ForOfPIC ArrayIterator.prototype.");

  TraceEdge(trc, &arrayProtoShape_, "ForOfPIC Array.prototype shape.");
  TraceEdge(trc, &arrayIteratorProtoShape_,
            "ForOfPIC ArrayIterator.prototype shape.");

  TraceEdge(trc, &canonicalIteratorFunc_, "ForOfPIC ArrayValues builtin.");
  TraceEdge(trc, &canonicalNextFunc_,
            "ForOfPIC ArrayIterator.prototype.next builtin.");

  // Free all the stubs in the chain.
  while (stubs_) {
    removeStub(stubs_, nullptr);
  }
||||||| merged common ancestors
void
js::ForOfPIC::Chain::trace(JSTracer* trc)
{
    if (!initialized_ || disabled_) {
        return;
    }

    TraceEdge(trc, &arrayProto_, "ForOfPIC Array.prototype.");
    TraceEdge(trc, &arrayIteratorProto_, "ForOfPIC ArrayIterator.prototype.");

    TraceEdge(trc, &arrayProtoShape_, "ForOfPIC Array.prototype shape.");
    TraceEdge(trc, &arrayIteratorProtoShape_, "ForOfPIC ArrayIterator.prototype shape.");

    TraceEdge(trc, &canonicalIteratorFunc_, "ForOfPIC ArrayValues builtin.");
    TraceEdge(trc, &canonicalNextFunc_, "ForOfPIC ArrayIterator.prototype.next builtin.");

    // Free all the stubs in the chain.
    while (stubs_) {
        removeStub(stubs_, nullptr);
    }
=======
void js::ForOfPIC::Chain::trace(JSTracer* trc) {
  TraceEdge(trc, &picObject_, "ForOfPIC object");

  if (!initialized_ || disabled_) {
    return;
  }

  TraceEdge(trc, &arrayProto_, "ForOfPIC Array.prototype.");
  TraceEdge(trc, &arrayIteratorProto_, "ForOfPIC ArrayIterator.prototype.");

  TraceEdge(trc, &arrayProtoShape_, "ForOfPIC Array.prototype shape.");
  TraceEdge(trc, &arrayIteratorProtoShape_,
            "ForOfPIC ArrayIterator.prototype shape.");

  TraceEdge(trc, &canonicalIteratorFunc_, "ForOfPIC ArrayValues builtin.");
  TraceEdge(trc, &canonicalNextFunc_,
            "ForOfPIC ArrayIterator.prototype.next builtin.");

  if (trc->isMarkingTracer()) {
    // Free all the stubs in the chain.
    freeAllStubs(trc->runtime()->defaultFreeOp());
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void js::ForOfPIC::Chain::sweep(FreeOp* fop) {
  // Free all the stubs in the chain.
  while (stubs_) {
    Stub* next = stubs_->next();
    fop->delete_(stubs_);
    stubs_ = next;
  }
  fop->delete_(this);
||||||| merged common ancestors
void
js::ForOfPIC::Chain::sweep(FreeOp* fop)
{
    // Free all the stubs in the chain.
    while (stubs_) {
        Stub* next = stubs_->next();
        fop->delete_(stubs_);
        stubs_ = next;
    }
    fop->delete_(this);
=======
static void ForOfPIC_finalize(FreeOp* fop, JSObject* obj) {
  MOZ_ASSERT(fop->maybeOnHelperThread());
  if (ForOfPIC::Chain* chain =
          ForOfPIC::fromJSObject(&obj->as<NativeObject>())) {
    chain->finalize(fop, obj);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void ForOfPIC_finalize(FreeOp* fop, JSObject* obj) {
  MOZ_ASSERT(fop->maybeOnHelperThread());
  if (ForOfPIC::Chain* chain =
          ForOfPIC::fromJSObject(&obj->as<NativeObject>())) {
    chain->sweep(fop);
  }
||||||| merged common ancestors
static void
ForOfPIC_finalize(FreeOp* fop, JSObject* obj)
{
    MOZ_ASSERT(fop->maybeOnHelperThread());
    if (ForOfPIC::Chain* chain = ForOfPIC::fromJSObject(&obj->as<NativeObject>())) {
        chain->sweep(fop);
    }
=======
void js::ForOfPIC::Chain::finalize(FreeOp* fop, JSObject* obj) {
  freeAllStubs(fop);
  fop->delete_(obj, this, MemoryUse::ForOfPIC);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void ForOfPIC_traceObject(JSTracer* trc, JSObject* obj) {
  if (ForOfPIC::Chain* chain =
          ForOfPIC::fromJSObject(&obj->as<NativeObject>())) {
    chain->trace(trc);
  }
||||||| merged common ancestors
static void
ForOfPIC_traceObject(JSTracer* trc, JSObject* obj)
{
    if (ForOfPIC::Chain* chain = ForOfPIC::fromJSObject(&obj->as<NativeObject>())) {
        chain->trace(trc);
    }
=======
void js::ForOfPIC::Chain::freeAllStubs(FreeOp* fop) {
  Stub* stub = stubs_;
  while (stub) {
    Stub* next = stub->next();
    fop->delete_(picObject_, stub, MemoryUse::ForOfPICStub);
    stub = next;
  }
  stubs_ = nullptr;
}

static void ForOfPIC_traceObject(JSTracer* trc, JSObject* obj) {
  if (ForOfPIC::Chain* chain =
          ForOfPIC::fromJSObject(&obj->as<NativeObject>())) {
    chain->trace(trc);
  }
>>>>>>> upstream-releases
}

static const ClassOps ForOfPICClassOps = {nullptr,
                                          nullptr,
                                          nullptr,
                                          nullptr,
                                          nullptr,
                                          nullptr,
                                          ForOfPIC_finalize,
                                          nullptr, /* call        */
                                          nullptr, /* hasInstance */
                                          nullptr, /* construct   */
                                          ForOfPIC_traceObject};

const Class ForOfPIC::class_ = {
<<<<<<< HEAD
    "ForOfPIC", JSCLASS_HAS_PRIVATE | JSCLASS_BACKGROUND_FINALIZE,
    &ForOfPICClassOps};

/* static */ NativeObject* js::ForOfPIC::createForOfPICObject(
    JSContext* cx, Handle<GlobalObject*> global) {
  cx->check(global);
  NativeObject* obj =
      NewNativeObjectWithGivenProto(cx, &ForOfPIC::class_, nullptr);
  if (!obj) {
    return nullptr;
  }
  ForOfPIC::Chain* chain = cx->new_<ForOfPIC::Chain>();
  if (!chain) {
    return nullptr;
  }
  obj->setPrivate(chain);
  return obj;
||||||| merged common ancestors
    "ForOfPIC",
    JSCLASS_HAS_PRIVATE |
    JSCLASS_BACKGROUND_FINALIZE,
    &ForOfPICClassOps
};

/* static */ NativeObject*
js::ForOfPIC::createForOfPICObject(JSContext* cx, Handle<GlobalObject*> global)
{
    cx->check(global);
    NativeObject* obj = NewNativeObjectWithGivenProto(cx, &ForOfPIC::class_, nullptr);
    if (!obj) {
        return nullptr;
    }
    ForOfPIC::Chain* chain = cx->new_<ForOfPIC::Chain>();
    if (!chain) {
        return nullptr;
    }
    obj->setPrivate(chain);
    return obj;
=======
    "ForOfPIC", JSCLASS_HAS_PRIVATE | JSCLASS_BACKGROUND_FINALIZE,
    &ForOfPICClassOps};

/* static */
NativeObject* js::ForOfPIC::createForOfPICObject(JSContext* cx,
                                                 Handle<GlobalObject*> global) {
  cx->check(global);
  NativeObject* obj = NewNativeObjectWithGivenProto(cx, &ForOfPIC::class_,
                                                    nullptr, TenuredObject);
  if (!obj) {
    return nullptr;
  }
  ForOfPIC::Chain* chain = cx->new_<ForOfPIC::Chain>(obj);
  if (!chain) {
    return nullptr;
  }
  InitObjectPrivate(obj, chain, MemoryUse::ForOfPIC);
  obj->setPrivate(chain);
  return obj;
>>>>>>> upstream-releases
}

/* static */ js::ForOfPIC::Chain* js::ForOfPIC::create(JSContext* cx) {
  MOZ_ASSERT(!cx->global()->getForOfPICObject());
  Rooted<GlobalObject*> global(cx, cx->global());
  NativeObject* obj = GlobalObject::getOrCreateForOfPICObject(cx, global);
  if (!obj) {
    return nullptr;
  }
  return fromJSObject(obj);
}
