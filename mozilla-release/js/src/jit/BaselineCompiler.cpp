/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "jit/BaselineCompiler.h"

#include "mozilla/Casting.h"

#include "jit/BaselineIC.h"
#include "jit/BaselineJIT.h"
#include "jit/FixedList.h"
#include "jit/IonAnalysis.h"
#include "jit/JitcodeMap.h"
#include "jit/JitSpewer.h"
#include "jit/Linker.h"
#ifdef JS_ION_PERF
<<<<<<< HEAD
#include "jit/PerfSpewer.h"
||||||| merged common ancestors
# include "jit/PerfSpewer.h"
=======
#  include "jit/PerfSpewer.h"
>>>>>>> upstream-releases
#endif
#include "jit/SharedICHelpers.h"
#include "jit/VMFunctions.h"
#include "js/UniquePtr.h"
#include "vm/AsyncFunction.h"
#include "vm/AsyncIteration.h"
#include "vm/EnvironmentObject.h"
#include "vm/Interpreter.h"
#include "vm/JSFunction.h"
#include "vm/TraceLogging.h"
#include "vtune/VTuneWrapper.h"

#include "jit/BaselineFrameInfo-inl.h"
#include "jit/MacroAssembler-inl.h"
#include "jit/SharedICHelpers-inl.h"
#include "jit/VMFunctionList-inl.h"
#include "vm/Interpreter-inl.h"
#include "vm/JSScript-inl.h"
#include "vm/NativeObject-inl.h"
#include "vm/TypeInference-inl.h"

using namespace js;
using namespace js::jit;

<<<<<<< HEAD
using mozilla::AssertedCast;
using mozilla::Maybe;

namespace js {
namespace jit {

BaselineCompilerHandler::BaselineCompilerHandler(TempAllocator& alloc,
                                                 JSScript* script)
    : alloc_(alloc),
      script_(script),
      compileDebugInstrumentation_(script->isDebuggee()) {}

BaselineInterpreterHandler::BaselineInterpreterHandler() {}

template <typename Handler>
template <typename... HandlerArgs>
BaselineCodeGen<Handler>::BaselineCodeGen(JSContext* cx, TempAllocator& alloc,
                                          JSScript* script,
                                          HandlerArgs&&... args)
    : handler(std::forward<HandlerArgs>(args)...),
      cx(cx),
      script(script),
      pc(script->code()),
      ionCompileable_(jit::IsIonEnabled(cx) && CanIonCompileScript(cx, script)),
      alloc_(alloc),
      analysis_(alloc, script),
      frame(script, masm),
      traceLoggerToggleOffsets_(cx),
      icEntryIndex_(0),
      pushedBeforeCall_(0),
||||||| merged common ancestors
using mozilla::AssertedCast;

BaselineCompiler::BaselineCompiler(JSContext* cx, TempAllocator& alloc, JSScript* script)
  : cx(cx),
    script(script),
    pc(script->code()),
    ionCompileable_(jit::IsIonEnabled(cx) && CanIonCompileScript(cx, script)),
    compileDebugInstrumentation_(script->isDebuggee()),
    alloc_(alloc),
    analysis_(alloc, script),
    frame(script, masm),
    stubSpace_(),
    icEntries_(),
    pcMappingEntries_(),
    icLoadLabels_(),
    pushedBeforeCall_(0),
=======
using JS::TraceKind;

using mozilla::AssertedCast;
using mozilla::Maybe;

namespace js {
namespace jit {

BaselineCompilerHandler::BaselineCompilerHandler(JSContext* cx,
                                                 MacroAssembler& masm,
                                                 TempAllocator& alloc,
                                                 JSScript* script)
    : frame_(script, masm),
      alloc_(alloc),
      analysis_(alloc, script),
      script_(script),
      pc_(script->code()),
      icEntryIndex_(0),
      compileDebugInstrumentation_(script->isDebuggee()),
      ionCompileable_(jit::IsIonEnabled(cx) &&
                      CanIonCompileScript(cx, script)) {}

BaselineInterpreterHandler::BaselineInterpreterHandler(JSContext* cx,
                                                       MacroAssembler& masm)
    : frame_(masm) {}

template <typename Handler>
template <typename... HandlerArgs>
BaselineCodeGen<Handler>::BaselineCodeGen(JSContext* cx, HandlerArgs&&... args)
    : handler(cx, masm, std::forward<HandlerArgs>(args)...),
      cx(cx),
      frame(handler.frame()),
      traceLoggerToggleOffsets_(cx),
      profilerEnterFrameToggleOffset_(),
      profilerExitFrameToggleOffset_(),
      pushedBeforeCall_(0),
>>>>>>> upstream-releases
#ifdef DEBUG
      inCall_(false),
#endif
<<<<<<< HEAD
      modifiesArguments_(false) {
}

BaselineCompiler::BaselineCompiler(JSContext* cx, TempAllocator& alloc,
                                   JSScript* script)
    : BaselineCodeGen(cx, alloc, script,
                      /* HandlerArgs = */ alloc, script),
      pcMappingEntries_(),
      profilerPushToggleOffset_(),
      profilerEnterFrameToggleOffset_(),
      profilerExitFrameToggleOffset_(),
      traceLoggerScriptTextIdOffset_() {
||||||| merged common ancestors
    profilerPushToggleOffset_(),
    profilerEnterFrameToggleOffset_(),
    profilerExitFrameToggleOffset_(),
    traceLoggerToggleOffsets_(cx),
    traceLoggerScriptTextIdOffset_(),
    yieldAndAwaitOffsets_(cx),
    modifiesArguments_(false)
{
=======
      modifiesArguments_(false) {
}

BaselineCompiler::BaselineCompiler(JSContext* cx, TempAllocator& alloc,
                                   JSScript* script)
    : BaselineCodeGen(cx, /* HandlerArgs = */ alloc, script),
      pcMappingEntries_(),
      profilerPushToggleOffset_(),
      traceLoggerScriptTextIdOffset_() {
>>>>>>> upstream-releases
#ifdef JS_CODEGEN_NONE
  MOZ_CRASH();
#endif
}

<<<<<<< HEAD
bool BaselineCompilerHandler::init() {
  uint32_t len = script_->length();
||||||| merged common ancestors
bool
BaselineCompiler::init()
{
    if (!analysis_.init(alloc_, cx->caches().gsnCache)) {
        return false;
    }
=======
BaselineInterpreterGenerator::BaselineInterpreterGenerator(JSContext* cx)
    : BaselineCodeGen(cx /* no handlerArgs */) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!labels_.init(alloc_, len)) {
    return false;
  }
||||||| merged common ancestors
    if (!labels_.init(alloc_, script->length())) {
        return false;
    }
=======
bool BaselineCompilerHandler::init(JSContext* cx) {
  if (!analysis_.init(alloc_, cx->caches().gsnCache)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (size_t i = 0; i < len; i++) {
    new (&labels_[i]) Label();
  }
||||||| merged common ancestors
    for (size_t i = 0; i < script->length(); i++) {
        new (&labels_[i]) Label();
    }
=======
  uint32_t len = script_->length();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    if (!frame.init(alloc_)) {
        return false;
    }
=======
  if (!labels_.init(alloc_, len)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCompiler::init() {
  if (!analysis_.init(alloc_, cx->caches().gsnCache)) {
    return false;
  }

  if (!handler.init()) {
    return false;
  }

  if (!frame.init(alloc_)) {
    return false;
  }

  return true;
}
||||||| merged common ancestors
    return true;
}
=======
  for (size_t i = 0; i < len; i++) {
    new (&labels_[i]) Label();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCompiler::addPCMappingEntry(bool addIndexEntry) {
  // Don't add multiple entries for a single pc.
  size_t nentries = pcMappingEntries_.length();
  if (nentries > 0 &&
      pcMappingEntries_[nentries - 1].pcOffset == script->pcToOffset(pc)) {
    return true;
  }
||||||| merged common ancestors
bool
BaselineCompiler::addPCMappingEntry(bool addIndexEntry)
{
    // Don't add multiple entries for a single pc.
    size_t nentries = pcMappingEntries_.length();
    if (nentries > 0 && pcMappingEntries_[nentries - 1].pcOffset == script->pcToOffset(pc)) {
        return true;
    }
=======
  if (!frame_.init(alloc_)) {
    return false;
  }

  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  PCMappingEntry entry;
  entry.pcOffset = script->pcToOffset(pc);
  entry.nativeOffset = masm.currentOffset();
  entry.slotInfo = getStackTopSlotInfo();
  entry.addIndexEntry = addIndexEntry;
||||||| merged common ancestors
    PCMappingEntry entry;
    entry.pcOffset = script->pcToOffset(pc);
    entry.nativeOffset = masm.currentOffset();
    entry.slotInfo = getStackTopSlotInfo();
    entry.addIndexEntry = addIndexEntry;
=======
bool BaselineCompiler::init() {
  if (!handler.init(cx)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return pcMappingEntries_.append(entry);
||||||| merged common ancestors
    return pcMappingEntries_.append(entry);
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
MethodStatus BaselineCompiler::compile() {
  JitSpew(JitSpew_BaselineScripts, "Baseline compiling script %s:%u:%u (%p)",
          script->filename(), script->lineno(), script->column(), script);
||||||| merged common ancestors
MethodStatus
BaselineCompiler::compile()
{
    JitSpew(JitSpew_BaselineScripts, "Baseline compiling script %s:%u:%u (%p)",
            script->filename(), script->lineno(), script->column(), script);
=======
bool BaselineCompiler::addPCMappingEntry(bool addIndexEntry) {
  // Don't add multiple entries for a single pc.
  size_t nentries = pcMappingEntries_.length();
  uint32_t pcOffset = handler.script()->pcToOffset(handler.pc());
  if (nentries > 0 && pcMappingEntries_[nentries - 1].pcOffset == pcOffset) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JitSpew(JitSpew_Codegen, "# Emitting baseline code for script %s:%u:%u",
          script->filename(), script->lineno(), script->column());
||||||| merged common ancestors
    JitSpew(JitSpew_Codegen, "# Emitting baseline code for script %s:%u:%u",
            script->filename(), script->lineno(), script->column());
=======
  PCMappingEntry entry;
  entry.pcOffset = pcOffset;
  entry.nativeOffset = masm.currentOffset();
  entry.slotInfo = getStackTopSlotInfo();
  entry.addIndexEntry = addIndexEntry;
>>>>>>> upstream-releases

<<<<<<< HEAD
  TraceLoggerThread* logger = TraceLoggerForCurrentThread(cx);
  TraceLoggerEvent scriptEvent(TraceLogger_AnnotateScripts, script);
  AutoTraceLog logScript(logger, scriptEvent);
  AutoTraceLog logCompile(logger, TraceLogger_BaselineCompilation);
||||||| merged common ancestors
    TraceLoggerThread* logger = TraceLoggerForCurrentThread(cx);
    TraceLoggerEvent scriptEvent(TraceLogger_AnnotateScripts, script);
    AutoTraceLog logScript(logger, scriptEvent);
    AutoTraceLog logCompile(logger, TraceLogger_BaselineCompilation);
=======
  return pcMappingEntries_.append(entry);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoKeepTypeScripts keepTypes(cx);
  if (!script->ensureHasTypes(cx, keepTypes) ||
      !script->ensureHasAnalyzedArgsUsage(cx)) {
    return Method_Error;
  }
||||||| merged common ancestors
    AutoKeepTypeScripts keepTypes(cx);
    if (!script->ensureHasTypes(cx, keepTypes) || !script->ensureHasAnalyzedArgsUsage(cx)) {
        return Method_Error;
    }
=======
MethodStatus BaselineCompiler::compile() {
  JSScript* script = handler.script();
  JitSpew(JitSpew_BaselineScripts, "Baseline compiling script %s:%u:%u (%p)",
          script->filename(), script->lineno(), script->column(), script);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // When code coverage is only enabled for optimizations, or when a Debugger
  // set the collectCoverageInfo flag, we have to create the ScriptCounts if
  // they do not exist.
  if (!script->hasScriptCounts() && cx->realm()->collectCoverage()) {
    if (!script->initScriptCounts(cx)) {
      return Method_Error;
    }
  }
||||||| merged common ancestors
    // When code coverage is only enabled for optimizations, or when a Debugger
    // set the collectCoverageInfo flag, we have to create the ScriptCounts if
    // they do not exist.
    if (!script->hasScriptCounts() && cx->realm()->collectCoverage()) {
        if (!script->initScriptCounts(cx)) {
            return Method_Error;
        }
    }
=======
  JitSpew(JitSpew_Codegen, "# Emitting baseline code for script %s:%u:%u",
          script->filename(), script->lineno(), script->column());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Pin analysis info during compilation.
  AutoEnterAnalysis autoEnterAnalysis(cx);
||||||| merged common ancestors
    // Pin analysis info during compilation.
    AutoEnterAnalysis autoEnterAnalysis(cx);
=======
  TraceLoggerThread* logger = TraceLoggerForCurrentThread(cx);
  TraceLoggerEvent scriptEvent(TraceLogger_AnnotateScripts, script);
  AutoTraceLog logScript(logger, scriptEvent);
  AutoTraceLog logCompile(logger, TraceLogger_BaselineCompilation);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(!script->hasBaselineScript());
||||||| merged common ancestors
    MOZ_ASSERT(!script->hasBaselineScript());
=======
  AutoKeepJitScripts keepJitScript(cx);
  if (!script->ensureHasJitScript(cx, keepJitScript)) {
    return Method_Error;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!emitPrologue()) {
    return Method_Error;
  }
||||||| merged common ancestors
    if (!emitPrologue()) {
        return Method_Error;
    }
=======
  // When code coverage is only enabled for optimizations, or when a Debugger
  // set the collectCoverageInfo flag, we have to create the ScriptCounts if
  // they do not exist.
  if (!script->hasScriptCounts() && cx->realm()->collectCoverage()) {
    if (!script->initScriptCounts(cx)) {
      return Method_Error;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MethodStatus status = emitBody();
  if (status != Method_Compiled) {
    return status;
  }
||||||| merged common ancestors
    MethodStatus status = emitBody();
    if (status != Method_Compiled) {
        return status;
    }
=======
  // Pin analysis info during compilation.
  AutoEnterAnalysis autoEnterAnalysis(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!emitEpilogue()) {
    return Method_Error;
  }
||||||| merged common ancestors
    if (!emitEpilogue()) {
        return Method_Error;
    }
=======
  MOZ_ASSERT(!script->hasBaselineScript());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!emitOutOfLinePostBarrierSlot()) {
    return Method_Error;
  }
||||||| merged common ancestors
    if (!emitOutOfLinePostBarrierSlot()) {
        return Method_Error;
    }
=======
  if (!emitPrologue()) {
    return Method_Error;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Linker linker(masm);
  if (masm.oom()) {
    ReportOutOfMemory(cx);
    return Method_Error;
  }
||||||| merged common ancestors
    Linker linker(masm);
    if (masm.oom()) {
        ReportOutOfMemory(cx);
        return Method_Error;
    }
=======
  MethodStatus status = emitBody();
  if (status != Method_Compiled) {
    return status;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoFlushICache afc("Baseline");
  JitCode* code = linker.newCode(cx, CodeKind::Baseline);
  if (!code) {
    return Method_Error;
  }
||||||| merged common ancestors
    AutoFlushICache afc("Baseline");
    JitCode* code = linker.newCode(cx, CodeKind::Baseline);
    if (!code) {
        return Method_Error;
    }
=======
  if (!emitEpilogue()) {
    return Method_Error;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Rooted<EnvironmentObject*> templateEnv(cx);
  if (script->functionNonDelazifying()) {
    RootedFunction fun(cx, script->functionNonDelazifying());
||||||| merged common ancestors
    Rooted<EnvironmentObject*> templateEnv(cx);
    if (script->functionNonDelazifying()) {
        RootedFunction fun(cx, script->functionNonDelazifying());

        if (fun->needsNamedLambdaEnvironment()) {
            templateEnv = NamedLambdaObject::createTemplateObject(cx, fun, gc::TenuredHeap);
            if (!templateEnv) {
                return Method_Error;
            }
        }

        if (fun->needsCallObject()) {
            RootedScript scriptRoot(cx, script);
            templateEnv = CallObject::createTemplateObject(cx, scriptRoot, templateEnv,
                                                           gc::TenuredHeap);
            if (!templateEnv) {
                return Method_Error;
            }
        }
    }
=======
  if (!emitOutOfLinePostBarrierSlot()) {
    return Method_Error;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (fun->needsNamedLambdaEnvironment()) {
      templateEnv =
          NamedLambdaObject::createTemplateObject(cx, fun, gc::TenuredHeap);
      if (!templateEnv) {
        return Method_Error;
      }
    }
||||||| merged common ancestors
    // Encode the pc mapping table. See PCMappingIndexEntry for
    // more information.
    Vector<PCMappingIndexEntry> pcMappingIndexEntries(cx);
    CompactBufferWriter pcEntries;
    uint32_t previousOffset = 0;

    for (size_t i = 0; i < pcMappingEntries_.length(); i++) {
        PCMappingEntry& entry = pcMappingEntries_[i];

        if (entry.addIndexEntry) {
            PCMappingIndexEntry indexEntry;
            indexEntry.pcOffset = entry.pcOffset;
            indexEntry.nativeOffset = entry.nativeOffset;
            indexEntry.bufferOffset = pcEntries.length();
            if (!pcMappingIndexEntries.append(indexEntry)) {
                ReportOutOfMemory(cx);
                return Method_Error;
            }
            previousOffset = entry.nativeOffset;
        }

        // Use the high bit of the SlotInfo byte to indicate the
        // native code offset (relative to the previous op) > 0 and
        // comes next in the buffer.
        MOZ_ASSERT((entry.slotInfo.toByte() & 0x80) == 0);

        if (entry.nativeOffset == previousOffset) {
            pcEntries.writeByte(entry.slotInfo.toByte());
        } else {
            MOZ_ASSERT(entry.nativeOffset > previousOffset);
            pcEntries.writeByte(0x80 | entry.slotInfo.toByte());
            pcEntries.writeUnsigned(entry.nativeOffset - previousOffset);
        }

        previousOffset = entry.nativeOffset;
    }
=======
  Linker linker(masm, "Baseline");
  if (masm.oom()) {
    ReportOutOfMemory(cx);
    return Method_Error;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (fun->needsCallObject()) {
      RootedScript scriptRoot(cx, script);
      templateEnv = CallObject::createTemplateObject(
          cx, scriptRoot, templateEnv, gc::TenuredHeap);
      if (!templateEnv) {
||||||| merged common ancestors
    if (pcEntries.oom()) {
        ReportOutOfMemory(cx);
=======
  JitCode* code = linker.newCode(cx, CodeKind::Baseline);
  if (!code) {
    return Method_Error;
  }

  Rooted<EnvironmentObject*> templateEnv(cx);
  if (script->functionNonDelazifying()) {
    RootedFunction fun(cx, script->functionNonDelazifying());

    if (fun->needsNamedLambdaEnvironment()) {
      templateEnv =
          NamedLambdaObject::createTemplateObject(cx, fun, gc::TenuredHeap);
      if (!templateEnv) {
>>>>>>> upstream-releases
        return Method_Error;
      }
    }
  }

  // Encode the pc mapping table. See PCMappingIndexEntry for
  // more information.
  Vector<PCMappingIndexEntry> pcMappingIndexEntries(cx);
  CompactBufferWriter pcEntries;
  uint32_t previousOffset = 0;

<<<<<<< HEAD
  for (size_t i = 0; i < pcMappingEntries_.length(); i++) {
    PCMappingEntry& entry = pcMappingEntries_[i];

    if (entry.addIndexEntry) {
      PCMappingIndexEntry indexEntry;
      indexEntry.pcOffset = entry.pcOffset;
      indexEntry.nativeOffset = entry.nativeOffset;
      indexEntry.bufferOffset = pcEntries.length();
      if (!pcMappingIndexEntries.append(indexEntry)) {
        ReportOutOfMemory(cx);
||||||| merged common ancestors
    // Note: There is an extra entry in the bytecode type map for the search hint, see below.
    size_t bytecodeTypeMapEntries = script->nTypeSets() + 1;
    UniquePtr<BaselineScript> baselineScript(
        BaselineScript::New(script, prologueOffset_.offset(),
                            epilogueOffset_.offset(),
                            profilerEnterFrameToggleOffset_.offset(),
                            profilerExitFrameToggleOffset_.offset(),
                            postDebugPrologueOffset_.offset(),
                            icEntries_.length(),
                            pcMappingIndexEntries.length(),
                            pcEntries.length(),
                            bytecodeTypeMapEntries,
                            yieldAndAwaitOffsets_.length(),
                            traceLoggerToggleOffsets_.length()),
        JS::DeletePolicy<BaselineScript>(cx->runtime()));
    if (!baselineScript) {
        ReportOutOfMemory(cx);
=======
    if (fun->needsCallObject()) {
      RootedScript scriptRoot(cx, script);
      templateEnv = CallObject::createTemplateObject(
          cx, scriptRoot, templateEnv, gc::TenuredHeap);
      if (!templateEnv) {
>>>>>>> upstream-releases
        return Method_Error;
<<<<<<< HEAD
      }
      previousOffset = entry.nativeOffset;
||||||| merged common ancestors
=======
      }
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
    // Use the high bit of the SlotInfo byte to indicate the
    // native code offset (relative to the previous op) > 0 and
    // comes next in the buffer.
    MOZ_ASSERT((entry.slotInfo.toByte() & 0x80) == 0);
||||||| merged common ancestors
    baselineScript->setMethod(code);
    baselineScript->setTemplateEnvironment(templateEnv);
=======
  // Encode the pc mapping table. See PCMappingIndexEntry for
  // more information.
  Vector<PCMappingIndexEntry> pcMappingIndexEntries(cx);
  CompactBufferWriter pcEntries;
  uint32_t previousOffset = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (entry.nativeOffset == previousOffset) {
      pcEntries.writeByte(entry.slotInfo.toByte());
    } else {
      MOZ_ASSERT(entry.nativeOffset > previousOffset);
      pcEntries.writeByte(0x80 | entry.slotInfo.toByte());
      pcEntries.writeUnsigned(entry.nativeOffset - previousOffset);
    }

    previousOffset = entry.nativeOffset;
  }

  if (pcEntries.oom()) {
    ReportOutOfMemory(cx);
    return Method_Error;
  }

  // Note: There is an extra entry in the bytecode type map for the search
  // hint, see below.
  size_t bytecodeTypeMapEntries = script->nTypeSets() + 1;
  size_t resumeEntries =
      script->hasResumeOffsets() ? script->resumeOffsets().size() : 0;
  UniquePtr<BaselineScript> baselineScript(
      BaselineScript::New(
          script, bailoutPrologueOffset_.offset(),
          debugOsrPrologueOffset_.offset(), debugOsrEpilogueOffset_.offset(),
          profilerEnterFrameToggleOffset_.offset(),
          profilerExitFrameToggleOffset_.offset(), retAddrEntries_.length(),
          pcMappingIndexEntries.length(), pcEntries.length(),
          bytecodeTypeMapEntries, resumeEntries,
          traceLoggerToggleOffsets_.length()),
      JS::DeletePolicy<BaselineScript>(cx->runtime()));
  if (!baselineScript) {
    ReportOutOfMemory(cx);
    return Method_Error;
  }

  baselineScript->setMethod(code);
  baselineScript->setTemplateEnvironment(templateEnv);

  JitSpew(JitSpew_BaselineScripts,
          "Created BaselineScript %p (raw %p) for %s:%u:%u",
          (void*)baselineScript.get(), (void*)code->raw(), script->filename(),
          script->lineno(), script->column());

  MOZ_ASSERT(pcMappingIndexEntries.length() > 0);
  baselineScript->copyPCMappingIndexEntries(&pcMappingIndexEntries[0]);

  MOZ_ASSERT(pcEntries.length() > 0);
  baselineScript->copyPCMappingEntries(pcEntries);

  // Copy RetAddrEntries.
  if (retAddrEntries_.length() > 0) {
    baselineScript->copyRetAddrEntries(script, &retAddrEntries_[0]);
  }

  // If profiler instrumentation is enabled, toggle instrumentation on.
  if (cx->runtime()->jitRuntime()->isProfilerInstrumentationEnabled(
          cx->runtime())) {
    baselineScript->toggleProfilerInstrumentation(true);
  }

  if (modifiesArguments_) {
    baselineScript->setModifiesArguments();
  }
  if (analysis_.usesEnvironmentChain()) {
    baselineScript->setUsesEnvironmentChain();
  }
||||||| merged common ancestors
    JitSpew(JitSpew_BaselineScripts, "Created BaselineScript %p (raw %p) for %s:%u:%u",
            (void*) baselineScript.get(), (void*) code->raw(),
            script->filename(), script->lineno(), script->column());
=======
  for (size_t i = 0; i < pcMappingEntries_.length(); i++) {
    PCMappingEntry& entry = pcMappingEntries_[i];
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef JS_TRACE_LOGGING
  // Initialize the tracelogger instrumentation.
  baselineScript->initTraceLogger(script, traceLoggerToggleOffsets_);
#endif

  uint32_t* bytecodeMap = baselineScript->bytecodeTypeMap();
  FillBytecodeTypeMap(script, bytecodeMap);

  // The last entry in the last index found, and is used to avoid binary
  // searches for the sought entry when queries are in linear order.
  bytecodeMap[script->nTypeSets()] = 0;

  // Compute yield/await native resume addresses.
  baselineScript->computeResumeNativeOffsets(script);

  if (compileDebugInstrumentation()) {
    baselineScript->setHasDebugInstrumentation();
  }
||||||| merged common ancestors
    MOZ_ASSERT(pcMappingIndexEntries.length() > 0);
    baselineScript->copyPCMappingIndexEntries(&pcMappingIndexEntries[0]);

    MOZ_ASSERT(pcEntries.length() > 0);
    baselineScript->copyPCMappingEntries(pcEntries);

    // Copy IC entries
    if (icEntries_.length()) {
        baselineScript->copyICEntries(script, &icEntries_[0]);
    }

    // Adopt fallback stubs from the compiler into the baseline script.
    baselineScript->adoptFallbackStubs(&stubSpace_);

    // If profiler instrumentation is enabled, toggle instrumentation on.
    if (cx->runtime()->jitRuntime()->isProfilerInstrumentationEnabled(cx->runtime())) {
        baselineScript->toggleProfilerInstrumentation(true);
    }
=======
    if (entry.addIndexEntry) {
      PCMappingIndexEntry indexEntry;
      indexEntry.pcOffset = entry.pcOffset;
      indexEntry.nativeOffset = entry.nativeOffset;
      indexEntry.bufferOffset = pcEntries.length();
      if (!pcMappingIndexEntries.append(indexEntry)) {
        ReportOutOfMemory(cx);
        return Method_Error;
      }
      previousOffset = entry.nativeOffset;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Always register a native => bytecode mapping entry, since profiler can be
  // turned on with baseline jitcode on stack, and baseline jitcode cannot be
  // invalidated.
  {
    JitSpew(JitSpew_Profiling,
            "Added JitcodeGlobalEntry for baseline script %s:%u:%u (%p)",
            script->filename(), script->lineno(), script->column(),
            baselineScript.get());
||||||| merged common ancestors
    // Patch IC loads using IC entries.
    for (size_t i = 0; i < icLoadLabels_.length(); i++) {
        CodeOffset label = icLoadLabels_[i].label;
        size_t icEntry = icLoadLabels_[i].icEntry;
        ICEntry* entryAddr = &(baselineScript->icEntry(icEntry));
        Assembler::PatchDataWithValueCheck(CodeLocationLabel(code, label),
                                           ImmPtr(entryAddr),
                                           ImmPtr((void*)-1));
    }
=======
    // Use the high bit of the SlotInfo byte to indicate the
    // native code offset (relative to the previous op) > 0 and
    // comes next in the buffer.
    MOZ_ASSERT((entry.slotInfo.toByte() & 0x80) == 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Generate profiling string.
    char* str = JitcodeGlobalEntry::createScriptString(cx, script);
    if (!str) {
      return Method_Error;
    }
||||||| merged common ancestors
    if (modifiesArguments_) {
        baselineScript->setModifiesArguments();
    }
    if (analysis_.usesEnvironmentChain()) {
        baselineScript->setUsesEnvironmentChain();
    }
=======
    if (entry.nativeOffset == previousOffset) {
      pcEntries.writeByte(entry.slotInfo.toByte());
    } else {
      MOZ_ASSERT(entry.nativeOffset > previousOffset);
      pcEntries.writeByte(0x80 | entry.slotInfo.toByte());
      pcEntries.writeUnsigned(entry.nativeOffset - previousOffset);
    }

    previousOffset = entry.nativeOffset;
  }

  if (pcEntries.oom()) {
    ReportOutOfMemory(cx);
    return Method_Error;
  }

  size_t resumeEntries =
      script->hasResumeOffsets() ? script->resumeOffsets().size() : 0;
  UniquePtr<BaselineScript> baselineScript(
      BaselineScript::New(script, bailoutPrologueOffset_.offset(),
                          warmUpCheckPrologueOffset_.offset(),
                          debugOsrPrologueOffset_.offset(),
                          debugOsrEpilogueOffset_.offset(),
                          profilerEnterFrameToggleOffset_.offset(),
                          profilerExitFrameToggleOffset_.offset(),
                          handler.retAddrEntries().length(),
                          pcMappingIndexEntries.length(), pcEntries.length(),
                          resumeEntries, traceLoggerToggleOffsets_.length()),
      JS::DeletePolicy<BaselineScript>(cx->runtime()));
  if (!baselineScript) {
    ReportOutOfMemory(cx);
    return Method_Error;
  }

  baselineScript->setMethod(code);
  baselineScript->setTemplateEnvironment(templateEnv);

  JitSpew(JitSpew_BaselineScripts,
          "Created BaselineScript %p (raw %p) for %s:%u:%u",
          (void*)baselineScript.get(), (void*)code->raw(), script->filename(),
          script->lineno(), script->column());

  MOZ_ASSERT(pcMappingIndexEntries.length() > 0);
  baselineScript->copyPCMappingIndexEntries(&pcMappingIndexEntries[0]);

  MOZ_ASSERT(pcEntries.length() > 0);
  baselineScript->copyPCMappingEntries(pcEntries);

  // Copy RetAddrEntries.
  if (handler.retAddrEntries().length() > 0) {
    baselineScript->copyRetAddrEntries(script,
                                       handler.retAddrEntries().begin());
  }

  // If profiler instrumentation is enabled, toggle instrumentation on.
  if (cx->runtime()->jitRuntime()->isProfilerInstrumentationEnabled(
          cx->runtime())) {
    baselineScript->toggleProfilerInstrumentation(true);
  }

  if (modifiesArguments_) {
    baselineScript->setModifiesArguments();
  }
  if (handler.analysis().usesEnvironmentChain()) {
    baselineScript->setUsesEnvironmentChain();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    JitcodeGlobalEntry::BaselineEntry entry;
    entry.init(code, code->raw(), code->rawEnd(), script, str);
||||||| merged common ancestors
#ifdef JS_TRACE_LOGGING
    // Initialize the tracelogger instrumentation.
    baselineScript->initTraceLogger(script, traceLoggerToggleOffsets_);
#endif

    uint32_t* bytecodeMap = baselineScript->bytecodeTypeMap();
    FillBytecodeTypeMap(script, bytecodeMap);

    // The last entry in the last index found, and is used to avoid binary
    // searches for the sought entry when queries are in linear order.
    bytecodeMap[script->nTypeSets()] = 0;
=======
#ifdef JS_TRACE_LOGGING
  // Initialize the tracelogger instrumentation.
  if (JS::TraceLoggerSupported()) {
    baselineScript->initTraceLogger(script, traceLoggerToggleOffsets_);
  }
#endif

  // Compute yield/await native resume addresses.
  baselineScript->computeResumeNativeOffsets(script);

  if (compileDebugInstrumentation()) {
    baselineScript->setHasDebugInstrumentation();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    JitcodeGlobalTable* globalTable =
        cx->runtime()->jitRuntime()->getJitcodeGlobalTable();
    if (!globalTable->addEntry(entry)) {
      entry.destroy();
      ReportOutOfMemory(cx);
      return Method_Error;
||||||| merged common ancestors
    baselineScript->copyYieldAndAwaitEntries(script, yieldAndAwaitOffsets_);

    if (compileDebugInstrumentation_) {
        baselineScript->setHasDebugInstrumentation();
=======
  // Always register a native => bytecode mapping entry, since profiler can be
  // turned on with baseline jitcode on stack, and baseline jitcode cannot be
  // invalidated.
  {
    JitSpew(JitSpew_Profiling,
            "Added JitcodeGlobalEntry for baseline script %s:%u:%u (%p)",
            script->filename(), script->lineno(), script->column(),
            baselineScript.get());

    // Generate profiling string.
    UniqueChars str = GeckoProfilerRuntime::allocProfileString(cx, script);
    if (!str) {
      return Method_Error;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    // Mark the jitcode as having a bytecode map.
    code->setHasBytecodeMap();
  }
||||||| merged common ancestors
    // Always register a native => bytecode mapping entry, since profiler can be
    // turned on with baseline jitcode on stack, and baseline jitcode cannot be invalidated.
    {
        JitSpew(JitSpew_Profiling, "Added JitcodeGlobalEntry for baseline script %s:%u:%u (%p)",
                    script->filename(), script->lineno(), script->column(), baselineScript.get());

        // Generate profiling string.
        char* str = JitcodeGlobalEntry::createScriptString(cx, script);
        if (!str) {
            return Method_Error;
        }

        JitcodeGlobalEntry::BaselineEntry entry;
        entry.init(code, code->raw(), code->rawEnd(), script, str);

        JitcodeGlobalTable* globalTable = cx->runtime()->jitRuntime()->getJitcodeGlobalTable();
        if (!globalTable->addEntry(entry)) {
            entry.destroy();
            ReportOutOfMemory(cx);
            return Method_Error;
        }

        // Mark the jitcode as having a bytecode map.
        code->setHasBytecodeMap();
    }
=======
    JitcodeGlobalEntry::BaselineEntry entry;
    entry.init(code, code->raw(), code->rawEnd(), script, str.release());

    JitcodeGlobalTable* globalTable =
        cx->runtime()->jitRuntime()->getJitcodeGlobalTable();
    if (!globalTable->addEntry(entry)) {
      entry.destroy();
      ReportOutOfMemory(cx);
      return Method_Error;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  script->setBaselineScript(cx->runtime(), baselineScript.release());
||||||| merged common ancestors
    script->setBaselineScript(cx->runtime(), baselineScript.release());
=======
    // Mark the jitcode as having a bytecode map.
    code->setHasBytecodeMap();
  }

  script->setBaselineScript(cx->runtime(), baselineScript.release());
>>>>>>> upstream-releases

#ifdef JS_ION_PERF
  writePerfSpewerBaselineProfile(script, code);
#endif

#ifdef MOZ_VTUNE
  vtune::MarkScript(code, script, "baseline");
#endif

  return Method_Compiled;
}

<<<<<<< HEAD
void BaselineCompiler::emitInitializeLocals() {
  // Initialize all locals to |undefined|. Lexical bindings are temporal
  // dead zoned in bytecode.

  size_t n = frame.nlocals();
  if (n == 0) {
    return;
  }

  // Use R0 to minimize code size. If the number of locals to push is <
  // LOOP_UNROLL_FACTOR, then the initialization pushes are emitted directly
  // and inline.  Otherwise, they're emitted in a partially unrolled loop.
  static const size_t LOOP_UNROLL_FACTOR = 4;
  size_t toPushExtra = n % LOOP_UNROLL_FACTOR;

  masm.moveValue(UndefinedValue(), R0);
||||||| merged common ancestors
void
BaselineCompiler::emitInitializeLocals()
{
    // Initialize all locals to |undefined|. Lexical bindings are temporal
    // dead zoned in bytecode.

    size_t n = frame.nlocals();
    if (n == 0) {
        return;
    }

    // Use R0 to minimize code size. If the number of locals to push is <
    // LOOP_UNROLL_FACTOR, then the initialization pushes are emitted directly
    // and inline.  Otherwise, they're emitted in a partially unrolled loop.
    static const size_t LOOP_UNROLL_FACTOR = 4;
    size_t toPushExtra = n % LOOP_UNROLL_FACTOR;

    masm.moveValue(UndefinedValue(), R0);

    // Handle any extra pushes left over by the optional unrolled loop below.
    for (size_t i = 0; i < toPushExtra; i++) {
        masm.pushValue(R0);
    }

    // Partially unrolled loop of pushes.
    if (n >= LOOP_UNROLL_FACTOR) {
        size_t toPush = n - toPushExtra;
        MOZ_ASSERT(toPush % LOOP_UNROLL_FACTOR == 0);
        MOZ_ASSERT(toPush >= LOOP_UNROLL_FACTOR);
        masm.move32(Imm32(toPush), R1.scratchReg());
        // Emit unrolled loop with 4 pushes per iteration.
        Label pushLoop;
        masm.bind(&pushLoop);
        for (size_t i = 0; i < LOOP_UNROLL_FACTOR; i++) {
            masm.pushValue(R0);
        }
        masm.branchSub32(Assembler::NonZero,
                         Imm32(LOOP_UNROLL_FACTOR), R1.scratchReg(), &pushLoop);
    }
}
=======
// On most platforms we use a dedicated bytecode PC register to avoid many
// dependent loads and stores for sequences of simple bytecode ops. This
// register must be saved/restored around VM and IC calls.
//
// On 32-bit x86 we don't have enough registers for this (because R0-R2 require
// 6 registers) so there we always store the pc on the frame.
static constexpr bool HasInterpreterPCReg() {
  return InterpreterPCReg != InvalidReg;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Handle any extra pushes left over by the optional unrolled loop below.
  for (size_t i = 0; i < toPushExtra; i++) {
    masm.pushValue(R0);
  }

  // Partially unrolled loop of pushes.
  if (n >= LOOP_UNROLL_FACTOR) {
    size_t toPush = n - toPushExtra;
    MOZ_ASSERT(toPush % LOOP_UNROLL_FACTOR == 0);
    MOZ_ASSERT(toPush >= LOOP_UNROLL_FACTOR);
    masm.move32(Imm32(toPush), R1.scratchReg());
    // Emit unrolled loop with 4 pushes per iteration.
    Label pushLoop;
    masm.bind(&pushLoop);
    for (size_t i = 0; i < LOOP_UNROLL_FACTOR; i++) {
      masm.pushValue(R0);
    }
    masm.branchSub32(Assembler::NonZero, Imm32(LOOP_UNROLL_FACTOR),
                     R1.scratchReg(), &pushLoop);
  }
}

bool BaselineCompiler::emitPrologue() {
#ifdef JS_USE_LINK_REGISTER
  // Push link register from generateEnterJIT()'s BLR.
  masm.pushReturnAddress();
  masm.checkStackAlignment();
#endif
  emitProfilerEnterFrame();

  if (script->trackRecordReplayProgress()) {
    masm.inc64(
        AbsoluteAddress(mozilla::recordreplay::ExecutionProgressCounter()));
  }

  masm.push(BaselineFrameReg);
  masm.moveStackPtrTo(BaselineFrameReg);
  masm.subFromStackPtr(Imm32(BaselineFrame::Size()));

  // Initialize BaselineFrame. For eval scripts, the env chain
  // is passed in R1, so we have to be careful not to clobber it.

  // Initialize BaselineFrame::flags.
  masm.store32(Imm32(0), frame.addressOfFlags());

  // Handle env chain pre-initialization (in case GC gets run
  // during stack check).  For global and eval scripts, the env
  // chain is in R1.  For function scripts, the env chain is in
  // the callee, nullptr is stored for now so that GC doesn't choke
  // on a bogus EnvironmentChain value in the frame.
  if (function()) {
    masm.storePtr(ImmPtr(nullptr), frame.addressOfEnvironmentChain());
  } else {
    masm.storePtr(R1.scratchReg(), frame.addressOfEnvironmentChain());
  }

  // Functions with a large number of locals require two stack checks.
  // The VMCall for a fallible stack check can only occur after the
  // env chain has been initialized, as that is required for proper
  // exception handling if the VMCall returns false.  The env chain
  // initialization can only happen after the UndefinedValues for the
  // local slots have been pushed. However by that time, the stack might
  // have grown too much.
  //
  // In these cases, we emit an extra, early, infallible check before pushing
  // the locals. The early check just sets a flag on the frame if the stack
  // check fails. If the flag is set, then the jitcode skips past the pushing
  // of the locals, and directly to env chain initialization followed by the
  // actual stack check, which will throw the correct exception.
  Label earlyStackCheckFailed;
  if (needsEarlyStackCheck()) {
    // Subtract the size of script->nslots() from the stack pointer.
    uint32_t slotsSize = script->nslots() * sizeof(Value);
    Register scratch = R1.scratchReg();
    masm.moveStackPtrTo(scratch);
    masm.subPtr(Imm32(slotsSize), scratch);

    // Set the OVER_RECURSED flag on the frame if the computed stack pointer
    // overflows the stack limit. We have to use the actual (*NoInterrupt)
    // stack limit here because we don't want to set the flag and throw an
    // overrecursion exception later in the interrupt case.
    Label stackCheckOk;
    masm.branchPtr(Assembler::BelowOrEqual,
                   AbsoluteAddress(cx->addressOfJitStackLimitNoInterrupt()),
                   scratch, &stackCheckOk);
    {
      masm.or32(Imm32(BaselineFrame::OVER_RECURSED), frame.addressOfFlags());
      masm.jump(&earlyStackCheckFailed);
    }
    masm.bind(&stackCheckOk);
  }
||||||| merged common ancestors
bool
BaselineCompiler::emitPrologue()
{
#ifdef JS_USE_LINK_REGISTER
    // Push link register from generateEnterJIT()'s BLR.
    masm.pushReturnAddress();
    masm.checkStackAlignment();
#endif
    emitProfilerEnterFrame();

    if (script->trackRecordReplayProgress()) {
        masm.inc64(AbsoluteAddress(mozilla::recordreplay::ExecutionProgressCounter()));
    }

    masm.push(BaselineFrameReg);
    masm.moveStackPtrTo(BaselineFrameReg);
    masm.subFromStackPtr(Imm32(BaselineFrame::Size()));

    // Initialize BaselineFrame. For eval scripts, the env chain
    // is passed in R1, so we have to be careful not to clobber it.

    // Initialize BaselineFrame::flags.
    masm.store32(Imm32(0), frame.addressOfFlags());

    // Handle env chain pre-initialization (in case GC gets run
    // during stack check).  For global and eval scripts, the env
    // chain is in R1.  For function scripts, the env chain is in
    // the callee, nullptr is stored for now so that GC doesn't choke
    // on a bogus EnvironmentChain value in the frame.
    if (function()) {
        masm.storePtr(ImmPtr(nullptr), frame.addressOfEnvironmentChain());
    } else {
        masm.storePtr(R1.scratchReg(), frame.addressOfEnvironmentChain());
    }

    // Functions with a large number of locals require two stack checks.
    // The VMCall for a fallible stack check can only occur after the
    // env chain has been initialized, as that is required for proper
    // exception handling if the VMCall returns false.  The env chain
    // initialization can only happen after the UndefinedValues for the
    // local slots have been pushed. However by that time, the stack might
    // have grown too much.
    //
    // In these cases, we emit an extra, early, infallible check before pushing
    // the locals. The early check just sets a flag on the frame if the stack
    // check fails. If the flag is set, then the jitcode skips past the pushing
    // of the locals, and directly to env chain initialization followed by the
    // actual stack check, which will throw the correct exception.
    Label earlyStackCheckFailed;
    if (needsEarlyStackCheck()) {
        // Subtract the size of script->nslots() from the stack pointer.
        uint32_t slotsSize = script->nslots() * sizeof(Value);
        Register scratch = R1.scratchReg();
        masm.moveStackPtrTo(scratch);
        masm.subPtr(Imm32(slotsSize), scratch);

        // Set the OVER_RECURSED flag on the frame if the computed stack pointer
        // overflows the stack limit. We have to use the actual (*NoInterrupt)
        // stack limit here because we don't want to set the flag and throw an
        // overrecursion exception later in the interrupt case.
        Label stackCheckOk;
        masm.branchPtr(Assembler::BelowOrEqual,
                       AbsoluteAddress(cx->addressOfJitStackLimitNoInterrupt()), scratch,
                       &stackCheckOk);
        {
            masm.or32(Imm32(BaselineFrame::OVER_RECURSED), frame.addressOfFlags());
            masm.jump(&earlyStackCheckFailed);
        }
        masm.bind(&stackCheckOk);
    }
=======
static Register LoadBytecodePC(MacroAssembler& masm, Register scratch) {
  if (HasInterpreterPCReg()) {
    return InterpreterPCReg;
  }

  Address pcAddr(BaselineFrameReg,
                 BaselineFrame::reverseOffsetOfInterpreterPC());
  masm.loadPtr(pcAddr, scratch);
  return scratch;
}

static void LoadInt8Operand(MacroAssembler& masm, Register dest) {
  Register pc = LoadBytecodePC(masm, dest);
  masm.load8SignExtend(Address(pc, sizeof(jsbytecode)), dest);
}

static void LoadUint8Operand(MacroAssembler& masm, Register dest) {
  Register pc = LoadBytecodePC(masm, dest);
  masm.load8ZeroExtend(Address(pc, sizeof(jsbytecode)), dest);
}

static void LoadUint16Operand(MacroAssembler& masm, Register dest) {
  Register pc = LoadBytecodePC(masm, dest);
  masm.load16ZeroExtend(Address(pc, sizeof(jsbytecode)), dest);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  emitInitializeLocals();
||||||| merged common ancestors
    emitInitializeLocals();
=======
static void LoadInt32Operand(MacroAssembler& masm, Register dest) {
  Register pc = LoadBytecodePC(masm, dest);
  masm.load32(Address(pc, sizeof(jsbytecode)), dest);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (needsEarlyStackCheck()) {
    masm.bind(&earlyStackCheckFailed);
  }
||||||| merged common ancestors
    if (needsEarlyStackCheck()) {
        masm.bind(&earlyStackCheckFailed);
    }
=======
static void LoadInt32OperandSignExtendToPtr(MacroAssembler& masm, Register pc,
                                            Register dest) {
  masm.load32SignExtendToPtr(Address(pc, sizeof(jsbytecode)), dest);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef JS_TRACE_LOGGING
  if (!emitTraceLoggerEnter()) {
    return false;
  }
#endif
||||||| merged common ancestors
#ifdef JS_TRACE_LOGGING
    if (!emitTraceLoggerEnter()) {
        return false;
    }
#endif
=======
static void LoadUint24Operand(MacroAssembler& masm, size_t offset,
                              Register dest) {
  // Load the opcode and operand, then left shift to discard the opcode.
  Register pc = LoadBytecodePC(masm, dest);
  masm.load32(Address(pc, offset), dest);
  masm.rshift32(Imm32(8), dest);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Record the offset of the prologue, because Ion can bailout before
  // the env chain is initialized.
  bailoutPrologueOffset_ = CodeOffset(masm.currentOffset());
||||||| merged common ancestors
    // Record the offset of the prologue, because Ion can bailout before
    // the env chain is initialized.
    prologueOffset_ = CodeOffset(masm.currentOffset());
=======
static void LoadInlineValueOperand(MacroAssembler& masm, ValueOperand dest) {
  // Note: the Value might be unaligned but as above we rely on all our
  // platforms having appropriate support for unaligned accesses (except for
  // floating point instructions on ARM).
  Register pc = LoadBytecodePC(masm, dest.scratchReg());
  masm.loadUnalignedValue(Address(pc, sizeof(jsbytecode)), dest);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // When compiling with Debugger instrumentation, set the debuggeeness of
  // the frame before any operation that can call into the VM.
  emitIsDebuggeeCheck();
||||||| merged common ancestors
    // When compiling with Debugger instrumentation, set the debuggeeness of
    // the frame before any operation that can call into the VM.
    emitIsDebuggeeCheck();
=======
template <>
void BaselineCompilerCodeGen::loadScript(Register dest) {
  masm.movePtr(ImmGCPtr(handler.script()), dest);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Initialize the env chain before any operation that may
  // call into the VM and trigger a GC.
  if (!initEnvironmentChain()) {
    return false;
  }
||||||| merged common ancestors
    // Initialize the env chain before any operation that may
    // call into the VM and trigger a GC.
    if (!initEnvironmentChain()) {
        return false;
    }
=======
template <>
void BaselineInterpreterCodeGen::loadScript(Register dest) {
  masm.loadPtr(frame.addressOfInterpreterScript(), dest);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.assertSyncedStack();
  masm.debugAssertContextRealm(script->realm(), R1.scratchReg());
||||||| merged common ancestors
    frame.assertSyncedStack();
    masm.debugAssertContextRealm(script->realm(), R1.scratchReg());
=======
template <>
void BaselineCompilerCodeGen::saveInterpreterPCReg() {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!emitStackCheck()) {
    return false;
  }
||||||| merged common ancestors
    if (!emitStackCheck()) {
        return false;
    }
=======
template <>
void BaselineInterpreterCodeGen::saveInterpreterPCReg() {
  if (HasInterpreterPCReg()) {
    masm.storePtr(InterpreterPCReg, frame.addressOfInterpreterPC());
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!emitDebugPrologue()) {
    return false;
  }
||||||| merged common ancestors
    if (!emitDebugPrologue()) {
        return false;
    }
=======
template <>
void BaselineCompilerCodeGen::restoreInterpreterPCReg() {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!emitWarmUpCounterIncrement()) {
    return false;
  }
||||||| merged common ancestors
    if (!emitWarmUpCounterIncrement()) {
        return false;
    }
=======
template <>
void BaselineInterpreterCodeGen::restoreInterpreterPCReg() {
  if (HasInterpreterPCReg()) {
    masm.loadPtr(frame.addressOfInterpreterPC(), InterpreterPCReg);
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!emitArgumentTypeChecks()) {
    return false;
  }
||||||| merged common ancestors
    if (!emitArgumentTypeChecks()) {
        return false;
    }
=======
template <>
void BaselineCompilerCodeGen::loadScriptAtom(Register index, Register dest) {
  MOZ_CRASH("BaselineCompiler shouldn't call loadScriptAtom");
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
template <>
void BaselineInterpreterCodeGen::loadScriptAtom(Register index, Register dest) {
  MOZ_ASSERT(index != dest);
  loadScript(dest);
  masm.loadPtr(Address(dest, JSScript::offsetOfScriptData()), dest);
  masm.loadPtr(
      BaseIndex(dest, index, ScalePointer, SharedScriptData::offsetOfAtoms()),
      dest);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCompiler::emitEpilogue() {
  // Record the offset of the epilogue, so we can do early return from
  // Debugger handlers during on-stack recompile.
  debugOsrEpilogueOffset_ = CodeOffset(masm.currentOffset());
||||||| merged common ancestors
bool
BaselineCompiler::emitEpilogue()
{
    // Record the offset of the epilogue, so we can do early return from
    // Debugger handlers during on-stack recompile.
    epilogueOffset_ = CodeOffset(masm.currentOffset());
=======
template <>
void BaselineCompilerCodeGen::emitInitializeLocals() {
  // Initialize all locals to |undefined|. Lexical bindings are temporal
  // dead zoned in bytecode.
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&return_);
||||||| merged common ancestors
    masm.bind(&return_);
=======
  size_t n = frame.nlocals();
  if (n == 0) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef JS_TRACE_LOGGING
  if (!emitTraceLoggerExit()) {
    return false;
  }
#endif
||||||| merged common ancestors
#ifdef JS_TRACE_LOGGING
    if (!emitTraceLoggerExit()) {
        return false;
    }
#endif
=======
  // Use R0 to minimize code size. If the number of locals to push is <
  // LOOP_UNROLL_FACTOR, then the initialization pushes are emitted directly
  // and inline.  Otherwise, they're emitted in a partially unrolled loop.
  static const size_t LOOP_UNROLL_FACTOR = 4;
  size_t toPushExtra = n % LOOP_UNROLL_FACTOR;
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.moveToStackPtr(BaselineFrameReg);
  masm.pop(BaselineFrameReg);
||||||| merged common ancestors
    masm.moveToStackPtr(BaselineFrameReg);
    masm.pop(BaselineFrameReg);
=======
  masm.moveValue(UndefinedValue(), R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  emitProfilerExitFrame();
||||||| merged common ancestors
    emitProfilerExitFrame();
=======
  // Handle any extra pushes left over by the optional unrolled loop below.
  for (size_t i = 0; i < toPushExtra; i++) {
    masm.pushValue(R0);
  }

  // Partially unrolled loop of pushes.
  if (n >= LOOP_UNROLL_FACTOR) {
    size_t toPush = n - toPushExtra;
    MOZ_ASSERT(toPush % LOOP_UNROLL_FACTOR == 0);
    MOZ_ASSERT(toPush >= LOOP_UNROLL_FACTOR);
    masm.move32(Imm32(toPush), R1.scratchReg());
    // Emit unrolled loop with 4 pushes per iteration.
    Label pushLoop;
    masm.bind(&pushLoop);
    for (size_t i = 0; i < LOOP_UNROLL_FACTOR; i++) {
      masm.pushValue(R0);
    }
    masm.branchSub32(Assembler::NonZero, Imm32(LOOP_UNROLL_FACTOR),
                     R1.scratchReg(), &pushLoop);
  }
}

template <>
void BaselineInterpreterCodeGen::emitInitializeLocals() {
  // Push |undefined| for all locals.

  Register scratch = R0.scratchReg();
  loadScript(scratch);
  masm.loadPtr(Address(scratch, JSScript::offsetOfScriptData()), scratch);
  masm.load32(Address(scratch, SharedScriptData::offsetOfNfixed()), scratch);

  Label top, done;
  masm.bind(&top);
  masm.branchTest32(Assembler::Zero, scratch, scratch, &done);
  {
    masm.pushValue(UndefinedValue());
    masm.sub32(Imm32(1), scratch);
    masm.jump(&top);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.ret();
  return true;
||||||| merged common ancestors
    masm.ret();
    return true;
=======
  masm.bind(&done);
>>>>>>> upstream-releases
}

// On input:
//  R2.scratchReg() contains object being written to.
//  Called with the baseline stack synced, except for R0 which is preserved.
//  All other registers are usable as scratch.
// This calls:
//    void PostWriteBarrier(JSRuntime* rt, JSObject* obj);
<<<<<<< HEAD
bool BaselineCompiler::emitOutOfLinePostBarrierSlot() {
  masm.bind(&postBarrierSlot_);

  Register objReg = R2.scratchReg();
  AllocatableGeneralRegisterSet regs(GeneralRegisterSet::All());
  regs.take(R0);
  regs.take(objReg);
  regs.take(BaselineFrameReg);
  Register scratch = regs.takeAny();
||||||| merged common ancestors
bool
BaselineCompiler::emitOutOfLinePostBarrierSlot()
{
    masm.bind(&postBarrierSlot_);

    Register objReg = R2.scratchReg();
    AllocatableGeneralRegisterSet regs(GeneralRegisterSet::All());
    regs.take(R0);
    regs.take(objReg);
    regs.take(BaselineFrameReg);
    Register scratch = regs.takeAny();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitOutOfLinePostBarrierSlot() {
  masm.bind(&postBarrierSlot_);

  saveInterpreterPCReg();

  Register objReg = R2.scratchReg();
  AllocatableGeneralRegisterSet regs(GeneralRegisterSet::All());
  regs.take(R0);
  regs.take(objReg);
  regs.take(BaselineFrameReg);
  Register scratch = regs.takeAny();
>>>>>>> upstream-releases
#if defined(JS_CODEGEN_ARM) || defined(JS_CODEGEN_ARM64)
  // On ARM, save the link register before calling.  It contains the return
  // address.  The |masm.ret()| later will pop this into |pc| to return.
  masm.push(lr);
#elif defined(JS_CODEGEN_MIPS32) || defined(JS_CODEGEN_MIPS64)
  masm.push(ra);
#endif
  masm.pushValue(R0);

  masm.setupUnalignedABICall(scratch);
  masm.movePtr(ImmPtr(cx->runtime()), scratch);
  masm.passABIArg(scratch);
  masm.passABIArg(objReg);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, PostWriteBarrier));

<<<<<<< HEAD
  masm.popValue(R0);
  masm.ret();
  return true;
||||||| merged common ancestors
    masm.popValue(R0);
    masm.ret();
    return true;
=======
  restoreInterpreterPCReg();

  masm.popValue(R0);
  masm.ret();
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitNextIC() {
  // Emit a call to an IC stored in ICScript. Calls to this must match the
  // ICEntry order in ICScript: first the non-op IC entries for |this| and
  // formal arguments, then the for-op IC entries for JOF_IC ops.
||||||| merged common ancestors
bool
BaselineCompiler::emitIC(ICStub* stub, ICEntry::Kind kind)
{
    ICEntry* entry = allocateICEntry(stub, kind);
    if (!entry) {
        return false;
    }
=======
template <>
bool BaselineCompilerCodeGen::emitNextIC() {
  // Emit a call to an IC stored in JitScript. Calls to this must match the
  // ICEntry order in JitScript: first the non-op IC entries for |this| and
  // formal arguments, then the for-op IC entries for JOF_IC ops.
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t pcOffset = script->pcToOffset(pc);
||||||| merged common ancestors
    CodeOffset patchOffset;
    EmitCallIC(&patchOffset, masm);
    entry->setReturnOffset(CodeOffset(masm.currentOffset()));
    if (!addICLoadLabel(patchOffset)) {
        return false;
    }
=======
  JSScript* script = handler.script();
  uint32_t pcOffset = script->pcToOffset(handler.pc());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // We don't use every ICEntry and we can skip unreachable ops, so we have
  // to loop until we find an ICEntry for the current pc.
  const ICEntry* entry;
  do {
    entry = &script->icScript()->icEntry(icEntryIndex_);
    icEntryIndex_++;
  } while (entry->pcOffset() < pcOffset);

  MOZ_RELEASE_ASSERT(entry->pcOffset() == pcOffset);
  MOZ_ASSERT_IF(entry->isForOp(), BytecodeOpHasIC(JSOp(*pc)));

  CodeOffset callOffset;
  EmitCallIC(masm, entry, &callOffset);

  RetAddrEntry::Kind kind =
      entry->isForOp() ? RetAddrEntry::Kind::IC : RetAddrEntry::Kind::NonOpIC;

  if (!retAddrEntries_.emplaceBack(script->pcToOffset(pc), kind, callOffset)) {
    ReportOutOfMemory(cx);
    return false;
  }

  return true;
||||||| merged common ancestors
    return true;
=======
  // We don't use every ICEntry and we can skip unreachable ops, so we have
  // to loop until we find an ICEntry for the current pc.
  const ICEntry* entry;
  do {
    entry = &script->jitScript()->icEntry(handler.icEntryIndex());
    handler.moveToNextICEntry();
  } while (entry->pcOffset() < pcOffset);

  MOZ_RELEASE_ASSERT(entry->pcOffset() == pcOffset);
  MOZ_ASSERT_IF(!entry->isForPrologue(), BytecodeOpHasIC(JSOp(*handler.pc())));

  CodeOffset callOffset;
  EmitCallIC(masm, entry, &callOffset);

  RetAddrEntry::Kind kind = entry->isForPrologue()
                                ? RetAddrEntry::Kind::PrologueIC
                                : RetAddrEntry::Kind::IC;

  if (!handler.retAddrEntries().emplaceBack(pcOffset, kind, callOffset)) {
    ReportOutOfMemory(cx);
    return false;
  }

  return true;
}

template <>
bool BaselineInterpreterCodeGen::emitNextIC() {
  saveInterpreterPCReg();
  masm.loadPtr(frame.addressOfInterpreterICEntry(), ICStubReg);
  masm.loadPtr(Address(ICStubReg, ICEntry::offsetOfFirstStub()), ICStubReg);
  masm.call(Address(ICStubReg, ICStub::offsetOfStubCode()));
  restoreInterpreterPCReg();
  return true;
>>>>>>> upstream-releases
}

template <typename Handler>
void BaselineCodeGen<Handler>::prepareVMCall() {
  pushedBeforeCall_ = masm.framePushed();
#ifdef DEBUG
  inCall_ = true;
#endif

  // Ensure everything is synced.
  frame.syncStack(0);

  // Save the frame pointer.
  masm.Push(BaselineFrameReg);
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::callVM(const VMFunction& fun,
                                      CallVMPhase phase) {
  TrampolinePtr code = cx->runtime()->jitRuntime()->getVMWrapper(fun);
||||||| merged common ancestors
bool
BaselineCompiler::callVM(const VMFunction& fun, CallVMPhase phase)
{
    TrampolinePtr code = cx->runtime()->jitRuntime()->getVMWrapper(fun);
=======
template <>
void BaselineCompilerCodeGen::storeFrameSizeAndPushDescriptor(
    uint32_t frameBaseSize, uint32_t argSize, const Address& frameSizeAddr,
    Register scratch1, Register scratch2) {
  uint32_t frameVals = frame.nlocals() + frame.stackDepth();
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef DEBUG
  // Assert prepareVMCall() has been called.
  MOZ_ASSERT(inCall_);
  inCall_ = false;
||||||| merged common ancestors
#ifdef DEBUG
    // Assert prepareVMCall() has been called.
    MOZ_ASSERT(inCall_);
    inCall_ = false;
=======
  uint32_t frameFullSize = frameBaseSize + (frameVals * sizeof(Value));
  masm.store32(Imm32(frameFullSize), frameSizeAddr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Assert the frame does not have an override pc when we're executing JIT
  // code.
  {
    Label ok;
    masm.branchTest32(Assembler::Zero, frame.addressOfFlags(),
                      Imm32(BaselineFrame::HAS_OVERRIDE_PC), &ok);
    masm.assumeUnreachable(
        "BaselineFrame shouldn't override pc when executing JIT code");
    masm.bind(&ok);
  }
#endif
||||||| merged common ancestors
    // Assert the frame does not have an override pc when we're executing JIT code.
    {
        Label ok;
        masm.branchTest32(Assembler::Zero, frame.addressOfFlags(),
                          Imm32(BaselineFrame::HAS_OVERRIDE_PC), &ok);
        masm.assumeUnreachable("BaselineFrame shouldn't override pc when executing JIT code");
        masm.bind(&ok);
    }
#endif
=======
  uint32_t descriptor = MakeFrameDescriptor(
      frameFullSize + argSize, FrameType::BaselineJS, ExitFrameLayout::Size());
  masm.push(Imm32(descriptor));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Compute argument size. Note that this include the size of the frame pointer
  // pushed by prepareVMCall.
  uint32_t argSize = fun.explicitStackSlots() * sizeof(void*) + sizeof(void*);

  // Assert all arguments were pushed.
  MOZ_ASSERT(masm.framePushed() - pushedBeforeCall_ == argSize);

  Address frameSizeAddress(BaselineFrameReg,
                           BaselineFrame::reverseOffsetOfFrameSize());
  uint32_t frameVals = frame.nlocals() + frame.stackDepth();
  uint32_t frameBaseSize =
      BaselineFrame::FramePointerOffset + BaselineFrame::Size();
  uint32_t frameFullSize = frameBaseSize + (frameVals * sizeof(Value));
  if (phase == POST_INITIALIZE) {
    masm.store32(Imm32(frameFullSize), frameSizeAddress);
    uint32_t descriptor =
        MakeFrameDescriptor(frameFullSize + argSize, FrameType::BaselineJS,
                            ExitFrameLayout::Size());
    masm.push(Imm32(descriptor));
  } else {
    MOZ_ASSERT(phase == CHECK_OVER_RECURSED);
    Label afterWrite;
    Label writePostInitialize;

    // If OVER_RECURSED is set, then frame locals haven't been pushed yet.
    masm.branchTest32(Assembler::Zero, frame.addressOfFlags(),
                      Imm32(BaselineFrame::OVER_RECURSED),
                      &writePostInitialize);

    masm.move32(Imm32(frameBaseSize), ICTailCallReg);
    masm.jump(&afterWrite);

    masm.bind(&writePostInitialize);
    masm.move32(Imm32(frameFullSize), ICTailCallReg);

    masm.bind(&afterWrite);
    masm.store32(ICTailCallReg, frameSizeAddress);
    masm.add32(Imm32(argSize), ICTailCallReg);
    masm.makeFrameDescriptor(ICTailCallReg, FrameType::BaselineJS,
                             ExitFrameLayout::Size());
    masm.push(ICTailCallReg);
  }
  MOZ_ASSERT(fun.expectTailCall == NonTailCall);
  // Perform the call.
  masm.call(code);
  uint32_t callOffset = masm.currentOffset();
  masm.pop(BaselineFrameReg);
||||||| merged common ancestors
    // Compute argument size. Note that this include the size of the frame pointer
    // pushed by prepareVMCall.
    uint32_t argSize = fun.explicitStackSlots() * sizeof(void*) + sizeof(void*);

    // Assert all arguments were pushed.
    MOZ_ASSERT(masm.framePushed() - pushedBeforeCall_ == argSize);

    Address frameSizeAddress(BaselineFrameReg, BaselineFrame::reverseOffsetOfFrameSize());
    uint32_t frameVals = frame.nlocals() + frame.stackDepth();
    uint32_t frameBaseSize = BaselineFrame::FramePointerOffset + BaselineFrame::Size();
    uint32_t frameFullSize = frameBaseSize + (frameVals * sizeof(Value));
    if (phase == POST_INITIALIZE) {
        masm.store32(Imm32(frameFullSize), frameSizeAddress);
        uint32_t descriptor = MakeFrameDescriptor(frameFullSize + argSize, FrameType::BaselineJS,
                                                  ExitFrameLayout::Size());
        masm.push(Imm32(descriptor));
    } else {
        MOZ_ASSERT(phase == CHECK_OVER_RECURSED);
        Label afterWrite;
        Label writePostInitialize;

        // If OVER_RECURSED is set, then frame locals haven't been pushed yet.
        masm.branchTest32(Assembler::Zero,
                          frame.addressOfFlags(),
                          Imm32(BaselineFrame::OVER_RECURSED),
                          &writePostInitialize);

        masm.move32(Imm32(frameBaseSize), ICTailCallReg);
        masm.jump(&afterWrite);

        masm.bind(&writePostInitialize);
        masm.move32(Imm32(frameFullSize), ICTailCallReg);

        masm.bind(&afterWrite);
        masm.store32(ICTailCallReg, frameSizeAddress);
        masm.add32(Imm32(argSize), ICTailCallReg);
        masm.makeFrameDescriptor(ICTailCallReg, FrameType::BaselineJS, ExitFrameLayout::Size());
        masm.push(ICTailCallReg);
    }
    MOZ_ASSERT(fun.expectTailCall == NonTailCall);
    // Perform the call.
    masm.call(code);
    uint32_t callOffset = masm.currentOffset();
    masm.pop(BaselineFrameReg);
=======
template <>
void BaselineInterpreterCodeGen::storeFrameSizeAndPushDescriptor(
    uint32_t frameBaseSize, uint32_t argSize, const Address& frameSizeAddr,
    Register scratch1, Register scratch2) {
  // scratch1 = FramePointer + BaselineFrame::FramePointerOffset - StackPointer.
  masm.computeEffectiveAddress(
      Address(BaselineFrameReg, BaselineFrame::FramePointerOffset), scratch1);
  masm.subStackPtrFrom(scratch1);
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef DEBUG
  // Assert the frame does not have an override pc when we're executing JIT
  // code.
  {
    Label ok;
    masm.branchTest32(Assembler::Zero, frame.addressOfFlags(),
                      Imm32(BaselineFrame::HAS_OVERRIDE_PC), &ok);
    masm.assumeUnreachable("BaselineFrame shouldn't override pc after VM call");
    masm.bind(&ok);
  }
#endif
||||||| merged common ancestors
#ifdef DEBUG
    // Assert the frame does not have an override pc when we're executing JIT code.
    {
        Label ok;
        masm.branchTest32(Assembler::Zero, frame.addressOfFlags(),
                          Imm32(BaselineFrame::HAS_OVERRIDE_PC), &ok);
        masm.assumeUnreachable("BaselineFrame shouldn't override pc after VM call");
        masm.bind(&ok);
    }
#endif
=======
  // Store the frame size without VMFunction arguments. Use
  // computeEffectiveAddress instead of sub32 to avoid an extra move.
  masm.computeEffectiveAddress(Address(scratch1, -int32_t(argSize)), scratch2);
  masm.store32(scratch2, frameSizeAddr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return appendRetAddrEntry(RetAddrEntry::Kind::CallVM, callOffset);
||||||| merged common ancestors
    // Add a fake ICEntry (without stubs), so that the return offset to
    // pc mapping works.
    return appendICEntry(ICEntry::Kind_CallVM, callOffset);
=======
  // Push frame descriptor based on the full frame size.
  masm.makeFrameDescriptor(scratch1, FrameType::BaselineJS,
                           ExitFrameLayout::Size());
  masm.push(scratch1);
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::callVMInternal(VMFunctionId id,
                                              CallVMPhase phase) {
#ifdef DEBUG
  // Assert prepareVMCall() has been called.
  MOZ_ASSERT(inCall_);
  inCall_ = false;

<<<<<<< HEAD
bool BaselineCompiler::emitStackCheck() {
  // If this is the late stack check for a frame which contains an early stack
  // check, then the early stack check might have failed and skipped past the
  // pushing of locals on the stack.
  //
  // If this is a possibility, then the OVER_RECURSED flag should be checked,
  // and the VMCall to CheckOverRecursedBaseline done unconditionally if it's
  // set.
  Label forceCall;
  if (needsEarlyStackCheck()) {
    masm.branchTest32(Assembler::NonZero, frame.addressOfFlags(),
                      Imm32(BaselineFrame::OVER_RECURSED), &forceCall);
  }

  Label skipCall;
  masm.branchStackPtrRhs(Assembler::BelowOrEqual,
                         AbsoluteAddress(cx->addressOfJitStackLimit()),
                         &skipCall);

  if (needsEarlyStackCheck()) {
    masm.bind(&forceCall);
  }

  prepareVMCall();
  masm.loadBaselineFramePtr(BaselineFrameReg, R1.scratchReg());
  pushArg(R1.scratchReg());

  CallVMPhase phase = POST_INITIALIZE;
  if (needsEarlyStackCheck()) {
    phase = CHECK_OVER_RECURSED;
  }

  if (!callVMNonOp(CheckOverRecursedBaselineInfo, phase)) {
    return false;
  }

  retAddrEntries_.back().setKind(RetAddrEntry::Kind::StackCheck);

  masm.bind(&skipCall);
  return true;
}

void BaselineCompiler::emitIsDebuggeeCheck() {
  if (compileDebugInstrumentation()) {
    masm.Push(BaselineFrameReg);
    masm.setupUnalignedABICall(R0.scratchReg());
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
    masm.passABIArg(R0.scratchReg());
    masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, jit::FrameIsDebuggeeCheck));
    masm.Pop(BaselineFrameReg);
  }
}
||||||| merged common ancestors
bool
BaselineCompiler::emitStackCheck()
{
    // If this is the late stack check for a frame which contains an early stack check,
    // then the early stack check might have failed and skipped past the pushing of locals
    // on the stack.
    //
    // If this is a possibility, then the OVER_RECURSED flag should be checked, and the
    // VMCall to CheckOverRecursedBaseline done unconditionally if it's set.
    Label forceCall;
    if (needsEarlyStackCheck()) {
        masm.branchTest32(Assembler::NonZero,
                          frame.addressOfFlags(),
                          Imm32(BaselineFrame::OVER_RECURSED),
                          &forceCall);
    }
=======
  // Assert the frame does not have an override pc when we're executing JIT
  // code.
  {
    Label ok;
    masm.branchTest32(Assembler::Zero, frame.addressOfFlags(),
                      Imm32(BaselineFrame::HAS_OVERRIDE_PC), &ok);
    masm.assumeUnreachable(
        "BaselineFrame shouldn't override pc when executing JIT code");
    masm.bind(&ok);
  }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*DebugPrologueFn)(JSContext*, BaselineFrame*, jsbytecode*, bool*);
static const VMFunction DebugPrologueInfo =
    FunctionInfo<DebugPrologueFn>(jit::DebugPrologue, "DebugPrologue");
||||||| merged common ancestors
    Label skipCall;
    masm.branchStackPtrRhs(Assembler::BelowOrEqual,
                           AbsoluteAddress(cx->addressOfJitStackLimit()),
                           &skipCall);
=======
  TrampolinePtr code = cx->runtime()->jitRuntime()->getVMWrapper(id);
  const VMFunctionData& fun = GetVMFunction(id);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCompiler::emitDebugPrologue() {
  if (compileDebugInstrumentation()) {
    // Load pointer to BaselineFrame in R0.
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
||||||| merged common ancestors
    if (needsEarlyStackCheck()) {
        masm.bind(&forceCall);
    }
=======
  // Compute argument size. Note that this include the size of the frame pointer
  // pushed by prepareVMCall.
  uint32_t argSize = fun.explicitStackSlots() * sizeof(void*) + sizeof(void*);
>>>>>>> upstream-releases

<<<<<<< HEAD
    prepareVMCall();
    pushArg(ImmPtr(pc));
    pushArg(R0.scratchReg());
    if (!callVM(DebugPrologueInfo)) {
      return false;
    }
||||||| merged common ancestors
    prepareVMCall();
    masm.loadBaselineFramePtr(BaselineFrameReg, R1.scratchReg());
    pushArg(R1.scratchReg());

    CallVMPhase phase = POST_INITIALIZE;
    if (needsEarlyStackCheck()) {
        phase = CHECK_OVER_RECURSED;
    }

    if (!callVMNonOp(CheckOverRecursedBaselineInfo, phase)) {
        return false;
    }

    icEntries_.back().setFakeKind(ICEntry::Kind_StackCheck);

    masm.bind(&skipCall);
    return true;
}

void
BaselineCompiler::emitIsDebuggeeCheck()
{
    if (compileDebugInstrumentation_) {
        masm.Push(BaselineFrameReg);
        masm.setupUnalignedABICall(R0.scratchReg());
        masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
        masm.passABIArg(R0.scratchReg());
        masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, jit::FrameIsDebuggeeCheck));
        masm.Pop(BaselineFrameReg);
    }
}
=======
  // Assert all arguments were pushed.
  MOZ_ASSERT(masm.framePushed() - pushedBeforeCall_ == argSize);

  saveInterpreterPCReg();

  Address frameSizeAddress(BaselineFrameReg,
                           BaselineFrame::reverseOffsetOfFrameSize());
  uint32_t frameBaseSize =
      BaselineFrame::FramePointerOffset + BaselineFrame::Size();
  if (phase == POST_INITIALIZE) {
    storeFrameSizeAndPushDescriptor(frameBaseSize, argSize, frameSizeAddress,
                                    R0.scratchReg(), R1.scratchReg());
  } else {
    MOZ_ASSERT(phase == CHECK_OVER_RECURSED);
    Label done, pushedFrameLocals;

    // If OVER_RECURSED is set, then frame locals haven't been pushed yet.
    masm.branchTest32(Assembler::Zero, frame.addressOfFlags(),
                      Imm32(BaselineFrame::OVER_RECURSED), &pushedFrameLocals);
    {
      masm.store32(Imm32(frameBaseSize), frameSizeAddress);
      uint32_t descriptor =
          MakeFrameDescriptor(frameBaseSize + argSize, FrameType::BaselineJS,
                              ExitFrameLayout::Size());
      masm.push(Imm32(descriptor));
      masm.jump(&done);
    }
    masm.bind(&pushedFrameLocals);
    {
      storeFrameSizeAndPushDescriptor(frameBaseSize, argSize, frameSizeAddress,
                                      R0.scratchReg(), R1.scratchReg());
    }
    masm.bind(&done);
  }
  MOZ_ASSERT(fun.expectTailCall == NonTailCall);
  // Perform the call.
  masm.call(code);
  uint32_t callOffset = masm.currentOffset();
  masm.Pop(BaselineFrameReg);

  // Pop arguments from framePushed.
  masm.implicitPop(fun.explicitStackSlots() * sizeof(void*));

  restoreInterpreterPCReg();

#ifdef DEBUG
  // Assert the frame does not have an override pc when we're executing JIT
  // code.
  {
    Label ok;
    masm.branchTest32(Assembler::Zero, frame.addressOfFlags(),
                      Imm32(BaselineFrame::HAS_OVERRIDE_PC), &ok);
    masm.assumeUnreachable("BaselineFrame shouldn't override pc after VM call");
    masm.bind(&ok);
  }
#endif

  return handler.appendRetAddrEntry(cx, RetAddrEntry::Kind::CallVM, callOffset);
}

template <typename Handler>
template <typename Fn, Fn fn>
bool BaselineCodeGen<Handler>::callVM(CallVMPhase phase) {
  VMFunctionId fnId = VMFunctionToId<Fn, fn>::id;
  return callVMInternal(fnId, phase);
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emitStackCheck() {
  // If this is the late stack check for a frame which contains an early stack
  // check, then the early stack check might have failed and skipped past the
  // pushing of locals on the stack.
  //
  // If this is a possibility, then the OVER_RECURSED flag should be checked,
  // and the VMCall to CheckOverRecursedBaseline done unconditionally if it's
  // set.
  Label forceCall;
  if (handler.needsEarlyStackCheck()) {
    masm.branchTest32(Assembler::NonZero, frame.addressOfFlags(),
                      Imm32(BaselineFrame::OVER_RECURSED), &forceCall);
  }

  Label skipCall;
  masm.branchStackPtrRhs(Assembler::BelowOrEqual,
                         AbsoluteAddress(cx->addressOfJitStackLimit()),
                         &skipCall);

  if (handler.needsEarlyStackCheck()) {
    masm.bind(&forceCall);
  }

  prepareVMCall();
  masm.loadBaselineFramePtr(BaselineFrameReg, R1.scratchReg());
  pushArg(R1.scratchReg());

  CallVMPhase phase = POST_INITIALIZE;
  if (handler.needsEarlyStackCheck()) {
    phase = CHECK_OVER_RECURSED;
  }

  using Fn = bool (*)(JSContext*, BaselineFrame*);
  if (!callVMNonOp<Fn, CheckOverRecursedBaseline>(phase)) {
    return false;
  }

  handler.markLastRetAddrEntryKind(RetAddrEntry::Kind::StackCheck);

  masm.bind(&skipCall);
  return true;
}

static void EmitCallFrameIsDebuggeeCheck(MacroAssembler& masm) {
  masm.Push(BaselineFrameReg);
  masm.setupUnalignedABICall(R0.scratchReg());
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
  masm.passABIArg(R0.scratchReg());
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, jit::FrameIsDebuggeeCheck));
  masm.Pop(BaselineFrameReg);
}

template <>
void BaselineCompilerCodeGen::emitIsDebuggeeCheck() {
  if (handler.compileDebugInstrumentation()) {
    EmitCallFrameIsDebuggeeCheck(masm);
  }
}

template <>
void BaselineInterpreterCodeGen::emitIsDebuggeeCheck() {
  // Use a toggled jump to call FrameIsDebuggeeCheck only if the debugger is
  // enabled.
  //
  // TODO(bug 1522394): consider having a cx->realm->isDebuggee guard before the
  // call. Consider moving the callWithABI out-of-line.

  Label skipCheck;
  CodeOffset toggleOffset = masm.toggledJump(&skipCheck);
  {
    saveInterpreterPCReg();
    EmitCallFrameIsDebuggeeCheck(masm);
    restoreInterpreterPCReg();
  }
  masm.bind(&skipCheck);
  handler.setDebuggeeCheckOffset(toggleOffset);
}

static void MaybeIncrementCodeCoverageCounter(MacroAssembler& masm,
                                              JSScript* script,
                                              jsbytecode* pc) {
  if (!script->hasScriptCounts()) {
    return;
  }
  PCCounts* counts = script->maybeGetPCCounts(pc);
  uint64_t* counterAddr = &counts->numExec();
  masm.inc64(AbsoluteAddress(counterAddr));
}

template <>
bool BaselineCompilerCodeGen::emitHandleCodeCoverageAtPrologue() {
  // If the main instruction is not a jump target, then we emit the
  // corresponding code coverage counter.
  JSScript* script = handler.script();
  jsbytecode* main = script->main();
  if (!BytecodeIsJumpTarget(JSOp(*main))) {
    MaybeIncrementCodeCoverageCounter(masm, script, main);
  }
  return true;
}

template <>
bool BaselineInterpreterCodeGen::emitHandleCodeCoverageAtPrologue() {
  Label skipCoverage;
  CodeOffset toggleOffset = masm.toggledJump(&skipCoverage);
  masm.call(handler.codeCoverageAtPrologueLabel());
  masm.bind(&skipCoverage);
  return handler.codeCoverageOffsets().append(toggleOffset.offset());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Fix up the RetAddrEntry appended by callVM for on-stack recompilation.
    retAddrEntries_.back().setKind(RetAddrEntry::Kind::DebugPrologue);

    // If the stub returns |true|, we have to return the value stored in the
    // frame's return value slot.
    Label done;
    masm.branchTest32(Assembler::Zero, ReturnReg, ReturnReg, &done);
    {
      masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
      masm.jump(&return_);
    }
    masm.bind(&done);
  }
||||||| merged common ancestors
typedef bool (*DebugPrologueFn)(JSContext*, BaselineFrame*, jsbytecode*, bool*);
static const VMFunction DebugPrologueInfo =
    FunctionInfo<DebugPrologueFn>(jit::DebugPrologue, "DebugPrologue");

bool
BaselineCompiler::emitDebugPrologue()
{
    if (compileDebugInstrumentation_) {
        // Load pointer to BaselineFrame in R0.
        masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());

        prepareVMCall();
        pushArg(ImmPtr(pc));
        pushArg(R0.scratchReg());
        if (!callVM(DebugPrologueInfo)) {
            return false;
        }

        // Fix up the fake ICEntry appended by callVM for on-stack recompilation.
        icEntries_.back().setFakeKind(ICEntry::Kind_DebugPrologue);

        // If the stub returns |true|, we have to return the value stored in the
        // frame's return value slot.
        Label done;
        masm.branchTest32(Assembler::Zero, ReturnReg, ReturnReg, &done);
        {
            masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
            masm.jump(&return_);
        }
        masm.bind(&done);
    }
=======
template <>
void BaselineCompilerCodeGen::subtractScriptSlotsSize(Register reg,
                                                      Register scratch) {
  uint32_t slotsSize = handler.script()->nslots() * sizeof(Value);
  masm.subPtr(Imm32(slotsSize), reg);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  debugOsrPrologueOffset_ = CodeOffset(masm.currentOffset());
||||||| merged common ancestors
    postDebugPrologueOffset_ = CodeOffset(masm.currentOffset());
=======
template <>
void BaselineInterpreterCodeGen::subtractScriptSlotsSize(Register reg,
                                                         Register scratch) {
  // reg = reg - script->nslots() * sizeof(Value)
  MOZ_ASSERT(reg != scratch);
  loadScript(scratch);
  masm.loadPtr(Address(scratch, JSScript::offsetOfScriptData()), scratch);
  masm.load32(Address(scratch, SharedScriptData::offsetOfNslots()), scratch);
  static_assert(sizeof(Value) == 8,
                "shift by 3 below assumes Value is 8 bytes");
  masm.lshiftPtr(Imm32(3), scratch);
  masm.subPtr(scratch, reg);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
template <>
void BaselineCompilerCodeGen::loadGlobalLexicalEnvironment(Register dest) {
  masm.movePtr(ImmGCPtr(&cx->global()->lexicalEnvironment()), dest);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*CheckGlobalOrEvalDeclarationConflictsFn)(JSContext*,
                                                        BaselineFrame*);
static const VMFunction CheckGlobalOrEvalDeclarationConflictsInfo =
    FunctionInfo<CheckGlobalOrEvalDeclarationConflictsFn>(
        jit::CheckGlobalOrEvalDeclarationConflicts,
        "CheckGlobalOrEvalDeclarationConflicts");
||||||| merged common ancestors
typedef bool (*CheckGlobalOrEvalDeclarationConflictsFn)(JSContext*, BaselineFrame*);
static const VMFunction CheckGlobalOrEvalDeclarationConflictsInfo =
    FunctionInfo<CheckGlobalOrEvalDeclarationConflictsFn>(jit::CheckGlobalOrEvalDeclarationConflicts,
                                                          "CheckGlobalOrEvalDeclarationConflicts");
=======
template <>
void BaselineInterpreterCodeGen::loadGlobalLexicalEnvironment(Register dest) {
  masm.loadPtr(AbsoluteAddress(cx->addressOfRealm()), dest);
  masm.loadPtr(Address(dest, Realm::offsetOfActiveLexicalEnvironment()), dest);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*InitFunctionEnvironmentObjectsFn)(JSContext*, BaselineFrame*);
static const VMFunction InitFunctionEnvironmentObjectsInfo =
    FunctionInfo<InitFunctionEnvironmentObjectsFn>(
        jit::InitFunctionEnvironmentObjects, "InitFunctionEnvironmentObjects");

bool BaselineCompiler::initEnvironmentChain() {
  CallVMPhase phase = POST_INITIALIZE;
  if (needsEarlyStackCheck()) {
    phase = CHECK_OVER_RECURSED;
  }

  RootedFunction fun(cx, function());
  if (fun) {
    // Use callee->environment as env chain. Note that we do this also
    // for needsSomeEnvironmentObject functions, so that the env chain
    // slot is properly initialized if the call triggers GC.
    Register callee = R0.scratchReg();
    Register scope = R1.scratchReg();
    masm.loadFunctionFromCalleeToken(frame.addressOfCalleeToken(), callee);
    masm.loadPtr(Address(callee, JSFunction::offsetOfEnvironment()), scope);
    masm.storePtr(scope, frame.addressOfEnvironmentChain());
||||||| merged common ancestors
typedef bool (*InitFunctionEnvironmentObjectsFn)(JSContext*, BaselineFrame*);
static const VMFunction InitFunctionEnvironmentObjectsInfo =
    FunctionInfo<InitFunctionEnvironmentObjectsFn>(jit::InitFunctionEnvironmentObjects,
                                                   "InitFunctionEnvironmentObjects");

bool
BaselineCompiler::initEnvironmentChain()
{
    CallVMPhase phase = POST_INITIALIZE;
    if (needsEarlyStackCheck()) {
        phase = CHECK_OVER_RECURSED;
    }
=======
template <>
void BaselineCompilerCodeGen::pushGlobalLexicalEnvironmentValue(
    ValueOperand scratch) {
  frame.push(ObjectValue(cx->global()->lexicalEnvironment()));
}

template <>
void BaselineInterpreterCodeGen::pushGlobalLexicalEnvironmentValue(
    ValueOperand scratch) {
  loadGlobalLexicalEnvironment(scratch.scratchReg());
  masm.tagValue(JSVAL_TYPE_OBJECT, scratch.scratchReg(), scratch);
  frame.push(scratch);
}

template <>
void BaselineCompilerCodeGen::loadGlobalThisValue(ValueOperand dest) {
  masm.moveValue(cx->global()->lexicalEnvironment().thisValue(), dest);
}

template <>
void BaselineInterpreterCodeGen::loadGlobalThisValue(ValueOperand dest) {
  Register scratch = dest.scratchReg();
  loadGlobalLexicalEnvironment(scratch);
  static constexpr size_t SlotOffset =
      LexicalEnvironmentObject::offsetOfThisValueOrScopeSlot();
  masm.loadValue(Address(scratch, SlotOffset), dest);
}

template <>
void BaselineCompilerCodeGen::pushScriptArg() {
  pushArg(ImmGCPtr(handler.script()));
}

template <>
void BaselineInterpreterCodeGen::pushScriptArg() {
  pushArg(frame.addressOfInterpreterScript());
}

template <>
void BaselineCompilerCodeGen::pushBytecodePCArg() {
  pushArg(ImmPtr(handler.pc()));
}

template <>
void BaselineInterpreterCodeGen::pushBytecodePCArg() {
  if (HasInterpreterPCReg()) {
    pushArg(InterpreterPCReg);
  } else {
    pushArg(frame.addressOfInterpreterPC());
  }
}

template <>
void BaselineCompilerCodeGen::pushScriptNameArg(Register scratch1,
                                                Register scratch2) {
  pushArg(ImmGCPtr(handler.script()->getName(handler.pc())));
}

template <>
void BaselineInterpreterCodeGen::pushScriptNameArg(Register scratch1,
                                                   Register scratch2) {
  MOZ_ASSERT(scratch1 != scratch2);

  LoadInt32Operand(masm, scratch1);

  loadScriptAtom(scratch1, scratch2);
  pushArg(scratch2);
}

static gc::Cell* GetScriptGCThing(JSScript* script, jsbytecode* pc,
                                  ScriptGCThingType type) {
  switch (type) {
    case ScriptGCThingType::RegExp:
      return script->getRegExp(pc);
    case ScriptGCThingType::Function:
      return script->getFunction(pc);
    case ScriptGCThingType::Scope:
      return script->getScope(pc);
    case ScriptGCThingType::BigInt:
      return script->getBigInt(pc);
  }
  MOZ_CRASH("Unexpected GCThing type");
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (fun->needsFunctionEnvironmentObjects()) {
      // Call into the VM to create the proper environment objects.
      prepareVMCall();
||||||| merged common ancestors
    RootedFunction fun(cx, function());
    if (fun) {
        // Use callee->environment as env chain. Note that we do this also
        // for needsSomeEnvironmentObject functions, so that the env chain
        // slot is properly initialized if the call triggers GC.
        Register callee = R0.scratchReg();
        Register scope = R1.scratchReg();
        masm.loadFunctionFromCalleeToken(frame.addressOfCalleeToken(), callee);
        masm.loadPtr(Address(callee, JSFunction::offsetOfEnvironment()), scope);
        masm.storePtr(scope, frame.addressOfEnvironmentChain());

        if (fun->needsFunctionEnvironmentObjects()) {
            // Call into the VM to create the proper environment objects.
            prepareVMCall();

            masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
            pushArg(R0.scratchReg());

            if (!callVMNonOp(InitFunctionEnvironmentObjectsInfo, phase)) {
                return false;
            }
        }
    } else if (module()) {
        // Modules use a pre-created scope object.
        Register scope = R1.scratchReg();
        masm.movePtr(ImmGCPtr(&module()->initialEnvironment()), scope);
        masm.storePtr(scope, frame.addressOfEnvironmentChain());
    } else {
        // EnvironmentChain pointer in BaselineFrame has already been initialized
        // in prologue, but we need to check for redeclaration errors.
=======
template <>
void BaselineCompilerCodeGen::loadScriptGCThing(ScriptGCThingType type,
                                                Register dest,
                                                Register scratch) {
  gc::Cell* thing = GetScriptGCThing(handler.script(), handler.pc(), type);
  masm.movePtr(ImmGCPtr(thing), dest);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
      masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
      pushArg(R0.scratchReg());
||||||| merged common ancestors
        prepareVMCall();
        masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
        pushArg(R0.scratchReg());
=======
template <>
void BaselineInterpreterCodeGen::loadScriptGCThing(ScriptGCThingType type,
                                                   Register dest,
                                                   Register scratch) {
  MOZ_ASSERT(dest != scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (!callVMNonOp(InitFunctionEnvironmentObjectsInfo, phase)) {
        return false;
      }
    }
  } else if (module()) {
    // Modules use a pre-created scope object.
    Register scope = R1.scratchReg();
    masm.movePtr(ImmGCPtr(&module()->initialEnvironment()), scope);
    masm.storePtr(scope, frame.addressOfEnvironmentChain());
  } else {
    // EnvironmentChain pointer in BaselineFrame has already been initialized
    // in prologue, but we need to check for redeclaration errors.
||||||| merged common ancestors
        if (!callVMNonOp(CheckGlobalOrEvalDeclarationConflictsInfo, phase)) {
            return false;
        }
    }
=======
  // Load the index in |scratch|.
  LoadInt32Operand(masm, scratch);

  // Load the GCCellPtr.
  loadScript(dest);
  masm.loadPtr(Address(dest, JSScript::offsetOfPrivateScriptData()), dest);
  masm.loadPtr(BaseIndex(dest, scratch, ScalePointer,
                         PrivateScriptData::offsetOfGCThings()),
               dest);

  // Clear the tag bits.
  switch (type) {
    case ScriptGCThingType::RegExp:
    case ScriptGCThingType::Function:
      // No-op because GCCellPtr tag bits are zero for objects.
      static_assert(uintptr_t(TraceKind::Object) == 0,
                    "Unexpected tag bits for object GCCellPtr");
      break;
    case ScriptGCThingType::Scope:
    case ScriptGCThingType::BigInt:
      // Use xorPtr with a 32-bit immediate because it's more efficient than
      // andPtr on 64-bit.
      static_assert(uintptr_t(TraceKind::Scope) >= JS::OutOfLineTraceKindMask,
                    "Expected Scopes to have OutOfLineTraceKindMask tag");
      static_assert(uintptr_t(TraceKind::BigInt) >= JS::OutOfLineTraceKindMask,
                    "Expected BigInts to have OutOfLineTraceKindMask tag");
      masm.xorPtr(Imm32(JS::OutOfLineTraceKindMask), dest);
      break;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    prepareVMCall();
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
    pushArg(R0.scratchReg());

    if (!callVMNonOp(CheckGlobalOrEvalDeclarationConflictsInfo, phase)) {
      return false;
    }
  }

  return true;
||||||| merged common ancestors
    return true;
=======
#ifdef DEBUG
  // Assert low bits are not set.
  Label ok;
  masm.branchTestPtr(Assembler::Zero, dest, Imm32(0b111), &ok);
  masm.assumeUnreachable("GC pointer with tag bits set");
  masm.bind(&ok);
#endif
>>>>>>> upstream-releases
}

template <>
void BaselineCompilerCodeGen::pushScriptGCThingArg(ScriptGCThingType type,
                                                   Register scratch1,
                                                   Register scratch2) {
  gc::Cell* thing = GetScriptGCThing(handler.script(), handler.pc(), type);
  pushArg(ImmGCPtr(thing));
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitInterruptCheck() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emitInterruptCheck()
{
    frame.syncStack(0);
=======
template <>
void BaselineInterpreterCodeGen::pushScriptGCThingArg(ScriptGCThingType type,
                                                      Register scratch1,
                                                      Register scratch2) {
  loadScriptGCThing(type, scratch1, scratch2);
  pushArg(scratch1);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label done;
  masm.branch32(Assembler::Equal, AbsoluteAddress(cx->addressOfInterruptBits()),
                Imm32(0), &done);
||||||| merged common ancestors
    Label done;
    masm.branch32(Assembler::Equal,
                  AbsoluteAddress(cx->addressOfInterruptBits()), Imm32(0),
                  &done);
=======
template <>
void BaselineCompilerCodeGen::pushUint8BytecodeOperandArg(Register) {
  MOZ_ASSERT(JOF_OPTYPE(JSOp(*handler.pc())) == JOF_UINT8);
  pushArg(Imm32(GET_UINT8(handler.pc())));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  if (!callVM(InterruptCheckInfo)) {
    return false;
  }
||||||| merged common ancestors
    prepareVMCall();
    if (!callVM(InterruptCheckInfo)) {
        return false;
    }
=======
template <>
void BaselineInterpreterCodeGen::pushUint8BytecodeOperandArg(Register scratch) {
  LoadUint8Operand(masm, scratch);
  pushArg(scratch);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&done);
  return true;
||||||| merged common ancestors
    masm.bind(&done);
    return true;
=======
template <>
void BaselineCompilerCodeGen::pushUint16BytecodeOperandArg(Register) {
  MOZ_ASSERT(JOF_OPTYPE(JSOp(*handler.pc())) == JOF_UINT16);
  pushArg(Imm32(GET_UINT16(handler.pc())));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*IonCompileScriptForBaselineFn)(JSContext*, BaselineFrame*,
                                              jsbytecode*);
static const VMFunction IonCompileScriptForBaselineInfo =
    FunctionInfo<IonCompileScriptForBaselineFn>(IonCompileScriptForBaseline,
                                                "IonCompileScriptForBaseline");
||||||| merged common ancestors
typedef bool (*IonCompileScriptForBaselineFn)(JSContext*, BaselineFrame*, jsbytecode*);
static const VMFunction IonCompileScriptForBaselineInfo =
    FunctionInfo<IonCompileScriptForBaselineFn>(IonCompileScriptForBaseline,
                                                "IonCompileScriptForBaseline");
=======
template <>
void BaselineInterpreterCodeGen::pushUint16BytecodeOperandArg(
    Register scratch) {
  LoadUint16Operand(masm, scratch);
  pushArg(scratch);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitWarmUpCounterIncrement(bool allowOsr) {
  // Emit no warm-up counter increments or bailouts if Ion is not
  // enabled, or if the script will never be Ion-compileable
||||||| merged common ancestors
bool
BaselineCompiler::emitWarmUpCounterIncrement(bool allowOsr)
{
    // Emit no warm-up counter increments or bailouts if Ion is not
    // enabled, or if the script will never be Ion-compileable

    if (!ionCompileable_) {
        return true;
    }
=======
template <>
void BaselineCompilerCodeGen::loadInt32LengthBytecodeOperand(Register dest) {
  uint32_t length = GET_UINT32(handler.pc());
  MOZ_ASSERT(length <= INT32_MAX,
             "the bytecode emitter must fail to compile code that would "
             "produce a length exceeding int32_t range");
  masm.move32(Imm32(AssertedCast<int32_t>(length)), dest);
}

template <>
void BaselineInterpreterCodeGen::loadInt32LengthBytecodeOperand(Register dest) {
  LoadInt32Operand(masm, dest);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!ionCompileable_) {
    return true;
  }
||||||| merged common ancestors
    frame.assertSyncedStack();
=======
template <>
void BaselineCompilerCodeGen::loadInt32IndexBytecodeOperand(ValueOperand dest) {
  uint32_t index = GET_UINT32(handler.pc());
  MOZ_ASSERT(index <= INT32_MAX,
             "the bytecode emitter must fail to compile code that would "
             "produce an index exceeding int32_t range");
  masm.moveValue(Int32Value(AssertedCast<int32_t>(index)), dest);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.assertSyncedStack();
||||||| merged common ancestors
    Register scriptReg = R2.scratchReg();
    Register countReg = R0.scratchReg();
    Address warmUpCounterAddr(scriptReg, JSScript::offsetOfWarmUpCounter());
=======
template <>
void BaselineInterpreterCodeGen::loadInt32IndexBytecodeOperand(
    ValueOperand dest) {
  Register scratch = dest.scratchReg();
  LoadInt32Operand(masm, scratch);
  masm.tagValue(JSVAL_TYPE_INT32, scratch, dest);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register scriptReg = R2.scratchReg();
  Register countReg = R0.scratchReg();
  Address warmUpCounterAddr(scriptReg, JSScript::offsetOfWarmUpCounter());
||||||| merged common ancestors
    masm.movePtr(ImmGCPtr(script), scriptReg);
    masm.load32(warmUpCounterAddr, countReg);
    masm.add32(Imm32(1), countReg);
    masm.store32(countReg, warmUpCounterAddr);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitDebugPrologue() {
  auto ifDebuggee = [this]() {
    // Load pointer to BaselineFrame in R0.
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.movePtr(ImmGCPtr(script), scriptReg);
  masm.load32(warmUpCounterAddr, countReg);
  masm.add32(Imm32(1), countReg);
  masm.store32(countReg, warmUpCounterAddr);
||||||| merged common ancestors
    // If this is a loop inside a catch or finally block, increment the warmup
    // counter but don't attempt OSR (Ion only compiles the try block).
    if (analysis_.info(pc).loopEntryInCatchOrFinally) {
        MOZ_ASSERT(JSOp(*pc) == JSOP_LOOPENTRY);
        return true;
    }
=======
    prepareVMCall();
    pushBytecodePCArg();
    pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If this is a loop inside a catch or finally block, increment the warmup
  // counter but don't attempt OSR (Ion only compiles the try block).
  if (analysis_.info(pc).loopEntryInCatchOrFinally) {
    MOZ_ASSERT(JSOp(*pc) == JSOP_LOOPENTRY);
    return true;
  }
||||||| merged common ancestors
    // OSR not possible at this loop entry.
    if (!allowOsr) {
        MOZ_ASSERT(JSOp(*pc) == JSOP_LOOPENTRY);
        return true;
    }
=======
    using Fn = bool (*)(JSContext*, BaselineFrame*, jsbytecode*, bool*);
    if (!callVM<Fn, jit::DebugPrologue>()) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // OSR not possible at this loop entry.
  if (!allowOsr) {
    MOZ_ASSERT(JSOp(*pc) == JSOP_LOOPENTRY);
    return true;
  }

  Label skipCall;

  const OptimizationInfo* info =
      IonOptimizations.get(IonOptimizations.firstLevel());
  uint32_t warmUpThreshold = info->compilerWarmUpThreshold(script, pc);
  masm.branch32(Assembler::LessThan, countReg, Imm32(warmUpThreshold),
                &skipCall);

  masm.branchPtr(Assembler::Equal,
                 Address(scriptReg, JSScript::offsetOfIonScript()),
                 ImmPtr(ION_COMPILING_SCRIPT), &skipCall);
||||||| merged common ancestors
    Label skipCall;

    const OptimizationInfo* info = IonOptimizations.get(IonOptimizations.firstLevel());
    uint32_t warmUpThreshold = info->compilerWarmUpThreshold(script, pc);
    masm.branch32(Assembler::LessThan, countReg, Imm32(warmUpThreshold), &skipCall);

    masm.branchPtr(Assembler::Equal,
                   Address(scriptReg, JSScript::offsetOfIonScript()),
                   ImmPtr(ION_COMPILING_SCRIPT), &skipCall);

    // Try to compile and/or finish a compilation.
    if (JSOp(*pc) == JSOP_LOOPENTRY) {
        // During the loop entry we can try to OSR into ion.
        // The ic has logic for this.
        ICWarmUpCounter_Fallback::Compiler stubCompiler(cx);
        if (!emitNonOpIC(stubCompiler.getStub(&stubSpace_))) {
            return false;
        }
    } else {
        // To call stubs we need to have an opcode. This code handles the
        // prologue and there is no dedicatd opcode present. Therefore use an
        // annotated vm call.
        prepareVMCall();
=======
    // Fix up the RetAddrEntry appended by callVM for on-stack recompilation.
    handler.markLastRetAddrEntryKind(RetAddrEntry::Kind::DebugPrologue);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Try to compile and/or finish a compilation.
  if (JSOp(*pc) == JSOP_LOOPENTRY) {
    // During the loop entry we can try to OSR into ion.
    // The ic has logic for this.
    if (!emitNextIC()) {
      return false;
    }
  } else {
    // To call stubs we need to have an opcode. This code handles the
    // prologue and there is no dedicatd opcode present. Therefore use an
    // annotated vm call.
    prepareVMCall();
||||||| merged common ancestors
        masm.Push(ImmPtr(pc));
        masm.PushBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
=======
    // If the stub returns |true|, we have to return the value stored in the
    // frame's return value slot.
    Label done;
    masm.branchTest32(Assembler::Zero, ReturnReg, ReturnReg, &done);
    {
      masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
      masm.jump(&return_);
    }
    masm.bind(&done);
    return true;
  };
  if (!emitDebugInstrumentation(ifDebuggee)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    masm.Push(ImmPtr(pc));
    masm.PushBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
||||||| merged common ancestors
        if (!callVM(IonCompileScriptForBaselineInfo)) {
            return false;
        }
=======
  debugOsrPrologueOffset_ = CodeOffset(masm.currentOffset());
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!callVM(IonCompileScriptForBaselineInfo)) {
      return false;
    }
||||||| merged common ancestors
        // Annotate the ICEntry as warmup counter.
        icEntries_.back().setFakeKind(ICEntry::Kind_WarmupCounter);
    }
    masm.bind(&skipCall);
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Annotate the RetAddrEntry as warmup counter.
    retAddrEntries_.back().setKind(RetAddrEntry::Kind::WarmupCounter);
  }
  masm.bind(&skipCall);

  return true;
||||||| merged common ancestors
    return true;
=======
template <>
void BaselineCompilerCodeGen::emitPreInitEnvironmentChain(
    Register nonFunctionEnv) {
  if (handler.function()) {
    masm.storePtr(ImmPtr(nullptr), frame.addressOfEnvironmentChain());
  } else {
    masm.storePtr(nonFunctionEnv, frame.addressOfEnvironmentChain());
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCompiler::emitArgumentTypeChecks() {
  if (!function()) {
    return true;
  }

  frame.pushThis();
  frame.popRegsAndSync(1);

  if (!emitNextIC()) {
    return false;
  }
||||||| merged common ancestors
bool
BaselineCompiler::emitArgumentTypeChecks()
{
    if (!function()) {
        return true;
    }

    frame.pushThis();
    frame.popRegsAndSync(1);

    ICTypeMonitor_Fallback::Compiler compiler(cx, uint32_t(0));
    if (!emitNonOpIC(compiler.getStub(&stubSpace_))) {
        return false;
    }
=======
template <>
void BaselineInterpreterCodeGen::emitPreInitEnvironmentChain(
    Register nonFunctionEnv) {
  Label notFunction, done;
  masm.branchTestPtr(Assembler::NonZero, frame.addressOfCalleeToken(),
                     Imm32(CalleeTokenScriptBit), &notFunction);
  {
    masm.storePtr(ImmPtr(nullptr), frame.addressOfEnvironmentChain());
    masm.jump(&done);
  }
  masm.bind(&notFunction);
  { masm.storePtr(nonFunctionEnv, frame.addressOfEnvironmentChain()); }
  masm.bind(&done);
}

template <>
void BaselineCompilerCodeGen::emitInitFrameFields() {
  masm.store32(Imm32(0), frame.addressOfFlags());
}

template <>
void BaselineInterpreterCodeGen::emitInitFrameFields() {
  // If we have a dedicated PC register we use it as scratch1 to avoid a
  // register move below.
  Register scratch1 =
      HasInterpreterPCReg() ? InterpreterPCReg : R0.scratchReg();
  Register scratch2 = R2.scratchReg();

  masm.store32(Imm32(BaselineFrame::RUNNING_IN_INTERPRETER),
               frame.addressOfFlags());

  // Initialize interpreterScript.
  Label notFunction, done;
  masm.loadPtr(frame.addressOfCalleeToken(), scratch1);
  masm.branchTestPtr(Assembler::NonZero, scratch1, Imm32(CalleeTokenScriptBit),
                     &notFunction);
  {
    // CalleeToken_Function or CalleeToken_FunctionConstructing.
    masm.andPtr(Imm32(uint32_t(CalleeTokenMask)), scratch1);
    masm.loadPtr(Address(scratch1, JSFunction::offsetOfScript()), scratch1);
    masm.jump(&done);
  }
  masm.bind(&notFunction);
  {
    // CalleeToken_Script.
    masm.andPtr(Imm32(uint32_t(CalleeTokenMask)), scratch1);
  }
  masm.bind(&done);
  masm.storePtr(scratch1, frame.addressOfInterpreterScript());

  // Initialize interpreterICEntry.
  masm.loadPtr(Address(scratch1, JSScript::offsetOfJitScript()), scratch2);
  masm.computeEffectiveAddress(
      Address(scratch2, JitScript::offsetOfICEntries()), scratch2);
  masm.storePtr(scratch2, frame.addressOfInterpreterICEntry());

  // Initialize interpreter pc.
  masm.loadPtr(Address(scratch1, JSScript::offsetOfScriptData()), scratch1);
  masm.load32(Address(scratch1, SharedScriptData::offsetOfCodeOffset()),
              scratch2);
  masm.addPtr(scratch2, scratch1);

  if (HasInterpreterPCReg()) {
    MOZ_ASSERT(scratch1 == InterpreterPCReg,
               "pc must be stored in the pc register");
  } else {
    masm.storePtr(scratch1, frame.addressOfInterpreterPC());
  }
}

template <>
template <typename F1, typename F2>
bool BaselineCompilerCodeGen::initEnvironmentChainHelper(
    const F1& initFunctionEnv, const F2& initGlobalOrEvalEnv,
    Register scratch) {
  if (handler.function()) {
    return initFunctionEnv();
  }
  if (handler.module()) {
    return true;
  }
  return initGlobalOrEvalEnv();
}

template <>
template <typename F1, typename F2>
bool BaselineInterpreterCodeGen::initEnvironmentChainHelper(
    const F1& initFunctionEnv, const F2& initGlobalOrEvalEnv,
    Register scratch) {
  // There are three cases:
  //
  // 1) Function script: use code emitted by initFunctionEnv.
  // 2) Module script: no-op.
  // 3) Global or eval script: use code emitted by initGlobalOrEvalEnv.

  Label notFunction, done;
  masm.loadPtr(frame.addressOfCalleeToken(), scratch);
  masm.branchTestPtr(Assembler::NonZero, scratch, Imm32(CalleeTokenScriptBit),
                     &notFunction);
  {
    if (!initFunctionEnv()) {
      return false;
    }
    masm.jump(&done);
  }
  masm.bind(&notFunction);
  {
    masm.andPtr(Imm32(uint32_t(CalleeTokenMask)), scratch);
    masm.branchTest32(Assembler::NonZero,
                      Address(scratch, JSScript::offsetOfImmutableFlags()),
                      Imm32(uint32_t(JSScript::ImmutableFlags::IsModule)),
                      &done);
    {
      if (!initGlobalOrEvalEnv()) {
        return false;
      }
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (size_t i = 0; i < function()->nargs(); i++) {
    frame.pushArg(i);
    frame.popRegsAndSync(1);

    if (!emitNextIC()) {
      return false;
    }
  }

  return true;
||||||| merged common ancestors
    for (size_t i = 0; i < function()->nargs(); i++) {
        frame.pushArg(i);
        frame.popRegsAndSync(1);

        ICTypeMonitor_Fallback::Compiler compiler(cx, i + 1);
        if (!emitNonOpIC(compiler.getStub(&stubSpace_))) {
            return false;
        }
    }

    return true;
=======
  masm.bind(&done);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCompiler::emitDebugTrap() {
  MOZ_ASSERT(compileDebugInstrumentation());
  MOZ_ASSERT(frame.numUnsyncedSlots() == 0);
||||||| merged common ancestors
bool
BaselineCompiler::emitDebugTrap()
{
    MOZ_ASSERT(compileDebugInstrumentation_);
    MOZ_ASSERT(frame.numUnsyncedSlots() == 0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::initEnvironmentChain() {
  CallVMPhase phase = POST_INITIALIZE;
  if (handler.needsEarlyStackCheck()) {
    phase = CHECK_OVER_RECURSED;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool enabled = script->stepModeEnabled() || script->hasBreakpointsAt(pc);
||||||| merged common ancestors
    bool enabled = script->stepModeEnabled() || script->hasBreakpointsAt(pc);
=======
  auto initFunctionEnv = [this, phase]() {
    // Use callee->environment as env chain. Note that we do this also
    // for needsSomeEnvironmentObject functions, so that the env chain
    // slot is properly initialized if the call triggers GC.
    Register callee = R0.scratchReg();
    Register scope = R1.scratchReg();
    masm.loadFunctionFromCalleeToken(frame.addressOfCalleeToken(), callee);
    masm.loadPtr(Address(callee, JSFunction::offsetOfEnvironment()), scope);
    masm.storePtr(scope, frame.addressOfEnvironmentChain());

    auto initEnv = [this, phase]() {
      // Call into the VM to create the proper environment objects.
      prepareVMCall();

      masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
      pushArg(R0.scratchReg());

      using Fn = bool (*)(JSContext*, BaselineFrame*);
      return callVMNonOp<Fn, jit::InitFunctionEnvironmentObjects>(phase);
    };
    return emitTestScriptFlag(
        JSScript::ImmutableFlags::NeedsFunctionEnvironmentObjects, true,
        initEnv, R2.scratchReg());
  };

  auto initGlobalOrEvalEnv = [this, phase]() {
    // EnvironmentChain pointer in BaselineFrame has already been initialized
    // in prologue, but we need to check for redeclaration errors in global and
    // eval scripts.
>>>>>>> upstream-releases

<<<<<<< HEAD
#if defined(JS_CODEGEN_ARM64)
  // Flush any pending constant pools to prevent incorrect
  // PCMappingEntry offsets. See Bug 1446819.
  masm.flush();
  // Fix up the PCMappingEntry to avoid any constant pool.
  pcMappingEntries_.back().nativeOffset = masm.currentOffset();
#endif
||||||| merged common ancestors
#if defined(JS_CODEGEN_ARM64)
    // Flush any pending constant pools to prevent incorrect
    // PCMappingEntry offsets. See Bug 1446819.
    masm.flush();
    // Fix up the PCMappingEntry to avoid any constant pool.
    pcMappingEntries_.back().nativeOffset = masm.currentOffset();
#endif
=======
    prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Emit patchable call to debug trap handler.
  JitCode* handler = cx->runtime()->jitRuntime()->debugTrapHandler(cx);
  if (!handler) {
    return false;
  }
  mozilla::DebugOnly<CodeOffset> offset = masm.toggledCall(handler, enabled);
||||||| merged common ancestors
    // Emit patchable call to debug trap handler.
    JitCode* handler = cx->runtime()->jitRuntime()->debugTrapHandler(cx);
    if (!handler) {
        return false;
    }
    mozilla::DebugOnly<CodeOffset> offset = masm.toggledCall(handler, enabled);
=======
    pushScriptArg();
    masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
    pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef DEBUG
  // Patchable call offset has to match the pc mapping offset.
  PCMappingEntry& entry = pcMappingEntries_.back();
  MOZ_ASSERT((&offset)->offset() == entry.nativeOffset);
#endif
||||||| merged common ancestors
#ifdef DEBUG
    // Patchable call offset has to match the pc mapping offset.
    PCMappingEntry& entry = pcMappingEntries_.back();
    MOZ_ASSERT((&offset)->offset() == entry.nativeOffset);
#endif
=======
    using Fn = bool (*)(JSContext*, HandleObject, HandleScript);
    return callVMNonOp<Fn, js::CheckGlobalOrEvalDeclarationConflicts>(phase);
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Add a RetAddrEntry for the return offset -> pc mapping.
  return appendRetAddrEntry(RetAddrEntry::Kind::DebugTrap,
                            masm.currentOffset());
||||||| merged common ancestors
    // Add an IC entry for the return offset -> pc mapping.
    return appendICEntry(ICEntry::Kind_DebugTrap, masm.currentOffset());
=======
  return initEnvironmentChainHelper(initFunctionEnv, initGlobalOrEvalEnv,
                                    R2.scratchReg());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
#ifdef JS_TRACE_LOGGING
bool BaselineCompiler::emitTraceLoggerEnter() {
  AllocatableRegisterSet regs(RegisterSet::Volatile());
  Register loggerReg = regs.takeAnyGeneral();
  Register scriptReg = regs.takeAnyGeneral();

  Label noTraceLogger;
  if (!traceLoggerToggleOffsets_.append(masm.toggledJump(&noTraceLogger))) {
    return false;
  }
||||||| merged common ancestors
#ifdef JS_TRACE_LOGGING
bool
BaselineCompiler::emitTraceLoggerEnter()
{
    AllocatableRegisterSet regs(RegisterSet::Volatile());
    Register loggerReg = regs.takeAnyGeneral();
    Register scriptReg = regs.takeAnyGeneral();

    Label noTraceLogger;
    if (!traceLoggerToggleOffsets_.append(masm.toggledJump(&noTraceLogger))) {
        return false;
    }

    masm.Push(loggerReg);
    masm.Push(scriptReg);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitInterruptCheck() {
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.Push(loggerReg);
  masm.Push(scriptReg);
||||||| merged common ancestors
    masm.loadTraceLogger(loggerReg);
=======
  Label done;
  masm.branch32(Assembler::Equal, AbsoluteAddress(cx->addressOfInterruptBits()),
                Imm32(0), &done);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadTraceLogger(loggerReg);
||||||| merged common ancestors
    // Script start.
    masm.movePtr(ImmGCPtr(script), scriptReg);
    masm.loadPtr(Address(scriptReg, JSScript::offsetOfBaselineScript()), scriptReg);
    Address scriptEvent(scriptReg, BaselineScript::offsetOfTraceLoggerScriptEvent());
    masm.computeEffectiveAddress(scriptEvent, scriptReg);
    masm.tracelogStartEvent(loggerReg, scriptReg);
=======
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Script start.
  masm.movePtr(ImmGCPtr(script), scriptReg);
  masm.loadPtr(Address(scriptReg, JSScript::offsetOfBaselineScript()),
               scriptReg);
  Address scriptEvent(scriptReg,
                      BaselineScript::offsetOfTraceLoggerScriptEvent());
  masm.computeEffectiveAddress(scriptEvent, scriptReg);
  masm.tracelogStartEvent(loggerReg, scriptReg);
||||||| merged common ancestors
    // Engine start.
    masm.tracelogStartId(loggerReg, TraceLogger_Baseline, /* force = */ true);
=======
  using Fn = bool (*)(JSContext*);
  if (!callVM<Fn, InterruptCheck>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Engine start.
  masm.tracelogStartId(loggerReg, TraceLogger_Baseline, /* force = */ true);
||||||| merged common ancestors
    masm.Pop(scriptReg);
    masm.Pop(loggerReg);
=======
  masm.bind(&done);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.Pop(scriptReg);
  masm.Pop(loggerReg);
||||||| merged common ancestors
    masm.bind(&noTraceLogger);
=======
template <>
bool BaselineCompilerCodeGen::emitWarmUpCounterIncrement() {
  // Emit no warm-up counter increments or bailouts if Ion is not
  // enabled, or if the script will never be Ion-compileable
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&noTraceLogger);

  return true;
}
||||||| merged common ancestors
    return true;
}
=======
  if (!handler.maybeIonCompileable()) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCompiler::emitTraceLoggerExit() {
  AllocatableRegisterSet regs(RegisterSet::Volatile());
  Register loggerReg = regs.takeAnyGeneral();
||||||| merged common ancestors
bool
BaselineCompiler::emitTraceLoggerExit()
{
    AllocatableRegisterSet regs(RegisterSet::Volatile());
    Register loggerReg = regs.takeAnyGeneral();
=======
  frame.assertSyncedStack();
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label noTraceLogger;
  if (!traceLoggerToggleOffsets_.append(masm.toggledJump(&noTraceLogger))) {
    return false;
  }
||||||| merged common ancestors
    Label noTraceLogger;
    if (!traceLoggerToggleOffsets_.append(masm.toggledJump(&noTraceLogger))) {
        return false;
    }
=======
  Register scriptReg = R2.scratchReg();
  Register countReg = R0.scratchReg();
  Address warmUpCounterAddr(scriptReg, JSScript::offsetOfWarmUpCounter());

  JSScript* script = handler.script();
  masm.movePtr(ImmGCPtr(script), scriptReg);
  masm.load32(warmUpCounterAddr, countReg);
  masm.add32(Imm32(1), countReg);
  masm.store32(countReg, warmUpCounterAddr);

  jsbytecode* pc = handler.pc();
  if (JSOp(*pc) == JSOP_LOOPENTRY) {
    // If this is a loop inside a catch or finally block, increment the warmup
    // counter but don't attempt OSR (Ion only compiles the try block).
    if (handler.analysis().info(pc).loopEntryInCatchOrFinally) {
      return true;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.Push(loggerReg);
  masm.loadTraceLogger(loggerReg);
||||||| merged common ancestors
    masm.Push(loggerReg);
    masm.loadTraceLogger(loggerReg);
=======
    if (!LoopEntryCanIonOsr(pc)) {
      // OSR into Ion not possible at this loop entry.
      return true;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.tracelogStopId(loggerReg, TraceLogger_Baseline, /* force = */ true);
  masm.tracelogStopId(loggerReg, TraceLogger_Scripts, /* force = */ true);
||||||| merged common ancestors
    masm.tracelogStopId(loggerReg, TraceLogger_Baseline, /* force = */ true);
    masm.tracelogStopId(loggerReg, TraceLogger_Scripts, /* force = */ true);
=======
  Label skipCall;
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.Pop(loggerReg);
||||||| merged common ancestors
    masm.Pop(loggerReg);
=======
  const OptimizationInfo* info =
      IonOptimizations.get(IonOptimizations.firstLevel());
  uint32_t warmUpThreshold = info->compilerWarmUpThreshold(script, pc);
  masm.branch32(Assembler::LessThan, countReg, Imm32(warmUpThreshold),
                &skipCall);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&noTraceLogger);
||||||| merged common ancestors
    masm.bind(&noTraceLogger);
=======
  masm.branchPtr(Assembler::Equal,
                 Address(scriptReg, JSScript::offsetOfIonScript()),
                 ImmPtr(ION_COMPILING_SCRIPT), &skipCall);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
  // Try to compile and/or finish a compilation.
  if (JSOp(*pc) == JSOP_LOOPENTRY) {
    // During the loop entry we can try to OSR into ion.
    // The ic has logic for this.
    if (!emitNextIC()) {
      return false;
    }
  } else {
    // To call stubs we need to have an opcode. This code handles the
    // prologue and there is no dedicatd opcode present. Therefore use an
    // annotated vm call.
    prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitTraceLoggerResume(
    Register baselineScript, AllocatableGeneralRegisterSet& regs) {
  Register scriptId = regs.takeAny();
  Register loggerReg = regs.takeAny();
||||||| merged common ancestors
bool
BaselineCompiler::emitTraceLoggerResume(Register baselineScript, AllocatableGeneralRegisterSet& regs)
{
    Register scriptId = regs.takeAny();
    Register loggerReg = regs.takeAny();
=======
    pushBytecodePCArg();
    masm.PushBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label noTraceLogger;
  if (!traceLoggerToggleOffsets_.append(masm.toggledJump(&noTraceLogger))) {
    return false;
  }
||||||| merged common ancestors
    Label noTraceLogger;
    if (!traceLoggerToggleOffsets_.append(masm.toggledJump(&noTraceLogger))) {
        return false;
    }
=======
    using Fn = bool (*)(JSContext*, BaselineFrame*, jsbytecode*);
    if (!callVM<Fn, IonCompileScriptForBaseline>()) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadTraceLogger(loggerReg);
||||||| merged common ancestors
    masm.loadTraceLogger(loggerReg);
=======
    // Annotate the RetAddrEntry as warmup counter.
    handler.markLastRetAddrEntryKind(RetAddrEntry::Kind::WarmupCounter);
  }
  masm.bind(&skipCall);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Address scriptEvent(baselineScript,
                      BaselineScript::offsetOfTraceLoggerScriptEvent());
  masm.computeEffectiveAddress(scriptEvent, scriptId);
  masm.tracelogStartEvent(loggerReg, scriptId);
  masm.tracelogStartId(loggerReg, TraceLogger_Baseline, /* force = */ true);
||||||| merged common ancestors
    Address scriptEvent(baselineScript, BaselineScript::offsetOfTraceLoggerScriptEvent());
    masm.computeEffectiveAddress(scriptEvent, scriptId);
    masm.tracelogStartEvent(loggerReg, scriptId);
    masm.tracelogStartId(loggerReg, TraceLogger_Baseline, /* force = */ true);
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  regs.add(loggerReg);
  regs.add(scriptId);
||||||| merged common ancestors
    regs.add(loggerReg);
    regs.add(scriptId);
=======
template <>
bool BaselineInterpreterCodeGen::emitWarmUpCounterIncrement() {
  Register scriptReg = R2.scratchReg();
  Register countReg = R0.scratchReg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&noTraceLogger);
||||||| merged common ancestors
    masm.bind(&noTraceLogger);
=======
  // Bump warm-up counter.
  Address warmUpCounterAddr(scriptReg, JSScript::offsetOfWarmUpCounter());
  loadScript(scriptReg);
  masm.load32(warmUpCounterAddr, countReg);
  masm.add32(Imm32(1), countReg);
  masm.store32(countReg, warmUpCounterAddr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  // If the script is warm enough for Baseline compilation, call into the VM to
  // compile it.
  Label done;
  masm.branch32(Assembler::BelowOrEqual, countReg,
                Imm32(JitOptions.baselineWarmUpThreshold), &done);
  masm.branchPtr(Assembler::Equal,
                 Address(scriptReg, JSScript::offsetOfBaselineScript()),
                 ImmPtr(BASELINE_DISABLED_SCRIPT), &done);
  {
    prepareVMCall();

    masm.PushBaselineFramePtr(BaselineFrameReg, R0.scratchReg());

    using Fn = bool (*)(JSContext*, BaselineFrame*, uint8_t**);
    if (!callVM<Fn, BaselineCompileFromBaselineInterpreter>()) {
      return false;
    }

    // If the function returned nullptr we either skipped compilation or were
    // unable to compile the script. Continue running in the interpreter.
    masm.branchTestPtr(Assembler::Zero, ReturnReg, ReturnReg, &done);

    // Success! Switch from interpreter to JIT code by jumping to the
    // corresponding code in the BaselineScript.
    //
    // This works because BaselineCompiler uses the same frame layout (stack is
    // synced at OSR points) and BaselineCompileFromBaselineInterpreter has
    // already cleared the RUNNING_IN_INTERPRETER flag for us.
    // See BaselineFrame::prepareForBaselineInterpreterToJitOSR.
    masm.jump(ReturnReg);
  }

  masm.bind(&done);
  return true;
}

template <>
bool BaselineCompilerCodeGen::emitArgumentTypeChecks() {
  if (!handler.function()) {
    return true;
  }

  frame.pushThis();
  frame.popRegsAndSync(1);

  if (!emitNextIC()) {
    return false;
  }

  size_t nargs = handler.function()->nargs();

  for (size_t i = 0; i < nargs; i++) {
    frame.pushArg(i);
    frame.popRegsAndSync(1);

    if (!emitNextIC()) {
      return false;
    }
  }

  return true;
}

template <>
bool BaselineInterpreterCodeGen::emitArgumentTypeChecks() {
  Register scratch1 = R1.scratchReg();

  // If the script is not a function, we're done.
  Label done;
  masm.loadPtr(frame.addressOfCalleeToken(), scratch1);
  masm.branchTestPtr(Assembler::NonZero, scratch1, Imm32(CalleeTokenScriptBit),
                     &done);

  // CalleeToken_Function or CalleeToken_FunctionConstructing.
  masm.andPtr(Imm32(uint32_t(CalleeTokenMask)), scratch1);

  // Store nargs in the frame's scratch slot.
  masm.load16ZeroExtend(Address(scratch1, JSFunction::offsetOfNargs()),
                        scratch1);
  masm.store32(scratch1, frame.addressOfScratchValue());

  // Type check |this|.
  masm.loadValue(frame.addressOfThis(), R0);
  if (!emitNextIC()) {
    return false;
  }
  frame.bumpInterpreterICEntry();

  // Type check arguments. Scratch1 holds the next argument's index.
  masm.move32(Imm32(0), scratch1);

  // Bounds check.
  Label top;
  masm.bind(&top);
  masm.branch32(Assembler::Equal, frame.addressOfScratchValue(), scratch1,
                &done);
  {
    // Load the argument, increment argument index. Use the frame's return value
    // slot to store this index across the IC call.
    BaseValueIndex addr(BaselineFrameReg, scratch1,
                        BaselineFrame::offsetOfArg(0));
    masm.loadValue(addr, R0);
    masm.add32(Imm32(1), scratch1);
    masm.store32(scratch1, frame.addressOfReturnValue());

    // Type check the argument.
    if (!emitNextIC()) {
      return false;
    }
    frame.bumpInterpreterICEntry();

    // Restore argument index.
    masm.load32(frame.addressOfReturnValue(), scratch1);
    masm.jump(&top);
  }

  masm.bind(&done);
  return true;
>>>>>>> upstream-releases
}

bool BaselineCompiler::emitDebugTrap() {
  MOZ_ASSERT(compileDebugInstrumentation());
  MOZ_ASSERT(frame.numUnsyncedSlots() == 0);

  JSScript* script = handler.script();
  bool enabled =
      script->stepModeEnabled() || script->hasBreakpointsAt(handler.pc());

#if defined(JS_CODEGEN_ARM64)
  // Flush any pending constant pools to prevent incorrect
  // PCMappingEntry offsets. See Bug 1446819.
  masm.flush();
  // Fix up the PCMappingEntry to avoid any constant pool.
  pcMappingEntries_.back().nativeOffset = masm.currentOffset();
#endif

<<<<<<< HEAD
void BaselineCompiler::emitProfilerEnterFrame() {
  // Store stack position to lastProfilingFrame variable, guarded by a toggled
  // jump. Starts off initially disabled.
  Label noInstrument;
  CodeOffset toggleOffset = masm.toggledJump(&noInstrument);
  masm.profilerEnterFrame(masm.getStackPointer(), R0.scratchReg());
  masm.bind(&noInstrument);

  // Store the start offset in the appropriate location.
  MOZ_ASSERT(!profilerEnterFrameToggleOffset_.bound());
  profilerEnterFrameToggleOffset_ = toggleOffset;
}

void BaselineCompiler::emitProfilerExitFrame() {
  // Store previous frame to lastProfilingFrame variable, guarded by a toggled
  // jump. Starts off initially disabled.
  Label noInstrument;
  CodeOffset toggleOffset = masm.toggledJump(&noInstrument);
  masm.profilerExitFrame();
  masm.bind(&noInstrument);

  // Store the start offset in the appropriate location.
  MOZ_ASSERT(!profilerExitFrameToggleOffset_.bound());
  profilerExitFrameToggleOffset_ = toggleOffset;
}

MethodStatus BaselineCompiler::emitBody() {
  MOZ_ASSERT(pc == script->code());

  bool lastOpUnreachable = false;
  uint32_t emittedOps = 0;
  mozilla::DebugOnly<jsbytecode*> prevpc = pc;

  while (true) {
    JSOp op = JSOp(*pc);
    JitSpew(JitSpew_BaselineOp, "Compiling op @ %d: %s",
            int(script->pcToOffset(pc)), CodeName[op]);

    BytecodeInfo* info = analysis_.maybeInfo(pc);

    // Skip unreachable ops.
    if (!info) {
      // Test if last instructions and stop emitting in that case.
      pc += GetBytecodeLength(pc);
      if (pc >= script->codeEnd()) {
        break;
      }

      lastOpUnreachable = true;
      prevpc = pc;
      continue;
    }

    if (info->jumpTarget) {
      // Fully sync the stack if there are incoming jumps.
      frame.syncStack(0);
      frame.setStackDepth(info->stackDepth);
      masm.bind(handler.labelOf(pc));
    } else if (MOZ_UNLIKELY(compileDebugInstrumentation())) {
      // Also fully sync the stack if the debugger is enabled.
      frame.syncStack(0);
    } else {
      // At the beginning of any op, at most the top 2 stack-values are
      // unsynced.
      if (frame.stackDepth() > 2) {
        frame.syncStack(2);
      }
    }

    frame.assertValidState(*info);

    // Add a PC -> native mapping entry for the current op. These entries are
    // used when we need the native code address for a given pc, for instance
    // for bailouts from Ion, the debugger and exception handling. See
    // PCMappingIndexEntry for more information.
    bool addIndexEntry =
        (pc == script->code() || lastOpUnreachable || emittedOps > 100);
    if (addIndexEntry) {
      emittedOps = 0;
    }
    if (MOZ_UNLIKELY(!addPCMappingEntry(addIndexEntry))) {
      ReportOutOfMemory(cx);
      return Method_Error;
    }

    // Emit traps for breakpoints and step mode.
    if (MOZ_UNLIKELY(compileDebugInstrumentation()) && !emitDebugTrap()) {
      return Method_Error;
    }

    switch (op) {
      // ===== NOT Yet Implemented =====
      case JSOP_FORCEINTERPRETER:
        // Intentionally not implemented.
      case JSOP_SETINTRINSIC:
        // Run-once opcode during self-hosting initialization.
      case JSOP_UNUSED151:
      case JSOP_LIMIT:
        // === !! WARNING WARNING WARNING !! ===
        // Do you really want to sacrifice performance by not implementing
        // this operation in the BaselineCompiler?
        JitSpew(JitSpew_BaselineAbort, "Unhandled op: %s", CodeName[op]);
        return Method_CantCompile;

#define EMIT_OP(OP)                                            \
  case OP:                                                     \
    if (MOZ_UNLIKELY(!this->emit_##OP())) return Method_Error; \
    break;
        OPCODE_LIST(EMIT_OP)
#undef EMIT_OP
    }

    // If the main instruction is not a jump target, then we emit the
    //  corresponding code coverage counter.
    if (pc == script->main() && !BytecodeIsJumpTarget(op)) {
      if (!emit_JSOP_JUMPTARGET()) {
        return Method_Error;
      }
    }

    // Test if last instructions and stop emitting in that case.
    pc += GetBytecodeLength(pc);
    if (pc >= script->codeEnd()) {
      break;
    }

    emittedOps++;
    lastOpUnreachable = false;
||||||| merged common ancestors
void
BaselineCompiler::emitProfilerEnterFrame()
{
    // Store stack position to lastProfilingFrame variable, guarded by a toggled jump.
    // Starts off initially disabled.
    Label noInstrument;
    CodeOffset toggleOffset = masm.toggledJump(&noInstrument);
    masm.profilerEnterFrame(masm.getStackPointer(), R0.scratchReg());
    masm.bind(&noInstrument);

    // Store the start offset in the appropriate location.
    MOZ_ASSERT(!profilerEnterFrameToggleOffset_.bound());
    profilerEnterFrameToggleOffset_ = toggleOffset;
}

void
BaselineCompiler::emitProfilerExitFrame()
{
    // Store previous frame to lastProfilingFrame variable, guarded by a toggled jump.
    // Starts off initially disabled.
    Label noInstrument;
    CodeOffset toggleOffset = masm.toggledJump(&noInstrument);
    masm.profilerExitFrame();
    masm.bind(&noInstrument);

    // Store the start offset in the appropriate location.
    MOZ_ASSERT(!profilerExitFrameToggleOffset_.bound());
    profilerExitFrameToggleOffset_ = toggleOffset;
}

MethodStatus
BaselineCompiler::emitBody()
{
    MOZ_ASSERT(pc == script->code());

    bool lastOpUnreachable = false;
    uint32_t emittedOps = 0;
    mozilla::DebugOnly<jsbytecode*> prevpc = pc;

    while (true) {
        JSOp op = JSOp(*pc);
        JitSpew(JitSpew_BaselineOp, "Compiling op @ %d: %s",
                int(script->pcToOffset(pc)), CodeName[op]);

        BytecodeInfo* info = analysis_.maybeInfo(pc);

        // Skip unreachable ops.
        if (!info) {
            // Test if last instructions and stop emitting in that case.
            pc += GetBytecodeLength(pc);
            if (pc >= script->codeEnd()) {
                break;
            }

            lastOpUnreachable = true;
            prevpc = pc;
            continue;
        }

        if (info->jumpTarget) {
            // Fully sync the stack if there are incoming jumps.
            frame.syncStack(0);
            frame.setStackDepth(info->stackDepth);
            masm.bind(labelOf(pc));
        } else if (MOZ_UNLIKELY(compileDebugInstrumentation_)) {
            // Also fully sync the stack if the debugger is enabled.
            frame.syncStack(0);
        } else {
            // At the beginning of any op, at most the top 2 stack-values are unsynced.
            if (frame.stackDepth() > 2) {
                frame.syncStack(2);
            }
        }

        frame.assertValidState(*info);

        // Add a PC -> native mapping entry for the current op. These entries are
        // used when we need the native code address for a given pc, for instance
        // for bailouts from Ion, the debugger and exception handling. See
        // PCMappingIndexEntry for more information.
        bool addIndexEntry = (pc == script->code() || lastOpUnreachable || emittedOps > 100);
        if (addIndexEntry) {
            emittedOps = 0;
        }
        if (MOZ_UNLIKELY(!addPCMappingEntry(addIndexEntry))) {
            ReportOutOfMemory(cx);
            return Method_Error;
        }

        // Emit traps for breakpoints and step mode.
        if (MOZ_UNLIKELY(compileDebugInstrumentation_) && !emitDebugTrap()) {
            return Method_Error;
        }

        switch (op) {
          // ===== NOT Yet Implemented =====
          case JSOP_FORCEINTERPRETER:
            // Intentionally not implemented.
          case JSOP_SETINTRINSIC:
            // Run-once opcode during self-hosting initialization.
          case JSOP_UNUSED126:
          case JSOP_UNUSED206:
          case JSOP_LIMIT:
            // === !! WARNING WARNING WARNING !! ===
            // Do you really want to sacrifice performance by not implementing
            // this operation in the BaselineCompiler?
            JitSpew(JitSpew_BaselineAbort, "Unhandled op: %s", CodeName[op]);
            return Method_CantCompile;

#define EMIT_OP(OP)                            \
          case OP:                             \
            if (MOZ_UNLIKELY(!this->emit_##OP())) \
                return Method_Error;           \
            break;
OPCODE_LIST(EMIT_OP)
#undef EMIT_OP
        }

        // If the main instruction is not a jump target, then we emit the
        //  corresponding code coverage counter.
        if (pc == script->main() && !BytecodeIsJumpTarget(op)) {
            if (!emit_JSOP_JUMPTARGET()) {
                return Method_Error;
            }
        }

        // Test if last instructions and stop emitting in that case.
        pc += GetBytecodeLength(pc);
        if (pc >= script->codeEnd()) {
            break;
        }

        emittedOps++;
        lastOpUnreachable = false;
=======
  // Emit patchable call to debug trap handler.
  JitCode* handlerCode = cx->runtime()->jitRuntime()->debugTrapHandler(
      cx, DebugTrapHandlerKind::Compiler);
  if (!handlerCode) {
    return false;
  }
  mozilla::DebugOnly<CodeOffset> offset =
      masm.toggledCall(handlerCode, enabled);

>>>>>>> upstream-releases
#ifdef DEBUG
<<<<<<< HEAD
    prevpc = pc;
||||||| merged common ancestors
        prevpc = pc;
=======
  // Patchable call offset has to match the pc mapping offset.
  PCMappingEntry& entry = pcMappingEntries_.back();
  MOZ_ASSERT((&offset)->offset() == entry.nativeOffset);
>>>>>>> upstream-releases
#endif
<<<<<<< HEAD
  }
||||||| merged common ancestors
    }
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(JSOp(*prevpc) == JSOP_RETRVAL);
  return Method_Compiled;
||||||| merged common ancestors
    MOZ_ASSERT(JSOp(*prevpc) == JSOP_RETRVAL);
    return Method_Compiled;
=======
  // Add a RetAddrEntry for the return offset -> pc mapping.
  return handler.appendRetAddrEntry(cx, RetAddrEntry::Kind::DebugTrap,
                                    masm.currentOffset());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NOP() {
  return true;
}
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_NOP()
{
    return true;
}
=======
#ifdef JS_TRACE_LOGGING
template <>
bool BaselineCompilerCodeGen::emitTraceLoggerEnter() {
  AllocatableRegisterSet regs(RegisterSet::Volatile());
  Register loggerReg = regs.takeAnyGeneral();
  Register scriptReg = regs.takeAnyGeneral();
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ITERNEXT() {
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_ITERNEXT()
{
    return true;
=======
  Label noTraceLogger;
  if (!traceLoggerToggleOffsets_.append(masm.toggledJump(&noTraceLogger))) {
    return false;
  }

  masm.Push(loggerReg);
  masm.Push(scriptReg);

  masm.loadTraceLogger(loggerReg);

  // Script start.
  masm.movePtr(ImmGCPtr(handler.script()), scriptReg);
  masm.loadPtr(Address(scriptReg, JSScript::offsetOfBaselineScript()),
               scriptReg);
  Address scriptEvent(scriptReg,
                      BaselineScript::offsetOfTraceLoggerScriptEvent());
  masm.computeEffectiveAddress(scriptEvent, scriptReg);
  masm.tracelogStartEvent(loggerReg, scriptReg);

  // Engine start.
  masm.tracelogStartId(loggerReg, TraceLogger_Baseline, /* force = */ true);

  masm.Pop(scriptReg);
  masm.Pop(loggerReg);

  masm.bind(&noTraceLogger);

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NOP_DESTRUCTURING() {
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_NOP_DESTRUCTURING()
{
    return true;
=======
template <>
bool BaselineInterpreterCodeGen::emitTraceLoggerEnter() {
  if (JS::TraceLoggerSupported()) {
    MOZ_CRASH("NYI: interpreter emitTraceLoggerEnter");
  }
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TRY_DESTRUCTURING_ITERCLOSE() {
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_TRY_DESTRUCTURING_ITERCLOSE()
{
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitTraceLoggerExit() {
  AllocatableRegisterSet regs(RegisterSet::Volatile());
  Register loggerReg = regs.takeAnyGeneral();

  Label noTraceLogger;
  if (!traceLoggerToggleOffsets_.append(masm.toggledJump(&noTraceLogger))) {
    return false;
  }

  masm.Push(loggerReg);
  masm.loadTraceLogger(loggerReg);

  masm.tracelogStopId(loggerReg, TraceLogger_Baseline, /* force = */ true);
  masm.tracelogStopId(loggerReg, TraceLogger_Scripts, /* force = */ true);

  masm.Pop(loggerReg);

  masm.bind(&noTraceLogger);

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LABEL() {
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_LABEL()
{
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitTraceLoggerResume(
    Register baselineScript, AllocatableGeneralRegisterSet& regs) {
  Register scriptId = regs.takeAny();
  Register loggerReg = regs.takeAny();

  Label noTraceLogger;
  if (!traceLoggerToggleOffsets_.append(masm.toggledJump(&noTraceLogger))) {
    return false;
  }

  masm.loadTraceLogger(loggerReg);

  Address scriptEvent(baselineScript,
                      BaselineScript::offsetOfTraceLoggerScriptEvent());
  masm.computeEffectiveAddress(scriptEvent, scriptId);
  masm.tracelogStartEvent(loggerReg, scriptId);
  masm.tracelogStartId(loggerReg, TraceLogger_Baseline, /* force = */ true);

  regs.add(loggerReg);
  regs.add(scriptId);

  masm.bind(&noTraceLogger);

  return true;
>>>>>>> upstream-releases
}
#endif

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_POP() {
  frame.pop();
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_POP()
{
    frame.pop();
    return true;
=======
template <typename Handler>
void BaselineCodeGen<Handler>::emitProfilerEnterFrame() {
  // Store stack position to lastProfilingFrame variable, guarded by a toggled
  // jump. Starts off initially disabled.
  Label noInstrument;
  CodeOffset toggleOffset = masm.toggledJump(&noInstrument);
  masm.profilerEnterFrame(masm.getStackPointer(), R0.scratchReg());
  masm.bind(&noInstrument);

  // Store the start offset in the appropriate location.
  MOZ_ASSERT(!profilerEnterFrameToggleOffset_.bound());
  profilerEnterFrameToggleOffset_ = toggleOffset;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_POPN() {
  frame.popn(GET_UINT16(pc));
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_POPN()
{
    frame.popn(GET_UINT16(pc));
    return true;
=======
template <typename Handler>
void BaselineCodeGen<Handler>::emitProfilerExitFrame() {
  // Store previous frame to lastProfilingFrame variable, guarded by a toggled
  // jump. Starts off initially disabled.
  Label noInstrument;
  CodeOffset toggleOffset = masm.toggledJump(&noInstrument);
  masm.profilerExitFrame();
  masm.bind(&noInstrument);

  // Store the start offset in the appropriate location.
  MOZ_ASSERT(!profilerExitFrameToggleOffset_.bound());
  profilerExitFrameToggleOffset_ = toggleOffset;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DUPAT() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_DUPAT()
{
    frame.syncStack(0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NOP() {
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // DUPAT takes a value on the stack and re-pushes it on top.  It's like
  // GETLOCAL but it addresses from the top of the stack instead of from the
  // stack frame.
||||||| merged common ancestors
    // DUPAT takes a value on the stack and re-pushes it on top.  It's like
    // GETLOCAL but it addresses from the top of the stack instead of from the
    // stack frame.
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ITERNEXT() {
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  int depth = -(GET_UINT24(pc) + 1);
  masm.loadValue(frame.addressOfStackValue(frame.peek(depth)), R0);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    int depth = -(GET_UINT24(pc) + 1);
    masm.loadValue(frame.addressOfStackValue(frame.peek(depth)), R0);
    frame.push(R0);
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NOP_DESTRUCTURING() {
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DUP() {
  // Keep top stack value in R0, sync the rest so that we can use R1. We use
  // separate registers because every register can be used by at most one
  // StackValue.
  frame.popRegsAndSync(1);
  masm.moveValue(R0, R1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_DUP()
{
    // Keep top stack value in R0, sync the rest so that we can use R1. We use
    // separate registers because every register can be used by at most one
    // StackValue.
    frame.popRegsAndSync(1);
    masm.moveValue(R0, R1);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TRY_DESTRUCTURING() {
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // inc/dec ops use DUP followed by ONE, ADD. Push R0 last to avoid a move.
  frame.push(R1);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    // inc/dec ops use DUP followed by ONE, ADD. Push R0 last to avoid a move.
    frame.push(R1);
    frame.push(R0);
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LABEL() {
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DUP2() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_DUP2()
{
    frame.syncStack(0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_POP() {
  frame.pop();
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R1);
||||||| merged common ancestors
    masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R1);
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_POPN() {
  frame.popn(GET_UINT16(handler.pc()));
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.push(R0);
  frame.push(R1);
  return true;
||||||| merged common ancestors
    frame.push(R0);
    frame.push(R1);
    return true;
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_POPN() {
  LoadUint16Operand(masm, R0.scratchReg());
  frame.popn(R0.scratchReg());
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SWAP() {
  // Keep top stack values in R0 and R1.
  frame.popRegsAndSync(2);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SWAP()
{
    // Keep top stack values in R0 and R1.
    frame.popRegsAndSync(2);
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_DUPAT() {
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.push(R1);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    frame.push(R1);
    frame.push(R0);
    return true;
=======
  // DUPAT takes a value on the stack and re-pushes it on top.  It's like
  // GETLOCAL but it addresses from the top of the stack instead of from the
  // stack frame.

  int depth = -(GET_UINT24(handler.pc()) + 1);
  masm.loadValue(frame.addressOfStackValue(depth), R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_PICK() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_PICK()
{
    frame.syncStack(0);
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_DUPAT() {
  LoadUint24Operand(masm, 0, R0.scratchReg());
  masm.loadValue(frame.addressOfStackValue(R0.scratchReg()), R0);
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Pick takes a value on the stack and moves it to the top.
  // For instance, pick 2:
  //     before: A B C D E
  //     after : A B D E C
||||||| merged common ancestors
    // Pick takes a value on the stack and moves it to the top.
    // For instance, pick 2:
    //     before: A B C D E
    //     after : A B D E C

    // First, move value at -(amount + 1) into R0.
    int32_t depth = -(GET_INT8(pc) + 1);
    masm.loadValue(frame.addressOfStackValue(frame.peek(depth)), R0);

    // Move the other values down.
    depth++;
    for (; depth < 0; depth++) {
        Address source = frame.addressOfStackValue(frame.peek(depth));
        Address dest = frame.addressOfStackValue(frame.peek(depth - 1));
        masm.loadValue(source, R1);
        masm.storeValue(R1, dest);
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DUP() {
  // Keep top stack value in R0, sync the rest so that we can use R1. We use
  // separate registers because every register can be used by at most one
  // StackValue.
  frame.popRegsAndSync(1);
  masm.moveValue(R0, R1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // First, move value at -(amount + 1) into R0.
  int32_t depth = -(GET_INT8(pc) + 1);
  masm.loadValue(frame.addressOfStackValue(frame.peek(depth)), R0);

  // Move the other values down.
  depth++;
  for (; depth < 0; depth++) {
    Address source = frame.addressOfStackValue(frame.peek(depth));
    Address dest = frame.addressOfStackValue(frame.peek(depth - 1));
    masm.loadValue(source, R1);
    masm.storeValue(R1, dest);
  }

  // Push R0.
  frame.pop();
  frame.push(R0);
  return true;
||||||| merged common ancestors
    // Push R0.
    frame.pop();
    frame.push(R0);
    return true;
=======
  // inc/dec ops use DUP followed by ONE, ADD. Push R0 last to avoid a move.
  frame.push(R1);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_UNPICK() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_UNPICK()
{
    frame.syncStack(0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DUP2() {
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Pick takes the top of the stack value and moves it under the nth value.
  // For instance, unpick 2:
  //     before: A B C D E
  //     after : A B E C D

  // First, move value at -1 into R0.
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);

  // Move the other values up.
  int32_t depth = -(GET_INT8(pc) + 1);
  for (int32_t i = -1; i > depth; i--) {
    Address source = frame.addressOfStackValue(frame.peek(i - 1));
    Address dest = frame.addressOfStackValue(frame.peek(i));
    masm.loadValue(source, R1);
    masm.storeValue(R1, dest);
  }
||||||| merged common ancestors
    // Pick takes the top of the stack value and moves it under the nth value.
    // For instance, unpick 2:
    //     before: A B C D E
    //     after : A B E C D

    // First, move value at -1 into R0.
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);

    // Move the other values up.
    int32_t depth = -(GET_INT8(pc) + 1);
    for (int32_t i = -1; i > depth; i--) {
        Address source = frame.addressOfStackValue(frame.peek(i - 1));
        Address dest = frame.addressOfStackValue(frame.peek(i));
        masm.loadValue(source, R1);
        masm.storeValue(R1, dest);
    }
=======
  masm.loadValue(frame.addressOfStackValue(-2), R0);
  masm.loadValue(frame.addressOfStackValue(-1), R1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Store R0 under the nth value.
  Address dest = frame.addressOfStackValue(frame.peek(depth));
  masm.storeValue(R0, dest);
  return true;
||||||| merged common ancestors
    // Store R0 under the nth value.
    Address dest = frame.addressOfStackValue(frame.peek(depth));
    masm.storeValue(R0, dest);
    return true;
=======
  frame.push(R0);
  frame.push(R1);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <>
void BaselineCompilerCodeGen::emitJump() {
  MOZ_ASSERT(IsJumpOpcode(JSOp(*pc)));
  frame.assertSyncedStack();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GOTO()
{
    frame.syncStack(0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SWAP() {
  // Keep top stack values in R0 and R1.
  frame.popRegsAndSync(2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  jsbytecode* target = pc + GET_JUMP_OFFSET(pc);
  masm.jump(handler.labelOf(target));
||||||| merged common ancestors
    jsbytecode* target = pc + GET_JUMP_OFFSET(pc);
    masm.jump(labelOf(target));
    return true;
=======
  frame.push(R1);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <>
void BaselineInterpreterCodeGen::emitJump() {
  // We have to add the current pc's jump offset to the frame's pc.
  MOZ_CRASH("NYI: interpreter emitJump");
}
||||||| merged common ancestors
bool
BaselineCompiler::emitToBoolean()
{
    Label skipIC;
    masm.branchTestBoolean(Assembler::Equal, R0, &skipIC);
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_PICK() {
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <>
void BaselineCompilerCodeGen::emitTestBooleanTruthy(bool branchIfTrue,
                                                    ValueOperand val) {
  MOZ_ASSERT(IsJumpOpcode(JSOp(*pc)));
  frame.assertSyncedStack();
||||||| merged common ancestors
    // Call IC
    ICToBool_Fallback::Compiler stubCompiler(cx);
    if (!emitOpIC(stubCompiler.getStub(&stubSpace_))) {
        return false;
    }
=======
  // Pick takes a value on the stack and moves it to the top.
  // For instance, pick 2:
  //     before: A B C D E
  //     after : A B D E C

  // First, move value at -(amount + 1) into R0.
  int32_t depth = -(GET_INT8(handler.pc()) + 1);
  masm.loadValue(frame.addressOfStackValue(depth), R0);

  // Move the other values down.
  depth++;
  for (; depth < 0; depth++) {
    Address source = frame.addressOfStackValue(depth);
    Address dest = frame.addressOfStackValue(depth - 1);
    masm.loadValue(source, R1);
    masm.storeValue(R1, dest);
  }

  // Push R0.
  frame.pop();
  frame.push(R0);
  return true;
}

template <>
bool BaselineInterpreterCodeGen::emit_JSOP_PICK() {
  // First, move the value to move up into R0.
  Register scratch = R2.scratchReg();
  LoadUint8Operand(masm, scratch);
  masm.loadValue(frame.addressOfStackValue(scratch), R0);

  // Move the other values down.
  Label top, done;
  masm.bind(&top);
  masm.sub32(Imm32(1), scratch);
  masm.branchTest32(Assembler::Signed, scratch, scratch, &done);
  {
    masm.loadValue(frame.addressOfStackValue(scratch), R1);
    masm.storeValue(R1, frame.addressOfStackValue(scratch, sizeof(Value)));
    masm.jump(&top);
  }

  masm.bind(&done);

  // Replace value on top of the stack with R0.
  masm.storeValue(R0, frame.addressOfStackValue(-1));
  return true;
}

template <>
bool BaselineCompilerCodeGen::emit_JSOP_UNPICK() {
  frame.syncStack(0);

  // Pick takes the top of the stack value and moves it under the nth value.
  // For instance, unpick 2:
  //     before: A B C D E
  //     after : A B E C D

  // First, move value at -1 into R0.
  masm.loadValue(frame.addressOfStackValue(-1), R0);

  MOZ_ASSERT(GET_INT8(handler.pc()) > 0,
             "Interpreter code assumes JSOP_UNPICK operand > 0");

  // Move the other values up.
  int32_t depth = -(GET_INT8(handler.pc()) + 1);
  for (int32_t i = -1; i > depth; i--) {
    Address source = frame.addressOfStackValue(i - 1);
    Address dest = frame.addressOfStackValue(i);
    masm.loadValue(source, R1);
    masm.storeValue(R1, dest);
  }

  // Store R0 under the nth value.
  Address dest = frame.addressOfStackValue(depth);
  masm.storeValue(R0, dest);
  return true;
}

template <>
bool BaselineInterpreterCodeGen::emit_JSOP_UNPICK() {
  Register scratch = R2.scratchReg();
  LoadUint8Operand(masm, scratch);

  // Move the top value into R0.
  masm.loadValue(frame.addressOfStackValue(-1), R0);

  // Overwrite the nth stack value with R0 but first save the old value in R1.
  masm.loadValue(frame.addressOfStackValue(scratch), R1);
  masm.storeValue(R0, frame.addressOfStackValue(scratch));

  // Now for each slot x in [n-1, 1] do the following:
  //
  // * Store the value in slot x in R0.
  // * Store the value in the previous slot (now in R1) in slot x.
  // * Move R0 to R1.

#ifdef DEBUG
  // Assert the operand > 0 so the sub32 below doesn't "underflow" to negative
  // values.
  {
    Label ok;
    masm.branch32(Assembler::GreaterThan, scratch, Imm32(0), &ok);
    masm.assumeUnreachable("JSOP_UNPICK with operand <= 0?");
    masm.bind(&ok);
  }
#endif

  Label top, done;
  masm.bind(&top);
  masm.sub32(Imm32(1), scratch);
  masm.branchTest32(Assembler::Zero, scratch, scratch, &done);
  {
    // Overwrite stack slot x with slot x + 1, saving the old value in R1.
    masm.loadValue(frame.addressOfStackValue(scratch), R0);
    masm.storeValue(R1, frame.addressOfStackValue(scratch));
    masm.moveValue(R0, R1);
    masm.jump(&top);
  }

  // Finally, replace the value on top of the stack (slot 0) with R1. This is
  // the value that used to be in slot 1.
  masm.bind(&done);
  masm.storeValue(R1, frame.addressOfStackValue(-1));
  return true;
}

template <>
void BaselineCompilerCodeGen::emitJump() {
  jsbytecode* pc = handler.pc();
  MOZ_ASSERT(IsJumpOpcode(JSOp(*pc)));
  frame.assertSyncedStack();

  jsbytecode* target = pc + GET_JUMP_OFFSET(pc);
  masm.jump(handler.labelOf(target));
}

template <>
void BaselineInterpreterCodeGen::emitJump() {
  // We have to add the current pc's jump offset to the current pc. We can use
  // R0 and R1 as scratch because we jump to the "next op" label so these
  // registers aren't in use at this point.
  Register scratch1 = R0.scratchReg();
  Register scratch2 = R1.scratchReg();
  Register pc = LoadBytecodePC(masm, scratch1);
  LoadInt32OperandSignExtendToPtr(masm, pc, scratch2);
  if (HasInterpreterPCReg()) {
    masm.addPtr(scratch2, InterpreterPCReg);
  } else {
    masm.addPtr(pc, scratch2);
    masm.storePtr(scratch2, frame.addressOfInterpreterPC());
  }
  masm.jump(handler.interpretOpWithPCRegLabel());
}

template <>
void BaselineCompilerCodeGen::emitTestBooleanTruthy(bool branchIfTrue,
                                                    ValueOperand val) {
  jsbytecode* pc = handler.pc();
  MOZ_ASSERT(IsJumpOpcode(JSOp(*pc)));
  frame.assertSyncedStack();

  jsbytecode* target = pc + GET_JUMP_OFFSET(pc);
  masm.branchTestBooleanTruthy(branchIfTrue, val, handler.labelOf(target));
}

template <>
void BaselineInterpreterCodeGen::emitTestBooleanTruthy(bool branchIfTrue,
                                                       ValueOperand val) {
  Label done;
  masm.branchTestBooleanTruthy(!branchIfTrue, val, &done);
  emitJump();
  masm.bind(&done);
}

template <>
template <typename F1, typename F2>
MOZ_MUST_USE bool BaselineCompilerCodeGen::emitTestScriptFlag(
    JSScript::ImmutableFlags flag, const F1& ifSet, const F2& ifNotSet,
    Register scratch) {
  return handler.script()->hasFlag(flag) ? ifSet() : ifNotSet();
}

template <>
template <typename F1, typename F2>
MOZ_MUST_USE bool BaselineInterpreterCodeGen::emitTestScriptFlag(
    JSScript::ImmutableFlags flag, const F1& ifSet, const F2& ifNotSet,
    Register scratch) {
  Label flagNotSet, done;
  loadScript(scratch);
  masm.branchTest32(Assembler::Zero,
                    Address(scratch, JSScript::offsetOfImmutableFlags()),
                    Imm32(uint32_t(flag)), &flagNotSet);
  {
    if (!ifSet()) {
      return false;
    }
    masm.jump(&done);
  }
  masm.bind(&flagNotSet);
  {
    if (!ifNotSet()) {
      return false;
    }
  }

  masm.bind(&done);
  return true;
}

template <>
template <typename F>
MOZ_MUST_USE bool BaselineCompilerCodeGen::emitTestScriptFlag(
    JSScript::ImmutableFlags flag, bool value, const F& emit,
    Register scratch) {
  if (handler.script()->hasFlag(flag) == value) {
    return emit();
  }
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  jsbytecode* target = pc + GET_JUMP_OFFSET(pc);
  masm.branchTestBooleanTruthy(branchIfTrue, val, handler.labelOf(target));
||||||| merged common ancestors
    masm.bind(&skipIC);
    return true;
=======
template <>
template <typename F>
MOZ_MUST_USE bool BaselineCompilerCodeGen::emitTestScriptFlag(
    JSScript::MutableFlags flag, bool value, const F& emit, Register scratch) {
  if (handler.script()->hasFlag(flag) == value) {
    return emit();
  }
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <>
void BaselineInterpreterCodeGen::emitTestBooleanTruthy(bool branchIfTrue,
                                                       ValueOperand val) {
  Label done;
  masm.branchTestBooleanTruthy(!branchIfTrue, val, &done);
  emitJump();
  masm.bind(&done);
}
||||||| merged common ancestors
bool
BaselineCompiler::emitTest(bool branchIfTrue)
{
    bool knownBoolean = frame.peek(-1)->isKnownBoolean();
=======
template <>
template <typename F>
MOZ_MUST_USE bool BaselineInterpreterCodeGen::emitTestScriptFlag(
    JSScript::ImmutableFlags flag, bool value, const F& emit,
    Register scratch) {
  Label done;
  loadScript(scratch);
  masm.branchTest32(value ? Assembler::Zero : Assembler::NonZero,
                    Address(scratch, JSScript::offsetOfImmutableFlags()),
                    Imm32(uint32_t(flag)), &done);
  {
    if (!emit()) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GOTO() {
  frame.syncStack(0);
  emitJump();
  return true;
}
||||||| merged common ancestors
    // Keep top stack value in R0.
    frame.popRegsAndSync(1);
=======
  masm.bind(&done);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitToBoolean() {
  Label skipIC;
  masm.branchTestBoolean(Assembler::Equal, R0, &skipIC);
||||||| merged common ancestors
    if (!knownBoolean && !emitToBoolean()) {
        return false;
    }
=======
template <>
template <typename F>
MOZ_MUST_USE bool BaselineInterpreterCodeGen::emitTestScriptFlag(
    JSScript::MutableFlags flag, bool value, const F& emit, Register scratch) {
  Label done;
  loadScript(scratch);
  masm.branchTest32(value ? Assembler::Zero : Assembler::NonZero,
                    Address(scratch, JSScript::offsetOfMutableFlags()),
                    Imm32(uint32_t(flag)), &done);
  {
    if (!emit()) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call IC
  if (!emitNextIC()) {
    return false;
  }

  masm.bind(&skipIC);
  return true;
||||||| merged common ancestors
    // IC will leave a BooleanValue in R0, just need to branch on it.
    masm.branchTestBooleanTruthy(branchIfTrue, R0, labelOf(pc + GET_JUMP_OFFSET(pc)));
    return true;
=======
  masm.bind(&done);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitTest(bool branchIfTrue) {
  bool knownBoolean = frame.peek(-1)->isKnownBoolean();

  // Keep top stack value in R0.
  frame.popRegsAndSync(1);

  if (!knownBoolean && !emitToBoolean()) {
    return false;
  }

  // IC will leave a BooleanValue in R0, just need to branch on it.
  emitTestBooleanTruthy(branchIfTrue, R0);
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_IFEQ()
{
    return emitTest(false);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GOTO() {
  frame.syncStack(0);
  emitJump();
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_IFEQ() {
  return emitTest(false);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_IFNE()
{
    return emitTest(true);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitToBoolean() {
  Label skipIC;
  masm.branchTestBoolean(Assembler::Equal, R0, &skipIC);

  // Call IC
  if (!emitNextIC()) {
    return false;
  }

  masm.bind(&skipIC);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_IFNE() {
  return emitTest(true);
}
||||||| merged common ancestors
bool
BaselineCompiler::emitAndOr(bool branchIfTrue)
{
    bool knownBoolean = frame.peek(-1)->isKnownBoolean();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitTest(bool branchIfTrue) {
  bool knownBoolean = frame.stackValueHasKnownType(-1, JSVAL_TYPE_BOOLEAN);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitAndOr(bool branchIfTrue) {
  bool knownBoolean = frame.peek(-1)->isKnownBoolean();
||||||| merged common ancestors
    // AND and OR leave the original value on the stack.
    frame.syncStack(0);
=======
  // Keep top stack value in R0.
  frame.popRegsAndSync(1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // AND and OR leave the original value on the stack.
  frame.syncStack(0);
||||||| merged common ancestors
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
    if (!knownBoolean && !emitToBoolean()) {
        return false;
    }
=======
  if (!knownBoolean && !emitToBoolean()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
  if (!knownBoolean && !emitToBoolean()) {
    return false;
  }

  emitTestBooleanTruthy(branchIfTrue, R0);
  return true;
||||||| merged common ancestors
    masm.branchTestBooleanTruthy(branchIfTrue, R0, labelOf(pc + GET_JUMP_OFFSET(pc)));
    return true;
=======
  // IC will leave a BooleanValue in R0, just need to branch on it.
  emitTestBooleanTruthy(branchIfTrue, R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_AND() {
  return emitAndOr(false);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_AND()
{
    return emitAndOr(false);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_IFEQ() {
  return emitTest(false);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_OR() {
  return emitAndOr(true);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_OR()
{
    return emitAndOr(true);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_IFNE() {
  return emitTest(true);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NOT() {
  bool knownBoolean = frame.peek(-1)->isKnownBoolean();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_NOT()
{
    bool knownBoolean = frame.peek(-1)->isKnownBoolean();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitAndOr(bool branchIfTrue) {
  bool knownBoolean = frame.stackValueHasKnownType(-1, JSVAL_TYPE_BOOLEAN);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Keep top stack value in R0.
  frame.popRegsAndSync(1);
||||||| merged common ancestors
    // Keep top stack value in R0.
    frame.popRegsAndSync(1);
=======
  // AND and OR leave the original value on the stack.
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!knownBoolean && !emitToBoolean()) {
    return false;
  }
||||||| merged common ancestors
    if (!knownBoolean && !emitToBoolean()) {
        return false;
    }
=======
  masm.loadValue(frame.addressOfStackValue(-1), R0);
  if (!knownBoolean && !emitToBoolean()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.notBoolean(R0);
||||||| merged common ancestors
    masm.notBoolean(R0);
=======
  emitTestBooleanTruthy(branchIfTrue, R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.push(R0, JSVAL_TYPE_BOOLEAN);
  return true;
||||||| merged common ancestors
    frame.push(R0, JSVAL_TYPE_BOOLEAN);
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_AND() {
  return emitAndOr(false);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_POS() {
  // Keep top stack value in R0.
  frame.popRegsAndSync(1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_POS()
{
    // Keep top stack value in R0.
    frame.popRegsAndSync(1);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_OR() {
  return emitAndOr(true);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Inline path for int32 and double.
  Label done;
  masm.branchTestNumber(Assembler::Equal, R0, &done);
||||||| merged common ancestors
    // Inline path for int32 and double.
    Label done;
    masm.branchTestNumber(Assembler::Equal, R0, &done);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NOT() {
  bool knownBoolean = frame.stackValueHasKnownType(-1, JSVAL_TYPE_BOOLEAN);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call IC.
  if (!emitNextIC()) {
    return false;
  }
||||||| merged common ancestors
    // Call IC.
    ICToNumber_Fallback::Compiler stubCompiler(cx);
    if (!emitOpIC(stubCompiler.getStub(&stubSpace_))) {
        return false;
    }
=======
  // Keep top stack value in R0.
  frame.popRegsAndSync(1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&done);
  frame.push(R0);
  return true;
}
||||||| merged common ancestors
    masm.bind(&done);
    frame.push(R0);
    return true;
}
=======
  if (!knownBoolean && !emitToBoolean()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LOOPHEAD() {
  if (!emit_JSOP_JUMPTARGET()) {
    return false;
  }
  return emitInterruptCheck();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_LOOPHEAD()
{
    if (!emit_JSOP_JUMPTARGET()) {
        return false;
    }
    return emitInterruptCheck();
=======
  masm.notBoolean(R0);

  frame.push(R0, JSVAL_TYPE_BOOLEAN);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LOOPENTRY() {
  if (!emit_JSOP_JUMPTARGET()) {
    return false;
  }
  frame.syncStack(0);
  if (!emitWarmUpCounterIncrement(LoopEntryCanIonOsr(pc))) {
    return false;
  }
  if (script->trackRecordReplayProgress()) {
    masm.inc64(
        AbsoluteAddress(mozilla::recordreplay::ExecutionProgressCounter()));
  }
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_LOOPENTRY()
{
    if (!emit_JSOP_JUMPTARGET()) {
        return false;
    }
    frame.syncStack(0);
    if (!emitWarmUpCounterIncrement(LoopEntryCanIonOsr(pc))) {
        return false;
    }
    if (script->trackRecordReplayProgress()) {
        masm.inc64(AbsoluteAddress(mozilla::recordreplay::ExecutionProgressCounter()));
    }
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_POS() {
  // Keep top stack value in R0.
  frame.popRegsAndSync(1);

  // Inline path for int32 and double; otherwise call VM.
  Label done;
  masm.branchTestNumber(Assembler::Equal, R0, &done);

  prepareVMCall();
  pushArg(R0);

  using Fn = bool (*)(JSContext*, HandleValue, MutableHandleValue);
  if (!callVM<Fn, DoToNumber>()) {
    return false;
  }

  masm.bind(&done);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_VOID() {
  frame.pop();
  frame.push(UndefinedValue());
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_VOID()
{
    frame.pop();
    frame.push(UndefinedValue());
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TONUMERIC() {
  // Keep top stack value in R0.
  frame.popRegsAndSync(1);

  // Inline path for int32 and double; otherwise call VM.
  Label done;
  masm.branchTestNumber(Assembler::Equal, R0, &done);

  prepareVMCall();
  pushArg(R0);

  using Fn = bool (*)(JSContext*, HandleValue, MutableHandleValue);
  if (!callVM<Fn, DoToNumeric>()) {
    return false;
  }

  masm.bind(&done);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_UNDEFINED() {
  // If this ever changes, change what JSOP_GIMPLICITTHIS does too.
  frame.push(UndefinedValue());
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_UNDEFINED()
{
    // If this ever changes, change what JSOP_GIMPLICITTHIS does too.
    frame.push(UndefinedValue());
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LOOPHEAD() {
  if (!emit_JSOP_JUMPTARGET()) {
    return false;
  }
  return emitInterruptCheck();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_HOLE() {
  frame.push(MagicValue(JS_ELEMENTS_HOLE));
  return true;
}
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_HOLE()
{
    frame.push(MagicValue(JS_ELEMENTS_HOLE));
    return true;
}
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitIncExecutionProgressCounter(
    Register scratch) {
  if (!mozilla::recordreplay::IsRecordingOrReplaying()) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NULL() {
  frame.push(NullValue());
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_NULL()
{
    frame.push(NullValue());
    return true;
=======
  auto incCounter = [this]() {
    masm.inc64(
        AbsoluteAddress(mozilla::recordreplay::ExecutionProgressCounter()));
    return true;
  };
  return emitTestScriptFlag(JSScript::MutableFlags::TrackRecordReplayProgress,
                            true, incCounter, scratch);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*ThrowCheckIsObjectFn)(JSContext*, CheckIsObjectKind);
static const VMFunction ThrowCheckIsObjectInfo =
    FunctionInfo<ThrowCheckIsObjectFn>(ThrowCheckIsObject,
                                       "ThrowCheckIsObject");
||||||| merged common ancestors
typedef bool (*ThrowCheckIsObjectFn)(JSContext*, CheckIsObjectKind);
static const VMFunction ThrowCheckIsObjectInfo =
    FunctionInfo<ThrowCheckIsObjectFn>(ThrowCheckIsObject, "ThrowCheckIsObject");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LOOPENTRY() {
  if (!emit_JSOP_JUMPTARGET()) {
    return false;
  }
  frame.syncStack(0);
  if (!emitWarmUpCounterIncrement()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKISOBJ() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CHECKISOBJ()
{
    frame.syncStack(0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
=======
  return emitIncExecutionProgressCounter(R0.scratchReg());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label ok;
  masm.branchTestObject(Assembler::Equal, R0, &ok);
||||||| merged common ancestors
    Label ok;
    masm.branchTestObject(Assembler::Equal, R0, &ok);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_VOID() {
  frame.pop();
  frame.push(UndefinedValue());
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    prepareVMCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_UNDEFINED() {
  // If this ever changes, change what JSOP_GIMPLICITTHIS does too.
  frame.push(UndefinedValue());
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(Imm32(GET_UINT8(pc)));
  if (!callVM(ThrowCheckIsObjectInfo)) {
    return false;
  }
||||||| merged common ancestors
    pushArg(Imm32(GET_UINT8(pc)));
    if (!callVM(ThrowCheckIsObjectInfo)) {
        return false;
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_HOLE() {
  frame.push(MagicValue(JS_ELEMENTS_HOLE));
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&ok);
  return true;
||||||| merged common ancestors
    masm.bind(&ok);
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NULL() {
  frame.push(NullValue());
  return true;
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKISOBJ() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-1), R0);

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKISCALLABLE() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CHECKISCALLABLE()
{
    frame.syncStack(0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
=======
  Label ok;
  masm.branchTestObject(Assembler::Equal, R0, &ok);
>>>>>>> upstream-releases

  prepareVMCall();

<<<<<<< HEAD
  pushArg(Imm32(GET_UINT8(pc)));
  pushArg(R0);
  if (!callVM(CheckIsCallableInfo)) {
    return false;
  }
||||||| merged common ancestors
    pushArg(Imm32(GET_UINT8(pc)));
    pushArg(R0);
    if (!callVM(CheckIsCallableInfo)) {
        return false;
    }
=======
  pushUint8BytecodeOperandArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  using Fn = bool (*)(JSContext*, CheckIsObjectKind);
  if (!callVM<Fn, ThrowCheckIsObject>()) {
    return false;
  }

  masm.bind(&ok);
  return true;
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKISCALLABLE() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-1), R0);

  prepareVMCall();

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKTHIS() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CHECKTHIS()
{
    frame.syncStack(0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
=======
  pushUint8BytecodeOperandArg(R1.scratchReg());
  pushArg(R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return emitCheckThis(R0);
||||||| merged common ancestors
    return emitCheckThis(R0);
=======
  using Fn = bool (*)(JSContext*, HandleValue, CheckIsCallableKind);
  if (!callVM<Fn, CheckIsCallable>()) {
    return false;
  }

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKTHISREINIT() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CHECKTHISREINIT()
{
    frame.syncStack(0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKTHIS() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-1), R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return emitCheckThis(R0, /* reinit = */ true);
||||||| merged common ancestors
    return emitCheckThis(R0, /* reinit = */true);
=======
  return emitCheckThis(R0);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitCheckThis(ValueOperand val, bool reinit) {
  Label thisOK;
  if (reinit) {
    masm.branchTestMagic(Assembler::Equal, val, &thisOK);
  } else {
    masm.branchTestMagic(Assembler::NotEqual, val, &thisOK);
  }
||||||| merged common ancestors
bool
BaselineCompiler::emitCheckThis(ValueOperand val, bool reinit)
{
    Label thisOK;
    if (reinit) {
        masm.branchTestMagic(Assembler::Equal, val, &thisOK);
    } else {
        masm.branchTestMagic(Assembler::NotEqual, val, &thisOK);
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKTHISREINIT() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-1), R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    prepareVMCall();
=======
  return emitCheckThis(R0, /* reinit = */ true);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (reinit) {
    if (!callVM(ThrowInitializedThisInfo)) {
      return false;
    }
  } else {
    masm.loadBaselineFramePtr(BaselineFrameReg, val.scratchReg());
    pushArg(val.scratchReg());
||||||| merged common ancestors
    if (reinit) {
        if (!callVM(ThrowInitializedThisInfo)) {
            return false;
        }
    } else {
        masm.loadBaselineFramePtr(BaselineFrameReg, val.scratchReg());
        pushArg(val.scratchReg());
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitCheckThis(ValueOperand val, bool reinit) {
  Label thisOK;
  if (reinit) {
    masm.branchTestMagic(Assembler::Equal, val, &thisOK);
  } else {
    masm.branchTestMagic(Assembler::NotEqual, val, &thisOK);
  }

  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!callVM(ThrowUninitializedThisInfo)) {
      return false;
||||||| merged common ancestors
        if (!callVM(ThrowUninitializedThisInfo)) {
            return false;
        }
=======
  if (reinit) {
    using Fn = bool (*)(JSContext*);
    if (!callVM<Fn, BaselineThrowInitializedThis>()) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
  } else {
    masm.loadBaselineFramePtr(BaselineFrameReg, val.scratchReg());
    pushArg(val.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&thisOK);
  return true;
||||||| merged common ancestors
    masm.bind(&thisOK);
    return true;
=======
    using Fn = bool (*)(JSContext*, BaselineFrame*);
    if (!callVM<Fn, BaselineThrowUninitializedThis>()) {
      return false;
    }
  }

  masm.bind(&thisOK);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*ThrowBadDerivedReturnFn)(JSContext*, HandleValue);
static const VMFunction ThrowBadDerivedReturnInfo =
    FunctionInfo<ThrowBadDerivedReturnFn>(jit::ThrowBadDerivedReturn,
                                          "ThrowBadDerivedReturn");
||||||| merged common ancestors
typedef bool (*ThrowBadDerivedReturnFn)(JSContext*, HandleValue);
static const VMFunction ThrowBadDerivedReturnInfo =
    FunctionInfo<ThrowBadDerivedReturnFn>(jit::ThrowBadDerivedReturn, "ThrowBadDerivedReturn");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKRETURN() {
  MOZ_ASSERT_IF(handler.maybeScript(),
                handler.maybeScript()->isDerivedClassConstructor());
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKRETURN() {
  MOZ_ASSERT(script->isDerivedClassConstructor());
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CHECKRETURN()
{
    MOZ_ASSERT(script->isDerivedClassConstructor());
=======
  // Load |this| in R0, return value in R1.
  frame.popRegsAndSync(1);
  emitLoadReturnValue(R1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load |this| in R0, return value in R1.
  frame.popRegsAndSync(1);
  emitLoadReturnValue(R1);
||||||| merged common ancestors
    // Load |this| in R0, return value in R1.
    frame.popRegsAndSync(1);
    emitLoadReturnValue(R1);
=======
  Label done, returnOK;
  masm.branchTestObject(Assembler::Equal, R1, &done);
  masm.branchTestUndefined(Assembler::Equal, R1, &returnOK);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label done, returnOK;
  masm.branchTestObject(Assembler::Equal, R1, &done);
  masm.branchTestUndefined(Assembler::Equal, R1, &returnOK);
||||||| merged common ancestors
    Label done, returnOK;
    masm.branchTestObject(Assembler::Equal, R1, &done);
    masm.branchTestUndefined(Assembler::Equal, R1, &returnOK);
=======
  prepareVMCall();
  pushArg(R1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(R1);
  if (!callVM(ThrowBadDerivedReturnInfo)) {
    return false;
  }
  masm.assumeUnreachable("Should throw on bad derived constructor return");
||||||| merged common ancestors
    prepareVMCall();
    pushArg(R1);
    if (!callVM(ThrowBadDerivedReturnInfo)) {
        return false;
    }
    masm.assumeUnreachable("Should throw on bad derived constructor return");
=======
  using Fn = bool (*)(JSContext*, HandleValue);
  if (!callVM<Fn, ThrowBadDerivedReturn>()) {
    return false;
  }
  masm.assumeUnreachable("Should throw on bad derived constructor return");
>>>>>>> upstream-releases

  masm.bind(&returnOK);

  if (!emitCheckThis(R0)) {
    return false;
  }

  // Store |this| in the return value slot.
  masm.storeValue(R0, frame.addressOfReturnValue());
  masm.or32(Imm32(BaselineFrame::HAS_RVAL), frame.addressOfFlags());

  masm.bind(&done);
  return true;
}

<<<<<<< HEAD
typedef bool (*GetFunctionThisFn)(JSContext*, BaselineFrame*,
                                  MutableHandleValue);
static const VMFunction GetFunctionThisInfo = FunctionInfo<GetFunctionThisFn>(
    jit::BaselineGetFunctionThis, "BaselineGetFunctionThis");
||||||| merged common ancestors
typedef bool (*GetFunctionThisFn)(JSContext*, BaselineFrame*, MutableHandleValue);
static const VMFunction GetFunctionThisInfo =
    FunctionInfo<GetFunctionThisFn>(jit::BaselineGetFunctionThis, "BaselineGetFunctionThis");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FUNCTIONTHIS() {
  MOZ_ASSERT_IF(handler.maybeFunction(), !handler.maybeFunction()->isArrow());
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FUNCTIONTHIS() {
  MOZ_ASSERT(function());
  MOZ_ASSERT(!function()->isArrow());

  frame.pushThis();

  // In strict mode code or self-hosted functions, |this| is left alone.
  if (script->strict() || (function() && function()->isSelfHostedBuiltin())) {
    return true;
  }
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_FUNCTIONTHIS()
{
    MOZ_ASSERT(function());
    MOZ_ASSERT(!function()->isArrow());

    frame.pushThis();

    // In strict mode code or self-hosted functions, |this| is left alone.
    if (script->strict() || (function() && function()->isSelfHostedBuiltin())) {
        return true;
    }
=======
  frame.pushThis();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load |thisv| in R0. Skip the call if it's already an object.
  Label skipCall;
  frame.popRegsAndSync(1);
  masm.branchTestObject(Assembler::Equal, R0, &skipCall);
||||||| merged common ancestors
    // Load |thisv| in R0. Skip the call if it's already an object.
    Label skipCall;
    frame.popRegsAndSync(1);
    masm.branchTestObject(Assembler::Equal, R0, &skipCall);
=======
  auto boxThis = [this]() {
    // Load |thisv| in R0. Skip the call if it's already an object.
    Label skipCall;
    frame.popRegsAndSync(1);
    masm.branchTestObject(Assembler::Equal, R0, &skipCall);
>>>>>>> upstream-releases

  prepareVMCall();
  masm.loadBaselineFramePtr(BaselineFrameReg, R1.scratchReg());

  pushArg(R1.scratchReg());

<<<<<<< HEAD
  if (!callVM(GetFunctionThisInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(GetFunctionThisInfo)) {
        return false;
    }
=======
    using Fn = bool (*)(JSContext*, BaselineFrame*, MutableHandleValue);
    if (!callVM<Fn, BaselineGetFunctionThis>()) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&skipCall);
  frame.push(R0);
  return true;
}

typedef void (*GetNonSyntacticGlobalThisFn)(JSContext*, HandleObject,
                                            MutableHandleValue);
static const VMFunction GetNonSyntacticGlobalThisInfo =
    FunctionInfo<GetNonSyntacticGlobalThisFn>(js::GetNonSyntacticGlobalThis,
                                              "GetNonSyntacticGlobalThis");
||||||| merged common ancestors
    masm.bind(&skipCall);
    frame.push(R0);
    return true;
}

typedef void (*GetNonSyntacticGlobalThisFn)(JSContext*, HandleObject, MutableHandleValue);
static const VMFunction GetNonSyntacticGlobalThisInfo =
    FunctionInfo<GetNonSyntacticGlobalThisFn>(js::GetNonSyntacticGlobalThis,
                                              "GetNonSyntacticGlobalThis");
=======
    masm.bind(&skipCall);
    frame.push(R0);
    return true;
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GLOBALTHIS() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GLOBALTHIS()
{
    frame.syncStack(0);
=======
  // In strict mode code, |this| is left alone.
  return emitTestScriptFlag(JSScript::ImmutableFlags::Strict, false, boxThis,
                            R2.scratchReg());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!script->hasNonSyntacticScope()) {
    LexicalEnvironmentObject* globalLexical =
        &script->global().lexicalEnvironment();
    masm.moveValue(globalLexical->thisValue(), R0);
    frame.push(R0);
    return true;
  }
||||||| merged common ancestors
    if (!script->hasNonSyntacticScope()) {
        LexicalEnvironmentObject* globalLexical = &script->global().lexicalEnvironment();
        masm.moveValue(globalLexical->thisValue(), R0);
        frame.push(R0);
        return true;
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GLOBALTHIS() {
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    prepareVMCall();
=======
  auto getNonSyntacticThis = [this]() {
    prepareVMCall();
>>>>>>> upstream-releases

  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
  pushArg(R0.scratchReg());

<<<<<<< HEAD
  if (!callVM(GetNonSyntacticGlobalThisInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(GetNonSyntacticGlobalThisInfo)) {
        return false;
    }
=======
    using Fn = void (*)(JSContext*, HandleObject, MutableHandleValue);
    if (!callVM<Fn, GetNonSyntacticGlobalThis>()) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.push(R0);
  return true;
||||||| merged common ancestors
    frame.push(R0);
    return true;
=======
    frame.push(R0);
    return true;
  };
  auto getGlobalThis = [this]() {
    loadGlobalThisValue(R0);
    frame.push(R0);
    return true;
  };
  return emitTestScriptFlag(JSScript::ImmutableFlags::HasNonSyntacticScope,
                            getNonSyntacticThis, getGlobalThis,
                            R2.scratchReg());
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TRUE() {
  frame.push(BooleanValue(true));
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FALSE() {
  frame.push(BooleanValue(false));
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ZERO() {
  frame.push(Int32Value(0));
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ONE() {
  frame.push(Int32Value(1));
  return true;
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INT8() {
  frame.push(Int32Value(GET_INT8(pc)));
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INT8()
{
    frame.push(Int32Value(GET_INT8(pc)));
    return true;
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_INT8() {
  frame.push(Int32Value(GET_INT8(handler.pc())));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INT32() {
  frame.push(Int32Value(GET_INT32(pc)));
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INT32()
{
    frame.push(Int32Value(GET_INT32(pc)));
    return true;
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_INT8() {
  LoadInt8Operand(masm, R0.scratchReg());
  masm.tagValue(JSVAL_TYPE_INT32, R0.scratchReg(), R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_UINT16() {
  frame.push(Int32Value(GET_UINT16(pc)));
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_UINT16()
{
    frame.push(Int32Value(GET_UINT16(pc)));
    return true;
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_INT32() {
  frame.push(Int32Value(GET_INT32(handler.pc())));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_UINT24() {
  frame.push(Int32Value(GET_UINT24(pc)));
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_UINT24()
{
    frame.push(Int32Value(GET_UINT24(pc)));
    return true;
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_INT32() {
  LoadInt32Operand(masm, R0.scratchReg());
  masm.tagValue(JSVAL_TYPE_INT32, R0.scratchReg(), R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_RESUMEINDEX() {
  return emit_JSOP_UINT24();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_DOUBLE()
{
    frame.push(script->getConst(GET_UINT32_INDEX(pc)));
    return true;
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_UINT16() {
  frame.push(Int32Value(GET_UINT16(handler.pc())));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DOUBLE() {
  frame.push(script->getConst(GET_UINT32_INDEX(pc)));
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_STRING()
{
    frame.push(StringValue(script->getAtom(pc)));
    return true;
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_UINT16() {
  LoadUint16Operand(masm, R0.scratchReg());
  masm.tagValue(JSVAL_TYPE_INT32, R0.scratchReg(), R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
#ifdef ENABLE_BIGINT
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_BIGINT() {
  frame.push(script->getConst(GET_UINT32_INDEX(pc)));
  return true;
}
#endif

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRING() {
  frame.push(StringValue(script->getAtom(pc)));
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SYMBOL()
{
    unsigned which = GET_UINT8(pc);
    JS::Symbol* sym = cx->runtime()->wellKnownSymbols->get(which);
    frame.push(SymbolValue(sym));
    return true;
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_UINT24() {
  frame.push(Int32Value(GET_UINT24(handler.pc())));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SYMBOL() {
  unsigned which = GET_UINT8(pc);
  JS::Symbol* sym = cx->runtime()->wellKnownSymbols->get(which);
  frame.push(SymbolValue(sym));
  return true;
}
||||||| merged common ancestors
typedef JSObject* (*DeepCloneObjectLiteralFn)(JSContext*, HandleObject, NewObjectKind);
static const VMFunction DeepCloneObjectLiteralInfo =
    FunctionInfo<DeepCloneObjectLiteralFn>(DeepCloneObjectLiteral, "DeepCloneObjectLiteral");
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_UINT24() {
  LoadUint24Operand(masm, 0, R0.scratchReg());
  masm.tagValue(JSVAL_TYPE_INT32, R0.scratchReg(), R0);
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef JSObject* (*DeepCloneObjectLiteralFn)(JSContext*, HandleObject,
                                              NewObjectKind);
static const VMFunction DeepCloneObjectLiteralInfo =
    FunctionInfo<DeepCloneObjectLiteralFn>(DeepCloneObjectLiteral,
                                           "DeepCloneObjectLiteral");
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_OBJECT()
{
    if (cx->realm()->creationOptions().cloneSingletons()) {
        RootedObject obj(cx, script->getObject(GET_UINT32_INDEX(pc)));
        if (!obj) {
            return false;
        }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_RESUMEINDEX() {
  return emit_JSOP_UINT24();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_OBJECT() {
  if (cx->realm()->creationOptions().cloneSingletons()) {
    RootedObject obj(cx, script->getObject(GET_UINT32_INDEX(pc)));
    if (!obj) {
      return false;
    }
||||||| merged common ancestors
        prepareVMCall();
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_DOUBLE() {
  frame.push(GET_INLINE_VALUE(handler.pc()));
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    prepareVMCall();
||||||| merged common ancestors
        pushArg(ImmWord(TenuredObject));
        pushArg(ImmGCPtr(obj));
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_DOUBLE() {
  LoadInlineValueOperand(masm, R0);
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    pushArg(ImmWord(TenuredObject));
    pushArg(ImmGCPtr(obj));
||||||| merged common ancestors
        if (!callVM(DeepCloneObjectLiteralInfo)) {
            return false;
        }
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_BIGINT() {
  BigInt* bi = handler.script()->getBigInt(handler.pc());
  frame.push(BigIntValue(bi));
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!callVM(DeepCloneObjectLiteralInfo)) {
      return false;
    }
||||||| merged common ancestors
        // Box and push return value.
        masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
        frame.push(R0);
        return true;
    }
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_BIGINT() {
  Register scratch1 = R0.scratchReg();
  Register scratch2 = R1.scratchReg();
  loadScriptGCThing(ScriptGCThingType::BigInt, scratch1, scratch2);
  masm.tagValue(JSVAL_TYPE_BIGINT, scratch1, R0);
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Box and push return value.
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.push(R0);
    return true;
  }

  cx->realm()->behaviors().setSingletonsAsValues();
  frame.push(ObjectValue(*script->getObject(pc)));
  return true;
||||||| merged common ancestors
    cx->realm()->behaviors().setSingletonsAsValues();
    frame.push(ObjectValue(*script->getObject(pc)));
    return true;
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_STRING() {
  frame.push(StringValue(handler.script()->getAtom(handler.pc())));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CALLSITEOBJ() {
  RootedObject cso(cx, script->getObject(pc));
  RootedObject raw(cx, script->getObject(GET_UINT32_INDEX(pc) + 1));
  if (!cso || !raw) {
    return false;
  }
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CALLSITEOBJ()
{
    RootedObject cso(cx, script->getObject(pc));
    RootedObject raw(cx, script->getObject(GET_UINT32_INDEX(pc) + 1));
    if (!cso || !raw) {
        return false;
    }
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_STRING() {
  Register scratch1 = R0.scratchReg();
  Register scratch2 = R1.scratchReg();
  LoadInt32Operand(masm, scratch1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!ProcessCallSiteObjOperation(cx, cso, raw)) {
    return false;
  }
||||||| merged common ancestors
    if (!ProcessCallSiteObjOperation(cx, cso, raw)) {
        return false;
    }
=======
  loadScriptAtom(scratch1, scratch2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.push(ObjectValue(*cso));
  return true;
||||||| merged common ancestors
    frame.push(ObjectValue(*cso));
    return true;
=======
  masm.tagValue(JSVAL_TYPE_STRING, scratch2, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

template <>
bool BaselineCompilerCodeGen::emit_JSOP_SYMBOL() {
  unsigned which = GET_UINT8(handler.pc());
  JS::Symbol* sym = cx->runtime()->wellKnownSymbols->get(which);
  frame.push(SymbolValue(sym));
  return true;
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_REGEXP() {
  RootedObject reObj(cx, script->getRegExp(pc));
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_REGEXP()
{
    RootedObject reObj(cx, script->getRegExp(pc));
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_SYMBOL() {
  Register scratch1 = R0.scratchReg();
  Register scratch2 = R1.scratchReg();
  LoadUint8Operand(masm, scratch1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(ImmGCPtr(reObj));
  if (!callVM(CloneRegExpObjectInfo)) {
    return false;
  }
||||||| merged common ancestors
    prepareVMCall();
    pushArg(ImmGCPtr(reObj));
    if (!callVM(CloneRegExpObjectInfo)) {
        return false;
    }
=======
  masm.movePtr(ImmPtr(cx->runtime()->wellKnownSymbols), scratch2);
  masm.loadPtr(BaseIndex(scratch2, scratch1, ScalePointer), scratch1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Box and push return value.
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    // Box and push return value.
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.push(R0);
    return true;
=======
  masm.tagValue(JSVAL_TYPE_SYMBOL, scratch1, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef JSObject* (*LambdaFn)(JSContext*, HandleFunction, HandleObject);
static const VMFunction LambdaInfo =
    FunctionInfo<LambdaFn>(js::Lambda, "Lambda");
||||||| merged common ancestors
typedef JSObject* (*LambdaFn)(JSContext*, HandleFunction, HandleObject);
static const VMFunction LambdaInfo = FunctionInfo<LambdaFn>(js::Lambda, "Lambda");
=======
JSObject* BaselineCompilerHandler::maybeNoCloneSingletonObject() {
  Realm* realm = script()->realm();
  if (realm->creationOptions().cloneSingletons()) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LAMBDA() {
  RootedFunction fun(cx, script->getFunction(GET_UINT32_INDEX(pc)));
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_LAMBDA()
{
    RootedFunction fun(cx, script->getFunction(GET_UINT32_INDEX(pc)));
=======
  realm->behaviors().setSingletonsAsValues();
  return script()->getObject(pc());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
||||||| merged common ancestors
    prepareVMCall();
    masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_OBJECT() {
  // If we know we don't have to clone the object literal, just push it
  // directly. Note that the interpreter always does the VM call; that's fine
  // because this op is only used in run-once code.
  if (JSObject* obj = handler.maybeNoCloneSingletonObject()) {
    frame.push(ObjectValue(*obj));
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(R0.scratchReg());
  pushArg(ImmGCPtr(fun));
||||||| merged common ancestors
    pushArg(R0.scratchReg());
    pushArg(ImmGCPtr(fun));
=======
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(LambdaInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(LambdaInfo)) {
        return false;
    }
=======
  pushBytecodePCArg();
  pushScriptArg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Box and push return value.
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    // Box and push return value.
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.push(R0);
    return true;
=======
  using Fn = JSObject* (*)(JSContext*, HandleScript, jsbytecode*);
  if (!callVM<Fn, SingletonObjectLiteralOperation>()) {
    return false;
  }

  // Box and push return value.
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef JSObject* (*LambdaArrowFn)(JSContext*, HandleFunction, HandleObject,
                                   HandleValue);
static const VMFunction LambdaArrowInfo =
    FunctionInfo<LambdaArrowFn>(js::LambdaArrow, "LambdaArrow");
||||||| merged common ancestors
typedef JSObject* (*LambdaArrowFn)(JSContext*, HandleFunction, HandleObject, HandleValue);
static const VMFunction LambdaArrowInfo =
    FunctionInfo<LambdaArrowFn>(js::LambdaArrow, "LambdaArrow");
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_CALLSITEOBJ() {
  RootedScript script(cx, handler.script());
  JSObject* cso = ProcessCallSiteObjOperation(cx, script, handler.pc());
  if (!cso) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LAMBDA_ARROW() {
  // Keep pushed newTarget in R0.
  frame.popRegsAndSync(1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_LAMBDA_ARROW()
{
    // Keep pushed newTarget in R0.
    frame.popRegsAndSync(1);
=======
  frame.push(ObjectValue(*cso));
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedFunction fun(cx, script->getFunction(GET_UINT32_INDEX(pc)));
||||||| merged common ancestors
    RootedFunction fun(cx, script->getFunction(GET_UINT32_INDEX(pc)));
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_CALLSITEOBJ() {
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  masm.loadPtr(frame.addressOfEnvironmentChain(), R2.scratchReg());
||||||| merged common ancestors
    prepareVMCall();
    masm.loadPtr(frame.addressOfEnvironmentChain(), R2.scratchReg());
=======
  pushBytecodePCArg();
  pushScriptArg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(R0);
  pushArg(R2.scratchReg());
  pushArg(ImmGCPtr(fun));
||||||| merged common ancestors
    pushArg(R0);
    pushArg(R2.scratchReg());
    pushArg(ImmGCPtr(fun));
=======
  using Fn = ArrayObject* (*)(JSContext*, HandleScript, jsbytecode*);
  if (!callVM<Fn, ProcessCallSiteObjOperation>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(LambdaArrowInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(LambdaArrowInfo)) {
        return false;
    }
=======
  // Box and push return value.
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Box and push return value.
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    // Box and push return value.
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.push(R0);
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_REGEXP() {
  prepareVMCall();
  pushScriptGCThingArg(ScriptGCThingType::RegExp, R0.scratchReg(),
                       R1.scratchReg());

  using Fn = JSObject* (*)(JSContext*, Handle<RegExpObject*>);
  if (!callVM<Fn, CloneRegExpObject>()) {
    return false;
  }

  // Box and push return value.
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*SetFunNameFn)(JSContext*, HandleFunction, HandleValue,
                             FunctionPrefixKind);
static const VMFunction SetFunNameInfo =
    FunctionInfo<SetFunNameFn>(js::SetFunctionNameIfNoOwnName, "SetFunName");
||||||| merged common ancestors
typedef bool (*SetFunNameFn)(JSContext*, HandleFunction, HandleValue, FunctionPrefixKind);
static const VMFunction SetFunNameInfo =
    FunctionInfo<SetFunNameFn>(js::SetFunctionNameIfNoOwnName, "SetFunName");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LAMBDA() {
  prepareVMCall();
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETFUNNAME() {
  frame.popRegsAndSync(2);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SETFUNNAME()
{
    frame.popRegsAndSync(2);
=======
  pushArg(R0.scratchReg());
  pushScriptGCThingArg(ScriptGCThingType::Function, R0.scratchReg(),
                       R1.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.push(R0);
  frame.syncStack(0);
||||||| merged common ancestors
    frame.push(R0);
    frame.syncStack(0);
=======
  using Fn = JSObject* (*)(JSContext*, HandleFunction, HandleObject);
  if (!callVM<Fn, js::Lambda>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  FunctionPrefixKind prefixKind = FunctionPrefixKind(GET_UINT8(pc));
  masm.unboxObject(R0, R0.scratchReg());
||||||| merged common ancestors
    FunctionPrefixKind prefixKind = FunctionPrefixKind(GET_UINT8(pc));
    masm.unboxObject(R0, R0.scratchReg());
=======
  // Box and push return value.
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    prepareVMCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LAMBDA_ARROW() {
  // Keep pushed newTarget in R0.
  frame.popRegsAndSync(1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(Imm32(int32_t(prefixKind)));
  pushArg(R1);
  pushArg(R0.scratchReg());
  return callVM(SetFunNameInfo);
||||||| merged common ancestors
    pushArg(Imm32(int32_t(prefixKind)));
    pushArg(R1);
    pushArg(R0.scratchReg());
    return callVM(SetFunNameInfo);
=======
  prepareVMCall();
  masm.loadPtr(frame.addressOfEnvironmentChain(), R2.scratchReg());

  pushArg(R0);
  pushArg(R2.scratchReg());
  pushScriptGCThingArg(ScriptGCThingType::Function, R0.scratchReg(),
                       R1.scratchReg());

  using Fn =
      JSObject* (*)(JSContext*, HandleFunction, HandleObject, HandleValue);
  if (!callVM<Fn, js::LambdaArrow>()) {
    return false;
  }

  // Box and push return value.
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
void BaselineCodeGen<Handler>::storeValue(const StackValue* source,
                                          const Address& dest,
                                          const ValueOperand& scratch) {
  switch (source->kind()) {
    case StackValue::Constant:
      masm.storeValue(source->constant(), dest);
      break;
    case StackValue::Register:
      masm.storeValue(source->reg(), dest);
      break;
    case StackValue::LocalSlot:
      masm.loadValue(frame.addressOfLocal(source->localSlot()), scratch);
      masm.storeValue(scratch, dest);
      break;
    case StackValue::ArgSlot:
      masm.loadValue(frame.addressOfArg(source->argSlot()), scratch);
      masm.storeValue(scratch, dest);
      break;
    case StackValue::ThisSlot:
      masm.loadValue(frame.addressOfThis(), scratch);
      masm.storeValue(scratch, dest);
      break;
    case StackValue::EvalNewTargetSlot:
      MOZ_ASSERT(script->isForEval());
      masm.loadValue(frame.addressOfEvalNewTarget(), scratch);
      masm.storeValue(scratch, dest);
      break;
    case StackValue::Stack:
      masm.loadValue(frame.addressOfStackValue(source), scratch);
      masm.storeValue(scratch, dest);
      break;
    default:
      MOZ_CRASH("Invalid kind");
  }
||||||| merged common ancestors
void
BaselineCompiler::storeValue(const StackValue* source, const Address& dest,
                             const ValueOperand& scratch)
{
    switch (source->kind()) {
      case StackValue::Constant:
        masm.storeValue(source->constant(), dest);
        break;
      case StackValue::Register:
        masm.storeValue(source->reg(), dest);
        break;
      case StackValue::LocalSlot:
        masm.loadValue(frame.addressOfLocal(source->localSlot()), scratch);
        masm.storeValue(scratch, dest);
        break;
      case StackValue::ArgSlot:
        masm.loadValue(frame.addressOfArg(source->argSlot()), scratch);
        masm.storeValue(scratch, dest);
        break;
      case StackValue::ThisSlot:
        masm.loadValue(frame.addressOfThis(), scratch);
        masm.storeValue(scratch, dest);
        break;
      case StackValue::EvalNewTargetSlot:
        MOZ_ASSERT(script->isForEval());
        masm.loadValue(frame.addressOfEvalNewTarget(), scratch);
        masm.storeValue(scratch, dest);
        break;
      case StackValue::Stack:
        masm.loadValue(frame.addressOfStackValue(source), scratch);
        masm.storeValue(scratch, dest);
        break;
      default:
        MOZ_CRASH("Invalid kind");
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETFUNNAME() {
  frame.popRegsAndSync(2);

  frame.push(R0);
  frame.syncStack(0);

  masm.unboxObject(R0, R0.scratchReg());

  prepareVMCall();

  pushUint8BytecodeOperandArg(R2.scratchReg());
  pushArg(R1);
  pushArg(R0.scratchReg());

  using Fn =
      bool (*)(JSContext*, HandleFunction, HandleValue, FunctionPrefixKind);
  return callVM<Fn, SetFunctionName>();
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_BITOR() {
  return emitBinaryArith();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_BITXOR() {
  return emitBinaryArith();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_BITAND() {
  return emitBinaryArith();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LSH() {
  return emitBinaryArith();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_RSH() {
  return emitBinaryArith();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_URSH() {
  return emitBinaryArith();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ADD() {
  return emitBinaryArith();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SUB() {
  return emitBinaryArith();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_MUL() {
  return emitBinaryArith();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DIV() {
  return emitBinaryArith();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_MOD() {
  return emitBinaryArith();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_POW() {
  return emitBinaryArith();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emitBinaryArith() {
  // Keep top JSStack value in R0 and R2
  frame.popRegsAndSync(2);

  // Call IC
  if (!emitNextIC()) {
    return false;
  }

  // Mark R0 as pushed stack value.
  frame.push(R0);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emitUnaryArith() {
  // Keep top stack value in R0.
  frame.popRegsAndSync(1);

  // Call IC
  if (!emitNextIC()) {
    return false;
  }

  // Mark R0 as pushed stack value.
  frame.push(R0);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_BITNOT() {
  return emitUnaryArith();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NEG() {
  return emitUnaryArith();
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LT() {
  return emitCompare();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_LT()
{
    return emitCompare();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INC() {
  return emitUnaryArith();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LE() {
  return emitCompare();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_LE()
{
    return emitCompare();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEC() {
  return emitUnaryArith();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GT() {
  return emitCompare();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GT()
{
    return emitCompare();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LT() {
  return emitCompare();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GE() {
  return emitCompare();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GE()
{
    return emitCompare();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LE() {
  return emitCompare();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_EQ() {
  return emitCompare();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_EQ()
{
    return emitCompare();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GT() {
  return emitCompare();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NE() {
  return emitCompare();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_NE()
{
    return emitCompare();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GE() {
  return emitCompare();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitCompare() {
  // CODEGEN
||||||| merged common ancestors
bool
BaselineCompiler::emitCompare()
{
    // CODEGEN
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_EQ() {
  return emitCompare();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Keep top JSStack value in R0 and R1.
  frame.popRegsAndSync(2);
||||||| merged common ancestors
    // Keep top JSStack value in R0 and R1.
    frame.popRegsAndSync(2);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NE() {
  return emitCompare();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call IC.
  if (!emitNextIC()) {
    return false;
  }
||||||| merged common ancestors
    // Call IC.
    ICCompare_Fallback::Compiler stubCompiler(cx);
    if (!emitOpIC(stubCompiler.getStub(&stubSpace_))) {
        return false;
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitCompare() {
  // CODEGEN
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Mark R0 as pushed stack value.
  frame.push(R0, JSVAL_TYPE_BOOLEAN);
  return true;
}
||||||| merged common ancestors
    // Mark R0 as pushed stack value.
    frame.push(R0, JSVAL_TYPE_BOOLEAN);
    return true;
}
=======
  // Keep top JSStack value in R0 and R1.
  frame.popRegsAndSync(2);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTEQ() {
  return emitCompare();
}
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_STRICTEQ()
{
    return emitCompare();
}
=======
  // Call IC.
  if (!emitNextIC()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTNE() {
  return emitCompare();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_STRICTNE()
{
    return emitCompare();
=======
  // Mark R0 as pushed stack value.
  frame.push(R0, JSVAL_TYPE_BOOLEAN);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CONDSWITCH() {
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CONDSWITCH()
{
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTEQ() {
  return emitCompare();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CASE() {
  frame.popRegsAndSync(1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CASE()
{
    frame.popRegsAndSync(2);
    frame.push(R0);
    frame.syncStack(0);

    // Call IC.
    ICCompare_Fallback::Compiler stubCompiler(cx);
    if (!emitOpIC(stubCompiler.getStub(&stubSpace_))) {
        return false;
    }

    Register payload = masm.extractInt32(R0, R0.scratchReg());
    jsbytecode* target = pc + GET_JUMP_OFFSET(pc);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTNE() {
  return emitCompare();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CONDSWITCH() {
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CASE() {
  frame.popRegsAndSync(1);
>>>>>>> upstream-releases

  Label done;
  masm.branchTestBooleanTruthy(/* branchIfTrue */ false, R0, &done);
  {
    // Pop the switch value if the case matches.
    masm.addToStackPtr(Imm32(sizeof(Value)));
    emitJump();
  }
  masm.bind(&done);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEFAULT() {
  frame.pop();
  return emit_JSOP_GOTO();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LINENO() {
  return true;
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NEWARRAY() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_NEWARRAY()
{
    frame.syncStack(0);

    uint32_t length = GET_UINT32(pc);
    MOZ_ASSERT(length <= INT32_MAX,
               "the bytecode emitter must fail to compile code that would "
               "produce JSOP_NEWARRAY with a length exceeding int32_t range");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NEWARRAY() {
  frame.syncStack(0);

  // Pass length in R0.
  loadInt32LengthBytecodeOperand(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t length = GET_UINT32(pc);
  MOZ_ASSERT(length <= INT32_MAX,
             "the bytecode emitter must fail to compile code that would "
             "produce JSOP_NEWARRAY with a length exceeding int32_t range");
||||||| merged common ancestors
    // Pass length in R0.
    masm.move32(Imm32(AssertedCast<int32_t>(length)), R0.scratchReg());
=======
  if (!emitNextIC()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Pass length in R0.
  masm.move32(Imm32(AssertedCast<int32_t>(length)), R0.scratchReg());
||||||| merged common ancestors
    ObjectGroup* group = ObjectGroup::allocationSiteGroup(cx, script, pc, JSProto_Array);
    if (!group) {
        return false;
    }
=======
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!emitNextIC()) {
    return false;
  }
||||||| merged common ancestors
    ICNewArray_Fallback::Compiler stubCompiler(cx, group);
    if (!emitOpIC(stubCompiler.getStub(&stubSpace_))) {
        return false;
    }
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_NEWARRAY_COPYONWRITE() {
  // This is like the interpreter implementation, but we can call
  // getOrFixupCopyOnWriteObject at compile-time.
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.push(R0);
  return true;
}
||||||| merged common ancestors
    frame.push(R0);
    return true;
}
=======
  RootedScript scriptRoot(cx, handler.script());
  JSObject* obj =
      ObjectGroup::getOrFixupCopyOnWriteObject(cx, scriptRoot, handler.pc());
  if (!obj) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef ArrayObject* (*NewArrayCopyOnWriteFn)(JSContext*, HandleArrayObject,
                                              gc::InitialHeap);
const VMFunction NewArrayCopyOnWriteInfo = FunctionInfo<NewArrayCopyOnWriteFn>(
    js::NewDenseCopyOnWriteArray, "NewDenseCopyOnWriteArray");
||||||| merged common ancestors
typedef ArrayObject* (*NewArrayCopyOnWriteFn)(JSContext*, HandleArrayObject, gc::InitialHeap);
const VMFunction jit::NewArrayCopyOnWriteInfo =
    FunctionInfo<NewArrayCopyOnWriteFn>(js::NewDenseCopyOnWriteArray, "NewDenseCopyOnWriteArray");
=======
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NEWARRAY_COPYONWRITE() {
  RootedScript scriptRoot(cx, script);
  JSObject* obj = ObjectGroup::getOrFixupCopyOnWriteObject(cx, scriptRoot, pc);
  if (!obj) {
    return false;
  }
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_NEWARRAY_COPYONWRITE()
{
    RootedScript scriptRoot(cx, script);
    JSObject* obj = ObjectGroup::getOrFixupCopyOnWriteObject(cx, scriptRoot, pc);
    if (!obj) {
        return false;
    }
=======
  pushArg(ImmGCPtr(obj));
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    prepareVMCall();
=======
  using Fn = ArrayObject* (*)(JSContext*, HandleArrayObject);
  if (!callVM<Fn, js::NewDenseCopyOnWriteArray>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(Imm32(gc::DefaultHeap));
  pushArg(ImmGCPtr(obj));
||||||| merged common ancestors
    pushArg(Imm32(gc::DefaultHeap));
    pushArg(ImmGCPtr(obj));
=======
  // Box and push return value.
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(NewArrayCopyOnWriteInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(NewArrayCopyOnWriteInfo)) {
        return false;
    }
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_NEWARRAY_COPYONWRITE() {
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Box and push return value.
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    // Box and push return value.
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.push(R0);
    return true;
=======
  pushBytecodePCArg();
  pushScriptArg();

  using Fn = ArrayObject* (*)(JSContext*, HandleScript, jsbytecode*);
  if (!callVM<Fn, NewArrayCopyOnWriteOperation>()) {
    return false;
  }

  // Box and push return value.
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITELEM_ARRAY() {
  // Keep the object and rhs on the stack.
  frame.syncStack(0);

<<<<<<< HEAD
  // Load object in R0, index in R1.
  masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R0);
  uint32_t index = GET_UINT32(pc);
  MOZ_ASSERT(index <= INT32_MAX,
             "the bytecode emitter must fail to compile code that would "
             "produce JSOP_INITELEM_ARRAY with a length exceeding "
             "int32_t range");
  masm.moveValue(Int32Value(AssertedCast<int32_t>(index)), R1);
||||||| merged common ancestors
    // Load object in R0, index in R1.
    masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R0);
    uint32_t index = GET_UINT32(pc);
    MOZ_ASSERT(index <= INT32_MAX,
               "the bytecode emitter must fail to compile code that would "
               "produce JSOP_INITELEM_ARRAY with a length exceeding "
               "int32_t range");
    masm.moveValue(Int32Value(AssertedCast<int32_t>(index)), R1);

    // Call IC.
    ICSetElem_Fallback::Compiler stubCompiler(cx);
    if (!emitOpIC(stubCompiler.getStub(&stubSpace_))) {
        return false;
    }
=======
  // Load object in R0, index in R1.
  masm.loadValue(frame.addressOfStackValue(-2), R0);
  loadInt32IndexBytecodeOperand(R1);

  // Call IC.
  if (!emitNextIC()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call IC.
  if (!emitNextIC()) {
    return false;
  }

  // Pop the rhs, so that the object is on the top of the stack.
  frame.pop();
  return true;
||||||| merged common ancestors
    // Pop the rhs, so that the object is on the top of the stack.
    frame.pop();
    return true;
=======
  // Pop the rhs, so that the object is on the top of the stack.
  frame.pop();
  return true;
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NEWOBJECT() {
  frame.syncStack(0);

  if (!emitNextIC()) {
    return false;
  }

  frame.push(R0);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NEWINIT() {
  frame.syncStack(0);

  if (!emitNextIC()) {
    return false;
  }

  frame.push(R0);
  return true;
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITELEM() {
  // Store RHS in the scratch slot.
  storeValue(frame.peek(-1), frame.addressOfScratchValue(), R2);
  frame.pop();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITELEM()
{
    // Store RHS in the scratch slot.
    storeValue(frame.peek(-1), frame.addressOfScratchValue(), R2);
    frame.pop();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITELEM() {
  // Store RHS in the scratch slot.
  frame.storeStackValue(-1, frame.addressOfScratchValue(), R2);
  frame.pop();
>>>>>>> upstream-releases

  // Keep object and index in R0 and R1.
  frame.popRegsAndSync(2);

  // Push the object to store the result of the IC.
  frame.push(R0);
  frame.syncStack(0);

  // Keep RHS on the stack.
  frame.pushScratchValue();

  // Call IC.
  if (!emitNextIC()) {
    return false;
  }

  // Pop the rhs, so that the object is on the top of the stack.
  frame.pop();
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITHIDDENELEM() {
  return emit_JSOP_INITELEM();
}

<<<<<<< HEAD
typedef bool (*MutateProtoFn)(JSContext* cx, HandlePlainObject obj,
                              HandleValue newProto);
static const VMFunction MutateProtoInfo =
    FunctionInfo<MutateProtoFn>(MutatePrototype, "MutatePrototype");

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_MUTATEPROTO() {
  // Keep values on the stack for the decompiler.
  frame.syncStack(0);
||||||| merged common ancestors
typedef bool (*MutateProtoFn)(JSContext* cx, HandlePlainObject obj, HandleValue newProto);
static const VMFunction MutateProtoInfo =
    FunctionInfo<MutateProtoFn>(MutatePrototype, "MutatePrototype");

bool
BaselineCompiler::emit_JSOP_MUTATEPROTO()
{
    // Keep values on the stack for the decompiler.
    frame.syncStack(0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_MUTATEPROTO() {
  // Keep values on the stack for the decompiler.
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.unboxObject(frame.addressOfStackValue(frame.peek(-2)), R0.scratchReg());
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R1);
||||||| merged common ancestors
    masm.unboxObject(frame.addressOfStackValue(frame.peek(-2)), R0.scratchReg());
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R1);
=======
  masm.unboxObject(frame.addressOfStackValue(-2), R0.scratchReg());
  masm.loadValue(frame.addressOfStackValue(-1), R1);
>>>>>>> upstream-releases

  prepareVMCall();

  pushArg(R1);
  pushArg(R0.scratchReg());

<<<<<<< HEAD
  if (!callVM(MutateProtoInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(MutateProtoInfo)) {
        return false;
    }
=======
  using Fn = bool (*)(JSContext*, HandlePlainObject, HandleValue);
  if (!callVM<Fn, MutatePrototype>()) {
    return false;
  }
>>>>>>> upstream-releases

  frame.pop();
  return true;
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITPROP() {
  // Load lhs in R0, rhs in R1.
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITPROP()
{
    // Load lhs in R0, rhs in R1.
    frame.syncStack(0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R1);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITPROP() {
  // Load lhs in R0, rhs in R1.
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-2), R0);
  masm.loadValue(frame.addressOfStackValue(-1), R1);
>>>>>>> upstream-releases

  // Call IC.
  if (!emitNextIC()) {
    return false;
  }

  // Leave the object on the stack.
  frame.pop();
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITLOCKEDPROP() {
  return emit_JSOP_INITPROP();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITHIDDENPROP() {
  return emit_JSOP_INITPROP();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETELEM() {
  // Keep top two stack values in R0 and R1.
  frame.popRegsAndSync(2);

  // Call IC.
  if (!emitNextIC()) {
    return false;
  }

  // Mark R0 as pushed stack value.
  frame.push(R0);
  return true;
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETELEM_SUPER() {
  // Store obj in the scratch slot.
  storeValue(frame.peek(-1), frame.addressOfScratchValue(), R2);
  frame.pop();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GETELEM_SUPER()
{
    // Store obj in the scratch slot.
    storeValue(frame.peek(-1), frame.addressOfScratchValue(), R2);
    frame.pop();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETELEM_SUPER() {
  // Store obj in the scratch slot.
  frame.storeStackValue(-1, frame.addressOfScratchValue(), R2);
  frame.pop();
>>>>>>> upstream-releases

  // Keep receiver and index in R0 and R1.
  frame.popRegsAndSync(2);

  // Keep obj on the stack.
  frame.pushScratchValue();

  if (!emitNextIC()) {
    return false;
  }

  frame.pop();  // This value is also popped in InitFromBailout.
  frame.push(R0);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CALLELEM() {
  return emit_JSOP_GETELEM();
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETELEM() {
  // Store RHS in the scratch slot.
  storeValue(frame.peek(-1), frame.addressOfScratchValue(), R2);
  frame.pop();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SETELEM()
{
    // Store RHS in the scratch slot.
    storeValue(frame.peek(-1), frame.addressOfScratchValue(), R2);
    frame.pop();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETELEM() {
  // Store RHS in the scratch slot.
  frame.storeStackValue(-1, frame.addressOfScratchValue(), R2);
  frame.pop();
>>>>>>> upstream-releases

  // Keep object and index in R0 and R1.
  frame.popRegsAndSync(2);

  // Keep RHS on the stack.
  frame.pushScratchValue();

  // Call IC.
  if (!emitNextIC()) {
    return false;
  }

  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTSETELEM() {
  return emit_JSOP_SETELEM();
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitSetElemSuper(bool strict) {
  // Incoming stack is |receiver, propval, obj, rval|. We need to shuffle
  // stack to leave rval when operation is complete.

  // Pop rval into R0, then load receiver into R1 and replace with rval.
  frame.popRegsAndSync(1);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-3)), R1);
  masm.storeValue(R0, frame.addressOfStackValue(frame.peek(-3)));
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SETELEM_SUPER()
{
    bool strict = IsCheckStrictOp(JSOp(*pc));

    // Incoming stack is |receiver, propval, obj, rval|. We need to shuffle
    // stack to leave rval when operation is complete.
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitSetElemSuper(bool strict) {
  // Incoming stack is |receiver, propval, obj, rval|. We need to shuffle
  // stack to leave rval when operation is complete.
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    // Pop rval into R0, then load receiver into R1 and replace with rval.
    frame.popRegsAndSync(1);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-3)), R1);
    masm.storeValue(R0, frame.addressOfStackValue(frame.peek(-3)));
=======
  // Pop rval into R0, then load receiver into R1 and replace with rval.
  frame.popRegsAndSync(1);
  masm.loadValue(frame.addressOfStackValue(-3), R1);
  masm.storeValue(R0, frame.addressOfStackValue(-3));
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(Imm32(strict));
  pushArg(R1);  // receiver
  pushArg(R0);  // rval
  masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R0);
  pushArg(R0);  // propval
  masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), R0.scratchReg());
  pushArg(R0.scratchReg());  // obj
||||||| merged common ancestors
    prepareVMCall();
=======
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(SetObjectElementInfo)) {
    return false;
  }
||||||| merged common ancestors
    pushArg(Imm32(strict));
    pushArg(R1); // receiver
    pushArg(R0); // rval
    masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R0);
    pushArg(R0); // propval
    masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), R0.scratchReg());
    pushArg(R0.scratchReg()); // obj
=======
  pushArg(Imm32(strict));
  pushArg(R1);  // receiver
  pushArg(R0);  // rval
  masm.loadValue(frame.addressOfStackValue(-2), R0);
  pushArg(R0);  // propval
  masm.unboxObject(frame.addressOfStackValue(-1), R0.scratchReg());
  pushArg(R0.scratchReg());  // obj
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.popn(2);
  return true;
}
||||||| merged common ancestors
    if (!callVM(SetObjectElementInfo)) {
        return false;
    }
=======
  using Fn = bool (*)(JSContext*, HandleObject, HandleValue, HandleValue,
                      HandleValue, bool);
  if (!callVM<Fn, js::SetObjectElementWithReceiver>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETELEM_SUPER() {
  return emitSetElemSuper(/* strict = */ false);
||||||| merged common ancestors
    frame.popn(2);
    return true;
=======
  frame.popn(2);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTSETELEM_SUPER() {
  return emitSetElemSuper(/* strict = */ true);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_STRICTSETELEM_SUPER()
{
    return emit_JSOP_SETELEM_SUPER();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETELEM_SUPER() {
  return emitSetElemSuper(/* strict = */ false);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*DeleteElementFn)(JSContext*, HandleValue, HandleValue, bool*);
static const VMFunction DeleteElementStrictInfo = FunctionInfo<DeleteElementFn>(
    DeleteElementJit<true>, "DeleteElementStrict");
static const VMFunction DeleteElementNonStrictInfo =
    FunctionInfo<DeleteElementFn>(DeleteElementJit<false>,
                                  "DeleteElementNonStrict");
||||||| merged common ancestors
typedef bool (*DeleteElementFn)(JSContext*, HandleValue, HandleValue, bool*);
static const VMFunction DeleteElementStrictInfo
    = FunctionInfo<DeleteElementFn>(DeleteElementJit<true>, "DeleteElementStrict");
static const VMFunction DeleteElementNonStrictInfo
    = FunctionInfo<DeleteElementFn>(DeleteElementJit<false>, "DeleteElementNonStrict");

bool
BaselineCompiler::emit_JSOP_DELELEM()
{
    // Keep values on the stack for the decompiler.
    frame.syncStack(0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R1);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTSETELEM_SUPER() {
  return emitSetElemSuper(/* strict = */ true);
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emitDelElem(bool strict) {
  // Keep values on the stack for the decompiler.
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-2), R0);
  masm.loadValue(frame.addressOfStackValue(-1), R1);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitDelElem(bool strict) {
  // Keep values on the stack for the decompiler.
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R1);
||||||| merged common ancestors
    prepareVMCall();
=======
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    pushArg(R1);
    pushArg(R0);
=======
  pushArg(R1);
  pushArg(R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(R1);
  pushArg(R0);
  if (!callVM(strict ? DeleteElementStrictInfo : DeleteElementNonStrictInfo)) {
    return false;
  }
||||||| merged common ancestors
    bool strict = JSOp(*pc) == JSOP_STRICTDELELEM;
    if (!callVM(strict ? DeleteElementStrictInfo : DeleteElementNonStrictInfo)) {
        return false;
    }
=======
  using Fn = bool (*)(JSContext*, HandleValue, HandleValue, bool*);
  if (strict) {
    if (!callVM<Fn, DeleteElementJit<true>>()) {
      return false;
    }
  } else {
    if (!callVM<Fn, DeleteElementJit<false>>()) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.boxNonDouble(JSVAL_TYPE_BOOLEAN, ReturnReg, R1);
  frame.popn(2);
  frame.push(R1);
  return true;
||||||| merged common ancestors
    masm.boxNonDouble(JSVAL_TYPE_BOOLEAN, ReturnReg, R1);
    frame.popn(2);
    frame.push(R1);
    return true;
=======
  masm.boxNonDouble(JSVAL_TYPE_BOOLEAN, ReturnReg, R1);
  frame.popn(2);
  frame.push(R1);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DELELEM() {
  return emitDelElem(/* strict = */ false);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DELELEM() {
  return emitDelElem(/* strict = */ false);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_STRICTDELELEM()
{
    return emit_JSOP_DELELEM();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTDELELEM() {
  return emitDelElem(/* strict = */ true);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTDELELEM() {
  return emitDelElem(/* strict = */ true);
}
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_IN()
{
    frame.popRegsAndSync(2);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_IN() {
  frame.popRegsAndSync(2);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_IN() {
  frame.popRegsAndSync(2);
||||||| merged common ancestors
    ICIn_Fallback::Compiler stubCompiler(cx);
    if (!emitOpIC(stubCompiler.getStub(&stubSpace_))) {
        return false;
    }
=======
  if (!emitNextIC()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!emitNextIC()) {
    return false;
  }

  frame.push(R0);
  return true;
||||||| merged common ancestors
    frame.push(R0);
    return true;
=======
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_HASOWN() {
  frame.popRegsAndSync(2);

  if (!emitNextIC()) {
    return false;
  }

<<<<<<< HEAD
  frame.push(R0);
  return true;
||||||| merged common ancestors
    frame.push(R0);
    return true;
=======
  frame.push(R0);
  return true;
}

template <>
bool BaselineCompilerCodeGen::tryOptimizeGetGlobalName() {
  PropertyName* name = handler.script()->getName(handler.pc());

  // These names are non-configurable on the global and cannot be shadowed.
  if (name == cx->names().undefined) {
    frame.push(UndefinedValue());
    return true;
  }
  if (name == cx->names().NaN) {
    frame.push(JS::NaNValue());
    return true;
  }
  if (name == cx->names().Infinity) {
    frame.push(JS::InfinityValue());
    return true;
  }

  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETGNAME() {
  if (script->hasNonSyntacticScope()) {
    return emit_JSOP_GETNAME();
  }
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GETGNAME()
{
    if (script->hasNonSyntacticScope()) {
        return emit_JSOP_GETNAME();
    }
=======
template <>
bool BaselineInterpreterCodeGen::tryOptimizeGetGlobalName() {
  // Interpreter doesn't optimize simple GETGNAMEs.
  return false;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedPropertyName name(cx, script->getName(pc));
||||||| merged common ancestors
    RootedPropertyName name(cx, script->getName(pc));
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETGNAME() {
  auto getName = [this]() { return emit_JSOP_GETNAME(); };
>>>>>>> upstream-releases

<<<<<<< HEAD
  // These names are non-configurable on the global and cannot be shadowed.
  if (name == cx->names().undefined) {
    frame.push(UndefinedValue());
    return true;
  }
  if (name == cx->names().NaN) {
    frame.push(cx->runtime()->NaNValue);
    return true;
  }
  if (name == cx->names().Infinity) {
    frame.push(cx->runtime()->positiveInfinityValue);
    return true;
  }
||||||| merged common ancestors
    // These names are non-configurable on the global and cannot be shadowed.
    if (name == cx->names().undefined) {
        frame.push(UndefinedValue());
        return true;
    }
    if (name == cx->names().NaN) {
        frame.push(cx->runtime()->NaNValue);
        return true;
    }
    if (name == cx->names().Infinity) {
        frame.push(cx->runtime()->positiveInfinityValue);
        return true;
    }
=======
  auto getGlobalName = [this]() {
    if (tryOptimizeGetGlobalName()) {
      return true;
    }
>>>>>>> upstream-releases

  frame.syncStack(0);

<<<<<<< HEAD
  masm.movePtr(ImmGCPtr(&script->global().lexicalEnvironment()),
               R0.scratchReg());
||||||| merged common ancestors
    masm.movePtr(ImmGCPtr(&script->global().lexicalEnvironment()), R0.scratchReg());
=======
    loadGlobalLexicalEnvironment(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call IC.
  if (!emitNextIC()) {
    return false;
  }
||||||| merged common ancestors
    // Call IC.
    ICGetName_Fallback::Compiler stubCompiler(cx);
    if (!emitOpIC(stubCompiler.getStub(&stubSpace_))) {
        return false;
    }
=======
    // Call IC.
    if (!emitNextIC()) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Mark R0 as pushed stack value.
  frame.push(R0);
  return true;
||||||| merged common ancestors
    // Mark R0 as pushed stack value.
    frame.push(R0);
    return true;
=======
    // Mark R0 as pushed stack value.
    frame.push(R0);
    return true;
  };
  return emitTestScriptFlag(JSScript::ImmutableFlags::HasNonSyntacticScope,
                            getName, getGlobalName, R2.scratchReg());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_BINDGNAME() {
  if (!script->hasNonSyntacticScope()) {
    // We can bind name to the global lexical scope if the binding already
    // exists, is initialized, and is writable (i.e., an initialized
    // 'let') at compile time.
    RootedPropertyName name(cx, script->getName(pc));
    Rooted<LexicalEnvironmentObject*> env(
        cx, &script->global().lexicalEnvironment());
    if (Shape* shape = env->lookup(cx, name)) {
      if (shape->writable() &&
          !env->getSlot(shape->slot()).isMagic(JS_UNINITIALIZED_LEXICAL)) {
        frame.push(ObjectValue(*env));
        return true;
      }
    } else if (Shape* shape = script->global().lookup(cx, name)) {
      // If the property does not currently exist on the global lexical
      // scope, we can bind name to the global object if the property
      // exists on the global and is non-configurable, as then it cannot
      // be shadowed.
      if (!shape->configurable()) {
        frame.push(ObjectValue(script->global()));
        return true;
      }
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_BINDGNAME()
{
    if (!script->hasNonSyntacticScope()) {
        // We can bind name to the global lexical scope if the binding already
        // exists, is initialized, and is writable (i.e., an initialized
        // 'let') at compile time.
        RootedPropertyName name(cx, script->getName(pc));
        Rooted<LexicalEnvironmentObject*> env(cx, &script->global().lexicalEnvironment());
        if (Shape* shape = env->lookup(cx, name)) {
            if (shape->writable() &&
                !env->getSlot(shape->slot()).isMagic(JS_UNINITIALIZED_LEXICAL))
            {
                frame.push(ObjectValue(*env));
                return true;
            }
        } else if (Shape* shape = script->global().lookup(cx, name)) {
            // If the property does not currently exist on the global lexical
            // scope, we can bind name to the global object if the property
            // exists on the global and is non-configurable, as then it cannot
            // be shadowed.
            if (!shape->configurable()) {
                frame.push(ObjectValue(script->global()));
                return true;
            }
        }

        // Otherwise we have to use the environment chain.
=======
template <>
bool BaselineCompilerCodeGen::tryOptimizeBindGlobalName() {
  JSScript* script = handler.script();
  if (script->hasNonSyntacticScope()) {
    return false;
  }

  // We can bind name to the global lexical scope if the binding already
  // exists, is initialized, and is writable (i.e., an initialized
  // 'let') at compile time.
  RootedPropertyName name(cx, script->getName(handler.pc()));
  Rooted<LexicalEnvironmentObject*> env(cx,
                                        &script->global().lexicalEnvironment());
  if (Shape* shape = env->lookup(cx, name)) {
    if (shape->writable() &&
        !env->getSlot(shape->slot()).isMagic(JS_UNINITIALIZED_LEXICAL)) {
      frame.push(ObjectValue(*env));
      return true;
    }
    return false;
  }

  if (Shape* shape = script->global().lookup(cx, name)) {
    // If the property does not currently exist on the global lexical
    // scope, we can bind name to the global object if the property
    // exists on the global and is non-configurable, as then it cannot
    // be shadowed.
    if (!shape->configurable()) {
      frame.push(ObjectValue(script->global()));
      return true;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
    // Otherwise we have to use the environment chain.
  }

  return emitBindName(JSOP_BINDGNAME);
||||||| merged common ancestors
    return emit_JSOP_BINDNAME();
=======
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef JSObject* (*BindVarFn)(JSContext*, HandleObject);
static const VMFunction BindVarInfo =
    FunctionInfo<BindVarFn>(jit::BindVar, "BindVar");
||||||| merged common ancestors
typedef JSObject* (*BindVarFn)(JSContext*, HandleObject);
static const VMFunction BindVarInfo = FunctionInfo<BindVarFn>(jit::BindVar, "BindVar");
=======
template <>
bool BaselineInterpreterCodeGen::tryOptimizeBindGlobalName() {
  // Interpreter doesn't optimize simple BINDGNAMEs.
  return false;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_BINDVAR() {
  frame.syncStack(0);
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_BINDVAR()
{
    frame.syncStack(0);
    masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_BINDGNAME() {
  if (tryOptimizeBindGlobalName()) {
    return true;
  }
  return emitBindName(JSOP_BINDGNAME);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(R0.scratchReg());
||||||| merged common ancestors
    prepareVMCall();
    pushArg(R0.scratchReg());
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_BINDVAR() {
  frame.syncStack(0);
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(BindVarInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(BindVarInfo)) {
        return false;
    }
=======
  prepareVMCall();
  pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.push(R0);
    return true;
=======
  using Fn = JSObject* (*)(JSContext*, JSObject*);
  if (!callVM<Fn, BindVarOperation>()) {
    return false;
  }

  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETPROP() {
  // Keep lhs in R0, rhs in R1.
  frame.popRegsAndSync(2);

  // Keep RHS on the stack.
  frame.push(R1);
  frame.syncStack(0);

  // Call IC.
  if (!emitNextIC()) {
    return false;
  }

  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTSETPROP() {
  return emit_JSOP_SETPROP();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETNAME() {
  return emit_JSOP_SETPROP();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTSETNAME() {
  return emit_JSOP_SETPROP();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETGNAME() {
  return emit_JSOP_SETPROP();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTSETGNAME() {
  return emit_JSOP_SETPROP();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emitSetPropSuper(bool strict) {
  // Incoming stack is |receiver, obj, rval|. We need to shuffle stack to
  // leave rval when operation is complete.

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitSetPropSuper(bool strict) {
  // Incoming stack is |receiver, obj, rval|. We need to shuffle stack to
  // leave rval when operation is complete.
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SETPROP_SUPER()
{
    bool strict = IsCheckStrictOp(JSOp(*pc));
=======
  // Pop rval into R0, then load receiver into R1 and replace with rval.
  frame.popRegsAndSync(1);
  masm.loadValue(frame.addressOfStackValue(-2), R1);
  masm.storeValue(R0, frame.addressOfStackValue(-2));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Pop rval into R0, then load receiver into R1 and replace with rval.
  frame.popRegsAndSync(1);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R1);
  masm.storeValue(R0, frame.addressOfStackValue(frame.peek(-2)));
||||||| merged common ancestors
    // Incoming stack is |receiver, obj, rval|. We need to shuffle stack to
    // leave rval when operation is complete.
=======
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();

  pushArg(Imm32(strict));
  pushArg(R0);  // rval
  pushArg(ImmGCPtr(script->getName(pc)));
  pushArg(R1);  // receiver
  masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), R0.scratchReg());
  pushArg(R0.scratchReg());  // obj
||||||| merged common ancestors
    // Pop rval into R0, then load receiver into R1 and replace with rval.
    frame.popRegsAndSync(1);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R1);
    masm.storeValue(R0, frame.addressOfStackValue(frame.peek(-2)));

    prepareVMCall();
=======
  pushArg(Imm32(strict));
  pushArg(R0);  // rval
  pushScriptNameArg(R0.scratchReg(), R2.scratchReg());
  pushArg(R1);  // receiver
  masm.unboxObject(frame.addressOfStackValue(-1), R0.scratchReg());
  pushArg(R0.scratchReg());  // obj
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(SetPropertySuperInfo)) {
    return false;
  }
||||||| merged common ancestors
    pushArg(Imm32(strict));
    pushArg(R0); // rval
    pushArg(ImmGCPtr(script->getName(pc)));
    pushArg(R1); // receiver
    masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), R0.scratchReg());
    pushArg(R0.scratchReg()); // obj
=======
  using Fn = bool (*)(JSContext*, HandleObject, HandleValue, HandlePropertyName,
                      HandleValue, bool);
  if (!callVM<Fn, js::SetPropertySuper>()) {
    return false;
  }
>>>>>>> upstream-releases

  frame.pop();
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETPROP_SUPER() {
  return emitSetPropSuper(/* strict = */ false);
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTSETPROP_SUPER() {
  return emitSetPropSuper(/* strict = */ true);
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETPROP() {
  // Keep object in R0.
  frame.popRegsAndSync(1);

  // Call IC.
  if (!emitNextIC()) {
    return false;
  }

  // Mark R0 as pushed stack value.
  frame.push(R0);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CALLPROP() {
  return emit_JSOP_GETPROP();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LENGTH() {
  return emit_JSOP_GETPROP();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETBOUNDNAME() {
  return emit_JSOP_GETPROP();
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETPROP_SUPER() {
  // Receiver -> R1, Object -> R0
  frame.popRegsAndSync(1);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R1);
  frame.pop();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GETPROP_SUPER()
{
    // Receiver -> R1, Object -> R0
    frame.popRegsAndSync(1);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R1);
    frame.pop();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETPROP_SUPER() {
  // Receiver -> R1, Object -> R0
  frame.popRegsAndSync(1);
  masm.loadValue(frame.addressOfStackValue(-1), R1);
  frame.pop();
>>>>>>> upstream-releases

  if (!emitNextIC()) {
    return false;
  }

  frame.push(R0);
  return true;
}

<<<<<<< HEAD
typedef bool (*DeletePropertyFn)(JSContext*, HandleValue, HandlePropertyName,
                                 bool*);
static const VMFunction DeletePropertyStrictInfo =
    FunctionInfo<DeletePropertyFn>(DeletePropertyJit<true>,
                                   "DeletePropertyStrict");
static const VMFunction DeletePropertyNonStrictInfo =
    FunctionInfo<DeletePropertyFn>(DeletePropertyJit<false>,
                                   "DeletePropertyNonStrict");

template <typename Handler>
bool BaselineCodeGen<Handler>::emitDelProp(bool strict) {
  // Keep value on the stack for the decompiler.
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);

  prepareVMCall();
||||||| merged common ancestors

typedef bool (*DeletePropertyFn)(JSContext*, HandleValue, HandlePropertyName, bool*);
static const VMFunction DeletePropertyStrictInfo =
    FunctionInfo<DeletePropertyFn>(DeletePropertyJit<true>, "DeletePropertyStrict");
static const VMFunction DeletePropertyNonStrictInfo =
    FunctionInfo<DeletePropertyFn>(DeletePropertyJit<false>, "DeletePropertyNonStrict");

bool
BaselineCompiler::emit_JSOP_DELPROP()
{
    // Keep value on the stack for the decompiler.
    frame.syncStack(0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);

    prepareVMCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitDelProp(bool strict) {
  // Keep value on the stack for the decompiler.
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-1), R0);

  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(ImmGCPtr(script->getName(pc)));
  pushArg(R0);
||||||| merged common ancestors
    pushArg(ImmGCPtr(script->getName(pc)));
    pushArg(R0);
=======
  pushScriptNameArg(R1.scratchReg(), R2.scratchReg());
  pushArg(R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(strict ? DeletePropertyStrictInfo
                     : DeletePropertyNonStrictInfo)) {
    return false;
  }
||||||| merged common ancestors
    bool strict = JSOp(*pc) == JSOP_STRICTDELPROP;
    if (!callVM(strict ? DeletePropertyStrictInfo : DeletePropertyNonStrictInfo)) {
        return false;
    }
=======
  using Fn = bool (*)(JSContext*, HandleValue, HandlePropertyName, bool*);
  if (strict) {
    if (!callVM<Fn, DeletePropertyJit<true>>()) {
      return false;
    }
  } else {
    if (!callVM<Fn, DeletePropertyJit<false>>()) {
      return false;
    }
  }
>>>>>>> upstream-releases

  masm.boxNonDouble(JSVAL_TYPE_BOOLEAN, ReturnReg, R1);
  frame.pop();
  frame.push(R1);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DELPROP() {
  return emitDelProp(/* strict = */ false);
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTDELPROP() {
  return emitDelProp(/* strict = */ true);
}

template <typename Handler>
void BaselineCodeGen<Handler>::getEnvironmentCoordinateObject(Register reg) {
  EnvironmentCoordinate ec(pc);

  masm.loadPtr(frame.addressOfEnvironmentChain(), reg);
  for (unsigned i = ec.hops(); i; i--) {
    masm.unboxObject(
        Address(reg, EnvironmentObject::offsetOfEnclosingEnvironment()), reg);
  }
||||||| merged common ancestors
void
BaselineCompiler::getEnvironmentCoordinateObject(Register reg)
{
    EnvironmentCoordinate ec(pc);

    masm.loadPtr(frame.addressOfEnvironmentChain(), reg);
    for (unsigned i = ec.hops(); i; i--) {
        masm.unboxObject(Address(reg, EnvironmentObject::offsetOfEnclosingEnvironment()), reg);
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTDELPROP() {
  return emitDelProp(/* strict = */ true);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
Address BaselineCodeGen<Handler>::getEnvironmentCoordinateAddressFromObject(
    Register objReg, Register reg) {
  EnvironmentCoordinate ec(pc);
  Shape* shape = EnvironmentCoordinateToEnvironmentShape(script, pc);
||||||| merged common ancestors
Address
BaselineCompiler::getEnvironmentCoordinateAddressFromObject(Register objReg, Register reg)
{
    EnvironmentCoordinate ec(pc);
    Shape* shape = EnvironmentCoordinateToEnvironmentShape(script, pc);
=======
template <>
void BaselineCompilerCodeGen::getEnvironmentCoordinateObject(Register reg) {
  EnvironmentCoordinate ec(handler.pc());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (shape->numFixedSlots() <= ec.slot()) {
    masm.loadPtr(Address(objReg, NativeObject::offsetOfSlots()), reg);
    return Address(reg, (ec.slot() - shape->numFixedSlots()) * sizeof(Value));
  }
||||||| merged common ancestors
    if (shape->numFixedSlots() <= ec.slot()) {
        masm.loadPtr(Address(objReg, NativeObject::offsetOfSlots()), reg);
        return Address(reg, (ec.slot() - shape->numFixedSlots()) * sizeof(Value));
    }
=======
  masm.loadPtr(frame.addressOfEnvironmentChain(), reg);
  for (unsigned i = ec.hops(); i; i--) {
    masm.unboxObject(
        Address(reg, EnvironmentObject::offsetOfEnclosingEnvironment()), reg);
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return Address(objReg, NativeObject::getFixedSlotOffset(ec.slot()));
||||||| merged common ancestors
    return Address(objReg, NativeObject::getFixedSlotOffset(ec.slot()));
=======
template <>
void BaselineInterpreterCodeGen::getEnvironmentCoordinateObject(Register reg) {
  MOZ_CRASH("Shouldn't call this for interpreter");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
Address BaselineCodeGen<Handler>::getEnvironmentCoordinateAddress(
    Register reg) {
  getEnvironmentCoordinateObject(reg);
  return getEnvironmentCoordinateAddressFromObject(reg, reg);
||||||| merged common ancestors
Address
BaselineCompiler::getEnvironmentCoordinateAddress(Register reg)
{
    getEnvironmentCoordinateObject(reg);
    return getEnvironmentCoordinateAddressFromObject(reg, reg);
=======
template <>
Address BaselineCompilerCodeGen::getEnvironmentCoordinateAddressFromObject(
    Register objReg, Register reg) {
  EnvironmentCoordinate ec(handler.pc());

  if (EnvironmentObject::nonExtensibleIsFixedSlot(ec)) {
    return Address(objReg, NativeObject::getFixedSlotOffset(ec.slot()));
  }

  uint32_t slot = EnvironmentObject::nonExtensibleDynamicSlotIndex(ec);
  masm.loadPtr(Address(objReg, NativeObject::offsetOfSlots()), reg);
  return Address(reg, slot * sizeof(Value));
}

template <>
Address BaselineInterpreterCodeGen::getEnvironmentCoordinateAddressFromObject(
    Register objReg, Register reg) {
  MOZ_CRASH("Shouldn't call this for interpreter");
}

template <typename Handler>
Address BaselineCodeGen<Handler>::getEnvironmentCoordinateAddress(
    Register reg) {
  getEnvironmentCoordinateObject(reg);
  return getEnvironmentCoordinateAddressFromObject(reg, reg);
}

// For a JOF_ENVCOORD op load the number of hops from the bytecode and skip this
// number of environment objects.
static void LoadAliasedVarEnv(MacroAssembler& masm, Register env,
                              Register scratch) {
  static_assert(ENVCOORD_HOPS_LEN == 1,
                "Code assumes number of hops is stored in uint8 operand");
  LoadUint8Operand(masm, scratch);

  Label top, done;
  masm.bind(&top);
  masm.branchTest32(Assembler::Zero, scratch, scratch, &done);
  {
    Address nextEnv(env, EnvironmentObject::offsetOfEnclosingEnvironment());
    masm.unboxObject(nextEnv, env);
    masm.sub32(Imm32(1), scratch);
    masm.jump(&top);
  }
  masm.bind(&done);
}

template <>
void BaselineCompilerCodeGen::emitGetAliasedVar(ValueOperand dest) {
  frame.syncStack(0);

  Address address = getEnvironmentCoordinateAddress(R0.scratchReg());
  masm.loadValue(address, dest);
}

template <>
void BaselineInterpreterCodeGen::emitGetAliasedVar(ValueOperand dest) {
  Register env = R0.scratchReg();
  Register scratch = R1.scratchReg();

  // Load the right environment object.
  masm.loadPtr(frame.addressOfEnvironmentChain(), env);
  LoadAliasedVarEnv(masm, env, scratch);

  // Load the slot index.
  static_assert(ENVCOORD_SLOT_LEN == 3,
                "Code assumes slot is stored in uint24 operand");
  LoadUint24Operand(masm, ENVCOORD_HOPS_LEN, scratch);

  // Load the Value from a fixed or dynamic slot.
  // See EnvironmentObject::nonExtensibleIsFixedSlot.
  Label isDynamic, done;
  masm.branch32(Assembler::AboveOrEqual, scratch,
                Imm32(NativeObject::MAX_FIXED_SLOTS), &isDynamic);
  {
    uint32_t offset = NativeObject::getFixedSlotOffset(0);
    masm.loadValue(BaseValueIndex(env, scratch, offset), dest);
    masm.jump(&done);
  }
  masm.bind(&isDynamic);
  {
    masm.loadPtr(Address(env, NativeObject::offsetOfSlots()), env);

    // Use an offset to subtract the number of fixed slots.
    int32_t offset = -int32_t(NativeObject::MAX_FIXED_SLOTS * sizeof(Value));
    masm.loadValue(BaseValueIndex(env, scratch, offset), dest);
  }
  masm.bind(&done);
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETALIASEDVAR() {
  emitGetAliasedVar(R0);

  if (handler.maybeIonCompileable()) {
    // No need to monitor types if we know Ion can't compile this script.
    if (!emitNextIC()) {
      return false;
    }
  }

  frame.push(R0);
  return true;
}

template <>
bool BaselineCompilerCodeGen::emit_JSOP_SETALIASEDVAR() {
  // Keep rvalue in R0.
  frame.popRegsAndSync(1);
  Register objReg = R2.scratchReg();

  getEnvironmentCoordinateObject(objReg);
  Address address =
      getEnvironmentCoordinateAddressFromObject(objReg, R1.scratchReg());
  masm.guardedCallPreBarrier(address, MIRType::Value);
  masm.storeValue(R0, address);
  frame.push(R0);

  // Only R0 is live at this point.
  // Scope coordinate object is already in R2.scratchReg().
  Register temp = R1.scratchReg();

  Label skipBarrier;
  masm.branchPtrInNurseryChunk(Assembler::Equal, objReg, temp, &skipBarrier);
  masm.branchValueIsNurseryCell(Assembler::NotEqual, R0, temp, &skipBarrier);

  masm.call(&postBarrierSlot_);  // Won't clobber R0

  masm.bind(&skipBarrier);
  return true;
}

template <>
bool BaselineInterpreterCodeGen::emit_JSOP_SETALIASEDVAR() {
  AllocatableGeneralRegisterSet regs(GeneralRegisterSet::All());
  regs.take(BaselineFrameReg);
  regs.take(R2);
  if (HasInterpreterPCReg()) {
    regs.take(InterpreterPCReg);
  }

  Register env = regs.takeAny();
  Register scratch1 = regs.takeAny();
  Register scratch2 = regs.takeAny();
  Register scratch3 = regs.takeAny();

  // Load the right environment object.
  masm.loadPtr(frame.addressOfEnvironmentChain(), env);
  LoadAliasedVarEnv(masm, env, scratch1);

  // Load the slot index.
  static_assert(ENVCOORD_SLOT_LEN == 3,
                "Code assumes slot is stored in uint24 operand");
  LoadUint24Operand(masm, ENVCOORD_HOPS_LEN, scratch1);

  // Store the RHS Value in R2.
  masm.loadValue(frame.addressOfStackValue(-1), R2);

  // Load a pointer to the fixed or dynamic slot into scratch2. We want to call
  // guardedCallPreBarrierAnyZone once to avoid code bloat.

  // See EnvironmentObject::nonExtensibleIsFixedSlot.
  Label isDynamic, done;
  masm.branch32(Assembler::AboveOrEqual, scratch1,
                Imm32(NativeObject::MAX_FIXED_SLOTS), &isDynamic);
  {
    uint32_t offset = NativeObject::getFixedSlotOffset(0);
    BaseValueIndex slotAddr(env, scratch1, offset);
    masm.computeEffectiveAddress(slotAddr, scratch2);
    masm.jump(&done);
  }
  masm.bind(&isDynamic);
  {
    masm.loadPtr(Address(env, NativeObject::offsetOfSlots()), scratch2);

    // Use an offset to subtract the number of fixed slots.
    int32_t offset = -int32_t(NativeObject::MAX_FIXED_SLOTS * sizeof(Value));
    BaseValueIndex slotAddr(scratch2, scratch1, offset);
    masm.computeEffectiveAddress(slotAddr, scratch2);
  }
  masm.bind(&done);

  // Pre-barrier and store.
  Address slotAddr(scratch2, 0);
  masm.guardedCallPreBarrierAnyZone(slotAddr, MIRType::Value, scratch3);
  masm.storeValue(R2, slotAddr);

  // Post barrier.
  Label skipBarrier;
  masm.branchPtrInNurseryChunk(Assembler::Equal, env, scratch1, &skipBarrier);
  masm.branchValueIsNurseryCell(Assembler::NotEqual, R2, scratch1,
                                &skipBarrier);
  {
    // Post barrier code expects the object in R2.
    masm.movePtr(env, R2.scratchReg());
    masm.call(&postBarrierSlot_);
  }
  masm.bind(&skipBarrier);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETALIASEDVAR() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GETALIASEDVAR()
{
    frame.syncStack(0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETNAME() {
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Address address = getEnvironmentCoordinateAddress(R0.scratchReg());
  masm.loadValue(address, R0);
||||||| merged common ancestors
    Address address = getEnvironmentCoordinateAddress(R0.scratchReg());
    masm.loadValue(address, R0);
=======
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (ionCompileable_) {
    // No need to monitor types if we know Ion can't compile this script.
    if (!emitNextIC()) {
      return false;
    }
  }
||||||| merged common ancestors
    if (ionCompileable_) {
        // No need to monitor types if we know Ion can't compile this script.
        ICTypeMonitor_Fallback::Compiler compiler(cx, nullptr);
        if (!emitOpIC(compiler.getStub(&stubSpace_))) {
            return false;
        }
    }
=======
  // Call IC.
  if (!emitNextIC()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.push(R0);
  return true;
||||||| merged common ancestors
    frame.push(R0);
    return true;
=======
  // Mark R0 as pushed stack value.
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETALIASEDVAR() {
  JSScript* outerScript = EnvironmentCoordinateFunctionScript(script, pc);
  if (outerScript && outerScript->treatAsRunOnce()) {
    // Type updates for this operation might need to be tracked, so treat
    // this as a SETPROP.
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SETALIASEDVAR()
{
    JSScript* outerScript = EnvironmentCoordinateFunctionScript(script, pc);
    if (outerScript && outerScript->treatAsRunOnce()) {
        // Type updates for this operation might need to be tracked, so treat
        // this as a SETPROP.

        // Load rhs into R1.
        frame.syncStack(0);
        masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R1);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitBindName(JSOp op) {
  // If we have a BINDGNAME without a non-syntactic scope, we pass the global
  // lexical environment to the IC instead of the frame's environment.

  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Load rhs into R1.
    frame.syncStack(0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R1);

    // Load and box lhs into R0.
    getEnvironmentCoordinateObject(R2.scratchReg());
    masm.tagValue(JSVAL_TYPE_OBJECT, R2.scratchReg(), R0);
||||||| merged common ancestors
        // Load and box lhs into R0.
        getEnvironmentCoordinateObject(R2.scratchReg());
        masm.tagValue(JSVAL_TYPE_OBJECT, R2.scratchReg(), R0);

        // Call SETPROP IC.
        ICSetProp_Fallback::Compiler compiler(cx);
        if (!emitOpIC(compiler.getStub(&stubSpace_))) {
            return false;
        }
=======
  auto loadGlobalLexical = [this]() {
    loadGlobalLexicalEnvironment(R0.scratchReg());
    return true;
  };
  auto loadFrameEnv = [this]() {
    masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
    return true;
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Call SETPROP IC.
    if (!emitNextIC()) {
      return false;
||||||| merged common ancestors
        return true;
=======
  if (op == JSOP_BINDNAME) {
    if (!loadFrameEnv()) {
      return false;
>>>>>>> upstream-releases
    }
  } else {
    MOZ_ASSERT(op == JSOP_BINDGNAME);
    if (!emitTestScriptFlag(JSScript::ImmutableFlags::HasNonSyntacticScope,
                            loadFrameEnv, loadGlobalLexical, R2.scratchReg())) {
      return false;
    }
  }

<<<<<<< HEAD
    return true;
  }
||||||| merged common ancestors
    // Keep rvalue in R0.
    frame.popRegsAndSync(1);
    Register objReg = R2.scratchReg();
=======
  // Call IC.
  if (!emitNextIC()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Keep rvalue in R0.
  frame.popRegsAndSync(1);
  Register objReg = R2.scratchReg();
||||||| merged common ancestors
    getEnvironmentCoordinateObject(objReg);
    Address address = getEnvironmentCoordinateAddressFromObject(objReg, R1.scratchReg());
    masm.guardedCallPreBarrier(address, MIRType::Value);
    masm.storeValue(R0, address);
    frame.push(R0);
=======
  // Mark R0 as pushed stack value.
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  getEnvironmentCoordinateObject(objReg);
  Address address =
      getEnvironmentCoordinateAddressFromObject(objReg, R1.scratchReg());
  masm.guardedCallPreBarrier(address, MIRType::Value);
  masm.storeValue(R0, address);
  frame.push(R0);
||||||| merged common ancestors
    // Only R0 is live at this point.
    // Scope coordinate object is already in R2.scratchReg().
    Register temp = R1.scratchReg();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_BINDNAME() {
  return emitBindName(JSOP_BINDNAME);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Only R0 is live at this point.
  // Scope coordinate object is already in R2.scratchReg().
  Register temp = R1.scratchReg();
||||||| merged common ancestors
    Label skipBarrier;
    masm.branchPtrInNurseryChunk(Assembler::Equal, objReg, temp, &skipBarrier);
    masm.branchValueIsNurseryCell(Assembler::NotEqual, R0, temp, &skipBarrier);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DELNAME() {
  frame.syncStack(0);
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label skipBarrier;
  masm.branchPtrInNurseryChunk(Assembler::Equal, objReg, temp, &skipBarrier);
  masm.branchValueIsNurseryCell(Assembler::NotEqual, R0, temp, &skipBarrier);
||||||| merged common ancestors
    masm.call(&postBarrierSlot_); // Won't clobber R0
=======
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.call(&postBarrierSlot_);  // Won't clobber R0

  masm.bind(&skipBarrier);
  return true;
||||||| merged common ancestors
    masm.bind(&skipBarrier);
    return true;
=======
  pushArg(R0.scratchReg());
  pushScriptNameArg(R1.scratchReg(), R2.scratchReg());

  using Fn = bool (*)(JSContext*, HandlePropertyName, HandleObject,
                      MutableHandleValue);
  if (!callVM<Fn, js::DeleteNameOperation>()) {
    return false;
  }

  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETNAME() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GETNAME()
{
    frame.syncStack(0);
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_GETIMPORT() {
  JSScript* script = handler.script();
  ModuleEnvironmentObject* env = GetModuleEnvironmentForScript(script);
  MOZ_ASSERT(env);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
||||||| merged common ancestors
    masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
=======
  jsid id = NameToId(script->getName(handler.pc()));
  ModuleEnvironmentObject* targetEnv;
  Shape* shape;
  MOZ_ALWAYS_TRUE(env->lookupImport(id, &targetEnv, &shape));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call IC.
  if (!emitNextIC()) {
    return false;
  }
||||||| merged common ancestors
    // Call IC.
    ICGetName_Fallback::Compiler stubCompiler(cx);
    if (!emitOpIC(stubCompiler.getStub(&stubSpace_))) {
        return false;
    }
=======
  EnsureTrackPropertyTypes(cx, targetEnv, shape->propid());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Mark R0 as pushed stack value.
  frame.push(R0);
  return true;
}
||||||| merged common ancestors
    // Mark R0 as pushed stack value.
    frame.push(R0);
    return true;
}
=======
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitBindName(JSOp op) {
  MOZ_ASSERT(op == JSOP_BINDNAME || op == JSOP_BINDGNAME);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_BINDNAME()
{
    frame.syncStack(0);
=======
  uint32_t slot = shape->slot();
  Register scratch = R0.scratchReg();
  masm.movePtr(ImmGCPtr(targetEnv), scratch);
  if (slot < targetEnv->numFixedSlots()) {
    masm.loadValue(Address(scratch, NativeObject::getFixedSlotOffset(slot)),
                   R0);
  } else {
    masm.loadPtr(Address(scratch, NativeObject::offsetOfSlots()), scratch);
    masm.loadValue(
        Address(scratch, (slot - targetEnv->numFixedSlots()) * sizeof(Value)),
        R0);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.syncStack(0);
||||||| merged common ancestors
    if (*pc == JSOP_BINDGNAME && !script->hasNonSyntacticScope()) {
        masm.movePtr(ImmGCPtr(&script->global().lexicalEnvironment()), R0.scratchReg());
    } else {
        masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
    }
=======
  // Imports are initialized by this point except in rare circumstances, so
  // don't emit a check unless we have to.
  if (targetEnv->getSlot(shape->slot()).isMagic(JS_UNINITIALIZED_LEXICAL)) {
    if (!emitUninitializedLexicalCheck(R0)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (op == JSOP_BINDGNAME && !script->hasNonSyntacticScope()) {
    masm.movePtr(ImmGCPtr(&script->global().lexicalEnvironment()),
                 R0.scratchReg());
  } else {
    masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
  }
||||||| merged common ancestors
    // Call IC.
    ICBindName_Fallback::Compiler stubCompiler(cx);
    if (!emitOpIC(stubCompiler.getStub(&stubSpace_))) {
        return false;
    }
=======
  if (handler.maybeIonCompileable()) {
    // No need to monitor types if we know Ion can't compile this script.
    if (!emitNextIC()) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call IC.
  if (!emitNextIC()) {
    return false;
  }

  // Mark R0 as pushed stack value.
  frame.push(R0);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_BINDNAME() {
  return emitBindName(JSOP_BINDNAME);
||||||| merged common ancestors
    // Mark R0 as pushed stack value.
    frame.push(R0);
    return true;
=======
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

template <>
bool BaselineInterpreterCodeGen::emit_JSOP_GETIMPORT() {
  frame.syncStack(0);

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DELNAME() {
  frame.syncStack(0);
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_DELNAME()
{
    frame.syncStack(0);
    masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
=======
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
>>>>>>> upstream-releases

  prepareVMCall();

<<<<<<< HEAD
  pushArg(R0.scratchReg());
  pushArg(ImmGCPtr(script->getName(pc)));
||||||| merged common ancestors
    pushArg(R0.scratchReg());
    pushArg(ImmGCPtr(script->getName(pc)));
=======
  pushBytecodePCArg();
  pushScriptArg();
  pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(DeleteNameInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(DeleteNameInfo)) {
        return false;
    }
=======
  using Fn = bool (*)(JSContext*, HandleObject, HandleScript, jsbytecode*,
                      MutableHandleValue);
  if (!callVM<Fn, GetImportOperation>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.push(R0);
  return true;
}
||||||| merged common ancestors
    frame.push(R0);
    return true;
}
=======
  // Enter the type monitor IC.
  if (!emitNextIC()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETIMPORT() {
  ModuleEnvironmentObject* env = GetModuleEnvironmentForScript(script);
  MOZ_ASSERT(env);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GETIMPORT()
{
    ModuleEnvironmentObject* env = GetModuleEnvironmentForScript(script);
    MOZ_ASSERT(env);
=======
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  ModuleEnvironmentObject* targetEnv;
  Shape* shape;
  MOZ_ALWAYS_TRUE(
      env->lookupImport(NameToId(script->getName(pc)), &targetEnv, &shape));
||||||| merged common ancestors
    ModuleEnvironmentObject* targetEnv;
    Shape* shape;
    MOZ_ALWAYS_TRUE(env->lookupImport(NameToId(script->getName(pc)), &targetEnv, &shape));
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETINTRINSIC() {
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  EnsureTrackPropertyTypes(cx, targetEnv, shape->propid());
||||||| merged common ancestors
    EnsureTrackPropertyTypes(cx, targetEnv, shape->propid());
=======
  if (!emitNextIC()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.syncStack(0);
||||||| merged common ancestors
    frame.syncStack(0);
=======
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t slot = shape->slot();
  Register scratch = R0.scratchReg();
  masm.movePtr(ImmGCPtr(targetEnv), scratch);
  if (slot < targetEnv->numFixedSlots()) {
    masm.loadValue(Address(scratch, NativeObject::getFixedSlotOffset(slot)),
                   R0);
  } else {
    masm.loadPtr(Address(scratch, NativeObject::offsetOfSlots()), scratch);
    masm.loadValue(
        Address(scratch, (slot - targetEnv->numFixedSlots()) * sizeof(Value)),
        R0);
  }
||||||| merged common ancestors
    uint32_t slot = shape->slot();
    Register scratch = R0.scratchReg();
    masm.movePtr(ImmGCPtr(targetEnv), scratch);
    if (slot < targetEnv->numFixedSlots()) {
        masm.loadValue(Address(scratch, NativeObject::getFixedSlotOffset(slot)), R0);
    } else {
        masm.loadPtr(Address(scratch, NativeObject::offsetOfSlots()), scratch);
        masm.loadValue(Address(scratch, (slot - targetEnv->numFixedSlots()) * sizeof(Value)), R0);
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETINTRINSIC() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-1), R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Imports are initialized by this point except in rare circumstances, so
  // don't emit a check unless we have to.
  if (targetEnv->getSlot(shape->slot()).isMagic(JS_UNINITIALIZED_LEXICAL)) {
    if (!emitUninitializedLexicalCheck(R0)) {
      return false;
    }
  }
||||||| merged common ancestors
    // Imports are initialized by this point except in rare circumstances, so
    // don't emit a check unless we have to.
    if (targetEnv->getSlot(shape->slot()).isMagic(JS_UNINITIALIZED_LEXICAL)) {
        if (!emitUninitializedLexicalCheck(R0)) {
            return false;
        }
    }
=======
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (ionCompileable_) {
    // No need to monitor types if we know Ion can't compile this script.
    if (!emitNextIC()) {
      return false;
    }
  }
||||||| merged common ancestors
    if (ionCompileable_) {
        // No need to monitor types if we know Ion can't compile this script.
        ICTypeMonitor_Fallback::Compiler compiler(cx, nullptr);
        if (!emitOpIC(compiler.getStub(&stubSpace_))) {
            return false;
        }
    }
=======
  pushArg(R0);
  pushBytecodePCArg();
  pushScriptArg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.push(R0);
  return true;
||||||| merged common ancestors
    frame.push(R0);
    return true;
=======
  using Fn = bool (*)(JSContext*, JSScript*, jsbytecode*, HandleValue);
  return callVM<Fn, SetIntrinsicOperation>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETINTRINSIC() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GETINTRINSIC()
{
    frame.syncStack(0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEFVAR() {
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!emitNextIC()) {
    return false;
  }
||||||| merged common ancestors
    ICGetIntrinsic_Fallback::Compiler stubCompiler(cx);
    if (!emitOpIC(stubCompiler.getStub(&stubSpace_))) {
        return false;
    }
=======
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.push(R0);
  return true;
}
||||||| merged common ancestors
    frame.push(R0);
    return true;
}
=======
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*DefVarFn)(JSContext*, HandlePropertyName, unsigned,
                         HandleObject);
static const VMFunction DefVarInfo = FunctionInfo<DefVarFn>(DefVar, "DefVar");
||||||| merged common ancestors
typedef bool (*DefVarFn)(JSContext*, HandlePropertyName, unsigned, HandleObject);
static const VMFunction DefVarInfo = FunctionInfo<DefVarFn>(DefVar, "DefVar");
=======
  pushBytecodePCArg();
  pushScriptArg();
  pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEFVAR() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_DEFVAR()
{
    frame.syncStack(0);
=======
  using Fn = bool (*)(JSContext*, HandleObject, HandleScript, jsbytecode*);
  return callVM<Fn, DefVarOperation>();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  unsigned attrs = JSPROP_ENUMERATE;
  if (!script->isForEval()) {
    attrs |= JSPROP_PERMANENT;
  }
  MOZ_ASSERT(attrs <= UINT32_MAX);
||||||| merged common ancestors
    unsigned attrs = JSPROP_ENUMERATE;
    if (!script->isForEval()) {
        attrs |= JSPROP_PERMANENT;
    }
    MOZ_ASSERT(attrs <= UINT32_MAX);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitDefLexical(JSOp op) {
  MOZ_ASSERT(op == JSOP_DEFCONST || op == JSOP_DEFLET);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
||||||| merged common ancestors
    masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
=======
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    prepareVMCall();
=======
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(R0.scratchReg());
  pushArg(Imm32(attrs));
  pushArg(ImmGCPtr(script->getName(pc)));
||||||| merged common ancestors
    pushArg(R0.scratchReg());
    pushArg(Imm32(attrs));
    pushArg(ImmGCPtr(script->getName(pc)));
=======
  prepareVMCall();

  pushBytecodePCArg();
  pushScriptArg();
  pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  return callVM(DefVarInfo);
||||||| merged common ancestors
    return callVM(DefVarInfo);
=======
  using Fn = bool (*)(JSContext*, HandleObject, HandleScript, jsbytecode*);
  return callVM<Fn, DefLexicalOperation>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*DefLexicalFn)(JSContext*, HandlePropertyName, unsigned,
                             HandleObject);
static const VMFunction DefLexicalInfo =
    FunctionInfo<DefLexicalFn>(DefLexical, "DefLexical");
||||||| merged common ancestors
typedef bool (*DefLexicalFn)(JSContext*, HandlePropertyName, unsigned, HandleObject);
static const VMFunction DefLexicalInfo = FunctionInfo<DefLexicalFn>(DefLexical, "DefLexical");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEFCONST() {
  return emitDefLexical(JSOP_DEFCONST);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitDefLexical(JSOp op) {
  MOZ_ASSERT(op == JSOP_DEFCONST || op == JSOP_DEFLET);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_DEFCONST()
{
    return emit_JSOP_DEFLET();
}
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEFLET() {
  return emitDefLexical(JSOP_DEFLET);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_DEFLET()
{
    frame.syncStack(0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEFFUN() {
  frame.popRegsAndSync(1);
  masm.unboxObject(R0, R0.scratchReg());
  masm.loadPtr(frame.addressOfEnvironmentChain(), R1.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  unsigned attrs = JSPROP_ENUMERATE | JSPROP_PERMANENT;
  if (op == JSOP_DEFCONST) {
    attrs |= JSPROP_READONLY;
  }
  MOZ_ASSERT(attrs <= UINT32_MAX);
||||||| merged common ancestors
    unsigned attrs = JSPROP_ENUMERATE | JSPROP_PERMANENT;
    if (*pc == JSOP_DEFCONST) {
        attrs |= JSPROP_READONLY;
    }
    MOZ_ASSERT(attrs <= UINT32_MAX);
=======
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
||||||| merged common ancestors
    masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
=======
  pushArg(R0.scratchReg());
  pushArg(R1.scratchReg());
  pushScriptArg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    prepareVMCall();
=======
  using Fn = bool (*)(JSContext*, HandleScript, HandleObject, HandleFunction);
  return callVM<Fn, DefFunOperation>();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(R0.scratchReg());
  pushArg(Imm32(attrs));
  pushArg(ImmGCPtr(script->getName(pc)));

  return callVM(DefLexicalInfo);
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEFCONST() {
  return emitDefLexical(JSOP_DEFCONST);
}
||||||| merged common ancestors
    pushArg(R0.scratchReg());
    pushArg(Imm32(attrs));
    pushArg(ImmGCPtr(script->getName(pc)));
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitInitPropGetterSetter() {
  // Keep values on the stack for the decompiler.
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEFLET() {
  return emitDefLexical(JSOP_DEFLET);
}
||||||| merged common ancestors
    return callVM(DefLexicalInfo);
}
=======
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*DefFunOperationFn)(JSContext*, HandleScript, HandleObject,
                                  HandleFunction);
static const VMFunction DefFunOperationInfo =
    FunctionInfo<DefFunOperationFn>(DefFunOperation, "DefFunOperation");
||||||| merged common ancestors
typedef bool (*DefFunOperationFn)(JSContext*, HandleScript, HandleObject, HandleFunction);
static const VMFunction DefFunOperationInfo =
    FunctionInfo<DefFunOperationFn>(DefFunOperation, "DefFunOperation");
=======
  masm.unboxObject(frame.addressOfStackValue(-1), R0.scratchReg());
  masm.unboxObject(frame.addressOfStackValue(-2), R1.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEFFUN() {
  frame.popRegsAndSync(1);
  masm.unboxObject(R0, R0.scratchReg());
  masm.loadPtr(frame.addressOfEnvironmentChain(), R1.scratchReg());
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_DEFFUN()
{
    frame.popRegsAndSync(1);
    masm.unboxObject(R0, R0.scratchReg());
    masm.loadPtr(frame.addressOfEnvironmentChain(), R1.scratchReg());
=======
  pushArg(R0.scratchReg());
  pushScriptNameArg(R0.scratchReg(), R2.scratchReg());
  pushArg(R1.scratchReg());
  pushBytecodePCArg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    prepareVMCall();
=======
  using Fn = bool (*)(JSContext*, jsbytecode*, HandleObject, HandlePropertyName,
                      HandleObject);
  if (!callVM<Fn, InitPropGetterSetterOperation>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(R0.scratchReg());
  pushArg(R1.scratchReg());
  pushArg(ImmGCPtr(script));
||||||| merged common ancestors
    pushArg(R0.scratchReg());
    pushArg(R1.scratchReg());
    pushArg(ImmGCPtr(script));
=======
  frame.pop();
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return callVM(DefFunOperationInfo);
||||||| merged common ancestors
    return callVM(DefFunOperationInfo);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITPROP_GETTER() {
  return emitInitPropGetterSetter();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*InitPropGetterSetterFn)(JSContext*, jsbytecode*, HandleObject,
                                       HandlePropertyName, HandleObject);
static const VMFunction InitPropGetterSetterInfo =
    FunctionInfo<InitPropGetterSetterFn>(InitGetterSetterOperation,
                                         "InitPropGetterSetterOperation");
||||||| merged common ancestors
typedef bool (*InitPropGetterSetterFn)(JSContext*, jsbytecode*, HandleObject, HandlePropertyName,
                                       HandleObject);
static const VMFunction InitPropGetterSetterInfo =
    FunctionInfo<InitPropGetterSetterFn>(InitGetterSetterOperation,
                                         "InitPropGetterSetterOperation");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITHIDDENPROP_GETTER() {
  return emitInitPropGetterSetter();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitInitPropGetterSetter() {
  // Keep values on the stack for the decompiler.
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emitInitPropGetterSetter()
{
    MOZ_ASSERT(JSOp(*pc) == JSOP_INITPROP_GETTER ||
               JSOp(*pc) == JSOP_INITHIDDENPROP_GETTER ||
               JSOp(*pc) == JSOP_INITPROP_SETTER ||
               JSOp(*pc) == JSOP_INITHIDDENPROP_SETTER);

    // Keep values on the stack for the decompiler.
    frame.syncStack(0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITPROP_SETTER() {
  return emitInitPropGetterSetter();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITHIDDENPROP_SETTER() {
  return emitInitPropGetterSetter();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    prepareVMCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitInitElemGetterSetter() {
  // Load index and value in R0 and R1, but keep values on the stack for the
  // decompiler.
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-2), R0);
  masm.unboxObject(frame.addressOfStackValue(-1), R1.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), R0.scratchReg());
  masm.unboxObject(frame.addressOfStackValue(frame.peek(-2)), R1.scratchReg());
||||||| merged common ancestors
    masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), R0.scratchReg());
    masm.unboxObject(frame.addressOfStackValue(frame.peek(-2)), R1.scratchReg());
=======
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(R0.scratchReg());
  pushArg(ImmGCPtr(script->getName(pc)));
  pushArg(R1.scratchReg());
  pushArg(ImmPtr(pc));
||||||| merged common ancestors
    pushArg(R0.scratchReg());
    pushArg(ImmGCPtr(script->getName(pc)));
    pushArg(R1.scratchReg());
    pushArg(ImmPtr(pc));
=======
  pushArg(R1.scratchReg());
  pushArg(R0);
  masm.unboxObject(frame.addressOfStackValue(-3), R0.scratchReg());
  pushArg(R0.scratchReg());
  pushBytecodePCArg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(InitPropGetterSetterInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(InitPropGetterSetterInfo)) {
        return false;
    }
=======
  using Fn = bool (*)(JSContext*, jsbytecode*, HandleObject, HandleValue,
                      HandleObject);
  if (!callVM<Fn, InitElemGetterSetterOperation>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.pop();
  return true;
||||||| merged common ancestors
    frame.pop();
    return true;
=======
  frame.popn(2);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITPROP_GETTER() {
  return emitInitPropGetterSetter();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITPROP_GETTER()
{
    return emitInitPropGetterSetter();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITELEM_GETTER() {
  return emitInitElemGetterSetter();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITHIDDENPROP_GETTER() {
  return emitInitPropGetterSetter();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITHIDDENPROP_GETTER()
{
    return emitInitPropGetterSetter();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITHIDDENELEM_GETTER() {
  return emitInitElemGetterSetter();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITPROP_SETTER() {
  return emitInitPropGetterSetter();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITPROP_SETTER()
{
    return emitInitPropGetterSetter();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITELEM_SETTER() {
  return emitInitElemGetterSetter();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITHIDDENPROP_SETTER() {
  return emitInitPropGetterSetter();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITHIDDENPROP_SETTER()
{
    return emitInitPropGetterSetter();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITHIDDENELEM_SETTER() {
  return emitInitElemGetterSetter();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*InitElemGetterSetterFn)(JSContext*, jsbytecode*, HandleObject,
                                       HandleValue, HandleObject);
static const VMFunction InitElemGetterSetterInfo =
    FunctionInfo<InitElemGetterSetterFn>(InitGetterSetterOperation,
                                         "InitElemGetterSetterOperation");

template <typename Handler>
bool BaselineCodeGen<Handler>::emitInitElemGetterSetter() {
  // Load index and value in R0 and R1, but keep values on the stack for the
  // decompiler.
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R0);
  masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), R1.scratchReg());
||||||| merged common ancestors
typedef bool (*InitElemGetterSetterFn)(JSContext*, jsbytecode*, HandleObject, HandleValue,
                                       HandleObject);
static const VMFunction InitElemGetterSetterInfo =
    FunctionInfo<InitElemGetterSetterFn>(InitGetterSetterOperation,
                                         "InitElemGetterSetterOperation");

bool
BaselineCompiler::emitInitElemGetterSetter()
{
    MOZ_ASSERT(JSOp(*pc) == JSOP_INITELEM_GETTER ||
               JSOp(*pc) == JSOP_INITHIDDENELEM_GETTER ||
               JSOp(*pc) == JSOP_INITELEM_SETTER ||
               JSOp(*pc) == JSOP_INITHIDDENELEM_SETTER);

    // Load index and value in R0 and R1, but keep values on the stack for the
    // decompiler.
    frame.syncStack(0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R0);
    masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), R1.scratchReg());
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITELEM_INC() {
  // Keep the object and rhs on the stack.
  frame.syncStack(0);

  // Load object in R0, index in R1.
  masm.loadValue(frame.addressOfStackValue(-3), R0);
  masm.loadValue(frame.addressOfStackValue(-2), R1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    prepareVMCall();
=======
  // Call IC.
  if (!emitNextIC()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(R1.scratchReg());
  pushArg(R0);
  masm.unboxObject(frame.addressOfStackValue(frame.peek(-3)), R0.scratchReg());
  pushArg(R0.scratchReg());
  pushArg(ImmPtr(pc));
||||||| merged common ancestors
    pushArg(R1.scratchReg());
    pushArg(R0);
    masm.unboxObject(frame.addressOfStackValue(frame.peek(-3)), R0.scratchReg());
    pushArg(R0.scratchReg());
    pushArg(ImmPtr(pc));
=======
  // Pop the rhs
  frame.pop();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(InitElemGetterSetterInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(InitElemGetterSetterInfo)) {
        return false;
    }
=======
  // Increment index
  Address indexAddr = frame.addressOfStackValue(-1);
#ifdef DEBUG
  Label isInt32;
  masm.branchTestInt32(Assembler::Equal, indexAddr, &isInt32);
  masm.assumeUnreachable("INITELEM_INC index must be Int32");
  masm.bind(&isInt32);
#endif
  masm.incrementInt32Value(indexAddr);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.popn(2);
  return true;
||||||| merged common ancestors
    frame.popn(2);
    return true;
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_GETLOCAL() {
  frame.pushLocal(GET_LOCALNO(handler.pc()));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITELEM_GETTER() {
  return emitInitElemGetterSetter();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITELEM_GETTER()
{
    return emitInitElemGetterSetter();
=======
static BaseValueIndex ComputeAddressOfLocal(MacroAssembler& masm,
                                            Register indexScratch) {
  // Locals are stored in memory at a negative offset from the frame pointer. We
  // negate the index first to effectively subtract it.
  masm.negPtr(indexScratch);
  return BaseValueIndex(BaselineFrameReg, indexScratch,
                        BaselineFrame::reverseOffsetOfLocal(0));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITHIDDENELEM_GETTER() {
  return emitInitElemGetterSetter();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITHIDDENELEM_GETTER()
{
    return emitInitElemGetterSetter();
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_GETLOCAL() {
  Register scratch = R0.scratchReg();
  LoadUint24Operand(masm, 0, scratch);
  BaseValueIndex addr = ComputeAddressOfLocal(masm, scratch);
  masm.loadValue(addr, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITELEM_SETTER() {
  return emitInitElemGetterSetter();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITELEM_SETTER()
{
    return emitInitElemGetterSetter();
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_SETLOCAL() {
  // Ensure no other StackValue refers to the old value, for instance i + (i =
  // 3). This also allows us to use R0 as scratch below.
  frame.syncStack(1);

  uint32_t local = GET_LOCALNO(handler.pc());
  frame.storeStackValue(-1, frame.addressOfLocal(local), R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITHIDDENELEM_SETTER() {
  return emitInitElemGetterSetter();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITHIDDENELEM_SETTER()
{
    return emitInitElemGetterSetter();
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_SETLOCAL() {
  Register scratch = R0.scratchReg();
  LoadUint24Operand(masm, 0, scratch);
  BaseValueIndex addr = ComputeAddressOfLocal(masm, scratch);
  masm.loadValue(frame.addressOfStackValue(-1), R1);
  masm.storeValue(R1, addr);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITELEM_INC() {
  // Keep the object and rhs on the stack.
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITELEM_INC()
{
    // Keep the object and rhs on the stack.
    frame.syncStack(0);
=======
template <>
bool BaselineCompilerCodeGen::emitFormalArgAccess(JSOp op) {
  MOZ_ASSERT(op == JSOP_GETARG || op == JSOP_SETARG);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load object in R0, index in R1.
  masm.loadValue(frame.addressOfStackValue(frame.peek(-3)), R0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R1);
||||||| merged common ancestors
    // Load object in R0, index in R1.
    masm.loadValue(frame.addressOfStackValue(frame.peek(-3)), R0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-2)), R1);
=======
  uint32_t arg = GET_ARGNO(handler.pc());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call IC.
  if (!emitNextIC()) {
    return false;
  }
||||||| merged common ancestors
    // Call IC.
    ICSetElem_Fallback::Compiler stubCompiler(cx);
    if (!emitOpIC(stubCompiler.getStub(&stubSpace_))) {
        return false;
    }
=======
  // Fast path: the script does not use |arguments| or formals don't
  // alias the arguments object.
  if (!handler.script()->argumentsAliasesFormals()) {
    if (op == JSOP_GETARG) {
      frame.pushArg(arg);
    } else {
      // See the comment in emit_JSOP_SETLOCAL.
      frame.syncStack(1);
      frame.storeStackValue(-1, frame.addressOfArg(arg), R0);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Pop the rhs
  frame.pop();

  // Increment index
  Address indexAddr = frame.addressOfStackValue(frame.peek(-1));
#ifdef DEBUG
  Label isInt32;
  masm.branchTestInt32(Assembler::Equal, indexAddr, &isInt32);
  masm.assumeUnreachable("INITELEM_INC index must be Int32");
  masm.bind(&isInt32);
#endif
  masm.incrementInt32Value(indexAddr);
  return true;
}
||||||| merged common ancestors
    // Pop the rhs
    frame.pop();

    // Increment index
    Address indexAddr = frame.addressOfStackValue(frame.peek(-1));
#ifdef DEBUG
    Label isInt32;
    masm.branchTestInt32(Assembler::Equal, indexAddr, &isInt32);
    masm.assumeUnreachable("INITELEM_INC index must be Int32");
    masm.bind(&isInt32);
#endif
    masm.incrementInt32Value(indexAddr);
    return true;
}
=======
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETLOCAL() {
  frame.pushLocal(GET_LOCALNO(pc));
  return true;
}
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GETLOCAL()
{
    frame.pushLocal(GET_LOCALNO(pc));
    return true;
}
=======
  // Sync so that we can use R0.
  frame.syncStack(0);

  // If the script is known to have an arguments object, we can just use it.
  // Else, we *may* have an arguments object (because we can't invalidate
  // when needsArgsObj becomes |true|), so we have to test HAS_ARGS_OBJ.
  Label done;
  if (!handler.script()->needsArgsObj()) {
    Label hasArgsObj;
    masm.branchTest32(Assembler::NonZero, frame.addressOfFlags(),
                      Imm32(BaselineFrame::HAS_ARGS_OBJ), &hasArgsObj);
    if (op == JSOP_GETARG) {
      masm.loadValue(frame.addressOfArg(arg), R0);
    } else {
      frame.storeStackValue(-1, frame.addressOfArg(arg), R0);
    }
    masm.jump(&done);
    masm.bind(&hasArgsObj);
  }

  // Load the arguments object data vector.
  Register reg = R2.scratchReg();
  masm.loadPtr(frame.addressOfArgsObj(), reg);
  masm.loadPrivate(Address(reg, ArgumentsObject::getDataSlotOffset()), reg);

  // Load/store the argument.
  Address argAddr(reg, ArgumentsData::offsetOfArgs() + arg * sizeof(Value));
  if (op == JSOP_GETARG) {
    masm.loadValue(argAddr, R0);
    frame.push(R0);
  } else {
    Register temp = R1.scratchReg();
    masm.guardedCallPreBarrierAnyZone(argAddr, MIRType::Value, temp);
    masm.loadValue(frame.addressOfStackValue(-1), R0);
    masm.storeValue(R0, argAddr);

    MOZ_ASSERT(frame.numUnsyncedSlots() == 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETLOCAL() {
  // Ensure no other StackValue refers to the old value, for instance i + (i =
  // 3). This also allows us to use R0 as scratch below.
  frame.syncStack(1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SETLOCAL()
{
    // Ensure no other StackValue refers to the old value, for instance i + (i = 3).
    // This also allows us to use R0 as scratch below.
    frame.syncStack(1);
=======
    // Reload the arguments object.
    Register reg = R2.scratchReg();
    masm.loadPtr(frame.addressOfArgsObj(), reg);
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t local = GET_LOCALNO(pc);
  storeValue(frame.peek(-1), frame.addressOfLocal(local), R0);
  return true;
||||||| merged common ancestors
    uint32_t local = GET_LOCALNO(pc);
    storeValue(frame.peek(-1), frame.addressOfLocal(local), R0);
    return true;
=======
    Label skipBarrier;

    masm.branchPtrInNurseryChunk(Assembler::Equal, reg, temp, &skipBarrier);
    masm.branchValueIsNurseryCell(Assembler::NotEqual, R0, temp, &skipBarrier);

    masm.call(&postBarrierSlot_);

    masm.bind(&skipBarrier);
  }

  masm.bind(&done);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitFormalArgAccess(uint32_t arg, bool get) {
  // Fast path: the script does not use |arguments| or formals don't
  // alias the arguments object.
  if (!script->argumentsAliasesFormals()) {
    if (get) {
      frame.pushArg(arg);
    } else {
      // See the comment in emit_JSOP_SETLOCAL.
      frame.syncStack(1);
      storeValue(frame.peek(-1), frame.addressOfArg(arg), R0);
    }
||||||| merged common ancestors
bool
BaselineCompiler::emitFormalArgAccess(uint32_t arg, bool get)
{
    // Fast path: the script does not use |arguments| or formals don't
    // alias the arguments object.
    if (!script->argumentsAliasesFormals()) {
        if (get) {
            frame.pushArg(arg);
        } else {
            // See the comment in emit_JSOP_SETLOCAL.
            frame.syncStack(1);
            storeValue(frame.peek(-1), frame.addressOfArg(arg), R0);
        }

        return true;
    }
=======
template <>
bool BaselineInterpreterCodeGen::emitFormalArgAccess(JSOp op) {
  MOZ_ASSERT(op == JSOP_GETARG || op == JSOP_SETARG);
>>>>>>> upstream-releases

<<<<<<< HEAD
    return true;
  }
||||||| merged common ancestors
    // Sync so that we can use R0.
    frame.syncStack(0);

    // If the script is known to have an arguments object, we can just use it.
    // Else, we *may* have an arguments object (because we can't invalidate
    // when needsArgsObj becomes |true|), so we have to test HAS_ARGS_OBJ.
    Label done;
    if (!script->needsArgsObj()) {
        Label hasArgsObj;
        masm.branchTest32(Assembler::NonZero, frame.addressOfFlags(),
                          Imm32(BaselineFrame::HAS_ARGS_OBJ), &hasArgsObj);
        if (get) {
            masm.loadValue(frame.addressOfArg(arg), R0);
        } else {
            storeValue(frame.peek(-1), frame.addressOfArg(arg), R0);
        }
        masm.jump(&done);
        masm.bind(&hasArgsObj);
    }
=======
  // Load the index.
  Register argReg = R1.scratchReg();
  LoadUint16Operand(masm, argReg);

  // If the frame has no arguments object, this must be an unaliased access.
  Label isUnaliased, done;
  masm.branchTest32(Assembler::Zero, frame.addressOfFlags(),
                    Imm32(BaselineFrame::HAS_ARGS_OBJ), &isUnaliased);
  {
    Register reg = R2.scratchReg();

    // If it's an unmapped arguments object, this is an unaliased access.
    loadScript(reg);
    masm.branchTest32(
        Assembler::Zero, Address(reg, JSScript::offsetOfImmutableFlags()),
        Imm32(uint32_t(JSScript::ImmutableFlags::HasMappedArgsObj)),
        &isUnaliased);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Sync so that we can use R0.
  frame.syncStack(0);
||||||| merged common ancestors
    // Load the arguments object data vector.
    Register reg = R2.scratchReg();
    masm.loadPtr(Address(BaselineFrameReg, BaselineFrame::reverseOffsetOfArgsObj()), reg);
    masm.loadPrivate(Address(reg, ArgumentsObject::getDataSlotOffset()), reg);
=======
    // Load the arguments object data vector.
    masm.loadPtr(frame.addressOfArgsObj(), reg);
    masm.loadPrivate(Address(reg, ArgumentsObject::getDataSlotOffset()), reg);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If the script is known to have an arguments object, we can just use it.
  // Else, we *may* have an arguments object (because we can't invalidate
  // when needsArgsObj becomes |true|), so we have to test HAS_ARGS_OBJ.
  Label done;
  if (!script->needsArgsObj()) {
    Label hasArgsObj;
    masm.branchTest32(Assembler::NonZero, frame.addressOfFlags(),
                      Imm32(BaselineFrame::HAS_ARGS_OBJ), &hasArgsObj);
    if (get) {
      masm.loadValue(frame.addressOfArg(arg), R0);
||||||| merged common ancestors
    // Load/store the argument.
    Address argAddr(reg, ArgumentsData::offsetOfArgs() + arg * sizeof(Value));
    if (get) {
        masm.loadValue(argAddr, R0);
        frame.push(R0);
=======
    // Load/store the argument.
    BaseValueIndex argAddr(reg, argReg, ArgumentsData::offsetOfArgs());
    if (op == JSOP_GETARG) {
      masm.loadValue(argAddr, R0);
      frame.push(R0);
>>>>>>> upstream-releases
    } else {
<<<<<<< HEAD
      storeValue(frame.peek(-1), frame.addressOfArg(arg), R0);
    }
    masm.jump(&done);
    masm.bind(&hasArgsObj);
  }

  // Load the arguments object data vector.
  Register reg = R2.scratchReg();
  masm.loadPtr(
      Address(BaselineFrameReg, BaselineFrame::reverseOffsetOfArgsObj()), reg);
  masm.loadPrivate(Address(reg, ArgumentsObject::getDataSlotOffset()), reg);

  // Load/store the argument.
  Address argAddr(reg, ArgumentsData::offsetOfArgs() + arg * sizeof(Value));
  if (get) {
    masm.loadValue(argAddr, R0);
    frame.push(R0);
  } else {
    masm.guardedCallPreBarrier(argAddr, MIRType::Value);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
    masm.storeValue(R0, argAddr);

    MOZ_ASSERT(frame.numUnsyncedSlots() == 0);

    Register temp = R1.scratchReg();
||||||| merged common ancestors
        masm.guardedCallPreBarrier(argAddr, MIRType::Value);
        masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
        masm.storeValue(R0, argAddr);

        MOZ_ASSERT(frame.numUnsyncedSlots() == 0);

        Register temp = R1.scratchReg();
=======
      masm.guardedCallPreBarrierAnyZone(argAddr, MIRType::Value,
                                        R0.scratchReg());
      masm.loadValue(frame.addressOfStackValue(-1), R0);
      masm.storeValue(R0, argAddr);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Reload the arguments object
    Register reg = R2.scratchReg();
    masm.loadPtr(
        Address(BaselineFrameReg, BaselineFrame::reverseOffsetOfArgsObj()),
        reg);
||||||| merged common ancestors
        // Reload the arguments object
        Register reg = R2.scratchReg();
        masm.loadPtr(Address(BaselineFrameReg, BaselineFrame::reverseOffsetOfArgsObj()), reg);
=======
      // Reload the arguments object.
      masm.loadPtr(frame.addressOfArgsObj(), reg);
>>>>>>> upstream-releases

<<<<<<< HEAD
    Label skipBarrier;
||||||| merged common ancestors
        Label skipBarrier;
=======
      Register temp = R1.scratchReg();
      masm.branchPtrInNurseryChunk(Assembler::Equal, reg, temp, &done);
      masm.branchValueIsNurseryCell(Assembler::NotEqual, R0, temp, &done);
>>>>>>> upstream-releases

<<<<<<< HEAD
    masm.branchPtrInNurseryChunk(Assembler::Equal, reg, temp, &skipBarrier);
    masm.branchValueIsNurseryCell(Assembler::NotEqual, R0, temp, &skipBarrier);

    masm.call(&postBarrierSlot_);

    masm.bind(&skipBarrier);
  }
||||||| merged common ancestors
        masm.branchPtrInNurseryChunk(Assembler::Equal, reg, temp, &skipBarrier);
        masm.branchValueIsNurseryCell(Assembler::NotEqual, R0, temp, &skipBarrier);

        masm.call(&postBarrierSlot_);

        masm.bind(&skipBarrier);
    }
=======
      masm.call(&postBarrierSlot_);
    }
    masm.jump(&done);
  }
  masm.bind(&isUnaliased);
  {
    BaseValueIndex addr(BaselineFrameReg, argReg,
                        BaselineFrame::offsetOfArg(0));
    if (op == JSOP_GETARG) {
      masm.loadValue(addr, R0);
      frame.push(R0);
    } else {
      masm.loadValue(frame.addressOfStackValue(-1), R0);
      masm.storeValue(R0, addr);
    }
  }
>>>>>>> upstream-releases

  masm.bind(&done);
  return true;
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETARG() {
  uint32_t arg = GET_ARGNO(pc);
  return emitFormalArgAccess(arg, /* get = */ true);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GETARG()
{
    uint32_t arg = GET_ARGNO(pc);
    return emitFormalArgAccess(arg, /* get = */ true);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETARG() {
  return emitFormalArgAccess(JSOP_GETARG);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETARG() {
  // Ionmonkey can't inline functions with SETARG with magic arguments.
  if (!script->argsObjAliasesFormals() && script->argumentsAliasesFormals()) {
    script->setUninlineable();
  }
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SETARG()
{
    // Ionmonkey can't inline functions with SETARG with magic arguments.
    if (!script->argsObjAliasesFormals() && script->argumentsAliasesFormals()) {
        script->setUninlineable();
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETARG() {
  // Ionmonkey can't inline functions with SETARG with magic arguments.
  if (JSScript* script = handler.maybeScript()) {
    if (!script->argsObjAliasesFormals() && script->argumentsAliasesFormals()) {
      script->setUninlineable();
    }
  }
>>>>>>> upstream-releases

  modifiesArguments_ = true;

<<<<<<< HEAD
  uint32_t arg = GET_ARGNO(pc);
  return emitFormalArgAccess(arg, /* get = */ false);
||||||| merged common ancestors
    uint32_t arg = GET_ARGNO(pc);
    return emitFormalArgAccess(arg, /* get = */ false);
=======
  return emitFormalArgAccess(JSOP_SETARG);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NEWTARGET() {
  if (script->isForEval()) {
    frame.pushEvalNewTarget();
    return true;
  }
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_NEWTARGET()
{
    if (script->isForEval()) {
        frame.pushEvalNewTarget();
        return true;
    }
=======
template <>
void BaselineCompilerCodeGen::loadNumFormalArguments(Register dest) {
  masm.move32(Imm32(handler.function()->nargs()), dest);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(function());
  frame.syncStack(0);
||||||| merged common ancestors
    MOZ_ASSERT(function());
    frame.syncStack(0);
=======
template <>
void BaselineInterpreterCodeGen::loadNumFormalArguments(Register dest) {
  masm.loadFunctionFromCalleeToken(frame.addressOfCalleeToken(), dest);
  masm.load16ZeroExtend(Address(dest, JSFunction::offsetOfNargs()), dest);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (function()->isArrow()) {
    // Arrow functions store their |new.target| value in an
    // extended slot.
    Register scratch = R0.scratchReg();
    masm.loadFunctionFromCalleeToken(frame.addressOfCalleeToken(), scratch);
    masm.loadValue(
        Address(scratch, FunctionExtended::offsetOfArrowNewTargetSlot()), R0);
    frame.push(R0);
    return true;
  }
||||||| merged common ancestors
    if (function()->isArrow()) {
        // Arrow functions store their |new.target| value in an
        // extended slot.
        Register scratch = R0.scratchReg();
        masm.loadFunctionFromCalleeToken(frame.addressOfCalleeToken(), scratch);
        masm.loadValue(Address(scratch, FunctionExtended::offsetOfArrowNewTargetSlot()), R0);
        frame.push(R0);
        return true;
    }
=======
template <typename Handler>
void BaselineCodeGen<Handler>::emitPushNonArrowFunctionNewTarget() {
  // if (isConstructing()) push(argv[Max(numActualArgs, numFormalArgs)])
  Label notConstructing, done;
  masm.branchTestPtr(Assembler::Zero, frame.addressOfCalleeToken(),
                     Imm32(CalleeToken_FunctionConstructing), &notConstructing);
  {
    Register argvLen = R0.scratchReg();
    Register nformals = R1.scratchReg();
    Address actualArgs(BaselineFrameReg,
                       BaselineFrame::offsetOfNumActualArgs());
    masm.loadPtr(actualArgs, argvLen);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // if (isConstructing()) push(argv[Max(numActualArgs, numFormalArgs)])
  Label notConstructing, done;
  masm.branchTestPtr(Assembler::Zero, frame.addressOfCalleeToken(),
                     Imm32(CalleeToken_FunctionConstructing), &notConstructing);
||||||| merged common ancestors
    // if (isConstructing()) push(argv[Max(numActualArgs, numFormalArgs)])
    Label notConstructing, done;
    masm.branchTestPtr(Assembler::Zero, frame.addressOfCalleeToken(),
                       Imm32(CalleeToken_FunctionConstructing), &notConstructing);
=======
    // If argvLen < nformals, set argvlen := nformals.
    loadNumFormalArguments(nformals);
    masm.cmp32Move32(Assembler::Below, argvLen, nformals, nformals, argvLen);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register argvLen = R0.scratchReg();
||||||| merged common ancestors
    Register argvLen = R0.scratchReg();
=======
    BaseValueIndex newTarget(BaselineFrameReg, argvLen,
                             BaselineFrame::offsetOfArg(0));
    masm.loadValue(newTarget, R0);
    masm.jump(&done);
  }
  // else push(undefined)
  masm.bind(&notConstructing);
  masm.moveValue(UndefinedValue(), R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Address actualArgs(BaselineFrameReg, BaselineFrame::offsetOfNumActualArgs());
  masm.loadPtr(actualArgs, argvLen);
||||||| merged common ancestors
    Address actualArgs(BaselineFrameReg, BaselineFrame::offsetOfNumActualArgs());
    masm.loadPtr(actualArgs, argvLen);
=======
  masm.bind(&done);
  frame.push(R0);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label useNFormals;
||||||| merged common ancestors
    Label useNFormals;
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_NEWTARGET() {
  if (handler.script()->isForEval()) {
    frame.pushEvalNewTarget();
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.branchPtr(Assembler::Below, argvLen, Imm32(function()->nargs()),
                 &useNFormals);
||||||| merged common ancestors
    masm.branchPtr(Assembler::Below, argvLen, Imm32(function()->nargs()),
                   &useNFormals);
=======
  MOZ_ASSERT(handler.function());
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  {
    BaseValueIndex newTarget(BaselineFrameReg, argvLen,
                             BaselineFrame::offsetOfArg(0));
    masm.loadValue(newTarget, R0);
    masm.jump(&done);
  }
||||||| merged common ancestors
    {
        BaseValueIndex newTarget(BaselineFrameReg, argvLen, BaselineFrame::offsetOfArg(0));
        masm.loadValue(newTarget, R0);
        masm.jump(&done);
    }
=======
  if (handler.function()->isArrow()) {
    // Arrow functions store their |new.target| value in an
    // extended slot.
    Register scratch = R0.scratchReg();
    masm.loadFunctionFromCalleeToken(frame.addressOfCalleeToken(), scratch);
    masm.loadValue(
        Address(scratch, FunctionExtended::offsetOfArrowNewTargetSlot()), R0);
    frame.push(R0);
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&useNFormals);
||||||| merged common ancestors
    masm.bind(&useNFormals);
=======
  emitPushNonArrowFunctionNewTarget();
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  {
    Address newTarget(
        BaselineFrameReg,
        BaselineFrame::offsetOfArg(0) + (function()->nargs() * sizeof(Value)));
    masm.loadValue(newTarget, R0);
    masm.jump(&done);
  }
||||||| merged common ancestors
    {
        Address newTarget(BaselineFrameReg,
                          BaselineFrame::offsetOfArg(0) + (function()->nargs() * sizeof(Value)));
        masm.loadValue(newTarget, R0);
        masm.jump(&done);
    }
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_NEWTARGET() {
  Register scratch1 = R0.scratchReg();
  Register scratch2 = R1.scratchReg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // else push(undefined)
  masm.bind(&notConstructing);
  masm.moveValue(UndefinedValue(), R0);
||||||| merged common ancestors
    // else push(undefined)
    masm.bind(&notConstructing);
    masm.moveValue(UndefinedValue(), R0);
=======
  Label isFunction, done;
  masm.loadPtr(frame.addressOfCalleeToken(), scratch1);
  masm.branchTestPtr(Assembler::Zero, scratch1, Imm32(CalleeTokenScriptBit),
                     &isFunction);
  {
    // Case 1: eval.
    frame.pushEvalNewTarget();
    masm.jump(&done);
  }

  masm.bind(&isFunction);

  Label notArrow;
  masm.andPtr(Imm32(uint32_t(CalleeTokenMask)), scratch1);
  masm.branchFunctionKind(Assembler::NotEqual, JSFunction::FunctionKind::Arrow,
                          scratch1, scratch2, &notArrow);
  {
    // Case 2: arrow function.
    masm.pushValue(
        Address(scratch1, FunctionExtended::offsetOfArrowNewTargetSlot()));
    masm.jump(&done);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&done);
  frame.push(R0);
||||||| merged common ancestors
    masm.bind(&done);
    frame.push(R0);
=======
  masm.bind(&notArrow);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  // Case 3: non-arrow function.
  emitPushNonArrowFunctionNewTarget();

  masm.bind(&done);
  return true;
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emitThrowConstAssignment() {
  prepareVMCall();
  pushArg(Imm32(JSMSG_BAD_CONST_ASSIGN));

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitThrowConstAssignment() {
  prepareVMCall();
  pushArg(Imm32(JSMSG_BAD_CONST_ASSIGN));
  return callVM(ThrowRuntimeLexicalErrorInfo);
||||||| merged common ancestors
bool
BaselineCompiler::emitThrowConstAssignment()
{
    prepareVMCall();
    pushArg(Imm32(JSMSG_BAD_CONST_ASSIGN));
    return callVM(ThrowRuntimeLexicalErrorInfo);
=======
  using Fn = bool (*)(JSContext*, unsigned);
  return callVM<Fn, jit::ThrowRuntimeLexicalError>();
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_THROWSETCONST() {
  return emitThrowConstAssignment();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_THROWSETALIASEDCONST() {
  return emitThrowConstAssignment();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_THROWSETCALLEE() {
  return emitThrowConstAssignment();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emitUninitializedLexicalCheck(
    const ValueOperand& val) {
  Label done;
  masm.branchTestMagicValue(Assembler::NotEqual, val, JS_UNINITIALIZED_LEXICAL,
                            &done);

<<<<<<< HEAD
  prepareVMCall();
  pushArg(Imm32(JSMSG_UNINITIALIZED_LEXICAL));
  if (!callVM(ThrowRuntimeLexicalErrorInfo)) {
    return false;
  }
||||||| merged common ancestors
    prepareVMCall();
    pushArg(Imm32(JSMSG_UNINITIALIZED_LEXICAL));
    if (!callVM(ThrowRuntimeLexicalErrorInfo)) {
        return false;
    }
=======
  prepareVMCall();
  pushArg(Imm32(JSMSG_UNINITIALIZED_LEXICAL));
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&done);
  return true;
||||||| merged common ancestors
    masm.bind(&done);
    return true;
=======
  using Fn = bool (*)(JSContext*, unsigned);
  if (!callVM<Fn, jit::ThrowRuntimeLexicalError>()) {
    return false;
  }

  masm.bind(&done);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKLEXICAL() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfLocal(GET_LOCALNO(pc)), R0);
  return emitUninitializedLexicalCheck(R0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CHECKLEXICAL()
{
    frame.syncStack(0);
    masm.loadValue(frame.addressOfLocal(GET_LOCALNO(pc)), R0);
    return emitUninitializedLexicalCheck(R0);
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_CHECKLEXICAL() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfLocal(GET_LOCALNO(handler.pc())), R0);
  return emitUninitializedLexicalCheck(R0);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITLEXICAL() {
  return emit_JSOP_SETLOCAL();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITLEXICAL()
{
    return emit_JSOP_SETLOCAL();
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_CHECKLEXICAL() {
  Register scratch = R0.scratchReg();
  LoadUint24Operand(masm, 0, scratch);
  BaseValueIndex addr = ComputeAddressOfLocal(masm, scratch);
  masm.loadValue(addr, R0);
  return emitUninitializedLexicalCheck(R0);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITGLEXICAL() {
  frame.popRegsAndSync(1);
  frame.push(ObjectValue(script->global().lexicalEnvironment()));
  frame.push(R0);
  return emit_JSOP_SETPROP();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITGLEXICAL()
{
    frame.popRegsAndSync(1);
    frame.push(ObjectValue(script->global().lexicalEnvironment()));
    frame.push(R0);
    return emit_JSOP_SETPROP();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITLEXICAL() {
  return emit_JSOP_SETLOCAL();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKALIASEDLEXICAL() {
  frame.syncStack(0);
  masm.loadValue(getEnvironmentCoordinateAddress(R0.scratchReg()), R0);
  return emitUninitializedLexicalCheck(R0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CHECKALIASEDLEXICAL()
{
    frame.syncStack(0);
    masm.loadValue(getEnvironmentCoordinateAddress(R0.scratchReg()), R0);
    return emitUninitializedLexicalCheck(R0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITGLEXICAL() {
  frame.popRegsAndSync(1);
  pushGlobalLexicalEnvironmentValue(R1);
  frame.push(R0);
  return emit_JSOP_SETPROP();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITALIASEDLEXICAL() {
  return emit_JSOP_SETALIASEDVAR();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITALIASEDLEXICAL()
{
    return emit_JSOP_SETALIASEDVAR();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKALIASEDLEXICAL() {
  frame.syncStack(0);
  emitGetAliasedVar(R0);
  return emitUninitializedLexicalCheck(R0);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_UNINITIALIZED() {
  frame.push(MagicValue(JS_UNINITIALIZED_LEXICAL));
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_UNINITIALIZED()
{
    frame.push(MagicValue(JS_UNINITIALIZED_LEXICAL));
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITALIASEDLEXICAL() {
  return emit_JSOP_SETALIASEDVAR();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitCall(JSOp op) {
  MOZ_ASSERT(IsCallOp(op));
||||||| merged common ancestors
bool
BaselineCompiler::emitCall()
{
    MOZ_ASSERT(IsCallPC(pc));
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_UNINITIALIZED() {
  frame.push(MagicValue(JS_UNINITIALIZED_LEXICAL));
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.syncStack(0);
||||||| merged common ancestors
    bool construct = JSOp(*pc) == JSOP_NEW || JSOp(*pc) == JSOP_SUPERCALL;
    uint32_t argc = GET_ARGC(pc);
=======
template <>
bool BaselineCompilerCodeGen::emitCall(JSOp op) {
  MOZ_ASSERT(IsCallOp(op));
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t argc = GET_ARGC(pc);
  masm.move32(Imm32(argc), R0.scratchReg());
||||||| merged common ancestors
    frame.syncStack(0);
    masm.move32(Imm32(argc), R0.scratchReg());
=======
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call IC
  if (!emitNextIC()) {
    return false;
  }
||||||| merged common ancestors
    // Call IC
    ICCall_Fallback::Compiler stubCompiler(cx, /* isConstructing = */ construct,
                                           /* isSpread = */ false);
    if (!emitOpIC(stubCompiler.getStub(&stubSpace_))) {
        return false;
    }
=======
  uint32_t argc = GET_ARGC(handler.pc());
  masm.move32(Imm32(argc), R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Update FrameInfo.
  bool construct = op == JSOP_NEW || op == JSOP_SUPERCALL;
  frame.popn(2 + argc + construct);
  frame.push(R0);
  return true;
}
||||||| merged common ancestors
    // Update FrameInfo.
    frame.popn(2 + argc + construct);
    frame.push(R0);
    return true;
}
=======
  // Call IC
  if (!emitNextIC()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitSpreadCall(JSOp op) {
  MOZ_ASSERT(IsCallOp(op));
||||||| merged common ancestors
bool
BaselineCompiler::emitSpreadCall()
{
    MOZ_ASSERT(IsCallPC(pc));
=======
  // Update FrameInfo.
  bool construct = IsConstructorCallOp(op);
  frame.popn(2 + argc + construct);
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.syncStack(0);
  masm.move32(Imm32(1), R0.scratchReg());
||||||| merged common ancestors
    frame.syncStack(0);
    masm.move32(Imm32(1), R0.scratchReg());
=======
template <>
bool BaselineInterpreterCodeGen::emitCall(JSOp op) {
  MOZ_ASSERT(IsCallOp(op));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call IC
  if (!emitNextIC()) {
    return false;
  }
||||||| merged common ancestors
    // Call IC
    bool construct = JSOp(*pc) == JSOP_SPREADNEW || JSOp(*pc) == JSOP_SPREADSUPERCALL;
    ICCall_Fallback::Compiler stubCompiler(cx, /* isConstructing = */ construct,
                                           /* isSpread = */ true);
    if (!emitOpIC(stubCompiler.getStub(&stubSpace_))) {
        return false;
    }
=======
  // The IC expects argc in R0.
  LoadUint16Operand(masm, R0.scratchReg());
  if (!emitNextIC()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Update FrameInfo.
  bool construct = op == JSOP_SPREADNEW || op == JSOP_SPREADSUPERCALL;
  frame.popn(3 + construct);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    // Update FrameInfo.
    frame.popn(3 + construct);
    frame.push(R0);
    return true;
=======
  // Pop the arguments. We have to reload pc/argc because the IC clobbers them.
  // The return value is in R0 so we can't use that.
  Register scratch = R1.scratchReg();
  uint32_t extraValuesToPop = IsConstructorCallOp(op) ? 3 : 2;
  Register spReg = AsRegister(masm.getStackPointer());
  LoadUint16Operand(masm, scratch);
  masm.computeEffectiveAddress(
      BaseValueIndex(spReg, scratch, extraValuesToPop * sizeof(Value)), spReg);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CALL() {
  return emitCall(JSOP_CALL);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CALL()
{
    return emitCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitSpreadCall(JSOp op) {
  MOZ_ASSERT(IsCallOp(op));

  frame.syncStack(0);
  masm.move32(Imm32(1), R0.scratchReg());

  // Call IC
  if (!emitNextIC()) {
    return false;
  }

  // Update FrameInfo.
  bool construct = op == JSOP_SPREADNEW || op == JSOP_SPREADSUPERCALL;
  frame.popn(3 + construct);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CALL_IGNORES_RV() {
  return emitCall(JSOP_CALL_IGNORES_RV);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CALL_IGNORES_RV()
{
    return emitCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CALL() {
  return emitCall(JSOP_CALL);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CALLITER() {
  return emitCall(JSOP_CALLITER);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CALLITER()
{
    return emitCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CALL_IGNORES_RV() {
  return emitCall(JSOP_CALL_IGNORES_RV);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NEW() {
  return emitCall(JSOP_NEW);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_NEW()
{
    return emitCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CALLITER() {
  return emitCall(JSOP_CALLITER);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SUPERCALL() {
  return emitCall(JSOP_SUPERCALL);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SUPERCALL()
{
    return emitCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_NEW() {
  return emitCall(JSOP_NEW);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FUNCALL() {
  return emitCall(JSOP_FUNCALL);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_FUNCALL()
{
    return emitCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SUPERCALL() {
  return emitCall(JSOP_SUPERCALL);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FUNAPPLY() {
  return emitCall(JSOP_FUNAPPLY);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_FUNAPPLY()
{
    return emitCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FUNCALL() {
  return emitCall(JSOP_FUNCALL);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_EVAL() {
  return emitCall(JSOP_EVAL);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_EVAL()
{
    return emitCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FUNAPPLY() {
  return emitCall(JSOP_FUNAPPLY);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTEVAL() {
  return emitCall(JSOP_STRICTEVAL);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_STRICTEVAL()
{
    return emitCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_EVAL() {
  return emitCall(JSOP_EVAL);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SPREADCALL() {
  return emitSpreadCall(JSOP_SPREADCALL);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SPREADCALL()
{
    return emitSpreadCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTEVAL() {
  return emitCall(JSOP_STRICTEVAL);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SPREADNEW() {
  return emitSpreadCall(JSOP_SPREADNEW);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SPREADNEW()
{
    return emitSpreadCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SPREADCALL() {
  return emitSpreadCall(JSOP_SPREADCALL);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SPREADSUPERCALL() {
  return emitSpreadCall(JSOP_SPREADSUPERCALL);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SPREADSUPERCALL()
{
    return emitSpreadCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SPREADNEW() {
  return emitSpreadCall(JSOP_SPREADNEW);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SPREADEVAL() {
  return emitSpreadCall(JSOP_SPREADEVAL);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SPREADEVAL()
{
    return emitSpreadCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SPREADSUPERCALL() {
  return emitSpreadCall(JSOP_SPREADSUPERCALL);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTSPREADEVAL() {
  return emitSpreadCall(JSOP_STRICTSPREADEVAL);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_STRICTSPREADEVAL()
{
    return emitSpreadCall();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SPREADEVAL() {
  return emitSpreadCall(JSOP_SPREADEVAL);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*OptimizeSpreadCallFn)(JSContext*, HandleValue, bool*);
static const VMFunction OptimizeSpreadCallInfo =
    FunctionInfo<OptimizeSpreadCallFn>(OptimizeSpreadCall,
                                       "OptimizeSpreadCall");
||||||| merged common ancestors
typedef bool (*OptimizeSpreadCallFn)(JSContext*, HandleValue, bool*);
static const VMFunction OptimizeSpreadCallInfo =
    FunctionInfo<OptimizeSpreadCallFn>(OptimizeSpreadCall, "OptimizeSpreadCall");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_STRICTSPREADEVAL() {
  return emitSpreadCall(JSOP_STRICTSPREADEVAL);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_OPTIMIZE_SPREADCALL() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_OPTIMIZE_SPREADCALL()
{
    frame.syncStack(0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_OPTIMIZE_SPREADCALL() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-1), R0);
>>>>>>> upstream-releases

  prepareVMCall();
  pushArg(R0);

<<<<<<< HEAD
  if (!callVM(OptimizeSpreadCallInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(OptimizeSpreadCallInfo)) {
        return false;
    }
=======
  using Fn = bool (*)(JSContext*, HandleValue, bool*);
  if (!callVM<Fn, OptimizeSpreadCall>()) {
    return false;
  }
>>>>>>> upstream-releases

  masm.boxNonDouble(JSVAL_TYPE_BOOLEAN, ReturnReg, R0);
  frame.push(R0);
  return true;
}

<<<<<<< HEAD
typedef bool (*ImplicitThisFn)(JSContext*, HandleObject, HandlePropertyName,
                               MutableHandleValue);
const VMFunction ImplicitThisInfo = FunctionInfo<ImplicitThisFn>(
    ImplicitThisOperation, "ImplicitThisOperation");

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_IMPLICITTHIS() {
  frame.syncStack(0);
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
||||||| merged common ancestors
typedef bool (*ImplicitThisFn)(JSContext*, HandleObject, HandlePropertyName,
                               MutableHandleValue);
const VMFunction jit::ImplicitThisInfo =
    FunctionInfo<ImplicitThisFn>(ImplicitThisOperation, "ImplicitThisOperation");

bool
BaselineCompiler::emit_JSOP_IMPLICITTHIS()
{
    frame.syncStack(0);
    masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_IMPLICITTHIS() {
  frame.syncStack(0);
  masm.loadPtr(frame.addressOfEnvironmentChain(), R0.scratchReg());
>>>>>>> upstream-releases

  prepareVMCall();

<<<<<<< HEAD
  pushArg(ImmGCPtr(script->getName(pc)));
  pushArg(R0.scratchReg());
||||||| merged common ancestors
    pushArg(ImmGCPtr(script->getName(pc)));
    pushArg(R0.scratchReg());
=======
  pushScriptNameArg(R1.scratchReg(), R2.scratchReg());
  pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(ImplicitThisInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(ImplicitThisInfo)) {
        return false;
    }
=======
  using Fn = bool (*)(JSContext*, HandleObject, HandlePropertyName,
                      MutableHandleValue);
  if (!callVM<Fn, ImplicitThisOperation>()) {
    return false;
  }
>>>>>>> upstream-releases

  frame.push(R0);
  return true;
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GIMPLICITTHIS() {
  if (!script->hasNonSyntacticScope()) {
    frame.push(UndefinedValue());
    return true;
  }
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GIMPLICITTHIS()
{
    if (!script->hasNonSyntacticScope()) {
        frame.push(UndefinedValue());
        return true;
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GIMPLICITTHIS() {
  auto pushUndefined = [this]() {
    frame.push(UndefinedValue());
    return true;
  };
  auto emitImplicitThis = [this]() { return emit_JSOP_IMPLICITTHIS(); };
>>>>>>> upstream-releases

<<<<<<< HEAD
  return emit_JSOP_IMPLICITTHIS();
||||||| merged common ancestors
    return emit_JSOP_IMPLICITTHIS();
=======
  return emitTestScriptFlag(JSScript::ImmutableFlags::HasNonSyntacticScope,
                            emitImplicitThis, pushUndefined, R2.scratchReg());
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INSTANCEOF() {
  frame.popRegsAndSync(2);

  if (!emitNextIC()) {
    return false;
  }

  frame.push(R0);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TYPEOF() {
  frame.popRegsAndSync(1);

  if (!emitNextIC()) {
    return false;
  }

  frame.push(R0);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TYPEOFEXPR() {
  return emit_JSOP_TYPEOF();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_THROWMSG() {
  prepareVMCall();
  pushUint16BytecodeOperandArg(R2.scratchReg());

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_THROWMSG() {
  prepareVMCall();
  pushArg(Imm32(GET_UINT16(pc)));
  return callVM(ThrowMsgInfo);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_THROWMSG()
{
    prepareVMCall();
    pushArg(Imm32(GET_UINT16(pc)));
    return callVM(ThrowMsgInfo);
=======
  using Fn = bool (*)(JSContext*, const unsigned);
  return callVM<Fn, js::ThrowMsgOperation>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*ThrowFn)(JSContext*, HandleValue);
static const VMFunction ThrowInfo = FunctionInfo<ThrowFn>(js::Throw, "Throw");

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_THROW() {
  // Keep value to throw in R0.
  frame.popRegsAndSync(1);
||||||| merged common ancestors
typedef bool (*ThrowFn)(JSContext*, HandleValue);
static const VMFunction ThrowInfo = FunctionInfo<ThrowFn>(js::Throw, "Throw");

bool
BaselineCompiler::emit_JSOP_THROW()
{
    // Keep value to throw in R0.
    frame.popRegsAndSync(1);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_THROW() {
  // Keep value to throw in R0.
  frame.popRegsAndSync(1);
>>>>>>> upstream-releases

  prepareVMCall();
  pushArg(R0);

<<<<<<< HEAD
  return callVM(ThrowInfo);
||||||| merged common ancestors
    return callVM(ThrowInfo);
=======
  using Fn = bool (*)(JSContext*, HandleValue);
  return callVM<Fn, js::ThrowOperation>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TRY() {
  if (!emit_JSOP_JUMPTARGET()) {
    return false;
  }
||||||| merged common ancestors
typedef bool (*ThrowingFn)(JSContext*, HandleValue);
static const VMFunction ThrowingInfo =
    FunctionInfo<ThrowingFn>(js::ThrowingOperation, "ThrowingOperation");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TRY() {
  // Ionmonkey can't inline function with JSOP_TRY.
  if (JSScript* script = handler.maybeScript()) {
    script->setUninlineable();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Ionmonkey can't inline function with JSOP_TRY.
  script->setUninlineable();
  return true;
}
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_THROWING()
{
    // Keep value to throw in R0.
    frame.popRegsAndSync(1);
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FINALLY() {
  // JSOP_FINALLY has a def count of 2, but these values are already on the
  // stack (they're pushed by JSOP_GOSUB). Update the compiler's stack state.
  frame.setStackDepth(frame.stackDepth() + 2);
||||||| merged common ancestors
    prepareVMCall();
    pushArg(R0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FINALLY() {
  // JSOP_FINALLY has a def count of 2, but these values are already on the
  // stack (they're pushed by JSOP_GOSUB). Update the compiler's stack state.
  frame.incStackDepth(2);
>>>>>>> upstream-releases

  // To match the interpreter, emit an interrupt check at the start of the
  // finally block.
  return emitInterruptCheck();
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GOSUB() {
  // Jump to the finally block.
  frame.syncStack(0);
  emitJump();
  return true;
}

static void LoadBaselineScriptResumeEntries(MacroAssembler& masm,
                                            JSScript* script, Register dest,
                                            Register scratch) {
  MOZ_ASSERT(dest != scratch);

  masm.movePtr(ImmGCPtr(script), dest);
  masm.loadPtr(Address(dest, JSScript::offsetOfBaselineScript()), dest);
  masm.load32(Address(dest, BaselineScript::offsetOfResumeEntriesOffset()),
              scratch);
  masm.addPtr(scratch, dest);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_TRY()
{
    if (!emit_JSOP_JUMPTARGET()) {
        return false;
    }

    // Ionmonkey can't inline function with JSOP_TRY.
    script->setUninlineable();
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GOSUB() {
  // Jump to the finally block.
  frame.syncStack(0);
  emitJump();
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_RETSUB() {
  frame.popRegsAndSync(2);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_FINALLY()
{
    // JSOP_FINALLY has a def count of 2, but these values are already on the
    // stack (they're pushed by JSOP_GOSUB). Update the compiler's stack state.
    frame.setStackDepth(frame.stackDepth() + 2);
=======
static void LoadBaselineScriptResumeEntries(MacroAssembler& masm,
                                            JSScript* script, Register dest,
                                            Register scratch) {
  MOZ_ASSERT(dest != scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label isReturn;
  masm.branchTestBooleanTruthy(/* branchIfTrue = */ false, R0, &isReturn);

  // R0 is |true|. We need to throw R1.
  prepareVMCall();
  pushArg(R1);
  if (!callVM(ThrowInfo)) {
    return false;
  }

  masm.bind(&isReturn);

  // R0 is |false|. R1 contains the resumeIndex to jump to.
  Register scratch1 = R2.scratchReg();
  Register scratch2 = R0.scratchReg();
  LoadBaselineScriptResumeEntries(masm, script, scratch1, scratch2);
  masm.unboxInt32(R1, scratch2);
  masm.loadPtr(
      BaseIndex(scratch1, scratch2, ScaleFromElemWidth(sizeof(uintptr_t))),
      scratch1);
  masm.jump(scratch1);
  return true;
||||||| merged common ancestors
    // To match the interpreter, emit an interrupt check at the start of the
    // finally block.
    return emitInterruptCheck();
=======
  masm.movePtr(ImmGCPtr(script), dest);
  masm.loadPtr(Address(dest, JSScript::offsetOfBaselineScript()), dest);
  masm.load32(Address(dest, BaselineScript::offsetOfResumeEntriesOffset()),
              scratch);
  masm.addPtr(scratch, dest);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <>
template <typename F1, typename F2>
MOZ_MUST_USE bool BaselineCompilerCodeGen::emitDebugInstrumentation(
    const F1& ifDebuggee, const Maybe<F2>& ifNotDebuggee) {
  // The JIT calls either ifDebuggee or (if present) ifNotDebuggee, because it
  // knows statically whether we're compiling with debug instrumentation.
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GOSUB()
{
    // Push |false| so that RETSUB knows the value on top of the
    // stack is not an exception but the offset to the op following
    // this GOSUB.
    frame.push(BooleanValue(false));
=======
template <typename Handler>
void BaselineCodeGen<Handler>::emitInterpJumpToResumeEntry(Register script,
                                                           Register resumeIndex,
                                                           Register scratch) {
  // Load JSScript::scriptData() into |script|.
  masm.loadPtr(Address(script, JSScript::offsetOfScriptData()), script);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (handler.compileDebugInstrumentation()) {
    return ifDebuggee();
  }
||||||| merged common ancestors
    int32_t nextOffset = script->pcToOffset(GetNextPc(pc));
    frame.push(Int32Value(nextOffset));
=======
  // Load the resume pcOffset in |resumeIndex|.
  masm.load32(Address(script, SharedScriptData::offsetOfResumeOffsetsOffset()),
              scratch);
  masm.computeEffectiveAddress(BaseIndex(scratch, resumeIndex, TimesFour),
                               scratch);
  masm.load32(BaseIndex(script, scratch, TimesOne), resumeIndex);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (ifNotDebuggee) {
    return (*ifNotDebuggee)();
  }

  return true;
}
||||||| merged common ancestors
    // Jump to the finally block.
    frame.syncStack(0);
    jsbytecode* target = pc + GET_JUMP_OFFSET(pc);
    masm.jump(labelOf(target));
    return true;
}
=======
  // Load pc* in |script|.
  masm.load32(Address(script, SharedScriptData::offsetOfCodeOffset()), scratch);
  masm.addPtr(scratch, script);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <>
template <typename F1, typename F2>
MOZ_MUST_USE bool BaselineInterpreterCodeGen::emitDebugInstrumentation(
    const F1& ifDebuggee, const Maybe<F2>& ifNotDebuggee) {
  // The interpreter emits both ifDebuggee and (if present) ifNotDebuggee
  // paths, with a branch based on the frame's DEBUGGEE flag.

  Label isNotDebuggee, done;
  masm.branchTest32(Assembler::Zero, frame.addressOfFlags(),
                    Imm32(BaselineFrame::DEBUGGEE), &isNotDebuggee);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_RETSUB()
{
    frame.popRegsAndSync(2);
=======
  // Add resume offset to PC, jump to it.
  masm.addPtr(resumeIndex, script);
  Address pcAddr(BaselineFrameReg,
                 BaselineFrame::reverseOffsetOfInterpreterPC());
  masm.storePtr(script, pcAddr);
  emitJumpToInterpretOpLabel();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!ifDebuggee()) {
    return false;
  }

  if (ifNotDebuggee) {
    masm.jump(&done);
  }

  masm.bind(&isNotDebuggee);

  if (ifNotDebuggee && !(*ifNotDebuggee)()) {
    return false;
  }

  masm.bind(&done);
  return true;
||||||| merged common ancestors
    ICRetSub_Fallback::Compiler stubCompiler(cx);
    return emitOpIC(stubCompiler.getStub(&stubSpace_));
=======
template <>
void BaselineCompilerCodeGen::jumpToResumeEntry(Register resumeIndex,
                                                Register scratch1,
                                                Register scratch2) {
  LoadBaselineScriptResumeEntries(masm, handler.script(), scratch1, scratch2);
  masm.loadPtr(
      BaseIndex(scratch1, resumeIndex, ScaleFromElemWidth(sizeof(uintptr_t))),
      scratch1);
  masm.jump(scratch1);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*PushLexicalEnvFn)(JSContext*, BaselineFrame*,
                                 Handle<LexicalScope*>);
static const VMFunction PushLexicalEnvInfo =
    FunctionInfo<PushLexicalEnvFn>(jit::PushLexicalEnv, "PushLexicalEnv");
||||||| merged common ancestors
typedef bool (*PushLexicalEnvFn)(JSContext*, BaselineFrame*, Handle<LexicalScope*>);
static const VMFunction PushLexicalEnvInfo =
    FunctionInfo<PushLexicalEnvFn>(jit::PushLexicalEnv, "PushLexicalEnv");
=======
template <>
void BaselineInterpreterCodeGen::jumpToResumeEntry(Register resumeIndex,
                                                   Register scratch1,
                                                   Register scratch2) {
  loadScript(scratch1);
  emitInterpJumpToResumeEntry(scratch1, resumeIndex, scratch2);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_PUSHLEXICALENV() {
  LexicalScope& scope = script->getScope(pc)->template as<LexicalScope>();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_PUSHLEXICALENV()
{
    LexicalScope& scope = script->getScope(pc)->as<LexicalScope>();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_RETSUB() {
  frame.popRegsAndSync(2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call a stub to push the block on the block chain.
  prepareVMCall();
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
||||||| merged common ancestors
    // Call a stub to push the block on the block chain.
    prepareVMCall();
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
=======
  Label isReturn;
  masm.branchTestBooleanTruthy(/* branchIfTrue = */ false, R0, &isReturn);
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(ImmGCPtr(&scope));
  pushArg(R0.scratchReg());
||||||| merged common ancestors
    pushArg(ImmGCPtr(&scope));
    pushArg(R0.scratchReg());
=======
  // R0 is |true|. We need to throw R1.
  prepareVMCall();
  pushArg(R1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return callVM(PushLexicalEnvInfo);
}
||||||| merged common ancestors
    return callVM(PushLexicalEnvInfo);
}
=======
  using Fn = bool (*)(JSContext*, HandleValue);
  if (!callVM<Fn, js::ThrowOperation>()) {
    return false;
  }
>>>>>>> upstream-releases

  masm.bind(&isReturn);

<<<<<<< HEAD
typedef bool (*DebugLeaveThenPopLexicalEnvFn)(JSContext*, BaselineFrame*,
                                              jsbytecode*);
static const VMFunction DebugLeaveThenPopLexicalEnvInfo =
    FunctionInfo<DebugLeaveThenPopLexicalEnvFn>(
        jit::DebugLeaveThenPopLexicalEnv, "DebugLeaveThenPopLexicalEnv");
||||||| merged common ancestors
typedef bool (*DebugLeaveThenPopLexicalEnvFn)(JSContext*, BaselineFrame*, jsbytecode*);
static const VMFunction DebugLeaveThenPopLexicalEnvInfo =
    FunctionInfo<DebugLeaveThenPopLexicalEnvFn>(jit::DebugLeaveThenPopLexicalEnv,
                                                "DebugLeaveThenPopLexicalEnv");
=======
  // R0 is |false|. R1 contains the resumeIndex to jump to.
  Register resumeIndexReg = R1.scratchReg();
  masm.unboxInt32(R1, resumeIndexReg);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_POPLEXICALENV() {
  prepareVMCall();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_POPLEXICALENV()
{
    prepareVMCall();
=======
  Register scratch1 = R2.scratchReg();
  Register scratch2 = R0.scratchReg();
  jumpToResumeEntry(resumeIndexReg, scratch1, scratch2);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
||||||| merged common ancestors
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());

    if (compileDebugInstrumentation_) {
        pushArg(ImmPtr(pc));
        pushArg(R0.scratchReg());
        return callVM(DebugLeaveThenPopLexicalEnvInfo);
    }
=======
template <>
template <typename F1, typename F2>
MOZ_MUST_USE bool BaselineCompilerCodeGen::emitDebugInstrumentation(
    const F1& ifDebuggee, const Maybe<F2>& ifNotDebuggee) {
  // The JIT calls either ifDebuggee or (if present) ifNotDebuggee, because it
  // knows statically whether we're compiling with debug instrumentation.

  if (handler.compileDebugInstrumentation()) {
    return ifDebuggee();
  }

  if (ifNotDebuggee) {
    return (*ifNotDebuggee)();
  }

  return true;
}

template <>
template <typename F1, typename F2>
MOZ_MUST_USE bool BaselineInterpreterCodeGen::emitDebugInstrumentation(
    const F1& ifDebuggee, const Maybe<F2>& ifNotDebuggee) {
  // The interpreter emits both ifDebuggee and (if present) ifNotDebuggee
  // paths, with a branch based on the frame's DEBUGGEE flag.

  Label isNotDebuggee, done;
  masm.branchTest32(Assembler::Zero, frame.addressOfFlags(),
                    Imm32(BaselineFrame::DEBUGGEE), &isNotDebuggee);

  if (!ifDebuggee()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  auto ifDebuggee = [this]() {
    pushArg(ImmPtr(pc));
    pushArg(R0.scratchReg());
    return callVM(DebugLeaveThenPopLexicalEnvInfo);
  };
  auto ifNotDebuggee = [this]() {
    pushArg(R0.scratchReg());
    return callVM(PopLexicalEnvInfo);
  };
  return emitDebugInstrumentation(ifDebuggee, mozilla::Some(ifNotDebuggee));
}
||||||| merged common ancestors
    pushArg(R0.scratchReg());
    return callVM(PopLexicalEnvInfo);
}
=======
  if (ifNotDebuggee) {
    masm.jump(&done);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*FreshenLexicalEnvFn)(JSContext*, BaselineFrame*);
static const VMFunction FreshenLexicalEnvInfo =
    FunctionInfo<FreshenLexicalEnvFn>(jit::FreshenLexicalEnv,
                                      "FreshenLexicalEnv");
||||||| merged common ancestors
typedef bool (*FreshenLexicalEnvFn)(JSContext*, BaselineFrame*);
static const VMFunction FreshenLexicalEnvInfo =
    FunctionInfo<FreshenLexicalEnvFn>(jit::FreshenLexicalEnv, "FreshenLexicalEnv");
=======
  masm.bind(&isNotDebuggee);
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*DebugLeaveThenFreshenLexicalEnvFn)(JSContext*, BaselineFrame*,
                                                  jsbytecode*);
static const VMFunction DebugLeaveThenFreshenLexicalEnvInfo =
    FunctionInfo<DebugLeaveThenFreshenLexicalEnvFn>(
        jit::DebugLeaveThenFreshenLexicalEnv,
        "DebugLeaveThenFreshenLexicalEnv");
||||||| merged common ancestors
typedef bool (*DebugLeaveThenFreshenLexicalEnvFn)(JSContext*, BaselineFrame*, jsbytecode*);
static const VMFunction DebugLeaveThenFreshenLexicalEnvInfo =
    FunctionInfo<DebugLeaveThenFreshenLexicalEnvFn>(jit::DebugLeaveThenFreshenLexicalEnv,
                                                    "DebugLeaveThenFreshenLexicalEnv");
=======
  if (ifNotDebuggee && !(*ifNotDebuggee)()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FRESHENLEXICALENV() {
  prepareVMCall();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_FRESHENLEXICALENV()
{
    prepareVMCall();

    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
=======
  masm.bind(&done);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_PUSHLEXICALENV() {
  // Call a stub to push the block on the block chain.
  prepareVMCall();
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
||||||| merged common ancestors
    if (compileDebugInstrumentation_) {
        pushArg(ImmPtr(pc));
        pushArg(R0.scratchReg());
        return callVM(DebugLeaveThenFreshenLexicalEnvInfo);
    }
=======
  pushScriptGCThingArg(ScriptGCThingType::Scope, R1.scratchReg(),
                       R2.scratchReg());
  pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  auto ifDebuggee = [this]() {
    pushArg(ImmPtr(pc));
    pushArg(R0.scratchReg());
    return callVM(DebugLeaveThenFreshenLexicalEnvInfo);
  };
  auto ifNotDebuggee = [this]() {
    pushArg(R0.scratchReg());
    return callVM(FreshenLexicalEnvInfo);
  };
  return emitDebugInstrumentation(ifDebuggee, mozilla::Some(ifNotDebuggee));
||||||| merged common ancestors
    pushArg(R0.scratchReg());
    return callVM(FreshenLexicalEnvInfo);
=======
  using Fn = bool (*)(JSContext*, BaselineFrame*, Handle<LexicalScope*>);
  return callVM<Fn, jit::PushLexicalEnv>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*RecreateLexicalEnvFn)(JSContext*, BaselineFrame*);
static const VMFunction RecreateLexicalEnvInfo =
    FunctionInfo<RecreateLexicalEnvFn>(jit::RecreateLexicalEnv,
                                       "RecreateLexicalEnv");

typedef bool (*DebugLeaveThenRecreateLexicalEnvFn)(JSContext*, BaselineFrame*,
                                                   jsbytecode*);
static const VMFunction DebugLeaveThenRecreateLexicalEnvInfo =
    FunctionInfo<DebugLeaveThenRecreateLexicalEnvFn>(
        jit::DebugLeaveThenRecreateLexicalEnv,
        "DebugLeaveThenRecreateLexicalEnv");
||||||| merged common ancestors

typedef bool (*RecreateLexicalEnvFn)(JSContext*, BaselineFrame*);
static const VMFunction RecreateLexicalEnvInfo =
    FunctionInfo<RecreateLexicalEnvFn>(jit::RecreateLexicalEnv, "RecreateLexicalEnv");

typedef bool (*DebugLeaveThenRecreateLexicalEnvFn)(JSContext*, BaselineFrame*, jsbytecode*);
static const VMFunction DebugLeaveThenRecreateLexicalEnvInfo =
    FunctionInfo<DebugLeaveThenRecreateLexicalEnvFn>(jit::DebugLeaveThenRecreateLexicalEnv,
                                                     "DebugLeaveThenRecreateLexicalEnv");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_POPLEXICALENV() {
  frame.syncStack(0);

  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_RECREATELEXICALENV() {
  prepareVMCall();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_RECREATELEXICALENV()
{
    prepareVMCall();
=======
  auto ifDebuggee = [this]() {
    prepareVMCall();
    pushBytecodePCArg();
    pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());

  auto ifDebuggee = [this]() {
    pushArg(ImmPtr(pc));
    pushArg(R0.scratchReg());
    return callVM(DebugLeaveThenRecreateLexicalEnvInfo);
  };
  auto ifNotDebuggee = [this]() {
||||||| merged common ancestors
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());

    if (compileDebugInstrumentation_) {
        pushArg(ImmPtr(pc));
        pushArg(R0.scratchReg());
        return callVM(DebugLeaveThenRecreateLexicalEnvInfo);
    }

=======
    using Fn = bool (*)(JSContext*, BaselineFrame*, jsbytecode*);
    return callVM<Fn, jit::DebugLeaveThenPopLexicalEnv>();
  };
  auto ifNotDebuggee = [this]() {
    prepareVMCall();
>>>>>>> upstream-releases
    pushArg(R0.scratchReg());
<<<<<<< HEAD
    return callVM(RecreateLexicalEnvInfo);
  };
  return emitDebugInstrumentation(ifDebuggee, mozilla::Some(ifNotDebuggee));
||||||| merged common ancestors
    return callVM(RecreateLexicalEnvInfo);
=======

    using Fn = bool (*)(JSContext*, BaselineFrame*);
    return callVM<Fn, jit::PopLexicalEnv>();
  };
  return emitDebugInstrumentation(ifDebuggee, mozilla::Some(ifNotDebuggee));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*DebugLeaveLexicalEnvFn)(JSContext*, BaselineFrame*, jsbytecode*);
static const VMFunction DebugLeaveLexicalEnvInfo =
    FunctionInfo<DebugLeaveLexicalEnvFn>(jit::DebugLeaveLexicalEnv,
                                         "DebugLeaveLexicalEnv");
||||||| merged common ancestors
typedef bool (*DebugLeaveLexicalEnvFn)(JSContext*, BaselineFrame*, jsbytecode*);
static const VMFunction DebugLeaveLexicalEnvInfo =
    FunctionInfo<DebugLeaveLexicalEnvFn>(jit::DebugLeaveLexicalEnv, "DebugLeaveLexicalEnv");

bool
BaselineCompiler::emit_JSOP_DEBUGLEAVELEXICALENV()
{
    if (!compileDebugInstrumentation_) {
        return true;
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FRESHENLEXICALENV() {
  frame.syncStack(0);

  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEBUGLEAVELEXICALENV() {
  auto ifDebuggee = [this]() {
||||||| merged common ancestors
=======
  auto ifDebuggee = [this]() {
    prepareVMCall();
    pushBytecodePCArg();
    pushArg(R0.scratchReg());

    using Fn = bool (*)(JSContext*, BaselineFrame*, jsbytecode*);
    return callVM<Fn, jit::DebugLeaveThenFreshenLexicalEnv>();
  };
  auto ifNotDebuggee = [this]() {
>>>>>>> upstream-releases
    prepareVMCall();
    pushArg(R0.scratchReg());
<<<<<<< HEAD
    return callVM(DebugLeaveLexicalEnvInfo);
  };
  return emitDebugInstrumentation(ifDebuggee);
||||||| merged common ancestors

    return callVM(DebugLeaveLexicalEnvInfo);
=======

    using Fn = bool (*)(JSContext*, BaselineFrame*);
    return callVM<Fn, jit::FreshenLexicalEnv>();
  };
  return emitDebugInstrumentation(ifDebuggee, mozilla::Some(ifNotDebuggee));
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_RECREATELEXICALENV() {
  frame.syncStack(0);

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_PUSHVARENV() {
  prepareVMCall();
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
  pushArg(ImmGCPtr(script->getScope(pc)));
  pushArg(R0.scratchReg());
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_PUSHVARENV()
{
    prepareVMCall();
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
    pushArg(ImmGCPtr(script->getScope(pc)));
    pushArg(R0.scratchReg());
=======
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());

  auto ifDebuggee = [this]() {
    prepareVMCall();
    pushBytecodePCArg();
    pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  return callVM(PushVarEnvInfo);
}
||||||| merged common ancestors
    return callVM(PushVarEnvInfo);
}
=======
    using Fn = bool (*)(JSContext*, BaselineFrame*, jsbytecode*);
    return callVM<Fn, jit::DebugLeaveThenRecreateLexicalEnv>();
  };
  auto ifNotDebuggee = [this]() {
    prepareVMCall();
    pushArg(R0.scratchReg());
>>>>>>> upstream-releases

    using Fn = bool (*)(JSContext*, BaselineFrame*);
    return callVM<Fn, jit::RecreateLexicalEnv>();
  };
  return emitDebugInstrumentation(ifDebuggee, mozilla::Some(ifNotDebuggee));
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_POPVARENV() {
  prepareVMCall();
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
  pushArg(R0.scratchReg());
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_POPVARENV()
{
    prepareVMCall();
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
    pushArg(R0.scratchReg());
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEBUGLEAVELEXICALENV() {
  auto ifDebuggee = [this]() {
    prepareVMCall();
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
    pushBytecodePCArg();
    pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  return callVM(PopVarEnvInfo);
||||||| merged common ancestors
    return callVM(PopVarEnvInfo);
=======
    using Fn = bool (*)(JSContext*, BaselineFrame*, jsbytecode*);
    return callVM<Fn, jit::DebugLeaveLexicalEnv>();
  };
  return emitDebugInstrumentation(ifDebuggee);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*EnterWithFn)(JSContext*, BaselineFrame*, HandleValue,
                            Handle<WithScope*>);
static const VMFunction EnterWithInfo =
    FunctionInfo<EnterWithFn>(jit::EnterWith, "EnterWith");

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ENTERWITH() {
  WithScope& withScope = script->getScope(pc)->template as<WithScope>();

  // Pop "with" object to R0.
  frame.popRegsAndSync(1);
||||||| merged common ancestors
typedef bool (*EnterWithFn)(JSContext*, BaselineFrame*, HandleValue, Handle<WithScope*>);
static const VMFunction EnterWithInfo =
    FunctionInfo<EnterWithFn>(jit::EnterWith, "EnterWith");

bool
BaselineCompiler::emit_JSOP_ENTERWITH()
{
    WithScope& withScope = script->getScope(pc)->as<WithScope>();

    // Pop "with" object to R0.
    frame.popRegsAndSync(1);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_PUSHVARENV() {
  prepareVMCall();
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
  pushScriptGCThingArg(ScriptGCThingType::Scope, R1.scratchReg(),
                       R2.scratchReg());
  pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call a stub to push the object onto the environment chain.
  prepareVMCall();
  masm.loadBaselineFramePtr(BaselineFrameReg, R1.scratchReg());
||||||| merged common ancestors
    // Call a stub to push the object onto the environment chain.
    prepareVMCall();
    masm.loadBaselineFramePtr(BaselineFrameReg, R1.scratchReg());
=======
  using Fn = bool (*)(JSContext*, BaselineFrame*, HandleScope);
  return callVM<Fn, jit::PushVarEnv>();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(ImmGCPtr(&withScope));
  pushArg(R0);
  pushArg(R1.scratchReg());
||||||| merged common ancestors
    pushArg(ImmGCPtr(&withScope));
    pushArg(R0);
    pushArg(R1.scratchReg());
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_POPVARENV() {
  prepareVMCall();
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
  pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  return callVM(EnterWithInfo);
||||||| merged common ancestors
    return callVM(EnterWithInfo);
=======
  using Fn = bool (*)(JSContext*, BaselineFrame*);
  return callVM<Fn, jit::PopVarEnv>();
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ENTERWITH() {
  // Pop "with" object to R0.
  frame.popRegsAndSync(1);

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LEAVEWITH() {
  // Call a stub to pop the with object from the environment chain.
  prepareVMCall();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_LEAVEWITH()
{
    // Call a stub to pop the with object from the environment chain.
    prepareVMCall();
=======
  // Call a stub to push the object onto the environment chain.
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
  pushArg(R0.scratchReg());
||||||| merged common ancestors
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
    pushArg(R0.scratchReg());
=======
  pushScriptGCThingArg(ScriptGCThingType::Scope, R1.scratchReg(),
                       R2.scratchReg());
  pushArg(R0);
  masm.loadBaselineFramePtr(BaselineFrameReg, R1.scratchReg());
  pushArg(R1.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  return callVM(LeaveWithInfo);
||||||| merged common ancestors
    return callVM(LeaveWithInfo);
=======
  using Fn =
      bool (*)(JSContext*, BaselineFrame*, HandleValue, Handle<WithScope*>);
  return callVM<Fn, jit::EnterWith>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*GetAndClearExceptionFn)(JSContext*, MutableHandleValue);
static const VMFunction GetAndClearExceptionInfo =
    FunctionInfo<GetAndClearExceptionFn>(GetAndClearException,
                                         "GetAndClearException");
||||||| merged common ancestors
typedef bool (*GetAndClearExceptionFn)(JSContext*, MutableHandleValue);
static const VMFunction GetAndClearExceptionInfo =
    FunctionInfo<GetAndClearExceptionFn>(GetAndClearException, "GetAndClearException");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_LEAVEWITH() {
  // Call a stub to pop the with object from the environment chain.
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_EXCEPTION() {
  prepareVMCall();

  if (!callVM(GetAndClearExceptionInfo)) {
    return false;
  }
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_EXCEPTION()
{
    prepareVMCall();

    if (!callVM(GetAndClearExceptionInfo)) {
        return false;
    }
=======
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
  pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.push(R0);
  return true;
||||||| merged common ancestors
    frame.push(R0);
    return true;
=======
  using Fn = bool (*)(JSContext*, BaselineFrame*);
  return callVM<Fn, jit::LeaveWith>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*OnDebuggerStatementFn)(JSContext*, BaselineFrame*,
                                      jsbytecode* pc, bool*);
static const VMFunction OnDebuggerStatementInfo =
    FunctionInfo<OnDebuggerStatementFn>(jit::OnDebuggerStatement,
                                        "OnDebuggerStatement");

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEBUGGER() {
  prepareVMCall();
  pushArg(ImmPtr(pc));

  frame.assertSyncedStack();
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
  pushArg(R0.scratchReg());

  if (!callVM(OnDebuggerStatementInfo)) {
    return false;
  }

  // If the stub returns |true|, return the frame's return value.
  Label done;
  masm.branchTest32(Assembler::Zero, ReturnReg, ReturnReg, &done);
  {
    masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
    masm.jump(&return_);
  }
  masm.bind(&done);
  return true;
}
||||||| merged common ancestors
typedef bool (*OnDebuggerStatementFn)(JSContext*, BaselineFrame*, jsbytecode* pc, bool*);
static const VMFunction OnDebuggerStatementInfo =
    FunctionInfo<OnDebuggerStatementFn>(jit::OnDebuggerStatement, "OnDebuggerStatement");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_EXCEPTION() {
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*DebugEpilogueFn)(JSContext*, BaselineFrame*, jsbytecode*);
static const VMFunction DebugEpilogueInfo = FunctionInfo<DebugEpilogueFn>(
    jit::DebugEpilogueOnBaselineReturn, "DebugEpilogueOnBaselineReturn");

template <typename Handler>
bool BaselineCodeGen<Handler>::emitReturn() {
  auto ifDebuggee = [this]() {
    // Move return value into the frame's rval slot.
    masm.storeValue(JSReturnOperand, frame.addressOfReturnValue());
    masm.or32(Imm32(BaselineFrame::HAS_RVAL), frame.addressOfFlags());
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_DEBUGGER()
{
    prepareVMCall();
    pushArg(ImmPtr(pc));
=======
  using Fn = bool (*)(JSContext*, MutableHandleValue);
  if (!callVM<Fn, GetAndClearException>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Load BaselineFrame pointer in R0.
    frame.syncStack(0);
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
||||||| merged common ancestors
    frame.assertSyncedStack();
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
    pushArg(R0.scratchReg());
=======
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    prepareVMCall();
    pushArg(ImmPtr(pc));
    pushArg(R0.scratchReg());
    if (!callVM(DebugEpilogueInfo)) {
      return false;
    }
||||||| merged common ancestors
    if (!callVM(OnDebuggerStatementInfo)) {
        return false;
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEBUGGER() {
  prepareVMCall();
  pushBytecodePCArg();
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Fix up the RetAddrEntry appended by callVM for on-stack recompilation.
    retAddrEntries_.back().setKind(RetAddrEntry::Kind::DebugEpilogue);
||||||| merged common ancestors
    // If the stub returns |true|, return the frame's return value.
    Label done;
    masm.branchTest32(Assembler::Zero, ReturnReg, ReturnReg, &done);
    {
        masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
        masm.jump(&return_);
    }
    masm.bind(&done);
    return true;
}
=======
  frame.assertSyncedStack();
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
  pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
    masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
    return true;
  };
  if (!emitDebugInstrumentation(ifDebuggee)) {
    return false;
  }
||||||| merged common ancestors
typedef bool (*DebugEpilogueFn)(JSContext*, BaselineFrame*, jsbytecode*);
static const VMFunction DebugEpilogueInfo =
    FunctionInfo<DebugEpilogueFn>(jit::DebugEpilogueOnBaselineReturn,
                                  "DebugEpilogueOnBaselineReturn");

bool
BaselineCompiler::emitReturn()
{
    if (compileDebugInstrumentation_) {
        // Move return value into the frame's rval slot.
        masm.storeValue(JSReturnOperand, frame.addressOfReturnValue());
        masm.or32(Imm32(BaselineFrame::HAS_RVAL), frame.addressOfFlags());

        // Load BaselineFrame pointer in R0.
        frame.syncStack(0);
        masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());

        prepareVMCall();
        pushArg(ImmPtr(pc));
        pushArg(R0.scratchReg());
        if (!callVM(DebugEpilogueInfo)) {
            return false;
        }

        // Fix up the fake ICEntry appended by callVM for on-stack recompilation.
        icEntries_.back().setFakeKind(ICEntry::Kind_DebugEpilogue);

        masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
    }
=======
  using Fn = bool (*)(JSContext*, BaselineFrame*, jsbytecode*, bool*);
  if (!callVM<Fn, jit::OnDebuggerStatement>()) {
    return false;
  }

  // If the stub returns |true|, return the frame's return value.
  Label done;
  masm.branchTest32(Assembler::Zero, ReturnReg, ReturnReg, &done);
  {
    masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
    masm.jump(&return_);
  }
  masm.bind(&done);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emitReturn() {
  auto ifDebuggee = [this]() {
    // Move return value into the frame's rval slot.
    masm.storeValue(JSReturnOperand, frame.addressOfReturnValue());
    masm.or32(Imm32(BaselineFrame::HAS_RVAL), frame.addressOfFlags());

    // Load BaselineFrame pointer in R0.
    frame.syncStack(0);
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());

    prepareVMCall();
    pushBytecodePCArg();
    pushArg(R0.scratchReg());

    using Fn = bool (*)(JSContext*, BaselineFrame*, jsbytecode*);
    if (!callVM<Fn, jit::DebugEpilogueOnBaselineReturn>()) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Only emit the jump if this JSOP_RETRVAL is not the last instruction.
  // Not needed for last instruction, because last instruction flows
  // into return label.
  if (pc + GetBytecodeLength(pc) < script->codeEnd()) {
    masm.jump(&return_);
  }
||||||| merged common ancestors
    // Only emit the jump if this JSOP_RETRVAL is not the last instruction.
    // Not needed for last instruction, because last instruction flows
    // into return label.
    if (pc + GetBytecodeLength(pc) < script->codeEnd()) {
        masm.jump(&return_);
    }
=======
    // Fix up the RetAddrEntry appended by callVM for on-stack recompilation.
    handler.markLastRetAddrEntryKind(RetAddrEntry::Kind::DebugEpilogue);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
    masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
    return true;
  };
  if (!emitDebugInstrumentation(ifDebuggee)) {
    return false;
  }

  // Only emit the jump if this JSOP_RETRVAL is not the last instruction.
  // Not needed for last instruction, because last instruction flows
  // into return label.
  if (!handler.isDefinitelyLastOp()) {
    masm.jump(&return_);
  }

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_RETURN() {
  MOZ_ASSERT(frame.stackDepth() == 1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_RETURN()
{
    MOZ_ASSERT(frame.stackDepth() == 1);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_RETURN() {
  frame.assertStackDepth(1);
>>>>>>> upstream-releases

  frame.popValue(JSReturnOperand);
  return emitReturn();
}

template <typename Handler>
void BaselineCodeGen<Handler>::emitLoadReturnValue(ValueOperand val) {
  Label done, noRval;
  masm.branchTest32(Assembler::Zero, frame.addressOfFlags(),
                    Imm32(BaselineFrame::HAS_RVAL), &noRval);
  masm.loadValue(frame.addressOfReturnValue(), val);
  masm.jump(&done);

  masm.bind(&noRval);
  masm.moveValue(UndefinedValue(), val);

  masm.bind(&done);
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_RETRVAL() {
  MOZ_ASSERT(frame.stackDepth() == 0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_RETRVAL()
{
    MOZ_ASSERT(frame.stackDepth() == 0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_RETRVAL() {
  frame.assertStackDepth(0);
>>>>>>> upstream-releases

  masm.moveValue(UndefinedValue(), JSReturnOperand);

<<<<<<< HEAD
  if (!script->noScriptRval()) {
    // Return the value in the return value slot, if any.
    Label done;
    Address flags = frame.addressOfFlags();
    masm.branchTest32(Assembler::Zero, flags, Imm32(BaselineFrame::HAS_RVAL),
                      &done);
    masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
    masm.bind(&done);
  }
||||||| merged common ancestors
    if (!script->noScriptRval()) {
        // Return the value in the return value slot, if any.
        Label done;
        Address flags = frame.addressOfFlags();
        masm.branchTest32(Assembler::Zero, flags, Imm32(BaselineFrame::HAS_RVAL), &done);
        masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
        masm.bind(&done);
    }
=======
  if (!handler.maybeScript() || !handler.maybeScript()->noScriptRval()) {
    // Return the value in the return value slot, if any.
    Label done;
    Address flags = frame.addressOfFlags();
    masm.branchTest32(Assembler::Zero, flags, Imm32(BaselineFrame::HAS_RVAL),
                      &done);
    masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
    masm.bind(&done);
  }
>>>>>>> upstream-releases

  return emitReturn();
}

<<<<<<< HEAD
typedef bool (*ToIdFn)(JSContext*, HandleValue, MutableHandleValue);
static const VMFunction ToIdInfo =
    FunctionInfo<ToIdFn>(js::ToIdOperation, "ToIdOperation");
||||||| merged common ancestors
typedef bool (*ToIdFn)(JSContext*, HandleValue, MutableHandleValue);
static const VMFunction ToIdInfo = FunctionInfo<ToIdFn>(js::ToIdOperation, "ToIdOperation");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TOID() {
  // Load index in R0, but keep values on the stack for the decompiler.
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-1), R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TOID() {
  // Load index in R0, but keep values on the stack for the decompiler.
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_TOID()
{
    // Load index in R0, but keep values on the stack for the decompiler.
    frame.syncStack(0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
=======
  // No-op if the index is trivally convertable to an id.
  Label done;
  masm.branchTestInt32(Assembler::Equal, R0, &done);
  masm.branchTestString(Assembler::Equal, R0, &done);
  masm.branchTestSymbol(Assembler::Equal, R0, &done);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // No-op if the index is trivally convertable to an id.
  Label done;
  masm.branchTestInt32(Assembler::Equal, R0, &done);
  masm.branchTestString(Assembler::Equal, R0, &done);
  masm.branchTestSymbol(Assembler::Equal, R0, &done);
||||||| merged common ancestors
    // No-op if the index is trivally convertable to an id.
    Label done;
    masm.branchTestInt32(Assembler::Equal, R0, &done);
    masm.branchTestString(Assembler::Equal, R0, &done);
    masm.branchTestSymbol(Assembler::Equal, R0, &done);
=======
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    prepareVMCall();
=======
  pushArg(R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(R0);
||||||| merged common ancestors
    pushArg(R0);
=======
  using Fn = bool (*)(JSContext*, HandleValue, MutableHandleValue);
  if (!callVM<Fn, js::ToIdOperation>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(ToIdInfo)) {
    return false;
  }

  masm.bind(&done);
  frame.pop();  // Pop index.
  frame.push(R0);
  return true;
||||||| merged common ancestors
    if (!callVM(ToIdInfo)) {
        return false;
    }

    masm.bind(&done);
    frame.pop(); // Pop index.
    frame.push(R0);
    return true;
=======
  masm.bind(&done);
  frame.pop();  // Pop index.
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef JSObject* (*ToAsyncFn)(JSContext*, HandleFunction);
static const VMFunction ToAsyncInfo =
    FunctionInfo<ToAsyncFn>(js::WrapAsyncFunction, "ToAsync");
||||||| merged common ancestors
typedef JSObject* (*ToAsyncFn)(JSContext*, HandleFunction);
static const VMFunction ToAsyncInfo = FunctionInfo<ToAsyncFn>(js::WrapAsyncFunction, "ToAsync");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TOASYNCITER() {
  frame.syncStack(0);
  masm.unboxObject(frame.addressOfStackValue(-2), R0.scratchReg());
  masm.loadValue(frame.addressOfStackValue(-1), R1);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TOASYNC() {
  frame.syncStack(0);
  masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), R0.scratchReg());
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_TOASYNC()
{
    frame.syncStack(0);
    masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), R0.scratchReg());
=======
  prepareVMCall();
  pushArg(R1);
  pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(R0.scratchReg());

  if (!callVM(ToAsyncInfo)) {
    return false;
  }
||||||| merged common ancestors
    prepareVMCall();
    pushArg(R0.scratchReg());

    if (!callVM(ToAsyncInfo)) {
        return false;
    }
=======
  using Fn = JSObject* (*)(JSContext*, HandleObject, HandleValue);
  if (!callVM<Fn, js::CreateAsyncFromSyncIterator>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.pop();
  frame.push(R0);
  return true;
||||||| merged common ancestors
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.pop();
    frame.push(R0);
    return true;
=======
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.popn(2);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TRYSKIPAWAIT() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-1), R0);

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TOASYNCGEN() {
  frame.syncStack(0);
  masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), R0.scratchReg());
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_TOASYNCGEN()
{
    frame.syncStack(0);
    masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), R0.scratchReg());
=======
  prepareVMCall();
  pushArg(R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(R0.scratchReg());
||||||| merged common ancestors
    prepareVMCall();
    pushArg(R0.scratchReg());
=======
  using Fn = bool (*)(JSContext*, HandleValue, MutableHandleValue);
  if (!callVM<Fn, jit::TrySkipAwait>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(ToAsyncGenInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(ToAsyncGenInfo)) {
        return false;
    }
=======
  Label cannotSkip, done;
  masm.branchTestMagicValue(Assembler::Equal, R0, JS_CANNOT_SKIP_AWAIT,
                            &cannotSkip);
  masm.moveValue(BooleanValue(true), R1);
  masm.jump(&done);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.pop();
  frame.push(R0);
  return true;
}
||||||| merged common ancestors
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.pop();
    frame.push(R0);
    return true;
}
=======
  masm.bind(&cannotSkip);
  masm.loadValue(frame.addressOfStackValue(-1), R0);
  masm.moveValue(BooleanValue(false), R1);
>>>>>>> upstream-releases

  masm.bind(&done);

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TOASYNCITER() {
  frame.syncStack(0);
  masm.unboxObject(frame.addressOfStackValue(frame.peek(-2)), R0.scratchReg());
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_TOASYNCITER()
{
    frame.syncStack(0);
    masm.unboxObject(frame.addressOfStackValue(frame.peek(-2)), R0.scratchReg());
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R1);
=======
  frame.pop();
  frame.push(R0);
  frame.push(R1);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(R1);
  pushArg(R0.scratchReg());
||||||| merged common ancestors
    prepareVMCall();
    pushArg(R1);
    pushArg(R0.scratchReg());
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ASYNCAWAIT() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-2), R1);
  masm.unboxObject(frame.addressOfStackValue(-1), R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(ToAsyncIterInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(ToAsyncIterInfo)) {
        return false;
    }
=======
  prepareVMCall();
  pushArg(R1);
  pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.popn(2);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.popn(2);
    frame.push(R0);
    return true;
=======
  using Fn = JSObject* (*)(JSContext*, Handle<AsyncFunctionGeneratorObject*>,
                           HandleValue);
  if (!callVM<Fn, js::AsyncFunctionAwait>()) {
    return false;
  }

  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.popn(2);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*TrySkipAwaitFn)(JSContext*, HandleValue, MutableHandleValue);
static const VMFunction TrySkipAwaitInfo =
    FunctionInfo<TrySkipAwaitFn>(jit::TrySkipAwait, "TrySkipAwait");
||||||| merged common ancestors
typedef bool (*TrySkipAwaitFn)(JSContext*, HandleValue, MutableHandleValue);
static const VMFunction TrySkipAwaitInfo = FunctionInfo<TrySkipAwaitFn>(jit::TrySkipAwait, "TrySkipAwait");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ASYNCRESOLVE() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-2), R1);
  masm.unboxObject(frame.addressOfStackValue(-1), R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TRYSKIPAWAIT() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_TRYSKIPAWAIT()
{
    frame.syncStack(0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
=======
  prepareVMCall();
  pushUint8BytecodeOperandArg(R2.scratchReg());
  pushArg(R1);
  pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(R0);
||||||| merged common ancestors
    prepareVMCall();
    pushArg(R0);
=======
  using Fn = JSObject* (*)(JSContext*, Handle<AsyncFunctionGeneratorObject*>,
                           HandleValue, AsyncFunctionResolveKind);
  if (!callVM<Fn, js::AsyncFunctionResolve>()) {
    return false;
  }

  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.popn(2);
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(TrySkipAwaitInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(TrySkipAwaitInfo)) {
        return false;
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKOBJCOERCIBLE() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(-1), R0);

  Label fail, done;
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label cannotSkip, done;
  masm.branchTestMagicValue(Assembler::Equal, R0, JS_CANNOT_SKIP_AWAIT,
                            &cannotSkip);
  masm.moveValue(BooleanValue(true), R1);
  masm.jump(&done);
||||||| merged common ancestors
    Label cannotSkip, done;
    masm.branchTestMagicValue(Assembler::Equal, R0, JS_CANNOT_SKIP_AWAIT, &cannotSkip);
    masm.moveValue(BooleanValue(true), R1);
    masm.jump(&done);
=======
  masm.branchTestUndefined(Assembler::Equal, R0, &fail);
  masm.branchTestNull(Assembler::NotEqual, R0, &done);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&cannotSkip);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
  masm.moveValue(BooleanValue(false), R1);
||||||| merged common ancestors
    masm.bind(&cannotSkip);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
    masm.moveValue(BooleanValue(false), R1);
=======
  masm.bind(&fail);
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&done);
||||||| merged common ancestors
    masm.bind(&done);
=======
  pushArg(R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.pop();
  frame.push(R0);
  frame.push(R1);
  return true;
||||||| merged common ancestors
    frame.pop();
    frame.push(R0);
    frame.push(R1);
    return true;
=======
  using Fn = bool (*)(JSContext*, HandleValue);
  if (!callVM<Fn, ThrowObjectCoercible>()) {
    return false;
  }

  masm.bind(&done);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*ThrowObjectCoercibleFn)(JSContext*, HandleValue);
static const VMFunction ThrowObjectCoercibleInfo =
    FunctionInfo<ThrowObjectCoercibleFn>(ThrowObjectCoercible,
                                         "ThrowObjectCoercible");
||||||| merged common ancestors
typedef bool (*ThrowObjectCoercibleFn)(JSContext*, HandleValue);
static const VMFunction ThrowObjectCoercibleInfo =
    FunctionInfo<ThrowObjectCoercibleFn>(ThrowObjectCoercible, "ThrowObjectCoercible");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TOSTRING() {
  // Keep top stack value in R0.
  frame.popRegsAndSync(1);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKOBJCOERCIBLE() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CHECKOBJCOERCIBLE()
{
    frame.syncStack(0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
=======
  // Inline path for string.
  Label done;
  masm.branchTestString(Assembler::Equal, R0, &done);

  prepareVMCall();

  pushArg(R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label fail, done;
||||||| merged common ancestors
    Label fail, done;
=======
  // Call ToStringSlow which doesn't handle string inputs.
  using Fn = JSString* (*)(JSContext*, HandleValue);
  if (!callVM<Fn, ToStringSlow<CanGC>>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.branchTestUndefined(Assembler::Equal, R0, &fail);
  masm.branchTestNull(Assembler::NotEqual, R0, &done);
||||||| merged common ancestors
    masm.branchTestUndefined(Assembler::Equal, R0, &fail);
    masm.branchTestNull(Assembler::NotEqual, R0, &done);
=======
  masm.tagValue(JSVAL_TYPE_STRING, ReturnReg, R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&fail);
  prepareVMCall();
||||||| merged common ancestors
    masm.bind(&fail);
    prepareVMCall();
=======
  masm.bind(&done);
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(R0);
||||||| merged common ancestors
    pushArg(R0);
=======
static constexpr uint32_t TableSwitchOpLowOffset = 1 * JUMP_OFFSET_LEN;
static constexpr uint32_t TableSwitchOpHighOffset = 2 * JUMP_OFFSET_LEN;
static constexpr uint32_t TableSwitchOpFirstResumeIndexOffset =
    3 * JUMP_OFFSET_LEN;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(ThrowObjectCoercibleInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(ThrowObjectCoercibleInfo)) {
        return false;
    }
=======
template <>
void BaselineCompilerCodeGen::emitGetTableSwitchIndex(ValueOperand val,
                                                      Register dest,
                                                      Register scratch1,
                                                      Register scratch2) {
  jsbytecode* pc = handler.pc();
  jsbytecode* defaultpc = pc + GET_JUMP_OFFSET(pc);
  Label* defaultLabel = handler.labelOf(defaultpc);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&done);
  return true;
||||||| merged common ancestors
    masm.bind(&done);
    return true;
=======
  int32_t low = GET_JUMP_OFFSET(pc + TableSwitchOpLowOffset);
  int32_t high = GET_JUMP_OFFSET(pc + TableSwitchOpHighOffset);
  int32_t length = high - low + 1;

  // Jump to the 'default' pc if not int32 (tableswitch is only used when
  // all cases are int32).
  masm.branchTestInt32(Assembler::NotEqual, val, defaultLabel);
  masm.unboxInt32(val, dest);

  // Subtract 'low'. Bounds check.
  if (low != 0) {
    masm.sub32(Imm32(low), dest);
  }
  masm.branch32(Assembler::AboveOrEqual, dest, Imm32(length), defaultLabel);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef JSString* (*ToStringFn)(JSContext*, HandleValue);
static const VMFunction ToStringInfo =
    FunctionInfo<ToStringFn>(ToStringSlow, "ToStringSlow");
||||||| merged common ancestors
typedef JSString* (*ToStringFn)(JSContext*, HandleValue);
static const VMFunction ToStringInfo = FunctionInfo<ToStringFn>(ToStringSlow, "ToStringSlow");
=======
template <>
void BaselineInterpreterCodeGen::emitGetTableSwitchIndex(ValueOperand val,
                                                         Register dest,
                                                         Register scratch1,
                                                         Register scratch2) {
  // Jump to the 'default' pc if not int32 (tableswitch is only used when
  // all cases are int32).
  Label done, jumpToDefault;
  masm.branchTestInt32(Assembler::NotEqual, val, &jumpToDefault);
  masm.unboxInt32(val, dest);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TOSTRING() {
  // Keep top stack value in R0.
  frame.popRegsAndSync(1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_TOSTRING()
{
    // Keep top stack value in R0.
    frame.popRegsAndSync(1);
=======
  Register pcReg = LoadBytecodePC(masm, scratch1);
  Address lowAddr(pcReg, sizeof(jsbytecode) + TableSwitchOpLowOffset);
  Address highAddr(pcReg, sizeof(jsbytecode) + TableSwitchOpHighOffset);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Inline path for string.
  Label done;
  masm.branchTestString(Assembler::Equal, R0, &done);
||||||| merged common ancestors
    // Inline path for string.
    Label done;
    masm.branchTestString(Assembler::Equal, R0, &done);
=======
  // Jump to default if val > high.
  masm.branch32(Assembler::LessThan, highAddr, dest, &jumpToDefault);
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    prepareVMCall();
=======
  // Jump to default if val < low.
  masm.load32(lowAddr, scratch2);
  masm.branch32(Assembler::GreaterThan, scratch2, dest, &jumpToDefault);
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushArg(R0);
||||||| merged common ancestors
    pushArg(R0);
=======
  // index := val - low.
  masm.sub32(scratch2, dest);
  masm.jump(&done);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call ToStringSlow which doesn't handle string inputs.
  if (!callVM(ToStringInfo)) {
    return false;
  }
||||||| merged common ancestors
    // Call ToStringSlow which doesn't handle string inputs.
    if (!callVM(ToStringInfo)) {
        return false;
    }
=======
  masm.bind(&jumpToDefault);
  emitJump();
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.tagValue(JSVAL_TYPE_STRING, ReturnReg, R0);
||||||| merged common ancestors
    masm.tagValue(JSVAL_TYPE_STRING, ReturnReg, R0);
=======
  masm.bind(&done);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&done);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    masm.bind(&done);
    frame.push(R0);
    return true;
=======
template <>
void BaselineCompilerCodeGen::emitTableSwitchJump(Register key,
                                                  Register scratch1,
                                                  Register scratch2) {
  // Jump to resumeEntries[firstResumeIndex + key].

  // Note: BytecodeEmitter::allocateResumeIndex static_asserts
  // |firstResumeIndex * sizeof(uintptr_t)| fits in int32_t.
  uint32_t firstResumeIndex =
      GET_RESUMEINDEX(handler.pc() + TableSwitchOpFirstResumeIndexOffset);
  LoadBaselineScriptResumeEntries(masm, handler.script(), scratch1, scratch2);
  masm.loadPtr(BaseIndex(scratch1, key, ScaleFromElemWidth(sizeof(uintptr_t)),
                         firstResumeIndex * sizeof(uintptr_t)),
               scratch1);
  masm.jump(scratch1);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <>
void BaselineCompilerCodeGen::emitGetTableSwitchIndex(ValueOperand val,
                                                      Register dest) {
  jsbytecode* defaultpc = pc + GET_JUMP_OFFSET(pc);
  Label* defaultLabel = handler.labelOf(defaultpc);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_TABLESWITCH()
{
    frame.popRegsAndSync(1);
=======
template <>
void BaselineInterpreterCodeGen::emitTableSwitchJump(Register key,
                                                     Register scratch1,
                                                     Register scratch2) {
  // Load the op's firstResumeIndex in scratch1.
  LoadUint24Operand(masm, TableSwitchOpFirstResumeIndexOffset, scratch1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  int32_t low = GET_JUMP_OFFSET(pc + 1 * JUMP_OFFSET_LEN);
  int32_t high = GET_JUMP_OFFSET(pc + 2 * JUMP_OFFSET_LEN);
  int32_t length = high - low + 1;
||||||| merged common ancestors
    // Call IC.
    ICTableSwitch::Compiler compiler(cx, pc);
    return emitOpIC(compiler.getStub(&stubSpace_));
}
=======
  masm.add32(key, scratch1);
  jumpToResumeEntry(scratch1, key, scratch2);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Jump to the 'default' pc if not int32 (tableswitch is only used when
  // all cases are int32).
  masm.branchTestInt32(Assembler::NotEqual, val, defaultLabel);
  masm.unboxInt32(val, dest);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_ITER()
{
    frame.popRegsAndSync(1);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TABLESWITCH() {
  frame.popRegsAndSync(1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Subtract 'low'. Bounds check.
  if (low != 0) {
    masm.sub32(Imm32(low), dest);
  }
  masm.branch32(Assembler::AboveOrEqual, dest, Imm32(length), defaultLabel);
}
||||||| merged common ancestors
    ICGetIterator_Fallback::Compiler compiler(cx);
    if (!emitOpIC(compiler.getStub(&stubSpace_))) {
        return false;
    }
=======
  Register key = R0.scratchReg();
  Register scratch1 = R1.scratchReg();
  Register scratch2 = R2.scratchReg();
>>>>>>> upstream-releases

<<<<<<< HEAD
template <>
void BaselineInterpreterCodeGen::emitGetTableSwitchIndex(ValueOperand val,
                                                         Register dest) {
  MOZ_CRASH("NYI: interpreter emitTableSwitchJumpTableIndex");
||||||| merged common ancestors
    frame.push(R0);
    return true;
=======
  // Call a stub to convert R0 from double to int32 if needed.
  // Note: this stub may clobber scratch1.
  masm.call(cx->runtime()->jitRuntime()->getDoubleToInt32ValueStub());

  // Load the index in the jump table in |key|, or branch to default pc if not
  // int32 or out-of-range.
  emitGetTableSwitchIndex(R0, key, scratch1, scratch2);

  // Jump to the target pc.
  emitTableSwitchJump(key, scratch1, scratch2);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_TABLESWITCH() {
  frame.popRegsAndSync(1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_MOREITER()
{
    frame.syncStack(0);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ITER() {
  frame.popRegsAndSync(1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t firstResumeIndex = GET_RESUMEINDEX(pc + 3 * JUMP_OFFSET_LEN);
||||||| merged common ancestors
    ICIteratorMore_Fallback::Compiler compiler(cx);
    if (!emitOpIC(compiler.getStub(&stubSpace_))) {
        return false;
    }
=======
  if (!emitNextIC()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register key = R0.scratchReg();
  Register scratch1 = R1.scratchReg();
  Register scratch2 = R2.scratchReg();
||||||| merged common ancestors
    frame.push(R0);
    return true;
}
=======
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call a stub to convert R0 from double to int32 if needed.
  // Note: this stub may clobber scratch1.
  masm.call(cx->runtime()->jitRuntime()->getDoubleToInt32ValueStub());
||||||| merged common ancestors
bool
BaselineCompiler::emitIsMagicValue()
{
    frame.syncStack(0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_MOREITER() {
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load the index in the jump table in |key|, or branch to default pc if not
  // int32 or out-of-range.
  emitGetTableSwitchIndex(R0, key);
||||||| merged common ancestors
    Label isMagic, done;
    masm.branchTestMagic(Assembler::Equal, frame.addressOfStackValue(frame.peek(-1)),
                         &isMagic);
    masm.moveValue(BooleanValue(false), R0);
    masm.jump(&done);
=======
  masm.unboxObject(frame.addressOfStackValue(-1), R1.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Jump to resumeEntries[firstResumeIndex + key].
  //
  // Note: BytecodeEmitter::allocateResumeIndex static_asserts
  // |firstResumeIndex * sizeof(uintptr_t)| fits in int32_t.
||||||| merged common ancestors
    masm.bind(&isMagic);
    masm.moveValue(BooleanValue(true), R0);
=======
  masm.iteratorMore(R1.scratchReg(), R0, R2.scratchReg());
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  LoadBaselineScriptResumeEntries(masm, script, scratch1, scratch2);
  masm.loadPtr(BaseIndex(scratch1, key, ScaleFromElemWidth(sizeof(uintptr_t)),
                         firstResumeIndex * sizeof(uintptr_t)),
               scratch1);
  masm.jump(scratch1);
||||||| merged common ancestors
    masm.bind(&done);
    frame.push(R0, JSVAL_TYPE_BOOLEAN);
    return true;
}
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitIsMagicValue() {
  frame.syncStack(0);

  Label isMagic, done;
  masm.branchTestMagic(Assembler::Equal, frame.addressOfStackValue(-1),
                       &isMagic);
  masm.moveValue(BooleanValue(false), R0);
  masm.jump(&done);

  masm.bind(&isMagic);
  masm.moveValue(BooleanValue(true), R0);

  masm.bind(&done);
  frame.push(R0, JSVAL_TYPE_BOOLEAN);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_ISNOITER()
{
    return emitIsMagicValue();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ISNOITER() {
  return emitIsMagicValue();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ITER() {
  frame.popRegsAndSync(1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_ENDITER()
{
    if (!emit_JSOP_JUMPTARGET()) {
        return false;
    }
    frame.popRegsAndSync(1);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ENDITER() {
  frame.popRegsAndSync(1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!emitNextIC()) {
    return false;
  }

  frame.push(R0);
  return true;
||||||| merged common ancestors
    ICIteratorClose_Fallback::Compiler compiler(cx);
    return emitOpIC(compiler.getStub(&stubSpace_));
=======
  AllocatableGeneralRegisterSet regs(GeneralRegisterSet::All());
  regs.take(BaselineFrameReg);
  if (HasInterpreterPCReg()) {
    regs.take(InterpreterPCReg);
  }

  Register obj = R0.scratchReg();
  regs.take(obj);
  masm.unboxObject(R0, obj);

  Register temp1 = regs.takeAny();
  Register temp2 = regs.takeAny();
  Register temp3 = regs.takeAny();
  masm.iteratorClose(obj, temp1, temp2, temp3);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_MOREITER() {
  frame.syncStack(0);
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);

  if (!emitNextIC()) {
    return false;
  }

  frame.push(R0);
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_ISGENCLOSING()
{
    return emitIsMagicValue();
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ISGENCLOSING() {
  return emitIsMagicValue();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emitIsMagicValue() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GETRVAL()
{
    frame.syncStack(0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETRVAL() {
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label isMagic, done;
  masm.branchTestMagic(Assembler::Equal,
                       frame.addressOfStackValue(frame.peek(-1)), &isMagic);
  masm.moveValue(BooleanValue(false), R0);
  masm.jump(&done);
||||||| merged common ancestors
    emitLoadReturnValue(R0);
=======
  emitLoadReturnValue(R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&isMagic);
  masm.moveValue(BooleanValue(true), R0);

  masm.bind(&done);
  frame.push(R0, JSVAL_TYPE_BOOLEAN);
  return true;
||||||| merged common ancestors
    frame.push(R0);
    return true;
=======
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ISNOITER() {
  return emitIsMagicValue();
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SETRVAL()
{
    // Store to the frame's return value slot.
    storeValue(frame.peek(-1), frame.addressOfReturnValue(), R2);
    masm.or32(Imm32(BaselineFrame::HAS_RVAL), frame.addressOfFlags());
    frame.pop();
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETRVAL() {
  // Store to the frame's return value slot.
  frame.storeStackValue(-1, frame.addressOfReturnValue(), R2);
  masm.or32(Imm32(BaselineFrame::HAS_RVAL), frame.addressOfFlags());
  frame.pop();
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ENDITER() {
  if (!emit_JSOP_JUMPTARGET()) {
    return false;
  }
  frame.popRegsAndSync(1);

  if (!emitNextIC()) {
    return false;
  }

  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CALLEE()
{
    MOZ_ASSERT(function());
    frame.syncStack(0);
    masm.loadFunctionFromCalleeToken(frame.addressOfCalleeToken(), R0.scratchReg());
    masm.tagValue(JSVAL_TYPE_OBJECT, R0.scratchReg(), R0);
    frame.push(R0);
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CALLEE() {
  MOZ_ASSERT_IF(handler.maybeScript(),
                handler.maybeScript()->functionNonDelazifying());
  frame.syncStack(0);
  masm.loadFunctionFromCalleeToken(frame.addressOfCalleeToken(),
                                   R0.scratchReg());
  masm.tagValue(JSVAL_TYPE_OBJECT, R0.scratchReg(), R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ISGENCLOSING() {
  return emitIsMagicValue();
}
||||||| merged common ancestors
void
BaselineCompiler::getThisEnvironmentCallee(Register reg)
{
    // Directly load callee from frame if we have a HomeObject
    if (function() && function()->allowSuperProperty()) {
        masm.loadFunctionFromCalleeToken(frame.addressOfCalleeToken(), reg);
        return;
    }
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_ENVCALLEE() {
  frame.syncStack(0);
  uint8_t numHops = GET_UINT8(handler.pc());
  Register scratch = R0.scratchReg();
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GETRVAL() {
  frame.syncStack(0);
||||||| merged common ancestors
    // Locate environment chain
    masm.loadPtr(frame.addressOfEnvironmentChain(), reg);
=======
  masm.loadPtr(frame.addressOfEnvironmentChain(), scratch);
  for (unsigned i = 0; i < numHops; i++) {
    Address nextAddr(scratch,
                     EnvironmentObject::offsetOfEnclosingEnvironment());
    masm.unboxObject(nextAddr, scratch);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  emitLoadReturnValue(R0);
||||||| merged common ancestors
    // Walk environment chain until first non-arrow CallObject
    for (ScopeIter si(script->innermostScope(pc)); si; si++) {
=======
  masm.loadValue(Address(scratch, CallObject::offsetOfCallee()), R0);
  frame.push(R0);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.push(R0);
  return true;
}
||||||| merged common ancestors
        // Find first non-arrow FunctionScope
        if (si.hasSyntacticEnvironment() && si.scope()->is<FunctionScope>()) {
            JSFunction* fn = si.scope()->as<FunctionScope>().canonicalFunction();
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_ENVCALLEE() {
  Register numHops = R0.scratchReg();
  LoadUint8Operand(masm, numHops);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SETRVAL() {
  // Store to the frame's return value slot.
  storeValue(frame.peek(-1), frame.addressOfReturnValue(), R2);
  masm.or32(Imm32(BaselineFrame::HAS_RVAL), frame.addressOfFlags());
  frame.pop();
  return true;
}
||||||| merged common ancestors
            if (!fn->isArrow()) {
                break;
            }
        }
=======
  Register env = R1.scratchReg();
  masm.loadPtr(frame.addressOfEnvironmentChain(), env);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CALLEE() {
  MOZ_ASSERT(function());
  frame.syncStack(0);
  masm.loadFunctionFromCalleeToken(frame.addressOfCalleeToken(),
                                   R0.scratchReg());
  masm.tagValue(JSVAL_TYPE_OBJECT, R0.scratchReg(), R0);
  frame.push(R0);
  return true;
}
||||||| merged common ancestors
        // Traverse environment chain
        if (si.scope()->hasEnvironment()) {
            Address nextAddr(reg, EnvironmentObject::offsetOfEnclosingEnvironment());
            masm.unboxObject(nextAddr, reg);
        }
    }
=======
  // Skip numHops environment objects.
  Label top, done;
  masm.bind(&top);
  masm.branchTest32(Assembler::Zero, numHops, numHops, &done);
  {
    Address nextAddr(env, EnvironmentObject::offsetOfEnclosingEnvironment());
    masm.unboxObject(nextAddr, env);
    masm.sub32(Imm32(1), numHops);
    masm.jump(&top);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ENVCALLEE() {
  frame.syncStack(0);
  uint8_t numHops = GET_UINT8(pc);
  Register scratch = R0.scratchReg();

  masm.loadPtr(frame.addressOfEnvironmentChain(), scratch);
  for (unsigned i = 0; i < numHops; i++) {
    Address nextAddr(scratch,
                     EnvironmentObject::offsetOfEnclosingEnvironment());
    masm.unboxObject(nextAddr, scratch);
  }

  masm.loadValue(Address(scratch, CallObject::offsetOfCallee()), R0);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    // Load callee
    masm.unboxObject(Address(reg, CallObject::offsetOfCallee()), reg);
=======
  masm.bind(&done);
  masm.pushValue(Address(env, CallObject::offsetOfCallee()));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef JSObject* (*HomeObjectSuperBaseFn)(JSContext*, HandleObject);
static const VMFunction HomeObjectSuperBaseInfo =
    FunctionInfo<HomeObjectSuperBaseFn>(HomeObjectSuperBase,
                                        "HomeObjectSuperBase");
||||||| merged common ancestors
typedef JSObject* (*HomeObjectSuperBaseFn)(JSContext*, HandleObject);
static const VMFunction HomeObjectSuperBaseInfo =
    FunctionInfo<HomeObjectSuperBaseFn>(HomeObjectSuperBase, "HomeObjectSuperBase");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SUPERBASE() {
  frame.popRegsAndSync(1);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SUPERBASE() {
  frame.popRegsAndSync(1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SUPERBASE()
{
    frame.syncStack(0);
=======
  Register scratch = R0.scratchReg();
  Register proto = R1.scratchReg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register scratch = R0.scratchReg();
  Register proto = R1.scratchReg();
||||||| merged common ancestors
    Register scratch = R0.scratchReg();
    Register proto = R1.scratchReg();
=======
  // Unbox callee.
  masm.unboxObject(R0, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Unbox callee.
  masm.unboxObject(R0, scratch);

  // Load [[HomeObject]]
  Address homeObjAddr(scratch,
                      FunctionExtended::offsetOfMethodHomeObjectSlot());
||||||| merged common ancestors
    // Lookup callee object of environment containing [[ThisValue]]
    getThisEnvironmentCallee(scratch);

    // Load [[HomeObject]]
    Address homeObjAddr(scratch, FunctionExtended::offsetOfMethodHomeObjectSlot());
=======
  // Load [[HomeObject]]
  Address homeObjAddr(scratch,
                      FunctionExtended::offsetOfMethodHomeObjectSlot());
>>>>>>> upstream-releases
#ifdef DEBUG
  Label isObject;
  masm.branchTestObject(Assembler::Equal, homeObjAddr, &isObject);
  masm.assumeUnreachable("[[HomeObject]] must be Object");
  masm.bind(&isObject);
#endif
<<<<<<< HEAD
  masm.unboxObject(homeObjAddr, scratch);

  // Load prototype from [[HomeObject]]
  masm.loadObjProto(scratch, proto);
||||||| merged common ancestors
    masm.unboxObject(homeObjAddr, scratch);

    // Load prototype from [[HomeObject]]
    masm.loadObjProto(scratch, proto);
=======
  masm.unboxObject(homeObjAddr, scratch);

  // Load prototype from [[HomeObject]]
  masm.loadObjProto(scratch, proto);

  Label hasProto;
  MOZ_ASSERT(uintptr_t(TaggedProto::LazyProto) == 1);
  masm.branchPtr(Assembler::Above, proto, ImmWord(1), &hasProto);

  // Use VMCall for missing or lazy proto
  prepareVMCall();
  pushArg(scratch);  // [[HomeObject]]
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label hasProto;
  MOZ_ASSERT(uintptr_t(TaggedProto::LazyProto) == 1);
  masm.branchPtr(Assembler::Above, proto, ImmWord(1), &hasProto);
||||||| merged common ancestors
    Label hasProto;
    MOZ_ASSERT(uintptr_t(TaggedProto::LazyProto) == 1);
    masm.branchPtr(Assembler::Above, proto, ImmWord(1), &hasProto);
=======
  using Fn = JSObject* (*)(JSContext*, HandleObject);
  if (!callVM<Fn, HomeObjectSuperBase>()) {
    return false;
  }
  masm.movePtr(ReturnReg, proto);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Use VMCall for missing or lazy proto
  prepareVMCall();
  pushArg(scratch);  // [[HomeObject]]
  if (!callVM(HomeObjectSuperBaseInfo)) {
    return false;
  }
  masm.movePtr(ReturnReg, proto);

  // Box prototype and return
  masm.bind(&hasProto);
  masm.tagValue(JSVAL_TYPE_OBJECT, proto, R1);
  frame.push(R1);
  return true;
||||||| merged common ancestors
    // Use VMCall for missing or lazy proto
    prepareVMCall();
    pushArg(scratch);  // [[HomeObject]]
    if (!callVM(HomeObjectSuperBaseInfo)) {
        return false;
    }
    masm.movePtr(ReturnReg, proto);

    // Box prototype and return
    masm.bind(&hasProto);
    masm.tagValue(JSVAL_TYPE_OBJECT, proto, R1);
    frame.push(R1);
    return true;
=======
  // Box prototype and return
  masm.bind(&hasProto);
  masm.tagValue(JSVAL_TYPE_OBJECT, proto, R1);
  frame.push(R1);
  return true;
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SUPERFUN() {
  frame.popRegsAndSync(1);

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_SUPERFUN() {
  frame.popRegsAndSync(1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_SUPERFUN()
{
    frame.syncStack(0);
=======
  Register callee = R0.scratchReg();
  Register proto = R1.scratchReg();
  Register scratch = R2.scratchReg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register callee = R0.scratchReg();
  Register proto = R1.scratchReg();
  Register scratch = R2.scratchReg();
||||||| merged common ancestors
    Register callee = R0.scratchReg();
    Register proto = R1.scratchReg();
    Register scratch = R2.scratchReg();
=======
  // Unbox callee.
  masm.unboxObject(R0, callee);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Unbox callee.
  masm.unboxObject(R0, callee);
||||||| merged common ancestors
    // Lookup callee object of environment containing [[ThisValue]]
    getThisEnvironmentCallee(callee);
=======
  // Load prototype of callee
  masm.loadObjProto(callee, proto);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load prototype of callee
  masm.loadObjProto(callee, proto);
||||||| merged common ancestors
    // Load prototype of callee
    masm.loadObjProto(callee, proto);
=======
  // Use VMCall for missing or lazy proto
  Label needVMCall;
  MOZ_ASSERT(uintptr_t(TaggedProto::LazyProto) == 1);
  masm.branchPtr(Assembler::BelowOrEqual, proto, ImmWord(1), &needVMCall);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Use VMCall for missing or lazy proto
  Label needVMCall;
  MOZ_ASSERT(uintptr_t(TaggedProto::LazyProto) == 1);
  masm.branchPtr(Assembler::BelowOrEqual, proto, ImmWord(1), &needVMCall);
||||||| merged common ancestors
    // Use VMCall for missing or lazy proto
    Label needVMCall;
    MOZ_ASSERT(uintptr_t(TaggedProto::LazyProto) == 1);
    masm.branchPtr(Assembler::BelowOrEqual, proto, ImmWord(1), &needVMCall);
=======
  // Use VMCall for non-JSFunction objects (eg. Proxy)
  masm.branchTestObjClass(Assembler::NotEqual, proto, &JSFunction::class_,
                          scratch, proto, &needVMCall);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Use VMCall for non-JSFunction objects (eg. Proxy)
  masm.branchTestObjClass(Assembler::NotEqual, proto, &JSFunction::class_,
                          scratch, proto, &needVMCall);
||||||| merged common ancestors
    // Use VMCall for non-JSFunction objects (eg. Proxy)
    masm.branchTestObjClass(Assembler::NotEqual, proto, &JSFunction::class_, scratch, proto,
                            &needVMCall);
=======
  // Use VMCall if not constructor
  masm.load16ZeroExtend(Address(proto, JSFunction::offsetOfFlags()), scratch);
  masm.branchTest32(Assembler::Zero, scratch, Imm32(JSFunction::CONSTRUCTOR),
                    &needVMCall);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Use VMCall if not constructor
  masm.load16ZeroExtend(Address(proto, JSFunction::offsetOfFlags()), scratch);
  masm.branchTest32(Assembler::Zero, scratch, Imm32(JSFunction::CONSTRUCTOR),
                    &needVMCall);
||||||| merged common ancestors
    // Use VMCall if not constructor
    masm.load16ZeroExtend(Address(proto, JSFunction::offsetOfFlags()), scratch);
    masm.branchTest32(Assembler::Zero, scratch, Imm32(JSFunction::CONSTRUCTOR), &needVMCall);
=======
  // Valid constructor
  Label hasSuperFun;
  masm.jump(&hasSuperFun);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Valid constructor
  Label hasSuperFun;
  masm.jump(&hasSuperFun);
||||||| merged common ancestors
    // Valid constructor
    Label hasSuperFun;
    masm.jump(&hasSuperFun);
=======
  // Slow path VM Call
  masm.bind(&needVMCall);
  prepareVMCall();
  pushArg(callee);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Slow path VM Call
  masm.bind(&needVMCall);
  prepareVMCall();
  pushArg(callee);
  if (!callVM(SuperFunOperationInfo)) {
    return false;
  }
  masm.movePtr(ReturnReg, proto);
||||||| merged common ancestors
    // Slow path VM Call
    masm.bind(&needVMCall);
    prepareVMCall();
    pushArg(callee);
    if (!callVM(SuperFunOperationInfo)) {
        return false;
    }
    masm.movePtr(ReturnReg, proto);
=======
  using Fn = JSObject* (*)(JSContext*, HandleObject);
  if (!callVM<Fn, js::SuperFunOperation>()) {
    return false;
  }
  masm.movePtr(ReturnReg, proto);
>>>>>>> upstream-releases

  // Box prototype and return
  masm.bind(&hasSuperFun);
  masm.tagValue(JSVAL_TYPE_OBJECT, proto, R1);
  frame.push(R1);
  return true;
}

<<<<<<< HEAD
typedef bool (*NewArgumentsObjectFn)(JSContext*, BaselineFrame*,
                                     MutableHandleValue);
static const VMFunction NewArgumentsObjectInfo =
    FunctionInfo<NewArgumentsObjectFn>(jit::NewArgumentsObject,
                                       "NewArgumentsObject");
||||||| merged common ancestors
typedef bool (*NewArgumentsObjectFn)(JSContext*, BaselineFrame*, MutableHandleValue);
static const VMFunction NewArgumentsObjectInfo =
    FunctionInfo<NewArgumentsObjectFn>(jit::NewArgumentsObject, "NewArgumentsObject");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ARGUMENTS() {
  frame.syncStack(0);

  MOZ_ASSERT_IF(handler.maybeScript(),
                handler.maybeScript()->argumentsHasVarBinding());
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_ARGUMENTS() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_ARGUMENTS()
{
    frame.syncStack(0);
=======
  Label done;
  if (!handler.maybeScript() || !handler.maybeScript()->needsArgsObj()) {
    // We assume the script does not need an arguments object. However, this
    // assumption can be invalidated later, see argumentsOptimizationFailed
    // in JSScript. Guard on the script's NeedsArgsObj flag.
    masm.moveValue(MagicValue(JS_OPTIMIZED_ARGUMENTS), R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(script->argumentsHasVarBinding());
||||||| merged common ancestors
    Label done;
    if (!script->argumentsHasVarBinding() || !script->needsArgsObj()) {
        // We assume the script does not need an arguments object. However, this
        // assumption can be invalidated later, see argumentsOptimizationFailed
        // in JSScript. Because we can't invalidate baseline JIT code, we set a
        // flag on BaselineScript when that happens and guard on it here.
        masm.moveValue(MagicValue(JS_OPTIMIZED_ARGUMENTS), R0);

        // Load script->baseline.
        Register scratch = R1.scratchReg();
        masm.movePtr(ImmGCPtr(script), scratch);
        masm.loadPtr(Address(scratch, JSScript::offsetOfBaselineScript()), scratch);

        // If we don't need an arguments object, skip the VM call.
        masm.branchTest32(Assembler::Zero, Address(scratch, BaselineScript::offsetOfFlags()),
                          Imm32(BaselineScript::NEEDS_ARGS_OBJ), &done);
    }
=======
    // If we don't need an arguments object, skip the VM call.
    Register scratch = R1.scratchReg();
    loadScript(scratch);
    masm.branchTest32(
        Assembler::Zero, Address(scratch, JSScript::offsetOfMutableFlags()),
        Imm32(uint32_t(JSScript::MutableFlags::NeedsArgsObj)), &done);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label done;
  if (!script->needsArgsObj()) {
    // We assume the script does not need an arguments object. However, this
    // assumption can be invalidated later, see argumentsOptimizationFailed
    // in JSScript. Guard on the script's NeedsArgsObj flag.
    masm.moveValue(MagicValue(JS_OPTIMIZED_ARGUMENTS), R0);
||||||| merged common ancestors
    prepareVMCall();
=======
  prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
    // If we don't need an arguments object, skip the VM call.
    Register scratch = R1.scratchReg();
    masm.movePtr(ImmGCPtr(script), scratch);
    masm.branchTest32(
        Assembler::Zero, Address(scratch, JSScript::offsetOfMutableFlags()),
        Imm32(uint32_t(JSScript::MutableFlags::NeedsArgsObj)), &done);
  }
||||||| merged common ancestors
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
    pushArg(R0.scratchReg());
=======
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
  pushArg(R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
||||||| merged common ancestors
    if (!callVM(NewArgumentsObjectInfo)) {
        return false;
    }
=======
  using Fn = bool (*)(JSContext*, BaselineFrame*, MutableHandleValue);
  if (!callVM<Fn, jit::NewArgumentsObject>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
  pushArg(R0.scratchReg());

  if (!callVM(NewArgumentsObjectInfo)) {
    return false;
  }

  masm.bind(&done);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    masm.bind(&done);
    frame.push(R0);
    return true;
=======
  masm.bind(&done);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*RunOnceScriptPrologueFn)(JSContext*, HandleScript);
static const VMFunction RunOnceScriptPrologueInfo =
    FunctionInfo<RunOnceScriptPrologueFn>(js::RunOnceScriptPrologue,
                                          "RunOnceScriptPrologue");

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_RUNONCE() {
  frame.syncStack(0);
||||||| merged common ancestors
typedef bool (*RunOnceScriptPrologueFn)(JSContext*, HandleScript);
static const VMFunction RunOnceScriptPrologueInfo =
    FunctionInfo<RunOnceScriptPrologueFn>(js::RunOnceScriptPrologue, "RunOnceScriptPrologue");

bool
BaselineCompiler::emit_JSOP_RUNONCE()
{
    frame.syncStack(0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_REST() {
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();

  masm.movePtr(ImmGCPtr(script), R0.scratchReg());
  pushArg(R0.scratchReg());
||||||| merged common ancestors
    prepareVMCall();

    masm.movePtr(ImmGCPtr(script), R0.scratchReg());
    pushArg(R0.scratchReg());
=======
  if (!emitNextIC()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return callVM(RunOnceScriptPrologueInfo);
||||||| merged common ancestors
    return callVM(RunOnceScriptPrologueInfo);
=======
  // Mark R0 as pushed stack value.
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_REST() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_REST()
{
    frame.syncStack(0);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GENERATOR() {
  frame.assertStackDepth(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!emitNextIC()) {
    return false;
  }
||||||| merged common ancestors
    ArrayObject* templateObject =
        ObjectGroup::newArrayObject(cx, nullptr, 0, TenuredObject,
                                    ObjectGroup::NewArrayKind::UnknownIndex);
    if (!templateObject) {
        return false;
    }
=======
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Mark R0 as pushed stack value.
  frame.push(R0);
  return true;
||||||| merged common ancestors
    // Call IC.
    ICRest_Fallback::Compiler compiler(cx, templateObject);
    if (!emitOpIC(compiler.getStub(&stubSpace_))) {
        return false;
    }

    // Mark R0 as pushed stack value.
    frame.push(R0);
    return true;
=======
  prepareVMCall();
  pushArg(R0.scratchReg());

  using Fn = JSObject* (*)(JSContext*, BaselineFrame*);
  if (!callVM<Fn, jit::CreateGenerator>()) {
    return false;
  }

  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITIALYIELD() {
  frame.syncStack(0);
  frame.assertStackDepth(1);

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_GENERATOR() {
  MOZ_ASSERT(frame.stackDepth() == 0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_GENERATOR()
{
    MOZ_ASSERT(frame.stackDepth() == 0);
=======
  Register genObj = R2.scratchReg();
  masm.unboxObject(frame.addressOfStackValue(-1), genObj);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
||||||| merged common ancestors
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
=======
  MOZ_ASSERT_IF(handler.maybePC(), GET_RESUMEINDEX(handler.maybePC()) == 0);
  masm.storeValue(
      Int32Value(0),
      Address(genObj, AbstractGeneratorObject::offsetOfResumeIndexSlot()));
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(R0.scratchReg());
  if (!callVM(CreateGeneratorInfo)) {
    return false;
  }
||||||| merged common ancestors
    prepareVMCall();
    pushArg(R0.scratchReg());
    if (!callVM(CreateGeneratorInfo)) {
        return false;
    }
=======
  Register envObj = R0.scratchReg();
  Register temp = R1.scratchReg();
  Address envChainSlot(genObj,
                       AbstractGeneratorObject::offsetOfEnvironmentChainSlot());
  masm.loadPtr(frame.addressOfEnvironmentChain(), envObj);
  masm.guardedCallPreBarrierAnyZone(envChainSlot, MIRType::Value, temp);
  masm.storeValue(JSVAL_TYPE_OBJECT, envObj, envChainSlot);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITIALYIELD() {
  frame.syncStack(0);
  MOZ_ASSERT(frame.stackDepth() == 1);

  Register genObj = R2.scratchReg();
  masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), genObj);

  MOZ_ASSERT(GET_RESUMEINDEX(pc) == 0);
  masm.storeValue(Int32Value(0),
                  Address(genObj, GeneratorObject::offsetOfResumeIndexSlot()));
||||||| merged common ancestors
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.push(R0);
    return true;
}

bool
BaselineCompiler::addYieldAndAwaitOffset()
{
    MOZ_ASSERT(*pc == JSOP_INITIALYIELD || *pc == JSOP_YIELD || *pc == JSOP_AWAIT);

    uint32_t yieldAndAwaitIndex = GET_UINT24(pc);

    while (yieldAndAwaitIndex >= yieldAndAwaitOffsets_.length()) {
        if (!yieldAndAwaitOffsets_.append(0)) {
            return false;
        }
    }
=======
  Label skipBarrier;
  masm.branchPtrInNurseryChunk(Assembler::Equal, genObj, temp, &skipBarrier);
  masm.branchPtrInNurseryChunk(Assembler::NotEqual, envObj, temp, &skipBarrier);
  masm.push(genObj);
  MOZ_ASSERT(genObj == R2.scratchReg());
  masm.call(&postBarrierSlot_);
  masm.pop(genObj);
  masm.bind(&skipBarrier);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register envObj = R0.scratchReg();
  Address envChainSlot(genObj, GeneratorObject::offsetOfEnvironmentChainSlot());
  masm.loadPtr(frame.addressOfEnvironmentChain(), envObj);
  masm.guardedCallPreBarrier(envChainSlot, MIRType::Value);
  masm.storeValue(JSVAL_TYPE_OBJECT, envObj, envChainSlot);

  Register temp = R1.scratchReg();
  Label skipBarrier;
  masm.branchPtrInNurseryChunk(Assembler::Equal, genObj, temp, &skipBarrier);
  masm.branchPtrInNurseryChunk(Assembler::NotEqual, envObj, temp, &skipBarrier);
  masm.push(genObj);
  MOZ_ASSERT(genObj == R2.scratchReg());
  masm.call(&postBarrierSlot_);
  masm.pop(genObj);
  masm.bind(&skipBarrier);

  masm.tagValue(JSVAL_TYPE_OBJECT, genObj, JSReturnOperand);
  return emitReturn();
||||||| merged common ancestors
    static_assert(JSOP_INITIALYIELD_LENGTH == JSOP_YIELD_LENGTH &&
                  JSOP_INITIALYIELD_LENGTH == JSOP_AWAIT_LENGTH,
                  "code below assumes INITIALYIELD and YIELD and AWAIT have same length");
    yieldAndAwaitOffsets_[yieldAndAwaitIndex] = script->pcToOffset(pc + JSOP_YIELD_LENGTH);
    return true;
=======
  masm.tagValue(JSVAL_TYPE_OBJECT, genObj, JSReturnOperand);
  return emitReturn();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*NormalSuspendFn)(JSContext*, HandleObject, BaselineFrame*,
                                jsbytecode*, uint32_t);
static const VMFunction NormalSuspendInfo =
    FunctionInfo<NormalSuspendFn>(jit::NormalSuspend, "NormalSuspend");
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITIALYIELD()
{
    if (!addYieldAndAwaitOffset()) {
        return false;
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_YIELD() {
  // Store generator in R0.
  frame.popRegsAndSync(1);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_YIELD() {
  // Store generator in R0.
  frame.popRegsAndSync(1);

  Register genObj = R2.scratchReg();
  masm.unboxObject(R0, genObj);

  MOZ_ASSERT(frame.stackDepth() >= 1);
||||||| merged common ancestors
    frame.syncStack(0);
    MOZ_ASSERT(frame.stackDepth() == 1);
=======
  Register genObj = R2.scratchReg();
  masm.unboxObject(R0, genObj);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (frame.stackDepth() == 1) {
    // If the expression stack is empty, we can inline the YIELD.
||||||| merged common ancestors
    Register genObj = R2.scratchReg();
    masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), genObj);
=======
  if (frame.hasKnownStackDepth(1)) {
    // If the expression stack is empty, we can inline the YIELD. Note that this
    // branch is never taken for the interpreter because it doesn't know static
    // stack depths.
>>>>>>> upstream-releases

<<<<<<< HEAD
    masm.storeValue(
        Int32Value(GET_RESUMEINDEX(pc)),
        Address(genObj, GeneratorObject::offsetOfResumeIndexSlot()));
||||||| merged common ancestors
    MOZ_ASSERT(GET_UINT24(pc) == 0);
    masm.storeValue(Int32Value(0),
                    Address(genObj, GeneratorObject::offsetOfYieldAndAwaitIndexSlot()));
=======
    Register temp = R1.scratchReg();
    Address resumeIndexSlot(genObj,
                            AbstractGeneratorObject::offsetOfResumeIndexSlot());
    jsbytecode* pc = handler.maybePC();
    MOZ_ASSERT(pc, "compiler-only code never has a null pc");
    masm.move32(Imm32(GET_RESUMEINDEX(pc)), temp);
    masm.storeValue(JSVAL_TYPE_INT32, temp, resumeIndexSlot);
>>>>>>> upstream-releases

    Register envObj = R0.scratchReg();
<<<<<<< HEAD
    Address envChainSlot(genObj,
                         GeneratorObject::offsetOfEnvironmentChainSlot());
||||||| merged common ancestors
    Address envChainSlot(genObj, GeneratorObject::offsetOfEnvironmentChainSlot());
=======
    Address envChainSlot(
        genObj, AbstractGeneratorObject::offsetOfEnvironmentChainSlot());
>>>>>>> upstream-releases
    masm.loadPtr(frame.addressOfEnvironmentChain(), envObj);
    masm.guardedCallPreBarrier(envChainSlot, MIRType::Value);
    masm.storeValue(JSVAL_TYPE_OBJECT, envObj, envChainSlot);

    Label skipBarrier;
    masm.branchPtrInNurseryChunk(Assembler::Equal, genObj, temp, &skipBarrier);
    masm.branchPtrInNurseryChunk(Assembler::NotEqual, envObj, temp,
                                 &skipBarrier);
    MOZ_ASSERT(genObj == R2.scratchReg());
    masm.call(&postBarrierSlot_);
    masm.bind(&skipBarrier);
  } else {
    masm.loadBaselineFramePtr(BaselineFrameReg, R1.scratchReg());

<<<<<<< HEAD
    prepareVMCall();
    pushArg(Imm32(frame.stackDepth()));
    pushArg(ImmPtr(pc));
    pushArg(R1.scratchReg());
    pushArg(genObj);
||||||| merged common ancestors
    masm.tagValue(JSVAL_TYPE_OBJECT, genObj, JSReturnOperand);
    return emitReturn();
}

typedef bool (*NormalSuspendFn)(JSContext*, HandleObject, BaselineFrame*, jsbytecode*, uint32_t);
static const VMFunction NormalSuspendInfo =
    FunctionInfo<NormalSuspendFn>(jit::NormalSuspend, "NormalSuspend");

bool
BaselineCompiler::emit_JSOP_YIELD()
{
    if (!addYieldAndAwaitOffset()) {
        return false;
    }

    // Store generator in R0.
    frame.popRegsAndSync(1);

    Register genObj = R2.scratchReg();
    masm.unboxObject(R0, genObj);

    MOZ_ASSERT(frame.stackDepth() >= 1);

    if (frame.stackDepth() == 1) {
        // If the expression stack is empty, we can inline the YIELD.

        masm.storeValue(Int32Value(GET_UINT24(pc)),
                        Address(genObj, GeneratorObject::offsetOfYieldAndAwaitIndexSlot()));

        Register envObj = R0.scratchReg();
        Address envChainSlot(genObj, GeneratorObject::offsetOfEnvironmentChainSlot());
        masm.loadPtr(frame.addressOfEnvironmentChain(), envObj);
        masm.guardedCallPreBarrier(envChainSlot, MIRType::Value);
        masm.storeValue(JSVAL_TYPE_OBJECT, envObj, envChainSlot);

        Register temp = R1.scratchReg();
        Label skipBarrier;
        masm.branchPtrInNurseryChunk(Assembler::Equal, genObj, temp, &skipBarrier);
        masm.branchPtrInNurseryChunk(Assembler::NotEqual, envObj, temp, &skipBarrier);
        MOZ_ASSERT(genObj == R2.scratchReg());
        masm.call(&postBarrierSlot_);
        masm.bind(&skipBarrier);
    } else {
        masm.loadBaselineFramePtr(BaselineFrameReg, R1.scratchReg());

        prepareVMCall();
        pushArg(Imm32(frame.stackDepth()));
        pushArg(ImmPtr(pc));
        pushArg(R1.scratchReg());
        pushArg(genObj);
=======
    prepareVMCall();
    pushBytecodePCArg();
    pushArg(R1.scratchReg());
    pushArg(genObj);

    using Fn = bool (*)(JSContext*, HandleObject, BaselineFrame*, jsbytecode*);
    if (!callVM<Fn, jit::NormalSuspend>()) {
      return false;
    }
  }

  masm.loadValue(frame.addressOfStackValue(-1), JSReturnOperand);
  return emitReturn();
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_AWAIT() {
  return emit_JSOP_YIELD();
}

template <>
template <typename F>
bool BaselineCompilerCodeGen::emitAfterYieldDebugInstrumentation(
    const F& ifDebuggee, Register) {
  if (handler.compileDebugInstrumentation()) {
    return ifDebuggee();
  }
  return true;
}

template <>
template <typename F>
bool BaselineInterpreterCodeGen::emitAfterYieldDebugInstrumentation(
    const F& ifDebuggee, Register scratch) {
  // Note that we can't use emitDebugInstrumentation here because the frame's
  // DEBUGGEE flag hasn't been initialized yet.

  // If the current Realm is not a debuggee we're done.
  Label done;
  masm.loadPtr(AbsoluteAddress(cx->addressOfRealm()), scratch);
  masm.branchTest32(Assembler::Zero,
                    Address(scratch, Realm::offsetOfDebugModeBits()),
                    Imm32(Realm::debugModeIsDebuggeeBit()), &done);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!callVM(NormalSuspendInfo)) {
      return false;
    }
  }
||||||| merged common ancestors
        if (!callVM(NormalSuspendInfo)) {
            return false;
        }
    }
=======
  if (!ifDebuggee()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), JSReturnOperand);
  return emitReturn();
||||||| merged common ancestors
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), JSReturnOperand);
    return emitReturn();
=======
  masm.bind(&done);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_AWAIT() {
  return emit_JSOP_YIELD();
}

typedef bool (*DebugAfterYieldFn)(JSContext*, BaselineFrame*, jsbytecode*,
                                  bool*);
static const VMFunction DebugAfterYieldInfo =
    FunctionInfo<DebugAfterYieldFn>(jit::DebugAfterYield, "DebugAfterYield");

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEBUGAFTERYIELD() {
  auto ifDebuggee = [this]() {
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_AWAIT()
{
    return emit_JSOP_YIELD();
}

typedef bool (*DebugAfterYieldFn)(JSContext*, BaselineFrame*, jsbytecode*, bool*);
static const VMFunction DebugAfterYieldInfo =
    FunctionInfo<DebugAfterYieldFn>(jit::DebugAfterYield, "DebugAfterYield");

bool
BaselineCompiler::emit_JSOP_DEBUGAFTERYIELD()
{
    if (!compileDebugInstrumentation_) {
        return true;
    }

=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_AFTERYIELD() {
  if (!emit_JSOP_JUMPTARGET()) {
    return false;
  }

  auto ifDebuggee = [this]() {
>>>>>>> upstream-releases
    frame.assertSyncedStack();
    masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
    prepareVMCall();
    pushBytecodePCArg();
    pushArg(R0.scratchReg());
<<<<<<< HEAD
    if (!callVM(DebugAfterYieldInfo)) {
      return false;
||||||| merged common ancestors
    if (!callVM(DebugAfterYieldInfo)) {
        return false;
=======

    using Fn = bool (*)(JSContext*, BaselineFrame*, jsbytecode*, bool*);
    if (!callVM<Fn, jit::DebugAfterYield>()) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    retAddrEntries_.back().setKind(RetAddrEntry::Kind::DebugAfterYield);
||||||| merged common ancestors
    icEntries_.back().setFakeKind(ICEntry::Kind_DebugAfterYield);
=======
    handler.markLastRetAddrEntryKind(RetAddrEntry::Kind::DebugAfterYield);
>>>>>>> upstream-releases

    Label done;
    masm.branchTest32(Assembler::Zero, ReturnReg, ReturnReg, &done);
    {
      masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
      masm.jump(&return_);
    }
    masm.bind(&done);
    return true;
<<<<<<< HEAD
  };
  return emitDebugInstrumentation(ifDebuggee);
}
||||||| merged common ancestors
}
=======
  };
  return emitAfterYieldDebugInstrumentation(ifDebuggee, R0.scratchReg());
}
>>>>>>> upstream-releases

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FINALYIELDRVAL() {
  // Store generator in R0.
  frame.popRegsAndSync(1);
  masm.unboxObject(R0, R0.scratchReg());

  prepareVMCall();
  pushBytecodePCArg();
  pushArg(R0.scratchReg());

  using Fn = bool (*)(JSContext*, HandleObject, jsbytecode*);
  if (!callVM<Fn, jit::FinalSuspend>()) {
    return false;
  }

  masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
  return emitReturn();
}

template <>
void BaselineCompilerCodeGen::emitJumpToInterpretOpLabel() {
  TrampolinePtr code =
      cx->runtime()->jitRuntime()->baselineInterpreter().interpretOpAddr();
  masm.jump(code);
}

template <>
void BaselineInterpreterCodeGen::emitJumpToInterpretOpLabel() {
  masm.jump(handler.interpretOpLabel());
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emitEnterGeneratorCode(Register script,
                                                      Register resumeIndex,
                                                      Register scratch) {
  Address baselineAddr(script, JSScript::offsetOfBaselineScript());

  auto emitEnterBaseline = [&]() {
    masm.loadPtr(baselineAddr, script);
    masm.load32(Address(script, BaselineScript::offsetOfResumeEntriesOffset()),
                scratch);
    masm.addPtr(scratch, script);
    masm.loadPtr(
        BaseIndex(script, resumeIndex, ScaleFromElemWidth(sizeof(uintptr_t))),
        scratch);
    masm.jump(scratch);
  };

  if (!JitOptions.baselineInterpreter) {
    // We must have a BaselineScript.
    emitEnterBaseline();
    return true;
  }

  // If the Baseline Interpreter is enabled we resume in either the
  // BaselineScript (if present) or Baseline Interpreter.
  Label noBaselineScript;
  masm.branchPtr(Assembler::BelowOrEqual, baselineAddr,
                 ImmPtr(BASELINE_DISABLED_SCRIPT), &noBaselineScript);
  emitEnterBaseline();

  masm.bind(&noBaselineScript);

  // Initialize interpreter frame fields.
  Address flagsAddr(BaselineFrameReg, BaselineFrame::reverseOffsetOfFlags());
  Address scriptAddr(BaselineFrameReg,
                     BaselineFrame::reverseOffsetOfInterpreterScript());
  masm.or32(Imm32(BaselineFrame::RUNNING_IN_INTERPRETER), flagsAddr);
  masm.storePtr(script, scriptAddr);

  // Initialize pc and jump to it.
  emitInterpJumpToResumeEntry(script, resumeIndex, scratch);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emitGeneratorResume(
    GeneratorResumeKind resumeKind) {
  frame.syncStack(0);
  masm.assertStackAlignment(sizeof(Value), 0);

  AllocatableGeneralRegisterSet regs(GeneralRegisterSet::All());
  regs.take(BaselineFrameReg);
  if (HasInterpreterPCReg()) {
    regs.take(InterpreterPCReg);
  }

  // Load generator object.
  Register genObj = regs.takeAny();
  masm.unboxObject(frame.addressOfStackValue(-2), genObj);

  // Load callee.
  Register callee = regs.takeAny();
  masm.unboxObject(
      Address(genObj, AbstractGeneratorObject::offsetOfCalleeSlot()), callee);

  // Load the return value.
  ValueOperand retVal = regs.takeAnyValue();
  masm.loadValue(frame.addressOfStackValue(-1), retVal);

  // Branch to interpret if the script does not have a JitScript or
  // BaselineScript (depending on whether the Baseline Interpreter is enabled).
  // Note that we don't relazify generator scripts, so the function is
  // guaranteed to be non-lazy.
  Label interpret;
  Register scratch1 = regs.takeAny();
  masm.loadPtr(Address(callee, JSFunction::offsetOfScript()), scratch1);
  Address baselineAddr(scratch1, JSScript::offsetOfBaselineScript());
  if (JitOptions.baselineInterpreter) {
    Address jitScriptAddr(scratch1, JSScript::offsetOfJitScript());
    masm.branchPtr(Assembler::Equal, jitScriptAddr, ImmPtr(nullptr),
                   &interpret);
  } else {
    masm.branchPtr(Assembler::BelowOrEqual, baselineAddr,
                   ImmPtr(BASELINE_DISABLED_SCRIPT), &interpret);
  }

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FINALYIELDRVAL() {
  // Store generator in R0.
  frame.popRegsAndSync(1);
  masm.unboxObject(R0, R0.scratchReg());
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_FINALYIELDRVAL()
{
    // Store generator in R0.
    frame.popRegsAndSync(1);
    masm.unboxObject(R0, R0.scratchReg());
=======
#ifdef JS_TRACE_LOGGING
  if (JS::TraceLoggerSupported()) {
    masm.loadPtr(baselineAddr, scratch1);
    if (!emitTraceLoggerResume(scratch1, regs)) {
      return false;
    }
  }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(ImmPtr(pc));
  pushArg(R0.scratchReg());
||||||| merged common ancestors
    prepareVMCall();
    pushArg(ImmPtr(pc));
    pushArg(R0.scratchReg());
=======
  // Push |undefined| for all formals.
  Register scratch2 = regs.takeAny();
  Label loop, loopDone;
  masm.load16ZeroExtend(Address(callee, JSFunction::offsetOfNargs()), scratch2);
  masm.bind(&loop);
  masm.branchTest32(Assembler::Zero, scratch2, scratch2, &loopDone);
  {
    masm.pushValue(UndefinedValue());
    masm.sub32(Imm32(1), scratch2);
    masm.jump(&loop);
  }
  masm.bind(&loopDone);

  // Push |undefined| for |this|.
  masm.pushValue(UndefinedValue());

  // Update BaselineFrame frameSize field and create the frame descriptor.
  masm.computeEffectiveAddress(
      Address(BaselineFrameReg, BaselineFrame::FramePointerOffset), scratch2);
  masm.subStackPtrFrom(scratch2);
  masm.store32(scratch2, Address(BaselineFrameReg,
                                 BaselineFrame::reverseOffsetOfFrameSize()));
  masm.makeFrameDescriptor(scratch2, FrameType::BaselineJS,
                           JitFrameLayout::Size());

  masm.push(Imm32(0));  // actual argc
  masm.PushCalleeToken(callee, /* constructing = */ false);
  masm.push(scratch2);  // frame descriptor

  // PushCalleeToken bumped framePushed. Reset it.
  MOZ_ASSERT(masm.framePushed() == sizeof(uintptr_t));
  masm.setFramePushed(0);

  regs.add(callee);

  // Push a fake return address on the stack. We will resume here when the
  // generator returns.
  Label genStart, returnTarget;
#ifdef JS_USE_LINK_REGISTER
  masm.call(&genStart);
#else
  masm.callAndPushReturnAddress(&genStart);
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(FinalSuspendInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(FinalSuspendInfo)) {
        return false;
    }
=======
  // Add a RetAddrEntry so the return offset -> pc mapping works.
  if (!handler.appendRetAddrEntry(cx, RetAddrEntry::Kind::IC,
                                  masm.currentOffset())) {
    return false;
  }

  masm.jump(&returnTarget);
  masm.bind(&genStart);
#ifdef JS_USE_LINK_REGISTER
  masm.pushReturnAddress();
#endif

  // If profiler instrumentation is on, update lastProfilingFrame on
  // current JitActivation
  {
    Register scratchReg = scratch2;
    Label skip;
    AbsoluteAddress addressOfEnabled(
        cx->runtime()->geckoProfiler().addressOfEnabled());
    masm.branch32(Assembler::Equal, addressOfEnabled, Imm32(0), &skip);
    masm.loadJSContext(scratchReg);
    masm.loadPtr(Address(scratchReg, JSContext::offsetOfProfilingActivation()),
                 scratchReg);
    masm.storeStackPtr(
        Address(scratchReg, JitActivation::offsetOfLastProfilingFrame()));
    masm.bind(&skip);
  }

  // Construct BaselineFrame.
  masm.push(BaselineFrameReg);
  masm.moveStackPtrTo(BaselineFrameReg);
  masm.subFromStackPtr(Imm32(BaselineFrame::Size()));
  masm.assertStackAlignment(sizeof(Value), 0);

  // Store flags and env chain.
  masm.store32(Imm32(BaselineFrame::HAS_INITIAL_ENV), frame.addressOfFlags());
  masm.unboxObject(
      Address(genObj, AbstractGeneratorObject::offsetOfEnvironmentChainSlot()),
      scratch2);
  masm.storePtr(scratch2, frame.addressOfEnvironmentChain());

  // Store the arguments object if there is one.
  Label noArgsObj;
  Address argsObjSlot(genObj, AbstractGeneratorObject::offsetOfArgsObjSlot());
  masm.branchTestUndefined(Assembler::Equal, argsObjSlot, &noArgsObj);
  masm.unboxObject(argsObjSlot, scratch2);
  {
    masm.storePtr(scratch2, frame.addressOfArgsObj());
    masm.or32(Imm32(BaselineFrame::HAS_ARGS_OBJ), frame.addressOfFlags());
  }
  masm.bind(&noArgsObj);

  // Push expression slots if needed.
  Label noExprStack;
  Address exprStackSlot(genObj,
                        AbstractGeneratorObject::offsetOfExpressionStackSlot());
  masm.branchTestNull(Assembler::Equal, exprStackSlot, &noExprStack);
  {
    masm.unboxObject(exprStackSlot, scratch2);

    Register initLength = regs.takeAny();
    masm.loadPtr(Address(scratch2, NativeObject::offsetOfElements()), scratch2);
    masm.load32(Address(scratch2, ObjectElements::offsetOfInitializedLength()),
                initLength);
    masm.store32(
        Imm32(0),
        Address(scratch2, ObjectElements::offsetOfInitializedLength()));

    Label loop, loopDone;
    masm.bind(&loop);
    masm.branchTest32(Assembler::Zero, initLength, initLength, &loopDone);
    {
      masm.pushValue(Address(scratch2, 0));
      masm.guardedCallPreBarrierAnyZone(Address(scratch2, 0), MIRType::Value,
                                        scratch1);
      masm.addPtr(Imm32(sizeof(Value)), scratch2);
      masm.sub32(Imm32(1), initLength);
      masm.jump(&loop);
    }
    masm.bind(&loopDone);
    regs.add(initLength);
  }

  masm.bind(&noExprStack);
  masm.pushValue(retVal);

  masm.switchToObjectRealm(genObj, scratch2);

  if (resumeKind == GeneratorResumeKind::Next) {
    // Load script in scratch1.
    masm.unboxObject(
        Address(genObj, AbstractGeneratorObject::offsetOfCalleeSlot()),
        scratch1);
    masm.loadPtr(Address(scratch1, JSFunction::offsetOfScript()), scratch1);

    // Load resume index in scratch2 and mark generator as running.
    Address resumeIndexSlot(genObj,
                            AbstractGeneratorObject::offsetOfResumeIndexSlot());
    masm.unboxInt32(resumeIndexSlot, scratch2);
    masm.storeValue(Int32Value(AbstractGeneratorObject::RESUME_INDEX_RUNNING),
                    resumeIndexSlot);

    if (!emitEnterGeneratorCode(scratch1, scratch2, regs.getAny())) {
      return false;
    }
  } else {
    MOZ_ASSERT(resumeKind == GeneratorResumeKind::Throw ||
               resumeKind == GeneratorResumeKind::Return);

    // Update the frame's frameSize field.
    masm.computeEffectiveAddress(
        Address(BaselineFrameReg, BaselineFrame::FramePointerOffset), scratch2);
    masm.movePtr(scratch2, scratch1);
    masm.subStackPtrFrom(scratch2);
    masm.store32(scratch2, Address(BaselineFrameReg,
                                   BaselineFrame::reverseOffsetOfFrameSize()));
    masm.loadBaselineFramePtr(BaselineFrameReg, scratch2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
  return emitReturn();
||||||| merged common ancestors
    masm.loadValue(frame.addressOfReturnValue(), JSReturnOperand);
    return emitReturn();
=======
    prepareVMCall();
    pushArg(Imm32(int32_t(resumeKind)));
    pushArg(retVal);
    pushArg(genObj);
    pushArg(scratch2);

    using Fn =
        bool (*)(JSContext*, BaselineFrame*, Handle<AbstractGeneratorObject*>,
                 HandleValue, uint32_t);
    TailCallVMFunctionId id =
        TailCallVMFunctionToId<Fn, jit::GeneratorThrowOrReturn>::id;
    TrampolinePtr code = cx->runtime()->jitRuntime()->getVMWrapper(id);
    const VMFunctionData& fun = GetVMFunction(id);

    // Create and push the frame descriptor.
    masm.subStackPtrFrom(scratch1);
    masm.makeFrameDescriptor(scratch1, FrameType::BaselineJS,
                             ExitFrameLayout::Size());
    masm.push(scratch1);

    // We have created a baseline frame as if we were the
    // callee. However, if we just did a regular call at this
    // point, our return address would be bogus: it would point at
    // self-hosted code, instead of the generator code that we are
    // pretending we are already executing. Instead, we push a
    // dummy return address. In jit::GeneratorThrowOrReturn,
    // we will set the baseline frame's overridePc. Frame iterators
    // will use the override pc instead of relying on the return
    // address.

    // On ARM64, the callee will push a bogus return address. On
    // other architectures, we push a null return address.
#ifndef JS_CODEGEN_ARM64
    masm.push(ImmWord(0));
#endif
    masm.jump(code);

    // Pop arguments and frame pointer (pushed by prepareVMCall) from
    // framePushed.
    masm.implicitPop((fun.explicitStackSlots() + 1) * sizeof(void*));
  }

  // Call into the VM to run in the C++ interpreter if there's no JitScript or
  // BaselineScript.
  masm.bind(&interpret);

  prepareVMCall();
  if (resumeKind == GeneratorResumeKind::Next) {
    pushArg(ImmGCPtr(cx->names().next));
  } else if (resumeKind == GeneratorResumeKind::Throw) {
    pushArg(ImmGCPtr(cx->names().throw_));
  } else {
    MOZ_ASSERT(resumeKind == GeneratorResumeKind::Return);
    pushArg(ImmGCPtr(cx->names().return_));
  }

  masm.loadValue(frame.addressOfStackValue(-1), retVal);
  pushArg(retVal);
  pushArg(genObj);

  using Fn = bool (*)(JSContext*, HandleObject, HandleValue, HandlePropertyName,
                      MutableHandleValue);
  if (!callVM<Fn, jit::InterpretResume>()) {
    return false;
  }

  // After the generator returns, we restore the stack pointer, switch back to
  // the current realm, push the return value, and we're done.
  masm.bind(&returnTarget);
  masm.computeEffectiveAddress(frame.addressOfStackValue(-1),
                               masm.getStackPointer());
  if (JSScript* script = handler.maybeScript()) {
    masm.switchToRealm(script->realm(), R2.scratchReg());
  } else {
    masm.switchToBaselineFrameRealm(R2.scratchReg());
  }
  restoreInterpreterPCReg();
  frame.popn(2);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*InterpretResumeFn)(JSContext*, HandleObject, HandleValue,
                                  HandlePropertyName, MutableHandleValue);
static const VMFunction InterpretResumeInfo =
    FunctionInfo<InterpretResumeFn>(jit::InterpretResume, "InterpretResume");
||||||| merged common ancestors
typedef bool (*InterpretResumeFn)(JSContext*, HandleObject, HandleValue, HandlePropertyName,
                                  MutableHandleValue);
static const VMFunction InterpretResumeInfo =
    FunctionInfo<InterpretResumeFn>(jit::InterpretResume, "InterpretResume");
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_RESUME() {
  auto resumeKind = AbstractGeneratorObject::getResumeKind(handler.pc());
  return emitGeneratorResume(resumeKind);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*GeneratorThrowFn)(JSContext*, BaselineFrame*,
                                 Handle<GeneratorObject*>, HandleValue,
                                 uint32_t);
static const VMFunction GeneratorThrowOrReturnInfo =
    FunctionInfo<GeneratorThrowFn>(jit::GeneratorThrowOrReturn,
                                   "GeneratorThrowOrReturn", TailCall);
||||||| merged common ancestors
typedef bool (*GeneratorThrowFn)(JSContext*, BaselineFrame*, Handle<GeneratorObject*>,
                                 HandleValue, uint32_t);
static const VMFunction GeneratorThrowOrReturnInfo =
    FunctionInfo<GeneratorThrowFn>(jit::GeneratorThrowOrReturn, "GeneratorThrowOrReturn", TailCall);
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_RESUME() {
  Register scratch = R0.scratchReg();
  LoadUint8Operand(masm, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_RESUME() {
  GeneratorObject::ResumeKind resumeKind = GeneratorObject::getResumeKind(pc);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_RESUME()
{
    GeneratorObject::ResumeKind resumeKind = GeneratorObject::getResumeKind(pc);
=======
  saveInterpreterPCReg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.syncStack(0);
  masm.assertStackAlignment(sizeof(Value), 0);
||||||| merged common ancestors
    frame.syncStack(0);
    masm.assertStackAlignment(sizeof(Value), 0);
=======
  Label throw_, return_, done;
  masm.branch32(Assembler::Equal, scratch,
                Imm32(int32_t(GeneratorResumeKind::Throw)), &throw_);
  masm.branch32(Assembler::Equal, scratch,
                Imm32(int32_t(GeneratorResumeKind::Return)), &return_);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AllocatableGeneralRegisterSet regs(GeneralRegisterSet::All());
  regs.take(BaselineFrameReg);
||||||| merged common ancestors
    AllocatableGeneralRegisterSet regs(GeneralRegisterSet::All());
    regs.take(BaselineFrameReg);
=======
#ifdef DEBUG
  Label ok;
  masm.branch32(Assembler::Equal, scratch,
                Imm32(int32_t(GeneratorResumeKind::Next)), &ok);
  masm.assumeUnreachable("JSOP_RESUME invalid ResumeKind");
  masm.bind(&ok);
#endif
  if (!emitGeneratorResume(GeneratorResumeKind::Next)) {
    return false;
  }
  masm.jump(&done);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load generator object.
  Register genObj = regs.takeAny();
  masm.unboxObject(frame.addressOfStackValue(frame.peek(-2)), genObj);
||||||| merged common ancestors
    // Load generator object.
    Register genObj = regs.takeAny();
    masm.unboxObject(frame.addressOfStackValue(frame.peek(-2)), genObj);
=======
  masm.bind(&throw_);
  if (!emitGeneratorResume(GeneratorResumeKind::Throw)) {
    return false;
  }
  masm.jump(&done);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load callee.
  Register callee = regs.takeAny();
  masm.unboxObject(Address(genObj, GeneratorObject::offsetOfCalleeSlot()),
                   callee);
||||||| merged common ancestors
    // Load callee.
    Register callee = regs.takeAny();
    masm.unboxObject(Address(genObj, GeneratorObject::offsetOfCalleeSlot()), callee);
=======
  masm.bind(&return_);
  if (!emitGeneratorResume(GeneratorResumeKind::Return)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load the script. Note that we don't relazify generator scripts, so it's
  // guaranteed to be non-lazy.
  Register scratch1 = regs.takeAny();
  masm.loadPtr(Address(callee, JSFunction::offsetOfScript()), scratch1);
||||||| merged common ancestors
    // Load the script. Note that we don't relazify generator scripts, so it's
    // guaranteed to be non-lazy.
    Register scratch1 = regs.takeAny();
    masm.loadPtr(Address(callee, JSFunction::offsetOfScript()), scratch1);
=======
  masm.bind(&done);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load the BaselineScript or call a stub if we don't have one.
  Label interpret;
  masm.loadPtr(Address(scratch1, JSScript::offsetOfBaselineScript()), scratch1);
  masm.branchPtr(Assembler::BelowOrEqual, scratch1,
                 ImmPtr(BASELINE_DISABLED_SCRIPT), &interpret);
||||||| merged common ancestors
    // Load the BaselineScript or call a stub if we don't have one.
    Label interpret;
    masm.loadPtr(Address(scratch1, JSScript::offsetOfBaselineScript()), scratch1);
    masm.branchPtr(Assembler::BelowOrEqual, scratch1, ImmPtr(BASELINE_DISABLED_SCRIPT), &interpret);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEBUGCHECKSELFHOSTED() {
#ifdef DEBUG
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef JS_TRACE_LOGGING
  if (!emitTraceLoggerResume(scratch1, regs)) {
    return false;
  }
||||||| merged common ancestors
#ifdef JS_TRACE_LOGGING
    if (!emitTraceLoggerResume(scratch1, regs)) {
        return false;
    }
=======
  masm.loadValue(frame.addressOfStackValue(-1), R0);

  prepareVMCall();
  pushArg(R0);

  using Fn = bool (*)(JSContext*, HandleValue);
  if (!callVM<Fn, js::Debug_CheckSelfHosted>()) {
    return false;
  }
>>>>>>> upstream-releases
#endif
  return true;
}

<<<<<<< HEAD
  // Push |undefined| for all formals.
  Register scratch2 = regs.takeAny();
  Label loop, loopDone;
  masm.load16ZeroExtend(Address(callee, JSFunction::offsetOfNargs()), scratch2);
  masm.bind(&loop);
  masm.branchTest32(Assembler::Zero, scratch2, scratch2, &loopDone);
  {
    masm.pushValue(UndefinedValue());
    masm.sub32(Imm32(1), scratch2);
    masm.jump(&loop);
  }
  masm.bind(&loopDone);

  // Push |undefined| for |this|.
  masm.pushValue(UndefinedValue());

  // Update BaselineFrame frameSize field and create the frame descriptor.
  masm.computeEffectiveAddress(
      Address(BaselineFrameReg, BaselineFrame::FramePointerOffset), scratch2);
  masm.subStackPtrFrom(scratch2);
  masm.store32(scratch2, Address(BaselineFrameReg,
                                 BaselineFrame::reverseOffsetOfFrameSize()));
  masm.makeFrameDescriptor(scratch2, FrameType::BaselineJS,
                           JitFrameLayout::Size());

  masm.Push(Imm32(0));  // actual argc
  masm.PushCalleeToken(callee, /* constructing = */ false);
  masm.Push(scratch2);  // frame descriptor

  regs.add(callee);

  // Load the return value.
  ValueOperand retVal = regs.takeAnyValue();
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), retVal);

  // Push a fake return address on the stack. We will resume here when the
  // generator returns.
  Label genStart, returnTarget;
#ifdef JS_USE_LINK_REGISTER
  masm.call(&genStart);
#else
  masm.callAndPushReturnAddress(&genStart);
#endif
||||||| merged common ancestors
    // Push |undefined| for all formals.
    Register scratch2 = regs.takeAny();
    Label loop, loopDone;
    masm.load16ZeroExtend(Address(callee, JSFunction::offsetOfNargs()), scratch2);
    masm.bind(&loop);
    masm.branchTest32(Assembler::Zero, scratch2, scratch2, &loopDone);
    {
        masm.pushValue(UndefinedValue());
        masm.sub32(Imm32(1), scratch2);
        masm.jump(&loop);
    }
    masm.bind(&loopDone);

    // Push |undefined| for |this|.
    masm.pushValue(UndefinedValue());

    // Update BaselineFrame frameSize field and create the frame descriptor.
    masm.computeEffectiveAddress(Address(BaselineFrameReg, BaselineFrame::FramePointerOffset),
                                 scratch2);
    masm.subStackPtrFrom(scratch2);
    masm.store32(scratch2, Address(BaselineFrameReg, BaselineFrame::reverseOffsetOfFrameSize()));
    masm.makeFrameDescriptor(scratch2, FrameType::BaselineJS, JitFrameLayout::Size());

    masm.Push(Imm32(0)); // actual argc
    masm.PushCalleeToken(callee, /* constructing = */ false);
    masm.Push(scratch2); // frame descriptor

    regs.add(callee);

    // Load the return value.
    ValueOperand retVal = regs.takeAnyValue();
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), retVal);

    // Push a fake return address on the stack. We will resume here when the
    // generator returns.
    Label genStart, returnTarget;
#ifdef JS_USE_LINK_REGISTER
    masm.call(&genStart);
#else
    masm.callAndPushReturnAddress(&genStart);
#endif
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_IS_CONSTRUCTING() {
  frame.push(MagicValue(JS_IS_CONSTRUCTING));
  return true;
}

template <>
bool BaselineCompilerCodeGen::emit_JSOP_JUMPTARGET() {
  MaybeIncrementCodeCoverageCounter(masm, handler.script(), handler.pc());
  return true;
}

template <>
bool BaselineInterpreterCodeGen::emit_JSOP_JUMPTARGET() {
  Register scratch1 = R0.scratchReg();
  Register scratch2 = R1.scratchReg();

  Label skipCoverage;
  CodeOffset toggleOffset = masm.toggledJump(&skipCoverage);
  masm.call(handler.codeCoverageAtPCLabel());
  masm.bind(&skipCoverage);
  if (!handler.codeCoverageOffsets().append(toggleOffset.offset())) {
    return false;
  }

  // Load icIndex in scratch1.
  LoadInt32Operand(masm, scratch1);

  // scratch1 := scratch1 * sizeof(ICEntry)
  static_assert(sizeof(ICEntry) == 8 || sizeof(ICEntry) == 16,
                "shift below depends on ICEntry size");
  uint32_t shift = (sizeof(ICEntry) == 16) ? 4 : 3;
  masm.lshiftPtr(Imm32(shift), scratch1);

  // Compute ICEntry* and store to frame->interpreterICEntry.
  loadScript(scratch2);
  masm.loadPtr(Address(scratch2, JSScript::offsetOfJitScript()), scratch2);
  masm.computeEffectiveAddress(
      BaseIndex(scratch2, scratch1, TimesOne, JitScript::offsetOfICEntries()),
      scratch2);
  masm.storePtr(scratch2, frame.addressOfInterpreterICEntry());
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Add a RetAddrEntry so the return offset -> pc mapping works.
  if (!appendRetAddrEntry(RetAddrEntry::Kind::IC, masm.currentOffset())) {
    return false;
  }
||||||| merged common ancestors
    // Add an IC entry so the return offset -> pc mapping works.
    if (!appendICEntry(ICEntry::Kind_Op, masm.currentOffset())) {
        return false;
    }
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKCLASSHERITAGE() {
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.jump(&returnTarget);
  masm.bind(&genStart);
#ifdef JS_USE_LINK_REGISTER
  masm.pushReturnAddress();
#endif
||||||| merged common ancestors
    masm.jump(&returnTarget);
    masm.bind(&genStart);
#ifdef JS_USE_LINK_REGISTER
    masm.pushReturnAddress();
#endif
=======
  // Leave the heritage value on the stack.
  masm.loadValue(frame.addressOfStackValue(-1), R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If profiler instrumentation is on, update lastProfilingFrame on
  // current JitActivation
  {
    Register scratchReg = scratch2;
    Label skip;
    AbsoluteAddress addressOfEnabled(
        cx->runtime()->geckoProfiler().addressOfEnabled());
    masm.branch32(Assembler::Equal, addressOfEnabled, Imm32(0), &skip);
    masm.loadJSContext(scratchReg);
    masm.loadPtr(Address(scratchReg, JSContext::offsetOfProfilingActivation()),
                 scratchReg);
    masm.storeStackPtr(
        Address(scratchReg, JitActivation::offsetOfLastProfilingFrame()));
    masm.bind(&skip);
  }

  // Construct BaselineFrame.
  masm.push(BaselineFrameReg);
  masm.moveStackPtrTo(BaselineFrameReg);
  masm.subFromStackPtr(Imm32(BaselineFrame::Size()));
  masm.assertStackAlignment(sizeof(Value), 0);

  // Store flags and env chain.
  masm.store32(Imm32(BaselineFrame::HAS_INITIAL_ENV), frame.addressOfFlags());
  masm.unboxObject(
      Address(genObj, GeneratorObject::offsetOfEnvironmentChainSlot()),
      scratch2);
  masm.storePtr(scratch2, frame.addressOfEnvironmentChain());

  // Store the arguments object if there is one.
  Label noArgsObj;
  Address argsObjSlot(genObj, GeneratorObject::offsetOfArgsObjSlot());
  masm.branchTestUndefined(Assembler::Equal, argsObjSlot, &noArgsObj);
  masm.unboxObject(argsObjSlot, scratch2);
  {
    masm.storePtr(scratch2, frame.addressOfArgsObj());
    masm.or32(Imm32(BaselineFrame::HAS_ARGS_OBJ), frame.addressOfFlags());
  }
  masm.bind(&noArgsObj);

  // Push expression slots if needed.
  Label noExprStack;
  Address exprStackSlot(genObj, GeneratorObject::offsetOfExpressionStackSlot());
  masm.branchTestNull(Assembler::Equal, exprStackSlot, &noExprStack);
  {
    masm.unboxObject(exprStackSlot, scratch2);

    Register initLength = regs.takeAny();
    masm.loadPtr(Address(scratch2, NativeObject::offsetOfElements()), scratch2);
    masm.load32(Address(scratch2, ObjectElements::offsetOfInitializedLength()),
                initLength);
    masm.store32(
        Imm32(0),
        Address(scratch2, ObjectElements::offsetOfInitializedLength()));
||||||| merged common ancestors
    // If profiler instrumentation is on, update lastProfilingFrame on
    // current JitActivation
    {
        Register scratchReg = scratch2;
        Label skip;
        AbsoluteAddress addressOfEnabled(cx->runtime()->geckoProfiler().addressOfEnabled());
        masm.branch32(Assembler::Equal, addressOfEnabled, Imm32(0), &skip);
        masm.loadJSContext(scratchReg);
        masm.loadPtr(Address(scratchReg, JSContext::offsetOfProfilingActivation()), scratchReg);
        masm.storeStackPtr(Address(scratchReg, JitActivation::offsetOfLastProfilingFrame()));
        masm.bind(&skip);
    }

    // Construct BaselineFrame.
    masm.push(BaselineFrameReg);
    masm.moveStackPtrTo(BaselineFrameReg);
    masm.subFromStackPtr(Imm32(BaselineFrame::Size()));
    masm.assertStackAlignment(sizeof(Value), 0);

    // Store flags and env chain.
    masm.store32(Imm32(BaselineFrame::HAS_INITIAL_ENV), frame.addressOfFlags());
    masm.unboxObject(Address(genObj, GeneratorObject::offsetOfEnvironmentChainSlot()), scratch2);
    masm.storePtr(scratch2, frame.addressOfEnvironmentChain());

    // Store the arguments object if there is one.
    Label noArgsObj;
    Address argsObjSlot(genObj, GeneratorObject::offsetOfArgsObjSlot());
    masm.branchTestUndefined(Assembler::Equal, argsObjSlot, &noArgsObj);
    masm.unboxObject(argsObjSlot, scratch2);
    {
        masm.storePtr(scratch2, frame.addressOfArgsObj());
        masm.or32(Imm32(BaselineFrame::HAS_ARGS_OBJ), frame.addressOfFlags());
    }
    masm.bind(&noArgsObj);
=======
  prepareVMCall();
  pushArg(R0);

  using Fn = bool (*)(JSContext*, HandleValue);
  return callVM<Fn, js::CheckClassHeritageOperation>();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    Label loop, loopDone;
    masm.bind(&loop);
    masm.branchTest32(Assembler::Zero, initLength, initLength, &loopDone);
    {
      masm.pushValue(Address(scratch2, 0));
      masm.guardedCallPreBarrier(Address(scratch2, 0), MIRType::Value);
      masm.addPtr(Imm32(sizeof(Value)), scratch2);
      masm.sub32(Imm32(1), initLength);
      masm.jump(&loop);
    }
    masm.bind(&loopDone);
    regs.add(initLength);
  }

  masm.bind(&noExprStack);
  masm.pushValue(retVal);

  masm.switchToObjectRealm(genObj, scratch2);

  if (resumeKind == GeneratorObject::NEXT) {
    // Determine the resume address based on the resumeIndex and the
    // resumeIndex -> native table in the BaselineScript.
    masm.load32(
        Address(scratch1, BaselineScript::offsetOfResumeEntriesOffset()),
        scratch2);
    masm.addPtr(scratch2, scratch1);
    masm.unboxInt32(Address(genObj, GeneratorObject::offsetOfResumeIndexSlot()),
                    scratch2);
    masm.loadPtr(
        BaseIndex(scratch1, scratch2, ScaleFromElemWidth(sizeof(uintptr_t))),
        scratch1);

    // Mark as running and jump to the generator's JIT code.
    masm.storeValue(
        Int32Value(GeneratorObject::RESUME_INDEX_RUNNING),
        Address(genObj, GeneratorObject::offsetOfResumeIndexSlot()));
    masm.jump(scratch1);
  } else {
    MOZ_ASSERT(resumeKind == GeneratorObject::THROW ||
               resumeKind == GeneratorObject::RETURN);

    // Update the frame's frameSize field.
    masm.computeEffectiveAddress(
        Address(BaselineFrameReg, BaselineFrame::FramePointerOffset), scratch2);
    masm.movePtr(scratch2, scratch1);
    masm.subStackPtrFrom(scratch2);
    masm.store32(scratch2, Address(BaselineFrameReg,
                                   BaselineFrame::reverseOffsetOfFrameSize()));
    masm.loadBaselineFramePtr(BaselineFrameReg, scratch2);
||||||| merged common ancestors
    // Push expression slots if needed.
    Label noExprStack;
    Address exprStackSlot(genObj, GeneratorObject::offsetOfExpressionStackSlot());
    masm.branchTestNull(Assembler::Equal, exprStackSlot, &noExprStack);
    {
        masm.unboxObject(exprStackSlot, scratch2);

        Register initLength = regs.takeAny();
        masm.loadPtr(Address(scratch2, NativeObject::offsetOfElements()), scratch2);
        masm.load32(Address(scratch2, ObjectElements::offsetOfInitializedLength()), initLength);
        masm.store32(Imm32(0), Address(scratch2, ObjectElements::offsetOfInitializedLength()));

        Label loop, loopDone;
        masm.bind(&loop);
        masm.branchTest32(Assembler::Zero, initLength, initLength, &loopDone);
        {
            masm.pushValue(Address(scratch2, 0));
            masm.guardedCallPreBarrier(Address(scratch2, 0), MIRType::Value);
            masm.addPtr(Imm32(sizeof(Value)), scratch2);
            masm.sub32(Imm32(1), initLength);
            masm.jump(&loop);
        }
        masm.bind(&loopDone);
        regs.add(initLength);
    }

    masm.bind(&noExprStack);
    masm.pushValue(retVal);

    masm.switchToObjectRealm(genObj, scratch2);

    if (resumeKind == GeneratorObject::NEXT) {
        // Determine the resume address based on the yieldAndAwaitIndex and the
        // yieldAndAwaitIndex -> native table in the BaselineScript.
        masm.load32(Address(scratch1, BaselineScript::offsetOfYieldEntriesOffset()), scratch2);
        masm.addPtr(scratch2, scratch1);
        masm.unboxInt32(Address(genObj, GeneratorObject::offsetOfYieldAndAwaitIndexSlot()),
                        scratch2);
        masm.loadPtr(BaseIndex(scratch1, scratch2, ScaleFromElemWidth(sizeof(uintptr_t))), scratch1);

        // Mark as running and jump to the generator's JIT code.
        masm.storeValue(Int32Value(GeneratorObject::YIELD_AND_AWAIT_INDEX_RUNNING),
                        Address(genObj, GeneratorObject::offsetOfYieldAndAwaitIndexSlot()));
        masm.jump(scratch1);
    } else {
        MOZ_ASSERT(resumeKind == GeneratorObject::THROW || resumeKind == GeneratorObject::RETURN);

        // Update the frame's frameSize field.
        masm.computeEffectiveAddress(Address(BaselineFrameReg, BaselineFrame::FramePointerOffset),
                                     scratch2);
        masm.movePtr(scratch2, scratch1);
        masm.subStackPtrFrom(scratch2);
        masm.store32(scratch2, Address(BaselineFrameReg, BaselineFrame::reverseOffsetOfFrameSize()));
        masm.loadBaselineFramePtr(BaselineFrameReg, scratch2);

        prepareVMCall();
        pushArg(Imm32(resumeKind));
        pushArg(retVal);
        pushArg(genObj);
        pushArg(scratch2);

        TrampolinePtr code = cx->runtime()->jitRuntime()->getVMWrapper(GeneratorThrowOrReturnInfo);

        // Create the frame descriptor.
        masm.subStackPtrFrom(scratch1);
        masm.makeFrameDescriptor(scratch1, FrameType::BaselineJS, ExitFrameLayout::Size());

        // Push the frame descriptor and a dummy return address (it doesn't
        // matter what we push here, frame iterators will use the frame pc
        // set in jit::GeneratorThrowOrReturn).
        masm.push(scratch1);

        // On ARM64, the callee will push the return address.
#ifndef JS_CODEGEN_ARM64
        masm.push(ImmWord(0));
#endif
        masm.jump(code);
    }

    // If the generator script has no JIT code, call into the VM.
    masm.bind(&interpret);
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITHOMEOBJECT() {
  // Load HomeObject in R0.
  frame.popRegsAndSync(1);

  // Load function off stack
  Register func = R2.scratchReg();
  masm.unboxObject(frame.addressOfStackValue(-1), func);

  // Set HOMEOBJECT_SLOT
  Register temp = R1.scratchReg();
  Address addr(func, FunctionExtended::offsetOfMethodHomeObjectSlot());
  masm.guardedCallPreBarrierAnyZone(addr, MIRType::Value, temp);
  masm.storeValue(R0, addr);

  Label skipBarrier;
  masm.branchPtrInNurseryChunk(Assembler::Equal, func, temp, &skipBarrier);
  masm.branchValueIsNurseryObject(Assembler::NotEqual, R0, temp, &skipBarrier);
  masm.call(&postBarrierSlot_);
  masm.bind(&skipBarrier);

  return true;
}

template <>
bool BaselineCompilerCodeGen::emit_JSOP_BUILTINPROTO() {
  // The builtin prototype is a constant for a given global.
  JSObject* builtin = BuiltinProtoOperation(cx, handler.pc());
  if (!builtin) {
    return false;
  }
  frame.push(ObjectValue(*builtin));
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    prepareVMCall();
    pushArg(Imm32(resumeKind));
    pushArg(retVal);
    pushArg(genObj);
    pushArg(scratch2);

    TrampolinePtr code =
        cx->runtime()->jitRuntime()->getVMWrapper(GeneratorThrowOrReturnInfo);

    // Create and push the frame descriptor.
    masm.subStackPtrFrom(scratch1);
    masm.makeFrameDescriptor(scratch1, FrameType::BaselineJS,
                             ExitFrameLayout::Size());
    masm.push(scratch1);

    // We have created a baseline frame as if we were the
    // callee. However, if we just did a regular call at this
    // point, our return address would be bogus: it would point at
    // self-hosted code, instead of the generator code that we are
    // pretending we are already executing. Instead, we push a
    // dummy return address. In jit::GeneratorThrowOrReturn,
    // we will set the baseline frame's overridePc. Frame iterators
    // will use the override pc instead of relying on the return
    // address.

    // On ARM64, the callee will push a bogus return address. On
    // other architectures, we push a null return address.
#ifndef JS_CODEGEN_ARM64
    masm.push(ImmWord(0));
#endif
    masm.jump(code);
  }

  // If the generator script has no JIT code, call into the VM.
  masm.bind(&interpret);

  prepareVMCall();
  if (resumeKind == GeneratorObject::NEXT) {
    pushArg(ImmGCPtr(cx->names().next));
  } else if (resumeKind == GeneratorObject::THROW) {
    pushArg(ImmGCPtr(cx->names().throw_));
  } else {
    MOZ_ASSERT(resumeKind == GeneratorObject::RETURN);
    pushArg(ImmGCPtr(cx->names().return_));
  }

  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), retVal);
  pushArg(retVal);
  pushArg(genObj);

  if (!callVM(InterpretResumeInfo)) {
    return false;
  }

  // After the generator returns, we restore the stack pointer, switch back to
  // the current realm, push the return value, and we're done.
  masm.bind(&returnTarget);
  masm.computeEffectiveAddress(frame.addressOfStackValue(frame.peek(-1)),
                               masm.getStackPointer());
  masm.switchToRealm(script->realm(), R2.scratchReg());
  frame.popn(2);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    prepareVMCall();
    if (resumeKind == GeneratorObject::NEXT) {
        pushArg(ImmGCPtr(cx->names().next));
    } else if (resumeKind == GeneratorObject::THROW) {
        pushArg(ImmGCPtr(cx->names().throw_));
    } else {
        MOZ_ASSERT(resumeKind == GeneratorObject::RETURN);
        pushArg(ImmGCPtr(cx->names().return_));
    }

    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), retVal);
    pushArg(retVal);
    pushArg(genObj);

    if (!callVM(InterpretResumeInfo)) {
        return false;
    }

    // After the generator returns, we restore the stack pointer, switch back to
    // the current realm, push the return value, and we're done.
    masm.bind(&returnTarget);
    masm.computeEffectiveAddress(frame.addressOfStackValue(frame.peek(-1)), masm.getStackPointer());
    masm.switchToRealm(script->realm(), R2.scratchReg());
    frame.popn(2);
    frame.push(R0);
    return true;
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_BUILTINPROTO() {
  prepareVMCall();

  pushBytecodePCArg();

  using Fn = JSObject* (*)(JSContext*, jsbytecode*);
  if (!callVM<Fn, BuiltinProtoOperation>()) {
    return false;
  }

  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*CheckSelfHostedFn)(JSContext*, HandleValue);
static const VMFunction CheckSelfHostedInfo = FunctionInfo<CheckSelfHostedFn>(
    js::Debug_CheckSelfHosted, "Debug_CheckSelfHosted");
||||||| merged common ancestors
typedef bool (*CheckSelfHostedFn)(JSContext*, HandleValue);
static const VMFunction CheckSelfHostedInfo =
    FunctionInfo<CheckSelfHostedFn>(js::Debug_CheckSelfHosted, "Debug_CheckSelfHosted");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_OBJWITHPROTO() {
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DEBUGCHECKSELFHOSTED() {
#ifdef DEBUG
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_DEBUGCHECKSELFHOSTED()
{
#ifdef DEBUG
    frame.syncStack(0);
=======
  // Leave the proto value on the stack for the decompiler
  masm.loadValue(frame.addressOfStackValue(-1), R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
||||||| merged common ancestors
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
=======
  prepareVMCall();
  pushArg(R0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(R0);
  if (!callVM(CheckSelfHostedInfo)) {
    return false;
  }
#endif
  return true;
||||||| merged common ancestors
    prepareVMCall();
    pushArg(R0);
    if (!callVM(CheckSelfHostedInfo)) {
        return false;
    }
#endif
    return true;

=======
  using Fn = JSObject* (*)(JSContext*, HandleValue);
  if (!callVM<Fn, js::ObjectWithProtoOperation>()) {
    return false;
  }

  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.pop();
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_IS_CONSTRUCTING() {
  frame.push(MagicValue(JS_IS_CONSTRUCTING));
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_IS_CONSTRUCTING()
{
    frame.push(MagicValue(JS_IS_CONSTRUCTING));
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FUNWITHPROTO() {
  frame.popRegsAndSync(1);

  masm.unboxObject(R0, R0.scratchReg());
  masm.loadPtr(frame.addressOfEnvironmentChain(), R1.scratchReg());

  prepareVMCall();
  pushArg(R0.scratchReg());
  pushArg(R1.scratchReg());
  pushScriptGCThingArg(ScriptGCThingType::Function, R0.scratchReg(),
                       R1.scratchReg());

  using Fn =
      JSObject* (*)(JSContext*, HandleFunction, HandleObject, HandleObject);
  if (!callVM<Fn, js::FunWithProtoOperation>()) {
    return false;
  }

  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_JUMPTARGET() {
  if (!script->hasScriptCounts()) {
    return true;
  }
  PCCounts* counts = script->maybeGetPCCounts(pc);
  uint64_t* counterAddr = &counts->numExec();
  masm.inc64(AbsoluteAddress(counterAddr));
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_JUMPTARGET()
{
    if (!script->hasScriptCounts()) {
        return true;
    }
    PCCounts* counts = script->maybeGetPCCounts(pc);
    uint64_t* counterAddr = &counts->numExec();
    masm.inc64(AbsoluteAddress(counterAddr));
    return true;
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CLASSCONSTRUCTOR() {
  frame.syncStack(0);

  // Pass nullptr as prototype to MakeDefaultConstructor
  prepareVMCall();
  pushArg(ImmPtr(nullptr));
  pushBytecodePCArg();
  pushScriptArg();

  using Fn =
      JSFunction* (*)(JSContext*, HandleScript, jsbytecode*, HandleObject);
  if (!callVM<Fn, js::MakeDefaultConstructor>()) {
    return false;
  }

  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DERIVEDCONSTRUCTOR() {
  frame.popRegsAndSync(1);

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CHECKCLASSHERITAGE() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CHECKCLASSHERITAGE()
{
    frame.syncStack(0);
=======
  masm.unboxObject(R0, R0.scratchReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Leave the heritage value on the stack.
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
||||||| merged common ancestors
    // Leave the heritage value on the stack.
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
=======
  prepareVMCall();
  pushArg(R0.scratchReg());
  pushBytecodePCArg();
  pushScriptArg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(R0);
  return callVM(CheckClassHeritageOperationInfo);
||||||| merged common ancestors
    prepareVMCall();
    pushArg(R0);
    return callVM(CheckClassHeritageOperationInfo);
=======
  using Fn =
      JSFunction* (*)(JSContext*, HandleScript, jsbytecode*, HandleObject);
  if (!callVM<Fn, js::MakeDefaultConstructor>()) {
    return false;
  }

  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_INITHOMEOBJECT() {
  // Load HomeObject in R0.
  frame.popRegsAndSync(1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_INITHOMEOBJECT()
{
    frame.syncStack(0);

    // Load HomeObject off stack
    unsigned skipOver = GET_UINT8(pc);
    MOZ_ASSERT(frame.stackDepth() >= skipOver + 2);
    masm.loadValue(frame.addressOfStackValue(frame.peek(-2 - skipOver)), R0);
=======
template <>
bool BaselineCompilerCodeGen::emit_JSOP_IMPORTMETA() {
  // Note: this is like the interpreter implementation, but optimized a bit by
  // calling GetModuleObjectForScript at compile-time.

  RootedModuleObject module(cx, GetModuleObjectForScript(handler.script()));
  MOZ_ASSERT(module);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load function off stack
  Register func = R2.scratchReg();
  masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), func);
||||||| merged common ancestors
    // Load function off stack
    Register func = R2.scratchReg();
    masm.unboxObject(frame.addressOfStackValue(frame.peek(-1)), func);
=======
  frame.syncStack(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Set HOMEOBJECT_SLOT
  Address addr(func, FunctionExtended::offsetOfMethodHomeObjectSlot());
  masm.guardedCallPreBarrier(addr, MIRType::Value);
  masm.storeValue(R0, addr);
||||||| merged common ancestors
    // Set HOMEOBJECT_SLOT
    Address addr(func, FunctionExtended::offsetOfMethodHomeObjectSlot());
    masm.guardedCallPreBarrier(addr, MIRType::Value);
    masm.storeValue(R0, addr);
=======
  prepareVMCall();
  pushArg(ImmGCPtr(module));
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register temp = R1.scratchReg();
  Label skipBarrier;
  masm.branchPtrInNurseryChunk(Assembler::Equal, func, temp, &skipBarrier);
  masm.branchValueIsNurseryObject(Assembler::NotEqual, R0, temp, &skipBarrier);
  masm.call(&postBarrierSlot_);
  masm.bind(&skipBarrier);
||||||| merged common ancestors
    Register temp = R1.scratchReg();
    Label skipBarrier;
    masm.branchPtrInNurseryChunk(Assembler::Equal, func, temp, &skipBarrier);
    masm.branchValueIsNurseryObject(Assembler::NotEqual, R0, temp, &skipBarrier);
    masm.call(&postBarrierSlot_);
    masm.bind(&skipBarrier);
=======
  using Fn = JSObject* (*)(JSContext*, HandleObject);
  if (!callVM<Fn, js::GetOrCreateModuleMetaObject>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_BUILTINPROTO() {
  // The builtin prototype is a constant for a given global.
  JSProtoKey key = static_cast<JSProtoKey>(GET_UINT8(pc));
  MOZ_ASSERT(key < JSProto_LIMIT);
  JSObject* builtin = GlobalObject::getOrCreatePrototype(cx, key);
  if (!builtin) {
    return false;
  }
  frame.push(ObjectValue(*builtin));
  return true;
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_BUILTINPROTO()
{
    // The builtin prototype is a constant for a given global.
    JSProtoKey key = static_cast<JSProtoKey>(GET_UINT8(pc));
    MOZ_ASSERT(key < JSProto_LIMIT);
    JSObject* builtin = GlobalObject::getOrCreatePrototype(cx, key);
    if (!builtin) {
        return false;
    }
    frame.push(ObjectValue(*builtin));
    return true;
=======
template <>
bool BaselineInterpreterCodeGen::emit_JSOP_IMPORTMETA() {
  prepareVMCall();

  pushScriptArg();

  using Fn = JSObject* (*)(JSContext*, HandleScript);
  if (!callVM<Fn, ImportMetaOperation>()) {
    return false;
  }

  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
>>>>>>> upstream-releases
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DYNAMIC_IMPORT() {
  // Put specifier value in R0.
  frame.popRegsAndSync(1);

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_OBJWITHPROTO() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_OBJWITHPROTO()
{
    frame.syncStack(0);
=======
  prepareVMCall();
  pushArg(R0);
  pushScriptArg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Leave the proto value on the stack for the decompiler
  masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
||||||| merged common ancestors
    // Leave the proto value on the stack for the decompiler
    masm.loadValue(frame.addressOfStackValue(frame.peek(-1)), R0);
=======
  using Fn = JSObject* (*)(JSContext*, HandleScript, HandleValue);
  if (!callVM<Fn, js::StartDynamicModuleImport>()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(R0);
  if (!callVM(ObjectWithProtoOperationInfo)) {
    return false;
  }
||||||| merged common ancestors
    prepareVMCall();
    pushArg(R0);
    if (!callVM(ObjectWithProtoOperationInfo)) {
        return false;
    }
=======
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
}

template <typename Handler>
bool BaselineCodeGen<Handler>::emitPrologue() {
#ifdef JS_USE_LINK_REGISTER
  // Push link register from generateEnterJIT()'s BLR.
  masm.pushReturnAddress();
  masm.checkStackAlignment();
#endif
  emitProfilerEnterFrame();

  masm.push(BaselineFrameReg);
  masm.moveStackPtrTo(BaselineFrameReg);
  masm.subFromStackPtr(Imm32(BaselineFrame::Size()));

  // Initialize BaselineFrame. For eval scripts, the env chain
  // is passed in R1, so we have to be careful not to clobber it.

  // Initialize BaselineFrame::flags and interpreter fields.
  emitInitFrameFields();

  // Handle env chain pre-initialization (in case GC gets run
  // during stack check).  For global and eval scripts, the env
  // chain is in R1.  For function scripts, the env chain is in
  // the callee, nullptr is stored for now so that GC doesn't choke
  // on a bogus EnvironmentChain value in the frame.
  emitPreInitEnvironmentChain(R1.scratchReg());

  if (!emitIncExecutionProgressCounter(R2.scratchReg())) {
    return false;
  }

  // Functions with a large number of locals require two stack checks.
  // The VMCall for a fallible stack check can only occur after the
  // env chain has been initialized, as that is required for proper
  // exception handling if the VMCall returns false.  The env chain
  // initialization can only happen after the UndefinedValues for the
  // local slots have been pushed. However by that time, the stack might
  // have grown too much.
  //
  // In these cases, we emit an extra, early, infallible check before pushing
  // the locals. The early check just sets a flag on the frame if the stack
  // check fails. If the flag is set, then the jitcode skips past the pushing
  // of the locals, and directly to env chain initialization followed by the
  // actual stack check, which will throw the correct exception.
  Label earlyStackCheckFailed;
  if (handler.needsEarlyStackCheck()) {
    // Subtract the size of script->nslots() from the stack pointer.
    Register scratch = R1.scratchReg();
    masm.moveStackPtrTo(scratch);
    subtractScriptSlotsSize(scratch, R2.scratchReg());

    // Set the OVER_RECURSED flag on the frame if the computed stack pointer
    // overflows the stack limit. We have to use the actual (*NoInterrupt)
    // stack limit here because we don't want to set the flag and throw an
    // overrecursion exception later in the interrupt case.
    Label stackCheckOk;
    masm.branchPtr(Assembler::BelowOrEqual,
                   AbsoluteAddress(cx->addressOfJitStackLimitNoInterrupt()),
                   scratch, &stackCheckOk);
    {
      masm.or32(Imm32(BaselineFrame::OVER_RECURSED), frame.addressOfFlags());
      masm.jump(&earlyStackCheckFailed);
    }
    masm.bind(&stackCheckOk);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.pop();
  frame.push(R0);
  return true;
||||||| merged common ancestors
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.pop();
    frame.push(R0);
    return true;
=======
  emitInitializeLocals();

  if (handler.needsEarlyStackCheck()) {
    masm.bind(&earlyStackCheckFailed);
  }

#ifdef JS_TRACE_LOGGING
  if (JS::TraceLoggerSupported() && !emitTraceLoggerEnter()) {
    return false;
  }
#endif

  // Record the offset of the prologue, because Ion can bailout before
  // the env chain is initialized.
  bailoutPrologueOffset_ = CodeOffset(masm.currentOffset());

  // When compiling with Debugger instrumentation, set the debuggeeness of
  // the frame before any operation that can call into the VM.
  emitIsDebuggeeCheck();

  // Initialize the env chain before any operation that may
  // call into the VM and trigger a GC.
  if (!initEnvironmentChain()) {
    return false;
  }

  frame.assertSyncedStack();

  if (JSScript* script = handler.maybeScript()) {
    masm.debugAssertContextRealm(script->realm(), R1.scratchReg());
  }

  if (!emitStackCheck()) {
    return false;
  }

  if (!emitDebugPrologue()) {
    return false;
  }

  if (!emitHandleCodeCoverageAtPrologue()) {
    return false;
  }

  if (!emitWarmUpCounterIncrement()) {
    return false;
  }

  warmUpCheckPrologueOffset_ = CodeOffset(masm.currentOffset());

  if (!emitArgumentTypeChecks()) {
    return false;
  }

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef JSObject* (*FunWithProtoFn)(JSContext*, HandleFunction, HandleObject,
                                    HandleObject);
static const VMFunction FunWithProtoInfo = FunctionInfo<FunWithProtoFn>(
    js::FunWithProtoOperation, "FunWithProtoOperation");
||||||| merged common ancestors
typedef JSObject* (*FunWithProtoFn)(JSContext*, HandleFunction, HandleObject, HandleObject);
static const VMFunction FunWithProtoInfo =
    FunctionInfo<FunWithProtoFn>(js::FunWithProtoOperation, "FunWithProtoOperation");
=======
template <typename Handler>
bool BaselineCodeGen<Handler>::emitEpilogue() {
  // Record the offset of the epilogue, so we can do early return from
  // Debugger handlers during on-stack recompile.
  debugOsrEpilogueOffset_ = CodeOffset(masm.currentOffset());
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_FUNWITHPROTO() {
  frame.popRegsAndSync(1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_FUNWITHPROTO()
{
    frame.popRegsAndSync(1);
=======
  masm.bind(&return_);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.unboxObject(R0, R0.scratchReg());
  masm.loadPtr(frame.addressOfEnvironmentChain(), R1.scratchReg());
||||||| merged common ancestors
    masm.unboxObject(R0, R0.scratchReg());
    masm.loadPtr(frame.addressOfEnvironmentChain(), R1.scratchReg());
=======
#ifdef JS_TRACE_LOGGING
  if (JS::TraceLoggerSupported() && !emitTraceLoggerExit()) {
    return false;
  }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(R0.scratchReg());
  pushArg(R1.scratchReg());
  pushArg(ImmGCPtr(script->getFunction(GET_UINT32_INDEX(pc))));
  if (!callVM(FunWithProtoInfo)) {
    return false;
  }
||||||| merged common ancestors
    prepareVMCall();
    pushArg(R0.scratchReg());
    pushArg(R1.scratchReg());
    pushArg(ImmGCPtr(script->getFunction(GET_UINT32_INDEX(pc))));
    if (!callVM(FunWithProtoInfo)) {
        return false;
    }
=======
  masm.moveToStackPtr(BaselineFrameReg);
  masm.pop(BaselineFrameReg);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.push(R0);
    return true;
=======
  emitProfilerExitFrame();

  masm.ret();
  return true;
>>>>>>> upstream-releases
}

MethodStatus BaselineCompiler::emitBody() {
  JSScript* script = handler.script();
  MOZ_ASSERT(handler.pc() == script->code());

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_CLASSCONSTRUCTOR() {
  frame.syncStack(0);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_CLASSCONSTRUCTOR()
{
    frame.syncStack(0);
=======
  bool lastOpUnreachable = false;
  uint32_t emittedOps = 0;
  mozilla::DebugOnly<jsbytecode*> prevpc = handler.pc();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Pass nullptr as prototype to MakeDefaultConstructor
  prepareVMCall();
  pushArg(ImmPtr(nullptr));
  pushArg(ImmPtr(pc));
  pushArg(ImmGCPtr(script));
  if (!callVM(MakeDefaultConstructorInfo)) {
    return false;
  }
||||||| merged common ancestors
    // Pass nullptr as prototype to MakeDefaultConstructor
    prepareVMCall();
    pushArg(ImmPtr(nullptr));
    pushArg(ImmPtr(pc));
    pushArg(ImmGCPtr(script));
    if (!callVM(MakeDefaultConstructorInfo)) {
        return false;
    }
=======
  while (true) {
    JSOp op = JSOp(*handler.pc());
    JitSpew(JitSpew_BaselineOp, "Compiling op @ %d: %s",
            int(script->pcToOffset(handler.pc())), CodeName[op]);

    BytecodeInfo* info = handler.analysis().maybeInfo(handler.pc());

    // Skip unreachable ops.
    if (!info) {
      // Test if last instructions and stop emitting in that case.
      handler.moveToNextPC();
      if (handler.pc() >= script->codeEnd()) {
        break;
      }

      lastOpUnreachable = true;
      prevpc = handler.pc();
      continue;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.push(R0);
    return true;
=======
    if (info->jumpTarget) {
      // Fully sync the stack if there are incoming jumps.
      frame.syncStack(0);
      frame.setStackDepth(info->stackDepth);
      masm.bind(handler.labelOf(handler.pc()));
    } else if (MOZ_UNLIKELY(compileDebugInstrumentation())) {
      // Also fully sync the stack if the debugger is enabled.
      frame.syncStack(0);
    } else {
      // At the beginning of any op, at most the top 2 stack-values are
      // unsynced.
      if (frame.stackDepth() > 2) {
        frame.syncStack(2);
      }
    }

    frame.assertValidState(*info);

    // Add a PC -> native mapping entry for the current op. These entries are
    // used when we need the native code address for a given pc, for instance
    // for bailouts from Ion, the debugger and exception handling. See
    // PCMappingIndexEntry for more information.
    bool addIndexEntry = (handler.pc() == script->code() || lastOpUnreachable ||
                          emittedOps > 100);
    if (addIndexEntry) {
      emittedOps = 0;
    }
    if (MOZ_UNLIKELY(!addPCMappingEntry(addIndexEntry))) {
      ReportOutOfMemory(cx);
      return Method_Error;
    }

    // Emit traps for breakpoints and step mode.
    if (MOZ_UNLIKELY(compileDebugInstrumentation()) && !emitDebugTrap()) {
      return Method_Error;
    }

    switch (op) {
      case JSOP_FORCEINTERPRETER:
        // Caller must have checked script->hasForceInterpreterOp().
      case JSOP_UNUSED71:
      case JSOP_UNUSED149:
      case JSOP_LIMIT:
        MOZ_CRASH("Unexpected op");

#define EMIT_OP(OP)                                            \
  case OP:                                                     \
    if (MOZ_UNLIKELY(!this->emit_##OP())) return Method_Error; \
    break;
        OPCODE_LIST(EMIT_OP)
#undef EMIT_OP
    }

    MOZ_ASSERT(masm.framePushed() == 0);

    // Test if last instructions and stop emitting in that case.
    handler.moveToNextPC();
    if (handler.pc() >= script->codeEnd()) {
      break;
    }

    emittedOps++;
    lastOpUnreachable = false;
#ifdef DEBUG
    prevpc = handler.pc();
#endif
  }

  MOZ_ASSERT(JSOp(*prevpc) == JSOP_RETRVAL);
  return Method_Compiled;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DERIVEDCONSTRUCTOR() {
  frame.popRegsAndSync(1);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_DERIVEDCONSTRUCTOR()
{
    frame.popRegsAndSync(1);
=======
bool BaselineInterpreterGenerator::emitDebugTrap() {
  JitRuntime* jrt = cx->runtime()->jitRuntime();
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.unboxObject(R0, R0.scratchReg());
||||||| merged common ancestors
    masm.unboxObject(R0, R0.scratchReg());
=======
  JitCode* handlerCode =
      jrt->debugTrapHandler(cx, DebugTrapHandlerKind::Interpreter);
  if (!handlerCode) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(R0.scratchReg());
  pushArg(ImmPtr(pc));
  pushArg(ImmGCPtr(script));
  if (!callVM(MakeDefaultConstructorInfo)) {
    return false;
  }
||||||| merged common ancestors
    prepareVMCall();
    pushArg(R0.scratchReg());
    pushArg(ImmPtr(pc));
    pushArg(ImmGCPtr(script));
    if (!callVM(MakeDefaultConstructorInfo)) {
        return false;
    }
=======
  CodeOffset offset = masm.toggledCall(handlerCode, /* enabled = */ false);
  if (!debugTrapOffsets_.append(offset.offset())) {
    ReportOutOfMemory(cx);
    return false;
  }

  return true;
}

// Register holding the bytecode pc during dispatch. This exists so the debug
// trap handler can reload the pc into this register when it's done.
static constexpr Register InterpreterPCRegAtDispatch =
    HasInterpreterPCReg() ? InterpreterPCReg : R0.scratchReg();

bool BaselineInterpreterGenerator::emitInterpreterLoop() {
  Register scratch1 = R0.scratchReg();
  Register scratch2 = R1.scratchReg();

  // Entry point for interpreting a bytecode op. No registers are live except
  // for InterpreterPCReg.
  masm.bind(handler.interpretOpWithPCRegLabel());

  // Emit a patchable call for debugger breakpoints/stepping.
  if (!emitDebugTrap()) {
    return false;
  }
  Label interpretOpAfterDebugTrap;
  masm.bind(&interpretOpAfterDebugTrap);

  // Load pc, bytecode op.
  Register pcReg = LoadBytecodePC(masm, scratch1);
  masm.load8ZeroExtend(Address(pcReg, 0), scratch1);

  // Jump to table[op].
  {
    CodeOffset label = masm.movWithPatch(ImmWord(uintptr_t(-1)), scratch2);
    if (!tableLabels_.append(label)) {
      return false;
    }
    BaseIndex pointer(scratch2, scratch1, ScalePointer);
    masm.branchToComputedAddress(pointer);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.push(R0);
    return true;
=======
  // At the end of each op, emit code to bump the pc and jump to the
  // next op (this is also known as a threaded interpreter).
  auto opEpilogue = [&](JSOp op, size_t opLength) -> bool {
    MOZ_ASSERT(masm.framePushed() == 0);

    if (!BytecodeFallsThrough(op)) {
      // Nothing to do.
      masm.assumeUnreachable("unexpected fall through");
      return true;
    }

    // Bump frame->interpreterICEntry if needed.
    if (BytecodeOpHasIC(op)) {
      frame.bumpInterpreterICEntry();
    }

    // Bump bytecode PC.
    if (HasInterpreterPCReg()) {
      MOZ_ASSERT(InterpreterPCRegAtDispatch == InterpreterPCReg);
      masm.addPtr(Imm32(opLength), InterpreterPCReg);
    } else {
      MOZ_ASSERT(InterpreterPCRegAtDispatch == scratch1);
      masm.loadPtr(frame.addressOfInterpreterPC(), InterpreterPCRegAtDispatch);
      masm.addPtr(Imm32(opLength), InterpreterPCRegAtDispatch);
      masm.storePtr(InterpreterPCRegAtDispatch, frame.addressOfInterpreterPC());
    }

    if (!emitDebugTrap()) {
      return false;
    }

    // Load the opcode, jump to table[op].
    masm.load8ZeroExtend(Address(InterpreterPCRegAtDispatch, 0), scratch1);
    CodeOffset label = masm.movWithPatch(ImmWord(uintptr_t(-1)), scratch2);
    if (!tableLabels_.append(label)) {
      return false;
    }
    BaseIndex pointer(scratch2, scratch1, ScalePointer);
    masm.branchToComputedAddress(pointer);
    return true;
  };

  // Emit code for each bytecode op.
  Label opLabels[JSOP_LIMIT];
#define EMIT_OP(OP)                     \
  {                                     \
    masm.bind(&opLabels[OP]);           \
    if (!this->emit_##OP()) {           \
      return false;                     \
    }                                   \
    if (!opEpilogue(OP, OP##_LENGTH)) { \
      return false;                     \
    }                                   \
  }
  OPCODE_LIST(EMIT_OP)
#undef EMIT_OP

  // External entry point to start interpreting bytecode ops. This is used for
  // things like exception handling and OSR.
  masm.bind(handler.interpretOpLabel());
  interpretOpOffset_ = masm.currentOffset();
  restoreInterpreterPCReg();
  masm.jump(handler.interpretOpWithPCRegLabel());

  // Second external entry point: this skips the debug trap for the first op
  // and is used by OSR.
  interpretOpNoDebugTrapOffset_ = masm.currentOffset();
  restoreInterpreterPCReg();
  masm.jump(&interpretOpAfterDebugTrap);

  // Emit code for JSOP_UNUSED* ops.
  Label invalidOp;
  masm.bind(&invalidOp);
  masm.assumeUnreachable("Invalid op");

  // Emit the table.
  masm.haltingAlign(sizeof(void*));

#if defined(JS_CODEGEN_ARM) || defined(JS_CODEGEN_ARM64)
  size_t numInstructions = JSOP_LIMIT * (sizeof(uintptr_t) / sizeof(uint32_t));
  AutoForbidPoolsAndNops afp(&masm, numInstructions);
#endif

  tableOffset_ = masm.currentOffset();

  for (size_t i = 0; i < JSOP_LIMIT; i++) {
    // Store a pointer to the code for the current op. If the op's label is not
    // bound it must be a JSOP_UNUSED* op and we use |invalidOp| instead.
    const Label& opLabel = opLabels[i];
    uint32_t opOffset = opLabel.bound() ? opLabel.offset() : invalidOp.offset();
    CodeLabel cl;
    masm.writeCodePointer(&cl);
    cl.target()->bind(opOffset);
    masm.addCodeLabel(cl);
  }

  return true;
>>>>>>> upstream-releases
}

void BaselineInterpreterGenerator::emitOutOfLineCodeCoverageInstrumentation() {
  masm.bind(handler.codeCoverageAtPrologueLabel());
#ifdef JS_USE_LINK_REGISTER
  masm.pushReturnAddress();
#endif

<<<<<<< HEAD
template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_IMPORTMETA() {
  RootedModuleObject module(cx, GetModuleObjectForScript(script));
  MOZ_ASSERT(module);
||||||| merged common ancestors
bool
BaselineCompiler::emit_JSOP_IMPORTMETA()
{
    RootedModuleObject module(cx, GetModuleObjectForScript(script));
    MOZ_ASSERT(module);
=======
  masm.Push(BaselineFrameReg);
  masm.setupUnalignedABICall(R0.scratchReg());
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
  masm.passABIArg(R0.scratchReg());
  masm.callWithABI(
      JS_FUNC_TO_DATA_PTR(void*, jit::HandleCodeCoverageAtPrologue));
  masm.Pop(BaselineFrameReg);
>>>>>>> upstream-releases

<<<<<<< HEAD
  frame.syncStack(0);
||||||| merged common ancestors
    frame.syncStack(0);
=======
  masm.ret();
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall();
  pushArg(ImmGCPtr(module));
  if (!callVM(GetOrCreateModuleMetaObjectInfo)) {
    return false;
  }
||||||| merged common ancestors
    prepareVMCall();
    pushArg(ImmGCPtr(module));
    if (!callVM(GetOrCreateModuleMetaObjectInfo)) {
        return false;
    }
=======
  masm.bind(handler.codeCoverageAtPCLabel());
#ifdef JS_USE_LINK_REGISTER
  masm.pushReturnAddress();
#endif

  saveInterpreterPCReg();

  masm.Push(BaselineFrameReg);
  masm.setupUnalignedABICall(R0.scratchReg());
  masm.loadBaselineFramePtr(BaselineFrameReg, R0.scratchReg());
  masm.passABIArg(R0.scratchReg());
  Register pcReg = LoadBytecodePC(masm, R2.scratchReg());
  masm.passABIArg(pcReg);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, jit::HandleCodeCoverageAtPC));
  masm.Pop(BaselineFrameReg);

  restoreInterpreterPCReg();
  masm.ret();
}

bool BaselineInterpreterGenerator::generate(BaselineInterpreter& interpreter) {
  if (!emitPrologue()) {
    return false;
  }

  if (!emitInterpreterLoop()) {
    return false;
  }

  if (!emitEpilogue()) {
    return false;
  }

  if (!emitOutOfLinePostBarrierSlot()) {
    return false;
  }

  emitOutOfLineCodeCoverageInstrumentation();

  {
    Linker linker(masm, "BaselineInterpreter");
    if (masm.oom()) {
      ReportOutOfMemory(cx);
      return false;
    }

    JitCode* code = linker.newCode(cx, CodeKind::Other);
    if (!code) {
      return false;
    }

    // Register BaselineInterpreter code with the profiler's JitCode table.
    {
      JitcodeGlobalEntry::BaselineInterpreterEntry entry;
      entry.init(code, code->raw(), code->rawEnd());

      JitcodeGlobalTable* globalTable =
          cx->runtime()->jitRuntime()->getJitcodeGlobalTable();
      if (!globalTable->addEntry(entry)) {
        ReportOutOfMemory(cx);
        return false;
      }

      code->setHasBytecodeMap();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
||||||| merged common ancestors
    masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
    frame.push(R0);
    return true;
=======
    // Patch loads now that we know the tableswitch base address.
    for (CodeOffset off : tableLabels_) {
      Assembler::PatchDataWithValueCheck(CodeLocationLabel(code, off),
                                         ImmPtr(code->raw() + tableOffset_),
                                         ImmPtr((void*)-1));
    }

#ifdef JS_ION_PERF
    writePerfSpewerJitCodeProfile(code, "BaselineInterpreter");
#endif

#ifdef MOZ_VTUNE
    vtune::MarkStub(code, "BaselineInterpreter");
#endif

    interpreter.init(code, interpretOpOffset_, interpretOpNoDebugTrapOffset_,
                     profilerEnterFrameToggleOffset_.offset(),
                     profilerExitFrameToggleOffset_.offset(),
                     handler.debuggeeCheckOffset().offset(),
                     std::move(debugTrapOffsets_),
                     std::move(handler.codeCoverageOffsets()));
  }

  if (cx->runtime()->geckoProfiler().enabled()) {
    interpreter.toggleProfilerInstrumentation(true);
  }

  if (coverage::IsLCovEnabled()) {
    interpreter.toggleCodeCoverageInstrumentationUnchecked(true);
  }

  return true;
}

JitCode* JitRuntime::generateDebugTrapHandler(JSContext* cx,
                                              DebugTrapHandlerKind kind) {
  StackMacroAssembler masm;

  AllocatableGeneralRegisterSet regs(GeneralRegisterSet::All());
  regs.takeUnchecked(BaselineFrameReg);
  regs.takeUnchecked(ICStubReg);
  if (HasInterpreterPCReg()) {
    regs.takeUnchecked(InterpreterPCReg);
  }
#ifdef JS_CODEGEN_ARM
  regs.takeUnchecked(BaselineSecondScratchReg);
  masm.setSecondScratchReg(BaselineSecondScratchReg);
#endif
  Register scratch1 = regs.takeAny();
  Register scratch2 = regs.takeAny();
  Register scratch3 = regs.takeAny();

  if (kind == DebugTrapHandlerKind::Interpreter) {
    // The interpreter calls this for every script when debugging, so check if
    // the script has any breakpoints or is in step mode before calling into
    // C++.
    Label hasDebugScript;
    Address scriptAddr(BaselineFrameReg,
                       BaselineFrame::reverseOffsetOfInterpreterScript());
    masm.loadPtr(scriptAddr, scratch1);
    masm.branchTest32(Assembler::NonZero,
                      Address(scratch1, JSScript::offsetOfMutableFlags()),
                      Imm32(int32_t(JSScript::MutableFlags::HasDebugScript)),
                      &hasDebugScript);
    masm.abiret();
    masm.bind(&hasDebugScript);

    if (HasInterpreterPCReg()) {
      // Update frame's bytecode pc because the debugger depends on it.
      Address pcAddr(BaselineFrameReg,
                     BaselineFrame::reverseOffsetOfInterpreterPC());
      masm.storePtr(InterpreterPCReg, pcAddr);
    }
  }

  // Load the return address in scratch1.
  masm.loadAbiReturnAddress(scratch1);

  // Load BaselineFrame pointer in scratch2.
  masm.loadBaselineFramePtr(BaselineFrameReg, scratch2);

  // Enter a stub frame and call the HandleDebugTrap VM function. Ensure
  // the stub frame has a nullptr ICStub pointer, since this pointer is marked
  // during GC.
  masm.movePtr(ImmPtr(nullptr), ICStubReg);
  EmitBaselineEnterStubFrame(masm, scratch3);

  using Fn = bool (*)(JSContext*, BaselineFrame*, uint8_t*, bool*);
  VMFunctionId id = VMFunctionToId<Fn, jit::HandleDebugTrap>::id;
  TrampolinePtr code = cx->runtime()->jitRuntime()->getVMWrapper(id);

  masm.push(scratch1);
  masm.push(scratch2);
  EmitBaselineCallVM(code, masm);

  EmitBaselineLeaveStubFrame(masm);

  // If the stub returns |true|, we have to perform a forced return
  // (return from the JS frame). If the stub returns |false|, just return
  // from the trap stub so that execution continues at the current pc.
  Label forcedReturn;
  masm.branchIfTrueBool(ReturnReg, &forcedReturn);

  if (kind == DebugTrapHandlerKind::Interpreter) {
    // We have to reload the bytecode pc register.
    Address pcAddr(BaselineFrameReg,
                   BaselineFrame::reverseOffsetOfInterpreterPC());
    masm.loadPtr(pcAddr, InterpreterPCRegAtDispatch);
  }
  masm.abiret();

  masm.bind(&forcedReturn);
  masm.loadValue(
      Address(BaselineFrameReg, BaselineFrame::reverseOffsetOfReturnValue()),
      JSReturnOperand);
  masm.moveToStackPtr(BaselineFrameReg);
  masm.pop(BaselineFrameReg);

  // Before returning, if profiling is turned on, make sure that
  // lastProfilingFrame is set to the correct caller frame.
  {
    Label skipProfilingInstrumentation;
    AbsoluteAddress addressOfEnabled(
        cx->runtime()->geckoProfiler().addressOfEnabled());
    masm.branch32(Assembler::Equal, addressOfEnabled, Imm32(0),
                  &skipProfilingInstrumentation);
    masm.profilerExitFrame();
    masm.bind(&skipProfilingInstrumentation);
  }

  masm.ret();

  Linker linker(masm, "DebugTrapHandler");
  JitCode* handlerCode = linker.newCode(cx, CodeKind::Other);
  if (!handlerCode) {
    return nullptr;
  }

#ifdef JS_ION_PERF
  writePerfSpewerJitCodeProfile(handlerCode, "DebugTrapHandler");
#endif
#ifdef MOZ_VTUNE
  vtune::MarkStub(handlerCode, "DebugTrapHandler");
#endif

  return handlerCode;
>>>>>>> upstream-releases
}
<<<<<<< HEAD

typedef JSObject* (*StartDynamicModuleImportFn)(JSContext*, HandleValue,
                                                HandleValue);
static const VMFunction StartDynamicModuleImportInfo =
    FunctionInfo<StartDynamicModuleImportFn>(js::StartDynamicModuleImport,
                                             "StartDynamicModuleImport");

template <typename Handler>
bool BaselineCodeGen<Handler>::emit_JSOP_DYNAMIC_IMPORT() {
  RootedValue referencingPrivate(cx,
                                 FindScriptOrModulePrivateForScript(script));

  // Put specifier value in R0.
  frame.popRegsAndSync(1);

  prepareVMCall();
  pushArg(R0);
  pushArg(referencingPrivate);
  if (!callVM(StartDynamicModuleImportInfo)) {
    return false;
  }

  masm.tagValue(JSVAL_TYPE_OBJECT, ReturnReg, R0);
  frame.push(R0);
  return true;
}

// Instantiate explicitly for now to make sure it compiles.
template class jit::BaselineCodeGen<BaselineInterpreterHandler>;

}  // namespace jit
}  // namespace js
||||||| merged common ancestors
=======

}  // namespace jit
}  // namespace js
>>>>>>> upstream-releases
