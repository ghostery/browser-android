/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* JS script descriptor. */

#ifndef vm_JSScript_h
#define vm_JSScript_h

#include "mozilla/ArrayUtils.h"
#include "mozilla/Atomics.h"
#include "mozilla/Maybe.h"
#include "mozilla/MaybeOneOf.h"
#include "mozilla/MemoryReporting.h"
#include "mozilla/RefPtr.h"
#include "mozilla/Span.h"
#include "mozilla/UniquePtr.h"
#include "mozilla/Utf8.h"
#include "mozilla/Variant.h"

#include <type_traits>  // std::is_same
#include <utility>      // std::move

#include "jstypes.h"

#include "frontend/BinASTRuntimeSupport.h"
#include "frontend/NameAnalysisTypes.h"
#include "gc/Barrier.h"
#include "gc/Rooting.h"
#include "jit/IonCode.h"
#include "js/CompileOptions.h"
#include "js/UbiNode.h"
#include "js/UniquePtr.h"
#include "js/Utility.h"
<<<<<<< HEAD
#include "util/StructuredSpewer.h"
#include "vm/BytecodeIterator.h"
#include "vm/BytecodeLocation.h"
||||||| merged common ancestors
=======
#include "util/StructuredSpewer.h"
#include "vm/BigIntType.h"
#include "vm/BytecodeIterator.h"
#include "vm/BytecodeLocation.h"
>>>>>>> upstream-releases
#include "vm/BytecodeUtil.h"
#include "vm/JSAtom.h"
#include "vm/NativeObject.h"
#include "vm/Scope.h"
#include "vm/Shape.h"
#include "vm/SharedImmutableStringsCache.h"
#include "vm/Time.h"

namespace JS {
struct ScriptSourceInfo;
template <typename UnitT>
class SourceText;
}  // namespace JS

namespace js {

namespace jit {
<<<<<<< HEAD
struct BaselineScript;
class ICScript;
struct IonScriptCounts;
}  // namespace jit
||||||| merged common ancestors
    struct BaselineScript;
    struct IonScriptCounts;
} // namespace jit
=======
class AutoKeepJitScripts;
struct BaselineScript;
struct IonScriptCounts;
class JitScript;
}  // namespace jit
>>>>>>> upstream-releases

#define ION_DISABLED_SCRIPT ((js::jit::IonScript*)0x1)
#define ION_COMPILING_SCRIPT ((js::jit::IonScript*)0x2)
#define ION_PENDING_SCRIPT ((js::jit::IonScript*)0x3)

#define BASELINE_DISABLED_SCRIPT ((js::jit::BaselineScript*)0x1)

class AutoSweepJitScript;
class BreakpointSite;
class Debugger;
class GCParallelTask;
class LazyScript;
class ModuleObject;
class RegExpObject;
class SourceCompressionTask;
class Shape;

namespace frontend {
<<<<<<< HEAD
struct BytecodeEmitter;
class FunctionBox;
class ModuleSharedContext;
}  // namespace frontend
||||||| merged common ancestors
    struct BytecodeEmitter;
    class FunctionBox;
    class ModuleSharedContext;
} // namespace frontend
=======
struct BytecodeEmitter;
class FunctionBox;
class ModuleSharedContext;
}  // namespace frontend

namespace gc {
void SweepLazyScripts(GCParallelTask* task);
}  // namespace gc
>>>>>>> upstream-releases

namespace detail {

// Do not call this directly! It is exposed for the friend declarations in
// this file.
<<<<<<< HEAD
bool CopyScript(JSContext* cx, HandleScript src, HandleScript dst,
                MutableHandle<GCVector<Scope*>> scopes);
||||||| merged common ancestors
bool
CopyScript(JSContext* cx, HandleScript src, HandleScript dst,
           MutableHandle<GCVector<Scope*>> scopes);
=======
JSScript* CopyScript(JSContext* cx, HandleScript src,
                     HandleScriptSourceObject sourceObject,
                     MutableHandle<GCVector<Scope*>> scopes);
>>>>>>> upstream-releases

}  // namespace detail

}  // namespace js

/*
 * [SMDOC] Try Notes
 *
 * Trynotes are attached to regions that are involved with
 * exception unwinding. They can be broken up into four categories:
 *
 * 1. CATCH and FINALLY: Basic exception handling. A CATCH trynote
 *    covers the range of the associated try. A FINALLY trynote covers
 *    the try and the catch.

 * 2. FOR_IN and DESTRUCTURING: These operations create an iterator
 *    which must be cleaned up (by calling IteratorClose) during
 *    exception unwinding.
 *
 * 3. FOR_OF and FOR_OF_ITERCLOSE: For-of loops handle unwinding using
 *    catch blocks. These trynotes are used for for-of breaks/returns,
 *    which create regions that are lexically within a for-of block,
 *    but logically outside of it. See TryNoteIter::settle for more
 *    details.
 *
 * 4. LOOP: This represents normal for/while/do-while loops. It is
 *    unnecessary for exception unwinding, but storing the boundaries
 *    of loops here is helpful for heuristics that need to know
 *    whether a given op is inside a loop.
 */
enum JSTryNoteKind {
<<<<<<< HEAD
  JSTRY_CATCH,
  JSTRY_FINALLY,
  JSTRY_FOR_IN,
  JSTRY_FOR_OF,
  JSTRY_LOOP,
  JSTRY_FOR_OF_ITERCLOSE,
  JSTRY_DESTRUCTURING_ITERCLOSE
||||||| merged common ancestors
    JSTRY_CATCH,
    JSTRY_FINALLY,
    JSTRY_FOR_IN,
    JSTRY_FOR_OF,
    JSTRY_LOOP,
    JSTRY_FOR_OF_ITERCLOSE,
    JSTRY_DESTRUCTURING_ITERCLOSE
=======
  JSTRY_CATCH,
  JSTRY_FINALLY,
  JSTRY_FOR_IN,
  JSTRY_DESTRUCTURING,
  JSTRY_FOR_OF,
  JSTRY_FOR_OF_ITERCLOSE,
  JSTRY_LOOP
>>>>>>> upstream-releases
};

/*
 * Exception handling record.
 */
struct JSTryNote {
<<<<<<< HEAD
  uint8_t kind;        /* one of JSTryNoteKind */
  uint32_t stackDepth; /* stack depth upon exception handler entry */
  uint32_t start;      /* start of the try statement or loop relative
                          to script->code() */
  uint32_t length;     /* length of the try statement or loop */
||||||| merged common ancestors
    uint8_t         kind;       /* one of JSTryNoteKind */
    uint32_t        stackDepth; /* stack depth upon exception handler entry */
    uint32_t        start;      /* start of the try statement or loop
                                   relative to script->main */
    uint32_t        length;     /* length of the try statement or loop */
=======
  uint32_t kind;       /* one of JSTryNoteKind */
  uint32_t stackDepth; /* stack depth upon exception handler entry */
  uint32_t start;      /* start of the try statement or loop relative
                          to script->code() */
  uint32_t length;     /* length of the try statement or loop */

  template <js::XDRMode mode>
  js::XDRResult XDR(js::XDRState<mode>* xdr);
>>>>>>> upstream-releases
};

namespace js {

// A block scope has a range in bytecode: it is entered at some offset, and left
// at some later offset.  Scopes can be nested.  Given an offset, the
// ScopeNote containing that offset whose with the highest start value
// indicates the block scope.  The block scope list is sorted by increasing
// start value.
//
// It is possible to leave a scope nonlocally, for example via a "break"
// statement, so there may be short bytecode ranges in a block scope in which we
// are popping the block chain in preparation for a goto.  These exits are also
// nested with respect to outer scopes.  The scopes in these exits are indicated
// by the "index" field, just like any other block.  If a nonlocal exit pops the
// last block scope, the index will be NoScopeIndex.
//
struct ScopeNote {
<<<<<<< HEAD
  // Sentinel index for no Scope.
  static const uint32_t NoScopeIndex = UINT32_MAX;

  // Sentinel index for no ScopeNote.
  static const uint32_t NoScopeNoteIndex = UINT32_MAX;

  uint32_t index;   // Index of Scope in the scopes array, or
                    // NoScopeIndex if there is no block scope in
                    // this range.
  uint32_t start;   // Bytecode offset at which this scope starts
                    // relative to script->code().
  uint32_t length;  // Bytecode length of scope.
  uint32_t parent;  // Index of parent block scope in notes, or NoScopeNote.
};

class ScriptCounts {
 public:
  typedef mozilla::Vector<PCCounts, 0, SystemAllocPolicy> PCCountsVector;
||||||| merged common ancestors
    // Sentinel index for no Scope.
    static const uint32_t NoScopeIndex = UINT32_MAX;

    // Sentinel index for no ScopeNote.
    static const uint32_t NoScopeNoteIndex = UINT32_MAX;

    uint32_t        index;      // Index of Scope in the scopes array, or
                                // NoScopeIndex if there is no block scope in
                                // this range.
    uint32_t        start;      // Bytecode offset at which this scope starts,
                                // from script->main().
    uint32_t        length;     // Bytecode length of scope.
    uint32_t        parent;     // Index of parent block scope in notes, or NoScopeNote.
};

struct ConstArray {
    js::GCPtrValue* vector;     // array of indexed constant values
    uint32_t length;
};

struct ObjectArray {
    js::GCPtrObject* vector;    // Array of indexed objects.
    uint32_t length;            // Count of indexed objects.
};

struct ScopeArray {
    js::GCPtrScope* vector;     // Array of indexed scopes.
    uint32_t        length;     // Count of indexed scopes.
};

struct TryNoteArray {
    JSTryNote*      vector;     // Array of indexed try notes.
    uint32_t        length;     // Count of indexed try notes.
};

struct ScopeNoteArray {
    ScopeNote* vector;          // Array of indexed ScopeNote records.
    uint32_t   length;          // Count of indexed try notes.
};

class YieldAndAwaitOffsetArray {
    friend bool
    detail::CopyScript(JSContext* cx, HandleScript src, HandleScript dst,
                       MutableHandle<GCVector<Scope*>> scopes);

    uint32_t*       vector_;    // Array of bytecode offsets.
    uint32_t        length_;    // Count of bytecode offsets.

  public:
    void init(uint32_t* vector, uint32_t length) {
        vector_ = vector;
        length_ = length;
    }
    uint32_t& operator[](uint32_t index) {
        MOZ_ASSERT(index < length_);
        return vector_[index];
    }
    uint32_t length() const {
        return length_;
    }
};
=======
  // Sentinel index for no Scope.
  static const uint32_t NoScopeIndex = UINT32_MAX;

  // Sentinel index for no ScopeNote.
  static const uint32_t NoScopeNoteIndex = UINT32_MAX;

  uint32_t index;   // Index of Scope in the scopes array, or
                    // NoScopeIndex if there is no block scope in
                    // this range.
  uint32_t start;   // Bytecode offset at which this scope starts
                    // relative to script->code().
  uint32_t length;  // Bytecode length of scope.
  uint32_t parent;  // Index of parent block scope in notes, or NoScopeNote.

  template <js::XDRMode mode>
  js::XDRResult XDR(js::XDRState<mode>* xdr);
};
>>>>>>> upstream-releases

<<<<<<< HEAD
  inline ScriptCounts();
  inline explicit ScriptCounts(PCCountsVector&& jumpTargets);
  inline ScriptCounts(ScriptCounts&& src);
  inline ~ScriptCounts();
||||||| merged common ancestors
class ScriptCounts
{
  public:
    typedef mozilla::Vector<PCCounts, 0, SystemAllocPolicy> PCCountsVector;
=======
class ScriptCounts {
 public:
  typedef mozilla::Vector<PCCounts, 0, SystemAllocPolicy> PCCountsVector;
>>>>>>> upstream-releases

<<<<<<< HEAD
  inline ScriptCounts& operator=(ScriptCounts&& src);
||||||| merged common ancestors
    inline ScriptCounts();
    inline explicit ScriptCounts(PCCountsVector&& jumpTargets);
    inline ScriptCounts(ScriptCounts&& src);
    inline ~ScriptCounts();
=======
  inline ScriptCounts();
  inline explicit ScriptCounts(PCCountsVector&& jumpTargets);
  inline ScriptCounts(ScriptCounts&& src);
  inline ~ScriptCounts();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Return the counter used to count the number of visits. Returns null if
  // the element is not found.
  PCCounts* maybeGetPCCounts(size_t offset);
  const PCCounts* maybeGetPCCounts(size_t offset) const;
||||||| merged common ancestors
    inline ScriptCounts& operator=(ScriptCounts&& src);
=======
  inline ScriptCounts& operator=(ScriptCounts&& src);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // PCCounts are stored at jump-target offsets. This function looks for the
  // previous PCCount which is in the same basic block as the current offset.
  PCCounts* getImmediatePrecedingPCCounts(size_t offset);
||||||| merged common ancestors
    // Return the counter used to count the number of visits. Returns null if
    // the element is not found.
    PCCounts* maybeGetPCCounts(size_t offset);
    const PCCounts* maybeGetPCCounts(size_t offset) const;
=======
  // Return the counter used to count the number of visits. Returns null if
  // the element is not found.
  PCCounts* maybeGetPCCounts(size_t offset);
  const PCCounts* maybeGetPCCounts(size_t offset) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Return the counter used to count the number of throws. Returns null if
  // the element is not found.
  const PCCounts* maybeGetThrowCounts(size_t offset) const;
||||||| merged common ancestors
    // PCCounts are stored at jump-target offsets. This function looks for the
    // previous PCCount which is in the same basic block as the current offset.
    PCCounts* getImmediatePrecedingPCCounts(size_t offset);
=======
  // PCCounts are stored at jump-target offsets. This function looks for the
  // previous PCCount which is in the same basic block as the current offset.
  PCCounts* getImmediatePrecedingPCCounts(size_t offset);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Throw counts are stored at the location of each throwing
  // instruction. This function looks for the previous throw count.
  //
  // Note: if the offset of the returned count is higher than the offset of
  // the immediate preceding PCCount, then this throw happened in the same
  // basic block.
  const PCCounts* getImmediatePrecedingThrowCounts(size_t offset) const;
||||||| merged common ancestors
    // Return the counter used to count the number of throws. Returns null if
    // the element is not found.
    const PCCounts* maybeGetThrowCounts(size_t offset) const;
=======
  // Return the counter used to count the number of throws. Returns null if
  // the element is not found.
  const PCCounts* maybeGetThrowCounts(size_t offset) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Return the counter used to count the number of throws. Allocate it if
  // none exists yet. Returns null if the allocation failed.
  PCCounts* getThrowCounts(size_t offset);
||||||| merged common ancestors
    // Throw counts are stored at the location of each throwing
    // instruction. This function looks for the previous throw count.
    //
    // Note: if the offset of the returned count is higher than the offset of
    // the immediate preceding PCCount, then this throw happened in the same
    // basic block.
    const PCCounts* getImmediatePrecedingThrowCounts(size_t offset) const;
=======
  // Throw counts are stored at the location of each throwing
  // instruction. This function looks for the previous throw count.
  //
  // Note: if the offset of the returned count is higher than the offset of
  // the immediate preceding PCCount, then this throw happened in the same
  // basic block.
  const PCCounts* getImmediatePrecedingThrowCounts(size_t offset) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t sizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf);
||||||| merged common ancestors
    // Return the counter used to count the number of throws. Allocate it if
    // none exists yet. Returns null if the allocation failed.
    PCCounts* getThrowCounts(size_t offset);
=======
  // Return the counter used to count the number of throws. Allocate it if
  // none exists yet. Returns null if the allocation failed.
  PCCounts* getThrowCounts(size_t offset);
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  friend class ::JSScript;
  friend struct ScriptAndCounts;
||||||| merged common ancestors
    size_t sizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf);
=======
  size_t sizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // This sorted array is used to map an offset to the number of times a
  // branch got visited.
  PCCountsVector pcCounts_;
||||||| merged common ancestors
  private:
    friend class ::JSScript;
    friend struct ScriptAndCounts;
=======
 private:
  friend class ::JSScript;
  friend struct ScriptAndCounts;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // This sorted vector is used to map an offset to the number of times an
  // instruction throw.
  PCCountsVector throwCounts_;
||||||| merged common ancestors
    // This sorted array is used to map an offset to the number of times a
    // branch got visited.
    PCCountsVector pcCounts_;
=======
  // This sorted array is used to map an offset to the number of times a
  // branch got visited.
  PCCountsVector pcCounts_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Information about any Ion compilations for the script.
  jit::IonScriptCounts* ionCounts_;
||||||| merged common ancestors
    // This sorted vector is used to map an offset to the number of times an
    // instruction throw.
    PCCountsVector throwCounts_;

    // Information about any Ion compilations for the script.
    jit::IonScriptCounts* ionCounts_;
=======
  // This sorted vector is used to map an offset to the number of times an
  // instruction throw.
  PCCountsVector throwCounts_;

  // Information about any Ion compilations for the script.
  jit::IonScriptCounts* ionCounts_;
>>>>>>> upstream-releases
};

// Note: The key of this hash map is a weak reference to a JSScript.  We do not
// use the WeakMap implementation provided in gc/WeakMap.h because it would be
// collected at the beginning of the sweeping of the realm, thus before the
// calls to the JSScript::finalize function which are used to aggregate code
// coverage results on the realm.
using UniqueScriptCounts = js::UniquePtr<ScriptCounts>;
<<<<<<< HEAD
using ScriptCountsMap = HashMap<JSScript*, UniqueScriptCounts,
                                DefaultHasher<JSScript*>, SystemAllocPolicy>;

using ScriptNameMap = HashMap<JSScript*, JS::UniqueChars,
                              DefaultHasher<JSScript*>, SystemAllocPolicy>;

#ifdef MOZ_VTUNE
using ScriptVTuneIdMap =
    HashMap<JSScript*, uint32_t, DefaultHasher<JSScript*>, SystemAllocPolicy>;
#endif

class DebugScript {
  friend class ::JSScript;
  friend class JS::Realm;

  /*
   * When non-zero, compile script in single-step mode. The top bit is set and
   * cleared by setStepMode, as used by JSD. The lower bits are a count,
   * adjusted by changeStepModeCount, used by the Debugger object. Only
   * when the bit is clear and the count is zero may we compile the script
   * without single-step support.
   */
  uint32_t stepMode;

  /*
   * Number of breakpoint sites at opcodes in the script. This is the number
   * of populated entries in DebugScript::breakpoints, below.
   */
  uint32_t numSites;

  /*
   * Breakpoints set in our script. For speed and simplicity, this array is
   * parallel to script->code(): the BreakpointSite for the opcode at
   * script->code()[offset] is debugScript->breakpoints[offset]. Naturally,
   * this array's true length is script->length().
   */
  BreakpointSite* breakpoints[1];
||||||| merged common ancestors
using ScriptCountsMap = HashMap<JSScript*,
                                UniqueScriptCounts,
                                DefaultHasher<JSScript*>,
                                SystemAllocPolicy>;

using ScriptNameMap = HashMap<JSScript*,
                              JS::UniqueChars,
                              DefaultHasher<JSScript*>,
                              SystemAllocPolicy>;

class DebugScript
{
    friend class ::JSScript;
    friend class JS::Realm;

    /*
     * When non-zero, compile script in single-step mode. The top bit is set and
     * cleared by setStepMode, as used by JSD. The lower bits are a count,
     * adjusted by changeStepModeCount, used by the Debugger object. Only
     * when the bit is clear and the count is zero may we compile the script
     * without single-step support.
     */
    uint32_t        stepMode;

    /*
     * Number of breakpoint sites at opcodes in the script. This is the number
     * of populated entries in DebugScript::breakpoints, below.
     */
    uint32_t        numSites;

    /*
     * Breakpoints set in our script. For speed and simplicity, this array is
     * parallel to script->code(): the BreakpointSite for the opcode at
     * script->code()[offset] is debugScript->breakpoints[offset]. Naturally,
     * this array's true length is script->length().
     */
    BreakpointSite* breakpoints[1];
=======
using ScriptCountsMap = HashMap<JSScript*, UniqueScriptCounts,
                                DefaultHasher<JSScript*>, SystemAllocPolicy>;

using ScriptNameMap = HashMap<JSScript*, JS::UniqueChars,
                              DefaultHasher<JSScript*>, SystemAllocPolicy>;

#ifdef MOZ_VTUNE
using ScriptVTuneIdMap =
    HashMap<JSScript*, uint32_t, DefaultHasher<JSScript*>, SystemAllocPolicy>;
#endif

class DebugScript {
  friend class ::JSScript;
  friend class JS::Realm;

  /*
   * If this is a generator script, this is the number of Debugger.Frames
   * referring to calls to this generator, whether live or suspended. Closed
   * generators do not contribute a count.
   *
   * When greater than zero, this script should be compiled with debug
   * instrumentation to call Debugger::onResumeFrame at each resumption site, so
   * that Debugger can reconnect any extant Debugger.Frames with the new
   * concrete frame.
   */
  uint32_t generatorObserverCount;

  /*
   * The number of Debugger.Frame objects that refer to frames running this
   * script and that have onStep handlers. When nonzero, the interpreter and JIT
   * must arrange to call Debugger::onSingleStep before each bytecode, or at
   * least at some useful granularity.
   */
  uint32_t stepperCount;

  /*
   * Number of breakpoint sites at opcodes in the script. This is the number
   * of populated entries in DebugScript::breakpoints, below.
   */
  uint32_t numSites;

  /*
   * Breakpoints set in our script. For speed and simplicity, this array is
   * parallel to script->code(): the BreakpointSite for the opcode at
   * script->code()[offset] is debugScript->breakpoints[offset]. Naturally,
   * this array's true length is script->length().
   */
  BreakpointSite* breakpoints[1];

  /*
   * True if this DebugScript carries any useful information. If false, it
   * should be removed from its JSScript.
   */
  bool needed() const {
    return generatorObserverCount > 0 || stepperCount > 0 || numSites > 0;
  }

  static size_t allocSize(size_t codeLength) {
    return offsetof(DebugScript, breakpoints) +
           codeLength * sizeof(BreakpointSite*);
  }
>>>>>>> upstream-releases
};

using UniqueDebugScript = js::UniquePtr<DebugScript, JS::FreePolicy>;
using DebugScriptMap = HashMap<JSScript*, UniqueDebugScript,
                               DefaultHasher<JSScript*>, SystemAllocPolicy>;

class ScriptSource;

struct ScriptSourceChunk {
  ScriptSource* ss = nullptr;
  uint32_t chunk = 0;

  ScriptSourceChunk() = default;

  ScriptSourceChunk(ScriptSource* ss, uint32_t chunk) : ss(ss), chunk(chunk) {
    MOZ_ASSERT(valid());
  }

  bool valid() const { return ss != nullptr; }

  bool operator==(const ScriptSourceChunk& other) const {
    return ss == other.ss && chunk == other.chunk;
  }
};

struct ScriptSourceChunkHasher {
  using Lookup = ScriptSourceChunk;

  static HashNumber hash(const ScriptSourceChunk& ssc) {
    return mozilla::AddToHash(DefaultHasher<ScriptSource*>::hash(ssc.ss),
                              ssc.chunk);
  }
  static bool match(const ScriptSourceChunk& c1, const ScriptSourceChunk& c2) {
    return c1 == c2;
  }
};

template <typename Unit>
using EntryUnits = mozilla::UniquePtr<Unit[], JS::FreePolicy>;

// The uncompressed source cache contains *either* UTF-8 source data *or*
// UTF-16 source data.  ScriptSourceChunk implies a ScriptSource that
// contains either UTF-8 data or UTF-16 data, so the nature of the key to
// Map below indicates how each SourceData ought to be interpreted.
using SourceData = mozilla::UniquePtr<void, JS::FreePolicy>;

template <typename Unit>
inline SourceData ToSourceData(EntryUnits<Unit> chars) {
  static_assert(std::is_same<SourceData::DeleterType,
                             typename EntryUnits<Unit>::DeleterType>::value,
                "EntryUnits and SourceData must share the same deleter "
                "type, that need not know the type of the data being freed, "
                "for the upcast below to be safe");
  return SourceData(chars.release());
}

class UncompressedSourceCache {
  using Map = HashMap<ScriptSourceChunk, SourceData, ScriptSourceChunkHasher,
                      SystemAllocPolicy>;

 public:
  // Hold an entry in the source data cache and prevent it from being purged on
  // GC.
  class AutoHoldEntry {
    UncompressedSourceCache* cache_ = nullptr;
    ScriptSourceChunk sourceChunk_ = {};
    SourceData data_ = nullptr;

   public:
    explicit AutoHoldEntry() = default;

    ~AutoHoldEntry() {
      if (cache_) {
        MOZ_ASSERT(sourceChunk_.valid());
        cache_->releaseEntry(*this);
      }
    }

    template <typename Unit>
    void holdUnits(EntryUnits<Unit> units) {
      MOZ_ASSERT(!cache_);
      MOZ_ASSERT(!sourceChunk_.valid());
      MOZ_ASSERT(!data_);

      data_ = ToSourceData(std::move(units));
    }

   private:
    void holdEntry(UncompressedSourceCache* cache,
                   const ScriptSourceChunk& sourceChunk) {
      // Initialise the holder for a specific cache and script source.
      // This will hold on to the cached source chars in the event that
      // the cache is purged.
      MOZ_ASSERT(!cache_);
      MOZ_ASSERT(!sourceChunk_.valid());
      MOZ_ASSERT(!data_);

      cache_ = cache;
      sourceChunk_ = sourceChunk;
    }

<<<<<<< HEAD
    void deferDelete(SourceData data) {
      // Take ownership of source chars now the cache is being purged. Remove
      // our reference to the ScriptSource which might soon be destroyed.
      MOZ_ASSERT(cache_);
      MOZ_ASSERT(sourceChunk_.valid());
      MOZ_ASSERT(!data_);
||||||| merged common ancestors
    static char16_t* toString(const char16_t* units) {
        return const_cast<char16_t*>(units);
    }
=======
    void deferDelete(SourceData data) {
      // Take ownership of source chars now the cache is being purged. Remove
      // our reference to the ScriptSource which might soon be destroyed.
      MOZ_ASSERT(cache_);
      MOZ_ASSERT(sourceChunk_.valid());
      MOZ_ASSERT(!data_);

      cache_ = nullptr;
      sourceChunk_ = ScriptSourceChunk();
>>>>>>> upstream-releases

<<<<<<< HEAD
      cache_ = nullptr;
      sourceChunk_ = ScriptSourceChunk();

      data_ = std::move(data);
||||||| merged common ancestors
    static UniqueTwoByteChars toCacheable(EntryUnits<char16_t> str) {
        return UniqueTwoByteChars(std::move(str));
=======
      data_ = std::move(data);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD

    const ScriptSourceChunk& sourceChunk() const { return sourceChunk_; }
    friend class UncompressedSourceCache;
  };

 private:
  UniquePtr<Map> map_ = nullptr;
  AutoHoldEntry* holder_ = nullptr;

 public:
  UncompressedSourceCache() = default;

  template <typename Unit>
  const Unit* lookup(const ScriptSourceChunk& ssc, AutoHoldEntry& asp);

  bool put(const ScriptSourceChunk& ssc, SourceData data, AutoHoldEntry& asp);

  void purge();

  size_t sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf);

 private:
  void holdEntry(AutoHoldEntry& holder, const ScriptSourceChunk& ssc);
  void releaseEntry(AutoHoldEntry& holder);
};

template <typename Unit>
struct SourceTypeTraits;

template <>
struct SourceTypeTraits<mozilla::Utf8Unit> {
  using CharT = char;
  using SharedImmutableString = js::SharedImmutableString;

  static const mozilla::Utf8Unit* units(const SharedImmutableString& string) {
    // Casting |char| data to |Utf8Unit| is safe because |Utf8Unit|
    // contains a |char|.  See the long comment in |Utf8Unit|'s definition.
    return reinterpret_cast<const mozilla::Utf8Unit*>(string.chars());
  }

  static char* toString(const mozilla::Utf8Unit* units) {
    auto asUnsigned =
        const_cast<unsigned char*>(mozilla::Utf8AsUnsignedChars(units));
    return reinterpret_cast<char*>(asUnsigned);
  }

  static UniqueChars toCacheable(EntryUnits<mozilla::Utf8Unit> str) {
    // The cache only stores strings of |char| or |char16_t|, and right now
    // it seems best not to gunk up the cache with |Utf8Unit| too.  So
    // cache |Utf8Unit| strings by interpreting them as |char| strings.
    char* chars = toString(str.release());
    return UniqueChars(chars);
  }
||||||| merged common ancestors
=======

    const ScriptSourceChunk& sourceChunk() const { return sourceChunk_; }
    friend class UncompressedSourceCache;
  };

 private:
  UniquePtr<Map> map_ = nullptr;
  AutoHoldEntry* holder_ = nullptr;

 public:
  UncompressedSourceCache() = default;

  template <typename Unit>
  const Unit* lookup(const ScriptSourceChunk& ssc, AutoHoldEntry& asp);

  bool put(const ScriptSourceChunk& ssc, SourceData data, AutoHoldEntry& asp);

  void purge();

  size_t sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf);

 private:
  void holdEntry(AutoHoldEntry& holder, const ScriptSourceChunk& ssc);
  void releaseEntry(AutoHoldEntry& holder);
>>>>>>> upstream-releases
};

<<<<<<< HEAD
template <>
struct SourceTypeTraits<char16_t> {
  using CharT = char16_t;
  using SharedImmutableString = js::SharedImmutableTwoByteString;

  static const char16_t* units(const SharedImmutableString& string) {
    return string.chars();
  }

  static char16_t* toString(const char16_t* units) {
    return const_cast<char16_t*>(units);
  }

  static UniqueTwoByteChars toCacheable(EntryUnits<char16_t> str) {
    return UniqueTwoByteChars(std::move(str));
  }
};

class ScriptSource {
  friend class SourceCompressionTask;

  class PinnedUnitsBase {
   protected:
    PinnedUnitsBase** stack_ = nullptr;
    PinnedUnitsBase* prev_ = nullptr;

    ScriptSource* source_;

    explicit PinnedUnitsBase(ScriptSource* source) : source_(source) {}
  };

 public:
  // Any users that wish to manipulate the char buffer of the ScriptSource
  // needs to do so via PinnedUnits for GC safety. A GC may compress
  // ScriptSources. If the source were initially uncompressed, then any raw
  // pointers to the char buffer would now point to the freed, uncompressed
  // chars. This is analogous to Rooted.
  template <typename Unit>
  class PinnedUnits : public PinnedUnitsBase {
    const Unit* units_;

   public:
    PinnedUnits(JSContext* cx, ScriptSource* source,
                UncompressedSourceCache::AutoHoldEntry& holder, size_t begin,
                size_t len);

    ~PinnedUnits();

    const Unit* get() const { return units_; }

    const typename SourceTypeTraits<Unit>::CharT* asChars() const {
      return SourceTypeTraits<Unit>::toString(get());
    }
  };

 private:
  mozilla::Atomic<uint32_t, mozilla::ReleaseAcquire,
                  mozilla::recordreplay::Behavior::DontPreserve>
      refs;

  // Note: while ScriptSources may be compressed off thread, they are only
  // modified by the main thread, and all members are always safe to access
  // on the main thread.

  // Indicate which field in the |data| union is active.
  struct Missing {};

  template <typename Unit>
  class Uncompressed {
    typename SourceTypeTraits<Unit>::SharedImmutableString string_;

   public:
    explicit Uncompressed(
        typename SourceTypeTraits<Unit>::SharedImmutableString str)
        : string_(std::move(str)) {}

    const Unit* units() const { return SourceTypeTraits<Unit>::units(string_); }

    size_t length() const { return string_.length(); }
  };

  template <typename Unit>
  struct Compressed {
    // Single-byte compressed text, regardless whether the original text
    // was single-byte or two-byte.
    SharedImmutableString raw;
    size_t uncompressedLength;

    Compressed(SharedImmutableString raw, size_t uncompressedLength)
        : raw(std::move(raw)), uncompressedLength(uncompressedLength) {}
  };

  struct BinAST {
    SharedImmutableString string;
    explicit BinAST(SharedImmutableString&& str) : string(std::move(str)) {}
  };

  using SourceType =
      mozilla::Variant<Compressed<mozilla::Utf8Unit>,
                       Uncompressed<mozilla::Utf8Unit>, Compressed<char16_t>,
                       Uncompressed<char16_t>, Missing, BinAST>;
  SourceType data;

  // If the GC attempts to call setCompressedSource with PinnedUnits
  // present, the first PinnedUnits (that is, bottom of the stack) will set
  // the compressed chars upon destruction.
  PinnedUnitsBase* pinnedUnitsStack_;
  mozilla::MaybeOneOf<Compressed<mozilla::Utf8Unit>, Compressed<char16_t>>
      pendingCompressed_;

  // The filename of this script.
  UniqueChars filename_;

  UniqueTwoByteChars displayURL_;
  UniqueTwoByteChars sourceMapURL_;
  bool mutedErrors_;

  // bytecode offset in caller script that generated this code.
  // This is present for eval-ed code, as well as "new Function(...)"-introduced
  // scripts.
  uint32_t introductionOffset_;

  // If this source is for Function constructor, the position of ")" after
  // parameter list in the source.  This is used to get function body.
  // 0 for other cases.
  uint32_t parameterListEnd_;

  // If this ScriptSource was generated by a code-introduction mechanism such
  // as |eval| or |new Function|, the debugger needs access to the "raw"
  // filename of the top-level script that contains the eval-ing code.  To
  // keep track of this, we must preserve the original outermost filename (of
  // the original introducer script), so that instead of a filename of
  // "foo.js line 30 > eval line 10 > Function", we can obtain the original
  // raw filename of "foo.js".
  //
  // In the case described above, this field will be non-null and will be the
  // original raw filename from above.  Otherwise this field will be null.
  UniqueChars introducerFilename_;

  // A string indicating how this source code was introduced into the system.
  // This accessor returns one of the following values:
  //      "eval" for code passed to |eval|.
  //      "Function" for code passed to the |Function| constructor.
  //      "Worker" for code loaded by calling the Web worker
  //      constructor&mdash;the worker's main script. "importScripts" for code
  //      by calling |importScripts| in a web worker. "handler" for code
  //      assigned to DOM elements' event handler IDL attributes.
  //      "scriptElement" for code belonging to <script> elements.
  //      undefined if the implementation doesn't know how the code was
  //      introduced.
  // This is a constant, statically allocated C string, so does not need
  // memory management.
  const char* introductionType_;

  // The bytecode cache encoder is used to encode only the content of function
  // which are delazified.  If this value is not nullptr, then each delazified
  // function should be recorded before their first execution.
  UniquePtr<XDRIncrementalEncoder> xdrEncoder_;

  // Instant at which the first parse of this source ended, or null
  // if the source hasn't been parsed yet.
  //
  // Used for statistics purposes, to determine how much time code spends
  // syntax parsed before being full parsed, to help determine whether
  // our syntax parse vs. full parse heuristics are correct.
  mozilla::TimeStamp parseEnded_;

  // True if we can call JSRuntime::sourceHook to load the source on
  // demand. If sourceRetrievable_ and hasSourceText() are false, it is not
  // possible to get source at all.
  bool sourceRetrievable_ : 1;
  bool hasIntroductionOffset_ : 1;
  bool containsAsmJS_ : 1;

  UniquePtr<frontend::BinASTSourceMetadata> binASTMetadata_;

  template <typename Unit>
  const Unit* chunkUnits(JSContext* cx,
                         UncompressedSourceCache::AutoHoldEntry& holder,
                         size_t chunk);

  // Return a string containing the chars starting at |begin| and ending at
  // |begin + len|.
  //
  // Warning: this is *not* GC-safe! Any chars to be handed out should use
  // PinnedUnits. See comment below.
  template <typename Unit>
  const Unit* units(JSContext* cx, UncompressedSourceCache::AutoHoldEntry& asp,
                    size_t begin, size_t len);
||||||| merged common ancestors
class ScriptSource
{
    friend class SourceCompressionTask;

    class PinnedUnitsBase
    {
      protected:
        PinnedUnitsBase** stack_ = nullptr;
        PinnedUnitsBase* prev_ = nullptr;

        ScriptSource* source_;

        explicit PinnedUnitsBase(ScriptSource* source)
          : source_(source)
        {}
    };

  public:
    // Any users that wish to manipulate the char buffer of the ScriptSource
    // needs to do so via PinnedUnits for GC safety. A GC may compress
    // ScriptSources. If the source were initially uncompressed, then any raw
    // pointers to the char buffer would now point to the freed, uncompressed
    // chars. This is analogous to Rooted.
    template<typename Unit>
    class PinnedUnits : public PinnedUnitsBase
    {
        const Unit* units_;

      public:
        PinnedUnits(JSContext* cx, ScriptSource* source,
                    UncompressedSourceCache::AutoHoldEntry& holder,
                    size_t begin, size_t len);
=======
template <typename Unit>
struct SourceTypeTraits;
>>>>>>> upstream-releases

<<<<<<< HEAD
  template <typename Unit>
  void movePendingCompressedSource();

 public:
  // When creating a JSString* from TwoByte source characters, we don't try to
  // to deflate to Latin1 for longer strings, because this can be slow.
  static const size_t SourceDeflateLimit = 100;

  explicit ScriptSource()
||||||| merged common ancestors
        ~PinnedUnits();

        const Unit* get() const {
            return units_;
        }

        const typename SourceTypeTraits<Unit>::CharT* asChars() const {
            return SourceTypeTraits<Unit>::toString(get());
        }
    };

  private:
    mozilla::Atomic<uint32_t, mozilla::ReleaseAcquire,
                    mozilla::recordreplay::Behavior::DontPreserve> refs;

    // Note: while ScriptSources may be compressed off thread, they are only
    // modified by the main thread, and all members are always safe to access
    // on the main thread.

    // Indicate which field in the |data| union is active.
    struct Missing { };

    template<typename Unit>
    class Uncompressed
    {
        typename SourceTypeTraits<Unit>::SharedImmutableString string_;


      public:
        explicit Uncompressed(typename SourceTypeTraits<Unit>::SharedImmutableString str)
          : string_(std::move(str))
        {}

        const Unit* units() const {
            return SourceTypeTraits<Unit>::units(string_);
        }

        size_t length() const {
            return string_.length();
        }
    };

    template<typename Unit>
    struct Compressed
    {
        // Single-byte compressed text, regardless whether the original text
        // was single-byte or two-byte.
        SharedImmutableString raw;
        size_t uncompressedLength;

        Compressed(SharedImmutableString raw, size_t uncompressedLength)
          : raw(std::move(raw)),
            uncompressedLength(uncompressedLength)
        { }
    };

    struct BinAST
    {
        SharedImmutableString string;
        explicit BinAST(SharedImmutableString&& str)
          : string(std::move(str))
        { }
    };

    using SourceType =
        mozilla::Variant<Compressed<mozilla::Utf8Unit>, Uncompressed<mozilla::Utf8Unit>,
                         Compressed<char16_t>, Uncompressed<char16_t>,
                         Missing,
                         BinAST>;
    SourceType data;

    // If the GC attempts to call setCompressedSource with PinnedUnits
    // present, the first PinnedUnits (that is, bottom of the stack) will set
    // the compressed chars upon destruction.
    PinnedUnitsBase* pinnedUnitsStack_;
    mozilla::MaybeOneOf<Compressed<mozilla::Utf8Unit>, Compressed<char16_t>> pendingCompressed_;

    // The filename of this script.
    UniqueChars filename_;

    UniqueTwoByteChars displayURL_;
    UniqueTwoByteChars sourceMapURL_;
    bool mutedErrors_;

    // bytecode offset in caller script that generated this code.
    // This is present for eval-ed code, as well as "new Function(...)"-introduced
    // scripts.
    uint32_t introductionOffset_;

    // If this source is for Function constructor, the position of ")" after
    // parameter list in the source.  This is used to get function body.
    // 0 for other cases.
    uint32_t parameterListEnd_;

    // If this ScriptSource was generated by a code-introduction mechanism such
    // as |eval| or |new Function|, the debugger needs access to the "raw"
    // filename of the top-level script that contains the eval-ing code.  To
    // keep track of this, we must preserve the original outermost filename (of
    // the original introducer script), so that instead of a filename of
    // "foo.js line 30 > eval line 10 > Function", we can obtain the original
    // raw filename of "foo.js".
    //
    // In the case described above, this field will be non-null and will be the
    // original raw filename from above.  Otherwise this field will be null.
    UniqueChars introducerFilename_;

    // A string indicating how this source code was introduced into the system.
    // This accessor returns one of the following values:
    //      "eval" for code passed to |eval|.
    //      "Function" for code passed to the |Function| constructor.
    //      "Worker" for code loaded by calling the Web worker constructor&mdash;the worker's main script.
    //      "importScripts" for code by calling |importScripts| in a web worker.
    //      "handler" for code assigned to DOM elements' event handler IDL attributes.
    //      "scriptElement" for code belonging to <script> elements.
    //      undefined if the implementation doesn't know how the code was introduced.
    // This is a constant, statically allocated C string, so does not need
    // memory management.
    const char* introductionType_;

    // The bytecode cache encoder is used to encode only the content of function
    // which are delazified.  If this value is not nullptr, then each delazified
    // function should be recorded before their first execution.
    UniquePtr<XDRIncrementalEncoder> xdrEncoder_;

    // Instant at which the first parse of this source ended, or null
    // if the source hasn't been parsed yet.
    //
    // Used for statistics purposes, to determine how much time code spends
    // syntax parsed before being full parsed, to help determine whether
    // our syntax parse vs. full parse heuristics are correct.
    mozilla::TimeStamp parseEnded_;

    // True if we can call JSRuntime::sourceHook to load the source on
    // demand. If sourceRetrievable_ and hasSourceText() are false, it is not
    // possible to get source at all.
    bool sourceRetrievable_:1;
    bool hasIntroductionOffset_:1;
    bool containsAsmJS_:1;

    UniquePtr<frontend::BinASTSourceMetadata> binASTMetadata_;

    template<typename Unit>
    const Unit* chunkUnits(JSContext* cx, UncompressedSourceCache::AutoHoldEntry& holder,
                           size_t chunk);

    // Return a string containing the chars starting at |begin| and ending at
    // |begin + len|.
    //
    // Warning: this is *not* GC-safe! Any chars to be handed out should use
    // PinnedUnits. See comment below.
    template<typename Unit>
    const Unit* units(JSContext* cx, UncompressedSourceCache::AutoHoldEntry& asp,
                      size_t begin, size_t len);

    template<typename Unit>
    void movePendingCompressedSource();

  public:
    // When creating a JSString* from TwoByte source characters, we don't try to
    // to deflate to Latin1 for longer strings, because this can be slow.
    static const size_t SourceDeflateLimit = 100;

    explicit ScriptSource()
=======
template <>
struct SourceTypeTraits<mozilla::Utf8Unit> {
  using CharT = char;
  using SharedImmutableString = js::SharedImmutableString;

  static const mozilla::Utf8Unit* units(const SharedImmutableString& string) {
    // Casting |char| data to |Utf8Unit| is safe because |Utf8Unit|
    // contains a |char|.  See the long comment in |Utf8Unit|'s definition.
    return reinterpret_cast<const mozilla::Utf8Unit*>(string.chars());
  }

  static char* toString(const mozilla::Utf8Unit* units) {
    auto asUnsigned =
        const_cast<unsigned char*>(mozilla::Utf8AsUnsignedChars(units));
    return reinterpret_cast<char*>(asUnsigned);
  }

  static UniqueChars toCacheable(EntryUnits<mozilla::Utf8Unit> str) {
    // The cache only stores strings of |char| or |char16_t|, and right now
    // it seems best not to gunk up the cache with |Utf8Unit| too.  So
    // cache |Utf8Unit| strings by interpreting them as |char| strings.
    char* chars = toString(str.release());
    return UniqueChars(chars);
  }
};

template <>
struct SourceTypeTraits<char16_t> {
  using CharT = char16_t;
  using SharedImmutableString = js::SharedImmutableTwoByteString;

  static const char16_t* units(const SharedImmutableString& string) {
    return string.chars();
  }

  static char16_t* toString(const char16_t* units) {
    return const_cast<char16_t*>(units);
  }

  static UniqueTwoByteChars toCacheable(EntryUnits<char16_t> str) {
    return UniqueTwoByteChars(std::move(str));
  }
};

// Synchronously compress the source of |script|, for testing purposes.
extern MOZ_MUST_USE bool SynchronouslyCompressSource(
    JSContext* cx, JS::Handle<JSScript*> script);

class ScriptSourceHolder;

// Retrievable source can be retrieved using the source hook (and therefore
// need not be XDR'd, can be discarded if desired because it can always be
// reconstituted later, etc.).
enum class SourceRetrievable { Yes, No };

class ScriptSource {
  friend class SourceCompressionTask;

  class PinnedUnitsBase {
   protected:
    PinnedUnitsBase** stack_ = nullptr;
    PinnedUnitsBase* prev_ = nullptr;

    ScriptSource* source_;

    explicit PinnedUnitsBase(ScriptSource* source) : source_(source) {}
  };

 public:
  // Any users that wish to manipulate the char buffer of the ScriptSource
  // needs to do so via PinnedUnits for GC safety. A GC may compress
  // ScriptSources. If the source were initially uncompressed, then any raw
  // pointers to the char buffer would now point to the freed, uncompressed
  // chars. This is analogous to Rooted.
  template <typename Unit>
  class PinnedUnits : public PinnedUnitsBase {
    const Unit* units_;

   public:
    PinnedUnits(JSContext* cx, ScriptSource* source,
                UncompressedSourceCache::AutoHoldEntry& holder, size_t begin,
                size_t len);

    ~PinnedUnits();

    const Unit* get() const { return units_; }

    const typename SourceTypeTraits<Unit>::CharT* asChars() const {
      return SourceTypeTraits<Unit>::toString(get());
    }
  };

 private:
  mozilla::Atomic<uint32_t, mozilla::ReleaseAcquire,
                  mozilla::recordreplay::Behavior::DontPreserve>
      refs;

  // Note: while ScriptSources may be compressed off thread, they are only
  // modified by the main thread, and all members are always safe to access
  // on the main thread.

  // Indicate which field in the |data| union is active.

  template <typename Unit>
  class UncompressedData {
    typename SourceTypeTraits<Unit>::SharedImmutableString string_;

   public:
    explicit UncompressedData(
        typename SourceTypeTraits<Unit>::SharedImmutableString str)
        : string_(std::move(str)) {}

    const Unit* units() const { return SourceTypeTraits<Unit>::units(string_); }

    size_t length() const { return string_.length(); }
  };

  // Uncompressed source text.
  template <typename Unit, SourceRetrievable CanRetrieve>
  class Uncompressed : public UncompressedData<Unit> {
    using Base = UncompressedData<Unit>;

   public:
    using Base::Base;
  };

  template <typename Unit>
  struct CompressedData {
    // Single-byte compressed text, regardless whether the original text
    // was single-byte or two-byte.
    SharedImmutableString raw;
    size_t uncompressedLength;

    CompressedData(SharedImmutableString raw, size_t uncompressedLength)
        : raw(std::move(raw)), uncompressedLength(uncompressedLength) {}
  };

  // Compressed source text.
  template <typename Unit, SourceRetrievable CanRetrieve>
  struct Compressed : public CompressedData<Unit> {
    using Base = CompressedData<Unit>;

   public:
    using Base::Base;
  };

  // Source that can be retrieved using the registered source hook.  |Unit|
  // records the source type so that source-text coordinates in functions and
  // scripts that depend on this |ScriptSource| are correct.
  template <typename Unit>
  struct Retrievable {
    // The source hook and script URL required to retrieve source are stored
    // elsewhere, so nothing is needed here.  It'd be better hygiene to store
    // something source-hook-like in each |ScriptSource| that needs it, but that
    // requires reimagining a source-hook API that currently depends on source
    // hooks being uniquely-owned pointers...
  };

  // Missing source text that isn't retrievable using the source hook.  (All
  // ScriptSources initially begin in this state.  Users that are compiling
  // source text will overwrite |data| to store a different state.)
  struct Missing {};

  // BinAST source.
  struct BinAST {
    SharedImmutableString string;
    UniquePtr<frontend::BinASTSourceMetadata> metadata;

    BinAST(SharedImmutableString&& str,
           UniquePtr<frontend::BinASTSourceMetadata> metadata)
        : string(std::move(str)), metadata(std::move(metadata)) {}
  };

  using SourceType =
      mozilla::Variant<Compressed<mozilla::Utf8Unit, SourceRetrievable::Yes>,
                       Uncompressed<mozilla::Utf8Unit, SourceRetrievable::Yes>,
                       Compressed<mozilla::Utf8Unit, SourceRetrievable::No>,
                       Uncompressed<mozilla::Utf8Unit, SourceRetrievable::No>,
                       Compressed<char16_t, SourceRetrievable::Yes>,
                       Uncompressed<char16_t, SourceRetrievable::Yes>,
                       Compressed<char16_t, SourceRetrievable::No>,
                       Uncompressed<char16_t, SourceRetrievable::No>,
                       Retrievable<mozilla::Utf8Unit>, Retrievable<char16_t>,
                       Missing, BinAST>;
  SourceType data;

  friend bool SynchronouslyCompressSource(JSContext* cx,
                                          JS::Handle<JSScript*> script);

  // If the GC calls triggerConvertToCompressedSource with PinnedUnits present,
  // the first PinnedUnits (that is, bottom of the stack) will install the
  // compressed chars upon destruction.
  //
  // Retrievability isn't part of the type here because uncompressed->compressed
  // transitions must preserve existing retrievability.
  PinnedUnitsBase* pinnedUnitsStack_;
  mozilla::MaybeOneOf<CompressedData<mozilla::Utf8Unit>,
                      CompressedData<char16_t>>
      pendingCompressed_;

  // The filename of this script.
  UniqueChars filename_;

  UniqueTwoByteChars displayURL_;
  UniqueTwoByteChars sourceMapURL_;
  bool mutedErrors_;

  // bytecode offset in caller script that generated this code.
  // This is present for eval-ed code, as well as "new Function(...)"-introduced
  // scripts.
  uint32_t introductionOffset_;

  // If this source is for Function constructor, the position of ")" after
  // parameter list in the source.  This is used to get function body.
  // 0 for other cases.
  uint32_t parameterListEnd_;

  // If this ScriptSource was generated by a code-introduction mechanism such
  // as |eval| or |new Function|, the debugger needs access to the "raw"
  // filename of the top-level script that contains the eval-ing code.  To
  // keep track of this, we must preserve the original outermost filename (of
  // the original introducer script), so that instead of a filename of
  // "foo.js line 30 > eval line 10 > Function", we can obtain the original
  // raw filename of "foo.js".
  //
  // In the case described above, this field will be non-null and will be the
  // original raw filename from above.  Otherwise this field will be null.
  UniqueChars introducerFilename_;

  // A string indicating how this source code was introduced into the system.
  // This accessor returns one of the following values:
  //      "eval" for code passed to |eval|.
  //      "Function" for code passed to the |Function| constructor.
  //      "Worker" for code loaded by calling the Web worker
  //      constructor&mdash;the worker's main script. "importScripts" for code
  //      by calling |importScripts| in a web worker. "handler" for code
  //      assigned to DOM elements' event handler IDL attributes.
  //      "scriptElement" for code belonging to <script> elements.
  //      undefined if the implementation doesn't know how the code was
  //      introduced.
  // This is a constant, statically allocated C string, so does not need
  // memory management.
  const char* introductionType_;

  // The bytecode cache encoder is used to encode only the content of function
  // which are delazified.  If this value is not nullptr, then each delazified
  // function should be recorded before their first execution.
  UniquePtr<XDRIncrementalEncoder> xdrEncoder_;

  // Instant at which the first parse of this source ended, or null
  // if the source hasn't been parsed yet.
  //
  // Used for statistics purposes, to determine how much time code spends
  // syntax parsed before being full parsed, to help determine whether
  // our syntax parse vs. full parse heuristics are correct.
  mozilla::TimeStamp parseEnded_;

  // An id for this source that is unique across the process. This can be used
  // to refer to this source from places that don't want to hold a strong
  // reference on the source itself.
  //
  // This is a 32 bit ID and could overflow, in which case the ID will not be
  // unique anymore.
  uint32_t id_;

  // How many ids have been handed out to sources.
  static mozilla::Atomic<uint32_t, mozilla::SequentiallyConsistent,
                         mozilla::recordreplay::Behavior::DontPreserve>
      idCount_;

  bool hasIntroductionOffset_ : 1;
  bool containsAsmJS_ : 1;

  template <typename Unit>
  const Unit* chunkUnits(JSContext* cx,
                         UncompressedSourceCache::AutoHoldEntry& holder,
                         size_t chunk);

  // Return a string containing the chars starting at |begin| and ending at
  // |begin + len|.
  //
  // Warning: this is *not* GC-safe! Any chars to be handed out must use
  // PinnedUnits. See comment below.
  template <typename Unit>
  const Unit* units(JSContext* cx, UncompressedSourceCache::AutoHoldEntry& asp,
                    size_t begin, size_t len);

 public:
  // When creating a JSString* from TwoByte source characters, we don't try to
  // to deflate to Latin1 for longer strings, because this can be slow.
  static const size_t SourceDeflateLimit = 100;

  explicit ScriptSource()
>>>>>>> upstream-releases
      : refs(0),
        data(SourceType(Missing())),
        pinnedUnitsStack_(nullptr),
        filename_(nullptr),
        displayURL_(nullptr),
        sourceMapURL_(nullptr),
        mutedErrors_(false),
        introductionOffset_(0),
        parameterListEnd_(0),
        introducerFilename_(nullptr),
        introductionType_(nullptr),
        xdrEncoder_(nullptr),
        id_(++idCount_),
        hasIntroductionOffset_(false),
<<<<<<< HEAD
        containsAsmJS_(false) {}

  ~ScriptSource() { MOZ_ASSERT(refs == 0); }

  void incref() { refs++; }
  void decref() {
    MOZ_ASSERT(refs != 0);
    if (--refs == 0) {
      js_delete(this);
    }
  }
  MOZ_MUST_USE bool initFromOptions(
      JSContext* cx, const JS::ReadOnlyCompileOptions& options,
      const mozilla::Maybe<uint32_t>& parameterListEnd = mozilla::Nothing());

  /**
   * The minimum script length (in code units) necessary for a script to be
   * eligible to be compressed.
   */
  static constexpr size_t MinimumCompressibleLength = 256;

  template <typename Unit>
  MOZ_MUST_USE bool setSourceCopy(JSContext* cx, JS::SourceText<Unit>& srcBuf);

  void setSourceRetrievable() { sourceRetrievable_ = true; }
  bool sourceRetrievable() const { return sourceRetrievable_; }
  bool hasSourceText() const {
    return hasUncompressedSource() || hasCompressedSource();
  }
  bool hasBinASTSource() const { return data.is<BinAST>(); }

  void setBinASTSourceMetadata(frontend::BinASTSourceMetadata* metadata) {
    MOZ_ASSERT(hasBinASTSource());
    binASTMetadata_.reset(metadata);
  }
  frontend::BinASTSourceMetadata* binASTSourceMetadata() const {
    MOZ_ASSERT(hasBinASTSource());
    return binASTMetadata_.get();
  }

 private:
  struct UncompressedDataMatcher {
    template <typename Unit>
    const void* match(const Uncompressed<Unit>& u) {
      return u.units();
||||||| merged common ancestors
        containsAsmJS_(false)
    {
    }

    ~ScriptSource() {
        MOZ_ASSERT(refs == 0);
=======
        containsAsmJS_(false) {}

  ~ScriptSource() { MOZ_ASSERT(refs == 0); }

  void incref() { refs++; }
  void decref() {
    MOZ_ASSERT(refs != 0);
    if (--refs == 0) {
      js_delete(this);
    }
  }
  MOZ_MUST_USE bool initFromOptions(
      JSContext* cx, const JS::ReadOnlyCompileOptions& options,
      const mozilla::Maybe<uint32_t>& parameterListEnd = mozilla::Nothing());

  /**
   * The minimum script length (in code units) necessary for a script to be
   * eligible to be compressed.
   */
  static constexpr size_t MinimumCompressibleLength = 256;

 private:
  class LoadSourceMatcher;

 public:
  // Attempt to load usable source for |ss| -- source text on which substring
  // operations and the like can be performed.  On success return true and set
  // |*loaded| to indicate whether usable source could be loaded; otherwise
  // return false.
  static bool loadSource(JSContext* cx, ScriptSource* ss, bool* loaded);

  // Assign source data from |srcBuf| to this recently-created |ScriptSource|.
  template <typename Unit>
  MOZ_MUST_USE bool assignSource(JSContext* cx,
                                 const JS::ReadOnlyCompileOptions& options,
                                 JS::SourceText<Unit>& srcBuf);

  bool hasSourceText() const {
    return hasUncompressedSource() || hasCompressedSource();
  }
  bool hasBinASTSource() const { return data.is<BinAST>(); }

  void setBinASTSourceMetadata(frontend::BinASTSourceMetadata* metadata) {
    MOZ_ASSERT(hasBinASTSource());
    data.as<BinAST>().metadata.reset(metadata);
  }
  frontend::BinASTSourceMetadata* binASTSourceMetadata() const {
    MOZ_ASSERT(hasBinASTSource());
    return data.as<BinAST>().metadata.get();
  }

 private:
  template <typename Unit>
  struct UncompressedDataMatcher {
    template <SourceRetrievable CanRetrieve>
    const UncompressedData<Unit>* operator()(
        const Uncompressed<Unit, CanRetrieve>& u) {
      return &u;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    template <typename T>
    const void* match(const T&) {
      MOZ_CRASH(
          "attempting to access uncompressed data in a "
          "ScriptSource not containing it");
      return nullptr;
    }
  };

 public:
  template <typename Unit>
  const Unit* uncompressedData() {
    return static_cast<const Unit*>(data.match(UncompressedDataMatcher()));
  }

 private:
  struct CompressedDataMatcher {
    template <typename Unit>
    char* match(const Compressed<Unit>& c) {
      return const_cast<char*>(c.raw.chars());
||||||| merged common ancestors
    void incref() { refs++; }
    void decref() {
        MOZ_ASSERT(refs != 0);
        if (--refs == 0) {
            js_delete(this);
        }
    }
    MOZ_MUST_USE bool initFromOptions(JSContext* cx,
                                      const JS::ReadOnlyCompileOptions& options,
                                      const mozilla::Maybe<uint32_t>& parameterListEnd = mozilla::Nothing());
    MOZ_MUST_USE bool setSourceCopy(JSContext* cx, JS::SourceBufferHolder& srcBuf);
    void setSourceRetrievable() { sourceRetrievable_ = true; }
    bool sourceRetrievable() const { return sourceRetrievable_; }
    bool hasSourceText() const { return hasUncompressedSource() || hasCompressedSource(); }
    bool hasBinASTSource() const { return data.is<BinAST>(); }

    void setBinASTSourceMetadata(frontend::BinASTSourceMetadata* metadata) {
        MOZ_ASSERT(hasBinASTSource());
        binASTMetadata_.reset(metadata);
=======
    template <typename T>
    const UncompressedData<Unit>* operator()(const T&) {
      MOZ_CRASH(
          "attempting to access uncompressed data in a ScriptSource not "
          "containing it");
      return nullptr;
    }
  };

 public:
  template <typename Unit>
  const UncompressedData<Unit>* uncompressedData() {
    return data.match(UncompressedDataMatcher<Unit>());
  }

 private:
  template <typename Unit>
  struct CompressedDataMatcher {
    template <SourceRetrievable CanRetrieve>
    const CompressedData<Unit>* operator()(
        const Compressed<Unit, CanRetrieve>& c) {
      return &c;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD

    template <typename T>
    char* match(const T&) {
      MOZ_CRASH(
          "attempting to access compressed data in a ScriptSource "
          "not containing it");
      return nullptr;
||||||| merged common ancestors
    frontend::BinASTSourceMetadata* binASTSourceMetadata() const {
        MOZ_ASSERT(hasBinASTSource());
        return binASTMetadata_.get();
=======

    template <typename T>
    const CompressedData<Unit>* operator()(const T&) {
      MOZ_CRASH(
          "attempting to access compressed data in a ScriptSource not "
          "containing it");
      return nullptr;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  };

 public:
  template <typename Unit>
  char* compressedData() {
    return data.match(CompressedDataMatcher());
  }

 private:
  struct BinASTDataMatcher {
    void* match(const BinAST& b) { return const_cast<char*>(b.string.chars()); }
||||||| merged common ancestors
=======
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
    void notBinAST() { MOZ_CRASH("ScriptSource isn't backed by BinAST data"); }

    template <typename T>
    void* match(const T&) {
      notBinAST();
      return nullptr;
    }
  };

 public:
  void* binASTData() { return data.match(BinASTDataMatcher()); }
||||||| merged common ancestors
  private:
    struct UncompressedDataMatcher
    {
        template<typename Unit>
        const void* match(const Uncompressed<Unit>& u) {
            return u.units();
        }

        template<typename T>
        const void* match(const T&) {
            MOZ_CRASH("attempting to access uncompressed data in a "
                      "ScriptSource not containing it");
            return nullptr;
        }
    };

  public:
    template<typename Unit>
    const Unit* uncompressedData() {
        return static_cast<const Unit*>(data.match(UncompressedDataMatcher()));
    }
=======
 public:
  template <typename Unit>
  const CompressedData<Unit>* compressedData() {
    return data.match(CompressedDataMatcher<Unit>());
  }

 private:
  struct BinASTDataMatcher {
    void* operator()(const BinAST& b) {
      return const_cast<char*>(b.string.chars());
    }

    void notBinAST() { MOZ_CRASH("ScriptSource isn't backed by BinAST data"); }

    template <typename T>
    void* operator()(const T&) {
      notBinAST();
      return nullptr;
    }
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  struct HasUncompressedSource {
    template <typename Unit>
    bool match(const Uncompressed<Unit>&) {
      return true;
||||||| merged common ancestors
  private:
    struct CompressedDataMatcher
    {
        template<typename Unit>
        char* match(const Compressed<Unit>& c) {
            return const_cast<char*>(c.raw.chars());
        }

        template<typename T>
        char* match(const T&) {
            MOZ_CRASH("attempting to access compressed data in a ScriptSource "
                      "not containing it");
            return nullptr;
        }
    };

  public:
    template<typename Unit>
    char* compressedData() {
        return data.match(CompressedDataMatcher());
=======
 public:
  void* binASTData() { return data.match(BinASTDataMatcher()); }

 private:
  struct HasUncompressedSource {
    template <typename Unit, SourceRetrievable CanRetrieve>
    bool operator()(const Uncompressed<Unit, CanRetrieve>&) {
      return true;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    template <typename Unit>
    bool match(const Compressed<Unit>&) {
      return false;
||||||| merged common ancestors
  private:
    struct BinASTDataMatcher
    {
        void* match(const BinAST& b) {
            return const_cast<char*>(b.string.chars());
        }

        void notBinAST() {
            MOZ_CRASH("ScriptSource isn't backed by BinAST data");
        }

        template<typename T>
        void* match(const T&) {
            notBinAST();
            return nullptr;
        }
    };

  public:
    void* binASTData() {
        return data.match(BinASTDataMatcher());
=======
    template <typename Unit, SourceRetrievable CanRetrieve>
    bool operator()(const Compressed<Unit, CanRetrieve>&) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    bool match(const BinAST&) { return false; }
||||||| merged common ancestors
  private:
    struct HasUncompressedSource
    {
        template<typename Unit>
        bool match(const Uncompressed<Unit>&) { return true; }
=======
    template <typename Unit>
    bool operator()(const Retrievable<Unit>&) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool match(const Missing&) { return false; }
  };
||||||| merged common ancestors
        template<typename Unit>
        bool match(const Compressed<Unit>&) { return false; }
=======
    bool operator()(const BinAST&) { return false; }
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  bool hasUncompressedSource() const {
    return data.match(HasUncompressedSource());
  }
||||||| merged common ancestors
        bool match(const BinAST&) { return false; }
=======
    bool operator()(const Missing&) { return false; }
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
  template <typename Unit>
  bool uncompressedSourceIs() const {
    MOZ_ASSERT(hasUncompressedSource());
    return data.is<Uncompressed<Unit>>();
  }
||||||| merged common ancestors
        bool match(const Missing&) { return false; }
    };
=======
 public:
  bool hasUncompressedSource() const {
    return data.match(HasUncompressedSource());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  struct HasCompressedSource {
    template <typename Unit>
    bool match(const Compressed<Unit>&) {
      return true;
||||||| merged common ancestors
  public:
    bool hasUncompressedSource() const {
        return data.match(HasUncompressedSource());
=======
 private:
  template <typename Unit>
  struct IsUncompressed {
    template <SourceRetrievable CanRetrieve>
    bool operator()(const Uncompressed<Unit, CanRetrieve>&) {
      return true;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    template <typename Unit>
    bool match(const Uncompressed<Unit>&) {
      return false;
||||||| merged common ancestors
    template<typename Unit>
    bool uncompressedSourceIs() const {
        MOZ_ASSERT(hasUncompressedSource());
        return data.is<Uncompressed<Unit>>();
=======
    template <typename T>
    bool operator()(const T&) {
      return false;
>>>>>>> upstream-releases
    }
  };

<<<<<<< HEAD
    bool match(const BinAST&) { return false; }
||||||| merged common ancestors
  private:
    struct HasCompressedSource
    {
        template<typename Unit>
        bool match(const Compressed<Unit>&) { return true; }
=======
 public:
  template <typename Unit>
  bool isUncompressed() const {
    return data.match(IsUncompressed<Unit>());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool match(const Missing&) { return false; }
  };
||||||| merged common ancestors
        template<typename Unit>
        bool match(const Uncompressed<Unit>&) { return false; }
=======
 private:
  struct HasCompressedSource {
    template <typename Unit, SourceRetrievable CanRetrieve>
    bool operator()(const Compressed<Unit, CanRetrieve>&) {
      return true;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  bool hasCompressedSource() const { return data.match(HasCompressedSource()); }
||||||| merged common ancestors
        bool match(const BinAST&) { return false; }
=======
    template <typename T>
    bool operator()(const T&) {
      return false;
    }
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
  template <typename Unit>
  bool compressedSourceIs() const {
    MOZ_ASSERT(hasCompressedSource());
    return data.is<Compressed<Unit>>();
  }
||||||| merged common ancestors
        bool match(const Missing&) { return false; }
    };
=======
 public:
  bool hasCompressedSource() const { return data.match(HasCompressedSource()); }
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  template <typename Unit>
  struct SourceTypeMatcher {
    template <template <typename C> class Data>
    bool match(const Data<Unit>&) {
      return true;
||||||| merged common ancestors
  public:
    bool hasCompressedSource() const {
        return data.match(HasCompressedSource());
=======
 private:
  template <typename Unit>
  struct IsCompressed {
    template <SourceRetrievable CanRetrieve>
    bool operator()(const Compressed<Unit, CanRetrieve>&) {
      return true;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    template <template <typename C> class Data, typename NotUnit>
    bool match(const Data<NotUnit>&) {
      return false;
||||||| merged common ancestors
    template<typename Unit>
    bool compressedSourceIs() const {
        MOZ_ASSERT(hasCompressedSource());
        return data.is<Compressed<Unit>>();
=======
    template <typename T>
    bool operator()(const T&) {
      return false;
>>>>>>> upstream-releases
    }
  };

<<<<<<< HEAD
    bool match(const BinAST&) {
      MOZ_CRASH("doesn't make sense to ask source type of BinAST data");
      return false;
    }
||||||| merged common ancestors
  private:
    template<typename Unit>
    struct SourceTypeMatcher
    {
        template<template<typename C> class Data>
        bool match(const Data<Unit>&) {
            return true;
        }

        template<template<typename C> class Data, typename NotUnit>
        bool match(const Data<NotUnit>&) {
            return false;
        }

        bool match(const BinAST&) {
            MOZ_CRASH("doesn't make sense to ask source type of BinAST data");
            return false;
        }

        bool match(const Missing&) {
            MOZ_CRASH("doesn't make sense to ask source type when missing");
            return false;
        }
    };

  public:
    template<typename Unit>
    bool hasSourceType() const {
        return data.match(SourceTypeMatcher<Unit>());
    }
=======
 public:
  template <typename Unit>
  bool isCompressed() const {
    return data.match(IsCompressed<Unit>());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool match(const Missing&) {
      MOZ_CRASH("doesn't make sense to ask source type when missing");
      return false;
||||||| merged common ancestors
  private:
    struct SourceCharSizeMatcher
    {
        template<template<typename C> class Data, typename Unit>
        uint8_t match(const Data<Unit>& data) {
            static_assert(std::is_same<Unit, mozilla::Utf8Unit>::value ||
                          std::is_same<Unit, char16_t>::value,
                          "should only have UTF-8 or UTF-16 source char");
            return sizeof(Unit);
        }

        uint8_t match(const BinAST&) {
            MOZ_CRASH("BinAST source has no source-char size");
            return 0;
        }

        uint8_t match(const Missing&) {
            MOZ_CRASH("missing source has no source-char size");
            return 0;
        }
    };

  public:
    uint8_t sourceCharSize() const {
        return data.match(SourceCharSizeMatcher());
=======
 private:
  template <typename Unit>
  struct SourceTypeMatcher {
    template <template <typename C, SourceRetrievable R> class Data,
              SourceRetrievable CanRetrieve>
    bool operator()(const Data<Unit, CanRetrieve>&) {
      return true;
>>>>>>> upstream-releases
    }
  };

<<<<<<< HEAD
 public:
  template <typename Unit>
  bool hasSourceType() const {
    return data.match(SourceTypeMatcher<Unit>());
  }

 private:
  struct SourceCharSizeMatcher {
    template <template <typename C> class Data, typename Unit>
    uint8_t match(const Data<Unit>& data) {
      static_assert(std::is_same<Unit, mozilla::Utf8Unit>::value ||
                        std::is_same<Unit, char16_t>::value,
                    "should only have UTF-8 or UTF-16 source char");
      return sizeof(Unit);
||||||| merged common ancestors
  private:
    struct UncompressedLengthMatcher
    {
        template<typename Unit>
        size_t match(const Uncompressed<Unit>& u) {
            return u.length();
        }

        template<typename Unit>
        size_t match(const Compressed<Unit>& u) {
            return u.uncompressedLength;
        }

        size_t match(const BinAST& b) {
            return b.string.length();
        }

        size_t match(const Missing& m) {
            MOZ_CRASH("ScriptSource::length on a missing source");
            return 0;
        }
    };

  public:
    size_t length() const {
        MOZ_ASSERT(hasSourceText() || hasBinASTSource());
        return data.match(UncompressedLengthMatcher());
=======
    template <template <typename C, SourceRetrievable R> class Data,
              typename NotUnit, SourceRetrievable CanRetrieve>
    bool operator()(const Data<NotUnit, CanRetrieve>&) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    uint8_t match(const BinAST&) {
      MOZ_CRASH("BinAST source has no source-char size");
      return 0;
||||||| merged common ancestors
  private:
    struct CompressedLengthOrZeroMatcher
    {
        template<typename Unit>
        size_t match(const Uncompressed<Unit>&) {
            return 0;
        }

        template<typename Unit>
        size_t match(const Compressed<Unit>& c) {
            return c.raw.length();
        }

        size_t match(const BinAST&) {
            MOZ_CRASH("trying to get compressed length for BinAST data");
            return 0;
        }

        size_t match(const Missing&) {
            MOZ_CRASH("missing source data");
            return 0;
        }
    };

  public:
    size_t compressedLengthOrZero() const {
        return data.match(CompressedLengthOrZeroMatcher());
=======
    bool operator()(const Retrievable<Unit>&) {
      MOZ_CRASH("source type only applies where actual text is available");
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    uint8_t match(const Missing&) {
      MOZ_CRASH("missing source has no source-char size");
      return 0;
    }
  };
||||||| merged common ancestors
    JSFlatString* substring(JSContext* cx, size_t start, size_t stop);
    JSFlatString* substringDontDeflate(JSContext* cx, size_t start, size_t stop);
=======
    template <typename NotUnit>
    bool operator()(const Retrievable<NotUnit>&) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  uint8_t sourceCharSize() const { return data.match(SourceCharSizeMatcher()); }
||||||| merged common ancestors
    MOZ_MUST_USE bool appendSubstring(JSContext* cx, js::StringBuffer& buf, size_t start, size_t stop);
=======
    bool operator()(const BinAST&) {
      MOZ_CRASH("doesn't make sense to ask source type of BinAST data");
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  struct UncompressedLengthMatcher {
    template <typename Unit>
    size_t match(const Uncompressed<Unit>& u) {
      return u.length();
||||||| merged common ancestors
    bool isFunctionBody() {
        return parameterListEnd_ != 0;
=======
    bool operator()(const Missing&) {
      MOZ_CRASH("doesn't make sense to ask source type when missing");
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
||||||| merged common ancestors
    JSFlatString* functionBodyString(JSContext* cx);
=======
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
    template <typename Unit>
    size_t match(const Compressed<Unit>& u) {
      return u.uncompressedLength;
    }
||||||| merged common ancestors
    void addSizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf,
                                JS::ScriptSourceInfo* info) const;
=======
 public:
  template <typename Unit>
  bool hasSourceType() const {
    return data.match(SourceTypeMatcher<Unit>());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    size_t match(const BinAST& b) { return b.string.length(); }
||||||| merged common ancestors
    template<typename Unit>
    MOZ_MUST_USE bool setSource(JSContext* cx,
                                EntryUnits<Unit>&& source,
                                size_t length);
=======
 private:
  struct UncompressedLengthMatcher {
    template <typename Unit, SourceRetrievable CanRetrieve>
    size_t operator()(const Uncompressed<Unit, CanRetrieve>& u) {
      return u.length();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    size_t match(const Missing& m) {
      MOZ_CRASH("ScriptSource::length on a missing source");
      return 0;
    }
  };
||||||| merged common ancestors
    template<typename Unit>
    void setSource(typename SourceTypeTraits<Unit>::SharedImmutableString uncompressed);
=======
    template <typename Unit, SourceRetrievable CanRetrieve>
    size_t operator()(const Compressed<Unit, CanRetrieve>& u) {
      return u.uncompressedLength;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  size_t length() const {
    MOZ_ASSERT(hasSourceText() || hasBinASTSource());
    return data.match(UncompressedLengthMatcher());
  }
||||||| merged common ancestors
    MOZ_MUST_USE bool tryCompressOffThread(JSContext* cx);
=======
    template <typename Unit>
    size_t operator()(const Retrievable<Unit>&) {
      MOZ_CRASH("ScriptSource::length on a missing-but-retrievable source");
      return 0;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  struct CompressedLengthOrZeroMatcher {
    template <typename Unit>
    size_t match(const Uncompressed<Unit>&) {
      return 0;
    }
||||||| merged common ancestors
    // The Unit parameter determines which type of compressed source is
    // recorded, but raw compressed source is always single-byte.
    template<typename Unit>
    void setCompressedSource(SharedImmutableString compressed, size_t sourceLength);
=======
    size_t operator()(const BinAST& b) { return b.string.length(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
    template <typename Unit>
    size_t match(const Compressed<Unit>& c) {
      return c.raw.length();
    }
||||||| merged common ancestors
    template<typename Unit>
    MOZ_MUST_USE bool setCompressedSource(JSContext* cx, UniqueChars&& raw, size_t rawLength,
                                          size_t sourceLength);
=======
    size_t operator()(const Missing& m) {
      MOZ_CRASH("ScriptSource::length on a missing source");
      return 0;
    }
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
    size_t match(const BinAST&) {
      MOZ_CRASH("trying to get compressed length for BinAST data");
      return 0;
    }
||||||| merged common ancestors
#if defined(JS_BUILD_BINAST)
=======
 public:
  size_t length() const {
    MOZ_ASSERT(hasSourceText() || hasBinASTSource());
    return data.match(UncompressedLengthMatcher());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    size_t match(const Missing&) {
      MOZ_CRASH("missing source data");
      return 0;
    }
  };
||||||| merged common ancestors
    /*
     * Do not take ownership of the given `buf`. Store the canonical, shared
     * and de-duplicated version. If there is no extant shared version of
     * `buf`, make a copy.
     */
    MOZ_MUST_USE bool setBinASTSourceCopy(JSContext* cx, const uint8_t* buf, size_t len);
=======
  JSFlatString* substring(JSContext* cx, size_t start, size_t stop);
  JSFlatString* substringDontDeflate(JSContext* cx, size_t start, size_t stop);
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  size_t compressedLengthOrZero() const {
    return data.match(CompressedLengthOrZeroMatcher());
  }
||||||| merged common ancestors
    /*
     * Take ownership of the given `buf` and return the canonical, shared and
     * de-duplicated version.
     */
    MOZ_MUST_USE bool setBinASTSource(JSContext* cx, UniqueChars&& buf, size_t len);
=======
  MOZ_MUST_USE bool appendSubstring(JSContext* cx, js::StringBuffer& buf,
                                    size_t start, size_t stop);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSFlatString* substring(JSContext* cx, size_t start, size_t stop);
  JSFlatString* substringDontDeflate(JSContext* cx, size_t start, size_t stop);
||||||| merged common ancestors
    const uint8_t* binASTSource();
=======
  bool isFunctionBody() { return parameterListEnd_ != 0; }
  JSFlatString* functionBodyString(JSContext* cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool appendSubstring(JSContext* cx, js::StringBuffer& buf,
                                    size_t start, size_t stop);
||||||| merged common ancestors
#endif /* JS_BUILD_BINAST */
=======
  void addSizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf,
                              JS::ScriptSourceInfo* info) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isFunctionBody() { return parameterListEnd_ != 0; }
  JSFlatString* functionBodyString(JSContext* cx);
||||||| merged common ancestors
  private:
    void performTaskWork(SourceCompressionTask* task);

    struct SetCompressedSourceFromTask
    {
        ScriptSource* const source_;
        SharedImmutableString& compressed_;

        SetCompressedSourceFromTask(ScriptSource* source, SharedImmutableString& compressed)
          : source_(source),
            compressed_(compressed)
        {}

        template<typename Unit>
        void match(const Uncompressed<Unit>&) {
            source_->setCompressedSource<Unit>(std::move(compressed_), source_->length());
        }

        template<typename Unit>
        void match(const Compressed<Unit>&) {
            MOZ_CRASH("can't set compressed source when source is already "
                      "compressed -- ScriptSource::tryCompressOffThread "
                      "shouldn't have queued up this task?");
        }

        void match(const BinAST&) {
            MOZ_CRASH("doesn't make sense to set compressed source for BinAST "
                      "data");
        }

        void match(const Missing&) {
            MOZ_CRASH("doesn't make sense to set compressed source for "
                      "missing source -- ScriptSource::tryCompressOffThread "
                      "shouldn't have queued up this task?");
        }
    };

    void setCompressedSourceFromTask(SharedImmutableString compressed);

  public:
    // XDR handling
    template <XDRMode mode>
    MOZ_MUST_USE XDRResult performXDR(XDRState<mode>* xdr);

  private:
    // It'd be better to make this function take <XDRMode, Unit>, as both
    // specializations of this function contain nested Unit-parametrized
    // helper classes that do everything the function needs to do.  But then
    // we'd need template function partial specialization to hold XDRMode
    // constant while varying Unit, so that idea's no dice.
    template <XDRMode mode>
    MOZ_MUST_USE XDRResult xdrUncompressedSource(XDRState<mode>* xdr, uint8_t sourceCharSize,
                                                 uint32_t uncompressedLength);

  public:
    MOZ_MUST_USE bool setFilename(JSContext* cx, const char* filename);
    const char* introducerFilename() const {
        return introducerFilename_ ? introducerFilename_.get() : filename_.get();
    }
    bool hasIntroductionType() const {
        return introductionType_;
    }
    const char* introductionType() const {
        MOZ_ASSERT(hasIntroductionType());
        return introductionType_;
    }
    const char* filename() const {
        return filename_.get();
    }
=======
 private:
  // Overwrites |data| with the uncompressed data from |source|.
  //
  // This function asserts nothing about |data|.  Users should use assertions to
  // double-check their own understandings of the |data| state transition being
  // performed.
  template <typename Unit>
  MOZ_MUST_USE bool setUncompressedSourceHelper(JSContext* cx,
                                                EntryUnits<Unit>&& source,
                                                size_t length,
                                                SourceRetrievable retrievable);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void addSizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf,
                              JS::ScriptSourceInfo* info) const;
||||||| merged common ancestors
    // Display URLs
    MOZ_MUST_USE bool setDisplayURL(JSContext* cx, const char16_t* displayURL);
    bool hasDisplayURL() const { return displayURL_ != nullptr; }
    const char16_t * displayURL() {
        MOZ_ASSERT(hasDisplayURL());
        return displayURL_.get();
    }
=======
 public:
  // Initialize a fresh |ScriptSource| with unretrievable, uncompressed source.
  template <typename Unit>
  MOZ_MUST_USE bool initializeUnretrievableUncompressedSource(
      JSContext* cx, EntryUnits<Unit>&& source, size_t length);
>>>>>>> upstream-releases

<<<<<<< HEAD
  template <typename Unit>
  MOZ_MUST_USE bool setSource(JSContext* cx, EntryUnits<Unit>&& source,
                              size_t length);
||||||| merged common ancestors
    // Source maps
    MOZ_MUST_USE bool setSourceMapURL(JSContext* cx, const char16_t* sourceMapURL);
    bool hasSourceMapURL() const { return sourceMapURL_ != nullptr; }
    const char16_t * sourceMapURL() {
        MOZ_ASSERT(hasSourceMapURL());
        return sourceMapURL_.get();
    }
=======
  // Set the retrieved source for a |ScriptSource| whose source was recorded as
  // missing but retrievable.
  template <typename Unit>
  MOZ_MUST_USE bool setRetrievedSource(JSContext* cx, EntryUnits<Unit>&& source,
                                       size_t length);
>>>>>>> upstream-releases

<<<<<<< HEAD
  template <typename Unit>
  void setSource(
      typename SourceTypeTraits<Unit>::SharedImmutableString uncompressed);
||||||| merged common ancestors
    bool mutedErrors() const { return mutedErrors_; }
=======
  MOZ_MUST_USE bool tryCompressOffThread(JSContext* cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool tryCompressOffThread(JSContext* cx);
||||||| merged common ancestors
    bool hasIntroductionOffset() const { return hasIntroductionOffset_; }
    uint32_t introductionOffset() const {
        MOZ_ASSERT(hasIntroductionOffset());
        return introductionOffset_;
    }
    void setIntroductionOffset(uint32_t offset) {
        MOZ_ASSERT(!hasIntroductionOffset());
        MOZ_ASSERT(offset <= (uint32_t)INT32_MAX);
        introductionOffset_ = offset;
        hasIntroductionOffset_ = true;
    }
=======
  // *Trigger* the conversion of this ScriptSource from containing uncompressed
  // |Unit|-encoded source to containing compressed source.  Conversion may not
  // be complete when this function returns: it'll be delayed if there's ongoing
  // use of the uncompressed source via |PinnedUnits|, in which case conversion
  // won't occur until the outermost |PinnedUnits| is destroyed.
  //
  // Compressed source is in bytes, no matter that |Unit| might be |char16_t|.
  // |sourceLength| is the length in code units (not bytes) of the uncompressed
  // source.
  template <typename Unit>
  void triggerConvertToCompressedSource(SharedImmutableString compressed,
                                        size_t sourceLength);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The Unit parameter determines which type of compressed source is
  // recorded, but raw compressed source is always single-byte.
  template <typename Unit>
  void setCompressedSource(SharedImmutableString compressed,
                           size_t sourceLength);
||||||| merged common ancestors
    bool containsAsmJS() const { return containsAsmJS_; }
    void setContainsAsmJS() {
        containsAsmJS_ = true;
    }
=======
  // Initialize a fresh ScriptSource as containing unretrievable compressed
  // source of the indicated original encoding.
  template <typename Unit>
  MOZ_MUST_USE bool initializeWithUnretrievableCompressedSource(
      JSContext* cx, UniqueChars&& raw, size_t rawLength, size_t sourceLength);
>>>>>>> upstream-releases

<<<<<<< HEAD
  template <typename Unit>
  MOZ_MUST_USE bool setCompressedSource(JSContext* cx, UniqueChars&& raw,
                                        size_t rawLength, size_t sourceLength);
||||||| merged common ancestors
    // Return wether an XDR encoder is present or not.
    bool hasEncoder() const { return bool(xdrEncoder_); }

    // Create a new XDR encoder, and encode the top-level JSScript. The result
    // of the encoding would be available in the |buffer| provided as argument,
    // as soon as |xdrFinalize| is called and all xdr function calls returned
    // successfully.
    bool xdrEncodeTopLevel(JSContext* cx, HandleScript script);

    // Encode a delazified JSFunction.  In case of errors, the XDR encoder is
    // freed and the |buffer| provided as argument to |xdrEncodeTopLevel| is
    // considered undefined.
    //
    // The |sourceObject| argument is the object holding the current
    // ScriptSource.
    bool xdrEncodeFunction(JSContext* cx, HandleFunction fun,
                           HandleScriptSourceObject sourceObject);

    // Linearize the encoded content in the |buffer| provided as argument to
    // |xdrEncodeTopLevel|, and free the XDR encoder.  In case of errors, the
    // |buffer| is considered undefined.
    bool xdrFinalizeEncoder(JS::TranscodeBuffer& buffer);

    const mozilla::TimeStamp parseEnded() const {
        return parseEnded_;
    }
    // Inform `this` source that it has been fully parsed.
    void recordParseEnded() {
        MOZ_ASSERT(parseEnded_.IsNull());
        parseEnded_ = ReallyNow();
    }
=======
#if defined(JS_BUILD_BINAST)

  /*
   * Do not take ownership of the given `buf`. Store the canonical, shared
   * and de-duplicated version. If there is no extant shared version of
   * `buf`, make a copy.
   */
  MOZ_MUST_USE bool setBinASTSourceCopy(JSContext* cx, const uint8_t* buf,
                                        size_t len);

  const uint8_t* binASTSource();
>>>>>>> upstream-releases

<<<<<<< HEAD
#if defined(JS_BUILD_BINAST)
||||||| merged common ancestors
    void trace(JSTracer* trc);
};
=======
#endif /* JS_BUILD_BINAST */
>>>>>>> upstream-releases

<<<<<<< HEAD
  /*
   * Do not take ownership of the given `buf`. Store the canonical, shared
   * and de-duplicated version. If there is no extant shared version of
   * `buf`, make a copy.
   */
  MOZ_MUST_USE bool setBinASTSourceCopy(JSContext* cx, const uint8_t* buf,
                                        size_t len);
||||||| merged common ancestors
class ScriptSourceHolder
{
    ScriptSource* ss;
  public:
    ScriptSourceHolder()
      : ss(nullptr)
    {}
    explicit ScriptSourceHolder(ScriptSource* ss)
      : ss(ss)
    {
        ss->incref();
    }
    ~ScriptSourceHolder()
    {
        if (ss) {
            ss->decref();
        }
    }
    void reset(ScriptSource* newss) {
        // incref before decref just in case ss == newss.
        if (newss) {
            newss->incref();
        }
        if (ss) {
            ss->decref();
        }
        ss = newss;
    }
    ScriptSource* get() const {
        return ss;
    }
};
=======
 private:
  void performTaskWork(SourceCompressionTask* task);

  struct TriggerConvertToCompressedSourceFromTask {
    ScriptSource* const source_;
    SharedImmutableString& compressed_;

    TriggerConvertToCompressedSourceFromTask(ScriptSource* source,
                                             SharedImmutableString& compressed)
        : source_(source), compressed_(compressed) {}

    template <typename Unit, SourceRetrievable CanRetrieve>
    void operator()(const Uncompressed<Unit, CanRetrieve>&) {
      source_->triggerConvertToCompressedSource<Unit>(std::move(compressed_),
                                                      source_->length());
    }

    template <typename Unit, SourceRetrievable CanRetrieve>
    void operator()(const Compressed<Unit, CanRetrieve>&) {
      MOZ_CRASH(
          "can't set compressed source when source is already compressed -- "
          "ScriptSource::tryCompressOffThread shouldn't have queued up this "
          "task?");
    }

    template <typename Unit>
    void operator()(const Retrievable<Unit>&) {
      MOZ_CRASH("shouldn't compressing unloaded-but-retrievable source");
    }

    void operator()(const BinAST&) {
      MOZ_CRASH("doesn't make sense to set compressed source for BinAST data");
    }

    void operator()(const Missing&) {
      MOZ_CRASH(
          "doesn't make sense to set compressed source for missing source -- "
          "ScriptSource::tryCompressOffThread shouldn't have queued up this "
          "task?");
    }
  };

  template <typename Unit>
  void convertToCompressedSource(SharedImmutableString compressed,
                                 size_t uncompressedLength);

  template <typename Unit>
  void performDelayedConvertToCompressedSource();

  void triggerConvertToCompressedSourceFromTask(
      SharedImmutableString compressed);

 private:
  // It'd be better to make this function take <XDRMode, Unit>, as both
  // specializations of this function contain nested Unit-parametrized
  // helper classes that do everything the function needs to do.  But then
  // we'd need template function partial specialization to hold XDRMode
  // constant while varying Unit, so that idea's no dice.
  template <XDRMode mode>
  MOZ_MUST_USE XDRResult xdrUnretrievableUncompressedSource(
      XDRState<mode>* xdr, uint8_t sourceCharSize, uint32_t uncompressedLength);

 public:
  MOZ_MUST_USE bool setFilename(JSContext* cx, const char* filename);
  const char* introducerFilename() const {
    return introducerFilename_ ? introducerFilename_.get() : filename_.get();
  }
  bool hasIntroductionType() const { return introductionType_; }
  const char* introductionType() const {
    MOZ_ASSERT(hasIntroductionType());
    return introductionType_;
  }
  const char* filename() const { return filename_.get(); }

  uint32_t id() const { return id_; }

  // Display URLs
  MOZ_MUST_USE bool setDisplayURL(JSContext* cx, const char16_t* displayURL);
  bool hasDisplayURL() const { return displayURL_ != nullptr; }
  const char16_t* displayURL() {
    MOZ_ASSERT(hasDisplayURL());
    return displayURL_.get();
  }

  // Source maps
  MOZ_MUST_USE bool setSourceMapURL(JSContext* cx,
                                    const char16_t* sourceMapURL);
  bool hasSourceMapURL() const { return sourceMapURL_ != nullptr; }
  const char16_t* sourceMapURL() {
    MOZ_ASSERT(hasSourceMapURL());
    return sourceMapURL_.get();
  }

  bool mutedErrors() const { return mutedErrors_; }

  bool hasIntroductionOffset() const { return hasIntroductionOffset_; }
  uint32_t introductionOffset() const {
    MOZ_ASSERT(hasIntroductionOffset());
    return introductionOffset_;
  }
  void setIntroductionOffset(uint32_t offset) {
    MOZ_ASSERT(!hasIntroductionOffset());
    MOZ_ASSERT(offset <= (uint32_t)INT32_MAX);
    introductionOffset_ = offset;
    hasIntroductionOffset_ = true;
  }

  bool containsAsmJS() const { return containsAsmJS_; }
  void setContainsAsmJS() { containsAsmJS_ = true; }

  // Return wether an XDR encoder is present or not.
  bool hasEncoder() const { return bool(xdrEncoder_); }

  // Create a new XDR encoder, and encode the top-level JSScript. The result
  // of the encoding would be available in the |buffer| provided as argument,
  // as soon as |xdrFinalize| is called and all xdr function calls returned
  // successfully.
  bool xdrEncodeTopLevel(JSContext* cx, HandleScript script);

  // Encode a delazified JSFunction.  In case of errors, the XDR encoder is
  // freed and the |buffer| provided as argument to |xdrEncodeTopLevel| is
  // considered undefined.
  //
  // The |sourceObject| argument is the object holding the current
  // ScriptSource.
  bool xdrEncodeFunction(JSContext* cx, HandleFunction fun,
                         HandleScriptSourceObject sourceObject);

  // Linearize the encoded content in the |buffer| provided as argument to
  // |xdrEncodeTopLevel|, and free the XDR encoder.  In case of errors, the
  // |buffer| is considered undefined.
  bool xdrFinalizeEncoder(JS::TranscodeBuffer& buffer);

  const mozilla::TimeStamp parseEnded() const { return parseEnded_; }
  // Inform `this` source that it has been fully parsed.
  void recordParseEnded() {
    MOZ_ASSERT(parseEnded_.IsNull());
    parseEnded_ = ReallyNow();
  }

 private:
  template <typename Unit,
            template <typename U, SourceRetrievable CanRetrieve> class Data,
            XDRMode mode>
  static void codeRetrievable(ScriptSource* ss);

  template <typename Unit, XDRMode mode>
  static MOZ_MUST_USE XDRResult codeUncompressedData(XDRState<mode>* const xdr,
                                                     ScriptSource* const ss);

  template <typename Unit, XDRMode mode>
  static MOZ_MUST_USE XDRResult codeCompressedData(XDRState<mode>* const xdr,
                                                   ScriptSource* const ss);

  template <XDRMode mode>
  static MOZ_MUST_USE XDRResult codeBinASTData(XDRState<mode>* const xdr,
                                               ScriptSource* const ss);

  template <typename Unit, XDRMode mode>
  static void codeRetrievableData(ScriptSource* ss);

  template <XDRMode mode>
  static MOZ_MUST_USE XDRResult xdrData(XDRState<mode>* const xdr,
                                        ScriptSource* const ss);

 public:
  template <XDRMode mode>
  static MOZ_MUST_USE XDRResult
  XDR(XDRState<mode>* xdr, const mozilla::Maybe<JS::CompileOptions>& options,
      MutableHandle<ScriptSourceHolder> ss);

  void trace(JSTracer* trc);
};
>>>>>>> upstream-releases

<<<<<<< HEAD
  /*
   * Take ownership of the given `buf` and return the canonical, shared and
   * de-duplicated version.
   */
  MOZ_MUST_USE bool setBinASTSource(JSContext* cx, UniqueChars&& buf,
                                    size_t len);

  const uint8_t* binASTSource();

#endif /* JS_BUILD_BINAST */
||||||| merged common ancestors
class ScriptSourceObject : public NativeObject
{
    static const ClassOps classOps_;

  public:
    static const Class class_;

    static void trace(JSTracer* trc, JSObject* obj);
    static void finalize(FreeOp* fop, JSObject* obj);
    static ScriptSourceObject* create(JSContext* cx, ScriptSource* source);
=======
class ScriptSourceHolder {
  ScriptSource* ss;
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  void performTaskWork(SourceCompressionTask* task);

  struct SetCompressedSourceFromTask {
    ScriptSource* const source_;
    SharedImmutableString& compressed_;

    SetCompressedSourceFromTask(ScriptSource* source,
                                SharedImmutableString& compressed)
        : source_(source), compressed_(compressed) {}

    template <typename Unit>
    void match(const Uncompressed<Unit>&) {
      source_->setCompressedSource<Unit>(std::move(compressed_),
                                         source_->length());
    }

    template <typename Unit>
    void match(const Compressed<Unit>&) {
      MOZ_CRASH(
          "can't set compressed source when source is already "
          "compressed -- ScriptSource::tryCompressOffThread "
          "shouldn't have queued up this task?");
    }

    void match(const BinAST&) {
      MOZ_CRASH(
          "doesn't make sense to set compressed source for BinAST "
          "data");
    }

    void match(const Missing&) {
      MOZ_CRASH(
          "doesn't make sense to set compressed source for "
          "missing source -- ScriptSource::tryCompressOffThread "
          "shouldn't have queued up this task?");
    }
  };

  void setCompressedSourceFromTask(SharedImmutableString compressed);

 public:
  // XDR handling
  template <XDRMode mode>
  MOZ_MUST_USE XDRResult performXDR(XDRState<mode>* xdr);

 private:
  // It'd be better to make this function take <XDRMode, Unit>, as both
  // specializations of this function contain nested Unit-parametrized
  // helper classes that do everything the function needs to do.  But then
  // we'd need template function partial specialization to hold XDRMode
  // constant while varying Unit, so that idea's no dice.
  template <XDRMode mode>
  MOZ_MUST_USE XDRResult xdrUncompressedSource(XDRState<mode>* xdr,
                                               uint8_t sourceCharSize,
                                               uint32_t uncompressedLength);

 public:
  MOZ_MUST_USE bool setFilename(JSContext* cx, const char* filename);
  const char* introducerFilename() const {
    return introducerFilename_ ? introducerFilename_.get() : filename_.get();
  }
  bool hasIntroductionType() const { return introductionType_; }
  const char* introductionType() const {
    MOZ_ASSERT(hasIntroductionType());
    return introductionType_;
  }
  const char* filename() const { return filename_.get(); }

  // Display URLs
  MOZ_MUST_USE bool setDisplayURL(JSContext* cx, const char16_t* displayURL);
  bool hasDisplayURL() const { return displayURL_ != nullptr; }
  const char16_t* displayURL() {
    MOZ_ASSERT(hasDisplayURL());
    return displayURL_.get();
  }

  // Source maps
  MOZ_MUST_USE bool setSourceMapURL(JSContext* cx,
                                    const char16_t* sourceMapURL);
  bool hasSourceMapURL() const { return sourceMapURL_ != nullptr; }
  const char16_t* sourceMapURL() {
    MOZ_ASSERT(hasSourceMapURL());
    return sourceMapURL_.get();
  }

  bool mutedErrors() const { return mutedErrors_; }

  bool hasIntroductionOffset() const { return hasIntroductionOffset_; }
  uint32_t introductionOffset() const {
    MOZ_ASSERT(hasIntroductionOffset());
    return introductionOffset_;
  }
  void setIntroductionOffset(uint32_t offset) {
    MOZ_ASSERT(!hasIntroductionOffset());
    MOZ_ASSERT(offset <= (uint32_t)INT32_MAX);
    introductionOffset_ = offset;
    hasIntroductionOffset_ = true;
  }

  bool containsAsmJS() const { return containsAsmJS_; }
  void setContainsAsmJS() { containsAsmJS_ = true; }

  // Return wether an XDR encoder is present or not.
  bool hasEncoder() const { return bool(xdrEncoder_); }

  // Create a new XDR encoder, and encode the top-level JSScript. The result
  // of the encoding would be available in the |buffer| provided as argument,
  // as soon as |xdrFinalize| is called and all xdr function calls returned
  // successfully.
  bool xdrEncodeTopLevel(JSContext* cx, HandleScript script);

  // Encode a delazified JSFunction.  In case of errors, the XDR encoder is
  // freed and the |buffer| provided as argument to |xdrEncodeTopLevel| is
  // considered undefined.
  //
  // The |sourceObject| argument is the object holding the current
  // ScriptSource.
  bool xdrEncodeFunction(JSContext* cx, HandleFunction fun,
                         HandleScriptSourceObject sourceObject);

  // Linearize the encoded content in the |buffer| provided as argument to
  // |xdrEncodeTopLevel|, and free the XDR encoder.  In case of errors, the
  // |buffer| is considered undefined.
  bool xdrFinalizeEncoder(JS::TranscodeBuffer& buffer);

  const mozilla::TimeStamp parseEnded() const { return parseEnded_; }
  // Inform `this` source that it has been fully parsed.
  void recordParseEnded() {
    MOZ_ASSERT(parseEnded_.IsNull());
    parseEnded_ = ReallyNow();
  }

  void trace(JSTracer* trc);
};

class ScriptSourceHolder {
  ScriptSource* ss;

 public:
  ScriptSourceHolder() : ss(nullptr) {}
  explicit ScriptSourceHolder(ScriptSource* ss) : ss(ss) { ss->incref(); }
  ~ScriptSourceHolder() {
    if (ss) {
      ss->decref();
||||||| merged common ancestors
    // Initialize those properties of this ScriptSourceObject whose values
    // are provided by |options|, re-wrapping as necessary.
    static bool initFromOptions(JSContext* cx, HandleScriptSourceObject source,
                                const JS::ReadOnlyCompileOptions& options);

    static bool initElementProperties(JSContext* cx, HandleScriptSourceObject source,
                                      HandleObject element, HandleString elementAttrName);

    bool hasSource() const {
        return !getReservedSlot(SOURCE_SLOT).isUndefined();
    }
    ScriptSource* source() const {
        return static_cast<ScriptSource*>(getReservedSlot(SOURCE_SLOT).toPrivate());
    }
    JSObject* element() const {
        return getReservedSlot(ELEMENT_SLOT).toObjectOrNull();
=======
 public:
  ScriptSourceHolder() : ss(nullptr) {}
  explicit ScriptSourceHolder(ScriptSource* ss) : ss(ss) { ss->incref(); }
  ~ScriptSourceHolder() {
    if (ss) {
      ss->decref();
>>>>>>> upstream-releases
    }
  }
  void reset(ScriptSource* newss) {
    // incref before decref just in case ss == newss.
    if (newss) {
      newss->incref();
    }
<<<<<<< HEAD
    if (ss) {
      ss->decref();
    }
    ss = newss;
  }
  ScriptSource* get() const { return ss; }
};
||||||| merged common ancestors
    JSScript* introductionScript() const {
        Value value = getReservedSlot(INTRODUCTION_SCRIPT_SLOT);
        if (value.isUndefined()) {
            return nullptr;
        }
        return value.toGCThing()->as<JSScript>();
    }

    void setPrivate(const Value& value) {
        setReservedSlot(PRIVATE_SLOT, value);
    }
    Value getPrivate() const {
        return getReservedSlot(PRIVATE_SLOT);
    }
=======
    if (ss) {
      ss->decref();
    }
    ss = newss;
  }
  ScriptSource* get() const { return ss; }

  void trace(JSTracer* trc) { ss->trace(trc); }
};
>>>>>>> upstream-releases

<<<<<<< HEAD
class ScriptSourceObject : public NativeObject {
  static const ClassOps classOps_;

 public:
  static const Class class_;

  static void trace(JSTracer* trc, JSObject* obj);
  static void finalize(FreeOp* fop, JSObject* obj);
  static ScriptSourceObject* create(JSContext* cx, ScriptSource* source);

  // Initialize those properties of this ScriptSourceObject whose values
  // are provided by |options|, re-wrapping as necessary.
  static bool initFromOptions(JSContext* cx, HandleScriptSourceObject source,
                              const JS::ReadOnlyCompileOptions& options);

  static bool initElementProperties(JSContext* cx,
                                    HandleScriptSourceObject source,
                                    HandleObject element,
                                    HandleString elementAttrName);

  bool hasSource() const { return !getReservedSlot(SOURCE_SLOT).isUndefined(); }
  ScriptSource* source() const {
    return static_cast<ScriptSource*>(getReservedSlot(SOURCE_SLOT).toPrivate());
  }
  JSObject* element() const {
    return getReservedSlot(ELEMENT_SLOT).toObjectOrNull();
  }
  const Value& elementAttributeName() const {
    MOZ_ASSERT(!getReservedSlot(ELEMENT_PROPERTY_SLOT).isMagic());
    return getReservedSlot(ELEMENT_PROPERTY_SLOT);
  }
  JSScript* introductionScript() const {
    Value value = getReservedSlot(INTRODUCTION_SCRIPT_SLOT);
    if (value.isUndefined()) {
      return nullptr;
    }
    return value.toGCThing()->as<JSScript>();
  }

  void setPrivate(const Value& value) { setReservedSlot(PRIVATE_SLOT, value); }
  Value getPrivate() const { return getReservedSlot(PRIVATE_SLOT); }

 private:
  enum {
    SOURCE_SLOT = 0,
    ELEMENT_SLOT,
    ELEMENT_PROPERTY_SLOT,
    INTRODUCTION_SCRIPT_SLOT,
    PRIVATE_SLOT,
    RESERVED_SLOTS
  };
||||||| merged common ancestors
  private:
    enum {
        SOURCE_SLOT = 0,
        ELEMENT_SLOT,
        ELEMENT_PROPERTY_SLOT,
        INTRODUCTION_SCRIPT_SLOT,
        PRIVATE_SLOT,
        RESERVED_SLOTS
    };
=======
// [SMDOC] ScriptSourceObject
//
// ScriptSourceObject stores the ScriptSource and GC pointers related to it.
//
// ScriptSourceObjects can be cloned when we clone the JSScript (in order to
// execute the script in a different compartment). In this case we create a new
// SSO that stores (a wrapper for) the original SSO in its "canonical slot".
// The canonical SSO is always used for the private, introductionScript,
// element, elementAttributeName slots. This means their accessors may return an
// object in a different compartment, hence the "unwrapped" prefix.
//
// Note that we don't clone the SSO when cloning the script for a different
// realm in the same compartment, so sso->realm() does not necessarily match the
// script's realm.
//
// We need ScriptSourceObject (instead of storing these GC pointers in the
// ScriptSource itself) to properly account for cross-zone pointers: the
// canonical SSO will be stored in the wrapper map if necessary so GC will do
// the right thing.
class ScriptSourceObject : public NativeObject {
  static const ClassOps classOps_;

  static ScriptSourceObject* createInternal(JSContext* cx, ScriptSource* source,
                                            HandleObject canonical);

  bool isCanonical() const {
    return &getReservedSlot(CANONICAL_SLOT).toObject() == this;
  }
  ScriptSourceObject* unwrappedCanonical() const;

 public:
  static const Class class_;

  static void trace(JSTracer* trc, JSObject* obj);
  static void finalize(FreeOp* fop, JSObject* obj);

  static ScriptSourceObject* create(JSContext* cx, ScriptSource* source);
  static ScriptSourceObject* clone(JSContext* cx, HandleScriptSourceObject sso);

  // Initialize those properties of this ScriptSourceObject whose values
  // are provided by |options|, re-wrapping as necessary.
  static bool initFromOptions(JSContext* cx, HandleScriptSourceObject source,
                              const JS::ReadOnlyCompileOptions& options);

  static bool initElementProperties(JSContext* cx,
                                    HandleScriptSourceObject source,
                                    HandleObject element,
                                    HandleString elementAttrName);

  bool hasSource() const { return !getReservedSlot(SOURCE_SLOT).isUndefined(); }
  ScriptSource* source() const {
    return static_cast<ScriptSource*>(getReservedSlot(SOURCE_SLOT).toPrivate());
  }

  JSObject* unwrappedElement() const {
    return unwrappedCanonical()->getReservedSlot(ELEMENT_SLOT).toObjectOrNull();
  }
  const Value& unwrappedElementAttributeName() const {
    const Value& v =
        unwrappedCanonical()->getReservedSlot(ELEMENT_PROPERTY_SLOT);
    MOZ_ASSERT(!v.isMagic());
    return v;
  }
  JSScript* unwrappedIntroductionScript() const {
    Value value =
        unwrappedCanonical()->getReservedSlot(INTRODUCTION_SCRIPT_SLOT);
    if (value.isUndefined()) {
      return nullptr;
    }
    return value.toGCThing()->as<JSScript>();
  }

  void setPrivate(JSRuntime* rt, const Value& value);

  Value canonicalPrivate() const {
    Value value = getReservedSlot(PRIVATE_SLOT);
    MOZ_ASSERT_IF(!isCanonical(), value.isUndefined());
    return value;
  }

 private:
  enum {
    SOURCE_SLOT = 0,
    CANONICAL_SLOT,
    ELEMENT_SLOT,
    ELEMENT_PROPERTY_SLOT,
    INTRODUCTION_SCRIPT_SLOT,
    PRIVATE_SLOT,
    RESERVED_SLOTS
  };
>>>>>>> upstream-releases
};

enum class GeneratorKind : bool { NotGenerator, Generator };
enum class FunctionAsyncKind : bool { SyncFunction, AsyncFunction };

struct FieldInitializers {
#ifdef DEBUG
  bool valid;
#endif
  // This struct will eventually have a vector of constant values for optimizing
  // field initializers.
  size_t numFieldInitializers;

  explicit FieldInitializers(size_t numFieldInitializers)
      :
#ifdef DEBUG
        valid(true),
#endif
        numFieldInitializers(numFieldInitializers) {
  }

  static FieldInitializers Invalid() { return FieldInitializers(); }

 private:
  FieldInitializers()
      :
#ifdef DEBUG
        valid(false),
#endif
        numFieldInitializers(0) {
  }
};

/*
 * NB: after a successful XDR_DECODE, XDRScript callers must do any required
 * subsequent set-up of owning function or script object and then call
 * CallNewScriptHook.
 */
<<<<<<< HEAD
template <XDRMode mode>
XDRResult XDRScript(XDRState<mode>* xdr, HandleScope enclosingScope,
                    HandleScriptSourceObject sourceObject, HandleFunction fun,
                    MutableHandleScript scriptp);

template <XDRMode mode>
XDRResult XDRLazyScript(XDRState<mode>* xdr, HandleScope enclosingScope,
                        HandleScriptSourceObject sourceObject,
                        HandleFunction fun, MutableHandle<LazyScript*> lazy);
||||||| merged common ancestors
template<XDRMode mode>
XDRResult
XDRScript(XDRState<mode>* xdr, HandleScope enclosingScope,
          HandleScriptSourceObject sourceObject,
          HandleFunction fun, MutableHandleScript scriptp);

template<XDRMode mode>
XDRResult
XDRLazyScript(XDRState<mode>* xdr, HandleScope enclosingScope,
              HandleScriptSourceObject sourceObject,
              HandleFunction fun, MutableHandle<LazyScript*> lazy);
=======
template <XDRMode mode>
XDRResult XDRScript(XDRState<mode>* xdr, HandleScope enclosingScope,
                    HandleScriptSourceObject sourceObject, HandleFunction fun,
                    MutableHandleScript scriptp);
>>>>>>> upstream-releases

<<<<<<< HEAD
/*
 * Code any constant value.
 */
template <XDRMode mode>
XDRResult XDRScriptConst(XDRState<mode>* xdr, MutableHandleValue vp);

// [SMDOC] - JSScript data layout (unshared)
//
// PrivateScriptData stores variable-length data associated with a script.
// Abstractly a PrivateScriptData consists of all these arrays:
//
//   * A non-empty array of GCPtrScope in scopes()
//   * A possibly-empty array of GCPtrValue in consts()
//   * A possibly-empty array of JSObject* in objects()
//   * A possibly-empty array of JSTryNote in tryNotes()
//   * A possibly-empty array of ScopeNote in scopeNotes()
//   * A possibly-empty array of uint32_t in resumeOffsets()
//
// Accessing any of these arrays just requires calling the appropriate public
// Span-computing function.
//
// Under the hood, PrivateScriptData is a small class followed by a memory
// layout that compactly encodes all these arrays, in this manner (only
// explicit padding, "--" separators for readability only):
//
//   <PrivateScriptData itself>
//   --
//   (OPTIONAL) PackedSpan for consts()
//   (OPTIONAL) PackedSpan for objects()
//   (OPTIONAL) PackedSpan for tryNotes()
//   (OPTIONAL) PackedSpan for scopeNotes()
//   (OPTIONAL) PackedSpan for resumeOffsets()
//   --
//   (REQUIRED) All the GCPtrScopes that constitute scopes()
//   --
//   (OPTIONAL) If there are consts, padding needed for space so far to be
//              GCPtrValue-aligned
//   (OPTIONAL) All the GCPtrValues that constitute consts()
//   --
//   (OPTIONAL) All the GCPtrObjects that constitute objects()
//   --
//   (OPTIONAL) All the JSTryNotes that constitute tryNotes()
//   --
//   (OPTIONAL) All the ScopeNotes that constitute scopeNotes()
//   --
//   (OPTIONAL) All the uint32_t's that constitute resumeOffsets()
//
// The contents of PrivateScriptData indicate which optional items are present.
// PrivateScriptData::packedOffsets contains bit-fields, one per array.
// Multiply each packed offset by sizeof(uint32_t) to compute a *real* offset.
//
// PrivateScriptData::scopesOffset indicates where scopes() begins. The bound
// of five PackedSpans ensures we can encode this offset compactly.
// PrivateScriptData::nscopes indicates the number of GCPtrScopes in scopes().
//
// The other PackedScriptData::*Offset fields indicate where a potential
// corresponding PackedSpan resides. If the packed offset is 0, there is no
// PackedSpan, and the array is empty. Otherwise the PackedSpan's uint32_t
// offset and length fields store: 1) a *non-packed* offset (a literal count of
// bytes offset from the *start* of PrivateScriptData struct) to the
// corresponding array, and 2) the number of elements in the array,
// respectively.
//
// PrivateScriptData and PackedSpan are 64-bit-aligned, so manual alignment in
// trailing fields is only necessary before the first trailing fields with
// increased alignment -- before GCPtrValues for consts(), on 32-bit, where the
// preceding GCPtrScopes as pointers are only 32-bit-aligned.
class alignas(JS::Value) PrivateScriptData final {
  struct PackedOffsets {
    static constexpr size_t SCALE = sizeof(uint32_t);
    static constexpr size_t MAX_OFFSET = 0b1111;

    // (Scaled) offset to Scopes
    uint32_t scopesOffset : 8;

    // (Scaled) offset to Spans. These are set to 0 if they don't exist.
    uint32_t constsSpanOffset : 4;
    uint32_t objectsSpanOffset : 4;
    uint32_t tryNotesSpanOffset : 4;
    uint32_t scopeNotesSpanOffset : 4;
    uint32_t resumeOffsetsSpanOffset : 4;
  };

  // Detect accidental size regressions.
  static_assert(sizeof(PackedOffsets) == sizeof(uint32_t),
                "unexpected bit-field packing");

  // A span describes base offset and length of one variable length array in
  // the private data.
  struct alignas(uintptr_t) PackedSpan {
    uint32_t offset;
    uint32_t length;
  };

  // Concrete Fields
  PackedOffsets packedOffsets = {};  // zeroes
  uint32_t nscopes;

  // Translate an offset into a concrete pointer.
  template <typename T>
  T* offsetToPointer(size_t offset) {
    uintptr_t base = reinterpret_cast<uintptr_t>(this);
    uintptr_t elem = base + offset;
    return reinterpret_cast<T*>(elem);
  }

  // Translate a PackedOffsets member into a pointer.
  template <typename T>
  T* packedOffsetToPointer(size_t packedOffset) {
    return offsetToPointer<T>(packedOffset * PackedOffsets::SCALE);
  }

  // Translates a PackedOffsets member into a PackedSpan* and then unpacks
  // that to a mozilla::Span.
  template <typename T>
  mozilla::Span<T> packedOffsetToSpan(size_t scaledSpanOffset) {
    PackedSpan* span = packedOffsetToPointer<PackedSpan>(scaledSpanOffset);
    T* base = offsetToPointer<T>(span->offset);
    return mozilla::MakeSpan(base, span->length);
  }

  // Helpers for creating initializing trailing data
  template <typename T>
  void initSpan(size_t* cursor, uint32_t scaledSpanOffset, size_t length);

  template <typename T>
  void initElements(size_t offset, size_t length);

  // Size to allocate
  static size_t AllocationSize(uint32_t nscopes, uint32_t nconsts,
                               uint32_t nobjects, uint32_t ntrynotes,
                               uint32_t nscopenotes, uint32_t nresumeoffsets);

  // Initialize header and PackedSpans
  PrivateScriptData(uint32_t nscopes_, uint32_t nconsts, uint32_t nobjects,
                    uint32_t ntrynotes, uint32_t nscopenotes,
                    uint32_t nresumeoffsets);

 public:
  // Accessors for typed array spans.
  mozilla::Span<GCPtrScope> scopes() {
    GCPtrScope* base =
        packedOffsetToPointer<GCPtrScope>(packedOffsets.scopesOffset);
    return mozilla::MakeSpan(base, nscopes);
  }
  mozilla::Span<GCPtrValue> consts() {
    return packedOffsetToSpan<GCPtrValue>(packedOffsets.constsSpanOffset);
  }
  mozilla::Span<GCPtrObject> objects() {
    return packedOffsetToSpan<GCPtrObject>(packedOffsets.objectsSpanOffset);
  }
  mozilla::Span<JSTryNote> tryNotes() {
    return packedOffsetToSpan<JSTryNote>(packedOffsets.tryNotesSpanOffset);
  }
  mozilla::Span<ScopeNote> scopeNotes() {
    return packedOffsetToSpan<ScopeNote>(packedOffsets.scopeNotesSpanOffset);
  }
  mozilla::Span<uint32_t> resumeOffsets() {
    return packedOffsetToSpan<uint32_t>(packedOffsets.resumeOffsetsSpanOffset);
  }

  // Fast tests for if array exists
  bool hasConsts() const { return packedOffsets.constsSpanOffset != 0; }
  bool hasObjects() const { return packedOffsets.objectsSpanOffset != 0; }
  bool hasTryNotes() const { return packedOffsets.tryNotesSpanOffset != 0; }
  bool hasScopeNotes() const { return packedOffsets.scopeNotesSpanOffset != 0; }
  bool hasResumeOffsets() const {
    return packedOffsets.resumeOffsetsSpanOffset != 0;
  }

  // Allocate a new PrivateScriptData. Headers and GCPtrs are initialized.
  // The size of allocation is returned as an out parameter.
  static PrivateScriptData* new_(JSContext* cx, uint32_t nscopes,
                                 uint32_t nconsts, uint32_t nobjects,
                                 uint32_t ntrynotes, uint32_t nscopenotes,
                                 uint32_t nresumeoffsets, uint32_t* dataSize);

  void traceChildren(JSTracer* trc);
};
||||||| merged common ancestors
/*
 * Code any constant value.
 */
template<XDRMode mode>
XDRResult
XDRScriptConst(XDRState<mode>* xdr, MutableHandleValue vp);
=======
template <XDRMode mode>
XDRResult XDRLazyScript(XDRState<mode>* xdr, HandleScope enclosingScope,
                        HandleScriptSourceObject sourceObject,
                        HandleFunction fun, MutableHandle<LazyScript*> lazy);
>>>>>>> upstream-releases

/*
 * Code any constant value.
 */
<<<<<<< HEAD
class SharedScriptData {
  // This class is reference counted as follows: each pointer from a JSScript
  // counts as one reference plus there may be one reference from the shared
  // script data table.
  mozilla::Atomic<uint32_t, mozilla::SequentiallyConsistent,
                  mozilla::recordreplay::Behavior::DontPreserve>
      refCount_;

  uint32_t natoms_;
  uint32_t codeLength_;
  uint32_t noteLength_;
  uintptr_t data_[1];

 public:
  static SharedScriptData* new_(JSContext* cx, uint32_t codeLength,
                                uint32_t srcnotesLength, uint32_t natoms);

  uint32_t refCount() const { return refCount_; }
  void incRefCount() { refCount_++; }
  void decRefCount() {
    MOZ_ASSERT(refCount_ != 0);
    uint32_t remain = --refCount_;
    if (remain == 0) {
      js_free(this);
    }
  }

  size_t dataLength() const {
    return (natoms_ * sizeof(GCPtrAtom)) + codeLength_ + noteLength_;
  }
  const uint8_t* data() const {
    return reinterpret_cast<const uint8_t*>(data_);
  }
  uint8_t* data() { return reinterpret_cast<uint8_t*>(data_); }

  uint32_t natoms() const { return natoms_; }
  GCPtrAtom* atoms() {
    if (!natoms_) {
      return nullptr;
    }
    return reinterpret_cast<GCPtrAtom*>(data());
  }

  uint32_t codeLength() const { return codeLength_; }
  jsbytecode* code() {
    return reinterpret_cast<jsbytecode*>(data() + natoms_ * sizeof(GCPtrAtom));
  }

  uint32_t numNotes() const { return noteLength_; }
  jssrcnote* notes() {
    return reinterpret_cast<jssrcnote*>(data() + natoms_ * sizeof(GCPtrAtom) +
                                        codeLength_);
  }

  void traceChildren(JSTracer* trc);

 private:
  SharedScriptData() = delete;
  SharedScriptData(const SharedScriptData&) = delete;
  SharedScriptData& operator=(const SharedScriptData&) = delete;
||||||| merged common ancestors
class SharedScriptData
{
    // This class is reference counted as follows: each pointer from a JSScript
    // counts as one reference plus there may be one reference from the shared
    // script data table.
    mozilla::Atomic<uint32_t, mozilla::SequentiallyConsistent,
                    mozilla::recordreplay::Behavior::DontPreserve> refCount_;

    uint32_t natoms_;
    uint32_t codeLength_;
    uint32_t noteLength_;
    uintptr_t data_[1];

  public:
    static SharedScriptData* new_(JSContext* cx, uint32_t codeLength,
                                  uint32_t srcnotesLength, uint32_t natoms);

    uint32_t refCount() const {
        return refCount_;
    }
    void incRefCount() {
        refCount_++;
    }
    void decRefCount() {
        MOZ_ASSERT(refCount_ != 0);
        uint32_t remain = --refCount_;
        if (remain == 0) {
            js_free(this);
        }
    }

    size_t dataLength() const {
        return (natoms_ * sizeof(GCPtrAtom)) + codeLength_ + noteLength_;
    }
    const uint8_t* data() const {
        return reinterpret_cast<const uint8_t*>(data_);
    }
    uint8_t* data() {
        return reinterpret_cast<uint8_t*>(data_);
    }

    uint32_t natoms() const {
        return natoms_;
    }
    GCPtrAtom* atoms() {
        if (!natoms_) {
            return nullptr;
        }
        return reinterpret_cast<GCPtrAtom*>(data());
    }

    uint32_t codeLength() const {
        return codeLength_;
    }
    jsbytecode* code() {
        return reinterpret_cast<jsbytecode*>(data() + natoms_ * sizeof(GCPtrAtom));
    }

    uint32_t numNotes() const {
        return noteLength_;
    }
    jssrcnote* notes() {
        return reinterpret_cast<jssrcnote*>(data() + natoms_ * sizeof(GCPtrAtom) + codeLength_);
    }

    void traceChildren(JSTracer* trc);

  private:
    SharedScriptData() = delete;
    SharedScriptData(const SharedScriptData&) = delete;
    SharedScriptData& operator=(const SharedScriptData&) = delete;
=======
template <XDRMode mode>
XDRResult XDRScriptConst(XDRState<mode>* xdr, MutableHandleValue vp);

// [SMDOC] - JSScript data layout (unshared)
//
// PrivateScriptData stores variable-length data associated with a script.
// Abstractly a PrivateScriptData consists of all these arrays:
//
//   * A non-empty array of GCCellPtr in gcthings()
//
// Accessing this array just requires calling the appropriate public
// Span-computing function.
class alignas(uintptr_t) PrivateScriptData final {
  uint32_t ngcthings = 0;

  js::FieldInitializers fieldInitializers_ = js::FieldInitializers::Invalid();

  // Translate an offset into a concrete pointer.
  template <typename T>
  T* offsetToPointer(size_t offset) {
    uintptr_t base = reinterpret_cast<uintptr_t>(this);
    uintptr_t elem = base + offset;
    return reinterpret_cast<T*>(elem);
  }

  // Helpers for creating initializing trailing data
  template <typename T>
  void initElements(size_t offset, size_t length);

  // Size to allocate
  static size_t AllocationSize(uint32_t ngcthings);

  // Initialize header and PackedSpans
  explicit PrivateScriptData(uint32_t ngcthings);

 public:
  static constexpr size_t offsetOfGCThings() {
    return sizeof(PrivateScriptData);
  }

  // Accessors for typed array spans.
  mozilla::Span<JS::GCCellPtr> gcthings() {
    size_t offset = offsetOfGCThings();
    return mozilla::MakeSpan(offsetToPointer<JS::GCCellPtr>(offset), ngcthings);
  }

  void setFieldInitializers(FieldInitializers fieldInitializers) {
    fieldInitializers_ = fieldInitializers;
  }
  const FieldInitializers& getFieldInitializers() { return fieldInitializers_; }

  // Allocate a new PrivateScriptData. Headers and GCPtrs are initialized.
  // The size of allocation is returned as an out parameter.
  static PrivateScriptData* new_(JSContext* cx, uint32_t ngcthings,
                                 uint32_t* dataSize);

  template <XDRMode mode>
  static MOZ_MUST_USE XDRResult XDR(js::XDRState<mode>* xdr,
                                    js::HandleScript script,
                                    js::HandleScriptSourceObject sourceObject,
                                    js::HandleScope scriptEnclosingScope,
                                    js::HandleFunction fun);

  // Clone src script data into dst script.
  static bool Clone(JSContext* cx, js::HandleScript src, js::HandleScript dst,
                    js::MutableHandle<JS::GCVector<js::Scope*>> scopes);

  static bool InitFromEmitter(JSContext* cx, js::HandleScript script,
                              js::frontend::BytecodeEmitter* bce);

  void trace(JSTracer* trc);

  // PrivateScriptData has trailing data so isn't copyable or movable.
  PrivateScriptData(const PrivateScriptData&) = delete;
  PrivateScriptData& operator=(const PrivateScriptData&) = delete;
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ScriptBytecodeHasher {
  class Lookup {
    friend struct ScriptBytecodeHasher;
||||||| merged common ancestors
struct ScriptBytecodeHasher
{
    class Lookup {
        friend struct ScriptBytecodeHasher;
=======
// [SMDOC] JSScript data layout (shared)
//
// SharedScriptData stores variable-length script data that may be shared
// between scripts with the same bytecode, even across different GC Zones.
// Abstractly this structure consists of multiple (optional) arrays that are
// exposed as mozilla::Span<T>. These arrays exist in a single heap allocation.
//
// Under the hood, SharedScriptData is a fixed-size header class followed the
// various array bodies interleaved with metadata to compactly encode the
// bounds. These arrays have varying requirements for alignment, performance,
// and jit-friendliness which leads to the complex indexing system below.
//
// Note: The '----' separators are for readability only.
//
// ----
//   <SharedScriptData itself>
// ----
//   (OPTIONAL) Array of GCPtrAtom constituting atoms()
// ----
//   (REQUIRED) Flags structure
//  codeOffset:
//   (REQUIRED) Array of jsbytecode constituting code()
//   (REQUIRED) Array of jssrcnote constituting notes()
// ----
//   (OPTIONAL) Array of uint32_t optional-offsets
//  optArrayOffset:
// ----
//  L0:
//   (OPTIONAL) Array of uint32_t constituting resumeOffsets()
//  L1:
//   (OPTIONAL) Array of ScopeNote constituting scopeNotes()
//  L2:
//   (OPTIONAL) Array of JSTryNote constituting tryNotes()
//  L3:
// ----
//
// NOTE: The notes() array must have been null-padded such that
//       flags/code/notes together have uint32_t alignment.
//
// The 'codeOffset' and 'optArrayOffset' labels shown have their byte-offset
// relative to 'this' stored as fields in SharedScriptData. They form the basis
// of the indexing system.
//
// The L0/L1/L2/L3 labels indicate the start and end of the optional arrays.
// Some of these labels may refer to the same location if the array between
// them is empty. Each unique label position has an offset stored in the
// optional-offsets table. Note that we also avoid entries for labels that
// match 'optArrayOffset'. This is done to save memory when arrays are empty.
//
// The flags() data indicates (for each optional array) which entry from the
// optional-offsets table marks the *end* of array. The array starts where the
// previous array ends and the first array begins at 'optArrayOffset'. The
// optional-offset table is addressed at negative indices from
// 'optArrayOffset'.
//
// In general, the length of each array is computed from subtracting the start
// offset of the array from the start offset of the subsequent array. The
// notable exception is that bytecode length is stored explicitly.
class alignas(uintptr_t) SharedScriptData final {
  // This class is reference counted as follows: each pointer from a JSScript
  // counts as one reference plus there may be one reference from the shared
  // script data table.
  mozilla::Atomic<uint32_t, mozilla::SequentiallyConsistent,
                  mozilla::recordreplay::Behavior::DontPreserve>
      refCount_ = {};

  uint32_t codeOffset_ = 0;  // Byte-offset from 'this'
  uint32_t codeLength_ = 0;
  uint32_t optArrayOffset_ = 0;

  // Offset of main entry point from code, after predef'ing prologue.
  uint32_t mainOffset = 0;

  // Fixed frame slots.
  uint32_t nfixed = 0;

  // Slots plus maximum stack depth.
  uint32_t nslots = 0;

  // Index into the gcthings array of the body scope.
  uint32_t bodyScopeIndex = 0;

  // Number of IC entries to allocate in JitScript for Baseline ICs.
  uint32_t numICEntries = 0;

  // ES6 function length.
  uint16_t funLength = 0;

  // Number of type sets used in this script for dynamic type monitoring.
  uint16_t numBytecodeTypeSets = 0;

  // NOTE: The raw bytes of this structure are used for hashing so use explicit
  // padding values as needed for predicatable results across compilers.

  struct Flags {
    uint8_t resumeOffsetsEndIndex : 2;
    uint8_t scopeNotesEndIndex : 2;
    uint8_t tryNotesEndIndex : 2;
    uint8_t _unused : 2;
  };
  static_assert(sizeof(Flags) == sizeof(uint8_t),
                "Structure packing is broken");

  friend class ::JSScript;

 private:
  // Offsets (in bytes) from 'this' to each component array. The delta between
  // each offset and the next offset is the size of each array and is defined
  // even if an array is empty.
  size_t atomOffset() const { return offsetOfAtoms(); }
  size_t flagOffset() const { return codeOffset_ - sizeof(Flags); }
  size_t codeOffset() const { return codeOffset_; }
  size_t noteOffset() const { return codeOffset_ + codeLength_; }
  size_t optionalOffsetsOffset() const {
    // Determine the location to beginning of optional-offsets array by looking
    // at index for try-notes.
    //
    //   optionalOffsetsOffset():
    //     (OPTIONAL) tryNotesEndOffset
    //     (OPTIONAL) scopeNotesEndOffset
    //     (OPTIONAL) resumeOffsetsEndOffset
    //   optArrayOffset_:
    //     ....
    unsigned numOffsets = flags().tryNotesEndIndex;
    MOZ_ASSERT(numOffsets >= flags().scopeNotesEndIndex);
    MOZ_ASSERT(numOffsets >= flags().resumeOffsetsEndIndex);

    return optArrayOffset_ - (numOffsets * sizeof(uint32_t));
  }
  size_t resumeOffsetsOffset() const { return optArrayOffset_; }
  size_t scopeNotesOffset() const {
    return getOptionalOffset(flags().resumeOffsetsEndIndex);
  }
  size_t tryNotesOffset() const {
    return getOptionalOffset(flags().scopeNotesEndIndex);
  }
  size_t endOffset() const {
    return getOptionalOffset(flags().tryNotesEndIndex);
  }

  // Size to allocate
  static size_t AllocationSize(uint32_t codeLength, uint32_t noteLength,
                               uint32_t natoms, uint32_t numResumeOffsets,
                               uint32_t numScopeNotes, uint32_t numTryNotes);

  // Translate an offset into a concrete pointer.
  template <typename T>
  T* offsetToPointer(size_t offset) {
    uintptr_t base = reinterpret_cast<uintptr_t>(this);
    return reinterpret_cast<T*>(base + offset);
  }

  template <typename T>
  void initElements(size_t offset, size_t length);

  void initOptionalArrays(size_t* cursor, Flags* flags,
                          uint32_t numResumeOffsets, uint32_t numScopeNotes,
                          uint32_t numTryNotes);

  // Initialize to GC-safe state
  SharedScriptData(uint32_t codeLength, uint32_t noteLength, uint32_t natoms,
                   uint32_t numResumeOffsets, uint32_t numScopeNotes,
                   uint32_t numTryNotes);

  void setOptionalOffset(int index, uint32_t offset) {
    MOZ_ASSERT((index > 0) && (offset != optArrayOffset_),
               "Implicit offset should not be stored");
    offsetToPointer<uint32_t>(optArrayOffset_)[-index] = offset;
  }
  uint32_t getOptionalOffset(int index) const {
    // The index 0 represents (implicitly) the offset 'optArrayOffset_'.
    if (index == 0) {
      return optArrayOffset_;
    }

    SharedScriptData* this_ = const_cast<SharedScriptData*>(this);
    return this_->offsetToPointer<uint32_t>(optArrayOffset_)[-index];
  }

 public:
  static SharedScriptData* new_(JSContext* cx, uint32_t codeLength,
                                uint32_t noteLength, uint32_t natoms,
                                uint32_t numResumeOffsets,
                                uint32_t numScopeNotes, uint32_t numTryNotes);

  // The code() and note() arrays together maintain an target alignment by
  // padding the source notes with null. This allows arrays with stricter
  // alignment requirements to follow them.
  static constexpr size_t CodeNoteAlign = sizeof(uint32_t);

  // Compute number of null notes to pad out source notes with.
  static uint32_t ComputeNotePadding(uint32_t codeLength, uint32_t noteLength) {
    uint32_t flagLength = sizeof(Flags);
    uint32_t nullLength =
        CodeNoteAlign - (flagLength + codeLength + noteLength) % CodeNoteAlign;

    // The source notes must have at least one null-terminator.
    MOZ_ASSERT(nullLength >= 1);

    return nullLength;
  }

  uint32_t refCount() const { return refCount_; }
  void AddRef() { refCount_++; }
  void Release() {
    MOZ_ASSERT(refCount_ != 0);
    uint32_t remain = --refCount_;
    if (remain == 0) {
      js_free(this);
    }
  }

  // Span over all raw bytes in this struct and its trailing arrays.
  mozilla::Span<const uint8_t> allocSpan() const {
    size_t allocSize = endOffset();
    return mozilla::MakeSpan(reinterpret_cast<const uint8_t*>(this), allocSize);
  }

  // Span over all immutable bytes in allocation. This excludes part of
  // structure used for reference counting and is the basis of how we
  // de-duplicate data.
  mozilla::Span<const uint8_t> immutableData() const {
    // The refCount_ must be first field of structure.
    static_assert(offsetof(SharedScriptData, refCount_) == 0,
                  "refCount_ must be at start of SharedScriptData");
    constexpr size_t dataOffset = sizeof(refCount_);

    static_assert(offsetof(SharedScriptData, numBytecodeTypeSets) +
                          sizeof(numBytecodeTypeSets) ==
                      sizeof(SharedScriptData),
                  "SharedScriptData should not have padding after last field");

    return allocSpan().From(dataOffset);
  }

  uint32_t natoms() const {
    return (flagOffset() - atomOffset()) / sizeof(GCPtrAtom);
  }
  GCPtrAtom* atoms() { return offsetToPointer<GCPtrAtom>(atomOffset()); }

  Flags& flagsRef() { return *offsetToPointer<Flags>(flagOffset()); }
  const Flags& flags() const {
    return const_cast<SharedScriptData*>(this)->flagsRef();
  }

  uint32_t codeLength() const { return codeLength_; }
  jsbytecode* code() { return offsetToPointer<jsbytecode>(codeOffset()); }

  uint32_t noteLength() const { return optionalOffsetsOffset() - noteOffset(); }
  jssrcnote* notes() { return offsetToPointer<jssrcnote>(noteOffset()); }

  mozilla::Span<uint32_t> resumeOffsets() {
    return mozilla::MakeSpan(offsetToPointer<uint32_t>(resumeOffsetsOffset()),
                             offsetToPointer<uint32_t>(scopeNotesOffset()));
  }
  mozilla::Span<ScopeNote> scopeNotes() {
    return mozilla::MakeSpan(offsetToPointer<ScopeNote>(scopeNotesOffset()),
                             offsetToPointer<ScopeNote>(tryNotesOffset()));
  }
  mozilla::Span<JSTryNote> tryNotes() {
    return mozilla::MakeSpan(offsetToPointer<JSTryNote>(tryNotesOffset()),
                             offsetToPointer<JSTryNote>(endOffset()));
  }

  static constexpr size_t offsetOfCodeOffset() {
    return offsetof(SharedScriptData, codeOffset_);
  }
  static constexpr size_t offsetOfResumeOffsetsOffset() {
    // Resume-offsets are the first optional array if they exist. Locate the
    // array with the 'optArrayOffset_' field.
    return offsetof(SharedScriptData, optArrayOffset_);
  }
  static constexpr size_t offsetOfNfixed() {
    return offsetof(SharedScriptData, nfixed);
  }
  static constexpr size_t offsetOfNslots() {
    return offsetof(SharedScriptData, nslots);
  }
  static constexpr size_t offsetOfFunLength() {
    return offsetof(SharedScriptData, funLength);
  }
  static constexpr size_t offsetOfAtoms() { return sizeof(SharedScriptData); }

  void traceChildren(JSTracer* trc);

  template <XDRMode mode>
  static MOZ_MUST_USE XDRResult XDR(js::XDRState<mode>* xdr,
                                    js::HandleScript script);

  static bool InitFromEmitter(JSContext* cx, js::HandleScript script,
                              js::frontend::BytecodeEmitter* bce,
                              uint32_t nslots);

  // Mark this SharedScriptData for use in a new zone
  void markForCrossZone(JSContext* cx);

  // SharedScriptData has trailing data so isn't copyable or movable.
  SharedScriptData(const SharedScriptData&) = delete;
  SharedScriptData& operator=(const SharedScriptData&) = delete;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
    SharedScriptData* scriptData;
    HashNumber hash;
||||||| merged common ancestors
        SharedScriptData* scriptData;
        HashNumber hash;
=======
// Two SharedScriptData instances may be de-duplicated if they have the same
// data in their immutableData() span. This Hasher enables that comparison.
struct SharedScriptDataHasher {
  using Lookup = RefPtr<SharedScriptData>;
>>>>>>> upstream-releases

<<<<<<< HEAD
   public:
    explicit Lookup(SharedScriptData* data);
    ~Lookup();
  };
||||||| merged common ancestors
      public:
        explicit Lookup(SharedScriptData* data);
        ~Lookup();
    };
=======
  static HashNumber hash(const Lookup& l) {
    mozilla::Span<const uint8_t> immutableData = l->immutableData();
    return mozilla::HashBytes(immutableData.data(), immutableData.size());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static HashNumber hash(const Lookup& l) { return l.hash; }
  static bool match(SharedScriptData* entry, const Lookup& lookup) {
    const SharedScriptData* data = lookup.scriptData;
    if (entry->natoms() != data->natoms()) {
      return false;
    }
    if (entry->codeLength() != data->codeLength()) {
      return false;
    }
    if (entry->numNotes() != data->numNotes()) {
      return false;
    }
    return mozilla::ArrayEqual<uint8_t>(entry->data(), data->data(),
                                        data->dataLength());
  }
||||||| merged common ancestors
    static HashNumber hash(const Lookup& l) {
        return l.hash;
    }
    static bool match(SharedScriptData* entry, const Lookup& lookup) {
        const SharedScriptData* data = lookup.scriptData;
        if (entry->natoms() != data->natoms()) {
            return false;
        }
        if (entry->codeLength() != data->codeLength()) {
            return false;
        }
        if (entry->numNotes() != data->numNotes()) {
            return false;
        }
        return mozilla::ArrayEqual<uint8_t>(entry->data(), data->data(), data->dataLength());
    }
=======
  static bool match(SharedScriptData* entry, const Lookup& lookup) {
    return entry->immutableData() == lookup->immutableData();
  }
>>>>>>> upstream-releases
};

class AutoLockScriptData;

<<<<<<< HEAD
using ScriptDataTable =
    HashSet<SharedScriptData*, ScriptBytecodeHasher, SystemAllocPolicy>;
||||||| merged common ancestors
using ScriptDataTable = HashSet<SharedScriptData*,
                                ScriptBytecodeHasher,
                                SystemAllocPolicy>;
=======
using ScriptDataTable =
    HashSet<SharedScriptData*, SharedScriptDataHasher, SystemAllocPolicy>;
>>>>>>> upstream-releases

extern void SweepScriptData(JSRuntime* rt);

extern void FreeScriptData(JSRuntime* rt);

} /* namespace js */

<<<<<<< HEAD
class JSScript : public js::gc::TenuredCell {
 private:
  // Pointer to baseline->method()->raw(), ion->method()->raw(), a wasm jit
  // entry, the JIT's EnterInterpreter stub, or the lazy link stub. Must be
  // non-null.
  uint8_t* jitCodeRaw_ = nullptr;
  uint8_t* jitCodeSkipArgCheck_ = nullptr;

  // Shareable script data
  js::SharedScriptData* scriptData_ = nullptr;

  // Unshared variable-length data
  js::PrivateScriptData* data_ = nullptr;

 public:
  JS::Realm* realm_ = nullptr;

 private:
  /* Persistent type information retained across GCs. */
  js::TypeScript* types_ = nullptr;

  // This script's ScriptSourceObject, or a CCW thereof.
  //
  // (When we clone a JSScript into a new compartment, we don't clone its
  // source object. Instead, the clone refers to a wrapper.)
  js::GCPtrObject sourceObject_ = {};

  /*
   * Information attached by Ion. Nexto a valid IonScript this could be
   * ION_DISABLED_SCRIPT, ION_COMPILING_SCRIPT or ION_PENDING_SCRIPT.
   * The later is a ion compilation that is ready, but hasn't been linked
   * yet.
   */
  js::jit::IonScript* ion = nullptr;

  /* Information attached by Baseline. */
  js::jit::BaselineScript* baseline = nullptr;

  /* Information used to re-lazify a lazily-parsed interpreted function. */
  js::LazyScript* lazyScript = nullptr;

  // 32-bit fields.

  /* Size of the used part of the data array. */
  uint32_t dataSize_ = 0;

  /* Base line number of script. */
  uint32_t lineno_ = 0;

  /* Base column of script, optionally set. */
  uint32_t column_ = 0;

  /* Offset of main entry point from code, after predef'ing prologue. */
  uint32_t mainOffset_ = 0;

  /* Fixed frame slots. */
  uint32_t nfixed_ = 0;

  /* Slots plus maximum stack depth. */
  uint32_t nslots_ = 0;

  /* Index into the scopes array of the body scope */
  uint32_t bodyScopeIndex_ = 0;

  // Range of characters in scriptSource which contains this script's
  // source, that is, the range used by the Parser to produce this script.
  //
  // Most scripted functions have sourceStart_ == toStringStart_ and
  // sourceEnd_ == toStringEnd_. However, for functions with extra
  // qualifiers (e.g. generators, async) and for class constructors (which
  // need to return the entire class source), their values differ.
  //
  // Each field points the following locations.
  //
  //   function * f(a, b) { return a + b; }
  //   ^          ^                        ^
  //   |          |                        |
  //   |          sourceStart_             sourceEnd_
  //   |                                   |
  //   toStringStart_                      toStringEnd_
  //
  // And, in the case of class constructors, an additional toStringEnd
  // offset is used.
  //
  //   class C { constructor() { this.field = 42; } }
  //   ^         ^                                 ^ ^
  //   |         |                                 | `---------`
  //   |         sourceStart_                      sourceEnd_  |
  //   |                                                       |
  //   toStringStart_                                          toStringEnd_
  uint32_t sourceStart_ = 0;
  uint32_t sourceEnd_ = 0;
  uint32_t toStringStart_ = 0;
  uint32_t toStringEnd_ = 0;

  // Number of times the script has been called or has had backedges taken.
  // When running in ion, also increased for any inlined scripts. Reset if
  // the script's JIT code is forcibly discarded.
  mozilla::Atomic<uint32_t, mozilla::Relaxed,
                  mozilla::recordreplay::Behavior::DontPreserve>
      warmUpCount = {};

  // Immutable flags should not be modified after this script has been
  // initialized. These flags should likely be preserved when serializing
  // (XDR) or copying (CopyScript) this script.
  enum class ImmutableFlags : uint32_t {
    // No need for result value of last expression statement.
    NoScriptRval = 1 << 0,

    // Code is in strict mode.
    Strict = 1 << 1,

    // Code has "use strict"; explicitly.
    ExplicitUseStrict = 1 << 2,

    // True if the script has a non-syntactic scope on its dynamic scope chain.
    // That is, there are objects about which we know nothing between the
    // outermost syntactic scope and the global.
    HasNonSyntacticScope = 1 << 3,

    // See Parser::selfHostingMode.
    SelfHosted = 1 << 4,

    // See FunctionBox.
    BindingsAccessedDynamically = 1 << 5,
    FunHasExtensibleScope = 1 << 6,

    // True if any formalIsAliased(i).
    FunHasAnyAliasedFormal = 1 << 7,

    // Script has singleton objects.
    HasSingletons = 1 << 8,

    FunctionHasThisBinding = 1 << 9,
    FunctionHasExtraBodyVarScope = 1 << 10,

    // Whether the arguments object for this script, if it needs one, should be
    // mapped (alias formal parameters).
    HasMappedArgsObj = 1 << 11,

    // Script contains inner functions. Used to check if we can relazify the
    // script.
    HasInnerFunctions = 1 << 12,

    NeedsHomeObject = 1 << 13,

    IsDerivedClassConstructor = 1 << 14,
    IsDefaultClassConstructor = 1 << 15,

    // Script is a lambda to treat as running once or a global or eval script
    // that will only run once.  Which one it is can be disambiguated by
    // checking whether function() is null.
    TreatAsRunOnce = 1 << 16,

    // 'this', 'arguments' and f.apply() are used. This is likely to be a
    // wrapper.
    IsLikelyConstructorWrapper = 1 << 17,

    // Set if this function is a generator function or async generator.
    IsGenerator = 1 << 18,

    // Set if this function is an async function or async generator.
    IsAsync = 1 << 19,

    // Set if this function has a rest parameter.
    HasRest = 1 << 20,

    // See comments below.
    ArgsHasVarBinding = 1 << 21,
  };
  // Note: don't make this a bitfield! It makes it hard to read these flags
  // from JIT code.
  uint32_t immutableFlags_ = 0;

  // Mutable flags typically store information about runtime or deoptimization
  // behavior of this script. This is only public for the JITs.
 public:
  enum class MutableFlags : uint32_t {
    // Have warned about uses of undefined properties in this script.
    WarnedAboutUndefinedProp = 1 << 0,

    // If treatAsRunOnce, whether script has executed.
    HasRunOnce = 1 << 1,

    // Script has been reused for a clone.
    HasBeenCloned = 1 << 2,
||||||| merged common ancestors
class JSScript : public js::gc::TenuredCell
{
  private:
    // Pointer to baseline->method()->raw(), ion->method()->raw(), a wasm jit
    // entry, the JIT's EnterInterpreter stub, or the lazy link stub. Must be
    // non-null.
    uint8_t* jitCodeRaw_ = nullptr;
    uint8_t* jitCodeSkipArgCheck_ = nullptr;

    js::SharedScriptData* scriptData_ = nullptr;

  public:
    // Pointer to variable-length data array (see comment above Create() for
    // details).
    uint8_t* data = nullptr;

    JS::Realm* realm_ = nullptr;

  private:
    /* Persistent type information retained across GCs. */
    js::TypeScript* types_ = nullptr;

    // This script's ScriptSourceObject, or a CCW thereof.
    //
    // (When we clone a JSScript into a new compartment, we don't clone its
    // source object. Instead, the clone refers to a wrapper.)
    js::GCPtrObject sourceObject_ = {};

    /*
     * Information attached by Ion. Nexto a valid IonScript this could be
     * ION_DISABLED_SCRIPT, ION_COMPILING_SCRIPT or ION_PENDING_SCRIPT.
     * The later is a ion compilation that is ready, but hasn't been linked
     * yet.
     */
    js::jit::IonScript* ion = nullptr;

    /* Information attached by Baseline. */
    js::jit::BaselineScript* baseline = nullptr;

    /* Information used to re-lazify a lazily-parsed interpreted function. */
    js::LazyScript* lazyScript = nullptr;

    // 32-bit fields.

    /* Size of the used part of the data array. */
    uint32_t dataSize_ = 0;

    /* Base line number of script. */
    uint32_t lineno_ = 0;

    /* Base column of script, optionally set. */
    uint32_t column_ = 0;

    /* Offset of main entry point from code, after predef'ing prologue. */
    uint32_t mainOffset_ = 0;

    /* Fixed frame slots. */
    uint32_t nfixed_ = 0;

    /* Slots plus maximum stack depth. */
    uint32_t nslots_ = 0;

    /* Index into the scopes array of the body scope */
    uint32_t bodyScopeIndex_ = 0;

    // Range of characters in scriptSource which contains this script's
    // source, that is, the range used by the Parser to produce this script.
    //
    // Most scripted functions have sourceStart_ == toStringStart_ and
    // sourceEnd_ == toStringEnd_. However, for functions with extra
    // qualifiers (e.g. generators, async) and for class constructors (which
    // need to return the entire class source), their values differ.
    //
    // Each field points the following locations.
    //
    //   function * f(a, b) { return a + b; }
    //   ^          ^                        ^
    //   |          |                        |
    //   |          sourceStart_             sourceEnd_
    //   |                                   |
    //   toStringStart_                      toStringEnd_
    //
    // And, in the case of class constructors, an additional toStringEnd
    // offset is used.
    //
    //   class C { constructor() { this.field = 42; } }
    //   ^         ^                                 ^ ^
    //   |         |                                 | `---------`
    //   |         sourceStart_                      sourceEnd_  |
    //   |                                                       |
    //   toStringStart_                                          toStringEnd_
    uint32_t sourceStart_ = 0;
    uint32_t sourceEnd_ = 0;
    uint32_t toStringStart_ = 0;
    uint32_t toStringEnd_ = 0;

#ifdef MOZ_VTUNE
    // Unique Method ID passed to the VTune profiler, or 0 if unset.
    // Allows attribution of different jitcode to the same source script.
    uint32_t vtuneMethodId_ = 0;
#endif

    // Number of times the script has been called or has had backedges taken.
    // When running in ion, also increased for any inlined scripts. Reset if
    // the script's JIT code is forcibly discarded.
    mozilla::Atomic<uint32_t, mozilla::Relaxed,
                    mozilla::recordreplay::Behavior::DontPreserve> warmUpCount = {};

    // 16-bit fields.

    /**
     * Number of times the |warmUpCount| was forcibly discarded. The counter is
     * reset when a script is successfully jit-compiled.
     */
    uint16_t warmUpResetCount = 0;

    /* ES6 function length. */
    uint16_t funLength_ = 0;

    /* Number of type sets used in this script for dynamic type monitoring. */
    uint16_t nTypeSets_ = 0;

    // Bit fields.

  public:
    // The kinds of the optional arrays.
    enum ArrayKind {
        CONSTS,
        OBJECTS,
        TRYNOTES,
        SCOPENOTES,
        ARRAY_KIND_BITS
    };

  private:
    struct BitFields
    {
        /*
         * Bit-fields can't have member initializers til C++2a, i.e. probably
         * C++20, so we can't initialize these to zero in place.  Instead we
         * braced-init this to all zeroes in the JSScript constructor, then
         * custom-assign particular bit-fields in the constructor body.
         */

        // The bits in this field indicate the presence/non-presence of several
        // optional arrays in |data|.  See the comments above Create() for details.
        uint8_t hasArrayBits_ : ARRAY_KIND_BITS;

        /*
         * All remaining bit-fields are single-bit bools.
         */

        // No need for result value of last expression statement.
        bool noScriptRval_ : 1;

        // Code is in strict mode.
        bool strict_ : 1;

        // Code has "use strict"; explicitly.
        bool explicitUseStrict_ : 1;

        // True if the script has a non-syntactic scope on its dynamic scope chain.
        // That is, there are objects about which we know nothing between the
        // outermost syntactic scope and the global.
        bool hasNonSyntacticScope_ : 1;

        // see Parser::selfHostingMode.
        bool selfHosted_ : 1;

        // See FunctionBox.
        bool bindingsAccessedDynamically_ : 1;
        bool funHasExtensibleScope_ : 1;
=======
namespace JS {

// Define a GCManagedDeletePolicy to allow deleting type outside of normal
// sweeping.
template <>
struct DeletePolicy<js::PrivateScriptData>
    : public js::GCManagedDeletePolicy<js::PrivateScriptData> {};

} /* namespace JS */

class JSScript : public js::gc::TenuredCell {
 private:
  // Pointer to baseline->method()->raw(), ion->method()->raw(), a wasm jit
  // entry, the JIT's EnterInterpreter stub, or the lazy link stub. Must be
  // non-null.
  uint8_t* jitCodeRaw_ = nullptr;

  // Shareable script data
  RefPtr<js::SharedScriptData> scriptData_ = {};

  // Unshared variable-length data
  js::PrivateScriptData* data_ = nullptr;

 public:
  JS::Realm* realm_ = nullptr;

 private:
  // JIT and type inference data for this script. May be purged on GC.
  js::jit::JitScript* jitScript_ = nullptr;

  // This script's ScriptSourceObject.
  js::GCPtr<js::ScriptSourceObject*> sourceObject_ = {};

  /*
   * Information attached by Ion. Nexto a valid IonScript this could be
   * ION_DISABLED_SCRIPT, ION_COMPILING_SCRIPT or ION_PENDING_SCRIPT.
   * The later is a ion compilation that is ready, but hasn't been linked
   * yet.
   */
  js::jit::IonScript* ion = nullptr;

  /* Information attached by Baseline. */
  js::jit::BaselineScript* baseline = nullptr;

  /* Information used to re-lazify a lazily-parsed interpreted function. */
  js::LazyScript* lazyScript = nullptr;

  // 32-bit fields.

  /* Size of the used part of the data array. */
  uint32_t dataSize_ = 0;

  /* Base line number of script. */
  uint32_t lineno_ = 0;

  /* Base column of script, optionally set. */
  uint32_t column_ = 0;

  // Range of characters in scriptSource which contains this script's
  // source, that is, the range used by the Parser to produce this script.
  //
  // Most scripted functions have sourceStart_ == toStringStart_ and
  // sourceEnd_ == toStringEnd_. However, for functions with extra
  // qualifiers (e.g. generators, async) and for class constructors (which
  // need to return the entire class source), their values differ.
  //
  // Each field points the following locations.
  //
  //   function * f(a, b) { return a + b; }
  //   ^          ^                        ^
  //   |          |                        |
  //   |          sourceStart_             sourceEnd_
  //   |                                   |
  //   toStringStart_                      toStringEnd_
  //
  // And, in the case of class constructors, an additional toStringEnd
  // offset is used.
  //
  //   class C { constructor() { this.field = 42; } }
  //   ^         ^                                 ^ ^
  //   |         |                                 | `---------`
  //   |         sourceStart_                      sourceEnd_  |
  //   |                                                       |
  //   toStringStart_                                          toStringEnd_
  uint32_t sourceStart_ = 0;
  uint32_t sourceEnd_ = 0;
  uint32_t toStringStart_ = 0;
  uint32_t toStringEnd_ = 0;

  // Number of times the script has been called or has had backedges taken.
  // When running in ion, also increased for any inlined scripts. Reset if
  // the script's JIT code is forcibly discarded.
  mozilla::Atomic<uint32_t, mozilla::Relaxed,
                  mozilla::recordreplay::Behavior::DontPreserve>
      warmUpCount = {};

  // Immutable flags should not be modified after this script has been
  // initialized. These flags should likely be preserved when serializing
  // (XDR) or copying (CopyScript) this script. This is only public for the
  // JITs.
 public:
  enum class ImmutableFlags : uint32_t {
    // No need for result value of last expression statement.
    NoScriptRval = 1 << 0,

    // Code is in strict mode.
    Strict = 1 << 1,

    // (1 << 2) is unused.

    // True if the script has a non-syntactic scope on its dynamic scope chain.
    // That is, there are objects about which we know nothing between the
    // outermost syntactic scope and the global.
    HasNonSyntacticScope = 1 << 3,

    // See Parser::selfHostingMode.
    SelfHosted = 1 << 4,

    // See FunctionBox.
    BindingsAccessedDynamically = 1 << 5,
    FunHasExtensibleScope = 1 << 6,

    // Bytecode contains JSOP_CALLSITEOBJ
    // (We don't relazify functions with template strings, due to observability)
    HasCallSiteObj = 1 << 7,

    // Script has singleton objects.
    HasSingletons = 1 << 8,

    FunctionHasThisBinding = 1 << 9,
    FunctionHasExtraBodyVarScope = 1 << 10,

    // Whether the arguments object for this script, if it needs one, should be
    // mapped (alias formal parameters).
    HasMappedArgsObj = 1 << 11,

    // Script contains inner functions. Used to check if we can relazify the
    // script.
    HasInnerFunctions = 1 << 12,

    NeedsHomeObject = 1 << 13,

    IsDerivedClassConstructor = 1 << 14,
    IsDefaultClassConstructor = 1 << 15,

    // Script is a lambda to treat as running once or a global or eval script
    // that will only run once.  Which one it is can be disambiguated by
    // checking whether function() is null.
    TreatAsRunOnce = 1 << 16,

    // 'this', 'arguments' and f.apply() are used. This is likely to be a
    // wrapper.
    IsLikelyConstructorWrapper = 1 << 17,

    // Set if this function is a generator function or async generator.
    IsGenerator = 1 << 18,

    // Set if this function is an async function or async generator.
    IsAsync = 1 << 19,

    // Set if this function has a rest parameter.
    HasRest = 1 << 20,

    // See comments below.
    ArgsHasVarBinding = 1 << 21,

    // Script came from eval().
    IsForEval = 1 << 22,

    // Whether this is a top-level module script.
    IsModule = 1 << 23,

    // Whether this function needs a call object or named lambda environment.
    NeedsFunctionEnvironmentObjects = 1 << 24,

    // LazyScript flags
    ShouldDeclareArguments = 1 << 25,
    IsBinAST = 1 << 26,
    HasDebuggerStatement = 1 << 27,
    HasDirectEval = 1 << 28,
  };

 private:
  // Note: don't make this a bitfield! It makes it hard to read these flags
  // from JIT code.
  uint32_t immutableFlags_ = 0;

  // Mutable flags typically store information about runtime or deoptimization
  // behavior of this script. This is only public for the JITs.
 public:
  enum class MutableFlags : uint32_t {
    // Number of times the |warmUpCount| was forcibly discarded. The counter is
    // reset when a script is successfully jit-compiled.
    WarmupResets_MASK = 0xFF,

    // Have warned about uses of undefined properties in this script.
    WarnedAboutUndefinedProp = 1 << 8,

    // If treatAsRunOnce, whether script has executed.
    HasRunOnce = 1 << 9,

    // Script has been reused for a clone.
    HasBeenCloned = 1 << 10,

    // Whether the record/replay execution progress counter (see RecordReplay.h)
    // should be updated as this script runs.
    TrackRecordReplayProgress = 1 << 11,

    // Script has an entry in Realm::scriptCountsMap.
    HasScriptCounts = 1 << 12,

    // Script has an entry in Realm::debugScriptMap.
    HasDebugScript = 1 << 13,

    // Do not relazify this script. This is used by the relazify() testing
    // function for scripts that are on the stack and also by the AutoDelazify
    // RAII class. Usually we don't relazify functions in compartments with
    // scripts on the stack, but the relazify() testing function overrides that,
    // and sometimes we're working with a cross-compartment function and need to
    // keep it from relazifying.
    DoNotRelazify = 1 << 14,

    // IonMonkey compilation hints.

    // Script has had hoisted bounds checks fail.
    FailedBoundsCheck = 1 << 15,

    // Script has had hoisted shape guard fail.
    FailedShapeGuard = 1 << 16,

    HadFrequentBailouts = 1 << 17,
    HadOverflowBailout = 1 << 18,

    // Explicitly marked as uninlineable.
    Uninlineable = 1 << 19,

    // Idempotent cache has triggered invalidation.
    InvalidatedIdempotentCache = 1 << 20,

    // Lexical check did fail and bail out.
    FailedLexicalCheck = 1 << 21,

    // See comments below.
    NeedsArgsAnalysis = 1 << 22,
    NeedsArgsObj = 1 << 23,

    // Set if the debugger's onNewScript hook has not yet been called.
    HideScriptFromDebugger = 1 << 24,

    // Set if the script has opted into spew
    SpewEnabled = 1 << 25,
  };

 private:
  // Note: don't make this a bitfield! It makes it hard to read these flags
  // from JIT code.
  uint32_t mutableFlags_ = 0;

  //
  // End of fields.  Start methods.
  //

 private:
  template <js::XDRMode mode>
  friend js::XDRResult js::XDRScript(js::XDRState<mode>* xdr,
                                     js::HandleScope enclosingScope,
                                     js::HandleScriptSourceObject sourceObject,
                                     js::HandleFunction fun,
                                     js::MutableHandleScript scriptp);

  template <js::XDRMode mode>
  friend js::XDRResult js::SharedScriptData::XDR(js::XDRState<mode>* xdr,
                                                 js::HandleScript script);

  friend bool js::SharedScriptData::InitFromEmitter(
      JSContext* cx, js::HandleScript script,
      js::frontend::BytecodeEmitter* bce, uint32_t nslot);

  template <js::XDRMode mode>
  friend js::XDRResult js::PrivateScriptData::XDR(
      js::XDRState<mode>* xdr, js::HandleScript script,
      js::HandleScriptSourceObject sourceObject,
      js::HandleScope scriptEnclosingScope, js::HandleFunction fun);

  friend bool js::PrivateScriptData::Clone(
      JSContext* cx, js::HandleScript src, js::HandleScript dst,
      js::MutableHandle<JS::GCVector<js::Scope*>> scopes);

  friend bool js::PrivateScriptData::InitFromEmitter(
      JSContext* cx, js::HandleScript script,
      js::frontend::BytecodeEmitter* bce);

  friend JSScript* js::detail::CopyScript(
      JSContext* cx, js::HandleScript src,
      js::HandleScriptSourceObject sourceObject,
      js::MutableHandle<JS::GCVector<js::Scope*>> scopes);

 private:
  JSScript(JS::Realm* realm, uint8_t* stubEntry,
           js::HandleScriptSourceObject sourceObject, uint32_t sourceStart,
           uint32_t sourceEnd, uint32_t toStringStart, uint32_t toStringend);

  static JSScript* New(JSContext* cx, js::HandleScriptSourceObject sourceObject,
                       uint32_t sourceStart, uint32_t sourceEnd,
                       uint32_t toStringStart, uint32_t toStringEnd);

 public:
  static JSScript* Create(JSContext* cx,
                          const JS::ReadOnlyCompileOptions& options,
                          js::HandleScriptSourceObject sourceObject,
                          uint32_t sourceStart, uint32_t sourceEnd,
                          uint32_t toStringStart, uint32_t toStringEnd);

  // NOTE: If you use createPrivateScriptData directly instead of via
  // fullyInitFromEmitter, you are responsible for notifying the debugger
  // after successfully creating the script.
  static bool createPrivateScriptData(JSContext* cx,
                                      JS::Handle<JSScript*> script,
                                      uint32_t ngcthings);

 private:
  void initFromFunctionBox(js::frontend::FunctionBox* funbox);

 public:
  static bool fullyInitFromEmitter(JSContext* cx, js::HandleScript script,
                                   js::frontend::BytecodeEmitter* bce);

  // Initialize the Function.prototype script.
  static bool initFunctionPrototype(JSContext* cx, js::HandleScript script,
                                    JS::HandleFunction functionProto);

#ifdef DEBUG
 private:
  // Assert that jump targets are within the code array of the script.
  void assertValidJumpTargets() const;

 public:
#endif

  // MutableFlags accessors.

  MOZ_MUST_USE bool hasFlag(MutableFlags flag) const {
    return mutableFlags_ & uint32_t(flag);
  }
  void setFlag(MutableFlags flag) { mutableFlags_ |= uint32_t(flag); }
  void setFlag(MutableFlags flag, bool b) {
    if (b) {
      setFlag(flag);
    } else {
      clearFlag(flag);
    }
  }
  void clearFlag(MutableFlags flag) { mutableFlags_ &= ~uint32_t(flag); }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Script came from eval(), and is still active.
    IsActiveEval = 1 << 3,
||||||| merged common ancestors
        // True if any formalIsAliased(i).
        bool funHasAnyAliasedFormal_ : 1;
=======
  // ImmutableFlags accessors.
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Script came from eval(), and is in eval cache.
    IsCachedEval = 1 << 4,
||||||| merged common ancestors
        // Have warned about uses of undefined properties in this script.
        bool warnedAboutUndefinedProp_ : 1;
=======
 public:
  MOZ_MUST_USE bool hasFlag(ImmutableFlags flag) const {
    return immutableFlags_ & uint32_t(flag);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Script has an entry in Realm::scriptCountsMap.
    HasScriptCounts = 1 << 5,
||||||| merged common ancestors
        // Script has singleton objects.
        bool hasSingletons_ : 1;
=======
 private:
  void setFlag(ImmutableFlags flag) { immutableFlags_ |= uint32_t(flag); }
  void setFlag(ImmutableFlags flag, bool b) {
    if (b) {
      setFlag(flag);
    } else {
      clearFlag(flag);
    }
  }
  void clearFlag(ImmutableFlags flag) { immutableFlags_ &= ~uint32_t(flag); }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Script has an entry in Realm::debugScriptMap.
    HasDebugScript = 1 << 6,
||||||| merged common ancestors
        // Script is a lambda to treat as running once or a global or eval script
        // that will only run once.  Which one it is can be disambiguated by
        // checking whether function() is null.
        bool treatAsRunOnce_ : 1;
=======
 public:
  inline JSPrincipals* principals();
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Freeze constraints for stack type sets have been generated.
    HasFreezeConstraints = 1 << 7,
||||||| merged common ancestors
        // If treatAsRunOnce, whether script has executed.
        bool hasRunOnce_ : 1;
=======
  JS::Compartment* compartment() const {
    return JS::GetCompartmentForRealm(realm_);
  }
  JS::Compartment* maybeCompartment() const { return compartment(); }
  JS::Realm* realm() const { return realm_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Generation for this script's TypeScript. If out of sync with the
    // TypeZone's generation, the TypeScript needs to be swept.
    TypesGeneration = 1 << 8,
||||||| merged common ancestors
        // Script has been reused for a clone.
        bool hasBeenCloned_ : 1;
=======
  js::SharedScriptData* scriptData() { return scriptData_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Do not relazify this script. This is used by the relazify() testing
    // function for scripts that are on the stack and also by the AutoDelazify
    // RAII class. Usually we don't relazify functions in compartments with
    // scripts on the stack, but the relazify() testing function overrides that,
    // and sometimes we're working with a cross-compartment function and need to
    // keep it from relazifying.
    DoNotRelazify = 1 << 9,
||||||| merged common ancestors
        // Script came from eval(), and is still active.
        bool isActiveEval_ : 1;
=======
  // Script bytecode is immutable after creation.
  jsbytecode* code() const {
    if (!scriptData_) {
      return nullptr;
    }
    return scriptData_->code();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // IonMonkey compilation hints.
||||||| merged common ancestors
        // Script came from eval(), and is in eval cache.
        bool isCachedEval_ : 1;
=======
  bool hasForceInterpreterOp() const {
    // JSOP_FORCEINTERPRETER, if present, must be the first op.
    MOZ_ASSERT(length() >= 1);
    return JSOp(*code()) == JSOP_FORCEINTERPRETER;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Script has had hoisted bounds checks fail.
    FailedBoundsCheck = 1 << 10,
||||||| merged common ancestors
        // 'this', 'arguments' and f.apply() are used. This is likely to be a wrapper.
        bool isLikelyConstructorWrapper_ : 1;
=======
  js::AllBytecodesIterable allLocations() {
    return js::AllBytecodesIterable(this);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Script has had hoisted shape guard fail.
    FailedShapeGuard = 1 << 11,
||||||| merged common ancestors
        // IonMonkey compilation hints.
=======
  js::BytecodeLocation location() { return js::BytecodeLocation(this, code()); }
>>>>>>> upstream-releases

<<<<<<< HEAD
    HadFrequentBailouts = 1 << 12,
    HadOverflowBailout = 1 << 13,
||||||| merged common ancestors
        /* Script has had hoisted bounds checks fail. */
        bool failedBoundsCheck_ : 1;
=======
  bool isUncompleted() const {
    // code() becomes non-null only if this script is complete.
    // See the comment in JSScript::fullyInitFromEmitter.
    return !code();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Explicitly marked as uninlineable.
    Uninlineable = 1 << 14,
||||||| merged common ancestors
        /* Script has had hoisted shape guard fail. */
        bool failedShapeGuard_ : 1;
=======
  size_t length() const {
    MOZ_ASSERT(scriptData_);
    return scriptData_->codeLength();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Idempotent cache has triggered invalidation.
    InvalidatedIdempotentCache = 1 << 15,
||||||| merged common ancestors
        bool hadFrequentBailouts_ : 1;
        bool hadOverflowBailout_ : 1;
=======
  jsbytecode* codeEnd() const { return code() + length(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Lexical check did fail and bail out.
    FailedLexicalCheck = 1 << 16,
||||||| merged common ancestors
        /* Explicitly marked as uninlineable. */
        bool uninlineable_ : 1;
=======
  jsbytecode* lastPC() const {
    jsbytecode* pc = codeEnd() - js::JSOP_RETRVAL_LENGTH;
    MOZ_ASSERT(*pc == JSOP_RETRVAL);
    return pc;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // See comments below.
    NeedsArgsAnalysis = 1 << 17,
    NeedsArgsObj = 1 << 18,
||||||| merged common ancestors
        // Idempotent cache has triggered invalidation.
        bool invalidatedIdempotentCache_ : 1;
=======
  bool containsPC(const jsbytecode* pc) const {
    return pc >= code() && pc < codeEnd();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Set if the debugger's onNewScript hook has not yet been called.
    HideScriptFromDebugger = 1 << 19,
||||||| merged common ancestors
        // Lexical check did fail and bail out.
        bool failedLexicalCheck_ : 1;

        // Script has an entry in Realm::scriptCountsMap.
        bool hasScriptCounts_ : 1;

        // Script has an entry in Realm::debugScriptMap.
        bool hasDebugScript_ : 1;

        // Freeze constraints for stack type sets have been generated.
        bool hasFreezeConstraints_ : 1;
=======
  bool contains(const js::BytecodeLocation& loc) const {
    return containsPC(loc.toRawBytecode());
  }

  size_t pcToOffset(const jsbytecode* pc) const {
    MOZ_ASSERT(containsPC(pc));
    return size_t(pc - code());
  }

  jsbytecode* offsetToPC(size_t offset) const {
    MOZ_ASSERT(offset < length());
    return code() + offset;
  }

  size_t mainOffset() const { return scriptData_->mainOffset; }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Set if the script has opted into spew
    SpewEnabled = 1 << 20,
  };
||||||| merged common ancestors
        /* See comments below. */
        bool argsHasVarBinding_ : 1;
        bool needsArgsAnalysis_ : 1;
        bool needsArgsObj_ : 1;
        bool functionHasThisBinding_ : 1;
        bool functionHasExtraBodyVarScope_ : 1;
=======
  uint32_t lineno() const { return lineno_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  // Note: don't make this a bitfield! It makes it hard to read these flags
  // from JIT code.
  uint32_t mutableFlags_ = 0;

  // 16-bit fields.

  /**
   * Number of times the |warmUpCount| was forcibly discarded. The counter is
   * reset when a script is successfully jit-compiled.
   */
  uint16_t warmUpResetCount = 0;
||||||| merged common ancestors
        // Whether the arguments object for this script, if it needs one, should be
        // mapped (alias formal parameters).
        bool hasMappedArgsObj_ : 1;
=======
  uint32_t column() const { return column_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* ES6 function length. */
  uint16_t funLength_ = 0;
||||||| merged common ancestors
        // Generation for this script's TypeScript. If out of sync with the
        // TypeZone's generation, the TypeScript needs to be swept.
        //
        // This should be a uint32 but is instead a bool so that MSVC packs it
        // correctly.
        bool typesGeneration_ : 1;
=======
  void setColumn(size_t column) { column_ = column; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Number of type sets used in this script for dynamic type monitoring. */
  uint16_t nTypeSets_ = 0;

  //
  // End of fields.  Start methods.
  //

 private:
  template <js::XDRMode mode>
  friend js::XDRResult js::XDRScript(js::XDRState<mode>* xdr,
                                     js::HandleScope enclosingScope,
                                     js::HandleScriptSourceObject sourceObject,
                                     js::HandleFunction fun,
                                     js::MutableHandleScript scriptp);

  friend bool js::detail::CopyScript(
      JSContext* cx, js::HandleScript src, js::HandleScript dst,
      js::MutableHandle<JS::GCVector<js::Scope*>> scopes);
||||||| merged common ancestors
        // Do not relazify this script. This is used by the relazify() testing
        // function for scripts that are on the stack and also by the AutoDelazify
        // RAII class. Usually we don't relazify functions in compartments with
        // scripts on the stack, but the relazify() testing function overrides that,
        // and sometimes we're working with a cross-compartment function and need to
        // keep it from relazifying.
        bool doNotRelazify_ : 1;
=======
  // The fixed part of a stack frame is comprised of vars (in function and
  // module code) and block-scoped locals (in all kinds of code).
  size_t nfixed() const { return scriptData_->nfixed; }
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  JSScript(JS::Realm* realm, uint8_t* stubEntry, js::HandleObject sourceObject,
           uint32_t sourceStart, uint32_t sourceEnd, uint32_t toStringStart,
           uint32_t toStringend);
||||||| merged common ancestors
        // Script contains inner functions. Used to check if we can relazify the
        // script.
        bool hasInnerFunctions_ : 1;
=======
  // Number of fixed slots reserved for slots that are always live. Only
  // nonzero for function or module code.
  size_t numAlwaysLiveFixedSlots() const {
    if (bodyScope()->is<js::FunctionScope>()) {
      return bodyScope()->as<js::FunctionScope>().nextFrameSlot();
    }
    if (bodyScope()->is<js::ModuleScope>()) {
      return bodyScope()->as<js::ModuleScope>().nextFrameSlot();
    }
    return 0;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static JSScript* New(JSContext* cx, js::HandleObject sourceObject,
                       uint32_t sourceStart, uint32_t sourceEnd,
                       uint32_t toStringStart, uint32_t toStringEnd);
||||||| merged common ancestors
        bool needsHomeObject_ : 1;
=======
  // Calculate the number of fixed slots that are live at a particular bytecode.
  size_t calculateLiveFixed(jsbytecode* pc);
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  static JSScript* Create(JSContext* cx,
                          const JS::ReadOnlyCompileOptions& options,
                          js::HandleObject sourceObject, uint32_t sourceStart,
                          uint32_t sourceEnd, uint32_t toStringStart,
                          uint32_t toStringEnd);

  // NOTE: If you use createPrivateScriptData directly instead of via
  // fullyInitFromEmitter, you are responsible for notifying the debugger
  // after successfully creating the script.
  static bool createPrivateScriptData(JSContext* cx,
                                      JS::Handle<JSScript*> script,
                                      uint32_t nscopes, uint32_t nconsts,
                                      uint32_t nobjects, uint32_t ntrynotes,
                                      uint32_t nscopenotes,
                                      uint32_t nresumeoffsets);

 private:
  static void initFromFunctionBox(js::HandleScript script,
                                  js::frontend::FunctionBox* funbox);
  static void initFromModuleContext(js::HandleScript script);

 public:
  static bool fullyInitFromEmitter(JSContext* cx, js::HandleScript script,
                                   js::frontend::BytecodeEmitter* bce);

  // Initialize the Function.prototype script.
  static bool initFunctionPrototype(JSContext* cx, js::HandleScript script,
                                    JS::HandleFunction functionProto);
||||||| merged common ancestors
        bool isDerivedClassConstructor_ : 1;
        bool isDefaultClassConstructor_ : 1;

        // True if this function is a generator function or async generator.
        bool isGenerator_ : 1;

        // True if this function is an async function or async generator.
        bool isAsync_ : 1;

        bool hasRest_ : 1;

        // True if the debugger's onNewScript hook has not yet been called.
        bool hideScriptFromDebugger_ : 1;
    } bitFields_;

    //
    // End of fields.  Start methods.
    //

  private:
    template <js::XDRMode mode>
    friend
    js::XDRResult
    js::XDRScript(js::XDRState<mode>* xdr, js::HandleScope enclosingScope,
                  js::HandleScriptSourceObject sourceObject, js::HandleFunction fun,
                  js::MutableHandleScript scriptp);

    friend bool
    js::detail::CopyScript(JSContext* cx, js::HandleScript src, js::HandleScript dst,
                           js::MutableHandle<JS::GCVector<js::Scope*>> scopes);

  private:
    JSScript(JS::Realm* realm, uint8_t* stubEntry, const JS::ReadOnlyCompileOptions& options,
             js::HandleObject sourceObject, uint32_t bufStart, uint32_t bufEnd,
             uint32_t toStringStart, uint32_t toStringend);

    static JSScript* createInitialized(JSContext* cx, const JS::ReadOnlyCompileOptions& options,
                                       js::HandleObject sourceObject,
                                       uint32_t bufStart, uint32_t bufEnd,
                                       uint32_t toStringStart, uint32_t toStringEnd);

  public:
    static JSScript* Create(JSContext* cx,
                            const JS::ReadOnlyCompileOptions& options,
                            js::HandleObject sourceObject,
                            uint32_t sourceStart, uint32_t sourceEnd,
                            uint32_t toStringStart, uint32_t toStringEnd);

    // Three ways ways to initialize a JSScript. Callers of partiallyInit()
    // are responsible for notifying the debugger after successfully creating
    // any kind (function or other) of new JSScript.  However, callers of
    // fullyInitFromEmitter() do not need to do this.
    static bool partiallyInit(JSContext* cx, JS::Handle<JSScript*> script,
                              uint32_t nscopes, uint32_t nconsts, uint32_t nobjects,
                              uint32_t ntrynotes, uint32_t nscopenotes, uint32_t nyieldoffsets);

  private:
    static void initFromFunctionBox(js::HandleScript script, js::frontend::FunctionBox* funbox);
    static void initFromModuleContext(js::HandleScript script);

  public:
    static bool fullyInitFromEmitter(JSContext* cx, js::HandleScript script,
                                     js::frontend::BytecodeEmitter* bce);

    // Initialize the Function.prototype script.
    static bool initFunctionPrototype(JSContext* cx, js::HandleScript script,
                                      JS::HandleFunction functionProto);
=======
  size_t nslots() const { return scriptData_->nslots; }

  unsigned numArgs() const {
    if (bodyScope()->is<js::FunctionScope>()) {
      return bodyScope()
          ->as<js::FunctionScope>()
          .numPositionalFormalParameters();
    }
    return 0;
  }

  inline js::Shape* initialEnvironmentShape() const;

  bool functionHasParameterExprs() const {
    // Only functions have parameters.
    js::Scope* scope = bodyScope();
    if (!scope->is<js::FunctionScope>()) {
      return false;
    }
    return scope->as<js::FunctionScope>().hasParameterExprs();
  }

  // If there are more than MaxBytecodeTypeSets JOF_TYPESET ops in the script,
  // the first MaxBytecodeTypeSets - 1 JOF_TYPESET ops have their own TypeSet
  // and all other JOF_TYPESET ops share the last TypeSet.
  static constexpr size_t MaxBytecodeTypeSets = UINT16_MAX;
  static_assert(sizeof(js::SharedScriptData::numBytecodeTypeSets) == 2,
                "MaxBytecodeTypeSets must match sizeof(numBytecodeTypeSets)");

  size_t numBytecodeTypeSets() const {
    return scriptData_->numBytecodeTypeSets;
  }

  size_t numICEntries() const { return scriptData_->numICEntries; }

  size_t funLength() const { return scriptData_->funLength; }

  uint32_t sourceStart() const { return sourceStart_; }

  uint32_t sourceEnd() const { return sourceEnd_; }

  uint32_t sourceLength() const { return sourceEnd_ - sourceStart_; }

  uint32_t toStringStart() const { return toStringStart_; }

  uint32_t toStringEnd() const { return toStringEnd_; }

  bool noScriptRval() const { return hasFlag(ImmutableFlags::NoScriptRval); }

  bool strict() const { return hasFlag(ImmutableFlags::Strict); }

  bool hasNonSyntacticScope() const {
    return hasFlag(ImmutableFlags::HasNonSyntacticScope);
  }

  bool selfHosted() const { return hasFlag(ImmutableFlags::SelfHosted); }
  bool bindingsAccessedDynamically() const {
    return hasFlag(ImmutableFlags::BindingsAccessedDynamically);
  }
  bool funHasExtensibleScope() const {
    return hasFlag(ImmutableFlags::FunHasExtensibleScope);
  }

  bool hasCallSiteObj() const {
    return hasFlag(ImmutableFlags::HasCallSiteObj);
  }

  bool hasSingletons() const { return hasFlag(ImmutableFlags::HasSingletons); }
  bool treatAsRunOnce() const {
    return hasFlag(ImmutableFlags::TreatAsRunOnce);
  }
  bool hasRunOnce() const { return hasFlag(MutableFlags::HasRunOnce); }
  bool hasBeenCloned() const { return hasFlag(MutableFlags::HasBeenCloned); }

  void setTreatAsRunOnce() { setFlag(ImmutableFlags::TreatAsRunOnce); }
  void setHasRunOnce() { setFlag(MutableFlags::HasRunOnce); }
  void setHasBeenCloned() { setFlag(MutableFlags::HasBeenCloned); }

  void cacheForEval() {
    MOZ_ASSERT(isForEval());
    // IsEvalCacheCandidate will make sure that there's nothing in this
    // script that would prevent reexecution even if isRunOnce is
    // true.  So just pretend like we never ran this script.
    clearFlag(MutableFlags::HasRunOnce);
  }

  bool isLikelyConstructorWrapper() const {
    return hasFlag(ImmutableFlags::IsLikelyConstructorWrapper);
  }
  void setLikelyConstructorWrapper() {
    setFlag(ImmutableFlags::IsLikelyConstructorWrapper);
  }

  bool failedBoundsCheck() const {
    return hasFlag(MutableFlags::FailedBoundsCheck);
  }
  bool failedShapeGuard() const {
    return hasFlag(MutableFlags::FailedShapeGuard);
  }
  bool hadFrequentBailouts() const {
    return hasFlag(MutableFlags::HadFrequentBailouts);
  }
  bool hadOverflowBailout() const {
    return hasFlag(MutableFlags::HadOverflowBailout);
  }
  bool uninlineable() const { return hasFlag(MutableFlags::Uninlineable); }
  bool invalidatedIdempotentCache() const {
    return hasFlag(MutableFlags::InvalidatedIdempotentCache);
  }
  bool failedLexicalCheck() const {
    return hasFlag(MutableFlags::FailedLexicalCheck);
  }
  bool isDefaultClassConstructor() const {
    return hasFlag(ImmutableFlags::IsDefaultClassConstructor);
  }

  void setFailedBoundsCheck() { setFlag(MutableFlags::FailedBoundsCheck); }
  void setFailedShapeGuard() { setFlag(MutableFlags::FailedShapeGuard); }
  void setHadFrequentBailouts() { setFlag(MutableFlags::HadFrequentBailouts); }
  void setHadOverflowBailout() { setFlag(MutableFlags::HadOverflowBailout); }
  void setUninlineable() { setFlag(MutableFlags::Uninlineable); }
  void setInvalidatedIdempotentCache() {
    setFlag(MutableFlags::InvalidatedIdempotentCache);
  }
  void setFailedLexicalCheck() { setFlag(MutableFlags::FailedLexicalCheck); }
  void setIsDefaultClassConstructor() {
    setFlag(ImmutableFlags::IsDefaultClassConstructor);
  }

  bool hasScriptCounts() const {
    return hasFlag(MutableFlags::HasScriptCounts);
  }
  bool hasScriptName();

  bool warnedAboutUndefinedProp() const {
    return hasFlag(MutableFlags::WarnedAboutUndefinedProp);
  }
  void setWarnedAboutUndefinedProp() {
    setFlag(MutableFlags::WarnedAboutUndefinedProp);
  }

  /* See ContextFlags::funArgumentsHasLocalBinding comment. */
  bool argumentsHasVarBinding() const {
    return hasFlag(ImmutableFlags::ArgsHasVarBinding);
  }
  void setArgumentsHasVarBinding();
  bool argumentsAliasesFormals() const {
    return argumentsHasVarBinding() && hasMappedArgsObj();
  }

  js::GeneratorKind generatorKind() const {
    return isGenerator() ? js::GeneratorKind::Generator
                         : js::GeneratorKind::NotGenerator;
  }
  bool isGenerator() const { return hasFlag(ImmutableFlags::IsGenerator); }

  js::FunctionAsyncKind asyncKind() const {
    return isAsync() ? js::FunctionAsyncKind::AsyncFunction
                     : js::FunctionAsyncKind::SyncFunction;
  }
  bool isAsync() const { return hasFlag(ImmutableFlags::IsAsync); }

  bool hasRest() const { return hasFlag(ImmutableFlags::HasRest); }

  bool hideScriptFromDebugger() const {
    return hasFlag(MutableFlags::HideScriptFromDebugger);
  }
  void clearHideScriptFromDebugger() {
    clearFlag(MutableFlags::HideScriptFromDebugger);
  }

  bool spewEnabled() const { return hasFlag(MutableFlags::SpewEnabled); }
  void setSpewEnabled(bool enabled) {
    setFlag(MutableFlags::SpewEnabled, enabled);
  }

  bool needsHomeObject() const {
    return hasFlag(ImmutableFlags::NeedsHomeObject);
  }

  bool isDerivedClassConstructor() const {
    return hasFlag(ImmutableFlags::IsDerivedClassConstructor);
  }

  /*
   * As an optimization, even when argsHasLocalBinding, the function prologue
   * may not need to create an arguments object. This is determined by
   * needsArgsObj which is set by AnalyzeArgumentsUsage. When !needsArgsObj,
   * the prologue may simply write MagicValue(JS_OPTIMIZED_ARGUMENTS) to
   * 'arguments's slot and any uses of 'arguments' will be guaranteed to
   * handle this magic value. To avoid spurious arguments object creation, we
   * maintain the invariant that needsArgsObj is only called after the script
   * has been analyzed.
   */
  bool analyzedArgsUsage() const {
    return !hasFlag(MutableFlags::NeedsArgsAnalysis);
  }
  inline bool ensureHasAnalyzedArgsUsage(JSContext* cx);
  bool needsArgsObj() const {
    MOZ_ASSERT(analyzedArgsUsage());
    return hasFlag(MutableFlags::NeedsArgsObj);
  }
  void setNeedsArgsObj(bool needsArgsObj);
  static void argumentsOptimizationFailed(JSContext* cx,
                                          js::HandleScript script);

  bool hasMappedArgsObj() const {
    return hasFlag(ImmutableFlags::HasMappedArgsObj);
  }

  bool functionHasThisBinding() const {
    return hasFlag(ImmutableFlags::FunctionHasThisBinding);
  }

  void setFieldInitializers(js::FieldInitializers fieldInitializers) {
    MOZ_ASSERT(data_);
    data_->setFieldInitializers(fieldInitializers);
  }

  const js::FieldInitializers& getFieldInitializers() const {
    MOZ_ASSERT(data_);
    return data_->getFieldInitializers();
  }

  /*
   * Arguments access (via JSOP_*ARG* opcodes) must access the canonical
   * location for the argument. If an arguments object exists AND it's mapped
   * ('arguments' aliases formals), then all access must go through the
   * arguments object. Otherwise, the local slot is the canonical location for
   * the arguments. Note: if a formal is aliased through the scope chain, then
   * script->formalIsAliased and JSOP_*ARG* opcodes won't be emitted at all.
   */
  bool argsObjAliasesFormals() const {
    return needsArgsObj() && hasMappedArgsObj();
  }

  bool hasDoNotRelazify() const { return hasFlag(MutableFlags::DoNotRelazify); }
  void setDoNotRelazify(bool b) { setFlag(MutableFlags::DoNotRelazify, b); }

  bool hasInnerFunctions() const {
    return hasFlag(ImmutableFlags::HasInnerFunctions);
  }

  static constexpr size_t offsetOfMutableFlags() {
    return offsetof(JSScript, mutableFlags_);
  }
  static size_t offsetOfImmutableFlags() {
    return offsetof(JSScript, immutableFlags_);
  }
  static constexpr size_t offsetOfScriptData() {
    return offsetof(JSScript, scriptData_);
  }
  static constexpr size_t offsetOfPrivateScriptData() {
    return offsetof(JSScript, data_);
  }
  static constexpr size_t offsetOfJitScript() {
    return offsetof(JSScript, jitScript_);
  }

  bool hasAnyIonScript() const { return hasIonScript(); }

  bool hasIonScript() const {
    bool res = ion && ion != ION_DISABLED_SCRIPT &&
               ion != ION_COMPILING_SCRIPT && ion != ION_PENDING_SCRIPT;
    MOZ_ASSERT_IF(res, baseline);
    return res;
  }
  bool canIonCompile() const { return ion != ION_DISABLED_SCRIPT; }
  bool isIonCompilingOffThread() const { return ion == ION_COMPILING_SCRIPT; }

  js::jit::IonScript* ionScript() const {
    MOZ_ASSERT(hasIonScript());
    return ion;
  }
  js::jit::IonScript* maybeIonScript() const { return ion; }
  js::jit::IonScript* const* addressOfIonScript() const { return &ion; }
  void setIonScript(JSRuntime* rt, js::jit::IonScript* ionScript);
  inline void clearIonScript();

  bool hasBaselineScript() const {
    bool res = baseline && baseline != BASELINE_DISABLED_SCRIPT;
    MOZ_ASSERT_IF(!res, !ion || ion == ION_DISABLED_SCRIPT);
    return res;
  }
  bool canBaselineCompile() const {
    return baseline != BASELINE_DISABLED_SCRIPT;
  }
  js::jit::BaselineScript* baselineScript() const {
    MOZ_ASSERT(hasBaselineScript());
    return baseline;
  }
  inline void setBaselineScript(JSRuntime* rt,
                                js::jit::BaselineScript* baselineScript);
  inline void clearBaselineScript();

  void updateJitCodeRaw(JSRuntime* rt);

  static size_t offsetOfBaselineScript() {
    return offsetof(JSScript, baseline);
  }
  static size_t offsetOfIonScript() { return offsetof(JSScript, ion); }
  static constexpr size_t offsetOfJitCodeRaw() {
    return offsetof(JSScript, jitCodeRaw_);
  }
  uint8_t* jitCodeRaw() const { return jitCodeRaw_; }

  // We don't relazify functions with a JitScript or JIT code, but some
  // callers (XDR, testing functions) want to know whether this script is
  // relazifiable ignoring (or after) discarding JIT code.
  bool isRelazifiableIgnoringJitCode() const {
    return (selfHosted() || lazyScript) && !hasInnerFunctions() &&
           !isGenerator() && !isAsync() && !isDefaultClassConstructor() &&
           !hasDoNotRelazify() && !hasCallSiteObj();
  }
  bool isRelazifiable() const {
    MOZ_ASSERT_IF(hasBaselineScript() || hasIonScript(), jitScript_);
    return isRelazifiableIgnoringJitCode() && !jitScript_;
  }
  void setLazyScript(js::LazyScript* lazy) { lazyScript = lazy; }
  js::LazyScript* maybeLazyScript() { return lazyScript; }

  /*
   * Original compiled function for the script, if it has a function.
   * nullptr for global and eval scripts.
   * The delazifying variant ensures that the function isn't lazy. The
   * non-delazifying variant must only be used after earlier code has
   * called ensureNonLazyCanonicalFunction and while the function can't
   * have been relazified.
   */
  inline JSFunction* functionDelazifying() const;
  JSFunction* functionNonDelazifying() const {
    if (bodyScope()->is<js::FunctionScope>()) {
      return bodyScope()->as<js::FunctionScope>().canonicalFunction();
    }
    return nullptr;
  }
  /*
   * De-lazifies the canonical function. Must be called before entering code
   * that expects the function to be non-lazy.
   */
  inline void ensureNonLazyCanonicalFunction();

  bool isModule() const {
    MOZ_ASSERT(hasFlag(ImmutableFlags::IsModule) ==
               bodyScope()->is<js::ModuleScope>());
    return hasFlag(ImmutableFlags::IsModule);
  }
  js::ModuleObject* module() const {
    if (isModule()) {
      return bodyScope()->as<js::ModuleScope>().module();
    }
    return nullptr;
  }

  bool isGlobalOrEvalCode() const {
    return bodyScope()->is<js::GlobalScope>() ||
           bodyScope()->is<js::EvalScope>();
  }
  bool isGlobalCode() const { return bodyScope()->is<js::GlobalScope>(); }

  // Returns true if the script may read formal arguments on the stack
  // directly, via lazy arguments or a rest parameter.
  bool mayReadFrameArgsDirectly();

  static JSFlatString* sourceData(JSContext* cx, JS::HandleScript script);

  MOZ_MUST_USE bool appendSourceDataForToString(JSContext* cx,
                                                js::StringBuffer& buf);

  void setSourceObject(js::ScriptSourceObject* object);
  js::ScriptSourceObject* sourceObject() const { return sourceObject_; }
  js::ScriptSource* scriptSource() const;
  js::ScriptSource* maybeForwardedScriptSource() const;

  void setDefaultClassConstructorSpan(js::ScriptSourceObject* sourceObject,
                                      uint32_t start, uint32_t end,
                                      unsigned line, unsigned column);

  bool mutedErrors() const { return scriptSource()->mutedErrors(); }
  const char* filename() const { return scriptSource()->filename(); }
  const char* maybeForwardedFilename() const {
    return maybeForwardedScriptSource()->filename();
  }

#ifdef MOZ_VTUNE
  // Unique Method ID passed to the VTune profiler. Allows attribution of
  // different jitcode to the same source script.
  uint32_t vtuneMethodID();
#endif

 public:
  /* Return whether this script was compiled for 'eval' */
  bool isForEval() const {
    bool forEval = hasFlag(ImmutableFlags::IsForEval);
    MOZ_ASSERT_IF(forEval, bodyScope()->is<js::EvalScope>());
    return forEval;
  }

  /* Return whether this is a 'direct eval' script in a function scope. */
  bool isDirectEvalInFunction() const {
    if (!isForEval()) {
      return false;
    }
    return bodyScope()->hasOnChain(js::ScopeKind::Function);
  }

  /*
   * Return whether this script is a top-level script.
   *
   * If we evaluate some code which contains a syntax error, then we might
   * produce a JSScript which has no associated bytecode. Testing with
   * |code()| filters out this kind of scripts.
   *
   * If this script has a function associated to it, then it is not the
   * top-level of a file.
   */
  bool isTopLevel() { return code() && !functionNonDelazifying(); }

  /* Ensure the script has a JitScript. */
  inline bool ensureHasJitScript(JSContext* cx, js::jit::AutoKeepJitScripts&);

  bool hasJitScript() const { return jitScript_ != nullptr; }
  js::jit::JitScript* jitScript() { return jitScript_; }

  void maybeReleaseJitScript();
  void releaseJitScript();

  inline js::GlobalObject& global() const;
  inline bool hasGlobal(const js::GlobalObject* global) const;
  js::GlobalObject& uninlinedGlobal() const;

  uint32_t bodyScopeIndex() const { return scriptData_->bodyScopeIndex; }

  js::Scope* bodyScope() const { return getScope(bodyScopeIndex()); }

  js::Scope* outermostScope() const {
    // The body scope may not be the outermost scope in the script when
    // the decl env scope is present.
    size_t index = 0;
    return getScope(index);
  }

  bool needsFunctionEnvironmentObjects() const {
    return hasFlag(ImmutableFlags::NeedsFunctionEnvironmentObjects);
  }

  bool functionHasExtraBodyVarScope() const {
    bool res = hasFlag(ImmutableFlags::FunctionHasExtraBodyVarScope);
    MOZ_ASSERT_IF(res, functionHasParameterExprs());
    return res;
  }

  js::VarScope* functionExtraBodyVarScope() const {
    MOZ_ASSERT(functionHasExtraBodyVarScope());
    for (JS::GCCellPtr gcThing : gcthings()) {
      if (!gcThing.is<js::Scope>()) {
        continue;
      }
      js::Scope* scope = &gcThing.as<js::Scope>();
      if (scope->kind() == js::ScopeKind::FunctionBodyVar) {
        return &scope->as<js::VarScope>();
      }
    }
    MOZ_CRASH("Function extra body var scope not found");
  }

  bool needsBodyEnvironment() const {
    for (JS::GCCellPtr gcThing : gcthings()) {
      if (!gcThing.is<js::Scope>()) {
        continue;
      }
      js::Scope* scope = &gcThing.as<js::Scope>();
      if (ScopeKindIsInBody(scope->kind()) && scope->hasEnvironment()) {
        return true;
      }
    }
    return false;
  }

  inline js::LexicalScope* maybeNamedLambdaScope() const;

  js::Scope* enclosingScope() const { return outermostScope()->enclosing(); }

 private:
  bool createJitScript(JSContext* cx);

  bool createSharedScriptData(JSContext* cx, uint32_t codeLength,
                              uint32_t noteLength, uint32_t natoms,
                              uint32_t numResumeOffsets, uint32_t numScopeNotes,
                              uint32_t numTryNotes);
  bool shareScriptData(JSContext* cx);
  void freeScriptData();

 public:
  uint32_t getWarmUpCount() const { return warmUpCount; }
  uint32_t incWarmUpCounter(uint32_t amount = 1) {
    return warmUpCount += amount;
  }
  uint32_t* addressOfWarmUpCounter() {
    return reinterpret_cast<uint32_t*>(&warmUpCount);
  }
  static size_t offsetOfWarmUpCounter() {
    return offsetof(JSScript, warmUpCount);
  }
  void resetWarmUpCounterForGC() {
    incWarmUpResetCounter();
    warmUpCount = 0;
  }

  void resetWarmUpCounterToDelayIonCompilation();

  unsigned getWarmUpResetCount() const {
    constexpr uint32_t MASK = uint32_t(MutableFlags::WarmupResets_MASK);
    return mutableFlags_ & MASK;
  }
  void incWarmUpResetCounter() {
    constexpr uint32_t MASK = uint32_t(MutableFlags::WarmupResets_MASK);
    uint32_t newCount = getWarmUpResetCount() + 1;
    if (newCount <= MASK) {
      mutableFlags_ &= ~MASK;
      mutableFlags_ |= newCount;
    }
  }
  void resetWarmUpResetCounter() {
    constexpr uint32_t MASK = uint32_t(MutableFlags::WarmupResets_MASK);
    mutableFlags_ &= ~MASK;
  }

 public:
  bool initScriptCounts(JSContext* cx);
  bool initScriptName(JSContext* cx);
  js::ScriptCounts& getScriptCounts();
  const char* getScriptName();
  js::PCCounts* maybeGetPCCounts(jsbytecode* pc);
  const js::PCCounts* maybeGetThrowCounts(jsbytecode* pc);
  js::PCCounts* getThrowCounts(jsbytecode* pc);
  uint64_t getHitCount(jsbytecode* pc);
  void incHitCount(jsbytecode* pc);  // Used when we bailout out of Ion.
  void addIonCounts(js::jit::IonScriptCounts* ionCounts);
  js::jit::IonScriptCounts* getIonCounts();
  void releaseScriptCounts(js::ScriptCounts* counts);
  void destroyScriptCounts();
  void destroyScriptName();
  void clearHasScriptCounts();
  void resetScriptCounts();

  jsbytecode* main() const { return code() + mainOffset(); }

  js::BytecodeLocation mainLocation() const {
    return js::BytecodeLocation(this, main());
  }

  js::BytecodeLocation endLocation() const {
    return js::BytecodeLocation(this, codeEnd());
  }

  /*
   * computedSizeOfData() is the in-use size of all the data sections.
   * sizeOfData() is the size of the block allocated to hold all the data
   * sections (which can be larger than the in-use size).
   */
  size_t computedSizeOfData() const;
  size_t sizeOfData(mozilla::MallocSizeOf mallocSizeOf) const;

  void addSizeOfJitScript(mozilla::MallocSizeOf mallocSizeOf,
                          size_t* sizeOfJitScript,
                          size_t* sizeOfBaselineFallbackStubs) const;

  size_t dataSize() const { return dataSize_; }

  bool hasTrynotes() const { return !scriptData_->tryNotes().empty(); }
  bool hasScopeNotes() const { return !scriptData_->scopeNotes().empty(); }
  bool hasResumeOffsets() const {
    return !scriptData_->resumeOffsets().empty();
  }

  mozilla::Span<const JS::GCCellPtr> gcthings() const {
    return data_->gcthings();
  }

  mozilla::Span<const JSTryNote> trynotes() const {
    return scriptData_->tryNotes();
  }

  mozilla::Span<const js::ScopeNote> scopeNotes() const {
    return scriptData_->scopeNotes();
  }

  mozilla::Span<const uint32_t> resumeOffsets() const {
    return scriptData_->resumeOffsets();
  }

  uint32_t tableSwitchCaseOffset(jsbytecode* pc, uint32_t caseIndex) const {
    MOZ_ASSERT(containsPC(pc));
    MOZ_ASSERT(*pc == JSOP_TABLESWITCH);
    uint32_t firstResumeIndex = GET_RESUMEINDEX(pc + 3 * JUMP_OFFSET_LEN);
    return resumeOffsets()[firstResumeIndex + caseIndex];
  }
  jsbytecode* tableSwitchCasePC(jsbytecode* pc, uint32_t caseIndex) const {
    return offsetToPC(tableSwitchCaseOffset(pc, caseIndex));
  }

  bool hasLoops();

  uint32_t numNotes() const {
    MOZ_ASSERT(scriptData_);
    return scriptData_->noteLength();
  }
  jssrcnote* notes() const {
    MOZ_ASSERT(scriptData_);
    return scriptData_->notes();
  }

  size_t natoms() const {
    MOZ_ASSERT(scriptData_);
    return scriptData_->natoms();
  }
  js::GCPtrAtom* atoms() const {
    MOZ_ASSERT(scriptData_);
    return scriptData_->atoms();
  }

  js::GCPtrAtom& getAtom(size_t index) const {
    MOZ_ASSERT(index < natoms());
    return atoms()[index];
  }

  js::GCPtrAtom& getAtom(jsbytecode* pc) const {
    MOZ_ASSERT(containsPC(pc) && containsPC(pc + sizeof(uint32_t)));
    MOZ_ASSERT(js::JOF_OPTYPE((JSOp)*pc) == JOF_ATOM);
    return getAtom(GET_UINT32_INDEX(pc));
  }

  js::PropertyName* getName(size_t index) {
    return getAtom(index)->asPropertyName();
  }

  js::PropertyName* getName(jsbytecode* pc) const {
    return getAtom(pc)->asPropertyName();
  }

  JSObject* getObject(size_t index) {
    MOZ_ASSERT(gcthings()[index].asCell()->isTenured());
    return &gcthings()[index].as<JSObject>();
  }

  JSObject* getObject(jsbytecode* pc) {
    MOZ_ASSERT(containsPC(pc) && containsPC(pc + sizeof(uint32_t)));
    return getObject(GET_UINT32_INDEX(pc));
  }

  js::Scope* getScope(size_t index) const {
    return &gcthings()[index].as<js::Scope>();
  }

  js::Scope* getScope(jsbytecode* pc) const {
    // This method is used to get a scope directly using a JSOp with an
    // index. To search through ScopeNotes to look for a Scope using pc,
    // use lookupScope.
    MOZ_ASSERT(containsPC(pc) && containsPC(pc + sizeof(uint32_t)));
    MOZ_ASSERT(js::JOF_OPTYPE(JSOp(*pc)) == JOF_SCOPE,
               "Did you mean to use lookupScope(pc)?");
    return getScope(GET_UINT32_INDEX(pc));
  }

  inline JSFunction* getFunction(size_t index);
  inline JSFunction* getFunction(jsbytecode* pc);

  JSFunction* function() const {
    if (functionNonDelazifying()) {
      return functionNonDelazifying();
    }
    return nullptr;
  }

  inline js::RegExpObject* getRegExp(size_t index);
  inline js::RegExpObject* getRegExp(jsbytecode* pc);

  js::BigInt* getBigInt(size_t index) {
    return &gcthings()[index].as<js::BigInt>();
  }

  js::BigInt* getBigInt(jsbytecode* pc) {
    MOZ_ASSERT(containsPC(pc));
    MOZ_ASSERT(js::JOF_OPTYPE(JSOp(*pc)) == JOF_BIGINT);
    return getBigInt(GET_UINT32_INDEX(pc));
  }

  // The following 3 functions find the static scope just before the
  // execution of the instruction pointed to by pc.

  js::Scope* lookupScope(jsbytecode* pc);

  js::Scope* innermostScope(jsbytecode* pc);
  js::Scope* innermostScope() { return innermostScope(main()); }

  /*
   * The isEmpty method tells whether this script has code that computes any
   * result (not return value, result AKA normal completion value) other than
   * JSVAL_VOID, or any other effects.
   */
  bool isEmpty() const {
    if (length() > 3) {
      return false;
    }

    jsbytecode* pc = code();
    if (noScriptRval() && JSOp(*pc) == JSOP_FALSE) {
      ++pc;
    }
    return JSOp(*pc) == JSOP_RETRVAL;
  }

  bool formalIsAliased(unsigned argSlot);
  bool formalLivesInArgumentsObject(unsigned argSlot);

 private:
  /* Change this->stepMode to |newValue|. */
  void setNewStepMode(js::FreeOp* fop, uint32_t newValue);

  js::DebugScript* getOrCreateDebugScript(JSContext* cx);
  js::DebugScript* debugScript();
  js::DebugScript* releaseDebugScript();
  void destroyDebugScript(js::FreeOp* fop);
  void freeDebugScript(js::FreeOp* fop);

  bool hasDebugScript() const { return hasFlag(MutableFlags::HasDebugScript); }

 public:
  bool hasBreakpointsAt(jsbytecode* pc);
  bool hasAnyBreakpointsOrStepMode() { return hasDebugScript(); }

  // See comment above 'debugMode' in Realm.h for explanation of
  // invariants of debuggee compartments, scripts, and frames.
  inline bool isDebuggee() const;

  js::BreakpointSite* getBreakpointSite(jsbytecode* pc) {
    return hasDebugScript() ? debugScript()->breakpoints[pcToOffset(pc)]
                            : nullptr;
  }

  js::BreakpointSite* getOrCreateBreakpointSite(JSContext* cx, jsbytecode* pc);

  void destroyBreakpointSite(js::FreeOp* fop, jsbytecode* pc);

  void clearBreakpointsIn(js::FreeOp* fop, js::Debugger* dbg,
                          JSObject* handler);

  /*
   * Increment or decrement the single-step count. If the count is non-zero
   * then the script is in single-step mode.
   *
   * Only incrementing is fallible, as it could allocate a DebugScript.
   */
  bool incrementStepperCount(JSContext* cx);
  void decrementStepperCount(js::FreeOp* fop);

  bool stepModeEnabled() {
    return hasDebugScript() && debugScript()->stepperCount > 0;
  }
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
 private:
  // Assert that jump targets are within the code array of the script.
  void assertValidJumpTargets() const;
||||||| merged common ancestors
  private:
    // Assert that jump targets are within the code array of the script.
    void assertValidJumpTargets() const;
=======
  uint32_t stepperCount() {
    return hasDebugScript() ? debugScript()->stepperCount : 0;
  }
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  // MutableFlags accessors.

  MOZ_MUST_USE bool hasFlag(MutableFlags flag) const {
    return mutableFlags_ & uint32_t(flag);
  }
  void setFlag(MutableFlags flag) { mutableFlags_ |= uint32_t(flag); }
  void setFlag(MutableFlags flag, bool b) {
    if (b) {
      setFlag(flag);
    } else {
      clearFlag(flag);
    }
  }
  void clearFlag(MutableFlags flag) { mutableFlags_ &= ~uint32_t(flag); }

  // ImmutableFlags accessors.

  MOZ_MUST_USE bool hasFlag(ImmutableFlags flag) const {
    return immutableFlags_ & uint32_t(flag);
  }
  void setFlag(ImmutableFlags flag) { immutableFlags_ |= uint32_t(flag); }
  void setFlag(ImmutableFlags flag, bool b) {
    if (b) {
      setFlag(flag);
    } else {
      clearFlag(flag);
    }
  }
  void clearFlag(ImmutableFlags flag) { immutableFlags_ &= ~uint32_t(flag); }

 public:
  inline JSPrincipals* principals();

  JS::Compartment* compartment() const {
    return JS::GetCompartmentForRealm(realm_);
  }
  JS::Compartment* maybeCompartment() const { return compartment(); }
  JS::Realm* realm() const { return realm_; }

  js::SharedScriptData* scriptData() { return scriptData_; }

  // Script bytecode is immutable after creation.
  jsbytecode* code() const {
    if (!scriptData_) {
      return nullptr;
    }
    return scriptData_->code();
  }

  js::AllBytecodesIterable allLocations() {
    return js::AllBytecodesIterable(this);
  }

  js::BytecodeLocation location() { return js::BytecodeLocation(this, code()); }

  bool isUncompleted() const {
    // code() becomes non-null only if this script is complete.
    // See the comment in JSScript::fullyInitFromEmitter.
    return !code();
  }

  size_t length() const {
    MOZ_ASSERT(scriptData_);
    return scriptData_->codeLength();
  }

  jsbytecode* codeEnd() const { return code() + length(); }
||||||| merged common ancestors
  public:
    inline JSPrincipals* principals();

    JS::Compartment* compartment() const { return JS::GetCompartmentForRealm(realm_); }
    JS::Compartment* maybeCompartment() const { return compartment(); }
    JS::Realm* realm() const { return realm_; }

    js::SharedScriptData* scriptData() {
        return scriptData_;
    }

    // Script bytecode is immutable after creation.
    jsbytecode* code() const {
        if (!scriptData_) {
            return nullptr;
        }
        return scriptData_->code();
    }
    bool isUncompleted() const {
        // code() becomes non-null only if this script is complete.
        // See the comment in JSScript::fullyInitFromEmitter.
        return !code();
    }

    size_t length() const {
        MOZ_ASSERT(scriptData_);
        return scriptData_->codeLength();
    }

    jsbytecode* codeEnd() const { return code() + length(); }

    jsbytecode* lastPC() const {
        jsbytecode* pc = codeEnd() - js::JSOP_RETRVAL_LENGTH;
        MOZ_ASSERT(*pc == JSOP_RETRVAL);
        return pc;
    }

    bool containsPC(const jsbytecode* pc) const {
        return pc >= code() && pc < codeEnd();
    }

    size_t pcToOffset(const jsbytecode* pc) const {
        MOZ_ASSERT(containsPC(pc));
        return size_t(pc - code());
    }

    jsbytecode* offsetToPC(size_t offset) const {
        MOZ_ASSERT(offset < length());
        return code() + offset;
    }

    size_t mainOffset() const {
        return mainOffset_;
    }

    uint32_t lineno() const {
        return lineno_;
    }

    uint32_t column() const {
        return column_;
    }

    void setColumn(size_t column) { column_ = column; }
=======
  /*
   * Increment or decrement the generator observer count. If the count is
   * non-zero then the script reports resumptions to the debugger.
   *
   * Only incrementing is fallible, as it could allocate a DebugScript.
   */
  bool incrementGeneratorObserverCount(JSContext* cx);
  void decrementGeneratorObserverCount(js::FreeOp* fop);
>>>>>>> upstream-releases

<<<<<<< HEAD
  jsbytecode* lastPC() const {
    jsbytecode* pc = codeEnd() - js::JSOP_RETRVAL_LENGTH;
    MOZ_ASSERT(*pc == JSOP_RETRVAL);
    return pc;
  }

  bool containsPC(const jsbytecode* pc) const {
    return pc >= code() && pc < codeEnd();
  }
||||||| merged common ancestors
    // The fixed part of a stack frame is comprised of vars (in function and
    // module code) and block-scoped locals (in all kinds of code).
    size_t nfixed() const {
        return nfixed_;
    }

    // Number of fixed slots reserved for slots that are always live. Only
    // nonzero for function or module code.
    size_t numAlwaysLiveFixedSlots() const {
        if (bodyScope()->is<js::FunctionScope>()) {
            return bodyScope()->as<js::FunctionScope>().nextFrameSlot();
        }
        if (bodyScope()->is<js::ModuleScope>()) {
            return bodyScope()->as<js::ModuleScope>().nextFrameSlot();
        }
        return 0;
    }
=======
  void finalize(js::FreeOp* fop);
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool contains(const js::BytecodeLocation& loc) const {
    return containsPC(loc.toRawBytecode());
  }
||||||| merged common ancestors
    // Calculate the number of fixed slots that are live at a particular bytecode.
    size_t calculateLiveFixed(jsbytecode* pc);
=======
  static const JS::TraceKind TraceKind = JS::TraceKind::Script;
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t pcToOffset(const jsbytecode* pc) const {
    MOZ_ASSERT(containsPC(pc));
    return size_t(pc - code());
  }
||||||| merged common ancestors
    size_t nslots() const {
        return nslots_;
    }
=======
  void traceChildren(JSTracer* trc);
>>>>>>> upstream-releases

<<<<<<< HEAD
  jsbytecode* offsetToPC(size_t offset) const {
    MOZ_ASSERT(offset < length());
    return code() + offset;
  }
||||||| merged common ancestors
    unsigned numArgs() const {
        if (bodyScope()->is<js::FunctionScope>()) {
            return bodyScope()->as<js::FunctionScope>().numPositionalFormalParameters();
        }
        return 0;
    }
=======
  // A helper class to prevent relazification of the given function's script
  // while it's holding on to it.  This class automatically roots the script.
  class AutoDelazify;
  friend class AutoDelazify;
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t mainOffset() const { return mainOffset_; }
||||||| merged common ancestors
    inline js::Shape* initialEnvironmentShape() const;
=======
  class AutoDelazify {
    JS::RootedScript script_;
    JSContext* cx_;
    bool oldDoNotRelazify_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t lineno() const { return lineno_; }
||||||| merged common ancestors
    bool functionHasParameterExprs() const {
        // Only functions have parameters.
        js::Scope* scope = bodyScope();
        if (!scope->is<js::FunctionScope>()) {
            return false;
        }
        return scope->as<js::FunctionScope>().hasParameterExprs();
    }
=======
   public:
    explicit AutoDelazify(JSContext* cx, JS::HandleFunction fun = nullptr)
        : script_(cx), cx_(cx), oldDoNotRelazify_(false) {
      holdScript(fun);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t column() const { return column_; }
||||||| merged common ancestors
    size_t nTypeSets() const {
        return nTypeSets_;
    }
=======
    ~AutoDelazify() { dropScript(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void setColumn(size_t column) { column_ = column; }
||||||| merged common ancestors
    size_t funLength() const {
        return funLength_;
    }
=======
    void operator=(JS::HandleFunction fun) {
      dropScript();
      holdScript(fun);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The fixed part of a stack frame is comprised of vars (in function and
  // module code) and block-scoped locals (in all kinds of code).
  size_t nfixed() const { return nfixed_; }
||||||| merged common ancestors
    static size_t offsetOfFunLength() {
        return offsetof(JSScript, funLength_);
    }
=======
    operator JS::HandleScript() const { return script_; }
    explicit operator bool() const { return script_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Number of fixed slots reserved for slots that are always live. Only
  // nonzero for function or module code.
  size_t numAlwaysLiveFixedSlots() const {
    if (bodyScope()->is<js::FunctionScope>()) {
      return bodyScope()->as<js::FunctionScope>().nextFrameSlot();
    }
    if (bodyScope()->is<js::ModuleScope>()) {
      return bodyScope()->as<js::ModuleScope>().nextFrameSlot();
    }
    return 0;
  }
||||||| merged common ancestors
    uint32_t sourceStart() const {
        return sourceStart_;
    }

    uint32_t sourceEnd() const {
        return sourceEnd_;
    }
=======
   private:
    void holdScript(JS::HandleFunction fun);
    void dropScript();
  };

  bool trackRecordReplayProgress() const {
    return hasFlag(MutableFlags::TrackRecordReplayProgress);
  }
};
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Calculate the number of fixed slots that are live at a particular bytecode.
  size_t calculateLiveFixed(jsbytecode* pc);
||||||| merged common ancestors
    uint32_t sourceLength() const {
        return sourceEnd_ - sourceStart_;
    }
=======
/* If this fails, add/remove padding within JSScript. */
static_assert(
    sizeof(JSScript) % js::gc::CellAlignBytes == 0,
    "Size of JSScript must be an integral multiple of js::gc::CellAlignBytes");
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t nslots() const { return nslots_; }
||||||| merged common ancestors
    uint32_t toStringStart() const {
        return toStringStart_;
    }
=======
namespace js {
>>>>>>> upstream-releases

<<<<<<< HEAD
  unsigned numArgs() const {
    if (bodyScope()->is<js::FunctionScope>()) {
      return bodyScope()
          ->as<js::FunctionScope>()
          .numPositionalFormalParameters();
    }
    return 0;
  }
||||||| merged common ancestors
    uint32_t toStringEnd() const {
        return toStringEnd_;
    }
=======
// Variable-length data for LazyScripts. Contains vector of inner functions and
// vector of captured property ids.
class alignas(uintptr_t) LazyScriptData final {
 private:
  uint32_t numClosedOverBindings_ = 0;
  uint32_t numInnerFunctions_ = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  inline js::Shape* initialEnvironmentShape() const;
||||||| merged common ancestors
    bool noScriptRval() const {
        return bitFields_.noScriptRval_;
    }
=======
  FieldInitializers fieldInitializers_ = FieldInitializers::Invalid();
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool functionHasParameterExprs() const {
    // Only functions have parameters.
    js::Scope* scope = bodyScope();
    if (!scope->is<js::FunctionScope>()) {
      return false;
    }
    return scope->as<js::FunctionScope>().hasParameterExprs();
  }

  size_t nTypeSets() const { return nTypeSets_; }

  size_t funLength() const { return funLength_; }

  static size_t offsetOfFunLength() { return offsetof(JSScript, funLength_); }

  uint32_t sourceStart() const { return sourceStart_; }

  uint32_t sourceEnd() const { return sourceEnd_; }

  uint32_t sourceLength() const { return sourceEnd_ - sourceStart_; }

  uint32_t toStringStart() const { return toStringStart_; }

  uint32_t toStringEnd() const { return toStringEnd_; }

  bool noScriptRval() const { return hasFlag(ImmutableFlags::NoScriptRval); }

  bool strict() const { return hasFlag(ImmutableFlags::Strict); }

  bool explicitUseStrict() const {
    return hasFlag(ImmutableFlags::ExplicitUseStrict);
  }

  bool hasNonSyntacticScope() const {
    return hasFlag(ImmutableFlags::HasNonSyntacticScope);
  }

  bool selfHosted() const { return hasFlag(ImmutableFlags::SelfHosted); }
  bool bindingsAccessedDynamically() const {
    return hasFlag(ImmutableFlags::BindingsAccessedDynamically);
  }
  bool funHasExtensibleScope() const {
    return hasFlag(ImmutableFlags::FunHasExtensibleScope);
  }
  bool funHasAnyAliasedFormal() const {
    return hasFlag(ImmutableFlags::FunHasAnyAliasedFormal);
  }

  bool hasSingletons() const { return hasFlag(ImmutableFlags::HasSingletons); }
  bool treatAsRunOnce() const {
    return hasFlag(ImmutableFlags::TreatAsRunOnce);
  }
  bool hasRunOnce() const { return hasFlag(MutableFlags::HasRunOnce); }
  bool hasBeenCloned() const { return hasFlag(MutableFlags::HasBeenCloned); }

  void setTreatAsRunOnce() { setFlag(ImmutableFlags::TreatAsRunOnce); }
  void setHasRunOnce() { setFlag(MutableFlags::HasRunOnce); }
  void setHasBeenCloned() { setFlag(MutableFlags::HasBeenCloned); }

  bool isActiveEval() const { return hasFlag(MutableFlags::IsActiveEval); }
  bool isCachedEval() const { return hasFlag(MutableFlags::IsCachedEval); }

  void cacheForEval() {
    MOZ_ASSERT(isActiveEval());
    MOZ_ASSERT(!isCachedEval());
    clearFlag(MutableFlags::IsActiveEval);
    setFlag(MutableFlags::IsCachedEval);
    // IsEvalCacheCandidate will make sure that there's nothing in this
    // script that would prevent reexecution even if isRunOnce is
    // true.  So just pretend like we never ran this script.
    clearFlag(MutableFlags::HasRunOnce);
  }

  void uncacheForEval() {
    MOZ_ASSERT(isCachedEval());
    MOZ_ASSERT(!isActiveEval());
    clearFlag(MutableFlags::IsCachedEval);
    setFlag(MutableFlags::IsActiveEval);
  }

  void setActiveEval() { setFlag(MutableFlags::IsActiveEval); }

  bool isLikelyConstructorWrapper() const {
    return hasFlag(ImmutableFlags::IsLikelyConstructorWrapper);
  }
  void setLikelyConstructorWrapper() {
    setFlag(ImmutableFlags::IsLikelyConstructorWrapper);
  }

  bool failedBoundsCheck() const {
    return hasFlag(MutableFlags::FailedBoundsCheck);
  }
  bool failedShapeGuard() const {
    return hasFlag(MutableFlags::FailedShapeGuard);
  }
  bool hadFrequentBailouts() const {
    return hasFlag(MutableFlags::HadFrequentBailouts);
  }
  bool hadOverflowBailout() const {
    return hasFlag(MutableFlags::HadOverflowBailout);
  }
  bool uninlineable() const { return hasFlag(MutableFlags::Uninlineable); }
  bool invalidatedIdempotentCache() const {
    return hasFlag(MutableFlags::InvalidatedIdempotentCache);
  }
  bool failedLexicalCheck() const {
    return hasFlag(MutableFlags::FailedLexicalCheck);
  }
  bool isDefaultClassConstructor() const {
    return hasFlag(ImmutableFlags::IsDefaultClassConstructor);
  }

  void setFailedBoundsCheck() { setFlag(MutableFlags::FailedBoundsCheck); }
  void setFailedShapeGuard() { setFlag(MutableFlags::FailedShapeGuard); }
  void setHadFrequentBailouts() { setFlag(MutableFlags::HadFrequentBailouts); }
  void setHadOverflowBailout() { setFlag(MutableFlags::HadOverflowBailout); }
  void setUninlineable() { setFlag(MutableFlags::Uninlineable); }
  void setInvalidatedIdempotentCache() {
    setFlag(MutableFlags::InvalidatedIdempotentCache);
  }
  void setFailedLexicalCheck() { setFlag(MutableFlags::FailedLexicalCheck); }
  void setIsDefaultClassConstructor() {
    setFlag(ImmutableFlags::IsDefaultClassConstructor);
  }

  bool hasScriptCounts() const {
    return hasFlag(MutableFlags::HasScriptCounts);
  }
  bool hasScriptName();

  bool hasFreezeConstraints() const {
    return hasFlag(MutableFlags::HasFreezeConstraints);
  }
  void setHasFreezeConstraints() {
    setFlag(MutableFlags::HasFreezeConstraints);
  }

  bool warnedAboutUndefinedProp() const {
    return hasFlag(MutableFlags::WarnedAboutUndefinedProp);
  }
  void setWarnedAboutUndefinedProp() {
    setFlag(MutableFlags::WarnedAboutUndefinedProp);
  }

  /* See ContextFlags::funArgumentsHasLocalBinding comment. */
  bool argumentsHasVarBinding() const {
    return hasFlag(ImmutableFlags::ArgsHasVarBinding);
  }
  void setArgumentsHasVarBinding();
  bool argumentsAliasesFormals() const {
    return argumentsHasVarBinding() && hasMappedArgsObj();
  }

  js::GeneratorKind generatorKind() const {
    return isGenerator() ? js::GeneratorKind::Generator
                         : js::GeneratorKind::NotGenerator;
  }
  bool isGenerator() const { return hasFlag(ImmutableFlags::IsGenerator); }

  js::FunctionAsyncKind asyncKind() const {
    return isAsync() ? js::FunctionAsyncKind::AsyncFunction
                     : js::FunctionAsyncKind::SyncFunction;
  }
  bool isAsync() const { return hasFlag(ImmutableFlags::IsAsync); }

  bool hasRest() const { return hasFlag(ImmutableFlags::HasRest); }

  bool hideScriptFromDebugger() const {
    return hasFlag(MutableFlags::HideScriptFromDebugger);
  }
  void clearHideScriptFromDebugger() {
    clearFlag(MutableFlags::HideScriptFromDebugger);
  }

  bool spewEnabled() const { return hasFlag(MutableFlags::SpewEnabled); }
  void setSpewEnabled(bool enabled) {
    setFlag(MutableFlags::SpewEnabled, enabled);
  }

  bool needsHomeObject() const {
    return hasFlag(ImmutableFlags::NeedsHomeObject);
  }

  bool isDerivedClassConstructor() const {
    return hasFlag(ImmutableFlags::IsDerivedClassConstructor);
  }

  /*
   * As an optimization, even when argsHasLocalBinding, the function prologue
   * may not need to create an arguments object. This is determined by
   * needsArgsObj which is set by AnalyzeArgumentsUsage. When !needsArgsObj,
   * the prologue may simply write MagicValue(JS_OPTIMIZED_ARGUMENTS) to
   * 'arguments's slot and any uses of 'arguments' will be guaranteed to
   * handle this magic value. To avoid spurious arguments object creation, we
   * maintain the invariant that needsArgsObj is only called after the script
   * has been analyzed.
   */
  bool analyzedArgsUsage() const {
    return !hasFlag(MutableFlags::NeedsArgsAnalysis);
  }
  inline bool ensureHasAnalyzedArgsUsage(JSContext* cx);
  bool needsArgsObj() const {
    MOZ_ASSERT(analyzedArgsUsage());
    return hasFlag(MutableFlags::NeedsArgsObj);
  }
  void setNeedsArgsObj(bool needsArgsObj);
  static bool argumentsOptimizationFailed(JSContext* cx,
                                          js::HandleScript script);

  bool hasMappedArgsObj() const {
    return hasFlag(ImmutableFlags::HasMappedArgsObj);
  }

  bool functionHasThisBinding() const {
    return hasFlag(ImmutableFlags::FunctionHasThisBinding);
  }

  /*
   * Arguments access (via JSOP_*ARG* opcodes) must access the canonical
   * location for the argument. If an arguments object exists AND it's mapped
   * ('arguments' aliases formals), then all access must go through the
   * arguments object. Otherwise, the local slot is the canonical location for
   * the arguments. Note: if a formal is aliased through the scope chain, then
   * script->formalIsAliased and JSOP_*ARG* opcodes won't be emitted at all.
   */
  bool argsObjAliasesFormals() const {
    return needsArgsObj() && hasMappedArgsObj();
  }

  uint32_t typesGeneration() const {
    return uint32_t(hasFlag(MutableFlags::TypesGeneration));
  }

  void setTypesGeneration(uint32_t generation) {
    MOZ_ASSERT(generation <= 1);
    setFlag(MutableFlags::TypesGeneration, bool(generation));
  }

  void setDoNotRelazify(bool b) { setFlag(MutableFlags::DoNotRelazify, b); }

  bool hasInnerFunctions() const {
    return hasFlag(ImmutableFlags::HasInnerFunctions);
  }

  static constexpr size_t offsetOfMutableFlags() {
    return offsetof(JSScript, mutableFlags_);
  }

  bool hasAnyIonScript() const { return hasIonScript(); }

  bool hasIonScript() const {
    bool res = ion && ion != ION_DISABLED_SCRIPT &&
               ion != ION_COMPILING_SCRIPT && ion != ION_PENDING_SCRIPT;
    MOZ_ASSERT_IF(res, baseline);
    return res;
  }
  bool canIonCompile() const { return ion != ION_DISABLED_SCRIPT; }
  bool isIonCompilingOffThread() const { return ion == ION_COMPILING_SCRIPT; }

  js::jit::IonScript* ionScript() const {
    MOZ_ASSERT(hasIonScript());
    return ion;
  }
  js::jit::IonScript* maybeIonScript() const { return ion; }
  js::jit::IonScript* const* addressOfIonScript() const { return &ion; }
  void setIonScript(JSRuntime* rt, js::jit::IonScript* ionScript);

  bool hasBaselineScript() const {
    bool res = baseline && baseline != BASELINE_DISABLED_SCRIPT;
    MOZ_ASSERT_IF(!res, !ion || ion == ION_DISABLED_SCRIPT);
    return res;
  }
  bool canBaselineCompile() const {
    return baseline != BASELINE_DISABLED_SCRIPT;
  }
  js::jit::BaselineScript* baselineScript() const {
    MOZ_ASSERT(hasBaselineScript());
    return baseline;
  }
  inline void setBaselineScript(JSRuntime* rt,
                                js::jit::BaselineScript* baselineScript);

  inline js::jit::ICScript* icScript() const;

  bool hasICScript() const {
    // ICScript is stored in TypeScript so we have an ICScript iff we have a
    // TypeScript.
    return !!types_;
  }

  void updateJitCodeRaw(JSRuntime* rt);

  static size_t offsetOfBaselineScript() {
    return offsetof(JSScript, baseline);
  }
  static size_t offsetOfIonScript() { return offsetof(JSScript, ion); }
  static constexpr size_t offsetOfJitCodeRaw() {
    return offsetof(JSScript, jitCodeRaw_);
  }
  static constexpr size_t offsetOfJitCodeSkipArgCheck() {
    return offsetof(JSScript, jitCodeSkipArgCheck_);
  }
  uint8_t* jitCodeRaw() const { return jitCodeRaw_; }

  // We don't relazify functions with a TypeScript or JIT code, but some
  // callers (XDR, testing functions) want to know whether this script is
  // relazifiable ignoring (or after) discarding JIT code.
  bool isRelazifiableIgnoringJitCode() const {
    return (selfHosted() || lazyScript) && !hasInnerFunctions() &&
           !isGenerator() && !isAsync() && !isDefaultClassConstructor() &&
           !hasFlag(MutableFlags::DoNotRelazify);
  }
  bool isRelazifiable() const {
    MOZ_ASSERT_IF(hasBaselineScript() || hasIonScript(), types_);
    return isRelazifiableIgnoringJitCode() && !types_;
  }
  void setLazyScript(js::LazyScript* lazy) { lazyScript = lazy; }
  js::LazyScript* maybeLazyScript() { return lazyScript; }

  /*
   * Original compiled function for the script, if it has a function.
   * nullptr for global and eval scripts.
   * The delazifying variant ensures that the function isn't lazy. The
   * non-delazifying variant must only be used after earlier code has
   * called ensureNonLazyCanonicalFunction and while the function can't
   * have been relazified.
   */
  inline JSFunction* functionDelazifying() const;
  JSFunction* functionNonDelazifying() const {
    if (bodyScope()->is<js::FunctionScope>()) {
      return bodyScope()->as<js::FunctionScope>().canonicalFunction();
    }
    return nullptr;
  }
  /*
   * De-lazifies the canonical function. Must be called before entering code
   * that expects the function to be non-lazy.
   */
  inline void ensureNonLazyCanonicalFunction();

  bool isModule() const { return bodyScope()->is<js::ModuleScope>(); }
  js::ModuleObject* module() const {
    if (isModule()) {
      return bodyScope()->as<js::ModuleScope>().module();
    }
    return nullptr;
  }

  bool isGlobalOrEvalCode() const {
    return bodyScope()->is<js::GlobalScope>() ||
           bodyScope()->is<js::EvalScope>();
  }
  bool isGlobalCode() const { return bodyScope()->is<js::GlobalScope>(); }

  // Returns true if the script may read formal arguments on the stack
  // directly, via lazy arguments or a rest parameter.
  bool mayReadFrameArgsDirectly();

  static JSFlatString* sourceData(JSContext* cx, JS::HandleScript script);

  MOZ_MUST_USE bool appendSourceDataForToString(JSContext* cx,
                                                js::StringBuffer& buf);

  static bool loadSource(JSContext* cx, js::ScriptSource* ss, bool* worked);

  void setSourceObject(JSObject* object);
  JSObject* sourceObject() const { return sourceObject_; }
  js::ScriptSourceObject& scriptSourceUnwrap() const;
  js::ScriptSource* scriptSource() const;
  js::ScriptSource* maybeForwardedScriptSource() const;

  void setDefaultClassConstructorSpan(JSObject* sourceObject, uint32_t start,
                                      uint32_t end, unsigned line,
                                      unsigned column);

  bool mutedErrors() const { return scriptSource()->mutedErrors(); }
  const char* filename() const { return scriptSource()->filename(); }
  const char* maybeForwardedFilename() const {
    return maybeForwardedScriptSource()->filename();
  }

#ifdef MOZ_VTUNE
  // Unique Method ID passed to the VTune profiler. Allows attribution of
  // different jitcode to the same source script.
  uint32_t vtuneMethodID();
#endif

 public:
  /* Return whether this script was compiled for 'eval' */
  bool isForEval() const {
    MOZ_ASSERT_IF(isCachedEval() || isActiveEval(),
                  bodyScope()->is<js::EvalScope>());
    return isCachedEval() || isActiveEval();
  }

  /* Return whether this is a 'direct eval' script in a function scope. */
  bool isDirectEvalInFunction() const {
    if (!isForEval()) {
      return false;
    }
    return bodyScope()->hasOnChain(js::ScopeKind::Function);
  }

  /*
   * Return whether this script is a top-level script.
   *
   * If we evaluate some code which contains a syntax error, then we might
   * produce a JSScript which has no associated bytecode. Testing with
   * |code()| filters out this kind of scripts.
   *
   * If this script has a function associated to it, then it is not the
   * top-level of a file.
   */
  bool isTopLevel() { return code() && !functionNonDelazifying(); }

  /* Ensure the script has a TypeScript. */
  inline bool ensureHasTypes(JSContext* cx, js::AutoKeepTypeScripts&);

  inline js::TypeScript* types(const js::AutoSweepTypeScript& sweep);
  inline bool typesNeedsSweep() const;

  void maybeReleaseTypes();
  void sweepTypes(const js::AutoSweepTypeScript& sweep);

  inline js::GlobalObject& global() const;
  js::GlobalObject& uninlinedGlobal() const;

  uint32_t bodyScopeIndex() const { return bodyScopeIndex_; }

  js::Scope* bodyScope() const { return getScope(bodyScopeIndex_); }

  js::Scope* outermostScope() const {
    // The body scope may not be the outermost scope in the script when
    // the decl env scope is present.
    size_t index = 0;
    return getScope(index);
  }

  bool functionHasExtraBodyVarScope() const {
    bool res = hasFlag(ImmutableFlags::FunctionHasExtraBodyVarScope);
    MOZ_ASSERT_IF(res, functionHasParameterExprs());
    return res;
  }

  js::VarScope* functionExtraBodyVarScope() const {
    MOZ_ASSERT(functionHasExtraBodyVarScope());
    for (js::Scope* scope : scopes()) {
      if (scope->kind() == js::ScopeKind::FunctionBodyVar) {
        return &scope->as<js::VarScope>();
      }
    }
    MOZ_CRASH("Function extra body var scope not found");
  }

  bool needsBodyEnvironment() const {
    for (js::Scope* scope : scopes()) {
      if (ScopeKindIsInBody(scope->kind()) && scope->hasEnvironment()) {
        return true;
      }
    }
    return false;
  }

  inline js::LexicalScope* maybeNamedLambdaScope() const;

  js::Scope* enclosingScope() const { return outermostScope()->enclosing(); }

 private:
  bool makeTypes(JSContext* cx);

  bool createSharedScriptData(JSContext* cx, uint32_t codeLength,
                              uint32_t noteLength, uint32_t natoms);
  bool shareScriptData(JSContext* cx);
  void freeScriptData();
  void setScriptData(js::SharedScriptData* data);

 public:
  uint32_t getWarmUpCount() const { return warmUpCount; }
  uint32_t incWarmUpCounter(uint32_t amount = 1) {
    return warmUpCount += amount;
  }
  uint32_t* addressOfWarmUpCounter() {
    return reinterpret_cast<uint32_t*>(&warmUpCount);
  }
  static size_t offsetOfWarmUpCounter() {
    return offsetof(JSScript, warmUpCount);
  }
  void resetWarmUpCounter() {
    incWarmUpResetCounter();
    warmUpCount = 0;
  }

  uint16_t getWarmUpResetCount() const { return warmUpResetCount; }
  uint16_t incWarmUpResetCounter(uint16_t amount = 1) {
    return warmUpResetCount += amount;
  }
  void resetWarmUpResetCounter() { warmUpResetCount = 0; }

 public:
  bool initScriptCounts(JSContext* cx);
  bool initScriptName(JSContext* cx);
  js::ScriptCounts& getScriptCounts();
  const char* getScriptName();
  js::PCCounts* maybeGetPCCounts(jsbytecode* pc);
  const js::PCCounts* maybeGetThrowCounts(jsbytecode* pc);
  js::PCCounts* getThrowCounts(jsbytecode* pc);
  uint64_t getHitCount(jsbytecode* pc);
  void incHitCount(jsbytecode* pc);  // Used when we bailout out of Ion.
  void addIonCounts(js::jit::IonScriptCounts* ionCounts);
  js::jit::IonScriptCounts* getIonCounts();
  void releaseScriptCounts(js::ScriptCounts* counts);
  void destroyScriptCounts();
  void destroyScriptName();
  void clearHasScriptCounts();
  void resetScriptCounts();

  jsbytecode* main() const { return code() + mainOffset(); }

  js::BytecodeLocation mainLocation() const {
    return js::BytecodeLocation(this, main());
  }

  js::BytecodeLocation endLocation() const {
    return js::BytecodeLocation(this, codeEnd());
  }

  /*
   * computedSizeOfData() is the in-use size of all the data sections.
   * sizeOfData() is the size of the block allocated to hold all the data
   * sections (which can be larger than the in-use size).
   */
  size_t computedSizeOfData() const;
  size_t sizeOfData(mozilla::MallocSizeOf mallocSizeOf) const;
  size_t sizeOfTypeScript(mozilla::MallocSizeOf mallocSizeOf) const;

  size_t dataSize() const { return dataSize_; }

  bool hasConsts() const { return data_->hasConsts(); }
  bool hasObjects() const { return data_->hasObjects(); }
  bool hasTrynotes() const { return data_->hasTryNotes(); }
  bool hasScopeNotes() const { return data_->hasScopeNotes(); }
  bool hasResumeOffsets() const { return data_->hasResumeOffsets(); }

  mozilla::Span<const js::GCPtrScope> scopes() const { return data_->scopes(); }

  mozilla::Span<const js::GCPtrValue> consts() const {
    MOZ_ASSERT(hasConsts());
    return data_->consts();
  }

  mozilla::Span<const js::GCPtrObject> objects() const {
    MOZ_ASSERT(hasObjects());
    return data_->objects();
  }

  mozilla::Span<const JSTryNote> trynotes() const {
    MOZ_ASSERT(hasTrynotes());
    return data_->tryNotes();
  }

  mozilla::Span<const js::ScopeNote> scopeNotes() const {
    MOZ_ASSERT(hasScopeNotes());
    return data_->scopeNotes();
  }

  mozilla::Span<const uint32_t> resumeOffsets() const {
    MOZ_ASSERT(hasResumeOffsets());
    return data_->resumeOffsets();
  }

  uint32_t tableSwitchCaseOffset(jsbytecode* pc, uint32_t caseIndex) const {
    MOZ_ASSERT(containsPC(pc));
    MOZ_ASSERT(*pc == JSOP_TABLESWITCH);
    uint32_t firstResumeIndex = GET_RESUMEINDEX(pc + 3 * JUMP_OFFSET_LEN);
    return resumeOffsets()[firstResumeIndex + caseIndex];
  }
  jsbytecode* tableSwitchCasePC(jsbytecode* pc, uint32_t caseIndex) const {
    return offsetToPC(tableSwitchCaseOffset(pc, caseIndex));
  }

  bool hasLoops();

  uint32_t numNotes() const {
    MOZ_ASSERT(scriptData_);
    return scriptData_->numNotes();
  }
  jssrcnote* notes() const {
    MOZ_ASSERT(scriptData_);
    return scriptData_->notes();
  }

  size_t natoms() const {
    MOZ_ASSERT(scriptData_);
    return scriptData_->natoms();
  }
  js::GCPtrAtom* atoms() const {
    MOZ_ASSERT(scriptData_);
    return scriptData_->atoms();
  }

  js::GCPtrAtom& getAtom(size_t index) const {
    MOZ_ASSERT(index < natoms());
    return atoms()[index];
  }

  js::GCPtrAtom& getAtom(jsbytecode* pc) const {
    MOZ_ASSERT(containsPC(pc) && containsPC(pc + sizeof(uint32_t)));
    MOZ_ASSERT(js::JOF_OPTYPE((JSOp)*pc) == JOF_ATOM);
    return getAtom(GET_UINT32_INDEX(pc));
  }

  js::PropertyName* getName(size_t index) {
    return getAtom(index)->asPropertyName();
  }

  js::PropertyName* getName(jsbytecode* pc) const {
    return getAtom(pc)->asPropertyName();
  }

  JSObject* getObject(size_t index) {
    MOZ_ASSERT(objects()[index]->isTenured());
    return objects()[index];
  }

  JSObject* getObject(jsbytecode* pc) {
    MOZ_ASSERT(containsPC(pc) && containsPC(pc + sizeof(uint32_t)));
    return getObject(GET_UINT32_INDEX(pc));
  }

  js::Scope* getScope(size_t index) const { return scopes()[index]; }

  js::Scope* getScope(jsbytecode* pc) const {
    // This method is used to get a scope directly using a JSOp with an
    // index. To search through ScopeNotes to look for a Scope using pc,
    // use lookupScope.
    MOZ_ASSERT(containsPC(pc) && containsPC(pc + sizeof(uint32_t)));
    MOZ_ASSERT(js::JOF_OPTYPE(JSOp(*pc)) == JOF_SCOPE,
               "Did you mean to use lookupScope(pc)?");
    return getScope(GET_UINT32_INDEX(pc));
  }

  inline JSFunction* getFunction(size_t index);
  JSFunction* function() const {
    if (functionNonDelazifying()) {
      return functionNonDelazifying();
    }
    return nullptr;
  }

  inline js::RegExpObject* getRegExp(size_t index);
  inline js::RegExpObject* getRegExp(jsbytecode* pc);

  const js::Value& getConst(size_t index) { return consts()[index]; }

  // The following 3 functions find the static scope just before the
  // execution of the instruction pointed to by pc.

  js::Scope* lookupScope(jsbytecode* pc);

  js::Scope* innermostScope(jsbytecode* pc);
  js::Scope* innermostScope() { return innermostScope(main()); }

  /*
   * The isEmpty method tells whether this script has code that computes any
   * result (not return value, result AKA normal completion value) other than
   * JSVAL_VOID, or any other effects.
   */
  bool isEmpty() const {
    if (length() > 3) {
      return false;
    }

    jsbytecode* pc = code();
    if (noScriptRval() && JSOp(*pc) == JSOP_FALSE) {
      ++pc;
    }
    return JSOp(*pc) == JSOP_RETRVAL;
  }

  bool formalIsAliased(unsigned argSlot);
  bool formalLivesInArgumentsObject(unsigned argSlot);

 private:
  /* Change this->stepMode to |newValue|. */
  void setNewStepMode(js::FreeOp* fop, uint32_t newValue);

  bool ensureHasDebugScript(JSContext* cx);
  js::DebugScript* debugScript();
  js::DebugScript* releaseDebugScript();
  void destroyDebugScript(js::FreeOp* fop);

  bool hasDebugScript() const { return hasFlag(MutableFlags::HasDebugScript); }

 public:
  bool hasBreakpointsAt(jsbytecode* pc);
  bool hasAnyBreakpointsOrStepMode() { return hasDebugScript(); }

  // See comment above 'debugMode' in Realm.h for explanation of
  // invariants of debuggee compartments, scripts, and frames.
  inline bool isDebuggee() const;

  js::BreakpointSite* getBreakpointSite(jsbytecode* pc) {
    return hasDebugScript() ? debugScript()->breakpoints[pcToOffset(pc)]
                            : nullptr;
  }

  js::BreakpointSite* getOrCreateBreakpointSite(JSContext* cx, jsbytecode* pc);

  void destroyBreakpointSite(js::FreeOp* fop, jsbytecode* pc);

  void clearBreakpointsIn(js::FreeOp* fop, js::Debugger* dbg,
                          JSObject* handler);

  /*
   * Increment or decrement the single-step count. If the count is non-zero
   * then the script is in single-step mode.
   *
   * Only incrementing is fallible, as it could allocate a DebugScript.
   */
  bool incrementStepModeCount(JSContext* cx);
  void decrementStepModeCount(js::FreeOp* fop);

  bool stepModeEnabled() {
    return hasDebugScript() && !!debugScript()->stepMode;
  }

#ifdef DEBUG
  uint32_t stepModeCount() {
    return hasDebugScript() ? debugScript()->stepMode : 0;
  }
#endif

  void finalize(js::FreeOp* fop);

  static const JS::TraceKind TraceKind = JS::TraceKind::Script;

  void traceChildren(JSTracer* trc);

  // A helper class to prevent relazification of the given function's script
  // while it's holding on to it.  This class automatically roots the script.
  class AutoDelazify;
  friend class AutoDelazify;

  class AutoDelazify {
    JS::RootedScript script_;
    JSContext* cx_;
    bool oldDoNotRelazify_;

   public:
    explicit AutoDelazify(JSContext* cx, JS::HandleFunction fun = nullptr)
        : script_(cx), cx_(cx), oldDoNotRelazify_(false) {
      holdScript(fun);
    }

    ~AutoDelazify() { dropScript(); }

    void operator=(JS::HandleFunction fun) {
      dropScript();
      holdScript(fun);
    }

    operator JS::HandleScript() const { return script_; }
    explicit operator bool() const { return script_; }

   private:
    void holdScript(JS::HandleFunction fun);
    void dropScript();
  };

  // Return whether the record/replay execution progress counter
  // (see RecordReplay.h) should be updated as this script runs.
  inline bool trackRecordReplayProgress() const;
};

/* If this fails, add/remove padding within JSScript. */
static_assert(
    sizeof(JSScript) % js::gc::CellAlignBytes == 0,
    "Size of JSScript must be an integral multiple of js::gc::CellAlignBytes");

namespace js {

// Information about a script which may be (or has been) lazily compiled to
// bytecode from its source.
class LazyScript : public gc::TenuredCell {
 private:
  // If non-nullptr, the script has been compiled and this is a forwarding
  // pointer to the result. This is a weak pointer: after relazification, we
  // can collect the script if there are no other pointers to it.
  WeakRef<JSScript*> script_;

  // Original function with which the lazy script is associated.
  GCPtrFunction function_;

  // This field holds one of:
  //   * LazyScript in which the script is nested.  This case happens if the
  //     enclosing script is lazily parsed and have never been compiled.
  //
  //     This is used by the debugger to delazify the enclosing scripts
  //     recursively.  The all ancestor LazyScripts in this linked-list are
  //     kept alive as long as this LazyScript is alive, which doesn't result
  //     in keeping them unnecessarily alive outside of the debugger for the
  //     following reasons:
  //
  //       * Outside of the debugger, a LazyScript is visible to user (which
  //         means the LazyScript can be pointed from somewhere else than the
  //         enclosing script) only if the enclosing script is compiled and
  //         executed.  While compiling the enclosing script, this field is
  //         changed to point the enclosing scope.  So the enclosing
  //         LazyScript is no more in the list.
  //       * Before the enclosing script gets compiled, this LazyScript is
  //         kept alive only if the outermost LazyScript in the list is kept
  //         alive.
  //       * Once this field is changed to point the enclosing scope, this
  //         field will never point the enclosing LazyScript again, since
  //         relazification is not performed on non-leaf scripts.
  //
  //   * Scope in which the script is nested.  This case happens if the
  //     enclosing script has ever been compiled.
  //
  //   * nullptr for incomplete (initial or failure) state
  //
  // This field should be accessed via accessors:
  //   * enclosingScope
  //   * setEnclosingScope (cannot be called twice)
  //   * enclosingLazyScript
  //   * setEnclosingLazyScript (cannot be called twice)
  // after checking:
  //   * hasEnclosingLazyScript
  //   * hasEnclosingScope
  //
  // The transition of fields are following:
  //
  //  o                               o
  //  | when function is lazily       | when decoded from XDR,
  //  | parsed inside a function      | and enclosing script is lazy
  //  | which is lazily parsed        | (CreateForXDR without enclosingScope)
  //  | (Create)                      |
  //  v                               v
  // +---------+                     +---------+
  // | nullptr |                     | nullptr |
  // +---------+                     +---------+
  //  |                               |
  //  | when enclosing function is    | when enclosing script is decoded
  //  | lazily parsed and this        | and this script's function is put
  //  | script's function is put      | into innerFunctions()
  //  | into innerFunctions()         | (setEnclosingLazyScript)
  //  | (setEnclosingLazyScript)      |
  //  |                               |
  //  |                               |     o
  //  |                               |     | when function is lazily
  //  |                               |     | parsed inside a function
  //  |                               |     | which is eagerly parsed
  //  |                               |     | (Create)
  //  v                               |     v
  // +----------------------+         |    +---------+
  // | enclosing LazyScript |<--------+    | nullptr |
  // +----------------------+              +---------+
  //  |                                     |
  //  v                                     |
  //  +<------------------------------------+
  //  |
  //  | when the enclosing script     o
  //  | is successfully compiled      | when decoded from XDR,
  //  | (setEnclosingScope)           | and enclosing script is not lazy
  //  v                               | (CreateForXDR with enclosingScope)
  // +-----------------+              |
  // | enclosing Scope |<-------------+
  // +-----------------+
  GCPtr<TenuredCell*> enclosingLazyScriptOrScope_;

  // ScriptSourceObject. We leave this set to nullptr until we generate
  // bytecode for our immediate parent. This is never a CCW; we don't clone
  // LazyScripts into other compartments.
  GCPtrObject sourceObject_;

  // Heap allocated table with any free variables or inner functions.
  void* table_;

 private:
  static const uint32_t NumClosedOverBindingsBits = 20;
  static const uint32_t NumInnerFunctionsBits = 20;

  struct PackedView {
    uint32_t shouldDeclareArguments : 1;
    uint32_t hasThisBinding : 1;
    uint32_t isAsync : 1;
    uint32_t isBinAST : 1;

    uint32_t numClosedOverBindings : NumClosedOverBindingsBits;

    // -- 32bit boundary --

    uint32_t numInnerFunctions : NumInnerFunctionsBits;

    // N.B. These are booleans but need to be uint32_t to pack correctly on
    // MSVC. If you add another boolean here, make sure to initialize it in
    // LazyScript::Create().
    uint32_t isGenerator : 1;
    uint32_t strict : 1;
    uint32_t bindingsAccessedDynamically : 1;
    uint32_t hasDebuggerStatement : 1;
    uint32_t hasDirectEval : 1;
    uint32_t isLikelyConstructorWrapper : 1;
    uint32_t hasBeenCloned : 1;
    uint32_t treatAsRunOnce : 1;
    uint32_t isDerivedClassConstructor : 1;
    uint32_t needsHomeObject : 1;
    uint32_t hasRest : 1;
    uint32_t parseGoal : 1;
  };

  union {
    PackedView p_;
    uint64_t packedFields_;
  };

  // Source location for the script.
  // See the comment in JSScript for the details
  uint32_t sourceStart_;
  uint32_t sourceEnd_;
  uint32_t toStringStart_;
  uint32_t toStringEnd_;
  // Line and column of |begin_| position, that is the position where we
  // start parsing.
  uint32_t lineno_;
  uint32_t column_;

  LazyScript(JSFunction* fun, ScriptSourceObject& sourceObject, void* table,
             uint64_t packedFields, uint32_t begin, uint32_t end,
             uint32_t toStringStart, uint32_t lineno, uint32_t column);

  // Create a LazyScript without initializing the closedOverBindings and the
  // innerFunctions. To be GC-safe, the caller must initialize both vectors
  // with valid atoms and functions.
  static LazyScript* CreateRaw(JSContext* cx, HandleFunction fun,
                               HandleScriptSourceObject sourceObject,
                               uint64_t packedData, uint32_t begin,
                               uint32_t end, uint32_t toStringStart,
                               uint32_t lineno, uint32_t column);

 public:
  static const uint32_t NumClosedOverBindingsLimit =
      1 << NumClosedOverBindingsBits;
  static const uint32_t NumInnerFunctionsLimit = 1 << NumInnerFunctionsBits;

  // Create a LazyScript and initialize closedOverBindings and innerFunctions
  // with the provided vectors.
  static LazyScript* Create(JSContext* cx, HandleFunction fun,
                            HandleScriptSourceObject sourceObject,
                            const frontend::AtomVector& closedOverBindings,
                            Handle<GCVector<JSFunction*, 8>> innerFunctions,
                            uint32_t begin, uint32_t end,
                            uint32_t toStringStart, uint32_t lineno,
                            uint32_t column, frontend::ParseGoal parseGoal);

  // Create a LazyScript and initialize the closedOverBindings and the
  // innerFunctions with dummy values to be replaced in a later initialization
  // phase.
  //
  // The "script" argument to this function can be null.  If it's non-null,
  // then this LazyScript should be associated with the given JSScript.
  //
  // The sourceObject and enclosingScope arguments may be null if the
  // enclosing function is also lazy.
  static LazyScript* CreateForXDR(JSContext* cx, HandleFunction fun,
                                  HandleScript script,
                                  HandleScope enclosingScope,
                                  HandleScriptSourceObject sourceObject,
                                  uint64_t packedData, uint32_t begin,
                                  uint32_t end, uint32_t toStringStart,
                                  uint32_t lineno, uint32_t column);

  void initRuntimeFields(uint64_t packedFields);

  static inline JSFunction* functionDelazifying(JSContext* cx,
                                                Handle<LazyScript*>);
  JSFunction* functionNonDelazifying() const { return function_; }

  JS::Compartment* compartment() const;
  JS::Compartment* maybeCompartment() const { return compartment(); }
  Realm* realm() const;

  void initScript(JSScript* script);

  JSScript* maybeScript() { return script_; }
  const JSScript* maybeScriptUnbarriered() const {
    return script_.unbarrieredGet();
  }
  bool hasScript() const { return bool(script_); }

  bool hasEnclosingScope() const {
    return enclosingLazyScriptOrScope_ &&
           enclosingLazyScriptOrScope_->is<Scope>();
  }
  bool hasEnclosingLazyScript() const {
    return enclosingLazyScriptOrScope_ &&
           enclosingLazyScriptOrScope_->is<LazyScript>();
  }

  LazyScript* enclosingLazyScript() const {
    MOZ_ASSERT(hasEnclosingLazyScript());
    return enclosingLazyScriptOrScope_->as<LazyScript>();
  }
  void setEnclosingLazyScript(LazyScript* enclosingLazyScript);

  Scope* enclosingScope() const {
    MOZ_ASSERT(hasEnclosingScope());
    return enclosingLazyScriptOrScope_->as<Scope>();
  }
  void setEnclosingScope(Scope* enclosingScope);

  bool hasNonSyntacticScope() const {
    return enclosingScope()->hasOnChain(ScopeKind::NonSyntactic);
  }

  ScriptSourceObject& sourceObject() const;
  ScriptSource* scriptSource() const { return sourceObject().source(); }
  ScriptSource* maybeForwardedScriptSource() const;
  bool mutedErrors() const { return scriptSource()->mutedErrors(); }

  uint32_t numClosedOverBindings() const { return p_.numClosedOverBindings; }
  JSAtom** closedOverBindings() { return (JSAtom**)table_; }

  uint32_t numInnerFunctions() const { return p_.numInnerFunctions; }
  GCPtrFunction* innerFunctions() {
    return (GCPtrFunction*)&closedOverBindings()[numClosedOverBindings()];
  }

  GeneratorKind generatorKind() const {
    return p_.isGenerator ? GeneratorKind::Generator
                          : GeneratorKind::NotGenerator;
  }

  bool isGenerator() const {
    return generatorKind() == GeneratorKind::Generator;
  }

  void setGeneratorKind(GeneratorKind kind) {
    // A script only gets its generator kind set as part of initialization,
    // so it can only transition from NotGenerator.
    MOZ_ASSERT(!isGenerator());
    p_.isGenerator = kind == GeneratorKind::Generator;
  }

  FunctionAsyncKind asyncKind() const {
    return p_.isAsync ? FunctionAsyncKind::AsyncFunction
                      : FunctionAsyncKind::SyncFunction;
  }
  bool isAsync() const { return p_.isAsync; }

  void setAsyncKind(FunctionAsyncKind kind) {
    p_.isAsync = kind == FunctionAsyncKind::AsyncFunction;
  }

  bool hasRest() const { return p_.hasRest; }
  void setHasRest() { p_.hasRest = true; }

  frontend::ParseGoal parseGoal() const {
    return frontend::ParseGoal(p_.parseGoal);
  }

  bool isBinAST() const { return p_.isBinAST; }
  void setIsBinAST() { p_.isBinAST = true; }

  bool strict() const { return p_.strict; }
  void setStrict() { p_.strict = true; }

  bool bindingsAccessedDynamically() const {
    return p_.bindingsAccessedDynamically;
  }
  void setBindingsAccessedDynamically() {
    p_.bindingsAccessedDynamically = true;
  }

  bool hasDebuggerStatement() const { return p_.hasDebuggerStatement; }
  void setHasDebuggerStatement() { p_.hasDebuggerStatement = true; }

  bool hasDirectEval() const { return p_.hasDirectEval; }
  void setHasDirectEval() { p_.hasDirectEval = true; }

  bool isLikelyConstructorWrapper() const {
    return p_.isLikelyConstructorWrapper;
  }
  void setLikelyConstructorWrapper() { p_.isLikelyConstructorWrapper = true; }

  bool hasBeenCloned() const { return p_.hasBeenCloned; }
  void setHasBeenCloned() { p_.hasBeenCloned = true; }

  bool treatAsRunOnce() const { return p_.treatAsRunOnce; }
  void setTreatAsRunOnce() { p_.treatAsRunOnce = true; }

  bool isDerivedClassConstructor() const {
    return p_.isDerivedClassConstructor;
  }
  void setIsDerivedClassConstructor() { p_.isDerivedClassConstructor = true; }

  bool needsHomeObject() const { return p_.needsHomeObject; }
  void setNeedsHomeObject() { p_.needsHomeObject = true; }

  bool shouldDeclareArguments() const { return p_.shouldDeclareArguments; }
  void setShouldDeclareArguments() { p_.shouldDeclareArguments = true; }

  bool hasThisBinding() const { return p_.hasThisBinding; }
  void setHasThisBinding() { p_.hasThisBinding = true; }

  const char* filename() const { return scriptSource()->filename(); }
  uint32_t sourceStart() const { return sourceStart_; }
  uint32_t sourceEnd() const { return sourceEnd_; }
  uint32_t sourceLength() const { return sourceEnd_ - sourceStart_; }
  uint32_t toStringStart() const { return toStringStart_; }
  uint32_t toStringEnd() const { return toStringEnd_; }
  uint32_t lineno() const { return lineno_; }
  uint32_t column() const { return column_; }

  void setToStringEnd(uint32_t toStringEnd) {
    MOZ_ASSERT(toStringStart_ <= toStringEnd);
    MOZ_ASSERT(toStringEnd_ >= sourceEnd_);
    toStringEnd_ = toStringEnd;
  }

  // Returns true if the enclosing script has ever been compiled.
  // Once the enclosing script is compiled, the scope chain is created.
  // This LazyScript is delazify-able as long as it has the enclosing scope,
  // even if the enclosing JSScript is GCed.
  // The enclosing JSScript can be GCed later if the enclosing scope is not
  // FunctionScope or ModuleScope.
  bool enclosingScriptHasEverBeenCompiled() const {
    return hasEnclosingScope();
  }

  friend class GCMarker;
  void traceChildren(JSTracer* trc);
  void finalize(js::FreeOp* fop);

  static const JS::TraceKind TraceKind = JS::TraceKind::LazyScript;

  size_t sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf) {
    return mallocSizeOf(table_);
  }

  uint64_t packedFields() const { return packedFields_; }
};

/* If this fails, add/remove padding within LazyScript. */
static_assert(sizeof(LazyScript) % js::gc::CellAlignBytes == 0,
              "Size of LazyScript must be an integral multiple of "
              "js::gc::CellAlignBytes");

struct ScriptAndCounts {
  /* This structure is stored and marked from the JSRuntime. */
  JSScript* script;
  ScriptCounts scriptCounts;

  inline explicit ScriptAndCounts(JSScript* script);
  inline ScriptAndCounts(ScriptAndCounts&& sac);

  const PCCounts* maybeGetPCCounts(jsbytecode* pc) const {
    return scriptCounts.maybeGetPCCounts(script->pcToOffset(pc));
  }
  const PCCounts* maybeGetThrowCounts(jsbytecode* pc) const {
    return scriptCounts.maybeGetThrowCounts(script->pcToOffset(pc));
  }

  jit::IonScriptCounts* getIonCounts() const { return scriptCounts.ionCounts_; }

  void trace(JSTracer* trc) {
    TraceRoot(trc, &script, "ScriptAndCounts::script");
  }
||||||| merged common ancestors
    bool strict() const {
        return bitFields_.strict_;
    }

    bool explicitUseStrict() const { return bitFields_.explicitUseStrict_; }

    bool hasNonSyntacticScope() const {
        return bitFields_.hasNonSyntacticScope_;
    }

    bool selfHosted() const { return bitFields_.selfHosted_; }
    bool bindingsAccessedDynamically() const { return bitFields_.bindingsAccessedDynamically_; }
    bool funHasExtensibleScope() const {
        return bitFields_.funHasExtensibleScope_;
    }
    bool funHasAnyAliasedFormal() const {
        return bitFields_.funHasAnyAliasedFormal_;
    }

    bool hasSingletons() const { return bitFields_.hasSingletons_; }
    bool treatAsRunOnce() const {
        return bitFields_.treatAsRunOnce_;
    }
    bool hasRunOnce() const { return bitFields_.hasRunOnce_; }
    bool hasBeenCloned() const { return bitFields_.hasBeenCloned_; }

    void setTreatAsRunOnce() { bitFields_.treatAsRunOnce_ = true; }
    void setHasRunOnce() { bitFields_.hasRunOnce_ = true; }
    void setHasBeenCloned() { bitFields_.hasBeenCloned_ = true; }

    bool isActiveEval() const { return bitFields_.isActiveEval_; }
    bool isCachedEval() const { return bitFields_.isCachedEval_; }

    void cacheForEval() {
        MOZ_ASSERT(isActiveEval());
        MOZ_ASSERT(!isCachedEval());
        bitFields_.isActiveEval_ = false;
        bitFields_.isCachedEval_ = true;
        // IsEvalCacheCandidate will make sure that there's nothing in this
        // script that would prevent reexecution even if isRunOnce is
        // true.  So just pretend like we never ran this script.
        bitFields_.hasRunOnce_ = false;
    }

    void uncacheForEval() {
        MOZ_ASSERT(isCachedEval());
        MOZ_ASSERT(!isActiveEval());
        bitFields_.isCachedEval_ = false;
        bitFields_.isActiveEval_ = true;
    }

    void setActiveEval() { bitFields_.isActiveEval_ = true; }

    bool isLikelyConstructorWrapper() const {
        return bitFields_.isLikelyConstructorWrapper_;
    }
    void setLikelyConstructorWrapper() { bitFields_.isLikelyConstructorWrapper_ = true; }

    bool failedBoundsCheck() const {
        return bitFields_.failedBoundsCheck_;
    }
    bool failedShapeGuard() const {
        return bitFields_.failedShapeGuard_;
    }
    bool hadFrequentBailouts() const {
        return bitFields_.hadFrequentBailouts_;
    }
    bool hadOverflowBailout() const {
        return bitFields_.hadOverflowBailout_;
    }
    bool uninlineable() const {
        return bitFields_.uninlineable_;
    }
    bool invalidatedIdempotentCache() const {
        return bitFields_.invalidatedIdempotentCache_;
    }
    bool failedLexicalCheck() const {
        return bitFields_.failedLexicalCheck_;
    }
    bool isDefaultClassConstructor() const {
        return bitFields_.isDefaultClassConstructor_;
    }

    void setFailedBoundsCheck() { bitFields_.failedBoundsCheck_ = true; }
    void setFailedShapeGuard() { bitFields_.failedShapeGuard_ = true; }
    void setHadFrequentBailouts() { bitFields_.hadFrequentBailouts_ = true; }
    void setHadOverflowBailout() { bitFields_.hadOverflowBailout_ = true; }
    void setUninlineable() { bitFields_.uninlineable_ = true; }
    void setInvalidatedIdempotentCache() { bitFields_.invalidatedIdempotentCache_ = true; }
    void setFailedLexicalCheck() { bitFields_.failedLexicalCheck_ = true; }
    void setIsDefaultClassConstructor() { bitFields_.isDefaultClassConstructor_ = true; }

    bool hasScriptCounts() const { return bitFields_.hasScriptCounts_; }
    bool hasScriptName();

    bool hasFreezeConstraints() const { return bitFields_.hasFreezeConstraints_; }
    void setHasFreezeConstraints() { bitFields_.hasFreezeConstraints_ = true; }

    bool warnedAboutUndefinedProp() const { return bitFields_.warnedAboutUndefinedProp_; }
    void setWarnedAboutUndefinedProp() { bitFields_.warnedAboutUndefinedProp_ = true; }

    /* See ContextFlags::funArgumentsHasLocalBinding comment. */
    bool argumentsHasVarBinding() const {
        return bitFields_.argsHasVarBinding_;
    }
    void setArgumentsHasVarBinding();
    bool argumentsAliasesFormals() const {
        return argumentsHasVarBinding() && hasMappedArgsObj();
    }

    js::GeneratorKind generatorKind() const {
        return bitFields_.isGenerator_ ? js::GeneratorKind::Generator : js::GeneratorKind::NotGenerator;
    }
    bool isGenerator() const { return bitFields_.isGenerator_; }
    void setGeneratorKind(js::GeneratorKind kind) {
        // A script only gets its generator kind set as part of initialization,
        // so it can only transition from not being a generator.
        MOZ_ASSERT(!isGenerator());
        bitFields_.isGenerator_ = kind == js::GeneratorKind::Generator;
    }

    js::FunctionAsyncKind asyncKind() const {
        return bitFields_.isAsync_
               ? js::FunctionAsyncKind::AsyncFunction
               : js::FunctionAsyncKind::SyncFunction;
    }
    bool isAsync() const {
        return bitFields_.isAsync_;
    }

    void setAsyncKind(js::FunctionAsyncKind kind) {
        bitFields_.isAsync_ = kind == js::FunctionAsyncKind::AsyncFunction;
    }

    bool hasRest() const {
        return bitFields_.hasRest_;
    }
    void setHasRest() {
        bitFields_.hasRest_ = true;
    }

    bool hideScriptFromDebugger() const {
        return bitFields_.hideScriptFromDebugger_;
    }
    void clearHideScriptFromDebugger() {
        bitFields_.hideScriptFromDebugger_ = false;
    }

    void setNeedsHomeObject() {
        bitFields_.needsHomeObject_ = true;
    }
    bool needsHomeObject() const {
        return bitFields_.needsHomeObject_;
    }

    bool isDerivedClassConstructor() const {
        return bitFields_.isDerivedClassConstructor_;
    }

    /*
     * As an optimization, even when argsHasLocalBinding, the function prologue
     * may not need to create an arguments object. This is determined by
     * needsArgsObj which is set by AnalyzeArgumentsUsage. When !needsArgsObj,
     * the prologue may simply write MagicValue(JS_OPTIMIZED_ARGUMENTS) to
     * 'arguments's slot and any uses of 'arguments' will be guaranteed to
     * handle this magic value. To avoid spurious arguments object creation, we
     * maintain the invariant that needsArgsObj is only called after the script
     * has been analyzed.
     */
    bool analyzedArgsUsage() const { return !bitFields_.needsArgsAnalysis_; }
    inline bool ensureHasAnalyzedArgsUsage(JSContext* cx);
    bool needsArgsObj() const {
        MOZ_ASSERT(analyzedArgsUsage());
        return bitFields_.needsArgsObj_;
    }
    void setNeedsArgsObj(bool needsArgsObj);
    static bool argumentsOptimizationFailed(JSContext* cx, js::HandleScript script);

    bool hasMappedArgsObj() const {
        return bitFields_.hasMappedArgsObj_;
    }

    bool functionHasThisBinding() const {
        return bitFields_.functionHasThisBinding_;
    }

    /*
     * Arguments access (via JSOP_*ARG* opcodes) must access the canonical
     * location for the argument. If an arguments object exists AND it's mapped
     * ('arguments' aliases formals), then all access must go through the
     * arguments object. Otherwise, the local slot is the canonical location for
     * the arguments. Note: if a formal is aliased through the scope chain, then
     * script->formalIsAliased and JSOP_*ARG* opcodes won't be emitted at all.
     */
    bool argsObjAliasesFormals() const {
        return needsArgsObj() && hasMappedArgsObj();
    }

    uint32_t typesGeneration() const {
        return (uint32_t) bitFields_.typesGeneration_;
    }

    void setTypesGeneration(uint32_t generation) {
        MOZ_ASSERT(generation <= 1);
        bitFields_.typesGeneration_ = (bool) generation;
    }

    void setDoNotRelazify(bool b) {
        bitFields_.doNotRelazify_ = b;
    }

    void setHasInnerFunctions(bool b) {
        bitFields_.hasInnerFunctions_ = b;
    }

    bool hasInnerFunctions() const {
        return bitFields_.hasInnerFunctions_;
    }

    bool hasAnyIonScript() const {
        return hasIonScript();
    }

    bool hasIonScript() const {
        bool res = ion && ion != ION_DISABLED_SCRIPT && ion != ION_COMPILING_SCRIPT &&
                          ion != ION_PENDING_SCRIPT;
        MOZ_ASSERT_IF(res, baseline);
        return res;
    }
    bool canIonCompile() const {
        return ion != ION_DISABLED_SCRIPT;
    }
    bool isIonCompilingOffThread() const {
        return ion == ION_COMPILING_SCRIPT;
    }

    js::jit::IonScript* ionScript() const {
        MOZ_ASSERT(hasIonScript());
        return ion;
    }
    js::jit::IonScript* maybeIonScript() const {
        return ion;
    }
    js::jit::IonScript* const* addressOfIonScript() const {
        return &ion;
    }
    void setIonScript(JSRuntime* rt, js::jit::IonScript* ionScript);

    bool hasBaselineScript() const {
        bool res = baseline && baseline != BASELINE_DISABLED_SCRIPT;
        MOZ_ASSERT_IF(!res, !ion || ion == ION_DISABLED_SCRIPT);
        return res;
    }
    bool canBaselineCompile() const {
        return baseline != BASELINE_DISABLED_SCRIPT;
    }
    js::jit::BaselineScript* baselineScript() const {
        MOZ_ASSERT(hasBaselineScript());
        return baseline;
    }
    inline void setBaselineScript(JSRuntime* rt, js::jit::BaselineScript* baselineScript);

    void updateJitCodeRaw(JSRuntime* rt);

    static size_t offsetOfBaselineScript() {
        return offsetof(JSScript, baseline);
    }
    static size_t offsetOfIonScript() {
        return offsetof(JSScript, ion);
    }
    static constexpr size_t offsetOfJitCodeRaw() {
        return offsetof(JSScript, jitCodeRaw_);
    }
    static constexpr size_t offsetOfJitCodeSkipArgCheck() {
        return offsetof(JSScript, jitCodeSkipArgCheck_);
    }
    uint8_t* jitCodeRaw() const {
        return jitCodeRaw_;
    }

    bool isRelazifiable() const {
        return (selfHosted() || lazyScript) && !bitFields_.hasInnerFunctions_ && !types_ &&
               !isGenerator() && !isAsync() &&
               !isDefaultClassConstructor() &&
               !hasBaselineScript() && !hasAnyIonScript() &&
               !bitFields_.doNotRelazify_;
    }
    void setLazyScript(js::LazyScript* lazy) {
        lazyScript = lazy;
    }
    js::LazyScript* maybeLazyScript() {
        return lazyScript;
    }

    /*
     * Original compiled function for the script, if it has a function.
     * nullptr for global and eval scripts.
     * The delazifying variant ensures that the function isn't lazy. The
     * non-delazifying variant must only be used after earlier code has
     * called ensureNonLazyCanonicalFunction and while the function can't
     * have been relazified.
     */
    inline JSFunction* functionDelazifying() const;
    JSFunction* functionNonDelazifying() const {
        if (bodyScope()->is<js::FunctionScope>()) {
            return bodyScope()->as<js::FunctionScope>().canonicalFunction();
        }
        return nullptr;
    }
    /*
     * De-lazifies the canonical function. Must be called before entering code
     * that expects the function to be non-lazy.
     */
    inline void ensureNonLazyCanonicalFunction();

    bool isModule() const { return bodyScope()->is<js::ModuleScope>(); }
    js::ModuleObject* module() const {
        if (isModule()) {
            return bodyScope()->as<js::ModuleScope>().module();
        }
        return nullptr;
    }

    bool isGlobalOrEvalCode() const {
        return bodyScope()->is<js::GlobalScope>() || bodyScope()->is<js::EvalScope>();
    }
    bool isGlobalCode() const {
        return bodyScope()->is<js::GlobalScope>();
    }

    // Returns true if the script may read formal arguments on the stack
    // directly, via lazy arguments or a rest parameter.
    bool mayReadFrameArgsDirectly();

    static JSFlatString* sourceData(JSContext* cx, JS::HandleScript script);

    MOZ_MUST_USE bool appendSourceDataForToString(JSContext* cx, js::StringBuffer& buf);

    static bool loadSource(JSContext* cx, js::ScriptSource* ss, bool* worked);

    void setSourceObject(JSObject* object);
    JSObject* sourceObject() const {
        return sourceObject_;
    }
    js::ScriptSourceObject& scriptSourceUnwrap() const;
    js::ScriptSource* scriptSource() const;
    js::ScriptSource* maybeForwardedScriptSource() const;

    void setDefaultClassConstructorSpan(JSObject* sourceObject, uint32_t start, uint32_t end,
                                        unsigned line, unsigned column);

    bool mutedErrors() const { return scriptSource()->mutedErrors(); }
    const char* filename() const { return scriptSource()->filename(); }
    const char* maybeForwardedFilename() const { return maybeForwardedScriptSource()->filename(); }

#ifdef MOZ_VTUNE
    uint32_t vtuneMethodID() const { return vtuneMethodId_; }
#endif

  public:

    /* Return whether this script was compiled for 'eval' */
    bool isForEval() const {
        MOZ_ASSERT_IF(isCachedEval() || isActiveEval(), bodyScope()->is<js::EvalScope>());
        return isCachedEval() || isActiveEval();
    }

    /* Return whether this is a 'direct eval' script in a function scope. */
    bool isDirectEvalInFunction() const {
        if (!isForEval()) {
            return false;
        }
        return bodyScope()->hasOnChain(js::ScopeKind::Function);
    }

    /*
     * Return whether this script is a top-level script.
     *
     * If we evaluate some code which contains a syntax error, then we might
     * produce a JSScript which has no associated bytecode. Testing with
     * |code()| filters out this kind of scripts.
     *
     * If this script has a function associated to it, then it is not the
     * top-level of a file.
     */
    bool isTopLevel() { return code() && !functionNonDelazifying(); }

    /* Ensure the script has a TypeScript. */
    inline bool ensureHasTypes(JSContext* cx, js::AutoKeepTypeScripts&);

    inline js::TypeScript* types(const js::AutoSweepTypeScript& sweep);

    inline bool typesNeedsSweep() const;

    void sweepTypes(const js::AutoSweepTypeScript& sweep);

    inline js::GlobalObject& global() const;
    js::GlobalObject& uninlinedGlobal() const;

    uint32_t bodyScopeIndex() const {
        return bodyScopeIndex_;
    }

    js::Scope* bodyScope() const {
        return getScope(bodyScopeIndex_);
    }

    js::Scope* outermostScope() const {
        // The body scope may not be the outermost scope in the script when
        // the decl env scope is present.
        size_t index = 0;
        return getScope(index);
    }

    bool functionHasExtraBodyVarScope() const {
        MOZ_ASSERT_IF(bitFields_.functionHasExtraBodyVarScope_, functionHasParameterExprs());
        return bitFields_.functionHasExtraBodyVarScope_;
    }

    js::VarScope* functionExtraBodyVarScope() const {
        MOZ_ASSERT(functionHasExtraBodyVarScope());
        for (js::Scope* scope : scopes()) {
            if (scope->kind() == js::ScopeKind::FunctionBodyVar) {
                return &scope->as<js::VarScope>();
            }
        }
        MOZ_CRASH("Function extra body var scope not found");
    }

    bool needsBodyEnvironment() const {
        for (js::Scope* scope : scopes()) {
            if (ScopeKindIsInBody(scope->kind()) && scope->hasEnvironment()) {
                return true;
            }
        }
        return false;
    }

    inline js::LexicalScope* maybeNamedLambdaScope() const;

    js::Scope* enclosingScope() const {
        return outermostScope()->enclosing();
    }

  private:
    bool makeTypes(JSContext* cx);

    bool createScriptData(JSContext* cx, uint32_t codeLength, uint32_t srcnotesLength,
                          uint32_t natoms);
    bool shareScriptData(JSContext* cx);
    void freeScriptData();
    void setScriptData(js::SharedScriptData* data);

  public:
    uint32_t getWarmUpCount() const { return warmUpCount; }
    uint32_t incWarmUpCounter(uint32_t amount = 1) { return warmUpCount += amount; }
    uint32_t* addressOfWarmUpCounter() { return reinterpret_cast<uint32_t*>(&warmUpCount); }
    static size_t offsetOfWarmUpCounter() { return offsetof(JSScript, warmUpCount); }
    void resetWarmUpCounter() { incWarmUpResetCounter(); warmUpCount = 0; }

    uint16_t getWarmUpResetCount() const { return warmUpResetCount; }
    uint16_t incWarmUpResetCounter(uint16_t amount = 1) { return warmUpResetCount += amount; }
    void resetWarmUpResetCounter() { warmUpResetCount = 0; }

  public:
    bool initScriptCounts(JSContext* cx);
    bool initScriptName(JSContext* cx);
    js::ScriptCounts& getScriptCounts();
    const char* getScriptName();
    js::PCCounts* maybeGetPCCounts(jsbytecode* pc);
    const js::PCCounts* maybeGetThrowCounts(jsbytecode* pc);
    js::PCCounts* getThrowCounts(jsbytecode* pc);
    uint64_t getHitCount(jsbytecode* pc);
    void incHitCount(jsbytecode* pc); // Used when we bailout out of Ion.
    void addIonCounts(js::jit::IonScriptCounts* ionCounts);
    js::jit::IonScriptCounts* getIonCounts();
    void releaseScriptCounts(js::ScriptCounts* counts);
    void destroyScriptCounts();
    void destroyScriptName();
    void clearHasScriptCounts();

    jsbytecode* main() const {
        return code() + mainOffset();
    }

    /*
     * computedSizeOfData() is the in-use size of all the data sections.
     * sizeOfData() is the size of the block allocated to hold all the data
     * sections (which can be larger than the in-use size).
     */
    size_t computedSizeOfData() const;
    size_t sizeOfData(mozilla::MallocSizeOf mallocSizeOf) const;
    size_t sizeOfTypeScript(mozilla::MallocSizeOf mallocSizeOf) const;

    bool hasArray(ArrayKind kind) const {
        return bitFields_.hasArrayBits_ & (1 << kind);
    }
    void setHasArray(ArrayKind kind) { bitFields_.hasArrayBits_ |= (1 << kind); }
    void cloneHasArray(JSScript* script) {
        bitFields_.hasArrayBits_ = script->bitFields_.hasArrayBits_;
    }

    bool hasConsts() const       { return hasArray(CONSTS); }
    bool hasObjects() const      { return hasArray(OBJECTS); }
    bool hasTrynotes() const     { return hasArray(TRYNOTES); }
    bool hasScopeNotes() const   { return hasArray(SCOPENOTES); }
    bool hasYieldAndAwaitOffsets() const {
        return isGenerator() || isAsync();
    }

#define OFF(fooOff, hasFoo, t)   (fooOff() + (hasFoo() ? sizeof(t) : 0))

    size_t scopesOffset() const       { return 0; }
    size_t constsOffset() const       { return scopesOffset() + sizeof(js::ScopeArray); }
    size_t objectsOffset() const      { return OFF(constsOffset,     hasConsts,     js::ConstArray); }
    size_t trynotesOffset() const     { return OFF(objectsOffset,    hasObjects,    js::ObjectArray); }
    size_t scopeNotesOffset() const   { return OFF(trynotesOffset,   hasTrynotes,   js::TryNoteArray); }
    size_t yieldAndAwaitOffsetsOffset() const {
        return OFF(scopeNotesOffset, hasScopeNotes, js::ScopeNoteArray);
    }

#undef OFF

    size_t dataSize() const { return dataSize_; }

  private:

    js::ConstArray* constsRaw() const {
        MOZ_ASSERT(hasConsts());
        return reinterpret_cast<js::ConstArray*>(data + constsOffset());
    }

    js::ObjectArray* objectsRaw() const {
        MOZ_ASSERT(hasObjects());
        return reinterpret_cast<js::ObjectArray*>(data + objectsOffset());
    }

    js::ScopeArray* scopesRaw() const {
        return reinterpret_cast<js::ScopeArray*>(data + scopesOffset());
    }

    js::TryNoteArray* trynotesRaw() const {
        MOZ_ASSERT(hasTrynotes());
        return reinterpret_cast<js::TryNoteArray*>(data + trynotesOffset());
    }

    js::ScopeNoteArray* scopeNotesRaw() const {
        MOZ_ASSERT(hasScopeNotes());
        return reinterpret_cast<js::ScopeNoteArray*>(data + scopeNotesOffset());
    }

    js::YieldAndAwaitOffsetArray& yieldAndAwaitOffsetsRaw() const {
        MOZ_ASSERT(hasYieldAndAwaitOffsets());
        return *reinterpret_cast<js::YieldAndAwaitOffsetArray*>(data +
                                                                yieldAndAwaitOffsetsOffset());
    }

  public:

    mozilla::Span<js::GCPtrValue> consts() const {
        js::ConstArray* array = constsRaw();
        return mozilla::MakeSpan(array->vector, array->length);
    }

    mozilla::Span<js::GCPtrObject> objects() const {
        js::ObjectArray* array = objectsRaw();
        return mozilla::MakeSpan(array->vector, array->length);
    }

    mozilla::Span<js::GCPtrScope> scopes() const {
        js::ScopeArray* array = scopesRaw();
        return mozilla::MakeSpan(array->vector, array->length);
    }

    mozilla::Span<JSTryNote> trynotes() const {
        js::TryNoteArray* array = trynotesRaw();
        return mozilla::MakeSpan(array->vector, array->length);
    }

    mozilla::Span<js::ScopeNote> scopeNotes() const {
        js::ScopeNoteArray* array = scopeNotesRaw();
        return mozilla::MakeSpan(array->vector, array->length);
    }

    mozilla::Span<uint32_t> yieldAndAwaitOffsets() const {
        js::YieldAndAwaitOffsetArray& array = yieldAndAwaitOffsetsRaw();
        return mozilla::MakeSpan(&array[0], array.length());
    }

    bool hasLoops();

    uint32_t numNotes() const {
        MOZ_ASSERT(scriptData_);
        return scriptData_->numNotes();
    }
    jssrcnote* notes() const {
        MOZ_ASSERT(scriptData_);
        return scriptData_->notes();
    }

    size_t natoms() const {
        MOZ_ASSERT(scriptData_);
        return scriptData_->natoms();
    }
    js::GCPtrAtom* atoms() const {
        MOZ_ASSERT(scriptData_);
        return scriptData_->atoms();
    }

    js::GCPtrAtom& getAtom(size_t index) const {
        MOZ_ASSERT(index < natoms());
        return atoms()[index];
    }

    js::GCPtrAtom& getAtom(jsbytecode* pc) const {
        MOZ_ASSERT(containsPC(pc) && containsPC(pc + sizeof(uint32_t)));
        MOZ_ASSERT(js::JOF_OPTYPE((JSOp)*pc) == JOF_ATOM);
        return getAtom(GET_UINT32_INDEX(pc));
    }

    js::PropertyName* getName(size_t index) {
        return getAtom(index)->asPropertyName();
    }

    js::PropertyName* getName(jsbytecode* pc) const {
        return getAtom(pc)->asPropertyName();
    }

    JSObject* getObject(size_t index) {
        MOZ_ASSERT(objects()[index]->isTenured());
        return objects()[index];
    }

    JSObject* getObject(jsbytecode* pc) {
        MOZ_ASSERT(containsPC(pc) && containsPC(pc + sizeof(uint32_t)));
        return getObject(GET_UINT32_INDEX(pc));
    }

    js::Scope* getScope(size_t index) const {
        return scopes()[index];
    }

    js::Scope* getScope(jsbytecode* pc) const {
        // This method is used to get a scope directly using a JSOp with an
        // index. To search through ScopeNotes to look for a Scope using pc,
        // use lookupScope.
        MOZ_ASSERT(containsPC(pc) && containsPC(pc + sizeof(uint32_t)));
        MOZ_ASSERT(js::JOF_OPTYPE(JSOp(*pc)) == JOF_SCOPE,
                   "Did you mean to use lookupScope(pc)?");
        return getScope(GET_UINT32_INDEX(pc));
    }

    inline JSFunction* getFunction(size_t index);
    JSFunction* function() const {
        if (functionNonDelazifying()) {
            return functionNonDelazifying();
        }
        return nullptr;
    }

    inline js::RegExpObject* getRegExp(size_t index);
    inline js::RegExpObject* getRegExp(jsbytecode* pc);

    const js::Value& getConst(size_t index) {
        return consts()[index];
    }

    // The following 3 functions find the static scope just before the
    // execution of the instruction pointed to by pc.

    js::Scope* lookupScope(jsbytecode* pc);

    js::Scope* innermostScope(jsbytecode* pc);
    js::Scope* innermostScope() { return innermostScope(main()); }

    /*
     * The isEmpty method tells whether this script has code that computes any
     * result (not return value, result AKA normal completion value) other than
     * JSVAL_VOID, or any other effects.
     */
    bool isEmpty() const {
        if (length() > 3) {
            return false;
        }

        jsbytecode* pc = code();
        if (noScriptRval() && JSOp(*pc) == JSOP_FALSE) {
            ++pc;
        }
        return JSOp(*pc) == JSOP_RETRVAL;
    }

    bool formalIsAliased(unsigned argSlot);
    bool formalLivesInArgumentsObject(unsigned argSlot);

  private:
    /* Change this->stepMode to |newValue|. */
    void setNewStepMode(js::FreeOp* fop, uint32_t newValue);

    bool ensureHasDebugScript(JSContext* cx);
    js::DebugScript* debugScript();
    js::DebugScript* releaseDebugScript();
    void destroyDebugScript(js::FreeOp* fop);

  public:
    bool hasBreakpointsAt(jsbytecode* pc);
    bool hasAnyBreakpointsOrStepMode() { return bitFields_.hasDebugScript_; }

    // See comment above 'debugMode' in Realm.h for explanation of
    // invariants of debuggee compartments, scripts, and frames.
    inline bool isDebuggee() const;

    js::BreakpointSite* getBreakpointSite(jsbytecode* pc)
    {
        return bitFields_.hasDebugScript_ ? debugScript()->breakpoints[pcToOffset(pc)] : nullptr;
    }

    js::BreakpointSite* getOrCreateBreakpointSite(JSContext* cx, jsbytecode* pc);

    void destroyBreakpointSite(js::FreeOp* fop, jsbytecode* pc);

    void clearBreakpointsIn(js::FreeOp* fop, js::Debugger* dbg, JSObject* handler);

    /*
     * Increment or decrement the single-step count. If the count is non-zero
     * then the script is in single-step mode.
     *
     * Only incrementing is fallible, as it could allocate a DebugScript.
     */
    bool incrementStepModeCount(JSContext* cx);
    void decrementStepModeCount(js::FreeOp* fop);

    bool stepModeEnabled() { return bitFields_.hasDebugScript_ && !!debugScript()->stepMode; }

#ifdef DEBUG
    uint32_t stepModeCount() { return bitFields_.hasDebugScript_ ? debugScript()->stepMode : 0; }
#endif

    void finalize(js::FreeOp* fop);

    static const JS::TraceKind TraceKind = JS::TraceKind::Script;

    void traceChildren(JSTracer* trc);

    // A helper class to prevent relazification of the given function's script
    // while it's holding on to it.  This class automatically roots the script.
    class AutoDelazify;
    friend class AutoDelazify;

    class AutoDelazify
    {
        JS::RootedScript script_;
        JSContext* cx_;
        bool oldDoNotRelazify_;
      public:
        explicit AutoDelazify(JSContext* cx, JS::HandleFunction fun = nullptr)
            : script_(cx)
            , cx_(cx)
            , oldDoNotRelazify_(false)
        {
            holdScript(fun);
        }

        ~AutoDelazify()
        {
            dropScript();
        }

        void operator=(JS::HandleFunction fun)
        {
            dropScript();
            holdScript(fun);
        }

        operator JS::HandleScript() const { return script_; }
        explicit operator bool() const { return script_; }

      private:
        void holdScript(JS::HandleFunction fun);
        void dropScript();
    };

    // Return whether the record/replay execution progress counter
    // (see RecordReplay.h) should be updated as this script runs.
    inline bool trackRecordReplayProgress() const;
};

/* If this fails, add/remove padding within JSScript. */
static_assert(sizeof(JSScript) % js::gc::CellAlignBytes == 0,
              "Size of JSScript must be an integral multiple of js::gc::CellAlignBytes");

namespace js {

// Information about a script which may be (or has been) lazily compiled to
// bytecode from its source.
class LazyScript : public gc::TenuredCell
{
  private:
    // If non-nullptr, the script has been compiled and this is a forwarding
    // pointer to the result. This is a weak pointer: after relazification, we
    // can collect the script if there are no other pointers to it.
    WeakRef<JSScript*> script_;

    // Original function with which the lazy script is associated.
    GCPtrFunction function_;

    // This field holds one of:
    //   * LazyScript in which the script is nested.  This case happens if the
    //     enclosing script is lazily parsed and have never been compiled.
    //
    //     This is used by the debugger to delazify the enclosing scripts
    //     recursively.  The all ancestor LazyScripts in this linked-list are
    //     kept alive as long as this LazyScript is alive, which doesn't result
    //     in keeping them unnecessarily alive outside of the debugger for the
    //     following reasons:
    //
    //       * Outside of the debugger, a LazyScript is visible to user (which
    //         means the LazyScript can be pointed from somewhere else than the
    //         enclosing script) only if the enclosing script is compiled and
    //         executed.  While compiling the enclosing script, this field is
    //         changed to point the enclosing scope.  So the enclosing
    //         LazyScript is no more in the list.
    //       * Before the enclosing script gets compiled, this LazyScript is
    //         kept alive only if the outermost LazyScript in the list is kept
    //         alive.
    //       * Once this field is changed to point the enclosing scope, this
    //         field will never point the enclosing LazyScript again, since
    //         relazification is not performed on non-leaf scripts.
    //
    //   * Scope in which the script is nested.  This case happens if the
    //     enclosing script has ever been compiled.
    //
    //   * nullptr for incomplete (initial or failure) state
    //
    // This field should be accessed via accessors:
    //   * enclosingScope
    //   * setEnclosingScope (cannot be called twice)
    //   * enclosingLazyScript
    //   * setEnclosingLazyScript (cannot be called twice)
    // after checking:
    //   * hasEnclosingLazyScript
    //   * hasEnclosingScope
    //
    // The transition of fields are following:
    //
    //  o                               o
    //  | when function is lazily       | when decoded from XDR,
    //  | parsed inside a function      | and enclosing script is lazy
    //  | which is lazily parsed        | (CreateForXDR without enclosingScope)
    //  | (Create)                      |
    //  v                               v
    // +---------+                     +---------+
    // | nullptr |                     | nullptr |
    // +---------+                     +---------+
    //  |                               |
    //  | when enclosing function is    | when enclosing script is decoded
    //  | lazily parsed and this        | and this script's function is put
    //  | script's function is put      | into innerFunctions()
    //  | into innerFunctions()         | (setEnclosingLazyScript)
    //  | (setEnclosingLazyScript)      |
    //  |                               |
    //  |                               |     o
    //  |                               |     | when function is lazily
    //  |                               |     | parsed inside a function
    //  |                               |     | which is eagerly parsed
    //  |                               |     | (Create)
    //  v                               |     v
    // +----------------------+         |    +---------+
    // | enclosing LazyScript |<--------+    | nullptr |
    // +----------------------+              +---------+
    //  |                                     |
    //  v                                     |
    //  +<------------------------------------+
    //  |
    //  | when the enclosing script     o
    //  | is successfully compiled      | when decoded from XDR,
    //  | (setEnclosingScope)           | and enclosing script is not lazy
    //  v                               | (CreateForXDR with enclosingScope)
    // +-----------------+              |
    // | enclosing Scope |<-------------+
    // +-----------------+
    GCPtr<TenuredCell*> enclosingLazyScriptOrScope_;

    // ScriptSourceObject. We leave this set to nullptr until we generate
    // bytecode for our immediate parent. This is never a CCW; we don't clone
    // LazyScripts into other compartments.
    GCPtrObject sourceObject_;

    // Heap allocated table with any free variables or inner functions.
    void* table_;

  private:
    static const uint32_t NumClosedOverBindingsBits = 20;
    static const uint32_t NumInnerFunctionsBits = 20;

    struct PackedView {
        uint32_t shouldDeclareArguments : 1;
        uint32_t hasThisBinding : 1;
        uint32_t isAsync : 1;
        uint32_t isBinAST : 1;

        uint32_t numClosedOverBindings : NumClosedOverBindingsBits;

        // -- 32bit boundary --

        uint32_t numInnerFunctions : NumInnerFunctionsBits;

        // N.B. These are booleans but need to be uint32_t to pack correctly on MSVC.
        // If you add another boolean here, make sure to initialize it in
        // LazyScript::Create().
        uint32_t isGenerator : 1;
        uint32_t strict : 1;
        uint32_t bindingsAccessedDynamically : 1;
        uint32_t hasDebuggerStatement : 1;
        uint32_t hasDirectEval : 1;
        uint32_t isLikelyConstructorWrapper : 1;
        uint32_t hasBeenCloned : 1;
        uint32_t treatAsRunOnce : 1;
        uint32_t isDerivedClassConstructor : 1;
        uint32_t needsHomeObject : 1;
        uint32_t hasRest : 1;
        uint32_t parseGoal : 1;
    };

    union {
        PackedView p_;
        uint64_t packedFields_;
    };

    // Source location for the script.
    // See the comment in JSScript for the details
    uint32_t sourceStart_;
    uint32_t sourceEnd_;
    uint32_t toStringStart_;
    uint32_t toStringEnd_;
    // Line and column of |begin_| position, that is the position where we
    // start parsing.
    uint32_t lineno_;
    uint32_t column_;

    LazyScript(JSFunction* fun, ScriptSourceObject& sourceObject,
               void* table, uint64_t packedFields,
               uint32_t begin, uint32_t end, uint32_t toStringStart,
               uint32_t lineno, uint32_t column);

    // Create a LazyScript without initializing the closedOverBindings and the
    // innerFunctions. To be GC-safe, the caller must initialize both vectors
    // with valid atoms and functions.
    static LazyScript* CreateRaw(JSContext* cx, HandleFunction fun,
                                 HandleScriptSourceObject sourceObject,
                                 uint64_t packedData, uint32_t begin, uint32_t end,
                                 uint32_t toStringStart, uint32_t lineno, uint32_t column);

  public:
    static const uint32_t NumClosedOverBindingsLimit = 1 << NumClosedOverBindingsBits;
    static const uint32_t NumInnerFunctionsLimit = 1 << NumInnerFunctionsBits;

    // Create a LazyScript and initialize closedOverBindings and innerFunctions
    // with the provided vectors.
    static LazyScript* Create(JSContext* cx, HandleFunction fun,
                              HandleScriptSourceObject sourceObject,
                              const frontend::AtomVector& closedOverBindings,
                              Handle<GCVector<JSFunction*, 8>> innerFunctions,
                              uint32_t begin, uint32_t end,
                              uint32_t toStringStart, uint32_t lineno, uint32_t column,
                              frontend::ParseGoal parseGoal);

    // Create a LazyScript and initialize the closedOverBindings and the
    // innerFunctions with dummy values to be replaced in a later initialization
    // phase.
    //
    // The "script" argument to this function can be null.  If it's non-null,
    // then this LazyScript should be associated with the given JSScript.
    //
    // The sourceObject and enclosingScope arguments may be null if the
    // enclosing function is also lazy.
    static LazyScript* CreateForXDR(JSContext* cx, HandleFunction fun,
                                    HandleScript script, HandleScope enclosingScope,
                                    HandleScriptSourceObject sourceObject,
                                    uint64_t packedData, uint32_t begin, uint32_t end,
                                    uint32_t toStringStart, uint32_t lineno, uint32_t column);

    void initRuntimeFields(uint64_t packedFields);

    static inline JSFunction* functionDelazifying(JSContext* cx, Handle<LazyScript*>);
    JSFunction* functionNonDelazifying() const {
        return function_;
    }

    JS::Compartment* compartment() const;
    JS::Compartment* maybeCompartment() const { return compartment(); }
    Realm* realm() const;

    void initScript(JSScript* script);

    JSScript* maybeScript() {
        return script_;
    }
    const JSScript* maybeScriptUnbarriered() const {
        return script_.unbarrieredGet();
    }
    bool hasScript() const {
        return bool(script_);
    }

    bool hasEnclosingScope() const {
        return enclosingLazyScriptOrScope_ &&
               enclosingLazyScriptOrScope_->is<Scope>();
    }
    bool hasEnclosingLazyScript() const {
        return enclosingLazyScriptOrScope_ &&
               enclosingLazyScriptOrScope_->is<LazyScript>();
    }

    LazyScript* enclosingLazyScript() const {
        MOZ_ASSERT(hasEnclosingLazyScript());
        return enclosingLazyScriptOrScope_->as<LazyScript>();
    }
    void setEnclosingLazyScript(LazyScript* enclosingLazyScript);

    Scope* enclosingScope() const {
        MOZ_ASSERT(hasEnclosingScope());
        return enclosingLazyScriptOrScope_->as<Scope>();
    }
    void setEnclosingScope(Scope* enclosingScope);

    bool hasNonSyntacticScope() const {
        return enclosingScope()->hasOnChain(ScopeKind::NonSyntactic);
    }

    ScriptSourceObject& sourceObject() const;
    ScriptSource* scriptSource() const {
        return sourceObject().source();
    }
    ScriptSource* maybeForwardedScriptSource() const;
    bool mutedErrors() const {
        return scriptSource()->mutedErrors();
    }

    uint32_t numClosedOverBindings() const {
        return p_.numClosedOverBindings;
    }
    JSAtom** closedOverBindings() {
        return (JSAtom**)table_;
    }

    uint32_t numInnerFunctions() const {
        return p_.numInnerFunctions;
    }
    GCPtrFunction* innerFunctions() {
        return (GCPtrFunction*)&closedOverBindings()[numClosedOverBindings()];
    }

    GeneratorKind generatorKind() const {
        return p_.isGenerator ? GeneratorKind::Generator : GeneratorKind::NotGenerator;
    }

    bool isGenerator() const { return generatorKind() == GeneratorKind::Generator; }

    void setGeneratorKind(GeneratorKind kind) {
        // A script only gets its generator kind set as part of initialization,
        // so it can only transition from NotGenerator.
        MOZ_ASSERT(!isGenerator());
        p_.isGenerator = kind == GeneratorKind::Generator;
    }

    FunctionAsyncKind asyncKind() const {
        return p_.isAsync ? FunctionAsyncKind::AsyncFunction : FunctionAsyncKind::SyncFunction;
    }
    bool isAsync() const {
        return p_.isAsync;
    }

    void setAsyncKind(FunctionAsyncKind kind) {
        p_.isAsync = kind == FunctionAsyncKind::AsyncFunction;
    }

    bool hasRest() const {
        return p_.hasRest;
    }
    void setHasRest() {
        p_.hasRest = true;
    }

    frontend::ParseGoal parseGoal() const {
        return frontend::ParseGoal(p_.parseGoal);
    }

    bool isBinAST() const {
        return p_.isBinAST;
    }
    void setIsBinAST() {
        p_.isBinAST = true;
    }

    bool strict() const {
        return p_.strict;
    }
    void setStrict() {
        p_.strict = true;
    }

    bool bindingsAccessedDynamically() const {
        return p_.bindingsAccessedDynamically;
    }
    void setBindingsAccessedDynamically() {
        p_.bindingsAccessedDynamically = true;
    }

    bool hasDebuggerStatement() const {
        return p_.hasDebuggerStatement;
    }
    void setHasDebuggerStatement() {
        p_.hasDebuggerStatement = true;
    }

    bool hasDirectEval() const {
        return p_.hasDirectEval;
    }
    void setHasDirectEval() {
        p_.hasDirectEval = true;
    }

    bool isLikelyConstructorWrapper() const {
        return p_.isLikelyConstructorWrapper;
    }
    void setLikelyConstructorWrapper() {
        p_.isLikelyConstructorWrapper = true;
    }

    bool hasBeenCloned() const {
        return p_.hasBeenCloned;
    }
    void setHasBeenCloned() {
        p_.hasBeenCloned = true;
    }

    bool treatAsRunOnce() const {
        return p_.treatAsRunOnce;
    }
    void setTreatAsRunOnce() {
        p_.treatAsRunOnce = true;
    }

    bool isDerivedClassConstructor() const {
        return p_.isDerivedClassConstructor;
    }
    void setIsDerivedClassConstructor() {
        p_.isDerivedClassConstructor = true;
    }

    bool needsHomeObject() const {
        return p_.needsHomeObject;
    }
    void setNeedsHomeObject() {
        p_.needsHomeObject = true;
    }

    bool shouldDeclareArguments() const {
        return p_.shouldDeclareArguments;
    }
    void setShouldDeclareArguments() {
        p_.shouldDeclareArguments = true;
    }

    bool hasThisBinding() const {
        return p_.hasThisBinding;
    }
    void setHasThisBinding() {
        p_.hasThisBinding = true;
    }

    const char* filename() const {
        return scriptSource()->filename();
    }
    uint32_t sourceStart() const {
        return sourceStart_;
    }
    uint32_t sourceEnd() const {
        return sourceEnd_;
    }
    uint32_t sourceLength() const {
        return sourceEnd_ - sourceStart_;
    }
    uint32_t toStringStart() const {
        return toStringStart_;
    }
    uint32_t toStringEnd() const {
        return toStringEnd_;
    }
    uint32_t lineno() const {
        return lineno_;
    }
    uint32_t column() const {
        return column_;
    }

    void setToStringEnd(uint32_t toStringEnd) {
        MOZ_ASSERT(toStringStart_ <= toStringEnd);
        MOZ_ASSERT(toStringEnd_ >= sourceEnd_);
        toStringEnd_ = toStringEnd;
    }

    // Returns true if the enclosing script has ever been compiled.
    // Once the enclosing script is compiled, the scope chain is created.
    // This LazyScript is delazify-able as long as it has the enclosing scope,
    // even if the enclosing JSScript is GCed.
    // The enclosing JSScript can be GCed later if the enclosing scope is not
    // FunctionScope or ModuleScope.
    bool enclosingScriptHasEverBeenCompiled() const {
        return hasEnclosingScope();
    }

    friend class GCMarker;
    void traceChildren(JSTracer* trc);
    void finalize(js::FreeOp* fop);

    static const JS::TraceKind TraceKind = JS::TraceKind::LazyScript;

    size_t sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf)
    {
        return mallocSizeOf(table_);
    }

    uint64_t packedFields() const {
        return packedFields_;
    }
};

/* If this fails, add/remove padding within LazyScript. */
static_assert(sizeof(LazyScript) % js::gc::CellAlignBytes == 0,
              "Size of LazyScript must be an integral multiple of js::gc::CellAlignBytes");

struct ScriptAndCounts
{
    /* This structure is stored and marked from the JSRuntime. */
    JSScript* script;
    ScriptCounts scriptCounts;

    inline explicit ScriptAndCounts(JSScript* script);
    inline ScriptAndCounts(ScriptAndCounts&& sac);

    const PCCounts* maybeGetPCCounts(jsbytecode* pc) const {
        return scriptCounts.maybeGetPCCounts(script->pcToOffset(pc));
    }
    const PCCounts* maybeGetThrowCounts(jsbytecode* pc) const {
        return scriptCounts.maybeGetThrowCounts(script->pcToOffset(pc));
    }

    jit::IonScriptCounts* getIonCounts() const {
        return scriptCounts.ionCounts_;
    }

    void trace(JSTracer* trc) {
        TraceRoot(trc, &script, "ScriptAndCounts::script");
    }
=======
  // Size to allocate
  static size_t AllocationSize(uint32_t numClosedOverBindings,
                               uint32_t numInnerFunctions);
  size_t allocationSize() const;

  // Translate an offset into a concrete pointer.
  template <typename T>
  T* offsetToPointer(size_t offset) {
    uintptr_t base = reinterpret_cast<uintptr_t>(this);
    return reinterpret_cast<T*>(base + offset);
  }

  template <typename T>
  void initElements(size_t offset, size_t length);

  LazyScriptData(uint32_t numClosedOverBindings, uint32_t numInnerFunctions);

 public:
  static LazyScriptData* new_(JSContext* cx, uint32_t numClosedOverBindings,
                              uint32_t numInnerFunctions);

  friend class LazyScript;

  mozilla::Span<GCPtrAtom> closedOverBindings();
  mozilla::Span<GCPtrFunction> innerFunctions();

  void trace(JSTracer* trc);

  // LazyScriptData has trailing data so isn't copyable or movable.
  LazyScriptData(const LazyScriptData&) = delete;
  LazyScriptData& operator=(const LazyScriptData&) = delete;
};

// Information about a script which may be (or has been) lazily compiled to
// bytecode from its source.
class LazyScript : public gc::TenuredCell {
  // If non-nullptr, the script has been compiled and this is a forwarding
  // pointer to the result. This is a weak pointer: after relazification, we
  // can collect the script if there are no other pointers to it.
  WeakHeapPtrScript script_;
  friend void js::gc::SweepLazyScripts(GCParallelTask* task);

  // Original function with which the lazy script is associated.
  GCPtrFunction function_;

  // This field holds one of:
  //   * LazyScript in which the script is nested.  This case happens if the
  //     enclosing script is lazily parsed and have never been compiled.
  //
  //     This is used by the debugger to delazify the enclosing scripts
  //     recursively.  The all ancestor LazyScripts in this linked-list are
  //     kept alive as long as this LazyScript is alive, which doesn't result
  //     in keeping them unnecessarily alive outside of the debugger for the
  //     following reasons:
  //
  //       * Outside of the debugger, a LazyScript is visible to user (which
  //         means the LazyScript can be pointed from somewhere else than the
  //         enclosing script) only if the enclosing script is compiled and
  //         executed.  While compiling the enclosing script, this field is
  //         changed to point the enclosing scope.  So the enclosing
  //         LazyScript is no more in the list.
  //       * Before the enclosing script gets compiled, this LazyScript is
  //         kept alive only if the outermost LazyScript in the list is kept
  //         alive.
  //       * Once this field is changed to point the enclosing scope, this
  //         field will never point the enclosing LazyScript again, since
  //         relazification is not performed on non-leaf scripts.
  //
  //   * Scope in which the script is nested.  This case happens if the
  //     enclosing script has ever been compiled.
  //
  //   * nullptr for incomplete (initial or failure) state
  //
  // This field should be accessed via accessors:
  //   * enclosingScope
  //   * setEnclosingScope (cannot be called twice)
  //   * enclosingLazyScript
  //   * setEnclosingLazyScript (cannot be called twice)
  // after checking:
  //   * hasEnclosingLazyScript
  //   * hasEnclosingScope
  //
  // The transition of fields are following:
  //
  //  o                               o
  //  | when function is lazily       | when decoded from XDR,
  //  | parsed inside a function      | and enclosing script is lazy
  //  | which is lazily parsed        | (CreateForXDR without enclosingScope)
  //  | (Create)                      |
  //  v                               v
  // +---------+                     +---------+
  // | nullptr |                     | nullptr |
  // +---------+                     +---------+
  //  |                               |
  //  | when enclosing function is    | when enclosing script is decoded
  //  | lazily parsed and this        | and this script's function is put
  //  | script's function is put      | into innerFunctions()
  //  | into innerFunctions()         | (setEnclosingLazyScript)
  //  | (setEnclosingLazyScript)      |
  //  |                               |
  //  |                               |     o
  //  |                               |     | when function is lazily
  //  |                               |     | parsed inside a function
  //  |                               |     | which is eagerly parsed
  //  |                               |     | (Create)
  //  v                               |     v
  // +----------------------+         |    +---------+
  // | enclosing LazyScript |<--------+    | nullptr |
  // +----------------------+              +---------+
  //  |                                     |
  //  v                                     |
  //  +<------------------------------------+
  //  |
  //  | when the enclosing script     o
  //  | is successfully compiled      | when decoded from XDR,
  //  | (setEnclosingScope)           | and enclosing script is not lazy
  //  v                               | (CreateForXDR with enclosingScope)
  // +-----------------+              |
  // | enclosing Scope |<-------------+
  // +-----------------+
  GCPtr<TenuredCell*> enclosingLazyScriptOrScope_;

  // ScriptSourceObject. We leave this set to nullptr until we generate
  // bytecode for our immediate parent.
  GCPtr<ScriptSourceObject*> sourceObject_;

  // Heap allocated table with any free variables, inner functions, or class
  // fields. This will be nullptr if none exist.
  LazyScriptData* lazyData_;

  static const uint32_t NumClosedOverBindingsBits = 20;
  static const uint32_t NumInnerFunctionsBits = 20;

  // See: JSScript::ImmutableFlags / MutableFlags.
  // NOTE: Lazy script only defines and uses a subset of these flags.
  using ImmutableFlags = JSScript::ImmutableFlags;
  using MutableFlags = JSScript::MutableFlags;

  uint32_t immutableFlags_;
  uint32_t mutableFlags_;

  MOZ_MUST_USE bool hasFlag(MutableFlags flag) const {
    return mutableFlags_ & uint32_t(flag);
  }
  void setFlag(MutableFlags flag) { mutableFlags_ |= uint32_t(flag); }

  MOZ_MUST_USE bool hasFlag(ImmutableFlags flag) const {
    return immutableFlags_ & uint32_t(flag);
  }
  void setFlag(ImmutableFlags flag) { immutableFlags_ |= uint32_t(flag); }

  // Source location for the script.
  // See the comment in JSScript for the details
  uint32_t sourceStart_;
  uint32_t sourceEnd_;
  uint32_t toStringStart_;
  uint32_t toStringEnd_;
  // Line and column of |begin_| position, that is the position where we
  // start parsing.
  uint32_t lineno_;
  uint32_t column_;

  LazyScript(JSFunction* fun, ScriptSourceObject& sourceObject,
             LazyScriptData* data, uint32_t immutableFlags,
             uint32_t sourceStart, uint32_t sourceEnd, uint32_t toStringStart,
             uint32_t lineno, uint32_t column);

  // Create a LazyScript without initializing the closedOverBindings and the
  // innerFunctions. To be GC-safe, the caller must initialize both vectors
  // with valid atoms and functions.
  static LazyScript* CreateRaw(JSContext* cx, uint32_t numClosedOverBindings,
                               uint32_t numInnerFunctions, HandleFunction fun,
                               HandleScriptSourceObject sourceObject,
                               uint32_t immutableFlags, uint32_t sourceStart,
                               uint32_t sourceEnd, uint32_t toStringStart,
                               uint32_t lineno, uint32_t column);

 public:
  static const uint32_t NumClosedOverBindingsLimit =
      1 << NumClosedOverBindingsBits;
  static const uint32_t NumInnerFunctionsLimit = 1 << NumInnerFunctionsBits;

  // Create a LazyScript and initialize closedOverBindings and innerFunctions
  // with the provided vectors.
  static LazyScript* Create(JSContext* cx, HandleFunction fun,
                            HandleScriptSourceObject sourceObject,
                            const frontend::AtomVector& closedOverBindings,
                            Handle<GCVector<JSFunction*, 8>> innerFunctions,
                            uint32_t sourceStart, uint32_t sourceEnd,
                            uint32_t toStringStart, uint32_t lineno,
                            uint32_t column, frontend::ParseGoal parseGoal);

  // Create a LazyScript and initialize the closedOverBindings and the
  // innerFunctions with dummy values to be replaced in a later initialization
  // phase.
  //
  // The "script" argument to this function can be null.  If it's non-null,
  // then this LazyScript should be associated with the given JSScript.
  //
  // The sourceObject and enclosingScope arguments may be null if the
  // enclosing function is also lazy.
  static LazyScript* CreateForXDR(
      JSContext* cx, uint32_t numClosedOverBindings, uint32_t numInnerFunctions,
      HandleFunction fun, HandleScript script, HandleScope enclosingScope,
      HandleScriptSourceObject sourceObject, uint32_t immutableFlags,
      uint32_t sourceStart, uint32_t sourceEnd, uint32_t toStringStart,
      uint32_t toStringEnd, uint32_t lineno, uint32_t column);

  static inline JSFunction* functionDelazifying(JSContext* cx,
                                                Handle<LazyScript*>);
  JSFunction* functionNonDelazifying() const { return function_; }

  JS::Compartment* compartment() const;
  JS::Compartment* maybeCompartment() const { return compartment(); }
  Realm* realm() const;

  void initScript(JSScript* script);

  JSScript* maybeScript() { return script_; }
  const JSScript* maybeScriptUnbarriered() const {
    return script_.unbarrieredGet();
  }
  bool hasScript() const { return bool(script_); }

  bool hasEnclosingScope() const {
    return enclosingLazyScriptOrScope_ &&
           enclosingLazyScriptOrScope_->is<Scope>();
  }
  bool hasEnclosingLazyScript() const {
    return enclosingLazyScriptOrScope_ &&
           enclosingLazyScriptOrScope_->is<LazyScript>();
  }

  LazyScript* enclosingLazyScript() const {
    MOZ_ASSERT(hasEnclosingLazyScript());
    return enclosingLazyScriptOrScope_->as<LazyScript>();
  }
  void setEnclosingLazyScript(LazyScript* enclosingLazyScript);

  Scope* enclosingScope() const {
    MOZ_ASSERT(hasEnclosingScope());
    return enclosingLazyScriptOrScope_->as<Scope>();
  }
  void setEnclosingScope(Scope* enclosingScope);

  bool hasNonSyntacticScope() const {
    return enclosingScope()->hasOnChain(ScopeKind::NonSyntactic);
  }

  ScriptSourceObject& sourceObject() const;
  ScriptSource* scriptSource() const { return sourceObject().source(); }
  ScriptSource* maybeForwardedScriptSource() const;
  bool mutedErrors() const { return scriptSource()->mutedErrors(); }

  mozilla::Span<GCPtrAtom> closedOverBindings() {
    return lazyData_ ? lazyData_->closedOverBindings()
                     : mozilla::Span<GCPtrAtom>();
  }
  uint32_t numClosedOverBindings() const {
    return lazyData_ ? lazyData_->closedOverBindings().size() : 0;
  };

  mozilla::Span<GCPtrFunction> innerFunctions() {
    return lazyData_ ? lazyData_->innerFunctions()
                     : mozilla::Span<GCPtrFunction>();
  }
  uint32_t numInnerFunctions() const {
    return lazyData_ ? lazyData_->innerFunctions().size() : 0;
  }

  GeneratorKind generatorKind() const {
    return hasFlag(ImmutableFlags::IsGenerator) ? GeneratorKind::Generator
                                                : GeneratorKind::NotGenerator;
  }

  bool isGenerator() const {
    return generatorKind() == GeneratorKind::Generator;
  }

  void setGeneratorKind(GeneratorKind kind) {
    // A script only gets its generator kind set as part of initialization,
    // so it can only transition from NotGenerator.
    MOZ_ASSERT(!isGenerator());
    if (kind == GeneratorKind::Generator) {
      setFlag(ImmutableFlags::IsGenerator);
    }
  }

  bool isAsync() const { return hasFlag(ImmutableFlags::IsAsync); }
  FunctionAsyncKind asyncKind() const {
    return isAsync() ? FunctionAsyncKind::AsyncFunction
                     : FunctionAsyncKind::SyncFunction;
  }

  void setAsyncKind(FunctionAsyncKind kind) {
    if (kind == FunctionAsyncKind::AsyncFunction) {
      setFlag(ImmutableFlags::IsAsync);
    }
  }

  bool hasRest() const { return hasFlag(ImmutableFlags::HasRest); }
  void setHasRest() { setFlag(ImmutableFlags::HasRest); }

  frontend::ParseGoal parseGoal() const {
    if (hasFlag(ImmutableFlags::IsModule)) {
      return frontend::ParseGoal::Module;
    }
    return frontend::ParseGoal::Script;
  }

  bool isBinAST() const { return hasFlag(ImmutableFlags::IsBinAST); }
  void setIsBinAST() { setFlag(ImmutableFlags::IsBinAST); }

  bool strict() const { return hasFlag(ImmutableFlags::Strict); }
  void setStrict() { setFlag(ImmutableFlags::Strict); }

  bool bindingsAccessedDynamically() const {
    return hasFlag(ImmutableFlags::BindingsAccessedDynamically);
  }
  void setBindingsAccessedDynamically() {
    setFlag(ImmutableFlags::BindingsAccessedDynamically);
  }

  bool hasDebuggerStatement() const {
    return hasFlag(ImmutableFlags::HasDebuggerStatement);
  }
  void setHasDebuggerStatement() {
    setFlag(ImmutableFlags::HasDebuggerStatement);
  }

  bool hasDirectEval() const { return hasFlag(ImmutableFlags::HasDirectEval); }
  void setHasDirectEval() { setFlag(ImmutableFlags::HasDirectEval); }

  bool isLikelyConstructorWrapper() const {
    return hasFlag(ImmutableFlags::IsLikelyConstructorWrapper);
  }
  void setLikelyConstructorWrapper() {
    setFlag(ImmutableFlags::IsLikelyConstructorWrapper);
  }

  bool hasBeenCloned() const { return hasFlag(MutableFlags::HasBeenCloned); }
  void setHasBeenCloned() { setFlag(MutableFlags::HasBeenCloned); }

  bool treatAsRunOnce() const {
    return hasFlag(ImmutableFlags::TreatAsRunOnce);
  }
  void setTreatAsRunOnce() { setFlag(ImmutableFlags::TreatAsRunOnce); }

  bool isDerivedClassConstructor() const {
    return hasFlag(ImmutableFlags::IsDerivedClassConstructor);
  }
  void setIsDerivedClassConstructor() {
    setFlag(ImmutableFlags::IsDerivedClassConstructor);
  }

  bool needsHomeObject() const {
    return hasFlag(ImmutableFlags::NeedsHomeObject);
  }
  void setNeedsHomeObject() { setFlag(ImmutableFlags::NeedsHomeObject); }

  bool shouldDeclareArguments() const {
    return hasFlag(ImmutableFlags::ShouldDeclareArguments);
  }
  void setShouldDeclareArguments() {
    setFlag(ImmutableFlags::ShouldDeclareArguments);
  }

  bool hasThisBinding() const {
    return hasFlag(ImmutableFlags::FunctionHasThisBinding);
  }
  void setHasThisBinding() { setFlag(ImmutableFlags::FunctionHasThisBinding); }

  void setFieldInitializers(FieldInitializers fieldInitializers) {
    MOZ_ASSERT(lazyData_);
    lazyData_->fieldInitializers_ = fieldInitializers;
  }

  const FieldInitializers& getFieldInitializers() const {
    MOZ_ASSERT(lazyData_);
    return lazyData_->fieldInitializers_;
  }

  const char* filename() const { return scriptSource()->filename(); }
  uint32_t sourceStart() const { return sourceStart_; }
  uint32_t sourceEnd() const { return sourceEnd_; }
  uint32_t sourceLength() const { return sourceEnd_ - sourceStart_; }
  uint32_t toStringStart() const { return toStringStart_; }
  uint32_t toStringEnd() const { return toStringEnd_; }
  uint32_t lineno() const { return lineno_; }
  uint32_t column() const { return column_; }

  void setToStringEnd(uint32_t toStringEnd) {
    MOZ_ASSERT(toStringStart_ <= toStringEnd);
    MOZ_ASSERT(toStringEnd_ >= sourceEnd_);
    toStringEnd_ = toStringEnd;
  }

  // Returns true if the enclosing script has ever been compiled.
  // Once the enclosing script is compiled, the scope chain is created.
  // This LazyScript is delazify-able as long as it has the enclosing scope,
  // even if the enclosing JSScript is GCed.
  // The enclosing JSScript can be GCed later if the enclosing scope is not
  // FunctionScope or ModuleScope.
  bool enclosingScriptHasEverBeenCompiled() const {
    return hasEnclosingScope();
  }

  friend class GCMarker;
  void traceChildren(JSTracer* trc);
  void finalize(js::FreeOp* fop);

  static const JS::TraceKind TraceKind = JS::TraceKind::LazyScript;

  size_t sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf) {
    return mallocSizeOf(lazyData_);
  }

  uint32_t immutableFlags() const { return immutableFlags_; }
};

/* If this fails, add/remove padding within LazyScript. */
static_assert(sizeof(LazyScript) % js::gc::CellAlignBytes == 0,
              "Size of LazyScript must be an integral multiple of "
              "js::gc::CellAlignBytes");

struct ScriptAndCounts {
  /* This structure is stored and marked from the JSRuntime. */
  JSScript* script;
  ScriptCounts scriptCounts;

  inline explicit ScriptAndCounts(JSScript* script);
  inline ScriptAndCounts(ScriptAndCounts&& sac);

  const PCCounts* maybeGetPCCounts(jsbytecode* pc) const {
    return scriptCounts.maybeGetPCCounts(script->pcToOffset(pc));
  }
  const PCCounts* maybeGetThrowCounts(jsbytecode* pc) const {
    return scriptCounts.maybeGetThrowCounts(script->pcToOffset(pc));
  }

  jit::IonScriptCounts* getIonCounts() const { return scriptCounts.ionCounts_; }

  void trace(JSTracer* trc) {
    TraceRoot(trc, &script, "ScriptAndCounts::script");
  }
>>>>>>> upstream-releases
};

extern char* FormatIntroducedFilename(JSContext* cx, const char* filename,
                                      unsigned lineno, const char* introducer);

struct GSNCache;

jssrcnote* GetSrcNote(GSNCache& cache, JSScript* script, jsbytecode* pc);

extern jssrcnote* GetSrcNote(JSContext* cx, JSScript* script, jsbytecode* pc);

extern jsbytecode* LineNumberToPC(JSScript* script, unsigned lineno);

extern JS_FRIEND_API unsigned GetScriptLineExtent(JSScript* script);

} /* namespace js */

namespace js {

extern unsigned PCToLineNumber(JSScript* script, jsbytecode* pc,
                               unsigned* columnp = nullptr);

extern unsigned PCToLineNumber(unsigned startLine, jssrcnote* notes,
                               jsbytecode* code, jsbytecode* pc,
                               unsigned* columnp = nullptr);

/*
 * This function returns the file and line number of the script currently
 * executing on cx. If there is no current script executing on cx (e.g., a
 * native called directly through JSAPI (e.g., by setTimeout)), nullptr and 0
 * are returned as the file and line.
 */
extern void DescribeScriptedCallerForCompilation(
    JSContext* cx, MutableHandleScript maybeScript, const char** file,
    unsigned* linenop, uint32_t* pcOffset, bool* mutedErrors);

/*
 * Like DescribeScriptedCallerForCompilation, but this function avoids looking
 * up the script/pc and the full linear scan to compute line number.
 */
extern void DescribeScriptedCallerForDirectEval(
    JSContext* cx, HandleScript script, jsbytecode* pc, const char** file,
    unsigned* linenop, uint32_t* pcOffset, bool* mutedErrors);

<<<<<<< HEAD
JSScript* CloneScriptIntoFunction(JSContext* cx, HandleScope enclosingScope,
                                  HandleFunction fun, HandleScript src);
||||||| merged common ancestors
JSScript*
CloneScriptIntoFunction(JSContext* cx, HandleScope enclosingScope, HandleFunction fun,
                        HandleScript src);
=======
JSScript* CloneScriptIntoFunction(JSContext* cx, HandleScope enclosingScope,
                                  HandleFunction fun, HandleScript src,
                                  Handle<ScriptSourceObject*> sourceObject);
>>>>>>> upstream-releases

JSScript* CloneGlobalScript(JSContext* cx, ScopeKind scopeKind,
                            HandleScript src);

} /* namespace js */

// JS::ubi::Nodes can point to js::LazyScripts; they're js::gc::Cell instances
// with no associated compartment.
namespace JS {
namespace ubi {
template <>
class Concrete<js::LazyScript> : TracerConcrete<js::LazyScript> {
 protected:
  explicit Concrete(js::LazyScript* ptr)
      : TracerConcrete<js::LazyScript>(ptr) {}

 public:
  static void construct(void* storage, js::LazyScript* ptr) {
    new (storage) Concrete(ptr);
  }

  CoarseType coarseType() const final { return CoarseType::Script; }
  Size size(mozilla::MallocSizeOf mallocSizeOf) const override;
  const char* scriptFilename() const final;

  const char16_t* typeName() const override { return concreteTypeName; }
  static const char16_t concreteTypeName[];
};
}  // namespace ubi
}  // namespace JS

#endif /* vm_JSScript_h */
