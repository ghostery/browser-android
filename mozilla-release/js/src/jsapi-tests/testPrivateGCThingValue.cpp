/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/ArrayUtils.h"  // mozilla::ArrayLength
#include "mozilla/Utf8.h"        // mozilla::Utf8Unit

#include "jsapi.h"

#include "js/CompilationAndEvaluation.h"  // JS::CompileDontInflate
#include "js/HeapAPI.h"
#include "js/SourceText.h"  // JS::Source{Ownership,Text}
#include "jsapi-tests/tests.h"

<<<<<<< HEAD
class TestTracer : public JS::CallbackTracer {
  void onChild(const JS::GCCellPtr& thing) override {
    if (thing.asCell() == expectedCell && thing.kind() == expectedKind) {
      found = true;
||||||| merged common ancestors
class TestTracer : public JS::CallbackTracer
{
    void onChild(const JS::GCCellPtr& thing) override {
        if (thing.asCell() == expectedCell && thing.kind() == expectedKind) {
            found = true;
        }
=======
class TestTracer final : public JS::CallbackTracer {
  void onChild(const JS::GCCellPtr& thing) override {
    if (thing.asCell() == expectedCell && thing.kind() == expectedKind) {
      found = true;
>>>>>>> upstream-releases
    }
  }

 public:
  js::gc::Cell* expectedCell;
  JS::TraceKind expectedKind;
  bool found;

  explicit TestTracer(JSContext* cx)
      : JS::CallbackTracer(cx),
        expectedCell(nullptr),
        expectedKind(static_cast<JS::TraceKind>(0)),
        found(false) {}
};

<<<<<<< HEAD
static const JSClass TestClass = {"TestClass", JSCLASS_HAS_RESERVED_SLOTS(1)};
||||||| merged common ancestors
static const JSClass TestClass = {
    "TestClass",
    JSCLASS_HAS_RESERVED_SLOTS(1)
};
=======
static const JSClass TestClass = {"TestClass", JSCLASS_HAS_RESERVED_SLOTS(1)};

BEGIN_TEST(testPrivateGCThingValue) {
  JS::RootedObject obj(cx, JS_NewObject(cx, &TestClass));
  CHECK(obj);
>>>>>>> upstream-releases

<<<<<<< HEAD
BEGIN_TEST(testPrivateGCThingValue) {
  JS::RootedObject obj(cx, JS_NewObject(cx, &TestClass));
  CHECK(obj);
||||||| merged common ancestors
BEGIN_TEST(testPrivateGCThingValue)
{
    JS::RootedObject obj(cx, JS_NewObject(cx, &TestClass));
    CHECK(obj);
=======
  // Make a JSScript to stick into a PrivateGCThingValue.
  static const char code[] = "'objet petit a'";
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Make a JSScript to stick into a PrivateGCThingValue.
  const char code[] = "'objet petit a'";
||||||| merged common ancestors
    // Make a JSScript to stick into a PrivateGCThingValue.
    const char code[] = "'objet petit a'";
=======
  JS::CompileOptions options(cx);
  options.setFileAndLine(__FILE__, __LINE__);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::CompileOptions options(cx);
  options.setFileAndLine(__FILE__, __LINE__);
||||||| merged common ancestors
    JS::CompileOptions options(cx);
    options.setFileAndLine(__FILE__, __LINE__);
=======
  JS::SourceText<mozilla::Utf8Unit> srcBuf;
  CHECK(srcBuf.init(cx, code, mozilla::ArrayLength(code) - 1,
                    JS::SourceOwnership::Borrowed));
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::RootedScript script(cx);
  CHECK(JS::CompileUtf8(cx, options, code, sizeof(code) - 1, &script));
  JS_SetReservedSlot(obj, 0, PrivateGCThingValue(script));
||||||| merged common ancestors
    JS::RootedScript script(cx);
    CHECK(JS::CompileUtf8(cx, options, code, sizeof(code) - 1, &script));
    JS_SetReservedSlot(obj, 0, PrivateGCThingValue(script));
=======
  JS::RootedScript script(cx, JS::CompileDontInflate(cx, options, srcBuf));
  CHECK(script);
  JS_SetReservedSlot(obj, 0, PrivateGCThingValue(script));
>>>>>>> upstream-releases

  TestTracer trc(cx);
  trc.expectedCell = script;
  trc.expectedKind = JS::TraceKind::Script;
  JS::TraceChildren(&trc, JS::GCCellPtr(obj, JS::TraceKind::Object));
  CHECK(trc.found);

  return true;
}
END_TEST(testPrivateGCThingValue)
