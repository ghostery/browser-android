/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

<<<<<<< HEAD
#include "js/CompilationAndEvaluation.h"
#include "js/MemoryFunctions.h"
||||||| merged common ancestors
#include "js/CompilationAndEvaluation.h"
=======
#include "mozilla/Utf8.h"  // mozilla::Utf8Unit

#include "js/CompilationAndEvaluation.h"  // JS::CompileFunction, JS::EvaluateDontInflate
#include "js/MemoryFunctions.h"
#include "js/SourceText.h"  // JS::Source{Ownership,Text}
>>>>>>> upstream-releases
#include "jsapi-tests/tests.h"
#include "vm/JSScript.h"

<<<<<<< HEAD
BEGIN_TEST(testBug795104) {
  JS::RealmBehaviorsRef(cx->realm()).setDiscardSource(true);
||||||| merged common ancestors
BEGIN_TEST(testBug795104)
{
    JS::RealmBehaviorsRef(cx->realm()).setDiscardSource(true);
=======
BEGIN_TEST(testBug795104) {
  JS::RealmBehaviorsRef(cx->realm()).setDiscardSource(true);

  const size_t strLen = 60002;
  char* s = static_cast<char*>(JS_malloc(cx, strLen));
  CHECK(s);
>>>>>>> upstream-releases

<<<<<<< HEAD
  const size_t strLen = 60002;
  char* s = static_cast<char*>(JS_malloc(cx, strLen));
  CHECK(s);
||||||| merged common ancestors
    const size_t strLen = 60002;
    char* s = static_cast<char*>(JS_malloc(cx, strLen));
    CHECK(s);
=======
  s[0] = '"';
  memset(s + 1, 'x', strLen - 2);
  s[strLen - 1] = '"';
>>>>>>> upstream-releases

<<<<<<< HEAD
  s[0] = '"';
  memset(s + 1, 'x', strLen - 2);
  s[strLen - 1] = '"';
||||||| merged common ancestors
    s[0] = '"';
    memset(s + 1, 'x', strLen - 2);
    s[strLen - 1] = '"';
=======
  JS::SourceText<mozilla::Utf8Unit> srcBuf;
  CHECK(srcBuf.init(cx, s, strLen, JS::SourceOwnership::Borrowed));
>>>>>>> upstream-releases

  JS::CompileOptions opts(cx);

<<<<<<< HEAD
  // We don't want an rval for our Evaluate call
  opts.setNoScriptRval(true);
||||||| merged common ancestors
    // We don't want an rval for our Evaluate call
    opts.setNoScriptRval(true);
=======
  // We don't want an rval for our JS::EvaluateDontInflate call
  opts.setNoScriptRval(true);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::RootedValue unused(cx);
  CHECK(JS::EvaluateUtf8(cx, opts, s, strLen, &unused));
||||||| merged common ancestors
    JS::RootedValue unused(cx);
    CHECK(JS::EvaluateUtf8(cx, opts, s, strLen, &unused));
=======
  JS::RootedValue unused(cx);
  CHECK(JS::EvaluateDontInflate(cx, opts, srcBuf, &unused));
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::RootedFunction fun(cx);
  JS::AutoObjectVector emptyScopeChain(cx);
||||||| merged common ancestors
    JS::RootedFunction fun(cx);
    JS::AutoObjectVector emptyScopeChain(cx);
=======
  JS::RootedFunction fun(cx);
  JS::RootedObjectVector emptyScopeChain(cx);
>>>>>>> upstream-releases

  // But when compiling a function we don't want to use no-rval
  // mode, since it's not supported for functions.
  opts.setNoScriptRval(false);

<<<<<<< HEAD
  CHECK(JS::CompileFunctionUtf8(cx, emptyScopeChain, opts, "f", 0, nullptr, s,
                                strLen, &fun));
  CHECK(fun);
||||||| merged common ancestors
    CHECK(JS::CompileFunctionUtf8(cx, emptyScopeChain, opts, "f", 0, nullptr, s, strLen, &fun));
    CHECK(fun);
=======
  fun = JS::CompileFunction(cx, emptyScopeChain, opts, "f", 0, nullptr, srcBuf);
  CHECK(fun);
>>>>>>> upstream-releases

  JS_free(cx, s);

  return true;
}
END_TEST(testBug795104)
