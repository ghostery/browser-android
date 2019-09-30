/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* Same-thread compilation and evaluation APIs. */

#include "js/CompilationAndEvaluation.h"

#include "mozilla/Maybe.h"      // mozilla::None, mozilla::Some
#include "mozilla/TextUtils.h"  // mozilla::IsAscii
#include "mozilla/Utf8.h"       // mozilla::Utf8Unit

#include <utility>  // std::move

#include "jsfriendapi.h"  // js::GetErrorMessage
#include "jstypes.h"      // JS_PUBLIC_API

<<<<<<< HEAD
#include "frontend/BytecodeCompilation.h"  // frontend::CompileGlobalScript
#include "frontend/FullParseHandler.h"     // frontend::FullParseHandler
#include "frontend/ParseContext.h"         // frontend::UsedNameTracker
#include "frontend/Parser.h"       // frontend::Parser, frontend::ParseGoal
#include "js/CharacterEncoding.h"  // JS::UTF8Chars, JS::UTF8CharsToNewTwoByteCharsZ
#include "js/RootingAPI.h"         // JS::Rooted
#include "js/SourceText.h"         // JS::SourceText
#include "js/TypeDecls.h"          // JS::HandleObject, JS::MutableHandleScript
#include "js/Utility.h"            // JS::UniqueTwoByteChars
#include "js/Value.h"              // JS::Value
#include "util/CompleteFile.h"     // js::FileContents, js::ReadCompleteFile
#include "util/StringBuffer.h"     // js::StringBuffer
#include "vm/Debugger.h"           // js::Debugger
#include "vm/EnvironmentObject.h"  // js::CreateNonSyntacticEnvironmentChain
#include "vm/Interpreter.h"        // js::Execute
#include "vm/JSContext.h"          // JSContext
||||||| merged common ancestors
#include "frontend/FullParseHandler.h" // frontend::FullParseHandler
#include "frontend/ParseContext.h" // frontend::UsedNameTracker
#include "frontend/Parser.h" // frontend::Parser, frontend::ParseGoal
#include "js/CharacterEncoding.h" // JS::UTF8Chars, JS::UTF8CharsToNewTwoByteCharsZ
#include "js/RootingAPI.h" // JS::Rooted
#include "js/SourceBufferHolder.h" // JS::SourceBufferHolder
#include "js/TypeDecls.h" // JS::HandleObject, JS::MutableHandleScript
#include "js/Value.h" // JS::Value
#include "util/CompleteFile.h" // js::FileContents, js::ReadCompleteFile
#include "util/StringBuffer.h" // js::StringBuffer
#include "vm/Debugger.h" // js::Debugger
#include "vm/EnvironmentObject.h" // js::CreateNonSyntacticEnvironmentChain
#include "vm/Interpreter.h" // js::Execute
#include "vm/JSContext.h" // JSContext
=======
#include "frontend/BytecodeCompilation.h"  // frontend::CompileGlobalScript
#include "frontend/FullParseHandler.h"     // frontend::FullParseHandler
#include "frontend/ParseContext.h"         // frontend::UsedNameTracker
#include "frontend/Parser.h"       // frontend::Parser, frontend::ParseGoal
#include "js/CharacterEncoding.h"  // JS::UTF8Chars, JS::UTF8CharsToNewTwoByteCharsZ
#include "js/RootingAPI.h"         // JS::Rooted
#include "js/SourceText.h"         // JS::SourceText
#include "js/TypeDecls.h"          // JS::HandleObject, JS::MutableHandleScript
#include "js/Utility.h"            // js::MallocArena, JS::UniqueTwoByteChars
#include "js/Value.h"              // JS::Value
#include "util/CompleteFile.h"     // js::FileContents, js::ReadCompleteFile
#include "util/StringBuffer.h"     // js::StringBuffer
#include "vm/Debugger.h"           // js::Debugger
#include "vm/EnvironmentObject.h"  // js::CreateNonSyntacticEnvironmentChain
#include "vm/Interpreter.h"        // js::Execute
#include "vm/JSContext.h"          // JSContext
>>>>>>> upstream-releases

#include "vm/JSContext-inl.h"  // JSContext::check

using mozilla::Utf8Unit;

using JS::CompileOptions;
using JS::HandleObject;
using JS::ReadOnlyCompileOptions;
using JS::SourceOwnership;
using JS::SourceText;
using JS::UniqueTwoByteChars;
using JS::UTF8Chars;
using JS::UTF8CharsToNewTwoByteCharsZ;

using namespace js;

<<<<<<< HEAD
JS_PUBLIC_API void JS::detail::ReportSourceTooLong(JSContext* cx) {
  JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                            JSMSG_SOURCE_TOO_LONG);
}

template <typename Unit>
static bool CompileSourceBuffer(JSContext* cx,
                                const ReadOnlyCompileOptions& options,
                                SourceText<Unit>& srcBuf,
                                JS::MutableHandleScript script) {
  ScopeKind scopeKind =
      options.nonSyntacticScope ? ScopeKind::NonSyntactic : ScopeKind::Global;
||||||| merged common ancestors
static bool
CompileSourceBuffer(JSContext* cx, const ReadOnlyCompileOptions& options,
                    SourceBufferHolder& srcBuf, JS::MutableHandleScript script)
{
    ScopeKind scopeKind = options.nonSyntacticScope ? ScopeKind::NonSyntactic : ScopeKind::Global;
=======
JS_PUBLIC_API void JS::detail::ReportSourceTooLong(JSContext* cx) {
  JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                            JSMSG_SOURCE_TOO_LONG);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(!cx->zone()->isAtomsZone());
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
||||||| merged common ancestors
    MOZ_ASSERT(!cx->zone()->isAtomsZone());
    AssertHeapIsIdle();
    CHECK_THREAD(cx);
=======
template <typename Unit>
static JSScript* CompileSourceBuffer(JSContext* cx,
                                     const ReadOnlyCompileOptions& options,
                                     SourceText<Unit>& srcBuf) {
  ScopeKind scopeKind =
      options.nonSyntacticScope ? ScopeKind::NonSyntactic : ScopeKind::Global;

  MOZ_ASSERT(!cx->zone()->isAtomsZone());
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  frontend::GlobalScriptInfo info(cx, options, scopeKind);
  script.set(frontend::CompileGlobalScript(info, srcBuf));
  return !!script;
||||||| merged common ancestors
    script.set(frontend::CompileGlobalScript(cx, cx->tempLifoAlloc(), scopeKind, options, srcBuf));
    return !!script;
=======
  frontend::GlobalScriptInfo info(cx, options, scopeKind);
  return frontend::CompileGlobalScript(info, srcBuf);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CompileLatin1(JSContext* cx, const ReadOnlyCompileOptions& options,
                          const char* bytes, size_t length,
                          JS::MutableHandleScript script) {
  auto chars = UniqueTwoByteChars(InflateString(cx, bytes, length));
  if (!chars) {
    return false;
  }

  SourceText<char16_t> source;
  if (!source.init(cx, std::move(chars), length)) {
    return false;
  }
||||||| merged common ancestors
static bool
CompileLatin1(JSContext* cx, const ReadOnlyCompileOptions& options,
              const char* bytes, size_t length, JS::MutableHandleScript script)
{
    char16_t* chars = InflateString(cx, bytes, length);
    if (!chars) {
        return false;
    }
=======
static JSScript* CompileUtf8Inflating(JSContext* cx,
                                      const ReadOnlyCompileOptions& options,
                                      SourceText<Utf8Unit>& srcBuf) {
  auto bytes = srcBuf.get();
  size_t length = srcBuf.length();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return CompileSourceBuffer(cx, options, source, script);
}
||||||| merged common ancestors
    SourceBufferHolder source(chars, length, SourceBufferHolder::GiveOwnership);
    return CompileSourceBuffer(cx, options, source, script);
}
=======
  auto chars = UniqueTwoByteChars(
      UTF8CharsToNewTwoByteCharsZ(cx, UTF8Chars(bytes, length), &length,
                                  js::MallocArena)
          .get());
  if (!chars) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool CompileUtf8(JSContext* cx, const ReadOnlyCompileOptions& options,
                        const char* bytes, size_t length,
                        JS::MutableHandleScript script) {
  auto chars = UniqueTwoByteChars(
      UTF8CharsToNewTwoByteCharsZ(cx, UTF8Chars(bytes, length), &length).get());
  if (!chars) {
    return false;
  }
||||||| merged common ancestors
static bool
CompileUtf8(JSContext* cx, const ReadOnlyCompileOptions& options,
            const char* bytes, size_t length, JS::MutableHandleScript script)
{
    char16_t* chars = UTF8CharsToNewTwoByteCharsZ(cx, UTF8Chars(bytes, length), &length).get();
    if (!chars) {
        return false;
    }
=======
  SourceText<char16_t> source;
  if (!source.init(cx, std::move(chars), length)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  SourceText<char16_t> source;
  if (!source.init(cx, std::move(chars), length)) {
    return false;
  }
||||||| merged common ancestors
    SourceBufferHolder source(chars, length, SourceBufferHolder::GiveOwnership);
    return CompileSourceBuffer(cx, options, source, script);
}
=======
  return CompileSourceBuffer(cx, options, source);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return CompileSourceBuffer(cx, options, source, script);
||||||| merged common ancestors
bool
JS::Compile(JSContext* cx, const ReadOnlyCompileOptions& options,
            SourceBufferHolder& srcBuf, JS::MutableHandleScript script)
{
    return CompileSourceBuffer(cx, options, srcBuf, script);
=======
JSScript* JS::Compile(JSContext* cx, const ReadOnlyCompileOptions& options,
                      SourceText<char16_t>& srcBuf) {
  return CompileSourceBuffer(cx, options, srcBuf);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool CompileUtf8DontInflate(JSContext* cx,
                                   const ReadOnlyCompileOptions& options,
                                   const char* bytes, size_t length,
                                   JS::MutableHandleScript script) {
  SourceText<Utf8Unit> source;
  if (!source.init(cx, bytes, length, SourceOwnership::Borrowed)) {
    return false;
  }

  return CompileSourceBuffer(cx, options, source, script);
||||||| merged common ancestors
bool
JS::CompileLatin1(JSContext* cx, const ReadOnlyCompileOptions& options,
                  const char* bytes, size_t length, JS::MutableHandleScript script)
{
    return ::CompileLatin1(cx, options, bytes, length, script);
=======
JSScript* JS::Compile(JSContext* cx, const ReadOnlyCompileOptions& options,
                      SourceText<Utf8Unit>& srcBuf) {
  return CompileUtf8Inflating(cx, options, srcBuf);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool JS::Compile(JSContext* cx, const ReadOnlyCompileOptions& options,
                 SourceText<char16_t>& srcBuf, JS::MutableHandleScript script) {
  return CompileSourceBuffer(cx, options, srcBuf, script);
||||||| merged common ancestors
bool
JS::CompileUtf8(JSContext* cx, const ReadOnlyCompileOptions& options,
                const char* bytes, size_t length, JS::MutableHandleScript script)
{
    return ::CompileUtf8(cx, options, bytes, length, script);
=======
JSScript* JS::CompileDontInflate(JSContext* cx,
                                 const ReadOnlyCompileOptions& options,
                                 SourceText<Utf8Unit>& srcBuf) {
  return CompileSourceBuffer(cx, options, srcBuf);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool JS::CompileDontInflate(JSContext* cx,
                            const ReadOnlyCompileOptions& options,
                            SourceText<Utf8Unit>& srcBuf,
                            JS::MutableHandleScript script) {
  return CompileSourceBuffer(cx, options, srcBuf, script);
}
||||||| merged common ancestors
bool
JS::CompileUtf8File(JSContext* cx, const ReadOnlyCompileOptions& options,
                    FILE* file, JS::MutableHandleScript script)
{
    FileContents buffer(cx);
    if (!ReadCompleteFile(cx, file, buffer)) {
        return false;
    }
=======
JSScript* JS::CompileUtf8File(JSContext* cx,
                              const ReadOnlyCompileOptions& options,
                              FILE* file) {
  FileContents buffer(cx);
  if (!ReadCompleteFile(cx, file, buffer)) {
    return nullptr;
  }

  SourceText<Utf8Unit> srcBuf;
  if (!srcBuf.init(cx, reinterpret_cast<const char*>(buffer.begin()),
                   buffer.length(), SourceOwnership::Borrowed)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool JS::CompileLatin1(JSContext* cx, const ReadOnlyCompileOptions& options,
                       const char* bytes, size_t length,
                       JS::MutableHandleScript script) {
  return ::CompileLatin1(cx, options, bytes, length, script);
||||||| merged common ancestors
    return ::CompileUtf8(cx, options,
                         reinterpret_cast<const char*>(buffer.begin()), buffer.length(), script);
=======
  return CompileUtf8Inflating(cx, options, srcBuf);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool JS::CompileUtf8(JSContext* cx, const ReadOnlyCompileOptions& options,
                     const char* bytes, size_t length,
                     JS::MutableHandleScript script) {
  return ::CompileUtf8(cx, options, bytes, length, script);
}
||||||| merged common ancestors
bool
JS::CompileUtf8Path(JSContext* cx, const ReadOnlyCompileOptions& optionsArg,
            const char* filename, JS::MutableHandleScript script)
{
    AutoFile file;
    if (!file.open(cx, filename)) {
        return false;
    }
=======
JSScript* JS::CompileUtf8FileDontInflate(JSContext* cx,
                                         const ReadOnlyCompileOptions& options,
                                         FILE* file) {
  FileContents buffer(cx);
  if (!ReadCompleteFile(cx, file, buffer)) {
    return nullptr;
  }

  SourceText<Utf8Unit> srcBuf;
  if (!srcBuf.init(cx, reinterpret_cast<const char*>(buffer.begin()),
                   buffer.length(), SourceOwnership::Borrowed)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool JS::CompileUtf8DontInflate(JSContext* cx,
                                const ReadOnlyCompileOptions& options,
                                const char* bytes, size_t length,
                                JS::MutableHandleScript script) {
  return ::CompileUtf8DontInflate(cx, options, bytes, length, script);
||||||| merged common ancestors
    CompileOptions options(cx, optionsArg);
    options.setFileAndLine(filename, 1);
    return CompileUtf8File(cx, options, file.fp(), script);
=======
  return CompileSourceBuffer(cx, options, srcBuf);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool JS::CompileUtf8File(JSContext* cx, const ReadOnlyCompileOptions& options,
                         FILE* file, JS::MutableHandleScript script) {
  FileContents buffer(cx);
  if (!ReadCompleteFile(cx, file, buffer)) {
    return false;
  }

  return ::CompileUtf8(cx, options,
                       reinterpret_cast<const char*>(buffer.begin()),
                       buffer.length(), script);
||||||| merged common ancestors
bool
JS::CompileForNonSyntacticScope(JSContext* cx, const ReadOnlyCompileOptions& optionsArg,
                                SourceBufferHolder& srcBuf, JS::MutableHandleScript script)
{
    CompileOptions options(cx, optionsArg);
    options.setNonSyntacticScope(true);
    return CompileSourceBuffer(cx, options, srcBuf, script);
=======
JSScript* JS::CompileUtf8Path(JSContext* cx,
                              const ReadOnlyCompileOptions& optionsArg,
                              const char* filename) {
  AutoFile file;
  if (!file.open(cx, filename)) {
    return nullptr;
  }

  CompileOptions options(cx, optionsArg);
  options.setFileAndLine(filename, 1);
  return CompileUtf8File(cx, options, file.fp());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool JS::CompileUtf8FileDontInflate(JSContext* cx,
                                    const ReadOnlyCompileOptions& options,
                                    FILE* file,
                                    JS::MutableHandleScript script) {
  FileContents buffer(cx);
  if (!ReadCompleteFile(cx, file, buffer)) {
    return false;
  }
||||||| merged common ancestors
bool
JS::CompileLatin1ForNonSyntacticScope(JSContext* cx, const ReadOnlyCompileOptions& optionsArg,
                                      const char* bytes, size_t length,
                                      JS::MutableHandleScript script)
{
    CompileOptions options(cx, optionsArg);
    options.setNonSyntacticScope(true);
=======
JSScript* JS::CompileUtf8PathDontInflate(
    JSContext* cx, const ReadOnlyCompileOptions& optionsArg,
    const char* filename) {
  AutoFile file;
  if (!file.open(cx, filename)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return ::CompileUtf8DontInflate(cx, options,
                                  reinterpret_cast<const char*>(buffer.begin()),
                                  buffer.length(), script);
||||||| merged common ancestors
    return ::CompileLatin1(cx, options, bytes, length, script);
=======
  CompileOptions options(cx, optionsArg);
  options.setFileAndLine(filename, 1);
  return CompileUtf8FileDontInflate(cx, options, file.fp());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool JS::CompileUtf8Path(JSContext* cx,
                         const ReadOnlyCompileOptions& optionsArg,
                         const char* filename, JS::MutableHandleScript script) {
  AutoFile file;
  if (!file.open(cx, filename)) {
    return false;
  }
||||||| merged common ancestors
JS_PUBLIC_API(bool)
JS_Utf8BufferIsCompilableUnit(JSContext* cx, HandleObject obj, const char* utf8, size_t length)
{
    AssertHeapIsIdle();
    CHECK_THREAD(cx);
    cx->check(obj);
=======
JSScript* JS::CompileForNonSyntacticScope(
    JSContext* cx, const ReadOnlyCompileOptions& optionsArg,
    SourceText<char16_t>& srcBuf) {
  CompileOptions options(cx, optionsArg);
  options.setNonSyntacticScope(true);
  return CompileSourceBuffer(cx, options, srcBuf);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  CompileOptions options(cx, optionsArg);
  options.setFileAndLine(filename, 1);
  return CompileUtf8File(cx, options, file.fp(), script);
}
||||||| merged common ancestors
    cx->clearPendingException();
=======
JSScript* JS::CompileForNonSyntacticScope(
    JSContext* cx, const ReadOnlyCompileOptions& optionsArg,
    SourceText<Utf8Unit>& srcBuf) {
  CompileOptions options(cx, optionsArg);
  options.setNonSyntacticScope(true);
  return CompileUtf8Inflating(cx, options, srcBuf);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool JS::CompileForNonSyntacticScope(JSContext* cx,
                                     const ReadOnlyCompileOptions& optionsArg,
                                     SourceText<char16_t>& srcBuf,
                                     JS::MutableHandleScript script) {
  CompileOptions options(cx, optionsArg);
  options.setNonSyntacticScope(true);
  return CompileSourceBuffer(cx, options, srcBuf, script);
}
||||||| merged common ancestors
    JS::UniqueTwoByteChars chars
        { UTF8CharsToNewTwoByteCharsZ(cx, UTF8Chars(utf8, length), &length).get() };
    if (!chars) {
        return true;
    }
=======
JSScript* JS::CompileForNonSyntacticScopeDontInflate(
    JSContext* cx, const ReadOnlyCompileOptions& optionsArg,
    SourceText<Utf8Unit>& srcBuf) {
  CompileOptions options(cx, optionsArg);
  options.setNonSyntacticScope(true);
  return CompileSourceBuffer(cx, options, srcBuf);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool JS::CompileLatin1ForNonSyntacticScope(
    JSContext* cx, const ReadOnlyCompileOptions& optionsArg, const char* bytes,
    size_t length, JS::MutableHandleScript script) {
  CompileOptions options(cx, optionsArg);
  options.setNonSyntacticScope(true);
||||||| merged common ancestors
    // Return true on any out-of-memory error or non-EOF-related syntax error, so our
    // caller doesn't try to collect more buffered source.
    bool result = true;
=======
JS_PUBLIC_API bool JS_Utf8BufferIsCompilableUnit(JSContext* cx,
                                                 HandleObject obj,
                                                 const char* utf8,
                                                 size_t length) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(obj);

  cx->clearPendingException();

  JS::UniqueTwoByteChars chars{
      UTF8CharsToNewTwoByteCharsZ(cx, UTF8Chars(utf8, length), &length,
                                  js::MallocArena)
          .get()};
  if (!chars) {
    return true;
  }

  // Return true on any out-of-memory error or non-EOF-related syntax error, so
  // our caller doesn't try to collect more buffered source.
  bool result = true;

  using frontend::CreateScriptSourceObject;
  using frontend::FullParseHandler;
  using frontend::ParseGoal;
  using frontend::Parser;
  using frontend::UsedNameTracker;

  CompileOptions options(cx);
  UsedNameTracker usedNames(cx);

  Rooted<ScriptSourceObject*> sourceObject(cx);
  sourceObject = CreateScriptSourceObject(cx, options, mozilla::Nothing());
  if (!sourceObject) {
    return false;
  }

  JS::AutoSuppressWarningReporter suppressWarnings(cx);
  Parser<FullParseHandler, char16_t> parser(
      cx, cx->tempLifoAlloc(), options, chars.get(), length,
      /* foldConstants = */ true, usedNames, nullptr, nullptr, sourceObject,
      ParseGoal::Script);
  if (!parser.checkOptions() || !parser.parse()) {
    // We ran into an error. If it was because we ran out of source, we
    // return false so our caller knows to try to collect more buffered
    // source.
    if (parser.isUnexpectedEOF()) {
      result = false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return ::CompileLatin1(cx, options, bytes, length, script);
}
||||||| merged common ancestors
    using frontend::UsedNameTracker;
    using frontend::CreateScriptSourceObject;
    using frontend::Parser;
    using frontend::FullParseHandler;
    using frontend::ParseGoal;
=======
    cx->clearPendingException();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_PUBLIC_API bool JS_Utf8BufferIsCompilableUnit(JSContext* cx,
                                                 HandleObject obj,
                                                 const char* utf8,
                                                 size_t length) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(obj);
||||||| merged common ancestors
    CompileOptions options(cx);
    UsedNameTracker usedNames(cx);
=======
  return result;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  cx->clearPendingException();
||||||| merged common ancestors
    Rooted<ScriptSourceObject*> sourceObject(cx);
    sourceObject = CreateScriptSourceObject(cx, options, mozilla::Nothing());
    if (!sourceObject) {
        return false;
    }
=======
class FunctionCompiler {
 private:
  JSContext* const cx_;
  RootedAtom nameAtom_;
  StringBuffer funStr_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::UniqueTwoByteChars chars{
      UTF8CharsToNewTwoByteCharsZ(cx, UTF8Chars(utf8, length), &length).get()};
  if (!chars) {
    return true;
  }

  // Return true on any out-of-memory error or non-EOF-related syntax error, so
  // our caller doesn't try to collect more buffered source.
  bool result = true;

  using frontend::CreateScriptSourceObject;
  using frontend::FullParseHandler;
  using frontend::ParseGoal;
  using frontend::Parser;
  using frontend::UsedNameTracker;

  CompileOptions options(cx);
  UsedNameTracker usedNames(cx);

  Rooted<ScriptSourceObject*> sourceObject(cx);
  sourceObject = CreateScriptSourceObject(cx, options, mozilla::Nothing());
  if (!sourceObject) {
    return false;
  }

  JS::AutoSuppressWarningReporter suppressWarnings(cx);
  Parser<FullParseHandler, char16_t> parser(
      cx, cx->tempLifoAlloc(), options, chars.get(), length,
      /* foldConstants = */ true, usedNames, nullptr, nullptr, sourceObject,
      ParseGoal::Script);
  if (!parser.checkOptions() || !parser.parse()) {
    // We ran into an error. If it was because we ran out of source, we
    // return false so our caller knows to try to collect more buffered
    // source.
    if (parser.isUnexpectedEOF()) {
      result = false;
    }

    cx->clearPendingException();
  }

  return result;
}

/*
 * enclosingScope is a scope, if any (e.g. a WithScope).  If the scope is the
 * global scope, this must be null.
 *
 * enclosingEnv is an environment to use, if it's not the global.
 */
static bool CompileFunction(
    JSContext* cx, const ReadOnlyCompileOptions& optionsArg, HandleAtom name,
    bool isInvalidName, SourceText<char16_t>& srcBuf, uint32_t parameterListEnd,
    HandleObject enclosingEnv, HandleScope enclosingScope,
    MutableHandleFunction fun) {
  MOZ_ASSERT(!cx->zone()->isAtomsZone());
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(enclosingEnv);
  RootedAtom funAtom(cx);

  fun.set(NewScriptedFunction(cx, 0, JSFunction::INTERPRETED_NORMAL,
                              isInvalidName ? nullptr : name,
                              /* proto = */ nullptr, gc::AllocKind::FUNCTION,
                              TenuredObject, enclosingEnv));
  if (!fun) {
    return false;
  }

  // Make sure the static scope chain matches up when we have a
  // non-syntactic scope.
  MOZ_ASSERT_IF(!IsGlobalLexicalEnvironment(enclosingEnv),
                enclosingScope->hasOnChain(ScopeKind::NonSyntactic));

  if (!js::frontend::CompileStandaloneFunction(cx, fun, optionsArg, srcBuf,
                                               mozilla::Some(parameterListEnd),
                                               enclosingScope)) {
    return false;
  }

  // When function name is not valid identifier, generated function source
  // in srcBuf doesn't have function name.  Set it here.
  if (isInvalidName) {
    fun->setAtom(name);
  }

  return true;
}

static MOZ_MUST_USE bool BuildFunctionString(const char* name, size_t nameLen,
                                             unsigned nargs,
                                             const char* const* argnames,
                                             const SourceText<char16_t>& srcBuf,
                                             StringBuffer* out,
                                             uint32_t* parameterListEnd) {
  MOZ_ASSERT(out);
  MOZ_ASSERT(parameterListEnd);

  if (!out->ensureTwoByteChars()) {
    return false;
  }
  if (!out->append("function ")) {
    return false;
  }
  if (name) {
    if (!out->append(name, nameLen)) {
      return false;
    }
  }
  if (!out->append("(")) {
    return false;
  }
  for (unsigned i = 0; i < nargs; i++) {
    if (i != 0) {
      if (!out->append(", ")) {
        return false;
      }
    }
    if (!out->append(argnames[i], strlen(argnames[i]))) {
      return false;
    }
  }

  // Remember the position of ")".
  *parameterListEnd = out->length();
  MOZ_ASSERT(FunctionConstructorMedialSigils[0] == ')');

  if (!out->append(FunctionConstructorMedialSigils)) {
    return false;
  }
  if (!out->append(srcBuf.get(), srcBuf.length())) {
    return false;
  }
  if (!out->append(FunctionConstructorFinalBrace)) {
    return false;
  }

  return true;
}

JS_PUBLIC_API bool JS::CompileFunction(JSContext* cx,
                                       AutoObjectVector& envChain,
                                       const ReadOnlyCompileOptions& options,
                                       const char* name, unsigned nargs,
                                       const char* const* argnames,
                                       SourceText<char16_t>& srcBuf,
                                       MutableHandleFunction fun) {
  RootedObject env(cx);
  RootedScope scope(cx);
  if (!CreateNonSyntacticEnvironmentChain(cx, envChain, &env, &scope)) {
    return false;
  }

  size_t nameLen = 0;
  bool isInvalidName = false;
  RootedAtom nameAtom(cx);
  if (name) {
    nameLen = strlen(name);
    nameAtom = Atomize(cx, name, nameLen);
    if (!nameAtom) {
      return false;
||||||| merged common ancestors
    Parser<FullParseHandler, char16_t> parser(cx, cx->tempLifoAlloc(), options,
                                              chars.get(), length, /* foldConstants = */ true,
                                              usedNames, nullptr, nullptr, sourceObject,
                                              ParseGoal::Script);
    JS::WarningReporter older = JS::SetWarningReporter(cx, nullptr);
    if (!parser.checkOptions() || !parser.parse()) {
        // We ran into an error. If it was because we ran out of source, we
        // return false so our caller knows to try to collect more buffered
        // source.
        if (parser.isUnexpectedEOF()) {
            result = false;
        }

        cx->clearPendingException();
    }
    JS::SetWarningReporter(cx, older);

    return result;
}

/*
 * enclosingScope is a scope, if any (e.g. a WithScope).  If the scope is the
 * global scope, this must be null.
 *
 * enclosingEnv is an environment to use, if it's not the global.
 */
static bool
CompileFunction(JSContext* cx, const ReadOnlyCompileOptions& optionsArg,
                HandleAtom name, bool isInvalidName,
                SourceBufferHolder& srcBuf, uint32_t parameterListEnd,
                HandleObject enclosingEnv, HandleScope enclosingScope,
                MutableHandleFunction fun)
{
    MOZ_ASSERT(!cx->zone()->isAtomsZone());
    AssertHeapIsIdle();
    CHECK_THREAD(cx);
    cx->check(enclosingEnv);
    RootedAtom funAtom(cx);

    fun.set(NewScriptedFunction(cx, 0, JSFunction::INTERPRETED_NORMAL,
                                isInvalidName ? nullptr : name,
                                /* proto = */ nullptr,
                                gc::AllocKind::FUNCTION, TenuredObject,
                                enclosingEnv));
    if (!fun) {
        return false;
    }

    // Make sure the static scope chain matches up when we have a
    // non-syntactic scope.
    MOZ_ASSERT_IF(!IsGlobalLexicalEnvironment(enclosingEnv),
                  enclosingScope->hasOnChain(ScopeKind::NonSyntactic));

    if (!js::frontend::CompileStandaloneFunction(cx, fun, optionsArg, srcBuf,
                                                 mozilla::Some(parameterListEnd), enclosingScope))
    {
        return false;
    }

    // When function name is not valid identifier, generated function source
    // in srcBuf doesn't have function name.  Set it here.
    if (isInvalidName) {
        fun->setAtom(name);
    }

    return true;
}

static MOZ_MUST_USE bool
BuildFunctionString(const char* name, size_t nameLen,
                    unsigned nargs, const char* const* argnames,
                    const SourceBufferHolder& srcBuf, StringBuffer* out,
                    uint32_t* parameterListEnd)
{
    MOZ_ASSERT(out);
    MOZ_ASSERT(parameterListEnd);

    if (!out->ensureTwoByteChars()) {
       return false;
    }
    if (!out->append("function ")) {
        return false;
    }
    if (name) {
        if (!out->append(name, nameLen)) {
            return false;
        }
    }
    if (!out->append("(")) {
        return false;
    }
    for (unsigned i = 0; i < nargs; i++) {
        if (i != 0) {
            if (!out->append(", ")) {
                return false;
            }
        }
        if (!out->append(argnames[i], strlen(argnames[i]))) {
            return false;
        }
    }

    // Remember the position of ")".
    *parameterListEnd = out->length();
    MOZ_ASSERT(FunctionConstructorMedialSigils[0] == ')');

    if (!out->append(FunctionConstructorMedialSigils)) {
        return false;
    }
    if (!out->append(srcBuf.get(), srcBuf.length())) {
        return false;
    }
    if (!out->append(FunctionConstructorFinalBrace)) {
        return false;
    }

    return true;
}

JS_PUBLIC_API(bool)
JS::CompileFunction(JSContext* cx, AutoObjectVector& envChain,
                    const ReadOnlyCompileOptions& options,
                    const char* name, unsigned nargs, const char* const* argnames,
                    SourceBufferHolder& srcBuf, MutableHandleFunction fun)
{
    RootedObject env(cx);
    RootedScope scope(cx);
    if (!CreateNonSyntacticEnvironmentChain(cx, envChain, &env, &scope)) {
        return false;
=======
  uint32_t parameterListEnd_ = 0;
  bool nameIsIdentifier_ = true;

 public:
  explicit FunctionCompiler(JSContext* cx)
      : cx_(cx), nameAtom_(cx), funStr_(cx) {
    AssertHeapIsIdle();
    CHECK_THREAD(cx);
    MOZ_ASSERT(!cx->zone()->isAtomsZone());
  }

  MOZ_MUST_USE bool init(const char* name, unsigned nargs,
                         const char* const* argnames) {
    if (!funStr_.ensureTwoByteChars()) {
      return false;
    }
    if (!funStr_.append("function ")) {
      return false;
    }

    if (name) {
      size_t nameLen = strlen(name);

      nameAtom_ = Atomize(cx_, name, nameLen);
      if (!nameAtom_) {
        return false;
      }

      // If the name is an identifier, we can just add it to source text.
      // Otherwise we'll have to set it manually later.
      nameIsIdentifier_ = js::frontend::IsIdentifier(
          reinterpret_cast<const Latin1Char*>(name), nameLen);
      if (nameIsIdentifier_) {
        if (!funStr_.append(nameAtom_)) {
          return false;
        }
      }
    }

    if (!funStr_.append("(")) {
      return false;
    }

    for (unsigned i = 0; i < nargs; i++) {
      if (i != 0) {
        if (!funStr_.append(", ")) {
          return false;
        }
      }
      if (!funStr_.append(argnames[i], strlen(argnames[i]))) {
        return false;
      }
    }

    // Remember the position of ")".
    parameterListEnd_ = funStr_.length();
    MOZ_ASSERT(FunctionConstructorMedialSigils[0] == ')');

    return funStr_.append(FunctionConstructorMedialSigils);
  }

  template <typename Unit>
  inline MOZ_MUST_USE bool addFunctionBody(const SourceText<Unit>& srcBuf) {
    return funStr_.append(srcBuf.get(), srcBuf.length());
  }

  JSFunction* finish(HandleObjectVector envChain,
                     const ReadOnlyCompileOptions& options) {
    if (!funStr_.append(FunctionConstructorFinalBrace)) {
      return nullptr;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    // If name is not valid identifier
    if (!js::frontend::IsIdentifier(reinterpret_cast<const Latin1Char*>(name),
                                    nameLen)) {
      isInvalidName = true;
||||||| merged common ancestors
    size_t nameLen = 0;
    bool isInvalidName = false;
    RootedAtom nameAtom(cx);
    if (name) {
        nameLen = strlen(name);
        nameAtom = Atomize(cx, name, nameLen);
        if (!nameAtom) {
            return false;
        }

        // If name is not valid identifier
        if (!js::frontend::IsIdentifier(name, nameLen)) {
            isInvalidName = true;
        }
=======
    size_t newLen = funStr_.length();
    UniqueTwoByteChars stolen(funStr_.stealChars());
    if (!stolen) {
      return nullptr;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

  uint32_t parameterListEnd;
  StringBuffer funStr(cx);
  if (!BuildFunctionString(isInvalidName ? nullptr : name, nameLen, nargs,
                           argnames, srcBuf, &funStr, &parameterListEnd)) {
    return false;
  }

  size_t newLen = funStr.length();
  UniqueTwoByteChars stolen(funStr.stealChars());
  if (!stolen) {
    return false;
  }

  SourceText<char16_t> newSrcBuf;
  if (!newSrcBuf.init(cx, std::move(stolen), newLen)) {
    return false;
  }

  return CompileFunction(cx, options, nameAtom, isInvalidName, newSrcBuf,
                         parameterListEnd, env, scope, fun);
}

JS_PUBLIC_API bool JS::CompileFunctionUtf8(
    JSContext* cx, AutoObjectVector& envChain,
    const ReadOnlyCompileOptions& options, const char* name, unsigned nargs,
    const char* const* argnames, const char* bytes, size_t length,
    MutableHandleFunction fun) {
  auto chars = UniqueTwoByteChars(
      UTF8CharsToNewTwoByteCharsZ(cx, UTF8Chars(bytes, length), &length).get());
  if (!chars) {
    return false;
  }

  SourceText<char16_t> source;
  if (!source.init(cx, std::move(chars), length)) {
    return false;
  }

  return CompileFunction(cx, envChain, options, name, nargs, argnames, source,
                         fun);
}

JS_PUBLIC_API bool JS::InitScriptSourceElement(JSContext* cx,
                                               HandleScript script,
                                               HandleObject element,
                                               HandleString elementAttrName) {
  MOZ_ASSERT(cx);
  MOZ_ASSERT(CurrentThreadCanAccessRuntime(cx->runtime()));

  RootedScriptSourceObject sso(
      cx, &script->sourceObject()->as<ScriptSourceObject>());
  return ScriptSourceObject::initElementProperties(cx, sso, element,
                                                   elementAttrName);
}

JS_PUBLIC_API void JS::ExposeScriptToDebugger(JSContext* cx,
                                              HandleScript script) {
  MOZ_ASSERT(cx);
  MOZ_ASSERT(CurrentThreadCanAccessRuntime(cx->runtime()));

  MOZ_ASSERT(script->hideScriptFromDebugger());
  script->clearHideScriptFromDebugger();
  Debugger::onNewScript(cx, script);
}

MOZ_NEVER_INLINE static bool ExecuteScript(JSContext* cx, HandleObject scope,
                                           HandleScript script, Value* rval) {
  MOZ_ASSERT(!cx->zone()->isAtomsZone());
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(scope, script);
  MOZ_ASSERT_IF(!IsGlobalLexicalEnvironment(scope),
                script->hasNonSyntacticScope());
  return Execute(cx, script, *scope, rval);
}

static bool ExecuteScript(JSContext* cx, AutoObjectVector& envChain,
                          HandleScript scriptArg, Value* rval) {
  RootedObject env(cx);
  RootedScope dummy(cx);
  if (!CreateNonSyntacticEnvironmentChain(cx, envChain, &env, &dummy)) {
    return false;
  }

  RootedScript script(cx, scriptArg);
  if (!script->hasNonSyntacticScope() && !IsGlobalLexicalEnvironment(env)) {
    script = CloneGlobalScript(cx, ScopeKind::NonSyntactic, script);
    if (!script) {
      return false;
||||||| merged common ancestors

    uint32_t parameterListEnd;
    StringBuffer funStr(cx);
    if (!BuildFunctionString(isInvalidName ? nullptr : name, nameLen, nargs, argnames, srcBuf,
                             &funStr, &parameterListEnd))
    {
        return false;
=======

    SourceText<char16_t> newSrcBuf;
    if (!newSrcBuf.init(cx_, std::move(stolen), newLen)) {
      return nullptr;
>>>>>>> upstream-releases
    }
    js::Debugger::onNewScript(cx, script);
  }

<<<<<<< HEAD
  return ExecuteScript(cx, env, script, rval);
}

MOZ_NEVER_INLINE JS_PUBLIC_API bool JS_ExecuteScript(JSContext* cx,
                                                     HandleScript scriptArg,
                                                     MutableHandleValue rval) {
  RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
  return ExecuteScript(cx, globalLexical, scriptArg, rval.address());
}

MOZ_NEVER_INLINE JS_PUBLIC_API bool JS_ExecuteScript(JSContext* cx,
                                                     HandleScript scriptArg) {
  RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
  return ExecuteScript(cx, globalLexical, scriptArg, nullptr);
}
||||||| merged common ancestors
    size_t newLen = funStr.length();
    SourceBufferHolder newSrcBuf(funStr.stealChars(), newLen, SourceBufferHolder::GiveOwnership);

    return CompileFunction(cx, options, nameAtom, isInvalidName, newSrcBuf, parameterListEnd, env,
                           scope, fun);
}

JS_PUBLIC_API(bool)
JS::CompileFunctionUtf8(JSContext* cx, AutoObjectVector& envChain,
                        const ReadOnlyCompileOptions& options,
                        const char* name, unsigned nargs, const char* const* argnames,
                        const char* bytes, size_t length, MutableHandleFunction fun)
{
    char16_t* chars = UTF8CharsToNewTwoByteCharsZ(cx, UTF8Chars(bytes, length), &length).get();
    if (!chars) {
        return false;
    }

    SourceBufferHolder source(chars, length, SourceBufferHolder::GiveOwnership);
    return CompileFunction(cx, envChain, options, name, nargs, argnames,
                           source, fun);
}

JS_PUBLIC_API(bool)
JS::InitScriptSourceElement(JSContext* cx, HandleScript script,
                            HandleObject element, HandleString elementAttrName)
{
    MOZ_ASSERT(cx);
    MOZ_ASSERT(CurrentThreadCanAccessRuntime(cx->runtime()));

    RootedScriptSourceObject sso(cx, &script->sourceObject()->as<ScriptSourceObject>());
    return ScriptSourceObject::initElementProperties(cx, sso, element, elementAttrName);
}
=======
    RootedObject enclosingEnv(cx_);
    RootedScope enclosingScope(cx_);
    if (!CreateNonSyntacticEnvironmentChain(cx_, envChain, &enclosingEnv,
                                            &enclosingScope)) {
      return nullptr;
    }

    cx_->check(enclosingEnv);
>>>>>>> upstream-releases

<<<<<<< HEAD
MOZ_NEVER_INLINE JS_PUBLIC_API bool JS_ExecuteScript(JSContext* cx,
                                                     AutoObjectVector& envChain,
                                                     HandleScript scriptArg,
                                                     MutableHandleValue rval) {
  return ExecuteScript(cx, envChain, scriptArg, rval.address());
}
||||||| merged common ancestors
JS_PUBLIC_API(void)
JS::ExposeScriptToDebugger(JSContext* cx, HandleScript script)
{
    MOZ_ASSERT(cx);
    MOZ_ASSERT(CurrentThreadCanAccessRuntime(cx->runtime()));

    MOZ_ASSERT(script->hideScriptFromDebugger());
    script->clearHideScriptFromDebugger();
    Debugger::onNewScript(cx, script);
}
=======
    RootedFunction fun(
        cx_,
        NewScriptedFunction(cx_, 0, JSFunction::INTERPRETED_NORMAL,
                            nameIsIdentifier_ ? HandleAtom(nameAtom_) : nullptr,
                            /* proto = */ nullptr, gc::AllocKind::FUNCTION,
                            TenuredObject, enclosingEnv));
    if (!fun) {
      return nullptr;
    }

    // Make sure the static scope chain matches up when we have a
    // non-syntactic scope.
    MOZ_ASSERT_IF(!IsGlobalLexicalEnvironment(enclosingEnv),
                  enclosingScope->hasOnChain(ScopeKind::NonSyntactic));
>>>>>>> upstream-releases

<<<<<<< HEAD
MOZ_NEVER_INLINE JS_PUBLIC_API bool JS_ExecuteScript(JSContext* cx,
                                                     AutoObjectVector& envChain,
                                                     HandleScript scriptArg) {
  return ExecuteScript(cx, envChain, scriptArg, nullptr);
}
||||||| merged common ancestors
MOZ_NEVER_INLINE static bool
ExecuteScript(JSContext* cx, HandleObject scope, HandleScript script, Value* rval)
{
    MOZ_ASSERT(!cx->zone()->isAtomsZone());
    AssertHeapIsIdle();
    CHECK_THREAD(cx);
    cx->check(scope, script);
    MOZ_ASSERT_IF(!IsGlobalLexicalEnvironment(scope), script->hasNonSyntacticScope());
    return Execute(cx, script, *scope, rval);
}
=======
    if (!js::frontend::CompileStandaloneFunction(
            cx_, &fun, options, newSrcBuf, mozilla::Some(parameterListEnd_),
            enclosingScope)) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_PUBLIC_API bool JS::CloneAndExecuteScript(JSContext* cx,
                                             HandleScript scriptArg,
                                             JS::MutableHandleValue rval) {
  CHECK_THREAD(cx);
  RootedScript script(cx, scriptArg);
  RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
  if (script->realm() != cx->realm()) {
    script = CloneGlobalScript(cx, ScopeKind::Global, script);
    if (!script) {
      return false;
||||||| merged common ancestors
static bool
ExecuteScript(JSContext* cx, AutoObjectVector& envChain, HandleScript scriptArg, Value* rval)
{
    RootedObject env(cx);
    RootedScope dummy(cx);
    if (!CreateNonSyntacticEnvironmentChain(cx, envChain, &env, &dummy)) {
        return false;
    }

    RootedScript script(cx, scriptArg);
    if (!script->hasNonSyntacticScope() && !IsGlobalLexicalEnvironment(env)) {
        script = CloneGlobalScript(cx, ScopeKind::NonSyntactic, script);
        if (!script) {
            return false;
        }
        js::Debugger::onNewScript(cx, script);
=======
    // When the function name isn't a valid identifier, the generated function
    // source in srcBuf won't include the name, so name the function manually.
    if (!nameIsIdentifier_) {
      fun->setAtom(nameAtom_);
    }

    return fun;
  }
};

JS_PUBLIC_API JSFunction* JS::CompileFunction(
    JSContext* cx, HandleObjectVector envChain,
    const ReadOnlyCompileOptions& options, const char* name, unsigned nargs,
    const char* const* argnames, SourceText<char16_t>& srcBuf) {
  FunctionCompiler compiler(cx);
  if (!compiler.init(name, nargs, argnames) ||
      !compiler.addFunctionBody(srcBuf)) {
    return nullptr;
  }

  return compiler.finish(envChain, options);
}

JS_PUBLIC_API JSFunction* JS::CompileFunction(
    JSContext* cx, HandleObjectVector envChain,
    const ReadOnlyCompileOptions& options, const char* name, unsigned nargs,
    const char* const* argnames, SourceText<Utf8Unit>& srcBuf) {
  FunctionCompiler compiler(cx);
  if (!compiler.init(name, nargs, argnames) ||
      !compiler.addFunctionBody(srcBuf)) {
    return nullptr;
  }

  return compiler.finish(envChain, options);
}

JS_PUBLIC_API JSFunction* JS::CompileFunctionUtf8(
    JSContext* cx, HandleObjectVector envChain,
    const ReadOnlyCompileOptions& options, const char* name, unsigned nargs,
    const char* const* argnames, const char* bytes, size_t length) {
  SourceText<Utf8Unit> srcBuf;
  if (!srcBuf.init(cx, bytes, length, SourceOwnership::Borrowed)) {
    return nullptr;
  }

  return CompileFunction(cx, envChain, options, name, nargs, argnames, srcBuf);
}

JS_PUBLIC_API bool JS::InitScriptSourceElement(JSContext* cx,
                                               HandleScript script,
                                               HandleObject element,
                                               HandleString elementAttrName) {
  MOZ_ASSERT(cx);
  MOZ_ASSERT(CurrentThreadCanAccessRuntime(cx->runtime()));

  RootedScriptSourceObject sso(
      cx, &script->sourceObject()->as<ScriptSourceObject>());
  return ScriptSourceObject::initElementProperties(cx, sso, element,
                                                   elementAttrName);
}

JS_PUBLIC_API void JS::ExposeScriptToDebugger(JSContext* cx,
                                              HandleScript script) {
  MOZ_ASSERT(cx);
  MOZ_ASSERT(CurrentThreadCanAccessRuntime(cx->runtime()));

  MOZ_ASSERT(script->hideScriptFromDebugger());
  script->clearHideScriptFromDebugger();
  Debugger::onNewScript(cx, script);
}

MOZ_NEVER_INLINE static bool ExecuteScript(JSContext* cx, HandleObject scope,
                                           HandleScript script, Value* rval) {
  MOZ_ASSERT(!cx->zone()->isAtomsZone());
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(scope, script);
  MOZ_ASSERT_IF(!IsGlobalLexicalEnvironment(scope),
                script->hasNonSyntacticScope());
  return Execute(cx, script, *scope, rval);
}

static bool ExecuteScript(JSContext* cx, HandleObjectVector envChain,
                          HandleScript scriptArg, Value* rval) {
  RootedObject env(cx);
  RootedScope dummy(cx);
  if (!CreateNonSyntacticEnvironmentChain(cx, envChain, &env, &dummy)) {
    return false;
  }

  RootedScript script(cx, scriptArg);
  if (!script->hasNonSyntacticScope() && !IsGlobalLexicalEnvironment(env)) {
    script = CloneGlobalScript(cx, ScopeKind::NonSyntactic, script);
    if (!script) {
      return false;
>>>>>>> upstream-releases
    }
    js::Debugger::onNewScript(cx, script);
  }

<<<<<<< HEAD
    js::Debugger::onNewScript(cx, script);
  }
  return ExecuteScript(cx, globalLexical, script, rval.address());
||||||| merged common ancestors
    return ExecuteScript(cx, env, script, rval);
}

MOZ_NEVER_INLINE JS_PUBLIC_API(bool)
JS_ExecuteScript(JSContext* cx, HandleScript scriptArg, MutableHandleValue rval)
{
    RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
    return ExecuteScript(cx, globalLexical, scriptArg, rval.address());
}

MOZ_NEVER_INLINE JS_PUBLIC_API(bool)
JS_ExecuteScript(JSContext* cx, HandleScript scriptArg)
{
    RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
    return ExecuteScript(cx, globalLexical, scriptArg, nullptr);
}

MOZ_NEVER_INLINE JS_PUBLIC_API(bool)
JS_ExecuteScript(JSContext* cx, AutoObjectVector& envChain,
                 HandleScript scriptArg, MutableHandleValue rval)
{
    return ExecuteScript(cx, envChain, scriptArg, rval.address());
}

MOZ_NEVER_INLINE JS_PUBLIC_API(bool)
JS_ExecuteScript(JSContext* cx, AutoObjectVector& envChain, HandleScript scriptArg)
{
    return ExecuteScript(cx, envChain, scriptArg, nullptr);
=======
  return ExecuteScript(cx, env, script, rval);
}

MOZ_NEVER_INLINE JS_PUBLIC_API bool JS_ExecuteScript(JSContext* cx,
                                                     HandleScript scriptArg,
                                                     MutableHandleValue rval) {
  RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
  return ExecuteScript(cx, globalLexical, scriptArg, rval.address());
}

MOZ_NEVER_INLINE JS_PUBLIC_API bool JS_ExecuteScript(JSContext* cx,
                                                     HandleScript scriptArg) {
  RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
  return ExecuteScript(cx, globalLexical, scriptArg, nullptr);
}

MOZ_NEVER_INLINE JS_PUBLIC_API bool JS_ExecuteScript(
    JSContext* cx, HandleObjectVector envChain, HandleScript scriptArg,
    MutableHandleValue rval) {
  return ExecuteScript(cx, envChain, scriptArg, rval.address());
}

MOZ_NEVER_INLINE JS_PUBLIC_API bool JS_ExecuteScript(
    JSContext* cx, HandleObjectVector envChain, HandleScript scriptArg) {
  return ExecuteScript(cx, envChain, scriptArg, nullptr);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_PUBLIC_API bool JS::CloneAndExecuteScript(JSContext* cx,
                                             JS::AutoObjectVector& envChain,
                                             HandleScript scriptArg,
                                             JS::MutableHandleValue rval) {
  CHECK_THREAD(cx);
  RootedScript script(cx, scriptArg);
  if (script->realm() != cx->realm()) {
    script = CloneGlobalScript(cx, ScopeKind::NonSyntactic, script);
||||||| merged common ancestors
JS_PUBLIC_API(bool)
JS::CloneAndExecuteScript(JSContext* cx, HandleScript scriptArg,
                          JS::MutableHandleValue rval)
{
    CHECK_THREAD(cx);
    RootedScript script(cx, scriptArg);
    RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
    if (script->realm() != cx->realm()) {
        script = CloneGlobalScript(cx, ScopeKind::Global, script);
        if (!script) {
            return false;
        }

        js::Debugger::onNewScript(cx, script);
    }
    return ExecuteScript(cx, globalLexical, script, rval.address());
}

JS_PUBLIC_API(bool)
JS::CloneAndExecuteScript(JSContext* cx, JS::AutoObjectVector& envChain,
                          HandleScript scriptArg,
                          JS::MutableHandleValue rval)
{
    CHECK_THREAD(cx);
    RootedScript script(cx, scriptArg);
    if (script->realm() != cx->realm()) {
        script = CloneGlobalScript(cx, ScopeKind::NonSyntactic, script);
        if (!script) {
            return false;
        }

        js::Debugger::onNewScript(cx, script);
    }
    return ExecuteScript(cx, envChain, script, rval.address());
}

static bool
Evaluate(JSContext* cx, ScopeKind scopeKind, HandleObject env,
         const ReadOnlyCompileOptions& optionsArg,
         SourceBufferHolder& srcBuf, MutableHandleValue rval)
{
    CompileOptions options(cx, optionsArg);
    MOZ_ASSERT(!cx->zone()->isAtomsZone());
    AssertHeapIsIdle();
    CHECK_THREAD(cx);
    cx->check(env);
    MOZ_ASSERT_IF(!IsGlobalLexicalEnvironment(env), scopeKind == ScopeKind::NonSyntactic);

    options.setIsRunOnce(true);
    RootedScript script(cx, frontend::CompileGlobalScript(cx, cx->tempLifoAlloc(),
                                                          scopeKind, options, srcBuf));
=======
JS_PUBLIC_API bool JS::CloneAndExecuteScript(JSContext* cx,
                                             HandleScript scriptArg,
                                             JS::MutableHandleValue rval) {
  CHECK_THREAD(cx);
  RootedScript script(cx, scriptArg);
  RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
  if (script->realm() != cx->realm()) {
    script = CloneGlobalScript(cx, ScopeKind::Global, script);
    if (!script) {
      return false;
    }

    js::Debugger::onNewScript(cx, script);
  }
  return ExecuteScript(cx, globalLexical, script, rval.address());
}

JS_PUBLIC_API bool JS::CloneAndExecuteScript(JSContext* cx,
                                             JS::HandleObjectVector envChain,
                                             HandleScript scriptArg,
                                             JS::MutableHandleValue rval) {
  CHECK_THREAD(cx);
  RootedScript script(cx, scriptArg);
  if (script->realm() != cx->realm()) {
    script = CloneGlobalScript(cx, ScopeKind::NonSyntactic, script);
>>>>>>> upstream-releases
    if (!script) {
      return false;
    }

<<<<<<< HEAD
    js::Debugger::onNewScript(cx, script);
  }
  return ExecuteScript(cx, envChain, script, rval.address());
}

static bool Evaluate(JSContext* cx, ScopeKind scopeKind, HandleObject env,
                     const ReadOnlyCompileOptions& optionsArg,
                     SourceText<char16_t>& srcBuf, MutableHandleValue rval) {
  CompileOptions options(cx, optionsArg);
  MOZ_ASSERT(!cx->zone()->isAtomsZone());
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(env);
  MOZ_ASSERT_IF(!IsGlobalLexicalEnvironment(env),
                scopeKind == ScopeKind::NonSyntactic);

  options.setIsRunOnce(true);
||||||| merged common ancestors
    bool result = Execute(cx, script, *env,
                          options.noScriptRval ? nullptr : rval.address());

    return result;
}

static bool
Evaluate(JSContext* cx, AutoObjectVector& envChain, const ReadOnlyCompileOptions& optionsArg,
         SourceBufferHolder& srcBuf, MutableHandleValue rval)
{
    RootedObject env(cx);
    RootedScope scope(cx);
    if (!CreateNonSyntacticEnvironmentChain(cx, envChain, &env, &scope)) {
        return false;
    }
    return ::Evaluate(cx, scope->kind(), env, optionsArg, srcBuf, rval);
}

extern JS_PUBLIC_API(bool)
JS::EvaluateUtf8(JSContext* cx, const ReadOnlyCompileOptions& options,
                 const char* bytes, size_t length, MutableHandle<Value> rval)
{
    char16_t* chars = UTF8CharsToNewTwoByteCharsZ(cx, UTF8Chars(bytes, length), &length).get();
    if (!chars) {
        return false;
    }
=======
    js::Debugger::onNewScript(cx, script);
  }
  return ExecuteScript(cx, envChain, script, rval.address());
}

template <typename Unit>
static bool EvaluateSourceBuffer(JSContext* cx, ScopeKind scopeKind,
                                 Handle<JSObject*> env,
                                 const ReadOnlyCompileOptions& optionsArg,
                                 SourceText<Unit>& srcBuf,
                                 MutableHandle<Value> rval) {
  CompileOptions options(cx, optionsArg);
  MOZ_ASSERT(!cx->zone()->isAtomsZone());
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(env);
  MOZ_ASSERT_IF(!IsGlobalLexicalEnvironment(env),
                scopeKind == ScopeKind::NonSyntactic);

  options.setIsRunOnce(true);

  RootedScript script(cx);
  {
    frontend::GlobalScriptInfo info(cx, options, scopeKind);
    script = frontend::CompileGlobalScript(info, srcBuf);
    if (!script) {
      return false;
    }
  }

  return Execute(cx, script, *env,
                 options.noScriptRval ? nullptr : rval.address());
}

JS_PUBLIC_API bool JS::Evaluate(JSContext* cx,
                                const ReadOnlyCompileOptions& options,
                                SourceText<Utf8Unit>& srcBuf,
                                MutableHandle<Value> rval) {
  RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());

  size_t length = srcBuf.length();
  auto chars = UniqueTwoByteChars(
      UTF8CharsToNewTwoByteCharsZ(cx, UTF8Chars(srcBuf.get(), length), &length,
                                  js::MallocArena)
          .get());
  if (!chars) {
    return false;
  }

  SourceText<char16_t> inflatedSrc;
  if (!inflatedSrc.init(cx, std::move(chars), length)) {
    return false;
  }

  return EvaluateSourceBuffer(cx, ScopeKind::Global, globalLexical, options,
                              inflatedSrc, rval);
}

JS_PUBLIC_API bool JS::EvaluateDontInflate(
    JSContext* cx, const ReadOnlyCompileOptions& options,
    SourceText<Utf8Unit>& srcBuf, MutableHandle<Value> rval) {
  RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
  return EvaluateSourceBuffer(cx, ScopeKind::Global, globalLexical, options,
                              srcBuf, rval);
}

JS_PUBLIC_API bool JS::Evaluate(JSContext* cx,
                                const ReadOnlyCompileOptions& optionsArg,
                                SourceText<char16_t>& srcBuf,
                                MutableHandleValue rval) {
  RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
  return EvaluateSourceBuffer(cx, ScopeKind::Global, globalLexical, optionsArg,
                              srcBuf, rval);
}

JS_PUBLIC_API bool JS::Evaluate(JSContext* cx, HandleObjectVector envChain,
                                const ReadOnlyCompileOptions& options,
                                SourceText<char16_t>& srcBuf,
                                MutableHandleValue rval) {
  RootedObject env(cx);
  RootedScope scope(cx);
  if (!CreateNonSyntacticEnvironmentChain(cx, envChain, &env, &scope)) {
    return false;
  }

  return EvaluateSourceBuffer(cx, scope->kind(), env, options, srcBuf, rval);
}

JS_PUBLIC_API bool JS::EvaluateUtf8Path(
    JSContext* cx, const ReadOnlyCompileOptions& optionsArg,
    const char* filename, MutableHandleValue rval) {
  FileContents buffer(cx);
  {
    AutoFile file;
    if (!file.open(cx, filename) || !file.readAll(cx, buffer)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedScript script(cx);
  {
    frontend::GlobalScriptInfo info(cx, options, scopeKind);
    script = frontend::CompileGlobalScript(info, srcBuf);
    if (!script) {
      return false;
    }
  }

  return Execute(cx, script, *env,
                 options.noScriptRval ? nullptr : rval.address());
}

static bool Evaluate(JSContext* cx, AutoObjectVector& envChain,
                     const ReadOnlyCompileOptions& optionsArg,
                     SourceText<char16_t>& srcBuf, MutableHandleValue rval) {
  RootedObject env(cx);
  RootedScope scope(cx);
  if (!CreateNonSyntacticEnvironmentChain(cx, envChain, &env, &scope)) {
    return false;
  }
  return ::Evaluate(cx, scope->kind(), env, optionsArg, srcBuf, rval);
}

extern JS_PUBLIC_API bool JS::EvaluateUtf8(
    JSContext* cx, const ReadOnlyCompileOptions& options, const char* bytes,
    size_t length, MutableHandle<Value> rval) {
  auto chars = UniqueTwoByteChars(
      UTF8CharsToNewTwoByteCharsZ(cx, UTF8Chars(bytes, length), &length).get());
  if (!chars) {
    return false;
  }

  SourceText<char16_t> srcBuf;
  if (!srcBuf.init(cx, std::move(chars), length)) {
    return false;
  }

  RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
  return ::Evaluate(cx, ScopeKind::Global, globalLexical, options, srcBuf,
                    rval);
}

extern JS_PUBLIC_API bool JS::EvaluateLatin1(
    JSContext* cx, const ReadOnlyCompileOptions& options, const char* bytes,
    size_t length, MutableHandle<Value> rval) {
  auto chars = UniqueTwoByteChars(InflateString(cx, bytes, length));
  if (!chars) {
    return false;
  }

  SourceText<char16_t> srcBuf;
  if (!srcBuf.init(cx, std::move(chars), length)) {
    return false;
  }

  RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
  return ::Evaluate(cx, ScopeKind::Global, globalLexical, options, srcBuf,
                    rval);
}

JS_PUBLIC_API bool JS::Evaluate(JSContext* cx,
                                const ReadOnlyCompileOptions& optionsArg,
                                SourceText<char16_t>& srcBuf,
                                MutableHandleValue rval) {
  RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
  return ::Evaluate(cx, ScopeKind::Global, globalLexical, optionsArg, srcBuf,
                    rval);
}

JS_PUBLIC_API bool JS::Evaluate(JSContext* cx, AutoObjectVector& envChain,
                                const ReadOnlyCompileOptions& optionsArg,
                                SourceText<char16_t>& srcBuf,
                                MutableHandleValue rval) {
  return ::Evaluate(cx, envChain, optionsArg, srcBuf, rval);
}

JS_PUBLIC_API bool JS::EvaluateUtf8Path(
    JSContext* cx, const ReadOnlyCompileOptions& optionsArg,
    const char* filename, MutableHandleValue rval) {
  FileContents buffer(cx);
  {
    AutoFile file;
    if (!file.open(cx, filename) || !file.readAll(cx, buffer)) {
      return false;
||||||| merged common ancestors
    SourceBufferHolder srcBuf(chars, length, SourceBufferHolder::GiveOwnership);
    RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
    return ::Evaluate(cx, ScopeKind::Global, globalLexical, options, srcBuf, rval);
}

extern JS_PUBLIC_API(bool)
JS::EvaluateLatin1(JSContext* cx, const ReadOnlyCompileOptions& options,
                   const char* bytes, size_t length, MutableHandle<Value> rval)
{
    char16_t* chars = InflateString(cx, bytes, length);
    if (!chars) {
        return false;
    }

    SourceBufferHolder srcBuf(chars, length, SourceBufferHolder::GiveOwnership);
    RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
    return ::Evaluate(cx, ScopeKind::Global, globalLexical, options, srcBuf, rval);
}

JS_PUBLIC_API(bool)
JS::Evaluate(JSContext* cx, const ReadOnlyCompileOptions& optionsArg,
             SourceBufferHolder& srcBuf, MutableHandleValue rval)
{
    RootedObject globalLexical(cx, &cx->global()->lexicalEnvironment());
    return ::Evaluate(cx, ScopeKind::Global, globalLexical, optionsArg, srcBuf, rval);
}

JS_PUBLIC_API(bool)
JS::Evaluate(JSContext* cx, AutoObjectVector& envChain, const ReadOnlyCompileOptions& optionsArg,
             SourceBufferHolder& srcBuf, MutableHandleValue rval)
{
    return ::Evaluate(cx, envChain, optionsArg, srcBuf, rval);
}

JS_PUBLIC_API(bool)
JS::EvaluateUtf8Path(JSContext* cx, const ReadOnlyCompileOptions& optionsArg,
                     const char* filename, MutableHandleValue rval)
{
    FileContents buffer(cx);
    {
        AutoFile file;
        if (!file.open(cx, filename) || !file.readAll(cx, buffer)) {
            return false;
        }
=======
  CompileOptions options(cx, optionsArg);
  options.setFileAndLine(filename, 1);

  auto contents = reinterpret_cast<const char*>(buffer.begin());
  size_t length = buffer.length();

  JS::SourceText<Utf8Unit> srcBuf;
  if (!srcBuf.init(cx, contents, length, JS::SourceOwnership::Borrowed)) {
    return false;
  }

  return Evaluate(cx, options, srcBuf, rval);
}

JS_PUBLIC_API bool JS::EvaluateUtf8PathDontInflate(
    JSContext* cx, const ReadOnlyCompileOptions& optionsArg,
    const char* filename, MutableHandleValue rval) {
  FileContents buffer(cx);
  {
    AutoFile file;
    if (!file.open(cx, filename) || !file.readAll(cx, buffer)) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
  }

  CompileOptions options(cx, optionsArg);
  options.setFileAndLine(filename, 1);

  auto contents = reinterpret_cast<const char*>(buffer.begin());
  size_t length = buffer.length();
>>>>>>> upstream-releases

<<<<<<< HEAD
  CompileOptions options(cx, optionsArg);
  options.setFileAndLine(filename, 1);
||||||| merged common ancestors
    CompileOptions options(cx, optionsArg);
    options.setFileAndLine(filename, 1);
=======
  JS::SourceText<Utf8Unit> srcBuf;
  if (!srcBuf.init(cx, contents, length, JS::SourceOwnership::Borrowed)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return EvaluateUtf8(cx, options,
                      reinterpret_cast<const char*>(buffer.begin()),
                      buffer.length(), rval);
||||||| merged common ancestors
    return EvaluateUtf8(cx, options,
                        reinterpret_cast<const char*>(buffer.begin()), buffer.length(), rval);
=======
  return EvaluateDontInflate(cx, options, srcBuf, rval);
>>>>>>> upstream-releases
}
