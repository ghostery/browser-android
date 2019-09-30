/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "jit/JitOptions.h"
#include "mozilla/TypeTraits.h"

#include <cstdlib>

#include "vm/JSFunction.h"

using namespace js;
using namespace js::jit;

using mozilla::Maybe;

namespace js {
namespace jit {

DefaultJitOptions JitOptions;

static void Warn(const char* env, const char* value) {
  fprintf(stderr, "Warning: I didn't understand %s=\"%s\"\n", env, value);
}

template <typename T>
struct IsBool : mozilla::FalseType {};
template <>
struct IsBool<bool> : mozilla::TrueType {};

static Maybe<int> ParseInt(const char* str) {
  char* endp;
  int retval = strtol(str, &endp, 0);
  if (*endp == '\0') {
    return mozilla::Some(retval);
  }
  return mozilla::Nothing();
}

template <typename T>
T overrideDefault(const char* param, T dflt) {
  char* str = getenv(param);
  if (!str) {
    return dflt;
  }
  if (IsBool<T>::value) {
    if (strcmp(str, "true") == 0 || strcmp(str, "yes") == 0) {
      return true;
    }
    if (strcmp(str, "false") == 0 || strcmp(str, "no") == 0) {
      return false;
    }
    Warn(param, str);
  } else {
    Maybe<int> value = ParseInt(str);
    if (value.isSome()) {
      return value.ref();
    }
    Warn(param, str);
  }
  return dflt;
}

#define SET_DEFAULT(var, dflt) var = overrideDefault("JIT_OPTION_" #var, dflt)
DefaultJitOptions::DefaultJitOptions() {
  // Whether to perform expensive graph-consistency DEBUG-only assertions.
  // It can be useful to disable this to reduce DEBUG-compile time of large
  // wasm programs.
  SET_DEFAULT(checkGraphConsistency, true);

#ifdef CHECK_OSIPOINT_REGISTERS
  // Emit extra code to verify live regs at the start of a VM call
  // are not modified before its OsiPoint.
  SET_DEFAULT(checkOsiPointRegisters, false);
#endif

<<<<<<< HEAD
  // Whether to enable extra code to perform dynamic validation of
  // RangeAnalysis results.
  SET_DEFAULT(checkRangeAnalysis, false);
||||||| merged common ancestors
    // Whether to enable extra code to perform dynamic validation of
    // RangeAnalysis results.
    SET_DEFAULT(checkRangeAnalysis, false);

    // Toggles whether IonBuilder fallbacks to a call if we fail to inline.
    SET_DEFAULT(disableInlineBacktracking, false);

    // Toggles whether Alignment Mask Analysis is globally disabled.
    SET_DEFAULT(disableAma, false);

    // Toggles whether Effective Address Analysis is globally disabled.
    SET_DEFAULT(disableEaa, false);

    // Toggles whether Edge Case Analysis is gobally disabled.
    SET_DEFAULT(disableEdgeCaseAnalysis, false);

    // Toggle whether global value numbering is globally disabled.
    SET_DEFAULT(disableGvn, false);

    // Toggles whether inlining is globally disabled.
    SET_DEFAULT(disableInlining, false);

    // Toggles whether loop invariant code motion is globally disabled.
    SET_DEFAULT(disableLicm, false);

    // Toggles whether Loop Unrolling is globally disabled.
    SET_DEFAULT(disableLoopUnrolling, true);

    // Toggles wheter optimization tracking is globally disabled.
    SET_DEFAULT(disableOptimizationTracking, true);
=======
  // Whether to enable extra code to perform dynamic validation of
  // RangeAnalysis results.
  SET_DEFAULT(checkRangeAnalysis, false);

  // Toggles whether IonBuilder fallbacks to a call if we fail to inline.
  SET_DEFAULT(disableInlineBacktracking, false);

  // Toggles whether Alignment Mask Analysis is globally disabled.
  SET_DEFAULT(disableAma, false);

  // Toggles whether Effective Address Analysis is globally disabled.
  SET_DEFAULT(disableEaa, false);

  // Toggles whether Edge Case Analysis is gobally disabled.
  SET_DEFAULT(disableEdgeCaseAnalysis, false);

  // Toggle whether global value numbering is globally disabled.
  SET_DEFAULT(disableGvn, false);

  // Toggles whether inlining is globally disabled.
  SET_DEFAULT(disableInlining, false);

  // Toggles whether loop invariant code motion is globally disabled.
  SET_DEFAULT(disableLicm, false);

  // Toggles wheter optimization tracking is globally disabled.
  SET_DEFAULT(disableOptimizationTracking, true);

  // Toggle whether Profile Guided Optimization is globally disabled.
  SET_DEFAULT(disablePgo, false);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether IonBuilder fallbacks to a call if we fail to inline.
  SET_DEFAULT(disableInlineBacktracking, false);
||||||| merged common ancestors
    // Toggle whether Profile Guided Optimization is globally disabled.
    SET_DEFAULT(disablePgo, false);
=======
  // Toggles whether instruction reordering is globally disabled.
  SET_DEFAULT(disableInstructionReordering, false);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether Alignment Mask Analysis is globally disabled.
  SET_DEFAULT(disableAma, false);
||||||| merged common ancestors
    // Toggles whether instruction reordering is globally disabled.
    SET_DEFAULT(disableInstructionReordering, false);
=======
  // Toggles whether Range Analysis is globally disabled.
  SET_DEFAULT(disableRangeAnalysis, false);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether Effective Address Analysis is globally disabled.
  SET_DEFAULT(disableEaa, false);
||||||| merged common ancestors
    // Toggles whether Range Analysis is globally disabled.
    SET_DEFAULT(disableRangeAnalysis, false);
=======
  // Toggles wheter Recover instructions is globally disabled.
  SET_DEFAULT(disableRecoverIns, false);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether Edge Case Analysis is gobally disabled.
  SET_DEFAULT(disableEdgeCaseAnalysis, false);
||||||| merged common ancestors
    // Toggles wheter Recover instructions is globally disabled.
    SET_DEFAULT(disableRecoverIns, false);
=======
  // Toggle whether eager scalar replacement is globally disabled.
  SET_DEFAULT(disableScalarReplacement, false);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggle whether global value numbering is globally disabled.
  SET_DEFAULT(disableGvn, false);
||||||| merged common ancestors
    // Toggle whether eager scalar replacement is globally disabled.
    SET_DEFAULT(disableScalarReplacement, false);
=======
  // Toggles whether CacheIR stubs are used.
  SET_DEFAULT(disableCacheIR, false);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether inlining is globally disabled.
  SET_DEFAULT(disableInlining, false);
||||||| merged common ancestors
    // Toggles whether CacheIR stubs are used.
    SET_DEFAULT(disableCacheIR, false);
=======
  // Toggles whether sink code motion is globally disabled.
  SET_DEFAULT(disableSink, true);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether loop invariant code motion is globally disabled.
  SET_DEFAULT(disableLicm, false);
||||||| merged common ancestors
    // Toggles whether CacheIR stubs for binary arith operations are used
    SET_DEFAULT(disableCacheIRBinaryArith, false);
=======
  // Toggles whether the use of multiple Ion optimization levels is globally
  // disabled.
  SET_DEFAULT(disableOptimizationLevels, false);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether Loop Unrolling is globally disabled.
  SET_DEFAULT(disableLoopUnrolling, true);
||||||| merged common ancestors
    // Toggles whether sincos optimization is globally disabled.
    // See bug984018: The MacOS is the only one that has the sincos fast.
    #if defined(XP_MACOSX)
        SET_DEFAULT(disableSincos, false);
    #else
        SET_DEFAULT(disableSincos, true);
    #endif
=======
  // Whether the Baseline Interpreter is enabled.
  SET_DEFAULT(baselineInterpreter, false);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles wheter optimization tracking is globally disabled.
  SET_DEFAULT(disableOptimizationTracking, true);
||||||| merged common ancestors
    // Toggles whether sink code motion is globally disabled.
    SET_DEFAULT(disableSink, true);
=======
  // Whether IonBuilder should prefer IC generation above specialized MIR.
  SET_DEFAULT(forceInlineCaches, false);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggle whether Profile Guided Optimization is globally disabled.
  SET_DEFAULT(disablePgo, false);
||||||| merged common ancestors
    // Whether functions are compiled immediately.
    SET_DEFAULT(eagerCompilation, false);
=======
  // Toggles whether large scripts are rejected.
  SET_DEFAULT(limitScriptSize, true);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether instruction reordering is globally disabled.
  SET_DEFAULT(disableInstructionReordering, false);
||||||| merged common ancestors
    // Whether IonBuilder should prefer IC generation above specialized MIR.
    SET_DEFAULT(forceInlineCaches, false);
=======
  // Toggles whether functions may be entered at loop headers.
  SET_DEFAULT(osr, true);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether Range Analysis is globally disabled.
  SET_DEFAULT(disableRangeAnalysis, false);
||||||| merged common ancestors
    // Toggles whether large scripts are rejected.
    SET_DEFAULT(limitScriptSize, true);
=======
  // Whether to enable extra code to perform dynamic validations.
  SET_DEFAULT(runExtraChecks, false);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles wheter Recover instructions is globally disabled.
  SET_DEFAULT(disableRecoverIns, false);
||||||| merged common ancestors
    // Toggles whether functions may be entered at loop headers.
    SET_DEFAULT(osr, true);
=======
  // How many invocations or loop iterations are needed before functions
  // enter the Baseline Interpreter.
  SET_DEFAULT(baselineInterpreterWarmUpThreshold, 10);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggle whether eager scalar replacement is globally disabled.
  SET_DEFAULT(disableScalarReplacement, false);
||||||| merged common ancestors
    // Whether to enable extra code to perform dynamic validations.
    SET_DEFAULT(runExtraChecks, false);
=======
  // How many invocations or loop iterations are needed before functions
  // are compiled with the baseline compiler.
  // Duplicated in all.js - ensure both match.
  SET_DEFAULT(baselineWarmUpThreshold, 10);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether CacheIR stubs are used.
  SET_DEFAULT(disableCacheIR, false);
||||||| merged common ancestors
    // How many invocations or loop iterations are needed before functions
    // are compiled with the baseline compiler.
    // Duplicated in all.js - ensure both match.
    SET_DEFAULT(baselineWarmUpThreshold, 10);
=======
  // How many invocations or loop iterations are needed before functions
  // are compiled with the Ion compiler at OptimizationLevel::Normal.
  // Duplicated in all.js - ensure both match.
  SET_DEFAULT(normalIonWarmUpThreshold, 1000);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether CacheIR stubs for binary arith operations are used
  SET_DEFAULT(disableCacheIRBinaryArith, false);
||||||| merged common ancestors
    // Number of exception bailouts (resuming into catch/finally block) before
    // we invalidate and forbid Ion compilation.
    SET_DEFAULT(exceptionBailoutThreshold, 10);
=======
  // How many invocations or loop iterations are needed before functions
  // are compiled with the Ion compiler at OptimizationLevel::Full.
  // Duplicated in all.js - ensure both match.
  SET_DEFAULT(fullIonWarmUpThreshold, 100'000);
>>>>>>> upstream-releases

<<<<<<< HEAD
// Toggles whether sincos optimization is globally disabled.
// See bug984018: The MacOS is the only one that has the sincos fast.
#if defined(XP_MACOSX)
  SET_DEFAULT(disableSincos, false);
#else
  SET_DEFAULT(disableSincos, true);
#endif
||||||| merged common ancestors
    // Number of bailouts without invalidation before we set
    // JSScript::hadFrequentBailouts and invalidate.
    // Duplicated in all.js - ensure both match.
    SET_DEFAULT(frequentBailoutThreshold, 10);

    // Whether to run all debug checks in debug builds.
    // Disabling might make it more enjoyable to run JS in debug builds.
    SET_DEFAULT(fullDebugChecks, true);

    // How many actual arguments are accepted on the C stack.
    SET_DEFAULT(maxStackArgs, 4096);

    // How many times we will try to enter a script via OSR before
    // invalidating the script.
    SET_DEFAULT(osrPcMismatchesBeforeRecompile, 6000);

    // The bytecode length limit for small function.
    SET_DEFAULT(smallFunctionMaxBytecodeLength_, 130);

    // An artificial testing limit for the maximum supported offset of
    // pc-relative jump and call instructions.
    SET_DEFAULT(jumpThreshold, UINT32_MAX);
=======
  // Number of exception bailouts (resuming into catch/finally block) before
  // we invalidate and forbid Ion compilation.
  SET_DEFAULT(exceptionBailoutThreshold, 10);

  // Number of bailouts without invalidation before we set
  // JSScript::hadFrequentBailouts and invalidate.
  // Duplicated in all.js - ensure both match.
  SET_DEFAULT(frequentBailoutThreshold, 10);

  // Whether to run all debug checks in debug builds.
  // Disabling might make it more enjoyable to run JS in debug builds.
  SET_DEFAULT(fullDebugChecks, true);

  // How many actual arguments are accepted on the C stack.
  SET_DEFAULT(maxStackArgs, 4096);

  // How many times we will try to enter a script via OSR before
  // invalidating the script.
  SET_DEFAULT(osrPcMismatchesBeforeRecompile, 6000);

  // The bytecode length limit for small function.
  SET_DEFAULT(smallFunctionMaxBytecodeLength_, 130);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether sink code motion is globally disabled.
  SET_DEFAULT(disableSink, true);

  // Whether functions are compiled immediately.
  SET_DEFAULT(eagerCompilation, false);

  // Whether IonBuilder should prefer IC generation above specialized MIR.
  SET_DEFAULT(forceInlineCaches, false);

  // Toggles whether large scripts are rejected.
  SET_DEFAULT(limitScriptSize, true);

  // Toggles whether functions may be entered at loop headers.
  SET_DEFAULT(osr, true);

  // Whether to enable extra code to perform dynamic validations.
  SET_DEFAULT(runExtraChecks, false);

  // How many invocations or loop iterations are needed before functions
  // are compiled with the baseline compiler.
  // Duplicated in all.js - ensure both match.
  SET_DEFAULT(baselineWarmUpThreshold, 10);

  // Number of exception bailouts (resuming into catch/finally block) before
  // we invalidate and forbid Ion compilation.
  SET_DEFAULT(exceptionBailoutThreshold, 10);

  // Number of bailouts without invalidation before we set
  // JSScript::hadFrequentBailouts and invalidate.
  // Duplicated in all.js - ensure both match.
  SET_DEFAULT(frequentBailoutThreshold, 10);

  // Whether to run all debug checks in debug builds.
  // Disabling might make it more enjoyable to run JS in debug builds.
  SET_DEFAULT(fullDebugChecks, true);

  // How many actual arguments are accepted on the C stack.
  SET_DEFAULT(maxStackArgs, 4096);

  // How many times we will try to enter a script via OSR before
  // invalidating the script.
  SET_DEFAULT(osrPcMismatchesBeforeRecompile, 6000);

  // The bytecode length limit for small function.
  SET_DEFAULT(smallFunctionMaxBytecodeLength_, 130);

  // An artificial testing limit for the maximum supported offset of
  // pc-relative jump and call instructions.
  SET_DEFAULT(jumpThreshold, UINT32_MAX);

  // Branch pruning heuristic is based on a scoring system, which is look at
  // different metrics and provide a score. The score is computed as a
  // projection where each factor defines the weight of each metric. Then this
  // score is compared against a threshold to prevent a branch from being
  // removed.
  SET_DEFAULT(branchPruningHitCountFactor, 1);
  SET_DEFAULT(branchPruningInstFactor, 10);
  SET_DEFAULT(branchPruningBlockSpanFactor, 100);
  SET_DEFAULT(branchPruningEffectfulInstFactor, 3500);
  SET_DEFAULT(branchPruningThreshold, 4000);

  // Force how many invocation or loop iterations are needed before compiling
  // a function with the highest ionmonkey optimization level.
  // (i.e. OptimizationLevel_Normal)
  const char* forcedDefaultIonWarmUpThresholdEnv =
      "JIT_OPTION_forcedDefaultIonWarmUpThreshold";
  if (const char* env = getenv(forcedDefaultIonWarmUpThresholdEnv)) {
    Maybe<int> value = ParseInt(env);
    if (value.isSome()) {
      forcedDefaultIonWarmUpThreshold.emplace(value.ref());
    } else {
      Warn(forcedDefaultIonWarmUpThresholdEnv, env);
    }
  }

  // Same but for compiling small functions.
  const char* forcedDefaultIonSmallFunctionWarmUpThresholdEnv =
      "JIT_OPTION_forcedDefaultIonSmallFunctionWarmUpThreshold";
  if (const char* env =
          getenv(forcedDefaultIonSmallFunctionWarmUpThresholdEnv)) {
    Maybe<int> value = ParseInt(env);
    if (value.isSome()) {
      forcedDefaultIonSmallFunctionWarmUpThreshold.emplace(value.ref());
    } else {
      Warn(forcedDefaultIonSmallFunctionWarmUpThresholdEnv, env);
    }
  }

  // Force the used register allocator instead of letting the optimization
  // pass decide.
  const char* forcedRegisterAllocatorEnv = "JIT_OPTION_forcedRegisterAllocator";
  if (const char* env = getenv(forcedRegisterAllocatorEnv)) {
    forcedRegisterAllocator = LookupRegisterAllocator(env);
    if (!forcedRegisterAllocator.isSome()) {
      Warn(forcedRegisterAllocatorEnv, env);
||||||| merged common ancestors
    // Branch pruning heuristic is based on a scoring system, which is look at
    // different metrics and provide a score. The score is computed as a
    // projection where each factor defines the weight of each metric. Then this
    // score is compared against a threshold to prevent a branch from being
    // removed.
    SET_DEFAULT(branchPruningHitCountFactor, 1);
    SET_DEFAULT(branchPruningInstFactor, 10);
    SET_DEFAULT(branchPruningBlockSpanFactor, 100);
    SET_DEFAULT(branchPruningEffectfulInstFactor, 3500);
    SET_DEFAULT(branchPruningThreshold, 4000);

    // Force how many invocation or loop iterations are needed before compiling
    // a function with the highest ionmonkey optimization level.
    // (i.e. OptimizationLevel_Normal)
    const char* forcedDefaultIonWarmUpThresholdEnv = "JIT_OPTION_forcedDefaultIonWarmUpThreshold";
    if (const char* env = getenv(forcedDefaultIonWarmUpThresholdEnv)) {
        Maybe<int> value = ParseInt(env);
        if (value.isSome()) {
            forcedDefaultIonWarmUpThreshold.emplace(value.ref());
        } else {
            Warn(forcedDefaultIonWarmUpThresholdEnv, env);
        }
    }

    // Same but for compiling small functions.
    const char* forcedDefaultIonSmallFunctionWarmUpThresholdEnv =
        "JIT_OPTION_forcedDefaultIonSmallFunctionWarmUpThreshold";
    if (const char* env = getenv(forcedDefaultIonSmallFunctionWarmUpThresholdEnv)) {
        Maybe<int> value = ParseInt(env);
        if (value.isSome()) {
            forcedDefaultIonSmallFunctionWarmUpThreshold.emplace(value.ref());
        } else {
            Warn(forcedDefaultIonSmallFunctionWarmUpThresholdEnv, env);
        }
    }

    // Force the used register allocator instead of letting the optimization
    // pass decide.
    const char* forcedRegisterAllocatorEnv = "JIT_OPTION_forcedRegisterAllocator";
    if (const char* env = getenv(forcedRegisterAllocatorEnv)) {
        forcedRegisterAllocator = LookupRegisterAllocator(env);
        if (!forcedRegisterAllocator.isSome()) {
            Warn(forcedRegisterAllocatorEnv, env);
        }
=======
  // An artificial testing limit for the maximum supported offset of
  // pc-relative jump and call instructions.
  SET_DEFAULT(jumpThreshold, UINT32_MAX);

  // Branch pruning heuristic is based on a scoring system, which is look at
  // different metrics and provide a score. The score is computed as a
  // projection where each factor defines the weight of each metric. Then this
  // score is compared against a threshold to prevent a branch from being
  // removed.
  SET_DEFAULT(branchPruningHitCountFactor, 1);
  SET_DEFAULT(branchPruningInstFactor, 10);
  SET_DEFAULT(branchPruningBlockSpanFactor, 100);
  SET_DEFAULT(branchPruningEffectfulInstFactor, 3500);
  SET_DEFAULT(branchPruningThreshold, 4000);

  // Limits on bytecode length and number of locals/arguments for Ion
  // compilation. There are different (lower) limits for when off-thread Ion
  // compilation isn't available.
  SET_DEFAULT(ionMaxScriptSize, 100 * 1000);
  SET_DEFAULT(ionMaxScriptSizeMainThread, 2 * 1000);
  SET_DEFAULT(ionMaxLocalsAndArgs, 10 * 1000);
  SET_DEFAULT(ionMaxLocalsAndArgsMainThread, 256);

  // Force the used register allocator instead of letting the optimization
  // pass decide.
  const char* forcedRegisterAllocatorEnv = "JIT_OPTION_forcedRegisterAllocator";
  if (const char* env = getenv(forcedRegisterAllocatorEnv)) {
    forcedRegisterAllocator = LookupRegisterAllocator(env);
    if (!forcedRegisterAllocator.isSome()) {
      Warn(forcedRegisterAllocatorEnv, env);
>>>>>>> upstream-releases
    }
  }

#if defined(JS_CODEGEN_MIPS32) || defined(JS_CODEGEN_MIPS64)
  SET_DEFAULT(spectreIndexMasking, false);
  SET_DEFAULT(spectreObjectMitigationsBarriers, false);
  SET_DEFAULT(spectreObjectMitigationsMisc, false);
  SET_DEFAULT(spectreStringMitigations, false);
  SET_DEFAULT(spectreValueMasking, false);
  SET_DEFAULT(spectreJitToCxxCalls, false);
#else
  SET_DEFAULT(spectreIndexMasking, true);
  SET_DEFAULT(spectreObjectMitigationsBarriers, true);
  SET_DEFAULT(spectreObjectMitigationsMisc, true);
  SET_DEFAULT(spectreStringMitigations, true);
  SET_DEFAULT(spectreValueMasking, true);
  SET_DEFAULT(spectreJitToCxxCalls, true);
#endif

<<<<<<< HEAD
  // Toggles whether unboxed plain objects can be created by the VM.
  SET_DEFAULT(disableUnboxedObjects, false);

  // Toggles the optimization whereby offsets are folded into loads and not
  // included in the bounds check.
  SET_DEFAULT(wasmFoldOffsets, true);
||||||| merged common ancestors
    SET_DEFAULT(spectreIndexMasking, true);
    SET_DEFAULT(spectreObjectMitigationsBarriers, true);
    SET_DEFAULT(spectreObjectMitigationsMisc, true);
    SET_DEFAULT(spectreStringMitigations, true);
    SET_DEFAULT(spectreValueMasking, true);
    SET_DEFAULT(spectreJitToCxxCalls, true);

    // Toggles whether unboxed plain objects can be created by the VM.
    SET_DEFAULT(disableUnboxedObjects, false);

    // Toggles the optimization whereby offsets are folded into loads and not
    // included in the bounds check.
    SET_DEFAULT(wasmFoldOffsets, true);
=======
  // Toggles the optimization whereby offsets are folded into loads and not
  // included in the bounds check.
  SET_DEFAULT(wasmFoldOffsets, true);
>>>>>>> upstream-releases

  // Controls whether two-tiered compilation should be requested when
  // compiling a new wasm module, independently of other heuristics, and
  // should be delayed to test both baseline and ion paths in compiled code,
  // as well as the transition from one tier to the other.
  SET_DEFAULT(wasmDelayTier2, false);

  // Until which wasm bytecode size should we accumulate functions, in order
  // to compile efficiently on helper threads. Baseline code compiles much
  // faster than Ion code so use scaled thresholds (see also bug 1320374).
  SET_DEFAULT(wasmBatchBaselineThreshold, 10000);
  SET_DEFAULT(wasmBatchIonThreshold, 1100);

#ifdef JS_TRACE_LOGGING
<<<<<<< HEAD
  // Toggles whether the traceLogger should be on or off.  In either case,
  // some data structures will always be created and initialized such as
  // the traceLoggerState.  However, unless this option is set to true
  // the traceLogger will not be recording any events.
  SET_DEFAULT(enableTraceLogger, false);
||||||| merged common ancestors
    // Toggles whether the traceLogger should be on or off.  In either case,
    // some data structures will always be created and initialized such as
    // the traceLoggerState.  However, unless this option is set to true
    // the traceLogger will not be recording any events.
    SET_DEFAULT(enableTraceLogger, false);
=======
  // Toggles whether the traceLogger should be on or off.  In either case,
  // some data structures will always be created and initialized such as
  // the traceLoggerState.  However, unless this option is set to true
  // the traceLogger will not be recording any events.
  SET_DEFAULT(enableTraceLogger, false);
#endif

  SET_DEFAULT(enableWasmJitExit, true);
  SET_DEFAULT(enableWasmJitEntry, true);
  SET_DEFAULT(enableWasmIonFastCalls, true);
#ifdef WASM_CODEGEN_DEBUG
  SET_DEFAULT(enableWasmImportCallSpew, false);
  SET_DEFAULT(enableWasmFuncCallSpew, false);
>>>>>>> upstream-releases
#endif
}

bool DefaultJitOptions::isSmallFunction(JSScript* script) const {
  return script->length() <= smallFunctionMaxBytecodeLength_;
}

<<<<<<< HEAD
void DefaultJitOptions::enableGvn(bool enable) { disableGvn = !enable; }
||||||| merged common ancestors
void
DefaultJitOptions::enableGvn(bool enable)
{
    disableGvn = !enable;
}
=======
void DefaultJitOptions::enableGvn(bool enable) { disableGvn = !enable; }

void DefaultJitOptions::setEagerIonCompilation() {
  baselineWarmUpThreshold = 0;
  normalIonWarmUpThreshold = 0;
  fullIonWarmUpThreshold = 0;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
void DefaultJitOptions::setEagerCompilation() {
  eagerCompilation = true;
  baselineWarmUpThreshold = 0;
  forcedDefaultIonWarmUpThreshold.reset();
  forcedDefaultIonWarmUpThreshold.emplace(0);
  forcedDefaultIonSmallFunctionWarmUpThreshold.reset();
  forcedDefaultIonSmallFunctionWarmUpThreshold.emplace(0);
||||||| merged common ancestors
void
DefaultJitOptions::setEagerCompilation()
{
    eagerCompilation = true;
    baselineWarmUpThreshold = 0;
    forcedDefaultIonWarmUpThreshold.reset();
    forcedDefaultIonWarmUpThreshold.emplace(0);
    forcedDefaultIonSmallFunctionWarmUpThreshold.reset();
    forcedDefaultIonSmallFunctionWarmUpThreshold.emplace(0);
=======
void DefaultJitOptions::setNormalIonWarmUpThreshold(uint32_t warmUpThreshold) {
  normalIonWarmUpThreshold = warmUpThreshold;

  if (fullIonWarmUpThreshold < normalIonWarmUpThreshold) {
    fullIonWarmUpThreshold = normalIonWarmUpThreshold;
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void DefaultJitOptions::setCompilerWarmUpThreshold(uint32_t warmUpThreshold) {
  forcedDefaultIonWarmUpThreshold.reset();
  forcedDefaultIonWarmUpThreshold.emplace(warmUpThreshold);
  forcedDefaultIonSmallFunctionWarmUpThreshold.reset();
  forcedDefaultIonSmallFunctionWarmUpThreshold.emplace(warmUpThreshold);

  // Undo eager compilation
  if (eagerCompilation && warmUpThreshold != 0) {
    jit::DefaultJitOptions defaultValues;
    eagerCompilation = false;
    baselineWarmUpThreshold = defaultValues.baselineWarmUpThreshold;
  }
||||||| merged common ancestors
void
DefaultJitOptions::setCompilerWarmUpThreshold(uint32_t warmUpThreshold)
{
    forcedDefaultIonWarmUpThreshold.reset();
    forcedDefaultIonWarmUpThreshold.emplace(warmUpThreshold);
    forcedDefaultIonSmallFunctionWarmUpThreshold.reset();
    forcedDefaultIonSmallFunctionWarmUpThreshold.emplace(warmUpThreshold);

    // Undo eager compilation
    if (eagerCompilation && warmUpThreshold != 0) {
        jit::DefaultJitOptions defaultValues;
        eagerCompilation = false;
        baselineWarmUpThreshold = defaultValues.baselineWarmUpThreshold;
    }
=======
void DefaultJitOptions::setFullIonWarmUpThreshold(uint32_t warmUpThreshold) {
  fullIonWarmUpThreshold = warmUpThreshold;

  if (normalIonWarmUpThreshold > fullIonWarmUpThreshold) {
    setNormalIonWarmUpThreshold(fullIonWarmUpThreshold);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void DefaultJitOptions::resetCompilerWarmUpThreshold() {
  forcedDefaultIonWarmUpThreshold.reset();
||||||| merged common ancestors
void
DefaultJitOptions::resetCompilerWarmUpThreshold()
{
    forcedDefaultIonWarmUpThreshold.reset();
=======
void DefaultJitOptions::resetNormalIonWarmUpThreshold() {
  jit::DefaultJitOptions defaultValues;
  setNormalIonWarmUpThreshold(defaultValues.normalIonWarmUpThreshold);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Undo eager compilation
  if (eagerCompilation) {
    jit::DefaultJitOptions defaultValues;
    eagerCompilation = false;
    baselineWarmUpThreshold = defaultValues.baselineWarmUpThreshold;
  }
||||||| merged common ancestors
    // Undo eager compilation
    if (eagerCompilation) {
        jit::DefaultJitOptions defaultValues;
        eagerCompilation = false;
        baselineWarmUpThreshold = defaultValues.baselineWarmUpThreshold;
    }
=======
void DefaultJitOptions::resetFullIonWarmUpThreshold() {
  jit::DefaultJitOptions defaultValues;
  setFullIonWarmUpThreshold(defaultValues.fullIonWarmUpThreshold);
>>>>>>> upstream-releases
}

}  // namespace jit
}  // namespace js
