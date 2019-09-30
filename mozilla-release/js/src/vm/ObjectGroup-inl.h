/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef vm_ObjectGroup_inl_h
#define vm_ObjectGroup_inl_h

#include "vm/ObjectGroup.h"

#include "gc/Zone.h"

<<<<<<< HEAD
inline bool ObjectGroup::needsSweep() {
  // Note: this can be called off thread during compacting GCs, in which case
  // nothing will be running on the main thread.
  MOZ_ASSERT(!TlsContext.get()->inUnsafeCallWithABI);
  return generation() != zoneFromAnyThread()->types.generation;
}
||||||| merged common ancestors
inline bool
ObjectGroup::needsSweep()
{
    // Note: this can be called off thread during compacting GCs, in which case
    // nothing will be running on the main thread.
    MOZ_ASSERT(!TlsContext.get()->inUnsafeCallWithABI);
    return generation() != zoneFromAnyThread()->types.generation;
}
=======
namespace js {
>>>>>>> upstream-releases

<<<<<<< HEAD
inline ObjectGroupFlags ObjectGroup::flags(const AutoSweepObjectGroup& sweep) {
  MOZ_ASSERT(sweep.group() == this);
  return flagsDontCheckGeneration();
||||||| merged common ancestors
inline ObjectGroupFlags
ObjectGroup::flags(const AutoSweepObjectGroup& sweep)
{
    MOZ_ASSERT(sweep.group() == this);
    return flagsDontCheckGeneration();
=======
inline bool ObjectGroup::needsSweep() {
  // Note: this can be called off thread during compacting GCs, in which case
  // nothing will be running on the main thread.
  MOZ_ASSERT(!TlsContext.get()->inUnsafeCallWithABI);
  return generation() != zoneFromAnyThread()->types.generation;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline void ObjectGroup::addFlags(const AutoSweepObjectGroup& sweep,
                                  ObjectGroupFlags flags) {
  MOZ_ASSERT(sweep.group() == this);
  flags_ |= flags;
||||||| merged common ancestors
inline void
ObjectGroup::addFlags(const AutoSweepObjectGroup& sweep, ObjectGroupFlags flags)
{
    MOZ_ASSERT(sweep.group() == this);
    flags_ |= flags;
=======
inline ObjectGroupFlags ObjectGroup::flags(const AutoSweepObjectGroup& sweep) {
  MOZ_ASSERT(sweep.group() == this);
  return flagsDontCheckGeneration();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline void ObjectGroup::clearFlags(const AutoSweepObjectGroup& sweep,
                                    ObjectGroupFlags flags) {
  MOZ_ASSERT(sweep.group() == this);
  flags_ &= ~flags;
||||||| merged common ancestors
inline void
ObjectGroup::clearFlags(const AutoSweepObjectGroup& sweep, ObjectGroupFlags flags)
{
    MOZ_ASSERT(sweep.group() == this);
    flags_ &= ~flags;
=======
inline void ObjectGroup::addFlags(const AutoSweepObjectGroup& sweep,
                                  ObjectGroupFlags flags) {
  MOZ_ASSERT(sweep.group() == this);
  flags_ |= flags;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool ObjectGroup::hasAnyFlags(const AutoSweepObjectGroup& sweep,
                                     ObjectGroupFlags flags) {
  MOZ_ASSERT((flags & OBJECT_FLAG_DYNAMIC_MASK) == flags);
  return !!(this->flags(sweep) & flags);
||||||| merged common ancestors
inline bool
ObjectGroup::hasAnyFlags(const AutoSweepObjectGroup& sweep, ObjectGroupFlags flags)
{
    MOZ_ASSERT((flags & OBJECT_FLAG_DYNAMIC_MASK) == flags);
    return !!(this->flags(sweep) & flags);
=======
inline void ObjectGroup::clearFlags(const AutoSweepObjectGroup& sweep,
                                    ObjectGroupFlags flags) {
  MOZ_ASSERT(sweep.group() == this);
  flags_ &= ~flags;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool ObjectGroup::hasAllFlags(const AutoSweepObjectGroup& sweep,
                                     ObjectGroupFlags flags) {
  MOZ_ASSERT((flags & OBJECT_FLAG_DYNAMIC_MASK) == flags);
  return (this->flags(sweep) & flags) == flags;
||||||| merged common ancestors
inline bool
ObjectGroup::hasAllFlags(const AutoSweepObjectGroup& sweep, ObjectGroupFlags flags)
{
    MOZ_ASSERT((flags & OBJECT_FLAG_DYNAMIC_MASK) == flags);
    return (this->flags(sweep) & flags) == flags;
=======
inline bool ObjectGroup::hasAnyFlags(const AutoSweepObjectGroup& sweep,
                                     ObjectGroupFlags flags) {
  MOZ_ASSERT((flags & OBJECT_FLAG_DYNAMIC_MASK) == flags);
  return !!(this->flags(sweep) & flags);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool ObjectGroup::unknownProperties(const AutoSweepObjectGroup& sweep) {
  MOZ_ASSERT_IF(flags(sweep) & OBJECT_FLAG_UNKNOWN_PROPERTIES,
                hasAllFlags(sweep, OBJECT_FLAG_DYNAMIC_MASK));
  return !!(flags(sweep) & OBJECT_FLAG_UNKNOWN_PROPERTIES);
||||||| merged common ancestors
inline bool
ObjectGroup::unknownProperties(const AutoSweepObjectGroup& sweep)
{
    MOZ_ASSERT_IF(flags(sweep) & OBJECT_FLAG_UNKNOWN_PROPERTIES,
                  hasAllFlags(sweep, OBJECT_FLAG_DYNAMIC_MASK));
    return !!(flags(sweep) & OBJECT_FLAG_UNKNOWN_PROPERTIES);
=======
inline bool ObjectGroup::hasAllFlags(const AutoSweepObjectGroup& sweep,
                                     ObjectGroupFlags flags) {
  MOZ_ASSERT((flags & OBJECT_FLAG_DYNAMIC_MASK) == flags);
  return (this->flags(sweep) & flags) == flags;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool ObjectGroup::shouldPreTenure(const AutoSweepObjectGroup& sweep) {
  return hasAnyFlags(sweep, OBJECT_FLAG_PRE_TENURE) &&
         !unknownProperties(sweep);
||||||| merged common ancestors
inline bool
ObjectGroup::shouldPreTenure(const AutoSweepObjectGroup& sweep)
{
    return hasAnyFlags(sweep, OBJECT_FLAG_PRE_TENURE) && !unknownProperties(sweep);
=======
inline bool ObjectGroup::unknownProperties(const AutoSweepObjectGroup& sweep) {
  MOZ_ASSERT_IF(flags(sweep) & OBJECT_FLAG_UNKNOWN_PROPERTIES,
                hasAllFlags(sweep, OBJECT_FLAG_DYNAMIC_MASK));
  return !!(flags(sweep) & OBJECT_FLAG_UNKNOWN_PROPERTIES);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool ObjectGroup::canPreTenure(const AutoSweepObjectGroup& sweep) {
  return !unknownProperties(sweep);
||||||| merged common ancestors
inline bool
ObjectGroup::canPreTenure(const AutoSweepObjectGroup& sweep)
{
    return !unknownProperties(sweep);
=======
inline bool ObjectGroup::shouldPreTenure(const AutoSweepObjectGroup& sweep) {
  MOZ_ASSERT(sweep.group() == this);
  return shouldPreTenureDontCheckGeneration();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool ObjectGroup::fromAllocationSite(const AutoSweepObjectGroup& sweep) {
  return flags(sweep) & OBJECT_FLAG_FROM_ALLOCATION_SITE;
||||||| merged common ancestors
inline bool
ObjectGroup::fromAllocationSite(const AutoSweepObjectGroup& sweep)
{
    return flags(sweep) & OBJECT_FLAG_FROM_ALLOCATION_SITE;
=======
inline bool ObjectGroup::shouldPreTenureDontCheckGeneration() {
  return hasAnyFlagsDontCheckGeneration(OBJECT_FLAG_PRE_TENURE) &&
         !unknownPropertiesDontCheckGeneration();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline void ObjectGroup::setShouldPreTenure(const AutoSweepObjectGroup& sweep,
                                            JSContext* cx) {
  MOZ_ASSERT(canPreTenure(sweep));
  setFlags(sweep, cx, OBJECT_FLAG_PRE_TENURE);
||||||| merged common ancestors
inline void
ObjectGroup::setShouldPreTenure(const AutoSweepObjectGroup& sweep, JSContext* cx)
{
    MOZ_ASSERT(canPreTenure(sweep));
    setFlags(sweep, cx, OBJECT_FLAG_PRE_TENURE);
=======
inline bool ObjectGroup::canPreTenure(const AutoSweepObjectGroup& sweep) {
  return !unknownProperties(sweep);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline TypeNewScript* ObjectGroup::newScript(
    const AutoSweepObjectGroup& sweep) {
  MOZ_ASSERT(sweep.group() == this);
  return newScriptDontCheckGeneration();
||||||| merged common ancestors
inline TypeNewScript*
ObjectGroup::newScript(const AutoSweepObjectGroup& sweep)
{
    MOZ_ASSERT(sweep.group() == this);
    return newScriptDontCheckGeneration();
=======
inline bool ObjectGroup::fromAllocationSite(const AutoSweepObjectGroup& sweep) {
  return flags(sweep) & OBJECT_FLAG_FROM_ALLOCATION_SITE;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline PreliminaryObjectArrayWithTemplate* ObjectGroup::maybePreliminaryObjects(
    const AutoSweepObjectGroup& sweep) {
  MOZ_ASSERT(sweep.group() == this);
  return maybePreliminaryObjectsDontCheckGeneration();
||||||| merged common ancestors
inline PreliminaryObjectArrayWithTemplate*
ObjectGroup::maybePreliminaryObjects(const AutoSweepObjectGroup& sweep)
{
    MOZ_ASSERT(sweep.group() == this);
    return maybePreliminaryObjectsDontCheckGeneration();
=======
inline void ObjectGroup::setShouldPreTenure(const AutoSweepObjectGroup& sweep,
                                            JSContext* cx) {
  MOZ_ASSERT(canPreTenure(sweep));
  setFlags(sweep, cx, OBJECT_FLAG_PRE_TENURE);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline UnboxedLayout* ObjectGroup::maybeUnboxedLayout(
    const AutoSweepObjectGroup& sweep) {
  MOZ_ASSERT(sweep.group() == this);
  return maybeUnboxedLayoutDontCheckGeneration();
||||||| merged common ancestors
inline UnboxedLayout*
ObjectGroup::maybeUnboxedLayout(const AutoSweepObjectGroup& sweep)
{
    MOZ_ASSERT(sweep.group() == this);
    return maybeUnboxedLayoutDontCheckGeneration();
=======
inline TypeNewScript* ObjectGroup::newScript(
    const AutoSweepObjectGroup& sweep) {
  MOZ_ASSERT(sweep.group() == this);
  return newScriptDontCheckGeneration();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline UnboxedLayout& ObjectGroup::unboxedLayout(
    const AutoSweepObjectGroup& sweep) {
  MOZ_ASSERT(sweep.group() == this);
  return unboxedLayoutDontCheckGeneration();
||||||| merged common ancestors
inline UnboxedLayout&
ObjectGroup::unboxedLayout(const AutoSweepObjectGroup& sweep)
{
    MOZ_ASSERT(sweep.group() == this);
    return unboxedLayoutDontCheckGeneration();
=======
inline PreliminaryObjectArrayWithTemplate* ObjectGroup::maybePreliminaryObjects(
    const AutoSweepObjectGroup& sweep) {
  MOZ_ASSERT(sweep.group() == this);
  return maybePreliminaryObjectsDontCheckGeneration();
>>>>>>> upstream-releases
}

}  // namespace js

#endif /* vm_ObjectGroup_inl_h */
