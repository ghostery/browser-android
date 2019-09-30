/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrDebugMarkerOp_DEFINED
#define GrDebugMarkerOp_DEFINED

#include "GrOp.h"
#include "GrRenderTargetProxy.h"

<<<<<<< HEAD
class GrOpFlushState;

||||||| merged common ancestors
=======
class GrOpFlushState;
class GrRecordingContext;

>>>>>>> upstream-releases
class GrDebugMarkerOp final : public GrOp {
public:
    DEFINE_OP_CLASS_ID

<<<<<<< HEAD
    static std::unique_ptr<GrOp> Make(GrContext*,
                                      GrRenderTargetProxy*,
                                      const SkString&);
||||||| merged common ancestors
    static std::unique_ptr<GrOp> Make(GrRenderTargetProxy* proxy, const SkString& str) {
        return std::unique_ptr<GrOp>(new GrDebugMarkerOp(proxy, str));
    }
=======
    static std::unique_ptr<GrOp> Make(GrRecordingContext*,
                                      GrRenderTargetProxy*,
                                      const SkString&);
>>>>>>> upstream-releases

    const char* name() const override { return "DebugMarker"; }

#ifdef SK_DEBUG
    SkString dumpInfo() const override {
        SkString string;
        string.append(INHERITED::dumpInfo());
        return string;
    }
#endif

private:
    friend class GrOpMemoryPool; // for ctor

    GrDebugMarkerOp(GrRenderTargetProxy* proxy, const SkString& str)
            : INHERITED(ClassID())
            , fStr(str) {
        // Make this cover the whole screen so it can't be reordered around
        this->makeFullScreen(proxy);
    }

    void onPrepare(GrOpFlushState*) override {}

<<<<<<< HEAD
    void onExecute(GrOpFlushState* state) override;
||||||| merged common ancestors
    void onExecute(GrOpFlushState* state) override {
        //SkDebugf("%s\n", fStr.c_str());
        if (state->caps().gpuTracingSupport()) {
            state->commandBuffer()->insertEventMarker(fStr.c_str());
        }
    }
=======
    void onExecute(GrOpFlushState*, const SkRect& chainBounds) override;
>>>>>>> upstream-releases

    SkString fStr;

    typedef GrOp INHERITED;
};

#endif
