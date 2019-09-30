/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef vm_ReceiverGuard_inl_h
#define vm_ReceiverGuard_inl_h

#include "vm/ReceiverGuard.h"

#include "builtin/TypedObject.h"
#include "vm/JSObject.h"

namespace js {

MOZ_ALWAYS_INLINE
<<<<<<< HEAD
ReceiverGuard::ReceiverGuard(JSObject* obj) : group(nullptr), shape(nullptr) {
  if (!obj->isNative()) {
    if (obj->is<UnboxedPlainObject>()) {
      group = obj->group();
      if (UnboxedExpandoObject* expando =
              obj->as<UnboxedPlainObject>().maybeExpando()) {
        shape = expando->lastProperty();
      }
      return;
    }
    if (obj->is<TypedObject>()) {
      group = obj->group();
      return;
    }
  }
  shape = obj->as<ShapedObject>().shape();
||||||| merged common ancestors
ReceiverGuard::ReceiverGuard(JSObject* obj)
  : group(nullptr), shape(nullptr)
{
    if (!obj->isNative()) {
        if (obj->is<UnboxedPlainObject>()) {
            group = obj->group();
            if (UnboxedExpandoObject* expando = obj->as<UnboxedPlainObject>().maybeExpando()) {
                shape = expando->lastProperty();
            }
            return;
        }
        if (obj->is<TypedObject>()) {
            group = obj->group();
            return;
        }
    }
    shape = obj->as<ShapedObject>().shape();
=======
ReceiverGuard::ReceiverGuard(JSObject* obj) : group_(nullptr), shape_(nullptr) {
  if (obj->isNative() || IsProxy(obj)) {
    shape_ = obj->shape();
    return;
  }
  MOZ_ASSERT(obj->is<TypedObject>());
  group_ = obj->group();
>>>>>>> upstream-releases
}

MOZ_ALWAYS_INLINE
ReceiverGuard::ReceiverGuard(ObjectGroup* group, Shape* shape)
<<<<<<< HEAD
    : group(group), shape(shape) {
  if (group) {
    const Class* clasp = group->clasp();
    if (clasp == &UnboxedPlainObject::class_) {
      // Keep both group and shape.
    } else if (IsTypedObjectClass(clasp)) {
      this->shape = nullptr;
    } else {
      this->group = nullptr;
||||||| merged common ancestors
  : group(group), shape(shape)
{
    if (group) {
        const Class* clasp = group->clasp();
        if (clasp == &UnboxedPlainObject::class_) {
            // Keep both group and shape.
        } else if (IsTypedObjectClass(clasp)) {
            this->shape = nullptr;
        } else {
            this->group = nullptr;
        }
=======
    : group_(group), shape_(shape) {
  if (group_) {
    const Class* clasp = group_->clasp();
    if (IsTypedObjectClass(clasp)) {
      this->shape_ = nullptr;
    } else {
      this->group_ = nullptr;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
  }
  // Only one of group_ or shape_ may be active at a time.
  MOZ_ASSERT_IF(group_ || shape_, !!group_ != !!shape_);
>>>>>>> upstream-releases
}

}  // namespace js

#endif /* vm_ReceiverGuard_inl_h */
