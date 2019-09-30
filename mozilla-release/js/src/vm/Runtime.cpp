/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/ArrayUtils.h"
#include "mozilla/Atomics.h"
#include "mozilla/DebugOnly.h"
#include "mozilla/MemoryReporting.h"
#include "mozilla/ThreadLocal.h"
#include "mozilla/Unused.h"

#if defined(XP_DARWIN)
<<<<<<< HEAD
#include <mach/mach.h>
||||||| merged common ancestors
# include <mach/mach.h>
=======
#  include <mach/mach.h>
>>>>>>> upstream-releases
#elif defined(XP_UNIX)
<<<<<<< HEAD
#include <sys/resource.h>
#endif  // defined(XP_DARWIN) || defined(XP_UNIX) || defined(XP_WIN)
||||||| merged common ancestors
# include <sys/resource.h>
#endif // defined(XP_DARWIN) || defined(XP_UNIX) || defined(XP_WIN)
=======
#  include <sys/resource.h>
#endif  // defined(XP_DARWIN) || defined(XP_UNIX) || defined(XP_WIN)
>>>>>>> upstream-releases
#include <locale.h>
#include <string.h>
#ifdef JS_CAN_CHECK_THREADSAFE_ACCESSES
<<<<<<< HEAD
#include <sys/mman.h>
||||||| merged common ancestors
# include <sys/mman.h>
=======
#  include <sys/mman.h>
>>>>>>> upstream-releases
#endif

#include "jsfriendapi.h"
#include "jsmath.h"

#include "builtin/Promise.h"
#include "gc/FreeOp.h"
#include "gc/GCInternals.h"
#include "gc/PublicIterators.h"
#include "jit/arm/Simulator-arm.h"
#include "jit/arm64/vixl/Simulator-vixl.h"
#include "jit/IonBuilder.h"
#include "jit/JitRealm.h"
#include "jit/mips32/Simulator-mips32.h"
#include "jit/mips64/Simulator-mips64.h"
#include "js/Date.h"
#include "js/MemoryMetrics.h"
#include "js/SliceBudget.h"
#include "js/StableStringChars.h"
#include "js/Wrapper.h"
#if EXPOSE_INTL_API
#  include "unicode/uloc.h"
#endif
#include "util/Windows.h"
#include "vm/DateTime.h"
#include "vm/Debugger.h"
#include "vm/JSAtom.h"
#include "vm/JSObject.h"
#include "vm/JSScript.h"
#include "vm/TraceLogging.h"
#include "vm/TraceLoggingGraph.h"
#include "wasm/WasmSignalHandlers.h"

#include "gc/GC-inl.h"
#include "vm/JSContext-inl.h"
#include "vm/Realm-inl.h"

using namespace js;

<<<<<<< HEAD
using JS::AutoStableStringChars;
using JS::DoubleNaNValue;
||||||| merged common ancestors
=======
using JS::AutoStableStringChars;
>>>>>>> upstream-releases
using mozilla::Atomic;
using mozilla::DebugOnly;
using mozilla::NegativeInfinity;
using mozilla::PodZero;
using mozilla::PositiveInfinity;

/* static */ MOZ_THREAD_LOCAL(JSContext*) js::TlsContext;
/* static */
Atomic<size_t> JSRuntime::liveRuntimesCount;
Atomic<JS::LargeAllocationFailureCallback> js::OnLargeAllocationFailure;

namespace js {
<<<<<<< HEAD
bool gCanUseExtraThreads = true;
}  // namespace js
||||||| merged common ancestors
    bool gCanUseExtraThreads = true;
} // namespace js
=======
void (*HelperThreadTaskCallback)(js::RunnableTask*);
>>>>>>> upstream-releases

<<<<<<< HEAD
void js::DisableExtraThreads() { gCanUseExtraThreads = false; }
||||||| merged common ancestors
void
js::DisableExtraThreads()
{
    gCanUseExtraThreads = false;
}
=======
bool gCanUseExtraThreads = true;
}  // namespace js

void js::DisableExtraThreads() { gCanUseExtraThreads = false; }
>>>>>>> upstream-releases

const JSSecurityCallbacks js::NullSecurityCallbacks = {};

static const JSWrapObjectCallbacks DefaultWrapObjectCallbacks = {
    TransparentObjectWrapper, nullptr};

static size_t ReturnZeroSize(const void* p) { return 0; }

JSRuntime::JSRuntime(JSRuntime* parentRuntime)
    : parentRuntime(parentRuntime),
#ifdef DEBUG
      updateChildRuntimeCount(parentRuntime),
      initialized_(false),
#endif
      mainContext_(nullptr),
      profilerSampleBufferRangeStart_(0),
      telemetryCallback(nullptr),
      consumeStreamCallback(nullptr),
      reportStreamErrorCallback(nullptr),
      hadOutOfMemory(false),
      allowRelazificationForTesting(false),
      destroyCompartmentCallback(nullptr),
      sizeOfIncludingThisCompartmentCallback(nullptr),
      destroyRealmCallback(nullptr),
      realmNameCallback(nullptr),
      externalStringSizeofCallback(nullptr),
      securityCallbacks(&NullSecurityCallbacks),
      DOMcallbacks(nullptr),
      destroyPrincipals(nullptr),
      readPrincipals(nullptr),
      warningReporter(nullptr),
      geckoProfiler_(thisFromCtor()),
      trustedPrincipals_(nullptr),
      wrapObjectCallbacks(&DefaultWrapObjectCallbacks),
      preserveWrapperCallback(nullptr),
      scriptEnvironmentPreparer(nullptr),
      ctypesActivityCallback(nullptr),
      windowProxyClass_(nullptr),
      scriptDataLock(mutexid::RuntimeScriptData),
#ifdef DEBUG
      activeThreadHasScriptDataAccess(false),
#endif
<<<<<<< HEAD
      numActiveHelperThreadZones(0),
      heapState_(JS::HeapState::Idle),
      numRealms(0),
      localeCallbacks(nullptr),
      defaultLocale(nullptr),
      profilingScripts(false),
      scriptAndCountsVector(nullptr),
      lcovOutput_(),
      jitRuntime_(nullptr),
      selfHostingGlobal_(nullptr),
      gc(thisFromCtor()),
      gcInitialized(false),
      NaNValue(DoubleNaNValue()),
      negativeInfinityValue(DoubleValue(NegativeInfinity<double>())),
      positiveInfinityValue(DoubleValue(PositiveInfinity<double>())),
      emptyString(nullptr),
      defaultFreeOp_(nullptr),
||||||| merged common ancestors
    numActiveHelperThreadZones(0),
    heapState_(JS::HeapState::Idle),
    numRealms(0),
    localeCallbacks(nullptr),
    defaultLocale(nullptr),
    profilingScripts(false),
    scriptAndCountsVector(nullptr),
    lcovOutput_(),
    jitRuntime_(nullptr),
    selfHostingGlobal_(nullptr),
    gc(thisFromCtor()),
    gcInitialized(false),
    NaNValue(DoubleNaNValue()),
    negativeInfinityValue(DoubleValue(NegativeInfinity<double>())),
    positiveInfinityValue(DoubleValue(PositiveInfinity<double>())),
    emptyString(nullptr),
    defaultFreeOp_(nullptr),
=======
      numActiveHelperThreadZones(0),
      heapState_(JS::HeapState::Idle),
      numRealms(0),
      numDebuggeeRealms_(0),
      numDebuggeeRealmsObservingCoverage_(0),
      localeCallbacks(nullptr),
      defaultLocale(nullptr),
      profilingScripts(false),
      scriptAndCountsVector(nullptr),
      lcovOutput_(),
      jitRuntime_(nullptr),
      selfHostingGlobal_(nullptr),
      gc(thisFromCtor()),
      gcInitialized(false),
      emptyString(nullptr),
      defaultFreeOp_(nullptr),
>>>>>>> upstream-releases
#if !EXPOSE_INTL_API
      thousandsSeparator(nullptr),
      decimalSeparator(nullptr),
      numGrouping(nullptr),
#endif
      beingDestroyed_(false),
      allowContentJS_(true),
      atoms_(nullptr),
      permanentAtomsDuringInit_(nullptr),
      permanentAtoms_(nullptr),
      staticStrings(nullptr),
      commonNames(nullptr),
      wellKnownSymbols(nullptr),
      jitSupportsFloatingPoint(false),
      jitSupportsUnalignedAccesses(false),
      jitSupportsSimd(false),
      offthreadIonCompilationEnabled_(true),
      parallelParsingEnabled_(true),
#ifdef DEBUG
      offThreadParsesRunning_(0),
      offThreadParsingBlocked_(false),
#endif
<<<<<<< HEAD
      autoWritableJitCodeActive_(false),
      oomCallback(nullptr),
      debuggerMallocSizeOf(ReturnZeroSize),
      performanceMonitoring_(),
      stackFormat_(parentRuntime ? js::StackFormat::Default
                                 : js::StackFormat::SpiderMonkey),
      wasmInstances(mutexid::WasmRuntimeInstances),
      moduleResolveHook(),
      moduleMetadataHook(),
      moduleDynamicImportHook() {
  JS_COUNT_CTOR(JSRuntime);
  liveRuntimesCount++;
||||||| merged common ancestors
    autoWritableJitCodeActive_(false),
    oomCallback(nullptr),
    debuggerMallocSizeOf(ReturnZeroSize),
    performanceMonitoring_(),
    stackFormat_(parentRuntime ? js::StackFormat::Default
                               : js::StackFormat::SpiderMonkey),
    wasmInstances(mutexid::WasmRuntimeInstances),
    moduleResolveHook(),
    moduleMetadataHook()
{
    JS_COUNT_CTOR(JSRuntime);
    liveRuntimesCount++;
=======
      autoWritableJitCodeActive_(false),
      oomCallback(nullptr),
      debuggerMallocSizeOf(ReturnZeroSize),
      stackFormat_(parentRuntime ? js::StackFormat::Default
                                 : js::StackFormat::SpiderMonkey),
      wasmInstances(mutexid::WasmRuntimeInstances),
      moduleResolveHook(),
      moduleMetadataHook(),
      moduleDynamicImportHook(),
      scriptPrivateAddRefHook(),
      scriptPrivateReleaseHook() {
  JS_COUNT_CTOR(JSRuntime);
  liveRuntimesCount++;
>>>>>>> upstream-releases

  // See function comment for why we call this now, not in JS_Init().
  wasm::EnsureEagerProcessSignalHandlers();
}

<<<<<<< HEAD
JSRuntime::~JSRuntime() {
  JS_COUNT_DTOR(JSRuntime);
  MOZ_ASSERT(!initialized_);
||||||| merged common ancestors
JSRuntime::~JSRuntime()
{
    JS_COUNT_DTOR(JSRuntime);
    MOZ_ASSERT(!initialized_);
=======
JSRuntime::~JSRuntime() {
  JS_COUNT_DTOR(JSRuntime);
  MOZ_ASSERT(!initialized_);

  DebugOnly<size_t> oldCount = liveRuntimesCount--;
  MOZ_ASSERT(oldCount > 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  DebugOnly<size_t> oldCount = liveRuntimesCount--;
  MOZ_ASSERT(oldCount > 0);
||||||| merged common ancestors
    DebugOnly<size_t> oldCount = liveRuntimesCount--;
    MOZ_ASSERT(oldCount > 0);
=======
  MOZ_ASSERT(wasmInstances.lock()->empty());
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(wasmInstances.lock()->empty());
||||||| merged common ancestors
    MOZ_ASSERT(wasmInstances.lock()->empty());
=======
  MOZ_ASSERT(offThreadParsesRunning_ == 0);
  MOZ_ASSERT(!offThreadParsingBlocked_);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(offThreadParsesRunning_ == 0);
  MOZ_ASSERT(!offThreadParsingBlocked_);
||||||| merged common ancestors
    MOZ_ASSERT(offThreadParsesRunning_ == 0);
    MOZ_ASSERT(!offThreadParsingBlocked_);
=======
  MOZ_ASSERT(numRealms == 0);
  MOZ_ASSERT(numDebuggeeRealms_ == 0);
  MOZ_ASSERT(numDebuggeeRealmsObservingCoverage_ == 0);
>>>>>>> upstream-releases
}

bool JSRuntime::init(JSContext* cx, uint32_t maxbytes,
                     uint32_t maxNurseryBytes) {
#ifdef DEBUG
  MOZ_ASSERT(!initialized_);
  initialized_ = true;
#endif

  if (CanUseExtraThreads() && !EnsureHelperThreadsInitialized()) {
    return false;
  }

  mainContext_ = cx;

<<<<<<< HEAD
  defaultFreeOp_ = js_new<js::FreeOp>(this);
  if (!defaultFreeOp_) {
    return false;
  }
||||||| merged common ancestors
    defaultFreeOp_ = js_new<js::FreeOp>(this);
    if (!defaultFreeOp_) {
        return false;
    }
=======
  defaultFreeOp_ = cx->defaultFreeOp();
>>>>>>> upstream-releases

  if (!gc.init(maxbytes, maxNurseryBytes)) {
    return false;
  }

  UniquePtr<Zone> atomsZone = MakeUnique<Zone>(this);
  if (!atomsZone || !atomsZone->init(true)) {
    return false;
  }

  gc.atomsZone = atomsZone.release();

  // The garbage collector depends on everything before this point being
  // initialized.
  gcInitialized = true;

  if (!InitRuntimeNumberState(this)) {
    return false;
  }

  js::ResetTimeZoneInternal(ResetTimeZoneMode::DontResetIfOffsetUnchanged);

  jitSupportsFloatingPoint = js::jit::JitSupportsFloatingPoint();
  jitSupportsUnalignedAccesses = js::jit::JitSupportsUnalignedAccesses();
  jitSupportsSimd = js::jit::JitSupportsSimd();

  if (!parentRuntime) {
    sharedImmutableStrings_ = js::SharedImmutableStringsCache::Create();
    if (!sharedImmutableStrings_) {
      return false;
    }
  }

  return true;
}

<<<<<<< HEAD
void JSRuntime::destroyRuntime() {
  MOZ_ASSERT(!JS::RuntimeHeapIsBusy());
  MOZ_ASSERT(childRuntimeCount == 0);
  MOZ_ASSERT(initialized_);
||||||| merged common ancestors
void
JSRuntime::destroyRuntime()
{
    MOZ_ASSERT(!JS::RuntimeHeapIsBusy());
    MOZ_ASSERT(childRuntimeCount == 0);
    MOZ_ASSERT(initialized_);

    sharedIntlData.ref().destroyInstance();
=======
void JSRuntime::destroyRuntime() {
  MOZ_ASSERT(!JS::RuntimeHeapIsBusy());
  MOZ_ASSERT(childRuntimeCount == 0);
  MOZ_ASSERT(initialized_);

  sharedIntlData.ref().destroyInstance();
>>>>>>> upstream-releases

<<<<<<< HEAD
  sharedIntlData.ref().destroyInstance();

  if (gcInitialized) {
    /*
     * Finish any in-progress GCs first. This ensures the parseWaitingOnGC
     * list is empty in CancelOffThreadParses.
     */
    JSContext* cx = mainContextFromOwnThread();
    if (JS::IsIncrementalGCInProgress(cx)) {
      gc::FinishGC(cx);
||||||| merged common ancestors
    if (gcInitialized) {
        /*
         * Finish any in-progress GCs first. This ensures the parseWaitingOnGC
         * list is empty in CancelOffThreadParses.
         */
        JSContext* cx = mainContextFromOwnThread();
        if (JS::IsIncrementalGCInProgress(cx)) {
            gc::FinishGC(cx);
        }

        /* Free source hook early, as its destructor may want to delete roots. */
        sourceHook = nullptr;

        /*
         * Cancel any pending, in progress or completed Ion compilations and
         * parse tasks. Waiting for wasm and compression tasks is done
         * synchronously (on the main thread or during parse tasks), so no
         * explicit canceling is needed for these.
         */
        CancelOffThreadIonCompile(this);
        CancelOffThreadParses(this);
        CancelOffThreadCompressions(this);

        /* Remove persistent GC roots. */
        gc.finishRoots();

        /*
         * Flag us as being destroyed. This allows the GC to free things like
         * interned atoms and Ion trampolines.
         */
        beingDestroyed_ = true;

        /* Allow the GC to release scripts that were being profiled. */
        profilingScripts = false;

        JS::PrepareForFullGC(cx);
        gc.gc(GC_NORMAL, JS::gcreason::DESTROY_RUNTIME);
=======
  if (gcInitialized) {
    /*
     * Finish any in-progress GCs first. This ensures the parseWaitingOnGC
     * list is empty in CancelOffThreadParses.
     */
    JSContext* cx = mainContextFromOwnThread();
    if (JS::IsIncrementalGCInProgress(cx)) {
      gc::FinishGC(cx);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    /* Free source hook early, as its destructor may want to delete roots. */
    sourceHook = nullptr;

    /*
     * Cancel any pending, in progress or completed Ion compilations and
     * parse tasks. Waiting for wasm and compression tasks is done
     * synchronously (on the main thread or during parse tasks), so no
     * explicit canceling is needed for these.
     */
    CancelOffThreadIonCompile(this);
    CancelOffThreadParses(this);
    CancelOffThreadCompressions(this);
||||||| merged common ancestors
    AutoNoteSingleThreadedRegion anstr;
=======
    /* Free source hook early, as its destructor may want to delete roots. */
    sourceHook = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
    /* Remove persistent GC roots. */
    gc.finishRoots();
||||||| merged common ancestors
    MOZ_ASSERT(!hasHelperThreadZones());
=======
    /*
     * Cancel any pending, in progress or completed Ion compilations and
     * parse tasks. Waiting for wasm and compression tasks is done
     * synchronously (on the main thread or during parse tasks), so no
     * explicit canceling is needed for these.
     */
    CancelOffThreadIonCompile(this);
    CancelOffThreadParses(this);
    CancelOffThreadCompressions(this);

    /* Remove persistent GC roots. */
    gc.finishRoots();
>>>>>>> upstream-releases

    /*
     * Flag us as being destroyed. This allows the GC to free things like
     * interned atoms and Ion trampolines.
     */
<<<<<<< HEAD
    beingDestroyed_ = true;

    /* Allow the GC to release scripts that were being profiled. */
    profilingScripts = false;

    JS::PrepareForFullGC(cx);
    gc.gc(GC_NORMAL, JS::gcreason::DESTROY_RUNTIME);
  }

  AutoNoteSingleThreadedRegion anstr;

  MOZ_ASSERT(!hasHelperThreadZones());

  /*
   * Even though all objects in the compartment are dead, we may have keep
   * some filenames around because of gcKeepAtoms.
   */
  FreeScriptData(this);
||||||| merged common ancestors
    FreeScriptData(this);
=======
    beingDestroyed_ = true;

    /* Allow the GC to release scripts that were being profiled. */
    profilingScripts = false;

    JS::PrepareForFullGC(cx);
    gc.gc(GC_NORMAL, JS::GCReason::DESTROY_RUNTIME);
  }

  AutoNoteSingleThreadedRegion anstr;

  MOZ_ASSERT(!hasHelperThreadZones());

  /*
   * Even though all objects in the compartment are dead, we may have keep
   * some filenames around because of gcKeepAtoms.
   */
  FreeScriptData(this);
>>>>>>> upstream-releases

#if !EXPOSE_INTL_API
  FinishRuntimeNumberState(this);
#endif

  gc.finish();

<<<<<<< HEAD
  js_delete(defaultFreeOp_.ref());

  defaultLocale = nullptr;
  js_delete(jitRuntime_.ref());
||||||| merged common ancestors
    js_delete(defaultFreeOp_.ref());

    defaultLocale = nullptr;
    js_delete(jitRuntime_.ref());
=======
  defaultLocale = nullptr;
  js_delete(jitRuntime_.ref());
>>>>>>> upstream-releases

#ifdef DEBUG
  initialized_ = false;
#endif
}

void JSRuntime::addTelemetry(int id, uint32_t sample, const char* key) {
  if (telemetryCallback) {
    (*telemetryCallback)(id, sample, key);
  }
}

void JSRuntime::setTelemetryCallback(
    JSRuntime* rt, JSAccumulateTelemetryDataCallback callback) {
  rt->telemetryCallback = callback;
}

void JSRuntime::setUseCounter(JSObject* obj, JSUseCounter counter) {
  if (useCounterCallback) {
    (*useCounterCallback)(obj, counter);
  }
}

void JSRuntime::setUseCounterCallback(JSRuntime* rt,
                                      JSSetUseCounterCallback callback) {
  rt->useCounterCallback = callback;
}

void JSRuntime::addSizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf,
                                       JS::RuntimeSizes* rtSizes) {
  rtSizes->object += mallocSizeOf(this);

  rtSizes->atomsTable += atoms().sizeOfIncludingThis(mallocSizeOf);
  rtSizes->gc.marker += gc.marker.sizeOfExcludingThis(mallocSizeOf);

  if (!parentRuntime) {
    rtSizes->atomsTable += mallocSizeOf(staticStrings);
    rtSizes->atomsTable += mallocSizeOf(commonNames);
    rtSizes->atomsTable += permanentAtoms()->sizeOfIncludingThis(mallocSizeOf);
  }

  JSContext* cx = mainContextFromAnyThread();
  rtSizes->contexts += mallocSizeOf(cx);
  rtSizes->contexts += cx->sizeOfExcludingThis(mallocSizeOf);
  rtSizes->temporary += cx->tempLifoAlloc().sizeOfExcludingThis(mallocSizeOf);
  rtSizes->interpreterStack +=
      cx->interpreterStack().sizeOfExcludingThis(mallocSizeOf);
#ifdef JS_TRACE_LOGGING
  if (cx->traceLogger) {
    rtSizes->tracelogger += cx->traceLogger->sizeOfIncludingThis(mallocSizeOf);
  }
#endif

  rtSizes->uncompressedSourceCache +=
      caches().uncompressedSourceCache.sizeOfExcludingThis(mallocSizeOf);

<<<<<<< HEAD
  rtSizes->gc.nurseryCommitted += gc.nursery().sizeOfHeapCommitted();
  rtSizes->gc.nurseryMallocedBuffers +=
      gc.nursery().sizeOfMallocedBuffers(mallocSizeOf);
  gc.storeBuffer().addSizeOfExcludingThis(mallocSizeOf, &rtSizes->gc);
||||||| merged common ancestors
    rtSizes->gc.nurseryCommitted += gc.nursery().sizeOfHeapCommitted();
    rtSizes->gc.nurseryMallocedBuffers += gc.nursery().sizeOfMallocedBuffers(mallocSizeOf);
    gc.storeBuffer().addSizeOfExcludingThis(mallocSizeOf, &rtSizes->gc);
=======
  rtSizes->gc.nurseryCommitted += gc.nursery().committed();
  rtSizes->gc.nurseryMallocedBuffers +=
      gc.nursery().sizeOfMallocedBuffers(mallocSizeOf);
  gc.storeBuffer().addSizeOfExcludingThis(mallocSizeOf, &rtSizes->gc);
>>>>>>> upstream-releases

  if (sharedImmutableStrings_) {
    rtSizes->sharedImmutableStringsCache +=
        sharedImmutableStrings_->sizeOfExcludingThis(mallocSizeOf);
  }

  rtSizes->sharedIntlData +=
      sharedIntlData.ref().sizeOfExcludingThis(mallocSizeOf);

  {
    AutoLockScriptData lock(this);
    rtSizes->scriptData +=
        scriptDataTable(lock).shallowSizeOfExcludingThis(mallocSizeOf);
    for (ScriptDataTable::Range r = scriptDataTable(lock).all(); !r.empty();
         r.popFront()) {
      rtSizes->scriptData += mallocSizeOf(r.front());
    }
  }

  if (jitRuntime_) {
    jitRuntime_->execAlloc().addSizeOfCode(&rtSizes->code);

    // Sizes of the IonBuilders we are holding for lazy linking
    for (auto builder : jitRuntime_->ionLazyLinkList(this)) {
      rtSizes->jitLazyLink += builder->sizeOfExcludingThis(mallocSizeOf);
    }
  }

  rtSizes->wasmRuntime +=
      wasmInstances.lock()->sizeOfExcludingThis(mallocSizeOf);
}

static bool HandleInterrupt(JSContext* cx, bool invokeCallback) {
  MOZ_ASSERT(!cx->zone()->isAtomsZone());

  // Interrupts can occur at different points between recording and replay,
  // so no recorded behaviors should occur while handling an interrupt.
  // Additionally, returning false here will change subsequent behavior, so
  // such an event cannot occur during recording or replay without
  // invalidating the recording.
  mozilla::recordreplay::AutoDisallowThreadEvents d;

  cx->runtime()->gc.gcIfRequested();

<<<<<<< HEAD
  // A worker thread may have requested an interrupt after finishing an Ion
  // compilation.
  jit::AttachFinishedCompilations(cx);
||||||| merged common ancestors
    // Don't call the interrupt callback if we only interrupted for GC or Ion.
    if (!invokeCallback) {
        return true;
    }

    // Important: Additional callbacks can occur inside the callback handler
    // if it re-enters the JS engine. The embedding must ensure that the
    // callback is disconnected before attempting such re-entry.
    if (cx->interruptCallbackDisabled) {
        return true;
    }
=======
  // A worker thread may have requested an interrupt after finishing an Ion
  // compilation.
  jit::AttachFinishedCompilations(cx);

  // Don't call the interrupt callback if we only interrupted for GC or Ion.
  if (!invokeCallback) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Don't call the interrupt callback if we only interrupted for GC or Ion.
  if (!invokeCallback) {
    return true;
  }
||||||| merged common ancestors
    bool stop = false;
    for (JSInterruptCallback cb : cx->interruptCallbacks()) {
        if (!cb(cx)) {
            stop = true;
        }
    }

    if (!stop) {
        // Debugger treats invoking the interrupt callback as a "step", so
        // invoke the onStep handler.
        if (cx->realm()->isDebuggee()) {
            ScriptFrameIter iter(cx);
            if (!iter.done() &&
                cx->compartment() == iter.compartment() &&
                iter.script()->stepModeEnabled())
            {
                RootedValue rval(cx);
                switch (Debugger::onSingleStep(cx, &rval)) {
                  case ResumeMode::Terminate:
                    mozilla::recordreplay::InvalidateRecording("Debugger single-step produced an error");
                    return false;
                  case ResumeMode::Continue:
                    return true;
                  case ResumeMode::Return:
                    // See note in Debugger::propagateForcedReturn.
                    Debugger::propagateForcedReturn(cx, iter.abstractFramePtr(), rval);
                    mozilla::recordreplay::InvalidateRecording("Debugger single-step forced return");
                    return false;
                  case ResumeMode::Throw:
                    cx->setPendingException(rval);
                    mozilla::recordreplay::InvalidateRecording("Debugger single-step threw an exception");
                    return false;
                  default:;
                }
            }
        }

        return true;
    }

    // No need to set aside any pending exception here: ComputeStackString
    // already does that.
    JSString* stack = ComputeStackString(cx);
    JSFlatString* flat = stack ? stack->ensureFlat(cx) : nullptr;

    const char16_t* chars;
    AutoStableStringChars stableChars(cx);
    if (flat && stableChars.initTwoByte(cx, flat)) {
        chars = stableChars.twoByteRange().begin().get();
    } else {
        chars = u"(stack not available)";
    }
    JS_ReportErrorFlagsAndNumberUC(cx, JSREPORT_WARNING, GetErrorMessage, nullptr,
                                   JSMSG_TERMINATED, chars);

    mozilla::recordreplay::InvalidateRecording("Interrupt callback forced return");
    return false;
}

void
JSContext::requestInterrupt(InterruptReason reason)
{
    interruptBits_ |= uint32_t(reason);
    jitStackLimit = UINTPTR_MAX;
=======
  // Important: Additional callbacks can occur inside the callback handler
  // if it re-enters the JS engine. The embedding must ensure that the
  // callback is disconnected before attempting such re-entry.
  if (cx->interruptCallbackDisabled) {
    return true;
  }

  bool stop = false;
  for (JSInterruptCallback cb : cx->interruptCallbacks()) {
    if (!cb(cx)) {
      stop = true;
    }
  }

  if (!stop) {
    // Debugger treats invoking the interrupt callback as a "step", so
    // invoke the onStep handler.
    if (cx->realm()->isDebuggee()) {
      ScriptFrameIter iter(cx);
      if (!iter.done() && cx->compartment() == iter.compartment() &&
          iter.script()->stepModeEnabled()) {
        RootedValue rval(cx);
        switch (Debugger::onSingleStep(cx, &rval)) {
          case ResumeMode::Terminate:
            mozilla::recordreplay::InvalidateRecording(
                "Debugger single-step produced an error");
            return false;
          case ResumeMode::Continue:
            return true;
          case ResumeMode::Return:
            // See note in Debugger::propagateForcedReturn.
            Debugger::propagateForcedReturn(cx, iter.abstractFramePtr(), rval);
            mozilla::recordreplay::InvalidateRecording(
                "Debugger single-step forced return");
            return false;
          case ResumeMode::Throw:
            cx->setPendingExceptionAndCaptureStack(rval);
            mozilla::recordreplay::InvalidateRecording(
                "Debugger single-step threw an exception");
            return false;
          default:;
        }
      }
    }

    return true;
  }

  // No need to set aside any pending exception here: ComputeStackString
  // already does that.
  JSString* stack = ComputeStackString(cx);
  JSFlatString* flat = stack ? stack->ensureFlat(cx) : nullptr;

  const char16_t* chars;
  AutoStableStringChars stableChars(cx);
  if (flat && stableChars.initTwoByte(cx, flat)) {
    chars = stableChars.twoByteRange().begin().get();
  } else {
    chars = u"(stack not available)";
  }
  JS_ReportErrorFlagsAndNumberUC(cx, JSREPORT_WARNING, GetErrorMessage, nullptr,
                                 JSMSG_TERMINATED, chars);

  mozilla::recordreplay::InvalidateRecording(
      "Interrupt callback forced return");
  return false;
}

void JSContext::requestInterrupt(InterruptReason reason) {
  interruptBits_ |= uint32_t(reason);
  jitStackLimit = UINTPTR_MAX;

  if (reason == InterruptReason::CallbackUrgent) {
    // If this interrupt is urgent (slow script dialog for instance), take
    // additional steps to interrupt corner cases where the above fields are
    // not regularly polled.
    FutexThread::lock();
    if (fx.isWaiting()) {
      fx.notify(FutexThread::NotifyForJSInterrupt);
    }
    fx.unlock();
    wasm::InterruptRunningCode(this);
  }
}

bool JSContext::handleInterrupt() {
  MOZ_ASSERT(CurrentThreadCanAccessRuntime(runtime()));
  if (hasAnyPendingInterrupt() || jitStackLimit == UINTPTR_MAX) {
    bool invokeCallback =
        hasPendingInterrupt(InterruptReason::CallbackUrgent) ||
        hasPendingInterrupt(InterruptReason::CallbackCanWait);
    interruptBits_ = 0;
    resetJitStackLimit();
    return HandleInterrupt(this, invokeCallback);
  }
  return true;
}

bool JSRuntime::setDefaultLocale(const char* locale) {
  if (!locale) {
    return false;
  }

  UniqueChars newLocale = DuplicateString(mainContextFromOwnThread(), locale);
  if (!newLocale) {
    return false;
  }

  defaultLocale.ref() = std::move(newLocale);
  return true;
}

void JSRuntime::resetDefaultLocale() { defaultLocale = nullptr; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Important: Additional callbacks can occur inside the callback handler
  // if it re-enters the JS engine. The embedding must ensure that the
  // callback is disconnected before attempting such re-entry.
  if (cx->interruptCallbackDisabled) {
    return true;
  }

  bool stop = false;
  for (JSInterruptCallback cb : cx->interruptCallbacks()) {
    if (!cb(cx)) {
      stop = true;
    }
  }

  if (!stop) {
    // Debugger treats invoking the interrupt callback as a "step", so
    // invoke the onStep handler.
    if (cx->realm()->isDebuggee()) {
      ScriptFrameIter iter(cx);
      if (!iter.done() && cx->compartment() == iter.compartment() &&
          iter.script()->stepModeEnabled()) {
        RootedValue rval(cx);
        switch (Debugger::onSingleStep(cx, &rval)) {
          case ResumeMode::Terminate:
            mozilla::recordreplay::InvalidateRecording(
                "Debugger single-step produced an error");
            return false;
          case ResumeMode::Continue:
            return true;
          case ResumeMode::Return:
            // See note in Debugger::propagateForcedReturn.
            Debugger::propagateForcedReturn(cx, iter.abstractFramePtr(), rval);
            mozilla::recordreplay::InvalidateRecording(
                "Debugger single-step forced return");
            return false;
          case ResumeMode::Throw:
            cx->setPendingException(rval);
            mozilla::recordreplay::InvalidateRecording(
                "Debugger single-step threw an exception");
            return false;
          default:;
        }
      }
    }
||||||| merged common ancestors
    if (reason == InterruptReason::CallbackUrgent) {
        // If this interrupt is urgent (slow script dialog for instance), take
        // additional steps to interrupt corner cases where the above fields are
        // not regularly polled.
        FutexThread::lock();
        if (fx.isWaiting()) {
            fx.notify(FutexThread::NotifyForJSInterrupt);
        }
        fx.unlock();
        wasm::InterruptRunningCode(this);
    }
}
=======
const char* JSRuntime::getDefaultLocale() {
  if (defaultLocale.ref()) {
    return defaultLocale.ref().get();
  }

  // Use ICU if available to retrieve the default locale, this ensures ICU's
  // default locale matches our default locale.
#if EXPOSE_INTL_API
  const char* locale = uloc_getDefault();
#else
  const char* locale = setlocale(LC_ALL, nullptr);
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
    return true;
  }

  // No need to set aside any pending exception here: ComputeStackString
  // already does that.
  JSString* stack = ComputeStackString(cx);
  JSFlatString* flat = stack ? stack->ensureFlat(cx) : nullptr;

  const char16_t* chars;
  AutoStableStringChars stableChars(cx);
  if (flat && stableChars.initTwoByte(cx, flat)) {
    chars = stableChars.twoByteRange().begin().get();
  } else {
    chars = u"(stack not available)";
  }
  JS_ReportErrorFlagsAndNumberUC(cx, JSREPORT_WARNING, GetErrorMessage, nullptr,
                                 JSMSG_TERMINATED, chars);

  mozilla::recordreplay::InvalidateRecording(
      "Interrupt callback forced return");
  return false;
}

void JSContext::requestInterrupt(InterruptReason reason) {
  interruptBits_ |= uint32_t(reason);
  jitStackLimit = UINTPTR_MAX;

  if (reason == InterruptReason::CallbackUrgent) {
    // If this interrupt is urgent (slow script dialog for instance), take
    // additional steps to interrupt corner cases where the above fields are
    // not regularly polled.
    FutexThread::lock();
    if (fx.isWaiting()) {
      fx.notify(FutexThread::NotifyForJSInterrupt);
    }
    fx.unlock();
    wasm::InterruptRunningCode(this);
  }
}

bool JSContext::handleInterrupt() {
  MOZ_ASSERT(CurrentThreadCanAccessRuntime(runtime()));
  if (hasAnyPendingInterrupt() || jitStackLimit == UINTPTR_MAX) {
    bool invokeCallback =
        hasPendingInterrupt(InterruptReason::CallbackUrgent) ||
        hasPendingInterrupt(InterruptReason::CallbackCanWait);
    interruptBits_ = 0;
    resetJitStackLimit();
    return HandleInterrupt(this, invokeCallback);
  }
  return true;
}

bool JSRuntime::setDefaultLocale(const char* locale) {
  if (!locale) {
    return false;
  }
||||||| merged common ancestors
bool
JSContext::handleInterrupt()
{
    MOZ_ASSERT(CurrentThreadCanAccessRuntime(runtime()));
    if (hasAnyPendingInterrupt() || jitStackLimit == UINTPTR_MAX) {
        bool invokeCallback =
            hasPendingInterrupt(InterruptReason::CallbackUrgent) ||
            hasPendingInterrupt(InterruptReason::CallbackCanWait);
        interruptBits_ = 0;
        resetJitStackLimit();
        return HandleInterrupt(this, invokeCallback);
    }
    return true;
}

bool
JSRuntime::setDefaultLocale(const char* locale)
{
    if (!locale) {
        return false;
    }
=======
  // convert to a well-formed BCP 47 language tag
  if (!locale || !strcmp(locale, "C")) {
    locale = "und";
  }

  UniqueChars lang = DuplicateString(mainContextFromOwnThread(), locale);
  if (!lang) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  UniqueChars newLocale = DuplicateString(mainContextFromOwnThread(), locale);
  if (!newLocale) {
    return false;
  }
||||||| merged common ancestors
    UniqueChars newLocale = DuplicateString(mainContextFromOwnThread(), locale);
    if (!newLocale) {
        return false;
    }
=======
  char* p;
  if ((p = strchr(lang.get(), '.'))) {
    *p = '\0';
  }
  while ((p = strchr(lang.get(), '_'))) {
    *p = '-';
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  defaultLocale.ref() = std::move(newLocale);
  return true;
||||||| merged common ancestors
    defaultLocale.ref() = std::move(newLocale);
    return true;
=======
  defaultLocale.ref() = std::move(lang);
  return defaultLocale.ref().get();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void JSRuntime::resetDefaultLocale() { defaultLocale = nullptr; }
||||||| merged common ancestors
void
JSRuntime::resetDefaultLocale()
{
    defaultLocale = nullptr;
}
=======
void JSRuntime::traceSharedIntlData(JSTracer* trc) {
  sharedIntlData.ref().trace(trc);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
const char* JSRuntime::getDefaultLocale() {
  if (defaultLocale.ref()) {
    return defaultLocale.ref().get();
  }
||||||| merged common ancestors
const char*
JSRuntime::getDefaultLocale()
{
    if (defaultLocale.ref()) {
        return defaultLocale.ref().get();
    }
=======
FreeOp::FreeOp(JSRuntime* maybeRuntime, bool isDefault)
    : JSFreeOp(maybeRuntime), isDefault(isDefault) {
  MOZ_ASSERT_IF(maybeRuntime, CurrentThreadCanAccessRuntime(maybeRuntime));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  const char* locale = setlocale(LC_ALL, nullptr);
||||||| merged common ancestors
    const char* locale = setlocale(LC_ALL, nullptr);
=======
FreeOp::~FreeOp() {
  for (size_t i = 0; i < freeLaterList.length(); i++) {
    free_(freeLaterList[i]);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // convert to a well-formed BCP 47 language tag
  if (!locale || !strcmp(locale, "C")) {
    locale = "und";
  }
||||||| merged common ancestors
    // convert to a well-formed BCP 47 language tag
    if (!locale || !strcmp(locale, "C")) {
        locale = "und";
    }
=======
  if (!jitPoisonRanges.empty()) {
    jit::ExecutableAllocator::poisonCode(runtime(), jitPoisonRanges);
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  UniqueChars lang = DuplicateString(mainContextFromOwnThread(), locale);
  if (!lang) {
    return nullptr;
  }
||||||| merged common ancestors
    UniqueChars lang = DuplicateString(mainContextFromOwnThread(), locale);
    if (!lang) {
        return nullptr;
    }
=======
GlobalObject* JSRuntime::getIncumbentGlobal(JSContext* cx) {
  MOZ_ASSERT(cx->jobQueue);
>>>>>>> upstream-releases

<<<<<<< HEAD
  char* p;
  if ((p = strchr(lang.get(), '.'))) {
    *p = '\0';
  }
  while ((p = strchr(lang.get(), '_'))) {
    *p = '-';
  }
||||||| merged common ancestors
    char* p;
    if ((p = strchr(lang.get(), '.'))) {
        *p = '\0';
    }
    while ((p = strchr(lang.get(), '_'))) {
        *p = '-';
    }
=======
  JSObject* obj = cx->jobQueue->getIncumbentGlobal(cx);
  if (!obj) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  defaultLocale.ref() = std::move(lang);
  return defaultLocale.ref().get();
||||||| merged common ancestors
    defaultLocale.ref() = std::move(lang);
    return defaultLocale.ref().get();
=======
  MOZ_ASSERT(obj->is<GlobalObject>(),
             "getIncumbentGlobalCallback must return a global!");
  return &obj->as<GlobalObject>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void JSRuntime::traceSharedIntlData(JSTracer* trc) {
  sharedIntlData.ref().trace(trc);
}
||||||| merged common ancestors
void
JSRuntime::traceSharedIntlData(JSTracer* trc)
{
    sharedIntlData.ref().trace(trc);
}
=======
bool JSRuntime::enqueuePromiseJob(JSContext* cx, HandleFunction job,
                                  HandleObject promise,
                                  Handle<GlobalObject*> incumbentGlobal) {
  MOZ_ASSERT(cx->jobQueue,
             "Must select a JobQueue implementation using JS::JobQueue "
             "or js::UseInternalJobQueues before using Promises");
>>>>>>> upstream-releases

<<<<<<< HEAD
FreeOp::FreeOp(JSRuntime* maybeRuntime) : JSFreeOp(maybeRuntime) {
  MOZ_ASSERT_IF(maybeRuntime, CurrentThreadCanAccessRuntime(maybeRuntime));
}
||||||| merged common ancestors
FreeOp::FreeOp(JSRuntime* maybeRuntime)
  : JSFreeOp(maybeRuntime)
{
    MOZ_ASSERT_IF(maybeRuntime, CurrentThreadCanAccessRuntime(maybeRuntime));
}
=======
  RootedObject allocationSite(cx);
  if (promise) {
#ifdef DEBUG
    AssertSameCompartment(job, promise);
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
FreeOp::~FreeOp() {
  for (size_t i = 0; i < freeLaterList.length(); i++) {
    free_(freeLaterList[i]);
  }

  if (!jitPoisonRanges.empty()) {
    jit::ExecutableAllocator::poisonCode(runtime(), jitPoisonRanges);
  }
||||||| merged common ancestors
FreeOp::~FreeOp()
{
    for (size_t i = 0; i < freeLaterList.length(); i++) {
        free_(freeLaterList[i]);
    }

    if (!jitPoisonRanges.empty()) {
        jit::ExecutableAllocator::poisonCode(runtime(), jitPoisonRanges);
    }
=======
    RootedObject unwrappedPromise(cx, promise);
    // While the job object is guaranteed to be unwrapped, the promise
    // might be wrapped. See the comments in EnqueuePromiseReactionJob in
    // builtin/Promise.cpp for details.
    if (IsWrapper(promise)) {
      unwrappedPromise = UncheckedUnwrap(promise);
    }
    if (unwrappedPromise->is<PromiseObject>()) {
      allocationSite = JS::GetPromiseAllocationSite(unwrappedPromise);
    }
  }
  return cx->jobQueue->enqueuePromiseJob(cx, promise, job, allocationSite,
                                         incumbentGlobal);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool FreeOp::isDefaultFreeOp() const {
  return runtime_ && runtime_->defaultFreeOp() == this;
||||||| merged common ancestors
bool
FreeOp::isDefaultFreeOp() const
{
    return runtime_ && runtime_->defaultFreeOp() == this;
=======
void JSRuntime::addUnhandledRejectedPromise(JSContext* cx,
                                            js::HandleObject promise) {
  MOZ_ASSERT(promise->is<PromiseObject>());
  if (!cx->promiseRejectionTrackerCallback) {
    return;
  }

  bool mutedErrors = false;
  if (JSScript* script = cx->currentScript()) {
    mutedErrors = script->mutedErrors();
  }

  void* data = cx->promiseRejectionTrackerCallbackData;
  cx->promiseRejectionTrackerCallback(
      cx, mutedErrors, promise, JS::PromiseRejectionHandlingState::Unhandled,
      data);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
GlobalObject* JSRuntime::getIncumbentGlobal(JSContext* cx) {
  // If the embedding didn't set a callback for getting the incumbent
  // global, the currently active global is used.
  if (!cx->getIncumbentGlobalCallback) {
    if (!cx->compartment()) {
      return nullptr;
    }
    return cx->global();
  }
||||||| merged common ancestors
GlobalObject*
JSRuntime::getIncumbentGlobal(JSContext* cx)
{
    // If the embedding didn't set a callback for getting the incumbent
    // global, the currently active global is used.
    if (!cx->getIncumbentGlobalCallback) {
        if (!cx->compartment()) {
            return nullptr;
        }
        return cx->global();
    }
=======
void JSRuntime::removeUnhandledRejectedPromise(JSContext* cx,
                                               js::HandleObject promise) {
  MOZ_ASSERT(promise->is<PromiseObject>());
  if (!cx->promiseRejectionTrackerCallback) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (JSObject* obj = cx->getIncumbentGlobalCallback(cx)) {
    MOZ_ASSERT(obj->is<GlobalObject>(),
               "getIncumbentGlobalCallback must return a global!");
    return &obj->as<GlobalObject>();
  }
||||||| merged common ancestors
    if (JSObject* obj = cx->getIncumbentGlobalCallback(cx)) {
        MOZ_ASSERT(obj->is<GlobalObject>(),
                   "getIncumbentGlobalCallback must return a global!");
        return &obj->as<GlobalObject>();
    }
=======
  bool mutedErrors = false;
  if (JSScript* script = cx->currentScript()) {
    mutedErrors = script->mutedErrors();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return nullptr;
||||||| merged common ancestors
    return nullptr;
=======
  void* data = cx->promiseRejectionTrackerCallbackData;
  cx->promiseRejectionTrackerCallback(
      cx, mutedErrors, promise, JS::PromiseRejectionHandlingState::Handled,
      data);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool JSRuntime::enqueuePromiseJob(JSContext* cx, HandleFunction job,
                                  HandleObject promise,
                                  Handle<GlobalObject*> incumbentGlobal) {
  MOZ_ASSERT(cx->enqueuePromiseJobCallback,
             "Must set a callback using JS::SetEnqueuePromiseJobCallback "
             "before using Promises");
||||||| merged common ancestors
bool
JSRuntime::enqueuePromiseJob(JSContext* cx, HandleFunction job, HandleObject promise,
                             Handle<GlobalObject*> incumbentGlobal)
{
    MOZ_ASSERT(cx->enqueuePromiseJobCallback,
               "Must set a callback using JS::SetEnqueuePromiseJobCallback before using Promises");
=======
mozilla::non_crypto::XorShift128PlusRNG& JSRuntime::randomKeyGenerator() {
  MOZ_ASSERT(CurrentThreadCanAccessRuntime(this));
  if (randomKeyGenerator_.isNothing()) {
    mozilla::Array<uint64_t, 2> seed;
    GenerateXorShift128PlusSeed(seed);
    randomKeyGenerator_.emplace(seed[0], seed[1]);
  }
  return randomKeyGenerator_.ref();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  void* data = cx->enqueuePromiseJobCallbackData;
  RootedObject allocationSite(cx);
  if (promise) {
#ifdef DEBUG
    AssertSameCompartment(job, promise);
#endif
||||||| merged common ancestors
    void* data = cx->enqueuePromiseJobCallbackData;
    RootedObject allocationSite(cx);
    if (promise) {
#ifdef DEBUG
        AssertSameCompartment(job, promise);
#endif
=======
mozilla::HashCodeScrambler JSRuntime::randomHashCodeScrambler() {
  auto& rng = randomKeyGenerator();
  return mozilla::HashCodeScrambler(rng.next(), rng.next());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    RootedObject unwrappedPromise(cx, promise);
    // While the job object is guaranteed to be unwrapped, the promise
    // might be wrapped. See the comments in EnqueuePromiseReactionJob in
    // builtin/Promise.cpp for details.
    if (IsWrapper(promise)) {
      unwrappedPromise = UncheckedUnwrap(promise);
    }
    if (unwrappedPromise->is<PromiseObject>()) {
      allocationSite = JS::GetPromiseAllocationSite(unwrappedPromise);
    }
  }
  return cx->enqueuePromiseJobCallback(cx, promise, job, allocationSite,
                                       incumbentGlobal, data);
||||||| merged common ancestors
        RootedObject unwrappedPromise(cx, promise);
        // While the job object is guaranteed to be unwrapped, the promise
        // might be wrapped. See the comments in EnqueuePromiseReactionJob in
        // builtin/Promise.cpp for details.
        if (IsWrapper(promise)) {
            unwrappedPromise = UncheckedUnwrap(promise);
        }
        if (unwrappedPromise->is<PromiseObject>()) {
            allocationSite = JS::GetPromiseAllocationSite(unwrappedPromise);
        }
    }
    return cx->enqueuePromiseJobCallback(cx, promise, job, allocationSite,
                                         incumbentGlobal, data);
=======
mozilla::non_crypto::XorShift128PlusRNG JSRuntime::forkRandomKeyGenerator() {
  auto& rng = randomKeyGenerator();
  return mozilla::non_crypto::XorShift128PlusRNG(rng.next(), rng.next());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void JSRuntime::addUnhandledRejectedPromise(JSContext* cx,
                                            js::HandleObject promise) {
  MOZ_ASSERT(promise->is<PromiseObject>());
  if (!cx->promiseRejectionTrackerCallback) {
    return;
  }
||||||| merged common ancestors
void
JSRuntime::addUnhandledRejectedPromise(JSContext* cx, js::HandleObject promise)
{
    MOZ_ASSERT(promise->is<PromiseObject>());
    if (!cx->promiseRejectionTrackerCallback) {
        return;
    }
=======
js::HashNumber JSRuntime::randomHashCode() {
  MOZ_ASSERT(CurrentThreadCanAccessRuntime(this));
>>>>>>> upstream-releases

<<<<<<< HEAD
  void* data = cx->promiseRejectionTrackerCallbackData;
  cx->promiseRejectionTrackerCallback(
      cx, promise, JS::PromiseRejectionHandlingState::Unhandled, data);
||||||| merged common ancestors
    void* data = cx->promiseRejectionTrackerCallbackData;
    cx->promiseRejectionTrackerCallback(cx, promise,
                                        JS::PromiseRejectionHandlingState::Unhandled, data);
=======
  if (randomHashCodeGenerator_.isNothing()) {
    mozilla::Array<uint64_t, 2> seed;
    GenerateXorShift128PlusSeed(seed);
    randomHashCodeGenerator_.emplace(seed[0], seed[1]);
  }

  return HashNumber(randomHashCodeGenerator_->next());
}

void JSRuntime::updateMallocCounter(size_t nbytes) {
  gc.updateMallocCounter(nbytes);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void JSRuntime::removeUnhandledRejectedPromise(JSContext* cx,
                                               js::HandleObject promise) {
  MOZ_ASSERT(promise->is<PromiseObject>());
  if (!cx->promiseRejectionTrackerCallback) {
    return;
  }
||||||| merged common ancestors
void
JSRuntime::removeUnhandledRejectedPromise(JSContext* cx, js::HandleObject promise)
{
    MOZ_ASSERT(promise->is<PromiseObject>());
    if (!cx->promiseRejectionTrackerCallback) {
        return;
    }
=======
JS_FRIEND_API void* JSRuntime::onOutOfMemory(AllocFunction allocFunc,
                                             arena_id_t arena, size_t nbytes,
                                             void* reallocPtr,
                                             JSContext* maybecx) {
  MOZ_ASSERT_IF(allocFunc != AllocFunction::Realloc, !reallocPtr);

  if (JS::RuntimeHeapIsBusy()) {
    return nullptr;
  }

  if (!oom::IsSimulatedOOMAllocation()) {
    /*
     * Retry when we are done with the background sweeping and have stopped
     * all the allocations and released the empty GC chunks.
     */
    gc.onOutOfMallocMemory();
    void* p;
    switch (allocFunc) {
      case AllocFunction::Malloc:
        p = js_arena_malloc(arena, nbytes);
        break;
      case AllocFunction::Calloc:
        p = js_arena_calloc(arena, nbytes, 1);
        break;
      case AllocFunction::Realloc:
        p = js_arena_realloc(arena, reallocPtr, nbytes);
        break;
      default:
        MOZ_CRASH();
    }
    if (p) {
      return p;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void* data = cx->promiseRejectionTrackerCallbackData;
  cx->promiseRejectionTrackerCallback(
      cx, promise, JS::PromiseRejectionHandlingState::Handled, data);
||||||| merged common ancestors
    void* data = cx->promiseRejectionTrackerCallbackData;
    cx->promiseRejectionTrackerCallback(cx, promise,
                                        JS::PromiseRejectionHandlingState::Handled, data);
=======
  if (maybecx) {
    ReportOutOfMemory(maybecx);
  }
  return nullptr;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
mozilla::non_crypto::XorShift128PlusRNG& JSRuntime::randomKeyGenerator() {
  MOZ_ASSERT(CurrentThreadCanAccessRuntime(this));
  if (randomKeyGenerator_.isNothing()) {
    mozilla::Array<uint64_t, 2> seed;
    GenerateXorShift128PlusSeed(seed);
    randomKeyGenerator_.emplace(seed[0], seed[1]);
  }
  return randomKeyGenerator_.ref();
||||||| merged common ancestors
mozilla::non_crypto::XorShift128PlusRNG&
JSRuntime::randomKeyGenerator()
{
    MOZ_ASSERT(CurrentThreadCanAccessRuntime(this));
    if (randomKeyGenerator_.isNothing()) {
        mozilla::Array<uint64_t, 2> seed;
        GenerateXorShift128PlusSeed(seed);
        randomKeyGenerator_.emplace(seed[0], seed[1]);
    }
    return randomKeyGenerator_.ref();
=======
void* JSRuntime::onOutOfMemoryCanGC(AllocFunction allocFunc, arena_id_t arena,
                                    size_t bytes, void* reallocPtr) {
  if (OnLargeAllocationFailure && bytes >= LARGE_ALLOCATION) {
    OnLargeAllocationFailure();
  }
  return onOutOfMemory(allocFunc, arena, bytes, reallocPtr);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
mozilla::HashCodeScrambler JSRuntime::randomHashCodeScrambler() {
  auto& rng = randomKeyGenerator();
  return mozilla::HashCodeScrambler(rng.next(), rng.next());
||||||| merged common ancestors
mozilla::HashCodeScrambler
JSRuntime::randomHashCodeScrambler()
{
    auto& rng = randomKeyGenerator();
    return mozilla::HashCodeScrambler(rng.next(), rng.next());
=======
bool JSRuntime::activeGCInAtomsZone() {
  Zone* zone = unsafeAtomsZone();
  return (zone->needsIncrementalBarrier() &&
          !gc.isVerifyPreBarriersEnabled()) ||
         zone->wasGCStarted();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
mozilla::non_crypto::XorShift128PlusRNG JSRuntime::forkRandomKeyGenerator() {
  auto& rng = randomKeyGenerator();
  return mozilla::non_crypto::XorShift128PlusRNG(rng.next(), rng.next());
||||||| merged common ancestors
mozilla::non_crypto::XorShift128PlusRNG
JSRuntime::forkRandomKeyGenerator()
{
    auto& rng = randomKeyGenerator();
    return mozilla::non_crypto::XorShift128PlusRNG(rng.next(), rng.next());
=======
void JSRuntime::setUsedByHelperThread(Zone* zone) {
  MOZ_ASSERT(!zone->usedByHelperThread());
  MOZ_ASSERT(!zone->wasGCStarted());
  MOZ_ASSERT(!isOffThreadParsingBlocked());

  zone->setUsedByHelperThread();
  if (numActiveHelperThreadZones++ == 0) {
    gc.setParallelAtomsAllocEnabled(true);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
js::HashNumber JSRuntime::randomHashCode() {
  MOZ_ASSERT(CurrentThreadCanAccessRuntime(this));
||||||| merged common ancestors
js::HashNumber
JSRuntime::randomHashCode()
{
    MOZ_ASSERT(CurrentThreadCanAccessRuntime(this));
=======
void JSRuntime::clearUsedByHelperThread(Zone* zone) {
  MOZ_ASSERT(zone->usedByHelperThread());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (randomHashCodeGenerator_.isNothing()) {
    mozilla::Array<uint64_t, 2> seed;
    GenerateXorShift128PlusSeed(seed);
    randomHashCodeGenerator_.emplace(seed[0], seed[1]);
  }
||||||| merged common ancestors
    if (randomHashCodeGenerator_.isNothing()) {
        mozilla::Array<uint64_t, 2> seed;
        GenerateXorShift128PlusSeed(seed);
        randomHashCodeGenerator_.emplace(seed[0], seed[1]);
    }
=======
  zone->clearUsedByHelperThread();
  if (--numActiveHelperThreadZones == 0) {
    gc.setParallelAtomsAllocEnabled(false);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return HashNumber(randomHashCodeGenerator_->next());
||||||| merged common ancestors
    return HashNumber(randomHashCodeGenerator_->next());
=======
  JSContext* cx = mainContextFromOwnThread();
  if (gc.fullGCForAtomsRequested() && cx->canCollectAtoms()) {
    gc.triggerFullGCForAtoms(cx);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void JSRuntime::updateMallocCounter(size_t nbytes) {
  gc.updateMallocCounter(nbytes);
||||||| merged common ancestors
void
JSRuntime::updateMallocCounter(size_t nbytes)
{
    gc.updateMallocCounter(nbytes);
=======
void JSRuntime::incrementNumDebuggeeRealms() {
  if (numDebuggeeRealms_ == 0) {
    jitRuntime()->baselineInterpreter().toggleDebuggerInstrumentation(true);
  }

  numDebuggeeRealms_++;
  MOZ_ASSERT(numDebuggeeRealms_ <= numRealms);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_FRIEND_API void* JSRuntime::onOutOfMemory(AllocFunction allocFunc,
                                             size_t nbytes, void* reallocPtr,
                                             JSContext* maybecx) {
  MOZ_ASSERT_IF(allocFunc != AllocFunction::Realloc, !reallocPtr);
||||||| merged common ancestors
JS_FRIEND_API(void*)
JSRuntime::onOutOfMemory(AllocFunction allocFunc, size_t nbytes, void* reallocPtr, JSContext* maybecx)
{
    MOZ_ASSERT_IF(allocFunc != AllocFunction::Realloc, !reallocPtr);
=======
void JSRuntime::decrementNumDebuggeeRealms() {
  MOZ_ASSERT(numDebuggeeRealms_ > 0);
  numDebuggeeRealms_--;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (JS::RuntimeHeapIsBusy()) {
    return nullptr;
  }
||||||| merged common ancestors
    if (JS::RuntimeHeapIsBusy()) {
        return nullptr;
    }
=======
  if (numDebuggeeRealms_ == 0) {
    jitRuntime()->baselineInterpreter().toggleDebuggerInstrumentation(false);
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!oom::IsSimulatedOOMAllocation()) {
    /*
     * Retry when we are done with the background sweeping and have stopped
     * all the allocations and released the empty GC chunks.
     */
    gc.onOutOfMallocMemory();
    void* p;
    switch (allocFunc) {
      case AllocFunction::Malloc:
        p = js_malloc(nbytes);
        break;
      case AllocFunction::Calloc:
        p = js_calloc(nbytes);
        break;
      case AllocFunction::Realloc:
        p = js_realloc(reallocPtr, nbytes);
        break;
      default:
        MOZ_CRASH();
    }
    if (p) {
      return p;
    }
  }

  if (maybecx) {
    ReportOutOfMemory(maybecx);
  }
  return nullptr;
||||||| merged common ancestors
    if (!oom::IsSimulatedOOMAllocation()) {
        /*
         * Retry when we are done with the background sweeping and have stopped
         * all the allocations and released the empty GC chunks.
         */
        gc.onOutOfMallocMemory();
        void* p;
        switch (allocFunc) {
          case AllocFunction::Malloc:
            p = js_malloc(nbytes);
            break;
          case AllocFunction::Calloc:
            p = js_calloc(nbytes);
            break;
          case AllocFunction::Realloc:
            p = js_realloc(reallocPtr, nbytes);
            break;
          default:
            MOZ_CRASH();
        }
        if (p) {
            return p;
        }
    }

    if (maybecx) {
        ReportOutOfMemory(maybecx);
    }
    return nullptr;
=======
void JSRuntime::incrementNumDebuggeeRealmsObservingCoverage() {
  if (numDebuggeeRealmsObservingCoverage_ == 0) {
    jit::BaselineInterpreter& interp = jitRuntime()->baselineInterpreter();
    interp.toggleCodeCoverageInstrumentation(true);
  }

  numDebuggeeRealmsObservingCoverage_++;
  MOZ_ASSERT(numDebuggeeRealmsObservingCoverage_ <= numRealms);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void* JSRuntime::onOutOfMemoryCanGC(AllocFunction allocFunc, size_t bytes,
                                    void* reallocPtr) {
  if (OnLargeAllocationFailure && bytes >= LARGE_ALLOCATION) {
    OnLargeAllocationFailure();
  }
  return onOutOfMemory(allocFunc, bytes, reallocPtr);
||||||| merged common ancestors
void*
JSRuntime::onOutOfMemoryCanGC(AllocFunction allocFunc, size_t bytes, void* reallocPtr)
{
    if (OnLargeAllocationFailure && bytes >= LARGE_ALLOCATION) {
        OnLargeAllocationFailure();
    }
    return onOutOfMemory(allocFunc, bytes, reallocPtr);
=======
void JSRuntime::decrementNumDebuggeeRealmsObservingCoverage() {
  MOZ_ASSERT(numDebuggeeRealmsObservingCoverage_ > 0);
  numDebuggeeRealmsObservingCoverage_--;

  if (numDebuggeeRealmsObservingCoverage_ == 0) {
    jit::BaselineInterpreter& interp = jitRuntime()->baselineInterpreter();
    interp.toggleCodeCoverageInstrumentation(false);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool JSRuntime::activeGCInAtomsZone() {
  Zone* zone = unsafeAtomsZone();
  return (zone->needsIncrementalBarrier() &&
          !gc.isVerifyPreBarriersEnabled()) ||
         zone->wasGCStarted();
||||||| merged common ancestors
bool
JSRuntime::activeGCInAtomsZone()
{
    Zone* zone = unsafeAtomsZone();
    return (zone->needsIncrementalBarrier() && !gc.isVerifyPreBarriersEnabled()) ||
           zone->wasGCStarted();
=======
bool js::CurrentThreadCanAccessRuntime(const JSRuntime* rt) {
  return rt->mainContextFromAnyThread() == TlsContext.get();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void JSRuntime::setUsedByHelperThread(Zone* zone) {
  MOZ_ASSERT(!zone->usedByHelperThread());
  MOZ_ASSERT(!zone->wasGCStarted());
  MOZ_ASSERT(!isOffThreadParsingBlocked());
||||||| merged common ancestors
void
JSRuntime::setUsedByHelperThread(Zone* zone)
{
    MOZ_ASSERT(!zone->usedByHelperThread());
    MOZ_ASSERT(!zone->wasGCStarted());
    MOZ_ASSERT(!isOffThreadParsingBlocked());
=======
bool js::CurrentThreadCanAccessZone(Zone* zone) {
  // Helper thread zones can only be used by their owning thread.
  if (zone->usedByHelperThread()) {
    return zone->ownedByCurrentHelperThread();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  zone->setUsedByHelperThread();
  if (numActiveHelperThreadZones++ == 0) {
    gc.setParallelAtomsAllocEnabled(true);
  }
||||||| merged common ancestors
    zone->setUsedByHelperThread();
    if (numActiveHelperThreadZones++ == 0) {
        gc.setParallelAtomsAllocEnabled(true);
    }
=======
  // Other zones can only be accessed by the runtime's active context.
  return CurrentThreadCanAccessRuntime(zone->runtime_);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void JSRuntime::clearUsedByHelperThread(Zone* zone) {
  MOZ_ASSERT(zone->usedByHelperThread());

  zone->clearUsedByHelperThread();
  if (--numActiveHelperThreadZones == 0) {
    gc.setParallelAtomsAllocEnabled(false);
  }
||||||| merged common ancestors
void
JSRuntime::clearUsedByHelperThread(Zone* zone)
{
    MOZ_ASSERT(zone->usedByHelperThread());

    zone->clearUsedByHelperThread();
    if (--numActiveHelperThreadZones == 0) {
        gc.setParallelAtomsAllocEnabled(false);
    }
=======
#ifdef DEBUG
bool js::CurrentThreadIsPerformingGC() {
  return TlsContext.get()->performingGC;
}
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSContext* cx = mainContextFromOwnThread();
  if (gc.fullGCForAtomsRequested() && cx->canCollectAtoms()) {
    gc.triggerFullGCForAtoms(cx);
  }
||||||| merged common ancestors
    JSContext* cx = mainContextFromOwnThread();
    if (gc.fullGCForAtomsRequested() && cx->canCollectAtoms()) {
        gc.triggerFullGCForAtoms(cx);
    }
=======
JS_FRIEND_API void JS::SetJSContextProfilerSampleBufferRangeStart(
    JSContext* cx, uint64_t rangeStart) {
  cx->runtime()->setProfilerSampleBufferRangeStart(rangeStart);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool js::CurrentThreadCanAccessRuntime(const JSRuntime* rt) {
  return rt->mainContextFromAnyThread() == TlsContext.get();
||||||| merged common ancestors
bool
js::CurrentThreadCanAccessRuntime(const JSRuntime* rt)
{
    return rt->mainContextFromAnyThread() == TlsContext.get();
=======
JS_FRIEND_API bool JS::IsProfilingEnabledForContext(JSContext* cx) {
  MOZ_ASSERT(cx);
  return cx->runtime()->geckoProfiler().enabled();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool js::CurrentThreadCanAccessZone(Zone* zone) {
  // Helper thread zones can only be used by their owning thread.
  if (zone->usedByHelperThread()) {
    return zone->ownedByCurrentHelperThread();
  }
||||||| merged common ancestors
bool
js::CurrentThreadCanAccessZone(Zone* zone)
{
    // Helper thread zones can only be used by their owning thread.
    if (zone->usedByHelperThread()) {
        return zone->ownedByCurrentHelperThread();
    }
=======
JS_FRIEND_API void JS::EnableRecordingAllocations(
    JSContext* cx, JS::RecordAllocationsCallback callback, double probability) {
  MOZ_ASSERT(cx);
  MOZ_ASSERT(cx->isMainThreadContext());
  cx->runtime()->startRecordingAllocations(probability, callback);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Other zones can only be accessed by the runtime's active context.
  return CurrentThreadCanAccessRuntime(zone->runtime_);
||||||| merged common ancestors
    // Other zones can only be accessed by the runtime's active context.
    return CurrentThreadCanAccessRuntime(zone->runtime_);
=======
JS_FRIEND_API void JS::DisableRecordingAllocations(JSContext* cx) {
  MOZ_ASSERT(cx);
  MOZ_ASSERT(cx->isMainThreadContext());
  cx->runtime()->stopRecordingAllocations();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
#ifdef DEBUG
bool js::CurrentThreadIsPerformingGC() {
  return TlsContext.get()->performingGC;
||||||| merged common ancestors
#ifdef DEBUG
bool
js::CurrentThreadIsPerformingGC()
{
    return TlsContext.get()->performingGC;
=======
JS_PUBLIC_API void JS::shadow::RegisterWeakCache(
    JSRuntime* rt, detail::WeakCacheBase* cachep) {
  rt->registerWeakCache(cachep);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_FRIEND_API void JS::SetJSContextProfilerSampleBufferRangeStart(
    JSContext* cx, uint64_t rangeStart) {
  cx->runtime()->setProfilerSampleBufferRangeStart(rangeStart);
||||||| merged common ancestors
JS_FRIEND_API(void)
JS::SetJSContextProfilerSampleBufferRangeStart(JSContext* cx, uint64_t rangeStart)
{
    cx->runtime()->setProfilerSampleBufferRangeStart(rangeStart);
=======
void JSRuntime::startRecordingAllocations(
    double probability, JS::RecordAllocationsCallback callback) {
  allocationSamplingProbability = probability;
  recordAllocationCallback = callback;

  // Go through all of the existing realms, and turn on allocation tracking.
  for (RealmsIter realm(this); !realm.done(); realm.next()) {
    realm->setAllocationMetadataBuilder(&SavedStacks::metadataBuilder);
    realm->chooseAllocationSamplingProbability();
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_FRIEND_API bool JS::IsProfilingEnabledForContext(JSContext* cx) {
  MOZ_ASSERT(cx);
  return cx->runtime()->geckoProfiler().enabled();
||||||| merged common ancestors
JS_FRIEND_API(bool)
JS::IsProfilingEnabledForContext(JSContext* cx)
{
    MOZ_ASSERT(cx);
    return cx->runtime()->geckoProfiler().enabled();
=======
void JSRuntime::stopRecordingAllocations() {
  recordAllocationCallback = nullptr;
  // Go through all of the existing realms, and turn on allocation tracking.
  for (RealmsIter realm(this); !realm.done(); realm.next()) {
    js::GlobalObject* global = realm->maybeGlobal();
    if (!realm->isDebuggee() || !global ||
        !Debugger::isObservedByDebuggerTrackingAllocations(*global)) {
      // Only remove the allocation metadata builder if no Debuggers are
      // tracking allocations.
      realm->forgetAllocationMetadataBuilder();
    }
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_PUBLIC_API void JS::shadow::RegisterWeakCache(
    JSRuntime* rt, detail::WeakCacheBase* cachep) {
  rt->registerWeakCache(cachep);
||||||| merged common ancestors
JS_PUBLIC_API(void)
JS::shadow::RegisterWeakCache(JSRuntime* rt, detail::WeakCacheBase* cachep)
{
    rt->registerWeakCache(cachep);
=======
// This function can run to ensure that when new realms are created
// they have allocation logging turned on.
void JSRuntime::ensureRealmIsRecordingAllocations(
    Handle<GlobalObject*> global) {
  if (recordAllocationCallback) {
    if (!global->realm()->isRecordingAllocations()) {
      // This is a new realm, turn on allocations for it.
      global->realm()->setAllocationMetadataBuilder(
          &SavedStacks::metadataBuilder);
    }
    // Ensure the probability is up to date with the current combination of
    // debuggers and runtime profiling.
    global->realm()->chooseAllocationSamplingProbability();
  }
>>>>>>> upstream-releases
}
