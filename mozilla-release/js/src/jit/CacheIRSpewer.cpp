/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifdef JS_CACHEIR_SPEW

#  include "jit/CacheIRSpewer.h"

#  include "mozilla/Sprintf.h"

#  ifdef XP_WIN
#    include <process.h>
#    define getpid _getpid
#  else
#    include <unistd.h>
#  endif
#  include <stdarg.h>

#  include "util/Text.h"
#  include "vm/JSFunction.h"
#  include "vm/JSObject.h"
#  include "vm/JSScript.h"

#  include "vm/JSObject-inl.h"
#  include "vm/Realm-inl.h"

using namespace js;
using namespace js::jit;

CacheIRSpewer CacheIRSpewer::cacheIRspewer;

CacheIRSpewer::CacheIRSpewer()
<<<<<<< HEAD
    : outputLock(mutexid::CacheIRSpewer), guardCount_(0) {
  spewInterval_ =
      getenv("CACHEIR_LOG_FLUSH") ? atoi(getenv("CACHEIR_LOG_FLUSH")) : 10000;
||||||| merged common ancestors
  : outputLock(mutexid::CacheIRSpewer)
{ }
=======
    : outputLock_(mutexid::CacheIRSpewer), guardCount_(0) {
  spewInterval_ =
      getenv("CACHEIR_LOG_FLUSH") ? atoi(getenv("CACHEIR_LOG_FLUSH")) : 10000;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (spewInterval_ < 1) {
    spewInterval_ = 1;
  }
}

CacheIRSpewer::~CacheIRSpewer() {
  if (!enabled()) {
    return;
  }

  json.ref().endList();
  output.flush();
  output.finish();
||||||| merged common ancestors
CacheIRSpewer::~CacheIRSpewer()
{
    if (!enabled()) {
        return;
    }

    json.ref().endList();
    output.flush();
    output.finish();
=======
  if (spewInterval_ < 1) {
    spewInterval_ = 1;
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
#ifndef JIT_SPEW_DIR
#if defined(_WIN32)
#define JIT_SPEW_DIR "."
#elif defined(__ANDROID__)
#define JIT_SPEW_DIR "/data/local/tmp"
#else
#define JIT_SPEW_DIR "/tmp"
#endif
#endif

bool CacheIRSpewer::init(const char* filename) {
  if (enabled()) {
||||||| merged common ancestors
#ifndef JIT_SPEW_DIR
# if defined(_WIN32)
#  define JIT_SPEW_DIR "."
# elif defined(__ANDROID__)
#  define JIT_SPEW_DIR "/data/local/tmp"
# else
#  define JIT_SPEW_DIR "/tmp"
# endif
#endif

bool
CacheIRSpewer::init(const char* filename)
{
    if (enabled()) {
        return true;
    }

    char name[256];
    uint32_t pid = getpid();
    // Default to JIT_SPEW_DIR/cacheir${pid}.json
    if (filename[0] == '1') {
        SprintfLiteral(name, JIT_SPEW_DIR "/cacheir%" PRIu32 ".json", pid);
    } else {
        SprintfLiteral(name, "%s%" PRIu32 ".json", filename, pid);
    }

    if (!output.init(name)) {
        return false;
    }
    output.put("[");

    json.emplace(output);
=======
CacheIRSpewer::~CacheIRSpewer() {
  if (!enabled()) {
    return;
  }

  json_.ref().endList();
  output_.flush();
  output_.finish();
}

#  ifndef JIT_SPEW_DIR
#    if defined(_WIN32)
#      define JIT_SPEW_DIR "."
#    elif defined(__ANDROID__)
#      define JIT_SPEW_DIR "/data/local/tmp"
#    else
#      define JIT_SPEW_DIR "/tmp"
#    endif
#  endif

bool CacheIRSpewer::init(const char* filename) {
  if (enabled()) {
>>>>>>> upstream-releases
    return true;
<<<<<<< HEAD
  }

  char name[256];
  uint32_t pid = getpid();
  // Default to JIT_SPEW_DIR/cacheir${pid}.json
  if (filename[0] == '1') {
    SprintfLiteral(name, JIT_SPEW_DIR "/cacheir%" PRIu32 ".json", pid);
  } else {
    SprintfLiteral(name, "%s%" PRIu32 ".json", filename, pid);
  }

  if (!output.init(name)) {
    return false;
  }
  output.put("[");

  json.emplace(output);
  return true;
||||||| merged common ancestors
=======
  }

  char name[256];
  uint32_t pid = getpid();
  // Default to JIT_SPEW_DIR/cacheir${pid}.json
  if (filename[0] == '1') {
    SprintfLiteral(name, JIT_SPEW_DIR "/cacheir%" PRIu32 ".json", pid);
  } else {
    SprintfLiteral(name, "%s%" PRIu32 ".json", filename, pid);
  }

  if (!output_.init(name)) {
    return false;
  }
  output_.put("[");

  json_.emplace(output_);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void CacheIRSpewer::beginCache(const IRGenerator& gen) {
  MOZ_ASSERT(enabled());
  JSONPrinter& j = json.ref();
  const char* filename = gen.script_->filename();
  j.beginObject();
  j.property("name", CacheKindNames[uint8_t(gen.cacheKind_)]);
  j.property("file", filename ? filename : "null");
  j.property("mode", int(gen.mode_));
  if (jsbytecode* pc = gen.pc_) {
    unsigned column;
    j.property("line", PCToLineNumber(gen.script_, pc, &column));
    j.property("column", column);
    j.formatProperty("pc", "%p", pc);
  }
||||||| merged common ancestors
void
CacheIRSpewer::beginCache(const IRGenerator& gen)
{
    MOZ_ASSERT(enabled());
    JSONPrinter& j = json.ref();
    const char* filename = gen.script_->filename();
    j.beginObject();
    j.property("name", CacheKindNames[uint8_t(gen.cacheKind_)]);
    j.property("file", filename ? filename : "null");
    j.property("mode", int(gen.mode_));
    if (jsbytecode* pc = gen.pc_) {
        unsigned column;
        j.property("line", PCToLineNumber(gen.script_, pc, &column));
        j.property("column", column);
        j.formatProperty("pc", "%p", pc);
    }
=======
void CacheIRSpewer::beginCache(const IRGenerator& gen) {
  MOZ_ASSERT(enabled());
  JSONPrinter& j = json_.ref();
  const char* filename = gen.script_->filename();
  j.beginObject();
  j.property("name", CacheKindNames[uint8_t(gen.cacheKind_)]);
  j.property("file", filename ? filename : "null");
  j.property("mode", int(gen.mode_));
  if (jsbytecode* pc = gen.pc_) {
    unsigned column;
    j.property("line", PCToLineNumber(gen.script_, pc, &column));
    j.property("column", column);
    j.formatProperty("pc", "%p", pc);
  }
>>>>>>> upstream-releases
}

template <typename CharT>
<<<<<<< HEAD
static void QuoteString(GenericPrinter& out, const CharT* s, size_t length) {
  const CharT* end = s + length;
  for (const CharT* t = s; t < end; s = ++t) {
    // This quote implementation is probably correct,
    // but uses \u even when not strictly necessary.
    char16_t c = *t;
    if (c == '"' || c == '\\') {
      out.printf("\\");
      out.printf("%c", char(c));
    } else if (c < ' ' || c >= 127 || !isprint(c)) {
      out.printf("\\u%04x", c);
    } else {
      out.printf("%c", char(c));
    }
  }
||||||| merged common ancestors
static void
QuoteString(GenericPrinter& out, const CharT* s, size_t length)
{
    const CharT* end = s + length;
    for (const CharT* t = s; t < end; s = ++t) {
        // This quote implementation is probably correct,
        // but uses \u even when not strictly necessary.
        char16_t c = *t;
        if (c == '"' || c == '\\') {
            out.printf("\\");
            out.printf("%c", char(c));
        } else if (c < ' ' || c >= 127 || !isprint(c)) {
            out.printf("\\u%04x", c);
        } else {
            out.printf("%c", char(c));
        }
    }
}

static void
QuoteString(GenericPrinter& out, JSLinearString* str)
{
    JS::AutoCheckCannotGC nogc;
    if (str->hasLatin1Chars()) {
        QuoteString(out, str->latin1Chars(nogc), str->length());
    } else {
        QuoteString(out, str->twoByteChars(nogc), str->length());
    }
=======
static void QuoteString(GenericPrinter& out, const CharT* s, size_t length) {
  const CharT* end = s + length;
  for (const CharT* t = s; t < end; s = ++t) {
    // This quote implementation is probably correct,
    // but uses \u even when not strictly necessary.
    char16_t c = *t;
    if (c == '"' || c == '\\') {
      out.printf("\\");
      out.printf("%c", char(c));
    } else if (!IsAsciiPrintable(c)) {
      out.printf("\\u%04x", c);
    } else {
      out.printf("%c", char(c));
    }
  }
}

static void QuoteString(GenericPrinter& out, JSLinearString* str) {
  JS::AutoCheckCannotGC nogc;
  if (str->hasLatin1Chars()) {
    QuoteString(out, str->latin1Chars(nogc), str->length());
  } else {
    QuoteString(out, str->twoByteChars(nogc), str->length());
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void QuoteString(GenericPrinter& out, JSLinearString* str) {
  JS::AutoCheckCannotGC nogc;
  if (str->hasLatin1Chars()) {
    QuoteString(out, str->latin1Chars(nogc), str->length());
  } else {
    QuoteString(out, str->twoByteChars(nogc), str->length());
  }
}
||||||| merged common ancestors
void
CacheIRSpewer::valueProperty(const char* name, const Value& v)
{
    MOZ_ASSERT(enabled());
    JSONPrinter& j = json.ref();

    j.beginObjectProperty(name);
=======
void CacheIRSpewer::valueProperty(const char* name, const Value& v) {
  MOZ_ASSERT(enabled());
  JSONPrinter& j = json_.ref();

  j.beginObjectProperty(name);
>>>>>>> upstream-releases

<<<<<<< HEAD
void CacheIRSpewer::valueProperty(const char* name, const Value& v) {
  MOZ_ASSERT(enabled());
  JSONPrinter& j = json.ref();

  j.beginObjectProperty(name);

  const char* type = InformalValueTypeName(v);
  if (v.isInt32()) {
    type = "int32";
  }
  j.property("type", type);

  if (v.isInt32()) {
    j.property("value", v.toInt32());
  } else if (v.isDouble()) {
    j.floatProperty("value", v.toDouble(), 3);
  } else if (v.isString() || v.isSymbol()) {
    JSString* str = v.isString() ? v.toString() : v.toSymbol()->description();
    if (str && str->isLinear()) {
      j.beginStringProperty("value");
      QuoteString(output, &str->asLinear());
      j.endStringProperty();
||||||| merged common ancestors
    const char* type = InformalValueTypeName(v);
    if (v.isInt32()) {
        type = "int32";
=======
  const char* type = InformalValueTypeName(v);
  if (v.isInt32()) {
    type = "int32";
  }
  j.property("type", type);

  if (v.isInt32()) {
    j.property("value", v.toInt32());
  } else if (v.isDouble()) {
    j.floatProperty("value", v.toDouble(), 3);
  } else if (v.isString() || v.isSymbol()) {
    JSString* str = v.isString() ? v.toString() : v.toSymbol()->description();
    if (str && str->isLinear()) {
      j.beginStringProperty("value");
      QuoteString(output_, &str->asLinear());
      j.endStringProperty();
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  } else if (v.isObject()) {
    JSObject& object = v.toObject();
    j.formatProperty("value", "%p (shape: %p)", &object, object.maybeShape());
    if (NativeObject* nobj =
            object.isNative() ? &object.as<NativeObject>() : nullptr) {
      j.beginListProperty("flags");
      {
        if (nobj->isIndexed()) {
          j.value("indexed");
        }
        if (nobj->inDictionaryMode()) {
          j.value("dictionaryMode");
||||||| merged common ancestors
    j.property("type", type);

    if (v.isInt32()) {
        j.property("value", v.toInt32());
    } else if (v.isDouble()) {
        j.floatProperty("value", v.toDouble(), 3);
    } else if (v.isString() || v.isSymbol()) {
        JSString* str = v.isString() ? v.toString() : v.toSymbol()->description();
        if (str && str->isLinear()) {
            j.beginStringProperty("value");
            QuoteString(output, &str->asLinear());
            j.endStringProperty();
=======
  } else if (v.isObject()) {
    JSObject& object = v.toObject();
    j.formatProperty("value", "%p (shape: %p)", &object, object.shape());
    if (NativeObject* nobj =
            object.isNative() ? &object.as<NativeObject>() : nullptr) {
      j.beginListProperty("flags");
      {
        if (nobj->isIndexed()) {
          j.value("indexed");
        }
        if (nobj->inDictionaryMode()) {
          j.value("dictionaryMode");
>>>>>>> upstream-releases
        }
      }
      j.endList();
      if (nobj->isIndexed()) {
        j.beginObjectProperty("indexed");
        {
          j.property("denseInitializedLength",
                     nobj->getDenseInitializedLength());
          j.property("denseCapacity", nobj->getDenseCapacity());
          j.property("denseElementsAreSealed", nobj->denseElementsAreSealed());
          j.property("denseElementsAreCopyOnWrite",
                     nobj->denseElementsAreCopyOnWrite());
          j.property("denseElementsAreFrozen", nobj->denseElementsAreFrozen());
        }
        j.endObject();
      }
    }
  }

<<<<<<< HEAD
  j.endObject();
||||||| merged common ancestors
    j.endObject();
=======
  j.endObject();
}

void CacheIRSpewer::opcodeProperty(const char* name, const JSOp op) {
  MOZ_ASSERT(enabled());
  JSONPrinter& j = json_.ref();

  j.beginStringProperty(name);
  output_.put(CodeName[op]);
  j.endStringProperty();
}

void CacheIRSpewer::CacheIRArgs(JSONPrinter& j, CacheIRReader& r,
                                CacheIROpFormat::ArgType arg) {
  j.beginObject();
  switch (arg) {
    case CacheIROpFormat::None:
      break;
    case CacheIROpFormat::Id:
      j.property("Id", r.readByte());
      break;
    case CacheIROpFormat::Field:
      j.property("Field", r.readByte());
      break;
    case CacheIROpFormat::Byte:
      j.property("Byte", r.readByte());
      break;
    case CacheIROpFormat::Int32:
      j.property("Int32", r.int32Immediate());
      break;
    case CacheIROpFormat::UInt32:
      j.property("Uint32", r.uint32Immediate());
      break;
    case CacheIROpFormat::Word:
      j.property("Word", uintptr_t(r.pointer()));
      break;
  }
  j.endObject();
>>>>>>> upstream-releases
}
template <typename... Args>
void CacheIRSpewer::CacheIRArgs(JSONPrinter& j, CacheIRReader& r,
                                CacheIROpFormat::ArgType arg, Args... args) {
  using namespace js::jit::CacheIROpFormat;

<<<<<<< HEAD
void CacheIRSpewer::opcodeProperty(const char* name, const JSOp op) {
  MOZ_ASSERT(enabled());
  JSONPrinter& j = json.ref();
||||||| merged common ancestors
void
CacheIRSpewer::opcodeProperty(const char* name, const JSOp op)
{
    MOZ_ASSERT(enabled());
    JSONPrinter& j = json.ref();
=======
  CacheIRArgs(j, r, arg);
  CacheIRArgs(j, r, args...);
}

void CacheIRSpewer::cacheIRSequence(CacheIRReader& reader) {
  using namespace js::jit::CacheIROpFormat;

  MOZ_ASSERT(enabled());
  JSONPrinter& j = json_.ref();

  j.beginListProperty("cacheIR");
  while (reader.more()) {
    j.beginObject();
    CacheOp op = reader.readOp();
    j.property("op", CacheIrOpNames[uint32_t(op)]);
    j.beginListProperty("args");

    switch (op) {
#  define DEFINE_OP(op, ...)               \
    case CacheOp::op:                      \
      CacheIRArgs(j, reader, __VA_ARGS__); \
      break;
      CACHE_IR_OPS(DEFINE_OP)
#  undef DEFINE_OP
      default:
        MOZ_CRASH("unreachable");
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  j.beginStringProperty(name);
  output.put(CodeName[op]);
  j.endStringProperty();
||||||| merged common ancestors
    j.beginStringProperty(name);
    output.put(CodeName[op]);
    j.endStringProperty();
=======
    j.endList();
    j.endObject();
  }
  j.endList();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void CacheIRSpewer::attached(const char* name) {
  MOZ_ASSERT(enabled());
  json.ref().property("attached", name);
||||||| merged common ancestors
void
CacheIRSpewer::attached(const char* name)
{
    MOZ_ASSERT(enabled());
    json.ref().property("attached", name);
=======
void CacheIRSpewer::attached(const char* name) {
  MOZ_ASSERT(enabled());
  json_.ref().property("attached", name);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void CacheIRSpewer::endCache() {
  MOZ_ASSERT(enabled());
  json.ref().endObject();
||||||| merged common ancestors
void
CacheIRSpewer::endCache()
{
    MOZ_ASSERT(enabled());
    json.ref().endObject();
=======
void CacheIRSpewer::endCache() {
  MOZ_ASSERT(enabled());
  json_.ref().endObject();
>>>>>>> upstream-releases
}

#endif /* JS_CACHEIR_SPEW */
