/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef vm_EnvironmentObject_inl_h
#define vm_EnvironmentObject_inl_h

#include "vm/EnvironmentObject.h"

#include "vm/JSObject-inl.h"
#include "vm/TypeInference-inl.h"

namespace js {

inline LexicalEnvironmentObject& NearestEnclosingExtensibleLexicalEnvironment(
    JSObject* env) {
  MOZ_ASSERT(env);
  while (!IsExtensibleLexicalEnvironment(env)) {
    env = env->enclosingEnvironment();
    MOZ_ASSERT(env);
  }
  return env->as<LexicalEnvironmentObject>();
}

<<<<<<< HEAD
inline void EnvironmentObject::setAliasedBinding(JSContext* cx, uint32_t slot,
                                                 PropertyName* name,
                                                 const Value& v) {
  if (isSingleton()) {
    MOZ_ASSERT(name);
    AddTypePropertyId(cx, this, NameToId(name), v);

    // Keep track of properties which have ever been overwritten.
    if (!getSlot(slot).isUndefined()) {
      Shape* shape = lookup(cx, name);
      shape->setOverwritten();
    }
  }

  setSlot(slot, v);
||||||| merged common ancestors
inline void
EnvironmentObject::setAliasedBinding(JSContext* cx, uint32_t slot, PropertyName* name,
                                     const Value& v)
{
    if (isSingleton()) {
        MOZ_ASSERT(name);
        AddTypePropertyId(cx, this, NameToId(name), v);

        // Keep track of properties which have ever been overwritten.
        if (!getSlot(slot).isUndefined()) {
            Shape* shape = lookup(cx, name);
            shape->setOverwritten();
        }
    }

    setSlot(slot, v);
=======
// Returns the innermost "qualified var object" on the environment chain.
// See the JSObject::isQualifiedVarObj comment for more info.
inline JSObject& GetVariablesObject(JSObject* envChain) {
  while (!envChain->isQualifiedVarObj()) {
    envChain = envChain->enclosingEnvironment();
  }
  MOZ_ASSERT(envChain);
  return *envChain;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline void EnvironmentObject::setAliasedBinding(JSContext* cx,
                                                 EnvironmentCoordinate ec,
                                                 PropertyName* name,
                                                 const Value& v) {
  setAliasedBinding(cx, ec.slot(), name, v);
||||||| merged common ancestors
inline void
EnvironmentObject::setAliasedBinding(JSContext* cx, EnvironmentCoordinate ec, PropertyName* name,
                                     const Value& v)
{
    setAliasedBinding(cx, ec.slot(), name, v);
=======
inline const Value& EnvironmentObject::aliasedBinding(
    EnvironmentCoordinate ec) {
  MOZ_ASSERT(!IsExtensibleLexicalEnvironment(this));
  MOZ_ASSERT(nonExtensibleIsFixedSlot(ec) ==
             NativeObject::isFixedSlot(ec.slot()));
  return getSlot(ec.slot());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline void EnvironmentObject::setAliasedBinding(JSContext* cx,
                                                 const BindingIter& bi,
                                                 const Value& v) {
  MOZ_ASSERT(bi.location().kind() == BindingLocation::Kind::Environment);
  setAliasedBinding(cx, bi.location().slot(), bi.name()->asPropertyName(), v);
||||||| merged common ancestors
inline void
EnvironmentObject::setAliasedBinding(JSContext* cx, const BindingIter& bi, const Value& v)
{
    MOZ_ASSERT(bi.location().kind() == BindingLocation::Kind::Environment);
    setAliasedBinding(cx, bi.location().slot(), bi.name()->asPropertyName(), v);
=======
inline void EnvironmentObject::setAliasedBinding(JSContext* cx, uint32_t slot,
                                                 const Value& v) {
  MOZ_ASSERT(!isSingleton());
  setSlot(slot, v);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline void CallObject::setAliasedFormalFromArguments(JSContext* cx,
                                                      const Value& argsValue,
                                                      jsid id, const Value& v) {
  setSlot(ArgumentsObject::SlotFromMagicScopeSlotValue(argsValue), v);
  if (isSingleton()) {
    AddTypePropertyId(cx, this, id, v);
  }
||||||| merged common ancestors
inline void
CallObject::setAliasedFormalFromArguments(JSContext* cx, const Value& argsValue, jsid id,
                                          const Value& v)
{
    setSlot(ArgumentsObject::SlotFromMagicScopeSlotValue(argsValue), v);
    if (isSingleton()) {
        AddTypePropertyId(cx, this, id, v);
    }
=======
inline void EnvironmentObject::setAliasedBinding(JSContext* cx,
                                                 EnvironmentCoordinate ec,
                                                 const Value& v) {
  MOZ_ASSERT(!IsExtensibleLexicalEnvironment(this));
  MOZ_ASSERT(nonExtensibleIsFixedSlot(ec) ==
             NativeObject::isFixedSlot(ec.slot()));
  setAliasedBinding(cx, ec.slot(), v);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
} /* namespace js */
||||||| merged common ancestors
}  /* namespace js */
=======
inline void EnvironmentObject::setAliasedBinding(JSContext* cx,
                                                 const BindingIter& bi,
                                                 const Value& v) {
  MOZ_ASSERT(bi.location().kind() == BindingLocation::Kind::Environment);
  setAliasedBinding(cx, bi.location().slot(), v);
}

inline void CallObject::setAliasedFormalFromArguments(JSContext* cx,
                                                      const Value& argsValue,
                                                      jsid id, const Value& v) {
  setSlot(ArgumentsObject::SlotFromMagicScopeSlotValue(argsValue), v);
  if (isSingleton()) {
    AddTypePropertyId(cx, this, id, v);
  }
}

} /* namespace js */
>>>>>>> upstream-releases

inline JSObject* JSObject::enclosingEnvironment() const {
  if (is<js::EnvironmentObject>()) {
    return &as<js::EnvironmentObject>().enclosingEnvironment();
  }

  if (is<js::DebugEnvironmentProxy>()) {
    return &as<js::DebugEnvironmentProxy>().enclosingEnvironment();
  }

  if (is<js::GlobalObject>()) {
    return nullptr;
  }

  MOZ_ASSERT_IF(is<JSFunction>(), as<JSFunction>().isInterpreted());
  return &nonCCWGlobal();
}

#endif /* vm_EnvironmentObject_inl_h */
