/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrSemaphoreOp_DEFINED
#define GrSemaphoreOp_DEFINED

#include "GrOp.h"

#include "GrRenderTargetProxy.h"
#include "GrSemaphore.h"
#include "SkRefCnt.h"

class GrRecordingContext;

class GrSemaphoreOp : public GrOp {
public:
<<<<<<< HEAD
    static std::unique_ptr<GrOp> MakeSignal(GrContext*,
                                            sk_sp<GrSemaphore>,
                                            GrRenderTargetProxy*,
                                            bool forceFlush);

    static std::unique_ptr<GrOp> MakeWait(GrContext*,
                                          sk_sp<GrSemaphore>,
                                          GrRenderTargetProxy*);
||||||| merged common ancestors
    static std::unique_ptr<GrSemaphoreOp> MakeSignal(sk_sp<GrSemaphore> semaphore,
                                                     GrRenderTargetProxy* proxy,
                                                     bool forceFlush);

    static std::unique_ptr<GrSemaphoreOp> MakeWait(sk_sp<GrSemaphore> semaphore,
                                                   GrRenderTargetProxy* proxy);
=======
    static std::unique_ptr<GrOp> MakeWait(GrRecordingContext*,
                                          sk_sp<GrSemaphore>,
                                          GrRenderTargetProxy*);
>>>>>>> upstream-releases

protected:
    GrSemaphoreOp(uint32_t classId, sk_sp<GrSemaphore> semaphore, GrRenderTargetProxy* proxy)
            : INHERITED(classId)
            , fSemaphore(std::move(semaphore)) {
        this->makeFullScreen(proxy);
    }

    sk_sp<GrSemaphore> fSemaphore;

private:
    void onPrepare(GrOpFlushState*) override {}

    typedef GrOp INHERITED;
};

#endif
