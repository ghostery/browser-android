/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrSemaphoreOp.h"

#include "GrContext.h"
#include "GrContextPriv.h"
#include "GrGpu.h"
#include "GrMemoryPool.h"
#include "GrOpFlushState.h"
<<<<<<< HEAD

class GrSignalSemaphoreOp final : public GrSemaphoreOp {
public:
    DEFINE_OP_CLASS_ID

    static std::unique_ptr<GrOp> Make(GrContext* context,
                                      sk_sp<GrSemaphore> semaphore,
                                      GrRenderTargetProxy* proxy,
                                      bool forceFlush) {
        GrOpMemoryPool* pool = context->contextPriv().opMemoryPool();

        return pool->allocate<GrSignalSemaphoreOp>(std::move(semaphore), proxy, forceFlush);
    }

    const char* name() const override { return "SignalSemaphore"; }

private:
    friend class GrOpMemoryPool; // for ctor

    explicit GrSignalSemaphoreOp(sk_sp<GrSemaphore> semaphore, GrRenderTargetProxy* proxy,
                                 bool forceFlush)
            : INHERITED(ClassID(), std::move(semaphore), proxy), fForceFlush(forceFlush) {}

    void onExecute(GrOpFlushState* state) override {
        state->gpu()->insertSemaphore(fSemaphore, fForceFlush);
    }

    bool fForceFlush;

    typedef GrSemaphoreOp INHERITED;
};
||||||| merged common ancestors

class GrSignalSemaphoreOp final : public GrSemaphoreOp {
public:
    DEFINE_OP_CLASS_ID

    static std::unique_ptr<GrSignalSemaphoreOp> Make(sk_sp<GrSemaphore> semaphore,
                                                     GrRenderTargetProxy* proxy,
                                                     bool forceFlush) {
        return std::unique_ptr<GrSignalSemaphoreOp>(new GrSignalSemaphoreOp(std::move(semaphore),
                                                                            proxy,
                                                                            forceFlush));
    }

    const char* name() const override { return "SignalSemaphore"; }

private:
    explicit GrSignalSemaphoreOp(sk_sp<GrSemaphore> semaphore, GrRenderTargetProxy* proxy,
                                 bool forceFlush)
            : INHERITED(ClassID(), std::move(semaphore), proxy), fForceFlush(forceFlush) {}

    void onExecute(GrOpFlushState* state) override {
        state->gpu()->insertSemaphore(fSemaphore, fForceFlush);
    }

    bool fForceFlush;

    typedef GrSemaphoreOp INHERITED;
};
=======
#include "GrRecordingContext.h"
#include "GrRecordingContextPriv.h"
>>>>>>> upstream-releases

class GrWaitSemaphoreOp final : public GrSemaphoreOp {
public:
    DEFINE_OP_CLASS_ID

<<<<<<< HEAD
    static std::unique_ptr<GrOp> Make(GrContext* context,
                                      sk_sp<GrSemaphore> semaphore,
                                      GrRenderTargetProxy* proxy) {
        GrOpMemoryPool* pool = context->contextPriv().opMemoryPool();

        return pool->allocate<GrWaitSemaphoreOp>(std::move(semaphore), proxy);
||||||| merged common ancestors
    static std::unique_ptr<GrWaitSemaphoreOp> Make(sk_sp<GrSemaphore> semaphore,
                                                   GrRenderTargetProxy* proxy) {
        return std::unique_ptr<GrWaitSemaphoreOp>(new GrWaitSemaphoreOp(std::move(semaphore),
                                                                        proxy));
=======
    static std::unique_ptr<GrOp> Make(GrRecordingContext* context,
                                      sk_sp<GrSemaphore> semaphore,
                                      GrRenderTargetProxy* proxy) {
        GrOpMemoryPool* pool = context->priv().opMemoryPool();

        return pool->allocate<GrWaitSemaphoreOp>(std::move(semaphore), proxy);
>>>>>>> upstream-releases
    }

    const char* name() const override { return "WaitSemaphore"; }

private:
    friend class GrOpMemoryPool; // for ctor

    explicit GrWaitSemaphoreOp(sk_sp<GrSemaphore> semaphore, GrRenderTargetProxy* proxy)
            : INHERITED(ClassID(), std::move(semaphore), proxy) {}

    void onExecute(GrOpFlushState* state, const SkRect& chainBounds) override {
        state->gpu()->waitSemaphore(fSemaphore);
    }

    typedef GrSemaphoreOp INHERITED;
};

////////////////////////////////////////////////////////////////////////////////

<<<<<<< HEAD
std::unique_ptr<GrOp> GrSemaphoreOp::MakeSignal(GrContext* context,
                                                sk_sp<GrSemaphore> semaphore,
                                                GrRenderTargetProxy* proxy,
                                                bool forceFlush) {
    return GrSignalSemaphoreOp::Make(context, std::move(semaphore), proxy, forceFlush);
}

std::unique_ptr<GrOp> GrSemaphoreOp::MakeWait(GrContext* context,
                                              sk_sp<GrSemaphore> semaphore,
                                              GrRenderTargetProxy* proxy) {
    return GrWaitSemaphoreOp::Make(context, std::move(semaphore), proxy);
||||||| merged common ancestors
std::unique_ptr<GrSemaphoreOp> GrSemaphoreOp::MakeSignal(sk_sp<GrSemaphore> semaphore,
                                                         GrRenderTargetProxy* proxy,
                                                         bool forceFlush) {
    return GrSignalSemaphoreOp::Make(std::move(semaphore), proxy, forceFlush);
}

std::unique_ptr<GrSemaphoreOp> GrSemaphoreOp::MakeWait(sk_sp<GrSemaphore> semaphore,
                                                       GrRenderTargetProxy* proxy) {
    return GrWaitSemaphoreOp::Make(std::move(semaphore), proxy);
=======
std::unique_ptr<GrOp> GrSemaphoreOp::MakeWait(GrRecordingContext* context,
                                              sk_sp<GrSemaphore> semaphore,
                                              GrRenderTargetProxy* proxy) {
    return GrWaitSemaphoreOp::Make(context, std::move(semaphore), proxy);
>>>>>>> upstream-releases
}


