/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/ArrayUtils.h"
#include "mozilla/UniquePtr.h"

#include "jsapi-tests/tests.h"

static unsigned gSliceCallbackCount = 0;

<<<<<<< HEAD
static void NonIncrementalGCSliceCallback(JSContext* cx,
                                          JS::GCProgress progress,
                                          const JS::GCDescription& desc) {
  using namespace JS;
  static GCProgress expect[] = {GC_CYCLE_BEGIN, GC_SLICE_BEGIN, GC_SLICE_END,
                                GC_CYCLE_END};

  MOZ_RELEASE_ASSERT(gSliceCallbackCount < mozilla::ArrayLength(expect));
  MOZ_RELEASE_ASSERT(progress == expect[gSliceCallbackCount++]);
  MOZ_RELEASE_ASSERT(desc.isZone_ == false);
  MOZ_RELEASE_ASSERT(desc.invocationKind_ == GC_NORMAL);
  MOZ_RELEASE_ASSERT(desc.reason_ == JS::gcreason::API);
  if (progress == GC_CYCLE_END) {
    mozilla::UniquePtr<char16_t> summary(desc.formatSummaryMessage(cx));
    mozilla::UniquePtr<char16_t> message(desc.formatSliceMessage(cx));
    mozilla::UniquePtr<char16_t> json(desc.formatJSON(cx, 0));
  }
||||||| merged common ancestors
static void
NonIncrementalGCSliceCallback(JSContext* cx, JS::GCProgress progress, const JS::GCDescription& desc)
{
    using namespace JS;
    static GCProgress expect[] =
        { GC_CYCLE_BEGIN, GC_SLICE_BEGIN, GC_SLICE_END, GC_CYCLE_END };

    MOZ_RELEASE_ASSERT(gSliceCallbackCount < mozilla::ArrayLength(expect));
    MOZ_RELEASE_ASSERT(progress == expect[gSliceCallbackCount++]);
    MOZ_RELEASE_ASSERT(desc.isZone_ == false);
    MOZ_RELEASE_ASSERT(desc.invocationKind_ == GC_NORMAL);
    MOZ_RELEASE_ASSERT(desc.reason_ == JS::gcreason::API);
    if (progress == GC_CYCLE_END) {
        mozilla::UniquePtr<char16_t> summary(desc.formatSummaryMessage(cx));
        mozilla::UniquePtr<char16_t> message(desc.formatSliceMessage(cx));
        mozilla::UniquePtr<char16_t> json(desc.formatJSON(cx, 0));
    }
=======
static void NonIncrementalGCSliceCallback(JSContext* cx,
                                          JS::GCProgress progress,
                                          const JS::GCDescription& desc) {
  using namespace JS;
  static GCProgress expect[] = {GC_CYCLE_BEGIN, GC_SLICE_BEGIN, GC_SLICE_END,
                                GC_CYCLE_END};

  MOZ_RELEASE_ASSERT(gSliceCallbackCount < mozilla::ArrayLength(expect));
  MOZ_RELEASE_ASSERT(progress == expect[gSliceCallbackCount++]);
  MOZ_RELEASE_ASSERT(desc.isZone_ == false);
  MOZ_RELEASE_ASSERT(desc.invocationKind_ == GC_NORMAL);
  MOZ_RELEASE_ASSERT(desc.reason_ == JS::GCReason::API);
  if (progress == GC_CYCLE_END) {
    mozilla::UniquePtr<char16_t> summary(desc.formatSummaryMessage(cx));
    mozilla::UniquePtr<char16_t> message(desc.formatSliceMessage(cx));
    mozilla::UniquePtr<char16_t> json(desc.formatJSONTelemetry(cx, 0));
  }
>>>>>>> upstream-releases
}

BEGIN_TEST(testGCSliceCallback) {
  gSliceCallbackCount = 0;
  JS::SetGCSliceCallback(cx, NonIncrementalGCSliceCallback);
  JS_GC(cx);
  JS::SetGCSliceCallback(cx, nullptr);
  CHECK(gSliceCallbackCount == 4);
  return true;
}
END_TEST(testGCSliceCallback)

<<<<<<< HEAD
static void RootsRemovedGCSliceCallback(JSContext* cx, JS::GCProgress progress,
                                        const JS::GCDescription& desc) {
  using namespace JS;
  using namespace JS::gcreason;

  static GCProgress expectProgress[] = {
      GC_CYCLE_BEGIN, GC_SLICE_BEGIN, GC_SLICE_END,   GC_SLICE_BEGIN,
      GC_SLICE_END,   GC_CYCLE_END,   GC_CYCLE_BEGIN, GC_SLICE_BEGIN,
      GC_SLICE_END,   GC_CYCLE_END};

  static Reason expectReasons[] = {
      DEBUG_GC, DEBUG_GC,      DEBUG_GC,      DEBUG_GC,      DEBUG_GC,
      DEBUG_GC, ROOTS_REMOVED, ROOTS_REMOVED, ROOTS_REMOVED, ROOTS_REMOVED};

  static_assert(
      mozilla::ArrayLength(expectProgress) ==
          mozilla::ArrayLength(expectReasons),
      "expectProgress and expectReasons arrays should be the same length");

  MOZ_RELEASE_ASSERT(gSliceCallbackCount <
                     mozilla::ArrayLength(expectProgress));
  MOZ_RELEASE_ASSERT(progress == expectProgress[gSliceCallbackCount]);
  MOZ_RELEASE_ASSERT(desc.isZone_ == false);
  MOZ_RELEASE_ASSERT(desc.invocationKind_ == GC_SHRINK);
  MOZ_RELEASE_ASSERT(desc.reason_ == expectReasons[gSliceCallbackCount]);
  gSliceCallbackCount++;
||||||| merged common ancestors
static void
RootsRemovedGCSliceCallback(JSContext* cx, JS::GCProgress progress, const JS::GCDescription& desc)
{
    using namespace JS;
    using namespace JS::gcreason;

    static GCProgress expectProgress[] = {
        GC_CYCLE_BEGIN, GC_SLICE_BEGIN, GC_SLICE_END, GC_SLICE_BEGIN, GC_SLICE_END, GC_CYCLE_END,
        GC_CYCLE_BEGIN, GC_SLICE_BEGIN, GC_SLICE_END, GC_CYCLE_END
    };

    static Reason expectReasons[] = {
        DEBUG_GC, DEBUG_GC, DEBUG_GC, DEBUG_GC, DEBUG_GC, DEBUG_GC,
        ROOTS_REMOVED, ROOTS_REMOVED, ROOTS_REMOVED, ROOTS_REMOVED
    };

    static_assert(mozilla::ArrayLength(expectProgress) == mozilla::ArrayLength(expectReasons),
                  "expectProgress and expectReasons arrays should be the same length");

    MOZ_RELEASE_ASSERT(gSliceCallbackCount < mozilla::ArrayLength(expectProgress));
    MOZ_RELEASE_ASSERT(progress == expectProgress[gSliceCallbackCount]);
    MOZ_RELEASE_ASSERT(desc.isZone_ == false);
    MOZ_RELEASE_ASSERT(desc.invocationKind_ == GC_SHRINK);
    MOZ_RELEASE_ASSERT(desc.reason_ == expectReasons[gSliceCallbackCount]);
    gSliceCallbackCount++;
=======
static void RootsRemovedGCSliceCallback(JSContext* cx, JS::GCProgress progress,
                                        const JS::GCDescription& desc) {
  using namespace JS;

  static GCProgress expectProgress[] = {
      GC_CYCLE_BEGIN, GC_SLICE_BEGIN, GC_SLICE_END,   GC_SLICE_BEGIN,
      GC_SLICE_END,   GC_CYCLE_END,   GC_CYCLE_BEGIN, GC_SLICE_BEGIN,
      GC_SLICE_END,   GC_CYCLE_END};

  static GCReason expectReasons[] = {
      GCReason::DEBUG_GC,      GCReason::DEBUG_GC,      GCReason::DEBUG_GC,
      GCReason::DEBUG_GC,      GCReason::DEBUG_GC,      GCReason::DEBUG_GC,
      GCReason::ROOTS_REMOVED, GCReason::ROOTS_REMOVED, GCReason::ROOTS_REMOVED,
      GCReason::ROOTS_REMOVED};

  static_assert(
      mozilla::ArrayLength(expectProgress) ==
          mozilla::ArrayLength(expectReasons),
      "expectProgress and expectReasons arrays should be the same length");

  MOZ_RELEASE_ASSERT(gSliceCallbackCount <
                     mozilla::ArrayLength(expectProgress));
  MOZ_RELEASE_ASSERT(progress == expectProgress[gSliceCallbackCount]);
  MOZ_RELEASE_ASSERT(desc.isZone_ == false);
  MOZ_RELEASE_ASSERT(desc.invocationKind_ == GC_SHRINK);
  MOZ_RELEASE_ASSERT(desc.reason_ == expectReasons[gSliceCallbackCount]);
  gSliceCallbackCount++;
>>>>>>> upstream-releases
}

BEGIN_TEST(testGCRootsRemoved) {
#ifdef JS_GC_ZEAL
  AutoLeaveZeal nozeal(cx);
#endif /* JS_GC_ZEAL */

<<<<<<< HEAD
  JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_INCREMENTAL);
||||||| merged common ancestors
    JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_INCREMENTAL);
=======
  JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_ZONE_INCREMENTAL);
>>>>>>> upstream-releases

  gSliceCallbackCount = 0;
  JS::SetGCSliceCallback(cx, RootsRemovedGCSliceCallback);

  JS::RootedObject obj(cx, JS_NewPlainObject(cx));
  CHECK(obj);

  JS::PrepareForFullGC(cx);
  js::SliceBudget budget(js::WorkBudget(1));
  cx->runtime()->gc.startDebugGC(GC_SHRINK, budget);
  CHECK(JS::IsIncrementalGCInProgress(cx));

  // Trigger another GC after the current one in shrinking / shutdown GCs.
  cx->runtime()->gc.notifyRootsRemoved();

<<<<<<< HEAD
  JS::FinishIncrementalGC(cx, JS::gcreason::DEBUG_GC);
  CHECK(!JS::IsIncrementalGCInProgress(cx));
||||||| merged common ancestors
    JS::FinishIncrementalGC(cx, JS::gcreason::DEBUG_GC);
    CHECK(!JS::IsIncrementalGCInProgress(cx));
=======
  JS::FinishIncrementalGC(cx, JS::GCReason::DEBUG_GC);
  CHECK(!JS::IsIncrementalGCInProgress(cx));
>>>>>>> upstream-releases

  JS::SetGCSliceCallback(cx, nullptr);

  JS_SetGCParameter(cx, JSGC_MODE, JSGC_MODE_GLOBAL);

  return true;
}
END_TEST(testGCRootsRemoved)
