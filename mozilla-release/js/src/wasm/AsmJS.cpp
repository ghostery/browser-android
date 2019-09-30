/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 *
 * Copyright 2014 Mozilla Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "wasm/AsmJS.h"

#include "mozilla/ArrayUtils.h"
#include "mozilla/Attributes.h"
#include "mozilla/Compression.h"
#include "mozilla/MathAlgorithms.h"
#include "mozilla/ScopeExit.h"
#include "mozilla/Sprintf.h"  // SprintfLiteral
#include "mozilla/Unused.h"
#include "mozilla/Utf8.h"  // mozilla::Utf8Unit
#include "mozilla/Variant.h"

#include <new>

#include "jsmath.h"
#include "jsutil.h"

#include "builtin/String.h"
#include "frontend/ParseNode.h"
#include "frontend/Parser.h"
#include "gc/Policy.h"
#include "js/BuildId.h"  // JS::BuildIdCharVector
#include "js/MemoryMetrics.h"
#include "js/Printf.h"
#include "js/SourceText.h"
#include "js/StableStringChars.h"
#include "js/Wrapper.h"
#include "util/StringBuffer.h"
#include "util/Text.h"
#include "vm/ErrorReporting.h"
#include "vm/SelfHosting.h"
#include "vm/Time.h"
#include "vm/TypedArrayObject.h"
#include "wasm/WasmCompile.h"
#include "wasm/WasmGenerator.h"
#include "wasm/WasmInstance.h"
#include "wasm/WasmIonCompile.h"
#include "wasm/WasmJS.h"
#include "wasm/WasmSerialize.h"
#include "wasm/WasmValidate.h"

#include "frontend/SharedContext-inl.h"
#include "vm/ArrayBufferObject-inl.h"
#include "vm/JSObject-inl.h"

using namespace js;
using namespace js::frontend;
using namespace js::jit;
using namespace js::wasm;

<<<<<<< HEAD
using JS::AsmJSOption;
using JS::AutoStableStringChars;
using JS::GenericNaN;
using JS::SourceOwnership;
using JS::SourceText;
||||||| merged common ancestors
=======
using JS::AsmJSOption;
using JS::AutoStableStringChars;
using JS::GenericNaN;
using JS::SourceOwnership;
using JS::SourceText;
using mozilla::Abs;
>>>>>>> upstream-releases
using mozilla::ArrayEqual;
using mozilla::AsVariant;
using mozilla::CeilingLog2;
using mozilla::HashGeneric;
using mozilla::IsNaN;
using mozilla::IsNegativeZero;
using mozilla::IsPositiveZero;
using mozilla::IsPowerOfTwo;
using mozilla::PodZero;
using mozilla::PositiveInfinity;
using mozilla::Unused;
<<<<<<< HEAD
using mozilla::Compression::LZ4;
||||||| merged common ancestors
using JS::AsmJSOption;
using JS::AutoStableStringChars;
using JS::GenericNaN;
using JS::SourceBufferHolder;
=======
using mozilla::Utf8Unit;
using mozilla::Compression::LZ4;
>>>>>>> upstream-releases

/*****************************************************************************/

// The asm.js valid heap lengths are precisely the WASM valid heap lengths for
// ARM greater or equal to MinHeapLength
static const size_t MinHeapLength = PageSize;

static uint32_t RoundUpToNextValidAsmJSHeapLength(uint32_t length) {
  if (length <= MinHeapLength) {
    return MinHeapLength;
  }

  return wasm::RoundUpToNextValidARMImmediate(length);
}

/*****************************************************************************/
// asm.js module object

// The asm.js spec recognizes this set of builtin Math functions.
enum AsmJSMathBuiltinFunction {
  AsmJSMathBuiltin_sin,
  AsmJSMathBuiltin_cos,
  AsmJSMathBuiltin_tan,
  AsmJSMathBuiltin_asin,
  AsmJSMathBuiltin_acos,
  AsmJSMathBuiltin_atan,
  AsmJSMathBuiltin_ceil,
  AsmJSMathBuiltin_floor,
  AsmJSMathBuiltin_exp,
  AsmJSMathBuiltin_log,
  AsmJSMathBuiltin_pow,
  AsmJSMathBuiltin_sqrt,
  AsmJSMathBuiltin_abs,
  AsmJSMathBuiltin_atan2,
  AsmJSMathBuiltin_imul,
  AsmJSMathBuiltin_fround,
  AsmJSMathBuiltin_min,
  AsmJSMathBuiltin_max,
  AsmJSMathBuiltin_clz32
};

// LitValPOD is a restricted version of LitVal suitable for asm.js that is
// always POD.

struct LitValPOD {
  PackedTypeCode valType_;
  union U {
    uint32_t u32_;
    uint64_t u64_;
    float f32_;
    double f64_;
  } u;

  LitValPOD() = default;

  explicit LitValPOD(uint32_t u32) : valType_(ValType(ValType::I32).packed()) {
    u.u32_ = u32;
  }
  explicit LitValPOD(uint64_t u64) : valType_(ValType(ValType::I64).packed()) {
    u.u64_ = u64;
  }

  explicit LitValPOD(float f32) : valType_(ValType(ValType::F32).packed()) {
    u.f32_ = f32;
  }
  explicit LitValPOD(double f64) : valType_(ValType(ValType::F64).packed()) {
    u.f64_ = f64;
  }

  LitVal asLitVal() const {
    switch (UnpackTypeCodeType(valType_)) {
      case TypeCode::I32:
        return LitVal(u.u32_);
      case TypeCode::I64:
        return LitVal(u.u64_);
      case TypeCode::F32:
        return LitVal(u.f32_);
      case TypeCode::F64:
        return LitVal(u.f64_);
      default:
        MOZ_CRASH("Can't happen");
    }
  }
};

static_assert(std::is_pod<LitValPOD>::value,
              "must be POD to be simply serialized/deserialized");

// An AsmJSGlobal represents a JS global variable in the asm.js module function.
class AsmJSGlobal {
 public:
  enum Which {
    Variable,
    FFI,
    ArrayView,
    ArrayViewCtor,
    MathBuiltinFunction,
    Constant
  };
  enum VarInitKind { InitConstant, InitImport };
  enum ConstantKind { GlobalConstant, MathConstant };

 private:
  struct CacheablePod {
    Which which_;
    union V {
      struct {
        VarInitKind initKind_;
        union U {
          PackedTypeCode importValType_;
          LitValPOD val_;
        } u;
<<<<<<< HEAD
      } var;
      uint32_t ffiIndex_;
      Scalar::Type viewType_;
      AsmJSMathBuiltinFunction mathBuiltinFunc_;
      struct {
        ConstantKind kind_;
        double value_;
      } constant;
    } u;
  } pod;
  CacheableChars field_;

  friend class ModuleValidator;

 public:
  AsmJSGlobal() = default;
  AsmJSGlobal(Which which, UniqueChars field) {
    mozilla::PodZero(&pod);  // zero padding for Valgrind
    pod.which_ = which;
    field_ = std::move(field);
  }
  const char* field() const { return field_.get(); }
  Which which() const { return pod.which_; }
  VarInitKind varInitKind() const {
    MOZ_ASSERT(pod.which_ == Variable);
    return pod.u.var.initKind_;
  }
  LitValPOD varInitVal() const {
    MOZ_ASSERT(pod.which_ == Variable);
    MOZ_ASSERT(pod.u.var.initKind_ == InitConstant);
    return pod.u.var.u.val_;
  }
  ValType varInitImportType() const {
    MOZ_ASSERT(pod.which_ == Variable);
    MOZ_ASSERT(pod.u.var.initKind_ == InitImport);
    return ValType(pod.u.var.u.importValType_);
  }
  uint32_t ffiIndex() const {
    MOZ_ASSERT(pod.which_ == FFI);
    return pod.u.ffiIndex_;
  }
  // When a view is created from an imported constructor:
  //   var I32 = stdlib.Int32Array;
  //   var i32 = new I32(buffer);
  // the second import has nothing to validate and thus has a null field.
  Scalar::Type viewType() const {
    MOZ_ASSERT(pod.which_ == ArrayView || pod.which_ == ArrayViewCtor);
    return pod.u.viewType_;
  }
  AsmJSMathBuiltinFunction mathBuiltinFunction() const {
    MOZ_ASSERT(pod.which_ == MathBuiltinFunction);
    return pod.u.mathBuiltinFunc_;
  }
  ConstantKind constantKind() const {
    MOZ_ASSERT(pod.which_ == Constant);
    return pod.u.constant.kind_;
  }
  double constantValue() const {
    MOZ_ASSERT(pod.which_ == Constant);
    return pod.u.constant.value_;
  }

  WASM_DECLARE_SERIALIZABLE(AsmJSGlobal);
||||||| merged common ancestors
    } pod;
    CacheableChars field_;

    friend class ModuleValidator;

  public:
    AsmJSGlobal() = default;
    AsmJSGlobal(Which which, UniqueChars field) {
        mozilla::PodZero(&pod);  // zero padding for Valgrind
        pod.which_ = which;
        field_ = std::move(field);
    }
    const char* field() const {
        return field_.get();
    }
    Which which() const {
        return pod.which_;
    }
    VarInitKind varInitKind() const {
        MOZ_ASSERT(pod.which_ == Variable);
        return pod.u.var.initKind_;
    }
    LitValPOD varInitVal() const {
        MOZ_ASSERT(pod.which_ == Variable);
        MOZ_ASSERT(pod.u.var.initKind_ == InitConstant);
        return pod.u.var.u.val_;
    }
    ValType varInitImportType() const {
        MOZ_ASSERT(pod.which_ == Variable);
        MOZ_ASSERT(pod.u.var.initKind_ == InitImport);
        return ValType(pod.u.var.u.importValType_);
    }
    uint32_t ffiIndex() const {
        MOZ_ASSERT(pod.which_ == FFI);
        return pod.u.ffiIndex_;
    }
    // When a view is created from an imported constructor:
    //   var I32 = stdlib.Int32Array;
    //   var i32 = new I32(buffer);
    // the second import has nothing to validate and thus has a null field.
    Scalar::Type viewType() const {
        MOZ_ASSERT(pod.which_ == ArrayView || pod.which_ == ArrayViewCtor);
        return pod.u.viewType_;
    }
    AsmJSMathBuiltinFunction mathBuiltinFunction() const {
        MOZ_ASSERT(pod.which_ == MathBuiltinFunction);
        return pod.u.mathBuiltinFunc_;
    }
    ConstantKind constantKind() const {
        MOZ_ASSERT(pod.which_ == Constant);
        return pod.u.constant.kind_;
    }
    double constantValue() const {
        MOZ_ASSERT(pod.which_ == Constant);
        return pod.u.constant.value_;
    }

    WASM_DECLARE_SERIALIZABLE(AsmJSGlobal);
=======
      } var;
      uint32_t ffiIndex_;
      Scalar::Type viewType_;
      AsmJSMathBuiltinFunction mathBuiltinFunc_;
      struct {
        ConstantKind kind_;
        double value_;
      } constant;
    } u;
  } pod;
  CacheableChars field_;

  friend class ModuleValidatorShared;
  template <typename Unit>
  friend class ModuleValidator;

 public:
  AsmJSGlobal() = default;
  AsmJSGlobal(Which which, UniqueChars field) {
    mozilla::PodZero(&pod);  // zero padding for Valgrind
    pod.which_ = which;
    field_ = std::move(field);
  }
  const char* field() const { return field_.get(); }
  Which which() const { return pod.which_; }
  VarInitKind varInitKind() const {
    MOZ_ASSERT(pod.which_ == Variable);
    return pod.u.var.initKind_;
  }
  LitValPOD varInitVal() const {
    MOZ_ASSERT(pod.which_ == Variable);
    MOZ_ASSERT(pod.u.var.initKind_ == InitConstant);
    return pod.u.var.u.val_;
  }
  ValType varInitImportType() const {
    MOZ_ASSERT(pod.which_ == Variable);
    MOZ_ASSERT(pod.u.var.initKind_ == InitImport);
    return ValType(pod.u.var.u.importValType_);
  }
  uint32_t ffiIndex() const {
    MOZ_ASSERT(pod.which_ == FFI);
    return pod.u.ffiIndex_;
  }
  // When a view is created from an imported constructor:
  //   var I32 = stdlib.Int32Array;
  //   var i32 = new I32(buffer);
  // the second import has nothing to validate and thus has a null field.
  Scalar::Type viewType() const {
    MOZ_ASSERT(pod.which_ == ArrayView || pod.which_ == ArrayViewCtor);
    return pod.u.viewType_;
  }
  AsmJSMathBuiltinFunction mathBuiltinFunction() const {
    MOZ_ASSERT(pod.which_ == MathBuiltinFunction);
    return pod.u.mathBuiltinFunc_;
  }
  ConstantKind constantKind() const {
    MOZ_ASSERT(pod.which_ == Constant);
    return pod.u.constant.kind_;
  }
  double constantValue() const {
    MOZ_ASSERT(pod.which_ == Constant);
    return pod.u.constant.value_;
  }
>>>>>>> upstream-releases
};

typedef Vector<AsmJSGlobal, 0, SystemAllocPolicy> AsmJSGlobalVector;

// An AsmJSImport is slightly different than an asm.js FFI function: a single
// asm.js FFI function can be called with many different signatures. When
// compiled to wasm, each unique FFI function paired with signature generates a
// wasm import.
class AsmJSImport {
  uint32_t ffiIndex_;

 public:
  AsmJSImport() = default;
  explicit AsmJSImport(uint32_t ffiIndex) : ffiIndex_(ffiIndex) {}
  uint32_t ffiIndex() const { return ffiIndex_; }
};

typedef Vector<AsmJSImport, 0, SystemAllocPolicy> AsmJSImportVector;

// An AsmJSExport logically extends Export with the extra information needed for
// an asm.js exported function, viz., the offsets in module's source chars in
// case the function is toString()ed.
class AsmJSExport {
  uint32_t funcIndex_ = 0;

  // All fields are treated as cacheable POD:
  uint32_t startOffsetInModule_ = 0;  // Store module-start-relative offsets
  uint32_t endOffsetInModule_ = 0;    // so preserved by serialization.

 public:
  AsmJSExport() = default;
  AsmJSExport(uint32_t funcIndex, uint32_t startOffsetInModule,
              uint32_t endOffsetInModule)
      : funcIndex_(funcIndex),
        startOffsetInModule_(startOffsetInModule),
        endOffsetInModule_(endOffsetInModule) {}
  uint32_t funcIndex() const { return funcIndex_; }
  uint32_t startOffsetInModule() const { return startOffsetInModule_; }
  uint32_t endOffsetInModule() const { return endOffsetInModule_; }
};

typedef Vector<AsmJSExport, 0, SystemAllocPolicy> AsmJSExportVector;

<<<<<<< HEAD
enum class CacheResult { Hit, Miss };

||||||| merged common ancestors
enum class CacheResult
{
    Hit,
    Miss
};

=======
>>>>>>> upstream-releases
// Holds the immutable guts of an AsmJSModule.
//
// AsmJSMetadata is built incrementally by ModuleValidator and then shared
// immutably between AsmJSModules.

struct AsmJSMetadataCacheablePod {
  uint32_t numFFIs = 0;
  uint32_t srcLength = 0;
  uint32_t srcLengthWithRightBrace = 0;

  AsmJSMetadataCacheablePod() = default;
};

<<<<<<< HEAD
struct js::AsmJSMetadata : Metadata, AsmJSMetadataCacheablePod {
  AsmJSGlobalVector asmJSGlobals;
  AsmJSImportVector asmJSImports;
  AsmJSExportVector asmJSExports;
  CacheableCharsVector asmJSFuncNames;
  CacheableChars globalArgumentName;
  CacheableChars importArgumentName;
  CacheableChars bufferArgumentName;

  CacheResult cacheResult;

  // These values are not serialized since they are relative to the
  // containing script which can be different between serialization and
  // deserialization contexts. Thus, they must be set explicitly using the
  // ambient Parser/ScriptSource after deserialization.
  //
  // srcStart refers to the offset in the ScriptSource to the beginning of
  // the asm.js module function. If the function has been created with the
  // Function constructor, this will be the first character in the function
  // source. Otherwise, it will be the opening parenthesis of the arguments
  // list.
  uint32_t toStringStart;
  uint32_t srcStart;
  bool strict;
  ScriptSourceHolder scriptSource;

  uint32_t srcEndBeforeCurly() const { return srcStart + srcLength; }
  uint32_t srcEndAfterCurly() const {
    return srcStart + srcLengthWithRightBrace;
  }

  AsmJSMetadata()
||||||| merged common ancestors
struct js::AsmJSMetadata : Metadata, AsmJSMetadataCacheablePod
{
    AsmJSGlobalVector       asmJSGlobals;
    AsmJSImportVector       asmJSImports;
    AsmJSExportVector       asmJSExports;
    CacheableCharsVector    asmJSFuncNames;
    CacheableChars          globalArgumentName;
    CacheableChars          importArgumentName;
    CacheableChars          bufferArgumentName;

    CacheResult             cacheResult;

    // These values are not serialized since they are relative to the
    // containing script which can be different between serialization and
    // deserialization contexts. Thus, they must be set explicitly using the
    // ambient Parser/ScriptSource after deserialization.
    //
    // srcStart refers to the offset in the ScriptSource to the beginning of
    // the asm.js module function. If the function has been created with the
    // Function constructor, this will be the first character in the function
    // source. Otherwise, it will be the opening parenthesis of the arguments
    // list.
    uint32_t                toStringStart;
    uint32_t                srcStart;
    bool                    strict;
    ScriptSourceHolder      scriptSource;

    uint32_t srcEndBeforeCurly() const {
        return srcStart + srcLength;
    }
    uint32_t srcEndAfterCurly() const {
        return srcStart + srcLengthWithRightBrace;
    }

    AsmJSMetadata()
=======
struct js::AsmJSMetadata : Metadata, AsmJSMetadataCacheablePod {
  AsmJSGlobalVector asmJSGlobals;
  AsmJSImportVector asmJSImports;
  AsmJSExportVector asmJSExports;
  CacheableCharsVector asmJSFuncNames;
  CacheableChars globalArgumentName;
  CacheableChars importArgumentName;
  CacheableChars bufferArgumentName;

  // These values are not serialized since they are relative to the
  // containing script which can be different between serialization and
  // deserialization contexts. Thus, they must be set explicitly using the
  // ambient Parser/ScriptSource after deserialization.
  //
  // srcStart refers to the offset in the ScriptSource to the beginning of
  // the asm.js module function. If the function has been created with the
  // Function constructor, this will be the first character in the function
  // source. Otherwise, it will be the opening parenthesis of the arguments
  // list.
  uint32_t toStringStart;
  uint32_t srcStart;
  bool strict;
  ScriptSourceHolder scriptSource;

  uint32_t srcEndBeforeCurly() const { return srcStart + srcLength; }
  uint32_t srcEndAfterCurly() const {
    return srcStart + srcLengthWithRightBrace;
  }

  AsmJSMetadata()
>>>>>>> upstream-releases
      : Metadata(ModuleKind::AsmJS),
        toStringStart(0),
        srcStart(0),
        strict(false) {}
  ~AsmJSMetadata() override {}

  const AsmJSExport& lookupAsmJSExport(uint32_t funcIndex) const {
    // The AsmJSExportVector isn't stored in sorted order so do a linear
    // search. This is for the super-cold and already-expensive toString()
    // path and the number of exports is generally small.
    for (const AsmJSExport& exp : asmJSExports) {
      if (exp.funcIndex() == funcIndex) {
        return exp;
      }
    }
<<<<<<< HEAD
    MOZ_CRASH("missing asm.js func export");
  }

  bool mutedErrors() const override {
    return scriptSource.get()->mutedErrors();
  }
  const char16_t* displayURL() const override {
    return scriptSource.get()->hasDisplayURL()
               ? scriptSource.get()->displayURL()
               : nullptr;
  }
  ScriptSource* maybeScriptSource() const override {
    return scriptSource.get();
  }
  bool getFuncName(NameContext ctx, uint32_t funcIndex,
                   UTF8Bytes* name) const override {
    const char* p = asmJSFuncNames[funcIndex].get();
    if (!p) {
      return true;
    }
    return name->append(p, strlen(p));
  }

  AsmJSMetadataCacheablePod& pod() { return *this; }
  const AsmJSMetadataCacheablePod& pod() const { return *this; }

  WASM_DECLARE_SERIALIZABLE_OVERRIDE(AsmJSMetadata)
||||||| merged common ancestors

    AsmJSMetadataCacheablePod& pod() { return *this; }
    const AsmJSMetadataCacheablePod& pod() const { return *this; }

    WASM_DECLARE_SERIALIZABLE_OVERRIDE(AsmJSMetadata)
=======
    MOZ_CRASH("missing asm.js func export");
  }

  bool mutedErrors() const override {
    return scriptSource.get()->mutedErrors();
  }
  const char16_t* displayURL() const override {
    return scriptSource.get()->hasDisplayURL()
               ? scriptSource.get()->displayURL()
               : nullptr;
  }
  ScriptSource* maybeScriptSource() const override {
    return scriptSource.get();
  }
  bool getFuncName(NameContext ctx, uint32_t funcIndex,
                   UTF8Bytes* name) const override {
    const char* p = asmJSFuncNames[funcIndex].get();
    if (!p) {
      return true;
    }
    return name->append(p, strlen(p));
  }

  AsmJSMetadataCacheablePod& pod() { return *this; }
  const AsmJSMetadataCacheablePod& pod() const { return *this; }
>>>>>>> upstream-releases
};

typedef RefPtr<AsmJSMetadata> MutableAsmJSMetadata;

/*****************************************************************************/
// ParseNode utilities

static inline ParseNode* NextNode(ParseNode* pn) { return pn->pn_next; }

static inline ParseNode* UnaryKid(ParseNode* pn) {
  return pn->as<UnaryNode>().kid();
}

static inline ParseNode* BinaryRight(ParseNode* pn) {
  return pn->as<BinaryNode>().right();
}

static inline ParseNode* BinaryLeft(ParseNode* pn) {
  return pn->as<BinaryNode>().left();
}

<<<<<<< HEAD
static inline ParseNode* ReturnExpr(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::Return));
  return UnaryKid(pn);
||||||| merged common ancestors
static inline ParseNode*
ReturnExpr(ParseNode* pn)
{
    MOZ_ASSERT(pn->isKind(ParseNodeKind::Return));
    return UnaryKid(pn);
=======
static inline ParseNode* ReturnExpr(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::ReturnStmt));
  return UnaryKid(pn);
>>>>>>> upstream-releases
}

static inline ParseNode* TernaryKid1(ParseNode* pn) {
  return pn->as<TernaryNode>().kid1();
}

static inline ParseNode* TernaryKid2(ParseNode* pn) {
  return pn->as<TernaryNode>().kid2();
}

static inline ParseNode* TernaryKid3(ParseNode* pn) {
  return pn->as<TernaryNode>().kid3();
}

static inline ParseNode* ListHead(ParseNode* pn) {
  return pn->as<ListNode>().head();
}

static inline unsigned ListLength(ParseNode* pn) {
  return pn->as<ListNode>().count();
}

<<<<<<< HEAD
static inline ParseNode* CallCallee(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::Call));
  return BinaryLeft(pn);
||||||| merged common ancestors
static inline ParseNode*
CallCallee(ParseNode* pn)
{
    MOZ_ASSERT(pn->isKind(ParseNodeKind::Call));
    return BinaryLeft(pn);
=======
static inline ParseNode* CallCallee(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::CallExpr));
  return BinaryLeft(pn);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline unsigned CallArgListLength(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::Call));
  return ListLength(BinaryRight(pn));
||||||| merged common ancestors
static inline unsigned
CallArgListLength(ParseNode* pn)
{
    MOZ_ASSERT(pn->isKind(ParseNodeKind::Call));
    return ListLength(BinaryRight(pn));
=======
static inline unsigned CallArgListLength(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::CallExpr));
  return ListLength(BinaryRight(pn));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline ParseNode* CallArgList(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::Call));
  return ListHead(BinaryRight(pn));
||||||| merged common ancestors
static inline ParseNode*
CallArgList(ParseNode* pn)
{
    MOZ_ASSERT(pn->isKind(ParseNodeKind::Call));
    return ListHead(BinaryRight(pn));
=======
static inline ParseNode* CallArgList(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::CallExpr));
  return ListHead(BinaryRight(pn));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline ParseNode* VarListHead(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::Var) ||
             pn->isKind(ParseNodeKind::Const));
  return ListHead(pn);
||||||| merged common ancestors
static inline ParseNode*
VarListHead(ParseNode* pn)
{
    MOZ_ASSERT(pn->isKind(ParseNodeKind::Var) || pn->isKind(ParseNodeKind::Const));
    return ListHead(pn);
=======
static inline ParseNode* VarListHead(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::VarStmt) ||
             pn->isKind(ParseNodeKind::ConstDecl));
  return ListHead(pn);
>>>>>>> upstream-releases
}

static inline bool IsDefaultCase(ParseNode* pn) {
  return pn->as<CaseClause>().isDefault();
}

static inline ParseNode* CaseExpr(ParseNode* pn) {
  return pn->as<CaseClause>().caseExpression();
}

static inline ParseNode* CaseBody(ParseNode* pn) {
  return pn->as<CaseClause>().statementList();
}

static inline ParseNode* BinaryOpLeft(ParseNode* pn) {
  MOZ_ASSERT(pn->isBinaryOperation());
  MOZ_ASSERT(pn->as<ListNode>().count() == 2);
  return ListHead(pn);
}

static inline ParseNode* BinaryOpRight(ParseNode* pn) {
  MOZ_ASSERT(pn->isBinaryOperation());
  MOZ_ASSERT(pn->as<ListNode>().count() == 2);
  return NextNode(ListHead(pn));
}

static inline ParseNode* BitwiseLeft(ParseNode* pn) { return BinaryOpLeft(pn); }

static inline ParseNode* BitwiseRight(ParseNode* pn) {
  return BinaryOpRight(pn);
}

<<<<<<< HEAD
static inline ParseNode* MultiplyLeft(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::Star));
  return BinaryOpLeft(pn);
||||||| merged common ancestors
static inline ParseNode*
MultiplyLeft(ParseNode* pn)
{
    MOZ_ASSERT(pn->isKind(ParseNodeKind::Star));
    return BinaryOpLeft(pn);
=======
static inline ParseNode* MultiplyLeft(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::MulExpr));
  return BinaryOpLeft(pn);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline ParseNode* MultiplyRight(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::Star));
  return BinaryOpRight(pn);
||||||| merged common ancestors
static inline ParseNode*
MultiplyRight(ParseNode* pn)
{
    MOZ_ASSERT(pn->isKind(ParseNodeKind::Star));
    return BinaryOpRight(pn);
=======
static inline ParseNode* MultiplyRight(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::MulExpr));
  return BinaryOpRight(pn);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline ParseNode* AddSubLeft(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::Add) || pn->isKind(ParseNodeKind::Sub));
  return BinaryOpLeft(pn);
||||||| merged common ancestors
static inline ParseNode*
AddSubLeft(ParseNode* pn)
{
    MOZ_ASSERT(pn->isKind(ParseNodeKind::Add) || pn->isKind(ParseNodeKind::Sub));
    return BinaryOpLeft(pn);
=======
static inline ParseNode* AddSubLeft(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::AddExpr) ||
             pn->isKind(ParseNodeKind::SubExpr));
  return BinaryOpLeft(pn);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline ParseNode* AddSubRight(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::Add) || pn->isKind(ParseNodeKind::Sub));
  return BinaryOpRight(pn);
||||||| merged common ancestors
static inline ParseNode*
AddSubRight(ParseNode* pn)
{
    MOZ_ASSERT(pn->isKind(ParseNodeKind::Add) || pn->isKind(ParseNodeKind::Sub));
    return BinaryOpRight(pn);
=======
static inline ParseNode* AddSubRight(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::AddExpr) ||
             pn->isKind(ParseNodeKind::SubExpr));
  return BinaryOpRight(pn);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline ParseNode* DivOrModLeft(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::Div) || pn->isKind(ParseNodeKind::Mod));
  return BinaryOpLeft(pn);
||||||| merged common ancestors
static inline ParseNode*
DivOrModLeft(ParseNode* pn)
{
    MOZ_ASSERT(pn->isKind(ParseNodeKind::Div) || pn->isKind(ParseNodeKind::Mod));
    return BinaryOpLeft(pn);
=======
static inline ParseNode* DivOrModLeft(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::DivExpr) ||
             pn->isKind(ParseNodeKind::ModExpr));
  return BinaryOpLeft(pn);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline ParseNode* DivOrModRight(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::Div) || pn->isKind(ParseNodeKind::Mod));
  return BinaryOpRight(pn);
||||||| merged common ancestors
static inline ParseNode*
DivOrModRight(ParseNode* pn)
{
    MOZ_ASSERT(pn->isKind(ParseNodeKind::Div) || pn->isKind(ParseNodeKind::Mod));
    return BinaryOpRight(pn);
=======
static inline ParseNode* DivOrModRight(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::DivExpr) ||
             pn->isKind(ParseNodeKind::ModExpr));
  return BinaryOpRight(pn);
>>>>>>> upstream-releases
}

static inline ParseNode* ComparisonLeft(ParseNode* pn) {
  return BinaryOpLeft(pn);
}

static inline ParseNode* ComparisonRight(ParseNode* pn) {
  return BinaryOpRight(pn);
}

<<<<<<< HEAD
static inline bool IsExpressionStatement(ParseNode* pn) {
  return pn->isKind(ParseNodeKind::ExpressionStatement);
||||||| merged common ancestors
static inline bool
IsExpressionStatement(ParseNode* pn)
{
    return pn->isKind(ParseNodeKind::ExpressionStatement);
=======
static inline bool IsExpressionStatement(ParseNode* pn) {
  return pn->isKind(ParseNodeKind::ExpressionStmt);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline ParseNode* ExpressionStatementExpr(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::ExpressionStatement));
  return UnaryKid(pn);
||||||| merged common ancestors
static inline ParseNode*
ExpressionStatementExpr(ParseNode* pn)
{
    MOZ_ASSERT(pn->isKind(ParseNodeKind::ExpressionStatement));
    return UnaryKid(pn);
=======
static inline ParseNode* ExpressionStatementExpr(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::ExpressionStmt));
  return UnaryKid(pn);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline PropertyName* LoopControlMaybeLabel(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::Break) ||
             pn->isKind(ParseNodeKind::Continue));
  return pn->as<LoopControlStatement>().label();
||||||| merged common ancestors
static inline PropertyName*
LoopControlMaybeLabel(ParseNode* pn)
{
    MOZ_ASSERT(pn->isKind(ParseNodeKind::Break) || pn->isKind(ParseNodeKind::Continue));
    return pn->as<LoopControlStatement>().label();
=======
static inline PropertyName* LoopControlMaybeLabel(ParseNode* pn) {
  MOZ_ASSERT(pn->isKind(ParseNodeKind::BreakStmt) ||
             pn->isKind(ParseNodeKind::ContinueStmt));
  return pn->as<LoopControlStatement>().label();
>>>>>>> upstream-releases
}

static inline PropertyName* LabeledStatementLabel(ParseNode* pn) {
  return pn->as<LabeledStatement>().label();
}

static inline ParseNode* LabeledStatementStatement(ParseNode* pn) {
  return pn->as<LabeledStatement>().statement();
}

static double NumberNodeValue(ParseNode* pn) {
  return pn->as<NumericLiteral>().value();
}

static bool NumberNodeHasFrac(ParseNode* pn) {
  return pn->as<NumericLiteral>().decimalPoint() == HasDecimal;
}

static ParseNode* DotBase(ParseNode* pn) {
  return &pn->as<PropertyAccess>().expression();
}

static PropertyName* DotMember(ParseNode* pn) {
  return &pn->as<PropertyAccess>().name();
}

static ParseNode* ElemBase(ParseNode* pn) {
  return &pn->as<PropertyByValue>().expression();
}

static ParseNode* ElemIndex(ParseNode* pn) {
  return &pn->as<PropertyByValue>().key();
}

<<<<<<< HEAD
static inline JSFunction* FunctionObject(CodeNode* funNode) {
  MOZ_ASSERT(funNode->isKind(ParseNodeKind::Function));
  return funNode->funbox()->function();
||||||| merged common ancestors
static inline JSFunction*
FunctionObject(CodeNode* funNode)
{
    MOZ_ASSERT(funNode->isKind(ParseNodeKind::Function));
    return funNode->funbox()->function();
=======
static inline PropertyName* FunctionName(FunctionNode* funNode) {
  if (JSAtom* name = funNode->funbox()->explicitName()) {
    return name->asPropertyName();
  }
  return nullptr;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline PropertyName* FunctionName(CodeNode* funNode) {
  if (JSAtom* name = FunctionObject(funNode)->explicitName()) {
    return name->asPropertyName();
  }
  return nullptr;
||||||| merged common ancestors
static inline PropertyName*
FunctionName(CodeNode* funNode)
{
    if (JSAtom* name = FunctionObject(funNode)->explicitName()) {
        return name->asPropertyName();
    }
    return nullptr;
=======
static inline ParseNode* FunctionStatementList(FunctionNode* funNode) {
  MOZ_ASSERT(funNode->body()->isKind(ParseNodeKind::ParamsBody));
  LexicalScopeNode* last =
      &funNode->body()->as<ListNode>().last()->as<LexicalScopeNode>();
  MOZ_ASSERT(last->isEmptyScope());
  ParseNode* body = last->scopeBody();
  MOZ_ASSERT(body->isKind(ParseNodeKind::StatementList));
  return body;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline ParseNode* FunctionStatementList(CodeNode* funNode) {
  MOZ_ASSERT(funNode->body()->isKind(ParseNodeKind::ParamsBody));
  LexicalScopeNode* last =
      &funNode->body()->as<ListNode>().last()->as<LexicalScopeNode>();
  MOZ_ASSERT(last->isEmptyScope());
  ParseNode* body = last->scopeBody();
  MOZ_ASSERT(body->isKind(ParseNodeKind::StatementList));
  return body;
||||||| merged common ancestors
static inline ParseNode*
FunctionStatementList(CodeNode* funNode)
{
    MOZ_ASSERT(funNode->body()->isKind(ParseNodeKind::ParamsBody));
    LexicalScopeNode* last = &funNode->body()->as<ListNode>().last()->as<LexicalScopeNode>();
    MOZ_ASSERT(last->isEmptyScope());
    ParseNode* body = last->scopeBody();
    MOZ_ASSERT(body->isKind(ParseNodeKind::StatementList));
    return body;
=======
static inline bool IsNormalObjectField(ParseNode* pn) {
  return pn->isKind(ParseNodeKind::PropertyDefinition) &&
         pn->as<PropertyDefinition>().accessorType() == AccessorType::None &&
         BinaryLeft(pn)->isKind(ParseNodeKind::ObjectPropertyName);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline bool IsNormalObjectField(ParseNode* pn) {
  return pn->isKind(ParseNodeKind::Colon) && pn->getOp() == JSOP_INITPROP &&
         BinaryLeft(pn)->isKind(ParseNodeKind::ObjectPropertyName);
||||||| merged common ancestors
static inline bool
IsNormalObjectField(ParseNode* pn)
{
    return pn->isKind(ParseNodeKind::Colon) &&
           pn->getOp() == JSOP_INITPROP &&
           BinaryLeft(pn)->isKind(ParseNodeKind::ObjectPropertyName);
=======
static inline PropertyName* ObjectNormalFieldName(ParseNode* pn) {
  MOZ_ASSERT(IsNormalObjectField(pn));
  MOZ_ASSERT(BinaryLeft(pn)->isKind(ParseNodeKind::ObjectPropertyName));
  return BinaryLeft(pn)->as<NameNode>().atom()->asPropertyName();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline PropertyName* ObjectNormalFieldName(ParseNode* pn) {
  MOZ_ASSERT(IsNormalObjectField(pn));
  MOZ_ASSERT(BinaryLeft(pn)->isKind(ParseNodeKind::ObjectPropertyName));
  return BinaryLeft(pn)->as<NameNode>().atom()->asPropertyName();
||||||| merged common ancestors
static inline PropertyName*
ObjectNormalFieldName(ParseNode* pn)
{
    MOZ_ASSERT(IsNormalObjectField(pn));
    MOZ_ASSERT(BinaryLeft(pn)->isKind(ParseNodeKind::ObjectPropertyName));
    return BinaryLeft(pn)->as<NameNode>().atom()->asPropertyName();
=======
static inline ParseNode* ObjectNormalFieldInitializer(ParseNode* pn) {
  MOZ_ASSERT(IsNormalObjectField(pn));
  return BinaryRight(pn);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline ParseNode* ObjectNormalFieldInitializer(ParseNode* pn) {
  MOZ_ASSERT(IsNormalObjectField(pn));
  return BinaryRight(pn);
||||||| merged common ancestors
static inline ParseNode*
ObjectNormalFieldInitializer(ParseNode* pn)
{
    MOZ_ASSERT(IsNormalObjectField(pn));
    return BinaryRight(pn);
=======
static inline bool IsUseOfName(ParseNode* pn, PropertyName* name) {
  return pn->isName(name);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline ParseNode* MaybeInitializer(ParseNode* pn) {
  return pn->as<NameNode>().initializer();
||||||| merged common ancestors
static inline ParseNode*
MaybeInitializer(ParseNode* pn)
{
    return pn->as<NameNode>().initializer();
=======
static inline bool IsIgnoredDirectiveName(JSContext* cx, JSAtom* atom) {
  return atom != cx->names().useStrict;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline bool IsUseOfName(ParseNode* pn, PropertyName* name) {
  return pn->isName(name);
||||||| merged common ancestors
static inline bool
IsUseOfName(ParseNode* pn, PropertyName* name)
{
    return pn->isName(name);
=======
static inline bool IsIgnoredDirective(JSContext* cx, ParseNode* pn) {
  return pn->isKind(ParseNodeKind::ExpressionStmt) &&
         UnaryKid(pn)->isKind(ParseNodeKind::StringExpr) &&
         IsIgnoredDirectiveName(cx, UnaryKid(pn)->as<NameNode>().atom());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline bool IsIgnoredDirectiveName(JSContext* cx, JSAtom* atom) {
  return atom != cx->names().useStrict;
||||||| merged common ancestors
static inline bool
IsIgnoredDirectiveName(JSContext* cx, JSAtom* atom)
{
    return atom != cx->names().useStrict;
=======
static inline bool IsEmptyStatement(ParseNode* pn) {
  return pn->isKind(ParseNodeKind::EmptyStmt);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline bool IsIgnoredDirective(JSContext* cx, ParseNode* pn) {
  return pn->isKind(ParseNodeKind::ExpressionStatement) &&
         UnaryKid(pn)->isKind(ParseNodeKind::String) &&
         IsIgnoredDirectiveName(cx, UnaryKid(pn)->as<NameNode>().atom());
||||||| merged common ancestors
static inline bool
IsIgnoredDirective(JSContext* cx, ParseNode* pn)
{
    return pn->isKind(ParseNodeKind::ExpressionStatement) &&
           UnaryKid(pn)->isKind(ParseNodeKind::String) &&
           IsIgnoredDirectiveName(cx, UnaryKid(pn)->as<NameNode>().atom());
=======
static inline ParseNode* SkipEmptyStatements(ParseNode* pn) {
  while (pn && IsEmptyStatement(pn)) {
    pn = pn->pn_next;
  }
  return pn;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline bool IsEmptyStatement(ParseNode* pn) {
  return pn->isKind(ParseNodeKind::EmptyStatement);
||||||| merged common ancestors
static inline bool
IsEmptyStatement(ParseNode* pn)
{
    return pn->isKind(ParseNodeKind::EmptyStatement);
=======
static inline ParseNode* NextNonEmptyStatement(ParseNode* pn) {
  return SkipEmptyStatements(pn->pn_next);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline ParseNode* SkipEmptyStatements(ParseNode* pn) {
  while (pn && IsEmptyStatement(pn)) {
    pn = pn->pn_next;
  }
  return pn;
}

static inline ParseNode* NextNonEmptyStatement(ParseNode* pn) {
  return SkipEmptyStatements(pn->pn_next);
}

static bool GetToken(AsmJSParser& parser, TokenKind* tkp) {
  auto& ts = parser.tokenStream;
  TokenKind tk;
  while (true) {
    if (!ts.getToken(&tk, TokenStreamShared::Operand)) {
      return false;
||||||| merged common ancestors
static inline ParseNode*
SkipEmptyStatements(ParseNode* pn)
{
    while (pn && IsEmptyStatement(pn)) {
        pn = pn->pn_next;
    }
    return pn;
}

static inline ParseNode*
NextNonEmptyStatement(ParseNode* pn)
{
    return SkipEmptyStatements(pn->pn_next);
}

static bool
GetToken(AsmJSParser& parser, TokenKind* tkp)
{
    auto& ts = parser.tokenStream;
    TokenKind tk;
    while (true) {
        if (!ts.getToken(&tk, TokenStreamShared::Operand)) {
            return false;
        }
        if (tk != TokenKind::Semi) {
            break;
        }
=======
template <typename Unit>
static bool GetToken(AsmJSParser<Unit>& parser, TokenKind* tkp) {
  auto& ts = parser.tokenStream;
  TokenKind tk;
  while (true) {
    if (!ts.getToken(&tk, TokenStreamShared::SlashIsRegExp)) {
      return false;
    }
    if (tk != TokenKind::Semi) {
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (tk != TokenKind::Semi) {
      break;
||||||| merged common ancestors
    *tkp = tk;
    return true;
}

static bool
PeekToken(AsmJSParser& parser, TokenKind* tkp)
{
    auto& ts = parser.tokenStream;
    TokenKind tk;
    while (true) {
        if (!ts.peekToken(&tk, TokenStream::Operand)) {
            return false;
        }
        if (tk != TokenKind::Semi) {
            break;
        }
        ts.consumeKnownToken(TokenKind::Semi, TokenStreamShared::Operand);
=======
  }
  *tkp = tk;
  return true;
}

template <typename Unit>
static bool PeekToken(AsmJSParser<Unit>& parser, TokenKind* tkp) {
  auto& ts = parser.tokenStream;
  TokenKind tk;
  while (true) {
    if (!ts.peekToken(&tk, TokenStream::SlashIsRegExp)) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
  *tkp = tk;
  return true;
||||||| merged common ancestors
    *tkp = tk;
    return true;
=======
    if (tk != TokenKind::Semi) {
      break;
    }
    ts.consumeKnownToken(TokenKind::Semi, TokenStreamShared::SlashIsRegExp);
  }
  *tkp = tk;
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool PeekToken(AsmJSParser& parser, TokenKind* tkp) {
  auto& ts = parser.tokenStream;
  TokenKind tk;
  while (true) {
    if (!ts.peekToken(&tk, TokenStream::Operand)) {
      return false;
    }
    if (tk != TokenKind::Semi) {
      break;
    }
    ts.consumeKnownToken(TokenKind::Semi, TokenStreamShared::Operand);
  }
  *tkp = tk;
  return true;
}
||||||| merged common ancestors
static bool
ParseVarOrConstStatement(AsmJSParser& parser, ParseNode** var)
{
    TokenKind tk;
    if (!PeekToken(parser, &tk)) {
        return false;
    }
    if (tk != TokenKind::Var && tk != TokenKind::Const) {
        *var = nullptr;
        return true;
    }

    *var = parser.statementListItem(YieldIsName);
    if (!*var) {
        return false;
    }
=======
template <typename Unit>
static bool ParseVarOrConstStatement(AsmJSParser<Unit>& parser,
                                     ParseNode** var) {
  TokenKind tk;
  if (!PeekToken(parser, &tk)) {
    return false;
  }
  if (tk != TokenKind::Var && tk != TokenKind::Const) {
    *var = nullptr;
    return true;
  }

  *var = parser.statementListItem(YieldIsName);
  if (!*var) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool ParseVarOrConstStatement(AsmJSParser& parser, ParseNode** var) {
  TokenKind tk;
  if (!PeekToken(parser, &tk)) {
    return false;
  }
  if (tk != TokenKind::Var && tk != TokenKind::Const) {
    *var = nullptr;
    return true;
  }

  *var = parser.statementListItem(YieldIsName);
  if (!*var) {
    return false;
  }

  MOZ_ASSERT((*var)->isKind(ParseNodeKind::Var) ||
             (*var)->isKind(ParseNodeKind::Const));
  return true;
||||||| merged common ancestors
    MOZ_ASSERT((*var)->isKind(ParseNodeKind::Var) || (*var)->isKind(ParseNodeKind::Const));
    return true;
=======
  MOZ_ASSERT((*var)->isKind(ParseNodeKind::VarStmt) ||
             (*var)->isKind(ParseNodeKind::ConstDecl));
  return true;
>>>>>>> upstream-releases
}

/*****************************************************************************/

// Represents the type and value of an asm.js numeric literal.
//
// A literal is a double iff the literal contains a decimal point (even if the
// fractional part is 0). Otherwise, integers may be classified:
//  fixnum: [0, 2^31)
//  negative int: [-2^31, 0)
//  big unsigned: [2^31, 2^32)
//  out of range: otherwise
// Lastly, a literal may be a float literal which is any double or integer
// literal coerced with Math.fround.
<<<<<<< HEAD
class NumLit {
 public:
  enum Which {
    Fixnum,
    NegativeInt,
    BigUnsigned,
    Double,
    Float,
    OutOfRangeInt = -1
  };

 private:
  Which which_;
  JS::Value value_;

 public:
  NumLit() = default;

  NumLit(Which w, const Value& v) : which_(w), value_(v) {}

  Which which() const { return which_; }

  int32_t toInt32() const {
    MOZ_ASSERT(which_ == Fixnum || which_ == NegativeInt ||
               which_ == BigUnsigned);
    return value_.toInt32();
  }

  uint32_t toUint32() const { return (uint32_t)toInt32(); }

  double toDouble() const {
    MOZ_ASSERT(which_ == Double);
    return value_.toDouble();
  }

  float toFloat() const {
    MOZ_ASSERT(which_ == Float);
    return float(value_.toDouble());
  }

  Value scalarValue() const {
    MOZ_ASSERT(which_ != OutOfRangeInt);
    return value_;
  }

  bool valid() const { return which_ != OutOfRangeInt; }

  bool isZeroBits() const {
    MOZ_ASSERT(valid());
    switch (which()) {
      case NumLit::Fixnum:
      case NumLit::NegativeInt:
      case NumLit::BigUnsigned:
        return toInt32() == 0;
      case NumLit::Double:
        return IsPositiveZero(toDouble());
      case NumLit::Float:
        return IsPositiveZero(toFloat());
      case NumLit::OutOfRangeInt:
        MOZ_CRASH("can't be here because of valid() check above");
||||||| merged common ancestors
class NumLit
{
  public:
    enum Which {
        Fixnum,
        NegativeInt,
        BigUnsigned,
        Double,
        Float,
        OutOfRangeInt = -1
    };

  private:
    Which which_;
    JS::Value value_;

  public:
    NumLit() = default;

    NumLit(Which w, const Value& v) : which_(w), value_(v) {}

    Which which() const {
        return which_;
    }

    int32_t toInt32() const {
        MOZ_ASSERT(which_ == Fixnum || which_ == NegativeInt || which_ == BigUnsigned);
        return value_.toInt32();
    }

    uint32_t toUint32() const {
        return (uint32_t)toInt32();
    }

    double toDouble() const {
        MOZ_ASSERT(which_ == Double);
        return value_.toDouble();
    }

    float toFloat() const {
        MOZ_ASSERT(which_ == Float);
        return float(value_.toDouble());
    }

    Value scalarValue() const {
        MOZ_ASSERT(which_ != OutOfRangeInt);
        return value_;
    }

    bool valid() const {
        return which_ != OutOfRangeInt;
    }

    bool isZeroBits() const {
        MOZ_ASSERT(valid());
        switch (which()) {
          case NumLit::Fixnum:
          case NumLit::NegativeInt:
          case NumLit::BigUnsigned:
            return toInt32() == 0;
          case NumLit::Double:
            return IsPositiveZero(toDouble());
          case NumLit::Float:
            return IsPositiveZero(toFloat());
          case NumLit::OutOfRangeInt:
            MOZ_CRASH("can't be here because of valid() check above");
        }
        return false;
=======
class NumLit {
 public:
  enum Which {
    Fixnum,
    NegativeInt,
    BigUnsigned,
    Double,
    Float,
    OutOfRangeInt = -1
  };

 private:
  Which which_;
  JS::Value value_;

 public:
  NumLit() = default;

  NumLit(Which w, const Value& v) : which_(w), value_(v) {}

  Which which() const { return which_; }

  int32_t toInt32() const {
    MOZ_ASSERT(which_ == Fixnum || which_ == NegativeInt ||
               which_ == BigUnsigned);
    return value_.toInt32();
  }

  uint32_t toUint32() const { return (uint32_t)toInt32(); }

  double toDouble() const {
    MOZ_ASSERT(which_ == Double);
    return value_.toDouble();
  }

  float toFloat() const {
    MOZ_ASSERT(which_ == Float);
    return float(value_.toDouble());
  }

  Value scalarValue() const {
    MOZ_ASSERT(which_ != OutOfRangeInt);
    return value_;
  }

  bool valid() const { return which_ != OutOfRangeInt; }

  bool isZeroBits() const {
    MOZ_ASSERT(valid());
    switch (which()) {
      case NumLit::Fixnum:
      case NumLit::NegativeInt:
      case NumLit::BigUnsigned:
        return toInt32() == 0;
      case NumLit::Double:
        return IsPositiveZero(toDouble());
      case NumLit::Float:
        return IsPositiveZero(toFloat());
      case NumLit::OutOfRangeInt:
        MOZ_CRASH("can't be here because of valid() check above");
    }
    return false;
  }

  LitValPOD value() const {
    switch (which_) {
      case NumLit::Fixnum:
      case NumLit::NegativeInt:
      case NumLit::BigUnsigned:
        return LitValPOD(toUint32());
      case NumLit::Float:
        return LitValPOD(toFloat());
      case NumLit::Double:
        return LitValPOD(toDouble());
      case NumLit::OutOfRangeInt:;
    }
    MOZ_CRASH("bad literal");
  }
};

// Represents the type of a general asm.js expression.
//
// A canonical subset of types representing the coercion targets: Int, Float,
// Double.
//
// Void is also part of the canonical subset which then maps to wasm::ExprType.

class Type {
 public:
  enum Which {
    Fixnum = NumLit::Fixnum,
    Signed = NumLit::NegativeInt,
    Unsigned = NumLit::BigUnsigned,
    DoubleLit = NumLit::Double,
    Float = NumLit::Float,
    Double,
    MaybeDouble,
    MaybeFloat,
    Floatish,
    Int,
    Intish,
    Void
  };

 private:
  Which which_;

 public:
  Type() = default;
  MOZ_IMPLICIT Type(Which w) : which_(w) {}

  // Map an already canonicalized Type to the return type of a function call.
  static Type ret(Type t) {
    MOZ_ASSERT(t.isCanonical());
    // The 32-bit external type is Signed, not Int.
    return t.isInt() ? Signed : t;
  }

  static Type lit(const NumLit& lit) {
    MOZ_ASSERT(lit.valid());
    Which which = Type::Which(lit.which());
    MOZ_ASSERT(which >= Fixnum && which <= Float);
    Type t;
    t.which_ = which;
    return t;
  }

  // Map |t| to one of the canonical vartype representations of a
  // wasm::ExprType.
  static Type canonicalize(Type t) {
    switch (t.which()) {
      case Fixnum:
      case Signed:
      case Unsigned:
      case Int:
        return Int;

      case Float:
        return Float;

      case DoubleLit:
      case Double:
        return Double;

      case Void:
        return Void;

      case MaybeDouble:
      case MaybeFloat:
      case Floatish:
      case Intish:
        // These types need some kind of coercion, they can't be mapped
        // to an ExprType.
        break;
    }
    MOZ_CRASH("Invalid vartype");
  }

  Which which() const { return which_; }

  bool operator==(Type rhs) const { return which_ == rhs.which_; }
  bool operator!=(Type rhs) const { return which_ != rhs.which_; }

  bool operator<=(Type rhs) const {
    switch (rhs.which_) {
      case Signed:
        return isSigned();
      case Unsigned:
        return isUnsigned();
      case DoubleLit:
        return isDoubleLit();
      case Double:
        return isDouble();
      case Float:
        return isFloat();
      case MaybeDouble:
        return isMaybeDouble();
      case MaybeFloat:
        return isMaybeFloat();
      case Floatish:
        return isFloatish();
      case Int:
        return isInt();
      case Intish:
        return isIntish();
      case Fixnum:
        return isFixnum();
      case Void:
        return isVoid();
    }
    MOZ_MAKE_COMPILER_ASSUME_IS_UNREACHABLE("unexpected rhs type");
  }

  bool isFixnum() const { return which_ == Fixnum; }

  bool isSigned() const { return which_ == Signed || which_ == Fixnum; }

  bool isUnsigned() const { return which_ == Unsigned || which_ == Fixnum; }

  bool isInt() const { return isSigned() || isUnsigned() || which_ == Int; }

  bool isIntish() const { return isInt() || which_ == Intish; }

  bool isDoubleLit() const { return which_ == DoubleLit; }

  bool isDouble() const { return isDoubleLit() || which_ == Double; }

  bool isMaybeDouble() const { return isDouble() || which_ == MaybeDouble; }

  bool isFloat() const { return which_ == Float; }

  bool isMaybeFloat() const { return isFloat() || which_ == MaybeFloat; }

  bool isFloatish() const { return isMaybeFloat() || which_ == Floatish; }

  bool isVoid() const { return which_ == Void; }

  bool isExtern() const { return isDouble() || isSigned(); }

  // Check if this is one of the valid types for a function argument.
  bool isArgType() const { return isInt() || isFloat() || isDouble(); }

  // Check if this is one of the valid types for a function return value.
  bool isReturnType() const {
    return isSigned() || isFloat() || isDouble() || isVoid();
  }

  // Check if this is one of the valid types for a global variable.
  bool isGlobalVarType() const { return isArgType(); }

  // Check if this is one of the canonical vartype representations of a
  // wasm::ExprType. See Type::canonicalize().
  bool isCanonical() const {
    switch (which()) {
      case Int:
      case Float:
      case Double:
      case Void:
        return true;
      default:
        return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    return false;
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  LitValPOD value() const {
    switch (which_) {
      case NumLit::Fixnum:
      case NumLit::NegativeInt:
      case NumLit::BigUnsigned:
        return LitValPOD(toUint32());
      case NumLit::Float:
        return LitValPOD(toFloat());
      case NumLit::Double:
        return LitValPOD(toDouble());
      case NumLit::OutOfRangeInt:;
    }
    MOZ_CRASH("bad literal");
  }
||||||| merged common ancestors
    LitValPOD value() const {
        switch (which_) {
          case NumLit::Fixnum:
          case NumLit::NegativeInt:
          case NumLit::BigUnsigned:
            return LitValPOD(toUint32());
          case NumLit::Float:
            return LitValPOD(toFloat());
          case NumLit::Double:
            return LitValPOD(toDouble());
          case NumLit::OutOfRangeInt:;
        }
        MOZ_CRASH("bad literal");
    }
=======
  // Check if this is a canonical representation of a wasm::ValType.
  bool isCanonicalValType() const { return !isVoid() && isCanonical(); }

  // Convert this canonical type to a wasm::ExprType.
  ExprType canonicalToExprType() const {
    switch (which()) {
      case Int:
        return ExprType::I32;
      case Float:
        return ExprType::F32;
      case Double:
        return ExprType::F64;
      case Void:
        return ExprType::Void;
      default:
        MOZ_CRASH("Need canonical type");
    }
  }

  // Convert this canonical type to a wasm::ValType.
  ValType canonicalToValType() const {
    return NonVoidToValType(canonicalToExprType());
  }

  // Convert this type to a wasm::ExprType for use in a wasm
  // block signature. This works for all types, including non-canonical
  // ones. Consequently, the type isn't valid for subsequent asm.js
  // validation; it's only valid for use in producing wasm.
  ExprType toWasmBlockSignatureType() const {
    switch (which()) {
      case Fixnum:
      case Signed:
      case Unsigned:
      case Int:
      case Intish:
        return ExprType::I32;

      case Float:
      case MaybeFloat:
      case Floatish:
        return ExprType::F32;

      case DoubleLit:
      case Double:
      case MaybeDouble:
        return ExprType::F64;

      case Void:
        return ExprType::Void;
    }
    MOZ_CRASH("Invalid Type");
  }

  const char* toChars() const {
    switch (which_) {
      case Double:
        return "double";
      case DoubleLit:
        return "doublelit";
      case MaybeDouble:
        return "double?";
      case Float:
        return "float";
      case Floatish:
        return "floatish";
      case MaybeFloat:
        return "float?";
      case Fixnum:
        return "fixnum";
      case Int:
        return "int";
      case Signed:
        return "signed";
      case Unsigned:
        return "unsigned";
      case Intish:
        return "intish";
      case Void:
        return "void";
    }
    MOZ_CRASH("Invalid Type");
  }
>>>>>>> upstream-releases
};

static const unsigned VALIDATION_LIFO_DEFAULT_CHUNK_SIZE = 4 * 1024;

<<<<<<< HEAD
class Type {
 public:
  enum Which {
    Fixnum = NumLit::Fixnum,
    Signed = NumLit::NegativeInt,
    Unsigned = NumLit::BigUnsigned,
    DoubleLit = NumLit::Double,
    Float = NumLit::Float,
    Double,
    MaybeDouble,
    MaybeFloat,
    Floatish,
    Int,
    Intish,
    Void
  };

 private:
  Which which_;

 public:
  Type() = default;
  MOZ_IMPLICIT Type(Which w) : which_(w) {}

  // Map an already canonicalized Type to the return type of a function call.
  static Type ret(Type t) {
    MOZ_ASSERT(t.isCanonical());
    // The 32-bit external type is Signed, not Int.
    return t.isInt() ? Signed : t;
  }

  static Type lit(const NumLit& lit) {
    MOZ_ASSERT(lit.valid());
    Which which = Type::Which(lit.which());
    MOZ_ASSERT(which >= Fixnum && which <= Float);
    Type t;
    t.which_ = which;
    return t;
  }

  // Map |t| to one of the canonical vartype representations of a
  // wasm::ExprType.
  static Type canonicalize(Type t) {
    switch (t.which()) {
      case Fixnum:
      case Signed:
      case Unsigned:
      case Int:
        return Int;

      case Float:
        return Float;

      case DoubleLit:
      case Double:
        return Double;

      case Void:
        return Void;

      case MaybeDouble:
      case MaybeFloat:
      case Floatish:
      case Intish:
        // These types need some kind of coercion, they can't be mapped
        // to an ExprType.
        break;
    }
    MOZ_CRASH("Invalid vartype");
  }
||||||| merged common ancestors
class Type
{
  public:
    enum Which {
        Fixnum = NumLit::Fixnum,
        Signed = NumLit::NegativeInt,
        Unsigned = NumLit::BigUnsigned,
        DoubleLit = NumLit::Double,
        Float = NumLit::Float,
        Double,
        MaybeDouble,
        MaybeFloat,
        Floatish,
        Int,
        Intish,
        Void
    };

  private:
    Which which_;

  public:
    Type() = default;
    MOZ_IMPLICIT Type(Which w) : which_(w) {}

    // Map an already canonicalized Type to the return type of a function call.
    static Type ret(Type t) {
        MOZ_ASSERT(t.isCanonical());
        // The 32-bit external type is Signed, not Int.
        return t.isInt() ? Signed: t;
    }

    static Type lit(const NumLit& lit) {
        MOZ_ASSERT(lit.valid());
        Which which = Type::Which(lit.which());
        MOZ_ASSERT(which >= Fixnum && which <= Float);
        Type t;
        t.which_ = which;
        return t;
    }

    // Map |t| to one of the canonical vartype representations of a
    // wasm::ExprType.
    static Type canonicalize(Type t) {
        switch(t.which()) {
          case Fixnum:
          case Signed:
          case Unsigned:
          case Int:
            return Int;

          case Float:
            return Float;

          case DoubleLit:
          case Double:
            return Double;

          case Void:
            return Void;

          case MaybeDouble:
          case MaybeFloat:
          case Floatish:
          case Intish:
            // These types need some kind of coercion, they can't be mapped
            // to an ExprType.
            break;
        }
        MOZ_CRASH("Invalid vartype");
    }
=======
class MOZ_STACK_CLASS JS_HAZ_ROOTED ModuleValidatorShared {
 public:
  class Func {
    PropertyName* name_;
    uint32_t sigIndex_;
    uint32_t firstUse_;
    uint32_t funcDefIndex_;

    bool defined_;

    // Available when defined:
    uint32_t srcBegin_;
    uint32_t srcEnd_;
    uint32_t line_;
    Bytes bytes_;
    Uint32Vector callSiteLineNums_;

   public:
    Func(PropertyName* name, uint32_t sigIndex, uint32_t firstUse,
         uint32_t funcDefIndex)
        : name_(name),
          sigIndex_(sigIndex),
          firstUse_(firstUse),
          funcDefIndex_(funcDefIndex),
          defined_(false),
          srcBegin_(0),
          srcEnd_(0),
          line_(0) {}

    PropertyName* name() const { return name_; }
    uint32_t sigIndex() const { return sigIndex_; }
    uint32_t firstUse() const { return firstUse_; }
    bool defined() const { return defined_; }
    uint32_t funcDefIndex() const { return funcDefIndex_; }

    void define(ParseNode* fn, uint32_t line, Bytes&& bytes,
                Uint32Vector&& callSiteLineNums) {
      MOZ_ASSERT(!defined_);
      defined_ = true;
      srcBegin_ = fn->pn_pos.begin;
      srcEnd_ = fn->pn_pos.end;
      line_ = line;
      bytes_ = std::move(bytes);
      callSiteLineNums_ = std::move(callSiteLineNums);
    }

    uint32_t srcBegin() const {
      MOZ_ASSERT(defined_);
      return srcBegin_;
    }
    uint32_t srcEnd() const {
      MOZ_ASSERT(defined_);
      return srcEnd_;
    }
    uint32_t line() const {
      MOZ_ASSERT(defined_);
      return line_;
    }
    const Bytes& bytes() const {
      MOZ_ASSERT(defined_);
      return bytes_;
    }
    Uint32Vector& callSiteLineNums() {
      MOZ_ASSERT(defined_);
      return callSiteLineNums_;
    }
  };

  using ConstFuncVector = Vector<const Func*>;
  using FuncVector = Vector<Func>;

  class Table {
    uint32_t sigIndex_;
    PropertyName* name_;
    uint32_t firstUse_;
    uint32_t mask_;
    bool defined_;

    Table(Table&& rhs) = delete;

   public:
    Table(uint32_t sigIndex, PropertyName* name, uint32_t firstUse,
          uint32_t mask)
        : sigIndex_(sigIndex),
          name_(name),
          firstUse_(firstUse),
          mask_(mask),
          defined_(false) {}

    uint32_t sigIndex() const { return sigIndex_; }
    PropertyName* name() const { return name_; }
    uint32_t firstUse() const { return firstUse_; }
    unsigned mask() const { return mask_; }
    bool defined() const { return defined_; }
    void define() {
      MOZ_ASSERT(!defined_);
      defined_ = true;
    }
  };

  using TableVector = Vector<Table*>;

  class Global {
   public:
    enum Which {
      Variable,
      ConstantLiteral,
      ConstantImport,
      Function,
      Table,
      FFI,
      ArrayView,
      ArrayViewCtor,
      MathBuiltinFunction
    };

   private:
    Which which_;
    union U {
      struct VarOrConst {
        Type::Which type_;
        unsigned index_;
        NumLit literalValue_;

        VarOrConst(unsigned index, const NumLit& lit)
            : type_(Type::lit(lit).which()),
              index_(index),
              literalValue_(lit)  // copies |lit|
        {}

        VarOrConst(unsigned index, Type::Which which)
            : type_(which), index_(index) {
          // The |literalValue_| field remains unused and
          // uninitialized for non-constant variables.
        }

        explicit VarOrConst(double constant)
            : type_(Type::Double),
              literalValue_(NumLit::Double, DoubleValue(constant)) {
          // The index_ field is unused and uninitialized for
          // constant doubles.
        }
      } varOrConst;
      uint32_t funcDefIndex_;
      uint32_t tableIndex_;
      uint32_t ffiIndex_;
      Scalar::Type viewType_;
      AsmJSMathBuiltinFunction mathBuiltinFunc_;

      // |varOrConst|, through |varOrConst.literalValue_|, has a
      // non-trivial constructor and therefore MUST be placement-new'd
      // into existence.
      MOZ_PUSH_DISABLE_NONTRIVIAL_UNION_WARNINGS
      U() : funcDefIndex_(0) {}
      MOZ_POP_DISABLE_NONTRIVIAL_UNION_WARNINGS
    } u;

    friend class ModuleValidatorShared;
    template <typename Unit>
    friend class ModuleValidator;
    friend class js::LifoAlloc;

    explicit Global(Which which) : which_(which) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Which which() const { return which_; }
||||||| merged common ancestors
    Which which() const { return which_; }
=======
   public:
    Which which() const { return which_; }
    Type varOrConstType() const {
      MOZ_ASSERT(which_ == Variable || which_ == ConstantLiteral ||
                 which_ == ConstantImport);
      return u.varOrConst.type_;
    }
    unsigned varOrConstIndex() const {
      MOZ_ASSERT(which_ == Variable || which_ == ConstantImport);
      return u.varOrConst.index_;
    }
    bool isConst() const {
      return which_ == ConstantLiteral || which_ == ConstantImport;
    }
    NumLit constLiteralValue() const {
      MOZ_ASSERT(which_ == ConstantLiteral);
      return u.varOrConst.literalValue_;
    }
    uint32_t funcDefIndex() const {
      MOZ_ASSERT(which_ == Function);
      return u.funcDefIndex_;
    }
    uint32_t tableIndex() const {
      MOZ_ASSERT(which_ == Table);
      return u.tableIndex_;
    }
    unsigned ffiIndex() const {
      MOZ_ASSERT(which_ == FFI);
      return u.ffiIndex_;
    }
    bool isAnyArrayView() const {
      return which_ == ArrayView || which_ == ArrayViewCtor;
    }
    Scalar::Type viewType() const {
      MOZ_ASSERT(isAnyArrayView());
      return u.viewType_;
    }
    bool isMathFunction() const { return which_ == MathBuiltinFunction; }
    AsmJSMathBuiltinFunction mathBuiltinFunction() const {
      MOZ_ASSERT(which_ == MathBuiltinFunction);
      return u.mathBuiltinFunc_;
    }
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool operator==(Type rhs) const { return which_ == rhs.which_; }
  bool operator!=(Type rhs) const { return which_ != rhs.which_; }
||||||| merged common ancestors
    bool operator==(Type rhs) const { return which_ == rhs.which_; }
    bool operator!=(Type rhs) const { return which_ != rhs.which_; }

    bool operator<=(Type rhs) const {
        switch (rhs.which_) {
          case Signed:      return isSigned();
          case Unsigned:    return isUnsigned();
          case DoubleLit:   return isDoubleLit();
          case Double:      return isDouble();
          case Float:       return isFloat();
          case MaybeDouble: return isMaybeDouble();
          case MaybeFloat:  return isMaybeFloat();
          case Floatish:    return isFloatish();
          case Int:         return isInt();
          case Intish:      return isIntish();
          case Fixnum:      return isFixnum();
          case Void:        return isVoid();
        }
        MOZ_MAKE_COMPILER_ASSUME_IS_UNREACHABLE("unexpected rhs type");
    }
=======
  struct MathBuiltin {
    enum Kind { Function, Constant };
    Kind kind;

    union {
      double cst;
      AsmJSMathBuiltinFunction func;
    } u;

    MathBuiltin() : kind(Kind(-1)), u{} {}
    explicit MathBuiltin(double cst) : kind(Constant) { u.cst = cst; }
    explicit MathBuiltin(AsmJSMathBuiltinFunction func) : kind(Function) {
      u.func = func;
    }
  };

  struct ArrayView {
    ArrayView(PropertyName* name, Scalar::Type type) : name(name), type(type) {}

    PropertyName* name;
    Scalar::Type type;
  };

 protected:
  class HashableSig {
    uint32_t sigIndex_;
    const TypeDefVector& types_;

   public:
    HashableSig(uint32_t sigIndex, const TypeDefVector& types)
        : sigIndex_(sigIndex), types_(types) {}
    uint32_t sigIndex() const { return sigIndex_; }
    const FuncType& funcType() const { return types_[sigIndex_].funcType(); }

    // Implement HashPolicy:
    using Lookup = const FuncType&;
    static HashNumber hash(Lookup l) { return l.hash(); }
    static bool match(HashableSig lhs, Lookup rhs) {
      return lhs.funcType() == rhs;
    }
  };

  class NamedSig : public HashableSig {
    PropertyName* name_;

   public:
    NamedSig(PropertyName* name, uint32_t sigIndex, const TypeDefVector& types)
        : HashableSig(sigIndex, types), name_(name) {}
    PropertyName* name() const { return name_; }

    // Implement HashPolicy:
    struct Lookup {
      PropertyName* name;
      const FuncType& funcType;
      Lookup(PropertyName* name, const FuncType& funcType)
          : name(name), funcType(funcType) {}
    };
    static HashNumber hash(Lookup l) {
      return HashGeneric(l.name, l.funcType.hash());
    }
    static bool match(NamedSig lhs, Lookup rhs) {
      return lhs.name() == rhs.name && lhs.funcType() == rhs.funcType;
    }
  };

  using SigSet = HashSet<HashableSig, HashableSig>;
  using FuncImportMap = HashMap<NamedSig, uint32_t, NamedSig>;
  using GlobalMap = HashMap<PropertyName*, Global*>;
  using MathNameMap = HashMap<PropertyName*, MathBuiltin>;
  using ArrayViewVector = Vector<ArrayView>;

 protected:
  JSContext* cx_;
  FunctionNode* moduleFunctionNode_;
  PropertyName* moduleFunctionName_;
  PropertyName* globalArgumentName_ = nullptr;
  PropertyName* importArgumentName_ = nullptr;
  PropertyName* bufferArgumentName_ = nullptr;
  MathNameMap standardLibraryMathNames_;
  RootedFunction dummyFunction_;

  // Validation-internal state:
  LifoAlloc validationLifo_;
  FuncVector funcDefs_;
  TableVector tables_;
  GlobalMap globalMap_;
  SigSet sigSet_;
  FuncImportMap funcImportMap_;
  ArrayViewVector arrayViews_;

  // State used to build the AsmJSModule in finish():
  CompilerEnvironment compilerEnv_;
  ModuleEnvironment env_;
  MutableAsmJSMetadata asmJSMetadata_;

  // Error reporting:
  UniqueChars errorString_ = nullptr;
  uint32_t errorOffset_ = UINT32_MAX;
  bool errorOverRecursed_ = false;

 protected:
  ModuleValidatorShared(JSContext* cx, FunctionNode* moduleFunctionNode)
      : cx_(cx),
        moduleFunctionNode_(moduleFunctionNode),
        moduleFunctionName_(FunctionName(moduleFunctionNode)),
        standardLibraryMathNames_(cx),
        dummyFunction_(cx),
        validationLifo_(VALIDATION_LIFO_DEFAULT_CHUNK_SIZE),
        funcDefs_(cx),
        tables_(cx),
        globalMap_(cx),
        sigSet_(cx),
        funcImportMap_(cx),
        arrayViews_(cx),
        compilerEnv_(CompileMode::Once, Tier::Optimized, OptimizedBackend::Ion,
                     DebugEnabled::False, /* gc types */ false),
        env_(/* gc types */ false, &compilerEnv_, Shareable::False,
             ModuleKind::AsmJS) {
    compilerEnv_.computeParameters(/* gc types */ false);
    env_.minMemoryLength = RoundUpToNextValidAsmJSHeapLength(0);
  }

 protected:
  MOZ_MUST_USE bool addStandardLibraryMathInfo() {
    static constexpr struct {
      const char* name;
      AsmJSMathBuiltinFunction func;
    } functions[] = {
        {"sin", AsmJSMathBuiltin_sin},       {"cos", AsmJSMathBuiltin_cos},
        {"tan", AsmJSMathBuiltin_tan},       {"asin", AsmJSMathBuiltin_asin},
        {"acos", AsmJSMathBuiltin_acos},     {"atan", AsmJSMathBuiltin_atan},
        {"ceil", AsmJSMathBuiltin_ceil},     {"floor", AsmJSMathBuiltin_floor},
        {"exp", AsmJSMathBuiltin_exp},       {"log", AsmJSMathBuiltin_log},
        {"pow", AsmJSMathBuiltin_pow},       {"sqrt", AsmJSMathBuiltin_sqrt},
        {"abs", AsmJSMathBuiltin_abs},       {"atan2", AsmJSMathBuiltin_atan2},
        {"imul", AsmJSMathBuiltin_imul},     {"clz32", AsmJSMathBuiltin_clz32},
        {"fround", AsmJSMathBuiltin_fround}, {"min", AsmJSMathBuiltin_min},
        {"max", AsmJSMathBuiltin_max},
    };

    auto AddMathFunction = [this](const char* name,
                                  AsmJSMathBuiltinFunction func) {
      JSAtom* atom = Atomize(cx_, name, strlen(name));
      if (!atom) {
        return false;
      }
      MathBuiltin builtin(func);
      return this->standardLibraryMathNames_.putNew(atom->asPropertyName(),
                                                    builtin);
    };
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool operator<=(Type rhs) const {
    switch (rhs.which_) {
      case Signed:
        return isSigned();
      case Unsigned:
        return isUnsigned();
      case DoubleLit:
        return isDoubleLit();
      case Double:
        return isDouble();
      case Float:
        return isFloat();
      case MaybeDouble:
        return isMaybeDouble();
      case MaybeFloat:
        return isMaybeFloat();
      case Floatish:
        return isFloatish();
      case Int:
        return isInt();
      case Intish:
        return isIntish();
      case Fixnum:
        return isFixnum();
      case Void:
        return isVoid();
||||||| merged common ancestors
    bool isFixnum() const {
        return which_ == Fixnum;
=======
    for (const auto& info : functions) {
      if (!AddMathFunction(info.name, info.func)) {
        return false;
      }
>>>>>>> upstream-releases
    }
    MOZ_MAKE_COMPILER_ASSUME_IS_UNREACHABLE("unexpected rhs type");
  }

<<<<<<< HEAD
  bool isFixnum() const { return which_ == Fixnum; }
||||||| merged common ancestors
    bool isSigned() const {
        return which_ == Signed || which_ == Fixnum;
    }
=======
    static constexpr struct {
      const char* name;
      double value;
    } constants[] = {
        {"E", M_E},
        {"LN10", M_LN10},
        {"LN2", M_LN2},
        {"LOG2E", M_LOG2E},
        {"LOG10E", M_LOG10E},
        {"PI", M_PI},
        {"SQRT1_2", M_SQRT1_2},
        {"SQRT2", M_SQRT2},
    };

    auto AddMathConstant = [this](const char* name, double cst) {
      JSAtom* atom = Atomize(cx_, name, strlen(name));
      if (!atom) {
        return false;
      }
      MathBuiltin builtin(cst);
      return this->standardLibraryMathNames_.putNew(atom->asPropertyName(),
                                                    builtin);
    };

    for (const auto& info : constants) {
      if (!AddMathConstant(info.name, info.value)) {
        return false;
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isSigned() const { return which_ == Signed || which_ == Fixnum; }
||||||| merged common ancestors
    bool isUnsigned() const {
        return which_ == Unsigned || which_ == Fixnum;
    }
=======
    return true;
  }

  MOZ_MUST_USE bool initDummyFunction() {
    // This flows into FunctionBox, so must be tenured.
    dummyFunction_ = NewScriptedFunction(
        cx_, 0, JSFunction::INTERPRETED, nullptr,
        /* proto = */ nullptr, gc::AllocKind::FUNCTION, TenuredObject);
    if (!dummyFunction_) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isUnsigned() const { return which_ == Unsigned || which_ == Fixnum; }
||||||| merged common ancestors
    bool isInt() const {
        return isSigned() || isUnsigned() || which_ == Int;
    }
=======
    return true;
  }

 public:
  JSContext* cx() const { return cx_; }
  PropertyName* moduleFunctionName() const { return moduleFunctionName_; }
  PropertyName* globalArgumentName() const { return globalArgumentName_; }
  PropertyName* importArgumentName() const { return importArgumentName_; }
  PropertyName* bufferArgumentName() const { return bufferArgumentName_; }
  const ModuleEnvironment& env() { return env_; }

  RootedFunction& dummyFunction() { return dummyFunction_; }
  uint32_t minMemoryLength() const { return env_.minMemoryLength; }

  void initModuleFunctionName(PropertyName* name) {
    MOZ_ASSERT(!moduleFunctionName_);
    moduleFunctionName_ = name;
  }
  MOZ_MUST_USE bool initGlobalArgumentName(PropertyName* n) {
    globalArgumentName_ = n;
    if (n) {
      MOZ_ASSERT(n->isTenured());
      asmJSMetadata_->globalArgumentName = StringToNewUTF8CharsZ(cx_, *n);
      if (!asmJSMetadata_->globalArgumentName) {
        return false;
      }
    }
    return true;
  }
  MOZ_MUST_USE bool initImportArgumentName(PropertyName* n) {
    importArgumentName_ = n;
    if (n) {
      MOZ_ASSERT(n->isTenured());
      asmJSMetadata_->importArgumentName = StringToNewUTF8CharsZ(cx_, *n);
      if (!asmJSMetadata_->importArgumentName) {
        return false;
      }
    }
    return true;
  }
  MOZ_MUST_USE bool initBufferArgumentName(PropertyName* n) {
    bufferArgumentName_ = n;
    if (n) {
      MOZ_ASSERT(n->isTenured());
      asmJSMetadata_->bufferArgumentName = StringToNewUTF8CharsZ(cx_, *n);
      if (!asmJSMetadata_->bufferArgumentName) {
        return false;
      }
    }
    return true;
  }
  bool addGlobalVarInit(PropertyName* var, const NumLit& lit, Type type,
                        bool isConst) {
    MOZ_ASSERT(type.isGlobalVarType());
    MOZ_ASSERT(type == Type::canonicalize(Type::lit(lit)));
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isInt() const { return isSigned() || isUnsigned() || which_ == Int; }
||||||| merged common ancestors
    bool isIntish() const {
        return isInt() || which_ == Intish;
    }
=======
    uint32_t index = env_.globals.length();
    if (!env_.globals.emplaceBack(type.canonicalToValType(), !isConst, index,
                                  ModuleKind::AsmJS)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isIntish() const { return isInt() || which_ == Intish; }
||||||| merged common ancestors
    bool isDoubleLit() const {
        return which_ == DoubleLit;
    }
=======
    Global::Which which = isConst ? Global::ConstantLiteral : Global::Variable;
    Global* global = validationLifo_.new_<Global>(which);
    if (!global) {
      return false;
    }
    if (isConst) {
      new (&global->u.varOrConst) Global::U::VarOrConst(index, lit);
    } else {
      new (&global->u.varOrConst) Global::U::VarOrConst(index, type.which());
    }
    if (!globalMap_.putNew(var, global)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isDoubleLit() const { return which_ == DoubleLit; }
||||||| merged common ancestors
    bool isDouble() const {
        return isDoubleLit() || which_ == Double;
    }
=======
    AsmJSGlobal g(AsmJSGlobal::Variable, nullptr);
    g.pod.u.var.initKind_ = AsmJSGlobal::InitConstant;
    g.pod.u.var.u.val_ = lit.value();
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }
  bool addGlobalVarImport(PropertyName* var, PropertyName* field, Type type,
                          bool isConst) {
    MOZ_ASSERT(type.isGlobalVarType());

    UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
    if (!fieldChars) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isDouble() const { return isDoubleLit() || which_ == Double; }
||||||| merged common ancestors
    bool isMaybeDouble() const {
        return isDouble() || which_ == MaybeDouble;
    }
=======
    uint32_t index = env_.globals.length();
    ValType valType = type.canonicalToValType();
    if (!env_.globals.emplaceBack(valType, !isConst, index,
                                  ModuleKind::AsmJS)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isMaybeDouble() const { return isDouble() || which_ == MaybeDouble; }
||||||| merged common ancestors
    bool isFloat() const {
        return which_ == Float;
    }
=======
    Global::Which which = isConst ? Global::ConstantImport : Global::Variable;
    Global* global = validationLifo_.new_<Global>(which);
    if (!global) {
      return false;
    }
    new (&global->u.varOrConst) Global::U::VarOrConst(index, type.which());
    if (!globalMap_.putNew(var, global)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isFloat() const { return which_ == Float; }
||||||| merged common ancestors
    bool isMaybeFloat() const {
        return isFloat() || which_ == MaybeFloat;
    }
=======
    AsmJSGlobal g(AsmJSGlobal::Variable, std::move(fieldChars));
    g.pod.u.var.initKind_ = AsmJSGlobal::InitImport;
    g.pod.u.var.u.importValType_ = valType.packed();
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }
  bool addArrayView(PropertyName* var, Scalar::Type vt,
                    PropertyName* maybeField) {
    UniqueChars fieldChars;
    if (maybeField) {
      fieldChars = StringToNewUTF8CharsZ(cx_, *maybeField);
      if (!fieldChars) {
        return false;
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isMaybeFloat() const { return isFloat() || which_ == MaybeFloat; }
||||||| merged common ancestors
    bool isFloatish() const {
        return isMaybeFloat() || which_ == Floatish;
    }
=======
    if (!arrayViews_.append(ArrayView(var, vt))) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isFloatish() const { return isMaybeFloat() || which_ == Floatish; }
||||||| merged common ancestors
    bool isVoid() const {
        return which_ == Void;
    }
=======
    Global* global = validationLifo_.new_<Global>(Global::ArrayView);
    if (!global) {
      return false;
    }
    new (&global->u.viewType_) Scalar::Type(vt);
    if (!globalMap_.putNew(var, global)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isVoid() const { return which_ == Void; }
||||||| merged common ancestors
    bool isExtern() const {
        return isDouble() || isSigned();
    }
=======
    AsmJSGlobal g(AsmJSGlobal::ArrayView, std::move(fieldChars));
    g.pod.u.viewType_ = vt;
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }
  bool addMathBuiltinFunction(PropertyName* var, AsmJSMathBuiltinFunction func,
                              PropertyName* field) {
    UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
    if (!fieldChars) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isExtern() const { return isDouble() || isSigned(); }
||||||| merged common ancestors
    // Check if this is one of the valid types for a function argument.
    bool isArgType() const {
        return isInt() || isFloat() || isDouble();
    }
=======
    Global* global = validationLifo_.new_<Global>(Global::MathBuiltinFunction);
    if (!global) {
      return false;
    }
    new (&global->u.mathBuiltinFunc_) AsmJSMathBuiltinFunction(func);
    if (!globalMap_.putNew(var, global)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Check if this is one of the valid types for a function argument.
  bool isArgType() const { return isInt() || isFloat() || isDouble(); }
||||||| merged common ancestors
    // Check if this is one of the valid types for a function return value.
    bool isReturnType() const {
        return isSigned() || isFloat() || isDouble() || isVoid();
    }
=======
    AsmJSGlobal g(AsmJSGlobal::MathBuiltinFunction, std::move(fieldChars));
    g.pod.u.mathBuiltinFunc_ = func;
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }

 private:
  bool addGlobalDoubleConstant(PropertyName* var, double constant) {
    Global* global = validationLifo_.new_<Global>(Global::ConstantLiteral);
    if (!global) {
      return false;
    }
    new (&global->u.varOrConst) Global::U::VarOrConst(constant);
    return globalMap_.putNew(var, global);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Check if this is one of the valid types for a function return value.
  bool isReturnType() const {
    return isSigned() || isFloat() || isDouble() || isVoid();
  }
||||||| merged common ancestors
    // Check if this is one of the valid types for a global variable.
    bool isGlobalVarType() const {
        return isArgType();
    }
=======
 public:
  bool addMathBuiltinConstant(PropertyName* var, double constant,
                              PropertyName* field) {
    UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
    if (!fieldChars) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Check if this is one of the valid types for a global variable.
  bool isGlobalVarType() const { return isArgType(); }
||||||| merged common ancestors
    // Check if this is one of the canonical vartype representations of a
    // wasm::ExprType. See Type::canonicalize().
    bool isCanonical() const {
        switch (which()) {
          case Int:
          case Float:
          case Double:
          case Void:
            return true;
          default:
            return false;
        }
    }
=======
    if (!addGlobalDoubleConstant(var, constant)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Check if this is one of the canonical vartype representations of a
  // wasm::ExprType. See Type::canonicalize().
  bool isCanonical() const {
    switch (which()) {
      case Int:
      case Float:
      case Double:
      case Void:
        return true;
      default:
        return false;
||||||| merged common ancestors
    // Check if this is a canonical representation of a wasm::ValType.
    bool isCanonicalValType() const {
        return !isVoid() && isCanonical();
=======
    AsmJSGlobal g(AsmJSGlobal::Constant, std::move(fieldChars));
    g.pod.u.constant.value_ = constant;
    g.pod.u.constant.kind_ = AsmJSGlobal::MathConstant;
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }
  bool addGlobalConstant(PropertyName* var, double constant,
                         PropertyName* field) {
    UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
    if (!fieldChars) {
      return false;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  // Check if this is a canonical representation of a wasm::ValType.
  bool isCanonicalValType() const { return !isVoid() && isCanonical(); }
||||||| merged common ancestors
    // Convert this canonical type to a wasm::ExprType.
    ExprType canonicalToExprType() const {
        switch (which()) {
          case Int:       return ExprType::I32;
          case Float:     return ExprType::F32;
          case Double:    return ExprType::F64;
          case Void:      return ExprType::Void;
          default:        MOZ_CRASH("Need canonical type");
        }
    }
=======
    if (!addGlobalDoubleConstant(var, constant)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Convert this canonical type to a wasm::ExprType.
  ExprType canonicalToExprType() const {
    switch (which()) {
      case Int:
        return ExprType::I32;
      case Float:
        return ExprType::F32;
      case Double:
        return ExprType::F64;
      case Void:
        return ExprType::Void;
      default:
        MOZ_CRASH("Need canonical type");
    }
  }

  // Convert this canonical type to a wasm::ValType.
  ValType canonicalToValType() const {
    return NonVoidToValType(canonicalToExprType());
  }

  // Convert this type to a wasm::ExprType for use in a wasm
  // block signature. This works for all types, including non-canonical
  // ones. Consequently, the type isn't valid for subsequent asm.js
  // validation; it's only valid for use in producing wasm.
  ExprType toWasmBlockSignatureType() const {
    switch (which()) {
      case Fixnum:
      case Signed:
      case Unsigned:
      case Int:
      case Intish:
        return ExprType::I32;

      case Float:
      case MaybeFloat:
      case Floatish:
        return ExprType::F32;

      case DoubleLit:
      case Double:
      case MaybeDouble:
        return ExprType::F64;

      case Void:
        return ExprType::Void;
    }
    MOZ_CRASH("Invalid Type");
  }

  const char* toChars() const {
    switch (which_) {
      case Double:
        return "double";
      case DoubleLit:
        return "doublelit";
      case MaybeDouble:
        return "double?";
      case Float:
        return "float";
      case Floatish:
        return "floatish";
      case MaybeFloat:
        return "float?";
      case Fixnum:
        return "fixnum";
      case Int:
        return "int";
      case Signed:
        return "signed";
      case Unsigned:
        return "unsigned";
      case Intish:
        return "intish";
      case Void:
        return "void";
    }
    MOZ_CRASH("Invalid Type");
  }
};
||||||| merged common ancestors
    // Convert this canonical type to a wasm::ValType.
    ValType canonicalToValType() const {
        return NonVoidToValType(canonicalToExprType());
    }

    // Convert this type to a wasm::ExprType for use in a wasm
    // block signature. This works for all types, including non-canonical
    // ones. Consequently, the type isn't valid for subsequent asm.js
    // validation; it's only valid for use in producing wasm.
    ExprType toWasmBlockSignatureType() const {
        switch (which()) {
          case Fixnum:
          case Signed:
          case Unsigned:
          case Int:
          case Intish:
            return ExprType::I32;

          case Float:
          case MaybeFloat:
          case Floatish:
            return ExprType::F32;

          case DoubleLit:
          case Double:
          case MaybeDouble:
            return ExprType::F64;

          case Void:
            return ExprType::Void;
        }
        MOZ_CRASH("Invalid Type");
    }

    const char* toChars() const {
        switch (which_) {
          case Double:      return "double";
          case DoubleLit:   return "doublelit";
          case MaybeDouble: return "double?";
          case Float:       return "float";
          case Floatish:    return "floatish";
          case MaybeFloat:  return "float?";
          case Fixnum:      return "fixnum";
          case Int:         return "int";
          case Signed:      return "signed";
          case Unsigned:    return "unsigned";
          case Intish:      return "intish";
          case Void:        return "void";
        }
        MOZ_CRASH("Invalid Type");
    }
};
=======
    AsmJSGlobal g(AsmJSGlobal::Constant, std::move(fieldChars));
    g.pod.u.constant.value_ = constant;
    g.pod.u.constant.kind_ = AsmJSGlobal::GlobalConstant;
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }
  bool addArrayViewCtor(PropertyName* var, Scalar::Type vt,
                        PropertyName* field) {
    UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
    if (!fieldChars) {
      return false;
    }
>>>>>>> upstream-releases

    Global* global = validationLifo_.new_<Global>(Global::ArrayViewCtor);
    if (!global) {
      return false;
    }
    new (&global->u.viewType_) Scalar::Type(vt);
    if (!globalMap_.putNew(var, global)) {
      return false;
    }

<<<<<<< HEAD
// The ModuleValidator encapsulates the entire validation of an asm.js module.
// Its lifetime goes from the validation of the top components of an asm.js
// module (all the globals), the emission of bytecode for all the functions in
// the module and the validation of function's pointer tables. It also finishes
// the compilation of all the module's stubs.
//
// Rooting note: ModuleValidator is a stack class that contains unrooted
// PropertyName (JSAtom) pointers.  This is safe because it cannot be
// constructed without a TokenStream reference.  TokenStream is itself a stack
// class that cannot be constructed without an AutoKeepAtoms being live on the
// stack, which prevents collection of atoms.
//
// ModuleValidator is marked as rooted in the rooting analysis.  Don't add
// non-JSAtom pointers, or this will break!
class MOZ_STACK_CLASS JS_HAZ_ROOTED ModuleValidator {
 public:
  class Func {
    PropertyName* name_;
    uint32_t sigIndex_;
    uint32_t firstUse_;
    uint32_t funcDefIndex_;

    bool defined_;

    // Available when defined:
    uint32_t srcBegin_;
    uint32_t srcEnd_;
    uint32_t line_;
    Bytes bytes_;
    Uint32Vector callSiteLineNums_;

   public:
    Func(PropertyName* name, uint32_t sigIndex, uint32_t firstUse,
         uint32_t funcDefIndex)
        : name_(name),
          sigIndex_(sigIndex),
          firstUse_(firstUse),
          funcDefIndex_(funcDefIndex),
          defined_(false),
          srcBegin_(0),
          srcEnd_(0),
          line_(0) {}

    PropertyName* name() const { return name_; }
    uint32_t sigIndex() const { return sigIndex_; }
    uint32_t firstUse() const { return firstUse_; }
    bool defined() const { return defined_; }
    uint32_t funcDefIndex() const { return funcDefIndex_; }

    void define(ParseNode* fn, uint32_t line, Bytes&& bytes,
                Uint32Vector&& callSiteLineNums) {
      MOZ_ASSERT(!defined_);
      defined_ = true;
      srcBegin_ = fn->pn_pos.begin;
      srcEnd_ = fn->pn_pos.end;
      line_ = line;
      bytes_ = std::move(bytes);
      callSiteLineNums_ = std::move(callSiteLineNums);
    }

    uint32_t srcBegin() const {
      MOZ_ASSERT(defined_);
      return srcBegin_;
    }
    uint32_t srcEnd() const {
      MOZ_ASSERT(defined_);
      return srcEnd_;
    }
    uint32_t line() const {
      MOZ_ASSERT(defined_);
      return line_;
    }
    const Bytes& bytes() const {
      MOZ_ASSERT(defined_);
      return bytes_;
    }
    Uint32Vector& callSiteLineNums() {
      MOZ_ASSERT(defined_);
      return callSiteLineNums_;
    }
  };

  typedef Vector<const Func*> ConstFuncVector;
  typedef Vector<Func> FuncVector;

  class Table {
    uint32_t sigIndex_;
    PropertyName* name_;
    uint32_t firstUse_;
    uint32_t mask_;
    bool defined_;

    Table(Table&& rhs) = delete;

   public:
    Table(uint32_t sigIndex, PropertyName* name, uint32_t firstUse,
          uint32_t mask)
        : sigIndex_(sigIndex),
          name_(name),
          firstUse_(firstUse),
          mask_(mask),
          defined_(false) {}

    uint32_t sigIndex() const { return sigIndex_; }
    PropertyName* name() const { return name_; }
    uint32_t firstUse() const { return firstUse_; }
    unsigned mask() const { return mask_; }
    bool defined() const { return defined_; }
    void define() {
      MOZ_ASSERT(!defined_);
      defined_ = true;
    }
  };

  typedef Vector<Table*> TableVector;

  class Global {
   public:
    enum Which {
      Variable,
      ConstantLiteral,
      ConstantImport,
      Function,
      Table,
      FFI,
      ArrayView,
      ArrayViewCtor,
      MathBuiltinFunction
    };

   private:
    Which which_;
    union U {
      struct VarOrConst {
        Type::Which type_;
        unsigned index_;
        NumLit literalValue_;

        VarOrConst(unsigned index, const NumLit& lit)
            : type_(Type::lit(lit).which()),
              index_(index),
              literalValue_(lit)  // copies |lit|
        {}

        VarOrConst(unsigned index, Type::Which which)
            : type_(which), index_(index) {
          // The |literalValue_| field remains unused and
          // uninitialized for non-constant variables.
        }

        explicit VarOrConst(double constant)
            : type_(Type::Double),
              literalValue_(NumLit::Double, DoubleValue(constant)) {
          // The index_ field is unused and uninitialized for
          // constant doubles.
        }
      } varOrConst;
      uint32_t funcDefIndex_;
      uint32_t tableIndex_;
      uint32_t ffiIndex_;
      Scalar::Type viewType_;
      AsmJSMathBuiltinFunction mathBuiltinFunc_;

      // |varOrConst|, through |varOrConst.literalValue_|, has a
      // non-trivial constructor and therefore MUST be placement-new'd
      // into existence.
      MOZ_PUSH_DISABLE_NONTRIVIAL_UNION_WARNINGS
      U() : funcDefIndex_(0) {}
      MOZ_POP_DISABLE_NONTRIVIAL_UNION_WARNINGS
    } u;

    friend class ModuleValidator;
    friend class js::LifoAlloc;

    explicit Global(Which which) : which_(which) {}

   public:
    Which which() const { return which_; }
    Type varOrConstType() const {
      MOZ_ASSERT(which_ == Variable || which_ == ConstantLiteral ||
                 which_ == ConstantImport);
      return u.varOrConst.type_;
    }
    unsigned varOrConstIndex() const {
      MOZ_ASSERT(which_ == Variable || which_ == ConstantImport);
      return u.varOrConst.index_;
    }
    bool isConst() const {
      return which_ == ConstantLiteral || which_ == ConstantImport;
    }
    NumLit constLiteralValue() const {
      MOZ_ASSERT(which_ == ConstantLiteral);
      return u.varOrConst.literalValue_;
    }
    uint32_t funcDefIndex() const {
      MOZ_ASSERT(which_ == Function);
      return u.funcDefIndex_;
    }
    uint32_t tableIndex() const {
      MOZ_ASSERT(which_ == Table);
      return u.tableIndex_;
    }
    unsigned ffiIndex() const {
      MOZ_ASSERT(which_ == FFI);
      return u.ffiIndex_;
    }
    bool isAnyArrayView() const {
      return which_ == ArrayView || which_ == ArrayViewCtor;
    }
    Scalar::Type viewType() const {
      MOZ_ASSERT(isAnyArrayView());
      return u.viewType_;
    }
    bool isMathFunction() const { return which_ == MathBuiltinFunction; }
    AsmJSMathBuiltinFunction mathBuiltinFunction() const {
      MOZ_ASSERT(which_ == MathBuiltinFunction);
      return u.mathBuiltinFunc_;
    }
  };

  struct MathBuiltin {
    enum Kind { Function, Constant };
    Kind kind;

    union {
      double cst;
      AsmJSMathBuiltinFunction func;
    } u;

    MathBuiltin() : kind(Kind(-1)), u{} {}
    explicit MathBuiltin(double cst) : kind(Constant) { u.cst = cst; }
    explicit MathBuiltin(AsmJSMathBuiltinFunction func) : kind(Function) {
      u.func = func;
    }
  };

  struct ArrayView {
    ArrayView(PropertyName* name, Scalar::Type type) : name(name), type(type) {}

    PropertyName* name;
    Scalar::Type type;
  };

 private:
  class HashableSig {
    uint32_t sigIndex_;
    const TypeDefVector& types_;

   public:
    HashableSig(uint32_t sigIndex, const TypeDefVector& types)
        : sigIndex_(sigIndex), types_(types) {}
    uint32_t sigIndex() const { return sigIndex_; }
    const FuncType& funcType() const { return types_[sigIndex_].funcType(); }

    // Implement HashPolicy:
    typedef const FuncType& Lookup;
    static HashNumber hash(Lookup l) { return l.hash(); }
    static bool match(HashableSig lhs, Lookup rhs) {
      return lhs.funcType() == rhs;
    }
  };

  class NamedSig : public HashableSig {
    PropertyName* name_;

   public:
    NamedSig(PropertyName* name, uint32_t sigIndex, const TypeDefVector& types)
        : HashableSig(sigIndex, types), name_(name) {}
    PropertyName* name() const { return name_; }

    // Implement HashPolicy:
    struct Lookup {
      PropertyName* name;
      const FuncType& funcType;
      Lookup(PropertyName* name, const FuncType& funcType)
          : name(name), funcType(funcType) {}
    };
    static HashNumber hash(Lookup l) {
      return HashGeneric(l.name, l.funcType.hash());
    }
    static bool match(NamedSig lhs, Lookup rhs) {
      return lhs.name() == rhs.name && lhs.funcType() == rhs.funcType;
    }
  };

  typedef HashSet<HashableSig, HashableSig> SigSet;
  typedef HashMap<NamedSig, uint32_t, NamedSig> FuncImportMap;
  typedef HashMap<PropertyName*, Global*> GlobalMap;
  typedef HashMap<PropertyName*, MathBuiltin> MathNameMap;
  typedef Vector<ArrayView> ArrayViewVector;

  JSContext* cx_;
  AsmJSParser& parser_;
  CodeNode* moduleFunctionNode_;
  PropertyName* moduleFunctionName_;
  PropertyName* globalArgumentName_;
  PropertyName* importArgumentName_;
  PropertyName* bufferArgumentName_;
  MathNameMap standardLibraryMathNames_;
  RootedFunction dummyFunction_;

  // Validation-internal state:
  LifoAlloc validationLifo_;
  FuncVector funcDefs_;
  TableVector tables_;
  GlobalMap globalMap_;
  SigSet sigSet_;
  FuncImportMap funcImportMap_;
  ArrayViewVector arrayViews_;

  // State used to build the AsmJSModule in finish():
  CompilerEnvironment compilerEnv_;
  ModuleEnvironment env_;
  MutableAsmJSMetadata asmJSMetadata_;

  // Error reporting:
  UniqueChars errorString_;
  uint32_t errorOffset_;
  bool errorOverRecursed_;

  // Helpers:
  bool addStandardLibraryMathName(const char* name,
                                  AsmJSMathBuiltinFunction func) {
    JSAtom* atom = Atomize(cx_, name, strlen(name));
    if (!atom) {
      return false;
    }
    MathBuiltin builtin(func);
    return standardLibraryMathNames_.putNew(atom->asPropertyName(), builtin);
  }
  bool addStandardLibraryMathName(const char* name, double cst) {
    JSAtom* atom = Atomize(cx_, name, strlen(name));
    if (!atom) {
      return false;
    }
    MathBuiltin builtin(cst);
    return standardLibraryMathNames_.putNew(atom->asPropertyName(), builtin);
  }
  bool newSig(FuncType&& sig, uint32_t* sigIndex) {
    if (env_.types.length() >= MaxTypes) {
      return failCurrentOffset("too many signatures");
    }

    *sigIndex = env_.types.length();
    return env_.types.append(std::move(sig));
  }
  bool declareSig(FuncType&& sig, uint32_t* sigIndex) {
    SigSet::AddPtr p = sigSet_.lookupForAdd(sig);
    if (p) {
      *sigIndex = p->sigIndex();
      MOZ_ASSERT(env_.types[*sigIndex].funcType() == sig);
      return true;
    }

    return newSig(std::move(sig), sigIndex) &&
           sigSet_.add(p, HashableSig(*sigIndex, env_.types));
  }

 public:
  ModuleValidator(JSContext* cx, AsmJSParser& parser,
                  CodeNode* moduleFunctionNode)
      : cx_(cx),
        parser_(parser),
        moduleFunctionNode_(moduleFunctionNode),
        moduleFunctionName_(FunctionName(moduleFunctionNode)),
        globalArgumentName_(nullptr),
        importArgumentName_(nullptr),
        bufferArgumentName_(nullptr),
        standardLibraryMathNames_(cx),
        dummyFunction_(cx),
        validationLifo_(VALIDATION_LIFO_DEFAULT_CHUNK_SIZE),
        funcDefs_(cx),
        tables_(cx),
        globalMap_(cx),
        sigSet_(cx),
        funcImportMap_(cx),
        arrayViews_(cx),
        compilerEnv_(CompileMode::Once, Tier::Optimized, OptimizedBackend::Ion,
                     DebugEnabled::False, HasGcTypes::False),
        env_(HasGcTypes::False, &compilerEnv_, Shareable::False,
             ModuleKind::AsmJS),
        errorString_(nullptr),
        errorOffset_(UINT32_MAX),
        errorOverRecursed_(false) {
    compilerEnv_.computeParameters(HasGcTypes::False);
    env_.minMemoryLength = RoundUpToNextValidAsmJSHeapLength(0);
  }

  ~ModuleValidator() {
    if (errorString_) {
      MOZ_ASSERT(errorOffset_ != UINT32_MAX);
      typeFailure(errorOffset_, errorString_.get());
    }
    if (errorOverRecursed_) {
      ReportOverRecursed(cx_);
    }
  }

 private:
  void typeFailure(uint32_t offset, ...) {
    va_list args;
    va_start(args, offset);

    auto& ts = tokenStream();
    ErrorMetadata metadata;
    if (ts.computeErrorMetadata(&metadata, offset)) {
      if (ts.anyCharsAccess().options().throwOnAsmJSValidationFailureOption) {
        ReportCompileError(cx_, std::move(metadata), nullptr, JSREPORT_ERROR,
                           JSMSG_USE_ASM_TYPE_FAIL, args);
      } else {
        // asm.js type failure is indicated by calling one of the fail*
        // functions below.  These functions always return false to
        // halt asm.js parsing.  Whether normal parsing is attempted as
        // fallback, depends whether an exception is also set.
        //
        // If warning succeeds, no exception is set.  If warning fails,
        // an exception is set and execution will halt.  Thus it's safe
        // and correct to ignore the return value here.
        Unused << ts.anyCharsAccess().compileWarning(
            std::move(metadata), nullptr, JSREPORT_WARNING,
            JSMSG_USE_ASM_TYPE_FAIL, args);
      }
    }

    va_end(args);
  }

 public:
  bool init() {
    asmJSMetadata_ = cx_->new_<AsmJSMetadata>();
    if (!asmJSMetadata_) {
      return false;
    }

    asmJSMetadata_->toStringStart =
        moduleFunctionNode_->funbox()->toStringStart;
    asmJSMetadata_->srcStart = moduleFunctionNode_->body()->pn_pos.begin;
    asmJSMetadata_->strict =
        parser_.pc->sc()->strict() && !parser_.pc->sc()->hasExplicitUseStrict();
    asmJSMetadata_->scriptSource.reset(parser_.ss);

    if (!addStandardLibraryMathName("sin", AsmJSMathBuiltin_sin) ||
        !addStandardLibraryMathName("cos", AsmJSMathBuiltin_cos) ||
        !addStandardLibraryMathName("tan", AsmJSMathBuiltin_tan) ||
        !addStandardLibraryMathName("asin", AsmJSMathBuiltin_asin) ||
        !addStandardLibraryMathName("acos", AsmJSMathBuiltin_acos) ||
        !addStandardLibraryMathName("atan", AsmJSMathBuiltin_atan) ||
        !addStandardLibraryMathName("ceil", AsmJSMathBuiltin_ceil) ||
        !addStandardLibraryMathName("floor", AsmJSMathBuiltin_floor) ||
        !addStandardLibraryMathName("exp", AsmJSMathBuiltin_exp) ||
        !addStandardLibraryMathName("log", AsmJSMathBuiltin_log) ||
        !addStandardLibraryMathName("pow", AsmJSMathBuiltin_pow) ||
        !addStandardLibraryMathName("sqrt", AsmJSMathBuiltin_sqrt) ||
        !addStandardLibraryMathName("abs", AsmJSMathBuiltin_abs) ||
        !addStandardLibraryMathName("atan2", AsmJSMathBuiltin_atan2) ||
        !addStandardLibraryMathName("imul", AsmJSMathBuiltin_imul) ||
        !addStandardLibraryMathName("clz32", AsmJSMathBuiltin_clz32) ||
        !addStandardLibraryMathName("fround", AsmJSMathBuiltin_fround) ||
        !addStandardLibraryMathName("min", AsmJSMathBuiltin_min) ||
        !addStandardLibraryMathName("max", AsmJSMathBuiltin_max) ||
        !addStandardLibraryMathName("E", M_E) ||
        !addStandardLibraryMathName("LN10", M_LN10) ||
        !addStandardLibraryMathName("LN2", M_LN2) ||
        !addStandardLibraryMathName("LOG2E", M_LOG2E) ||
        !addStandardLibraryMathName("LOG10E", M_LOG10E) ||
        !addStandardLibraryMathName("PI", M_PI) ||
        !addStandardLibraryMathName("SQRT1_2", M_SQRT1_2) ||
        !addStandardLibraryMathName("SQRT2", M_SQRT2)) {
      return false;
    }

    // This flows into FunctionBox, so must be tenured.
    dummyFunction_ = NewScriptedFunction(
        cx_, 0, JSFunction::INTERPRETED, nullptr,
        /* proto = */ nullptr, gc::AllocKind::FUNCTION, TenuredObject);
    if (!dummyFunction_) {
      return false;
    }

    return true;
  }

  JSContext* cx() const { return cx_; }
  PropertyName* moduleFunctionName() const { return moduleFunctionName_; }
  PropertyName* globalArgumentName() const { return globalArgumentName_; }
  PropertyName* importArgumentName() const { return importArgumentName_; }
  PropertyName* bufferArgumentName() const { return bufferArgumentName_; }
  const ModuleEnvironment& env() { return env_; }

  AsmJSParser& parser() const { return parser_; }

  auto tokenStream() const -> decltype(parser_.tokenStream)& {
    return parser_.tokenStream;
  }

  RootedFunction& dummyFunction() { return dummyFunction_; }
  uint32_t minMemoryLength() const { return env_.minMemoryLength; }

  void initModuleFunctionName(PropertyName* name) {
    MOZ_ASSERT(!moduleFunctionName_);
    moduleFunctionName_ = name;
  }
  MOZ_MUST_USE bool initGlobalArgumentName(PropertyName* n) {
    globalArgumentName_ = n;
    if (n) {
      MOZ_ASSERT(n->isTenured());
      asmJSMetadata_->globalArgumentName = StringToNewUTF8CharsZ(cx_, *n);
      if (!asmJSMetadata_->globalArgumentName) {
        return false;
      }
    }
    return true;
  }
  MOZ_MUST_USE bool initImportArgumentName(PropertyName* n) {
    importArgumentName_ = n;
    if (n) {
      MOZ_ASSERT(n->isTenured());
      asmJSMetadata_->importArgumentName = StringToNewUTF8CharsZ(cx_, *n);
      if (!asmJSMetadata_->importArgumentName) {
        return false;
      }
    }
    return true;
  }
  MOZ_MUST_USE bool initBufferArgumentName(PropertyName* n) {
    bufferArgumentName_ = n;
    if (n) {
      MOZ_ASSERT(n->isTenured());
      asmJSMetadata_->bufferArgumentName = StringToNewUTF8CharsZ(cx_, *n);
      if (!asmJSMetadata_->bufferArgumentName) {
        return false;
      }
    }
    return true;
  }
  bool addGlobalVarInit(PropertyName* var, const NumLit& lit, Type type,
                        bool isConst) {
    MOZ_ASSERT(type.isGlobalVarType());
    MOZ_ASSERT(type == Type::canonicalize(Type::lit(lit)));

    uint32_t index = env_.globals.length();
    if (!env_.globals.emplaceBack(type.canonicalToValType(), !isConst, index,
                                  ModuleKind::AsmJS)) {
      return false;
    }

    Global::Which which = isConst ? Global::ConstantLiteral : Global::Variable;
    Global* global = validationLifo_.new_<Global>(which);
    if (!global) {
      return false;
    }
    if (isConst) {
      new (&global->u.varOrConst) Global::U::VarOrConst(index, lit);
    } else {
      new (&global->u.varOrConst) Global::U::VarOrConst(index, type.which());
    }
    if (!globalMap_.putNew(var, global)) {
      return false;
    }

    AsmJSGlobal g(AsmJSGlobal::Variable, nullptr);
    g.pod.u.var.initKind_ = AsmJSGlobal::InitConstant;
    g.pod.u.var.u.val_ = lit.value();
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }
  bool addGlobalVarImport(PropertyName* var, PropertyName* field, Type type,
                          bool isConst) {
    MOZ_ASSERT(type.isGlobalVarType());

    UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
    if (!fieldChars) {
      return false;
    }

    uint32_t index = env_.globals.length();
    ValType valType = type.canonicalToValType();
    if (!env_.globals.emplaceBack(valType, !isConst, index,
                                  ModuleKind::AsmJS)) {
      return false;
    }

    Global::Which which = isConst ? Global::ConstantImport : Global::Variable;
    Global* global = validationLifo_.new_<Global>(which);
    if (!global) {
      return false;
    }
    new (&global->u.varOrConst) Global::U::VarOrConst(index, type.which());
    if (!globalMap_.putNew(var, global)) {
      return false;
    }

    AsmJSGlobal g(AsmJSGlobal::Variable, std::move(fieldChars));
    g.pod.u.var.initKind_ = AsmJSGlobal::InitImport;
    g.pod.u.var.u.importValType_ = valType.packed();
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }
  bool addArrayView(PropertyName* var, Scalar::Type vt,
                    PropertyName* maybeField) {
    UniqueChars fieldChars;
    if (maybeField) {
      fieldChars = StringToNewUTF8CharsZ(cx_, *maybeField);
      if (!fieldChars) {
        return false;
      }
    }

    if (!arrayViews_.append(ArrayView(var, vt))) {
      return false;
    }

    Global* global = validationLifo_.new_<Global>(Global::ArrayView);
    if (!global) {
      return false;
    }
    new (&global->u.viewType_) Scalar::Type(vt);
    if (!globalMap_.putNew(var, global)) {
      return false;
    }

    AsmJSGlobal g(AsmJSGlobal::ArrayView, std::move(fieldChars));
    g.pod.u.viewType_ = vt;
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }
  bool addMathBuiltinFunction(PropertyName* var, AsmJSMathBuiltinFunction func,
                              PropertyName* field) {
    UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
    if (!fieldChars) {
      return false;
    }

    Global* global = validationLifo_.new_<Global>(Global::MathBuiltinFunction);
    if (!global) {
      return false;
    }
    new (&global->u.mathBuiltinFunc_) AsmJSMathBuiltinFunction(func);
    if (!globalMap_.putNew(var, global)) {
      return false;
    }

    AsmJSGlobal g(AsmJSGlobal::MathBuiltinFunction, std::move(fieldChars));
    g.pod.u.mathBuiltinFunc_ = func;
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }

 private:
  bool addGlobalDoubleConstant(PropertyName* var, double constant) {
    Global* global = validationLifo_.new_<Global>(Global::ConstantLiteral);
    if (!global) {
      return false;
    }
    new (&global->u.varOrConst) Global::U::VarOrConst(constant);
    return globalMap_.putNew(var, global);
  }

 public:
  bool addMathBuiltinConstant(PropertyName* var, double constant,
                              PropertyName* field) {
    UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
    if (!fieldChars) {
      return false;
    }

    if (!addGlobalDoubleConstant(var, constant)) {
      return false;
    }

    AsmJSGlobal g(AsmJSGlobal::Constant, std::move(fieldChars));
    g.pod.u.constant.value_ = constant;
    g.pod.u.constant.kind_ = AsmJSGlobal::MathConstant;
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }
  bool addGlobalConstant(PropertyName* var, double constant,
                         PropertyName* field) {
    UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
    if (!fieldChars) {
      return false;
    }

    if (!addGlobalDoubleConstant(var, constant)) {
      return false;
    }

    AsmJSGlobal g(AsmJSGlobal::Constant, std::move(fieldChars));
    g.pod.u.constant.value_ = constant;
    g.pod.u.constant.kind_ = AsmJSGlobal::GlobalConstant;
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }
  bool addArrayViewCtor(PropertyName* var, Scalar::Type vt,
                        PropertyName* field) {
    UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
    if (!fieldChars) {
      return false;
    }

    Global* global = validationLifo_.new_<Global>(Global::ArrayViewCtor);
    if (!global) {
      return false;
    }
    new (&global->u.viewType_) Scalar::Type(vt);
    if (!globalMap_.putNew(var, global)) {
      return false;
    }

    AsmJSGlobal g(AsmJSGlobal::ArrayViewCtor, std::move(fieldChars));
    g.pod.u.viewType_ = vt;
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }
  bool addFFI(PropertyName* var, PropertyName* field) {
    UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
    if (!fieldChars) {
      return false;
    }

    if (asmJSMetadata_->numFFIs == UINT32_MAX) {
      return false;
    }
    uint32_t ffiIndex = asmJSMetadata_->numFFIs++;

    Global* global = validationLifo_.new_<Global>(Global::FFI);
    if (!global) {
      return false;
    }
    new (&global->u.ffiIndex_) uint32_t(ffiIndex);
    if (!globalMap_.putNew(var, global)) {
      return false;
    }

    AsmJSGlobal g(AsmJSGlobal::FFI, std::move(fieldChars));
    g.pod.u.ffiIndex_ = ffiIndex;
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }
  bool addExportField(const Func& func, PropertyName* maybeField) {
    // Record the field name of this export.
    CacheableChars fieldChars;
    if (maybeField) {
      fieldChars = StringToNewUTF8CharsZ(cx_, *maybeField);
    } else {
      fieldChars = DuplicateString("");
    }
    if (!fieldChars) {
      return false;
    }

    // Declare which function is exported which gives us an index into the
    // module ExportVector.
    uint32_t funcIndex = funcImportMap_.count() + func.funcDefIndex();
    if (!env_.exports.emplaceBack(std::move(fieldChars), funcIndex,
                                  DefinitionKind::Function)) {
      return false;
    }

    // The exported function might have already been exported in which case
    // the index will refer into the range of AsmJSExports.
    return asmJSMetadata_->asmJSExports.emplaceBack(
        funcIndex, func.srcBegin() - asmJSMetadata_->srcStart,
        func.srcEnd() - asmJSMetadata_->srcStart);
  }
  bool addFuncDef(PropertyName* name, uint32_t firstUse, FuncType&& sig,
                  Func** func) {
    uint32_t sigIndex;
    if (!declareSig(std::move(sig), &sigIndex)) {
      return false;
    }

    uint32_t funcDefIndex = funcDefs_.length();
    if (funcDefIndex >= MaxFuncs) {
      return failCurrentOffset("too many functions");
    }

    Global* global = validationLifo_.new_<Global>(Global::Function);
    if (!global) {
      return false;
    }
    new (&global->u.funcDefIndex_) uint32_t(funcDefIndex);
    if (!globalMap_.putNew(name, global)) {
      return false;
    }
    if (!funcDefs_.emplaceBack(name, sigIndex, firstUse, funcDefIndex)) {
      return false;
    }
    *func = &funcDefs_.back();
    return true;
  }
  bool declareFuncPtrTable(FuncType&& sig, PropertyName* name,
                           uint32_t firstUse, uint32_t mask,
                           uint32_t* tableIndex) {
    if (mask > MaxTableInitialLength) {
      return failCurrentOffset("function pointer table too big");
    }

    MOZ_ASSERT(env_.tables.length() == tables_.length());
    *tableIndex = env_.tables.length();

    uint32_t sigIndex;
    if (!newSig(std::move(sig), &sigIndex)) {
      return false;
    }

    MOZ_ASSERT(sigIndex >= env_.asmJSSigToTableIndex.length());
    if (!env_.asmJSSigToTableIndex.resize(sigIndex + 1)) {
      return false;
    }

    env_.asmJSSigToTableIndex[sigIndex] = env_.tables.length();
    if (!env_.tables.emplaceBack(TableKind::TypedFunction, Limits(mask + 1))) {
      return false;
    }

    Global* global = validationLifo_.new_<Global>(Global::Table);
    if (!global) {
      return false;
    }

    new (&global->u.tableIndex_) uint32_t(*tableIndex);
    if (!globalMap_.putNew(name, global)) {
      return false;
    }

    Table* t = validationLifo_.new_<Table>(sigIndex, name, firstUse, mask);
    return t && tables_.append(t);
  }
  bool defineFuncPtrTable(uint32_t tableIndex, Uint32Vector&& elems) {
    Table& table = *tables_[tableIndex];
    if (table.defined()) {
      return false;
    }

    table.define();

    for (uint32_t& index : elems) {
      index += funcImportMap_.count();
    }

    MutableElemSegment seg = js_new<ElemSegment>();
    if (!seg) {
      return false;
    }
    seg->tableIndex = tableIndex;
    seg->offsetIfActive = Some(InitExpr(LitVal(uint32_t(0))));
    seg->elemFuncIndices = std::move(elems);
    return env_.elemSegments.append(std::move(seg));
  }
  bool declareImport(PropertyName* name, FuncType&& sig, unsigned ffiIndex,
                     uint32_t* importIndex) {
    FuncImportMap::AddPtr p =
        funcImportMap_.lookupForAdd(NamedSig::Lookup(name, sig));
    if (p) {
      *importIndex = p->value();
      return true;
    }

    *importIndex = funcImportMap_.count();
    MOZ_ASSERT(*importIndex == asmJSMetadata_->asmJSImports.length());

    if (*importIndex >= MaxImports) {
      return failCurrentOffset("too many imports");
    }

    if (!asmJSMetadata_->asmJSImports.emplaceBack(ffiIndex)) {
      return false;
    }

    uint32_t sigIndex;
    if (!declareSig(std::move(sig), &sigIndex)) {
      return false;
    }

    return funcImportMap_.add(p, NamedSig(name, sigIndex, env_.types),
                              *importIndex);
  }

  bool tryConstantAccess(uint64_t start, uint64_t width) {
    MOZ_ASSERT(UINT64_MAX - start > width);
    uint64_t len = start + width;
    if (len > uint64_t(INT32_MAX) + 1) {
      return false;
    }
    len = RoundUpToNextValidAsmJSHeapLength(len);
    if (len > env_.minMemoryLength) {
      env_.minMemoryLength = len;
    }
    return true;
  }

  // Error handling.
  bool hasAlreadyFailed() const { return !!errorString_; }

  bool failOffset(uint32_t offset, const char* str) {
    MOZ_ASSERT(!hasAlreadyFailed());
    MOZ_ASSERT(errorOffset_ == UINT32_MAX);
    MOZ_ASSERT(str);
    errorOffset_ = offset;
    errorString_ = DuplicateString(str);
    return false;
  }

  bool failCurrentOffset(const char* str) {
    return failOffset(tokenStream().anyCharsAccess().currentToken().pos.begin,
                      str);
  }

  bool fail(ParseNode* pn, const char* str) {
    return failOffset(pn->pn_pos.begin, str);
  }

  bool failfVAOffset(uint32_t offset, const char* fmt, va_list ap)
      MOZ_FORMAT_PRINTF(3, 0) {
    MOZ_ASSERT(!hasAlreadyFailed());
    MOZ_ASSERT(errorOffset_ == UINT32_MAX);
    MOZ_ASSERT(fmt);
    errorOffset_ = offset;
    errorString_ = JS_vsmprintf(fmt, ap);
    return false;
  }

  bool failfOffset(uint32_t offset, const char* fmt, ...)
      MOZ_FORMAT_PRINTF(3, 4) {
    va_list ap;
    va_start(ap, fmt);
    failfVAOffset(offset, fmt, ap);
    va_end(ap);
    return false;
  }

  bool failf(ParseNode* pn, const char* fmt, ...) MOZ_FORMAT_PRINTF(3, 4) {
    va_list ap;
    va_start(ap, fmt);
    failfVAOffset(pn->pn_pos.begin, fmt, ap);
    va_end(ap);
    return false;
  }

  bool failNameOffset(uint32_t offset, const char* fmt, PropertyName* name) {
    // This function is invoked without the caller properly rooting its locals.
    gc::AutoSuppressGC suppress(cx_);
    if (UniqueChars bytes = AtomToPrintableString(cx_, name)) {
      failfOffset(offset, fmt, bytes.get());
    }
    return false;
  }

  bool failName(ParseNode* pn, const char* fmt, PropertyName* name) {
    return failNameOffset(pn->pn_pos.begin, fmt, name);
  }

  bool failOverRecursed() {
    errorOverRecursed_ = true;
    return false;
  }

  unsigned numArrayViews() const { return arrayViews_.length(); }
  const ArrayView& arrayView(unsigned i) const { return arrayViews_[i]; }
  unsigned numFuncDefs() const { return funcDefs_.length(); }
  const Func& funcDef(unsigned i) const { return funcDefs_[i]; }
  unsigned numFuncPtrTables() const { return tables_.length(); }
  Table& table(unsigned i) const { return *tables_[i]; }

  const Global* lookupGlobal(PropertyName* name) const {
    if (GlobalMap::Ptr p = globalMap_.lookup(name)) {
      return p->value();
    }
    return nullptr;
  }

  Func* lookupFuncDef(PropertyName* name) {
    if (GlobalMap::Ptr p = globalMap_.lookup(name)) {
      Global* value = p->value();
      if (value->which() == Global::Function) {
        return &funcDefs_[value->funcDefIndex()];
      }
    }
    return nullptr;
  }

  bool lookupStandardLibraryMathName(PropertyName* name,
                                     MathBuiltin* mathBuiltin) const {
    if (MathNameMap::Ptr p = standardLibraryMathNames_.lookup(name)) {
      *mathBuiltin = p->value();
      return true;
    }
    return false;
  }

  bool startFunctionBodies() {
    if (!arrayViews_.empty()) {
      env_.memoryUsage = MemoryUsage::Unshared;
    } else {
      env_.memoryUsage = MemoryUsage::None;
    }
    return true;
  }
  SharedModule finish(UniqueLinkData* linkData) {
    MOZ_ASSERT(env_.funcTypes.empty());
    if (!env_.funcTypes.resize(funcImportMap_.count() + funcDefs_.length())) {
      return nullptr;
    }
    for (FuncImportMap::Range r = funcImportMap_.all(); !r.empty();
         r.popFront()) {
      uint32_t funcIndex = r.front().value();
      MOZ_ASSERT(!env_.funcTypes[funcIndex]);
      env_.funcTypes[funcIndex] =
          &env_.types[r.front().key().sigIndex()].funcType();
    }
    for (const Func& func : funcDefs_) {
      uint32_t funcIndex = funcImportMap_.count() + func.funcDefIndex();
      MOZ_ASSERT(!env_.funcTypes[funcIndex]);
      env_.funcTypes[funcIndex] = &env_.types[func.sigIndex()].funcType();
    }

    if (!env_.funcImportGlobalDataOffsets.resize(funcImportMap_.count())) {
      return nullptr;
    }

    MOZ_ASSERT(asmJSMetadata_->asmJSFuncNames.empty());
    if (!asmJSMetadata_->asmJSFuncNames.resize(funcImportMap_.count())) {
      return nullptr;
    }
    for (const Func& func : funcDefs_) {
      CacheableChars funcName = StringToNewUTF8CharsZ(cx_, *func.name());
      if (!funcName ||
          !asmJSMetadata_->asmJSFuncNames.emplaceBack(std::move(funcName))) {
        return nullptr;
      }
    }

    uint32_t endBeforeCurly =
        tokenStream().anyCharsAccess().currentToken().pos.end;
    asmJSMetadata_->srcLength = endBeforeCurly - asmJSMetadata_->srcStart;

    TokenPos pos;
    MOZ_ALWAYS_TRUE(
        tokenStream().peekTokenPos(&pos, TokenStreamShared::Operand));
    uint32_t endAfterCurly = pos.end;
    asmJSMetadata_->srcLengthWithRightBrace =
        endAfterCurly - asmJSMetadata_->srcStart;

    ScriptedCaller scriptedCaller;
    if (parser_.ss->filename()) {
      scriptedCaller.line = 0;  // unused
      scriptedCaller.filename = DuplicateString(parser_.ss->filename());
      if (!scriptedCaller.filename) {
        return nullptr;
      }
    }

    MutableCompileArgs args =
        cx_->new_<CompileArgs>(cx_, std::move(scriptedCaller));
    if (!args) {
      return nullptr;
    }

    uint32_t codeSectionSize = 0;
    for (const Func& func : funcDefs_) {
      codeSectionSize += func.bytes().length();
    }

    env_.codeSection.emplace();
    env_.codeSection->start = 0;
    env_.codeSection->size = codeSectionSize;

    // asm.js does not have any wasm bytecode to save; view-source is
    // provided through the ScriptSource.
    SharedBytes bytes = cx_->new_<ShareableBytes>();
    if (!bytes) {
      return nullptr;
    }

    ModuleGenerator mg(*args, &env_, nullptr, nullptr);
    if (!mg.init(asmJSMetadata_.get())) {
      return nullptr;
    }

    for (Func& func : funcDefs_) {
      if (!mg.compileFuncDef(funcImportMap_.count() + func.funcDefIndex(),
                             func.line(), func.bytes().begin(),
                             func.bytes().end(),
                             std::move(func.callSiteLineNums()))) {
        return nullptr;
      }
    }

    if (!mg.finishFuncDefs()) {
      return nullptr;
    }

    return mg.finishModule(*bytes, nullptr, linkData);
  }
};

/*****************************************************************************/
// Numeric literal utilities

static bool IsNumericNonFloatLiteral(ParseNode* pn) {
  // Note: '-' is never rolled into the number; numbers are always positive
  // and negations must be applied manually.
  return pn->isKind(ParseNodeKind::Number) ||
         (pn->isKind(ParseNodeKind::Neg) &&
          UnaryKid(pn)->isKind(ParseNodeKind::Number));
}

static bool IsCallToGlobal(ModuleValidator& m, ParseNode* pn,
                           const ModuleValidator::Global** global) {
  if (!pn->isKind(ParseNodeKind::Call)) {
    return false;
  }

  ParseNode* callee = CallCallee(pn);
  if (!callee->isKind(ParseNodeKind::Name)) {
    return false;
  }

  *global = m.lookupGlobal(callee->as<NameNode>().name());
  return !!*global;
}

static bool IsCoercionCall(ModuleValidator& m, ParseNode* pn, Type* coerceTo,
                           ParseNode** coercedExpr) {
  const ModuleValidator::Global* global;
  if (!IsCallToGlobal(m, pn, &global)) {
    return false;
  }

  if (CallArgListLength(pn) != 1) {
    return false;
  }

  if (coercedExpr) {
    *coercedExpr = CallArgList(pn);
  }

  if (global->isMathFunction() &&
      global->mathBuiltinFunction() == AsmJSMathBuiltin_fround) {
    *coerceTo = Type::Float;
    return true;
  }

  return false;
}

static bool IsFloatLiteral(ModuleValidator& m, ParseNode* pn) {
  ParseNode* coercedExpr;
  Type coerceTo;
  if (!IsCoercionCall(m, pn, &coerceTo, &coercedExpr)) {
    return false;
  }
  // Don't fold into || to avoid clang/memcheck bug (bug 1077031).
  if (!coerceTo.isFloat()) {
    return false;
  }
  return IsNumericNonFloatLiteral(coercedExpr);
}

static bool IsNumericLiteral(ModuleValidator& m, ParseNode* pn);

static NumLit ExtractNumericLiteral(ModuleValidator& m, ParseNode* pn);

static inline bool IsLiteralInt(ModuleValidator& m, ParseNode* pn,
                                uint32_t* u32);

static bool IsNumericLiteral(ModuleValidator& m, ParseNode* pn) {
  return IsNumericNonFloatLiteral(pn) || IsFloatLiteral(m, pn);
}

// The JS grammar treats -42 as -(42) (i.e., with separate grammar
// productions) for the unary - and literal 42). However, the asm.js spec
// recognizes -42 (modulo parens, so -(42) and -((42))) as a single literal
// so fold the two potential parse nodes into a single double value.
static double ExtractNumericNonFloatValue(ParseNode* pn,
                                          ParseNode** out = nullptr) {
  MOZ_ASSERT(IsNumericNonFloatLiteral(pn));

  if (pn->isKind(ParseNodeKind::Neg)) {
    pn = UnaryKid(pn);
    if (out) {
      *out = pn;
    }
    return -NumberNodeValue(pn);
  }

  return NumberNodeValue(pn);
}

static NumLit ExtractNumericLiteral(ModuleValidator& m, ParseNode* pn) {
  MOZ_ASSERT(IsNumericLiteral(m, pn));

  if (pn->isKind(ParseNodeKind::Call)) {
    // Float literals are explicitly coerced and thus the coerced literal may be
    // any valid (non-float) numeric literal.
    MOZ_ASSERT(CallArgListLength(pn) == 1);
    pn = CallArgList(pn);
    double d = ExtractNumericNonFloatValue(pn);
    return NumLit(NumLit::Float, DoubleValue(d));
  }

  double d = ExtractNumericNonFloatValue(pn, &pn);

  // The asm.js spec syntactically distinguishes any literal containing a
  // decimal point or the literal -0 as having double type.
  if (NumberNodeHasFrac(pn) || IsNegativeZero(d)) {
    return NumLit(NumLit::Double, DoubleValue(d));
  }

  // The syntactic checks above rule out these double values.
  MOZ_ASSERT(!IsNegativeZero(d));
  MOZ_ASSERT(!IsNaN(d));

  // Although doubles can only *precisely* represent 53-bit integers, they
  // can *imprecisely* represent integers much bigger than an int64_t.
  // Furthermore, d may be inf or -inf. In both cases, casting to an int64_t
  // is undefined, so test against the integer bounds using doubles.
  if (d < double(INT32_MIN) || d > double(UINT32_MAX)) {
    return NumLit(NumLit::OutOfRangeInt, UndefinedValue());
  }

  // With the above syntactic and range limitations, d is definitely an
  // integer in the range [INT32_MIN, UINT32_MAX] range.
  int64_t i64 = int64_t(d);
  if (i64 >= 0) {
    if (i64 <= INT32_MAX) {
      return NumLit(NumLit::Fixnum, Int32Value(i64));
    }
    MOZ_ASSERT(i64 <= UINT32_MAX);
    return NumLit(NumLit::BigUnsigned, Int32Value(uint32_t(i64)));
  }
  MOZ_ASSERT(i64 >= INT32_MIN);
  return NumLit(NumLit::NegativeInt, Int32Value(i64));
}

static inline bool IsLiteralInt(const NumLit& lit, uint32_t* u32) {
  switch (lit.which()) {
    case NumLit::Fixnum:
    case NumLit::BigUnsigned:
    case NumLit::NegativeInt:
      *u32 = lit.toUint32();
      return true;
    case NumLit::Double:
    case NumLit::Float:
    case NumLit::OutOfRangeInt:
      return false;
  }
  MOZ_MAKE_COMPILER_ASSUME_IS_UNREACHABLE("Bad literal type");
}

static inline bool IsLiteralInt(ModuleValidator& m, ParseNode* pn,
                                uint32_t* u32) {
  return IsNumericLiteral(m, pn) &&
         IsLiteralInt(ExtractNumericLiteral(m, pn), u32);
}

/*****************************************************************************/

namespace {

typedef Vector<PropertyName*, 4, SystemAllocPolicy> LabelVector;

// Encapsulates the building of an asm bytecode function from an asm.js function
// source code, packing the asm.js code into the asm bytecode form that can
// be decoded and compiled with a FunctionCompiler.
class MOZ_STACK_CLASS FunctionValidator {
 public:
  struct Local {
    Type type;
    unsigned slot;
    Local(Type t, unsigned slot) : type(t), slot(slot) {
      MOZ_ASSERT(type.isCanonicalValType());
    }
  };

 private:
  typedef HashMap<PropertyName*, Local> LocalMap;
  typedef HashMap<PropertyName*, uint32_t> LabelMap;

  ModuleValidator& m_;
  ParseNode* fn_;
  Bytes bytes_;
  Encoder encoder_;
  Uint32Vector callSiteLineNums_;
  LocalMap locals_;

  // Labels
  LabelMap breakLabels_;
  LabelMap continueLabels_;
  Uint32Vector breakableStack_;
  Uint32Vector continuableStack_;
  uint32_t blockDepth_;

  bool hasAlreadyReturned_;
  ExprType ret_;

 public:
  FunctionValidator(ModuleValidator& m, ParseNode* fn)
      : m_(m),
        fn_(fn),
        encoder_(bytes_),
        locals_(m.cx()),
        breakLabels_(m.cx()),
        continueLabels_(m.cx()),
        blockDepth_(0),
        hasAlreadyReturned_(false),
        ret_(ExprType::Limit) {}

  ModuleValidator& m() const { return m_; }
  JSContext* cx() const { return m_.cx(); }
  ParseNode* fn() const { return fn_; }

  void define(ModuleValidator::Func* func, unsigned line) {
    MOZ_ASSERT(!blockDepth_);
    MOZ_ASSERT(breakableStack_.empty());
    MOZ_ASSERT(continuableStack_.empty());
    MOZ_ASSERT(breakLabels_.empty());
    MOZ_ASSERT(continueLabels_.empty());
    func->define(fn_, line, std::move(bytes_), std::move(callSiteLineNums_));
  }

  bool fail(ParseNode* pn, const char* str) { return m_.fail(pn, str); }

  bool failf(ParseNode* pn, const char* fmt, ...) MOZ_FORMAT_PRINTF(3, 4) {
    va_list ap;
    va_start(ap, fmt);
    m_.failfVAOffset(pn->pn_pos.begin, fmt, ap);
    va_end(ap);
    return false;
  }

  bool failName(ParseNode* pn, const char* fmt, PropertyName* name) {
    return m_.failName(pn, fmt, name);
  }

  /***************************************************** Local scope setup */

  bool addLocal(ParseNode* pn, PropertyName* name, Type type) {
    LocalMap::AddPtr p = locals_.lookupForAdd(name);
    if (p) {
      return failName(pn, "duplicate local name '%s' not allowed", name);
    }
    return locals_.add(p, name, Local(type, locals_.count()));
  }

  /****************************** For consistency of returns in a function */

  bool hasAlreadyReturned() const { return hasAlreadyReturned_; }

  ExprType returnedType() const { return ret_; }

  void setReturnedType(ExprType ret) {
    ret_ = ret;
    hasAlreadyReturned_ = true;
  }

  /**************************************************************** Labels */
 private:
  bool writeBr(uint32_t absolute, Op op = Op::Br) {
    MOZ_ASSERT(op == Op::Br || op == Op::BrIf);
    MOZ_ASSERT(absolute < blockDepth_);
    return encoder().writeOp(op) &&
           encoder().writeVarU32(blockDepth_ - 1 - absolute);
  }
  void removeLabel(PropertyName* label, LabelMap* map) {
    LabelMap::Ptr p = map->lookup(label);
    MOZ_ASSERT(p);
    map->remove(p);
  }

 public:
  bool pushBreakableBlock() {
    return encoder().writeOp(Op::Block) &&
           encoder().writeFixedU8(uint8_t(ExprType::Void)) &&
           breakableStack_.append(blockDepth_++);
  }
  bool popBreakableBlock() {
    MOZ_ALWAYS_TRUE(breakableStack_.popCopy() == --blockDepth_);
    return encoder().writeOp(Op::End);
  }

  bool pushUnbreakableBlock(const LabelVector* labels = nullptr) {
    if (labels) {
      for (PropertyName* label : *labels) {
        if (!breakLabels_.putNew(label, blockDepth_)) {
          return false;
        }
      }
    }
    blockDepth_++;
    return encoder().writeOp(Op::Block) &&
           encoder().writeFixedU8(uint8_t(ExprType::Void));
  }
  bool popUnbreakableBlock(const LabelVector* labels = nullptr) {
    if (labels) {
      for (PropertyName* label : *labels) {
        removeLabel(label, &breakLabels_);
      }
    }
    --blockDepth_;
    return encoder().writeOp(Op::End);
  }

  bool pushContinuableBlock() {
    return encoder().writeOp(Op::Block) &&
           encoder().writeFixedU8(uint8_t(ExprType::Void)) &&
           continuableStack_.append(blockDepth_++);
  }
  bool popContinuableBlock() {
    MOZ_ALWAYS_TRUE(continuableStack_.popCopy() == --blockDepth_);
    return encoder().writeOp(Op::End);
  }

  bool pushLoop() {
    return encoder().writeOp(Op::Block) &&
           encoder().writeFixedU8(uint8_t(ExprType::Void)) &&
           encoder().writeOp(Op::Loop) &&
           encoder().writeFixedU8(uint8_t(ExprType::Void)) &&
           breakableStack_.append(blockDepth_++) &&
           continuableStack_.append(blockDepth_++);
  }
  bool popLoop() {
    MOZ_ALWAYS_TRUE(continuableStack_.popCopy() == --blockDepth_);
    MOZ_ALWAYS_TRUE(breakableStack_.popCopy() == --blockDepth_);
    return encoder().writeOp(Op::End) && encoder().writeOp(Op::End);
  }

  bool pushIf(size_t* typeAt) {
    ++blockDepth_;
    return encoder().writeOp(Op::If) && encoder().writePatchableFixedU7(typeAt);
  }
  bool switchToElse() {
    MOZ_ASSERT(blockDepth_ > 0);
    return encoder().writeOp(Op::Else);
  }
  void setIfType(size_t typeAt, ExprType type) {
    encoder().patchFixedU7(typeAt, uint8_t(type.code()));
  }
  bool popIf() {
    MOZ_ASSERT(blockDepth_ > 0);
    --blockDepth_;
    return encoder().writeOp(Op::End);
  }
  bool popIf(size_t typeAt, ExprType type) {
    MOZ_ASSERT(blockDepth_ > 0);
    --blockDepth_;
    if (!encoder().writeOp(Op::End)) {
      return false;
    }

    setIfType(typeAt, type);
    return true;
  }

  bool writeBreakIf() { return writeBr(breakableStack_.back(), Op::BrIf); }
  bool writeContinueIf() { return writeBr(continuableStack_.back(), Op::BrIf); }
  bool writeUnlabeledBreakOrContinue(bool isBreak) {
    return writeBr(isBreak ? breakableStack_.back() : continuableStack_.back());
  }
  bool writeContinue() { return writeBr(continuableStack_.back()); }

  bool addLabels(const LabelVector& labels, uint32_t relativeBreakDepth,
                 uint32_t relativeContinueDepth) {
    for (PropertyName* label : labels) {
      if (!breakLabels_.putNew(label, blockDepth_ + relativeBreakDepth)) {
        return false;
      }
      if (!continueLabels_.putNew(label, blockDepth_ + relativeContinueDepth)) {
        return false;
      }
    }
    return true;
  }
  void removeLabels(const LabelVector& labels) {
    for (PropertyName* label : labels) {
      removeLabel(label, &breakLabels_);
      removeLabel(label, &continueLabels_);
    }
  }
  bool writeLabeledBreakOrContinue(PropertyName* label, bool isBreak) {
    LabelMap& map = isBreak ? breakLabels_ : continueLabels_;
    if (LabelMap::Ptr p = map.lookup(label)) {
      return writeBr(p->value());
    }
    MOZ_CRASH("nonexistent label");
  }

  /*************************************************** Read-only interface */

  const Local* lookupLocal(PropertyName* name) const {
    if (auto p = locals_.lookup(name)) {
      return &p->value();
    }
    return nullptr;
  }

  const ModuleValidator::Global* lookupGlobal(PropertyName* name) const {
    if (locals_.has(name)) {
      return nullptr;
    }
    return m_.lookupGlobal(name);
  }

  size_t numLocals() const { return locals_.count(); }

  /**************************************************** Encoding interface */

  Encoder& encoder() { return encoder_; }

  MOZ_MUST_USE bool writeInt32Lit(int32_t i32) {
    return encoder().writeOp(Op::I32Const) && encoder().writeVarS32(i32);
  }
  MOZ_MUST_USE bool writeConstExpr(const NumLit& lit) {
    switch (lit.which()) {
      case NumLit::Fixnum:
      case NumLit::NegativeInt:
      case NumLit::BigUnsigned:
        return writeInt32Lit(lit.toInt32());
      case NumLit::Float:
        return encoder().writeOp(Op::F32Const) &&
               encoder().writeFixedF32(lit.toFloat());
      case NumLit::Double:
        return encoder().writeOp(Op::F64Const) &&
               encoder().writeFixedF64(lit.toDouble());
      case NumLit::OutOfRangeInt:
        break;
    }
    MOZ_CRASH("unexpected literal type");
  }
  MOZ_MUST_USE bool writeCall(ParseNode* pn, Op op) {
    if (!encoder().writeOp(op)) {
      return false;
    }

    TokenStreamAnyChars& anyChars = m().tokenStream().anyCharsAccess();
    return callSiteLineNums_.append(
        anyChars.srcCoords.lineNum(pn->pn_pos.begin));
  }
  MOZ_MUST_USE bool writeCall(ParseNode* pn, MozOp op) {
    if (!encoder().writeOp(op)) {
      return false;
    }

    TokenStreamAnyChars& anyChars = m().tokenStream().anyCharsAccess();
    return callSiteLineNums_.append(
        anyChars.srcCoords.lineNum(pn->pn_pos.begin));
  }
  MOZ_MUST_USE bool prepareCall(ParseNode* pn) {
    TokenStreamAnyChars& anyChars = m().tokenStream().anyCharsAccess();
    return callSiteLineNums_.append(
        anyChars.srcCoords.lineNum(pn->pn_pos.begin));
  }
};

} /* anonymous namespace */

/*****************************************************************************/
// asm.js type-checking and code-generation algorithm

static bool CheckIdentifier(ModuleValidator& m, ParseNode* usepn,
                            PropertyName* name) {
  if (name == m.cx()->names().arguments || name == m.cx()->names().eval) {
    return m.failName(usepn, "'%s' is not an allowed identifier", name);
  }
  return true;
}

static bool CheckModuleLevelName(ModuleValidator& m, ParseNode* usepn,
                                 PropertyName* name) {
  if (!CheckIdentifier(m, usepn, name)) {
    return false;
  }

  if (name == m.moduleFunctionName() || name == m.globalArgumentName() ||
      name == m.importArgumentName() || name == m.bufferArgumentName() ||
      m.lookupGlobal(name)) {
    return m.failName(usepn, "duplicate name '%s' not allowed", name);
  }

  return true;
}

static bool CheckFunctionHead(ModuleValidator& m, CodeNode* funNode) {
  FunctionBox* funbox = funNode->funbox();
  MOZ_ASSERT(!funbox->hasExprBody());

  if (funbox->hasRest()) {
    return m.fail(funNode, "rest args not allowed");
  }
  if (funbox->hasDestructuringArgs) {
    return m.fail(funNode, "destructuring args not allowed");
  }
  return true;
}

static bool CheckArgument(ModuleValidator& m, ParseNode* arg,
                          PropertyName** name) {
  *name = nullptr;

  if (!arg->isKind(ParseNodeKind::Name)) {
    return m.fail(arg, "argument is not a plain name");
  }

  PropertyName* argName = arg->as<NameNode>().name();
  ;
  if (!CheckIdentifier(m, arg, argName)) {
    return false;
  }

  *name = argName;
  return true;
}

static bool CheckModuleArgument(ModuleValidator& m, ParseNode* arg,
                                PropertyName** name) {
  if (!CheckArgument(m, arg, name)) {
    return false;
  }

  if (!CheckModuleLevelName(m, arg, *name)) {
    return false;
  }

  return true;
}

static bool CheckModuleArguments(ModuleValidator& m, CodeNode* funNode) {
  unsigned numFormals;
  ParseNode* arg1 = FunctionFormalParametersList(funNode, &numFormals);
  ParseNode* arg2 = arg1 ? NextNode(arg1) : nullptr;
  ParseNode* arg3 = arg2 ? NextNode(arg2) : nullptr;

  if (numFormals > 3) {
    return m.fail(funNode, "asm.js modules takes at most 3 argument");
  }

  PropertyName* arg1Name = nullptr;
  if (arg1 && !CheckModuleArgument(m, arg1, &arg1Name)) {
    return false;
  }
  if (!m.initGlobalArgumentName(arg1Name)) {
    return false;
  }

  PropertyName* arg2Name = nullptr;
  if (arg2 && !CheckModuleArgument(m, arg2, &arg2Name)) {
    return false;
  }
  if (!m.initImportArgumentName(arg2Name)) {
    return false;
  }

  PropertyName* arg3Name = nullptr;
  if (arg3 && !CheckModuleArgument(m, arg3, &arg3Name)) {
    return false;
  }
  if (!m.initBufferArgumentName(arg3Name)) {
    return false;
  }

  return true;
}

static bool CheckPrecedingStatements(ModuleValidator& m, ParseNode* stmtList) {
  MOZ_ASSERT(stmtList->isKind(ParseNodeKind::StatementList));

  ParseNode* stmt = ListHead(stmtList);
  for (unsigned i = 0, n = ListLength(stmtList); i < n; i++) {
    if (!IsIgnoredDirective(m.cx(), stmt)) {
      return m.fail(stmt, "invalid asm.js statement");
    }
  }

  return true;
}

static bool CheckGlobalVariableInitConstant(ModuleValidator& m,
                                            PropertyName* varName,
                                            ParseNode* initNode, bool isConst) {
  NumLit lit = ExtractNumericLiteral(m, initNode);
  if (!lit.valid()) {
    return m.fail(initNode,
                  "global initializer is out of representable integer range");
  }

  Type canonicalType = Type::canonicalize(Type::lit(lit));
  if (!canonicalType.isGlobalVarType()) {
    return m.fail(initNode, "global variable type not allowed");
  }

  return m.addGlobalVarInit(varName, lit, canonicalType, isConst);
}

static bool CheckTypeAnnotation(ModuleValidator& m, ParseNode* coercionNode,
                                Type* coerceTo,
                                ParseNode** coercedExpr = nullptr) {
  switch (coercionNode->getKind()) {
    case ParseNodeKind::BitOr: {
      ParseNode* rhs = BitwiseRight(coercionNode);
      uint32_t i;
      if (!IsLiteralInt(m, rhs, &i) || i != 0) {
        return m.fail(rhs, "must use |0 for argument/return coercion");
      }
      *coerceTo = Type::Int;
      if (coercedExpr) {
        *coercedExpr = BitwiseLeft(coercionNode);
      }
      return true;
    }
    case ParseNodeKind::Pos: {
      *coerceTo = Type::Double;
      if (coercedExpr) {
        *coercedExpr = UnaryKid(coercionNode);
      }
      return true;
    }
    case ParseNodeKind::Call: {
      if (IsCoercionCall(m, coercionNode, coerceTo, coercedExpr)) {
        return true;
      }
      break;
    }
    default:;
  }

  return m.fail(coercionNode, "must be of the form +x, x|0 or fround(x)");
}

static bool CheckGlobalVariableInitImport(ModuleValidator& m,
                                          PropertyName* varName,
                                          ParseNode* initNode, bool isConst) {
  Type coerceTo;
  ParseNode* coercedExpr;
  if (!CheckTypeAnnotation(m, initNode, &coerceTo, &coercedExpr)) {
    return false;
  }

  if (!coercedExpr->isKind(ParseNodeKind::Dot)) {
    return m.failName(coercedExpr, "invalid import expression for global '%s'",
                      varName);
  }

  if (!coerceTo.isGlobalVarType()) {
    return m.fail(initNode, "global variable type not allowed");
  }

  ParseNode* base = DotBase(coercedExpr);
  PropertyName* field = DotMember(coercedExpr);

  PropertyName* importName = m.importArgumentName();
  if (!importName) {
    return m.fail(coercedExpr,
                  "cannot import without an asm.js foreign parameter");
  }
  if (!IsUseOfName(base, importName)) {
    return m.failName(coercedExpr, "base of import expression must be '%s'",
                      importName);
  }

  return m.addGlobalVarImport(varName, field, coerceTo, isConst);
}

static bool IsArrayViewCtorName(ModuleValidator& m, PropertyName* name,
                                Scalar::Type* type) {
  JSAtomState& names = m.cx()->names();
  if (name == names.Int8Array) {
    *type = Scalar::Int8;
  } else if (name == names.Uint8Array) {
    *type = Scalar::Uint8;
  } else if (name == names.Int16Array) {
    *type = Scalar::Int16;
  } else if (name == names.Uint16Array) {
    *type = Scalar::Uint16;
  } else if (name == names.Int32Array) {
    *type = Scalar::Int32;
  } else if (name == names.Uint32Array) {
    *type = Scalar::Uint32;
  } else if (name == names.Float32Array) {
    *type = Scalar::Float32;
  } else if (name == names.Float64Array) {
    *type = Scalar::Float64;
  } else {
    return false;
  }
  return true;
}

static bool CheckNewArrayViewArgs(ModuleValidator& m, ParseNode* newExpr,
                                  PropertyName* bufferName) {
  ParseNode* ctorExpr = BinaryLeft(newExpr);
  ParseNode* ctorArgs = BinaryRight(newExpr);
  ParseNode* bufArg = ListHead(ctorArgs);
  if (!bufArg || NextNode(bufArg) != nullptr) {
    return m.fail(ctorExpr,
                  "array view constructor takes exactly one argument");
  }

  if (!IsUseOfName(bufArg, bufferName)) {
    return m.failName(bufArg, "argument to array view constructor must be '%s'",
                      bufferName);
  }

  return true;
}

static bool CheckNewArrayView(ModuleValidator& m, PropertyName* varName,
                              ParseNode* newExpr) {
  PropertyName* globalName = m.globalArgumentName();
  if (!globalName) {
    return m.fail(
        newExpr, "cannot create array view without an asm.js global parameter");
  }

  PropertyName* bufferName = m.bufferArgumentName();
  if (!bufferName) {
    return m.fail(newExpr,
                  "cannot create array view without an asm.js heap parameter");
  }

  ParseNode* ctorExpr = BinaryLeft(newExpr);

  PropertyName* field;
  Scalar::Type type;
  if (ctorExpr->isKind(ParseNodeKind::Dot)) {
    ParseNode* base = DotBase(ctorExpr);

    if (!IsUseOfName(base, globalName)) {
      return m.failName(base, "expecting '%s.*Array", globalName);
    }

    field = DotMember(ctorExpr);
    if (!IsArrayViewCtorName(m, field, &type)) {
      return m.fail(ctorExpr, "could not match typed array name");
    }
  } else {
    if (!ctorExpr->isKind(ParseNodeKind::Name)) {
      return m.fail(ctorExpr,
                    "expecting name of imported array view constructor");
    }

    PropertyName* globalName = ctorExpr->as<NameNode>().name();
    const ModuleValidator::Global* global = m.lookupGlobal(globalName);
    if (!global) {
      return m.failName(ctorExpr, "%s not found in module global scope",
                        globalName);
    }

    if (global->which() != ModuleValidator::Global::ArrayViewCtor) {
      return m.failName(ctorExpr,
                        "%s must be an imported array view constructor",
                        globalName);
    }

    field = nullptr;
    type = global->viewType();
  }

  if (!CheckNewArrayViewArgs(m, newExpr, bufferName)) {
    return false;
  }

  return m.addArrayView(varName, type, field);
}

static bool CheckGlobalMathImport(ModuleValidator& m, ParseNode* initNode,
                                  PropertyName* varName, PropertyName* field) {
  // Math builtin, with the form glob.Math.[[builtin]]
  ModuleValidator::MathBuiltin mathBuiltin;
  if (!m.lookupStandardLibraryMathName(field, &mathBuiltin)) {
    return m.failName(initNode, "'%s' is not a standard Math builtin", field);
  }

  switch (mathBuiltin.kind) {
    case ModuleValidator::MathBuiltin::Function:
      return m.addMathBuiltinFunction(varName, mathBuiltin.u.func, field);
    case ModuleValidator::MathBuiltin::Constant:
      return m.addMathBuiltinConstant(varName, mathBuiltin.u.cst, field);
    default:
      break;
  }
  MOZ_CRASH("unexpected or uninitialized math builtin type");
}

static bool CheckGlobalDotImport(ModuleValidator& m, PropertyName* varName,
                                 ParseNode* initNode) {
  ParseNode* base = DotBase(initNode);
  PropertyName* field = DotMember(initNode);

  if (base->isKind(ParseNodeKind::Dot)) {
    ParseNode* global = DotBase(base);
    PropertyName* math = DotMember(base);

    PropertyName* globalName = m.globalArgumentName();
    if (!globalName) {
      return m.fail(
          base, "import statement requires the module have a stdlib parameter");
    }

    if (!IsUseOfName(global, globalName)) {
      if (global->isKind(ParseNodeKind::Dot)) {
        return m.failName(base,
                          "imports can have at most two dot accesses "
                          "(e.g. %s.Math.sin)",
                          globalName);
      }
      return m.failName(base, "expecting %s.*", globalName);
    }

    if (math == m.cx()->names().Math) {
      return CheckGlobalMathImport(m, initNode, varName, field);
    }
    return m.failName(base, "expecting %s.Math", globalName);
  }

  if (!base->isKind(ParseNodeKind::Name)) {
    return m.fail(base, "expected name of variable or parameter");
  }

  auto baseName = base->as<NameNode>().name();
  if (baseName == m.globalArgumentName()) {
    if (field == m.cx()->names().NaN) {
      return m.addGlobalConstant(varName, GenericNaN(), field);
    }
    if (field == m.cx()->names().Infinity) {
      return m.addGlobalConstant(varName, PositiveInfinity<double>(), field);
    }

    Scalar::Type type;
    if (IsArrayViewCtorName(m, field, &type)) {
      return m.addArrayViewCtor(varName, type, field);
    }

    return m.failName(
        initNode, "'%s' is not a standard constant or typed array name", field);
  }

  if (baseName != m.importArgumentName()) {
    return m.fail(base, "expected global or import name");
  }

  return m.addFFI(varName, field);
}

static bool CheckModuleGlobal(ModuleValidator& m, ParseNode* var,
                              bool isConst) {
  if (!var->isKind(ParseNodeKind::Name)) {
    return m.fail(var, "import variable is not a plain name");
  }

  PropertyName* varName = var->as<NameNode>().name();
  if (!CheckModuleLevelName(m, var, varName)) {
    return false;
  }

  ParseNode* initNode = MaybeInitializer(var);
  if (!initNode) {
    return m.fail(var, "module import needs initializer");
  }

  if (IsNumericLiteral(m, initNode)) {
    return CheckGlobalVariableInitConstant(m, varName, initNode, isConst);
  }

  if (initNode->isKind(ParseNodeKind::BitOr) ||
      initNode->isKind(ParseNodeKind::Pos) ||
      initNode->isKind(ParseNodeKind::Call)) {
    return CheckGlobalVariableInitImport(m, varName, initNode, isConst);
  }

  if (initNode->isKind(ParseNodeKind::New)) {
    return CheckNewArrayView(m, varName, initNode);
  }

  if (initNode->isKind(ParseNodeKind::Dot)) {
    return CheckGlobalDotImport(m, varName, initNode);
  }

  return m.fail(initNode, "unsupported import expression");
}

static bool CheckModuleProcessingDirectives(ModuleValidator& m) {
  auto& ts = m.parser().tokenStream;
  while (true) {
    bool matched;
    if (!ts.matchToken(&matched, TokenKind::String,
                       TokenStreamShared::Operand)) {
      return false;
    }
    if (!matched) {
      return true;
    }

    if (!IsIgnoredDirectiveName(m.cx(),
                                ts.anyCharsAccess().currentToken().atom())) {
      return m.failCurrentOffset("unsupported processing directive");
    }

    TokenKind tt;
    if (!ts.getToken(&tt)) {
      return false;
    }
    if (tt != TokenKind::Semi) {
      return m.failCurrentOffset("expected semicolon after string literal");
    }
  }
}

static bool CheckModuleGlobals(ModuleValidator& m) {
  while (true) {
    ParseNode* varStmt;
    if (!ParseVarOrConstStatement(m.parser(), &varStmt)) {
      return false;
    }
    if (!varStmt) {
      break;
    }
    for (ParseNode* var = VarListHead(varStmt); var; var = NextNode(var)) {
      if (!CheckModuleGlobal(m, var, varStmt->isKind(ParseNodeKind::Const))) {
        return false;
      }
    }
  }

  return true;
}

static bool ArgFail(FunctionValidator& f, PropertyName* argName,
                    ParseNode* stmt) {
  return f.failName(stmt,
                    "expecting argument type declaration for '%s' of the "
                    "form 'arg = arg|0' or 'arg = +arg' or 'arg = fround(arg)'",
                    argName);
}

static bool CheckArgumentType(FunctionValidator& f, ParseNode* stmt,
                              PropertyName* name, Type* type) {
  if (!stmt || !IsExpressionStatement(stmt)) {
    return ArgFail(f, name, stmt ? stmt : f.fn());
  }

  ParseNode* initNode = ExpressionStatementExpr(stmt);
  if (!initNode->isKind(ParseNodeKind::Assign)) {
    return ArgFail(f, name, stmt);
  }

  ParseNode* argNode = BinaryLeft(initNode);
  ParseNode* coercionNode = BinaryRight(initNode);

  if (!IsUseOfName(argNode, name)) {
    return ArgFail(f, name, stmt);
  }

  ParseNode* coercedExpr;
  if (!CheckTypeAnnotation(f.m(), coercionNode, type, &coercedExpr)) {
    return false;
  }

  if (!type->isArgType()) {
    return f.failName(stmt, "invalid type for argument '%s'", name);
  }

  if (!IsUseOfName(coercedExpr, name)) {
    return ArgFail(f, name, stmt);
  }

  return true;
}

static bool CheckProcessingDirectives(ModuleValidator& m,
                                      ParseNode** stmtIter) {
  ParseNode* stmt = *stmtIter;

  while (stmt && IsIgnoredDirective(m.cx(), stmt)) {
    stmt = NextNode(stmt);
  }

  *stmtIter = stmt;
  return true;
}

static bool CheckArguments(FunctionValidator& f, ParseNode** stmtIter,
                           ValTypeVector* argTypes) {
  ParseNode* stmt = *stmtIter;

  unsigned numFormals;
  ParseNode* argpn = FunctionFormalParametersList(f.fn(), &numFormals);

  for (unsigned i = 0; i < numFormals;
       i++, argpn = NextNode(argpn), stmt = NextNode(stmt)) {
    PropertyName* name;
    if (!CheckArgument(f.m(), argpn, &name)) {
      return false;
    }

    Type type;
    if (!CheckArgumentType(f, stmt, name, &type)) {
      return false;
    }

    if (!argTypes->append(type.canonicalToValType())) {
      return false;
    }

    if (!f.addLocal(argpn, name, type)) {
      return false;
    }
  }

  *stmtIter = stmt;
  return true;
}

static bool IsLiteralOrConst(FunctionValidator& f, ParseNode* pn, NumLit* lit) {
  if (pn->isKind(ParseNodeKind::Name)) {
    const ModuleValidator::Global* global =
        f.lookupGlobal(pn->as<NameNode>().name());
    if (!global ||
        global->which() != ModuleValidator::Global::ConstantLiteral) {
      return false;
    }

    *lit = global->constLiteralValue();
    return true;
  }

  if (!IsNumericLiteral(f.m(), pn)) {
    return false;
  }

  *lit = ExtractNumericLiteral(f.m(), pn);
  return true;
}

static bool CheckFinalReturn(FunctionValidator& f,
                             ParseNode* lastNonEmptyStmt) {
  if (!f.encoder().writeOp(Op::End)) {
    return false;
  }

  if (!f.hasAlreadyReturned()) {
    f.setReturnedType(ExprType::Void);
    return true;
  }

  if (!lastNonEmptyStmt->isKind(ParseNodeKind::Return) &&
      !IsVoid(f.returnedType())) {
    return f.fail(lastNonEmptyStmt,
                  "void incompatible with previous return type");
  }

  return true;
}

static bool CheckVariable(FunctionValidator& f, ParseNode* var,
                          ValTypeVector* types, Vector<NumLit>* inits) {
  if (!var->isKind(ParseNodeKind::Name)) {
    return f.fail(var, "local variable is not a plain name");
  }

  PropertyName* name = var->as<NameNode>().name();

  if (!CheckIdentifier(f.m(), var, name)) {
    return false;
  }

  ParseNode* initNode = MaybeInitializer(var);
  if (!initNode) {
    return f.failName(
        var, "var '%s' needs explicit type declaration via an initial value",
        name);
  }

  NumLit lit;
  if (!IsLiteralOrConst(f, initNode, &lit)) {
    return f.failName(
        var, "var '%s' initializer must be literal or const literal", name);
  }

  if (!lit.valid()) {
    return f.failName(var, "var '%s' initializer out of range", name);
  }

  Type type = Type::canonicalize(Type::lit(lit));

  return f.addLocal(var, name, type) &&
         types->append(type.canonicalToValType()) && inits->append(lit);
}

static bool CheckVariables(FunctionValidator& f, ParseNode** stmtIter) {
  ParseNode* stmt = *stmtIter;

  uint32_t firstVar = f.numLocals();

  ValTypeVector types;
  Vector<NumLit> inits(f.cx());

  for (; stmt && stmt->isKind(ParseNodeKind::Var);
       stmt = NextNonEmptyStatement(stmt)) {
    for (ParseNode* var = VarListHead(stmt); var; var = NextNode(var)) {
      if (!CheckVariable(f, var, &types, &inits)) {
        return false;
      }
    }
  }

  MOZ_ASSERT(f.encoder().empty());

  if (!EncodeLocalEntries(f.encoder(), types)) {
    return false;
  }

  for (uint32_t i = 0; i < inits.length(); i++) {
    NumLit lit = inits[i];
    if (lit.isZeroBits()) {
      continue;
    }
    if (!f.writeConstExpr(lit)) {
      return false;
    }
    if (!f.encoder().writeOp(Op::SetLocal)) {
      return false;
    }
    if (!f.encoder().writeVarU32(firstVar + i)) {
      return false;
    }
  }
||||||| merged common ancestors
// The ModuleValidator encapsulates the entire validation of an asm.js module.
// Its lifetime goes from the validation of the top components of an asm.js
// module (all the globals), the emission of bytecode for all the functions in
// the module and the validation of function's pointer tables. It also finishes
// the compilation of all the module's stubs.
//
// Rooting note: ModuleValidator is a stack class that contains unrooted
// PropertyName (JSAtom) pointers.  This is safe because it cannot be
// constructed without a TokenStream reference.  TokenStream is itself a stack
// class that cannot be constructed without an AutoKeepAtoms being live on the
// stack, which prevents collection of atoms.
//
// ModuleValidator is marked as rooted in the rooting analysis.  Don't add
// non-JSAtom pointers, or this will break!
class MOZ_STACK_CLASS JS_HAZ_ROOTED ModuleValidator
{
  public:
    class Func
    {
        PropertyName* name_;
        uint32_t sigIndex_;
        uint32_t firstUse_;
        uint32_t funcDefIndex_;

        bool defined_;

        // Available when defined:
        uint32_t srcBegin_;
        uint32_t srcEnd_;
        uint32_t line_;
        Bytes bytes_;
        Uint32Vector callSiteLineNums_;

      public:
        Func(PropertyName* name, uint32_t sigIndex, uint32_t firstUse, uint32_t funcDefIndex)
          : name_(name), sigIndex_(sigIndex), firstUse_(firstUse), funcDefIndex_(funcDefIndex),
            defined_(false), srcBegin_(0), srcEnd_(0), line_(0)
        {}

        PropertyName* name() const { return name_; }
        uint32_t sigIndex() const { return sigIndex_; }
        uint32_t firstUse() const { return firstUse_; }
        bool defined() const { return defined_; }
        uint32_t funcDefIndex() const { return funcDefIndex_; }

        void define(ParseNode* fn, uint32_t line, Bytes&& bytes, Uint32Vector&& callSiteLineNums) {
            MOZ_ASSERT(!defined_);
            defined_ = true;
            srcBegin_ = fn->pn_pos.begin;
            srcEnd_ = fn->pn_pos.end;
            line_ = line;
            bytes_ = std::move(bytes);
            callSiteLineNums_ = std::move(callSiteLineNums);
        }

        uint32_t srcBegin() const { MOZ_ASSERT(defined_); return srcBegin_; }
        uint32_t srcEnd() const { MOZ_ASSERT(defined_); return srcEnd_; }
        uint32_t line() const { MOZ_ASSERT(defined_); return line_; }
        const Bytes& bytes() const { MOZ_ASSERT(defined_); return bytes_; }
        Uint32Vector& callSiteLineNums() { MOZ_ASSERT(defined_); return callSiteLineNums_; }
    };

    typedef Vector<const Func*> ConstFuncVector;
    typedef Vector<Func> FuncVector;

    class Table
    {
        uint32_t sigIndex_;
        PropertyName* name_;
        uint32_t firstUse_;
        uint32_t mask_;
        bool defined_;

        Table(Table&& rhs) = delete;

      public:
        Table(uint32_t sigIndex, PropertyName* name, uint32_t firstUse, uint32_t mask)
          : sigIndex_(sigIndex), name_(name), firstUse_(firstUse), mask_(mask), defined_(false)
        {}

        uint32_t sigIndex() const { return sigIndex_; }
        PropertyName* name() const { return name_; }
        uint32_t firstUse() const { return firstUse_; }
        unsigned mask() const { return mask_; }
        bool defined() const { return defined_; }
        void define() { MOZ_ASSERT(!defined_); defined_ = true; }
    };

    typedef Vector<Table*> TableVector;

    class Global
    {
      public:
        enum Which {
            Variable,
            ConstantLiteral,
            ConstantImport,
            Function,
            Table,
            FFI,
            ArrayView,
            ArrayViewCtor,
            MathBuiltinFunction
        };

      private:
        Which which_;
        union U {
            struct VarOrConst {
                Type::Which type_;
                unsigned index_;
                NumLit literalValue_;

                VarOrConst(unsigned index, const NumLit& lit)
                  : type_(Type::lit(lit).which()),
                    index_(index),
                    literalValue_(lit) // copies |lit|
                {}

                VarOrConst(unsigned index, Type::Which which)
                  : type_(which),
                    index_(index)
                {
                    // The |literalValue_| field remains unused and
                    // uninitialized for non-constant variables.
                }

                explicit VarOrConst(double constant)
                  : type_(Type::Double),
                    literalValue_(NumLit::Double, DoubleValue(constant))
                {
                    // The index_ field is unused and uninitialized for
                    // constant doubles.
                }
            } varOrConst;
            uint32_t funcDefIndex_;
            uint32_t tableIndex_;
            uint32_t ffiIndex_;
            Scalar::Type viewType_;
            AsmJSMathBuiltinFunction mathBuiltinFunc_;

            // |varOrConst|, through |varOrConst.literalValue_|, has a
            // non-trivial constructor and therefore MUST be placement-new'd
            // into existence.
            MOZ_PUSH_DISABLE_NONTRIVIAL_UNION_WARNINGS
            U() : funcDefIndex_(0) {}
            MOZ_POP_DISABLE_NONTRIVIAL_UNION_WARNINGS
        } u;

        friend class ModuleValidator;
        friend class js::LifoAlloc;

        explicit Global(Which which) : which_(which) {}

      public:
        Which which() const {
            return which_;
        }
        Type varOrConstType() const {
            MOZ_ASSERT(which_ == Variable || which_ == ConstantLiteral || which_ == ConstantImport);
            return u.varOrConst.type_;
        }
        unsigned varOrConstIndex() const {
            MOZ_ASSERT(which_ == Variable || which_ == ConstantImport);
            return u.varOrConst.index_;
        }
        bool isConst() const {
            return which_ == ConstantLiteral || which_ == ConstantImport;
        }
        NumLit constLiteralValue() const {
            MOZ_ASSERT(which_ == ConstantLiteral);
            return u.varOrConst.literalValue_;
        }
        uint32_t funcDefIndex() const {
            MOZ_ASSERT(which_ == Function);
            return u.funcDefIndex_;
        }
        uint32_t tableIndex() const {
            MOZ_ASSERT(which_ == Table);
            return u.tableIndex_;
        }
        unsigned ffiIndex() const {
            MOZ_ASSERT(which_ == FFI);
            return u.ffiIndex_;
        }
        bool isAnyArrayView() const {
            return which_ == ArrayView || which_ == ArrayViewCtor;
        }
        Scalar::Type viewType() const {
            MOZ_ASSERT(isAnyArrayView());
            return u.viewType_;
        }
        bool isMathFunction() const {
            return which_ == MathBuiltinFunction;
        }
        AsmJSMathBuiltinFunction mathBuiltinFunction() const {
            MOZ_ASSERT(which_ == MathBuiltinFunction);
            return u.mathBuiltinFunc_;
        }
    };

    struct MathBuiltin
    {
        enum Kind { Function, Constant };
        Kind kind;

        union {
            double cst;
            AsmJSMathBuiltinFunction func;
        } u;

        MathBuiltin() : kind(Kind(-1)), u{} {}
        explicit MathBuiltin(double cst) : kind(Constant) {
            u.cst = cst;
        }
        explicit MathBuiltin(AsmJSMathBuiltinFunction func) : kind(Function) {
            u.func = func;
        }
    };

    struct ArrayView
    {
        ArrayView(PropertyName* name, Scalar::Type type)
          : name(name), type(type)
        {}

        PropertyName* name;
        Scalar::Type type;
    };

  private:
    class HashableSig
    {
        uint32_t sigIndex_;
        const TypeDefVector& types_;

      public:
        HashableSig(uint32_t sigIndex, const TypeDefVector& types)
          : sigIndex_(sigIndex), types_(types)
        {}
        uint32_t sigIndex() const {
            return sigIndex_;
        }
        const FuncType& funcType() const {
            return types_[sigIndex_].funcType();
        }

        // Implement HashPolicy:
        typedef const FuncType& Lookup;
        static HashNumber hash(Lookup l) {
            return l.hash();
        }
        static bool match(HashableSig lhs, Lookup rhs) {
            return lhs.funcType() == rhs;
        }
    };

    class NamedSig : public HashableSig
    {
        PropertyName* name_;

      public:
        NamedSig(PropertyName* name, uint32_t sigIndex, const TypeDefVector& types)
          : HashableSig(sigIndex, types), name_(name)
        {}
        PropertyName* name() const {
            return name_;
        }

        // Implement HashPolicy:
        struct Lookup {
            PropertyName* name;
            const FuncType& funcType;
            Lookup(PropertyName* name, const FuncType& funcType) : name(name), funcType(funcType) {}
        };
        static HashNumber hash(Lookup l) {
            return HashGeneric(l.name, l.funcType.hash());
        }
        static bool match(NamedSig lhs, Lookup rhs) {
            return lhs.name() == rhs.name && lhs.funcType() == rhs.funcType;
        }
    };

    typedef HashSet<HashableSig, HashableSig> SigSet;
    typedef HashMap<NamedSig, uint32_t, NamedSig> FuncImportMap;
    typedef HashMap<PropertyName*, Global*> GlobalMap;
    typedef HashMap<PropertyName*, MathBuiltin> MathNameMap;
    typedef Vector<ArrayView> ArrayViewVector;

    JSContext*            cx_;
    AsmJSParser&          parser_;
    CodeNode*             moduleFunctionNode_;
    PropertyName*         moduleFunctionName_;
    PropertyName*         globalArgumentName_;
    PropertyName*         importArgumentName_;
    PropertyName*         bufferArgumentName_;
    MathNameMap           standardLibraryMathNames_;
    RootedFunction        dummyFunction_;

    // Validation-internal state:
    LifoAlloc             validationLifo_;
    FuncVector            funcDefs_;
    TableVector           tables_;
    GlobalMap             globalMap_;
    SigSet                sigSet_;
    FuncImportMap         funcImportMap_;
    ArrayViewVector       arrayViews_;

    // State used to build the AsmJSModule in finish():
    CompilerEnvironment   compilerEnv_;
    ModuleEnvironment     env_;
    MutableAsmJSMetadata  asmJSMetadata_;

    // Error reporting:
    UniqueChars           errorString_;
    uint32_t              errorOffset_;
    bool                  errorOverRecursed_;

    // Helpers:
    bool addStandardLibraryMathName(const char* name, AsmJSMathBuiltinFunction func) {
        JSAtom* atom = Atomize(cx_, name, strlen(name));
        if (!atom) {
            return false;
        }
        MathBuiltin builtin(func);
        return standardLibraryMathNames_.putNew(atom->asPropertyName(), builtin);
    }
    bool addStandardLibraryMathName(const char* name, double cst) {
        JSAtom* atom = Atomize(cx_, name, strlen(name));
        if (!atom) {
            return false;
        }
        MathBuiltin builtin(cst);
        return standardLibraryMathNames_.putNew(atom->asPropertyName(), builtin);
    }
    bool newSig(FuncType&& sig, uint32_t* sigIndex) {
        if (env_.types.length() >= MaxTypes) {
            return failCurrentOffset("too many signatures");
        }

        *sigIndex = env_.types.length();
        return env_.types.append(std::move(sig));
    }
    bool declareSig(FuncType&& sig, uint32_t* sigIndex) {
        SigSet::AddPtr p = sigSet_.lookupForAdd(sig);
        if (p) {
            *sigIndex = p->sigIndex();
            MOZ_ASSERT(env_.types[*sigIndex].funcType() == sig);
            return true;
        }

        return newSig(std::move(sig), sigIndex) &&
               sigSet_.add(p, HashableSig(*sigIndex, env_.types));
    }

  public:
    ModuleValidator(JSContext* cx, AsmJSParser& parser, CodeNode* moduleFunctionNode)
      : cx_(cx),
        parser_(parser),
        moduleFunctionNode_(moduleFunctionNode),
        moduleFunctionName_(FunctionName(moduleFunctionNode)),
        globalArgumentName_(nullptr),
        importArgumentName_(nullptr),
        bufferArgumentName_(nullptr),
        standardLibraryMathNames_(cx),
        dummyFunction_(cx),
        validationLifo_(VALIDATION_LIFO_DEFAULT_CHUNK_SIZE),
        funcDefs_(cx),
        tables_(cx),
        globalMap_(cx),
        sigSet_(cx),
        funcImportMap_(cx),
        arrayViews_(cx),
        compilerEnv_(CompileMode::Once, Tier::Optimized, OptimizedBackend::Ion, DebugEnabled::False,
                     HasGcTypes::False),
        env_(HasGcTypes::False, &compilerEnv_, Shareable::False, ModuleKind::AsmJS),
        errorString_(nullptr),
        errorOffset_(UINT32_MAX),
        errorOverRecursed_(false)
    {
        compilerEnv_.computeParameters(HasGcTypes::False);
        env_.minMemoryLength = RoundUpToNextValidAsmJSHeapLength(0);
    }

    ~ModuleValidator() {
        if (errorString_) {
            MOZ_ASSERT(errorOffset_ != UINT32_MAX);
            typeFailure(errorOffset_, errorString_.get());
        }
        if (errorOverRecursed_) {
            ReportOverRecursed(cx_);
        }
    }

  private:
    void typeFailure(uint32_t offset, ...) {
        va_list args;
        va_start(args, offset);

        auto& ts = tokenStream();
        ErrorMetadata metadata;
        if (ts.computeErrorMetadata(&metadata, offset)) {
            if (ts.anyCharsAccess().options().throwOnAsmJSValidationFailureOption) {
                ReportCompileError(cx_, std::move(metadata), nullptr, JSREPORT_ERROR,
                                   JSMSG_USE_ASM_TYPE_FAIL, args);
            } else {
                // asm.js type failure is indicated by calling one of the fail*
                // functions below.  These functions always return false to
                // halt asm.js parsing.  Whether normal parsing is attempted as
                // fallback, depends whether an exception is also set.
                //
                // If warning succeeds, no exception is set.  If warning fails,
                // an exception is set and execution will halt.  Thus it's safe
                // and correct to ignore the return value here.
                Unused << ts.anyCharsAccess().compileWarning(std::move(metadata), nullptr,
                                                             JSREPORT_WARNING,
                                                             JSMSG_USE_ASM_TYPE_FAIL, args);
            }
        }

        va_end(args);
    }

  public:
    bool init() {
        asmJSMetadata_ = cx_->new_<AsmJSMetadata>();
        if (!asmJSMetadata_) {
            return false;
        }

        asmJSMetadata_->toStringStart = moduleFunctionNode_->funbox()->toStringStart;
        asmJSMetadata_->srcStart = moduleFunctionNode_->body()->pn_pos.begin;
        asmJSMetadata_->strict = parser_.pc->sc()->strict() &&
                                 !parser_.pc->sc()->hasExplicitUseStrict();
        asmJSMetadata_->scriptSource.reset(parser_.ss);

        if (!addStandardLibraryMathName("sin", AsmJSMathBuiltin_sin) ||
            !addStandardLibraryMathName("cos", AsmJSMathBuiltin_cos) ||
            !addStandardLibraryMathName("tan", AsmJSMathBuiltin_tan) ||
            !addStandardLibraryMathName("asin", AsmJSMathBuiltin_asin) ||
            !addStandardLibraryMathName("acos", AsmJSMathBuiltin_acos) ||
            !addStandardLibraryMathName("atan", AsmJSMathBuiltin_atan) ||
            !addStandardLibraryMathName("ceil", AsmJSMathBuiltin_ceil) ||
            !addStandardLibraryMathName("floor", AsmJSMathBuiltin_floor) ||
            !addStandardLibraryMathName("exp", AsmJSMathBuiltin_exp) ||
            !addStandardLibraryMathName("log", AsmJSMathBuiltin_log) ||
            !addStandardLibraryMathName("pow", AsmJSMathBuiltin_pow) ||
            !addStandardLibraryMathName("sqrt", AsmJSMathBuiltin_sqrt) ||
            !addStandardLibraryMathName("abs", AsmJSMathBuiltin_abs) ||
            !addStandardLibraryMathName("atan2", AsmJSMathBuiltin_atan2) ||
            !addStandardLibraryMathName("imul", AsmJSMathBuiltin_imul) ||
            !addStandardLibraryMathName("clz32", AsmJSMathBuiltin_clz32) ||
            !addStandardLibraryMathName("fround", AsmJSMathBuiltin_fround) ||
            !addStandardLibraryMathName("min", AsmJSMathBuiltin_min) ||
            !addStandardLibraryMathName("max", AsmJSMathBuiltin_max) ||
            !addStandardLibraryMathName("E", M_E) ||
            !addStandardLibraryMathName("LN10", M_LN10) ||
            !addStandardLibraryMathName("LN2", M_LN2) ||
            !addStandardLibraryMathName("LOG2E", M_LOG2E) ||
            !addStandardLibraryMathName("LOG10E", M_LOG10E) ||
            !addStandardLibraryMathName("PI", M_PI) ||
            !addStandardLibraryMathName("SQRT1_2", M_SQRT1_2) ||
            !addStandardLibraryMathName("SQRT2", M_SQRT2))
        {
            return false;
        }

        // This flows into FunctionBox, so must be tenured.
        dummyFunction_ = NewScriptedFunction(cx_, 0, JSFunction::INTERPRETED, nullptr,
                                             /* proto = */ nullptr, gc::AllocKind::FUNCTION,
                                             TenuredObject);
        if (!dummyFunction_) {
            return false;
        }

        return true;
    }

    JSContext* cx() const                    { return cx_; }
    PropertyName* moduleFunctionName() const { return moduleFunctionName_; }
    PropertyName* globalArgumentName() const { return globalArgumentName_; }
    PropertyName* importArgumentName() const { return importArgumentName_; }
    PropertyName* bufferArgumentName() const { return bufferArgumentName_; }
    const ModuleEnvironment& env()           { return env_; }

    AsmJSParser& parser() const { return parser_; }

    auto tokenStream() const
      -> decltype(parser_.tokenStream)&
    {
        return parser_.tokenStream;
    }

    RootedFunction& dummyFunction()          { return dummyFunction_; }
    uint32_t minMemoryLength() const         { return env_.minMemoryLength; }

    void initModuleFunctionName(PropertyName* name) {
        MOZ_ASSERT(!moduleFunctionName_);
        moduleFunctionName_ = name;
    }
    MOZ_MUST_USE bool initGlobalArgumentName(PropertyName* n) {
        globalArgumentName_ = n;
        if (n) {
            MOZ_ASSERT(n->isTenured());
            asmJSMetadata_->globalArgumentName = StringToNewUTF8CharsZ(cx_, *n);
            if (!asmJSMetadata_->globalArgumentName) {
                return false;
            }
        }
        return true;
    }
    MOZ_MUST_USE bool initImportArgumentName(PropertyName* n) {
        importArgumentName_ = n;
        if (n) {
            MOZ_ASSERT(n->isTenured());
            asmJSMetadata_->importArgumentName = StringToNewUTF8CharsZ(cx_, *n);
            if (!asmJSMetadata_->importArgumentName) {
                return false;
            }
        }
        return true;
    }
    MOZ_MUST_USE bool initBufferArgumentName(PropertyName* n) {
        bufferArgumentName_ = n;
        if (n) {
            MOZ_ASSERT(n->isTenured());
            asmJSMetadata_->bufferArgumentName = StringToNewUTF8CharsZ(cx_, *n);
            if (!asmJSMetadata_->bufferArgumentName) {
                return false;
            }
        }
        return true;
    }
    bool addGlobalVarInit(PropertyName* var, const NumLit& lit, Type type, bool isConst) {
        MOZ_ASSERT(type.isGlobalVarType());
        MOZ_ASSERT(type == Type::canonicalize(Type::lit(lit)));

        uint32_t index = env_.globals.length();
        if (!env_.globals.emplaceBack(type.canonicalToValType(), !isConst, index,
                                      ModuleKind::AsmJS))
        {
            return false;
        }

        Global::Which which = isConst ? Global::ConstantLiteral : Global::Variable;
        Global* global = validationLifo_.new_<Global>(which);
        if (!global) {
            return false;
        }
        if (isConst) {
            new (&global->u.varOrConst) Global::U::VarOrConst(index, lit);
        } else {
            new (&global->u.varOrConst) Global::U::VarOrConst(index, type.which());
        }
        if (!globalMap_.putNew(var, global)) {
            return false;
        }

        AsmJSGlobal g(AsmJSGlobal::Variable, nullptr);
        g.pod.u.var.initKind_ = AsmJSGlobal::InitConstant;
        g.pod.u.var.u.val_ = lit.value();
        return asmJSMetadata_->asmJSGlobals.append(std::move(g));
    }
    bool addGlobalVarImport(PropertyName* var, PropertyName* field, Type type, bool isConst) {
        MOZ_ASSERT(type.isGlobalVarType());

        UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
        if (!fieldChars) {
            return false;
        }

        uint32_t index = env_.globals.length();
        ValType valType = type.canonicalToValType();
        if (!env_.globals.emplaceBack(valType, !isConst, index, ModuleKind::AsmJS)) {
            return false;
        }

        Global::Which which = isConst ? Global::ConstantImport : Global::Variable;
        Global* global = validationLifo_.new_<Global>(which);
        if (!global) {
            return false;
        }
        new (&global->u.varOrConst) Global::U::VarOrConst(index, type.which());
        if (!globalMap_.putNew(var, global)) {
            return false;
        }

        AsmJSGlobal g(AsmJSGlobal::Variable, std::move(fieldChars));
        g.pod.u.var.initKind_ = AsmJSGlobal::InitImport;
        g.pod.u.var.u.importValType_ = valType.packed();
        return asmJSMetadata_->asmJSGlobals.append(std::move(g));
    }
    bool addArrayView(PropertyName* var, Scalar::Type vt, PropertyName* maybeField) {
        UniqueChars fieldChars;
        if (maybeField) {
            fieldChars = StringToNewUTF8CharsZ(cx_, *maybeField);
            if (!fieldChars) {
                return false;
            }
        }

        if (!arrayViews_.append(ArrayView(var, vt))) {
            return false;
        }

        Global* global = validationLifo_.new_<Global>(Global::ArrayView);
        if (!global) {
            return false;
        }
        new (&global->u.viewType_) Scalar::Type(vt);
        if (!globalMap_.putNew(var, global)) {
            return false;
        }

        AsmJSGlobal g(AsmJSGlobal::ArrayView, std::move(fieldChars));
        g.pod.u.viewType_ = vt;
        return asmJSMetadata_->asmJSGlobals.append(std::move(g));
    }
    bool addMathBuiltinFunction(PropertyName* var, AsmJSMathBuiltinFunction func,
                                PropertyName* field)
    {
        UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
        if (!fieldChars) {
            return false;
        }

        Global* global = validationLifo_.new_<Global>(Global::MathBuiltinFunction);
        if (!global) {
            return false;
        }
        new (&global->u.mathBuiltinFunc_) AsmJSMathBuiltinFunction(func);
        if (!globalMap_.putNew(var, global)) {
            return false;
        }

        AsmJSGlobal g(AsmJSGlobal::MathBuiltinFunction, std::move(fieldChars));
        g.pod.u.mathBuiltinFunc_ = func;
        return asmJSMetadata_->asmJSGlobals.append(std::move(g));
    }
  private:
    bool addGlobalDoubleConstant(PropertyName* var, double constant) {
        Global* global = validationLifo_.new_<Global>(Global::ConstantLiteral);
        if (!global) {
            return false;
        }
        new (&global->u.varOrConst) Global::U::VarOrConst(constant);
        return globalMap_.putNew(var, global);
    }
  public:
    bool addMathBuiltinConstant(PropertyName* var, double constant, PropertyName* field) {
        UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
        if (!fieldChars) {
            return false;
        }

        if (!addGlobalDoubleConstant(var, constant)) {
            return false;
        }

        AsmJSGlobal g(AsmJSGlobal::Constant, std::move(fieldChars));
        g.pod.u.constant.value_ = constant;
        g.pod.u.constant.kind_ = AsmJSGlobal::MathConstant;
        return asmJSMetadata_->asmJSGlobals.append(std::move(g));
    }
    bool addGlobalConstant(PropertyName* var, double constant, PropertyName* field) {
        UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
        if (!fieldChars) {
            return false;
        }

        if (!addGlobalDoubleConstant(var, constant)) {
            return false;
        }

        AsmJSGlobal g(AsmJSGlobal::Constant, std::move(fieldChars));
        g.pod.u.constant.value_ = constant;
        g.pod.u.constant.kind_ = AsmJSGlobal::GlobalConstant;
        return asmJSMetadata_->asmJSGlobals.append(std::move(g));
    }
    bool addArrayViewCtor(PropertyName* var, Scalar::Type vt, PropertyName* field) {
        UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
        if (!fieldChars) {
            return false;
        }

        Global* global = validationLifo_.new_<Global>(Global::ArrayViewCtor);
        if (!global) {
            return false;
        }
        new (&global->u.viewType_) Scalar::Type(vt);
        if (!globalMap_.putNew(var, global)) {
            return false;
        }

        AsmJSGlobal g(AsmJSGlobal::ArrayViewCtor, std::move(fieldChars));
        g.pod.u.viewType_ = vt;
        return asmJSMetadata_->asmJSGlobals.append(std::move(g));
    }
    bool addFFI(PropertyName* var, PropertyName* field) {
        UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
        if (!fieldChars) {
            return false;
        }

        if (asmJSMetadata_->numFFIs == UINT32_MAX) {
            return false;
        }
        uint32_t ffiIndex = asmJSMetadata_->numFFIs++;

        Global* global = validationLifo_.new_<Global>(Global::FFI);
        if (!global) {
            return false;
        }
        new (&global->u.ffiIndex_) uint32_t(ffiIndex);
        if (!globalMap_.putNew(var, global)) {
            return false;
        }

        AsmJSGlobal g(AsmJSGlobal::FFI, std::move(fieldChars));
        g.pod.u.ffiIndex_ = ffiIndex;
        return asmJSMetadata_->asmJSGlobals.append(std::move(g));
    }
    bool addExportField(const Func& func, PropertyName* maybeField) {
        // Record the field name of this export.
        CacheableChars fieldChars;
        if (maybeField) {
            fieldChars = StringToNewUTF8CharsZ(cx_, *maybeField);
        } else {
            fieldChars = DuplicateString("");
        }
        if (!fieldChars) {
            return false;
        }

        // Declare which function is exported which gives us an index into the
        // module ExportVector.
        uint32_t funcIndex = funcImportMap_.count() + func.funcDefIndex();
        if (!env_.exports.emplaceBack(std::move(fieldChars), funcIndex, DefinitionKind::Function)) {
            return false;
        }

        // The exported function might have already been exported in which case
        // the index will refer into the range of AsmJSExports.
        return asmJSMetadata_->asmJSExports.emplaceBack(funcIndex,
                                                        func.srcBegin() - asmJSMetadata_->srcStart,
                                                        func.srcEnd() - asmJSMetadata_->srcStart);
    }
    bool addFuncDef(PropertyName* name, uint32_t firstUse, FuncType&& sig, Func** func) {
        uint32_t sigIndex;
        if (!declareSig(std::move(sig), &sigIndex)) {
            return false;
        }

        uint32_t funcDefIndex = funcDefs_.length();
        if (funcDefIndex >= MaxFuncs) {
            return failCurrentOffset("too many functions");
        }

        Global* global = validationLifo_.new_<Global>(Global::Function);
        if (!global) {
            return false;
        }
        new (&global->u.funcDefIndex_) uint32_t(funcDefIndex);
        if (!globalMap_.putNew(name, global)) {
            return false;
        }
        if (!funcDefs_.emplaceBack(name, sigIndex, firstUse, funcDefIndex)) {
            return false;
        }
        *func = &funcDefs_.back();
        return true;
    }
    bool declareFuncPtrTable(FuncType&& sig, PropertyName* name, uint32_t firstUse, uint32_t mask,
                             uint32_t* tableIndex)
    {
        if (mask > MaxTableInitialLength) {
            return failCurrentOffset("function pointer table too big");
        }

        MOZ_ASSERT(env_.tables.length() == tables_.length());
        *tableIndex = env_.tables.length();

        uint32_t sigIndex;
        if (!newSig(std::move(sig), &sigIndex)) {
            return false;
        }

        MOZ_ASSERT(sigIndex >= env_.asmJSSigToTableIndex.length());
        if (!env_.asmJSSigToTableIndex.resize(sigIndex + 1)) {
            return false;
        }

        env_.asmJSSigToTableIndex[sigIndex] = env_.tables.length();
        if (!env_.tables.emplaceBack(TableKind::TypedFunction, Limits(mask + 1))) {
            return false;
        }

        Global* global = validationLifo_.new_<Global>(Global::Table);
        if (!global) {
            return false;
        }

        new (&global->u.tableIndex_) uint32_t(*tableIndex);
        if (!globalMap_.putNew(name, global)) {
            return false;
        }

        Table* t = validationLifo_.new_<Table>(sigIndex, name, firstUse, mask);
        return t && tables_.append(t);
    }
    bool defineFuncPtrTable(uint32_t tableIndex, Uint32Vector&& elems) {
        Table& table = *tables_[tableIndex];
        if (table.defined()) {
            return false;
        }

        table.define();

        for (uint32_t& index : elems) {
            index += funcImportMap_.count();
        }

        MutableElemSegment seg = js_new<ElemSegment>();
        if (!seg) {
            return false;
        }
        seg->tableIndex = tableIndex;
        seg->offsetIfActive = Some(InitExpr(LitVal(uint32_t(0))));
        seg->elemFuncIndices = std::move(elems);
        return env_.elemSegments.append(std::move(seg));
    }
    bool declareImport(PropertyName* name, FuncType&& sig, unsigned ffiIndex, uint32_t* importIndex) {
        FuncImportMap::AddPtr p = funcImportMap_.lookupForAdd(NamedSig::Lookup(name, sig));
        if (p) {
            *importIndex = p->value();
            return true;
        }

        *importIndex = funcImportMap_.count();
        MOZ_ASSERT(*importIndex == asmJSMetadata_->asmJSImports.length());

        if (*importIndex >= MaxImports) {
            return failCurrentOffset("too many imports");
        }

        if (!asmJSMetadata_->asmJSImports.emplaceBack(ffiIndex)) {
            return false;
        }

        uint32_t sigIndex;
        if (!declareSig(std::move(sig), &sigIndex)) {
            return false;
        }

        return funcImportMap_.add(p, NamedSig(name, sigIndex, env_.types), *importIndex);
    }

    bool tryConstantAccess(uint64_t start, uint64_t width) {
        MOZ_ASSERT(UINT64_MAX - start > width);
        uint64_t len = start + width;
        if (len > uint64_t(INT32_MAX) + 1) {
            return false;
        }
        len = RoundUpToNextValidAsmJSHeapLength(len);
        if (len > env_.minMemoryLength) {
            env_.minMemoryLength = len;
        }
        return true;
    }

    // Error handling.
    bool hasAlreadyFailed() const {
        return !!errorString_;
    }

    bool failOffset(uint32_t offset, const char* str) {
        MOZ_ASSERT(!hasAlreadyFailed());
        MOZ_ASSERT(errorOffset_ == UINT32_MAX);
        MOZ_ASSERT(str);
        errorOffset_ = offset;
        errorString_ = DuplicateString(str);
        return false;
    }

    bool failCurrentOffset(const char* str) {
        return failOffset(tokenStream().anyCharsAccess().currentToken().pos.begin, str);
    }

    bool fail(ParseNode* pn, const char* str) {
        return failOffset(pn->pn_pos.begin, str);
    }

    bool failfVAOffset(uint32_t offset, const char* fmt, va_list ap) MOZ_FORMAT_PRINTF(3, 0) {
        MOZ_ASSERT(!hasAlreadyFailed());
        MOZ_ASSERT(errorOffset_ == UINT32_MAX);
        MOZ_ASSERT(fmt);
        errorOffset_ = offset;
        errorString_ = JS_vsmprintf(fmt, ap);
        return false;
    }

    bool failfOffset(uint32_t offset, const char* fmt, ...) MOZ_FORMAT_PRINTF(3, 4) {
        va_list ap;
        va_start(ap, fmt);
        failfVAOffset(offset, fmt, ap);
        va_end(ap);
        return false;
    }

    bool failf(ParseNode* pn, const char* fmt, ...) MOZ_FORMAT_PRINTF(3, 4) {
        va_list ap;
        va_start(ap, fmt);
        failfVAOffset(pn->pn_pos.begin, fmt, ap);
        va_end(ap);
        return false;
    }

    bool failNameOffset(uint32_t offset, const char* fmt, PropertyName* name) {
        // This function is invoked without the caller properly rooting its locals.
        gc::AutoSuppressGC suppress(cx_);
        if (UniqueChars bytes = AtomToPrintableString(cx_, name)) {
            failfOffset(offset, fmt, bytes.get());
        }
        return false;
    }

    bool failName(ParseNode* pn, const char* fmt, PropertyName* name) {
        return failNameOffset(pn->pn_pos.begin, fmt, name);
    }

    bool failOverRecursed() {
        errorOverRecursed_ = true;
        return false;
    }

    unsigned numArrayViews() const {
        return arrayViews_.length();
    }
    const ArrayView& arrayView(unsigned i) const {
        return arrayViews_[i];
    }
    unsigned numFuncDefs() const {
        return funcDefs_.length();
    }
    const Func& funcDef(unsigned i) const {
        return funcDefs_[i];
    }
    unsigned numFuncPtrTables() const {
        return tables_.length();
    }
    Table& table(unsigned i) const {
        return *tables_[i];
    }

    const Global* lookupGlobal(PropertyName* name) const {
        if (GlobalMap::Ptr p = globalMap_.lookup(name)) {
            return p->value();
        }
        return nullptr;
    }

    Func* lookupFuncDef(PropertyName* name) {
        if (GlobalMap::Ptr p = globalMap_.lookup(name)) {
            Global* value = p->value();
            if (value->which() == Global::Function) {
                return &funcDefs_[value->funcDefIndex()];
            }
        }
        return nullptr;
    }

    bool lookupStandardLibraryMathName(PropertyName* name, MathBuiltin* mathBuiltin) const {
        if (MathNameMap::Ptr p = standardLibraryMathNames_.lookup(name)) {
            *mathBuiltin = p->value();
            return true;
        }
        return false;
    }

    bool startFunctionBodies() {
        if (!arrayViews_.empty()) {
            env_.memoryUsage = MemoryUsage::Unshared;
        } else {
            env_.memoryUsage = MemoryUsage::None;
        }
        return true;
    }
    SharedModule finish(UniqueLinkData* linkData) {
        MOZ_ASSERT(env_.funcTypes.empty());
        if (!env_.funcTypes.resize(funcImportMap_.count() + funcDefs_.length())) {
            return nullptr;
        }
        for (FuncImportMap::Range r = funcImportMap_.all(); !r.empty(); r.popFront()) {
            uint32_t funcIndex = r.front().value();
            MOZ_ASSERT(!env_.funcTypes[funcIndex]);
            env_.funcTypes[funcIndex] = &env_.types[r.front().key().sigIndex()].funcType();
        }
        for (const Func& func : funcDefs_) {
            uint32_t funcIndex = funcImportMap_.count() + func.funcDefIndex();
            MOZ_ASSERT(!env_.funcTypes[funcIndex]);
            env_.funcTypes[funcIndex] = &env_.types[func.sigIndex()].funcType();
        }

        if (!env_.funcImportGlobalDataOffsets.resize(funcImportMap_.count())) {
            return nullptr;
        }

        MOZ_ASSERT(asmJSMetadata_->asmJSFuncNames.empty());
        if (!asmJSMetadata_->asmJSFuncNames.resize(funcImportMap_.count())) {
            return nullptr;
        }
        for (const Func& func : funcDefs_) {
            CacheableChars funcName = StringToNewUTF8CharsZ(cx_, *func.name());
            if (!funcName || !asmJSMetadata_->asmJSFuncNames.emplaceBack(std::move(funcName))) {
                return nullptr;
            }
        }

        uint32_t endBeforeCurly = tokenStream().anyCharsAccess().currentToken().pos.end;
        asmJSMetadata_->srcLength = endBeforeCurly - asmJSMetadata_->srcStart;

        TokenPos pos;
        MOZ_ALWAYS_TRUE(tokenStream().peekTokenPos(&pos, TokenStreamShared::Operand));
        uint32_t endAfterCurly = pos.end;
        asmJSMetadata_->srcLengthWithRightBrace = endAfterCurly - asmJSMetadata_->srcStart;

        ScriptedCaller scriptedCaller;
        if (parser_.ss->filename()) {
            scriptedCaller.line = 0;  // unused
            scriptedCaller.filename = DuplicateString(parser_.ss->filename());
            if (!scriptedCaller.filename) {
                return nullptr;
            }
        }

        MutableCompileArgs args = cx_->new_<CompileArgs>(cx_, std::move(scriptedCaller));
        if (!args) {
            return nullptr;
        }

        uint32_t codeSectionSize = 0;
        for (const Func& func : funcDefs_) {
            codeSectionSize += func.bytes().length();
        }

        env_.codeSection.emplace();
        env_.codeSection->start = 0;
        env_.codeSection->size = codeSectionSize;

        // asm.js does not have any wasm bytecode to save; view-source is
        // provided through the ScriptSource.
        SharedBytes bytes = cx_->new_<ShareableBytes>();
        if (!bytes) {
            return nullptr;
        }

        ModuleGenerator mg(*args, &env_, nullptr, nullptr);
        if (!mg.init(asmJSMetadata_.get())) {
            return nullptr;
        }

        for (Func& func : funcDefs_) {
            if (!mg.compileFuncDef(funcImportMap_.count() + func.funcDefIndex(), func.line(),
                                   func.bytes().begin(), func.bytes().end(),
                                   std::move(func.callSiteLineNums()))) {
                return nullptr;
            }
        }

        if (!mg.finishFuncDefs()) {
            return nullptr;
        }

        return mg.finishModule(*bytes, nullptr, linkData);
    }
};

/*****************************************************************************/
// Numeric literal utilities

static bool
IsNumericNonFloatLiteral(ParseNode* pn)
{
    // Note: '-' is never rolled into the number; numbers are always positive
    // and negations must be applied manually.
    return pn->isKind(ParseNodeKind::Number) ||
           (pn->isKind(ParseNodeKind::Neg) && UnaryKid(pn)->isKind(ParseNodeKind::Number));
}

static bool
IsCallToGlobal(ModuleValidator& m, ParseNode* pn, const ModuleValidator::Global** global)
{
    if (!pn->isKind(ParseNodeKind::Call)) {
        return false;
    }

    ParseNode* callee = CallCallee(pn);
    if (!callee->isKind(ParseNodeKind::Name)) {
        return false;
    }

    *global = m.lookupGlobal(callee->as<NameNode>().name());
    return !!*global;
}

static bool
IsCoercionCall(ModuleValidator& m, ParseNode* pn, Type* coerceTo, ParseNode** coercedExpr)
{
    const ModuleValidator::Global* global;
    if (!IsCallToGlobal(m, pn, &global)) {
        return false;
    }

    if (CallArgListLength(pn) != 1) {
        return false;
    }

    if (coercedExpr) {
        *coercedExpr = CallArgList(pn);
    }

    if (global->isMathFunction() && global->mathBuiltinFunction() == AsmJSMathBuiltin_fround) {
        *coerceTo = Type::Float;
        return true;
    }

    return false;
}

static bool
IsFloatLiteral(ModuleValidator& m, ParseNode* pn)
{
    ParseNode* coercedExpr;
    Type coerceTo;
    if (!IsCoercionCall(m, pn, &coerceTo, &coercedExpr)) {
        return false;
    }
    // Don't fold into || to avoid clang/memcheck bug (bug 1077031).
    if (!coerceTo.isFloat()) {
        return false;
    }
    return IsNumericNonFloatLiteral(coercedExpr);
}

static bool
IsNumericLiteral(ModuleValidator& m, ParseNode* pn);

static NumLit
ExtractNumericLiteral(ModuleValidator& m, ParseNode* pn);

static inline bool
IsLiteralInt(ModuleValidator& m, ParseNode* pn, uint32_t* u32);

static bool
IsNumericLiteral(ModuleValidator& m, ParseNode* pn)
{
    return IsNumericNonFloatLiteral(pn) || IsFloatLiteral(m, pn);
}

// The JS grammar treats -42 as -(42) (i.e., with separate grammar
// productions) for the unary - and literal 42). However, the asm.js spec
// recognizes -42 (modulo parens, so -(42) and -((42))) as a single literal
// so fold the two potential parse nodes into a single double value.
static double
ExtractNumericNonFloatValue(ParseNode* pn, ParseNode** out = nullptr)
{
    MOZ_ASSERT(IsNumericNonFloatLiteral(pn));

    if (pn->isKind(ParseNodeKind::Neg)) {
        pn = UnaryKid(pn);
        if (out) {
            *out = pn;
        }
        return -NumberNodeValue(pn);
    }

    return NumberNodeValue(pn);
}

static NumLit
ExtractNumericLiteral(ModuleValidator& m, ParseNode* pn)
{
    MOZ_ASSERT(IsNumericLiteral(m, pn));

    if (pn->isKind(ParseNodeKind::Call)) {
        // Float literals are explicitly coerced and thus the coerced literal may be
        // any valid (non-float) numeric literal.
        MOZ_ASSERT(CallArgListLength(pn) == 1);
        pn = CallArgList(pn);
        double d = ExtractNumericNonFloatValue(pn);
        return NumLit(NumLit::Float, DoubleValue(d));
    }

    double d = ExtractNumericNonFloatValue(pn, &pn);

    // The asm.js spec syntactically distinguishes any literal containing a
    // decimal point or the literal -0 as having double type.
    if (NumberNodeHasFrac(pn) || IsNegativeZero(d)) {
        return NumLit(NumLit::Double, DoubleValue(d));
    }

    // The syntactic checks above rule out these double values.
    MOZ_ASSERT(!IsNegativeZero(d));
    MOZ_ASSERT(!IsNaN(d));

    // Although doubles can only *precisely* represent 53-bit integers, they
    // can *imprecisely* represent integers much bigger than an int64_t.
    // Furthermore, d may be inf or -inf. In both cases, casting to an int64_t
    // is undefined, so test against the integer bounds using doubles.
    if (d < double(INT32_MIN) || d > double(UINT32_MAX)) {
        return NumLit(NumLit::OutOfRangeInt, UndefinedValue());
    }

    // With the above syntactic and range limitations, d is definitely an
    // integer in the range [INT32_MIN, UINT32_MAX] range.
    int64_t i64 = int64_t(d);
    if (i64 >= 0) {
        if (i64 <= INT32_MAX) {
            return NumLit(NumLit::Fixnum, Int32Value(i64));
        }
        MOZ_ASSERT(i64 <= UINT32_MAX);
        return NumLit(NumLit::BigUnsigned, Int32Value(uint32_t(i64)));
    }
    MOZ_ASSERT(i64 >= INT32_MIN);
    return NumLit(NumLit::NegativeInt, Int32Value(i64));
}

static inline bool
IsLiteralInt(const NumLit& lit, uint32_t* u32)
{
    switch (lit.which()) {
      case NumLit::Fixnum:
      case NumLit::BigUnsigned:
      case NumLit::NegativeInt:
        *u32 = lit.toUint32();
        return true;
      case NumLit::Double:
      case NumLit::Float:
      case NumLit::OutOfRangeInt:
        return false;
    }
    MOZ_MAKE_COMPILER_ASSUME_IS_UNREACHABLE("Bad literal type");
}

static inline bool
IsLiteralInt(ModuleValidator& m, ParseNode* pn, uint32_t* u32)
{
    return IsNumericLiteral(m, pn) &&
           IsLiteralInt(ExtractNumericLiteral(m, pn), u32);
}

/*****************************************************************************/

namespace {

typedef Vector<PropertyName*, 4, SystemAllocPolicy> LabelVector;

// Encapsulates the building of an asm bytecode function from an asm.js function
// source code, packing the asm.js code into the asm bytecode form that can
// be decoded and compiled with a FunctionCompiler.
class MOZ_STACK_CLASS FunctionValidator
{
  public:
    struct Local
    {
        Type type;
        unsigned slot;
        Local(Type t, unsigned slot) : type(t), slot(slot) {
            MOZ_ASSERT(type.isCanonicalValType());
        }
    };

  private:
    typedef HashMap<PropertyName*, Local> LocalMap;
    typedef HashMap<PropertyName*, uint32_t> LabelMap;

    ModuleValidator&  m_;
    ParseNode*        fn_;
    Bytes             bytes_;
    Encoder           encoder_;
    Uint32Vector      callSiteLineNums_;
    LocalMap          locals_;

    // Labels
    LabelMap          breakLabels_;
    LabelMap          continueLabels_;
    Uint32Vector      breakableStack_;
    Uint32Vector      continuableStack_;
    uint32_t          blockDepth_;

    bool              hasAlreadyReturned_;
    ExprType          ret_;

  public:
    FunctionValidator(ModuleValidator& m, ParseNode* fn)
      : m_(m),
        fn_(fn),
        encoder_(bytes_),
        locals_(m.cx()),
        breakLabels_(m.cx()),
        continueLabels_(m.cx()),
        blockDepth_(0),
        hasAlreadyReturned_(false),
        ret_(ExprType::Limit)
    {}

    ModuleValidator& m() const        { return m_; }
    JSContext* cx() const             { return m_.cx(); }
    ParseNode* fn() const             { return fn_; }

    void define(ModuleValidator::Func* func, unsigned line) {
        MOZ_ASSERT(!blockDepth_);
        MOZ_ASSERT(breakableStack_.empty());
        MOZ_ASSERT(continuableStack_.empty());
        MOZ_ASSERT(breakLabels_.empty());
        MOZ_ASSERT(continueLabels_.empty());
        func->define(fn_, line, std::move(bytes_), std::move(callSiteLineNums_));
    }

    bool fail(ParseNode* pn, const char* str) {
        return m_.fail(pn, str);
    }

    bool failf(ParseNode* pn, const char* fmt, ...) MOZ_FORMAT_PRINTF(3, 4) {
        va_list ap;
        va_start(ap, fmt);
        m_.failfVAOffset(pn->pn_pos.begin, fmt, ap);
        va_end(ap);
        return false;
    }

    bool failName(ParseNode* pn, const char* fmt, PropertyName* name) {
        return m_.failName(pn, fmt, name);
    }

    /***************************************************** Local scope setup */

    bool addLocal(ParseNode* pn, PropertyName* name, Type type) {
        LocalMap::AddPtr p = locals_.lookupForAdd(name);
        if (p) {
            return failName(pn, "duplicate local name '%s' not allowed", name);
        }
        return locals_.add(p, name, Local(type, locals_.count()));
    }

    /****************************** For consistency of returns in a function */

    bool hasAlreadyReturned() const {
        return hasAlreadyReturned_;
    }

    ExprType returnedType() const {
        return ret_;
    }

    void setReturnedType(ExprType ret) {
        ret_ = ret;
        hasAlreadyReturned_ = true;
    }

    /**************************************************************** Labels */
  private:
    bool writeBr(uint32_t absolute, Op op = Op::Br) {
        MOZ_ASSERT(op == Op::Br || op == Op::BrIf);
        MOZ_ASSERT(absolute < blockDepth_);
        return encoder().writeOp(op) &&
               encoder().writeVarU32(blockDepth_ - 1 - absolute);
    }
    void removeLabel(PropertyName* label, LabelMap* map) {
        LabelMap::Ptr p = map->lookup(label);
        MOZ_ASSERT(p);
        map->remove(p);
    }

  public:
    bool pushBreakableBlock() {
        return encoder().writeOp(Op::Block) &&
               encoder().writeFixedU8(uint8_t(ExprType::Void)) &&
               breakableStack_.append(blockDepth_++);
    }
    bool popBreakableBlock() {
        MOZ_ALWAYS_TRUE(breakableStack_.popCopy() == --blockDepth_);
        return encoder().writeOp(Op::End);
    }

    bool pushUnbreakableBlock(const LabelVector* labels = nullptr) {
        if (labels) {
            for (PropertyName* label : *labels) {
                if (!breakLabels_.putNew(label, blockDepth_)) {
                    return false;
                }
            }
        }
        blockDepth_++;
        return encoder().writeOp(Op::Block) &&
               encoder().writeFixedU8(uint8_t(ExprType::Void));
    }
    bool popUnbreakableBlock(const LabelVector* labels = nullptr) {
        if (labels) {
            for (PropertyName* label : *labels) {
                removeLabel(label, &breakLabels_);
            }
        }
        --blockDepth_;
        return encoder().writeOp(Op::End);
    }

    bool pushContinuableBlock() {
        return encoder().writeOp(Op::Block) &&
               encoder().writeFixedU8(uint8_t(ExprType::Void)) &&
               continuableStack_.append(blockDepth_++);
    }
    bool popContinuableBlock() {
        MOZ_ALWAYS_TRUE(continuableStack_.popCopy() == --blockDepth_);
        return encoder().writeOp(Op::End);
    }

    bool pushLoop() {
        return encoder().writeOp(Op::Block) &&
               encoder().writeFixedU8(uint8_t(ExprType::Void)) &&
               encoder().writeOp(Op::Loop) &&
               encoder().writeFixedU8(uint8_t(ExprType::Void)) &&
               breakableStack_.append(blockDepth_++) &&
               continuableStack_.append(blockDepth_++);
    }
    bool popLoop() {
        MOZ_ALWAYS_TRUE(continuableStack_.popCopy() == --blockDepth_);
        MOZ_ALWAYS_TRUE(breakableStack_.popCopy() == --blockDepth_);
        return encoder().writeOp(Op::End) &&
               encoder().writeOp(Op::End);
    }

    bool pushIf(size_t* typeAt) {
        ++blockDepth_;
        return encoder().writeOp(Op::If) &&
               encoder().writePatchableFixedU7(typeAt);
    }
    bool switchToElse() {
        MOZ_ASSERT(blockDepth_ > 0);
        return encoder().writeOp(Op::Else);
    }
    void setIfType(size_t typeAt, ExprType type) {
        encoder().patchFixedU7(typeAt, uint8_t(type.code()));
    }
    bool popIf() {
        MOZ_ASSERT(blockDepth_ > 0);
        --blockDepth_;
        return encoder().writeOp(Op::End);
    }
    bool popIf(size_t typeAt, ExprType type) {
        MOZ_ASSERT(blockDepth_ > 0);
        --blockDepth_;
        if (!encoder().writeOp(Op::End)) {
            return false;
        }

        setIfType(typeAt, type);
        return true;
    }

    bool writeBreakIf() {
        return writeBr(breakableStack_.back(), Op::BrIf);
    }
    bool writeContinueIf() {
        return writeBr(continuableStack_.back(), Op::BrIf);
    }
    bool writeUnlabeledBreakOrContinue(bool isBreak) {
        return writeBr(isBreak? breakableStack_.back() : continuableStack_.back());
    }
    bool writeContinue() {
        return writeBr(continuableStack_.back());
    }

    bool addLabels(const LabelVector& labels, uint32_t relativeBreakDepth,
                   uint32_t relativeContinueDepth)
    {
        for (PropertyName* label : labels) {
            if (!breakLabels_.putNew(label, blockDepth_ + relativeBreakDepth)) {
                return false;
            }
            if (!continueLabels_.putNew(label, blockDepth_ + relativeContinueDepth)) {
                return false;
            }
        }
        return true;
    }
    void removeLabels(const LabelVector& labels) {
        for (PropertyName* label : labels) {
            removeLabel(label, &breakLabels_);
            removeLabel(label, &continueLabels_);
        }
    }
    bool writeLabeledBreakOrContinue(PropertyName* label, bool isBreak) {
        LabelMap& map = isBreak ? breakLabels_ : continueLabels_;
        if (LabelMap::Ptr p = map.lookup(label)) {
            return writeBr(p->value());
        }
        MOZ_CRASH("nonexistent label");
    }

    /*************************************************** Read-only interface */

    const Local* lookupLocal(PropertyName* name) const {
        if (auto p = locals_.lookup(name)) {
            return &p->value();
        }
        return nullptr;
    }

    const ModuleValidator::Global* lookupGlobal(PropertyName* name) const {
        if (locals_.has(name)) {
            return nullptr;
        }
        return m_.lookupGlobal(name);
    }

    size_t numLocals() const { return locals_.count(); }

    /**************************************************** Encoding interface */

    Encoder& encoder() { return encoder_; }

    MOZ_MUST_USE bool writeInt32Lit(int32_t i32) {
        return encoder().writeOp(Op::I32Const) &&
               encoder().writeVarS32(i32);
    }
    MOZ_MUST_USE bool writeConstExpr(const NumLit& lit) {
        switch (lit.which()) {
          case NumLit::Fixnum:
          case NumLit::NegativeInt:
          case NumLit::BigUnsigned:
            return writeInt32Lit(lit.toInt32());
          case NumLit::Float:
            return encoder().writeOp(Op::F32Const) &&
                   encoder().writeFixedF32(lit.toFloat());
          case NumLit::Double:
            return encoder().writeOp(Op::F64Const) &&
                   encoder().writeFixedF64(lit.toDouble());
          case NumLit::OutOfRangeInt:
            break;
        }
        MOZ_CRASH("unexpected literal type");
    }
    MOZ_MUST_USE bool writeCall(ParseNode* pn, Op op) {
        if (!encoder().writeOp(op)) {
            return false;
        }

        TokenStreamAnyChars& anyChars = m().tokenStream().anyCharsAccess();
        return callSiteLineNums_.append(anyChars.srcCoords.lineNum(pn->pn_pos.begin));
    }
    MOZ_MUST_USE bool writeCall(ParseNode* pn, MozOp op) {
        if (!encoder().writeOp(op)) {
            return false;
        }

        TokenStreamAnyChars& anyChars = m().tokenStream().anyCharsAccess();
        return callSiteLineNums_.append(anyChars.srcCoords.lineNum(pn->pn_pos.begin));
    }
    MOZ_MUST_USE bool prepareCall(ParseNode* pn) {
        TokenStreamAnyChars& anyChars = m().tokenStream().anyCharsAccess();
        return callSiteLineNums_.append(anyChars.srcCoords.lineNum(pn->pn_pos.begin));
    }
};

} /* anonymous namespace */

/*****************************************************************************/
// asm.js type-checking and code-generation algorithm

static bool
CheckIdentifier(ModuleValidator& m, ParseNode* usepn, PropertyName* name)
{
    if (name == m.cx()->names().arguments || name == m.cx()->names().eval) {
        return m.failName(usepn, "'%s' is not an allowed identifier", name);
    }
    return true;
}

static bool
CheckModuleLevelName(ModuleValidator& m, ParseNode* usepn, PropertyName* name)
{
    if (!CheckIdentifier(m, usepn, name)) {
        return false;
    }

    if (name == m.moduleFunctionName() ||
        name == m.globalArgumentName() ||
        name == m.importArgumentName() ||
        name == m.bufferArgumentName() ||
        m.lookupGlobal(name))
    {
        return m.failName(usepn, "duplicate name '%s' not allowed", name);
    }

    return true;
}

static bool
CheckFunctionHead(ModuleValidator& m, CodeNode* funNode)
{
    FunctionBox* funbox = funNode->funbox();
    MOZ_ASSERT(!funbox->hasExprBody());

    if (funbox->hasRest()) {
        return m.fail(funNode, "rest args not allowed");
    }
    if (funbox->hasDestructuringArgs) {
        return m.fail(funNode, "destructuring args not allowed");
    }
    return true;
}

static bool
CheckArgument(ModuleValidator& m, ParseNode* arg, PropertyName** name)
{
    *name = nullptr;

    if (!arg->isKind(ParseNodeKind::Name)) {
        return m.fail(arg, "argument is not a plain name");
    }

    PropertyName* argName = arg->as<NameNode>().name();;
    if (!CheckIdentifier(m, arg, argName)) {
        return false;
    }

    *name = argName;
    return true;
}

static bool
CheckModuleArgument(ModuleValidator& m, ParseNode* arg, PropertyName** name)
{
    if (!CheckArgument(m, arg, name)) {
        return false;
    }

    if (!CheckModuleLevelName(m, arg, *name)) {
        return false;
    }

    return true;
}

static bool
CheckModuleArguments(ModuleValidator& m, CodeNode* funNode)
{
    unsigned numFormals;
    ParseNode* arg1 = FunctionFormalParametersList(funNode, &numFormals);
    ParseNode* arg2 = arg1 ? NextNode(arg1) : nullptr;
    ParseNode* arg3 = arg2 ? NextNode(arg2) : nullptr;

    if (numFormals > 3) {
        return m.fail(funNode, "asm.js modules takes at most 3 argument");
    }

    PropertyName* arg1Name = nullptr;
    if (arg1 && !CheckModuleArgument(m, arg1, &arg1Name)) {
        return false;
    }
    if (!m.initGlobalArgumentName(arg1Name)) {
        return false;
    }

    PropertyName* arg2Name = nullptr;
    if (arg2 && !CheckModuleArgument(m, arg2, &arg2Name)) {
        return false;
    }
    if (!m.initImportArgumentName(arg2Name)) {
        return false;
    }

    PropertyName* arg3Name = nullptr;
    if (arg3 && !CheckModuleArgument(m, arg3, &arg3Name)) {
        return false;
    }
    if (!m.initBufferArgumentName(arg3Name)) {
        return false;
    }

    return true;
}

static bool
CheckPrecedingStatements(ModuleValidator& m, ParseNode* stmtList)
{
    MOZ_ASSERT(stmtList->isKind(ParseNodeKind::StatementList));

    ParseNode* stmt = ListHead(stmtList);
    for (unsigned i = 0, n = ListLength(stmtList); i < n; i++) {
        if (!IsIgnoredDirective(m.cx(), stmt)) {
            return m.fail(stmt, "invalid asm.js statement");
        }
    }

    return true;
}

static bool
CheckGlobalVariableInitConstant(ModuleValidator& m, PropertyName* varName, ParseNode* initNode,
                                bool isConst)
{
    NumLit lit = ExtractNumericLiteral(m, initNode);
    if (!lit.valid()) {
        return m.fail(initNode, "global initializer is out of representable integer range");
    }

    Type canonicalType = Type::canonicalize(Type::lit(lit));
    if (!canonicalType.isGlobalVarType()) {
        return m.fail(initNode, "global variable type not allowed");
    }

    return m.addGlobalVarInit(varName, lit, canonicalType, isConst);
}

static bool
CheckTypeAnnotation(ModuleValidator& m, ParseNode* coercionNode, Type* coerceTo,
                    ParseNode** coercedExpr = nullptr)
{
    switch (coercionNode->getKind()) {
      case ParseNodeKind::BitOr: {
        ParseNode* rhs = BitwiseRight(coercionNode);
        uint32_t i;
        if (!IsLiteralInt(m, rhs, &i) || i != 0) {
            return m.fail(rhs, "must use |0 for argument/return coercion");
        }
        *coerceTo = Type::Int;
        if (coercedExpr) {
            *coercedExpr = BitwiseLeft(coercionNode);
        }
        return true;
      }
      case ParseNodeKind::Pos: {
        *coerceTo = Type::Double;
        if (coercedExpr) {
            *coercedExpr = UnaryKid(coercionNode);
        }
        return true;
      }
      case ParseNodeKind::Call: {
        if (IsCoercionCall(m, coercionNode, coerceTo, coercedExpr)) {
            return true;
        }
        break;
      }
      default:;
    }

    return m.fail(coercionNode, "must be of the form +x, x|0 or fround(x)");
}

static bool
CheckGlobalVariableInitImport(ModuleValidator& m, PropertyName* varName, ParseNode* initNode,
                              bool isConst)
{
    Type coerceTo;
    ParseNode* coercedExpr;
    if (!CheckTypeAnnotation(m, initNode, &coerceTo, &coercedExpr)) {
        return false;
    }

    if (!coercedExpr->isKind(ParseNodeKind::Dot)) {
        return m.failName(coercedExpr, "invalid import expression for global '%s'", varName);
    }

    if (!coerceTo.isGlobalVarType()) {
        return m.fail(initNode, "global variable type not allowed");
    }

    ParseNode* base = DotBase(coercedExpr);
    PropertyName* field = DotMember(coercedExpr);

    PropertyName* importName = m.importArgumentName();
    if (!importName) {
        return m.fail(coercedExpr, "cannot import without an asm.js foreign parameter");
    }
    if (!IsUseOfName(base, importName)) {
        return m.failName(coercedExpr, "base of import expression must be '%s'", importName);
    }

    return m.addGlobalVarImport(varName, field, coerceTo, isConst);
}

static bool
IsArrayViewCtorName(ModuleValidator& m, PropertyName* name, Scalar::Type* type)
{
    JSAtomState& names = m.cx()->names();
    if (name == names.Int8Array) {
        *type = Scalar::Int8;
    } else if (name == names.Uint8Array) {
        *type = Scalar::Uint8;
    } else if (name == names.Int16Array) {
        *type = Scalar::Int16;
    } else if (name == names.Uint16Array) {
        *type = Scalar::Uint16;
    } else if (name == names.Int32Array) {
        *type = Scalar::Int32;
    } else if (name == names.Uint32Array) {
        *type = Scalar::Uint32;
    } else if (name == names.Float32Array) {
        *type = Scalar::Float32;
    } else if (name == names.Float64Array) {
        *type = Scalar::Float64;
    } else {
        return false;
    }
    return true;
}

static bool
CheckNewArrayViewArgs(ModuleValidator& m, ParseNode* newExpr, PropertyName* bufferName)
{
    ParseNode* ctorExpr = BinaryLeft(newExpr);
    ParseNode* ctorArgs = BinaryRight(newExpr);
    ParseNode* bufArg = ListHead(ctorArgs);
    if (!bufArg || NextNode(bufArg) != nullptr) {
        return m.fail(ctorExpr, "array view constructor takes exactly one argument");
    }

    if (!IsUseOfName(bufArg, bufferName)) {
        return m.failName(bufArg, "argument to array view constructor must be '%s'", bufferName);
    }

    return true;
}

static bool
CheckNewArrayView(ModuleValidator& m, PropertyName* varName, ParseNode* newExpr)
{
    PropertyName* globalName = m.globalArgumentName();
    if (!globalName) {
        return m.fail(newExpr, "cannot create array view without an asm.js global parameter");
    }

    PropertyName* bufferName = m.bufferArgumentName();
    if (!bufferName) {
        return m.fail(newExpr, "cannot create array view without an asm.js heap parameter");
    }

    ParseNode* ctorExpr = BinaryLeft(newExpr);

    PropertyName* field;
    Scalar::Type type;
    if (ctorExpr->isKind(ParseNodeKind::Dot)) {
        ParseNode* base = DotBase(ctorExpr);

        if (!IsUseOfName(base, globalName)) {
            return m.failName(base, "expecting '%s.*Array", globalName);
        }

        field = DotMember(ctorExpr);
        if (!IsArrayViewCtorName(m, field, &type)) {
            return m.fail(ctorExpr, "could not match typed array name");
        }
    } else {
        if (!ctorExpr->isKind(ParseNodeKind::Name)) {
            return m.fail(ctorExpr, "expecting name of imported array view constructor");
        }

        PropertyName* globalName = ctorExpr->as<NameNode>().name();
        const ModuleValidator::Global* global = m.lookupGlobal(globalName);
        if (!global) {
            return m.failName(ctorExpr, "%s not found in module global scope", globalName);
        }

        if (global->which() != ModuleValidator::Global::ArrayViewCtor) {
            return m.failName(ctorExpr, "%s must be an imported array view constructor", globalName);
        }

        field = nullptr;
        type = global->viewType();
    }

    if (!CheckNewArrayViewArgs(m, newExpr, bufferName)) {
        return false;
    }

    return m.addArrayView(varName, type, field);
}

static bool
CheckGlobalMathImport(ModuleValidator& m, ParseNode* initNode, PropertyName* varName,
                      PropertyName* field)
{
    // Math builtin, with the form glob.Math.[[builtin]]
    ModuleValidator::MathBuiltin mathBuiltin;
    if (!m.lookupStandardLibraryMathName(field, &mathBuiltin)) {
        return m.failName(initNode, "'%s' is not a standard Math builtin", field);
    }

    switch (mathBuiltin.kind) {
      case ModuleValidator::MathBuiltin::Function:
        return m.addMathBuiltinFunction(varName, mathBuiltin.u.func, field);
      case ModuleValidator::MathBuiltin::Constant:
        return m.addMathBuiltinConstant(varName, mathBuiltin.u.cst, field);
      default:
        break;
    }
    MOZ_CRASH("unexpected or uninitialized math builtin type");
}

static bool
CheckGlobalDotImport(ModuleValidator& m, PropertyName* varName, ParseNode* initNode)
{
    ParseNode* base = DotBase(initNode);
    PropertyName* field = DotMember(initNode);

    if (base->isKind(ParseNodeKind::Dot)) {
        ParseNode* global = DotBase(base);
        PropertyName* math = DotMember(base);

        PropertyName* globalName = m.globalArgumentName();
        if (!globalName) {
            return m.fail(base, "import statement requires the module have a stdlib parameter");
        }

        if (!IsUseOfName(global, globalName)) {
            if (global->isKind(ParseNodeKind::Dot)) {
                return m.failName(base, "imports can have at most two dot accesses "
                                        "(e.g. %s.Math.sin)", globalName);
            }
            return m.failName(base, "expecting %s.*", globalName);
        }

        if (math == m.cx()->names().Math) {
            return CheckGlobalMathImport(m, initNode, varName, field);
        }
        return m.failName(base, "expecting %s.Math", globalName);
    }

    if (!base->isKind(ParseNodeKind::Name)) {
        return m.fail(base, "expected name of variable or parameter");
    }

    auto baseName = base->as<NameNode>().name();
    if (baseName == m.globalArgumentName()) {
        if (field == m.cx()->names().NaN) {
            return m.addGlobalConstant(varName, GenericNaN(), field);
        }
        if (field == m.cx()->names().Infinity) {
            return m.addGlobalConstant(varName, PositiveInfinity<double>(), field);
        }

        Scalar::Type type;
        if (IsArrayViewCtorName(m, field, &type)) {
            return m.addArrayViewCtor(varName, type, field);
        }

        return m.failName(initNode, "'%s' is not a standard constant or typed array name", field);
    }

    if (baseName != m.importArgumentName()) {
        return m.fail(base, "expected global or import name");
    }

    return m.addFFI(varName, field);
}

static bool
CheckModuleGlobal(ModuleValidator& m, ParseNode* var, bool isConst)
{
    if (!var->isKind(ParseNodeKind::Name)) {
        return m.fail(var, "import variable is not a plain name");
    }

    PropertyName* varName = var->as<NameNode>().name();
    if (!CheckModuleLevelName(m, var, varName)) {
        return false;
    }

    ParseNode* initNode = MaybeInitializer(var);
    if (!initNode) {
        return m.fail(var, "module import needs initializer");
    }

    if (IsNumericLiteral(m, initNode)) {
        return CheckGlobalVariableInitConstant(m, varName, initNode, isConst);
    }

    if (initNode->isKind(ParseNodeKind::BitOr) ||
        initNode->isKind(ParseNodeKind::Pos) ||
        initNode->isKind(ParseNodeKind::Call))
    {
        return CheckGlobalVariableInitImport(m, varName, initNode, isConst);
    }

    if (initNode->isKind(ParseNodeKind::New)) {
        return CheckNewArrayView(m, varName, initNode);
    }

    if (initNode->isKind(ParseNodeKind::Dot)) {
        return CheckGlobalDotImport(m, varName, initNode);
    }

    return m.fail(initNode, "unsupported import expression");
}

static bool
CheckModuleProcessingDirectives(ModuleValidator& m)
{
    auto& ts = m.parser().tokenStream;
    while (true) {
        bool matched;
        if (!ts.matchToken(&matched, TokenKind::String, TokenStreamShared::Operand)) {
            return false;
        }
        if (!matched) {
            return true;
        }

        if (!IsIgnoredDirectiveName(m.cx(), ts.anyCharsAccess().currentToken().atom())) {
            return m.failCurrentOffset("unsupported processing directive");
        }

        TokenKind tt;
        if (!ts.getToken(&tt)) {
            return false;
        }
        if (tt != TokenKind::Semi) {
            return m.failCurrentOffset("expected semicolon after string literal");
        }
    }
}

static bool
CheckModuleGlobals(ModuleValidator& m)
{
    while (true) {
        ParseNode* varStmt;
        if (!ParseVarOrConstStatement(m.parser(), &varStmt)) {
            return false;
        }
        if (!varStmt) {
            break;
        }
        for (ParseNode* var = VarListHead(varStmt); var; var = NextNode(var)) {
            if (!CheckModuleGlobal(m, var, varStmt->isKind(ParseNodeKind::Const))) {
                return false;
            }
        }
    }

    return true;
}

static bool
ArgFail(FunctionValidator& f, PropertyName* argName, ParseNode* stmt)
{
    return f.failName(stmt, "expecting argument type declaration for '%s' of the "
                      "form 'arg = arg|0' or 'arg = +arg' or 'arg = fround(arg)'", argName);
}

static bool
CheckArgumentType(FunctionValidator& f, ParseNode* stmt, PropertyName* name, Type* type)
{
    if (!stmt || !IsExpressionStatement(stmt)) {
        return ArgFail(f, name, stmt ? stmt : f.fn());
    }

    ParseNode* initNode = ExpressionStatementExpr(stmt);
    if (!initNode->isKind(ParseNodeKind::Assign)) {
        return ArgFail(f, name, stmt);
    }

    ParseNode* argNode = BinaryLeft(initNode);
    ParseNode* coercionNode = BinaryRight(initNode);

    if (!IsUseOfName(argNode, name)) {
        return ArgFail(f, name, stmt);
    }

    ParseNode* coercedExpr;
    if (!CheckTypeAnnotation(f.m(), coercionNode, type, &coercedExpr)) {
        return false;
    }

    if (!type->isArgType()) {
        return f.failName(stmt, "invalid type for argument '%s'", name);
    }

    if (!IsUseOfName(coercedExpr, name)) {
        return ArgFail(f, name, stmt);
    }

    return true;
}

static bool
CheckProcessingDirectives(ModuleValidator& m, ParseNode** stmtIter)
{
    ParseNode* stmt = *stmtIter;

    while (stmt && IsIgnoredDirective(m.cx(), stmt)) {
        stmt = NextNode(stmt);
    }

    *stmtIter = stmt;
    return true;
}

static bool
CheckArguments(FunctionValidator& f, ParseNode** stmtIter, ValTypeVector* argTypes)
{
    ParseNode* stmt = *stmtIter;

    unsigned numFormals;
    ParseNode* argpn = FunctionFormalParametersList(f.fn(), &numFormals);

    for (unsigned i = 0; i < numFormals; i++, argpn = NextNode(argpn), stmt = NextNode(stmt)) {
        PropertyName* name;
        if (!CheckArgument(f.m(), argpn, &name)) {
            return false;
        }

        Type type;
        if (!CheckArgumentType(f, stmt, name, &type)) {
            return false;
        }

        if (!argTypes->append(type.canonicalToValType())) {
            return false;
        }

        if (!f.addLocal(argpn, name, type)) {
            return false;
        }
    }

    *stmtIter = stmt;
    return true;
}

static bool
IsLiteralOrConst(FunctionValidator& f, ParseNode* pn, NumLit* lit)
{
    if (pn->isKind(ParseNodeKind::Name)) {
        const ModuleValidator::Global* global = f.lookupGlobal(pn->as<NameNode>().name());
        if (!global || global->which() != ModuleValidator::Global::ConstantLiteral) {
            return false;
        }

        *lit = global->constLiteralValue();
        return true;
    }

    if (!IsNumericLiteral(f.m(), pn)) {
        return false;
    }

    *lit = ExtractNumericLiteral(f.m(), pn);
    return true;
}

static bool
CheckFinalReturn(FunctionValidator& f, ParseNode* lastNonEmptyStmt)
{
    if (!f.encoder().writeOp(Op::End)) {
        return false;
    }

    if (!f.hasAlreadyReturned()) {
        f.setReturnedType(ExprType::Void);
        return true;
    }

    if (!lastNonEmptyStmt->isKind(ParseNodeKind::Return) && !IsVoid(f.returnedType())) {
        return f.fail(lastNonEmptyStmt, "void incompatible with previous return type");
    }
=======
    AsmJSGlobal g(AsmJSGlobal::ArrayViewCtor, std::move(fieldChars));
    g.pod.u.viewType_ = vt;
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }
  bool addFFI(PropertyName* var, PropertyName* field) {
    UniqueChars fieldChars = StringToNewUTF8CharsZ(cx_, *field);
    if (!fieldChars) {
      return false;
    }

    if (asmJSMetadata_->numFFIs == UINT32_MAX) {
      return false;
    }
    uint32_t ffiIndex = asmJSMetadata_->numFFIs++;

    Global* global = validationLifo_.new_<Global>(Global::FFI);
    if (!global) {
      return false;
    }
    new (&global->u.ffiIndex_) uint32_t(ffiIndex);
    if (!globalMap_.putNew(var, global)) {
      return false;
    }

    AsmJSGlobal g(AsmJSGlobal::FFI, std::move(fieldChars));
    g.pod.u.ffiIndex_ = ffiIndex;
    return asmJSMetadata_->asmJSGlobals.append(std::move(g));
  }
  bool addExportField(const Func& func, PropertyName* maybeField) {
    // Record the field name of this export.
    CacheableChars fieldChars;
    if (maybeField) {
      fieldChars = StringToNewUTF8CharsZ(cx_, *maybeField);
    } else {
      fieldChars = DuplicateString("");
    }
    if (!fieldChars) {
      return false;
    }

    // Declare which function is exported which gives us an index into the
    // module ExportVector.
    uint32_t funcIndex = funcImportMap_.count() + func.funcDefIndex();
    if (!env_.exports.emplaceBack(std::move(fieldChars), funcIndex,
                                  DefinitionKind::Function)) {
      return false;
    }

    // The exported function might have already been exported in which case
    // the index will refer into the range of AsmJSExports.
    return asmJSMetadata_->asmJSExports.emplaceBack(
        funcIndex, func.srcBegin() - asmJSMetadata_->srcStart,
        func.srcEnd() - asmJSMetadata_->srcStart);
  }

  bool defineFuncPtrTable(uint32_t tableIndex, Uint32Vector&& elems) {
    Table& table = *tables_[tableIndex];
    if (table.defined()) {
      return false;
    }

    table.define();

    for (uint32_t& index : elems) {
      index += funcImportMap_.count();
    }

    MutableElemSegment seg = js_new<ElemSegment>();
    if (!seg) {
      return false;
    }
    seg->tableIndex = tableIndex;
    seg->offsetIfActive = Some(InitExpr(LitVal(uint32_t(0))));
    seg->elemFuncIndices = std::move(elems);
    return env_.elemSegments.append(std::move(seg));
  }

  bool tryConstantAccess(uint64_t start, uint64_t width) {
    MOZ_ASSERT(UINT64_MAX - start > width);
    uint64_t len = start + width;
    if (len > uint64_t(INT32_MAX) + 1) {
      return false;
    }
    len = RoundUpToNextValidAsmJSHeapLength(len);
    if (len > env_.minMemoryLength) {
      env_.minMemoryLength = len;
    }
    return true;
  }

  // Error handling.
  bool hasAlreadyFailed() const { return !!errorString_; }

  bool failOffset(uint32_t offset, const char* str) {
    MOZ_ASSERT(!hasAlreadyFailed());
    MOZ_ASSERT(errorOffset_ == UINT32_MAX);
    MOZ_ASSERT(str);
    errorOffset_ = offset;
    errorString_ = DuplicateString(str);
    return false;
  }

  bool fail(ParseNode* pn, const char* str) {
    return failOffset(pn->pn_pos.begin, str);
  }

  bool failfVAOffset(uint32_t offset, const char* fmt, va_list ap)
      MOZ_FORMAT_PRINTF(3, 0) {
    MOZ_ASSERT(!hasAlreadyFailed());
    MOZ_ASSERT(errorOffset_ == UINT32_MAX);
    MOZ_ASSERT(fmt);
    errorOffset_ = offset;
    errorString_ = JS_vsmprintf(fmt, ap);
    return false;
  }

  bool failfOffset(uint32_t offset, const char* fmt, ...)
      MOZ_FORMAT_PRINTF(3, 4) {
    va_list ap;
    va_start(ap, fmt);
    failfVAOffset(offset, fmt, ap);
    va_end(ap);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *stmtIter = stmt;
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
  bool failf(ParseNode* pn, const char* fmt, ...) MOZ_FORMAT_PRINTF(3, 4) {
    va_list ap;
    va_start(ap, fmt);
    failfVAOffset(pn->pn_pos.begin, fmt, ap);
    va_end(ap);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckExpr(FunctionValidator& f, ParseNode* op, Type* type);
||||||| merged common ancestors
static bool
CheckVariable(FunctionValidator& f, ParseNode* var, ValTypeVector* types, Vector<NumLit>* inits)
{
    if (!var->isKind(ParseNodeKind::Name)) {
        return f.fail(var, "local variable is not a plain name");
    }
=======
  bool failNameOffset(uint32_t offset, const char* fmt, PropertyName* name) {
    // This function is invoked without the caller properly rooting its locals.
    gc::AutoSuppressGC suppress(cx_);
    if (UniqueChars bytes = AtomToPrintableString(cx_, name)) {
      failfOffset(offset, fmt, bytes.get());
    }
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckNumericLiteral(FunctionValidator& f, ParseNode* num,
                                Type* type) {
  NumLit lit = ExtractNumericLiteral(f.m(), num);
  if (!lit.valid()) {
    return f.fail(num, "numeric literal out of representable integer range");
  }
  *type = Type::lit(lit);
  return f.writeConstExpr(lit);
}

static bool CheckVarRef(FunctionValidator& f, ParseNode* varRef, Type* type) {
  PropertyName* name = varRef->as<NameNode>().name();
||||||| merged common ancestors
    PropertyName* name = var->as<NameNode>().name();

    if (!CheckIdentifier(f.m(), var, name)) {
        return false;
    }
=======
  bool failName(ParseNode* pn, const char* fmt, PropertyName* name) {
    return failNameOffset(pn->pn_pos.begin, fmt, name);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (const FunctionValidator::Local* local = f.lookupLocal(name)) {
    if (!f.encoder().writeOp(Op::GetLocal)) {
      return false;
    }
    if (!f.encoder().writeVarU32(local->slot)) {
      return false;
    }
    *type = local->type;
    return true;
  }

  if (const ModuleValidator::Global* global = f.lookupGlobal(name)) {
    switch (global->which()) {
      case ModuleValidator::Global::ConstantLiteral:
        *type = global->varOrConstType();
        return f.writeConstExpr(global->constLiteralValue());
      case ModuleValidator::Global::ConstantImport:
      case ModuleValidator::Global::Variable: {
        *type = global->varOrConstType();
        return f.encoder().writeOp(Op::GetGlobal) &&
               f.encoder().writeVarU32(global->varOrConstIndex());
      }
      case ModuleValidator::Global::Function:
      case ModuleValidator::Global::FFI:
      case ModuleValidator::Global::MathBuiltinFunction:
      case ModuleValidator::Global::Table:
      case ModuleValidator::Global::ArrayView:
      case ModuleValidator::Global::ArrayViewCtor:
        break;
||||||| merged common ancestors
    ParseNode* initNode = MaybeInitializer(var);
    if (!initNode) {
        return f.failName(var, "var '%s' needs explicit type declaration via an initial value", name);
    }

    NumLit lit;
    if (!IsLiteralOrConst(f, initNode, &lit)) {
        return f.failName(var, "var '%s' initializer must be literal or const literal", name);
    }

    if (!lit.valid()) {
        return f.failName(var, "var '%s' initializer out of range", name);
=======
  bool failOverRecursed() {
    errorOverRecursed_ = true;
    return false;
  }

  unsigned numArrayViews() const { return arrayViews_.length(); }
  const ArrayView& arrayView(unsigned i) const { return arrayViews_[i]; }
  unsigned numFuncDefs() const { return funcDefs_.length(); }
  const Func& funcDef(unsigned i) const { return funcDefs_[i]; }
  unsigned numFuncPtrTables() const { return tables_.length(); }
  Table& table(unsigned i) const { return *tables_[i]; }

  const Global* lookupGlobal(PropertyName* name) const {
    if (GlobalMap::Ptr p = globalMap_.lookup(name)) {
      return p->value();
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    return f.failName(varRef,
                      "'%s' may not be accessed by ordinary expressions", name);
  }
||||||| merged common ancestors
=======
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return f.failName(varRef, "'%s' not found in local or asm.js module scope",
                    name);
}

static inline bool IsLiteralOrConstInt(FunctionValidator& f, ParseNode* pn,
                                       uint32_t* u32) {
  NumLit lit;
  if (!IsLiteralOrConst(f, pn, &lit)) {
    return false;
  }

  return IsLiteralInt(lit, u32);
}

static const int32_t NoMask = -1;

static bool CheckArrayAccess(FunctionValidator& f, ParseNode* viewName,
                             ParseNode* indexExpr, Scalar::Type* viewType) {
  if (!viewName->isKind(ParseNodeKind::Name)) {
    return f.fail(viewName,
                  "base of array access must be a typed array view name");
  }
||||||| merged common ancestors
    Type type = Type::canonicalize(Type::lit(lit));

    return f.addLocal(var, name, type) &&
           types->append(type.canonicalToValType()) &&
           inits->append(lit);
}

static bool
CheckVariables(FunctionValidator& f, ParseNode** stmtIter)
{
    ParseNode* stmt = *stmtIter;

    uint32_t firstVar = f.numLocals();

    ValTypeVector types;
    Vector<NumLit> inits(f.cx());

    for (; stmt && stmt->isKind(ParseNodeKind::Var); stmt = NextNonEmptyStatement(stmt)) {
        for (ParseNode* var = VarListHead(stmt); var; var = NextNode(var)) {
            if (!CheckVariable(f, var, &types, &inits)) {
                return false;
            }
        }
    }
=======
  Func* lookupFuncDef(PropertyName* name) {
    if (GlobalMap::Ptr p = globalMap_.lookup(name)) {
      Global* value = p->value();
      if (value->which() == Global::Function) {
        return &funcDefs_[value->funcDefIndex()];
      }
    }
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const ModuleValidator::Global* global =
      f.lookupGlobal(viewName->as<NameNode>().name());
  if (!global || !global->isAnyArrayView()) {
    return f.fail(viewName,
                  "base of array access must be a typed array view name");
  }

  *viewType = global->viewType();
||||||| merged common ancestors
    MOZ_ASSERT(f.encoder().empty());

    if (!EncodeLocalEntries(f.encoder(), types)) {
        return false;
    }
=======
  bool lookupStandardLibraryMathName(PropertyName* name,
                                     MathBuiltin* mathBuiltin) const {
    if (MathNameMap::Ptr p = standardLibraryMathNames_.lookup(name)) {
      *mathBuiltin = p->value();
      return true;
    }
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t index;
  if (IsLiteralOrConstInt(f, indexExpr, &index)) {
    uint64_t byteOffset = uint64_t(index) << TypedArrayShift(*viewType);
    uint64_t width = TypedArrayElemSize(*viewType);
    if (!f.m().tryConstantAccess(byteOffset, width)) {
      return f.fail(indexExpr, "constant index out of range");
||||||| merged common ancestors
    for (uint32_t i = 0; i < inits.length(); i++) {
        NumLit lit = inits[i];
        if (lit.isZeroBits()) {
            continue;
        }
        if (!f.writeConstExpr(lit)) {
            return false;
        }
        if (!f.encoder().writeOp(Op::SetLocal)) {
            return false;
        }
        if (!f.encoder().writeVarU32(firstVar + i)) {
            return false;
        }
=======
  bool startFunctionBodies() {
    if (!arrayViews_.empty()) {
      env_.memoryUsage = MemoryUsage::Unshared;
    } else {
      env_.memoryUsage = MemoryUsage::None;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD

    return f.writeInt32Lit(byteOffset);
  }

  // Mask off the low bits to account for the clearing effect of a right shift
  // followed by the left shift implicit in the array access. E.g., H32[i>>2]
  // loses the low two bits.
  int32_t mask = ~(TypedArrayElemSize(*viewType) - 1);

  if (indexExpr->isKind(ParseNodeKind::Rsh)) {
    ParseNode* shiftAmountNode = BitwiseRight(indexExpr);

    uint32_t shift;
    if (!IsLiteralInt(f.m(), shiftAmountNode, &shift)) {
      return f.failf(shiftAmountNode, "shift amount must be constant");
    }

    unsigned requiredShift = TypedArrayShift(*viewType);
    if (shift != requiredShift) {
      return f.failf(shiftAmountNode, "shift amount must be %u", requiredShift);
    }

    ParseNode* pointerNode = BitwiseLeft(indexExpr);

    Type pointerType;
    if (!CheckExpr(f, pointerNode, &pointerType)) {
      return false;
    }

    if (!pointerType.isIntish()) {
      return f.failf(pointerNode, "%s is not a subtype of int",
                     pointerType.toChars());
    }
  } else {
    // For legacy scalar access compatibility, accept Int8/Uint8 accesses
    // with no shift.
    if (TypedArrayShift(*viewType) != 0) {
      return f.fail(
          indexExpr,
          "index expression isn't shifted; must be an Int8/Uint8 access");
    }

    MOZ_ASSERT(mask == NoMask);

    ParseNode* pointerNode = indexExpr;
||||||| merged common ancestors

    *stmtIter = stmt;
    return true;
}

static bool
CheckExpr(FunctionValidator& f, ParseNode* op, Type* type);

static bool
CheckNumericLiteral(FunctionValidator& f, ParseNode* num, Type* type)
{
    NumLit lit = ExtractNumericLiteral(f.m(), num);
    if (!lit.valid()) {
        return f.fail(num, "numeric literal out of representable integer range");
    }
    *type = Type::lit(lit);
    return f.writeConstExpr(lit);
}

static bool
CheckVarRef(FunctionValidator& f, ParseNode* varRef, Type* type)
{
    PropertyName* name = varRef->as<NameNode>().name();

    if (const FunctionValidator::Local* local = f.lookupLocal(name)) {
        if (!f.encoder().writeOp(Op::GetLocal)) {
            return false;
        }
        if (!f.encoder().writeVarU32(local->slot)) {
            return false;
        }
        *type = local->type;
        return true;
    }

    if (const ModuleValidator::Global* global = f.lookupGlobal(name)) {
        switch (global->which()) {
          case ModuleValidator::Global::ConstantLiteral:
            *type = global->varOrConstType();
            return f.writeConstExpr(global->constLiteralValue());
          case ModuleValidator::Global::ConstantImport:
          case ModuleValidator::Global::Variable: {
            *type = global->varOrConstType();
            return f.encoder().writeOp(Op::GetGlobal) &&
                   f.encoder().writeVarU32(global->varOrConstIndex());
          }
          case ModuleValidator::Global::Function:
          case ModuleValidator::Global::FFI:
          case ModuleValidator::Global::MathBuiltinFunction:
          case ModuleValidator::Global::Table:
          case ModuleValidator::Global::ArrayView:
          case ModuleValidator::Global::ArrayViewCtor:
            break;
        }
        return f.failName(varRef, "'%s' may not be accessed by ordinary expressions", name);
    }

    return f.failName(varRef, "'%s' not found in local or asm.js module scope", name);
}

static inline bool
IsLiteralOrConstInt(FunctionValidator& f, ParseNode* pn, uint32_t* u32)
{
    NumLit lit;
    if (!IsLiteralOrConst(f, pn, &lit)) {
        return false;
    }

    return IsLiteralInt(lit, u32);
}

static const int32_t NoMask = -1;

static bool
CheckArrayAccess(FunctionValidator& f, ParseNode* viewName, ParseNode* indexExpr,
                 Scalar::Type* viewType)
{
    if (!viewName->isKind(ParseNodeKind::Name)) {
        return f.fail(viewName, "base of array access must be a typed array view name");
    }

    const ModuleValidator::Global* global = f.lookupGlobal(viewName->as<NameNode>().name());
    if (!global || !global->isAnyArrayView()) {
        return f.fail(viewName, "base of array access must be a typed array view name");
    }

    *viewType = global->viewType();

    uint32_t index;
    if (IsLiteralOrConstInt(f, indexExpr, &index)) {
        uint64_t byteOffset = uint64_t(index) << TypedArrayShift(*viewType);
        uint64_t width = TypedArrayElemSize(*viewType);
        if (!f.m().tryConstantAccess(byteOffset, width)) {
            return f.fail(indexExpr, "constant index out of range");
        }
=======
    return true;
  }
};
>>>>>>> upstream-releases

<<<<<<< HEAD
    Type pointerType;
    if (!CheckExpr(f, pointerNode, &pointerType)) {
      return false;
||||||| merged common ancestors
        return f.writeInt32Lit(byteOffset);
=======
// The ModuleValidator encapsulates the entire validation of an asm.js module.
// Its lifetime goes from the validation of the top components of an asm.js
// module (all the globals), the emission of bytecode for all the functions in
// the module and the validation of function's pointer tables. It also finishes
// the compilation of all the module's stubs.
//
// Rooting note: ModuleValidator is a stack class that contains unrooted
// PropertyName (JSAtom) pointers.  This is safe because it cannot be
// constructed without a TokenStream reference.  TokenStream is itself a stack
// class that cannot be constructed without an AutoKeepAtoms being live on the
// stack, which prevents collection of atoms.
//
// ModuleValidator is marked as rooted in the rooting analysis.  Don't add
// non-JSAtom pointers, or this will break!
template <typename Unit>
class MOZ_STACK_CLASS JS_HAZ_ROOTED ModuleValidator
    : public ModuleValidatorShared {
 private:
  AsmJSParser<Unit>& parser_;

 public:
  ModuleValidator(JSContext* cx, AsmJSParser<Unit>& parser,
                  FunctionNode* moduleFunctionNode)
      : ModuleValidatorShared(cx, moduleFunctionNode), parser_(parser) {}

  ~ModuleValidator() {
    if (errorString_) {
      MOZ_ASSERT(errorOffset_ != UINT32_MAX);
      typeFailure(errorOffset_, errorString_.get());
    }
    if (errorOverRecursed_) {
      ReportOverRecursed(cx_);
    }
  }

 private:
  // Helpers:
  bool newSig(FuncType&& sig, uint32_t* sigIndex) {
    if (env_.types.length() >= MaxTypes) {
      return failCurrentOffset("too many signatures");
    }

    *sigIndex = env_.types.length();
    return env_.types.append(std::move(sig));
  }
  bool declareSig(FuncType&& sig, uint32_t* sigIndex) {
    SigSet::AddPtr p = sigSet_.lookupForAdd(sig);
    if (p) {
      *sigIndex = p->sigIndex();
      MOZ_ASSERT(env_.types[*sigIndex].funcType() == sig);
      return true;
    }

    return newSig(std::move(sig), sigIndex) &&
           sigSet_.add(p, HashableSig(*sigIndex, env_.types));
  }

 private:
  void typeFailure(uint32_t offset, ...) {
    va_list args;
    va_start(args, offset);

    auto& ts = tokenStream();
    ErrorMetadata metadata;
    if (ts.computeErrorMetadata(&metadata, AsVariant(offset))) {
      if (ts.anyCharsAccess().options().throwOnAsmJSValidationFailureOption) {
        ReportCompileError(cx_, std::move(metadata), nullptr, JSREPORT_ERROR,
                           JSMSG_USE_ASM_TYPE_FAIL, &args);
      } else {
        // asm.js type failure is indicated by calling one of the fail*
        // functions below.  These functions always return false to
        // halt asm.js parsing.  Whether normal parsing is attempted as
        // fallback, depends whether an exception is also set.
        //
        // If warning succeeds, no exception is set.  If warning fails,
        // an exception is set and execution will halt.  Thus it's safe
        // and correct to ignore the return value here.
        Unused << ts.compileWarning(std::move(metadata), nullptr,
                                    JSREPORT_WARNING, JSMSG_USE_ASM_TYPE_FAIL,
                                    &args);
      }
>>>>>>> upstream-releases
    }
    if (!pointerType.isInt()) {
      return f.failf(pointerNode, "%s is not a subtype of int",
                     pointerType.toChars());
    }
  }

<<<<<<< HEAD
  // Don't generate the mask op if there is no need for it which could happen
  // for a shift of zero.
  if (mask != NoMask) {
    return f.writeInt32Lit(mask) && f.encoder().writeOp(Op::I32And);
  }

  return true;
}

static bool WriteArrayAccessFlags(FunctionValidator& f, Scalar::Type viewType) {
  // asm.js only has naturally-aligned accesses.
  size_t align = TypedArrayElemSize(viewType);
  MOZ_ASSERT(IsPowerOfTwo(align));
  if (!f.encoder().writeFixedU8(CeilingLog2(align))) {
    return false;
  }

  // asm.js doesn't have constant offsets, so just encode a 0.
  if (!f.encoder().writeVarU32(0)) {
    return false;
  }

  return true;
}

static bool CheckLoadArray(FunctionValidator& f, ParseNode* elem, Type* type) {
  Scalar::Type viewType;

  if (!CheckArrayAccess(f, ElemBase(elem), ElemIndex(elem), &viewType)) {
    return false;
  }

  switch (viewType) {
    case Scalar::Int8:
      if (!f.encoder().writeOp(Op::I32Load8S)) return false;
      break;
    case Scalar::Uint8:
      if (!f.encoder().writeOp(Op::I32Load8U)) return false;
      break;
    case Scalar::Int16:
      if (!f.encoder().writeOp(Op::I32Load16S)) return false;
      break;
    case Scalar::Uint16:
      if (!f.encoder().writeOp(Op::I32Load16U)) return false;
      break;
    case Scalar::Uint32:
    case Scalar::Int32:
      if (!f.encoder().writeOp(Op::I32Load)) return false;
      break;
    case Scalar::Float32:
      if (!f.encoder().writeOp(Op::F32Load)) return false;
      break;
    case Scalar::Float64:
      if (!f.encoder().writeOp(Op::F64Load)) return false;
      break;
    default:
      MOZ_CRASH("unexpected scalar type");
  }

  switch (viewType) {
    case Scalar::Int8:
    case Scalar::Int16:
    case Scalar::Int32:
    case Scalar::Uint8:
    case Scalar::Uint16:
    case Scalar::Uint32:
      *type = Type::Intish;
      break;
    case Scalar::Float32:
      *type = Type::MaybeFloat;
      break;
    case Scalar::Float64:
      *type = Type::MaybeDouble;
      break;
    default:
      MOZ_CRASH("Unexpected array type");
  }

  if (!WriteArrayAccessFlags(f, viewType)) {
    return false;
  }

  return true;
}

static bool CheckStoreArray(FunctionValidator& f, ParseNode* lhs,
                            ParseNode* rhs, Type* type) {
  Scalar::Type viewType;
  if (!CheckArrayAccess(f, ElemBase(lhs), ElemIndex(lhs), &viewType)) {
    return false;
  }
||||||| merged common ancestors
    // Mask off the low bits to account for the clearing effect of a right shift
    // followed by the left shift implicit in the array access. E.g., H32[i>>2]
    // loses the low two bits.
    int32_t mask = ~(TypedArrayElemSize(*viewType) - 1);

    if (indexExpr->isKind(ParseNodeKind::Rsh)) {
        ParseNode* shiftAmountNode = BitwiseRight(indexExpr);

        uint32_t shift;
        if (!IsLiteralInt(f.m(), shiftAmountNode, &shift)) {
            return f.failf(shiftAmountNode, "shift amount must be constant");
        }

        unsigned requiredShift = TypedArrayShift(*viewType);
        if (shift != requiredShift) {
            return f.failf(shiftAmountNode, "shift amount must be %u", requiredShift);
        }

        ParseNode* pointerNode = BitwiseLeft(indexExpr);

        Type pointerType;
        if (!CheckExpr(f, pointerNode, &pointerType)) {
            return false;
        }

        if (!pointerType.isIntish()) {
            return f.failf(pointerNode, "%s is not a subtype of int", pointerType.toChars());
        }
    } else {
        // For legacy scalar access compatibility, accept Int8/Uint8 accesses
        // with no shift.
        if (TypedArrayShift(*viewType) != 0) {
            return f.fail(indexExpr, "index expression isn't shifted; must be an Int8/Uint8 access");
        }

        MOZ_ASSERT(mask == NoMask);

        ParseNode* pointerNode = indexExpr;
=======
    va_end(args);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type rhsType;
  if (!CheckExpr(f, rhs, &rhsType)) {
    return false;
  }

  switch (viewType) {
    case Scalar::Int8:
    case Scalar::Int16:
    case Scalar::Int32:
    case Scalar::Uint8:
    case Scalar::Uint16:
    case Scalar::Uint32:
      if (!rhsType.isIntish()) {
        return f.failf(lhs, "%s is not a subtype of intish", rhsType.toChars());
      }
      break;
    case Scalar::Float32:
      if (!rhsType.isMaybeDouble() && !rhsType.isFloatish()) {
        return f.failf(lhs, "%s is not a subtype of double? or floatish",
                       rhsType.toChars());
      }
      break;
    case Scalar::Float64:
      if (!rhsType.isMaybeFloat() && !rhsType.isMaybeDouble()) {
        return f.failf(lhs, "%s is not a subtype of float? or double?",
                       rhsType.toChars());
      }
      break;
    default:
      MOZ_CRASH("Unexpected view type");
  }

  switch (viewType) {
    case Scalar::Int8:
    case Scalar::Uint8:
      if (!f.encoder().writeOp(MozOp::I32TeeStore8)) {
        return false;
      }
      break;
    case Scalar::Int16:
    case Scalar::Uint16:
      if (!f.encoder().writeOp(MozOp::I32TeeStore16)) {
        return false;
      }
      break;
    case Scalar::Int32:
    case Scalar::Uint32:
      if (!f.encoder().writeOp(MozOp::I32TeeStore)) {
        return false;
      }
      break;
    case Scalar::Float32:
      if (rhsType.isFloatish()) {
        if (!f.encoder().writeOp(MozOp::F32TeeStore)) {
          return false;
        }
      } else {
        if (!f.encoder().writeOp(MozOp::F64TeeStoreF32)) {
          return false;
        }
      }
      break;
    case Scalar::Float64:
      if (rhsType.isFloatish()) {
        if (!f.encoder().writeOp(MozOp::F32TeeStoreF64)) {
          return false;
        }
      } else {
        if (!f.encoder().writeOp(MozOp::F64TeeStore)) {
          return false;
        }
      }
      break;
    default:
      MOZ_CRASH("unexpected scalar type");
  }

  if (!WriteArrayAccessFlags(f, viewType)) {
    return false;
  }
||||||| merged common ancestors
        Type pointerType;
        if (!CheckExpr(f, pointerNode, &pointerType)) {
            return false;
        }
        if (!pointerType.isInt()) {
            return f.failf(pointerNode, "%s is not a subtype of int", pointerType.toChars());
        }
    }

    // Don't generate the mask op if there is no need for it which could happen
    // for a shift of zero.
    if (mask != NoMask) {
        return f.writeInt32Lit(mask) &&
               f.encoder().writeOp(Op::I32And);
    }
=======
 public:
  bool init() {
    asmJSMetadata_ = cx_->new_<AsmJSMetadata>();
    if (!asmJSMetadata_) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *type = rhsType;
  return true;
}

static bool CheckAssignName(FunctionValidator& f, ParseNode* lhs,
                            ParseNode* rhs, Type* type) {
  RootedPropertyName name(f.cx(), lhs->as<NameNode>().name());
||||||| merged common ancestors
    return true;
}

static bool
WriteArrayAccessFlags(FunctionValidator& f, Scalar::Type viewType)
{
    // asm.js only has naturally-aligned accesses.
    size_t align = TypedArrayElemSize(viewType);
    MOZ_ASSERT(IsPowerOfTwo(align));
    if (!f.encoder().writeFixedU8(CeilingLog2(align))) {
        return false;
    }
=======
    asmJSMetadata_->toStringStart =
        moduleFunctionNode_->funbox()->toStringStart;
    asmJSMetadata_->srcStart = moduleFunctionNode_->body()->pn_pos.begin;
    asmJSMetadata_->strict = parser_.pc_->sc()->strict() &&
                             !parser_.pc_->sc()->hasExplicitUseStrict();
    asmJSMetadata_->scriptSource.reset(parser_.ss);

    if (!addStandardLibraryMathInfo()) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (const FunctionValidator::Local* lhsVar = f.lookupLocal(name)) {
    Type rhsType;
    if (!CheckExpr(f, rhs, &rhsType)) {
      return false;
||||||| merged common ancestors
    // asm.js doesn't have constant offsets, so just encode a 0.
    if (!f.encoder().writeVarU32(0)) {
        return false;
=======
    if (!initDummyFunction()) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (!f.encoder().writeOp(Op::TeeLocal)) {
      return false;
    }
    if (!f.encoder().writeVarU32(lhsVar->slot)) {
      return false;
||||||| merged common ancestors
    return true;
}

static bool
CheckLoadArray(FunctionValidator& f, ParseNode* elem, Type* type)
{
    Scalar::Type viewType;

    if (!CheckArrayAccess(f, ElemBase(elem), ElemIndex(elem), &viewType)) {
        return false;
    }

    switch (viewType) {
      case Scalar::Int8:    if (!f.encoder().writeOp(Op::I32Load8S))  return false; break;
      case Scalar::Uint8:   if (!f.encoder().writeOp(Op::I32Load8U))  return false; break;
      case Scalar::Int16:   if (!f.encoder().writeOp(Op::I32Load16S)) return false; break;
      case Scalar::Uint16:  if (!f.encoder().writeOp(Op::I32Load16U)) return false; break;
      case Scalar::Uint32:
      case Scalar::Int32:   if (!f.encoder().writeOp(Op::I32Load))    return false; break;
      case Scalar::Float32: if (!f.encoder().writeOp(Op::F32Load))    return false; break;
      case Scalar::Float64: if (!f.encoder().writeOp(Op::F64Load))    return false; break;
      default: MOZ_CRASH("unexpected scalar type");
    }

    switch (viewType) {
      case Scalar::Int8:
      case Scalar::Int16:
      case Scalar::Int32:
      case Scalar::Uint8:
      case Scalar::Uint16:
      case Scalar::Uint32:
        *type = Type::Intish;
        break;
      case Scalar::Float32:
        *type = Type::MaybeFloat;
        break;
      case Scalar::Float64:
        *type = Type::MaybeDouble;
        break;
      default: MOZ_CRASH("Unexpected array type");
=======
    return true;
  }

  AsmJSParser<Unit>& parser() const { return parser_; }

  auto& tokenStream() const { return parser_.tokenStream; }

 public:
  bool addFuncDef(PropertyName* name, uint32_t firstUse, FuncType&& sig,
                  Func** func) {
    uint32_t sigIndex;
    if (!declareSig(std::move(sig), &sigIndex)) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (!(rhsType <= lhsVar->type)) {
      return f.failf(lhs, "%s is not a subtype of %s", rhsType.toChars(),
                     lhsVar->type.toChars());
||||||| merged common ancestors
    if (!WriteArrayAccessFlags(f, viewType)) {
        return false;
=======
    uint32_t funcDefIndex = funcDefs_.length();
    if (funcDefIndex >= MaxFuncs) {
      return failCurrentOffset("too many functions");
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    *type = rhsType;
||||||| merged common ancestors

=======

    Global* global = validationLifo_.new_<Global>(Global::Function);
    if (!global) {
      return false;
    }
    new (&global->u.funcDefIndex_) uint32_t(funcDefIndex);
    if (!globalMap_.putNew(name, global)) {
      return false;
    }
    if (!funcDefs_.emplaceBack(name, sigIndex, firstUse, funcDefIndex)) {
      return false;
    }
    *func = &funcDefs_.back();
>>>>>>> upstream-releases
    return true;
<<<<<<< HEAD
  }
||||||| merged common ancestors
}
=======
  }
  bool declareFuncPtrTable(FuncType&& sig, PropertyName* name,
                           uint32_t firstUse, uint32_t mask,
                           uint32_t* tableIndex) {
    if (mask > MaxTableInitialLength) {
      return failCurrentOffset("function pointer table too big");
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (const ModuleValidator::Global* global = f.lookupGlobal(name)) {
    if (global->which() != ModuleValidator::Global::Variable) {
      return f.failName(lhs, "'%s' is not a mutable variable", name);
||||||| merged common ancestors
static bool
CheckStoreArray(FunctionValidator& f, ParseNode* lhs, ParseNode* rhs, Type* type)
{
    Scalar::Type viewType;
    if (!CheckArrayAccess(f, ElemBase(lhs), ElemIndex(lhs), &viewType)) {
        return false;
=======
    MOZ_ASSERT(env_.tables.length() == tables_.length());
    *tableIndex = env_.tables.length();

    uint32_t sigIndex;
    if (!newSig(std::move(sig), &sigIndex)) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    Type rhsType;
    if (!CheckExpr(f, rhs, &rhsType)) {
      return false;
||||||| merged common ancestors
    Type rhsType;
    if (!CheckExpr(f, rhs, &rhsType)) {
        return false;
=======
    MOZ_ASSERT(sigIndex >= env_.asmJSSigToTableIndex.length());
    if (!env_.asmJSSigToTableIndex.resize(sigIndex + 1)) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    Type globType = global->varOrConstType();
    if (!(rhsType <= globType)) {
      return f.failf(lhs, "%s is not a subtype of %s", rhsType.toChars(),
                     globType.toChars());
||||||| merged common ancestors
    switch (viewType) {
      case Scalar::Int8:
      case Scalar::Int16:
      case Scalar::Int32:
      case Scalar::Uint8:
      case Scalar::Uint16:
      case Scalar::Uint32:
        if (!rhsType.isIntish()) {
            return f.failf(lhs, "%s is not a subtype of intish", rhsType.toChars());
        }
        break;
      case Scalar::Float32:
        if (!rhsType.isMaybeDouble() && !rhsType.isFloatish()) {
            return f.failf(lhs, "%s is not a subtype of double? or floatish", rhsType.toChars());
        }
        break;
      case Scalar::Float64:
        if (!rhsType.isMaybeFloat() && !rhsType.isMaybeDouble()) {
            return f.failf(lhs, "%s is not a subtype of float? or double?", rhsType.toChars());
        }
        break;
      default:
        MOZ_CRASH("Unexpected view type");
=======
    env_.asmJSSigToTableIndex[sigIndex] = env_.tables.length();
    if (!env_.tables.emplaceBack(TableKind::AsmJS, Limits(mask + 1))) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (!f.encoder().writeOp(MozOp::TeeGlobal)) {
      return false;
||||||| merged common ancestors

    switch (viewType) {
      case Scalar::Int8:
      case Scalar::Uint8:
        if (!f.encoder().writeOp(MozOp::I32TeeStore8)) {
            return false;
        }
        break;
      case Scalar::Int16:
      case Scalar::Uint16:
        if (!f.encoder().writeOp(MozOp::I32TeeStore16)) {
            return false;
        }
        break;
      case Scalar::Int32:
      case Scalar::Uint32:
        if (!f.encoder().writeOp(MozOp::I32TeeStore)) {
            return false;
        }
        break;
      case Scalar::Float32:
        if (rhsType.isFloatish()) {
            if (!f.encoder().writeOp(MozOp::F32TeeStore)) {
                return false;
            }
        } else {
            if (!f.encoder().writeOp(MozOp::F64TeeStoreF32)) {
                return false;
            }
        }
        break;
      case Scalar::Float64:
        if (rhsType.isFloatish()) {
            if (!f.encoder().writeOp(MozOp::F32TeeStoreF64)) {
                return false;
            }
        } else {
            if (!f.encoder().writeOp(MozOp::F64TeeStore)) {
                return false;
            }
        }
        break;
      default: MOZ_CRASH("unexpected scalar type");
=======

    Global* global = validationLifo_.new_<Global>(Global::Table);
    if (!global) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (!f.encoder().writeVarU32(global->varOrConstIndex())) {
      return false;
||||||| merged common ancestors

    if (!WriteArrayAccessFlags(f, viewType)) {
        return false;
=======

    new (&global->u.tableIndex_) uint32_t(*tableIndex);
    if (!globalMap_.putNew(name, global)) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    *type = rhsType;
    return true;
  }

  return f.failName(lhs, "'%s' not found in local or asm.js module scope",
                    name);
}
||||||| merged common ancestors
    *type = rhsType;
    return true;
}
=======
    Table* t = validationLifo_.new_<Table>(sigIndex, name, firstUse, mask);
    return t && tables_.append(t);
  }
  bool declareImport(PropertyName* name, FuncType&& sig, unsigned ffiIndex,
                     uint32_t* importIndex) {
    FuncImportMap::AddPtr p =
        funcImportMap_.lookupForAdd(NamedSig::Lookup(name, sig));
    if (p) {
      *importIndex = p->value();
      return true;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckAssign(FunctionValidator& f, ParseNode* assign, Type* type) {
  MOZ_ASSERT(assign->isKind(ParseNodeKind::Assign));
||||||| merged common ancestors
static bool
CheckAssignName(FunctionValidator& f, ParseNode* lhs, ParseNode* rhs, Type* type)
{
    RootedPropertyName name(f.cx(), lhs->as<NameNode>().name());
=======
    *importIndex = funcImportMap_.count();
    MOZ_ASSERT(*importIndex == asmJSMetadata_->asmJSImports.length());
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* lhs = BinaryLeft(assign);
  ParseNode* rhs = BinaryRight(assign);
||||||| merged common ancestors
    if (const FunctionValidator::Local* lhsVar = f.lookupLocal(name)) {
        Type rhsType;
        if (!CheckExpr(f, rhs, &rhsType)) {
            return false;
        }
=======
    if (*importIndex >= MaxImports) {
      return failCurrentOffset("too many imports");
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (lhs->getKind() == ParseNodeKind::Elem) {
    return CheckStoreArray(f, lhs, rhs, type);
  }
||||||| merged common ancestors
        if (!f.encoder().writeOp(Op::TeeLocal)) {
            return false;
        }
        if (!f.encoder().writeVarU32(lhsVar->slot)) {
            return false;
        }
=======
    if (!asmJSMetadata_->asmJSImports.emplaceBack(ffiIndex)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (lhs->getKind() == ParseNodeKind::Name) {
    return CheckAssignName(f, lhs, rhs, type);
  }
||||||| merged common ancestors
        if (!(rhsType <= lhsVar->type)) {
            return f.failf(lhs, "%s is not a subtype of %s",
                           rhsType.toChars(), lhsVar->type.toChars());
        }
        *type = rhsType;
        return true;
    }
=======
    uint32_t sigIndex;
    if (!declareSig(std::move(sig), &sigIndex)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return f.fail(
      assign,
      "left-hand side of assignment must be a variable or array access");
}
||||||| merged common ancestors
    if (const ModuleValidator::Global* global = f.lookupGlobal(name)) {
        if (global->which() != ModuleValidator::Global::Variable) {
            return f.failName(lhs, "'%s' is not a mutable variable", name);
        }
=======
    return funcImportMap_.add(p, NamedSig(name, sigIndex, env_.types),
                              *importIndex);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckMathIMul(FunctionValidator& f, ParseNode* call, Type* type) {
  if (CallArgListLength(call) != 2) {
    return f.fail(call, "Math.imul must be passed 2 arguments");
  }
||||||| merged common ancestors
        Type rhsType;
        if (!CheckExpr(f, rhs, &rhsType)) {
            return false;
        }
=======
  // Error handling.
  bool failCurrentOffset(const char* str) {
    return failOffset(tokenStream().anyCharsAccess().currentToken().pos.begin,
                      str);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* lhs = CallArgList(call);
  ParseNode* rhs = NextNode(lhs);
||||||| merged common ancestors
        Type globType = global->varOrConstType();
        if (!(rhsType <= globType)) {
            return f.failf(lhs, "%s is not a subtype of %s", rhsType.toChars(), globType.toChars());
        }
        if (!f.encoder().writeOp(MozOp::TeeGlobal)) {
            return false;
        }
        if (!f.encoder().writeVarU32(global->varOrConstIndex())) {
            return false;
        }
=======
  SharedModule finish() {
    MOZ_ASSERT(env_.funcTypes.empty());
    if (!env_.funcTypes.resize(funcImportMap_.count() + funcDefs_.length())) {
      return nullptr;
    }
    for (FuncImportMap::Range r = funcImportMap_.all(); !r.empty();
         r.popFront()) {
      uint32_t funcIndex = r.front().value();
      MOZ_ASSERT(!env_.funcTypes[funcIndex]);
      env_.funcTypes[funcIndex] =
          &env_.types[r.front().key().sigIndex()].funcType();
    }
    for (const Func& func : funcDefs_) {
      uint32_t funcIndex = funcImportMap_.count() + func.funcDefIndex();
      MOZ_ASSERT(!env_.funcTypes[funcIndex]);
      env_.funcTypes[funcIndex] = &env_.types[func.sigIndex()].funcType();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type lhsType;
  if (!CheckExpr(f, lhs, &lhsType)) {
    return false;
  }

  Type rhsType;
  if (!CheckExpr(f, rhs, &rhsType)) {
    return false;
  }

  if (!lhsType.isIntish()) {
    return f.failf(lhs, "%s is not a subtype of intish", lhsType.toChars());
  }
  if (!rhsType.isIntish()) {
    return f.failf(rhs, "%s is not a subtype of intish", rhsType.toChars());
  }
||||||| merged common ancestors
        *type = rhsType;
        return true;
    }
=======
    if (!env_.funcImportGlobalDataOffsets.resize(funcImportMap_.count())) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *type = Type::Signed;
  return f.encoder().writeOp(Op::I32Mul);
}
||||||| merged common ancestors
    return f.failName(lhs, "'%s' not found in local or asm.js module scope", name);
}
=======
    MOZ_ASSERT(asmJSMetadata_->asmJSFuncNames.empty());
    if (!asmJSMetadata_->asmJSFuncNames.resize(funcImportMap_.count())) {
      return nullptr;
    }
    for (const Func& func : funcDefs_) {
      CacheableChars funcName = StringToNewUTF8CharsZ(cx_, *func.name());
      if (!funcName ||
          !asmJSMetadata_->asmJSFuncNames.emplaceBack(std::move(funcName))) {
        return nullptr;
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckMathClz32(FunctionValidator& f, ParseNode* call, Type* type) {
  if (CallArgListLength(call) != 1) {
    return f.fail(call, "Math.clz32 must be passed 1 argument");
  }
||||||| merged common ancestors
static bool
CheckAssign(FunctionValidator& f, ParseNode* assign, Type* type)
{
    MOZ_ASSERT(assign->isKind(ParseNodeKind::Assign));
=======
    uint32_t endBeforeCurly =
        tokenStream().anyCharsAccess().currentToken().pos.end;
    asmJSMetadata_->srcLength = endBeforeCurly - asmJSMetadata_->srcStart;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* arg = CallArgList(call);
||||||| merged common ancestors
    ParseNode* lhs = BinaryLeft(assign);
    ParseNode* rhs = BinaryRight(assign);
=======
    TokenPos pos;
    MOZ_ALWAYS_TRUE(
        tokenStream().peekTokenPos(&pos, TokenStreamShared::SlashIsRegExp));
    uint32_t endAfterCurly = pos.end;
    asmJSMetadata_->srcLengthWithRightBrace =
        endAfterCurly - asmJSMetadata_->srcStart;
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type argType;
  if (!CheckExpr(f, arg, &argType)) {
    return false;
  }
||||||| merged common ancestors
    if (lhs->getKind() == ParseNodeKind::Elem) {
        return CheckStoreArray(f, lhs, rhs, type);
    }
=======
    ScriptedCaller scriptedCaller;
    if (parser_.ss->filename()) {
      scriptedCaller.line = 0;  // unused
      scriptedCaller.filename = DuplicateString(parser_.ss->filename());
      if (!scriptedCaller.filename) {
        return nullptr;
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!argType.isIntish()) {
    return f.failf(arg, "%s is not a subtype of intish", argType.toChars());
  }
||||||| merged common ancestors
    if (lhs->getKind() == ParseNodeKind::Name) {
        return CheckAssignName(f, lhs, rhs, type);
    }
=======
    SharedCompileArgs args = CompileArgs::build(cx_, std::move(scriptedCaller));
    if (!args) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *type = Type::Fixnum;
  return f.encoder().writeOp(Op::I32Clz);
}

static bool CheckMathAbs(FunctionValidator& f, ParseNode* call, Type* type) {
  if (CallArgListLength(call) != 1) {
    return f.fail(call, "Math.abs must be passed 1 argument");
  }
||||||| merged common ancestors
    return f.fail(assign, "left-hand side of assignment must be a variable or array access");
}

static bool
CheckMathIMul(FunctionValidator& f, ParseNode* call, Type* type)
{
    if (CallArgListLength(call) != 2) {
        return f.fail(call, "Math.imul must be passed 2 arguments");
    }
=======
    uint32_t codeSectionSize = 0;
    for (const Func& func : funcDefs_) {
      codeSectionSize += func.bytes().length();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* arg = CallArgList(call);
||||||| merged common ancestors
    ParseNode* lhs = CallArgList(call);
    ParseNode* rhs = NextNode(lhs);
=======
    env_.codeSection.emplace();
    env_.codeSection->start = 0;
    env_.codeSection->size = codeSectionSize;
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type argType;
  if (!CheckExpr(f, arg, &argType)) {
    return false;
  }
||||||| merged common ancestors
    Type lhsType;
    if (!CheckExpr(f, lhs, &lhsType)) {
        return false;
    }
=======
    // asm.js does not have any wasm bytecode to save; view-source is
    // provided through the ScriptSource.
    SharedBytes bytes = cx_->new_<ShareableBytes>();
    if (!bytes) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (argType.isSigned()) {
    *type = Type::Unsigned;
    return f.encoder().writeOp(MozOp::I32Abs);
  }
||||||| merged common ancestors
    Type rhsType;
    if (!CheckExpr(f, rhs, &rhsType)) {
        return false;
    }
=======
    ModuleGenerator mg(*args, &env_, nullptr, nullptr);
    if (!mg.init(asmJSMetadata_.get())) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (argType.isMaybeDouble()) {
    *type = Type::Double;
    return f.encoder().writeOp(Op::F64Abs);
  }
||||||| merged common ancestors
    if (!lhsType.isIntish()) {
        return f.failf(lhs, "%s is not a subtype of intish", lhsType.toChars());
    }
    if (!rhsType.isIntish()) {
        return f.failf(rhs, "%s is not a subtype of intish", rhsType.toChars());
    }
=======
    for (Func& func : funcDefs_) {
      if (!mg.compileFuncDef(funcImportMap_.count() + func.funcDefIndex(),
                             func.line(), func.bytes().begin(),
                             func.bytes().end(),
                             std::move(func.callSiteLineNums()))) {
        return nullptr;
      }
    }

    if (!mg.finishFuncDefs()) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (argType.isMaybeFloat()) {
    *type = Type::Floatish;
    return f.encoder().writeOp(Op::F32Abs);
  }

  return f.failf(call, "%s is not a subtype of signed, float? or double?",
                 argType.toChars());
}
||||||| merged common ancestors
    *type = Type::Signed;
    return f.encoder().writeOp(Op::I32Mul);
}
=======
    return mg.finishModule(*bytes);
  }
};
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckMathSqrt(FunctionValidator& f, ParseNode* call, Type* type) {
  if (CallArgListLength(call) != 1) {
    return f.fail(call, "Math.sqrt must be passed 1 argument");
  }
||||||| merged common ancestors
static bool
CheckMathClz32(FunctionValidator& f, ParseNode* call, Type* type)
{
    if (CallArgListLength(call) != 1) {
        return f.fail(call, "Math.clz32 must be passed 1 argument");
    }
=======
/*****************************************************************************/
// Numeric literal utilities
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* arg = CallArgList(call);
||||||| merged common ancestors
    ParseNode* arg = CallArgList(call);
=======
static bool IsNumericNonFloatLiteral(ParseNode* pn) {
  // Note: '-' is never rolled into the number; numbers are always positive
  // and negations must be applied manually.
  return pn->isKind(ParseNodeKind::NumberExpr) ||
         (pn->isKind(ParseNodeKind::NegExpr) &&
          UnaryKid(pn)->isKind(ParseNodeKind::NumberExpr));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type argType;
  if (!CheckExpr(f, arg, &argType)) {
    return false;
  }
||||||| merged common ancestors
    Type argType;
    if (!CheckExpr(f, arg, &argType)) {
        return false;
    }
=======
static bool IsCallToGlobal(ModuleValidatorShared& m, ParseNode* pn,
                           const ModuleValidatorShared::Global** global) {
  if (!pn->isKind(ParseNodeKind::CallExpr)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (argType.isMaybeDouble()) {
    *type = Type::Double;
    return f.encoder().writeOp(Op::F64Sqrt);
  }
||||||| merged common ancestors
    if (!argType.isIntish()) {
        return f.failf(arg, "%s is not a subtype of intish", argType.toChars());
    }
=======
  ParseNode* callee = CallCallee(pn);
  if (!callee->isKind(ParseNodeKind::Name)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (argType.isMaybeFloat()) {
    *type = Type::Floatish;
    return f.encoder().writeOp(Op::F32Sqrt);
  }
||||||| merged common ancestors
    *type = Type::Fixnum;
    return f.encoder().writeOp(Op::I32Clz);
}
=======
  *global = m.lookupGlobal(callee->as<NameNode>().name());
  return !!*global;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return f.failf(call, "%s is neither a subtype of double? nor float?",
                 argType.toChars());
}
||||||| merged common ancestors
static bool
CheckMathAbs(FunctionValidator& f, ParseNode* call, Type* type)
{
    if (CallArgListLength(call) != 1) {
        return f.fail(call, "Math.abs must be passed 1 argument");
    }
=======
static bool IsCoercionCall(ModuleValidatorShared& m, ParseNode* pn,
                           Type* coerceTo, ParseNode** coercedExpr) {
  const ModuleValidatorShared::Global* global;
  if (!IsCallToGlobal(m, pn, &global)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckMathMinMax(FunctionValidator& f, ParseNode* callNode,
                            bool isMax, Type* type) {
  if (CallArgListLength(callNode) < 2) {
    return f.fail(callNode, "Math.min/max must be passed at least 2 arguments");
  }
||||||| merged common ancestors
    ParseNode* arg = CallArgList(call);
=======
  if (CallArgListLength(pn) != 1) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* firstArg = CallArgList(callNode);
  Type firstType;
  if (!CheckExpr(f, firstArg, &firstType)) {
    return false;
  }

  Op op = Op::Limit;
  MozOp mozOp = MozOp::Limit;
  if (firstType.isMaybeDouble()) {
    *type = Type::Double;
    firstType = Type::MaybeDouble;
    op = isMax ? Op::F64Max : Op::F64Min;
  } else if (firstType.isMaybeFloat()) {
    *type = Type::Float;
    firstType = Type::MaybeFloat;
    op = isMax ? Op::F32Max : Op::F32Min;
  } else if (firstType.isSigned()) {
    *type = Type::Signed;
    firstType = Type::Signed;
    mozOp = isMax ? MozOp::I32Max : MozOp::I32Min;
  } else {
    return f.failf(firstArg, "%s is not a subtype of double?, float? or signed",
                   firstType.toChars());
  }
||||||| merged common ancestors
    Type argType;
    if (!CheckExpr(f, arg, &argType)) {
        return false;
    }
=======
  if (coercedExpr) {
    *coercedExpr = CallArgList(pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  unsigned numArgs = CallArgListLength(callNode);
  ParseNode* nextArg = NextNode(firstArg);
  for (unsigned i = 1; i < numArgs; i++, nextArg = NextNode(nextArg)) {
    Type nextType;
    if (!CheckExpr(f, nextArg, &nextType)) {
      return false;
    }
    if (!(nextType <= firstType)) {
      return f.failf(nextArg, "%s is not a subtype of %s", nextType.toChars(),
                     firstType.toChars());
    }
||||||| merged common ancestors
    if (argType.isSigned()) {
        *type = Type::Unsigned;
        return f.encoder().writeOp(MozOp::I32Abs);
    }

    if (argType.isMaybeDouble()) {
        *type = Type::Double;
        return f.encoder().writeOp(Op::F64Abs);
    }
=======
  if (global->isMathFunction() &&
      global->mathBuiltinFunction() == AsmJSMathBuiltin_fround) {
    *coerceTo = Type::Float;
    return true;
  }

  return false;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (op != Op::Limit) {
      if (!f.encoder().writeOp(op)) {
        return false;
      }
    } else {
      if (!f.encoder().writeOp(mozOp)) {
        return false;
      }
    }
  }
||||||| merged common ancestors
    if (argType.isMaybeFloat()) {
        *type = Type::Floatish;
        return f.encoder().writeOp(Op::F32Abs);
    }
=======
static bool IsFloatLiteral(ModuleValidatorShared& m, ParseNode* pn) {
  ParseNode* coercedExpr;
  Type coerceTo;
  if (!IsCoercionCall(m, pn, &coerceTo, &coercedExpr)) {
    return false;
  }
  // Don't fold into || to avoid clang/memcheck bug (bug 1077031).
  if (!coerceTo.isFloat()) {
    return false;
  }
  return IsNumericNonFloatLiteral(coercedExpr);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return f.failf(call, "%s is not a subtype of signed, float? or double?", argType.toChars());
=======
static bool IsNumericLiteral(ModuleValidatorShared& m, ParseNode* pn) {
  return IsNumericNonFloatLiteral(pn) || IsFloatLiteral(m, pn);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*CheckArgType)(FunctionValidator& f, ParseNode* argNode,
                             Type type);
||||||| merged common ancestors
static bool
CheckMathSqrt(FunctionValidator& f, ParseNode* call, Type* type)
{
    if (CallArgListLength(call) != 1) {
        return f.fail(call, "Math.sqrt must be passed 1 argument");
    }
=======
// The JS grammar treats -42 as -(42) (i.e., with separate grammar
// productions) for the unary - and literal 42). However, the asm.js spec
// recognizes -42 (modulo parens, so -(42) and -((42))) as a single literal
// so fold the two potential parse nodes into a single double value.
static double ExtractNumericNonFloatValue(ParseNode* pn,
                                          ParseNode** out = nullptr) {
  MOZ_ASSERT(IsNumericNonFloatLiteral(pn));

  if (pn->isKind(ParseNodeKind::NegExpr)) {
    pn = UnaryKid(pn);
    if (out) {
      *out = pn;
    }
    return -NumberNodeValue(pn);
  }

  return NumberNodeValue(pn);
}

static NumLit ExtractNumericLiteral(ModuleValidatorShared& m, ParseNode* pn) {
  MOZ_ASSERT(IsNumericLiteral(m, pn));

  if (pn->isKind(ParseNodeKind::CallExpr)) {
    // Float literals are explicitly coerced and thus the coerced literal may be
    // any valid (non-float) numeric literal.
    MOZ_ASSERT(CallArgListLength(pn) == 1);
    pn = CallArgList(pn);
    double d = ExtractNumericNonFloatValue(pn);
    return NumLit(NumLit::Float, DoubleValue(d));
  }

  double d = ExtractNumericNonFloatValue(pn, &pn);

  // The asm.js spec syntactically distinguishes any literal containing a
  // decimal point or the literal -0 as having double type.
  if (NumberNodeHasFrac(pn) || IsNegativeZero(d)) {
    return NumLit(NumLit::Double, DoubleValue(d));
  }

  // The syntactic checks above rule out these double values.
  MOZ_ASSERT(!IsNegativeZero(d));
  MOZ_ASSERT(!IsNaN(d));

  // Although doubles can only *precisely* represent 53-bit integers, they
  // can *imprecisely* represent integers much bigger than an int64_t.
  // Furthermore, d may be inf or -inf. In both cases, casting to an int64_t
  // is undefined, so test against the integer bounds using doubles.
  if (d < double(INT32_MIN) || d > double(UINT32_MAX)) {
    return NumLit(NumLit::OutOfRangeInt, UndefinedValue());
  }

  // With the above syntactic and range limitations, d is definitely an
  // integer in the range [INT32_MIN, UINT32_MAX] range.
  int64_t i64 = int64_t(d);
  if (i64 >= 0) {
    if (i64 <= INT32_MAX) {
      return NumLit(NumLit::Fixnum, Int32Value(i64));
    }
    MOZ_ASSERT(i64 <= UINT32_MAX);
    return NumLit(NumLit::BigUnsigned, Int32Value(uint32_t(i64)));
  }
  MOZ_ASSERT(i64 >= INT32_MIN);
  return NumLit(NumLit::NegativeInt, Int32Value(i64));
}

static inline bool IsLiteralInt(const NumLit& lit, uint32_t* u32) {
  switch (lit.which()) {
    case NumLit::Fixnum:
    case NumLit::BigUnsigned:
    case NumLit::NegativeInt:
      *u32 = lit.toUint32();
      return true;
    case NumLit::Double:
    case NumLit::Float:
    case NumLit::OutOfRangeInt:
      return false;
  }
  MOZ_MAKE_COMPILER_ASSUME_IS_UNREACHABLE("Bad literal type");
}

static inline bool IsLiteralInt(ModuleValidatorShared& m, ParseNode* pn,
                                uint32_t* u32) {
  return IsNumericLiteral(m, pn) &&
         IsLiteralInt(ExtractNumericLiteral(m, pn), u32);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <CheckArgType checkArg>
static bool CheckCallArgs(FunctionValidator& f, ParseNode* callNode,
                          ValTypeVector* args) {
  ParseNode* argNode = CallArgList(callNode);
  for (unsigned i = 0; i < CallArgListLength(callNode);
       i++, argNode = NextNode(argNode)) {
    Type type;
    if (!CheckExpr(f, argNode, &type)) {
      return false;
    }
||||||| merged common ancestors
    ParseNode* arg = CallArgList(call);
=======
/*****************************************************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!checkArg(f, argNode, type)) {
      return false;
    }
||||||| merged common ancestors
    Type argType;
    if (!CheckExpr(f, arg, &argType)) {
        return false;
    }
=======
namespace {
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!args->append(Type::canonicalize(type).canonicalToValType())) {
      return false;
    }
  }
  return true;
}
||||||| merged common ancestors
    if (argType.isMaybeDouble()) {
        *type = Type::Double;
        return f.encoder().writeOp(Op::F64Sqrt);
    }
=======
typedef Vector<PropertyName*, 4, SystemAllocPolicy> LabelVector;
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckSignatureAgainstExisting(ModuleValidator& m, ParseNode* usepn,
                                          const FuncType& sig,
                                          const FuncType& existing) {
  if (sig.args().length() != existing.args().length()) {
    return m.failf(usepn,
                   "incompatible number of arguments (%zu"
                   " here vs. %zu before)",
                   sig.args().length(), existing.args().length());
  }

  for (unsigned i = 0; i < sig.args().length(); i++) {
    if (sig.arg(i) != existing.arg(i)) {
      return m.failf(
          usepn, "incompatible type for argument %u: (%s here vs. %s before)",
          i, ToCString(sig.arg(i)), ToCString(existing.arg(i)));
||||||| merged common ancestors
    if (argType.isMaybeFloat()) {
        *type = Type::Floatish;
        return f.encoder().writeOp(Op::F32Sqrt);
=======
class MOZ_STACK_CLASS FunctionValidatorShared {
 public:
  struct Local {
    Type type;
    unsigned slot;
    Local(Type t, unsigned slot) : type(t), slot(slot) {
      MOZ_ASSERT(type.isCanonicalValType());
    }
  };

 protected:
  using LocalMap = HashMap<PropertyName*, Local>;
  using LabelMap = HashMap<PropertyName*, uint32_t>;

  // This is also a ModuleValidator<Unit>& after the appropriate static_cast<>.
  ModuleValidatorShared& m_;

  ParseNode* fn_;
  Bytes bytes_;
  Encoder encoder_;
  Uint32Vector callSiteLineNums_;
  LocalMap locals_;

  // Labels
  LabelMap breakLabels_;
  LabelMap continueLabels_;
  Uint32Vector breakableStack_;
  Uint32Vector continuableStack_;
  uint32_t blockDepth_;

  bool hasAlreadyReturned_;
  ExprType ret_;

 private:
  FunctionValidatorShared(ModuleValidatorShared& m, ParseNode* fn,
                          JSContext* cx)
      : m_(m),
        fn_(fn),
        encoder_(bytes_),
        locals_(cx),
        breakLabels_(cx),
        continueLabels_(cx),
        blockDepth_(0),
        hasAlreadyReturned_(false),
        ret_(ExprType::Limit) {}

 protected:
  template <typename Unit>
  FunctionValidatorShared(ModuleValidator<Unit>& m, ParseNode* fn,
                          JSContext* cx)
      : FunctionValidatorShared(static_cast<ModuleValidatorShared&>(m), fn,
                                cx) {}

 public:
  ModuleValidatorShared& m() const { return m_; }

  JSContext* cx() const { return m_.cx(); }
  ParseNode* fn() const { return fn_; }

  void define(ModuleValidatorShared::Func* func, unsigned line) {
    MOZ_ASSERT(!blockDepth_);
    MOZ_ASSERT(breakableStack_.empty());
    MOZ_ASSERT(continuableStack_.empty());
    MOZ_ASSERT(breakLabels_.empty());
    MOZ_ASSERT(continueLabels_.empty());
    func->define(fn_, line, std::move(bytes_), std::move(callSiteLineNums_));
  }

  bool fail(ParseNode* pn, const char* str) { return m_.fail(pn, str); }

  bool failf(ParseNode* pn, const char* fmt, ...) MOZ_FORMAT_PRINTF(3, 4) {
    va_list ap;
    va_start(ap, fmt);
    m_.failfVAOffset(pn->pn_pos.begin, fmt, ap);
    va_end(ap);
    return false;
  }

  bool failName(ParseNode* pn, const char* fmt, PropertyName* name) {
    return m_.failName(pn, fmt, name);
  }

  /***************************************************** Local scope setup */

  bool addLocal(ParseNode* pn, PropertyName* name, Type type) {
    LocalMap::AddPtr p = locals_.lookupForAdd(name);
    if (p) {
      return failName(pn, "duplicate local name '%s' not allowed", name);
    }
    return locals_.add(p, name, Local(type, locals_.count()));
  }

  /****************************** For consistency of returns in a function */

  bool hasAlreadyReturned() const { return hasAlreadyReturned_; }

  ExprType returnedType() const { return ret_; }

  void setReturnedType(ExprType ret) {
    ret_ = ret;
    hasAlreadyReturned_ = true;
  }

  /**************************************************************** Labels */
 private:
  bool writeBr(uint32_t absolute, Op op = Op::Br) {
    MOZ_ASSERT(op == Op::Br || op == Op::BrIf);
    MOZ_ASSERT(absolute < blockDepth_);
    return encoder().writeOp(op) &&
           encoder().writeVarU32(blockDepth_ - 1 - absolute);
  }
  void removeLabel(PropertyName* label, LabelMap* map) {
    LabelMap::Ptr p = map->lookup(label);
    MOZ_ASSERT(p);
    map->remove(p);
  }

 public:
  bool pushBreakableBlock() {
    return encoder().writeOp(Op::Block) &&
           encoder().writeFixedU8(uint8_t(ExprType::Void)) &&
           breakableStack_.append(blockDepth_++);
  }
  bool popBreakableBlock() {
    MOZ_ALWAYS_TRUE(breakableStack_.popCopy() == --blockDepth_);
    return encoder().writeOp(Op::End);
  }

  bool pushUnbreakableBlock(const LabelVector* labels = nullptr) {
    if (labels) {
      for (PropertyName* label : *labels) {
        if (!breakLabels_.putNew(label, blockDepth_)) {
          return false;
        }
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

  if (sig.ret() != existing.ret()) {
    return m.failf(usepn, "%s incompatible with previous return of type %s",
                   ToCString(sig.ret()), ToCString(existing.ret()));
  }

  MOZ_ASSERT(sig == existing);
  return true;
}

static bool CheckFunctionSignature(ModuleValidator& m, ParseNode* usepn,
                                   FuncType&& sig, PropertyName* name,
                                   ModuleValidator::Func** func) {
  if (sig.args().length() > MaxParams) {
    return m.failf(usepn, "too many parameters");
  }
||||||| merged common ancestors

    return f.failf(call, "%s is neither a subtype of double? nor float?", argType.toChars());
}

static bool
CheckMathMinMax(FunctionValidator& f, ParseNode* callNode, bool isMax, Type* type)
{
    if (CallArgListLength(callNode) < 2) {
        return f.fail(callNode, "Math.min/max must be passed at least 2 arguments");
    }
=======
    blockDepth_++;
    return encoder().writeOp(Op::Block) &&
           encoder().writeFixedU8(uint8_t(ExprType::Void));
  }
  bool popUnbreakableBlock(const LabelVector* labels = nullptr) {
    if (labels) {
      for (PropertyName* label : *labels) {
        removeLabel(label, &breakLabels_);
      }
    }
    --blockDepth_;
    return encoder().writeOp(Op::End);
  }

  bool pushContinuableBlock() {
    return encoder().writeOp(Op::Block) &&
           encoder().writeFixedU8(uint8_t(ExprType::Void)) &&
           continuableStack_.append(blockDepth_++);
  }
  bool popContinuableBlock() {
    MOZ_ALWAYS_TRUE(continuableStack_.popCopy() == --blockDepth_);
    return encoder().writeOp(Op::End);
  }

  bool pushLoop() {
    return encoder().writeOp(Op::Block) &&
           encoder().writeFixedU8(uint8_t(ExprType::Void)) &&
           encoder().writeOp(Op::Loop) &&
           encoder().writeFixedU8(uint8_t(ExprType::Void)) &&
           breakableStack_.append(blockDepth_++) &&
           continuableStack_.append(blockDepth_++);
  }
  bool popLoop() {
    MOZ_ALWAYS_TRUE(continuableStack_.popCopy() == --blockDepth_);
    MOZ_ALWAYS_TRUE(breakableStack_.popCopy() == --blockDepth_);
    return encoder().writeOp(Op::End) && encoder().writeOp(Op::End);
  }

  bool pushIf(size_t* typeAt) {
    ++blockDepth_;
    return encoder().writeOp(Op::If) && encoder().writePatchableFixedU7(typeAt);
  }
  bool switchToElse() {
    MOZ_ASSERT(blockDepth_ > 0);
    return encoder().writeOp(Op::Else);
  }
  void setIfType(size_t typeAt, ExprType type) {
    encoder().patchFixedU7(typeAt, uint8_t(type.code()));
  }
  bool popIf() {
    MOZ_ASSERT(blockDepth_ > 0);
    --blockDepth_;
    return encoder().writeOp(Op::End);
  }
  bool popIf(size_t typeAt, ExprType type) {
    MOZ_ASSERT(blockDepth_ > 0);
    --blockDepth_;
    if (!encoder().writeOp(Op::End)) {
      return false;
    }

    setIfType(typeAt, type);
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ModuleValidator::Func* existing = m.lookupFuncDef(name);
  if (!existing) {
    if (!CheckModuleLevelName(m, usepn, name)) {
      return false;
||||||| merged common ancestors
    ParseNode* firstArg = CallArgList(callNode);
    Type firstType;
    if (!CheckExpr(f, firstArg, &firstType)) {
        return false;
=======
  bool writeBreakIf() { return writeBr(breakableStack_.back(), Op::BrIf); }
  bool writeContinueIf() { return writeBr(continuableStack_.back(), Op::BrIf); }
  bool writeUnlabeledBreakOrContinue(bool isBreak) {
    return writeBr(isBreak ? breakableStack_.back() : continuableStack_.back());
  }
  bool writeContinue() { return writeBr(continuableStack_.back()); }

  bool addLabels(const LabelVector& labels, uint32_t relativeBreakDepth,
                 uint32_t relativeContinueDepth) {
    for (PropertyName* label : labels) {
      if (!breakLabels_.putNew(label, blockDepth_ + relativeBreakDepth)) {
        return false;
      }
      if (!continueLabels_.putNew(label, blockDepth_ + relativeContinueDepth)) {
        return false;
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    return m.addFuncDef(name, usepn->pn_pos.begin, std::move(sig), func);
  }

  const FuncTypeWithId& existingSig =
      m.env().types[existing->sigIndex()].funcType();
||||||| merged common ancestors

    Op op = Op::Limit;
    MozOp mozOp = MozOp::Limit;
    if (firstType.isMaybeDouble()) {
        *type = Type::Double;
        firstType = Type::MaybeDouble;
        op = isMax ? Op::F64Max : Op::F64Min;
    } else if (firstType.isMaybeFloat()) {
        *type = Type::Float;
        firstType = Type::MaybeFloat;
        op = isMax ? Op::F32Max : Op::F32Min;
    } else if (firstType.isSigned()) {
        *type = Type::Signed;
        firstType = Type::Signed;
        mozOp = isMax ? MozOp::I32Max : MozOp::I32Min;
    } else {
        return f.failf(firstArg, "%s is not a subtype of double?, float? or signed",
                       firstType.toChars());
    }

    unsigned numArgs = CallArgListLength(callNode);
    ParseNode* nextArg = NextNode(firstArg);
    for (unsigned i = 1; i < numArgs; i++, nextArg = NextNode(nextArg)) {
        Type nextType;
        if (!CheckExpr(f, nextArg, &nextType)) {
            return false;
        }
        if (!(nextType <= firstType)) {
            return f.failf(nextArg, "%s is not a subtype of %s", nextType.toChars(), firstType.toChars());
        }
=======
    return true;
  }
  void removeLabels(const LabelVector& labels) {
    for (PropertyName* label : labels) {
      removeLabel(label, &breakLabels_);
      removeLabel(label, &continueLabels_);
    }
  }
  bool writeLabeledBreakOrContinue(PropertyName* label, bool isBreak) {
    LabelMap& map = isBreak ? breakLabels_ : continueLabels_;
    if (LabelMap::Ptr p = map.lookup(label)) {
      return writeBr(p->value());
    }
    MOZ_CRASH("nonexistent label");
  }

  /*************************************************** Read-only interface */
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckSignatureAgainstExisting(m, usepn, sig, existingSig)) {
    return false;
  }
||||||| merged common ancestors
        if (op != Op::Limit) {
            if (!f.encoder().writeOp(op)) {
                return false;
            }
        } else {
            if (!f.encoder().writeOp(mozOp)) {
                return false;
            }
        }
    }
=======
  const Local* lookupLocal(PropertyName* name) const {
    if (auto p = locals_.lookup(name)) {
      return &p->value();
    }
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *func = existing;
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
  const ModuleValidatorShared::Global* lookupGlobal(PropertyName* name) const {
    if (locals_.has(name)) {
      return nullptr;
    }
    return m_.lookupGlobal(name);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckIsArgType(FunctionValidator& f, ParseNode* argNode,
                           Type type) {
  if (!type.isArgType()) {
    return f.failf(argNode, "%s is not a subtype of int, float, or double",
                   type.toChars());
  }
  return true;
}
||||||| merged common ancestors
typedef bool (*CheckArgType)(FunctionValidator& f, ParseNode* argNode, Type type);
=======
  size_t numLocals() const { return locals_.count(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckInternalCall(FunctionValidator& f, ParseNode* callNode,
                              PropertyName* calleeName, Type ret, Type* type) {
  MOZ_ASSERT(ret.isCanonical());
||||||| merged common ancestors
template <CheckArgType checkArg>
static bool
CheckCallArgs(FunctionValidator& f, ParseNode* callNode, ValTypeVector* args)
{
    ParseNode* argNode = CallArgList(callNode);
    for (unsigned i = 0; i < CallArgListLength(callNode); i++, argNode = NextNode(argNode)) {
        Type type;
        if (!CheckExpr(f, argNode, &type)) {
            return false;
        }
=======
  /**************************************************** Encoding interface */
>>>>>>> upstream-releases

<<<<<<< HEAD
  ValTypeVector args;
  if (!CheckCallArgs<CheckIsArgType>(f, callNode, &args)) {
    return false;
  }
||||||| merged common ancestors
        if (!checkArg(f, argNode, type)) {
            return false;
        }
=======
  Encoder& encoder() { return encoder_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  FuncType sig(std::move(args), ret.canonicalToExprType());
||||||| merged common ancestors
        if (!args->append(Type::canonicalize(type).canonicalToValType())) {
            return false;
        }
    }
    return true;
}
=======
  MOZ_MUST_USE bool writeInt32Lit(int32_t i32) {
    return encoder().writeOp(Op::I32Const) && encoder().writeVarS32(i32);
  }
  MOZ_MUST_USE bool writeConstExpr(const NumLit& lit) {
    switch (lit.which()) {
      case NumLit::Fixnum:
      case NumLit::NegativeInt:
      case NumLit::BigUnsigned:
        return writeInt32Lit(lit.toInt32());
      case NumLit::Float:
        return encoder().writeOp(Op::F32Const) &&
               encoder().writeFixedF32(lit.toFloat());
      case NumLit::Double:
        return encoder().writeOp(Op::F64Const) &&
               encoder().writeFixedF64(lit.toDouble());
      case NumLit::OutOfRangeInt:
        break;
    }
    MOZ_CRASH("unexpected literal type");
  }
};
>>>>>>> upstream-releases

<<<<<<< HEAD
  ModuleValidator::Func* callee;
  if (!CheckFunctionSignature(f.m(), callNode, std::move(sig), calleeName,
                              &callee)) {
    return false;
  }
||||||| merged common ancestors
static bool
CheckSignatureAgainstExisting(ModuleValidator& m, ParseNode* usepn, const FuncType& sig,
                              const FuncType& existing)
{
    if (sig.args().length() != existing.args().length()) {
        return m.failf(usepn, "incompatible number of arguments (%zu"
                       " here vs. %zu before)",
                       sig.args().length(), existing.args().length());
    }
=======
// Encapsulates the building of an asm bytecode function from an asm.js function
// source code, packing the asm.js code into the asm bytecode form that can
// be decoded and compiled with a FunctionCompiler.
template <typename Unit>
class MOZ_STACK_CLASS FunctionValidator : public FunctionValidatorShared {
 public:
  FunctionValidator(ModuleValidator<Unit>& m, ParseNode* fn)
      : FunctionValidatorShared(m, fn, m.cx()) {}

 public:
  ModuleValidator<Unit>& m() const {
    return static_cast<ModuleValidator<Unit>&>(FunctionValidatorShared::m());
  }

  MOZ_MUST_USE bool writeCall(ParseNode* pn, Op op) {
    if (!encoder().writeOp(op)) {
      return false;
    }

    return appendCallSiteLineNumber(pn);
  }
  MOZ_MUST_USE bool writeCall(ParseNode* pn, MozOp op) {
    if (!encoder().writeOp(op)) {
      return false;
    }

    return appendCallSiteLineNumber(pn);
  }
  MOZ_MUST_USE bool prepareCall(ParseNode* pn) {
    return appendCallSiteLineNumber(pn);
  }

 private:
  MOZ_MUST_USE bool appendCallSiteLineNumber(ParseNode* node) {
    const TokenStreamAnyChars& anyChars = m().tokenStream().anyCharsAccess();

    auto lineToken = anyChars.lineToken(node->pn_pos.begin);
    return callSiteLineNums_.append(anyChars.lineNumber(lineToken));
  }
};
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!f.writeCall(callNode, MozOp::OldCallDirect)) {
    return false;
  }
||||||| merged common ancestors
    for (unsigned i = 0; i < sig.args().length(); i++) {
        if (sig.arg(i) != existing.arg(i)) {
            return m.failf(usepn, "incompatible type for argument %u: (%s here vs. %s before)", i,
                           ToCString(sig.arg(i)), ToCString(existing.arg(i)));
        }
    }
=======
} /* anonymous namespace */
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!f.encoder().writeVarU32(callee->funcDefIndex())) {
    return false;
  }
||||||| merged common ancestors
    if (sig.ret() != existing.ret()) {
        return m.failf(usepn, "%s incompatible with previous return of type %s",
                       ToCString(sig.ret()), ToCString(existing.ret()));
    }
=======
/*****************************************************************************/
// asm.js type-checking and code-generation algorithm
>>>>>>> upstream-releases

<<<<<<< HEAD
  *type = Type::ret(ret);
  return true;
||||||| merged common ancestors
    MOZ_ASSERT(sig == existing);
    return true;
=======
static bool CheckIdentifier(ModuleValidatorShared& m, ParseNode* usepn,
                            PropertyName* name) {
  if (name == m.cx()->names().arguments || name == m.cx()->names().eval) {
    return m.failName(usepn, "'%s' is not an allowed identifier", name);
  }
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CheckFuncPtrTableAgainstExisting(ModuleValidator& m,
                                             ParseNode* usepn,
                                             PropertyName* name, FuncType&& sig,
                                             unsigned mask,
                                             uint32_t* tableIndex) {
  if (const ModuleValidator::Global* existing = m.lookupGlobal(name)) {
    if (existing->which() != ModuleValidator::Global::Table) {
      return m.failName(usepn, "'%s' is not a function-pointer table", name);
    }

    ModuleValidator::Table& table = m.table(existing->tableIndex());
    if (mask != table.mask()) {
      return m.failf(usepn, "mask does not match previous value (%u)",
                     table.mask());
    }

    if (!CheckSignatureAgainstExisting(
            m, usepn, sig, m.env().types[table.sigIndex()].funcType())) {
      return false;
    }
||||||| merged common ancestors
static bool
CheckFunctionSignature(ModuleValidator& m, ParseNode* usepn, FuncType&& sig, PropertyName* name,
                       ModuleValidator::Func** func)
{
    if (sig.args().length() > MaxParams) {
        return m.failf(usepn, "too many parameters");
    }

    ModuleValidator::Func* existing = m.lookupFuncDef(name);
    if (!existing) {
        if (!CheckModuleLevelName(m, usepn, name)) {
            return false;
        }
        return m.addFuncDef(name, usepn->pn_pos.begin, std::move(sig), func);
    }

    const FuncTypeWithId& existingSig = m.env().types[existing->sigIndex()].funcType();

    if (!CheckSignatureAgainstExisting(m, usepn, sig, existingSig)) {
        return false;
    }
=======
static bool CheckModuleLevelName(ModuleValidatorShared& m, ParseNode* usepn,
                                 PropertyName* name) {
  if (!CheckIdentifier(m, usepn, name)) {
    return false;
  }

  if (name == m.moduleFunctionName() || name == m.globalArgumentName() ||
      name == m.importArgumentName() || name == m.bufferArgumentName() ||
      m.lookupGlobal(name)) {
    return m.failName(usepn, "duplicate name '%s' not allowed", name);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    *tableIndex = existing->tableIndex();
    return true;
  }
||||||| merged common ancestors
    *func = existing;
    return true;
}
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckModuleLevelName(m, usepn, name)) {
    return false;
  }

  if (!m.declareFuncPtrTable(std::move(sig), name, usepn->pn_pos.begin, mask,
                             tableIndex)) {
    return false;
  }

  return true;
||||||| merged common ancestors
static bool
CheckIsArgType(FunctionValidator& f, ParseNode* argNode, Type type)
{
    if (!type.isArgType()) {
        return f.failf(argNode, "%s is not a subtype of int, float, or double", type.toChars());
    }
    return true;
=======
static bool CheckFunctionHead(ModuleValidatorShared& m, FunctionNode* funNode) {
  FunctionBox* funbox = funNode->funbox();
  MOZ_ASSERT(!funbox->hasExprBody());

  if (funbox->hasRest()) {
    return m.fail(funNode, "rest args not allowed");
  }
  if (funbox->hasDestructuringArgs) {
    return m.fail(funNode, "destructuring args not allowed");
  }
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CheckFuncPtrCall(FunctionValidator& f, ParseNode* callNode,
                             Type ret, Type* type) {
  MOZ_ASSERT(ret.isCanonical());
||||||| merged common ancestors
static bool
CheckInternalCall(FunctionValidator& f, ParseNode* callNode, PropertyName* calleeName,
                  Type ret, Type* type)
{
    MOZ_ASSERT(ret.isCanonical());
=======
static bool CheckArgument(ModuleValidatorShared& m, ParseNode* arg,
                          PropertyName** name) {
  *name = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* callee = CallCallee(callNode);
  ParseNode* tableNode = ElemBase(callee);
  ParseNode* indexExpr = ElemIndex(callee);
||||||| merged common ancestors
    ValTypeVector args;
    if (!CheckCallArgs<CheckIsArgType>(f, callNode, &args)) {
        return false;
    }
=======
  if (!arg->isKind(ParseNodeKind::Name)) {
    return m.fail(arg, "argument is not a plain name");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!tableNode->isKind(ParseNodeKind::Name)) {
    return f.fail(tableNode, "expecting name of function-pointer array");
  }
||||||| merged common ancestors
    FuncType sig(std::move(args), ret.canonicalToExprType());
=======
  PropertyName* argName = arg->as<NameNode>().name();
  ;
  if (!CheckIdentifier(m, arg, argName)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  PropertyName* name = tableNode->as<NameNode>().name();
  if (const ModuleValidator::Global* existing = f.lookupGlobal(name)) {
    if (existing->which() != ModuleValidator::Global::Table) {
      return f.failName(
          tableNode, "'%s' is not the name of a function-pointer array", name);
    }
  }
||||||| merged common ancestors
    ModuleValidator::Func* callee;
    if (!CheckFunctionSignature(f.m(), callNode, std::move(sig), calleeName, &callee)) {
        return false;
    }
=======
  *name = argName;
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!indexExpr->isKind(ParseNodeKind::BitAnd)) {
    return f.fail(indexExpr,
                  "function-pointer table index expression needs & mask");
  }
||||||| merged common ancestors
    if (!f.writeCall(callNode, MozOp::OldCallDirect)) {
        return false;
    }
=======
static bool CheckModuleArgument(ModuleValidatorShared& m, ParseNode* arg,
                                PropertyName** name) {
  if (!CheckArgument(m, arg, name)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* indexNode = BitwiseLeft(indexExpr);
  ParseNode* maskNode = BitwiseRight(indexExpr);
||||||| merged common ancestors
    if (!f.encoder().writeVarU32(callee->funcDefIndex())) {
        return false;
    }
=======
  if (!CheckModuleLevelName(m, arg, *name)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t mask;
  if (!IsLiteralInt(f.m(), maskNode, &mask) || mask == UINT32_MAX ||
      !IsPowerOfTwo(mask + 1)) {
    return f.fail(maskNode,
                  "function-pointer table index mask value must be a power of "
                  "two minus 1");
  }
||||||| merged common ancestors
    *type = Type::ret(ret);
    return true;
}
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type indexType;
  if (!CheckExpr(f, indexNode, &indexType)) {
    return false;
  }

  if (!indexType.isIntish()) {
    return f.failf(indexNode, "%s is not a subtype of intish",
                   indexType.toChars());
  }
||||||| merged common ancestors
static bool
CheckFuncPtrTableAgainstExisting(ModuleValidator& m, ParseNode* usepn, PropertyName* name,
                                 FuncType&& sig, unsigned mask, uint32_t* tableIndex)
{
    if (const ModuleValidator::Global* existing = m.lookupGlobal(name)) {
        if (existing->which() != ModuleValidator::Global::Table) {
            return m.failName(usepn, "'%s' is not a function-pointer table", name);
        }

        ModuleValidator::Table& table = m.table(existing->tableIndex());
        if (mask != table.mask()) {
            return m.failf(usepn, "mask does not match previous value (%u)", table.mask());
        }
=======
static bool CheckModuleArguments(ModuleValidatorShared& m,
                                 FunctionNode* funNode) {
  unsigned numFormals;
  ParseNode* arg1 = FunctionFormalParametersList(funNode, &numFormals);
  ParseNode* arg2 = arg1 ? NextNode(arg1) : nullptr;
  ParseNode* arg3 = arg2 ? NextNode(arg2) : nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ValTypeVector args;
  if (!CheckCallArgs<CheckIsArgType>(f, callNode, &args)) {
    return false;
  }
||||||| merged common ancestors
        if (!CheckSignatureAgainstExisting(m, usepn, sig, m.env().types[table.sigIndex()].funcType())) {
            return false;
        }
=======
  if (numFormals > 3) {
    return m.fail(funNode, "asm.js modules takes at most 3 argument");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  FuncType sig(std::move(args), ret.canonicalToExprType());
||||||| merged common ancestors
        *tableIndex = existing->tableIndex();
        return true;
    }
=======
  PropertyName* arg1Name = nullptr;
  if (arg1 && !CheckModuleArgument(m, arg1, &arg1Name)) {
    return false;
  }
  if (!m.initGlobalArgumentName(arg1Name)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t tableIndex;
  if (!CheckFuncPtrTableAgainstExisting(f.m(), tableNode, name, std::move(sig),
                                        mask, &tableIndex)) {
    return false;
  }
||||||| merged common ancestors
    if (!CheckModuleLevelName(m, usepn, name)) {
        return false;
    }
=======
  PropertyName* arg2Name = nullptr;
  if (arg2 && !CheckModuleArgument(m, arg2, &arg2Name)) {
    return false;
  }
  if (!m.initImportArgumentName(arg2Name)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!f.writeCall(callNode, MozOp::OldCallIndirect)) {
    return false;
  }
||||||| merged common ancestors
    if (!m.declareFuncPtrTable(std::move(sig), name, usepn->pn_pos.begin, mask, tableIndex)) {
        return false;
    }
=======
  PropertyName* arg3Name = nullptr;
  if (arg3 && !CheckModuleArgument(m, arg3, &arg3Name)) {
    return false;
  }
  if (!m.initBufferArgumentName(arg3Name)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call signature
  if (!f.encoder().writeVarU32(f.m().table(tableIndex).sigIndex())) {
    return false;
  }
||||||| merged common ancestors
    return true;
}
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  *type = Type::ret(ret);
  return true;
}
||||||| merged common ancestors
static bool
CheckFuncPtrCall(FunctionValidator& f, ParseNode* callNode, Type ret, Type* type)
{
    MOZ_ASSERT(ret.isCanonical());
=======
static bool CheckPrecedingStatements(ModuleValidatorShared& m,
                                     ParseNode* stmtList) {
  MOZ_ASSERT(stmtList->isKind(ParseNodeKind::StatementList));
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckIsExternType(FunctionValidator& f, ParseNode* argNode,
                              Type type) {
  if (!type.isExtern()) {
    return f.failf(argNode, "%s is not a subtype of extern", type.toChars());
  }
  return true;
}

static bool CheckFFICall(FunctionValidator& f, ParseNode* callNode,
                         unsigned ffiIndex, Type ret, Type* type) {
  MOZ_ASSERT(ret.isCanonical());
||||||| merged common ancestors
    ParseNode* callee = CallCallee(callNode);
    ParseNode* tableNode = ElemBase(callee);
    ParseNode* indexExpr = ElemIndex(callee);

    if (!tableNode->isKind(ParseNodeKind::Name)) {
        return f.fail(tableNode, "expecting name of function-pointer array");
    }
=======
  ParseNode* stmt = ListHead(stmtList);
  for (unsigned i = 0, n = ListLength(stmtList); i < n; i++) {
    if (!IsIgnoredDirective(m.cx(), stmt)) {
      return m.fail(stmt, "invalid asm.js statement");
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  PropertyName* calleeName = CallCallee(callNode)->as<NameNode>().name();
||||||| merged common ancestors
    PropertyName* name = tableNode->as<NameNode>().name();
    if (const ModuleValidator::Global* existing = f.lookupGlobal(name)) {
        if (existing->which() != ModuleValidator::Global::Table) {
            return f.failName(tableNode, "'%s' is not the name of a function-pointer array", name);
        }
    }
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (ret.isFloat()) {
    return f.fail(callNode, "FFI calls can't return float");
  }
||||||| merged common ancestors
    if (!indexExpr->isKind(ParseNodeKind::BitAnd)) {
        return f.fail(indexExpr, "function-pointer table index expression needs & mask");
    }
=======
static bool CheckGlobalVariableInitConstant(ModuleValidatorShared& m,
                                            PropertyName* varName,
                                            ParseNode* initNode, bool isConst) {
  NumLit lit = ExtractNumericLiteral(m, initNode);
  if (!lit.valid()) {
    return m.fail(initNode,
                  "global initializer is out of representable integer range");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ValTypeVector args;
  if (!CheckCallArgs<CheckIsExternType>(f, callNode, &args)) {
    return false;
  }
||||||| merged common ancestors
    ParseNode* indexNode = BitwiseLeft(indexExpr);
    ParseNode* maskNode = BitwiseRight(indexExpr);
=======
  Type canonicalType = Type::canonicalize(Type::lit(lit));
  if (!canonicalType.isGlobalVarType()) {
    return m.fail(initNode, "global variable type not allowed");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  FuncType sig(std::move(args), ret.canonicalToExprType());
||||||| merged common ancestors
    uint32_t mask;
    if (!IsLiteralInt(f.m(), maskNode, &mask) || mask == UINT32_MAX || !IsPowerOfTwo(mask + 1)) {
        return f.fail(maskNode, "function-pointer table index mask value must be a power of two minus 1");
    }
=======
  return m.addGlobalVarInit(varName, lit, canonicalType, isConst);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t importIndex;
  if (!f.m().declareImport(calleeName, std::move(sig), ffiIndex,
                           &importIndex)) {
    return false;
  }

  if (!f.writeCall(callNode, Op::Call)) {
    return false;
  }

  if (!f.encoder().writeVarU32(importIndex)) {
    return false;
  }

  *type = Type::ret(ret);
  return true;
}

static bool CheckFloatCoercionArg(FunctionValidator& f, ParseNode* inputNode,
                                  Type inputType) {
  if (inputType.isMaybeDouble()) {
    return f.encoder().writeOp(Op::F32DemoteF64);
  }
  if (inputType.isSigned()) {
    return f.encoder().writeOp(Op::F32ConvertSI32);
  }
  if (inputType.isUnsigned()) {
    return f.encoder().writeOp(Op::F32ConvertUI32);
  }
  if (inputType.isFloatish()) {
    return true;
  }
||||||| merged common ancestors
    Type indexType;
    if (!CheckExpr(f, indexNode, &indexType)) {
        return false;
    }

    if (!indexType.isIntish()) {
        return f.failf(indexNode, "%s is not a subtype of intish", indexType.toChars());
    }

    ValTypeVector args;
    if (!CheckCallArgs<CheckIsArgType>(f, callNode, &args)) {
        return false;
    }
=======
static bool CheckTypeAnnotation(ModuleValidatorShared& m,
                                ParseNode* coercionNode, Type* coerceTo,
                                ParseNode** coercedExpr = nullptr) {
  switch (coercionNode->getKind()) {
    case ParseNodeKind::BitOrExpr: {
      ParseNode* rhs = BitwiseRight(coercionNode);
      uint32_t i;
      if (!IsLiteralInt(m, rhs, &i) || i != 0) {
        return m.fail(rhs, "must use |0 for argument/return coercion");
      }
      *coerceTo = Type::Int;
      if (coercedExpr) {
        *coercedExpr = BitwiseLeft(coercionNode);
      }
      return true;
    }
    case ParseNodeKind::PosExpr: {
      *coerceTo = Type::Double;
      if (coercedExpr) {
        *coercedExpr = UnaryKid(coercionNode);
      }
      return true;
    }
    case ParseNodeKind::CallExpr: {
      if (IsCoercionCall(m, coercionNode, coerceTo, coercedExpr)) {
        return true;
      }
      break;
    }
    default:;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return f.failf(inputNode,
                 "%s is not a subtype of signed, unsigned, double? or floatish",
                 inputType.toChars());
}
||||||| merged common ancestors
    FuncType sig(std::move(args), ret.canonicalToExprType());
=======
  return m.fail(coercionNode, "must be of the form +x, x|0 or fround(x)");
}
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckCoercedCall(FunctionValidator& f, ParseNode* call, Type ret,
                             Type* type);
||||||| merged common ancestors
    uint32_t tableIndex;
    if (!CheckFuncPtrTableAgainstExisting(f.m(), tableNode, name, std::move(sig), mask, &tableIndex)) {
        return false;
    }
=======
static bool CheckGlobalVariableInitImport(ModuleValidatorShared& m,
                                          PropertyName* varName,
                                          ParseNode* initNode, bool isConst) {
  Type coerceTo;
  ParseNode* coercedExpr;
  if (!CheckTypeAnnotation(m, initNode, &coerceTo, &coercedExpr)) {
    return false;
  }

  if (!coercedExpr->isKind(ParseNodeKind::DotExpr)) {
    return m.failName(coercedExpr, "invalid import expression for global '%s'",
                      varName);
  }

  if (!coerceTo.isGlobalVarType()) {
    return m.fail(initNode, "global variable type not allowed");
  }

  ParseNode* base = DotBase(coercedExpr);
  PropertyName* field = DotMember(coercedExpr);

  PropertyName* importName = m.importArgumentName();
  if (!importName) {
    return m.fail(coercedExpr,
                  "cannot import without an asm.js foreign parameter");
  }
  if (!IsUseOfName(base, importName)) {
    return m.failName(coercedExpr, "base of import expression must be '%s'",
                      importName);
  }

  return m.addGlobalVarImport(varName, field, coerceTo, isConst);
}

static bool IsArrayViewCtorName(ModuleValidatorShared& m, PropertyName* name,
                                Scalar::Type* type) {
  JSAtomState& names = m.cx()->names();
  if (name == names.Int8Array) {
    *type = Scalar::Int8;
  } else if (name == names.Uint8Array) {
    *type = Scalar::Uint8;
  } else if (name == names.Int16Array) {
    *type = Scalar::Int16;
  } else if (name == names.Uint16Array) {
    *type = Scalar::Uint16;
  } else if (name == names.Int32Array) {
    *type = Scalar::Int32;
  } else if (name == names.Uint32Array) {
    *type = Scalar::Uint32;
  } else if (name == names.Float32Array) {
    *type = Scalar::Float32;
  } else if (name == names.Float64Array) {
    *type = Scalar::Float64;
  } else {
    return false;
  }
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckCoercionArg(FunctionValidator& f, ParseNode* arg,
                             Type expected, Type* type) {
  MOZ_ASSERT(expected.isCanonicalValType());
||||||| merged common ancestors
    if (!f.writeCall(callNode, MozOp::OldCallIndirect)) {
        return false;
    }
=======
static bool CheckNewArrayViewArgs(ModuleValidatorShared& m, ParseNode* newExpr,
                                  PropertyName* bufferName) {
  ParseNode* ctorExpr = BinaryLeft(newExpr);
  ParseNode* ctorArgs = BinaryRight(newExpr);
  ParseNode* bufArg = ListHead(ctorArgs);
  if (!bufArg || NextNode(bufArg) != nullptr) {
    return m.fail(ctorExpr,
                  "array view constructor takes exactly one argument");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (arg->isKind(ParseNodeKind::Call)) {
    return CheckCoercedCall(f, arg, expected, type);
  }
||||||| merged common ancestors
    // Call signature
    if (!f.encoder().writeVarU32(f.m().table(tableIndex).sigIndex())) {
        return false;
    }
=======
  if (!IsUseOfName(bufArg, bufferName)) {
    return m.failName(bufArg, "argument to array view constructor must be '%s'",
                      bufferName);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type argType;
  if (!CheckExpr(f, arg, &argType)) {
    return false;
  }
||||||| merged common ancestors
    *type = Type::ret(ret);
    return true;
}
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (expected.isFloat()) {
    if (!CheckFloatCoercionArg(f, arg, argType)) {
      return false;
    }
  } else {
    MOZ_CRASH("not call coercions");
  }

  *type = Type::ret(expected);
  return true;
}
||||||| merged common ancestors
static bool
CheckIsExternType(FunctionValidator& f, ParseNode* argNode, Type type)
{
    if (!type.isExtern()) {
        return f.failf(argNode, "%s is not a subtype of extern", type.toChars());
    }
    return true;
}
=======
static bool CheckNewArrayView(ModuleValidatorShared& m, PropertyName* varName,
                              ParseNode* newExpr) {
  PropertyName* globalName = m.globalArgumentName();
  if (!globalName) {
    return m.fail(
        newExpr, "cannot create array view without an asm.js global parameter");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckMathFRound(FunctionValidator& f, ParseNode* callNode,
                            Type* type) {
  if (CallArgListLength(callNode) != 1) {
    return f.fail(callNode, "Math.fround must be passed 1 argument");
  }
||||||| merged common ancestors
static bool
CheckFFICall(FunctionValidator& f, ParseNode* callNode, unsigned ffiIndex, Type ret, Type* type)
{
    MOZ_ASSERT(ret.isCanonical());
=======
  PropertyName* bufferName = m.bufferArgumentName();
  if (!bufferName) {
    return m.fail(newExpr,
                  "cannot create array view without an asm.js heap parameter");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* argNode = CallArgList(callNode);
  Type argType;
  if (!CheckCoercionArg(f, argNode, Type::Float, &argType)) {
    return false;
  }

  MOZ_ASSERT(argType == Type::Float);
  *type = Type::Float;
  return true;
}

static bool CheckMathBuiltinCall(FunctionValidator& f, ParseNode* callNode,
                                 AsmJSMathBuiltinFunction func, Type* type) {
  unsigned arity = 0;
  Op f32 = Op::Limit;
  Op f64 = Op::Limit;
  MozOp mozf64 = MozOp::Limit;
  switch (func) {
    case AsmJSMathBuiltin_imul:
      return CheckMathIMul(f, callNode, type);
    case AsmJSMathBuiltin_clz32:
      return CheckMathClz32(f, callNode, type);
    case AsmJSMathBuiltin_abs:
      return CheckMathAbs(f, callNode, type);
    case AsmJSMathBuiltin_sqrt:
      return CheckMathSqrt(f, callNode, type);
    case AsmJSMathBuiltin_fround:
      return CheckMathFRound(f, callNode, type);
    case AsmJSMathBuiltin_min:
      return CheckMathMinMax(f, callNode, /* isMax = */ false, type);
    case AsmJSMathBuiltin_max:
      return CheckMathMinMax(f, callNode, /* isMax = */ true, type);
    case AsmJSMathBuiltin_ceil:
      arity = 1;
      f64 = Op::F64Ceil;
      f32 = Op::F32Ceil;
      break;
    case AsmJSMathBuiltin_floor:
      arity = 1;
      f64 = Op::F64Floor;
      f32 = Op::F32Floor;
      break;
    case AsmJSMathBuiltin_sin:
      arity = 1;
      mozf64 = MozOp::F64Sin;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_cos:
      arity = 1;
      mozf64 = MozOp::F64Cos;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_tan:
      arity = 1;
      mozf64 = MozOp::F64Tan;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_asin:
      arity = 1;
      mozf64 = MozOp::F64Asin;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_acos:
      arity = 1;
      mozf64 = MozOp::F64Acos;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_atan:
      arity = 1;
      mozf64 = MozOp::F64Atan;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_exp:
      arity = 1;
      mozf64 = MozOp::F64Exp;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_log:
      arity = 1;
      mozf64 = MozOp::F64Log;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_pow:
      arity = 2;
      mozf64 = MozOp::F64Pow;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_atan2:
      arity = 2;
      mozf64 = MozOp::F64Atan2;
      f32 = Op::Unreachable;
      break;
    default:
      MOZ_CRASH("unexpected mathBuiltin function");
  }

  unsigned actualArity = CallArgListLength(callNode);
  if (actualArity != arity) {
    return f.failf(callNode, "call passed %u arguments, expected %u",
                   actualArity, arity);
  }

  if (!f.prepareCall(callNode)) {
    return false;
  }
||||||| merged common ancestors
    PropertyName* calleeName = CallCallee(callNode)->as<NameNode>().name();
=======
  ParseNode* ctorExpr = BinaryLeft(newExpr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type firstType;
  ParseNode* argNode = CallArgList(callNode);
  if (!CheckExpr(f, argNode, &firstType)) {
    return false;
  }
||||||| merged common ancestors
    if (ret.isFloat()) {
        return f.fail(callNode, "FFI calls can't return float");
    }
=======
  PropertyName* field;
  Scalar::Type type;
  if (ctorExpr->isKind(ParseNodeKind::DotExpr)) {
    ParseNode* base = DotBase(ctorExpr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!firstType.isMaybeFloat() && !firstType.isMaybeDouble()) {
    return f.fail(
        argNode,
        "arguments to math call should be a subtype of double? or float?");
  }
||||||| merged common ancestors
    ValTypeVector args;
    if (!CheckCallArgs<CheckIsExternType>(f, callNode, &args)) {
        return false;
    }
=======
    if (!IsUseOfName(base, globalName)) {
      return m.failName(base, "expecting '%s.*Array", globalName);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool opIsDouble = firstType.isMaybeDouble();
  if (!opIsDouble && f32 == Op::Unreachable) {
    return f.fail(callNode, "math builtin cannot be used as float");
  }

  if (arity == 2) {
    Type secondType;
    argNode = NextNode(argNode);
    if (!CheckExpr(f, argNode, &secondType)) {
      return false;
||||||| merged common ancestors
    FuncType sig(std::move(args), ret.canonicalToExprType());

    uint32_t importIndex;
    if (!f.m().declareImport(calleeName, std::move(sig), ffiIndex, &importIndex)) {
        return false;
=======
    field = DotMember(ctorExpr);
    if (!IsArrayViewCtorName(m, field, &type)) {
      return m.fail(ctorExpr, "could not match typed array name");
    }
  } else {
    if (!ctorExpr->isKind(ParseNodeKind::Name)) {
      return m.fail(ctorExpr,
                    "expecting name of imported array view constructor");
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (firstType.isMaybeDouble() && !secondType.isMaybeDouble()) {
      return f.fail(
          argNode,
          "both arguments to math builtin call should be the same type");
||||||| merged common ancestors
    if (!f.writeCall(callNode, Op::Call)) {
        return false;
=======
    PropertyName* globalName = ctorExpr->as<NameNode>().name();
    const ModuleValidatorShared::Global* global = m.lookupGlobal(globalName);
    if (!global) {
      return m.failName(ctorExpr, "%s not found in module global scope",
                        globalName);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (firstType.isMaybeFloat() && !secondType.isMaybeFloat()) {
      return f.fail(
          argNode,
          "both arguments to math builtin call should be the same type");
||||||| merged common ancestors

    if (!f.encoder().writeVarU32(importIndex)) {
        return false;
=======

    if (global->which() != ModuleValidatorShared::Global::ArrayViewCtor) {
      return m.failName(ctorExpr,
                        "%s must be an imported array view constructor",
                        globalName);
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (opIsDouble) {
    if (f64 != Op::Limit) {
      if (!f.encoder().writeOp(f64)) {
        return false;
      }
    } else {
      if (!f.encoder().writeOp(mozf64)) {
        return false;
      }
    }
  } else {
    if (!f.encoder().writeOp(f32)) {
      return false;
    }
  }
||||||| merged common ancestors
    *type = Type::ret(ret);
    return true;
}

static bool
CheckFloatCoercionArg(FunctionValidator& f, ParseNode* inputNode, Type inputType)
{
    if (inputType.isMaybeDouble()) {
        return f.encoder().writeOp(Op::F32DemoteF64);
    }
    if (inputType.isSigned()) {
        return f.encoder().writeOp(Op::F32ConvertSI32);
    }
    if (inputType.isUnsigned()) {
        return f.encoder().writeOp(Op::F32ConvertUI32);
    }
    if (inputType.isFloatish()) {
        return true;
    }
=======
    field = nullptr;
    type = global->viewType();
  }

  if (!CheckNewArrayViewArgs(m, newExpr, bufferName)) {
    return false;
  }

  return m.addArrayView(varName, type, field);
}

static bool CheckGlobalMathImport(ModuleValidatorShared& m, ParseNode* initNode,
                                  PropertyName* varName, PropertyName* field) {
  // Math builtin, with the form glob.Math.[[builtin]]
  ModuleValidatorShared::MathBuiltin mathBuiltin;
  if (!m.lookupStandardLibraryMathName(field, &mathBuiltin)) {
    return m.failName(initNode, "'%s' is not a standard Math builtin", field);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *type = opIsDouble ? Type::Double : Type::Floatish;
  return true;
||||||| merged common ancestors
    return f.failf(inputNode, "%s is not a subtype of signed, unsigned, double? or floatish",
                   inputType.toChars());
=======
  switch (mathBuiltin.kind) {
    case ModuleValidatorShared::MathBuiltin::Function:
      return m.addMathBuiltinFunction(varName, mathBuiltin.u.func, field);
    case ModuleValidatorShared::MathBuiltin::Constant:
      return m.addMathBuiltinConstant(varName, mathBuiltin.u.cst, field);
    default:
      break;
  }
  MOZ_CRASH("unexpected or uninitialized math builtin type");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CheckUncoercedCall(FunctionValidator& f, ParseNode* expr,
                               Type* type) {
  MOZ_ASSERT(expr->isKind(ParseNodeKind::Call));
||||||| merged common ancestors
static bool
CheckCoercedCall(FunctionValidator& f, ParseNode* call, Type ret, Type* type);
=======
static bool CheckGlobalDotImport(ModuleValidatorShared& m,
                                 PropertyName* varName, ParseNode* initNode) {
  ParseNode* base = DotBase(initNode);
  PropertyName* field = DotMember(initNode);
>>>>>>> upstream-releases

<<<<<<< HEAD
  const ModuleValidator::Global* global;
  if (IsCallToGlobal(f.m(), expr, &global) && global->isMathFunction()) {
    return CheckMathBuiltinCall(f, expr, global->mathBuiltinFunction(), type);
  }
||||||| merged common ancestors
static bool
CheckCoercionArg(FunctionValidator& f, ParseNode* arg, Type expected, Type* type)
{
    MOZ_ASSERT(expected.isCanonicalValType());
=======
  if (base->isKind(ParseNodeKind::DotExpr)) {
    ParseNode* global = DotBase(base);
    PropertyName* math = DotMember(base);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return f.fail(
      expr,
      "all function calls must be calls to standard lib math functions,"
      " ignored (via f(); or comma-expression), coerced to signed (via f()|0),"
      " coerced to float (via fround(f())), or coerced to double (via +f())");
}
||||||| merged common ancestors
    if (arg->isKind(ParseNodeKind::Call)) {
        return CheckCoercedCall(f, arg, expected, type);
    }
=======
    PropertyName* globalName = m.globalArgumentName();
    if (!globalName) {
      return m.fail(
          base, "import statement requires the module have a stdlib parameter");
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CoerceResult(FunctionValidator& f, ParseNode* expr, Type expected,
                         Type actual, Type* type) {
  MOZ_ASSERT(expected.isCanonical());
||||||| merged common ancestors
    Type argType;
    if (!CheckExpr(f, arg, &argType)) {
        return false;
    }
=======
    if (!IsUseOfName(global, globalName)) {
      if (global->isKind(ParseNodeKind::DotExpr)) {
        return m.failName(base,
                          "imports can have at most two dot accesses "
                          "(e.g. %s.Math.sin)",
                          globalName);
      }
      return m.failName(base, "expecting %s.*", globalName);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // At this point, the bytecode resembles this:
  //      | the thing we wanted to coerce | current position |>
  switch (expected.which()) {
    case Type::Void:
      if (!actual.isVoid()) {
        if (!f.encoder().writeOp(Op::Drop)) {
          return false;
        }
      }
      break;
    case Type::Int:
      if (!actual.isIntish()) {
        return f.failf(expr, "%s is not a subtype of intish", actual.toChars());
      }
      break;
    case Type::Float:
      if (!CheckFloatCoercionArg(f, expr, actual)) {
        return false;
      }
      break;
    case Type::Double:
      if (actual.isMaybeDouble()) {
        // No conversion necessary.
      } else if (actual.isMaybeFloat()) {
        if (!f.encoder().writeOp(Op::F64PromoteF32)) {
          return false;
        }
      } else if (actual.isSigned()) {
        if (!f.encoder().writeOp(Op::F64ConvertSI32)) {
          return false;
        }
      } else if (actual.isUnsigned()) {
        if (!f.encoder().writeOp(Op::F64ConvertUI32)) {
          return false;
        }
      } else {
        return f.failf(
            expr, "%s is not a subtype of double?, float?, signed or unsigned",
            actual.toChars());
      }
      break;
    default:
      MOZ_CRASH("unexpected uncoerced result type");
  }
||||||| merged common ancestors
    if (expected.isFloat()) {
        if (!CheckFloatCoercionArg(f, arg, argType)) {
            return false;
        }
    } else {
        MOZ_CRASH("not call coercions");
    }
=======
    if (math == m.cx()->names().Math) {
      return CheckGlobalMathImport(m, initNode, varName, field);
    }
    return m.failName(base, "expecting %s.Math", globalName);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *type = Type::ret(expected);
  return true;
}
||||||| merged common ancestors
    *type = Type::ret(expected);
    return true;
}
=======
  if (!base->isKind(ParseNodeKind::Name)) {
    return m.fail(base, "expected name of variable or parameter");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckCoercedMathBuiltinCall(FunctionValidator& f,
                                        ParseNode* callNode,
                                        AsmJSMathBuiltinFunction func, Type ret,
                                        Type* type) {
  Type actual;
  if (!CheckMathBuiltinCall(f, callNode, func, &actual)) {
    return false;
  }
  return CoerceResult(f, callNode, ret, actual, type);
}

static bool CheckCoercedCall(FunctionValidator& f, ParseNode* call, Type ret,
                             Type* type) {
  MOZ_ASSERT(ret.isCanonical());

  if (!CheckRecursionLimitDontReport(f.cx())) {
    return f.m().failOverRecursed();
  }

  if (IsNumericLiteral(f.m(), call)) {
    NumLit lit = ExtractNumericLiteral(f.m(), call);
    if (!f.writeConstExpr(lit)) {
      return false;
    }
    return CoerceResult(f, call, ret, Type::lit(lit), type);
  }

  ParseNode* callee = CallCallee(call);

  if (callee->isKind(ParseNodeKind::Elem)) {
    return CheckFuncPtrCall(f, call, ret, type);
  }

  if (!callee->isKind(ParseNodeKind::Name)) {
    return f.fail(callee, "unexpected callee expression type");
  }

  PropertyName* calleeName = callee->as<NameNode>().name();

  if (const ModuleValidator::Global* global = f.lookupGlobal(calleeName)) {
    switch (global->which()) {
      case ModuleValidator::Global::FFI:
        return CheckFFICall(f, call, global->ffiIndex(), ret, type);
      case ModuleValidator::Global::MathBuiltinFunction:
        return CheckCoercedMathBuiltinCall(
            f, call, global->mathBuiltinFunction(), ret, type);
      case ModuleValidator::Global::ConstantLiteral:
      case ModuleValidator::Global::ConstantImport:
      case ModuleValidator::Global::Variable:
      case ModuleValidator::Global::Table:
      case ModuleValidator::Global::ArrayView:
      case ModuleValidator::Global::ArrayViewCtor:
        return f.failName(callee, "'%s' is not callable function", calleeName);
      case ModuleValidator::Global::Function:
        break;
||||||| merged common ancestors
static bool
CheckMathFRound(FunctionValidator& f, ParseNode* callNode, Type* type)
{
    if (CallArgListLength(callNode) != 1) {
        return f.fail(callNode, "Math.fround must be passed 1 argument");
    }

    ParseNode* argNode = CallArgList(callNode);
    Type argType;
    if (!CheckCoercionArg(f, argNode, Type::Float, &argType)) {
        return false;
=======
  auto baseName = base->as<NameNode>().name();
  if (baseName == m.globalArgumentName()) {
    if (field == m.cx()->names().NaN) {
      return m.addGlobalConstant(varName, GenericNaN(), field);
    }
    if (field == m.cx()->names().Infinity) {
      return m.addGlobalConstant(varName, PositiveInfinity<double>(), field);
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  return CheckInternalCall(f, call, calleeName, ret, type);
}

static bool CheckPos(FunctionValidator& f, ParseNode* pos, Type* type) {
  MOZ_ASSERT(pos->isKind(ParseNodeKind::Pos));
  ParseNode* operand = UnaryKid(pos);
||||||| merged common ancestors
    MOZ_ASSERT(argType == Type::Float);
    *type = Type::Float;
    return true;
}

static bool
CheckMathBuiltinCall(FunctionValidator& f, ParseNode* callNode, AsmJSMathBuiltinFunction func,
                     Type* type)
{
    unsigned arity = 0;
    Op f32 = Op::Limit;
    Op f64 = Op::Limit;
    MozOp mozf64 = MozOp::Limit;
    switch (func) {
      case AsmJSMathBuiltin_imul:   return CheckMathIMul(f, callNode, type);
      case AsmJSMathBuiltin_clz32:  return CheckMathClz32(f, callNode, type);
      case AsmJSMathBuiltin_abs:    return CheckMathAbs(f, callNode, type);
      case AsmJSMathBuiltin_sqrt:   return CheckMathSqrt(f, callNode, type);
      case AsmJSMathBuiltin_fround: return CheckMathFRound(f, callNode, type);
      case AsmJSMathBuiltin_min:    return CheckMathMinMax(f, callNode, /* isMax = */ false, type);
      case AsmJSMathBuiltin_max:    return CheckMathMinMax(f, callNode, /* isMax = */ true, type);
      case AsmJSMathBuiltin_ceil:   arity = 1; f64 = Op::F64Ceil;        f32 = Op::F32Ceil;     break;
      case AsmJSMathBuiltin_floor:  arity = 1; f64 = Op::F64Floor;       f32 = Op::F32Floor;    break;
      case AsmJSMathBuiltin_sin:    arity = 1; mozf64 = MozOp::F64Sin;   f32 = Op::Unreachable; break;
      case AsmJSMathBuiltin_cos:    arity = 1; mozf64 = MozOp::F64Cos;   f32 = Op::Unreachable; break;
      case AsmJSMathBuiltin_tan:    arity = 1; mozf64 = MozOp::F64Tan;   f32 = Op::Unreachable; break;
      case AsmJSMathBuiltin_asin:   arity = 1; mozf64 = MozOp::F64Asin;  f32 = Op::Unreachable; break;
      case AsmJSMathBuiltin_acos:   arity = 1; mozf64 = MozOp::F64Acos;  f32 = Op::Unreachable; break;
      case AsmJSMathBuiltin_atan:   arity = 1; mozf64 = MozOp::F64Atan;  f32 = Op::Unreachable; break;
      case AsmJSMathBuiltin_exp:    arity = 1; mozf64 = MozOp::F64Exp;   f32 = Op::Unreachable; break;
      case AsmJSMathBuiltin_log:    arity = 1; mozf64 = MozOp::F64Log;   f32 = Op::Unreachable; break;
      case AsmJSMathBuiltin_pow:    arity = 2; mozf64 = MozOp::F64Pow;   f32 = Op::Unreachable; break;
      case AsmJSMathBuiltin_atan2:  arity = 2; mozf64 = MozOp::F64Atan2; f32 = Op::Unreachable; break;
      default: MOZ_CRASH("unexpected mathBuiltin function");
    }

    unsigned actualArity = CallArgListLength(callNode);
    if (actualArity != arity) {
        return f.failf(callNode, "call passed %u arguments, expected %u", actualArity, arity);
    }

    if (!f.prepareCall(callNode)) {
        return false;
    }
=======
    Scalar::Type type;
    if (IsArrayViewCtorName(m, field, &type)) {
      return m.addArrayViewCtor(varName, type, field);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (operand->isKind(ParseNodeKind::Call)) {
    return CheckCoercedCall(f, operand, Type::Double, type);
  }
||||||| merged common ancestors
    Type firstType;
    ParseNode* argNode = CallArgList(callNode);
    if (!CheckExpr(f, argNode, &firstType)) {
        return false;
    }
=======
    return m.failName(
        initNode, "'%s' is not a standard constant or typed array name", field);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type actual;
  if (!CheckExpr(f, operand, &actual)) {
    return false;
  }
||||||| merged common ancestors
    if (!firstType.isMaybeFloat() && !firstType.isMaybeDouble()) {
        return f.fail(argNode, "arguments to math call should be a subtype of double? or float?");
    }
=======
  if (baseName != m.importArgumentName()) {
    return m.fail(base, "expected global or import name");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return CoerceResult(f, operand, Type::Double, actual, type);
}
||||||| merged common ancestors
    bool opIsDouble = firstType.isMaybeDouble();
    if (!opIsDouble && f32 == Op::Unreachable) {
        return f.fail(callNode, "math builtin cannot be used as float");
    }
=======
  return m.addFFI(varName, field);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckNot(FunctionValidator& f, ParseNode* expr, Type* type) {
  MOZ_ASSERT(expr->isKind(ParseNodeKind::Not));
  ParseNode* operand = UnaryKid(expr);
||||||| merged common ancestors
    if (arity == 2) {
        Type secondType;
        argNode = NextNode(argNode);
        if (!CheckExpr(f, argNode, &secondType)) {
            return false;
        }
=======
static bool CheckModuleGlobal(ModuleValidatorShared& m, ParseNode* decl,
                              bool isConst) {
  if (!decl->isKind(ParseNodeKind::AssignExpr)) {
    return m.fail(decl, "module import needs initializer");
  }
  AssignmentNode* assignNode = &decl->as<AssignmentNode>();
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type operandType;
  if (!CheckExpr(f, operand, &operandType)) {
    return false;
  }
||||||| merged common ancestors
        if (firstType.isMaybeDouble() && !secondType.isMaybeDouble()) {
            return f.fail(argNode, "both arguments to math builtin call should be the same type");
        }
        if (firstType.isMaybeFloat() && !secondType.isMaybeFloat()) {
            return f.fail(argNode, "both arguments to math builtin call should be the same type");
        }
    }
=======
  ParseNode* var = assignNode->left();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!operandType.isInt()) {
    return f.failf(operand, "%s is not a subtype of int",
                   operandType.toChars());
  }
||||||| merged common ancestors
    if (opIsDouble) {
        if (f64 != Op::Limit) {
            if (!f.encoder().writeOp(f64)) {
                return false;
            }
        } else {
            if (!f.encoder().writeOp(mozf64)) {
                return false;
            }
        }
    } else {
        if (!f.encoder().writeOp(f32)) {
            return false;
        }
    }
=======
  if (!var->isKind(ParseNodeKind::Name)) {
    return m.fail(var, "import variable is not a plain name");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *type = Type::Int;
  return f.encoder().writeOp(Op::I32Eqz);
}
||||||| merged common ancestors
    *type = opIsDouble ? Type::Double : Type::Floatish;
    return true;
}
=======
  PropertyName* varName = var->as<NameNode>().name();
  if (!CheckModuleLevelName(m, var, varName)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckNeg(FunctionValidator& f, ParseNode* expr, Type* type) {
  MOZ_ASSERT(expr->isKind(ParseNodeKind::Neg));
  ParseNode* operand = UnaryKid(expr);
||||||| merged common ancestors
static bool
CheckUncoercedCall(FunctionValidator& f, ParseNode* expr, Type* type)
{
    MOZ_ASSERT(expr->isKind(ParseNodeKind::Call));
=======
  ParseNode* initNode = assignNode->right();
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type operandType;
  if (!CheckExpr(f, operand, &operandType)) {
    return false;
  }
||||||| merged common ancestors
    const ModuleValidator::Global* global;
    if (IsCallToGlobal(f.m(), expr, &global) && global->isMathFunction()) {
        return CheckMathBuiltinCall(f, expr, global->mathBuiltinFunction(), type);
    }
=======
  if (IsNumericLiteral(m, initNode)) {
    return CheckGlobalVariableInitConstant(m, varName, initNode, isConst);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (operandType.isInt()) {
    *type = Type::Intish;
    return f.encoder().writeOp(MozOp::I32Neg);
  }
||||||| merged common ancestors
    return f.fail(expr, "all function calls must be calls to standard lib math functions,"
                        " ignored (via f(); or comma-expression), coerced to signed (via f()|0),"
                        " coerced to float (via fround(f())), or coerced to double (via +f())");
}
=======
  if (initNode->isKind(ParseNodeKind::BitOrExpr) ||
      initNode->isKind(ParseNodeKind::PosExpr) ||
      initNode->isKind(ParseNodeKind::CallExpr)) {
    return CheckGlobalVariableInitImport(m, varName, initNode, isConst);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (operandType.isMaybeDouble()) {
    *type = Type::Double;
    return f.encoder().writeOp(Op::F64Neg);
  }
||||||| merged common ancestors
static bool
CoerceResult(FunctionValidator& f, ParseNode* expr, Type expected, Type actual,
             Type* type)
{
    MOZ_ASSERT(expected.isCanonical());
=======
  if (initNode->isKind(ParseNodeKind::NewExpr)) {
    return CheckNewArrayView(m, varName, initNode);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (operandType.isMaybeFloat()) {
    *type = Type::Floatish;
    return f.encoder().writeOp(Op::F32Neg);
  }
||||||| merged common ancestors
    // At this point, the bytecode resembles this:
    //      | the thing we wanted to coerce | current position |>
    switch (expected.which()) {
      case Type::Void:
        if (!actual.isVoid()) {
            if (!f.encoder().writeOp(Op::Drop)) {
                return false;
            }
        }
        break;
      case Type::Int:
        if (!actual.isIntish()) {
            return f.failf(expr, "%s is not a subtype of intish", actual.toChars());
        }
        break;
      case Type::Float:
        if (!CheckFloatCoercionArg(f, expr, actual)) {
            return false;
        }
        break;
      case Type::Double:
        if (actual.isMaybeDouble()) {
            // No conversion necessary.
        } else if (actual.isMaybeFloat()) {
            if (!f.encoder().writeOp(Op::F64PromoteF32)) {
                return false;
            }
        } else if (actual.isSigned()) {
            if (!f.encoder().writeOp(Op::F64ConvertSI32)) {
                return false;
            }
        } else if (actual.isUnsigned()) {
            if (!f.encoder().writeOp(Op::F64ConvertUI32)) {
                return false;
            }
        } else {
            return f.failf(expr, "%s is not a subtype of double?, float?, signed or unsigned", actual.toChars());
        }
        break;
      default:
        MOZ_CRASH("unexpected uncoerced result type");
    }
=======
  if (initNode->isKind(ParseNodeKind::DotExpr)) {
    return CheckGlobalDotImport(m, varName, initNode);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return f.failf(operand, "%s is not a subtype of int, float? or double?",
                 operandType.toChars());
||||||| merged common ancestors
    *type = Type::ret(expected);
    return true;
=======
  return m.fail(initNode, "unsupported import expression");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CheckCoerceToInt(FunctionValidator& f, ParseNode* expr,
                             Type* type) {
  MOZ_ASSERT(expr->isKind(ParseNodeKind::BitNot));
  ParseNode* operand = UnaryKid(expr);
||||||| merged common ancestors
static bool
CheckCoercedMathBuiltinCall(FunctionValidator& f, ParseNode* callNode, AsmJSMathBuiltinFunction func,
                            Type ret, Type* type)
{
    Type actual;
    if (!CheckMathBuiltinCall(f, callNode, func, &actual)) {
        return false;
    }
    return CoerceResult(f, callNode, ret, actual, type);
}
=======
template <typename Unit>
static bool CheckModuleProcessingDirectives(ModuleValidator<Unit>& m) {
  auto& ts = m.parser().tokenStream;
  while (true) {
    bool matched;
    if (!ts.matchToken(&matched, TokenKind::String,
                       TokenStreamShared::SlashIsRegExp)) {
      return false;
    }
    if (!matched) {
      return true;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type operandType;
  if (!CheckExpr(f, operand, &operandType)) {
    return false;
  }
||||||| merged common ancestors
static bool
CheckCoercedCall(FunctionValidator& f, ParseNode* call, Type ret, Type* type)
{
    MOZ_ASSERT(ret.isCanonical());
=======
    if (!IsIgnoredDirectiveName(m.cx(),
                                ts.anyCharsAccess().currentToken().atom())) {
      return m.failCurrentOffset("unsupported processing directive");
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (operandType.isMaybeDouble() || operandType.isMaybeFloat()) {
    *type = Type::Signed;
    Op opcode =
        operandType.isMaybeDouble() ? Op::I32TruncSF64 : Op::I32TruncSF32;
    return f.encoder().writeOp(opcode);
  }
||||||| merged common ancestors
    if (!CheckRecursionLimitDontReport(f.cx())) {
        return f.m().failOverRecursed();
    }
=======
    TokenKind tt;
    if (!ts.getToken(&tt)) {
      return false;
    }
    if (tt != TokenKind::Semi) {
      return m.failCurrentOffset("expected semicolon after string literal");
    }
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!operandType.isIntish()) {
    return f.failf(operand, "%s is not a subtype of double?, float? or intish",
                   operandType.toChars());
  }
||||||| merged common ancestors
    if (IsNumericLiteral(f.m(), call)) {
        NumLit lit = ExtractNumericLiteral(f.m(), call);
        if (!f.writeConstExpr(lit)) {
            return false;
        }
        return CoerceResult(f, call, ret, Type::lit(lit), type);
    }
=======
template <typename Unit>
static bool CheckModuleGlobals(ModuleValidator<Unit>& m) {
  while (true) {
    ParseNode* varStmt;
    if (!ParseVarOrConstStatement(m.parser(), &varStmt)) {
      return false;
    }
    if (!varStmt) {
      break;
    }
    for (ParseNode* var = VarListHead(varStmt); var; var = NextNode(var)) {
      if (!CheckModuleGlobal(m, var,
                             varStmt->isKind(ParseNodeKind::ConstDecl))) {
        return false;
      }
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *type = Type::Signed;
  return true;
}
||||||| merged common ancestors
    ParseNode* callee = CallCallee(call);
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckBitNot(FunctionValidator& f, ParseNode* neg, Type* type) {
  MOZ_ASSERT(neg->isKind(ParseNodeKind::BitNot));
  ParseNode* operand = UnaryKid(neg);
||||||| merged common ancestors
    if (callee->isKind(ParseNodeKind::Elem)) {
        return CheckFuncPtrCall(f, call, ret, type);
    }
=======
static bool ArgFail(FunctionValidatorShared& f, PropertyName* argName,
                    ParseNode* stmt) {
  return f.failName(stmt,
                    "expecting argument type declaration for '%s' of the "
                    "form 'arg = arg|0' or 'arg = +arg' or 'arg = fround(arg)'",
                    argName);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (operand->isKind(ParseNodeKind::BitNot)) {
    return CheckCoerceToInt(f, operand, type);
  }
||||||| merged common ancestors
    if (!callee->isKind(ParseNodeKind::Name)) {
        return f.fail(callee, "unexpected callee expression type");
    }
=======
static bool CheckArgumentType(FunctionValidatorShared& f, ParseNode* stmt,
                              PropertyName* name, Type* type) {
  if (!stmt || !IsExpressionStatement(stmt)) {
    return ArgFail(f, name, stmt ? stmt : f.fn());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type operandType;
  if (!CheckExpr(f, operand, &operandType)) {
    return false;
  }
||||||| merged common ancestors
    PropertyName* calleeName = callee->as<NameNode>().name();
=======
  ParseNode* initNode = ExpressionStatementExpr(stmt);
  if (!initNode->isKind(ParseNodeKind::AssignExpr)) {
    return ArgFail(f, name, stmt);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!operandType.isIntish()) {
    return f.failf(operand, "%s is not a subtype of intish",
                   operandType.toChars());
  }
||||||| merged common ancestors
    if (const ModuleValidator::Global* global = f.lookupGlobal(calleeName)) {
        switch (global->which()) {
          case ModuleValidator::Global::FFI:
            return CheckFFICall(f, call, global->ffiIndex(), ret, type);
          case ModuleValidator::Global::MathBuiltinFunction:
            return CheckCoercedMathBuiltinCall(f, call, global->mathBuiltinFunction(), ret, type);
          case ModuleValidator::Global::ConstantLiteral:
          case ModuleValidator::Global::ConstantImport:
          case ModuleValidator::Global::Variable:
          case ModuleValidator::Global::Table:
          case ModuleValidator::Global::ArrayView:
          case ModuleValidator::Global::ArrayViewCtor:
            return f.failName(callee, "'%s' is not callable function", calleeName);
          case ModuleValidator::Global::Function:
            break;
        }
    }
=======
  ParseNode* argNode = BinaryLeft(initNode);
  ParseNode* coercionNode = BinaryRight(initNode);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!f.encoder().writeOp(MozOp::I32BitNot)) {
    return false;
  }
||||||| merged common ancestors
    return CheckInternalCall(f, call, calleeName, ret, type);
}
=======
  if (!IsUseOfName(argNode, name)) {
    return ArgFail(f, name, stmt);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *type = Type::Signed;
  return true;
}
||||||| merged common ancestors
static bool
CheckPos(FunctionValidator& f, ParseNode* pos, Type* type)
{
    MOZ_ASSERT(pos->isKind(ParseNodeKind::Pos));
    ParseNode* operand = UnaryKid(pos);
=======
  ParseNode* coercedExpr;
  if (!CheckTypeAnnotation(f.m(), coercionNode, type, &coercedExpr)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckAsExprStatement(FunctionValidator& f, ParseNode* exprStmt);
||||||| merged common ancestors
    if (operand->isKind(ParseNodeKind::Call)) {
        return CheckCoercedCall(f, operand, Type::Double, type);
    }
=======
  if (!type->isArgType()) {
    return f.failName(stmt, "invalid type for argument '%s'", name);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckComma(FunctionValidator& f, ParseNode* comma, Type* type) {
  MOZ_ASSERT(comma->isKind(ParseNodeKind::Comma));
  ParseNode* operands = ListHead(comma);
||||||| merged common ancestors
    Type actual;
    if (!CheckExpr(f, operand, &actual)) {
        return false;
    }
=======
  if (!IsUseOfName(coercedExpr, name)) {
    return ArgFail(f, name, stmt);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The block depth isn't taken into account here, because a comma list can't
  // contain breaks and continues and nested control flow structures.
  if (!f.encoder().writeOp(Op::Block)) {
    return false;
  }
||||||| merged common ancestors
    return CoerceResult(f, operand, Type::Double, actual, type);
}
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t typeAt;
  if (!f.encoder().writePatchableFixedU7(&typeAt)) {
    return false;
  }
||||||| merged common ancestors
static bool
CheckNot(FunctionValidator& f, ParseNode* expr, Type* type)
{
    MOZ_ASSERT(expr->isKind(ParseNodeKind::Not));
    ParseNode* operand = UnaryKid(expr);
=======
static bool CheckProcessingDirectives(ModuleValidatorShared& m,
                                      ParseNode** stmtIter) {
  ParseNode* stmt = *stmtIter;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* pn = operands;
  for (; NextNode(pn); pn = NextNode(pn)) {
    if (!CheckAsExprStatement(f, pn)) {
      return false;
    }
  }

  if (!CheckExpr(f, pn, type)) {
    return false;
  }
||||||| merged common ancestors
    Type operandType;
    if (!CheckExpr(f, operand, &operandType)) {
        return false;
    }

    if (!operandType.isInt()) {
        return f.failf(operand, "%s is not a subtype of int", operandType.toChars());
    }
=======
  while (stmt && IsIgnoredDirective(m.cx(), stmt)) {
    stmt = NextNode(stmt);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  f.encoder().patchFixedU7(typeAt,
                           uint8_t(type->toWasmBlockSignatureType().code()));

  return f.encoder().writeOp(Op::End);
||||||| merged common ancestors
    *type = Type::Int;
    return f.encoder().writeOp(Op::I32Eqz);
=======
  *stmtIter = stmt;
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CheckConditional(FunctionValidator& f, ParseNode* ternary,
                             Type* type) {
  MOZ_ASSERT(ternary->isKind(ParseNodeKind::Conditional));
||||||| merged common ancestors
static bool
CheckNeg(FunctionValidator& f, ParseNode* expr, Type* type)
{
    MOZ_ASSERT(expr->isKind(ParseNodeKind::Neg));
    ParseNode* operand = UnaryKid(expr);
=======
static bool CheckArguments(FunctionValidatorShared& f, ParseNode** stmtIter,
                           ValTypeVector* argTypes) {
  ParseNode* stmt = *stmtIter;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* cond = TernaryKid1(ternary);
  ParseNode* thenExpr = TernaryKid2(ternary);
  ParseNode* elseExpr = TernaryKid3(ternary);
||||||| merged common ancestors
    Type operandType;
    if (!CheckExpr(f, operand, &operandType)) {
        return false;
    }
=======
  unsigned numFormals;
  ParseNode* argpn = FunctionFormalParametersList(f.fn(), &numFormals);

  for (unsigned i = 0; i < numFormals;
       i++, argpn = NextNode(argpn), stmt = NextNode(stmt)) {
    PropertyName* name;
    if (!CheckArgument(f.m(), argpn, &name)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type condType;
  if (!CheckExpr(f, cond, &condType)) {
    return false;
  }
||||||| merged common ancestors
    if (operandType.isInt()) {
        *type = Type::Intish;
        return f.encoder().writeOp(MozOp::I32Neg);
    }
=======
    Type type;
    if (!CheckArgumentType(f, stmt, name, &type)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!condType.isInt()) {
    return f.failf(cond, "%s is not a subtype of int", condType.toChars());
  }
||||||| merged common ancestors
    if (operandType.isMaybeDouble()) {
        *type = Type::Double;
        return f.encoder().writeOp(Op::F64Neg);
    }
=======
    if (!argTypes->append(type.canonicalToValType())) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t typeAt;
  if (!f.pushIf(&typeAt)) {
    return false;
  }
||||||| merged common ancestors
    if (operandType.isMaybeFloat()) {
        *type = Type::Floatish;
        return f.encoder().writeOp(Op::F32Neg);
    }
=======
    if (!f.addLocal(argpn, name, type)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type thenType;
  if (!CheckExpr(f, thenExpr, &thenType)) {
    return false;
  }
||||||| merged common ancestors
    return f.failf(operand, "%s is not a subtype of int, float? or double?", operandType.toChars());
}
=======
  *stmtIter = stmt;
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!f.switchToElse()) {
    return false;
  }

  Type elseType;
  if (!CheckExpr(f, elseExpr, &elseType)) {
    return false;
  }
||||||| merged common ancestors
static bool
CheckCoerceToInt(FunctionValidator& f, ParseNode* expr, Type* type)
{
    MOZ_ASSERT(expr->isKind(ParseNodeKind::BitNot));
    ParseNode* operand = UnaryKid(expr);

    Type operandType;
    if (!CheckExpr(f, operand, &operandType)) {
        return false;
    }
=======
static bool IsLiteralOrConst(FunctionValidatorShared& f, ParseNode* pn,
                             NumLit* lit) {
  if (pn->isKind(ParseNodeKind::Name)) {
    const ModuleValidatorShared::Global* global =
        f.lookupGlobal(pn->as<NameNode>().name());
    if (!global ||
        global->which() != ModuleValidatorShared::Global::ConstantLiteral) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (thenType.isInt() && elseType.isInt()) {
    *type = Type::Int;
  } else if (thenType.isDouble() && elseType.isDouble()) {
    *type = Type::Double;
  } else if (thenType.isFloat() && elseType.isFloat()) {
    *type = Type::Float;
  } else {
    return f.failf(
        ternary,
        "then/else branches of conditional must both produce int, float, "
        "double, current types are %s and %s",
        thenType.toChars(), elseType.toChars());
  }

  if (!f.popIf(typeAt, type->toWasmBlockSignatureType())) {
    return false;
  }
||||||| merged common ancestors
    if (operandType.isMaybeDouble() || operandType.isMaybeFloat()) {
        *type = Type::Signed;
        Op opcode = operandType.isMaybeDouble() ? Op::I32TruncSF64 : Op::I32TruncSF32;
        return f.encoder().writeOp(opcode);
    }
=======
    *lit = global->constLiteralValue();
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    if (!operandType.isIntish()) {
        return f.failf(operand, "%s is not a subtype of double?, float? or intish", operandType.toChars());
    }
=======
  if (!IsNumericLiteral(f.m(), pn)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool IsValidIntMultiplyConstant(ModuleValidator& m, ParseNode* expr) {
  if (!IsNumericLiteral(m, expr)) {
    return false;
  }

  NumLit lit = ExtractNumericLiteral(m, expr);
  switch (lit.which()) {
    case NumLit::Fixnum:
    case NumLit::NegativeInt:
      if (abs(lit.toInt32()) < (1 << 20)) {
        return true;
      }
      return false;
    case NumLit::BigUnsigned:
    case NumLit::Double:
    case NumLit::Float:
    case NumLit::OutOfRangeInt:
      return false;
  }

  MOZ_MAKE_COMPILER_ASSUME_IS_UNREACHABLE("Bad literal");
||||||| merged common ancestors
    *type = Type::Signed;
    return true;
=======
  *lit = ExtractNumericLiteral(f.m(), pn);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CheckMultiply(FunctionValidator& f, ParseNode* star, Type* type) {
  MOZ_ASSERT(star->isKind(ParseNodeKind::Star));
  ParseNode* lhs = MultiplyLeft(star);
  ParseNode* rhs = MultiplyRight(star);
||||||| merged common ancestors
static bool
CheckBitNot(FunctionValidator& f, ParseNode* neg, Type* type)
{
    MOZ_ASSERT(neg->isKind(ParseNodeKind::BitNot));
    ParseNode* operand = UnaryKid(neg);
=======
static bool CheckFinalReturn(FunctionValidatorShared& f,
                             ParseNode* lastNonEmptyStmt) {
  if (!f.encoder().writeOp(Op::End)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type lhsType;
  if (!CheckExpr(f, lhs, &lhsType)) {
    return false;
  }
||||||| merged common ancestors
    if (operand->isKind(ParseNodeKind::BitNot)) {
        return CheckCoerceToInt(f, operand, type);
    }
=======
  if (!f.hasAlreadyReturned()) {
    f.setReturnedType(ExprType::Void);
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type rhsType;
  if (!CheckExpr(f, rhs, &rhsType)) {
    return false;
  }
||||||| merged common ancestors
    Type operandType;
    if (!CheckExpr(f, operand, &operandType)) {
        return false;
    }
=======
  if (!lastNonEmptyStmt->isKind(ParseNodeKind::ReturnStmt) &&
      !IsVoid(f.returnedType())) {
    return f.fail(lastNonEmptyStmt,
                  "void incompatible with previous return type");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (lhsType.isInt() && rhsType.isInt()) {
    if (!IsValidIntMultiplyConstant(f.m(), lhs) &&
        !IsValidIntMultiplyConstant(f.m(), rhs)) {
      return f.fail(
          star,
          "one arg to int multiply must be a small (-2^20, 2^20) int literal");
    }
    *type = Type::Intish;
    return f.encoder().writeOp(Op::I32Mul);
  }
||||||| merged common ancestors
    if (!operandType.isIntish()) {
        return f.failf(operand, "%s is not a subtype of intish", operandType.toChars());
    }
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (lhsType.isMaybeDouble() && rhsType.isMaybeDouble()) {
    *type = Type::Double;
    return f.encoder().writeOp(Op::F64Mul);
  }
||||||| merged common ancestors
    if (!f.encoder().writeOp(MozOp::I32BitNot)) {
        return false;
    }
=======
static bool CheckVariable(FunctionValidatorShared& f, ParseNode* decl,
                          ValTypeVector* types, Vector<NumLit>* inits) {
  if (!decl->isKind(ParseNodeKind::AssignExpr)) {
    return f.failName(
        decl, "var '%s' needs explicit type declaration via an initial value",
        decl->as<NameNode>().name());
  }
  AssignmentNode* assignNode = &decl->as<AssignmentNode>();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (lhsType.isMaybeFloat() && rhsType.isMaybeFloat()) {
    *type = Type::Floatish;
    return f.encoder().writeOp(Op::F32Mul);
  }

  return f.fail(
      star, "multiply operands must be both int, both double? or both float?");
}
||||||| merged common ancestors
    *type = Type::Signed;
    return true;
}
=======
  ParseNode* var = assignNode->left();
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckAddOrSub(FunctionValidator& f, ParseNode* expr, Type* type,
                          unsigned* numAddOrSubOut = nullptr) {
  if (!CheckRecursionLimitDontReport(f.cx())) {
    return f.m().failOverRecursed();
  }
||||||| merged common ancestors
static bool
CheckAsExprStatement(FunctionValidator& f, ParseNode* exprStmt);
=======
  if (!var->isKind(ParseNodeKind::Name)) {
    return f.fail(var, "local variable is not a plain name");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(expr->isKind(ParseNodeKind::Add) ||
             expr->isKind(ParseNodeKind::Sub));
  ParseNode* lhs = AddSubLeft(expr);
  ParseNode* rhs = AddSubRight(expr);
||||||| merged common ancestors
static bool
CheckComma(FunctionValidator& f, ParseNode* comma, Type* type)
{
    MOZ_ASSERT(comma->isKind(ParseNodeKind::Comma));
    ParseNode* operands = ListHead(comma);
=======
  PropertyName* name = var->as<NameNode>().name();
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type lhsType, rhsType;
  unsigned lhsNumAddOrSub, rhsNumAddOrSub;

  if (lhs->isKind(ParseNodeKind::Add) || lhs->isKind(ParseNodeKind::Sub)) {
    if (!CheckAddOrSub(f, lhs, &lhsType, &lhsNumAddOrSub)) {
      return false;
    }
    if (lhsType == Type::Intish) {
      lhsType = Type::Int;
    }
  } else {
    if (!CheckExpr(f, lhs, &lhsType)) {
      return false;
    }
    lhsNumAddOrSub = 0;
  }
||||||| merged common ancestors
    // The block depth isn't taken into account here, because a comma list can't
    // contain breaks and continues and nested control flow structures.
    if (!f.encoder().writeOp(Op::Block)) {
        return false;
    }
=======
  if (!CheckIdentifier(f.m(), var, name)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (rhs->isKind(ParseNodeKind::Add) || rhs->isKind(ParseNodeKind::Sub)) {
    if (!CheckAddOrSub(f, rhs, &rhsType, &rhsNumAddOrSub)) {
      return false;
    }
    if (rhsType == Type::Intish) {
      rhsType = Type::Int;
    }
  } else {
    if (!CheckExpr(f, rhs, &rhsType)) {
      return false;
    }
    rhsNumAddOrSub = 0;
  }

  unsigned numAddOrSub = lhsNumAddOrSub + rhsNumAddOrSub + 1;
  if (numAddOrSub > (1 << 20)) {
    return f.fail(expr, "too many + or - without intervening coercion");
  }

  if (lhsType.isInt() && rhsType.isInt()) {
    if (!f.encoder().writeOp(expr->isKind(ParseNodeKind::Add) ? Op::I32Add
                                                              : Op::I32Sub)) {
      return false;
    }
    *type = Type::Intish;
  } else if (lhsType.isMaybeDouble() && rhsType.isMaybeDouble()) {
    if (!f.encoder().writeOp(expr->isKind(ParseNodeKind::Add) ? Op::F64Add
                                                              : Op::F64Sub)) {
      return false;
    }
    *type = Type::Double;
  } else if (lhsType.isMaybeFloat() && rhsType.isMaybeFloat()) {
    if (!f.encoder().writeOp(expr->isKind(ParseNodeKind::Add) ? Op::F32Add
                                                              : Op::F32Sub)) {
      return false;
    }
    *type = Type::Floatish;
  } else {
    return f.failf(
        expr,
        "operands to + or - must both be int, float? or double?, got %s and %s",
        lhsType.toChars(), rhsType.toChars());
  }

  if (numAddOrSubOut) {
    *numAddOrSubOut = numAddOrSub;
  }
  return true;
}

static bool CheckDivOrMod(FunctionValidator& f, ParseNode* expr, Type* type) {
  MOZ_ASSERT(expr->isKind(ParseNodeKind::Div) ||
             expr->isKind(ParseNodeKind::Mod));

  ParseNode* lhs = DivOrModLeft(expr);
  ParseNode* rhs = DivOrModRight(expr);

  Type lhsType, rhsType;
  if (!CheckExpr(f, lhs, &lhsType)) {
    return false;
  }
  if (!CheckExpr(f, rhs, &rhsType)) {
    return false;
  }
||||||| merged common ancestors
    size_t typeAt;
    if (!f.encoder().writePatchableFixedU7(&typeAt)) {
        return false;
    }
=======
  ParseNode* initNode = assignNode->right();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (lhsType.isMaybeDouble() && rhsType.isMaybeDouble()) {
    *type = Type::Double;
    if (expr->isKind(ParseNodeKind::Div)) {
      return f.encoder().writeOp(Op::F64Div);
    }
    return f.encoder().writeOp(MozOp::F64Mod);
  }
||||||| merged common ancestors
    ParseNode* pn = operands;
    for (; NextNode(pn); pn = NextNode(pn)) {
        if (!CheckAsExprStatement(f, pn)) {
            return false;
        }
    }
=======
  NumLit lit;
  if (!IsLiteralOrConst(f, initNode, &lit)) {
    return f.failName(
        var, "var '%s' initializer must be literal or const literal", name);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (lhsType.isMaybeFloat() && rhsType.isMaybeFloat()) {
    *type = Type::Floatish;
    if (expr->isKind(ParseNodeKind::Div)) {
      return f.encoder().writeOp(Op::F32Div);
    } else {
      return f.fail(expr, "modulo cannot receive float arguments");
    }
  }

  if (lhsType.isSigned() && rhsType.isSigned()) {
    *type = Type::Intish;
    return f.encoder().writeOp(expr->isKind(ParseNodeKind::Div) ? Op::I32DivS
                                                                : Op::I32RemS);
  }
||||||| merged common ancestors
    if (!CheckExpr(f, pn, type)) {
        return false;
    }
=======
  if (!lit.valid()) {
    return f.failName(var, "var '%s' initializer out of range", name);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (lhsType.isUnsigned() && rhsType.isUnsigned()) {
    *type = Type::Intish;
    return f.encoder().writeOp(expr->isKind(ParseNodeKind::Div) ? Op::I32DivU
                                                                : Op::I32RemU);
  }
||||||| merged common ancestors
    f.encoder().patchFixedU7(typeAt, uint8_t(type->toWasmBlockSignatureType().code()));
=======
  Type type = Type::canonicalize(Type::lit(lit));
>>>>>>> upstream-releases

<<<<<<< HEAD
  return f.failf(
      expr,
      "arguments to / or %% must both be double?, float?, signed, or unsigned; "
      "%s and %s are given",
      lhsType.toChars(), rhsType.toChars());
||||||| merged common ancestors
    return f.encoder().writeOp(Op::End);
=======
  return f.addLocal(var, name, type) &&
         types->append(type.canonicalToValType()) && inits->append(lit);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CheckComparison(FunctionValidator& f, ParseNode* comp, Type* type) {
  MOZ_ASSERT(
      comp->isKind(ParseNodeKind::Lt) || comp->isKind(ParseNodeKind::Le) ||
      comp->isKind(ParseNodeKind::Gt) || comp->isKind(ParseNodeKind::Ge) ||
      comp->isKind(ParseNodeKind::Eq) || comp->isKind(ParseNodeKind::Ne));

  ParseNode* lhs = ComparisonLeft(comp);
  ParseNode* rhs = ComparisonRight(comp);
||||||| merged common ancestors
static bool
CheckConditional(FunctionValidator& f, ParseNode* ternary, Type* type)
{
    MOZ_ASSERT(ternary->isKind(ParseNodeKind::Conditional));

    ParseNode* cond = TernaryKid1(ternary);
    ParseNode* thenExpr = TernaryKid2(ternary);
    ParseNode* elseExpr = TernaryKid3(ternary);
=======
static bool CheckVariables(FunctionValidatorShared& f, ParseNode** stmtIter) {
  ParseNode* stmt = *stmtIter;
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type lhsType, rhsType;
  if (!CheckExpr(f, lhs, &lhsType)) {
    return false;
  }
  if (!CheckExpr(f, rhs, &rhsType)) {
    return false;
  }

  if (!(lhsType.isSigned() && rhsType.isSigned()) &&
      !(lhsType.isUnsigned() && rhsType.isUnsigned()) &&
      !(lhsType.isDouble() && rhsType.isDouble()) &&
      !(lhsType.isFloat() && rhsType.isFloat())) {
    return f.failf(comp,
                   "arguments to a comparison must both be signed, unsigned, "
                   "floats or doubles; "
                   "%s and %s are given",
                   lhsType.toChars(), rhsType.toChars());
  }

  Op stmt;
  if (lhsType.isSigned() && rhsType.isSigned()) {
    switch (comp->getKind()) {
      case ParseNodeKind::Eq:
        stmt = Op::I32Eq;
        break;
      case ParseNodeKind::Ne:
        stmt = Op::I32Ne;
        break;
      case ParseNodeKind::Lt:
        stmt = Op::I32LtS;
        break;
      case ParseNodeKind::Le:
        stmt = Op::I32LeS;
        break;
      case ParseNodeKind::Gt:
        stmt = Op::I32GtS;
        break;
      case ParseNodeKind::Ge:
        stmt = Op::I32GeS;
        break;
      default:
        MOZ_CRASH("unexpected comparison op");
    }
  } else if (lhsType.isUnsigned() && rhsType.isUnsigned()) {
    switch (comp->getKind()) {
      case ParseNodeKind::Eq:
        stmt = Op::I32Eq;
        break;
      case ParseNodeKind::Ne:
        stmt = Op::I32Ne;
        break;
      case ParseNodeKind::Lt:
        stmt = Op::I32LtU;
        break;
      case ParseNodeKind::Le:
        stmt = Op::I32LeU;
        break;
      case ParseNodeKind::Gt:
        stmt = Op::I32GtU;
        break;
      case ParseNodeKind::Ge:
        stmt = Op::I32GeU;
        break;
      default:
        MOZ_CRASH("unexpected comparison op");
    }
  } else if (lhsType.isDouble()) {
    switch (comp->getKind()) {
      case ParseNodeKind::Eq:
        stmt = Op::F64Eq;
        break;
      case ParseNodeKind::Ne:
        stmt = Op::F64Ne;
        break;
      case ParseNodeKind::Lt:
        stmt = Op::F64Lt;
        break;
      case ParseNodeKind::Le:
        stmt = Op::F64Le;
        break;
      case ParseNodeKind::Gt:
        stmt = Op::F64Gt;
        break;
      case ParseNodeKind::Ge:
        stmt = Op::F64Ge;
        break;
      default:
        MOZ_CRASH("unexpected comparison op");
    }
  } else if (lhsType.isFloat()) {
    switch (comp->getKind()) {
      case ParseNodeKind::Eq:
        stmt = Op::F32Eq;
        break;
      case ParseNodeKind::Ne:
        stmt = Op::F32Ne;
        break;
      case ParseNodeKind::Lt:
        stmt = Op::F32Lt;
        break;
      case ParseNodeKind::Le:
        stmt = Op::F32Le;
        break;
      case ParseNodeKind::Gt:
        stmt = Op::F32Gt;
        break;
      case ParseNodeKind::Ge:
        stmt = Op::F32Ge;
        break;
      default:
        MOZ_CRASH("unexpected comparison op");
    }
  } else {
    MOZ_CRASH("unexpected type");
  }

  *type = Type::Int;
  return f.encoder().writeOp(stmt);
}

static bool CheckBitwise(FunctionValidator& f, ParseNode* bitwise, Type* type) {
  ParseNode* lhs = BitwiseLeft(bitwise);
  ParseNode* rhs = BitwiseRight(bitwise);

  int32_t identityElement;
  bool onlyOnRight;
  switch (bitwise->getKind()) {
    case ParseNodeKind::BitOr:
      identityElement = 0;
      onlyOnRight = false;
      *type = Type::Signed;
      break;
    case ParseNodeKind::BitAnd:
      identityElement = -1;
      onlyOnRight = false;
      *type = Type::Signed;
      break;
    case ParseNodeKind::BitXor:
      identityElement = 0;
      onlyOnRight = false;
      *type = Type::Signed;
      break;
    case ParseNodeKind::Lsh:
      identityElement = 0;
      onlyOnRight = true;
      *type = Type::Signed;
      break;
    case ParseNodeKind::Rsh:
      identityElement = 0;
      onlyOnRight = true;
      *type = Type::Signed;
      break;
    case ParseNodeKind::Ursh:
      identityElement = 0;
      onlyOnRight = true;
      *type = Type::Unsigned;
      break;
    default:
      MOZ_CRASH("not a bitwise op");
  }

  uint32_t i;
  if (!onlyOnRight && IsLiteralInt(f.m(), lhs, &i) &&
      i == uint32_t(identityElement)) {
    Type rhsType;
    if (!CheckExpr(f, rhs, &rhsType)) {
      return false;
    }
    if (!rhsType.isIntish()) {
      return f.failf(bitwise, "%s is not a subtype of intish",
                     rhsType.toChars());
    }
    return true;
  }
||||||| merged common ancestors
    Type condType;
    if (!CheckExpr(f, cond, &condType)) {
        return false;
    }

    if (!condType.isInt()) {
        return f.failf(cond, "%s is not a subtype of int", condType.toChars());
    }

    size_t typeAt;
    if (!f.pushIf(&typeAt)) {
        return false;
    }

    Type thenType;
    if (!CheckExpr(f, thenExpr, &thenType)) {
        return false;
    }

    if (!f.switchToElse()) {
        return false;
    }

    Type elseType;
    if (!CheckExpr(f, elseExpr, &elseType)) {
        return false;
    }

    if (thenType.isInt() && elseType.isInt()) {
        *type = Type::Int;
    } else if (thenType.isDouble() && elseType.isDouble()) {
        *type = Type::Double;
    } else if (thenType.isFloat() && elseType.isFloat()) {
        *type = Type::Float;
    } else {
        return f.failf(ternary, "then/else branches of conditional must both produce int, float, "
                       "double, current types are %s and %s",
                       thenType.toChars(), elseType.toChars());
    }

    if (!f.popIf(typeAt, type->toWasmBlockSignatureType())) {
        return false;
    }

    return true;
}

static bool
IsValidIntMultiplyConstant(ModuleValidator& m, ParseNode* expr)
{
    if (!IsNumericLiteral(m, expr)) {
        return false;
    }

    NumLit lit = ExtractNumericLiteral(m, expr);
    switch (lit.which()) {
      case NumLit::Fixnum:
      case NumLit::NegativeInt:
        if (abs(lit.toInt32()) < (1<<20)) {
            return true;
        }
        return false;
      case NumLit::BigUnsigned:
      case NumLit::Double:
      case NumLit::Float:
      case NumLit::OutOfRangeInt:
        return false;
    }

    MOZ_MAKE_COMPILER_ASSUME_IS_UNREACHABLE("Bad literal");
}

static bool
CheckMultiply(FunctionValidator& f, ParseNode* star, Type* type)
{
    MOZ_ASSERT(star->isKind(ParseNodeKind::Star));
    ParseNode* lhs = MultiplyLeft(star);
    ParseNode* rhs = MultiplyRight(star);

    Type lhsType;
    if (!CheckExpr(f, lhs, &lhsType)) {
        return false;
    }

    Type rhsType;
    if (!CheckExpr(f, rhs, &rhsType)) {
        return false;
    }

    if (lhsType.isInt() && rhsType.isInt()) {
        if (!IsValidIntMultiplyConstant(f.m(), lhs) && !IsValidIntMultiplyConstant(f.m(), rhs)) {
            return f.fail(star, "one arg to int multiply must be a small (-2^20, 2^20) int literal");
        }
        *type = Type::Intish;
        return f.encoder().writeOp(Op::I32Mul);
    }

    if (lhsType.isMaybeDouble() && rhsType.isMaybeDouble()) {
        *type = Type::Double;
        return f.encoder().writeOp(Op::F64Mul);
    }

    if (lhsType.isMaybeFloat() && rhsType.isMaybeFloat()) {
        *type = Type::Floatish;
        return f.encoder().writeOp(Op::F32Mul);
    }

    return f.fail(star, "multiply operands must be both int, both double? or both float?");
}

static bool
CheckAddOrSub(FunctionValidator& f, ParseNode* expr, Type* type, unsigned* numAddOrSubOut = nullptr)
{
    if (!CheckRecursionLimitDontReport(f.cx())) {
        return f.m().failOverRecursed();
    }

    MOZ_ASSERT(expr->isKind(ParseNodeKind::Add) || expr->isKind(ParseNodeKind::Sub));
    ParseNode* lhs = AddSubLeft(expr);
    ParseNode* rhs = AddSubRight(expr);

    Type lhsType, rhsType;
    unsigned lhsNumAddOrSub, rhsNumAddOrSub;

    if (lhs->isKind(ParseNodeKind::Add) || lhs->isKind(ParseNodeKind::Sub)) {
        if (!CheckAddOrSub(f, lhs, &lhsType, &lhsNumAddOrSub)) {
            return false;
        }
        if (lhsType == Type::Intish) {
            lhsType = Type::Int;
        }
    } else {
        if (!CheckExpr(f, lhs, &lhsType)) {
            return false;
        }
        lhsNumAddOrSub = 0;
    }

    if (rhs->isKind(ParseNodeKind::Add) || rhs->isKind(ParseNodeKind::Sub)) {
        if (!CheckAddOrSub(f, rhs, &rhsType, &rhsNumAddOrSub)) {
            return false;
        }
        if (rhsType == Type::Intish) {
            rhsType = Type::Int;
        }
    } else {
        if (!CheckExpr(f, rhs, &rhsType)) {
            return false;
        }
        rhsNumAddOrSub = 0;
    }

    unsigned numAddOrSub = lhsNumAddOrSub + rhsNumAddOrSub + 1;
    if (numAddOrSub > (1<<20)) {
        return f.fail(expr, "too many + or - without intervening coercion");
    }

    if (lhsType.isInt() && rhsType.isInt()) {
        if (!f.encoder().writeOp(expr->isKind(ParseNodeKind::Add) ? Op::I32Add : Op::I32Sub)) {
            return false;
        }
        *type = Type::Intish;
    } else if (lhsType.isMaybeDouble() && rhsType.isMaybeDouble()) {
        if (!f.encoder().writeOp(expr->isKind(ParseNodeKind::Add) ? Op::F64Add : Op::F64Sub)) {
            return false;
        }
        *type = Type::Double;
    } else if (lhsType.isMaybeFloat() && rhsType.isMaybeFloat()) {
        if (!f.encoder().writeOp(expr->isKind(ParseNodeKind::Add) ? Op::F32Add : Op::F32Sub)) {
            return false;
        }
        *type = Type::Floatish;
    } else {
        return f.failf(expr, "operands to + or - must both be int, float? or double?, got %s and %s",
                       lhsType.toChars(), rhsType.toChars());
    }

    if (numAddOrSubOut) {
        *numAddOrSubOut = numAddOrSub;
    }
    return true;
}

static bool
CheckDivOrMod(FunctionValidator& f, ParseNode* expr, Type* type)
{
    MOZ_ASSERT(expr->isKind(ParseNodeKind::Div) || expr->isKind(ParseNodeKind::Mod));

    ParseNode* lhs = DivOrModLeft(expr);
    ParseNode* rhs = DivOrModRight(expr);

    Type lhsType, rhsType;
    if (!CheckExpr(f, lhs, &lhsType)) {
        return false;
    }
    if (!CheckExpr(f, rhs, &rhsType)) {
        return false;
    }

    if (lhsType.isMaybeDouble() && rhsType.isMaybeDouble()) {
        *type = Type::Double;
        if (expr->isKind(ParseNodeKind::Div)) {
            return f.encoder().writeOp(Op::F64Div);
        }
        return f.encoder().writeOp(MozOp::F64Mod);
    }

    if (lhsType.isMaybeFloat() && rhsType.isMaybeFloat()) {
        *type = Type::Floatish;
        if (expr->isKind(ParseNodeKind::Div)) {
            return f.encoder().writeOp(Op::F32Div);
        } else {
            return f.fail(expr, "modulo cannot receive float arguments");
        }
    }

    if (lhsType.isSigned() && rhsType.isSigned()) {
        *type = Type::Intish;
        return f.encoder().writeOp(expr->isKind(ParseNodeKind::Div) ? Op::I32DivS : Op::I32RemS);
    }

    if (lhsType.isUnsigned() && rhsType.isUnsigned()) {
        *type = Type::Intish;
        return f.encoder().writeOp(expr->isKind(ParseNodeKind::Div) ? Op::I32DivU : Op::I32RemU);
    }

    return f.failf(expr, "arguments to / or %% must both be double?, float?, signed, or unsigned; "
                   "%s and %s are given", lhsType.toChars(), rhsType.toChars());
}

static bool
CheckComparison(FunctionValidator& f, ParseNode* comp, Type* type)
{
    MOZ_ASSERT(comp->isKind(ParseNodeKind::Lt) ||
               comp->isKind(ParseNodeKind::Le) ||
               comp->isKind(ParseNodeKind::Gt) ||
               comp->isKind(ParseNodeKind::Ge) ||
               comp->isKind(ParseNodeKind::Eq) ||
               comp->isKind(ParseNodeKind::Ne));

    ParseNode* lhs = ComparisonLeft(comp);
    ParseNode* rhs = ComparisonRight(comp);

    Type lhsType, rhsType;
    if (!CheckExpr(f, lhs, &lhsType)) {
        return false;
    }
    if (!CheckExpr(f, rhs, &rhsType)) {
        return false;
    }

    if (!(lhsType.isSigned() && rhsType.isSigned()) &&
        !(lhsType.isUnsigned() && rhsType.isUnsigned()) &&
        !(lhsType.isDouble() && rhsType.isDouble()) &&
        !(lhsType.isFloat() && rhsType.isFloat()))
    {
        return f.failf(comp, "arguments to a comparison must both be signed, unsigned, floats or doubles; "
                       "%s and %s are given", lhsType.toChars(), rhsType.toChars());
    }

    Op stmt;
    if (lhsType.isSigned() && rhsType.isSigned()) {
        switch (comp->getKind()) {
          case ParseNodeKind::Eq: stmt = Op::I32Eq;  break;
          case ParseNodeKind::Ne: stmt = Op::I32Ne;  break;
          case ParseNodeKind::Lt: stmt = Op::I32LtS; break;
          case ParseNodeKind::Le: stmt = Op::I32LeS; break;
          case ParseNodeKind::Gt: stmt = Op::I32GtS; break;
          case ParseNodeKind::Ge: stmt = Op::I32GeS; break;
          default: MOZ_CRASH("unexpected comparison op");
        }
    } else if (lhsType.isUnsigned() && rhsType.isUnsigned()) {
        switch (comp->getKind()) {
          case ParseNodeKind::Eq: stmt = Op::I32Eq;  break;
          case ParseNodeKind::Ne: stmt = Op::I32Ne;  break;
          case ParseNodeKind::Lt: stmt = Op::I32LtU; break;
          case ParseNodeKind::Le: stmt = Op::I32LeU; break;
          case ParseNodeKind::Gt: stmt = Op::I32GtU; break;
          case ParseNodeKind::Ge: stmt = Op::I32GeU; break;
          default: MOZ_CRASH("unexpected comparison op");
        }
    } else if (lhsType.isDouble()) {
        switch (comp->getKind()) {
          case ParseNodeKind::Eq: stmt = Op::F64Eq; break;
          case ParseNodeKind::Ne: stmt = Op::F64Ne; break;
          case ParseNodeKind::Lt: stmt = Op::F64Lt; break;
          case ParseNodeKind::Le: stmt = Op::F64Le; break;
          case ParseNodeKind::Gt: stmt = Op::F64Gt; break;
          case ParseNodeKind::Ge: stmt = Op::F64Ge; break;
          default: MOZ_CRASH("unexpected comparison op");
        }
    } else if (lhsType.isFloat()) {
        switch (comp->getKind()) {
          case ParseNodeKind::Eq: stmt = Op::F32Eq; break;
          case ParseNodeKind::Ne: stmt = Op::F32Ne; break;
          case ParseNodeKind::Lt: stmt = Op::F32Lt; break;
          case ParseNodeKind::Le: stmt = Op::F32Le; break;
          case ParseNodeKind::Gt: stmt = Op::F32Gt; break;
          case ParseNodeKind::Ge: stmt = Op::F32Ge; break;
          default: MOZ_CRASH("unexpected comparison op");
        }
    } else {
        MOZ_CRASH("unexpected type");
    }

    *type = Type::Int;
    return f.encoder().writeOp(stmt);
}

static bool
CheckBitwise(FunctionValidator& f, ParseNode* bitwise, Type* type)
{
    ParseNode* lhs = BitwiseLeft(bitwise);
    ParseNode* rhs = BitwiseRight(bitwise);

    int32_t identityElement;
    bool onlyOnRight;
    switch (bitwise->getKind()) {
      case ParseNodeKind::BitOr:
        identityElement = 0;
        onlyOnRight = false;
        *type = Type::Signed;
        break;
      case ParseNodeKind::BitAnd:
        identityElement = -1;
        onlyOnRight = false;
        *type = Type::Signed;
        break;
      case ParseNodeKind::BitXor:
        identityElement = 0;
        onlyOnRight = false;
        *type = Type::Signed;
        break;
      case ParseNodeKind::Lsh:
        identityElement = 0;
        onlyOnRight = true;
        *type = Type::Signed;
        break;
      case ParseNodeKind::Rsh:
        identityElement = 0;
        onlyOnRight = true;
        *type = Type::Signed;
        break;
      case ParseNodeKind::Ursh:
        identityElement = 0;
        onlyOnRight = true;
        *type = Type::Unsigned;
        break;
      default: MOZ_CRASH("not a bitwise op");
    }

    uint32_t i;
    if (!onlyOnRight && IsLiteralInt(f.m(), lhs, &i) && i == uint32_t(identityElement)) {
        Type rhsType;
        if (!CheckExpr(f, rhs, &rhsType)) {
            return false;
        }
        if (!rhsType.isIntish()) {
            return f.failf(bitwise, "%s is not a subtype of intish", rhsType.toChars());
        }
        return true;
    }

    if (IsLiteralInt(f.m(), rhs, &i) && i == uint32_t(identityElement)) {
        if (bitwise->isKind(ParseNodeKind::BitOr) && lhs->isKind(ParseNodeKind::Call)) {
            return CheckCoercedCall(f, lhs, Type::Int, type);
        }

        Type lhsType;
        if (!CheckExpr(f, lhs, &lhsType)) {
            return false;
        }
        if (!lhsType.isIntish()) {
            return f.failf(bitwise, "%s is not a subtype of intish", lhsType.toChars());
        }
        return true;
    }

    Type lhsType;
    if (!CheckExpr(f, lhs, &lhsType)) {
        return false;
    }

    Type rhsType;
    if (!CheckExpr(f, rhs, &rhsType)) {
        return false;
    }

    if (!lhsType.isIntish()) {
        return f.failf(lhs, "%s is not a subtype of intish", lhsType.toChars());
    }
    if (!rhsType.isIntish()) {
        return f.failf(rhs, "%s is not a subtype of intish", rhsType.toChars());
    }

    switch (bitwise->getKind()) {
      case ParseNodeKind::BitOr:  if (!f.encoder().writeOp(Op::I32Or))   return false; break;
      case ParseNodeKind::BitAnd: if (!f.encoder().writeOp(Op::I32And))  return false; break;
      case ParseNodeKind::BitXor: if (!f.encoder().writeOp(Op::I32Xor))  return false; break;
      case ParseNodeKind::Lsh:    if (!f.encoder().writeOp(Op::I32Shl))  return false; break;
      case ParseNodeKind::Rsh:    if (!f.encoder().writeOp(Op::I32ShrS)) return false; break;
      case ParseNodeKind::Ursh:   if (!f.encoder().writeOp(Op::I32ShrU)) return false; break;
      default: MOZ_CRASH("not a bitwise op");
    }

    return true;
}

static bool
CheckExpr(FunctionValidator& f, ParseNode* expr, Type* type)
{
    if (!CheckRecursionLimitDontReport(f.cx())) {
        return f.m().failOverRecursed();
    }

    if (IsNumericLiteral(f.m(), expr)) {
        return CheckNumericLiteral(f, expr, type);
    }

    switch (expr->getKind()) {
      case ParseNodeKind::Name:        return CheckVarRef(f, expr, type);
      case ParseNodeKind::Elem:        return CheckLoadArray(f, expr, type);
      case ParseNodeKind::Assign:      return CheckAssign(f, expr, type);
      case ParseNodeKind::Pos:         return CheckPos(f, expr, type);
      case ParseNodeKind::Not:         return CheckNot(f, expr, type);
      case ParseNodeKind::Neg:         return CheckNeg(f, expr, type);
      case ParseNodeKind::BitNot:      return CheckBitNot(f, expr, type);
      case ParseNodeKind::Comma:       return CheckComma(f, expr, type);
      case ParseNodeKind::Conditional: return CheckConditional(f, expr, type);
      case ParseNodeKind::Star:        return CheckMultiply(f, expr, type);
      case ParseNodeKind::Call:        return CheckUncoercedCall(f, expr, type);

      case ParseNodeKind::Add:
      case ParseNodeKind::Sub:         return CheckAddOrSub(f, expr, type);

      case ParseNodeKind::Div:
      case ParseNodeKind::Mod:         return CheckDivOrMod(f, expr, type);

      case ParseNodeKind::Lt:
      case ParseNodeKind::Le:
      case ParseNodeKind::Gt:
      case ParseNodeKind::Ge:
      case ParseNodeKind::Eq:
      case ParseNodeKind::Ne:          return CheckComparison(f, expr, type);

      case ParseNodeKind::BitOr:
      case ParseNodeKind::BitAnd:
      case ParseNodeKind::BitXor:
      case ParseNodeKind::Lsh:
      case ParseNodeKind::Rsh:
      case ParseNodeKind::Ursh:        return CheckBitwise(f, expr, type);

      default:;
    }

    return f.fail(expr, "unsupported expression");
}

static bool
CheckStatement(FunctionValidator& f, ParseNode* stmt);

static bool
CheckAsExprStatement(FunctionValidator& f, ParseNode* expr)
{
    if (expr->isKind(ParseNodeKind::Call)) {
        Type ignored;
        return CheckCoercedCall(f, expr, Type::Void, &ignored);
    }

    Type resultType;
    if (!CheckExpr(f, expr, &resultType)) {
        return false;
    }

    if (!resultType.isVoid()) {
        if (!f.encoder().writeOp(Op::Drop)) {
            return false;
        }
    }

    return true;
}

static bool
CheckExprStatement(FunctionValidator& f, ParseNode* exprStmt)
{
    MOZ_ASSERT(exprStmt->isKind(ParseNodeKind::ExpressionStatement));
    return CheckAsExprStatement(f, UnaryKid(exprStmt));
}

static bool
CheckLoopConditionOnEntry(FunctionValidator& f, ParseNode* cond)
{
    uint32_t maybeLit;
    if (IsLiteralInt(f.m(), cond, &maybeLit) && maybeLit) {
        return true;
    }
=======
  uint32_t firstVar = f.numLocals();

  ValTypeVector types;
  Vector<NumLit> inits(f.cx());

  for (; stmt && stmt->isKind(ParseNodeKind::VarStmt);
       stmt = NextNonEmptyStatement(stmt)) {
    for (ParseNode* var = VarListHead(stmt); var; var = NextNode(var)) {
      if (!CheckVariable(f, var, &types, &inits)) {
        return false;
      }
    }
  }

  MOZ_ASSERT(f.encoder().empty());

  if (!EncodeLocalEntries(f.encoder(), types)) {
    return false;
  }

  for (uint32_t i = 0; i < inits.length(); i++) {
    NumLit lit = inits[i];
    if (lit.isZeroBits()) {
      continue;
    }
    if (!f.writeConstExpr(lit)) {
      return false;
    }
    if (!f.encoder().writeOp(Op::SetLocal)) {
      return false;
    }
    if (!f.encoder().writeVarU32(firstVar + i)) {
      return false;
    }
  }

  *stmtIter = stmt;
  return true;
}

template <typename Unit>
static bool CheckExpr(FunctionValidator<Unit>& f, ParseNode* op, Type* type);

template <typename Unit>
static bool CheckNumericLiteral(FunctionValidator<Unit>& f, ParseNode* num,
                                Type* type) {
  NumLit lit = ExtractNumericLiteral(f.m(), num);
  if (!lit.valid()) {
    return f.fail(num, "numeric literal out of representable integer range");
  }
  *type = Type::lit(lit);
  return f.writeConstExpr(lit);
}

static bool CheckVarRef(FunctionValidatorShared& f, ParseNode* varRef,
                        Type* type) {
  PropertyName* name = varRef->as<NameNode>().name();

  if (const FunctionValidatorShared::Local* local = f.lookupLocal(name)) {
    if (!f.encoder().writeOp(Op::GetLocal)) {
      return false;
    }
    if (!f.encoder().writeVarU32(local->slot)) {
      return false;
    }
    *type = local->type;
    return true;
  }

  if (const ModuleValidatorShared::Global* global = f.lookupGlobal(name)) {
    switch (global->which()) {
      case ModuleValidatorShared::Global::ConstantLiteral:
        *type = global->varOrConstType();
        return f.writeConstExpr(global->constLiteralValue());
      case ModuleValidatorShared::Global::ConstantImport:
      case ModuleValidatorShared::Global::Variable: {
        *type = global->varOrConstType();
        return f.encoder().writeOp(Op::GetGlobal) &&
               f.encoder().writeVarU32(global->varOrConstIndex());
      }
      case ModuleValidatorShared::Global::Function:
      case ModuleValidatorShared::Global::FFI:
      case ModuleValidatorShared::Global::MathBuiltinFunction:
      case ModuleValidatorShared::Global::Table:
      case ModuleValidatorShared::Global::ArrayView:
      case ModuleValidatorShared::Global::ArrayViewCtor:
        break;
    }
    return f.failName(varRef,
                      "'%s' may not be accessed by ordinary expressions", name);
  }

  return f.failName(varRef, "'%s' not found in local or asm.js module scope",
                    name);
}

static inline bool IsLiteralOrConstInt(FunctionValidatorShared& f,
                                       ParseNode* pn, uint32_t* u32) {
  NumLit lit;
  if (!IsLiteralOrConst(f, pn, &lit)) {
    return false;
  }

  return IsLiteralInt(lit, u32);
}

static const int32_t NoMask = -1;

template <typename Unit>
static bool CheckArrayAccess(FunctionValidator<Unit>& f, ParseNode* viewName,
                             ParseNode* indexExpr, Scalar::Type* viewType) {
  if (!viewName->isKind(ParseNodeKind::Name)) {
    return f.fail(viewName,
                  "base of array access must be a typed array view name");
  }

  const ModuleValidatorShared::Global* global =
      f.lookupGlobal(viewName->as<NameNode>().name());
  if (!global || !global->isAnyArrayView()) {
    return f.fail(viewName,
                  "base of array access must be a typed array view name");
  }

  *viewType = global->viewType();

  uint32_t index;
  if (IsLiteralOrConstInt(f, indexExpr, &index)) {
    uint64_t byteOffset = uint64_t(index) << TypedArrayShift(*viewType);
    uint64_t width = TypedArrayElemSize(*viewType);
    if (!f.m().tryConstantAccess(byteOffset, width)) {
      return f.fail(indexExpr, "constant index out of range");
    }

    return f.writeInt32Lit(byteOffset);
  }

  // Mask off the low bits to account for the clearing effect of a right shift
  // followed by the left shift implicit in the array access. E.g., H32[i>>2]
  // loses the low two bits.
  int32_t mask = ~(TypedArrayElemSize(*viewType) - 1);

  if (indexExpr->isKind(ParseNodeKind::RshExpr)) {
    ParseNode* shiftAmountNode = BitwiseRight(indexExpr);

    uint32_t shift;
    if (!IsLiteralInt(f.m(), shiftAmountNode, &shift)) {
      return f.failf(shiftAmountNode, "shift amount must be constant");
    }

    unsigned requiredShift = TypedArrayShift(*viewType);
    if (shift != requiredShift) {
      return f.failf(shiftAmountNode, "shift amount must be %u", requiredShift);
    }

    ParseNode* pointerNode = BitwiseLeft(indexExpr);

    Type pointerType;
    if (!CheckExpr(f, pointerNode, &pointerType)) {
      return false;
    }

    if (!pointerType.isIntish()) {
      return f.failf(pointerNode, "%s is not a subtype of int",
                     pointerType.toChars());
    }
  } else {
    // For legacy scalar access compatibility, accept Int8/Uint8 accesses
    // with no shift.
    if (TypedArrayShift(*viewType) != 0) {
      return f.fail(
          indexExpr,
          "index expression isn't shifted; must be an Int8/Uint8 access");
    }

    MOZ_ASSERT(mask == NoMask);

    ParseNode* pointerNode = indexExpr;

    Type pointerType;
    if (!CheckExpr(f, pointerNode, &pointerType)) {
      return false;
    }
    if (!pointerType.isInt()) {
      return f.failf(pointerNode, "%s is not a subtype of int",
                     pointerType.toChars());
    }
  }

  // Don't generate the mask op if there is no need for it which could happen
  // for a shift of zero.
  if (mask != NoMask) {
    return f.writeInt32Lit(mask) && f.encoder().writeOp(Op::I32And);
  }

  return true;
}

static bool WriteArrayAccessFlags(FunctionValidatorShared& f,
                                  Scalar::Type viewType) {
  // asm.js only has naturally-aligned accesses.
  size_t align = TypedArrayElemSize(viewType);
  MOZ_ASSERT(IsPowerOfTwo(align));
  if (!f.encoder().writeFixedU8(CeilingLog2(align))) {
    return false;
  }

  // asm.js doesn't have constant offsets, so just encode a 0.
  if (!f.encoder().writeVarU32(0)) {
    return false;
  }

  return true;
}

template <typename Unit>
static bool CheckLoadArray(FunctionValidator<Unit>& f, ParseNode* elem,
                           Type* type) {
  Scalar::Type viewType;

  if (!CheckArrayAccess(f, ElemBase(elem), ElemIndex(elem), &viewType)) {
    return false;
  }

  switch (viewType) {
    case Scalar::Int8:
      if (!f.encoder().writeOp(Op::I32Load8S)) return false;
      break;
    case Scalar::Uint8:
      if (!f.encoder().writeOp(Op::I32Load8U)) return false;
      break;
    case Scalar::Int16:
      if (!f.encoder().writeOp(Op::I32Load16S)) return false;
      break;
    case Scalar::Uint16:
      if (!f.encoder().writeOp(Op::I32Load16U)) return false;
      break;
    case Scalar::Uint32:
    case Scalar::Int32:
      if (!f.encoder().writeOp(Op::I32Load)) return false;
      break;
    case Scalar::Float32:
      if (!f.encoder().writeOp(Op::F32Load)) return false;
      break;
    case Scalar::Float64:
      if (!f.encoder().writeOp(Op::F64Load)) return false;
      break;
    default:
      MOZ_CRASH("unexpected scalar type");
  }

  switch (viewType) {
    case Scalar::Int8:
    case Scalar::Int16:
    case Scalar::Int32:
    case Scalar::Uint8:
    case Scalar::Uint16:
    case Scalar::Uint32:
      *type = Type::Intish;
      break;
    case Scalar::Float32:
      *type = Type::MaybeFloat;
      break;
    case Scalar::Float64:
      *type = Type::MaybeDouble;
      break;
    default:
      MOZ_CRASH("Unexpected array type");
  }

  if (!WriteArrayAccessFlags(f, viewType)) {
    return false;
  }

  return true;
}

template <typename Unit>
static bool CheckStoreArray(FunctionValidator<Unit>& f, ParseNode* lhs,
                            ParseNode* rhs, Type* type) {
  Scalar::Type viewType;
  if (!CheckArrayAccess(f, ElemBase(lhs), ElemIndex(lhs), &viewType)) {
    return false;
  }

  Type rhsType;
  if (!CheckExpr(f, rhs, &rhsType)) {
    return false;
  }

  switch (viewType) {
    case Scalar::Int8:
    case Scalar::Int16:
    case Scalar::Int32:
    case Scalar::Uint8:
    case Scalar::Uint16:
    case Scalar::Uint32:
      if (!rhsType.isIntish()) {
        return f.failf(lhs, "%s is not a subtype of intish", rhsType.toChars());
      }
      break;
    case Scalar::Float32:
      if (!rhsType.isMaybeDouble() && !rhsType.isFloatish()) {
        return f.failf(lhs, "%s is not a subtype of double? or floatish",
                       rhsType.toChars());
      }
      break;
    case Scalar::Float64:
      if (!rhsType.isMaybeFloat() && !rhsType.isMaybeDouble()) {
        return f.failf(lhs, "%s is not a subtype of float? or double?",
                       rhsType.toChars());
      }
      break;
    default:
      MOZ_CRASH("Unexpected view type");
  }

  switch (viewType) {
    case Scalar::Int8:
    case Scalar::Uint8:
      if (!f.encoder().writeOp(MozOp::I32TeeStore8)) {
        return false;
      }
      break;
    case Scalar::Int16:
    case Scalar::Uint16:
      if (!f.encoder().writeOp(MozOp::I32TeeStore16)) {
        return false;
      }
      break;
    case Scalar::Int32:
    case Scalar::Uint32:
      if (!f.encoder().writeOp(MozOp::I32TeeStore)) {
        return false;
      }
      break;
    case Scalar::Float32:
      if (rhsType.isFloatish()) {
        if (!f.encoder().writeOp(MozOp::F32TeeStore)) {
          return false;
        }
      } else {
        if (!f.encoder().writeOp(MozOp::F64TeeStoreF32)) {
          return false;
        }
      }
      break;
    case Scalar::Float64:
      if (rhsType.isFloatish()) {
        if (!f.encoder().writeOp(MozOp::F32TeeStoreF64)) {
          return false;
        }
      } else {
        if (!f.encoder().writeOp(MozOp::F64TeeStore)) {
          return false;
        }
      }
      break;
    default:
      MOZ_CRASH("unexpected scalar type");
  }

  if (!WriteArrayAccessFlags(f, viewType)) {
    return false;
  }

  *type = rhsType;
  return true;
}

template <typename Unit>
static bool CheckAssignName(FunctionValidator<Unit>& f, ParseNode* lhs,
                            ParseNode* rhs, Type* type) {
  RootedPropertyName name(f.cx(), lhs->as<NameNode>().name());

  if (const FunctionValidatorShared::Local* lhsVar = f.lookupLocal(name)) {
    Type rhsType;
    if (!CheckExpr(f, rhs, &rhsType)) {
      return false;
    }

    if (!f.encoder().writeOp(Op::TeeLocal)) {
      return false;
    }
    if (!f.encoder().writeVarU32(lhsVar->slot)) {
      return false;
    }

    if (!(rhsType <= lhsVar->type)) {
      return f.failf(lhs, "%s is not a subtype of %s", rhsType.toChars(),
                     lhsVar->type.toChars());
    }
    *type = rhsType;
    return true;
  }

  if (const ModuleValidatorShared::Global* global = f.lookupGlobal(name)) {
    if (global->which() != ModuleValidatorShared::Global::Variable) {
      return f.failName(lhs, "'%s' is not a mutable variable", name);
    }

    Type rhsType;
    if (!CheckExpr(f, rhs, &rhsType)) {
      return false;
    }

    Type globType = global->varOrConstType();
    if (!(rhsType <= globType)) {
      return f.failf(lhs, "%s is not a subtype of %s", rhsType.toChars(),
                     globType.toChars());
    }
    if (!f.encoder().writeOp(MozOp::TeeGlobal)) {
      return false;
    }
    if (!f.encoder().writeVarU32(global->varOrConstIndex())) {
      return false;
    }

    *type = rhsType;
    return true;
  }

  return f.failName(lhs, "'%s' not found in local or asm.js module scope",
                    name);
}

template <typename Unit>
static bool CheckAssign(FunctionValidator<Unit>& f, ParseNode* assign,
                        Type* type) {
  MOZ_ASSERT(assign->isKind(ParseNodeKind::AssignExpr));

  ParseNode* lhs = BinaryLeft(assign);
  ParseNode* rhs = BinaryRight(assign);

  if (lhs->getKind() == ParseNodeKind::ElemExpr) {
    return CheckStoreArray(f, lhs, rhs, type);
  }

  if (lhs->getKind() == ParseNodeKind::Name) {
    return CheckAssignName(f, lhs, rhs, type);
  }

  return f.fail(
      assign,
      "left-hand side of assignment must be a variable or array access");
}

template <typename Unit>
static bool CheckMathIMul(FunctionValidator<Unit>& f, ParseNode* call,
                          Type* type) {
  if (CallArgListLength(call) != 2) {
    return f.fail(call, "Math.imul must be passed 2 arguments");
  }

  ParseNode* lhs = CallArgList(call);
  ParseNode* rhs = NextNode(lhs);

  Type lhsType;
  if (!CheckExpr(f, lhs, &lhsType)) {
    return false;
  }

  Type rhsType;
  if (!CheckExpr(f, rhs, &rhsType)) {
    return false;
  }

  if (!lhsType.isIntish()) {
    return f.failf(lhs, "%s is not a subtype of intish", lhsType.toChars());
  }
  if (!rhsType.isIntish()) {
    return f.failf(rhs, "%s is not a subtype of intish", rhsType.toChars());
  }

  *type = Type::Signed;
  return f.encoder().writeOp(Op::I32Mul);
}

template <typename Unit>
static bool CheckMathClz32(FunctionValidator<Unit>& f, ParseNode* call,
                           Type* type) {
  if (CallArgListLength(call) != 1) {
    return f.fail(call, "Math.clz32 must be passed 1 argument");
  }

  ParseNode* arg = CallArgList(call);

  Type argType;
  if (!CheckExpr(f, arg, &argType)) {
    return false;
  }

  if (!argType.isIntish()) {
    return f.failf(arg, "%s is not a subtype of intish", argType.toChars());
  }

  *type = Type::Fixnum;
  return f.encoder().writeOp(Op::I32Clz);
}

template <typename Unit>
static bool CheckMathAbs(FunctionValidator<Unit>& f, ParseNode* call,
                         Type* type) {
  if (CallArgListLength(call) != 1) {
    return f.fail(call, "Math.abs must be passed 1 argument");
  }

  ParseNode* arg = CallArgList(call);

  Type argType;
  if (!CheckExpr(f, arg, &argType)) {
    return false;
  }

  if (argType.isSigned()) {
    *type = Type::Unsigned;
    return f.encoder().writeOp(MozOp::I32Abs);
  }

  if (argType.isMaybeDouble()) {
    *type = Type::Double;
    return f.encoder().writeOp(Op::F64Abs);
  }

  if (argType.isMaybeFloat()) {
    *type = Type::Floatish;
    return f.encoder().writeOp(Op::F32Abs);
  }

  return f.failf(call, "%s is not a subtype of signed, float? or double?",
                 argType.toChars());
}

template <typename Unit>
static bool CheckMathSqrt(FunctionValidator<Unit>& f, ParseNode* call,
                          Type* type) {
  if (CallArgListLength(call) != 1) {
    return f.fail(call, "Math.sqrt must be passed 1 argument");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (IsLiteralInt(f.m(), rhs, &i) && i == uint32_t(identityElement)) {
    if (bitwise->isKind(ParseNodeKind::BitOr) &&
        lhs->isKind(ParseNodeKind::Call)) {
      return CheckCoercedCall(f, lhs, Type::Int, type);
    }
||||||| merged common ancestors
    Type condType;
    if (!CheckExpr(f, cond, &condType)) {
        return false;
    }
    if (!condType.isInt()) {
        return f.failf(cond, "%s is not a subtype of int", condType.toChars());
    }
=======
  ParseNode* arg = CallArgList(call);
>>>>>>> upstream-releases

<<<<<<< HEAD
    Type lhsType;
    if (!CheckExpr(f, lhs, &lhsType)) {
      return false;
    }
    if (!lhsType.isIntish()) {
      return f.failf(bitwise, "%s is not a subtype of intish",
                     lhsType.toChars());
    }
    return true;
  }
||||||| merged common ancestors
    if (!f.encoder().writeOp(Op::I32Eqz)) {
        return false;
    }

    // brIf (i32.eqz $f) $out
    if (!f.writeBreakIf()) {
        return false;
    }

    return true;
}

static bool
CheckWhile(FunctionValidator& f, ParseNode* whileStmt, const LabelVector* labels = nullptr)
{
    MOZ_ASSERT(whileStmt->isKind(ParseNodeKind::While));
    ParseNode* cond = BinaryLeft(whileStmt);
    ParseNode* body = BinaryRight(whileStmt);

    // A while loop `while(#cond) #body` is equivalent to:
    // (block $after_loop
    //    (loop $top
    //       (brIf $after_loop (i32.eq 0 #cond))
    //       #body
    //       (br $top)
    //    )
    // )
    if (labels && !f.addLabels(*labels, 0, 1)) {
        return false;
    }

    if (!f.pushLoop()) {
        return false;
    }
=======
  Type argType;
  if (!CheckExpr(f, arg, &argType)) {
    return false;
  }

  if (argType.isMaybeDouble()) {
    *type = Type::Double;
    return f.encoder().writeOp(Op::F64Sqrt);
  }

  if (argType.isMaybeFloat()) {
    *type = Type::Floatish;
    return f.encoder().writeOp(Op::F32Sqrt);
  }

  return f.failf(call, "%s is neither a subtype of double? nor float?",
                 argType.toChars());
}

template <typename Unit>
static bool CheckMathMinMax(FunctionValidator<Unit>& f, ParseNode* callNode,
                            bool isMax, Type* type) {
  if (CallArgListLength(callNode) < 2) {
    return f.fail(callNode, "Math.min/max must be passed at least 2 arguments");
  }

  ParseNode* firstArg = CallArgList(callNode);
  Type firstType;
  if (!CheckExpr(f, firstArg, &firstType)) {
    return false;
  }

  Op op = Op::Limit;
  MozOp mozOp = MozOp::Limit;
  if (firstType.isMaybeDouble()) {
    *type = Type::Double;
    firstType = Type::MaybeDouble;
    op = isMax ? Op::F64Max : Op::F64Min;
  } else if (firstType.isMaybeFloat()) {
    *type = Type::Float;
    firstType = Type::MaybeFloat;
    op = isMax ? Op::F32Max : Op::F32Min;
  } else if (firstType.isSigned()) {
    *type = Type::Signed;
    firstType = Type::Signed;
    mozOp = isMax ? MozOp::I32Max : MozOp::I32Min;
  } else {
    return f.failf(firstArg, "%s is not a subtype of double?, float? or signed",
                   firstType.toChars());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type lhsType;
  if (!CheckExpr(f, lhs, &lhsType)) {
    return false;
  }
||||||| merged common ancestors
    if (!CheckLoopConditionOnEntry(f, cond)) {
        return false;
    }
    if (!CheckStatement(f, body)) {
        return false;
    }
    if (!f.writeContinue()) {
        return false;
    }

    if (!f.popLoop()) {
        return false;
    }
    if (labels) {
        f.removeLabels(*labels);
    }
    return true;
}
=======
  unsigned numArgs = CallArgListLength(callNode);
  ParseNode* nextArg = NextNode(firstArg);
  for (unsigned i = 1; i < numArgs; i++, nextArg = NextNode(nextArg)) {
    Type nextType;
    if (!CheckExpr(f, nextArg, &nextType)) {
      return false;
    }
    if (!(nextType <= firstType)) {
      return f.failf(nextArg, "%s is not a subtype of %s", nextType.toChars(),
                     firstType.toChars());
    }

    if (op != Op::Limit) {
      if (!f.encoder().writeOp(op)) {
        return false;
      }
    } else {
      if (!f.encoder().writeOp(mozOp)) {
        return false;
      }
    }
  }

  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type rhsType;
  if (!CheckExpr(f, rhs, &rhsType)) {
    return false;
  }

  if (!lhsType.isIntish()) {
    return f.failf(lhs, "%s is not a subtype of intish", lhsType.toChars());
  }
  if (!rhsType.isIntish()) {
    return f.failf(rhs, "%s is not a subtype of intish", rhsType.toChars());
  }

  switch (bitwise->getKind()) {
    case ParseNodeKind::BitOr:
      if (!f.encoder().writeOp(Op::I32Or)) return false;
      break;
    case ParseNodeKind::BitAnd:
      if (!f.encoder().writeOp(Op::I32And)) return false;
      break;
    case ParseNodeKind::BitXor:
      if (!f.encoder().writeOp(Op::I32Xor)) return false;
      break;
    case ParseNodeKind::Lsh:
      if (!f.encoder().writeOp(Op::I32Shl)) return false;
      break;
    case ParseNodeKind::Rsh:
      if (!f.encoder().writeOp(Op::I32ShrS)) return false;
      break;
    case ParseNodeKind::Ursh:
      if (!f.encoder().writeOp(Op::I32ShrU)) return false;
      break;
    default:
      MOZ_CRASH("not a bitwise op");
  }

  return true;
}

static bool CheckExpr(FunctionValidator& f, ParseNode* expr, Type* type) {
  if (!CheckRecursionLimitDontReport(f.cx())) {
    return f.m().failOverRecursed();
  }

  if (IsNumericLiteral(f.m(), expr)) {
    return CheckNumericLiteral(f, expr, type);
  }

  switch (expr->getKind()) {
    case ParseNodeKind::Name:
      return CheckVarRef(f, expr, type);
    case ParseNodeKind::Elem:
      return CheckLoadArray(f, expr, type);
    case ParseNodeKind::Assign:
      return CheckAssign(f, expr, type);
    case ParseNodeKind::Pos:
      return CheckPos(f, expr, type);
    case ParseNodeKind::Not:
      return CheckNot(f, expr, type);
    case ParseNodeKind::Neg:
      return CheckNeg(f, expr, type);
    case ParseNodeKind::BitNot:
      return CheckBitNot(f, expr, type);
    case ParseNodeKind::Comma:
      return CheckComma(f, expr, type);
    case ParseNodeKind::Conditional:
      return CheckConditional(f, expr, type);
    case ParseNodeKind::Star:
      return CheckMultiply(f, expr, type);
    case ParseNodeKind::Call:
      return CheckUncoercedCall(f, expr, type);

    case ParseNodeKind::Add:
    case ParseNodeKind::Sub:
      return CheckAddOrSub(f, expr, type);

    case ParseNodeKind::Div:
    case ParseNodeKind::Mod:
      return CheckDivOrMod(f, expr, type);

    case ParseNodeKind::Lt:
    case ParseNodeKind::Le:
    case ParseNodeKind::Gt:
    case ParseNodeKind::Ge:
    case ParseNodeKind::Eq:
    case ParseNodeKind::Ne:
      return CheckComparison(f, expr, type);

    case ParseNodeKind::BitOr:
    case ParseNodeKind::BitAnd:
    case ParseNodeKind::BitXor:
    case ParseNodeKind::Lsh:
    case ParseNodeKind::Rsh:
    case ParseNodeKind::Ursh:
      return CheckBitwise(f, expr, type);

    default:;
  }

  return f.fail(expr, "unsupported expression");
}

static bool CheckStatement(FunctionValidator& f, ParseNode* stmt);

static bool CheckAsExprStatement(FunctionValidator& f, ParseNode* expr) {
  if (expr->isKind(ParseNodeKind::Call)) {
    Type ignored;
    return CheckCoercedCall(f, expr, Type::Void, &ignored);
  }

  Type resultType;
  if (!CheckExpr(f, expr, &resultType)) {
    return false;
  }
||||||| merged common ancestors
static bool
CheckFor(FunctionValidator& f, ParseNode* forStmt, const LabelVector* labels = nullptr)
{
    MOZ_ASSERT(forStmt->isKind(ParseNodeKind::For));
    ParseNode* forHead = BinaryLeft(forStmt);
    ParseNode* body = BinaryRight(forStmt);

    if (!forHead->isKind(ParseNodeKind::ForHead)) {
        return f.fail(forHead, "unsupported for-loop statement");
    }

    ParseNode* maybeInit = TernaryKid1(forHead);
    ParseNode* maybeCond = TernaryKid2(forHead);
    ParseNode* maybeInc = TernaryKid3(forHead);

    // A for-loop `for (#init; #cond; #inc) #body` is equivalent to:
    // (block                                               // depth X
    //   (#init)
    //   (block $after_loop                                 // depth X+1 (block)
    //     (loop $loop_top                                  // depth X+2 (loop)
    //       (brIf $after (eq 0 #cond))
    //       (block $after_body #body)                      // depth X+3
    //       #inc
    //       (br $loop_top)
    //     )
    //   )
    // )
    // A break in the body should break out to $after_loop, i.e. depth + 1.
    // A continue in the body should break out to $after_body, i.e. depth + 3.
    if (labels && !f.addLabels(*labels, 1, 3)) {
        return false;
    }
=======
using CheckArgType = bool (*)(FunctionValidatorShared& f, ParseNode* argNode,
                              Type type);

template <CheckArgType checkArg, typename Unit>
static bool CheckCallArgs(FunctionValidator<Unit>& f, ParseNode* callNode,
                          ValTypeVector* args) {
  ParseNode* argNode = CallArgList(callNode);
  for (unsigned i = 0; i < CallArgListLength(callNode);
       i++, argNode = NextNode(argNode)) {
    Type type;
    if (!CheckExpr(f, argNode, &type)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!resultType.isVoid()) {
    if (!f.encoder().writeOp(Op::Drop)) {
      return false;
||||||| merged common ancestors
    if (!f.pushUnbreakableBlock()) {
        return false;
=======
    if (!checkArg(f, argNode, type)) {
      return false;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    if (maybeInit && !CheckAsExprStatement(f, maybeInit)) {
        return false;
    }
=======
    if (!args->append(Type::canonicalize(type).canonicalToValType())) {
      return false;
    }
  }
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckExprStatement(FunctionValidator& f, ParseNode* exprStmt) {
  MOZ_ASSERT(exprStmt->isKind(ParseNodeKind::ExpressionStatement));
  return CheckAsExprStatement(f, UnaryKid(exprStmt));
}
||||||| merged common ancestors
    {
        if (!f.pushLoop()) {
            return false;
        }
=======
static bool CheckSignatureAgainstExisting(ModuleValidatorShared& m,
                                          ParseNode* usepn, const FuncType& sig,
                                          const FuncType& existing) {
  if (sig.args().length() != existing.args().length()) {
    return m.failf(usepn,
                   "incompatible number of arguments (%zu"
                   " here vs. %zu before)",
                   sig.args().length(), existing.args().length());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckLoopConditionOnEntry(FunctionValidator& f, ParseNode* cond) {
  uint32_t maybeLit;
  if (IsLiteralInt(f.m(), cond, &maybeLit) && maybeLit) {
    return true;
  }
||||||| merged common ancestors
        if (maybeCond && !CheckLoopConditionOnEntry(f, maybeCond)) {
            return false;
        }
=======
  for (unsigned i = 0; i < sig.args().length(); i++) {
    if (sig.arg(i) != existing.arg(i)) {
      return m.failf(
          usepn, "incompatible type for argument %u: (%s here vs. %s before)",
          i, ToCString(sig.arg(i)), ToCString(existing.arg(i)));
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type condType;
  if (!CheckExpr(f, cond, &condType)) {
    return false;
  }
  if (!condType.isInt()) {
    return f.failf(cond, "%s is not a subtype of int", condType.toChars());
  }
||||||| merged common ancestors
        {
            // Continuing in the body should just break out to the increment.
            if (!f.pushContinuableBlock()) {
                return false;
            }
            if (!CheckStatement(f, body)) {
                return false;
            }
            if (!f.popContinuableBlock()) {
                return false;
            }
        }
=======
  if (sig.ret() != existing.ret()) {
    return m.failf(usepn, "%s incompatible with previous return of type %s",
                   ToCString(sig.ret()), ToCString(existing.ret()));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!f.encoder().writeOp(Op::I32Eqz)) {
    return false;
  }
||||||| merged common ancestors
        if (maybeInc && !CheckAsExprStatement(f, maybeInc)) {
            return false;
        }
=======
  MOZ_ASSERT(sig == existing);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // brIf (i32.eqz $f) $out
  if (!f.writeBreakIf()) {
    return false;
  }

  return true;
}

static bool CheckWhile(FunctionValidator& f, ParseNode* whileStmt,
                       const LabelVector* labels = nullptr) {
  MOZ_ASSERT(whileStmt->isKind(ParseNodeKind::While));
  ParseNode* cond = BinaryLeft(whileStmt);
  ParseNode* body = BinaryRight(whileStmt);

  // A while loop `while(#cond) #body` is equivalent to:
  // (block $after_loop
  //    (loop $top
  //       (brIf $after_loop (i32.eq 0 #cond))
  //       #body
  //       (br $top)
  //    )
  // )
  if (labels && !f.addLabels(*labels, 0, 1)) {
    return false;
  }
||||||| merged common ancestors
        if (!f.writeContinue()) {
            return false;
        }
        if (!f.popLoop()) {
            return false;
        }
    }
=======
template <typename Unit>
static bool CheckFunctionSignature(ModuleValidator<Unit>& m, ParseNode* usepn,
                                   FuncType&& sig, PropertyName* name,
                                   ModuleValidatorShared::Func** func) {
  if (sig.args().length() > MaxParams) {
    return m.failf(usepn, "too many parameters");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!f.pushLoop()) {
    return false;
  }
||||||| merged common ancestors
    if (!f.popUnbreakableBlock()) {
        return false;
    }
=======
  ModuleValidatorShared::Func* existing = m.lookupFuncDef(name);
  if (!existing) {
    if (!CheckModuleLevelName(m, usepn, name)) {
      return false;
    }
    return m.addFuncDef(name, usepn->pn_pos.begin, std::move(sig), func);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckLoopConditionOnEntry(f, cond)) {
    return false;
  }
  if (!CheckStatement(f, body)) {
    return false;
  }
  if (!f.writeContinue()) {
    return false;
  }
||||||| merged common ancestors
    if (labels) {
        f.removeLabels(*labels);
    }
=======
  const FuncTypeWithId& existingSig =
      m.env().types[existing->sigIndex()].funcType();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!f.popLoop()) {
    return false;
  }
  if (labels) {
    f.removeLabels(*labels);
  }
  return true;
}

static bool CheckFor(FunctionValidator& f, ParseNode* forStmt,
                     const LabelVector* labels = nullptr) {
  MOZ_ASSERT(forStmt->isKind(ParseNodeKind::For));
  ParseNode* forHead = BinaryLeft(forStmt);
  ParseNode* body = BinaryRight(forStmt);

  if (!forHead->isKind(ParseNodeKind::ForHead)) {
    return f.fail(forHead, "unsupported for-loop statement");
  }

  ParseNode* maybeInit = TernaryKid1(forHead);
  ParseNode* maybeCond = TernaryKid2(forHead);
  ParseNode* maybeInc = TernaryKid3(forHead);

  // A for-loop `for (#init; #cond; #inc) #body` is equivalent to:
  // (block                                               // depth X
  //   (#init)
  //   (block $after_loop                                 // depth X+1 (block)
  //     (loop $loop_top                                  // depth X+2 (loop)
  //       (brIf $after (eq 0 #cond))
  //       (block $after_body #body)                      // depth X+3
  //       #inc
  //       (br $loop_top)
  //     )
  //   )
  // )
  // A break in the body should break out to $after_loop, i.e. depth + 1.
  // A continue in the body should break out to $after_body, i.e. depth + 3.
  if (labels && !f.addLabels(*labels, 1, 3)) {
    return false;
  }
||||||| merged common ancestors
    return true;
}
=======
  if (!CheckSignatureAgainstExisting(m, usepn, sig, existingSig)) {
    return false;
  }

  *func = existing;
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!f.pushUnbreakableBlock()) {
    return false;
  }
||||||| merged common ancestors
static bool
CheckDoWhile(FunctionValidator& f, ParseNode* whileStmt, const LabelVector* labels = nullptr)
{
    MOZ_ASSERT(whileStmt->isKind(ParseNodeKind::DoWhile));
    ParseNode* body = BinaryLeft(whileStmt);
    ParseNode* cond = BinaryRight(whileStmt);

    // A do-while loop `do { #body } while (#cond)` is equivalent to:
    // (block $after_loop           // depth X
    //   (loop $top                 // depth X+1
    //     (block #body)            // depth X+2
    //     (brIf #cond $top)
    //   )
    // )
    // A break should break out of the entire loop, i.e. at depth 0.
    // A continue should break out to the condition, i.e. at depth 2.
    if (labels && !f.addLabels(*labels, 0, 2)) {
        return false;
    }
=======
static bool CheckIsArgType(FunctionValidatorShared& f, ParseNode* argNode,
                           Type type) {
  if (!type.isArgType()) {
    return f.failf(argNode, "%s is not a subtype of int, float, or double",
                   type.toChars());
  }
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (maybeInit && !CheckAsExprStatement(f, maybeInit)) {
    return false;
  }

  {
    if (!f.pushLoop()) {
      return false;
    }
||||||| merged common ancestors
    if (!f.pushLoop()) {
        return false;
    }
=======
template <typename Unit>
static bool CheckInternalCall(FunctionValidator<Unit>& f, ParseNode* callNode,
                              PropertyName* calleeName, Type ret, Type* type) {
  MOZ_ASSERT(ret.isCanonical());
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (maybeCond && !CheckLoopConditionOnEntry(f, maybeCond)) {
      return false;
    }
||||||| merged common ancestors
    {
        // An unlabeled continue in the body should break out to the condition.
        if (!f.pushContinuableBlock()) {
            return false;
        }
        if (!CheckStatement(f, body)) {
            return false;
        }
        if (!f.popContinuableBlock()) {
            return false;
        }
    }
=======
  ValTypeVector args;
  if (!CheckCallArgs<CheckIsArgType>(f, callNode, &args)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    {
      // Continuing in the body should just break out to the increment.
      if (!f.pushContinuableBlock()) {
        return false;
      }
      if (!CheckStatement(f, body)) {
        return false;
      }
      if (!f.popContinuableBlock()) {
        return false;
      }
    }
||||||| merged common ancestors
    Type condType;
    if (!CheckExpr(f, cond, &condType)) {
        return false;
    }
    if (!condType.isInt()) {
        return f.failf(cond, "%s is not a subtype of int", condType.toChars());
    }
=======
  FuncType sig(std::move(args), ret.canonicalToExprType());
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (maybeInc && !CheckAsExprStatement(f, maybeInc)) {
      return false;
    }
||||||| merged common ancestors
    if (!f.writeContinueIf()) {
        return false;
    }
=======
  ModuleValidatorShared::Func* callee;
  if (!CheckFunctionSignature(f.m(), callNode, std::move(sig), calleeName,
                              &callee)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!f.writeContinue()) {
      return false;
    }
    if (!f.popLoop()) {
      return false;
    }
  }
||||||| merged common ancestors
    if (!f.popLoop()) {
        return false;
    }
    if (labels) {
        f.removeLabels(*labels);
    }
    return true;
}
=======
  if (!f.writeCall(callNode, MozOp::OldCallDirect)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!f.popUnbreakableBlock()) {
    return false;
  }

  if (labels) {
    f.removeLabels(*labels);
  }

  return true;
}

static bool CheckDoWhile(FunctionValidator& f, ParseNode* whileStmt,
                         const LabelVector* labels = nullptr) {
  MOZ_ASSERT(whileStmt->isKind(ParseNodeKind::DoWhile));
  ParseNode* body = BinaryLeft(whileStmt);
  ParseNode* cond = BinaryRight(whileStmt);

  // A do-while loop `do { #body } while (#cond)` is equivalent to:
  // (block $after_loop           // depth X
  //   (loop $top                 // depth X+1
  //     (block #body)            // depth X+2
  //     (brIf #cond $top)
  //   )
  // )
  // A break should break out of the entire loop, i.e. at depth 0.
  // A continue should break out to the condition, i.e. at depth 2.
  if (labels && !f.addLabels(*labels, 0, 2)) {
    return false;
  }
||||||| merged common ancestors
static bool CheckStatementList(FunctionValidator& f, ParseNode*, const LabelVector* = nullptr);
=======
  if (!f.encoder().writeVarU32(callee->funcDefIndex())) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!f.pushLoop()) {
    return false;
  }
||||||| merged common ancestors
static bool
CheckLabel(FunctionValidator& f, ParseNode* labeledStmt)
{
    MOZ_ASSERT(labeledStmt->isKind(ParseNodeKind::Label));
=======
  *type = Type::ret(ret);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  {
    // An unlabeled continue in the body should break out to the condition.
    if (!f.pushContinuableBlock()) {
      return false;
||||||| merged common ancestors
    LabelVector labels;
    ParseNode* innermost = labeledStmt;
    do {
        if (!labels.append(LabeledStatementLabel(innermost))) {
            return false;
        }
        innermost = LabeledStatementStatement(innermost);
    } while (innermost->getKind() == ParseNodeKind::Label);

    switch (innermost->getKind()) {
      case ParseNodeKind::For:
        return CheckFor(f, innermost, &labels);
      case ParseNodeKind::DoWhile:
        return CheckDoWhile(f, innermost, &labels);
      case ParseNodeKind::While:
        return CheckWhile(f, innermost, &labels);
      case ParseNodeKind::StatementList:
        return CheckStatementList(f, innermost, &labels);
      default:
        break;
=======
template <typename Unit>
static bool CheckFuncPtrTableAgainstExisting(ModuleValidator<Unit>& m,
                                             ParseNode* usepn,
                                             PropertyName* name, FuncType&& sig,
                                             unsigned mask,
                                             uint32_t* tableIndex) {
  if (const ModuleValidatorShared::Global* existing = m.lookupGlobal(name)) {
    if (existing->which() != ModuleValidatorShared::Global::Table) {
      return m.failName(usepn, "'%s' is not a function-pointer table", name);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (!CheckStatement(f, body)) {
      return false;
||||||| merged common ancestors

    if (!f.pushUnbreakableBlock(&labels)) {
        return false;
=======

    ModuleValidatorShared::Table& table = m.table(existing->tableIndex());
    if (mask != table.mask()) {
      return m.failf(usepn, "mask does not match previous value (%u)",
                     table.mask());
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (!f.popContinuableBlock()) {
      return false;
||||||| merged common ancestors

    if (!CheckStatement(f, innermost)) {
        return false;
=======

    if (!CheckSignatureAgainstExisting(
            m, usepn, sig, m.env().types[table.sigIndex()].funcType())) {
      return false;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  Type condType;
  if (!CheckExpr(f, cond, &condType)) {
    return false;
  }
  if (!condType.isInt()) {
    return f.failf(cond, "%s is not a subtype of int", condType.toChars());
  }

  if (!f.writeContinueIf()) {
    return false;
  }

  if (!f.popLoop()) {
    return false;
  }
  if (labels) {
    f.removeLabels(*labels);
  }
  return true;
}

static bool CheckStatementList(FunctionValidator& f, ParseNode*,
                               const LabelVector* = nullptr);

static bool CheckLabel(FunctionValidator& f, ParseNode* labeledStmt) {
  MOZ_ASSERT(labeledStmt->isKind(ParseNodeKind::Label));

  LabelVector labels;
  ParseNode* innermost = labeledStmt;
  do {
    if (!labels.append(LabeledStatementLabel(innermost))) {
      return false;
    }
    innermost = LabeledStatementStatement(innermost);
  } while (innermost->getKind() == ParseNodeKind::Label);

  switch (innermost->getKind()) {
    case ParseNodeKind::For:
      return CheckFor(f, innermost, &labels);
    case ParseNodeKind::DoWhile:
      return CheckDoWhile(f, innermost, &labels);
    case ParseNodeKind::While:
      return CheckWhile(f, innermost, &labels);
    case ParseNodeKind::StatementList:
      return CheckStatementList(f, innermost, &labels);
    default:
      break;
  }

  if (!f.pushUnbreakableBlock(&labels)) {
    return false;
  }

  if (!CheckStatement(f, innermost)) {
    return false;
  }

  if (!f.popUnbreakableBlock(&labels)) {
    return false;
  }
  return true;
||||||| merged common ancestors
    if (!f.popUnbreakableBlock(&labels)) {
        return false;
    }
    return true;
=======
    *tableIndex = existing->tableIndex();
    return true;
  }

  if (!CheckModuleLevelName(m, usepn, name)) {
    return false;
  }

  if (!m.declareFuncPtrTable(std::move(sig), name, usepn->pn_pos.begin, mask,
                             tableIndex)) {
    return false;
  }

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CheckIf(FunctionValidator& f, ParseNode* ifStmt) {
  uint32_t numIfEnd = 1;
||||||| merged common ancestors
static bool
CheckIf(FunctionValidator& f, ParseNode* ifStmt)
{
    uint32_t numIfEnd = 1;
=======
template <typename Unit>
static bool CheckFuncPtrCall(FunctionValidator<Unit>& f, ParseNode* callNode,
                             Type ret, Type* type) {
  MOZ_ASSERT(ret.isCanonical());
>>>>>>> upstream-releases

<<<<<<< HEAD
recurse:
  MOZ_ASSERT(ifStmt->isKind(ParseNodeKind::If));
  ParseNode* cond = TernaryKid1(ifStmt);
  ParseNode* thenStmt = TernaryKid2(ifStmt);
  ParseNode* elseStmt = TernaryKid3(ifStmt);
||||||| merged common ancestors
  recurse:
    MOZ_ASSERT(ifStmt->isKind(ParseNodeKind::If));
    ParseNode* cond = TernaryKid1(ifStmt);
    ParseNode* thenStmt = TernaryKid2(ifStmt);
    ParseNode* elseStmt = TernaryKid3(ifStmt);
=======
  ParseNode* callee = CallCallee(callNode);
  ParseNode* tableNode = ElemBase(callee);
  ParseNode* indexExpr = ElemIndex(callee);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Type condType;
  if (!CheckExpr(f, cond, &condType)) {
    return false;
  }
  if (!condType.isInt()) {
    return f.failf(cond, "%s is not a subtype of int", condType.toChars());
  }
||||||| merged common ancestors
    Type condType;
    if (!CheckExpr(f, cond, &condType)) {
        return false;
    }
    if (!condType.isInt()) {
        return f.failf(cond, "%s is not a subtype of int", condType.toChars());
    }
=======
  if (!tableNode->isKind(ParseNodeKind::Name)) {
    return f.fail(tableNode, "expecting name of function-pointer array");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t typeAt;
  if (!f.pushIf(&typeAt)) {
    return false;
  }
||||||| merged common ancestors
    size_t typeAt;
    if (!f.pushIf(&typeAt)) {
        return false;
    }
=======
  PropertyName* name = tableNode->as<NameNode>().name();
  if (const ModuleValidatorShared::Global* existing = f.lookupGlobal(name)) {
    if (existing->which() != ModuleValidatorShared::Global::Table) {
      return f.failName(
          tableNode, "'%s' is not the name of a function-pointer array", name);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  f.setIfType(typeAt, ExprType::Void);
||||||| merged common ancestors
    f.setIfType(typeAt, ExprType::Void);
=======
  if (!indexExpr->isKind(ParseNodeKind::BitAndExpr)) {
    return f.fail(indexExpr,
                  "function-pointer table index expression needs & mask");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckStatement(f, thenStmt)) {
    return false;
  }
||||||| merged common ancestors
    if (!CheckStatement(f, thenStmt)) {
        return false;
    }
=======
  ParseNode* indexNode = BitwiseLeft(indexExpr);
  ParseNode* maskNode = BitwiseRight(indexExpr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (elseStmt) {
    if (!f.switchToElse()) {
      return false;
    }
||||||| merged common ancestors
    if (elseStmt) {
        if (!f.switchToElse()) {
            return false;
        }
=======
  uint32_t mask;
  if (!IsLiteralInt(f.m(), maskNode, &mask) || mask == UINT32_MAX ||
      !IsPowerOfTwo(mask + 1)) {
    return f.fail(maskNode,
                  "function-pointer table index mask value must be a power of "
                  "two minus 1");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (elseStmt->isKind(ParseNodeKind::If)) {
      ifStmt = elseStmt;
      if (numIfEnd++ == UINT32_MAX) {
        return false;
      }
      goto recurse;
    }
||||||| merged common ancestors
        if (elseStmt->isKind(ParseNodeKind::If)) {
            ifStmt = elseStmt;
            if (numIfEnd++ == UINT32_MAX) {
                return false;
            }
            goto recurse;
        }
=======
  Type indexType;
  if (!CheckExpr(f, indexNode, &indexType)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!CheckStatement(f, elseStmt)) {
      return false;
    }
  }
||||||| merged common ancestors
        if (!CheckStatement(f, elseStmt)) {
            return false;
        }
    }
=======
  if (!indexType.isIntish()) {
    return f.failf(indexNode, "%s is not a subtype of intish",
                   indexType.toChars());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (uint32_t i = 0; i != numIfEnd; ++i) {
    if (!f.popIf()) {
      return false;
    }
  }
||||||| merged common ancestors
    for (uint32_t i = 0; i != numIfEnd; ++i) {
        if (!f.popIf()) {
            return false;
        }
    }
=======
  ValTypeVector args;
  if (!CheckCallArgs<CheckIsArgType>(f, callNode, &args)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  FuncType sig(std::move(args), ret.canonicalToExprType());

  uint32_t tableIndex;
  if (!CheckFuncPtrTableAgainstExisting(f.m(), tableNode, name, std::move(sig),
                                        mask, &tableIndex)) {
    return false;
  }

  if (!f.writeCall(callNode, MozOp::OldCallIndirect)) {
    return false;
  }

  // Call signature
  if (!f.encoder().writeVarU32(f.m().table(tableIndex).sigIndex())) {
    return false;
  }

  *type = Type::ret(ret);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CheckCaseExpr(FunctionValidator& f, ParseNode* caseExpr,
                          int32_t* value) {
  if (!IsNumericLiteral(f.m(), caseExpr)) {
    return f.fail(caseExpr,
                  "switch case expression must be an integer literal");
  }
||||||| merged common ancestors
static bool
CheckCaseExpr(FunctionValidator& f, ParseNode* caseExpr, int32_t* value)
{
    if (!IsNumericLiteral(f.m(), caseExpr)) {
        return f.fail(caseExpr, "switch case expression must be an integer literal");
    }
=======
static bool CheckIsExternType(FunctionValidatorShared& f, ParseNode* argNode,
                              Type type) {
  if (!type.isExtern()) {
    return f.failf(argNode, "%s is not a subtype of extern", type.toChars());
  }
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  NumLit lit = ExtractNumericLiteral(f.m(), caseExpr);
  switch (lit.which()) {
    case NumLit::Fixnum:
    case NumLit::NegativeInt:
      *value = lit.toInt32();
      break;
    case NumLit::OutOfRangeInt:
    case NumLit::BigUnsigned:
      return f.fail(caseExpr, "switch case expression out of integer range");
    case NumLit::Double:
    case NumLit::Float:
      return f.fail(caseExpr,
                    "switch case expression must be an integer literal");
  }
||||||| merged common ancestors
    NumLit lit = ExtractNumericLiteral(f.m(), caseExpr);
    switch (lit.which()) {
      case NumLit::Fixnum:
      case NumLit::NegativeInt:
        *value = lit.toInt32();
        break;
      case NumLit::OutOfRangeInt:
      case NumLit::BigUnsigned:
        return f.fail(caseExpr, "switch case expression out of integer range");
      case NumLit::Double:
      case NumLit::Float:
        return f.fail(caseExpr, "switch case expression must be an integer literal");
    }
=======
template <typename Unit>
static bool CheckFFICall(FunctionValidator<Unit>& f, ParseNode* callNode,
                         unsigned ffiIndex, Type ret, Type* type) {
  MOZ_ASSERT(ret.isCanonical());
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  PropertyName* calleeName = CallCallee(callNode)->as<NameNode>().name();

  if (ret.isFloat()) {
    return f.fail(callNode, "FFI calls can't return float");
  }

  ValTypeVector args;
  if (!CheckCallArgs<CheckIsExternType>(f, callNode, &args)) {
    return false;
  }

  FuncType sig(std::move(args), ret.canonicalToExprType());

  uint32_t importIndex;
  if (!f.m().declareImport(calleeName, std::move(sig), ffiIndex,
                           &importIndex)) {
    return false;
  }

  if (!f.writeCall(callNode, Op::Call)) {
    return false;
  }

  if (!f.encoder().writeVarU32(importIndex)) {
    return false;
  }

  *type = Type::ret(ret);
  return true;
}

static bool CheckFloatCoercionArg(FunctionValidatorShared& f,
                                  ParseNode* inputNode, Type inputType) {
  if (inputType.isMaybeDouble()) {
    return f.encoder().writeOp(Op::F32DemoteF64);
  }
  if (inputType.isSigned()) {
    return f.encoder().writeOp(Op::F32ConvertSI32);
  }
  if (inputType.isUnsigned()) {
    return f.encoder().writeOp(Op::F32ConvertUI32);
  }
  if (inputType.isFloatish()) {
    return true;
  }

  return f.failf(inputNode,
                 "%s is not a subtype of signed, unsigned, double? or floatish",
                 inputType.toChars());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CheckDefaultAtEnd(FunctionValidator& f, ParseNode* stmt) {
  for (; stmt; stmt = NextNode(stmt)) {
    if (IsDefaultCase(stmt) && NextNode(stmt) != nullptr) {
      return f.fail(stmt, "default label must be at the end");
||||||| merged common ancestors
static bool
CheckDefaultAtEnd(FunctionValidator& f, ParseNode* stmt)
{
    for (; stmt; stmt = NextNode(stmt)) {
        if (IsDefaultCase(stmt) && NextNode(stmt) != nullptr) {
            return f.fail(stmt, "default label must be at the end");
        }
=======
template <typename Unit>
static bool CheckCoercedCall(FunctionValidator<Unit>& f, ParseNode* call,
                             Type ret, Type* type);

template <typename Unit>
static bool CheckCoercionArg(FunctionValidator<Unit>& f, ParseNode* arg,
                             Type expected, Type* type) {
  MOZ_ASSERT(expected.isCanonicalValType());

  if (arg->isKind(ParseNodeKind::CallExpr)) {
    return CheckCoercedCall(f, arg, expected, type);
  }

  Type argType;
  if (!CheckExpr(f, arg, &argType)) {
    return false;
  }

  if (expected.isFloat()) {
    if (!CheckFloatCoercionArg(f, arg, argType)) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
  } else {
    MOZ_CRASH("not call coercions");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  *type = Type::ret(expected);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CheckSwitchRange(FunctionValidator& f, ParseNode* stmt,
                             int32_t* low, int32_t* high,
                             uint32_t* tableLength) {
  if (IsDefaultCase(stmt)) {
    *low = 0;
    *high = -1;
    *tableLength = 0;
    return true;
  }

  int32_t i = 0;
  if (!CheckCaseExpr(f, CaseExpr(stmt), &i)) {
    return false;
  }

  *low = *high = i;
||||||| merged common ancestors
static bool
CheckSwitchRange(FunctionValidator& f, ParseNode* stmt, int32_t* low, int32_t* high,
                 uint32_t* tableLength)
{
    if (IsDefaultCase(stmt)) {
        *low = 0;
        *high = -1;
        *tableLength = 0;
        return true;
    }
=======
template <typename Unit>
static bool CheckMathFRound(FunctionValidator<Unit>& f, ParseNode* callNode,
                            Type* type) {
  if (CallArgListLength(callNode) != 1) {
    return f.fail(callNode, "Math.fround must be passed 1 argument");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* initialStmt = stmt;
  for (stmt = NextNode(stmt); stmt && !IsDefaultCase(stmt);
       stmt = NextNode(stmt)) {
    int32_t i = 0;
    if (!CheckCaseExpr(f, CaseExpr(stmt), &i)) {
      return false;
    }
||||||| merged common ancestors
    int32_t i = 0;
    if (!CheckCaseExpr(f, CaseExpr(stmt), &i)) {
        return false;
    }
=======
  ParseNode* argNode = CallArgList(callNode);
  Type argType;
  if (!CheckCoercionArg(f, argNode, Type::Float, &argType)) {
    return false;
  }

  MOZ_ASSERT(argType == Type::Float);
  *type = Type::Float;
  return true;
}

template <typename Unit>
static bool CheckMathBuiltinCall(FunctionValidator<Unit>& f,
                                 ParseNode* callNode,
                                 AsmJSMathBuiltinFunction func, Type* type) {
  unsigned arity = 0;
  Op f32 = Op::Limit;
  Op f64 = Op::Limit;
  MozOp mozf64 = MozOp::Limit;
  switch (func) {
    case AsmJSMathBuiltin_imul:
      return CheckMathIMul(f, callNode, type);
    case AsmJSMathBuiltin_clz32:
      return CheckMathClz32(f, callNode, type);
    case AsmJSMathBuiltin_abs:
      return CheckMathAbs(f, callNode, type);
    case AsmJSMathBuiltin_sqrt:
      return CheckMathSqrt(f, callNode, type);
    case AsmJSMathBuiltin_fround:
      return CheckMathFRound(f, callNode, type);
    case AsmJSMathBuiltin_min:
      return CheckMathMinMax(f, callNode, /* isMax = */ false, type);
    case AsmJSMathBuiltin_max:
      return CheckMathMinMax(f, callNode, /* isMax = */ true, type);
    case AsmJSMathBuiltin_ceil:
      arity = 1;
      f64 = Op::F64Ceil;
      f32 = Op::F32Ceil;
      break;
    case AsmJSMathBuiltin_floor:
      arity = 1;
      f64 = Op::F64Floor;
      f32 = Op::F32Floor;
      break;
    case AsmJSMathBuiltin_sin:
      arity = 1;
      mozf64 = MozOp::F64Sin;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_cos:
      arity = 1;
      mozf64 = MozOp::F64Cos;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_tan:
      arity = 1;
      mozf64 = MozOp::F64Tan;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_asin:
      arity = 1;
      mozf64 = MozOp::F64Asin;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_acos:
      arity = 1;
      mozf64 = MozOp::F64Acos;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_atan:
      arity = 1;
      mozf64 = MozOp::F64Atan;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_exp:
      arity = 1;
      mozf64 = MozOp::F64Exp;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_log:
      arity = 1;
      mozf64 = MozOp::F64Log;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_pow:
      arity = 2;
      mozf64 = MozOp::F64Pow;
      f32 = Op::Unreachable;
      break;
    case AsmJSMathBuiltin_atan2:
      arity = 2;
      mozf64 = MozOp::F64Atan2;
      f32 = Op::Unreachable;
      break;
    default:
      MOZ_CRASH("unexpected mathBuiltin function");
  }

  unsigned actualArity = CallArgListLength(callNode);
  if (actualArity != arity) {
    return f.failf(callNode, "call passed %u arguments, expected %u",
                   actualArity, arity);
  }

  if (!f.prepareCall(callNode)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    *low = Min(*low, i);
    *high = Max(*high, i);
  }
||||||| merged common ancestors
    *low = *high = i;

    ParseNode* initialStmt = stmt;
    for (stmt = NextNode(stmt); stmt && !IsDefaultCase(stmt); stmt = NextNode(stmt)) {
        int32_t i = 0;
        if (!CheckCaseExpr(f, CaseExpr(stmt), &i)) {
            return false;
        }

        *low = Min(*low, i);
        *high = Max(*high, i);
    }
=======
  Type firstType;
  ParseNode* argNode = CallArgList(callNode);
  if (!CheckExpr(f, argNode, &firstType)) {
    return false;
  }

  if (!firstType.isMaybeFloat() && !firstType.isMaybeDouble()) {
    return f.fail(
        argNode,
        "arguments to math call should be a subtype of double? or float?");
  }

  bool opIsDouble = firstType.isMaybeDouble();
  if (!opIsDouble && f32 == Op::Unreachable) {
    return f.fail(callNode, "math builtin cannot be used as float");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  int64_t i64 = (int64_t(*high) - int64_t(*low)) + 1;
  if (i64 > MaxBrTableElems) {
    return f.fail(
        initialStmt,
        "all switch statements generate tables; this table would be too big");
  }
||||||| merged common ancestors
    int64_t i64 = (int64_t(*high) - int64_t(*low)) + 1;
    if (i64 > MaxBrTableElems) {
        return f.fail(initialStmt, "all switch statements generate tables; this table would be too big");
    }
=======
  if (arity == 2) {
    Type secondType;
    argNode = NextNode(argNode);
    if (!CheckExpr(f, argNode, &secondType)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *tableLength = uint32_t(i64);
  return true;
}
||||||| merged common ancestors
    *tableLength = uint32_t(i64);
    return true;
}
=======
    if (firstType.isMaybeDouble() && !secondType.isMaybeDouble()) {
      return f.fail(
          argNode,
          "both arguments to math builtin call should be the same type");
    }
    if (firstType.isMaybeFloat() && !secondType.isMaybeFloat()) {
      return f.fail(
          argNode,
          "both arguments to math builtin call should be the same type");
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckSwitchExpr(FunctionValidator& f, ParseNode* switchExpr) {
  Type exprType;
  if (!CheckExpr(f, switchExpr, &exprType)) {
    return false;
  }
  if (!exprType.isSigned()) {
    return f.failf(switchExpr, "%s is not a subtype of signed",
                   exprType.toChars());
  }
  return true;
||||||| merged common ancestors
static bool
CheckSwitchExpr(FunctionValidator& f, ParseNode* switchExpr)
{
    Type exprType;
    if (!CheckExpr(f, switchExpr, &exprType)) {
        return false;
    }
    if (!exprType.isSigned()) {
        return f.failf(switchExpr, "%s is not a subtype of signed", exprType.toChars());
    }
    return true;
=======
  if (opIsDouble) {
    if (f64 != Op::Limit) {
      if (!f.encoder().writeOp(f64)) {
        return false;
      }
    } else {
      if (!f.encoder().writeOp(mozf64)) {
        return false;
      }
    }
  } else {
    if (!f.encoder().writeOp(f32)) {
      return false;
    }
  }

  *type = opIsDouble ? Type::Double : Type::Floatish;
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// A switch will be constructed as:
// - the default block wrapping all the other blocks, to be able to break
// out of the switch with an unlabeled break statement. It has two statements
// (an inner block and the default expr). asm.js rules require default to be at
// the end, so the default block always encloses all the cases blocks.
// - one block per case between low and high; undefined cases just jump to the
// default case. Each of these blocks contain two statements: the next case's
// block and the possibly empty statement list comprising the case body. The
// last block pushed is the first case so the (relative) branch target therefore
// matches the sequential order of cases.
// - one block for the br_table, so that the first break goes to the first
// case's block.
static bool CheckSwitch(FunctionValidator& f, ParseNode* switchStmt) {
  MOZ_ASSERT(switchStmt->isKind(ParseNodeKind::Switch));
||||||| merged common ancestors
// A switch will be constructed as:
// - the default block wrapping all the other blocks, to be able to break
// out of the switch with an unlabeled break statement. It has two statements
// (an inner block and the default expr). asm.js rules require default to be at
// the end, so the default block always encloses all the cases blocks.
// - one block per case between low and high; undefined cases just jump to the
// default case. Each of these blocks contain two statements: the next case's
// block and the possibly empty statement list comprising the case body. The
// last block pushed is the first case so the (relative) branch target therefore
// matches the sequential order of cases.
// - one block for the br_table, so that the first break goes to the first
// case's block.
static bool
CheckSwitch(FunctionValidator& f, ParseNode* switchStmt)
{
    MOZ_ASSERT(switchStmt->isKind(ParseNodeKind::Switch));

    ParseNode* switchExpr = BinaryLeft(switchStmt);
    ParseNode* switchBody = BinaryRight(switchStmt);

    if (switchBody->is<LexicalScopeNode>()) {
        LexicalScopeNode* scope = &switchBody->as<LexicalScopeNode>();
        if (!scope->isEmptyScope()) {
            return f.fail(scope, "switch body may not contain lexical declarations");
        }
        switchBody = scope->scopeBody();
    }

    ParseNode* stmt = ListHead(switchBody);
    if (!stmt) {
        if (!CheckSwitchExpr(f, switchExpr)) {
            return false;
        }
        if (!f.encoder().writeOp(Op::Drop)) {
            return false;
        }
        return true;
    }

    if (!CheckDefaultAtEnd(f, stmt)) {
        return false;
    }

    int32_t low = 0, high = 0;
    uint32_t tableLength = 0;
    if (!CheckSwitchRange(f, stmt, &low, &high, &tableLength)) {
        return false;
    }

    static const uint32_t CASE_NOT_DEFINED = UINT32_MAX;

    Uint32Vector caseDepths;
    if (!caseDepths.appendN(CASE_NOT_DEFINED, tableLength)) {
        return false;
    }

    uint32_t numCases = 0;
    for (ParseNode* s = stmt; s && !IsDefaultCase(s); s = NextNode(s)) {
        int32_t caseValue = ExtractNumericLiteral(f.m(), CaseExpr(s)).toInt32();
=======
template <typename Unit>
static bool CheckUncoercedCall(FunctionValidator<Unit>& f, ParseNode* expr,
                               Type* type) {
  MOZ_ASSERT(expr->isKind(ParseNodeKind::CallExpr));

  const ModuleValidatorShared::Global* global;
  if (IsCallToGlobal(f.m(), expr, &global) && global->isMathFunction()) {
    return CheckMathBuiltinCall(f, expr, global->mathBuiltinFunction(), type);
  }

  return f.fail(
      expr,
      "all function calls must be calls to standard lib math functions,"
      " ignored (via f(); or comma-expression), coerced to signed (via f()|0),"
      " coerced to float (via fround(f())), or coerced to double (via +f())");
}

static bool CoerceResult(FunctionValidatorShared& f, ParseNode* expr,
                         Type expected, Type actual, Type* type) {
  MOZ_ASSERT(expected.isCanonical());

  // At this point, the bytecode resembles this:
  //      | the thing we wanted to coerce | current position |>
  switch (expected.which()) {
    case Type::Void:
      if (!actual.isVoid()) {
        if (!f.encoder().writeOp(Op::Drop)) {
          return false;
        }
      }
      break;
    case Type::Int:
      if (!actual.isIntish()) {
        return f.failf(expr, "%s is not a subtype of intish", actual.toChars());
      }
      break;
    case Type::Float:
      if (!CheckFloatCoercionArg(f, expr, actual)) {
        return false;
      }
      break;
    case Type::Double:
      if (actual.isMaybeDouble()) {
        // No conversion necessary.
      } else if (actual.isMaybeFloat()) {
        if (!f.encoder().writeOp(Op::F64PromoteF32)) {
          return false;
        }
      } else if (actual.isSigned()) {
        if (!f.encoder().writeOp(Op::F64ConvertSI32)) {
          return false;
        }
      } else if (actual.isUnsigned()) {
        if (!f.encoder().writeOp(Op::F64ConvertUI32)) {
          return false;
        }
      } else {
        return f.failf(
            expr, "%s is not a subtype of double?, float?, signed or unsigned",
            actual.toChars());
      }
      break;
    default:
      MOZ_CRASH("unexpected uncoerced result type");
  }

  *type = Type::ret(expected);
  return true;
}

template <typename Unit>
static bool CheckCoercedMathBuiltinCall(FunctionValidator<Unit>& f,
                                        ParseNode* callNode,
                                        AsmJSMathBuiltinFunction func, Type ret,
                                        Type* type) {
  Type actual;
  if (!CheckMathBuiltinCall(f, callNode, func, &actual)) {
    return false;
  }
  return CoerceResult(f, callNode, ret, actual, type);
}

template <typename Unit>
static bool CheckCoercedCall(FunctionValidator<Unit>& f, ParseNode* call,
                             Type ret, Type* type) {
  MOZ_ASSERT(ret.isCanonical());

  if (!CheckRecursionLimitDontReport(f.cx())) {
    return f.m().failOverRecursed();
  }

  if (IsNumericLiteral(f.m(), call)) {
    NumLit lit = ExtractNumericLiteral(f.m(), call);
    if (!f.writeConstExpr(lit)) {
      return false;
    }
    return CoerceResult(f, call, ret, Type::lit(lit), type);
  }

  ParseNode* callee = CallCallee(call);

  if (callee->isKind(ParseNodeKind::ElemExpr)) {
    return CheckFuncPtrCall(f, call, ret, type);
  }

  if (!callee->isKind(ParseNodeKind::Name)) {
    return f.fail(callee, "unexpected callee expression type");
  }

  PropertyName* calleeName = callee->as<NameNode>().name();

  if (const ModuleValidatorShared::Global* global =
          f.lookupGlobal(calleeName)) {
    switch (global->which()) {
      case ModuleValidatorShared::Global::FFI:
        return CheckFFICall(f, call, global->ffiIndex(), ret, type);
      case ModuleValidatorShared::Global::MathBuiltinFunction:
        return CheckCoercedMathBuiltinCall(
            f, call, global->mathBuiltinFunction(), ret, type);
      case ModuleValidatorShared::Global::ConstantLiteral:
      case ModuleValidatorShared::Global::ConstantImport:
      case ModuleValidatorShared::Global::Variable:
      case ModuleValidatorShared::Global::Table:
      case ModuleValidatorShared::Global::ArrayView:
      case ModuleValidatorShared::Global::ArrayViewCtor:
        return f.failName(callee, "'%s' is not callable function", calleeName);
      case ModuleValidatorShared::Global::Function:
        break;
    }
  }

  return CheckInternalCall(f, call, calleeName, ret, type);
}

template <typename Unit>
static bool CheckPos(FunctionValidator<Unit>& f, ParseNode* pos, Type* type) {
  MOZ_ASSERT(pos->isKind(ParseNodeKind::PosExpr));
  ParseNode* operand = UnaryKid(pos);

  if (operand->isKind(ParseNodeKind::CallExpr)) {
    return CheckCoercedCall(f, operand, Type::Double, type);
  }

  Type actual;
  if (!CheckExpr(f, operand, &actual)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* switchExpr = BinaryLeft(switchStmt);
  ParseNode* switchBody = BinaryRight(switchStmt);
||||||| merged common ancestors
        MOZ_ASSERT(caseValue >= low);
        unsigned i = caseValue - low;
        if (caseDepths[i] != CASE_NOT_DEFINED) {
            return f.fail(s, "no duplicate case labels");
        }
=======
  return CoerceResult(f, operand, Type::Double, actual, type);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (switchBody->is<LexicalScopeNode>()) {
    LexicalScopeNode* scope = &switchBody->as<LexicalScopeNode>();
    if (!scope->isEmptyScope()) {
      return f.fail(scope, "switch body may not contain lexical declarations");
    }
    switchBody = scope->scopeBody();
  }
||||||| merged common ancestors
        MOZ_ASSERT(numCases != CASE_NOT_DEFINED);
        caseDepths[i] = numCases++;
    }
=======
template <typename Unit>
static bool CheckNot(FunctionValidator<Unit>& f, ParseNode* expr, Type* type) {
  MOZ_ASSERT(expr->isKind(ParseNodeKind::NotExpr));
  ParseNode* operand = UnaryKid(expr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* stmt = ListHead(switchBody);
  if (!stmt) {
    if (!CheckSwitchExpr(f, switchExpr)) {
      return false;
    }
    if (!f.encoder().writeOp(Op::Drop)) {
      return false;
    }
    return true;
  }
||||||| merged common ancestors
    // Open the wrapping breakable default block.
    if (!f.pushBreakableBlock()) {
        return false;
    }

    // Open all the case blocks.
    for (uint32_t i = 0; i < numCases; i++) {
        if (!f.pushUnbreakableBlock()) {
            return false;
        }
    }
=======
  Type operandType;
  if (!CheckExpr(f, operand, &operandType)) {
    return false;
  }

  if (!operandType.isInt()) {
    return f.failf(operand, "%s is not a subtype of int",
                   operandType.toChars());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckDefaultAtEnd(f, stmt)) {
    return false;
  }
||||||| merged common ancestors
    // Open the br_table block.
    if (!f.pushUnbreakableBlock()) {
        return false;
    }
=======
  *type = Type::Int;
  return f.encoder().writeOp(Op::I32Eqz);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  int32_t low = 0, high = 0;
  uint32_t tableLength = 0;
  if (!CheckSwitchRange(f, stmt, &low, &high, &tableLength)) {
    return false;
  }
||||||| merged common ancestors
    // The default block is the last one.
    uint32_t defaultDepth = numCases;
=======
template <typename Unit>
static bool CheckNeg(FunctionValidator<Unit>& f, ParseNode* expr, Type* type) {
  MOZ_ASSERT(expr->isKind(ParseNodeKind::NegExpr));
  ParseNode* operand = UnaryKid(expr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static const uint32_t CASE_NOT_DEFINED = UINT32_MAX;
||||||| merged common ancestors
    // Subtract lowest case value, so that all the cases start from 0.
    if (low) {
        if (!CheckSwitchExpr(f, switchExpr)) {
            return false;
        }
        if (!f.writeInt32Lit(low)) {
            return false;
        }
        if (!f.encoder().writeOp(Op::I32Sub)) {
            return false;
        }
    } else {
        if (!CheckSwitchExpr(f, switchExpr)) {
            return false;
        }
    }
=======
  Type operandType;
  if (!CheckExpr(f, operand, &operandType)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Uint32Vector caseDepths;
  if (!caseDepths.appendN(CASE_NOT_DEFINED, tableLength)) {
    return false;
  }
||||||| merged common ancestors
    // Start the br_table block.
    if (!f.encoder().writeOp(Op::BrTable)) {
        return false;
    }
=======
  if (operandType.isInt()) {
    *type = Type::Intish;
    return f.encoder().writeOp(MozOp::I32Neg);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t numCases = 0;
  for (ParseNode* s = stmt; s && !IsDefaultCase(s); s = NextNode(s)) {
    int32_t caseValue = ExtractNumericLiteral(f.m(), CaseExpr(s)).toInt32();
||||||| merged common ancestors
    // Write the number of cases (tableLength - 1 + 1 (default)).
    // Write the number of cases (tableLength - 1 + 1 (default)).
    if (!f.encoder().writeVarU32(tableLength)) {
        return false;
    }
=======
  if (operandType.isMaybeDouble()) {
    *type = Type::Double;
    return f.encoder().writeOp(Op::F64Neg);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    MOZ_ASSERT(caseValue >= low);
    unsigned i = caseValue - low;
    if (caseDepths[i] != CASE_NOT_DEFINED) {
      return f.fail(s, "no duplicate case labels");
    }
||||||| merged common ancestors
    // Each case value describes the relative depth to the actual block. When
    // a case is not explicitly defined, it goes to the default.
    for (size_t i = 0; i < tableLength; i++) {
        uint32_t target = caseDepths[i] == CASE_NOT_DEFINED ? defaultDepth : caseDepths[i];
        if (!f.encoder().writeVarU32(target)) {
            return false;
        }
    }
=======
  if (operandType.isMaybeFloat()) {
    *type = Type::Floatish;
    return f.encoder().writeOp(Op::F32Neg);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    MOZ_ASSERT(numCases != CASE_NOT_DEFINED);
    caseDepths[i] = numCases++;
  }
||||||| merged common ancestors
    // Write the default depth.
    if (!f.encoder().writeVarU32(defaultDepth)) {
        return false;
    }
=======
  return f.failf(operand, "%s is not a subtype of int, float? or double?",
                 operandType.toChars());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Open the wrapping breakable default block.
  if (!f.pushBreakableBlock()) {
    return false;
  }
||||||| merged common ancestors
    // Our br_table is done. Close its block, write the cases down in order.
    if (!f.popUnbreakableBlock()) {
        return false;
    }
=======
template <typename Unit>
static bool CheckCoerceToInt(FunctionValidator<Unit>& f, ParseNode* expr,
                             Type* type) {
  MOZ_ASSERT(expr->isKind(ParseNodeKind::BitNotExpr));
  ParseNode* operand = UnaryKid(expr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Open all the case blocks.
  for (uint32_t i = 0; i < numCases; i++) {
    if (!f.pushUnbreakableBlock()) {
      return false;
    }
  }
||||||| merged common ancestors
    for (; stmt && !IsDefaultCase(stmt); stmt = NextNode(stmt)) {
        if (!CheckStatement(f, CaseBody(stmt))) {
            return false;
        }
        if (!f.popUnbreakableBlock()) {
            return false;
        }
    }
=======
  Type operandType;
  if (!CheckExpr(f, operand, &operandType)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Open the br_table block.
  if (!f.pushUnbreakableBlock()) {
    return false;
  }
||||||| merged common ancestors
    // Write the default block.
    if (stmt && IsDefaultCase(stmt)) {
        if (!CheckStatement(f, CaseBody(stmt))) {
            return false;
        }
    }
=======
  if (operandType.isMaybeDouble() || operandType.isMaybeFloat()) {
    *type = Type::Signed;
    Op opcode =
        operandType.isMaybeDouble() ? Op::I32TruncSF64 : Op::I32TruncSF32;
    return f.encoder().writeOp(opcode);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The default block is the last one.
  uint32_t defaultDepth = numCases;
||||||| merged common ancestors
    // Close the wrapping block.
    if (!f.popBreakableBlock()) {
        return false;
    }
    return true;
}
=======
  if (!operandType.isIntish()) {
    return f.failf(operand, "%s is not a subtype of double?, float? or intish",
                   operandType.toChars());
  }

  *type = Type::Signed;
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Subtract lowest case value, so that all the cases start from 0.
  if (low) {
    if (!CheckSwitchExpr(f, switchExpr)) {
      return false;
    }
    if (!f.writeInt32Lit(low)) {
      return false;
    }
    if (!f.encoder().writeOp(Op::I32Sub)) {
      return false;
    }
  } else {
    if (!CheckSwitchExpr(f, switchExpr)) {
      return false;
||||||| merged common ancestors
static bool
CheckReturnType(FunctionValidator& f, ParseNode* usepn, Type ret)
{
    if (!f.hasAlreadyReturned()) {
        f.setReturnedType(ret.canonicalToExprType());
        return true;
    }

    if (f.returnedType() != ret.canonicalToExprType()) {
        return f.failf(usepn, "%s incompatible with previous return of type %s",
                       Type::ret(ret).toChars(), ToCString(f.returnedType()));
    }

    return true;
}

static bool
CheckReturn(FunctionValidator& f, ParseNode* returnStmt)
{
    ParseNode* expr = ReturnExpr(returnStmt);

    if (!expr) {
        if (!CheckReturnType(f, returnStmt, Type::Void)) {
            return false;
        }
    } else {
        Type type;
        if (!CheckExpr(f, expr, &type)) {
            return false;
        }

        if (!type.isReturnType()) {
            return f.failf(expr, "%s is not a valid return type", type.toChars());
        }

        if (!CheckReturnType(f, expr, Type::canonicalize(type))) {
            return false;
        }
    }

    if (!f.encoder().writeOp(Op::Return)) {
        return false;
=======
template <typename Unit>
static bool CheckBitNot(FunctionValidator<Unit>& f, ParseNode* neg,
                        Type* type) {
  MOZ_ASSERT(neg->isKind(ParseNodeKind::BitNotExpr));
  ParseNode* operand = UnaryKid(neg);

  if (operand->isKind(ParseNodeKind::BitNotExpr)) {
    return CheckCoerceToInt(f, operand, type);
  }

  Type operandType;
  if (!CheckExpr(f, operand, &operandType)) {
    return false;
  }

  if (!operandType.isIntish()) {
    return f.failf(operand, "%s is not a subtype of intish",
                   operandType.toChars());
  }

  if (!f.encoder().writeOp(MozOp::I32BitNot)) {
    return false;
  }

  *type = Type::Signed;
  return true;
}

template <typename Unit>
static bool CheckAsExprStatement(FunctionValidator<Unit>& f,
                                 ParseNode* exprStmt);

template <typename Unit>
static bool CheckComma(FunctionValidator<Unit>& f, ParseNode* comma,
                       Type* type) {
  MOZ_ASSERT(comma->isKind(ParseNodeKind::CommaExpr));
  ParseNode* operands = ListHead(comma);

  // The block depth isn't taken into account here, because a comma list can't
  // contain breaks and continues and nested control flow structures.
  if (!f.encoder().writeOp(Op::Block)) {
    return false;
  }

  size_t typeAt;
  if (!f.encoder().writePatchableFixedU7(&typeAt)) {
    return false;
  }

  ParseNode* pn = operands;
  for (; NextNode(pn); pn = NextNode(pn)) {
    if (!CheckAsExprStatement(f, pn)) {
      return false;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  // Start the br_table block.
  if (!f.encoder().writeOp(Op::BrTable)) {
    return false;
  }
||||||| merged common ancestors
    return true;
}

static bool
CheckStatementList(FunctionValidator& f, ParseNode* stmtList, const LabelVector* labels /*= nullptr */)
{
    MOZ_ASSERT(stmtList->isKind(ParseNodeKind::StatementList));

    if (!f.pushUnbreakableBlock(labels)) {
        return false;
    }
=======
  if (!CheckExpr(f, pn, type)) {
    return false;
  }

  f.encoder().patchFixedU7(typeAt,
                           uint8_t(type->toWasmBlockSignatureType().code()));

  return f.encoder().writeOp(Op::End);
}

template <typename Unit>
static bool CheckConditional(FunctionValidator<Unit>& f, ParseNode* ternary,
                             Type* type) {
  MOZ_ASSERT(ternary->isKind(ParseNodeKind::ConditionalExpr));

  ParseNode* cond = TernaryKid1(ternary);
  ParseNode* thenExpr = TernaryKid2(ternary);
  ParseNode* elseExpr = TernaryKid3(ternary);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Write the number of cases (tableLength - 1 + 1 (default)).
  // Write the number of cases (tableLength - 1 + 1 (default)).
  if (!f.encoder().writeVarU32(tableLength)) {
    return false;
  }
||||||| merged common ancestors
    for (ParseNode* stmt = ListHead(stmtList); stmt; stmt = NextNode(stmt)) {
        if (!CheckStatement(f, stmt)) {
            return false;
        }
    }
=======
  Type condType;
  if (!CheckExpr(f, cond, &condType)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Each case value describes the relative depth to the actual block. When
  // a case is not explicitly defined, it goes to the default.
  for (size_t i = 0; i < tableLength; i++) {
    uint32_t target =
        caseDepths[i] == CASE_NOT_DEFINED ? defaultDepth : caseDepths[i];
    if (!f.encoder().writeVarU32(target)) {
      return false;
    }
  }
||||||| merged common ancestors
    if (!f.popUnbreakableBlock(labels)) {
        return false;
    }
    return true;
}
=======
  if (!condType.isInt()) {
    return f.failf(cond, "%s is not a subtype of int", condType.toChars());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Write the default depth.
  if (!f.encoder().writeVarU32(defaultDepth)) {
    return false;
  }
||||||| merged common ancestors
static bool
CheckLexicalScope(FunctionValidator& f, ParseNode* node)
{
    LexicalScopeNode* lexicalScope = &node->as<LexicalScopeNode>();
    if (!lexicalScope->isEmptyScope()) {
        return f.fail(lexicalScope, "cannot have 'let' or 'const' declarations");
    }
=======
  size_t typeAt;
  if (!f.pushIf(&typeAt)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Our br_table is done. Close its block, write the cases down in order.
  if (!f.popUnbreakableBlock()) {
    return false;
  }
||||||| merged common ancestors
    return CheckStatement(f, lexicalScope->scopeBody());
}
=======
  Type thenType;
  if (!CheckExpr(f, thenExpr, &thenType)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (; stmt && !IsDefaultCase(stmt); stmt = NextNode(stmt)) {
    if (!CheckStatement(f, CaseBody(stmt))) {
      return false;
    }
    if (!f.popUnbreakableBlock()) {
      return false;
    }
  }
||||||| merged common ancestors
static bool
CheckBreakOrContinue(FunctionValidator& f, bool isBreak, ParseNode* stmt)
{
    if (PropertyName* maybeLabel = LoopControlMaybeLabel(stmt)) {
        return f.writeLabeledBreakOrContinue(maybeLabel, isBreak);
    }
    return f.writeUnlabeledBreakOrContinue(isBreak);
}

static bool
CheckStatement(FunctionValidator& f, ParseNode* stmt)
{
    if (!CheckRecursionLimitDontReport(f.cx())) {
        return f.m().failOverRecursed();
    }
=======
  if (!f.switchToElse()) {
    return false;
  }

  Type elseType;
  if (!CheckExpr(f, elseExpr, &elseType)) {
    return false;
  }

  if (thenType.isInt() && elseType.isInt()) {
    *type = Type::Int;
  } else if (thenType.isDouble() && elseType.isDouble()) {
    *type = Type::Double;
  } else if (thenType.isFloat() && elseType.isFloat()) {
    *type = Type::Float;
  } else {
    return f.failf(
        ternary,
        "then/else branches of conditional must both produce int, float, "
        "double, current types are %s and %s",
        thenType.toChars(), elseType.toChars());
  }

  if (!f.popIf(typeAt, type->toWasmBlockSignatureType())) {
    return false;
  }

  return true;
}

template <typename Unit>
static bool IsValidIntMultiplyConstant(ModuleValidator<Unit>& m,
                                       ParseNode* expr) {
  if (!IsNumericLiteral(m, expr)) {
    return false;
  }

  NumLit lit = ExtractNumericLiteral(m, expr);
  switch (lit.which()) {
    case NumLit::Fixnum:
    case NumLit::NegativeInt:
      if (Abs(lit.toInt32()) < (uint32_t(1) << 20)) {
        return true;
      }
      return false;
    case NumLit::BigUnsigned:
    case NumLit::Double:
    case NumLit::Float:
    case NumLit::OutOfRangeInt:
      return false;
  }

  MOZ_MAKE_COMPILER_ASSUME_IS_UNREACHABLE("Bad literal");
}

template <typename Unit>
static bool CheckMultiply(FunctionValidator<Unit>& f, ParseNode* star,
                          Type* type) {
  MOZ_ASSERT(star->isKind(ParseNodeKind::MulExpr));
  ParseNode* lhs = MultiplyLeft(star);
  ParseNode* rhs = MultiplyRight(star);

  Type lhsType;
  if (!CheckExpr(f, lhs, &lhsType)) {
    return false;
  }

  Type rhsType;
  if (!CheckExpr(f, rhs, &rhsType)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Write the default block.
  if (stmt && IsDefaultCase(stmt)) {
    if (!CheckStatement(f, CaseBody(stmt))) {
      return false;
||||||| merged common ancestors
    switch (stmt->getKind()) {
      case ParseNodeKind::EmptyStatement:       return true;
      case ParseNodeKind::ExpressionStatement:  return CheckExprStatement(f, stmt);
      case ParseNodeKind::While:                return CheckWhile(f, stmt);
      case ParseNodeKind::For:                  return CheckFor(f, stmt);
      case ParseNodeKind::DoWhile:              return CheckDoWhile(f, stmt);
      case ParseNodeKind::Label:                return CheckLabel(f, stmt);
      case ParseNodeKind::If:                   return CheckIf(f, stmt);
      case ParseNodeKind::Switch:               return CheckSwitch(f, stmt);
      case ParseNodeKind::Return:               return CheckReturn(f, stmt);
      case ParseNodeKind::StatementList:        return CheckStatementList(f, stmt);
      case ParseNodeKind::Break:                return CheckBreakOrContinue(f, true, stmt);
      case ParseNodeKind::Continue:             return CheckBreakOrContinue(f, false, stmt);
      case ParseNodeKind::LexicalScope:         return CheckLexicalScope(f, stmt);
      default:;
=======
  if (lhsType.isInt() && rhsType.isInt()) {
    if (!IsValidIntMultiplyConstant(f.m(), lhs) &&
        !IsValidIntMultiplyConstant(f.m(), rhs)) {
      return f.fail(
          star,
          "one arg to int multiply must be a small (-2^20, 2^20) int literal");
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
    *type = Type::Intish;
    return f.encoder().writeOp(Op::I32Mul);
  }

  if (lhsType.isMaybeDouble() && rhsType.isMaybeDouble()) {
    *type = Type::Double;
    return f.encoder().writeOp(Op::F64Mul);
  }

  if (lhsType.isMaybeFloat() && rhsType.isMaybeFloat()) {
    *type = Type::Floatish;
    return f.encoder().writeOp(Op::F32Mul);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Close the wrapping block.
  if (!f.popBreakableBlock()) {
    return false;
  }
  return true;
||||||| merged common ancestors
    return f.fail(stmt, "unexpected statement kind");
=======
  return f.fail(
      star, "multiply operands must be both int, both double? or both float?");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CheckReturnType(FunctionValidator& f, ParseNode* usepn, Type ret) {
  if (!f.hasAlreadyReturned()) {
    f.setReturnedType(ret.canonicalToExprType());
    return true;
  }

  if (f.returnedType() != ret.canonicalToExprType()) {
    return f.failf(usepn, "%s incompatible with previous return of type %s",
                   Type::ret(ret).toChars(), ToCString(f.returnedType()));
  }
||||||| merged common ancestors
static bool
ParseFunction(ModuleValidator& m, CodeNode** funNodeOut, unsigned* line)
{
    auto& tokenStream = m.tokenStream();
=======
template <typename Unit>
static bool CheckAddOrSub(FunctionValidator<Unit>& f, ParseNode* expr,
                          Type* type, unsigned* numAddOrSubOut = nullptr) {
  if (!CheckRecursionLimitDontReport(f.cx())) {
    return f.m().failOverRecursed();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    tokenStream.consumeKnownToken(TokenKind::Function, TokenStreamShared::Operand);
=======
  MOZ_ASSERT(expr->isKind(ParseNodeKind::AddExpr) ||
             expr->isKind(ParseNodeKind::SubExpr));
  ParseNode* lhs = AddSubLeft(expr);
  ParseNode* rhs = AddSubRight(expr);
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckReturn(FunctionValidator& f, ParseNode* returnStmt) {
  ParseNode* expr = ReturnExpr(returnStmt);
||||||| merged common ancestors
    auto& anyChars = tokenStream.anyCharsAccess();
    uint32_t toStringStart = anyChars.currentToken().pos.begin;
    *line = anyChars.srcCoords.lineNum(anyChars.currentToken().pos.end);
=======
  Type lhsType, rhsType;
  unsigned lhsNumAddOrSub, rhsNumAddOrSub;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!expr) {
    if (!CheckReturnType(f, returnStmt, Type::Void)) {
      return false;
||||||| merged common ancestors
    TokenKind tk;
    if (!tokenStream.getToken(&tk, TokenStreamShared::Operand)) {
        return false;
    }
    if (tk == TokenKind::Mul) {
        return m.failCurrentOffset("unexpected generator function");
=======
  if (lhs->isKind(ParseNodeKind::AddExpr) ||
      lhs->isKind(ParseNodeKind::SubExpr)) {
    if (!CheckAddOrSub(f, lhs, &lhsType, &lhsNumAddOrSub)) {
      return false;
    }
    if (lhsType == Type::Intish) {
      lhsType = Type::Int;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  } else {
    Type type;
    if (!CheckExpr(f, expr, &type)) {
      return false;
||||||| merged common ancestors
    if (!TokenKindIsPossibleIdentifier(tk)) {
        return false;  // The regular parser will throw a SyntaxError, no need to m.fail.
=======
  } else {
    if (!CheckExpr(f, lhs, &lhsType)) {
      return false;
>>>>>>> upstream-releases
    }
    lhsNumAddOrSub = 0;
  }

<<<<<<< HEAD
    if (!type.isReturnType()) {
      return f.failf(expr, "%s is not a valid return type", type.toChars());
||||||| merged common ancestors
    RootedPropertyName name(m.cx(), m.parser().bindingIdentifier(YieldIsName));
    if (!name) {
        return false;
=======
  if (rhs->isKind(ParseNodeKind::AddExpr) ||
      rhs->isKind(ParseNodeKind::SubExpr)) {
    if (!CheckAddOrSub(f, rhs, &rhsType, &rhsNumAddOrSub)) {
      return false;
>>>>>>> upstream-releases
    }
    if (rhsType == Type::Intish) {
      rhsType = Type::Int;
    }
  } else {
    if (!CheckExpr(f, rhs, &rhsType)) {
      return false;
    }
    rhsNumAddOrSub = 0;
  }

  unsigned numAddOrSub = lhsNumAddOrSub + rhsNumAddOrSub + 1;
  if (numAddOrSub > (1 << 20)) {
    return f.fail(expr, "too many + or - without intervening coercion");
  }

  if (lhsType.isInt() && rhsType.isInt()) {
    if (!f.encoder().writeOp(
            expr->isKind(ParseNodeKind::AddExpr) ? Op::I32Add : Op::I32Sub)) {
      return false;
    }
    *type = Type::Intish;
  } else if (lhsType.isMaybeDouble() && rhsType.isMaybeDouble()) {
    if (!f.encoder().writeOp(
            expr->isKind(ParseNodeKind::AddExpr) ? Op::F64Add : Op::F64Sub)) {
      return false;
    }
    *type = Type::Double;
  } else if (lhsType.isMaybeFloat() && rhsType.isMaybeFloat()) {
    if (!f.encoder().writeOp(
            expr->isKind(ParseNodeKind::AddExpr) ? Op::F32Add : Op::F32Sub)) {
      return false;
    }
    *type = Type::Floatish;
  } else {
    return f.failf(
        expr,
        "operands to + or - must both be int, float? or double?, got %s and %s",
        lhsType.toChars(), rhsType.toChars());
  }

  if (numAddOrSubOut) {
    *numAddOrSubOut = numAddOrSub;
  }
  return true;
}

template <typename Unit>
static bool CheckDivOrMod(FunctionValidator<Unit>& f, ParseNode* expr,
                          Type* type) {
  MOZ_ASSERT(expr->isKind(ParseNodeKind::DivExpr) ||
             expr->isKind(ParseNodeKind::ModExpr));

  ParseNode* lhs = DivOrModLeft(expr);
  ParseNode* rhs = DivOrModRight(expr);

  Type lhsType, rhsType;
  if (!CheckExpr(f, lhs, &lhsType)) {
    return false;
  }
  if (!CheckExpr(f, rhs, &rhsType)) {
    return false;
  }

<<<<<<< HEAD
    if (!CheckReturnType(f, expr, Type::canonicalize(type))) {
      return false;
||||||| merged common ancestors
    CodeNode* funNode = m.parser().handler.newFunctionStatement(m.parser().pos());
    if (!funNode) {
        return false;
=======
  if (lhsType.isMaybeDouble() && rhsType.isMaybeDouble()) {
    *type = Type::Double;
    if (expr->isKind(ParseNodeKind::DivExpr)) {
      return f.encoder().writeOp(Op::F64Div);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
    return f.encoder().writeOp(MozOp::F64Mod);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!f.encoder().writeOp(Op::Return)) {
    return false;
  }
||||||| merged common ancestors
    RootedFunction& fun = m.dummyFunction();
    fun->setAtom(name);
    fun->setArgCount(0);
=======
  if (lhsType.isMaybeFloat() && rhsType.isMaybeFloat()) {
    *type = Type::Floatish;
    if (expr->isKind(ParseNodeKind::DivExpr)) {
      return f.encoder().writeOp(Op::F32Div);
    } else {
      return f.fail(expr, "modulo cannot receive float arguments");
    }
  }

  if (lhsType.isSigned() && rhsType.isSigned()) {
    *type = Type::Intish;
    return f.encoder().writeOp(
        expr->isKind(ParseNodeKind::DivExpr) ? Op::I32DivS : Op::I32RemS);
  }

  if (lhsType.isUnsigned() && rhsType.isUnsigned()) {
    *type = Type::Intish;
    return f.encoder().writeOp(
        expr->isKind(ParseNodeKind::DivExpr) ? Op::I32DivU : Op::I32RemU);
  }

  return f.failf(
      expr,
      "arguments to / or %% must both be double?, float?, signed, or unsigned; "
      "%s and %s are given",
      lhsType.toChars(), rhsType.toChars());
}

template <typename Unit>
static bool CheckComparison(FunctionValidator<Unit>& f, ParseNode* comp,
                            Type* type) {
  MOZ_ASSERT(comp->isKind(ParseNodeKind::LtExpr) ||
             comp->isKind(ParseNodeKind::LeExpr) ||
             comp->isKind(ParseNodeKind::GtExpr) ||
             comp->isKind(ParseNodeKind::GeExpr) ||
             comp->isKind(ParseNodeKind::EqExpr) ||
             comp->isKind(ParseNodeKind::NeExpr));

  ParseNode* lhs = ComparisonLeft(comp);
  ParseNode* rhs = ComparisonRight(comp);

  Type lhsType, rhsType;
  if (!CheckExpr(f, lhs, &lhsType)) {
    return false;
  }
  if (!CheckExpr(f, rhs, &rhsType)) {
    return false;
  }

  if (!(lhsType.isSigned() && rhsType.isSigned()) &&
      !(lhsType.isUnsigned() && rhsType.isUnsigned()) &&
      !(lhsType.isDouble() && rhsType.isDouble()) &&
      !(lhsType.isFloat() && rhsType.isFloat())) {
    return f.failf(comp,
                   "arguments to a comparison must both be signed, unsigned, "
                   "floats or doubles; "
                   "%s and %s are given",
                   lhsType.toChars(), rhsType.toChars());
  }

  Op stmt;
  if (lhsType.isSigned() && rhsType.isSigned()) {
    switch (comp->getKind()) {
      case ParseNodeKind::EqExpr:
        stmt = Op::I32Eq;
        break;
      case ParseNodeKind::NeExpr:
        stmt = Op::I32Ne;
        break;
      case ParseNodeKind::LtExpr:
        stmt = Op::I32LtS;
        break;
      case ParseNodeKind::LeExpr:
        stmt = Op::I32LeS;
        break;
      case ParseNodeKind::GtExpr:
        stmt = Op::I32GtS;
        break;
      case ParseNodeKind::GeExpr:
        stmt = Op::I32GeS;
        break;
      default:
        MOZ_CRASH("unexpected comparison op");
    }
  } else if (lhsType.isUnsigned() && rhsType.isUnsigned()) {
    switch (comp->getKind()) {
      case ParseNodeKind::EqExpr:
        stmt = Op::I32Eq;
        break;
      case ParseNodeKind::NeExpr:
        stmt = Op::I32Ne;
        break;
      case ParseNodeKind::LtExpr:
        stmt = Op::I32LtU;
        break;
      case ParseNodeKind::LeExpr:
        stmt = Op::I32LeU;
        break;
      case ParseNodeKind::GtExpr:
        stmt = Op::I32GtU;
        break;
      case ParseNodeKind::GeExpr:
        stmt = Op::I32GeU;
        break;
      default:
        MOZ_CRASH("unexpected comparison op");
    }
  } else if (lhsType.isDouble()) {
    switch (comp->getKind()) {
      case ParseNodeKind::EqExpr:
        stmt = Op::F64Eq;
        break;
      case ParseNodeKind::NeExpr:
        stmt = Op::F64Ne;
        break;
      case ParseNodeKind::LtExpr:
        stmt = Op::F64Lt;
        break;
      case ParseNodeKind::LeExpr:
        stmt = Op::F64Le;
        break;
      case ParseNodeKind::GtExpr:
        stmt = Op::F64Gt;
        break;
      case ParseNodeKind::GeExpr:
        stmt = Op::F64Ge;
        break;
      default:
        MOZ_CRASH("unexpected comparison op");
    }
  } else if (lhsType.isFloat()) {
    switch (comp->getKind()) {
      case ParseNodeKind::EqExpr:
        stmt = Op::F32Eq;
        break;
      case ParseNodeKind::NeExpr:
        stmt = Op::F32Ne;
        break;
      case ParseNodeKind::LtExpr:
        stmt = Op::F32Lt;
        break;
      case ParseNodeKind::LeExpr:
        stmt = Op::F32Le;
        break;
      case ParseNodeKind::GtExpr:
        stmt = Op::F32Gt;
        break;
      case ParseNodeKind::GeExpr:
        stmt = Op::F32Ge;
        break;
      default:
        MOZ_CRASH("unexpected comparison op");
    }
  } else {
    MOZ_CRASH("unexpected type");
  }

  *type = Type::Int;
  return f.encoder().writeOp(stmt);
}

template <typename Unit>
static bool CheckBitwise(FunctionValidator<Unit>& f, ParseNode* bitwise,
                         Type* type) {
  ParseNode* lhs = BitwiseLeft(bitwise);
  ParseNode* rhs = BitwiseRight(bitwise);

  int32_t identityElement;
  bool onlyOnRight;
  switch (bitwise->getKind()) {
    case ParseNodeKind::BitOrExpr:
      identityElement = 0;
      onlyOnRight = false;
      *type = Type::Signed;
      break;
    case ParseNodeKind::BitAndExpr:
      identityElement = -1;
      onlyOnRight = false;
      *type = Type::Signed;
      break;
    case ParseNodeKind::BitXorExpr:
      identityElement = 0;
      onlyOnRight = false;
      *type = Type::Signed;
      break;
    case ParseNodeKind::LshExpr:
      identityElement = 0;
      onlyOnRight = true;
      *type = Type::Signed;
      break;
    case ParseNodeKind::RshExpr:
      identityElement = 0;
      onlyOnRight = true;
      *type = Type::Signed;
      break;
    case ParseNodeKind::UrshExpr:
      identityElement = 0;
      onlyOnRight = true;
      *type = Type::Unsigned;
      break;
    default:
      MOZ_CRASH("not a bitwise op");
  }

  uint32_t i;
  if (!onlyOnRight && IsLiteralInt(f.m(), lhs, &i) &&
      i == uint32_t(identityElement)) {
    Type rhsType;
    if (!CheckExpr(f, rhs, &rhsType)) {
      return false;
    }
    if (!rhsType.isIntish()) {
      return f.failf(bitwise, "%s is not a subtype of intish",
                     rhsType.toChars());
    }
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    ParseContext* outerpc = m.parser().pc;
    Directives directives(outerpc);
    FunctionBox* funbox = m.parser().newFunctionBox(funNode, fun, toStringStart, directives,
                                                    GeneratorKind::NotGenerator,
                                                    FunctionAsyncKind::SyncFunction);
    if (!funbox) {
        return false;
    }
    funbox->initWithEnclosingParseContext(outerpc, FunctionSyntaxKind::Statement);
=======
  if (IsLiteralInt(f.m(), rhs, &i) && i == uint32_t(identityElement)) {
    if (bitwise->isKind(ParseNodeKind::BitOrExpr) &&
        lhs->isKind(ParseNodeKind::CallExpr)) {
      return CheckCoercedCall(f, lhs, Type::Int, type);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckStatementList(FunctionValidator& f, ParseNode* stmtList,
                               const LabelVector* labels /*= nullptr */) {
  MOZ_ASSERT(stmtList->isKind(ParseNodeKind::StatementList));
||||||| merged common ancestors
    Directives newDirectives = directives;
    SourceParseContext funpc(&m.parser(), funbox, &newDirectives);
    if (!funpc.init()) {
        return false;
    }
=======
    Type lhsType;
    if (!CheckExpr(f, lhs, &lhsType)) {
      return false;
    }
    if (!lhsType.isIntish()) {
      return f.failf(bitwise, "%s is not a subtype of intish",
                     lhsType.toChars());
    }
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!f.pushUnbreakableBlock(labels)) {
    return false;
  }
||||||| merged common ancestors
    if (!m.parser().functionFormalParametersAndBody(InAllowed, YieldIsName, &funNode,
                                                    FunctionSyntaxKind::Statement))
    {
        if (anyChars.hadError() || directives == newDirectives) {
            return false;
        }
=======
  Type lhsType;
  if (!CheckExpr(f, lhs, &lhsType)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (ParseNode* stmt = ListHead(stmtList); stmt; stmt = NextNode(stmt)) {
    if (!CheckStatement(f, stmt)) {
      return false;
||||||| merged common ancestors
        return m.fail(funNode, "encountered new directive in function");
=======
  Type rhsType;
  if (!CheckExpr(f, rhs, &rhsType)) {
    return false;
  }

  if (!lhsType.isIntish()) {
    return f.failf(lhs, "%s is not a subtype of intish", lhsType.toChars());
  }
  if (!rhsType.isIntish()) {
    return f.failf(rhs, "%s is not a subtype of intish", rhsType.toChars());
  }

  switch (bitwise->getKind()) {
    case ParseNodeKind::BitOrExpr:
      if (!f.encoder().writeOp(Op::I32Or)) return false;
      break;
    case ParseNodeKind::BitAndExpr:
      if (!f.encoder().writeOp(Op::I32And)) return false;
      break;
    case ParseNodeKind::BitXorExpr:
      if (!f.encoder().writeOp(Op::I32Xor)) return false;
      break;
    case ParseNodeKind::LshExpr:
      if (!f.encoder().writeOp(Op::I32Shl)) return false;
      break;
    case ParseNodeKind::RshExpr:
      if (!f.encoder().writeOp(Op::I32ShrS)) return false;
      break;
    case ParseNodeKind::UrshExpr:
      if (!f.encoder().writeOp(Op::I32ShrU)) return false;
      break;
    default:
      MOZ_CRASH("not a bitwise op");
  }

  return true;
}

template <typename Unit>
static bool CheckExpr(FunctionValidator<Unit>& f, ParseNode* expr, Type* type) {
  if (!CheckRecursionLimitDontReport(f.cx())) {
    return f.m().failOverRecursed();
  }

  if (IsNumericLiteral(f.m(), expr)) {
    return CheckNumericLiteral(f, expr, type);
  }

  switch (expr->getKind()) {
    case ParseNodeKind::Name:
      return CheckVarRef(f, expr, type);
    case ParseNodeKind::ElemExpr:
      return CheckLoadArray(f, expr, type);
    case ParseNodeKind::AssignExpr:
      return CheckAssign(f, expr, type);
    case ParseNodeKind::PosExpr:
      return CheckPos(f, expr, type);
    case ParseNodeKind::NotExpr:
      return CheckNot(f, expr, type);
    case ParseNodeKind::NegExpr:
      return CheckNeg(f, expr, type);
    case ParseNodeKind::BitNotExpr:
      return CheckBitNot(f, expr, type);
    case ParseNodeKind::CommaExpr:
      return CheckComma(f, expr, type);
    case ParseNodeKind::ConditionalExpr:
      return CheckConditional(f, expr, type);
    case ParseNodeKind::MulExpr:
      return CheckMultiply(f, expr, type);
    case ParseNodeKind::CallExpr:
      return CheckUncoercedCall(f, expr, type);

    case ParseNodeKind::AddExpr:
    case ParseNodeKind::SubExpr:
      return CheckAddOrSub(f, expr, type);

    case ParseNodeKind::DivExpr:
    case ParseNodeKind::ModExpr:
      return CheckDivOrMod(f, expr, type);

    case ParseNodeKind::LtExpr:
    case ParseNodeKind::LeExpr:
    case ParseNodeKind::GtExpr:
    case ParseNodeKind::GeExpr:
    case ParseNodeKind::EqExpr:
    case ParseNodeKind::NeExpr:
      return CheckComparison(f, expr, type);

    case ParseNodeKind::BitOrExpr:
    case ParseNodeKind::BitAndExpr:
    case ParseNodeKind::BitXorExpr:
    case ParseNodeKind::LshExpr:
    case ParseNodeKind::RshExpr:
    case ParseNodeKind::UrshExpr:
      return CheckBitwise(f, expr, type);

    default:;
  }

  return f.fail(expr, "unsupported expression");
}

template <typename Unit>
static bool CheckStatement(FunctionValidator<Unit>& f, ParseNode* stmt);

template <typename Unit>
static bool CheckAsExprStatement(FunctionValidator<Unit>& f, ParseNode* expr) {
  if (expr->isKind(ParseNodeKind::CallExpr)) {
    Type ignored;
    return CheckCoercedCall(f, expr, Type::Void, &ignored);
  }

  Type resultType;
  if (!CheckExpr(f, expr, &resultType)) {
    return false;
  }

  if (!resultType.isVoid()) {
    if (!f.encoder().writeOp(Op::Drop)) {
      return false;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (!f.popUnbreakableBlock(labels)) {
    return false;
  }
  return true;
}

static bool CheckLexicalScope(FunctionValidator& f, ParseNode* node) {
  LexicalScopeNode* lexicalScope = &node->as<LexicalScopeNode>();
  if (!lexicalScope->isEmptyScope()) {
    return f.fail(lexicalScope, "cannot have 'let' or 'const' declarations");
  }

  return CheckStatement(f, lexicalScope->scopeBody());
}

static bool CheckBreakOrContinue(FunctionValidator& f, bool isBreak,
                                 ParseNode* stmt) {
  if (PropertyName* maybeLabel = LoopControlMaybeLabel(stmt)) {
    return f.writeLabeledBreakOrContinue(maybeLabel, isBreak);
  }
  return f.writeUnlabeledBreakOrContinue(isBreak);
}

static bool CheckStatement(FunctionValidator& f, ParseNode* stmt) {
  if (!CheckRecursionLimitDontReport(f.cx())) {
    return f.m().failOverRecursed();
  }

  switch (stmt->getKind()) {
    case ParseNodeKind::EmptyStatement:
      return true;
    case ParseNodeKind::ExpressionStatement:
      return CheckExprStatement(f, stmt);
    case ParseNodeKind::While:
      return CheckWhile(f, stmt);
    case ParseNodeKind::For:
      return CheckFor(f, stmt);
    case ParseNodeKind::DoWhile:
      return CheckDoWhile(f, stmt);
    case ParseNodeKind::Label:
      return CheckLabel(f, stmt);
    case ParseNodeKind::If:
      return CheckIf(f, stmt);
    case ParseNodeKind::Switch:
      return CheckSwitch(f, stmt);
    case ParseNodeKind::Return:
      return CheckReturn(f, stmt);
    case ParseNodeKind::StatementList:
      return CheckStatementList(f, stmt);
    case ParseNodeKind::Break:
      return CheckBreakOrContinue(f, true, stmt);
    case ParseNodeKind::Continue:
      return CheckBreakOrContinue(f, false, stmt);
    case ParseNodeKind::LexicalScope:
      return CheckLexicalScope(f, stmt);
    default:;
  }

  return f.fail(stmt, "unexpected statement kind");
}

static bool ParseFunction(ModuleValidator& m, CodeNode** funNodeOut,
                          unsigned* line) {
  auto& tokenStream = m.tokenStream();

  tokenStream.consumeKnownToken(TokenKind::Function,
                                TokenStreamShared::Operand);

  auto& anyChars = tokenStream.anyCharsAccess();
  uint32_t toStringStart = anyChars.currentToken().pos.begin;
  *line = anyChars.srcCoords.lineNum(anyChars.currentToken().pos.end);

  TokenKind tk;
  if (!tokenStream.getToken(&tk, TokenStreamShared::Operand)) {
    return false;
  }
  if (tk == TokenKind::Mul) {
    return m.failCurrentOffset("unexpected generator function");
  }
  if (!TokenKindIsPossibleIdentifier(tk)) {
    return false;  // The regular parser will throw a SyntaxError, no need to
                   // m.fail.
  }

  RootedPropertyName name(m.cx(), m.parser().bindingIdentifier(YieldIsName));
  if (!name) {
    return false;
  }
||||||| merged common ancestors
    MOZ_ASSERT(!anyChars.hadError());
    MOZ_ASSERT(directives == newDirectives);
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  CodeNode* funNode = m.parser().handler.newFunctionStatement(m.parser().pos());
  if (!funNode) {
    return false;
  }

  RootedFunction& fun = m.dummyFunction();
  fun->setAtom(name);
  fun->setArgCount(0);

  ParseContext* outerpc = m.parser().pc;
  Directives directives(outerpc);
  FunctionBox* funbox = m.parser().newFunctionBox(
      funNode, fun, toStringStart, directives, GeneratorKind::NotGenerator,
      FunctionAsyncKind::SyncFunction);
  if (!funbox) {
    return false;
  }
  funbox->initWithEnclosingParseContext(outerpc, FunctionSyntaxKind::Statement);
||||||| merged common ancestors
    *funNodeOut = funNode;
    return true;
}
=======
template <typename Unit>
static bool CheckExprStatement(FunctionValidator<Unit>& f,
                               ParseNode* exprStmt) {
  MOZ_ASSERT(exprStmt->isKind(ParseNodeKind::ExpressionStmt));
  return CheckAsExprStatement(f, UnaryKid(exprStmt));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Directives newDirectives = directives;
  SourceParseContext funpc(&m.parser(), funbox, &newDirectives);
  if (!funpc.init()) {
    return false;
  }
||||||| merged common ancestors
static bool
CheckFunction(ModuleValidator& m)
{
    // asm.js modules can be quite large when represented as parse trees so pop
    // the backing LifoAlloc after parsing/compiling each function. Release the
    // parser's lifo memory after the last use of a parse node.
    AsmJSParser::Mark mark = m.parser().mark();
    auto releaseMark = mozilla::MakeScopeExit([&] {
        m.parser().release(mark);
    });

    CodeNode* funNode = nullptr;
    unsigned line = 0;
    if (!ParseFunction(m, &funNode, &line)) {
        return false;
    }
=======
template <typename Unit>
static bool CheckLoopConditionOnEntry(FunctionValidator<Unit>& f,
                                      ParseNode* cond) {
  uint32_t maybeLit;
  if (IsLiteralInt(f.m(), cond, &maybeLit) && maybeLit) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!m.parser().functionFormalParametersAndBody(
          InAllowed, YieldIsName, &funNode, FunctionSyntaxKind::Statement)) {
    if (anyChars.hadError() || directives == newDirectives) {
      return false;
    }
||||||| merged common ancestors
    if (!CheckFunctionHead(m, funNode)) {
        return false;
    }
=======
  Type condType;
  if (!CheckExpr(f, cond, &condType)) {
    return false;
  }
  if (!condType.isInt()) {
    return f.failf(cond, "%s is not a subtype of int", condType.toChars());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    return m.fail(funNode, "encountered new directive in function");
  }
||||||| merged common ancestors
    FunctionValidator f(m, funNode);
=======
  if (!f.encoder().writeOp(Op::I32Eqz)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(!anyChars.hadError());
  MOZ_ASSERT(directives == newDirectives);
||||||| merged common ancestors
    ParseNode* stmtIter = ListHead(FunctionStatementList(funNode));
=======
  // brIf (i32.eqz $f) $out
  if (!f.writeBreakIf()) {
    return false;
  }

  return true;
}

template <typename Unit>
static bool CheckWhile(FunctionValidator<Unit>& f, ParseNode* whileStmt,
                       const LabelVector* labels = nullptr) {
  MOZ_ASSERT(whileStmt->isKind(ParseNodeKind::WhileStmt));
  ParseNode* cond = BinaryLeft(whileStmt);
  ParseNode* body = BinaryRight(whileStmt);

  // A while loop `while(#cond) #body` is equivalent to:
  // (block $after_loop
  //    (loop $top
  //       (brIf $after_loop (i32.eq 0 #cond))
  //       #body
  //       (br $top)
  //    )
  // )
  if (labels && !f.addLabels(*labels, 0, 1)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *funNodeOut = funNode;
  return true;
}
||||||| merged common ancestors
    if (!CheckProcessingDirectives(m, &stmtIter)) {
        return false;
    }
=======
  if (!f.pushLoop()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckFunction(ModuleValidator& m) {
  // asm.js modules can be quite large when represented as parse trees so pop
  // the backing LifoAlloc after parsing/compiling each function. Release the
  // parser's lifo memory after the last use of a parse node.
  AsmJSParser::Mark mark = m.parser().mark();
  auto releaseMark = mozilla::MakeScopeExit([&] { m.parser().release(mark); });
||||||| merged common ancestors
    ValTypeVector args;
    if (!CheckArguments(f, &stmtIter, &args)) {
        return false;
    }
=======
  if (!CheckLoopConditionOnEntry(f, cond)) {
    return false;
  }
  if (!CheckStatement(f, body)) {
    return false;
  }
  if (!f.writeContinue()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  CodeNode* funNode = nullptr;
  unsigned line = 0;
  if (!ParseFunction(m, &funNode, &line)) {
    return false;
  }
||||||| merged common ancestors
    if (!CheckVariables(f, &stmtIter)) {
        return false;
    }
=======
  if (!f.popLoop()) {
    return false;
  }
  if (labels) {
    f.removeLabels(*labels);
  }
  return true;
}

template <typename Unit>
static bool CheckFor(FunctionValidator<Unit>& f, ParseNode* forStmt,
                     const LabelVector* labels = nullptr) {
  MOZ_ASSERT(forStmt->isKind(ParseNodeKind::ForStmt));
  ParseNode* forHead = BinaryLeft(forStmt);
  ParseNode* body = BinaryRight(forStmt);

  if (!forHead->isKind(ParseNodeKind::ForHead)) {
    return f.fail(forHead, "unsupported for-loop statement");
  }

  ParseNode* maybeInit = TernaryKid1(forHead);
  ParseNode* maybeCond = TernaryKid2(forHead);
  ParseNode* maybeInc = TernaryKid3(forHead);

  // A for-loop `for (#init; #cond; #inc) #body` is equivalent to:
  // (block                                               // depth X
  //   (#init)
  //   (block $after_loop                                 // depth X+1 (block)
  //     (loop $loop_top                                  // depth X+2 (loop)
  //       (brIf $after (eq 0 #cond))
  //       (block $after_body #body)                      // depth X+3
  //       #inc
  //       (br $loop_top)
  //     )
  //   )
  // )
  // A break in the body should break out to $after_loop, i.e. depth + 1.
  // A continue in the body should break out to $after_body, i.e. depth + 3.
  if (labels && !f.addLabels(*labels, 1, 3)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckFunctionHead(m, funNode)) {
    return false;
  }
||||||| merged common ancestors
    ParseNode* lastNonEmptyStmt = nullptr;
    for (; stmtIter; stmtIter = NextNonEmptyStatement(stmtIter)) {
        lastNonEmptyStmt = stmtIter;
        if (!CheckStatement(f, stmtIter)) {
            return false;
        }
    }
=======
  if (!f.pushUnbreakableBlock()) {
    return false;
  }

  if (maybeInit && !CheckAsExprStatement(f, maybeInit)) {
    return false;
  }

  {
    if (!f.pushLoop()) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  FunctionValidator f(m, funNode);
||||||| merged common ancestors
    if (!CheckFinalReturn(f, lastNonEmptyStmt)) {
        return false;
    }
=======
    if (maybeCond && !CheckLoopConditionOnEntry(f, maybeCond)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* stmtIter = ListHead(FunctionStatementList(funNode));
||||||| merged common ancestors
    ModuleValidator::Func* func = nullptr;
    if (!CheckFunctionSignature(m, funNode, FuncType(std::move(args), f.returnedType()),
                                FunctionName(funNode), &func))
    {
        return false;
    }
=======
    {
      // Continuing in the body should just break out to the increment.
      if (!f.pushContinuableBlock()) {
        return false;
      }
      if (!CheckStatement(f, body)) {
        return false;
      }
      if (!f.popContinuableBlock()) {
        return false;
      }
    }

    if (maybeInc && !CheckAsExprStatement(f, maybeInc)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckProcessingDirectives(m, &stmtIter)) {
    return false;
  }
||||||| merged common ancestors
    if (func->defined()) {
        return m.failName(funNode, "function '%s' already defined", FunctionName(funNode));
    }
=======
    if (!f.writeContinue()) {
      return false;
    }
    if (!f.popLoop()) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ValTypeVector args;
  if (!CheckArguments(f, &stmtIter, &args)) {
    return false;
  }
||||||| merged common ancestors
    f.define(func, line);
=======
  if (!f.popUnbreakableBlock()) {
    return false;
  }

  if (labels) {
    f.removeLabels(*labels);
  }

  return true;
}

template <typename Unit>
static bool CheckDoWhile(FunctionValidator<Unit>& f, ParseNode* whileStmt,
                         const LabelVector* labels = nullptr) {
  MOZ_ASSERT(whileStmt->isKind(ParseNodeKind::DoWhileStmt));
  ParseNode* body = BinaryLeft(whileStmt);
  ParseNode* cond = BinaryRight(whileStmt);

  // A do-while loop `do { #body } while (#cond)` is equivalent to:
  // (block $after_loop           // depth X
  //   (loop $top                 // depth X+1
  //     (block #body)            // depth X+2
  //     (brIf #cond $top)
  //   )
  // )
  // A break should break out of the entire loop, i.e. at depth 0.
  // A continue should break out to the condition, i.e. at depth 2.
  if (labels && !f.addLabels(*labels, 0, 2)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckVariables(f, &stmtIter)) {
    return false;
  }
||||||| merged common ancestors
    return true;
}
=======
  if (!f.pushLoop()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* lastNonEmptyStmt = nullptr;
  for (; stmtIter; stmtIter = NextNonEmptyStatement(stmtIter)) {
    lastNonEmptyStmt = stmtIter;
    if (!CheckStatement(f, stmtIter)) {
      return false;
||||||| merged common ancestors
static bool
CheckAllFunctionsDefined(ModuleValidator& m)
{
    for (unsigned i = 0; i < m.numFuncDefs(); i++) {
        const ModuleValidator::Func& f = m.funcDef(i);
        if (!f.defined()) {
            return m.failNameOffset(f.firstUse(), "missing definition of function %s", f.name());
        }
=======
  {
    // An unlabeled continue in the body should break out to the condition.
    if (!f.pushContinuableBlock()) {
      return false;
    }
    if (!CheckStatement(f, body)) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
    if (!f.popContinuableBlock()) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckFinalReturn(f, lastNonEmptyStmt)) {
    return false;
  }
||||||| merged common ancestors
    return true;
}
=======
  Type condType;
  if (!CheckExpr(f, cond, &condType)) {
    return false;
  }
  if (!condType.isInt()) {
    return f.failf(cond, "%s is not a subtype of int", condType.toChars());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ModuleValidator::Func* func = nullptr;
  if (!CheckFunctionSignature(m, funNode,
                              FuncType(std::move(args), f.returnedType()),
                              FunctionName(funNode), &func)) {
    return false;
  }
||||||| merged common ancestors
static bool
CheckFunctions(ModuleValidator& m)
{
    while (true) {
        TokenKind tk;
        if (!PeekToken(m.parser(), &tk)) {
            return false;
        }
=======
  if (!f.writeContinueIf()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (func->defined()) {
    return m.failName(funNode, "function '%s' already defined",
                      FunctionName(funNode));
  }
||||||| merged common ancestors
        if (tk != TokenKind::Function) {
            break;
        }
=======
  if (!f.popLoop()) {
    return false;
  }
  if (labels) {
    f.removeLabels(*labels);
  }
  return true;
}

template <typename Unit>
static bool CheckStatementList(FunctionValidator<Unit>& f, ParseNode*,
                               const LabelVector* = nullptr);

template <typename Unit>
static bool CheckLabel(FunctionValidator<Unit>& f, ParseNode* labeledStmt) {
  MOZ_ASSERT(labeledStmt->isKind(ParseNodeKind::LabelStmt));

  LabelVector labels;
  ParseNode* innermost = labeledStmt;
  do {
    if (!labels.append(LabeledStatementLabel(innermost))) {
      return false;
    }
    innermost = LabeledStatementStatement(innermost);
  } while (innermost->getKind() == ParseNodeKind::LabelStmt);

  switch (innermost->getKind()) {
    case ParseNodeKind::ForStmt:
      return CheckFor(f, innermost, &labels);
    case ParseNodeKind::DoWhileStmt:
      return CheckDoWhile(f, innermost, &labels);
    case ParseNodeKind::WhileStmt:
      return CheckWhile(f, innermost, &labels);
    case ParseNodeKind::StatementList:
      return CheckStatementList(f, innermost, &labels);
    default:
      break;
  }

  if (!f.pushUnbreakableBlock(&labels)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  f.define(func, line);
||||||| merged common ancestors
        if (!CheckFunction(m)) {
            return false;
        }
    }
=======
  if (!CheckStatement(f, innermost)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return CheckAllFunctionsDefined(m);
=======
  if (!f.popUnbreakableBlock(&labels)) {
    return false;
  }
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CheckAllFunctionsDefined(ModuleValidator& m) {
  for (unsigned i = 0; i < m.numFuncDefs(); i++) {
    const ModuleValidator::Func& f = m.funcDef(i);
    if (!f.defined()) {
      return m.failNameOffset(f.firstUse(), "missing definition of function %s",
                              f.name());
    }
  }

  return true;
}

static bool CheckFunctions(ModuleValidator& m) {
  while (true) {
    TokenKind tk;
    if (!PeekToken(m.parser(), &tk)) {
      return false;
    }

    if (tk != TokenKind::Function) {
      break;
    }
||||||| merged common ancestors
static bool
CheckFuncPtrTable(ModuleValidator& m, ParseNode* var)
{
    if (!var->isKind(ParseNodeKind::Name)) {
        return m.fail(var, "function-pointer table name is not a plain name");
    }

    ParseNode* arrayLiteral = MaybeInitializer(var);
    if (!arrayLiteral || !arrayLiteral->isKind(ParseNodeKind::Array)) {
        return m.fail(var, "function-pointer table's initializer must be an array literal");
    }

    unsigned length = ListLength(arrayLiteral);
=======
template <typename Unit>
static bool CheckIf(FunctionValidator<Unit>& f, ParseNode* ifStmt) {
  uint32_t numIfEnd = 1;
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!CheckFunction(m)) {
      return false;
    }
  }
||||||| merged common ancestors
    if (!IsPowerOfTwo(length)) {
        return m.failf(arrayLiteral, "function-pointer table length must be a power of 2 (is %u)", length);
    }
=======
recurse:
  MOZ_ASSERT(ifStmt->isKind(ParseNodeKind::IfStmt));
  ParseNode* cond = TernaryKid1(ifStmt);
  ParseNode* thenStmt = TernaryKid2(ifStmt);
  ParseNode* elseStmt = TernaryKid3(ifStmt);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return CheckAllFunctionsDefined(m);
}
||||||| merged common ancestors
    unsigned mask = length - 1;
=======
  Type condType;
  if (!CheckExpr(f, cond, &condType)) {
    return false;
  }
  if (!condType.isInt()) {
    return f.failf(cond, "%s is not a subtype of int", condType.toChars());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckFuncPtrTable(ModuleValidator& m, ParseNode* var) {
  if (!var->isKind(ParseNodeKind::Name)) {
    return m.fail(var, "function-pointer table name is not a plain name");
  }
||||||| merged common ancestors
    Uint32Vector elemFuncDefIndices;
    const FuncType* sig = nullptr;
    for (ParseNode* elem = ListHead(arrayLiteral); elem; elem = NextNode(elem)) {
        if (!elem->isKind(ParseNodeKind::Name)) {
            return m.fail(elem, "function-pointer table's elements must be names of functions");
        }
=======
  size_t typeAt;
  if (!f.pushIf(&typeAt)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* arrayLiteral = MaybeInitializer(var);
  if (!arrayLiteral || !arrayLiteral->isKind(ParseNodeKind::Array)) {
    return m.fail(
        var, "function-pointer table's initializer must be an array literal");
  }
||||||| merged common ancestors
        PropertyName* funcName = elem->as<NameNode>().name();
        const ModuleValidator::Func* func = m.lookupFuncDef(funcName);
        if (!func) {
            return m.fail(elem, "function-pointer table's elements must be names of functions");
        }
=======
  f.setIfType(typeAt, ExprType::Void);
>>>>>>> upstream-releases

<<<<<<< HEAD
  unsigned length = ListLength(arrayLiteral);
||||||| merged common ancestors
        const FuncType& funcSig = m.env().types[func->sigIndex()].funcType();
        if (sig) {
            if (*sig != funcSig) {
                return m.fail(elem, "all functions in table must have same signature");
            }
        } else {
            sig = &funcSig;
        }
=======
  if (!CheckStatement(f, thenStmt)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!IsPowerOfTwo(length)) {
    return m.failf(arrayLiteral,
                   "function-pointer table length must be a power of 2 (is %u)",
                   length);
  }

  unsigned mask = length - 1;

  Uint32Vector elemFuncDefIndices;
  const FuncType* sig = nullptr;
  for (ParseNode* elem = ListHead(arrayLiteral); elem; elem = NextNode(elem)) {
    if (!elem->isKind(ParseNodeKind::Name)) {
      return m.fail(
          elem, "function-pointer table's elements must be names of functions");
||||||| merged common ancestors
        if (!elemFuncDefIndices.append(func->funcDefIndex())) {
            return false;
        }
=======
  if (elseStmt) {
    if (!f.switchToElse()) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    PropertyName* funcName = elem->as<NameNode>().name();
    const ModuleValidator::Func* func = m.lookupFuncDef(funcName);
    if (!func) {
      return m.fail(
          elem, "function-pointer table's elements must be names of functions");
||||||| merged common ancestors
    FuncType copy;
    if (!copy.clone(*sig)) {
        return false;
=======
    if (elseStmt->isKind(ParseNodeKind::IfStmt)) {
      ifStmt = elseStmt;
      if (numIfEnd++ == UINT32_MAX) {
        return false;
      }
      goto recurse;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    const FuncType& funcSig = m.env().types[func->sigIndex()].funcType();
    if (sig) {
      if (*sig != funcSig) {
        return m.fail(elem, "all functions in table must have same signature");
      }
    } else {
      sig = &funcSig;
||||||| merged common ancestors
    uint32_t tableIndex;
    if (!CheckFuncPtrTableAgainstExisting(m, var, var->as<NameNode>().name(), std::move(copy), mask, &tableIndex)) {
        return false;
=======
    if (!CheckStatement(f, elseStmt)) {
      return false;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
    if (!elemFuncDefIndices.append(func->funcDefIndex())) {
      return false;
||||||| merged common ancestors
    if (!m.defineFuncPtrTable(tableIndex, std::move(elemFuncDefIndices))) {
        return m.fail(var, "duplicate function-pointer definition");
=======
  for (uint32_t i = 0; i != numIfEnd; ++i) {
    if (!f.popIf()) {
      return false;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  FuncType copy;
  if (!copy.clone(*sig)) {
    return false;
  }
||||||| merged common ancestors
    return true;
}
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t tableIndex;
  if (!CheckFuncPtrTableAgainstExisting(m, var, var->as<NameNode>().name(),
                                        std::move(copy), mask, &tableIndex)) {
    return false;
  }
||||||| merged common ancestors
static bool
CheckFuncPtrTables(ModuleValidator& m)
{
    while (true) {
        ParseNode* varStmt;
        if (!ParseVarOrConstStatement(m.parser(), &varStmt)) {
            return false;
        }
        if (!varStmt) {
            break;
        }
        for (ParseNode* var = VarListHead(varStmt); var; var = NextNode(var)) {
            if (!CheckFuncPtrTable(m, var)) {
                return false;
            }
        }
    }
=======
static bool CheckCaseExpr(FunctionValidatorShared& f, ParseNode* caseExpr,
                          int32_t* value) {
  if (!IsNumericLiteral(f.m(), caseExpr)) {
    return f.fail(caseExpr,
                  "switch case expression must be an integer literal");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!m.defineFuncPtrTable(tableIndex, std::move(elemFuncDefIndices))) {
    return m.fail(var, "duplicate function-pointer definition");
  }
||||||| merged common ancestors
    for (unsigned i = 0; i < m.numFuncPtrTables(); i++) {
        ModuleValidator::Table& table = m.table(i);
        if (!table.defined()) {
            return m.failNameOffset(table.firstUse(),
                                    "function-pointer table %s wasn't defined",
                                    table.name());
        }
    }
=======
  NumLit lit = ExtractNumericLiteral(f.m(), caseExpr);
  switch (lit.which()) {
    case NumLit::Fixnum:
    case NumLit::NegativeInt:
      *value = lit.toInt32();
      break;
    case NumLit::OutOfRangeInt:
    case NumLit::BigUnsigned:
      return f.fail(caseExpr, "switch case expression out of integer range");
    case NumLit::Double:
    case NumLit::Float:
      return f.fail(caseExpr,
                    "switch case expression must be an integer literal");
  }
>>>>>>> upstream-releases

  return true;
}

<<<<<<< HEAD
static bool CheckFuncPtrTables(ModuleValidator& m) {
  while (true) {
    ParseNode* varStmt;
    if (!ParseVarOrConstStatement(m.parser(), &varStmt)) {
      return false;
    }
    if (!varStmt) {
      break;
||||||| merged common ancestors
static bool
CheckModuleExportFunction(ModuleValidator& m, ParseNode* pn, PropertyName* maybeFieldName = nullptr)
{
    if (!pn->isKind(ParseNodeKind::Name)) {
        return m.fail(pn, "expected name of exported function");
=======
static bool CheckDefaultAtEnd(FunctionValidatorShared& f, ParseNode* stmt) {
  for (; stmt; stmt = NextNode(stmt)) {
    if (IsDefaultCase(stmt) && NextNode(stmt) != nullptr) {
      return f.fail(stmt, "default label must be at the end");
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    for (ParseNode* var = VarListHead(varStmt); var; var = NextNode(var)) {
      if (!CheckFuncPtrTable(m, var)) {
        return false;
      }
    }
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (unsigned i = 0; i < m.numFuncPtrTables(); i++) {
    ModuleValidator::Table& table = m.table(i);
    if (!table.defined()) {
      return m.failNameOffset(table.firstUse(),
                              "function-pointer table %s wasn't defined",
                              table.name());
    }
  }

  return true;
||||||| merged common ancestors
    PropertyName* funcName = pn->as<NameNode>().name();
    const ModuleValidator::Func* func = m.lookupFuncDef(funcName);
    if (!func) {
        return m.failName(pn, "function '%s' not found", funcName);
    }

    return m.addExportField(*func, maybeFieldName);
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CheckModuleExportFunction(ModuleValidator& m, ParseNode* pn,
                                      PropertyName* maybeFieldName = nullptr) {
  if (!pn->isKind(ParseNodeKind::Name)) {
    return m.fail(pn, "expected name of exported function");
  }

  PropertyName* funcName = pn->as<NameNode>().name();
  const ModuleValidator::Func* func = m.lookupFuncDef(funcName);
  if (!func) {
    return m.failName(pn, "function '%s' not found", funcName);
  }
||||||| merged common ancestors
static bool
CheckModuleExportObject(ModuleValidator& m, ParseNode* object)
{
    MOZ_ASSERT(object->isKind(ParseNodeKind::Object));

    for (ParseNode* pn = ListHead(object); pn; pn = NextNode(pn)) {
        if (!IsNormalObjectField(pn)) {
            return m.fail(pn, "only normal object properties may be used in the export object literal");
        }
=======
static bool CheckSwitchRange(FunctionValidatorShared& f, ParseNode* stmt,
                             int32_t* low, int32_t* high,
                             uint32_t* tableLength) {
  if (IsDefaultCase(stmt)) {
    *low = 0;
    *high = -1;
    *tableLength = 0;
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return m.addExportField(*func, maybeFieldName);
}
||||||| merged common ancestors
        PropertyName* fieldName = ObjectNormalFieldName(pn);
=======
  int32_t i = 0;
  if (!CheckCaseExpr(f, CaseExpr(stmt), &i)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckModuleExportObject(ModuleValidator& m, ParseNode* object) {
  MOZ_ASSERT(object->isKind(ParseNodeKind::Object));
||||||| merged common ancestors
        ParseNode* initNode = ObjectNormalFieldInitializer(pn);
        if (!initNode->isKind(ParseNodeKind::Name)) {
            return m.fail(initNode, "initializer of exported object literal must be name of function");
        }
=======
  *low = *high = i;
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (ParseNode* pn = ListHead(object); pn; pn = NextNode(pn)) {
    if (!IsNormalObjectField(pn)) {
      return m.fail(pn,
                    "only normal object properties may be used in the export "
                    "object literal");
||||||| merged common ancestors
        if (!CheckModuleExportFunction(m, initNode, fieldName)) {
            return false;
        }
=======
  ParseNode* initialStmt = stmt;
  for (stmt = NextNode(stmt); stmt && !IsDefaultCase(stmt);
       stmt = NextNode(stmt)) {
    int32_t i = 0;
    if (!CheckCaseExpr(f, CaseExpr(stmt), &i)) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    PropertyName* fieldName = ObjectNormalFieldName(pn);
||||||| merged common ancestors
    return true;
}
=======
    *low = Min(*low, i);
    *high = Max(*high, i);
  }

  int64_t i64 = (int64_t(*high) - int64_t(*low)) + 1;
  if (i64 > MaxBrTableElems) {
    return f.fail(
        initialStmt,
        "all switch statements generate tables; this table would be too big");
  }

  *tableLength = uint32_t(i64);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    ParseNode* initNode = ObjectNormalFieldInitializer(pn);
    if (!initNode->isKind(ParseNodeKind::Name)) {
      return m.fail(
          initNode,
          "initializer of exported object literal must be name of function");
    }
||||||| merged common ancestors
static bool
CheckModuleReturn(ModuleValidator& m)
{
    TokenKind tk;
    if (!GetToken(m.parser(), &tk)) {
        return false;
    }
    auto& ts = m.parser().tokenStream;
    if (tk != TokenKind::Return) {
        return m.failCurrentOffset((tk == TokenKind::RightCurly || tk == TokenKind::Eof)
                                   ? "expecting return statement"
                                   : "invalid asm.js. statement");
    }
    ts.anyCharsAccess().ungetToken();
=======
template <typename Unit>
static bool CheckSwitchExpr(FunctionValidator<Unit>& f, ParseNode* switchExpr) {
  Type exprType;
  if (!CheckExpr(f, switchExpr, &exprType)) {
    return false;
  }
  if (!exprType.isSigned()) {
    return f.failf(switchExpr, "%s is not a subtype of signed",
                   exprType.toChars());
  }
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!CheckModuleExportFunction(m, initNode, fieldName)) {
      return false;
    }
  }
||||||| merged common ancestors
    ParseNode* returnStmt = m.parser().statementListItem(YieldIsName);
    if (!returnStmt) {
        return false;
    }
=======
// A switch will be constructed as:
// - the default block wrapping all the other blocks, to be able to break
// out of the switch with an unlabeled break statement. It has two statements
// (an inner block and the default expr). asm.js rules require default to be at
// the end, so the default block always encloses all the cases blocks.
// - one block per case between low and high; undefined cases just jump to the
// default case. Each of these blocks contain two statements: the next case's
// block and the possibly empty statement list comprising the case body. The
// last block pushed is the first case so the (relative) branch target therefore
// matches the sequential order of cases.
// - one block for the br_table, so that the first break goes to the first
// case's block.
template <typename Unit>
static bool CheckSwitch(FunctionValidator<Unit>& f, ParseNode* switchStmt) {
  MOZ_ASSERT(switchStmt->isKind(ParseNodeKind::SwitchStmt));
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    ParseNode* returnExpr = ReturnExpr(returnStmt);
    if (!returnExpr) {
        return m.fail(returnStmt, "export statement must return something");
    }
=======
  ParseNode* switchExpr = BinaryLeft(switchStmt);
  ParseNode* switchBody = BinaryRight(switchStmt);
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckModuleReturn(ModuleValidator& m) {
  TokenKind tk;
  if (!GetToken(m.parser(), &tk)) {
    return false;
  }
  auto& ts = m.parser().tokenStream;
  if (tk != TokenKind::Return) {
    return m.failCurrentOffset(
        (tk == TokenKind::RightCurly || tk == TokenKind::Eof)
            ? "expecting return statement"
            : "invalid asm.js. statement");
  }
  ts.anyCharsAccess().ungetToken();

  ParseNode* returnStmt = m.parser().statementListItem(YieldIsName);
  if (!returnStmt) {
    return false;
  }

  ParseNode* returnExpr = ReturnExpr(returnStmt);
  if (!returnExpr) {
    return m.fail(returnStmt, "export statement must return something");
  }

  if (returnExpr->isKind(ParseNodeKind::Object)) {
    if (!CheckModuleExportObject(m, returnExpr)) {
      return false;
    }
  } else {
    if (!CheckModuleExportFunction(m, returnExpr)) {
      return false;
||||||| merged common ancestors
    if (returnExpr->isKind(ParseNodeKind::Object)) {
        if (!CheckModuleExportObject(m, returnExpr)) {
            return false;
        }
    } else {
        if (!CheckModuleExportFunction(m, returnExpr)) {
            return false;
        }
=======
  if (switchBody->is<LexicalScopeNode>()) {
    LexicalScopeNode* scope = &switchBody->as<LexicalScopeNode>();
    if (!scope->isEmptyScope()) {
      return f.fail(scope, "switch body may not contain lexical declarations");
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
    switchBody = scope->scopeBody();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
  ParseNode* stmt = ListHead(switchBody);
  if (!stmt) {
    if (!CheckSwitchExpr(f, switchExpr)) {
      return false;
    }
    if (!f.encoder().writeOp(Op::Drop)) {
      return false;
    }
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CheckModuleEnd(ModuleValidator& m) {
  TokenKind tk;
  if (!GetToken(m.parser(), &tk)) {
    return false;
  }
||||||| merged common ancestors
static bool
CheckModuleEnd(ModuleValidator &m)
{
    TokenKind tk;
    if (!GetToken(m.parser(), &tk)) {
        return false;
    }
=======
  if (!CheckDefaultAtEnd(f, stmt)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (tk != TokenKind::Eof && tk != TokenKind::RightCurly) {
    return m.failCurrentOffset(
        "top-level export (return) must be the last statement");
  }
||||||| merged common ancestors
    if (tk != TokenKind::Eof && tk != TokenKind::RightCurly) {
        return m.failCurrentOffset("top-level export (return) must be the last statement");
    }
=======
  int32_t low = 0, high = 0;
  uint32_t tableLength = 0;
  if (!CheckSwitchRange(f, stmt, &low, &high, &tableLength)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  m.parser().tokenStream.anyCharsAccess().ungetToken();
  return true;
}
||||||| merged common ancestors
    m.parser().tokenStream.anyCharsAccess().ungetToken();
    return true;
}
=======
  static const uint32_t CASE_NOT_DEFINED = UINT32_MAX;
>>>>>>> upstream-releases

<<<<<<< HEAD
static SharedModule CheckModule(JSContext* cx, AsmJSParser& parser,
                                ParseNode* stmtList, UniqueLinkData* linkData,
                                unsigned* time) {
  int64_t before = PRMJ_Now();
||||||| merged common ancestors
static SharedModule
CheckModule(JSContext* cx, AsmJSParser& parser, ParseNode* stmtList, UniqueLinkData* linkData,
            unsigned* time)
{
    int64_t before = PRMJ_Now();
=======
  Uint32Vector caseDepths;
  if (!caseDepths.appendN(CASE_NOT_DEFINED, tableLength)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  CodeNode* moduleFunctionNode = parser.pc->functionBox()->functionNode;
||||||| merged common ancestors
    CodeNode* moduleFunctionNode = parser.pc->functionBox()->functionNode;
=======
  uint32_t numCases = 0;
  for (ParseNode* s = stmt; s && !IsDefaultCase(s); s = NextNode(s)) {
    int32_t caseValue = ExtractNumericLiteral(f.m(), CaseExpr(s)).toInt32();
>>>>>>> upstream-releases

<<<<<<< HEAD
  ModuleValidator m(cx, parser, moduleFunctionNode);
  if (!m.init()) {
    return nullptr;
  }
||||||| merged common ancestors
    ModuleValidator m(cx, parser, moduleFunctionNode);
    if (!m.init()) {
        return nullptr;
    }
=======
    MOZ_ASSERT(caseValue >= low);
    unsigned i = caseValue - low;
    if (caseDepths[i] != CASE_NOT_DEFINED) {
      return f.fail(s, "no duplicate case labels");
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckFunctionHead(m, moduleFunctionNode)) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!CheckFunctionHead(m, moduleFunctionNode)) {
        return nullptr;
    }
=======
    MOZ_ASSERT(numCases != CASE_NOT_DEFINED);
    caseDepths[i] = numCases++;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckModuleArguments(m, moduleFunctionNode)) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!CheckModuleArguments(m, moduleFunctionNode)) {
        return nullptr;
    }
=======
  // Open the wrapping breakable default block.
  if (!f.pushBreakableBlock()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckPrecedingStatements(m, stmtList)) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!CheckPrecedingStatements(m, stmtList)) {
        return nullptr;
    }
=======
  // Open all the case blocks.
  for (uint32_t i = 0; i < numCases; i++) {
    if (!f.pushUnbreakableBlock()) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckModuleProcessingDirectives(m)) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!CheckModuleProcessingDirectives(m)) {
        return nullptr;
    }
=======
  // Open the br_table block.
  if (!f.pushUnbreakableBlock()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckModuleGlobals(m)) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!CheckModuleGlobals(m)) {
        return nullptr;
    }
=======
  // The default block is the last one.
  uint32_t defaultDepth = numCases;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!m.startFunctionBodies()) {
    return nullptr;
  }

  if (!CheckFunctions(m)) {
    return nullptr;
  }

  if (!CheckFuncPtrTables(m)) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!m.startFunctionBodies()) {
        return nullptr;
    }

    if (!CheckFunctions(m)) {
        return nullptr;
    }

    if (!CheckFuncPtrTables(m)) {
        return nullptr;
    }
=======
  // Subtract lowest case value, so that all the cases start from 0.
  if (low) {
    if (!CheckSwitchExpr(f, switchExpr)) {
      return false;
    }
    if (!f.writeInt32Lit(low)) {
      return false;
    }
    if (!f.encoder().writeOp(Op::I32Sub)) {
      return false;
    }
  } else {
    if (!CheckSwitchExpr(f, switchExpr)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckModuleReturn(m)) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!CheckModuleReturn(m)) {
        return nullptr;
    }
=======
  // Start the br_table block.
  if (!f.encoder().writeOp(Op::BrTable)) {
    return false;
  }

  // Write the number of cases (tableLength - 1 + 1 (default)).
  // Write the number of cases (tableLength - 1 + 1 (default)).
  if (!f.encoder().writeVarU32(tableLength)) {
    return false;
  }

  // Each case value describes the relative depth to the actual block. When
  // a case is not explicitly defined, it goes to the default.
  for (size_t i = 0; i < tableLength; i++) {
    uint32_t target =
        caseDepths[i] == CASE_NOT_DEFINED ? defaultDepth : caseDepths[i];
    if (!f.encoder().writeVarU32(target)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckModuleEnd(m)) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!CheckModuleEnd(m)) {
        return nullptr;
    }
=======
  // Write the default depth.
  if (!f.encoder().writeVarU32(defaultDepth)) {
    return false;
  }

  // Our br_table is done. Close its block, write the cases down in order.
  if (!f.popUnbreakableBlock()) {
    return false;
  }

  for (; stmt && !IsDefaultCase(stmt); stmt = NextNode(stmt)) {
    if (!CheckStatement(f, CaseBody(stmt))) {
      return false;
    }
    if (!f.popUnbreakableBlock()) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  SharedModule module = m.finish(linkData);
  if (!module) {
    return nullptr;
  }
||||||| merged common ancestors
    SharedModule module = m.finish(linkData);
    if (!module) {
        return nullptr;
    }
=======
  // Write the default block.
  if (stmt && IsDefaultCase(stmt)) {
    if (!CheckStatement(f, CaseBody(stmt))) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *time = (PRMJ_Now() - before) / PRMJ_USEC_PER_MSEC;
  return module;
||||||| merged common ancestors
    *time = (PRMJ_Now() - before) / PRMJ_USEC_PER_MSEC;
    return module;
=======
  // Close the wrapping block.
  if (!f.popBreakableBlock()) {
    return false;
  }
  return true;
>>>>>>> upstream-releases
}

static bool CheckReturnType(FunctionValidatorShared& f, ParseNode* usepn,
                            Type ret) {
  if (!f.hasAlreadyReturned()) {
    f.setReturnedType(ret.canonicalToExprType());
    return true;
  }

<<<<<<< HEAD
static bool LinkFail(JSContext* cx, const char* str) {
  JS_ReportErrorFlagsAndNumberASCII(cx, JSREPORT_WARNING, GetErrorMessage,
                                    nullptr, JSMSG_USE_ASM_LINK_FAIL, str);
  return false;
}
||||||| merged common ancestors
static bool
LinkFail(JSContext* cx, const char* str)
{
    JS_ReportErrorFlagsAndNumberASCII(cx, JSREPORT_WARNING, GetErrorMessage, nullptr,
                                      JSMSG_USE_ASM_LINK_FAIL, str);
    return false;
}
=======
  if (f.returnedType() != ret.canonicalToExprType()) {
    return f.failf(usepn, "%s incompatible with previous return of type %s",
                   Type::ret(ret).toChars(), ToCString(f.returnedType()));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool IsMaybeWrappedScriptedProxy(JSObject* obj) {
  JSObject* unwrapped = UncheckedUnwrap(obj);
  return unwrapped && IsScriptedProxy(unwrapped);
||||||| merged common ancestors
static bool
IsMaybeWrappedScriptedProxy(JSObject* obj)
{
    JSObject* unwrapped = UncheckedUnwrap(obj);
    return unwrapped && IsScriptedProxy(unwrapped);
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool GetDataProperty(JSContext* cx, HandleValue objVal, HandleAtom field,
                            MutableHandleValue v) {
  if (!objVal.isObject()) {
    return LinkFail(cx, "accessing property of non-object");
  }
||||||| merged common ancestors
static bool
GetDataProperty(JSContext* cx, HandleValue objVal, HandleAtom field, MutableHandleValue v)
{
    if (!objVal.isObject()) {
        return LinkFail(cx, "accessing property of non-object");
    }
=======
template <typename Unit>
static bool CheckReturn(FunctionValidator<Unit>& f, ParseNode* returnStmt) {
  ParseNode* expr = ReturnExpr(returnStmt);
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedObject obj(cx, &objVal.toObject());
  if (IsMaybeWrappedScriptedProxy(obj)) {
    return LinkFail(cx, "accessing property of a Proxy");
  }

  Rooted<PropertyDescriptor> desc(cx);
  RootedId id(cx, AtomToId(field));
  if (!GetPropertyDescriptor(cx, obj, id, &desc)) {
    return false;
  }
||||||| merged common ancestors
    RootedObject obj(cx, &objVal.toObject());
    if (IsMaybeWrappedScriptedProxy(obj)) {
        return LinkFail(cx, "accessing property of a Proxy");
    }

    Rooted<PropertyDescriptor> desc(cx);
    RootedId id(cx, AtomToId(field));
    if (!GetPropertyDescriptor(cx, obj, id, &desc)) {
        return false;
    }
=======
  if (!expr) {
    if (!CheckReturnType(f, returnStmt, Type::Void)) {
      return false;
    }
  } else {
    Type type;
    if (!CheckExpr(f, expr, &type)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!desc.object()) {
    return LinkFail(cx, "property not present on object");
  }
||||||| merged common ancestors
    if (!desc.object()) {
        return LinkFail(cx, "property not present on object");
    }
=======
    if (!type.isReturnType()) {
      return f.failf(expr, "%s is not a valid return type", type.toChars());
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!desc.isDataDescriptor()) {
    return LinkFail(cx, "property is not a data property");
  }
||||||| merged common ancestors
    if (!desc.isDataDescriptor()) {
        return LinkFail(cx, "property is not a data property");
    }
=======
    if (!CheckReturnType(f, expr, Type::canonicalize(type))) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  v.set(desc.value());
  return true;
||||||| merged common ancestors
    v.set(desc.value());
    return true;
=======
  if (!f.encoder().writeOp(Op::Return)) {
    return false;
  }

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool GetDataProperty(JSContext* cx, HandleValue objVal,
                            const char* fieldChars, MutableHandleValue v) {
  RootedAtom field(cx, AtomizeUTF8Chars(cx, fieldChars, strlen(fieldChars)));
  if (!field) {
    return false;
  }
||||||| merged common ancestors
static bool
GetDataProperty(JSContext* cx, HandleValue objVal, const char* fieldChars, MutableHandleValue v)
{
    RootedAtom field(cx, AtomizeUTF8Chars(cx, fieldChars, strlen(fieldChars)));
    if (!field) {
        return false;
    }
=======
template <typename Unit>
static bool CheckStatementList(FunctionValidator<Unit>& f, ParseNode* stmtList,
                               const LabelVector* labels /*= nullptr */) {
  MOZ_ASSERT(stmtList->isKind(ParseNodeKind::StatementList));

  if (!f.pushUnbreakableBlock(labels)) {
    return false;
  }

  for (ParseNode* stmt = ListHead(stmtList); stmt; stmt = NextNode(stmt)) {
    if (!CheckStatement(f, stmt)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return GetDataProperty(cx, objVal, field, v);
}
||||||| merged common ancestors
    return GetDataProperty(cx, objVal, field, v);
}
=======
  if (!f.popUnbreakableBlock(labels)) {
    return false;
  }
  return true;
}

template <typename Unit>
static bool CheckLexicalScope(FunctionValidator<Unit>& f, ParseNode* node) {
  LexicalScopeNode* lexicalScope = &node->as<LexicalScopeNode>();
  if (!lexicalScope->isEmptyScope()) {
    return f.fail(lexicalScope, "cannot have 'let' or 'const' declarations");
  }

  return CheckStatement(f, lexicalScope->scopeBody());
}

static bool CheckBreakOrContinue(FunctionValidatorShared& f, bool isBreak,
                                 ParseNode* stmt) {
  if (PropertyName* maybeLabel = LoopControlMaybeLabel(stmt)) {
    return f.writeLabeledBreakOrContinue(maybeLabel, isBreak);
  }
  return f.writeUnlabeledBreakOrContinue(isBreak);
}

template <typename Unit>
static bool CheckStatement(FunctionValidator<Unit>& f, ParseNode* stmt) {
  if (!CheckRecursionLimitDontReport(f.cx())) {
    return f.m().failOverRecursed();
  }

  switch (stmt->getKind()) {
    case ParseNodeKind::EmptyStmt:
      return true;
    case ParseNodeKind::ExpressionStmt:
      return CheckExprStatement(f, stmt);
    case ParseNodeKind::WhileStmt:
      return CheckWhile(f, stmt);
    case ParseNodeKind::ForStmt:
      return CheckFor(f, stmt);
    case ParseNodeKind::DoWhileStmt:
      return CheckDoWhile(f, stmt);
    case ParseNodeKind::LabelStmt:
      return CheckLabel(f, stmt);
    case ParseNodeKind::IfStmt:
      return CheckIf(f, stmt);
    case ParseNodeKind::SwitchStmt:
      return CheckSwitch(f, stmt);
    case ParseNodeKind::ReturnStmt:
      return CheckReturn(f, stmt);
    case ParseNodeKind::StatementList:
      return CheckStatementList(f, stmt);
    case ParseNodeKind::BreakStmt:
      return CheckBreakOrContinue(f, true, stmt);
    case ParseNodeKind::ContinueStmt:
      return CheckBreakOrContinue(f, false, stmt);
    case ParseNodeKind::LexicalScope:
      return CheckLexicalScope(f, stmt);
    default:;
  }

  return f.fail(stmt, "unexpected statement kind");
}

template <typename Unit>
static bool ParseFunction(ModuleValidator<Unit>& m, FunctionNode** funNodeOut,
                          unsigned* line) {
  auto& tokenStream = m.tokenStream();

  tokenStream.consumeKnownToken(TokenKind::Function,
                                TokenStreamShared::SlashIsRegExp);

  auto& anyChars = tokenStream.anyCharsAccess();
  uint32_t toStringStart = anyChars.currentToken().pos.begin;
  *line = anyChars.lineNumber(anyChars.lineToken(toStringStart));

  TokenKind tk;
  if (!tokenStream.getToken(&tk, TokenStreamShared::SlashIsRegExp)) {
    return false;
  }
  if (tk == TokenKind::Mul) {
    return m.failCurrentOffset("unexpected generator function");
  }
  if (!TokenKindIsPossibleIdentifier(tk)) {
    return false;  // The regular parser will throw a SyntaxError, no need to
                   // m.fail.
  }

  RootedPropertyName name(m.cx(), m.parser().bindingIdentifier(YieldIsName));
  if (!name) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool GetDataProperty(JSContext* cx, HandleValue objVal,
                            ImmutablePropertyNamePtr field,
                            MutableHandleValue v) {
  // Help the conversion along for all the cx->names().* users.
  HandlePropertyName fieldHandle = field;
  return GetDataProperty(cx, objVal, fieldHandle, v);
}
||||||| merged common ancestors
static bool
GetDataProperty(JSContext* cx, HandleValue objVal, ImmutablePropertyNamePtr field, MutableHandleValue v)
{
    // Help the conversion along for all the cx->names().* users.
    HandlePropertyName fieldHandle = field;
    return GetDataProperty(cx, objVal, fieldHandle, v);
}
=======
  FunctionNode* funNode = m.parser().handler_.newFunction(
      FunctionSyntaxKind::Statement, m.parser().pos());
  if (!funNode) {
    return false;
  }

  RootedFunction& fun = m.dummyFunction();
  fun->setAtom(name);
  fun->setArgCount(0);

  ParseContext* outerpc = m.parser().pc_;
  Directives directives(outerpc);
  FunctionBox* funbox = m.parser().newFunctionBox(
      funNode, fun, toStringStart, directives, GeneratorKind::NotGenerator,
      FunctionAsyncKind::SyncFunction);
  if (!funbox) {
    return false;
  }
  funbox->initWithEnclosingParseContext(outerpc, fun,
                                        FunctionSyntaxKind::Statement);
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool HasObjectValueOfMethodPure(JSObject* obj, JSContext* cx) {
  Value v;
  if (!GetPropertyPure(cx, obj, NameToId(cx->names().valueOf), &v)) {
    return false;
  }
||||||| merged common ancestors
static bool
HasObjectValueOfMethodPure(JSObject* obj, JSContext* cx)
{
    Value v;
    if (!GetPropertyPure(cx, obj, NameToId(cx->names().valueOf), &v)) {
        return false;
    }

    JSFunction* fun;
    if (!IsFunctionObject(v, &fun)) {
        return false;
    }

    return IsSelfHostedFunctionWithName(fun, cx->names().Object_valueOf);
}
=======
  Directives newDirectives = directives;
  SourceParseContext funpc(&m.parser(), funbox, &newDirectives);
  if (!funpc.init()) {
    return false;
  }

  if (!m.parser().functionFormalParametersAndBody(
          InAllowed, YieldIsName, &funNode, FunctionSyntaxKind::Statement)) {
    if (anyChars.hadError() || directives == newDirectives) {
      return false;
    }

    return m.fail(funNode, "encountered new directive in function");
  }

  MOZ_ASSERT(!anyChars.hadError());
  MOZ_ASSERT(directives == newDirectives);

  *funNodeOut = funNode;
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSFunction* fun;
  if (!IsFunctionObject(v, &fun)) {
||||||| merged common ancestors
static bool
HasPureCoercion(JSContext* cx, HandleValue v)
{
    // Ideally, we'd reject all non-primitives, but Emscripten has a bug that
    // generates code that passes functions for some imports. To avoid breaking
    // all the code that contains this bug, we make an exception for functions
    // that don't have user-defined valueOf or toString, for their coercions
    // are not observable and coercion via ToNumber/ToInt32 definitely produces
    // NaN/0. We should remove this special case later once most apps have been
    // built with newer Emscripten.
    if (v.toObject().is<JSFunction>() &&
        HasNoToPrimitiveMethodPure(&v.toObject(), cx) &&
        HasObjectValueOfMethodPure(&v.toObject(), cx) &&
        HasNativeMethodPure(&v.toObject(), cx->names().toString, fun_toString, cx))
    {
        return true;
    }
=======
template <typename Unit>
static bool CheckFunction(ModuleValidator<Unit>& m) {
  // asm.js modules can be quite large when represented as parse trees so pop
  // the backing LifoAlloc after parsing/compiling each function. Release the
  // parser's lifo memory after the last use of a parse node.
  frontend::ParserBase::Mark mark = m.parser().mark();
  auto releaseMark =
      mozilla::MakeScopeExit([&m, &mark] { m.parser().release(mark); });

  FunctionNode* funNode = nullptr;
  unsigned line = 0;
  if (!ParseFunction(m, &funNode, &line)) {
>>>>>>> upstream-releases
    return false;
<<<<<<< HEAD
  }

  return IsSelfHostedFunctionWithName(fun, cx->names().Object_valueOf);
}

static bool HasPureCoercion(JSContext* cx, HandleValue v) {
  // Ideally, we'd reject all non-primitives, but Emscripten has a bug that
  // generates code that passes functions for some imports. To avoid breaking
  // all the code that contains this bug, we make an exception for functions
  // that don't have user-defined valueOf or toString, for their coercions
  // are not observable and coercion via ToNumber/ToInt32 definitely produces
  // NaN/0. We should remove this special case later once most apps have been
  // built with newer Emscripten.
  if (v.toObject().is<JSFunction>() &&
      HasNoToPrimitiveMethodPure(&v.toObject(), cx) &&
      HasObjectValueOfMethodPure(&v.toObject(), cx) &&
      HasNativeMethodPure(&v.toObject(), cx->names().toString, fun_toString,
                          cx)) {
    return true;
  }
  return false;
}
||||||| merged common ancestors
}
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool ValidateGlobalVariable(JSContext* cx, const AsmJSGlobal& global,
                                   HandleValue importVal,
                                   Maybe<LitValPOD>* val) {
  switch (global.varInitKind()) {
    case AsmJSGlobal::InitConstant:
      val->emplace(global.varInitVal());
      return true;
||||||| merged common ancestors
static bool
ValidateGlobalVariable(JSContext* cx, const AsmJSGlobal& global, HandleValue importVal,
                       Maybe<LitValPOD>* val)
{
    switch (global.varInitKind()) {
      case AsmJSGlobal::InitConstant:
        val->emplace(global.varInitVal());
        return true;
=======
  if (!CheckFunctionHead(m, funNode)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case AsmJSGlobal::InitImport: {
      RootedValue v(cx);
      if (!GetDataProperty(cx, importVal, global.field(), &v)) {
        return false;
      }
||||||| merged common ancestors
      case AsmJSGlobal::InitImport: {
        RootedValue v(cx);
        if (!GetDataProperty(cx, importVal, global.field(), &v)) {
            return false;
        }
=======
  FunctionValidator<Unit> f(m, funNode);
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (!v.isPrimitive() && !HasPureCoercion(cx, v)) {
        return LinkFail(cx, "Imported values must be primitives");
      }
||||||| merged common ancestors
        if (!v.isPrimitive() && !HasPureCoercion(cx, v)) {
            return LinkFail(cx, "Imported values must be primitives");
        }
=======
  ParseNode* stmtIter = ListHead(FunctionStatementList(funNode));
>>>>>>> upstream-releases

<<<<<<< HEAD
      switch (global.varInitImportType().code()) {
        case ValType::I32: {
          int32_t i32;
          if (!ToInt32(cx, v, &i32)) {
            return false;
          }
          val->emplace(uint32_t(i32));
          return true;
        }
        case ValType::I64:
          MOZ_CRASH("int64");
        case ValType::F32: {
          float f;
          if (!RoundFloat32(cx, v, &f)) {
            return false;
          }
          val->emplace(f);
          return true;
        }
        case ValType::F64: {
          double d;
          if (!ToNumber(cx, v, &d)) {
            return false;
          }
          val->emplace(d);
          return true;
        }
        case ValType::Ref:
        case ValType::NullRef:
        case ValType::AnyRef: {
          MOZ_CRASH("not available in asm.js");
        }
      }
    }
  }
||||||| merged common ancestors
        switch (global.varInitImportType().code()) {
          case ValType::I32: {
            int32_t i32;
            if (!ToInt32(cx, v, &i32)) {
                return false;
            }
            val->emplace(uint32_t(i32));
            return true;
          }
          case ValType::I64:
            MOZ_CRASH("int64");
          case ValType::F32: {
            float f;
            if (!RoundFloat32(cx, v, &f)) {
                return false;
            }
            val->emplace(f);
            return true;
          }
          case ValType::F64: {
            double d;
            if (!ToNumber(cx, v, &d)) {
                return false;
            }
            val->emplace(d);
            return true;
          }
          case ValType::Ref:
          case ValType::AnyRef: {
            MOZ_CRASH("not available in asm.js");
          }
        }
      }
    }
=======
  if (!CheckProcessingDirectives(m, &stmtIter)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_CRASH("unreachable");
}
||||||| merged common ancestors
    MOZ_CRASH("unreachable");
}
=======
  ValTypeVector args;
  if (!CheckArguments(f, &stmtIter, &args)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool ValidateFFI(JSContext* cx, const AsmJSGlobal& global,
                        HandleValue importVal,
                        MutableHandle<FunctionVector> ffis) {
  RootedValue v(cx);
  if (!GetDataProperty(cx, importVal, global.field(), &v)) {
    return false;
  }
||||||| merged common ancestors
static bool
ValidateFFI(JSContext* cx, const AsmJSGlobal& global, HandleValue importVal,
            MutableHandle<FunctionVector> ffis)
{
    RootedValue v(cx);
    if (!GetDataProperty(cx, importVal, global.field(), &v)) {
        return false;
    }
=======
  if (!CheckVariables(f, &stmtIter)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!IsFunctionObject(v)) {
    return LinkFail(cx, "FFI imports must be functions");
  }
||||||| merged common ancestors
    if (!IsFunctionObject(v)) {
        return LinkFail(cx, "FFI imports must be functions");
    }
=======
  ParseNode* lastNonEmptyStmt = nullptr;
  for (; stmtIter; stmtIter = NextNonEmptyStatement(stmtIter)) {
    lastNonEmptyStmt = stmtIter;
    if (!CheckStatement(f, stmtIter)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ffis[global.ffiIndex()].set(&v.toObject().as<JSFunction>());
  return true;
}
||||||| merged common ancestors
    ffis[global.ffiIndex()].set(&v.toObject().as<JSFunction>());
    return true;
}
=======
  if (!CheckFinalReturn(f, lastNonEmptyStmt)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool ValidateArrayView(JSContext* cx, const AsmJSGlobal& global,
                              HandleValue globalVal) {
  if (!global.field()) {
    return true;
  }
||||||| merged common ancestors
static bool
ValidateArrayView(JSContext* cx, const AsmJSGlobal& global, HandleValue globalVal)
{
    if (!global.field()) {
        return true;
    }

    RootedValue v(cx);
    if (!GetDataProperty(cx, globalVal, global.field(), &v)) {
        return false;
    }

    bool tac = IsTypedArrayConstructor(v, global.viewType());
    if (!tac) {
        return LinkFail(cx, "bad typed array constructor");
    }

    return true;
}
=======
  ModuleValidatorShared::Func* func = nullptr;
  if (!CheckFunctionSignature(m, funNode,
                              FuncType(std::move(args), f.returnedType()),
                              FunctionName(funNode), &func)) {
    return false;
  }

  if (func->defined()) {
    return m.failName(funNode, "function '%s' already defined",
                      FunctionName(funNode));
  }

  f.define(func, line);

  return true;
}

static bool CheckAllFunctionsDefined(ModuleValidatorShared& m) {
  for (unsigned i = 0; i < m.numFuncDefs(); i++) {
    const ModuleValidatorShared::Func& f = m.funcDef(i);
    if (!f.defined()) {
      return m.failNameOffset(f.firstUse(), "missing definition of function %s",
                              f.name());
    }
  }

  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedValue v(cx);
  if (!GetDataProperty(cx, globalVal, global.field(), &v)) {
    return false;
  }
||||||| merged common ancestors
static bool
ValidateMathBuiltinFunction(JSContext* cx, const AsmJSGlobal& global, HandleValue globalVal)
{
    RootedValue v(cx);
    if (!GetDataProperty(cx, globalVal, cx->names().Math, &v)) {
        return false;
    }

    if (!GetDataProperty(cx, v, global.field(), &v)) {
        return false;
    }
=======
template <typename Unit>
static bool CheckFunctions(ModuleValidator<Unit>& m) {
  while (true) {
    TokenKind tk;
    if (!PeekToken(m.parser(), &tk)) {
      return false;
    }

    if (tk != TokenKind::Function) {
      break;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool tac = IsTypedArrayConstructor(v, global.viewType());
  if (!tac) {
    return LinkFail(cx, "bad typed array constructor");
  }
||||||| merged common ancestors
    Native native = nullptr;
    switch (global.mathBuiltinFunction()) {
      case AsmJSMathBuiltin_sin: native = math_sin; break;
      case AsmJSMathBuiltin_cos: native = math_cos; break;
      case AsmJSMathBuiltin_tan: native = math_tan; break;
      case AsmJSMathBuiltin_asin: native = math_asin; break;
      case AsmJSMathBuiltin_acos: native = math_acos; break;
      case AsmJSMathBuiltin_atan: native = math_atan; break;
      case AsmJSMathBuiltin_ceil: native = math_ceil; break;
      case AsmJSMathBuiltin_floor: native = math_floor; break;
      case AsmJSMathBuiltin_exp: native = math_exp; break;
      case AsmJSMathBuiltin_log: native = math_log; break;
      case AsmJSMathBuiltin_pow: native = math_pow; break;
      case AsmJSMathBuiltin_sqrt: native = math_sqrt; break;
      case AsmJSMathBuiltin_min: native = math_min; break;
      case AsmJSMathBuiltin_max: native = math_max; break;
      case AsmJSMathBuiltin_abs: native = math_abs; break;
      case AsmJSMathBuiltin_atan2: native = math_atan2; break;
      case AsmJSMathBuiltin_imul: native = math_imul; break;
      case AsmJSMathBuiltin_clz32: native = math_clz32; break;
      case AsmJSMathBuiltin_fround: native = math_fround; break;
    }

    if (!IsNativeFunction(v, native)) {
        return LinkFail(cx, "bad Math.* builtin function");
    }
=======
    if (!CheckFunction(m)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  return CheckAllFunctionsDefined(m);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool ValidateMathBuiltinFunction(JSContext* cx,
                                        const AsmJSGlobal& global,
                                        HandleValue globalVal) {
  RootedValue v(cx);
  if (!GetDataProperty(cx, globalVal, cx->names().Math, &v)) {
    return false;
  }
||||||| merged common ancestors
static bool
ValidateConstant(JSContext* cx, const AsmJSGlobal& global, HandleValue globalVal)
{
    RootedValue v(cx, globalVal);

    if (global.constantKind() == AsmJSGlobal::MathConstant) {
        if (!GetDataProperty(cx, v, cx->names().Math, &v)) {
            return false;
        }
    }

    if (!GetDataProperty(cx, v, global.field(), &v)) {
        return false;
    }

    if (!v.isNumber()) {
        return LinkFail(cx, "math / global constant value needs to be a number");
    }

    // NaN != NaN
    if (IsNaN(global.constantValue())) {
        if (!IsNaN(v.toNumber())) {
            return LinkFail(cx, "global constant value needs to be NaN");
        }
    } else {
        if (v.toNumber() != global.constantValue()) {
            return LinkFail(cx, "global constant value mismatch");
        }
    }
=======
template <typename Unit>
static bool CheckFuncPtrTable(ModuleValidator<Unit>& m, ParseNode* decl) {
  if (!decl->isKind(ParseNodeKind::AssignExpr)) {
    return m.fail(decl, "function-pointer table must have initializer");
  }
  AssignmentNode* assignNode = &decl->as<AssignmentNode>();

  ParseNode* var = assignNode->left();

  if (!var->isKind(ParseNodeKind::Name)) {
    return m.fail(var, "function-pointer table name is not a plain name");
  }

  ParseNode* arrayLiteral = assignNode->right();

  if (!arrayLiteral->isKind(ParseNodeKind::ArrayExpr)) {
    return m.fail(
        var, "function-pointer table's initializer must be an array literal");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!GetDataProperty(cx, v, global.field(), &v)) {
    return false;
  }

  Native native = nullptr;
  switch (global.mathBuiltinFunction()) {
    case AsmJSMathBuiltin_sin:
      native = math_sin;
      break;
    case AsmJSMathBuiltin_cos:
      native = math_cos;
      break;
    case AsmJSMathBuiltin_tan:
      native = math_tan;
      break;
    case AsmJSMathBuiltin_asin:
      native = math_asin;
      break;
    case AsmJSMathBuiltin_acos:
      native = math_acos;
      break;
    case AsmJSMathBuiltin_atan:
      native = math_atan;
      break;
    case AsmJSMathBuiltin_ceil:
      native = math_ceil;
      break;
    case AsmJSMathBuiltin_floor:
      native = math_floor;
      break;
    case AsmJSMathBuiltin_exp:
      native = math_exp;
      break;
    case AsmJSMathBuiltin_log:
      native = math_log;
      break;
    case AsmJSMathBuiltin_pow:
      native = math_pow;
      break;
    case AsmJSMathBuiltin_sqrt:
      native = math_sqrt;
      break;
    case AsmJSMathBuiltin_min:
      native = math_min;
      break;
    case AsmJSMathBuiltin_max:
      native = math_max;
      break;
    case AsmJSMathBuiltin_abs:
      native = math_abs;
      break;
    case AsmJSMathBuiltin_atan2:
      native = math_atan2;
      break;
    case AsmJSMathBuiltin_imul:
      native = math_imul;
      break;
    case AsmJSMathBuiltin_clz32:
      native = math_clz32;
      break;
    case AsmJSMathBuiltin_fround:
      native = math_fround;
      break;
  }

  if (!IsNativeFunction(v, native)) {
    return LinkFail(cx, "bad Math.* builtin function");
  }

  return true;
}

static bool ValidateConstant(JSContext* cx, const AsmJSGlobal& global,
                             HandleValue globalVal) {
  RootedValue v(cx, globalVal);

  if (global.constantKind() == AsmJSGlobal::MathConstant) {
    if (!GetDataProperty(cx, v, cx->names().Math, &v)) {
      return false;
    }
  }

  if (!GetDataProperty(cx, v, global.field(), &v)) {
    return false;
  }

  if (!v.isNumber()) {
    return LinkFail(cx, "math / global constant value needs to be a number");
  }

  // NaN != NaN
  if (IsNaN(global.constantValue())) {
    if (!IsNaN(v.toNumber())) {
      return LinkFail(cx, "global constant value needs to be NaN");
    }
  } else {
    if (v.toNumber() != global.constantValue()) {
      return LinkFail(cx, "global constant value mismatch");
    }
  }

  return true;
}

static bool CheckBuffer(JSContext* cx, const AsmJSMetadata& metadata,
                        HandleValue bufferVal,
                        MutableHandle<ArrayBufferObjectMaybeShared*> buffer) {
  if (metadata.memoryUsage == MemoryUsage::Shared) {
    if (!IsSharedArrayBuffer(bufferVal)) {
      return LinkFail(
          cx, "shared views can only be constructed onto SharedArrayBuffer");
    }
  } else {
    if (!IsArrayBuffer(bufferVal)) {
      return LinkFail(
          cx, "unshared views can only be constructed onto ArrayBuffer");
    }
  }

  buffer.set(&AsAnyArrayBuffer(bufferVal));
  uint32_t memoryLength = buffer->byteLength();

  if (!IsValidAsmJSHeapLength(memoryLength)) {
    UniqueChars msg(JS_smprintf(
        "ArrayBuffer byteLength 0x%x is not a valid heap length. The next "
        "valid length is 0x%x",
        memoryLength, RoundUpToNextValidAsmJSHeapLength(memoryLength)));
    if (!msg) {
      return false;
    }
    return LinkFail(cx, msg.get());
  }

  // This check is sufficient without considering the size of the loaded datum
  // because heap loads and stores start on an aligned boundary and the heap
  // byteLength has larger alignment.
  MOZ_ASSERT((metadata.minMemoryLength - 1) <= INT32_MAX);
  if (memoryLength < metadata.minMemoryLength) {
    UniqueChars msg(JS_smprintf(
        "ArrayBuffer byteLength of 0x%x is less than 0x%x (the size implied "
        "by const heap accesses).",
        memoryLength, metadata.minMemoryLength));
    if (!msg) {
      return false;
    }
    return LinkFail(cx, msg.get());
  }

  if (buffer->is<ArrayBufferObject>()) {
    Rooted<ArrayBufferObject*> arrayBuffer(cx,
                                           &buffer->as<ArrayBufferObject>());
    if (!ArrayBufferObject::prepareForAsmJS(cx, arrayBuffer)) {
      return LinkFail(cx, "Unable to prepare ArrayBuffer for asm.js use");
    }
  } else {
    return LinkFail(cx, "Unable to prepare SharedArrayBuffer for asm.js use");
  }

  MOZ_ASSERT(buffer->isPreparedForAsmJS());
  return true;
}

static bool GetImports(JSContext* cx, const AsmJSMetadata& metadata,
                       HandleValue globalVal, HandleValue importVal,
                       MutableHandle<FunctionVector> funcImports,
                       MutableHandleValVector valImports) {
  Rooted<FunctionVector> ffis(cx, FunctionVector(cx));
  if (!ffis.resize(metadata.numFFIs)) {
    return false;
  }
||||||| merged common ancestors
    return true;
}
=======
  unsigned length = ListLength(arrayLiteral);
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (const AsmJSGlobal& global : metadata.asmJSGlobals) {
    switch (global.which()) {
      case AsmJSGlobal::Variable: {
        Maybe<LitValPOD> litVal;
        if (!ValidateGlobalVariable(cx, global, importVal, &litVal)) {
          return false;
        }
        if (!valImports.append(Val(litVal->asLitVal()))) {
          return false;
        }
        break;
      }
      case AsmJSGlobal::FFI:
        if (!ValidateFFI(cx, global, importVal, &ffis)) {
          return false;
        }
        break;
      case AsmJSGlobal::ArrayView:
      case AsmJSGlobal::ArrayViewCtor:
        if (!ValidateArrayView(cx, global, globalVal)) {
          return false;
        }
        break;
      case AsmJSGlobal::MathBuiltinFunction:
        if (!ValidateMathBuiltinFunction(cx, global, globalVal)) {
          return false;
        }
        break;
      case AsmJSGlobal::Constant:
        if (!ValidateConstant(cx, global, globalVal)) {
          return false;
        }
        break;
||||||| merged common ancestors
static bool
CheckBuffer(JSContext* cx, const AsmJSMetadata& metadata, HandleValue bufferVal,
            MutableHandle<ArrayBufferObjectMaybeShared*> buffer)
{
    if (metadata.memoryUsage == MemoryUsage::Shared) {
        if (!IsSharedArrayBuffer(bufferVal)) {
            return LinkFail(cx, "shared views can only be constructed onto SharedArrayBuffer");
        }
    } else {
        if (!IsArrayBuffer(bufferVal)) {
            return LinkFail(cx, "unshared views can only be constructed onto ArrayBuffer");
        }
    }

    buffer.set(&AsAnyArrayBuffer(bufferVal));
    uint32_t memoryLength = buffer->byteLength();

    if (!IsValidAsmJSHeapLength(memoryLength)) {
        UniqueChars msg(
            JS_smprintf("ArrayBuffer byteLength 0x%x is not a valid heap length. The next "
                        "valid length is 0x%x",
                        memoryLength,
                        RoundUpToNextValidAsmJSHeapLength(memoryLength)));
        if (!msg) {
            return false;
        }
        return LinkFail(cx, msg.get());
    }

    // This check is sufficient without considering the size of the loaded datum because heap
    // loads and stores start on an aligned boundary and the heap byteLength has larger alignment.
    MOZ_ASSERT((metadata.minMemoryLength - 1) <= INT32_MAX);
    if (memoryLength < metadata.minMemoryLength) {
        UniqueChars msg(
            JS_smprintf("ArrayBuffer byteLength of 0x%x is less than 0x%x (the size implied "
                        "by const heap accesses).",
                        memoryLength,
                        metadata.minMemoryLength));
        if (!msg) {
            return false;
        }
        return LinkFail(cx, msg.get());
    }

    if (buffer->is<ArrayBufferObject>()) {
        Rooted<ArrayBufferObject*> arrayBuffer(cx, &buffer->as<ArrayBufferObject>());
        if (!ArrayBufferObject::prepareForAsmJS(cx, arrayBuffer)) {
            return LinkFail(cx, "Unable to prepare ArrayBuffer for asm.js use");
        }
    } else {
        return LinkFail(cx, "Unable to prepare SharedArrayBuffer for asm.js use");
    }

    MOZ_ASSERT(buffer->isPreparedForAsmJS());
    return true;
}

static bool
GetImports(JSContext* cx, const AsmJSMetadata& metadata, HandleValue globalVal,
           HandleValue importVal, MutableHandle<FunctionVector> funcImports,
           MutableHandleValVector valImports)
{
    Rooted<FunctionVector> ffis(cx, FunctionVector(cx));
    if (!ffis.resize(metadata.numFFIs)) {
        return false;
    }

    for (const AsmJSGlobal& global : metadata.asmJSGlobals) {
        switch (global.which()) {
          case AsmJSGlobal::Variable: {
            Maybe<LitValPOD> litVal;
            if (!ValidateGlobalVariable(cx, global, importVal, &litVal)) {
                return false;
            }
            if (!valImports.append(Val(litVal->asLitVal()))) {
                return false;
            }
            break;
          }
          case AsmJSGlobal::FFI:
            if (!ValidateFFI(cx, global, importVal, &ffis)) {
                return false;
            }
            break;
          case AsmJSGlobal::ArrayView:
          case AsmJSGlobal::ArrayViewCtor:
            if (!ValidateArrayView(cx, global, globalVal)) {
                return false;
            }
            break;
          case AsmJSGlobal::MathBuiltinFunction:
            if (!ValidateMathBuiltinFunction(cx, global, globalVal)) {
                return false;
            }
            break;
          case AsmJSGlobal::Constant:
            if (!ValidateConstant(cx, global, globalVal)) {
                return false;
            }
            break;
        }
=======
  if (!IsPowerOfTwo(length)) {
    return m.failf(arrayLiteral,
                   "function-pointer table length must be a power of 2 (is %u)",
                   length);
  }

  unsigned mask = length - 1;

  Uint32Vector elemFuncDefIndices;
  const FuncType* sig = nullptr;
  for (ParseNode* elem = ListHead(arrayLiteral); elem; elem = NextNode(elem)) {
    if (!elem->isKind(ParseNodeKind::Name)) {
      return m.fail(
          elem, "function-pointer table's elements must be names of functions");
    }

    PropertyName* funcName = elem->as<NameNode>().name();
    const ModuleValidatorShared::Func* func = m.lookupFuncDef(funcName);
    if (!func) {
      return m.fail(
          elem, "function-pointer table's elements must be names of functions");
    }

    const FuncType& funcSig = m.env().types[func->sigIndex()].funcType();
    if (sig) {
      if (*sig != funcSig) {
        return m.fail(elem, "all functions in table must have same signature");
      }
    } else {
      sig = &funcSig;
    }

    if (!elemFuncDefIndices.append(func->funcDefIndex())) {
      return false;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  for (const AsmJSImport& import : metadata.asmJSImports) {
    if (!funcImports.append(ffis[import.ffiIndex()])) {
      return false;
    }
  }
||||||| merged common ancestors
    for (const AsmJSImport& import : metadata.asmJSImports) {
        if (!funcImports.append(ffis[import.ffiIndex()])) {
            return false;
        }
    }
=======
  FuncType copy;
  if (!copy.clone(*sig)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
  uint32_t tableIndex;
  if (!CheckFuncPtrTableAgainstExisting(m, var, var->as<NameNode>().name(),
                                        std::move(copy), mask, &tableIndex)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool TryInstantiate(JSContext* cx, CallArgs args, const Module& module,
                           const AsmJSMetadata& metadata,
                           MutableHandleWasmInstanceObject instanceObj,
                           MutableHandleObject exportObj) {
  HandleValue globalVal = args.get(0);
  HandleValue importVal = args.get(1);
  HandleValue bufferVal = args.get(2);
||||||| merged common ancestors
static bool
TryInstantiate(JSContext* cx, CallArgs args, const Module& module, const AsmJSMetadata& metadata,
               MutableHandleWasmInstanceObject instanceObj, MutableHandleObject exportObj)
{
    HandleValue globalVal = args.get(0);
    HandleValue importVal = args.get(1);
    HandleValue bufferVal = args.get(2);
=======
  if (!m.defineFuncPtrTable(tableIndex, std::move(elemFuncDefIndices))) {
    return m.fail(var, "duplicate function-pointer definition");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Re-check HasCompilerSupport(cx) since this varies per-thread and
  // 'module' may have been produced on a parser thread.
  if (!HasCompilerSupport(cx)) {
    return LinkFail(cx, "no compiler support");
  }
||||||| merged common ancestors
    RootedArrayBufferObjectMaybeShared buffer(cx);
    RootedWasmMemoryObject memory(cx);
    if (module.metadata().usesMemory()) {
        if (!CheckBuffer(cx, metadata, bufferVal, &buffer)) {
            return false;
        }
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedArrayBufferObjectMaybeShared buffer(cx);
  RootedWasmMemoryObject memory(cx);
  if (module.metadata().usesMemory()) {
    if (!CheckBuffer(cx, metadata, bufferVal, &buffer)) {
      return false;
||||||| merged common ancestors
        memory = WasmMemoryObject::create(cx, buffer, nullptr);
        if (!memory) {
            return false;
        }
=======
template <typename Unit>
static bool CheckFuncPtrTables(ModuleValidator<Unit>& m) {
  while (true) {
    ParseNode* varStmt;
    if (!ParseVarOrConstStatement(m.parser(), &varStmt)) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD

    memory = WasmMemoryObject::create(cx, buffer, nullptr);
    if (!memory) {
      return false;
||||||| merged common ancestors

    RootedValVector valImports(cx);
    Rooted<FunctionVector> funcs(cx, FunctionVector(cx));
    if (!GetImports(cx, metadata, globalVal, importVal, &funcs, &valImports)) {
        return false;
=======
    if (!varStmt) {
      break;
    }
    for (ParseNode* var = VarListHead(varStmt); var; var = NextNode(var)) {
      if (!CheckFuncPtrTable(m, var)) {
        return false;
      }
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  RootedValVector valImports(cx);
  Rooted<FunctionVector> funcs(cx, FunctionVector(cx));
  if (!GetImports(cx, metadata, globalVal, importVal, &funcs, &valImports)) {
    return false;
  }
||||||| merged common ancestors
    Rooted<WasmGlobalObjectVector> globalObjs(cx);
=======
  for (unsigned i = 0; i < m.numFuncPtrTables(); i++) {
    ModuleValidatorShared::Table& table = m.table(i);
    if (!table.defined()) {
      return m.failNameOffset(table.firstUse(),
                              "function-pointer table %s wasn't defined",
                              table.name());
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Rooted<WasmGlobalObjectVector> globalObjs(cx);
  Rooted<WasmTableObjectVector> tables(cx);
  if (!module.instantiate(cx, funcs, tables.get(), memory, valImports,
                          globalObjs.get(), nullptr, instanceObj))
    return false;
||||||| merged common ancestors
    RootedWasmTableObject table(cx);
    if (!module.instantiate(cx, funcs, table, memory, valImports, globalObjs.get(), nullptr,
                            instanceObj))
        return false;
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  exportObj.set(&instanceObj->exportsObj());
  return true;
||||||| merged common ancestors
    exportObj.set(&instanceObj->exportsObj());
    return true;
=======
static bool CheckModuleExportFunction(ModuleValidatorShared& m, ParseNode* pn,
                                      PropertyName* maybeFieldName = nullptr) {
  if (!pn->isKind(ParseNodeKind::Name)) {
    return m.fail(pn, "expected name of exported function");
  }

  PropertyName* funcName = pn->as<NameNode>().name();
  const ModuleValidatorShared::Func* func = m.lookupFuncDef(funcName);
  if (!func) {
    return m.failName(pn, "function '%s' not found", funcName);
  }

  return m.addExportField(*func, maybeFieldName);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool HandleInstantiationFailure(JSContext* cx, CallArgs args,
                                       const AsmJSMetadata& metadata) {
  RootedAtom name(cx, args.callee().as<JSFunction>().explicitName());
||||||| merged common ancestors
static bool
HandleInstantiationFailure(JSContext* cx, CallArgs args, const AsmJSMetadata& metadata)
{
    RootedAtom name(cx, args.callee().as<JSFunction>().explicitName());
=======
static bool CheckModuleExportObject(ModuleValidatorShared& m,
                                    ParseNode* object) {
  MOZ_ASSERT(object->isKind(ParseNodeKind::ObjectExpr));
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (cx->isExceptionPending()) {
    return false;
  }
||||||| merged common ancestors
    if (cx->isExceptionPending()) {
        return false;
    }

    ScriptSource* source = metadata.scriptSource.get();
=======
  for (ParseNode* pn = ListHead(object); pn; pn = NextNode(pn)) {
    if (!IsNormalObjectField(pn)) {
      return m.fail(pn,
                    "only normal object properties may be used in the export "
                    "object literal");
    }

    PropertyName* fieldName = ObjectNormalFieldName(pn);
>>>>>>> upstream-releases

<<<<<<< HEAD
  ScriptSource* source = metadata.scriptSource.get();
||||||| merged common ancestors
    // Source discarding is allowed to affect JS semantics because it is never
    // enabled for normal JS content.
    bool haveSource = source->hasSourceText();
    if (!haveSource && !JSScript::loadSource(cx, source, &haveSource)) {
        return false;
    }
    if (!haveSource) {
        JS_ReportErrorASCII(cx, "asm.js link failure with source discarding enabled");
        return false;
    }
=======
    ParseNode* initNode = ObjectNormalFieldInitializer(pn);
    if (!initNode->isKind(ParseNodeKind::Name)) {
      return m.fail(
          initNode,
          "initializer of exported object literal must be name of function");
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Source discarding is allowed to affect JS semantics because it is never
  // enabled for normal JS content.
  bool haveSource = source->hasSourceText();
  if (!haveSource && !JSScript::loadSource(cx, source, &haveSource)) {
    return false;
  }
  if (!haveSource) {
    JS_ReportErrorASCII(cx,
                        "asm.js link failure with source discarding enabled");
    return false;
  }
||||||| merged common ancestors
    uint32_t begin = metadata.toStringStart;
    uint32_t end = metadata.srcEndAfterCurly();
    Rooted<JSFlatString*> src(cx, source->substringDontDeflate(cx, begin, end));
    if (!src) {
        return false;
    }
=======
    if (!CheckModuleExportFunction(m, initNode, fieldName)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t begin = metadata.toStringStart;
  uint32_t end = metadata.srcEndAfterCurly();
  Rooted<JSFlatString*> src(cx, source->substringDontDeflate(cx, begin, end));
  if (!src) {
    return false;
  }
||||||| merged common ancestors
    RootedFunction fun(cx, NewScriptedFunction(cx, 0, JSFunction::INTERPRETED_NORMAL,
                                               name, /* proto = */ nullptr, gc::AllocKind::FUNCTION,
                                               TenuredObject));
    if (!fun) {
        return false;
    }
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedFunction fun(
      cx, NewScriptedFunction(cx, 0, JSFunction::INTERPRETED_NORMAL, name,
                              /* proto = */ nullptr, gc::AllocKind::FUNCTION,
                              TenuredObject));
  if (!fun) {
    return false;
  }

  JS::CompileOptions options(cx);
  options.setMutedErrors(source->mutedErrors())
      .setFile(source->filename())
      .setNoScriptRval(false);
  options.asmJSOption = AsmJSOption::Disabled;

  // The exported function inherits an implicit strict context if the module
  // also inherited it somehow.
  if (metadata.strict) {
    options.strictOption = true;
  }

  AutoStableStringChars stableChars(cx);
  if (!stableChars.initTwoByte(cx, src)) {
    return false;
  }
||||||| merged common ancestors
    JS::CompileOptions options(cx);
    options.setMutedErrors(source->mutedErrors())
           .setFile(source->filename())
           .setNoScriptRval(false);
    options.asmJSOption = AsmJSOption::Disabled;
=======
template <typename Unit>
static bool CheckModuleReturn(ModuleValidator<Unit>& m) {
  TokenKind tk;
  if (!GetToken(m.parser(), &tk)) {
    return false;
  }
  auto& ts = m.parser().tokenStream;
  if (tk != TokenKind::Return) {
    return m.failCurrentOffset(
        (tk == TokenKind::RightCurly || tk == TokenKind::Eof)
            ? "expecting return statement"
            : "invalid asm.js. statement");
  }
  ts.anyCharsAccess().ungetToken();

  ParseNode* returnStmt = m.parser().statementListItem(YieldIsName);
  if (!returnStmt) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  SourceText<char16_t> srcBuf;
||||||| merged common ancestors
    // The exported function inherits an implicit strict context if the module
    // also inherited it somehow.
    if (metadata.strict) {
        options.strictOption = true;
    }
=======
  ParseNode* returnExpr = ReturnExpr(returnStmt);
  if (!returnExpr) {
    return m.fail(returnStmt, "export statement must return something");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const char16_t* chars = stableChars.twoByteRange().begin().get();
  SourceOwnership ownership = stableChars.maybeGiveOwnershipToCaller()
                                  ? SourceOwnership::TakeOwnership
                                  : SourceOwnership::Borrowed;
  if (!srcBuf.init(cx, chars, end - begin, ownership)) {
    return false;
  }

  if (!frontend::CompileStandaloneFunction(cx, &fun, options, srcBuf,
                                           Nothing())) {
    return false;
  }
||||||| merged common ancestors
    AutoStableStringChars stableChars(cx);
    if (!stableChars.initTwoByte(cx, src)) {
        return false;
    }

    const char16_t* chars = stableChars.twoByteRange().begin().get();
    SourceBufferHolder::Ownership ownership = stableChars.maybeGiveOwnershipToCaller()
                                              ? SourceBufferHolder::GiveOwnership
                                              : SourceBufferHolder::NoOwnership;
    SourceBufferHolder srcBuf(chars, end - begin, ownership);
    if (!frontend::CompileStandaloneFunction(cx, &fun, options, srcBuf, Nothing())) {
        return false;
    }
=======
  if (returnExpr->isKind(ParseNodeKind::ObjectExpr)) {
    if (!CheckModuleExportObject(m, returnExpr)) {
      return false;
    }
  } else {
    if (!CheckModuleExportFunction(m, returnExpr)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Call the function we just recompiled.
  args.setCallee(ObjectValue(*fun));
  return InternalCallOrConstruct(
      cx, args, args.isConstructing() ? CONSTRUCT : NO_CONSTRUCT);
||||||| merged common ancestors
    // Call the function we just recompiled.
    args.setCallee(ObjectValue(*fun));
    return InternalCallOrConstruct(cx, args, args.isConstructing() ? CONSTRUCT : NO_CONSTRUCT);
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static const Module& AsmJSModuleFunctionToModule(JSFunction* fun) {
  MOZ_ASSERT(IsAsmJSModule(fun));
  const Value& v = fun->getExtendedSlot(FunctionExtended::ASMJS_MODULE_SLOT);
  return v.toObject().as<WasmModuleObject>().module();
||||||| merged common ancestors
static const Module&
AsmJSModuleFunctionToModule(JSFunction* fun)
{
    MOZ_ASSERT(IsAsmJSModule(fun));
    const Value& v = fun->getExtendedSlot(FunctionExtended::ASMJS_MODULE_SLOT);
    return v.toObject().as<WasmModuleObject>().module();
=======
template <typename Unit>
static bool CheckModuleEnd(ModuleValidator<Unit>& m) {
  TokenKind tk;
  if (!GetToken(m.parser(), &tk)) {
    return false;
  }

  if (tk != TokenKind::Eof && tk != TokenKind::RightCurly) {
    return m.failCurrentOffset(
        "top-level export (return) must be the last statement");
  }

  m.parser().tokenStream.anyCharsAccess().ungetToken();
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Implements the semantics of an asm.js module function that has been
// successfully validated.
bool js::InstantiateAsmJS(JSContext* cx, unsigned argc, JS::Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
||||||| merged common ancestors
// Implements the semantics of an asm.js module function that has been successfully validated.
bool
js::InstantiateAsmJS(JSContext* cx, unsigned argc, JS::Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
=======
template <typename Unit>
static SharedModule CheckModule(JSContext* cx, AsmJSParser<Unit>& parser,
                                ParseNode* stmtList, unsigned* time) {
  int64_t before = PRMJ_Now();

  FunctionNode* moduleFunctionNode = parser.pc_->functionBox()->functionNode;

  ModuleValidator<Unit> m(cx, parser, moduleFunctionNode);
  if (!m.init()) {
    return nullptr;
  }

  if (!CheckFunctionHead(m, moduleFunctionNode)) {
    return nullptr;
  }

  if (!CheckModuleArguments(m, moduleFunctionNode)) {
    return nullptr;
  }

  if (!CheckPrecedingStatements(m, stmtList)) {
    return nullptr;
  }

  if (!CheckModuleProcessingDirectives(m)) {
    return nullptr;
  }

  if (!CheckModuleGlobals(m)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSFunction* callee = &args.callee().as<JSFunction>();
  const Module& module = AsmJSModuleFunctionToModule(callee);
  const AsmJSMetadata& metadata = module.metadata().asAsmJS();
||||||| merged common ancestors
    JSFunction* callee = &args.callee().as<JSFunction>();
    const Module& module = AsmJSModuleFunctionToModule(callee);
    const AsmJSMetadata& metadata = module.metadata().asAsmJS();
=======
  if (!m.startFunctionBodies()) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedWasmInstanceObject instanceObj(cx);
  RootedObject exportObj(cx);
  if (!TryInstantiate(cx, args, module, metadata, &instanceObj, &exportObj)) {
    // Link-time validation checks failed, so reparse the entire asm.js
    // module from scratch to get normal interpreted bytecode which we can
    // simply Invoke. Very slow.
    return HandleInstantiationFailure(cx, args, metadata);
  }
||||||| merged common ancestors
    RootedWasmInstanceObject instanceObj(cx);
    RootedObject exportObj(cx);
    if (!TryInstantiate(cx, args, module, metadata, &instanceObj, &exportObj)) {
        // Link-time validation checks failed, so reparse the entire asm.js
        // module from scratch to get normal interpreted bytecode which we can
        // simply Invoke. Very slow.
        return HandleInstantiationFailure(cx, args, metadata);
    }
=======
  if (!CheckFunctions(m)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  args.rval().set(ObjectValue(*exportObj));
  return true;
}
||||||| merged common ancestors
    args.rval().set(ObjectValue(*exportObj));
    return true;
}
=======
  if (!CheckFuncPtrTables(m)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static JSFunction* NewAsmJSModuleFunction(JSContext* cx, JSFunction* origFun,
                                          HandleObject moduleObj) {
  RootedAtom name(cx, origFun->explicitName());
||||||| merged common ancestors
static JSFunction*
NewAsmJSModuleFunction(JSContext* cx, JSFunction* origFun, HandleObject moduleObj)
{
    RootedAtom name(cx, origFun->explicitName());
=======
  if (!CheckModuleReturn(m)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSFunction::Flags flags = origFun->isLambda() ? JSFunction::ASMJS_LAMBDA_CTOR
                                                : JSFunction::ASMJS_CTOR;
  JSFunction* moduleFun = NewNativeConstructor(
      cx, InstantiateAsmJS, origFun->nargs(), name,
      gc::AllocKind::FUNCTION_EXTENDED, TenuredObject, flags);
  if (!moduleFun) {
    return nullptr;
  }
||||||| merged common ancestors
    JSFunction::Flags flags = origFun->isLambda() ? JSFunction::ASMJS_LAMBDA_CTOR
                                                  : JSFunction::ASMJS_CTOR;
    JSFunction* moduleFun =
        NewNativeConstructor(cx, InstantiateAsmJS, origFun->nargs(), name,
                             gc::AllocKind::FUNCTION_EXTENDED, TenuredObject,
                             flags);
    if (!moduleFun) {
        return nullptr;
    }
=======
  if (!CheckModuleEnd(m)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  moduleFun->setExtendedSlot(FunctionExtended::ASMJS_MODULE_SLOT,
                             ObjectValue(*moduleObj));
||||||| merged common ancestors
    moduleFun->setExtendedSlot(FunctionExtended::ASMJS_MODULE_SLOT, ObjectValue(*moduleObj));
=======
  SharedModule module = m.finish();
  if (!module) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(IsAsmJSModule(moduleFun));
  return moduleFun;
||||||| merged common ancestors
    MOZ_ASSERT(IsAsmJSModule(moduleFun));
    return moduleFun;
=======
  *time = (PRMJ_Now() - before) / PRMJ_USEC_PER_MSEC;
  return module;
>>>>>>> upstream-releases
}

/*****************************************************************************/
<<<<<<< HEAD
// Caching and cloning

size_t AsmJSGlobal::serializedSize() const {
  return sizeof(pod) + field_.serializedSize();
}

uint8_t* AsmJSGlobal::serialize(uint8_t* cursor) const {
  cursor = WriteBytes(cursor, &pod, sizeof(pod));
  cursor = field_.serialize(cursor);
  return cursor;
}

const uint8_t* AsmJSGlobal::deserialize(const uint8_t* cursor) {
  (cursor = ReadBytes(cursor, &pod, sizeof(pod))) &&
      (cursor = field_.deserialize(cursor));
  return cursor;
}

size_t AsmJSGlobal::sizeOfExcludingThis(MallocSizeOf mallocSizeOf) const {
  return field_.sizeOfExcludingThis(mallocSizeOf);
}

size_t AsmJSMetadata::serializedSize() const {
  return Metadata::serializedSize() + sizeof(pod()) +
         SerializedVectorSize(asmJSGlobals) +
         SerializedPodVectorSize(asmJSImports) +
         SerializedPodVectorSize(asmJSExports) +
         SerializedVectorSize(asmJSFuncNames) +
         globalArgumentName.serializedSize() +
         importArgumentName.serializedSize() +
         bufferArgumentName.serializedSize();
}

uint8_t* AsmJSMetadata::serialize(uint8_t* cursor) const {
  cursor = Metadata::serialize(cursor);
  cursor = WriteBytes(cursor, &pod(), sizeof(pod()));
  cursor = SerializeVector(cursor, asmJSGlobals);
  cursor = SerializePodVector(cursor, asmJSImports);
  cursor = SerializePodVector(cursor, asmJSExports);
  cursor = SerializeVector(cursor, asmJSFuncNames);
  cursor = globalArgumentName.serialize(cursor);
  cursor = importArgumentName.serialize(cursor);
  cursor = bufferArgumentName.serialize(cursor);
  return cursor;
}

const uint8_t* AsmJSMetadata::deserialize(const uint8_t* cursor) {
  (cursor = Metadata::deserialize(cursor)) &&
      (cursor = ReadBytes(cursor, &pod(), sizeof(pod()))) &&
      (cursor = DeserializeVector(cursor, &asmJSGlobals)) &&
      (cursor = DeserializePodVector(cursor, &asmJSImports)) &&
      (cursor = DeserializePodVector(cursor, &asmJSExports)) &&
      (cursor = DeserializeVector(cursor, &asmJSFuncNames)) &&
      (cursor = globalArgumentName.deserialize(cursor)) &&
      (cursor = importArgumentName.deserialize(cursor)) &&
      (cursor = bufferArgumentName.deserialize(cursor));
  cacheResult = CacheResult::Hit;
  return cursor;
}

size_t AsmJSMetadata::sizeOfExcludingThis(MallocSizeOf mallocSizeOf) const {
  return Metadata::sizeOfExcludingThis(mallocSizeOf) +
         SizeOfVectorExcludingThis(asmJSGlobals, mallocSizeOf) +
         asmJSImports.sizeOfExcludingThis(mallocSizeOf) +
         asmJSExports.sizeOfExcludingThis(mallocSizeOf) +
         SizeOfVectorExcludingThis(asmJSFuncNames, mallocSizeOf) +
         globalArgumentName.sizeOfExcludingThis(mallocSizeOf) +
         importArgumentName.sizeOfExcludingThis(mallocSizeOf) +
         bufferArgumentName.sizeOfExcludingThis(mallocSizeOf);
}

namespace {

// Assumptions captures ambient state that must be the same when compiling and
// deserializing a module for the compiled code to be valid. If it's not, then
// the module must be recompiled from scratch.
||||||| merged common ancestors
// Caching and cloning

size_t
AsmJSGlobal::serializedSize() const
{
    return sizeof(pod) +
           field_.serializedSize();
}

uint8_t*
AsmJSGlobal::serialize(uint8_t* cursor) const
{
    cursor = WriteBytes(cursor, &pod, sizeof(pod));
    cursor = field_.serialize(cursor);
    return cursor;
}

const uint8_t*
AsmJSGlobal::deserialize(const uint8_t* cursor)
{
    (cursor = ReadBytes(cursor, &pod, sizeof(pod))) &&
    (cursor = field_.deserialize(cursor));
    return cursor;
}

size_t
AsmJSGlobal::sizeOfExcludingThis(MallocSizeOf mallocSizeOf) const
{
    return field_.sizeOfExcludingThis(mallocSizeOf);
}

size_t
AsmJSMetadata::serializedSize() const
{
    return Metadata::serializedSize() +
           sizeof(pod()) +
           SerializedVectorSize(asmJSGlobals) +
           SerializedPodVectorSize(asmJSImports) +
           SerializedPodVectorSize(asmJSExports) +
           SerializedVectorSize(asmJSFuncNames) +
           globalArgumentName.serializedSize() +
           importArgumentName.serializedSize() +
           bufferArgumentName.serializedSize();
}

uint8_t*
AsmJSMetadata::serialize(uint8_t* cursor) const
{
    cursor = Metadata::serialize(cursor);
    cursor = WriteBytes(cursor, &pod(), sizeof(pod()));
    cursor = SerializeVector(cursor, asmJSGlobals);
    cursor = SerializePodVector(cursor, asmJSImports);
    cursor = SerializePodVector(cursor, asmJSExports);
    cursor = SerializeVector(cursor, asmJSFuncNames);
    cursor = globalArgumentName.serialize(cursor);
    cursor = importArgumentName.serialize(cursor);
    cursor = bufferArgumentName.serialize(cursor);
    return cursor;
}

const uint8_t*
AsmJSMetadata::deserialize(const uint8_t* cursor)
{
    (cursor = Metadata::deserialize(cursor)) &&
    (cursor = ReadBytes(cursor, &pod(), sizeof(pod()))) &&
    (cursor = DeserializeVector(cursor, &asmJSGlobals)) &&
    (cursor = DeserializePodVector(cursor, &asmJSImports)) &&
    (cursor = DeserializePodVector(cursor, &asmJSExports)) &&
    (cursor = DeserializeVector(cursor, &asmJSFuncNames)) &&
    (cursor = globalArgumentName.deserialize(cursor)) &&
    (cursor = importArgumentName.deserialize(cursor)) &&
    (cursor = bufferArgumentName.deserialize(cursor));
    cacheResult = CacheResult::Hit;
    return cursor;
}

size_t
AsmJSMetadata::sizeOfExcludingThis(MallocSizeOf mallocSizeOf) const
{
    return Metadata::sizeOfExcludingThis(mallocSizeOf) +
           SizeOfVectorExcludingThis(asmJSGlobals, mallocSizeOf) +
           asmJSImports.sizeOfExcludingThis(mallocSizeOf) +
           asmJSExports.sizeOfExcludingThis(mallocSizeOf) +
           SizeOfVectorExcludingThis(asmJSFuncNames, mallocSizeOf) +
           globalArgumentName.sizeOfExcludingThis(mallocSizeOf) +
           importArgumentName.sizeOfExcludingThis(mallocSizeOf) +
           bufferArgumentName.sizeOfExcludingThis(mallocSizeOf);
}

namespace {

// Assumptions captures ambient state that must be the same when compiling and
// deserializing a module for the compiled code to be valid. If it's not, then
// the module must be recompiled from scratch.
=======
// Link-time validation
>>>>>>> upstream-releases

<<<<<<< HEAD
struct Assumptions {
  uint32_t cpuId;
  JS::BuildIdCharVector buildId;
||||||| merged common ancestors
struct Assumptions
{
    uint32_t              cpuId;
    JS::BuildIdCharVector buildId;
=======
static bool LinkFail(JSContext* cx, const char* str) {
  JS_ReportErrorFlagsAndNumberASCII(cx, JSREPORT_WARNING, GetErrorMessage,
                                    nullptr, JSMSG_USE_ASM_LINK_FAIL, str);
  return false;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Assumptions();
  bool init();
||||||| merged common ancestors
    Assumptions();
    bool init();
=======
static bool IsMaybeWrappedScriptedProxy(JSObject* obj) {
  JSObject* unwrapped = UncheckedUnwrap(obj);
  return unwrapped && IsScriptedProxy(unwrapped);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool operator==(const Assumptions& rhs) const;
  bool operator!=(const Assumptions& rhs) const { return !(*this == rhs); }
||||||| merged common ancestors
    bool operator==(const Assumptions& rhs) const;
    bool operator!=(const Assumptions& rhs) const { return !(*this == rhs); }
=======
static bool GetDataProperty(JSContext* cx, HandleValue objVal, HandleAtom field,
                            MutableHandleValue v) {
  if (!objVal.isObject()) {
    return LinkFail(cx, "accessing property of non-object");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t serializedSize() const;
  uint8_t* serialize(uint8_t* cursor) const;
  const uint8_t* deserialize(const uint8_t* cursor, size_t remain);
};
||||||| merged common ancestors
    size_t serializedSize() const;
    uint8_t* serialize(uint8_t* cursor) const;
    const uint8_t* deserialize(const uint8_t* cursor, size_t remain);
};
=======
  RootedObject obj(cx, &objVal.toObject());
  if (IsMaybeWrappedScriptedProxy(obj)) {
    return LinkFail(cx, "accessing property of a Proxy");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
Assumptions::Assumptions() : cpuId(ObservedCPUFeatures()), buildId() {}
||||||| merged common ancestors
Assumptions::Assumptions()
  : cpuId(ObservedCPUFeatures()),
    buildId()
{}
=======
  Rooted<PropertyDescriptor> desc(cx);
  RootedId id(cx, AtomToId(field));
  if (!GetPropertyDescriptor(cx, obj, id, &desc)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool Assumptions::init() { return GetBuildId && GetBuildId(&buildId); }
||||||| merged common ancestors
bool
Assumptions::init()
{
    return GetBuildId && GetBuildId(&buildId);
}
=======
  if (!desc.object()) {
    return LinkFail(cx, "property not present on object");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool Assumptions::operator==(const Assumptions& rhs) const {
  return cpuId == rhs.cpuId && buildId.length() == rhs.buildId.length() &&
         ArrayEqual(buildId.begin(), rhs.buildId.begin(), buildId.length());
}
||||||| merged common ancestors
bool
Assumptions::operator==(const Assumptions& rhs) const
{
    return cpuId == rhs.cpuId &&
           buildId.length() == rhs.buildId.length() &&
           ArrayEqual(buildId.begin(), rhs.buildId.begin(), buildId.length());
}
=======
  if (!desc.isDataDescriptor()) {
    return LinkFail(cx, "property is not a data property");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
size_t Assumptions::serializedSize() const {
  return sizeof(uint32_t) + SerializedPodVectorSize(buildId);
||||||| merged common ancestors
size_t
Assumptions::serializedSize() const
{
    return sizeof(uint32_t) +
           SerializedPodVectorSize(buildId);
=======
  v.set(desc.value());
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
uint8_t* Assumptions::serialize(uint8_t* cursor) const {
  // The format of serialized Assumptions must never change in a way that
  // would cause old cache files written with by an old build-id to match the
  // assumptions of a different build-id.
||||||| merged common ancestors
uint8_t*
Assumptions::serialize(uint8_t* cursor) const
{
    // The format of serialized Assumptions must never change in a way that
    // would cause old cache files written with by an old build-id to match the
    // assumptions of a different build-id.
=======
static bool GetDataProperty(JSContext* cx, HandleValue objVal,
                            const char* fieldChars, MutableHandleValue v) {
  RootedAtom field(cx, AtomizeUTF8Chars(cx, fieldChars, strlen(fieldChars)));
  if (!field) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  cursor = WriteScalar<uint32_t>(cursor, cpuId);
  cursor = SerializePodVector(cursor, buildId);
  return cursor;
||||||| merged common ancestors
    cursor = WriteScalar<uint32_t>(cursor, cpuId);
    cursor = SerializePodVector(cursor, buildId);
    return cursor;
=======
  return GetDataProperty(cx, objVal, field, v);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
const uint8_t* Assumptions::deserialize(const uint8_t* cursor, size_t remain) {
  (cursor = ReadScalarChecked<uint32_t>(cursor, &remain, &cpuId)) &&
      (cursor = DeserializePodVectorChecked(cursor, &remain, &buildId));
  return cursor;
||||||| merged common ancestors
const uint8_t*
Assumptions::deserialize(const uint8_t* cursor, size_t remain)
{
    (cursor = ReadScalarChecked<uint32_t>(cursor, &remain, &cpuId)) &&
    (cursor = DeserializePodVectorChecked(cursor, &remain, &buildId));
    return cursor;
=======
static bool GetDataProperty(JSContext* cx, HandleValue objVal,
                            ImmutablePropertyNamePtr field,
                            MutableHandleValue v) {
  // Help the conversion along for all the cx->names().* users.
  HandlePropertyName fieldHandle = field;
  return GetDataProperty(cx, objVal, fieldHandle, v);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
class ModuleChars {
 protected:
  uint32_t isFunCtor_;
  Vector<CacheableChars, 0, SystemAllocPolicy> funCtorArgs_;
||||||| merged common ancestors
class ModuleChars
{
  protected:
    uint32_t isFunCtor_;
    Vector<CacheableChars, 0, SystemAllocPolicy> funCtorArgs_;
=======
static bool HasObjectValueOfMethodPure(JSObject* obj, JSContext* cx) {
  Value v;
  if (!GetPropertyPure(cx, obj, NameToId(cx->names().valueOf), &v)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  static uint32_t beginOffset(AsmJSParser& parser) {
    return parser.pc->functionBox()->functionNode->pn_pos.begin;
  }
||||||| merged common ancestors
  public:
    static uint32_t beginOffset(AsmJSParser& parser) {
        return parser.pc->functionBox()->functionNode->pn_pos.begin;
    }
=======
  JSFunction* fun;
  if (!IsFunctionObject(v, &fun)) {
    return false;
  }

  return IsSelfHostedFunctionWithName(fun, cx->names().Object_valueOf);
}

static bool HasPureCoercion(JSContext* cx, HandleValue v) {
  // Ideally, we'd reject all non-primitives, but Emscripten has a bug that
  // generates code that passes functions for some imports. To avoid breaking
  // all the code that contains this bug, we make an exception for functions
  // that don't have user-defined valueOf or toString, for their coercions
  // are not observable and coercion via ToNumber/ToInt32 definitely produces
  // NaN/0. We should remove this special case later once most apps have been
  // built with newer Emscripten.
  if (v.toObject().is<JSFunction>() &&
      HasNoToPrimitiveMethodPure(&v.toObject(), cx) &&
      HasObjectValueOfMethodPure(&v.toObject(), cx) &&
      HasNativeMethodPure(&v.toObject(), cx->names().toString, fun_toString,
                          cx)) {
    return true;
  }
  return false;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  static uint32_t endOffset(AsmJSParser& parser) {
    TokenPos pos(0, 0);  // initialize to silence GCC warning
    MOZ_ALWAYS_TRUE(
        parser.tokenStream.peekTokenPos(&pos, TokenStreamShared::Operand));
    return pos.end;
  }
};
||||||| merged common ancestors
    static uint32_t endOffset(AsmJSParser& parser) {
        TokenPos pos(0, 0);  // initialize to silence GCC warning
        MOZ_ALWAYS_TRUE(parser.tokenStream.peekTokenPos(&pos, TokenStreamShared::Operand));
        return pos.end;
    }
};
=======
static bool ValidateGlobalVariable(JSContext* cx, const AsmJSGlobal& global,
                                   HandleValue importVal,
                                   Maybe<LitValPOD>* val) {
  switch (global.varInitKind()) {
    case AsmJSGlobal::InitConstant:
      val->emplace(global.varInitVal());
      return true;
>>>>>>> upstream-releases

<<<<<<< HEAD
class ModuleCharsForStore : ModuleChars {
  uint32_t uncompressedSize_;
  uint32_t compressedSize_;
  Vector<char, 0, SystemAllocPolicy> compressedBuffer_;
||||||| merged common ancestors
class ModuleCharsForStore : ModuleChars
{
    uint32_t uncompressedSize_;
    uint32_t compressedSize_;
    Vector<char, 0, SystemAllocPolicy> compressedBuffer_;
=======
    case AsmJSGlobal::InitImport: {
      RootedValue v(cx);
      if (!GetDataProperty(cx, importVal, global.field(), &v)) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  bool init(AsmJSParser& parser) {
    MOZ_ASSERT(beginOffset(parser) < endOffset(parser));
||||||| merged common ancestors
  public:
    bool init(AsmJSParser& parser) {
        MOZ_ASSERT(beginOffset(parser) < endOffset(parser));
=======
      if (!v.isPrimitive() && !HasPureCoercion(cx, v)) {
        return LinkFail(cx, "Imported values must be primitives");
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    uncompressedSize_ =
        (endOffset(parser) - beginOffset(parser)) * sizeof(char16_t);
    size_t maxCompressedSize = LZ4::maxCompressedSize(uncompressedSize_);
    if (maxCompressedSize < uncompressedSize_) {
      return false;
    }

    if (!compressedBuffer_.resize(maxCompressedSize)) {
      return false;
    }

    const char16_t* chars =
        parser.tokenStream.codeUnitPtrAt(beginOffset(parser));
    const char* source = reinterpret_cast<const char*>(chars);
    size_t compressedSize =
        LZ4::compress(source, uncompressedSize_, compressedBuffer_.begin());
    if (!compressedSize || compressedSize > UINT32_MAX) {
      return false;
    }

    compressedSize_ = compressedSize;

    // For a function statement or named function expression:
    //   function f(x,y,z) { abc }
    // the range [beginOffset, endOffset) captures the source:
    //   f(x,y,z) { abc }
    // An unnamed function expression captures the same thing, sans 'f'.
    // Since asm.js modules do not contain any free variables, equality of
    // [beginOffset, endOffset) is sufficient to guarantee identical code
    // generation, modulo Assumptions.
    //
    // For functions created with 'new Function', function arguments are
    // not present in the source so we must manually explicitly serialize
    // and match the formals as a Vector of PropertyName.
    isFunCtor_ = parser.pc->isStandaloneFunctionBody();
    if (isFunCtor_) {
      unsigned numArgs;
      CodeNode* functionNode = parser.pc->functionBox()->functionNode;
      ParseNode* arg = FunctionFormalParametersList(functionNode, &numArgs);
      for (unsigned i = 0; i < numArgs; i++, arg = arg->pn_next) {
        UniqueChars name =
            StringToNewUTF8CharsZ(nullptr, *arg->as<NameNode>().name());
        if (!name || !funCtorArgs_.append(std::move(name))) {
          return false;
||||||| merged common ancestors
        uncompressedSize_ = (endOffset(parser) - beginOffset(parser)) * sizeof(char16_t);
        size_t maxCompressedSize = LZ4::maxCompressedSize(uncompressedSize_);
        if (maxCompressedSize < uncompressedSize_) {
            return false;
        }

        if (!compressedBuffer_.resize(maxCompressedSize)) {
            return false;
        }

        const char16_t* chars = parser.tokenStream.codeUnitPtrAt(beginOffset(parser));
        const char* source = reinterpret_cast<const char*>(chars);
        size_t compressedSize = LZ4::compress(source, uncompressedSize_, compressedBuffer_.begin());
        if (!compressedSize || compressedSize > UINT32_MAX) {
            return false;
        }

        compressedSize_ = compressedSize;

        // For a function statement or named function expression:
        //   function f(x,y,z) { abc }
        // the range [beginOffset, endOffset) captures the source:
        //   f(x,y,z) { abc }
        // An unnamed function expression captures the same thing, sans 'f'.
        // Since asm.js modules do not contain any free variables, equality of
        // [beginOffset, endOffset) is sufficient to guarantee identical code
        // generation, modulo Assumptions.
        //
        // For functions created with 'new Function', function arguments are
        // not present in the source so we must manually explicitly serialize
        // and match the formals as a Vector of PropertyName.
        isFunCtor_ = parser.pc->isStandaloneFunctionBody();
        if (isFunCtor_) {
            unsigned numArgs;
            CodeNode* functionNode = parser.pc->functionBox()->functionNode;
            ParseNode* arg = FunctionFormalParametersList(functionNode, &numArgs);
            for (unsigned i = 0; i < numArgs; i++, arg = arg->pn_next) {
                UniqueChars name = StringToNewUTF8CharsZ(nullptr, *arg->as<NameNode>().name());
                if (!name || !funCtorArgs_.append(std::move(name))) {
                    return false;
                }
            }
=======
      switch (global.varInitImportType().code()) {
        case ValType::I32: {
          int32_t i32;
          if (!ToInt32(cx, v, &i32)) {
            return false;
          }
          val->emplace(uint32_t(i32));
          return true;
        }
        case ValType::I64:
          MOZ_CRASH("int64");
        case ValType::F32: {
          float f;
          if (!RoundFloat32(cx, v, &f)) {
            return false;
          }
          val->emplace(f);
          return true;
        }
        case ValType::F64: {
          double d;
          if (!ToNumber(cx, v, &d)) {
            return false;
          }
          val->emplace(d);
          return true;
        }
        case ValType::Ref:
        case ValType::NullRef:
        case ValType::AnyRef:
        case ValType::FuncRef: {
          MOZ_CRASH("not available in asm.js");
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
      }
    }
||||||| merged common ancestors

        return true;
    }

    size_t serializedSize() const {
        return sizeof(uint32_t) +
               sizeof(uint32_t) +
               compressedSize_ +
               sizeof(uint32_t) +
               (isFunCtor_ ? SerializedVectorSize(funCtorArgs_) : 0);
    }
=======
      }
    }
  }

  MOZ_CRASH("unreachable");
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    return true;
  }

  size_t serializedSize() const {
    return sizeof(uint32_t) + sizeof(uint32_t) + compressedSize_ +
           sizeof(uint32_t) +
           (isFunCtor_ ? SerializedVectorSize(funCtorArgs_) : 0);
  }

  uint8_t* serialize(uint8_t* cursor) const {
    cursor = WriteScalar<uint32_t>(cursor, uncompressedSize_);
    cursor = WriteScalar<uint32_t>(cursor, compressedSize_);
    cursor = WriteBytes(cursor, compressedBuffer_.begin(), compressedSize_);
    cursor = WriteScalar<uint32_t>(cursor, isFunCtor_);
    if (isFunCtor_) {
      cursor = SerializeVector(cursor, funCtorArgs_);
    }
    return cursor;
  }
};
||||||| merged common ancestors
    uint8_t* serialize(uint8_t* cursor) const {
        cursor = WriteScalar<uint32_t>(cursor, uncompressedSize_);
        cursor = WriteScalar<uint32_t>(cursor, compressedSize_);
        cursor = WriteBytes(cursor, compressedBuffer_.begin(), compressedSize_);
        cursor = WriteScalar<uint32_t>(cursor, isFunCtor_);
        if (isFunCtor_) {
            cursor = SerializeVector(cursor, funCtorArgs_);
        }
        return cursor;
    }
};
=======
static bool ValidateFFI(JSContext* cx, const AsmJSGlobal& global,
                        HandleValue importVal,
                        MutableHandle<FunctionVector> ffis) {
  RootedValue v(cx);
  if (!GetDataProperty(cx, importVal, global.field(), &v)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
class ModuleCharsForLookup : ModuleChars {
  Vector<char16_t, 0, SystemAllocPolicy> chars_;
||||||| merged common ancestors
class ModuleCharsForLookup : ModuleChars
{
    Vector<char16_t, 0, SystemAllocPolicy> chars_;
=======
  if (!IsFunctionObject(v)) {
    return LinkFail(cx, "FFI imports must be functions");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  const uint8_t* deserialize(const uint8_t* cursor) {
    uint32_t uncompressedSize;
    cursor = ReadScalar<uint32_t>(cursor, &uncompressedSize);
||||||| merged common ancestors
  public:
    const uint8_t* deserialize(const uint8_t* cursor) {
        uint32_t uncompressedSize;
        cursor = ReadScalar<uint32_t>(cursor, &uncompressedSize);
=======
  ffis[global.ffiIndex()].set(&v.toObject().as<JSFunction>());
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    uint32_t compressedSize;
    cursor = ReadScalar<uint32_t>(cursor, &compressedSize);
||||||| merged common ancestors
        uint32_t compressedSize;
        cursor = ReadScalar<uint32_t>(cursor, &compressedSize);

        if (!chars_.resize(uncompressedSize / sizeof(char16_t))) {
            return nullptr;
        }
=======
static bool ValidateArrayView(JSContext* cx, const AsmJSGlobal& global,
                              HandleValue globalVal) {
  if (!global.field()) {
    return true;
  }

  if (Scalar::isBigIntType(global.viewType())) {
    return LinkFail(cx, "bad typed array constructor");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!chars_.resize(uncompressedSize / sizeof(char16_t))) {
      return nullptr;
    }
||||||| merged common ancestors
        const char* source = reinterpret_cast<const char*>(cursor);
        char* dest = reinterpret_cast<char*>(chars_.begin());
        if (!LZ4::decompress(source, dest, uncompressedSize)) {
            return nullptr;
        }
=======
  RootedValue v(cx);
  if (!GetDataProperty(cx, globalVal, global.field(), &v)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    const char* source = reinterpret_cast<const char*>(cursor);
    char* dest = reinterpret_cast<char*>(chars_.begin());
    if (!LZ4::decompress(source, dest, uncompressedSize)) {
      return nullptr;
    }
||||||| merged common ancestors
        cursor += compressedSize;
=======
  bool tac = IsTypedArrayConstructor(v, global.viewType());
  if (!tac) {
    return LinkFail(cx, "bad typed array constructor");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    cursor += compressedSize;
||||||| merged common ancestors
        cursor = ReadScalar<uint32_t>(cursor, &isFunCtor_);
        if (isFunCtor_) {
            cursor = DeserializeVector(cursor, &funCtorArgs_);
        }
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    cursor = ReadScalar<uint32_t>(cursor, &isFunCtor_);
    if (isFunCtor_) {
      cursor = DeserializeVector(cursor, &funCtorArgs_);
    }
||||||| merged common ancestors
        return cursor;
    }
=======
static bool ValidateMathBuiltinFunction(JSContext* cx,
                                        const AsmJSGlobal& global,
                                        HandleValue globalVal) {
  RootedValue v(cx);
  if (!GetDataProperty(cx, globalVal, cx->names().Math, &v)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    return cursor;
  }

  bool match(AsmJSParser& parser) const {
    const char16_t* parseBegin =
        parser.tokenStream.codeUnitPtrAt(beginOffset(parser));
    const char16_t* parseLimit = parser.tokenStream.rawLimit();
    MOZ_ASSERT(parseLimit >= parseBegin);
    if (uint32_t(parseLimit - parseBegin) < chars_.length()) {
      return false;
    }
    if (!ArrayEqual(chars_.begin(), parseBegin, chars_.length())) {
      return false;
    }
    if (isFunCtor_ != parser.pc->isStandaloneFunctionBody()) {
      return false;
    }
    if (isFunCtor_) {
      // For function statements, the closing } is included as the last
      // character of the matched source. For Function constructor,
      // parsing terminates with EOF which we must explicitly check. This
      // prevents
      //   new Function('"use asm"; function f() {} return f')
      // from incorrectly matching
      //   new Function('"use asm"; function f() {} return ff')
      if (parseBegin + chars_.length() != parseLimit) {
        return false;
      }
      unsigned numArgs;
      CodeNode* functionNode = parser.pc->functionBox()->functionNode;
      ParseNode* arg = FunctionFormalParametersList(functionNode, &numArgs);
      if (funCtorArgs_.length() != numArgs) {
        return false;
      }
      for (unsigned i = 0; i < funCtorArgs_.length(); i++, arg = arg->pn_next) {
        UniqueChars name =
            StringToNewUTF8CharsZ(nullptr, *arg->as<NameNode>().name());
        if (!name || strcmp(funCtorArgs_[i].get(), name.get())) {
          return false;
||||||| merged common ancestors
    bool match(AsmJSParser& parser) const {
        const char16_t* parseBegin = parser.tokenStream.codeUnitPtrAt(beginOffset(parser));
        const char16_t* parseLimit = parser.tokenStream.rawLimit();
        MOZ_ASSERT(parseLimit >= parseBegin);
        if (uint32_t(parseLimit - parseBegin) < chars_.length()) {
            return false;
        }
        if (!ArrayEqual(chars_.begin(), parseBegin, chars_.length())) {
            return false;
        }
        if (isFunCtor_ != parser.pc->isStandaloneFunctionBody()) {
            return false;
        }
        if (isFunCtor_) {
            // For function statements, the closing } is included as the last
            // character of the matched source. For Function constructor,
            // parsing terminates with EOF which we must explicitly check. This
            // prevents
            //   new Function('"use asm"; function f() {} return f')
            // from incorrectly matching
            //   new Function('"use asm"; function f() {} return ff')
            if (parseBegin + chars_.length() != parseLimit) {
                return false;
            }
            unsigned numArgs;
            CodeNode* functionNode = parser.pc->functionBox()->functionNode;
            ParseNode* arg = FunctionFormalParametersList(functionNode, &numArgs);
            if (funCtorArgs_.length() != numArgs) {
                return false;
            }
            for (unsigned i = 0; i < funCtorArgs_.length(); i++, arg = arg->pn_next) {
                UniqueChars name = StringToNewUTF8CharsZ(nullptr, *arg->as<NameNode>().name());
                if (!name || strcmp(funCtorArgs_[i].get(), name.get())) {
                    return false;
                }
            }
=======
  if (!GetDataProperty(cx, v, global.field(), &v)) {
    return false;
  }

  Native native = nullptr;
  switch (global.mathBuiltinFunction()) {
    case AsmJSMathBuiltin_sin:
      native = math_sin;
      break;
    case AsmJSMathBuiltin_cos:
      native = math_cos;
      break;
    case AsmJSMathBuiltin_tan:
      native = math_tan;
      break;
    case AsmJSMathBuiltin_asin:
      native = math_asin;
      break;
    case AsmJSMathBuiltin_acos:
      native = math_acos;
      break;
    case AsmJSMathBuiltin_atan:
      native = math_atan;
      break;
    case AsmJSMathBuiltin_ceil:
      native = math_ceil;
      break;
    case AsmJSMathBuiltin_floor:
      native = math_floor;
      break;
    case AsmJSMathBuiltin_exp:
      native = math_exp;
      break;
    case AsmJSMathBuiltin_log:
      native = math_log;
      break;
    case AsmJSMathBuiltin_pow:
      native = math_pow;
      break;
    case AsmJSMathBuiltin_sqrt:
      native = math_sqrt;
      break;
    case AsmJSMathBuiltin_min:
      native = math_min;
      break;
    case AsmJSMathBuiltin_max:
      native = math_max;
      break;
    case AsmJSMathBuiltin_abs:
      native = math_abs;
      break;
    case AsmJSMathBuiltin_atan2:
      native = math_atan2;
      break;
    case AsmJSMathBuiltin_imul:
      native = math_imul;
      break;
    case AsmJSMathBuiltin_clz32:
      native = math_clz32;
      break;
    case AsmJSMathBuiltin_fround:
      native = math_fround;
      break;
  }

  if (!IsNativeFunction(v, native)) {
    return LinkFail(cx, "bad Math.* builtin function");
  }

  return true;
}

static bool ValidateConstant(JSContext* cx, const AsmJSGlobal& global,
                             HandleValue globalVal) {
  RootedValue v(cx, globalVal);

  if (global.constantKind() == AsmJSGlobal::MathConstant) {
    if (!GetDataProperty(cx, v, cx->names().Math, &v)) {
      return false;
    }
  }

  if (!GetDataProperty(cx, v, global.field(), &v)) {
    return false;
  }

  if (!v.isNumber()) {
    return LinkFail(cx, "math / global constant value needs to be a number");
  }

  // NaN != NaN
  if (IsNaN(global.constantValue())) {
    if (!IsNaN(v.toNumber())) {
      return LinkFail(cx, "global constant value needs to be NaN");
    }
  } else {
    if (v.toNumber() != global.constantValue()) {
      return LinkFail(cx, "global constant value mismatch");
    }
  }

  return true;
}

static bool CheckBuffer(JSContext* cx, const AsmJSMetadata& metadata,
                        HandleValue bufferVal,
                        MutableHandle<ArrayBufferObjectMaybeShared*> buffer) {
  if (metadata.memoryUsage == MemoryUsage::Shared) {
    if (!IsSharedArrayBuffer(bufferVal)) {
      return LinkFail(
          cx, "shared views can only be constructed onto SharedArrayBuffer");
    }
  } else {
    if (!IsArrayBuffer(bufferVal)) {
      return LinkFail(
          cx, "unshared views can only be constructed onto ArrayBuffer");
    }
  }

  buffer.set(&AsAnyArrayBuffer(bufferVal));
  uint32_t memoryLength = buffer->byteLength();

  if (!IsValidAsmJSHeapLength(memoryLength)) {
    UniqueChars msg(JS_smprintf(
        "ArrayBuffer byteLength 0x%x is not a valid heap length. The next "
        "valid length is 0x%x",
        memoryLength, RoundUpToNextValidAsmJSHeapLength(memoryLength)));
    if (!msg) {
      return false;
    }
    return LinkFail(cx, msg.get());
  }

  // This check is sufficient without considering the size of the loaded datum
  // because heap loads and stores start on an aligned boundary and the heap
  // byteLength has larger alignment.
  MOZ_ASSERT((metadata.minMemoryLength - 1) <= INT32_MAX);
  if (memoryLength < metadata.minMemoryLength) {
    UniqueChars msg(JS_smprintf(
        "ArrayBuffer byteLength of 0x%x is less than 0x%x (the size implied "
        "by const heap accesses).",
        memoryLength, metadata.minMemoryLength));
    if (!msg) {
      return false;
    }
    return LinkFail(cx, msg.get());
  }

  if (buffer->is<ArrayBufferObject>()) {
    Rooted<ArrayBufferObject*> arrayBuffer(cx,
                                           &buffer->as<ArrayBufferObject>());
    if (!arrayBuffer->prepareForAsmJS()) {
      return LinkFail(cx, "Unable to prepare ArrayBuffer for asm.js use");
    }
  } else {
    return LinkFail(cx, "Unable to prepare SharedArrayBuffer for asm.js use");
  }

  MOZ_ASSERT(buffer->isPreparedForAsmJS());
  return true;
}

static bool GetImports(JSContext* cx, const AsmJSMetadata& metadata,
                       HandleValue globalVal, HandleValue importVal,
                       ImportValues* imports) {
  Rooted<FunctionVector> ffis(cx, FunctionVector(cx));
  if (!ffis.resize(metadata.numFFIs)) {
    return false;
  }

  for (const AsmJSGlobal& global : metadata.asmJSGlobals) {
    switch (global.which()) {
      case AsmJSGlobal::Variable: {
        Maybe<LitValPOD> litVal;
        if (!ValidateGlobalVariable(cx, global, importVal, &litVal)) {
          return false;
        }
        if (!imports->globalValues.append(Val(litVal->asLitVal()))) {
          return false;
        }
        break;
      }
      case AsmJSGlobal::FFI:
        if (!ValidateFFI(cx, global, importVal, &ffis)) {
          return false;
        }
        break;
      case AsmJSGlobal::ArrayView:
      case AsmJSGlobal::ArrayViewCtor:
        if (!ValidateArrayView(cx, global, globalVal)) {
          return false;
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
      }
    }
    return true;
  }
};

struct ScopedCacheEntryOpenedForWrite {
  JSContext* cx;
  const size_t serializedSize;
  uint8_t* memory;
  intptr_t handle;

  ScopedCacheEntryOpenedForWrite(JSContext* cx, size_t serializedSize)
      : cx(cx), serializedSize(serializedSize), memory(nullptr), handle(-1) {}

  ~ScopedCacheEntryOpenedForWrite() {
    if (memory) {
      cx->asmJSCacheOps().closeEntryForWrite(serializedSize, memory, handle);
    }
  }
};

struct ScopedCacheEntryOpenedForRead {
  JSContext* cx;
  size_t serializedSize;
  const uint8_t* memory;
  intptr_t handle;

  explicit ScopedCacheEntryOpenedForRead(JSContext* cx)
      : cx(cx), serializedSize(0), memory(nullptr), handle(0) {}

  ~ScopedCacheEntryOpenedForRead() {
    if (memory) {
      cx->asmJSCacheOps().closeEntryForRead(serializedSize, memory, handle);
||||||| merged common ancestors
        return true;
    }
};

struct ScopedCacheEntryOpenedForWrite
{
    JSContext* cx;
    const size_t serializedSize;
    uint8_t* memory;
    intptr_t handle;

    ScopedCacheEntryOpenedForWrite(JSContext* cx, size_t serializedSize)
      : cx(cx), serializedSize(serializedSize), memory(nullptr), handle(-1)
    {}

    ~ScopedCacheEntryOpenedForWrite() {
        if (memory) {
            cx->asmJSCacheOps().closeEntryForWrite(serializedSize, memory, handle);
        }
    }
};

struct ScopedCacheEntryOpenedForRead
{
    JSContext* cx;
    size_t serializedSize;
    const uint8_t* memory;
    intptr_t handle;

    explicit ScopedCacheEntryOpenedForRead(JSContext* cx)
      : cx(cx), serializedSize(0), memory(nullptr), handle(0)
    {}

    ~ScopedCacheEntryOpenedForRead() {
        if (memory) {
            cx->asmJSCacheOps().closeEntryForRead(serializedSize, memory, handle);
        }
=======
        break;
      case AsmJSGlobal::MathBuiltinFunction:
        if (!ValidateMathBuiltinFunction(cx, global, globalVal)) {
          return false;
        }
        break;
      case AsmJSGlobal::Constant:
        if (!ValidateConstant(cx, global, globalVal)) {
          return false;
        }
        break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
};

}  // unnamed namespace
||||||| merged common ancestors
};

} // unnamed namespace
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static JS::AsmJSCacheResult StoreAsmJSModuleInCache(AsmJSParser& parser,
                                                    const Module& module,
                                                    const LinkData& linkData,
                                                    JSContext* cx) {
  ModuleCharsForStore moduleChars;
  if (!moduleChars.init(parser)) {
    return JS::AsmJSCache_InternalError;
  }
||||||| merged common ancestors
static JS::AsmJSCacheResult
StoreAsmJSModuleInCache(AsmJSParser& parser, const Module& module, const LinkData& linkData,
                        JSContext* cx)
{
    ModuleCharsForStore moduleChars;
    if (!moduleChars.init(parser)) {
        return JS::AsmJSCache_InternalError;
    }
=======
  for (const AsmJSImport& import : metadata.asmJSImports) {
    if (!imports->funcs.append(ffis[import.ffiIndex()])) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t moduleSize = module.serializedSize(linkData);
  MOZ_RELEASE_ASSERT(moduleSize <= UINT32_MAX);

  Assumptions assumptions;
  if (!assumptions.init()) {
    return JS::AsmJSCache_InternalError;
  }
||||||| merged common ancestors
    size_t moduleSize = module.serializedSize(linkData);
    MOZ_RELEASE_ASSERT(moduleSize <= UINT32_MAX);

    Assumptions assumptions;
    if (!assumptions.init()) {
        return JS::AsmJSCache_InternalError;
    }
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t serializedSize = assumptions.serializedSize() + sizeof(uint32_t) +
                          moduleSize + moduleChars.serializedSize();
||||||| merged common ancestors
    size_t serializedSize = assumptions.serializedSize() +
                            sizeof(uint32_t) +
                            moduleSize +
                            moduleChars.serializedSize();
=======
static bool TryInstantiate(JSContext* cx, CallArgs args, const Module& module,
                           const AsmJSMetadata& metadata,
                           MutableHandleWasmInstanceObject instanceObj,
                           MutableHandleObject exportObj) {
  HandleValue globalVal = args.get(0);
  HandleValue importVal = args.get(1);
  HandleValue bufferVal = args.get(2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::OpenAsmJSCacheEntryForWriteOp open =
      cx->asmJSCacheOps().openEntryForWrite;
  if (!open) {
    return JS::AsmJSCache_Disabled_Internal;
  }
||||||| merged common ancestors
    JS::OpenAsmJSCacheEntryForWriteOp open = cx->asmJSCacheOps().openEntryForWrite;
    if (!open) {
        return JS::AsmJSCache_Disabled_Internal;
    }
=======
  // Re-check HasCompilerSupport(cx) since this varies per-thread and
  // 'module' may have been produced on a parser thread.
  if (!HasCompilerSupport(cx)) {
    return LinkFail(cx, "no compiler support");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const char16_t* begin =
      parser.tokenStream.codeUnitPtrAt(ModuleChars::beginOffset(parser));
  const char16_t* end =
      parser.tokenStream.codeUnitPtrAt(ModuleChars::endOffset(parser));
||||||| merged common ancestors
    const char16_t* begin = parser.tokenStream.codeUnitPtrAt(ModuleChars::beginOffset(parser));
    const char16_t* end = parser.tokenStream.codeUnitPtrAt(ModuleChars::endOffset(parser));
=======
  Rooted<ImportValues> imports(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  ScopedCacheEntryOpenedForWrite entry(cx, serializedSize);
  JS::AsmJSCacheResult openResult = open(
      cx->global(), begin, end, serializedSize, &entry.memory, &entry.handle);
  if (openResult != JS::AsmJSCache_Success) {
    return openResult;
  }
||||||| merged common ancestors
    ScopedCacheEntryOpenedForWrite entry(cx, serializedSize);
    JS::AsmJSCacheResult openResult =
        open(cx->global(), begin, end, serializedSize, &entry.memory, &entry.handle);
    if (openResult != JS::AsmJSCache_Success) {
        return openResult;
    }
=======
  if (module.metadata().usesMemory()) {
    RootedArrayBufferObjectMaybeShared buffer(cx);
    if (!CheckBuffer(cx, metadata, bufferVal, &buffer)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint8_t* cursor = entry.memory;

  cursor = assumptions.serialize(cursor);
||||||| merged common ancestors
    uint8_t* cursor = entry.memory;

    cursor = assumptions.serialize(cursor);
=======
    imports.get().memory = WasmMemoryObject::create(cx, buffer, nullptr);
    if (!imports.get().memory) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  cursor = WriteScalar<uint32_t>(cursor, moduleSize);
||||||| merged common ancestors
    cursor = WriteScalar<uint32_t>(cursor, moduleSize);
=======
  if (!GetImports(cx, metadata, globalVal, importVal, imports.address())) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  module.serialize(linkData, cursor, moduleSize);
  cursor += moduleSize;
||||||| merged common ancestors
    module.serialize(linkData, cursor, moduleSize);
    cursor += moduleSize;
=======
  if (!module.instantiate(cx, imports.get(), nullptr, instanceObj)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  cursor = moduleChars.serialize(cursor);
||||||| merged common ancestors
    cursor = moduleChars.serialize(cursor);
=======
  exportObj.set(&instanceObj->exportsObj());
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_RELEASE_ASSERT(cursor == entry.memory + serializedSize);
||||||| merged common ancestors
    MOZ_RELEASE_ASSERT(cursor == entry.memory + serializedSize);
=======
static bool HandleInstantiationFailure(JSContext* cx, CallArgs args,
                                       const AsmJSMetadata& metadata) {
  RootedAtom name(cx, args.callee().as<JSFunction>().explicitName());
>>>>>>> upstream-releases

<<<<<<< HEAD
  return JS::AsmJSCache_Success;
}
||||||| merged common ancestors
    return JS::AsmJSCache_Success;
}
=======
  if (cx->isExceptionPending()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool LookupAsmJSModuleInCache(JSContext* cx, AsmJSParser& parser,
                                     bool* loadedFromCache,
                                     SharedModule* module,
                                     UniqueChars* compilationTimeReport) {
  int64_t before = PRMJ_Now();
||||||| merged common ancestors
static bool
LookupAsmJSModuleInCache(JSContext* cx, AsmJSParser& parser, bool* loadedFromCache,
                         SharedModule* module, UniqueChars* compilationTimeReport)
{
    int64_t before = PRMJ_Now();
=======
  ScriptSource* source = metadata.scriptSource.get();
>>>>>>> upstream-releases

<<<<<<< HEAD
  *loadedFromCache = false;
||||||| merged common ancestors
    *loadedFromCache = false;
=======
  // Source discarding is allowed to affect JS semantics because it is never
  // enabled for normal JS content.
  bool haveSource;
  if (!ScriptSource::loadSource(cx, source, &haveSource)) {
    return false;
  }
  if (!haveSource) {
    JS_ReportErrorASCII(cx,
                        "asm.js link failure with source discarding enabled");
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::OpenAsmJSCacheEntryForReadOp open = cx->asmJSCacheOps().openEntryForRead;
  if (!open) {
    return true;
  }
||||||| merged common ancestors
    JS::OpenAsmJSCacheEntryForReadOp open = cx->asmJSCacheOps().openEntryForRead;
    if (!open) {
        return true;
    }

    const char16_t* begin = parser.tokenStream.codeUnitPtrAt(ModuleChars::beginOffset(parser));
    const char16_t* limit = parser.tokenStream.rawLimit();
=======
  uint32_t begin = metadata.toStringStart;
  uint32_t end = metadata.srcEndAfterCurly();
  Rooted<JSFlatString*> src(cx, source->substringDontDeflate(cx, begin, end));
  if (!src) {
    return false;
  }

  RootedFunction fun(
      cx, NewScriptedFunction(cx, 0, JSFunction::INTERPRETED_NORMAL, name,
                              /* proto = */ nullptr, gc::AllocKind::FUNCTION,
                              TenuredObject));
  if (!fun) {
    return false;
  }

  JS::CompileOptions options(cx);
  options.setMutedErrors(source->mutedErrors())
      .setFile(source->filename())
      .setNoScriptRval(false);
  options.asmJSOption = AsmJSOption::Disabled;

  // The exported function inherits an implicit strict context if the module
  // also inherited it somehow.
  if (metadata.strict) {
    options.strictOption = true;
  }

  AutoStableStringChars stableChars(cx);
  if (!stableChars.initTwoByte(cx, src)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const char16_t* begin =
      parser.tokenStream.codeUnitPtrAt(ModuleChars::beginOffset(parser));
  const char16_t* limit = parser.tokenStream.rawLimit();
||||||| merged common ancestors
    ScopedCacheEntryOpenedForRead entry(cx);
    if (!open(cx->global(), begin, limit, &entry.serializedSize, &entry.memory, &entry.handle)) {
        return true;
    }
=======
  SourceText<char16_t> srcBuf;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ScopedCacheEntryOpenedForRead entry(cx);
  if (!open(cx->global(), begin, limit, &entry.serializedSize, &entry.memory,
            &entry.handle)) {
    return true;
  }
||||||| merged common ancestors
    const uint8_t* cursor = entry.memory;
=======
  const char16_t* chars = stableChars.twoByteRange().begin().get();
  SourceOwnership ownership = stableChars.maybeGiveOwnershipToCaller()
                                  ? SourceOwnership::TakeOwnership
                                  : SourceOwnership::Borrowed;
  if (!srcBuf.init(cx, chars, end - begin, ownership)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const uint8_t* cursor = entry.memory;
||||||| merged common ancestors
    Assumptions deserializedAssumptions;
    cursor = deserializedAssumptions.deserialize(cursor, entry.serializedSize);
    if (!cursor) {
        return true;
    }
=======
  if (!frontend::CompileStandaloneFunction(cx, &fun, options, srcBuf,
                                           Nothing())) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Assumptions deserializedAssumptions;
  cursor = deserializedAssumptions.deserialize(cursor, entry.serializedSize);
  if (!cursor) {
    return true;
  }
||||||| merged common ancestors
    Assumptions currentAssumptions;
    if (!currentAssumptions.init() || currentAssumptions != deserializedAssumptions) {
        return true;
    }
=======
  // Call the function we just recompiled.
  args.setCallee(ObjectValue(*fun));
  return InternalCallOrConstruct(
      cx, args, args.isConstructing() ? CONSTRUCT : NO_CONSTRUCT);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Assumptions currentAssumptions;
  if (!currentAssumptions.init() ||
      currentAssumptions != deserializedAssumptions) {
    return true;
  }
||||||| merged common ancestors
    uint32_t moduleSize;
    cursor = ReadScalar<uint32_t>(cursor, &moduleSize);
    if (!cursor) {
        return true;
    }
=======
static const Module& AsmJSModuleFunctionToModule(JSFunction* fun) {
  MOZ_ASSERT(IsAsmJSModule(fun));
  const Value& v = fun->getExtendedSlot(FunctionExtended::ASMJS_MODULE_SLOT);
  return v.toObject().as<WasmModuleObject>().module();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t moduleSize;
  cursor = ReadScalar<uint32_t>(cursor, &moduleSize);
  if (!cursor) {
    return true;
  }
||||||| merged common ancestors
    MutableAsmJSMetadata asmJSMetadata = cx->new_<AsmJSMetadata>();
    if (!asmJSMetadata) {
        return false;
    }
=======
// Implements the semantics of an asm.js module function that has been
// successfully validated.
bool js::InstantiateAsmJS(JSContext* cx, unsigned argc, JS::Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MutableAsmJSMetadata asmJSMetadata = cx->new_<AsmJSMetadata>();
  if (!asmJSMetadata) {
    return false;
  }
||||||| merged common ancestors
    *module = Module::deserialize(cursor, moduleSize, asmJSMetadata.get());
    if (!*module) {
        ReportOutOfMemory(cx);
        return false;
    }
    cursor += moduleSize;
=======
  JSFunction* callee = &args.callee().as<JSFunction>();
  const Module& module = AsmJSModuleFunctionToModule(callee);
  const AsmJSMetadata& metadata = module.metadata().asAsmJS();
>>>>>>> upstream-releases

<<<<<<< HEAD
  *module = Module::deserialize(cursor, moduleSize, asmJSMetadata.get());
  if (!*module) {
    ReportOutOfMemory(cx);
    return false;
  }
  cursor += moduleSize;

  // Due to the hash comparison made by openEntryForRead, this should succeed
  // with high probability.
  ModuleCharsForLookup moduleChars;
  cursor = moduleChars.deserialize(cursor);
  if (!moduleChars.match(parser)) {
    return true;
  }
||||||| merged common ancestors
    // Due to the hash comparison made by openEntryForRead, this should succeed
    // with high probability.
    ModuleCharsForLookup moduleChars;
    cursor = moduleChars.deserialize(cursor);
    if (!moduleChars.match(parser)) {
        return true;
    }
=======
  RootedWasmInstanceObject instanceObj(cx);
  RootedObject exportObj(cx);
  if (!TryInstantiate(cx, args, module, metadata, &instanceObj, &exportObj)) {
    // Link-time validation checks failed, so reparse the entire asm.js
    // module from scratch to get normal interpreted bytecode which we can
    // simply Invoke. Very slow.
    return HandleInstantiationFailure(cx, args, metadata);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Don't punish release users by crashing if there is a programmer error
  // here, just gracefully return with a cache miss.
#ifdef NIGHTLY_BUILD
  MOZ_RELEASE_ASSERT(cursor == entry.memory + entry.serializedSize);
#endif
  if (cursor != entry.memory + entry.serializedSize) {
    return true;
  }
||||||| merged common ancestors
    // Don't punish release users by crashing if there is a programmer error
    // here, just gracefully return with a cache miss.
#ifdef NIGHTLY_BUILD
    MOZ_RELEASE_ASSERT(cursor == entry.memory + entry.serializedSize);
#endif
    if (cursor != entry.memory + entry.serializedSize) {
        return true;
    }
=======
  args.rval().set(ObjectValue(*exportObj));
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // See AsmJSMetadata comment as well as ModuleValidator::init().
  asmJSMetadata->toStringStart = parser.pc->functionBox()->toStringStart;
  asmJSMetadata->srcStart =
      parser.pc->functionBox()->functionNode->body()->pn_pos.begin;
  asmJSMetadata->strict =
      parser.pc->sc()->strict() && !parser.pc->sc()->hasExplicitUseStrict();
  asmJSMetadata->scriptSource.reset(parser.ss);
||||||| merged common ancestors
    // See AsmJSMetadata comment as well as ModuleValidator::init().
    asmJSMetadata->toStringStart = parser.pc->functionBox()->toStringStart;
    asmJSMetadata->srcStart = parser.pc->functionBox()->functionNode->body()->pn_pos.begin;
    asmJSMetadata->strict = parser.pc->sc()->strict() && !parser.pc->sc()->hasExplicitUseStrict();
    asmJSMetadata->scriptSource.reset(parser.ss);
=======
static JSFunction* NewAsmJSModuleFunction(JSContext* cx, JSFunction* origFun,
                                          HandleObject moduleObj) {
  RootedAtom name(cx, origFun->explicitName());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!parser.tokenStream.advance(asmJSMetadata->srcEndBeforeCurly())) {
    return false;
  }
||||||| merged common ancestors
    if (!parser.tokenStream.advance(asmJSMetadata->srcEndBeforeCurly())) {
        return false;
    }
=======
  JSFunction::Flags flags = origFun->isLambda() ? JSFunction::ASMJS_LAMBDA_CTOR
                                                : JSFunction::ASMJS_CTOR;
  JSFunction* moduleFun = NewNativeConstructor(
      cx, InstantiateAsmJS, origFun->nargs(), name,
      gc::AllocKind::FUNCTION_EXTENDED, TenuredObject, flags);
  if (!moduleFun) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  int64_t after = PRMJ_Now();
  int ms = (after - before) / PRMJ_USEC_PER_MSEC;
  *compilationTimeReport = JS_smprintf("loaded from cache in %dms", ms);
  if (!*compilationTimeReport) {
    return false;
  }
||||||| merged common ancestors
    int64_t after = PRMJ_Now();
    int ms = (after - before) / PRMJ_USEC_PER_MSEC;
    *compilationTimeReport = JS_smprintf("loaded from cache in %dms", ms);
    if (!*compilationTimeReport) {
        return false;
    }
=======
  moduleFun->setExtendedSlot(FunctionExtended::ASMJS_MODULE_SLOT,
                             ObjectValue(*moduleObj));
>>>>>>> upstream-releases

<<<<<<< HEAD
  *loadedFromCache = true;
  return true;
||||||| merged common ancestors
    *loadedFromCache = true;
    return true;
=======
  MOZ_ASSERT(IsAsmJSModule(moduleFun));
  return moduleFun;
>>>>>>> upstream-releases
}

/*****************************************************************************/
// Top-level js::CompileAsmJS

<<<<<<< HEAD
static bool NoExceptionPending(JSContext* cx) {
  return cx->helperThread() || !cx->isExceptionPending();
||||||| merged common ancestors
static bool
NoExceptionPending(JSContext* cx)
{
    return cx->helperThread() || !cx->isExceptionPending();
=======
static bool NoExceptionPending(JSContext* cx) {
  return cx->isHelperThreadContext() || !cx->isExceptionPending();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool SuccessfulValidation(AsmJSParser& parser, UniqueChars str) {
  return parser.warningNoOffset(JSMSG_USE_ASM_TYPE_OK, str.get());
}
||||||| merged common ancestors
static bool
SuccessfulValidation(AsmJSParser& parser, UniqueChars str)
{
    return parser.warningNoOffset(JSMSG_USE_ASM_TYPE_OK, str.get());
}
=======
static bool SuccessfulValidation(frontend::ParserBase& parser,
                                 unsigned compilationTime) {
  constexpr unsigned errNum =
#ifdef JS_MORE_DETERMINISTIC
      JSMSG_USE_ASM_TYPE_OK_NO_TIME
#else
      JSMSG_USE_ASM_TYPE_OK
#endif
      ;
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool TypeFailureWarning(AsmJSParser& parser, const char* str) {
  if (parser.options().throwOnAsmJSValidationFailureOption) {
    parser.errorNoOffset(JSMSG_USE_ASM_TYPE_FAIL, str ? str : "");
    return false;
  }

  // Per the asm.js standard convention, whether failure sets a pending
  // exception determines whether to attempt non-asm.js reparsing, so ignore
  // the return value below.
  Unused << parser.warningNoOffset(JSMSG_USE_ASM_TYPE_FAIL, str ? str : "");
  return false;
||||||| merged common ancestors
static bool
TypeFailureWarning(AsmJSParser& parser, const char* str)
{
    if (parser.options().throwOnAsmJSValidationFailureOption) {
        parser.errorNoOffset(JSMSG_USE_ASM_TYPE_FAIL, str ? str : "");
        return false;
    }

    // Per the asm.js standard convention, whether failure sets a pending
    // exception determines whether to attempt non-asm.js reparsing, so ignore
    // the return value below.
    Unused << parser.warningNoOffset(JSMSG_USE_ASM_TYPE_FAIL, str ? str : "");
    return false;
=======
  char timeChars[20];
  SprintfLiteral(timeChars, "%u", compilationTime);

  return parser.warningNoOffset(errNum, timeChars);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool EstablishPreconditions(JSContext* cx, AsmJSParser& parser) {
  // asm.js requires Ion.
  if (!HasCompilerSupport(cx) || !IonCanCompile()) {
    return TypeFailureWarning(parser, "Disabled by lack of compiler support");
  }

  switch (parser.options().asmJSOption) {
    case AsmJSOption::Disabled:
      return TypeFailureWarning(parser, "Disabled by 'asmjs' runtime option");
    case AsmJSOption::DisabledByDebugger:
      return TypeFailureWarning(parser, "Disabled by debugger");
    case AsmJSOption::Enabled:
      break;
  }

  if (parser.pc->isGenerator()) {
    return TypeFailureWarning(parser, "Disabled by generator context");
  }
||||||| merged common ancestors
static bool
EstablishPreconditions(JSContext* cx, AsmJSParser& parser)
{
    // asm.js requires Ion.
    if (!HasCompilerSupport(cx) || !IonCanCompile()) {
        return TypeFailureWarning(parser, "Disabled by lack of compiler support");
    }

    switch (parser.options().asmJSOption) {
      case AsmJSOption::Disabled:
        return TypeFailureWarning(parser, "Disabled by 'asmjs' runtime option");
      case AsmJSOption::DisabledByDebugger:
        return TypeFailureWarning(parser, "Disabled by debugger");
      case AsmJSOption::Enabled:
        break;
    }

    if (parser.pc->isGenerator()) {
        return TypeFailureWarning(parser, "Disabled by generator context");
    }
=======
static bool TypeFailureWarning(frontend::ParserBase& parser, const char* str) {
  if (parser.options().throwOnAsmJSValidationFailureOption) {
    parser.errorNoOffset(JSMSG_USE_ASM_TYPE_FAIL, str ? str : "");
    return false;
  }

  // Per the asm.js standard convention, whether failure sets a pending
  // exception determines whether to attempt non-asm.js reparsing, so ignore
  // the return value below.
  Unused << parser.warningNoOffset(JSMSG_USE_ASM_TYPE_FAIL, str ? str : "");
  return false;
}

static bool EstablishPreconditions(JSContext* cx,
                                   frontend::ParserBase& parser) {
  // asm.js requires Ion.
  if (!HasCompilerSupport(cx) || !IonCanCompile()) {
    return TypeFailureWarning(parser, "Disabled by lack of compiler support");
  }

  switch (parser.options().asmJSOption) {
    case AsmJSOption::Disabled:
      return TypeFailureWarning(parser, "Disabled by 'asmjs' runtime option");
    case AsmJSOption::DisabledByDebugger:
      return TypeFailureWarning(parser, "Disabled by debugger");
    case AsmJSOption::Enabled:
      break;
  }

  if (parser.pc_->isGenerator()) {
    return TypeFailureWarning(parser, "Disabled by generator context");
  }

  if (parser.pc_->isAsync()) {
    return TypeFailureWarning(parser, "Disabled by async context");
  }

  if (parser.pc_->isArrowFunction()) {
    return TypeFailureWarning(parser, "Disabled by arrow function context");
  }

  // Class constructors are also methods
  if (parser.pc_->isMethod() || parser.pc_->isGetterOrSetter()) {
    return TypeFailureWarning(
        parser, "Disabled by class constructor or method context");
  }

  return true;
}

template <typename Unit>
static bool DoCompileAsmJS(JSContext* cx, AsmJSParser<Unit>& parser,
                           ParseNode* stmtList, bool* validated) {
  *validated = false;

  // Various conditions disable asm.js optimizations.
  if (!EstablishPreconditions(cx, parser)) {
    return NoExceptionPending(cx);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (parser.pc->isAsync()) {
    return TypeFailureWarning(parser, "Disabled by async context");
  }
||||||| merged common ancestors
    if (parser.pc->isAsync()) {
        return TypeFailureWarning(parser, "Disabled by async context");
    }
=======
  // "Checking" parses, validates and compiles, producing a fully compiled
  // WasmModuleObject as result.
  unsigned time;
  SharedModule module = CheckModule(cx, parser, stmtList, &time);
  if (!module) {
    return NoExceptionPending(cx);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (parser.pc->isArrowFunction()) {
    return TypeFailureWarning(parser, "Disabled by arrow function context");
  }
||||||| merged common ancestors
    if (parser.pc->isArrowFunction()) {
        return TypeFailureWarning(parser, "Disabled by arrow function context");
    }
=======
  // Hand over ownership to a GC object wrapper which can then be referenced
  // from the module function.
  Rooted<WasmModuleObject*> moduleObj(cx,
                                      WasmModuleObject::create(cx, *module));
  if (!moduleObj) {
    return false;
  }

  // The module function dynamically links the AsmJSModule when called and
  // generates a set of functions wrapping all the exports.
  FunctionBox* funbox = parser.pc_->functionBox();
  RootedFunction moduleFun(
      cx, NewAsmJSModuleFunction(cx, funbox->function(), moduleObj));
  if (!moduleFun) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Class constructors are also methods
  if (parser.pc->isMethod() || parser.pc->isGetterOrSetter()) {
    return TypeFailureWarning(
        parser, "Disabled by class constructor or method context");
  }
||||||| merged common ancestors
    // Class constructors are also methods
    if (parser.pc->isMethod() || parser.pc->isGetterOrSetter()) {
        return TypeFailureWarning(parser, "Disabled by class constructor or method context");
    }
=======
  // Finished! Clobber the default function created by the parser with the new
  // asm.js module function. Special cases in the bytecode emitter avoid
  // generating bytecode for asm.js functions, allowing this asm.js module
  // function to be the finished result.
  MOZ_ASSERT(funbox->function()->isInterpreted());
  funbox->clobberFunction(moduleFun);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  // Success! Write to the console with a "warning" message indicating
  // total compilation time.
  *validated = true;
  SuccessfulValidation(parser, time);
  return NoExceptionPending(cx);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static UniqueChars BuildConsoleMessage(unsigned time,
                                       JS::AsmJSCacheResult cacheResult) {
#ifndef JS_MORE_DETERMINISTIC
  const char* cacheString = "";
  switch (cacheResult) {
    case JS::AsmJSCache_Success:
      cacheString = "stored in cache";
      break;
    case JS::AsmJSCache_ModuleTooSmall:
      cacheString = "not stored in cache (too small to benefit)";
      break;
    case JS::AsmJSCache_SynchronousScript:
      cacheString =
          "unable to cache asm.js in synchronous scripts; try loading "
          "asm.js via <script async> or createElement('script')";
      break;
    case JS::AsmJSCache_QuotaExceeded:
      cacheString = "not enough temporary storage quota to store in cache";
      break;
    case JS::AsmJSCache_StorageInitFailure:
      cacheString = "storage initialization failed (consider filing a bug)";
      break;
    case JS::AsmJSCache_Disabled_Internal:
      cacheString =
          "caching disabled by internal configuration (consider filing a bug)";
      break;
    case JS::AsmJSCache_Disabled_ShellFlags:
      cacheString = "caching disabled by missing command-line arguments";
      break;
    case JS::AsmJSCache_Disabled_JitInspector:
      cacheString = "caching disabled by active JIT inspector";
      break;
    case JS::AsmJSCache_InternalError:
      cacheString =
          "unable to store in cache due to internal error (consider filing a "
          "bug)";
      break;
    case JS::AsmJSCache_Disabled_PrivateBrowsing:
      cacheString = "caching disabled by private browsing mode";
      break;
    case JS::AsmJSCache_LIMIT:
      MOZ_CRASH("bad AsmJSCacheResult");
      break;
  }

  return JS_smprintf("total compilation time %dms; %s", time, cacheString);
#else
  return DuplicateString("");
#endif
||||||| merged common ancestors
static UniqueChars
BuildConsoleMessage(unsigned time, JS::AsmJSCacheResult cacheResult)
{
#ifndef JS_MORE_DETERMINISTIC
    const char* cacheString = "";
    switch (cacheResult) {
      case JS::AsmJSCache_Success:
        cacheString = "stored in cache";
        break;
      case JS::AsmJSCache_ModuleTooSmall:
        cacheString = "not stored in cache (too small to benefit)";
        break;
      case JS::AsmJSCache_SynchronousScript:
        cacheString = "unable to cache asm.js in synchronous scripts; try loading "
                      "asm.js via <script async> or createElement('script')";
        break;
      case JS::AsmJSCache_QuotaExceeded:
        cacheString = "not enough temporary storage quota to store in cache";
        break;
      case JS::AsmJSCache_StorageInitFailure:
        cacheString = "storage initialization failed (consider filing a bug)";
        break;
      case JS::AsmJSCache_Disabled_Internal:
        cacheString = "caching disabled by internal configuration (consider filing a bug)";
        break;
      case JS::AsmJSCache_Disabled_ShellFlags:
        cacheString = "caching disabled by missing command-line arguments";
        break;
      case JS::AsmJSCache_Disabled_JitInspector:
        cacheString = "caching disabled by active JIT inspector";
        break;
      case JS::AsmJSCache_InternalError:
        cacheString = "unable to store in cache due to internal error (consider filing a bug)";
        break;
      case JS::AsmJSCache_Disabled_PrivateBrowsing:
        cacheString = "caching disabled by private browsing mode";
        break;
      case JS::AsmJSCache_LIMIT:
        MOZ_CRASH("bad AsmJSCacheResult");
        break;
    }

    return JS_smprintf("total compilation time %dms; %s", time, cacheString);
#else
    return DuplicateString("");
#endif
=======
bool js::CompileAsmJS(JSContext* cx, AsmJSParser<char16_t>& parser,
                      ParseNode* stmtList, bool* validated) {
  return DoCompileAsmJS(cx, parser, stmtList, validated);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool js::CompileAsmJS(JSContext* cx, AsmJSParser& parser, ParseNode* stmtList,
                      bool* validated) {
  *validated = false;

  // Various conditions disable asm.js optimizations.
  if (!EstablishPreconditions(cx, parser)) {
    return NoExceptionPending(cx);
  }

  // Before spending any time parsing the module, try to look it up in the
  // embedding's cache using the chars about to be parsed as the key.
  bool loadedFromCache;
  SharedModule module;
  UniqueChars message;
  if (!LookupAsmJSModuleInCache(cx, parser, &loadedFromCache, &module,
                                &message)) {
    return false;
  }

  // If not present in the cache, parse, validate and generate code in a
  // single linear pass over the chars of the asm.js module.
  if (!loadedFromCache) {
    // "Checking" parses, validates and compiles, producing a fully compiled
    // WasmModuleObject as result.
    UniqueLinkData linkData;
    unsigned time;
    module = CheckModule(cx, parser, stmtList, &linkData, &time);
    if (!module) {
      return NoExceptionPending(cx);
    }

    // Try to store the AsmJSModule in the embedding's cache. The
    // AsmJSModule must be stored before static linking since static linking
    // specializes the AsmJSModule to the current process's address space
    // and therefore must be executed after a cache hit.
    JS::AsmJSCacheResult cacheResult =
        StoreAsmJSModuleInCache(parser, *module, *linkData, cx);

    // Build the string message to display in the developer console.
    message = BuildConsoleMessage(time, cacheResult);
    if (!message) {
      return NoExceptionPending(cx);
    }
  }

  // Hand over ownership to a GC object wrapper which can then be referenced
  // from the module function.
  Rooted<WasmModuleObject*> moduleObj(cx,
                                      WasmModuleObject::create(cx, *module));
  if (!moduleObj) {
    return false;
  }

  // The module function dynamically links the AsmJSModule when called and
  // generates a set of functions wrapping all the exports.
  FunctionBox* funbox = parser.pc->functionBox();
  RootedFunction moduleFun(
      cx, NewAsmJSModuleFunction(cx, funbox->function(), moduleObj));
  if (!moduleFun) {
    return false;
  }

  // Finished! Clobber the default function created by the parser with the new
  // asm.js module function. Special cases in the bytecode emitter avoid
  // generating bytecode for asm.js functions, allowing this asm.js module
  // function to be the finished result.
  MOZ_ASSERT(funbox->function()->isInterpreted());
  funbox->clobberFunction(moduleFun);

  // Success! Write to the console with a "warning" message.
  *validated = true;
  SuccessfulValidation(parser, std::move(message));
  return NoExceptionPending(cx);
||||||| merged common ancestors
bool
js::CompileAsmJS(JSContext* cx, AsmJSParser& parser, ParseNode* stmtList, bool* validated)
{
    *validated = false;

    // Various conditions disable asm.js optimizations.
    if (!EstablishPreconditions(cx, parser)) {
        return NoExceptionPending(cx);
    }

    // Before spending any time parsing the module, try to look it up in the
    // embedding's cache using the chars about to be parsed as the key.
    bool loadedFromCache;
    SharedModule module;
    UniqueChars message;
    if (!LookupAsmJSModuleInCache(cx, parser, &loadedFromCache, &module, &message)) {
        return false;
    }

    // If not present in the cache, parse, validate and generate code in a
    // single linear pass over the chars of the asm.js module.
    if (!loadedFromCache) {
        // "Checking" parses, validates and compiles, producing a fully compiled
        // WasmModuleObject as result.
        UniqueLinkData linkData;
        unsigned time;
        module = CheckModule(cx, parser, stmtList, &linkData, &time);
        if (!module) {
            return NoExceptionPending(cx);
        }

        // Try to store the AsmJSModule in the embedding's cache. The
        // AsmJSModule must be stored before static linking since static linking
        // specializes the AsmJSModule to the current process's address space
        // and therefore must be executed after a cache hit.
        JS::AsmJSCacheResult cacheResult = StoreAsmJSModuleInCache(parser, *module, *linkData, cx);

        // Build the string message to display in the developer console.
        message = BuildConsoleMessage(time, cacheResult);
        if (!message) {
            return NoExceptionPending(cx);
        }
    }

    // Hand over ownership to a GC object wrapper which can then be referenced
    // from the module function.
    Rooted<WasmModuleObject*> moduleObj(cx, WasmModuleObject::create(cx, *module));
    if (!moduleObj) {
        return false;
    }

    // The module function dynamically links the AsmJSModule when called and
    // generates a set of functions wrapping all the exports.
    FunctionBox* funbox = parser.pc->functionBox();
    RootedFunction moduleFun(cx, NewAsmJSModuleFunction(cx, funbox->function(), moduleObj));
    if (!moduleFun) {
        return false;
    }

    // Finished! Clobber the default function created by the parser with the new
    // asm.js module function. Special cases in the bytecode emitter avoid
    // generating bytecode for asm.js functions, allowing this asm.js module
    // function to be the finished result.
    MOZ_ASSERT(funbox->function()->isInterpreted());
    funbox->object = moduleFun;

    // Success! Write to the console with a "warning" message.
    *validated = true;
    SuccessfulValidation(parser, std::move(message));
    return NoExceptionPending(cx);
=======
bool js::CompileAsmJS(JSContext* cx, AsmJSParser<Utf8Unit>& parser,
                      ParseNode* stmtList, bool* validated) {
  return DoCompileAsmJS(cx, parser, stmtList, validated);
>>>>>>> upstream-releases
}

/*****************************************************************************/
// asm.js testing functions

<<<<<<< HEAD
bool js::IsAsmJSModuleNative(Native native) {
  return native == InstantiateAsmJS;
}

bool js::IsAsmJSModule(JSFunction* fun) {
  return fun->maybeNative() == InstantiateAsmJS;
||||||| merged common ancestors
bool
js::IsAsmJSModuleNative(Native native)
{
    return native == InstantiateAsmJS;
}

bool
js::IsAsmJSModule(JSFunction* fun)
{
    return fun->maybeNative() == InstantiateAsmJS;
=======
bool js::IsAsmJSModuleNative(Native native) {
  return native == InstantiateAsmJS;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool js::IsAsmJSFunction(JSFunction* fun) {
  if (IsExportedFunction(fun)) {
    return ExportedFunctionToInstance(fun).metadata().isAsmJS();
  }
  return false;
||||||| merged common ancestors
bool
js::IsAsmJSFunction(JSFunction* fun)
{
    if (IsExportedFunction(fun)) {
        return ExportedFunctionToInstance(fun).metadata().isAsmJS();
    }
    return false;
=======
bool js::IsAsmJSModule(JSFunction* fun) {
  return fun->maybeNative() == InstantiateAsmJS;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool js::IsAsmJSStrictModeModuleOrFunction(JSFunction* fun) {
  if (IsAsmJSModule(fun)) {
    return AsmJSModuleFunctionToModule(fun).metadata().asAsmJS().strict;
  }

  if (IsAsmJSFunction(fun)) {
    return ExportedFunctionToInstance(fun).metadata().asAsmJS().strict;
  }

  return false;
||||||| merged common ancestors
bool
js::IsAsmJSStrictModeModuleOrFunction(JSFunction* fun)
{
    if (IsAsmJSModule(fun)) {
        return AsmJSModuleFunctionToModule(fun).metadata().asAsmJS().strict;
    }

    if (IsAsmJSFunction(fun)) {
        return ExportedFunctionToInstance(fun).metadata().asAsmJS().strict;
    }

    return false;
=======
bool js::IsAsmJSFunction(JSFunction* fun) {
  return fun->kind() == JSFunction::AsmJS;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool js::IsAsmJSCompilationAvailable(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
||||||| merged common ancestors
bool
js::IsAsmJSCompilationAvailable(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
=======
bool js::IsAsmJSStrictModeModuleOrFunction(JSFunction* fun) {
  if (IsAsmJSModule(fun)) {
    return AsmJSModuleFunctionToModule(fun).metadata().asAsmJS().strict;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // See EstablishPreconditions.
  bool available =
      HasCompilerSupport(cx) && IonCanCompile() && cx->options().asmJS();
||||||| merged common ancestors
    // See EstablishPreconditions.
    bool available = HasCompilerSupport(cx) && IonCanCompile() && cx->options().asmJS();
=======
  if (IsAsmJSFunction(fun)) {
    return ExportedFunctionToInstance(fun).metadata().asAsmJS().strict;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  args.rval().set(BooleanValue(available));
  return true;
||||||| merged common ancestors
    args.rval().set(BooleanValue(available));
    return true;
=======
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static JSFunction* MaybeWrappedNativeFunction(const Value& v) {
  if (!v.isObject()) {
    return nullptr;
  }

  JSObject* obj = CheckedUnwrap(&v.toObject());
  if (!obj) {
    return nullptr;
  }
||||||| merged common ancestors
static JSFunction*
MaybeWrappedNativeFunction(const Value& v)
{
    if (!v.isObject()) {
        return nullptr;
    }

    JSObject* obj = CheckedUnwrap(&v.toObject());
    if (!obj) {
        return nullptr;
    }
=======
bool js::IsAsmJSCompilationAvailable(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!obj->is<JSFunction>()) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!obj->is<JSFunction>()) {
        return nullptr;
    }
=======
  // See EstablishPreconditions.
  bool available =
      HasCompilerSupport(cx) && IonCanCompile() && cx->options().asmJS();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return &obj->as<JSFunction>();
||||||| merged common ancestors
    return &obj->as<JSFunction>();
=======
  args.rval().set(BooleanValue(available));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool js::IsAsmJSModule(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  bool rval = false;
  if (JSFunction* fun = MaybeWrappedNativeFunction(args.get(0))) {
    rval = IsAsmJSModule(fun);
  }
||||||| merged common ancestors
bool
js::IsAsmJSModule(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);

    bool rval = false;
    if (JSFunction* fun = MaybeWrappedNativeFunction(args.get(0))) {
        rval = IsAsmJSModule(fun);
    }
=======
static JSFunction* MaybeWrappedNativeFunction(const Value& v) {
  if (!v.isObject()) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  args.rval().set(BooleanValue(rval));
  return true;
||||||| merged common ancestors
    args.rval().set(BooleanValue(rval));
    return true;
=======
  return v.toObject().maybeUnwrapIf<JSFunction>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool js::IsAsmJSFunction(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
||||||| merged common ancestors
bool
js::IsAsmJSFunction(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
=======
bool js::IsAsmJSModule(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool rval = false;
  if (JSFunction* fun = MaybeWrappedNativeFunction(args.get(0))) {
    rval = IsAsmJSFunction(fun);
  }
||||||| merged common ancestors
    bool rval = false;
    if (JSFunction* fun = MaybeWrappedNativeFunction(args.get(0))) {
        rval = IsAsmJSFunction(fun);
    }
=======
  bool rval = false;
  if (JSFunction* fun = MaybeWrappedNativeFunction(args.get(0))) {
    rval = IsAsmJSModule(fun);
  }
>>>>>>> upstream-releases

  args.rval().set(BooleanValue(rval));
  return true;
}

<<<<<<< HEAD
bool js::IsAsmJSModuleLoadedFromCache(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
||||||| merged common ancestors
bool
js::IsAsmJSModuleLoadedFromCache(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
=======
bool js::IsAsmJSFunction(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSFunction* fun = MaybeWrappedNativeFunction(args.get(0));
  if (!fun || !IsAsmJSModule(fun)) {
    JS_ReportErrorNumberUTF8(
        cx, GetErrorMessage, nullptr, JSMSG_USE_ASM_TYPE_FAIL,
        "argument passed to isAsmJSModuleLoadedFromCache is not a "
        "validated asm.js module");
    return false;
  }
||||||| merged common ancestors
    JSFunction* fun = MaybeWrappedNativeFunction(args.get(0));
    if (!fun || !IsAsmJSModule(fun)) {
        JS_ReportErrorNumberUTF8(cx, GetErrorMessage, nullptr, JSMSG_USE_ASM_TYPE_FAIL,
                                 "argument passed to isAsmJSModuleLoadedFromCache is not a "
                                 "validated asm.js module");
        return false;
    }
=======
  bool rval = false;
  if (JSFunction* fun = MaybeWrappedNativeFunction(args.get(0))) {
    rval = IsAsmJSFunction(fun);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool loadedFromCache =
      AsmJSModuleFunctionToModule(fun).metadata().asAsmJS().cacheResult ==
      CacheResult::Hit;

  args.rval().set(BooleanValue(loadedFromCache));
  return true;
||||||| merged common ancestors
    bool loadedFromCache =
        AsmJSModuleFunctionToModule(fun).metadata().asAsmJS().cacheResult == CacheResult::Hit;

    args.rval().set(BooleanValue(loadedFromCache));
    return true;
=======
  args.rval().set(BooleanValue(rval));
  return true;
>>>>>>> upstream-releases
}

/*****************************************************************************/
// asm.js toString/toSource support

JSString* js::AsmJSModuleToString(JSContext* cx, HandleFunction fun,
                                  bool isToSource) {
  MOZ_ASSERT(IsAsmJSModule(fun));

  const AsmJSMetadata& metadata =
      AsmJSModuleFunctionToModule(fun).metadata().asAsmJS();
  uint32_t begin = metadata.toStringStart;
  uint32_t end = metadata.srcEndAfterCurly();
  ScriptSource* source = metadata.scriptSource.get();

<<<<<<< HEAD
  StringBuffer out(cx);
||||||| merged common ancestors
    StringBuffer out(cx);
=======
  JSStringBuilder out(cx);
>>>>>>> upstream-releases

  if (isToSource && fun->isLambda() && !out.append("(")) {
    return nullptr;
  }

<<<<<<< HEAD
  bool haveSource = source->hasSourceText();
  if (!haveSource && !JSScript::loadSource(cx, source, &haveSource)) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!haveSource) {
        if (!out.append("function ")) {
            return nullptr;
        }
        if (fun->explicitName() && !out.append(fun->explicitName())) {
            return nullptr;
        }
        if (!out.append("() {\n    [native code]\n}")) {
            return nullptr;
        }
    } else {
        Rooted<JSFlatString*> src(cx, source->substring(cx, begin, end));
        if (!src) {
            return nullptr;
        }
=======
  bool haveSource;
  if (!ScriptSource::loadSource(cx, source, &haveSource)) {
    return nullptr;
  }
>>>>>>> upstream-releases

  if (!haveSource) {
    if (!out.append("function ")) {
      return nullptr;
    }
    if (fun->explicitName() && !out.append(fun->explicitName())) {
      return nullptr;
    }
    if (!out.append("() {\n    [native code]\n}")) {
      return nullptr;
    }
  } else {
    Rooted<JSFlatString*> src(cx, source->substring(cx, begin, end));
    if (!src) {
      return nullptr;
    }

    if (!out.append(src)) {
      return nullptr;
    }
  }

  if (isToSource && fun->isLambda() && !out.append(")")) {
    return nullptr;
  }

  return out.finishString();
}

JSString* js::AsmJSFunctionToString(JSContext* cx, HandleFunction fun) {
  MOZ_ASSERT(IsAsmJSFunction(fun));

  const AsmJSMetadata& metadata =
      ExportedFunctionToInstance(fun).metadata().asAsmJS();
  const AsmJSExport& f =
      metadata.lookupAsmJSExport(ExportedFunctionToFuncIndex(fun));

  uint32_t begin = metadata.srcStart + f.startOffsetInModule();
  uint32_t end = metadata.srcStart + f.endOffsetInModule();

<<<<<<< HEAD
  ScriptSource* source = metadata.scriptSource.get();
  StringBuffer out(cx);
||||||| merged common ancestors
    ScriptSource* source = metadata.scriptSource.get();
    StringBuffer out(cx);
=======
  ScriptSource* source = metadata.scriptSource.get();
  JSStringBuilder out(cx);
>>>>>>> upstream-releases

  if (!out.append("function ")) {
    return nullptr;
  }

<<<<<<< HEAD
  bool haveSource = source->hasSourceText();
  if (!haveSource && !JSScript::loadSource(cx, source, &haveSource)) {
    return nullptr;
  }
||||||| merged common ancestors
    bool haveSource = source->hasSourceText();
    if (!haveSource && !JSScript::loadSource(cx, source, &haveSource)) {
        return nullptr;
    }
=======
  bool haveSource;
  if (!ScriptSource::loadSource(cx, source, &haveSource)) {
    return nullptr;
  }
>>>>>>> upstream-releases

  if (!haveSource) {
    // asm.js functions can't be anonymous
    MOZ_ASSERT(fun->explicitName());
    if (!out.append(fun->explicitName())) {
      return nullptr;
    }
    if (!out.append("() {\n    [native code]\n}")) {
      return nullptr;
    }
  } else {
    Rooted<JSFlatString*> src(cx, source->substring(cx, begin, end));
    if (!src) {
      return nullptr;
    }
    if (!out.append(src)) {
      return nullptr;
    }
  }

  return out.finishString();
}

bool js::IsValidAsmJSHeapLength(uint32_t length) {
  if (length < MinHeapLength) {
    return false;
  }

  return wasm::IsValidARMImmediate(length);
}
