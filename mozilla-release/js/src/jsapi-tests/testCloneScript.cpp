/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 *
 * Test script cloning.
 */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

<<<<<<< HEAD
#include <string.h>  // strlen
||||||| merged common ancestors
#include <string.h> // strlen
=======
#include "mozilla/Utf8.h"  // mozilla::Utf8Unit
>>>>>>> upstream-releases

<<<<<<< HEAD
#include "jsapi.h"  // sundry symbols not moved to more-specific headers yet
||||||| merged common ancestors
#include "jsapi.h" // sundry symbols not moved to more-specific headers yet
=======
#include <string.h>  // strlen

#include "jsapi.h"  // sundry symbols not moved to more-specific headers yet
>>>>>>> upstream-releases
#include "jsfriendapi.h"
<<<<<<< HEAD
#include "jspubtd.h"  // JS::AutoObjectVector
||||||| merged common ancestors
#include "jspubtd.h" // JS::AutoObjectVector
=======
#include "jspubtd.h"  // JS::RootedObjectVector
>>>>>>> upstream-releases

<<<<<<< HEAD
#include "js/CompilationAndEvaluation.h"  // JS::CompileFunction
#include "js/CompileOptions.h"            // JS::CompileOptions
#include "js/RootingAPI.h"                // JS::Rooted
#include "js/TypeDecls.h"                 // JSFunction, JSObject
||||||| merged common ancestors
#include "js/CompilationAndEvaluation.h" // JS::CompileFunction
#include "js/CompileOptions.h" // JS::CompileOptions
#include "js/RootingAPI.h" // JS::Rooted
#include "js/TypeDecls.h" // JSFunction, JSObject
=======
#include "js/CompilationAndEvaluation.h"  // JS::CompileFunction
#include "js/CompileOptions.h"            // JS::CompileOptions
#include "js/RootingAPI.h"                // JS::Rooted
#include "js/SourceText.h"                // JS::Source{Ownership,Text}
#include "js/TypeDecls.h"                 // JSFunction, JSObject
>>>>>>> upstream-releases
#include "jsapi-tests/tests.h"

<<<<<<< HEAD
BEGIN_TEST(test_cloneScript) {
  JS::RootedObject A(cx, createGlobal());
  JS::RootedObject B(cx, createGlobal());

  CHECK(A);
  CHECK(B);

  static const char source[] =
      "var i = 0;\n"
      "var sum = 0;\n"
      "while (i < 10) {\n"
      "    sum += i;\n"
      "    ++i;\n"
      "}\n"
      "(sum);\n";

  JS::RootedObject obj(cx);

  // compile for A
  {
    JSAutoRealm a(cx, A);

    JS::CompileOptions options(cx);
    options.setFileAndLine(__FILE__, 1);

    JS::RootedFunction fun(cx);
    JS::AutoObjectVector emptyScopeChain(cx);
    CHECK(JS::CompileFunctionUtf8(cx, emptyScopeChain, options, "f", 0, nullptr,
                                  source, strlen(source), &fun));
    CHECK(obj = JS_GetFunctionObject(fun));
  }

  // clone into B
  {
    JSAutoRealm b(cx, B);
    CHECK(JS::CloneFunctionObject(cx, obj));
  }

  return true;
||||||| merged common ancestors
BEGIN_TEST(test_cloneScript)
{
    JS::RootedObject A(cx, createGlobal());
    JS::RootedObject B(cx, createGlobal());

    CHECK(A);
    CHECK(B);

    static const char source[] =
        "var i = 0;\n"
        "var sum = 0;\n"
        "while (i < 10) {\n"
        "    sum += i;\n"
        "    ++i;\n"
        "}\n"
        "(sum);\n";

    JS::RootedObject obj(cx);

    // compile for A
    {
        JSAutoRealm a(cx, A);

        JS::CompileOptions options(cx);
        options.setFileAndLine(__FILE__, 1);

        JS::RootedFunction fun(cx);
        JS::AutoObjectVector emptyScopeChain(cx);
        CHECK(JS::CompileFunctionUtf8(cx, emptyScopeChain, options, "f", 0, nullptr,
                                      source, strlen(source), &fun));
        CHECK(obj = JS_GetFunctionObject(fun));
    }

    // clone into B
    {
        JSAutoRealm b(cx, B);
        CHECK(JS::CloneFunctionObject(cx, obj));
    }

    return true;
=======
BEGIN_TEST(test_cloneScript) {
  JS::RootedObject A(cx, createGlobal());
  JS::RootedObject B(cx, createGlobal());

  CHECK(A);
  CHECK(B);

  static const char source[] =
      "var i = 0;\n"
      "var sum = 0;\n"
      "while (i < 10) {\n"
      "    sum += i;\n"
      "    ++i;\n"
      "}\n"
      "(sum);\n";

  JS::RootedObject obj(cx);

  // compile for A
  {
    JSAutoRealm a(cx, A);

    JS::SourceText<mozilla::Utf8Unit> srcBuf;
    CHECK(srcBuf.init(cx, source, mozilla::ArrayLength(source) - 1,
                      JS::SourceOwnership::Borrowed));

    JS::CompileOptions options(cx);
    options.setFileAndLine(__FILE__, 1);

    JS::RootedFunction fun(cx);
    JS::RootedObjectVector emptyScopeChain(cx);
    fun = JS::CompileFunction(cx, emptyScopeChain, options, "f", 0, nullptr,
                              srcBuf);
    CHECK(fun);
    CHECK(obj = JS_GetFunctionObject(fun));
  }

  // clone into B
  {
    JSAutoRealm b(cx, B);
    CHECK(JS::CloneFunctionObject(cx, obj));
  }

  return true;
>>>>>>> upstream-releases
}
END_TEST(test_cloneScript)

<<<<<<< HEAD
struct Principals final : public JSPrincipals {
 public:
  Principals() { refcount = 0; }

  bool write(JSContext* cx, JSStructuredCloneWriter* writer) override {
    MOZ_ASSERT(false, "not imlemented");
    return false;
  }
};
||||||| merged common ancestors
struct Principals final : public JSPrincipals
{
  public:
    Principals()
    {
        refcount = 0;
    }

    bool write(JSContext* cx, JSStructuredCloneWriter* writer) override {
        MOZ_ASSERT(false, "not imlemented");
        return false;
    }
};
=======
struct Principals final : public JSPrincipals {
 public:
  Principals() { refcount = 0; }
>>>>>>> upstream-releases

<<<<<<< HEAD
class AutoDropPrincipals {
  JSContext* cx;
  JSPrincipals* principals;

 public:
  AutoDropPrincipals(JSContext* cx, JSPrincipals* principals)
      : cx(cx), principals(principals) {
    JS_HoldPrincipals(principals);
  }

  ~AutoDropPrincipals() { JS_DropPrincipals(cx, principals); }
||||||| merged common ancestors
class AutoDropPrincipals
{
    JSContext* cx;
    JSPrincipals* principals;

  public:
    AutoDropPrincipals(JSContext* cx, JSPrincipals* principals)
      : cx(cx), principals(principals)
    {
        JS_HoldPrincipals(principals);
    }

    ~AutoDropPrincipals()
    {
        JS_DropPrincipals(cx, principals);
    }
=======
  bool write(JSContext* cx, JSStructuredCloneWriter* writer) override {
    MOZ_ASSERT(false, "not imlemented");
    return false;
  }
>>>>>>> upstream-releases
};

static void DestroyPrincipals(JSPrincipals* principals) {
  auto p = static_cast<Principals*>(principals);
  delete p;
}

<<<<<<< HEAD
BEGIN_TEST(test_cloneScriptWithPrincipals) {
  JS_InitDestroyPrincipalsCallback(cx, DestroyPrincipals);
||||||| merged common ancestors
BEGIN_TEST(test_cloneScriptWithPrincipals)
{
    JS_InitDestroyPrincipalsCallback(cx, DestroyPrincipals);
=======
BEGIN_TEST(test_cloneScriptWithPrincipals) {
  JS_InitDestroyPrincipalsCallback(cx, DestroyPrincipals);

  JS::AutoHoldPrincipals principalsA(cx, new Principals());
  JS::AutoHoldPrincipals principalsB(cx, new Principals());
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSPrincipals* principalsA = new Principals();
  AutoDropPrincipals dropA(cx, principalsA);
  JSPrincipals* principalsB = new Principals();
  AutoDropPrincipals dropB(cx, principalsB);
||||||| merged common ancestors
    JSPrincipals* principalsA = new Principals();
    AutoDropPrincipals dropA(cx, principalsA);
    JSPrincipals* principalsB = new Principals();
    AutoDropPrincipals dropB(cx, principalsB);
=======
  JS::RootedObject A(cx, createGlobal(principalsA.get()));
  JS::RootedObject B(cx, createGlobal(principalsB.get()));
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::RootedObject A(cx, createGlobal(principalsA));
  JS::RootedObject B(cx, createGlobal(principalsB));
||||||| merged common ancestors
    JS::RootedObject A(cx, createGlobal(principalsA));
    JS::RootedObject B(cx, createGlobal(principalsB));
=======
  CHECK(A);
  CHECK(B);
>>>>>>> upstream-releases

<<<<<<< HEAD
  CHECK(A);
  CHECK(B);
||||||| merged common ancestors
    CHECK(A);
    CHECK(B);
=======
  const char* argnames[] = {"arg"};
  static const char source[] = "return function() { return arg; }";
>>>>>>> upstream-releases

<<<<<<< HEAD
  const char* argnames[] = {"arg"};
  static const char source[] = "return function() { return arg; }";
||||||| merged common ancestors
    const char* argnames[] = { "arg" };
    static const char source[] = "return function() { return arg; }";
=======
  JS::RootedObject obj(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::RootedObject obj(cx);
||||||| merged common ancestors
    JS::RootedObject obj(cx);
=======
  // Compile in A
  {
    JSAutoRealm a(cx, A);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Compile in A
  {
    JSAutoRealm a(cx, A);
||||||| merged common ancestors
    // Compile in A
    {
        JSAutoRealm a(cx, A);
=======
    JS::SourceText<mozilla::Utf8Unit> srcBuf;
    CHECK(srcBuf.init(cx, source, mozilla::ArrayLength(source) - 1,
                      JS::SourceOwnership::Borrowed));
>>>>>>> upstream-releases

    JS::CompileOptions options(cx);
    options.setFileAndLine(__FILE__, 1);

<<<<<<< HEAD
    JS::RootedFunction fun(cx);
    JS::AutoObjectVector emptyScopeChain(cx);
    CHECK(JS::CompileFunctionUtf8(cx, emptyScopeChain, options, "f",
                                  mozilla::ArrayLength(argnames), argnames,
                                  source, strlen(source), &fun));
    CHECK(fun);
||||||| merged common ancestors
        JS::RootedFunction fun(cx);
        JS::AutoObjectVector emptyScopeChain(cx);
        CHECK(JS::CompileFunctionUtf8(cx, emptyScopeChain, options, "f",
                                      mozilla::ArrayLength(argnames), argnames,
                                      source, strlen(source), &fun));
        CHECK(fun);
=======
    JS::RootedFunction fun(cx);
    JS::RootedObjectVector emptyScopeChain(cx);
    fun = JS::CompileFunction(cx, emptyScopeChain, options, "f",
                              mozilla::ArrayLength(argnames), argnames, srcBuf);
    CHECK(fun);
>>>>>>> upstream-releases

    JSScript* script;
    CHECK(script = JS_GetFunctionScript(cx, fun));

<<<<<<< HEAD
    CHECK(JS_GetScriptPrincipals(script) == principalsA);
    CHECK(obj = JS_GetFunctionObject(fun));
  }
||||||| merged common ancestors
        CHECK(JS_GetScriptPrincipals(script) == principalsA);
        CHECK(obj = JS_GetFunctionObject(fun));
    }
=======
    CHECK(JS_GetScriptPrincipals(script) == principalsA.get());
    CHECK(obj = JS_GetFunctionObject(fun));
  }
>>>>>>> upstream-releases

  // Clone into B
  {
    JSAutoRealm b(cx, B);
    JS::RootedObject cloned(cx);
    CHECK(cloned = JS::CloneFunctionObject(cx, obj));

    JS::RootedFunction fun(cx);
    JS::RootedValue clonedValue(cx, JS::ObjectValue(*cloned));
    CHECK(fun = JS_ValueToFunction(cx, clonedValue));

    JSScript* script;
    CHECK(script = JS_GetFunctionScript(cx, fun));

<<<<<<< HEAD
    CHECK(JS_GetScriptPrincipals(script) == principalsB);
||||||| merged common ancestors
        CHECK(JS_GetScriptPrincipals(script) == principalsB);
=======
    CHECK(JS_GetScriptPrincipals(script) == principalsB.get());
>>>>>>> upstream-releases

    JS::RootedValue v(cx);
    JS::RootedValue arg(cx, JS::Int32Value(1));
    CHECK(JS_CallFunctionValue(cx, B, clonedValue, JS::HandleValueArray(arg),
                               &v));
    CHECK(v.isObject());

<<<<<<< HEAD
    JSObject* funobj = &v.toObject();
    CHECK(JS_ObjectIsFunction(cx, funobj));
    CHECK(fun = JS_ValueToFunction(cx, v));
    CHECK(script = JS_GetFunctionScript(cx, fun));
    CHECK(JS_GetScriptPrincipals(script) == principalsB);
  }
||||||| merged common ancestors
        JSObject* funobj = &v.toObject();
        CHECK(JS_ObjectIsFunction(cx, funobj));
        CHECK(fun = JS_ValueToFunction(cx, v));
        CHECK(script = JS_GetFunctionScript(cx, fun));
        CHECK(JS_GetScriptPrincipals(script) == principalsB);
    }
=======
    JSObject* funobj = &v.toObject();
    CHECK(JS_ObjectIsFunction(funobj));
    CHECK(fun = JS_ValueToFunction(cx, v));
    CHECK(script = JS_GetFunctionScript(cx, fun));
    CHECK(JS_GetScriptPrincipals(script) == principalsB.get());
  }
>>>>>>> upstream-releases

  return true;
}
END_TEST(test_cloneScriptWithPrincipals)
