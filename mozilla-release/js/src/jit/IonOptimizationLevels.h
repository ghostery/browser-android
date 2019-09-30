/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_IonOptimizationLevels_h
#define jit_IonOptimizationLevels_h

#include "mozilla/EnumeratedArray.h"

#include "jstypes.h"

#include "jit/JitOptions.h"
#include "js/TypeDecls.h"

namespace js {
namespace jit {

<<<<<<< HEAD
enum class OptimizationLevel : uint8_t { Normal, Wasm, Count, DontCompile };
||||||| merged common ancestors
enum class OptimizationLevel : uint8_t
{
    Normal,
    Wasm,
    Count,
    DontCompile
};
=======
// [SMDOC] Ion Optimization Levels
//
// Ion can do aggressive inlining, but inlining a lot of code will have a
// negative effect on compilation time and memory usage. It also means we spend
// more time in the slower Baseline code while compiling the Ion code
// off-thread or after an invalidation.
//
// To address this, Ion consists of two tiers:
//
// * Normal: the first tier (warm-up threshold of 1,000) only inlines small
//           functions one level deep. This tier also has recompile checks to
//           recompile the script when it becomes very hot.
//
// * Full: the second tier (warm-up threshold of 100,000) is only used for very
//         hot code so we can afford inlining a lot more code.
//
// See MRecompileCheck::RecompileCheckType for more info.

enum class OptimizationLevel : uint8_t {
  Normal,
  Full,
  Wasm,
  Count,
  DontCompile
};
>>>>>>> upstream-releases

#ifdef JS_JITSPEW
<<<<<<< HEAD
inline const char* OptimizationLevelString(OptimizationLevel level) {
  switch (level) {
    case OptimizationLevel::DontCompile:
      return "Optimization_DontCompile";
    case OptimizationLevel::Normal:
      return "Optimization_Normal";
    case OptimizationLevel::Wasm:
      return "Optimization_Wasm";
    case OptimizationLevel::Count:;
  }
  MOZ_CRASH("Invalid OptimizationLevel");
||||||| merged common ancestors
inline const char*
OptimizationLevelString(OptimizationLevel level)
{
    switch (level) {
      case OptimizationLevel::DontCompile:
        return "Optimization_DontCompile";
      case OptimizationLevel::Normal:
        return "Optimization_Normal";
      case OptimizationLevel::Wasm:
        return "Optimization_Wasm";
      case OptimizationLevel::Count:;
    }
    MOZ_CRASH("Invalid OptimizationLevel");
=======
inline const char* OptimizationLevelString(OptimizationLevel level) {
  switch (level) {
    case OptimizationLevel::DontCompile:
      return "Optimization_DontCompile";
    case OptimizationLevel::Normal:
      return "Optimization_Normal";
    case OptimizationLevel::Full:
      return "Optimization_Full";
    case OptimizationLevel::Wasm:
      return "Optimization_Wasm";
    case OptimizationLevel::Count:;
  }
  MOZ_CRASH("Invalid OptimizationLevel");
>>>>>>> upstream-releases
}
#endif

<<<<<<< HEAD
class OptimizationInfo {
 public:
  OptimizationLevel level_;

  // Toggles whether Effective Address Analysis is performed.
  bool eaa_;
||||||| merged common ancestors
class OptimizationInfo
{
  public:
    OptimizationLevel level_;

    // Toggles whether Effective Address Analysis is performed.
    bool eaa_;
=======
// Class representing the Ion optimization settings for an OptimizationLevel.
class OptimizationInfo {
  OptimizationLevel level_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether Alignment Mask Analysis is performed.
  bool ama_;
||||||| merged common ancestors
    // Toggles whether Alignment Mask Analysis is performed.
    bool ama_;
=======
  // Toggles whether Effective Address Analysis is performed.
  bool eaa_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether Edge Case Analysis is used.
  bool edgeCaseAnalysis_;
||||||| merged common ancestors
    // Toggles whether Edge Case Analysis is used.
    bool edgeCaseAnalysis_;
=======
  // Toggles whether Alignment Mask Analysis is performed.
  bool ama_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether redundant checks get removed.
  bool eliminateRedundantChecks_;
||||||| merged common ancestors
    // Toggles whether redundant checks get removed.
    bool eliminateRedundantChecks_;
=======
  // Toggles whether Edge Case Analysis is used.
  bool edgeCaseAnalysis_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether interpreted scripts get inlined.
  bool inlineInterpreted_;
||||||| merged common ancestors
    // Toggles whether interpreted scripts get inlined.
    bool inlineInterpreted_;
=======
  // Toggles whether redundant checks get removed.
  bool eliminateRedundantChecks_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether native scripts get inlined.
  bool inlineNative_;
||||||| merged common ancestors
    // Toggles whether native scripts get inlined.
    bool inlineNative_;
=======
  // Toggles whether interpreted scripts get inlined.
  bool inlineInterpreted_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether global value numbering is used.
  bool gvn_;
||||||| merged common ancestors
    // Toggles whether global value numbering is used.
    bool gvn_;
=======
  // Toggles whether native scripts get inlined.
  bool inlineNative_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether loop invariant code motion is performed.
  bool licm_;
||||||| merged common ancestors
    // Toggles whether loop invariant code motion is performed.
    bool licm_;
=======
  // Toggles whether global value numbering is used.
  bool gvn_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether Range Analysis is used.
  bool rangeAnalysis_;
||||||| merged common ancestors
    // Toggles whether Range Analysis is used.
    bool rangeAnalysis_;
=======
  // Toggles whether loop invariant code motion is performed.
  bool licm_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether loop unrolling is performed.
  bool loopUnrolling_;
||||||| merged common ancestors
    // Toggles whether loop unrolling is performed.
    bool loopUnrolling_;
=======
  // Toggles whether Range Analysis is used.
  bool rangeAnalysis_;
>>>>>>> upstream-releases

  // Toggles whether instruction reordering is performed.
  bool reordering_;

  // Toggles whether Truncation based on Range Analysis is used.
  bool autoTruncate_;

<<<<<<< HEAD
  // Toggles whether sincos is used.
  bool sincos_;
||||||| merged common ancestors
    // Toggles whether sincos is used.
    bool sincos_;
=======
  // Toggles whether sink is used.
  bool sink_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether sink is used.
  bool sink_;
||||||| merged common ancestors
    // Toggles whether sink is used.
    bool sink_;
=======
  // Describes which register allocator to use.
  IonRegisterAllocator registerAllocator_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Describes which register allocator to use.
  IonRegisterAllocator registerAllocator_;
||||||| merged common ancestors
    // Describes which register allocator to use.
    IonRegisterAllocator registerAllocator_;
=======
  // The maximum total bytecode size of an inline call site. We use a lower
  // value if off-thread compilation is not available, to avoid stalling the
  // main thread.
  uint32_t inlineMaxBytecodePerCallSiteHelperThread_;
  uint32_t inlineMaxBytecodePerCallSiteMainThread_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The maximum total bytecode size of an inline call site. We use a lower
  // value if off-thread compilation is not available, to avoid stalling the
  // main thread.
  uint32_t inlineMaxBytecodePerCallSiteHelperThread_;
  uint32_t inlineMaxBytecodePerCallSiteMainThread_;
||||||| merged common ancestors
    // The maximum total bytecode size of an inline call site. We use a lower
    // value if off-thread compilation is not available, to avoid stalling the
    // main thread.
    uint32_t inlineMaxBytecodePerCallSiteHelperThread_;
    uint32_t inlineMaxBytecodePerCallSiteMainThread_;
=======
  // The maximum value we allow for baselineScript->inlinedBytecodeLength_
  // when inlining.
  uint16_t inlineMaxCalleeInlinedBytecodeLength_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The maximum value we allow for baselineScript->inlinedBytecodeLength_
  // when inlining.
  uint16_t inlineMaxCalleeInlinedBytecodeLength_;
||||||| merged common ancestors
    // The maximum value we allow for baselineScript->inlinedBytecodeLength_
    // when inlining.
    uint16_t inlineMaxCalleeInlinedBytecodeLength_;
=======
  // The maximum bytecode length we'll inline in a single compilation.
  uint32_t inlineMaxTotalBytecodeLength_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The maximum bytecode length we'll inline in a single compilation.
  uint32_t inlineMaxTotalBytecodeLength_;
||||||| merged common ancestors
    // The maximum bytecode length we'll inline in a single compilation.
    uint32_t inlineMaxTotalBytecodeLength_;
=======
  // The maximum bytecode length the caller may have,
  // before we stop inlining large functions in that caller.
  uint32_t inliningMaxCallerBytecodeLength_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The maximum bytecode length the caller may have,
  // before we stop inlining large functions in that caller.
  uint32_t inliningMaxCallerBytecodeLength_;
||||||| merged common ancestors
    // The maximum bytecode length the caller may have,
    // before we stop inlining large functions in that caller.
    uint32_t inliningMaxCallerBytecodeLength_;
=======
  // The maximum inlining depth.
  uint32_t maxInlineDepth_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The maximum inlining depth.
  uint32_t maxInlineDepth_;
||||||| merged common ancestors
    // The maximum inlining depth.
    uint32_t maxInlineDepth_;
=======
  // Toggles whether scalar replacement is used.
  bool scalarReplacement_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Toggles whether scalar replacement is used.
  bool scalarReplacement_;
||||||| merged common ancestors
    // Toggles whether scalar replacement is used.
    bool scalarReplacement_;
=======
  // The maximum inlining depth for functions.
  //
  // Inlining small functions has almost no compiling overhead
  // and removes the otherwise needed call overhead.
  // The value is currently very low.
  // Actually it is only needed to make sure we don't blow out the stack.
  uint32_t smallFunctionMaxInlineDepth_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The maximum inlining depth for functions.
  //
  // Inlining small functions has almost no compiling overhead
  // and removes the otherwise needed call overhead.
  // The value is currently very low.
  // Actually it is only needed to make sure we don't blow out the stack.
  uint32_t smallFunctionMaxInlineDepth_;
||||||| merged common ancestors
    // The maximum inlining depth for functions.
    //
    // Inlining small functions has almost no compiling overhead
    // and removes the otherwise needed call overhead.
    // The value is currently very low.
    // Actually it is only needed to make sure we don't blow out the stack.
    uint32_t smallFunctionMaxInlineDepth_;
=======
  // How many invocations or loop iterations are needed before calls
  // are inlined, as a fraction of compilerWarmUpThreshold.
  double inliningWarmUpThresholdFactor_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // How many invocations or loop iterations are needed before functions
  // are compiled.
  uint32_t compilerWarmUpThreshold_;
||||||| merged common ancestors
    // How many invocations or loop iterations are needed before functions
    // are compiled.
    uint32_t compilerWarmUpThreshold_;
=======
  // How many invocations or loop iterations are needed before a function
  // is hot enough to recompile the outerScript to inline that function,
  // as a multiplication of inliningWarmUpThreshold.
  uint32_t inliningRecompileThresholdFactor_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Default compiler warmup threshold, unless it is overridden.
  static const uint32_t CompilerWarmupThreshold;

  // How many invocations or loop iterations are needed before small functions
  // are compiled.
  uint32_t compilerSmallFunctionWarmUpThreshold_;

  // Default small function compiler warmup threshold, unless it is overridden.
  static const uint32_t CompilerSmallFunctionWarmupThreshold;

  // How many invocations or loop iterations are needed before calls
  // are inlined, as a fraction of compilerWarmUpThreshold.
  double inliningWarmUpThresholdFactor_;

  // How many invocations or loop iterations are needed before a function
  // is hot enough to recompile the outerScript to inline that function,
  // as a multiplication of inliningWarmUpThreshold.
  uint32_t inliningRecompileThresholdFactor_;
||||||| merged common ancestors
    // Default compiler warmup threshold, unless it is overridden.
    static const uint32_t CompilerWarmupThreshold;

    // How many invocations or loop iterations are needed before small functions
    // are compiled.
    uint32_t compilerSmallFunctionWarmUpThreshold_;

    // Default small function compiler warmup threshold, unless it is overridden.
    static const uint32_t CompilerSmallFunctionWarmupThreshold;

    // How many invocations or loop iterations are needed before calls
    // are inlined, as a fraction of compilerWarmUpThreshold.
    double inliningWarmUpThresholdFactor_;

    // How many invocations or loop iterations are needed before a function
    // is hot enough to recompile the outerScript to inline that function,
    // as a multiplication of inliningWarmUpThreshold.
    uint32_t inliningRecompileThresholdFactor_;
=======
  uint32_t baseCompilerWarmUpThreshold() const {
    switch (level_) {
      case OptimizationLevel::Normal:
        return JitOptions.normalIonWarmUpThreshold;
      case OptimizationLevel::Full:
        if (!JitOptions.disableOptimizationLevels) {
          return JitOptions.fullIonWarmUpThreshold;
        }
        // Use the 'normal' threshold so Ion uses a single optimization level,
        // OptimizationLevel::Full.
        return JitOptions.normalIonWarmUpThreshold;
      case OptimizationLevel::DontCompile:
      case OptimizationLevel::Wasm:
      case OptimizationLevel::Count:
        break;
    }
    MOZ_CRASH("Unexpected optimization level");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  constexpr OptimizationInfo()
||||||| merged common ancestors
    constexpr OptimizationInfo()
=======
 public:
  constexpr OptimizationInfo()
>>>>>>> upstream-releases
      : level_(OptimizationLevel::Normal),
        eaa_(false),
        ama_(false),
        edgeCaseAnalysis_(false),
        eliminateRedundantChecks_(false),
        inlineInterpreted_(false),
        inlineNative_(false),
        gvn_(false),
        licm_(false),
        rangeAnalysis_(false),
        reordering_(false),
        autoTruncate_(false),
        sink_(false),
        registerAllocator_(RegisterAllocator_Backtracking),
        inlineMaxBytecodePerCallSiteHelperThread_(0),
        inlineMaxBytecodePerCallSiteMainThread_(0),
        inlineMaxCalleeInlinedBytecodeLength_(0),
        inlineMaxTotalBytecodeLength_(0),
        inliningMaxCallerBytecodeLength_(0),
        maxInlineDepth_(0),
        scalarReplacement_(false),
        smallFunctionMaxInlineDepth_(0),
        inliningWarmUpThresholdFactor_(0.0),
        inliningRecompileThresholdFactor_(0) {}

<<<<<<< HEAD
  void initNormalOptimizationInfo();
  void initWasmOptimizationInfo();

  OptimizationLevel level() const { return level_; }
||||||| merged common ancestors
    void initNormalOptimizationInfo();
    void initWasmOptimizationInfo();

    OptimizationLevel level() const {
        return level_;
    }
=======
  void initNormalOptimizationInfo();
  void initFullOptimizationInfo();
  void initWasmOptimizationInfo();
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool inlineInterpreted() const {
    return inlineInterpreted_ && !JitOptions.disableInlining;
  }
||||||| merged common ancestors
    bool inlineInterpreted() const {
        return inlineInterpreted_ && !JitOptions.disableInlining;
    }
=======
  OptimizationLevel level() const { return level_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool inlineNative() const {
    return inlineNative_ && !JitOptions.disableInlining;
  }
||||||| merged common ancestors
    bool inlineNative() const {
        return inlineNative_ && !JitOptions.disableInlining;
    }
=======
  bool inlineInterpreted() const {
    return inlineInterpreted_ && !JitOptions.disableInlining;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t compilerWarmUpThreshold(JSScript* script,
                                   jsbytecode* pc = nullptr) const;
||||||| merged common ancestors
    uint32_t compilerWarmUpThreshold(JSScript* script, jsbytecode* pc = nullptr) const;
=======
  bool inlineNative() const {
    return inlineNative_ && !JitOptions.disableInlining;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool gvnEnabled() const { return gvn_ && !JitOptions.disableGvn; }
||||||| merged common ancestors
    bool gvnEnabled() const {
        return gvn_ && !JitOptions.disableGvn;
    }
=======
  uint32_t compilerWarmUpThreshold(JSScript* script,
                                   jsbytecode* pc = nullptr) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool licmEnabled() const { return licm_ && !JitOptions.disableLicm; }
||||||| merged common ancestors
    bool licmEnabled() const {
        return licm_ && !JitOptions.disableLicm;
    }
=======
  uint32_t recompileWarmUpThreshold(JSScript* script, jsbytecode* pc) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool rangeAnalysisEnabled() const {
    return rangeAnalysis_ && !JitOptions.disableRangeAnalysis;
  }
||||||| merged common ancestors
    bool rangeAnalysisEnabled() const {
        return rangeAnalysis_ && !JitOptions.disableRangeAnalysis;
    }
=======
  bool gvnEnabled() const { return gvn_ && !JitOptions.disableGvn; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool loopUnrollingEnabled() const {
    return loopUnrolling_ && !JitOptions.disableLoopUnrolling;
  }
||||||| merged common ancestors
    bool loopUnrollingEnabled() const {
        return loopUnrolling_ && !JitOptions.disableLoopUnrolling;
    }
=======
  bool licmEnabled() const { return licm_ && !JitOptions.disableLicm; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool instructionReorderingEnabled() const {
    return reordering_ && !JitOptions.disableInstructionReordering;
  }
||||||| merged common ancestors
    bool instructionReorderingEnabled() const {
        return reordering_ && !JitOptions.disableInstructionReordering;
    }
=======
  bool rangeAnalysisEnabled() const {
    return rangeAnalysis_ && !JitOptions.disableRangeAnalysis;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool autoTruncateEnabled() const {
    return autoTruncate_ && rangeAnalysisEnabled();
  }
||||||| merged common ancestors
    bool autoTruncateEnabled() const {
        return autoTruncate_ && rangeAnalysisEnabled();
    }
=======
  bool instructionReorderingEnabled() const {
    return reordering_ && !JitOptions.disableInstructionReordering;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool sincosEnabled() const { return sincos_ && !JitOptions.disableSincos; }
||||||| merged common ancestors
    bool sincosEnabled() const {
        return sincos_ && !JitOptions.disableSincos;
    }
=======
  bool autoTruncateEnabled() const {
    return autoTruncate_ && rangeAnalysisEnabled();
  }
>>>>>>> upstream-releases

  bool sinkEnabled() const { return sink_ && !JitOptions.disableSink; }

  bool eaaEnabled() const { return eaa_ && !JitOptions.disableEaa; }

  bool amaEnabled() const { return ama_ && !JitOptions.disableAma; }

  bool edgeCaseAnalysisEnabled() const {
    return edgeCaseAnalysis_ && !JitOptions.disableEdgeCaseAnalysis;
  }

  bool eliminateRedundantChecksEnabled() const {
    return eliminateRedundantChecks_;
  }

  IonRegisterAllocator registerAllocator() const {
    return JitOptions.forcedRegisterAllocator.valueOr(registerAllocator_);
  }

  bool scalarReplacementEnabled() const {
    return scalarReplacement_ && !JitOptions.disableScalarReplacement;
  }

  uint32_t smallFunctionMaxInlineDepth() const {
    return smallFunctionMaxInlineDepth_;
  }

  bool isSmallFunction(JSScript* script) const;

  uint32_t maxInlineDepth() const { return maxInlineDepth_; }

  uint32_t inlineMaxBytecodePerCallSite(bool offThread) const {
    return (offThread || !JitOptions.limitScriptSize)
               ? inlineMaxBytecodePerCallSiteHelperThread_
               : inlineMaxBytecodePerCallSiteMainThread_;
<<<<<<< HEAD
  }

  uint16_t inlineMaxCalleeInlinedBytecodeLength() const {
    return inlineMaxCalleeInlinedBytecodeLength_;
  }

  uint32_t inlineMaxTotalBytecodeLength() const {
    return inlineMaxTotalBytecodeLength_;
  }

  uint32_t inliningMaxCallerBytecodeLength() const {
    return inliningMaxCallerBytecodeLength_;
  }

  uint32_t inliningWarmUpThreshold() const {
    uint32_t compilerWarmUpThreshold =
        JitOptions.forcedDefaultIonWarmUpThreshold.valueOr(
            compilerWarmUpThreshold_);
    return compilerWarmUpThreshold * inliningWarmUpThresholdFactor_;
  }

  uint32_t inliningRecompileThreshold() const {
    return inliningWarmUpThreshold() * inliningRecompileThresholdFactor_;
  }
||||||| merged common ancestors
    }

    uint16_t inlineMaxCalleeInlinedBytecodeLength() const {
        return inlineMaxCalleeInlinedBytecodeLength_;
    }

    uint32_t inlineMaxTotalBytecodeLength() const {
        return inlineMaxTotalBytecodeLength_;
    }

    uint32_t inliningMaxCallerBytecodeLength() const {
        return inliningMaxCallerBytecodeLength_;
    }

    uint32_t inliningWarmUpThreshold() const {
        uint32_t compilerWarmUpThreshold = JitOptions.forcedDefaultIonWarmUpThreshold
            .valueOr(compilerWarmUpThreshold_);
        return compilerWarmUpThreshold * inliningWarmUpThresholdFactor_;
    }

    uint32_t inliningRecompileThreshold() const {
        return inliningWarmUpThreshold() * inliningRecompileThresholdFactor_;
    }
=======
  }

  uint16_t inlineMaxCalleeInlinedBytecodeLength() const {
    return inlineMaxCalleeInlinedBytecodeLength_;
  }

  uint32_t inlineMaxTotalBytecodeLength() const {
    return inlineMaxTotalBytecodeLength_;
  }

  uint32_t inliningMaxCallerBytecodeLength() const {
    return inliningMaxCallerBytecodeLength_;
  }

  uint32_t inliningWarmUpThreshold() const {
    return baseCompilerWarmUpThreshold() * inliningWarmUpThresholdFactor_;
  }

  uint32_t inliningRecompileThreshold() const {
    return inliningWarmUpThreshold() * inliningRecompileThresholdFactor_;
  }
>>>>>>> upstream-releases
};

class OptimizationLevelInfo {
 private:
  mozilla::EnumeratedArray<OptimizationLevel, OptimizationLevel::Count,
                           OptimizationInfo>
      infos_;

 public:
  OptimizationLevelInfo();

  const OptimizationInfo* get(OptimizationLevel level) const {
    return &infos_[level];
  }

  OptimizationLevel nextLevel(OptimizationLevel level) const;
  OptimizationLevel firstLevel() const;
  bool isLastLevel(OptimizationLevel level) const;
  OptimizationLevel levelForScript(JSScript* script,
                                   jsbytecode* pc = nullptr) const;
};

extern const OptimizationLevelInfo IonOptimizations;

}  // namespace jit
}  // namespace js

#endif /* jit_IonOptimizationLevels_h */
