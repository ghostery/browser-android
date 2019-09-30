/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrClearStencilClipOp_DEFINED
#define GrClearStencilClipOp_DEFINED

#include "GrFixedClip.h"
#include "GrOp.h"
#include "GrRenderTargetProxy.h"

<<<<<<< HEAD
class GrOpFlushState;

||||||| merged common ancestors
=======
class GrOpFlushState;
class GrRecordingContext;

>>>>>>> upstream-releases
class GrClearStencilClipOp final : public GrOp {
public:
    DEFINE_OP_CLASS_ID

<<<<<<< HEAD
    static std::unique_ptr<GrOp> Make(GrContext* context,
                                      const GrFixedClip& clip,
                                      bool insideStencilMask,
                                      GrRenderTargetProxy* proxy);
||||||| merged common ancestors
    static std::unique_ptr<GrOp> Make(const GrFixedClip& clip, bool insideStencilMask,
                                      GrRenderTargetProxy* proxy) {
        return std::unique_ptr<GrOp>(new GrClearStencilClipOp(clip, insideStencilMask, proxy));
    }
=======
    static std::unique_ptr<GrOp> Make(GrRecordingContext* context,
                                      const GrFixedClip& clip,
                                      bool insideStencilMask,
                                      GrRenderTargetProxy* proxy);
>>>>>>> upstream-releases

    const char* name() const override { return "ClearStencilClip"; }

#ifdef SK_DEBUG
    SkString dumpInfo() const override {
        SkString string("Scissor [");
        if (fClip.scissorEnabled()) {
            const SkIRect& r = fClip.scissorRect();
            string.appendf("L: %d, T: %d, R: %d, B: %d", r.fLeft, r.fTop, r.fRight, r.fBottom);
        } else {
            string.append("disabled");
        }
        string.appendf("], insideMask: %s\n", fInsideStencilMask ? "true" : "false");
        string.append(INHERITED::dumpInfo());
        return string;
    }
#endif

private:
    friend class GrOpMemoryPool; // for ctor

    GrClearStencilClipOp(const GrFixedClip& clip, bool insideStencilMask,
                         GrRenderTargetProxy* proxy)
            : INHERITED(ClassID())
            , fClip(clip)
            , fInsideStencilMask(insideStencilMask) {
        const SkRect& bounds = fClip.scissorEnabled()
                                            ? SkRect::Make(fClip.scissorRect())
                                            : SkRect::MakeIWH(proxy->width(), proxy->height());
        this->setBounds(bounds, HasAABloat::kNo, IsZeroArea::kNo);
    }

    void onPrepare(GrOpFlushState*) override {}

<<<<<<< HEAD
    void onExecute(GrOpFlushState* state) override;
||||||| merged common ancestors
    void onExecute(GrOpFlushState* state) override {
        SkASSERT(state->rtCommandBuffer());
        state->rtCommandBuffer()->clearStencilClip(fClip, fInsideStencilMask);
    }
=======
    void onExecute(GrOpFlushState*, const SkRect& chainBounds) override;
>>>>>>> upstream-releases

    const GrFixedClip fClip;
    const bool        fInsideStencilMask;

    typedef GrOp INHERITED;
};

#endif
