/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef gc_Allocator_h
#define gc_Allocator_h

#include "gc/GCLock.h"
#include "gc/Heap.h"
#include "js/RootingAPI.h"

class JSFatInlineString;

namespace js {

struct Class;

// Allocate a new GC thing that's not a JSObject or a string.
//
// After a successful allocation the caller must fully initialize the thing
// before calling any function that can potentially trigger GC. This will ensure
// that GC tracing never sees junk values stored in the partially initialized
// thing.
template <typename T, AllowGC allowGC = CanGC>
T* Allocate(JSContext* cx);

<<<<<<< HEAD
// Use for JSObject. A longer signature that includes additional information in
// support of various optimizations. If dynamic slots are requested they will be
// allocated and the pointer stored directly in |NativeObject::slots_|.
template <typename, AllowGC allowGC = CanGC>
JSObject* Allocate(JSContext* cx, gc::AllocKind kind, size_t nDynamicSlots,
                   gc::InitialHeap heap, const Class* clasp);
||||||| merged common ancestors
// Use for JSObject. A longer signature that includes additional information in
// support of various optimizations. If dynamic slots are requested they will be
// allocated and the pointer stored directly in |NativeObject::slots_|.
template <typename, AllowGC allowGC = CanGC>
JSObject*
Allocate(JSContext* cx, gc::AllocKind kind, size_t nDynamicSlots, gc::InitialHeap heap,
         const Class* clasp);
=======
// Allocate a JSObject.
//
// A longer signature that includes additional information in support of various
// optimizations. If dynamic slots are requested they will be allocated and the
// pointer stored directly in |NativeObject::slots_|.
template <AllowGC allowGC = CanGC>
JSObject* AllocateObject(JSContext* cx, gc::AllocKind kind,
                         size_t nDynamicSlots, gc::InitialHeap heap,
                         const Class* clasp);
>>>>>>> upstream-releases

// Internal function used for nursery-allocatable strings.
template <typename StringAllocT, AllowGC allowGC = CanGC>
<<<<<<< HEAD
StringAllocT* AllocateString(JSContext* cx, gc::InitialHeap heap);
||||||| merged common ancestors
StringAllocT*
AllocateString(JSContext* cx, gc::InitialHeap heap);
=======
StringAllocT* AllocateStringImpl(JSContext* cx, gc::InitialHeap heap);
>>>>>>> upstream-releases

// Allocate a string.
//
// Use for nursery-allocatable strings. Returns a value cast to the correct
// type.
template <typename StringT, AllowGC allowGC = CanGC>
<<<<<<< HEAD
StringT* Allocate(JSContext* cx, gc::InitialHeap heap) {
  return static_cast<StringT*>(js::AllocateString<JSString, allowGC>(cx, heap));
||||||| merged common ancestors
StringT*
Allocate(JSContext* cx, gc::InitialHeap heap)
{
    return static_cast<StringT*>(js::AllocateString<JSString, allowGC>(cx, heap));
=======
StringT* AllocateString(JSContext* cx, gc::InitialHeap heap) {
  return static_cast<StringT*>(AllocateStringImpl<JSString, allowGC>(cx, heap));
>>>>>>> upstream-releases
}

// Specialization for JSFatInlineString that must use a different allocation
// type. Note that we have to explicitly specialize for both values of AllowGC
// because partial function specialization is not allowed.
template <>
<<<<<<< HEAD
inline JSFatInlineString* Allocate<JSFatInlineString, CanGC>(
    JSContext* cx, gc::InitialHeap heap) {
  return static_cast<JSFatInlineString*>(
      js::AllocateString<JSFatInlineString, CanGC>(cx, heap));
||||||| merged common ancestors
inline JSFatInlineString*
Allocate<JSFatInlineString, CanGC>(JSContext* cx, gc::InitialHeap heap)
{
    return static_cast<JSFatInlineString*>(js::AllocateString<JSFatInlineString, CanGC>(cx, heap));
=======
inline JSFatInlineString* AllocateString<JSFatInlineString, CanGC>(
    JSContext* cx, gc::InitialHeap heap) {
  return static_cast<JSFatInlineString*>(
      js::AllocateStringImpl<JSFatInlineString, CanGC>(cx, heap));
>>>>>>> upstream-releases
}

template <>
<<<<<<< HEAD
inline JSFatInlineString* Allocate<JSFatInlineString, NoGC>(
    JSContext* cx, gc::InitialHeap heap) {
  return static_cast<JSFatInlineString*>(
      js::AllocateString<JSFatInlineString, NoGC>(cx, heap));
||||||| merged common ancestors
inline JSFatInlineString*
Allocate<JSFatInlineString, NoGC>(JSContext* cx, gc::InitialHeap heap)
{
    return static_cast<JSFatInlineString*>(js::AllocateString<JSFatInlineString, NoGC>(cx, heap));
=======
inline JSFatInlineString* AllocateString<JSFatInlineString, NoGC>(
    JSContext* cx, gc::InitialHeap heap) {
  return static_cast<JSFatInlineString*>(
      js::AllocateStringImpl<JSFatInlineString, NoGC>(cx, heap));
>>>>>>> upstream-releases
}

}  // namespace js

#endif  // gc_Allocator_h
