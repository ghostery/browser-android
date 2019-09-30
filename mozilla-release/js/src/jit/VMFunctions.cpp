/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "jit/VMFunctions.h"

#include "builtin/Promise.h"
#include "builtin/TypedObject.h"
#include "frontend/BytecodeCompiler.h"
#include "jit/arm/Simulator-arm.h"
#include "jit/BaselineIC.h"
#include "jit/JitFrames.h"
#include "jit/JitRealm.h"
#include "jit/mips32/Simulator-mips32.h"
#include "jit/mips64/Simulator-mips64.h"
#include "vm/ArrayObject.h"
#include "vm/Debugger.h"
#include "vm/EqualityOperations.h"  // js::StrictlyEqual
#include "vm/Interpreter.h"
#include "vm/SelfHosting.h"
#include "vm/TraceLogging.h"

#include "jit/BaselineFrame-inl.h"
#include "jit/JitFrames-inl.h"
#include "jit/VMFunctionList-inl.h"
#include "vm/Debugger-inl.h"
#include "vm/Interpreter-inl.h"
#include "vm/JSScript-inl.h"
#include "vm/NativeObject-inl.h"
#include "vm/StringObject-inl.h"
#include "vm/TypeInference-inl.h"

using namespace js;
using namespace js::jit;

namespace js {
namespace jit {

<<<<<<< HEAD
// Statics are initialized to null.
/* static */ VMFunction* VMFunction::functions;

AutoDetectInvalidation::AutoDetectInvalidation(JSContext* cx,
                                               MutableHandleValue rval)
    : cx_(cx),
      ionScript_(GetTopJitJSScript(cx)->ionScript()),
      rval_(rval),
      disabled_(false) {}

bool InvokeFunction(JSContext* cx, HandleObject obj, bool constructing,
                    bool ignoresReturnValue, uint32_t argc, Value* argv,
                    MutableHandleValue rval) {
  TraceLoggerThread* logger = TraceLoggerForCurrentThread(cx);
  TraceLogStartEvent(logger, TraceLogger_Call);

  AutoArrayRooter argvRoot(cx, argc + 1 + constructing, argv);

  // Data in the argument vector is arranged for a JIT -> JIT call.
  RootedValue thisv(cx, argv[0]);
  Value* argvWithoutThis = argv + 1;

  RootedValue fval(cx, ObjectValue(*obj));
  if (constructing) {
    if (!IsConstructor(fval)) {
      ReportValueError(cx, JSMSG_NOT_CONSTRUCTOR, JSDVG_IGNORE_STACK, fval,
                       nullptr);
      return false;
    }
||||||| merged common ancestors
// Statics are initialized to null.
/* static */ VMFunction* VMFunction::functions;

AutoDetectInvalidation::AutoDetectInvalidation(JSContext* cx, MutableHandleValue rval)
  : cx_(cx),
    ionScript_(GetTopJitJSScript(cx)->ionScript()),
    rval_(rval),
    disabled_(false)
{ }

bool
InvokeFunction(JSContext* cx, HandleObject obj, bool constructing, bool ignoresReturnValue,
               uint32_t argc, Value* argv, MutableHandleValue rval)
{
    TraceLoggerThread* logger = TraceLoggerForCurrentThread(cx);
    TraceLogStartEvent(logger, TraceLogger_Call);

    AutoArrayRooter argvRoot(cx, argc + 1 + constructing, argv);

    // Data in the argument vector is arranged for a JIT -> JIT call.
    RootedValue thisv(cx, argv[0]);
    Value* argvWithoutThis = argv + 1;

    RootedValue fval(cx, ObjectValue(*obj));
    if (constructing) {
        if (!IsConstructor(fval)) {
            ReportValueError(cx, JSMSG_NOT_CONSTRUCTOR, JSDVG_IGNORE_STACK, fval, nullptr);
            return false;
        }

        ConstructArgs cargs(cx);
        if (!cargs.init(cx, argc)) {
            return false;
        }

        for (uint32_t i = 0; i < argc; i++) {
            cargs[i].set(argvWithoutThis[i]);
        }

        RootedValue newTarget(cx, argvWithoutThis[argc]);

        // If |this| hasn't been created, or is JS_UNINITIALIZED_LEXICAL,
        // we can use normal construction code without creating an extraneous
        // object.
        if (thisv.isMagic()) {
            MOZ_ASSERT(thisv.whyMagic() == JS_IS_CONSTRUCTING ||
                       thisv.whyMagic() == JS_UNINITIALIZED_LEXICAL);

            RootedObject obj(cx);
            if (!Construct(cx, fval, cargs, newTarget, &obj)) {
                return false;
            }

            rval.setObject(*obj);
            return true;
        }

        // Otherwise the default |this| has already been created.  We could
        // almost perform a *call* at this point, but we'd break |new.target|
        // in the function.  So in this one weird case we call a one-off
        // construction path that *won't* set |this| to JS_IS_CONSTRUCTING.
        return InternalConstructWithProvidedThis(cx, fval, thisv, cargs, newTarget, rval);
    }
=======
// Helper template to build the VMFunctionData for a function.
template <typename... Args>
struct VMFunctionDataHelper;

template <class R, typename... Args>
struct VMFunctionDataHelper<R (*)(JSContext*, Args...)>
    : public VMFunctionData {
  using Fun = R (*)(JSContext*, Args...);

  static constexpr DataType returnType() { return TypeToDataType<R>::result; }
  static constexpr DataType outParam() {
    return OutParamToDataType<typename LastArg<Args...>::Type>::result;
  }
  static constexpr RootType outParamRootType() {
    return OutParamToRootType<typename LastArg<Args...>::Type>::result;
  }
  static constexpr size_t NbArgs() { return LastArg<Args...>::nbArgs; }
  static constexpr size_t explicitArgs() {
    return NbArgs() - (outParam() != Type_Void ? 1 : 0);
  }
  static constexpr uint32_t argumentProperties() {
    return BitMask<TypeToArgProperties, uint32_t, 2, Args...>::result;
  }
  static constexpr uint32_t argumentPassedInFloatRegs() {
    return BitMask<TypeToPassInFloatReg, uint32_t, 2, Args...>::result;
  }
  static constexpr uint64_t argumentRootTypes() {
    return BitMask<TypeToRootType, uint64_t, 3, Args...>::result;
  }
  constexpr explicit VMFunctionDataHelper(const char* name)
      : VMFunctionData(name, explicitArgs(), argumentProperties(),
                       argumentPassedInFloatRegs(), argumentRootTypes(),
                       outParam(), outParamRootType(), returnType(),
                       /* extraValuesToPop = */ 0, NonTailCall) {}
  constexpr explicit VMFunctionDataHelper(const char* name,
                                          MaybeTailCall expectTailCall,
                                          PopValues extraValuesToPop)
      : VMFunctionData(name, explicitArgs(), argumentProperties(),
                       argumentPassedInFloatRegs(), argumentRootTypes(),
                       outParam(), outParamRootType(), returnType(),
                       extraValuesToPop.numValues, expectTailCall) {}
};

// GCC warns when the signature does not have matching attributes (for example
// MOZ_MUST_USE). Squelch this warning to avoid a GCC-only footgun.
#if MOZ_IS_GCC
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
    ConstructArgs cargs(cx);
    if (!cargs.init(cx, argc)) {
      return false;
    }
||||||| merged common ancestors
    InvokeArgsMaybeIgnoresReturnValue args(cx, ignoresReturnValue);
    if (!args.init(cx, argc)) {
        return false;
    }
=======
// Generate VMFunctionData array.
static constexpr VMFunctionData vmFunctions[] = {
#define DEF_VMFUNCTION(name, fp) VMFunctionDataHelper<decltype(&(::fp))>(#name),
    VMFUNCTION_LIST(DEF_VMFUNCTION)
#undef DEF_VMFUNCTION
};
static constexpr VMFunctionData tailCallVMFunctions[] = {
#define DEF_VMFUNCTION(name, fp, valuesToPop)              \
  VMFunctionDataHelper<decltype(&(::fp))>(#name, TailCall, \
                                          PopValues(valuesToPop)),
    TAIL_CALL_VMFUNCTION_LIST(DEF_VMFUNCTION)
#undef DEF_VMFUNCTION
};

#if MOZ_IS_GCC
#  pragma GCC diagnostic pop
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
    for (uint32_t i = 0; i < argc; i++) {
      cargs[i].set(argvWithoutThis[i]);
    }
||||||| merged common ancestors
    for (size_t i = 0; i < argc; i++) {
        args[i].set(argvWithoutThis[i]);
    }
=======
// Generate arrays storing C++ function pointers. These pointers are not stored
// in VMFunctionData because there's no good way to cast them to void* in
// constexpr code. Compilers are smart enough to treat the const array below as
// constexpr.
#define DEF_VMFUNCTION(name, fp, ...) (void*)(::fp),
static void* const vmFunctionTargets[] = {VMFUNCTION_LIST(DEF_VMFUNCTION)};
static void* const tailCallVMFunctionTargets[] = {
    TAIL_CALL_VMFUNCTION_LIST(DEF_VMFUNCTION)};
#undef DEF_VMFUNCTION
>>>>>>> upstream-releases

<<<<<<< HEAD
    RootedValue newTarget(cx, argvWithoutThis[argc]);

    // If |this| hasn't been created, or is JS_UNINITIALIZED_LEXICAL,
    // we can use normal construction code without creating an extraneous
    // object.
    if (thisv.isMagic()) {
      MOZ_ASSERT(thisv.whyMagic() == JS_IS_CONSTRUCTING ||
                 thisv.whyMagic() == JS_UNINITIALIZED_LEXICAL);

      RootedObject obj(cx);
      if (!Construct(cx, fval, cargs, newTarget, &obj)) {
        return false;
      }

      rval.setObject(*obj);
      return true;
    }

    // Otherwise the default |this| has already been created.  We could
    // almost perform a *call* at this point, but we'd break |new.target|
    // in the function.  So in this one weird case we call a one-off
    // construction path that *won't* set |this| to JS_IS_CONSTRUCTING.
    return InternalConstructWithProvidedThis(cx, fval, thisv, cargs, newTarget,
                                             rval);
  }

  InvokeArgsMaybeIgnoresReturnValue args(cx, ignoresReturnValue);
  if (!args.init(cx, argc)) {
    return false;
  }

  for (size_t i = 0; i < argc; i++) {
    args[i].set(argvWithoutThis[i]);
  }

  return Call(cx, fval, thisv, args, rval);
||||||| merged common ancestors
    return Call(cx, fval, thisv, args, rval);
=======
const VMFunctionData& GetVMFunction(VMFunctionId id) {
  return vmFunctions[size_t(id)];
}
const VMFunctionData& GetVMFunction(TailCallVMFunctionId id) {
  return tailCallVMFunctions[size_t(id)];
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool InvokeFunctionShuffleNewTarget(JSContext* cx, HandleObject obj,
                                    uint32_t numActualArgs,
                                    uint32_t numFormalArgs, Value* argv,
                                    MutableHandleValue rval) {
  MOZ_ASSERT(numFormalArgs > numActualArgs);
  argv[1 + numActualArgs] = argv[1 + numFormalArgs];
  return InvokeFunction(cx, obj, true, false, numActualArgs, argv, rval);
||||||| merged common ancestors
bool
InvokeFunctionShuffleNewTarget(JSContext* cx, HandleObject obj, uint32_t numActualArgs,
                               uint32_t numFormalArgs, Value* argv, MutableHandleValue rval)
{
    MOZ_ASSERT(numFormalArgs > numActualArgs);
    argv[1 + numActualArgs] = argv[1 + numFormalArgs];
    return InvokeFunction(cx, obj, true, false, numActualArgs, argv, rval);
=======
static void* GetVMFunctionTarget(VMFunctionId id) {
  return vmFunctionTargets[size_t(id)];
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool InvokeFromInterpreterStub(JSContext* cx,
                               InterpreterStubExitFrameLayout* frame) {
  JitFrameLayout* jsFrame = frame->jsFrame();
  CalleeToken token = jsFrame->calleeToken();
||||||| merged common ancestors
bool
InvokeFromInterpreterStub(JSContext* cx, InterpreterStubExitFrameLayout* frame)
{
    JitFrameLayout* jsFrame = frame->jsFrame();
    CalleeToken token = jsFrame->calleeToken();
=======
static void* GetVMFunctionTarget(TailCallVMFunctionId id) {
  return tailCallVMFunctionTargets[size_t(id)];
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Value* argv = jsFrame->argv();
  uint32_t numActualArgs = jsFrame->numActualArgs();
  bool constructing = CalleeTokenIsConstructing(token);
  RootedFunction fun(cx, CalleeTokenToFunction(token));
||||||| merged common ancestors
    Value* argv = jsFrame->argv();
    uint32_t numActualArgs = jsFrame->numActualArgs();
    bool constructing = CalleeTokenIsConstructing(token);
    RootedFunction fun(cx, CalleeTokenToFunction(token));
=======
template <typename IdT>
bool JitRuntime::generateVMWrappers(JSContext* cx, MacroAssembler& masm,
                                    VMWrapperOffsets& offsets) {
  // Generate all VM function wrappers.
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Ensure new.target immediately follows the actual arguments (the arguments
  // rectifier added padding). See also InvokeFunctionShuffleNewTarget.
  if (constructing && numActualArgs < fun->nargs()) {
    argv[1 + numActualArgs] = argv[1 + fun->nargs()];
  }
||||||| merged common ancestors
    // Ensure new.target immediately follows the actual arguments (the arguments
    // rectifier added padding). See also InvokeFunctionShuffleNewTarget.
    if (constructing && numActualArgs < fun->nargs()) {
        argv[1 + numActualArgs] = argv[1 + fun->nargs()];
    }
=======
  static constexpr size_t NumVMFunctions = size_t(IdT::Count);
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedValue rval(cx);
  if (!InvokeFunction(cx, fun, constructing,
                      /* ignoresReturnValue = */ false, numActualArgs, argv,
                      &rval)) {
    return false;
  }
||||||| merged common ancestors
    RootedValue rval(cx);
    if (!InvokeFunction(cx, fun, constructing,
                        /* ignoresReturnValue = */ false,
                        numActualArgs, argv, &rval))
    {
        return false;
    }
=======
  if (!offsets.reserve(NumVMFunctions)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Overwrite |this| with the return value.
  argv[0] = rval;
  return true;
}
||||||| merged common ancestors
    // Overwrite |this| with the return value.
    argv[0] = rval;
    return true;
}
=======
#ifdef DEBUG
  const char* lastName = nullptr;
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
bool CheckOverRecursed(JSContext* cx) {
  // We just failed the jitStackLimit check. There are two possible reasons:
  //  1) jitStackLimit was the real stack limit and we're over-recursed
  //  2) jitStackLimit was set to UINTPTR_MAX by JSContext::requestInterrupt
  //     and we need to call JSContext::handleInterrupt.
||||||| merged common ancestors
bool
CheckOverRecursed(JSContext* cx)
{
    // We just failed the jitStackLimit check. There are two possible reasons:
    //  1) jitStackLimit was the real stack limit and we're over-recursed
    //  2) jitStackLimit was set to UINTPTR_MAX by JSContext::requestInterrupt
    //     and we need to call JSContext::handleInterrupt.
=======
  for (size_t i = 0; i < NumVMFunctions; i++) {
    IdT id = IdT(i);
    const VMFunctionData& fun = GetVMFunction(id);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // This handles 1).
#ifdef JS_SIMULATOR
  if (cx->simulator()->overRecursedWithExtra(0)) {
    ReportOverRecursed(cx);
    return false;
  }
#else
  if (!CheckRecursionLimit(cx)) {
    return false;
  }
||||||| merged common ancestors
    // This handles 1).
#ifdef JS_SIMULATOR
    if (cx->simulator()->overRecursedWithExtra(0)) {
        ReportOverRecursed(cx);
        return false;
    }
#else
    if (!CheckRecursionLimit(cx)) {
        return false;
    }
=======
#ifdef DEBUG
    // Assert the list is sorted by name.
    if (lastName) {
      MOZ_ASSERT(strcmp(lastName, fun.name()) < 0,
                 "VM function list must be sorted by name");
    }
    lastName = fun.name();
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  // This handles 2).
  gc::MaybeVerifyBarriers(cx);
  return cx->handleInterrupt();
}
||||||| merged common ancestors
    // This handles 2).
    gc::MaybeVerifyBarriers(cx);
    return cx->handleInterrupt();
}
=======
    JitSpew(JitSpew_Codegen, "# VM function wrapper (%s)", fun.name());
>>>>>>> upstream-releases

<<<<<<< HEAD
// This function gets called when the overrecursion check fails for a Baseline
// frame. This is just like CheckOverRecursed, with an extra check to handle
// early stack check failures.
bool CheckOverRecursedBaseline(JSContext* cx, BaselineFrame* frame) {
  // The OVERRECURSED flag may have already been set on the frame by an
  // early over-recursed check (before pushing the locals).  If so, throw
  // immediately.
  if (frame->overRecursed()) {
    ReportOverRecursed(cx);
    return false;
  }
||||||| merged common ancestors
// This function gets called when the overrecursion check fails for a Baseline
// frame. This is just like CheckOverRecursed, with an extra check to handle
// early stack check failures.
bool
CheckOverRecursedBaseline(JSContext* cx, BaselineFrame* frame)
{
    // The OVERRECURSED flag may have already been set on the frame by an
    // early over-recursed check (before pushing the locals).  If so, throw
    // immediately.
    if (frame->overRecursed()) {
        ReportOverRecursed(cx);
        return false;
    }
=======
    uint32_t offset;
    if (!generateVMWrapper(cx, masm, fun, GetVMFunctionTarget(id), &offset)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return CheckOverRecursed(cx);
}
||||||| merged common ancestors
    return CheckOverRecursed(cx);
}
=======
    MOZ_ASSERT(offsets.length() == size_t(id));
    offsets.infallibleAppend(offset);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JSObject* BindVar(JSContext* cx, HandleObject envChain) {
  JSObject* obj = envChain;
  while (!obj->isQualifiedVarObj()) {
    obj = obj->enclosingEnvironment();
  }
  MOZ_ASSERT(obj);
  return obj;
}
||||||| merged common ancestors
JSObject*
BindVar(JSContext* cx, HandleObject envChain)
{
    JSObject* obj = envChain;
    while (!obj->isQualifiedVarObj()) {
        obj = obj->enclosingEnvironment();
    }
    MOZ_ASSERT(obj);
    return obj;
}
=======
  return true;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
bool DefVar(JSContext* cx, HandlePropertyName dn, unsigned attrs,
            HandleObject envChain) {
  // Given the ScopeChain, extract the VarObj.
  RootedObject obj(cx, BindVar(cx, envChain));
  return DefVarOperation(cx, obj, dn, attrs);
}
||||||| merged common ancestors
bool
DefVar(JSContext* cx, HandlePropertyName dn, unsigned attrs, HandleObject envChain)
{
    // Given the ScopeChain, extract the VarObj.
    RootedObject obj(cx, BindVar(cx, envChain));
    return DefVarOperation(cx, obj, dn, attrs);
}
=======
bool JitRuntime::generateVMWrappers(JSContext* cx, MacroAssembler& masm) {
  if (!generateVMWrappers<VMFunctionId>(cx, masm, functionWrapperOffsets_)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool DefLexical(JSContext* cx, HandlePropertyName dn, unsigned attrs,
                HandleObject envChain) {
  // Find the extensible lexical scope.
  Rooted<LexicalEnvironmentObject*> lexicalEnv(
      cx, &NearestEnclosingExtensibleLexicalEnvironment(envChain));
||||||| merged common ancestors
bool
DefLexical(JSContext* cx, HandlePropertyName dn, unsigned attrs, HandleObject envChain)
{
    // Find the extensible lexical scope.
    Rooted<LexicalEnvironmentObject*> lexicalEnv(cx,
        &NearestEnclosingExtensibleLexicalEnvironment(envChain));
=======
  if (!generateVMWrappers<TailCallVMFunctionId>(
          cx, masm, tailCallFunctionWrapperOffsets_)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Find the variables object.
  RootedObject varObj(cx, BindVar(cx, envChain));
  return DefLexicalOperation(cx, lexicalEnv, varObj, dn, attrs);
||||||| merged common ancestors
    // Find the variables object.
    RootedObject varObj(cx, BindVar(cx, envChain));
    return DefLexicalOperation(cx, lexicalEnv, varObj, dn, attrs);
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool DefGlobalLexical(JSContext* cx, HandlePropertyName dn, unsigned attrs) {
  Rooted<LexicalEnvironmentObject*> globalLexical(
      cx, &cx->global()->lexicalEnvironment());
  return DefLexicalOperation(cx, globalLexical, cx->global(), dn, attrs);
}
||||||| merged common ancestors
bool
DefGlobalLexical(JSContext* cx, HandlePropertyName dn, unsigned attrs)
{
    Rooted<LexicalEnvironmentObject*> globalLexical(cx, &cx->global()->lexicalEnvironment());
    return DefLexicalOperation(cx, globalLexical, cx->global(), dn, attrs);
}
=======
AutoDetectInvalidation::AutoDetectInvalidation(JSContext* cx,
                                               MutableHandleValue rval)
    : cx_(cx),
      ionScript_(GetTopJitJSScript(cx)->ionScript()),
      rval_(rval),
      disabled_(false) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool MutatePrototype(JSContext* cx, HandlePlainObject obj, HandleValue value) {
  if (!value.isObjectOrNull()) {
    return true;
  }
||||||| merged common ancestors
bool
MutatePrototype(JSContext* cx, HandlePlainObject obj, HandleValue value)
{
    if (!value.isObjectOrNull()) {
        return true;
    }

    RootedObject newProto(cx, value.toObjectOrNull());
    return SetPrototype(cx, obj, newProto);
}

template<bool Equal>
bool
LooselyEqual(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs, bool* res)
{
    if (!js::LooselyEqual(cx, lhs, rhs, res)) {
        return false;
    }
    if (!Equal) {
        *res = !*res;
    }
    return true;
}
=======
bool InvokeFunction(JSContext* cx, HandleObject obj, bool constructing,
                    bool ignoresReturnValue, uint32_t argc, Value* argv,
                    MutableHandleValue rval) {
  TraceLoggerThread* logger = TraceLoggerForCurrentThread(cx);
  TraceLogStartEvent(logger, TraceLogger_Call);

  AutoArrayRooter argvRoot(cx, argc + 1 + constructing, argv);

  // Data in the argument vector is arranged for a JIT -> JIT call.
  RootedValue thisv(cx, argv[0]);
  Value* argvWithoutThis = argv + 1;

  RootedValue fval(cx, ObjectValue(*obj));
  if (constructing) {
    if (!IsConstructor(fval)) {
      ReportValueError(cx, JSMSG_NOT_CONSTRUCTOR, JSDVG_IGNORE_STACK, fval,
                       nullptr);
      return false;
    }

    ConstructArgs cargs(cx);
    if (!cargs.init(cx, argc)) {
      return false;
    }

    for (uint32_t i = 0; i < argc; i++) {
      cargs[i].set(argvWithoutThis[i]);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedObject newProto(cx, value.toObjectOrNull());
  return SetPrototype(cx, obj, newProto);
}
||||||| merged common ancestors
template bool LooselyEqual<true>(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs, bool* res);
template bool LooselyEqual<false>(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs, bool* res);
=======
    RootedValue newTarget(cx, argvWithoutThis[argc]);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <bool Equal>
bool LooselyEqual(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs,
                  bool* res) {
  if (!js::LooselyEqual(cx, lhs, rhs, res)) {
    return false;
  }
  if (!Equal) {
    *res = !*res;
  }
  return true;
}

template bool LooselyEqual<true>(JSContext* cx, MutableHandleValue lhs,
                                 MutableHandleValue rhs, bool* res);
template bool LooselyEqual<false>(JSContext* cx, MutableHandleValue lhs,
                                  MutableHandleValue rhs, bool* res);

template <bool Equal>
bool StrictlyEqual(JSContext* cx, MutableHandleValue lhs,
                   MutableHandleValue rhs, bool* res) {
  if (!js::StrictlyEqual(cx, lhs, rhs, res)) {
    return false;
  }
  if (!Equal) {
    *res = !*res;
  }
  return true;
}
||||||| merged common ancestors
template<bool Equal>
bool
StrictlyEqual(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs, bool* res)
{
    if (!js::StrictlyEqual(cx, lhs, rhs, res)) {
        return false;
    }
    if (!Equal) {
        *res = !*res;
    }
    return true;
}
=======
    // If |this| hasn't been created, or is JS_UNINITIALIZED_LEXICAL,
    // we can use normal construction code without creating an extraneous
    // object.
    if (thisv.isMagic()) {
      MOZ_ASSERT(thisv.whyMagic() == JS_IS_CONSTRUCTING ||
                 thisv.whyMagic() == JS_UNINITIALIZED_LEXICAL);

      RootedObject obj(cx);
      if (!Construct(cx, fval, cargs, newTarget, &obj)) {
        return false;
      }

      rval.setObject(*obj);
      return true;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
template bool StrictlyEqual<true>(JSContext* cx, MutableHandleValue lhs,
                                  MutableHandleValue rhs, bool* res);
template bool StrictlyEqual<false>(JSContext* cx, MutableHandleValue lhs,
                                   MutableHandleValue rhs, bool* res);
||||||| merged common ancestors
template bool StrictlyEqual<true>(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs, bool* res);
template bool StrictlyEqual<false>(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs, bool* res);
=======
    // Otherwise the default |this| has already been created.  We could
    // almost perform a *call* at this point, but we'd break |new.target|
    // in the function.  So in this one weird case we call a one-off
    // construction path that *won't* set |this| to JS_IS_CONSTRUCTING.
    return InternalConstructWithProvidedThis(cx, fval, thisv, cargs, newTarget,
                                             rval);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool LessThan(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs,
              bool* res) {
  return LessThanOperation(cx, lhs, rhs, res);
}
||||||| merged common ancestors
bool
LessThan(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs, bool* res)
{
    return LessThanOperation(cx, lhs, rhs, res);
}
=======
  InvokeArgsMaybeIgnoresReturnValue args(cx, ignoresReturnValue);
  if (!args.init(cx, argc)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool LessThanOrEqual(JSContext* cx, MutableHandleValue lhs,
                     MutableHandleValue rhs, bool* res) {
  return LessThanOrEqualOperation(cx, lhs, rhs, res);
}
||||||| merged common ancestors
bool
LessThanOrEqual(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs, bool* res)
{
    return LessThanOrEqualOperation(cx, lhs, rhs, res);
}
=======
  for (size_t i = 0; i < argc; i++) {
    args[i].set(argvWithoutThis[i]);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool GreaterThan(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs,
                 bool* res) {
  return GreaterThanOperation(cx, lhs, rhs, res);
||||||| merged common ancestors
bool
GreaterThan(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs, bool* res)
{
    return GreaterThanOperation(cx, lhs, rhs, res);
=======
  return Call(cx, fval, thisv, args, rval);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool GreaterThanOrEqual(JSContext* cx, MutableHandleValue lhs,
                        MutableHandleValue rhs, bool* res) {
  return GreaterThanOrEqualOperation(cx, lhs, rhs, res);
||||||| merged common ancestors
bool
GreaterThanOrEqual(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs, bool* res)
{
    return GreaterThanOrEqualOperation(cx, lhs, rhs, res);
=======
bool InvokeFunctionShuffleNewTarget(JSContext* cx, HandleObject obj,
                                    uint32_t numActualArgs,
                                    uint32_t numFormalArgs, Value* argv,
                                    MutableHandleValue rval) {
  MOZ_ASSERT(numFormalArgs > numActualArgs);
  argv[1 + numActualArgs] = argv[1 + numFormalArgs];
  return InvokeFunction(cx, obj, true, false, numActualArgs, argv, rval);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <bool Equal>
bool StringsEqual(JSContext* cx, HandleString lhs, HandleString rhs,
                  bool* res) {
  if (!js::EqualStrings(cx, lhs, rhs, res)) {
    return false;
  }
  if (!Equal) {
    *res = !*res;
  }
  return true;
}
||||||| merged common ancestors
template<bool Equal>
bool
StringsEqual(JSContext* cx, HandleString lhs, HandleString rhs, bool* res)
{
    if (!js::EqualStrings(cx, lhs, rhs, res)) {
        return false;
    }
    if (!Equal) {
        *res = !*res;
    }
    return true;
}
=======
bool InvokeFromInterpreterStub(JSContext* cx,
                               InterpreterStubExitFrameLayout* frame) {
  JitFrameLayout* jsFrame = frame->jsFrame();
  CalleeToken token = jsFrame->calleeToken();
>>>>>>> upstream-releases

<<<<<<< HEAD
template bool StringsEqual<true>(JSContext* cx, HandleString lhs,
                                 HandleString rhs, bool* res);
template bool StringsEqual<false>(JSContext* cx, HandleString lhs,
                                  HandleString rhs, bool* res);
||||||| merged common ancestors
template bool StringsEqual<true>(JSContext* cx, HandleString lhs, HandleString rhs, bool* res);
template bool StringsEqual<false>(JSContext* cx, HandleString lhs, HandleString rhs, bool* res);
=======
  Value* argv = jsFrame->argv();
  uint32_t numActualArgs = jsFrame->numActualArgs();
  bool constructing = CalleeTokenIsConstructing(token);
  RootedFunction fun(cx, CalleeTokenToFunction(token));
>>>>>>> upstream-releases

  // Ensure new.target immediately follows the actual arguments (the arguments
  // rectifier added padding). See also InvokeFunctionShuffleNewTarget.
  if (constructing && numActualArgs < fun->nargs()) {
    argv[1 + numActualArgs] = argv[1 + fun->nargs()];
  }

<<<<<<< HEAD
bool StringSplitHelper(JSContext* cx, HandleString str, HandleString sep,
                       HandleObjectGroup group, uint32_t limit,
                       MutableHandleValue result) {
  JSObject* resultObj = str_split_string(cx, group, str, sep, limit);
  if (!resultObj) {
    return false;
  }
||||||| merged common ancestors
bool StringSplitHelper(JSContext* cx, HandleString str, HandleString sep,
                       HandleObjectGroup group, uint32_t limit,
                       MutableHandleValue result)
{
    JSObject* resultObj = str_split_string(cx, group, str, sep, limit);
    if (!resultObj) {
        return false;
    }
=======
  RootedValue rval(cx);
  if (!InvokeFunction(cx, fun, constructing,
                      /* ignoresReturnValue = */ false, numActualArgs, argv,
                      &rval)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  result.setObject(*resultObj);
  return true;
||||||| merged common ancestors
    result.setObject(*resultObj);
    return true;
=======
  // Overwrite |this| with the return value.
  argv[0] = rval;
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*StringSplitHelperFn)(JSContext*, HandleString, HandleString,
                                    HandleObjectGroup, uint32_t limit,
                                    MutableHandleValue);
const VMFunction StringSplitHelperInfo =
    FunctionInfo<StringSplitHelperFn>(StringSplitHelper, "StringSplitHelper");

bool ArrayPopDense(JSContext* cx, HandleObject obj, MutableHandleValue rval) {
  MOZ_ASSERT(obj->is<ArrayObject>());
||||||| merged common ancestors
typedef bool (*StringSplitHelperFn)(JSContext*, HandleString, HandleString, HandleObjectGroup,
                                    uint32_t limit, MutableHandleValue);
const VMFunction StringSplitHelperInfo =
    FunctionInfo<StringSplitHelperFn>(StringSplitHelper, "StringSplitHelper");

bool
ArrayPopDense(JSContext* cx, HandleObject obj, MutableHandleValue rval)
{
    MOZ_ASSERT(obj->is<ArrayObject>());

    AutoDetectInvalidation adi(cx, rval);

    JS::AutoValueArray<2> argv(cx);
    argv[0].setUndefined();
    argv[1].setObject(*obj);
    if (!js::array_pop(cx, 0, argv.begin())) {
        return false;
    }

    // If the result is |undefined|, the array was probably empty and we
    // have to monitor the return value.
    rval.set(argv[0]);
    if (rval.isUndefined()) {
        TypeScript::Monitor(cx, rval);
    }
    return true;
}
=======
bool CheckOverRecursed(JSContext* cx) {
  // We just failed the jitStackLimit check. There are two possible reasons:
  //  1) jitStackLimit was the real stack limit and we're over-recursed
  //  2) jitStackLimit was set to UINTPTR_MAX by JSContext::requestInterrupt
  //     and we need to call JSContext::handleInterrupt.

  // This handles 1).
#ifdef JS_SIMULATOR
  if (cx->simulator()->overRecursedWithExtra(0)) {
    ReportOverRecursed(cx);
    return false;
  }
#else
  if (!CheckRecursionLimit(cx)) {
    return false;
  }
#endif

  // This handles 2).
  gc::MaybeVerifyBarriers(cx);
  return cx->handleInterrupt();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoDetectInvalidation adi(cx, rval);

  JS::AutoValueArray<2> argv(cx);
  argv[0].setUndefined();
  argv[1].setObject(*obj);
  if (!js::array_pop(cx, 0, argv.begin())) {
    return false;
  }
||||||| merged common ancestors
bool
ArrayPushDense(JSContext* cx, HandleArrayObject arr, HandleValue v, uint32_t* length)
{
    *length = arr->length();
    DenseElementResult result = arr->setOrExtendDenseElements(cx, *length, v.address(), 1,
                                                              ShouldUpdateTypes::DontUpdate);
    if (result != DenseElementResult::Incomplete) {
        (*length)++;
        return result == DenseElementResult::Success;
    }

    // AutoDetectInvalidation uses GetTopJitJSScript(cx)->ionScript(), but it's
    // possible the setOrExtendDenseElements call already invalidated the
    // IonScript. JSJitFrameIter::ionScript works when the script is invalidated
    // so we use that instead.
    JSJitFrameIter frame(cx->activation()->asJit());
    MOZ_ASSERT(frame.type() == FrameType::Exit);
    ++frame;
    IonScript* ionScript = frame.ionScript();

    JS::AutoValueArray<3> argv(cx);
    AutoDetectInvalidation adi(cx, argv[0], ionScript);
    argv[0].setUndefined();
    argv[1].setObject(*arr);
    argv[2].set(v);
    if (!js::array_push(cx, 1, argv.begin())) {
        return false;
    }
=======
// This function gets called when the overrecursion check fails for a Baseline
// frame. This is just like CheckOverRecursed, with an extra check to handle
// early stack check failures.
bool CheckOverRecursedBaseline(JSContext* cx, BaselineFrame* frame) {
  // The OVERRECURSED flag may have already been set on the frame by an
  // early over-recursed check (before pushing the locals).  If so, throw
  // immediately.
  if (frame->overRecursed()) {
    ReportOverRecursed(cx);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If the result is |undefined|, the array was probably empty and we
  // have to monitor the return value.
  rval.set(argv[0]);
  if (rval.isUndefined()) {
    TypeScript::Monitor(cx, rval);
  }
  return true;
}

bool ArrayPushDense(JSContext* cx, HandleArrayObject arr, HandleValue v,
                    uint32_t* length) {
  *length = arr->length();
  DenseElementResult result = arr->setOrExtendDenseElements(
      cx, *length, v.address(), 1, ShouldUpdateTypes::DontUpdate);
  if (result != DenseElementResult::Incomplete) {
    (*length)++;
    return result == DenseElementResult::Success;
  }

  // AutoDetectInvalidation uses GetTopJitJSScript(cx)->ionScript(), but it's
  // possible the setOrExtendDenseElements call already invalidated the
  // IonScript. JSJitFrameIter::ionScript works when the script is invalidated
  // so we use that instead.
  JSJitFrameIter frame(cx->activation()->asJit());
  MOZ_ASSERT(frame.type() == FrameType::Exit);
  ++frame;
  IonScript* ionScript = frame.ionScript();

  JS::AutoValueArray<3> argv(cx);
  AutoDetectInvalidation adi(cx, argv[0], ionScript);
  argv[0].setUndefined();
  argv[1].setObject(*arr);
  argv[2].set(v);
  if (!js::array_push(cx, 1, argv.begin())) {
    return false;
  }
||||||| merged common ancestors
    if (argv[0].isInt32()) {
        *length = argv[0].toInt32();
        return true;
    }
=======
  return CheckOverRecursed(cx);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (argv[0].isInt32()) {
    *length = argv[0].toInt32();
||||||| merged common ancestors
    // array_push changed the length to be larger than INT32_MAX. In this case
    // OBJECT_FLAG_LENGTH_OVERFLOW was set, TI invalidated the script, and the
    // AutoDetectInvalidation instance on the stack will replace *length with
    // the actual return value during bailout.
    MOZ_ASSERT(adi.shouldSetReturnOverride());
    MOZ_ASSERT(argv[0].toDouble() == double(INT32_MAX) + 1);
    *length = 0;
=======
bool MutatePrototype(JSContext* cx, HandlePlainObject obj, HandleValue value) {
  if (!value.isObjectOrNull()) {
>>>>>>> upstream-releases
    return true;
<<<<<<< HEAD
  }

  // array_push changed the length to be larger than INT32_MAX. In this case
  // OBJECT_FLAG_LENGTH_OVERFLOW was set, TI invalidated the script, and the
  // AutoDetectInvalidation instance on the stack will replace *length with
  // the actual return value during bailout.
  MOZ_ASSERT(adi.shouldSetReturnOverride());
  MOZ_ASSERT(argv[0].toDouble() == double(INT32_MAX) + 1);
  *length = 0;
  return true;
}
||||||| merged common ancestors
}
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool ArrayShiftDense(JSContext* cx, HandleObject obj, MutableHandleValue rval) {
  MOZ_ASSERT(obj->is<ArrayObject>());
||||||| merged common ancestors
bool
ArrayShiftDense(JSContext* cx, HandleObject obj, MutableHandleValue rval)
{
    MOZ_ASSERT(obj->is<ArrayObject>());
=======
  RootedObject newProto(cx, value.toObjectOrNull());
  return SetPrototype(cx, obj, newProto);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoDetectInvalidation adi(cx, rval);
||||||| merged common ancestors
    AutoDetectInvalidation adi(cx, rval);
=======
template <EqualityKind Kind>
bool LooselyEqual(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs,
                  bool* res) {
  if (!js::LooselyEqual(cx, lhs, rhs, res)) {
    return false;
  }
  if (Kind != EqualityKind::Equal) {
    *res = !*res;
  }
  return true;
}

template bool LooselyEqual<EqualityKind::Equal>(JSContext* cx,
                                                MutableHandleValue lhs,
                                                MutableHandleValue rhs,
                                                bool* res);
template bool LooselyEqual<EqualityKind::NotEqual>(JSContext* cx,
                                                   MutableHandleValue lhs,
                                                   MutableHandleValue rhs,
                                                   bool* res);

template <EqualityKind Kind>
bool StrictlyEqual(JSContext* cx, MutableHandleValue lhs,
                   MutableHandleValue rhs, bool* res) {
  if (!js::StrictlyEqual(cx, lhs, rhs, res)) {
    return false;
  }
  if (Kind != EqualityKind::Equal) {
    *res = !*res;
  }
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::AutoValueArray<2> argv(cx);
  argv[0].setUndefined();
  argv[1].setObject(*obj);
  if (!js::array_shift(cx, 0, argv.begin())) {
    return false;
  }
||||||| merged common ancestors
    JS::AutoValueArray<2> argv(cx);
    argv[0].setUndefined();
    argv[1].setObject(*obj);
    if (!js::array_shift(cx, 0, argv.begin())) {
        return false;
    }
=======
template bool StrictlyEqual<EqualityKind::Equal>(JSContext* cx,
                                                 MutableHandleValue lhs,
                                                 MutableHandleValue rhs,
                                                 bool* res);
template bool StrictlyEqual<EqualityKind::NotEqual>(JSContext* cx,
                                                    MutableHandleValue lhs,
                                                    MutableHandleValue rhs,
                                                    bool* res);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If the result is |undefined|, the array was probably empty and we
  // have to monitor the return value.
  rval.set(argv[0]);
  if (rval.isUndefined()) {
    TypeScript::Monitor(cx, rval);
  }
  return true;
||||||| merged common ancestors
    // If the result is |undefined|, the array was probably empty and we
    // have to monitor the return value.
    rval.set(argv[0]);
    if (rval.isUndefined()) {
        TypeScript::Monitor(cx, rval);
    }
    return true;
=======
bool LessThan(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs,
              bool* res) {
  return LessThanOperation(cx, lhs, rhs, res);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSString* ArrayJoin(JSContext* cx, HandleObject array, HandleString sep) {
  JS::AutoValueArray<3> argv(cx);
  argv[0].setUndefined();
  argv[1].setObject(*array);
  argv[2].setString(sep);
  if (!js::array_join(cx, 1, argv.begin())) {
    return nullptr;
  }
  return argv[0].toString();
}

bool SetArrayLength(JSContext* cx, HandleObject obj, HandleValue value,
                    bool strict) {
  Handle<ArrayObject*> array = obj.as<ArrayObject>();

  RootedId id(cx, NameToId(cx->names().length));
  ObjectOpResult result;

  // SetArrayLength is called by IC stubs for SetProp and SetElem on arrays'
  // "length" property.
  //
  // ArraySetLength below coerces |value| before checking for length being
  // writable, and in the case of illegal values, will throw RangeError even
  // when "length" is not writable. This is incorrect observable behavior,
  // as a regular [[Set]] operation will check for "length" being
  // writable before attempting any assignment.
  //
  // So, perform ArraySetLength if and only if "length" is writable.
  if (array->lengthIsWritable()) {
    if (!ArraySetLength(cx, array, id, JSPROP_PERMANENT, value, result)) {
      return false;
    }
  } else {
    MOZ_ALWAYS_TRUE(result.fail(JSMSG_READ_ONLY));
  }

  return result.checkStrictErrorOrWarning(cx, obj, id, strict);
||||||| merged common ancestors
JSString*
ArrayJoin(JSContext* cx, HandleObject array, HandleString sep)
{
    JS::AutoValueArray<3> argv(cx);
    argv[0].setUndefined();
    argv[1].setObject(*array);
    argv[2].setString(sep);
    if (!js::array_join(cx, 1, argv.begin())) {
        return nullptr;
    }
    return argv[0].toString();
}

bool
SetArrayLength(JSContext* cx, HandleObject obj, HandleValue value, bool strict)
{
    Handle<ArrayObject*> array = obj.as<ArrayObject>();

    RootedId id(cx, NameToId(cx->names().length));
    ObjectOpResult result;

    // SetArrayLength is called by IC stubs for SetProp and SetElem on arrays'
    // "length" property.
    //
    // ArraySetLength below coerces |value| before checking for length being
    // writable, and in the case of illegal values, will throw RangeError even
    // when "length" is not writable. This is incorrect observable behavior,
    // as a regular [[Set]] operation will check for "length" being
    // writable before attempting any assignment.
    //
    // So, perform ArraySetLength if and only if "length" is writable.
    if (array->lengthIsWritable()) {
        if (!ArraySetLength(cx, array, id, JSPROP_PERMANENT, value, result)) {
            return false;
        }
    } else {
        MOZ_ALWAYS_TRUE(result.fail(JSMSG_READ_ONLY));
    }

    return result.checkStrictErrorOrWarning(cx, obj, id, strict);
=======
bool LessThanOrEqual(JSContext* cx, MutableHandleValue lhs,
                     MutableHandleValue rhs, bool* res) {
  return LessThanOrEqualOperation(cx, lhs, rhs, res);
>>>>>>> upstream-releases
}

bool GreaterThan(JSContext* cx, MutableHandleValue lhs, MutableHandleValue rhs,
                 bool* res) {
  return GreaterThanOperation(cx, lhs, rhs, res);
}

<<<<<<< HEAD
bool CharCodeAt(JSContext* cx, HandleString str, int32_t index,
                uint32_t* code) {
  char16_t c;
  if (!str->getChar(cx, index, &c)) {
    return false;
  }
  *code = c;
  return true;
||||||| merged common ancestors
bool
CharCodeAt(JSContext* cx, HandleString str, int32_t index, uint32_t* code)
{
    char16_t c;
    if (!str->getChar(cx, index, &c)) {
        return false;
    }
    *code = c;
    return true;
=======
bool GreaterThanOrEqual(JSContext* cx, MutableHandleValue lhs,
                        MutableHandleValue rhs, bool* res) {
  return GreaterThanOrEqualOperation(cx, lhs, rhs, res);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSFlatString* StringFromCharCode(JSContext* cx, int32_t code) {
  char16_t c = char16_t(code);
||||||| merged common ancestors
JSFlatString*
StringFromCharCode(JSContext* cx, int32_t code)
{
    char16_t c = char16_t(code);
=======
template <EqualityKind Kind>
bool StringsEqual(JSContext* cx, HandleString lhs, HandleString rhs,
                  bool* res) {
  if (!js::EqualStrings(cx, lhs, rhs, res)) {
    return false;
  }
  if (Kind != EqualityKind::Equal) {
    *res = !*res;
  }
  return true;
}

template bool StringsEqual<EqualityKind::Equal>(JSContext* cx, HandleString lhs,
                                                HandleString rhs, bool* res);
template bool StringsEqual<EqualityKind::NotEqual>(JSContext* cx,
                                                   HandleString lhs,
                                                   HandleString rhs, bool* res);

template <ComparisonKind Kind>
bool StringsCompare(JSContext* cx, HandleString lhs, HandleString rhs,
                    bool* res) {
  int32_t result;
  if (!js::CompareStrings(cx, lhs, rhs, &result)) {
    return false;
  }
  if (Kind == ComparisonKind::LessThan) {
    *res = result < 0;
  } else {
    *res = result >= 0;
  }
  return true;
}

template bool StringsCompare<ComparisonKind::LessThan>(JSContext* cx,
                                                       HandleString lhs,
                                                       HandleString rhs,
                                                       bool* res);
template bool StringsCompare<ComparisonKind::GreaterThanOrEqual>(
    JSContext* cx, HandleString lhs, HandleString rhs, bool* res);

bool ArrayPopDense(JSContext* cx, HandleObject obj, MutableHandleValue rval) {
  MOZ_ASSERT(obj->is<ArrayObject>());

  AutoDetectInvalidation adi(cx, rval);

  JS::AutoValueArray<2> argv(cx);
  argv[0].setUndefined();
  argv[1].setObject(*obj);
  if (!js::array_pop(cx, 0, argv.begin())) {
    return false;
  }

  // If the result is |undefined|, the array was probably empty and we
  // have to monitor the return value.
  rval.set(argv[0]);
  if (rval.isUndefined()) {
    jsbytecode* pc;
    JSScript* script = cx->currentScript(&pc);
    JitScript::MonitorBytecodeType(cx, script, pc, rval);
  }
  return true;
}

bool ArrayPushDense(JSContext* cx, HandleArrayObject arr, HandleValue v,
                    uint32_t* length) {
  *length = arr->length();
  DenseElementResult result = arr->setOrExtendDenseElements(
      cx, *length, v.address(), 1, ShouldUpdateTypes::DontUpdate);
  if (result != DenseElementResult::Incomplete) {
    (*length)++;
    return result == DenseElementResult::Success;
  }

  // AutoDetectInvalidation uses GetTopJitJSScript(cx)->ionScript(), but it's
  // possible the setOrExtendDenseElements call already invalidated the
  // IonScript. JSJitFrameIter::ionScript works when the script is invalidated
  // so we use that instead.
  JSJitFrameIter frame(cx->activation()->asJit());
  MOZ_ASSERT(frame.type() == FrameType::Exit);
  ++frame;
  IonScript* ionScript = frame.ionScript();

  JS::AutoValueArray<3> argv(cx);
  AutoDetectInvalidation adi(cx, argv[0], ionScript);
  argv[0].setUndefined();
  argv[1].setObject(*arr);
  argv[2].set(v);
  if (!js::array_push(cx, 1, argv.begin())) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (StaticStrings::hasUnit(c)) {
    return cx->staticStrings().getUnit(c);
  }
||||||| merged common ancestors
    if (StaticStrings::hasUnit(c)) {
        return cx->staticStrings().getUnit(c);
    }
=======
  if (argv[0].isInt32()) {
    *length = argv[0].toInt32();
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return NewStringCopyNDontDeflate<CanGC>(cx, &c, 1);
||||||| merged common ancestors
    return NewStringCopyNDontDeflate<CanGC>(cx, &c, 1);
=======
  // array_push changed the length to be larger than INT32_MAX. In this case
  // OBJECT_FLAG_LENGTH_OVERFLOW was set, TI invalidated the script, and the
  // AutoDetectInvalidation instance on the stack will replace *length with
  // the actual return value during bailout.
  MOZ_ASSERT(adi.shouldSetReturnOverride());
  MOZ_ASSERT(argv[0].toDouble() == double(INT32_MAX) + 1);
  *length = 0;
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSString* StringFromCodePoint(JSContext* cx, int32_t codePoint) {
  RootedValue rval(cx, Int32Value(codePoint));
  if (!str_fromCodePoint_one_arg(cx, rval, &rval)) {
    return nullptr;
  }
||||||| merged common ancestors
JSString*
StringFromCodePoint(JSContext* cx, int32_t codePoint)
{
    RootedValue rval(cx, Int32Value(codePoint));
    if (!str_fromCodePoint_one_arg(cx, rval, &rval)) {
        return nullptr;
    }
=======
bool ArrayShiftDense(JSContext* cx, HandleObject obj, MutableHandleValue rval) {
  MOZ_ASSERT(obj->is<ArrayObject>());
>>>>>>> upstream-releases

<<<<<<< HEAD
  return rval.toString();
}
||||||| merged common ancestors
    return rval.toString();
}
=======
  AutoDetectInvalidation adi(cx, rval);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool SetProperty(JSContext* cx, HandleObject obj, HandlePropertyName name,
                 HandleValue value, bool strict, jsbytecode* pc) {
  RootedId id(cx, NameToId(name));
||||||| merged common ancestors
bool
SetProperty(JSContext* cx, HandleObject obj, HandlePropertyName name, HandleValue value,
            bool strict, jsbytecode* pc)
{
    RootedId id(cx, NameToId(name));
=======
  JS::AutoValueArray<2> argv(cx);
  argv[0].setUndefined();
  argv[1].setObject(*obj);
  if (!js::array_shift(cx, 0, argv.begin())) {
    return false;
  }

  // If the result is |undefined|, the array was probably empty and we
  // have to monitor the return value.
  rval.set(argv[0]);
  if (rval.isUndefined()) {
    jsbytecode* pc;
    JSScript* script = cx->currentScript(&pc);
    JitScript::MonitorBytecodeType(cx, script, pc, rval);
  }
  return true;
}

JSString* ArrayJoin(JSContext* cx, HandleObject array, HandleString sep) {
  JS::AutoValueArray<3> argv(cx);
  argv[0].setUndefined();
  argv[1].setObject(*array);
  argv[2].setString(sep);
  if (!js::array_join(cx, 1, argv.begin())) {
    return nullptr;
  }
  return argv[0].toString();
}

bool SetArrayLength(JSContext* cx, HandleObject obj, HandleValue value,
                    bool strict) {
  Handle<ArrayObject*> array = obj.as<ArrayObject>();

  RootedId id(cx, NameToId(cx->names().length));
  ObjectOpResult result;

  // SetArrayLength is called by IC stubs for SetProp and SetElem on arrays'
  // "length" property.
  //
  // ArraySetLength below coerces |value| before checking for length being
  // writable, and in the case of illegal values, will throw RangeError even
  // when "length" is not writable. This is incorrect observable behavior,
  // as a regular [[Set]] operation will check for "length" being
  // writable before attempting any assignment.
  //
  // So, perform ArraySetLength if and only if "length" is writable.
  if (array->lengthIsWritable()) {
    if (!ArraySetLength(cx, array, id, JSPROP_PERMANENT, value, result)) {
      return false;
    }
  } else {
    MOZ_ALWAYS_TRUE(result.fail(JSMSG_READ_ONLY));
  }

  return result.checkStrictErrorOrWarning(cx, obj, id, strict);
}

bool CharCodeAt(JSContext* cx, HandleString str, int32_t index,
                uint32_t* code) {
  char16_t c;
  if (!str->getChar(cx, index, &c)) {
    return false;
  }
  *code = c;
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSOp op = JSOp(*pc);
||||||| merged common ancestors
    JSOp op = JSOp(*pc);
=======
JSFlatString* StringFromCharCode(JSContext* cx, int32_t code) {
  char16_t c = char16_t(code);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (op == JSOP_SETALIASEDVAR || op == JSOP_INITALIASEDLEXICAL) {
    // Aliased var assigns ignore readonly attributes on the property, as
    // required for initializing 'const' closure variables.
    Shape* shape = obj->as<NativeObject>().lookup(cx, name);
    MOZ_ASSERT(shape && shape->isDataProperty());
    obj->as<NativeObject>().setSlotWithType(cx, shape, value);
    return true;
  }

  RootedValue receiver(cx, ObjectValue(*obj));
  ObjectOpResult result;
  if (MOZ_LIKELY(!obj->getOpsSetProperty())) {
    if (op == JSOP_SETNAME || op == JSOP_STRICTSETNAME || op == JSOP_SETGNAME ||
        op == JSOP_STRICTSETGNAME) {
      if (!NativeSetProperty<Unqualified>(cx, obj.as<NativeObject>(), id, value,
                                          receiver, result)) {
        return false;
      }
    } else {
      if (!NativeSetProperty<Qualified>(cx, obj.as<NativeObject>(), id, value,
                                        receiver, result)) {
        return false;
      }
    }
  } else {
    if (!SetProperty(cx, obj, id, value, receiver, result)) {
      return false;
    }
  }
  return result.checkStrictErrorOrWarning(cx, obj, id, strict);
||||||| merged common ancestors
    if (op == JSOP_SETALIASEDVAR || op == JSOP_INITALIASEDLEXICAL) {
        // Aliased var assigns ignore readonly attributes on the property, as
        // required for initializing 'const' closure variables.
        Shape* shape = obj->as<NativeObject>().lookup(cx, name);
        MOZ_ASSERT(shape && shape->isDataProperty());
        obj->as<NativeObject>().setSlotWithType(cx, shape, value);
        return true;
    }

    RootedValue receiver(cx, ObjectValue(*obj));
    ObjectOpResult result;
    if (MOZ_LIKELY(!obj->getOpsSetProperty())) {
        if (op == JSOP_SETNAME || op == JSOP_STRICTSETNAME ||
            op == JSOP_SETGNAME || op == JSOP_STRICTSETGNAME)
        {
            if (!NativeSetProperty<Unqualified>(cx, obj.as<NativeObject>(), id, value, receiver,
                                                result))
            {
                return false;
            }
        } else {
            if (!NativeSetProperty<Qualified>(cx, obj.as<NativeObject>(), id, value, receiver,
                                              result))
            {
                return false;
            }
        }
    } else {
        if (!SetProperty(cx, obj, id, value, receiver, result)) {
            return false;
        }
    }
    return result.checkStrictErrorOrWarning(cx, obj, id, strict);
=======
  if (StaticStrings::hasUnit(c)) {
    return cx->staticStrings().getUnit(c);
  }

  return NewStringCopyNDontDeflate<CanGC>(cx, &c, 1);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool InterruptCheck(JSContext* cx) {
  gc::MaybeVerifyBarriers(cx);
||||||| merged common ancestors
bool
InterruptCheck(JSContext* cx)
{
    gc::MaybeVerifyBarriers(cx);
=======
JSString* StringFromCodePoint(JSContext* cx, int32_t codePoint) {
  RootedValue rval(cx, Int32Value(codePoint));
  if (!str_fromCodePoint_one_arg(cx, rval, &rval)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return CheckForInterrupt(cx);
||||||| merged common ancestors
    return CheckForInterrupt(cx);
=======
  return rval.toString();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void* MallocWrapper(JS::Zone* zone, size_t nbytes) {
  AutoUnsafeCallWithABI unsafe;
  return zone->pod_malloc<uint8_t>(nbytes);
}
||||||| merged common ancestors
void*
MallocWrapper(JS::Zone* zone, size_t nbytes)
{
    AutoUnsafeCallWithABI unsafe;
    return zone->pod_malloc<uint8_t>(nbytes);
}
=======
bool SetProperty(JSContext* cx, HandleObject obj, HandlePropertyName name,
                 HandleValue value, bool strict, jsbytecode* pc) {
  RootedId id(cx, NameToId(name));
>>>>>>> upstream-releases

<<<<<<< HEAD
JSObject* NewCallObject(JSContext* cx, HandleShape shape,
                        HandleObjectGroup group) {
  JSObject* obj = CallObject::create(cx, shape, group);
  if (!obj) {
    return nullptr;
  }
||||||| merged common ancestors
JSObject*
NewCallObject(JSContext* cx, HandleShape shape, HandleObjectGroup group)
{
    JSObject* obj = CallObject::create(cx, shape, group);
    if (!obj) {
        return nullptr;
    }
=======
  JSOp op = JSOp(*pc);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The JIT creates call objects in the nursery, so elides barriers for
  // the initializing writes. The interpreter, however, may have allocated
  // the call object tenured, so barrier as needed before re-entering.
  if (!IsInsideNursery(obj)) {
    cx->runtime()->gc.storeBuffer().putWholeCell(obj);
  }
||||||| merged common ancestors
    // The JIT creates call objects in the nursery, so elides barriers for
    // the initializing writes. The interpreter, however, may have allocated
    // the call object tenured, so barrier as needed before re-entering.
    if (!IsInsideNursery(obj)) {
        cx->runtime()->gc.storeBuffer().putWholeCell(obj);
    }
=======
  if (op == JSOP_SETALIASEDVAR || op == JSOP_INITALIASEDLEXICAL) {
    // Aliased var assigns ignore readonly attributes on the property, as
    // required for initializing 'const' closure variables.
    Shape* shape = obj->as<NativeObject>().lookup(cx, name);
    MOZ_ASSERT(shape && shape->isDataProperty());
    obj->as<NativeObject>().setSlotWithType(cx, shape, value);
    return true;
  }

  RootedValue receiver(cx, ObjectValue(*obj));
  ObjectOpResult result;
  if (MOZ_LIKELY(!obj->getOpsSetProperty())) {
    if (op == JSOP_SETNAME || op == JSOP_STRICTSETNAME || op == JSOP_SETGNAME ||
        op == JSOP_STRICTSETGNAME) {
      if (!NativeSetProperty<Unqualified>(cx, obj.as<NativeObject>(), id, value,
                                          receiver, result)) {
        return false;
      }
    } else {
      if (!NativeSetProperty<Qualified>(cx, obj.as<NativeObject>(), id, value,
                                        receiver, result)) {
        return false;
      }
    }
  } else {
    if (!SetProperty(cx, obj, id, value, receiver, result)) {
      return false;
    }
  }
  return result.checkStrictErrorOrWarning(cx, obj, id, strict);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return obj;
||||||| merged common ancestors
    return obj;
=======
bool InterruptCheck(JSContext* cx) {
  gc::MaybeVerifyBarriers(cx);

  return CheckForInterrupt(cx);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSObject* NewSingletonCallObject(JSContext* cx, HandleShape shape) {
  JSObject* obj = CallObject::createSingleton(cx, shape);
  if (!obj) {
    return nullptr;
  }
||||||| merged common ancestors
JSObject*
NewSingletonCallObject(JSContext* cx, HandleShape shape)
{
    JSObject* obj = CallObject::createSingleton(cx, shape);
    if (!obj) {
        return nullptr;
    }
=======
JSObject* NewCallObject(JSContext* cx, HandleShape shape,
                        HandleObjectGroup group) {
  JSObject* obj = CallObject::create(cx, shape, group);
  if (!obj) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The JIT creates call objects in the nursery, so elides barriers for
  // the initializing writes. The interpreter, however, may have allocated
  // the call object tenured, so barrier as needed before re-entering.
  MOZ_ASSERT(!IsInsideNursery(obj),
             "singletons are created in the tenured heap");
  cx->runtime()->gc.storeBuffer().putWholeCell(obj);
||||||| merged common ancestors
    // The JIT creates call objects in the nursery, so elides barriers for
    // the initializing writes. The interpreter, however, may have allocated
    // the call object tenured, so barrier as needed before re-entering.
    MOZ_ASSERT(!IsInsideNursery(obj),
               "singletons are created in the tenured heap");
    cx->runtime()->gc.storeBuffer().putWholeCell(obj);
=======
  // The JIT creates call objects in the nursery, so elides barriers for
  // the initializing writes. The interpreter, however, may have allocated
  // the call object tenured, so barrier as needed before re-entering.
  if (!IsInsideNursery(obj)) {
    cx->runtime()->gc.storeBuffer().putWholeCell(obj);
  }
>>>>>>> upstream-releases

  return obj;
}

JSObject* NewStringObject(JSContext* cx, HandleString str) {
  return StringObject::create(cx, str);
}

bool OperatorIn(JSContext* cx, HandleValue key, HandleObject obj, bool* out) {
  RootedId id(cx);
  return ToPropertyKey(cx, key, &id) && HasProperty(cx, obj, id, out);
}

bool OperatorInI(JSContext* cx, uint32_t index, HandleObject obj, bool* out) {
  RootedValue key(cx, Int32Value(index));
  return OperatorIn(cx, key, obj, out);
}

bool GetIntrinsicValue(JSContext* cx, HandlePropertyName name,
                       MutableHandleValue rval) {
  if (!GlobalObject::getIntrinsicValue(cx, cx->global(), name, rval)) {
    return false;
  }

<<<<<<< HEAD
  // This function is called when we try to compile a cold getintrinsic
  // op. MCallGetIntrinsicValue has an AliasSet of None for optimization
  // purposes, as its side effect is not observable from JS. We are
  // guaranteed to bail out after this function, but because of its AliasSet,
  // type info will not be reflowed. Manually monitor here.
  TypeScript::Monitor(cx, rval);
||||||| merged common ancestors
    // This function is called when we try to compile a cold getintrinsic
    // op. MCallGetIntrinsicValue has an AliasSet of None for optimization
    // purposes, as its side effect is not observable from JS. We are
    // guaranteed to bail out after this function, but because of its AliasSet,
    // type info will not be reflowed. Manually monitor here.
    TypeScript::Monitor(cx, rval);
=======
  // This function is called when we try to compile a cold getintrinsic
  // op. MCallGetIntrinsicValue has an AliasSet of None for optimization
  // purposes, as its side effect is not observable from JS. We are
  // guaranteed to bail out after this function, but because of its AliasSet,
  // type info will not be reflowed. Manually monitor here.
  jsbytecode* pc;
  JSScript* script = cx->currentScript(&pc);
  JitScript::MonitorBytecodeType(cx, script, pc, rval);
>>>>>>> upstream-releases

  return true;
}

<<<<<<< HEAD
bool CreateThis(JSContext* cx, HandleObject callee, HandleObject newTarget,
                MutableHandleValue rval) {
  rval.set(MagicValue(JS_IS_CONSTRUCTING));

  if (callee->is<JSFunction>()) {
    RootedFunction fun(cx, &callee->as<JSFunction>());
    if (fun->isInterpreted() && fun->isConstructor()) {
      JSScript* script = JSFunction::getOrCreateScript(cx, fun);
      if (!script) {
        return false;
      }
      AutoRealm ar(cx, script);
      if (!js::CreateThis(cx, fun, script, newTarget, GenericObject, rval)) {
        return false;
      }
||||||| merged common ancestors
bool
CreateThis(JSContext* cx, HandleObject callee, HandleObject newTarget, MutableHandleValue rval)
{
    rval.set(MagicValue(JS_IS_CONSTRUCTING));

    if (callee->is<JSFunction>()) {
        RootedFunction fun(cx, &callee->as<JSFunction>());
        if (fun->isInterpreted() && fun->isConstructor()) {
            JSScript* script = JSFunction::getOrCreateScript(cx, fun);
            if (!script) {
                return false;
            }
            AutoRealm ar(cx, script);
            AutoKeepTypeScripts keepTypes(cx);
            if (!script->ensureHasTypes(cx, keepTypes)) {
                return false;
            }
            if (!js::CreateThis(cx, fun, script, newTarget, GenericObject, rval)) {
                return false;
            }
        }
=======
bool CreateThis(JSContext* cx, HandleObject callee, HandleObject newTarget,
                MutableHandleValue rval) {
  rval.set(MagicValue(JS_IS_CONSTRUCTING));

  if (callee->is<JSFunction>()) {
    RootedFunction fun(cx, &callee->as<JSFunction>());
    if (fun->isInterpreted() && fun->isConstructor()) {
      JSScript* script = JSFunction::getOrCreateScript(cx, fun);
      if (!script) {
        return false;
      }
      if (!js::CreateThis(cx, fun, script, newTarget, GenericObject, rval)) {
        return false;
      }
      MOZ_ASSERT_IF(rval.isObject(),
                    fun->realm() == rval.toObject().nonCCWRealm());
>>>>>>> upstream-releases
    }
  }

  return true;
}

<<<<<<< HEAD
bool GetDynamicNamePure(JSContext* cx, JSObject* envChain, JSString* str,
                        Value* vp) {
  // Lookup a string on the env chain, returning the value found through rval.
  // This function is infallible, and cannot GC or invalidate.
  // Returns false if the lookup could not be completed without GC.

  AutoUnsafeCallWithABI unsafe;
||||||| merged common ancestors
bool
GetDynamicNamePure(JSContext* cx, JSObject* envChain, JSString* str, Value* vp)
{
    // Lookup a string on the env chain, returning the value found through rval.
    // This function is infallible, and cannot GC or invalidate.
    // Returns false if the lookup could not be completed without GC.

    AutoUnsafeCallWithABI unsafe;
=======
bool GetDynamicNamePure(JSContext* cx, JSObject* envChain, JSString* str,
                        Value* vp) {
  // Lookup a string on the env chain, returning the value found through rval.
  // This function is infallible, and cannot GC or invalidate.
  // Returns false if the lookup could not be completed without GC.
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSAtom* atom;
  if (str->isAtom()) {
    atom = &str->asAtom();
  } else {
    atom = AtomizeString(cx, str);
    if (!atom) {
      cx->recoverFromOutOfMemory();
      return false;
||||||| merged common ancestors
    JSAtom* atom;
    if (str->isAtom()) {
        atom = &str->asAtom();
    } else {
        atom = AtomizeString(cx, str);
        if (!atom) {
            cx->recoverFromOutOfMemory();
            return false;
        }
    }

    if (!frontend::IsIdentifier(atom) || frontend::IsKeyword(atom)) {
        return false;
=======
  AutoUnsafeCallWithABI unsafe;

  JSAtom* atom;
  if (str->isAtom()) {
    atom = &str->asAtom();
  } else {
    atom = AtomizeString(cx, str);
    if (!atom) {
      cx->recoverFromOutOfMemory();
      return false;
>>>>>>> upstream-releases
    }
  }

  if (!frontend::IsIdentifier(atom) || frontend::IsKeyword(atom)) {
    return false;
  }

  PropertyResult prop;
  JSObject* scope = nullptr;
  JSObject* pobj = nullptr;
  if (LookupNameNoGC(cx, atom->asPropertyName(), envChain, &scope, &pobj,
                     &prop)) {
    return FetchNameNoGC(pobj, prop, vp);
  }
  return false;
}

void PostWriteBarrier(JSRuntime* rt, js::gc::Cell* cell) {
  AutoUnsafeCallWithABI unsafe;
  MOZ_ASSERT(!IsInsideNursery(cell));
  rt->gc.storeBuffer().putWholeCell(cell);
}

static const size_t MAX_WHOLE_CELL_BUFFER_SIZE = 4096;

template <IndexInBounds InBounds>
void PostWriteElementBarrier(JSRuntime* rt, JSObject* obj, int32_t index) {
  AutoUnsafeCallWithABI unsafe;

  MOZ_ASSERT(!IsInsideNursery(obj));

  if (InBounds == IndexInBounds::Yes) {
    MOZ_ASSERT(uint32_t(index) <
               obj->as<NativeObject>().getDenseInitializedLength());
  } else {
    if (MOZ_UNLIKELY(!obj->is<NativeObject>() || index < 0 ||
                     uint32_t(index) >=
                         NativeObject::MAX_DENSE_ELEMENTS_COUNT)) {
      rt->gc.storeBuffer().putWholeCell(obj);
      return;
    }
  }

  NativeObject* nobj = &obj->as<NativeObject>();
  if (nobj->isInWholeCellBuffer()) {
    return;
  }

  if (nobj->getDenseInitializedLength() > MAX_WHOLE_CELL_BUFFER_SIZE
#ifdef JS_GC_ZEAL
      || rt->hasZealMode(gc::ZealMode::ElementsBarrier)
#endif
  ) {
    rt->gc.storeBuffer().putSlot(nobj, HeapSlot::Element,
                                 nobj->unshiftedIndex(index), 1);
    return;
  }

  rt->gc.storeBuffer().putWholeCell(obj);
}

template void PostWriteElementBarrier<IndexInBounds::Yes>(JSRuntime* rt,
                                                          JSObject* obj,
                                                          int32_t index);

template void PostWriteElementBarrier<IndexInBounds::Maybe>(JSRuntime* rt,
                                                            JSObject* obj,
                                                            int32_t index);

void PostGlobalWriteBarrier(JSRuntime* rt, GlobalObject* obj) {
  MOZ_ASSERT(obj->JSObject::is<GlobalObject>());

  if (!obj->realm()->globalWriteBarriered) {
    PostWriteBarrier(rt, obj);
    obj->realm()->globalWriteBarriered = 1;
  }
}

int32_t GetIndexFromString(JSString* str) {
  // We shouldn't GC here as this is called directly from IC code.
  AutoUnsafeCallWithABI unsafe;

  if (!str->isFlat()) {
    return -1;
  }

  uint32_t index = UINT32_MAX;
  if (!str->asFlat().isIndex(&index) || index > INT32_MAX) {
    return -1;
  }

  return int32_t(index);
}

JSObject* WrapObjectPure(JSContext* cx, JSObject* obj) {
  // IC code calls this directly so we shouldn't GC.
  AutoUnsafeCallWithABI unsafe;

  MOZ_ASSERT(obj);
  MOZ_ASSERT(cx->compartment() != obj->compartment());

  // From: Compartment::getNonWrapperObjectForCurrentCompartment
  // Note that if the object is same-compartment, but has been wrapped into a
  // different compartment, we need to unwrap it and return the bare same-
  // compartment object. Note again that windows are always wrapped by a
  // WindowProxy even when same-compartment so take care not to strip this
  // particular wrapper.
  obj = UncheckedUnwrap(obj, /* stopAtWindowProxy = */ true);
  if (cx->compartment() == obj->compartment()) {
    MOZ_ASSERT(!IsWindow(obj));
    JS::ExposeObjectToActiveJS(obj);
    return obj;
  }

  // Try to Lookup an existing wrapper for this object. We assume that
  // if we can find such a wrapper, not calling preWrap is correct.
  if (WrapperMap::Ptr p = cx->compartment()->lookupWrapper(obj)) {
    JSObject* wrapped = &p->value().get().toObject();

    // Ensure the wrapper is still exposed.
    JS::ExposeObjectToActiveJS(wrapped);
    return wrapped;
  }

  return nullptr;
}

static bool HandlePrologueResumeMode(JSContext* cx, BaselineFrame* frame,
                                     jsbytecode* pc, bool* mustReturn,
                                     ResumeMode resumeMode) {
  *mustReturn = false;
  switch (resumeMode) {
    case ResumeMode::Continue:
      return true;

    case ResumeMode::Return:
      // The script is going to return immediately, so we have to call the
      // debug epilogue handler as well.
      MOZ_ASSERT(frame->hasReturnValue());
      *mustReturn = true;
      return jit::DebugEpilogue(cx, frame, pc, true);

    case ResumeMode::Throw:
    case ResumeMode::Terminate:
      return false;

    default:
      MOZ_CRASH("bad Debugger::onEnterFrame resume mode");
  }
}

bool DebugPrologue(JSContext* cx, BaselineFrame* frame, jsbytecode* pc,
                   bool* mustReturn) {
  ResumeMode resumeMode = Debugger::onEnterFrame(cx, frame);
  return HandlePrologueResumeMode(cx, frame, pc, mustReturn, resumeMode);
}

<<<<<<< HEAD
bool DebugEpilogueOnBaselineReturn(JSContext* cx, BaselineFrame* frame,
                                   jsbytecode* pc) {
  if (!DebugEpilogue(cx, frame, pc, true)) {
    // DebugEpilogue popped the frame by updating packedExitFP, so run the
    // stop event here before we enter the exception handler.
    TraceLoggerThread* logger = TraceLoggerForCurrentThread(cx);
    TraceLogStopEvent(logger, TraceLogger_Baseline);
    TraceLogStopEvent(logger, TraceLogger_Scripts);
    return false;
  }

  return true;
}

bool DebugEpilogue(JSContext* cx, BaselineFrame* frame, jsbytecode* pc,
                   bool ok) {
  // If Debugger::onLeaveFrame returns |true| we have to return the frame's
  // return value. If it returns |false|, the debugger threw an exception.
  // In both cases we have to pop debug scopes.
  ok = Debugger::onLeaveFrame(cx, frame, pc, ok);

  // Unwind to the outermost environment and set pc to the end of the
  // script, regardless of error.
  EnvironmentIter ei(cx, frame, pc);
  UnwindAllEnvironmentsInFrame(cx, ei);
  JSScript* script = frame->script();
  frame->setOverridePc(script->lastPC());

  if (!ok) {
    // Pop this frame by updating packedExitFP, so that the exception
    // handling code will start at the previous frame.
    JitFrameLayout* prefix = frame->framePrefix();
    EnsureBareExitFrame(cx->activation()->asJit(), prefix);
    return false;
  }

  // Clear the override pc. This is not necessary for correctness: the frame
  // will return immediately, but this simplifies the check we emit in debug
  // builds after each callVM, to ensure this flag is not set.
  frame->clearOverridePc();
  return true;
||||||| merged common ancestors
    // Clear the override pc. This is not necessary for correctness: the frame
    // will return immediately, but this simplifies the check we emit in debug
    // builds after each callVM, to ensure this flag is not set.
    frame->clearOverridePc();
    return true;
=======
bool DebugEpilogueOnBaselineReturn(JSContext* cx, BaselineFrame* frame,
                                   jsbytecode* pc) {
  if (!DebugEpilogue(cx, frame, pc, true)) {
    // DebugEpilogue popped the frame by updating packedExitFP, so run the
    // stop event here before we enter the exception handler.
    TraceLoggerThread* logger = TraceLoggerForCurrentThread(cx);
    TraceLogStopEvent(logger, TraceLogger_Baseline);
    TraceLogStopEvent(logger, TraceLogger_Scripts);
    return false;
  }

  return true;
}

bool DebugEpilogue(JSContext* cx, BaselineFrame* frame, jsbytecode* pc,
                   bool ok) {
  // If Debugger::onLeaveFrame returns |true| we have to return the frame's
  // return value. If it returns |false|, the debugger threw an exception.
  // In both cases we have to pop debug scopes.
  ok = Debugger::onLeaveFrame(cx, frame, pc, ok);

  // Unwind to the outermost environment and set pc to the end of the
  // script, regardless of error.
  EnvironmentIter ei(cx, frame, pc);
  UnwindAllEnvironmentsInFrame(cx, ei);
  JSScript* script = frame->script();
  frame->setOverridePc(script->offsetToPC(0));

  if (!ok) {
    // Pop this frame by updating packedExitFP, so that the exception
    // handling code will start at the previous frame.
    frame->deleteDebugModeOSRInfo();
    JitFrameLayout* prefix = frame->framePrefix();
    EnsureBareExitFrame(cx->activation()->asJit(), prefix);
    return false;
  }

  // Clear the override pc. This is not necessary for correctness: the frame
  // will return immediately, but this simplifies the check we emit in debug
  // builds after each callVM, to ensure this flag is not set.
  frame->clearOverridePc();
  return true;
>>>>>>> upstream-releases
}

void FrameIsDebuggeeCheck(BaselineFrame* frame) {
  AutoUnsafeCallWithABI unsafe;
  if (frame->script()->isDebuggee()) {
    frame->setIsDebuggee();
  }
}

<<<<<<< HEAD
JSObject* CreateGenerator(JSContext* cx, BaselineFrame* frame) {
  return GeneratorObject::create(cx, frame);
||||||| merged common ancestors
JSObject*
CreateGenerator(JSContext* cx, BaselineFrame* frame)
{
    return GeneratorObject::create(cx, frame);
=======
JSObject* CreateGenerator(JSContext* cx, BaselineFrame* frame) {
  return AbstractGeneratorObject::create(cx, frame);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool NormalSuspend(JSContext* cx, HandleObject obj, BaselineFrame* frame,
                   jsbytecode* pc, uint32_t stackDepth) {
  MOZ_ASSERT(*pc == JSOP_YIELD || *pc == JSOP_AWAIT);
||||||| merged common ancestors
bool
NormalSuspend(JSContext* cx, HandleObject obj, BaselineFrame* frame, jsbytecode* pc,
              uint32_t stackDepth)
{
    MOZ_ASSERT(*pc == JSOP_YIELD || *pc == JSOP_AWAIT);
=======
bool NormalSuspend(JSContext* cx, HandleObject obj, BaselineFrame* frame,
                   jsbytecode* pc) {
  MOZ_ASSERT(*pc == JSOP_YIELD || *pc == JSOP_AWAIT);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Return value is still on the stack.
  MOZ_ASSERT(stackDepth >= 1);
||||||| merged common ancestors
    // Return value is still on the stack.
    MOZ_ASSERT(stackDepth >= 1);
=======
  MOZ_ASSERT(frame->numValueSlots() > frame->script()->nfixed());
  uint32_t stackDepth = frame->numValueSlots() - frame->script()->nfixed();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The expression stack slots are stored on the stack in reverse order, so
  // we copy them to a Vector and pass a pointer to that instead. We use
  // stackDepth - 1 because we don't want to include the return value.
  AutoValueVector exprStack(cx);
  if (!exprStack.reserve(stackDepth - 1)) {
    return false;
  }
||||||| merged common ancestors
    // The expression stack slots are stored on the stack in reverse order, so
    // we copy them to a Vector and pass a pointer to that instead. We use
    // stackDepth - 1 because we don't want to include the return value.
    AutoValueVector exprStack(cx);
    if (!exprStack.reserve(stackDepth - 1)) {
        return false;
    }
=======
  // Return value is still on the stack.
  MOZ_ASSERT(stackDepth >= 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t firstSlot = frame->numValueSlots() - stackDepth;
  for (size_t i = 0; i < stackDepth - 1; i++) {
    exprStack.infallibleAppend(*frame->valueSlot(firstSlot + i));
  }
||||||| merged common ancestors
    size_t firstSlot = frame->numValueSlots() - stackDepth;
    for (size_t i = 0; i < stackDepth - 1; i++) {
        exprStack.infallibleAppend(*frame->valueSlot(firstSlot + i));
    }
=======
  // The expression stack slots are stored on the stack in reverse order, so
  // we copy them to a Vector and pass a pointer to that instead. We use
  // stackDepth - 1 because we don't want to include the return value.
  RootedValueVector exprStack(cx);
  if (!exprStack.reserve(stackDepth - 1)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(exprStack.length() == stackDepth - 1);
||||||| merged common ancestors
    MOZ_ASSERT(exprStack.length() == stackDepth - 1);
=======
  size_t firstSlot = frame->numValueSlots() - stackDepth;
  for (size_t i = 0; i < stackDepth - 1; i++) {
    exprStack.infallibleAppend(*frame->valueSlot(firstSlot + i));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return GeneratorObject::normalSuspend(cx, obj, frame, pc, exprStack.begin(),
                                        stackDepth - 1);
||||||| merged common ancestors
    return GeneratorObject::normalSuspend(cx, obj, frame, pc, exprStack.begin(), stackDepth - 1);
=======
  MOZ_ASSERT(exprStack.length() == stackDepth - 1);

  return AbstractGeneratorObject::normalSuspend(
      cx, obj, frame, pc, exprStack.begin(), stackDepth - 1);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool FinalSuspend(JSContext* cx, HandleObject obj, jsbytecode* pc) {
  MOZ_ASSERT(*pc == JSOP_FINALYIELDRVAL);
  GeneratorObject::finalSuspend(obj);
  return true;
||||||| merged common ancestors
bool
FinalSuspend(JSContext* cx, HandleObject obj, jsbytecode* pc)
{
    MOZ_ASSERT(*pc == JSOP_FINALYIELDRVAL);
    GeneratorObject::finalSuspend(obj);
    return true;
=======
bool FinalSuspend(JSContext* cx, HandleObject obj, jsbytecode* pc) {
  MOZ_ASSERT(*pc == JSOP_FINALYIELDRVAL);
  AbstractGeneratorObject::finalSuspend(obj);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool InterpretResume(JSContext* cx, HandleObject obj, HandleValue val,
                     HandlePropertyName kind, MutableHandleValue rval) {
  MOZ_ASSERT(obj->is<GeneratorObject>());
||||||| merged common ancestors
bool
InterpretResume(JSContext* cx, HandleObject obj, HandleValue val, HandlePropertyName kind,
                MutableHandleValue rval)
{
    MOZ_ASSERT(obj->is<GeneratorObject>());
=======
bool InterpretResume(JSContext* cx, HandleObject obj, HandleValue val,
                     HandlePropertyName kind, MutableHandleValue rval) {
  MOZ_ASSERT(obj->is<AbstractGeneratorObject>());
>>>>>>> upstream-releases

  FixedInvokeArgs<3> args(cx);

  args[0].setObject(*obj);
  args[1].set(val);
  args[2].setString(kind);

  return CallSelfHostedFunction(cx, cx->names().InterpretGeneratorResume,
                                UndefinedHandleValue, args, rval);
}

<<<<<<< HEAD
bool DebugAfterYield(JSContext* cx, BaselineFrame* frame, jsbytecode* pc,
                     bool* mustReturn) {
  // The BaselineFrame has just been constructed by JSOP_RESUME in the
  // caller. We need to set its debuggee flag as necessary.
  //
  // If a breakpoint is set on JSOP_DEBUGAFTERYIELD, or stepping is enabled,
  // we may already have done this work. Don't fire onEnterFrame again.
  if (frame->script()->isDebuggee() && !frame->isDebuggee()) {
    frame->setIsDebuggee();
    ResumeMode resumeMode = Debugger::onResumeFrame(cx, frame);
    return HandlePrologueResumeMode(cx, frame, pc, mustReturn, resumeMode);
  }
||||||| merged common ancestors
bool
DebugAfterYield(JSContext* cx, BaselineFrame* frame, jsbytecode* pc, bool* mustReturn)
{
    *mustReturn = false;
=======
bool DebugAfterYield(JSContext* cx, BaselineFrame* frame, jsbytecode* pc,
                     bool* mustReturn) {
  // The BaselineFrame has just been constructed by JSOP_RESUME in the
  // caller. We need to set its debuggee flag as necessary.
  //
  // If a breakpoint is set on JSOP_AFTERYIELD, or stepping is enabled,
  // we may already have done this work. Don't fire onEnterFrame again.
  if (frame->script()->isDebuggee() && !frame->isDebuggee()) {
    frame->setIsDebuggee();
    ResumeMode resumeMode = Debugger::onResumeFrame(cx, frame);
    return HandlePrologueResumeMode(cx, frame, pc, mustReturn, resumeMode);
  }
>>>>>>> upstream-releases

  *mustReturn = false;
  return true;
}

<<<<<<< HEAD
bool GeneratorThrowOrReturn(JSContext* cx, BaselineFrame* frame,
                            Handle<GeneratorObject*> genObj, HandleValue arg,
                            uint32_t resumeKind) {
  // Set the frame's pc to the current resume pc, so that frame iterators
  // work. This function always returns false, so we're guaranteed to enter
  // the exception handler where we will clear the pc.
  JSScript* script = frame->script();
  uint32_t offset = script->resumeOffsets()[genObj->resumeIndex()];
  jsbytecode* pc = script->offsetToPC(offset);
  frame->setOverridePc(pc);
||||||| merged common ancestors
bool
GeneratorThrowOrReturn(JSContext* cx, BaselineFrame* frame, Handle<GeneratorObject*> genObj,
                       HandleValue arg, uint32_t resumeKind)
{
    // Set the frame's pc to the current resume pc, so that frame iterators
    // work. This function always returns false, so we're guaranteed to enter
    // the exception handler where we will clear the pc.
    JSScript* script = frame->script();
    uint32_t offset = script->yieldAndAwaitOffsets()[genObj->yieldAndAwaitIndex()];
    jsbytecode* pc = script->offsetToPC(offset);
    frame->setOverridePc(pc);

    // In the interpreter, GeneratorObject::resume marks the generator as running,
    // so we do the same.
    genObj->setRunning();

    bool mustReturn = false;
    if (!DebugAfterYield(cx, frame, pc, &mustReturn)) {
        return false;
    }
    if (mustReturn) {
        resumeKind = GeneratorObject::RETURN;
    }
=======
bool GeneratorThrowOrReturn(JSContext* cx, BaselineFrame* frame,
                            Handle<AbstractGeneratorObject*> genObj,
                            HandleValue arg, uint32_t resumeKindArg) {
  // Set the frame's pc to the current resume pc, so that frame iterators
  // work. This function always returns false, so we're guaranteed to enter
  // the exception handler where we will clear the pc.
  JSScript* script = frame->script();
  uint32_t offset = script->resumeOffsets()[genObj->resumeIndex()];
  jsbytecode* pc = script->offsetToPC(offset);
  frame->setOverridePc(pc);

  // In the interpreter, AbstractGeneratorObject::resume marks the generator as
  // running, so we do the same.
  genObj->setRunning();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // In the interpreter, GeneratorObject::resume marks the generator as running,
  // so we do the same.
  genObj->setRunning();

  bool mustReturn = false;
  if (!DebugAfterYield(cx, frame, pc, &mustReturn)) {
||||||| merged common ancestors
    MOZ_ALWAYS_FALSE(js::GeneratorThrowOrReturn(cx, frame, genObj, arg, resumeKind));
=======
  bool mustReturn = false;
  if (!DebugAfterYield(cx, frame, pc, &mustReturn)) {
>>>>>>> upstream-releases
    return false;
<<<<<<< HEAD
  }
  if (mustReturn) {
    resumeKind = GeneratorObject::RETURN;
  }

  MOZ_ALWAYS_FALSE(
      js::GeneratorThrowOrReturn(cx, frame, genObj, arg, resumeKind));
  return false;
}
||||||| merged common ancestors
}
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool CheckGlobalOrEvalDeclarationConflicts(JSContext* cx,
                                           BaselineFrame* frame) {
  RootedScript script(cx, frame->script());
  RootedObject envChain(cx, frame->environmentChain());
  RootedObject varObj(cx, BindVar(cx, envChain));

  if (script->isForEval()) {
    // Strict eval and eval in parameter default expressions have their
    // own call objects.
    //
    // Non-strict eval may introduce 'var' bindings that conflict with
    // lexical bindings in an enclosing lexical scope.
    if (!script->bodyScope()->hasEnvironment()) {
      MOZ_ASSERT(
          !script->strict() &&
          (!script->enclosingScope()->is<FunctionScope>() ||
           !script->enclosingScope()->as<FunctionScope>().hasParameterExprs()));
      if (!CheckEvalDeclarationConflicts(cx, script, envChain, varObj)) {
        return false;
      }
    }
  } else {
    Rooted<LexicalEnvironmentObject*> lexicalEnv(
        cx, &NearestEnclosingExtensibleLexicalEnvironment(envChain));
    if (!CheckGlobalDeclarationConflicts(cx, script, lexicalEnv, varObj)) {
      return false;
    }
  }
||||||| merged common ancestors
bool
CheckGlobalOrEvalDeclarationConflicts(JSContext* cx, BaselineFrame* frame)
{
    RootedScript script(cx, frame->script());
    RootedObject envChain(cx, frame->environmentChain());
    RootedObject varObj(cx, BindVar(cx, envChain));

    if (script->isForEval()) {
        // Strict eval and eval in parameter default expressions have their
        // own call objects.
        //
        // Non-strict eval may introduce 'var' bindings that conflict with
        // lexical bindings in an enclosing lexical scope.
        if (!script->bodyScope()->hasEnvironment()) {
            MOZ_ASSERT(!script->strict() &&
                       (!script->enclosingScope()->is<FunctionScope>() ||
                        !script->enclosingScope()->as<FunctionScope>().hasParameterExprs()));
            if (!CheckEvalDeclarationConflicts(cx, script, envChain, varObj)) {
                return false;
            }
        }
    } else {
        Rooted<LexicalEnvironmentObject*> lexicalEnv(cx,
            &NearestEnclosingExtensibleLexicalEnvironment(envChain));
        if (!CheckGlobalDeclarationConflicts(cx, script, lexicalEnv, varObj)) {
            return false;
        }
    }
=======
  GeneratorResumeKind resumeKind = GeneratorResumeKind(resumeKindArg);
  if (mustReturn) {
    resumeKind = GeneratorResumeKind::Return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  MOZ_ALWAYS_FALSE(
      js::GeneratorThrowOrReturn(cx, frame, genObj, arg, resumeKind));
  return false;
>>>>>>> upstream-releases
}

bool GlobalNameConflictsCheckFromIon(JSContext* cx, HandleScript script) {
  Rooted<LexicalEnvironmentObject*> globalLexical(
      cx, &cx->global()->lexicalEnvironment());
  return CheckGlobalDeclarationConflicts(cx, script, globalLexical,
                                         cx->global());
}

bool InitFunctionEnvironmentObjects(JSContext* cx, BaselineFrame* frame) {
  return frame->initFunctionEnvironmentObjects(cx);
}

bool NewArgumentsObject(JSContext* cx, BaselineFrame* frame,
                        MutableHandleValue res) {
  ArgumentsObject* obj = ArgumentsObject::createExpected(cx, frame);
  if (!obj) {
    return false;
  }
  res.setObject(*obj);
  return true;
}

<<<<<<< HEAD
JSObject* CopyLexicalEnvironmentObject(JSContext* cx, HandleObject env,
                                       bool copySlots) {
  Handle<LexicalEnvironmentObject*> lexicalEnv =
      env.as<LexicalEnvironmentObject>();

  if (copySlots) {
    return LexicalEnvironmentObject::clone(cx, lexicalEnv);
  }
||||||| merged common ancestors
JSObject*
CopyLexicalEnvironmentObject(JSContext* cx, HandleObject env, bool copySlots)
{
    Handle<LexicalEnvironmentObject*> lexicalEnv = env.as<LexicalEnvironmentObject>();

    if (copySlots) {
        return LexicalEnvironmentObject::clone(cx, lexicalEnv);
    }
=======
JSObject* CopyLexicalEnvironmentObject(JSContext* cx, HandleObject env,
                                       bool copySlots) {
  Handle<LexicalEnvironmentObject*> lexicalEnv =
      env.as<LexicalEnvironmentObject>();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return LexicalEnvironmentObject::recreate(cx, lexicalEnv);
}
||||||| merged common ancestors
    return LexicalEnvironmentObject::recreate(cx, lexicalEnv);
}

JSObject*
InitRestParameter(JSContext* cx, uint32_t length, Value* rest, HandleObject templateObj,
                  HandleObject objRes)
{
    if (objRes) {
        Rooted<ArrayObject*> arrRes(cx, &objRes->as<ArrayObject>());

        MOZ_ASSERT(!arrRes->getDenseInitializedLength());
        MOZ_ASSERT(arrRes->group() == templateObj->group());

        // Fast path: we managed to allocate the array inline; initialize the
        // slots.
        if (length > 0) {
            if (!arrRes->ensureElements(cx, length)) {
                return nullptr;
            }
            arrRes->initDenseElements(rest, length);
            arrRes->setLengthInt32(length);
        }
        return arrRes;
    }
=======
  if (copySlots) {
    return LexicalEnvironmentObject::clone(cx, lexicalEnv);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JSObject* InitRestParameter(JSContext* cx, uint32_t length, Value* rest,
                            HandleObject templateObj, HandleObject objRes) {
  if (objRes) {
    Rooted<ArrayObject*> arrRes(cx, &objRes->as<ArrayObject>());

    MOZ_ASSERT(!arrRes->getDenseInitializedLength());
    MOZ_ASSERT(arrRes->group() == templateObj->group());

    // Fast path: we managed to allocate the array inline; initialize the
    // slots.
    if (length > 0) {
      if (!arrRes->ensureElements(cx, length)) {
        return nullptr;
      }
      arrRes->initDenseElements(rest, length);
      arrRes->setLengthInt32(length);
    }
    return arrRes;
  }

  NewObjectKind newKind;
  {
    AutoSweepObjectGroup sweep(templateObj->group());
    newKind = templateObj->group()->shouldPreTenure(sweep) ? TenuredObject
                                                           : GenericObject;
  }
  ArrayObject* arrRes = NewDenseCopiedArray(cx, length, rest, nullptr, newKind);
  if (arrRes) {
    arrRes->setGroup(templateObj->group());
  }
  return arrRes;
||||||| merged common ancestors
    NewObjectKind newKind;
    {
        AutoSweepObjectGroup sweep(templateObj->group());
        newKind = templateObj->group()->shouldPreTenure(sweep) ? TenuredObject : GenericObject;
    }
    ArrayObject* arrRes = NewDenseCopiedArray(cx, length, rest, nullptr, newKind);
    if (arrRes) {
        arrRes->setGroup(templateObj->group());
    }
    return arrRes;
=======
  return LexicalEnvironmentObject::recreate(cx, lexicalEnv);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool HandleDebugTrap(JSContext* cx, BaselineFrame* frame, uint8_t* retAddr,
                     bool* mustReturn) {
  *mustReturn = false;

  RootedScript script(cx, frame->script());
  jsbytecode* pc =
      script->baselineScript()->retAddrEntryFromReturnAddress(retAddr).pc(
          script);
||||||| merged common ancestors
bool
HandleDebugTrap(JSContext* cx, BaselineFrame* frame, uint8_t* retAddr, bool* mustReturn)
{
    *mustReturn = false;

    RootedScript script(cx, frame->script());
    jsbytecode* pc = script->baselineScript()->icEntryFromReturnAddress(retAddr).pc(script);
=======
JSObject* InitRestParameter(JSContext* cx, uint32_t length, Value* rest,
                            HandleObject templateObj, HandleObject objRes) {
  if (objRes) {
    Rooted<ArrayObject*> arrRes(cx, &objRes->as<ArrayObject>());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (*pc == JSOP_DEBUGAFTERYIELD) {
    // JSOP_DEBUGAFTERYIELD will set the frame's debuggee flag and call the
    // onEnterFrame handler, but if we set a breakpoint there we have to do
    // it now.
    MOZ_ASSERT(!frame->isDebuggee());
||||||| merged common ancestors
    if (*pc == JSOP_DEBUGAFTERYIELD) {
        // JSOP_DEBUGAFTERYIELD will set the frame's debuggee flag and call the
        // onEnterFrame handler, but if we set a breakpoint there we have to do
        // it now.
        MOZ_ASSERT(!frame->isDebuggee());
=======
    MOZ_ASSERT(!arrRes->getDenseInitializedLength());
    MOZ_ASSERT(arrRes->group() == templateObj->group());
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!DebugAfterYield(cx, frame, pc, mustReturn)) {
      return false;
    }
    if (*mustReturn) {
      return true;
||||||| merged common ancestors
        if (!DebugAfterYield(cx, frame, pc, mustReturn)) {
            return false;
        }
        if (*mustReturn) {
            return true;
        }
=======
    // Fast path: we managed to allocate the array inline; initialize the
    // slots.
    if (length > 0) {
      if (!arrRes->ensureElements(cx, length)) {
        return nullptr;
      }
      arrRes->initDenseElements(rest, length);
      arrRes->setLengthInt32(length);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

  MOZ_ASSERT(frame->isDebuggee());
  MOZ_ASSERT(script->stepModeEnabled() || script->hasBreakpointsAt(pc));
||||||| merged common ancestors

    MOZ_ASSERT(frame->isDebuggee());
    MOZ_ASSERT(script->stepModeEnabled() || script->hasBreakpointsAt(pc));
=======
    return arrRes;
  }

  NewObjectKind newKind;
  {
    AutoSweepObjectGroup sweep(templateObj->group());
    newKind = templateObj->group()->shouldPreTenure(sweep) ? TenuredObject
                                                           : GenericObject;
  }
  ArrayObject* arrRes = NewDenseCopiedArray(cx, length, rest, nullptr, newKind);
  if (arrRes) {
    arrRes->setGroup(templateObj->group());
  }
  return arrRes;
}

bool HandleDebugTrap(JSContext* cx, BaselineFrame* frame, uint8_t* retAddr,
                     bool* mustReturn) {
  *mustReturn = false;

  RootedScript script(cx, frame->script());
  jsbytecode* pc;
  if (frame->runningInInterpreter()) {
    pc = frame->interpreterPC();
  } else {
    BaselineScript* blScript = script->baselineScript();
    pc = blScript->retAddrEntryFromReturnAddress(retAddr).pc(script);
  }

  // The Baseline Interpreter calls HandleDebugTrap for every op when the script
  // is in step mode or has breakpoints. The Baseline Compiler can toggle
  // breakpoints more granularly for specific bytecode PCs.
  if (frame->runningInInterpreter()) {
    MOZ_ASSERT(script->hasAnyBreakpointsOrStepMode());
  } else {
    MOZ_ASSERT(script->stepModeEnabled() || script->hasBreakpointsAt(pc));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedValue rval(cx);
  ResumeMode resumeMode = ResumeMode::Continue;
||||||| merged common ancestors
    RootedValue rval(cx);
    ResumeMode resumeMode = ResumeMode::Continue;
=======
  if (*pc == JSOP_AFTERYIELD) {
    // JSOP_AFTERYIELD will set the frame's debuggee flag and call the
    // onEnterFrame handler, but if we set a breakpoint there we have to do
    // it now.
    MOZ_ASSERT(!frame->isDebuggee());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (script->stepModeEnabled()) {
    resumeMode = Debugger::onSingleStep(cx, &rval);
  }
||||||| merged common ancestors
    if (script->stepModeEnabled()) {
        resumeMode = Debugger::onSingleStep(cx, &rval);
    }
=======
    if (!DebugAfterYield(cx, frame, pc, mustReturn)) {
      return false;
    }
    if (*mustReturn) {
      return true;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (resumeMode == ResumeMode::Continue && script->hasBreakpointsAt(pc)) {
    resumeMode = Debugger::onTrap(cx, &rval);
  }
||||||| merged common ancestors
    if (resumeMode == ResumeMode::Continue && script->hasBreakpointsAt(pc)) {
        resumeMode = Debugger::onTrap(cx, &rval);
    }
=======
    // If the frame is not a debuggee we're done. This can happen, for instance,
    // if the onEnterFrame hook called removeDebuggee.
    if (!frame->isDebuggee()) {
      return true;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  switch (resumeMode) {
    case ResumeMode::Continue:
      break;
||||||| merged common ancestors
    switch (resumeMode) {
      case ResumeMode::Continue:
        break;
=======
  MOZ_ASSERT(frame->isDebuggee());
>>>>>>> upstream-releases

<<<<<<< HEAD
    case ResumeMode::Terminate:
      return false;
||||||| merged common ancestors
      case ResumeMode::Terminate:
        return false;
=======
  RootedValue rval(cx);
  ResumeMode resumeMode = ResumeMode::Continue;
>>>>>>> upstream-releases

<<<<<<< HEAD
    case ResumeMode::Return:
      *mustReturn = true;
      frame->setReturnValue(rval);
      return jit::DebugEpilogue(cx, frame, pc, true);
||||||| merged common ancestors
      case ResumeMode::Return:
        *mustReturn = true;
        frame->setReturnValue(rval);
        return jit::DebugEpilogue(cx, frame, pc, true);
=======
  if (script->stepModeEnabled()) {
    resumeMode = Debugger::onSingleStep(cx, &rval);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case ResumeMode::Throw:
      cx->setPendingException(rval);
      return false;
||||||| merged common ancestors
      case ResumeMode::Throw:
        cx->setPendingException(rval);
        return false;
=======
  if (resumeMode == ResumeMode::Continue && script->hasBreakpointsAt(pc)) {
    resumeMode = Debugger::onTrap(cx, &rval);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    default:
      MOZ_CRASH("Invalid step/breakpoint resume mode");
  }
||||||| merged common ancestors
      default:
        MOZ_CRASH("Invalid step/breakpoint resume mode");
    }
=======
  switch (resumeMode) {
    case ResumeMode::Continue:
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
    case ResumeMode::Terminate:
      return false;

    case ResumeMode::Return:
      *mustReturn = true;
      frame->setReturnValue(rval);
      return jit::DebugEpilogue(cx, frame, pc, true);

    case ResumeMode::Throw:
      cx->setPendingExceptionAndCaptureStack(rval);
      return false;

    default:
      MOZ_CRASH("Invalid step/breakpoint resume mode");
  }

  return true;
>>>>>>> upstream-releases
}

bool OnDebuggerStatement(JSContext* cx, BaselineFrame* frame, jsbytecode* pc,
                         bool* mustReturn) {
  *mustReturn = false;

  switch (Debugger::onDebuggerStatement(cx, frame)) {
    case ResumeMode::Continue:
      return true;

    case ResumeMode::Return:
      *mustReturn = true;
      return jit::DebugEpilogue(cx, frame, pc, true);

    case ResumeMode::Throw:
    case ResumeMode::Terminate:
      return false;

    default:
      MOZ_CRASH("Invalid OnDebuggerStatement resume mode");
  }
}

bool GlobalHasLiveOnDebuggerStatement(JSContext* cx) {
  AutoUnsafeCallWithABI unsafe;
  return cx->realm()->isDebuggee() &&
         Debugger::hasLiveHook(cx->global(), Debugger::OnDebuggerStatement);
}

bool PushLexicalEnv(JSContext* cx, BaselineFrame* frame,
                    Handle<LexicalScope*> scope) {
  return frame->pushLexicalEnvironment(cx, scope);
}

bool PopLexicalEnv(JSContext* cx, BaselineFrame* frame) {
  frame->popOffEnvironmentChain<LexicalEnvironmentObject>();
  return true;
}

bool DebugLeaveThenPopLexicalEnv(JSContext* cx, BaselineFrame* frame,
                                 jsbytecode* pc) {
  MOZ_ALWAYS_TRUE(DebugLeaveLexicalEnv(cx, frame, pc));
  frame->popOffEnvironmentChain<LexicalEnvironmentObject>();
  return true;
}

bool FreshenLexicalEnv(JSContext* cx, BaselineFrame* frame) {
  return frame->freshenLexicalEnvironment(cx);
}

bool DebugLeaveThenFreshenLexicalEnv(JSContext* cx, BaselineFrame* frame,
                                     jsbytecode* pc) {
  MOZ_ALWAYS_TRUE(DebugLeaveLexicalEnv(cx, frame, pc));
  return frame->freshenLexicalEnvironment(cx);
}

bool RecreateLexicalEnv(JSContext* cx, BaselineFrame* frame) {
  return frame->recreateLexicalEnvironment(cx);
}

bool DebugLeaveThenRecreateLexicalEnv(JSContext* cx, BaselineFrame* frame,
                                      jsbytecode* pc) {
  MOZ_ALWAYS_TRUE(DebugLeaveLexicalEnv(cx, frame, pc));
  return frame->recreateLexicalEnvironment(cx);
}

<<<<<<< HEAD
bool DebugLeaveLexicalEnv(JSContext* cx, BaselineFrame* frame, jsbytecode* pc) {
  MOZ_ASSERT(frame->script()->baselineScript()->hasDebugInstrumentation());
  if (cx->realm()->isDebuggee()) {
    DebugEnvironments::onPopLexical(cx, frame, pc);
  }
  return true;
||||||| merged common ancestors
bool
DebugLeaveLexicalEnv(JSContext* cx, BaselineFrame* frame, jsbytecode* pc)
{
    MOZ_ASSERT(frame->script()->baselineScript()->hasDebugInstrumentation());
    if (cx->realm()->isDebuggee()) {
        DebugEnvironments::onPopLexical(cx, frame, pc);
    }
    return true;
=======
bool DebugLeaveLexicalEnv(JSContext* cx, BaselineFrame* frame, jsbytecode* pc) {
  MOZ_ASSERT_IF(!frame->runningInInterpreter(),
                frame->script()->baselineScript()->hasDebugInstrumentation());
  if (cx->realm()->isDebuggee()) {
    DebugEnvironments::onPopLexical(cx, frame, pc);
  }
  return true;
>>>>>>> upstream-releases
}

bool PushVarEnv(JSContext* cx, BaselineFrame* frame, HandleScope scope) {
  return frame->pushVarEnvironment(cx, scope);
}

bool PopVarEnv(JSContext* cx, BaselineFrame* frame) {
  frame->popOffEnvironmentChain<VarEnvironmentObject>();
  return true;
}

bool EnterWith(JSContext* cx, BaselineFrame* frame, HandleValue val,
               Handle<WithScope*> templ) {
  return EnterWithOperation(cx, frame, val, templ);
}

bool LeaveWith(JSContext* cx, BaselineFrame* frame) {
  if (MOZ_UNLIKELY(frame->isDebuggee())) {
    DebugEnvironments::onPopWith(frame);
  }
  frame->popOffEnvironmentChain<WithEnvironmentObject>();
  return true;
}

bool InitBaselineFrameForOsr(BaselineFrame* frame,
                             InterpreterFrame* interpFrame,
                             uint32_t numStackValues) {
  return frame->initForOsr(interpFrame, numStackValues);
}

JSObject* CreateDerivedTypedObj(JSContext* cx, HandleObject descr,
                                HandleObject owner, int32_t offset) {
  MOZ_ASSERT(descr->is<TypeDescr>());
  MOZ_ASSERT(owner->is<TypedObject>());
  Rooted<TypeDescr*> descr1(cx, &descr->as<TypeDescr>());
  Rooted<TypedObject*> owner1(cx, &owner->as<TypedObject>());
  return OutlineTypedObject::createDerived(cx, descr1, owner1, offset);
}

JSString* StringReplace(JSContext* cx, HandleString string,
                        HandleString pattern, HandleString repl) {
  MOZ_ASSERT(string);
  MOZ_ASSERT(pattern);
  MOZ_ASSERT(repl);

  return str_replace_string_raw(cx, string, pattern, repl);
}

bool RecompileImpl(JSContext* cx, bool force) {
  MOZ_ASSERT(cx->currentlyRunningInJit());
  JitActivationIterator activations(cx);
  JSJitFrameIter frame(activations->asJit());

  MOZ_ASSERT(frame.type() == FrameType::Exit);
  ++frame;

  RootedScript script(cx, frame.script());
  MOZ_ASSERT(script->hasIonScript());

  if (!IsIonEnabled(cx)) {
    return true;
  }

  MethodStatus status = Recompile(cx, script, nullptr, nullptr, force);
  if (status == Method_Error) {
    return false;
  }

  return true;
}

<<<<<<< HEAD
bool ForcedRecompile(JSContext* cx) {
  return RecompileImpl(cx, /* force = */ true);
||||||| merged common ancestors
bool
ForcedRecompile(JSContext* cx)
{
    return RecompileImpl(cx, /* force = */ true);
=======
bool IonForcedRecompile(JSContext* cx) {
  return RecompileImpl(cx, /* force = */ true);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool Recompile(JSContext* cx) { return RecompileImpl(cx, /* force = */ false); }
||||||| merged common ancestors
bool
Recompile(JSContext* cx)
{
    return RecompileImpl(cx, /* force = */ false);
}
=======
bool IonRecompile(JSContext* cx) {
  return RecompileImpl(cx, /* force = */ false);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool SetDenseElement(JSContext* cx, HandleNativeObject obj, int32_t index,
                     HandleValue value, bool strict) {
  // This function is called from Ion code for StoreElementHole's OOL path.
  // In this case we know the object is native and that no type changes are
  // needed.
||||||| merged common ancestors
bool
SetDenseElement(JSContext* cx, HandleNativeObject obj, int32_t index, HandleValue value,
                bool strict)
{
    // This function is called from Ion code for StoreElementHole's OOL path.
    // In this case we know the object is native and that no type changes are
    // needed.
=======
bool IonForcedInvalidation(JSContext* cx) {
  MOZ_ASSERT(cx->currentlyRunningInJit());
  JitActivationIterator activations(cx);
  JSJitFrameIter frame(activations->asJit());
>>>>>>> upstream-releases

<<<<<<< HEAD
  DenseElementResult result = obj->setOrExtendDenseElements(
      cx, index, value.address(), 1, ShouldUpdateTypes::DontUpdate);
  if (result != DenseElementResult::Incomplete) {
    return result == DenseElementResult::Success;
  }
||||||| merged common ancestors
    DenseElementResult result = obj->setOrExtendDenseElements(cx, index, value.address(), 1,
                                                              ShouldUpdateTypes::DontUpdate);
    if (result != DenseElementResult::Incomplete) {
        return result == DenseElementResult::Success;
    }
=======
  MOZ_ASSERT(frame.type() == FrameType::Exit);
  ++frame;
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedValue indexVal(cx, Int32Value(index));
  return SetObjectElement(cx, obj, indexVal, value, strict);
||||||| merged common ancestors
    RootedValue indexVal(cx, Int32Value(index));
    return SetObjectElement(cx, obj, indexVal, value, strict);
=======
  RootedScript script(cx, frame.script());
  MOZ_ASSERT(script->hasIonScript());

  if (script->baselineScript()->hasPendingIonBuilder()) {
    LinkIonScript(cx, script);
    return true;
  }

  Invalidate(cx, script, /* resetUses = */ false,
             /* cancelOffThread = */ false);
  return true;
}

bool SetDenseElement(JSContext* cx, HandleNativeObject obj, int32_t index,
                     HandleValue value, bool strict) {
  // This function is called from Ion code for StoreElementHole's OOL path.
  // In this case we know the object is native and that no type changes are
  // needed.

  DenseElementResult result = obj->setOrExtendDenseElements(
      cx, index, value.address(), 1, ShouldUpdateTypes::DontUpdate);
  if (result != DenseElementResult::Incomplete) {
    return result == DenseElementResult::Success;
  }

  RootedValue indexVal(cx, Int32Value(index));
  return SetObjectElement(cx, obj, indexVal, value, strict);
>>>>>>> upstream-releases
}

void AutoDetectInvalidation::setReturnOverride() {
  cx_->setIonReturnOverride(rval_.get());
}

void AssertValidObjectPtr(JSContext* cx, JSObject* obj) {
  AutoUnsafeCallWithABI unsafe;
#ifdef DEBUG
<<<<<<< HEAD
  // Check what we can, so that we'll hopefully assert/crash if we get a
  // bogus object (pointer).
  MOZ_ASSERT(obj->compartment() == cx->compartment());
  MOZ_ASSERT(obj->zoneFromAnyThread() == cx->zone());
  MOZ_ASSERT(obj->runtimeFromMainThread() == cx->runtime());

  MOZ_ASSERT_IF(!obj->hasLazyGroup() && obj->maybeShape(),
                obj->group()->clasp() == obj->maybeShape()->getObjectClass());

  if (obj->isTenured()) {
    MOZ_ASSERT(obj->isAligned());
    gc::AllocKind kind = obj->asTenured().getAllocKind();
    MOZ_ASSERT(gc::IsObjectAllocKind(kind));
  }
||||||| merged common ancestors
    // Check what we can, so that we'll hopefully assert/crash if we get a
    // bogus object (pointer).
    MOZ_ASSERT(obj->compartment() == cx->compartment());
    MOZ_ASSERT(obj->zoneFromAnyThread() == cx->zone());
    MOZ_ASSERT(obj->runtimeFromMainThread() == cx->runtime());

    MOZ_ASSERT_IF(!obj->hasLazyGroup() && obj->maybeShape(),
                  obj->group()->clasp() == obj->maybeShape()->getObjectClass());

    if (obj->isTenured()) {
        MOZ_ASSERT(obj->isAligned());
        gc::AllocKind kind = obj->asTenured().getAllocKind();
        MOZ_ASSERT(gc::IsObjectAllocKind(kind));
    }
=======
  // Check what we can, so that we'll hopefully assert/crash if we get a
  // bogus object (pointer).
  MOZ_ASSERT(obj->compartment() == cx->compartment());
  MOZ_ASSERT(obj->zoneFromAnyThread() == cx->zone());
  MOZ_ASSERT(obj->runtimeFromMainThread() == cx->runtime());

  MOZ_ASSERT_IF(!obj->hasLazyGroup(),
                obj->group()->clasp() == obj->shape()->getObjectClass());

  if (obj->isTenured()) {
    MOZ_ASSERT(obj->isAligned());
    gc::AllocKind kind = obj->asTenured().getAllocKind();
    MOZ_ASSERT(gc::IsObjectAllocKind(kind));
  }
>>>>>>> upstream-releases
#endif
}

void AssertValidObjectOrNullPtr(JSContext* cx, JSObject* obj) {
  AutoUnsafeCallWithABI unsafe;
  if (obj) {
    AssertValidObjectPtr(cx, obj);
  }
}

void AssertValidStringPtr(JSContext* cx, JSString* str) {
  AutoUnsafeCallWithABI unsafe;
#ifdef DEBUG
  // We can't closely inspect strings from another runtime.
  if (str->runtimeFromAnyThread() != cx->runtime()) {
    MOZ_ASSERT(str->isPermanentAtom());
    return;
  }

  if (str->isAtom()) {
    MOZ_ASSERT(str->zone()->isAtomsZone());
  } else {
    MOZ_ASSERT(str->zone() == cx->zone());
  }

  MOZ_ASSERT(str->isAligned());
  MOZ_ASSERT(str->length() <= JSString::MAX_LENGTH);

  gc::AllocKind kind = str->getAllocKind();
  if (str->isFatInline()) {
    MOZ_ASSERT(kind == gc::AllocKind::FAT_INLINE_STRING ||
               kind == gc::AllocKind::FAT_INLINE_ATOM);
  } else if (str->isExternal()) {
    MOZ_ASSERT(kind == gc::AllocKind::EXTERNAL_STRING);
  } else if (str->isAtom()) {
    MOZ_ASSERT(kind == gc::AllocKind::ATOM);
  } else if (str->isFlat()) {
    MOZ_ASSERT(kind == gc::AllocKind::STRING ||
               kind == gc::AllocKind::FAT_INLINE_STRING ||
               kind == gc::AllocKind::EXTERNAL_STRING);
  } else {
    MOZ_ASSERT(kind == gc::AllocKind::STRING);
  }
#endif
}

void AssertValidSymbolPtr(JSContext* cx, JS::Symbol* sym) {
  AutoUnsafeCallWithABI unsafe;

  // We can't closely inspect symbols from another runtime.
  if (sym->runtimeFromAnyThread() != cx->runtime()) {
    MOZ_ASSERT(sym->isWellKnownSymbol());
    return;
  }

  MOZ_ASSERT(sym->zone()->isAtomsZone());
  MOZ_ASSERT(sym->isAligned());
  if (JSString* desc = sym->description()) {
    MOZ_ASSERT(desc->isAtom());
    AssertValidStringPtr(cx, desc);
  }

  MOZ_ASSERT(sym->getAllocKind() == gc::AllocKind::SYMBOL);
}

<<<<<<< HEAD
void AssertValidValue(JSContext* cx, Value* v) {
  AutoUnsafeCallWithABI unsafe;
  if (v->isObject()) {
    AssertValidObjectPtr(cx, &v->toObject());
  } else if (v->isString()) {
    AssertValidStringPtr(cx, v->toString());
  } else if (v->isSymbol()) {
    AssertValidSymbolPtr(cx, v->toSymbol());
  }
||||||| merged common ancestors
void
AssertValidValue(JSContext* cx, Value* v)
{
    AutoUnsafeCallWithABI unsafe;
    if (v->isObject()) {
        AssertValidObjectPtr(cx, &v->toObject());
    } else if (v->isString()) {
        AssertValidStringPtr(cx, v->toString());
    } else if (v->isSymbol()) {
        AssertValidSymbolPtr(cx, v->toSymbol());
    }
=======
void AssertValidBigIntPtr(JSContext* cx, JS::BigInt* bi) {
  AutoUnsafeCallWithABI unsafe;
  // FIXME: check runtime?
  MOZ_ASSERT(cx->zone() == bi->zone());
  MOZ_ASSERT(bi->isAligned());
  MOZ_ASSERT(bi->isTenured());
  MOZ_ASSERT(bi->getAllocKind() == gc::AllocKind::BIGINT);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool ObjectIsCallable(JSObject* obj) {
  AutoUnsafeCallWithABI unsafe;
  return obj->isCallable();
||||||| merged common ancestors
bool
ObjectIsCallable(JSObject* obj)
{
    AutoUnsafeCallWithABI unsafe;
    return obj->isCallable();
=======
void AssertValidValue(JSContext* cx, Value* v) {
  AutoUnsafeCallWithABI unsafe;
  if (v->isObject()) {
    AssertValidObjectPtr(cx, &v->toObject());
  } else if (v->isString()) {
    AssertValidStringPtr(cx, v->toString());
  } else if (v->isSymbol()) {
    AssertValidSymbolPtr(cx, v->toSymbol());
  } else if (v->isBigInt()) {
    AssertValidBigIntPtr(cx, v->toBigInt());
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool ObjectIsConstructor(JSObject* obj) {
  AutoUnsafeCallWithABI unsafe;
  return obj->isConstructor();
||||||| merged common ancestors
bool
ObjectIsConstructor(JSObject* obj)
{
    AutoUnsafeCallWithABI unsafe;
    return obj->isConstructor();
=======
bool ObjectIsCallable(JSObject* obj) {
  AutoUnsafeCallWithABI unsafe;
  return obj->isCallable();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void MarkValueFromJit(JSRuntime* rt, Value* vp) {
  AutoUnsafeCallWithABI unsafe;
  TraceManuallyBarrieredEdge(&rt->gc.marker, vp, "write barrier");
||||||| merged common ancestors
void
MarkValueFromJit(JSRuntime* rt, Value* vp)
{
    AutoUnsafeCallWithABI unsafe;
    TraceManuallyBarrieredEdge(&rt->gc.marker, vp, "write barrier");
=======
bool ObjectIsConstructor(JSObject* obj) {
  AutoUnsafeCallWithABI unsafe;
  return obj->isConstructor();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void MarkStringFromJit(JSRuntime* rt, JSString** stringp) {
  AutoUnsafeCallWithABI unsafe;
  MOZ_ASSERT(*stringp);
  TraceManuallyBarrieredEdge(&rt->gc.marker, stringp, "write barrier");
||||||| merged common ancestors
void
MarkStringFromJit(JSRuntime* rt, JSString** stringp)
{
    AutoUnsafeCallWithABI unsafe;
    MOZ_ASSERT(*stringp);
    TraceManuallyBarrieredEdge(&rt->gc.marker, stringp, "write barrier");
=======
void MarkValueFromJit(JSRuntime* rt, Value* vp) {
  AutoUnsafeCallWithABI unsafe;
  TraceManuallyBarrieredEdge(&rt->gc.marker, vp, "write barrier");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void MarkObjectFromJit(JSRuntime* rt, JSObject** objp) {
  AutoUnsafeCallWithABI unsafe;
  MOZ_ASSERT(*objp);
  TraceManuallyBarrieredEdge(&rt->gc.marker, objp, "write barrier");
||||||| merged common ancestors
void
MarkObjectFromJit(JSRuntime* rt, JSObject** objp)
{
    AutoUnsafeCallWithABI unsafe;
    MOZ_ASSERT(*objp);
    TraceManuallyBarrieredEdge(&rt->gc.marker, objp, "write barrier");
=======
void MarkStringFromJit(JSRuntime* rt, JSString** stringp) {
  AutoUnsafeCallWithABI unsafe;
  MOZ_ASSERT(*stringp);
  TraceManuallyBarrieredEdge(&rt->gc.marker, stringp, "write barrier");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void MarkShapeFromJit(JSRuntime* rt, Shape** shapep) {
  AutoUnsafeCallWithABI unsafe;
  TraceManuallyBarrieredEdge(&rt->gc.marker, shapep, "write barrier");
||||||| merged common ancestors
void
MarkShapeFromJit(JSRuntime* rt, Shape** shapep)
{
    AutoUnsafeCallWithABI unsafe;
    TraceManuallyBarrieredEdge(&rt->gc.marker, shapep, "write barrier");
=======
void MarkObjectFromJit(JSRuntime* rt, JSObject** objp) {
  AutoUnsafeCallWithABI unsafe;
  MOZ_ASSERT(*objp);
  TraceManuallyBarrieredEdge(&rt->gc.marker, objp, "write barrier");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void MarkObjectGroupFromJit(JSRuntime* rt, ObjectGroup** groupp) {
  AutoUnsafeCallWithABI unsafe;
  TraceManuallyBarrieredEdge(&rt->gc.marker, groupp, "write barrier");
||||||| merged common ancestors
void
MarkObjectGroupFromJit(JSRuntime* rt, ObjectGroup** groupp)
{
    AutoUnsafeCallWithABI unsafe;
    TraceManuallyBarrieredEdge(&rt->gc.marker, groupp, "write barrier");
=======
void MarkShapeFromJit(JSRuntime* rt, Shape** shapep) {
  AutoUnsafeCallWithABI unsafe;
  TraceManuallyBarrieredEdge(&rt->gc.marker, shapep, "write barrier");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool ThrowRuntimeLexicalError(JSContext* cx, unsigned errorNumber) {
  ScriptFrameIter iter(cx);
  RootedScript script(cx, iter.script());
  ReportRuntimeLexicalError(cx, errorNumber, script, iter.pc());
  return false;
||||||| merged common ancestors
bool
ThrowRuntimeLexicalError(JSContext* cx, unsigned errorNumber)
{
    ScriptFrameIter iter(cx);
    RootedScript script(cx, iter.script());
    ReportRuntimeLexicalError(cx, errorNumber, script, iter.pc());
    return false;
=======
void MarkObjectGroupFromJit(JSRuntime* rt, ObjectGroup** groupp) {
  AutoUnsafeCallWithABI unsafe;
  TraceManuallyBarrieredEdge(&rt->gc.marker, groupp, "write barrier");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool ThrowBadDerivedReturn(JSContext* cx, HandleValue v) {
  ReportValueError(cx, JSMSG_BAD_DERIVED_RETURN, JSDVG_IGNORE_STACK, v,
                   nullptr);
  return false;
||||||| merged common ancestors
bool
ThrowBadDerivedReturn(JSContext* cx, HandleValue v)
{
    ReportValueError(cx, JSMSG_BAD_DERIVED_RETURN, JSDVG_IGNORE_STACK, v, nullptr);
    return false;
=======
bool ThrowRuntimeLexicalError(JSContext* cx, unsigned errorNumber) {
  ScriptFrameIter iter(cx);
  RootedScript script(cx, iter.script());
  ReportRuntimeLexicalError(cx, errorNumber, script, iter.pc());
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineThrowUninitializedThis(JSContext* cx, BaselineFrame* frame) {
  return ThrowUninitializedThis(cx, frame);
||||||| merged common ancestors
bool
BaselineThrowUninitializedThis(JSContext* cx, BaselineFrame* frame)
{
    return ThrowUninitializedThis(cx, frame);
=======
bool ThrowBadDerivedReturn(JSContext* cx, HandleValue v) {
  ReportValueError(cx, JSMSG_BAD_DERIVED_RETURN, JSDVG_IGNORE_STACK, v,
                   nullptr);
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineThrowInitializedThis(JSContext* cx) {
  return ThrowInitializedThis(cx);
||||||| merged common ancestors
bool
BaselineThrowInitializedThis(JSContext* cx)
{
    return ThrowInitializedThis(cx);
=======
bool BaselineThrowUninitializedThis(JSContext* cx, BaselineFrame* frame) {
  return ThrowUninitializedThis(cx, frame);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool ThrowObjectCoercible(JSContext* cx, HandleValue v) {
  MOZ_ASSERT(v.isUndefined() || v.isNull());
  MOZ_ALWAYS_FALSE(ToObjectSlow(cx, v, true));
  return false;
||||||| merged common ancestors

bool
ThrowObjectCoercible(JSContext* cx, HandleValue v)
{
    MOZ_ASSERT(v.isUndefined() || v.isNull());
    MOZ_ALWAYS_FALSE(ToObjectSlow(cx, v, true));
    return false;
=======
bool BaselineThrowInitializedThis(JSContext* cx) {
  return ThrowInitializedThis(cx);
}

bool ThrowObjectCoercible(JSContext* cx, HandleValue v) {
  MOZ_ASSERT(v.isUndefined() || v.isNull());
  MOZ_ALWAYS_FALSE(ToObjectSlow(cx, v, true));
  return false;
>>>>>>> upstream-releases
}

bool BaselineGetFunctionThis(JSContext* cx, BaselineFrame* frame,
                             MutableHandleValue res) {
  return GetFunctionThis(cx, frame, res);
}

bool CallNativeGetter(JSContext* cx, HandleFunction callee, HandleObject obj,
                      MutableHandleValue result) {
  AutoRealm ar(cx, callee);

  MOZ_ASSERT(callee->isNative());
  JSNative natfun = callee->native();

  JS::AutoValueArray<2> vp(cx);
  vp[0].setObject(*callee.get());
  vp[1].setObject(*obj.get());

  if (!natfun(cx, 0, vp.begin())) {
    return false;
  }

  result.set(vp[0]);
  return true;
}

<<<<<<< HEAD
bool CallNativeSetter(JSContext* cx, HandleFunction callee, HandleObject obj,
                      HandleValue rhs) {
  AutoRealm ar(cx, callee);
||||||| merged common ancestors
bool
CallNativeSetter(JSContext* cx, HandleFunction callee, HandleObject obj, HandleValue rhs)
{
    AutoRealm ar(cx, callee);
=======
bool CallNativeGetterByValue(JSContext* cx, HandleFunction callee,
                             HandleValue receiver, MutableHandleValue result) {
  AutoRealm ar(cx, callee);
>>>>>>> upstream-releases

  MOZ_ASSERT(callee->isNative());
  JSNative natfun = callee->native();

<<<<<<< HEAD
  JS::AutoValueArray<3> vp(cx);
  vp[0].setObject(*callee.get());
  vp[1].setObject(*obj.get());
  vp[2].set(rhs);
||||||| merged common ancestors
    JS::AutoValueArray<3> vp(cx);
    vp[0].setObject(*callee.get());
    vp[1].setObject(*obj.get());
    vp[2].set(rhs);
=======
  JS::AutoValueArray<2> vp(cx);
  vp[0].setObject(*callee.get());
  vp[1].set(receiver);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return natfun(cx, 1, vp.begin());
||||||| merged common ancestors
    return natfun(cx, 1, vp.begin());
=======
  if (!natfun(cx, 0, vp.begin())) {
    return false;
  }

  result.set(vp[0]);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool EqualStringsHelperPure(JSString* str1, JSString* str2) {
  // IC code calls this directly so we shouldn't GC.
  AutoUnsafeCallWithABI unsafe;
||||||| merged common ancestors
bool
EqualStringsHelperPure(JSString* str1, JSString* str2)
{
    // IC code calls this directly so we shouldn't GC.
    AutoUnsafeCallWithABI unsafe;
=======
bool CallNativeSetter(JSContext* cx, HandleFunction callee, HandleObject obj,
                      HandleValue rhs) {
  AutoRealm ar(cx, callee);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(str1->isAtom());
  MOZ_ASSERT(!str2->isAtom());
  MOZ_ASSERT(str1->length() == str2->length());
||||||| merged common ancestors
    MOZ_ASSERT(str1->isAtom());
    MOZ_ASSERT(!str2->isAtom());
    MOZ_ASSERT(str1->length() == str2->length());
=======
  MOZ_ASSERT(callee->isNative());
  JSNative natfun = callee->native();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // ensureLinear is intentionally called with a nullptr to avoid OOM
  // reporting; if it fails, we will continue to the next stub.
  JSLinearString* str2Linear = str2->ensureLinear(nullptr);
  if (!str2Linear) {
    return false;
  }
||||||| merged common ancestors
    // ensureLinear is intentionally called with a nullptr to avoid OOM
    // reporting; if it fails, we will continue to the next stub.
    JSLinearString* str2Linear = str2->ensureLinear(nullptr);
    if (!str2Linear) {
        return false;
    }
=======
  JS::AutoValueArray<3> vp(cx);
  vp[0].setObject(*callee.get());
  vp[1].setObject(*obj.get());
  vp[2].set(rhs);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return EqualChars(&str1->asLinear(), str2Linear);
||||||| merged common ancestors
    return EqualChars(&str1->asLinear(), str2Linear);
=======
  return natfun(cx, 1, vp.begin());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool CheckIsCallable(JSContext* cx, HandleValue v, CheckIsCallableKind kind) {
  if (!IsCallable(v)) {
    return ThrowCheckIsCallable(cx, kind);
  }
||||||| merged common ancestors
bool
CheckIsCallable(JSContext* cx, HandleValue v, CheckIsCallableKind kind)
{
    if (!IsCallable(v)) {
        return ThrowCheckIsCallable(cx, kind);
    }
=======
bool EqualStringsHelperPure(JSString* str1, JSString* str2) {
  // IC code calls this directly so we shouldn't GC.
  AutoUnsafeCallWithABI unsafe;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  MOZ_ASSERT(str1->isAtom());
  MOZ_ASSERT(!str2->isAtom());
  MOZ_ASSERT(str1->length() == str2->length());

  // ensureLinear is intentionally called with a nullptr to avoid OOM
  // reporting; if it fails, we will continue to the next stub.
  JSLinearString* str2Linear = str2->ensureLinear(nullptr);
  if (!str2Linear) {
    return false;
  }

  return EqualChars(&str1->asLinear(), str2Linear);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <bool HandleMissing>
static MOZ_ALWAYS_INLINE bool GetNativeDataPropertyPure(JSContext* cx,
                                                        NativeObject* obj,
                                                        jsid id, Value* vp) {
  // Fast path used by megamorphic IC stubs. Unlike our other property
  // lookup paths, this is optimized to be as fast as possible for simple
  // data property lookups.
||||||| merged common ancestors
template <bool HandleMissing>
static MOZ_ALWAYS_INLINE bool
GetNativeDataPropertyPure(JSContext* cx, NativeObject* obj, jsid id, Value* vp)
{
    // Fast path used by megamorphic IC stubs. Unlike our other property
    // lookup paths, this is optimized to be as fast as possible for simple
    // data property lookups.

    AutoUnsafeCallWithABI unsafe;

    MOZ_ASSERT(JSID_IS_ATOM(id) || JSID_IS_SYMBOL(id));

    while (true) {
        if (Shape* shape = obj->lastProperty()->search(cx, id)) {
            if (!shape->isDataProperty()) {
                return false;
            }

            *vp = obj->getSlot(shape->slot());
            return true;
        }

        // Property not found. Watch out for Class hooks.
        if (MOZ_UNLIKELY(!obj->is<PlainObject>())) {
            if (ClassMayResolveId(cx->names(), obj->getClass(), id, obj)) {
                return false;
            }
        }

        JSObject* proto = obj->staticPrototype();
        if (!proto) {
            if (HandleMissing) {
                vp->setUndefined();
                return true;
            }
            return false;
        }

        if (!proto->isNative()) {
            return false;
        }
        obj = &proto->as<NativeObject>();
    }
}
=======
bool CheckIsCallable(JSContext* cx, HandleValue v, CheckIsCallableKind kind) {
  if (!IsCallable(v)) {
    return ThrowCheckIsCallable(cx, kind);
  }

  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoUnsafeCallWithABI unsafe;

  MOZ_ASSERT(JSID_IS_ATOM(id) || JSID_IS_SYMBOL(id));

  while (true) {
    if (Shape* shape = obj->lastProperty()->search(cx, id)) {
      if (!shape->isDataProperty()) {
        return false;
      }

      *vp = obj->getSlot(shape->slot());
      return true;
||||||| merged common ancestors
template <bool HandleMissing>
bool
GetNativeDataPropertyPure(JSContext* cx, JSObject* obj, PropertyName* name, Value* vp)
{
    // Condition checked by caller.
    MOZ_ASSERT(obj->isNative());
    return GetNativeDataPropertyPure<HandleMissing>(cx, &obj->as<NativeObject>(), NameToId(name), vp);
}

template bool
GetNativeDataPropertyPure<true>(JSContext* cx, JSObject* obj, PropertyName* name, Value* vp);

template bool
GetNativeDataPropertyPure<false>(JSContext* cx, JSObject* obj, PropertyName* name, Value* vp);

static MOZ_ALWAYS_INLINE bool
ValueToAtomOrSymbolPure(JSContext* cx, Value& idVal, jsid* id)
{
    if (MOZ_LIKELY(idVal.isString())) {
        JSString* s = idVal.toString();
        JSAtom* atom;
        if (s->isAtom()) {
            atom = &s->asAtom();
        } else {
            atom = AtomizeString(cx, s);
            if (!atom) {
                cx->recoverFromOutOfMemory();
                return false;
            }
        }
        *id = AtomToId(atom);
    } else if (idVal.isSymbol()) {
        *id = SYMBOL_TO_JSID(idVal.toSymbol());
    } else {
        if (!ValueToIdPure(idVal, id)) {
            return false;
        }
=======
template <bool HandleMissing>
static MOZ_ALWAYS_INLINE bool GetNativeDataPropertyPure(JSContext* cx,
                                                        NativeObject* obj,
                                                        jsid id, Value* vp) {
  // Fast path used by megamorphic IC stubs. Unlike our other property
  // lookup paths, this is optimized to be as fast as possible for simple
  // data property lookups.

  AutoUnsafeCallWithABI unsafe;

  MOZ_ASSERT(JSID_IS_ATOM(id) || JSID_IS_SYMBOL(id));

  while (true) {
    if (Shape* shape = obj->lastProperty()->search(cx, id)) {
      if (!shape->isDataProperty()) {
        return false;
      }

      *vp = obj->getSlot(shape->slot());
      return true;
>>>>>>> upstream-releases
    }

    // Property not found. Watch out for Class hooks.
    if (MOZ_UNLIKELY(!obj->is<PlainObject>())) {
      if (ClassMayResolveId(cx->names(), obj->getClass(), id, obj)) {
        return false;
      }
    }

    JSObject* proto = obj->staticPrototype();
    if (!proto) {
      if (HandleMissing) {
        vp->setUndefined();
        return true;
      }
      return false;
    }

    if (!proto->isNative()) {
      return false;
    }
    obj = &proto->as<NativeObject>();
  }
}

template <bool HandleMissing>
bool GetNativeDataPropertyPure(JSContext* cx, JSObject* obj, PropertyName* name,
                               Value* vp) {
  // Condition checked by caller.
  MOZ_ASSERT(obj->isNative());
  return GetNativeDataPropertyPure<HandleMissing>(cx, &obj->as<NativeObject>(),
                                                  NameToId(name), vp);
}

template bool GetNativeDataPropertyPure<true>(JSContext* cx, JSObject* obj,
                                              PropertyName* name, Value* vp);

template bool GetNativeDataPropertyPure<false>(JSContext* cx, JSObject* obj,
                                               PropertyName* name, Value* vp);

static MOZ_ALWAYS_INLINE bool ValueToAtomOrSymbolPure(JSContext* cx,
                                                      Value& idVal, jsid* id) {
  if (MOZ_LIKELY(idVal.isString())) {
    JSString* s = idVal.toString();
    JSAtom* atom;
    if (s->isAtom()) {
      atom = &s->asAtom();
    } else {
      atom = AtomizeString(cx, s);
      if (!atom) {
        cx->recoverFromOutOfMemory();
        return false;
      }
    }
    *id = AtomToId(atom);
  } else if (idVal.isSymbol()) {
    *id = SYMBOL_TO_JSID(idVal.toSymbol());
  } else {
    if (!ValueToIdPure(idVal, id)) {
      return false;
    }
  }

  // Watch out for ids that may be stored in dense elements.
  static_assert(NativeObject::MAX_DENSE_ELEMENTS_COUNT < JSID_INT_MAX,
                "All dense elements must have integer jsids");
  if (MOZ_UNLIKELY(JSID_IS_INT(*id))) {
    return false;
  }

  return true;
}

<<<<<<< HEAD
template <bool HandleMissing>
bool GetNativeDataPropertyByValuePure(JSContext* cx, JSObject* obj, Value* vp) {
  AutoUnsafeCallWithABI unsafe;
||||||| merged common ancestors
template bool
GetNativeDataPropertyByValuePure<true>(JSContext* cx, JSObject* obj, Value* vp);
=======
template <bool HandleMissing>
bool GetNativeDataPropertyByValuePure(JSContext* cx, JSObject* obj, Value* vp) {
  AutoUnsafeCallWithABI unsafe;

  // Condition checked by caller.
  MOZ_ASSERT(obj->isNative());

  // vp[0] contains the id, result will be stored in vp[1].
  Value idVal = vp[0];
  jsid id;
  if (!ValueToAtomOrSymbolPure(cx, idVal, &id)) {
    return false;
  }

  Value* res = vp + 1;
  return GetNativeDataPropertyPure<HandleMissing>(cx, &obj->as<NativeObject>(),
                                                  id, res);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Condition checked by caller.
  MOZ_ASSERT(obj->isNative());

  // vp[0] contains the id, result will be stored in vp[1].
  Value idVal = vp[0];
  jsid id;
  if (!ValueToAtomOrSymbolPure(cx, idVal, &id)) {
    return false;
  }

  Value* res = vp + 1;
  return GetNativeDataPropertyPure<HandleMissing>(cx, &obj->as<NativeObject>(),
                                                  id, res);
}

template bool GetNativeDataPropertyByValuePure<true>(JSContext* cx,
                                                     JSObject* obj, Value* vp);

template bool GetNativeDataPropertyByValuePure<false>(JSContext* cx,
                                                      JSObject* obj, Value* vp);
||||||| merged common ancestors
template bool
GetNativeDataPropertyByValuePure<false>(JSContext* cx, JSObject* obj, Value* vp);
=======
template bool GetNativeDataPropertyByValuePure<true>(JSContext* cx,
                                                     JSObject* obj, Value* vp);

template bool GetNativeDataPropertyByValuePure<false>(JSContext* cx,
                                                      JSObject* obj, Value* vp);
>>>>>>> upstream-releases

template <bool NeedsTypeBarrier>
bool SetNativeDataPropertyPure(JSContext* cx, JSObject* obj, PropertyName* name,
                               Value* val) {
  AutoUnsafeCallWithABI unsafe;

  if (MOZ_UNLIKELY(!obj->isNative())) {
    return false;
  }

  NativeObject* nobj = &obj->as<NativeObject>();
  Shape* shape = nobj->lastProperty()->search(cx, NameToId(name));
  if (!shape || !shape->isDataProperty() || !shape->writable()) {
    return false;
  }

  if (NeedsTypeBarrier && !HasTypePropertyId(nobj, NameToId(name), *val)) {
    return false;
  }

  nobj->setSlot(shape->slot(), *val);
  return true;
}

<<<<<<< HEAD
template bool SetNativeDataPropertyPure<true>(JSContext* cx, JSObject* obj,
                                              PropertyName* name, Value* val);
||||||| merged common ancestors
template bool
SetNativeDataPropertyPure<true>(JSContext* cx, JSObject* obj, PropertyName* name, Value* val);
=======
template bool SetNativeDataPropertyPure<true>(JSContext* cx, JSObject* obj,
                                              PropertyName* name, Value* val);

template bool SetNativeDataPropertyPure<false>(JSContext* cx, JSObject* obj,
                                               PropertyName* name, Value* val);
>>>>>>> upstream-releases

<<<<<<< HEAD
template bool SetNativeDataPropertyPure<false>(JSContext* cx, JSObject* obj,
                                               PropertyName* name, Value* val);
||||||| merged common ancestors
template bool
SetNativeDataPropertyPure<false>(JSContext* cx, JSObject* obj, PropertyName* name, Value* val);
=======
bool ObjectHasGetterSetterPure(JSContext* cx, JSObject* objArg,
                               Shape* propShape) {
  AutoUnsafeCallWithABI unsafe;
>>>>>>> upstream-releases

<<<<<<< HEAD
bool ObjectHasGetterSetterPure(JSContext* cx, JSObject* objArg,
                               Shape* propShape) {
  AutoUnsafeCallWithABI unsafe;
||||||| merged common ancestors
bool
ObjectHasGetterSetterPure(JSContext* cx, JSObject* objArg, Shape* propShape)
{
    AutoUnsafeCallWithABI unsafe;
=======
  MOZ_ASSERT(propShape->hasGetterObject() || propShape->hasSetterObject());
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(propShape->hasGetterObject() || propShape->hasSetterObject());
||||||| merged common ancestors
    MOZ_ASSERT(propShape->hasGetterObject() || propShape->hasSetterObject());
=======
  // Window objects may require outerizing (passing the WindowProxy to the
  // getter/setter), so we don't support them here.
  if (MOZ_UNLIKELY(!objArg->isNative() || IsWindow(objArg))) {
    return false;
  }

  NativeObject* nobj = &objArg->as<NativeObject>();
  jsid id = propShape->propid();

  while (true) {
    if (Shape* shape = nobj->lastProperty()->search(cx, id)) {
      if (shape == propShape) {
        return true;
      }
      if (shape->getterOrUndefined() == propShape->getterOrUndefined() &&
          shape->setterOrUndefined() == propShape->setterOrUndefined()) {
        return true;
      }
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Window objects may require outerizing (passing the WindowProxy to the
  // getter/setter), so we don't support them here.
  if (MOZ_UNLIKELY(!objArg->isNative() || IsWindow(objArg))) {
    return false;
  }

  NativeObject* nobj = &objArg->as<NativeObject>();
  jsid id = propShape->propid();

  while (true) {
    if (Shape* shape = nobj->lastProperty()->search(cx, id)) {
      if (shape == propShape) {
        return true;
      }
      if (shape->getterOrUndefined() == propShape->getterOrUndefined() &&
          shape->setterOrUndefined() == propShape->setterOrUndefined()) {
        return true;
      }
      return false;
    }

    // Property not found. Watch out for Class hooks.
    if (!nobj->is<PlainObject>()) {
      if (ClassMayResolveId(cx->names(), nobj->getClass(), id, nobj)) {
||||||| merged common ancestors
    // Window objects may require outerizing (passing the WindowProxy to the
    // getter/setter), so we don't support them here.
    if (MOZ_UNLIKELY(!objArg->isNative() || IsWindow(objArg))) {
=======
    // Property not found. Watch out for Class hooks.
    if (!nobj->is<PlainObject>()) {
      if (ClassMayResolveId(cx->names(), nobj->getClass(), id, nobj)) {
>>>>>>> upstream-releases
        return false;
<<<<<<< HEAD
      }
    }

    JSObject* proto = nobj->staticPrototype();
    if (!proto) {
      return false;
||||||| merged common ancestors
=======
      }
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (!proto->isNative()) {
      return false;
||||||| merged common ancestors
    NativeObject* nobj = &objArg->as<NativeObject>();
    jsid id = propShape->propid();

    while (true) {
        if (Shape* shape = nobj->lastProperty()->search(cx, id)) {
            if (shape == propShape) {
                return true;
            }
            if (shape->getterOrUndefined() == propShape->getterOrUndefined() &&
                shape->setterOrUndefined() == propShape->setterOrUndefined())
            {
                return true;
            }
            return false;
        }

        // Property not found. Watch out for Class hooks.
        if (!nobj->is<PlainObject>()) {
            if (ClassMayResolveId(cx->names(), nobj->getClass(), id, nobj)) {
                return false;
            }
        }

        JSObject* proto = nobj->staticPrototype();
        if (!proto) {
            return false;
        }

        if (!proto->isNative()) {
            return false;
        }
        nobj = &proto->as<NativeObject>();
=======
    JSObject* proto = nobj->staticPrototype();
    if (!proto) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    nobj = &proto->as<NativeObject>();
  }
||||||| merged common ancestors
=======

    if (!proto->isNative()) {
      return false;
    }
    nobj = &proto->as<NativeObject>();
  }
>>>>>>> upstream-releases
}

template <bool HasOwn>
bool HasNativeDataPropertyPure(JSContext* cx, JSObject* obj, Value* vp) {
  AutoUnsafeCallWithABI unsafe;

  // vp[0] contains the id, result will be stored in vp[1].
  Value idVal = vp[0];
  jsid id;
  if (!ValueToAtomOrSymbolPure(cx, idVal, &id)) {
    return false;
  }

  do {
    if (obj->isNative()) {
      if (obj->as<NativeObject>().lastProperty()->search(cx, id)) {
        vp[1].setBoolean(true);
        return true;
      }

      // Fail if there's a resolve hook, unless the mayResolve hook tells
      // us the resolve hook won't define a property with this id.
      if (MOZ_UNLIKELY(
              ClassMayResolveId(cx->names(), obj->getClass(), id, obj))) {
        return false;
<<<<<<< HEAD
      }
    } else if (obj->is<UnboxedPlainObject>()) {
      if (obj->as<UnboxedPlainObject>().containsUnboxedOrExpandoProperty(cx,
                                                                         id)) {
        vp[1].setBoolean(true);
        return true;
      }
    } else if (obj->is<TypedObject>()) {
      if (obj->as<TypedObject>().typeDescr().hasProperty(cx->names(), id)) {
        vp[1].setBoolean(true);
        return true;
      }
    } else {
      return false;
||||||| merged common ancestors
=======
      }
    } else if (obj->is<TypedObject>()) {
      if (obj->as<TypedObject>().typeDescr().hasProperty(cx->names(), id)) {
        vp[1].setBoolean(true);
        return true;
      }
    } else {
      return false;
>>>>>>> upstream-releases
    }

    // If implementing Object.hasOwnProperty, don't follow protochain.
    if (HasOwn) {
      break;
    }

    // Get prototype. Objects that may allow dynamic prototypes are already
    // filtered out above.
    obj = obj->staticPrototype();
  } while (obj);

  // Missing property.
  vp[1].setBoolean(false);
  return true;
}

template bool HasNativeDataPropertyPure<true>(JSContext* cx, JSObject* obj,
                                              Value* vp);

template bool HasNativeDataPropertyPure<false>(JSContext* cx, JSObject* obj,
                                               Value* vp);

bool HasNativeElementPure(JSContext* cx, NativeObject* obj, int32_t index,
                          Value* vp) {
  AutoUnsafeCallWithABI unsafe;

  MOZ_ASSERT(obj->getClass()->isNative());
  MOZ_ASSERT(!obj->getOpsHasProperty());
  MOZ_ASSERT(!obj->getOpsLookupProperty());
  MOZ_ASSERT(!obj->getOpsGetOwnPropertyDescriptor());

  if (MOZ_UNLIKELY(index < 0)) {
    return false;
  }

  if (obj->containsDenseElement(index)) {
    vp[0].setBoolean(true);
    return true;
  }

  jsid id = INT_TO_JSID(index);
  if (obj->lastProperty()->search(cx, id)) {
    vp[0].setBoolean(true);
    return true;
  }

  // Fail if there's a resolve hook, unless the mayResolve hook tells
  // us the resolve hook won't define a property with this id.
  if (MOZ_UNLIKELY(ClassMayResolveId(cx->names(), obj->getClass(), id, obj))) {
    return false;
  }
  // TypedArrayObject are also native and contain indexed properties.
  if (MOZ_UNLIKELY(obj->is<TypedArrayObject>())) {
    vp[0].setBoolean(uint32_t(index) < obj->as<TypedArrayObject>().length());
    return true;
<<<<<<< HEAD
  }

  vp[0].setBoolean(false);
  return true;
}
||||||| merged common ancestors
}
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JSString* TypeOfObject(JSObject* obj, JSRuntime* rt) {
  AutoUnsafeCallWithABI unsafe;
  JSType type = js::TypeOfObject(obj);
  return TypeName(type, *rt->commonNames);
||||||| merged common ancestors
JSString*
TypeOfObject(JSObject* obj, JSRuntime* rt)
{
    AutoUnsafeCallWithABI unsafe;
    JSType type = js::TypeOfObject(obj);
    return TypeName(type, *rt->commonNames);
=======
  vp[0].setBoolean(false);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool GetPrototypeOf(JSContext* cx, HandleObject target,
                    MutableHandleValue rval) {
  MOZ_ASSERT(target->hasDynamicPrototype());
||||||| merged common ancestors
bool
GetPrototypeOf(JSContext* cx, HandleObject target, MutableHandleValue rval)
{
    MOZ_ASSERT(target->hasDynamicPrototype());
=======
void HandleCodeCoverageAtPC(BaselineFrame* frame, jsbytecode* pc) {
  AutoUnsafeCallWithABI unsafe(UnsafeABIStrictness::AllowPendingExceptions);
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedObject proto(cx);
  if (!GetPrototype(cx, target, &proto)) {
    return false;
  }
  rval.setObjectOrNull(proto);
  return true;
}
||||||| merged common ancestors
    RootedObject proto(cx);
    if (!GetPrototype(cx, target, &proto)) {
        return false;
    }
    rval.setObjectOrNull(proto);
    return true;
}
=======
  MOZ_ASSERT(frame->runningInInterpreter());

  JSScript* script = frame->script();
  MOZ_ASSERT(pc == script->main() || BytecodeIsJumpTarget(JSOp(*pc)));

  if (!script->hasScriptCounts()) {
    if (!script->realm()->collectCoverageForDebug()) {
      return;
    }
    JSContext* cx = script->runtimeFromMainThread()->mainContextFromOwnThread();
    AutoEnterOOMUnsafeRegion oomUnsafe;
    if (!script->initScriptCounts(cx)) {
      oomUnsafe.crash("initScriptCounts");
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void CloseIteratorFromIon(JSContext* cx, JSObject* obj) { CloseIterator(obj); }
||||||| merged common ancestors
void
CloseIteratorFromIon(JSContext* cx, JSObject* obj)
{
    CloseIterator(obj);
}
=======
  PCCounts* counts = script->maybeGetPCCounts(pc);
  MOZ_ASSERT(counts);
  counts->numExec()++;
}
>>>>>>> upstream-releases

void HandleCodeCoverageAtPrologue(BaselineFrame* frame) {
  AutoUnsafeCallWithABI unsafe;

<<<<<<< HEAD
typedef JSString* (*ConcatStringsFn)(JSContext*, HandleString, HandleString);
const VMFunction ConcatStringsInfo =
    FunctionInfo<ConcatStringsFn>(ConcatStrings<CanGC>, "ConcatStrings");
||||||| merged common ancestors

typedef JSString* (*ConcatStringsFn)(JSContext*, HandleString, HandleString);
const VMFunction ConcatStringsInfo =
    FunctionInfo<ConcatStringsFn>(ConcatStrings<CanGC>, "ConcatStrings");
=======
  MOZ_ASSERT(frame->runningInInterpreter());

  JSScript* script = frame->script();
  jsbytecode* main = script->main();
  if (!BytecodeIsJumpTarget(JSOp(*main))) {
    HandleCodeCoverageAtPC(frame, main);
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
static JSString* ConvertObjectToStringForConcat(JSContext* cx,
                                                HandleValue obj) {
  MOZ_ASSERT(obj.isObject());
  RootedValue rootedObj(cx, obj);
  if (!ToPrimitive(cx, &rootedObj)) {
    return nullptr;
  }
  return ToString<CanGC>(cx, rootedObj);
}

bool DoConcatStringObject(JSContext* cx, HandleValue lhs, HandleValue rhs,
                          MutableHandleValue res) {
  JSString* lstr = nullptr;
  JSString* rstr = nullptr;

  if (lhs.isString()) {
    // Convert rhs first.
    MOZ_ASSERT(lhs.isString() && rhs.isObject());
    rstr = ConvertObjectToStringForConcat(cx, rhs);
    if (!rstr) {
      return false;
    }

    // lhs is already string.
    lstr = lhs.toString();
  } else {
    MOZ_ASSERT(rhs.isString() && lhs.isObject());
    // Convert lhs first.
    lstr = ConvertObjectToStringForConcat(cx, lhs);
    if (!lstr) {
      return false;
    }
||||||| merged common ancestors
static JSString*
ConvertObjectToStringForConcat(JSContext* cx, HandleValue obj)
{
    MOZ_ASSERT(obj.isObject());
    RootedValue rootedObj(cx, obj);
    if (!ToPrimitive(cx, &rootedObj)) {
        return nullptr;
    }
    return ToString<CanGC>(cx, rootedObj);
}

bool
DoConcatStringObject(JSContext* cx, HandleValue lhs, HandleValue rhs,
                     MutableHandleValue res)
{
    JSString* lstr = nullptr;
    JSString* rstr = nullptr;

    if (lhs.isString()) {
        // Convert rhs first.
        MOZ_ASSERT(lhs.isString() && rhs.isObject());
        rstr = ConvertObjectToStringForConcat(cx, rhs);
        if (!rstr) {
            return false;
        }

        // lhs is already string.
        lstr = lhs.toString();
    } else {
        MOZ_ASSERT(rhs.isString() && lhs.isObject());
        // Convert lhs first.
        lstr = ConvertObjectToStringForConcat(cx, lhs);
        if (!lstr) {
            return false;
        }

        // rhs is already string.
        rstr = rhs.toString();
    }
=======
JSString* TypeOfObject(JSObject* obj, JSRuntime* rt) {
  AutoUnsafeCallWithABI unsafe;
  JSType type = js::TypeOfObject(obj);
  return TypeName(type, *rt->commonNames);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    // rhs is already string.
    rstr = rhs.toString();
  }

  JSString* str = ConcatStrings<NoGC>(cx, lstr, rstr);
  if (!str) {
    RootedString nlstr(cx, lstr), nrstr(cx, rstr);
    str = ConcatStrings<CanGC>(cx, nlstr, nrstr);
    if (!str) {
      return false;
    }
  }
||||||| merged common ancestors
    JSString* str = ConcatStrings<NoGC>(cx, lstr, rstr);
    if (!str) {
        RootedString nlstr(cx, lstr), nrstr(cx, rstr);
        str = ConcatStrings<CanGC>(cx, nlstr, nrstr);
        if (!str) {
            return false;
        }
    }
=======
bool GetPrototypeOf(JSContext* cx, HandleObject target,
                    MutableHandleValue rval) {
  MOZ_ASSERT(target->hasDynamicPrototype());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Technically, we need to call TypeScript::MonitorString for this PC, however
  // it was called when this stub was attached so it's OK.
||||||| merged common ancestors
    // Technically, we need to call TypeScript::MonitorString for this PC, however
    // it was called when this stub was attached so it's OK.
=======
  RootedObject proto(cx);
  if (!GetPrototype(cx, target, &proto)) {
    return false;
  }
  rval.setObjectOrNull(proto);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  res.setString(str);
  return true;
||||||| merged common ancestors
    res.setString(str);
    return true;
=======
static JSString* ConvertObjectToStringForConcat(JSContext* cx,
                                                HandleValue obj) {
  MOZ_ASSERT(obj.isObject());
  RootedValue rootedObj(cx, obj);
  if (!ToPrimitive(cx, &rootedObj)) {
    return nullptr;
  }
  return ToString<CanGC>(cx, rootedObj);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*DoConcatStringObjectFn)(JSContext*, HandleValue, HandleValue,
                                       MutableHandleValue);
const VMFunction DoConcatStringObjectInfo =
    FunctionInfo<DoConcatStringObjectFn>(
        DoConcatStringObject, "DoConcatStringObject", TailCall, PopValues(2));
||||||| merged common ancestors
typedef bool (*DoConcatStringObjectFn)(JSContext*, HandleValue, HandleValue,
                                       MutableHandleValue);
const VMFunction DoConcatStringObjectInfo =
    FunctionInfo<DoConcatStringObjectFn>(DoConcatStringObject, "DoConcatStringObject", TailCall, PopValues(2));
=======
bool DoConcatStringObject(JSContext* cx, HandleValue lhs, HandleValue rhs,
                          MutableHandleValue res) {
  JSString* lstr = nullptr;
  JSString* rstr = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
MOZ_MUST_USE bool TrySkipAwait(JSContext* cx, HandleValue val,
                               MutableHandleValue resolved) {
  bool canSkip;
  if (!TrySkipAwait(cx, val, &canSkip, resolved)) {
    return false;
  }
||||||| merged common ancestors
MOZ_MUST_USE bool
TrySkipAwait(JSContext* cx, HandleValue val, MutableHandleValue resolved)
{
    bool canSkip;
    if (!TrySkipAwait(cx, val, &canSkip, resolved)) {
        return false;
    }
=======
  if (lhs.isString()) {
    // Convert rhs first.
    MOZ_ASSERT(lhs.isString() && rhs.isObject());
    rstr = ConvertObjectToStringForConcat(cx, rhs);
    if (!rstr) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!canSkip) {
    resolved.setMagic(JS_CANNOT_SKIP_AWAIT);
  }
||||||| merged common ancestors
    if (!canSkip) {
        resolved.setMagic(JS_CANNOT_SKIP_AWAIT);
    }
=======
    // lhs is already string.
    lstr = lhs.toString();
  } else {
    MOZ_ASSERT(rhs.isString() && lhs.isObject());
    // Convert lhs first.
    lstr = ConvertObjectToStringForConcat(cx, lhs);
    if (!lstr) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
    // rhs is already string.
    rstr = rhs.toString();
  }

  JSString* str = ConcatStrings<NoGC>(cx, lstr, rstr);
  if (!str) {
    RootedString nlstr(cx, lstr), nrstr(cx, rstr);
    str = ConcatStrings<CanGC>(cx, nlstr, nrstr);
    if (!str) {
      return false;
    }
  }

  // Note: we don't have to call JitScript::MonitorBytecodeType because we
  // monitored the string-type when attaching the IC stub.

  res.setString(str);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*ProxyGetPropertyFn)(JSContext*, HandleObject, HandleId,
                                   MutableHandleValue);
const VMFunction ProxyGetPropertyInfo =
    FunctionInfo<ProxyGetPropertyFn>(ProxyGetProperty, "ProxyGetProperty");
||||||| merged common ancestors
typedef bool (*ProxyGetPropertyFn)(JSContext*, HandleObject, HandleId, MutableHandleValue);
const VMFunction ProxyGetPropertyInfo =
    FunctionInfo<ProxyGetPropertyFn>(ProxyGetProperty, "ProxyGetProperty");
=======
MOZ_MUST_USE bool TrySkipAwait(JSContext* cx, HandleValue val,
                               MutableHandleValue resolved) {
  bool canSkip;
  if (!TrySkipAwait(cx, val, &canSkip, resolved)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*ProxyGetPropertyByValueFn)(JSContext*, HandleObject, HandleValue,
                                          MutableHandleValue);
const VMFunction ProxyGetPropertyByValueInfo =
    FunctionInfo<ProxyGetPropertyByValueFn>(ProxyGetPropertyByValue,
                                            "ProxyGetPropertyByValue");
||||||| merged common ancestors
typedef bool (*ProxyGetPropertyByValueFn)(JSContext*, HandleObject, HandleValue, MutableHandleValue);
const VMFunction ProxyGetPropertyByValueInfo =
    FunctionInfo<ProxyGetPropertyByValueFn>(ProxyGetPropertyByValue, "ProxyGetPropertyByValue");
=======
  if (!canSkip) {
    resolved.setMagic(JS_CANNOT_SKIP_AWAIT);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*ProxySetPropertyFn)(JSContext*, HandleObject, HandleId,
                                   HandleValue, bool);
const VMFunction ProxySetPropertyInfo =
    FunctionInfo<ProxySetPropertyFn>(ProxySetProperty, "ProxySetProperty");
||||||| merged common ancestors
typedef bool (*ProxySetPropertyFn)(JSContext*, HandleObject, HandleId, HandleValue, bool);
const VMFunction ProxySetPropertyInfo =
    FunctionInfo<ProxySetPropertyFn>(ProxySetProperty, "ProxySetProperty");
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*ProxySetPropertyByValueFn)(JSContext*, HandleObject, HandleValue,
                                          HandleValue, bool);
const VMFunction ProxySetPropertyByValueInfo =
    FunctionInfo<ProxySetPropertyByValueFn>(ProxySetPropertyByValue,
                                            "ProxySetPropertyByValue");
||||||| merged common ancestors
typedef bool (*ProxySetPropertyByValueFn)(JSContext*, HandleObject, HandleValue, HandleValue, bool);
const VMFunction ProxySetPropertyByValueInfo =
    FunctionInfo<ProxySetPropertyByValueFn>(ProxySetPropertyByValue, "ProxySetPropertyByValue");
=======
bool IsPossiblyWrappedTypedArray(JSContext* cx, JSObject* obj, bool* result) {
  JSObject* unwrapped = CheckedUnwrapDynamic(obj, cx);
  if (!unwrapped) {
    ReportAccessDenied(cx);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*ProxyHasFn)(JSContext*, HandleObject, HandleValue,
                           MutableHandleValue);
const VMFunction ProxyHasInfo = FunctionInfo<ProxyHasFn>(ProxyHas, "ProxyHas");
||||||| merged common ancestors
typedef bool (*ProxyHasFn)(JSContext*, HandleObject, HandleValue, MutableHandleValue);
const VMFunction ProxyHasInfo = FunctionInfo<ProxyHasFn>(ProxyHas, "ProxyHas");
=======
  *result = unwrapped->is<TypedArrayObject>();
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*ProxyHasOwnFn)(JSContext*, HandleObject, HandleValue,
                              MutableHandleValue);
const VMFunction ProxyHasOwnInfo =
    FunctionInfo<ProxyHasOwnFn>(ProxyHasOwn, "ProxyHasOwn");
||||||| merged common ancestors
typedef bool (*ProxyHasOwnFn)(JSContext*, HandleObject, HandleValue, MutableHandleValue);
const VMFunction ProxyHasOwnInfo = FunctionInfo<ProxyHasOwnFn>(ProxyHasOwn, "ProxyHasOwn");
=======
bool DoToNumber(JSContext* cx, HandleValue arg, MutableHandleValue ret) {
  ret.set(arg);
  return ToNumber(cx, ret);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*NativeGetElementFn)(JSContext*, HandleNativeObject, HandleValue,
                                   int32_t, MutableHandleValue);
const VMFunction NativeGetElementInfo =
    FunctionInfo<NativeGetElementFn>(NativeGetElement, "NativeGetProperty");
||||||| merged common ancestors
typedef bool (*NativeGetElementFn)(JSContext*, HandleNativeObject, HandleValue, int32_t,
                                   MutableHandleValue);
const VMFunction NativeGetElementInfo =
    FunctionInfo<NativeGetElementFn>(NativeGetElement, "NativeGetProperty");
=======
bool DoToNumeric(JSContext* cx, HandleValue arg, MutableHandleValue ret) {
  ret.set(arg);
  return ToNumeric(cx, ret);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*AddOrUpdateSparseElementHelperFn)(JSContext* cx,
                                                 HandleArrayObject obj,
                                                 int32_t int_id, HandleValue v,
                                                 bool strict);
const VMFunction AddOrUpdateSparseElementHelperInfo =
    FunctionInfo<AddOrUpdateSparseElementHelperFn>(
        AddOrUpdateSparseElementHelper, "AddOrUpdateSparseElementHelper");

typedef bool (*GetSparseElementHelperFn)(JSContext* cx, HandleArrayObject obj,
                                         int32_t int_id,
                                         MutableHandleValue result);
const VMFunction GetSparseElementHelperInfo =
    FunctionInfo<GetSparseElementHelperFn>(GetSparseElementHelper,
                                           "getSparseElementHelper");

}  // namespace jit
}  // namespace js
||||||| merged common ancestors
} // namespace jit
} // namespace js
=======
}  // namespace jit
}  // namespace js
>>>>>>> upstream-releases
