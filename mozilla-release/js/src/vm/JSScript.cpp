/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * JS script operations.
 */

#include "vm/JSScript-inl.h"

#include "mozilla/DebugOnly.h"
#include "mozilla/Maybe.h"
#include "mozilla/MemoryReporting.h"
#include "mozilla/PodOperations.h"
#include "mozilla/ScopeExit.h"
#include "mozilla/Sprintf.h"
#include "mozilla/Unused.h"
#include "mozilla/Vector.h"

#include <algorithm>
#include <new>
#include <string.h>
#include <type_traits>
#include <utility>

#include "jsapi.h"
#include "jstypes.h"
#include "jsutil.h"

#include "frontend/BytecodeCompiler.h"
#include "frontend/BytecodeEmitter.h"
#include "frontend/SharedContext.h"
#include "gc/FreeOp.h"
#include "jit/BaselineJIT.h"
#include "jit/Ion.h"
#include "jit/IonCode.h"
#include "jit/JitOptions.h"
#include "jit/JitRealm.h"
#include "js/CompileOptions.h"
#include "js/MemoryMetrics.h"
#include "js/Printf.h"
#include "js/SourceText.h"
#include "js/UniquePtr.h"
#include "js/Utility.h"
#include "js/Wrapper.h"
#include "util/StringBuffer.h"
#include "util/Text.h"
#include "vm/ArgumentsObject.h"
#include "vm/BytecodeIterator.h"
#include "vm/BytecodeLocation.h"
#include "vm/BytecodeUtil.h"
#include "vm/Compression.h"
#include "vm/Debugger.h"
#include "vm/HelperThreads.h"  // js::RunPendingSourceCompressions
#include "vm/JSAtom.h"
#include "vm/JSContext.h"
#include "vm/JSFunction.h"
#include "vm/JSObject.h"
#include "vm/Opcodes.h"
#include "vm/SelfHosting.h"
#include "vm/Shape.h"
#include "vm/SharedImmutableStringsCache.h"
#include "vm/Xdr.h"
#include "vtune/VTuneWrapper.h"

#include "gc/Marking-inl.h"
#include "vm/BytecodeIterator-inl.h"
#include "vm/BytecodeLocation-inl.h"
#include "vm/Compartment-inl.h"
#include "vm/EnvironmentObject-inl.h"
#include "vm/JSFunction-inl.h"
#include "vm/JSObject-inl.h"
#include "vm/NativeObject-inl.h"
#include "vm/SharedImmutableStringsCache-inl.h"
#include "vm/Stack-inl.h"

using namespace js;

using mozilla::Maybe;
using mozilla::PodCopy;
using mozilla::PointerRangeSize;
using mozilla::Utf8AsUnsignedChars;
using mozilla::Utf8Unit;

using JS::CompileOptions;
using JS::ReadOnlyCompileOptions;
<<<<<<< HEAD
using JS::SourceText;

template <XDRMode mode>
XDRResult js::XDRScriptConst(XDRState<mode>* xdr, MutableHandleValue vp) {
  JSContext* cx = xdr->cx();

  enum ConstTag {
    SCRIPT_INT,
    SCRIPT_DOUBLE,
    SCRIPT_ATOM,
    SCRIPT_TRUE,
    SCRIPT_FALSE,
    SCRIPT_NULL,
    SCRIPT_OBJECT,
    SCRIPT_VOID,
    SCRIPT_HOLE,
#ifdef ENABLE_BIGINT
    SCRIPT_BIGINT
#endif
  };

  ConstTag tag;
  if (mode == XDR_ENCODE) {
    if (vp.isInt32()) {
      tag = SCRIPT_INT;
    } else if (vp.isDouble()) {
      tag = SCRIPT_DOUBLE;
    } else if (vp.isString()) {
      tag = SCRIPT_ATOM;
    } else if (vp.isTrue()) {
      tag = SCRIPT_TRUE;
    } else if (vp.isFalse()) {
      tag = SCRIPT_FALSE;
    } else if (vp.isNull()) {
      tag = SCRIPT_NULL;
    } else if (vp.isObject()) {
      tag = SCRIPT_OBJECT;
    } else if (vp.isMagic(JS_ELEMENTS_HOLE)) {
      tag = SCRIPT_HOLE;
    }
#ifdef ENABLE_BIGINT
    else if (vp.isBigInt()) {
      tag = SCRIPT_BIGINT;
||||||| merged common ancestors
using JS::SourceBufferHolder;

template<XDRMode mode>
XDRResult
js::XDRScriptConst(XDRState<mode>* xdr, MutableHandleValue vp)
{
    JSContext* cx = xdr->cx();

    enum ConstTag {
        SCRIPT_INT,
        SCRIPT_DOUBLE,
        SCRIPT_ATOM,
        SCRIPT_TRUE,
        SCRIPT_FALSE,
        SCRIPT_NULL,
        SCRIPT_OBJECT,
        SCRIPT_VOID,
        SCRIPT_HOLE
    };

    ConstTag tag;
    if (mode == XDR_ENCODE) {
        if (vp.isInt32()) {
            tag = SCRIPT_INT;
        } else if (vp.isDouble()) {
            tag = SCRIPT_DOUBLE;
        } else if (vp.isString()) {
            tag = SCRIPT_ATOM;
        } else if (vp.isTrue()) {
            tag = SCRIPT_TRUE;
        } else if (vp.isFalse()) {
            tag = SCRIPT_FALSE;
        } else if (vp.isNull()) {
            tag = SCRIPT_NULL;
        } else if (vp.isObject()) {
            tag = SCRIPT_OBJECT;
        } else if (vp.isMagic(JS_ELEMENTS_HOLE)) {
            tag = SCRIPT_HOLE;
        } else {
            MOZ_ASSERT(vp.isUndefined());
            tag = SCRIPT_VOID;
        }
=======
using JS::SourceText;

template <XDRMode mode>
XDRResult js::XDRScriptConst(XDRState<mode>* xdr, MutableHandleValue vp) {
  JSContext* cx = xdr->cx();

  enum ConstTag {
    SCRIPT_INT,
    SCRIPT_DOUBLE,
    SCRIPT_ATOM,
    SCRIPT_TRUE,
    SCRIPT_FALSE,
    SCRIPT_NULL,
    SCRIPT_OBJECT,
    SCRIPT_VOID,
    SCRIPT_HOLE,
    SCRIPT_BIGINT
  };

  ConstTag tag;
  if (mode == XDR_ENCODE) {
    if (vp.isInt32()) {
      tag = SCRIPT_INT;
    } else if (vp.isDouble()) {
      tag = SCRIPT_DOUBLE;
    } else if (vp.isString()) {
      tag = SCRIPT_ATOM;
    } else if (vp.isTrue()) {
      tag = SCRIPT_TRUE;
    } else if (vp.isFalse()) {
      tag = SCRIPT_FALSE;
    } else if (vp.isNull()) {
      tag = SCRIPT_NULL;
    } else if (vp.isObject()) {
      tag = SCRIPT_OBJECT;
    } else if (vp.isMagic(JS_ELEMENTS_HOLE)) {
      tag = SCRIPT_HOLE;
    } else if (vp.isBigInt()) {
      tag = SCRIPT_BIGINT;
    } else {
      MOZ_ASSERT(vp.isUndefined());
      tag = SCRIPT_VOID;
    }
  }

  MOZ_TRY(xdr->codeEnum32(&tag));

  switch (tag) {
    case SCRIPT_INT: {
      uint32_t i;
      if (mode == XDR_ENCODE) {
        i = uint32_t(vp.toInt32());
      }
      MOZ_TRY(xdr->codeUint32(&i));
      if (mode == XDR_DECODE) {
        vp.set(Int32Value(int32_t(i)));
      }
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
#endif
    else {
      MOZ_ASSERT(vp.isUndefined());
      tag = SCRIPT_VOID;
    }
  }
||||||| merged common ancestors
=======
    case SCRIPT_DOUBLE: {
      double d;
      if (mode == XDR_ENCODE) {
        d = vp.toDouble();
      }
      MOZ_TRY(xdr->codeDouble(&d));
      if (mode == XDR_DECODE) {
        vp.set(DoubleValue(d));
      }
      break;
    }
    case SCRIPT_ATOM: {
      RootedAtom atom(cx);
      if (mode == XDR_ENCODE) {
        atom = &vp.toString()->asAtom();
      }
      MOZ_TRY(XDRAtom(xdr, &atom));
      if (mode == XDR_DECODE) {
        vp.set(StringValue(atom));
      }
      break;
    }
    case SCRIPT_TRUE:
      if (mode == XDR_DECODE) {
        vp.set(BooleanValue(true));
      }
      break;
    case SCRIPT_FALSE:
      if (mode == XDR_DECODE) {
        vp.set(BooleanValue(false));
      }
      break;
    case SCRIPT_NULL:
      if (mode == XDR_DECODE) {
        vp.set(NullValue());
      }
      break;
    case SCRIPT_OBJECT: {
      RootedObject obj(cx);
      if (mode == XDR_ENCODE) {
        obj = &vp.toObject();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_TRY(xdr->codeEnum32(&tag));
||||||| merged common ancestors
    MOZ_TRY(xdr->codeEnum32(&tag));
=======
      MOZ_TRY(XDRObjectLiteral(xdr, &obj));
>>>>>>> upstream-releases

<<<<<<< HEAD
  switch (tag) {
    case SCRIPT_INT: {
      uint32_t i;
      if (mode == XDR_ENCODE) {
        i = uint32_t(vp.toInt32());
||||||| merged common ancestors
    switch (tag) {
      case SCRIPT_INT: {
        uint32_t i;
        if (mode == XDR_ENCODE) {
            i = uint32_t(vp.toInt32());
        }
        MOZ_TRY(xdr->codeUint32(&i));
        if (mode == XDR_DECODE) {
            vp.set(Int32Value(int32_t(i)));
        }
        break;
=======
      if (mode == XDR_DECODE) {
        vp.setObject(*obj);
>>>>>>> upstream-releases
      }
<<<<<<< HEAD
      MOZ_TRY(xdr->codeUint32(&i));
      if (mode == XDR_DECODE) {
        vp.set(Int32Value(int32_t(i)));
||||||| merged common ancestors
      case SCRIPT_DOUBLE: {
        double d;
        if (mode == XDR_ENCODE) {
            d = vp.toDouble();
        }
        MOZ_TRY(xdr->codeDouble(&d));
        if (mode == XDR_DECODE) {
            vp.set(DoubleValue(d));
        }
        break;
=======
      break;
    }
    case SCRIPT_VOID:
      if (mode == XDR_DECODE) {
        vp.set(UndefinedValue());
>>>>>>> upstream-releases
      }
<<<<<<< HEAD
      break;
    }
    case SCRIPT_DOUBLE: {
      double d;
      if (mode == XDR_ENCODE) {
        d = vp.toDouble();
      }
      MOZ_TRY(xdr->codeDouble(&d));
      if (mode == XDR_DECODE) {
        vp.set(DoubleValue(d));
      }
      break;
    }
    case SCRIPT_ATOM: {
      RootedAtom atom(cx);
      if (mode == XDR_ENCODE) {
        atom = &vp.toString()->asAtom();
      }
      MOZ_TRY(XDRAtom(xdr, &atom));
      if (mode == XDR_DECODE) {
        vp.set(StringValue(atom));
      }
      break;
    }
    case SCRIPT_TRUE:
      if (mode == XDR_DECODE) {
        vp.set(BooleanValue(true));
      }
      break;
    case SCRIPT_FALSE:
      if (mode == XDR_DECODE) {
        vp.set(BooleanValue(false));
      }
      break;
    case SCRIPT_NULL:
      if (mode == XDR_DECODE) {
        vp.set(NullValue());
      }
      break;
    case SCRIPT_OBJECT: {
      RootedObject obj(cx);
      if (mode == XDR_ENCODE) {
        obj = &vp.toObject();
||||||| merged common ancestors
      case SCRIPT_ATOM: {
        RootedAtom atom(cx);
        if (mode == XDR_ENCODE) {
            atom = &vp.toString()->asAtom();
        }
        MOZ_TRY(XDRAtom(xdr, &atom));
        if (mode == XDR_DECODE) {
            vp.set(StringValue(atom));
        }
        break;
=======
      break;
    case SCRIPT_HOLE:
      if (mode == XDR_DECODE) {
        vp.setMagic(JS_ELEMENTS_HOLE);
      }
      break;
    case SCRIPT_BIGINT: {
      RootedBigInt bi(cx);
      if (mode == XDR_ENCODE) {
        bi = vp.toBigInt();
>>>>>>> upstream-releases
      }

<<<<<<< HEAD
      MOZ_TRY(XDRObjectLiteral(xdr, &obj));
||||||| merged common ancestors
        MOZ_TRY(XDRObjectLiteral(xdr, &obj));
=======
      MOZ_TRY(XDRBigInt(xdr, &bi));
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (mode == XDR_DECODE) {
        vp.setObject(*obj);
||||||| merged common ancestors
        if (mode == XDR_DECODE) {
            vp.setObject(*obj);
        }
        break;
=======
      if (mode == XDR_DECODE) {
        vp.setBigInt(bi);
>>>>>>> upstream-releases
      }
      break;
    }
<<<<<<< HEAD
    case SCRIPT_VOID:
      if (mode == XDR_DECODE) {
        vp.set(UndefinedValue());
      }
      break;
    case SCRIPT_HOLE:
      if (mode == XDR_DECODE) {
        vp.setMagic(JS_ELEMENTS_HOLE);
      }
      break;
#ifdef ENABLE_BIGINT
    case SCRIPT_BIGINT: {
      RootedBigInt bi(cx);
      if (mode == XDR_ENCODE) {
        bi = vp.toBigInt();
      }

      MOZ_TRY(XDRBigInt(xdr, &bi));

      if (mode == XDR_DECODE) {
        vp.setBigInt(bi);
      }
      break;
    }
#endif
    default:
      // Fail in debug, but only soft-fail in release
      MOZ_ASSERT(false, "Bad XDR value kind");
      return xdr->fail(JS::TranscodeResult_Failure_BadDecode);
  }
  return Ok();
||||||| merged common ancestors
    return Ok();
=======
    default:
      // Fail in debug, but only soft-fail in release
      MOZ_ASSERT(false, "Bad XDR value kind");
      return xdr->fail(JS::TranscodeResult_Failure_BadDecode);
  }
  return Ok();
>>>>>>> upstream-releases
}

template XDRResult js::XDRScriptConst(XDRState<XDR_ENCODE>*,
                                      MutableHandleValue);

template XDRResult js::XDRScriptConst(XDRState<XDR_DECODE>*,
                                      MutableHandleValue);

// Code LazyScript's closed over bindings.
<<<<<<< HEAD
template <XDRMode mode>
static XDRResult XDRLazyClosedOverBindings(XDRState<mode>* xdr,
                                           MutableHandle<LazyScript*> lazy) {
  JSContext* cx = xdr->cx();
  RootedAtom atom(cx);
  for (size_t i = 0; i < lazy->numClosedOverBindings(); i++) {
    uint8_t endOfScopeSentinel;
    if (mode == XDR_ENCODE) {
      atom = lazy->closedOverBindings()[i];
      endOfScopeSentinel = !atom;
    }
||||||| merged common ancestors
template<XDRMode mode>
static XDRResult
XDRLazyClosedOverBindings(XDRState<mode>* xdr, MutableHandle<LazyScript*> lazy)
{
    JSContext* cx = xdr->cx();
    RootedAtom atom(cx);
    for (size_t i = 0; i < lazy->numClosedOverBindings(); i++) {
        uint8_t endOfScopeSentinel;
        if (mode == XDR_ENCODE) {
            atom = lazy->closedOverBindings()[i];
            endOfScopeSentinel = !atom;
        }
=======
template <XDRMode mode>
static XDRResult XDRLazyClosedOverBindings(XDRState<mode>* xdr,
                                           MutableHandle<LazyScript*> lazy) {
  JSContext* cx = xdr->cx();
  RootedAtom atom(cx);
  for (GCPtrAtom& elem : lazy->closedOverBindings()) {
    uint8_t endOfScopeSentinel;
    if (mode == XDR_ENCODE) {
      atom = elem.get();
      endOfScopeSentinel = !atom;
    }
>>>>>>> upstream-releases

    MOZ_TRY(xdr->codeUint8(&endOfScopeSentinel));

    if (endOfScopeSentinel) {
      atom = nullptr;
    } else {
      MOZ_TRY(XDRAtom(xdr, &atom));
    }

<<<<<<< HEAD
    if (mode == XDR_DECODE) {
      lazy->closedOverBindings()[i] = atom;
||||||| merged common ancestors
        if (mode == XDR_DECODE) {
            lazy->closedOverBindings()[i] = atom;
        }
=======
    if (mode == XDR_DECODE) {
      elem.init(atom);
>>>>>>> upstream-releases
    }
  }

  return Ok();
}

// Code the missing part needed to re-create a LazyScript from a JSScript.
<<<<<<< HEAD
template <XDRMode mode>
static XDRResult XDRRelazificationInfo(XDRState<mode>* xdr, HandleFunction fun,
                                       HandleScript script,
                                       HandleScope enclosingScope,
                                       MutableHandle<LazyScript*> lazy) {
  MOZ_ASSERT_IF(mode == XDR_ENCODE, script->isRelazifiableIgnoringJitCode() &&
                                        script->maybeLazyScript());
  MOZ_ASSERT_IF(mode == XDR_ENCODE, !lazy->numInnerFunctions());

  JSContext* cx = xdr->cx();

  uint64_t packedFields;
  {
    uint32_t sourceStart = script->sourceStart();
    uint32_t sourceEnd = script->sourceEnd();
    uint32_t toStringStart = script->toStringStart();
    uint32_t toStringEnd = script->toStringEnd();
    uint32_t lineno = script->lineno();
    uint32_t column = script->column();

    if (mode == XDR_ENCODE) {
      packedFields = lazy->packedFields();
      MOZ_ASSERT(sourceStart == lazy->sourceStart());
      MOZ_ASSERT(sourceEnd == lazy->sourceEnd());
      MOZ_ASSERT(toStringStart == lazy->toStringStart());
      MOZ_ASSERT(toStringEnd == lazy->toStringEnd());
      MOZ_ASSERT(lineno == lazy->lineno());
      MOZ_ASSERT(column == lazy->column());
      // We can assert we have no inner functions because we don't
      // relazify scripts with inner functions.  See
      // JSFunction::createScriptForLazilyInterpretedFunction.
      MOZ_ASSERT(lazy->numInnerFunctions() == 0);
    }
||||||| merged common ancestors
template<XDRMode mode>
static XDRResult
XDRRelazificationInfo(XDRState<mode>* xdr, HandleFunction fun, HandleScript script,
                      HandleScope enclosingScope, MutableHandle<LazyScript*> lazy)
{
    MOZ_ASSERT_IF(mode == XDR_ENCODE, script->isRelazifiable() && script->maybeLazyScript());
    MOZ_ASSERT_IF(mode == XDR_ENCODE, !lazy->numInnerFunctions());

    JSContext* cx = xdr->cx();

    uint64_t packedFields;
    {
        uint32_t sourceStart = script->sourceStart();
        uint32_t sourceEnd = script->sourceEnd();
        uint32_t toStringStart = script->toStringStart();
        uint32_t toStringEnd = script->toStringEnd();
        uint32_t lineno = script->lineno();
        uint32_t column = script->column();

        if (mode == XDR_ENCODE) {
            packedFields = lazy->packedFields();
            MOZ_ASSERT(sourceStart == lazy->sourceStart());
            MOZ_ASSERT(sourceEnd == lazy->sourceEnd());
            MOZ_ASSERT(toStringStart == lazy->toStringStart());
            MOZ_ASSERT(toStringEnd == lazy->toStringEnd());
            MOZ_ASSERT(lineno == lazy->lineno());
            MOZ_ASSERT(column == lazy->column());
            // We can assert we have no inner functions because we don't
            // relazify scripts with inner functions.  See
            // JSFunction::createScriptForLazilyInterpretedFunction.
            MOZ_ASSERT(lazy->numInnerFunctions() == 0);
        }
=======
template <XDRMode mode>
static XDRResult XDRRelazificationInfo(XDRState<mode>* xdr, HandleFunction fun,
                                       HandleScript script,
                                       HandleScope enclosingScope,
                                       MutableHandle<LazyScript*> lazy) {
  MOZ_ASSERT_IF(mode == XDR_ENCODE, script->isRelazifiableIgnoringJitCode() &&
                                        script->maybeLazyScript());
  MOZ_ASSERT_IF(mode == XDR_ENCODE, !lazy->numInnerFunctions());

  JSContext* cx = xdr->cx();

  uint32_t immutableFlags;
  uint32_t numClosedOverBindings;
  {
    uint32_t sourceStart = script->sourceStart();
    uint32_t sourceEnd = script->sourceEnd();
    uint32_t toStringStart = script->toStringStart();
    uint32_t toStringEnd = script->toStringEnd();
    uint32_t lineno = script->lineno();
    uint32_t column = script->column();
    uint32_t numFieldInitializers;
>>>>>>> upstream-releases

<<<<<<< HEAD
    MOZ_TRY(xdr->codeUint64(&packedFields));
||||||| merged common ancestors
        MOZ_TRY(xdr->codeUint64(&packedFields));
=======
    if (mode == XDR_ENCODE) {
      immutableFlags = lazy->immutableFlags();
      numClosedOverBindings = lazy->numClosedOverBindings();
      MOZ_ASSERT(sourceStart == lazy->sourceStart());
      MOZ_ASSERT(sourceEnd == lazy->sourceEnd());
      MOZ_ASSERT(toStringStart == lazy->toStringStart());
      MOZ_ASSERT(toStringEnd == lazy->toStringEnd());
      MOZ_ASSERT(lineno == lazy->lineno());
      MOZ_ASSERT(column == lazy->column());
      // We can assert we have no inner functions because we don't
      // relazify scripts with inner functions.  See
      // JSFunction::createScriptForLazilyInterpretedFunction.
      MOZ_ASSERT(lazy->numInnerFunctions() == 0);
      if (fun->kind() == JSFunction::FunctionKind::ClassConstructor) {
        numFieldInitializers =
            (uint32_t)lazy->getFieldInitializers().numFieldInitializers;
      } else {
        numFieldInitializers = UINT32_MAX;
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (mode == XDR_DECODE) {
      RootedScriptSourceObject sourceObject(cx, &script->scriptSourceUnwrap());
      lazy.set(LazyScript::CreateForXDR(
          cx, fun, script, enclosingScope, sourceObject, packedFields,
          sourceStart, sourceEnd, toStringStart, lineno, column));
      if (!lazy) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }
||||||| merged common ancestors
        if (mode == XDR_DECODE) {
            RootedScriptSourceObject sourceObject(cx, &script->scriptSourceUnwrap());
            lazy.set(LazyScript::CreateForXDR(cx, fun, script, enclosingScope, sourceObject,
                                              packedFields, sourceStart, sourceEnd, toStringStart,
                                              lineno, column));
            if (!lazy) {
                return xdr->fail(JS::TranscodeResult_Throw);
            }
=======
    MOZ_TRY(xdr->codeUint32(&immutableFlags));
    MOZ_TRY(xdr->codeUint32(&numFieldInitializers));
    MOZ_TRY(xdr->codeUint32(&numClosedOverBindings));
>>>>>>> upstream-releases

<<<<<<< HEAD
      lazy->setToStringEnd(toStringEnd);
||||||| merged common ancestors
            lazy->setToStringEnd(toStringEnd);
=======
    if (mode == XDR_DECODE) {
      RootedScriptSourceObject sourceObject(cx, script->sourceObject());
      lazy.set(LazyScript::CreateForXDR(
          cx, numClosedOverBindings, /* numInnerFunctions = */ 0, fun, script,
          enclosingScope, sourceObject, immutableFlags, sourceStart, sourceEnd,
          toStringStart, toStringEnd, lineno, column));
      if (!lazy) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // As opposed to XDRLazyScript, we need to restore the runtime bits
      // of the script, as we are trying to match the fact this function
      // has already been parsed and that it would need to be re-lazified.
      lazy->initRuntimeFields(packedFields);
||||||| merged common ancestors
            // As opposed to XDRLazyScript, we need to restore the runtime bits
            // of the script, as we are trying to match the fact this function
            // has already been parsed and that it would need to be re-lazified.
            lazy->initRuntimeFields(packedFields);
        }
=======
      if (numFieldInitializers != UINT32_MAX) {
        lazy->setFieldInitializers(
            FieldInitializers((size_t)numFieldInitializers));
      }
>>>>>>> upstream-releases
    }
  }

  // Code binding names.
  MOZ_TRY(XDRLazyClosedOverBindings(xdr, lazy));

  // No need to do anything with inner functions, since we asserted we don't
  // have any.

  return Ok();
}

<<<<<<< HEAD
static inline uint32_t FindScopeIndex(JSScript* script, Scope& scope) {
  auto scopes = script->scopes();
  unsigned length = scopes.size();
  for (uint32_t i = 0; i < length; ++i) {
    if (scopes[i] == &scope) {
      return i;
    }
  }
||||||| merged common ancestors
static inline uint32_t
FindScopeIndex(JSScript* script, Scope& scope)
{
    auto scopes = script->scopes();
    unsigned length = scopes.size();
    for (uint32_t i = 0; i < length; ++i) {
        if (scopes[i] == &scope) {
            return i;
        }
    }
=======
template <XDRMode mode>
XDRResult JSTryNote::XDR(XDRState<mode>* xdr) {
  MOZ_TRY(xdr->codeUint32(&kind));
  MOZ_TRY(xdr->codeUint32(&stackDepth));
  MOZ_TRY(xdr->codeUint32(&start));
  MOZ_TRY(xdr->codeUint32(&length));
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_CRASH("Scope not found");
||||||| merged common ancestors
    MOZ_CRASH("Scope not found");
=======
  return Ok();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
enum XDRClassKind { CK_RegexpObject, CK_JSFunction, CK_JSObject };

template <XDRMode mode>
XDRResult js::XDRScript(XDRState<mode>* xdr, HandleScope scriptEnclosingScope,
                        HandleScriptSourceObject sourceObjectArg,
                        HandleFunction fun, MutableHandleScript scriptp) {
  using ImmutableFlags = JSScript::ImmutableFlags;

  /* NB: Keep this in sync with CopyScript. */
||||||| merged common ancestors
enum XDRClassKind {
    CK_RegexpObject,
    CK_JSFunction,
    CK_JSObject
};

template<XDRMode mode>
XDRResult
js::XDRScript(XDRState<mode>* xdr, HandleScope scriptEnclosingScope,
              HandleScriptSourceObject sourceObjectArg, HandleFunction fun,
              MutableHandleScript scriptp)
{
    /* NB: Keep this in sync with CopyScript. */

    enum ScriptBits {
        NoScriptRval,
        Strict,
        ContainsDynamicNameAccess,
        FunHasExtensibleScope,
        FunHasAnyAliasedFormal,
        ArgumentsHasVarBinding,
        NeedsArgsObj,
        HasMappedArgsObj,
        FunctionHasThisBinding,
        FunctionHasExtraBodyVarScope,
        IsGenerator,
        IsAsync,
        HasRest,
        OwnSource,
        ExplicitUseStrict,
        SelfHosted,
        HasSingleton,
        TreatAsRunOnce,
        HasLazyScript,
        HasNonSyntacticScope,
        HasInnerFunctions,
        NeedsHomeObject,
        IsDerivedClassConstructor,
        IsDefaultClassConstructor,
    };

    uint32_t length, lineno, column, nfixed, nslots;
    uint32_t natoms, nsrcnotes;
    uint32_t nconsts, nobjects, nscopes, nregexps, ntrynotes, nscopenotes, nyieldoffsets;
    uint32_t prologueLength;
    uint32_t funLength = 0;
    uint32_t nTypeSets = 0;
    uint32_t scriptBits = 0;
    uint32_t bodyScopeIndex = 0;

    JSContext* cx = xdr->cx();
    RootedScript script(cx);
    natoms = nsrcnotes = 0;
    nconsts = nobjects = nscopes = nregexps = ntrynotes = nscopenotes = nyieldoffsets = 0;

    if (mode == XDR_ENCODE) {
        script = scriptp.get();
        MOZ_ASSERT(script->functionNonDelazifying() == fun);

        if (!fun && script->treatAsRunOnce() && script->hasRunOnce()) {
            // This is a toplevel or eval script that's runOnce.  We want to
            // make sure that we're not XDR-saving an object we emitted for
            // JSOP_OBJECT that then got modified.  So throw if we're not
            // cloning in JSOP_OBJECT or if we ever didn't clone in it in the
            // past.
            Realm* realm = cx->realm();
            if (!realm->creationOptions().cloneSingletons() ||
                !realm->behaviors().getSingletonsAsTemplates())
            {
                return xdr->fail(JS::TranscodeResult_Failure_RunOnceNotSupported);
            }
        }
    }
=======
template <XDRMode mode>
XDRResult ScopeNote::XDR(XDRState<mode>* xdr) {
  MOZ_TRY(xdr->codeUint32(&index));
  MOZ_TRY(xdr->codeUint32(&start));
  MOZ_TRY(xdr->codeUint32(&length));
  MOZ_TRY(xdr->codeUint32(&parent));

  return Ok();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  enum ScriptBits {
    NeedsArgsObj,
    OwnSource,
    HasLazyScript,
  };
||||||| merged common ancestors
    if (mode == XDR_ENCODE) {
        length = script->length();
    }
    MOZ_TRY(xdr->codeUint32(&length));
=======
static inline uint32_t FindScopeIndex(mozilla::Span<const JS::GCCellPtr> scopes,
                                      Scope& scope) {
  unsigned length = scopes.size();
  for (uint32_t i = 0; i < length; ++i) {
    if (scopes[i].asCell() == &scope) {
      return i;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t length, lineno, column, nfixed, nslots;
  uint32_t natoms, nsrcnotes;
  uint32_t nconsts, nobjects, nscopes, nregexps, ntrynotes, nscopenotes,
      nresumeoffsets;
  uint32_t prologueLength;
  uint32_t funLength = 0;
  uint32_t nTypeSets = 0;
  uint32_t scriptBits = 0;
  uint32_t bodyScopeIndex = 0;
  uint32_t immutableFlags = 0;
||||||| merged common ancestors
    if (mode == XDR_ENCODE) {
        prologueLength = script->mainOffset();
        lineno = script->lineno();
        column = script->column();
        nfixed = script->nfixed();
        nslots = script->nslots();
=======
  MOZ_CRASH("Scope not found");
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSContext* cx = xdr->cx();
  RootedScript script(cx);
  natoms = nsrcnotes = 0;
  nconsts = nobjects = nscopes = nregexps = ntrynotes = nscopenotes =
      nresumeoffsets = 0;
||||||| merged common ancestors
        bodyScopeIndex = script->bodyScopeIndex();
        natoms = script->natoms();
=======
template <XDRMode mode>
static XDRResult XDRInnerObject(XDRState<mode>* xdr,
                                js::PrivateScriptData* data,
                                HandleScriptSourceObject sourceObject,
                                MutableHandleObject inner) {
  enum class ClassKind { RegexpObject, JSFunction, JSObject, ArrayObject };
  JSContext* cx = xdr->cx();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mode == XDR_ENCODE) {
    script = scriptp.get();
    MOZ_ASSERT(script->functionNonDelazifying() == fun);
||||||| merged common ancestors
        nsrcnotes = script->numNotes();
=======
  ClassKind classk;
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!fun && script->treatAsRunOnce() && script->hasRunOnce()) {
      // This is a toplevel or eval script that's runOnce.  We want to
      // make sure that we're not XDR-saving an object we emitted for
      // JSOP_OBJECT that then got modified.  So throw if we're not
      // cloning in JSOP_OBJECT or if we ever didn't clone in it in the
      // past.
      Realm* realm = cx->realm();
      if (!realm->creationOptions().cloneSingletons() ||
          !realm->behaviors().getSingletonsAsTemplates()) {
        return xdr->fail(JS::TranscodeResult_Failure_RunOnceNotSupported);
      }
    }
  }

  if (mode == XDR_ENCODE) {
    length = script->length();
  }
  MOZ_TRY(xdr->codeUint32(&length));

  if (mode == XDR_ENCODE) {
    prologueLength = script->mainOffset();
    lineno = script->lineno();
    column = script->column();
    nfixed = script->nfixed();
    nslots = script->nslots();

    bodyScopeIndex = script->bodyScopeIndex();
    natoms = script->natoms();

    immutableFlags = script->immutableFlags_;

    nsrcnotes = script->numNotes();

    nscopes = script->scopes().size();
    if (script->hasConsts()) {
      nconsts = script->consts().size();
    }
    if (script->hasObjects()) {
      nobjects = script->objects().size();
    }
    if (script->hasTrynotes()) {
      ntrynotes = script->trynotes().size();
    }
    if (script->hasScopeNotes()) {
      nscopenotes = script->scopeNotes().size();
    }
    if (script->hasResumeOffsets()) {
      nresumeoffsets = script->resumeOffsets().size();
    }

    nTypeSets = script->nTypeSets();
    funLength = script->funLength();

    if (script->analyzedArgsUsage() && script->needsArgsObj()) {
      scriptBits |= (1 << NeedsArgsObj);
    }
    MOZ_ASSERT_IF(sourceObjectArg,
                  sourceObjectArg->source() == script->scriptSource());
    if (!sourceObjectArg) {
      scriptBits |= (1 << OwnSource);
    }
    if (script->isRelazifiableIgnoringJitCode()) {
      scriptBits |= (1 << HasLazyScript);
    }
  }

  MOZ_TRY(xdr->codeUint32(&prologueLength));

  // To fuse allocations, we need lengths of all embedded arrays early.
  MOZ_TRY(xdr->codeUint32(&natoms));
  MOZ_TRY(xdr->codeUint32(&nsrcnotes));
  MOZ_TRY(xdr->codeUint32(&nconsts));
  MOZ_TRY(xdr->codeUint32(&nobjects));
  MOZ_TRY(xdr->codeUint32(&nscopes));
  MOZ_TRY(xdr->codeUint32(&ntrynotes));
  MOZ_TRY(xdr->codeUint32(&nscopenotes));
  MOZ_TRY(xdr->codeUint32(&nresumeoffsets));
  MOZ_TRY(xdr->codeUint32(&nTypeSets));
  MOZ_TRY(xdr->codeUint32(&funLength));
  MOZ_TRY(xdr->codeUint32(&scriptBits));
  MOZ_TRY(xdr->codeUint32(&immutableFlags));

  MOZ_ASSERT(!!(scriptBits & (1 << OwnSource)) == !sourceObjectArg);
  RootedScriptSourceObject sourceObject(cx, sourceObjectArg);

  if (mode == XDR_DECODE) {
    // When loading from the bytecode cache, we get the CompileOptions from
    // the document. If the noScriptRval or selfHostingMode flag doesn't
    // match, we should fail. This only applies to the top-level and not
    // its inner functions.
    bool noScriptRval =
        !!(immutableFlags & uint32_t(ImmutableFlags::NoScriptRval));
    bool selfHosted = !!(immutableFlags & uint32_t(ImmutableFlags::SelfHosted));
    mozilla::Maybe<CompileOptions> options;
    if (xdr->hasOptions() && (scriptBits & (1 << OwnSource))) {
      options.emplace(xdr->cx(), xdr->options());
      if (options->noScriptRval != noScriptRval ||
          options->selfHostingMode != selfHosted) {
        return xdr->fail(JS::TranscodeResult_Failure_WrongCompileOption);
      }
    } else {
      options.emplace(xdr->cx());
      (*options).setNoScriptRval(noScriptRval).setSelfHostingMode(selfHosted);
    }
||||||| merged common ancestors
        if (script->hasConsts()) {
            nconsts = script->consts().size();
        }
        if (script->hasObjects()) {
            nobjects = script->objects().size();
        }
        nscopes = script->scopes().size();
        if (script->hasTrynotes()) {
            ntrynotes = script->trynotes().size();
        }
        if (script->hasScopeNotes()) {
            nscopenotes = script->scopeNotes().size();
        }
        if (script->hasYieldAndAwaitOffsets()) {
            nyieldoffsets = script->yieldAndAwaitOffsets().size();
        }
=======
  if (mode == XDR_ENCODE) {
    if (inner->is<RegExpObject>()) {
      classk = ClassKind::RegexpObject;
    } else if (inner->is<JSFunction>()) {
      classk = ClassKind::JSFunction;
    } else if (inner->is<PlainObject>()) {
      classk = ClassKind::JSObject;
    } else if (inner->is<ArrayObject>()) {
      classk = ClassKind::ArrayObject;
    } else {
      MOZ_CRASH("Cannot encode this class of object.");
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (scriptBits & (1 << OwnSource)) {
      ScriptSource* ss = cx->new_<ScriptSource>();
      if (!ss) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }
      ScriptSourceHolder ssHolder(ss);

      /*
       * We use this CompileOptions only to initialize the
       * ScriptSourceObject. Most CompileOptions fields aren't used by
       * ScriptSourceObject, and those that are (element; elementAttributeName)
       * aren't preserved by XDR. So this can be simple.
       */
      if (!ss->initFromOptions(cx, *options)) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }
||||||| merged common ancestors
        nTypeSets = script->nTypeSets();
        funLength = script->funLength();
=======
  MOZ_TRY(xdr->codeEnum32(&classk));
>>>>>>> upstream-releases

<<<<<<< HEAD
      sourceObject = ScriptSourceObject::create(cx, ss);
      if (!sourceObject) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }
||||||| merged common ancestors
        if (script->noScriptRval()) {
            scriptBits |= (1 << NoScriptRval);
        }
        if (script->strict()) {
            scriptBits |= (1 << Strict);
        }
        if (script->explicitUseStrict()) {
            scriptBits |= (1 << ExplicitUseStrict);
        }
        if (script->selfHosted()) {
            scriptBits |= (1 << SelfHosted);
        }
        if (script->bindingsAccessedDynamically()) {
            scriptBits |= (1 << ContainsDynamicNameAccess);
        }
        if (script->funHasExtensibleScope()) {
            scriptBits |= (1 << FunHasExtensibleScope);
        }
        if (script->funHasAnyAliasedFormal()) {
            scriptBits |= (1 << FunHasAnyAliasedFormal);
        }
        if (script->argumentsHasVarBinding()) {
            scriptBits |= (1 << ArgumentsHasVarBinding);
        }
        if (script->analyzedArgsUsage() && script->needsArgsObj()) {
            scriptBits |= (1 << NeedsArgsObj);
        }
        if (script->hasMappedArgsObj()) {
            scriptBits |= (1 << HasMappedArgsObj);
        }
        if (script->functionHasThisBinding()) {
            scriptBits |= (1 << FunctionHasThisBinding);
        }
        if (script->functionHasExtraBodyVarScope()) {
            scriptBits |= (1 << FunctionHasExtraBodyVarScope);
        }
        MOZ_ASSERT_IF(sourceObjectArg, sourceObjectArg->source() == script->scriptSource());
        if (!sourceObjectArg) {
            scriptBits |= (1 << OwnSource);
        }
        if (script->isGenerator()) {
            scriptBits |= (1 << IsGenerator);
        }
        if (script->isAsync()) {
            scriptBits |= (1 << IsAsync);
        }
        if (script->hasRest()) {
            scriptBits |= (1 << HasRest);
        }
        if (script->hasSingletons()) {
            scriptBits |= (1 << HasSingleton);
        }
        if (script->treatAsRunOnce()) {
            scriptBits |= (1 << TreatAsRunOnce);
        }
        if (script->isRelazifiable()) {
            scriptBits |= (1 << HasLazyScript);
        }
        if (script->hasNonSyntacticScope()) {
            scriptBits |= (1 << HasNonSyntacticScope);
        }
        if (script->hasInnerFunctions()) {
            scriptBits |= (1 << HasInnerFunctions);
        }
        if (script->needsHomeObject()) {
            scriptBits |= (1 << NeedsHomeObject);
        }
        if (script->isDerivedClassConstructor()) {
            scriptBits |= (1 << IsDerivedClassConstructor);
        }
        if (script->isDefaultClassConstructor()) {
            scriptBits |= (1 << IsDefaultClassConstructor);
        }
    }
=======
  switch (classk) {
    case ClassKind::RegexpObject: {
      Rooted<RegExpObject*> regexp(cx);
      if (mode == XDR_ENCODE) {
        regexp = &inner->as<RegExpObject>();
      }
      MOZ_TRY(XDRScriptRegExpObject(xdr, &regexp));
      if (mode == XDR_DECODE) {
        inner.set(regexp);
      }
      break;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (xdr->hasScriptSourceObjectOut()) {
        // When the ScriptSourceObjectOut is provided by ParseTask, it
        // is stored in a location which is traced by the GC.
        *xdr->scriptSourceObjectOut() = sourceObject;
      } else if (!ScriptSourceObject::initFromOptions(cx, sourceObject,
                                                      *options)) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }
    }

    script = JSScript::Create(cx, *options, sourceObject, 0, 0, 0, 0);
    if (!script) {
      return xdr->fail(JS::TranscodeResult_Throw);
    }
||||||| merged common ancestors
    MOZ_TRY(xdr->codeUint32(&prologueLength));

    // To fuse allocations, we need lengths of all embedded arrays early.
    MOZ_TRY(xdr->codeUint32(&natoms));
    MOZ_TRY(xdr->codeUint32(&nsrcnotes));
    MOZ_TRY(xdr->codeUint32(&nconsts));
    MOZ_TRY(xdr->codeUint32(&nobjects));
    MOZ_TRY(xdr->codeUint32(&nscopes));
    MOZ_TRY(xdr->codeUint32(&ntrynotes));
    MOZ_TRY(xdr->codeUint32(&nscopenotes));
    MOZ_TRY(xdr->codeUint32(&nyieldoffsets));
    MOZ_TRY(xdr->codeUint32(&nTypeSets));
    MOZ_TRY(xdr->codeUint32(&funLength));
    MOZ_TRY(xdr->codeUint32(&scriptBits));
=======
    case ClassKind::JSFunction: {
      /* Code the nested function's enclosing scope. */
      uint32_t funEnclosingScopeIndex = 0;
      RootedScope funEnclosingScope(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Set the script in its function now so that inner scripts to be
    // decoded may iterate the static scope chain.
    if (fun) {
      fun->initScript(script);
    }
  } else {
    // When encoding, we do not mutate any of the JSScript or LazyScript, so
    // we can safely unwrap it here.
    sourceObject = &script->scriptSourceUnwrap();
  }
||||||| merged common ancestors
    MOZ_ASSERT(!!(scriptBits & (1 << OwnSource)) == !sourceObjectArg);
    RootedScriptSourceObject sourceObject(cx, sourceObjectArg);
=======
      if (mode == XDR_ENCODE) {
        RootedFunction function(cx, &inner->as<JSFunction>());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mode == XDR_DECODE) {
    if (!JSScript::createPrivateScriptData(cx, script, nscopes, nconsts,
                                           nobjects, ntrynotes, nscopenotes,
                                           nresumeoffsets)) {
      return xdr->fail(JS::TranscodeResult_Throw);
    }
||||||| merged common ancestors
    if (mode == XDR_DECODE) {
        // When loading from the bytecode cache, we get the CompileOptions from
        // the document. If the noScriptRval or selfHostingMode flag doesn't
        // match, we should fail. This only applies to the top-level and not
        // its inner functions.
        mozilla::Maybe<CompileOptions> options;
        if (xdr->hasOptions() && (scriptBits & (1 << OwnSource))) {
            options.emplace(xdr->cx(), xdr->options());
            if (options->noScriptRval != !!(scriptBits & (1 << NoScriptRval)) ||
                options->selfHostingMode != !!(scriptBits & (1 << SelfHosted)))
            {
                return xdr->fail(JS::TranscodeResult_Failure_WrongCompileOption);
            }
        } else {
            options.emplace(xdr->cx());
            (*options).setNoScriptRval(!!(scriptBits & (1 << NoScriptRval)))
                      .setSelfHostingMode(!!(scriptBits & (1 << SelfHosted)));
        }
=======
        if (function->isInterpretedLazy()) {
          funEnclosingScope = function->lazyScript()->enclosingScope();
        } else if (function->isInterpreted()) {
          funEnclosingScope = function->nonLazyScript()->enclosingScope();
        } else {
          MOZ_ASSERT(function->isAsmJSNative());
          return xdr->fail(JS::TranscodeResult_Failure_AsmJSNotSupported);
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
    MOZ_ASSERT(!script->mainOffset());
    script->mainOffset_ = prologueLength;
    script->funLength_ = funLength;

    MOZ_ASSERT(nTypeSets <= UINT16_MAX);
    script->nTypeSets_ = uint16_t(nTypeSets);

    scriptp.set(script);
||||||| merged common ancestors
        if (scriptBits & (1 << OwnSource)) {
            ScriptSource* ss = cx->new_<ScriptSource>();
            if (!ss) {
                return xdr->fail(JS::TranscodeResult_Throw);
            }
            ScriptSourceHolder ssHolder(ss);

            /*
             * We use this CompileOptions only to initialize the
             * ScriptSourceObject. Most CompileOptions fields aren't used by
             * ScriptSourceObject, and those that are (element; elementAttributeName)
             * aren't preserved by XDR. So this can be simple.
             */
            if (!ss->initFromOptions(cx, *options)) {
                return xdr->fail(JS::TranscodeResult_Throw);
            }

            sourceObject = ScriptSourceObject::create(cx, ss);
            if (!sourceObject) {
                return xdr->fail(JS::TranscodeResult_Throw);
            }

            if (xdr->hasScriptSourceObjectOut()) {
                // When the ScriptSourceObjectOut is provided by ParseTask, it
                // is stored in a location which is traced by the GC.
                *xdr->scriptSourceObjectOut() = sourceObject;
            } else if (!ScriptSourceObject::initFromOptions(cx, sourceObject, *options)) {
                return xdr->fail(JS::TranscodeResult_Throw);
            }
        }
=======
        funEnclosingScopeIndex =
            FindScopeIndex(data->gcthings(), *funEnclosingScope);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    script->immutableFlags_ = immutableFlags;
||||||| merged common ancestors
        script = JSScript::Create(cx, *options, sourceObject, 0, 0, 0, 0);
        if (!script) {
            return xdr->fail(JS::TranscodeResult_Throw);
        }
=======
      MOZ_TRY(xdr->codeUint32(&funEnclosingScopeIndex));
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (script->hasFlag(ImmutableFlags::ArgsHasVarBinding)) {
      // Call setArgumentsHasVarBinding to initialize the
      // NeedsArgsAnalysis flag.
      script->setArgumentsHasVarBinding();
    }
    if (scriptBits & (1 << NeedsArgsObj)) {
      script->setNeedsArgsObj(true);
||||||| merged common ancestors
        // Set the script in its function now so that inner scripts to be
        // decoded may iterate the static scope chain.
        if (fun) {
            fun->initScript(script);
        }
    } else {
        // When encoding, we do not mutate any of the JSScript or LazyScript, so
        // we can safely unwrap it here.
        sourceObject = &script->scriptSourceUnwrap();
=======
      if (mode == XDR_DECODE) {
        funEnclosingScope =
            &data->gcthings()[funEnclosingScopeIndex].as<Scope>();
      }

      // Code nested function and script.
      RootedFunction tmp(cx);
      if (mode == XDR_ENCODE) {
        tmp = &inner->as<JSFunction>();
      }
      MOZ_TRY(
          XDRInterpretedFunction(xdr, funEnclosingScope, sourceObject, &tmp));
      if (mode == XDR_DECODE) {
        inner.set(tmp);
      }
      break;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  JS_STATIC_ASSERT(sizeof(jsbytecode) == 1);
  JS_STATIC_ASSERT(sizeof(jssrcnote) == 1);
||||||| merged common ancestors
    if (mode == XDR_DECODE) {
        if (!JSScript::partiallyInit(cx, script, nscopes, nconsts, nobjects, ntrynotes,
                                     nscopenotes, nyieldoffsets))
        {
            return xdr->fail(JS::TranscodeResult_Throw);
        }
=======
    case ClassKind::JSObject:
    case ClassKind::ArrayObject: {
      /* Code object literal. */
      RootedObject tmp(cx);
      if (mode == XDR_ENCODE) {
        tmp = inner.get();
      }
      MOZ_TRY(XDRObjectLiteral(xdr, &tmp));
      if (mode == XDR_DECODE) {
        inner.set(tmp);
      }
      break;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (scriptBits & (1 << OwnSource)) {
    MOZ_TRY(sourceObject->source()->performXDR<mode>(xdr));
  }
  MOZ_TRY(xdr->codeUint32(&script->sourceStart_));
  MOZ_TRY(xdr->codeUint32(&script->sourceEnd_));
  MOZ_TRY(xdr->codeUint32(&script->toStringStart_));
  MOZ_TRY(xdr->codeUint32(&script->toStringEnd_));
  MOZ_TRY(xdr->codeUint32(&lineno));
  MOZ_TRY(xdr->codeUint32(&column));
  MOZ_TRY(xdr->codeUint32(&nfixed));
  MOZ_TRY(xdr->codeUint32(&nslots));
  MOZ_TRY(xdr->codeUint32(&bodyScopeIndex));
||||||| merged common ancestors
        MOZ_ASSERT(!script->mainOffset());
        script->mainOffset_ = prologueLength;
        script->funLength_ = funLength;
=======
    default: {
      // Fail in debug, but only soft-fail in release
      MOZ_ASSERT(false, "Bad XDR class kind");
      return xdr->fail(JS::TranscodeResult_Failure_BadDecode);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mode == XDR_DECODE) {
    script->lineno_ = lineno;
    script->column_ = column;
    script->nfixed_ = nfixed;
    script->nslots_ = nslots;
    script->bodyScopeIndex_ = bodyScopeIndex;
  }
||||||| merged common ancestors
        MOZ_ASSERT(nTypeSets <= UINT16_MAX);
        script->nTypeSets_ = uint16_t(nTypeSets);
=======
  return Ok();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mode == XDR_DECODE) {
    if (!script->createSharedScriptData(cx, length, nsrcnotes, natoms)) {
      return xdr->fail(JS::TranscodeResult_Throw);
    }
  }
||||||| merged common ancestors
        scriptp.set(script);
=======
template <XDRMode mode>
static XDRResult XDRScope(XDRState<mode>* xdr, js::PrivateScriptData* data,
                          HandleScope scriptEnclosingScope, HandleFunction fun,
                          bool isFirstScope, MutableHandleScope scope) {
  JSContext* cx = xdr->cx();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If XDR operation fails, we must call JSScript::freeScriptData in order
  // to neuter the script. Various things that iterate raw scripts in a GC
  // arena use the presense of this data to detect if initialization is
  // complete.
  auto scriptDataGuard = mozilla::MakeScopeExit([&] {
    if (mode == XDR_DECODE) {
      script->freeScriptData();
    }
  });
||||||| merged common ancestors
        if (scriptBits & (1 << Strict)) {
            script->bitFields_.strict_ = true;
        }
        if (scriptBits & (1 << ExplicitUseStrict)) {
            script->bitFields_.explicitUseStrict_ = true;
        }
        if (scriptBits & (1 << ContainsDynamicNameAccess)) {
            script->bitFields_.bindingsAccessedDynamically_ = true;
        }
        if (scriptBits & (1 << FunHasExtensibleScope)) {
            script->bitFields_.funHasExtensibleScope_ = true;
        }
        if (scriptBits & (1 << FunHasAnyAliasedFormal)) {
            script->bitFields_.funHasAnyAliasedFormal_ = true;
        }
        if (scriptBits & (1 << ArgumentsHasVarBinding)) {
            script->setArgumentsHasVarBinding();
        }
        if (scriptBits & (1 << NeedsArgsObj)) {
            script->setNeedsArgsObj(true);
        }
        if (scriptBits & (1 << HasMappedArgsObj)) {
            script->bitFields_.hasMappedArgsObj_ = true;
        }
        if (scriptBits & (1 << FunctionHasThisBinding)) {
            script->bitFields_.functionHasThisBinding_ = true;
        }
        if (scriptBits & (1 << FunctionHasExtraBodyVarScope)) {
            script->bitFields_.functionHasExtraBodyVarScope_ = true;
        }
        if (scriptBits & (1 << HasSingleton)) {
            script->bitFields_.hasSingletons_ = true;
        }
        if (scriptBits & (1 << TreatAsRunOnce)) {
            script->bitFields_.treatAsRunOnce_ = true;
        }
        if (scriptBits & (1 << HasNonSyntacticScope)) {
            script->bitFields_.hasNonSyntacticScope_ = true;
        }
        if (scriptBits & (1 << HasInnerFunctions)) {
            script->bitFields_.hasInnerFunctions_ = true;
        }
        if (scriptBits & (1 << NeedsHomeObject)) {
            script->bitFields_.needsHomeObject_ = true;
        }
        if (scriptBits & (1 << IsDerivedClassConstructor)) {
            script->bitFields_.isDerivedClassConstructor_ = true;
        }
        if (scriptBits & (1 << IsDefaultClassConstructor)) {
            script->bitFields_.isDefaultClassConstructor_ = true;
        }
        if (scriptBits & (1 << IsGenerator)) {
            script->setGeneratorKind(GeneratorKind::Generator);
        }
        if (scriptBits & (1 << IsAsync)) {
            script->setAsyncKind(FunctionAsyncKind::AsyncFunction);
        }
        if (scriptBits & (1 << HasRest)) {
            script->setHasRest();
        }
    }
=======
  ScopeKind scopeKind;
  RootedScope enclosing(cx);
  uint32_t enclosingIndex = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  jsbytecode* code = script->code();
  MOZ_TRY(xdr->codeBytes(code, length));
  MOZ_TRY(xdr->codeBytes(code + length, nsrcnotes));
||||||| merged common ancestors
    JS_STATIC_ASSERT(sizeof(jsbytecode) == 1);
    JS_STATIC_ASSERT(sizeof(jssrcnote) == 1);
=======
  // The enclosingScope is encoded using an integer index into the scope array.
  // This means that scopes must be topologically sorted.
  if (mode == XDR_ENCODE) {
    scopeKind = scope->kind();
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (uint32_t i = 0; i != natoms; ++i) {
    if (mode == XDR_DECODE) {
      RootedAtom tmp(cx);
      MOZ_TRY(XDRAtom(xdr, &tmp));
      script->atoms()[i].init(tmp);
    } else {
      RootedAtom tmp(cx, script->atoms()[i]);
      MOZ_TRY(XDRAtom(xdr, &tmp));
||||||| merged common ancestors
    if (scriptBits & (1 << OwnSource)) {
        MOZ_TRY(sourceObject->source()->performXDR<mode>(xdr));
=======
    if (isFirstScope) {
      enclosingIndex = UINT32_MAX;
    } else {
      MOZ_ASSERT(scope->enclosing());
      enclosingIndex = FindScopeIndex(data->gcthings(), *scope->enclosing());
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (mode == XDR_DECODE) {
    if (!script->shareScriptData(cx)) {
      return xdr->fail(JS::TranscodeResult_Throw);
||||||| merged common ancestors
    if (mode == XDR_DECODE) {
        script->lineno_ = lineno;
        script->column_ = column;
        script->nfixed_ = nfixed;
        script->nslots_ = nslots;
        script->bodyScopeIndex_ = bodyScopeIndex;
=======
  MOZ_TRY(xdr->codeEnum32(&scopeKind));
  MOZ_TRY(xdr->codeUint32(&enclosingIndex));

  if (mode == XDR_DECODE) {
    if (isFirstScope) {
      MOZ_ASSERT(enclosingIndex == UINT32_MAX);
      enclosing = scriptEnclosingScope;
    } else {
      enclosing = &data->gcthings()[enclosingIndex].as<Scope>();
    }
  }

  switch (scopeKind) {
    case ScopeKind::Function:
      MOZ_TRY(FunctionScope::XDR(xdr, fun, enclosing, scope));
      break;
    case ScopeKind::FunctionBodyVar:
    case ScopeKind::ParameterExpressionVar:
      MOZ_TRY(VarScope::XDR(xdr, scopeKind, enclosing, scope));
      break;
    case ScopeKind::Lexical:
    case ScopeKind::SimpleCatch:
    case ScopeKind::Catch:
    case ScopeKind::NamedLambda:
    case ScopeKind::StrictNamedLambda:
      MOZ_TRY(LexicalScope::XDR(xdr, scopeKind, enclosing, scope));
      break;
    case ScopeKind::With:
      MOZ_TRY(WithScope::XDR(xdr, enclosing, scope));
      break;
    case ScopeKind::Eval:
    case ScopeKind::StrictEval:
      MOZ_TRY(EvalScope::XDR(xdr, scopeKind, enclosing, scope));
      break;
    case ScopeKind::Global:
    case ScopeKind::NonSyntactic:
      MOZ_TRY(GlobalScope::XDR(xdr, scopeKind, scope));
      break;
    case ScopeKind::Module:
    case ScopeKind::WasmInstance:
      MOZ_CRASH("NYI");
      break;
    case ScopeKind::WasmFunction:
      MOZ_CRASH("wasm functions cannot be nested in JSScripts");
      break;
    default:
      // Fail in debug, but only soft-fail in release
      MOZ_ASSERT(false, "Bad XDR scope kind");
      return xdr->fail(JS::TranscodeResult_Failure_BadDecode);
  }

  return Ok();
}

template <XDRMode mode>
static XDRResult XDRScriptGCThing(XDRState<mode>* xdr, PrivateScriptData* data,
                                  HandleScriptSourceObject sourceObject,
                                  HandleScope scriptEnclosingScope,
                                  HandleFunction fun, bool* isFirstScope,
                                  JS::GCCellPtr* thingp) {
  JSContext* cx = xdr->cx();

  enum class GCThingTag { Object, Scope, BigInt };

  JS::GCCellPtr thing;

  GCThingTag tag;
  if (mode == XDR_ENCODE) {
    thing = *thingp;
    if (thing.is<JSObject>()) {
      tag = GCThingTag::Object;
    } else if (thing.is<Scope>()) {
      tag = GCThingTag::Scope;
    } else {
      MOZ_ASSERT(thing.is<BigInt>());
      tag = GCThingTag::BigInt;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  js::PrivateScriptData* data = script->data_;

  if (nconsts) {
    RootedValue val(cx);
    for (GCPtrValue& elem : data->consts()) {
      if (mode == XDR_ENCODE) {
        val = elem.get();
      }
      MOZ_TRY(XDRScriptConst(xdr, &val));
      if (mode == XDR_DECODE) {
        elem.init(val);
      }
||||||| merged common ancestors
    if (mode == XDR_DECODE) {
        if (!script->createScriptData(cx, length, nsrcnotes, natoms)) {
            return xdr->fail(JS::TranscodeResult_Throw);
        }
=======
  MOZ_TRY(xdr->codeEnum32(&tag));

  switch (tag) {
    case GCThingTag::Object: {
      RootedObject obj(cx);
      if (mode == XDR_ENCODE) {
        obj = &thing.as<JSObject>();
      }
      MOZ_TRY(XDRInnerObject(xdr, data, sourceObject, &obj));
      if (mode == XDR_DECODE) {
        *thingp = JS::GCCellPtr(obj.get());
      }
      break;
    }
    case GCThingTag::Scope: {
      RootedScope scope(cx);
      if (mode == XDR_ENCODE) {
        scope = &thing.as<Scope>();
      }
      MOZ_TRY(XDRScope(xdr, data, scriptEnclosingScope, fun, *isFirstScope,
                       &scope));
      if (mode == XDR_DECODE) {
        *thingp = JS::GCCellPtr(scope.get());
      }
      *isFirstScope = false;
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
    case GCThingTag::BigInt: {
      RootedBigInt bi(cx);
      if (mode == XDR_ENCODE) {
        bi = &thing.as<BigInt>();
      }
      MOZ_TRY(XDRBigInt(xdr, &bi));
      if (mode == XDR_DECODE) {
        *thingp = JS::GCCellPtr(bi.get());
      }
      break;
    }
    default:
      // Fail in debug, but only soft-fail in release.
      MOZ_ASSERT(false, "Bad XDR GCThingTag");
      return xdr->fail(JS::TranscodeResult_Failure_BadDecode);
  }
  return Ok();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  {
    MOZ_ASSERT(nscopes != 0);
    GCPtrScope* vector = data->scopes().data();
    RootedScope scope(cx);
    RootedScope enclosing(cx);
    ScopeKind scopeKind;
    uint32_t enclosingScopeIndex = 0;
    for (uint32_t i = 0; i != nscopes; ++i) {
      if (mode == XDR_ENCODE) {
        scope = vector[i];
        scopeKind = scope->kind();
      } else {
        scope = nullptr;
      }

      MOZ_TRY(xdr->codeEnum32(&scopeKind));

      if (mode == XDR_ENCODE) {
        if (i == 0) {
          enclosingScopeIndex = UINT32_MAX;
        } else {
          MOZ_ASSERT(scope->enclosing());
          enclosingScopeIndex = FindScopeIndex(script, *scope->enclosing());
        }
      }
||||||| merged common ancestors
    // If XDR operation fails, we must call JSScript::freeScriptData in order
    // to neuter the script. Various things that iterate raw scripts in a GC
    // arena use the presense of this data to detect if initialization is
    // complete.
    auto scriptDataGuard = mozilla::MakeScopeExit([&] {
        if (mode == XDR_DECODE) {
            script->freeScriptData();
        }
    });
=======
template <XDRMode mode>
/* static */
XDRResult js::PrivateScriptData::XDR(XDRState<mode>* xdr, HandleScript script,
                                     HandleScriptSourceObject sourceObject,
                                     HandleScope scriptEnclosingScope,
                                     HandleFunction fun) {
  uint32_t ngcthings = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
      MOZ_TRY(xdr->codeUint32(&enclosingScopeIndex));
||||||| merged common ancestors
    jsbytecode* code = script->code();
    MOZ_TRY(xdr->codeBytes(code, length));
    MOZ_TRY(xdr->codeBytes(code + length, nsrcnotes));
=======
  JSContext* cx = xdr->cx();
  PrivateScriptData* data = nullptr;

  if (mode == XDR_ENCODE) {
    data = script->data_;

    ngcthings = data->gcthings().size();
  }

  MOZ_TRY(xdr->codeUint32(&ngcthings));

  if (mode == XDR_DECODE) {
    if (!JSScript::createPrivateScriptData(cx, script, ngcthings)) {
      return xdr->fail(JS::TranscodeResult_Throw);
    }

    data = script->data_;
  }

  bool isFirstScope = true;
  for (JS::GCCellPtr& gcThing : data->gcthings()) {
    MOZ_TRY(XDRScriptGCThing(xdr, data, sourceObject, scriptEnclosingScope, fun,
                             &isFirstScope, &gcThing));
  }

  // Verify marker to detect data corruption after decoding GC things. A
  // mismatch here indicates we will almost certainly crash in release.
  MOZ_TRY(xdr->codeMarker(0xF83B989A));

  return Ok();
}

/* static */ size_t SharedScriptData::AllocationSize(
    uint32_t codeLength, uint32_t noteLength, uint32_t natoms,
    uint32_t numResumeOffsets, uint32_t numScopeNotes, uint32_t numTryNotes) {
  size_t size = sizeof(SharedScriptData);

  size += natoms * sizeof(GCPtrAtom);
  size += sizeof(Flags);
  size += codeLength * sizeof(jsbytecode);
  size += noteLength * sizeof(jssrcnote);

  unsigned numOptionalArrays = unsigned(numResumeOffsets > 0) +
                               unsigned(numScopeNotes > 0) +
                               unsigned(numTryNotes > 0);
  size += numOptionalArrays * sizeof(uint32_t);

  size += numResumeOffsets * sizeof(uint32_t);
  size += numScopeNotes * sizeof(ScopeNote);
  size += numTryNotes * sizeof(JSTryNote);

  return size;
}

// Placement-new elements of an array. This should optimize away for types with
// trivial default initiation.
template <typename T>
void SharedScriptData::initElements(size_t offset, size_t length) {
  uintptr_t base = reinterpret_cast<uintptr_t>(this);
  DefaultInitializeElements<T>(reinterpret_cast<void*>(base + offset), length);
}

// Initialize the optional arrays in the trailing allocation. This is a set of
// offsets that delimit each optional array followed by the arrays themselves.
// See comment before 'SharedScriptData' for more details.
void SharedScriptData::initOptionalArrays(size_t* pcursor,
                                          SharedScriptData::Flags* flags,
                                          uint32_t numResumeOffsets,
                                          uint32_t numScopeNotes,
                                          uint32_t numTryNotes) {
  size_t cursor = (*pcursor);

  // The byte arrays must have already been padded.
  MOZ_ASSERT(cursor % sizeof(uint32_t) == 0);

  // Each non-empty optional array needs will need an offset to its end.
  unsigned numOptionalArrays = unsigned(numResumeOffsets > 0) +
                               unsigned(numScopeNotes > 0) +
                               unsigned(numTryNotes > 0);

  // Default-initialize the optional-offsets.
  static_assert(alignof(SharedScriptData) >= alignof(uint32_t),
                "Incompatible alignment");
  initElements<uint32_t>(cursor, numOptionalArrays);
  cursor += numOptionalArrays * sizeof(uint32_t);

  // Offset between optional-offsets table and the optional arrays. This is
  // later used to access the optional-offsets table as well as first optional
  // array.
  optArrayOffset_ = cursor;

  // Each optional array that follows must store an end-offset in the offset
  // table. Assign table entries by using this 'offsetIndex'. The index 0 is
  // reserved for implicit value 'optArrayOffset'.
  int offsetIndex = 0;

  // Default-initialize optional 'resumeOffsets'.
  MOZ_ASSERT(resumeOffsetsOffset() == cursor);
  if (numResumeOffsets > 0) {
    static_assert(sizeof(uint32_t) >= alignof(uint32_t),
                  "Incompatible alignment");
    initElements<uint32_t>(cursor, numResumeOffsets);
    cursor += numResumeOffsets * sizeof(uint32_t);
    setOptionalOffset(++offsetIndex, cursor);
  }
  flags->resumeOffsetsEndIndex = offsetIndex;

  // Default-initialize optional 'scopeNotes'.
  MOZ_ASSERT(scopeNotesOffset() == cursor);
  if (numScopeNotes > 0) {
    static_assert(sizeof(uint32_t) >= alignof(ScopeNote),
                  "Incompatible alignment");
    initElements<ScopeNote>(cursor, numScopeNotes);
    cursor += numScopeNotes * sizeof(ScopeNote);
    setOptionalOffset(++offsetIndex, cursor);
  }
  flags->scopeNotesEndIndex = offsetIndex;

  // Default-initialize optional 'tryNotes'
  MOZ_ASSERT(tryNotesOffset() == cursor);
  if (numTryNotes > 0) {
    static_assert(sizeof(ScopeNote) >= alignof(JSTryNote),
                  "Incompatible alignment");
    initElements<JSTryNote>(cursor, numTryNotes);
    cursor += numTryNotes * sizeof(JSTryNote);
    setOptionalOffset(++offsetIndex, cursor);
  }
  flags->tryNotesEndIndex = offsetIndex;

  MOZ_ASSERT(endOffset() == cursor);
  (*pcursor) = cursor;
}

SharedScriptData::SharedScriptData(uint32_t codeLength, uint32_t noteLength,
                                   uint32_t natoms, uint32_t numResumeOffsets,
                                   uint32_t numScopeNotes, uint32_t numTryNotes)
    : codeLength_(codeLength) {
  // Variable-length data begins immediately after SharedScriptData itself.
  size_t cursor = sizeof(*this);

  // Default-initialize 'atoms'
  static_assert(alignof(SharedScriptData) >= alignof(GCPtrAtom),
                "Incompatible alignment");
  initElements<GCPtrAtom>(cursor, natoms);
  cursor += natoms * sizeof(GCPtrAtom);

  // The following arrays are byte-aligned with additional padding to ensure
  // that together they maintain uint32_t-alignment.
  {
    MOZ_ASSERT(cursor % CodeNoteAlign == 0);

    // Zero-initialize 'flags'
    static_assert(CodeNoteAlign >= alignof(Flags), "Incompatible alignment");
    new (offsetToPointer<void>(cursor)) Flags{};
    cursor += sizeof(Flags);

    static_assert(alignof(Flags) >= alignof(jsbytecode),
                  "Incompatible alignment");
    codeOffset_ = cursor;
    initElements<jsbytecode>(cursor, codeLength);
    cursor += codeLength * sizeof(jsbytecode);

    static_assert(alignof(jsbytecode) >= alignof(jssrcnote),
                  "Incompatible alignment");
    initElements<jssrcnote>(cursor, noteLength);
    cursor += noteLength * sizeof(jssrcnote);

    MOZ_ASSERT(cursor % CodeNoteAlign == 0);
  }

  // Initialization for remaining arrays.
  initOptionalArrays(&cursor, &flagsRef(), numResumeOffsets, numScopeNotes,
                     numTryNotes);

  // Check that we correctly recompute the expected values.
  MOZ_ASSERT(this->natoms() == natoms);
  MOZ_ASSERT(this->codeLength() == codeLength);
  MOZ_ASSERT(this->noteLength() == noteLength);

  // Sanity check
  MOZ_ASSERT(AllocationSize(codeLength, noteLength, natoms, numResumeOffsets,
                            numScopeNotes, numTryNotes) == cursor);
}

template <XDRMode mode>
/* static */
XDRResult SharedScriptData::XDR(XDRState<mode>* xdr, HandleScript script) {
  uint32_t natoms = 0;
  uint32_t codeLength = 0;
  uint32_t noteLength = 0;
  uint32_t numResumeOffsets = 0;
  uint32_t numScopeNotes = 0;
  uint32_t numTryNotes = 0;

  JSContext* cx = xdr->cx();
  SharedScriptData* ssd = nullptr;

  if (mode == XDR_ENCODE) {
    ssd = script->scriptData();

    natoms = ssd->natoms();
    codeLength = ssd->codeLength();
    noteLength = ssd->noteLength();

    numResumeOffsets = ssd->resumeOffsets().size();
    numScopeNotes = ssd->scopeNotes().size();
    numTryNotes = ssd->tryNotes().size();
  }

  MOZ_TRY(xdr->codeUint32(&natoms));
  MOZ_TRY(xdr->codeUint32(&codeLength));
  MOZ_TRY(xdr->codeUint32(&noteLength));
  MOZ_TRY(xdr->codeUint32(&numResumeOffsets));
  MOZ_TRY(xdr->codeUint32(&numScopeNotes));
  MOZ_TRY(xdr->codeUint32(&numTryNotes));

  if (mode == XDR_DECODE) {
    if (!script->createSharedScriptData(cx, codeLength, noteLength, natoms,
                                        numResumeOffsets, numScopeNotes,
                                        numTryNotes)) {
      return xdr->fail(JS::TranscodeResult_Throw);
    }
    ssd = script->scriptData();
  }

  MOZ_TRY(xdr->codeUint32(&ssd->mainOffset));
  MOZ_TRY(xdr->codeUint32(&ssd->nfixed));
  MOZ_TRY(xdr->codeUint32(&ssd->nslots));
  MOZ_TRY(xdr->codeUint32(&ssd->bodyScopeIndex));
  MOZ_TRY(xdr->codeUint32(&ssd->numICEntries));
  MOZ_TRY(xdr->codeUint16(&ssd->funLength));
  MOZ_TRY(xdr->codeUint16(&ssd->numBytecodeTypeSets));

  JS_STATIC_ASSERT(sizeof(jsbytecode) == 1);
  JS_STATIC_ASSERT(sizeof(jssrcnote) == 1);

  jsbytecode* code = ssd->code();
  jssrcnote* notes = ssd->notes();
  MOZ_TRY(xdr->codeBytes(code, codeLength));
  MOZ_TRY(xdr->codeBytes(notes, noteLength));

  {
    RootedAtom atom(cx);
    GCPtrAtom* vector = ssd->atoms();
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (mode == XDR_DECODE) {
        if (i == 0) {
          MOZ_ASSERT(enclosingScopeIndex == UINT32_MAX);
          enclosing = scriptEnclosingScope;
        } else {
          MOZ_ASSERT(enclosingScopeIndex < i);
          enclosing = vector[enclosingScopeIndex];
        }
      }

      switch (scopeKind) {
        case ScopeKind::Function:
          MOZ_ASSERT(i == script->bodyScopeIndex());
          MOZ_TRY(FunctionScope::XDR(xdr, fun, enclosing, &scope));
          break;
        case ScopeKind::FunctionBodyVar:
        case ScopeKind::ParameterExpressionVar:
          MOZ_TRY(VarScope::XDR(xdr, scopeKind, enclosing, &scope));
          break;
        case ScopeKind::Lexical:
        case ScopeKind::SimpleCatch:
        case ScopeKind::Catch:
        case ScopeKind::NamedLambda:
        case ScopeKind::StrictNamedLambda:
          MOZ_TRY(LexicalScope::XDR(xdr, scopeKind, enclosing, &scope));
          break;
        case ScopeKind::With:
          if (mode == XDR_DECODE) {
            scope = WithScope::create(cx, enclosing);
            if (!scope) {
              return xdr->fail(JS::TranscodeResult_Throw);
            }
          }
          break;
        case ScopeKind::Eval:
        case ScopeKind::StrictEval:
          MOZ_TRY(EvalScope::XDR(xdr, scopeKind, enclosing, &scope));
          break;
        case ScopeKind::Global:
        case ScopeKind::NonSyntactic:
          MOZ_TRY(GlobalScope::XDR(xdr, scopeKind, &scope));
          break;
        case ScopeKind::Module:
        case ScopeKind::WasmInstance:
          MOZ_CRASH("NYI");
          break;
        case ScopeKind::WasmFunction:
          MOZ_CRASH("wasm functions cannot be nested in JSScripts");
          break;
        default:
          // Fail in debug, but only soft-fail in release
          MOZ_ASSERT(false, "Bad XDR scope kind");
          return xdr->fail(JS::TranscodeResult_Failure_BadDecode);
      }

      if (mode == XDR_DECODE) {
        vector[i].init(scope);
      }
||||||| merged common ancestors
    for (uint32_t i = 0; i != natoms; ++i) {
        if (mode == XDR_DECODE) {
            RootedAtom tmp(cx);
            MOZ_TRY(XDRAtom(xdr, &tmp));
            script->atoms()[i].init(tmp);
        } else {
            RootedAtom tmp(cx, script->atoms()[i]);
            MOZ_TRY(XDRAtom(xdr, &tmp));
        }
=======
    for (uint32_t i = 0; i != natoms; ++i) {
      if (mode == XDR_ENCODE) {
        atom = vector[i];
      }
      MOZ_TRY(XDRAtom(xdr, &atom));
      if (mode == XDR_DECODE) {
        vector[i].init(atom);
      }
    }
  }

  for (uint32_t& elem : ssd->resumeOffsets()) {
    MOZ_TRY(xdr->codeUint32(&elem));
  }

  for (ScopeNote& elem : ssd->scopeNotes()) {
    MOZ_TRY(elem.XDR(xdr));
  }

  for (JSTryNote& elem : ssd->tryNotes()) {
    MOZ_TRY(elem.XDR(xdr));
  }

  return Ok();
}

template
    /* static */
    XDRResult
    SharedScriptData::XDR(XDRState<XDR_ENCODE>* xdr, HandleScript script);

template
    /* static */
    XDRResult
    SharedScriptData::XDR(XDRState<XDR_DECODE>* xdr, HandleScript script);

template <XDRMode mode>
XDRResult js::XDRScript(XDRState<mode>* xdr, HandleScope scriptEnclosingScope,
                        HandleScriptSourceObject sourceObjectArg,
                        HandleFunction fun, MutableHandleScript scriptp) {
  using ImmutableFlags = JSScript::ImmutableFlags;

  /* NB: Keep this in sync with CopyScript. */

  enum XDRScriptFlags {
    OwnSource,
    HasLazyScript,
  };

  uint8_t xdrScriptFlags = 0;

  uint32_t lineno = 0;
  uint32_t column = 0;
  uint32_t mainOffset = 0;
  uint32_t nfixed = 0;
  uint32_t nslots = 0;
  uint32_t bodyScopeIndex = 0;
  uint32_t sourceStart = 0;
  uint32_t sourceEnd = 0;
  uint32_t toStringStart = 0;
  uint32_t toStringEnd = 0;
  uint32_t immutableFlags = 0;

  // NOTE: |mutableFlags| are not preserved by XDR.

  JSContext* cx = xdr->cx();
  RootedScript script(cx);

  if (mode == XDR_ENCODE) {
    script = scriptp.get();
    MOZ_ASSERT(script->functionNonDelazifying() == fun);

    if (!fun && script->treatAsRunOnce() && script->hasRunOnce()) {
      // This is a toplevel or eval script that's runOnce.  We want to
      // make sure that we're not XDR-saving an object we emitted for
      // JSOP_OBJECT that then got modified.  So throw if we're not
      // cloning in JSOP_OBJECT or if we ever didn't clone in it in the
      // past.
      Realm* realm = cx->realm();
      if (!realm->creationOptions().cloneSingletons() ||
          !realm->behaviors().getSingletonsAsTemplates()) {
        return xdr->fail(JS::TranscodeResult_Failure_RunOnceNotSupported);
      }
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    // Verify marker to detect data corruption after decoding scope data. A
    // mismatch here indicates we will almost certainly crash in release.
    MOZ_TRY(xdr->codeMarker(0x48922BAB));
  }

  /*
   * Here looping from 0-to-length to xdr objects is essential to ensure that
   * all references to enclosing blocks (via FindScopeIndex below) happen
   * after the enclosing block has been XDR'd.
   */
  if (nobjects) {
    for (GCPtrObject& elem : data->objects()) {
      XDRClassKind classk;

      if (mode == XDR_ENCODE) {
        JSObject* obj = elem.get();
        if (obj->is<RegExpObject>()) {
          classk = CK_RegexpObject;
        } else if (obj->is<JSFunction>()) {
          classk = CK_JSFunction;
        } else if (obj->is<PlainObject>() || obj->is<ArrayObject>()) {
          classk = CK_JSObject;
        } else {
          MOZ_CRASH("Cannot encode this class of object.");
        }
      }
||||||| merged common ancestors
    if (mode == XDR_DECODE) {
        if (!script->shareScriptData(cx)) {
            return xdr->fail(JS::TranscodeResult_Throw);
        }
    }
=======
    if (!sourceObjectArg) {
      xdrScriptFlags |= (1 << OwnSource);
    }
    if (script->isRelazifiableIgnoringJitCode()) {
      xdrScriptFlags |= (1 << HasLazyScript);
    }
  }

  MOZ_TRY(xdr->codeUint8(&xdrScriptFlags));

  if (mode == XDR_ENCODE) {
    lineno = script->lineno();
    column = script->column();
    mainOffset = script->mainOffset();
    nfixed = script->nfixed();
    nslots = script->nslots();
    bodyScopeIndex = script->bodyScopeIndex();

    sourceStart = script->sourceStart();
    sourceEnd = script->sourceEnd();
    toStringStart = script->toStringStart();
    toStringEnd = script->toStringEnd();

    immutableFlags = script->immutableFlags_;
  }

  MOZ_TRY(xdr->codeUint32(&lineno));
  MOZ_TRY(xdr->codeUint32(&column));
  MOZ_TRY(xdr->codeUint32(&mainOffset));
  MOZ_TRY(xdr->codeUint32(&nfixed));
  MOZ_TRY(xdr->codeUint32(&nslots));
  MOZ_TRY(xdr->codeUint32(&bodyScopeIndex));
  MOZ_TRY(xdr->codeUint32(&sourceStart));
  MOZ_TRY(xdr->codeUint32(&sourceEnd));
  MOZ_TRY(xdr->codeUint32(&toStringStart));
  MOZ_TRY(xdr->codeUint32(&toStringEnd));
  MOZ_TRY(xdr->codeUint32(&immutableFlags));

  RootedScriptSourceObject sourceObject(cx, sourceObjectArg);
  Maybe<CompileOptions> options;

  if (mode == XDR_DECODE) {
    // When loading from the bytecode cache, we get the CompileOptions from
    // the document. If the noScriptRval or selfHostingMode flag doesn't
    // match, we should fail. This only applies to the top-level and not
    // its inner functions.
    bool noScriptRval =
        !!(immutableFlags & uint32_t(ImmutableFlags::NoScriptRval));
    bool selfHosted = !!(immutableFlags & uint32_t(ImmutableFlags::SelfHosted));
    if (xdr->hasOptions() && (xdrScriptFlags & (1 << OwnSource))) {
      options.emplace(xdr->cx(), xdr->options());
      if (options->noScriptRval != noScriptRval ||
          options->selfHostingMode != selfHosted) {
        return xdr->fail(JS::TranscodeResult_Failure_WrongCompileOption);
      }
    } else {
      options.emplace(xdr->cx());
      (*options).setNoScriptRval(noScriptRval).setSelfHostingMode(selfHosted);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      MOZ_TRY(xdr->codeEnum32(&classk));

      switch (classk) {
        case CK_RegexpObject: {
          Rooted<RegExpObject*> regexp(cx);
          if (mode == XDR_ENCODE) {
            regexp = &elem->as<RegExpObject>();
          }
          MOZ_TRY(XDRScriptRegExpObject(xdr, &regexp));
          if (mode == XDR_DECODE) {
            elem.init(regexp);
          }
          break;
        }
||||||| merged common ancestors
    if (nconsts) {
        RootedValue val(cx);
        for (GCPtrValue& elem : script->consts()) {
            if (mode == XDR_ENCODE) {
                val = elem.get();
            }
            MOZ_TRY(XDRScriptConst(xdr, &val));
            if (mode == XDR_DECODE) {
                elem.init(val);
            }
        }
    }
=======
  if (xdrScriptFlags & (1 << OwnSource)) {
    Rooted<ScriptSourceHolder> ssHolder(cx);

    // We are relying on the script's ScriptSource so the caller should not
    // have passed in an explicit one.
    MOZ_ASSERT(sourceObjectArg == nullptr);

    if (mode == XDR_ENCODE) {
      sourceObject = script->sourceObject();
      ssHolder.get().reset(sourceObject->source());
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
        case CK_JSFunction: {
          /* Code the nested function's enclosing scope. */
          uint32_t funEnclosingScopeIndex = 0;
          RootedScope funEnclosingScope(cx);
          if (mode == XDR_ENCODE) {
            RootedFunction function(cx, &elem->as<JSFunction>());

            if (function->isInterpretedLazy()) {
              funEnclosingScope = function->lazyScript()->enclosingScope();
            } else if (function->isInterpreted()) {
              funEnclosingScope = function->nonLazyScript()->enclosingScope();
            } else {
              MOZ_ASSERT(function->isAsmJSNative());
              return xdr->fail(JS::TranscodeResult_Failure_AsmJSNotSupported);
            }

            funEnclosingScopeIndex = FindScopeIndex(script, *funEnclosingScope);
          }

          MOZ_TRY(xdr->codeUint32(&funEnclosingScopeIndex));

          if (mode == XDR_DECODE) {
            funEnclosingScope = script->getScope(funEnclosingScopeIndex);
          }

          // Code nested function and script.
          RootedFunction tmp(cx);
          if (mode == XDR_ENCODE) {
            tmp = &elem->as<JSFunction>();
          }
          MOZ_TRY(XDRInterpretedFunction(xdr, funEnclosingScope, sourceObject,
                                         &tmp));
          if (mode == XDR_DECODE) {
            elem.init(tmp);
          }
          break;
        }

        case CK_JSObject: {
          /* Code object literal. */
          RootedObject tmp(cx);
          if (mode == XDR_ENCODE) {
            tmp = elem.get();
          }
          MOZ_TRY(XDRObjectLiteral(xdr, &tmp));
          if (mode == XDR_DECODE) {
            elem.init(tmp);
          }
          break;
        }
||||||| merged common ancestors
    {
        MOZ_ASSERT(nscopes != 0);
        GCPtrScope* vector = script->scopes().data();
        RootedScope scope(cx);
        RootedScope enclosing(cx);
        ScopeKind scopeKind;
        uint32_t enclosingScopeIndex = 0;
        for (uint32_t i = 0; i != nscopes; ++i) {
            if (mode == XDR_ENCODE) {
                scope = vector[i];
                scopeKind = scope->kind();
            } else {
                scope = nullptr;
            }

            MOZ_TRY(xdr->codeEnum32(&scopeKind));

            if (mode == XDR_ENCODE) {
                if (i == 0) {
                    enclosingScopeIndex = UINT32_MAX;
                } else {
                    MOZ_ASSERT(scope->enclosing());
                    enclosingScopeIndex = FindScopeIndex(script, *scope->enclosing());
                }
            }

            MOZ_TRY(xdr->codeUint32(&enclosingScopeIndex));

            if (mode == XDR_DECODE) {
                if (i == 0) {
                    MOZ_ASSERT(enclosingScopeIndex == UINT32_MAX);
                    enclosing = scriptEnclosingScope;
                } else {
                    MOZ_ASSERT(enclosingScopeIndex < i);
                    enclosing = vector[enclosingScopeIndex];
                }
            }

            switch (scopeKind) {
              case ScopeKind::Function:
                MOZ_ASSERT(i == script->bodyScopeIndex());
                MOZ_TRY(FunctionScope::XDR(xdr, fun, enclosing, &scope));
                break;
              case ScopeKind::FunctionBodyVar:
              case ScopeKind::ParameterExpressionVar:
                MOZ_TRY(VarScope::XDR(xdr, scopeKind, enclosing, &scope));
                break;
              case ScopeKind::Lexical:
              case ScopeKind::SimpleCatch:
              case ScopeKind::Catch:
              case ScopeKind::NamedLambda:
              case ScopeKind::StrictNamedLambda:
                MOZ_TRY(LexicalScope::XDR(xdr, scopeKind, enclosing, &scope));
                break;
              case ScopeKind::With:
                if (mode == XDR_DECODE) {
                    scope = WithScope::create(cx, enclosing);
                    if (!scope) {
                        return xdr->fail(JS::TranscodeResult_Throw);
                    }
                }
                break;
              case ScopeKind::Eval:
              case ScopeKind::StrictEval:
                MOZ_TRY(EvalScope::XDR(xdr, scopeKind, enclosing, &scope));
                break;
              case ScopeKind::Global:
              case ScopeKind::NonSyntactic:
                MOZ_TRY(GlobalScope::XDR(xdr, scopeKind, &scope));
                break;
              case ScopeKind::Module:
              case ScopeKind::WasmInstance:
                MOZ_CRASH("NYI");
                break;
              case ScopeKind::WasmFunction:
                MOZ_CRASH("wasm functions cannot be nested in JSScripts");
                break;
              default:
                // Fail in debug, but only soft-fail in release
                MOZ_ASSERT(false, "Bad XDR scope kind");
                return xdr->fail(JS::TranscodeResult_Failure_BadDecode);
            }

            if (mode == XDR_DECODE) {
                vector[i].init(scope);
            }
        }
=======
    MOZ_TRY(ScriptSource::XDR(xdr, options, &ssHolder));
>>>>>>> upstream-releases

<<<<<<< HEAD
        default: {
          // Fail in debug, but only soft-fail in release
          MOZ_ASSERT(false, "Bad XDR class kind");
          return xdr->fail(JS::TranscodeResult_Failure_BadDecode);
        }
      }
||||||| merged common ancestors
        // Verify marker to detect data corruption after decoding scope data. A
        // mismatch here indicates we will almost certainly crash in release.
        MOZ_TRY(xdr->codeMarker(0x48922BAB));
=======
    if (mode == XDR_DECODE) {
      sourceObject = ScriptSourceObject::create(cx, ssHolder.get().get());
      if (!sourceObject) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }

      if (xdr->hasScriptSourceObjectOut()) {
        // When the ScriptSourceObjectOut is provided by ParseTask, it
        // is stored in a location which is traced by the GC.
        *xdr->scriptSourceObjectOut() = sourceObject;
      } else if (!ScriptSourceObject::initFromOptions(cx, sourceObject,
                                                      *options)) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
  } else {
    // While encoding, the ScriptSource passed in must match the ScriptSource
    // of the script.
    MOZ_ASSERT_IF(mode == XDR_ENCODE,
                  sourceObjectArg->source() == script->scriptSource());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Verify marker to detect data corruption after decoding object data. A
  // mismatch here indicates we will almost certainly crash in release.
  MOZ_TRY(xdr->codeMarker(0xF83B989A));

  if (ntrynotes) {
    for (JSTryNote& elem : data->tryNotes()) {
      MOZ_TRY(xdr->codeUint8(&elem.kind));
      MOZ_TRY(xdr->codeUint32(&elem.stackDepth));
      MOZ_TRY(xdr->codeUint32(&elem.start));
      MOZ_TRY(xdr->codeUint32(&elem.length));
||||||| merged common ancestors
    /*
     * Here looping from 0-to-length to xdr objects is essential to ensure that
     * all references to enclosing blocks (via FindScopeIndex below) happen
     * after the enclosing block has been XDR'd.
     */
    if (nobjects) {
        for (GCPtrObject& elem : script->objects()) {
            XDRClassKind classk;

            if (mode == XDR_ENCODE) {
                JSObject* obj = elem.get();
                if (obj->is<RegExpObject>()) {
                    classk = CK_RegexpObject;
                } else if (obj->is<JSFunction>()) {
                    classk = CK_JSFunction;
                } else if (obj->is<PlainObject>() || obj->is<ArrayObject>()) {
                    classk = CK_JSObject;
                } else {
                    MOZ_CRASH("Cannot encode this class of object.");
                }
            }

            MOZ_TRY(xdr->codeEnum32(&classk));

            switch (classk) {
              case CK_RegexpObject: {
                Rooted<RegExpObject*> regexp(cx);
                if (mode == XDR_ENCODE) {
                    regexp = &elem->as<RegExpObject>();
                }
                MOZ_TRY(XDRScriptRegExpObject(xdr, &regexp));
                if (mode == XDR_DECODE) {
                    elem.init(regexp);
                }
                break;
              }

              case CK_JSFunction: {
                /* Code the nested function's enclosing scope. */
                uint32_t funEnclosingScopeIndex = 0;
                RootedScope funEnclosingScope(cx);
                if (mode == XDR_ENCODE) {
                    RootedFunction function(cx, &elem->as<JSFunction>());

                    if (function->isInterpretedLazy()) {
                        funEnclosingScope = function->lazyScript()->enclosingScope();
                    } else if (function->isInterpreted()) {
                        funEnclosingScope = function->nonLazyScript()->enclosingScope();
                    } else {
                        MOZ_ASSERT(function->isAsmJSNative());
                        return xdr->fail(JS::TranscodeResult_Failure_AsmJSNotSupported);
                    }

                    funEnclosingScopeIndex = FindScopeIndex(script, *funEnclosingScope);
                }

                MOZ_TRY(xdr->codeUint32(&funEnclosingScopeIndex));

                if (mode == XDR_DECODE) {
                    funEnclosingScope = script->getScope(funEnclosingScopeIndex);
                }

                // Code nested function and script.
                RootedFunction tmp(cx);
                if (mode == XDR_ENCODE) {
                    tmp = &elem->as<JSFunction>();
                }
                MOZ_TRY(XDRInterpretedFunction(xdr, funEnclosingScope, sourceObject, &tmp));
                if (mode == XDR_DECODE) {
                    elem.init(tmp);
                }
                break;
              }

              case CK_JSObject: {
                /* Code object literal. */
                RootedObject tmp(cx);
                if (mode == XDR_ENCODE) {
                    tmp = elem.get();
                }
                MOZ_TRY(XDRObjectLiteral(xdr, &tmp));
                if (mode == XDR_DECODE) {
                    elem.init(tmp);
                }
                break;
              }

              default: {
                // Fail in debug, but only soft-fail in release
                MOZ_ASSERT(false, "Bad XDR class kind");
                return xdr->fail(JS::TranscodeResult_Failure_BadDecode);
              }
            }
        }
=======
  if (mode == XDR_DECODE) {
    script = JSScript::Create(cx, *options, sourceObject, sourceStart,
                              sourceEnd, toStringStart, toStringEnd);
    if (!script) {
      return xdr->fail(JS::TranscodeResult_Throw);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
    scriptp.set(script);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (nscopenotes) {
    for (ScopeNote& elem : data->scopeNotes()) {
      MOZ_TRY(xdr->codeUint32(&elem.index));
      MOZ_TRY(xdr->codeUint32(&elem.start));
      MOZ_TRY(xdr->codeUint32(&elem.length));
      MOZ_TRY(xdr->codeUint32(&elem.parent));
||||||| merged common ancestors
    // Verify marker to detect data corruption after decoding object data. A
    // mismatch here indicates we will almost certainly crash in release.
    MOZ_TRY(xdr->codeMarker(0xF83B989A));

    if (ntrynotes) {
        for (JSTryNote& elem : script->trynotes()) {
            MOZ_TRY(xdr->codeUint8(&elem.kind));
            MOZ_TRY(xdr->codeUint32(&elem.stackDepth));
            MOZ_TRY(xdr->codeUint32(&elem.start));
            MOZ_TRY(xdr->codeUint32(&elem.length));
        }
=======
    script->lineno_ = lineno;
    script->column_ = column;
    script->immutableFlags_ = immutableFlags;

    if (script->hasFlag(ImmutableFlags::ArgsHasVarBinding)) {
      // Call setArgumentsHasVarBinding to initialize the
      // NeedsArgsAnalysis flag.
      script->setArgumentsHasVarBinding();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (nresumeoffsets) {
    for (uint32_t& elem : data->resumeOffsets()) {
      MOZ_TRY(xdr->codeUint32(&elem));
||||||| merged common ancestors
    if (nscopenotes) {
        for (ScopeNote& elem : script->scopeNotes()) {
            MOZ_TRY(xdr->codeUint32(&elem.index));
            MOZ_TRY(xdr->codeUint32(&elem.start));
            MOZ_TRY(xdr->codeUint32(&elem.length));
            MOZ_TRY(xdr->codeUint32(&elem.parent));
        }
=======
    // Set the script in its function now so that inner scripts to be
    // decoded may iterate the static scope chain.
    if (fun) {
      fun->initScript(script);
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (scriptBits & (1 << HasLazyScript)) {
    Rooted<LazyScript*> lazy(cx);
    if (mode == XDR_ENCODE) {
      lazy = script->maybeLazyScript();
||||||| merged common ancestors
    if (nyieldoffsets) {
        for (uint32_t& elem : script->yieldAndAwaitOffsets()) {
            MOZ_TRY(xdr->codeUint32(&elem));
        }
=======
  // If XDR operation fails, we must call JSScript::freeScriptData in order
  // to neuter the script. Various things that iterate raw scripts in a GC
  // arena use the presense of this data to detect if initialization is
  // complete.
  auto scriptDataGuard = mozilla::MakeScopeExit([&] {
    if (mode == XDR_DECODE) {
      script->freeScriptData();
>>>>>>> upstream-releases
    }
  });

<<<<<<< HEAD
    MOZ_TRY(
        XDRRelazificationInfo(xdr, fun, script, scriptEnclosingScope, &lazy));
||||||| merged common ancestors
    if (scriptBits & (1 << HasLazyScript)) {
        Rooted<LazyScript*> lazy(cx);
        if (mode == XDR_ENCODE) {
            lazy = script->maybeLazyScript();
        }

        MOZ_TRY(XDRRelazificationInfo(xdr, fun, script, scriptEnclosingScope, &lazy));
=======
  // NOTE: The script data is rooted by the script.
  MOZ_TRY(PrivateScriptData::XDR<mode>(xdr, script, sourceObject,
                                       scriptEnclosingScope, fun));
  MOZ_TRY(SharedScriptData::XDR<mode>(xdr, script));

  if (mode == XDR_DECODE) {
    if (!script->shareScriptData(cx)) {
      return xdr->fail(JS::TranscodeResult_Throw);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (mode == XDR_DECODE) {
      script->setLazyScript(lazy);
||||||| merged common ancestors
        if (mode == XDR_DECODE) {
            script->setLazyScript(lazy);
        }
=======
  if (xdrScriptFlags & (1 << HasLazyScript)) {
    Rooted<LazyScript*> lazy(cx);
    if (mode == XDR_ENCODE) {
      lazy = script->maybeLazyScript();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (mode == XDR_DECODE) {
    scriptp.set(script);
||||||| merged common ancestors
    if (mode == XDR_DECODE) {
        scriptp.set(script);
=======
    MOZ_TRY(
        XDRRelazificationInfo(xdr, fun, script, scriptEnclosingScope, &lazy));

    if (mode == XDR_DECODE) {
      script->setLazyScript(lazy);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    /* see BytecodeEmitter::tellDebuggerAboutCompiledScript */
    if (!fun && !cx->helperThread()) {
      Debugger::onNewScript(cx, script);
||||||| merged common ancestors
        /* see BytecodeEmitter::tellDebuggerAboutCompiledScript */
        if (!fun && !cx->helperThread()) {
            Debugger::onNewScript(cx, script);
        }
=======
  if (mode == XDR_DECODE) {
    /* see BytecodeEmitter::tellDebuggerAboutCompiledScript */
    if (!fun && !cx->isHelperThreadContext()) {
      Debugger::onNewScript(cx, script);
>>>>>>> upstream-releases
    }
  }

  scriptDataGuard.release();
  return Ok();
}

<<<<<<< HEAD
template XDRResult js::XDRScript(XDRState<XDR_ENCODE>*, HandleScope,
                                 HandleScriptSourceObject, HandleFunction,
                                 MutableHandleScript);

template XDRResult js::XDRScript(XDRState<XDR_DECODE>*, HandleScope,
                                 HandleScriptSourceObject, HandleFunction,
                                 MutableHandleScript);

template <XDRMode mode>
XDRResult js::XDRLazyScript(XDRState<mode>* xdr, HandleScope enclosingScope,
                            HandleScriptSourceObject sourceObject,
                            HandleFunction fun,
                            MutableHandle<LazyScript*> lazy) {
  MOZ_ASSERT_IF(mode == XDR_DECODE, sourceObject);

  JSContext* cx = xdr->cx();

  {
    uint32_t sourceStart;
    uint32_t sourceEnd;
    uint32_t toStringStart;
    uint32_t toStringEnd;
    uint32_t lineno;
    uint32_t column;
    uint64_t packedFields;

    if (mode == XDR_ENCODE) {
      // Note: it's possible the LazyScript has a non-null script_ pointer
      // to a JSScript. We don't encode it: we can just delazify the
      // lazy script.
||||||| merged common ancestors
template XDRResult
js::XDRScript(XDRState<XDR_ENCODE>*, HandleScope, HandleScriptSourceObject, HandleFunction,
              MutableHandleScript);

template XDRResult
js::XDRScript(XDRState<XDR_DECODE>*, HandleScope, HandleScriptSourceObject, HandleFunction,
              MutableHandleScript);

template<XDRMode mode>
XDRResult
js::XDRLazyScript(XDRState<mode>* xdr, HandleScope enclosingScope,
                  HandleScriptSourceObject sourceObject, HandleFunction fun,
                  MutableHandle<LazyScript*> lazy)
{
    MOZ_ASSERT_IF(mode == XDR_DECODE, sourceObject);

    JSContext* cx = xdr->cx();

    {
        uint32_t sourceStart;
        uint32_t sourceEnd;
        uint32_t toStringStart;
        uint32_t toStringEnd;
        uint32_t lineno;
        uint32_t column;
        uint64_t packedFields;

        if (mode == XDR_ENCODE) {
            // Note: it's possible the LazyScript has a non-null script_ pointer
            // to a JSScript. We don't encode it: we can just delazify the
            // lazy script.

            MOZ_ASSERT(fun == lazy->functionNonDelazifying());

            sourceStart = lazy->sourceStart();
            sourceEnd = lazy->sourceEnd();
            toStringStart = lazy->toStringStart();
            toStringEnd = lazy->toStringEnd();
            lineno = lazy->lineno();
            column = lazy->column();
            packedFields = lazy->packedFields();
        }
=======
template XDRResult js::XDRScript(XDRState<XDR_ENCODE>*, HandleScope,
                                 HandleScriptSourceObject, HandleFunction,
                                 MutableHandleScript);
>>>>>>> upstream-releases

<<<<<<< HEAD
      MOZ_ASSERT(fun == lazy->functionNonDelazifying());

      sourceStart = lazy->sourceStart();
      sourceEnd = lazy->sourceEnd();
      toStringStart = lazy->toStringStart();
      toStringEnd = lazy->toStringEnd();
      lineno = lazy->lineno();
      column = lazy->column();
      packedFields = lazy->packedFields();
||||||| merged common ancestors
        MOZ_TRY(xdr->codeUint32(&sourceStart));
        MOZ_TRY(xdr->codeUint32(&sourceEnd));
        MOZ_TRY(xdr->codeUint32(&toStringStart));
        MOZ_TRY(xdr->codeUint32(&toStringEnd));
        MOZ_TRY(xdr->codeUint32(&lineno));
        MOZ_TRY(xdr->codeUint32(&column));
        MOZ_TRY(xdr->codeUint64(&packedFields));

        if (mode == XDR_DECODE) {
            lazy.set(LazyScript::CreateForXDR(cx, fun, nullptr, enclosingScope, sourceObject,
                                              packedFields, sourceStart, sourceEnd, toStringStart,
                                              lineno, column));
            if (!lazy) {
                return xdr->fail(JS::TranscodeResult_Throw);
            }
            lazy->setToStringEnd(toStringEnd);
            fun->initLazyScript(lazy);
        }
=======
template XDRResult js::XDRScript(XDRState<XDR_DECODE>*, HandleScope,
                                 HandleScriptSourceObject, HandleFunction,
                                 MutableHandleScript);

template <XDRMode mode>
XDRResult js::XDRLazyScript(XDRState<mode>* xdr, HandleScope enclosingScope,
                            HandleScriptSourceObject sourceObject,
                            HandleFunction fun,
                            MutableHandle<LazyScript*> lazy) {
  MOZ_ASSERT_IF(mode == XDR_DECODE, sourceObject);

  JSContext* cx = xdr->cx();

  {
    uint32_t sourceStart;
    uint32_t sourceEnd;
    uint32_t toStringStart;
    uint32_t toStringEnd;
    uint32_t lineno;
    uint32_t column;
    uint32_t immutableFlags;
    uint32_t numFieldInitializers;
    uint32_t numClosedOverBindings;
    uint32_t numInnerFunctions;

    if (mode == XDR_ENCODE) {
      // Note: it's possible the LazyScript has a non-null script_ pointer
      // to a JSScript. We don't encode it: we can just delazify the
      // lazy script.

      MOZ_ASSERT(fun == lazy->functionNonDelazifying());

      sourceStart = lazy->sourceStart();
      sourceEnd = lazy->sourceEnd();
      toStringStart = lazy->toStringStart();
      toStringEnd = lazy->toStringEnd();
      lineno = lazy->lineno();
      column = lazy->column();
      immutableFlags = lazy->immutableFlags();
      if (fun->kind() == JSFunction::FunctionKind::ClassConstructor) {
        numFieldInitializers =
            (uint32_t)lazy->getFieldInitializers().numFieldInitializers;
      } else {
        numFieldInitializers = UINT32_MAX;
      }
      numClosedOverBindings = lazy->numClosedOverBindings();
      numInnerFunctions = lazy->numInnerFunctions();
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    MOZ_TRY(xdr->codeUint32(&sourceStart));
    MOZ_TRY(xdr->codeUint32(&sourceEnd));
    MOZ_TRY(xdr->codeUint32(&toStringStart));
    MOZ_TRY(xdr->codeUint32(&toStringEnd));
    MOZ_TRY(xdr->codeUint32(&lineno));
    MOZ_TRY(xdr->codeUint32(&column));
    MOZ_TRY(xdr->codeUint64(&packedFields));
||||||| merged common ancestors
    // Code closed-over bindings.
    MOZ_TRY(XDRLazyClosedOverBindings(xdr, lazy));
=======
    MOZ_TRY(xdr->codeUint32(&sourceStart));
    MOZ_TRY(xdr->codeUint32(&sourceEnd));
    MOZ_TRY(xdr->codeUint32(&toStringStart));
    MOZ_TRY(xdr->codeUint32(&toStringEnd));
    MOZ_TRY(xdr->codeUint32(&lineno));
    MOZ_TRY(xdr->codeUint32(&column));
    MOZ_TRY(xdr->codeUint32(&immutableFlags));
    MOZ_TRY(xdr->codeUint32(&numFieldInitializers));
    MOZ_TRY(xdr->codeUint32(&numClosedOverBindings));
    MOZ_TRY(xdr->codeUint32(&numInnerFunctions));
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (mode == XDR_DECODE) {
      lazy.set(LazyScript::CreateForXDR(
          cx, fun, nullptr, enclosingScope, sourceObject, packedFields,
          sourceStart, sourceEnd, toStringStart, lineno, column));
      if (!lazy) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }
      lazy->setToStringEnd(toStringEnd);
      fun->initLazyScript(lazy);
    }
  }

  // Code closed-over bindings.
  MOZ_TRY(XDRLazyClosedOverBindings(xdr, lazy));

  // Code inner functions.
  {
    RootedFunction func(cx);
    GCPtrFunction* innerFunctions = lazy->innerFunctions();
    size_t numInnerFunctions = lazy->numInnerFunctions();
    for (size_t i = 0; i < numInnerFunctions; i++) {
      if (mode == XDR_ENCODE) {
        func = innerFunctions[i];
      }
||||||| merged common ancestors
    // Code inner functions.
    {
        RootedFunction func(cx);
        GCPtrFunction* innerFunctions = lazy->innerFunctions();
        size_t numInnerFunctions = lazy->numInnerFunctions();
        for (size_t i = 0; i < numInnerFunctions; i++) {
            if (mode == XDR_ENCODE) {
                func = innerFunctions[i];
            }
=======
    if (mode == XDR_DECODE) {
      lazy.set(LazyScript::CreateForXDR(
          cx, numClosedOverBindings, numInnerFunctions, fun, nullptr,
          enclosingScope, sourceObject, immutableFlags, sourceStart, sourceEnd,
          toStringStart, toStringEnd, lineno, column));
      if (!lazy) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      MOZ_TRY(XDRInterpretedFunction(xdr, nullptr, sourceObject, &func));
||||||| merged common ancestors
            MOZ_TRY(XDRInterpretedFunction(xdr, nullptr, sourceObject, &func));
=======
      if (numFieldInitializers != UINT32_MAX) {
        lazy->setFieldInitializers(
            FieldInitializers((size_t)numFieldInitializers));
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (mode == XDR_DECODE) {
        innerFunctions[i] = func;
        if (innerFunctions[i]->isInterpretedLazy()) {
          innerFunctions[i]->lazyScript()->setEnclosingLazyScript(lazy);
        }
      }
||||||| merged common ancestors
            if (mode == XDR_DECODE) {
                innerFunctions[i] = func;
                if (innerFunctions[i]->isInterpretedLazy()) {
                    innerFunctions[i]->lazyScript()->setEnclosingLazyScript(lazy);
                }
            }
        }
=======
      fun->initLazyScript(lazy);
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  return Ok();
}
||||||| merged common ancestors
    return Ok();
}
=======
  // Code closed-over bindings.
  MOZ_TRY(XDRLazyClosedOverBindings(xdr, lazy));
>>>>>>> upstream-releases

<<<<<<< HEAD
template XDRResult js::XDRLazyScript(XDRState<XDR_ENCODE>*, HandleScope,
                                     HandleScriptSourceObject, HandleFunction,
                                     MutableHandle<LazyScript*>);
||||||| merged common ancestors
template XDRResult
js::XDRLazyScript(XDRState<XDR_ENCODE>*, HandleScope, HandleScriptSourceObject,
                  HandleFunction, MutableHandle<LazyScript*>);

template XDRResult
js::XDRLazyScript(XDRState<XDR_DECODE>*, HandleScope, HandleScriptSourceObject,
                  HandleFunction, MutableHandle<LazyScript*>);

void
JSScript::setSourceObject(JSObject* object)
{
    MOZ_ASSERT(compartment() == object->compartment());
    sourceObject_ = object;
}

void
JSScript::setDefaultClassConstructorSpan(JSObject* sourceObject, uint32_t start, uint32_t end,
                                         unsigned line, unsigned column)
{
    MOZ_ASSERT(isDefaultClassConstructor());
    setSourceObject(sourceObject);
    toStringStart_ = start;
    toStringEnd_ = end;
    sourceStart_ = start;
    sourceEnd_ = end;
    lineno_ = line;
    column_ = column;
    // Since this script has been changed to point into the user's source, we
    // can clear its self-hosted flag, allowing Debugger to see it.
    bitFields_.selfHosted_ = false;
}

js::ScriptSourceObject&
JSScript::scriptSourceUnwrap() const {
    // This may be called off the main thread. It's OK not to expose the source
    // object here as it doesn't escape.
    return UncheckedUnwrapWithoutExpose(sourceObject())->as<ScriptSourceObject>();
}

js::ScriptSource*
JSScript::scriptSource() const {
    return scriptSourceUnwrap().source();
}

js::ScriptSource*
JSScript::maybeForwardedScriptSource() const {
    JSObject* source = MaybeForwarded(sourceObject());
    // This may be called during GC. It's OK not to expose the source object
    // here as it doesn't escape.
    return UncheckedUnwrapWithoutExpose(source)->as<ScriptSourceObject>().source();
}

bool
JSScript::initScriptCounts(JSContext* cx)
{
    MOZ_ASSERT(!hasScriptCounts());

    // Record all pc which are the first instruction of a basic block.
    mozilla::Vector<jsbytecode*, 16, SystemAllocPolicy> jumpTargets;
    jsbytecode* mainPc = main();
    jsbytecode* end = codeEnd();
    for (jsbytecode* pc = code(); pc != end; pc = GetNextPc(pc)) {
        if (BytecodeIsJumpTarget(JSOp(*pc)) || pc == mainPc) {
            if (!jumpTargets.append(pc)) {
                ReportOutOfMemory(cx);
                return false;
            }
        }
    }
=======
  // Code inner functions.
  {
    RootedFunction func(cx);
    for (GCPtrFunction& elem : lazy->innerFunctions()) {
      if (mode == XDR_ENCODE) {
        func = elem.get();
      }

      MOZ_TRY(XDRInterpretedFunction(xdr, nullptr, sourceObject, &func));

      if (mode == XDR_DECODE) {
        elem.init(func);
        if (elem->isInterpretedLazy()) {
          elem->lazyScript()->setEnclosingLazyScript(lazy);
        }
      }
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template XDRResult js::XDRLazyScript(XDRState<XDR_DECODE>*, HandleScope,
                                     HandleScriptSourceObject, HandleFunction,
                                     MutableHandle<LazyScript*>);
||||||| merged common ancestors
    // Initialize all PCCounts counters to 0.
    ScriptCounts::PCCountsVector base;
    if (!base.reserve(jumpTargets.length())) {
        ReportOutOfMemory(cx);
        return false;
    }
=======
  return Ok();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
void JSScript::setSourceObject(JSObject* object) {
  MOZ_ASSERT(compartment() == object->compartment());
  sourceObject_ = object;
}
||||||| merged common ancestors
    for (size_t i = 0; i < jumpTargets.length(); i++) {
        base.infallibleEmplaceBack(pcToOffset(jumpTargets[i]));
    }
=======
template XDRResult js::XDRLazyScript(XDRState<XDR_ENCODE>*, HandleScope,
                                     HandleScriptSourceObject, HandleFunction,
                                     MutableHandle<LazyScript*>);
>>>>>>> upstream-releases

<<<<<<< HEAD
void JSScript::setDefaultClassConstructorSpan(JSObject* sourceObject,
                                              uint32_t start, uint32_t end,
                                              unsigned line, unsigned column) {
  MOZ_ASSERT(isDefaultClassConstructor());
  setSourceObject(sourceObject);
  toStringStart_ = start;
  toStringEnd_ = end;
  sourceStart_ = start;
  sourceEnd_ = end;
  lineno_ = line;
  column_ = column;
  // Since this script has been changed to point into the user's source, we
  // can clear its self-hosted flag, allowing Debugger to see it.
  clearFlag(ImmutableFlags::SelfHosted);
}
||||||| merged common ancestors
    // Create realm's scriptCountsMap if necessary.
    if (!realm()->scriptCountsMap) {
        auto map = cx->make_unique<ScriptCountsMap>();
        if (!map) {
            return false;
        }
=======
template XDRResult js::XDRLazyScript(XDRState<XDR_DECODE>*, HandleScope,
                                     HandleScriptSourceObject, HandleFunction,
                                     MutableHandle<LazyScript*>);
>>>>>>> upstream-releases

<<<<<<< HEAD
js::ScriptSourceObject& JSScript::scriptSourceUnwrap() const {
  // This may be called off the main thread. It's OK not to expose the source
  // object here as it doesn't escape.
  return UncheckedUnwrapWithoutExpose(sourceObject())->as<ScriptSourceObject>();
}
||||||| merged common ancestors
        realm()->scriptCountsMap = std::move(map);
    }
=======
void JSScript::setSourceObject(js::ScriptSourceObject* object) {
  MOZ_ASSERT(compartment() == object->compartment());
  sourceObject_ = object;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
js::ScriptSource* JSScript::scriptSource() const {
  return scriptSourceUnwrap().source();
}
||||||| merged common ancestors
    // Allocate the ScriptCounts.
    UniqueScriptCounts sc = cx->make_unique<ScriptCounts>(std::move(base));
    if (!sc) {
        ReportOutOfMemory(cx);
        return false;
    }
=======
void JSScript::setDefaultClassConstructorSpan(
    js::ScriptSourceObject* sourceObject, uint32_t start, uint32_t end,
    unsigned line, unsigned column) {
  MOZ_ASSERT(isDefaultClassConstructor());
  setSourceObject(sourceObject);
  toStringStart_ = start;
  toStringEnd_ = end;
  sourceStart_ = start;
  sourceEnd_ = end;
  lineno_ = line;
  column_ = column;
  // Since this script has been changed to point into the user's source, we
  // can clear its self-hosted flag, allowing Debugger to see it.
  clearFlag(ImmutableFlags::SelfHosted);
}

js::ScriptSource* JSScript::scriptSource() const {
  return sourceObject()->source();
}

js::ScriptSource* JSScript::maybeForwardedScriptSource() const {
  JSObject* source = MaybeForwarded(sourceObject());
  // This may be called during GC. It's OK not to expose the source object
  // here as it doesn't escape.
  return UncheckedUnwrapWithoutExpose(source)
      ->as<ScriptSourceObject>()
      .source();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
js::ScriptSource* JSScript::maybeForwardedScriptSource() const {
  JSObject* source = MaybeForwarded(sourceObject());
  // This may be called during GC. It's OK not to expose the source object
  // here as it doesn't escape.
  return UncheckedUnwrapWithoutExpose(source)
      ->as<ScriptSourceObject>()
      .source();
}

bool JSScript::initScriptCounts(JSContext* cx) {
  MOZ_ASSERT(!hasScriptCounts());

  // Record all pc which are the first instruction of a basic block.
  mozilla::Vector<jsbytecode*, 16, SystemAllocPolicy> jumpTargets;

  js::BytecodeLocation main = mainLocation();
  AllBytecodesIterable iterable(this);
  for (auto& loc : iterable) {
    if (loc.isJumpTarget() || loc == main) {
      if (!jumpTargets.append(loc.toRawBytecode())) {
||||||| merged common ancestors
    // Register the current ScriptCounts in the realm's map.
    if (!realm()->scriptCountsMap->putNew(this, std::move(sc))) {
=======
bool JSScript::initScriptCounts(JSContext* cx) {
  MOZ_ASSERT(!hasScriptCounts());

  // Record all pc which are the first instruction of a basic block.
  mozilla::Vector<jsbytecode*, 16, SystemAllocPolicy> jumpTargets;

  js::BytecodeLocation main = mainLocation();
  AllBytecodesIterable iterable(this);
  for (auto& loc : iterable) {
    if (loc.isJumpTarget() || loc == main) {
      if (!jumpTargets.append(loc.toRawBytecode())) {
>>>>>>> upstream-releases
        ReportOutOfMemory(cx);
        return false;
      }
    }
<<<<<<< HEAD
  }

  // Initialize all PCCounts counters to 0.
  ScriptCounts::PCCountsVector base;
  if (!base.reserve(jumpTargets.length())) {
    ReportOutOfMemory(cx);
    return false;
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (size_t i = 0; i < jumpTargets.length(); i++) {
    base.infallibleEmplaceBack(pcToOffset(jumpTargets[i]));
  }
||||||| merged common ancestors
    // safe to set this;  we can't fail after this point.
    bitFields_.hasScriptCounts_ = true;
=======
  // Initialize all PCCounts counters to 0.
  ScriptCounts::PCCountsVector base;
  if (!base.reserve(jumpTargets.length())) {
    ReportOutOfMemory(cx);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Create realm's scriptCountsMap if necessary.
  if (!realm()->scriptCountsMap) {
    auto map = cx->make_unique<ScriptCountsMap>();
    if (!map) {
      return false;
||||||| merged common ancestors
    // Enable interrupts in any interpreter frames running on this script. This
    // is used to let the interpreter increment the PCCounts, if present.
    for (ActivationIterator iter(cx); !iter.done(); ++iter) {
        if (iter->isInterpreter()) {
            iter->asInterpreter()->enableInterruptsIfRunning(this);
        }
=======
  for (size_t i = 0; i < jumpTargets.length(); i++) {
    base.infallibleEmplaceBack(pcToOffset(jumpTargets[i]));
  }

  // Create realm's scriptCountsMap if necessary.
  if (!realm()->scriptCountsMap) {
    auto map = cx->make_unique<ScriptCountsMap>();
    if (!map) {
      return false;
>>>>>>> upstream-releases
    }

    realm()->scriptCountsMap = std::move(map);
  }

  // Allocate the ScriptCounts.
  UniqueScriptCounts sc = cx->make_unique<ScriptCounts>(std::move(base));
  if (!sc) {
    ReportOutOfMemory(cx);
    return false;
  }

  // Register the current ScriptCounts in the realm's map.
  if (!realm()->scriptCountsMap->putNew(this, std::move(sc))) {
    ReportOutOfMemory(cx);
    return false;
  }

  // safe to set this;  we can't fail after this point.
  setFlag(MutableFlags::HasScriptCounts);

  // Enable interrupts in any interpreter frames running on this script. This
  // is used to let the interpreter increment the PCCounts, if present.
  for (ActivationIterator iter(cx); !iter.done(); ++iter) {
    if (iter->isInterpreter()) {
      iter->asInterpreter()->enableInterruptsIfRunning(this);
    }
  }

  return true;
}

static inline ScriptCountsMap::Ptr GetScriptCountsMapEntry(JSScript* script) {
  MOZ_ASSERT(script->hasScriptCounts());
  ScriptCountsMap::Ptr p = script->realm()->scriptCountsMap->lookup(script);
  MOZ_ASSERT(p);
  return p;
}

static inline ScriptNameMap::Ptr GetScriptNameMapEntry(JSScript* script) {
  auto p = script->realm()->scriptNameMap->lookup(script);
  MOZ_ASSERT(p);
  return p;
}

ScriptCounts& JSScript::getScriptCounts() {
  ScriptCountsMap::Ptr p = GetScriptCountsMapEntry(this);
  return *p->value();
}

const char* JSScript::getScriptName() {
  auto p = GetScriptNameMapEntry(this);
  return p->value().get();
}

js::PCCounts* ScriptCounts::maybeGetPCCounts(size_t offset) {
  PCCounts searched = PCCounts(offset);
  PCCounts* elem =
      std::lower_bound(pcCounts_.begin(), pcCounts_.end(), searched);
  if (elem == pcCounts_.end() || elem->pcOffset() != offset) {
    return nullptr;
  }
  return elem;
}

const js::PCCounts* ScriptCounts::maybeGetPCCounts(size_t offset) const {
  PCCounts searched = PCCounts(offset);
  const PCCounts* elem =
      std::lower_bound(pcCounts_.begin(), pcCounts_.end(), searched);
  if (elem == pcCounts_.end() || elem->pcOffset() != offset) {
    return nullptr;
  }
  return elem;
}

js::PCCounts* ScriptCounts::getImmediatePrecedingPCCounts(size_t offset) {
  PCCounts searched = PCCounts(offset);
  PCCounts* elem =
      std::lower_bound(pcCounts_.begin(), pcCounts_.end(), searched);
  if (elem == pcCounts_.end()) {
    return &pcCounts_.back();
  }
  if (elem->pcOffset() == offset) {
    return elem;
<<<<<<< HEAD
  }
  if (elem != pcCounts_.begin()) {
    return elem - 1;
  }
  return nullptr;
}

const js::PCCounts* ScriptCounts::maybeGetThrowCounts(size_t offset) const {
  PCCounts searched = PCCounts(offset);
  const PCCounts* elem =
      std::lower_bound(throwCounts_.begin(), throwCounts_.end(), searched);
  if (elem == throwCounts_.end() || elem->pcOffset() != offset) {
    return nullptr;
  }
  return elem;
}

const js::PCCounts* ScriptCounts::getImmediatePrecedingThrowCounts(
    size_t offset) const {
  PCCounts searched = PCCounts(offset);
  const PCCounts* elem =
      std::lower_bound(throwCounts_.begin(), throwCounts_.end(), searched);
  if (elem == throwCounts_.end()) {
    if (throwCounts_.begin() == throwCounts_.end()) {
      return nullptr;
    }
    return &throwCounts_.back();
  }
  if (elem->pcOffset() == offset) {
    return elem;
  }
  if (elem != throwCounts_.begin()) {
    return elem - 1;
  }
  return nullptr;
}
||||||| merged common ancestors
}
=======
  }
  if (elem != pcCounts_.begin()) {
    return elem - 1;
  }
  return nullptr;
}

const js::PCCounts* ScriptCounts::maybeGetThrowCounts(size_t offset) const {
  PCCounts searched = PCCounts(offset);
  const PCCounts* elem =
      std::lower_bound(throwCounts_.begin(), throwCounts_.end(), searched);
  if (elem == throwCounts_.end() || elem->pcOffset() != offset) {
    return nullptr;
  }
  return elem;
}

const js::PCCounts* ScriptCounts::getImmediatePrecedingThrowCounts(
    size_t offset) const {
  PCCounts searched = PCCounts(offset);
  const PCCounts* elem =
      std::lower_bound(throwCounts_.begin(), throwCounts_.end(), searched);
  if (elem == throwCounts_.end()) {
    if (throwCounts_.begin() == throwCounts_.end()) {
      return nullptr;
    }
    return &throwCounts_.back();
  }
  if (elem->pcOffset() == offset) {
    return elem;
  }
  if (elem != throwCounts_.begin()) {
    return elem - 1;
  }
  return nullptr;
}

js::PCCounts* ScriptCounts::getThrowCounts(size_t offset) {
  PCCounts searched = PCCounts(offset);
  PCCounts* elem =
      std::lower_bound(throwCounts_.begin(), throwCounts_.end(), searched);
  if (elem == throwCounts_.end() || elem->pcOffset() != offset) {
    elem = throwCounts_.insert(elem, searched);
  }
  return elem;
}

size_t ScriptCounts::sizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf) {
  return mallocSizeOf(this) + pcCounts_.sizeOfExcludingThis(mallocSizeOf) +
         throwCounts_.sizeOfExcludingThis(mallocSizeOf) +
         ionCounts_->sizeOfIncludingThis(mallocSizeOf);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
js::PCCounts* ScriptCounts::getThrowCounts(size_t offset) {
  PCCounts searched = PCCounts(offset);
  PCCounts* elem =
      std::lower_bound(throwCounts_.begin(), throwCounts_.end(), searched);
  if (elem == throwCounts_.end() || elem->pcOffset() != offset) {
    elem = throwCounts_.insert(elem, searched);
  }
  return elem;
}

size_t ScriptCounts::sizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf) {
  return mallocSizeOf(this) + pcCounts_.sizeOfExcludingThis(mallocSizeOf) +
         throwCounts_.sizeOfExcludingThis(mallocSizeOf) +
         ionCounts_->sizeOfIncludingThis(mallocSizeOf);
||||||| merged common ancestors
size_t
ScriptCounts::sizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf) {
    return mallocSizeOf(this) +
        pcCounts_.sizeOfExcludingThis(mallocSizeOf) +
        throwCounts_.sizeOfExcludingThis(mallocSizeOf) +
        ionCounts_->sizeOfIncludingThis(mallocSizeOf);
=======
void JSScript::setIonScript(JSRuntime* rt, js::jit::IonScript* ionScript) {
  MOZ_ASSERT_IF(ionScript != ION_DISABLED_SCRIPT,
                !baselineScript()->hasPendingIonBuilder());
  if (hasIonScript()) {
    js::jit::IonScript::writeBarrierPre(zone(), ion);
    clearIonScript();
  }
  ion = ionScript;
  MOZ_ASSERT_IF(hasIonScript(), hasBaselineScript());
  if (hasIonScript()) {
    AddCellMemory(this, ion->allocBytes(), js::MemoryUse::IonScript);
  }
  updateJitCodeRaw(rt);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void JSScript::setIonScript(JSRuntime* rt, js::jit::IonScript* ionScript) {
  MOZ_ASSERT_IF(ionScript != ION_DISABLED_SCRIPT,
                !baselineScript()->hasPendingIonBuilder());
  if (hasIonScript()) {
    js::jit::IonScript::writeBarrierPre(zone(), ion);
  }
  ion = ionScript;
  MOZ_ASSERT_IF(hasIonScript(), hasBaselineScript());
  updateJitCodeRaw(rt);
||||||| merged common ancestors
void
JSScript::setIonScript(JSRuntime* rt, js::jit::IonScript* ionScript)
{
    MOZ_ASSERT_IF(ionScript != ION_DISABLED_SCRIPT, !baselineScript()->hasPendingIonBuilder());
    if (hasIonScript()) {
        js::jit::IonScript::writeBarrierPre(zone(), ion);
    }
    ion = ionScript;
    MOZ_ASSERT_IF(hasIonScript(), hasBaselineScript());
    updateJitCodeRaw(rt);
=======
js::PCCounts* JSScript::maybeGetPCCounts(jsbytecode* pc) {
  MOZ_ASSERT(containsPC(pc));
  return getScriptCounts().maybeGetPCCounts(pcToOffset(pc));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
js::PCCounts* JSScript::maybeGetPCCounts(jsbytecode* pc) {
  MOZ_ASSERT(containsPC(pc));
  return getScriptCounts().maybeGetPCCounts(pcToOffset(pc));
||||||| merged common ancestors
js::PCCounts*
JSScript::maybeGetPCCounts(jsbytecode* pc) {
    MOZ_ASSERT(containsPC(pc));
    return getScriptCounts().maybeGetPCCounts(pcToOffset(pc));
=======
const js::PCCounts* JSScript::maybeGetThrowCounts(jsbytecode* pc) {
  MOZ_ASSERT(containsPC(pc));
  return getScriptCounts().maybeGetThrowCounts(pcToOffset(pc));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
const js::PCCounts* JSScript::maybeGetThrowCounts(jsbytecode* pc) {
  MOZ_ASSERT(containsPC(pc));
  return getScriptCounts().maybeGetThrowCounts(pcToOffset(pc));
||||||| merged common ancestors
const js::PCCounts*
JSScript::maybeGetThrowCounts(jsbytecode* pc) {
    MOZ_ASSERT(containsPC(pc));
    return getScriptCounts().maybeGetThrowCounts(pcToOffset(pc));
=======
js::PCCounts* JSScript::getThrowCounts(jsbytecode* pc) {
  MOZ_ASSERT(containsPC(pc));
  return getScriptCounts().getThrowCounts(pcToOffset(pc));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
js::PCCounts* JSScript::getThrowCounts(jsbytecode* pc) {
  MOZ_ASSERT(containsPC(pc));
  return getScriptCounts().getThrowCounts(pcToOffset(pc));
}
||||||| merged common ancestors
js::PCCounts*
JSScript::getThrowCounts(jsbytecode* pc) {
    MOZ_ASSERT(containsPC(pc));
    return getScriptCounts().getThrowCounts(pcToOffset(pc));
}
=======
uint64_t JSScript::getHitCount(jsbytecode* pc) {
  MOZ_ASSERT(containsPC(pc));
  if (pc < main()) {
    pc = main();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
uint64_t JSScript::getHitCount(jsbytecode* pc) {
  MOZ_ASSERT(containsPC(pc));
  if (pc < main()) {
    pc = main();
  }

  ScriptCounts& sc = getScriptCounts();
  size_t targetOffset = pcToOffset(pc);
  const js::PCCounts* baseCount =
      sc.getImmediatePrecedingPCCounts(targetOffset);
  if (!baseCount) {
    return 0;
  }
  if (baseCount->pcOffset() == targetOffset) {
    return baseCount->numExec();
  }
  MOZ_ASSERT(baseCount->pcOffset() < targetOffset);
  uint64_t count = baseCount->numExec();
  do {
    const js::PCCounts* throwCount =
        sc.getImmediatePrecedingThrowCounts(targetOffset);
    if (!throwCount) {
      return count;
    }
    if (throwCount->pcOffset() <= baseCount->pcOffset()) {
      return count;
||||||| merged common ancestors
uint64_t
JSScript::getHitCount(jsbytecode* pc)
{
    MOZ_ASSERT(containsPC(pc));
    if (pc < main()) {
        pc = main();
    }

    ScriptCounts& sc = getScriptCounts();
    size_t targetOffset = pcToOffset(pc);
    const js::PCCounts* baseCount = sc.getImmediatePrecedingPCCounts(targetOffset);
    if (!baseCount) {
        return 0;
    }
    if (baseCount->pcOffset() == targetOffset) {
        return baseCount->numExec();
=======
  ScriptCounts& sc = getScriptCounts();
  size_t targetOffset = pcToOffset(pc);
  const js::PCCounts* baseCount =
      sc.getImmediatePrecedingPCCounts(targetOffset);
  if (!baseCount) {
    return 0;
  }
  if (baseCount->pcOffset() == targetOffset) {
    return baseCount->numExec();
  }
  MOZ_ASSERT(baseCount->pcOffset() < targetOffset);
  uint64_t count = baseCount->numExec();
  do {
    const js::PCCounts* throwCount =
        sc.getImmediatePrecedingThrowCounts(targetOffset);
    if (!throwCount) {
      return count;
    }
    if (throwCount->pcOffset() <= baseCount->pcOffset()) {
      return count;
>>>>>>> upstream-releases
    }
    count -= throwCount->numExec();
    targetOffset = throwCount->pcOffset() - 1;
  } while (true);
}

void JSScript::incHitCount(jsbytecode* pc) {
  MOZ_ASSERT(containsPC(pc));
  if (pc < main()) {
    pc = main();
  }

  ScriptCounts& sc = getScriptCounts();
  js::PCCounts* baseCount = sc.getImmediatePrecedingPCCounts(pcToOffset(pc));
  if (!baseCount) {
    return;
  }
  baseCount->numExec()++;
}

void JSScript::addIonCounts(jit::IonScriptCounts* ionCounts) {
  ScriptCounts& sc = getScriptCounts();
  if (sc.ionCounts_) {
    ionCounts->setPrevious(sc.ionCounts_);
  }
  sc.ionCounts_ = ionCounts;
}

jit::IonScriptCounts* JSScript::getIonCounts() {
  return getScriptCounts().ionCounts_;
}

void JSScript::clearHasScriptCounts() {
  clearFlag(MutableFlags::HasScriptCounts);
}

void JSScript::releaseScriptCounts(ScriptCounts* counts) {
  ScriptCountsMap::Ptr p = GetScriptCountsMapEntry(this);
  *counts = std::move(*p->value().get());
  realm()->scriptCountsMap->remove(p);
  clearHasScriptCounts();
}

void JSScript::destroyScriptCounts() {
  if (hasScriptCounts()) {
    ScriptCounts scriptCounts;
    releaseScriptCounts(&scriptCounts);
  }
}

void JSScript::destroyScriptName() {
  auto p = GetScriptNameMapEntry(this);
  realm()->scriptNameMap->remove(p);
}

<<<<<<< HEAD
void JSScript::resetScriptCounts() {
  if (!hasScriptCounts()) {
    return;
  }
||||||| merged common ancestors
bool
JSScript::hasScriptName()
{
    if (!realm()->scriptNameMap) {
        return false;
    }
=======
void JSScript::resetScriptCounts() {
  if (!hasScriptCounts()) {
    return;
  }

  ScriptCounts& sc = getScriptCounts();

  for (PCCounts& elem : sc.pcCounts_) {
    elem.numExec() = 0;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ScriptCounts& sc = getScriptCounts();

  for (PCCounts& elem : sc.pcCounts_) {
    elem.numExec() = 0;
  }

  for (PCCounts& elem : sc.throwCounts_) {
    elem.numExec() = 0;
  }
||||||| merged common ancestors
    auto p = realm()->scriptNameMap->lookup(this);
    return p.found();
=======
  for (PCCounts& elem : sc.throwCounts_) {
    elem.numExec() = 0;
  }
>>>>>>> upstream-releases
}

bool JSScript::hasScriptName() {
  if (!realm()->scriptNameMap) {
    return false;
  }

  auto p = realm()->scriptNameMap->lookup(this);
  return p.found();
}

<<<<<<< HEAD
void ScriptSourceObject::finalize(FreeOp* fop, JSObject* obj) {
  MOZ_ASSERT(fop->onMainThread());
  ScriptSourceObject* sso = &obj->as<ScriptSourceObject>();
  sso->source()->decref();
}

void ScriptSourceObject::trace(JSTracer* trc, JSObject* obj) {
  // This can be invoked during allocation of the SSO itself, before we've had a
  // chance to initialize things properly. In that case, there's nothing to
  // trace.
  if (obj->as<ScriptSourceObject>().hasSource()) {
    obj->as<ScriptSourceObject>().source()->trace(trc);
  }
||||||| merged common ancestors
void
ScriptSourceObject::trace(JSTracer* trc, JSObject* obj)
{
    // This can be invoked during allocation of the SSO itself, before we've had a chance
    // to initialize things properly. In that case, there's nothing to trace.
    if (obj->as<ScriptSourceObject>().hasSource()) {
        obj->as<ScriptSourceObject>().source()->trace(trc);
    }
=======
void ScriptSourceObject::finalize(FreeOp* fop, JSObject* obj) {
  MOZ_ASSERT(fop->onMainThread());
  ScriptSourceObject* sso = &obj->as<ScriptSourceObject>();
  sso->source()->decref();

  // Clear the private value, calling the release hook if necessary.
  sso->setPrivate(fop->runtime(), UndefinedValue());
}

void ScriptSourceObject::trace(JSTracer* trc, JSObject* obj) {
  // This can be invoked during allocation of the SSO itself, before we've had a
  // chance to initialize things properly. In that case, there's nothing to
  // trace.
  if (obj->as<ScriptSourceObject>().hasSource()) {
    obj->as<ScriptSourceObject>().source()->trace(trc);
  }
>>>>>>> upstream-releases
}

static const ClassOps ScriptSourceObjectClassOps = {
    nullptr, /* addProperty */
    nullptr, /* delProperty */
    nullptr, /* enumerate */
    nullptr, /* newEnumerate */
    nullptr, /* resolve */
    nullptr, /* mayResolve */
    ScriptSourceObject::finalize,
    nullptr, /* call */
    nullptr, /* hasInstance */
    nullptr, /* construct */
    ScriptSourceObject::trace};

const Class ScriptSourceObject::class_ = {
    "ScriptSource",
<<<<<<< HEAD
    JSCLASS_HAS_RESERVED_SLOTS(RESERVED_SLOTS) | JSCLASS_IS_ANONYMOUS |
        JSCLASS_FOREGROUND_FINALIZE,
    &ScriptSourceObjectClassOps};

ScriptSourceObject* ScriptSourceObject::create(JSContext* cx,
                                               ScriptSource* source) {
  RootedScriptSourceObject sourceObject(
      cx, NewObjectWithGivenProto<ScriptSourceObject>(cx, nullptr));
  if (!sourceObject) {
    return nullptr;
  }

  source->incref();  // The matching decref is in ScriptSourceObject::finalize.
  sourceObject->initReservedSlot(SOURCE_SLOT, PrivateValue(source));

  // The remaining slots should eventually be populated by a call to
  // initFromOptions. Poison them until that point.
  sourceObject->initReservedSlot(ELEMENT_SLOT, MagicValue(JS_GENERIC_MAGIC));
  sourceObject->initReservedSlot(ELEMENT_PROPERTY_SLOT,
                                 MagicValue(JS_GENERIC_MAGIC));
  sourceObject->initReservedSlot(INTRODUCTION_SCRIPT_SLOT,
                                 MagicValue(JS_GENERIC_MAGIC));

  return sourceObject;
}

/* static */ bool ScriptSourceObject::initFromOptions(
    JSContext* cx, HandleScriptSourceObject source,
    const ReadOnlyCompileOptions& options) {
  cx->releaseCheck(source);
  MOZ_ASSERT(source->getReservedSlot(ELEMENT_SLOT).isMagic(JS_GENERIC_MAGIC));
  MOZ_ASSERT(
      source->getReservedSlot(ELEMENT_PROPERTY_SLOT).isMagic(JS_GENERIC_MAGIC));
  MOZ_ASSERT(source->getReservedSlot(INTRODUCTION_SCRIPT_SLOT)
                 .isMagic(JS_GENERIC_MAGIC));

  RootedObject element(cx, options.element());
  RootedString elementAttributeName(cx, options.elementAttributeName());
  if (!initElementProperties(cx, source, element, elementAttributeName)) {
    return false;
  }

  // There is no equivalent of cross-compartment wrappers for scripts. If the
  // introduction script and ScriptSourceObject are in different compartments,
  // we would be creating a cross-compartment script reference, which is
  // forbidden. In that case, simply don't bother to retain the introduction
  // script.
  Value introductionScript = UndefinedValue();
  if (options.introductionScript() &&
      options.introductionScript()->compartment() == cx->compartment()) {
    introductionScript.setPrivateGCThing(options.introductionScript());
  }
  source->setReservedSlot(INTRODUCTION_SCRIPT_SLOT, introductionScript);

  return true;
}

/* static */ bool ScriptSourceObject::initElementProperties(
    JSContext* cx, HandleScriptSourceObject source, HandleObject element,
    HandleString elementAttrName) {
  RootedValue elementValue(cx, ObjectOrNullValue(element));
  if (!cx->compartment()->wrap(cx, &elementValue)) {
    return false;
  }
||||||| merged common ancestors
    JSCLASS_HAS_RESERVED_SLOTS(RESERVED_SLOTS) |
    JSCLASS_IS_ANONYMOUS |
    JSCLASS_FOREGROUND_FINALIZE,
    &ScriptSourceObjectClassOps
};

ScriptSourceObject*
ScriptSourceObject::create(JSContext* cx, ScriptSource* source)
{
    RootedScriptSourceObject sourceObject(cx, NewObjectWithGivenProto<ScriptSourceObject>(cx, nullptr));
    if (!sourceObject) {
        return nullptr;
    }
=======
    JSCLASS_HAS_RESERVED_SLOTS(RESERVED_SLOTS) | JSCLASS_FOREGROUND_FINALIZE,
    &ScriptSourceObjectClassOps};

ScriptSourceObject* ScriptSourceObject::createInternal(JSContext* cx,
                                                       ScriptSource* source,
                                                       HandleObject canonical) {
  ScriptSourceObject* obj =
      NewObjectWithGivenProto<ScriptSourceObject>(cx, nullptr);
  if (!obj) {
    return nullptr;
  }

  source->incref();  // The matching decref is in ScriptSourceObject::finalize.

  obj->initReservedSlot(SOURCE_SLOT, PrivateValue(source));
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedValue nameValue(cx);
  if (elementAttrName) {
    nameValue = StringValue(elementAttrName);
  }
  if (!cx->compartment()->wrap(cx, &nameValue)) {
    return false;
  }
||||||| merged common ancestors
    source->incref();    // The matching decref is in ScriptSourceObject::finalize.
    sourceObject->initReservedSlot(SOURCE_SLOT, PrivateValue(source));
=======
  if (canonical) {
    obj->initReservedSlot(CANONICAL_SLOT, ObjectValue(*canonical));
  } else {
    obj->initReservedSlot(CANONICAL_SLOT, ObjectValue(*obj));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  source->setReservedSlot(ELEMENT_SLOT, elementValue);
  source->setReservedSlot(ELEMENT_PROPERTY_SLOT, nameValue);
||||||| merged common ancestors
    // The remaining slots should eventually be populated by a call to
    // initFromOptions. Poison them until that point.
    sourceObject->initReservedSlot(ELEMENT_SLOT, MagicValue(JS_GENERIC_MAGIC));
    sourceObject->initReservedSlot(ELEMENT_PROPERTY_SLOT, MagicValue(JS_GENERIC_MAGIC));
    sourceObject->initReservedSlot(INTRODUCTION_SCRIPT_SLOT, MagicValue(JS_GENERIC_MAGIC));
=======
  // The slots below should either be populated by a call to initFromOptions or,
  // if this is a non-canonical ScriptSourceObject, they are unused. Poison
  // them.
  obj->initReservedSlot(ELEMENT_SLOT, MagicValue(JS_GENERIC_MAGIC));
  obj->initReservedSlot(ELEMENT_PROPERTY_SLOT, MagicValue(JS_GENERIC_MAGIC));
  obj->initReservedSlot(INTRODUCTION_SCRIPT_SLOT, MagicValue(JS_GENERIC_MAGIC));
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return sourceObject;
=======
  return obj;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ bool JSScript::loadSource(JSContext* cx, ScriptSource* ss,
                                       bool* worked) {
  MOZ_ASSERT(!ss->hasSourceText());
  *worked = false;
  if (!cx->runtime()->sourceHook.ref() || !ss->sourceRetrievable()) {
    return true;
  }
  char16_t* src = nullptr;
  size_t length;
  if (!cx->runtime()->sourceHook->load(cx, ss->filename(), &src, &length)) {
    return false;
  }
  if (!src) {
    return true;
  }
||||||| merged common ancestors
/* static */ bool
ScriptSourceObject::initFromOptions(JSContext* cx, HandleScriptSourceObject source,
                                    const ReadOnlyCompileOptions& options)
{
    cx->releaseCheck(source);
    MOZ_ASSERT(source->getReservedSlot(ELEMENT_SLOT).isMagic(JS_GENERIC_MAGIC));
    MOZ_ASSERT(source->getReservedSlot(ELEMENT_PROPERTY_SLOT).isMagic(JS_GENERIC_MAGIC));
    MOZ_ASSERT(source->getReservedSlot(INTRODUCTION_SCRIPT_SLOT).isMagic(JS_GENERIC_MAGIC));

    RootedObject element(cx, options.element());
    RootedString elementAttributeName(cx, options.elementAttributeName());
    if (!initElementProperties(cx, source, element, elementAttributeName)) {
        return false;
    }

    // There is no equivalent of cross-compartment wrappers for scripts. If the
    // introduction script and ScriptSourceObject are in different compartments,
    // we would be creating a cross-compartment script reference, which is
    // forbidden. In that case, simply don't bother to retain the introduction
    // script.
    Value introductionScript = UndefinedValue();
    if (options.introductionScript() &&
        options.introductionScript()->compartment() == cx->compartment())
    {
        introductionScript.setPrivateGCThing(options.introductionScript());
    }
    source->setReservedSlot(INTRODUCTION_SCRIPT_SLOT, introductionScript);

    return true;
}

/* static */ bool
ScriptSourceObject::initElementProperties(JSContext* cx, HandleScriptSourceObject source,
                                          HandleObject element, HandleString elementAttrName)
{
    RootedValue elementValue(cx, ObjectOrNullValue(element));
    if (!cx->compartment()->wrap(cx, &elementValue)) {
        return false;
    }

    RootedValue nameValue(cx);
    if (elementAttrName) {
        nameValue = StringValue(elementAttrName);
    }
    if (!cx->compartment()->wrap(cx, &nameValue)) {
        return false;
    }

    source->setReservedSlot(ELEMENT_SLOT, elementValue);
    source->setReservedSlot(ELEMENT_PROPERTY_SLOT, nameValue);

    return true;
}
=======
ScriptSourceObject* ScriptSourceObject::create(JSContext* cx,
                                               ScriptSource* source) {
  return createInternal(cx, source, nullptr);
}

ScriptSourceObject* ScriptSourceObject::clone(JSContext* cx,
                                              HandleScriptSourceObject sso) {
  MOZ_ASSERT(cx->compartment() != sso->compartment());

  RootedObject wrapped(cx, sso);
  if (!cx->compartment()->wrap(cx, &wrapped)) {
    return nullptr;
  }

  return createInternal(cx, sso->source(), wrapped);
}

ScriptSourceObject* ScriptSourceObject::unwrappedCanonical() const {
  MOZ_ASSERT(CurrentThreadCanAccessRuntime(runtimeFromAnyThread()));

  JSObject* obj = &getReservedSlot(CANONICAL_SLOT).toObject();
  return &UncheckedUnwrap(obj)->as<ScriptSourceObject>();
}

/* static */
bool ScriptSourceObject::initFromOptions(
    JSContext* cx, HandleScriptSourceObject source,
    const ReadOnlyCompileOptions& options) {
  cx->releaseCheck(source);
  MOZ_ASSERT(source->isCanonical());
  MOZ_ASSERT(source->getReservedSlot(ELEMENT_SLOT).isMagic(JS_GENERIC_MAGIC));
  MOZ_ASSERT(
      source->getReservedSlot(ELEMENT_PROPERTY_SLOT).isMagic(JS_GENERIC_MAGIC));
  MOZ_ASSERT(source->getReservedSlot(INTRODUCTION_SCRIPT_SLOT)
                 .isMagic(JS_GENERIC_MAGIC));

  RootedObject element(cx, options.element());
  RootedString elementAttributeName(cx, options.elementAttributeName());
  if (!initElementProperties(cx, source, element, elementAttributeName)) {
    return false;
  }

  // There is no equivalent of cross-compartment wrappers for scripts. If the
  // introduction script and ScriptSourceObject are in different compartments,
  // we would be creating a cross-compartment script reference, which is
  // forbidden. We can still store a CCW to the script source object though.
  RootedValue introductionScript(cx);
  if (JSScript* script = options.introductionScript()) {
    if (script->compartment() == cx->compartment()) {
      introductionScript.setPrivateGCThing(options.introductionScript());
    }
  }
  source->setReservedSlot(INTRODUCTION_SCRIPT_SLOT, introductionScript);

  // Set the private value to that of the script or module that this source is
  // part of, if any.
  RootedValue privateValue(cx);
  if (JSScript* script = options.scriptOrModule()) {
    privateValue = script->sourceObject()->canonicalPrivate();
    if (!JS_WrapValue(cx, &privateValue)) {
      return false;
    }
  }
  source->setPrivate(cx->runtime(), privateValue);

  return true;
}

/* static */
bool ScriptSourceObject::initElementProperties(JSContext* cx,
                                               HandleScriptSourceObject source,
                                               HandleObject element,
                                               HandleString elementAttrName) {
  MOZ_ASSERT(source->isCanonical());

  RootedValue elementValue(cx, ObjectOrNullValue(element));
  if (!cx->compartment()->wrap(cx, &elementValue)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // XXX On-demand source is currently only UTF-16.  Perhaps it should be
  //     changed to UTF-8, or UTF-8 be allowed in addition to UTF-16?
  if (!ss->setSource(cx, EntryUnits<char16_t>(src), length)) {
    return false;
  }
||||||| merged common ancestors
/* static */ bool
JSScript::loadSource(JSContext* cx, ScriptSource* ss, bool* worked)
{
    MOZ_ASSERT(!ss->hasSourceText());
    *worked = false;
    if (!cx->runtime()->sourceHook.ref() || !ss->sourceRetrievable()) {
        return true;
    }
    char16_t* src = nullptr;
    size_t length;
    if (!cx->runtime()->sourceHook->load(cx, ss->filename(), &src, &length)) {
        return false;
    }
    if (!src) {
        return true;
    }

    // XXX On-demand source is currently only UTF-16.  Perhaps it should be
    //     changed to UTF-8, or UTF-8 be allowed in addition to UTF-16?
    if (!ss->setSource(cx, EntryUnits<char16_t>(src), length)) {
        return false;
    }
=======
  RootedValue nameValue(cx);
  if (elementAttrName) {
    nameValue = StringValue(elementAttrName);
  }
  if (!cx->compartment()->wrap(cx, &nameValue)) {
    return false;
  }

  source->setReservedSlot(ELEMENT_SLOT, elementValue);
  source->setReservedSlot(ELEMENT_PROPERTY_SLOT, nameValue);
>>>>>>> upstream-releases

<<<<<<< HEAD
  *worked = true;
  return true;
||||||| merged common ancestors
    *worked = true;
    return true;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ JSFlatString* JSScript::sourceData(JSContext* cx,
                                                HandleScript script) {
  MOZ_ASSERT(script->scriptSource()->hasSourceText());
  return script->scriptSource()->substring(cx, script->sourceStart(),
                                           script->sourceEnd());
||||||| merged common ancestors
/* static */ JSFlatString*
JSScript::sourceData(JSContext* cx, HandleScript script)
{
    MOZ_ASSERT(script->scriptSource()->hasSourceText());
    return script->scriptSource()->substring(cx, script->sourceStart(), script->sourceEnd());
=======
void ScriptSourceObject::setPrivate(JSRuntime* rt, const Value& value) {
  // Update the private value, calling addRef/release hooks if necessary
  // to allow the embedding to maintain a reference count for the
  // private data.
  JS::AutoSuppressGCAnalysis nogc;
  Value prevValue = getReservedSlot(PRIVATE_SLOT);
  rt->releaseScriptPrivate(prevValue);
  setReservedSlot(PRIVATE_SLOT, value);
  rt->addRefScriptPrivate(value);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool JSScript::appendSourceDataForToString(JSContext* cx, StringBuffer& buf) {
  MOZ_ASSERT(scriptSource()->hasSourceText());
  return scriptSource()->appendSubstring(cx, buf, toStringStart(),
                                         toStringEnd());
}
||||||| merged common ancestors
bool
JSScript::appendSourceDataForToString(JSContext* cx, StringBuffer& buf)
{
    MOZ_ASSERT(scriptSource()->hasSourceText());
    return scriptSource()->appendSubstring(cx, buf, toStringStart(), toStringEnd());
}
=======
class ScriptSource::LoadSourceMatcher {
  JSContext* const cx_;
  ScriptSource* const ss_;
  bool* const loaded_;
>>>>>>> upstream-releases

<<<<<<< HEAD
void UncompressedSourceCache::holdEntry(AutoHoldEntry& holder,
                                        const ScriptSourceChunk& ssc) {
  MOZ_ASSERT(!holder_);
  holder.holdEntry(this, ssc);
  holder_ = &holder;
}
||||||| merged common ancestors
void
UncompressedSourceCache::holdEntry(AutoHoldEntry& holder, const ScriptSourceChunk& ssc)
{
    MOZ_ASSERT(!holder_);
    holder.holdEntry(this, ssc);
    holder_ = &holder;
}
=======
 public:
  explicit LoadSourceMatcher(JSContext* cx, ScriptSource* ss, bool* loaded)
      : cx_(cx), ss_(ss), loaded_(loaded) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
void UncompressedSourceCache::releaseEntry(AutoHoldEntry& holder) {
  MOZ_ASSERT(holder_ == &holder);
  holder_ = nullptr;
}
||||||| merged common ancestors
void
UncompressedSourceCache::releaseEntry(AutoHoldEntry& holder)
{
    MOZ_ASSERT(holder_ == &holder);
    holder_ = nullptr;
}
=======
  template <typename Unit, SourceRetrievable CanRetrieve>
  bool operator()(const Compressed<Unit, CanRetrieve>&) const {
    *loaded_ = true;
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Unit>
const Unit* UncompressedSourceCache::lookup(const ScriptSourceChunk& ssc,
                                            AutoHoldEntry& holder) {
  MOZ_ASSERT(!holder_);
  MOZ_ASSERT(ssc.ss->compressedSourceIs<Unit>());
||||||| merged common ancestors
template<typename Unit>
const Unit*
UncompressedSourceCache::lookup(const ScriptSourceChunk& ssc, AutoHoldEntry& holder)
{
    MOZ_ASSERT(!holder_);
    MOZ_ASSERT(ssc.ss->compressedSourceIs<Unit>());

    if (!map_) {
        return nullptr;
    }

    if (Map::Ptr p = map_->lookup(ssc)) {
        holdEntry(holder, ssc);
        return static_cast<const Unit*>(p->value().get());
    }
=======
  template <typename Unit, SourceRetrievable CanRetrieve>
  bool operator()(const Uncompressed<Unit, CanRetrieve>&) const {
    *loaded_ = true;
    return true;
  }

  template <typename Unit>
  bool operator()(const Retrievable<Unit>&) {
    if (!cx_->runtime()->sourceHook.ref()) {
      *loaded_ = false;
      return true;
    }

    size_t length;

    // The first argument is just for overloading -- its value doesn't matter.
    if (!tryLoadAndSetSource(Unit('0'), &length)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!map_) {
    return nullptr;
  }
||||||| merged common ancestors
    return nullptr;
}

bool
UncompressedSourceCache::put(const ScriptSourceChunk& ssc, SourceData data, AutoHoldEntry& holder)
{
    MOZ_ASSERT(!holder_);

    if (!map_) {
        map_ = MakeUnique<Map>();
        if (!map_) {
            return false;
        }
    }

    if (!map_->put(ssc, std::move(data))) {
        return false;
    }
=======
    cx_->updateMallocCounter(length * sizeof(Unit));

    return true;
  }

  bool operator()(const Missing&) const {
    *loaded_ = false;
    return true;
  }

  bool operator()(const BinAST&) const {
    *loaded_ = false;
    return true;
  }

 private:
  bool tryLoadAndSetSource(const Utf8Unit&, size_t* length) const {
    char* utf8Source;
    if (!cx_->runtime()->sourceHook->load(cx_, ss_->filename(), nullptr,
                                          &utf8Source, length)) {
      return false;
    }

    if (!utf8Source) {
      *loaded_ = false;
      return true;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (Map::Ptr p = map_->lookup(ssc)) {
    holdEntry(holder, ssc);
    return static_cast<const Unit*>(p->value().get());
  }
||||||| merged common ancestors
    holdEntry(holder, ssc);
    return true;
}
=======
    if (!ss_->setRetrievedSource(
            cx_, EntryUnits<Utf8Unit>(reinterpret_cast<Utf8Unit*>(utf8Source)),
            *length)) {
      return false;
    }

    *loaded_ = true;
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return nullptr;
||||||| merged common ancestors
void
UncompressedSourceCache::purge()
{
    if (!map_) {
        return;
    }

    for (Map::Range r = map_->all(); !r.empty(); r.popFront()) {
        if (holder_ && r.front().key() == holder_->sourceChunk()) {
            holder_->deferDelete(std::move(r.front().value()));
            holder_ = nullptr;
        }
    }

    map_ = nullptr;
=======
  bool tryLoadAndSetSource(const char16_t&, size_t* length) const {
    char16_t* utf16Source;
    if (!cx_->runtime()->sourceHook->load(cx_, ss_->filename(), &utf16Source,
                                          nullptr, length)) {
      return false;
    }

    if (!utf16Source) {
      *loaded_ = false;
      return true;
    }

    if (!ss_->setRetrievedSource(cx_, EntryUnits<char16_t>(utf16Source),
                                 *length)) {
      return false;
    }

    *loaded_ = true;
    return true;
  }
};

/* static */
bool ScriptSource::loadSource(JSContext* cx, ScriptSource* ss, bool* loaded) {
  return ss->data.match(LoadSourceMatcher(cx, ss, loaded));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool UncompressedSourceCache::put(const ScriptSourceChunk& ssc, SourceData data,
                                  AutoHoldEntry& holder) {
  MOZ_ASSERT(!holder_);
||||||| merged common ancestors
size_t
UncompressedSourceCache::sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf)
{
    size_t n = 0;
    if (map_ && !map_->empty()) {
        n += map_->shallowSizeOfIncludingThis(mallocSizeOf);
        for (Map::Range r = map_->all(); !r.empty(); r.popFront()) {
            n += mallocSizeOf(r.front().value().get());
        }
    }
    return n;
}

template<typename Unit>
const Unit*
ScriptSource::chunkUnits(JSContext* cx, UncompressedSourceCache::AutoHoldEntry& holder,
                         size_t chunk)
{
    const Compressed<Unit>& c = data.as<Compressed<Unit>>();

    ScriptSourceChunk ssc(this, chunk);
    if (const Unit* decompressed = cx->caches().uncompressedSourceCache.lookup<Unit>(ssc, holder)) {
        return decompressed;
    }

    size_t totalLengthInBytes = length() * sizeof(Unit);
    size_t chunkBytes = Compressor::chunkSize(totalLengthInBytes, chunk);

    MOZ_ASSERT((chunkBytes % sizeof(Unit)) == 0);
    const size_t lengthWithNull = (chunkBytes / sizeof(Unit)) + 1;
    EntryUnits<Unit> decompressed(js_pod_malloc<Unit>(lengthWithNull));
    if (!decompressed) {
        JS_ReportOutOfMemory(cx);
        return nullptr;
    }
=======
/* static */
JSFlatString* JSScript::sourceData(JSContext* cx, HandleScript script) {
  MOZ_ASSERT(script->scriptSource()->hasSourceText());
  return script->scriptSource()->substring(cx, script->sourceStart(),
                                           script->sourceEnd());
}

bool JSScript::appendSourceDataForToString(JSContext* cx, StringBuffer& buf) {
  MOZ_ASSERT(scriptSource()->hasSourceText());
  return scriptSource()->appendSubstring(cx, buf, toStringStart(),
                                         toStringEnd());
}

void UncompressedSourceCache::holdEntry(AutoHoldEntry& holder,
                                        const ScriptSourceChunk& ssc) {
  MOZ_ASSERT(!holder_);
  holder.holdEntry(this, ssc);
  holder_ = &holder;
}

void UncompressedSourceCache::releaseEntry(AutoHoldEntry& holder) {
  MOZ_ASSERT(holder_ == &holder);
  holder_ = nullptr;
}

template <typename Unit>
const Unit* UncompressedSourceCache::lookup(const ScriptSourceChunk& ssc,
                                            AutoHoldEntry& holder) {
  MOZ_ASSERT(!holder_);
  MOZ_ASSERT(ssc.ss->isCompressed<Unit>());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!map_) {
    map_ = MakeUnique<Map>();
    if (!map_) {
      return false;
    }
  }
||||||| merged common ancestors
    // Compression treats input and output memory as plain ol' bytes. These
    // reinterpret_cast<>s accord exactly with that.
    if (!DecompressStringChunk(reinterpret_cast<const unsigned char*>(c.raw.chars()),
                               chunk,
                               reinterpret_cast<unsigned char*>(decompressed.get()),
                               chunkBytes))
    {
        JS_ReportOutOfMemory(cx);
        return nullptr;
    }
=======
  if (!map_) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!map_->put(ssc, std::move(data))) {
    return false;
  }
||||||| merged common ancestors
    decompressed[lengthWithNull - 1] = Unit('\0');
=======
  if (Map::Ptr p = map_->lookup(ssc)) {
    holdEntry(holder, ssc);
    return static_cast<const Unit*>(p->value().get());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  holdEntry(holder, ssc);
  return true;
||||||| merged common ancestors
    const Unit* ret = decompressed.get();
    if (!cx->caches().uncompressedSourceCache.put(ssc, ToSourceData(std::move(decompressed)),
                                                  holder))
    {
        JS_ReportOutOfMemory(cx);
        return nullptr;
    }
    return ret;
=======
  return nullptr;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void UncompressedSourceCache::purge() {
  if (!map_) {
    return;
  }
||||||| merged common ancestors
template<typename Unit>
void
ScriptSource::movePendingCompressedSource()
{
    if (pendingCompressed_.empty()) {
        return;
    }

    Compressed<Unit>& pending = pendingCompressed_.ref<Compressed<Unit>>();
=======
bool UncompressedSourceCache::put(const ScriptSourceChunk& ssc, SourceData data,
                                  AutoHoldEntry& holder) {
  MOZ_ASSERT(!holder_);

  if (!map_) {
    map_ = MakeUnique<Map>();
    if (!map_) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (Map::Range r = map_->all(); !r.empty(); r.popFront()) {
    if (holder_ && r.front().key() == holder_->sourceChunk()) {
      holder_->deferDelete(std::move(r.front().value()));
      holder_ = nullptr;
    }
  }
||||||| merged common ancestors
    MOZ_ASSERT(!hasCompressedSource());
    MOZ_ASSERT_IF(hasUncompressedSource(),
                  pending.uncompressedLength == length());
=======
  if (!map_->put(ssc, std::move(data))) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  map_ = nullptr;
||||||| merged common ancestors
    data = SourceType(std::move(pending));
    pendingCompressed_.destroy();
=======
  holdEntry(holder, ssc);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
size_t UncompressedSourceCache::sizeOfExcludingThis(
    mozilla::MallocSizeOf mallocSizeOf) {
  size_t n = 0;
  if (map_ && !map_->empty()) {
    n += map_->shallowSizeOfIncludingThis(mallocSizeOf);
    for (Map::Range r = map_->all(); !r.empty(); r.popFront()) {
      n += mallocSizeOf(r.front().value().get());
||||||| merged common ancestors
template<typename Unit>
ScriptSource::PinnedUnits<Unit>::~PinnedUnits()
{
    if (units_) {
        MOZ_ASSERT(*stack_ == this);
        *stack_ = prev_;
        if (!prev_) {
            source_->movePendingCompressedSource<Unit>();
        }
=======
void UncompressedSourceCache::purge() {
  if (!map_) {
    return;
  }

  for (Map::Range r = map_->all(); !r.empty(); r.popFront()) {
    if (holder_ && r.front().key() == holder_->sourceChunk()) {
      holder_->deferDelete(std::move(r.front().value()));
      holder_ = nullptr;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
  return n;
}
||||||| merged common ancestors
}
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Unit>
const Unit* ScriptSource::chunkUnits(
    JSContext* cx, UncompressedSourceCache::AutoHoldEntry& holder,
    size_t chunk) {
  const Compressed<Unit>& c = data.as<Compressed<Unit>>();
||||||| merged common ancestors
template<typename Unit>
const Unit*
ScriptSource::units(JSContext* cx, UncompressedSourceCache::AutoHoldEntry& holder,
                    size_t begin, size_t len)
{
    MOZ_ASSERT(begin <= length());
    MOZ_ASSERT(begin + len <= length());

    if (data.is<Uncompressed<Unit>>()) {
        const Unit* units = data.as<Uncompressed<Unit>>().units();
        if (!units) {
            return nullptr;
        }
        return units + begin;
    }

    if (data.is<Missing>()) {
        MOZ_CRASH("ScriptSource::units() on ScriptSource with SourceType = Missing");
    }

    MOZ_ASSERT(data.is<Compressed<Unit>>());

    // Determine which chunk(s) we are interested in, and the offsets within
    // these chunks.
    size_t firstChunk, lastChunk;
    size_t firstChunkOffset, lastChunkOffset;
    MOZ_ASSERT(len > 0);
    Compressor::toChunkOffset(begin * sizeof(Unit), &firstChunk, &firstChunkOffset);
    Compressor::toChunkOffset((begin + len - 1) * sizeof(Unit), &lastChunk, &lastChunkOffset);
=======
  map_ = nullptr;
}

size_t UncompressedSourceCache::sizeOfExcludingThis(
    mozilla::MallocSizeOf mallocSizeOf) {
  size_t n = 0;
  if (map_ && !map_->empty()) {
    n += map_->shallowSizeOfIncludingThis(mallocSizeOf);
    for (Map::Range r = map_->all(); !r.empty(); r.popFront()) {
      n += mallocSizeOf(r.front().value().get());
    }
  }
  return n;
}

template <typename Unit>
const Unit* ScriptSource::chunkUnits(
    JSContext* cx, UncompressedSourceCache::AutoHoldEntry& holder,
    size_t chunk) {
  const CompressedData<Unit>& c = *compressedData<Unit>();

  ScriptSourceChunk ssc(this, chunk);
  if (const Unit* decompressed =
          cx->caches().uncompressedSourceCache.lookup<Unit>(ssc, holder)) {
    return decompressed;
  }

  size_t totalLengthInBytes = length() * sizeof(Unit);
  size_t chunkBytes = Compressor::chunkSize(totalLengthInBytes, chunk);
>>>>>>> upstream-releases

<<<<<<< HEAD
  ScriptSourceChunk ssc(this, chunk);
  if (const Unit* decompressed =
          cx->caches().uncompressedSourceCache.lookup<Unit>(ssc, holder)) {
    return decompressed;
  }
||||||| merged common ancestors
    MOZ_ASSERT(firstChunkOffset % sizeof(Unit) == 0);
    size_t firstUnit = firstChunkOffset / sizeof(Unit);
=======
  MOZ_ASSERT((chunkBytes % sizeof(Unit)) == 0);
  const size_t chunkLength = chunkBytes / sizeof(Unit);
  EntryUnits<Unit> decompressed(js_pod_malloc<Unit>(chunkLength));
  if (!decompressed) {
    JS_ReportOutOfMemory(cx);
    return nullptr;
  }

  // Compression treats input and output memory as plain ol' bytes. These
  // reinterpret_cast<>s accord exactly with that.
  if (!DecompressStringChunk(
          reinterpret_cast<const unsigned char*>(c.raw.chars()), chunk,
          reinterpret_cast<unsigned char*>(decompressed.get()), chunkBytes)) {
    JS_ReportOutOfMemory(cx);
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t totalLengthInBytes = length() * sizeof(Unit);
  size_t chunkBytes = Compressor::chunkSize(totalLengthInBytes, chunk);
||||||| merged common ancestors
    if (firstChunk == lastChunk) {
        const Unit* units = chunkUnits<Unit>(cx, holder, firstChunk);
        if (!units) {
            return nullptr;
        }
=======
  const Unit* ret = decompressed.get();
  if (!cx->caches().uncompressedSourceCache.put(
          ssc, ToSourceData(std::move(decompressed)), holder)) {
    JS_ReportOutOfMemory(cx);
    return nullptr;
  }
  return ret;
}

template <typename Unit>
void ScriptSource::convertToCompressedSource(SharedImmutableString compressed,
                                             size_t uncompressedLength) {
  MOZ_ASSERT(isUncompressed<Unit>());
  MOZ_ASSERT(uncompressedData<Unit>()->length() == uncompressedLength);

  if (data.is<Uncompressed<Unit, SourceRetrievable::Yes>>()) {
    data = SourceType(Compressed<Unit, SourceRetrievable::Yes>(
        std::move(compressed), uncompressedLength));
  } else {
    data = SourceType(Compressed<Unit, SourceRetrievable::No>(
        std::move(compressed), uncompressedLength));
  }
}

template <typename Unit>
void ScriptSource::performDelayedConvertToCompressedSource() {
  // There might not be a conversion to compressed source happening at all.
  if (pendingCompressed_.empty()) {
    return;
  }

  CompressedData<Unit>& pending =
      pendingCompressed_.ref<CompressedData<Unit>>();

  convertToCompressedSource<Unit>(std::move(pending.raw),
                                  pending.uncompressedLength);

  pendingCompressed_.destroy();
}

template <typename Unit>
ScriptSource::PinnedUnits<Unit>::~PinnedUnits() {
  if (units_) {
    MOZ_ASSERT(*stack_ == this);
    *stack_ = prev_;
    if (!prev_) {
      source_->performDelayedConvertToCompressedSource<Unit>();
    }
  }
}

template <typename Unit>
const Unit* ScriptSource::units(JSContext* cx,
                                UncompressedSourceCache::AutoHoldEntry& holder,
                                size_t begin, size_t len) {
  MOZ_ASSERT(begin <= length());
  MOZ_ASSERT(begin + len <= length());

  if (isUncompressed<Unit>()) {
    const Unit* units = uncompressedData<Unit>()->units();
    if (!units) {
      return nullptr;
    }
    return units + begin;
  }

  if (data.is<Missing>()) {
    MOZ_CRASH("ScriptSource::units() on ScriptSource with missing source");
  }

  if (data.is<Retrievable<Unit>>()) {
    MOZ_CRASH("ScriptSource::units() on ScriptSource with retrievable source");
  }

  MOZ_ASSERT(isCompressed<Unit>());

  // Determine first/last chunks, the offset (in bytes) into the first chunk
  // of the requested units, and the number of bytes in the last chunk.
  //
  // Note that first and last chunk sizes are miscomputed and *must not be
  // used* when the first chunk is the last chunk.
  size_t firstChunk, firstChunkOffset, firstChunkSize;
  size_t lastChunk, lastChunkSize;
  Compressor::rangeToChunkAndOffset(
      begin * sizeof(Unit), (begin + len) * sizeof(Unit), &firstChunk,
      &firstChunkOffset, &firstChunkSize, &lastChunk, &lastChunkSize);
  MOZ_ASSERT(firstChunk <= lastChunk);
  MOZ_ASSERT(firstChunkOffset % sizeof(Unit) == 0);
  MOZ_ASSERT(firstChunkSize % sizeof(Unit) == 0);

  size_t firstUnit = firstChunkOffset / sizeof(Unit);

  // Directly return units within a single chunk.  UncompressedSourceCache
  // and |holder| will hold the units alive past function return.
  if (firstChunk == lastChunk) {
    const Unit* units = chunkUnits<Unit>(cx, holder, firstChunk);
    if (!units) {
      return nullptr;
    }

    return units + firstUnit;
  }

  // Otherwise the units span multiple chunks.  Copy successive chunks'
  // decompressed units into freshly-allocated memory to return.
  EntryUnits<Unit> decompressed(js_pod_malloc<Unit>(len));
  if (!decompressed) {
    JS_ReportOutOfMemory(cx);
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT((chunkBytes % sizeof(Unit)) == 0);
  const size_t chunkLength = chunkBytes / sizeof(Unit);
  EntryUnits<Unit> decompressed(js_pod_malloc<Unit>(chunkLength));
  if (!decompressed) {
    JS_ReportOutOfMemory(cx);
    return nullptr;
  }

  // Compression treats input and output memory as plain ol' bytes. These
  // reinterpret_cast<>s accord exactly with that.
  if (!DecompressStringChunk(
          reinterpret_cast<const unsigned char*>(c.raw.chars()), chunk,
          reinterpret_cast<unsigned char*>(decompressed.get()), chunkBytes)) {
    JS_ReportOutOfMemory(cx);
    return nullptr;
  }
||||||| merged common ancestors
        return units + firstUnit;
    }
=======
  Unit* cursor;
>>>>>>> upstream-releases

<<<<<<< HEAD
  const Unit* ret = decompressed.get();
  if (!cx->caches().uncompressedSourceCache.put(
          ssc, ToSourceData(std::move(decompressed)), holder)) {
    JS_ReportOutOfMemory(cx);
    return nullptr;
  }
  return ret;
}

template <typename Unit>
void ScriptSource::movePendingCompressedSource() {
  if (pendingCompressed_.empty()) {
    return;
  }

  Compressed<Unit>& pending = pendingCompressed_.ref<Compressed<Unit>>();

  MOZ_ASSERT(!hasCompressedSource());
  MOZ_ASSERT_IF(hasUncompressedSource(),
                pending.uncompressedLength == length());

  data = SourceType(std::move(pending));
  pendingCompressed_.destroy();
}

template <typename Unit>
ScriptSource::PinnedUnits<Unit>::~PinnedUnits() {
  if (units_) {
    MOZ_ASSERT(*stack_ == this);
    *stack_ = prev_;
    if (!prev_) {
      source_->movePendingCompressedSource<Unit>();
    }
  }
}

template <typename Unit>
const Unit* ScriptSource::units(JSContext* cx,
                                UncompressedSourceCache::AutoHoldEntry& holder,
                                size_t begin, size_t len) {
  MOZ_ASSERT(begin <= length());
  MOZ_ASSERT(begin + len <= length());

  if (data.is<Uncompressed<Unit>>()) {
    const Unit* units = data.as<Uncompressed<Unit>>().units();
    if (!units) {
      return nullptr;
    }
    return units + begin;
  }

  if (data.is<Missing>()) {
    MOZ_CRASH(
        "ScriptSource::units() on ScriptSource with SourceType = Missing");
  }

  MOZ_ASSERT(data.is<Compressed<Unit>>());

  // Determine first/last chunks, the offset (in bytes) into the first chunk
  // of the requested units, and the number of bytes in the last chunk.
  //
  // Note that first and last chunk sizes are miscomputed and *must not be
  // used* when the first chunk is the last chunk.
  size_t firstChunk, firstChunkOffset, firstChunkSize;
  size_t lastChunk, lastChunkSize;
  Compressor::rangeToChunkAndOffset(
      begin * sizeof(Unit), (begin + len) * sizeof(Unit), &firstChunk,
      &firstChunkOffset, &firstChunkSize, &lastChunk, &lastChunkSize);
  MOZ_ASSERT(firstChunk <= lastChunk);
  MOZ_ASSERT(firstChunkOffset % sizeof(Unit) == 0);
  MOZ_ASSERT(firstChunkSize % sizeof(Unit) == 0);

  size_t firstUnit = firstChunkOffset / sizeof(Unit);

  // Directly return units within a single chunk.  UncompressedSourceCache
  // and |holder| will hold the units alive past function return.
  if (firstChunk == lastChunk) {
    const Unit* units = chunkUnits<Unit>(cx, holder, firstChunk);
    if (!units) {
      return nullptr;
    }

    return units + firstUnit;
  }

  // Otherwise the units span multiple chunks.  Copy successive chunks'
  // decompressed units into freshly-allocated memory to return.
  EntryUnits<Unit> decompressed(js_pod_malloc<Unit>(len));
  if (!decompressed) {
    JS_ReportOutOfMemory(cx);
    return nullptr;
  }
||||||| merged common ancestors
    // We need multiple chunks. Allocate a (null-terminated) buffer to hold
    // |len| units and copy uncompressed units from the chunks into it. We use
    // chunkUnits() so we benefit from chunk caching by UncompressedSourceCache.
=======
  {
    // |AutoHoldEntry| is single-shot, and a holder successfully filled in
    // by |chunkUnits| must be destroyed before another can be used.  Thus
    // we can't use |holder| with |chunkUnits| when |chunkUnits| is used
    // with multiple chunks, and we must use and destroy distinct, fresh
    // holders for each chunk.
    UncompressedSourceCache::AutoHoldEntry firstHolder;
    const Unit* units = chunkUnits<Unit>(cx, firstHolder, firstChunk);
    if (!units) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Unit* cursor;
||||||| merged common ancestors
    MOZ_ASSERT(firstChunk < lastChunk);
=======
    cursor = std::copy_n(units + firstUnit, firstChunkSize / sizeof(Unit),
                         decompressed.get());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  {
    // |AutoHoldEntry| is single-shot, and a holder successfully filled in
    // by |chunkUnits| must be destroyed before another can be used.  Thus
    // we can't use |holder| with |chunkUnits| when |chunkUnits| is used
    // with multiple chunks, and we must use and destroy distinct, fresh
    // holders for each chunk.
    UncompressedSourceCache::AutoHoldEntry firstHolder;
    const Unit* units = chunkUnits<Unit>(cx, firstHolder, firstChunk);
    if (!units) {
      return nullptr;
||||||| merged common ancestors
    size_t lengthWithNull = len + 1;
    EntryUnits<Unit> decompressed(js_pod_malloc<Unit>(lengthWithNull));
    if (!decompressed) {
        JS_ReportOutOfMemory(cx);
        return nullptr;
=======
  for (size_t i = firstChunk + 1; i < lastChunk; i++) {
    UncompressedSourceCache::AutoHoldEntry chunkHolder;
    const Unit* units = chunkUnits<Unit>(cx, chunkHolder, i);
    if (!units) {
      return nullptr;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    cursor = std::copy_n(units + firstUnit, firstChunkSize / sizeof(Unit),
                         decompressed.get());
  }

  for (size_t i = firstChunk + 1; i < lastChunk; i++) {
    UncompressedSourceCache::AutoHoldEntry chunkHolder;
    const Unit* units = chunkUnits<Unit>(cx, chunkHolder, i);
    if (!units) {
      return nullptr;
||||||| merged common ancestors
    size_t totalLengthInBytes = length() * sizeof(Unit);
    Unit* cursor = decompressed.get();

    for (size_t i = firstChunk; i <= lastChunk; i++) {
        UncompressedSourceCache::AutoHoldEntry chunkHolder;
        const Unit* units = chunkUnits<Unit>(cx, chunkHolder, i);
        if (!units) {
            return nullptr;
        }

        size_t numUnits = Compressor::chunkSize(totalLengthInBytes, i) / sizeof(Unit);
        if (i == firstChunk) {
            MOZ_ASSERT(firstUnit < numUnits);
            units += firstUnit;
            numUnits -= firstUnit;
        } else if (i == lastChunk) {
            size_t numUnitsNew = lastChunkOffset / sizeof(Unit) + 1;
            MOZ_ASSERT(numUnitsNew <= numUnits);
            numUnits = numUnitsNew;
        }
        mozilla::PodCopy(cursor, units, numUnits);
        cursor += numUnits;
=======
    cursor = std::copy_n(units, Compressor::CHUNK_SIZE / sizeof(Unit), cursor);
  }

  {
    UncompressedSourceCache::AutoHoldEntry lastHolder;
    const Unit* units = chunkUnits<Unit>(cx, lastHolder, lastChunk);
    if (!units) {
      return nullptr;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    cursor = std::copy_n(units, Compressor::CHUNK_SIZE / sizeof(Unit), cursor);
  }
||||||| merged common ancestors
    // XXX Bug 1499192: can we remove the null-termination?  It's unclear if
    //     anyone uses chunk implicit null-termination, chunks can contain
    //     nulls anyway, and the extra character risks size-class goofs.
    *cursor++ = Unit('\0');
    MOZ_ASSERT(PointerRangeSize(decompressed.get(), cursor) == lengthWithNull);
=======
    cursor = std::copy_n(units, lastChunkSize / sizeof(Unit), cursor);
  }

  MOZ_ASSERT(PointerRangeSize(decompressed.get(), cursor) == len);
>>>>>>> upstream-releases

<<<<<<< HEAD
  {
    UncompressedSourceCache::AutoHoldEntry lastHolder;
    const Unit* units = chunkUnits<Unit>(cx, lastHolder, lastChunk);
    if (!units) {
      return nullptr;
    }
||||||| merged common ancestors
    // Transfer ownership to |holder|.
    const Unit* ret = decompressed.get();
    holder.holdUnits(std::move(decompressed));
    return ret;
}

template<typename Unit>
ScriptSource::PinnedUnits<Unit>::PinnedUnits(JSContext* cx, ScriptSource* source,
                                             UncompressedSourceCache::AutoHoldEntry& holder,
                                             size_t begin, size_t len)
  : PinnedUnitsBase(source)
{
    MOZ_ASSERT(source->hasSourceType<Unit>(),
               "must pin units of source's type");

    units_ = source->units<Unit>(cx, holder, begin, len);
    if (units_) {
        stack_ = &source->pinnedUnitsStack_;
        prev_ = *stack_;
        *stack_ = this;
    }
}

template class ScriptSource::PinnedUnits<Utf8Unit>;
template class ScriptSource::PinnedUnits<char16_t>;

JSFlatString*
ScriptSource::substring(JSContext* cx, size_t start, size_t stop)
{
    MOZ_ASSERT(start <= stop);
=======
  // Transfer ownership to |holder|.
  const Unit* ret = decompressed.get();
  holder.holdUnits(std::move(decompressed));
  return ret;
}

template <typename Unit>
ScriptSource::PinnedUnits<Unit>::PinnedUnits(
    JSContext* cx, ScriptSource* source,
    UncompressedSourceCache::AutoHoldEntry& holder, size_t begin, size_t len)
    : PinnedUnitsBase(source) {
  MOZ_ASSERT(source->hasSourceType<Unit>(), "must pin units of source's type");

  units_ = source->units<Unit>(cx, holder, begin, len);
  if (units_) {
    stack_ = &source->pinnedUnitsStack_;
    prev_ = *stack_;
    *stack_ = this;
  }
}

template class ScriptSource::PinnedUnits<Utf8Unit>;
template class ScriptSource::PinnedUnits<char16_t>;

JSFlatString* ScriptSource::substring(JSContext* cx, size_t start,
                                      size_t stop) {
  MOZ_ASSERT(start <= stop);
>>>>>>> upstream-releases

<<<<<<< HEAD
    cursor = std::copy_n(units, lastChunkSize / sizeof(Unit), cursor);
  }
||||||| merged common ancestors
    size_t len = stop - start;
    UncompressedSourceCache::AutoHoldEntry holder;
=======
  size_t len = stop - start;
  UncompressedSourceCache::AutoHoldEntry holder;
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(PointerRangeSize(decompressed.get(), cursor) == len);
||||||| merged common ancestors
    // UTF-8 source text.
    if (hasSourceType<Utf8Unit>()) {
        PinnedUnits<Utf8Unit> units(cx, this, holder, start, len);
        if (!units.asChars()) {
            return nullptr;
        }

        const char* str = units.asChars();
        return NewStringCopyUTF8N<CanGC>(cx, JS::UTF8Chars(str, len));
    }

    // UTF-16 source text.
    PinnedUnits<char16_t> units(cx, this, holder, start, len);
    if (!units.asChars()) {
        return nullptr;
    }
=======
  // UTF-8 source text.
  if (hasSourceType<Utf8Unit>()) {
    PinnedUnits<Utf8Unit> units(cx, this, holder, start, len);
    if (!units.asChars()) {
      return nullptr;
    }

    const char* str = units.asChars();
    return NewStringCopyUTF8N<CanGC>(cx, JS::UTF8Chars(str, len));
  }

  // UTF-16 source text.
  PinnedUnits<char16_t> units(cx, this, holder, start, len);
  if (!units.asChars()) {
    return nullptr;
  }

  return NewStringCopyN<CanGC>(cx, units.asChars(), len);
}

JSFlatString* ScriptSource::substringDontDeflate(JSContext* cx, size_t start,
                                                 size_t stop) {
  MOZ_ASSERT(start <= stop);

  size_t len = stop - start;
  UncompressedSourceCache::AutoHoldEntry holder;

  // UTF-8 source text.
  if (hasSourceType<Utf8Unit>()) {
    PinnedUnits<Utf8Unit> units(cx, this, holder, start, len);
    if (!units.asChars()) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Transfer ownership to |holder|.
  const Unit* ret = decompressed.get();
  holder.holdUnits(std::move(decompressed));
  return ret;
}
||||||| merged common ancestors
    return NewStringCopyN<CanGC>(cx, units.asChars(), len);
}
=======
    const char* str = units.asChars();

    // There doesn't appear to be a non-deflating UTF-8 string creation
    // function -- but then again, it's not entirely clear how current
    // callers benefit from non-deflation.
    return NewStringCopyUTF8N<CanGC>(cx, JS::UTF8Chars(str, len));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Unit>
ScriptSource::PinnedUnits<Unit>::PinnedUnits(
    JSContext* cx, ScriptSource* source,
    UncompressedSourceCache::AutoHoldEntry& holder, size_t begin, size_t len)
    : PinnedUnitsBase(source) {
  MOZ_ASSERT(source->hasSourceType<Unit>(), "must pin units of source's type");
||||||| merged common ancestors
JSFlatString*
ScriptSource::substringDontDeflate(JSContext* cx, size_t start, size_t stop)
{
    MOZ_ASSERT(start <= stop);
=======
  // UTF-16 source text.
  PinnedUnits<char16_t> units(cx, this, holder, start, len);
  if (!units.asChars()) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  units_ = source->units<Unit>(cx, holder, begin, len);
  if (units_) {
    stack_ = &source->pinnedUnitsStack_;
    prev_ = *stack_;
    *stack_ = this;
  }
}
||||||| merged common ancestors
    size_t len = stop - start;
    UncompressedSourceCache::AutoHoldEntry holder;
=======
  return NewStringCopyNDontDeflate<CanGC>(cx, units.asChars(), len);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template class ScriptSource::PinnedUnits<Utf8Unit>;
template class ScriptSource::PinnedUnits<char16_t>;
||||||| merged common ancestors
    // UTF-8 source text.
    if (hasSourceType<Utf8Unit>()) {
        PinnedUnits<Utf8Unit> units(cx, this, holder, start, len);
        if (!units.asChars()) {
            return nullptr;
        }
=======
bool ScriptSource::appendSubstring(JSContext* cx, StringBuffer& buf,
                                   size_t start, size_t stop) {
  MOZ_ASSERT(start <= stop);
>>>>>>> upstream-releases

<<<<<<< HEAD
JSFlatString* ScriptSource::substring(JSContext* cx, size_t start,
                                      size_t stop) {
  MOZ_ASSERT(start <= stop);
||||||| merged common ancestors
        const char* str = units.asChars();
=======
  size_t len = stop - start;
  UncompressedSourceCache::AutoHoldEntry holder;
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t len = stop - start;
  UncompressedSourceCache::AutoHoldEntry holder;
||||||| merged common ancestors
        // There doesn't appear to be a non-deflating UTF-8 string creation
        // function -- but then again, it's not entirely clear how current
        // callers benefit from non-deflation.
        return NewStringCopyUTF8N<CanGC>(cx, JS::UTF8Chars(str, len));
    }
=======
  if (hasSourceType<Utf8Unit>()) {
    PinnedUnits<Utf8Unit> pinned(cx, this, holder, start, len);
    if (!pinned.get()) {
      return false;
    }
    if (len > SourceDeflateLimit && !buf.ensureTwoByteChars()) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // UTF-8 source text.
  if (hasSourceType<Utf8Unit>()) {
    PinnedUnits<Utf8Unit> units(cx, this, holder, start, len);
    if (!units.asChars()) {
      return nullptr;
||||||| merged common ancestors
    // UTF-16 source text.
    PinnedUnits<char16_t> units(cx, this, holder, start, len);
    if (!units.asChars()) {
        return nullptr;
=======
    const Utf8Unit* units = pinned.get();
    return buf.append(units, len);
  } else {
    PinnedUnits<char16_t> pinned(cx, this, holder, start, len);
    if (!pinned.get()) {
      return false;
    }
    if (len > SourceDeflateLimit && !buf.ensureTwoByteChars()) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    const char* str = units.asChars();
    return NewStringCopyUTF8N<CanGC>(cx, JS::UTF8Chars(str, len));
  }
||||||| merged common ancestors
    return NewStringCopyNDontDeflate<CanGC>(cx, units.asChars(), len);
}

bool
ScriptSource::appendSubstring(JSContext* cx, StringBuffer& buf, size_t start, size_t stop)
{
    MOZ_ASSERT(start <= stop);
=======
    const char16_t* units = pinned.get();
    return buf.append(units, len);
  }
}

JSFlatString* ScriptSource::functionBodyString(JSContext* cx) {
  MOZ_ASSERT(isFunctionBody());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // UTF-16 source text.
  PinnedUnits<char16_t> units(cx, this, holder, start, len);
  if (!units.asChars()) {
    return nullptr;
  }

  return NewStringCopyN<CanGC>(cx, units.asChars(), len);
||||||| merged common ancestors
    size_t len = stop - start;
    UncompressedSourceCache::AutoHoldEntry holder;

    if (hasSourceType<Utf8Unit>()) {
        MOZ_CRASH("for now");
        return false;
    } else {
        PinnedUnits<char16_t> units(cx, this, holder, start, len);
        if (!units.asChars()) {
            return false;
        }
        if (len > SourceDeflateLimit && !buf.ensureTwoByteChars()) {
            return false;
        }
        return buf.append(units.asChars(), len);
    }
=======
  size_t start =
      parameterListEnd_ + (sizeof(FunctionConstructorMedialSigils) - 1);
  size_t stop = length() - (sizeof(FunctionConstructorFinalBrace) - 1);
  return substring(cx, start, stop);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSFlatString* ScriptSource::substringDontDeflate(JSContext* cx, size_t start,
                                                 size_t stop) {
  MOZ_ASSERT(start <= stop);
||||||| merged common ancestors
JSFlatString*
ScriptSource::functionBodyString(JSContext* cx)
{
    MOZ_ASSERT(isFunctionBody());
=======
template <typename Unit>
MOZ_MUST_USE bool ScriptSource::setUncompressedSourceHelper(
    JSContext* cx, EntryUnits<Unit>&& source, size_t length,
    SourceRetrievable retrievable) {
  auto& cache = cx->zone()->runtimeFromAnyThread()->sharedImmutableStrings();
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t len = stop - start;
  UncompressedSourceCache::AutoHoldEntry holder;
||||||| merged common ancestors
    size_t start = parameterListEnd_ + (sizeof(FunctionConstructorMedialSigils) - 1);
    size_t stop = length() - (sizeof(FunctionConstructorFinalBrace) - 1);
    return substring(cx, start, stop);
}
=======
  auto uniqueChars = SourceTypeTraits<Unit>::toCacheable(std::move(source));
  auto deduped = cache.getOrCreate(std::move(uniqueChars), length);
  if (!deduped) {
    ReportOutOfMemory(cx);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // UTF-8 source text.
  if (hasSourceType<Utf8Unit>()) {
    PinnedUnits<Utf8Unit> units(cx, this, holder, start, len);
    if (!units.asChars()) {
      return nullptr;
    }
||||||| merged common ancestors
template<typename Unit>
void
ScriptSource::setSource(typename SourceTypeTraits<Unit>::SharedImmutableString uncompressed)
{
    MOZ_ASSERT(data.is<Missing>());
    data = SourceType(Uncompressed<Unit>(std::move(uncompressed)));
}
=======
  if (retrievable == SourceRetrievable::Yes) {
    data = SourceType(
        Uncompressed<Unit, SourceRetrievable::Yes>(std::move(*deduped)));
  } else {
    data = SourceType(
        Uncompressed<Unit, SourceRetrievable::No>(std::move(*deduped)));
  }
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    const char* str = units.asChars();

    // There doesn't appear to be a non-deflating UTF-8 string creation
    // function -- but then again, it's not entirely clear how current
    // callers benefit from non-deflation.
    return NewStringCopyUTF8N<CanGC>(cx, JS::UTF8Chars(str, len));
  }

  // UTF-16 source text.
  PinnedUnits<char16_t> units(cx, this, holder, start, len);
  if (!units.asChars()) {
    return nullptr;
  }

  return NewStringCopyNDontDeflate<CanGC>(cx, units.asChars(), len);
||||||| merged common ancestors
template<typename Unit>
MOZ_MUST_USE bool
ScriptSource::setSource(JSContext* cx, EntryUnits<Unit>&& source, size_t length)
{
    auto& cache = cx->zone()->runtimeFromAnyThread()->sharedImmutableStrings();

    auto uniqueChars = SourceTypeTraits<Unit>::toCacheable(std::move(source));
    auto deduped = cache.getOrCreate(std::move(uniqueChars), length);
    if (!deduped) {
        ReportOutOfMemory(cx);
        return false;
    }

    setSource<Unit>(std::move(*deduped));
    return true;
=======
template <typename Unit>
MOZ_MUST_USE bool ScriptSource::setRetrievedSource(JSContext* cx,
                                                   EntryUnits<Unit>&& source,
                                                   size_t length) {
  MOZ_ASSERT(data.is<Retrievable<Unit>>(),
             "retrieved source can only overwrite the corresponding "
             "retrievable source");
  return setUncompressedSourceHelper(cx, std::move(source), length,
                                     SourceRetrievable::Yes);
>>>>>>> upstream-releases
}

bool ScriptSource::appendSubstring(JSContext* cx, StringBuffer& buf,
                                   size_t start, size_t stop) {
  MOZ_ASSERT(start <= stop);

<<<<<<< HEAD
  size_t len = stop - start;
  UncompressedSourceCache::AutoHoldEntry holder;

  if (hasSourceType<Utf8Unit>()) {
    PinnedUnits<Utf8Unit> pinned(cx, this, holder, start, len);
    if (!pinned.get()) {
      return false;
    }
    if (len > SourceDeflateLimit && !buf.ensureTwoByteChars()) {
      return false;
    }
||||||| merged common ancestors
MOZ_MUST_USE bool
ScriptSource::setBinASTSourceCopy(JSContext* cx, const uint8_t* buf, size_t len)
{
    auto &cache = cx->zone()->runtimeFromAnyThread()->sharedImmutableStrings();
    auto deduped = cache.getOrCreate(reinterpret_cast<const char *>(buf), len);
    if (!deduped) {
        ReportOutOfMemory(cx);
        return false;
    }
    MOZ_ASSERT(data.is<Missing>());
    data = SourceType(BinAST(std::move(*deduped)));
    return true;
}
=======
MOZ_MUST_USE bool ScriptSource::setBinASTSourceCopy(JSContext* cx,
                                                    const uint8_t* buf,
                                                    size_t len) {
  MOZ_ASSERT(data.is<Missing>());
>>>>>>> upstream-releases

<<<<<<< HEAD
    const Utf8Unit* units = pinned.get();
    return buf.append(units, len);
  } else {
    PinnedUnits<char16_t> pinned(cx, this, holder, start, len);
    if (!pinned.get()) {
      return false;
    }
    if (len > SourceDeflateLimit && !buf.ensureTwoByteChars()) {
      return false;
    }
||||||| merged common ancestors
MOZ_MUST_USE bool
ScriptSource::setBinASTSource(JSContext* cx, UniqueChars&& buf, size_t len)
{
    auto& cache = cx->zone()->runtimeFromAnyThread()->sharedImmutableStrings();
    auto deduped = cache.getOrCreate(std::move(buf), len);
    if (!deduped) {
        ReportOutOfMemory(cx);
        return false;
    }
    MOZ_ASSERT(data.is<Missing>());
    data = SourceType(BinAST(std::move(*deduped)));
    return true;
}
=======
  auto& cache = cx->zone()->runtimeFromAnyThread()->sharedImmutableStrings();
  auto deduped = cache.getOrCreate(reinterpret_cast<const char*>(buf), len);
  if (!deduped) {
    ReportOutOfMemory(cx);
    return false;
  }

  data = SourceType(BinAST(std::move(*deduped), nullptr));
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    const char16_t* units = pinned.get();
    return buf.append(units, len);
  }
||||||| merged common ancestors
const uint8_t*
ScriptSource::binASTSource()
{
    MOZ_ASSERT(hasBinASTSource());
    return reinterpret_cast<const uint8_t*>(data.as<BinAST>().string.chars());
=======
const uint8_t* ScriptSource::binASTSource() {
  MOZ_ASSERT(hasBinASTSource());
  return reinterpret_cast<const uint8_t*>(data.as<BinAST>().string.chars());
>>>>>>> upstream-releases
}

JSFlatString* ScriptSource::functionBodyString(JSContext* cx) {
  MOZ_ASSERT(isFunctionBody());

<<<<<<< HEAD
  size_t start =
      parameterListEnd_ + (sizeof(FunctionConstructorMedialSigils) - 1);
  size_t stop = length() - (sizeof(FunctionConstructorFinalBrace) - 1);
  return substring(cx, start, stop);
}
||||||| merged common ancestors
bool
ScriptSource::tryCompressOffThread(JSContext* cx)
{
    if (!hasUncompressedSource()) {
        // This excludes already-compressed, missing, and BinAST source.
        return true;
    }
=======
bool ScriptSource::tryCompressOffThread(JSContext* cx) {
  // Beware: |js::SynchronouslyCompressSource| assumes that this function is
  // only called once, just after a script has been compiled, and it's never
  // called at some random time after that.  If multiple calls of this can ever
  // occur, that function may require changes.
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Unit>
void ScriptSource::setSource(
    typename SourceTypeTraits<Unit>::SharedImmutableString uncompressed) {
  MOZ_ASSERT(data.is<Missing>());
  data = SourceType(Uncompressed<Unit>(std::move(uncompressed)));
}
||||||| merged common ancestors
    // There are several cases where source compression is not a good idea:
    //  - If the script is tiny, then compression will save little or no space.
    //  - If there is only one core, then compression will contend with JS
    //    execution (which hurts benchmarketing).
    //
    // Otherwise, enqueue a compression task to be processed when a major
    // GC is requested.

    bool canCompressOffThread =
        HelperThreadState().cpuCount > 1 &&
        HelperThreadState().threadCount >= 2 &&
        CanUseExtraThreads();
    const size_t TINY_SCRIPT = 256;
    if (TINY_SCRIPT > length() || !canCompressOffThread) {
        return true;
    }
=======
  if (!hasUncompressedSource()) {
    // This excludes compressed, missing, retrievable, and BinAST source.
    return true;
  }

  // There are several cases where source compression is not a good idea:
  //  - If the script is tiny, then compression will save little or no space.
  //  - If there is only one core, then compression will contend with JS
  //    execution (which hurts benchmarketing).
  //
  // Otherwise, enqueue a compression task to be processed when a major
  // GC is requested.

  bool canCompressOffThread = HelperThreadState().cpuCount > 1 &&
                              HelperThreadState().threadCount >= 2 &&
                              CanUseExtraThreads();
  if (length() < ScriptSource::MinimumCompressibleLength ||
      !canCompressOffThread) {
    return true;
  }

  // The SourceCompressionTask needs to record the major GC number for
  // scheduling. If we're parsing off thread, this number is not safe to
  // access.
  //
  // When parsing on the main thread, the attempts made to compress off
  // thread in BytecodeCompiler will succeed.
  //
  // When parsing off-thread, the above attempts will fail and the attempt
  // made in ParseTask::finish will succeed.
  if (!CurrentThreadCanAccessRuntime(cx->runtime())) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Unit>
MOZ_MUST_USE bool ScriptSource::setSource(JSContext* cx,
                                          EntryUnits<Unit>&& source,
                                          size_t length) {
  auto& cache = cx->zone()->runtimeFromAnyThread()->sharedImmutableStrings();
||||||| merged common ancestors
    // The SourceCompressionTask needs to record the major GC number for
    // scheduling. If we're parsing off thread, this number is not safe to
    // access.
    //
    // When parsing on the main thread, the attempts made to compress off
    // thread in BytecodeCompiler will succeed.
    //
    // When parsing off-thread, the above attempts will fail and the attempt
    // made in ParseTask::finish will succeed.
    if (!CurrentThreadCanAccessRuntime(cx->runtime())) {
        return true;
    }
=======
  // Heap allocate the task. It will be freed upon compression
  // completing in AttachFinishedCompressedSources.
  auto task = MakeUnique<SourceCompressionTask>(cx->runtime(), this);
  if (!task) {
    ReportOutOfMemory(cx);
    return false;
  }
  return EnqueueOffThreadCompression(cx, std::move(task));
}

template <typename Unit>
void ScriptSource::triggerConvertToCompressedSource(
    SharedImmutableString compressed, size_t uncompressedLength) {
  MOZ_ASSERT(isUncompressed<Unit>(),
             "should only be triggering compressed source installation to "
             "overwrite identically-encoded uncompressed source");
  MOZ_ASSERT(uncompressedData<Unit>()->length() == uncompressedLength);

  // If units aren't pinned -- and they probably won't be, we'd have to have a
  // GC in the small window of time where a |PinnedUnits| was live -- then we
  // can immediately convert.
  if (MOZ_LIKELY(!pinnedUnitsStack_)) {
    convertToCompressedSource<Unit>(std::move(compressed), uncompressedLength);
    return;
  }

  // Otherwise, set aside the compressed-data info.  The conversion is performed
  // when the last |PinnedUnits| dies.
  MOZ_ASSERT(pendingCompressed_.empty(),
             "shouldn't be multiple conversions happening");
  pendingCompressed_.construct<CompressedData<Unit>>(std::move(compressed),
                                                     uncompressedLength);
}

template <typename Unit>
MOZ_MUST_USE bool ScriptSource::initializeWithUnretrievableCompressedSource(
    JSContext* cx, UniqueChars&& compressed, size_t rawLength,
    size_t sourceLength) {
  MOZ_ASSERT(data.is<Missing>(), "shouldn't be double-initializing");
  MOZ_ASSERT(compressed != nullptr);

  auto& cache = cx->zone()->runtimeFromAnyThread()->sharedImmutableStrings();
  auto deduped = cache.getOrCreate(std::move(compressed), rawLength);
  if (!deduped) {
    ReportOutOfMemory(cx);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  auto uniqueChars = SourceTypeTraits<Unit>::toCacheable(std::move(source));
  auto deduped = cache.getOrCreate(std::move(uniqueChars), length);
  if (!deduped) {
    ReportOutOfMemory(cx);
    return false;
  }

  setSource<Unit>(std::move(*deduped));
  return true;
}
||||||| merged common ancestors
    // Heap allocate the task. It will be freed upon compression
    // completing in AttachFinishedCompressedSources.
    auto task = MakeUnique<SourceCompressionTask>(cx->runtime(), this);
    if (!task) {
        ReportOutOfMemory(cx);
        return false;
    }
    return EnqueueOffThreadCompression(cx, std::move(task));
}
=======
  MOZ_ASSERT(pinnedUnitsStack_ == nullptr,
             "shouldn't be initializing a ScriptSource while its characters "
             "are pinned -- that only makes sense with a ScriptSource actively "
             "being inspected");
>>>>>>> upstream-releases

<<<<<<< HEAD
#if defined(JS_BUILD_BINAST)
||||||| merged common ancestors
template<typename Unit>
void
ScriptSource::setCompressedSource(SharedImmutableString raw, size_t uncompressedLength)
{
    MOZ_ASSERT(data.is<Missing>() || hasUncompressedSource());
    MOZ_ASSERT_IF(hasUncompressedSource(), length() == uncompressedLength);
=======
  data = SourceType(Compressed<Unit, SourceRetrievable::No>(std::move(*deduped),
                                                            sourceLength));
>>>>>>> upstream-releases

<<<<<<< HEAD
MOZ_MUST_USE bool ScriptSource::setBinASTSourceCopy(JSContext* cx,
                                                    const uint8_t* buf,
                                                    size_t len) {
  auto& cache = cx->zone()->runtimeFromAnyThread()->sharedImmutableStrings();
  auto deduped = cache.getOrCreate(reinterpret_cast<const char*>(buf), len);
  if (!deduped) {
    ReportOutOfMemory(cx);
    return false;
  }
  MOZ_ASSERT(data.is<Missing>());
  data = SourceType(BinAST(std::move(*deduped)));
  return true;
}

MOZ_MUST_USE bool ScriptSource::setBinASTSource(JSContext* cx,
                                                UniqueChars&& buf, size_t len) {
  auto& cache = cx->zone()->runtimeFromAnyThread()->sharedImmutableStrings();
  auto deduped = cache.getOrCreate(std::move(buf), len);
  if (!deduped) {
    ReportOutOfMemory(cx);
    return false;
  }
  MOZ_ASSERT(data.is<Missing>());
  data = SourceType(BinAST(std::move(*deduped)));
  return true;
||||||| merged common ancestors
    if (pinnedUnitsStack_) {
        MOZ_ASSERT(pendingCompressed_.empty());
        pendingCompressed_.construct<Compressed<Unit>>(std::move(raw), uncompressedLength);
    } else {
        data = SourceType(Compressed<Unit>(std::move(raw), uncompressedLength));
    }
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
const uint8_t* ScriptSource::binASTSource() {
  MOZ_ASSERT(hasBinASTSource());
  return reinterpret_cast<const uint8_t*>(data.as<BinAST>().string.chars());
}

#endif /* JS_BUILD_BINAST */
||||||| merged common ancestors
template<typename Unit>
MOZ_MUST_USE bool
ScriptSource::setCompressedSource(JSContext* cx, UniqueChars&& compressed, size_t rawLength,
                                  size_t sourceLength)
{
    MOZ_ASSERT(compressed);

    auto& cache = cx->zone()->runtimeFromAnyThread()->sharedImmutableStrings();
    auto deduped = cache.getOrCreate(std::move(compressed), rawLength);
    if (!deduped) {
        ReportOutOfMemory(cx);
        return false;
    }
=======
template <typename Unit>
bool ScriptSource::assignSource(JSContext* cx,
                                const ReadOnlyCompileOptions& options,
                                SourceText<Unit>& srcBuf) {
  MOZ_ASSERT(data.is<Missing>(),
             "source assignment should only occur on fresh ScriptSources");
>>>>>>> upstream-releases

<<<<<<< HEAD
bool ScriptSource::tryCompressOffThread(JSContext* cx) {
  if (!hasUncompressedSource()) {
    // This excludes already-compressed, missing, and BinAST source.
    return true;
  }

  // There are several cases where source compression is not a good idea:
  //  - If the script is tiny, then compression will save little or no space.
  //  - If there is only one core, then compression will contend with JS
  //    execution (which hurts benchmarketing).
  //
  // Otherwise, enqueue a compression task to be processed when a major
  // GC is requested.

  bool canCompressOffThread = HelperThreadState().cpuCount > 1 &&
                              HelperThreadState().threadCount >= 2 &&
                              CanUseExtraThreads();
  if (length() < ScriptSource::MinimumCompressibleLength ||
      !canCompressOffThread) {
    return true;
  }

  // The SourceCompressionTask needs to record the major GC number for
  // scheduling. If we're parsing off thread, this number is not safe to
  // access.
  //
  // When parsing on the main thread, the attempts made to compress off
  // thread in BytecodeCompiler will succeed.
  //
  // When parsing off-thread, the above attempts will fail and the attempt
  // made in ParseTask::finish will succeed.
  if (!CurrentThreadCanAccessRuntime(cx->runtime())) {
||||||| merged common ancestors
    setCompressedSource<Unit>(std::move(*deduped), sourceLength);
=======
  if (options.discardSource) {
>>>>>>> upstream-releases
    return true;
<<<<<<< HEAD
  }

  // Heap allocate the task. It will be freed upon compression
  // completing in AttachFinishedCompressedSources.
  auto task = MakeUnique<SourceCompressionTask>(cx->runtime(), this);
  if (!task) {
    ReportOutOfMemory(cx);
    return false;
  }
  return EnqueueOffThreadCompression(cx, std::move(task));
}

template <typename Unit>
void ScriptSource::setCompressedSource(SharedImmutableString raw,
                                       size_t uncompressedLength) {
  MOZ_ASSERT(data.is<Missing>() || hasUncompressedSource());
  MOZ_ASSERT_IF(hasUncompressedSource(), length() == uncompressedLength);

  if (pinnedUnitsStack_) {
    MOZ_ASSERT(pendingCompressed_.empty());
    pendingCompressed_.construct<Compressed<Unit>>(std::move(raw),
                                                   uncompressedLength);
  } else {
    data = SourceType(Compressed<Unit>(std::move(raw), uncompressedLength));
  }
}

template <typename Unit>
MOZ_MUST_USE bool ScriptSource::setCompressedSource(JSContext* cx,
                                                    UniqueChars&& compressed,
                                                    size_t rawLength,
                                                    size_t sourceLength) {
  MOZ_ASSERT(compressed);

  auto& cache = cx->zone()->runtimeFromAnyThread()->sharedImmutableStrings();
  auto deduped = cache.getOrCreate(std::move(compressed), rawLength);
  if (!deduped) {
    ReportOutOfMemory(cx);
    return false;
  }

  setCompressedSource<Unit>(std::move(*deduped), sourceLength);
  return true;
}

template <typename Unit>
bool ScriptSource::setSourceCopy(JSContext* cx, SourceText<Unit>& srcBuf) {
  MOZ_ASSERT(!hasSourceText());
||||||| merged common ancestors
}

bool
ScriptSource::setSourceCopy(JSContext* cx, SourceBufferHolder& srcBuf)
{
    MOZ_ASSERT(!hasSourceText());
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSRuntime* runtime = cx->zone()->runtimeFromAnyThread();
  auto& cache = runtime->sharedImmutableStrings();
  auto deduped = cache.getOrCreate(srcBuf.get(), srcBuf.length(), [&srcBuf]() {
    using CharT = typename SourceTypeTraits<Unit>::CharT;
    return srcBuf.ownsUnits()
               ? UniquePtr<CharT[], JS::FreePolicy>(srcBuf.takeChars())
||||||| merged common ancestors
    JSRuntime* runtime = cx->zone()->runtimeFromAnyThread();
    auto& cache = runtime->sharedImmutableStrings();
    auto deduped = cache.getOrCreate(srcBuf.get(), srcBuf.length(), [&srcBuf]() {
        return srcBuf.ownsChars()
               ? UniqueTwoByteChars(srcBuf.take())
=======
  if (options.sourceIsLazy) {
    data = SourceType(Retrievable<Unit>());
    return true;
  }

  JSRuntime* runtime = cx->zone()->runtimeFromAnyThread();
  auto& cache = runtime->sharedImmutableStrings();
  auto deduped = cache.getOrCreate(srcBuf.get(), srcBuf.length(), [&srcBuf]() {
    using CharT = typename SourceTypeTraits<Unit>::CharT;
    return srcBuf.ownsUnits()
               ? UniquePtr<CharT[], JS::FreePolicy>(srcBuf.takeChars())
>>>>>>> upstream-releases
               : DuplicateString(srcBuf.get(), srcBuf.length());
  });
  if (!deduped) {
    ReportOutOfMemory(cx);
    return false;
  }

<<<<<<< HEAD
  setSource<Unit>(std::move(*deduped));
  return true;
||||||| merged common ancestors
    setSource<char16_t>(std::move(*deduped));
    return true;
=======
  data = SourceType(
      Uncompressed<Unit, SourceRetrievable::No>(std::move(*deduped)));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template bool ScriptSource::setSourceCopy(JSContext* cx,
                                          SourceText<char16_t>& srcBuf);
template bool ScriptSource::setSourceCopy(JSContext* cx,
                                          SourceText<Utf8Unit>& srcBuf);

void ScriptSource::trace(JSTracer* trc) {
||||||| merged common ancestors
void
ScriptSource::trace(JSTracer* trc)
{
=======
template bool ScriptSource::assignSource(JSContext* cx,
                                         const ReadOnlyCompileOptions& options,
                                         SourceText<char16_t>& srcBuf);
template bool ScriptSource::assignSource(JSContext* cx,
                                         const ReadOnlyCompileOptions& options,
                                         SourceText<Utf8Unit>& srcBuf);

void ScriptSource::trace(JSTracer* trc) {
>>>>>>> upstream-releases
#ifdef JS_BUILD_BINAST
<<<<<<< HEAD
  if (binASTMetadata_) {
    binASTMetadata_->trace(trc);
  }
||||||| merged common ancestors
    if (binASTMetadata_) {
        binASTMetadata_->trace(trc);
    }
=======
  if (data.is<BinAST>()) {
    if (auto& metadata = data.as<BinAST>().metadata) {
      metadata->trace(trc);
    }
  }
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
  MOZ_ASSERT(!binASTMetadata_);
#endif  // JS_BUILD_BINAST
}

static MOZ_MUST_USE bool reallocUniquePtr(UniqueChars& unique, size_t size) {
  auto newPtr = static_cast<char*>(js_realloc(unique.get(), size));
  if (!newPtr) {
    return false;
  }

  // Since the realloc succeeded, unique is now holding a freed pointer.
  mozilla::Unused << unique.release();
  unique.reset(newPtr);
  return true;
}

template <typename Unit>
void SourceCompressionTask::workEncodingSpecific() {
  ScriptSource* source = sourceHolder_.get();
  MOZ_ASSERT(source->data.is<ScriptSource::Uncompressed<Unit>>());

  // Try to keep the maximum memory usage down by only allocating half the
  // size of the string, first.
  size_t inputBytes = source->length() * sizeof(Unit);
  size_t firstSize = inputBytes / 2;
  UniqueChars compressed(js_pod_malloc<char>(firstSize));
  if (!compressed) {
    return;
  }

  const Unit* chars =
      source->data.as<ScriptSource::Uncompressed<Unit>>().units();
  Compressor comp(reinterpret_cast<const unsigned char*>(chars), inputBytes);
  if (!comp.init()) {
    return;
  }

  comp.setOutput(reinterpret_cast<unsigned char*>(compressed.get()), firstSize);
  bool cont = true;
  bool reallocated = false;
  while (cont) {
    if (shouldCancel()) {
      return;
||||||| merged common ancestors
    MOZ_ASSERT(!binASTMetadata_);
#endif // JS_BUILD_BINAST
}

static MOZ_MUST_USE bool
reallocUniquePtr(UniqueChars& unique, size_t size)
{
    auto newPtr = static_cast<char*>(js_realloc(unique.get(), size));
    if (!newPtr) {
        return false;
    }

    // Since the realloc succeeded, unique is now holding a freed pointer.
    mozilla::Unused << unique.release();
    unique.reset(newPtr);
    return true;
}

template<typename Unit>
void
SourceCompressionTask::workEncodingSpecific()
{
    ScriptSource* source = sourceHolder_.get();
    MOZ_ASSERT(source->data.is<ScriptSource::Uncompressed<Unit>>());

    // Try to keep the maximum memory usage down by only allocating half the
    // size of the string, first.
    size_t inputBytes = source->length() * sizeof(Unit);
    size_t firstSize = inputBytes / 2;
    UniqueChars compressed(js_pod_malloc<char>(firstSize));
    if (!compressed) {
        return;
    }

    const Unit* chars = source->data.as<ScriptSource::Uncompressed<Unit>>().units();
    Compressor comp(reinterpret_cast<const unsigned char*>(chars), inputBytes);
    if (!comp.init()) {
        return;
=======
  MOZ_ASSERT(!data.is<BinAST>());
#endif  // JS_BUILD_BINAST
}

static MOZ_MUST_USE bool reallocUniquePtr(UniqueChars& unique, size_t size) {
  auto newPtr = static_cast<char*>(js_realloc(unique.get(), size));
  if (!newPtr) {
    return false;
  }

  // Since the realloc succeeded, unique is now holding a freed pointer.
  mozilla::Unused << unique.release();
  unique.reset(newPtr);
  return true;
}

template <typename Unit>
void SourceCompressionTask::workEncodingSpecific() {
  ScriptSource* source = sourceHolder_.get();
  MOZ_ASSERT(source->isUncompressed<Unit>());

  // Try to keep the maximum memory usage down by only allocating half the
  // size of the string, first.
  size_t inputBytes = source->length() * sizeof(Unit);
  size_t firstSize = inputBytes / 2;
  UniqueChars compressed(js_pod_malloc<char>(firstSize));
  if (!compressed) {
    return;
  }

  const Unit* chars = source->uncompressedData<Unit>()->units();
  Compressor comp(reinterpret_cast<const unsigned char*>(chars), inputBytes);
  if (!comp.init()) {
    return;
  }

  comp.setOutput(reinterpret_cast<unsigned char*>(compressed.get()), firstSize);
  bool cont = true;
  bool reallocated = false;
  while (cont) {
    if (shouldCancel()) {
      return;
>>>>>>> upstream-releases
    }

    switch (comp.compressMore()) {
      case Compressor::CONTINUE:
        break;
      case Compressor::MOREOUTPUT: {
        if (reallocated) {
          // The compressed string is longer than the original string.
          return;
        }

        // The compressed output is greater than half the size of the
        // original string. Reallocate to the full size.
        if (!reallocUniquePtr(compressed, inputBytes)) {
          return;
        }

        comp.setOutput(reinterpret_cast<unsigned char*>(compressed.get()),
                       inputBytes);
        reallocated = true;
        break;
      }
      case Compressor::DONE:
        cont = false;
        break;
      case Compressor::OOM:
        return;
    }
  }

  size_t totalBytes = comp.totalBytesNeeded();

  // Shrink the buffer to the size of the compressed data.
  if (!reallocUniquePtr(compressed, totalBytes)) {
    return;
  }

  comp.finish(compressed.get(), totalBytes);

  if (shouldCancel()) {
    return;
  }

  auto& strings = runtime_->sharedImmutableStrings();
  resultString_ = strings.getOrCreate(std::move(compressed), totalBytes);
}

struct SourceCompressionTask::PerformTaskWork {
  SourceCompressionTask* const task_;

  explicit PerformTaskWork(SourceCompressionTask* task) : task_(task) {}

<<<<<<< HEAD
  template <typename Unit>
  void match(const ScriptSource::Uncompressed<Unit>&) {
    task_->workEncodingSpecific<Unit>();
  }
||||||| merged common ancestors
    template<typename Unit>
    void match(const ScriptSource::Uncompressed<Unit>&) {
        task_->workEncodingSpecific<Unit>();
    }
=======
  template <typename Unit, SourceRetrievable CanRetrieve>
  void operator()(const ScriptSource::Uncompressed<Unit, CanRetrieve>&) {
    task_->workEncodingSpecific<Unit>();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  template <typename T>
  void match(const T&) {
    MOZ_CRASH(
        "why are we compressing missing, already-compressed, or "
        "BinAST source?");
  }
||||||| merged common ancestors
    template<typename T>
    void match (const T&) {
        MOZ_CRASH("why are we compressing missing, already-compressed, or "
                  "BinAST source?");
    }
=======
  template <typename T>
  void operator()(const T&) {
    MOZ_CRASH(
        "why are we compressing missing, missing-but-retrievable, "
        "already-compressed, or BinAST source?");
  }
>>>>>>> upstream-releases
};

void ScriptSource::performTaskWork(SourceCompressionTask* task) {
  MOZ_ASSERT(hasUncompressedSource());
  data.match(SourceCompressionTask::PerformTaskWork(task));
}

<<<<<<< HEAD
void SourceCompressionTask::work() {
  if (shouldCancel()) {
    return;
  }
||||||| merged common ancestors
void
SourceCompressionTask::work()
{
    if (shouldCancel()) {
        return;
    }
=======
void SourceCompressionTask::runTask() {
  if (shouldCancel()) {
    return;
  }
>>>>>>> upstream-releases

  ScriptSource* source = sourceHolder_.get();
  MOZ_ASSERT(source->hasUncompressedSource());

  source->performTaskWork(this);
}

<<<<<<< HEAD
void ScriptSource::setCompressedSourceFromTask(
    SharedImmutableString compressed) {
  data.match(SetCompressedSourceFromTask(this, compressed));
||||||| merged common ancestors
void
ScriptSource::setCompressedSourceFromTask(SharedImmutableString compressed)
{
    data.match(SetCompressedSourceFromTask(this, compressed));
=======
void ScriptSource::triggerConvertToCompressedSourceFromTask(
    SharedImmutableString compressed) {
  data.match(TriggerConvertToCompressedSourceFromTask(this, compressed));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void SourceCompressionTask::complete() {
  if (!shouldCancel() && resultString_.isSome()) {
    ScriptSource* source = sourceHolder_.get();
    source->setCompressedSourceFromTask(std::move(*resultString_));
  }
}
||||||| merged common ancestors
void
SourceCompressionTask::complete()
{
    if (!shouldCancel() && resultString_.isSome()) {
        ScriptSource* source = sourceHolder_.get();
        source->setCompressedSourceFromTask(std::move(*resultString_));
    }
}
=======
void SourceCompressionTask::complete() {
  if (!shouldCancel() && resultString_.isSome()) {
    ScriptSource* source = sourceHolder_.get();
    source->triggerConvertToCompressedSourceFromTask(std::move(*resultString_));
  }
}

bool js::SynchronouslyCompressSource(JSContext* cx,
                                     JS::Handle<JSScript*> script) {
  MOZ_ASSERT(!cx->isHelperThreadContext(),
             "should only sync-compress on the main thread");

  // Finish all pending source compressions, including the single compression
  // task that may have been created (by |ScriptSource::tryCompressOffThread|)
  // just after the script was compiled.  Because we have flushed this queue,
  // no code below needs to synchronize with an off-thread parse task that
  // assumes the immutability of a |ScriptSource|'s data.
  //
  // This *may* end up compressing |script|'s source.  If it does -- we test
  // this below -- that takes care of things.  But if it doesn't, we will
  // synchronously compress ourselves (and as noted above, this won't race
  // anything).
  RunPendingSourceCompressions(cx->runtime());

  ScriptSource* ss = script->scriptSource();
  MOZ_ASSERT(!ss->pinnedUnitsStack_,
             "can't synchronously compress while source units are in use");

  // In principle a previously-triggered compression on a helper thread could
  // have already completed.  If that happens, there's nothing more to do.
  if (ss->hasCompressedSource()) {
    return true;
  }

  MOZ_ASSERT(ss->hasUncompressedSource(),
             "shouldn't be compressing uncompressible source");

  // Use an explicit scope to delineate the lifetime of |task|, for simplicity.
  {
#ifdef DEBUG
    uint32_t sourceRefs = ss->refs;
#endif
    MOZ_ASSERT(sourceRefs > 0, "at least |script| here should have a ref");

    // |SourceCompressionTask::shouldCancel| can periodically result in source
    // compression being canceled if we're not careful.  Guarantee that two refs
    // to |ss| are always live in this function (at least one preexisting and
    // one held by the task) so that compression is never canceled.
    auto task = MakeUnique<SourceCompressionTask>(cx->runtime(), ss);
    if (!task) {
      ReportOutOfMemory(cx);
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
void ScriptSource::addSizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf,
                                          JS::ScriptSourceInfo* info) const {
  info->misc += mallocSizeOf(this) + mallocSizeOf(filename_.get()) +
                mallocSizeOf(introducerFilename_.get());
  info->numScripts++;
||||||| merged common ancestors
void
ScriptSource::addSizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf,
                                     JS::ScriptSourceInfo* info) const
{
    info->misc += mallocSizeOf(this) +
                  mallocSizeOf(filename_.get()) +
                  mallocSizeOf(introducerFilename_.get());
    info->numScripts++;
=======
    MOZ_ASSERT(ss->refs > sourceRefs, "must have at least two refs now");

    // Attempt to compress.  This may not succeed if OOM happens, but (because
    // it ordinarily happens on a helper thread) no error will ever be set here.
    MOZ_ASSERT(!cx->isExceptionPending());
    task->runTask();
    MOZ_ASSERT(!cx->isExceptionPending());

    // Convert |ss| from uncompressed to compressed data.
    task->complete();

    MOZ_ASSERT(!cx->isExceptionPending());
  }

  // The only way source won't be compressed here is if OOM happened.
  return ss->hasCompressedSource();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool ScriptSource::xdrEncodeTopLevel(JSContext* cx, HandleScript script) {
  // Encoding failures are reported by the xdrFinalizeEncoder function.
  if (containsAsmJS()) {
    return true;
  }
||||||| merged common ancestors
bool
ScriptSource::xdrEncodeTopLevel(JSContext* cx, HandleScript script)
{
    // Encoding failures are reported by the xdrFinalizeEncoder function.
    if (containsAsmJS()) {
        return true;
    }

    xdrEncoder_ = js::MakeUnique<XDRIncrementalEncoder>(cx);
    if (!xdrEncoder_) {
        ReportOutOfMemory(cx);
        return false;
    }
=======
void ScriptSource::addSizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf,
                                          JS::ScriptSourceInfo* info) const {
  info->misc += mallocSizeOf(this) + mallocSizeOf(filename_.get()) +
                mallocSizeOf(introducerFilename_.get());
  info->numScripts++;
}

bool ScriptSource::xdrEncodeTopLevel(JSContext* cx, HandleScript script) {
  // Encoding failures are reported by the xdrFinalizeEncoder function.
  if (containsAsmJS()) {
    return true;
  }
>>>>>>> upstream-releases

  xdrEncoder_ = js::MakeUnique<XDRIncrementalEncoder>(cx);
  if (!xdrEncoder_) {
    ReportOutOfMemory(cx);
    return false;
  }

  MOZ_ASSERT(hasEncoder());
  auto failureCase =
      mozilla::MakeScopeExit([&] { xdrEncoder_.reset(nullptr); });

  RootedScript s(cx, script);
  XDRResult res = xdrEncoder_->codeScript(&s);
  if (res.isErr()) {
    // On encoding failure, let failureCase destroy encoder and return true
    // to avoid failing any currently executing script.
    if (res.unwrapErr() & JS::TranscodeResult_Failure) {
      return true;
    }

    return false;
  }

  failureCase.release();
  return true;
}

bool ScriptSource::xdrEncodeFunction(JSContext* cx, HandleFunction fun,
                                     HandleScriptSourceObject sourceObject) {
  MOZ_ASSERT(sourceObject->source() == this);
  MOZ_ASSERT(hasEncoder());
  auto failureCase =
      mozilla::MakeScopeExit([&] { xdrEncoder_.reset(nullptr); });

  RootedFunction f(cx, fun);
  XDRResult res = xdrEncoder_->codeFunction(&f, sourceObject);
  if (res.isErr()) {
    // On encoding failure, let failureCase destroy encoder and return true
    // to avoid failing any currently executing script.
    if (res.unwrapErr() & JS::TranscodeResult_Failure) {
      return true;
    }
    return false;
  }

  failureCase.release();
  return true;
}

bool ScriptSource::xdrFinalizeEncoder(JS::TranscodeBuffer& buffer) {
  if (!hasEncoder()) {
    return false;
  }

<<<<<<< HEAD
  auto cleanup = mozilla::MakeScopeExit([&] { xdrEncoder_.reset(nullptr); });
||||||| merged common ancestors
    auto cleanup = mozilla::MakeScopeExit([&] {
        xdrEncoder_.reset(nullptr);
    });
=======
  auto cleanup = mozilla::MakeScopeExit([&] { xdrEncoder_.reset(nullptr); });

  XDRResult res = xdrEncoder_->linearize(buffer);
  return res.isOk();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  XDRResult res = xdrEncoder_->linearize(buffer);
  return res.isOk();
||||||| merged common ancestors
    XDRResult res = xdrEncoder_->linearize(buffer);
    return res.isOk();
=======
template <typename Unit>
MOZ_MUST_USE bool ScriptSource::initializeUnretrievableUncompressedSource(
    JSContext* cx, EntryUnits<Unit>&& source, size_t length) {
  MOZ_ASSERT(data.is<Missing>(), "must be initializing a fresh ScriptSource");
  return setUncompressedSourceHelper(cx, std::move(source), length,
                                     SourceRetrievable::No);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Unit>
struct SourceDecoder {
  XDRState<XDR_DECODE>* const xdr_;
  ScriptSource* const scriptSource_;
  const uint32_t uncompressedLength_;
||||||| merged common ancestors
template<typename Unit>
struct SourceDecoder
{
    XDRState<XDR_DECODE>* const xdr_;
    ScriptSource* const scriptSource_;
    const uint32_t uncompressedLength_;
=======
template <typename Unit>
struct UnretrievableSourceDecoder {
  XDRState<XDR_DECODE>* const xdr_;
  ScriptSource* const scriptSource_;
  const uint32_t uncompressedLength_;
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  SourceDecoder(XDRState<XDR_DECODE>* xdr, ScriptSource* scriptSource,
                uint32_t uncompressedLength)
||||||| merged common ancestors
  public:
    SourceDecoder(XDRState<XDR_DECODE>* xdr, ScriptSource* scriptSource,
                  uint32_t uncompressedLength)
=======
 public:
  UnretrievableSourceDecoder(XDRState<XDR_DECODE>* xdr,
                             ScriptSource* scriptSource,
                             uint32_t uncompressedLength)
>>>>>>> upstream-releases
      : xdr_(xdr),
        scriptSource_(scriptSource),
        uncompressedLength_(uncompressedLength) {}

  XDRResult decode() {
    auto sourceUnits =
        xdr_->cx()->make_pod_array<Unit>(Max<size_t>(uncompressedLength_, 1));
    if (!sourceUnits) {
      return xdr_->fail(JS::TranscodeResult_Throw);
    }

    MOZ_TRY(xdr_->codeChars(sourceUnits.get(), uncompressedLength_));

<<<<<<< HEAD
    if (!scriptSource_->setSource(xdr_->cx(), std::move(sourceUnits),
                                  uncompressedLength_)) {
      return xdr_->fail(JS::TranscodeResult_Throw);
||||||| merged common ancestors
        return Ok();
=======
    if (!scriptSource_->initializeUnretrievableUncompressedSource(
            xdr_->cx(), std::move(sourceUnits), uncompressedLength_)) {
      return xdr_->fail(JS::TranscodeResult_Throw);
>>>>>>> upstream-releases
    }

    return Ok();
  }
};

namespace js {

<<<<<<< HEAD
template <>
XDRResult ScriptSource::xdrUncompressedSource<XDR_DECODE>(
    XDRState<XDR_DECODE>* xdr, uint8_t sourceCharSize,
    uint32_t uncompressedLength) {
  MOZ_ASSERT(sourceCharSize == 1 || sourceCharSize == 2);
||||||| merged common ancestors
template<>
XDRResult
ScriptSource::xdrUncompressedSource<XDR_DECODE>(XDRState<XDR_DECODE>* xdr,
                                                uint8_t sourceCharSize,
                                                uint32_t uncompressedLength)
{
    MOZ_ASSERT(sourceCharSize == 1 || sourceCharSize == 2);

    if (sourceCharSize == 1) {
        SourceDecoder<Utf8Unit> decoder(xdr, this, uncompressedLength);
        return decoder.decode();
    }
=======
template <>
XDRResult ScriptSource::xdrUnretrievableUncompressedSource<XDR_DECODE>(
    XDRState<XDR_DECODE>* xdr, uint8_t sourceCharSize,
    uint32_t uncompressedLength) {
  MOZ_ASSERT(sourceCharSize == 1 || sourceCharSize == 2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (sourceCharSize == 1) {
    SourceDecoder<Utf8Unit> decoder(xdr, this, uncompressedLength);
||||||| merged common ancestors
    SourceDecoder<char16_t> decoder(xdr, this, uncompressedLength);
=======
  if (sourceCharSize == 1) {
    UnretrievableSourceDecoder<Utf8Unit> decoder(xdr, this, uncompressedLength);
>>>>>>> upstream-releases
    return decoder.decode();
<<<<<<< HEAD
  }

  SourceDecoder<char16_t> decoder(xdr, this, uncompressedLength);
  return decoder.decode();
||||||| merged common ancestors
=======
  }

  UnretrievableSourceDecoder<char16_t> decoder(xdr, this, uncompressedLength);
  return decoder.decode();
>>>>>>> upstream-releases
}

}  // namespace js

<<<<<<< HEAD
template <typename Unit>
struct SourceEncoder {
  XDRState<XDR_ENCODE>* const xdr_;
  ScriptSource* const source_;
  const uint32_t uncompressedLength_;
||||||| merged common ancestors
template<typename Unit>
struct SourceEncoder
{
    XDRState<XDR_ENCODE>* const xdr_;
    ScriptSource* const source_;
    const uint32_t uncompressedLength_;
=======
template <typename Unit>
struct UnretrievableSourceEncoder {
  XDRState<XDR_ENCODE>* const xdr_;
  ScriptSource* const source_;
  const uint32_t uncompressedLength_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  SourceEncoder(XDRState<XDR_ENCODE>* xdr, ScriptSource* source,
                uint32_t uncompressedLength)
      : xdr_(xdr), source_(source), uncompressedLength_(uncompressedLength) {}
||||||| merged common ancestors
    SourceEncoder(XDRState<XDR_ENCODE>* xdr, ScriptSource* source, uint32_t uncompressedLength)
      : xdr_(xdr),
        source_(source),
        uncompressedLength_(uncompressedLength)
    {}
=======
  UnretrievableSourceEncoder(XDRState<XDR_ENCODE>* xdr, ScriptSource* source,
                             uint32_t uncompressedLength)
      : xdr_(xdr), source_(source), uncompressedLength_(uncompressedLength) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  XDRResult encode() {
    Unit* sourceUnits = const_cast<Unit*>(source_->uncompressedData<Unit>());
||||||| merged common ancestors
    XDRResult encode() {
        Unit* sourceUnits = const_cast<Unit*>(source_->uncompressedData<Unit>());
=======
  XDRResult encode() {
    Unit* sourceUnits =
        const_cast<Unit*>(source_->uncompressedData<Unit>()->units());
>>>>>>> upstream-releases

    return xdr_->codeChars(sourceUnits, uncompressedLength_);
  }
};

namespace js {

<<<<<<< HEAD
template <>
XDRResult ScriptSource::xdrUncompressedSource<XDR_ENCODE>(
    XDRState<XDR_ENCODE>* xdr, uint8_t sourceCharSize,
    uint32_t uncompressedLength) {
  MOZ_ASSERT(sourceCharSize == 1 || sourceCharSize == 2);
||||||| merged common ancestors
template<>
XDRResult
ScriptSource::xdrUncompressedSource<XDR_ENCODE>(XDRState<XDR_ENCODE>* xdr,
                                                uint8_t sourceCharSize,
                                                uint32_t uncompressedLength)
{
    MOZ_ASSERT(sourceCharSize == 1 || sourceCharSize == 2);

    if (sourceCharSize == 1) {
        SourceEncoder<Utf8Unit> encoder(xdr, this, uncompressedLength);
        return encoder.encode();
    }
=======
template <>
XDRResult ScriptSource::xdrUnretrievableUncompressedSource<XDR_ENCODE>(
    XDRState<XDR_ENCODE>* xdr, uint8_t sourceCharSize,
    uint32_t uncompressedLength) {
  MOZ_ASSERT(sourceCharSize == 1 || sourceCharSize == 2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (sourceCharSize == 1) {
    SourceEncoder<Utf8Unit> encoder(xdr, this, uncompressedLength);
||||||| merged common ancestors
    SourceEncoder<char16_t> encoder(xdr, this, uncompressedLength);
=======
  if (sourceCharSize == 1) {
    UnretrievableSourceEncoder<Utf8Unit> encoder(xdr, this, uncompressedLength);
>>>>>>> upstream-releases
    return encoder.encode();
<<<<<<< HEAD
  }
||||||| merged common ancestors
}
=======
  }

  UnretrievableSourceEncoder<char16_t> encoder(xdr, this, uncompressedLength);
  return encoder.encode();
}

}  // namespace js

template <typename Unit, XDRMode mode>
/* static */
XDRResult ScriptSource::codeUncompressedData(XDRState<mode>* const xdr,
                                             ScriptSource* const ss) {
  static_assert(std::is_same<Unit, Utf8Unit>::value ||
                    std::is_same<Unit, char16_t>::value,
                "should handle UTF-8 and UTF-16");

  if (mode == XDR_ENCODE) {
    MOZ_ASSERT(ss->isUncompressed<Unit>());
  } else {
    MOZ_ASSERT(ss->data.is<Missing>());
  }

  uint32_t uncompressedLength;
  if (mode == XDR_ENCODE) {
    uncompressedLength = ss->uncompressedData<Unit>()->length();
  }
  MOZ_TRY(xdr->codeUint32(&uncompressedLength));

  return ss->xdrUnretrievableUncompressedSource(xdr, sizeof(Unit),
                                                uncompressedLength);
}

template <typename Unit, XDRMode mode>
/* static */
XDRResult ScriptSource::codeCompressedData(XDRState<mode>* const xdr,
                                           ScriptSource* const ss) {
  static_assert(std::is_same<Unit, Utf8Unit>::value ||
                    std::is_same<Unit, char16_t>::value,
                "should handle UTF-8 and UTF-16");

  if (mode == XDR_ENCODE) {
    MOZ_ASSERT(ss->isCompressed<Unit>());
  } else {
    MOZ_ASSERT(ss->data.is<Missing>());
  }

  uint32_t uncompressedLength;
  if (mode == XDR_ENCODE) {
    uncompressedLength = ss->data.as<Compressed<Unit, SourceRetrievable::No>>()
                             .uncompressedLength;
  }
  MOZ_TRY(xdr->codeUint32(&uncompressedLength));

  uint32_t compressedLength;
  if (mode == XDR_ENCODE) {
    compressedLength =
        ss->data.as<Compressed<Unit, SourceRetrievable::No>>().raw.length();
  }
  MOZ_TRY(xdr->codeUint32(&compressedLength));

  if (mode == XDR_DECODE) {
    // Compressed data is always single-byte chars.
    auto bytes = xdr->cx()->template make_pod_array<char>(compressedLength);
    if (!bytes) {
      return xdr->fail(JS::TranscodeResult_Throw);
    }
    MOZ_TRY(xdr->codeBytes(bytes.get(), compressedLength));

    if (!ss->initializeWithUnretrievableCompressedSource<Unit>(
            xdr->cx(), std::move(bytes), compressedLength,
            uncompressedLength)) {
      return xdr->fail(JS::TranscodeResult_Throw);
    }
  } else {
    void* bytes = const_cast<char*>(ss->compressedData<Unit>()->raw.chars());
    MOZ_TRY(xdr->codeBytes(bytes, compressedLength));
  }

  return Ok();
}

template <typename Unit,
          template <typename U, SourceRetrievable CanRetrieve> class Data,
          XDRMode mode>
/* static */
void ScriptSource::codeRetrievable(ScriptSource* const ss) {
  static_assert(std::is_same<Unit, Utf8Unit>::value ||
                    std::is_same<Unit, char16_t>::value,
                "should handle UTF-8 and UTF-16");

  if (mode == XDR_ENCODE) {
    MOZ_ASSERT((ss->data.is<Data<Unit, SourceRetrievable::Yes>>()));
  } else {
    MOZ_ASSERT(ss->data.is<Missing>());
    ss->data = SourceType(Retrievable<Unit>());
  }
}

template <XDRMode mode>
/* static */
XDRResult ScriptSource::codeBinASTData(XDRState<mode>* const xdr,
                                       ScriptSource* const ss) {
#if !defined(JS_BUILD_BINAST)
  return xdr->fail(JS::TranscodeResult_Throw);
#else
  if (mode == XDR_ENCODE) {
    MOZ_ASSERT(ss->data.is<BinAST>());
  } else {
    MOZ_ASSERT(ss->data.is<Missing>());
  }

  // XDR the length of the BinAST data.
  uint32_t binASTLength;
  if (mode == XDR_ENCODE) {
    binASTLength = ss->data.as<BinAST>().string.length();
  }
  MOZ_TRY(xdr->codeUint32(&binASTLength));

  // XDR the BinAST data.
  Maybe<SharedImmutableString> binASTData;
  if (mode == XDR_DECODE) {
    auto bytes =
        xdr->cx()->template make_pod_array<char>(Max<size_t>(binASTLength, 1));
    if (!bytes) {
      return xdr->fail(JS::TranscodeResult_Throw);
    }
    MOZ_TRY(xdr->codeBytes(bytes.get(), binASTLength));

    auto& cache =
        xdr->cx()->zone()->runtimeFromAnyThread()->sharedImmutableStrings();
    binASTData = cache.getOrCreate(std::move(bytes), binASTLength);
    if (!binASTData) {
      ReportOutOfMemory(xdr->cx());
      return xdr->fail(JS::TranscodeResult_Throw);
    }
  } else {
    void* bytes = ss->binASTData();
    MOZ_TRY(xdr->codeBytes(bytes, binASTLength));
  }

  // XDR any BinAST metadata.
  uint8_t hasMetadata;
  if (mode == XDR_ENCODE) {
    hasMetadata = ss->data.as<BinAST>().metadata != nullptr;
  }
  MOZ_TRY(xdr->codeUint8(&hasMetadata));

  Rooted<UniquePtr<frontend::BinASTSourceMetadata>> freshMetadata(xdr->cx());
  if (hasMetadata) {
    // If we're decoding, this is a *mutable borrowed* reference to the
    // |UniquePtr| stored in the |Rooted| above, and the |UniquePtr| will be
    // filled with freshly allocated metadata.
    //
    // If we're encoding, this is an *immutable borrowed* reference to the
    // |UniquePtr| stored in |ss|.  (Immutable up to GCs transparently moving
    // things around, that is.)
    UniquePtr<frontend::BinASTSourceMetadata>& binASTMetadata =
        mode == XDR_DECODE ? freshMetadata.get()
                           : ss->data.as<BinAST>().metadata;

    uint32_t numBinASTKinds;
    uint32_t numStrings;
    if (mode == XDR_ENCODE) {
      numBinASTKinds = binASTMetadata->numBinASTKinds();
      numStrings = binASTMetadata->numStrings();
    }
    MOZ_TRY(xdr->codeUint32(&numBinASTKinds));
    MOZ_TRY(xdr->codeUint32(&numStrings));
>>>>>>> upstream-releases

<<<<<<< HEAD
  SourceEncoder<char16_t> encoder(xdr, this, uncompressedLength);
  return encoder.encode();
}
||||||| merged common ancestors
} // namespace js
=======
    if (mode == XDR_DECODE) {
      // Use calloc, since we're storing this immediately, and filling it
      // might GC, to avoid marking bogus atoms.
      void* mem = js_calloc(frontend::BinASTSourceMetadata::totalSize(
          numBinASTKinds, numStrings));
      if (!mem) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
}  // namespace js
||||||| merged common ancestors
template<XDRMode mode>
XDRResult
ScriptSource::performXDR(XDRState<mode>* xdr)
{
    uint8_t hasSource = hasSourceText();
    MOZ_TRY(xdr->codeUint8(&hasSource));
=======
      auto metadata =
          new (mem) frontend::BinASTSourceMetadata(numBinASTKinds, numStrings);
      binASTMetadata.reset(metadata);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <XDRMode mode>
XDRResult ScriptSource::performXDR(XDRState<mode>* xdr) {
  uint8_t hasSource = hasSourceText();
  MOZ_TRY(xdr->codeUint8(&hasSource));
||||||| merged common ancestors
    uint8_t hasBinSource = hasBinASTSource();
    MOZ_TRY(xdr->codeUint8(&hasBinSource));
=======
    MOZ_ASSERT(binASTMetadata != nullptr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint8_t hasBinSource = hasBinASTSource();
  MOZ_TRY(xdr->codeUint8(&hasBinSource));
||||||| merged common ancestors
    uint8_t retrievable = sourceRetrievable_;
    MOZ_TRY(xdr->codeUint8(&retrievable));
    sourceRetrievable_ = retrievable;
=======
    frontend::BinASTKind* binASTKindBase = binASTMetadata->binASTKindBase();
    for (uint32_t i = 0; i < numBinASTKinds; i++) {
      MOZ_TRY(xdr->codeEnum32(&binASTKindBase[i]));
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint8_t retrievable = sourceRetrievable_;
  MOZ_TRY(xdr->codeUint8(&retrievable));
  sourceRetrievable_ = retrievable;
||||||| merged common ancestors
    if ((hasSource || hasBinSource) && !sourceRetrievable_) {
        uint32_t uncompressedLength = 0;
        if (mode == XDR_ENCODE) {
            uncompressedLength = length();
        }
        MOZ_TRY(xdr->codeUint32(&uncompressedLength));
=======
    RootedAtom atom(xdr->cx());
    JSAtom** atomsBase = binASTMetadata->atomsBase();
    auto slices = binASTMetadata->sliceBase();
    const char* sourceBase =
        (mode == XDR_ENCODE ? ss->data.as<BinAST>().string : *binASTData)
            .chars();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if ((hasSource || hasBinSource) && !sourceRetrievable_) {
    uint32_t uncompressedLength = 0;
    if (mode == XDR_ENCODE) {
      uncompressedLength = length();
    }
    MOZ_TRY(xdr->codeUint32(&uncompressedLength));
||||||| merged common ancestors
        // A compressed length of 0 indicates source is uncompressed (or is
        // BinAST if |hasBinSource|).
        uint32_t compressedLength;
        if (mode == XDR_ENCODE) {
            compressedLength = compressedLengthOrZero();
        }
        MOZ_TRY(xdr->codeUint32(&compressedLength));
=======
    for (uint32_t i = 0; i < numStrings; i++) {
      uint8_t isNull;
      if (mode == XDR_ENCODE) {
        atom = binASTMetadata->getAtom(i);
        isNull = !atom;
      }
      MOZ_TRY(xdr->codeUint8(&isNull));
      if (isNull) {
        atom = nullptr;
      } else {
        MOZ_TRY(XDRAtom(xdr, &atom));
      }
      if (mode == XDR_DECODE) {
        atomsBase[i] = atom;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (hasBinSource) {
      if (mode == XDR_DECODE) {
#if defined(JS_BUILD_BINAST)
        auto bytes = xdr->cx()->template make_pod_array<char>(
            Max<size_t>(uncompressedLength, 1));
        if (!bytes) {
          return xdr->fail(JS::TranscodeResult_Throw);
        }
        MOZ_TRY(xdr->codeBytes(bytes.get(), uncompressedLength));
||||||| merged common ancestors
        uint8_t srcCharSize;
        if (mode == XDR_ENCODE) {
            srcCharSize = sourceCharSize();
        }
        MOZ_TRY(xdr->codeUint8(&srcCharSize));
=======
      uint64_t sliceOffset;
      uint32_t sliceLen;
      if (mode == XDR_ENCODE) {
        auto& slice = binASTMetadata->getSlice(i);
        sliceOffset = slice.begin() - sourceBase;
        sliceLen = slice.byteLen_;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (!setBinASTSource(xdr->cx(), std::move(bytes), uncompressedLength)) {
          return xdr->fail(JS::TranscodeResult_Throw);
        }
#else
        MOZ_ASSERT(mode != XDR_ENCODE);
        return xdr->fail(JS::TranscodeResult_Throw);
#endif /* JS_BUILD_BINAST */
      } else {
        void* bytes = binASTData();
        MOZ_TRY(xdr->codeBytes(bytes, uncompressedLength));
      }
    } else {
      // A compressed length of 0 indicates source is uncompressed
      uint32_t compressedLength;
      if (mode == XDR_ENCODE) {
        compressedLength = compressedLengthOrZero();
      }
      MOZ_TRY(xdr->codeUint32(&compressedLength));
||||||| merged common ancestors
        if (srcCharSize != 1 && srcCharSize != 2) {
            // Fail in debug, but only soft-fail in release, if the source-char
            // size is invalid.
            MOZ_ASSERT_UNREACHABLE("bad XDR source chars size");
            return xdr->fail(JS::TranscodeResult_Failure_BadDecode);
        }

        if (hasBinSource) {
            if (mode == XDR_DECODE) {
#if defined(JS_BUILD_BINAST)
                auto bytes =
                    xdr->cx()->template make_pod_array<char>(Max<size_t>(uncompressedLength, 1));
                if (!bytes) {
                    return xdr->fail(JS::TranscodeResult_Throw);
                }
                MOZ_TRY(xdr->codeBytes(bytes.get(), uncompressedLength));

                if (!setBinASTSource(xdr->cx(), std::move(bytes), uncompressedLength)) {
                    return xdr->fail(JS::TranscodeResult_Throw);
                }
#else
                MOZ_ASSERT(mode != XDR_ENCODE);
                return xdr->fail(JS::TranscodeResult_Throw);
#endif /* JS_BUILD_BINAST */
            } else {
                void* bytes = binASTData();
                MOZ_TRY(xdr->codeBytes(bytes, uncompressedLength));
            }
        } else if (compressedLength) {
            if (mode == XDR_DECODE) {
                // Compressed data is always single-byte chars.
                auto bytes = xdr->cx()->template make_pod_array<char>(compressedLength);
                if (!bytes) {
                    return xdr->fail(JS::TranscodeResult_Throw);
                }
                MOZ_TRY(xdr->codeBytes(bytes.get(), compressedLength));

                if (!(srcCharSize == 1
                      ? setCompressedSource<Utf8Unit>(xdr->cx(), std::move(bytes),
                                                      compressedLength, uncompressedLength)
                      : setCompressedSource<char16_t>(xdr->cx(), std::move(bytes),
                                                      compressedLength, uncompressedLength)))
                {
                    return xdr->fail(JS::TranscodeResult_Throw);
                }
            } else {
                void* bytes = srcCharSize == 1
                              ? compressedData<Utf8Unit>()
                              : compressedData<char16_t>();
                MOZ_TRY(xdr->codeBytes(bytes, compressedLength));
            }
        } else {
            MOZ_TRY(xdrUncompressedSource(xdr, srcCharSize, uncompressedLength));
        }
=======
      MOZ_TRY(xdr->codeUint64(&sliceOffset));
      MOZ_TRY(xdr->codeUint32(&sliceLen));

      if (mode == XDR_DECODE) {
        new (&slices[i]) frontend::BinASTSourceMetadata::CharSlice(
            sourceBase + sliceOffset, sliceLen);
      }
    }
  }

  if (mode == XDR_DECODE) {
    MOZ_ASSERT(binASTData.isSome());
    MOZ_ASSERT(freshMetadata != nullptr);

    MOZ_ASSERT(ss->data.is<Missing>(),
               "should only be initializing a fresh ScriptSource");

    ss->data = SourceType(
        BinAST(std::move(*binASTData), std::move(freshMetadata.get())));
  }

  MOZ_ASSERT(binASTData.isNothing());
  MOZ_ASSERT(freshMetadata == nullptr);
  MOZ_ASSERT(ss->data.is<BinAST>());

  return Ok();
#endif  // !defined(JS_BUILD_BINAST)
}

template <typename Unit, XDRMode mode>
/* static */
void ScriptSource::codeRetrievableData(ScriptSource* ss) {
  // There's nothing to code for retrievable data.  Just be sure to set
  // retrievable data when decoding.
  if (mode == XDR_ENCODE) {
    MOZ_ASSERT(ss->data.is<Retrievable<Unit>>());
  } else {
    MOZ_ASSERT(ss->data.is<Missing>());
    ss->data = SourceType(Retrievable<Unit>());
  }
}

template <XDRMode mode>
/* static */
XDRResult ScriptSource::xdrData(XDRState<mode>* const xdr,
                                ScriptSource* const ss) {
  // The order here corresponds to the type order in |ScriptSource::SourceType|
  // so number->internal Variant tag is a no-op.
  enum class DataType {
    CompressedUtf8Retrievable,
    UncompressedUtf8Retrievable,
    CompressedUtf8NotRetrievable,
    UncompressedUtf8NotRetrievable,
    CompressedUtf16Retrievable,
    UncompressedUtf16Retrievable,
    CompressedUtf16NotRetrievable,
    UncompressedUtf16NotRetrievable,
    RetrievableUtf8,
    RetrievableUtf16,
    Missing,
    BinAST,
  };

  DataType tag;
  {
    // This is terrible, but we can't do better.  When |mode == XDR_DECODE| we
    // don't have a |ScriptSource::data| |Variant| to match -- the entire XDR
    // idiom for tagged unions depends on coding a tag-number, then the
    // corresponding tagged data.  So we must manually define a tag-enum, code
    // it, then switch on it (and ignore the |Variant::match| API).
    class XDRDataTag {
     public:
      DataType operator()(const Compressed<Utf8Unit, SourceRetrievable::Yes>&) {
        return DataType::CompressedUtf8Retrievable;
      }
      DataType operator()(
          const Uncompressed<Utf8Unit, SourceRetrievable::Yes>&) {
        return DataType::UncompressedUtf8Retrievable;
      }
      DataType operator()(const Compressed<Utf8Unit, SourceRetrievable::No>&) {
        return DataType::CompressedUtf8NotRetrievable;
      }
      DataType operator()(
          const Uncompressed<Utf8Unit, SourceRetrievable::No>&) {
        return DataType::UncompressedUtf8NotRetrievable;
      }
      DataType operator()(const Compressed<char16_t, SourceRetrievable::Yes>&) {
        return DataType::CompressedUtf16Retrievable;
      }
      DataType operator()(
          const Uncompressed<char16_t, SourceRetrievable::Yes>&) {
        return DataType::UncompressedUtf16Retrievable;
      }
      DataType operator()(const Compressed<char16_t, SourceRetrievable::No>&) {
        return DataType::CompressedUtf16NotRetrievable;
      }
      DataType operator()(
          const Uncompressed<char16_t, SourceRetrievable::No>&) {
        return DataType::UncompressedUtf16NotRetrievable;
      }
      DataType operator()(const Retrievable<Utf8Unit>&) {
        return DataType::RetrievableUtf8;
      }
      DataType operator()(const Retrievable<char16_t>&) {
        return DataType::RetrievableUtf16;
      }
      DataType operator()(const Missing&) { return DataType::Missing; }
      DataType operator()(const BinAST&) { return DataType::BinAST; }
    };
>>>>>>> upstream-releases

<<<<<<< HEAD
      uint8_t srcCharSize;
      if (mode == XDR_ENCODE) {
        srcCharSize = sourceCharSize();
      }
      MOZ_TRY(xdr->codeUint8(&srcCharSize));

      if (srcCharSize != 1 && srcCharSize != 2) {
        // Fail in debug, but only soft-fail in release, if the source-char
        // size is invalid.
        MOZ_ASSERT_UNREACHABLE("bad XDR source chars size");
        return xdr->fail(JS::TranscodeResult_Failure_BadDecode);
      }

      if (compressedLength) {
        if (mode == XDR_DECODE) {
          // Compressed data is always single-byte chars.
          auto bytes =
              xdr->cx()->template make_pod_array<char>(compressedLength);
          if (!bytes) {
            return xdr->fail(JS::TranscodeResult_Throw);
          }
          MOZ_TRY(xdr->codeBytes(bytes.get(), compressedLength));

          if (!(srcCharSize == 1 ? setCompressedSource<Utf8Unit>(
                                       xdr->cx(), std::move(bytes),
                                       compressedLength, uncompressedLength)
                                 : setCompressedSource<char16_t>(
                                       xdr->cx(), std::move(bytes),
                                       compressedLength, uncompressedLength))) {
            return xdr->fail(JS::TranscodeResult_Throw);
          }
        } else {
          void* bytes = srcCharSize == 1 ? compressedData<Utf8Unit>()
                                         : compressedData<char16_t>();
          MOZ_TRY(xdr->codeBytes(bytes, compressedLength));
        }
      } else {
        MOZ_TRY(xdrUncompressedSource(xdr, srcCharSize, uncompressedLength));
      }
||||||| merged common ancestors
        uint8_t hasMetadata = !!binASTMetadata_;
        MOZ_TRY(xdr->codeUint8(&hasMetadata));
        if (hasMetadata) {
#if defined(JS_BUILD_BINAST)
            uint32_t numBinKinds;
            uint32_t numStrings;
            if (mode == XDR_ENCODE) {
                numBinKinds = binASTMetadata_->numBinKinds();
                numStrings = binASTMetadata_->numStrings();
            }
            MOZ_TRY(xdr->codeUint32(&numBinKinds));
            MOZ_TRY(xdr->codeUint32(&numStrings));

            if (mode == XDR_DECODE) {
                // Use calloc, since we're storing this immediately, and filling it might GC, to
                // avoid marking bogus atoms.
                setBinASTSourceMetadata(
                    static_cast<frontend::BinASTSourceMetadata*>(
                        js_calloc(frontend::BinASTSourceMetadata::totalSize(numBinKinds, numStrings))));
                if (!binASTMetadata_) {
                    return xdr->fail(JS::TranscodeResult_Throw);
                }
            }

            for (uint32_t i = 0; i < numBinKinds; i++) {
                frontend::BinKind* binKindBase = binASTMetadata_->binKindBase();
                MOZ_TRY(xdr->codeEnum32(&binKindBase[i]));
            }

            RootedAtom atom(xdr->cx());
            JSAtom** atomsBase = binASTMetadata_->atomsBase();
            auto slices = binASTMetadata_->sliceBase();
            auto sourceBase = reinterpret_cast<const char*>(binASTSource());

            for (uint32_t i = 0; i < numStrings; i++) {
                uint8_t isNull;
                if (mode == XDR_ENCODE) {
                    atom = binASTMetadata_->getAtom(i);
                    isNull = !atom;
                }
                MOZ_TRY(xdr->codeUint8(&isNull));
                if (isNull) {
                    atom = nullptr;
                } else {
                    MOZ_TRY(XDRAtom(xdr, &atom));
                }
                if (mode == XDR_DECODE) {
                    atomsBase[i] = atom;
                }

                uint64_t sliceOffset;
                uint32_t sliceLen;
                if (mode == XDR_ENCODE) {
                    auto &slice = binASTMetadata_->getSlice(i);
                    sliceOffset = slice.begin()-sourceBase;
                    sliceLen = slice.byteLen_;
                }

                MOZ_TRY(xdr->codeUint64(&sliceOffset));
                MOZ_TRY(xdr->codeUint32(&sliceLen));

                if (mode == XDR_DECODE) {
                    new (&slices[i]) frontend::BinASTSourceMetadata::CharSlice(sourceBase + sliceOffset, sliceLen);
                }
            }
#else
            // No BinAST, no BinASTMetadata
            MOZ_ASSERT(mode != XDR_ENCODE);
            return xdr->fail(JS::TranscodeResult_Throw);
#endif // JS_BUILD_BINAST
        }
=======
    uint8_t type;
    if (mode == XDR_ENCODE) {
      type = static_cast<uint8_t>(ss->data.match(XDRDataTag()));
>>>>>>> upstream-releases
    }
    MOZ_TRY(xdr->codeUint8(&type));

<<<<<<< HEAD
    uint8_t hasMetadata = !!binASTMetadata_;
    MOZ_TRY(xdr->codeUint8(&hasMetadata));
    if (hasMetadata) {
#if defined(JS_BUILD_BINAST)
      uint32_t numBinKinds;
      uint32_t numStrings;
      if (mode == XDR_ENCODE) {
        numBinKinds = binASTMetadata_->numBinKinds();
        numStrings = binASTMetadata_->numStrings();
      }
      MOZ_TRY(xdr->codeUint32(&numBinKinds));
      MOZ_TRY(xdr->codeUint32(&numStrings));

      if (mode == XDR_DECODE) {
        // Use calloc, since we're storing this immediately, and filling it
        // might GC, to avoid marking bogus atoms.
        auto metadata = static_cast<frontend::BinASTSourceMetadata*>(
            js_calloc(frontend::BinASTSourceMetadata::totalSize(numBinKinds,
                                                                numStrings)));
        if (!metadata) {
          return xdr->fail(JS::TranscodeResult_Throw);
        }
        new (metadata) frontend::BinASTSourceMetadata(numBinKinds, numStrings);
        setBinASTSourceMetadata(metadata);
      }

      for (uint32_t i = 0; i < numBinKinds; i++) {
        frontend::BinKind* binKindBase = binASTMetadata_->binKindBase();
        MOZ_TRY(xdr->codeEnum32(&binKindBase[i]));
      }
||||||| merged common ancestors
    uint8_t haveSourceMap = hasSourceMapURL();
    MOZ_TRY(xdr->codeUint8(&haveSourceMap));
=======
    if (type > static_cast<uint8_t>(DataType::BinAST)) {
      // Fail in debug, but only soft-fail in release, if the type is invalid.
      MOZ_ASSERT_UNREACHABLE("bad tag");
      return xdr->fail(JS::TranscodeResult_Failure_BadDecode);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
      RootedAtom atom(xdr->cx());
      JSAtom** atomsBase = binASTMetadata_->atomsBase();
      auto slices = binASTMetadata_->sliceBase();
      auto sourceBase = reinterpret_cast<const char*>(binASTSource());
||||||| merged common ancestors
    if (haveSourceMap) {
        uint32_t sourceMapURLLen = (mode == XDR_DECODE) ? 0 : js_strlen(sourceMapURL_.get());
        MOZ_TRY(xdr->codeUint32(&sourceMapURLLen));
=======
    tag = static_cast<DataType>(type);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      for (uint32_t i = 0; i < numStrings; i++) {
        uint8_t isNull;
        if (mode == XDR_ENCODE) {
          atom = binASTMetadata_->getAtom(i);
          isNull = !atom;
        }
        MOZ_TRY(xdr->codeUint8(&isNull));
        if (isNull) {
          atom = nullptr;
        } else {
          MOZ_TRY(XDRAtom(xdr, &atom));
        }
        if (mode == XDR_DECODE) {
          atomsBase[i] = atom;
        }

        uint64_t sliceOffset;
        uint32_t sliceLen;
        if (mode == XDR_ENCODE) {
          auto& slice = binASTMetadata_->getSlice(i);
          sliceOffset = slice.begin() - sourceBase;
          sliceLen = slice.byteLen_;
        }

        MOZ_TRY(xdr->codeUint64(&sliceOffset));
        MOZ_TRY(xdr->codeUint32(&sliceLen));

        if (mode == XDR_DECODE) {
          new (&slices[i]) frontend::BinASTSourceMetadata::CharSlice(
              sourceBase + sliceOffset, sliceLen);
        }
      }
#else
      // No BinAST, no BinASTMetadata
      MOZ_ASSERT(mode != XDR_ENCODE);
      return xdr->fail(JS::TranscodeResult_Throw);
#endif  // JS_BUILD_BINAST
    }
  }
||||||| merged common ancestors
        if (mode == XDR_DECODE) {
            sourceMapURL_ = xdr->cx()->template make_pod_array<char16_t>(sourceMapURLLen + 1);
            if (!sourceMapURL_) {
                return xdr->fail(JS::TranscodeResult_Throw);
            }
        }
        auto guard = mozilla::MakeScopeExit([&] {
            if (mode == XDR_DECODE) {
                sourceMapURL_ = nullptr;
            }
        });
        MOZ_TRY(xdr->codeChars(sourceMapURL_.get(), sourceMapURLLen));
        guard.release();
        sourceMapURL_[sourceMapURLLen] = '\0';
    }

    uint8_t haveDisplayURL = hasDisplayURL();
    MOZ_TRY(xdr->codeUint8(&haveDisplayURL));

    if (haveDisplayURL) {
        uint32_t displayURLLen = (mode == XDR_DECODE) ? 0 : js_strlen(displayURL_.get());
        MOZ_TRY(xdr->codeUint32(&displayURLLen));

        if (mode == XDR_DECODE) {
            displayURL_ = xdr->cx()->template make_pod_array<char16_t>(displayURLLen + 1);
            if (!displayURL_) {
                return xdr->fail(JS::TranscodeResult_Throw);
            }
        }
        auto guard = mozilla::MakeScopeExit([&] {
            if (mode == XDR_DECODE) {
                displayURL_ = nullptr;
            }
        });
        MOZ_TRY(xdr->codeChars(displayURL_.get(), displayURLLen));
        guard.release();
        displayURL_[displayURLLen] = '\0';
    }

    uint8_t haveFilename = !!filename_;
    MOZ_TRY(xdr->codeUint8(&haveFilename));

    if (haveFilename) {
        const char* fn = filename();
        MOZ_TRY(xdr->codeCString(&fn));
        // Note: If the decoder has an option, then the filename is defined by
        // the CompileOption from the document.
        MOZ_ASSERT_IF(mode == XDR_DECODE && xdr->hasOptions(), filename());
        if (mode == XDR_DECODE && !xdr->hasOptions() && !setFilename(xdr->cx(), fn)) {
            return xdr->fail(JS::TranscodeResult_Throw);
        }
=======
  switch (tag) {
    case DataType::CompressedUtf8Retrievable:
      ScriptSource::codeRetrievable<Utf8Unit, Compressed, mode>(ss);
      return Ok();
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint8_t haveSourceMap = hasSourceMapURL();
  MOZ_TRY(xdr->codeUint8(&haveSourceMap));

  if (haveSourceMap) {
    uint32_t sourceMapURLLen =
        (mode == XDR_DECODE) ? 0 : js_strlen(sourceMapURL_.get());
    MOZ_TRY(xdr->codeUint32(&sourceMapURLLen));

    if (mode == XDR_DECODE) {
      sourceMapURL_ =
          xdr->cx()->template make_pod_array<char16_t>(sourceMapURLLen + 1);
      if (!sourceMapURL_) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }
    }
    auto guard = mozilla::MakeScopeExit([&] {
      if (mode == XDR_DECODE) {
        sourceMapURL_ = nullptr;
      }
    });
    MOZ_TRY(xdr->codeChars(sourceMapURL_.get(), sourceMapURLLen));
    guard.release();
    sourceMapURL_[sourceMapURLLen] = '\0';
  }
||||||| merged common ancestors
        // Note the content of sources decoded when recording or replaying.
        if (mode == XDR_DECODE &&
            hasSourceText() &&
            mozilla::recordreplay::IsRecordingOrReplaying())
        {
            UncompressedSourceCache::AutoHoldEntry holder;

            if (hasSourceType<Utf8Unit>()) {
                // UTF-8 source text.
                ScriptSource::PinnedUnits<Utf8Unit> units(xdr->cx(), this, holder, 0, length());
                if (!units.get()) {
                    return xdr->fail(JS::TranscodeResult_Throw);
                }
                mozilla::recordreplay::NoteContentParse8(this, filename(), "application/javascript",
                                                         units.get(), length());
            } else {
                // UTF-16 source text.
                ScriptSource::PinnedUnits<char16_t> units(xdr->cx(), this, holder, 0, length());
                if (!units.get()) {
                    return xdr->fail(JS::TranscodeResult_Throw);
                }
                mozilla::recordreplay::NoteContentParse16(this, filename(), "application/javascript",
                                                          units.get(), length());
            }
        }
    }
=======
    case DataType::CompressedUtf8NotRetrievable:
      return ScriptSource::codeCompressedData<Utf8Unit>(xdr, ss);
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint8_t haveDisplayURL = hasDisplayURL();
  MOZ_TRY(xdr->codeUint8(&haveDisplayURL));

  if (haveDisplayURL) {
    uint32_t displayURLLen =
        (mode == XDR_DECODE) ? 0 : js_strlen(displayURL_.get());
    MOZ_TRY(xdr->codeUint32(&displayURLLen));

    if (mode == XDR_DECODE) {
      displayURL_ =
          xdr->cx()->template make_pod_array<char16_t>(displayURLLen + 1);
      if (!displayURL_) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }
    }
    auto guard = mozilla::MakeScopeExit([&] {
      if (mode == XDR_DECODE) {
        displayURL_ = nullptr;
      }
    });
    MOZ_TRY(xdr->codeChars(displayURL_.get(), displayURLLen));
    guard.release();
    displayURL_[displayURLLen] = '\0';
  }

  uint8_t haveFilename = !!filename_;
  MOZ_TRY(xdr->codeUint8(&haveFilename));

  if (haveFilename) {
    const char* fn = filename();
    MOZ_TRY(xdr->codeCString(&fn));
    // Note: If the decoder has an option, then the filename is defined by
    // the CompileOption from the document.
    MOZ_ASSERT_IF(mode == XDR_DECODE && xdr->hasOptions(), filename());
    if (mode == XDR_DECODE && !xdr->hasOptions() &&
        !setFilename(xdr->cx(), fn)) {
      return xdr->fail(JS::TranscodeResult_Throw);
    }

    // Note the content of sources decoded when recording or replaying.
    if (mode == XDR_DECODE && hasSourceText() &&
        mozilla::recordreplay::IsRecordingOrReplaying()) {
      UncompressedSourceCache::AutoHoldEntry holder;

      if (hasSourceType<Utf8Unit>()) {
        // UTF-8 source text.
        ScriptSource::PinnedUnits<Utf8Unit> units(xdr->cx(), this, holder, 0,
                                                  length());
        if (!units.get()) {
          return xdr->fail(JS::TranscodeResult_Throw);
        }
        mozilla::recordreplay::NoteContentParse(
            this, filename(), "application/javascript", units.get(), length());
      } else {
        // UTF-16 source text.
        ScriptSource::PinnedUnits<char16_t> units(xdr->cx(), this, holder, 0,
                                                  length());
        if (!units.get()) {
          return xdr->fail(JS::TranscodeResult_Throw);
        }
        mozilla::recordreplay::NoteContentParse(
            this, filename(), "application/javascript", units.get(), length());
      }
    }
  }

  return Ok();
}
||||||| merged common ancestors
    return Ok();
}
=======
    case DataType::UncompressedUtf8Retrievable:
      ScriptSource::codeRetrievable<Utf8Unit, Uncompressed, mode>(ss);
      return Ok();
>>>>>>> upstream-releases

<<<<<<< HEAD
// Format and return a cx->pod_malloc'ed URL for a generated script like:
//   {filename} line {lineno} > {introducer}
// For example:
//   foo.js line 7 > eval
// indicating code compiled by the call to 'eval' on line 7 of foo.js.
char* js::FormatIntroducedFilename(JSContext* cx, const char* filename,
                                   unsigned lineno, const char* introducer) {
  // Compute the length of the string in advance, so we can allocate a
  // buffer of the right size on the first shot.
  //
  // (JS_smprintf would be perfect, as that allocates the result
  // dynamically as it formats the string, but it won't allocate from cx,
  // and wants us to use a special free function.)
  char linenoBuf[15];
  size_t filenameLen = strlen(filename);
  size_t linenoLen = SprintfLiteral(linenoBuf, "%u", lineno);
  size_t introducerLen = strlen(introducer);
  size_t len = filenameLen + 6 /* == strlen(" line ") */ + linenoLen +
               3 /* == strlen(" > ") */ + introducerLen + 1 /* \0 */;
  char* formatted = cx->pod_malloc<char>(len);
  if (!formatted) {
    return nullptr;
  }
||||||| merged common ancestors
// Format and return a cx->pod_malloc'ed URL for a generated script like:
//   {filename} line {lineno} > {introducer}
// For example:
//   foo.js line 7 > eval
// indicating code compiled by the call to 'eval' on line 7 of foo.js.
char*
js::FormatIntroducedFilename(JSContext* cx, const char* filename, unsigned lineno,
                             const char* introducer)
{
    // Compute the length of the string in advance, so we can allocate a
    // buffer of the right size on the first shot.
    //
    // (JS_smprintf would be perfect, as that allocates the result
    // dynamically as it formats the string, but it won't allocate from cx,
    // and wants us to use a special free function.)
    char linenoBuf[15];
    size_t filenameLen = strlen(filename);
    size_t linenoLen = SprintfLiteral(linenoBuf, "%u", lineno);
    size_t introducerLen = strlen(introducer);
    size_t len = filenameLen                    +
                 6 /* == strlen(" line ") */    +
                 linenoLen                      +
                 3 /* == strlen(" > ") */       +
                 introducerLen                  +
                 1 /* \0 */;
    char* formatted = cx->pod_malloc<char>(len);
    if (!formatted) {
        return nullptr;
    }
=======
    case DataType::UncompressedUtf8NotRetrievable:
      return ScriptSource::codeUncompressedData<Utf8Unit>(xdr, ss);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::DebugOnly<size_t> checkLen = snprintf(
      formatted, len, "%s line %s > %s", filename, linenoBuf, introducer);
  MOZ_ASSERT(checkLen == len - 1);
||||||| merged common ancestors
    mozilla::DebugOnly<size_t> checkLen = snprintf(formatted, len, "%s line %s > %s",
                                                   filename, linenoBuf, introducer);
    MOZ_ASSERT(checkLen == len - 1);
=======
    case DataType::CompressedUtf16Retrievable:
      ScriptSource::codeRetrievable<char16_t, Compressed, mode>(ss);
      return Ok();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return formatted;
}
||||||| merged common ancestors
    return formatted;
}
=======
    case DataType::CompressedUtf16NotRetrievable:
      return ScriptSource::codeCompressedData<char16_t>(xdr, ss);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool ScriptSource::initFromOptions(JSContext* cx,
                                   const ReadOnlyCompileOptions& options,
                                   const Maybe<uint32_t>& parameterListEnd) {
  MOZ_ASSERT(!filename_);
  MOZ_ASSERT(!introducerFilename_);
||||||| merged common ancestors
bool
ScriptSource::initFromOptions(JSContext* cx, const ReadOnlyCompileOptions& options,
                              const Maybe<uint32_t>& parameterListEnd)
{
    MOZ_ASSERT(!filename_);
    MOZ_ASSERT(!introducerFilename_);
=======
    case DataType::UncompressedUtf16Retrievable:
      ScriptSource::codeRetrievable<char16_t, Uncompressed, mode>(ss);
      return Ok();
>>>>>>> upstream-releases

<<<<<<< HEAD
  mutedErrors_ = options.mutedErrors();
||||||| merged common ancestors
    mutedErrors_ = options.mutedErrors();
=======
    case DataType::UncompressedUtf16NotRetrievable:
      return ScriptSource::codeUncompressedData<char16_t>(xdr, ss);
>>>>>>> upstream-releases

<<<<<<< HEAD
  introductionType_ = options.introductionType;
  setIntroductionOffset(options.introductionOffset);
  parameterListEnd_ = parameterListEnd.isSome() ? parameterListEnd.value() : 0;
||||||| merged common ancestors
    introductionType_ = options.introductionType;
    setIntroductionOffset(options.introductionOffset);
    parameterListEnd_ = parameterListEnd.isSome() ? parameterListEnd.value() : 0;
=======
    case DataType::Missing: {
      MOZ_ASSERT(ss->data.is<Missing>(),
                 "ScriptSource::data is initialized as missing, so neither "
                 "encoding nor decoding has to change anything");
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (options.hasIntroductionInfo) {
    MOZ_ASSERT(options.introductionType != nullptr);
    const char* filename =
        options.filename() ? options.filename() : "<unknown>";
    char* formatted = FormatIntroducedFilename(
        cx, filename, options.introductionLineno, options.introductionType);
    if (!formatted) {
      return false;
||||||| merged common ancestors
    if (options.hasIntroductionInfo) {
        MOZ_ASSERT(options.introductionType != nullptr);
        const char* filename = options.filename() ? options.filename() : "<unknown>";
        char* formatted = FormatIntroducedFilename(cx, filename, options.introductionLineno,
                                                   options.introductionType);
        if (!formatted) {
            return false;
        }
        filename_.reset(formatted);
    } else if (options.filename()) {
        if (!setFilename(cx, options.filename())) {
            return false;
        }
=======
      // There's no data to XDR for missing source.
      break;
>>>>>>> upstream-releases
    }
    filename_.reset(formatted);
  } else if (options.filename()) {
    if (!setFilename(cx, options.filename())) {
      return false;
    }
  }

<<<<<<< HEAD
  if (options.introducerFilename()) {
    introducerFilename_ = DuplicateString(cx, options.introducerFilename());
    if (!introducerFilename_) {
      return false;
    }
  }
||||||| merged common ancestors
    if (options.introducerFilename()) {
        introducerFilename_ = DuplicateString(cx, options.introducerFilename());
        if (!introducerFilename_) {
            return false;
        }
    }
=======
    case DataType::RetrievableUtf8:
      ScriptSource::codeRetrievableData<Utf8Unit, mode>(ss);
      return Ok();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
    case DataType::RetrievableUtf16:
      ScriptSource::codeRetrievableData<char16_t, mode>(ss);
      return Ok();
>>>>>>> upstream-releases

<<<<<<< HEAD
bool ScriptSource::setFilename(JSContext* cx, const char* filename) {
  MOZ_ASSERT(!filename_);
  filename_ = DuplicateString(cx, filename);
  return filename_ != nullptr;
}

bool ScriptSource::setDisplayURL(JSContext* cx, const char16_t* displayURL) {
  MOZ_ASSERT(displayURL);
  if (hasDisplayURL()) {
    // FIXME: filename_.get() should be UTF-8 (bug 987069).
    if (!cx->helperThread() &&
        !JS_ReportErrorFlagsAndNumberLatin1(
            cx, JSREPORT_WARNING, GetErrorMessage, nullptr,
            JSMSG_ALREADY_HAS_PRAGMA, filename_.get(), "//# sourceURL")) {
      return false;
    }
  }
  size_t len = js_strlen(displayURL) + 1;
  if (len == 1) {
    return true;
  }
||||||| merged common ancestors
bool
ScriptSource::setFilename(JSContext* cx, const char* filename)
{
    MOZ_ASSERT(!filename_);
    filename_ = DuplicateString(cx, filename);
    return filename_ != nullptr;
}

bool
ScriptSource::setDisplayURL(JSContext* cx, const char16_t* displayURL)
{
    MOZ_ASSERT(displayURL);
    if (hasDisplayURL()) {
        // FIXME: filename_.get() should be UTF-8 (bug 987069).
        if (!cx->helperThread() &&
            !JS_ReportErrorFlagsAndNumberLatin1(cx, JSREPORT_WARNING,
                                                GetErrorMessage, nullptr,
                                                JSMSG_ALREADY_HAS_PRAGMA, filename_.get(),
                                                "//# sourceURL"))
        {
            return false;
        }
    }
    size_t len = js_strlen(displayURL) + 1;
    if (len == 1) {
        return true;
    }
=======
    case DataType::BinAST:
      return codeBinASTData(xdr, ss);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  displayURL_ = DuplicateString(cx, displayURL);
  return displayURL_ != nullptr;
||||||| merged common ancestors
    displayURL_ = DuplicateString(cx, displayURL);
    return displayURL_ != nullptr;
=======
  // The range-check on |type| far above ought ensure the above |switch| is
  // exhaustive and all cases will return, but not all compilers understand
  // this.  Make the Missing case break to here so control obviously never flows
  // off the end.
  MOZ_ASSERT(tag == DataType::Missing);
  return Ok();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool ScriptSource::setSourceMapURL(JSContext* cx,
                                   const char16_t* sourceMapURL) {
  MOZ_ASSERT(sourceMapURL);
||||||| merged common ancestors
bool
ScriptSource::setSourceMapURL(JSContext* cx, const char16_t* sourceMapURL)
{
    MOZ_ASSERT(sourceMapURL);
=======
template <XDRMode mode>
/* static */
XDRResult ScriptSource::XDR(XDRState<mode>* xdr,
                            const mozilla::Maybe<JS::CompileOptions>& options,
                            MutableHandle<ScriptSourceHolder> holder) {
  JSContext* cx = xdr->cx();
  ScriptSource* ss = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t len = js_strlen(sourceMapURL) + 1;
  if (len == 1) {
    return true;
  }
||||||| merged common ancestors
    size_t len = js_strlen(sourceMapURL) + 1;
    if (len == 1) {
        return true;
    }
=======
  if (mode == XDR_ENCODE) {
    ss = holder.get().get();
  } else {
    // Allocate a new ScriptSource and root it with the holder.
    ss = cx->new_<ScriptSource>();
    if (!ss) {
      return xdr->fail(JS::TranscodeResult_Throw);
    }
    holder.get().reset(ss);
>>>>>>> upstream-releases

<<<<<<< HEAD
  sourceMapURL_ = DuplicateString(cx, sourceMapURL);
  return sourceMapURL_ != nullptr;
}

/*
 * [SMDOC] JSScript data layout (shared)
 *
 * Shared script data management.
 *
 * SharedScriptData::data contains data that can be shared within a
 * runtime. The atoms() data is placed first to simplify its alignment.
 *
 * Array elements   Pointed to by         Length
 * --------------   -------------         ------
 * GCPtrAtom        atoms()               natoms()
 * jsbytecode       code()                codeLength()
 * jsscrnote        notes()               numNotes()
 */

SharedScriptData* js::SharedScriptData::new_(JSContext* cx, uint32_t codeLength,
                                             uint32_t srcnotesLength,
                                             uint32_t natoms) {
  size_t dataLength = natoms * sizeof(GCPtrAtom) + codeLength + srcnotesLength;
  size_t allocLength = offsetof(SharedScriptData, data_) + dataLength;
  auto entry =
      reinterpret_cast<SharedScriptData*>(cx->pod_malloc<uint8_t>(allocLength));
  if (!entry) {
    ReportOutOfMemory(cx);
    return nullptr;
  }
||||||| merged common ancestors
    sourceMapURL_ = DuplicateString(cx, sourceMapURL);
    return sourceMapURL_ != nullptr;
}

/*
 * [SMDOC] JSScript data layout (shared)
 *
 * Shared script data management.
 *
 * SharedScriptData::data contains data that can be shared within a
 * runtime. The atoms() data is placed first to simplify its alignment.
 *
 * Array elements   Pointed to by         Length
 * --------------   -------------         ------
 * GCPtrAtom        atoms()               natoms()
 * jsbytecode       code()                codeLength()
 * jsscrnote        notes()               numNotes()
 */

SharedScriptData*
js::SharedScriptData::new_(JSContext* cx, uint32_t codeLength,
                           uint32_t srcnotesLength, uint32_t natoms)
{
    size_t dataLength = natoms * sizeof(GCPtrAtom) + codeLength + srcnotesLength;
    size_t allocLength = offsetof(SharedScriptData, data_) + dataLength;
    auto entry = reinterpret_cast<SharedScriptData*>(cx->pod_malloc<uint8_t>(allocLength));
    if (!entry) {
        ReportOutOfMemory(cx);
        return nullptr;
    }

    /* Diagnostic for Bug 1399373.
     * We expect bytecode is always non-empty. */
    MOZ_DIAGNOSTIC_ASSERT(codeLength > 0);
=======
    // We use this CompileOptions only to initialize the ScriptSourceObject.
    // Most CompileOptions fields aren't used by ScriptSourceObject, and those
    // that are (element; elementAttributeName) aren't preserved by XDR. So
    // this can be simple.
    if (!ss->initFromOptions(cx, *options)) {
      return xdr->fail(JS::TranscodeResult_Throw);
    }
  }

  MOZ_TRY(xdrData(xdr, ss));
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Diagnostic for Bug 1399373.
   * We expect bytecode is always non-empty. */
  MOZ_DIAGNOSTIC_ASSERT(codeLength > 0);
||||||| merged common ancestors
    entry->refCount_ = 0;
    entry->natoms_ = natoms;
    entry->codeLength_ = codeLength;
    entry->noteLength_ = srcnotesLength;
=======
  uint8_t haveSourceMap = ss->hasSourceMapURL();
  MOZ_TRY(xdr->codeUint8(&haveSourceMap));
>>>>>>> upstream-releases

<<<<<<< HEAD
  entry->refCount_ = 0;
  entry->natoms_ = natoms;
  entry->codeLength_ = codeLength;
  entry->noteLength_ = srcnotesLength;
||||||| merged common ancestors
    /*
     * Call constructors to initialize the storage that will be accessed as a
     * GCPtrAtom array via atoms().
     */
    static_assert(offsetof(SharedScriptData, data_) % sizeof(GCPtrAtom) == 0,
                  "atoms must have GCPtrAtom alignment");
    GCPtrAtom* atoms = entry->atoms();
    for (unsigned i = 0; i < natoms; ++i) {
        new (&atoms[i]) GCPtrAtom();
    }
=======
  if (haveSourceMap) {
    UniqueTwoByteChars& sourceMapURL(ss->sourceMapURL_);
    uint32_t sourceMapURLLen =
        (mode == XDR_DECODE) ? 0 : js_strlen(sourceMapURL.get());
    MOZ_TRY(xdr->codeUint32(&sourceMapURLLen));

    if (mode == XDR_DECODE) {
      sourceMapURL =
          xdr->cx()->template make_pod_array<char16_t>(sourceMapURLLen + 1);
      if (!sourceMapURL) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }
    }
    auto guard = mozilla::MakeScopeExit([&] {
      if (mode == XDR_DECODE) {
        sourceMapURL = nullptr;
      }
    });
    MOZ_TRY(xdr->codeChars(sourceMapURL.get(), sourceMapURLLen));
    guard.release();
    sourceMapURL[sourceMapURLLen] = '\0';
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /*
   * Call constructors to initialize the storage that will be accessed as a
   * GCPtrAtom array via atoms().
   */
  static_assert(offsetof(SharedScriptData, data_) % sizeof(GCPtrAtom) == 0,
                "atoms must have GCPtrAtom alignment");
  GCPtrAtom* atoms = entry->atoms();
  for (unsigned i = 0; i < natoms; ++i) {
    new (&atoms[i]) GCPtrAtom();
  }
||||||| merged common ancestors
    // Sanity check the dataLength() computation
    MOZ_ASSERT(entry->dataLength() == dataLength);
=======
  uint8_t haveDisplayURL = ss->hasDisplayURL();
  MOZ_TRY(xdr->codeUint8(&haveDisplayURL));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Sanity check the dataLength() computation
  MOZ_ASSERT(entry->dataLength() == dataLength);

  return entry;
}
||||||| merged common ancestors
    return entry;
}
=======
  if (haveDisplayURL) {
    UniqueTwoByteChars& displayURL(ss->displayURL_);
    uint32_t displayURLLen =
        (mode == XDR_DECODE) ? 0 : js_strlen(displayURL.get());
    MOZ_TRY(xdr->codeUint32(&displayURLLen));
>>>>>>> upstream-releases

<<<<<<< HEAD
inline js::ScriptBytecodeHasher::Lookup::Lookup(SharedScriptData* data)
    : scriptData(data),
      hash(mozilla::HashBytes(scriptData->data(), scriptData->dataLength())) {
  scriptData->incRefCount();
}
||||||| merged common ancestors
inline
js::ScriptBytecodeHasher::Lookup::Lookup(SharedScriptData* data)
  : scriptData(data),
    hash(mozilla::HashBytes(scriptData->data(), scriptData->dataLength()))
{
    scriptData->incRefCount();
}
=======
    if (mode == XDR_DECODE) {
      displayURL =
          xdr->cx()->template make_pod_array<char16_t>(displayURLLen + 1);
      if (!displayURL) {
        return xdr->fail(JS::TranscodeResult_Throw);
      }
    }
    auto guard = mozilla::MakeScopeExit([&] {
      if (mode == XDR_DECODE) {
        displayURL = nullptr;
      }
    });
    MOZ_TRY(xdr->codeChars(displayURL.get(), displayURLLen));
    guard.release();
    displayURL[displayURLLen] = '\0';
  }

  uint8_t haveFilename = !!ss->filename_;
  MOZ_TRY(xdr->codeUint8(&haveFilename));

  if (haveFilename) {
    const char* fn = ss->filename();
    MOZ_TRY(xdr->codeCString(&fn));
    // Note: If the decoder has an option, then the filename is defined by
    // the CompileOption from the document.
    MOZ_ASSERT_IF(mode == XDR_DECODE && xdr->hasOptions(), ss->filename());
    if (mode == XDR_DECODE && !xdr->hasOptions() &&
        !ss->setFilename(xdr->cx(), fn)) {
      return xdr->fail(JS::TranscodeResult_Throw);
    }

    // Note the content of sources decoded when recording or replaying.
    if (mode == XDR_DECODE && ss->hasSourceText() &&
        mozilla::recordreplay::IsRecordingOrReplaying()) {
      UncompressedSourceCache::AutoHoldEntry holder;

      if (ss->hasSourceType<Utf8Unit>()) {
        // UTF-8 source text.
        ScriptSource::PinnedUnits<Utf8Unit> units(xdr->cx(), ss, holder, 0,
                                                  ss->length());
        if (!units.get()) {
          return xdr->fail(JS::TranscodeResult_Throw);
        }
        mozilla::recordreplay::NoteContentParse(ss, ss->filename(),
                                                "application/javascript",
                                                units.get(), ss->length());
      } else {
        // UTF-16 source text.
        ScriptSource::PinnedUnits<char16_t> units(xdr->cx(), ss, holder, 0,
                                                  ss->length());
        if (!units.get()) {
          return xdr->fail(JS::TranscodeResult_Throw);
        }
        mozilla::recordreplay::NoteContentParse(ss, ss->filename(),
                                                "application/javascript",
                                                units.get(), ss->length());
      }
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
inline js::ScriptBytecodeHasher::Lookup::~Lookup() {
  scriptData->decRefCount();
||||||| merged common ancestors
inline
js::ScriptBytecodeHasher::Lookup::~Lookup()
{
    scriptData->decRefCount();
=======
  return Ok();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool JSScript::createSharedScriptData(JSContext* cx, uint32_t codeLength,
                                      uint32_t noteLength, uint32_t natoms) {
  MOZ_ASSERT(!scriptData());
  SharedScriptData* ssd =
      SharedScriptData::new_(cx, codeLength, noteLength, natoms);
  if (!ssd) {
    return false;
  }
||||||| merged common ancestors
bool
JSScript::createScriptData(JSContext* cx, uint32_t codeLength, uint32_t srcnotesLength,
                           uint32_t natoms)
{
    MOZ_ASSERT(!scriptData());
    SharedScriptData* ssd = SharedScriptData::new_(cx, codeLength, srcnotesLength, natoms);
    if (!ssd) {
        return false;
    }
=======
// Format and return a cx->pod_malloc'ed URL for a generated script like:
//   {filename} line {lineno} > {introducer}
// For example:
//   foo.js line 7 > eval
// indicating code compiled by the call to 'eval' on line 7 of foo.js.
char* js::FormatIntroducedFilename(JSContext* cx, const char* filename,
                                   unsigned lineno, const char* introducer) {
  // Compute the length of the string in advance, so we can allocate a
  // buffer of the right size on the first shot.
  //
  // (JS_smprintf would be perfect, as that allocates the result
  // dynamically as it formats the string, but it won't allocate from cx,
  // and wants us to use a special free function.)
  char linenoBuf[15];
  size_t filenameLen = strlen(filename);
  size_t linenoLen = SprintfLiteral(linenoBuf, "%u", lineno);
  size_t introducerLen = strlen(introducer);
  size_t len = filenameLen + 6 /* == strlen(" line ") */ + linenoLen +
               3 /* == strlen(" > ") */ + introducerLen + 1 /* \0 */;
  char* formatted = cx->pod_malloc<char>(len);
  if (!formatted) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  setScriptData(ssd);
  return true;
}
||||||| merged common ancestors
    setScriptData(ssd);
    return true;
}
=======
  mozilla::DebugOnly<size_t> checkLen = snprintf(
      formatted, len, "%s line %s > %s", filename, linenoBuf, introducer);
  MOZ_ASSERT(checkLen == len - 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
void JSScript::freeScriptData() {
  if (scriptData_) {
    scriptData_->decRefCount();
    scriptData_ = nullptr;
  }
||||||| merged common ancestors
void
JSScript::freeScriptData()
{
    scriptData_->decRefCount();
    scriptData_ = nullptr;
=======
  return formatted;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void JSScript::setScriptData(js::SharedScriptData* data) {
  MOZ_ASSERT(!scriptData_);
  scriptData_ = data;
  scriptData_->incRefCount();
}
||||||| merged common ancestors
void
JSScript::setScriptData(js::SharedScriptData* data)
{
    MOZ_ASSERT(!scriptData_);
    scriptData_ = data;
    scriptData_->incRefCount();
}
=======
bool ScriptSource::initFromOptions(JSContext* cx,
                                   const ReadOnlyCompileOptions& options,
                                   const Maybe<uint32_t>& parameterListEnd) {
  MOZ_ASSERT(!filename_);
  MOZ_ASSERT(!introducerFilename_);
>>>>>>> upstream-releases

<<<<<<< HEAD
/*
 * Takes ownership of its *ssd parameter and either adds it into the runtime's
 * ScriptDataTable or frees it if a matching entry already exists.
 *
 * Sets the |code| and |atoms| fields on the given JSScript.
 */
bool JSScript::shareScriptData(JSContext* cx) {
  SharedScriptData* ssd = scriptData();
  MOZ_ASSERT(ssd);
  MOZ_ASSERT(ssd->refCount() == 1);
||||||| merged common ancestors
/*
 * Takes ownership of its *ssd parameter and either adds it into the runtime's
 * ScriptDataTable or frees it if a matching entry already exists.
 *
 * Sets the |code| and |atoms| fields on the given JSScript.
 */
bool
JSScript::shareScriptData(JSContext* cx)
{
    SharedScriptData* ssd = scriptData();
    MOZ_ASSERT(ssd);
    MOZ_ASSERT(ssd->refCount() == 1);

    // Calculate the hash before taking the lock. Because the data is reference
    // counted, it also will be freed after releasing the lock if necessary.
    ScriptBytecodeHasher::Lookup lookup(ssd);

    AutoLockScriptData lock(cx->runtime());

    ScriptDataTable::AddPtr p = cx->scriptDataTable(lock).lookupForAdd(lookup);
    if (p) {
        MOZ_ASSERT(ssd != *p);
        freeScriptData();
        setScriptData(*p);
    } else {
        if (!cx->scriptDataTable(lock).add(p, ssd)) {
            freeScriptData();
            ReportOutOfMemory(cx);
            return false;
        }
=======
  mutedErrors_ = options.mutedErrors();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Calculate the hash before taking the lock. Because the data is reference
  // counted, it also will be freed after releasing the lock if necessary.
  ScriptBytecodeHasher::Lookup lookup(ssd);

  AutoLockScriptData lock(cx->runtime());

  ScriptDataTable::AddPtr p = cx->scriptDataTable(lock).lookupForAdd(lookup);
  if (p) {
    MOZ_ASSERT(ssd != *p);
    freeScriptData();
    setScriptData(*p);
  } else {
    if (!cx->scriptDataTable(lock).add(p, ssd)) {
      freeScriptData();
      ReportOutOfMemory(cx);
      return false;
    }
||||||| merged common ancestors
        // Being in the table counts as a reference on the script data.
        scriptData()->incRefCount();
    }
=======
  introductionType_ = options.introductionType;
  setIntroductionOffset(options.introductionOffset);
  parameterListEnd_ = parameterListEnd.isSome() ? parameterListEnd.value() : 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Being in the table counts as a reference on the script data.
    scriptData()->incRefCount();
  }

  MOZ_ASSERT(scriptData()->refCount() >= 2);
  return true;
}
||||||| merged common ancestors
    MOZ_ASSERT(scriptData()->refCount() >= 2);
    return true;
}
=======
  if (options.hasIntroductionInfo) {
    MOZ_ASSERT(options.introductionType != nullptr);
    const char* filename =
        options.filename() ? options.filename() : "<unknown>";
    char* formatted = FormatIntroducedFilename(
        cx, filename, options.introductionLineno, options.introductionType);
    if (!formatted) {
      return false;
    }
    filename_.reset(formatted);
  } else if (options.filename()) {
    if (!setFilename(cx, options.filename())) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void js::SweepScriptData(JSRuntime* rt) {
  // Entries are removed from the table when their reference count is one,
  // i.e. when the only reference to them is from the table entry.
||||||| merged common ancestors
void
js::SweepScriptData(JSRuntime* rt)
{
    // Entries are removed from the table when their reference count is one,
    // i.e. when the only reference to them is from the table entry.
=======
  if (options.introducerFilename()) {
    introducerFilename_ = DuplicateString(cx, options.introducerFilename());
    if (!introducerFilename_) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoLockScriptData lock(rt);
  ScriptDataTable& table = rt->scriptDataTable(lock);
||||||| merged common ancestors
    AutoLockScriptData lock(rt);
    ScriptDataTable& table = rt->scriptDataTable(lock);
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (ScriptDataTable::Enum e(table); !e.empty(); e.popFront()) {
    SharedScriptData* scriptData = e.front();
    if (scriptData->refCount() == 1) {
      scriptData->decRefCount();
      e.removeFront();
    }
  }
||||||| merged common ancestors
    for (ScriptDataTable::Enum e(table); !e.empty(); e.popFront()) {
        SharedScriptData* scriptData = e.front();
        if (scriptData->refCount() == 1) {
            scriptData->decRefCount();
            e.removeFront();
        }
    }
=======
bool ScriptSource::setFilename(JSContext* cx, const char* filename) {
  MOZ_ASSERT(!filename_);
  filename_ = DuplicateString(cx, filename);
  return filename_ != nullptr;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void js::FreeScriptData(JSRuntime* rt) {
  AutoLockScriptData lock(rt);
||||||| merged common ancestors
void
js::FreeScriptData(JSRuntime* rt)
{
    AutoLockScriptData lock(rt);
=======
bool ScriptSource::setDisplayURL(JSContext* cx, const char16_t* displayURL) {
  MOZ_ASSERT(displayURL);
  if (hasDisplayURL()) {
    // FIXME: filename_.get() should be UTF-8 (bug 987069).
    if (!cx->isHelperThreadContext() &&
        !JS_ReportErrorFlagsAndNumberLatin1(
            cx, JSREPORT_WARNING, GetErrorMessage, nullptr,
            JSMSG_ALREADY_HAS_PRAGMA, filename_.get(), "//# sourceURL")) {
      return false;
    }
  }
  size_t len = js_strlen(displayURL) + 1;
  if (len == 1) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ScriptDataTable& table = rt->scriptDataTable(lock);
||||||| merged common ancestors
    ScriptDataTable& table = rt->scriptDataTable(lock);
=======
  displayURL_ = DuplicateString(cx, displayURL);
  return displayURL_ != nullptr;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The table should be empty unless the embedding leaked GC things.
  MOZ_ASSERT_IF(rt->gc.shutdownCollectedEverything(), table.empty());
||||||| merged common ancestors
    // The table should be empty unless the embedding leaked GC things.
    MOZ_ASSERT_IF(rt->gc.shutdownCollectedEverything(), table.empty());
=======
bool ScriptSource::setSourceMapURL(JSContext* cx,
                                   const char16_t* sourceMapURL) {
  MOZ_ASSERT(sourceMapURL);
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (ScriptDataTable::Enum e(table); !e.empty(); e.popFront()) {
#ifdef DEBUG
    SharedScriptData* scriptData = e.front();
    fprintf(stderr,
            "ERROR: GC found live SharedScriptData %p with ref count %d at "
            "shutdown\n",
            scriptData, scriptData->refCount());
#endif
    js_free(e.front());
  }

  table.clear();
}

// Placement-new elements of an array. This should optimize away for types with
// trivial default initiation.
template <typename T>
static void DefaultInitializeElements(void* arrayPtr, size_t length) {
  uintptr_t elem = reinterpret_cast<uintptr_t>(arrayPtr);
  MOZ_ASSERT(elem % alignof(T) == 0);

  for (size_t i = 0; i < length; ++i) {
    new (reinterpret_cast<T*>(elem)) T;
    elem += sizeof(T);
  }
}

/* static */ size_t PrivateScriptData::AllocationSize(
    uint32_t nscopes, uint32_t nconsts, uint32_t nobjects, uint32_t ntrynotes,
    uint32_t nscopenotes, uint32_t nresumeoffsets) {
  size_t size = sizeof(PrivateScriptData);

  if (nconsts) {
    size += sizeof(PackedSpan);
  }
  if (nobjects) {
    size += sizeof(PackedSpan);
  }
  if (ntrynotes) {
    size += sizeof(PackedSpan);
  }
  if (nscopenotes) {
    size += sizeof(PackedSpan);
  }
  if (nresumeoffsets) {
    size += sizeof(PackedSpan);
  }

  size += nscopes * sizeof(GCPtrScope);

  if (nconsts) {
    // The scope array doesn't maintain Value alignment, so compute the
    // padding needed to remedy this.
    size = JS_ROUNDUP(size, alignof(GCPtrValue));
    size += nconsts * sizeof(GCPtrValue);
  }
  if (nobjects) {
    size += nobjects * sizeof(GCPtrObject);
  }
  if (ntrynotes) {
    size += ntrynotes * sizeof(JSTryNote);
  }
  if (nscopenotes) {
    size += nscopenotes * sizeof(ScopeNote);
  }
  if (nresumeoffsets) {
    size += nresumeoffsets * sizeof(uint32_t);
  }

  return size;
}

// Placement-new elements of an array. This should optimize away for types with
// trivial default initiation.
template <typename T>
void PrivateScriptData::initElements(size_t offset, size_t length) {
  uintptr_t base = reinterpret_cast<uintptr_t>(this);
  DefaultInitializeElements<T>(reinterpret_cast<void*>(base + offset), length);
}

template <typename T>
void PrivateScriptData::initSpan(size_t* cursor, uint32_t scaledSpanOffset,
                                 size_t length) {
  // PackedSpans are elided when arrays are empty
  if (scaledSpanOffset == 0) {
    MOZ_ASSERT(length == 0);
    return;
  }

  // Placement-new the PackedSpan
  PackedSpan* span = packedOffsetToPointer<PackedSpan>(scaledSpanOffset);
  span = new (span) PackedSpan{uint32_t(*cursor), uint32_t(length)};

  // Placement-new the elements
  initElements<T>(*cursor, length);

  // Advance cursor
  (*cursor) += length * sizeof(T);
}

// Initialize PackedSpans and placement-new the trailing arrays.
PrivateScriptData::PrivateScriptData(uint32_t nscopes_, uint32_t nconsts,
                                     uint32_t nobjects, uint32_t ntrynotes,
                                     uint32_t nscopenotes,
                                     uint32_t nresumeoffsets)
    : nscopes(nscopes_) {
  // Convert cursor possition to a packed offset.
  auto ToPackedOffset = [](size_t cursor) {
    MOZ_ASSERT(cursor % PackedOffsets::SCALE == 0);
    return cursor / PackedOffsets::SCALE;
  };

  // Helper to allocate a PackedSpan from the variable length data.
  auto TakeSpan = [=](size_t* cursor) {
    size_t packedOffset = ToPackedOffset(*cursor);
    MOZ_ASSERT(packedOffset <= PackedOffsets::MAX_OFFSET);

    (*cursor) += sizeof(PackedSpan);
    return packedOffset;
  };

  // Variable-length data begins immediately after PrivateScriptData itself.
  // NOTE: Alignment is computed using cursor/offset so the alignment of
  // PrivateScriptData must be stricter than any trailing array type.
  size_t cursor = sizeof(*this);

  // Layout PackedSpan structures and initialize packedOffsets fields.
  static_assert(alignof(PrivateScriptData) >= alignof(PackedSpan),
                "Incompatible alignment");
  if (nconsts) {
    packedOffsets.constsSpanOffset = TakeSpan(&cursor);
  }
  if (nobjects) {
    packedOffsets.objectsSpanOffset = TakeSpan(&cursor);
  }
  if (ntrynotes) {
    packedOffsets.tryNotesSpanOffset = TakeSpan(&cursor);
  }
  if (nscopenotes) {
    packedOffsets.scopeNotesSpanOffset = TakeSpan(&cursor);
  }
  if (nresumeoffsets) {
    packedOffsets.resumeOffsetsSpanOffset = TakeSpan(&cursor);
  }

  // Layout and initialize the scopes array. Manually insert padding so that
  // the subsequent |consts| array is aligned.
  {
    MOZ_ASSERT(nscopes > 0);

    static_assert(alignof(PackedSpan) >= alignof(GCPtrScope),
                  "Incompatible alignment");
    initElements<GCPtrScope>(cursor, nscopes);
    packedOffsets.scopesOffset = ToPackedOffset(cursor);

    cursor += nscopes * sizeof(GCPtrScope);
  }

  if (nconsts) {
    // Pad to required alignment if we are emitting constant array.
    cursor = JS_ROUNDUP(cursor, alignof(GCPtrValue));

    static_assert(alignof(PrivateScriptData) >= alignof(GCPtrValue),
                  "Incompatible alignment");
    initSpan<GCPtrValue>(&cursor, packedOffsets.constsSpanOffset, nconsts);
  }

  // Layout arrays, initialize PackedSpans and placement-new the elements.
  static_assert(alignof(GCPtrValue) >= alignof(GCPtrObject),
                "Incompatible alignment");
  static_assert(alignof(GCPtrScope) >= alignof(GCPtrObject),
                "Incompatible alignment");
  initSpan<GCPtrObject>(&cursor, packedOffsets.objectsSpanOffset, nobjects);
  static_assert(alignof(GCPtrObject) >= alignof(JSTryNote),
                "Incompatible alignment");
  initSpan<JSTryNote>(&cursor, packedOffsets.tryNotesSpanOffset, ntrynotes);
  static_assert(alignof(JSTryNote) >= alignof(ScopeNote),
                "Incompatible alignment");
  initSpan<ScopeNote>(&cursor, packedOffsets.scopeNotesSpanOffset, nscopenotes);
  static_assert(alignof(ScopeNote) >= alignof(uint32_t),
                "Incompatible alignment");
  initSpan<uint32_t>(&cursor, packedOffsets.resumeOffsetsSpanOffset,
                     nresumeoffsets);

  // Sanity check
  MOZ_ASSERT(AllocationSize(nscopes_, nconsts, nobjects, ntrynotes, nscopenotes,
                            nresumeoffsets) == cursor);
}

/* static */ PrivateScriptData* PrivateScriptData::new_(
    JSContext* cx, uint32_t nscopes, uint32_t nconsts, uint32_t nobjects,
    uint32_t ntrynotes, uint32_t nscopenotes, uint32_t nresumeoffsets,
    uint32_t* dataSize) {
  // Compute size including trailing arrays
  size_t size = AllocationSize(nscopes, nconsts, nobjects, ntrynotes,
                               nscopenotes, nresumeoffsets);

  // Allocate contiguous raw buffer
  void* raw = cx->pod_malloc<uint8_t>(size);
  MOZ_ASSERT(uintptr_t(raw) % alignof(PrivateScriptData) == 0);
  if (!raw) {
    return nullptr;
  }
||||||| merged common ancestors
    for (ScriptDataTable::Enum e(table); !e.empty(); e.popFront()) {
#ifdef DEBUG
        SharedScriptData* scriptData = e.front();
        fprintf(stderr, "ERROR: GC found live SharedScriptData %p with ref count %d at shutdown\n",
                scriptData, scriptData->refCount());
#endif
        js_free(e.front());
    }

    table.clear();
}

/*
 * [SMDOC] JSScript data layout (unshared)
 *
 * JSScript::data and SharedScriptData::data have complex,
 * manually-controlled, memory layouts.
 *
 * JSScript::data begins with some optional array headers. They are optional
 * because they often aren't needed, i.e. the corresponding arrays often have
 * zero elements. Each header has a bit in JSScript::hasArrayBits that
 * indicates if it's present within |data|; from this the offset of each
 * present array header can be computed. Each header has an accessor function
 * in JSScript that encapsulates this offset computation.
 *
 * Array type      Array elements  Accessor
 * ----------      --------------  --------
 * ConstArray      Consts          consts()
 * ObjectArray     Objects         objects()
 * ObjectArray     Regexps         regexps()
 * TryNoteArray    Try notes       trynotes()
 * ScopeNoteArray  Scope notes     scopeNotes()
 *
 * Then are the elements of several arrays.
 * - Most of these arrays have headers listed above (if present). For each of
 *   these, the array pointer and the array length is stored in the header.
 * - The remaining arrays have pointers and lengths that are stored directly in
 *   JSScript. This is because, unlike the others, they are nearly always
 *   non-zero length and so the optional-header space optimization isn't
 *   worthwhile.
 *
 * Array elements   Pointed to by         Length
 * --------------   -------------         ------
 * Consts           consts()->vector      consts()->length
 * Objects          objects()->vector     objects()->length
 * Regexps          regexps()->vector     regexps()->length
 * Try notes        trynotes()->vector    trynotes()->length
 * Scope notes      scopeNotes()->vector  scopeNotes()->length
 *
 * IMPORTANT: This layout has two key properties.
 * - It ensures that everything has sufficient alignment; in particular, the
 *   consts() elements need Value alignment.
 * - It ensures there are no gaps between elements, which saves space and makes
 *   manual layout easy. In particular, in the second part, arrays with larger
 *   elements precede arrays with smaller elements.
 *
 * The following static assertions check JSScript::data's alignment properties.
 */

template<class T>
constexpr bool
KeepsValueAlignment() {
    return alignof(JS::Value) % alignof(T) == 0 &&
           sizeof(T) % sizeof(JS::Value) == 0;
}
=======
  size_t len = js_strlen(sourceMapURL) + 1;
  if (len == 1) {
    return true;
  }

  sourceMapURL_ = DuplicateString(cx, sourceMapURL);
  return sourceMapURL_ != nullptr;
}

/* static */ mozilla::Atomic<uint32_t, mozilla::SequentiallyConsistent,
                             mozilla::recordreplay::Behavior::DontPreserve>
    ScriptSource::idCount_;

/*
 * [SMDOC] JSScript data layout (shared)
 *
 * Shared script data management.
 *
 * SharedScriptData::data contains data that can be shared within a runtime.
 * The atoms() data is placed first to simplify its alignment.
 *
 * Array elements   Pointed to by         Length
 * --------------   -------------         ------
 * GCPtrAtom        atoms()               natoms()
 * jsbytecode       code()                codeLength()
 * jsscrnote        notes()               noteLength()
 * uint32_t         resumeOffsets()
 * ScopeNote        scopeNotes()
 * JSTryNote        tryNotes()
 */

SharedScriptData* js::SharedScriptData::new_(
    JSContext* cx, uint32_t codeLength, uint32_t noteLength, uint32_t natoms,
    uint32_t numResumeOffsets, uint32_t numScopeNotes, uint32_t numTryNotes) {
  // Compute size including trailing arrays
  size_t size = AllocationSize(codeLength, noteLength, natoms, numResumeOffsets,
                               numScopeNotes, numTryNotes);

  // Allocate contiguous raw buffer
  void* raw = cx->pod_malloc<uint8_t>(size);
  MOZ_ASSERT(uintptr_t(raw) % alignof(SharedScriptData) == 0);
  if (!raw) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (dataSize) {
    *dataSize = size;
  }
||||||| merged common ancestors
template<class T>
constexpr bool
HasValueAlignment() {
    return alignof(JS::Value) == alignof(T) &&
           sizeof(T) == sizeof(JS::Value);
}
=======
  // Constuct the SharedScriptData. Trailing arrays are uninitialized but
  // GCPtrs are put into a safe state.
  return new (raw)
      SharedScriptData(codeLength, noteLength, natoms, numResumeOffsets,
                       numScopeNotes, numTryNotes);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Constuct the PrivateScriptData. Trailing arrays are uninitialized but
  // GCPtrs are put into a safe state.
  return new (raw) PrivateScriptData(nscopes, nconsts, nobjects, ntrynotes,
                                     nscopenotes, nresumeoffsets);
||||||| merged common ancestors
template<class T1, class T2>
constexpr bool
NoPaddingBetweenEntries() {
    return alignof(T1) % alignof(T2) == 0;
=======
bool JSScript::createSharedScriptData(JSContext* cx, uint32_t codeLength,
                                      uint32_t noteLength, uint32_t natoms,
                                      uint32_t numResumeOffsets,
                                      uint32_t numScopeNotes,
                                      uint32_t numTryNotes) {
#ifdef DEBUG
  // The compact arrays need to maintain uint32_t alignment. This should have
  // been done by padding out source notes.
  size_t byteArrayLength =
      sizeof(SharedScriptData::Flags) + codeLength + noteLength;
  MOZ_ASSERT(byteArrayLength % sizeof(uint32_t) == 0,
             "Source notes should have been padded already");
#endif

  MOZ_ASSERT(!scriptData_);
  scriptData_ =
      SharedScriptData::new_(cx, codeLength, noteLength, natoms,
                             numResumeOffsets, numScopeNotes, numTryNotes);
  return !!scriptData_;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void PrivateScriptData::traceChildren(JSTracer* trc) {
  auto scopearray = scopes();
  TraceRange(trc, scopearray.size(), scopearray.data(), "scopes");
||||||| merged common ancestors
/*
 * These assertions ensure that there is no padding between the array headers,
 * and also that the consts() elements (which follow immediately afterward) are
 * Value-aligned.  (There is an assumption that |data| itself is Value-aligned;
 * we check this below).
 */
JS_STATIC_ASSERT(KeepsValueAlignment<ConstArray>());
JS_STATIC_ASSERT(KeepsValueAlignment<ObjectArray>());       /* there are two of these */
JS_STATIC_ASSERT(KeepsValueAlignment<TryNoteArray>());
JS_STATIC_ASSERT(KeepsValueAlignment<ScopeNoteArray>());

/* These assertions ensure there is no padding required between array elements. */
JS_STATIC_ASSERT(HasValueAlignment<GCPtrValue>());
JS_STATIC_ASSERT((NoPaddingBetweenEntries<GCPtrValue, GCPtrObject>()));
JS_STATIC_ASSERT((NoPaddingBetweenEntries<GCPtrObject, GCPtrObject>()));
JS_STATIC_ASSERT((NoPaddingBetweenEntries<GCPtrObject, JSTryNote>()));
JS_STATIC_ASSERT((NoPaddingBetweenEntries<JSTryNote, uint32_t>()));
JS_STATIC_ASSERT((NoPaddingBetweenEntries<uint32_t, uint32_t>()));

JS_STATIC_ASSERT((NoPaddingBetweenEntries<GCPtrValue, ScopeNote>()));
JS_STATIC_ASSERT((NoPaddingBetweenEntries<ScopeNote, ScopeNote>()));
JS_STATIC_ASSERT((NoPaddingBetweenEntries<JSTryNote, ScopeNote>()));
JS_STATIC_ASSERT((NoPaddingBetweenEntries<GCPtrObject, ScopeNote>()));
JS_STATIC_ASSERT((NoPaddingBetweenEntries<ScopeNote, uint32_t>()));

static inline size_t
ScriptDataSize(uint32_t nscopes, uint32_t nconsts, uint32_t nobjects,
               uint32_t ntrynotes, uint32_t nscopenotes, uint32_t nyieldoffsets)
{
    size_t size = 0;
=======
void JSScript::freeScriptData() { scriptData_ = nullptr; }

/*
 * Takes ownership of its *ssd parameter and either adds it into the runtime's
 * ScriptDataTable or frees it if a matching entry already exists.
 *
 * Sets the |code| and |atoms| fields on the given JSScript.
 */
bool JSScript::shareScriptData(JSContext* cx) {
  SharedScriptData* ssd = scriptData();
  MOZ_ASSERT(ssd);
  MOZ_ASSERT(ssd->refCount() == 1);

  // Calculate the hash before taking the lock. Because the data is reference
  // counted, it also will be freed after releasing the lock if necessary.
  SharedScriptDataHasher::Lookup lookup(ssd);

  AutoLockScriptData lock(cx->runtime());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (hasConsts()) {
    auto constarray = consts();
    TraceRange(trc, constarray.size(), constarray.data(), "consts");
  }

  if (hasObjects()) {
    auto objarray = objects();
    TraceRange(trc, objarray.size(), objarray.data(), "objects");
  }
}
||||||| merged common ancestors
    MOZ_ASSERT(nscopes != 0);
    size += sizeof(ScopeArray) + nscopes * sizeof(Scope*);
    if (nconsts != 0) {
        size += sizeof(ConstArray) + nconsts * sizeof(Value);
    }
    if (nobjects != 0) {
        size += sizeof(ObjectArray) + nobjects * sizeof(NativeObject*);
    }
    if (ntrynotes != 0) {
        size += sizeof(TryNoteArray) + ntrynotes * sizeof(JSTryNote);
    }
    if (nscopenotes != 0) {
        size += sizeof(ScopeNoteArray) + nscopenotes * sizeof(ScopeNote);
    }
    if (nyieldoffsets != 0) {
        size += sizeof(YieldAndAwaitOffsetArray) + nyieldoffsets * sizeof(uint32_t);
    }

     return size;
}
=======
  ScriptDataTable::AddPtr p = cx->scriptDataTable(lock).lookupForAdd(lookup);
  if (p) {
    MOZ_ASSERT(ssd != *p);
    scriptData_ = *p;
  } else {
    if (!cx->scriptDataTable(lock).add(p, ssd)) {
      ReportOutOfMemory(cx);
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
JSScript::JSScript(JS::Realm* realm, uint8_t* stubEntry,
                   HandleObject sourceObject, uint32_t sourceStart,
                   uint32_t sourceEnd, uint32_t toStringStart,
                   uint32_t toStringEnd)
    :
#ifndef JS_CODEGEN_NONE
      jitCodeRaw_(stubEntry),
      jitCodeSkipArgCheck_(stubEntry),
#endif
      realm_(realm),
      sourceStart_(sourceStart),
      sourceEnd_(sourceEnd),
      toStringStart_(toStringStart),
      toStringEnd_(toStringEnd) {
  // See JSScript.h for further details.
  MOZ_ASSERT(toStringStart <= sourceStart);
  MOZ_ASSERT(sourceStart <= sourceEnd);
  MOZ_ASSERT(sourceEnd <= toStringEnd);

  setSourceObject(sourceObject);
}

/* static */ JSScript* JSScript::New(JSContext* cx, HandleObject sourceObject,
                                     uint32_t sourceStart, uint32_t sourceEnd,
                                     uint32_t toStringStart,
                                     uint32_t toStringEnd) {
  void* script = Allocate<JSScript>(cx);
  if (!script) {
    return nullptr;
  }
||||||| merged common ancestors
JSScript::JSScript(JS::Realm* realm, uint8_t* stubEntry, const ReadOnlyCompileOptions& options,
                   HandleObject sourceObject, uint32_t bufStart, uint32_t bufEnd,
                   uint32_t toStringStart, uint32_t toStringEnd)
  :
#ifndef JS_CODEGEN_NONE
    jitCodeRaw_(stubEntry),
    jitCodeSkipArgCheck_(stubEntry),
#endif
    realm_(realm),
    sourceStart_(bufStart),
    sourceEnd_(bufEnd),
    toStringStart_(toStringStart),
    toStringEnd_(toStringEnd),
#ifdef MOZ_VTUNE
    vtuneMethodId_(vtune::GenerateUniqueMethodID()),
#endif
    bitFields_{} // zeroes everything -- some fields custom-assigned below
{
    // bufStart and bufEnd specify the range of characters parsed by the
    // Parser to produce this script. toStringStart and toStringEnd specify
    // the range of characters to be returned for Function.prototype.toString.
    MOZ_ASSERT(bufStart <= bufEnd);
    MOZ_ASSERT(toStringStart <= toStringEnd);
    MOZ_ASSERT(toStringStart <= bufStart);
    MOZ_ASSERT(toStringEnd >= bufEnd);

    bitFields_.noScriptRval_ = options.noScriptRval;
    bitFields_.selfHosted_ = options.selfHostingMode;
    bitFields_.treatAsRunOnce_ = options.isRunOnce;
    bitFields_.hideScriptFromDebugger_ = options.hideScriptFromDebugger;

    setSourceObject(sourceObject);
}

/* static */ JSScript*
JSScript::createInitialized(JSContext* cx, const ReadOnlyCompileOptions& options,
                            HandleObject sourceObject,
                            uint32_t bufStart, uint32_t bufEnd,
                            uint32_t toStringStart, uint32_t toStringEnd)
{
    void* script = Allocate<JSScript>(cx);
    if (!script) {
        return nullptr;
    }
=======
    // Being in the table counts as a reference on the script data.
    ssd->AddRef();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifndef JS_CODEGEN_NONE
  uint8_t* stubEntry = cx->runtime()->jitRuntime()->interpreterStub().value;
#else
  uint8_t* stubEntry = nullptr;
#endif
||||||| merged common ancestors
    uint8_t* stubEntry =
#ifndef JS_CODEGEN_NONE
        cx->runtime()->jitRuntime()->interpreterStub().value
#else
        nullptr
#endif
        ;

    return new (script) JSScript(cx->realm(), stubEntry, options, sourceObject,
                                 bufStart, bufEnd, toStringStart, toStringEnd);
}
=======
  // Refs: JSScript, ScriptDataTable
  MOZ_ASSERT(scriptData()->refCount() >= 2);

  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return new (script)
      JSScript(cx->realm(), stubEntry, sourceObject, sourceStart, sourceEnd,
               toStringStart, toStringEnd);
||||||| merged common ancestors
/* static */ JSScript*
JSScript::Create(JSContext* cx, const ReadOnlyCompileOptions& options,
                 HandleObject sourceObject, uint32_t bufStart, uint32_t bufEnd,
                 uint32_t toStringStart, uint32_t toStringEnd)
{
    RootedScript script(cx, createInitialized(cx, options, sourceObject, bufStart, bufEnd,
                                              toStringStart, toStringEnd));
    if (!script) {
        return nullptr;
    }

    if (cx->runtime()->lcovOutput().isEnabled()) {
        if (!script->initScriptName(cx)) {
            return nullptr;
        }
    }

    return script;
=======
void js::SweepScriptData(JSRuntime* rt) {
  // Entries are removed from the table when their reference count is one,
  // i.e. when the only reference to them is from the table entry.

  AutoLockScriptData lock(rt);
  ScriptDataTable& table = rt->scriptDataTable(lock);

  for (ScriptDataTable::Enum e(table); !e.empty(); e.popFront()) {
    SharedScriptData* scriptData = e.front();
    if (scriptData->refCount() == 1) {
      scriptData->Release();
      e.removeFront();
    }
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ JSScript* JSScript::Create(
    JSContext* cx, const ReadOnlyCompileOptions& options,
    HandleObject sourceObject, uint32_t sourceStart, uint32_t sourceEnd,
    uint32_t toStringStart, uint32_t toStringEnd) {
  RootedScript script(cx, JSScript::New(cx, sourceObject, sourceStart,
                                        sourceEnd, toStringStart, toStringEnd));
  if (!script) {
    return nullptr;
  }
||||||| merged common ancestors
bool
JSScript::initScriptName(JSContext* cx)
{
    MOZ_ASSERT(!hasScriptName());

    if (!filename()) {
        return true;
    }

    // Create realm's scriptNameMap if necessary.
    if (!realm()->scriptNameMap) {
        auto map = cx->make_unique<ScriptNameMap>();
        if (!map) {
            return false;
        }

        realm()->scriptNameMap = std::move(map);
    }

    UniqueChars name = DuplicateString(filename());
    if (!name) {
        ReportOutOfMemory(cx);
        return false;
    }

    // Register the script name in the realm's map.
    if (!realm()->scriptNameMap->putNew(this, std::move(name))) {
        ReportOutOfMemory(cx);
        return false;
    }
=======
void js::FreeScriptData(JSRuntime* rt) {
  AutoLockScriptData lock(rt);

  ScriptDataTable& table = rt->scriptDataTable(lock);

  // The table should be empty unless the embedding leaked GC things.
  MOZ_ASSERT_IF(rt->gc.shutdownCollectedEverything(), table.empty());

#ifdef DEBUG
  size_t numLive = 0;
  size_t maxCells = 5;
  char* env = getenv("JS_GC_MAX_LIVE_CELLS");
  if (env && *env) {
    maxCells = atol(env);
  }
#endif

  for (ScriptDataTable::Enum e(table); !e.empty(); e.popFront()) {
#ifdef DEBUG
    if (++numLive <= maxCells) {
      SharedScriptData* scriptData = e.front();
      fprintf(stderr,
              "ERROR: GC found live SharedScriptData %p with ref count %d at "
              "shutdown\n",
              scriptData, scriptData->refCount());
    }
#endif
    js_free(e.front());
  }

#ifdef DEBUG
  if (numLive > 0) {
    fprintf(stderr, "ERROR: GC found %zu live SharedScriptData at shutdown\n",
            numLive);
  }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Record compile options that get checked at runtime.
  script->setFlag(ImmutableFlags::NoScriptRval, options.noScriptRval);
  script->setFlag(ImmutableFlags::SelfHosted, options.selfHostingMode);
  script->setFlag(ImmutableFlags::TreatAsRunOnce, options.isRunOnce);
  script->setFlag(MutableFlags::HideScriptFromDebugger,
                  options.hideScriptFromDebugger);
||||||| merged common ancestors
    return true;
}
=======
  table.clear();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (cx->runtime()->lcovOutput().isEnabled()) {
    if (!script->initScriptName(cx)) {
      return nullptr;
    }
  }
||||||| merged common ancestors
static inline uint8_t*
AllocScriptData(JSContext* cx, size_t size)
{
    if (!size) {
        return nullptr;
    }
=======
/* static */
size_t PrivateScriptData::AllocationSize(uint32_t ngcthings) {
  size_t size = sizeof(PrivateScriptData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return script;
||||||| merged common ancestors
    uint8_t* data = cx->pod_calloc<uint8_t>(JS_ROUNDUP(size, sizeof(Value)));
    if (!data) {
        return nullptr;
    }
    MOZ_ASSERT(size_t(data) % sizeof(Value) == 0);
    return data;
=======
  size += ngcthings * sizeof(JS::GCCellPtr);

  return size;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
#ifdef MOZ_VTUNE
uint32_t JSScript::vtuneMethodID() {
  if (!realm()->scriptVTuneIdMap) {
    auto map = MakeUnique<ScriptVTuneIdMap>();
    if (!map) {
      MOZ_CRASH("Failed to allocate ScriptVTuneIdMap");
    }
||||||| merged common ancestors
/* static */ bool
JSScript::partiallyInit(JSContext* cx, HandleScript script, uint32_t nscopes,
                        uint32_t nconsts, uint32_t nobjects, uint32_t ntrynotes,
                        uint32_t nscopenotes, uint32_t nyieldoffsets)
{
    cx->check(script);

    size_t size = ScriptDataSize(nscopes, nconsts, nobjects, ntrynotes,
                                 nscopenotes, nyieldoffsets);
    script->data = AllocScriptData(cx, size);
    if (size && !script->data) {
        return false;
    }

    script->dataSize_ = size;

    uint8_t* cursor = script->data;

    // There must always be at least 1 scope, the body scope.
    MOZ_ASSERT(nscopes != 0);
    cursor += sizeof(ScopeArray);

    if (nconsts != 0) {
        script->setHasArray(CONSTS);
        cursor += sizeof(ConstArray);
    }
    if (nobjects != 0) {
        script->setHasArray(OBJECTS);
        cursor += sizeof(ObjectArray);
    }

    if (ntrynotes != 0) {
        script->setHasArray(TRYNOTES);
        cursor += sizeof(TryNoteArray);
    }
    if (nscopenotes != 0) {
        script->setHasArray(SCOPENOTES);
        cursor += sizeof(ScopeNoteArray);
    }

    YieldAndAwaitOffsetArray* yieldAndAwaitOffsets = nullptr;
    if (nyieldoffsets != 0) {
        yieldAndAwaitOffsets = reinterpret_cast<YieldAndAwaitOffsetArray*>(cursor);
        cursor += sizeof(YieldAndAwaitOffsetArray);
    }

    if (nconsts != 0) {
        MOZ_ASSERT(reinterpret_cast<uintptr_t>(cursor) % sizeof(JS::Value) == 0);
        script->constsRaw()->length = nconsts;
        script->constsRaw()->vector = (GCPtrValue*)cursor;
        cursor += nconsts * sizeof(script->constsRaw()->vector[0]);
    }
=======
// Placement-new elements of an array. This should optimize away for types with
// trivial default initiation.
template <typename T>
void PrivateScriptData::initElements(size_t offset, size_t length) {
  void* raw = offsetToPointer<void>(offset);
  DefaultInitializeElements<T>(raw, length);
}

// Initialize and placement-new the trailing arrays.
PrivateScriptData::PrivateScriptData(uint32_t ngcthings)
    : ngcthings(ngcthings) {
  // Variable-length data begins immediately after PrivateScriptData itself.
  // NOTE: Alignment is computed using cursor/offset so the alignment of
  // PrivateScriptData must be stricter than any trailing array type.
  size_t cursor = sizeof(*this);

  // Layout and initialize the gcthings array.
  {
    MOZ_ASSERT(ngcthings > 0);

    static_assert(alignof(PrivateScriptData) >= alignof(JS::GCCellPtr),
                  "Incompatible alignment");
    initElements<JS::GCCellPtr>(cursor, ngcthings);

    cursor += ngcthings * sizeof(JS::GCCellPtr);
  }

  // Sanity check
  MOZ_ASSERT(AllocationSize(ngcthings) == cursor);
}

/* static */
PrivateScriptData* PrivateScriptData::new_(JSContext* cx, uint32_t ngcthings,
                                           uint32_t* dataSize) {
  // Compute size including trailing arrays
  size_t size = AllocationSize(ngcthings);

  // Allocate contiguous raw buffer
  void* raw = cx->pod_malloc<uint8_t>(size);
  MOZ_ASSERT(uintptr_t(raw) % alignof(PrivateScriptData) == 0);
  if (!raw) {
    return nullptr;
  }

  if (dataSize) {
    *dataSize = size;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    realm()->scriptVTuneIdMap = std::move(map);
  }
||||||| merged common ancestors
    script->scopesRaw()->length = nscopes;
    script->scopesRaw()->vector = (GCPtrScope*)cursor;
    cursor += nscopes * sizeof(script->scopesRaw()->vector[0]);
=======
  // Constuct the PrivateScriptData. Trailing arrays are uninitialized but
  // GCPtrs are put into a safe state.
  return new (raw) PrivateScriptData(ngcthings);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  ScriptVTuneIdMap::AddPtr p = realm()->scriptVTuneIdMap->lookupForAdd(this);
  if (p) {
    return p->value();
  }
||||||| merged common ancestors
    if (nobjects != 0) {
        script->objectsRaw()->length = nobjects;
        script->objectsRaw()->vector = (GCPtrObject*)cursor;
        cursor += nobjects * sizeof(script->objectsRaw()->vector[0]);
    }
=======
/* static */ bool PrivateScriptData::InitFromEmitter(
    JSContext* cx, js::HandleScript script, frontend::BytecodeEmitter* bce) {
  uint32_t ngcthings = bce->perScriptData().gcThingList().length();
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t id = vtune::GenerateUniqueMethodID();
  if (!realm()->scriptVTuneIdMap->add(p, this, id)) {
    MOZ_CRASH("Failed to add vtune method id");
  }
||||||| merged common ancestors
    if (ntrynotes != 0) {
        script->trynotesRaw()->length = ntrynotes;
        script->trynotesRaw()->vector = reinterpret_cast<JSTryNote*>(cursor);
        size_t vectorSize = ntrynotes * sizeof(script->trynotesRaw()->vector[0]);
#ifdef DEBUG
        memset(cursor, 0, vectorSize);
#endif
        cursor += vectorSize;
    }
=======
  // Create and initialize PrivateScriptData
  if (!JSScript::createPrivateScriptData(cx, script, ngcthings)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return id;
}
#endif
||||||| merged common ancestors
    if (nscopenotes != 0) {
        script->scopeNotesRaw()->length = nscopenotes;
        script->scopeNotesRaw()->vector = reinterpret_cast<ScopeNote*>(cursor);
        size_t vectorSize = nscopenotes * sizeof(script->scopeNotesRaw()->vector[0]);
#ifdef DEBUG
        memset(cursor, 0, vectorSize);
#endif
        cursor += vectorSize;
    }
=======
  js::PrivateScriptData* data = script->data_;
  if (ngcthings) {
    bce->perScriptData().gcThingList().finish(data->gcthings());
  }

  return true;
}

void PrivateScriptData::trace(JSTracer* trc) {
  for (JS::GCCellPtr& elem : gcthings()) {
    gc::Cell* thing = elem.asCell();
    TraceManuallyBarrieredGenericPointerEdge(trc, &thing, "script-gcthing");
    if (thing != elem.asCell()) {
      elem = JS::GCCellPtr(thing, elem.kind());
    }
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool JSScript::initScriptName(JSContext* cx) {
  MOZ_ASSERT(!hasScriptName());
||||||| merged common ancestors
    if (nyieldoffsets != 0) {
        yieldAndAwaitOffsets->init(reinterpret_cast<uint32_t*>(cursor), nyieldoffsets);
        size_t vectorSize = nyieldoffsets * sizeof(script->yieldAndAwaitOffsetsRaw()[0]);
#ifdef DEBUG
        memset(cursor, 0, vectorSize);
#endif
        cursor += vectorSize;
    }
=======
JSScript::JSScript(JS::Realm* realm, uint8_t* stubEntry,
                   HandleScriptSourceObject sourceObject, uint32_t sourceStart,
                   uint32_t sourceEnd, uint32_t toStringStart,
                   uint32_t toStringEnd)
    :
#ifndef JS_CODEGEN_NONE
      jitCodeRaw_(stubEntry),
#endif
      realm_(realm),
      sourceStart_(sourceStart),
      sourceEnd_(sourceEnd),
      toStringStart_(toStringStart),
      toStringEnd_(toStringEnd) {
  // See JSScript.h for further details.
  MOZ_ASSERT(toStringStart <= sourceStart);
  MOZ_ASSERT(sourceStart <= sourceEnd);
  MOZ_ASSERT(sourceEnd <= toStringEnd);

  setSourceObject(sourceObject);
}

/* static */
JSScript* JSScript::New(JSContext* cx, HandleScriptSourceObject sourceObject,
                        uint32_t sourceStart, uint32_t sourceEnd,
                        uint32_t toStringStart, uint32_t toStringEnd) {
  void* script = Allocate<JSScript>(cx);
  if (!script) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!filename()) {
    return true;
  }
||||||| merged common ancestors
    MOZ_ASSERT(cursor == script->data + size);
    return true;
}
=======
#ifndef JS_CODEGEN_NONE
  uint8_t* stubEntry = cx->runtime()->jitRuntime()->interpreterStub().value;
#else
  uint8_t* stubEntry = nullptr;
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Create realm's scriptNameMap if necessary.
  if (!realm()->scriptNameMap) {
    auto map = cx->make_unique<ScriptNameMap>();
    if (!map) {
      return false;
    }
||||||| merged common ancestors
/* static */ bool
JSScript::initFunctionPrototype(JSContext* cx, Handle<JSScript*> script,
                                HandleFunction functionProto)
{
    uint32_t numScopes = 1;
    uint32_t numConsts = 0;
    uint32_t numObjects = 0;
    uint32_t numTryNotes = 0;
    uint32_t numScopeNotes = 0;
    uint32_t numYieldAndAwaitOffsets = 0;
    if (!partiallyInit(cx, script, numScopes, numConsts, numObjects, numTryNotes,
                       numScopeNotes, numYieldAndAwaitOffsets))
    {
        return false;
    }
=======
  return new (script)
      JSScript(cx->realm(), stubEntry, sourceObject, sourceStart, sourceEnd,
               toStringStart, toStringEnd);
}

static bool ShouldTrackRecordReplayProgress(JSScript* script) {
  // Progress is only tracked when recording or replaying, and only for
  // scripts associated with the main thread's runtime. Whether self hosted
  // scripts execute may depend on performed Ion optimizations (for example,
  // self hosted TypedObject logic), so they are ignored.
  return MOZ_UNLIKELY(mozilla::recordreplay::IsRecordingOrReplaying()) &&
         !script->runtimeFromAnyThread()->parentRuntime &&
         !script->selfHosted() &&
         mozilla::recordreplay::ShouldUpdateProgressCounter(script->filename());
}

/* static */
JSScript* JSScript::Create(JSContext* cx, const ReadOnlyCompileOptions& options,
                           HandleScriptSourceObject sourceObject,
                           uint32_t sourceStart, uint32_t sourceEnd,
                           uint32_t toStringStart, uint32_t toStringEnd) {
  RootedScript script(cx, JSScript::New(cx, sourceObject, sourceStart,
                                        sourceEnd, toStringStart, toStringEnd));
  if (!script) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    realm()->scriptNameMap = std::move(map);
  }
||||||| merged common ancestors
    script->nTypeSets_ = 0;

    RootedScope enclosing(cx, &cx->global()->emptyGlobalScope());
    Scope* functionProtoScope = FunctionScope::create(cx, nullptr, false, false, functionProto,
                                                      enclosing);
    if (!functionProtoScope) {
        return false;
    }
    script->scopesRaw()->vector[0].init(functionProtoScope);
=======
  // Record compile options that get checked at runtime.
  script->setFlag(ImmutableFlags::NoScriptRval, options.noScriptRval);
  script->setFlag(ImmutableFlags::SelfHosted, options.selfHostingMode);
  script->setFlag(ImmutableFlags::TreatAsRunOnce, options.isRunOnce);
  script->setFlag(MutableFlags::HideScriptFromDebugger,
                  options.hideScriptFromDebugger);

  script->setFlag(MutableFlags::TrackRecordReplayProgress,
                  ShouldTrackRecordReplayProgress(script));
>>>>>>> upstream-releases

<<<<<<< HEAD
  UniqueChars name = DuplicateString(filename());
  if (!name) {
    ReportOutOfMemory(cx);
    return false;
  }
||||||| merged common ancestors
    uint32_t codeLength = 1;
    uint32_t srcNotesLength = 1;
    uint32_t numAtoms = 0;
    if (!script->createScriptData(cx, codeLength, srcNotesLength, numAtoms)) {
        return false;
    }
=======
  if (coverage::IsLCovEnabled()) {
    if (!script->initScriptName(cx)) {
      return nullptr;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Register the script name in the realm's map.
  if (!realm()->scriptNameMap->putNew(this, std::move(name))) {
    ReportOutOfMemory(cx);
    return false;
  }
||||||| merged common ancestors
    jsbytecode* code = script->code();
    code[0] = JSOP_RETRVAL;
    code[1] = SRC_NULL;
    return script->shareScriptData(cx);
}
=======
  return script;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
static void
InitAtomMap(frontend::AtomIndexMap& indices, GCPtrAtom* atoms)
{
    for (frontend::AtomIndexMap::Range r = indices.all(); !r.empty(); r.popFront()) {
        JSAtom* atom = r.front().key();
        uint32_t index = r.front().value();
        MOZ_ASSERT(index < indices.count());
        atoms[index].init(atom);
    }
}
=======
#ifdef MOZ_VTUNE
uint32_t JSScript::vtuneMethodID() {
  if (!realm()->scriptVTuneIdMap) {
    auto map = MakeUnique<ScriptVTuneIdMap>();
    if (!map) {
      MOZ_CRASH("Failed to allocate ScriptVTuneIdMap");
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
static inline uint8_t* AllocScriptData(JSContext* cx, size_t size) {
  if (!size) {
    return nullptr;
  }
||||||| merged common ancestors
/* static */ void
JSScript::initFromFunctionBox(HandleScript script, frontend::FunctionBox* funbox)
{
    JSFunction* fun = funbox->function();
    if (fun->isInterpretedLazy()) {
        fun->setUnlazifiedScript(script);
    } else {
        fun->setScript(script);
    }

    script->bitFields_.funHasExtensibleScope_ = funbox->hasExtensibleScope();
    script->bitFields_.needsHomeObject_ = funbox->needsHomeObject();
    script->bitFields_.isDerivedClassConstructor_ = funbox->isDerivedClassConstructor();

    if (funbox->argumentsHasLocalBinding()) {
        script->setArgumentsHasVarBinding();
        if (funbox->definitelyNeedsArgsObj()) {
            script->setNeedsArgsObj(true);
        }
    } else {
        MOZ_ASSERT(!funbox->definitelyNeedsArgsObj());
    }
    script->bitFields_.hasMappedArgsObj_ = funbox->hasMappedArgsObj();

    script->bitFields_.functionHasThisBinding_ = funbox->hasThisBinding();
    script->bitFields_.functionHasExtraBodyVarScope_ = funbox->hasExtraBodyVarScope();

    script->funLength_ = funbox->length;

    script->setGeneratorKind(funbox->generatorKind());
    script->setAsyncKind(funbox->asyncKind());
    if (funbox->hasRest()) {
        script->setHasRest();
    }

    PositionalFormalParameterIter fi(script);
    while (fi && !fi.closedOver()) {
        fi++;
    }
    script->bitFields_.funHasAnyAliasedFormal_ = !!fi;
=======
    realm()->scriptVTuneIdMap = std::move(map);
  }

  ScriptVTuneIdMap::AddPtr p = realm()->scriptVTuneIdMap->lookupForAdd(this);
  if (p) {
    return p->value();
  }

  uint32_t id = vtune::GenerateUniqueMethodID();
  if (!realm()->scriptVTuneIdMap->add(p, this, id)) {
    MOZ_CRASH("Failed to add vtune method id");
  }

  return id;
}
#endif

bool JSScript::initScriptName(JSContext* cx) {
  MOZ_ASSERT(!hasScriptName());

  if (!filename()) {
    return true;
  }

  // Create realm's scriptNameMap if necessary.
  if (!realm()->scriptNameMap) {
    auto map = cx->make_unique<ScriptNameMap>();
    if (!map) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint8_t* data = cx->pod_calloc<uint8_t>(JS_ROUNDUP(size, sizeof(Value)));
  if (!data) {
    return nullptr;
  }
  MOZ_ASSERT(size_t(data) % sizeof(Value) == 0);
  return data;
}
||||||| merged common ancestors
    script->setHasInnerFunctions(funbox->hasInnerFunctions());
}
=======
    realm()->scriptNameMap = std::move(map);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
/* static */ bool JSScript::createPrivateScriptData(
    JSContext* cx, HandleScript script, uint32_t nscopes, uint32_t nconsts,
    uint32_t nobjects, uint32_t ntrynotes, uint32_t nscopenotes,
    uint32_t nresumeoffsets) {
  cx->check(script);
||||||| merged common ancestors
/* static */ void
JSScript::initFromModuleContext(HandleScript script)
{
    script->bitFields_.funHasExtensibleScope_ = false;
    script->bitFields_.needsHomeObject_ = false;
    script->bitFields_.isDerivedClassConstructor_ = false;
    script->funLength_ = 0;

    script->setGeneratorKind(GeneratorKind::NotGenerator);

    // Since modules are only run once, mark the script so that initializers
    // created within it may be given more precise types.
    script->setTreatAsRunOnce();
    MOZ_ASSERT(!script->hasRunOnce());
}
=======
  UniqueChars name = DuplicateString(filename());
  if (!name) {
    ReportOutOfMemory(cx);
    return false;
  }

  // Register the script name in the realm's map.
  if (!realm()->scriptNameMap->putNew(this, std::move(name))) {
    ReportOutOfMemory(cx);
    return false;
  }

  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t dataSize;

  PrivateScriptData* data =
      PrivateScriptData::new_(cx, nscopes, nconsts, nobjects, ntrynotes,
                              nscopenotes, nresumeoffsets, &dataSize);
  if (!data) {
    return false;
  }

  script->data_ = data;
  script->dataSize_ = dataSize;
  return true;
}

/* static */ bool JSScript::initFunctionPrototype(
    JSContext* cx, HandleScript script, HandleFunction functionProto) {
  uint32_t numScopes = 1;
  uint32_t numConsts = 0;
  uint32_t numObjects = 0;
  uint32_t numTryNotes = 0;
  uint32_t numScopeNotes = 0;
  uint32_t nresumeoffsets = 0;
  if (!createPrivateScriptData(cx, script, numScopes, numConsts, numObjects,
                               numTryNotes, numScopeNotes, nresumeoffsets)) {
    return false;
  }
||||||| merged common ancestors
/* static */ bool
JSScript::fullyInitFromEmitter(JSContext* cx, HandleScript script, frontend::BytecodeEmitter* bce)
{
    /* The counts of indexed things must be checked during code generation. */
    MOZ_ASSERT(bce->atomIndices->count() <= INDEX_LIMIT);
    MOZ_ASSERT(bce->objectList.length <= INDEX_LIMIT);

    uint64_t nslots = bce->maxFixedSlots + static_cast<uint64_t>(bce->maxStackDepth);
    if (nslots > UINT32_MAX) {
        bce->reportError(nullptr, JSMSG_NEED_DIET, js_script_str);
        return false;
    }
=======
/* static */
bool JSScript::createPrivateScriptData(JSContext* cx, HandleScript script,
                                       uint32_t ngcthings) {
  cx->check(script);
  MOZ_ASSERT(!script->data_);
>>>>>>> upstream-releases

<<<<<<< HEAD
  script->nTypeSets_ = 0;
||||||| merged common ancestors
    uint32_t mainLength = bce->offset();
    uint32_t prologueLength = bce->prologueOffset();
    uint32_t nsrcnotes;
    if (!bce->finishTakingSrcNotes(&nsrcnotes)) {
        return false;
    }
    uint32_t natoms = bce->atomIndices->count();
    if (!partiallyInit(cx, script,
                       bce->scopeList.length(), bce->numberList.length(), bce->objectList.length,
                       bce->tryNoteList.length(), bce->scopeNoteList.length(),
                       bce->yieldAndAwaitOffsetList.length()))
    {
        return false;
    }
=======
  uint32_t dataSize;
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedScope enclosing(cx, &cx->global()->emptyGlobalScope());
  Scope* functionProtoScope = FunctionScope::create(cx, nullptr, false, false,
                                                    functionProto, enclosing);
  if (!functionProtoScope) {
    return false;
  }
||||||| merged common ancestors
    MOZ_ASSERT(script->mainOffset() == 0);
    script->mainOffset_ = prologueLength;
    script->nTypeSets_ = bce->typesetCount;
    script->lineno_ = bce->firstLine;
=======
  PrivateScriptData* data = PrivateScriptData::new_(cx, ngcthings, &dataSize);
  if (!data) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::Span<GCPtrScope> scopes = script->data_->scopes();
  scopes[0].init(functionProtoScope);
||||||| merged common ancestors
    if (!script->createScriptData(cx, prologueLength + mainLength, nsrcnotes, natoms)) {
        return false;
    }
=======
  script->data_ = data;
  script->dataSize_ = dataSize;
  AddCellMemory(script, dataSize, MemoryUse::ScriptPrivateData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t codeLength = 1;
  uint32_t noteLength = 1;
  uint32_t numAtoms = 0;
  if (!script->createSharedScriptData(cx, codeLength, noteLength, numAtoms)) {
    return false;
  }

  jsbytecode* code = script->scriptData_->code();
  code[0] = JSOP_RETRVAL;

  jssrcnote* notes = script->scriptData_->notes();
  notes[0] = SRC_NULL;

  return script->shareScriptData(cx);
}

static void InitAtomMap(frontend::AtomIndexMap& indices, GCPtrAtom* atoms) {
  for (frontend::AtomIndexMap::Range r = indices.all(); !r.empty();
       r.popFront()) {
    JSAtom* atom = r.front().key();
    uint32_t index = r.front().value();
    MOZ_ASSERT(index < indices.count());
    atoms[index].init(atom);
  }
}

/* static */ void JSScript::initFromFunctionBox(HandleScript script,
                                                frontend::FunctionBox* funbox) {
  JSFunction* fun = funbox->function();
  if (fun->isInterpretedLazy()) {
    fun->setUnlazifiedScript(script);
  } else {
    fun->setScript(script);
  }

  script->setFlag(ImmutableFlags::FunHasExtensibleScope,
                  funbox->hasExtensibleScope());
  script->setFlag(ImmutableFlags::NeedsHomeObject, funbox->needsHomeObject());
  script->setFlag(ImmutableFlags::IsDerivedClassConstructor,
                  funbox->isDerivedClassConstructor());

  if (funbox->argumentsHasLocalBinding()) {
    script->setArgumentsHasVarBinding();
    if (funbox->definitelyNeedsArgsObj()) {
      script->setNeedsArgsObj(true);
    }
  } else {
    MOZ_ASSERT(!funbox->definitelyNeedsArgsObj());
  }
  script->setFlag(ImmutableFlags::HasMappedArgsObj, funbox->hasMappedArgsObj());

  script->setFlag(ImmutableFlags::FunctionHasThisBinding,
                  funbox->hasThisBinding());
  script->setFlag(ImmutableFlags::FunctionHasExtraBodyVarScope,
                  funbox->hasExtraBodyVarScope());

  script->funLength_ = funbox->length;

  script->setFlag(ImmutableFlags::IsGenerator, funbox->isGenerator());
  script->setFlag(ImmutableFlags::IsAsync, funbox->isAsync());
  script->setFlag(ImmutableFlags::HasRest, funbox->hasRest());

  PositionalFormalParameterIter fi(script);
  while (fi && !fi.closedOver()) {
    fi++;
  }
  script->setFlag(ImmutableFlags::FunHasAnyAliasedFormal, !!fi);

  script->setFlag(ImmutableFlags::HasInnerFunctions,
                  funbox->hasInnerFunctions());
}

/* static */ void JSScript::initFromModuleContext(HandleScript script) {
  // Since modules are only run once, mark the script so that initializers
  // created within it may be given more precise types.
  script->setTreatAsRunOnce();
  MOZ_ASSERT(!script->hasRunOnce());
}

/* static */ bool JSScript::fullyInitFromEmitter(
    JSContext* cx, HandleScript script, frontend::BytecodeEmitter* bce) {
  /* The counts of indexed things must be checked during code generation. */
  MOZ_ASSERT(bce->atomIndices->count() <= INDEX_LIMIT);
  MOZ_ASSERT(bce->objectList.length <= INDEX_LIMIT);

  uint64_t nslots =
      bce->maxFixedSlots + static_cast<uint64_t>(bce->maxStackDepth);
  if (nslots > UINT32_MAX) {
    bce->reportError(nullptr, JSMSG_NEED_DIET, js_script_str);
    return false;
  }
||||||| merged common ancestors
    // Any fallible operation after JSScript::createScriptData should reset
    // JSScript.scriptData_, in order to treat this script as uncompleted,
    // in JSScript::isUncompleted.
    // JSScript::shareScriptData resets it before returning false.
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t mainLength = bce->offset();
  uint32_t prologueLength = bce->prologueOffset();
  uint32_t nsrcnotes;
  if (!bce->finishTakingSrcNotes(&nsrcnotes)) {
    return false;
  }
  uint32_t natoms = bce->atomIndices->count();
  if (!createPrivateScriptData(
          cx, script, bce->scopeList.length(), bce->numberList.length(),
          bce->objectList.length, bce->tryNoteList.length(),
          bce->scopeNoteList.length(), bce->resumeOffsetList.length())) {
    return false;
  }
||||||| merged common ancestors
    jsbytecode* code = script->code();
    PodCopy<jsbytecode>(code, bce->prologue.code.begin(), prologueLength);
    PodCopy<jsbytecode>(code + prologueLength, bce->main.code.begin(), mainLength);
    bce->copySrcNotes((jssrcnote*)(code + script->length()), nsrcnotes);
    InitAtomMap(*bce->atomIndices, script->atoms());
=======
/* static */
bool JSScript::initFunctionPrototype(JSContext* cx, HandleScript script,
                                     HandleFunction functionProto) {
  uint32_t numGCThings = 1;
  if (!createPrivateScriptData(cx, script, numGCThings)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(script->mainOffset() == 0);
  script->mainOffset_ = prologueLength;
  script->nTypeSets_ = bce->typesetCount;
  script->lineno_ = bce->firstLine;
||||||| merged common ancestors
    if (!script->shareScriptData(cx)) {
        return false;
    }
=======
  RootedScope enclosing(cx, &cx->global()->emptyGlobalScope());
  Scope* functionProtoScope = FunctionScope::create(cx, nullptr, false, false,
                                                    functionProto, enclosing);
  if (!functionProtoScope) {
    return false;
  }

  mozilla::Span<JS::GCCellPtr> gcthings = script->data_->gcthings();
  gcthings[0] = JS::GCCellPtr(functionProtoScope);

  uint32_t codeLength = 1;
  uint32_t noteLength = 3;
  uint32_t numAtoms = 0;
  uint32_t numResumeOffsets = 0;
  uint32_t numScopeNotes = 0;
  uint32_t numTryNotes = 0;
  if (!script->createSharedScriptData(cx, codeLength, noteLength, numAtoms,
                                      numResumeOffsets, numScopeNotes,
                                      numTryNotes)) {
    return false;
  }

  jsbytecode* code = script->scriptData_->code();
  code[0] = JSOP_RETRVAL;

  jssrcnote* notes = script->scriptData_->notes();
  notes[0] = SRC_NULL;
  notes[1] = SRC_NULL;
  notes[2] = SRC_NULL;

  return script->shareScriptData(cx);
}

static void InitAtomMap(frontend::AtomIndexMap& indices, GCPtrAtom* atoms) {
  for (frontend::AtomIndexMap::Range r = indices.all(); !r.empty();
       r.popFront()) {
    JSAtom* atom = r.front().key();
    uint32_t index = r.front().value();
    MOZ_ASSERT(index < indices.count());
    atoms[index].init(atom);
  }
}

static bool NeedsFunctionEnvironmentObjects(frontend::BytecodeEmitter* bce) {
  // See JSFunction::needsCallObject()
  js::Scope* bodyScope = bce->bodyScope();
  if (bodyScope->kind() == js::ScopeKind::Function) {
    if (bodyScope->hasEnvironment()) {
      return true;
    }
  }

  // See JSScript::maybeNamedLambdaScope()
  js::Scope* outerScope = bce->outermostScope();
  if (outerScope->kind() == js::ScopeKind::NamedLambda ||
      outerScope->kind() == js::ScopeKind::StrictNamedLambda) {
    MOZ_ASSERT(bce->sc->asFunctionBox()->isNamedLambda());

    if (outerScope->hasEnvironment()) {
      return true;
    }
  }

  return false;
}

void JSScript::initFromFunctionBox(frontend::FunctionBox* funbox) {
  setFlag(ImmutableFlags::FunHasExtensibleScope, funbox->hasExtensibleScope());
  setFlag(ImmutableFlags::NeedsHomeObject, funbox->needsHomeObject());
  setFlag(ImmutableFlags::IsDerivedClassConstructor,
          funbox->isDerivedClassConstructor());
  setFlag(ImmutableFlags::HasMappedArgsObj, funbox->hasMappedArgsObj());
  setFlag(ImmutableFlags::FunctionHasThisBinding, funbox->hasThisBinding());
  setFlag(ImmutableFlags::FunctionHasExtraBodyVarScope,
          funbox->hasExtraBodyVarScope());
  setFlag(ImmutableFlags::IsGenerator, funbox->isGenerator());
  setFlag(ImmutableFlags::IsAsync, funbox->isAsync());
  setFlag(ImmutableFlags::HasRest, funbox->hasRest());
  setFlag(ImmutableFlags::HasInnerFunctions, funbox->hasInnerFunctions());

  if (funbox->argumentsHasLocalBinding()) {
    setArgumentsHasVarBinding();
    if (funbox->definitelyNeedsArgsObj()) {
      setNeedsArgsObj(true);
    }
  } else {
    MOZ_ASSERT(!funbox->definitelyNeedsArgsObj());
  }
}

/* static */
bool JSScript::fullyInitFromEmitter(JSContext* cx, HandleScript script,
                                    frontend::BytecodeEmitter* bce) {
  MOZ_ASSERT(!script->data_, "JSScript already initialized");

  // If initialization fails, we must call JSScript::freeScriptData in order to
  // neuter the script. Various things that iterate raw scripts in a GC arena
  // use the presense of this data to detect if initialization is complete.
  auto scriptDataGuard =
      mozilla::MakeScopeExit([&] { script->freeScriptData(); });

  /* The counts of indexed things must be checked during code generation. */
  MOZ_ASSERT(bce->perScriptData().atomIndices()->count() <= INDEX_LIMIT);
  MOZ_ASSERT(bce->perScriptData().gcThingList().length() <= INDEX_LIMIT);

  uint64_t nslots =
      bce->maxFixedSlots +
      static_cast<uint64_t>(bce->bytecodeSection().maxStackDepth());
  if (nslots > UINT32_MAX) {
    bce->reportError(nullptr, JSMSG_NEED_DIET, js_script_str);
    return false;
  }

  // Initialize POD fields
  script->lineno_ = bce->firstLine;

  // Initialize script flags from BytecodeEmitter
  script->setFlag(ImmutableFlags::Strict, bce->sc->strict());
  script->setFlag(ImmutableFlags::BindingsAccessedDynamically,
                  bce->sc->bindingsAccessedDynamically());
  script->setFlag(ImmutableFlags::HasCallSiteObj, bce->hasCallSiteObj);
  script->setFlag(ImmutableFlags::HasSingletons, bce->hasSingletons);
  script->setFlag(ImmutableFlags::IsForEval, bce->sc->isEvalContext());
  script->setFlag(ImmutableFlags::IsModule, bce->sc->isModuleContext());
  script->setFlag(ImmutableFlags::HasNonSyntacticScope,
                  bce->outermostScope()->hasOnChain(ScopeKind::NonSyntactic));
  script->setFlag(ImmutableFlags::NeedsFunctionEnvironmentObjects,
                  NeedsFunctionEnvironmentObjects(bce));

  // Initialize script flags from FunctionBox
  if (bce->sc->isFunctionBox()) {
    script->initFromFunctionBox(bce->sc->asFunctionBox());
  }

  // Create and initialize PrivateScriptData
  if (!PrivateScriptData::InitFromEmitter(cx, script, bce)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!script->createSharedScriptData(cx, prologueLength + mainLength,
                                      nsrcnotes, natoms)) {
    return false;
  }
||||||| merged common ancestors
    if (bce->numberList.length() != 0) {
        bce->numberList.finish(script->consts());
    }
    if (bce->objectList.length != 0) {
        bce->objectList.finish(script->objects());
    }
    if (bce->scopeList.length() != 0) {
        bce->scopeList.finish(script->scopes());
    }
    if (bce->tryNoteList.length() != 0) {
        bce->tryNoteList.finish(script->trynotes());
    }
    if (bce->scopeNoteList.length() != 0) {
        bce->scopeNoteList.finish(script->scopeNotes(), prologueLength);
    }
    script->bitFields_.strict_ = bce->sc->strict();
    script->bitFields_.explicitUseStrict_ = bce->sc->hasExplicitUseStrict();
    script->bitFields_.bindingsAccessedDynamically_ = bce->sc->bindingsAccessedDynamically();
    script->bitFields_.hasSingletons_ = bce->hasSingletons;
=======
  // Create and initialize SharedScriptData
  if (!SharedScriptData::InitFromEmitter(cx, script, bce, nslots)) {
    return false;
  }
  if (!script->shareScriptData(cx)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Any fallible operation after JSScript::createSharedScriptData should
  // reset JSScript.scriptData_, in order to treat this script as
  // uncompleted, in JSScript::isUncompleted.  JSScript::shareScriptData
  // resets it before returning false.
||||||| merged common ancestors
    script->nfixed_ = bce->maxFixedSlots;
    script->nslots_ = nslots;
    script->bodyScopeIndex_ = bce->bodyScopeIndex;
    script->bitFields_.hasNonSyntacticScope_ =
        bce->outermostScope()->hasOnChain(ScopeKind::NonSyntactic);
=======
  // NOTE: JSScript is now constructed and should be linked in.
>>>>>>> upstream-releases

<<<<<<< HEAD
  jsbytecode* code = script->code();
  PodCopy<jsbytecode>(code, bce->prologue.code.begin(), prologueLength);
  PodCopy<jsbytecode>(code + prologueLength, bce->main.code.begin(),
                      mainLength);
  bce->copySrcNotes((jssrcnote*)(code + script->length()), nsrcnotes);
  InitAtomMap(*bce->atomIndices, script->atoms());
||||||| merged common ancestors
    // There shouldn't be any fallible operation after initFromFunctionBox,
    // JSFunction::hasUncompletedScript relies on the fact that the existence
    // of the pointer to JSScript means the pointed JSScript is complete.
    if (bce->sc->isFunctionBox()) {
        initFromFunctionBox(script, bce->sc->asFunctionBox());
    } else if (bce->sc->isModuleContext()) {
        initFromModuleContext(script);
    }
=======
  // Link JSFunction to this JSScript.
  if (bce->sc->isFunctionBox()) {
    JSFunction* fun = bce->sc->asFunctionBox()->function();
    if (fun->isInterpretedLazy()) {
      fun->setUnlazifiedScript(script);
    } else {
      fun->setScript(script);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!script->shareScriptData(cx)) {
    return false;
  }

  js::PrivateScriptData* data = script->data_;
  if (bce->numberList.length() != 0) {
    bce->numberList.finish(data->consts());
  }
  if (bce->objectList.length != 0) {
    bce->objectList.finish(data->objects());
  }
  if (bce->scopeList.length() != 0) {
    bce->scopeList.finish(data->scopes());
  }
  if (bce->tryNoteList.length() != 0) {
    bce->tryNoteList.finish(data->tryNotes(), prologueLength);
  }
  if (bce->scopeNoteList.length() != 0) {
    bce->scopeNoteList.finish(data->scopeNotes(), prologueLength);
  }
  if (bce->resumeOffsetList.length() != 0) {
    bce->resumeOffsetList.finish(data->resumeOffsets(), prologueLength);
  }

  script->setFlag(ImmutableFlags::Strict, bce->sc->strict());
  script->setFlag(ImmutableFlags::ExplicitUseStrict,
                  bce->sc->hasExplicitUseStrict());
  script->setFlag(ImmutableFlags::BindingsAccessedDynamically,
                  bce->sc->bindingsAccessedDynamically());
  script->setFlag(ImmutableFlags::HasSingletons, bce->hasSingletons);

  script->nfixed_ = bce->maxFixedSlots;
  script->nslots_ = nslots;
  script->bodyScopeIndex_ = bce->bodyScopeIndex;
  script->setFlag(ImmutableFlags::HasNonSyntacticScope,
                  bce->outermostScope()->hasOnChain(ScopeKind::NonSyntactic));

  // There shouldn't be any fallible operation after initFromFunctionBox,
  // JSFunction::hasUncompletedScript relies on the fact that the existence
  // of the pointer to JSScript means the pointed JSScript is complete.
  if (bce->sc->isFunctionBox()) {
    initFromFunctionBox(script, bce->sc->asFunctionBox());
  } else if (bce->sc->isModuleContext()) {
    initFromModuleContext(script);
  }

#ifdef JS_STRUCTURED_SPEW
  // We want this to happen after line number initialization to allow filtering
  // to work.
  script->setSpewEnabled(StructuredSpewer::enabled(script));
#endif
||||||| merged common ancestors
    // Copy yield offsets last, as the generator kind is set in
    // initFromFunctionBox.
    if (bce->yieldAndAwaitOffsetList.length() != 0) {
        bce->yieldAndAwaitOffsetList.finish(script->yieldAndAwaitOffsets(), prologueLength);
    }
=======
  // Part of the parse result – the scope containing each inner function – must
  // be stored in the inner function itself. Do this now that compilation is
  // complete and can no longer fail.
  bce->perScriptData().gcThingList().finishInnerFunctions();

#ifdef JS_STRUCTURED_SPEW
  // We want this to happen after line number initialization to allow filtering
  // to work.
  script->setSpewEnabled(cx->spewer().enabled(script));
#endif
>>>>>>> upstream-releases

#ifdef DEBUG
  script->assertValidJumpTargets();
#endif

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  scriptDataGuard.release();
  return true;
>>>>>>> upstream-releases
}

#ifdef DEBUG
<<<<<<< HEAD
void JSScript::assertValidJumpTargets() const {
  BytecodeLocation mainLoc = mainLocation();
  BytecodeLocation endLoc = endLocation();
  AllBytecodesIterable iter(this);
  for (BytecodeLocation loc : iter) {
    // Check jump instructions' target.
    if (loc.isJump()) {
      BytecodeLocation target = loc.getJumpTarget();
      MOZ_ASSERT(mainLoc <= target && target < endLoc);
      MOZ_ASSERT(target.isJumpTarget());

      // Check fallthrough of conditional jump instructions.
      if (loc.fallsThrough()) {
        BytecodeLocation fallthrough = loc.next();
        MOZ_ASSERT(mainLoc <= fallthrough && fallthrough < endLoc);
        MOZ_ASSERT(fallthrough.isJumpTarget());
      }
||||||| merged common ancestors
void
JSScript::assertValidJumpTargets() const
{
    jsbytecode* end = codeEnd();
    jsbytecode* mainEntry = main();
    for (jsbytecode* pc = code(); pc != end; pc = GetNextPc(pc)) {
        // Check jump instructions' target.
        if (IsJumpOpcode(JSOp(*pc))) {
            jsbytecode* target = pc + GET_JUMP_OFFSET(pc);
            MOZ_ASSERT(mainEntry <= target && target < end);
            MOZ_ASSERT(BytecodeIsJumpTarget(JSOp(*target)));

            // Check fallthrough of conditional jump instructions.
            if (BytecodeFallsThrough(JSOp(*pc))) {
                jsbytecode* fallthrough = GetNextPc(pc);
                MOZ_ASSERT(mainEntry <= fallthrough && fallthrough < end);
                MOZ_ASSERT(BytecodeIsJumpTarget(JSOp(*fallthrough)));
            }
        }

        // Check table switch case labels.
        if (JSOp(*pc) == JSOP_TABLESWITCH) {
            jsbytecode* pc2 = pc;
            int32_t len = GET_JUMP_OFFSET(pc2);

            // Default target.
            MOZ_ASSERT(mainEntry <= pc + len && pc + len < end);
            MOZ_ASSERT(BytecodeIsJumpTarget(JSOp(*(pc + len))));

            pc2 += JUMP_OFFSET_LEN;
            int32_t low = GET_JUMP_OFFSET(pc2);
            pc2 += JUMP_OFFSET_LEN;
            int32_t high = GET_JUMP_OFFSET(pc2);

            for (int i = 0; i < high - low + 1; i++) {
                pc2 += JUMP_OFFSET_LEN;
                int32_t off = (int32_t) GET_JUMP_OFFSET(pc2);
                // Case (i + low)
                MOZ_ASSERT_IF(off, mainEntry <= pc + off && pc + off < end);
                MOZ_ASSERT_IF(off, BytecodeIsJumpTarget(JSOp(*(pc + off))));
            }
        }
    }

    // Check catch/finally blocks as jump targets.
    if (hasTrynotes()) {
        for (const JSTryNote& tn : trynotes()) {
            jsbytecode* tryStart = mainEntry + tn.start;
            jsbytecode* tryPc = tryStart - 1;
            if (tn.kind != JSTRY_CATCH && tn.kind != JSTRY_FINALLY) {
                continue;
            }

            MOZ_ASSERT(JSOp(*tryPc) == JSOP_TRY);
            jsbytecode* tryTarget = tryStart + tn.length;
            MOZ_ASSERT(mainEntry <= tryTarget && tryTarget < end);
            MOZ_ASSERT(BytecodeIsJumpTarget(JSOp(*tryTarget)));
        }
=======
void JSScript::assertValidJumpTargets() const {
  BytecodeLocation mainLoc = mainLocation();
  BytecodeLocation endLoc = endLocation();
  AllBytecodesIterable iter(this);
  for (BytecodeLocation loc : iter) {
    // Check jump instructions' target.
    if (loc.isJump()) {
      BytecodeLocation target = loc.getJumpTarget();
      MOZ_ASSERT(mainLoc <= target && target < endLoc);
      MOZ_ASSERT(target.isJumpTarget());

      // Check fallthrough of conditional jump instructions.
      if (loc.fallsThrough()) {
        BytecodeLocation fallthrough = loc.next();
        MOZ_ASSERT(mainLoc <= fallthrough && fallthrough < endLoc);
        MOZ_ASSERT(fallthrough.isJumpTarget());
      }
    }

    // Check table switch case labels.
    if (loc.is(JSOP_TABLESWITCH)) {
      BytecodeLocation target = loc.getJumpTarget();

      // Default target.
      MOZ_ASSERT(mainLoc <= target && target < endLoc);
      MOZ_ASSERT(target.isJumpTarget());

      int32_t low = loc.getTableSwitchLow();
      int32_t high = loc.getTableSwitchHigh();

      for (int i = 0; i < high - low + 1; i++) {
        BytecodeLocation switchCase(this,
                                    tableSwitchCasePC(loc.toRawBytecode(), i));
        MOZ_ASSERT(mainLoc <= switchCase && switchCase < endLoc);
        MOZ_ASSERT(switchCase.isJumpTarget());
      }
    }
  }

  // Check catch/finally blocks as jump targets.
  if (hasTrynotes()) {
    for (const JSTryNote& tn : trynotes()) {
      jsbytecode* end = codeEnd();
      jsbytecode* mainEntry = main();

      jsbytecode* tryStart = offsetToPC(tn.start);
      jsbytecode* tryPc = tryStart - 1;
      if (tn.kind != JSTRY_CATCH && tn.kind != JSTRY_FINALLY) {
        continue;
      }

      MOZ_ASSERT(JSOp(*tryPc) == JSOP_TRY);
      jsbytecode* tryTarget = tryStart + tn.length;
      MOZ_ASSERT(mainEntry <= tryTarget && tryTarget < end);
      MOZ_ASSERT(BytecodeIsJumpTarget(JSOp(*tryTarget)));
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
||||||| merged common ancestors
}
#endif

size_t
JSScript::computedSizeOfData() const
{
    return dataSize();
}

size_t
JSScript::sizeOfData(mozilla::MallocSizeOf mallocSizeOf) const
{
    return mallocSizeOf(data);
}

size_t
JSScript::sizeOfTypeScript(mozilla::MallocSizeOf mallocSizeOf) const
{
    return types_ ? types_->sizeOfIncludingThis(mallocSizeOf) : 0;
}

js::GlobalObject&
JSScript::uninlinedGlobal() const
{
    return global();
}
=======
  }
}
#endif

size_t JSScript::computedSizeOfData() const { return dataSize(); }

size_t JSScript::sizeOfData(mozilla::MallocSizeOf mallocSizeOf) const {
  return mallocSizeOf(data_);
}

void JSScript::addSizeOfJitScript(mozilla::MallocSizeOf mallocSizeOf,
                                  size_t* sizeOfJitScript,
                                  size_t* sizeOfBaselineFallbackStubs) const {
  if (!jitScript_) {
    return;
  }

  jitScript_->addSizeOfIncludingThis(mallocSizeOf, sizeOfJitScript,
                                     sizeOfBaselineFallbackStubs);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Check table switch case labels.
    if (loc.is(JSOP_TABLESWITCH)) {
      BytecodeLocation target = loc.getJumpTarget();
||||||| merged common ancestors
void
JSScript::finalize(FreeOp* fop)
{
    // NOTE: this JSScript may be partially initialized at this point.  E.g. we
    // may have created it and partially initialized it with
    // JSScript::Create(), but not yet finished initializing it with
    // fullyInitFromEmitter() or fullyInitTrivial().
=======
js::GlobalObject& JSScript::uninlinedGlobal() const { return global(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // Default target.
      MOZ_ASSERT(mainLoc <= target && target < endLoc);
      MOZ_ASSERT(target.isJumpTarget());
||||||| merged common ancestors
    // Collect code coverage information for this script and all its inner
    // scripts, and store the aggregated information on the realm.
    MOZ_ASSERT_IF(hasScriptName(), fop->runtime()->lcovOutput().isEnabled());
    if (fop->runtime()->lcovOutput().isEnabled() && hasScriptName()) {
        realm()->lcovOutput.collectCodeCoverageInfo(realm(), this, getScriptName());
        destroyScriptName();
    }
=======
void JSScript::finalize(FreeOp* fop) {
  // NOTE: this JSScript may be partially initialized at this point.  E.g. we
  // may have created it and partially initialized it with
  // JSScript::Create(), but not yet finished initializing it with
  // fullyInitFromEmitter() or fullyInitTrivial().
>>>>>>> upstream-releases

<<<<<<< HEAD
      int32_t low = loc.getTableSwitchLow();
      int32_t high = loc.getTableSwitchHigh();
||||||| merged common ancestors
    fop->runtime()->geckoProfiler().onScriptFinalized(this);
=======
  // Collect code coverage information for this script and all its inner
  // scripts, and store the aggregated information on the realm.
  MOZ_ASSERT_IF(hasScriptName(), coverage::IsLCovEnabled());
  if (coverage::IsLCovEnabled() && hasScriptName()) {
    realm()->lcovOutput.collectCodeCoverageInfo(realm(), this, getScriptName());
    destroyScriptName();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      for (int i = 0; i < high - low + 1; i++) {
        BytecodeLocation switchCase(this,
                                    tableSwitchCasePC(loc.toRawBytecode(), i));
        MOZ_ASSERT(mainLoc <= switchCase && switchCase < endLoc);
        MOZ_ASSERT(switchCase.isJumpTarget());
      }
    }
  }
||||||| merged common ancestors
    if (types_) {
        types_->destroy();
    }
=======
  fop->runtime()->geckoProfiler().onScriptFinalized(this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Check catch/finally blocks as jump targets.
  if (hasTrynotes()) {
    for (const JSTryNote& tn : trynotes()) {
      jsbytecode* end = codeEnd();
      jsbytecode* mainEntry = main();
||||||| merged common ancestors
    jit::DestroyJitScripts(fop, this);
=======
  jit::DestroyJitScripts(fop, this);
>>>>>>> upstream-releases

<<<<<<< HEAD
      jsbytecode* tryStart = offsetToPC(tn.start);
      jsbytecode* tryPc = tryStart - 1;
      if (tn.kind != JSTRY_CATCH && tn.kind != JSTRY_FINALLY) {
        continue;
      }
||||||| merged common ancestors
    destroyScriptCounts();
    destroyDebugScript(fop);
=======
  destroyScriptCounts();
  destroyDebugScript(fop);
>>>>>>> upstream-releases

<<<<<<< HEAD
      MOZ_ASSERT(JSOp(*tryPc) == JSOP_TRY);
      jsbytecode* tryTarget = tryStart + tn.length;
      MOZ_ASSERT(mainEntry <= tryTarget && tryTarget < end);
      MOZ_ASSERT(BytecodeIsJumpTarget(JSOp(*tryTarget)));
    }
  }
}
#endif
||||||| merged common ancestors
    if (data) {
        JS_POISON(data, 0xdb, computedSizeOfData(), MemCheckKind::MakeNoAccess);
        fop->free_(data);
    }
=======
#ifdef MOZ_VTUNE
  if (realm()->scriptVTuneIdMap) {
    // Note: we should only get here if the VTune JIT profiler is running.
    realm()->scriptVTuneIdMap->remove(this);
  }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
size_t JSScript::computedSizeOfData() const { return dataSize(); }
||||||| merged common ancestors
    if (scriptData_) {
        scriptData_->decRefCount();
    }
=======
  if (data_) {
    size_t size = computedSizeOfData();
    AlwaysPoison(data_, JS_POISONED_JSSCRIPT_DATA_PATTERN, size,
                 MemCheckKind::MakeNoAccess);
    fop->free_(this, data_, size, MemoryUse::ScriptPrivateData);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
size_t JSScript::sizeOfData(mozilla::MallocSizeOf mallocSizeOf) const {
  return mallocSizeOf(data_);
||||||| merged common ancestors
    // In most cases, our LazyScript's script pointer will reference this
    // script, and thus be nulled out by normal weakref processing. However, if
    // we unlazified the LazyScript during incremental sweeping, it will have a
    // completely different JSScript.
    MOZ_ASSERT_IF(lazyScript && !IsAboutToBeFinalizedUnbarriered(&lazyScript),
                  !lazyScript->hasScript() || lazyScript->maybeScriptUnbarriered() != this);
=======
  freeScriptData();

  // In most cases, our LazyScript's script pointer will reference this
  // script, and thus be nulled out by normal weakref processing. However, if
  // we unlazified the LazyScript during incremental sweeping, it will have a
  // completely different JSScript.
  MOZ_ASSERT_IF(
      lazyScript && !IsAboutToBeFinalizedUnbarriered(&lazyScript),
      !lazyScript->hasScript() || lazyScript->maybeScriptUnbarriered() != this);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
size_t JSScript::sizeOfTypeScript(mozilla::MallocSizeOf mallocSizeOf) const {
  return types_ ? types_->sizeOfIncludingThis(mallocSizeOf) : 0;
||||||| merged common ancestors
static const uint32_t GSN_CACHE_THRESHOLD = 100;

void
GSNCache::purge()
{
    code = nullptr;
    map.clearAndCompact();
=======
static const uint32_t GSN_CACHE_THRESHOLD = 100;

void GSNCache::purge() {
  code = nullptr;
  map.clearAndCompact();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
js::GlobalObject& JSScript::uninlinedGlobal() const { return global(); }

void JSScript::finalize(FreeOp* fop) {
  // NOTE: this JSScript may be partially initialized at this point.  E.g. we
  // may have created it and partially initialized it with
  // JSScript::Create(), but not yet finished initializing it with
  // fullyInitFromEmitter() or fullyInitTrivial().

  // Collect code coverage information for this script and all its inner
  // scripts, and store the aggregated information on the realm.
  MOZ_ASSERT_IF(hasScriptName(), fop->runtime()->lcovOutput().isEnabled());
  if (fop->runtime()->lcovOutput().isEnabled() && hasScriptName()) {
    realm()->lcovOutput.collectCodeCoverageInfo(realm(), this, getScriptName());
    destroyScriptName();
  }

  fop->runtime()->geckoProfiler().onScriptFinalized(this);
||||||| merged common ancestors
jssrcnote*
js::GetSrcNote(GSNCache& cache, JSScript* script, jsbytecode* pc)
{
    size_t target = pc - script->code();
    if (target >= script->length()) {
        return nullptr;
    }

    if (cache.code == script->code()) {
        GSNCache::Map::Ptr p = cache.map.lookup(pc);
        return p ? p->value() : nullptr;
    }

    size_t offset = 0;
    jssrcnote* result;
    for (jssrcnote* sn = script->notes(); ; sn = SN_NEXT(sn)) {
        if (SN_IS_TERMINATOR(sn)) {
            result = nullptr;
            break;
        }
        offset += SN_DELTA(sn);
        if (offset == target && SN_IS_GETTABLE(sn)) {
            result = sn;
            break;
        }
    }

    if (cache.code != script->code() && script->length() >= GSN_CACHE_THRESHOLD) {
        unsigned nsrcnotes = 0;
        for (jssrcnote* sn = script->notes(); !SN_IS_TERMINATOR(sn); sn = SN_NEXT(sn)) {
            if (SN_IS_GETTABLE(sn)) {
                ++nsrcnotes;
            }
        }
        if (cache.code) {
            cache.map.clear();
            cache.code = nullptr;
        }
        if (cache.map.reserve(nsrcnotes)) {
            pc = script->code();
            for (jssrcnote* sn = script->notes(); !SN_IS_TERMINATOR(sn);
                 sn = SN_NEXT(sn))
            {
                pc += SN_DELTA(sn);
                if (SN_IS_GETTABLE(sn)) {
                    cache.map.putNewInfallible(pc, sn);
                }
            }
            cache.code = script->code();
        }
    }
=======
jssrcnote* js::GetSrcNote(GSNCache& cache, JSScript* script, jsbytecode* pc) {
  size_t target = pc - script->code();
  if (target >= script->length()) {
    return nullptr;
  }

  if (cache.code == script->code()) {
    GSNCache::Map::Ptr p = cache.map.lookup(pc);
    return p ? p->value() : nullptr;
  }

  size_t offset = 0;
  jssrcnote* result;
  for (jssrcnote* sn = script->notes();; sn = SN_NEXT(sn)) {
    if (SN_IS_TERMINATOR(sn)) {
      result = nullptr;
      break;
    }
    offset += SN_DELTA(sn);
    if (offset == target && SN_IS_GETTABLE(sn)) {
      result = sn;
      break;
    }
  }

  if (cache.code != script->code() && script->length() >= GSN_CACHE_THRESHOLD) {
    unsigned nsrcnotes = 0;
    for (jssrcnote* sn = script->notes(); !SN_IS_TERMINATOR(sn);
         sn = SN_NEXT(sn)) {
      if (SN_IS_GETTABLE(sn)) {
        ++nsrcnotes;
      }
    }
    if (cache.code) {
      cache.map.clear();
      cache.code = nullptr;
    }
    if (cache.map.reserve(nsrcnotes)) {
      pc = script->code();
      for (jssrcnote* sn = script->notes(); !SN_IS_TERMINATOR(sn);
           sn = SN_NEXT(sn)) {
        pc += SN_DELTA(sn);
        if (SN_IS_GETTABLE(sn)) {
          cache.map.putNewInfallible(pc, sn);
        }
      }
      cache.code = script->code();
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (types_) {
    types_->destroy(zone());
  }
||||||| merged common ancestors
    return result;
}
=======
  return result;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  jit::DestroyJitScripts(fop, this);
||||||| merged common ancestors
jssrcnote*
js::GetSrcNote(JSContext* cx, JSScript* script, jsbytecode* pc)
{
    return GetSrcNote(cx->caches().gsnCache, script, pc);
}
=======
jssrcnote* js::GetSrcNote(JSContext* cx, JSScript* script, jsbytecode* pc) {
  return GetSrcNote(cx->caches().gsnCache, script, pc);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  destroyScriptCounts();
  destroyDebugScript(fop);

#ifdef MOZ_VTUNE
  if (realm()->scriptVTuneIdMap) {
    // Note: we should only get here if the VTune JIT profiler is running.
    realm()->scriptVTuneIdMap->remove(this);
  }
#endif
||||||| merged common ancestors
unsigned
js::PCToLineNumber(unsigned startLine, jssrcnote* notes, jsbytecode* code, jsbytecode* pc,
                   unsigned* columnp)
{
    unsigned lineno = startLine;
    unsigned column = 0;

    /*
     * Walk through source notes accumulating their deltas, keeping track of
     * line-number notes, until we pass the note for pc's offset within
     * script->code.
     */
    ptrdiff_t offset = 0;
    ptrdiff_t target = pc - code;
    for (jssrcnote* sn = notes; !SN_IS_TERMINATOR(sn); sn = SN_NEXT(sn)) {
        offset += SN_DELTA(sn);
        if (offset > target) {
            break;
        }
=======
unsigned js::PCToLineNumber(unsigned startLine, jssrcnote* notes,
                            jsbytecode* code, jsbytecode* pc,
                            unsigned* columnp) {
  unsigned lineno = startLine;
  unsigned column = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (data_) {
    JS_POISON(data_, 0xdb, computedSizeOfData(), MemCheckKind::MakeNoAccess);
    fop->free_(data_);
  }
||||||| merged common ancestors
        SrcNoteType type = SN_TYPE(sn);
        if (type == SRC_SETLINE) {
            lineno = unsigned(GetSrcNoteOffset(sn, SrcNote::SetLine::Line));
            column = 0;
        } else if (type == SRC_NEWLINE) {
            lineno++;
            column = 0;
        } else if (type == SRC_COLSPAN) {
            ptrdiff_t colspan = SN_OFFSET_TO_COLSPAN(GetSrcNoteOffset(sn, SrcNote::ColSpan::Span));
            MOZ_ASSERT(ptrdiff_t(column) + colspan >= 0);
            column += colspan;
        }
    }
=======
  /*
   * Walk through source notes accumulating their deltas, keeping track of
   * line-number notes, until we pass the note for pc's offset within
   * script->code.
   */
  ptrdiff_t offset = 0;
  ptrdiff_t target = pc - code;
  for (jssrcnote* sn = notes; !SN_IS_TERMINATOR(sn); sn = SN_NEXT(sn)) {
    offset += SN_DELTA(sn);
    if (offset > target) {
      break;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (scriptData_) {
    scriptData_->decRefCount();
  }
||||||| merged common ancestors
    if (columnp) {
        *columnp = column;
    }
=======
    SrcNoteType type = SN_TYPE(sn);
    if (type == SRC_SETLINE) {
      lineno = unsigned(GetSrcNoteOffset(sn, SrcNote::SetLine::Line));
      column = 0;
    } else if (type == SRC_NEWLINE) {
      lineno++;
      column = 0;
    } else if (type == SRC_COLSPAN) {
      ptrdiff_t colspan =
          SN_OFFSET_TO_COLSPAN(GetSrcNoteOffset(sn, SrcNote::ColSpan::Span));
      MOZ_ASSERT(ptrdiff_t(column) + colspan >= 0);
      column += colspan;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // In most cases, our LazyScript's script pointer will reference this
  // script, and thus be nulled out by normal weakref processing. However, if
  // we unlazified the LazyScript during incremental sweeping, it will have a
  // completely different JSScript.
  MOZ_ASSERT_IF(
      lazyScript && !IsAboutToBeFinalizedUnbarriered(&lazyScript),
      !lazyScript->hasScript() || lazyScript->maybeScriptUnbarriered() != this);
||||||| merged common ancestors
    return lineno;
=======
  if (columnp) {
    *columnp = column;
  }

  return lineno;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static const uint32_t GSN_CACHE_THRESHOLD = 100;
||||||| merged common ancestors
unsigned
js::PCToLineNumber(JSScript* script, jsbytecode* pc, unsigned* columnp)
{
    /* Cope with InterpreterFrame.pc value prior to entering Interpret. */
    if (!pc) {
        return 0;
    }
=======
unsigned js::PCToLineNumber(JSScript* script, jsbytecode* pc,
                            unsigned* columnp) {
  /* Cope with InterpreterFrame.pc value prior to entering Interpret. */
  if (!pc) {
    return 0;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void GSNCache::purge() {
  code = nullptr;
  map.clearAndCompact();
||||||| merged common ancestors
    return PCToLineNumber(script->lineno(), script->notes(), script->code(), pc, columnp);
=======
  return PCToLineNumber(script->lineno(), script->notes(), script->code(), pc,
                        columnp);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
jssrcnote* js::GetSrcNote(GSNCache& cache, JSScript* script, jsbytecode* pc) {
  size_t target = pc - script->code();
  if (target >= script->length()) {
    return nullptr;
  }

  if (cache.code == script->code()) {
    GSNCache::Map::Ptr p = cache.map.lookup(pc);
    return p ? p->value() : nullptr;
  }

  size_t offset = 0;
  jssrcnote* result;
  for (jssrcnote* sn = script->notes();; sn = SN_NEXT(sn)) {
    if (SN_IS_TERMINATOR(sn)) {
      result = nullptr;
      break;
    }
    offset += SN_DELTA(sn);
    if (offset == target && SN_IS_GETTABLE(sn)) {
      result = sn;
      break;
    }
  }

  if (cache.code != script->code() && script->length() >= GSN_CACHE_THRESHOLD) {
    unsigned nsrcnotes = 0;
    for (jssrcnote* sn = script->notes(); !SN_IS_TERMINATOR(sn);
         sn = SN_NEXT(sn)) {
      if (SN_IS_GETTABLE(sn)) {
        ++nsrcnotes;
      }
    }
    if (cache.code) {
      cache.map.clear();
      cache.code = nullptr;
||||||| merged common ancestors
jsbytecode*
js::LineNumberToPC(JSScript* script, unsigned target)
{
    ptrdiff_t offset = 0;
    ptrdiff_t best = -1;
    unsigned lineno = script->lineno();
    unsigned bestdiff = SN_MAX_OFFSET;
    for (jssrcnote* sn = script->notes(); !SN_IS_TERMINATOR(sn); sn = SN_NEXT(sn)) {
        /*
         * Exact-match only if offset is not in the prologue; otherwise use
         * nearest greater-or-equal line number match.
         */
        if (lineno == target && offset >= ptrdiff_t(script->mainOffset())) {
            goto out;
        }
        if (lineno >= target) {
            unsigned diff = lineno - target;
            if (diff < bestdiff) {
                bestdiff = diff;
                best = offset;
            }
        }
        offset += SN_DELTA(sn);
        SrcNoteType type = SN_TYPE(sn);
        if (type == SRC_SETLINE) {
            lineno = unsigned(GetSrcNoteOffset(sn, SrcNote::SetLine::Line));
        } else if (type == SRC_NEWLINE) {
            lineno++;
        }
    }
    if (best >= 0) {
        offset = best;
=======
jsbytecode* js::LineNumberToPC(JSScript* script, unsigned target) {
  ptrdiff_t offset = 0;
  ptrdiff_t best = -1;
  unsigned lineno = script->lineno();
  unsigned bestdiff = SN_MAX_OFFSET;
  for (jssrcnote* sn = script->notes(); !SN_IS_TERMINATOR(sn);
       sn = SN_NEXT(sn)) {
    /*
     * Exact-match only if offset is not in the prologue; otherwise use
     * nearest greater-or-equal line number match.
     */
    if (lineno == target && offset >= ptrdiff_t(script->mainOffset())) {
      goto out;
    }
    if (lineno >= target) {
      unsigned diff = lineno - target;
      if (diff < bestdiff) {
        bestdiff = diff;
        best = offset;
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (cache.map.reserve(nsrcnotes)) {
      pc = script->code();
      for (jssrcnote* sn = script->notes(); !SN_IS_TERMINATOR(sn);
           sn = SN_NEXT(sn)) {
        pc += SN_DELTA(sn);
        if (SN_IS_GETTABLE(sn)) {
          cache.map.putNewInfallible(pc, sn);
        }
      }
      cache.code = script->code();
    }
  }

  return result;
}

jssrcnote* js::GetSrcNote(JSContext* cx, JSScript* script, jsbytecode* pc) {
  return GetSrcNote(cx->caches().gsnCache, script, pc);
||||||| merged common ancestors
out:
    return script->offsetToPC(offset);
}

JS_FRIEND_API(unsigned)
js::GetScriptLineExtent(JSScript* script)
{
    unsigned lineno = script->lineno();
    unsigned maxLineNo = lineno;
    for (jssrcnote* sn = script->notes(); !SN_IS_TERMINATOR(sn); sn = SN_NEXT(sn)) {
        SrcNoteType type = SN_TYPE(sn);
        if (type == SRC_SETLINE) {
            lineno = unsigned(GetSrcNoteOffset(sn, SrcNote::SetLine::Line));
        } else if (type == SRC_NEWLINE) {
            lineno++;
        }

        if (maxLineNo < lineno) {
            maxLineNo = lineno;
        }
    }

    return 1 + maxLineNo - script->lineno();
}

void
js::DescribeScriptedCallerForDirectEval(JSContext* cx, HandleScript script, jsbytecode* pc,
                                        const char** file, unsigned* linenop, uint32_t* pcOffset,
                                        bool* mutedErrors)
{
    MOZ_ASSERT(script->containsPC(pc));

    static_assert(JSOP_SPREADEVAL_LENGTH == JSOP_STRICTSPREADEVAL_LENGTH,
                  "next op after a spread must be at consistent offset");
    static_assert(JSOP_EVAL_LENGTH == JSOP_STRICTEVAL_LENGTH,
                  "next op after a direct eval must be at consistent offset");

    MOZ_ASSERT(JSOp(*pc) == JSOP_EVAL || JSOp(*pc) == JSOP_STRICTEVAL ||
               JSOp(*pc) == JSOP_SPREADEVAL || JSOp(*pc) == JSOP_STRICTSPREADEVAL);

    bool isSpread = (JSOp(*pc) == JSOP_SPREADEVAL ||
                     JSOp(*pc) == JSOP_STRICTSPREADEVAL);
    jsbytecode* nextpc = pc + (isSpread ? JSOP_SPREADEVAL_LENGTH : JSOP_EVAL_LENGTH);
    MOZ_ASSERT(*nextpc == JSOP_LINENO);

    *file = script->filename();
    *linenop = GET_UINT32(nextpc);
    *pcOffset = script->pcToOffset(pc);
    *mutedErrors = script->mutedErrors();
=======
    offset += SN_DELTA(sn);
    SrcNoteType type = SN_TYPE(sn);
    if (type == SRC_SETLINE) {
      lineno = unsigned(GetSrcNoteOffset(sn, SrcNote::SetLine::Line));
    } else if (type == SRC_NEWLINE) {
      lineno++;
    }
  }
  if (best >= 0) {
    offset = best;
  }
out:
  return script->offsetToPC(offset);
}

JS_FRIEND_API unsigned js::GetScriptLineExtent(JSScript* script) {
  unsigned lineno = script->lineno();
  unsigned maxLineNo = lineno;
  for (jssrcnote* sn = script->notes(); !SN_IS_TERMINATOR(sn);
       sn = SN_NEXT(sn)) {
    SrcNoteType type = SN_TYPE(sn);
    if (type == SRC_SETLINE) {
      lineno = unsigned(GetSrcNoteOffset(sn, SrcNote::SetLine::Line));
    } else if (type == SRC_NEWLINE) {
      lineno++;
    }

    if (maxLineNo < lineno) {
      maxLineNo = lineno;
    }
  }

  return 1 + maxLineNo - script->lineno();
}

void js::DescribeScriptedCallerForDirectEval(JSContext* cx, HandleScript script,
                                             jsbytecode* pc, const char** file,
                                             unsigned* linenop,
                                             uint32_t* pcOffset,
                                             bool* mutedErrors) {
  MOZ_ASSERT(script->containsPC(pc));

  static_assert(JSOP_SPREADEVAL_LENGTH == JSOP_STRICTSPREADEVAL_LENGTH,
                "next op after a spread must be at consistent offset");
  static_assert(JSOP_EVAL_LENGTH == JSOP_STRICTEVAL_LENGTH,
                "next op after a direct eval must be at consistent offset");

  MOZ_ASSERT(JSOp(*pc) == JSOP_EVAL || JSOp(*pc) == JSOP_STRICTEVAL ||
             JSOp(*pc) == JSOP_SPREADEVAL ||
             JSOp(*pc) == JSOP_STRICTSPREADEVAL);

  bool isSpread =
      (JSOp(*pc) == JSOP_SPREADEVAL || JSOp(*pc) == JSOP_STRICTSPREADEVAL);
  jsbytecode* nextpc =
      pc + (isSpread ? JSOP_SPREADEVAL_LENGTH : JSOP_EVAL_LENGTH);
  MOZ_ASSERT(*nextpc == JSOP_LINENO);

  *file = script->filename();
  *linenop = GET_UINT32(nextpc);
  *pcOffset = script->pcToOffset(pc);
  *mutedErrors = script->mutedErrors();
}

void js::DescribeScriptedCallerForCompilation(
    JSContext* cx, MutableHandleScript maybeScript, const char** file,
    unsigned* linenop, uint32_t* pcOffset, bool* mutedErrors) {
  NonBuiltinFrameIter iter(cx, cx->realm()->principals());

  if (iter.done()) {
    maybeScript.set(nullptr);
    *file = nullptr;
    *linenop = 0;
    *pcOffset = 0;
    *mutedErrors = false;
    return;
  }

  *file = iter.filename();
  *linenop = iter.computeLine();
  *mutedErrors = iter.mutedErrors();

  // These values are only used for introducer fields which are debugging
  // information and can be safely left null for wasm frames.
  if (iter.hasScript()) {
    maybeScript.set(iter.script());
    *pcOffset = iter.pc() - maybeScript->code();
  } else {
    maybeScript.set(nullptr);
    *pcOffset = 0;
  }
}

static JSObject* CloneInnerInterpretedFunction(
    JSContext* cx, HandleScope enclosingScope, HandleFunction srcFun,
    Handle<ScriptSourceObject*> sourceObject) {
  /* NB: Keep this in sync with XDRInterpretedFunction. */
  RootedObject cloneProto(cx);
  if (!GetFunctionPrototype(cx, srcFun->generatorKind(), srcFun->asyncKind(),
                            &cloneProto)) {
    return nullptr;
  }

  gc::AllocKind allocKind = srcFun->getAllocKind();
  uint16_t flags = srcFun->flags();
  if (srcFun->isSelfHostedBuiltin()) {
    // Functions in the self-hosting compartment are only extended in
    // debug mode. For top-level functions, FUNCTION_EXTENDED gets used by
    // the cloning algorithm. Do the same for inner functions here.
    allocKind = gc::AllocKind::FUNCTION_EXTENDED;
    flags |= JSFunction::Flags::EXTENDED;
  }
  RootedAtom atom(cx, srcFun->displayAtom());
  if (atom) {
    cx->markAtom(atom);
  }
  RootedFunction clone(
      cx, NewFunctionWithProto(cx, nullptr, srcFun->nargs(),
                               JSFunction::Flags(flags), nullptr, atom,
                               cloneProto, allocKind, TenuredObject));
  if (!clone) {
    return nullptr;
  }

  JSScript::AutoDelazify srcScript(cx, srcFun);
  if (!srcScript) {
    return nullptr;
  }
  JSScript* cloneScript = CloneScriptIntoFunction(cx, enclosingScope, clone,
                                                  srcScript, sourceObject);
  if (!cloneScript) {
    return nullptr;
  }

  if (!JSFunction::setTypeForScriptedFunction(cx, clone)) {
    return nullptr;
  }

  return clone;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
unsigned js::PCToLineNumber(unsigned startLine, jssrcnote* notes,
                            jsbytecode* code, jsbytecode* pc,
                            unsigned* columnp) {
  unsigned lineno = startLine;
  unsigned column = 0;
||||||| merged common ancestors
void
js::DescribeScriptedCallerForCompilation(JSContext* cx, MutableHandleScript maybeScript,
                                         const char** file, unsigned* linenop, uint32_t* pcOffset,
                                         bool* mutedErrors)
{
    NonBuiltinFrameIter iter(cx, cx->realm()->principals());
=======
static JSObject* CloneScriptObject(JSContext* cx, PrivateScriptData* srcData,
                                   HandleObject obj,
                                   Handle<ScriptSourceObject*> sourceObject,
                                   JS::HandleVector<StackGCCellPtr> gcThings) {
  if (obj->is<RegExpObject>()) {
    return CloneScriptRegExpObject(cx, obj->as<RegExpObject>());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /*
   * Walk through source notes accumulating their deltas, keeping track of
   * line-number notes, until we pass the note for pc's offset within
   * script->code.
   */
  ptrdiff_t offset = 0;
  ptrdiff_t target = pc - code;
  for (jssrcnote* sn = notes; !SN_IS_TERMINATOR(sn); sn = SN_NEXT(sn)) {
    offset += SN_DELTA(sn);
    if (offset > target) {
      break;
||||||| merged common ancestors
    if (iter.done()) {
        maybeScript.set(nullptr);
        *file = nullptr;
        *linenop = 0;
        *pcOffset = 0;
        *mutedErrors = false;
        return;
=======
  if (obj->is<JSFunction>()) {
    HandleFunction innerFun = obj.as<JSFunction>();
    if (innerFun->isNative()) {
      if (cx->realm() != innerFun->realm()) {
        MOZ_ASSERT(innerFun->isAsmJSNative());
        JS_ReportErrorASCII(cx, "AsmJS modules do not yet support cloning.");
        return nullptr;
      }
      return innerFun;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    SrcNoteType type = SN_TYPE(sn);
    if (type == SRC_SETLINE) {
      lineno = unsigned(GetSrcNoteOffset(sn, SrcNote::SetLine::Line));
      column = 0;
    } else if (type == SRC_NEWLINE) {
      lineno++;
      column = 0;
    } else if (type == SRC_COLSPAN) {
      ptrdiff_t colspan =
          SN_OFFSET_TO_COLSPAN(GetSrcNoteOffset(sn, SrcNote::ColSpan::Span));
      MOZ_ASSERT(ptrdiff_t(column) + colspan >= 0);
      column += colspan;
||||||| merged common ancestors
    *file = iter.filename();
    *linenop = iter.computeLine();
    *mutedErrors = iter.mutedErrors();

    // These values are only used for introducer fields which are debugging
    // information and can be safely left null for wasm frames.
    if (iter.hasScript()) {
        maybeScript.set(iter.script());
        *pcOffset = iter.pc() - maybeScript->code();
    } else {
        maybeScript.set(nullptr);
        *pcOffset = 0;
=======
    if (innerFun->isInterpretedLazy()) {
      AutoRealm ar(cx, innerFun);
      if (!JSFunction::getOrCreateScript(cx, innerFun)) {
        return nullptr;
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

  if (columnp) {
    *columnp = column;
  }
||||||| merged common ancestors
}
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
  return lineno;
}

unsigned js::PCToLineNumber(JSScript* script, jsbytecode* pc,
                            unsigned* columnp) {
  /* Cope with InterpreterFrame.pc value prior to entering Interpret. */
  if (!pc) {
    return 0;
  }
||||||| merged common ancestors
template <class T>
static inline T*
Rebase(JSScript* dst, JSScript* src, T* srcp)
{
    size_t off = reinterpret_cast<uint8_t*>(srcp) - src->data;
    return reinterpret_cast<T*>(dst->data + off);
}

static JSObject*
CloneInnerInterpretedFunction(JSContext* cx, HandleScope enclosingScope, HandleFunction srcFun)
{
    /* NB: Keep this in sync with XDRInterpretedFunction. */
    RootedObject cloneProto(cx);
    if (srcFun->isGenerator() || srcFun->isAsync()) {
        cloneProto = GlobalObject::getOrCreateGeneratorFunctionPrototype(cx, cx->global());
        if (!cloneProto) {
            return nullptr;
        }
    }
=======
    Scope* enclosing = innerFun->nonLazyScript()->enclosingScope();
    uint32_t scopeIndex = FindScopeIndex(srcData->gcthings(), *enclosing);
    RootedScope enclosingClone(cx,
                               &gcThings[scopeIndex].get().get().as<Scope>());
    return CloneInnerInterpretedFunction(cx, enclosingClone, innerFun,
                                         sourceObject);
  }

  return DeepCloneObjectLiteral(cx, obj, TenuredObject);
}

/* static */
bool PrivateScriptData::Clone(JSContext* cx, HandleScript src, HandleScript dst,
                              MutableHandle<GCVector<Scope*>> scopes) {
  PrivateScriptData* srcData = src->data_;
  uint32_t ngcthings = srcData->gcthings().size();

  // Clone GC things.
  JS::RootedVector<StackGCCellPtr> gcThings(cx);
  size_t scopeIndex = 0;
  Rooted<ScriptSourceObject*> sourceObject(cx, dst->sourceObject());
  RootedObject obj(cx);
  RootedScope scope(cx);
  RootedScope enclosingScope(cx);
  RootedBigInt bigint(cx);
  for (JS::GCCellPtr gcThing : srcData->gcthings()) {
    if (gcThing.is<JSObject>()) {
      obj = &gcThing.as<JSObject>();
      JSObject* clone =
          CloneScriptObject(cx, srcData, obj, sourceObject, gcThings);
      if (!clone || !gcThings.append(JS::GCCellPtr(clone))) {
        return false;
      }
    } else if (gcThing.is<Scope>()) {
      // The passed in scopes vector contains body scopes that needed to be
      // cloned especially, depending on whether the script is a function or
      // global scope. Clone all other scopes.
      if (scopeIndex < scopes.length()) {
        if (!gcThings.append(JS::GCCellPtr(scopes[scopeIndex].get()))) {
          return false;
        }
      } else {
        scope = &gcThing.as<Scope>();
        uint32_t enclosingScopeIndex =
            FindScopeIndex(srcData->gcthings(), *scope->enclosing());
        enclosingScope = &gcThings[enclosingScopeIndex].get().get().as<Scope>();
        Scope* clone = Scope::clone(cx, scope, enclosingScope);
        if (!clone || !gcThings.append(JS::GCCellPtr(clone))) {
          return false;
        }
      }
      scopeIndex++;
    } else {
      bigint = &gcThing.as<BigInt>();
      BigInt* clone = bigint;
      if (cx->zone() != bigint->zone()) {
        clone = BigInt::copy(cx, bigint);
        if (!clone) {
          return false;
        }
      }
      if (!gcThings.append(JS::GCCellPtr(clone))) {
        return false;
      }
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return PCToLineNumber(script->lineno(), script->notes(), script->code(), pc,
                        columnp);
}

jsbytecode* js::LineNumberToPC(JSScript* script, unsigned target) {
  ptrdiff_t offset = 0;
  ptrdiff_t best = -1;
  unsigned lineno = script->lineno();
  unsigned bestdiff = SN_MAX_OFFSET;
  for (jssrcnote* sn = script->notes(); !SN_IS_TERMINATOR(sn);
       sn = SN_NEXT(sn)) {
    /*
     * Exact-match only if offset is not in the prologue; otherwise use
     * nearest greater-or-equal line number match.
     */
    if (lineno == target && offset >= ptrdiff_t(script->mainOffset())) {
      goto out;
    }
    if (lineno >= target) {
      unsigned diff = lineno - target;
      if (diff < bestdiff) {
        bestdiff = diff;
        best = offset;
      }
    }
    offset += SN_DELTA(sn);
    SrcNoteType type = SN_TYPE(sn);
    if (type == SRC_SETLINE) {
      lineno = unsigned(GetSrcNoteOffset(sn, SrcNote::SetLine::Line));
    } else if (type == SRC_NEWLINE) {
      lineno++;
    }
  }
  if (best >= 0) {
    offset = best;
  }
out:
  return script->offsetToPC(offset);
}

JS_FRIEND_API unsigned js::GetScriptLineExtent(JSScript* script) {
  unsigned lineno = script->lineno();
  unsigned maxLineNo = lineno;
  for (jssrcnote* sn = script->notes(); !SN_IS_TERMINATOR(sn);
       sn = SN_NEXT(sn)) {
    SrcNoteType type = SN_TYPE(sn);
    if (type == SRC_SETLINE) {
      lineno = unsigned(GetSrcNoteOffset(sn, SrcNote::SetLine::Line));
    } else if (type == SRC_NEWLINE) {
      lineno++;
    }

    if (maxLineNo < lineno) {
      maxLineNo = lineno;
    }
  }

  return 1 + maxLineNo - script->lineno();
}

void js::DescribeScriptedCallerForDirectEval(JSContext* cx, HandleScript script,
                                             jsbytecode* pc, const char** file,
                                             unsigned* linenop,
                                             uint32_t* pcOffset,
                                             bool* mutedErrors) {
  MOZ_ASSERT(script->containsPC(pc));

  static_assert(JSOP_SPREADEVAL_LENGTH == JSOP_STRICTSPREADEVAL_LENGTH,
                "next op after a spread must be at consistent offset");
  static_assert(JSOP_EVAL_LENGTH == JSOP_STRICTEVAL_LENGTH,
                "next op after a direct eval must be at consistent offset");

  MOZ_ASSERT(JSOp(*pc) == JSOP_EVAL || JSOp(*pc) == JSOP_STRICTEVAL ||
             JSOp(*pc) == JSOP_SPREADEVAL ||
             JSOp(*pc) == JSOP_STRICTSPREADEVAL);

  bool isSpread =
      (JSOp(*pc) == JSOP_SPREADEVAL || JSOp(*pc) == JSOP_STRICTSPREADEVAL);
  jsbytecode* nextpc =
      pc + (isSpread ? JSOP_SPREADEVAL_LENGTH : JSOP_EVAL_LENGTH);
  MOZ_ASSERT(*nextpc == JSOP_LINENO);

  *file = script->filename();
  *linenop = GET_UINT32(nextpc);
  *pcOffset = script->pcToOffset(pc);
  *mutedErrors = script->mutedErrors();
}

void js::DescribeScriptedCallerForCompilation(
    JSContext* cx, MutableHandleScript maybeScript, const char** file,
    unsigned* linenop, uint32_t* pcOffset, bool* mutedErrors) {
  NonBuiltinFrameIter iter(cx, cx->realm()->principals());

  if (iter.done()) {
    maybeScript.set(nullptr);
    *file = nullptr;
    *linenop = 0;
    *pcOffset = 0;
    *mutedErrors = false;
    return;
  }

  *file = iter.filename();
  *linenop = iter.computeLine();
  *mutedErrors = iter.mutedErrors();

  // These values are only used for introducer fields which are debugging
  // information and can be safely left null for wasm frames.
  if (iter.hasScript()) {
    maybeScript.set(iter.script());
    *pcOffset = iter.pc() - maybeScript->code();
  } else {
    maybeScript.set(nullptr);
    *pcOffset = 0;
  }
}

static JSObject* CloneInnerInterpretedFunction(JSContext* cx,
                                               HandleScope enclosingScope,
                                               HandleFunction srcFun) {
  /* NB: Keep this in sync with XDRInterpretedFunction. */
  RootedObject cloneProto(cx);
  if (srcFun->isGenerator() || srcFun->isAsync()) {
    cloneProto =
        GlobalObject::getOrCreateGeneratorFunctionPrototype(cx, cx->global());
    if (!cloneProto) {
      return nullptr;
    }
  }

  gc::AllocKind allocKind = srcFun->getAllocKind();
  uint16_t flags = srcFun->flags();
  if (srcFun->isSelfHostedBuiltin()) {
    // Functions in the self-hosting compartment are only extended in
    // debug mode. For top-level functions, FUNCTION_EXTENDED gets used by
    // the cloning algorithm. Do the same for inner functions here.
    allocKind = gc::AllocKind::FUNCTION_EXTENDED;
    flags |= JSFunction::Flags::EXTENDED;
  }
  RootedAtom atom(cx, srcFun->displayAtom());
  if (atom) {
    cx->markAtom(atom);
  }
  RootedFunction clone(
      cx, NewFunctionWithProto(cx, nullptr, srcFun->nargs(),
                               JSFunction::Flags(flags), nullptr, atom,
                               cloneProto, allocKind, TenuredObject));
  if (!clone) {
    return nullptr;
  }

  JSScript::AutoDelazify srcScript(cx, srcFun);
  if (!srcScript) {
    return nullptr;
  }
  JSScript* cloneScript =
      CloneScriptIntoFunction(cx, enclosingScope, clone, srcScript);
  if (!cloneScript) {
    return nullptr;
  }
||||||| merged common ancestors
    gc::AllocKind allocKind = srcFun->getAllocKind();
    uint16_t flags = srcFun->flags();
    if (srcFun->isSelfHostedBuiltin()) {
        // Functions in the self-hosting compartment are only extended in
        // debug mode. For top-level functions, FUNCTION_EXTENDED gets used by
        // the cloning algorithm. Do the same for inner functions here.
        allocKind = gc::AllocKind::FUNCTION_EXTENDED;
        flags |= JSFunction::Flags::EXTENDED;
    }
    RootedAtom atom(cx, srcFun->displayAtom());
    if (atom) {
        cx->markAtom(atom);
    }
    RootedFunction clone(cx, NewFunctionWithProto(cx, nullptr, srcFun->nargs(),
                                                  JSFunction::Flags(flags), nullptr, atom,
                                                  cloneProto, allocKind, TenuredObject));
    if (!clone) {
        return nullptr;
    }

    JSScript::AutoDelazify srcScript(cx, srcFun);
    if (!srcScript) {
        return nullptr;
    }
    JSScript* cloneScript = CloneScriptIntoFunction(cx, enclosingScope, clone, srcScript);
    if (!cloneScript) {
        return nullptr;
    }
=======
  // Create the new PrivateScriptData on |dst| and fill it in.
  if (!JSScript::createPrivateScriptData(cx, dst, ngcthings)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!JSFunction::setTypeForScriptedFunction(cx, clone)) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!JSFunction::setTypeForScriptedFunction(cx, clone)) {
        return nullptr;
    }
=======
  PrivateScriptData* dstData = dst->data_;
  {
    auto array = dstData->gcthings();
    for (uint32_t i = 0; i < ngcthings; ++i) {
      array[i] = gcThings[i].get().get();
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return clone;
||||||| merged common ancestors
    return clone;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool js::detail::CopyScript(JSContext* cx, HandleScript src, HandleScript dst,
                            MutableHandle<GCVector<Scope*>> scopes) {
  // We don't copy the HideScriptFromDebugger flag and it's not clear what
  // should happen if it's set on the source script.
  MOZ_ASSERT(!src->hideScriptFromDebugger());

  if (src->treatAsRunOnce() && !src->functionNonDelazifying()) {
    JS_ReportErrorASCII(cx, "No cloning toplevel run-once scripts");
    return false;
  }

  /* NB: Keep this in sync with XDRScript. */
||||||| merged common ancestors
bool
js::detail::CopyScript(JSContext* cx, HandleScript src, HandleScript dst,
                       MutableHandle<GCVector<Scope*>> scopes)
{
    if (src->treatAsRunOnce() && !src->functionNonDelazifying()) {
        JS_ReportErrorASCII(cx, "No cloning toplevel run-once scripts");
        return false;
    }
=======
JSScript* js::detail::CopyScript(JSContext* cx, HandleScript src,
                                 HandleScriptSourceObject sourceObject,
                                 MutableHandle<GCVector<Scope*>> scopes) {
  // We don't copy the HideScriptFromDebugger flag and it's not clear what
  // should happen if it's set on the source script.
  MOZ_ASSERT(!src->hideScriptFromDebugger());
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Some embeddings are not careful to use ExposeObjectToActiveJS as needed. */
  MOZ_ASSERT(!src->sourceObject()->isMarkedGray());
||||||| merged common ancestors
    /* NB: Keep this in sync with XDRScript. */
=======
  if (src->treatAsRunOnce() && !src->functionNonDelazifying()) {
    JS_ReportErrorASCII(cx, "No cloning toplevel run-once scripts");
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t nscopes = src->scopes().size();
  uint32_t nconsts = src->hasConsts() ? src->consts().size() : 0;
  uint32_t nobjects = src->hasObjects() ? src->objects().size() : 0;
||||||| merged common ancestors
    /* Some embeddings are not careful to use ExposeObjectToActiveJS as needed. */
    MOZ_ASSERT(!src->sourceObject()->isMarkedGray());
=======
  /* NB: Keep this in sync with XDRScript. */
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Script data */
||||||| merged common ancestors
    uint32_t nconsts = src->hasConsts() ? src->consts().size() : 0;
    uint32_t nobjects = src->hasObjects() ? src->objects().size() : 0;
    uint32_t nscopes = src->scopes().size();
    uint32_t ntrynotes = src->hasTrynotes() ? src->trynotes().size() : 0;
    uint32_t nscopenotes = src->hasScopeNotes() ? src->scopeNotes().size() : 0;
    uint32_t nyieldoffsets = src->hasYieldAndAwaitOffsets() ? src->yieldAndAwaitOffsets().size() : 0;
=======
  // Some embeddings are not careful to use ExposeObjectToActiveJS as needed.
  JS::AssertObjectIsNotGray(sourceObject);
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t size = src->dataSize();
  UniquePtr<uint8_t, JS::FreePolicy> data(AllocScriptData(cx, size));
  if (!data) {
    return false;
  }

  /* Scopes */
||||||| merged common ancestors
    /* Script data */
=======
  CompileOptions options(cx);
  options.setMutedErrors(src->mutedErrors())
      .setSelfHostingMode(src->selfHosted())
      .setNoScriptRval(src->noScriptRval());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The passed in scopes vector contains body scopes that needed to be
  // cloned especially, depending on whether the script is a function or
  // global scope. Starting at scopes.length() means we only deal with
  // intra-body scopes.
  {
    MOZ_ASSERT(nscopes != 0);
    MOZ_ASSERT(src->bodyScopeIndex() + 1 == scopes.length());
    RootedScope original(cx);
    RootedScope clone(cx);
    for (const GCPtrScope& elem : src->scopes().From(scopes.length())) {
      original = elem.get();
      clone = Scope::clone(cx, original,
                           scopes[FindScopeIndex(src, *original->enclosing())]);
      if (!clone || !scopes.append(clone)) {
        return false;
      }
    }
  }
||||||| merged common ancestors
    size_t size = src->dataSize();
    UniquePtr<uint8_t, JS::FreePolicy> data(AllocScriptData(cx, size));
    if (size && !data) {
        return false;
    }
=======
  // Create a new JSScript to fill in
  RootedScript dst(
      cx, JSScript::Create(cx, options, sourceObject, src->sourceStart(),
                           src->sourceEnd(), src->toStringStart(),
                           src->toStringEnd()));
  if (!dst) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Constants */

  AutoValueVector consts(cx);
  if (nconsts != 0) {
    RootedValue val(cx);
    RootedValue clone(cx);
    for (const GCPtrValue& elem : src->consts()) {
      val = elem.get();
      if (val.isDouble()) {
        clone = val;
      }
#ifdef ENABLE_BIGINT
      else if (val.isBigInt()) {
        if (cx->zone() == val.toBigInt()->zone()) {
          clone.setBigInt(val.toBigInt());
        } else {
          RootedBigInt b(cx, val.toBigInt());
          BigInt* copy = BigInt::copy(cx, b);
          if (!copy) {
            return false;
          }
          clone.setBigInt(copy);
        }
      }
#endif
      else {
        MOZ_ASSERT_UNREACHABLE("bad script consts() element");
      }
||||||| merged common ancestors
    /* Scopes */

    // The passed in scopes vector contains body scopes that needed to be
    // cloned especially, depending on whether the script is a function or
    // global scope. Starting at scopes.length() means we only deal with
    // intra-body scopes.
    {
        MOZ_ASSERT(nscopes != 0);
        MOZ_ASSERT(src->bodyScopeIndex() + 1 == scopes.length());
        RootedScope original(cx);
        RootedScope clone(cx);
        for (const GCPtrScope& elem : src->scopes().From(scopes.length())) {
            original = elem.get();
            clone = Scope::clone(cx, original, scopes[FindScopeIndex(src, *original->enclosing())]);
            if (!clone || !scopes.append(clone)) {
                return false;
            }
        }
    }
=======
  // Copy POD fields
  dst->lineno_ = src->lineno();
  dst->column_ = src->column();
  dst->immutableFlags_ = src->immutableFlags_;
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (!consts.append(clone)) {
        return false;
      }
    }
  }

  /* Objects */

  AutoObjectVector objects(cx);
  if (nobjects != 0) {
    RootedObject obj(cx);
    RootedObject clone(cx);
    for (const GCPtrObject& elem : src->objects()) {
      obj = elem.get();
      clone = nullptr;
      if (obj->is<RegExpObject>()) {
        clone = CloneScriptRegExpObject(cx, obj->as<RegExpObject>());
      } else if (obj->is<JSFunction>()) {
        RootedFunction innerFun(cx, &obj->as<JSFunction>());
        if (innerFun->isNative()) {
          if (cx->compartment() != innerFun->compartment()) {
            MOZ_ASSERT(innerFun->isAsmJSNative());
            JS_ReportErrorASCII(cx,
                                "AsmJS modules do not yet support cloning.");
            return false;
          }
          clone = innerFun;
        } else {
          if (innerFun->isInterpretedLazy()) {
            AutoRealm ar(cx, innerFun);
            if (!JSFunction::getOrCreateScript(cx, innerFun)) {
              return false;
            }
          }

          Scope* enclosing = innerFun->nonLazyScript()->enclosingScope();
          RootedScope enclosingClone(cx,
                                     scopes[FindScopeIndex(src, *enclosing)]);
          clone = CloneInnerInterpretedFunction(cx, enclosingClone, innerFun);
        }
      } else {
        clone = DeepCloneObjectLiteral(cx, obj, TenuredObject);
      }
||||||| merged common ancestors
    /* Objects */

    AutoObjectVector objects(cx);
    if (nobjects != 0) {
        RootedObject obj(cx);
        RootedObject clone(cx);
        for (const GCPtrObject& elem : src->objects()) {
            obj = elem.get();
            clone = nullptr;
            if (obj->is<RegExpObject>()) {
                clone = CloneScriptRegExpObject(cx, obj->as<RegExpObject>());
            } else if (obj->is<JSFunction>()) {
                RootedFunction innerFun(cx, &obj->as<JSFunction>());
                if (innerFun->isNative()) {
                    if (cx->compartment() != innerFun->compartment()) {
                        MOZ_ASSERT(innerFun->isAsmJSNative());
                        JS_ReportErrorASCII(cx, "AsmJS modules do not yet support cloning.");
                        return false;
                    }
                    clone = innerFun;
                } else {
                    if (innerFun->isInterpretedLazy()) {
                        AutoRealm ar(cx, innerFun);
                        if (!JSFunction::getOrCreateScript(cx, innerFun)) {
                            return false;
                        }
                    }

                    Scope* enclosing = innerFun->nonLazyScript()->enclosingScope();
                    RootedScope enclosingClone(cx, scopes[FindScopeIndex(src, *enclosing)]);
                    clone = CloneInnerInterpretedFunction(cx, enclosingClone, innerFun);
                }
            } else {
                clone = DeepCloneObjectLiteral(cx, obj, TenuredObject);
            }

            if (!clone || !objects.append(clone)) {
                return false;
            }
        }
    }
=======
  dst->setFlag(JSScript::ImmutableFlags::HasNonSyntacticScope,
               scopes[0]->hasOnChain(ScopeKind::NonSyntactic));
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (!clone || !objects.append(clone)) {
        return false;
      }
    }
  }
||||||| merged common ancestors
    /* This assignment must occur before all the Rebase calls. */
    dst->data = data.release();
    dst->dataSize_ = size;
    MOZ_ASSERT(bool(dst->data) == bool(src->data));
    if (dst->data) {
        memcpy(dst->data, src->data, size);
    }
=======
  if (src->argumentsHasVarBinding()) {
    dst->setArgumentsHasVarBinding();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  dst->data_ = reinterpret_cast<js::PrivateScriptData*>(data.release());
  dst->dataSize_ = size;
  memcpy(dst->data_, src->data_, size);
||||||| merged common ancestors
    if (cx->zone() != src->zoneFromAnyThread()) {
        for (size_t i = 0; i < src->scriptData()->natoms(); i++) {
            cx->markAtom(src->scriptData()->atoms()[i]);
        }
    }
=======
  // Clone the PrivateScriptData into dst
  if (!PrivateScriptData::Clone(cx, src, dst, scopes)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (cx->zone() != src->zoneFromAnyThread()) {
    for (size_t i = 0; i < src->scriptData()->natoms(); i++) {
      cx->markAtom(src->scriptData()->atoms()[i]);
    }
  }

  /* Script filenames, bytecodes and atoms are runtime-wide. */
  dst->setScriptData(src->scriptData());

  dst->lineno_ = src->lineno();
  dst->mainOffset_ = src->mainOffset();
  dst->nfixed_ = src->nfixed();
  dst->nslots_ = src->nslots();
  dst->bodyScopeIndex_ = src->bodyScopeIndex_;
  dst->funLength_ = src->funLength();
  dst->nTypeSets_ = src->nTypeSets();

  dst->immutableFlags_ = src->immutableFlags_;
  dst->setFlag(JSScript::ImmutableFlags::HasNonSyntacticScope,
               scopes[0]->hasOnChain(ScopeKind::NonSyntactic));

  if (src->argumentsHasVarBinding()) {
    dst->setArgumentsHasVarBinding();
    if (src->analyzedArgsUsage()) {
      dst->setNeedsArgsObj(src->needsArgsObj());
    }
  }

  {
    auto array = dst->data_->scopes();
    for (uint32_t i = 0; i < nscopes; ++i) {
      array[i].init(scopes[i]);
    }
  }
  if (nconsts) {
    auto array = dst->data_->consts();
    for (unsigned i = 0; i < nconsts; ++i) {
      array[i].init(consts[i]);
    }
  }
  if (nobjects) {
    auto array = dst->data_->objects();
    for (unsigned i = 0; i < nobjects; ++i) {
      array[i].init(objects[i]);
    }
  }
||||||| merged common ancestors
    /* Script filenames, bytecodes and atoms are runtime-wide. */
    dst->setScriptData(src->scriptData());

    dst->lineno_ = src->lineno();
    dst->mainOffset_ = src->mainOffset();
    dst->nfixed_ = src->nfixed();
    dst->nslots_ = src->nslots();
    dst->bodyScopeIndex_ = src->bodyScopeIndex_;
    dst->funLength_ = src->funLength();
    dst->nTypeSets_ = src->nTypeSets();
    if (src->argumentsHasVarBinding()) {
        dst->setArgumentsHasVarBinding();
        if (src->analyzedArgsUsage()) {
            dst->setNeedsArgsObj(src->needsArgsObj());
        }
    }
    dst->bitFields_.hasMappedArgsObj_ = src->hasMappedArgsObj();
    dst->bitFields_.functionHasThisBinding_ = src->functionHasThisBinding();
    dst->bitFields_.functionHasExtraBodyVarScope_ = src->functionHasExtraBodyVarScope();
    dst->cloneHasArray(src);
    dst->bitFields_.strict_ = src->strict();
    dst->bitFields_.explicitUseStrict_ = src->explicitUseStrict();
    dst->bitFields_.hasNonSyntacticScope_ = scopes[0]->hasOnChain(ScopeKind::NonSyntactic);
    dst->bitFields_.bindingsAccessedDynamically_ = src->bindingsAccessedDynamically();
    dst->bitFields_.funHasExtensibleScope_ = src->funHasExtensibleScope();
    dst->bitFields_.funHasAnyAliasedFormal_ = src->funHasAnyAliasedFormal();
    dst->bitFields_.hasSingletons_ = src->hasSingletons();
    dst->bitFields_.treatAsRunOnce_ = src->treatAsRunOnce();
    dst->bitFields_.hasInnerFunctions_ = src->hasInnerFunctions();
    dst->setGeneratorKind(src->generatorKind());
    dst->bitFields_.isDerivedClassConstructor_ = src->isDerivedClassConstructor();
    dst->bitFields_.needsHomeObject_ = src->needsHomeObject();
    dst->bitFields_.isDefaultClassConstructor_ = src->isDefaultClassConstructor();
    dst->bitFields_.isAsync_ = src->bitFields_.isAsync_;
    dst->bitFields_.hasRest_ = src->bitFields_.hasRest_;
    dst->bitFields_.hideScriptFromDebugger_ = src->bitFields_.hideScriptFromDebugger_;

    if (nconsts != 0) {
        GCPtrValue* vector = Rebase<GCPtrValue>(dst, src, src->constsRaw()->vector);
        dst->constsRaw()->vector = vector;
        for (unsigned i = 0; i < nconsts; ++i) {
            MOZ_ASSERT_IF(vector[i].isGCThing(), vector[i].toString()->isAtom());
        }
    }
    if (nobjects != 0) {
        GCPtrObject* vector = Rebase<GCPtrObject>(dst, src, src->objectsRaw()->vector);
        dst->objectsRaw()->vector = vector;
        for (unsigned i = 0; i < nobjects; ++i) {
            vector[i].init(&objects[i]->as<NativeObject>());
        }
    }
    {
        GCPtrScope* vector = Rebase<GCPtrScope>(dst, src, src->scopesRaw()->vector);
        dst->scopesRaw()->vector = vector;
        for (uint32_t i = 0; i < nscopes; ++i) {
            vector[i].init(scopes[i]);
        }
    }
    if (ntrynotes != 0) {
        dst->trynotesRaw()->vector = Rebase<JSTryNote>(dst, src, src->trynotesRaw()->vector);
    }
    if (nscopenotes != 0) {
        dst->scopeNotesRaw()->vector = Rebase<ScopeNote>(dst, src, src->scopeNotesRaw()->vector);
    }
    if (nyieldoffsets != 0) {
        dst->yieldAndAwaitOffsetsRaw().vector_ =
            Rebase<uint32_t>(dst, src, src->yieldAndAwaitOffsetsRaw().vector_);
    }
=======
  // The SharedScriptData can be reused by any zone in the Runtime as long as
  // we make sure to mark first (to sync Atom pointers).
  if (cx->zone() != src->zoneFromAnyThread()) {
    src->scriptData()->markForCrossZone(cx);
  }
  dst->scriptData_ = src->scriptData();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  return dst;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static JSScript* CreateEmptyScriptForClone(JSContext* cx, HandleScript src) {
  /*
   * Wrap the script source object as needed. Self-hosted scripts may be
   * in another runtime, so lazily create a new script source object to
   * use for them.
   */
  RootedObject sourceObject(cx);
  if (src->realm()->isSelfHostingRealm()) {
    if (!cx->realm()->selfHostingScriptSource) {
      CompileOptions options(cx);
      FillSelfHostingCompileOptions(options);

      ScriptSourceObject* obj = frontend::CreateScriptSourceObject(cx, options);
      if (!obj) {
        return nullptr;
      }
      cx->realm()->selfHostingScriptSource.set(obj);
    }
    sourceObject = cx->realm()->selfHostingScriptSource;
  } else {
    sourceObject = src->sourceObject();
    if (!cx->compartment()->wrap(cx, &sourceObject)) {
      return nullptr;
||||||| merged common ancestors
static JSScript*
CreateEmptyScriptForClone(JSContext* cx, HandleScript src)
{
    /*
     * Wrap the script source object as needed. Self-hosted scripts may be
     * in another runtime, so lazily create a new script source object to
     * use for them.
     */
    RootedObject sourceObject(cx);
    if (src->realm()->isSelfHostingRealm()) {
        if (!cx->realm()->selfHostingScriptSource) {
            CompileOptions options(cx);
            FillSelfHostingCompileOptions(options);

            ScriptSourceObject* obj = frontend::CreateScriptSourceObject(cx, options);
            if (!obj) {
                return nullptr;
            }
            cx->realm()->selfHostingScriptSource.set(obj);
        }
        sourceObject = cx->realm()->selfHostingScriptSource;
    } else {
        sourceObject = src->sourceObject();
        if (!cx->compartment()->wrap(cx, &sourceObject)) {
            return nullptr;
        }
=======
JSScript* js::CloneGlobalScript(JSContext* cx, ScopeKind scopeKind,
                                HandleScript src) {
  MOZ_ASSERT(scopeKind == ScopeKind::Global ||
             scopeKind == ScopeKind::NonSyntactic);

  Rooted<ScriptSourceObject*> sourceObject(cx, src->sourceObject());
  if (cx->compartment() != sourceObject->compartment()) {
    sourceObject = ScriptSourceObject::clone(cx, sourceObject);
    if (!sourceObject) {
      return nullptr;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  CompileOptions options(cx);
  options.setMutedErrors(src->mutedErrors())
      .setSelfHostingMode(src->selfHosted())
      .setNoScriptRval(src->noScriptRval());
||||||| merged common ancestors
    CompileOptions options(cx);
    options.setMutedErrors(src->mutedErrors())
           .setSelfHostingMode(src->selfHosted())
           .setNoScriptRval(src->noScriptRval());
=======
  MOZ_ASSERT(src->bodyScopeIndex() == 0);
  Rooted<GCVector<Scope*>> scopes(cx, GCVector<Scope*>(cx));
  Rooted<GlobalScope*> original(cx, &src->bodyScope()->as<GlobalScope>());
  GlobalScope* clone = GlobalScope::clone(cx, original, scopeKind);
  if (!clone || !scopes.append(clone)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return JSScript::Create(cx, options, sourceObject, src->sourceStart(),
                          src->sourceEnd(), src->toStringStart(),
                          src->toStringEnd());
||||||| merged common ancestors
    return JSScript::Create(cx, options, sourceObject, src->sourceStart(), src->sourceEnd(),
                            src->toStringStart(), src->toStringEnd());
=======
  return detail::CopyScript(cx, src, sourceObject, &scopes);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSScript* js::CloneGlobalScript(JSContext* cx, ScopeKind scopeKind,
                                HandleScript src) {
  MOZ_ASSERT(scopeKind == ScopeKind::Global ||
             scopeKind == ScopeKind::NonSyntactic);
||||||| merged common ancestors
JSScript*
js::CloneGlobalScript(JSContext* cx, ScopeKind scopeKind, HandleScript src)
{
    MOZ_ASSERT(scopeKind == ScopeKind::Global || scopeKind == ScopeKind::NonSyntactic);
=======
JSScript* js::CloneScriptIntoFunction(
    JSContext* cx, HandleScope enclosingScope, HandleFunction fun,
    HandleScript src, Handle<ScriptSourceObject*> sourceObject) {
  MOZ_ASSERT(fun->isInterpreted());
  MOZ_ASSERT(!fun->hasScript() || fun->hasUncompletedScript());
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedScript dst(cx, CreateEmptyScriptForClone(cx, src));
  if (!dst) {
    return nullptr;
  }
||||||| merged common ancestors
    RootedScript dst(cx, CreateEmptyScriptForClone(cx, src));
    if (!dst) {
        return nullptr;
    }
=======
  // Clone the non-intra-body scopes.
  Rooted<GCVector<Scope*>> scopes(cx, GCVector<Scope*>(cx));
  RootedScope original(cx);
  RootedScope enclosingClone(cx);
  for (uint32_t i = 0; i <= src->bodyScopeIndex(); i++) {
    original = src->getScope(i);

    if (i == 0) {
      enclosingClone = enclosingScope;
    } else {
      MOZ_ASSERT(src->getScope(i - 1) == original->enclosing());
      enclosingClone = scopes[i - 1];
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(src->bodyScopeIndex() == 0);
  Rooted<GCVector<Scope*>> scopes(cx, GCVector<Scope*>(cx));
  Rooted<GlobalScope*> original(cx, &src->bodyScope()->as<GlobalScope>());
  GlobalScope* clone = GlobalScope::clone(cx, original, scopeKind);
  if (!clone || !scopes.append(clone)) {
    return nullptr;
  }
||||||| merged common ancestors
    MOZ_ASSERT(src->bodyScopeIndex() == 0);
    Rooted<GCVector<Scope*>> scopes(cx, GCVector<Scope*>(cx));
    Rooted<GlobalScope*> original(cx, &src->bodyScope()->as<GlobalScope>());
    GlobalScope* clone = GlobalScope::clone(cx, original, scopeKind);
    if (!clone || !scopes.append(clone)) {
        return nullptr;
    }
=======
    Scope* clone;
    if (original->is<FunctionScope>()) {
      clone = FunctionScope::clone(cx, original.as<FunctionScope>(), fun,
                                   enclosingClone);
    } else {
      clone = Scope::clone(cx, original, enclosingClone);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!detail::CopyScript(cx, src, dst, &scopes)) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!detail::CopyScript(cx, src, dst, &scopes)) {
        return nullptr;
    }
=======
    if (!clone || !scopes.append(clone)) {
      return nullptr;
    }
  }

  // Save flags in case we need to undo the early mutations.
  const int preservedFlags = fun->flags();
  RootedScript dst(cx, detail::CopyScript(cx, src, sourceObject, &scopes));
  if (!dst) {
    fun->setFlags(preservedFlags);
    return nullptr;
  }

  // Finally set the script after all the fallible operations.
  if (fun->isInterpretedLazy()) {
    fun->setUnlazifiedScript(dst);
  } else {
    fun->initScript(dst);
  }
>>>>>>> upstream-releases

  return dst;
}

<<<<<<< HEAD
JSScript* js::CloneScriptIntoFunction(JSContext* cx, HandleScope enclosingScope,
                                      HandleFunction fun, HandleScript src) {
  MOZ_ASSERT(fun->isInterpreted());
  MOZ_ASSERT(!fun->hasScript() || fun->hasUncompletedScript());
||||||| merged common ancestors
JSScript*
js::CloneScriptIntoFunction(JSContext* cx, HandleScope enclosingScope, HandleFunction fun,
                            HandleScript src)
{
    MOZ_ASSERT(fun->isInterpreted());
    MOZ_ASSERT(!fun->hasScript() || fun->hasUncompletedScript());

    RootedScript dst(cx, CreateEmptyScriptForClone(cx, src));
    if (!dst) {
        return nullptr;
    }

    // Clone the non-intra-body scopes.
    Rooted<GCVector<Scope*>> scopes(cx, GCVector<Scope*>(cx));
    RootedScope original(cx);
    RootedScope enclosingClone(cx);
    for (uint32_t i = 0; i <= src->bodyScopeIndex(); i++) {
        original = src->getScope(i);
=======
DebugScript* JSScript::debugScript() {
  MOZ_ASSERT(hasDebugScript());
  DebugScriptMap* map = realm()->debugScriptMap.get();
  MOZ_ASSERT(map);
  DebugScriptMap::Ptr p = map->lookup(this);
  MOZ_ASSERT(p);
  return p->value().get();
}

DebugScript* JSScript::releaseDebugScript() {
  MOZ_ASSERT(hasDebugScript());
  DebugScriptMap* map = realm()->debugScriptMap.get();
  MOZ_ASSERT(map);
  DebugScriptMap::Ptr p = map->lookup(this);
  MOZ_ASSERT(p);
  DebugScript* debug = p->value().release();
  map->remove(p);
  clearFlag(MutableFlags::HasDebugScript);
  return debug;
}

void JSScript::destroyDebugScript(FreeOp* fop) {
  if (hasDebugScript()) {
#ifdef DEBUG
    for (jsbytecode* pc = code(); pc < codeEnd(); pc++) {
      MOZ_ASSERT(!getBreakpointSite(pc));
    }
#endif
    freeDebugScript(fop);
  }
}

void JSScript::freeDebugScript(FreeOp* fop) {
  MOZ_ASSERT(hasDebugScript());
  fop->free_(this, releaseDebugScript(), DebugScript::allocSize(length()),
             MemoryUse::ScriptDebugScript);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedScript dst(cx, CreateEmptyScriptForClone(cx, src));
  if (!dst) {
    return nullptr;
  }
||||||| merged common ancestors
        if (i == 0) {
            enclosingClone = enclosingScope;
        } else {
            MOZ_ASSERT(src->getScope(i - 1) == original->enclosing());
            enclosingClone = scopes[i - 1];
        }
=======
DebugScript* JSScript::getOrCreateDebugScript(JSContext* cx) {
  if (hasDebugScript()) {
    return debugScript();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Clone the non-intra-body scopes.
  Rooted<GCVector<Scope*>> scopes(cx, GCVector<Scope*>(cx));
  RootedScope original(cx);
  RootedScope enclosingClone(cx);
  for (uint32_t i = 0; i <= src->bodyScopeIndex(); i++) {
    original = src->getScope(i);
||||||| merged common ancestors
        Scope* clone;
        if (original->is<FunctionScope>()) {
            clone = FunctionScope::clone(cx, original.as<FunctionScope>(), fun, enclosingClone);
        } else {
            clone = Scope::clone(cx, original, enclosingClone);
        }
=======
  size_t nbytes = DebugScript::allocSize(length());
  UniqueDebugScript debug(
      reinterpret_cast<DebugScript*>(cx->pod_calloc<uint8_t>(nbytes)));
  if (!debug) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (i == 0) {
      enclosingClone = enclosingScope;
    } else {
      MOZ_ASSERT(src->getScope(i - 1) == original->enclosing());
      enclosingClone = scopes[i - 1];
||||||| merged common ancestors
        if (!clone || !scopes.append(clone)) {
            return nullptr;
        }
=======
  /* Create realm's debugScriptMap if necessary. */
  if (!realm()->debugScriptMap) {
    auto map = cx->make_unique<DebugScriptMap>();
    if (!map) {
      return nullptr;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    Scope* clone;
    if (original->is<FunctionScope>()) {
      clone = FunctionScope::clone(cx, original.as<FunctionScope>(), fun,
                                   enclosingClone);
    } else {
      clone = Scope::clone(cx, original, enclosingClone);
    }
||||||| merged common ancestors
    // Save flags in case we need to undo the early mutations.
    const int preservedFlags = fun->flags();
    if (!detail::CopyScript(cx, src, dst, &scopes)) {
        fun->setFlags(preservedFlags);
        return nullptr;
    }
=======
    realm()->debugScriptMap = std::move(map);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!clone || !scopes.append(clone)) {
      return nullptr;
    }
  }

  // Save flags in case we need to undo the early mutations.
  const int preservedFlags = fun->flags();
  if (!detail::CopyScript(cx, src, dst, &scopes)) {
    fun->setFlags(preservedFlags);
    return nullptr;
  }

  // Finally set the script after all the fallible operations.
  if (fun->isInterpretedLazy()) {
    fun->setUnlazifiedScript(dst);
  } else {
    fun->initScript(dst);
  }
||||||| merged common ancestors
    // Finally set the script after all the fallible operations.
    if (fun->isInterpretedLazy()) {
        fun->setUnlazifiedScript(dst);
    } else {
        fun->initScript(dst);
    }
=======
  DebugScript* borrowed = debug.get();
  if (!realm()->debugScriptMap->putNew(this, std::move(debug))) {
    ReportOutOfMemory(cx);
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return dst;
}
||||||| merged common ancestors
    return dst;
}
=======
  setFlag(MutableFlags::HasDebugScript);  // safe to set this;  we can't fail
                                          // after this point
  AddCellMemory(this, nbytes, MemoryUse::ScriptDebugScript);
>>>>>>> upstream-releases

<<<<<<< HEAD
DebugScript* JSScript::debugScript() {
  MOZ_ASSERT(hasDebugScript());
  DebugScriptMap* map = realm()->debugScriptMap.get();
  MOZ_ASSERT(map);
  DebugScriptMap::Ptr p = map->lookup(this);
  MOZ_ASSERT(p);
  return p->value().get();
}
||||||| merged common ancestors
DebugScript*
JSScript::debugScript()
{
    MOZ_ASSERT(bitFields_.hasDebugScript_);
    DebugScriptMap* map = realm()->debugScriptMap.get();
    MOZ_ASSERT(map);
    DebugScriptMap::Ptr p = map->lookup(this);
    MOZ_ASSERT(p);
    return p->value().get();
}
=======
  /*
   * Ensure that any Interpret() instances running on this script have
   * interrupts enabled. The interrupts must stay enabled until the
   * debug state is destroyed.
   */
  for (ActivationIterator iter(cx); !iter.done(); ++iter) {
    if (iter->isInterpreter()) {
      iter->asInterpreter()->enableInterruptsIfRunning(this);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
DebugScript* JSScript::releaseDebugScript() {
  MOZ_ASSERT(hasDebugScript());
  DebugScriptMap* map = realm()->debugScriptMap.get();
  MOZ_ASSERT(map);
  DebugScriptMap::Ptr p = map->lookup(this);
  MOZ_ASSERT(p);
  DebugScript* debug = p->value().release();
  map->remove(p);
  clearFlag(MutableFlags::HasDebugScript);
  return debug;
||||||| merged common ancestors
DebugScript*
JSScript::releaseDebugScript()
{
    MOZ_ASSERT(bitFields_.hasDebugScript_);
    DebugScriptMap* map = realm()->debugScriptMap.get();
    MOZ_ASSERT(map);
    DebugScriptMap::Ptr p = map->lookup(this);
    MOZ_ASSERT(p);
    DebugScript* debug = p->value().release();
    map->remove(p);
    bitFields_.hasDebugScript_ = false;
    return debug;
=======
  return borrowed;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void JSScript::destroyDebugScript(FreeOp* fop) {
  if (hasDebugScript()) {
#ifdef DEBUG
    for (jsbytecode* pc = code(); pc < codeEnd(); pc++) {
      if (BreakpointSite* site = getBreakpointSite(pc)) {
        /* Breakpoints are swept before finalization. */
        MOZ_ASSERT(site->firstBreakpoint() == nullptr);
        MOZ_ASSERT(getBreakpointSite(pc) == nullptr);
      }
    }
#endif
    fop->free_(releaseDebugScript());
  }
}
||||||| merged common ancestors
void
JSScript::destroyDebugScript(FreeOp* fop)
{
    if (bitFields_.hasDebugScript_) {
#ifdef DEBUG
        for (jsbytecode* pc = code(); pc < codeEnd(); pc++) {
            if (BreakpointSite* site = getBreakpointSite(pc)) {
                /* Breakpoints are swept before finalization. */
                MOZ_ASSERT(site->firstBreakpoint() == nullptr);
                MOZ_ASSERT(getBreakpointSite(pc) == nullptr);
            }
        }
#endif
        fop->free_(releaseDebugScript());
    }
}
=======
bool JSScript::incrementGeneratorObserverCount(JSContext* cx) {
  cx->check(this);
  MOZ_ASSERT(cx->realm()->isDebuggee());
>>>>>>> upstream-releases

<<<<<<< HEAD
bool JSScript::ensureHasDebugScript(JSContext* cx) {
  if (hasDebugScript()) {
    return true;
  }
||||||| merged common ancestors
bool
JSScript::ensureHasDebugScript(JSContext* cx)
{
    if (bitFields_.hasDebugScript_) {
        return true;
    }

    size_t nbytes = offsetof(DebugScript, breakpoints) + length() * sizeof(BreakpointSite*);
    UniqueDebugScript debug(reinterpret_cast<DebugScript*>(cx->pod_calloc<uint8_t>(nbytes)));
    if (!debug) {
        return false;
    }
=======
  AutoRealm ar(cx, this);

  DebugScript* debug = getOrCreateDebugScript(cx);
  if (!debug) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t nbytes =
      offsetof(DebugScript, breakpoints) + length() * sizeof(BreakpointSite*);
  UniqueDebugScript debug(
      reinterpret_cast<DebugScript*>(cx->pod_calloc<uint8_t>(nbytes)));
  if (!debug) {
    return false;
  }
||||||| merged common ancestors
    /* Create realm's debugScriptMap if necessary. */
    if (!realm()->debugScriptMap) {
        auto map = cx->make_unique<DebugScriptMap>();
        if (!map) {
            return false;
        }
=======
  debug->generatorObserverCount++;
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Create realm's debugScriptMap if necessary. */
  if (!realm()->debugScriptMap) {
    auto map = cx->make_unique<DebugScriptMap>();
    if (!map) {
      return false;
    }
||||||| merged common ancestors
        realm()->debugScriptMap = std::move(map);
    }
=======
  // It is our caller's responsibility, before bumping the generator observer
  // count, to make sure that the baseline code includes the necessary
  // JS_AFTERYIELD instrumentation by calling
  // {ensure,update}ExecutionObservabilityOfScript.
  MOZ_ASSERT_IF(hasBaselineScript(), baseline->hasDebugInstrumentation());
>>>>>>> upstream-releases

<<<<<<< HEAD
    realm()->debugScriptMap = std::move(map);
  }
||||||| merged common ancestors
    if (!realm()->debugScriptMap->putNew(this, std::move(debug))) {
        ReportOutOfMemory(cx);
        return false;
    }
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!realm()->debugScriptMap->putNew(this, std::move(debug))) {
    ReportOutOfMemory(cx);
    return false;
  }
||||||| merged common ancestors
    bitFields_.hasDebugScript_ = true; // safe to set this;  we can't fail after this point
=======
void JSScript::decrementGeneratorObserverCount(js::FreeOp* fop) {
  DebugScript* debug = debugScript();
  MOZ_ASSERT(debug);
  MOZ_ASSERT(debug->generatorObserverCount > 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  setFlag(MutableFlags::HasDebugScript);  // safe to set this;  we can't fail
                                          // after this point

  /*
   * Ensure that any Interpret() instances running on this script have
   * interrupts enabled. The interrupts must stay enabled until the
   * debug state is destroyed.
   */
  for (ActivationIterator iter(cx); !iter.done(); ++iter) {
    if (iter->isInterpreter()) {
      iter->asInterpreter()->enableInterruptsIfRunning(this);
    }
  }
||||||| merged common ancestors
    /*
     * Ensure that any Interpret() instances running on this script have
     * interrupts enabled. The interrupts must stay enabled until the
     * debug state is destroyed.
     */
    for (ActivationIterator iter(cx); !iter.done(); ++iter) {
        if (iter->isInterpreter()) {
            iter->asInterpreter()->enableInterruptsIfRunning(this);
        }
    }
=======
  debug->generatorObserverCount--;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  if (!debug->needed()) {
    destroyDebugScript(fop);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void JSScript::setNewStepMode(FreeOp* fop, uint32_t newValue) {
  DebugScript* debug = debugScript();
  uint32_t prior = debug->stepMode;
  debug->stepMode = newValue;
||||||| merged common ancestors
void
JSScript::setNewStepMode(FreeOp* fop, uint32_t newValue)
{
    DebugScript* debug = debugScript();
    uint32_t prior = debug->stepMode;
    debug->stepMode = newValue;
=======
bool JSScript::incrementStepperCount(JSContext* cx) {
  cx->check(this);
  MOZ_ASSERT(cx->realm()->isDebuggee());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!prior != !newValue) {
    if (hasBaselineScript()) {
      baseline->toggleDebugTraps(this, nullptr);
    }
||||||| merged common ancestors
    if (!prior != !newValue) {
        if (hasBaselineScript()) {
            baseline->toggleDebugTraps(this, nullptr);
        }
=======
  AutoRealm ar(cx, this);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!stepModeEnabled() && !debug->numSites) {
      fop->free_(releaseDebugScript());
||||||| merged common ancestors
        if (!stepModeEnabled() && !debug->numSites) {
            fop->free_(releaseDebugScript());
        }
=======
  DebugScript* debug = getOrCreateDebugScript(cx);
  if (!debug) {
    return false;
  }

  debug->stepperCount++;

  if (debug->stepperCount == 1) {
    if (hasBaselineScript()) {
      baseline->toggleDebugTraps(this, nullptr);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
  }

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool JSScript::incrementStepModeCount(JSContext* cx) {
  cx->check(this);
  MOZ_ASSERT(cx->realm()->isDebuggee());
||||||| merged common ancestors
bool
JSScript::incrementStepModeCount(JSContext* cx)
{
    cx->check(this);
    MOZ_ASSERT(cx->realm()->isDebuggee());
=======
void JSScript::decrementStepperCount(FreeOp* fop) {
  DebugScript* debug = debugScript();
  MOZ_ASSERT(debug);
  MOZ_ASSERT(debug->stepperCount > 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoRealm ar(cx, this);
||||||| merged common ancestors
    AutoRealm ar(cx, this);
=======
  debug->stepperCount--;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!ensureHasDebugScript(cx)) {
    return false;
  }
||||||| merged common ancestors
    if (!ensureHasDebugScript(cx)) {
        return false;
    }
=======
  if (debug->stepperCount == 0) {
    if (hasBaselineScript()) {
      baseline->toggleDebugTraps(this, nullptr);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  DebugScript* debug = debugScript();
  uint32_t count = debug->stepMode;
  setNewStepMode(cx->runtime()->defaultFreeOp(), count + 1);
  return true;
}

void JSScript::decrementStepModeCount(FreeOp* fop) {
  DebugScript* debug = debugScript();
  uint32_t count = debug->stepMode;
  MOZ_ASSERT(count > 0);
  setNewStepMode(fop, count - 1);
||||||| merged common ancestors
    DebugScript* debug = debugScript();
    uint32_t count = debug->stepMode;
    setNewStepMode(cx->runtime()->defaultFreeOp(), count + 1);
    return true;
}

void
JSScript::decrementStepModeCount(FreeOp* fop)
{
    DebugScript* debug = debugScript();
    uint32_t count = debug->stepMode;
    MOZ_ASSERT(count > 0);
    setNewStepMode(fop, count - 1);
=======
    if (!debug->needed()) {
      freeDebugScript(fop);
    }
  }
>>>>>>> upstream-releases
}

BreakpointSite* JSScript::getOrCreateBreakpointSite(JSContext* cx,
                                                    jsbytecode* pc) {
  AutoRealm ar(cx, this);

<<<<<<< HEAD
  if (!ensureHasDebugScript(cx)) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!ensureHasDebugScript(cx)) {
        return nullptr;
    }
=======
  DebugScript* debug = getOrCreateDebugScript(cx);
  if (!debug) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  DebugScript* debug = debugScript();
  BreakpointSite*& site = debug->breakpoints[pcToOffset(pc)];
||||||| merged common ancestors
    DebugScript* debug = debugScript();
    BreakpointSite*& site = debug->breakpoints[pcToOffset(pc)];
=======
  BreakpointSite*& site = debug->breakpoints[pcToOffset(pc)];
>>>>>>> upstream-releases

  if (!site) {
    site = cx->new_<JSBreakpointSite>(this, pc);
    if (!site) {
      return nullptr;
    }
<<<<<<< HEAD
    debug->numSites++;
  }
||||||| merged common ancestors
=======
    debug->numSites++;
    AddCellMemory(this, sizeof(JSBreakpointSite), MemoryUse::BreakpointSite);
  }
>>>>>>> upstream-releases

  return site;
}

void JSScript::destroyBreakpointSite(FreeOp* fop, jsbytecode* pc) {
  DebugScript* debug = debugScript();
  BreakpointSite*& site = debug->breakpoints[pcToOffset(pc)];
  MOZ_ASSERT(site);

<<<<<<< HEAD
  fop->delete_(site);
  site = nullptr;
||||||| merged common ancestors
    fop->delete_(site);
    site = nullptr;
=======
  RemoveCellMemory(this, sizeof(JSBreakpointSite), MemoryUse::BreakpointSite);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (--debug->numSites == 0 && !stepModeEnabled()) {
    fop->free_(releaseDebugScript());
  }
||||||| merged common ancestors
    if (--debug->numSites == 0 && !stepModeEnabled()) {
        fop->free_(releaseDebugScript());
    }
=======
  fop->delete_(site);
  site = nullptr;

  debug->numSites--;
  if (!debug->needed()) {
    freeDebugScript(fop);
  }
>>>>>>> upstream-releases
}

void JSScript::clearBreakpointsIn(FreeOp* fop, js::Debugger* dbg,
                                  JSObject* handler) {
  if (!hasAnyBreakpointsOrStepMode()) {
    return;
  }

  for (jsbytecode* pc = code(); pc < codeEnd(); pc++) {
    BreakpointSite* site = getBreakpointSite(pc);
    if (site) {
      Breakpoint* nextbp;
      for (Breakpoint* bp = site->firstBreakpoint(); bp; bp = nextbp) {
        nextbp = bp->nextInSite();
        if ((!dbg || bp->debugger == dbg) &&
            (!handler || bp->getHandler() == handler)) {
          bp->destroy(fop);
        }
      }
    }
  }
}

bool JSScript::hasBreakpointsAt(jsbytecode* pc) {
  BreakpointSite* site = getBreakpointSite(pc);
  if (!site) {
    return false;
  }

  return site->enabledCount > 0;
}

<<<<<<< HEAD
void SharedScriptData::traceChildren(JSTracer* trc) {
  MOZ_ASSERT(refCount() != 0);
  for (uint32_t i = 0; i < natoms(); ++i) {
    TraceNullableEdge(trc, &atoms()[i], "atom");
  }
}
||||||| merged common ancestors
void
SharedScriptData::traceChildren(JSTracer* trc)
{
    MOZ_ASSERT(refCount() != 0);
    for (uint32_t i = 0; i < natoms(); ++i) {
        TraceNullableEdge(trc, &atoms()[i], "atom");
    }
}
=======
/* static */ bool SharedScriptData::InitFromEmitter(
    JSContext* cx, js::HandleScript script, frontend::BytecodeEmitter* bce,
    uint32_t nslots) {
  uint32_t natoms = bce->perScriptData().atomIndices()->count();
>>>>>>> upstream-releases

<<<<<<< HEAD
void JSScript::traceChildren(JSTracer* trc) {
  // NOTE: this JSScript may be partially initialized at this point.  E.g. we
  // may have created it and partially initialized it with
  // JSScript::Create(), but not yet finished initializing it with
  // fullyInitFromEmitter() or fullyInitTrivial().
||||||| merged common ancestors
void
JSScript::traceChildren(JSTracer* trc)
{
    // NOTE: this JSScript may be partially initialized at this point.  E.g. we
    // may have created it and partially initialized it with
    // JSScript::Create(), but not yet finished initializing it with
    // fullyInitFromEmitter() or fullyInitTrivial().
=======
  size_t codeLength = bce->bytecodeSection().code().length();
  MOZ_RELEASE_ASSERT(codeLength <= frontend::MaxBytecodeLength);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT_IF(trc->isMarkingTracer() &&
                    GCMarker::fromTracer(trc)->shouldCheckCompartments(),
                zone()->isCollecting());
||||||| merged common ancestors
    MOZ_ASSERT_IF(trc->isMarkingTracer() &&
                  GCMarker::fromTracer(trc)->shouldCheckCompartments(),
                  zone()->isCollecting());

    if (scriptData()) {
        scriptData()->traceChildren(trc);
    }
=======
  // There are 1-4 copies of SN_MAKE_TERMINATOR appended after the source
  // notes. These are a combination of sentinel and padding values.
  static_assert(frontend::MaxSrcNotesLength <= UINT32_MAX - CodeNoteAlign,
                "Length + CodeNoteAlign shouldn't overflow UINT32_MAX");
  size_t noteLength = bce->bytecodeSection().notes().length();
  MOZ_RELEASE_ASSERT(noteLength <= frontend::MaxSrcNotesLength);

  size_t nullLength = ComputeNotePadding(codeLength, noteLength);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (data_) {
    data_->traceChildren(trc);
  }
||||||| merged common ancestors
    if (data) {
        auto array = scopes();
        TraceRange(trc, array.size(), array.data(), "scopes");
    }
=======
  uint32_t numResumeOffsets =
      bce->bytecodeSection().resumeOffsetList().length();
  uint32_t numScopeNotes = bce->bytecodeSection().scopeNoteList().length();
  uint32_t numTryNotes = bce->bytecodeSection().tryNoteList().length();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (scriptData()) {
    scriptData()->traceChildren(trc);
  }
||||||| merged common ancestors
    if (hasConsts()) {
        auto array = consts();
        TraceRange(trc, array.size(), array.data(), "consts");
    }
=======
  // Create and initialize SharedScriptData
  if (!script->createSharedScriptData(cx, codeLength, noteLength + nullLength,
                                      natoms, numResumeOffsets, numScopeNotes,
                                      numTryNotes)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT_IF(sourceObject(),
                MaybeForwarded(sourceObject())->compartment() == compartment());
  TraceNullableEdge(trc, &sourceObject_, "sourceObject");
||||||| merged common ancestors
    if (hasObjects()) {
        auto array = objects();
        TraceRange(trc, array.size(), array.data(), "objects");
    }
=======
  js::SharedScriptData* data = script->scriptData_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (maybeLazyScript()) {
    TraceManuallyBarrieredEdge(trc, &lazyScript, "lazyScript");
  }
||||||| merged common ancestors
    MOZ_ASSERT_IF(sourceObject(), MaybeForwarded(sourceObject())->compartment() == compartment());
    TraceNullableEdge(trc, &sourceObject_, "sourceObject");
=======
  // Initialize POD fields
  data->mainOffset = bce->mainOffset();
  data->nfixed = bce->maxFixedSlots;
  data->nslots = nslots;
  data->bodyScopeIndex = bce->bodyScopeIndex;
  data->numICEntries = bce->bytecodeSection().numICEntries();
  data->numBytecodeTypeSets =
      std::min<uint32_t>(uint32_t(JSScript::MaxBytecodeTypeSets),
                         bce->bytecodeSection().numTypeSets());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (trc->isMarkingTracer()) {
    realm()->mark();
  }
||||||| merged common ancestors
    if (maybeLazyScript()) {
        TraceManuallyBarrieredEdge(trc, &lazyScript, "lazyScript");
    }
=======
  if (bce->sc->isFunctionBox()) {
    data->funLength = bce->sc->asFunctionBox()->length;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  jit::TraceJitScripts(trc, this);
||||||| merged common ancestors
    if (trc->isMarkingTracer()) {
        realm()->mark();
    }
=======
  // Initialize trailing arrays
  InitAtomMap(*bce->perScriptData().atomIndices(), data->atoms());
  std::copy_n(bce->bytecodeSection().code().begin(), codeLength, data->code());
  std::copy_n(bce->bytecodeSection().notes().begin(), noteLength,
              data->notes());
  std::fill_n(data->notes() + noteLength, nullLength, SRC_NULL);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (trc->isMarkingTracer()) {
    return GCMarker::fromTracer(trc)->markImplicitEdges(this);
  }
}
||||||| merged common ancestors
    jit::TraceJitScripts(trc, this);
}
=======
  bce->bytecodeSection().resumeOffsetList().finish(data->resumeOffsets());
  bce->bytecodeSection().scopeNoteList().finish(data->scopeNotes());
  bce->bytecodeSection().tryNoteList().finish(data->tryNotes());
>>>>>>> upstream-releases

<<<<<<< HEAD
void LazyScript::finalize(FreeOp* fop) { fop->free_(table_); }
||||||| merged common ancestors
void
LazyScript::finalize(FreeOp* fop)
{
    fop->free_(table_);
}
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
size_t JSScript::calculateLiveFixed(jsbytecode* pc) {
  size_t nlivefixed = numAlwaysLiveFixedSlots();
||||||| merged common ancestors
size_t
JSScript::calculateLiveFixed(jsbytecode* pc)
{
    size_t nlivefixed = numAlwaysLiveFixedSlots();
=======
void SharedScriptData::traceChildren(JSTracer* trc) {
  MOZ_ASSERT(refCount() != 0);
  for (uint32_t i = 0; i < natoms(); ++i) {
    TraceNullableEdge(trc, &atoms()[i], "atom");
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (nfixed() != nlivefixed) {
    Scope* scope = lookupScope(pc);
    if (scope) {
      scope = MaybeForwarded(scope);
    }
||||||| merged common ancestors
    if (nfixed() != nlivefixed) {
        Scope* scope = lookupScope(pc);
        if (scope) {
            scope = MaybeForwarded(scope);
        }
=======
void SharedScriptData::markForCrossZone(JSContext* cx) {
  for (uint32_t i = 0; i < natoms(); ++i) {
    cx->markAtom(atoms()[i]);
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Find the nearest LexicalScope in the same script.
    while (scope && scope->is<WithScope>()) {
      scope = scope->enclosing();
      if (scope) {
        scope = MaybeForwarded(scope);
      }
    }
||||||| merged common ancestors
        // Find the nearest LexicalScope in the same script.
        while (scope && scope->is<WithScope>()) {
            scope = scope->enclosing();
            if (scope) {
                scope = MaybeForwarded(scope);
            }
        }
=======
void JSScript::traceChildren(JSTracer* trc) {
  // NOTE: this JSScript may be partially initialized at this point.  E.g. we
  // may have created it and partially initialized it with
  // JSScript::Create(), but not yet finished initializing it with
  // fullyInitFromEmitter() or fullyInitTrivial().
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (scope) {
      if (scope->is<LexicalScope>()) {
        nlivefixed = scope->as<LexicalScope>().nextFrameSlot();
      } else if (scope->is<VarScope>()) {
        nlivefixed = scope->as<VarScope>().nextFrameSlot();
      }
    }
  }
||||||| merged common ancestors
        if (scope) {
            if (scope->is<LexicalScope>()) {
                nlivefixed = scope->as<LexicalScope>().nextFrameSlot();
            } else if (scope->is<VarScope>()) {
                nlivefixed = scope->as<VarScope>().nextFrameSlot();
            }
        }
    }
=======
  MOZ_ASSERT_IF(trc->isMarkingTracer() &&
                    GCMarker::fromTracer(trc)->shouldCheckCompartments(),
                zone()->isCollecting());
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(nlivefixed <= nfixed());
  MOZ_ASSERT(nlivefixed >= numAlwaysLiveFixedSlots());
||||||| merged common ancestors
    MOZ_ASSERT(nlivefixed <= nfixed());
    MOZ_ASSERT(nlivefixed >= numAlwaysLiveFixedSlots());
=======
  if (data_) {
    data_->trace(trc);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return nlivefixed;
}
||||||| merged common ancestors
    return nlivefixed;
}
=======
  if (scriptData()) {
    scriptData()->traceChildren(trc);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
Scope* JSScript::lookupScope(jsbytecode* pc) {
  MOZ_ASSERT(containsPC(pc));
||||||| merged common ancestors
Scope*
JSScript::lookupScope(jsbytecode* pc)
{
    MOZ_ASSERT(containsPC(pc));

    if (!hasScopeNotes()) {
        return nullptr;
    }
=======
  MOZ_ASSERT_IF(sourceObject(),
                MaybeForwarded(sourceObject())->compartment() == compartment());
  TraceNullableEdge(trc, &sourceObject_, "sourceObject");

  if (maybeLazyScript()) {
    TraceManuallyBarrieredEdge(trc, &lazyScript, "lazyScript");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!hasScopeNotes()) {
    return nullptr;
  }

  size_t offset = pc - code();

  auto notes = scopeNotes();
  Scope* scope = nullptr;

  // Find the innermost block chain using a binary search.
  size_t bottom = 0;
  size_t top = notes.size();

  while (bottom < top) {
    size_t mid = bottom + (top - bottom) / 2;
    const ScopeNote* note = &notes[mid];
    if (note->start <= offset) {
      // Block scopes are ordered in the list by their starting offset, and
      // since blocks form a tree ones earlier in the list may cover the pc even
      // if later blocks end before the pc. This only happens when the earlier
      // block is a parent of the later block, so we need to check parents of
      // |mid| in the searched range for coverage.
      size_t check = mid;
      while (check >= bottom) {
        const ScopeNote* checkNote = &notes[check];
        MOZ_ASSERT(checkNote->start <= offset);
        if (offset < checkNote->start + checkNote->length) {
          // We found a matching block chain but there may be inner ones
          // at a higher block chain index than mid. Continue the binary search.
          if (checkNote->index == ScopeNote::NoScopeIndex) {
            scope = nullptr;
          } else {
            scope = getScope(checkNote->index);
          }
          break;
        }
        if (checkNote->parent == UINT32_MAX) {
          break;
        }
        check = checkNote->parent;
      }
      bottom = mid + 1;
    } else {
      top = mid;
    }
  }
||||||| merged common ancestors
    size_t offset = pc - code();

    auto notes = scopeNotes();
    Scope* scope = nullptr;

    // Find the innermost block chain using a binary search.
    size_t bottom = 0;
    size_t top = notes.size();

    while (bottom < top) {
        size_t mid = bottom + (top - bottom) / 2;
        const ScopeNote* note = &notes[mid];
        if (note->start <= offset) {
            // Block scopes are ordered in the list by their starting offset, and since
            // blocks form a tree ones earlier in the list may cover the pc even if
            // later blocks end before the pc. This only happens when the earlier block
            // is a parent of the later block, so we need to check parents of |mid| in
            // the searched range for coverage.
            size_t check = mid;
            while (check >= bottom) {
                const ScopeNote* checkNote = &notes[check];
                MOZ_ASSERT(checkNote->start <= offset);
                if (offset < checkNote->start + checkNote->length) {
                    // We found a matching block chain but there may be inner ones
                    // at a higher block chain index than mid. Continue the binary search.
                    if (checkNote->index == ScopeNote::NoScopeIndex) {
                        scope = nullptr;
                    } else {
                        scope = getScope(checkNote->index);
                    }
                    break;
                }
                if (checkNote->parent == UINT32_MAX) {
                    break;
                }
                check = checkNote->parent;
            }
            bottom = mid + 1;
        } else {
            top = mid;
        }
    }
=======
  JSObject* global = realm()->unsafeUnbarrieredMaybeGlobal();
  MOZ_ASSERT(global);
  TraceManuallyBarrieredEdge(trc, &global, "script_global");
>>>>>>> upstream-releases

<<<<<<< HEAD
  return scope;
||||||| merged common ancestors
    return scope;
}

Scope*
JSScript::innermostScope(jsbytecode* pc)
{
    if (Scope* scope = lookupScope(pc)) {
        return scope;
    }
    return bodyScope();
=======
  jit::TraceJitScripts(trc, this);

  if (trc->isMarkingTracer()) {
    GCMarker::fromTracer(trc)->markImplicitEdges(this);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
Scope* JSScript::innermostScope(jsbytecode* pc) {
  if (Scope* scope = lookupScope(pc)) {
    return scope;
  }
  return bodyScope();
||||||| merged common ancestors
void
JSScript::setArgumentsHasVarBinding()
{
    bitFields_.argsHasVarBinding_ = true;
    bitFields_.needsArgsAnalysis_ = true;
=======
void LazyScript::finalize(FreeOp* fop) {
  if (lazyData_) {
    fop->free_(this, lazyData_, lazyData_->allocationSize(),
               MemoryUse::LazyScriptData);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void JSScript::setArgumentsHasVarBinding() {
  setFlag(ImmutableFlags::ArgsHasVarBinding);
  setFlag(MutableFlags::NeedsArgsAnalysis);
}
||||||| merged common ancestors
void
JSScript::setNeedsArgsObj(bool needsArgsObj)
{
    MOZ_ASSERT_IF(needsArgsObj, argumentsHasVarBinding());
    bitFields_.needsArgsAnalysis_ = false;
    bitFields_.needsArgsObj_ = needsArgsObj;
}
=======
size_t JSScript::calculateLiveFixed(jsbytecode* pc) {
  size_t nlivefixed = numAlwaysLiveFixedSlots();
>>>>>>> upstream-releases

<<<<<<< HEAD
void JSScript::setNeedsArgsObj(bool needsArgsObj) {
  MOZ_ASSERT_IF(needsArgsObj, argumentsHasVarBinding());
  clearFlag(MutableFlags::NeedsArgsAnalysis);
  setFlag(MutableFlags::NeedsArgsObj, needsArgsObj);
||||||| merged common ancestors
void
js::SetFrameArgumentsObject(JSContext* cx, AbstractFramePtr frame,
                            HandleScript script, JSObject* argsobj)
{
    /*
     * Replace any optimized arguments in the frame with an explicit arguments
     * object. Note that 'arguments' may have already been overwritten.
     */

    Rooted<BindingIter> bi(cx, BindingIter(script));
    while (bi && bi.name() != cx->names().arguments) {
        bi++;
    }
    if (!bi) {
        return;
    }

    if (bi.location().kind() == BindingLocation::Kind::Environment) {
        /*
         * Scan the script to find the slot in the call object that 'arguments'
         * is assigned to.
         */
        jsbytecode* pc = script->code();
        while (*pc != JSOP_ARGUMENTS) {
            pc += GetBytecodeLength(pc);
        }
        pc += JSOP_ARGUMENTS_LENGTH;
        MOZ_ASSERT(*pc == JSOP_SETALIASEDVAR);

        // Note that here and below, it is insufficient to only check for
        // JS_OPTIMIZED_ARGUMENTS, as Ion could have optimized out the
        // arguments slot.
        EnvironmentObject& env = frame.callObj().as<EnvironmentObject>();
        if (IsOptimizedPlaceholderMagicValue(env.aliasedBinding(bi))) {
            env.setAliasedBinding(cx, bi, ObjectValue(*argsobj));
        }
    } else {
        MOZ_ASSERT(bi.location().kind() == BindingLocation::Kind::Frame);
        uint32_t frameSlot = bi.location().slot();
        if (IsOptimizedPlaceholderMagicValue(frame.unaliasedLocal(frameSlot))) {
            frame.unaliasedLocal(frameSlot) = ObjectValue(*argsobj);
        }
    }
=======
  if (nfixed() != nlivefixed) {
    Scope* scope = lookupScope(pc);
    if (scope) {
      scope = MaybeForwarded(scope);
    }

    // Find the nearest LexicalScope in the same script.
    while (scope && scope->is<WithScope>()) {
      scope = scope->enclosing();
      if (scope) {
        scope = MaybeForwarded(scope);
      }
    }

    if (scope) {
      if (scope->is<LexicalScope>()) {
        nlivefixed = scope->as<LexicalScope>().nextFrameSlot();
      } else if (scope->is<VarScope>()) {
        nlivefixed = scope->as<VarScope>().nextFrameSlot();
      }
    }
  }

  MOZ_ASSERT(nlivefixed <= nfixed());
  MOZ_ASSERT(nlivefixed >= numAlwaysLiveFixedSlots());

  return nlivefixed;
}

Scope* JSScript::lookupScope(jsbytecode* pc) {
  MOZ_ASSERT(containsPC(pc));

  if (!hasScopeNotes()) {
    return nullptr;
  }

  size_t offset = pc - code();

  auto notes = scopeNotes();
  Scope* scope = nullptr;

  // Find the innermost block chain using a binary search.
  size_t bottom = 0;
  size_t top = notes.size();

  while (bottom < top) {
    size_t mid = bottom + (top - bottom) / 2;
    const ScopeNote* note = &notes[mid];
    if (note->start <= offset) {
      // Block scopes are ordered in the list by their starting offset, and
      // since blocks form a tree ones earlier in the list may cover the pc even
      // if later blocks end before the pc. This only happens when the earlier
      // block is a parent of the later block, so we need to check parents of
      // |mid| in the searched range for coverage.
      size_t check = mid;
      while (check >= bottom) {
        const ScopeNote* checkNote = &notes[check];
        MOZ_ASSERT(checkNote->start <= offset);
        if (offset < checkNote->start + checkNote->length) {
          // We found a matching block chain but there may be inner ones
          // at a higher block chain index than mid. Continue the binary search.
          if (checkNote->index == ScopeNote::NoScopeIndex) {
            scope = nullptr;
          } else {
            scope = getScope(checkNote->index);
          }
          break;
        }
        if (checkNote->parent == UINT32_MAX) {
          break;
        }
        check = checkNote->parent;
      }
      bottom = mid + 1;
    } else {
      top = mid;
    }
  }

  return scope;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void js::SetFrameArgumentsObject(JSContext* cx, AbstractFramePtr frame,
                                 HandleScript script, JSObject* argsobj) {
  /*
   * Replace any optimized arguments in the frame with an explicit arguments
   * object. Note that 'arguments' may have already been overwritten.
   */
||||||| merged common ancestors
/* static */ bool
JSScript::argumentsOptimizationFailed(JSContext* cx, HandleScript script)
{
    MOZ_ASSERT(script->functionNonDelazifying());
    MOZ_ASSERT(script->analyzedArgsUsage());
    MOZ_ASSERT(script->argumentsHasVarBinding());

    /*
     * It is possible that the arguments optimization has already failed,
     * everything has been fixed up, but there was an outstanding magic value
     * on the stack that has just now flowed into an apply. In this case, there
     * is nothing to do; GuardFunApplySpeculation will patch in the real
     * argsobj.
     */
    if (script->needsArgsObj()) {
        return true;
    }

    MOZ_ASSERT(!script->isGenerator());
    MOZ_ASSERT(!script->isAsync());
=======
Scope* JSScript::innermostScope(jsbytecode* pc) {
  if (Scope* scope = lookupScope(pc)) {
    return scope;
  }
  return bodyScope();
}

void JSScript::setArgumentsHasVarBinding() {
  setFlag(ImmutableFlags::ArgsHasVarBinding);
  setFlag(MutableFlags::NeedsArgsAnalysis);
}

void JSScript::setNeedsArgsObj(bool needsArgsObj) {
  MOZ_ASSERT_IF(needsArgsObj, argumentsHasVarBinding());
  clearFlag(MutableFlags::NeedsArgsAnalysis);
  setFlag(MutableFlags::NeedsArgsObj, needsArgsObj);
}

void js::SetFrameArgumentsObject(JSContext* cx, AbstractFramePtr frame,
                                 HandleScript script, JSObject* argsobj) {
  /*
   * Replace any optimized arguments in the frame with an explicit arguments
   * object. Note that 'arguments' may have already been overwritten.
   */
>>>>>>> upstream-releases

  Rooted<BindingIter> bi(cx, BindingIter(script));
  while (bi && bi.name() != cx->names().arguments) {
    bi++;
  }
  if (!bi) {
    return;
  }

  if (bi.location().kind() == BindingLocation::Kind::Environment) {
    /*
     * Scan the script to find the slot in the call object that 'arguments'
     * is assigned to.
     */
<<<<<<< HEAD
    jsbytecode* pc = script->code();
    while (*pc != JSOP_ARGUMENTS) {
      pc += GetBytecodeLength(pc);
    }
    pc += JSOP_ARGUMENTS_LENGTH;
    MOZ_ASSERT(*pc == JSOP_SETALIASEDVAR);

    // Note that here and below, it is insufficient to only check for
    // JS_OPTIMIZED_ARGUMENTS, as Ion could have optimized out the
    // arguments slot.
    EnvironmentObject& env = frame.callObj().as<EnvironmentObject>();
    if (IsOptimizedPlaceholderMagicValue(env.aliasedBinding(bi))) {
      env.setAliasedBinding(cx, bi, ObjectValue(*argsobj));
    }
  } else {
    MOZ_ASSERT(bi.location().kind() == BindingLocation::Kind::Frame);
    uint32_t frameSlot = bi.location().slot();
    if (IsOptimizedPlaceholderMagicValue(frame.unaliasedLocal(frameSlot))) {
      frame.unaliasedLocal(frameSlot) = ObjectValue(*argsobj);
    }
  }
}

/* static */ bool JSScript::argumentsOptimizationFailed(JSContext* cx,
                                                        HandleScript script) {
  MOZ_ASSERT(script->functionNonDelazifying());
  MOZ_ASSERT(script->analyzedArgsUsage());
  MOZ_ASSERT(script->argumentsHasVarBinding());

  /*
   * It is possible that the arguments optimization has already failed,
   * everything has been fixed up, but there was an outstanding magic value
   * on the stack that has just now flowed into an apply. In this case, there
   * is nothing to do; GuardFunApplySpeculation will patch in the real
   * argsobj.
   */
  if (script->needsArgsObj()) {
    return true;
  }

  MOZ_ASSERT(!script->isGenerator());
  MOZ_ASSERT(!script->isAsync());

  script->setFlag(MutableFlags::NeedsArgsObj);

  /*
   * By design, the arguments optimization is only made when there are no
   * outstanding cases of MagicValue(JS_OPTIMIZED_ARGUMENTS) at any points
   * where the optimization could fail, other than an active invocation of
   * 'f.apply(x, arguments)'. Thus, there are no outstanding values of
   * MagicValue(JS_OPTIMIZED_ARGUMENTS) on the stack. However, there are
   * three things that need fixup:
   *  - there may be any number of activations of this script that don't have
   *    an argsObj that now need one.
   *  - jit code compiled (and possible active on the stack) with the static
   *    assumption of !script->needsArgsObj();
   *  - type inference data for the script assuming script->needsArgsObj
   */
  for (AllScriptFramesIter i(cx); !i.done(); ++i) {
||||||| merged common ancestors
    if (script->hasBaselineScript()) {
        script->baselineScript()->setNeedsArgsObj();
    }

=======
    jsbytecode* pc = script->code();
    while (*pc != JSOP_ARGUMENTS) {
      pc += GetBytecodeLength(pc);
    }
    pc += JSOP_ARGUMENTS_LENGTH;
    MOZ_ASSERT(*pc == JSOP_SETALIASEDVAR);

    // Note that here and below, it is insufficient to only check for
    // JS_OPTIMIZED_ARGUMENTS, as Ion could have optimized out the
    // arguments slot.
    EnvironmentObject& env = frame.callObj().as<EnvironmentObject>();
    if (IsOptimizedPlaceholderMagicValue(env.aliasedBinding(bi))) {
      env.setAliasedBinding(cx, bi, ObjectValue(*argsobj));
    }
  } else {
    MOZ_ASSERT(bi.location().kind() == BindingLocation::Kind::Frame);
    uint32_t frameSlot = bi.location().slot();
    if (IsOptimizedPlaceholderMagicValue(frame.unaliasedLocal(frameSlot))) {
      frame.unaliasedLocal(frameSlot) = ObjectValue(*argsobj);
    }
  }
}

/* static */
void JSScript::argumentsOptimizationFailed(JSContext* cx, HandleScript script) {
  MOZ_ASSERT(script->functionNonDelazifying());
  MOZ_ASSERT(script->analyzedArgsUsage());
  MOZ_ASSERT(script->argumentsHasVarBinding());

  /*
   * It is possible that the arguments optimization has already failed,
   * everything has been fixed up, but there was an outstanding magic value
   * on the stack that has just now flowed into an apply. In this case, there
   * is nothing to do; GuardFunApplySpeculation will patch in the real
   * argsobj.
   */
  if (script->needsArgsObj()) {
    return;
  }

  MOZ_ASSERT(!script->isGenerator());
  MOZ_ASSERT(!script->isAsync());

  script->setFlag(MutableFlags::NeedsArgsObj);

  /*
   * By design, the arguments optimization is only made when there are no
   * outstanding cases of MagicValue(JS_OPTIMIZED_ARGUMENTS) at any points
   * where the optimization could fail, other than an active invocation of
   * 'f.apply(x, arguments)'. Thus, there are no outstanding values of
   * MagicValue(JS_OPTIMIZED_ARGUMENTS) on the stack. However, there are
   * three things that need fixup:
   *  - there may be any number of activations of this script that don't have
   *    an argsObj that now need one.
   *  - jit code compiled (and possible active on the stack) with the static
   *    assumption of !script->needsArgsObj();
   *  - type inference data for the script assuming script->needsArgsObj
   */
  for (AllScriptFramesIter i(cx); !i.done(); ++i) {
>>>>>>> upstream-releases
    /*
     * We cannot reliably create an arguments object for Ion activations of
     * this script.  To maintain the invariant that "script->needsArgsObj
     * implies fp->hasArgsObj", the Ion bail mechanism will create an
     * arguments object right after restoring the BaselineFrame and before
     * entering Baseline code (in jit::FinishBailoutToBaseline).
     */
    if (i.isIon()) {
      continue;
    }
    AbstractFramePtr frame = i.abstractFramePtr();
    if (frame.isFunctionFrame() && frame.script() == script) {
      /* We crash on OOM since cleaning up here would be complicated. */
      AutoEnterOOMUnsafeRegion oomUnsafe;
      ArgumentsObject* argsobj = ArgumentsObject::createExpected(cx, frame);
      if (!argsobj) {
        oomUnsafe.crash("JSScript::argumentsOptimizationFailed");
      }
      SetFrameArgumentsObject(cx, frame, script, argsobj);
    }
<<<<<<< HEAD
  }

  return true;
||||||| merged common ancestors

    return true;
=======
  }
>>>>>>> upstream-releases
}

bool JSScript::formalIsAliased(unsigned argSlot) {
  if (functionHasParameterExprs()) {
    return false;
  }

  for (PositionalFormalParameterIter fi(this); fi; fi++) {
    if (fi.argumentSlot() == argSlot) {
      return fi.closedOver();
    }
  }
  MOZ_CRASH("Argument slot not found");
}

bool JSScript::formalLivesInArgumentsObject(unsigned argSlot) {
  return argsObjAliasesFormals() && !formalIsAliased(argSlot);
}

<<<<<<< HEAD
LazyScript::LazyScript(JSFunction* fun, ScriptSourceObject& sourceObject,
                       void* table, uint64_t packedFields, uint32_t sourceStart,
                       uint32_t sourceEnd, uint32_t toStringStart,
                       uint32_t lineno, uint32_t column)
    : script_(nullptr),
      function_(fun),
      sourceObject_(&sourceObject),
      table_(table),
      packedFields_(packedFields),
      sourceStart_(sourceStart),
      sourceEnd_(sourceEnd),
      toStringStart_(toStringStart),
      toStringEnd_(sourceEnd),
      lineno_(lineno),
      column_(column) {
  MOZ_ASSERT(function_);
  MOZ_ASSERT(sourceObject_);
  MOZ_ASSERT(function_->compartment() == sourceObject_->compartment());
  MOZ_ASSERT(sourceStart <= sourceEnd);
  MOZ_ASSERT(toStringStart <= sourceStart);
}
||||||| merged common ancestors
LazyScript::LazyScript(JSFunction* fun, ScriptSourceObject& sourceObject,
                       void* table, uint64_t packedFields,
                       uint32_t sourceStart, uint32_t sourceEnd,
                       uint32_t toStringStart, uint32_t lineno, uint32_t column)
  : script_(nullptr),
    function_(fun),
    sourceObject_(&sourceObject),
    table_(table),
    packedFields_(packedFields),
    sourceStart_(sourceStart),
    sourceEnd_(sourceEnd),
    toStringStart_(toStringStart),
    toStringEnd_(sourceEnd),
    lineno_(lineno),
    column_(column)
{
    MOZ_ASSERT(function_);
    MOZ_ASSERT(sourceObject_);
    MOZ_ASSERT(function_->compartment() == sourceObject_->compartment());
    MOZ_ASSERT(sourceStart <= sourceEnd);
    MOZ_ASSERT(toStringStart <= sourceStart);
}

void
LazyScript::initScript(JSScript* script)
{
    MOZ_ASSERT(script);
    MOZ_ASSERT(!script_.unbarrieredGet());
    script_.set(script);
}

JS::Compartment*
LazyScript::compartment() const
{
    return function_->compartment();
}

Realm*
LazyScript::realm() const
{
    return function_->realm();
}

void
LazyScript::setEnclosingLazyScript(LazyScript* enclosingLazyScript)
{
    MOZ_ASSERT(enclosingLazyScript);

    // We never change an existing LazyScript.
    MOZ_ASSERT(!hasEnclosingLazyScript());

    // Enclosing scopes never transition back to enclosing lazy scripts.
    MOZ_ASSERT(!hasEnclosingScope());

    enclosingLazyScriptOrScope_ = enclosingLazyScript;
}

void
LazyScript::setEnclosingScope(Scope* enclosingScope)
{
    MOZ_ASSERT(enclosingScope);
    MOZ_ASSERT(!hasEnclosingScope());

    enclosingLazyScriptOrScope_ = enclosingScope;
}

ScriptSourceObject&
LazyScript::sourceObject() const
{
    return sourceObject_->as<ScriptSourceObject>();
}

ScriptSource*
LazyScript::maybeForwardedScriptSource() const
{
    JSObject* source = MaybeForwarded(&sourceObject());
    return UncheckedUnwrapWithoutExpose(source)->as<ScriptSourceObject>().source();
}

/* static */ LazyScript*
LazyScript::CreateRaw(JSContext* cx, HandleFunction fun,
                      HandleScriptSourceObject sourceObject,
                      uint64_t packedFields, uint32_t sourceStart, uint32_t sourceEnd,
                      uint32_t toStringStart, uint32_t lineno, uint32_t column)
{
    cx->check(fun);

    MOZ_ASSERT(sourceObject);
    union {
        PackedView p;
        uint64_t packed;
    };

    packed = packedFields;

    // Reset runtime flags to obtain a fresh LazyScript.
    p.hasBeenCloned = false;
    p.treatAsRunOnce = false;

    size_t bytes = (p.numClosedOverBindings * sizeof(JSAtom*))
                 + (p.numInnerFunctions * sizeof(GCPtrFunction));

    UniquePtr<uint8_t, JS::FreePolicy> table;
    if (bytes) {
        table.reset(cx->pod_malloc<uint8_t>(bytes));
        if (!table) {
            return nullptr;
        }
    }
=======
/* static */ size_t LazyScriptData::AllocationSize(
    uint32_t numClosedOverBindings, uint32_t numInnerFunctions) {
  size_t size = sizeof(LazyScriptData);

  size += numClosedOverBindings * sizeof(GCPtrAtom);
  size += numInnerFunctions * sizeof(GCPtrFunction);

  return size;
}

inline size_t LazyScriptData::allocationSize() const {
  return AllocationSize(numClosedOverBindings_, numInnerFunctions_);
}

// Placement-new elements of an array. This should optimize away for types with
// trivial default initiation.
template <typename T>
void LazyScriptData::initElements(size_t offset, size_t length) {
  void* raw = offsetToPointer<void>(offset);
  DefaultInitializeElements<T>(raw, length);
}

LazyScriptData::LazyScriptData(uint32_t numClosedOverBindings,
                               uint32_t numInnerFunctions)
    : numClosedOverBindings_(numClosedOverBindings),
      numInnerFunctions_(numInnerFunctions) {
  // Variable-length data begins immediately after LazyScriptData itself.
  size_t cursor = sizeof(*this);

  // Default-initialize trailing arrays.

  static_assert(alignof(LazyScriptData) >= alignof(GCPtrAtom),
                "Incompatible alignment");
  initElements<GCPtrAtom>(cursor, numClosedOverBindings);
  cursor += numClosedOverBindings * sizeof(GCPtrAtom);

  static_assert(alignof(GCPtrAtom) >= alignof(GCPtrFunction),
                "Incompatible alignment");
  initElements<GCPtrFunction>(cursor, numInnerFunctions);
  cursor += numInnerFunctions * sizeof(GCPtrFunction);

  // Sanity check
  MOZ_ASSERT(AllocationSize(numClosedOverBindings, numInnerFunctions) ==
             cursor);
}

/* static */ LazyScriptData* LazyScriptData::new_(
    JSContext* cx, uint32_t numClosedOverBindings, uint32_t numInnerFunctions) {
  // Compute size including trailing arrays
  size_t size = AllocationSize(numClosedOverBindings, numInnerFunctions);

  // Allocate contiguous raw buffer
  void* raw = cx->pod_malloc<uint8_t>(size);
  MOZ_ASSERT(uintptr_t(raw) % alignof(LazyScriptData) == 0);
  if (!raw) {
    return nullptr;
  }

  // Constuct the LazyScriptData. Trailing arrays are uninitialized but
  // GCPtrs are put into a safe state.
  return new (raw) LazyScriptData(numClosedOverBindings, numInnerFunctions);
}

mozilla::Span<GCPtrAtom> LazyScriptData::closedOverBindings() {
  size_t offset = sizeof(LazyScriptData);
  return mozilla::MakeSpan(offsetToPointer<GCPtrAtom>(offset),
                           numClosedOverBindings_);
}

mozilla::Span<GCPtrFunction> LazyScriptData::innerFunctions() {
  size_t offset =
      sizeof(LazyScriptData) + sizeof(GCPtrAtom) * numClosedOverBindings_;
  return mozilla::MakeSpan(offsetToPointer<GCPtrFunction>(offset),
                           numInnerFunctions_);
}

void LazyScriptData::trace(JSTracer* trc) {
  if (numClosedOverBindings_) {
    auto array = closedOverBindings();
    TraceRange(trc, array.size(), array.data(), "closedOverBindings");
  }

  if (numInnerFunctions_) {
    auto array = innerFunctions();
    TraceRange(trc, array.size(), array.data(), "innerFunctions");
  }
}

LazyScript::LazyScript(JSFunction* fun, ScriptSourceObject& sourceObject,
                       LazyScriptData* data, uint32_t immutableFlags,
                       uint32_t sourceStart, uint32_t sourceEnd,
                       uint32_t toStringStart, uint32_t lineno, uint32_t column)
    : script_(nullptr),
      function_(fun),
      sourceObject_(&sourceObject),
      lazyData_(data),
      immutableFlags_(immutableFlags),
      mutableFlags_(0),
      sourceStart_(sourceStart),
      sourceEnd_(sourceEnd),
      toStringStart_(toStringStart),
      toStringEnd_(sourceEnd),
      lineno_(lineno),
      column_(column) {
  MOZ_ASSERT(function_);
  MOZ_ASSERT(sourceObject_);
  MOZ_ASSERT(function_->compartment() == sourceObject_->compartment());
  MOZ_ASSERT(sourceStart <= sourceEnd);
  MOZ_ASSERT(toStringStart <= sourceStart);
>>>>>>> upstream-releases

<<<<<<< HEAD
void LazyScript::initScript(JSScript* script) {
  MOZ_ASSERT(script);
  MOZ_ASSERT(!script_.unbarrieredGet());
  script_.set(script);
}
||||||| merged common ancestors
    LazyScript* res = Allocate<LazyScript>(cx);
    if (!res) {
        return nullptr;
    }
=======
  if (data) {
    AddCellMemory(this, data->allocationSize(), MemoryUse::LazyScriptData);
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
JS::Compartment* LazyScript::compartment() const {
  return function_->compartment();
}
||||||| merged common ancestors
    cx->realm()->scheduleDelazificationForDebugger();

    return new (res) LazyScript(fun, *sourceObject, table.release(), packed, sourceStart,
                                sourceEnd, toStringStart, lineno, column);
}

/* static */ LazyScript*
LazyScript::Create(JSContext* cx, HandleFunction fun,
                   HandleScriptSourceObject sourceObject,
                   const frontend::AtomVector& closedOverBindings,
                   Handle<GCVector<JSFunction*, 8>> innerFunctions,
                   uint32_t sourceStart, uint32_t sourceEnd,
                   uint32_t toStringStart, uint32_t lineno, uint32_t column,
                   frontend::ParseGoal parseGoal)
{
    union {
        PackedView p;
        uint64_t packedFields;
    };

    p.shouldDeclareArguments = false;
    p.hasThisBinding = false;
    p.isAsync = false;
    p.hasRest = false;
    p.numClosedOverBindings = closedOverBindings.length();
    p.numInnerFunctions = innerFunctions.length();
    p.isGenerator = false;
    p.strict = false;
    p.bindingsAccessedDynamically = false;
    p.hasDebuggerStatement = false;
    p.hasDirectEval = false;
    p.isLikelyConstructorWrapper = false;
    p.isDerivedClassConstructor = false;
    p.needsHomeObject = false;
    p.isBinAST = false;
    p.parseGoal = uint32_t(parseGoal);

    LazyScript* res = LazyScript::CreateRaw(cx, fun, sourceObject, packedFields,
                                            sourceStart, sourceEnd,
                                            toStringStart, lineno, column);
    if (!res) {
        return nullptr;
    }
=======
void LazyScript::initScript(JSScript* script) {
  MOZ_ASSERT(script);
  MOZ_ASSERT(!script_.unbarrieredGet());
  script_.set(script);
}

JS::Compartment* LazyScript::compartment() const {
  return function_->compartment();
}

Realm* LazyScript::realm() const { return function_->realm(); }

void LazyScript::setEnclosingLazyScript(LazyScript* enclosingLazyScript) {
  MOZ_ASSERT(enclosingLazyScript);
>>>>>>> upstream-releases

<<<<<<< HEAD
Realm* LazyScript::realm() const { return function_->realm(); }
||||||| merged common ancestors
    JSAtom** resClosedOverBindings = res->closedOverBindings();
    for (size_t i = 0; i < res->numClosedOverBindings(); i++) {
        resClosedOverBindings[i] = closedOverBindings[i];
    }
=======
  // We never change an existing LazyScript.
  MOZ_ASSERT(!hasEnclosingLazyScript());
>>>>>>> upstream-releases

<<<<<<< HEAD
void LazyScript::setEnclosingLazyScript(LazyScript* enclosingLazyScript) {
  MOZ_ASSERT(enclosingLazyScript);
||||||| merged common ancestors
    GCPtrFunction* resInnerFunctions = res->innerFunctions();
    for (size_t i = 0; i < res->numInnerFunctions(); i++) {
        resInnerFunctions[i].init(innerFunctions[i]);
        if (resInnerFunctions[i]->isInterpretedLazy()) {
            resInnerFunctions[i]->lazyScript()->setEnclosingLazyScript(res);
        }
    }
=======
  // Enclosing scopes never transition back to enclosing lazy scripts.
  MOZ_ASSERT(!hasEnclosingScope());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // We never change an existing LazyScript.
  MOZ_ASSERT(!hasEnclosingLazyScript());

  // Enclosing scopes never transition back to enclosing lazy scripts.
  MOZ_ASSERT(!hasEnclosingScope());

  enclosingLazyScriptOrScope_ = enclosingLazyScript;
||||||| merged common ancestors
    return res;
=======
  enclosingLazyScriptOrScope_ = enclosingLazyScript;
>>>>>>> upstream-releases
}

void LazyScript::setEnclosingScope(Scope* enclosingScope) {
  MOZ_ASSERT(enclosingScope);
  MOZ_ASSERT(!hasEnclosingScope());

  enclosingLazyScriptOrScope_ = enclosingScope;
}

ScriptSourceObject& LazyScript::sourceObject() const {
  return sourceObject_->as<ScriptSourceObject>();
}

ScriptSource* LazyScript::maybeForwardedScriptSource() const {
  JSObject* source = MaybeForwarded(&sourceObject());
  return UncheckedUnwrapWithoutExpose(source)
      ->as<ScriptSourceObject>()
      .source();
}

<<<<<<< HEAD
/* static */ LazyScript* LazyScript::CreateRaw(
    JSContext* cx, HandleFunction fun, HandleScriptSourceObject sourceObject,
    uint64_t packedFields, uint32_t sourceStart, uint32_t sourceEnd,
    uint32_t toStringStart, uint32_t lineno, uint32_t column) {
  cx->check(fun);
||||||| merged common ancestors
    GCPtrFunction* functions = res->innerFunctions();
    for (i = 0, num = res->numInnerFunctions(); i < num; i++) {
        functions[i].init(dummyFun);
    }
=======
/* static */
LazyScript* LazyScript::CreateRaw(JSContext* cx, uint32_t numClosedOverBindings,
                                  uint32_t numInnerFunctions,
                                  HandleFunction fun,
                                  HandleScriptSourceObject sourceObject,
                                  uint32_t immutableFlags, uint32_t sourceStart,
                                  uint32_t sourceEnd, uint32_t toStringStart,
                                  uint32_t lineno, uint32_t column) {
  cx->check(fun);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(sourceObject);
  union {
    PackedView p;
    uint64_t packed;
  };

  packed = packedFields;

  // Reset runtime flags to obtain a fresh LazyScript.
  p.hasBeenCloned = false;
  p.treatAsRunOnce = false;
||||||| merged common ancestors
    // Set the enclosing scope of the lazy function. This value should only be
    // set if we have a non-lazy enclosing script at this point.
    // LazyScript::enclosingScriptHasEverBeenCompiled relies on the enclosing
    // scope being non-null if we have ever been nested inside non-lazy
    // function.
    MOZ_ASSERT(!res->hasEnclosingScope());
    if (enclosingScope) {
        res->setEnclosingScope(enclosingScope);
    }
=======
  MOZ_ASSERT(sourceObject);
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t bytes = (p.numClosedOverBindings * sizeof(JSAtom*)) +
                 (p.numInnerFunctions * sizeof(GCPtrFunction));

  UniquePtr<uint8_t, JS::FreePolicy> table;
  if (bytes) {
    table.reset(cx->pod_malloc<uint8_t>(bytes));
    if (!table) {
      return nullptr;
||||||| merged common ancestors
    MOZ_ASSERT(!res->hasScript());
    if (script) {
        res->initScript(script);
=======
  // Allocate a LazyScriptData if it will not be empty. Lazy class constructors
  // also need LazyScriptData for field lists.
  Rooted<UniquePtr<LazyScriptData>> data(cx);
  if (numClosedOverBindings || numInnerFunctions || fun->isClassConstructor()) {
    data.reset(
        LazyScriptData::new_(cx, numClosedOverBindings, numInnerFunctions));
    if (!data) {
      return nullptr;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  LazyScript* res = Allocate<LazyScript>(cx);
  if (!res) {
    return nullptr;
  }

  cx->realm()->scheduleDelazificationForDebugger();

  return new (res)
      LazyScript(fun, *sourceObject, table.release(), packed, sourceStart,
                 sourceEnd, toStringStart, lineno, column);
}

/* static */ LazyScript* LazyScript::Create(
    JSContext* cx, HandleFunction fun, HandleScriptSourceObject sourceObject,
    const frontend::AtomVector& closedOverBindings,
    Handle<GCVector<JSFunction*, 8>> innerFunctions, uint32_t sourceStart,
    uint32_t sourceEnd, uint32_t toStringStart, uint32_t lineno,
    uint32_t column, frontend::ParseGoal parseGoal) {
  union {
    PackedView p;
    uint64_t packedFields;
  };

  p.shouldDeclareArguments = false;
  p.hasThisBinding = false;
  p.isAsync = false;
  p.hasRest = false;
  p.numClosedOverBindings = closedOverBindings.length();
  p.numInnerFunctions = innerFunctions.length();
  p.isGenerator = false;
  p.strict = false;
  p.bindingsAccessedDynamically = false;
  p.hasDebuggerStatement = false;
  p.hasDirectEval = false;
  p.isLikelyConstructorWrapper = false;
  p.isDerivedClassConstructor = false;
  p.needsHomeObject = false;
  p.isBinAST = false;
  p.parseGoal = uint32_t(parseGoal);

  LazyScript* res =
      LazyScript::CreateRaw(cx, fun, sourceObject, packedFields, sourceStart,
                            sourceEnd, toStringStart, lineno, column);
  if (!res) {
    return nullptr;
  }

  JSAtom** resClosedOverBindings = res->closedOverBindings();
  for (size_t i = 0; i < res->numClosedOverBindings(); i++) {
    resClosedOverBindings[i] = closedOverBindings[i];
  }

  GCPtrFunction* resInnerFunctions = res->innerFunctions();
  for (size_t i = 0; i < res->numInnerFunctions(); i++) {
    resInnerFunctions[i].init(innerFunctions[i]);
    if (resInnerFunctions[i]->isInterpretedLazy()) {
      resInnerFunctions[i]->lazyScript()->setEnclosingLazyScript(res);
||||||| merged common ancestors
    return res;
}

void
LazyScript::initRuntimeFields(uint64_t packedFields)
{
    union {
        PackedView p;
        uint64_t packed;
    };

    packed = packedFields;
    p_.hasBeenCloned = p.hasBeenCloned;
    p_.treatAsRunOnce = p.treatAsRunOnce;
}

void
JSScript::updateJitCodeRaw(JSRuntime* rt)
{
    MOZ_ASSERT(rt);
    if (hasBaselineScript() && baseline->hasPendingIonBuilder()) {
        MOZ_ASSERT(!isIonCompilingOffThread());
        jitCodeRaw_ = rt->jitRuntime()->lazyLinkStub().value;
        jitCodeSkipArgCheck_ = jitCodeRaw_;
    } else if (hasIonScript()) {
        jitCodeRaw_ = ion->method()->raw();
        jitCodeSkipArgCheck_ = jitCodeRaw_ + ion->getSkipArgCheckEntryOffset();
    } else if (hasBaselineScript()) {
        jitCodeRaw_ = baseline->method()->raw();
        jitCodeSkipArgCheck_ = jitCodeRaw_;
    } else {
        jitCodeRaw_ = rt->jitRuntime()->interpreterStub().value;
        jitCodeSkipArgCheck_ = jitCodeRaw_;
=======
  LazyScript* res = Allocate<LazyScript>(cx);
  if (!res) {
    return nullptr;
  }

  cx->realm()->scheduleDelazificationForDebugger();

  return new (res)
      LazyScript(fun, *sourceObject, data.release(), immutableFlags,
                 sourceStart, sourceEnd, toStringStart, lineno, column);
}

/* static */
LazyScript* LazyScript::Create(JSContext* cx, HandleFunction fun,
                               HandleScriptSourceObject sourceObject,
                               const frontend::AtomVector& closedOverBindings,
                               Handle<GCVector<JSFunction*, 8>> innerFunctions,
                               uint32_t sourceStart, uint32_t sourceEnd,
                               uint32_t toStringStart, uint32_t lineno,
                               uint32_t column, frontend::ParseGoal parseGoal) {
  uint32_t immutableFlags = 0;
  if (parseGoal == frontend::ParseGoal::Module) {
    immutableFlags |= uint32_t(ImmutableFlags::IsModule);
  }

  LazyScript* res = LazyScript::CreateRaw(
      cx, closedOverBindings.length(), innerFunctions.length(), fun,
      sourceObject, immutableFlags, sourceStart, sourceEnd, toStringStart,
      lineno, column);
  if (!res) {
    return nullptr;
  }

  mozilla::Span<GCPtrAtom> resClosedOverBindings = res->closedOverBindings();
  for (size_t i = 0; i < res->numClosedOverBindings(); i++) {
    resClosedOverBindings[i].init(closedOverBindings[i]);
  }

  mozilla::Span<GCPtrFunction> resInnerFunctions = res->innerFunctions();
  for (size_t i = 0; i < res->numInnerFunctions(); i++) {
    resInnerFunctions[i].init(innerFunctions[i]);
    if (resInnerFunctions[i]->isInterpretedLazy()) {
      resInnerFunctions[i]->lazyScript()->setEnclosingLazyScript(res);
    }
  }

  return res;
}

/* static */
LazyScript* LazyScript::CreateForXDR(
    JSContext* cx, uint32_t numClosedOverBindings, uint32_t numInnerFunctions,
    HandleFunction fun, HandleScript script, HandleScope enclosingScope,
    HandleScriptSourceObject sourceObject, uint32_t immutableFlags,
    uint32_t sourceStart, uint32_t sourceEnd, uint32_t toStringStart,
    uint32_t toStringEnd, uint32_t lineno, uint32_t column) {
  LazyScript* res = LazyScript::CreateRaw(
      cx, numClosedOverBindings, numInnerFunctions, fun, sourceObject,
      immutableFlags, sourceStart, sourceEnd, toStringStart, lineno, column);
  if (!res) {
    return nullptr;
  }

  res->setToStringEnd(toStringEnd);

  // Set the enclosing scope of the lazy function. This value should only be
  // set if we have a non-lazy enclosing script at this point.
  // LazyScript::enclosingScriptHasEverBeenCompiled relies on the enclosing
  // scope being non-null if we have ever been nested inside non-lazy
  // function.
  MOZ_ASSERT(!res->hasEnclosingScope());
  if (enclosingScope) {
    res->setEnclosingScope(enclosingScope);
  }

  MOZ_ASSERT(!res->hasScript());
  if (script) {
    res->initScript(script);
  }

  return res;
}

void JSScript::updateJitCodeRaw(JSRuntime* rt) {
  MOZ_ASSERT(rt);
  uint8_t* jitCodeSkipArgCheck;
  if (hasBaselineScript() && baseline->hasPendingIonBuilder()) {
    MOZ_ASSERT(!isIonCompilingOffThread());
    jitCodeRaw_ = rt->jitRuntime()->lazyLinkStub().value;
    jitCodeSkipArgCheck = jitCodeRaw_;
  } else if (hasIonScript()) {
    jitCodeRaw_ = ion->method()->raw();
    jitCodeSkipArgCheck = jitCodeRaw_ + ion->getSkipArgCheckEntryOffset();
  } else if (hasBaselineScript()) {
    jitCodeRaw_ = baseline->method()->raw();
    jitCodeSkipArgCheck = jitCodeRaw_;
  } else if (jitScript() && js::jit::JitOptions.baselineInterpreter) {
    jitCodeRaw_ = rt->jitRuntime()->baselineInterpreter().codeRaw();
    jitCodeSkipArgCheck = jitCodeRaw_;
  } else {
    jitCodeRaw_ = rt->jitRuntime()->interpreterStub().value;
    jitCodeSkipArgCheck = jitCodeRaw_;
  }
  MOZ_ASSERT(jitCodeRaw_);
  MOZ_ASSERT(jitCodeSkipArgCheck);

  if (jitScript_) {
    jitScript_->jitCodeSkipArgCheck_ = jitCodeSkipArgCheck;
  }
}

bool JSScript::hasLoops() {
  if (!hasTrynotes()) {
    return false;
  }
  for (const JSTryNote& tn : trynotes()) {
    switch (tn.kind) {
      case JSTRY_FOR_IN:
      case JSTRY_FOR_OF:
      case JSTRY_LOOP:
        return true;
      case JSTRY_CATCH:
      case JSTRY_FINALLY:
      case JSTRY_FOR_OF_ITERCLOSE:
      case JSTRY_DESTRUCTURING:
        break;
      default:
        MOZ_ASSERT(false, "Add new try note type to JSScript::hasLoops");
        break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

  return res;
||||||| merged common ancestors
    MOZ_ASSERT(jitCodeRaw_);
    MOZ_ASSERT(jitCodeSkipArgCheck_);
=======
  }
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ LazyScript* LazyScript::CreateForXDR(
    JSContext* cx, HandleFunction fun, HandleScript script,
    HandleScope enclosingScope, HandleScriptSourceObject sourceObject,
    uint64_t packedFields, uint32_t sourceStart, uint32_t sourceEnd,
    uint32_t toStringStart, uint32_t lineno, uint32_t column) {
  // Dummy atom which is not a valid property name.
  RootedAtom dummyAtom(cx, cx->names().comma);

  // Dummy function which is not a valid function as this is the one which is
  // holding this lazy script.
  HandleFunction dummyFun = fun;

  LazyScript* res =
      LazyScript::CreateRaw(cx, fun, sourceObject, packedFields, sourceStart,
                            sourceEnd, toStringStart, lineno, column);
  if (!res) {
    return nullptr;
  }

  // Fill with dummies, to be GC-safe after the initialization of the free
  // variables and inner functions.
  size_t i, num;
  JSAtom** closedOverBindings = res->closedOverBindings();
  for (i = 0, num = res->numClosedOverBindings(); i < num; i++) {
    closedOverBindings[i] = dummyAtom;
  }

  GCPtrFunction* functions = res->innerFunctions();
  for (i = 0, num = res->numInnerFunctions(); i < num; i++) {
    functions[i].init(dummyFun);
  }

  // Set the enclosing scope of the lazy function. This value should only be
  // set if we have a non-lazy enclosing script at this point.
  // LazyScript::enclosingScriptHasEverBeenCompiled relies on the enclosing
  // scope being non-null if we have ever been nested inside non-lazy
  // function.
  MOZ_ASSERT(!res->hasEnclosingScope());
  if (enclosingScope) {
    res->setEnclosingScope(enclosingScope);
  }

  MOZ_ASSERT(!res->hasScript());
  if (script) {
    res->initScript(script);
  }

  return res;
}

void LazyScript::initRuntimeFields(uint64_t packedFields) {
  union {
    PackedView p;
    uint64_t packed;
  };

  packed = packedFields;
  p_.hasBeenCloned = p.hasBeenCloned;
  p_.treatAsRunOnce = p.treatAsRunOnce;
}

void JSScript::updateJitCodeRaw(JSRuntime* rt) {
  MOZ_ASSERT(rt);
  if (hasBaselineScript() && baseline->hasPendingIonBuilder()) {
    MOZ_ASSERT(!isIonCompilingOffThread());
    jitCodeRaw_ = rt->jitRuntime()->lazyLinkStub().value;
    jitCodeSkipArgCheck_ = jitCodeRaw_;
  } else if (hasIonScript()) {
    jitCodeRaw_ = ion->method()->raw();
    jitCodeSkipArgCheck_ = jitCodeRaw_ + ion->getSkipArgCheckEntryOffset();
  } else if (hasBaselineScript()) {
    jitCodeRaw_ = baseline->method()->raw();
    jitCodeSkipArgCheck_ = jitCodeRaw_;
  } else {
    jitCodeRaw_ = rt->jitRuntime()->interpreterStub().value;
    jitCodeSkipArgCheck_ = jitCodeRaw_;
  }
  MOZ_ASSERT(jitCodeRaw_);
  MOZ_ASSERT(jitCodeSkipArgCheck_);
}

bool JSScript::hasLoops() {
  if (!hasTrynotes()) {
    return false;
  }
  for (const JSTryNote& tn : trynotes()) {
    switch (tn.kind) {
      case JSTRY_FOR_IN:
      case JSTRY_FOR_OF:
      case JSTRY_LOOP:
        return true;
      case JSTRY_CATCH:
      case JSTRY_FINALLY:
      case JSTRY_FOR_OF_ITERCLOSE:
      case JSTRY_DESTRUCTURING_ITERCLOSE:
        break;
      default:
        MOZ_ASSERT(false, "Add new try note type to JSScript::hasLoops");
        break;
    }
  }
  return false;
||||||| merged common ancestors
bool
JSScript::hasLoops()
{
    if (!hasTrynotes()) {
        return false;
    }
    for (const JSTryNote& tn : trynotes()) {
        switch (tn.kind) {
          case JSTRY_FOR_IN:
          case JSTRY_FOR_OF:
          case JSTRY_LOOP:
            return true;
          case JSTRY_CATCH:
          case JSTRY_FINALLY:
          case JSTRY_FOR_OF_ITERCLOSE:
          case JSTRY_DESTRUCTURING_ITERCLOSE:
            break;
          default:
            MOZ_ASSERT(false, "Add new try note type to JSScript::hasLoops");
            break;
        }
    }
    return false;
=======
bool JSScript::mayReadFrameArgsDirectly() {
  return argumentsHasVarBinding() || hasRest();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool JSScript::mayReadFrameArgsDirectly() {
  return argumentsHasVarBinding() || hasRest();
||||||| merged common ancestors
bool
JSScript::mayReadFrameArgsDirectly()
{
    return argumentsHasVarBinding() || hasRest();
=======
void JSScript::resetWarmUpCounterToDelayIonCompilation() {
  // Reset the warm-up count only if it's greater than the BaselineCompiler
  // threshold. We do this to ensure this has no effect on Baseline compilation
  // because we don't want scripts to get stuck in the (Baseline) interpreter in
  // pathological cases.

  if (warmUpCount > jit::JitOptions.baselineWarmUpThreshold) {
    incWarmUpResetCounter();
    warmUpCount = jit::JitOptions.baselineWarmUpThreshold;
  }
>>>>>>> upstream-releases
}

void JSScript::AutoDelazify::holdScript(JS::HandleFunction fun) {
  if (fun) {
    if (fun->realm()->isSelfHostingRealm()) {
      // The self-hosting realm is shared across runtimes, so we can't use
      // JSAutoRealm: it could cause races. Functions in the self-hosting
      // realm will never be lazy, so we can safely assume we don't have
      // to delazify.
      script_ = fun->nonLazyScript();
    } else {
      JSAutoRealm ar(cx_, fun);
      script_ = JSFunction::getOrCreateScript(cx_, fun);
      if (script_) {
        oldDoNotRelazify_ = script_->hasFlag(MutableFlags::DoNotRelazify);
        script_->setDoNotRelazify(true);
      }
    }
  }
}

void JSScript::AutoDelazify::dropScript() {
  // Don't touch script_ if it's in the self-hosting realm, see the comment
  // in holdScript.
  if (script_ && !script_->realm()->isSelfHostingRealm()) {
    script_->setDoNotRelazify(oldDoNotRelazify_);
  }
  script_ = nullptr;
}

JS::ubi::Base::Size JS::ubi::Concrete<JSScript>::size(
    mozilla::MallocSizeOf mallocSizeOf) const {
  Size size = gc::Arena::thingSize(get().asTenured().getAllocKind());

<<<<<<< HEAD
  size += get().sizeOfData(mallocSizeOf);
  size += get().sizeOfTypeScript(mallocSizeOf);
||||||| merged common ancestors
    size += get().sizeOfData(mallocSizeOf);
    size += get().sizeOfTypeScript(mallocSizeOf);
=======
  size += get().sizeOfData(mallocSizeOf);
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t baselineSize = 0;
  size_t baselineStubsSize = 0;
  jit::AddSizeOfBaselineData(&get(), mallocSizeOf, &baselineSize,
                             &baselineStubsSize);
  size += baselineSize;
  size += baselineStubsSize;
||||||| merged common ancestors
    size_t baselineSize = 0;
    size_t baselineStubsSize = 0;
    jit::AddSizeOfBaselineData(&get(), mallocSizeOf, &baselineSize, &baselineStubsSize);
    size += baselineSize;
    size += baselineStubsSize;
=======
  size_t jitScriptSize = 0;
  size_t fallbackStubSize = 0;
  get().addSizeOfJitScript(mallocSizeOf, &jitScriptSize, &fallbackStubSize);
  size += jitScriptSize;
  size += fallbackStubSize;
>>>>>>> upstream-releases

<<<<<<< HEAD
  size += jit::SizeOfIonData(&get(), mallocSizeOf);
||||||| merged common ancestors
    size += jit::SizeOfIonData(&get(), mallocSizeOf);
=======
  size_t baselineSize = 0;
  jit::AddSizeOfBaselineData(&get(), mallocSizeOf, &baselineSize);
  size += baselineSize;
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(size > 0);
  return size;
||||||| merged common ancestors
    MOZ_ASSERT(size > 0);
    return size;
=======
  size += jit::SizeOfIonData(&get(), mallocSizeOf);

  MOZ_ASSERT(size > 0);
  return size;
>>>>>>> upstream-releases
}

const char* JS::ubi::Concrete<JSScript>::scriptFilename() const {
  return get().filename();
}

JS::ubi::Node::Size JS::ubi::Concrete<js::LazyScript>::size(
    mozilla::MallocSizeOf mallocSizeOf) const {
  Size size = gc::Arena::thingSize(get().asTenured().getAllocKind());
  size += get().sizeOfExcludingThis(mallocSizeOf);
  return size;
}

const char* JS::ubi::Concrete<js::LazyScript>::scriptFilename() const {
  auto source = get().sourceObject().source();
  if (!source) {
    return nullptr;
  }

  return source->filename();
}
