/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrSimpleMeshDrawOpHelper.h"
#include "GrAppliedClip.h"
#include "GrProcessorSet.h"
#include "GrRect.h"
#include "GrUserStencilSettings.h"

GrSimpleMeshDrawOpHelper::GrSimpleMeshDrawOpHelper(const MakeArgs& args, GrAAType aaType,
                                                   Flags flags)
        : fProcessors(args.fProcessorSet)
        , fPipelineFlags(0)
        , fAAType((int)aaType)
        , fUsesLocalCoords(false)
        , fCompatibleWithAlphaAsCoveage(false) {
    SkDEBUGCODE(fDidAnalysis = false);
    SkDEBUGCODE(fMadePipeline = false);
    if (GrAATypeIsHW(aaType)) {
        fPipelineFlags |= GrPipeline::kHWAntialias_Flag;
    }
    if (flags & Flags::kSnapVerticesToPixelCenters) {
        fPipelineFlags |= GrPipeline::kSnapVerticesToPixelCenters_Flag;
    }
}

GrSimpleMeshDrawOpHelper::~GrSimpleMeshDrawOpHelper() {
    if (fProcessors) {
        fProcessors->~GrProcessorSet();
    }
}

GrDrawOp::FixedFunctionFlags GrSimpleMeshDrawOpHelper::fixedFunctionFlags() const {
    return GrAATypeIsHW((this->aaType())) ? GrDrawOp::FixedFunctionFlags::kUsesHWAA
                                          : GrDrawOp::FixedFunctionFlags::kNone;
}

static bool none_as_coverage_aa_compatible(GrAAType aa1, GrAAType aa2) {
    return (aa1 == GrAAType::kNone && aa2 == GrAAType::kCoverage) ||
           (aa1 == GrAAType::kCoverage && aa2 == GrAAType::kNone);
}

bool GrSimpleMeshDrawOpHelper::isCompatible(const GrSimpleMeshDrawOpHelper& that,
                                            const GrCaps& caps, const SkRect& thisBounds,
                                            const SkRect& thatBounds, bool noneAsCoverageAA) const {
    if (SkToBool(fProcessors) != SkToBool(that.fProcessors)) {
        return false;
    }
    if (fProcessors) {
        if (*fProcessors != *that.fProcessors) {
            return false;
        }
    }
    bool result = fPipelineFlags == that.fPipelineFlags && (fAAType == that.fAAType ||
            (noneAsCoverageAA && none_as_coverage_aa_compatible(this->aaType(), that.aaType())));
    SkASSERT(!result || fCompatibleWithAlphaAsCoveage == that.fCompatibleWithAlphaAsCoveage);
    SkASSERT(!result || fUsesLocalCoords == that.fUsesLocalCoords);
    return result;
}

<<<<<<< HEAD
GrDrawOp::RequiresDstTexture GrSimpleMeshDrawOpHelper::xpRequiresDstTexture(
        const GrCaps& caps, const GrAppliedClip* clip, GrProcessorAnalysisCoverage geometryCoverage,
        GrProcessorAnalysisColor* geometryColor) {
||||||| merged common ancestors
GrDrawOp::RequiresDstTexture GrSimpleMeshDrawOpHelper::xpRequiresDstTexture(
        const GrCaps& caps, const GrAppliedClip* clip, GrPixelConfigIsClamped dstIsClamped,
        GrProcessorAnalysisCoverage geometryCoverage, GrProcessorAnalysisColor* geometryColor) {
=======
GrProcessorSet::Analysis GrSimpleMeshDrawOpHelper::finalizeProcessors(
        const GrCaps& caps, const GrAppliedClip* clip, GrFSAAType fsaaType,
        GrProcessorAnalysisCoverage geometryCoverage, SkPMColor4f* geometryColor) {
    GrProcessorAnalysisColor color = *geometryColor;
    auto result = this->finalizeProcessors(caps, clip, fsaaType, geometryCoverage, &color);
    color.isConstant(geometryColor);
    return result;
}

GrProcessorSet::Analysis GrSimpleMeshDrawOpHelper::finalizeProcessors(
        const GrCaps& caps, const GrAppliedClip* clip, const GrUserStencilSettings* userStencil,
        GrFSAAType fsaaType, GrProcessorAnalysisCoverage geometryCoverage,
        GrProcessorAnalysisColor* geometryColor) {
>>>>>>> upstream-releases
    SkDEBUGCODE(fDidAnalysis = true);
    GrProcessorSet::Analysis analysis;
    if (fProcessors) {
        GrProcessorAnalysisCoverage coverage = geometryCoverage;
        if (GrProcessorAnalysisCoverage::kNone == coverage) {
            coverage = clip->numClipCoverageFragmentProcessors()
                               ? GrProcessorAnalysisCoverage::kSingleChannel
                               : GrProcessorAnalysisCoverage::kNone;
        }
<<<<<<< HEAD
        bool isMixedSamples = this->aaType() == GrAAType::kMixedSamples;
        GrColor overrideColor;
        analysis = fProcessors->finalize(*geometryColor, coverage, clip, isMixedSamples, caps,
                                         &overrideColor);
||||||| merged common ancestors
        bool isMixedSamples = this->aaType() == GrAAType::kMixedSamples;
        GrColor overrideColor;
        analysis = fProcessors->finalize(*geometryColor, coverage, clip, isMixedSamples, caps,
                                         dstIsClamped, &overrideColor);
=======
        SkPMColor4f overrideColor;
        analysis = fProcessors->finalize(
                *geometryColor, coverage, clip, userStencil, fsaaType, caps, &overrideColor);
>>>>>>> upstream-releases
        if (analysis.inputColorIsOverridden()) {
            *geometryColor = overrideColor;
        }
    } else {
        analysis = GrProcessorSet::EmptySetAnalysis();
    }
    fUsesLocalCoords = analysis.usesLocalCoords();
    fCompatibleWithAlphaAsCoveage = analysis.isCompatibleWithCoverageAsAlpha();
    return analysis;
}

<<<<<<< HEAD
GrDrawOp::RequiresDstTexture GrSimpleMeshDrawOpHelper::xpRequiresDstTexture(
        const GrCaps& caps, const GrAppliedClip* clip, GrProcessorAnalysisCoverage geometryCoverage,
        GrColor* geometryColor) {
    GrProcessorAnalysisColor color = *geometryColor;
    auto result = this->xpRequiresDstTexture(caps, clip, geometryCoverage, &color);
    color.isConstant(geometryColor);
    return result;
||||||| merged common ancestors
GrDrawOp::RequiresDstTexture GrSimpleMeshDrawOpHelper::xpRequiresDstTexture(
        const GrCaps& caps, const GrAppliedClip* clip, GrPixelConfigIsClamped dstIsClamped,
        GrProcessorAnalysisCoverage geometryCoverage, GrColor* geometryColor) {
    GrProcessorAnalysisColor color = *geometryColor;
    auto result = this->xpRequiresDstTexture(caps, clip, dstIsClamped, geometryCoverage, &color);
    color.isConstant(geometryColor);
    return result;
=======
void GrSimpleMeshDrawOpHelper::executeDrawsAndUploads(
        const GrOp* op, GrOpFlushState* flushState, const SkRect& chainBounds) {
    if (fProcessors) {
        flushState->executeDrawsAndUploadsForMeshDrawOp(
                op, chainBounds, std::move(*fProcessors), fPipelineFlags);
    } else {
        flushState->executeDrawsAndUploadsForMeshDrawOp(
                op, chainBounds, GrProcessorSet::MakeEmptySet(), fPipelineFlags);
    }
>>>>>>> upstream-releases
}

#ifdef SK_DEBUG
SkString GrSimpleMeshDrawOpHelper::dumpInfo() const {
    const GrProcessorSet& processors = fProcessors ? *fProcessors : GrProcessorSet::EmptySet();
    SkString result = processors.dumpProcessors();
    result.append("AA Type: ");
    switch (this->aaType()) {
        case GrAAType::kNone:
            result.append(" none\n");
            break;
        case GrAAType::kCoverage:
            result.append(" coverage\n");
            break;
        case GrAAType::kMSAA:
            result.append(" msaa\n");
            break;
        case GrAAType::kMixedSamples:
            result.append(" mixed samples\n");
            break;
    }
    result.append(GrPipeline::DumpFlags(fPipelineFlags));
    return result;
}
<<<<<<< HEAD

GrPipeline::InitArgs GrSimpleMeshDrawOpHelper::pipelineInitArgs(
        GrMeshDrawOp::Target* target) const {
    GrPipeline::InitArgs args;
    args.fFlags = this->pipelineFlags();
    args.fProxy = target->proxy();
    args.fDstProxy = target->dstProxy();
    args.fCaps = &target->caps();
    args.fResourceProvider = target->resourceProvider();
    return args;
}

auto GrSimpleMeshDrawOpHelper::internalMakePipeline(GrMeshDrawOp::Target* target,
                                                    const GrPipeline::InitArgs& args,
                                                    int numPrimitiveProcessorProxies)
        -> PipelineAndFixedDynamicState {
    // A caller really should only call this once as the processor set and applied clip get
    // moved into the GrPipeline.
    SkASSERT(!fMadePipeline);
    SkDEBUGCODE(fMadePipeline = true);
    auto clip = target->detachAppliedClip();
    GrPipeline::FixedDynamicState* fixedDynamicState = nullptr;
    if (clip.scissorState().enabled() || numPrimitiveProcessorProxies) {
        fixedDynamicState = target->allocFixedDynamicState(clip.scissorState().rect());
        if (numPrimitiveProcessorProxies) {
            fixedDynamicState->fPrimitiveProcessorTextures =
                    target->allocPrimitiveProcessorTextureArray(numPrimitiveProcessorProxies);
        }
    }
    if (fProcessors) {
        return {target->allocPipeline(args, std::move(*fProcessors), std::move(clip)),
                fixedDynamicState};
    } else {
        return {target->allocPipeline(args, GrProcessorSet::MakeEmptySet(), std::move(clip)),
                fixedDynamicState};
    }
}
||||||| merged common ancestors

GrPipeline::InitArgs GrSimpleMeshDrawOpHelper::pipelineInitArgs(
        GrMeshDrawOp::Target* target) const {
    GrPipeline::InitArgs args;
    args.fFlags = this->pipelineFlags();
    args.fProxy = target->proxy();
    args.fDstProxy = target->dstProxy();
    args.fCaps = &target->caps();
    args.fResourceProvider = target->resourceProvider();
    return args;
}

GrPipeline* GrSimpleMeshDrawOpHelper::internalMakePipeline(GrMeshDrawOp::Target* target,
                                                           const GrPipeline::InitArgs& args) {
    // A caller really should only call this once as the processor set and applied clip get
    // moved into the GrPipeline.
    SkASSERT(!fMadePipeline);
    SkDEBUGCODE(fMadePipeline = true);
    if (fProcessors) {
        return target->allocPipeline(args, std::move(*fProcessors), target->detachAppliedClip());
    } else {
        return target->allocPipeline(args, GrProcessorSet::MakeEmptySet(),
                                     target->detachAppliedClip());
    }
}
=======
#endif
>>>>>>> upstream-releases

GrSimpleMeshDrawOpHelperWithStencil::GrSimpleMeshDrawOpHelperWithStencil(
        const MakeArgs& args, GrAAType aaType, const GrUserStencilSettings* stencilSettings,
        Flags flags)
        : INHERITED(args, aaType, flags)
        , fStencilSettings(stencilSettings ? stencilSettings : &GrUserStencilSettings::kUnused) {}

GrDrawOp::FixedFunctionFlags GrSimpleMeshDrawOpHelperWithStencil::fixedFunctionFlags() const {
    GrDrawOp::FixedFunctionFlags flags = INHERITED::fixedFunctionFlags();
    if (fStencilSettings != &GrUserStencilSettings::kUnused) {
        flags |= GrDrawOp::FixedFunctionFlags::kUsesStencil;
    }
    return flags;
}

GrProcessorSet::Analysis GrSimpleMeshDrawOpHelperWithStencil::finalizeProcessors(
        const GrCaps& caps, const GrAppliedClip* clip, GrFSAAType fsaaType,
        GrProcessorAnalysisCoverage geometryCoverage, SkPMColor4f* geometryColor) {
    GrProcessorAnalysisColor color = *geometryColor;
    auto result = this->finalizeProcessors(caps, clip, fsaaType, geometryCoverage, &color);
    color.isConstant(geometryColor);
    return result;
}

bool GrSimpleMeshDrawOpHelperWithStencil::isCompatible(
        const GrSimpleMeshDrawOpHelperWithStencil& that, const GrCaps& caps,
        const SkRect& thisBounds, const SkRect& thatBounds, bool noneAsCoverageAA) const {
    return INHERITED::isCompatible(that, caps, thisBounds, thatBounds, noneAsCoverageAA) &&
           fStencilSettings == that.fStencilSettings;
}

<<<<<<< HEAD
auto GrSimpleMeshDrawOpHelperWithStencil::makePipeline(GrMeshDrawOp::Target* target,
                                                       int numPrimitiveProcessorTextures)
        -> PipelineAndFixedDynamicState {
    auto args = INHERITED::pipelineInitArgs(target);
    args.fUserStencil = fStencilSettings;
    return this->internalMakePipeline(target, args, numPrimitiveProcessorTextures);
||||||| merged common ancestors
const GrPipeline* GrSimpleMeshDrawOpHelperWithStencil::makePipeline(
        GrMeshDrawOp::Target* target) {
    auto args = INHERITED::pipelineInitArgs(target);
    args.fUserStencil = fStencilSettings;
    return this->internalMakePipeline(target, args);
=======
void GrSimpleMeshDrawOpHelperWithStencil::executeDrawsAndUploads(
        const GrOp* op, GrOpFlushState* flushState, const SkRect& chainBounds) {
    if (fProcessors) {
        flushState->executeDrawsAndUploadsForMeshDrawOp(
                op, chainBounds, std::move(*fProcessors), fPipelineFlags, fStencilSettings);
    } else {
        flushState->executeDrawsAndUploadsForMeshDrawOp(
                op, chainBounds, GrProcessorSet::MakeEmptySet(), fPipelineFlags, fStencilSettings);
    }
>>>>>>> upstream-releases
}

#ifdef SK_DEBUG
SkString GrSimpleMeshDrawOpHelperWithStencil::dumpInfo() const {
    SkString result = INHERITED::dumpInfo();
    result.appendf("Stencil settings: %s\n", (fStencilSettings ? "yes" : "no"));
    return result;
}
#endif
