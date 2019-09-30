/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* Various JS utility functions. */

#include "jsutil.h"

#include "mozilla/Assertions.h"
#include "mozilla/Atomics.h"
#include "mozilla/Maybe.h"
#include "mozilla/ThreadLocal.h"

#include <stdio.h>

#include "jstypes.h"

#include "js/Utility.h"
#include "util/Windows.h"
#include "vm/HelperThreads.h"

using namespace js;

using mozilla::Maybe;

#if defined(DEBUG) || defined(JS_OOM_BREAKPOINT)
/* For OOM testing functionality in Utility.h. */
namespace js {

mozilla::Atomic<AutoEnterOOMUnsafeRegion*> AutoEnterOOMUnsafeRegion::owner_;

namespace oom {

<<<<<<< HEAD
JS_PUBLIC_DATA FailureSimulator simulator;
static MOZ_THREAD_LOCAL(uint32_t) threadType;
||||||| merged common ancestors
JS_PUBLIC_DATA(uint32_t) targetThread = 0;
MOZ_THREAD_LOCAL(uint32_t) threadType;
JS_PUBLIC_DATA(uint64_t) maxAllocations = UINT64_MAX;
JS_PUBLIC_DATA(uint64_t) counter = 0;
JS_PUBLIC_DATA(bool) failAlways = true;
MOZ_THREAD_LOCAL(bool) isAllocationThread;

JS_PUBLIC_DATA(uint32_t) stackTargetThread = 0;
JS_PUBLIC_DATA(uint64_t) maxStackChecks = UINT64_MAX;
JS_PUBLIC_DATA(uint64_t) stackCheckCounter = 0;
JS_PUBLIC_DATA(bool) stackCheckFailAlways = true;
MOZ_THREAD_LOCAL(bool) isStackCheckThread;

JS_PUBLIC_DATA(uint32_t) interruptTargetThread = 0;
JS_PUBLIC_DATA(uint64_t) maxInterruptChecks = UINT64_MAX;
JS_PUBLIC_DATA(uint64_t) interruptCheckCounter = 0;
JS_PUBLIC_DATA(bool) interruptCheckFailAlways = true;
MOZ_THREAD_LOCAL(bool) isInterruptCheckThread;

bool
InitThreadType(void) {
    return threadType.init() && isAllocationThread.init() &&
        isStackCheckThread.init() && isInterruptCheckThread.init();
}

void
SetThreadType(ThreadType type) {
    threadType.set(type);
    isAllocationThread.set(false);
    isStackCheckThread.set(false);
    isInterruptCheckThread.set(false);
}

uint32_t
GetThreadType(void) {
    return threadType.get();
}

uint32_t
GetAllocationThreadType(void) {
    if (isAllocationThread.get()) {
        return js::THREAD_TYPE_CURRENT;
    }
    return threadType.get();
}

uint32_t
GetStackCheckThreadType(void) {
    if (isStackCheckThread.get()) {
        return js::THREAD_TYPE_CURRENT;
    }
    return threadType.get();
}
=======
JS_PUBLIC_DATA FailureSimulator simulator;
static MOZ_THREAD_LOCAL(uint32_t) threadType;

bool InitThreadType() { return threadType.init(); }

void SetThreadType(ThreadType type) { threadType.set(type); }

uint32_t GetThreadType(void) { return threadType.get(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool InitThreadType() { return threadType.init(); }
||||||| merged common ancestors
uint32_t
GetInterruptCheckThreadType(void) {
    if (isInterruptCheckThread.get()) {
        return js::THREAD_TYPE_CURRENT;
    }
    return threadType.get();
}
=======
static inline bool IsHelperThreadType(uint32_t thread) {
  return thread != THREAD_TYPE_NONE && thread != THREAD_TYPE_MAIN;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
void SetThreadType(ThreadType type) { threadType.set(type); }
||||||| merged common ancestors
static inline bool
IsHelperThreadType(uint32_t thread)
{
    return thread != THREAD_TYPE_NONE && thread != THREAD_TYPE_MAIN &&
        thread != THREAD_TYPE_CURRENT;
}
=======
void FailureSimulator::simulateFailureAfter(Kind kind, uint64_t checks,
                                            uint32_t thread, bool always) {
  Maybe<AutoLockHelperThreadState> lock;
  if (IsHelperThreadType(targetThread_) || IsHelperThreadType(thread)) {
    lock.emplace();
    HelperThreadState().waitForAllThreadsLocked(lock.ref());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
uint32_t GetThreadType(void) { return threadType.get(); }
||||||| merged common ancestors
void
SimulateOOMAfter(uint64_t allocations, uint32_t thread, bool always)
{
    Maybe<AutoLockHelperThreadState> lock;
    if (IsHelperThreadType(targetThread) || IsHelperThreadType(thread)) {
        lock.emplace();
        HelperThreadState().waitForAllThreadsLocked(lock.ref());
    }

    MOZ_ASSERT(counter + allocations > counter);
    MOZ_ASSERT(thread > js::THREAD_TYPE_NONE && thread < js::THREAD_TYPE_MAX);
    targetThread = thread;
    if (thread == js::THREAD_TYPE_CURRENT) {
        isAllocationThread.set(true);
    }
    maxAllocations = counter + allocations;
    failAlways = always;
}
=======
  MOZ_ASSERT(counter_ + checks > counter_);
  MOZ_ASSERT(thread > js::THREAD_TYPE_NONE && thread < js::THREAD_TYPE_MAX);
  targetThread_ = thread;
  maxChecks_ = counter_ + checks;
  failAlways_ = always;
  kind_ = kind;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
static inline bool IsHelperThreadType(uint32_t thread) {
  return thread != THREAD_TYPE_NONE && thread != THREAD_TYPE_MAIN;
}
||||||| merged common ancestors
void
ResetSimulatedOOM()
{
    Maybe<AutoLockHelperThreadState> lock;
    if (IsHelperThreadType(targetThread)) {
        lock.emplace();
        HelperThreadState().waitForAllThreadsLocked(lock.ref());
    }

    targetThread = THREAD_TYPE_NONE;
    isAllocationThread.set(false);
    maxAllocations = UINT64_MAX;
    failAlways = false;
}
=======
void FailureSimulator::reset() {
  Maybe<AutoLockHelperThreadState> lock;
  if (IsHelperThreadType(targetThread_)) {
    lock.emplace();
    HelperThreadState().waitForAllThreadsLocked(lock.ref());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void FailureSimulator::simulateFailureAfter(Kind kind, uint64_t checks,
                                            uint32_t thread, bool always) {
  Maybe<AutoLockHelperThreadState> lock;
  if (IsHelperThreadType(targetThread_) || IsHelperThreadType(thread)) {
    lock.emplace();
    HelperThreadState().waitForAllThreadsLocked(lock.ref());
  }
||||||| merged common ancestors
void
SimulateStackOOMAfter(uint64_t checks, uint32_t thread, bool always)
{
    Maybe<AutoLockHelperThreadState> lock;
    if (IsHelperThreadType(stackTargetThread) || IsHelperThreadType(thread)) {
        lock.emplace();
        HelperThreadState().waitForAllThreadsLocked(lock.ref());
    }

    MOZ_ASSERT(stackCheckCounter + checks > stackCheckCounter);
    MOZ_ASSERT(thread > js::THREAD_TYPE_NONE && thread < js::THREAD_TYPE_MAX);
    stackTargetThread = thread;
    if (thread == js::THREAD_TYPE_CURRENT) {
        isStackCheckThread.set(true);
    }
    maxStackChecks = stackCheckCounter + checks;
    stackCheckFailAlways = always;
}
=======
  targetThread_ = THREAD_TYPE_NONE;
  maxChecks_ = UINT64_MAX;
  failAlways_ = false;
  kind_ = Kind::Nothing;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(counter_ + checks > counter_);
  MOZ_ASSERT(thread > js::THREAD_TYPE_NONE && thread < js::THREAD_TYPE_MAX);
  targetThread_ = thread;
  maxChecks_ = counter_ + checks;
  failAlways_ = always;
  kind_ = kind;
}
||||||| merged common ancestors
void
ResetSimulatedStackOOM()
{
    Maybe<AutoLockHelperThreadState> lock;
    if (IsHelperThreadType(stackTargetThread)) {
        lock.emplace();
        HelperThreadState().waitForAllThreadsLocked(lock.ref());
    }

    stackTargetThread = THREAD_TYPE_NONE;
    isStackCheckThread.set(false);
    maxStackChecks = UINT64_MAX;
    stackCheckFailAlways = false;
}
=======
}  // namespace oom
}  // namespace js
#endif  // defined(DEBUG) || defined(JS_OOM_BREAKPOINT)
>>>>>>> upstream-releases

<<<<<<< HEAD
void FailureSimulator::reset() {
  Maybe<AutoLockHelperThreadState> lock;
  if (IsHelperThreadType(targetThread_)) {
    lock.emplace();
    HelperThreadState().waitForAllThreadsLocked(lock.ref());
  }
||||||| merged common ancestors
void
SimulateInterruptAfter(uint64_t checks, uint32_t thread, bool always)
{
    Maybe<AutoLockHelperThreadState> lock;
    if (IsHelperThreadType(interruptTargetThread) || IsHelperThreadType(thread)) {
        lock.emplace();
        HelperThreadState().waitForAllThreadsLocked(lock.ref());
    }

    MOZ_ASSERT(interruptCheckCounter + checks > interruptCheckCounter);
    MOZ_ASSERT(thread > js::THREAD_TYPE_NONE && thread < js::THREAD_TYPE_MAX);
    interruptTargetThread = thread;
    if (thread == js::THREAD_TYPE_CURRENT) {
        isInterruptCheckThread.set(true);
    }
    maxInterruptChecks = interruptCheckCounter + checks;
    interruptCheckFailAlways = always;
}
=======
#if defined(FUZZING)
namespace js {
namespace oom {
JS_PUBLIC_DATA size_t largeAllocLimit = 0;
void InitLargeAllocLimit() {
  char* limitStr = getenv("MOZ_FUZZ_LARGE_ALLOC_LIMIT");
  if (limitStr) {
    largeAllocLimit = atoll(limitStr);
  }
}
}  // namespace oom
}  // namespace js
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  targetThread_ = THREAD_TYPE_NONE;
  maxChecks_ = UINT64_MAX;
  failAlways_ = false;
  kind_ = Kind::Nothing;
}
||||||| merged common ancestors
void
ResetSimulatedInterrupt()
{
    Maybe<AutoLockHelperThreadState> lock;
    if (IsHelperThreadType(interruptTargetThread)) {
        lock.emplace();
        HelperThreadState().waitForAllThreadsLocked(lock.ref());
    }

    interruptTargetThread = THREAD_TYPE_NONE;
    isInterruptCheckThread.set(false);
    maxInterruptChecks = UINT64_MAX;
    interruptCheckFailAlways = false;
}
=======
bool js::gDisablePoisoning = false;
>>>>>>> upstream-releases

<<<<<<< HEAD
}  // namespace oom
}  // namespace js
#endif  // defined(DEBUG) || defined(JS_OOM_BREAKPOINT)
||||||| merged common ancestors
} // namespace oom
} // namespace js
#endif // defined(DEBUG) || defined(JS_OOM_BREAKPOINT)
=======
JS_PUBLIC_DATA arena_id_t js::MallocArena;
JS_PUBLIC_DATA arena_id_t js::ArrayBufferContentsArena;
JS_PUBLIC_DATA arena_id_t js::StringBufferArena;
>>>>>>> upstream-releases

void js::InitMallocAllocator() {
  MallocArena = moz_create_arena();

<<<<<<< HEAD
JS_PUBLIC_DATA arena_id_t js::MallocArena;
JS_PUBLIC_DATA arena_id_t js::ArrayBufferContentsArena;
||||||| merged common ancestors
JS_PUBLIC_DATA(arena_id_t) js::MallocArena;
JS_PUBLIC_DATA(arena_id_t) js::ArrayBufferContentsArena;
=======
  arena_params_t params;
  params.mFlags |= ARENA_FLAG_RANDOMIZE_SMALL;
  ArrayBufferContentsArena = moz_create_arena_with_params(&params);
  StringBufferArena = moz_create_arena_with_params(&params);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
void js::InitMallocAllocator() {
  MallocArena = moz_create_arena();
  ArrayBufferContentsArena = moz_create_arena();
||||||| merged common ancestors
void
js::InitMallocAllocator()
{
    MallocArena = moz_create_arena();
    ArrayBufferContentsArena = moz_create_arena();
=======
void js::ShutDownMallocAllocator() {
  // Until Bug 1364359 is fixed it is unsafe to call moz_dispose_arena.
  // moz_dispose_arena(MallocArena);
  // moz_dispose_arena(ArrayBufferContentsArena);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void js::ShutDownMallocAllocator() {
  // Until Bug 1364359 is fixed it is unsafe to call moz_dispose_arena.
  // moz_dispose_arena(MallocArena);
  // moz_dispose_arena(ArrayBufferContentsArena);
||||||| merged common ancestors
void
js::ShutDownMallocAllocator()
{
    // Until Bug 1364359 is fixed it is unsafe to call moz_dispose_arena.
    // moz_dispose_arena(MallocArena);
    // moz_dispose_arena(ArrayBufferContentsArena);
=======
#ifdef MOZ_DEBUG
extern void js::AssertJSStringBufferInCorrectArena(const void* ptr) {
//  `jemalloc_ptr_info()` only exists if MOZ_MEMORY is defined, and it only
//  returns an arenaId if MOZ_DEBUG is defined. Otherwise, this function is
//  a no-op.
#  if defined(MOZ_MEMORY) && defined(MOZ_DEBUG)
  if (ptr) {
    jemalloc_ptr_info_t ptrInfo{};
    jemalloc_ptr_info(ptr, &ptrInfo);
    MOZ_ASSERT(ptrInfo.tag != TagUnknown);
    MOZ_ASSERT(ptrInfo.arenaId == js::StringBufferArena);
  }
#  endif
>>>>>>> upstream-releases
}
#endif

JS_PUBLIC_API void JS_Assert(const char* s, const char* file, int ln) {
  MOZ_ReportAssertionFailure(s, file, ln);
  MOZ_CRASH();
}

#ifdef __linux__

#  include <malloc.h>
#  include <stdlib.h>

namespace js {

// This function calls all the vanilla heap allocation functions.  It is never
// called, and exists purely to help config/check_vanilla_allocations.py.  See
// that script for more details.
extern MOZ_COLD void AllTheNonBasicVanillaNewAllocations() {
  // posix_memalign and aligned_alloc aren't available on all Linux
  // configurations.
  // valloc was deprecated in Android 5.0
  // char* q;
  // posix_memalign((void**)&q, 16, 16);

  intptr_t p = intptr_t(malloc(16)) + intptr_t(calloc(1, 16)) +
               intptr_t(realloc(nullptr, 16)) + intptr_t(new char) +
               intptr_t(new char) + intptr_t(new char) +
               intptr_t(new char[16]) + intptr_t(memalign(16, 16)) +
               // intptr_t(q) +
               // intptr_t(aligned_alloc(16, 16)) +
               // intptr_t(valloc(4096)) +
               intptr_t(strdup("dummy"));

  printf("%u\n", uint32_t(p));  // make sure |p| is not optimized away

  free((int*)p);  // this would crash if ever actually called

  MOZ_CRASH();
}

}  // namespace js

#endif  // __linux__
