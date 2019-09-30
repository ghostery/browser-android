/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "jsapi.h"
#include "jspubtd.h"

#include "gc/Heap.h"
#include "js/CompilationAndEvaluation.h"  // JS::CompileDontInflate
#include "js/SourceText.h"                // JS::Source{Ownership,Text}
#include "jsapi-tests/tests.h"

<<<<<<< HEAD
JS::GCCellPtr GivesAndTakesCells(JS::GCCellPtr cell) { return cell; }
||||||| merged common ancestors
JS::GCCellPtr
GivesAndTakesCells(JS::GCCellPtr cell)
{
    return cell;
}
=======
JS::GCCellPtr GivesAndTakesCells(JS::GCCellPtr cell) { return cell; }

BEGIN_TEST(testGCCellPtr) {
  JS::RootedObject obj(cx, JS_NewPlainObject(cx));
  CHECK(obj);
>>>>>>> upstream-releases

<<<<<<< HEAD
BEGIN_TEST(testGCCellPtr) {
  JS::RootedObject obj(cx, JS_NewPlainObject(cx));
  CHECK(obj);
||||||| merged common ancestors
BEGIN_TEST(testGCCellPtr)
{
    JS::RootedObject obj(cx, JS_NewPlainObject(cx));
    CHECK(obj);
=======
  JS::RootedString str(cx, JS_NewStringCopyZ(cx, "probably foobar"));
  CHECK(str);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::RootedString str(cx, JS_NewStringCopyZ(cx, "probably foobar"));
  CHECK(str);
||||||| merged common ancestors
    JS::RootedString str(cx, JS_NewStringCopyZ(cx, "probably foobar"));
    CHECK(str);
=======
  const char* code = "function foo() { return 'bar'; }";
>>>>>>> upstream-releases

<<<<<<< HEAD
  const char* code = "function foo() { return 'bar'; }";
||||||| merged common ancestors
    const char* code = "function foo() { return 'bar'; }";
=======
  JS::CompileOptions opts(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::CompileOptions opts(cx);
||||||| merged common ancestors
    JS::CompileOptions opts(cx);
=======
  JS::SourceText<mozilla::Utf8Unit> srcBuf;
  CHECK(srcBuf.init(cx, code, strlen(code), JS::SourceOwnership::Borrowed));
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::RootedScript script(cx);
  CHECK(JS::CompileUtf8(cx, opts, code, strlen(code), &script));
  CHECK(script);
||||||| merged common ancestors
    JS::RootedScript script(cx);
    CHECK(JS::CompileUtf8(cx, opts, code, strlen(code), &script));
    CHECK(script);
=======
  JS::RootedScript script(cx, JS::CompileDontInflate(cx, opts, srcBuf));
  CHECK(script);
>>>>>>> upstream-releases

  CHECK(!JS::GCCellPtr(nullptr));

  CHECK(JS::GCCellPtr(obj.get()));
  CHECK(JS::GCCellPtr(obj.get()).kind() == JS::TraceKind::Object);
  CHECK(JS::GCCellPtr(JS::ObjectValue(*obj)).kind() == JS::TraceKind::Object);

  CHECK(JS::GCCellPtr(str.get()));
  CHECK(JS::GCCellPtr(str.get()).kind() == JS::TraceKind::String);
  CHECK(JS::GCCellPtr(JS::StringValue(str)).kind() == JS::TraceKind::String);

  CHECK(JS::GCCellPtr(script.get()));
  CHECK(!JS::GCCellPtr(nullptr));
  CHECK(JS::GCCellPtr(script.get()).kind() == JS::TraceKind::Script);

  JS::GCCellPtr objcell(obj.get());
  JS::GCCellPtr scriptcell = JS::GCCellPtr(script.get());
  CHECK(GivesAndTakesCells(objcell));
  CHECK(GivesAndTakesCells(scriptcell));

  JS::GCCellPtr copy = objcell;
  CHECK(copy == objcell);

  return true;
}
END_TEST(testGCCellPtr)
