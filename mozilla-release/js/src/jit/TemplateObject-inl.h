/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_TemplateObject_inl_h
#define jit_TemplateObject_inl_h

#include "jit/TemplateObject.h"

#include "vm/RegExpObject.h"

namespace js {
namespace jit {

<<<<<<< HEAD
inline gc::AllocKind TemplateObject::getAllocKind() const {
  return obj_->asTenured().getAllocKind();
}

inline bool TemplateObject::isNative() const { return obj_->isNative(); }

inline bool TemplateObject::isArrayObject() const {
  return obj_->is<ArrayObject>();
||||||| merged common ancestors
inline gc::AllocKind
TemplateObject::getAllocKind() const
{
    return obj_->asTenured().getAllocKind();
}

inline bool
TemplateObject::isNative() const
{
    return obj_->isNative();
}

inline bool
TemplateObject::isArrayObject() const
{
    return obj_->is<ArrayObject>();
=======
inline gc::AllocKind TemplateObject::getAllocKind() const {
  return obj_->asTenured().getAllocKind();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool TemplateObject::isArgumentsObject() const {
  return obj_->is<ArgumentsObject>();
}
||||||| merged common ancestors
inline bool
TemplateObject::isArgumentsObject() const
{
    return obj_->is<ArgumentsObject>();
}
=======
inline bool TemplateObject::isNative() const { return obj_->isNative(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
inline bool TemplateObject::isTypedArrayObject() const {
  return obj_->is<TypedArrayObject>();
||||||| merged common ancestors
inline bool
TemplateObject::isTypedArrayObject() const
{
    return obj_->is<TypedArrayObject>();
=======
inline bool TemplateObject::isArrayObject() const {
  return obj_->is<ArrayObject>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool TemplateObject::isRegExpObject() const {
  return obj_->is<RegExpObject>();
||||||| merged common ancestors
inline bool
TemplateObject::isRegExpObject() const
{
    return obj_->is<RegExpObject>();
=======
inline bool TemplateObject::isArgumentsObject() const {
  return obj_->is<ArgumentsObject>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool TemplateObject::isInlineTypedObject() const {
  return obj_->is<InlineTypedObject>();
||||||| merged common ancestors
inline bool
TemplateObject::isInlineTypedObject() const
{
    return obj_->is<InlineTypedObject>();
=======
inline bool TemplateObject::isTypedArrayObject() const {
  return obj_->is<TypedArrayObject>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool TemplateObject::isUnboxedPlainObject() const {
  return obj_->is<UnboxedPlainObject>();
||||||| merged common ancestors
inline bool
TemplateObject::isUnboxedPlainObject() const
{
    return obj_->is<UnboxedPlainObject>();
=======
inline bool TemplateObject::isRegExpObject() const {
  return obj_->is<RegExpObject>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool TemplateObject::isCallObject() const {
  return obj_->is<CallObject>();
||||||| merged common ancestors
inline bool
TemplateObject::isCallObject() const
{
    return obj_->is<CallObject>();
=======
inline bool TemplateObject::isInlineTypedObject() const {
  return obj_->is<InlineTypedObject>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool TemplateObject::isPlainObject() const {
  return obj_->is<PlainObject>();
||||||| merged common ancestors
inline bool
TemplateObject::isPlainObject() const
{
    return obj_->is<PlainObject>();
=======
inline bool TemplateObject::isCallObject() const {
  return obj_->is<CallObject>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline gc::Cell* TemplateObject::group() const {
  MOZ_ASSERT(!obj_->hasLazyGroup());
  return obj_->group();
||||||| merged common ancestors
inline gc::Cell*
TemplateObject::group() const
{
    MOZ_ASSERT(!obj_->hasLazyGroup());
    return obj_->group();
=======
inline bool TemplateObject::isPlainObject() const {
  return obj_->is<PlainObject>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline gc::Cell* TemplateObject::maybeShape() const {
  if (obj_->is<ShapedObject>()) {
    Shape* shape = obj_->maybeShape();
    MOZ_ASSERT(!shape->inDictionary());
    return shape;
  }
  return nullptr;
||||||| merged common ancestors
inline gc::Cell*
TemplateObject::maybeShape() const
{
    if (obj_->is<ShapedObject>()) {
        Shape* shape = obj_->maybeShape();
        MOZ_ASSERT(!shape->inDictionary());
        return shape;
    }
    return nullptr;
=======
inline gc::Cell* TemplateObject::group() const {
  MOZ_ASSERT(!obj_->hasLazyGroup());
  return obj_->group();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline uint32_t TemplateObject::getInlineTypedObjectSize() const {
  return obj_->as<InlineTypedObject>().size();
||||||| merged common ancestors
inline uint32_t
TemplateObject::getInlineTypedObjectSize() const
{
    return obj_->as<InlineTypedObject>().size();
=======
inline gc::Cell* TemplateObject::shape() const {
  Shape* shape = obj_->shape();
  MOZ_ASSERT(!shape->inDictionary());
  return shape;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline uint8_t* TemplateObject::getInlineTypedObjectMem(
    const JS::AutoRequireNoGC& nogc) const {
  return obj_->as<InlineTypedObject>().inlineTypedMem(nogc);
||||||| merged common ancestors
inline uint8_t*
TemplateObject::getInlineTypedObjectMem(const JS::AutoRequireNoGC& nogc) const
{
    return obj_->as<InlineTypedObject>().inlineTypedMem(nogc);
=======
inline uint32_t TemplateObject::getInlineTypedObjectSize() const {
  return obj_->as<InlineTypedObject>().size();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline const UnboxedLayout& TemplateObject::unboxedObjectLayout() const {
  return obj_->as<UnboxedPlainObject>().layoutDontCheckGeneration();
||||||| merged common ancestors
inline const UnboxedLayout&
TemplateObject::unboxedObjectLayout() const
{
    return obj_->as<UnboxedPlainObject>().layoutDontCheckGeneration();
=======
inline uint8_t* TemplateObject::getInlineTypedObjectMem(
    const JS::AutoRequireNoGC& nogc) const {
  return obj_->as<InlineTypedObject>().inlineTypedMem(nogc);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
#ifdef DEBUG
inline bool TemplateObject::unboxedObjectHasExpando() const {
  return obj_->as<UnboxedPlainObject>().maybeExpando();
}
#endif

inline const NativeTemplateObject& TemplateObject::asNativeTemplateObject()
    const {
  MOZ_ASSERT(isNative());
  return *static_cast<const NativeTemplateObject*>(this);
||||||| merged common ancestors
#ifdef DEBUG
inline bool
TemplateObject::unboxedObjectHasExpando() const
{
    return obj_->as<UnboxedPlainObject>().maybeExpando();
}
#endif

inline const NativeTemplateObject&
TemplateObject::asNativeTemplateObject() const
{
    MOZ_ASSERT(isNative());
    return *static_cast<const NativeTemplateObject*>(this);
=======
inline const NativeTemplateObject& TemplateObject::asNativeTemplateObject()
    const {
  MOZ_ASSERT(isNative());
  return *static_cast<const NativeTemplateObject*>(this);
>>>>>>> upstream-releases
}

inline bool NativeTemplateObject::hasDynamicSlots() const {
  return asNative().hasDynamicSlots();
}

inline uint32_t NativeTemplateObject::numDynamicSlots() const {
  // We can't call numDynamicSlots because that uses shape->base->clasp and
  // shape->base can change when we create a ShapeTable.
  return NativeObject::dynamicSlotsCount(numFixedSlots(), slotSpan(),
                                         obj_->getClass());
}

inline uint32_t NativeTemplateObject::numUsedFixedSlots() const {
  return asNative().numUsedFixedSlots();
}

inline uint32_t NativeTemplateObject::numFixedSlots() const {
  return asNative().numFixedSlots();
}

inline uint32_t NativeTemplateObject::slotSpan() const {
  // Don't call NativeObject::slotSpan, it uses shape->base->clasp and the
  // shape's BaseShape can change when we create a ShapeTable for it.
  return asNative().shape()->slotSpan(obj_->getClass());
}

inline Value NativeTemplateObject::getSlot(uint32_t i) const {
  return asNative().getSlot(i);
}

inline const Value* NativeTemplateObject::getDenseElements() const {
  return asNative().getDenseElementsAllowCopyOnWrite();
}

#ifdef DEBUG
inline bool NativeTemplateObject::isSharedMemory() const {
  return asNative().isSharedMemory();
}
#endif

inline uint32_t NativeTemplateObject::getDenseCapacity() const {
  return asNative().getDenseCapacity();
}

inline uint32_t NativeTemplateObject::getDenseInitializedLength() const {
  return asNative().getDenseInitializedLength();
}

inline uint32_t NativeTemplateObject::getArrayLength() const {
  return obj_->as<ArrayObject>().length();
}

inline bool NativeTemplateObject::hasDynamicElements() const {
  return asNative().hasDynamicElements();
}

inline bool NativeTemplateObject::hasPrivate() const {
  return asNative().hasPrivate();
}

inline gc::Cell* NativeTemplateObject::regExpShared() const {
  RegExpObject* regexp = &obj_->as<RegExpObject>();
  MOZ_ASSERT(regexp->hasShared());
  MOZ_ASSERT(regexp->getPrivate() == regexp->sharedRef().get());
  return regexp->sharedRef().get();
}

inline void* NativeTemplateObject::getPrivate() const {
  return asNative().getPrivate();
}

}  // namespace jit
}  // namespace js

#endif /* jit_TemplateObject_inl_h */
