/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/Utf8.h"  // mozilla::Utf8Unit

#include "gc/GCInternals.h"
<<<<<<< HEAD
#include "js/CompilationAndEvaluation.h"
#include "js/SourceText.h"
||||||| merged common ancestors
#include "js/CompilationAndEvaluation.h"
#include "js/SourceBufferHolder.h"
=======
#include "js/CompilationAndEvaluation.h"  // JS::Compile{,ForNonSyntacticScope}{,DontInflate}
#include "js/SourceText.h"                // JS::Source{Ownership,Text}
>>>>>>> upstream-releases
#include "jsapi-tests/tests.h"
#include "vm/Monitor.h"
#include "vm/MutexIDs.h"

using namespace JS;
using js::AutoLockMonitor;

struct OffThreadTask {
  OffThreadTask() : monitor(js::mutexid::ShellOffThreadState), token(nullptr) {}

  OffThreadToken* waitUntilDone(JSContext* cx) {
    if (OffThreadParsingMustWaitForGC(cx->runtime())) {
      js::gc::FinishGC(cx);
    }

    AutoLockMonitor alm(monitor);
    while (!token) {
      alm.wait();
    }
    OffThreadToken* result = token;
    token = nullptr;
    return result;
  }

  void markDone(JS::OffThreadToken* tokenArg) {
    AutoLockMonitor alm(monitor);
    token = tokenArg;
    alm.notify();
  }

  static void OffThreadCallback(OffThreadToken* token, void* context) {
    auto self = static_cast<OffThreadTask*>(context);
    self->markDone(token);
  }

  js::Monitor monitor;
  OffThreadToken* token;
};

BEGIN_TEST(testCompileScript) {
  CHECK(testCompile(true));

  CHECK(testCompile(false));
  return true;
}

bool testCompile(bool nonSyntactic) {
  static const char src[] = "42\n";
  static const char16_t src_16[] = u"42\n";

  constexpr size_t length = sizeof(src) - 1;
  static_assert(sizeof(src_16) / sizeof(*src_16) - 1 == length,
                "Source buffers must be same length");

  JS::CompileOptions options(cx);
  options.setNonSyntacticScope(nonSyntactic);

<<<<<<< HEAD
  JS::SourceText<char16_t> buf;
  CHECK(buf.init(cx, src_16, length, JS::SourceOwnership::Borrowed));
||||||| merged common ancestors
    JS::RootedScript script(cx);
=======
  JS::SourceText<char16_t> buf16;
  CHECK(buf16.init(cx, src_16, length, JS::SourceOwnership::Borrowed));
>>>>>>> upstream-releases

  JS::RootedScript script(cx);

<<<<<<< HEAD
  // Check explicit non-syntactic compilation first to make sure it doesn't
  // modify our options object.
  CHECK(CompileForNonSyntacticScope(cx, options, buf, &script));
  CHECK_EQUAL(script->hasNonSyntacticScope(), true);

  CHECK(CompileLatin1ForNonSyntacticScope(cx, options, src, length, &script));
  CHECK_EQUAL(script->hasNonSyntacticScope(), true);

  {
    JS::SourceText<char16_t> srcBuf;
    CHECK(srcBuf.init(cx, src_16, length, JS::SourceOwnership::Borrowed));
||||||| merged common ancestors
    // Check explicit non-syntactic compilation first to make sure it doesn't
    // modify our options object.
    CHECK(CompileForNonSyntacticScope(cx, options, buf, &script));
    CHECK_EQUAL(script->hasNonSyntacticScope(), true);
=======
  // Check explicit non-syntactic compilation first to make sure it doesn't
  // modify our options object.
  script = CompileForNonSyntacticScope(cx, options, buf16);
  CHECK(script);
  CHECK_EQUAL(script->hasNonSyntacticScope(), true);

  JS::SourceText<mozilla::Utf8Unit> buf8;
  CHECK(buf8.init(cx, src, length, JS::SourceOwnership::Borrowed));

  script = CompileForNonSyntacticScopeDontInflate(cx, options, buf8);
  CHECK(script);
  CHECK_EQUAL(script->hasNonSyntacticScope(), true);
>>>>>>> upstream-releases

<<<<<<< HEAD
    CHECK(CompileForNonSyntacticScope(cx, options, srcBuf, &script));
||||||| merged common ancestors
    CHECK(CompileLatin1ForNonSyntacticScope(cx, options, src, length, &script));
=======
  {
    JS::SourceText<char16_t> srcBuf;
    CHECK(srcBuf.init(cx, src_16, length, JS::SourceOwnership::Borrowed));

    script = CompileForNonSyntacticScope(cx, options, srcBuf);
    CHECK(script);
>>>>>>> upstream-releases
    CHECK_EQUAL(script->hasNonSyntacticScope(), true);
  }

<<<<<<< HEAD
  CHECK(Compile(cx, options, buf, &script));
  CHECK_EQUAL(script->hasNonSyntacticScope(), nonSyntactic);
||||||| merged common ancestors
    {
        JS::SourceBufferHolder srcBuf(src_16, length, JS::SourceBufferHolder::NoOwnership);
        CHECK(CompileForNonSyntacticScope(cx, options, srcBuf, &script));
        CHECK_EQUAL(script->hasNonSyntacticScope(), true);
    }
=======
  script = Compile(cx, options, buf16);
  CHECK(script);
  CHECK_EQUAL(script->hasNonSyntacticScope(), nonSyntactic);
>>>>>>> upstream-releases

<<<<<<< HEAD
  CHECK(CompileLatin1(cx, options, src, length, &script));
  CHECK_EQUAL(script->hasNonSyntacticScope(), nonSyntactic);
||||||| merged common ancestors
=======
  script = CompileDontInflate(cx, options, buf8);
  CHECK(script);
  CHECK_EQUAL(script->hasNonSyntacticScope(), nonSyntactic);
>>>>>>> upstream-releases

  {
    JS::SourceText<char16_t> srcBuf;
    CHECK(srcBuf.init(cx, src_16, length, JS::SourceOwnership::Borrowed));

<<<<<<< HEAD
    CHECK(Compile(cx, options, srcBuf, &script));
||||||| merged common ancestors
    CHECK(CompileLatin1(cx, options, src, length, &script));
=======
    script = Compile(cx, options, srcBuf);
    CHECK(script);
>>>>>>> upstream-releases
    CHECK_EQUAL(script->hasNonSyntacticScope(), nonSyntactic);
  }

<<<<<<< HEAD
  options.forceAsync = true;
  OffThreadTask task;
  OffThreadToken* token;

  JS::SourceText<char16_t> srcBuf;
  CHECK(srcBuf.init(cx, src_16, length, JS::SourceOwnership::Borrowed));
||||||| merged common ancestors
    {
        JS::SourceBufferHolder srcBuf(src_16, length, JS::SourceBufferHolder::NoOwnership);
        CHECK(Compile(cx, options, srcBuf, &script));
        CHECK_EQUAL(script->hasNonSyntacticScope(), nonSyntactic);
    }

=======
  options.forceAsync = true;
  OffThreadTask task;
  OffThreadToken* token;
>>>>>>> upstream-releases

<<<<<<< HEAD
  CHECK(CompileOffThread(cx, options, srcBuf, task.OffThreadCallback, &task));
  CHECK(token = task.waitUntilDone(cx));
  CHECK(script = FinishOffThreadScript(cx, token));
  CHECK_EQUAL(script->hasNonSyntacticScope(), nonSyntactic);
||||||| merged common ancestors
    options.forceAsync = true;
    OffThreadTask task;
    OffThreadToken* token;

    JS::SourceBufferHolder srcBuf(src_16, length, JS::SourceBufferHolder::NoOwnership);
    CHECK(CompileOffThread(cx, options, srcBuf, task.OffThreadCallback, &task));
    CHECK(token = task.waitUntilDone(cx));
    CHECK(script = FinishOffThreadScript(cx, token));
    CHECK_EQUAL(script->hasNonSyntacticScope(), nonSyntactic);
=======
  JS::SourceText<char16_t> srcBuf;
  CHECK(srcBuf.init(cx, src_16, length, JS::SourceOwnership::Borrowed));

  CHECK(CompileOffThread(cx, options, srcBuf, task.OffThreadCallback, &task));
  CHECK(token = task.waitUntilDone(cx));
  CHECK(script = FinishOffThreadScript(cx, token));
  CHECK_EQUAL(script->hasNonSyntacticScope(), nonSyntactic);
>>>>>>> upstream-releases

  return true;
}
END_TEST(testCompileScript);
