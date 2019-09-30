/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "jit/CompileWrappers.h"

#include "gc/GC.h"
#include "jit/Ion.h"
#include "jit/JitRealm.h"

#include "vm/Realm-inl.h"

using namespace js;
using namespace js::jit;

JSRuntime* CompileRuntime::runtime() {
  return reinterpret_cast<JSRuntime*>(this);
}

<<<<<<< HEAD
/* static */ CompileRuntime* CompileRuntime::get(JSRuntime* rt) {
  return reinterpret_cast<CompileRuntime*>(rt);
||||||| merged common ancestors
/* static */ CompileRuntime*
CompileRuntime::get(JSRuntime* rt)
{
    return reinterpret_cast<CompileRuntime*>(rt);
=======
/* static */
CompileRuntime* CompileRuntime::get(JSRuntime* rt) {
  return reinterpret_cast<CompileRuntime*>(rt);
>>>>>>> upstream-releases
}

#ifdef JS_GC_ZEAL
const uint32_t* CompileRuntime::addressOfGCZealModeBits() {
  return runtime()->gc.addressOfZealModeBits();
}
#endif

const JitRuntime* CompileRuntime::jitRuntime() {
  return runtime()->jitRuntime();
}

GeckoProfilerRuntime& CompileRuntime::geckoProfiler() {
  return runtime()->geckoProfiler();
}

bool CompileRuntime::jitSupportsFloatingPoint() {
  return runtime()->jitSupportsFloatingPoint;
}

<<<<<<< HEAD
bool CompileRuntime::hadOutOfMemory() { return runtime()->hadOutOfMemory; }

bool CompileRuntime::profilingScripts() { return runtime()->profilingScripts; }
||||||| merged common ancestors
bool
CompileRuntime::hadOutOfMemory()
{
    return runtime()->hadOutOfMemory;
}

bool
CompileRuntime::profilingScripts()
{
    return runtime()->profilingScripts;
}
=======
bool CompileRuntime::hadOutOfMemory() { return runtime()->hadOutOfMemory; }
>>>>>>> upstream-releases

<<<<<<< HEAD
const JSAtomState& CompileRuntime::names() { return *runtime()->commonNames; }
||||||| merged common ancestors
const JSAtomState&
CompileRuntime::names()
{
    return *runtime()->commonNames;
}
=======
bool CompileRuntime::profilingScripts() { return runtime()->profilingScripts; }
>>>>>>> upstream-releases

<<<<<<< HEAD
const PropertyName* CompileRuntime::emptyString() {
  return runtime()->emptyString;
}
||||||| merged common ancestors
const PropertyName*
CompileRuntime::emptyString()
{
    return runtime()->emptyString;
}
=======
const JSAtomState& CompileRuntime::names() { return *runtime()->commonNames; }
>>>>>>> upstream-releases

<<<<<<< HEAD
const StaticStrings& CompileRuntime::staticStrings() {
  return *runtime()->staticStrings;
||||||| merged common ancestors
const StaticStrings&
CompileRuntime::staticStrings()
{
    return *runtime()->staticStrings;
=======
const PropertyName* CompileRuntime::emptyString() {
  return runtime()->emptyString;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
const Value& CompileRuntime::NaNValue() { return runtime()->NaNValue; }
||||||| merged common ancestors
const Value&
CompileRuntime::NaNValue()
{
    return runtime()->NaNValue;
}
=======
const StaticStrings& CompileRuntime::staticStrings() {
  return *runtime()->staticStrings;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
const Value& CompileRuntime::positiveInfinityValue() {
  return runtime()->positiveInfinityValue;
||||||| merged common ancestors
const Value&
CompileRuntime::positiveInfinityValue()
{
    return runtime()->positiveInfinityValue;
=======
const WellKnownSymbols& CompileRuntime::wellKnownSymbols() {
  return *runtime()->wellKnownSymbols;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
const WellKnownSymbols& CompileRuntime::wellKnownSymbols() {
  return *runtime()->wellKnownSymbols;
||||||| merged common ancestors
const WellKnownSymbols&
CompileRuntime::wellKnownSymbols()
{
    return *runtime()->wellKnownSymbols;
=======
const void* CompileRuntime::mainContextPtr() {
  return runtime()->mainContextFromAnyThread();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
const void* CompileRuntime::mainContextPtr() {
  return runtime()->mainContextFromAnyThread();
||||||| merged common ancestors
const void*
CompileRuntime::mainContextPtr()
{
    return runtime()->mainContextFromAnyThread();
=======
uint32_t* CompileRuntime::addressOfTenuredAllocCount() {
  return runtime()->mainContextFromAnyThread()->addressOfTenuredAllocCount();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
uint32_t* CompileRuntime::addressOfTenuredAllocCount() {
  return runtime()->mainContextFromAnyThread()->addressOfTenuredAllocCount();
||||||| merged common ancestors
uint32_t*
CompileRuntime::addressOfTenuredAllocCount()
{
    return runtime()->mainContextFromAnyThread()->addressOfTenuredAllocCount();
=======
const void* CompileRuntime::addressOfJitStackLimit() {
  return runtime()->mainContextFromAnyThread()->addressOfJitStackLimit();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
const void* CompileRuntime::addressOfJitStackLimit() {
  return runtime()->mainContextFromAnyThread()->addressOfJitStackLimit();
||||||| merged common ancestors
const void*
CompileRuntime::addressOfJitStackLimit()
{
    return runtime()->mainContextFromAnyThread()->addressOfJitStackLimit();
=======
const void* CompileRuntime::addressOfInterruptBits() {
  return runtime()->mainContextFromAnyThread()->addressOfInterruptBits();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
const void* CompileRuntime::addressOfInterruptBits() {
  return runtime()->mainContextFromAnyThread()->addressOfInterruptBits();
||||||| merged common ancestors
const void*
CompileRuntime::addressOfInterruptBits()
{
    return runtime()->mainContextFromAnyThread()->addressOfInterruptBits();
=======
const void* CompileRuntime::addressOfZone() {
  return runtime()->mainContextFromAnyThread()->addressOfZone();
>>>>>>> upstream-releases
}

#ifdef DEBUG
bool CompileRuntime::isInsideNursery(gc::Cell* cell) {
  return UninlinedIsInsideNursery(cell);
}
#endif

const DOMCallbacks* CompileRuntime::DOMcallbacks() {
  return runtime()->DOMcallbacks;
}

bool CompileRuntime::runtimeMatches(JSRuntime* rt) { return rt == runtime(); }

Zone* CompileZone::zone() { return reinterpret_cast<Zone*>(this); }

<<<<<<< HEAD
/* static */ CompileZone* CompileZone::get(Zone* zone) {
  return reinterpret_cast<CompileZone*>(zone);
||||||| merged common ancestors
/* static */ CompileZone*
CompileZone::get(Zone* zone)
{
    return reinterpret_cast<CompileZone*>(zone);
=======
/* static */
CompileZone* CompileZone::get(Zone* zone) {
  return reinterpret_cast<CompileZone*>(zone);
>>>>>>> upstream-releases
}

CompileRuntime* CompileZone::runtime() {
  return CompileRuntime::get(zone()->runtimeFromAnyThread());
}

bool CompileZone::isAtomsZone() { return zone()->isAtomsZone(); }

#ifdef DEBUG
const void* CompileZone::addressOfIonBailAfter() {
  return zone()->runtimeFromAnyThread()->jitRuntime()->addressOfIonBailAfter();
}
#endif

const uint32_t* CompileZone::addressOfNeedsIncrementalBarrier() {
  return zone()->addressOfNeedsIncrementalBarrier();
}

gc::FreeSpan** CompileZone::addressOfFreeList(gc::AllocKind allocKind) {
  return zone()->arenas.addressOfFreeList(allocKind);
}

void* CompileZone::addressOfNurseryPosition() {
  return zone()->runtimeFromAnyThread()->gc.addressOfNurseryPosition();
}

void* CompileZone::addressOfStringNurseryPosition() {
  // Objects and strings share a nursery, for now at least.
  return zone()->runtimeFromAnyThread()->gc.addressOfNurseryPosition();
}

const void* CompileZone::addressOfNurseryCurrentEnd() {
  return zone()->runtimeFromAnyThread()->gc.addressOfNurseryCurrentEnd();
}

const void* CompileZone::addressOfStringNurseryCurrentEnd() {
  // Although objects and strings share a nursery (and this may change)
  // there is still a separate string end address.  The only time it
  // is different from the regular end address, is when nursery strings are
  // disabled (it will be NULL).
  //
  // This function returns _a pointer to_ that end address.
  return zone()->runtimeFromAnyThread()->gc.addressOfStringNurseryCurrentEnd();
}

uint32_t* CompileZone::addressOfNurseryAllocCount() {
  return zone()->runtimeFromAnyThread()->gc.addressOfNurseryAllocCount();
}

bool CompileZone::canNurseryAllocateStrings() {
  return nurseryExists() &&
         zone()->runtimeFromAnyThread()->gc.nursery().canAllocateStrings() &&
         zone()->allocNurseryStrings;
}

bool CompileZone::nurseryExists() {
  return zone()->runtimeFromAnyThread()->gc.nursery().exists();
}

void CompileZone::setMinorGCShouldCancelIonCompilations() {
  MOZ_ASSERT(CurrentThreadCanAccessZone(zone()));
  JSRuntime* rt = zone()->runtimeFromMainThread();
  rt->gc.storeBuffer().setShouldCancelIonCompilations();
}

JS::Realm* CompileRealm::realm() { return reinterpret_cast<JS::Realm*>(this); }

<<<<<<< HEAD
/* static */ CompileRealm* CompileRealm::get(JS::Realm* realm) {
  return reinterpret_cast<CompileRealm*>(realm);
||||||| merged common ancestors
/* static */ CompileRealm*
CompileRealm::get(JS::Realm* realm)
{
    return reinterpret_cast<CompileRealm*>(realm);
=======
/* static */
CompileRealm* CompileRealm::get(JS::Realm* realm) {
  return reinterpret_cast<CompileRealm*>(realm);
>>>>>>> upstream-releases
}

CompileZone* CompileRealm::zone() { return CompileZone::get(realm()->zone()); }

CompileRuntime* CompileRealm::runtime() {
  return CompileRuntime::get(realm()->runtimeFromAnyThread());
}

const mozilla::non_crypto::XorShift128PlusRNG*
CompileRealm::addressOfRandomNumberGenerator() {
  return realm()->addressOfRandomNumberGenerator();
}

const JitRealm* CompileRealm::jitRealm() { return realm()->jitRealm(); }

const GlobalObject* CompileRealm::maybeGlobal() {
  // This uses unsafeUnbarrieredMaybeGlobal() so as not to trigger the read
  // barrier on the global from off thread.  This is safe because we
  // abort Ion compilation when we GC.
  return realm()->unsafeUnbarrieredMaybeGlobal();
}

const uint32_t* CompileRealm::addressOfGlobalWriteBarriered() {
  return &realm()->globalWriteBarriered;
}

bool CompileRealm::hasAllocationMetadataBuilder() {
  return realm()->hasAllocationMetadataBuilder();
}

// Note: This function is thread-safe because setSingletonAsValue sets a boolean
// variable to false, and this boolean variable has no way to be resetted to
// true. So even if there is a concurrent write, this concurrent write will
// always have the same value.  If there is a concurrent read, then we will
// clone a singleton instead of using the value which is baked in the JSScript,
// and this would be an unfortunate allocation, but this will not change the
// semantics of the JavaScript code which is executed.
void CompileRealm::setSingletonsAsValues() {
  realm()->behaviors().setSingletonsAsValues();
}

JitCompileOptions::JitCompileOptions()
<<<<<<< HEAD
    : cloneSingletons_(false),
      profilerSlowAssertionsEnabled_(false),
      offThreadCompilationAvailable_(false)
#ifdef ENABLE_WASM_GC
      ,
      wasmGcEnabled_(false)
#endif
{
}

JitCompileOptions::JitCompileOptions(JSContext* cx) {
  cloneSingletons_ = cx->realm()->creationOptions().cloneSingletons();
  profilerSlowAssertionsEnabled_ =
      cx->runtime()->geckoProfiler().enabled() &&
      cx->runtime()->geckoProfiler().slowAssertionsEnabled();
  offThreadCompilationAvailable_ = OffThreadCompilationAvailable(cx);
#ifdef ENABLE_WASM_GC
  wasmGcEnabled_ = cx->options().wasmGc();
#endif
||||||| merged common ancestors
  : cloneSingletons_(false),
    profilerSlowAssertionsEnabled_(false),
    offThreadCompilationAvailable_(false)
#ifdef ENABLE_WASM_GC
    , wasmGcEnabled_(false)
#endif
{
}

JitCompileOptions::JitCompileOptions(JSContext* cx)
{
    cloneSingletons_ = cx->realm()->creationOptions().cloneSingletons();
    profilerSlowAssertionsEnabled_ = cx->runtime()->geckoProfiler().enabled() &&
                                     cx->runtime()->geckoProfiler().slowAssertionsEnabled();
    offThreadCompilationAvailable_ = OffThreadCompilationAvailable(cx);
#ifdef ENABLE_WASM_GC
    wasmGcEnabled_ = cx->options().wasmGc();
#endif
=======
    : cloneSingletons_(false),
      profilerSlowAssertionsEnabled_(false),
      offThreadCompilationAvailable_(false) {}

JitCompileOptions::JitCompileOptions(JSContext* cx) {
  cloneSingletons_ = cx->realm()->creationOptions().cloneSingletons();
  profilerSlowAssertionsEnabled_ =
      cx->runtime()->geckoProfiler().enabled() &&
      cx->runtime()->geckoProfiler().slowAssertionsEnabled();
  offThreadCompilationAvailable_ = OffThreadCompilationAvailable(cx);
>>>>>>> upstream-releases
}
