/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrGLSemaphore.h"

#include "GrGLGpu.h"

<<<<<<< HEAD
GrGLSemaphore::GrGLSemaphore(GrGLGpu* gpu, bool isOwned)
        : INHERITED(gpu), fSync(0), fIsOwned(isOwned) {
    isOwned ? this->registerWithCache(SkBudgeted::kNo) : this->registerWithCacheWrapped();
||||||| merged common ancestors
GrGLSemaphore::GrGLSemaphore(const GrGLGpu* gpu, bool isOwned)
    : INHERITED(gpu), fSync(0), fIsOwned(isOwned) {
=======
GrGLSemaphore::GrGLSemaphore(GrGLGpu* gpu, bool isOwned)
        : INHERITED(gpu), fSync(0), fIsOwned(isOwned) {
    isOwned ? this->registerWithCache(SkBudgeted::kNo)
            : this->registerWithCacheWrapped(GrWrapCacheable::kNo);
>>>>>>> upstream-releases
}

void GrGLSemaphore::onRelease() {
    if (fSync && fIsOwned) {
        static_cast<const GrGLGpu*>(this->getGpu())->deleteSync(fSync);
    }
    fSync = 0;
    INHERITED::onRelease();
}

void GrGLSemaphore::onAbandon() {
    fSync = 0;
    INHERITED::onAbandon();
}
