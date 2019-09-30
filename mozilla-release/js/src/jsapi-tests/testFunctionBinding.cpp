/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 *
 * Test function name binding.
 */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/Utf8.h"  // mozilla::Utf8Unit

#include "jsfriendapi.h"

#include "js/CompilationAndEvaluation.h"  // JS::CompileFunction
#include "js/SourceText.h"                // JS::Source{Ownership,Text}
#include "jsapi-tests/tests.h"

using namespace js;

<<<<<<< HEAD
BEGIN_TEST(test_functionBinding) {
  RootedFunction fun(cx);

  JS::CompileOptions options(cx);
  options.setFileAndLine(__FILE__, __LINE__);

  // Named function shouldn't have it's binding.
  const char s1chars[] = "return (typeof s1) == 'undefined';";
  JS::AutoObjectVector emptyScopeChain(cx);
  CHECK(JS::CompileFunctionUtf8(cx, emptyScopeChain, options, "s1", 0, nullptr,
                                s1chars, strlen(s1chars), &fun));
  CHECK(fun);

  JS::AutoValueVector args(cx);
  RootedValue rval(cx);
  CHECK(JS::Call(cx, UndefinedHandleValue, fun, args, &rval));
  CHECK(rval.isBoolean());
  CHECK(rval.toBoolean());

  // Named function shouldn't have `anonymous` binding.
  const char s2chars[] = "return (typeof anonymous) == 'undefined';";
  CHECK(JS::CompileFunctionUtf8(cx, emptyScopeChain, options, "s2", 0, nullptr,
                                s2chars, strlen(s2chars), &fun));
  CHECK(fun);

  CHECK(JS::Call(cx, UndefinedHandleValue, fun, args, &rval));
  CHECK(rval.isBoolean());
  CHECK(rval.toBoolean());

  // Anonymous function shouldn't have `anonymous` binding.
  const char s3chars[] = "return (typeof anonymous) == 'undefined';";
  CHECK(JS::CompileFunctionUtf8(cx, emptyScopeChain, options, nullptr, 0,
                                nullptr, s3chars, strlen(s3chars), &fun));
  CHECK(fun);

  CHECK(JS::Call(cx, UndefinedHandleValue, fun, args, &rval));
  CHECK(rval.isBoolean());
  CHECK(rval.toBoolean());

  return true;
||||||| merged common ancestors
BEGIN_TEST(test_functionBinding)
{
    RootedFunction fun(cx);

    JS::CompileOptions options(cx);
    options.setFileAndLine(__FILE__, __LINE__);

    // Named function shouldn't have it's binding.
    const char s1chars[] = "return (typeof s1) == 'undefined';";
    JS::AutoObjectVector emptyScopeChain(cx);
    CHECK(JS::CompileFunctionUtf8(cx, emptyScopeChain, options, "s1", 0, nullptr,
                                  s1chars, strlen(s1chars), &fun));
    CHECK(fun);

    JS::AutoValueVector args(cx);
    RootedValue rval(cx);
    CHECK(JS::Call(cx, UndefinedHandleValue, fun, args, &rval));
    CHECK(rval.isBoolean());
    CHECK(rval.toBoolean());

    // Named function shouldn't have `anonymous` binding.
    const char s2chars[] = "return (typeof anonymous) == 'undefined';";
    CHECK(JS::CompileFunctionUtf8(cx, emptyScopeChain, options, "s2", 0, nullptr,
                                  s2chars, strlen(s2chars), &fun));
    CHECK(fun);

    CHECK(JS::Call(cx, UndefinedHandleValue, fun, args, &rval));
    CHECK(rval.isBoolean());
    CHECK(rval.toBoolean());

    // Anonymous function shouldn't have `anonymous` binding.
    const char s3chars[] = "return (typeof anonymous) == 'undefined';";
    CHECK(JS::CompileFunctionUtf8(cx, emptyScopeChain, options, nullptr, 0, nullptr,
                                  s3chars, strlen(s3chars), &fun));
    CHECK(fun);

    CHECK(JS::Call(cx, UndefinedHandleValue, fun, args, &rval));
    CHECK(rval.isBoolean());
    CHECK(rval.toBoolean());

    return true;
=======
BEGIN_TEST(test_functionBinding) {
  RootedFunction fun(cx);

  JS::CompileOptions options(cx);
  options.setFileAndLine(__FILE__, __LINE__);

  JS::RootedObjectVector emptyScopeChain(cx);

  // Named function shouldn't have it's binding.
  {
    static const char s1chars[] = "return (typeof s1) == 'undefined';";

    JS::SourceText<mozilla::Utf8Unit> srcBuf;
    CHECK(srcBuf.init(cx, s1chars, mozilla::ArrayLength(s1chars) - 1,
                      JS::SourceOwnership::Borrowed));

    fun = JS::CompileFunction(cx, emptyScopeChain, options, "s1", 0, nullptr,
                              srcBuf);
    CHECK(fun);
  }

  JS::RootedValueVector args(cx);
  RootedValue rval(cx);
  CHECK(JS::Call(cx, UndefinedHandleValue, fun, args, &rval));
  CHECK(rval.isBoolean());
  CHECK(rval.toBoolean());

  // Named function shouldn't have `anonymous` binding.
  {
    static const char s2chars[] = "return (typeof anonymous) == 'undefined';";

    JS::SourceText<mozilla::Utf8Unit> srcBuf;
    CHECK(srcBuf.init(cx, s2chars, mozilla::ArrayLength(s2chars) - 1,
                      JS::SourceOwnership::Borrowed));

    fun = JS::CompileFunction(cx, emptyScopeChain, options, "s2", 0, nullptr,
                              srcBuf);
    CHECK(fun);
  }

  CHECK(JS::Call(cx, UndefinedHandleValue, fun, args, &rval));
  CHECK(rval.isBoolean());
  CHECK(rval.toBoolean());

  // Anonymous function shouldn't have `anonymous` binding.
  {
    static const char s3chars[] = "return (typeof anonymous) == 'undefined';";

    JS::SourceText<mozilla::Utf8Unit> srcBuf;
    CHECK(srcBuf.init(cx, s3chars, mozilla::ArrayLength(s3chars) - 1,
                      JS::SourceOwnership::Borrowed));

    fun = JS::CompileFunction(cx, emptyScopeChain, options, nullptr, 0, nullptr,
                              srcBuf);
    CHECK(fun);
  }

  CHECK(JS::Call(cx, UndefinedHandleValue, fun, args, &rval));
  CHECK(rval.isBoolean());
  CHECK(rval.toBoolean());

  return true;
>>>>>>> upstream-releases
}
END_TEST(test_functionBinding)
