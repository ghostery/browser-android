/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "vm/TraceLogging.h"

#include "mozilla/EndianUtils.h"
#include "mozilla/MemoryReporting.h"
#include "mozilla/ScopeExit.h"

#include <algorithm>
#include <string.h>
#include <utility>

#include "jit/BaselineJIT.h"
#include "jit/CompileWrappers.h"
#include "jit/JitSpewer.h"
#include "js/Printf.h"
#include "js/TraceLoggerAPI.h"
#include "threading/LockGuard.h"
#include "util/Text.h"
#include "vm/JSScript.h"
#include "vm/Runtime.h"
#include "vm/Time.h"
#include "vm/TraceLoggingGraph.h"

#include "jit/JitFrames-inl.h"

using namespace js;

static TraceLoggerThreadState* traceLoggerState = nullptr;

<<<<<<< HEAD
static bool EnsureTraceLoggerState() {
  if (MOZ_LIKELY(traceLoggerState)) {
    return true;
  }
||||||| merged common ancestors
#if defined(MOZ_HAVE_RDTSC)
=======
static bool getTraceLoggerSupported() {
  char* str = getenv("JS_TRACE_LOGGING");
>>>>>>> upstream-releases

<<<<<<< HEAD
  traceLoggerState = js_new<TraceLoggerThreadState>();
  if (!traceLoggerState) {
    return false;
  }
||||||| merged common ancestors
uint64_t inline rdtsc() {
    return ReadTimestampCounter();
}
=======
  if (!str) {
    // Default to unsupported.
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!traceLoggerState->init()) {
    DestroyTraceLoggerThreadState();
    return false;
  }
||||||| merged common ancestors
#elif defined(__powerpc__)
static __inline__ uint64_t
rdtsc(void)
{
    uint64_t result=0;
    uint32_t upper, lower,tmp;
    __asm__ volatile(
            "0:                  \n"
            "\tmftbu   %0           \n"
            "\tmftb    %1           \n"
            "\tmftbu   %2           \n"
            "\tcmpw    %2,%0        \n"
            "\tbne     0b         \n"
            : "=r"(upper),"=r"(lower),"=r"(tmp)
            );
    result = upper;
    result = result<<32;
    result = result|lower;
=======
  if (strcmp(str, "false") == 0 || strcmp(str, "no") == 0 ||
      strcmp(str, "0") == 0) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    return result;
=======
  if (strcmp(str, "true") == 0 || strcmp(str, "yes") == 0 ||
      strcmp(str, "1") == 0) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
size_t js::SizeOfTraceLogState(mozilla::MallocSizeOf mallocSizeOf) {
  return traceLoggerState ? traceLoggerState->sizeOfIncludingThis(mallocSizeOf)
                          : 0;
||||||| merged common ancestors
=======
  fprintf(stderr, "Warning: I didn't understand JS_TRACE_LOGGING=\"%s\"\n",
          str);
  return false;
>>>>>>> upstream-releases
}
<<<<<<< HEAD

void js::ResetTraceLogger() {
  if (!traceLoggerState) {
    return;
  }
||||||| merged common ancestors
#elif defined(__arm__) || defined(__aarch64__)

#include <sys/time.h>
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
  traceLoggerState->clear();
||||||| merged common ancestors
static __inline__ uint64_t
rdtsc(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t ret = tv.tv_sec;
    ret *= 1000000;
    ret += tv.tv_usec;
    return ret;
=======
size_t js::SizeOfTraceLogState(mozilla::MallocSizeOf mallocSizeOf) {
  return traceLoggerState ? traceLoggerState->sizeOfIncludingThis(mallocSizeOf)
                          : 0;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void js::DestroyTraceLoggerThreadState() {
  if (traceLoggerState) {
    js_delete(traceLoggerState);
    traceLoggerState = nullptr;
  }
||||||| merged common ancestors
#else

uint64_t inline
rdtsc(void)
{
    return 0;
=======
void js::ResetTraceLogger() {
  if (!traceLoggerState) {
    return;
  }

  traceLoggerState->clear();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
#ifdef DEBUG
bool js::CurrentThreadOwnsTraceLoggerThreadStateLock() {
  return traceLoggerState && traceLoggerState->lock.ownedByCurrentThread();
}
#endif
||||||| merged common ancestors
#endif // defined(MOZ_HAVE_RDTSC)
=======
void js::DestroyTraceLoggerThreadState() {
  if (traceLoggerState) {
    js_delete(traceLoggerState);
    traceLoggerState = nullptr;
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
void js::DestroyTraceLogger(TraceLoggerThread* logger) {
  if (!EnsureTraceLoggerState()) {
    return;
  }
  traceLoggerState->destroyLogger(logger);
}
||||||| merged common ancestors
static bool
EnsureTraceLoggerState()
{
    if (MOZ_LIKELY(traceLoggerState)) {
        return true;
    }
=======
#ifdef DEBUG
bool js::CurrentThreadOwnsTraceLoggerThreadStateLock() {
  return traceLoggerState && traceLoggerState->lock.ownedByCurrentThread();
}
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
bool TraceLoggerThread::init() {
  if (!events.init()) {
    return false;
  }
||||||| merged common ancestors
    traceLoggerState = js_new<TraceLoggerThreadState>();
    if (!traceLoggerState) {
        return false;
    }
=======
void js::DestroyTraceLogger(TraceLoggerThread* logger) {
  MOZ_ASSERT(traceLoggerState);
  traceLoggerState->destroyLogger(logger);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Minimum amount of capacity needed for operation to allow flushing.
  // Flushing requires space for the actual event and two spaces to log the
  // start and stop of flushing.
  if (!events.ensureSpaceBeforeAdd(3)) {
    return false;
  }

  return true;
}

void TraceLoggerThread::initGraph() {
  // Create a graph. I don't like this is called reset, but it locks the
  // graph into the UniquePtr. So it gets deleted when TraceLoggerThread
  // is destructed.
  graph.reset(js_new<TraceLoggerGraph>());
  if (!graph.get()) {
    return;
  }

  MOZ_ASSERT(traceLoggerState);
  bool graphFile = traceLoggerState->isGraphFileEnabled();
  double delta =
      traceLoggerState->getTimeStampOffset(mozilla::TimeStamp::Now());
  uint64_t start = static_cast<uint64_t>(delta);
  if (!graph->init(start, graphFile)) {
    graph = nullptr;
    return;
  }

  if (graphFile) {
    // Report the textIds to the graph.
    for (uint32_t i = 0; i < TraceLogger_TreeItemEnd; i++) {
      TraceLoggerTextId id = TraceLoggerTextId(i);
      graph->addTextId(i, TLTextIdString(id));
    }
    graph->addTextId(TraceLogger_TreeItemEnd, "TraceLogger internal");
    for (uint32_t i = TraceLogger_TreeItemEnd + 1; i < TraceLogger_Last; i++) {
      TraceLoggerTextId id = TraceLoggerTextId(i);
      graph->addTextId(i, TLTextIdString(id));
    }
  }
}

void TraceLoggerThreadState::disableAllTextIds() {
  for (uint32_t i = 1; i < TraceLogger_Last; i++) {
    enabledTextIds[i] = false;
  }
}

void TraceLoggerThreadState::enableTextIdsForProfiler() {
  enableDefaultLogging();
}

void TraceLoggerThreadState::disableTextIdsForProfiler() {
  disableAllTextIds();
  // We have to keep the Baseline and IonMonkey id's alive because they control
  // whether the jitted codegen has tracelogger start & stop events builtin.
  // Otherwise, we end up in situations when some jitted code that was created
  // before the profiler was even started ends up not starting and stoping any
  // events.  The TraceLogger_Engine stop events can accidentally stop the wrong
  // event in this case, and then it's no longer possible to build a graph.
  enabledTextIds[TraceLogger_Engine] = true;
  enabledTextIds[TraceLogger_Interpreter] = true;
  enabledTextIds[TraceLogger_Baseline] = true;
  enabledTextIds[TraceLogger_IonMonkey] = true;
}

void TraceLoggerThreadState::enableDefaultLogging() {
  enabledTextIds[TraceLogger_AnnotateScripts] = true;
  enabledTextIds[TraceLogger_Bailout] = true;
  enabledTextIds[TraceLogger_Baseline] = true;
  enabledTextIds[TraceLogger_BaselineCompilation] = true;
  enabledTextIds[TraceLogger_GC] = true;
  enabledTextIds[TraceLogger_GCAllocation] = true;
  enabledTextIds[TraceLogger_GCSweeping] = true;
  enabledTextIds[TraceLogger_Interpreter] = true;
  enabledTextIds[TraceLogger_IonAnalysis] = true;
  enabledTextIds[TraceLogger_IonCompilation] = true;
  enabledTextIds[TraceLogger_IonLinking] = true;
  enabledTextIds[TraceLogger_IonMonkey] = true;
  enabledTextIds[TraceLogger_MinorGC] = true;
  enabledTextIds[TraceLogger_Frontend] = true;
  enabledTextIds[TraceLogger_ParsingFull] = true;
  enabledTextIds[TraceLogger_ParsingSyntax] = true;
  enabledTextIds[TraceLogger_BytecodeEmission] = true;
  enabledTextIds[TraceLogger_IrregexpCompile] = true;
  enabledTextIds[TraceLogger_IrregexpExecute] = true;
  enabledTextIds[TraceLogger_Scripts] = true;
  enabledTextIds[TraceLogger_Engine] = true;
  enabledTextIds[TraceLogger_WasmCompilation] = true;
  enabledTextIds[TraceLogger_Interpreter] = true;
  enabledTextIds[TraceLogger_Baseline] = true;
  enabledTextIds[TraceLogger_IonMonkey] = true;
}

void TraceLoggerThreadState::enableIonLogging() {
  enabledTextIds[TraceLogger_IonCompilation] = true;
  enabledTextIds[TraceLogger_IonLinking] = true;
  enabledTextIds[TraceLogger_PruneUnusedBranches] = true;
  enabledTextIds[TraceLogger_FoldTests] = true;
  enabledTextIds[TraceLogger_SplitCriticalEdges] = true;
  enabledTextIds[TraceLogger_RenumberBlocks] = true;
  enabledTextIds[TraceLogger_ScalarReplacement] = true;
  enabledTextIds[TraceLogger_DominatorTree] = true;
  enabledTextIds[TraceLogger_PhiAnalysis] = true;
  enabledTextIds[TraceLogger_MakeLoopsContiguous] = true;
  enabledTextIds[TraceLogger_ApplyTypes] = true;
  enabledTextIds[TraceLogger_EagerSimdUnbox] = true;
  enabledTextIds[TraceLogger_AliasAnalysis] = true;
  enabledTextIds[TraceLogger_GVN] = true;
  enabledTextIds[TraceLogger_LICM] = true;
  enabledTextIds[TraceLogger_Sincos] = true;
  enabledTextIds[TraceLogger_RangeAnalysis] = true;
  enabledTextIds[TraceLogger_LoopUnrolling] = true;
  enabledTextIds[TraceLogger_FoldLinearArithConstants] = true;
  enabledTextIds[TraceLogger_EffectiveAddressAnalysis] = true;
  enabledTextIds[TraceLogger_AlignmentMaskAnalysis] = true;
  enabledTextIds[TraceLogger_EliminateDeadCode] = true;
  enabledTextIds[TraceLogger_ReorderInstructions] = true;
  enabledTextIds[TraceLogger_EdgeCaseAnalysis] = true;
  enabledTextIds[TraceLogger_EliminateRedundantChecks] = true;
  enabledTextIds[TraceLogger_AddKeepAliveInstructions] = true;
  enabledTextIds[TraceLogger_GenerateLIR] = true;
  enabledTextIds[TraceLogger_RegisterAllocation] = true;
  enabledTextIds[TraceLogger_GenerateCode] = true;
  enabledTextIds[TraceLogger_Scripts] = true;
  enabledTextIds[TraceLogger_IonBuilderRestartLoop] = true;
}

void TraceLoggerThreadState::enableFrontendLogging() {
  enabledTextIds[TraceLogger_Frontend] = true;
  enabledTextIds[TraceLogger_ParsingFull] = true;
  enabledTextIds[TraceLogger_ParsingSyntax] = true;
  enabledTextIds[TraceLogger_BytecodeEmission] = true;
  enabledTextIds[TraceLogger_BytecodeFoldConstants] = true;
  enabledTextIds[TraceLogger_BytecodeNameFunctions] = true;
}

TraceLoggerThread::~TraceLoggerThread() {
  if (graph.get()) {
    if (!failed) {
      graph->log(events, traceLoggerState->startTime);
    }
    graph = nullptr;
  }
}
||||||| merged common ancestors
    if (!traceLoggerState->init()) {
        DestroyTraceLoggerThreadState();
        return false;
    }
=======
bool TraceLoggerThread::init() {
  if (!events_.init()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool TraceLoggerThread::enable() {
  if (enabled_ > 0) {
    enabled_++;
||||||| merged common ancestors
=======
  // Minimum amount of capacity needed for operation to allow flushing.
  // Flushing requires space for the actual event and two spaces to log the
  // start and stop of flushing.
  if (!events_.ensureSpaceBeforeAdd(3)) {
    return false;
  }

  char* buffer = js_pod_malloc<char>(32);
  js::ThisThread::GetName(buffer, 32);
  threadName_.reset(buffer);

  return true;
}

void TraceLoggerThread::initGraph() {
  // Create a graph. I don't like this is called reset, but it locks the
  // graph into the UniquePtr. So it gets deleted when TraceLoggerThread
  // is destructed.
  graph_.reset(js_new<TraceLoggerGraph>());
  if (!graph_.get()) {
    return;
  }

  MOZ_ASSERT(traceLoggerState);
  bool graphFile = traceLoggerState->isGraphFileEnabled();
  double delta =
      traceLoggerState->getTimeStampOffset(mozilla::TimeStamp::NowUnfuzzed());
  uint64_t start = static_cast<uint64_t>(delta);
  if (!graph_->init(start, graphFile)) {
    graph_ = nullptr;
    return;
  }

  if (graphFile) {
    // Report the textIds to the graph.
    for (uint32_t i = 0; i < TraceLogger_TreeItemEnd; i++) {
      TraceLoggerTextId id = TraceLoggerTextId(i);
      graph_->addTextId(i, TLTextIdString(id));
    }
    graph_->addTextId(TraceLogger_TreeItemEnd, "TraceLogger internal");
    for (uint32_t i = TraceLogger_TreeItemEnd + 1; i < TraceLogger_Last; i++) {
      TraceLoggerTextId id = TraceLoggerTextId(i);
      graph_->addTextId(i, TLTextIdString(id));
    }
  }
}

void TraceLoggerThreadState::disableAllTextIds() {
  for (uint32_t i = 1; i < TraceLogger_Last; i++) {
    enabledTextIds[i] = false;
  }
}

void TraceLoggerThreadState::enableTextIdsForProfiler() {
  enableDefaultLogging();
}

void TraceLoggerThreadState::disableTextIdsForProfiler() {
  disableAllTextIds();
  // We have to keep the Baseline and IonMonkey id's alive because they control
  // whether the jitted codegen has tracelogger start & stop events builtin.
  // Otherwise, we end up in situations when some jitted code that was created
  // before the profiler was even started ends up not starting and stoping any
  // events.  The TraceLogger_Engine stop events can accidentally stop the wrong
  // event in this case, and then it's no longer possible to build a graph.
  enabledTextIds[TraceLogger_Engine] = true;
  enabledTextIds[TraceLogger_Interpreter] = true;
  enabledTextIds[TraceLogger_Baseline] = true;
  enabledTextIds[TraceLogger_IonMonkey] = true;
}

void TraceLoggerThreadState::enableDefaultLogging() {
  enabledTextIds[TraceLogger_AnnotateScripts] = true;
  enabledTextIds[TraceLogger_Bailout] = true;
  enabledTextIds[TraceLogger_Baseline] = true;
  enabledTextIds[TraceLogger_BaselineCompilation] = true;
  enabledTextIds[TraceLogger_GC] = true;
  enabledTextIds[TraceLogger_GCAllocation] = true;
  enabledTextIds[TraceLogger_GCSweeping] = true;
  enabledTextIds[TraceLogger_Interpreter] = true;
  enabledTextIds[TraceLogger_IonAnalysis] = true;
  enabledTextIds[TraceLogger_IonCompilation] = true;
  enabledTextIds[TraceLogger_IonLinking] = true;
  enabledTextIds[TraceLogger_IonMonkey] = true;
  enabledTextIds[TraceLogger_MinorGC] = true;
  enabledTextIds[TraceLogger_Frontend] = true;
  enabledTextIds[TraceLogger_ParsingFull] = true;
  enabledTextIds[TraceLogger_ParsingSyntax] = true;
  enabledTextIds[TraceLogger_BytecodeEmission] = true;
  enabledTextIds[TraceLogger_IrregexpCompile] = true;
  enabledTextIds[TraceLogger_IrregexpExecute] = true;
  enabledTextIds[TraceLogger_Scripts] = true;
  enabledTextIds[TraceLogger_Engine] = true;
  enabledTextIds[TraceLogger_WasmCompilation] = true;
  enabledTextIds[TraceLogger_Invalidation] = true;
}

void TraceLoggerThreadState::enableIonLogging() {
  enabledTextIds[TraceLogger_IonCompilation] = true;
  enabledTextIds[TraceLogger_IonLinking] = true;
  enabledTextIds[TraceLogger_PruneUnusedBranches] = true;
  enabledTextIds[TraceLogger_FoldTests] = true;
  enabledTextIds[TraceLogger_SplitCriticalEdges] = true;
  enabledTextIds[TraceLogger_RenumberBlocks] = true;
  enabledTextIds[TraceLogger_ScalarReplacement] = true;
  enabledTextIds[TraceLogger_DominatorTree] = true;
  enabledTextIds[TraceLogger_PhiAnalysis] = true;
  enabledTextIds[TraceLogger_MakeLoopsContiguous] = true;
  enabledTextIds[TraceLogger_ApplyTypes] = true;
  enabledTextIds[TraceLogger_EagerSimdUnbox] = true;
  enabledTextIds[TraceLogger_AliasAnalysis] = true;
  enabledTextIds[TraceLogger_GVN] = true;
  enabledTextIds[TraceLogger_LICM] = true;
  enabledTextIds[TraceLogger_RangeAnalysis] = true;
  enabledTextIds[TraceLogger_LoopUnrolling] = true;
  enabledTextIds[TraceLogger_FoldLinearArithConstants] = true;
  enabledTextIds[TraceLogger_EffectiveAddressAnalysis] = true;
  enabledTextIds[TraceLogger_AlignmentMaskAnalysis] = true;
  enabledTextIds[TraceLogger_EliminateDeadCode] = true;
  enabledTextIds[TraceLogger_ReorderInstructions] = true;
  enabledTextIds[TraceLogger_EdgeCaseAnalysis] = true;
  enabledTextIds[TraceLogger_EliminateRedundantChecks] = true;
  enabledTextIds[TraceLogger_AddKeepAliveInstructions] = true;
  enabledTextIds[TraceLogger_GenerateLIR] = true;
  enabledTextIds[TraceLogger_RegisterAllocation] = true;
  enabledTextIds[TraceLogger_GenerateCode] = true;
  enabledTextIds[TraceLogger_Scripts] = true;
  enabledTextIds[TraceLogger_IonBuilderRestartLoop] = true;
}

void TraceLoggerThreadState::enableFrontendLogging() {
  enabledTextIds[TraceLogger_Frontend] = true;
  enabledTextIds[TraceLogger_ParsingFull] = true;
  enabledTextIds[TraceLogger_ParsingSyntax] = true;
  enabledTextIds[TraceLogger_BytecodeEmission] = true;
  enabledTextIds[TraceLogger_BytecodeFoldConstants] = true;
  enabledTextIds[TraceLogger_BytecodeNameFunctions] = true;
}

TraceLoggerThread::~TraceLoggerThread() {
  if (graph_.get()) {
    if (!failed_) {
      graph_->log(events_, traceLoggerState->startTime);
    }
    graph_ = nullptr;
  }
}

bool TraceLoggerThread::enable() {
  if (enabled_ > 0) {
    enabled_++;
>>>>>>> upstream-releases
    return true;
<<<<<<< HEAD
  }
||||||| merged common ancestors
}
=======
  }

  if (failed_) {
    return false;
  }

  enabled_ = 1;
  logTimestamp(TraceLogger_Enable);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (failed) {
    return false;
  }

  enabled_ = 1;
  logTimestamp(TraceLogger_Enable);

  return true;
||||||| merged common ancestors
size_t
js::SizeOfTraceLogState(mozilla::MallocSizeOf mallocSizeOf)
{
    return traceLoggerState ? traceLoggerState->sizeOfIncludingThis(mallocSizeOf) : 0;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool TraceLoggerThread::fail(JSContext* cx, const char* error) {
  JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                            JSMSG_TRACELOGGER_ENABLE_FAIL, error);
  failed = true;
  enabled_ = 0;
||||||| merged common ancestors
void
js::ResetTraceLogger()
{
    if (!traceLoggerState) {
        return;
    }
=======
bool TraceLoggerThread::fail(JSContext* cx, const char* error) {
  JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                            JSMSG_TRACELOGGER_ENABLE_FAIL, error);
  failed_ = true;
  enabled_ = 0;
>>>>>>> upstream-releases

  return false;
}

<<<<<<< HEAD
void TraceLoggerThread::silentFail(const char* error) {
  traceLoggerState->maybeSpewError(error);
  failed = true;
  enabled_ = 0;
||||||| merged common ancestors
void
js::DestroyTraceLoggerThreadState()
{
    if (traceLoggerState) {
        js_delete(traceLoggerState);
        traceLoggerState = nullptr;
    }
=======
void TraceLoggerThread::silentFail(const char* error) {
  traceLoggerState->maybeSpewError(error);
  failed_ = true;
  enabled_ = 0;
>>>>>>> upstream-releases
}

size_t TraceLoggerThread::sizeOfExcludingThis(
    mozilla::MallocSizeOf mallocSizeOf) const {
  size_t size = 0;
#ifdef DEBUG
<<<<<<< HEAD
  size += graphStack.sizeOfExcludingThis(mallocSizeOf);
||||||| merged common ancestors
bool
js::CurrentThreadOwnsTraceLoggerThreadStateLock()
{
    return traceLoggerState && traceLoggerState->lock.ownedByCurrentThread();
}
=======
  size += graphStack_.sizeOfExcludingThis(mallocSizeOf);
>>>>>>> upstream-releases
#endif
<<<<<<< HEAD
  size += events.sizeOfExcludingThis(mallocSizeOf);
  if (graph.get()) {
    size += graph->sizeOfIncludingThis(mallocSizeOf);
  }
  return size;
}
||||||| merged common ancestors
=======
  size += events_.sizeOfExcludingThis(mallocSizeOf);
  if (graph_.get()) {
    size += graph_->sizeOfIncludingThis(mallocSizeOf);
  }
  return size;
}
>>>>>>> upstream-releases

size_t TraceLoggerThread::sizeOfIncludingThis(
    mozilla::MallocSizeOf mallocSizeOf) const {
  return mallocSizeOf(this) + sizeOfExcludingThis(mallocSizeOf);
}

bool TraceLoggerThread::enable(JSContext* cx) {
  using namespace js::jit;

  if (!enable()) {
    return fail(cx, "internal error");
  }

  if (enabled_ == 1) {
    // Get the top Activation to log the top script/pc (No inlined frames).
    ActivationIterator iter(cx);
    Activation* act = iter.activation();

    if (!act) {
      return fail(cx, "internal error");
    }

    JSScript* script = nullptr;
    int32_t engine = 0;

    if (act->isJit()) {
      JitFrameIter frame(iter->asJit());

      while (!frame.done()) {
        if (frame.isWasm()) {
          JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                    JSMSG_TRACELOGGER_ENABLE_FAIL,
                                    "not yet supported in wasm code");
          return false;
        }
        if (frame.asJSJit().isScripted()) {
          break;
        }
<<<<<<< HEAD
        ++frame;
      }
||||||| merged common ancestors
    }
}

void
TraceLoggerThreadState::disableAllTextIds() {
    for (uint32_t i = 1; i < TraceLogger_Last; i++) {
        enabledTextIds[i] = false;
    }
}

void
TraceLoggerThreadState::enableTextIdsForProfiler() {
    enableDefaultLogging();
}
=======
        ++frame;
      }

      MOZ_ASSERT(!frame.done());

      const JSJitFrameIter& jitFrame = frame.asJSJit();
      MOZ_ASSERT(jitFrame.isIonJS() || jitFrame.isBaselineJS());

      script = jitFrame.script();
      engine =
          jitFrame.isIonJS() ? TraceLogger_IonMonkey : TraceLogger_Baseline;
    } else {
      MOZ_ASSERT(act->isInterpreter());
      InterpreterFrame* fp = act->asInterpreter()->current();
      MOZ_ASSERT(!fp->runningInJit());

      script = fp->script();
      engine = TraceLogger_Interpreter;
    }
    if (script->compartment() != cx->compartment()) {
      return fail(cx, "compartment mismatch");
    }

    TraceLoggerEvent event(TraceLogger_Scripts, script);
    startEvent(event);
    startEvent(engine);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      MOZ_ASSERT(!frame.done());
||||||| merged common ancestors
void
TraceLoggerThreadState::disableTextIdsForProfiler() {
    disableAllTextIds();
    // We have to keep the Baseline and IonMonkey id's alive because they control whether
    // the jitted codegen has tracelogger start & stop events builtin.  Otherwise, we end up
    // in situations when some jitted code that was created before the profiler was even
    // started ends up not starting and stoping any events.  The TraceLogger_Engine stop events
    // can accidentally stop the wrong event in this case, and then it's no longer possible to
    // build a graph.
    enabledTextIds[TraceLogger_Engine] = true;
    enabledTextIds[TraceLogger_Interpreter] = true;
    enabledTextIds[TraceLogger_Baseline] = true;
    enabledTextIds[TraceLogger_IonMonkey] = true;
}
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
      const JSJitFrameIter& jitFrame = frame.asJSJit();
      MOZ_ASSERT(jitFrame.isIonJS() || jitFrame.isBaselineJS());
||||||| merged common ancestors
void
TraceLoggerThreadState::enableDefaultLogging()
{
    enabledTextIds[TraceLogger_AnnotateScripts] = true;
    enabledTextIds[TraceLogger_Bailout] = true;
    enabledTextIds[TraceLogger_Baseline] = true;
    enabledTextIds[TraceLogger_BaselineCompilation] = true;
    enabledTextIds[TraceLogger_GC] = true;
    enabledTextIds[TraceLogger_GCAllocation] = true;
    enabledTextIds[TraceLogger_GCSweeping] = true;
    enabledTextIds[TraceLogger_Interpreter] = true;
    enabledTextIds[TraceLogger_IonAnalysis] = true;
    enabledTextIds[TraceLogger_IonCompilation] = true;
    enabledTextIds[TraceLogger_IonLinking] = true;
    enabledTextIds[TraceLogger_IonMonkey] = true;
    enabledTextIds[TraceLogger_MinorGC] = true;
    enabledTextIds[TraceLogger_Frontend] = true;
    enabledTextIds[TraceLogger_ParsingFull] = true;
    enabledTextIds[TraceLogger_ParsingSyntax] = true;
    enabledTextIds[TraceLogger_BytecodeEmission] = true;
    enabledTextIds[TraceLogger_IrregexpCompile] = true;
    enabledTextIds[TraceLogger_IrregexpExecute] = true;
    enabledTextIds[TraceLogger_Scripts] = true;
    enabledTextIds[TraceLogger_Engine] = true;
    enabledTextIds[TraceLogger_WasmCompilation] = true;
    enabledTextIds[TraceLogger_Interpreter] = true;
    enabledTextIds[TraceLogger_Baseline] = true;
    enabledTextIds[TraceLogger_IonMonkey] = true;
}
=======
bool TraceLoggerThread::disable(bool force, const char* error) {
  if (failed_) {
    MOZ_ASSERT(enabled_ == 0);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      script = jitFrame.script();
      engine =
          jitFrame.isIonJS() ? TraceLogger_IonMonkey : TraceLogger_Baseline;
    } else {
      MOZ_ASSERT(act->isInterpreter());
      InterpreterFrame* fp = act->asInterpreter()->current();
      MOZ_ASSERT(!fp->runningInJit());
||||||| merged common ancestors
void
TraceLoggerThreadState::enableIonLogging()
{
    enabledTextIds[TraceLogger_IonCompilation] = true;
    enabledTextIds[TraceLogger_IonLinking] = true;
    enabledTextIds[TraceLogger_PruneUnusedBranches] = true;
    enabledTextIds[TraceLogger_FoldTests] = true;
    enabledTextIds[TraceLogger_SplitCriticalEdges] = true;
    enabledTextIds[TraceLogger_RenumberBlocks] = true;
    enabledTextIds[TraceLogger_ScalarReplacement] = true;
    enabledTextIds[TraceLogger_DominatorTree] = true;
    enabledTextIds[TraceLogger_PhiAnalysis] = true;
    enabledTextIds[TraceLogger_MakeLoopsContiguous] = true;
    enabledTextIds[TraceLogger_ApplyTypes] = true;
    enabledTextIds[TraceLogger_EagerSimdUnbox] = true;
    enabledTextIds[TraceLogger_AliasAnalysis] = true;
    enabledTextIds[TraceLogger_GVN] = true;
    enabledTextIds[TraceLogger_LICM] = true;
    enabledTextIds[TraceLogger_Sincos] = true;
    enabledTextIds[TraceLogger_RangeAnalysis] = true;
    enabledTextIds[TraceLogger_LoopUnrolling] = true;
    enabledTextIds[TraceLogger_FoldLinearArithConstants] = true;
    enabledTextIds[TraceLogger_EffectiveAddressAnalysis] = true;
    enabledTextIds[TraceLogger_AlignmentMaskAnalysis] = true;
    enabledTextIds[TraceLogger_EliminateDeadCode] = true;
    enabledTextIds[TraceLogger_ReorderInstructions] = true;
    enabledTextIds[TraceLogger_EdgeCaseAnalysis] = true;
    enabledTextIds[TraceLogger_EliminateRedundantChecks] = true;
    enabledTextIds[TraceLogger_AddKeepAliveInstructions] = true;
    enabledTextIds[TraceLogger_GenerateLIR] = true;
    enabledTextIds[TraceLogger_RegisterAllocation] = true;
    enabledTextIds[TraceLogger_GenerateCode] = true;
    enabledTextIds[TraceLogger_Scripts] = true;
    enabledTextIds[TraceLogger_IonBuilderRestartLoop] = true;
}

void
TraceLoggerThreadState::enableFrontendLogging()
{
    enabledTextIds[TraceLogger_Frontend] = true;
    enabledTextIds[TraceLogger_ParsingFull] = true;
    enabledTextIds[TraceLogger_ParsingSyntax] = true;
    enabledTextIds[TraceLogger_BytecodeEmission] = true;
    enabledTextIds[TraceLogger_BytecodeFoldConstants] = true;
    enabledTextIds[TraceLogger_BytecodeNameFunctions] = true;
}

TraceLoggerThread::~TraceLoggerThread()
{
    if (graph.get()) {
        if (!failed) {
            graph->log(events);
        }
        graph = nullptr;
    }
}
=======
  if (enabled_ == 0) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      script = fp->script();
      engine = TraceLogger_Interpreter;
    }
    if (script->compartment() != cx->compartment()) {
      return fail(cx, "compartment mismatch");
    }
||||||| merged common ancestors
bool
TraceLoggerThread::enable()
{
    if (enabled_ > 0) {
        enabled_++;
        return true;
    }

    if (failed) {
        return false;
    }
=======
  if (enabled_ > 1 && !force) {
    enabled_--;
    return true;
  }

  if (force) {
    traceLoggerState->maybeSpewError(error);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    TraceLoggerEvent event(TraceLogger_Scripts, script);
    startEvent(event);
    startEvent(engine);
  }
||||||| merged common ancestors
    enabled_ = 1;
    logTimestamp(TraceLogger_Enable);
=======
  logTimestamp(TraceLogger_Disable);
  enabled_ = 0;
>>>>>>> upstream-releases

  return true;
}

<<<<<<< HEAD
bool TraceLoggerThread::disable(bool force, const char* error) {
  if (failed) {
    MOZ_ASSERT(enabled_ == 0);
    return false;
  }
||||||| merged common ancestors
bool
TraceLoggerThread::fail(JSContext* cx, const char* error)
{
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_TRACELOGGER_ENABLE_FAIL, error);
    failed = true;
    enabled_ = 0;

    return false;
}
=======
const char* TraceLoggerThread::maybeEventText(uint32_t id) {
  if (TLTextIdIsEnumEvent(id)) {
    return TLTextIdString(static_cast<TraceLoggerTextId>(id));
  }
  return traceLoggerState->maybeEventText(id);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (enabled_ == 0) {
    return true;
  }

  if (enabled_ > 1 && !force) {
    enabled_--;
    return true;
  }

  if (force) {
    traceLoggerState->maybeSpewError(error);
  }
||||||| merged common ancestors
void
TraceLoggerThread::silentFail(const char* error)
{
    traceLoggerState->maybeSpewError(error);
    failed = true;
    enabled_ = 0;
}
=======
TraceLoggerEventPayload* TraceLoggerThreadState::getPayload(uint32_t id) {
  if (TLTextIdIsEnumEvent(id)) {
    return nullptr;
  }

  TextIdToPayloadMap::Ptr p = textIdPayloads.lookup(id);
  if (!p) {
    return nullptr;
  }

  p->value()->use();
  return p->value();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  logTimestamp(TraceLogger_Disable);
  enabled_ = 0;
||||||| merged common ancestors
size_t
TraceLoggerThread::sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf) const
{
    size_t size = 0;
#ifdef DEBUG
    size += graphStack.sizeOfExcludingThis(mallocSizeOf);
#endif
    size += events.sizeOfExcludingThis(mallocSizeOf);
    if (graph.get()) {
        size += graph->sizeOfIncludingThis(mallocSizeOf);
    }
    return size;
}
=======
const char* TraceLoggerThreadState::maybeEventText(uint32_t id) {
  LockGuard<Mutex> guard(lock);

  TextIdToPayloadMap::Ptr p = textIdPayloads.lookup(id);
  if (!p) {
    return nullptr;
  }

  uint32_t dictId = p->value()->dictionaryId();
  MOZ_ASSERT(dictId < nextDictionaryId);
  return dictionaryData[dictId].get();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
size_t
TraceLoggerThread::sizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf) const
{
    return mallocSizeOf(this) + sizeOfExcludingThis(mallocSizeOf);
=======
const char* TraceLoggerThreadState::maybeEventText(TraceLoggerEventPayload* p) {
  LockGuard<Mutex> guard(lock);
  if (!p) {
    return nullptr;
  }

  uint32_t dictId = p->dictionaryId();
  MOZ_ASSERT(dictId < nextDictionaryId);
  return dictionaryData[dictId].get();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
const char* TraceLoggerThread::maybeEventText(uint32_t id) {
  if (id < TraceLogger_Last) {
    return TLTextIdString(static_cast<TraceLoggerTextId>(id));
  }
  return traceLoggerState->maybeEventText(id);
}
||||||| merged common ancestors
bool
TraceLoggerThread::enable(JSContext* cx)
{
    using namespace js::jit;
=======
size_t TraceLoggerThreadState::sizeOfExcludingThis(
    mozilla::MallocSizeOf mallocSizeOf) {
  LockGuard<Mutex> guard(lock);
>>>>>>> upstream-releases

<<<<<<< HEAD
TraceLoggerEventPayload* TraceLoggerThreadState::getPayload(uint32_t id) {
  if (id < TraceLogger_Last) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!enable()) {
        return fail(cx, "internal error");
    }
=======
  // Do not count threadLoggers since they are counted by
  // JSContext::traceLogger.
>>>>>>> upstream-releases

<<<<<<< HEAD
  TextIdToPayloadMap::Ptr p = textIdPayloads.lookup(id);
  if (!p) {
    return nullptr;
  }
||||||| merged common ancestors
    if (enabled_ == 1) {
        // Get the top Activation to log the top script/pc (No inlined frames).
        ActivationIterator iter(cx);
        Activation* act = iter.activation();
=======
  size_t size = 0;
  size += dictionaryData.sizeOfExcludingThis(mallocSizeOf);
  size += payloadDictionary.shallowSizeOfExcludingThis(mallocSizeOf);
  size += textIdPayloads.shallowSizeOfExcludingThis(mallocSizeOf);
  for (TextIdToPayloadMap::Range r = textIdPayloads.all(); !r.empty();
       r.popFront()) {
    r.front().value()->sizeOfIncludingThis(mallocSizeOf);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  p->value()->use();
  return p->value();
}
||||||| merged common ancestors
        if (!act) {
            return fail(cx, "internal error");
        }
=======
  return size;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
const char* TraceLoggerThreadState::maybeEventText(uint32_t id) {
  LockGuard<Mutex> guard(lock);
||||||| merged common ancestors
        JSScript* script = nullptr;
        int32_t engine = 0;

        if (act->isJit()) {
            JitFrameIter frame(iter->asJit());

            while (!frame.done()) {
                if (frame.isWasm()) {
                    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                              JSMSG_TRACELOGGER_ENABLE_FAIL,
                                              "not yet supported in wasm code");
                    return false;
                }
                if (frame.asJSJit().isScripted()) {
                    break;
                }
                ++frame;
            }

            MOZ_ASSERT(!frame.done());

            const JSJitFrameIter& jitFrame = frame.asJSJit();
            MOZ_ASSERT(jitFrame.isIonJS() || jitFrame.isBaselineJS());

            script = jitFrame.script();
            engine = jitFrame.isIonJS() ? TraceLogger_IonMonkey : TraceLogger_Baseline;
        } else {
            MOZ_ASSERT(act->isInterpreter());
            InterpreterFrame* fp = act->asInterpreter()->current();
            MOZ_ASSERT(!fp->runningInJit());

            script = fp->script();
            engine = TraceLogger_Interpreter;
        }
        if (script->compartment() != cx->compartment()) {
            return fail(cx, "compartment mismatch");
        }
=======
TraceLoggerEventPayload* TraceLoggerThreadState::getOrCreateEventPayload(
    const char* text) {
  LockGuard<Mutex> guard(lock);

  uint32_t dictId = nextDictionaryId;

  StringHashToDictionaryMap::AddPtr dictp =
      payloadDictionary.lookupForAdd(text);
  if (dictp) {
    dictId = dictp->value();
    MOZ_ASSERT(dictId < nextDictionaryId);  // Sanity check.
  } else {
    UniqueChars str = DuplicateString(text);
    if (!str) {
      return nullptr;
    }
    if (!payloadDictionary.add(dictp, str.get(), nextDictionaryId)) {
      return nullptr;
    }
    if (!dictionaryData.append(std::move(str))) {
      return nullptr;
    }

    nextDictionaryId++;
  }

  // Look for a free entry, as some textId's may
  // already be taken from previous profiling sessions.
  while (textIdPayloads.has(nextTextId)) {
    nextTextId++;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  TextIdToPayloadMap::Ptr p = textIdPayloads.lookup(id);
  if (!p) {
    return nullptr;
  }
||||||| merged common ancestors
        TraceLoggerEvent event(TraceLogger_Scripts, script);
        startEvent(event);
        startEvent(engine);
    }
=======
  auto* payload = js_new<TraceLoggerEventPayload>(nextTextId, dictId);
  if (!payload) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t dictId = p->value()->dictionaryId();
  MOZ_ASSERT(dictId < nextDictionaryId);
  return dictionaryData[dictId].get();
}
||||||| merged common ancestors
    return true;
}
=======
  if (!textIdPayloads.putNew(nextTextId, payload)) {
    js_delete(payload);
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
const char* TraceLoggerThreadState::maybeEventText(TraceLoggerEventPayload* p) {
  LockGuard<Mutex> guard(lock);
  if (!p) {
    return nullptr;
  }
||||||| merged common ancestors
bool
TraceLoggerThread::disable(bool force, const char* error)
{
    if (failed) {
        MOZ_ASSERT(enabled_ == 0);
        return false;
    }
=======
  payload->use();
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t dictId = p->dictionaryId();
  MOZ_ASSERT(dictId < nextDictionaryId);
  return dictionaryData[dictId].get();
}
||||||| merged common ancestors
    if (enabled_ == 0) {
        return true;
    }
=======
  nextTextId++;
>>>>>>> upstream-releases

<<<<<<< HEAD
size_t TraceLoggerThreadState::sizeOfExcludingThis(
    mozilla::MallocSizeOf mallocSizeOf) {
  LockGuard<Mutex> guard(lock);
||||||| merged common ancestors
    if (enabled_ > 1 && !force) {
        enabled_--;
        return true;
    }
=======
  return payload;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Do not count threadLoggers since they are counted by
  // JSContext::traceLogger.
||||||| merged common ancestors
    if (force) {
        traceLoggerState->maybeSpewError(error);
    }
=======
TraceLoggerEventPayload* TraceLoggerThreadState::getOrCreateEventPayload(
    const char* filename, uint32_t lineno, uint32_t colno) {
  if (!filename) {
    filename = "<unknown>";
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t size = 0;
  size += dictionaryData.sizeOfExcludingThis(mallocSizeOf);
  size += payloadDictionary.shallowSizeOfExcludingThis(mallocSizeOf);
  size += textIdPayloads.shallowSizeOfExcludingThis(mallocSizeOf);
  for (TextIdToPayloadMap::Range r = textIdPayloads.all(); !r.empty();
       r.popFront()) {
    r.front().value()->sizeOfIncludingThis(mallocSizeOf);
  }
||||||| merged common ancestors
    logTimestamp(TraceLogger_Disable);
    enabled_ = 0;
=======
  TraceLoggerEventPayload* payload = getOrCreateEventPayload(filename);
  if (!payload) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return size;
||||||| merged common ancestors
    return true;
=======
  payload->setLine(lineno);
  payload->setColumn(colno);

  return payload;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
TraceLoggerEventPayload* TraceLoggerThreadState::getOrCreateEventPayload(
    const char* text) {
  LockGuard<Mutex> guard(lock);
||||||| merged common ancestors
const char*
TraceLoggerThread::maybeEventText(uint32_t id)
{
    if (id < TraceLogger_Last) {
        return TLTextIdString(static_cast<TraceLoggerTextId>(id));
    }
    return traceLoggerState->maybeEventText(id);
}
=======
TraceLoggerEventPayload* TraceLoggerThreadState::getOrCreateEventPayload(
    JSScript* script) {
  return getOrCreateEventPayload(script->filename(), script->lineno(),
                                 script->column());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t dictId = nextDictionaryId;
||||||| merged common ancestors
TraceLoggerEventPayload*
TraceLoggerThreadState::getPayload(uint32_t id) {
    if (id < TraceLogger_Last) {
        return nullptr;
    }
=======
void TraceLoggerThreadState::purgeUnusedPayloads() {
  // Care needs to be taken to maintain a coherent state in this function,
  // as payloads can have their use count change at any time from non-zero to
  // zero (but not the other way around; see TraceLoggerEventPayload::use()).
  LockGuard<Mutex> guard(lock);
>>>>>>> upstream-releases

<<<<<<< HEAD
  StringHashToDictionaryMap::AddPtr dictp =
      payloadDictionary.lookupForAdd(text);
  if (dictp) {
    dictId = dictp->value();
    MOZ_ASSERT(dictId < nextDictionaryId);  // Sanity check.
  } else {
    UniqueChars str = DuplicateString(text);
    if (!str) {
      return nullptr;
    }
    if (!payloadDictionary.add(dictp, str.get(), nextDictionaryId)) {
      return nullptr;
    }
    if (!dictionaryData.append(std::move(str))) {
      return nullptr;
||||||| merged common ancestors
    TextIdToPayloadMap::Ptr p = textIdPayloads.lookup(id);
    if (!p) {
        return nullptr;
=======
  // Free all other payloads that have no uses anymore.
  for (TextIdToPayloadMap::Enum e(textIdPayloads); !e.empty(); e.popFront()) {
    if (e.front().value()->uses() == 0) {
      uint32_t dictId = e.front().value()->dictionaryId();
      dictionaryData.erase(dictionaryData.begin() + dictId);
      js_delete(e.front().value());
      e.removeFront();
>>>>>>> upstream-releases
    }
<<<<<<< HEAD

    nextDictionaryId++;
  }
||||||| merged common ancestors

    p->value()->use();
    return p->value();
}
=======
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Look for a free entry, as some textId's may
  // already be taken from previous profiling sessions.
  while (textIdPayloads.has(nextTextId)) {
    nextTextId++;
  }
||||||| merged common ancestors
const char*
TraceLoggerThreadState::maybeEventText(uint32_t id)
{
    LockGuard<Mutex> guard(lock);
=======
void TraceLoggerThread::startEvent(TraceLoggerTextId id) {
  startEvent(uint32_t(id));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  auto* payload = js_new<TraceLoggerEventPayload>(nextTextId, dictId);
  if (!payload) {
    return nullptr;
  }

  if (!textIdPayloads.putNew(nextTextId, payload)) {
    js_delete(payload);
    return nullptr;
  }
||||||| merged common ancestors
    TextIdToPayloadMap::Ptr p = textIdPayloads.lookup(id);
    if (!p) {
        return nullptr;
    }

    uint32_t dictId = p->value()->dictionaryId();
    MOZ_ASSERT(dictId < nextDictionaryId);
    return dictionaryData[dictId].get();
}
=======
void TraceLoggerThread::startEvent(const TraceLoggerEvent& event) {
  if (!event.hasTextId()) {
    if (!enabled()) {
      return;
    }
    startEvent(TraceLogger_Error);
    disable(/* force = */ true,
            "TraceLogger encountered an empty event. "
            "Potentially due to OOM during creation of "
            "this event. Disabling TraceLogger.");
    return;
  }
  startEvent(event.textId());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  payload->use();
||||||| merged common ancestors
const char*
TraceLoggerThreadState::maybeEventText(TraceLoggerEventPayload *p)
{
    LockGuard<Mutex> guard(lock);
    if (!p) {
        return nullptr;
    }
=======
void TraceLoggerThread::startEvent(uint32_t id) {
  if (!jit::JitOptions.enableTraceLogger) {
    return;
  }

  MOZ_ASSERT(TLTextIdIsTreeEvent(id) || id == TraceLogger_Error);
  MOZ_ASSERT(traceLoggerState);
  if (!traceLoggerState->isTextIdEnabled(id)) {
    return;
  }

#ifdef DEBUG
  if (enabled_ > 0) {
    AutoEnterOOMUnsafeRegion oomUnsafe;
    if (!graphStack_.append(id)) {
      oomUnsafe.crash("Could not add item to debug stack.");
    }
  }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  nextTextId++;
||||||| merged common ancestors
=======
  if (graph_.get() && traceLoggerState->isGraphFileEnabled()) {
    // Flush each textId to disk.  textId values up to TraceLogger_Last are
    // statically defined and each one has an associated constant event string
    // defined by TLTextIdString().  For any events with textId >=
    // TraceLogger_Last the payload associated with that textId must first be
    // found and then maybeEventText() will find the event string form the
    // dictionary.
    for (uint32_t otherId = graph_->nextTextId(); otherId <= id; otherId++) {
      if (TLTextIdIsEnumEvent(id)) {
        const char* text = TLTextIdString(static_cast<TraceLoggerTextId>(id));
        graph_->addTextId(otherId, text);
      } else {
        TraceLoggerEventPayload* p = traceLoggerState->getPayload(id);
        if (p) {
          const char* filename = traceLoggerState->maybeEventText(p);
          mozilla::Maybe<uint32_t> line = p->line();
          mozilla::Maybe<uint32_t> column = p->column();
          graph_->addTextId(otherId, filename, line, column);
          p->release();
        }
      }
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return payload;
||||||| merged common ancestors
    uint32_t dictId = p->dictionaryId();
    MOZ_ASSERT(dictId < nextDictionaryId);
    return dictionaryData[dictId].get();
=======
  log(id);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
TraceLoggerEventPayload* TraceLoggerThreadState::getOrCreateEventPayload(
    const char* filename, uint32_t lineno, uint32_t colno) {
  if (!filename) {
    filename = "<unknown>";
  }
||||||| merged common ancestors
size_t
TraceLoggerThreadState::sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf)
{
    LockGuard<Mutex> guard(lock);
=======
void TraceLoggerThread::stopEvent(TraceLoggerTextId id) {
  stopEvent(uint32_t(id));
}

void TraceLoggerThread::stopEvent(const TraceLoggerEvent& event) {
  if (!event.hasTextId()) {
    stopEvent(TraceLogger_Error);
    return;
  }
  stopEvent(event.textId());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  TraceLoggerEventPayload* payload = getOrCreateEventPayload(filename);
  if (!payload) {
    return nullptr;
  }
||||||| merged common ancestors
    // Do not count threadLoggers since they are counted by JSContext::traceLogger.
=======
void TraceLoggerThread::stopEvent(uint32_t id) {
  if (!jit::JitOptions.enableTraceLogger) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  payload->setLine(lineno);
  payload->setColumn(colno);
||||||| merged common ancestors
    size_t size = 0;
    size += dictionaryData.sizeOfExcludingThis(mallocSizeOf);
    size += payloadDictionary.shallowSizeOfExcludingThis(mallocSizeOf);
    size += textIdPayloads.shallowSizeOfExcludingThis(mallocSizeOf);
    for (TextIdToPayloadMap::Range r = textIdPayloads.all(); !r.empty(); r.popFront()) {
        r.front().value()->sizeOfIncludingThis(mallocSizeOf);
    }
=======
  MOZ_ASSERT(TLTextIdIsTreeEvent(id) || id == TraceLogger_Error);
  MOZ_ASSERT(traceLoggerState);
  if (!traceLoggerState->isTextIdEnabled(id)) {
    return;
  }

#ifdef DEBUG
  if (!graphStack_.empty()) {
    uint32_t prev = graphStack_.popCopy();
    if (id == TraceLogger_Error || prev == TraceLogger_Error) {
      // When encountering an Error id the stack will most likely not be correct
      // anymore. Ignore this.
    } else if (id == TraceLogger_Engine) {
      MOZ_ASSERT(prev == TraceLogger_IonMonkey ||
                 prev == TraceLogger_Baseline ||
                 prev == TraceLogger_Interpreter);
    } else if (id == TraceLogger_Scripts) {
      MOZ_ASSERT(TLTextIdIsScriptEvent(prev));
    } else if (TLTextIdIsScriptEvent(id)) {
      MOZ_ASSERT(TLTextIdIsScriptEvent(prev));
      if (prev != id) {
        // Ignore if the text has been flushed already.
        MOZ_ASSERT_IF(maybeEventText(prev),
                      strcmp(maybeEventText(id), maybeEventText(prev)) == 0);
      }
    } else {
      MOZ_ASSERT(id == prev);
    }
  }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  return payload;
||||||| merged common ancestors
    return size;
=======
  log(TraceLogger_Stop);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
TraceLoggerEventPayload* TraceLoggerThreadState::getOrCreateEventPayload(
    JSScript* script) {
  return getOrCreateEventPayload(script->filename(), script->lineno(),
                                 script->column());
}
||||||| merged common ancestors
TraceLoggerEventPayload*
TraceLoggerThreadState::getOrCreateEventPayload(const char* text)
{
    LockGuard<Mutex> guard(lock);

    uint32_t dictId = nextDictionaryId;
=======
JS::AutoTraceLoggerLockGuard::AutoTraceLoggerLockGuard() {
  traceLoggerState->lock.lock();
}

JS::AutoTraceLoggerLockGuard::~AutoTraceLoggerLockGuard() {
  traceLoggerState->lock.unlock();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
void TraceLoggerThreadState::purgeUnusedPayloads() {
  // Care needs to be taken to maintain a coherent state in this function,
  // as payloads can have their use count change at any time from non-zero to
  // zero (but not the other way around; see TraceLoggerEventPayload::use()).
  LockGuard<Mutex> guard(lock);

  // Free all other payloads that have no uses anymore.
  for (TextIdToPayloadMap::Enum e(textIdPayloads); !e.empty(); e.popFront()) {
    if (e.front().value()->uses() == 0) {
      uint32_t dictId = e.front().value()->dictionaryId();
      dictionaryData.erase(dictionaryData.begin() + dictId);
      js_delete(e.front().value());
      e.removeFront();
||||||| merged common ancestors
    StringHashToDictionaryMap::AddPtr dictp = payloadDictionary.lookupForAdd(text);
    if (dictp) {
        dictId = dictp->value();
        MOZ_ASSERT(dictId < nextDictionaryId); // Sanity check.
    } else {
        UniqueChars str = DuplicateString(text);
        if (!str) {
            return nullptr;
        }
        if(!payloadDictionary.add(dictp, str.get(), nextDictionaryId)) {
            return nullptr;
        }
        if(!dictionaryData.append(std::move(str))) {
            return nullptr;
        }

        nextDictionaryId++;
=======
size_t JS::TraceLoggerDictionaryImpl::NextChunk(JSContext* cx,
                                                size_t* dataIndex,
                                                char buffer[],
                                                size_t bufferSize) {
  MOZ_ASSERT(dataIndex != nullptr);
  if (!traceLoggerState || bufferSize == 0 || !buffer ||
      !jit::JitOptions.enableTraceLogger) {
    return 0;
  }

  size_t bufferIndex = 0;

  const char* eventString = nullptr;
  if (TLTextIdIsEnumEvent(*dataIndex)) {
    eventString = TLTextIdString(static_cast<TraceLoggerTextId>(*dataIndex));
  } else {
    uint32_t dictId = *dataIndex - TraceLogger_Last;
    if (dictId < traceLoggerState->dictionaryData.length()) {
      eventString = traceLoggerState->dictionaryData[dictId].get();
      MOZ_ASSERT(eventString);
    }
  }

  if (eventString) {
    size_t length = strlen(eventString);
    if (length < bufferSize - 1) {
      memcpy(buffer, eventString, length);
      buffer[length] = '\0';
      bufferIndex = length;
    } else {
      memcpy(buffer, eventString, bufferSize);
      buffer[bufferSize - 1] = '\0';
      bufferIndex = bufferSize - 1;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
}
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void TraceLoggerThread::startEvent(TraceLoggerTextId id) {
  startEvent(uint32_t(id));
}
||||||| merged common ancestors
    uint32_t textId = nextTextId;

    auto* payload = js_new<TraceLoggerEventPayload>(textId, dictId);
    if (!payload) {
        return nullptr;
    }
=======
  (*dataIndex)++;
  return bufferIndex;
}

size_t JS::TraceLoggerIdImpl::NextChunk(JSContext* cx, size_t* dataIndex,
                                        uint32_t buffer[], size_t bufferSize) {
  MOZ_ASSERT(dataIndex != nullptr);
  if (!cx || !cx->traceLogger) {
    return 0;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void TraceLoggerThread::startEvent(const TraceLoggerEvent& event) {
  if (!event.hasTextId()) {
    if (!enabled()) {
      return;
||||||| merged common ancestors
    if (!textIdPayloads.putNew(textId, payload)) {
        js_delete(payload);
        return nullptr;
=======
  if (bufferSize == 0 || !buffer || !jit::JitOptions.enableTraceLogger) {
    return 0;
  }

  size_t bufferIndex = 0;
  ContinuousSpace<EventEntry>& events = cx->traceLogger->events_;

  for (; *dataIndex < events.size(); (*dataIndex)++) {
    if (TLTextIdIsInternalEvent(events[*dataIndex].textId)) {
      continue;
    }

    if (TLTextIdIsScriptEvent(events[*dataIndex].textId)) {
      TraceLoggerEventPayload* payload =
          traceLoggerState->getPayload(events[*dataIndex].textId);
      MOZ_ASSERT(payload);
      if (!payload) {
        return 0;
      }
      // Write the index of this event into the jsTracerDictionary array
      // property
      uint32_t dictId = TraceLogger_Last + payload->dictionaryId();
      buffer[bufferIndex++] = dictId;
      payload->release();
    } else {
      buffer[bufferIndex++] = events[*dataIndex].textId;
>>>>>>> upstream-releases
    }
    startEvent(TraceLogger_Error);
    disable(/* force = */ true,
            "TraceLogger encountered an empty event. "
            "Potentially due to OOM during creation of "
            "this event. Disabling TraceLogger.");
    return;
  }
  startEvent(event.textId());
}

<<<<<<< HEAD
void TraceLoggerThread::startEvent(uint32_t id) {
  if (!jit::JitOptions.enableTraceLogger) {
    return;
  }
||||||| merged common ancestors
    payload->use();

    nextTextId++;
=======
    if (bufferIndex == bufferSize) {
      break;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(TLTextIdIsTreeEvent(id) || id == TraceLogger_Error);
  MOZ_ASSERT(traceLoggerState);
  if (!traceLoggerState->isTextIdEnabled(id)) {
    return;
  }
||||||| merged common ancestors
    return payload;
}
=======
  return bufferIndex;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef DEBUG
  if (enabled_ > 0) {
    AutoEnterOOMUnsafeRegion oomUnsafe;
    if (!graphStack.append(id)) {
      oomUnsafe.crash("Could not add item to debug stack.");
    }
  }
#endif
||||||| merged common ancestors
TraceLoggerEventPayload*
TraceLoggerThreadState::getOrCreateEventPayload(const char* filename,
                                                uint32_t lineno, uint32_t colno)
{
    if (!filename) {
        filename = "<unknown>";
    }
=======
size_t JS::TraceLoggerLineNoImpl::NextChunk(JSContext* cx, size_t* dataIndex,
                                            int32_t buffer[],
                                            size_t bufferSize) {
  MOZ_ASSERT(dataIndex != nullptr);
  if (!cx || !cx->traceLogger) {
    return 0;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (graph.get() && traceLoggerState->isGraphFileEnabled()) {
    // Flush each textId to disk.  textId values up to TraceLogger_Last are
    // statically defined and each one has an associated constant event string
    // defined by TLTextIdString().  For any events with textId >=
    // TraceLogger_Last the payload associated with that textId must first be
    // found and then maybeEventText() will find the event string form the
    // dictionary.
    for (uint32_t otherId = graph->nextTextId(); otherId <= id; otherId++) {
      if (id < TraceLogger_Last) {
        const char* text = TLTextIdString(static_cast<TraceLoggerTextId>(id));
        graph->addTextId(otherId, text);
      } else {
        TraceLoggerEventPayload* p = traceLoggerState->getPayload(id);
        if (p) {
          const char* filename = traceLoggerState->maybeEventText(p);
          mozilla::Maybe<uint32_t> line = p->line();
          mozilla::Maybe<uint32_t> column = p->column();
          graph->addTextId(otherId, filename, line, column);
          p->release();
        }
      }
||||||| merged common ancestors
    TraceLoggerEventPayload *payload = getOrCreateEventPayload(filename);
    if (!payload) {
        return nullptr;
=======
  if (bufferSize == 0 || !buffer || !jit::JitOptions.enableTraceLogger) {
    return 0;
  }

  size_t bufferIndex = 0;
  ContinuousSpace<EventEntry>& events = cx->traceLogger->events_;

  for (; *dataIndex < events.size(); (*dataIndex)++) {
    if (TLTextIdIsInternalEvent(events[*dataIndex].textId)) {
      continue;
    }

    if (TLTextIdIsScriptEvent(events[*dataIndex].textId)) {
      TraceLoggerEventPayload* payload =
          traceLoggerState->getPayload(events[*dataIndex].textId);
      MOZ_ASSERT(payload);
      if (!payload) {
        return 0;
      }
      mozilla::Maybe<uint32_t> line = payload->line();
      payload->release();
      if (line) {
        buffer[bufferIndex++] = *line;
      } else {
        buffer[bufferIndex++] = -1;
      }
    } else {
      buffer[bufferIndex++] = -1;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
    if (bufferIndex == bufferSize) {
      break;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  log(id);
}

void TraceLoggerThread::stopEvent(TraceLoggerTextId id) {
  stopEvent(uint32_t(id));
}

void TraceLoggerThread::stopEvent(const TraceLoggerEvent& event) {
  if (!event.hasTextId()) {
    stopEvent(TraceLogger_Error);
    return;
  }
  stopEvent(event.textId());
||||||| merged common ancestors
    payload->setLine(lineno);
    payload->setColumn(colno);

    return payload;
}

TraceLoggerEventPayload*
TraceLoggerThreadState::getOrCreateEventPayload(JSScript* script)
{
    return getOrCreateEventPayload(script->filename(), script->lineno(), script->column());
=======
  return bufferIndex;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void TraceLoggerThread::stopEvent(uint32_t id) {
  if (!jit::JitOptions.enableTraceLogger) {
    return;
  }
||||||| merged common ancestors
void
TraceLoggerThreadState::purgeUnusedPayloads()
{
    // Care needs to be taken to maintain a coherent state in this function,
    // as payloads can have their use count change at any time from non-zero to
    // zero (but not the other way around; see TraceLoggerEventPayload::use()).
    LockGuard<Mutex> guard(lock);
=======
size_t JS::TraceLoggerColNoImpl::NextChunk(JSContext* cx, size_t* dataIndex,
                                           int32_t buffer[],
                                           size_t bufferSize) {
  MOZ_ASSERT(dataIndex != nullptr);
  if (!cx || !cx->traceLogger) {
    return 0;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(TLTextIdIsTreeEvent(id) || id == TraceLogger_Error);
  MOZ_ASSERT(traceLoggerState);
  if (!traceLoggerState->isTextIdEnabled(id)) {
    return;
  }

#ifdef DEBUG
  if (!graphStack.empty()) {
    uint32_t prev = graphStack.popCopy();
    if (id == TraceLogger_Error || prev == TraceLogger_Error) {
      // When encountering an Error id the stack will most likely not be correct
      // anymore. Ignore this.
    } else if (id == TraceLogger_Engine) {
      MOZ_ASSERT(prev == TraceLogger_IonMonkey ||
                 prev == TraceLogger_Baseline ||
                 prev == TraceLogger_Interpreter);
    } else if (id == TraceLogger_Scripts) {
      MOZ_ASSERT(prev >= TraceLogger_Last);
    } else if (id >= TraceLogger_Last) {
      MOZ_ASSERT(prev >= TraceLogger_Last);
      if (prev != id) {
        // Ignore if the text has been flushed already.
        MOZ_ASSERT_IF(maybeEventText(prev),
                      strcmp(maybeEventText(id), maybeEventText(prev)) == 0);
      }
    } else {
      MOZ_ASSERT(id == prev);
||||||| merged common ancestors
    // Free all other payloads that have no uses anymore.
    for (TextIdToPayloadMap::Enum e(textIdPayloads); !e.empty(); e.popFront()) {
        if (e.front().value()->uses() == 0) {
            uint32_t dictId = e.front().value()->dictionaryId();
            dictionaryData.erase(dictionaryData.begin() + dictId);
            js_delete(e.front().value());
            e.removeFront();
        }
=======
  if (bufferSize == 0 || !buffer || !jit::JitOptions.enableTraceLogger) {
    return 0;
  }

  size_t bufferIndex = 0;
  ContinuousSpace<EventEntry>& events = cx->traceLogger->events_;

  for (; *dataIndex < events.size(); (*dataIndex)++) {
    if (TLTextIdIsInternalEvent(events[*dataIndex].textId)) {
      continue;
    }

    if (TLTextIdIsScriptEvent(events[*dataIndex].textId)) {
      TraceLoggerEventPayload* payload =
          traceLoggerState->getPayload(events[*dataIndex].textId);
      MOZ_ASSERT(payload);
      if (!payload) {
        return 0;
      }
      mozilla::Maybe<uint32_t> column = payload->column();
      payload->release();
      if (column) {
        buffer[bufferIndex++] = *column;
      } else {
        buffer[bufferIndex++] = -1;
      }
    } else {
      buffer[bufferIndex++] = -1;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
#endif

  log(TraceLogger_Stop);
}
||||||| merged common ancestors
}
=======
    if (bufferIndex == bufferSize) {
      break;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JS::AutoTraceLoggerLockGuard::AutoTraceLoggerLockGuard() {
  traceLoggerState->lock.lock();
||||||| merged common ancestors
void
TraceLoggerThread::startEvent(TraceLoggerTextId id) {
    startEvent(uint32_t(id));
=======
  return bufferIndex;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS::AutoTraceLoggerLockGuard::~AutoTraceLoggerLockGuard() {
  traceLoggerState->lock.unlock();
}
||||||| merged common ancestors
void
TraceLoggerThread::startEvent(const TraceLoggerEvent& event) {
    if (!event.hasTextId()) {
        if (!enabled()) {
            return;
        }
        startEvent(TraceLogger_Error);
        disable(/* force = */ true, "TraceLogger encountered an empty event. "
                                    "Potentially due to OOM during creation of "
                                    "this event. Disabling TraceLogger.");
        return;
    }
    startEvent(event.textId());
}
=======
size_t JS::TraceLoggerTimeStampImpl::NextChunk(JSContext* cx, size_t* dataIndex,
                                               mozilla::TimeStamp buffer[],
                                               size_t bufferSize) {
  MOZ_ASSERT(dataIndex != nullptr);
  if (!cx || !cx->traceLogger) {
    return 0;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
size_t JS::TraceLoggerDictionaryImpl::NextChunk(JSContext* cx,
                                                size_t* dataIndex,
                                                char buffer[],
                                                size_t bufferSize) {
  MOZ_ASSERT(dataIndex != nullptr);
  if (!traceLoggerState || bufferSize == 0 || !buffer ||
      !jit::JitOptions.enableTraceLogger) {
    return 0;
  }

  size_t bufferIndex = 0;

  const char* eventString = nullptr;
  if (*dataIndex < TraceLogger_Last) {
    eventString = TLTextIdString(static_cast<TraceLoggerTextId>(*dataIndex));
  } else {
    uint32_t dictId = *dataIndex - TraceLogger_Last;
    if (dictId < traceLoggerState->dictionaryData.length()) {
      eventString = traceLoggerState->dictionaryData[dictId].get();
      MOZ_ASSERT(eventString);
    }
  }

  if (eventString) {
    size_t length = strlen(eventString);
    if (length < bufferSize - 1) {
      memcpy(buffer, eventString, length);
      buffer[length] = '\0';
      bufferIndex = length;
    } else {
      memcpy(buffer, eventString, bufferSize);
      buffer[bufferSize - 1] = '\0';
      bufferIndex = bufferSize - 1;
    }
  }
||||||| merged common ancestors
void
TraceLoggerThread::startEvent(uint32_t id)
{
    if (!jit::JitOptions.enableTraceLogger) {
        return;
    }
=======
  if (bufferSize == 0 || !buffer || !jit::JitOptions.enableTraceLogger) {
    return 0;
  }

  size_t bufferIndex = 0;
  ContinuousSpace<EventEntry>& events = cx->traceLogger->events_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  (*dataIndex)++;
  return bufferIndex;
}

size_t JS::TraceLoggerIdImpl::NextChunk(JSContext* cx, size_t* dataIndex,
                                        uint32_t buffer[], size_t bufferSize) {
  MOZ_ASSERT(dataIndex != nullptr);
  if (!cx || !cx->traceLogger) {
    return 0;
  }

  if (bufferSize == 0 || !buffer || !jit::JitOptions.enableTraceLogger) {
    return 0;
  }

  size_t bufferIndex = 0;
  ContinuousSpace<EventEntry>& events = cx->traceLogger->events;

  for (; *dataIndex < events.size(); (*dataIndex)++) {
    if (TLTextIdIsInternalEvent(events[*dataIndex].textId)) {
      continue;
||||||| merged common ancestors
    MOZ_ASSERT(TLTextIdIsTreeEvent(id) || id == TraceLogger_Error);
    MOZ_ASSERT(traceLoggerState);
    if (!traceLoggerState->isTextIdEnabled(id)) {
       return;
=======
  for (; *dataIndex < events.size(); (*dataIndex)++) {
    if (TLTextIdIsInternalEvent(events[*dataIndex].textId)) {
      continue;
    }
    buffer[bufferIndex++] = events[*dataIndex].time;
    if (bufferIndex == bufferSize) {
      break;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
    if (events[*dataIndex].textId >= TraceLogger_Last) {
      TraceLoggerEventPayload* payload =
          traceLoggerState->getPayload(events[*dataIndex].textId);
      MOZ_ASSERT(payload);
      // Write the index of this event into the jsTracerDictionary array
      // property
      uint32_t dictId = TraceLogger_Last + payload->dictionaryId();
      buffer[bufferIndex++] = dictId;
      payload->release();
    } else {
      buffer[bufferIndex++] = events[*dataIndex].textId;
      ;
    }
||||||| merged common ancestors
#ifdef DEBUG
    if (enabled_ > 0) {
        AutoEnterOOMUnsafeRegion oomUnsafe;
        if (!graphStack.append(id)) {
            oomUnsafe.crash("Could not add item to debug stack.");
        }
    }
#endif
=======
  return bufferIndex;
}

size_t JS::TraceLoggerDurationImpl::NextChunk(JSContext* cx, size_t* dataIndex,
                                              double buffer[],
                                              size_t bufferSize) {
  MOZ_ASSERT(dataIndex != nullptr);
  if (!cx || !cx->traceLogger) {
    return 0;
  }

  if (bufferSize == 0 || !buffer || !jit::JitOptions.enableTraceLogger) {
    return 0;
  }

  ContinuousSpace<EventEntry>& events = cx->traceLogger->events_;
  Vector<size_t, 0, js::SystemAllocPolicy> eventStack;
  using EventDurationMap =
      HashMap<size_t, double, DefaultHasher<size_t>, SystemAllocPolicy>;
  EventDurationMap eventMap;

  size_t bufferIndex = 0;
  for (; *dataIndex < events.size(); (*dataIndex)++) {
    if (TLTextIdIsInternalEvent(events[*dataIndex].textId)) {
      continue;
    }
    double duration = 0;
    if (TLTextIdIsLogEvent(events[*dataIndex].textId)) {
      // log events are snapshot events with no start & stop
      duration = -1;
    } else if (EventDurationMap::Ptr p = eventMap.lookup(*dataIndex)) {
      // value has already been cached
      duration = p->value();
    } else {
      MOZ_ASSERT(eventStack.empty());
      if (!eventStack.append(*dataIndex)) {
        return 0;
      }

      // Search through the events array to find the matching stop event in
      // order to calculate the duration time.  Cache all other durations we
      // calculate in the meantime.
      for (size_t j = *dataIndex + 1; j < events.size(); j++) {
        uint32_t id = events[j].textId;
        if (id == TraceLogger_Stop) {
          uint32_t prev = eventStack.popCopy();
          double delta = (events[j].time - events[prev].time).ToMicroseconds();
          if (prev == *dataIndex) {
            MOZ_ASSERT(eventStack.empty());
            duration = delta;
            break;
          }

          if (!eventMap.putNew(prev, delta)) {
            return 0;
          }

        } else if (TLTextIdIsTreeEvent(id)) {
          if (!eventStack.append(j)) {
            return 0;
          }
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (bufferIndex == bufferSize) {
      break;
||||||| merged common ancestors
    if (graph.get() && traceLoggerState->isGraphFileEnabled()) {
        // Flush each textId to disk.  textId values up to TraceLogger_Last are statically defined
        // and each one has an associated constant event string defined by TLTextIdString().  For
        // any events with textId >= TraceLogger_Last the payload associated with that textId must
        // first be found and then maybeEventText() will find the event string form the dictionary.
        for (uint32_t otherId = graph->nextTextId(); otherId <= id; otherId++) {
            if (id < TraceLogger_Last) {
                const char *text = TLTextIdString(static_cast<TraceLoggerTextId>(id));
                graph->addTextId(otherId, text);
            } else {
                TraceLoggerEventPayload *p = traceLoggerState->getPayload(id);
                if (p) {
                    const char *filename = traceLoggerState->maybeEventText(p);
                    mozilla::Maybe<uint32_t> line   = p->line();
                    mozilla::Maybe<uint32_t> column = p->column();
                    graph->addTextId(otherId, filename, line, column);
                    p->release();
                }
            }
        }
=======
        // If we reach the end of the list, use the last event as the end
        // event for all events remaining on the stack.
        if (j == events.size() - 1) {
          while (!eventStack.empty()) {
            uint32_t prev = eventStack.popCopy();
            double delta =
                (events[j].time - events[prev].time).ToMicroseconds();
            if (prev == *dataIndex) {
              MOZ_ASSERT(eventStack.empty());
              duration = delta;
            } else {
              if (!eventMap.putNew(prev, delta)) {
                return 0;
              }
            }
          }
        }
      }
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  return bufferIndex;
}
||||||| merged common ancestors
    log(id);
}
=======
    buffer[bufferIndex++] = duration;
    if (bufferIndex == bufferSize) {
      break;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
size_t JS::TraceLoggerLineNoImpl::NextChunk(JSContext* cx, size_t* dataIndex,
                                            int32_t buffer[],
                                            size_t bufferSize) {
  MOZ_ASSERT(dataIndex != nullptr);
  if (!cx || !cx->traceLogger) {
    return 0;
  }
||||||| merged common ancestors
void
TraceLoggerThread::stopEvent(TraceLoggerTextId id) {
    stopEvent(uint32_t(id));
}
=======
  return bufferIndex;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (bufferSize == 0 || !buffer || !jit::JitOptions.enableTraceLogger) {
    return 0;
  }

  size_t bufferIndex = 0;
  ContinuousSpace<EventEntry>& events = cx->traceLogger->events;

  for (; *dataIndex < events.size(); (*dataIndex)++) {
    if (TLTextIdIsInternalEvent(events[*dataIndex].textId)) {
      continue;
    }

    if (events[*dataIndex].textId >= TraceLogger_Last) {
      TraceLoggerEventPayload* payload =
          traceLoggerState->getPayload(events[*dataIndex].textId);
      MOZ_ASSERT(payload);
      mozilla::Maybe<uint32_t> line = payload->line();
      payload->release();
      if (line) {
        buffer[bufferIndex++] = *line;
      } else {
        buffer[bufferIndex++] = -1;
      }
    } else {
      buffer[bufferIndex++] = -1;
    }
    if (bufferIndex == bufferSize) {
      break;
    }
  }

  return bufferIndex;
||||||| merged common ancestors
void
TraceLoggerThread::stopEvent(const TraceLoggerEvent& event) {
    if (!event.hasTextId()) {
        stopEvent(TraceLogger_Error);
        return;
    }
    stopEvent(event.textId());
=======
void TraceLoggerThread::logTimestamp(TraceLoggerTextId id) {
  logTimestamp(uint32_t(id));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
size_t JS::TraceLoggerColNoImpl::NextChunk(JSContext* cx, size_t* dataIndex,
                                           int32_t buffer[],
                                           size_t bufferSize) {
  MOZ_ASSERT(dataIndex != nullptr);
  if (!cx || !cx->traceLogger) {
    return 0;
  }

  if (bufferSize == 0 || !buffer || !jit::JitOptions.enableTraceLogger) {
    return 0;
  }

  size_t bufferIndex = 0;
  ContinuousSpace<EventEntry>& events = cx->traceLogger->events;

  for (; *dataIndex < events.size(); (*dataIndex)++) {
    if (TLTextIdIsInternalEvent(events[*dataIndex].textId)) {
      continue;
    }

    if (events[*dataIndex].textId >= TraceLogger_Last) {
      TraceLoggerEventPayload* payload =
          traceLoggerState->getPayload(events[*dataIndex].textId);
      MOZ_ASSERT(payload);
      mozilla::Maybe<uint32_t> column = payload->column();
      payload->release();
      if (column) {
        buffer[bufferIndex++] = *column;
      } else {
        buffer[bufferIndex++] = -1;
      }
    } else {
      buffer[bufferIndex++] = -1;
    }
    if (bufferIndex == bufferSize) {
      break;
    }
  }
||||||| merged common ancestors
void
TraceLoggerThread::stopEvent(uint32_t id)
{
    if (!jit::JitOptions.enableTraceLogger) {
        return;
    }
=======
void TraceLoggerThread::logTimestamp(uint32_t id) {
  MOZ_ASSERT(id > TraceLogger_TreeItemEnd && id < TraceLogger_Last);
  log(id);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return bufferIndex;
}

size_t JS::TraceLoggerTimeStampImpl::NextChunk(JSContext* cx, size_t* dataIndex,
                                               mozilla::TimeStamp buffer[],
                                               size_t bufferSize) {
  MOZ_ASSERT(dataIndex != nullptr);
  if (!cx || !cx->traceLogger) {
    return 0;
  }

  if (bufferSize == 0 || !buffer || !jit::JitOptions.enableTraceLogger) {
    return 0;
  }

  size_t bufferIndex = 0;
  ContinuousSpace<EventEntry>& events = cx->traceLogger->events;

  for (; *dataIndex < events.size(); (*dataIndex)++) {
    if (TLTextIdIsInternalEvent(events[*dataIndex].textId)) {
      continue;
    }
    buffer[bufferIndex++] = events[*dataIndex].time;
    if (bufferIndex == bufferSize) {
      break;
    }
  }
||||||| merged common ancestors
    MOZ_ASSERT(TLTextIdIsTreeEvent(id) || id == TraceLogger_Error);
    MOZ_ASSERT(traceLoggerState);
    if (!traceLoggerState->isTextIdEnabled(id)) {
        return;
    }
=======
void TraceLoggerThread::log(uint32_t id) {
  if (enabled_ == 0) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return bufferIndex;
}

size_t JS::TraceLoggerDurationImpl::NextChunk(JSContext* cx, size_t* dataIndex,
                                              double buffer[],
                                              size_t bufferSize) {
  MOZ_ASSERT(dataIndex != nullptr);
  if (!cx || !cx->traceLogger) {
    return 0;
  }

  if (bufferSize == 0 || !buffer || !jit::JitOptions.enableTraceLogger) {
    return 0;
  }

  ContinuousSpace<EventEntry>& events = cx->traceLogger->events;
  Vector<size_t, 0, js::SystemAllocPolicy> eventStack;
  using EventDurationMap =
      HashMap<size_t, double, DefaultHasher<size_t>, SystemAllocPolicy>;
  EventDurationMap eventMap;

  size_t bufferIndex = 0;
  for (; *dataIndex < events.size(); (*dataIndex)++) {
    if (TLTextIdIsInternalEvent(events[*dataIndex].textId)) {
      continue;
    }
    double duration = 0;
    if (TLTextIdIsLogEvent(events[*dataIndex].textId)) {
      // log events are snapshot events with no start & stop
      duration = -1;
    } else if (EventDurationMap::Ptr p = eventMap.lookup(*dataIndex)) {
      // value has already been cached
      duration = p->value();
    } else {
      MOZ_ASSERT(eventStack.empty());
      if (!eventStack.append(*dataIndex)) {
        return 0;
      }

      // Search through the events array to find the matching stop event in
      // order to calculate the duration time.  Cache all other durations we
      // calculate in the meantime.
      for (size_t j = *dataIndex + 1; j < events.size(); j++) {
        uint32_t id = events[j].textId;
        if (id == TraceLogger_Stop) {
          uint32_t prev = eventStack.popCopy();
          double delta = (events[j].time - events[prev].time).ToMicroseconds();
          if (prev == *dataIndex) {
            MOZ_ASSERT(eventStack.empty());
            duration = delta;
            break;
          }

          if (!eventMap.putNew(prev, delta)) {
            return 0;
          }

        } else if (TLTextIdIsTreeEvent(id)) {
          if (!eventStack.append(j)) {
            return 0;
          }
        }
      }
    }
||||||| merged common ancestors
#ifdef DEBUG
    if (!graphStack.empty()) {
        uint32_t prev = graphStack.popCopy();
        if (id == TraceLogger_Error || prev == TraceLogger_Error) {
            // When encountering an Error id the stack will most likely not be correct anymore.
            // Ignore this.
        } else if (id == TraceLogger_Engine) {
            MOZ_ASSERT(prev == TraceLogger_IonMonkey || prev == TraceLogger_Baseline ||
                       prev == TraceLogger_Interpreter);
        } else if (id == TraceLogger_Scripts) {
            MOZ_ASSERT(prev >= TraceLogger_Last);
        } else if (id >= TraceLogger_Last) {
            MOZ_ASSERT(prev >= TraceLogger_Last);
            if (prev != id) {
                // Ignore if the text has been flushed already.
                MOZ_ASSERT_IF(maybeEventText(prev), strcmp(maybeEventText(id), maybeEventText(prev)) == 0);
            }
        } else {
            MOZ_ASSERT(id == prev);
        }
    }
#endif
=======
#ifdef DEBUG
  if (id == TraceLogger_Disable) {
    graphStack_.clear();
  }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
    buffer[bufferIndex++] = duration;
    if (bufferIndex == bufferSize) {
      break;
    }
  }

  return bufferIndex;
}
||||||| merged common ancestors
    log(TraceLogger_Stop);
}
=======
  MOZ_ASSERT(traceLoggerState);
>>>>>>> upstream-releases

<<<<<<< HEAD
void TraceLoggerThread::logTimestamp(TraceLoggerTextId id) {
  logTimestamp(uint32_t(id));
}
||||||| merged common ancestors
void
TraceLoggerThread::logTimestamp(TraceLoggerTextId id)
{
    logTimestamp(uint32_t(id));
}
=======
  // We request for 3 items to add, since if we don't have enough room
  // we record the time it took to make more space. To log this information
  // we need 2 extra free entries.
  if (!events_.hasSpaceForAdd(3)) {
    mozilla::TimeStamp start = mozilla::TimeStamp::NowUnfuzzed();
>>>>>>> upstream-releases

<<<<<<< HEAD
void TraceLoggerThread::logTimestamp(uint32_t id) {
  MOZ_ASSERT(id > TraceLogger_TreeItemEnd && id < TraceLogger_Last);
  log(id);
}
||||||| merged common ancestors
void
TraceLoggerThread::logTimestamp(uint32_t id)
{
    MOZ_ASSERT(id > TraceLogger_TreeItemEnd && id < TraceLogger_Last);
    log(id);
}
=======
    if (!events_.ensureSpaceBeforeAdd(3)) {
      if (graph_.get()) {
        graph_->log(events_, traceLoggerState->startTime);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
void TraceLoggerThread::log(uint32_t id) {
  if (enabled_ == 0) {
    return;
  }
||||||| merged common ancestors
void
TraceLoggerThread::log(uint32_t id)
{
    if (enabled_ == 0) {
        return;
    }
=======
      // The data structures are full, and the graph file is not enabled
      // so we cannot flush to disk.  Trace logging should stop here.
      if (!traceLoggerState->isGraphFileEnabled()) {
        enabled_ = 0;
        return;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef DEBUG
  if (id == TraceLogger_Disable) {
    graphStack.clear();
  }
#endif
||||||| merged common ancestors
#ifdef DEBUG
    if (id == TraceLogger_Disable) {
        graphStack.clear();
    }
#endif
=======
      iteration_++;
      events_.clear();

      // Periodically remove unused payloads from the global logger state.
      traceLoggerState->purgeUnusedPayloads();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(traceLoggerState);
||||||| merged common ancestors
    MOZ_ASSERT(traceLoggerState);
=======
    // Log the time it took to flush the events_ as being from the
    // Tracelogger.
    if (graph_.get()) {
      MOZ_ASSERT(events_.hasSpaceForAdd(2));
      EventEntry& entryStart = events_.pushUninitialized();
      entryStart.time = start;
      entryStart.textId = TraceLogger_Internal;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // We request for 3 items to add, since if we don't have enough room
  // we record the time it took to make more space. To log this information
  // we need 2 extra free entries.
  if (!events.hasSpaceForAdd(3)) {
    mozilla::TimeStamp start = mozilla::TimeStamp::Now();
||||||| merged common ancestors
    // We request for 3 items to add, since if we don't have enough room
    // we record the time it took to make more space. To log this information
    // we need 2 extra free entries.
    if (!events.hasSpaceForAdd(3)) {
        uint64_t start = rdtsc() - traceLoggerState->startupTime;
=======
      EventEntry& entryStop = events_.pushUninitialized();
      entryStop.time = mozilla::TimeStamp::NowUnfuzzed();
      entryStop.textId = TraceLogger_Stop;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!events.ensureSpaceBeforeAdd(3)) {
      if (graph.get()) {
        graph->log(events, traceLoggerState->startTime);
      }

      // The data structures are full, and the graph file is not enabled
      // so we cannot flush to disk.  Trace logging should stop here.
      if (!traceLoggerState->isGraphFileEnabled()) {
        enabled_ = 0;
        return;
      }
||||||| merged common ancestors
        if (!events.ensureSpaceBeforeAdd(3)) {
            if (graph.get()) {
                graph->log(events);
            }
=======
  mozilla::TimeStamp time = mozilla::TimeStamp::NowUnfuzzed();
>>>>>>> upstream-releases

<<<<<<< HEAD
      iteration_++;
      events.clear();
||||||| merged common ancestors
            // The data structures are full, and the graph file is not enabled
            // so we cannot flush to disk.  Trace logging should stop here.
            if (!traceLoggerState->isGraphFileEnabled()) {
                enabled_ = 0;
                return;
            }
=======
  EventEntry& entry = events_.pushUninitialized();
  entry.time = time;
  entry.textId = id;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
      // Periodically remove unused payloads from the global logger state.
      traceLoggerState->purgeUnusedPayloads();
    }
||||||| merged common ancestors
            iteration_++;
            events.clear();
=======
bool TraceLoggerThreadState::remapDictionaryEntries(
    mozilla::Vector<UniqueChars, 0, SystemAllocPolicy>* newDictionary,
    uint32_t* newNextDictionaryId) {
  MOZ_ASSERT(newNextDictionaryId != nullptr && newDictionary != nullptr);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Log the time it took to flush the events as being from the
    // Tracelogger.
    if (graph.get()) {
      MOZ_ASSERT(events.hasSpaceForAdd(2));
      EventEntry& entryStart = events.pushUninitialized();
      entryStart.time = start;
      entryStart.textId = TraceLogger_Internal;

      EventEntry& entryStop = events.pushUninitialized();
      entryStop.time = mozilla::TimeStamp::Now();
      entryStop.textId = TraceLogger_Stop;
    }
  }

  mozilla::TimeStamp time = mozilla::TimeStamp::Now();

  EventEntry& entry = events.pushUninitialized();
  entry.time = time;
  entry.textId = id;
}

bool TraceLoggerThreadState::remapDictionaryEntries(
    mozilla::Vector<UniqueChars, 0, SystemAllocPolicy>* newDictionary,
    uint32_t* newNextDictionaryId) {
  MOZ_ASSERT(newNextDictionaryId != nullptr && newDictionary != nullptr);

  typedef HashMap<uint32_t, uint32_t, DefaultHasher<uint32_t>,
                  SystemAllocPolicy>
      DictionaryMap;
  DictionaryMap dictionaryMap;

  // Clear all payloads that are not currently used.  There may be some events
  // that still hold a pointer to a payload.  Restarting the profiler may reuse
  // the exact same event as a previous session if it's still alive so we need
  // to maintain it's existence.
  for (TextIdToPayloadMap::Enum e(textIdPayloads); !e.empty(); e.popFront()) {
    if (e.front().value()->uses() == 0) {
      js_delete(e.front().value());
      e.removeFront();
    } else {
      TraceLoggerEventPayload* payload = e.front().value();
      uint32_t dictId = payload->dictionaryId();

      if (dictionaryMap.has(dictId)) {
        DictionaryMap::Ptr mapPointer = dictionaryMap.lookup(dictId);
        MOZ_ASSERT(mapPointer);
        payload->setDictionaryId(mapPointer->value());
      } else {
        if (!newDictionary->append(std::move(dictionaryData[dictId]))) {
          return false;
||||||| merged common ancestors
            // Periodically remove unused payloads from the global logger state.
            traceLoggerState->purgeUnusedPayloads();
=======
  typedef HashMap<uint32_t, uint32_t, DefaultHasher<uint32_t>,
                  SystemAllocPolicy>
      DictionaryMap;
  DictionaryMap dictionaryMap;

  // Clear all payloads that are not currently used.  There may be some events
  // that still hold a pointer to a payload.  Restarting the profiler may reuse
  // the exact same event as a previous session if it's still alive so we need
  // to maintain it's existence.
  for (TextIdToPayloadMap::Enum e(textIdPayloads); !e.empty(); e.popFront()) {
    if (e.front().value()->uses() == 0) {
      js_delete(e.front().value());
      e.removeFront();
    } else {
      TraceLoggerEventPayload* payload = e.front().value();
      uint32_t dictId = payload->dictionaryId();

      if (dictionaryMap.has(dictId)) {
        DictionaryMap::Ptr mapPointer = dictionaryMap.lookup(dictId);
        MOZ_ASSERT(mapPointer);
        payload->setDictionaryId(mapPointer->value());
      } else {
        if (!newDictionary->append(std::move(dictionaryData[dictId]))) {
          return false;
>>>>>>> upstream-releases
        }
        payload->setDictionaryId(*newNextDictionaryId);

        if (!dictionaryMap.putNew(dictId, *newNextDictionaryId)) {
          return false;
        }

        (*newNextDictionaryId)++;
      }
    }
  }

  return true;
}

void TraceLoggerThreadState::clear() {
  LockGuard<Mutex> guard(lock);

  uint32_t newNextDictionaryId = 0;
  mozilla::Vector<UniqueChars, 0, SystemAllocPolicy> newDictionary;
  if (remapDictionaryEntries(&newDictionary, &newNextDictionaryId)) {
    // Clear and free any data used for the string dictionary.
    for (auto range = dictionaryData.all(); !range.empty(); range.popFront()) {
      range.front().reset();
    }
    dictionaryData.clearAndFree();
    dictionaryData = std::move(newDictionary);

    payloadDictionary.clearAndCompact();

    nextTextId = TraceLogger_Last;
    nextDictionaryId = newNextDictionaryId;
  }

  for (TraceLoggerThread* logger : threadLoggers) {
    logger->clear();
  }
}

<<<<<<< HEAD
void TraceLoggerThread::clear() {
  if (graph.get()) {
    graph.reset();
  }
||||||| merged common ancestors
void TraceLoggerThread::clear()
{
    if (graph.get()) {
        graph.reset();
    }
=======
void TraceLoggerThread::clear() {
  if (graph_.get()) {
    graph_.reset();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  graph = nullptr;
||||||| merged common ancestors
    graph = nullptr;
=======
  graph_ = nullptr;
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
  graphStack.clear();
||||||| merged common ancestors
    graphStack.clear();
=======
  graphStack_.clear();
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  if (!events.reset()) {
    silentFail("Cannot reset event buffer.");
  }
||||||| merged common ancestors
    if (!events.reset()) {
        silentFail("Cannot reset event buffer.");
    }

=======
  if (!events_.reset()) {
    silentFail("Cannot reset event buffer.");
  }
>>>>>>> upstream-releases
}

TraceLoggerThreadState::~TraceLoggerThreadState() {
  while (TraceLoggerThread* logger = threadLoggers.popFirst()) {
    js_delete(logger);
  }

  threadLoggers.clear();

  for (TextIdToPayloadMap::Range r = textIdPayloads.all(); !r.empty();
       r.popFront()) {
    js_delete(r.front().value());
  }

#ifdef DEBUG
  initialized = false;
#endif
}

<<<<<<< HEAD
bool TraceLoggerThreadState::init() {
  const char* env = getenv("TLLOG");
  if (env) {
    if (strstr(env, "help")) {
      fflush(nullptr);
      printf(
          "\n"
          "usage: TLLOG=option,option,option,... where options can be:\n"
          "\n"
          "Collections:\n"
          "  Default        Output all default. It includes:\n"
          "                 AnnotateScripts, Bailout, Baseline, "
          "BaselineCompilation, GC,\n"
          "                 GCAllocation, GCSweeping, Interpreter, "
          "IonAnalysis, IonCompilation,\n"
          "                 IonLinking, IonMonkey, MinorGC, Frontend, "
          "ParsingFull,\n"
          "                 ParsingSyntax, BytecodeEmission, IrregexpCompile, "
          "IrregexpExecute,\n"
          "                 Scripts, Engine, WasmCompilation\n"
          "\n"
          "  IonCompiler    Output all information about compilation. It "
          "includes:\n"
          "                 IonCompilation, IonLinking, PruneUnusedBranches, "
          "FoldTests,\n"
          "                 SplitCriticalEdges, RenumberBlocks, "
          "ScalarReplacement,\n"
          "                 DominatorTree, PhiAnalysis, MakeLoopsContiguous, "
          "ApplyTypes,\n"
          "                 EagerSimdUnbox, AliasAnalysis, GVN, LICM, Sincos, "
          "RangeAnalysis,\n"
          "                 LoopUnrolling, FoldLinearArithConstants, "
          "EffectiveAddressAnalysis,\n"
          "                 AlignmentMaskAnalysis, EliminateDeadCode, "
          "ReorderInstructions,\n"
          "                 EdgeCaseAnalysis, EliminateRedundantChecks,\n"
          "                 AddKeepAliveInstructions, GenerateLIR, "
          "RegisterAllocation,\n"
          "                 GenerateCode, Scripts, IonBuilderRestartLoop\n"
          "\n"
          "  VMSpecific     Output the specific name of the VM call\n"
          "\n"
          "  Frontend       Output all information about frontend compilation. "
          "It includes:\n"
          "                 Frontend, ParsingFull, ParsingSyntax, Tokenizing,\n"
          "                 BytecodeEmission, BytecodeFoldConstants, "
          "BytecodeNameFunctions\n"
          "Specific log items:\n");
      for (uint32_t i = 1; i < TraceLogger_Last; i++) {
        TraceLoggerTextId id = TraceLoggerTextId(i);
        if (!TLTextIdIsTogglable(id)) {
          continue;
||||||| merged common ancestors
static bool
ContainsFlag(const char* str, const char* flag)
{
    size_t flaglen = strlen(flag);
    const char* index = strstr(str, flag);
    while (index) {
        if ((index == str || index[-1] == ',') && (index[flaglen] == 0 || index[flaglen] == ',')) {
            return true;
=======
bool TraceLoggerThreadState::init() {
  const char* env = getenv("TLLOG");
  if (env) {
    if (strstr(env, "help")) {
      fflush(nullptr);
      printf(
          "\n"
          "usage: TLLOG=option,option,option,... where options can be:\n"
          "\n"
          "Collections:\n"
          "  Default        Output all default. It includes:\n"
          "                 AnnotateScripts, Bailout, Baseline, "
          "BaselineCompilation, GC,\n"
          "                 GCAllocation, GCSweeping, Interpreter, "
          "IonAnalysis, IonCompilation,\n"
          "                 IonLinking, IonMonkey, MinorGC, Frontend, "
          "ParsingFull,\n"
          "                 ParsingSyntax, BytecodeEmission, IrregexpCompile, "
          "IrregexpExecute,\n"
          "                 Scripts, Engine, WasmCompilation\n"
          "\n"
          "  IonCompiler    Output all information about compilation. It "
          "includes:\n"
          "                 IonCompilation, IonLinking, PruneUnusedBranches, "
          "FoldTests,\n"
          "                 SplitCriticalEdges, RenumberBlocks, "
          "ScalarReplacement,\n"
          "                 DominatorTree, PhiAnalysis, MakeLoopsContiguous, "
          "ApplyTypes,\n"
          "                 EagerSimdUnbox, AliasAnalysis, GVN, LICM, "
          "RangeAnalysis,\n"
          "                 LoopUnrolling, FoldLinearArithConstants, "
          "EffectiveAddressAnalysis,\n"
          "                 AlignmentMaskAnalysis, EliminateDeadCode, "
          "ReorderInstructions,\n"
          "                 EdgeCaseAnalysis, EliminateRedundantChecks,\n"
          "                 AddKeepAliveInstructions, GenerateLIR, "
          "RegisterAllocation,\n"
          "                 GenerateCode, Scripts, IonBuilderRestartLoop\n"
          "\n"
          "  VMSpecific     Output the specific name of the VM call\n"
          "\n"
          "  Frontend       Output all information about frontend compilation. "
          "It includes:\n"
          "                 Frontend, ParsingFull, ParsingSyntax, Tokenizing,\n"
          "                 BytecodeEmission, BytecodeFoldConstants, "
          "BytecodeNameFunctions\n"
          "Specific log items:\n");
      for (uint32_t i = 1; i < TraceLogger_Last; i++) {
        TraceLoggerTextId id = TraceLoggerTextId(i);
        if (!TLTextIdIsTogglable(id)) {
          continue;
>>>>>>> upstream-releases
        }
        printf("  %s\n", TLTextIdString(id));
      }
      printf("\n");
      exit(0);
      /*NOTREACHED*/
    }
<<<<<<< HEAD

    for (uint32_t i = 1; i < TraceLogger_Last; i++) {
      TraceLoggerTextId id = TraceLoggerTextId(i);
      if (TLTextIdIsTogglable(id)) {
        enabledTextIds[i] = ContainsFlag(env, TLTextIdString(id));
      } else {
        enabledTextIds[i] = true;
      }
    }
||||||| merged common ancestors
    return false;
}

bool
TraceLoggerThreadState::init()
{
    const char* env = getenv("TLLOG");
    if (env) {
        if (strstr(env, "help")) {
            fflush(nullptr);
            printf(
                "\n"
                "usage: TLLOG=option,option,option,... where options can be:\n"
                "\n"
                "Collections:\n"
                "  Default        Output all default. It includes:\n"
                "                 AnnotateScripts, Bailout, Baseline, BaselineCompilation, GC,\n"
                "                 GCAllocation, GCSweeping, Interpreter, IonAnalysis, IonCompilation,\n"
                "                 IonLinking, IonMonkey, MinorGC, Frontend, ParsingFull,\n"
                "                 ParsingSyntax, BytecodeEmission, IrregexpCompile, IrregexpExecute,\n"
                "                 Scripts, Engine, WasmCompilation\n"
                "\n"
                "  IonCompiler    Output all information about compilation. It includes:\n"
                "                 IonCompilation, IonLinking, PruneUnusedBranches, FoldTests,\n"
                "                 SplitCriticalEdges, RenumberBlocks, ScalarReplacement,\n"
                "                 DominatorTree, PhiAnalysis, MakeLoopsContiguous, ApplyTypes,\n"
                "                 EagerSimdUnbox, AliasAnalysis, GVN, LICM, Sincos, RangeAnalysis,\n"
                "                 LoopUnrolling, FoldLinearArithConstants, EffectiveAddressAnalysis,\n"
                "                 AlignmentMaskAnalysis, EliminateDeadCode, ReorderInstructions,\n"
                "                 EdgeCaseAnalysis, EliminateRedundantChecks,\n"
                "                 AddKeepAliveInstructions, GenerateLIR, RegisterAllocation,\n"
                "                 GenerateCode, Scripts, IonBuilderRestartLoop\n"
                "\n"
                "  VMSpecific     Output the specific name of the VM call\n"
                "\n"
                "  Frontend       Output all information about frontend compilation. It includes:\n"
                "                 Frontend, ParsingFull, ParsingSyntax, Tokenizing,\n"
                "                 BytecodeEmission, BytecodeFoldConstants, BytecodeNameFunctions\n"
                "Specific log items:\n"
            );
            for (uint32_t i = 1; i < TraceLogger_Last; i++) {
                TraceLoggerTextId id = TraceLoggerTextId(i);
                if (!TLTextIdIsTogglable(id)) {
                    continue;
                }
                printf("  %s\n", TLTextIdString(id));
            }
            printf("\n");
            exit(0);
            /*NOTREACHED*/
        }

        for (uint32_t i = 1; i < TraceLogger_Last; i++) {
            TraceLoggerTextId id = TraceLoggerTextId(i);
            if (TLTextIdIsTogglable(id)) {
                enabledTextIds[i] = ContainsFlag(env, TLTextIdString(id));
            } else {
                enabledTextIds[i] = true;
            }
        }

        if (ContainsFlag(env, "Default")) {
            enableDefaultLogging();
        }

        if (ContainsFlag(env, "IonCompiler")) {
            enableIonLogging();
        }

        if (ContainsFlag(env, "Frontend")) {
            enableFrontendLogging();
        }
=======

    for (uint32_t i = 1; i < TraceLogger_Last; i++) {
      TraceLoggerTextId id = TraceLoggerTextId(i);
      if (TLTextIdIsTogglable(id)) {
        enabledTextIds[i] = ContainsFlag(env, TLTextIdString(id));
      } else {
        enabledTextIds[i] = true;
      }
    }

    if (ContainsFlag(env, "Default")) {
      enableDefaultLogging();
    }

    if (ContainsFlag(env, "IonCompiler")) {
      enableIonLogging();
    }

    if (ContainsFlag(env, "Frontend")) {
      enableFrontendLogging();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (ContainsFlag(env, "Default")) {
      enableDefaultLogging();
    }
||||||| merged common ancestors
#ifdef DEBUG
        enabledTextIds[TraceLogger_Error] = true;
#endif
=======
#ifdef DEBUG
    enabledTextIds[TraceLogger_Error] = true;
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (ContainsFlag(env, "IonCompiler")) {
      enableIonLogging();
    }
||||||| merged common ancestors
    } else {
        // Most of the textId's will be enabled through JS::StartTraceLogger when
        // the gecko profiler is started.
        disableTextIdsForProfiler();
    }
=======
  } else {
    // Most of the textId's will be enabled through JS::StartTraceLogger when
    // the gecko profiler is started.
    disableTextIdsForProfiler();
  }

  enabledTextIds[TraceLogger_Interpreter] = enabledTextIds[TraceLogger_Engine];
  enabledTextIds[TraceLogger_Baseline] = enabledTextIds[TraceLogger_Engine];
  enabledTextIds[TraceLogger_IonMonkey] = enabledTextIds[TraceLogger_Engine];

  enabledTextIds[TraceLogger_Error] = true;

  const char* options = getenv("TLOPTIONS");
  if (options) {
    if (strstr(options, "help")) {
      fflush(nullptr);
      printf(
          "\n"
          "usage: TLOPTIONS=option,option,option,... where options can be:\n"
          "\n"
          "  EnableMainThread        Start logging main threads immediately.\n"
          "  EnableOffThread         Start logging helper threads "
          "immediately.\n"
          "  EnableGraph             Enable the tracelogging graph.\n"
          "  EnableGraphFile         Enable flushing tracelogger data to a "
          "file.\n"
          "  Errors                  Report errors during tracing to "
          "stderr.\n");
      printf("\n");
      exit(0);
      /*NOTREACHED*/
    }

    if (strstr(options, "EnableMainThread")) {
      mainThreadEnabled = true;
    }
    if (strstr(options, "EnableOffThread")) {
      helperThreadEnabled = true;
    }
    if (strstr(options, "EnableGraph")) {
      graphEnabled = true;
    }
    if (strstr(options, "EnableGraphFile")) {
      graphFileEnabled = true;
      jit::JitOptions.enableTraceLogger = true;
    }
    if (strstr(options, "Errors")) {
      spewErrors = true;
    }
  } else {
    mainThreadEnabled = true;
    helperThreadEnabled = true;
    graphEnabled = false;
    graphFileEnabled = false;
    spewErrors = false;
  }

  startTime = mozilla::TimeStamp::NowUnfuzzed();
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (ContainsFlag(env, "Frontend")) {
      enableFrontendLogging();
    }
||||||| merged common ancestors
    enabledTextIds[TraceLogger_Interpreter] = enabledTextIds[TraceLogger_Engine];
    enabledTextIds[TraceLogger_Baseline] = enabledTextIds[TraceLogger_Engine];
    enabledTextIds[TraceLogger_IonMonkey] = enabledTextIds[TraceLogger_Engine];
=======
#ifdef DEBUG
  initialized = true;
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef DEBUG
    enabledTextIds[TraceLogger_Error] = true;
#endif
||||||| merged common ancestors
    enabledTextIds[TraceLogger_Error] = true;
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  } else {
    // Most of the textId's will be enabled through JS::StartTraceLogger when
    // the gecko profiler is started.
    disableTextIdsForProfiler();
  }

  enabledTextIds[TraceLogger_Interpreter] = enabledTextIds[TraceLogger_Engine];
  enabledTextIds[TraceLogger_Baseline] = enabledTextIds[TraceLogger_Engine];
  enabledTextIds[TraceLogger_IonMonkey] = enabledTextIds[TraceLogger_Engine];

  enabledTextIds[TraceLogger_Error] = true;

  const char* options = getenv("TLOPTIONS");
  if (options) {
    if (strstr(options, "help")) {
      fflush(nullptr);
      printf(
          "\n"
          "usage: TLOPTIONS=option,option,option,... where options can be:\n"
          "\n"
          "  EnableMainThread        Start logging main threads immediately.\n"
          "  EnableOffThread         Start logging helper threads "
          "immediately.\n"
          "  EnableGraph             Enable the tracelogging graph.\n"
          "  EnableGraphFile         Enable flushing tracelogger data to a "
          "file.\n"
          "  Errors                  Report errors during tracing to "
          "stderr.\n");
      printf("\n");
      exit(0);
      /*NOTREACHED*/
    }

    if (strstr(options, "EnableMainThread")) {
      mainThreadEnabled = true;
    }
    if (strstr(options, "EnableOffThread")) {
      helperThreadEnabled = true;
    }
    if (strstr(options, "EnableGraph")) {
      graphEnabled = true;
    }
    if (strstr(options, "EnableGraphFile")) {
      graphFileEnabled = true;
      jit::JitOptions.enableTraceLogger = true;
    }
    if (strstr(options, "Errors")) {
      spewErrors = true;
    }
  } else {
    mainThreadEnabled = true;
    helperThreadEnabled = true;
    graphEnabled = false;
    graphFileEnabled = false;
    spewErrors = false;
  }

  startTime = mozilla::TimeStamp::Now();
||||||| merged common ancestors
    const char* options = getenv("TLOPTIONS");
    if (options) {
        if (strstr(options, "help")) {
            fflush(nullptr);
            printf(
                "\n"
                "usage: TLOPTIONS=option,option,option,... where options can be:\n"
                "\n"
                "  EnableMainThread        Start logging main threads immediately.\n"
                "  EnableOffThread         Start logging helper threads immediately.\n"
                "  EnableGraph             Enable the tracelogging graph.\n"
                "  EnableGraphFile         Enable flushing tracelogger data to a file.\n"
                "  Errors                  Report errors during tracing to stderr.\n"
            );
            printf("\n");
            exit(0);
            /*NOTREACHED*/
        }

        if (strstr(options, "EnableMainThread")) {
            mainThreadEnabled = true;
        }
        if (strstr(options, "EnableOffThread")) {
            helperThreadEnabled = true;
        }
        if (strstr(options, "EnableGraph")) {
            graphEnabled = true;
        }
        if (strstr(options, "EnableGraphFile")) {
            graphFileEnabled = true;
            jit::JitOptions.enableTraceLogger = true;
        }
        if (strstr(options, "Errors")) {
            spewErrors = true;
        }
    } else {
            mainThreadEnabled = true;
            helperThreadEnabled = true;
            graphEnabled = false;
            graphFileEnabled = false;
            spewErrors = false;
    }

    startupTime = rdtsc();
=======
void TraceLoggerThreadState::enableTextId(JSContext* cx, uint32_t textId) {
  MOZ_ASSERT(TLTextIdIsTogglable(textId));

  if (enabledTextIds[textId]) {
    return;
  }

  ReleaseAllJITCode(cx->runtime()->defaultFreeOp());
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef DEBUG
  initialized = true;
#endif
||||||| merged common ancestors
#ifdef DEBUG
    initialized = true;
#endif
=======
  enabledTextIds[textId] = true;
  if (textId == TraceLogger_Engine) {
    enabledTextIds[TraceLogger_IonMonkey] = true;
    enabledTextIds[TraceLogger_Baseline] = true;
    enabledTextIds[TraceLogger_Interpreter] = true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  if (textId == TraceLogger_Scripts) {
    jit::ToggleBaselineTraceLoggerScripts(cx->runtime(), true);
  }
  if (textId == TraceLogger_Engine) {
    jit::ToggleBaselineTraceLoggerEngine(cx->runtime(), true);
  }
>>>>>>> upstream-releases
}
void TraceLoggerThreadState::disableTextId(JSContext* cx, uint32_t textId) {
  MOZ_ASSERT(TLTextIdIsTogglable(textId));

<<<<<<< HEAD
void TraceLoggerThreadState::enableTextId(JSContext* cx, uint32_t textId) {
  MOZ_ASSERT(TLTextIdIsTogglable(textId));

  if (enabledTextIds[textId]) {
    return;
  }
||||||| merged common ancestors
void
TraceLoggerThreadState::enableTextId(JSContext* cx, uint32_t textId)
{
    MOZ_ASSERT(TLTextIdIsTogglable(textId));

    if (enabledTextIds[textId]) {
        return;
    }
=======
  if (!enabledTextIds[textId]) {
    return;
  }
>>>>>>> upstream-releases

  ReleaseAllJITCode(cx->runtime()->defaultFreeOp());

<<<<<<< HEAD
  enabledTextIds[textId] = true;
  if (textId == TraceLogger_Engine) {
    enabledTextIds[TraceLogger_IonMonkey] = true;
    enabledTextIds[TraceLogger_Baseline] = true;
    enabledTextIds[TraceLogger_Interpreter] = true;
  }
||||||| merged common ancestors
    enabledTextIds[textId] = true;
    if (textId == TraceLogger_Engine) {
        enabledTextIds[TraceLogger_IonMonkey] = true;
        enabledTextIds[TraceLogger_Baseline] = true;
        enabledTextIds[TraceLogger_Interpreter] = true;
    }

    if (textId == TraceLogger_Scripts) {
        jit::ToggleBaselineTraceLoggerScripts(cx->runtime(), true);
    }
    if (textId == TraceLogger_Engine) {
        jit::ToggleBaselineTraceLoggerEngine(cx->runtime(), true);
    }
=======
  enabledTextIds[textId] = false;
  if (textId == TraceLogger_Engine) {
    enabledTextIds[TraceLogger_IonMonkey] = false;
    enabledTextIds[TraceLogger_Baseline] = false;
    enabledTextIds[TraceLogger_Interpreter] = false;
  }

  if (textId == TraceLogger_Scripts) {
    jit::ToggleBaselineTraceLoggerScripts(cx->runtime(), false);
  }
  if (textId == TraceLogger_Engine) {
    jit::ToggleBaselineTraceLoggerEngine(cx->runtime(), false);
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (textId == TraceLogger_Scripts) {
    jit::ToggleBaselineTraceLoggerScripts(cx->runtime(), true);
  }
  if (textId == TraceLogger_Engine) {
    jit::ToggleBaselineTraceLoggerEngine(cx->runtime(), true);
  }
||||||| merged common ancestors
=======
TraceLoggerThread* js::TraceLoggerForCurrentThread(JSContext* maybecx) {
  if (!traceLoggerState) {
    return nullptr;
  }
  return traceLoggerState->forCurrentThread(maybecx);
>>>>>>> upstream-releases
}
<<<<<<< HEAD
void TraceLoggerThreadState::disableTextId(JSContext* cx, uint32_t textId) {
  MOZ_ASSERT(TLTextIdIsTogglable(textId));
||||||| merged common ancestors
void
TraceLoggerThreadState::disableTextId(JSContext* cx, uint32_t textId)
{
    MOZ_ASSERT(TLTextIdIsTogglable(textId));
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!enabledTextIds[textId]) {
    return;
  }
||||||| merged common ancestors
    if (!enabledTextIds[textId]) {
        return;
    }
=======
TraceLoggerThread* TraceLoggerThreadState::forCurrentThread(
    JSContext* maybecx) {
  if (!jit::JitOptions.enableTraceLogger) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ReleaseAllJITCode(cx->runtime()->defaultFreeOp());
||||||| merged common ancestors
    ReleaseAllJITCode(cx->runtime()->defaultFreeOp());
=======
  MOZ_ASSERT(initialized);
  MOZ_ASSERT_IF(maybecx, maybecx == TlsContext.get());
>>>>>>> upstream-releases

<<<<<<< HEAD
  enabledTextIds[textId] = false;
  if (textId == TraceLogger_Engine) {
    enabledTextIds[TraceLogger_IonMonkey] = false;
    enabledTextIds[TraceLogger_Baseline] = false;
    enabledTextIds[TraceLogger_Interpreter] = false;
  }
||||||| merged common ancestors
    enabledTextIds[textId] = false;
    if (textId == TraceLogger_Engine) {
        enabledTextIds[TraceLogger_IonMonkey] = false;
        enabledTextIds[TraceLogger_Baseline] = false;
        enabledTextIds[TraceLogger_Interpreter] = false;
    }
=======
  JSContext* cx = maybecx ? maybecx : TlsContext.get();
  if (!cx) {
    return nullptr;
  }

  if (!cx->traceLogger) {
    LockGuard<Mutex> guard(lock);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (textId == TraceLogger_Scripts) {
    jit::ToggleBaselineTraceLoggerScripts(cx->runtime(), false);
  }
  if (textId == TraceLogger_Engine) {
    jit::ToggleBaselineTraceLoggerEngine(cx->runtime(), false);
  }
}
||||||| merged common ancestors
    if (textId == TraceLogger_Scripts) {
        jit::ToggleBaselineTraceLoggerScripts(cx->runtime(), false);
    }
    if (textId == TraceLogger_Engine) {
        jit::ToggleBaselineTraceLoggerEngine(cx->runtime(), false);
    }
}
=======
    TraceLoggerThread* logger = js_new<TraceLoggerThread>(cx);
    if (!logger) {
      return nullptr;
    }

    if (!logger->init()) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
TraceLoggerThread* js::TraceLoggerForCurrentThread(JSContext* maybecx) {
  if (!EnsureTraceLoggerState()) {
    return nullptr;
  }
  return traceLoggerState->forCurrentThread(maybecx);
}
||||||| merged common ancestors
TraceLoggerThread*
js::TraceLoggerForCurrentThread(JSContext* maybecx)
{
    if (!EnsureTraceLoggerState()) {
        return nullptr;
    }
    return traceLoggerState->forCurrentThread(maybecx);
}
=======
    threadLoggers.insertFront(logger);
    cx->traceLogger = logger;

    if (graphEnabled) {
      logger->initGraph();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
TraceLoggerThread* TraceLoggerThreadState::forCurrentThread(
    JSContext* maybecx) {
  if (!jit::JitOptions.enableTraceLogger) {
    return nullptr;
  }
||||||| merged common ancestors
TraceLoggerThread*
TraceLoggerThreadState::forCurrentThread(JSContext* maybecx)
{
    if (!jit::JitOptions.enableTraceLogger) {
        return nullptr;
    }
=======
    if (CurrentHelperThread() ? helperThreadEnabled : mainThreadEnabled) {
      logger->enable();
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(initialized);
  MOZ_ASSERT_IF(maybecx, maybecx == TlsContext.get());
||||||| merged common ancestors
    MOZ_ASSERT(initialized);
    MOZ_ASSERT_IF(maybecx, maybecx == TlsContext.get());
=======
  return cx->traceLogger;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSContext* cx = maybecx ? maybecx : TlsContext.get();
  if (!cx) {
    return nullptr;
  }
||||||| merged common ancestors
    JSContext* cx = maybecx ? maybecx : TlsContext.get();
    if (!cx) {
        return nullptr;
    }
=======
void TraceLoggerThreadState::destroyLogger(TraceLoggerThread* logger) {
  MOZ_ASSERT(initialized);
  MOZ_ASSERT(logger);
  LockGuard<Mutex> guard(lock);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!cx->traceLogger) {
    LockGuard<Mutex> guard(lock);
||||||| merged common ancestors
    if (!cx->traceLogger) {
        LockGuard<Mutex> guard(lock);
=======
  logger->remove();
  js_delete(logger);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    TraceLoggerThread* logger = js_new<TraceLoggerThread>();
    if (!logger) {
      return nullptr;
    }
||||||| merged common ancestors
        TraceLoggerThread* logger = js_new<TraceLoggerThread>();
        if (!logger) {
            return nullptr;
        }
=======
bool js::TraceLogTextIdEnabled(uint32_t textId) {
  if (!traceLoggerState) {
    return false;
  }
  return traceLoggerState->isTextIdEnabled(textId);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!logger->init()) {
      return nullptr;
    }
||||||| merged common ancestors
        if (!logger->init()) {
            return nullptr;
        }
=======
void js::TraceLogEnableTextId(JSContext* cx, uint32_t textId) {
  if (!traceLoggerState) {
    return;
  }
  traceLoggerState->enableTextId(cx, textId);
}
void js::TraceLogDisableTextId(JSContext* cx, uint32_t textId) {
  if (!traceLoggerState) {
    return;
  }
  traceLoggerState->disableTextId(cx, textId);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    threadLoggers.insertFront(logger);
    cx->traceLogger = logger;
||||||| merged common ancestors
        threadLoggers.insertFront(logger);
        cx->traceLogger = logger;
=======
TraceLoggerEvent::TraceLoggerEvent(TraceLoggerTextId type, JSScript* script)
    : TraceLoggerEvent(type, script->filename(), script->lineno(),
                       script->column()) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (graphEnabled) {
      logger->initGraph();
    }
||||||| merged common ancestors
        if (graphEnabled) {
            logger->initGraph();
        }
=======
TraceLoggerEvent::TraceLoggerEvent(TraceLoggerTextId type, const char* filename,
                                   uint32_t line, uint32_t column)
    : payload_() {
  MOZ_ASSERT(
      type == TraceLogger_Scripts || type == TraceLogger_AnnotateScripts ||
      type == TraceLogger_InlinedScripts || type == TraceLogger_Frontend);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (CurrentHelperThread() ? helperThreadEnabled : mainThreadEnabled) {
      logger->enable();
    }
  }
||||||| merged common ancestors
        if (CurrentHelperThread() ? helperThreadEnabled : mainThreadEnabled) {
            logger->enable();
        }
    }
=======
  if (!traceLoggerState || !jit::JitOptions.enableTraceLogger) {
    return;
  }

  // Only log scripts when enabled, otherwise use the more generic type
  // (which will get filtered out).
  if (!traceLoggerState->isTextIdEnabled(type)) {
    payload_.setTextId(type);
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return cx->traceLogger;
||||||| merged common ancestors
    return cx->traceLogger;
=======
  payload_.setEventPayload(
      traceLoggerState->getOrCreateEventPayload(filename, line, column));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void TraceLoggerThreadState::destroyLogger(TraceLoggerThread* logger) {
  MOZ_ASSERT(initialized);
  MOZ_ASSERT(logger);
  LockGuard<Mutex> guard(lock);
||||||| merged common ancestors
void
TraceLoggerThreadState::destroyLogger(TraceLoggerThread* logger)
{
    MOZ_ASSERT(initialized);
    MOZ_ASSERT(logger);
    LockGuard<Mutex> guard(lock);
=======
TraceLoggerEvent::TraceLoggerEvent(const char* text) : payload_() {
  if (jit::JitOptions.enableTraceLogger && traceLoggerState) {
    payload_.setEventPayload(traceLoggerState->getOrCreateEventPayload(text));
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  logger->remove();
  js_delete(logger);
||||||| merged common ancestors
    logger->remove();
    js_delete(logger);
=======
TraceLoggerEvent::~TraceLoggerEvent() {
  if (hasExtPayload()) {
    extPayload()->release();
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool js::TraceLogTextIdEnabled(uint32_t textId) {
  if (!EnsureTraceLoggerState()) {
    return false;
  }
  return traceLoggerState->isTextIdEnabled(textId);
||||||| merged common ancestors
bool
js::TraceLogTextIdEnabled(uint32_t textId)
{
    if (!EnsureTraceLoggerState()) {
        return false;
    }
    return traceLoggerState->isTextIdEnabled(textId);
=======
uint32_t TraceLoggerEvent::textId() const {
  MOZ_ASSERT(hasTextId());
  if (hasExtPayload()) {
    return extPayload()->textId();
  }
  return payload_.textId();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void js::TraceLogEnableTextId(JSContext* cx, uint32_t textId) {
  if (!EnsureTraceLoggerState()) {
    return;
  }
  traceLoggerState->enableTextId(cx, textId);
||||||| merged common ancestors
void
js::TraceLogEnableTextId(JSContext* cx, uint32_t textId)
{
    if (!EnsureTraceLoggerState()) {
        return;
    }
    traceLoggerState->enableTextId(cx, textId);
=======
TraceLoggerEvent& TraceLoggerEvent::operator=(const TraceLoggerEvent& other) {
  if (other.hasExtPayload()) {
    other.extPayload()->use();
  }
  if (hasExtPayload()) {
    extPayload()->release();
  }

  payload_ = other.payload_;

  return *this;
>>>>>>> upstream-releases
}
<<<<<<< HEAD
void js::TraceLogDisableTextId(JSContext* cx, uint32_t textId) {
  if (!EnsureTraceLoggerState()) {
    return;
  }
  traceLoggerState->disableTextId(cx, textId);
||||||| merged common ancestors
void
js::TraceLogDisableTextId(JSContext* cx, uint32_t textId)
{
    if (!EnsureTraceLoggerState()) {
        return;
    }
    traceLoggerState->disableTextId(cx, textId);
=======

TraceLoggerEvent::TraceLoggerEvent(const TraceLoggerEvent& other)
    : payload_(other.payload_) {
  if (hasExtPayload()) {
    extPayload()->use();
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
TraceLoggerEvent::TraceLoggerEvent(TraceLoggerTextId type, JSScript* script)
    : TraceLoggerEvent(type, script->filename(), script->lineno(),
                       script->column()) {}
||||||| merged common ancestors
TraceLoggerEvent::TraceLoggerEvent(TraceLoggerTextId type, JSScript* script)
  : TraceLoggerEvent(type, script->filename(), script->lineno(), script->column())
{ }
=======
JS_PUBLIC_API bool JS::InitTraceLogger() {
  MOZ_RELEASE_ASSERT(!traceLoggerState);
>>>>>>> upstream-releases

<<<<<<< HEAD
TraceLoggerEvent::TraceLoggerEvent(TraceLoggerTextId type, const char* filename,
                                   uint32_t line, uint32_t column)
    : payload_() {
  MOZ_ASSERT(
      type == TraceLogger_Scripts || type == TraceLogger_AnnotateScripts ||
      type == TraceLogger_InlinedScripts || type == TraceLogger_Frontend);
||||||| merged common ancestors
TraceLoggerEvent::TraceLoggerEvent(TraceLoggerTextId type, const char* filename, uint32_t line,
                                   uint32_t column)
  : payload_()
{
    MOZ_ASSERT(type == TraceLogger_Scripts || type == TraceLogger_AnnotateScripts ||
               type == TraceLogger_InlinedScripts || type == TraceLogger_Frontend);
=======
  if (!getTraceLoggerSupported()) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!traceLoggerState || !jit::JitOptions.enableTraceLogger) {
    return;
  }
||||||| merged common ancestors
    if (!traceLoggerState || !jit::JitOptions.enableTraceLogger) {
        return;
    }
=======
  traceLoggerState = js_new<TraceLoggerThreadState>();
  if (!traceLoggerState) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Only log scripts when enabled, otherwise use the more generic type
  // (which will get filtered out).
  if (!traceLoggerState->isTextIdEnabled(type)) {
    payload_.setTextId(type);
    return;
  }
||||||| merged common ancestors
    // Only log scripts when enabled, otherwise use the more generic type
    // (which will get filtered out).
    if (!traceLoggerState->isTextIdEnabled(type)) {
        payload_.setTextId(type);
        return;
    }
=======
  if (!traceLoggerState->init()) {
    DestroyTraceLoggerThreadState();
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  payload_.setEventPayload(
      traceLoggerState->getOrCreateEventPayload(filename, line, column));
||||||| merged common ancestors
    payload_.setEventPayload(
        traceLoggerState->getOrCreateEventPayload(filename, line, column));
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
TraceLoggerEvent::TraceLoggerEvent(const char* text) : payload_() {
  if (jit::JitOptions.enableTraceLogger && traceLoggerState) {
    payload_.setEventPayload(traceLoggerState->getOrCreateEventPayload(text));
  }
||||||| merged common ancestors
TraceLoggerEvent::TraceLoggerEvent(const char* text)
  : payload_()
{
    if (jit::JitOptions.enableTraceLogger && traceLoggerState) {
        payload_.setEventPayload(traceLoggerState->getOrCreateEventPayload(text));
    }
=======
JS_PUBLIC_API bool JS::TraceLoggerSupported() { return traceLoggerState; }

// Perform a process wide synchronous spew of every thread that tracelogger has
// captured.
void TraceLoggerThreadState::spewTraceLoggerStats() {
  for (TraceLoggerThread* logger : threadLoggers) {
    logger->spewTraceLoggerStats();
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
TraceLoggerEvent::~TraceLoggerEvent() {
  if (hasExtPayload()) {
    extPayload()->release();
  }
}
||||||| merged common ancestors
TraceLoggerEvent::~TraceLoggerEvent()
{
    if (hasExtPayload()) {
        extPayload()->release();
    }
}
=======
// Usage here is JS_TRACELOGGER_SPEW=<event1>,<event2>,etc for custom spewing.
// If the environment variable is not found, we use a default set of events.
static bool getSpewIds(EventVector& spewIds) {
  const char* env = getenv("JS_TRACELOGGER_SPEW");
  if (env) {
    for (uint32_t i = 1; i < TraceLogger_Last; i++) {
      TraceLoggerTextId id = TraceLoggerTextId(i);
      if (ContainsFlag(env, TLTextIdString(id))) {
        if (!spewIds.append(id)) {
          return false;
        }
      }
    }
  } else {
    const uint32_t defaultSpewEvents[] = {
        TraceLogger_ParsingFull, TraceLogger_Interpreter,
        TraceLogger_Baseline,    TraceLogger_BaselineCompilation,
        TraceLogger_IonMonkey,   TraceLogger_IonCompilation,
        TraceLogger_Bailout};
>>>>>>> upstream-releases

<<<<<<< HEAD
uint32_t TraceLoggerEvent::textId() const {
  MOZ_ASSERT(hasTextId());
  if (hasExtPayload()) {
    return extPayload()->textId();
  }
  return payload_.textId();
||||||| merged common ancestors
uint32_t
TraceLoggerEvent::textId() const
{
    MOZ_ASSERT(hasTextId());
    if (hasExtPayload()) {
        return extPayload()->textId();
    }
    return payload_.textId();
=======
    for (uint32_t id : defaultSpewEvents) {
      if (!spewIds.append(id)) {
        return false;
      }
    }
  }

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
TraceLoggerEvent& TraceLoggerEvent::operator=(const TraceLoggerEvent& other) {
  if (other.hasExtPayload()) {
    other.extPayload()->use();
  }
  if (hasExtPayload()) {
    extPayload()->release();
  }
||||||| merged common ancestors
TraceLoggerEvent&
TraceLoggerEvent::operator=(const TraceLoggerEvent& other)
{
    if (other.hasExtPayload()) {
        other.extPayload()->use();
    }
    if (hasExtPayload()) {
        extPayload()->release();
    }
=======
static void spewHeaderRow(UniqueChars& threadName, EventVector& spewIds) {
  if (threadName) {
    JitSpew(jit::JitSpew_ScriptStats, "Thread: %s (pid=%d)", threadName.get(),
            getpid());
  } else {
    JitSpew(jit::JitSpew_ScriptStats, "Unknown Thread (pid=%d)", getpid());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  payload_ = other.payload_;
||||||| merged common ancestors
    payload_ = other.payload_;
=======
  UniqueChars header = JS_smprintf("%10s ", "totalTime");
  for (uint32_t i : spewIds) {
    TraceLoggerTextId id = TraceLoggerTextId(i);
    if (TLTextIdIsLogEvent(id)) {
      header =
          JS_sprintf_append(std::move(header), "%12s ", TLTextIdString(id));
    } else {
      header =
          JS_sprintf_append(std::move(header), "%25s ", TLTextIdString(id));
    }
  }

  JitSpew(jit::JitSpew_ScriptStats, "%s Script", header.get());
}

void TraceLoggerThread::spewTraceLoggerStats() {
  if (!jit::JitOptions.enableTraceLogger) {
    return;
  }

  ScriptMap map;
  if (!collectTraceLoggerStats(map)) {
    return;
  }
  if (map.empty()) {
    return;
  }

  SortedStatsVector sorted_map;
  if (!sortTraceLoggerStats(map, sorted_map)) {
    return;
  }
  map.clearAndCompact();

  EventVector spewIds;
  if (!getSpewIds(spewIds)) {
    return;
  }

  // Dynamically generate the header row in JitSpew.
  spewHeaderRow(threadName_, spewIds);

  for (UniquePtr<ScriptStats>& datap : sorted_map) {
    auto& tlevents = datap->events_;
    uint32_t selfTime = datap->selfTime;

    if (selfTime == 0) {
      continue;
    }

    UniqueChars row = JS_smprintf("%10u ", selfTime);
    for (uint32_t i : spewIds) {
      TraceLoggerTextId id = TraceLoggerTextId(i);
      uint32_t time = tlevents[id].time;
      uint32_t freq = tlevents[id].count;
      uint32_t percent = (time * 100) / selfTime;
      if (TLTextIdIsLogEvent(id)) {
        row = JS_sprintf_append(std::move(row), "%12u ", freq);
      } else {
        row = JS_sprintf_append(std::move(row), "%8u (%3u%%,f=%-7u) ", time,
                                percent, freq);
      }
    }
    JitSpew(jit::JitSpew_ScriptStats, "%s %s", row.get(), datap->scriptName);

    // If structured spewer is enabled, we might as well spew everything.
    AutoStructuredSpewer spew(cx_, SpewChannel::ScriptStats, nullptr);
    if (spew) {
      spew->property("script", datap->scriptName);
      spew->property("totalTime", selfTime);
      spew->beginListProperty("events");
      for (uint32_t i = 1; i < TraceLogger_Last; i++) {
        TraceLoggerTextId id = TraceLoggerTextId(i);
        if (TLTextIdIsInternalEvent(id) || tlevents[id].count == 0) {
          continue;
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return *this;
||||||| merged common ancestors
    return *this;
=======
        spew->beginObject();
        spew->property("id", TLTextIdString(id));
        if (TLTextIdIsTreeEvent(id)) {
          spew->property("time", tlevents[id].time);
          spew->property("frequency", tlevents[id].count);
        } else if (TLTextIdIsLogEvent(id)) {
          spew->property("frequency", tlevents[id].count);
        }
        spew->endObject();
      }
      spew->endList();
    }
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
TraceLoggerEvent::TraceLoggerEvent(const TraceLoggerEvent& other)
    : payload_(other.payload_) {
  if (hasExtPayload()) {
    extPayload()->use();
  }
||||||| merged common ancestors
TraceLoggerEvent::TraceLoggerEvent(const TraceLoggerEvent& other)
  : payload_(other.payload_)
{
    if (hasExtPayload()) {
        extPayload()->use();
    }
=======
static bool updateScriptMap(ScriptMap& map, char* key, uint32_t eventId,
                            uint32_t value) {
  if (!key) {
    return false;
  }

  if (!map.has(key)) {
    UniquePtr<ScriptStats> datap;
    datap.reset(js_new<ScriptStats>(key));
    if (!map.putNew(key, std::move(datap))) {
      return false;
    }
  }
  ScriptMap::Ptr p = map.lookup(key);
  p->value()->events_[eventId].time += value;
  p->value()->events_[eventId].count++;

  if (TLTextIdIsTreeEvent(eventId)) {
    p->value()->selfTime += value;
  }
  return true;
}

UniqueChars TraceLoggerThreadState::getFullScriptName(uint32_t textId) {
  TraceLoggerEventPayload* payload = getPayload(textId);
  MOZ_ASSERT(payload);
  if (!payload) {
    return nullptr;
  }
  char* filename = dictionaryData[payload->dictionaryId()].get();
  uint32_t lineno = payload->line() ? *(payload->line()) : 0;
  uint32_t colno = payload->column() ? *(payload->column()) : 0;
  UniqueChars scriptName = JS_smprintf("%s:%u:%u", filename, lineno, colno);
  payload->release();
  return scriptName;
}

static bool sortBySelfTime(const UniquePtr<ScriptStats>& lhs,
                           const UniquePtr<ScriptStats>& rhs) {
  return lhs.get()->selfTime > rhs.get()->selfTime;
}

bool TraceLoggerThread::sortTraceLoggerStats(ScriptMap& map,
                                             SortedStatsVector& sorted_map) {
  for (auto range = map.all(); !range.empty(); range.popFront()) {
    if (!sorted_map.append(std::move(range.front().value()))) {
      return false;
    }
  }

  std::sort(sorted_map.begin(), sorted_map.end(), sortBySelfTime);

  return true;
}

// Traverse each event and calculate the self-time, along with the script that
// each event belongs to. We do this quickly by maintaining two stacks:
//  (i) eventStack - Each new event encountered is pushed onto the stack. Events
//                   are popped off whenever a TraceLogger_Stop is encountered
//                   and sent to updateScriptMap.
// (ii) funcStack - Each new script encountered is pushed onto this stack.
//                  Elements are popped off whenever a TraceLogger_Stop is
//                  encountered that matches a script event on the top of
//                  eventStack.
bool TraceLoggerThread::collectTraceLoggerStats(ScriptMap& map) {
  uint32_t totalJSTime = 0;

  struct eventInfo {
    uint32_t textId;
    uint32_t time;
    mozilla::TimeStamp start;

    explicit eventInfo(uint32_t textId_) : textId(textId_), time(0) {}
  };

  Vector<eventInfo*, 0, js::SystemAllocPolicy> eventStack;
  Vector<uint32_t, 0, js::SystemAllocPolicy> funcStack;

  mozilla::TimeStamp startTime, stopTime;
  uint32_t size = events_.size();
  for (size_t i = 0; i < size; i++) {
    uint32_t textId = events_[i].textId;

    // Record any log events that have no durations such as Bailouts with a
    // value of 1.  Make sure the funcStack actually has something in it or
    // else the Bailout event will not be associated with any script.  This
    // can commonly occur when profiling & tracing starts since we may have
    // already past the point where the script event is created.
    if (TLTextIdIsLogEvent(textId) && !funcStack.empty()) {
      UniqueChars script =
          traceLoggerState->getFullScriptName(funcStack.back());
      if (!updateScriptMap(map, script.release(), textId, 1)) {
        return false;
      }
    }

    // Hit a new tree event or a stop event, so add (new event timestamp - old
    // event timestamp) to the old event's self-time.
    if (TLTextIdIsTreeEvent(textId)) {
      if (!eventStack.empty()) {
        stopTime = events_[i].time;

        uint32_t deltaTime =
            static_cast<uint32_t>((stopTime - startTime).ToMicroseconds());
        eventStack.back()->time += deltaTime;

        if (TLTextIdIsEnumEvent(eventStack.back()->textId)) {
          if (!funcStack.empty() && !eventStack.empty()) {
            UniqueChars script =
                traceLoggerState->getFullScriptName(funcStack.back());
            if (!updateScriptMap(map, script.release(),
                                 eventStack.back()->textId, deltaTime)) {
              return false;
            }
          }
        }
        totalJSTime += deltaTime;
      }

      if (TLTextIdIsScriptEvent(textId)) {
        if (!funcStack.append(textId)) {
          return false;
        }
      }

      eventInfo* entry = js_new<eventInfo>(textId);
      entry->start = events_[i].time;
      if (!eventStack.append(entry)) {
        return false;
      }

      startTime = events_[i].time;

    } else if (textId == TraceLogger_Stop) {
      if (!eventStack.empty()) {
        stopTime = events_[i].time;

        uint32_t deltaTime =
            static_cast<uint32_t>((stopTime - startTime).ToMicroseconds());
        eventInfo* entry = eventStack.popCopy();

        uint32_t topId = entry->textId;
        entry->time += deltaTime;

        if (TLTextIdIsEnumEvent(topId)) {
          // funcStack will usually be empty near the beginning of a profiling
          // session since we may have skipped the point where the script event
          // is created.  If that's the case, then skip this event since we
          // cannot associate it with any script.
          if (!funcStack.empty()) {
            UniqueChars script =
                traceLoggerState->getFullScriptName(funcStack.back());
            if (!updateScriptMap(map, script.release(), topId, deltaTime)) {
              return false;
            }
          }
        }
        js_delete(entry);

        if (TLTextIdIsScriptEvent(topId) && !funcStack.empty()) {
          funcStack.popBack();
        }

        totalJSTime += deltaTime;
        startTime = events_[i].time;
      }
    }
  }

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_PUBLIC_API void JS::ResetTraceLogger(void) { js::ResetTraceLogger(); }
||||||| merged common ancestors
JS_PUBLIC_API(void)
JS::ResetTraceLogger(void)
{
    js::ResetTraceLogger();
}
=======
JS_PUBLIC_API void JS::ResetTraceLogger(void) { js::ResetTraceLogger(); }

JS_PUBLIC_API void JS::SpewTraceLoggerThread(JSContext* cx) {
  if (!traceLoggerState) {
    return;
  }

  if (cx && cx->traceLogger) {
    cx->traceLogger->spewTraceLoggerStats();
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_PUBLIC_API void JS::StartTraceLogger(JSContext* cx) {
  if (!EnsureTraceLoggerState()) {
    return;
  }
||||||| merged common ancestors
JS_PUBLIC_API(void)
JS::StartTraceLogger(JSContext *cx)
{
    if (jit::JitOptions.enableTraceLogger || !traceLoggerState)  {
        return;
    }
=======
JS_PUBLIC_API void JS::SpewTraceLoggerForCurrentProcess() {
  if (!traceLoggerState) {
    return;
  }
  traceLoggerState->spewTraceLoggerStats();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!jit::JitOptions.enableTraceLogger) {
||||||| merged common ancestors
=======
JS_PUBLIC_API void JS::StartTraceLogger(JSContext* cx) {
  if (!traceLoggerState) {
    return;
  }

  if (!jit::JitOptions.enableTraceLogger) {
>>>>>>> upstream-releases
    LockGuard<Mutex> guard(traceLoggerState->lock);
    traceLoggerState->enableTextIdsForProfiler();
    jit::JitOptions.enableTraceLogger = true;
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
    traceLoggerState->startTime = mozilla::TimeStamp::Now();
  }
>>>>>>> upstream-releases

  TraceLoggerThread* logger = traceLoggerState->forCurrentThread(cx);
  if (logger) {
    logger->enable();
  }
}

JS_PUBLIC_API void JS::StopTraceLogger(JSContext* cx) {
  if (!traceLoggerState) {
    return;
  }

  if (jit::JitOptions.enableTraceLogger) {
    LockGuard<Mutex> guard(traceLoggerState->lock);
    traceLoggerState->disableTextIdsForProfiler();
    jit::JitOptions.enableTraceLogger = false;
  }

  TraceLoggerThread* logger = traceLoggerState->forCurrentThread(cx);
  if (logger) {
    logger->disable();
  }
}
