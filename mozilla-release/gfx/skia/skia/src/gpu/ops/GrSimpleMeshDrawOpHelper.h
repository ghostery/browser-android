/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrSimpleMeshDrawOpHelper_DEFINED
#define GrSimpleMeshDrawOpHelper_DEFINED

<<<<<<< HEAD
#include "GrContext.h"
#include "GrContextPriv.h"
#include "GrMemoryPool.h" // only here bc of the templated FactoryHelper
||||||| merged common ancestors
=======
#include "GrMemoryPool.h" // only here bc of the templated FactoryHelper
>>>>>>> upstream-releases
#include "GrMeshDrawOp.h"
#include "GrOpFlushState.h"
#include "GrPipeline.h"
<<<<<<< HEAD
#include <new>
||||||| merged common ancestors
=======
#include "GrRecordingContext.h"
#include "GrRecordingContextPriv.h"
#include <new>
>>>>>>> upstream-releases

struct SkRect;

/**
 * This class can be used to help implement simple mesh draw ops. It reduces the amount of
 * boilerplate code to type and also provides a mechanism for optionally allocating space for a
 * GrProcessorSet based on a GrPaint. It is intended to be used by ops that construct a single
 * GrPipeline for a uniform primitive color and a GrPaint.
 */
class GrSimpleMeshDrawOpHelper {
public:
    struct MakeArgs;

    /**
     * This can be used by a Op class to perform allocation and initialization such that a
     * GrProcessorSet (if required) is allocated as part of the the same allocation that as
     * the Op instance. It requires that Op implements a constructor of the form:
     *      Op(MakeArgs, GrColor, OpArgs...)
     * which is public or made accessible via 'friend'.
     */
    template <typename Op, typename... OpArgs>
<<<<<<< HEAD
    static std::unique_ptr<GrDrawOp> FactoryHelper(GrContext*, GrPaint&& paint, OpArgs... opArgs);
||||||| merged common ancestors
    static std::unique_ptr<GrDrawOp> FactoryHelper(GrPaint&& paint, OpArgs... opArgs);
=======
    static std::unique_ptr<GrDrawOp> FactoryHelper(GrRecordingContext*, GrPaint&&, OpArgs...);
>>>>>>> upstream-releases

    enum class Flags : uint32_t {
        kNone = 0x0,
        kSnapVerticesToPixelCenters = 0x1,
    };
    GR_DECL_BITFIELD_CLASS_OPS_FRIENDS(Flags);

    GrSimpleMeshDrawOpHelper(const MakeArgs&, GrAAType, Flags = Flags::kNone);
    ~GrSimpleMeshDrawOpHelper();

    GrSimpleMeshDrawOpHelper() = delete;
    GrSimpleMeshDrawOpHelper(const GrSimpleMeshDrawOpHelper&) = delete;
    GrSimpleMeshDrawOpHelper& operator=(const GrSimpleMeshDrawOpHelper&) = delete;

    GrDrawOp::FixedFunctionFlags fixedFunctionFlags() const;

    // noneAACompatibleWithCoverage should be set to true if the op can properly render a non-AA
    // primitive merged into a coverage-based op.
    bool isCompatible(const GrSimpleMeshDrawOpHelper& that, const GrCaps&, const SkRect& thisBounds,
                      const SkRect& thatBounds, bool noneAACompatibleWithCoverage = false) const;

    /**
     * Finalizes the processor set and determines whether the destination must be provided
     * to the fragment shader as a texture for blending.
     *
     * @param geometryCoverage Describes the coverage output of the op's geometry processor
     * @param geometryColor An in/out param. As input this informs processor analysis about the
     *                      color the op expects to output from its geometry processor. As output
     *                      this may be set to a known color in which case the op must output this
     *                      color from its geometry processor instead.
     */
<<<<<<< HEAD
    GrDrawOp::RequiresDstTexture xpRequiresDstTexture(const GrCaps& caps, const GrAppliedClip* clip,
                                                      GrProcessorAnalysisCoverage geometryCoverage,
                                                      GrProcessorAnalysisColor* geometryColor);
||||||| merged common ancestors
    GrDrawOp::RequiresDstTexture xpRequiresDstTexture(const GrCaps& caps, const GrAppliedClip* clip,
                                                      GrPixelConfigIsClamped dstIsClamped,
                                                      GrProcessorAnalysisCoverage geometryCoverage,
                                                      GrProcessorAnalysisColor* geometryColor);
=======
    GrProcessorSet::Analysis finalizeProcessors(
            const GrCaps& caps, const GrAppliedClip* clip, GrFSAAType fsaaType,
            GrProcessorAnalysisCoverage geometryCoverage, GrProcessorAnalysisColor* geometryColor) {
        return this->finalizeProcessors(caps, clip, &GrUserStencilSettings::kUnused, fsaaType,
                                        geometryCoverage, geometryColor);
    }
>>>>>>> upstream-releases

    /**
     * Version of above that can be used by ops that have a constant color geometry processor
     * output. The op passes this color as 'geometryColor' and after return if 'geometryColor' has
     * changed the op must override its geometry processor color output with the new color.
     */
<<<<<<< HEAD
    GrDrawOp::RequiresDstTexture xpRequiresDstTexture(const GrCaps&, const GrAppliedClip*,
                                                      GrProcessorAnalysisCoverage geometryCoverage,
                                                      GrColor* geometryColor);
||||||| merged common ancestors
    GrDrawOp::RequiresDstTexture xpRequiresDstTexture(const GrCaps&, const GrAppliedClip*,
                                                      GrPixelConfigIsClamped dstIsClamped,
                                                      GrProcessorAnalysisCoverage geometryCoverage,
                                                      GrColor* geometryColor);
=======
    GrProcessorSet::Analysis finalizeProcessors(
            const GrCaps&, const GrAppliedClip*, GrFSAAType,
            GrProcessorAnalysisCoverage geometryCoverage, SkPMColor4f* geometryColor);

    bool isTrivial() const {
      return fProcessors == nullptr;
    }
>>>>>>> upstream-releases

    bool usesLocalCoords() const {
        SkASSERT(fDidAnalysis);
        return fUsesLocalCoords;
    }

    bool compatibleWithAlphaAsCoverage() const { return fCompatibleWithAlphaAsCoveage; }

<<<<<<< HEAD
    using PipelineAndFixedDynamicState = GrOpFlushState::PipelineAndFixedDynamicState;
    /** Makes a pipeline that consumes the processor set and the op's applied clip. */
    PipelineAndFixedDynamicState makePipeline(GrMeshDrawOp::Target* target,
                                              int numPrimitiveProcessorTextures = 0) {
        return this->internalMakePipeline(target, this->pipelineInitArgs(target),
                                          numPrimitiveProcessorTextures);
    }

||||||| merged common ancestors
    /** Makes a pipeline that consumes the processor set and the op's applied clip. */
    GrPipeline* makePipeline(GrMeshDrawOp::Target* target) {
        return this->internalMakePipeline(target, this->pipelineInitArgs(target));
    }

=======
>>>>>>> upstream-releases
    struct MakeArgs {
    private:
        MakeArgs() = default;

        GrProcessorSet* fProcessorSet;

        friend class GrSimpleMeshDrawOpHelper;
    };

    void visitProxies(const std::function<void(GrSurfaceProxy*)>& func) const {
        if (fProcessors) {
            fProcessors->visitProxies(func);
        }
    }

#ifdef SK_DEBUG
    SkString dumpInfo() const;
#endif
    GrAAType aaType() const { return static_cast<GrAAType>(fAAType); }

    void setAAType(GrAAType aaType) {
      fAAType = static_cast<unsigned>(aaType);
    }

    void executeDrawsAndUploads(const GrOp*, GrOpFlushState*, const SkRect& chainBounds);

protected:
    uint32_t pipelineFlags() const { return fPipelineFlags; }

<<<<<<< HEAD
    GrPipeline::InitArgs pipelineInitArgs(GrMeshDrawOp::Target* target) const;

    PipelineAndFixedDynamicState internalMakePipeline(GrMeshDrawOp::Target*,
                                                      const GrPipeline::InitArgs&,
                                                      int numPrimitiveProcessorTextures);
||||||| merged common ancestors
    GrPipeline::InitArgs pipelineInitArgs(GrMeshDrawOp::Target* target) const;

    GrPipeline* internalMakePipeline(GrMeshDrawOp::Target*, const GrPipeline::InitArgs&);
=======
    GrProcessorSet::Analysis finalizeProcessors(
            const GrCaps& caps, const GrAppliedClip*, const GrUserStencilSettings*, GrFSAAType,
            GrProcessorAnalysisCoverage geometryCoverage, GrProcessorAnalysisColor* geometryColor);
>>>>>>> upstream-releases

    GrProcessorSet* fProcessors;
    unsigned fPipelineFlags : 8;
    unsigned fAAType : 2;
    unsigned fUsesLocalCoords : 1;
    unsigned fCompatibleWithAlphaAsCoveage : 1;
    SkDEBUGCODE(unsigned fMadePipeline : 1;)
    SkDEBUGCODE(unsigned fDidAnalysis : 1;)
};

/**
 * This class extends GrSimpleMeshDrawOpHelper to support an optional GrUserStencilSettings. This
 * uses private inheritance because it non-virtually overrides methods in the base class and should
 * never be used with a GrSimpleMeshDrawOpHelper pointer or reference.
 */
class GrSimpleMeshDrawOpHelperWithStencil : private GrSimpleMeshDrawOpHelper {
public:
    using MakeArgs = GrSimpleMeshDrawOpHelper::MakeArgs;
    using Flags = GrSimpleMeshDrawOpHelper::Flags;
<<<<<<< HEAD
    using PipelineAndFixedDynamicState = GrOpFlushState::PipelineAndFixedDynamicState;

||||||| merged common ancestors
=======

>>>>>>> upstream-releases
    using GrSimpleMeshDrawOpHelper::visitProxies;

    // using declarations can't be templated, so this is a pass through function instead.
    template <typename Op, typename... OpArgs>
<<<<<<< HEAD
    static std::unique_ptr<GrDrawOp> FactoryHelper(GrContext* context, GrPaint&& paint,
                                                   OpArgs... opArgs) {
||||||| merged common ancestors
    static std::unique_ptr<GrDrawOp> FactoryHelper(GrPaint&& paint, OpArgs... opArgs) {
=======
    static std::unique_ptr<GrDrawOp> FactoryHelper(GrRecordingContext* context, GrPaint&& paint,
                                                   OpArgs... opArgs) {
>>>>>>> upstream-releases
        return GrSimpleMeshDrawOpHelper::FactoryHelper<Op, OpArgs...>(
                context, std::move(paint), std::forward<OpArgs>(opArgs)...);
    }

    GrSimpleMeshDrawOpHelperWithStencil(const MakeArgs&, GrAAType, const GrUserStencilSettings*,
                                        Flags = Flags::kNone);

    GrDrawOp::FixedFunctionFlags fixedFunctionFlags() const;

    GrProcessorSet::Analysis finalizeProcessors(
            const GrCaps& caps, const GrAppliedClip* clip, GrFSAAType fsaaType,
            GrProcessorAnalysisCoverage geometryCoverage, GrProcessorAnalysisColor* geometryColor) {
        return this->INHERITED::finalizeProcessors(
                caps, clip, fStencilSettings, fsaaType, geometryCoverage, geometryColor);
    }

    GrProcessorSet::Analysis finalizeProcessors(
            const GrCaps&, const GrAppliedClip*, GrFSAAType,
            GrProcessorAnalysisCoverage geometryCoverage, SkPMColor4f* geometryColor);

    using GrSimpleMeshDrawOpHelper::aaType;
    using GrSimpleMeshDrawOpHelper::setAAType;
    using GrSimpleMeshDrawOpHelper::isTrivial;
    using GrSimpleMeshDrawOpHelper::usesLocalCoords;
    using GrSimpleMeshDrawOpHelper::compatibleWithAlphaAsCoverage;

    bool isCompatible(const GrSimpleMeshDrawOpHelperWithStencil& that, const GrCaps&,
                      const SkRect& thisBounds, const SkRect& thatBounds,
                      bool noneAACompatibleWithCoverage = false) const;

<<<<<<< HEAD
    PipelineAndFixedDynamicState makePipeline(GrMeshDrawOp::Target*,
                                              int numPrimitiveProcessorTextures = 0);
||||||| merged common ancestors
    const GrPipeline* makePipeline(GrMeshDrawOp::Target*);
=======
    void executeDrawsAndUploads(const GrOp*, GrOpFlushState*, const SkRect& chainBounds);
>>>>>>> upstream-releases

#ifdef SK_DEBUG
    SkString dumpInfo() const;
#endif

private:
    const GrUserStencilSettings* fStencilSettings;
    typedef GrSimpleMeshDrawOpHelper INHERITED;
};

template <typename Op, typename... OpArgs>
<<<<<<< HEAD
std::unique_ptr<GrDrawOp> GrSimpleMeshDrawOpHelper::FactoryHelper(GrContext* context,
                                                                  GrPaint&& paint,
||||||| merged common ancestors
std::unique_ptr<GrDrawOp> GrSimpleMeshDrawOpHelper::FactoryHelper(GrPaint&& paint,
=======
std::unique_ptr<GrDrawOp> GrSimpleMeshDrawOpHelper::FactoryHelper(GrRecordingContext* context,
                                                                  GrPaint&& paint,
>>>>>>> upstream-releases
                                                                  OpArgs... opArgs) {
<<<<<<< HEAD
    GrOpMemoryPool* pool = context->contextPriv().opMemoryPool();

||||||| merged common ancestors
=======
    GrOpMemoryPool* pool = context->priv().opMemoryPool();

>>>>>>> upstream-releases
    MakeArgs makeArgs;
<<<<<<< HEAD
    GrColor color = paint.getColor();

||||||| merged common ancestors
    makeArgs.fSRGBFlags = GrPipeline::SRGBFlagsFromPaint(paint);
    GrColor color = paint.getColor();
=======

>>>>>>> upstream-releases
    if (paint.isTrivial()) {
        makeArgs.fProcessorSet = nullptr;
<<<<<<< HEAD
        return pool->allocate<Op>(makeArgs, color, std::forward<OpArgs>(opArgs)...);
||||||| merged common ancestors
        return std::unique_ptr<GrDrawOp>(new Op(makeArgs, color, std::forward<OpArgs>(opArgs)...));
=======
        return pool->allocate<Op>(makeArgs, paint.getColor4f(), std::forward<OpArgs>(opArgs)...);
>>>>>>> upstream-releases
    } else {
        char* mem = (char*) pool->allocate(sizeof(Op) + sizeof(GrProcessorSet));
        char* setMem = mem + sizeof(Op);
        auto color = paint.getColor4f();
        makeArgs.fProcessorSet = new (setMem) GrProcessorSet(std::move(paint));
<<<<<<< HEAD

        return std::unique_ptr<GrDrawOp>(new (mem) Op(makeArgs, color,
                                                      std::forward<OpArgs>(opArgs)...));
||||||| merged common ancestors
        return std::unique_ptr<GrDrawOp>(
                new (mem) Op(makeArgs, color, std::forward<OpArgs>(opArgs)...));
=======
        return std::unique_ptr<GrDrawOp>(new (mem) Op(makeArgs, color,
                                                      std::forward<OpArgs>(opArgs)...));
>>>>>>> upstream-releases
    }
}

GR_MAKE_BITFIELD_CLASS_OPS(GrSimpleMeshDrawOpHelper::Flags)

#endif
