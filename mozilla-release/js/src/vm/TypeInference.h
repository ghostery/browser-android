/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* Definitions related to javascript type inference. */

#ifndef vm_TypeInference_h
#define vm_TypeInference_h

#include "mozilla/Attributes.h"
#include "mozilla/Maybe.h"
#include "mozilla/MemoryReporting.h"

#include "jsfriendapi.h"
#include "jstypes.h"

#include "ds/LifoAlloc.h"
#include "gc/Barrier.h"
#include "jit/IonTypes.h"
#include "js/AllocPolicy.h"
#include "js/HeapAPI.h"  // js::CurrentThreadCanAccessZone
#include "js/UbiNode.h"
#include "js/Utility.h"
#include "js/Vector.h"
#include "threading/ProtectedData.h"  // js::ZoneData
#include "vm/Shape.h"
#include "vm/TypeSet.h"

namespace js {

class TypeConstraint;
class TypeZone;
class CompilerConstraintList;
class HeapTypeSetKey;

namespace jit {

class ICScript;
struct IonScript;
class JitScript;
class TempAllocator;

}  // namespace jit

// If there is an OOM while sweeping types, the type information is deoptimized
// so that it stays correct (i.e. overapproximates the possible types in the
// zone), but constraints might not have been triggered on the deoptimization
// or even copied over completely. In this case, destroy all JIT code and new
// script information in the zone, the only things whose correctness depends on
// the type constraints.
class AutoClearTypeInferenceStateOnOOM {
  Zone* zone;

  AutoClearTypeInferenceStateOnOOM(const AutoClearTypeInferenceStateOnOOM&) =
      delete;
  void operator=(const AutoClearTypeInferenceStateOnOOM&) = delete;

 public:
  explicit AutoClearTypeInferenceStateOnOOM(Zone* zone);
  ~AutoClearTypeInferenceStateOnOOM();
};

class MOZ_RAII AutoSweepBase {
  // Make sure we don't GC while this class is live since GC might trigger
  // (incremental) sweeping.
  JS::AutoCheckCannotGC nogc;
};

// Sweep an ObjectGroup. Functions that expect a swept group should take a
// reference to this class.
class MOZ_RAII AutoSweepObjectGroup : public AutoSweepBase {
#ifdef DEBUG
  ObjectGroup* group_;
#endif

 public:
  inline explicit AutoSweepObjectGroup(ObjectGroup* group);
#ifdef DEBUG
  inline ~AutoSweepObjectGroup();

  ObjectGroup* group() const { return group_; }
#endif
};

<<<<<<< HEAD
// Sweep a TypeScript. Functions that expect a swept script should take a
// reference to this class.
class MOZ_RAII AutoSweepTypeScript : public AutoSweepBase {
||||||| merged common ancestors
// Sweep a TypeScript. Functions that expect a swept script should take a
// reference to this class.
class MOZ_RAII AutoSweepTypeScript : public AutoSweepBase
{
=======
// Sweep the type inference data in a JitScript. Functions that expect a swept
// script should take a reference to this class.
class MOZ_RAII AutoSweepJitScript : public AutoSweepBase {
>>>>>>> upstream-releases
#ifdef DEBUG
<<<<<<< HEAD
  JSScript* script_;
||||||| merged common ancestors
    JSScript* script_;
=======
  Zone* zone_;
  jit::JitScript* jitScript_;
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
 public:
  inline explicit AutoSweepTypeScript(JSScript* script);
||||||| merged common ancestors
  public:
    inline explicit AutoSweepTypeScript(JSScript* script);
=======
 public:
  inline explicit AutoSweepJitScript(JSScript* script);
>>>>>>> upstream-releases
#ifdef DEBUG
<<<<<<< HEAD
  inline ~AutoSweepTypeScript();
||||||| merged common ancestors
    inline ~AutoSweepTypeScript();
=======
  inline ~AutoSweepJitScript();
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSScript* script() const { return script_; }
||||||| merged common ancestors
    JSScript* script() const {
        return script_;
    }
=======
  jit::JitScript* jitScript() const { return jitScript_; }
  Zone* zone() const { return zone_; }
>>>>>>> upstream-releases
#endif
};

<<<<<<< HEAD
CompilerConstraintList* NewCompilerConstraintList(jit::TempAllocator& alloc);
||||||| merged common ancestors
CompilerConstraintList*
NewCompilerConstraintList(jit::TempAllocator& alloc);
=======
CompilerConstraintList* NewCompilerConstraintList(jit::TempAllocator& alloc);

// Stack class to record information about constraints that need to be added
// after finishing the Definite Properties Analysis. When the analysis succeeds
// the |finishConstraints| method must be called to add the constraints to the
// TypeSets.
//
// There are two constraint types managed here:
//
//   1. Proto constraints for HeapTypeSets, to guard against things like getters
//      and setters on the proto chain.
//
//   2. Inlining constraints for StackTypeSets, to invalidate when additional
//      functions could be called at call sites where we inlined a function.
//
// This class uses bare GC-thing pointers because GC is suppressed when the
// analysis runs.
class MOZ_RAII DPAConstraintInfo {
  struct ProtoConstraint {
    JSObject* proto;
    jsid id;
    ProtoConstraint(JSObject* proto, jsid id) : proto(proto), id(id) {}
  };
  struct InliningConstraint {
    JSScript* caller;
    JSScript* callee;
    InliningConstraint(JSScript* caller, JSScript* callee)
        : caller(caller), callee(callee) {}
  };

  JS::AutoCheckCannotGC nogc_;
  Vector<ProtoConstraint, 8> protoConstraints_;
  Vector<InliningConstraint, 4> inliningConstraints_;

 public:
  explicit DPAConstraintInfo(JSContext* cx)
      : nogc_(cx), protoConstraints_(cx), inliningConstraints_(cx) {}

  DPAConstraintInfo(const DPAConstraintInfo&) = delete;
  void operator=(const DPAConstraintInfo&) = delete;

  MOZ_MUST_USE bool addProtoConstraint(JSObject* proto, jsid id) {
    return protoConstraints_.emplaceBack(proto, id);
  }
  MOZ_MUST_USE bool addInliningConstraint(JSScript* caller, JSScript* callee) {
    return inliningConstraints_.emplaceBack(caller, callee);
  }

  MOZ_MUST_USE bool finishConstraints(JSContext* cx, ObjectGroup* group);
};
>>>>>>> upstream-releases

<<<<<<< HEAD
bool AddClearDefiniteGetterSetterForPrototypeChain(JSContext* cx,
                                                   ObjectGroup* group,
                                                   HandleId id);
||||||| merged common ancestors
bool
AddClearDefiniteGetterSetterForPrototypeChain(JSContext* cx, ObjectGroup* group, HandleId id);
=======
bool AddClearDefiniteGetterSetterForPrototypeChain(
    JSContext* cx, DPAConstraintInfo& constraintInfo, ObjectGroup* group,
    HandleId id, bool* added);
>>>>>>> upstream-releases

bool AddClearDefiniteFunctionUsesInScript(JSContext* cx, ObjectGroup* group,
                                          JSScript* script,
                                          JSScript* calleeScript);

// For groups where only a small number of objects have been allocated, this
// structure keeps track of all objects in the group. Once COUNT objects have
// been allocated, this structure is cleared and the objects are analyzed, to
// perform the new script properties analyses or determine if an unboxed
// representation can be used.
class PreliminaryObjectArray {
 public:
  static const uint32_t COUNT = 20;

 private:
  // All objects with the type which have been allocated. The pointers in
  // this array are weak.
  JSObject* objects[COUNT] = {};  // zeroes

 public:
  PreliminaryObjectArray() = default;

  void registerNewObject(PlainObject* res);
  void unregisterObject(PlainObject* obj);

  JSObject* get(size_t i) const {
    MOZ_ASSERT(i < COUNT);
    return objects[i];
  }

  bool full() const;
  bool empty() const;
  void sweep();
};

class PreliminaryObjectArrayWithTemplate : public PreliminaryObjectArray {
  HeapPtr<Shape*> shape_;

 public:
  explicit PreliminaryObjectArrayWithTemplate(Shape* shape) : shape_(shape) {}

  void clear() { shape_.init(nullptr); }

  Shape* shape() { return shape_; }

  void maybeAnalyze(JSContext* cx, ObjectGroup* group, bool force = false);

  void trace(JSTracer* trc);

  static void writeBarrierPre(
      PreliminaryObjectArrayWithTemplate* preliminaryObjects);
};

/**
 * A type representing the initializer of a property within a script being
 * 'new'd.
 */
<<<<<<< HEAD
class TypeNewScriptInitializer {
 public:
  enum Kind { SETPROP, SETPROP_FRAME, DONE } kind;
  uint32_t offset;

  TypeNewScriptInitializer(Kind kind, uint32_t offset)
      : kind(kind), offset(offset) {}
||||||| merged common ancestors
class TypeNewScriptInitializer
{
  public:
    enum Kind
    {
        SETPROP,
        SETPROP_FRAME,
        DONE
    } kind;
    uint32_t offset;

    TypeNewScriptInitializer(Kind kind, uint32_t offset)
      : kind(kind), offset(offset)
    {}
=======
class TypeNewScriptInitializer {
 public:
  enum Kind { SETPROP, SETPROP_FRAME } kind;
  uint32_t offset;

  TypeNewScriptInitializer(Kind kind, uint32_t offset)
      : kind(kind), offset(offset) {}
>>>>>>> upstream-releases
};

/* Is this a reasonable PC to be doing inlining on? */
inline bool isInlinableCall(jsbytecode* pc);

bool ClassCanHaveExtraProperties(const Class* clasp);

class RecompileInfo {
  JSScript* script_;
  IonCompilationId id_;

 public:
  RecompileInfo(JSScript* script, IonCompilationId id)
      : script_(script), id_(id) {}

  JSScript* script() const { return script_; }

  inline jit::IonScript* maybeIonScriptToInvalidate(const TypeZone& zone) const;

  inline bool shouldSweep(const TypeZone& zone);

  bool operator==(const RecompileInfo& other) const {
    return script_ == other.script_ && id_ == other.id_;
  }
};

// The RecompileInfoVector has a MinInlineCapacity of one so that invalidating a
// single IonScript doesn't require an allocation.
typedef Vector<RecompileInfo, 1, SystemAllocPolicy> RecompileInfoVector;

<<<<<<< HEAD
/* Persistent type information for a script, retained across GCs. */
class TypeScript {
  friend class ::JSScript;

  // The freeze constraints added to stack type sets will only directly
  // invalidate the script containing those stack type sets. This Vector
  // contains compilations that inlined this script, so we can invalidate
  // them as well.
  RecompileInfoVector inlinedCompilations_;

  // ICScript and TypeScript have the same lifetimes, so we store a pointer to
  // ICScript here to not increase sizeof(JSScript).
  js::UniquePtr<js::jit::ICScript> icScript_;

  // Variable-size array
  StackTypeSet typeArray_[1];

 public:
  RecompileInfoVector& inlinedCompilations() { return inlinedCompilations_; }
  MOZ_MUST_USE bool addInlinedCompilation(RecompileInfo info) {
    if (!inlinedCompilations_.empty() && inlinedCompilations_.back() == info) {
      return true;
    }
    return inlinedCompilations_.append(info);
  }

  jit::ICScript* icScript() const {
    MOZ_ASSERT(icScript_);
    return icScript_.get();
  }

  /* Array of type sets for variables and JOF_TYPESET ops. */
  StackTypeSet* typeArray() const {
    // Ensure typeArray_ is the last data member of TypeScript.
    JS_STATIC_ASSERT(sizeof(TypeScript) ==
                     sizeof(typeArray_) + offsetof(TypeScript, typeArray_));
    return const_cast<StackTypeSet*>(typeArray_);
  }

  static inline size_t SizeIncludingTypeArray(size_t arraySize) {
    // Ensure typeArray_ is the last data member of TypeScript.
    JS_STATIC_ASSERT(sizeof(TypeScript) ==
                     sizeof(StackTypeSet) + offsetof(TypeScript, typeArray_));
    return offsetof(TypeScript, typeArray_) + arraySize * sizeof(StackTypeSet);
  }

  static inline unsigned NumTypeSets(JSScript* script);

  static inline StackTypeSet* ThisTypes(JSScript* script);
  static inline StackTypeSet* ArgTypes(JSScript* script, unsigned i);

  /* Get the type set for values observed at an opcode. */
  static inline StackTypeSet* BytecodeTypes(JSScript* script, jsbytecode* pc);

  template <typename TYPESET>
  static inline TYPESET* BytecodeTypes(JSScript* script, jsbytecode* pc,
                                       uint32_t* bytecodeMap, uint32_t* hint,
                                       TYPESET* typeArray);

  /*
   * Monitor a bytecode pushing any value. This must be called for any opcode
   * which is JOF_TYPESET, and where either the script has not been analyzed
   * by type inference or where the pc has type barriers. For simplicity, we
   * always monitor JOF_TYPESET opcodes in the interpreter and stub calls,
   * and only look at barriers when generating JIT code for the script.
   */
  static inline void Monitor(JSContext* cx, JSScript* script, jsbytecode* pc,
                             const js::Value& val);
  static inline void Monitor(JSContext* cx, JSScript* script, jsbytecode* pc,
                             TypeSet::Type type);
  static inline void Monitor(JSContext* cx, const js::Value& rval);

  static inline void Monitor(JSContext* cx, JSScript* script, jsbytecode* pc,
                             StackTypeSet* types, const js::Value& val);

  /* Monitor an assignment at a SETELEM on a non-integer identifier. */
  static inline void MonitorAssign(JSContext* cx, HandleObject obj, jsid id);

  /* Add a type for a variable in a script. */
  static inline void SetThis(JSContext* cx, JSScript* script,
                             TypeSet::Type type);
  static inline void SetThis(JSContext* cx, JSScript* script,
                             const js::Value& value);
  static inline void SetArgument(JSContext* cx, JSScript* script, unsigned arg,
                                 TypeSet::Type type);
  static inline void SetArgument(JSContext* cx, JSScript* script, unsigned arg,
                                 const js::Value& value);

  /*
   * Freeze all the stack type sets in a script, for a compilation. Returns
   * copies of the type sets which will be checked against the actual ones
   * under FinishCompilation, to detect any type changes.
   */
  static bool FreezeTypeSets(CompilerConstraintList* constraints,
                             JSScript* script, TemporaryTypeSet** pThisTypes,
                             TemporaryTypeSet** pArgTypes,
                             TemporaryTypeSet** pBytecodeTypes);

  static void Purge(JSContext* cx, HandleScript script);

  void destroy(Zone* zone);

  size_t sizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf) const {
    // Note: icScript_ size is reported in jit::AddSizeOfBaselineData.
    return mallocSizeOf(this);
  }

#ifdef DEBUG
  void printTypes(JSContext* cx, HandleScript script) const;
#endif
};

// Ensures no TypeScripts are purged in the current zone.
class MOZ_RAII AutoKeepTypeScripts {
  TypeZone& zone_;
  bool prev_;

  AutoKeepTypeScripts(const AutoKeepTypeScripts&) = delete;
  void operator=(const AutoKeepTypeScripts&) = delete;

 public:
  explicit inline AutoKeepTypeScripts(JSContext* cx);
  inline ~AutoKeepTypeScripts();
};

void FillBytecodeTypeMap(JSScript* script, uint32_t* bytecodeMap);

class RecompileInfo;

||||||| merged common ancestors
/* Persistent type information for a script, retained across GCs. */
class TypeScript
{
    friend class ::JSScript;

    // The freeze constraints added to stack type sets will only directly
    // invalidate the script containing those stack type sets. This Vector
    // contains compilations that inlined this script, so we can invalidate
    // them as well.
    RecompileInfoVector inlinedCompilations_;

    // Variable-size array
    StackTypeSet typeArray_[1];

  public:
    RecompileInfoVector& inlinedCompilations() {
        return inlinedCompilations_;
    }
    MOZ_MUST_USE bool addInlinedCompilation(RecompileInfo info) {
        if (!inlinedCompilations_.empty() && inlinedCompilations_.back() == info) {
            return true;
        }
        return inlinedCompilations_.append(info);
    }

    /* Array of type sets for variables and JOF_TYPESET ops. */
    StackTypeSet* typeArray() const {
        // Ensure typeArray_ is the last data member of TypeScript.
        JS_STATIC_ASSERT(sizeof(TypeScript) ==
                         sizeof(typeArray_) + offsetof(TypeScript, typeArray_));
        return const_cast<StackTypeSet*>(typeArray_);
    }

    static inline size_t SizeIncludingTypeArray(size_t arraySize) {
        // Ensure typeArray_ is the last data member of TypeScript.
        JS_STATIC_ASSERT(sizeof(TypeScript) ==
            sizeof(StackTypeSet) + offsetof(TypeScript, typeArray_));
        return offsetof(TypeScript, typeArray_) + arraySize * sizeof(StackTypeSet);
    }

    static inline unsigned NumTypeSets(JSScript* script);

    static inline StackTypeSet* ThisTypes(JSScript* script);
    static inline StackTypeSet* ArgTypes(JSScript* script, unsigned i);

    /* Get the type set for values observed at an opcode. */
    static inline StackTypeSet* BytecodeTypes(JSScript* script, jsbytecode* pc);

    template <typename TYPESET>
    static inline TYPESET* BytecodeTypes(JSScript* script, jsbytecode* pc, uint32_t* bytecodeMap,
                                         uint32_t* hint, TYPESET* typeArray);

    /*
     * Monitor a bytecode pushing any value. This must be called for any opcode
     * which is JOF_TYPESET, and where either the script has not been analyzed
     * by type inference or where the pc has type barriers. For simplicity, we
     * always monitor JOF_TYPESET opcodes in the interpreter and stub calls,
     * and only look at barriers when generating JIT code for the script.
     */
    static inline void Monitor(JSContext* cx, JSScript* script, jsbytecode* pc,
                               const js::Value& val);
    static inline void Monitor(JSContext* cx, JSScript* script, jsbytecode* pc,
                               TypeSet::Type type);
    static inline void Monitor(JSContext* cx, const js::Value& rval);

    static inline void Monitor(JSContext* cx, JSScript* script, jsbytecode* pc,
                               StackTypeSet* types, const js::Value& val);

    /* Monitor an assignment at a SETELEM on a non-integer identifier. */
    static inline void MonitorAssign(JSContext* cx, HandleObject obj, jsid id);

    /* Add a type for a variable in a script. */
    static inline void SetThis(JSContext* cx, JSScript* script, TypeSet::Type type);
    static inline void SetThis(JSContext* cx, JSScript* script, const js::Value& value);
    static inline void SetArgument(JSContext* cx, JSScript* script, unsigned arg,
                                   TypeSet::Type type);
    static inline void SetArgument(JSContext* cx, JSScript* script, unsigned arg,
                                   const js::Value& value);

    /*
     * Freeze all the stack type sets in a script, for a compilation. Returns
     * copies of the type sets which will be checked against the actual ones
     * under FinishCompilation, to detect any type changes.
     */
    static bool FreezeTypeSets(CompilerConstraintList* constraints, JSScript* script,
                               TemporaryTypeSet** pThisTypes,
                               TemporaryTypeSet** pArgTypes,
                               TemporaryTypeSet** pBytecodeTypes);

    static void Purge(JSContext* cx, HandleScript script);

    void destroy();

    size_t sizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf) const {
        return mallocSizeOf(this);
    }

#ifdef DEBUG
    void printTypes(JSContext* cx, HandleScript script) const;
#endif
};

// Ensures no TypeScripts are purged in the current zone.
class MOZ_RAII AutoKeepTypeScripts
{
    TypeZone& zone_;
    bool prev_;

    AutoKeepTypeScripts(const AutoKeepTypeScripts&) = delete;
    void operator=(const AutoKeepTypeScripts&) = delete;

  public:
    explicit inline AutoKeepTypeScripts(JSContext* cx);
    inline ~AutoKeepTypeScripts();
};

void
FillBytecodeTypeMap(JSScript* script, uint32_t* bytecodeMap);

class RecompileInfo;

=======
>>>>>>> upstream-releases
// Generate the type constraints for the compilation. Sets |isValidOut| based on
// whether the type constraints still hold.
bool FinishCompilation(JSContext* cx, HandleScript script,
                       CompilerConstraintList* constraints,
                       IonCompilationId compilationId, bool* isValidOut);

// Update the actual types in any scripts queried by constraints with any
// speculative types added during the definite properties analysis.
void FinishDefinitePropertiesAnalysis(JSContext* cx,
                                      CompilerConstraintList* constraints);

struct AutoEnterAnalysis;

<<<<<<< HEAD
class TypeZone {
  JS::Zone* const zone_;
||||||| merged common ancestors
class TypeZone
{
    JS::Zone* const zone_;

    /* Pool for type information in this zone. */
    static const size_t TYPE_LIFO_ALLOC_PRIMARY_CHUNK_SIZE = 8 * 1024;
    ZoneData<LifoAlloc> typeLifoAlloc_;
=======
class TypeZone {
  JS::Zone* const zone_;

  /* Pool for type information in this zone. */
  static const size_t TYPE_LIFO_ALLOC_PRIMARY_CHUNK_SIZE = 8 * 1024;
  ZoneData<LifoAlloc> typeLifoAlloc_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Pool for type information in this zone. */
  static const size_t TYPE_LIFO_ALLOC_PRIMARY_CHUNK_SIZE = 8 * 1024;
  ZoneData<LifoAlloc> typeLifoAlloc_;
||||||| merged common ancestors
    // Under CodeGenerator::link, the id of the current compilation.
    ZoneData<mozilla::Maybe<IonCompilationId>> currentCompilationId_;
=======
  // Under CodeGenerator::link, the id of the current compilation.
  ZoneData<mozilla::Maybe<IonCompilationId>> currentCompilationId_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Under CodeGenerator::link, the id of the current compilation.
  ZoneData<mozilla::Maybe<IonCompilationId>> currentCompilationId_;
||||||| merged common ancestors
    TypeZone(const TypeZone&) = delete;
    void operator=(const TypeZone&) = delete;
=======
  TypeZone(const TypeZone&) = delete;
  void operator=(const TypeZone&) = delete;
>>>>>>> upstream-releases

<<<<<<< HEAD
  TypeZone(const TypeZone&) = delete;
  void operator=(const TypeZone&) = delete;
||||||| merged common ancestors
  public:
    // Current generation for sweeping.
    ZoneOrGCTaskOrIonCompileData<uint32_t> generation;
=======
 public:
  // Current generation for sweeping.
  ZoneOrGCTaskOrIonCompileData<uint32_t> generation;
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  // Current generation for sweeping.
  ZoneOrGCTaskOrIonCompileData<uint32_t> generation;
||||||| merged common ancestors
    // During incremental sweeping, allocator holding the old type information
    // for the zone.
    ZoneData<LifoAlloc> sweepTypeLifoAlloc;
=======
  // During incremental sweeping, allocator holding the old type information
  // for the zone.
  ZoneData<LifoAlloc> sweepTypeLifoAlloc;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // During incremental sweeping, allocator holding the old type information
  // for the zone.
  ZoneData<LifoAlloc> sweepTypeLifoAlloc;
||||||| merged common ancestors
    // During incremental sweeping, whether to try to destroy all type
    // information attached to scripts.
    ZoneData<bool> sweepReleaseTypes;
=======
  ZoneData<bool> sweepingTypes;
  ZoneData<bool> oomSweepingTypes;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ZoneData<bool> sweepingTypes;
  ZoneData<bool> oomSweepingTypes;
||||||| merged common ancestors
    ZoneData<bool> sweepingTypes;
    ZoneData<bool> oomSweepingTypes;
=======
  ZoneData<bool> keepJitScripts;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ZoneData<bool> keepTypeScripts;
||||||| merged common ancestors
    ZoneData<bool> keepTypeScripts;
=======
  // The topmost AutoEnterAnalysis on the stack, if there is one.
  ZoneData<AutoEnterAnalysis*> activeAnalysis;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The topmost AutoEnterAnalysis on the stack, if there is one.
  ZoneData<AutoEnterAnalysis*> activeAnalysis;
||||||| merged common ancestors
    // The topmost AutoEnterAnalysis on the stack, if there is one.
    ZoneData<AutoEnterAnalysis*> activeAnalysis;
=======
  explicit TypeZone(JS::Zone* zone);
  ~TypeZone();
>>>>>>> upstream-releases

<<<<<<< HEAD
  explicit TypeZone(JS::Zone* zone);
  ~TypeZone();
||||||| merged common ancestors
    explicit TypeZone(JS::Zone* zone);
    ~TypeZone();
=======
  JS::Zone* zone() const { return zone_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::Zone* zone() const { return zone_; }

  LifoAlloc& typeLifoAlloc() {
||||||| merged common ancestors
    JS::Zone* zone() const { return zone_; }

    LifoAlloc& typeLifoAlloc() {
=======
  LifoAlloc& typeLifoAlloc() {
>>>>>>> upstream-releases
#ifdef JS_CRASH_DIAGNOSTICS
    MOZ_RELEASE_ASSERT(CurrentThreadCanAccessZone(zone_));
#endif
    return typeLifoAlloc_.ref();
  }

  void beginSweep();
  void endSweep(JSRuntime* rt);
  void clearAllNewScriptsOnOOM();

  /* Mark a script as needing recompilation once inference has finished. */
  void addPendingRecompile(JSContext* cx, const RecompileInfo& info);
  void addPendingRecompile(JSContext* cx, JSScript* script);

  void processPendingRecompiles(FreeOp* fop, RecompileInfoVector& recompiles);

  bool isSweepingTypes() const { return sweepingTypes; }
  void setSweepingTypes(bool sweeping) {
    MOZ_RELEASE_ASSERT(sweepingTypes != sweeping);
    MOZ_ASSERT_IF(sweeping, !oomSweepingTypes);
    sweepingTypes = sweeping;
    oomSweepingTypes = false;
  }
  void setOOMSweepingTypes() {
    MOZ_ASSERT(sweepingTypes);
    oomSweepingTypes = true;
  }
  bool hadOOMSweepingTypes() {
    MOZ_ASSERT(sweepingTypes);
    return oomSweepingTypes;
  }

  mozilla::Maybe<IonCompilationId> currentCompilationId() const {
    return currentCompilationId_.ref();
  }
  mozilla::Maybe<IonCompilationId>& currentCompilationIdRef() {
    return currentCompilationId_.ref();
  }
};

enum TypeSpewChannel {
  ISpewOps,    /* ops: New constraints and types. */
  ISpewResult, /* result: Final type sets. */
  SPEW_COUNT
};

#ifdef DEBUG

bool InferSpewActive(TypeSpewChannel channel);
const char* InferSpewColorReset();
const char* InferSpewColor(TypeConstraint* constraint);
const char* InferSpewColor(TypeSet* types);

<<<<<<< HEAD
#define InferSpew(channel, ...)   \
  if (InferSpewActive(channel)) { \
    InferSpewImpl(__VA_ARGS__);   \
  } else {                        \
  }
||||||| merged common ancestors
#define InferSpew(channel, ...) if (InferSpewActive(channel)) { InferSpewImpl(__VA_ARGS__); } else {}
=======
#  define InferSpew(channel, ...)   \
    if (InferSpewActive(channel)) { \
      InferSpewImpl(__VA_ARGS__);   \
    } else {                        \
    }
>>>>>>> upstream-releases
void InferSpewImpl(const char* fmt, ...) MOZ_FORMAT_PRINTF(1, 2);

/* Check that the type property for id in group contains value. */
bool ObjectGroupHasProperty(JSContext* cx, ObjectGroup* group, jsid id,
                            const Value& value);

#else

inline const char* InferSpewColorReset() { return nullptr; }
inline const char* InferSpewColor(TypeConstraint* constraint) {
  return nullptr;
}
inline const char* InferSpewColor(TypeSet* types) { return nullptr; }

<<<<<<< HEAD
#define InferSpew(channel, ...) \
  do {                          \
  } while (0)
||||||| merged common ancestors
#define InferSpew(channel, ...) do {} while (0)
=======
#  define InferSpew(channel, ...) \
    do {                          \
    } while (0)
>>>>>>> upstream-releases

#endif

// Prints type information for a context if spew is enabled or force is set.
void PrintTypes(JSContext* cx, JS::Compartment* comp, bool force);

} /* namespace js */

// JS::ubi::Nodes can point to object groups; they're js::gc::Cell instances
// with no associated compartment.
namespace JS {
namespace ubi {

template <>
class Concrete<js::ObjectGroup> : TracerConcrete<js::ObjectGroup> {
 protected:
  explicit Concrete(js::ObjectGroup* ptr)
      : TracerConcrete<js::ObjectGroup>(ptr) {}

 public:
  static void construct(void* storage, js::ObjectGroup* ptr) {
    new (storage) Concrete(ptr);
  }

  Size size(mozilla::MallocSizeOf mallocSizeOf) const override;

  const char16_t* typeName() const override { return concreteTypeName; }
  static const char16_t concreteTypeName[];
};

}  // namespace ubi
}  // namespace JS

#endif /* vm_TypeInference_h */
