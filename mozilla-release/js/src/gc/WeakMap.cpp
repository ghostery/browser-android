/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "gc/WeakMap-inl.h"

#include <string.h>

#include "jsapi.h"
#include "jsfriendapi.h"

#include "gc/PublicIterators.h"
#include "js/Wrapper.h"
#include "vm/GlobalObject.h"
#include "vm/JSContext.h"
#include "vm/JSObject.h"

#include "vm/JSObject-inl.h"

using namespace js;
using namespace js::gc;

WeakMapBase::WeakMapBase(JSObject* memOf, Zone* zone)
<<<<<<< HEAD
    : memberOf(memOf), zone_(zone), marked(false) {
  MOZ_ASSERT_IF(memberOf, memberOf->compartment()->zone() == zone);
||||||| merged common ancestors
  : memberOf(memOf),
    zone_(zone),
    marked(false)
{
    MOZ_ASSERT_IF(memberOf, memberOf->compartment()->zone() == zone);
=======
    : memberOf(memOf), zone_(zone), marked(false), markColor(MarkColor::Black) {
  MOZ_ASSERT_IF(memberOf, memberOf->compartment()->zone() == zone);
>>>>>>> upstream-releases
}

WeakMapBase::~WeakMapBase() {
  MOZ_ASSERT(CurrentThreadIsGCSweeping() || CurrentThreadCanAccessZone(zone_));
}

void WeakMapBase::unmarkZone(JS::Zone* zone) {
  for (WeakMapBase* m : zone->gcWeakMapList()) {
    m->marked = false;
  }
}

<<<<<<< HEAD
void WeakMapBase::traceZone(JS::Zone* zone, JSTracer* tracer) {
  MOZ_ASSERT(tracer->weakMapAction() != DoNotTraceWeakMaps);
  for (WeakMapBase* m : zone->gcWeakMapList()) {
    m->trace(tracer);
    TraceNullableEdge(tracer, &m->memberOf, "memberOf");
  }
||||||| merged common ancestors
void
WeakMapBase::traceZone(JS::Zone* zone, JSTracer* tracer)
{
    MOZ_ASSERT(tracer->weakMapAction() != DoNotTraceWeakMaps);
    for (WeakMapBase* m : zone->gcWeakMapList()) {
        m->trace(tracer);
        TraceNullableEdge(tracer, &m->memberOf, "memberOf");
    }
=======
void WeakMapBase::traceZone(JS::Zone* zone, JSTracer* tracer) {
  MOZ_ASSERT(tracer->weakMapAction() != DoNotTraceWeakMaps);
  for (WeakMapBase* m : zone->gcWeakMapList()) {
    m->trace(tracer);
    TraceNullableEdge(tracer, &m->memberOf, "memberOf");
  }
}

#if defined(JS_GC_ZEAL) || defined(DEBUG)
bool WeakMapBase::checkMarkingForZone(JS::Zone* zone) {
  // This is called at the end of marking.
  MOZ_ASSERT(zone->isGCMarking());

  bool ok = true;
  for (WeakMapBase* m : zone->gcWeakMapList()) {
    if (m->marked && !m->checkMarking()) {
      ok = false;
    }
  }

  return ok;
>>>>>>> upstream-releases
}
#endif

<<<<<<< HEAD
bool WeakMapBase::markZoneIteratively(JS::Zone* zone, GCMarker* marker) {
  bool markedAny = false;
  for (WeakMapBase* m : zone->gcWeakMapList()) {
    if (m->marked && m->markIteratively(marker)) {
      markedAny = true;
||||||| merged common ancestors
bool
WeakMapBase::markZoneIteratively(JS::Zone* zone, GCMarker* marker)
{
    bool markedAny = false;
    for (WeakMapBase* m : zone->gcWeakMapList()) {
        if (m->marked && m->markIteratively(marker)) {
            markedAny = true;
        }
=======
bool WeakMapBase::markZoneIteratively(JS::Zone* zone, GCMarker* marker) {
  bool markedAny = false;
  for (WeakMapBase* m : zone->gcWeakMapList()) {
    if (m->marked && m->markEntries(marker)) {
      markedAny = true;
>>>>>>> upstream-releases
    }
  }
  return markedAny;
}

<<<<<<< HEAD
bool WeakMapBase::findInterZoneEdges(JS::Zone* zone) {
  for (WeakMapBase* m : zone->gcWeakMapList()) {
    if (!m->findZoneEdges()) {
      return false;
||||||| merged common ancestors
bool
WeakMapBase::findInterZoneEdges(JS::Zone* zone)
{
    for (WeakMapBase* m : zone->gcWeakMapList()) {
        if (!m->findZoneEdges()) {
            return false;
        }
=======
bool WeakMapBase::findSweepGroupEdges(JS::Zone* zone) {
  for (WeakMapBase* m : zone->gcWeakMapList()) {
    if (!m->findZoneEdges()) {
      return false;
>>>>>>> upstream-releases
    }
  }
  return true;
}

void WeakMapBase::sweepZone(JS::Zone* zone) {
  for (WeakMapBase* m = zone->gcWeakMapList().getFirst(); m;) {
    WeakMapBase* next = m->getNext();
    if (m->marked) {
      m->sweep();
    } else {
      m->clearAndCompact();
      m->removeFrom(zone->gcWeakMapList());
    }
    m = next;
  }

#ifdef DEBUG
  for (WeakMapBase* m : zone->gcWeakMapList()) {
    MOZ_ASSERT(m->isInList() && m->marked);
  }
#endif
}

void WeakMapBase::traceAllMappings(WeakMapTracer* tracer) {
  JSRuntime* rt = tracer->runtime;
  for (ZonesIter zone(rt, SkipAtoms); !zone.done(); zone.next()) {
    for (WeakMapBase* m : zone->gcWeakMapList()) {
      // The WeakMapTracer callback is not allowed to GC.
      JS::AutoSuppressGCAnalysis nogc;
      m->traceMappings(tracer);
    }
  }
}

bool WeakMapBase::saveZoneMarkedWeakMaps(JS::Zone* zone,
                                         WeakMapSet& markedWeakMaps) {
  for (WeakMapBase* m : zone->gcWeakMapList()) {
    if (m->marked && !markedWeakMaps.put(m)) {
      return false;
    }
<<<<<<< HEAD
  }
  return true;
}

void WeakMapBase::restoreMarkedWeakMaps(WeakMapSet& markedWeakMaps) {
  for (WeakMapSet::Range r = markedWeakMaps.all(); !r.empty(); r.popFront()) {
    WeakMapBase* map = r.front();
    MOZ_ASSERT(map->zone()->isGCMarking());
    MOZ_ASSERT(!map->marked);
    map->marked = true;
  }
}

bool ObjectValueMap::findZoneEdges() {
  /*
   * For unmarked weakmap keys with delegates in a different zone, add a zone
   * edge to ensure that the delegate zone finishes marking before the key
   * zone.
   */
  JS::AutoSuppressGCAnalysis nogc;
  for (Range r = all(); !r.empty(); r.popFront()) {
    JSObject* key = r.front().key();
    if (key->asTenured().isMarkedBlack()) {
      continue;
    }
    JSObject* delegate = getDelegate(key);
    if (!delegate) {
      continue;
    }
    Zone* delegateZone = delegate->zone();
    if (delegateZone == zone() || !delegateZone->isGCMarking()) {
      continue;
    }
    if (!delegateZone->gcSweepGroupEdges().put(key->zone())) {
      return false;
||||||| merged common ancestors
}

bool
ObjectValueMap::findZoneEdges()
{
    /*
     * For unmarked weakmap keys with delegates in a different zone, add a zone
     * edge to ensure that the delegate zone finishes marking before the key
     * zone.
     */
    JS::AutoSuppressGCAnalysis nogc;
    for (Range r = all(); !r.empty(); r.popFront()) {
        JSObject* key = r.front().key();
        if (key->asTenured().isMarkedBlack()) {
            continue;
        }
        JSObject* delegate = getDelegate(key);
        if (!delegate) {
            continue;
        }
        Zone* delegateZone = delegate->zone();
        if (delegateZone == zone() || !delegateZone->isGCMarking()) {
            continue;
        }
        if (!delegateZone->gcSweepGroupEdges().put(key->zone())) {
            return false;
        }
=======
  }
  return true;
}

void WeakMapBase::restoreMarkedWeakMaps(WeakMapSet& markedWeakMaps) {
  for (WeakMapSet::Range r = markedWeakMaps.all(); !r.empty(); r.popFront()) {
    WeakMapBase* map = r.front();
    MOZ_ASSERT(map->zone()->isGCMarking());
    MOZ_ASSERT(!map->marked);
    map->marked = true;
  }
}

size_t ObjectValueMap::sizeOfIncludingThis(mozilla::MallocSizeOf mallocSizeOf) {
  return mallocSizeOf(this) + shallowSizeOfExcludingThis(mallocSizeOf);
}

bool ObjectValueMap::findZoneEdges() {
  /*
   * For unmarked weakmap keys with delegates in a different zone, add a zone
   * edge to ensure that the delegate zone finishes marking before the key
   * zone.
   */
  JS::AutoSuppressGCAnalysis nogc;
  for (Range r = all(); !r.empty(); r.popFront()) {
    JSObject* key = r.front().key();
    if (key->asTenured().isMarkedBlack()) {
      continue;
    }
    JSObject* delegate = getDelegate(key);
    if (!delegate) {
      continue;
    }
    Zone* delegateZone = delegate->zone();
    if (delegateZone == zone() || !delegateZone->isGCMarking()) {
      continue;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
  return true;
||||||| merged common ancestors
    return true;
=======
    if (!delegateZone->addSweepGroupEdgeTo(key->zone())) {
      return false;
    }
  }
  return true;
>>>>>>> upstream-releases
}

ObjectWeakMap::ObjectWeakMap(JSContext* cx) : map(cx, nullptr) {}

JSObject* ObjectWeakMap::lookup(const JSObject* obj) {
  if (ObjectValueMap::Ptr p = map.lookup(const_cast<JSObject*>(obj))) {
    return &p->value().toObject();
  }
  return nullptr;
}

bool ObjectWeakMap::add(JSContext* cx, JSObject* obj, JSObject* target) {
  MOZ_ASSERT(obj && target);

<<<<<<< HEAD
  MOZ_ASSERT(!map.has(obj));
  if (!map.put(obj, ObjectValue(*target))) {
    ReportOutOfMemory(cx);
    return false;
  }
||||||| merged common ancestors
    MOZ_ASSERT(!map.has(obj));
    if (!map.put(obj, ObjectValue(*target))) {
        ReportOutOfMemory(cx);
        return false;
    }
=======
  Value targetVal(ObjectValue(*target));
  if (!map.putNew(obj, targetVal)) {
    ReportOutOfMemory(cx);
    return false;
  }
>>>>>>> upstream-releases

  return true;
}

void ObjectWeakMap::clear() { map.clear(); }

void ObjectWeakMap::trace(JSTracer* trc) { map.trace(trc); }

size_t ObjectWeakMap::sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf) {
  return map.shallowSizeOfExcludingThis(mallocSizeOf);
}

#ifdef JSGC_HASH_TABLE_CHECKS
void ObjectWeakMap::checkAfterMovingGC() {
  for (ObjectValueMap::Range r = map.all(); !r.empty(); r.popFront()) {
    CheckGCThingAfterMovingGC(r.front().key().get());
    CheckGCThingAfterMovingGC(&r.front().value().toObject());
  }
}
#endif  // JSGC_HASH_TABLE_CHECKS
