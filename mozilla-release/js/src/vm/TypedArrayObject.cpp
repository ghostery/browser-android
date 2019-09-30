/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "vm/TypedArrayObject-inl.h"
#include "vm/TypedArrayObject.h"

#include "mozilla/Alignment.h"
#include "mozilla/CheckedInt.h"
#include "mozilla/FloatingPoint.h"
#include "mozilla/PodOperations.h"
#include "mozilla/TextUtils.h"

#include <string.h>
#ifndef XP_WIN
<<<<<<< HEAD
#include <sys/mman.h>
||||||| merged common ancestors
# include <sys/mman.h>
=======
#  include <sys/mman.h>
>>>>>>> upstream-releases
#endif

#include "jsapi.h"
#include "jsnum.h"
#include "jstypes.h"
#include "jsutil.h"

#include "builtin/Array.h"
#include "builtin/DataViewObject.h"
#include "builtin/TypedObjectConstants.h"
#include "gc/Barrier.h"
#include "gc/Marking.h"
#include "jit/InlinableNatives.h"
#include "js/Conversions.h"
#include "js/PropertySpec.h"
#include "js/UniquePtr.h"
#include "js/Wrapper.h"
#include "util/Windows.h"
#include "vm/ArrayBufferObject.h"
#include "vm/GlobalObject.h"
#include "vm/Interpreter.h"
#include "vm/JSContext.h"
#include "vm/JSObject.h"
#include "vm/PIC.h"
#include "vm/SelfHosting.h"
#include "vm/SharedMem.h"
#include "vm/WrapperObject.h"

#include "gc/Nursery-inl.h"
#include "gc/StoreBuffer-inl.h"
#include "vm/ArrayBufferObject-inl.h"
#include "vm/JSAtom-inl.h"
#include "vm/NativeObject-inl.h"
#include "vm/Shape-inl.h"

using namespace js;

using JS::CanonicalizeNaN;
using JS::ToInt32;
using JS::ToUint32;
<<<<<<< HEAD
using mozilla::AssertedCast;
using mozilla::IsAsciiDigit;
||||||| merged common ancestors
=======
using mozilla::CheckedUint32;
using mozilla::IsAsciiDigit;
>>>>>>> upstream-releases

/*
 * TypedArrayObject
 *
 * The non-templated base class for the specific typed implementations.
 * This class holds all the member variables that are used by
 * the subclasses.
 */

<<<<<<< HEAD
/* static */ int TypedArrayObject::lengthOffset() {
  return NativeObject::getFixedSlotOffset(LENGTH_SLOT);
}

/* static */ int TypedArrayObject::dataOffset() {
  return NativeObject::getPrivateDataOffset(DATA_SLOT);
||||||| merged common ancestors
/* static */ int
TypedArrayObject::lengthOffset()
{
    return NativeObject::getFixedSlotOffset(LENGTH_SLOT);
}

/* static */ int
TypedArrayObject::dataOffset()
{
    return NativeObject::getPrivateDataOffset(DATA_SLOT);
=======
bool TypedArrayObject::convertForSideEffect(JSContext* cx,
                                            HandleValue v) const {
  switch (type()) {
    case Scalar::BigInt64:
    case Scalar::BigUint64: {
      return ToBigInt(cx, v) != nullptr;
    }
    case Scalar::Int8:
    case Scalar::Uint8:
    case Scalar::Int16:
    case Scalar::Uint16:
    case Scalar::Int32:
    case Scalar::Uint32:
    case Scalar::Float32:
    case Scalar::Float64:
    case Scalar::Uint8Clamped: {
      double ignore;
      return ToNumber(cx, v, &ignore);
    }
    case Scalar::MaxTypedArrayViewType:
    case Scalar::Int64:
      MOZ_CRASH("Unsupported TypedArray type");
  }
  MOZ_ASSERT_UNREACHABLE("Invalid scalar type");
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ bool TypedArrayObject::is(HandleValue v) {
  return v.isObject() && v.toObject().is<TypedArrayObject>();
||||||| merged common ancestors
/* static */ bool
TypedArrayObject::is(HandleValue v)
{
    return v.isObject() && v.toObject().is<TypedArrayObject>();
=======
/* static */
bool TypedArrayObject::is(HandleValue v) {
  return v.isObject() && v.toObject().is<TypedArrayObject>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ bool TypedArrayObject::ensureHasBuffer(
    JSContext* cx, Handle<TypedArrayObject*> tarray) {
  if (tarray->hasBuffer()) {
    return true;
  }

  Rooted<ArrayBufferObject*> buffer(
      cx, ArrayBufferObject::create(cx, tarray->byteLength()));
  if (!buffer) {
    return false;
  }

  if (!buffer->addView(cx, tarray)) {
    return false;
  }
||||||| merged common ancestors
/* static */ bool
TypedArrayObject::ensureHasBuffer(JSContext* cx, Handle<TypedArrayObject*> tarray)
{
    if (tarray->hasBuffer()) {
        return true;
    }

    Rooted<ArrayBufferObject*> buffer(cx, ArrayBufferObject::create(cx, tarray->byteLength()));
    if (!buffer) {
        return false;
    }

    if (!buffer->addView(cx, tarray)) {
        return false;
    }
=======
/* static */
bool TypedArrayObject::ensureHasBuffer(JSContext* cx,
                                       Handle<TypedArrayObject*> tarray) {
  if (tarray->hasBuffer()) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // tarray is not shared, because if it were it would have a buffer.
  memcpy(buffer->dataPointer(), tarray->dataPointerUnshared(),
         tarray->byteLength());
||||||| merged common ancestors
    // tarray is not shared, because if it were it would have a buffer.
    memcpy(buffer->dataPointer(), tarray->dataPointerUnshared(), tarray->byteLength());
=======
  AutoRealm ar(cx, tarray);
  Rooted<ArrayBufferObject*> buffer(
      cx, ArrayBufferObject::createZeroed(cx, tarray->byteLength()));
  if (!buffer) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If the object is in the nursery, the buffer will be freed by the next
  // nursery GC. Free the data slot pointer if the object has no inline data.
  Nursery& nursery = cx->nursery();
  if (tarray->isTenured() && !tarray->hasInlineElements() &&
      !nursery.isInside(tarray->elements())) {
    js_free(tarray->elements());
  }
||||||| merged common ancestors
    // If the object is in the nursery, the buffer will be freed by the next
    // nursery GC. Free the data slot pointer if the object has no inline data.
    Nursery& nursery = cx->nursery();
    if (tarray->isTenured() && !tarray->hasInlineElements() &&
        !nursery.isInside(tarray->elements()))
    {
        js_free(tarray->elements());
    }
=======
  // Attaching the first view to an array buffer is infallible.
  MOZ_ALWAYS_TRUE(buffer->addView(cx, tarray));

  // tarray is not shared, because if it were it would have a buffer.
  memcpy(buffer->dataPointer(), tarray->dataPointerUnshared(),
         tarray->byteLength());

  // If the object is in the nursery, the buffer will be freed by the next
  // nursery GC. Free the data slot pointer if the object has no inline data.
  size_t nbytes = JS_ROUNDUP(tarray->byteLength(), sizeof(Value));
  Nursery& nursery = cx->nursery();
  if (tarray->isTenured() && !tarray->hasInlineElements() &&
      !nursery.isInside(tarray->elements())) {
    js_free(tarray->elements());
    RemoveCellMemory(tarray, nbytes, MemoryUse::TypedArrayElements);
  }
>>>>>>> upstream-releases

  tarray->setPrivate(buffer->dataPointer());

  tarray->setFixedSlot(TypedArrayObject::BUFFER_SLOT, ObjectValue(*buffer));

  // Notify compiled jit code that the base pointer has moved.
  MarkObjectStateChange(cx, tarray);

  return true;
}

#ifdef DEBUG
void TypedArrayObject::assertZeroLengthArrayData() const {
  if (length() == 0 && !hasBuffer()) {
    uint8_t* end = fixedData(TypedArrayObject::FIXED_DATA_START);
    MOZ_ASSERT(end[0] == ZeroLengthArrayData);
  }
}
#endif

void TypedArrayObject::finalize(FreeOp* fop, JSObject* obj) {
  MOZ_ASSERT(!IsInsideNursery(obj));
  TypedArrayObject* curObj = &obj->as<TypedArrayObject>();

<<<<<<< HEAD
  // Template objects or discarded objects (which didn't have enough room
  // for inner elements). Don't have anything to free.
  if (!curObj->elementsRaw()) {
    return;
  }
||||||| merged common ancestors
    // Template objects or discarded objects (which didn't have enough room
    // for inner elements). Don't have anything to free.
    if (!curObj->elementsRaw()) {
        return;
    }
=======
  // Template objects or discarded objects (which didn't have enough room
  // for inner elements) don't have anything to free.
  if (!curObj->elementsRaw()) {
    return;
  }
>>>>>>> upstream-releases

  curObj->assertZeroLengthArrayData();

  // Typed arrays with a buffer object do not need to be free'd
  if (curObj->hasBuffer()) {
    return;
  }

<<<<<<< HEAD
  // Free the data slot pointer if it does not point into the old JSObject.
  if (!curObj->hasInlineElements()) {
    js_free(curObj->elements());
  }
||||||| merged common ancestors
    // Free the data slot pointer if it does not point into the old JSObject.
    if (!curObj->hasInlineElements()) {
        js_free(curObj->elements());
    }
=======
  // Free the data slot pointer if it does not point into the old JSObject.
  if (!curObj->hasInlineElements()) {
    size_t nbytes = JS_ROUNDUP(curObj->byteLength(), sizeof(Value));
    fop->free_(obj, curObj->elements(), nbytes, MemoryUse::TypedArrayElements);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ size_t TypedArrayObject::objectMoved(JSObject* obj,
                                                  JSObject* old) {
  TypedArrayObject* newObj = &obj->as<TypedArrayObject>();
  TypedArrayObject* oldObj = &old->as<TypedArrayObject>();
  MOZ_ASSERT(newObj->elementsRaw() == oldObj->elementsRaw());
  MOZ_ASSERT(obj->isTenured());
||||||| merged common ancestors
/* static */ size_t
TypedArrayObject::objectMoved(JSObject* obj, JSObject* old)
{
    TypedArrayObject* newObj = &obj->as<TypedArrayObject>();
    TypedArrayObject* oldObj = &old->as<TypedArrayObject>();
    MOZ_ASSERT(newObj->elementsRaw() == oldObj->elementsRaw());
    MOZ_ASSERT(obj->isTenured());

    // Typed arrays with a buffer object do not need an update.
    if (oldObj->hasBuffer()) {
        return 0;
    }
=======
/* static */
size_t TypedArrayObject::objectMoved(JSObject* obj, JSObject* old) {
  TypedArrayObject* newObj = &obj->as<TypedArrayObject>();
  const TypedArrayObject* oldObj = &old->as<TypedArrayObject>();
  MOZ_ASSERT(newObj->elementsRaw() == oldObj->elementsRaw());
  MOZ_ASSERT(obj->isTenured());

  // Typed arrays with a buffer object do not need an update.
  if (oldObj->hasBuffer()) {
    return 0;
  }

  if (!IsInsideNursery(old)) {
    // Update the data slot pointer if it points to the old JSObject.
    if (oldObj->hasInlineElements()) {
      newObj->setInlineElements();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Typed arrays with a buffer object do not need an update.
  if (oldObj->hasBuffer()) {
    return 0;
  }
||||||| merged common ancestors
    if (!IsInsideNursery(old)) {
        // Update the data slot pointer if it points to the old JSObject.
        if (oldObj->hasInlineElements()) {
            newObj->setInlineElements();
        }
=======
    return 0;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!IsInsideNursery(old)) {
    // Update the data slot pointer if it points to the old JSObject.
    if (oldObj->hasInlineElements()) {
      newObj->setInlineElements();
    }
||||||| merged common ancestors
        return 0;
    }
=======
  void* buf = oldObj->elements();
>>>>>>> upstream-releases

<<<<<<< HEAD
    return 0;
  }
||||||| merged common ancestors
    Nursery& nursery = obj->runtimeFromMainThread()->gc.nursery();
    void* buf = oldObj->elements();
=======
  // Discarded objects (which didn't have enough room for inner elements) don't
  // have any data to move.
  if (!buf) {
    return 0;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Nursery& nursery = obj->runtimeFromMainThread()->gc.nursery();
  void* buf = oldObj->elements();
||||||| merged common ancestors
    if (!nursery.isInside(buf)) {
        nursery.removeMallocedBuffer(buf);
        return 0;
    }
=======
  Nursery& nursery = obj->runtimeFromMainThread()->gc.nursery();
  if (!nursery.isInside(buf)) {
    nursery.removeMallocedBuffer(buf);
    size_t nbytes = JS_ROUNDUP(newObj->byteLength(), sizeof(Value));
    AddCellMemory(newObj, nbytes, MemoryUse::TypedArrayElements);
    return 0;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!nursery.isInside(buf)) {
    nursery.removeMallocedBuffer(buf);
    return 0;
  }

  // Determine if we can use inline data for the target array. If this is
  // possible, the nursery will have picked an allocation size that is large
  // enough.
  size_t nbytes = 0;
  switch (oldObj->type()) {
#define OBJECT_MOVED_TYPED_ARRAY(T, N)     \
  case Scalar::N:                          \
    nbytes = oldObj->length() * sizeof(T); \
    break;
    JS_FOR_EACH_TYPED_ARRAY(OBJECT_MOVED_TYPED_ARRAY)
#undef OBJECT_MOVED_TYPED_ARRAY
    default:
      MOZ_CRASH("Unsupported TypedArray type");
  }
||||||| merged common ancestors
    // Determine if we can use inline data for the target array. If this is
    // possible, the nursery will have picked an allocation size that is large
    // enough.
    size_t nbytes = 0;
    switch (oldObj->type()) {
#define OBJECT_MOVED_TYPED_ARRAY(T, N) \
      case Scalar::N: \
        nbytes = oldObj->length() * sizeof(T); \
        break;
JS_FOR_EACH_TYPED_ARRAY(OBJECT_MOVED_TYPED_ARRAY)
#undef OBJECT_MOVED_TYPED_ARRAY
      default:
        MOZ_CRASH("Unsupported TypedArray type");
    }
=======
  // Determine if we can use inline data for the target array. If this is
  // possible, the nursery will have picked an allocation size that is large
  // enough.
  size_t nbytes = oldObj->byteLength();
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t headerSize = dataOffset() + sizeof(HeapSlot);
||||||| merged common ancestors
    size_t headerSize = dataOffset() + sizeof(HeapSlot);
=======
  constexpr size_t headerSize = dataOffset() + sizeof(HeapSlot);
>>>>>>> upstream-releases

  // See AllocKindForLazyBuffer.
  gc::AllocKind newAllocKind = obj->asTenured().getAllocKind();
  MOZ_ASSERT_IF(nbytes == 0,
                headerSize + sizeof(uint8_t) <= GetGCKindBytes(newAllocKind));

  if (headerSize + nbytes <= GetGCKindBytes(newAllocKind)) {
    MOZ_ASSERT(oldObj->hasInlineElements());
#ifdef DEBUG
    if (nbytes == 0) {
      uint8_t* output = newObj->fixedData(TypedArrayObject::FIXED_DATA_START);
      output[0] = ZeroLengthArrayData;
    }
<<<<<<< HEAD
#endif
    newObj->setInlineElements();
  } else {
    MOZ_ASSERT(!oldObj->hasInlineElements());
    AutoEnterOOMUnsafeRegion oomUnsafe;
    nbytes = JS_ROUNDUP(nbytes, sizeof(Value));
    void* data = newObj->zone()->pod_malloc<uint8_t>(nbytes);
    if (!data) {
      oomUnsafe.crash(
          "Failed to allocate typed array elements while tenuring.");
    }
    MOZ_ASSERT(!nursery.isInside(data));
    newObj->initPrivate(data);
  }
||||||| merged common ancestors
=======
#endif
    newObj->setInlineElements();
  } else {
    MOZ_ASSERT(!oldObj->hasInlineElements());
    MOZ_ASSERT((CheckedUint32(nbytes) + sizeof(Value)).isValid(),
               "JS_ROUNDUP must not overflow");

    AutoEnterOOMUnsafeRegion oomUnsafe;
    nbytes = JS_ROUNDUP(nbytes, sizeof(Value));
    void* data = newObj->zone()->pod_malloc<uint8_t>(
        nbytes, js::ArrayBufferContentsArena);
    if (!data) {
      oomUnsafe.crash(
          "Failed to allocate typed array elements while tenuring.");
    }
    MOZ_ASSERT(!nursery.isInside(data));
    InitObjectPrivate(newObj, data, nbytes, MemoryUse::TypedArrayElements);
  }
>>>>>>> upstream-releases

  mozilla::PodCopy(newObj->elements(), oldObj->elements(), nbytes);

  // Set a forwarding pointer for the element buffers in case they were
  // preserved on the stack by Ion.
  nursery.setForwardingPointerWhileTenuring(
      oldObj->elements(), newObj->elements(),
      /* direct = */ nbytes >= sizeof(uintptr_t));

  return newObj->hasInlineElements() ? 0 : nbytes;
}

bool TypedArrayObject::hasInlineElements() const {
  return elements() == this->fixedData(TypedArrayObject::FIXED_DATA_START) &&
         byteLength() <= TypedArrayObject::INLINE_BUFFER_LIMIT;
}

void TypedArrayObject::setInlineElements() {
  char* dataSlot = reinterpret_cast<char*>(this) + this->dataOffset();
  *reinterpret_cast<void**>(dataSlot) =
      this->fixedData(TypedArrayObject::FIXED_DATA_START);
}

/* Helper clamped uint8_t type */

uint32_t JS_FASTCALL js::ClampDoubleToUint8(const double x) {
  // Not < so that NaN coerces to 0
  if (!(x >= 0)) {
    return 0;
  }

  if (x > 255) {
    return 255;
  }

  double toTruncate = x + 0.5;
  uint8_t y = uint8_t(toTruncate);

  /*
   * now val is rounded to nearest, ties rounded up.  We want
   * rounded to nearest ties to even, so check whether we had a
   * tie.
   */
  if (y == toTruncate) {
    /*
     * It was a tie (since adding 0.5 gave us the exact integer
     * we want).  Since we rounded up, we either already have an
     * even number or we have an odd number but the number we
     * want is one less.  So just unconditionally masking out the
     * ones bit should do the trick to get us the value we
     * want.
     */
<<<<<<< HEAD
    return y & ~1;
  }

  return y;
||||||| merged common ancestors
    if (y == toTruncate) {
        /*
         * It was a tie (since adding 0.5 gave us the exact integer
         * we want).  Since we rounded up, we either already have an
         * even number or we have an odd number but the number we
         * want is one less.  So just unconditionally masking out the
         * ones bit should do the trick to get us the value we
         * want.
         */
        return y & ~1;
    }

    return y;
=======
    return y & ~1;
  }

  return y;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename ElementType>
static inline JSObject* NewArray(JSContext* cx, uint32_t nelements);

||||||| merged common ancestors
template<typename ElementType>
static inline JSObject*
NewArray(JSContext* cx, uint32_t nelements);

=======
>>>>>>> upstream-releases
namespace {

enum class SpeciesConstructorOverride { None, ArrayBuffer };

<<<<<<< HEAD
enum class CreateSingleton { Yes, No };
||||||| merged common ancestors
enum class CreateSingleton {
    Yes,
    No
};

template<typename NativeType>
class TypedArrayObjectTemplate : public TypedArrayObject
{
    friend class TypedArrayObject;
=======
enum class CreateSingleton { No, Yes };

template <typename NativeType>
class TypedArrayObjectTemplate : public TypedArrayObject {
  friend class TypedArrayObject;
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename NativeType>
class TypedArrayObjectTemplate : public TypedArrayObject {
  friend class TypedArrayObject;
||||||| merged common ancestors
  public:
    static constexpr Scalar::Type ArrayTypeID() { return TypeIDOfType<NativeType>::id; }
    static bool ArrayTypeIsUnsigned() { return TypeIsUnsigned<NativeType>(); }
    static bool ArrayTypeIsFloatingPoint() { return TypeIsFloatingPoint<NativeType>(); }
=======
 public:
  static constexpr Scalar::Type ArrayTypeID() {
    return TypeIDOfType<NativeType>::id;
  }
  static constexpr JSProtoKey protoKey() {
    return TypeIDOfType<NativeType>::protoKey;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  static constexpr Scalar::Type ArrayTypeID() {
    return TypeIDOfType<NativeType>::id;
  }
  static bool ArrayTypeIsUnsigned() { return TypeIsUnsigned<NativeType>(); }
  static bool ArrayTypeIsFloatingPoint() {
    return TypeIsFloatingPoint<NativeType>();
  }
||||||| merged common ancestors
    static const size_t BYTES_PER_ELEMENT = sizeof(NativeType);
=======
  static constexpr bool ArrayTypeIsUnsigned() {
    return TypeIsUnsigned<NativeType>();
  }
  static constexpr bool ArrayTypeIsFloatingPoint() {
    return TypeIsFloatingPoint<NativeType>();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static const size_t BYTES_PER_ELEMENT = sizeof(NativeType);
||||||| merged common ancestors
    static JSObject*
    createPrototype(JSContext* cx, JSProtoKey key)
    {
        Handle<GlobalObject*> global = cx->global();
        RootedObject typedArrayProto(cx, GlobalObject::getOrCreateTypedArrayPrototype(cx, global));
        if (!typedArrayProto) {
            return nullptr;
        }
=======
  static constexpr size_t BYTES_PER_ELEMENT = sizeof(NativeType);
>>>>>>> upstream-releases

  static JSObject* createPrototype(JSContext* cx, JSProtoKey key) {
    Handle<GlobalObject*> global = cx->global();
    RootedObject typedArrayProto(
        cx, GlobalObject::getOrCreateTypedArrayPrototype(cx, global));
    if (!typedArrayProto) {
      return nullptr;
    }

    const Class* clasp = TypedArrayObject::protoClassForType(ArrayTypeID());
    return GlobalObject::createBlankPrototypeInheriting(cx, clasp,
                                                        typedArrayProto);
  }

  static JSObject* createConstructor(JSContext* cx, JSProtoKey key) {
    Handle<GlobalObject*> global = cx->global();
    RootedFunction ctorProto(
        cx, GlobalObject::getOrCreateTypedArrayConstructor(cx, global));
    if (!ctorProto) {
      return nullptr;
    }

    JSFunction* fun =
        NewFunctionWithProto(cx, class_constructor, 3, JSFunction::NATIVE_CTOR,
                             nullptr, ClassName(key, cx), ctorProto,
                             gc::AllocKind::FUNCTION, SingletonObject);

    if (fun) {
      fun->setJitInfo(&jit::JitInfo_TypedArrayConstructor);
    }

<<<<<<< HEAD
    return fun;
  }
||||||| merged common ancestors
    static inline const Class* instanceClass()
    {
        return TypedArrayObject::classForType(ArrayTypeID());
    }
=======
    return fun;
  }

  static inline const Class* instanceClass() {
    return TypedArrayObject::classForType(ArrayTypeID());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static inline const Class* instanceClass() {
    return TypedArrayObject::classForType(ArrayTypeID());
  }
||||||| merged common ancestors
    static bool is(HandleValue v) {
        return v.isObject() && v.toObject().hasClass(instanceClass());
    }
=======
  static bool is(HandleValue v) {
    return v.isObject() && v.toObject().hasClass(instanceClass());
  }

  static bool convertValue(JSContext* cx, HandleValue v, NativeType* result);

  static TypedArrayObject* newBuiltinClassInstance(JSContext* cx,
                                                   gc::AllocKind allocKind,
                                                   NewObjectKind newKind) {
    JSObject* obj =
        NewBuiltinClassInstance(cx, instanceClass(), allocKind, newKind);
    return obj ? &obj->as<TypedArrayObject>() : nullptr;
  }

  static TypedArrayObject* makeProtoInstance(JSContext* cx, HandleObject proto,
                                             gc::AllocKind allocKind) {
    MOZ_ASSERT(proto);

    JSObject* obj =
        NewObjectWithGivenProto(cx, instanceClass(), proto, allocKind);
    return obj ? &obj->as<TypedArrayObject>() : nullptr;
  }

  static TypedArrayObject* makeTypedInstance(JSContext* cx,
                                             CreateSingleton createSingleton,
                                             HandleObjectGroup group,
                                             gc::AllocKind allocKind) {
    if (createSingleton == CreateSingleton::Yes) {
      MOZ_ASSERT(!group);
      return newBuiltinClassInstance(cx, allocKind, SingletonObject);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool is(HandleValue v) {
    return v.isObject() && v.toObject().hasClass(instanceClass());
  }

  static void setIndexValue(TypedArrayObject& tarray, uint32_t index,
                            double d) {
    // If the array is an integer array, we only handle up to
    // 32-bit ints from this point on.  if we want to handle
    // 64-bit ints, we'll need some changes.

    // Assign based on characteristics of the destination type
    if (ArrayTypeIsFloatingPoint()) {
      setIndex(tarray, index, NativeType(d));
    } else if (ArrayTypeIsUnsigned()) {
      MOZ_ASSERT(sizeof(NativeType) <= 4);
      uint32_t n = ToUint32(d);
      setIndex(tarray, index, NativeType(n));
    } else if (ArrayTypeID() == Scalar::Uint8Clamped) {
      // The uint8_clamped type has a special rounding converter
      // for doubles.
      setIndex(tarray, index, NativeType(d));
    } else {
      MOZ_ASSERT(sizeof(NativeType) <= 4);
      int32_t n = ToInt32(d);
      setIndex(tarray, index, NativeType(n));
||||||| merged common ancestors
    static void
    setIndexValue(TypedArrayObject& tarray, uint32_t index, double d)
    {
        // If the array is an integer array, we only handle up to
        // 32-bit ints from this point on.  if we want to handle
        // 64-bit ints, we'll need some changes.

        // Assign based on characteristics of the destination type
        if (ArrayTypeIsFloatingPoint()) {
            setIndex(tarray, index, NativeType(d));
        } else if (ArrayTypeIsUnsigned()) {
            MOZ_ASSERT(sizeof(NativeType) <= 4);
            uint32_t n = ToUint32(d);
            setIndex(tarray, index, NativeType(n));
        } else if (ArrayTypeID() == Scalar::Uint8Clamped) {
            // The uint8_clamped type has a special rounding converter
            // for doubles.
            setIndex(tarray, index, NativeType(d));
        } else {
            MOZ_ASSERT(sizeof(NativeType) <= 4);
            int32_t n = ToInt32(d);
            setIndex(tarray, index, NativeType(n));
        }
=======
    if (group) {
      MOZ_ASSERT(group->clasp() == instanceClass());
      NewObjectKind newKind = GenericObject;
      {
        AutoSweepObjectGroup sweep(group);
        if (group->shouldPreTenure(sweep)) {
          newKind = TenuredObject;
        }
      }
      return NewObjectWithGroup<TypedArrayObject>(cx, group, allocKind,
                                                  newKind);
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  static TypedArrayObject* makeProtoInstance(JSContext* cx, HandleObject proto,
                                             gc::AllocKind allocKind) {
    MOZ_ASSERT(proto);

    JSObject* obj =
        NewObjectWithGivenProto(cx, instanceClass(), proto, allocKind);
    return obj ? &obj->as<TypedArrayObject>() : nullptr;
  }
||||||| merged common ancestors
    static TypedArrayObject*
    makeProtoInstance(JSContext* cx, HandleObject proto, gc::AllocKind allocKind)
    {
        MOZ_ASSERT(proto);
=======
    jsbytecode* pc;
    RootedScript script(cx, cx->currentScript(&pc));
    Rooted<TypedArrayObject*> obj(
        cx, newBuiltinClassInstance(cx, allocKind, GenericObject));
    if (!obj) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static TypedArrayObject* makeTypedInstance(JSContext* cx,
                                             CreateSingleton createSingleton,
                                             gc::AllocKind allocKind) {
    const Class* clasp = instanceClass();
    if (createSingleton == CreateSingleton::Yes) {
      JSObject* obj =
          NewBuiltinClassInstance(cx, clasp, allocKind, SingletonObject);
      if (!obj) {
        return nullptr;
      }
      return &obj->as<TypedArrayObject>();
||||||| merged common ancestors
        JSObject* obj = NewObjectWithGivenProto(cx, instanceClass(), proto, allocKind);
        return obj ? &obj->as<TypedArrayObject>() : nullptr;
=======
    if (script && !ObjectGroup::setAllocationSiteObjectGroup(
                      cx, script, pc, obj, /* singleton = */ false)) {
      return nullptr;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    jsbytecode* pc;
    RootedScript script(cx, cx->currentScript(&pc));
    NewObjectKind newKind = GenericObject;
    if (script &&
        ObjectGroup::useSingletonForAllocationSite(script, pc, clasp)) {
      newKind = SingletonObject;
    }
    RootedObject obj(cx,
                     NewBuiltinClassInstance(cx, clasp, allocKind, newKind));
    if (!obj) {
      return nullptr;
    }
||||||| merged common ancestors
    static TypedArrayObject*
    makeTypedInstance(JSContext* cx, CreateSingleton createSingleton, gc::AllocKind allocKind)
    {
        const Class* clasp = instanceClass();
        if (createSingleton == CreateSingleton::Yes) {
            JSObject* obj = NewBuiltinClassInstance(cx, clasp, allocKind, SingletonObject);
            if (!obj) {
                return nullptr;
            }
            return &obj->as<TypedArrayObject>();
        }
=======
    return obj;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (script && !ObjectGroup::setAllocationSiteObjectGroup(
                      cx, script, pc, obj, newKind == SingletonObject)) {
      return nullptr;
    }
||||||| merged common ancestors
        jsbytecode* pc;
        RootedScript script(cx, cx->currentScript(&pc));
        NewObjectKind newKind = GenericObject;
        if (script && ObjectGroup::useSingletonForAllocationSite(script, pc, clasp)) {
            newKind = SingletonObject;
        }
        RootedObject obj(cx, NewBuiltinClassInstance(cx, clasp, allocKind, newKind));
        if (!obj) {
            return nullptr;
        }
=======
  static TypedArrayObject* makeInstance(
      JSContext* cx, Handle<ArrayBufferObjectMaybeShared*> buffer,
      CreateSingleton createSingleton, uint32_t byteOffset, uint32_t len,
      HandleObject proto, HandleObjectGroup group = nullptr) {
    MOZ_ASSERT(len < INT32_MAX / BYTES_PER_ELEMENT);

    gc::AllocKind allocKind =
        buffer ? gc::GetGCObjectKind(instanceClass())
               : AllocKindForLazyBuffer(len * BYTES_PER_ELEMENT);

    // Subclassing mandates that we hand in the proto every time. Most of
    // the time, though, that [[Prototype]] will not be interesting. If
    // it isn't, we can do some more TI optimizations.
    RootedObject checkProto(cx);
    if (proto) {
      checkProto = GlobalObject::getOrCreatePrototype(cx, protoKey());
      if (!checkProto) {
        return nullptr;
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    return &obj->as<TypedArrayObject>();
  }
||||||| merged common ancestors
        if (script && !ObjectGroup::setAllocationSiteObjectGroup(cx, script, pc, obj,
                                                                 newKind == SingletonObject))
        {
            return nullptr;
        }
=======
    AutoSetNewObjectMetadata metadata(cx);
    Rooted<TypedArrayObject*> obj(cx);
    if (proto && proto != checkProto) {
      MOZ_ASSERT(!group);
      obj = makeProtoInstance(cx, proto, allocKind);
    } else {
      obj = makeTypedInstance(cx, createSingleton, group, allocKind);
    }
    if (!obj || !obj->init(cx, buffer, byteOffset, len, BYTES_PER_ELEMENT)) {
      return nullptr;
    }

    return obj;
  }

  static TypedArrayObject* makeTemplateObject(JSContext* cx, int32_t len) {
    MOZ_ASSERT(len >= 0);
    size_t nbytes;
    MOZ_ALWAYS_TRUE(CalculateAllocSize<NativeType>(len, &nbytes));
    MOZ_ASSERT(nbytes < TypedArrayObject::SINGLETON_BYTE_LENGTH);
    bool fitsInline = nbytes <= INLINE_BUFFER_LIMIT;
    gc::AllocKind allocKind = !fitsInline ? gc::GetGCObjectKind(instanceClass())
                                          : AllocKindForLazyBuffer(nbytes);
    MOZ_ASSERT(allocKind >= gc::GetGCObjectKind(instanceClass()));
>>>>>>> upstream-releases

<<<<<<< HEAD
  static TypedArrayObject* makeInstance(
      JSContext* cx, Handle<ArrayBufferObjectMaybeShared*> buffer,
      CreateSingleton createSingleton, uint32_t byteOffset, uint32_t len,
      HandleObject proto) {
    MOZ_ASSERT(len < INT32_MAX / sizeof(NativeType));

    gc::AllocKind allocKind =
        buffer ? gc::GetGCObjectKind(instanceClass())
               : AllocKindForLazyBuffer(len * sizeof(NativeType));

    // Subclassing mandates that we hand in the proto every time. Most of
    // the time, though, that [[Prototype]] will not be interesting. If
    // it isn't, we can do some more TI optimizations.
    RootedObject checkProto(cx);
    if (proto) {
      checkProto = GlobalObject::getOrCreatePrototype(
          cx, JSCLASS_CACHED_PROTO_KEY(instanceClass()));
      if (!checkProto) {
        return nullptr;
      }
||||||| merged common ancestors
        return &obj->as<TypedArrayObject>();
=======
    AutoSetNewObjectMetadata metadata(cx);
    jsbytecode* pc;
    RootedScript script(cx, cx->currentScript(&pc));
    Rooted<TypedArrayObject*> tarray(
        cx, newBuiltinClassInstance(cx, allocKind, TenuredObject));
    if (!tarray) {
      return nullptr;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    AutoSetNewObjectMetadata metadata(cx);
    Rooted<TypedArrayObject*> obj(cx);
    if (proto && proto != checkProto) {
      obj = makeProtoInstance(cx, proto, allocKind);
    } else {
      obj = makeTypedInstance(cx, createSingleton, allocKind);
    }
    if (!obj || !obj->init(cx, buffer, byteOffset, len, BYTES_PER_ELEMENT)) {
      return nullptr;
    }
||||||| merged common ancestors
    static TypedArrayObject*
    makeInstance(JSContext* cx, Handle<ArrayBufferObjectMaybeShared*> buffer,
                 CreateSingleton createSingleton, uint32_t byteOffset, uint32_t len,
                 HandleObject proto)
    {
        MOZ_ASSERT_IF(!buffer, byteOffset == 0);
        MOZ_ASSERT_IF(buffer, !buffer->isDetached());
        MOZ_ASSERT(len < INT32_MAX / sizeof(NativeType));

        gc::AllocKind allocKind = buffer
                                  ? gc::GetGCObjectKind(instanceClass())
                                  : AllocKindForLazyBuffer(len * sizeof(NativeType));

        // Subclassing mandates that we hand in the proto every time. Most of
        // the time, though, that [[Prototype]] will not be interesting. If
        // it isn't, we can do some more TI optimizations.
        RootedObject checkProto(cx);
        if (proto) {
            checkProto =
                GlobalObject::getOrCreatePrototype(cx, JSCLASS_CACHED_PROTO_KEY(instanceClass()));
            if (!checkProto) {
                return nullptr;
            }
        }
=======
    initTypedArraySlots(tarray, len);
>>>>>>> upstream-releases

<<<<<<< HEAD
    return obj;
  }
||||||| merged common ancestors
        AutoSetNewObjectMetadata metadata(cx);
        Rooted<TypedArrayObject*> obj(cx);
        if (proto && proto != checkProto) {
            obj = makeProtoInstance(cx, proto, allocKind);
        } else {
            obj = makeTypedInstance(cx, createSingleton, allocKind);
        }
        if (!obj) {
            return nullptr;
        }
=======
    // Template objects do not need memory for its elements, since there
    // won't be any elements to store. Therefore, we set the pointer to
    // nullptr and avoid allocating memory that will never be used.
    tarray->initPrivate(nullptr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static TypedArrayObject* makeTemplateObject(JSContext* cx, int32_t len) {
    MOZ_ASSERT(len >= 0);
    size_t nbytes;
    MOZ_ALWAYS_TRUE(CalculateAllocSize<NativeType>(len, &nbytes));
    MOZ_ASSERT(nbytes < TypedArrayObject::SINGLETON_BYTE_LENGTH);
    NewObjectKind newKind = TenuredObject;
    bool fitsInline = nbytes <= INLINE_BUFFER_LIMIT;
    const Class* clasp = instanceClass();
    gc::AllocKind allocKind = !fitsInline ? gc::GetGCObjectKind(clasp)
                                          : AllocKindForLazyBuffer(nbytes);
    MOZ_ASSERT(CanBeFinalizedInBackground(allocKind, clasp));
    allocKind = GetBackgroundAllocKind(allocKind);

    AutoSetNewObjectMetadata metadata(cx);
    jsbytecode* pc;
    RootedScript script(cx, cx->currentScript(&pc));
    if (script &&
        ObjectGroup::useSingletonForAllocationSite(script, pc, clasp)) {
      newKind = SingletonObject;
    }
    JSObject* tmp = NewBuiltinClassInstance(cx, clasp, allocKind, newKind);
    if (!tmp) {
      return nullptr;
    }

    Rooted<TypedArrayObject*> tarray(cx, &tmp->as<TypedArrayObject>());
    initTypedArraySlots(tarray, len);

    // Template objects do not need memory for its elements, since there
    // won't be any elements to store. Therefore, we set the pointer to
    // nullptr and avoid allocating memory that will never be used.
    tarray->initPrivate(nullptr);

    if (script && !ObjectGroup::setAllocationSiteObjectGroup(
                      cx, script, pc, tarray, newKind == SingletonObject)) {
      return nullptr;
    }

    return tarray;
  }
||||||| merged common ancestors
        bool isSharedMemory = buffer && IsSharedArrayBuffer(buffer.get());
=======
    if (script && !ObjectGroup::setAllocationSiteObjectGroup(
                      cx, script, pc, tarray, /* singleton = */ false)) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void initTypedArraySlots(TypedArrayObject* tarray, int32_t len) {
    MOZ_ASSERT(len >= 0);
    tarray->setFixedSlot(TypedArrayObject::BUFFER_SLOT, NullValue());
    tarray->setFixedSlot(TypedArrayObject::LENGTH_SLOT,
                         Int32Value(AssertedCast<int32_t>(len)));
    tarray->setFixedSlot(TypedArrayObject::BYTEOFFSET_SLOT, Int32Value(0));

    // Verify that the private slot is at the expected place.
    MOZ_ASSERT(tarray->numFixedSlots() == TypedArrayObject::DATA_SLOT);
||||||| merged common ancestors
        obj->setFixedSlot(TypedArrayObject::BUFFER_SLOT, ObjectOrNullValue(buffer));
        // This is invariant.  Self-hosting code that sets BUFFER_SLOT
        // (if it does) must maintain it, should it need to.
        if (isSharedMemory) {
            obj->setIsSharedMemory();
        }
=======
    return tarray;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef DEBUG
    if (len == 0) {
      uint8_t* output = tarray->fixedData(TypedArrayObject::FIXED_DATA_START);
      output[0] = TypedArrayObject::ZeroLengthArrayData;
    }
#endif
  }
||||||| merged common ancestors
        if (buffer) {
            obj->initDataPointer(buffer->dataPointerEither() + byteOffset);

            // If the buffer is for an inline typed object, the data pointer
            // may be in the nursery, so include a barrier to make sure this
            // object is updated if that typed object moves.
            auto ptr = buffer->dataPointerEither();
            if (!IsInsideNursery(obj) && cx->nursery().isInside(ptr)) {
                // Shared buffer data should never be nursery-allocated, so we
                // need to fail here if isSharedMemory.  However, mmap() can
                // place a SharedArrayRawBuffer up against the bottom end of a
                // nursery chunk, and a zero-length buffer will erroneously be
                // perceived as being inside the nursery; sidestep that.
                if (isSharedMemory) {
                    MOZ_ASSERT(buffer->byteLength() == 0 &&
                               (uintptr_t(ptr.unwrapValue()) & gc::ChunkMask) == 0);
                } else {
                    cx->runtime()->gc.storeBuffer().putWholeCell(obj);
                }
            }
        } else {
            void* data = obj->fixedData(FIXED_DATA_START);
            obj->initPrivate(data);
            memset(data, 0, len * sizeof(NativeType));
#ifdef DEBUG
            if (len == 0) {
                uint8_t* elements = static_cast<uint8_t*>(data);
                elements[0] = ZeroLengthArrayData;
            }
#endif
        }

        obj->setFixedSlot(TypedArrayObject::LENGTH_SLOT, Int32Value(len));
        obj->setFixedSlot(TypedArrayObject::BYTEOFFSET_SLOT, Int32Value(byteOffset));
=======
  static void initTypedArraySlots(TypedArrayObject* tarray, int32_t len) {
    MOZ_ASSERT(len >= 0);
    tarray->initFixedSlot(TypedArrayObject::BUFFER_SLOT, NullValue());
    tarray->initFixedSlot(TypedArrayObject::LENGTH_SLOT, Int32Value(len));
    tarray->initFixedSlot(TypedArrayObject::BYTEOFFSET_SLOT, Int32Value(0));

    // Verify that the private slot is at the expected place.
    MOZ_ASSERT(tarray->numFixedSlots() == TypedArrayObject::DATA_SLOT);
>>>>>>> upstream-releases

  static void initTypedArrayData(JSContext* cx, TypedArrayObject* tarray,
                                 int32_t len, void* buf,
                                 gc::AllocKind allocKind) {
    if (buf) {
#ifdef DEBUG
<<<<<<< HEAD
      Nursery& nursery = cx->nursery();
      MOZ_ASSERT_IF(!nursery.isInside(buf) && !tarray->hasInlineElements(),
                    tarray->isTenured());
#endif
      tarray->initPrivate(buf);
    } else {
      size_t nbytes = len * sizeof(NativeType);
#ifdef DEBUG
      size_t dataOffset = TypedArrayObject::dataOffset();
      size_t offset = dataOffset + sizeof(HeapSlot);
      MOZ_ASSERT(offset + nbytes <= GetGCKindBytes(allocKind));
||||||| merged common ancestors
        if (buffer) {
            uint32_t arrayByteLength = obj->byteLength();
            uint32_t arrayByteOffset = obj->byteOffset();
            uint32_t bufferByteLength = buffer->byteLength();
            // Unwraps are safe: both are for the pointer value.
            if (IsArrayBuffer(buffer.get())) {
                MOZ_ASSERT_IF(!AsArrayBuffer(buffer.get()).isDetached(),
                              buffer->dataPointerEither().unwrap(/*safe*/) <= obj->dataPointerEither().unwrap(/*safe*/));
            }
            MOZ_ASSERT(bufferByteLength - arrayByteOffset >= arrayByteLength);
            MOZ_ASSERT(arrayByteOffset <= bufferByteLength);
        }

        // Verify that the private slot is at the expected place
        MOZ_ASSERT(obj->numFixedSlots() == TypedArrayObject::DATA_SLOT);
=======
    if (len == 0) {
      uint8_t* output = tarray->fixedData(TypedArrayObject::FIXED_DATA_START);
      output[0] = TypedArrayObject::ZeroLengthArrayData;
    }
>>>>>>> upstream-releases
#endif
  }

<<<<<<< HEAD
      void* data = tarray->fixedData(FIXED_DATA_START);
      tarray->initPrivate(data);
      memset(data, 0, nbytes);
||||||| merged common ancestors
        // ArrayBufferObjects track their views to support detaching.
        if (buffer && buffer->is<ArrayBufferObject>()) {
            if (!buffer->as<ArrayBufferObject>().addView(cx, obj)) {
                return nullptr;
            }
        }

        return obj;
=======
  static void initTypedArrayData(TypedArrayObject* tarray, void* buf,
                                 size_t nbytes, gc::AllocKind allocKind) {
    if (buf) {
      InitObjectPrivate(tarray, buf, nbytes, MemoryUse::TypedArrayElements);
    } else {
#ifdef DEBUG
      constexpr size_t dataOffset = TypedArrayObject::dataOffset();
      constexpr size_t offset = dataOffset + sizeof(HeapSlot);
      MOZ_ASSERT(offset + nbytes <= GetGCKindBytes(allocKind));
#endif

      void* data = tarray->fixedData(FIXED_DATA_START);
      tarray->initPrivate(data);
      memset(data, 0, nbytes);
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  static TypedArrayObject* makeTypedArrayWithTemplate(
      JSContext* cx, TypedArrayObject* templateObj, int32_t len) {
    if (len < 0 || uint32_t(len) >= INT32_MAX / sizeof(NativeType)) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_BAD_ARRAY_LENGTH);
      return nullptr;
    }
||||||| merged common ancestors
    static TypedArrayObject*
    makeTemplateObject(JSContext* cx, int32_t len)
    {
        MOZ_ASSERT(len >= 0);
        size_t nbytes;
        MOZ_ALWAYS_TRUE(CalculateAllocSize<NativeType>(len, &nbytes));
        MOZ_ASSERT(nbytes < TypedArrayObject::SINGLETON_BYTE_LENGTH);
        NewObjectKind newKind = TenuredObject;
        bool fitsInline = nbytes <= INLINE_BUFFER_LIMIT;
        const Class* clasp = instanceClass();
        gc::AllocKind allocKind = !fitsInline
                                  ? gc::GetGCObjectKind(clasp)
                                  : AllocKindForLazyBuffer(nbytes);
        MOZ_ASSERT(CanBeFinalizedInBackground(allocKind, clasp));
        allocKind = GetBackgroundAllocKind(allocKind);

        AutoSetNewObjectMetadata metadata(cx);
        jsbytecode* pc;
        RootedScript script(cx, cx->currentScript(&pc));
        if (script && ObjectGroup::useSingletonForAllocationSite(script, pc, clasp)) {
            newKind = SingletonObject;
        }
        JSObject* tmp = NewBuiltinClassInstance(cx, clasp, allocKind, newKind);
        if (!tmp) {
            return nullptr;
        }

        Rooted<TypedArrayObject*> tarray(cx, &tmp->as<TypedArrayObject>());
        initTypedArraySlots(tarray, len);
=======
  static TypedArrayObject* makeTypedArrayWithTemplate(
      JSContext* cx, TypedArrayObject* templateObj, int32_t len) {
    if (len < 0 || uint32_t(len) >= INT32_MAX / BYTES_PER_ELEMENT) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_BAD_ARRAY_LENGTH);
      return nullptr;
    }

    size_t nbytes = len * BYTES_PER_ELEMENT;
    bool fitsInline = nbytes <= INLINE_BUFFER_LIMIT;
>>>>>>> upstream-releases

<<<<<<< HEAD
    size_t nbytes;
    MOZ_ALWAYS_TRUE(js::CalculateAllocSize<NativeType>(len, &nbytes));
||||||| merged common ancestors
        // Template objects do not need memory for its elements, since there
        // won't be any elements to store. Therefore, we set the pointer to
        // nullptr and avoid allocating memory that will never be used.
        tarray->initPrivate(nullptr);
=======
    AutoSetNewObjectMetadata metadata(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool fitsInline = nbytes <= INLINE_BUFFER_LIMIT;
||||||| merged common ancestors
        if (script && !ObjectGroup::setAllocationSiteObjectGroup(cx, script, pc, tarray,
                                                                 newKind == SingletonObject))
        {
            return nullptr;
        }
=======
    gc::AllocKind allocKind = !fitsInline ? gc::GetGCObjectKind(instanceClass())
                                          : AllocKindForLazyBuffer(nbytes);
    RootedObjectGroup group(cx, templateObj->group());
    MOZ_ASSERT(group->clasp() == instanceClass());
>>>>>>> upstream-releases

<<<<<<< HEAD
    AutoSetNewObjectMetadata metadata(cx);
||||||| merged common ancestors
        return tarray;
    }
=======
    TypedArrayObject* obj =
        NewObjectWithGroup<TypedArrayObject>(cx, group, allocKind);
    if (!obj) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    const Class* clasp = templateObj->group()->clasp();
    gc::AllocKind allocKind = !fitsInline ? gc::GetGCObjectKind(clasp)
                                          : AllocKindForLazyBuffer(nbytes);
    MOZ_ASSERT(CanBeFinalizedInBackground(allocKind, clasp));
    allocKind = GetBackgroundAllocKind(allocKind);
    RootedObjectGroup group(cx, templateObj->group());
||||||| merged common ancestors
    static void
    initTypedArraySlots(TypedArrayObject* tarray, int32_t len)
    {
        MOZ_ASSERT(len >= 0);
        tarray->setFixedSlot(TypedArrayObject::BUFFER_SLOT, NullValue());
        tarray->setFixedSlot(TypedArrayObject::LENGTH_SLOT, Int32Value(AssertedCast<int32_t>(len)));
        tarray->setFixedSlot(TypedArrayObject::BYTEOFFSET_SLOT, Int32Value(0));
=======
    initTypedArraySlots(obj, len);
>>>>>>> upstream-releases

<<<<<<< HEAD
    NewObjectKind newKind = TenuredObject;
||||||| merged common ancestors
        // Verify that the private slot is at the expected place.
        MOZ_ASSERT(tarray->numFixedSlots() == TypedArrayObject::DATA_SLOT);
=======
    void* buf = nullptr;
    if (!fitsInline) {
      MOZ_ASSERT(len > 0);
      MOZ_ASSERT((CheckedUint32(nbytes) + sizeof(Value)).isValid(),
                 "JS_ROUNDUP must not overflow");
>>>>>>> upstream-releases

<<<<<<< HEAD
    UniquePtr<void, JS::FreePolicy> buf;
    if (!fitsInline && len > 0) {
      buf.reset(cx->pod_calloc<uint8_t>(nbytes));
      if (!buf) {
        return nullptr;
      }
||||||| merged common ancestors
#ifdef DEBUG
        if (len == 0) {
            uint8_t* output = tarray->fixedData(TypedArrayObject::FIXED_DATA_START);
            output[0] = TypedArrayObject::ZeroLengthArrayData;
        }
#endif
=======
      nbytes = JS_ROUNDUP(nbytes, sizeof(Value));
      buf = cx->nursery().allocateZeroedBuffer(obj, nbytes,
                                               js::ArrayBufferContentsArena);
      if (!buf) {
        ReportOutOfMemory(cx);
        return nullptr;
      }
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    TypedArrayObject* obj =
        NewObjectWithGroup<TypedArrayObject>(cx, group, allocKind, newKind);
    if (!obj) {
      return nullptr;
    }
||||||| merged common ancestors
    static void
    initTypedArrayData(JSContext* cx, TypedArrayObject* tarray, int32_t len,
                       void* buf, gc::AllocKind allocKind)
    {
        if (buf) {
#ifdef DEBUG
            Nursery& nursery = cx->nursery();
            MOZ_ASSERT_IF(!nursery.isInside(buf) && !tarray->hasInlineElements(),
                          tarray->isTenured());
#endif
            tarray->initPrivate(buf);
        } else {
            size_t nbytes = len * sizeof(NativeType);
#ifdef DEBUG
            size_t dataOffset = TypedArrayObject::dataOffset();
            size_t offset = dataOffset + sizeof(HeapSlot);
            MOZ_ASSERT(offset + nbytes <= GetGCKindBytes(allocKind));
#endif

            void* data = tarray->fixedData(FIXED_DATA_START);
            tarray->initPrivate(data);
            memset(data, 0, nbytes);
        }
    }
=======
    initTypedArrayData(obj, buf, nbytes, allocKind);
>>>>>>> upstream-releases

<<<<<<< HEAD
    initTypedArraySlots(obj, len);
    initTypedArrayData(cx, obj, len, buf.release(), allocKind);

    return obj;
  }
||||||| merged common ancestors
    static TypedArrayObject*
    makeTypedArrayWithTemplate(JSContext* cx, TypedArrayObject* templateObj, int32_t len)
    {
        if (len < 0 || uint32_t(len) >= INT32_MAX / sizeof(NativeType)) {
            JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_BAD_ARRAY_LENGTH);
            return nullptr;
        }
=======
    return obj;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // ES2018 draft rev 8340bf9a8427ea81bb0d1459471afbcc91d18add
  // 22.2.4.1 TypedArray ( )
  // 22.2.4.2 TypedArray ( length )
  // 22.2.4.3 TypedArray ( typedArray )
  // 22.2.4.4 TypedArray ( object )
  // 22.2.4.5 TypedArray ( buffer [ , byteOffset [ , length ] ] )
  static bool class_constructor(JSContext* cx, unsigned argc, Value* vp) {
    CallArgs args = CallArgsFromVp(argc, vp);
||||||| merged common ancestors
        size_t nbytes;
        MOZ_ALWAYS_TRUE(js::CalculateAllocSize<NativeType>(len, &nbytes));
=======
  static TypedArrayObject* makeTypedArrayWithTemplate(
      JSContext* cx, TypedArrayObject* templateObj, HandleObject array) {
    MOZ_ASSERT(!IsWrapper(array));
    MOZ_ASSERT(!array->is<ArrayBufferObjectMaybeShared>());
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step 1 (22.2.4.1) or 2 (22.2.4.2-5).
    if (!ThrowIfNotConstructing(cx, args, "typed array")) {
      return false;
    }
||||||| merged common ancestors
        bool fitsInline = nbytes <= INLINE_BUFFER_LIMIT;
=======
    RootedObjectGroup group(cx, templateObj->group());
    return fromArray(cx, array, nullptr, group);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    JSObject* obj = create(cx, args);
    if (!obj) {
      return false;
    }
    args.rval().setObject(*obj);
    return true;
  }
||||||| merged common ancestors
        AutoSetNewObjectMetadata metadata(cx);
=======
  static TypedArrayObject* makeTypedArrayWithTemplate(
      JSContext* cx, TypedArrayObject* templateObj, HandleObject arrayBuffer,
      HandleValue byteOffsetValue, HandleValue lengthValue) {
    MOZ_ASSERT(!IsWrapper(arrayBuffer));
    MOZ_ASSERT(arrayBuffer->is<ArrayBufferObjectMaybeShared>());
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  static JSObject* create(JSContext* cx, const CallArgs& args) {
    MOZ_ASSERT(args.isConstructing());
||||||| merged common ancestors
        const Class* clasp = templateObj->group()->clasp();
        gc::AllocKind allocKind = !fitsInline
                                  ? gc::GetGCObjectKind(clasp)
                                  : AllocKindForLazyBuffer(nbytes);
        MOZ_ASSERT(CanBeFinalizedInBackground(allocKind, clasp));
        allocKind = GetBackgroundAllocKind(allocKind);
        RootedObjectGroup group(cx, templateObj->group());
=======
    RootedObjectGroup group(cx, templateObj->group());
>>>>>>> upstream-releases

<<<<<<< HEAD
    // 22.2.4.1 TypedArray ( )
    // 22.2.4.2 TypedArray ( length )
    if (args.length() == 0 || !args[0].isObject()) {
      // 22.2.4.2, step 3.
      uint64_t len;
      if (!ToIndex(cx, args.get(0), JSMSG_BAD_ARRAY_LENGTH, &len)) {
        return nullptr;
      }
||||||| merged common ancestors
        NewObjectKind newKind = TenuredObject;
=======
    uint64_t byteOffset, length;
    if (!byteOffsetAndLength(cx, byteOffsetValue, lengthValue, &byteOffset,
                             &length)) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // 22.2.4.1, step 3 and 22.2.4.2, step 5.
      // 22.2.4.2.1 AllocateTypedArray, step 1.
      RootedObject proto(cx);
      if (!GetPrototypeFromBuiltinConstructor(cx, args, &proto)) {
        return nullptr;
      }
||||||| merged common ancestors
        UniquePtr<void, JS::FreePolicy> buf;
        if (!fitsInline && len > 0) {
            buf.reset(cx->pod_calloc<uint8_t>(nbytes));
            if (!buf) {
                return nullptr;
            }
        }
=======
    return fromBufferSameCompartment(
        cx, arrayBuffer.as<ArrayBufferObjectMaybeShared>(), byteOffset, length,
        nullptr, group);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      return fromLength(cx, len, proto);
    }
||||||| merged common ancestors
        TypedArrayObject* obj = NewObjectWithGroup<TypedArrayObject>(cx, group, allocKind, newKind);
        if (!obj) {
            return nullptr;
        }
=======
  // ES2018 draft rev 8340bf9a8427ea81bb0d1459471afbcc91d18add
  // 22.2.4.1 TypedArray ( )
  // 22.2.4.2 TypedArray ( length )
  // 22.2.4.3 TypedArray ( typedArray )
  // 22.2.4.4 TypedArray ( object )
  // 22.2.4.5 TypedArray ( buffer [ , byteOffset [ , length ] ] )
  static bool class_constructor(JSContext* cx, unsigned argc, Value* vp) {
    CallArgs args = CallArgsFromVp(argc, vp);
>>>>>>> upstream-releases

<<<<<<< HEAD
    RootedObject dataObj(cx, &args[0].toObject());
||||||| merged common ancestors
        initTypedArraySlots(obj, len);
        initTypedArrayData(cx, obj, len, buf.release(), allocKind);
=======
    // Step 1 (22.2.4.1) or 2 (22.2.4.2-5).
    if (!ThrowIfNotConstructing(cx, args, "typed array")) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // 22.2.4.{3,4,5}, step 4.
    // 22.2.4.2.1 AllocateTypedArray, step 1.
    RootedObject proto(cx);
    if (!GetPrototypeFromBuiltinConstructor(cx, args, &proto)) {
      return nullptr;
||||||| merged common ancestors
        return obj;
=======
    JSObject* obj = create(cx, args);
    if (!obj) {
      return false;
>>>>>>> upstream-releases
    }
    args.rval().setObject(*obj);
    return true;
  }

 private:
  static JSObject* create(JSContext* cx, const CallArgs& args) {
    MOZ_ASSERT(args.isConstructing());

<<<<<<< HEAD
||||||| merged common ancestors
    // ES2018 draft rev 8340bf9a8427ea81bb0d1459471afbcc91d18add
    // 22.2.4.1 TypedArray ( )
    // 22.2.4.2 TypedArray ( length )
=======
    // 22.2.4.1 TypedArray ( )
    // 22.2.4.2 TypedArray ( length )
    if (args.length() == 0 || !args[0].isObject()) {
      // 22.2.4.2, step 3.
      uint64_t len;
      if (!ToIndex(cx, args.get(0), JSMSG_BAD_ARRAY_LENGTH, &len)) {
        return nullptr;
      }

      // 22.2.4.1, step 3 and 22.2.4.2, step 5.
      // 22.2.4.2.1 AllocateTypedArray, step 1.
      RootedObject proto(cx);
      if (!GetPrototypeFromBuiltinConstructor(cx, args, protoKey(), &proto)) {
        return nullptr;
      }

      return fromLength(cx, len, proto);
    }

    RootedObject dataObj(cx, &args[0].toObject());

    // 22.2.4.{3,4,5}, step 4.
    // 22.2.4.2.1 AllocateTypedArray, step 1.
    RootedObject proto(cx);
    if (!GetPrototypeFromBuiltinConstructor(cx, args, protoKey(), &proto)) {
      return nullptr;
    }

>>>>>>> upstream-releases
    // 22.2.4.3 TypedArray ( typedArray )
    // 22.2.4.4 TypedArray ( object )
<<<<<<< HEAD
    if (!UncheckedUnwrap(dataObj)->is<ArrayBufferObjectMaybeShared>()) {
      return fromArray(cx, dataObj, proto);
||||||| merged common ancestors
    // 22.2.4.5 TypedArray ( buffer [ , byteOffset [ , length ] ] )
    static bool
    class_constructor(JSContext* cx, unsigned argc, Value* vp)
    {
        CallArgs args = CallArgsFromVp(argc, vp);

        // Step 1 (22.2.4.1) or 2 (22.2.4.2-5).
        if (!ThrowIfNotConstructing(cx, args, "typed array")) {
            return false;
        }

        JSObject* obj = create(cx, args);
        if (!obj) {
            return false;
        }
        args.rval().setObject(*obj);
        return true;
=======
    if (!UncheckedUnwrap(dataObj)->is<ArrayBufferObjectMaybeShared>()) {
      return fromArray(cx, dataObj, proto);
    }

    // 22.2.4.5 TypedArray ( buffer [ , byteOffset [ , length ] ] )

    uint64_t byteOffset, length;
    if (!byteOffsetAndLength(cx, args.get(1), args.get(2), &byteOffset,
                             &length)) {
      return nullptr;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    // 22.2.4.5 TypedArray ( buffer [ , byteOffset [ , length ] ] )
||||||| merged common ancestors
  private:
    static JSObject*
    create(JSContext* cx, const CallArgs& args)
    {
        MOZ_ASSERT(args.isConstructing());

        // 22.2.4.1 TypedArray ( )
        // 22.2.4.2 TypedArray ( length )
        if (args.length() == 0 || !args[0].isObject()) {
            // 22.2.4.2, step 3.
            uint64_t len;
            if (!ToIndex(cx, args.get(0), JSMSG_BAD_ARRAY_LENGTH, &len)) {
                return nullptr;
            }

            // 22.2.4.1, step 3 and 22.2.4.2, step 5.
            // 22.2.4.2.1 AllocateTypedArray, step 1.
            RootedObject proto(cx);
            if (!GetPrototypeFromBuiltinConstructor(cx, args, &proto)) {
                return nullptr;
            }

            return fromLength(cx, len, proto);
        }
=======
    // Steps 9-17.
    if (dataObj->is<ArrayBufferObjectMaybeShared>()) {
      HandleArrayBufferObjectMaybeShared buffer =
          dataObj.as<ArrayBufferObjectMaybeShared>();
      return fromBufferSameCompartment(cx, buffer, byteOffset, length, proto);
    }
    return fromBufferWrapped(cx, dataObj, byteOffset, length, proto);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    uint64_t byteOffset = 0;
    if (args.hasDefined(1)) {
      // Step 6.
      if (!ToIndex(cx, args[1], &byteOffset)) {
        return nullptr;
      }
||||||| merged common ancestors
        RootedObject dataObj(cx, &args[0].toObject());
=======
  // ES2018 draft rev 8340bf9a8427ea81bb0d1459471afbcc91d18add
  // 22.2.4.5 TypedArray ( buffer [ , byteOffset [ , length ] ] )
  // Steps 6-8.
  static bool byteOffsetAndLength(JSContext* cx, HandleValue byteOffsetValue,
                                  HandleValue lengthValue, uint64_t* byteOffset,
                                  uint64_t* length) {
    *byteOffset = 0;
    if (!byteOffsetValue.isUndefined()) {
      // Step 6.
      if (!ToIndex(cx, byteOffsetValue, byteOffset)) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // Step 7.
      if (byteOffset % sizeof(NativeType) != 0) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_TYPED_ARRAY_CONSTRUCT_BOUNDS);
        return nullptr;
      }
    }
||||||| merged common ancestors
        // 22.2.4.{3,4,5}, step 4.
        // 22.2.4.2.1 AllocateTypedArray, step 1.
        RootedObject proto(cx);
        if (!GetPrototypeFromBuiltinConstructor(cx, args, &proto)) {
            return nullptr;
        }
=======
      // Step 7.
      if (*byteOffset % BYTES_PER_ELEMENT != 0) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_TYPED_ARRAY_CONSTRUCT_BOUNDS);
        return false;
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    uint64_t length = UINT64_MAX;
    if (args.hasDefined(2)) {
      // Step 8.a.
      if (!ToIndex(cx, args[2], &length)) {
        return nullptr;
      }
    }
||||||| merged common ancestors
        // 22.2.4.3 TypedArray ( typedArray )
        // 22.2.4.4 TypedArray ( object )
        if (!UncheckedUnwrap(dataObj)->is<ArrayBufferObjectMaybeShared>()) {
            return fromArray(cx, dataObj, proto);
        }
=======
    *length = UINT64_MAX;
    if (!lengthValue.isUndefined()) {
      // Step 8.a.
      if (!ToIndex(cx, lengthValue, length)) {
        return false;
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Steps 9-17.
    if (dataObj->is<ArrayBufferObjectMaybeShared>()) {
      HandleArrayBufferObjectMaybeShared buffer =
          dataObj.as<ArrayBufferObjectMaybeShared>();
      return fromBufferSameCompartment(cx, buffer, byteOffset, length, proto);
    }
    return fromBufferWrapped(cx, dataObj, byteOffset, length, proto);
  }
||||||| merged common ancestors
        // 22.2.4.5 TypedArray ( buffer [ , byteOffset [ , length ] ] )

        uint64_t byteOffset = 0;
        if (args.hasDefined(1)) {
            // Step 6.
            if (!ToIndex(cx, args[1], &byteOffset)) {
                return nullptr;
            }

            // Step 7.
            if (byteOffset % sizeof(NativeType) != 0) {
                JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                          JSMSG_TYPED_ARRAY_CONSTRUCT_BOUNDS);
                return nullptr;
            }
        }
=======
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // ES2018 draft rev 8340bf9a8427ea81bb0d1459471afbcc91d18add
  // 22.2.4.5 TypedArray ( buffer [ , byteOffset [ , length ] ] )
  // Steps 9-12.
  static bool computeAndCheckLength(
      JSContext* cx, HandleArrayBufferObjectMaybeShared bufferMaybeUnwrapped,
      uint64_t byteOffset, uint64_t lengthIndex, uint32_t* length) {
    MOZ_ASSERT(byteOffset % sizeof(NativeType) == 0);
    MOZ_ASSERT(byteOffset < uint64_t(DOUBLE_INTEGRAL_PRECISION_LIMIT));
    MOZ_ASSERT_IF(lengthIndex != UINT64_MAX,
                  lengthIndex < uint64_t(DOUBLE_INTEGRAL_PRECISION_LIMIT));
||||||| merged common ancestors
        uint64_t length = UINT64_MAX;
        if (args.hasDefined(2)) {
            // Step 8.a.
            if (!ToIndex(cx, args[2], &length)) {
                return nullptr;
            }
        }
=======
  // ES2018 draft rev 8340bf9a8427ea81bb0d1459471afbcc91d18add
  // 22.2.4.5 TypedArray ( buffer [ , byteOffset [ , length ] ] )
  // Steps 9-12.
  static bool computeAndCheckLength(
      JSContext* cx, HandleArrayBufferObjectMaybeShared bufferMaybeUnwrapped,
      uint64_t byteOffset, uint64_t lengthIndex, uint32_t* length) {
    MOZ_ASSERT(byteOffset % BYTES_PER_ELEMENT == 0);
    MOZ_ASSERT(byteOffset < uint64_t(DOUBLE_INTEGRAL_PRECISION_LIMIT));
    MOZ_ASSERT_IF(lengthIndex != UINT64_MAX,
                  lengthIndex < uint64_t(DOUBLE_INTEGRAL_PRECISION_LIMIT));
>>>>>>> upstream-releases

    // Step 9.
    if (bufferMaybeUnwrapped->isDetached()) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_TYPED_ARRAY_DETACHED);
      return false;
    }

    // Step 10.
    uint32_t bufferByteLength = bufferMaybeUnwrapped->byteLength();

<<<<<<< HEAD
    uint32_t len;
    if (lengthIndex == UINT64_MAX) {
      // Steps 11.a, 11.c.
      if (bufferByteLength % sizeof(NativeType) != 0 ||
          byteOffset > bufferByteLength) {
        // The given byte array doesn't map exactly to
        // |sizeof(NativeType) * N| or |byteOffset| is invalid.
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_TYPED_ARRAY_CONSTRUCT_BOUNDS);
        return false;
      }
||||||| merged common ancestors
        // Step 10.
        uint32_t bufferByteLength = bufferMaybeUnwrapped->byteLength();

        uint32_t len;
        if (lengthIndex == UINT64_MAX) {
            // Steps 11.a, 11.c.
            if (bufferByteLength % sizeof(NativeType) != 0 || byteOffset > bufferByteLength) {
                // The given byte array doesn't map exactly to
                // |sizeof(NativeType) * N| or |byteOffset| is invalid.
                JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                          JSMSG_TYPED_ARRAY_CONSTRUCT_BOUNDS);
                return false;
            }

            // Step 11.b.
            uint32_t newByteLength = bufferByteLength - uint32_t(byteOffset);
            len = newByteLength / sizeof(NativeType);
        } else {
            // Step 12.a.
            uint64_t newByteLength = lengthIndex * sizeof(NativeType);

            // Step 12.b.
            if (byteOffset + newByteLength > bufferByteLength) {
                // |byteOffset + newByteLength| is too big for the arraybuffer
                JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                          JSMSG_TYPED_ARRAY_CONSTRUCT_BOUNDS);
                return false;
            }

            len = uint32_t(lengthIndex);
        }
=======
    uint32_t len;
    if (lengthIndex == UINT64_MAX) {
      // Steps 11.a, 11.c.
      if (bufferByteLength % BYTES_PER_ELEMENT != 0 ||
          byteOffset > bufferByteLength) {
        // The given byte array doesn't map exactly to
        // |BYTES_PER_ELEMENT * N| or |byteOffset| is invalid.
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_TYPED_ARRAY_CONSTRUCT_BOUNDS);
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // Step 11.b.
      uint32_t newByteLength = bufferByteLength - uint32_t(byteOffset);
      len = newByteLength / sizeof(NativeType);
    } else {
      // Step 12.a.
      uint64_t newByteLength = lengthIndex * sizeof(NativeType);

      // Step 12.b.
      if (byteOffset + newByteLength > bufferByteLength) {
        // |byteOffset + newByteLength| is too big for the arraybuffer
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_TYPED_ARRAY_CONSTRUCT_BOUNDS);
        return false;
      }
||||||| merged common ancestors
        // ArrayBuffer is too large for TypedArrays:
        // Standalone ArrayBuffers can hold up to INT32_MAX bytes, whereas
        // buffers in TypedArrays must have less than or equal to
        // |INT32_MAX - sizeof(NativeType) - INT32_MAX % sizeof(NativeType)|
        // bytes.
        if (len >= INT32_MAX / sizeof(NativeType)) {
            JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                      JSMSG_TYPED_ARRAY_CONSTRUCT_BOUNDS);
            return false;
        }
        MOZ_ASSERT(byteOffset <= UINT32_MAX);
=======
      // Step 11.b.
      uint32_t newByteLength = bufferByteLength - uint32_t(byteOffset);
      len = newByteLength / BYTES_PER_ELEMENT;
    } else {
      // Step 12.a.
      uint64_t newByteLength = lengthIndex * BYTES_PER_ELEMENT;

      // Step 12.b.
      if (byteOffset + newByteLength > bufferByteLength) {
        // |byteOffset + newByteLength| is too big for the arraybuffer
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_TYPED_ARRAY_CONSTRUCT_BOUNDS);
        return false;
      }
>>>>>>> upstream-releases

      len = uint32_t(lengthIndex);
    }

<<<<<<< HEAD
    // ArrayBuffer is too large for TypedArrays:
    // Standalone ArrayBuffers can hold up to INT32_MAX bytes, whereas
    // buffers in TypedArrays must have less than or equal to
    // |INT32_MAX - sizeof(NativeType) - INT32_MAX % sizeof(NativeType)|
    // bytes.
    if (len >= INT32_MAX / sizeof(NativeType)) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_TYPED_ARRAY_CONSTRUCT_BOUNDS);
      return false;
    }
    MOZ_ASSERT(byteOffset <= UINT32_MAX);
||||||| merged common ancestors
    // ES2018 draft rev 8340bf9a8427ea81bb0d1459471afbcc91d18add
    // 22.2.4.5 TypedArray ( buffer [ , byteOffset [ , length ] ] )
    // Steps 9-17.
    static JSObject*
    fromBufferSameCompartment(JSContext* cx, HandleArrayBufferObjectMaybeShared buffer,
                              uint64_t byteOffset, uint64_t lengthIndex, HandleObject proto)
    {
        // Steps 9-12.
        uint32_t length;
        if (!computeAndCheckLength(cx, buffer, byteOffset, lengthIndex, &length)) {
            return nullptr;
        }
=======
    // ArrayBuffer is too large for TypedArrays:
    // Standalone ArrayBuffers can hold up to INT32_MAX bytes, whereas
    // buffers in TypedArrays must have less than or equal to
    // |INT32_MAX - BYTES_PER_ELEMENT - INT32_MAX % BYTES_PER_ELEMENT|
    // bytes.
    if (len >= INT32_MAX / BYTES_PER_ELEMENT) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_TYPED_ARRAY_CONSTRUCT_BOUNDS);
      return false;
    }
    MOZ_ASSERT(byteOffset <= UINT32_MAX);
>>>>>>> upstream-releases

    *length = len;
    return true;
  }

<<<<<<< HEAD
  // ES2018 draft rev 8340bf9a8427ea81bb0d1459471afbcc91d18add
  // 22.2.4.5 TypedArray ( buffer [ , byteOffset [ , length ] ] )
  // Steps 9-17.
  static JSObject* fromBufferSameCompartment(
      JSContext* cx, HandleArrayBufferObjectMaybeShared buffer,
      uint64_t byteOffset, uint64_t lengthIndex, HandleObject proto) {
    // Steps 9-12.
    uint32_t length;
    if (!computeAndCheckLength(cx, buffer, byteOffset, lengthIndex, &length)) {
      return nullptr;
    }
||||||| merged common ancestors
        // Steps 13-17.
        return makeInstance(cx, buffer, createSingleton, uint32_t(byteOffset), length, proto);
    }

    // Create a TypedArray object in another compartment.
    //
    // ES6 supports creating a TypedArray in global A (using global A's
    // TypedArray constructor) backed by an ArrayBuffer created in global B.
    //
    // Our TypedArrayObject implementation doesn't support a TypedArray in
    // compartment A backed by an ArrayBuffer in compartment B. So in this
    // case, we create the TypedArray in B (!) and return a cross-compartment
    // wrapper.
    //
    // Extra twist: the spec says the new TypedArray's [[Prototype]] must be
    // A's TypedArray.prototype. So even though we're creating the TypedArray
    // in B, its [[Prototype]] must be (a cross-compartment wrapper for) the
    // TypedArray.prototype in A.
    static JSObject*
    fromBufferWrapped(JSContext* cx, HandleObject bufobj, uint64_t byteOffset,
                      uint64_t lengthIndex, HandleObject proto)
    {
        JSObject* unwrapped = CheckedUnwrap(bufobj);
        if (!unwrapped) {
            ReportAccessDenied(cx);
            return nullptr;
        }
=======
  // ES2018 draft rev 8340bf9a8427ea81bb0d1459471afbcc91d18add
  // 22.2.4.5 TypedArray ( buffer [ , byteOffset [ , length ] ] )
  // Steps 9-17.
  static TypedArrayObject* fromBufferSameCompartment(
      JSContext* cx, HandleArrayBufferObjectMaybeShared buffer,
      uint64_t byteOffset, uint64_t lengthIndex, HandleObject proto,
      HandleObjectGroup group = nullptr) {
    // Steps 9-12.
    uint32_t length;
    if (!computeAndCheckLength(cx, buffer, byteOffset, lengthIndex, &length)) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    CreateSingleton createSingleton = CreateSingleton::No;
    if (length * sizeof(NativeType) >=
        TypedArrayObject::SINGLETON_BYTE_LENGTH) {
      createSingleton = CreateSingleton::Yes;
    }
||||||| merged common ancestors
        if (!unwrapped->is<ArrayBufferObjectMaybeShared>()) {
            JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_TYPED_ARRAY_BAD_ARGS);
            return nullptr;
        }
=======
    CreateSingleton createSingleton = CreateSingleton::No;
    if (!group &&
        length * BYTES_PER_ELEMENT >= TypedArrayObject::SINGLETON_BYTE_LENGTH) {
      createSingleton = CreateSingleton::Yes;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Steps 13-17.
    return makeInstance(cx, buffer, createSingleton, uint32_t(byteOffset),
                        length, proto);
  }
||||||| merged common ancestors
        RootedArrayBufferObjectMaybeShared unwrappedBuffer(cx);
        unwrappedBuffer = &unwrapped->as<ArrayBufferObjectMaybeShared>();
=======
    // Steps 13-17.
    return makeInstance(cx, buffer, createSingleton, uint32_t(byteOffset),
                        length, proto, group);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Create a TypedArray object in another compartment.
  //
  // ES6 supports creating a TypedArray in global A (using global A's
  // TypedArray constructor) backed by an ArrayBuffer created in global B.
  //
  // Our TypedArrayObject implementation doesn't support a TypedArray in
  // compartment A backed by an ArrayBuffer in compartment B. So in this
  // case, we create the TypedArray in B (!) and return a cross-compartment
  // wrapper.
  //
  // Extra twist: the spec says the new TypedArray's [[Prototype]] must be
  // A's TypedArray.prototype. So even though we're creating the TypedArray
  // in B, its [[Prototype]] must be (a cross-compartment wrapper for) the
  // TypedArray.prototype in A.
  static JSObject* fromBufferWrapped(JSContext* cx, HandleObject bufobj,
                                     uint64_t byteOffset, uint64_t lengthIndex,
                                     HandleObject proto) {
    JSObject* unwrapped = CheckedUnwrap(bufobj);
    if (!unwrapped) {
      ReportAccessDenied(cx);
      return nullptr;
    }

    if (!unwrapped->is<ArrayBufferObjectMaybeShared>()) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_TYPED_ARRAY_BAD_ARGS);
      return nullptr;
    }

    RootedArrayBufferObjectMaybeShared unwrappedBuffer(cx);
    unwrappedBuffer = &unwrapped->as<ArrayBufferObjectMaybeShared>();

    uint32_t length;
    if (!computeAndCheckLength(cx, unwrappedBuffer, byteOffset, lengthIndex,
                               &length)) {
      return nullptr;
    }

    // Make sure to get the [[Prototype]] for the created typed array from
    // this compartment.
    RootedObject protoRoot(cx, proto);
    if (!protoRoot) {
      protoRoot = GlobalObject::getOrCreatePrototype(
          cx, JSCLASS_CACHED_PROTO_KEY(instanceClass()));
      if (!protoRoot) {
        return nullptr;
      }
    }
||||||| merged common ancestors
        uint32_t length;
        if (!computeAndCheckLength(cx, unwrappedBuffer, byteOffset, lengthIndex, &length)) {
            return nullptr;
        }
=======
  // Create a TypedArray object in another compartment.
  //
  // ES6 supports creating a TypedArray in global A (using global A's
  // TypedArray constructor) backed by an ArrayBuffer created in global B.
  //
  // Our TypedArrayObject implementation doesn't support a TypedArray in
  // compartment A backed by an ArrayBuffer in compartment B. So in this
  // case, we create the TypedArray in B (!) and return a cross-compartment
  // wrapper.
  //
  // Extra twist: the spec says the new TypedArray's [[Prototype]] must be
  // A's TypedArray.prototype. So even though we're creating the TypedArray
  // in B, its [[Prototype]] must be (a cross-compartment wrapper for) the
  // TypedArray.prototype in A.
  static JSObject* fromBufferWrapped(JSContext* cx, HandleObject bufobj,
                                     uint64_t byteOffset, uint64_t lengthIndex,
                                     HandleObject proto) {
    JSObject* unwrapped = CheckedUnwrapStatic(bufobj);
    if (!unwrapped) {
      ReportAccessDenied(cx);
      return nullptr;
    }

    if (!unwrapped->is<ArrayBufferObjectMaybeShared>()) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_TYPED_ARRAY_BAD_ARGS);
      return nullptr;
    }

    RootedArrayBufferObjectMaybeShared unwrappedBuffer(cx);
    unwrappedBuffer = &unwrapped->as<ArrayBufferObjectMaybeShared>();

    uint32_t length;
    if (!computeAndCheckLength(cx, unwrappedBuffer, byteOffset, lengthIndex,
                               &length)) {
      return nullptr;
    }

    // Make sure to get the [[Prototype]] for the created typed array from
    // this compartment.
    RootedObject protoRoot(cx, proto);
    if (!protoRoot) {
      protoRoot = GlobalObject::getOrCreatePrototype(cx, protoKey());
      if (!protoRoot) {
        return nullptr;
      }
    }
>>>>>>> upstream-releases

    RootedObject typedArray(cx);
    {
      JSAutoRealm ar(cx, unwrappedBuffer);

      RootedObject wrappedProto(cx, protoRoot);
      if (!cx->compartment()->wrap(cx, &wrappedProto)) {
        return nullptr;
      }

      typedArray = makeInstance(cx, unwrappedBuffer, CreateSingleton::No,
                                uint32_t(byteOffset), length, wrappedProto);
      if (!typedArray) {
        return nullptr;
      }
    }

    if (!cx->compartment()->wrap(cx, &typedArray)) {
      return nullptr;
    }

    return typedArray;
  }

<<<<<<< HEAD
 public:
  static JSObject* fromBuffer(JSContext* cx, HandleObject bufobj,
                              uint32_t byteOffset, int32_t lengthInt) {
    if (byteOffset % sizeof(NativeType) != 0) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_TYPED_ARRAY_CONSTRUCT_BOUNDS);
      return nullptr;  // invalid byteOffset
||||||| merged common ancestors
        uint64_t lengthIndex = lengthInt >= 0 ? uint64_t(lengthInt) : UINT64_MAX;
        if (bufobj->is<ArrayBufferObjectMaybeShared>()) {
            HandleArrayBufferObjectMaybeShared buffer = bufobj.as<ArrayBufferObjectMaybeShared>();
            return fromBufferSameCompartment(cx, buffer, byteOffset, lengthIndex, nullptr);
        }
        return fromBufferWrapped(cx, bufobj, byteOffset, lengthIndex, nullptr);
=======
 public:
  static JSObject* fromBuffer(JSContext* cx, HandleObject bufobj,
                              uint32_t byteOffset, int32_t lengthInt) {
    if (byteOffset % BYTES_PER_ELEMENT != 0) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_TYPED_ARRAY_CONSTRUCT_BOUNDS);
      return nullptr;  // invalid byteOffset
>>>>>>> upstream-releases
    }

    uint64_t lengthIndex = lengthInt >= 0 ? uint64_t(lengthInt) : UINT64_MAX;
    if (bufobj->is<ArrayBufferObjectMaybeShared>()) {
      HandleArrayBufferObjectMaybeShared buffer =
          bufobj.as<ArrayBufferObjectMaybeShared>();
      return fromBufferSameCompartment(cx, buffer, byteOffset, lengthIndex,
                                       nullptr);
    }
    return fromBufferWrapped(cx, bufobj, byteOffset, lengthIndex, nullptr);
  }

<<<<<<< HEAD
  static bool maybeCreateArrayBuffer(JSContext* cx, uint32_t count,
                                     uint32_t unit,
                                     HandleObject nonDefaultProto,
                                     MutableHandle<ArrayBufferObject*> buffer) {
    if (count >= INT32_MAX / unit) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_BAD_ARRAY_LENGTH);
      return false;
    }
    uint32_t byteLength = count * unit;
||||||| merged common ancestors
        MOZ_ASSERT(byteLength < INT32_MAX);
        static_assert(INLINE_BUFFER_LIMIT % sizeof(NativeType) == 0,
                      "ArrayBuffer inline storage shouldn't waste any space");
=======
  static bool maybeCreateArrayBuffer(JSContext* cx, uint32_t count,
                                     HandleObject nonDefaultProto,
                                     MutableHandle<ArrayBufferObject*> buffer) {
    if (count >= INT32_MAX / BYTES_PER_ELEMENT) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_BAD_ARRAY_LENGTH);
      return false;
    }
    uint32_t byteLength = count * BYTES_PER_ELEMENT;
>>>>>>> upstream-releases

<<<<<<< HEAD
    MOZ_ASSERT(byteLength < INT32_MAX);
    static_assert(INLINE_BUFFER_LIMIT % sizeof(NativeType) == 0,
                  "ArrayBuffer inline storage shouldn't waste any space");
||||||| merged common ancestors
        if (!nonDefaultProto && byteLength <= INLINE_BUFFER_LIMIT) {
            // The array's data can be inline, and the buffer created lazily.
            return true;
        }
=======
    MOZ_ASSERT(byteLength < INT32_MAX);
    static_assert(INLINE_BUFFER_LIMIT % BYTES_PER_ELEMENT == 0,
                  "ArrayBuffer inline storage shouldn't waste any space");
>>>>>>> upstream-releases

    if (!nonDefaultProto && byteLength <= INLINE_BUFFER_LIMIT) {
      // The array's data can be inline, and the buffer created lazily.
      return true;
    }

<<<<<<< HEAD
    ArrayBufferObject* buf =
        ArrayBufferObject::create(cx, byteLength, nonDefaultProto);
    if (!buf) {
      return false;
||||||| merged common ancestors
        buffer.set(buf);
        return true;
=======
    ArrayBufferObject* buf =
        ArrayBufferObject::createZeroed(cx, byteLength, nonDefaultProto);
    if (!buf) {
      return false;
>>>>>>> upstream-releases
    }

    buffer.set(buf);
    return true;
  }

  // 22.2.4.1 TypedArray ( )
  // 22.2.4.2 TypedArray ( length )
  static JSObject* fromLength(JSContext* cx, uint64_t nelements,
                              HandleObject proto = nullptr) {
    // 22.2.4.1, step 1 and 22.2.4.2, steps 1-3 (performed in caller).
    // 22.2.4.1, step 2 and 22.2.4.2, step 4 (implicit).
    // 22.2.4.1, step 3 and 22.2.4.2, step 5 (call AllocateTypedArray).

    if (nelements > UINT32_MAX) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_BAD_ARRAY_LENGTH);
      return nullptr;
    }

<<<<<<< HEAD
    Rooted<ArrayBufferObject*> buffer(cx);
    if (!maybeCreateArrayBuffer(cx, uint32_t(nelements), BYTES_PER_ELEMENT,
                                nullptr, &buffer)) {
      return nullptr;
||||||| merged common ancestors
        return makeInstance(cx, buffer, CreateSingleton::No, 0, uint32_t(nelements), proto);
=======
    Rooted<ArrayBufferObject*> buffer(cx);
    if (!maybeCreateArrayBuffer(cx, uint32_t(nelements), nullptr, &buffer)) {
      return nullptr;
>>>>>>> upstream-releases
    }

    return makeInstance(cx, buffer, CreateSingleton::No, 0, uint32_t(nelements),
                        proto);
  }

<<<<<<< HEAD
  static bool AllocateArrayBuffer(JSContext* cx, HandleObject ctor,
                                  uint32_t count, uint32_t unit,
                                  MutableHandle<ArrayBufferObject*> buffer);
||||||| merged common ancestors
    static JSObject*
    fromArray(JSContext* cx, HandleObject other, HandleObject proto = nullptr);
=======
  static bool AllocateArrayBuffer(JSContext* cx, HandleObject ctor,
                                  uint32_t count,
                                  MutableHandle<ArrayBufferObject*> buffer);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static JSObject* fromArray(JSContext* cx, HandleObject other,
                             HandleObject proto = nullptr);
||||||| merged common ancestors
    static JSObject*
    fromTypedArray(JSContext* cx, HandleObject other, bool isWrapped, HandleObject proto);
=======
  static TypedArrayObject* fromArray(JSContext* cx, HandleObject other,
                                     HandleObject proto = nullptr,
                                     HandleObjectGroup group = nullptr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static JSObject* fromTypedArray(JSContext* cx, HandleObject other,
                                  bool isWrapped, HandleObject proto);
||||||| merged common ancestors
    static JSObject*
    fromObject(JSContext* cx, HandleObject other, HandleObject proto);
=======
  static TypedArrayObject* fromTypedArray(JSContext* cx, HandleObject other,
                                          bool isWrapped, HandleObject proto,
                                          HandleObjectGroup group);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static JSObject* fromObject(JSContext* cx, HandleObject other,
                              HandleObject proto);
||||||| merged common ancestors
    static const NativeType
    getIndex(TypedArrayObject* tarray, uint32_t index)
    {
        MOZ_ASSERT(index < tarray->length());
        return jit::AtomicOperations::loadSafeWhenRacy(tarray->dataPointerEither().cast<NativeType*>() + index);
    }
=======
  static TypedArrayObject* fromObject(JSContext* cx, HandleObject other,
                                      HandleObject proto,
                                      HandleObjectGroup group);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static const NativeType getIndex(TypedArrayObject* tarray, uint32_t index) {
    MOZ_ASSERT(index < tarray->length());
    return jit::AtomicOperations::loadSafeWhenRacy(
        tarray->dataPointerEither().cast<NativeType*>() + index);
  }

  static void setIndex(TypedArrayObject& tarray, uint32_t index,
                       NativeType val) {
    MOZ_ASSERT(index < tarray.length());
    jit::AtomicOperations::storeSafeWhenRacy(
        tarray.dataPointerEither().cast<NativeType*>() + index, val);
  }
||||||| merged common ancestors
    static void
    setIndex(TypedArrayObject& tarray, uint32_t index, NativeType val)
    {
        MOZ_ASSERT(index < tarray.length());
        jit::AtomicOperations::storeSafeWhenRacy(tarray.dataPointerEither().cast<NativeType*>() + index, val);
    }
=======
  static const NativeType getIndex(TypedArrayObject* tarray, uint32_t index) {
    MOZ_ASSERT(index < tarray->length());
    return jit::AtomicOperations::loadSafeWhenRacy(
        tarray->dataPointerEither().cast<NativeType*>() + index);
  }

  static void setIndex(TypedArrayObject& tarray, uint32_t index,
                       NativeType val) {
    MOZ_ASSERT(index < tarray.length());
    jit::AtomicOperations::storeSafeWhenRacy(
        tarray.dataPointerEither().cast<NativeType*>() + index, val);
  }

  static bool getElement(JSContext* cx, TypedArrayObject* tarray,
                         uint32_t index, MutableHandleValue val);
  static bool getElementPure(TypedArrayObject* tarray, uint32_t index,
                             Value* vp);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static Value getIndexValue(TypedArrayObject* tarray, uint32_t index);
||||||| merged common ancestors
    static Value getIndexValue(TypedArrayObject* tarray, uint32_t index);
=======
  static bool setElement(JSContext* cx, Handle<TypedArrayObject*> obj,
                         uint64_t index, HandleValue v, ObjectOpResult& result);
  static bool defineElement(JSContext* cx, HandleObject obj, uint64_t index,
                            HandleValue v, ObjectOpResult& result);
>>>>>>> upstream-releases
};

template <typename NativeType>
bool TypedArrayObjectTemplate<NativeType>::convertValue(JSContext* cx,
                                                        HandleValue v,
                                                        NativeType* result) {
  double d;
  if (!ToNumber(cx, v, &d)) {
    return false;
  }

#ifdef JS_MORE_DETERMINISTIC
  // See the comment in ElementSpecific::doubleToNative.
  d = JS::CanonicalizeNaN(d);
#endif

  // Assign based on characteristics of the destination type
  if (ArrayTypeIsFloatingPoint()) {
    *result = NativeType(d);
  } else if (ArrayTypeIsUnsigned()) {
    MOZ_ASSERT(sizeof(NativeType) <= 4);
    uint32_t n = ToUint32(d);
    *result = NativeType(n);
  } else if (ArrayTypeID() == Scalar::Uint8Clamped) {
    // The uint8_clamped type has a special rounding converter
    // for doubles.
    *result = NativeType(d);
  } else {
    MOZ_ASSERT(sizeof(NativeType) <= 4);
    int32_t n = ToInt32(d);
    *result = NativeType(n);
  }
  return true;
}

template <>
bool TypedArrayObjectTemplate<int64_t>::convertValue(JSContext* cx,
                                                     HandleValue v,
                                                     int64_t* result) {
  JS_TRY_VAR_OR_RETURN_FALSE(cx, *result, ToBigInt64(cx, v));
  return true;
}

template <>
bool TypedArrayObjectTemplate<uint64_t>::convertValue(JSContext* cx,
                                                      HandleValue v,
                                                      uint64_t* result) {
  JS_TRY_VAR_OR_RETURN_FALSE(cx, *result, ToBigUint64(cx, v));
  return true;
}

// https://tc39.github.io/proposal-bigint/#sec-integerindexedelementset
// 7.8 IntegerIndexedElementSet ( O, index, value )
template <typename NativeType>
/* static */ bool TypedArrayObjectTemplate<NativeType>::setElement(
    JSContext* cx, Handle<TypedArrayObject*> obj, uint64_t index, HandleValue v,
    ObjectOpResult& result) {
  // Steps 1-2 are enforced by the caller.

  // Steps 3-6.
  NativeType nativeValue;
  if (!convertValue(cx, v, &nativeValue)) {
    return false;
  }

  // Step 8.
  if (obj->hasDetachedBuffer()) {
    return result.failSoft(JSMSG_TYPED_ARRAY_DETACHED);
  }

  // Steps 9-10 are enforced by the caller.

  // Step 11.
  uint32_t length = obj->length();

  // Step 12.
  if (index >= length) {
    return result.failSoft(JSMSG_BAD_INDEX);
  }

  // Steps 7, 13-16.
  TypedArrayObjectTemplate<NativeType>::setIndex(*obj, index, nativeValue);

  // Step 17.
  return result.succeed();
}

// Version of IntegerIndexedElementSet with no length check, used in
// [[DefineOwnProperty]]
template <typename NativeType>
/* static */ bool TypedArrayObjectTemplate<NativeType>::defineElement(
    JSContext* cx, HandleObject obj, uint64_t index, HandleValue v,
    ObjectOpResult& result) {
  // Steps 1-2 are enforced by the caller.

  // Steps 3-6.
  NativeType nativeValue;
  if (!convertValue(cx, v, &nativeValue)) {
    return false;
  }

  // Step 8.
  if (obj->as<TypedArrayObject>().hasDetachedBuffer()) {
    return result.fail(JSMSG_TYPED_ARRAY_DETACHED);
  }

  // Steps 9-12 are enforced by the caller.

  // Steps 7, 13-16.
  TypedArrayObjectTemplate<NativeType>::setIndex(obj->as<TypedArrayObject>(),
                                                 index, nativeValue);

  // Step 17.
  return result.succeed();
}

#define CREATE_TYPE_FOR_TYPED_ARRAY(T, N) \
  typedef TypedArrayObjectTemplate<T> N##Array;
JS_FOR_EACH_TYPED_ARRAY(CREATE_TYPE_FOR_TYPED_ARRAY)
#undef CREATE_TYPE_FOR_TYPED_ARRAY

} /* anonymous namespace */

<<<<<<< HEAD
TypedArrayObject* js::TypedArrayCreateWithTemplate(JSContext* cx,
                                                   HandleObject templateObj,
                                                   int32_t len) {
  MOZ_ASSERT(templateObj->is<TypedArrayObject>());
  TypedArrayObject* tobj = &templateObj->as<TypedArrayObject>();

  switch (tobj->type()) {
#define CREATE_TYPED_ARRAY(T, N)                                             \
  case Scalar::N:                                                            \
    return TypedArrayObjectTemplate<T>::makeTypedArrayWithTemplate(cx, tobj, \
                                                                   len);
    JS_FOR_EACH_TYPED_ARRAY(CREATE_TYPED_ARRAY)
||||||| merged common ancestors
TypedArrayObject*
js::TypedArrayCreateWithTemplate(JSContext* cx, HandleObject templateObj, int32_t len)
{
    MOZ_ASSERT(templateObj->is<TypedArrayObject>());
    TypedArrayObject* tobj = &templateObj->as<TypedArrayObject>();

    switch (tobj->type()) {
#define CREATE_TYPED_ARRAY(T, N) \
      case Scalar::N: \
        return TypedArrayObjectTemplate<T>::makeTypedArrayWithTemplate(cx, tobj, len);
JS_FOR_EACH_TYPED_ARRAY(CREATE_TYPED_ARRAY)
=======
TypedArrayObject* js::NewTypedArrayWithTemplateAndLength(
    JSContext* cx, HandleObject templateObj, int32_t len) {
  MOZ_ASSERT(templateObj->is<TypedArrayObject>());
  TypedArrayObject* tobj = &templateObj->as<TypedArrayObject>();

  switch (tobj->type()) {
#define CREATE_TYPED_ARRAY(T, N)                                             \
  case Scalar::N:                                                            \
    return TypedArrayObjectTemplate<T>::makeTypedArrayWithTemplate(cx, tobj, \
                                                                   len);
    JS_FOR_EACH_TYPED_ARRAY(CREATE_TYPED_ARRAY)
#undef CREATE_TYPED_ARRAY
    default:
      MOZ_CRASH("Unsupported TypedArray type");
  }
}

TypedArrayObject* js::NewTypedArrayWithTemplateAndArray(
    JSContext* cx, HandleObject templateObj, HandleObject array) {
  MOZ_ASSERT(templateObj->is<TypedArrayObject>());
  TypedArrayObject* tobj = &templateObj->as<TypedArrayObject>();

  switch (tobj->type()) {
#define CREATE_TYPED_ARRAY(T, N)                                             \
  case Scalar::N:                                                            \
    return TypedArrayObjectTemplate<T>::makeTypedArrayWithTemplate(cx, tobj, \
                                                                   array);
    JS_FOR_EACH_TYPED_ARRAY(CREATE_TYPED_ARRAY)
>>>>>>> upstream-releases
#undef CREATE_TYPED_ARRAY
<<<<<<< HEAD
    default:
      MOZ_CRASH("Unsupported TypedArray type");
  }
||||||| merged common ancestors
      default:
        MOZ_CRASH("Unsupported TypedArray type");
    }
=======
    default:
      MOZ_CRASH("Unsupported TypedArray type");
  }
}

TypedArrayObject* js::NewTypedArrayWithTemplateAndBuffer(
    JSContext* cx, HandleObject templateObj, HandleObject arrayBuffer,
    HandleValue byteOffset, HandleValue length) {
  MOZ_ASSERT(templateObj->is<TypedArrayObject>());
  TypedArrayObject* tobj = &templateObj->as<TypedArrayObject>();

  switch (tobj->type()) {
#define CREATE_TYPED_ARRAY(T, N)                                    \
  case Scalar::N:                                                   \
    return TypedArrayObjectTemplate<T>::makeTypedArrayWithTemplate( \
        cx, tobj, arrayBuffer, byteOffset, length);
    JS_FOR_EACH_TYPED_ARRAY(CREATE_TYPED_ARRAY)
#undef CREATE_TYPED_ARRAY
    default:
      MOZ_CRASH("Unsupported TypedArray type");
  }
>>>>>>> upstream-releases
}

// ES2018 draft rev 2aea8f3e617b49df06414eb062ab44fad87661d3
// 24.1.1.1 AllocateArrayBuffer ( constructor, byteLength )
<<<<<<< HEAD
// byteLength = count * unit
template <typename T>
/* static */ bool TypedArrayObjectTemplate<T>::AllocateArrayBuffer(
    JSContext* cx, HandleObject ctor, uint32_t count, uint32_t unit,
    MutableHandle<ArrayBufferObject*> buffer) {
  // 24.1.1.1 step 1 (partially).
  RootedObject proto(cx);

  JSFunction* arrayBufferCtor =
      GlobalObject::getOrCreateArrayBufferConstructor(cx, cx->global());
  if (!arrayBufferCtor) {
    return false;
  }

  // As an optimization, skip the "prototype" lookup for %ArrayBuffer%.
  if (ctor != arrayBufferCtor) {
    // 9.1.13 OrdinaryCreateFromConstructor, steps 1-2.
    if (!GetPrototypeFromConstructor(cx, ctor, &proto)) {
      return false;
    }

    JSObject* arrayBufferProto =
        GlobalObject::getOrCreateArrayBufferPrototype(cx, cx->global());
    if (!arrayBufferProto) {
      return false;
||||||| merged common ancestors
// byteLength = count * unit
template<typename T>
/* static */ bool
TypedArrayObjectTemplate<T>::AllocateArrayBuffer(JSContext* cx, HandleObject ctor,
                                                 uint32_t count, uint32_t unit,
                                                 MutableHandle<ArrayBufferObject*> buffer)
{
    // 24.1.1.1 step 1 (partially).
    RootedObject proto(cx);

    JSFunction* arrayBufferCtor = GlobalObject::getOrCreateArrayBufferConstructor(cx, cx->global());
    if (!arrayBufferCtor) {
        return false;
    }

    // As an optimization, skip the "prototype" lookup for %ArrayBuffer%.
    if (ctor != arrayBufferCtor) {
        // 9.1.13 OrdinaryCreateFromConstructor, steps 1-2.
        if (!GetPrototypeFromConstructor(cx, ctor, &proto)) {
            return false;
        }

        JSObject* arrayBufferProto = GlobalObject::getOrCreateArrayBufferPrototype(cx, cx->global());
        if (!arrayBufferProto) {
            return false;
        }

        // Reset |proto| if it's the default %ArrayBufferPrototype%.
        if (proto == arrayBufferProto) {
            proto = nullptr;
        }
=======
// byteLength = count * BYTES_PER_ELEMENT
template <typename T>
/* static */ bool TypedArrayObjectTemplate<T>::AllocateArrayBuffer(
    JSContext* cx, HandleObject ctor, uint32_t count,
    MutableHandle<ArrayBufferObject*> buffer) {
  // 24.1.1.1 step 1 (partially).
  RootedObject proto(cx);

  JSFunction* arrayBufferCtor =
      GlobalObject::getOrCreateArrayBufferConstructor(cx, cx->global());
  if (!arrayBufferCtor) {
    return false;
  }

  // As an optimization, skip the "prototype" lookup for %ArrayBuffer%.
  if (ctor != arrayBufferCtor) {
    // 9.1.13 OrdinaryCreateFromConstructor, steps 1-2.
    if (!GetPrototypeFromConstructor(cx, ctor, JSProto_ArrayBuffer, &proto)) {
      return false;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
    // Reset |proto| if it's the default %ArrayBufferPrototype%.
    if (proto == arrayBufferProto) {
      proto = nullptr;
    }
  }
||||||| merged common ancestors
    // 24.1.1.1 steps 1 (remaining part), 2-6.
    if (!maybeCreateArrayBuffer(cx, count, unit, proto, buffer)) {
        return false;
    }
=======
  // 24.1.1.1 steps 1 (remaining part), 2-6.
  if (!maybeCreateArrayBuffer(cx, count, proto, buffer)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // 24.1.1.1 steps 1 (remaining part), 2-6.
  if (!maybeCreateArrayBuffer(cx, count, unit, proto, buffer)) {
    return false;
  }

  return true;
||||||| merged common ancestors
    return true;
=======
  return true;
>>>>>>> upstream-releases
}

static bool IsArrayBufferSpecies(JSContext* cx, JSFunction* species) {
  return IsSelfHostedFunctionWithName(species, cx->names().ArrayBufferSpecies);
}

static JSObject* GetBufferSpeciesConstructor(
    JSContext* cx, Handle<TypedArrayObject*> typedArray, bool isWrapped,
    SpeciesConstructorOverride override) {
  RootedObject defaultCtor(
      cx, GlobalObject::getOrCreateArrayBufferConstructor(cx, cx->global()));
  if (!defaultCtor) {
    return nullptr;
  }

  // Use the current global's ArrayBuffer if the override is set.
  if (override == SpeciesConstructorOverride::ArrayBuffer) {
    return defaultCtor;
  }

  RootedObject obj(cx, typedArray->bufferEither());
  if (!obj) {
    MOZ_ASSERT(!isWrapped);

    // The buffer was never exposed to content code, so if
    // 1. %ArrayBufferPrototype%.constructor == %ArrayBuffer%, and
    // 2. %ArrayBuffer%[@@species] == ArrayBufferSpecies
    // we don't have to reify the buffer object and can simply return the
    // default arrray buffer constructor.

    JSObject* proto =
        GlobalObject::getOrCreateArrayBufferPrototype(cx, cx->global());
    if (!proto) {
      return nullptr;
    }

    Value ctor;
    bool found;
    if (GetOwnPropertyPure(cx, proto, NameToId(cx->names().constructor), &ctor,
                           &found) &&
        ctor.isObject() && &ctor.toObject() == defaultCtor) {
      jsid speciesId = SYMBOL_TO_JSID(cx->wellKnownSymbols().species);
      JSFunction* getter;
      if (GetOwnGetterPure(cx, defaultCtor, speciesId, &getter) && getter &&
          IsArrayBufferSpecies(cx, getter)) {
        return defaultCtor;
      }
    }

    if (!TypedArrayObject::ensureHasBuffer(cx, typedArray)) {
      return nullptr;
    }

    obj.set(typedArray->bufferEither());
  } else {
    if (isWrapped && !cx->compartment()->wrap(cx, &obj)) {
      return nullptr;
    }
  }

  return SpeciesConstructor(cx, obj, defaultCtor, IsArrayBufferSpecies);
}

<<<<<<< HEAD
template <typename T>
/* static */ JSObject* TypedArrayObjectTemplate<T>::fromArray(
    JSContext* cx, HandleObject other, HandleObject proto /* = nullptr */) {
  // Allow nullptr proto for FriendAPI methods, which don't care about
  // subclassing.
  if (other->is<TypedArrayObject>()) {
    return fromTypedArray(cx, other, /* wrapped= */ false, proto);
  }
||||||| merged common ancestors
template<typename T>
/* static */ JSObject*
TypedArrayObjectTemplate<T>::fromArray(JSContext* cx, HandleObject other,
                                       HandleObject proto /* = nullptr */)
{
    // Allow nullptr proto for FriendAPI methods, which don't care about
    // subclassing.
    if (other->is<TypedArrayObject>()) {
        return fromTypedArray(cx, other, /* wrapped= */ false, proto);
    }
=======
template <typename T>
/* static */ TypedArrayObject* TypedArrayObjectTemplate<T>::fromArray(
    JSContext* cx, HandleObject other, HandleObject proto /* = nullptr */,
    HandleObjectGroup group /* = nullptr */) {
  // Allow nullptr proto for FriendAPI methods, which don't care about
  // subclassing.
  if (other->is<TypedArrayObject>()) {
    return fromTypedArray(cx, other, /* wrapped= */ false, proto, group);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (other->is<WrapperObject>() &&
      UncheckedUnwrap(other)->is<TypedArrayObject>()) {
    return fromTypedArray(cx, other, /* wrapped= */ true, proto);
  }
||||||| merged common ancestors
    if (other->is<WrapperObject>() && UncheckedUnwrap(other)->is<TypedArrayObject>()) {
        return fromTypedArray(cx, other, /* wrapped= */ true, proto);
    }
=======
  if (other->is<WrapperObject>() &&
      UncheckedUnwrap(other)->is<TypedArrayObject>()) {
    return fromTypedArray(cx, other, /* wrapped= */ true, proto, group);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return fromObject(cx, other, proto);
||||||| merged common ancestors
    return fromObject(cx, other, proto);
=======
  return fromObject(cx, other, proto, group);
>>>>>>> upstream-releases
}

// ES2018 draft rev 272beb67bc5cd9fd18a220665198384108208ee1
// 22.2.4.3 TypedArray ( typedArray )
<<<<<<< HEAD
template <typename T>
/* static */ JSObject* TypedArrayObjectTemplate<T>::fromTypedArray(
    JSContext* cx, HandleObject other, bool isWrapped, HandleObject proto) {
  // Step 1.
  MOZ_ASSERT_IF(!isWrapped, other->is<TypedArrayObject>());
  MOZ_ASSERT_IF(isWrapped, other->is<WrapperObject>() &&
                               UncheckedUnwrap(other)->is<TypedArrayObject>());
||||||| merged common ancestors
template<typename T>
/* static */ JSObject*
TypedArrayObjectTemplate<T>::fromTypedArray(JSContext* cx, HandleObject other, bool isWrapped,
                                            HandleObject proto)
{
    // Step 1.
    MOZ_ASSERT_IF(!isWrapped, other->is<TypedArrayObject>());
    MOZ_ASSERT_IF(isWrapped,
                  other->is<WrapperObject>() &&
                  UncheckedUnwrap(other)->is<TypedArrayObject>());

    // Step 2 (Already performed in caller).

    // Steps 3-4 (Allocation deferred until later).

    // Step 5.
    Rooted<TypedArrayObject*> srcArray(cx);
    if (!isWrapped) {
        srcArray = &other->as<TypedArrayObject>();
    } else {
        RootedObject unwrapped(cx, CheckedUnwrap(other));
        if (!unwrapped) {
            ReportAccessDenied(cx);
            return nullptr;
        }
=======
template <typename T>
/* static */ TypedArrayObject* TypedArrayObjectTemplate<T>::fromTypedArray(
    JSContext* cx, HandleObject other, bool isWrapped, HandleObject proto,
    HandleObjectGroup group) {
  // Step 1.
  MOZ_ASSERT_IF(!isWrapped, other->is<TypedArrayObject>());
  MOZ_ASSERT_IF(isWrapped, other->is<WrapperObject>() &&
                               UncheckedUnwrap(other)->is<TypedArrayObject>());

  // Step 2 (Already performed in caller).

  // Steps 3-4 (Allocation deferred until later).

  // Step 5.
  Rooted<TypedArrayObject*> srcArray(cx);
  if (!isWrapped) {
    srcArray = &other->as<TypedArrayObject>();
  } else {
    srcArray = other->maybeUnwrapAs<TypedArrayObject>();
    if (!srcArray) {
      ReportAccessDenied(cx);
      return nullptr;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 2 (Already performed in caller).
||||||| merged common ancestors
        JSAutoRealm ar(cx, unwrapped);
=======
  // To keep things simpler, we always reify the array buffer for cross-realm or
  // wrapped typed arrays. Note: isWrapped does not imply cross-realm, because
  // of same-compartment wrappers.
  if (cx->realm() != srcArray->realm() || isWrapped) {
    if (!TypedArrayObject::ensureHasBuffer(cx, srcArray)) {
      return nullptr;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Steps 3-4 (Allocation deferred until later).
||||||| merged common ancestors
        srcArray = &unwrapped->as<TypedArrayObject>();
=======
  // Step 6 (skipped).
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 5.
  Rooted<TypedArrayObject*> srcArray(cx);
  if (!isWrapped) {
    srcArray = &other->as<TypedArrayObject>();
  } else {
    RootedObject unwrapped(cx, CheckedUnwrap(other));
    if (!unwrapped) {
      ReportAccessDenied(cx);
      return nullptr;
    }
||||||| merged common ancestors
        // To keep things simpler, we always reify the array buffer for
        // wrapped typed arrays.
        if (!TypedArrayObject::ensureHasBuffer(cx, srcArray)) {
            return nullptr;
        }
    }
=======
  // Step 7.
  if (srcArray->hasDetachedBuffer()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_TYPED_ARRAY_DETACHED);
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    JSAutoRealm ar(cx, unwrapped);
||||||| merged common ancestors
    // Step 6 (skipped).
=======
  // Step 8 (skipped).
>>>>>>> upstream-releases

<<<<<<< HEAD
    srcArray = &unwrapped->as<TypedArrayObject>();

    // To keep things simpler, we always reify the array buffer for
    // wrapped typed arrays.
    if (!TypedArrayObject::ensureHasBuffer(cx, srcArray)) {
      return nullptr;
    }
  }
||||||| merged common ancestors
    // Step 7.
    if (srcArray->hasDetachedBuffer()) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_TYPED_ARRAY_DETACHED);
        return nullptr;
    }
=======
  // Step 9.
  uint32_t elementLength = srcArray->length();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 6 (skipped).
||||||| merged common ancestors
    // Step 9.
    uint32_t elementLength = srcArray->length();
=======
  // Steps 10-15 (skipped).
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 7.
  if (srcArray->hasDetachedBuffer()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_TYPED_ARRAY_DETACHED);
    return nullptr;
  }
||||||| merged common ancestors
    // Steps 10-11.
    Scalar::Type srcType = srcArray->type();
=======
  // Steps 16-17.
  bool isShared = srcArray->isSharedMemory();
  SpeciesConstructorOverride override =
      isShared ? SpeciesConstructorOverride::ArrayBuffer
               : SpeciesConstructorOverride::None;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 9.
  uint32_t elementLength = srcArray->length();
||||||| merged common ancestors
    // Steps 12-13 (skipped).
=======
  RootedObject bufferCtor(
      cx, GetBufferSpeciesConstructor(cx, srcArray, isWrapped, override));
  if (!bufferCtor) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Steps 10-11.
  Scalar::Type srcType = srcArray->type();
||||||| merged common ancestors
    // Steps 16-17.
    bool isShared = srcArray->isSharedMemory();
    SpeciesConstructorOverride override = isShared ? SpeciesConstructorOverride::ArrayBuffer
                                                   : SpeciesConstructorOverride::None;
=======
  // Steps 18-19.
  Rooted<ArrayBufferObject*> buffer(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Steps 12-13 (skipped).
||||||| merged common ancestors
    RootedObject bufferCtor(cx, GetBufferSpeciesConstructor(cx, srcArray, isWrapped, override));
    if (!bufferCtor) {
        return nullptr;
    }
=======
  // Step 19.a or 18.a, 24.1.1.4 CloneArrayBuffer(...) steps 1-3.
  if (!AllocateArrayBuffer(cx, bufferCtor, elementLength, &buffer)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Steps 16-17.
  bool isShared = srcArray->isSharedMemory();
  SpeciesConstructorOverride override =
      isShared ? SpeciesConstructorOverride::ArrayBuffer
               : SpeciesConstructorOverride::None;
||||||| merged common ancestors
    // Steps 8, 18-19.
    Rooted<ArrayBufferObject*> buffer(cx);
    if (ArrayTypeID() == srcType) {
        // Step 15.
        uint32_t byteLength = srcArray->byteLength();

        // Step 18.a.
        // 24.1.1.4 CloneArrayBuffer(...), steps 1-3.
        if (!AllocateArrayBuffer(cx, bufferCtor, byteLength, 1, &buffer)) {
            return nullptr;
        }
    } else {
        // Steps 14-15, 19.a.
        if (!AllocateArrayBuffer(cx, bufferCtor, elementLength, BYTES_PER_ELEMENT, &buffer)) {
            return nullptr;
        }
    }
=======
  // Step 19.b or 24.1.1.4 step 4.
  if (srcArray->hasDetachedBuffer()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_TYPED_ARRAY_DETACHED);
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedObject bufferCtor(
      cx, GetBufferSpeciesConstructor(cx, srcArray, isWrapped, override));
  if (!bufferCtor) {
    return nullptr;
  }
||||||| merged common ancestors
    // Step 19.b or 24.1.1.4 step 4.
    if (srcArray->hasDetachedBuffer()) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_TYPED_ARRAY_DETACHED);
        return nullptr;
    }
=======
  // BigInt proposal 7.24, step 19.c.
  if (Scalar::isBigIntType(ArrayTypeID()) !=
      Scalar::isBigIntType(srcArray->type())) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_NOT_BIGINT);
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Steps 8, 18-19.
  Rooted<ArrayBufferObject*> buffer(cx);
  if (ArrayTypeID() == srcType) {
    // Step 15.
    uint32_t byteLength = srcArray->byteLength();
||||||| merged common ancestors
    // Steps 3-4 (remaining part), 20-23.
    Rooted<TypedArrayObject*> obj(cx, makeInstance(cx, buffer, CreateSingleton::No, 0,
                                                   elementLength, proto));
    if (!obj) {
        return nullptr;
    }
=======
  // Steps 3-4 (remaining part), 20-23.
  Rooted<TypedArrayObject*> obj(
      cx, makeInstance(cx, buffer, CreateSingleton::No, 0, elementLength, proto,
                       group));
  if (!obj) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step 18.a.
    // 24.1.1.4 CloneArrayBuffer(...), steps 1-3.
    if (!AllocateArrayBuffer(cx, bufferCtor, byteLength, 1, &buffer)) {
      return nullptr;
||||||| merged common ancestors
    // Steps 19.c-f or 24.1.1.4 steps 5-7.
    MOZ_ASSERT(!obj->isSharedMemory());
    if (isShared) {
        if (!ElementSpecific<T, SharedOps>::setFromTypedArray(obj, srcArray, 0)) {
            return nullptr;
        }
    } else {
        if (!ElementSpecific<T, UnsharedOps>::setFromTypedArray(obj, srcArray, 0)) {
            return nullptr;
        }
=======
  // Steps 19.c-f or 24.1.1.4 steps 5-7.
  MOZ_ASSERT(!obj->isSharedMemory());
  if (isShared) {
    if (!ElementSpecific<T, SharedOps>::setFromTypedArray(obj, srcArray, 0)) {
      return nullptr;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  } else {
    // Steps 14-15, 19.a.
    if (!AllocateArrayBuffer(cx, bufferCtor, elementLength, BYTES_PER_ELEMENT,
                             &buffer)) {
      return nullptr;
    }
  }
||||||| merged common ancestors
=======
  } else {
    if (!ElementSpecific<T, UnsharedOps>::setFromTypedArray(obj, srcArray, 0)) {
      return nullptr;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 19.b or 24.1.1.4 step 4.
  if (srcArray->hasDetachedBuffer()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_TYPED_ARRAY_DETACHED);
    return nullptr;
  }
||||||| merged common ancestors
    // Step 24.
    return obj;
}
=======
  // Step 24.
  return obj;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Steps 3-4 (remaining part), 20-23.
  Rooted<TypedArrayObject*> obj(
      cx,
      makeInstance(cx, buffer, CreateSingleton::No, 0, elementLength, proto));
  if (!obj) {
    return nullptr;
  }
||||||| merged common ancestors
static MOZ_ALWAYS_INLINE bool
IsOptimizableInit(JSContext* cx, HandleObject iterable, bool* optimized)
{
    MOZ_ASSERT(!*optimized);
=======
static MOZ_ALWAYS_INLINE bool IsOptimizableInit(JSContext* cx,
                                                HandleObject iterable,
                                                bool* optimized) {
  MOZ_ASSERT(!*optimized);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Steps 19.c-f or 24.1.1.4 steps 5-7.
  MOZ_ASSERT(!obj->isSharedMemory());
  if (isShared) {
    if (!ElementSpecific<T, SharedOps>::setFromTypedArray(obj, srcArray, 0)) {
      return nullptr;
    }
  } else {
    if (!ElementSpecific<T, UnsharedOps>::setFromTypedArray(obj, srcArray, 0)) {
      return nullptr;
    }
  }
||||||| merged common ancestors
    if (!IsPackedArray(iterable)) {
        return true;
    }

    ForOfPIC::Chain* stubChain = ForOfPIC::getOrCreate(cx);
    if (!stubChain) {
        return false;
    }
=======
  if (!IsPackedArray(iterable)) {
    return true;
  }

  ForOfPIC::Chain* stubChain = ForOfPIC::getOrCreate(cx);
  if (!stubChain) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 24.
  return obj;
||||||| merged common ancestors
    return stubChain->tryOptimizeArray(cx, iterable.as<ArrayObject>(), optimized);
=======
  return stubChain->tryOptimizeArray(cx, iterable.as<ArrayObject>(), optimized);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static MOZ_ALWAYS_INLINE bool IsOptimizableInit(JSContext* cx,
                                                HandleObject iterable,
                                                bool* optimized) {
  MOZ_ASSERT(!*optimized);
||||||| merged common ancestors
// ES2017 draft rev 6859bb9ccaea9c6ede81d71e5320e3833b92cb3e
// 22.2.4.4 TypedArray ( object )
template<typename T>
/* static */ JSObject*
TypedArrayObjectTemplate<T>::fromObject(JSContext* cx, HandleObject other, HandleObject proto)
{
    // Steps 1-2 (Already performed in caller).
=======
// ES2017 draft rev 6859bb9ccaea9c6ede81d71e5320e3833b92cb3e
// 22.2.4.4 TypedArray ( object )
template <typename T>
/* static */ TypedArrayObject* TypedArrayObjectTemplate<T>::fromObject(
    JSContext* cx, HandleObject other, HandleObject proto,
    HandleObjectGroup group) {
  // Steps 1-2 (Already performed in caller).
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!IsPackedArray(iterable)) {
    return true;
  }
||||||| merged common ancestors
    // Steps 3-4 (Allocation deferred until later).
=======
  // Steps 3-4 (Allocation deferred until later).
>>>>>>> upstream-releases

<<<<<<< HEAD
  ForOfPIC::Chain* stubChain = ForOfPIC::getOrCreate(cx);
  if (!stubChain) {
    return false;
  }
||||||| merged common ancestors
    bool optimized = false;
    if (!IsOptimizableInit(cx, other, &optimized)) {
        return nullptr;
    }
=======
  bool optimized = false;
  if (!IsOptimizableInit(cx, other, &optimized)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return stubChain->tryOptimizeArray(cx, iterable.as<ArrayObject>(), optimized);
}
||||||| merged common ancestors
    // Fast path when iterable is a packed array using the default iterator.
    if (optimized) {
        // Step 6.a (We don't need to call IterableToList for the fast path).
        HandleArrayObject array = other.as<ArrayObject>();
=======
  // Fast path when iterable is a packed array using the default iterator.
  if (optimized) {
    // Step 6.a (We don't need to call IterableToList for the fast path).
    HandleArrayObject array = other.as<ArrayObject>();
>>>>>>> upstream-releases

<<<<<<< HEAD
// ES2017 draft rev 6859bb9ccaea9c6ede81d71e5320e3833b92cb3e
// 22.2.4.4 TypedArray ( object )
template <typename T>
/* static */ JSObject* TypedArrayObjectTemplate<T>::fromObject(
    JSContext* cx, HandleObject other, HandleObject proto) {
  // Steps 1-2 (Already performed in caller).
||||||| merged common ancestors
        // Step 6.b.
        uint32_t len = array->getDenseInitializedLength();
=======
    // Step 6.b.
    uint32_t len = array->getDenseInitializedLength();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Steps 3-4 (Allocation deferred until later).
||||||| merged common ancestors
        // Step 6.c.
        Rooted<ArrayBufferObject*> buffer(cx);
        if (!maybeCreateArrayBuffer(cx, len, BYTES_PER_ELEMENT, nullptr, &buffer)) {
            return nullptr;
        }
=======
    // Step 6.c.
    Rooted<ArrayBufferObject*> buffer(cx);
    if (!maybeCreateArrayBuffer(cx, len, nullptr, &buffer)) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool optimized = false;
  if (!IsOptimizableInit(cx, other, &optimized)) {
    return nullptr;
  }
||||||| merged common ancestors
        Rooted<TypedArrayObject*> obj(cx, makeInstance(cx, buffer, CreateSingleton::No, 0,
                                                       len, proto));
        if (!obj) {
            return nullptr;
        }
=======
    Rooted<TypedArrayObject*> obj(
        cx,
        makeInstance(cx, buffer, CreateSingleton::No, 0, len, proto, group));
    if (!obj) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Fast path when iterable is a packed array using the default iterator.
  if (optimized) {
    // Step 6.a (We don't need to call IterableToList for the fast path).
    HandleArrayObject array = other.as<ArrayObject>();
||||||| merged common ancestors
        // Steps 6.d-e.
        MOZ_ASSERT(!obj->isSharedMemory());
        if (!ElementSpecific<T, UnsharedOps>::initFromIterablePackedArray(cx, obj, array)) {
            return nullptr;
        }
=======
    // Steps 6.d-e.
    MOZ_ASSERT(!obj->isSharedMemory());
    if (!ElementSpecific<T, UnsharedOps>::initFromIterablePackedArray(cx, obj,
                                                                      array)) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step 6.b.
    uint32_t len = array->getDenseInitializedLength();
||||||| merged common ancestors
        // Step 6.f (The assertion isn't applicable for the fast path).
=======
    // Step 6.f (The assertion isn't applicable for the fast path).
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step 6.c.
    Rooted<ArrayBufferObject*> buffer(cx);
    if (!maybeCreateArrayBuffer(cx, len, BYTES_PER_ELEMENT, nullptr, &buffer)) {
      return nullptr;
    }
||||||| merged common ancestors
        // Step 6.g.
        return obj;
    }
=======
    // Step 6.g.
    return obj;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    Rooted<TypedArrayObject*> obj(
        cx, makeInstance(cx, buffer, CreateSingleton::No, 0, len, proto));
    if (!obj) {
      return nullptr;
||||||| merged common ancestors
    // Step 5.
    RootedValue callee(cx);
    RootedId iteratorId(cx, SYMBOL_TO_JSID(cx->wellKnownSymbols().iterator));
    if (!GetProperty(cx, other, other, iteratorId, &callee)) {
        return nullptr;
=======
  // Step 5.
  RootedValue callee(cx);
  RootedId iteratorId(cx, SYMBOL_TO_JSID(cx->wellKnownSymbols().iterator));
  if (!GetProperty(cx, other, other, iteratorId, &callee)) {
    return nullptr;
  }

  // Steps 6-8.
  RootedObject arrayLike(cx);
  if (!callee.isNullOrUndefined()) {
    // Throw if other[Symbol.iterator] isn't callable.
    if (!callee.isObject() || !callee.toObject().isCallable()) {
      RootedValue otherVal(cx, ObjectValue(*other));
      UniqueChars bytes =
          DecompileValueGenerator(cx, JSDVG_SEARCH_STACK, otherVal, nullptr);
      if (!bytes) {
        return nullptr;
      }
      JS_ReportErrorNumberUTF8(cx, GetErrorMessage, nullptr, JSMSG_NOT_ITERABLE,
                               bytes.get());
      return nullptr;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    // Steps 6.d-e.
    MOZ_ASSERT(!obj->isSharedMemory());
    if (!ElementSpecific<T, UnsharedOps>::initFromIterablePackedArray(cx, obj,
                                                                      array)) {
      return nullptr;
    }
||||||| merged common ancestors
    // Steps 6-8.
    RootedObject arrayLike(cx);
    if (!callee.isNullOrUndefined()) {
        // Throw if other[Symbol.iterator] isn't callable.
        if (!callee.isObject() || !callee.toObject().isCallable()) {
            RootedValue otherVal(cx, ObjectValue(*other));
            UniqueChars bytes = DecompileValueGenerator(cx, JSDVG_SEARCH_STACK, otherVal, nullptr);
            if (!bytes) {
                return nullptr;
            }
            JS_ReportErrorNumberUTF8(cx, GetErrorMessage, nullptr, JSMSG_NOT_ITERABLE,
                                     bytes.get());
            return nullptr;
        }

        FixedInvokeArgs<2> args2(cx);
        args2[0].setObject(*other);
        args2[1].set(callee);
=======
    FixedInvokeArgs<2> args2(cx);
    args2[0].setObject(*other);
    args2[1].set(callee);

    // Step 6.a.
    RootedValue rval(cx);
    if (!CallSelfHostedFunction(cx, cx->names().IterableToList,
                                UndefinedHandleValue, args2, &rval)) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step 6.f (The assertion isn't applicable for the fast path).
||||||| merged common ancestors
        // Step 6.a.
        RootedValue rval(cx);
        if (!CallSelfHostedFunction(cx, cx->names().IterableToList, UndefinedHandleValue, args2,
                                    &rval))
        {
            return nullptr;
        }
=======
    // Steps 6.b-g (Implemented in steps 9-13 below).
    arrayLike = &rval.toObject();
  } else {
    // Step 7 is an assertion: object is not an Iterator. Testing this is
    // literally the very last thing we did, so we don't assert here.
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step 6.g.
    return obj;
  }
||||||| merged common ancestors
        // Steps 6.b-g (Implemented in steps 9-13 below).
        arrayLike = &rval.toObject();
    } else {
        // Step 7 is an assertion: object is not an Iterator. Testing this is
        // literally the very last thing we did, so we don't assert here.
=======
    // Step 8.
    arrayLike = other;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 5.
  RootedValue callee(cx);
  RootedId iteratorId(cx, SYMBOL_TO_JSID(cx->wellKnownSymbols().iterator));
  if (!GetProperty(cx, other, other, iteratorId, &callee)) {
    return nullptr;
  }
||||||| merged common ancestors
        // Step 8.
        arrayLike = other;
    }
=======
  // Step 9.
  uint32_t len;
  if (!GetLengthProperty(cx, arrayLike, &len)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Steps 6-8.
  RootedObject arrayLike(cx);
  if (!callee.isNullOrUndefined()) {
    // Throw if other[Symbol.iterator] isn't callable.
    if (!callee.isObject() || !callee.toObject().isCallable()) {
      RootedValue otherVal(cx, ObjectValue(*other));
      UniqueChars bytes =
          DecompileValueGenerator(cx, JSDVG_SEARCH_STACK, otherVal, nullptr);
      if (!bytes) {
        return nullptr;
      }
      JS_ReportErrorNumberUTF8(cx, GetErrorMessage, nullptr, JSMSG_NOT_ITERABLE,
                               bytes.get());
      return nullptr;
    }
||||||| merged common ancestors
    // Step 9.
    uint32_t len;
    if (!GetLengthProperty(cx, arrayLike, &len)) {
        return nullptr;
    }
=======
  // Step 10.
  Rooted<ArrayBufferObject*> buffer(cx);
  if (!maybeCreateArrayBuffer(cx, len, nullptr, &buffer)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    FixedInvokeArgs<2> args2(cx);
    args2[0].setObject(*other);
    args2[1].set(callee);
||||||| merged common ancestors
    // Step 10.
    Rooted<ArrayBufferObject*> buffer(cx);
    if (!maybeCreateArrayBuffer(cx, len, BYTES_PER_ELEMENT, nullptr, &buffer)) {
        return nullptr;
    }
=======
  Rooted<TypedArrayObject*> obj(
      cx, makeInstance(cx, buffer, CreateSingleton::No, 0, len, proto, group));
  if (!obj) {
    return nullptr;
  }

  // Steps 11-12.
  MOZ_ASSERT(!obj->isSharedMemory());
  if (!ElementSpecific<T, UnsharedOps>::setFromNonTypedArray(cx, obj, arrayLike,
                                                             len)) {
    return nullptr;
  }

  // Step 13.
  return obj;
}

bool TypedArrayConstructor(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                            JSMSG_TYPED_ARRAY_CALL_OR_CONSTRUCT,
                            args.isConstructing() ? "construct" : "call");
  return false;
}

template <typename T>
static bool GetTemplateObjectForNative(JSContext* cx,
                                       const JS::HandleValueArray args,
                                       MutableHandleObject res) {
  if (args.length() == 0) {
    return true;
  }

  HandleValue arg = args[0];
  if (arg.isInt32()) {
    uint32_t len = 0;
    if (arg.toInt32() >= 0) {
      len = arg.toInt32();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step 6.a.
    RootedValue rval(cx);
    if (!CallSelfHostedFunction(cx, cx->names().IterableToList,
                                UndefinedHandleValue, args2, &rval)) {
      return nullptr;
||||||| merged common ancestors
    Rooted<TypedArrayObject*> obj(cx, makeInstance(cx, buffer, CreateSingleton::No, 0, len,
                                                   proto));
    if (!obj) {
        return nullptr;
=======
    size_t nbytes;
    if (!js::CalculateAllocSize<T>(len, &nbytes)) {
      return true;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    // Steps 6.b-g (Implemented in steps 9-13 below).
    arrayLike = &rval.toObject();
  } else {
    // Step 7 is an assertion: object is not an Iterator. Testing this is
    // literally the very last thing we did, so we don't assert here.
||||||| merged common ancestors
    // Steps 11-12.
    MOZ_ASSERT(!obj->isSharedMemory());
    if (!ElementSpecific<T, UnsharedOps>::setFromNonTypedArray(cx, obj, arrayLike, len)) {
        return nullptr;
    }
=======
    if (nbytes >= TypedArrayObject::SINGLETON_BYTE_LENGTH) {
      return true;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step 8.
    arrayLike = other;
  }

  // Step 9.
  uint32_t len;
  if (!GetLengthProperty(cx, arrayLike, &len)) {
    return nullptr;
  }

  // Step 10.
  Rooted<ArrayBufferObject*> buffer(cx);
  if (!maybeCreateArrayBuffer(cx, len, BYTES_PER_ELEMENT, nullptr, &buffer)) {
    return nullptr;
  }

  Rooted<TypedArrayObject*> obj(
      cx, makeInstance(cx, buffer, CreateSingleton::No, 0, len, proto));
  if (!obj) {
    return nullptr;
  }

  // Steps 11-12.
  MOZ_ASSERT(!obj->isSharedMemory());
  if (!ElementSpecific<T, UnsharedOps>::setFromNonTypedArray(cx, obj, arrayLike,
                                                             len)) {
    return nullptr;
  }

  // Step 13.
  return obj;
}
||||||| merged common ancestors
    // Step 13.
    return obj;
}
=======
    res.set(TypedArrayObjectTemplate<T>::makeTemplateObject(cx, len));
    return !!res;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool TypedArrayConstructor(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                            JSMSG_TYPED_ARRAY_CALL_OR_CONSTRUCT,
                            args.isConstructing() ? "construct" : "call");
  return false;
||||||| merged common ancestors
bool
TypedArrayConstructor(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_TYPED_ARRAY_CALL_OR_CONSTRUCT,
                              args.isConstructing() ? "construct" : "call");
    return false;
=======
  // We don't support wrappers, because of the complicated interaction between
  // wrapped ArrayBuffers and TypedArrays, see |fromBufferWrapped()|.
  if (arg.isObject() && !IsWrapper(&arg.toObject())) {
    // We don't use the template's length in the object case, so we can create
    // the template typed array with an initial length of zero.
    uint32_t len = 0;
    res.set(TypedArrayObjectTemplate<T>::makeTemplateObject(cx, len));
    return !!res;
  }

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ bool TypedArrayObject::GetTemplateObjectForNative(
    JSContext* cx, Native native, uint32_t len, MutableHandleObject res) {
#define CHECK_TYPED_ARRAY_CONSTRUCTOR(T, N)                              \
  if (native == &TypedArrayObjectTemplate<T>::class_constructor) {       \
    size_t nbytes;                                                       \
    if (!js::CalculateAllocSize<T>(len, &nbytes)) return true;           \
                                                                         \
    if (nbytes < TypedArrayObject::SINGLETON_BYTE_LENGTH) {              \
      res.set(TypedArrayObjectTemplate<T>::makeTemplateObject(cx, len)); \
      return !!res;                                                      \
    }                                                                    \
  }
  JS_FOR_EACH_TYPED_ARRAY(CHECK_TYPED_ARRAY_CONSTRUCTOR)
||||||| merged common ancestors
/* static */ bool
TypedArrayObject::GetTemplateObjectForNative(JSContext* cx, Native native, uint32_t len,
                                             MutableHandleObject res)
{
#define CHECK_TYPED_ARRAY_CONSTRUCTOR(T, N) \
    if (native == &TypedArrayObjectTemplate<T>::class_constructor) { \
        size_t nbytes; \
        if (!js::CalculateAllocSize<T>(len, &nbytes)) \
            return true; \
        \
        if (nbytes < TypedArrayObject::SINGLETON_BYTE_LENGTH) { \
            res.set(TypedArrayObjectTemplate<T>::makeTemplateObject(cx, len)); \
            return !!res; \
        } \
    }
JS_FOR_EACH_TYPED_ARRAY(CHECK_TYPED_ARRAY_CONSTRUCTOR)
=======
/* static */ bool TypedArrayObject::GetTemplateObjectForNative(
    JSContext* cx, Native native, const JS::HandleValueArray args,
    MutableHandleObject res) {
  MOZ_ASSERT(!res);
#define CHECK_TYPED_ARRAY_CONSTRUCTOR(T, N)                        \
  if (native == &TypedArrayObjectTemplate<T>::class_constructor) { \
    return ::GetTemplateObjectForNative<T>(cx, args, res);         \
  }
  JS_FOR_EACH_TYPED_ARRAY(CHECK_TYPED_ARRAY_CONSTRUCTOR)
>>>>>>> upstream-releases
#undef CHECK_TYPED_ARRAY_CONSTRUCTOR
<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  return true;
}

static bool TypedArray_lengthGetter(JSContext* cx, unsigned argc, Value* vp) {
  return TypedArrayObject::Getter<TypedArrayObject::lengthValue>(cx, argc, vp);
}

static bool TypedArray_byteOffsetGetter(JSContext* cx, unsigned argc,
                                        Value* vp) {
  return TypedArrayObject::Getter<TypedArrayObject::byteOffsetValue>(cx, argc,
                                                                     vp);
}

bool BufferGetterImpl(JSContext* cx, const CallArgs& args) {
  MOZ_ASSERT(TypedArrayObject::is(args.thisv()));
  Rooted<TypedArrayObject*> tarray(
      cx, &args.thisv().toObject().as<TypedArrayObject>());
  if (!TypedArrayObject::ensureHasBuffer(cx, tarray)) {
    return false;
  }
  args.rval().set(TypedArrayObject::bufferValue(tarray));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool TypedArray_lengthGetter(JSContext* cx, unsigned argc, Value* vp) {
  return TypedArrayObject::Getter<TypedArrayObject::lengthValue>(cx, argc, vp);
||||||| merged common ancestors
static bool
TypedArray_lengthGetter(JSContext* cx, unsigned argc, Value* vp)
{
    return TypedArrayObject::Getter<TypedArrayObject::lengthValue>(cx, argc, vp);
=======
/*static*/
bool js::TypedArray_bufferGetter(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  return CallNonGenericMethod<TypedArrayObject::is, BufferGetterImpl>(cx, args);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BufferGetterImpl(JSContext* cx, const CallArgs& args) {
  MOZ_ASSERT(TypedArrayObject::is(args.thisv()));
  Rooted<TypedArrayObject*> tarray(
      cx, &args.thisv().toObject().as<TypedArrayObject>());
  if (!TypedArrayObject::ensureHasBuffer(cx, tarray)) {
    return false;
  }
  args.rval().set(TypedArrayObject::bufferValue(tarray));
  return true;
}
||||||| merged common ancestors
bool
BufferGetterImpl(JSContext* cx, const CallArgs& args)
{
    MOZ_ASSERT(TypedArrayObject::is(args.thisv()));
    Rooted<TypedArrayObject*> tarray(cx, &args.thisv().toObject().as<TypedArrayObject>());
    if (!TypedArrayObject::ensureHasBuffer(cx, tarray)) {
        return false;
    }
    args.rval().set(TypedArrayObject::bufferValue(tarray));
    return true;
}
=======
// ES2019 draft rev fc9ecdcd74294d0ca3146d4b274e2a8e79565dc3
// 22.2.3.32 get %TypedArray%.prototype [ @@toStringTag ]
static bool TypedArray_toStringTagGetter(JSContext* cx, unsigned argc,
                                         Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Steps 1-2.
  if (!args.thisv().isObject()) {
    args.rval().setUndefined();
    return true;
  }

  JSObject* obj = CheckedUnwrapStatic(&args.thisv().toObject());
  if (!obj) {
    ReportAccessDenied(cx);
    return false;
  }

  // Step 3.
  if (!obj->is<TypedArrayObject>()) {
    args.rval().setUndefined();
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
/*static*/ bool js::TypedArray_bufferGetter(JSContext* cx, unsigned argc,
                                            Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  return CallNonGenericMethod<TypedArrayObject::is, BufferGetterImpl>(cx, args);
||||||| merged common ancestors
/*static*/ bool
js::TypedArray_bufferGetter(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
    return CallNonGenericMethod<TypedArrayObject::is, BufferGetterImpl>(cx, args);
=======
  // Steps 4-6.
  JSProtoKey protoKey = StandardProtoKeyOrNull(obj);
  MOZ_ASSERT(protoKey);

  args.rval().setString(ClassName(protoKey, cx));
  return true;
>>>>>>> upstream-releases
}

/* static */ const JSPropertySpec TypedArrayObject::protoAccessors[] = {
    JS_PSG("length", TypedArray_lengthGetter, 0),
    JS_PSG("buffer", TypedArray_bufferGetter, 0),
<<<<<<< HEAD
    JS_PSG("byteLength",
           TypedArrayObject::Getter<TypedArrayObject::byteLengthValue>, 0),
    JS_PSG("byteOffset",
           TypedArrayObject::Getter<TypedArrayObject::byteOffsetValue>, 0),
    JS_SELF_HOSTED_SYM_GET(toStringTag, "TypedArrayToStringTag", 0),
    JS_PS_END};

template <typename T>
static inline bool SetFromTypedArray(Handle<TypedArrayObject*> target,
                                     Handle<TypedArrayObject*> source,
                                     uint32_t offset) {
  // WARNING: |source| may be an unwrapped typed array from a different
  // compartment. Proceed with caution!

  if (target->isSharedMemory() || source->isSharedMemory()) {
    return ElementSpecific<T, SharedOps>::setFromTypedArray(target, source,
                                                            offset);
  }
  return ElementSpecific<T, UnsharedOps>::setFromTypedArray(target, source,
                                                            offset);
||||||| merged common ancestors
    JS_PSG("byteLength", TypedArrayObject::Getter<TypedArrayObject::byteLengthValue>, 0),
    JS_PSG("byteOffset", TypedArrayObject::Getter<TypedArrayObject::byteOffsetValue>, 0),
    JS_SELF_HOSTED_SYM_GET(toStringTag, "TypedArrayToStringTag", 0),
    JS_PS_END
};

template<typename T>
static inline bool
SetFromTypedArray(Handle<TypedArrayObject*> target, Handle<TypedArrayObject*> source,
                  uint32_t offset)
{
    // WARNING: |source| may be an unwrapped typed array from a different
    // compartment. Proceed with caution!

    if (target->isSharedMemory() || source->isSharedMemory()) {
        return ElementSpecific<T, SharedOps>::setFromTypedArray(target, source, offset);
    }
    return ElementSpecific<T, UnsharedOps>::setFromTypedArray(target, source, offset);
=======
    JS_PSG("byteLength",
           TypedArrayObject::Getter<TypedArrayObject::byteLengthValue>, 0),
    JS_PSG("byteOffset", TypedArray_byteOffsetGetter, 0),
    JS_SYM_GET(toStringTag, TypedArray_toStringTagGetter, 0),
    JS_PS_END};

template <typename T>
static inline bool SetFromTypedArray(Handle<TypedArrayObject*> target,
                                     Handle<TypedArrayObject*> source,
                                     uint32_t offset) {
  // WARNING: |source| may be an unwrapped typed array from a different
  // compartment. Proceed with caution!

  if (target->isSharedMemory() || source->isSharedMemory()) {
    return ElementSpecific<T, SharedOps>::setFromTypedArray(target, source,
                                                            offset);
  }
  return ElementSpecific<T, UnsharedOps>::setFromTypedArray(target, source,
                                                            offset);
>>>>>>> upstream-releases
}

template <typename T>
static inline bool SetFromNonTypedArray(JSContext* cx,
                                        Handle<TypedArrayObject*> target,
                                        HandleObject source, uint32_t len,
                                        uint32_t offset) {
  MOZ_ASSERT(!source->is<TypedArrayObject>(), "use SetFromTypedArray");

  if (target->isSharedMemory()) {
    return ElementSpecific<T, SharedOps>::setFromNonTypedArray(
        cx, target, source, len, offset);
  }
  return ElementSpecific<T, UnsharedOps>::setFromNonTypedArray(
      cx, target, source, len, offset);
}

// ES2017 draft rev c57ef95c45a371f9c9485bb1c3881dbdc04524a2
// 22.2.3.23 %TypedArray%.prototype.set ( overloaded [ , offset ] )
// 22.2.3.23.1 %TypedArray%.prototype.set ( array [ , offset ] )
// 22.2.3.23.2 %TypedArray%.prototype.set( typedArray [ , offset ] )
<<<<<<< HEAD
/* static */ bool TypedArrayObject::set_impl(JSContext* cx,
                                             const CallArgs& args) {
  MOZ_ASSERT(TypedArrayObject::is(args.thisv()));
||||||| merged common ancestors
/* static */ bool
TypedArrayObject::set_impl(JSContext* cx, const CallArgs& args)
{
    MOZ_ASSERT(TypedArrayObject::is(args.thisv()));

    // Steps 1-5 (Validation performed as part of CallNonGenericMethod).
    Rooted<TypedArrayObject*> target(cx, &args.thisv().toObject().as<TypedArrayObject>());

    // Steps 6-7.
    double targetOffset = 0;
    if (args.length() > 1) {
        // Step 6.
        if (!ToInteger(cx, args[1], &targetOffset)) {
            return false;
        }
=======
/* static */
bool TypedArrayObject::set_impl(JSContext* cx, const CallArgs& args) {
  MOZ_ASSERT(TypedArrayObject::is(args.thisv()));
>>>>>>> upstream-releases

  // Steps 1-5 (Validation performed as part of CallNonGenericMethod).
  Rooted<TypedArrayObject*> target(
      cx, &args.thisv().toObject().as<TypedArrayObject>());

  // Steps 6-7.
  double targetOffset = 0;
  if (args.length() > 1) {
    // Step 6.
    if (!ToInteger(cx, args[1], &targetOffset)) {
      return false;
    }

    // Step 7.
    if (targetOffset < 0) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_BAD_INDEX);
      return false;
    }
  }

  // Steps 8-9.
  if (target->hasDetachedBuffer()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_TYPED_ARRAY_DETACHED);
    return false;
  }

<<<<<<< HEAD
  // 22.2.3.23.1, step 15. (22.2.3.23.2 only applies if args[0] is a typed
  // array, so it doesn't make a difference there to apply ToObject here.)
  RootedObject src(cx, ToObject(cx, args.get(0)));
  if (!src) {
    return false;
  }

  Rooted<TypedArrayObject*> srcTypedArray(cx);
  {
    JSObject* obj = CheckedUnwrap(src);
    if (!obj) {
      ReportAccessDenied(cx);
      return false;
    }

    if (obj->is<TypedArrayObject>()) {
      srcTypedArray = &obj->as<TypedArrayObject>();
||||||| merged common ancestors
        if (obj->is<TypedArrayObject>()) {
            srcTypedArray = &obj->as<TypedArrayObject>();
        }
=======
  // 22.2.3.23.1, step 15. (22.2.3.23.2 only applies if args[0] is a typed
  // array, so it doesn't make a difference there to apply ToObject here.)
  RootedObject src(cx, ToObject(cx, args.get(0)));
  if (!src) {
    return false;
  }

  Rooted<TypedArrayObject*> srcTypedArray(cx);
  {
    JSObject* obj = CheckedUnwrapStatic(src);
    if (!obj) {
      ReportAccessDenied(cx);
      return false;
    }

    if (obj->is<TypedArrayObject>()) {
      srcTypedArray = &obj->as<TypedArrayObject>();
>>>>>>> upstream-releases
    }
  }

  if (srcTypedArray) {
    // Remaining steps of 22.2.3.23.2.

    // WARNING: |srcTypedArray| may be an unwrapped typed array from a
    // different compartment. Proceed with caution!

    // Steps 11-12.
    if (srcTypedArray->hasDetachedBuffer()) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_TYPED_ARRAY_DETACHED);
      return false;
    }

    // Step 10 (Reordered).
    uint32_t targetLength = target->length();

    // Step 22 (Split into two checks to provide better error messages).
    if (targetOffset > targetLength) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_BAD_INDEX);
      return false;
    }

<<<<<<< HEAD
    // Step 22 (Cont'd).
    uint32_t offset = uint32_t(targetOffset);
    if (srcTypedArray->length() > targetLength - offset) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_BAD_ARRAY_LENGTH);
      return false;
    }
||||||| merged common ancestors
        // Step 22 (Cont'd).
        uint32_t offset = uint32_t(targetOffset);
        if (srcTypedArray->length() > targetLength - offset) {
            JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_BAD_ARRAY_LENGTH);
            return false;
        }
=======
    // Step 22 (Cont'd).
    uint32_t offset = uint32_t(targetOffset);
    if (srcTypedArray->length() > targetLength - offset) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_SOURCE_ARRAY_TOO_LONG);
      return false;
    }

    if (Scalar::isBigIntType(target->type()) !=
        Scalar::isBigIntType(srcTypedArray->type())) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_NOT_BIGINT);
      return false;
    }
>>>>>>> upstream-releases

    // Steps 13-21, 23-28.
    switch (target->type()) {
#define SET_FROM_TYPED_ARRAY(T, N)                                          \
  case Scalar::N:                                                           \
    if (!SetFromTypedArray<T>(target, srcTypedArray, offset)) return false; \
    break;
      JS_FOR_EACH_TYPED_ARRAY(SET_FROM_TYPED_ARRAY)
#undef SET_FROM_TYPED_ARRAY
      default:
        MOZ_CRASH("Unsupported TypedArray type");
    }
  } else {
    // Remaining steps of 22.2.3.23.1.

    // Step 10.
    // We can't reorder this step because side-effects in step 16 can
    // detach the underlying array buffer from the typed array.
    uint32_t targetLength = target->length();

<<<<<<< HEAD
    // Step 16.
    uint32_t srcLength;
    if (!GetLengthProperty(cx, src, &srcLength)) {
      return false;
    }

    // Step 17 (Split into two checks to provide better error messages).
    if (targetOffset > targetLength) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_BAD_INDEX);
      return false;
    }

    // Step 17 (Cont'd).
    uint32_t offset = uint32_t(targetOffset);
    if (srcLength > targetLength - offset) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_BAD_ARRAY_LENGTH);
      return false;
    }
||||||| merged common ancestors
        // Step 16.
        uint32_t srcLength;
        if (!GetLengthProperty(cx, src, &srcLength)) {
            return false;
        }
=======
    // Step 16.
    uint32_t srcLength;
    if (!GetLengthProperty(cx, src, &srcLength)) {
      return false;
    }

    // Step 17 (Split into two checks to provide better error messages).
    if (targetOffset > targetLength) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_BAD_INDEX);
      return false;
    }

    // Step 17 (Cont'd).
    uint32_t offset = uint32_t(targetOffset);
    if (srcLength > targetLength - offset) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_SOURCE_ARRAY_TOO_LONG);
      return false;
    }
>>>>>>> upstream-releases

    // Steps 11-14, 18-21.
    if (srcLength > 0) {
      // GetLengthProperty in step 16 can lead to the execution of user
      // code which may detach the buffer. Handle this case here to
      // ensure SetFromNonTypedArray is never called with a detached
      // buffer. We still need to execute steps 21.a-b for their
      // possible side-effects.
      if (target->hasDetachedBuffer()) {
        // Steps 21.a-b.
        RootedValue v(cx);
        if (!GetElement(cx, src, src, 0, &v)) {
          return false;
        }

        double unused;
        if (!ToNumber(cx, v, &unused)) {
          return false;
        }

        // Step 21.c.
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_TYPED_ARRAY_DETACHED);
        return false;
      }

      switch (target->type()) {
#define SET_FROM_NON_TYPED_ARRAY(T, N)                                \
  case Scalar::N:                                                     \
    if (!SetFromNonTypedArray<T>(cx, target, src, srcLength, offset)) \
      return false;                                                   \
    break;
        JS_FOR_EACH_TYPED_ARRAY(SET_FROM_NON_TYPED_ARRAY)
#undef SET_FROM_NON_TYPED_ARRAY
        default:
          MOZ_CRASH("Unsupported TypedArray type");
      }

      // Step 21.c.
      // SetFromNonTypedArray doesn't throw when the array buffer gets
      // detached.
      if (target->hasDetachedBuffer()) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_TYPED_ARRAY_DETACHED);
        return false;
      }
    }
  }

  // Step 29/22.
  args.rval().setUndefined();
  return true;
}

<<<<<<< HEAD
/* static */ bool TypedArrayObject::set(JSContext* cx, unsigned argc,
                                        Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  return CallNonGenericMethod<TypedArrayObject::is, TypedArrayObject::set_impl>(
      cx, args);
||||||| merged common ancestors
/* static */ bool
TypedArrayObject::set(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
    return CallNonGenericMethod<TypedArrayObject::is, TypedArrayObject::set_impl>(cx, args);
=======
/* static */
bool TypedArrayObject::set(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  return CallNonGenericMethod<TypedArrayObject::is, TypedArrayObject::set_impl>(
      cx, args);
>>>>>>> upstream-releases
}

/* static */ const JSFunctionSpec TypedArrayObject::protoFunctions[] = {
    JS_SELF_HOSTED_FN("subarray", "TypedArraySubarray", 2, 0),
#if 0 /* disabled until perf-testing is completed */
    JS_SELF_HOSTED_FN("set", "TypedArraySet", 2, 0),
#else
    JS_FN("set", TypedArrayObject::set, 1, 0),
#endif
    JS_SELF_HOSTED_FN("copyWithin", "TypedArrayCopyWithin", 3, 0),
    JS_SELF_HOSTED_FN("every", "TypedArrayEvery", 1, 0),
    JS_SELF_HOSTED_FN("fill", "TypedArrayFill", 3, 0),
    JS_SELF_HOSTED_FN("filter", "TypedArrayFilter", 1, 0),
    JS_SELF_HOSTED_FN("find", "TypedArrayFind", 1, 0),
    JS_SELF_HOSTED_FN("findIndex", "TypedArrayFindIndex", 1, 0),
    JS_SELF_HOSTED_FN("forEach", "TypedArrayForEach", 1, 0),
    JS_SELF_HOSTED_FN("indexOf", "TypedArrayIndexOf", 2, 0),
    JS_SELF_HOSTED_FN("join", "TypedArrayJoin", 1, 0),
    JS_SELF_HOSTED_FN("lastIndexOf", "TypedArrayLastIndexOf", 1, 0),
    JS_SELF_HOSTED_FN("map", "TypedArrayMap", 1, 0),
    JS_SELF_HOSTED_FN("reduce", "TypedArrayReduce", 1, 0),
    JS_SELF_HOSTED_FN("reduceRight", "TypedArrayReduceRight", 1, 0),
    JS_SELF_HOSTED_FN("reverse", "TypedArrayReverse", 0, 0),
    JS_SELF_HOSTED_FN("slice", "TypedArraySlice", 2, 0),
    JS_SELF_HOSTED_FN("some", "TypedArraySome", 1, 0),
    JS_SELF_HOSTED_FN("sort", "TypedArraySort", 1, 0),
    JS_SELF_HOSTED_FN("entries", "TypedArrayEntries", 0, 0),
    JS_SELF_HOSTED_FN("keys", "TypedArrayKeys", 0, 0),
    JS_SELF_HOSTED_FN("values", "$TypedArrayValues", 0, 0),
    JS_SELF_HOSTED_SYM_FN(iterator, "$TypedArrayValues", 0, 0),
    JS_SELF_HOSTED_FN("includes", "TypedArrayIncludes", 2, 0),
    JS_SELF_HOSTED_FN("toString", "ArrayToString", 0, 0),
    JS_SELF_HOSTED_FN("toLocaleString", "TypedArrayToLocaleString", 2, 0),
    JS_FS_END};

/* static */ const JSFunctionSpec TypedArrayObject::staticFunctions[] = {
    JS_SELF_HOSTED_FN("from", "TypedArrayStaticFrom", 3, 0),
    JS_SELF_HOSTED_FN("of", "TypedArrayStaticOf", 0, 0), JS_FS_END};

<<<<<<< HEAD
/* static */ const JSPropertySpec TypedArrayObject::staticProperties[] = {
    JS_SELF_HOSTED_SYM_GET(species, "TypedArraySpecies", 0), JS_PS_END};
||||||| merged common ancestors
/* static */ const JSPropertySpec
TypedArrayObject::staticProperties[] = {
    JS_SELF_HOSTED_SYM_GET(species, "TypedArraySpecies", 0),
    JS_PS_END
};
=======
/* static */ const JSPropertySpec TypedArrayObject::staticProperties[] = {
    JS_SELF_HOSTED_SYM_GET(species, "$TypedArraySpecies", 0), JS_PS_END};
>>>>>>> upstream-releases

static JSObject* CreateSharedTypedArrayPrototype(JSContext* cx,
                                                 JSProtoKey key) {
  return GlobalObject::createBlankPrototype(
      cx, cx->global(), &TypedArrayObject::sharedTypedArrayPrototypeClass);
}

static const ClassSpec TypedArrayObjectSharedTypedArrayPrototypeClassSpec = {
    GenericCreateConstructor<TypedArrayConstructor, 0, gc::AllocKind::FUNCTION>,
    CreateSharedTypedArrayPrototype,
    TypedArrayObject::staticFunctions,
    TypedArrayObject::staticProperties,
    TypedArrayObject::protoFunctions,
    TypedArrayObject::protoAccessors,
    nullptr,
    ClassSpec::DontDefineConstructor};

<<<<<<< HEAD
/* static */ const Class TypedArrayObject::sharedTypedArrayPrototypeClass = {
    "TypedArrayPrototype", JSCLASS_HAS_CACHED_PROTO(JSProto_TypedArray),
    JS_NULL_CLASS_OPS, &TypedArrayObjectSharedTypedArrayPrototypeClassSpec};

// this default implementation is only valid for integer types
// less than 32-bits in size.
template <typename NativeType>
Value TypedArrayObjectTemplate<NativeType>::getIndexValue(
    TypedArrayObject* tarray, uint32_t index) {
  static_assert(sizeof(NativeType) < 4,
                "this method must only handle NativeType values that are "
                "always exact int32_t values");

  return Int32Value(getIndex(tarray, index));
}
||||||| merged common ancestors
/* static */ const Class
TypedArrayObject::sharedTypedArrayPrototypeClass = {
    "TypedArrayPrototype",
    JSCLASS_HAS_CACHED_PROTO(JSProto_TypedArray),
    JS_NULL_CLASS_OPS,
    &TypedArrayObjectSharedTypedArrayPrototypeClassSpec
};

// this default implementation is only valid for integer types
// less than 32-bits in size.
template<typename NativeType>
Value
TypedArrayObjectTemplate<NativeType>::getIndexValue(TypedArrayObject* tarray, uint32_t index)
{
    static_assert(sizeof(NativeType) < 4,
                  "this method must only handle NativeType values that are "
                  "always exact int32_t values");

    return Int32Value(getIndex(tarray, index));
}
=======
/* static */ const Class TypedArrayObject::sharedTypedArrayPrototypeClass = {
    "TypedArrayPrototype", JSCLASS_HAS_CACHED_PROTO(JSProto_TypedArray),
    JS_NULL_CLASS_OPS, &TypedArrayObjectSharedTypedArrayPrototypeClassSpec};
>>>>>>> upstream-releases

namespace {

<<<<<<< HEAD
// and we need to specialize for 32-bit integers and floats
template <>
Value TypedArrayObjectTemplate<int32_t>::getIndexValue(TypedArrayObject* tarray,
                                                       uint32_t index) {
  return Int32Value(getIndex(tarray, index));
||||||| merged common ancestors
// and we need to specialize for 32-bit integers and floats
template<>
Value
TypedArrayObjectTemplate<int32_t>::getIndexValue(TypedArrayObject* tarray, uint32_t index)
{
    return Int32Value(getIndex(tarray, index));
=======
// This default implementation is only valid for integer types less
// than 32-bits in size.
template <typename NativeType>
bool TypedArrayObjectTemplate<NativeType>::getElementPure(
    TypedArrayObject* tarray, uint32_t index, Value* vp) {
  static_assert(sizeof(NativeType) < 4,
                "this method must only handle NativeType values that are "
                "always exact int32_t values");

  *vp = Int32Value(getIndex(tarray, index));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <>
Value TypedArrayObjectTemplate<uint32_t>::getIndexValue(
    TypedArrayObject* tarray, uint32_t index) {
  uint32_t val = getIndex(tarray, index);
  return NumberValue(val);
||||||| merged common ancestors
template<>
Value
TypedArrayObjectTemplate<uint32_t>::getIndexValue(TypedArrayObject* tarray, uint32_t index)
{
    uint32_t val = getIndex(tarray, index);
    return NumberValue(val);
=======
// We need to specialize for floats and other integer types.
template <>
bool TypedArrayObjectTemplate<int32_t>::getElementPure(TypedArrayObject* tarray,
                                                       uint32_t index,
                                                       Value* vp) {
  *vp = Int32Value(getIndex(tarray, index));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <>
Value TypedArrayObjectTemplate<float>::getIndexValue(TypedArrayObject* tarray,
                                                     uint32_t index) {
  float val = getIndex(tarray, index);
  double dval = val;

  /*
   * Doubles in typed arrays could be typed-punned arrays of integers. This
   * could allow user code to break the engine-wide invariant that only
   * canonical nans are stored into jsvals, which means user code could
   * confuse the engine into interpreting a double-typed jsval as an
   * object-typed jsval.
   *
   * This could be removed for platforms/compilers known to convert a 32-bit
   * non-canonical nan to a 64-bit canonical nan.
   */
  return DoubleValue(CanonicalizeNaN(dval));
||||||| merged common ancestors
template<>
Value
TypedArrayObjectTemplate<float>::getIndexValue(TypedArrayObject* tarray, uint32_t index)
{
    float val = getIndex(tarray, index);
    double dval = val;

    /*
     * Doubles in typed arrays could be typed-punned arrays of integers. This
     * could allow user code to break the engine-wide invariant that only
     * canonical nans are stored into jsvals, which means user code could
     * confuse the engine into interpreting a double-typed jsval as an
     * object-typed jsval.
     *
     * This could be removed for platforms/compilers known to convert a 32-bit
     * non-canonical nan to a 64-bit canonical nan.
     */
    return DoubleValue(CanonicalizeNaN(dval));
=======
template <>
bool TypedArrayObjectTemplate<uint32_t>::getElementPure(
    TypedArrayObject* tarray, uint32_t index, Value* vp) {
  uint32_t val = getIndex(tarray, index);
  *vp = NumberValue(val);
  return true;
}

template <>
bool TypedArrayObjectTemplate<float>::getElementPure(TypedArrayObject* tarray,
                                                     uint32_t index,
                                                     Value* vp) {
  float val = getIndex(tarray, index);
  double dval = val;

  /*
   * Doubles in typed arrays could be typed-punned arrays of integers. This
   * could allow user code to break the engine-wide invariant that only
   * canonical nans are stored into jsvals, which means user code could
   * confuse the engine into interpreting a double-typed jsval as an
   * object-typed jsval.
   *
   * This could be removed for platforms/compilers known to convert a 32-bit
   * non-canonical nan to a 64-bit canonical nan.
   */
  *vp = JS::CanonicalizedDoubleValue(dval);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <>
Value TypedArrayObjectTemplate<double>::getIndexValue(TypedArrayObject* tarray,
                                                      uint32_t index) {
  double val = getIndex(tarray, index);

  /*
   * Doubles in typed arrays could be typed-punned arrays of integers. This
   * could allow user code to break the engine-wide invariant that only
   * canonical nans are stored into jsvals, which means user code could
   * confuse the engine into interpreting a double-typed jsval as an
   * object-typed jsval.
   */
  return DoubleValue(CanonicalizeNaN(val));
||||||| merged common ancestors
template<>
Value
TypedArrayObjectTemplate<double>::getIndexValue(TypedArrayObject* tarray, uint32_t index)
{
    double val = getIndex(tarray, index);

    /*
     * Doubles in typed arrays could be typed-punned arrays of integers. This
     * could allow user code to break the engine-wide invariant that only
     * canonical nans are stored into jsvals, which means user code could
     * confuse the engine into interpreting a double-typed jsval as an
     * object-typed jsval.
     */
    return DoubleValue(CanonicalizeNaN(val));
=======
template <>
bool TypedArrayObjectTemplate<double>::getElementPure(TypedArrayObject* tarray,
                                                      uint32_t index,
                                                      Value* vp) {
  double val = getIndex(tarray, index);

  /*
   * Doubles in typed arrays could be typed-punned arrays of integers. This
   * could allow user code to break the engine-wide invariant that only
   * canonical nans are stored into jsvals, which means user code could
   * confuse the engine into interpreting a double-typed jsval as an
   * object-typed jsval.
   */
  *vp = JS::CanonicalizedDoubleValue(val);
  return true;
}

template <>
bool TypedArrayObjectTemplate<int64_t>::getElementPure(TypedArrayObject* tarray,
                                                       uint32_t index,
                                                       Value* vp) {
  return false;
>>>>>>> upstream-releases
}

template <>
bool TypedArrayObjectTemplate<uint64_t>::getElementPure(
    TypedArrayObject* tarray, uint32_t index, Value* vp) {
  return false;
}
} /* anonymous namespace */

<<<<<<< HEAD
Value TypedArrayObject::getElement(uint32_t index) {
  switch (type()) {
    case Scalar::Int8:
      return Int8Array::getIndexValue(this, index);
    case Scalar::Uint8:
      return Uint8Array::getIndexValue(this, index);
    case Scalar::Int16:
      return Int16Array::getIndexValue(this, index);
    case Scalar::Uint16:
      return Uint16Array::getIndexValue(this, index);
    case Scalar::Int32:
      return Int32Array::getIndexValue(this, index);
    case Scalar::Uint32:
      return Uint32Array::getIndexValue(this, index);
    case Scalar::Float32:
      return Float32Array::getIndexValue(this, index);
    case Scalar::Float64:
      return Float64Array::getIndexValue(this, index);
    case Scalar::Uint8Clamped:
      return Uint8ClampedArray::getIndexValue(this, index);
    case Scalar::Int64:
    case Scalar::MaxTypedArrayViewType:
      break;
  }

  MOZ_CRASH("Unknown TypedArray type");
||||||| merged common ancestors
Value
TypedArrayObject::getElement(uint32_t index)
{
    switch (type()) {
      case Scalar::Int8:
        return Int8Array::getIndexValue(this, index);
      case Scalar::Uint8:
        return Uint8Array::getIndexValue(this, index);
      case Scalar::Int16:
        return Int16Array::getIndexValue(this, index);
      case Scalar::Uint16:
        return Uint16Array::getIndexValue(this, index);
      case Scalar::Int32:
        return Int32Array::getIndexValue(this, index);
      case Scalar::Uint32:
        return Uint32Array::getIndexValue(this, index);
      case Scalar::Float32:
        return Float32Array::getIndexValue(this, index);
      case Scalar::Float64:
        return Float64Array::getIndexValue(this, index);
      case Scalar::Uint8Clamped:
        return Uint8ClampedArray::getIndexValue(this, index);
      case Scalar::Int64:
      case Scalar::MaxTypedArrayViewType:
        break;
    }

    MOZ_CRASH("Unknown TypedArray type");
=======
namespace {

template <typename NativeType>
bool TypedArrayObjectTemplate<NativeType>::getElement(JSContext* cx,
                                                      TypedArrayObject* tarray,
                                                      uint32_t index,
                                                      MutableHandleValue val) {
  MOZ_ALWAYS_TRUE(getElementPure(tarray, index, val.address()));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void TypedArrayObject::setElement(TypedArrayObject& obj, uint32_t index,
                                  double d) {
  MOZ_ASSERT(index < obj.length());
||||||| merged common ancestors
void
TypedArrayObject::setElement(TypedArrayObject& obj, uint32_t index, double d)
{
    MOZ_ASSERT(index < obj.length());
=======
template <>
bool TypedArrayObjectTemplate<int64_t>::getElement(JSContext* cx,
                                                   TypedArrayObject* tarray,
                                                   uint32_t index,
                                                   MutableHandleValue val) {
  int64_t n = getIndex(tarray, index);
  BigInt* res = BigInt::createFromInt64(cx, n);
  if (!res) {
    return false;
  }
  val.setBigInt(res);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef JS_MORE_DETERMINISTIC
  // See the comment in ElementSpecific::doubleToNative.
  d = JS::CanonicalizeNaN(d);
#endif
||||||| merged common ancestors
#ifdef JS_MORE_DETERMINISTIC
    // See the comment in ElementSpecific::doubleToNative.
    d = JS::CanonicalizeNaN(d);
#endif
=======
template <>
bool TypedArrayObjectTemplate<uint64_t>::getElement(JSContext* cx,
                                                    TypedArrayObject* tarray,
                                                    uint32_t index,
                                                    MutableHandleValue val) {
  uint64_t n = getIndex(tarray, index);
  BigInt* res = BigInt::createFromUint64(cx, n);
  if (!res) {
    return false;
  }
  val.setBigInt(res);
  return true;
}
} /* anonymous namespace */
>>>>>>> upstream-releases

<<<<<<< HEAD
  switch (obj.type()) {
    case Scalar::Int8:
      Int8Array::setIndexValue(obj, index, d);
      return;
    case Scalar::Uint8:
      Uint8Array::setIndexValue(obj, index, d);
      return;
    case Scalar::Uint8Clamped:
      Uint8ClampedArray::setIndexValue(obj, index, d);
      return;
    case Scalar::Int16:
      Int16Array::setIndexValue(obj, index, d);
      return;
    case Scalar::Uint16:
      Uint16Array::setIndexValue(obj, index, d);
      return;
    case Scalar::Int32:
      Int32Array::setIndexValue(obj, index, d);
      return;
    case Scalar::Uint32:
      Uint32Array::setIndexValue(obj, index, d);
      return;
    case Scalar::Float32:
      Float32Array::setIndexValue(obj, index, d);
      return;
    case Scalar::Float64:
      Float64Array::setIndexValue(obj, index, d);
      return;
    case Scalar::Int64:
    case Scalar::MaxTypedArrayViewType:
      break;
  }

  MOZ_CRASH("Unknown TypedArray type");
||||||| merged common ancestors
    switch (obj.type()) {
      case Scalar::Int8:
        Int8Array::setIndexValue(obj, index, d);
        return;
      case Scalar::Uint8:
        Uint8Array::setIndexValue(obj, index, d);
        return;
      case Scalar::Uint8Clamped:
        Uint8ClampedArray::setIndexValue(obj, index, d);
        return;
      case Scalar::Int16:
        Int16Array::setIndexValue(obj, index, d);
        return;
      case Scalar::Uint16:
        Uint16Array::setIndexValue(obj, index, d);
        return;
      case Scalar::Int32:
        Int32Array::setIndexValue(obj, index, d);
        return;
      case Scalar::Uint32:
        Uint32Array::setIndexValue(obj, index, d);
        return;
      case Scalar::Float32:
        Float32Array::setIndexValue(obj, index, d);
        return;
      case Scalar::Float64:
        Float64Array::setIndexValue(obj, index, d);
        return;
      case Scalar::Int64:
      case Scalar::MaxTypedArrayViewType:
        break;
    }

    MOZ_CRASH("Unknown TypedArray type");
=======
namespace js {

template <>
bool TypedArrayObject::getElement<CanGC>(JSContext* cx, uint32_t index,
                                         MutableHandleValue val) {
  switch (type()) {
#define GET_ELEMENT(T, N) \
  case Scalar::N:         \
    return N##Array::getElement(cx, this, index, val);
    JS_FOR_EACH_TYPED_ARRAY(GET_ELEMENT)
#undef GET_ELEMENT
    case Scalar::MaxTypedArrayViewType:
    case Scalar::Int64:
      break;
  }

  MOZ_CRASH("Unknown TypedArray type");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void TypedArrayObject::getElements(Value* vp) {
  uint32_t length = this->length();
  MOZ_ASSERT_IF(length > 0, !hasDetachedBuffer());

  switch (type()) {
#define GET_ELEMENTS(T, N)                      \
  case Scalar::N:                               \
    for (uint32_t i = 0; i < length; ++i, ++vp) \
      *vp = N##Array::getIndexValue(this, i);   \
    break;
    JS_FOR_EACH_TYPED_ARRAY(GET_ELEMENTS)
||||||| merged common ancestors
void
TypedArrayObject::getElements(Value* vp)
{
    uint32_t length = this->length();
    MOZ_ASSERT_IF(length > 0, !hasDetachedBuffer());

    switch (type()) {
#define GET_ELEMENTS(T, N) \
      case Scalar::N: \
        for (uint32_t i = 0; i < length; ++i, ++vp) \
            *vp = N##Array::getIndexValue(this, i); \
        break;
JS_FOR_EACH_TYPED_ARRAY(GET_ELEMENTS)
=======
template <>
bool TypedArrayObject::getElement<NoGC>(
    JSContext* cx, uint32_t index,
    typename MaybeRooted<Value, NoGC>::MutableHandleType vp) {
  return getElementPure(index, vp.address());
}

}  // namespace js

bool TypedArrayObject::getElementPure(uint32_t index, Value* vp) {
  switch (type()) {
#define GET_ELEMENT_PURE(T, N) \
  case Scalar::N:              \
    return N##Array::getElementPure(this, index, vp);
    JS_FOR_EACH_TYPED_ARRAY(GET_ELEMENT_PURE)
#undef GET_ELEMENT
    case Scalar::MaxTypedArrayViewType:
    case Scalar::Int64:
      break;
  }

  MOZ_CRASH("Unknown TypedArray type");
}

/* static */
bool TypedArrayObject::getElements(JSContext* cx,
                                   Handle<TypedArrayObject*> tarray,
                                   Value* vp) {
  uint32_t length = tarray->length();
  MOZ_ASSERT_IF(length > 0, !tarray->hasDetachedBuffer());

  switch (tarray->type()) {
#define GET_ELEMENTS(T, N)                                                     \
  case Scalar::N:                                                              \
    for (uint32_t i = 0; i < length; ++i, ++vp) {                              \
      if (!N##Array::getElement(cx, tarray, i,                                 \
                                MutableHandleValue::fromMarkedLocation(vp))) { \
        return false;                                                          \
      }                                                                        \
    }                                                                          \
    return true;
    JS_FOR_EACH_TYPED_ARRAY(GET_ELEMENTS)
>>>>>>> upstream-releases
#undef GET_ELEMENTS
<<<<<<< HEAD
    default:
      MOZ_CRASH("Unknown TypedArray type");
  }
||||||| merged common ancestors
      default:
        MOZ_CRASH("Unknown TypedArray type");
    }
=======
    case Scalar::MaxTypedArrayViewType:
    case Scalar::Int64:
      break;
  }

  MOZ_CRASH("Unknown TypedArray type");
>>>>>>> upstream-releases
}

/***
 *** JS impl
 ***/

/*
 * TypedArrayObject boilerplate
 */

<<<<<<< HEAD
#define IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Name, NativeType)                \
  JS_FRIEND_API JSObject* JS_New##Name##Array(JSContext* cx,                 \
                                              uint32_t nelements) {          \
    return TypedArrayObjectTemplate<NativeType>::fromLength(cx, nelements);  \
  }                                                                          \
  JS_FRIEND_API JSObject* JS_New##Name##ArrayFromArray(JSContext* cx,        \
                                                       HandleObject other) { \
    return TypedArrayObjectTemplate<NativeType>::fromArray(cx, other);       \
  }                                                                          \
  JS_FRIEND_API JSObject* JS_New##Name##ArrayWithBuffer(                     \
      JSContext* cx, HandleObject arrayBuffer, uint32_t byteOffset,          \
      int32_t length) {                                                      \
    return TypedArrayObjectTemplate<NativeType>::fromBuffer(                 \
        cx, arrayBuffer, byteOffset, length);                                \
  }                                                                          \
  JS_FRIEND_API bool JS_Is##Name##Array(JSObject* obj) {                     \
    if (!(obj = CheckedUnwrap(obj))) return false;                           \
    const Class* clasp = obj->getClass();                                    \
    return clasp == TypedArrayObjectTemplate<NativeType>::instanceClass();   \
  }                                                                          \
  JS_FRIEND_API JSObject* js::Unwrap##Name##Array(JSObject* obj) {           \
    obj = CheckedUnwrap(obj);                                                \
    if (!obj) return nullptr;                                                \
    const Class* clasp = obj->getClass();                                    \
    if (clasp == TypedArrayObjectTemplate<NativeType>::instanceClass())      \
      return obj;                                                            \
    return nullptr;                                                          \
  }                                                                          \
  const js::Class* const js::detail::Name##ArrayClassPtr =                   \
      &js::TypedArrayObject::classes                                         \
          [TypedArrayObjectTemplate<NativeType>::ArrayTypeID()];

IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Int8, int8_t)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Uint8, uint8_t)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Uint8Clamped, uint8_clamped)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Int16, int16_t)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Uint16, uint16_t)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Int32, int32_t)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Uint32, uint32_t)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Float32, float)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Float64, double)

#define IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Name, ExternalType, InternalType) \
  JS_FRIEND_API JSObject* JS_GetObjectAs##Name##Array(                         \
      JSObject* obj, uint32_t* length, bool* isShared, ExternalType** data) {  \
    if (!(obj = CheckedUnwrap(obj))) return nullptr;                           \
                                                                               \
    const Class* clasp = obj->getClass();                                      \
    if (clasp != TypedArrayObjectTemplate<InternalType>::instanceClass())      \
      return nullptr;                                                          \
                                                                               \
    TypedArrayObject* tarr = &obj->as<TypedArrayObject>();                     \
    *length = tarr->length();                                                  \
    *isShared = tarr->isSharedMemory();                                        \
    *data = static_cast<ExternalType*>(tarr->dataPointerEither().unwrap(       \
        /*safe - caller sees isShared flag*/));                                \
                                                                               \
    return obj;                                                                \
  }

IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Int8, int8_t, int8_t)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Uint8, uint8_t, uint8_t)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Uint8Clamped, uint8_t, uint8_clamped)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Int16, int16_t, int16_t)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Uint16, uint16_t, uint16_t)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Int32, int32_t, int32_t)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Uint32, uint32_t, uint32_t)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Float32, float, float)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Float64, double, double)

||||||| merged common ancestors
#define IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Name,NativeType)                                    \
  JS_FRIEND_API(JSObject*) JS_New ## Name ## Array(JSContext* cx, uint32_t nelements)           \
  {                                                                                             \
      return TypedArrayObjectTemplate<NativeType>::fromLength(cx, nelements);                   \
  }                                                                                             \
  JS_FRIEND_API(JSObject*) JS_New ## Name ## ArrayFromArray(JSContext* cx, HandleObject other)  \
  {                                                                                             \
      return TypedArrayObjectTemplate<NativeType>::fromArray(cx, other);                        \
  }                                                                                             \
  JS_FRIEND_API(JSObject*) JS_New ## Name ## ArrayWithBuffer(JSContext* cx,                     \
                               HandleObject arrayBuffer, uint32_t byteOffset, int32_t length)   \
  {                                                                                             \
      return TypedArrayObjectTemplate<NativeType>::fromBuffer(cx, arrayBuffer, byteOffset,      \
                                                              length);                          \
  }                                                                                             \
  JS_FRIEND_API(bool) JS_Is ## Name ## Array(JSObject* obj)                                     \
  {                                                                                             \
      if (!(obj = CheckedUnwrap(obj)))                                                          \
          return false;                                                                         \
      const Class* clasp = obj->getClass();                                                     \
      return clasp == TypedArrayObjectTemplate<NativeType>::instanceClass();                    \
  }                                                                                             \
  JS_FRIEND_API(JSObject*) js::Unwrap ## Name ## Array(JSObject* obj)                           \
  {                                                                                             \
      obj = CheckedUnwrap(obj);                                                                 \
      if (!obj)                                                                                 \
          return nullptr;                                                                       \
      const Class* clasp = obj->getClass();                                                     \
      if (clasp == TypedArrayObjectTemplate<NativeType>::instanceClass())                       \
          return obj;                                                                           \
      return nullptr;                                                                           \
  }                                                                                             \
  const js::Class* const js::detail::Name ## ArrayClassPtr =                                    \
      &js::TypedArrayObject::classes[TypedArrayObjectTemplate<NativeType>::ArrayTypeID()];

IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Int8, int8_t)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Uint8, uint8_t)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Uint8Clamped, uint8_clamped)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Int16, int16_t)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Uint16, uint16_t)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Int32, int32_t)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Uint32, uint32_t)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Float32, float)
IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(Float64, double)

#define IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Name, ExternalType, InternalType)              \
  JS_FRIEND_API(JSObject*) JS_GetObjectAs ## Name ## Array(JSObject* obj,                  \
                                                            uint32_t* length,               \
                                                            bool* isShared,                 \
                                                            ExternalType** data)            \
  {                                                                                         \
      if (!(obj = CheckedUnwrap(obj)))                                                      \
          return nullptr;                                                                   \
                                                                                            \
      const Class* clasp = obj->getClass();                                                 \
      if (clasp != TypedArrayObjectTemplate<InternalType>::instanceClass())                 \
          return nullptr;                                                                   \
                                                                                            \
      TypedArrayObject* tarr = &obj->as<TypedArrayObject>();                                \
      *length = tarr->length();                                                             \
      *isShared = tarr->isSharedMemory();                                                         \
      *data = static_cast<ExternalType*>(tarr->dataPointerEither().unwrap(/*safe - caller sees isShared flag*/)); \
                                                                                            \
      return obj;                                                                           \
  }

IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Int8, int8_t, int8_t)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Uint8, uint8_t, uint8_t)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Uint8Clamped, uint8_t, uint8_clamped)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Int16, int16_t, int16_t)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Uint16, uint16_t, uint16_t)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Int32, int32_t, int32_t)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Uint32, uint32_t, uint32_t)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Float32, float, float)
IMPL_TYPED_ARRAY_COMBINED_UNWRAPPERS(Float64, double, double)

=======
>>>>>>> upstream-releases
static const ClassOps TypedArrayClassOps = {
    nullptr,                      /* addProperty */
    nullptr,                      /* delProperty */
    nullptr,                      /* enumerate   */
    nullptr,                      /* newEnumerate */
    nullptr,                      /* resolve     */
    nullptr,                      /* mayResolve  */
    TypedArrayObject::finalize,   /* finalize    */
    nullptr,                      /* call        */
    nullptr,                      /* hasInstance */
    nullptr,                      /* construct   */
    ArrayBufferViewObject::trace, /* trace  */
};

static const ClassExtension TypedArrayClassExtension = {
    TypedArrayObject::objectMoved,
};

<<<<<<< HEAD
#define IMPL_TYPED_ARRAY_PROPERTIES(_type)                            \
  {                                                                   \
    JS_INT32_PS("BYTES_PER_ELEMENT", _type##Array::BYTES_PER_ELEMENT, \
                JSPROP_READONLY | JSPROP_PERMANENT),                  \
        JS_PS_END                                                     \
  }

static const JSPropertySpec
    static_prototype_properties[Scalar::MaxTypedArrayViewType][2] = {
        IMPL_TYPED_ARRAY_PROPERTIES(Int8),
        IMPL_TYPED_ARRAY_PROPERTIES(Uint8),
        IMPL_TYPED_ARRAY_PROPERTIES(Int16),
        IMPL_TYPED_ARRAY_PROPERTIES(Uint16),
        IMPL_TYPED_ARRAY_PROPERTIES(Int32),
        IMPL_TYPED_ARRAY_PROPERTIES(Uint32),
        IMPL_TYPED_ARRAY_PROPERTIES(Float32),
        IMPL_TYPED_ARRAY_PROPERTIES(Float64),
        IMPL_TYPED_ARRAY_PROPERTIES(Uint8Clamped)};

#define IMPL_TYPED_ARRAY_CLASS_SPEC(_type)                                   \
  {                                                                          \
    _type##Array::createConstructor, _type##Array::createPrototype, nullptr, \
        static_prototype_properties[Scalar::Type::_type], nullptr,           \
        static_prototype_properties[Scalar::Type::_type], nullptr,           \
        JSProto_TypedArray                                                   \
  }
||||||| merged common ancestors
#define IMPL_TYPED_ARRAY_PROPERTIES(_type)                                     \
{                                                                              \
JS_INT32_PS("BYTES_PER_ELEMENT", _type##Array::BYTES_PER_ELEMENT,              \
            JSPROP_READONLY | JSPROP_PERMANENT),                               \
JS_PS_END                                                                      \
}

static const JSPropertySpec static_prototype_properties[Scalar::MaxTypedArrayViewType][2] = {
    IMPL_TYPED_ARRAY_PROPERTIES(Int8),
    IMPL_TYPED_ARRAY_PROPERTIES(Uint8),
    IMPL_TYPED_ARRAY_PROPERTIES(Int16),
    IMPL_TYPED_ARRAY_PROPERTIES(Uint16),
    IMPL_TYPED_ARRAY_PROPERTIES(Int32),
    IMPL_TYPED_ARRAY_PROPERTIES(Uint32),
    IMPL_TYPED_ARRAY_PROPERTIES(Float32),
    IMPL_TYPED_ARRAY_PROPERTIES(Float64),
    IMPL_TYPED_ARRAY_PROPERTIES(Uint8Clamped)
};

#define IMPL_TYPED_ARRAY_CLASS_SPEC(_type)                                     \
{                                                                              \
    _type##Array::createConstructor,                                           \
    _type##Array::createPrototype,                                             \
    nullptr,                                                                   \
    static_prototype_properties[Scalar::Type::_type],                          \
    nullptr,                                                                   \
    static_prototype_properties[Scalar::Type::_type],                          \
    nullptr,                                                                   \
    JSProto_TypedArray                                                         \
}

static const ClassSpec TypedArrayObjectClassSpecs[Scalar::MaxTypedArrayViewType] = {
    IMPL_TYPED_ARRAY_CLASS_SPEC(Int8),
    IMPL_TYPED_ARRAY_CLASS_SPEC(Uint8),
    IMPL_TYPED_ARRAY_CLASS_SPEC(Int16),
    IMPL_TYPED_ARRAY_CLASS_SPEC(Uint16),
    IMPL_TYPED_ARRAY_CLASS_SPEC(Int32),
    IMPL_TYPED_ARRAY_CLASS_SPEC(Uint32),
    IMPL_TYPED_ARRAY_CLASS_SPEC(Float32),
    IMPL_TYPED_ARRAY_CLASS_SPEC(Float64),
    IMPL_TYPED_ARRAY_CLASS_SPEC(Uint8Clamped)
};
=======
static const JSPropertySpec
    static_prototype_properties[Scalar::MaxTypedArrayViewType][2] = {
#define IMPL_TYPED_ARRAY_PROPERTIES(NativeType, Name)               \
  {JS_INT32_PS("BYTES_PER_ELEMENT", Name##Array::BYTES_PER_ELEMENT, \
               JSPROP_READONLY | JSPROP_PERMANENT),                 \
   JS_PS_END},

        JS_FOR_EACH_TYPED_ARRAY(IMPL_TYPED_ARRAY_PROPERTIES)
#undef IMPL_TYPED_ARRAY_PROPERTIES
};
>>>>>>> upstream-releases

<<<<<<< HEAD
static const ClassSpec
    TypedArrayObjectClassSpecs[Scalar::MaxTypedArrayViewType] = {
        IMPL_TYPED_ARRAY_CLASS_SPEC(Int8),
        IMPL_TYPED_ARRAY_CLASS_SPEC(Uint8),
        IMPL_TYPED_ARRAY_CLASS_SPEC(Int16),
        IMPL_TYPED_ARRAY_CLASS_SPEC(Uint16),
        IMPL_TYPED_ARRAY_CLASS_SPEC(Int32),
        IMPL_TYPED_ARRAY_CLASS_SPEC(Uint32),
        IMPL_TYPED_ARRAY_CLASS_SPEC(Float32),
        IMPL_TYPED_ARRAY_CLASS_SPEC(Float64),
        IMPL_TYPED_ARRAY_CLASS_SPEC(Uint8Clamped)};

#define IMPL_TYPED_ARRAY_CLASS(_type)                                        \
  {                                                                          \
#_type "Array",                                                          \
        JSCLASS_HAS_RESERVED_SLOTS(TypedArrayObject::RESERVED_SLOTS) |       \
            JSCLASS_HAS_PRIVATE |                                            \
            JSCLASS_HAS_CACHED_PROTO(JSProto_##_type##Array) |               \
            JSCLASS_DELAY_METADATA_BUILDER | JSCLASS_SKIP_NURSERY_FINALIZE | \
            JSCLASS_BACKGROUND_FINALIZE,                                     \
        &TypedArrayClassOps,                                                 \
        &TypedArrayObjectClassSpecs[Scalar::Type::_type],                    \
                                    &TypedArrayClassExtension                \
  }
||||||| merged common ancestors
#define IMPL_TYPED_ARRAY_CLASS(_type)                                          \
{                                                                              \
    #_type "Array",                                                            \
    JSCLASS_HAS_RESERVED_SLOTS(TypedArrayObject::RESERVED_SLOTS) |             \
    JSCLASS_HAS_PRIVATE |                                                      \
    JSCLASS_HAS_CACHED_PROTO(JSProto_##_type##Array) |                         \
    JSCLASS_DELAY_METADATA_BUILDER |                                           \
    JSCLASS_SKIP_NURSERY_FINALIZE |                                            \
    JSCLASS_BACKGROUND_FINALIZE,                                               \
    &TypedArrayClassOps,                                                       \
    &TypedArrayObjectClassSpecs[Scalar::Type::_type],                          \
    &TypedArrayClassExtension                                                  \
}
=======
static const ClassSpec
    TypedArrayObjectClassSpecs[Scalar::MaxTypedArrayViewType] = {
#define IMPL_TYPED_ARRAY_CLASS_SPEC(NativeType, Name) \
  {Name##Array::createConstructor,                    \
   Name##Array::createPrototype,                      \
   nullptr,                                           \
   static_prototype_properties[Scalar::Type::Name],   \
   nullptr,                                           \
   static_prototype_properties[Scalar::Type::Name],   \
   nullptr,                                           \
   JSProto_TypedArray},

        JS_FOR_EACH_TYPED_ARRAY(IMPL_TYPED_ARRAY_CLASS_SPEC)
#undef IMPL_TYPED_ARRAY_CLASS_SPEC
};
>>>>>>> upstream-releases

const Class TypedArrayObject::classes[Scalar::MaxTypedArrayViewType] = {
<<<<<<< HEAD
    IMPL_TYPED_ARRAY_CLASS(Int8),        IMPL_TYPED_ARRAY_CLASS(Uint8),
    IMPL_TYPED_ARRAY_CLASS(Int16),       IMPL_TYPED_ARRAY_CLASS(Uint16),
    IMPL_TYPED_ARRAY_CLASS(Int32),       IMPL_TYPED_ARRAY_CLASS(Uint32),
    IMPL_TYPED_ARRAY_CLASS(Float32),     IMPL_TYPED_ARRAY_CLASS(Float64),
    IMPL_TYPED_ARRAY_CLASS(Uint8Clamped)};
||||||| merged common ancestors
    IMPL_TYPED_ARRAY_CLASS(Int8),
    IMPL_TYPED_ARRAY_CLASS(Uint8),
    IMPL_TYPED_ARRAY_CLASS(Int16),
    IMPL_TYPED_ARRAY_CLASS(Uint16),
    IMPL_TYPED_ARRAY_CLASS(Int32),
    IMPL_TYPED_ARRAY_CLASS(Uint32),
    IMPL_TYPED_ARRAY_CLASS(Float32),
    IMPL_TYPED_ARRAY_CLASS(Float64),
    IMPL_TYPED_ARRAY_CLASS(Uint8Clamped)
};
=======
#define IMPL_TYPED_ARRAY_CLASS(NativeType, Name)                               \
  {#Name "Array",                                                              \
   JSCLASS_HAS_RESERVED_SLOTS(TypedArrayObject::RESERVED_SLOTS) |              \
       JSCLASS_HAS_PRIVATE | JSCLASS_HAS_CACHED_PROTO(JSProto_##Name##Array) | \
       JSCLASS_DELAY_METADATA_BUILDER | JSCLASS_SKIP_NURSERY_FINALIZE |        \
       JSCLASS_BACKGROUND_FINALIZE,                                            \
   &TypedArrayClassOps, &TypedArrayObjectClassSpecs[Scalar::Type::Name],       \
   &TypedArrayClassExtension},

    JS_FOR_EACH_TYPED_ARRAY(IMPL_TYPED_ARRAY_CLASS)
#undef IMPL_TYPED_ARRAY_CLASS
};
>>>>>>> upstream-releases

// The various typed array prototypes are supposed to 1) be normal objects,
// 2) stringify to "[object <name of constructor>]", and 3) (Gecko-specific)
// be xrayable.  The first and second requirements mandate (in the absence of
// @@toStringTag) a custom class.  The third requirement mandates that each
// prototype's class have the relevant typed array's cached JSProtoKey in them.
// Thus we need one class with cached prototype per kind of typed array, with a
// delegated ClassSpec.
<<<<<<< HEAD
#define IMPL_TYPED_ARRAY_PROTO_CLASS(_type)                                    \
  {                                                                            \
/*                                                                             \
 * Actually ({}).toString.call(Uint8Array.prototype) should throw, because     \
 * Uint8Array.prototype lacks the the typed array internal slots.  (Same as    \
 * with %TypedArray%.prototype.)  It's not clear this is desirable (see        \
 * above), but it's what we've always done, so keep doing it till we           \
 * implement @@toStringTag or ES6 changes.                                     \
 */                                                                            \
#_type "ArrayPrototype", JSCLASS_HAS_CACHED_PROTO(JSProto_##_type##Array), \
        JS_NULL_CLASS_OPS, &TypedArrayObjectClassSpecs[Scalar::Type::_type]    \
  }

||||||| merged common ancestors
#define IMPL_TYPED_ARRAY_PROTO_CLASS(_type) \
{ \
    /*
     * Actually ({}).toString.call(Uint8Array.prototype) should throw, because
     * Uint8Array.prototype lacks the the typed array internal slots.  (Same as
     * with %TypedArray%.prototype.)  It's not clear this is desirable (see
     * above), but it's what we've always done, so keep doing it till we
     * implement @@toStringTag or ES6 changes.
     */ \
    #_type "ArrayPrototype", \
    JSCLASS_HAS_CACHED_PROTO(JSProto_##_type##Array), \
    JS_NULL_CLASS_OPS, \
    &TypedArrayObjectClassSpecs[Scalar::Type::_type] \
}

=======
//
// Actually ({}).toString.call(Uint8Array.prototype) should throw, because
// Uint8Array.prototype lacks the the typed array internal slots.  (Same as
// with %TypedArray%.prototype.)  It's not clear this is desirable (see
// above), but it's what we've always done, so keep doing it till we
// implement @@toStringTag or ES6 changes.
>>>>>>> upstream-releases
const Class TypedArrayObject::protoClasses[Scalar::MaxTypedArrayViewType] = {
<<<<<<< HEAD
    IMPL_TYPED_ARRAY_PROTO_CLASS(Int8),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Uint8),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Int16),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Uint16),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Int32),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Uint32),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Float32),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Float64),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Uint8Clamped)};
||||||| merged common ancestors
    IMPL_TYPED_ARRAY_PROTO_CLASS(Int8),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Uint8),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Int16),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Uint16),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Int32),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Uint32),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Float32),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Float64),
    IMPL_TYPED_ARRAY_PROTO_CLASS(Uint8Clamped)
};
=======
#define IMPL_TYPED_ARRAY_PROTO_CLASS(NativeType, Name)                      \
  {#Name "ArrayPrototype", JSCLASS_HAS_CACHED_PROTO(JSProto_##Name##Array), \
   JS_NULL_CLASS_OPS, &TypedArrayObjectClassSpecs[Scalar::Type::Name]},

    JS_FOR_EACH_TYPED_ARRAY(IMPL_TYPED_ARRAY_PROTO_CLASS)
#undef IMPL_TYPED_ARRAY_PROTO_CLASS
};
>>>>>>> upstream-releases

<<<<<<< HEAD
/* static */ bool TypedArrayObject::isOriginalLengthGetter(Native native) {
  return native == TypedArray_lengthGetter;
}

bool js::IsTypedArrayConstructor(HandleValue v, uint32_t type) {
  switch (type) {
    case Scalar::Int8:
      return IsNativeFunction(v, Int8Array::class_constructor);
    case Scalar::Uint8:
      return IsNativeFunction(v, Uint8Array::class_constructor);
    case Scalar::Int16:
      return IsNativeFunction(v, Int16Array::class_constructor);
    case Scalar::Uint16:
      return IsNativeFunction(v, Uint16Array::class_constructor);
    case Scalar::Int32:
      return IsNativeFunction(v, Int32Array::class_constructor);
    case Scalar::Uint32:
      return IsNativeFunction(v, Uint32Array::class_constructor);
    case Scalar::Float32:
      return IsNativeFunction(v, Float32Array::class_constructor);
    case Scalar::Float64:
      return IsNativeFunction(v, Float64Array::class_constructor);
    case Scalar::Uint8Clamped:
      return IsNativeFunction(v, Uint8ClampedArray::class_constructor);
    case Scalar::MaxTypedArrayViewType:
      break;
  }
  MOZ_CRASH("unexpected typed array type");
||||||| merged common ancestors
/* static */ bool
TypedArrayObject::isOriginalLengthGetter(Native native)
{
    return native == TypedArray_lengthGetter;
}

bool
js::IsTypedArrayConstructor(HandleValue v, uint32_t type)
{
    switch (type) {
      case Scalar::Int8:
        return IsNativeFunction(v, Int8Array::class_constructor);
      case Scalar::Uint8:
        return IsNativeFunction(v, Uint8Array::class_constructor);
      case Scalar::Int16:
        return IsNativeFunction(v, Int16Array::class_constructor);
      case Scalar::Uint16:
        return IsNativeFunction(v, Uint16Array::class_constructor);
      case Scalar::Int32:
        return IsNativeFunction(v, Int32Array::class_constructor);
      case Scalar::Uint32:
        return IsNativeFunction(v, Uint32Array::class_constructor);
      case Scalar::Float32:
        return IsNativeFunction(v, Float32Array::class_constructor);
      case Scalar::Float64:
        return IsNativeFunction(v, Float64Array::class_constructor);
      case Scalar::Uint8Clamped:
        return IsNativeFunction(v, Uint8ClampedArray::class_constructor);
      case Scalar::MaxTypedArrayViewType:
        break;
    }
    MOZ_CRASH("unexpected typed array type");
=======
/* static */
bool TypedArrayObject::isOriginalLengthGetter(Native native) {
  return native == TypedArray_lengthGetter;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool js::IsBufferSource(JSObject* object, SharedMem<uint8_t*>* dataPointer,
                        size_t* byteLength) {
  if (object->is<TypedArrayObject>()) {
    TypedArrayObject& view = object->as<TypedArrayObject>();
    *dataPointer = view.dataPointerEither().cast<uint8_t*>();
    *byteLength = view.byteLength();
    return true;
  }
||||||| merged common ancestors
bool
js::IsBufferSource(JSObject* object, SharedMem<uint8_t*>* dataPointer, size_t* byteLength)
{
    if (object->is<TypedArrayObject>()) {
        TypedArrayObject& view = object->as<TypedArrayObject>();
        *dataPointer = view.dataPointerEither().cast<uint8_t*>();
        *byteLength = view.byteLength();
        return true;
    }
=======
/* static */
bool TypedArrayObject::isOriginalByteOffsetGetter(Native native) {
  return native == TypedArray_byteOffsetGetter;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (object->is<DataViewObject>()) {
    DataViewObject& view = object->as<DataViewObject>();
    *dataPointer = view.dataPointerEither().cast<uint8_t*>();
    *byteLength = view.byteLength();
    return true;
  }
||||||| merged common ancestors
    if (object->is<DataViewObject>()) {
        DataViewObject& view = object->as<DataViewObject>();
        *dataPointer = view.dataPointerEither().cast<uint8_t*>();
        *byteLength = view.byteLength();
        return true;
    }
=======
bool js::IsTypedArrayConstructor(const JSObject* obj) {
#define CHECK_TYPED_ARRAY_CONSTRUCTOR(T, N)                 \
  if (IsNativeFunction(obj, N##Array::class_constructor)) { \
    return true;                                            \
  }
  JS_FOR_EACH_TYPED_ARRAY(CHECK_TYPED_ARRAY_CONSTRUCTOR)
#undef CHECK_TYPED_ARRAY_CONSTRUCTOR
  return false;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (object->is<ArrayBufferObject>()) {
    ArrayBufferObject& buffer = object->as<ArrayBufferObject>();
    *dataPointer = buffer.dataPointerShared();
    *byteLength = buffer.byteLength();
    return true;
  }
||||||| merged common ancestors
    if (object->is<ArrayBufferObject>()) {
        ArrayBufferObject& buffer = object->as<ArrayBufferObject>();
        *dataPointer = buffer.dataPointerShared();
        *byteLength = buffer.byteLength();
        return true;
    }
=======
bool js::IsTypedArrayConstructor(HandleValue v, uint32_t type) {
  switch (type) {
    case Scalar::Int8:
      return IsNativeFunction(v, Int8Array::class_constructor);
    case Scalar::Uint8:
      return IsNativeFunction(v, Uint8Array::class_constructor);
    case Scalar::Int16:
      return IsNativeFunction(v, Int16Array::class_constructor);
    case Scalar::Uint16:
      return IsNativeFunction(v, Uint16Array::class_constructor);
    case Scalar::Int32:
      return IsNativeFunction(v, Int32Array::class_constructor);
    case Scalar::Uint32:
      return IsNativeFunction(v, Uint32Array::class_constructor);
    case Scalar::BigInt64:
      return IsNativeFunction(v, BigInt64Array::class_constructor);
    case Scalar::BigUint64:
      return IsNativeFunction(v, BigUint64Array::class_constructor);
    case Scalar::Float32:
      return IsNativeFunction(v, Float32Array::class_constructor);
    case Scalar::Float64:
      return IsNativeFunction(v, Float64Array::class_constructor);
    case Scalar::Uint8Clamped:
      return IsNativeFunction(v, Uint8ClampedArray::class_constructor);
    case Scalar::MaxTypedArrayViewType:
      break;
  }
  MOZ_CRASH("unexpected typed array type");
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (object->is<SharedArrayBufferObject>()) {
    SharedArrayBufferObject& buffer = object->as<SharedArrayBufferObject>();
    *dataPointer = buffer.dataPointerShared();
    *byteLength = buffer.byteLength();
    return true;
  }
||||||| merged common ancestors
    if (object->is<SharedArrayBufferObject>()) {
        SharedArrayBufferObject& buffer = object->as<SharedArrayBufferObject>();
        *dataPointer = buffer.dataPointerShared();
        *byteLength = buffer.byteLength();
        return true;
    }
=======
bool js::IsBufferSource(JSObject* object, SharedMem<uint8_t*>* dataPointer,
                        size_t* byteLength) {
  if (object->is<TypedArrayObject>()) {
    TypedArrayObject& view = object->as<TypedArrayObject>();
    *dataPointer = view.dataPointerEither().cast<uint8_t*>();
    *byteLength = view.byteLength();
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return false;
}
||||||| merged common ancestors
    return false;
}
=======
  if (object->is<DataViewObject>()) {
    DataViewObject& view = object->as<DataViewObject>();
    *dataPointer = view.dataPointerEither().cast<uint8_t*>();
    *byteLength = view.byteLength();
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename CharT>
bool js::StringIsTypedArrayIndex(const CharT* s, size_t length,
                                 uint64_t* indexp) {
  const CharT* end = s + length;
||||||| merged common ancestors
template <typename CharT>
bool
js::StringIsTypedArrayIndex(const CharT* s, size_t length, uint64_t* indexp)
{
    const CharT* end = s + length;
=======
  if (object->is<ArrayBufferObject>()) {
    ArrayBufferObject& buffer = object->as<ArrayBufferObject>();
    *dataPointer = buffer.dataPointerShared();
    *byteLength = buffer.byteLength();
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (s == end) {
    return false;
  }
||||||| merged common ancestors
    if (s == end) {
        return false;
    }

    bool negative = false;
    if (*s == '-') {
        negative = true;
        if (++s == end) {
            return false;
        }
    }
=======
  if (object->is<SharedArrayBufferObject>()) {
    SharedArrayBufferObject& buffer = object->as<SharedArrayBufferObject>();
    *dataPointer = buffer.dataPointerShared();
    *byteLength = buffer.byteLength();
    return true;
  }

  return false;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool negative = false;
  if (*s == '-') {
    negative = true;
    if (++s == end) {
      return false;
    }
  }
||||||| merged common ancestors
    if (!IsAsciiDigit(*s)) {
        return false;
    }
=======
template <typename CharT>
bool js::StringIsTypedArrayIndex(const CharT* s, size_t length,
                                 uint64_t* indexp) {
  const CharT* end = s + length;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!IsAsciiDigit(*s)) {
    return false;
  }
||||||| merged common ancestors
    uint64_t index = 0;
    uint32_t digit = JS7_UNDEC(*s++);

    /* Don't allow leading zeros. */
    if (digit == 0 && s != end) {
        return false;
    }
=======
  if (s == end) {
    return false;
  }

  bool negative = false;
  if (*s == '-') {
    negative = true;
    if (++s == end) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint64_t index = 0;
  uint32_t digit = JS7_UNDEC(*s++);
||||||| merged common ancestors
    index = digit;
=======
  if (!IsAsciiDigit(*s)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Don't allow leading zeros. */
  if (digit == 0 && s != end) {
    return false;
  }
||||||| merged common ancestors
    for (; s < end; s++) {
        if (!IsAsciiDigit(*s)) {
            return false;
        }
=======
  uint64_t index = 0;
  uint32_t digit = AsciiDigitToNumber(*s++);

  /* Don't allow leading zeros. */
  if (digit == 0 && s != end) {
    return false;
  }
>>>>>>> upstream-releases

  index = digit;

  for (; s < end; s++) {
    if (!IsAsciiDigit(*s)) {
      return false;
    }

<<<<<<< HEAD
    digit = JS7_UNDEC(*s);

    /* Watch for overflows. */
    if ((UINT64_MAX - digit) / 10 < index) {
      index = UINT64_MAX;
||||||| merged common ancestors
    if (negative) {
        *indexp = UINT64_MAX;
=======
    digit = AsciiDigitToNumber(*s);

    /* Watch for overflows. */
    if ((UINT64_MAX - digit) / 10 < index) {
      index = UINT64_MAX;
>>>>>>> upstream-releases
    } else {
      index = 10 * index + digit;
    }
  }

  if (negative) {
    *indexp = UINT64_MAX;
  } else {
    *indexp = index;
  }
  return true;
}

<<<<<<< HEAD
template bool js::StringIsTypedArrayIndex(const char16_t* s, size_t length,
                                          uint64_t* indexp);
||||||| merged common ancestors
template bool
js::StringIsTypedArrayIndex(const char16_t* s, size_t length, uint64_t* indexp);
=======
template bool js::StringIsTypedArrayIndex(const char16_t* s, size_t length,
                                          uint64_t* indexp);

template bool js::StringIsTypedArrayIndex(const Latin1Char* s, size_t length,
                                          uint64_t* indexp);

bool js::SetTypedArrayElement(JSContext* cx, Handle<TypedArrayObject*> obj,
                              uint64_t index, HandleValue v,
                              ObjectOpResult& result) {
  TypedArrayObject* tobj = &obj->as<TypedArrayObject>();

  switch (tobj->type()) {
#define SET_TYPED_ARRAY_ELEMENT(T, N) \
  case Scalar::N:                     \
    return TypedArrayObjectTemplate<T>::setElement(cx, obj, index, v, result);
    JS_FOR_EACH_TYPED_ARRAY(SET_TYPED_ARRAY_ELEMENT)
#undef SET_TYPED_ARRAY_ELEMENT
    case Scalar::MaxTypedArrayViewType:
    case Scalar::Int64:
      break;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template bool js::StringIsTypedArrayIndex(const Latin1Char* s, size_t length,
                                          uint64_t* indexp);
||||||| merged common ancestors
template bool
js::StringIsTypedArrayIndex(const Latin1Char* s, size_t length, uint64_t* indexp);
=======
  MOZ_CRASH("Unsupported TypedArray type");
}
>>>>>>> upstream-releases

/* ES6 draft rev 34 (2015 Feb 20) 9.4.5.3 [[DefineOwnProperty]] step 3.c. */
<<<<<<< HEAD
bool js::DefineTypedArrayElement(JSContext* cx, HandleObject obj,
                                 uint64_t index,
                                 Handle<PropertyDescriptor> desc,
                                 ObjectOpResult& result) {
  MOZ_ASSERT(obj->is<TypedArrayObject>());

  // These are all substeps of 3.b.
||||||| merged common ancestors
bool
js::DefineTypedArrayElement(JSContext* cx, HandleObject obj, uint64_t index,
                            Handle<PropertyDescriptor> desc, ObjectOpResult& result)
{
    MOZ_ASSERT(obj->is<TypedArrayObject>());

    // These are all substeps of 3.b.

    // Steps i-iii are handled by the caller.

    // Steps iv-v.
    // We (wrongly) ignore out of range defines with a value.
    uint32_t length = obj->as<TypedArrayObject>().length();
    if (index >= length) {
        if (obj->as<TypedArrayObject>().hasDetachedBuffer()) {
            return result.failSoft(JSMSG_TYPED_ARRAY_DETACHED);
        }
        return result.failSoft(JSMSG_BAD_INDEX);
    }

    // Step vi.
    if (desc.isAccessorDescriptor()) {
        return result.fail(JSMSG_CANT_REDEFINE_PROP);
    }
=======
bool js::DefineTypedArrayElement(JSContext* cx, HandleObject obj,
                                 uint64_t index,
                                 Handle<PropertyDescriptor> desc,
                                 ObjectOpResult& result) {
  MOZ_ASSERT(obj->is<TypedArrayObject>());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Steps i-iii are handled by the caller.
||||||| merged common ancestors
    // Step vii.
    if (desc.hasConfigurable() && desc.configurable()) {
        return result.fail(JSMSG_CANT_REDEFINE_PROP);
    }
=======
  // These are all substeps of 3.b.
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Steps iv-v.
  // We (wrongly) ignore out of range defines with a value.
  uint32_t length = obj->as<TypedArrayObject>().length();
  if (index >= length) {
    if (obj->as<TypedArrayObject>().hasDetachedBuffer()) {
      return result.failSoft(JSMSG_TYPED_ARRAY_DETACHED);
    }
    return result.failSoft(JSMSG_BAD_INDEX);
  }
||||||| merged common ancestors
    // Step viii.
    if (desc.hasEnumerable() && !desc.enumerable()) {
        return result.fail(JSMSG_CANT_REDEFINE_PROP);
    }
=======
  // Steps i-iii are handled by the caller.
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step vi.
  if (desc.isAccessorDescriptor()) {
    return result.fail(JSMSG_CANT_REDEFINE_PROP);
  }
||||||| merged common ancestors
    // Step ix.
    if (desc.hasWritable() && !desc.writable()) {
        return result.fail(JSMSG_CANT_REDEFINE_PROP);
    }
=======
  // Steps iv-v.
  // We (wrongly) ignore out of range defines with a value.
  uint32_t length = obj->as<TypedArrayObject>().length();
  if (index >= length) {
    if (obj->as<TypedArrayObject>().hasDetachedBuffer()) {
      return result.failSoft(JSMSG_TYPED_ARRAY_DETACHED);
    }
    return result.failSoft(JSMSG_BAD_INDEX);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step vii.
  if (desc.hasConfigurable() && desc.configurable()) {
    return result.fail(JSMSG_CANT_REDEFINE_PROP);
  }

  // Step viii.
  if (desc.hasEnumerable() && !desc.enumerable()) {
    return result.fail(JSMSG_CANT_REDEFINE_PROP);
  }

  // Step ix.
  if (desc.hasWritable() && !desc.writable()) {
    return result.fail(JSMSG_CANT_REDEFINE_PROP);
  }

  // Step x.
  if (desc.hasValue()) {
    // The following step numbers refer to 9.4.5.9
    // IntegerIndexedElementSet.

    // Steps 1-2 are enforced by the caller.

    // Step 3.
    double numValue;
    if (!ToNumber(cx, desc.value(), &numValue)) {
      return false;
    }

    // Steps 4-5, 8-9.
    if (obj->as<TypedArrayObject>().hasDetachedBuffer()) {
      return result.fail(JSMSG_TYPED_ARRAY_DETACHED);
    }
||||||| merged common ancestors
    // Step x.
    if (desc.hasValue()) {
        // The following step numbers refer to 9.4.5.9
        // IntegerIndexedElementSet.

        // Steps 1-2 are enforced by the caller.

        // Step 3.
        double numValue;
        if (!ToNumber(cx, desc.value(), &numValue)) {
            return false;
        }

        // Steps 4-5, 8-9.
        if (obj->as<TypedArrayObject>().hasDetachedBuffer()) {
            return result.fail(JSMSG_TYPED_ARRAY_DETACHED);
        }

        // Steps 10-16.
        TypedArrayObject::setElement(obj->as<TypedArrayObject>(), index, numValue);
    }
=======
  // Step vi.
  if (desc.isAccessorDescriptor()) {
    return result.fail(JSMSG_CANT_REDEFINE_PROP);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Steps 10-16.
    TypedArrayObject::setElement(obj->as<TypedArrayObject>(), index, numValue);
  }

  // Step xii.
  return result.succeed();
}
||||||| merged common ancestors
    // Step xii.
    return result.succeed();
}
=======
  // Step vii.
  if (desc.hasConfigurable() && desc.configurable()) {
    return result.fail(JSMSG_CANT_REDEFINE_PROP);
  }
>>>>>>> upstream-releases

  // Step viii.
  if (desc.hasEnumerable() && !desc.enumerable()) {
    return result.fail(JSMSG_CANT_REDEFINE_PROP);
  }

<<<<<<< HEAD
JS_FRIEND_API bool JS_IsTypedArrayObject(JSObject* obj) {
  return obj->canUnwrapAs<TypedArrayObject>();
}
||||||| merged common ancestors
JS_FRIEND_API(bool)
JS_IsTypedArrayObject(JSObject* obj)
{
    obj = CheckedUnwrap(obj);
    return obj ? obj->is<TypedArrayObject>() : false;
}
=======
  // Step ix.
  if (desc.hasWritable() && !desc.writable()) {
    return result.fail(JSMSG_CANT_REDEFINE_PROP);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_FRIEND_API uint32_t JS_GetTypedArrayLength(JSObject* obj) {
  obj = CheckedUnwrap(obj);
  if (!obj) {
    return 0;
  }
  return obj->as<TypedArrayObject>().length();
}
||||||| merged common ancestors
JS_FRIEND_API(uint32_t)
JS_GetTypedArrayLength(JSObject* obj)
{
    obj = CheckedUnwrap(obj);
    if (!obj) {
        return 0;
    }
    return obj->as<TypedArrayObject>().length();
}
=======
  // Step x.
  if (desc.hasValue()) {
    TypedArrayObject* tobj = &obj->as<TypedArrayObject>();
    switch (tobj->type()) {
#define DEFINE_TYPED_ARRAY_ELEMENT(T, N)                              \
  case Scalar::N:                                                     \
    return TypedArrayObjectTemplate<T>::defineElement(cx, obj, index, \
                                                      desc.value(), result);
      JS_FOR_EACH_TYPED_ARRAY(DEFINE_TYPED_ARRAY_ELEMENT)
#undef DEFINE_TYPED_ARRAY_ELEMENT
      case Scalar::MaxTypedArrayViewType:
      case Scalar::Int64:
        break;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_FRIEND_API uint32_t JS_GetTypedArrayByteOffset(JSObject* obj) {
  obj = CheckedUnwrap(obj);
  if (!obj) {
    return 0;
  }
  return obj->as<TypedArrayObject>().byteOffset();
}
||||||| merged common ancestors
JS_FRIEND_API(uint32_t)
JS_GetTypedArrayByteOffset(JSObject* obj)
{
    obj = CheckedUnwrap(obj);
    if (!obj) {
        return 0;
    }
    return obj->as<TypedArrayObject>().byteOffset();
}
=======
    MOZ_CRASH("Unsupported TypedArray type");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_FRIEND_API uint32_t JS_GetTypedArrayByteLength(JSObject* obj) {
  obj = CheckedUnwrap(obj);
  if (!obj) {
    return 0;
  }
  return obj->as<TypedArrayObject>().byteLength();
||||||| merged common ancestors
JS_FRIEND_API(uint32_t)
JS_GetTypedArrayByteLength(JSObject* obj)
{
    obj = CheckedUnwrap(obj);
    if (!obj) {
        return 0;
    }
    return obj->as<TypedArrayObject>().byteLength();
=======
  // Step xii.
  return result.succeed();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_FRIEND_API bool JS_GetTypedArraySharedness(JSObject* obj) {
  obj = CheckedUnwrap(obj);
  if (!obj) {
    return false;
  }
  return obj->as<TypedArrayObject>().isSharedMemory();
}
||||||| merged common ancestors
JS_FRIEND_API(bool)
JS_GetTypedArraySharedness(JSObject* obj)
{
    obj = CheckedUnwrap(obj);
    if (!obj) {
        return false;
    }
    return obj->as<TypedArrayObject>().isSharedMemory();
}
=======
/* JS Friend API */
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_FRIEND_API js::Scalar::Type JS_GetArrayBufferViewType(JSObject* obj) {
  obj = CheckedUnwrap(obj);
  if (!obj) {
    return Scalar::MaxTypedArrayViewType;
  }
||||||| merged common ancestors
JS_FRIEND_API(js::Scalar::Type)
JS_GetArrayBufferViewType(JSObject* obj)
{
    obj = CheckedUnwrap(obj);
    if (!obj) {
        return Scalar::MaxTypedArrayViewType;
    }
=======
template <typename NativeType>
struct ExternalTypeOf {
  using Type = NativeType;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (obj->is<TypedArrayObject>()) {
    return obj->as<TypedArrayObject>().type();
  }
  if (obj->is<DataViewObject>()) {
    return Scalar::MaxTypedArrayViewType;
  }
  MOZ_CRASH("invalid ArrayBufferView type");
}
||||||| merged common ancestors
    if (obj->is<TypedArrayObject>()) {
        return obj->as<TypedArrayObject>().type();
    }
    if (obj->is<DataViewObject>()) {
        return Scalar::MaxTypedArrayViewType;
    }
    MOZ_CRASH("invalid ArrayBufferView type");
}
=======
template <>
struct ExternalTypeOf<uint8_clamped> {
  using Type = uint8_t;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_FRIEND_API int8_t* JS_GetInt8ArrayData(JSObject* obj, bool* isSharedMemory,
                                          const JS::AutoRequireNoGC&) {
  obj = CheckedUnwrap(obj);
  if (!obj) {
    return nullptr;
  }
  TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
  MOZ_ASSERT((int32_t)tarr->type() == Scalar::Int8);
  *isSharedMemory = tarr->isSharedMemory();
  return static_cast<int8_t*>(
      tarr->dataPointerEither().unwrap(/*safe - caller sees isShared*/));
}
||||||| merged common ancestors
JS_FRIEND_API(int8_t*)
JS_GetInt8ArrayData(JSObject* obj, bool* isSharedMemory, const JS::AutoRequireNoGC&)
{
    obj = CheckedUnwrap(obj);
    if (!obj) {
        return nullptr;
    }
    TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
    MOZ_ASSERT((int32_t) tarr->type() == Scalar::Int8);
    *isSharedMemory = tarr->isSharedMemory();
    return static_cast<int8_t*>(tarr->dataPointerEither().unwrap(/*safe - caller sees isShared*/));
}
=======
#define IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS(NativeType, Name)                \
  JS_FRIEND_API JSObject* JS_New##Name##Array(JSContext* cx,                 \
                                              uint32_t nelements) {          \
    return TypedArrayObjectTemplate<NativeType>::fromLength(cx, nelements);  \
  }                                                                          \
                                                                             \
  JS_FRIEND_API JSObject* JS_New##Name##ArrayFromArray(JSContext* cx,        \
                                                       HandleObject other) { \
    return TypedArrayObjectTemplate<NativeType>::fromArray(cx, other);       \
  }                                                                          \
                                                                             \
  JS_FRIEND_API JSObject* JS_New##Name##ArrayWithBuffer(                     \
      JSContext* cx, HandleObject arrayBuffer, uint32_t byteOffset,          \
      int32_t length) {                                                      \
    return TypedArrayObjectTemplate<NativeType>::fromBuffer(                 \
        cx, arrayBuffer, byteOffset, length);                                \
  }                                                                          \
                                                                             \
  JS_FRIEND_API JSObject* js::Unwrap##Name##Array(JSObject* obj) {           \
    obj = obj->maybeUnwrapIf<TypedArrayObject>();                            \
    if (!obj) {                                                              \
      return nullptr;                                                        \
    }                                                                        \
    const Class* clasp = obj->getClass();                                    \
    if (clasp != TypedArrayObjectTemplate<NativeType>::instanceClass()) {    \
      return nullptr;                                                        \
    }                                                                        \
    return obj;                                                              \
  }                                                                          \
                                                                             \
  JS_FRIEND_API bool JS_Is##Name##Array(JSObject* obj) {                     \
    return js::Unwrap##Name##Array(obj) != nullptr;                          \
  }                                                                          \
                                                                             \
  const js::Class* const js::detail::Name##ArrayClassPtr =                   \
      &js::TypedArrayObject::classes                                         \
          [TypedArrayObjectTemplate<NativeType>::ArrayTypeID()];             \
                                                                             \
  JS_FRIEND_API JSObject* JS_GetObjectAs##Name##Array(                       \
      JSObject* obj, uint32_t* length, bool* isShared,                       \
      ExternalTypeOf<NativeType>::Type** data) {                             \
    obj = js::Unwrap##Name##Array(obj);                                      \
    if (!obj) {                                                              \
      return nullptr;                                                        \
    }                                                                        \
    TypedArrayObject* tarr = &obj->as<TypedArrayObject>();                   \
    *length = tarr->length();                                                \
    *isShared = tarr->isSharedMemory();                                      \
    *data = static_cast<ExternalTypeOf<NativeType>::Type*>(                  \
        tarr->dataPointerEither().unwrap(                                    \
            /*safe - caller sees isShared flag*/));                          \
    return obj;                                                              \
  }                                                                          \
                                                                             \
  JS_FRIEND_API ExternalTypeOf<NativeType>::Type* JS_Get##Name##ArrayData(   \
      JSObject* obj, bool* isSharedMemory, const JS::AutoRequireNoGC&) {     \
    TypedArrayObject* tarr = obj->maybeUnwrapAs<TypedArrayObject>();         \
    if (!tarr) {                                                             \
      return nullptr;                                                        \
    }                                                                        \
    MOZ_ASSERT(tarr->type() == TypeIDOfType<NativeType>::id);                \
    *isSharedMemory = tarr->isSharedMemory();                                \
    return static_cast<ExternalTypeOf<NativeType>::Type*>(                   \
        tarr->dataPointerEither().unwrap(/*safe - caller sees isShared*/));  \
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_FRIEND_API uint8_t* JS_GetUint8ArrayData(JSObject* obj, bool* isSharedMemory,
                                            const JS::AutoRequireNoGC&) {
  obj = CheckedUnwrap(obj);
  if (!obj) {
    return nullptr;
  }
  TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
  MOZ_ASSERT((int32_t)tarr->type() == Scalar::Uint8);
  *isSharedMemory = tarr->isSharedMemory();
  return static_cast<uint8_t*>(
      tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
}
||||||| merged common ancestors
JS_FRIEND_API(uint8_t*)
JS_GetUint8ArrayData(JSObject* obj, bool* isSharedMemory, const JS::AutoRequireNoGC&)
{
    obj = CheckedUnwrap(obj);
    if (!obj) {
        return nullptr;
    }
    TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
    MOZ_ASSERT((int32_t) tarr->type() == Scalar::Uint8);
    *isSharedMemory = tarr->isSharedMemory();
    return static_cast<uint8_t*>(tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
}
=======
JS_FOR_EACH_TYPED_ARRAY(IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS)
#undef IMPL_TYPED_ARRAY_JSAPI_CONSTRUCTORS
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_FRIEND_API uint8_t* JS_GetUint8ClampedArrayData(JSObject* obj,
                                                   bool* isSharedMemory,
                                                   const JS::AutoRequireNoGC&) {
  obj = CheckedUnwrap(obj);
  if (!obj) {
    return nullptr;
  }
  TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
  MOZ_ASSERT((int32_t)tarr->type() == Scalar::Uint8Clamped);
  *isSharedMemory = tarr->isSharedMemory();
  return static_cast<uint8_t*>(
      tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
||||||| merged common ancestors
JS_FRIEND_API(uint8_t*)
JS_GetUint8ClampedArrayData(JSObject* obj, bool* isSharedMemory, const JS::AutoRequireNoGC&)
{
    obj = CheckedUnwrap(obj);
    if (!obj) {
        return nullptr;
    }
    TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
    MOZ_ASSERT((int32_t) tarr->type() == Scalar::Uint8Clamped);
    *isSharedMemory = tarr->isSharedMemory();
    return static_cast<uint8_t*>(tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
=======
JS_FRIEND_API bool JS_IsTypedArrayObject(JSObject* obj) {
  return obj->canUnwrapAs<TypedArrayObject>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_FRIEND_API int16_t* JS_GetInt16ArrayData(JSObject* obj, bool* isSharedMemory,
                                            const JS::AutoRequireNoGC&) {
  obj = CheckedUnwrap(obj);
  if (!obj) {
    return nullptr;
  }
  TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
  MOZ_ASSERT((int32_t)tarr->type() == Scalar::Int16);
  *isSharedMemory = tarr->isSharedMemory();
  return static_cast<int16_t*>(
      tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
||||||| merged common ancestors
JS_FRIEND_API(int16_t*)
JS_GetInt16ArrayData(JSObject* obj, bool* isSharedMemory, const JS::AutoRequireNoGC&)
{
    obj = CheckedUnwrap(obj);
    if (!obj) {
        return nullptr;
    }
    TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
    MOZ_ASSERT((int32_t) tarr->type() == Scalar::Int16);
    *isSharedMemory = tarr->isSharedMemory();
    return static_cast<int16_t*>(tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
=======
JS_FRIEND_API uint32_t JS_GetTypedArrayLength(JSObject* obj) {
  TypedArrayObject* tarr = obj->maybeUnwrapAs<TypedArrayObject>();
  if (!tarr) {
    return 0;
  }
  return tarr->length();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_FRIEND_API uint16_t* JS_GetUint16ArrayData(JSObject* obj,
                                              bool* isSharedMemory,
                                              const JS::AutoRequireNoGC&) {
  obj = CheckedUnwrap(obj);
  if (!obj) {
    return nullptr;
  }
  TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
  MOZ_ASSERT((int32_t)tarr->type() == Scalar::Uint16);
  *isSharedMemory = tarr->isSharedMemory();
  return static_cast<uint16_t*>(
      tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
||||||| merged common ancestors
JS_FRIEND_API(uint16_t*)
JS_GetUint16ArrayData(JSObject* obj, bool* isSharedMemory, const JS::AutoRequireNoGC&)
{
    obj = CheckedUnwrap(obj);
    if (!obj) {
        return nullptr;
    }
    TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
    MOZ_ASSERT((int32_t) tarr->type() == Scalar::Uint16);
    *isSharedMemory = tarr->isSharedMemory();
    return static_cast<uint16_t*>(tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
=======
JS_FRIEND_API uint32_t JS_GetTypedArrayByteOffset(JSObject* obj) {
  TypedArrayObject* tarr = obj->maybeUnwrapAs<TypedArrayObject>();
  if (!tarr) {
    return 0;
  }
  return tarr->byteOffset();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_FRIEND_API int32_t* JS_GetInt32ArrayData(JSObject* obj, bool* isSharedMemory,
                                            const JS::AutoRequireNoGC&) {
  obj = CheckedUnwrap(obj);
  if (!obj) {
    return nullptr;
  }
  TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
  MOZ_ASSERT((int32_t)tarr->type() == Scalar::Int32);
  *isSharedMemory = tarr->isSharedMemory();
  return static_cast<int32_t*>(
      tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
||||||| merged common ancestors
JS_FRIEND_API(int32_t*)
JS_GetInt32ArrayData(JSObject* obj, bool* isSharedMemory, const JS::AutoRequireNoGC&)
{
    obj = CheckedUnwrap(obj);
    if (!obj) {
        return nullptr;
    }
    TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
    MOZ_ASSERT((int32_t) tarr->type() == Scalar::Int32);
    *isSharedMemory = tarr->isSharedMemory();
    return static_cast<int32_t*>(tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
=======
JS_FRIEND_API uint32_t JS_GetTypedArrayByteLength(JSObject* obj) {
  TypedArrayObject* tarr = obj->maybeUnwrapAs<TypedArrayObject>();
  if (!tarr) {
    return 0;
  }
  return tarr->byteLength();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_FRIEND_API uint32_t* JS_GetUint32ArrayData(JSObject* obj,
                                              bool* isSharedMemory,
                                              const JS::AutoRequireNoGC&) {
  obj = CheckedUnwrap(obj);
  if (!obj) {
    return nullptr;
  }
  TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
  MOZ_ASSERT((int32_t)tarr->type() == Scalar::Uint32);
  *isSharedMemory = tarr->isSharedMemory();
  return static_cast<uint32_t*>(
      tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
||||||| merged common ancestors
JS_FRIEND_API(uint32_t*)
JS_GetUint32ArrayData(JSObject* obj, bool* isSharedMemory, const JS::AutoRequireNoGC&)
{
    obj = CheckedUnwrap(obj);
    if (!obj) {
        return nullptr;
    }
    TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
    MOZ_ASSERT((int32_t) tarr->type() == Scalar::Uint32);
    *isSharedMemory = tarr->isSharedMemory();
    return static_cast<uint32_t*>(tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
=======
JS_FRIEND_API bool JS_GetTypedArraySharedness(JSObject* obj) {
  TypedArrayObject* tarr = obj->maybeUnwrapAs<TypedArrayObject>();
  if (!tarr) {
    return false;
  }
  return tarr->isSharedMemory();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_FRIEND_API float* JS_GetFloat32ArrayData(JSObject* obj, bool* isSharedMemory,
                                            const JS::AutoRequireNoGC&) {
  obj = CheckedUnwrap(obj);
  if (!obj) {
    return nullptr;
  }
  TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
  MOZ_ASSERT((int32_t)tarr->type() == Scalar::Float32);
  *isSharedMemory = tarr->isSharedMemory();
  return static_cast<float*>(
      tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
}
||||||| merged common ancestors
JS_FRIEND_API(float*)
JS_GetFloat32ArrayData(JSObject* obj, bool* isSharedMemory, const JS::AutoRequireNoGC&)
{
    obj = CheckedUnwrap(obj);
    if (!obj) {
        return nullptr;
    }
    TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
    MOZ_ASSERT((int32_t) tarr->type() == Scalar::Float32);
    *isSharedMemory = tarr->isSharedMemory();
    return static_cast<float*>(tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
}
=======
JS_FRIEND_API js::Scalar::Type JS_GetArrayBufferViewType(JSObject* obj) {
  ArrayBufferViewObject* view = obj->maybeUnwrapAs<ArrayBufferViewObject>();
  if (!view) {
    return Scalar::MaxTypedArrayViewType;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_FRIEND_API double* JS_GetFloat64ArrayData(JSObject* obj,
                                             bool* isSharedMemory,
                                             const JS::AutoRequireNoGC&) {
  obj = CheckedUnwrap(obj);
  if (!obj) {
    return nullptr;
  }
  TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
  MOZ_ASSERT((int32_t)tarr->type() == Scalar::Float64);
  *isSharedMemory = tarr->isSharedMemory();
  return static_cast<double*>(
      tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
||||||| merged common ancestors
JS_FRIEND_API(double*)
JS_GetFloat64ArrayData(JSObject* obj, bool* isSharedMemory, const JS::AutoRequireNoGC&)
{
    obj = CheckedUnwrap(obj);
    if (!obj) {
        return nullptr;
    }
    TypedArrayObject* tarr = &obj->as<TypedArrayObject>();
    MOZ_ASSERT((int32_t) tarr->type() == Scalar::Float64);
    *isSharedMemory = tarr->isSharedMemory();
    return static_cast<double*>(tarr->dataPointerEither().unwrap(/*safe - caller sees isSharedMemory*/));
=======
  if (view->is<TypedArrayObject>()) {
    return view->as<TypedArrayObject>().type();
  }
  if (view->is<DataViewObject>()) {
    return Scalar::MaxTypedArrayViewType;
  }
  MOZ_CRASH("invalid ArrayBufferView type");
>>>>>>> upstream-releases
}
