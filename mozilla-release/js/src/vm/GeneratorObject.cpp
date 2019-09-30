/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "vm/GeneratorObject.h"

#include "js/PropertySpec.h"
#include "vm/AsyncFunction.h"
#include "vm/AsyncIteration.h"
#include "vm/JSObject.h"

#include "vm/ArrayObject-inl.h"
#include "vm/Debugger-inl.h"
#include "vm/JSAtom-inl.h"
#include "vm/JSScript-inl.h"
#include "vm/NativeObject-inl.h"
#include "vm/Stack-inl.h"

using namespace js;

<<<<<<< HEAD
JSObject* GeneratorObject::create(JSContext* cx, AbstractFramePtr frame) {
  MOZ_ASSERT(frame.isGeneratorFrame());
  MOZ_ASSERT(frame.script()->nfixed() == 0);
  MOZ_ASSERT(!frame.isConstructing());

  Rooted<GlobalObject*> global(cx, cx->global());

  RootedValue pval(cx);
  RootedObject fun(cx, frame.callee());
  // FIXME: This would be faster if we could avoid doing a lookup to get
  // the prototype for the instance.  Bug 906600.
  if (!GetProperty(cx, fun, fun, cx->names().prototype, &pval)) {
    return nullptr;
  }
  RootedObject proto(cx, pval.isObject() ? &pval.toObject() : nullptr);
  if (!proto) {
    proto = GlobalObject::getOrCreateGeneratorObjectPrototype(cx, global);
    if (!proto) {
      return nullptr;
    }
  }
  Rooted<GeneratorObject*> genObj(
      cx, NewObjectWithGivenProto<GeneratorObject>(cx, proto));
  if (!genObj) {
    return nullptr;
  }

  genObj->setCallee(*frame.callee());
  genObj->setEnvironmentChain(*frame.environmentChain());
  if (frame.script()->needsArgsObj()) {
    genObj->setArgsObj(frame.argsObj());
  }
  genObj->clearExpressionStack();

  if (!Debugger::onNewGenerator(cx, frame, genObj)) {
    return nullptr;
  }

  return genObj;
||||||| merged common ancestors
JSObject*
GeneratorObject::create(JSContext* cx, AbstractFramePtr frame)
{
    MOZ_ASSERT(frame.script()->isGenerator() || frame.script()->isAsync());
    MOZ_ASSERT(frame.script()->nfixed() == 0);
    MOZ_ASSERT(!frame.isConstructing());

    Rooted<GlobalObject*> global(cx, cx->global());

    RootedValue pval(cx);
    RootedObject fun(cx, frame.callee());
    // FIXME: This would be faster if we could avoid doing a lookup to get
    // the prototype for the instance.  Bug 906600.
    if (!GetProperty(cx, fun, fun, cx->names().prototype, &pval)) {
        return nullptr;
    }
    RootedObject proto(cx, pval.isObject() ? &pval.toObject() : nullptr);
    if (!proto) {
        proto = GlobalObject::getOrCreateGeneratorObjectPrototype(cx, global);
        if (!proto) {
            return nullptr;
        }
    }
    Rooted<GeneratorObject*> genObj(cx, NewObjectWithGivenProto<GeneratorObject>(cx, proto));
    if (!genObj) {
        return nullptr;
    }

    genObj->setCallee(*frame.callee());
    genObj->setEnvironmentChain(*frame.environmentChain());
    if (frame.script()->needsArgsObj()) {
        genObj->setArgsObj(frame.argsObj());
    }
    genObj->clearExpressionStack();

    return genObj;
=======
JSObject* AbstractGeneratorObject::create(JSContext* cx,
                                          AbstractFramePtr frame) {
  MOZ_ASSERT(frame.isGeneratorFrame());
  MOZ_ASSERT(frame.script()->nfixed() == 0);
  MOZ_ASSERT(!frame.isConstructing());

  RootedFunction fun(cx, frame.callee());

  Rooted<AbstractGeneratorObject*> genObj(cx);
  if (!fun->isAsync()) {
    genObj = GeneratorObject::create(cx, fun);
  } else if (fun->isGenerator()) {
    genObj = AsyncGeneratorObject::create(cx, fun);
  } else {
    genObj = AsyncFunctionGeneratorObject::create(cx, fun);
  }
  if (!genObj) {
    return nullptr;
  }

  genObj->setCallee(*frame.callee());
  genObj->setEnvironmentChain(*frame.environmentChain());
  if (frame.script()->needsArgsObj()) {
    genObj->setArgsObj(frame.argsObj());
  }
  genObj->clearExpressionStack();

  if (!Debugger::onNewGenerator(cx, frame, genObj)) {
    return nullptr;
  }

  return genObj;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool GeneratorObject::suspend(JSContext* cx, HandleObject obj,
                              AbstractFramePtr frame, jsbytecode* pc, Value* vp,
                              unsigned nvalues) {
  MOZ_ASSERT(*pc == JSOP_INITIALYIELD || *pc == JSOP_YIELD ||
             *pc == JSOP_AWAIT);

  Rooted<GeneratorObject*> genObj(cx, &obj->as<GeneratorObject>());
  MOZ_ASSERT(!genObj->hasExpressionStack() || genObj->isExpressionStackEmpty());
  MOZ_ASSERT_IF(*pc == JSOP_AWAIT, genObj->callee().isAsync());
  MOZ_ASSERT_IF(*pc == JSOP_YIELD, genObj->callee().isGenerator());

  ArrayObject* stack = nullptr;
  if (nvalues > 0) {
    do {
      if (genObj->hasExpressionStack()) {
        MOZ_ASSERT(genObj->expressionStack().getDenseInitializedLength() == 0);
        auto result = genObj->expressionStack().setOrExtendDenseElements(
            cx, 0, vp, nvalues, ShouldUpdateTypes::DontUpdate);
        if (result == DenseElementResult::Success) {
          MOZ_ASSERT(genObj->expressionStack().getDenseInitializedLength() ==
                     nvalues);
          break;
        }
        if (result == DenseElementResult::Failure) {
          return false;
        }
      }
||||||| merged common ancestors
bool
GeneratorObject::suspend(JSContext* cx, HandleObject obj, AbstractFramePtr frame, jsbytecode* pc,
                         Value* vp, unsigned nvalues)
{
    MOZ_ASSERT(*pc == JSOP_INITIALYIELD || *pc == JSOP_YIELD || *pc == JSOP_AWAIT);

    Rooted<GeneratorObject*> genObj(cx, &obj->as<GeneratorObject>());
    MOZ_ASSERT(!genObj->hasExpressionStack() || genObj->isExpressionStackEmpty());
    MOZ_ASSERT_IF(*pc == JSOP_AWAIT, genObj->callee().isAsync());
    MOZ_ASSERT_IF(*pc == JSOP_YIELD, genObj->callee().isGenerator());

    ArrayObject* stack = nullptr;
    if (nvalues > 0) {
        do {
            if (genObj->hasExpressionStack()) {
                MOZ_ASSERT(genObj->expressionStack().getDenseInitializedLength() == 0);
                auto result = genObj->expressionStack().setOrExtendDenseElements(
                    cx, 0, vp, nvalues, ShouldUpdateTypes::DontUpdate);
                if (result == DenseElementResult::Success) {
                    MOZ_ASSERT(genObj->expressionStack().getDenseInitializedLength() == nvalues);
                    break;
                }
                if (result == DenseElementResult::Failure) {
                    return false;
                }
            }

            stack = NewDenseCopiedArray(cx, nvalues, vp);
            if (!stack) {
                return false;
            }
        } while (false);
    }
=======
bool AbstractGeneratorObject::suspend(JSContext* cx, HandleObject obj,
                                      AbstractFramePtr frame, jsbytecode* pc,
                                      Value* vp, unsigned nvalues) {
  MOZ_ASSERT(*pc == JSOP_INITIALYIELD || *pc == JSOP_YIELD ||
             *pc == JSOP_AWAIT);

  auto genObj = obj.as<AbstractGeneratorObject>();
  MOZ_ASSERT(!genObj->hasExpressionStack() || genObj->isExpressionStackEmpty());
  MOZ_ASSERT_IF(*pc == JSOP_AWAIT, genObj->callee().isAsync());
  MOZ_ASSERT_IF(*pc == JSOP_YIELD, genObj->callee().isGenerator());

  ArrayObject* stack = nullptr;
  if (nvalues > 0) {
    do {
      if (genObj->hasExpressionStack()) {
        MOZ_ASSERT(genObj->expressionStack().getDenseInitializedLength() == 0);
        auto result = genObj->expressionStack().setOrExtendDenseElements(
            cx, 0, vp, nvalues, ShouldUpdateTypes::DontUpdate);
        if (result == DenseElementResult::Success) {
          MOZ_ASSERT(genObj->expressionStack().getDenseInitializedLength() ==
                     nvalues);
          break;
        }
        if (result == DenseElementResult::Failure) {
          return false;
        }
      }
>>>>>>> upstream-releases

      stack = NewDenseCopiedArray(cx, nvalues, vp);
      if (!stack) {
        return false;
      }
    } while (false);
  }

<<<<<<< HEAD
  genObj->setResumeIndex(pc);
  genObj->setEnvironmentChain(*frame.environmentChain());
  if (stack) {
    genObj->setExpressionStack(*stack);
  }
||||||| merged common ancestors
    return true;
}
=======
  genObj->setResumeIndex(pc);
  genObj->setEnvironmentChain(*frame.environmentChain());
  if (stack) {
    genObj->setExpressionStack(*stack);
  }

  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
void
GeneratorObject::finalSuspend(HandleObject obj)
{
    GeneratorObject* genObj = &obj->as<GeneratorObject>();
    MOZ_ASSERT(genObj->isRunning() || genObj->isClosing());
    genObj->setClosed();
=======
void AbstractGeneratorObject::finalSuspend(HandleObject obj) {
  auto* genObj = &obj->as<AbstractGeneratorObject>();
  MOZ_ASSERT(genObj->isRunning());
  genObj->setClosed();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void GeneratorObject::finalSuspend(HandleObject obj) {
  GeneratorObject* genObj = &obj->as<GeneratorObject>();
  MOZ_ASSERT(genObj->isRunning() || genObj->isClosing());
  genObj->setClosed();
||||||| merged common ancestors
GeneratorObject*
js::GetGeneratorObjectForFrame(JSContext* cx, AbstractFramePtr frame)
{
    cx->check(frame);
    MOZ_ASSERT(frame.isFunctionFrame() &&
               (frame.callee()->isGenerator() || frame.callee()->isAsync()));

    // The ".generator" binding is always present and always "aliased".
    CallObject& callObj = frame.callObj();
    Shape* shape = callObj.lookup(cx, cx->names().dotGenerator);
    Value genValue = callObj.getSlot(shape->slot());

    // If the `generator; setaliasedvar ".generator"; initialyield` bytecode
    // sequence has not run yet, genValue is undefined.
    return genValue.isObject()
           ? &genValue.toObject().as<GeneratorObject>()
           : nullptr;
=======
AbstractGeneratorObject* js::GetGeneratorObjectForFrame(
    JSContext* cx, AbstractFramePtr frame) {
  cx->check(frame);
  MOZ_ASSERT(frame.isGeneratorFrame());

  if (!frame.hasInitialEnvironment()) {
    return nullptr;
  }

  // The ".generator" binding is always present and always "aliased".
  CallObject& callObj = frame.callObj();
  Shape* shape = callObj.lookup(cx, cx->names().dotGenerator);
  Value genValue = callObj.getSlot(shape->slot());

  // If the `generator; setaliasedvar ".generator"; initialyield` bytecode
  // sequence has not run yet, genValue is undefined.
  return genValue.isObject()
             ? &genValue.toObject().as<AbstractGeneratorObject>()
             : nullptr;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
GeneratorObject* js::GetGeneratorObjectForFrame(JSContext* cx,
                                                AbstractFramePtr frame) {
  cx->check(frame);
  MOZ_ASSERT(frame.isGeneratorFrame());
||||||| merged common ancestors
void
js::SetGeneratorClosed(JSContext* cx, AbstractFramePtr frame)
{
    CallObject& callObj = frame.callObj();

    // Get the generator object stored on the scope chain and close it.
    Shape* shape = callObj.lookup(cx, cx->names().dotGenerator);
    GeneratorObject& genObj = callObj.getSlot(shape->slot()).toObject().as<GeneratorObject>();
    genObj.setClosed();
}
=======
void js::SetGeneratorClosed(JSContext* cx, AbstractFramePtr frame) {
  CallObject& callObj = frame.callObj();

  // Get the generator object stored on the scope chain and close it.
  Shape* shape = callObj.lookup(cx, cx->names().dotGenerator);
  auto& genObj =
      callObj.getSlot(shape->slot()).toObject().as<AbstractGeneratorObject>();
  genObj.setClosed();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!frame.hasInitialEnvironment()) {
    return nullptr;
  }
||||||| merged common ancestors
bool
js::GeneratorThrowOrReturn(JSContext* cx, AbstractFramePtr frame, Handle<GeneratorObject*> genObj,
                           HandleValue arg, uint32_t resumeKind)
{
    if (resumeKind == GeneratorObject::THROW) {
        cx->setPendingException(arg);
    } else {
        MOZ_ASSERT(resumeKind == GeneratorObject::RETURN);

        MOZ_ASSERT(arg.isObject());
        frame.setReturnValue(arg);

        RootedValue closing(cx, MagicValue(JS_GENERATOR_CLOSING));
        cx->setPendingException(closing);
        genObj->setClosing();
    }
    return false;
}
=======
bool js::GeneratorThrowOrReturn(JSContext* cx, AbstractFramePtr frame,
                                Handle<AbstractGeneratorObject*> genObj,
                                HandleValue arg,
                                GeneratorResumeKind resumeKind) {
  MOZ_ASSERT(genObj->isRunning());
  if (resumeKind == GeneratorResumeKind::Throw) {
    cx->setPendingExceptionAndCaptureStack(arg);
  } else {
    MOZ_ASSERT(resumeKind == GeneratorResumeKind::Return);

    MOZ_ASSERT_IF(genObj->is<GeneratorObject>(), arg.isObject());
    frame.setReturnValue(arg);

    RootedValue closing(cx, MagicValue(JS_GENERATOR_CLOSING));
    cx->setPendingException(closing, nullptr);
  }
  return false;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The ".generator" binding is always present and always "aliased".
  CallObject& callObj = frame.callObj();
  Shape* shape = callObj.lookup(cx, cx->names().dotGenerator);
  Value genValue = callObj.getSlot(shape->slot());
||||||| merged common ancestors
bool
GeneratorObject::resume(JSContext* cx, InterpreterActivation& activation,
                        Handle<GeneratorObject*> genObj, HandleValue arg)
{
    MOZ_ASSERT(genObj->isSuspended());
=======
bool AbstractGeneratorObject::resume(JSContext* cx,
                                     InterpreterActivation& activation,
                                     Handle<AbstractGeneratorObject*> genObj,
                                     HandleValue arg) {
  MOZ_ASSERT(genObj->isSuspended());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If the `generator; setaliasedvar ".generator"; initialyield` bytecode
  // sequence has not run yet, genValue is undefined.
  return genValue.isObject() ? &genValue.toObject().as<GeneratorObject>()
                             : nullptr;
}

void js::SetGeneratorClosed(JSContext* cx, AbstractFramePtr frame) {
  CallObject& callObj = frame.callObj();
||||||| merged common ancestors
    RootedFunction callee(cx, &genObj->callee());
    RootedObject envChain(cx, &genObj->environmentChain());
    if (!activation.resumeGeneratorFrame(callee, envChain)) {
        return false;
    }
    activation.regs().fp()->setResumedGenerator();

    if (genObj->hasArgsObj()) {
        activation.regs().fp()->initArgsObj(genObj->argsObj());
    }
=======
  RootedFunction callee(cx, &genObj->callee());
  RootedObject envChain(cx, &genObj->environmentChain());
  if (!activation.resumeGeneratorFrame(callee, envChain)) {
    return false;
  }
  activation.regs().fp()->setResumedGenerator();

  if (genObj->hasArgsObj()) {
    activation.regs().fp()->initArgsObj(genObj->argsObj());
  }

  if (genObj->hasExpressionStack() && !genObj->isExpressionStackEmpty()) {
    uint32_t len = genObj->expressionStack().getDenseInitializedLength();
    MOZ_ASSERT(activation.regs().spForStackDepth(len));
    const Value* src = genObj->expressionStack().getDenseElements();
    mozilla::PodCopy(activation.regs().sp, src, len);
    activation.regs().sp += len;
    genObj->expressionStack().setDenseInitializedLength(0);
  }

  JSScript* script = callee->nonLazyScript();
  uint32_t offset = script->resumeOffsets()[genObj->resumeIndex()];
  activation.regs().pc = script->offsetToPC(offset);

  // Always push on a value, even if we are raising an exception. In the
  // exception case, the stack needs to have something on it so that exception
  // handling doesn't skip the catch blocks. See TryNoteIter::settle.
  activation.regs().sp++;
  MOZ_ASSERT(activation.regs().spForStackDepth(activation.regs().stackDepth()));
  activation.regs().sp[-1] = arg;

  genObj->setRunning();
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Get the generator object stored on the scope chain and close it.
  Shape* shape = callObj.lookup(cx, cx->names().dotGenerator);
  GeneratorObject& genObj =
      callObj.getSlot(shape->slot()).toObject().as<GeneratorObject>();
  genObj.setClosed();
}

bool js::GeneratorThrowOrReturn(JSContext* cx, AbstractFramePtr frame,
                                Handle<GeneratorObject*> genObj,
                                HandleValue arg, uint32_t resumeKind) {
  if (resumeKind == GeneratorObject::THROW) {
    cx->setPendingException(arg);
  } else {
    MOZ_ASSERT(resumeKind == GeneratorObject::RETURN);

    MOZ_ASSERT(arg.isObject());
    frame.setReturnValue(arg);

    RootedValue closing(cx, MagicValue(JS_GENERATOR_CLOSING));
    cx->setPendingException(closing);
    genObj->setClosing();
  }
  return false;
}

bool GeneratorObject::resume(JSContext* cx, InterpreterActivation& activation,
                             Handle<GeneratorObject*> genObj, HandleValue arg) {
  MOZ_ASSERT(genObj->isSuspended());

  RootedFunction callee(cx, &genObj->callee());
  RootedObject envChain(cx, &genObj->environmentChain());
  if (!activation.resumeGeneratorFrame(callee, envChain)) {
    return false;
  }
  activation.regs().fp()->setResumedGenerator();

  if (genObj->hasArgsObj()) {
    activation.regs().fp()->initArgsObj(genObj->argsObj());
  }

  if (genObj->hasExpressionStack() && !genObj->isExpressionStackEmpty()) {
    uint32_t len = genObj->expressionStack().getDenseInitializedLength();
    MOZ_ASSERT(activation.regs().spForStackDepth(len));
    const Value* src = genObj->expressionStack().getDenseElements();
    mozilla::PodCopy(activation.regs().sp, src, len);
    activation.regs().sp += len;
    genObj->expressionStack().setDenseInitializedLength(0);
  }

  JSScript* script = callee->nonLazyScript();
  uint32_t offset = script->resumeOffsets()[genObj->resumeIndex()];
  activation.regs().pc = script->offsetToPC(offset);

  // Always push on a value, even if we are raising an exception. In the
  // exception case, the stack needs to have something on it so that exception
  // handling doesn't skip the catch blocks. See TryNoteIter::settle.
  activation.regs().sp++;
  MOZ_ASSERT(activation.regs().spForStackDepth(activation.regs().stackDepth()));
  activation.regs().sp[-1] = arg;

  genObj->setRunning();
  return true;
||||||| merged common ancestors
    if (genObj->hasExpressionStack() && !genObj->isExpressionStackEmpty()) {
        uint32_t len = genObj->expressionStack().getDenseInitializedLength();
        MOZ_ASSERT(activation.regs().spForStackDepth(len));
        const Value* src = genObj->expressionStack().getDenseElements();
        mozilla::PodCopy(activation.regs().sp, src, len);
        activation.regs().sp += len;
        genObj->expressionStack().setDenseInitializedLength(0);
    }

    JSScript* script = callee->nonLazyScript();
    uint32_t offset = script->yieldAndAwaitOffsets()[genObj->yieldAndAwaitIndex()];
    activation.regs().pc = script->offsetToPC(offset);

    // Always push on a value, even if we are raising an exception. In the
    // exception case, the stack needs to have something on it so that exception
    // handling doesn't skip the catch blocks. See TryNoteIter::settle.
    activation.regs().sp++;
    MOZ_ASSERT(activation.regs().spForStackDepth(activation.regs().stackDepth()));
    activation.regs().sp[-1] = arg;

    genObj->setRunning();
    return true;
=======
GeneratorObject* GeneratorObject::create(JSContext* cx, HandleFunction fun) {
  MOZ_ASSERT(fun->isGenerator() && !fun->isAsync());

  // FIXME: This would be faster if we could avoid doing a lookup to get
  // the prototype for the instance.  Bug 906600.
  RootedValue pval(cx);
  if (!GetProperty(cx, fun, fun, cx->names().prototype, &pval)) {
    return nullptr;
  }
  RootedObject proto(cx, pval.isObject() ? &pval.toObject() : nullptr);
  if (!proto) {
    proto = GlobalObject::getOrCreateGeneratorObjectPrototype(cx, cx->global());
    if (!proto) {
      return nullptr;
    }
  }
  return NewObjectWithGivenProto<GeneratorObject>(cx, proto);
>>>>>>> upstream-releases
}

const Class GeneratorObject::class_ = {
    "Generator", JSCLASS_HAS_RESERVED_SLOTS(GeneratorObject::RESERVED_SLOTS)};

static const JSFunctionSpec generator_methods[] = {
    JS_SELF_HOSTED_FN("next", "GeneratorNext", 1, 0),
    JS_SELF_HOSTED_FN("throw", "GeneratorThrow", 1, 0),
<<<<<<< HEAD
    JS_SELF_HOSTED_FN("return", "GeneratorReturn", 1, 0), JS_FS_END};

JSObject* js::NewSingletonObjectWithFunctionPrototype(
    JSContext* cx, Handle<GlobalObject*> global) {
  RootedObject proto(cx,
                     GlobalObject::getOrCreateFunctionPrototype(cx, global));
  if (!proto) {
    return nullptr;
  }
  return NewObjectWithGivenProto<PlainObject>(cx, proto, SingletonObject);
||||||| merged common ancestors
    JS_SELF_HOSTED_FN("return", "GeneratorReturn", 1, 0),
    JS_FS_END
};

JSObject*
js::NewSingletonObjectWithFunctionPrototype(JSContext* cx, Handle<GlobalObject*> global)
{
    RootedObject proto(cx, GlobalObject::getOrCreateFunctionPrototype(cx, global));
    if (!proto) {
        return nullptr;
    }
    return NewObjectWithGivenProto<PlainObject>(cx, proto, SingletonObject);
=======
    JS_SELF_HOSTED_FN("return", "GeneratorReturn", 1, 0), JS_FS_END};

JSObject* js::NewSingletonObjectWithFunctionPrototype(
    JSContext* cx, Handle<GlobalObject*> global) {
  RootedObject proto(cx,
                     GlobalObject::getOrCreateFunctionPrototype(cx, global));
  if (!proto) {
    return nullptr;
  }
  RootedObject obj(
      cx, NewObjectWithGivenProto<PlainObject>(cx, proto, SingletonObject));
  if (!obj) {
    return nullptr;
  }
  if (!JSObject::setDelegate(cx, obj)) {
    return nullptr;
  }
  return obj;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ bool GlobalObject::initGenerators(JSContext* cx,
                                               Handle<GlobalObject*> global) {
  if (global->getReservedSlot(GENERATOR_OBJECT_PROTO).isObject()) {
    return true;
  }
||||||| merged common ancestors
/* static */ bool
GlobalObject::initGenerators(JSContext* cx, Handle<GlobalObject*> global)
{
    if (global->getReservedSlot(GENERATOR_OBJECT_PROTO).isObject()) {
        return true;
    }
=======
/* static */
bool GlobalObject::initGenerators(JSContext* cx, Handle<GlobalObject*> global) {
  if (global->getReservedSlot(GENERATOR_OBJECT_PROTO).isObject()) {
    return true;
  }
>>>>>>> upstream-releases

  RootedObject iteratorProto(
      cx, GlobalObject::getOrCreateIteratorPrototype(cx, global));
  if (!iteratorProto) {
    return false;
  }

  RootedObject genObjectProto(cx, GlobalObject::createBlankPrototypeInheriting(
                                      cx, &PlainObject::class_, iteratorProto));
  if (!genObjectProto) {
    return false;
  }
  if (!DefinePropertiesAndFunctions(cx, genObjectProto, nullptr,
                                    generator_methods) ||
      !DefineToStringTag(cx, genObjectProto, cx->names().Generator)) {
    return false;
  }

<<<<<<< HEAD
  RootedObject genFunctionProto(
      cx, NewSingletonObjectWithFunctionPrototype(cx, global));
  if (!genFunctionProto || !JSObject::setDelegate(cx, genFunctionProto)) {
    return false;
  }
  if (!LinkConstructorAndPrototype(cx, genFunctionProto, genObjectProto,
                                   JSPROP_READONLY, JSPROP_READONLY) ||
      !DefineToStringTag(cx, genFunctionProto, cx->names().GeneratorFunction)) {
    return false;
  }
||||||| merged common ancestors
    RootedObject genFunctionProto(cx, NewSingletonObjectWithFunctionPrototype(cx, global));
    if (!genFunctionProto || !JSObject::setDelegate(cx, genFunctionProto)) {
        return false;
    }
    if (!LinkConstructorAndPrototype(cx, genFunctionProto, genObjectProto, JSPROP_READONLY,
                                     JSPROP_READONLY) ||
        !DefineToStringTag(cx, genFunctionProto, cx->names().GeneratorFunction))
    {
        return false;
    }
=======
  RootedObject genFunctionProto(
      cx, NewSingletonObjectWithFunctionPrototype(cx, global));
  if (!genFunctionProto) {
    return false;
  }
  if (!LinkConstructorAndPrototype(cx, genFunctionProto, genObjectProto,
                                   JSPROP_READONLY, JSPROP_READONLY) ||
      !DefineToStringTag(cx, genFunctionProto, cx->names().GeneratorFunction)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedValue function(cx, global->getConstructor(JSProto_Function));
  if (!function.toObjectOrNull()) {
    return false;
  }
  RootedObject proto(cx, &function.toObject());
  RootedAtom name(cx, cx->names().GeneratorFunction);
  RootedObject genFunction(
      cx, NewFunctionWithProto(cx, Generator, 1, JSFunction::NATIVE_CTOR,
                               nullptr, name, proto, gc::AllocKind::FUNCTION,
                               SingletonObject));
  if (!genFunction) {
    return false;
  }
  if (!LinkConstructorAndPrototype(cx, genFunction, genFunctionProto,
                                   JSPROP_PERMANENT | JSPROP_READONLY,
                                   JSPROP_READONLY)) {
    return false;
  }
||||||| merged common ancestors
    RootedValue function(cx, global->getConstructor(JSProto_Function));
    if (!function.toObjectOrNull()) {
        return false;
    }
    RootedObject proto(cx, &function.toObject());
    RootedAtom name(cx, cx->names().GeneratorFunction);
    RootedObject genFunction(cx, NewFunctionWithProto(cx, Generator, 1,
                                                      JSFunction::NATIVE_CTOR, nullptr, name,
                                                      proto, gc::AllocKind::FUNCTION,
                                                      SingletonObject));
    if (!genFunction) {
        return false;
    }
    if (!LinkConstructorAndPrototype(cx, genFunction, genFunctionProto,
                                     JSPROP_PERMANENT | JSPROP_READONLY, JSPROP_READONLY))
    {
        return false;
    }
=======
  RootedObject proto(
      cx, GlobalObject::getOrCreateFunctionConstructor(cx, cx->global()));
  if (!proto) {
    return false;
  }
  HandlePropertyName name = cx->names().GeneratorFunction;
  RootedObject genFunction(
      cx, NewFunctionWithProto(cx, Generator, 1, JSFunction::NATIVE_CTOR,
                               nullptr, name, proto, gc::AllocKind::FUNCTION,
                               SingletonObject));
  if (!genFunction) {
    return false;
  }
  if (!LinkConstructorAndPrototype(cx, genFunction, genFunctionProto,
                                   JSPROP_PERMANENT | JSPROP_READONLY,
                                   JSPROP_READONLY)) {
    return false;
  }
>>>>>>> upstream-releases

  global->setReservedSlot(GENERATOR_OBJECT_PROTO, ObjectValue(*genObjectProto));
  global->setReservedSlot(GENERATOR_FUNCTION, ObjectValue(*genFunction));
  global->setReservedSlot(GENERATOR_FUNCTION_PROTO,
                          ObjectValue(*genFunctionProto));
  return true;
}

<<<<<<< HEAD
bool GeneratorObject::isAfterYield() { return isAfterYieldOrAwait(JSOP_YIELD); }
||||||| merged common ancestors
bool
GeneratorObject::isAfterYield()
{
    return isAfterYieldOrAwait(JSOP_YIELD);
}
=======
bool AbstractGeneratorObject::isAfterYield() {
  return isAfterYieldOrAwait(JSOP_YIELD);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool GeneratorObject::isAfterAwait() { return isAfterYieldOrAwait(JSOP_AWAIT); }
||||||| merged common ancestors
bool
GeneratorObject::isAfterAwait()
{
    return isAfterYieldOrAwait(JSOP_AWAIT);
}
=======
bool AbstractGeneratorObject::isAfterAwait() {
  return isAfterYieldOrAwait(JSOP_AWAIT);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool GeneratorObject::isAfterYieldOrAwait(JSOp op) {
  if (isClosed() || isClosing() || isRunning()) {
    return false;
  }
||||||| merged common ancestors
bool
GeneratorObject::isAfterYieldOrAwait(JSOp op)
{
    if (isClosed() || isClosing() || isRunning()) {
        return false;
    }
=======
bool AbstractGeneratorObject::isAfterYieldOrAwait(JSOp op) {
  if (isClosed() || isRunning()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSScript* script = callee().nonLazyScript();
  jsbytecode* code = script->code();
  uint32_t nextOffset = script->resumeOffsets()[resumeIndex()];
  if (code[nextOffset] != JSOP_DEBUGAFTERYIELD) {
    return false;
  }
||||||| merged common ancestors
    JSScript* script = callee().nonLazyScript();
    jsbytecode* code = script->code();
    uint32_t nextOffset = script->yieldAndAwaitOffsets()[yieldAndAwaitIndex()];
    if (code[nextOffset] != JSOP_DEBUGAFTERYIELD) {
        return false;
    }
=======
  JSScript* script = callee().nonLazyScript();
  jsbytecode* code = script->code();
  uint32_t nextOffset = script->resumeOffsets()[resumeIndex()];
  if (code[nextOffset] != JSOP_AFTERYIELD) {
    return false;
  }

  static_assert(JSOP_YIELD_LENGTH == JSOP_INITIALYIELD_LENGTH,
                "JSOP_YIELD and JSOP_INITIALYIELD must have the same length");
  static_assert(JSOP_YIELD_LENGTH == JSOP_AWAIT_LENGTH,
                "JSOP_YIELD and JSOP_AWAIT must have the same length");
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t offset = nextOffset - JSOP_YIELD_LENGTH;
  MOZ_ASSERT(code[offset] == JSOP_INITIALYIELD || code[offset] == JSOP_YIELD ||
             code[offset] == JSOP_AWAIT);
||||||| merged common ancestors
    uint32_t offset = nextOffset - JSOP_YIELD_LENGTH;
    MOZ_ASSERT(code[offset] == JSOP_INITIALYIELD || code[offset] == JSOP_YIELD ||
               code[offset] == JSOP_AWAIT);
=======
  uint32_t offset = nextOffset - JSOP_YIELD_LENGTH;
  MOZ_ASSERT(code[offset] == JSOP_INITIALYIELD || code[offset] == JSOP_YIELD ||
             code[offset] == JSOP_AWAIT);

  return code[offset] == op;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return code[offset] == op;
||||||| merged common ancestors
    return code[offset] == op;
=======
template <>
bool JSObject::is<js::AbstractGeneratorObject>() const {
  return is<GeneratorObject>() || is<AsyncFunctionGeneratorObject>() ||
         is<AsyncGeneratorObject>();
>>>>>>> upstream-releases
}
