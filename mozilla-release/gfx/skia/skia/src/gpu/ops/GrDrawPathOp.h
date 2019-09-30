/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrDrawPathOp_DEFINED
#define GrDrawPathOp_DEFINED

#include "GrDrawOp.h"
#include "GrOpFlushState.h"
#include "GrPath.h"
#include "GrPathProcessor.h"
#include "GrPathRendering.h"
#include "GrProcessorSet.h"
#include "GrStencilSettings.h"

class GrPaint;
class GrRecordingContext;

class GrDrawPathOpBase : public GrDrawOp {
protected:
    GrDrawPathOpBase(uint32_t classID, const SkMatrix& viewMatrix, GrPaint&&,
                     GrPathRendering::FillType, GrAAType);

    FixedFunctionFlags fixedFunctionFlags() const override {
        if (GrAATypeIsHW(fAAType)) {
            return FixedFunctionFlags::kUsesHWAA | FixedFunctionFlags::kUsesStencil;
        }
        return FixedFunctionFlags::kUsesStencil;
    }
<<<<<<< HEAD
    RequiresDstTexture finalize(const GrCaps& caps, const GrAppliedClip* clip) override {
        return this->doProcessorAnalysis(caps, clip).requiresDstTexture()
                ? RequiresDstTexture::kYes : RequiresDstTexture::kNo;
||||||| merged common ancestors
    RequiresDstTexture finalize(const GrCaps& caps, const GrAppliedClip* clip,
                                GrPixelConfigIsClamped dstIsClamped) override {
        return this->doProcessorAnalysis(caps, clip, dstIsClamped).requiresDstTexture()
                ? RequiresDstTexture::kYes : RequiresDstTexture::kNo;
=======
    GrProcessorSet::Analysis finalize(
            const GrCaps& caps, const GrAppliedClip* clip, GrFSAAType fsaaType) override {
        return this->doProcessorAnalysis(caps, clip, fsaaType);
>>>>>>> upstream-releases
    }

    void visitProxies(const VisitProxyFunc& func, VisitorType) const override {
        fProcessorSet.visitProxies(func);
    }

protected:
    const SkMatrix& viewMatrix() const { return fViewMatrix; }
    const SkPMColor4f& color() const { return fInputColor; }
    GrPathRendering::FillType fillType() const { return fFillType; }
    const GrProcessorSet& processors() const { return fProcessorSet; }
    GrProcessorSet detachProcessors() { return std::move(fProcessorSet); }
    inline GrPipeline::InitArgs pipelineInitArgs(const GrOpFlushState&);
<<<<<<< HEAD
    const GrProcessorSet::Analysis& doProcessorAnalysis(const GrCaps& caps,
                                                        const GrAppliedClip* clip) {
        bool isMixedSamples = GrAAType::kMixedSamples == fAAType;
        fAnalysis = fProcessorSet.finalize(fInputColor, GrProcessorAnalysisCoverage::kNone, clip,
                                           isMixedSamples, caps, &fInputColor);
        return fAnalysis;
    }
||||||| merged common ancestors
    const GrProcessorSet::Analysis& doProcessorAnalysis(const GrCaps& caps,
                                                        const GrAppliedClip* clip,
                                                        GrPixelConfigIsClamped dstIsClamped) {
        bool isMixedSamples = GrAAType::kMixedSamples == fAAType;
        fAnalysis = fProcessorSet.finalize(fInputColor, GrProcessorAnalysisCoverage::kNone, clip,
                                           isMixedSamples, caps, dstIsClamped, &fInputColor);
        return fAnalysis;
    }
=======
    const GrProcessorSet::Analysis& doProcessorAnalysis(
            const GrCaps&, const GrAppliedClip*, GrFSAAType);
>>>>>>> upstream-releases
    const GrProcessorSet::Analysis& processorAnalysis() const {
        SkASSERT(fAnalysis.isInitialized());
        return fAnalysis;
    }

private:
    void onPrepare(GrOpFlushState*) final {}

    SkMatrix fViewMatrix;
    SkPMColor4f fInputColor;
    GrProcessorSet::Analysis fAnalysis;
    GrPathRendering::FillType fFillType;
    GrAAType fAAType;
    GrProcessorSet fProcessorSet;

    typedef GrDrawOp INHERITED;
};

class GrDrawPathOp final : public GrDrawPathOpBase {
public:
    DEFINE_OP_CLASS_ID

<<<<<<< HEAD
    static std::unique_ptr<GrDrawOp> Make(GrContext*,
                                          const SkMatrix& viewMatrix,
                                          GrPaint&&,
                                          GrAAType,
                                          GrPath*);
||||||| merged common ancestors
    static std::unique_ptr<GrDrawOp> Make(const SkMatrix& viewMatrix, GrPaint&& paint,
                                          GrAAType aaType, GrPath* path) {
        return std::unique_ptr<GrDrawOp>(
                new GrDrawPathOp(viewMatrix, std::move(paint), aaType, path));
    }
=======
    static std::unique_ptr<GrDrawOp> Make(GrRecordingContext*,
                                          const SkMatrix& viewMatrix,
                                          GrPaint&&,
                                          GrAAType,
                                          GrPath*);
>>>>>>> upstream-releases

    const char* name() const override { return "DrawPath"; }

#ifdef SK_DEBUG
    SkString dumpInfo() const override;
#endif

private:
    friend class GrOpMemoryPool; // for ctor

    GrDrawPathOp(const SkMatrix& viewMatrix, GrPaint&& paint, GrAAType aaType, const GrPath* path)
            : GrDrawPathOpBase(ClassID(), viewMatrix, std::move(paint), path->getFillType(), aaType)
            , fPath(path) {
        this->setTransformedBounds(path->getBounds(), viewMatrix, HasAABloat::kNo, IsZeroArea::kNo);
    }

<<<<<<< HEAD
    void onExecute(GrOpFlushState* state) override;
||||||| merged common ancestors
    bool onCombineIfPossible(GrOp* t, const GrCaps& caps) override { return false; }

    void onExecute(GrOpFlushState* state) override;
=======
    void onExecute(GrOpFlushState*, const SkRect& chainBounds) override;
>>>>>>> upstream-releases

    GrPendingIOResource<const GrPath, kRead_GrIOType> fPath;

    typedef GrDrawPathOpBase INHERITED;
};

#endif
