/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_CompileWrappers_h
#define jit_CompileWrappers_h

#include "vm/JSContext.h"

namespace js {
namespace jit {

class JitRuntime;

// During Ion compilation we need access to various bits of the current
// compartment, runtime and so forth. However, since compilation can run off
// thread while the main thread is mutating the VM, this access needs
// to be restricted. The classes below give the compiler an interface to access
// all necessary information in a threadsafe fashion.

class CompileRuntime {
  JSRuntime* runtime();

 public:
  static CompileRuntime* get(JSRuntime* rt);

#ifdef JS_GC_ZEAL
  const uint32_t* addressOfGCZealModeBits();
#endif

  const JitRuntime* jitRuntime();

  // Compilation does not occur off thread when the Gecko Profiler is enabled.
  GeckoProfilerRuntime& geckoProfiler();

  bool jitSupportsFloatingPoint();
  bool hadOutOfMemory();
  bool profilingScripts();

<<<<<<< HEAD
  const JSAtomState& names();
  const PropertyName* emptyString();
  const StaticStrings& staticStrings();
  const Value& NaNValue();
  const Value& positiveInfinityValue();
  const WellKnownSymbols& wellKnownSymbols();
||||||| merged common ancestors
    const JSAtomState& names();
    const PropertyName* emptyString();
    const StaticStrings& staticStrings();
    const Value& NaNValue();
    const Value& positiveInfinityValue();
    const WellKnownSymbols& wellKnownSymbols();
=======
  const JSAtomState& names();
  const PropertyName* emptyString();
  const StaticStrings& staticStrings();
  const WellKnownSymbols& wellKnownSymbols();
>>>>>>> upstream-releases

<<<<<<< HEAD
  const void* mainContextPtr();
  uint32_t* addressOfTenuredAllocCount();
  const void* addressOfJitStackLimit();
  const void* addressOfInterruptBits();
||||||| merged common ancestors
    const void* mainContextPtr();
    uint32_t* addressOfTenuredAllocCount();
    const void* addressOfJitStackLimit();
    const void* addressOfInterruptBits();
=======
  const void* mainContextPtr();
  uint32_t* addressOfTenuredAllocCount();
  const void* addressOfJitStackLimit();
  const void* addressOfInterruptBits();
  const void* addressOfZone();
>>>>>>> upstream-releases

#ifdef DEBUG
  bool isInsideNursery(gc::Cell* cell);
#endif

  // DOM callbacks must be threadsafe (and will hopefully be removed soon).
  const DOMCallbacks* DOMcallbacks();

  bool runtimeMatches(JSRuntime* rt);
};

class CompileZone {
  Zone* zone();

 public:
  static CompileZone* get(Zone* zone);

  CompileRuntime* runtime();
  bool isAtomsZone();

#ifdef DEBUG
  const void* addressOfIonBailAfter();
#endif

  const uint32_t* addressOfNeedsIncrementalBarrier();
  gc::FreeSpan** addressOfFreeList(gc::AllocKind allocKind);
  void* addressOfNurseryPosition();
  void* addressOfStringNurseryPosition();
  const void* addressOfNurseryCurrentEnd();
  const void* addressOfStringNurseryCurrentEnd();

  uint32_t* addressOfNurseryAllocCount();

  bool nurseryExists();
  bool canNurseryAllocateStrings();
  void setMinorGCShouldCancelIonCompilations();
};

class JitRealm;

class CompileRealm {
  JS::Realm* realm();

 public:
  static CompileRealm* get(JS::Realm* realm);

  CompileZone* zone();
  CompileRuntime* runtime();

  const void* realmPtr() { return realm(); }

  const mozilla::non_crypto::XorShift128PlusRNG*
  addressOfRandomNumberGenerator();

  const JitRealm* jitRealm();

  const GlobalObject* maybeGlobal();
  const uint32_t* addressOfGlobalWriteBarriered();

  bool hasAllocationMetadataBuilder();

  // Mirror RealmOptions.
  void setSingletonsAsValues();
};

class JitCompileOptions {
 public:
  JitCompileOptions();
  explicit JitCompileOptions(JSContext* cx);

  bool cloneSingletons() const { return cloneSingletons_; }

  bool profilerSlowAssertionsEnabled() const {
    return profilerSlowAssertionsEnabled_;
  }

  bool offThreadCompilationAvailable() const {
    return offThreadCompilationAvailable_;
  }

<<<<<<< HEAD
#ifdef ENABLE_WASM_GC
  bool wasmGcEnabled() const { return wasmGcEnabled_; }
#endif

 private:
  bool cloneSingletons_;
  bool profilerSlowAssertionsEnabled_;
  bool offThreadCompilationAvailable_;
#ifdef ENABLE_WASM_GC
  bool wasmGcEnabled_;
#endif
||||||| merged common ancestors
#ifdef ENABLE_WASM_GC
    bool wasmGcEnabled() const {
        return wasmGcEnabled_;
    }
#endif

  private:
    bool cloneSingletons_;
    bool profilerSlowAssertionsEnabled_;
    bool offThreadCompilationAvailable_;
#ifdef ENABLE_WASM_GC
    bool wasmGcEnabled_;
#endif
=======
 private:
  bool cloneSingletons_;
  bool profilerSlowAssertionsEnabled_;
  bool offThreadCompilationAvailable_;
>>>>>>> upstream-releases
};

}  // namespace jit
}  // namespace js

#endif  // jit_CompileWrappers_h
