/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 */

#include "jsfriendapi.h"
#include "js/MemoryFunctions.h"

#include "builtin/TestingFunctions.h"
#include "js/ArrayBuffer.h"  // JS::{GetArrayBuffer{ByteLength,Data},IsArrayBufferObject,NewArrayBuffer{,WithContents},StealArrayBufferContents}
#include "js/MemoryFunctions.h"
#include "jsapi-tests/tests.h"

BEGIN_TEST(testArrayBuffer_bug720949_steal) {
  static const unsigned NUM_TEST_BUFFERS = 2;
  static const unsigned MAGIC_VALUE_1 = 3;
  static const unsigned MAGIC_VALUE_2 = 17;

  JS::RootedObject buf_len1(cx), buf_len200(cx);
  JS::RootedObject tarray_len1(cx), tarray_len200(cx);

  uint32_t sizes[NUM_TEST_BUFFERS] = {sizeof(uint32_t), 200 * sizeof(uint32_t)};
  JS::HandleObject testBuf[NUM_TEST_BUFFERS] = {buf_len1, buf_len200};
  JS::HandleObject testArray[NUM_TEST_BUFFERS] = {tarray_len1, tarray_len200};

<<<<<<< HEAD
  // Single-element ArrayBuffer (uses fixed slots for storage)
  CHECK(buf_len1 = JS_NewArrayBuffer(cx, sizes[0]));
  CHECK(tarray_len1 = JS_NewInt32ArrayWithBuffer(cx, testBuf[0], 0, -1));
||||||| merged common ancestors
    // No views
    buffer = JS_NewArrayBuffer(cx, 2000);
    buffer = nullptr;
    GC(cx);
=======
  // Single-element ArrayBuffer (uses fixed slots for storage)
  CHECK(buf_len1 = JS::NewArrayBuffer(cx, sizes[0]));
  CHECK(tarray_len1 = JS_NewInt32ArrayWithBuffer(cx, testBuf[0], 0, -1));
>>>>>>> upstream-releases

<<<<<<< HEAD
  CHECK(JS_SetElement(cx, testArray[0], 0, MAGIC_VALUE_1));

  // Many-element ArrayBuffer (uses dynamic storage)
  CHECK(buf_len200 = JS_NewArrayBuffer(cx, 200 * sizeof(uint32_t)));
  CHECK(tarray_len200 = JS_NewInt32ArrayWithBuffer(cx, testBuf[1], 0, -1));

  for (unsigned i = 0; i < NUM_TEST_BUFFERS; i++) {
    JS::HandleObject obj = testBuf[i];
    JS::HandleObject view = testArray[i];
    uint32_t size = sizes[i];
    JS::RootedValue v(cx);
||||||| merged common ancestors
    // One view.
    {
        buffer = JS_NewArrayBuffer(cx, 2000);
        JS::RootedObject view(cx, JS_NewUint8ArrayWithBuffer(cx, buffer, 0, -1));
        void* contents = JS_StealArrayBufferContents(cx, buffer);
        CHECK(contents != nullptr);
        JS_free(nullptr, contents);
        GC(cx);
        CHECK(hasDetachedBuffer(view));
        CHECK(JS_IsDetachedArrayBufferObject(buffer));
        view = nullptr;
        GC(cx);
        buffer = nullptr;
        GC(cx);
    }
=======
  CHECK(JS_SetElement(cx, testArray[0], 0, MAGIC_VALUE_1));
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Byte lengths should all agree
    CHECK(JS_IsArrayBufferObject(obj));
    CHECK_EQUAL(JS_GetArrayBufferByteLength(obj), size);
    CHECK(JS_GetProperty(cx, obj, "byteLength", &v));
    CHECK(v.isInt32(size));
    CHECK(JS_GetProperty(cx, view, "byteLength", &v));
    CHECK(v.isInt32(size));

    // Modifying the underlying data should update the value returned through
    // the view
||||||| merged common ancestors
    // Two views
=======
  // Many-element ArrayBuffer (uses dynamic storage)
  CHECK(buf_len200 = JS::NewArrayBuffer(cx, 200 * sizeof(uint32_t)));
  CHECK(tarray_len200 = JS_NewInt32ArrayWithBuffer(cx, testBuf[1], 0, -1));

  for (unsigned i = 0; i < NUM_TEST_BUFFERS; i++) {
    JS::HandleObject obj = testBuf[i];
    JS::HandleObject view = testArray[i];
    uint32_t size = sizes[i];
    JS::RootedValue v(cx);

    // Byte lengths should all agree
    CHECK(JS::IsArrayBufferObject(obj));
    CHECK_EQUAL(JS::GetArrayBufferByteLength(obj), size);
    CHECK(JS_GetProperty(cx, obj, "byteLength", &v));
    CHECK(v.isInt32(size));
    CHECK(JS_GetProperty(cx, view, "byteLength", &v));
    CHECK(v.isInt32(size));

    // Modifying the underlying data should update the value returned through
    // the view
>>>>>>> upstream-releases
    {
<<<<<<< HEAD
      JS::AutoCheckCannotGC nogc;
      bool sharedDummy;
      uint8_t* data = JS_GetArrayBufferData(obj, &sharedDummy, nogc);
      CHECK(data != nullptr);
      *reinterpret_cast<uint32_t*>(data) = MAGIC_VALUE_2;
||||||| merged common ancestors
        buffer = JS_NewArrayBuffer(cx, 2000);

        JS::RootedObject view1(cx, JS_NewUint8ArrayWithBuffer(cx, buffer, 0, -1));
        JS::RootedObject view2(cx, JS_NewUint8ArrayWithBuffer(cx, buffer, 1, 200));

        // Remove, re-add a view
        view2 = nullptr;
        GC(cx);
        view2 = JS_NewUint8ArrayWithBuffer(cx, buffer, 1, 200);

        // Detach
        void* contents = JS_StealArrayBufferContents(cx, buffer);
        CHECK(contents != nullptr);
        JS_free(nullptr, contents);

        CHECK(hasDetachedBuffer(view1));
        CHECK(hasDetachedBuffer(view2));
        CHECK(JS_IsDetachedArrayBufferObject(buffer));

        view1 = nullptr;
        GC(cx);
        view2 = nullptr;
        GC(cx);
        buffer = nullptr;
        GC(cx);
=======
      JS::AutoCheckCannotGC nogc;
      bool sharedDummy;
      uint8_t* data = JS::GetArrayBufferData(obj, &sharedDummy, nogc);
      CHECK(data != nullptr);
      *reinterpret_cast<uint32_t*>(data) = MAGIC_VALUE_2;
>>>>>>> upstream-releases
    }
    CHECK(JS_GetElement(cx, view, 0, &v));
    CHECK(v.isInt32(MAGIC_VALUE_2));

<<<<<<< HEAD
    // Steal the contents
    void* contents = JS_StealArrayBufferContents(cx, obj);
    CHECK(contents != nullptr);
||||||| merged common ancestors
    return true;
}
=======
    // Steal the contents
    void* contents = JS::StealArrayBufferContents(cx, obj);
    CHECK(contents != nullptr);
>>>>>>> upstream-releases

<<<<<<< HEAD
    CHECK(JS_IsDetachedArrayBufferObject(obj));
||||||| merged common ancestors
static void GC(JSContext* cx)
{
    JS_GC(cx);
    JS_GC(cx); // Trigger another to wait for background finalization to end
}
=======
    CHECK(JS::IsDetachedArrayBufferObject(obj));
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Transfer to a new ArrayBuffer
    JS::RootedObject dst(cx, JS_NewArrayBufferWithContents(cx, size, contents));
    CHECK(JS_IsArrayBufferObject(dst));
    {
      JS::AutoCheckCannotGC nogc;
      bool sharedDummy;
      (void)JS_GetArrayBufferData(obj, &sharedDummy, nogc);
    }
||||||| merged common ancestors
bool hasDetachedBuffer(JS::HandleObject obj) {
    JS::RootedValue v(cx);
    return JS_GetProperty(cx, obj, "byteLength", &v) && v.toInt32() == 0;
}
=======
    // Transfer to a new ArrayBuffer
    JS::RootedObject dst(cx,
                         JS::NewArrayBufferWithContents(cx, size, contents));
    CHECK(JS::IsArrayBufferObject(dst));
    {
      JS::AutoCheckCannotGC nogc;
      bool sharedDummy;
      (void)JS::GetArrayBufferData(obj, &sharedDummy, nogc);
    }
>>>>>>> upstream-releases

    JS::RootedObject dstview(cx, JS_NewInt32ArrayWithBuffer(cx, dst, 0, -1));
    CHECK(dstview != nullptr);

<<<<<<< HEAD
    CHECK_EQUAL(JS_GetArrayBufferByteLength(dst), size);
    {
      JS::AutoCheckCannotGC nogc;
      bool sharedDummy;
      uint8_t* data = JS_GetArrayBufferData(dst, &sharedDummy, nogc);
      CHECK(data != nullptr);
      CHECK_EQUAL(*reinterpret_cast<uint32_t*>(data), MAGIC_VALUE_2);
||||||| merged common ancestors
BEGIN_TEST(testArrayBuffer_externalize)
{
    if (!testWithSize(cx, 2)) {  // ArrayBuffer data stored inline in the object.
        return false;
    }
    if (!testWithSize(cx, 2000)) { // ArrayBuffer data stored out-of-line in a separate heap allocation.
        return false;
=======
    CHECK_EQUAL(JS::GetArrayBufferByteLength(dst), size);
    {
      JS::AutoCheckCannotGC nogc;
      bool sharedDummy;
      uint8_t* data = JS::GetArrayBufferData(dst, &sharedDummy, nogc);
      CHECK(data != nullptr);
      CHECK_EQUAL(*reinterpret_cast<uint32_t*>(data), MAGIC_VALUE_2);
>>>>>>> upstream-releases
    }
    CHECK(JS_GetElement(cx, dstview, 0, &v));
    CHECK(v.isInt32(MAGIC_VALUE_2));
  }

  return true;
}
END_TEST(testArrayBuffer_bug720949_steal)

// Varying number of views of a buffer, to test the detachment weak pointers
BEGIN_TEST(testArrayBuffer_bug720949_viewList) {
  JS::RootedObject buffer(cx);

<<<<<<< HEAD
  // No views
  buffer = JS_NewArrayBuffer(cx, 2000);
  buffer = nullptr;
  GC(cx);
||||||| merged common ancestors
    JS::RootedObject view(cx, JS_NewUint8ArrayWithBuffer(cx, buffer, 0, -1));
    CHECK(view != nullptr);
=======
  // No views
  buffer = JS::NewArrayBuffer(cx, 2000);
  buffer = nullptr;
  GC(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // One view.
  {
    buffer = JS_NewArrayBuffer(cx, 2000);
    JS::RootedObject view(cx, JS_NewUint8ArrayWithBuffer(cx, buffer, 0, -1));
    void* contents = JS_StealArrayBufferContents(cx, buffer);
||||||| merged common ancestors
    void* contents = JS_ExternalizeArrayBufferContents(cx, buffer);
=======
  // One view.
  {
    buffer = JS::NewArrayBuffer(cx, 2000);
    JS::RootedObject view(cx, JS_NewUint8ArrayWithBuffer(cx, buffer, 0, -1));
    void* contents = JS::StealArrayBufferContents(cx, buffer);
>>>>>>> upstream-releases
    CHECK(contents != nullptr);
<<<<<<< HEAD
    JS_free(nullptr, contents);
    GC(cx);
    CHECK(hasDetachedBuffer(view));
    CHECK(JS_IsDetachedArrayBufferObject(buffer));
    view = nullptr;
    GC(cx);
    buffer = nullptr;
    GC(cx);
  }
||||||| merged common ancestors
    uint32_t actualLength;
    CHECK(hasExpectedLength(cx, view, &actualLength));
    CHECK(actualLength == n);
    CHECK(!JS_IsDetachedArrayBufferObject(buffer));
    CHECK(JS_GetArrayBufferByteLength(buffer) == uint32_t(n));
=======
    JS_free(nullptr, contents);
    GC(cx);
    CHECK(hasDetachedBuffer(view));
    CHECK(JS::IsDetachedArrayBufferObject(buffer));
    view = nullptr;
    GC(cx);
    buffer = nullptr;
    GC(cx);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Two views
  {
    buffer = JS_NewArrayBuffer(cx, 2000);
||||||| merged common ancestors
    uint8_t* uint8Contents = static_cast<uint8_t*>(contents);
    CHECK(*uint8Contents == 0);
    uint8_t randomByte(rand() % UINT8_MAX);
    *uint8Contents = randomByte;
=======
  // Two views
  {
    buffer = JS::NewArrayBuffer(cx, 2000);
>>>>>>> upstream-releases

    JS::RootedObject view1(cx, JS_NewUint8ArrayWithBuffer(cx, buffer, 0, -1));
    JS::RootedObject view2(cx, JS_NewUint8ArrayWithBuffer(cx, buffer, 1, 200));

    // Remove, re-add a view
    view2 = nullptr;
    GC(cx);
<<<<<<< HEAD
    view2 = JS_NewUint8ArrayWithBuffer(cx, buffer, 1, 200);
||||||| merged common ancestors
=======
    view2 = JS_NewUint8ArrayWithBuffer(cx, buffer, 1, 200);

    // Detach
    void* contents = JS::StealArrayBufferContents(cx, buffer);
    CHECK(contents != nullptr);
    JS_free(nullptr, contents);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Detach
    void* contents = JS_StealArrayBufferContents(cx, buffer);
    CHECK(contents != nullptr);
    JS_free(nullptr, contents);

    CHECK(hasDetachedBuffer(view1));
    CHECK(hasDetachedBuffer(view2));
    CHECK(JS_IsDetachedArrayBufferObject(buffer));

    view1 = nullptr;
||||||| merged common ancestors
    CHECK(JS_DetachArrayBuffer(cx, buffer));
=======
    CHECK(hasDetachedBuffer(view1));
    CHECK(hasDetachedBuffer(view2));
    CHECK(JS::IsDetachedArrayBufferObject(buffer));

    view1 = nullptr;
>>>>>>> upstream-releases
    GC(cx);
    view2 = nullptr;
    GC(cx);
    buffer = nullptr;
    GC(cx);
  }

  return true;
}

static void GC(JSContext* cx) {
  JS_GC(cx);
  JS_GC(cx);  // Trigger another to wait for background finalization to end
}

bool hasDetachedBuffer(JS::HandleObject obj) {
  JS::RootedValue v(cx);
  return JS_GetProperty(cx, obj, "byteLength", &v) && v.toInt32() == 0;
}

<<<<<<< HEAD
END_TEST(testArrayBuffer_bug720949_viewList)
||||||| merged common ancestors
END_TEST(testArrayBuffer_externalize)

BEGIN_TEST(testArrayBuffer_customFreeFunc)
{
    ExternalData data("One two three four");

    // The buffer takes ownership of the data.
    JS::RootedObject buffer(cx, JS_NewExternalArrayBuffer(cx, data.len(), data.contents(),
        &ExternalData::freeCallback, &data));
    CHECK(buffer);
    CHECK(!data.wasFreed());
=======
END_TEST(testArrayBuffer_bug720949_viewList)

BEGIN_TEST(testArrayBuffer_customFreeFunc) {
  ExternalData data("One two three four");

  // The buffer takes ownership of the data.
  JS::RootedObject buffer(
      cx, JS::NewExternalArrayBuffer(cx, data.len(), data.contents(),
                                     &ExternalData::freeCallback, &data));
  CHECK(buffer);
  CHECK(!data.wasFreed());

  uint32_t len;
  bool isShared;
  uint8_t* bufferData;
  JS::GetArrayBufferLengthAndData(buffer, &len, &isShared, &bufferData);
  CHECK_EQUAL(len, data.len());
  CHECK(bufferData == data.contents());
  CHECK(strcmp(reinterpret_cast<char*>(bufferData), data.asString()) == 0);

  buffer = nullptr;
  JS_GC(cx);
  JS_GC(cx);
  CHECK(data.wasFreed());

  return true;
}
END_TEST(testArrayBuffer_customFreeFunc)

BEGIN_TEST(testArrayBuffer_staticContents) {
  ExternalData data("One two three four");

  // When not passing a free function, the buffer doesn't own the data.
  JS::RootedObject buffer(
      cx, JS::NewExternalArrayBuffer(cx, data.len(), data.contents(), nullptr));
  CHECK(buffer);
  CHECK(!data.wasFreed());

  uint32_t len;
  bool isShared;
  uint8_t* bufferData;
  JS::GetArrayBufferLengthAndData(buffer, &len, &isShared, &bufferData);
  CHECK_EQUAL(len, data.len());
  CHECK(bufferData == data.contents());
  CHECK(strcmp(reinterpret_cast<char*>(bufferData), data.asString()) == 0);

  buffer = nullptr;
  JS_GC(cx);
  JS_GC(cx);
  CHECK(!data.wasFreed());

  data.free();
  return true;
}
END_TEST(testArrayBuffer_staticContents)
>>>>>>> upstream-releases

<<<<<<< HEAD
BEGIN_TEST(testArrayBuffer_externalize) {
  if (!testWithSize(cx, 2)) {  // ArrayBuffer data stored inline in the object.
    return false;
  }
  if (!testWithSize(cx, 2000)) {  // ArrayBuffer data stored out-of-line in a
                                  // separate heap allocation.
    return false;
  }
||||||| merged common ancestors
    uint32_t len;
    bool isShared;
    uint8_t* bufferData;
    js::GetArrayBufferLengthAndData(buffer, &len, &isShared, &bufferData);
    CHECK_EQUAL(len, data.len());
    CHECK(bufferData == data.contents());
    CHECK(strcmp(reinterpret_cast<char*>(bufferData), data.asString()) == 0);
=======
BEGIN_TEST(testArrayBuffer_stealDetachExternal) {
  static const char dataBytes[] = "One two three four";
  ExternalData data(dataBytes);
  JS::RootedObject buffer(
      cx, JS::NewExternalArrayBuffer(cx, data.len(), data.contents(),
                                     &ExternalData::freeCallback, &data));
  CHECK(buffer);
  CHECK(!data.wasFreed());

  void* stolenContents = JS::StealArrayBufferContents(cx, buffer);

  // External buffers are stealable: the data is copied into freshly allocated
  // memory, and the buffer's data pointer is cleared (immediately freeing the
  // data) and the buffer is marked as detached.
  CHECK(stolenContents != data.contents());
  CHECK(strcmp(reinterpret_cast<char*>(stolenContents), dataBytes) == 0);
  CHECK(data.wasFreed());
  CHECK(JS::IsDetachedArrayBufferObject(buffer));

  JS_free(cx, stolenContents);
  return true;
}
END_TEST(testArrayBuffer_stealDetachExternal)
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    buffer = nullptr;
    JS_GC(cx);
    JS_GC(cx);
    CHECK(data.wasFreed());
=======
BEGIN_TEST(testArrayBuffer_serializeExternal) {
  JS::RootedValue serializeValue(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool testWithSize(JSContext* cx, size_t n) {
  JS::RootedObject buffer(cx, JS_NewArrayBuffer(cx, n));
  CHECK(buffer != nullptr);

  JS::RootedObject view(cx, JS_NewUint8ArrayWithBuffer(cx, buffer, 0, -1));
  CHECK(view != nullptr);

  void* contents = JS_ExternalizeArrayBufferContents(cx, buffer);
  CHECK(contents != nullptr);
  uint32_t actualLength;
  CHECK(hasExpectedLength(cx, view, &actualLength));
  CHECK(actualLength == n);
  CHECK(!JS_IsDetachedArrayBufferObject(buffer));
  CHECK(JS_GetArrayBufferByteLength(buffer) == uint32_t(n));

  uint8_t* uint8Contents = static_cast<uint8_t*>(contents);
  CHECK(*uint8Contents == 0);
  uint8_t randomByte(rand() % UINT8_MAX);
  *uint8Contents = randomByte;

  JS::RootedValue v(cx);
  CHECK(JS_GetElement(cx, view, 0, &v));
  CHECK(v.toInt32() == randomByte);

  view = nullptr;
  GC(cx);

  CHECK(JS_DetachArrayBuffer(cx, buffer));
  GC(cx);
  CHECK(*uint8Contents == randomByte);
  JS_free(cx, contents);
  GC(cx);
  buffer = nullptr;
  GC(cx);

  return true;
}
||||||| merged common ancestors
    return true;
}
END_TEST(testArrayBuffer_customFreeFunc)
=======
  {
    JS::RootedFunction serialize(cx);
    serialize =
        JS_NewFunction(cx, js::testingFunc_serialize, 1, 0, "serialize");
    CHECK(serialize);
>>>>>>> upstream-releases

<<<<<<< HEAD
static void GC(JSContext* cx) {
  JS_GC(cx);
  JS_GC(cx);  // Trigger another to wait for background finalization to end
}
||||||| merged common ancestors
BEGIN_TEST(testArrayBuffer_staticContents)
{
    ExternalData data("One two three four");
=======
    serializeValue.setObject(*JS_GetFunctionObject(serialize));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool hasExpectedLength(JSContext* cx, JS::HandleObject obj,
                              uint32_t* len) {
  JS::RootedValue v(cx);
  if (!JS_GetProperty(cx, obj, "byteLength", &v)) {
    return false;
  }
  *len = v.toInt32();
  return true;
}
||||||| merged common ancestors
    // When not passing a free function, the buffer doesn't own the data.
    JS::RootedObject buffer(cx, JS_NewExternalArrayBuffer(cx, data.len(), data.contents(),
        nullptr));
    CHECK(buffer);
    CHECK(!data.wasFreed());
=======
  ExternalData data("One two three four");
  JS::RootedObject externalBuffer(
      cx, JS::NewExternalArrayBuffer(cx, data.len(), data.contents(),
                                     &ExternalData::freeCallback, &data));
  CHECK(externalBuffer);
  CHECK(!data.wasFreed());
>>>>>>> upstream-releases

<<<<<<< HEAD
END_TEST(testArrayBuffer_externalize)
||||||| merged common ancestors
    uint32_t len;
    bool isShared;
    uint8_t* bufferData;
    js::GetArrayBufferLengthAndData(buffer, &len, &isShared, &bufferData);
    CHECK_EQUAL(len, data.len());
    CHECK(bufferData == data.contents());
    CHECK(strcmp(reinterpret_cast<char*>(bufferData), data.asString()) == 0);
=======
  JS::RootedValue v(cx, JS::ObjectValue(*externalBuffer));
  JS::RootedObject transferMap(cx,
                               JS_NewArrayObject(cx, JS::HandleValueArray(v)));
  CHECK(transferMap);
>>>>>>> upstream-releases

<<<<<<< HEAD
BEGIN_TEST(testArrayBuffer_customFreeFunc) {
  ExternalData data("One two three four");

  // The buffer takes ownership of the data.
  JS::RootedObject buffer(
      cx, JS_NewExternalArrayBuffer(cx, data.len(), data.contents(),
                                    &ExternalData::freeCallback, &data));
  CHECK(buffer);
  CHECK(!data.wasFreed());

  uint32_t len;
  bool isShared;
  uint8_t* bufferData;
  js::GetArrayBufferLengthAndData(buffer, &len, &isShared, &bufferData);
  CHECK_EQUAL(len, data.len());
  CHECK(bufferData == data.contents());
  CHECK(strcmp(reinterpret_cast<char*>(bufferData), data.asString()) == 0);

  buffer = nullptr;
  JS_GC(cx);
  JS_GC(cx);
  CHECK(data.wasFreed());

  return true;
}
END_TEST(testArrayBuffer_customFreeFunc)
||||||| merged common ancestors
    buffer = nullptr;
    JS_GC(cx);
    JS_GC(cx);
    CHECK(!data.wasFreed());
=======
  JS::AutoValueArray<2> args(cx);
  args[0].setObject(*externalBuffer);
  args[1].setObject(*transferMap);
>>>>>>> upstream-releases

<<<<<<< HEAD
BEGIN_TEST(testArrayBuffer_staticContents) {
  ExternalData data("One two three four");

  // When not passing a free function, the buffer doesn't own the data.
  JS::RootedObject buffer(
      cx, JS_NewExternalArrayBuffer(cx, data.len(), data.contents(), nullptr));
  CHECK(buffer);
  CHECK(!data.wasFreed());

  uint32_t len;
  bool isShared;
  uint8_t* bufferData;
  js::GetArrayBufferLengthAndData(buffer, &len, &isShared, &bufferData);
  CHECK_EQUAL(len, data.len());
  CHECK(bufferData == data.contents());
  CHECK(strcmp(reinterpret_cast<char*>(bufferData), data.asString()) == 0);

  buffer = nullptr;
  JS_GC(cx);
  JS_GC(cx);
  CHECK(!data.wasFreed());

  data.free();
  return true;
}
END_TEST(testArrayBuffer_staticContents)
||||||| merged common ancestors
    data.free();
    return true;
}
END_TEST(testArrayBuffer_staticContents)
=======
  // serialize(externalBuffer, [externalBuffer]) should throw for an unhandled
  // BufferContents kind.
  CHECK(!JS::Call(cx, JS::UndefinedHandleValue, serializeValue,
                  JS::HandleValueArray(args), &v));
>>>>>>> upstream-releases

<<<<<<< HEAD
BEGIN_TEST(testArrayBuffer_stealDetachExternal) {
  ExternalData data("One two three four");
  JS::RootedObject buffer(
      cx, JS_NewExternalArrayBuffer(cx, data.len(), data.contents(),
                                    &ExternalData::freeCallback, &data));
  CHECK(buffer);
  CHECK(!data.wasFreed());

  void* stolenContents = JS_StealArrayBufferContents(cx, buffer);
  // External buffers are currently not stealable, since stealing only
  // gives you a pointer with no indication how to free it. So this should
  // copy the data.
  CHECK(stolenContents != data.contents());
  CHECK(strcmp(reinterpret_cast<char*>(stolenContents), data.asString()) == 0);
  // External buffers are currently not stealable, so this should keep the
  // reference to the data and just mark the buffer as detached.
  CHECK(JS_IsDetachedArrayBufferObject(buffer));
  CHECK(!data.wasFreed());

  buffer = nullptr;
  JS_GC(cx);
  JS_GC(cx);
  CHECK(data.wasFreed());

  return true;
||||||| merged common ancestors
BEGIN_TEST(testArrayBuffer_stealDetachExternal)
{
    ExternalData data("One two three four");
    JS::RootedObject buffer(cx, JS_NewExternalArrayBuffer(cx, data.len(), data.contents(),
        &ExternalData::freeCallback, &data));
    CHECK(buffer);
    CHECK(!data.wasFreed());

    void* stolenContents = JS_StealArrayBufferContents(cx, buffer);
    // External buffers are currently not stealable, since stealing only
    // gives you a pointer with no indication how to free it. So this should
    // copy the data.
    CHECK(stolenContents != data.contents());
    CHECK(strcmp(reinterpret_cast<char*>(stolenContents), data.asString()) == 0);
    // External buffers are currently not stealable, so this should keep the
    // reference to the data and just mark the buffer as detached.
    CHECK(JS_IsDetachedArrayBufferObject(buffer));
    CHECK(!data.wasFreed());

    buffer = nullptr;
    JS_GC(cx);
    JS_GC(cx);
    CHECK(data.wasFreed());

    return true;
=======
  JS::RootedValue exn(cx);
  CHECK(JS_GetPendingException(cx, &exn));
  JS_ClearPendingException(cx);

  js::ErrorReport report(cx);
  CHECK(report.init(cx, exn, js::ErrorReport::NoSideEffects));

  CHECK_EQUAL(report.report()->errorNumber,
              static_cast<unsigned int>(JSMSG_SC_NOT_TRANSFERABLE));

  // Data should have been left alone.
  CHECK(!data.wasFreed());

  v.setNull();
  transferMap = nullptr;
  args[0].setNull();
  args[1].setNull();
  externalBuffer = nullptr;

  JS_GC(cx);
  JS_GC(cx);
  CHECK(data.wasFreed());

  return true;
>>>>>>> upstream-releases
}
<<<<<<< HEAD
END_TEST(testArrayBuffer_stealDetachExternal)
||||||| merged common ancestors
END_TEST(testArrayBuffer_stealDetachExternal)
=======
END_TEST(testArrayBuffer_serializeExternal)
>>>>>>> upstream-releases
