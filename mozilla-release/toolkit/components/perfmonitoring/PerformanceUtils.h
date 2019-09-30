/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PerformanceUtils_h
#define PerformanceUtils_h

<<<<<<< HEAD
#include "mozilla/PerformanceTypes.h"
||||||| merged common ancestors
#include "mozilla/dom/DOMTypes.h"   // defines PerformanceInfo
=======
#include "mozilla/PerformanceTypes.h"

class nsPIDOMWindowOuter;
>>>>>>> upstream-releases

namespace mozilla {

/**
<<<<<<< HEAD
 * Returns an array of promises to asynchronously collect all performance
 * info in the current process.
||||||| merged common ancestors
 * Collects all performance info in the current process
 * and adds then in the aMetrics array
=======
 * Returns an array of promises to asynchronously collect all performance
 * info in the current process.
 */
nsTArray<RefPtr<PerformanceInfoPromise>> CollectPerformanceInfo();

/**
 * Asynchronously collects memory info for a given window
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
nsTArray<RefPtr<PerformanceInfoPromise>> CollectPerformanceInfo();
||||||| merged common ancestors
void CollectPerformanceInfo(nsTArray<dom::PerformanceInfo>& aMetrics);
=======
RefPtr<MemoryPromise> CollectMemoryInfo(
    const nsCOMPtr<nsPIDOMWindowOuter>& aWindow,
    const RefPtr<AbstractThread>& aEventTarget);
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Asynchronously collects memory info for a given window
 */
RefPtr<MemoryPromise> CollectMemoryInfo(
    const nsCOMPtr<nsPIDOMWindowOuter>& aWindow,
    const RefPtr<AbstractThread>& aEventTarget);

}  // namespace mozilla
#endif  // PerformanceUtils_h
||||||| merged common ancestors
} // namespace mozilla
#endif   // PerformanceUtils_h
=======
}  // namespace mozilla
#endif  // PerformanceUtils_h
>>>>>>> upstream-releases
