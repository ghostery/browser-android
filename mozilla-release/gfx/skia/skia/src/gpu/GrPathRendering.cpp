/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrGpu.h"
#include "GrPathRendering.h"
#include "GrRenderTarget.h"
#include "SkDescriptor.h"
#include "SkScalerContext.h"
#include "SkGlyph.h"
#include "SkMatrix.h"
#include "SkTypeface.h"

const GrUserStencilSettings& GrPathRendering::GetStencilPassSettings(FillType fill) {
    switch (fill) {
        default:
            SK_ABORT("Unexpected path fill.");
        case GrPathRendering::kWinding_FillType: {
            constexpr static GrUserStencilSettings kWindingStencilPass(
                GrUserStencilSettings::StaticInit<
                    0xffff,
                    GrUserStencilTest::kAlwaysIfInClip,
                    0xffff,
                    GrUserStencilOp::kIncWrap,
                    GrUserStencilOp::kIncWrap,
                    0xffff>()
            );
            return kWindingStencilPass;
        }
        case GrPathRendering::kEvenOdd_FillType: {
            constexpr static GrUserStencilSettings kEvenOddStencilPass(
                GrUserStencilSettings::StaticInit<
                    0xffff,
                    GrUserStencilTest::kAlwaysIfInClip,
                    0xffff,
                    GrUserStencilOp::kInvert,
                    GrUserStencilOp::kInvert,
                    0xffff>()
            );
            return kEvenOddStencilPass;
        }
    }
}

void GrPathRendering::stencilPath(const StencilPathArgs& args, const GrPath* path) {
    fGpu->handleDirtyContext();
    this->onStencilPath(args, path);
}

<<<<<<< HEAD
void GrPathRendering::drawPath(const GrPrimitiveProcessor& primProc,
                               const GrPipeline& pipeline,
                               const GrPipeline::FixedDynamicState& fixedDynamicState,
||||||| merged common ancestors
void GrPathRendering::drawPath(const GrPipeline& pipeline,
                               const GrPrimitiveProcessor& primProc,
=======
void GrPathRendering::drawPath(GrRenderTarget* renderTarget, GrSurfaceOrigin origin,
                               const GrPrimitiveProcessor& primProc,
                               const GrPipeline& pipeline,
                               const GrPipeline::FixedDynamicState& fixedDynamicState,
>>>>>>> upstream-releases
                               // Cover pass settings in pipeline.
                               const GrStencilSettings& stencilPassSettings,
                               const GrPath* path) {
    fGpu->handleDirtyContext();
<<<<<<< HEAD
    if (GrXferBarrierType barrierType = pipeline.xferBarrierType(*fGpu->caps())) {
        fGpu->xferBarrier(pipeline.renderTarget(), barrierType);
    }
    this->onDrawPath(primProc, pipeline, fixedDynamicState, stencilPassSettings, path);
||||||| merged common ancestors
    if (GrXferBarrierType barrierType = pipeline.xferBarrierType(*fGpu->caps())) {
        fGpu->xferBarrier(pipeline.renderTarget(), barrierType);
    }
    this->onDrawPath(pipeline, primProc, stencilPassSettings, path);
}

void GrPathRendering::drawPaths(const GrPipeline& pipeline,
                                const GrPrimitiveProcessor& primProc,
                                // Cover pass settings in pipeline.
                                const GrStencilSettings& stencilPassSettings,
                                const GrPathRange* pathRange,
                                const void* indices,
                                PathIndexType indexType,
                                const float transformValues[],
                                PathTransformType transformType,
                                int count) {
    fGpu->handleDirtyContext();
    if (GrXferBarrierType barrierType = pipeline.xferBarrierType(*fGpu->caps())) {
        fGpu->xferBarrier(pipeline.renderTarget(), barrierType);
    }
#ifdef SK_DEBUG
    pathRange->assertPathsLoaded(indices, indexType, count);
#endif
    this->onDrawPaths(pipeline, primProc, stencilPassSettings, pathRange, indices, indexType,
                      transformValues, transformType, count);
=======
    if (GrXferBarrierType barrierType = pipeline.xferBarrierType(renderTarget->asTexture(),
                                                                 *fGpu->caps())) {
        fGpu->xferBarrier(renderTarget, barrierType);
    }
    this->onDrawPath(renderTarget, origin, primProc, pipeline, fixedDynamicState,
                     stencilPassSettings, path);
>>>>>>> upstream-releases
}
