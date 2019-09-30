/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "js/RegExp.h"
#include "js/RegExpFlags.h"
#include "jsapi-tests/tests.h"

BEGIN_TEST(testObjectIsRegExp) {
  JS::RootedValue val(cx);

  bool isRegExp;

<<<<<<< HEAD
  EVAL("new Object", &val);
  JS::RootedObject obj(cx, val.toObjectOrNull());
  CHECK(JS_ObjectIsRegExp(cx, obj, &isRegExp));
  CHECK(!isRegExp);
||||||| merged common ancestors
    EVAL("new Object", &val);
    JS::RootedObject obj(cx, val.toObjectOrNull());
    CHECK(JS_ObjectIsRegExp(cx, obj, &isRegExp));
    CHECK(!isRegExp);
=======
  EVAL("new Object", &val);
  JS::RootedObject obj(cx, val.toObjectOrNull());
  CHECK(JS::ObjectIsRegExp(cx, obj, &isRegExp));
  CHECK(!isRegExp);
>>>>>>> upstream-releases

<<<<<<< HEAD
  EVAL("/foopy/", &val);
  obj = val.toObjectOrNull();
  CHECK(JS_ObjectIsRegExp(cx, obj, &isRegExp));
  CHECK(isRegExp);
||||||| merged common ancestors
    EVAL("/foopy/", &val);
    obj = val.toObjectOrNull();
    CHECK(JS_ObjectIsRegExp(cx, obj, &isRegExp));
    CHECK(isRegExp);
=======
  EVAL("/foopy/", &val);
  obj = val.toObjectOrNull();
  CHECK(JS::ObjectIsRegExp(cx, obj, &isRegExp));
  CHECK(isRegExp);
>>>>>>> upstream-releases

  return true;
}
END_TEST(testObjectIsRegExp)

BEGIN_TEST(testGetRegExpFlags) {
  JS::RootedValue val(cx);
  JS::RootedObject obj(cx);

<<<<<<< HEAD
  EVAL("/foopy/", &val);
  obj = val.toObjectOrNull();
  CHECK_EQUAL(JS_GetRegExpFlags(cx, obj), 0u);
||||||| merged common ancestors
    EVAL("/foopy/", &val);
    obj = val.toObjectOrNull();
    CHECK_EQUAL(JS_GetRegExpFlags(cx, obj), 0u);
=======
  EVAL("/foopy/", &val);
  obj = val.toObjectOrNull();
  CHECK_EQUAL(JS::GetRegExpFlags(cx, obj),
              JS::RegExpFlags(JS::RegExpFlag::NoFlags));
>>>>>>> upstream-releases

<<<<<<< HEAD
  EVAL("/foopy/g", &val);
  obj = val.toObjectOrNull();
  CHECK_EQUAL(JS_GetRegExpFlags(cx, obj), JSREG_GLOB);
||||||| merged common ancestors
    EVAL("/foopy/g", &val);
    obj = val.toObjectOrNull();
    CHECK_EQUAL(JS_GetRegExpFlags(cx, obj), JSREG_GLOB);
=======
  EVAL("/foopy/g", &val);
  obj = val.toObjectOrNull();
  CHECK_EQUAL(JS::GetRegExpFlags(cx, obj),
              JS::RegExpFlags(JS::RegExpFlag::Global));
>>>>>>> upstream-releases

<<<<<<< HEAD
  EVAL("/foopy/gi", &val);
  obj = val.toObjectOrNull();
  CHECK_EQUAL(JS_GetRegExpFlags(cx, obj), (JSREG_FOLD | JSREG_GLOB));
||||||| merged common ancestors
    EVAL("/foopy/gi", &val);
    obj = val.toObjectOrNull();
    CHECK_EQUAL(JS_GetRegExpFlags(cx, obj), (JSREG_FOLD | JSREG_GLOB));
=======
  EVAL("/foopy/gi", &val);
  obj = val.toObjectOrNull();
  CHECK_EQUAL(
      JS::GetRegExpFlags(cx, obj),
      JS::RegExpFlags(JS::RegExpFlag::Global | JS::RegExpFlag::IgnoreCase));
>>>>>>> upstream-releases

  return true;
}
END_TEST(testGetRegExpFlags)

BEGIN_TEST(testGetRegExpSource) {
  JS::RootedValue val(cx);
  JS::RootedObject obj(cx);

<<<<<<< HEAD
  EVAL("/foopy/", &val);
  obj = val.toObjectOrNull();
  JSString* source = JS_GetRegExpSource(cx, obj);
  CHECK(JS_FlatStringEqualsAscii(JS_ASSERT_STRING_IS_FLAT(source), "foopy"));
||||||| merged common ancestors
    EVAL("/foopy/", &val);
    obj = val.toObjectOrNull();
    JSString* source = JS_GetRegExpSource(cx, obj);
    CHECK(JS_FlatStringEqualsAscii(JS_ASSERT_STRING_IS_FLAT(source), "foopy"));
=======
  EVAL("/foopy/", &val);
  obj = val.toObjectOrNull();
  JSString* source = JS::GetRegExpSource(cx, obj);
  CHECK(source);
  CHECK(JS_FlatStringEqualsAscii(JS_ASSERT_STRING_IS_FLAT(source), "foopy"));
>>>>>>> upstream-releases

  return true;
}
END_TEST(testGetRegExpSource)
