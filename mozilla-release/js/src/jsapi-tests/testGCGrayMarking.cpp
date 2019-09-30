/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "gc/Heap.h"
#include "gc/Verifier.h"
#include "gc/WeakMap.h"
#include "gc/Zone.h"
#include "js/Proxy.h"
#include "jsapi-tests/tests.h"

using namespace js;
using namespace js::gc;

namespace js {

struct GCManagedObjectWeakMap : public ObjectWeakMap {
  using ObjectWeakMap::ObjectWeakMap;
};

}  // namespace js

namespace JS {

template <>
struct DeletePolicy<js::GCManagedObjectWeakMap>
    : public js::GCManagedDeletePolicy<js::GCManagedObjectWeakMap> {};

template <>
struct MapTypeToRootKind<js::GCManagedObjectWeakMap*> {
  static const JS::RootKind kind = JS::RootKind::Traceable;
};

template <>
struct GCPolicy<js::GCManagedObjectWeakMap*>
    : public NonGCPointerPolicy<js::GCManagedObjectWeakMap*> {};

}  // namespace JS

class AutoNoAnalysisForTest {
 public:
  AutoNoAnalysisForTest() {}
} JS_HAZ_GC_SUPPRESSED;

BEGIN_TEST(testGCGrayMarking) {
  AutoNoAnalysisForTest disableAnalysis;
  AutoDisableCompactingGC disableCompactingGC(cx);
#ifdef JS_GC_ZEAL
  AutoLeaveZeal nozeal(cx);
#endif /* JS_GC_ZEAL */

  CHECK(InitGlobals());
  JSAutoRealm ar(cx, global1);

  InitGrayRootTracer();

<<<<<<< HEAD
  bool ok = TestMarking() && TestWeakMaps() && TestUnassociatedWeakMaps() &&
            TestCCWs() && TestGrayUnmarking();
||||||| merged common ancestors
    bool ok =
        TestMarking() &&
        TestWeakMaps() &&
        TestUnassociatedWeakMaps() &&
        TestCCWs() &&
        TestGrayUnmarking();
=======
  // Enable incremental GC.
  JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_ZONE_INCREMENTAL);
>>>>>>> upstream-releases

<<<<<<< HEAD
  global1 = nullptr;
  global2 = nullptr;
  RemoveGrayRootTracer();
||||||| merged common ancestors
    global1 = nullptr;
    global2 = nullptr;
    RemoveGrayRootTracer();
=======
  bool ok = TestMarking() && TestJSWeakMaps() && TestInternalWeakMaps() &&
            TestCCWs() && TestGrayUnmarking();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return ok;
||||||| merged common ancestors
    return ok;
=======
  JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_GLOBAL);

  global1 = nullptr;
  global2 = nullptr;
  RemoveGrayRootTracer();

  return ok;
>>>>>>> upstream-releases
}

bool TestMarking() {
  JSObject* sameTarget = AllocPlainObject();
  CHECK(sameTarget);

  JSObject* sameSource = AllocSameCompartmentSourceObject(sameTarget);
  CHECK(sameSource);

  JSObject* crossTarget = AllocPlainObject();
  CHECK(crossTarget);

  JSObject* crossSource = GetCrossCompartmentWrapper(crossTarget);
  CHECK(crossSource);

  // Test GC with black roots marks objects black.

  JS::RootedObject blackRoot1(cx, sameSource);
  JS::RootedObject blackRoot2(cx, crossSource);

  JS_GC(cx);

  CHECK(IsMarkedBlack(sameSource));
  CHECK(IsMarkedBlack(crossSource));
  CHECK(IsMarkedBlack(sameTarget));
  CHECK(IsMarkedBlack(crossTarget));

  // Test GC with black and gray roots marks objects black.

  grayRoots.grayRoot1 = sameSource;
  grayRoots.grayRoot2 = crossSource;

  JS_GC(cx);

  CHECK(IsMarkedBlack(sameSource));
  CHECK(IsMarkedBlack(crossSource));
  CHECK(IsMarkedBlack(sameTarget));
  CHECK(IsMarkedBlack(crossTarget));

  CHECK(!JS::ObjectIsMarkedGray(sameSource));

  // Test GC with gray roots marks object gray.

  blackRoot1 = nullptr;
  blackRoot2 = nullptr;

  JS_GC(cx);

  CHECK(IsMarkedGray(sameSource));
  CHECK(IsMarkedGray(crossSource));
  CHECK(IsMarkedGray(sameTarget));
  CHECK(IsMarkedGray(crossTarget));

  CHECK(JS::ObjectIsMarkedGray(sameSource));

  // Test ExposeToActiveJS marks gray objects black.

  JS::ExposeObjectToActiveJS(sameSource);
  JS::ExposeObjectToActiveJS(crossSource);
  CHECK(IsMarkedBlack(sameSource));
  CHECK(IsMarkedBlack(crossSource));
  CHECK(IsMarkedBlack(sameTarget));
  CHECK(IsMarkedBlack(crossTarget));

  // Test a zone GC with black roots marks gray object in other zone black.

  JS_GC(cx);

  CHECK(IsMarkedGray(crossSource));
  CHECK(IsMarkedGray(crossTarget));

  blackRoot1 = crossSource;
  CHECK(ZoneGC(crossSource->zone()));

  CHECK(IsMarkedBlack(crossSource));
  CHECK(IsMarkedBlack(crossTarget));

  blackRoot1 = nullptr;
  blackRoot2 = nullptr;
  grayRoots.grayRoot1 = nullptr;
  grayRoots.grayRoot2 = nullptr;

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
}

bool
TestWeakMaps()
{
    JSObject* weakMap = JS::NewWeakMapObject(cx);
    CHECK(weakMap);

    JSObject* key;
    JSObject* value;
    {
        JS::RootedObject rootedMap(cx, weakMap);

        key = AllocWeakmapKeyObject();
        CHECK(key);

        value = AllocPlainObject();
        CHECK(value);

        weakMap = rootedMap;
    }

    {
        JS::RootedObject rootedMap(cx, weakMap);
        JS::RootedObject rootedKey(cx, key);
        JS::RootedValue rootedValue(cx, ObjectValue(*value));
        CHECK(SetWeakMapEntry(cx, rootedMap, rootedKey, rootedValue));
    }

    // Test the value of a weakmap entry is marked gray by GC if both the
    // weakmap and key are marked gray.

    grayRoots.grayRoot1 = weakMap;
    grayRoots.grayRoot2 = key;
    JS_GC(cx);
    CHECK(IsMarkedGray(weakMap));
    CHECK(IsMarkedGray(key));
    CHECK(IsMarkedGray(value));

    // Test the value of a weakmap entry is marked gray by GC if one of the
    // weakmap and the key is marked gray and the other black.

    JS::RootedObject blackRoot1(cx);
    blackRoot1 = weakMap;
    grayRoots.grayRoot1 = key;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedBlack(weakMap));
    CHECK(IsMarkedGray(key));
    CHECK(IsMarkedGray(value));

    blackRoot1 = key;
    grayRoots.grayRoot1 = weakMap;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedGray(weakMap));
    CHECK(IsMarkedBlack(key));
    CHECK(IsMarkedGray(value));

    // Test the value of a weakmap entry is marked black by GC if both the
    // weakmap and the key are marked black.

    JS::RootedObject blackRoot2(cx);
    grayRoots.grayRoot1 = nullptr;
    grayRoots.grayRoot2 = nullptr;

    blackRoot1 = weakMap;
    blackRoot2 = key;
    JS_GC(cx);
    CHECK(IsMarkedBlack(weakMap));
    CHECK(IsMarkedBlack(key));
    CHECK(IsMarkedBlack(value));

    blackRoot1 = key;
    blackRoot2 = weakMap;
    JS_GC(cx);
    CHECK(IsMarkedBlack(weakMap));
    CHECK(IsMarkedBlack(key));
    CHECK(IsMarkedBlack(value));

    // Test that a weakmap key is marked gray if it has a gray delegate and the
    // map is either gray or black.

    JSObject* delegate = AllocDelegateForKey(key);
    blackRoot1 = weakMap;
    blackRoot2 = nullptr;
    grayRoots.grayRoot1 = delegate;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedGray(delegate));
    CHECK(IsMarkedGray(key));
    CHECK(IsMarkedBlack(weakMap));
    CHECK(IsMarkedGray(value));

    blackRoot1 = nullptr;
    blackRoot2 = nullptr;
    grayRoots.grayRoot1 = weakMap;
    grayRoots.grayRoot2 = delegate;
    JS_GC(cx);
    CHECK(IsMarkedGray(delegate));
    CHECK(IsMarkedGray(key));
    CHECK(IsMarkedGray(weakMap));
    CHECK(IsMarkedGray(value));

    // Test that a weakmap key is marked gray if it has a black delegate but
    // the map is gray.

    blackRoot1 = delegate;
    blackRoot2 = nullptr;
    grayRoots.grayRoot1 = weakMap;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedBlack(delegate));
    CHECK(IsMarkedGray(key));
    CHECK(IsMarkedGray(weakMap));
    CHECK(IsMarkedGray(value));

    blackRoot1 = delegate;
    blackRoot2 = nullptr;
    grayRoots.grayRoot1 = weakMap;
    grayRoots.grayRoot2 = key;
    JS_GC(cx);
    CHECK(IsMarkedBlack(delegate));
    CHECK(IsMarkedGray(key));
    CHECK(IsMarkedGray(weakMap));
    CHECK(IsMarkedGray(value));

    // Test that a weakmap key is marked black if it has a black delegate and
    // the map is black.

    blackRoot1 = delegate;
    blackRoot2 = weakMap;
    grayRoots.grayRoot1 = nullptr;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedBlack(delegate));
    CHECK(IsMarkedBlack(key));
    CHECK(IsMarkedBlack(weakMap));
    CHECK(IsMarkedBlack(value));

    blackRoot1 = delegate;
    blackRoot2 = weakMap;
    grayRoots.grayRoot1 = key;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedBlack(delegate));
    CHECK(IsMarkedBlack(key));
    CHECK(IsMarkedBlack(weakMap));
    CHECK(IsMarkedBlack(value));

    // Test what happens if there is a delegate but it is not marked for both
    // black and gray cases.

    delegate = nullptr;
    blackRoot1 = key;
    blackRoot2 = weakMap;
    grayRoots.grayRoot1 = nullptr;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedBlack(key));
    CHECK(IsMarkedBlack(weakMap));
    CHECK(IsMarkedBlack(value));

    CHECK(AllocDelegateForKey(key));
    blackRoot1 = nullptr;
    blackRoot2 = nullptr;
    grayRoots.grayRoot1 = weakMap;
    grayRoots.grayRoot2 = key;
    JS_GC(cx);
    CHECK(IsMarkedGray(key));
    CHECK(IsMarkedGray(weakMap));
    CHECK(IsMarkedGray(value));

    blackRoot1 = nullptr;
    blackRoot2 = nullptr;
    grayRoots.grayRoot1 = nullptr;
    grayRoots.grayRoot2 = nullptr;

    return true;
=======
  return true;
}

static const CellColor DontMark = CellColor::White;

enum MarkKeyOrDelegate : bool { MarkKey = true, MarkDelegate = false };

bool TestJSWeakMaps() {
  for (auto keyOrDelegateColor : MarkedCellColors) {
    for (auto mapColor : MarkedCellColors) {
      for (auto markKeyOrDelegate : {MarkKey, MarkDelegate}) {
        CellColor expected =
            ExpectedWeakMapValueColor(keyOrDelegateColor, mapColor);
        CHECK(TestJSWeakMap(markKeyOrDelegate, keyOrDelegateColor, mapColor,
                            expected));
#ifdef JS_GC_ZEAL
        CHECK(TestJSWeakMapWithGrayUnmarking(
            markKeyOrDelegate, keyOrDelegateColor, mapColor, expected));
#endif
      }
    }
  }

  return true;
}

bool TestInternalWeakMaps() {
  for (auto keyMarkColor : AllCellColors) {
    for (auto delegateMarkColor : AllCellColors) {
      if (keyMarkColor == CellColor::White &&
          delegateMarkColor == CellColor::White) {
        continue;
      }

      CellColor keyOrDelegateColor =
          ExpectedKeyAndDelegateColor(keyMarkColor, delegateMarkColor);
      CellColor expected =
          ExpectedWeakMapValueColor(keyOrDelegateColor, CellColor::Black);
      CHECK(TestInternalWeakMap(keyMarkColor, delegateMarkColor, expected));

#ifdef JS_GC_ZEAL
      CHECK(TestInternalWeakMapWithGrayUnmarking(keyMarkColor,
                                                 delegateMarkColor, expected));
#endif
    }
  }

  return true;
}

bool TestJSWeakMap(MarkKeyOrDelegate markKey, CellColor weakMapMarkColor,
                   CellColor keyOrDelegateMarkColor,
                   CellColor expectedValueColor) {
  // Test marking a JS WeakMap object.
  //
  // This marks the map and one of the key or delegate. The key/delegate and the
  // value can end up different colors depending on the color of the map.

  JSObject* weakMap;
  JSObject* key;
  JSObject* value;

  // If both map and key are marked the same color, test both possible
  // orderings.
  unsigned markOrderings = weakMapMarkColor == keyOrDelegateMarkColor ? 2 : 1;

  for (unsigned markOrder = 0; markOrder < markOrderings; markOrder++) {
    CHECK(CreateJSWeakMapObjects(&weakMap, &key, &value));

    JSObject* delegate = UncheckedUnwrapWithoutExpose(key);
    JSObject* keyOrDelegate = markKey ? key : delegate;

    RootedObject blackRoot1(cx);
    RootedObject blackRoot2(cx);

    RootObject(weakMap, weakMapMarkColor, blackRoot1, grayRoots.grayRoot1);
    RootObject(keyOrDelegate, keyOrDelegateMarkColor, blackRoot2,
               grayRoots.grayRoot2);

    if (markOrder != 0) {
      mozilla::Swap(blackRoot1.get(), blackRoot2.get());
      mozilla::Swap(grayRoots.grayRoot1, grayRoots.grayRoot2);
    }

    JS_GC(cx);

    ClearGrayRoots();

    CHECK(GetCellColor(weakMap) == weakMapMarkColor);
    CHECK(GetCellColor(keyOrDelegate) == keyOrDelegateMarkColor);
    CHECK(GetCellColor(value) == expectedValueColor);
  }

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool TestWeakMaps() {
  JSObject* weakMap = JS::NewWeakMapObject(cx);
  CHECK(weakMap);
||||||| merged common ancestors
bool
TestUnassociatedWeakMaps()
{
    // Make a weakmap that's not associated with a JSObject.
    auto weakMap = cx->make_unique<GCManagedObjectWeakMap>(cx);
    CHECK(weakMap);
=======
#ifdef JS_GC_ZEAL
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSObject* key;
  JSObject* value;
  {
    JS::RootedObject rootedMap(cx, weakMap);
||||||| merged common ancestors
    // Make sure this gets traced during GC.
    Rooted<GCManagedObjectWeakMap*> rootMap(cx, weakMap.get());
=======
bool TestJSWeakMapWithGrayUnmarking(MarkKeyOrDelegate markKey,
                                    CellColor weakMapMarkColor,
                                    CellColor keyOrDelegateMarkColor,
                                    CellColor expectedValueColor) {
  // This is like the previous test, but things are marked black by gray
  // unmarking during incremental GC.
>>>>>>> upstream-releases

<<<<<<< HEAD
    key = AllocWeakmapKeyObject();
    CHECK(key);
||||||| merged common ancestors
    JSObject* key = AllocWeakmapKeyObject();
    CHECK(key);
=======
  JSObject* weakMap;
  JSObject* key;
  JSObject* value;
>>>>>>> upstream-releases

<<<<<<< HEAD
    value = AllocPlainObject();
    CHECK(value);
||||||| merged common ancestors
    JSObject* value = AllocPlainObject();
    CHECK(value);
=======
  // If both map and key are marked the same color, test both possible
  // orderings.
  unsigned markOrderings = weakMapMarkColor == keyOrDelegateMarkColor ? 2 : 1;
>>>>>>> upstream-releases

<<<<<<< HEAD
    weakMap = rootedMap;
  }

  {
    JS::RootedObject rootedMap(cx, weakMap);
    JS::RootedObject rootedKey(cx, key);
    JS::RootedValue rootedValue(cx, ObjectValue(*value));
    CHECK(SetWeakMapEntry(cx, rootedMap, rootedKey, rootedValue));
  }

  // Test the value of a weakmap entry is marked gray by GC if both the
  // weakmap and key are marked gray.

  grayRoots.grayRoot1 = weakMap;
  grayRoots.grayRoot2 = key;
  JS_GC(cx);
  CHECK(IsMarkedGray(weakMap));
  CHECK(IsMarkedGray(key));
  CHECK(IsMarkedGray(value));

  // Test the value of a weakmap entry is marked gray by GC if one of the
  // weakmap and the key is marked gray and the other black.

  JS::RootedObject blackRoot1(cx);
  blackRoot1 = weakMap;
  grayRoots.grayRoot1 = key;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedBlack(weakMap));
  CHECK(IsMarkedGray(key));
  CHECK(IsMarkedGray(value));

  blackRoot1 = key;
  grayRoots.grayRoot1 = weakMap;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedGray(weakMap));
  CHECK(IsMarkedBlack(key));
  CHECK(IsMarkedGray(value));

  // Test the value of a weakmap entry is marked black by GC if both the
  // weakmap and the key are marked black.

  JS::RootedObject blackRoot2(cx);
  grayRoots.grayRoot1 = nullptr;
  grayRoots.grayRoot2 = nullptr;

  blackRoot1 = weakMap;
  blackRoot2 = key;
  JS_GC(cx);
  CHECK(IsMarkedBlack(weakMap));
  CHECK(IsMarkedBlack(key));
  CHECK(IsMarkedBlack(value));

  blackRoot1 = key;
  blackRoot2 = weakMap;
  JS_GC(cx);
  CHECK(IsMarkedBlack(weakMap));
  CHECK(IsMarkedBlack(key));
  CHECK(IsMarkedBlack(value));

  // Test that a weakmap key is marked gray if it has a gray delegate and the
  // map is either gray or black.

  JSObject* delegate = AllocDelegateForKey(key);
  blackRoot1 = weakMap;
  blackRoot2 = nullptr;
  grayRoots.grayRoot1 = delegate;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedGray(delegate));
  CHECK(IsMarkedGray(key));
  CHECK(IsMarkedBlack(weakMap));
  CHECK(IsMarkedGray(value));

  blackRoot1 = nullptr;
  blackRoot2 = nullptr;
  grayRoots.grayRoot1 = weakMap;
  grayRoots.grayRoot2 = delegate;
  JS_GC(cx);
  CHECK(IsMarkedGray(delegate));
  CHECK(IsMarkedGray(key));
  CHECK(IsMarkedGray(weakMap));
  CHECK(IsMarkedGray(value));

  // Test that a weakmap key is marked gray if it has a black delegate but
  // the map is gray.

  blackRoot1 = delegate;
  blackRoot2 = nullptr;
  grayRoots.grayRoot1 = weakMap;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedBlack(delegate));
  CHECK(IsMarkedGray(key));
  CHECK(IsMarkedGray(weakMap));
  CHECK(IsMarkedGray(value));

  blackRoot1 = delegate;
  blackRoot2 = nullptr;
  grayRoots.grayRoot1 = weakMap;
  grayRoots.grayRoot2 = key;
  JS_GC(cx);
  CHECK(IsMarkedBlack(delegate));
  CHECK(IsMarkedGray(key));
  CHECK(IsMarkedGray(weakMap));
  CHECK(IsMarkedGray(value));

  // Test that a weakmap key is marked black if it has a black delegate and
  // the map is black.

  blackRoot1 = delegate;
  blackRoot2 = weakMap;
  grayRoots.grayRoot1 = nullptr;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedBlack(delegate));
  CHECK(IsMarkedBlack(key));
  CHECK(IsMarkedBlack(weakMap));
  CHECK(IsMarkedBlack(value));

  blackRoot1 = delegate;
  blackRoot2 = weakMap;
  grayRoots.grayRoot1 = key;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedBlack(delegate));
  CHECK(IsMarkedBlack(key));
  CHECK(IsMarkedBlack(weakMap));
  CHECK(IsMarkedBlack(value));

  // Test what happens if there is a delegate but it is not marked for both
  // black and gray cases.

  delegate = nullptr;
  blackRoot1 = key;
  blackRoot2 = weakMap;
  grayRoots.grayRoot1 = nullptr;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedBlack(key));
  CHECK(IsMarkedBlack(weakMap));
  CHECK(IsMarkedBlack(value));

  CHECK(AllocDelegateForKey(key));
  blackRoot1 = nullptr;
  blackRoot2 = nullptr;
  grayRoots.grayRoot1 = weakMap;
  grayRoots.grayRoot2 = key;
  JS_GC(cx);
  CHECK(IsMarkedGray(key));
  CHECK(IsMarkedGray(weakMap));
  CHECK(IsMarkedGray(value));

  blackRoot1 = nullptr;
  blackRoot2 = nullptr;
  grayRoots.grayRoot1 = nullptr;
  grayRoots.grayRoot2 = nullptr;

  return true;
||||||| merged common ancestors
    CHECK(weakMap->add(cx, key, value));

    // Test the value of a weakmap entry is marked gray by GC if the
    // key is marked gray.

    grayRoots.grayRoot1 = key;
    JS_GC(cx);
    CHECK(IsMarkedGray(key));
    CHECK(IsMarkedGray(value));

    // Test the value of a weakmap entry is marked gray by GC if the key is marked gray.

    grayRoots.grayRoot1 = key;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedGray(key));
    CHECK(IsMarkedGray(value));

    // Test the value of a weakmap entry is marked black by GC if the key is
    // marked black.

    JS::RootedObject blackRoot(cx);
    blackRoot = key;
    grayRoots.grayRoot1 = nullptr;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedBlack(key));
    CHECK(IsMarkedBlack(value));

    // Test that a weakmap key is marked gray if it has a gray delegate.

    JSObject* delegate = AllocDelegateForKey(key);
    blackRoot = nullptr;
    grayRoots.grayRoot1 = delegate;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedGray(delegate));
    CHECK(IsMarkedGray(key));
    CHECK(IsMarkedGray(value));

    // Test that a weakmap key is marked black if it has a black delegate.

    blackRoot = delegate;
    grayRoots.grayRoot1 = nullptr;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedBlack(delegate));
    CHECK(IsMarkedBlack(key));
    CHECK(IsMarkedBlack(value));

    blackRoot = delegate;
    grayRoots.grayRoot1 = key;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedBlack(delegate));
    CHECK(IsMarkedBlack(key));
    CHECK(IsMarkedBlack(value));

    // Test what happens if there is a delegate but it is not marked for both
    // black and gray cases.

    delegate = nullptr;
    blackRoot = key;
    grayRoots.grayRoot1 = nullptr;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedBlack(key));
    CHECK(IsMarkedBlack(value));

    CHECK(AllocDelegateForKey(key));
    blackRoot = nullptr;
    grayRoots.grayRoot1 = key;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedGray(key));
    CHECK(IsMarkedGray(value));

    blackRoot = nullptr;
    grayRoots.grayRoot1 = nullptr;
    grayRoots.grayRoot2 = nullptr;

    return true;
=======
  JS_SetGCZeal(cx, uint8_t(ZealMode::YieldWhileGrayMarking), 0);

  for (unsigned markOrder = 0; markOrder < markOrderings; markOrder++) {
    CHECK(CreateJSWeakMapObjects(&weakMap, &key, &value));

    JSObject* delegate = UncheckedUnwrapWithoutExpose(key);
    JSObject* keyOrDelegate = markKey ? key : delegate;

    grayRoots.grayRoot1 = keyOrDelegate;
    grayRoots.grayRoot2 = weakMap;

    // Start an incremental GC and run until gray roots have been pushed onto
    // the mark stack.
    JS::PrepareForFullGC(cx);
    JS::StartIncrementalGC(cx, GC_NORMAL, JS::GCReason::DEBUG_GC, 1000000);
    MOZ_ASSERT(cx->runtime()->gc.state() == gc::State::Sweep);
    MOZ_ASSERT(cx->zone()->gcState() == Zone::MarkBlackAndGray);

    // Unmark gray things as specified.
    if (markOrder != 0) {
      MaybeExposeObject(weakMap, weakMapMarkColor);
      MaybeExposeObject(keyOrDelegate, keyOrDelegateMarkColor);
    } else {
      MaybeExposeObject(keyOrDelegate, keyOrDelegateMarkColor);
      MaybeExposeObject(weakMap, weakMapMarkColor);
    }

    JS::FinishIncrementalGC(cx, JS::GCReason::API);

    ClearGrayRoots();

    CHECK(GetCellColor(weakMap) == weakMapMarkColor);
    CHECK(GetCellColor(keyOrDelegate) == keyOrDelegateMarkColor);
    CHECK(GetCellColor(value) == expectedValueColor);
  }

  JS_UnsetGCZeal(cx, uint8_t(ZealMode::YieldWhileGrayMarking));

  return true;
}

static void MaybeExposeObject(JSObject* object, CellColor color) {
  if (color == CellColor::Black) {
    JS::ExposeObjectToActiveJS(object);
  }
}

#endif  // JS_GC_ZEAL

bool CreateJSWeakMapObjects(JSObject** weakMapOut, JSObject** keyOut,
                            JSObject** valueOut) {
  RootedObject key(cx, AllocWeakmapKeyObject());
  CHECK(key);

  RootedObject value(cx, AllocPlainObject());
  CHECK(value);

  RootedObject weakMap(cx, JS::NewWeakMapObject(cx));
  CHECK(weakMap);

  JS::RootedValue valueValue(cx, ObjectValue(*value));
  CHECK(SetWeakMapEntry(cx, weakMap, key, valueValue));

  *weakMapOut = weakMap;
  *keyOut = key;
  *valueOut = value;
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool TestUnassociatedWeakMaps() {
  // Make a weakmap that's not associated with a JSObject.
  auto weakMap = cx->make_unique<GCManagedObjectWeakMap>(cx);
  CHECK(weakMap);

  // Make sure this gets traced during GC.
  Rooted<GCManagedObjectWeakMap*> rootMap(cx, weakMap.get());

  JSObject* key = AllocWeakmapKeyObject();
  CHECK(key);

  JSObject* value = AllocPlainObject();
  CHECK(value);

  CHECK(weakMap->add(cx, key, value));

  // Test the value of a weakmap entry is marked gray by GC if the
  // key is marked gray.

  grayRoots.grayRoot1 = key;
  JS_GC(cx);
  CHECK(IsMarkedGray(key));
  CHECK(IsMarkedGray(value));

  // Test the value of a weakmap entry is marked gray by GC if the key is marked
  // gray.

  grayRoots.grayRoot1 = key;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedGray(key));
  CHECK(IsMarkedGray(value));

  // Test the value of a weakmap entry is marked black by GC if the key is
  // marked black.

  JS::RootedObject blackRoot(cx);
  blackRoot = key;
  grayRoots.grayRoot1 = nullptr;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedBlack(key));
  CHECK(IsMarkedBlack(value));

  // Test that a weakmap key is marked gray if it has a gray delegate.

  JSObject* delegate = AllocDelegateForKey(key);
  blackRoot = nullptr;
  grayRoots.grayRoot1 = delegate;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedGray(delegate));
  CHECK(IsMarkedGray(key));
  CHECK(IsMarkedGray(value));

  // Test that a weakmap key is marked black if it has a black delegate.

  blackRoot = delegate;
  grayRoots.grayRoot1 = nullptr;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedBlack(delegate));
  CHECK(IsMarkedBlack(key));
  CHECK(IsMarkedBlack(value));

  blackRoot = delegate;
  grayRoots.grayRoot1 = key;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedBlack(delegate));
  CHECK(IsMarkedBlack(key));
  CHECK(IsMarkedBlack(value));

  // Test what happens if there is a delegate but it is not marked for both
  // black and gray cases.

  delegate = nullptr;
  blackRoot = key;
  grayRoots.grayRoot1 = nullptr;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedBlack(key));
  CHECK(IsMarkedBlack(value));

  CHECK(AllocDelegateForKey(key));
  blackRoot = nullptr;
  grayRoots.grayRoot1 = key;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedGray(key));
  CHECK(IsMarkedGray(value));

  blackRoot = nullptr;
  grayRoots.grayRoot1 = nullptr;
  grayRoots.grayRoot2 = nullptr;

  return true;
}
||||||| merged common ancestors
bool
TestCCWs()
{
    JSObject* target = AllocPlainObject();
    CHECK(target);

    // Test getting a new wrapper doesn't return a gray wrapper.

    RootedObject blackRoot(cx, target);
    JSObject* wrapper = GetCrossCompartmentWrapper(target);
    CHECK(wrapper);
    CHECK(!IsMarkedGray(wrapper));

    // Test getting an existing wrapper doesn't return a gray wrapper.

    grayRoots.grayRoot1 = wrapper;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedGray(wrapper));
    CHECK(IsMarkedBlack(target));

    CHECK(GetCrossCompartmentWrapper(target) == wrapper);
    CHECK(!IsMarkedGray(wrapper));

    // Test getting an existing wrapper doesn't return a gray wrapper
    // during incremental GC.

    JS_GC(cx);
    CHECK(IsMarkedGray(wrapper));
    CHECK(IsMarkedBlack(target));

    JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_INCREMENTAL);
    JS::PrepareForFullGC(cx);
    js::SliceBudget budget(js::WorkBudget(1));
    cx->runtime()->gc.startDebugGC(GC_NORMAL, budget);
    CHECK(JS::IsIncrementalGCInProgress(cx));

    CHECK(!IsMarkedBlack(wrapper));
    CHECK(wrapper->zone()->isGCMarkingBlack());

    CHECK(GetCrossCompartmentWrapper(target) == wrapper);
    CHECK(IsMarkedBlack(wrapper));

    JS::FinishIncrementalGC(cx, JS::gcreason::API);

    // Test behaviour of gray CCWs marked black by a barrier during incremental
    // GC.

    // Initial state: source and target are gray.
    blackRoot = nullptr;
    grayRoots.grayRoot1 = wrapper;
    grayRoots.grayRoot2 = nullptr;
    JS_GC(cx);
    CHECK(IsMarkedGray(wrapper));
    CHECK(IsMarkedGray(target));

    // Incremental zone GC started: the source is now unmarked.
    JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_INCREMENTAL);
    JS::PrepareZoneForGC(wrapper->zone());
    budget = js::SliceBudget(js::WorkBudget(1));
    cx->runtime()->gc.startDebugGC(GC_NORMAL, budget);
    CHECK(JS::IsIncrementalGCInProgress(cx));
    CHECK(wrapper->zone()->isGCMarkingBlack());
    CHECK(!target->zone()->wasGCStarted());
    CHECK(!IsMarkedBlack(wrapper));
    CHECK(!IsMarkedGray(wrapper));
    CHECK(IsMarkedGray(target));

    // Betweeen GC slices: source marked black by barrier, target is still gray.
    // ObjectIsMarkedGray() and CheckObjectIsNotMarkedGray() should handle this
    // case and report that target is not marked gray.
    grayRoots.grayRoot1.get();
    CHECK(IsMarkedBlack(wrapper));
    CHECK(IsMarkedGray(target));
    CHECK(!JS::ObjectIsMarkedGray(target));
    MOZ_ASSERT(JS::ObjectIsNotGray(target));

    // Final state: source and target are black.
    JS::FinishIncrementalGC(cx, JS::gcreason::API);
    CHECK(IsMarkedBlack(wrapper));
    CHECK(IsMarkedBlack(target));

    grayRoots.grayRoot1 = nullptr;
    grayRoots.grayRoot2 = nullptr;
=======
bool TestInternalWeakMap(CellColor keyMarkColor, CellColor delegateMarkColor,
                         CellColor expectedColor) {
  // Test marking for internal weakmaps (without an owning JSObject).
  //
  // All of the key, delegate and value are expected to end up the same color.

  UniquePtr<GCManagedObjectWeakMap> weakMap;
  JSObject* key;
  JSObject* value;

  // If both key and delegate are marked the same color, test both possible
  // orderings.
  unsigned markOrderings = keyMarkColor == delegateMarkColor ? 2 : 1;

  for (unsigned markOrder = 0; markOrder < markOrderings; markOrder++) {
    CHECK(CreateInternalWeakMapObjects(&weakMap, &key, &value));

    JSObject* delegate = UncheckedUnwrapWithoutExpose(key);

    RootedObject blackRoot1(cx);
    RootedObject blackRoot2(cx);

    Rooted<GCManagedObjectWeakMap*> rootMap(cx, weakMap.get());
    RootObject(key, keyMarkColor, blackRoot1, grayRoots.grayRoot1);
    RootObject(delegate, delegateMarkColor, blackRoot2, grayRoots.grayRoot2);

    if (markOrder != 0) {
      mozilla::Swap(blackRoot1.get(), blackRoot2.get());
      mozilla::Swap(grayRoots.grayRoot1, grayRoots.grayRoot2);
    }

    JS_GC(cx);

    ClearGrayRoots();

    CHECK(GetCellColor(key) == expectedColor);
    CHECK(GetCellColor(delegate) == expectedColor);
    CHECK(GetCellColor(value) == expectedColor);
  }

  return true;
}

#ifdef JS_GC_ZEAL

bool TestInternalWeakMapWithGrayUnmarking(CellColor keyMarkColor,
                                          CellColor delegateMarkColor,
                                          CellColor expectedColor) {
  UniquePtr<GCManagedObjectWeakMap> weakMap;
  JSObject* key;
  JSObject* value;

  // If both key and delegate are marked the same color, test both possible
  // orderings.
  unsigned markOrderings = keyMarkColor == delegateMarkColor ? 2 : 1;

  JS_SetGCZeal(cx, uint8_t(ZealMode::YieldWhileGrayMarking), 0);

  for (unsigned markOrder = 0; markOrder < markOrderings; markOrder++) {
    CHECK(CreateInternalWeakMapObjects(&weakMap, &key, &value));

    JSObject* delegate = UncheckedUnwrapWithoutExpose(key);

    Rooted<GCManagedObjectWeakMap*> rootMap(cx, weakMap.get());
    grayRoots.grayRoot1 = key;
    grayRoots.grayRoot2 = delegate;

    // Start an incremental GC and run until gray roots have been pushed onto
    // the mark stack.
    JS::PrepareForFullGC(cx);
    JS::StartIncrementalGC(cx, GC_NORMAL, JS::GCReason::DEBUG_GC, 1000000);
    MOZ_ASSERT(cx->runtime()->gc.state() == gc::State::Sweep);
    MOZ_ASSERT(cx->zone()->gcState() == Zone::MarkBlackAndGray);

    // Unmark gray things as specified.
    if (markOrder != 0) {
      MaybeExposeObject(key, keyMarkColor);
      MaybeExposeObject(delegate, delegateMarkColor);
    } else {
      MaybeExposeObject(key, keyMarkColor);
      MaybeExposeObject(delegate, delegateMarkColor);
    }

    JS::FinishIncrementalGC(cx, JS::GCReason::API);

    ClearGrayRoots();

    CHECK(GetCellColor(key) == expectedColor);
    CHECK(GetCellColor(delegate) == expectedColor);
    CHECK(GetCellColor(value) == expectedColor);
  }

  JS_UnsetGCZeal(cx, uint8_t(ZealMode::YieldWhileGrayMarking));

  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool TestCCWs() {
  JSObject* target = AllocPlainObject();
  CHECK(target);

  // Test getting a new wrapper doesn't return a gray wrapper.

  RootedObject blackRoot(cx, target);
  JSObject* wrapper = GetCrossCompartmentWrapper(target);
  CHECK(wrapper);
  CHECK(!IsMarkedGray(wrapper));

  // Test getting an existing wrapper doesn't return a gray wrapper.

  grayRoots.grayRoot1 = wrapper;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedGray(wrapper));
  CHECK(IsMarkedBlack(target));

  CHECK(GetCrossCompartmentWrapper(target) == wrapper);
  CHECK(!IsMarkedGray(wrapper));

  // Test getting an existing wrapper doesn't return a gray wrapper
  // during incremental GC.

  JS_GC(cx);
  CHECK(IsMarkedGray(wrapper));
  CHECK(IsMarkedBlack(target));

  JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_INCREMENTAL);
  JS::PrepareForFullGC(cx);
  js::SliceBudget budget(js::WorkBudget(1));
  cx->runtime()->gc.startDebugGC(GC_NORMAL, budget);
  CHECK(JS::IsIncrementalGCInProgress(cx));

  CHECK(!IsMarkedBlack(wrapper));
  CHECK(wrapper->zone()->isGCMarkingBlack());

  CHECK(GetCrossCompartmentWrapper(target) == wrapper);
  CHECK(IsMarkedBlack(wrapper));

  JS::FinishIncrementalGC(cx, JS::gcreason::API);

  // Test behaviour of gray CCWs marked black by a barrier during incremental
  // GC.

  // Initial state: source and target are gray.
  blackRoot = nullptr;
  grayRoots.grayRoot1 = wrapper;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedGray(wrapper));
  CHECK(IsMarkedGray(target));

  // Incremental zone GC started: the source is now unmarked.
  JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_INCREMENTAL);
  JS::PrepareZoneForGC(wrapper->zone());
  budget = js::SliceBudget(js::WorkBudget(1));
  cx->runtime()->gc.startDebugGC(GC_NORMAL, budget);
  CHECK(JS::IsIncrementalGCInProgress(cx));
  CHECK(wrapper->zone()->isGCMarkingBlack());
  CHECK(!target->zone()->wasGCStarted());
  CHECK(!IsMarkedBlack(wrapper));
  CHECK(!IsMarkedGray(wrapper));
  CHECK(IsMarkedGray(target));

  // Betweeen GC slices: source marked black by barrier, target is still
  // gray. Target will be marked gray eventually. ObjectIsMarkedGray() is
  // conservative and reports that target is not marked gray; ObjectIsNotGray
  // reports the actual state.
  grayRoots.grayRoot1.get();
  CHECK(IsMarkedBlack(wrapper));
  CHECK(IsMarkedGray(target));
  CHECK(!JS::ObjectIsMarkedGray(target));
  MOZ_ASSERT(!JS::ObjectIsNotGray(target));

  // Final state: source and target are black.
  JS::FinishIncrementalGC(cx, JS::gcreason::API);
  CHECK(IsMarkedBlack(wrapper));
  CHECK(IsMarkedBlack(target));

  grayRoots.grayRoot1 = nullptr;
  grayRoots.grayRoot2 = nullptr;

  return true;
||||||| merged common ancestors
    return true;
=======
#endif  // JS_GC_ZEAL

bool CreateInternalWeakMapObjects(UniquePtr<GCManagedObjectWeakMap>* weakMapOut,
                                  JSObject** keyOut, JSObject** valueOut) {
  RootedObject key(cx, AllocWeakmapKeyObject());
  CHECK(key);

  RootedObject value(cx, AllocPlainObject());
  CHECK(value);

  auto weakMap = cx->make_unique<GCManagedObjectWeakMap>(cx);
  CHECK(weakMap);

  CHECK(weakMap->add(cx, key, value));

  *weakMapOut = std::move(weakMap);
  *keyOut = key;
  *valueOut = value;
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool TestGrayUnmarking() {
  const size_t length = 2000;
||||||| merged common ancestors
bool
TestGrayUnmarking()
{
    const size_t length = 2000;
=======
void RootObject(JSObject* object, CellColor color, RootedObject& blackRoot,
                JS::Heap<JSObject*>& grayRoot) {
  if (color == CellColor::Black) {
    blackRoot = object;
  } else if (color == CellColor::Gray) {
    grayRoot = object;
  } else {
    MOZ_RELEASE_ASSERT(color == CellColor::White);
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSObject* chain = AllocObjectChain(length);
  CHECK(chain);
||||||| merged common ancestors
    JSObject* chain = AllocObjectChain(length);
    CHECK(chain);
=======
bool TestCCWs() {
  JSObject* target = AllocPlainObject();
  CHECK(target);

  // Test getting a new wrapper doesn't return a gray wrapper.

  RootedObject blackRoot(cx, target);
  JSObject* wrapper = GetCrossCompartmentWrapper(target);
  CHECK(wrapper);
  CHECK(!IsMarkedGray(wrapper));

  // Test getting an existing wrapper doesn't return a gray wrapper.

  grayRoots.grayRoot1 = wrapper;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedGray(wrapper));
  CHECK(IsMarkedBlack(target));

  CHECK(GetCrossCompartmentWrapper(target) == wrapper);
  CHECK(!IsMarkedGray(wrapper));

  // Test getting an existing wrapper doesn't return a gray wrapper
  // during incremental GC.

  JS_GC(cx);
  CHECK(IsMarkedGray(wrapper));
  CHECK(IsMarkedBlack(target));

  JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_ZONE_INCREMENTAL);
  JS::PrepareForFullGC(cx);
  js::SliceBudget budget(js::WorkBudget(1));
  cx->runtime()->gc.startDebugGC(GC_NORMAL, budget);
  CHECK(JS::IsIncrementalGCInProgress(cx));

  CHECK(!IsMarkedBlack(wrapper));
  CHECK(wrapper->zone()->isGCMarkingBlackOnly());

  CHECK(GetCrossCompartmentWrapper(target) == wrapper);
  CHECK(IsMarkedBlack(wrapper));

  JS::FinishIncrementalGC(cx, JS::GCReason::API);

  // Test behaviour of gray CCWs marked black by a barrier during incremental
  // GC.

  // Initial state: source and target are gray.
  blackRoot = nullptr;
  grayRoots.grayRoot1 = wrapper;
  grayRoots.grayRoot2 = nullptr;
  JS_GC(cx);
  CHECK(IsMarkedGray(wrapper));
  CHECK(IsMarkedGray(target));

  // Incremental zone GC started: the source is now unmarked.
  JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_ZONE_INCREMENTAL);
  JS::PrepareZoneForGC(wrapper->zone());
  budget = js::SliceBudget(js::WorkBudget(1));
  cx->runtime()->gc.startDebugGC(GC_NORMAL, budget);
  CHECK(JS::IsIncrementalGCInProgress(cx));
  CHECK(wrapper->zone()->isGCMarkingBlackOnly());
  CHECK(!target->zone()->wasGCStarted());
  CHECK(!IsMarkedBlack(wrapper));
  CHECK(!IsMarkedGray(wrapper));
  CHECK(IsMarkedGray(target));

  // Betweeen GC slices: source marked black by barrier, target is
  // still gray. Target will be marked gray
  // eventually. ObjectIsMarkedGray() is conservative and reports
  // that target is not marked gray; AssertObjectIsNotGray() will
  // assert.
  grayRoots.grayRoot1.get();
  CHECK(IsMarkedBlack(wrapper));
  CHECK(IsMarkedGray(target));
  CHECK(!JS::ObjectIsMarkedGray(target));

  // Final state: source and target are black.
  JS::FinishIncrementalGC(cx, JS::GCReason::API);
  CHECK(IsMarkedBlack(wrapper));
  CHECK(IsMarkedBlack(target));

  grayRoots.grayRoot1 = nullptr;
  grayRoots.grayRoot2 = nullptr;

  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedObject blackRoot(cx, chain);
  JS_GC(cx);
  size_t count;
  CHECK(IterateObjectChain(chain, ColorCheckFunctor(MarkColor::Black, &count)));
  CHECK(count == length);
||||||| merged common ancestors
    RootedObject blackRoot(cx, chain);
    JS_GC(cx);
    size_t count;
    CHECK(IterateObjectChain(chain, ColorCheckFunctor(MarkColor::Black, &count)));
    CHECK(count == length);
=======
bool TestGrayUnmarking() {
  const size_t length = 2000;
>>>>>>> upstream-releases

<<<<<<< HEAD
  blackRoot = nullptr;
  grayRoots.grayRoot1 = chain;
  JS_GC(cx);
  CHECK(cx->runtime()->gc.areGrayBitsValid());
  CHECK(IterateObjectChain(chain, ColorCheckFunctor(MarkColor::Gray, &count)));
  CHECK(count == length);
||||||| merged common ancestors
    blackRoot = nullptr;
    grayRoots.grayRoot1 = chain;
    JS_GC(cx);
    CHECK(cx->runtime()->gc.areGrayBitsValid());
    CHECK(IterateObjectChain(chain, ColorCheckFunctor(MarkColor::Gray, &count)));
    CHECK(count == length);
=======
  JSObject* chain = AllocObjectChain(length);
  CHECK(chain);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS::ExposeObjectToActiveJS(chain);
  CHECK(cx->runtime()->gc.areGrayBitsValid());
  CHECK(IterateObjectChain(chain, ColorCheckFunctor(MarkColor::Black, &count)));
  CHECK(count == length);
||||||| merged common ancestors
    JS::ExposeObjectToActiveJS(chain);
    CHECK(cx->runtime()->gc.areGrayBitsValid());
    CHECK(IterateObjectChain(chain, ColorCheckFunctor(MarkColor::Black, &count)));
    CHECK(count == length);
=======
  RootedObject blackRoot(cx, chain);
  JS_GC(cx);
  size_t count;
  CHECK(IterateObjectChain(chain, ColorCheckFunctor(MarkColor::Black, &count)));
  CHECK(count == length);
>>>>>>> upstream-releases

<<<<<<< HEAD
  grayRoots.grayRoot1 = nullptr;
||||||| merged common ancestors
    grayRoots.grayRoot1 = nullptr;
=======
  blackRoot = nullptr;
  grayRoots.grayRoot1 = chain;
  JS_GC(cx);
  CHECK(cx->runtime()->gc.areGrayBitsValid());
  CHECK(IterateObjectChain(chain, ColorCheckFunctor(MarkColor::Gray, &count)));
  CHECK(count == length);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  JS::ExposeObjectToActiveJS(chain);
  CHECK(cx->runtime()->gc.areGrayBitsValid());
  CHECK(IterateObjectChain(chain, ColorCheckFunctor(MarkColor::Black, &count)));
  CHECK(count == length);

  grayRoots.grayRoot1 = nullptr;

  return true;
>>>>>>> upstream-releases
}

struct ColorCheckFunctor {
  MarkColor color;
  size_t& count;

  ColorCheckFunctor(MarkColor colorArg, size_t* countArg)
      : color(colorArg), count(*countArg) {
    count = 0;
  }

  bool operator()(JSObject* obj) {
    if (!CheckCellColor(obj, color)) {
      return false;
    }

    NativeObject& nobj = obj->as<NativeObject>();
    if (!CheckCellColor(nobj.shape(), color)) {
      return false;
    }

    Shape* shape = nobj.shape();
    if (!CheckCellColor(shape, color)) {
      return false;
    }

    // Shapes and symbols are never marked gray.
    jsid id = shape->propid();
    if (JSID_IS_GCTHING(id) &&
        !CheckCellColor(JSID_TO_GCTHING(id).asCell(), MarkColor::Black)) {
      return false;
    }

    count++;
    return true;
  }
};

JS::PersistentRootedObject global1;
JS::PersistentRootedObject global2;

struct GrayRoots {
  JS::Heap<JSObject*> grayRoot1;
  JS::Heap<JSObject*> grayRoot2;
};

GrayRoots grayRoots;

bool InitGlobals() {
  global1.init(cx, global);
  if (!createGlobal()) {
    return false;
  }
  global2.init(cx, global);
  return global2 != nullptr;
}

<<<<<<< HEAD
void InitGrayRootTracer() {
  grayRoots.grayRoot1 = nullptr;
  grayRoots.grayRoot2 = nullptr;
  JS_SetGrayGCRootsTracer(cx, TraceGrayRoots, &grayRoots);
||||||| merged common ancestors
void
InitGrayRootTracer()
{
    grayRoots.grayRoot1 = nullptr;
    grayRoots.grayRoot2 = nullptr;
    JS_SetGrayGCRootsTracer(cx, TraceGrayRoots, &grayRoots);
=======
void ClearGrayRoots() {
  grayRoots.grayRoot1 = nullptr;
  grayRoots.grayRoot2 = nullptr;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void RemoveGrayRootTracer() {
  grayRoots.grayRoot1 = nullptr;
  grayRoots.grayRoot2 = nullptr;
  JS_SetGrayGCRootsTracer(cx, nullptr, nullptr);
||||||| merged common ancestors
void
RemoveGrayRootTracer()
{
    grayRoots.grayRoot1 = nullptr;
    grayRoots.grayRoot2 = nullptr;
    JS_SetGrayGCRootsTracer(cx, nullptr, nullptr);
=======
void InitGrayRootTracer() {
  ClearGrayRoots();
  JS_SetGrayGCRootsTracer(cx, TraceGrayRoots, &grayRoots);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void TraceGrayRoots(JSTracer* trc, void* data) {
  auto grayRoots = static_cast<GrayRoots*>(data);
  TraceEdge(trc, &grayRoots->grayRoot1, "gray root 1");
  TraceEdge(trc, &grayRoots->grayRoot2, "gray root 2");
||||||| merged common ancestors
static void
TraceGrayRoots(JSTracer* trc, void* data)
{
    auto grayRoots = static_cast<GrayRoots*>(data);
    TraceEdge(trc, &grayRoots->grayRoot1, "gray root 1");
    TraceEdge(trc, &grayRoots->grayRoot2, "gray root 2");
=======
void RemoveGrayRootTracer() {
  ClearGrayRoots();
  JS_SetGrayGCRootsTracer(cx, nullptr, nullptr);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSObject* AllocPlainObject() {
  JS::RootedObject obj(cx, JS_NewPlainObject(cx));
  EvictNursery();

  MOZ_ASSERT(obj->compartment() == global1->compartment());
  return obj;
||||||| merged common ancestors
JSObject*
AllocPlainObject()
{
    JS::RootedObject obj(cx, JS_NewPlainObject(cx));
    EvictNursery();

    MOZ_ASSERT(obj->compartment() == global1->compartment());
    return obj;
=======
static void TraceGrayRoots(JSTracer* trc, void* data) {
  auto grayRoots = static_cast<GrayRoots*>(data);
  TraceEdge(trc, &grayRoots->grayRoot1, "gray root 1");
  TraceEdge(trc, &grayRoots->grayRoot2, "gray root 2");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSObject* AllocSameCompartmentSourceObject(JSObject* target) {
  JS::RootedObject source(cx, JS_NewPlainObject(cx));
  if (!source) {
    return nullptr;
  }

  JS::RootedObject obj(cx, target);
  if (!JS_DefineProperty(cx, source, "ptr", obj, 0)) {
    return nullptr;
  }

  EvictNursery();
||||||| merged common ancestors
JSObject*
AllocSameCompartmentSourceObject(JSObject* target)
{
    JS::RootedObject source(cx, JS_NewPlainObject(cx));
    if (!source) {
        return nullptr;
    }

    JS::RootedObject obj(cx, target);
    if (!JS_DefineProperty(cx, source, "ptr", obj, 0)) {
        return nullptr;
    }

    EvictNursery();
=======
JSObject* AllocPlainObject() {
  JS::RootedObject obj(cx, JS_NewPlainObject(cx));
  EvictNursery();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(source->compartment() == global1->compartment());
  return source;
||||||| merged common ancestors
    MOZ_ASSERT(source->compartment() == global1->compartment());
    return source;
=======
  MOZ_ASSERT(obj->compartment() == global1->compartment());
  return obj;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSObject* GetCrossCompartmentWrapper(JSObject* target) {
  MOZ_ASSERT(target->compartment() == global1->compartment());
  JS::RootedObject obj(cx, target);
  JSAutoRealm ar(cx, global2);
  if (!JS_WrapObject(cx, &obj)) {
    return nullptr;
  }
||||||| merged common ancestors
JSObject*
GetCrossCompartmentWrapper(JSObject* target)
{
    MOZ_ASSERT(target->compartment() == global1->compartment());
    JS::RootedObject obj(cx, target);
    JSAutoRealm ar(cx, global2);
    if (!JS_WrapObject(cx, &obj)) {
        return nullptr;
    }
=======
JSObject* AllocSameCompartmentSourceObject(JSObject* target) {
  JS::RootedObject source(cx, JS_NewPlainObject(cx));
  if (!source) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  EvictNursery();
||||||| merged common ancestors
    EvictNursery();
=======
  JS::RootedObject obj(cx, target);
  if (!JS_DefineProperty(cx, source, "ptr", obj, 0)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(obj->compartment() == global2->compartment());
  return obj;
}
||||||| merged common ancestors
    MOZ_ASSERT(obj->compartment() == global2->compartment());
    return obj;
}
=======
  EvictNursery();
>>>>>>> upstream-releases

<<<<<<< HEAD
static JSObject* GetKeyDelegate(JSObject* obj) {
  return static_cast<JSObject*>(obj->as<NativeObject>().getPrivate());
||||||| merged common ancestors
static JSObject*
GetKeyDelegate(JSObject* obj)
{
    return static_cast<JSObject*>(obj->as<NativeObject>().getPrivate());
=======
  MOZ_ASSERT(source->compartment() == global1->compartment());
  return source;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSObject* AllocWeakmapKeyObject() {
  static const js::ClassExtension KeyClassExtension = {GetKeyDelegate};
||||||| merged common ancestors
JSObject*
AllocWeakmapKeyObject()
{
    static const js::ClassExtension KeyClassExtension = {
        GetKeyDelegate
    };

    static const js::Class KeyClass = {
        "keyWithDelegate",
        JSCLASS_HAS_PRIVATE,
        JS_NULL_CLASS_OPS,
        JS_NULL_CLASS_SPEC,
        &KeyClassExtension,
        JS_NULL_OBJECT_OPS
    };

    JS::RootedObject key(cx, JS_NewObject(cx, Jsvalify(&KeyClass)));
    if (!key) {
        return nullptr;
    }
=======
JSObject* GetCrossCompartmentWrapper(JSObject* target) {
  MOZ_ASSERT(target->compartment() == global1->compartment());
  JS::RootedObject obj(cx, target);
  JSAutoRealm ar(cx, global2);
  if (!JS_WrapObject(cx, &obj)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static const js::Class KeyClass = {"keyWithDelegate",  JSCLASS_HAS_PRIVATE,
                                     JS_NULL_CLASS_OPS,  JS_NULL_CLASS_SPEC,
                                     &KeyClassExtension, JS_NULL_OBJECT_OPS};

  JS::RootedObject key(cx, JS_NewObject(cx, Jsvalify(&KeyClass)));
  if (!key) {
    return nullptr;
  }

  EvictNursery();
  return key;
||||||| merged common ancestors
    EvictNursery();
    return key;
=======
  EvictNursery();

  MOZ_ASSERT(obj->compartment() == global2->compartment());
  return obj;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSObject* AllocDelegateForKey(JSObject* key) {
  JS::RootedObject obj(cx, JS_NewPlainObject(cx));
  EvictNursery();
||||||| merged common ancestors
JSObject*
AllocDelegateForKey(JSObject* key)
{
    JS::RootedObject obj(cx, JS_NewPlainObject(cx));
    EvictNursery();
=======
JSObject* AllocWeakmapKeyObject() {
  JS::RootedObject delegate(cx, JS_NewPlainObject(cx));
  if (!delegate) {
    return nullptr;
  }

  JS::RootedObject key(cx,
                       js::Wrapper::New(cx, delegate, &js::Wrapper::singleton));
>>>>>>> upstream-releases

<<<<<<< HEAD
  key->as<NativeObject>().setPrivate(obj);
  return obj;
||||||| merged common ancestors
    key->as<NativeObject>().setPrivate(obj);
    return obj;
=======
  EvictNursery();
  return key;
>>>>>>> upstream-releases
}

JSObject* AllocObjectChain(size_t length) {
  // Allocate a chain of linked JSObjects.

  // Use a unique property name so the shape is not shared with any other
  // objects.
  RootedString nextPropName(cx, JS_NewStringCopyZ(cx, "unique14142135"));
  RootedId nextId(cx);
  if (!JS_StringToId(cx, nextPropName, &nextId)) {
    return nullptr;
  }

  RootedObject head(cx);
  for (size_t i = 0; i < length; i++) {
    RootedValue next(cx, ObjectOrNullValue(head));
    head = AllocPlainObject();
    if (!head) {
      return nullptr;
    }
    if (!JS_DefinePropertyById(cx, head, nextId, next, 0)) {
      return nullptr;
    }
  }

  return head;
}

template <typename F>
bool IterateObjectChain(JSObject* chain, F f) {
  RootedObject obj(cx, chain);
  while (obj) {
    if (!f(obj)) {
      return false;
    }

    // Access the 'next' property via the object's slots to avoid triggering
    // gray marking assertions when calling JS_GetPropertyById.
    NativeObject& nobj = obj->as<NativeObject>();
    MOZ_ASSERT(nobj.slotSpan() == 1);
    obj = nobj.getSlot(0).toObjectOrNull();
  }

  return true;
}

static bool IsMarkedBlack(Cell* cell) {
  TenuredCell* tc = &cell->asTenured();
  return tc->isMarkedBlack();
}

static bool IsMarkedGray(Cell* cell) {
  TenuredCell* tc = &cell->asTenured();
  bool isGray = tc->isMarkedGray();
  MOZ_ASSERT_IF(isGray, tc->isMarkedAny());
  return isGray;
}

static bool CheckCellColor(Cell* cell, MarkColor color) {
  MOZ_ASSERT(color == MarkColor::Black || color == MarkColor::Gray);
  if (color == MarkColor::Black && !IsMarkedBlack(cell)) {
    printf("Found non-black cell: %p\n", cell);
    return false;
  } else if (color == MarkColor::Gray && !IsMarkedGray(cell)) {
    printf("Found non-gray cell: %p\n", cell);
    return false;
  }

  return true;
}

<<<<<<< HEAD
void EvictNursery() { cx->runtime()->gc.evictNursery(); }

bool ZoneGC(JS::Zone* zone) {
  uint32_t oldMode = JS_GetGCParameter(cx, JSGC_MODE);
  JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_ZONE);
  JS::PrepareZoneForGC(zone);
  cx->runtime()->gc.gc(GC_NORMAL, JS::gcreason::API);
  CHECK(!cx->runtime()->gc.isFullGc());
  JS_SetGCParameter(cx, JSGC_MODE, oldMode);
  return true;
||||||| merged common ancestors
bool
ZoneGC(JS::Zone* zone)
{
    uint32_t oldMode = JS_GetGCParameter(cx, JSGC_MODE);
    JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_ZONE);
    JS::PrepareZoneForGC(zone);
    cx->runtime()->gc.gc(GC_NORMAL, JS::gcreason::API);
    CHECK(!cx->runtime()->gc.isFullGc());
    JS_SetGCParameter(cx, JSGC_MODE, oldMode);
    return true;
=======
void EvictNursery() { cx->runtime()->gc.evictNursery(); }

bool ZoneGC(JS::Zone* zone) {
  uint32_t oldMode = JS_GetGCParameter(cx, JSGC_MODE);
  JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_ZONE);
  JS::PrepareZoneForGC(zone);
  cx->runtime()->gc.gc(GC_NORMAL, JS::GCReason::API);
  CHECK(!cx->runtime()->gc.isFullGc());
  JS_SetGCParameter(cx, JSGC_MODE, oldMode);
  return true;
>>>>>>> upstream-releases
}

END_TEST(testGCGrayMarking)
