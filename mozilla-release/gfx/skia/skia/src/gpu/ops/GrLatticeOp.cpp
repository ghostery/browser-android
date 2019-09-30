/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrLatticeOp.h"
#include "GrDefaultGeoProcFactory.h"
#include "GrDrawOpTest.h"
#include "GrGpu.h"
#include "GrMeshDrawOp.h"
#include "GrOpFlushState.h"
#include "GrResourceProvider.h"
#include "GrResourceProviderPriv.h"
#include "GrSimpleMeshDrawOpHelper.h"
#include "GrVertexWriter.h"
#include "SkBitmap.h"
#include "SkLatticeIter.h"
#include "SkMatrixPriv.h"
#include "SkRect.h"
#include "glsl/GrGLSLColorSpaceXformHelper.h"
#include "glsl/GrGLSLGeometryProcessor.h"
#include "glsl/GrGLSLVarying.h"

namespace {

<<<<<<< HEAD
class LatticeGP : public GrGeometryProcessor {
public:
    struct Vertex {
        SkPoint fPosition;
        SkPoint fTextureCoords;
        SkRect fTextureDomain;
        GrColor fColor;
    };

    static sk_sp<GrGeometryProcessor> Make(const GrTextureProxy* proxy,
                                           sk_sp<GrColorSpaceXform> csxf,
                                           GrSamplerState::Filter filter) {
        return sk_sp<GrGeometryProcessor>(new LatticeGP(proxy, std::move(csxf), filter));
    }

    const char* name() const override { return "LatticeGP"; }

    void getGLSLProcessorKey(const GrShaderCaps&, GrProcessorKeyBuilder* b) const override {
        b->add32(GrColorSpaceXform::XformKey(fColorSpaceXform.get()));
    }

    GrGLSLPrimitiveProcessor* createGLSLInstance(const GrShaderCaps& caps) const override {
        class GLSLProcessor : public GrGLSLGeometryProcessor {
        public:
            void setData(const GrGLSLProgramDataManager& pdman, const GrPrimitiveProcessor& proc,
                         FPCoordTransformIter&& transformIter) override {
                const auto& latticeGP = proc.cast<LatticeGP>();
                this->setTransformDataHelper(SkMatrix::I(), pdman, &transformIter);
                fColorSpaceXformHelper.setData(pdman, latticeGP.fColorSpaceXform.get());
            }

        private:
            void onEmitCode(EmitArgs& args, GrGPArgs* gpArgs) override {
                using Interpolation = GrGLSLVaryingHandler::Interpolation;
                const auto& latticeGP = args.fGP.cast<LatticeGP>();
                fColorSpaceXformHelper.emitCode(args.fUniformHandler,
                                                latticeGP.fColorSpaceXform.get());

                args.fVaryingHandler->emitAttributes(latticeGP);
                this->writeOutputPosition(args.fVertBuilder, gpArgs, latticeGP.kPositions.name());
                this->emitTransforms(args.fVertBuilder,
                                     args.fVaryingHandler,
                                     args.fUniformHandler,
                                     latticeGP.kTextureCoords.asShaderVar(),
                                     args.fFPCoordTransformHandler);
                args.fFragBuilder->codeAppend("float2 textureCoords;");
                args.fVaryingHandler->addPassThroughAttribute(latticeGP.kTextureCoords,
                                                              "textureCoords");
                args.fFragBuilder->codeAppend("float4 textureDomain;");
                args.fVaryingHandler->addPassThroughAttribute(
                        latticeGP.kTextureDomain, "textureDomain", Interpolation::kCanBeFlat);
                args.fVaryingHandler->addPassThroughAttribute(latticeGP.kColors, args.fOutputColor,
                                                              Interpolation::kCanBeFlat);
                args.fFragBuilder->codeAppendf("%s = ", args.fOutputColor);
                args.fFragBuilder->appendTextureLookupAndModulate(
                        args.fOutputColor,
                        args.fTexSamplers[0],
                        "clamp(textureCoords, textureDomain.xy, textureDomain.zw)",
                        kFloat2_GrSLType,
                        &fColorSpaceXformHelper);
                args.fFragBuilder->codeAppend(";");
                args.fFragBuilder->codeAppendf("%s = half4(1);", args.fOutputCoverage);
            }
            GrGLSLColorSpaceXformHelper fColorSpaceXformHelper;
        };
        return new GLSLProcessor;
    }

private:
    LatticeGP(const GrTextureProxy* proxy, sk_sp<GrColorSpaceXform> csxf,
              GrSamplerState::Filter filter)
            : INHERITED(kLatticeGP_ClassID), fColorSpaceXform(std::move(csxf)) {
        fSampler.reset(proxy->textureType(), proxy->config(), filter);
        this->setTextureSamplerCnt(1);
        this->setVertexAttributeCnt(4);
    }

    const Attribute& onVertexAttribute(int i) const override {
        return IthAttribute(i, kPositions, kTextureCoords, kTextureDomain, kColors);
    }

    const TextureSampler& onTextureSampler(int) const override { return fSampler; }

    static constexpr Attribute kPositions =
            {"position", kFloat2_GrVertexAttribType, kFloat2_GrSLType};
    static constexpr Attribute kTextureCoords =
            {"textureCoords", kFloat2_GrVertexAttribType, kFloat2_GrSLType};
    static constexpr Attribute kTextureDomain =
            {"textureDomain", kFloat4_GrVertexAttribType, kFloat4_GrSLType};
    static constexpr Attribute kColors =
            {"color", kUByte4_norm_GrVertexAttribType, kHalf4_GrSLType};

    sk_sp<GrColorSpaceXform> fColorSpaceXform;
    TextureSampler fSampler;

    typedef GrGeometryProcessor INHERITED;
};

constexpr GrPrimitiveProcessor::Attribute LatticeGP::kPositions;
constexpr GrPrimitiveProcessor::Attribute LatticeGP::kTextureCoords;
constexpr GrPrimitiveProcessor::Attribute LatticeGP::kTextureDomain;
constexpr GrPrimitiveProcessor::Attribute LatticeGP::kColors;

||||||| merged common ancestors
=======
class LatticeGP : public GrGeometryProcessor {
public:
    static sk_sp<GrGeometryProcessor> Make(GrGpu* gpu,
                                           const GrTextureProxy* proxy,
                                           sk_sp<GrColorSpaceXform> csxf,
                                           GrSamplerState::Filter filter,
                                           bool wideColor) {
        return sk_sp<GrGeometryProcessor>(
                new LatticeGP(gpu, proxy, std::move(csxf), filter, wideColor));
    }

    const char* name() const override { return "LatticeGP"; }

    void getGLSLProcessorKey(const GrShaderCaps&, GrProcessorKeyBuilder* b) const override {
        b->add32(GrColorSpaceXform::XformKey(fColorSpaceXform.get()));
    }

    GrGLSLPrimitiveProcessor* createGLSLInstance(const GrShaderCaps& caps) const override {
        class GLSLProcessor : public GrGLSLGeometryProcessor {
        public:
            void setData(const GrGLSLProgramDataManager& pdman, const GrPrimitiveProcessor& proc,
                         FPCoordTransformIter&& transformIter) override {
                const auto& latticeGP = proc.cast<LatticeGP>();
                this->setTransformDataHelper(SkMatrix::I(), pdman, &transformIter);
                fColorSpaceXformHelper.setData(pdman, latticeGP.fColorSpaceXform.get());
            }

        private:
            void onEmitCode(EmitArgs& args, GrGPArgs* gpArgs) override {
                using Interpolation = GrGLSLVaryingHandler::Interpolation;
                const auto& latticeGP = args.fGP.cast<LatticeGP>();
                fColorSpaceXformHelper.emitCode(args.fUniformHandler,
                                                latticeGP.fColorSpaceXform.get());

                args.fVaryingHandler->emitAttributes(latticeGP);
                this->writeOutputPosition(args.fVertBuilder, gpArgs, latticeGP.fInPosition.name());
                this->emitTransforms(args.fVertBuilder,
                                     args.fVaryingHandler,
                                     args.fUniformHandler,
                                     latticeGP.fInTextureCoords.asShaderVar(),
                                     args.fFPCoordTransformHandler);
                args.fFragBuilder->codeAppend("float2 textureCoords;");
                args.fVaryingHandler->addPassThroughAttribute(latticeGP.fInTextureCoords,
                                                              "textureCoords");
                args.fFragBuilder->codeAppend("float4 textureDomain;");
                args.fVaryingHandler->addPassThroughAttribute(
                        latticeGP.fInTextureDomain, "textureDomain", Interpolation::kCanBeFlat);
                args.fVaryingHandler->addPassThroughAttribute(latticeGP.fInColor,
                                                              args.fOutputColor,
                                                              Interpolation::kCanBeFlat);
                args.fFragBuilder->codeAppendf("%s = ", args.fOutputColor);
                args.fFragBuilder->appendTextureLookupAndModulate(
                        args.fOutputColor,
                        args.fTexSamplers[0],
                        "clamp(textureCoords, textureDomain.xy, textureDomain.zw)",
                        kFloat2_GrSLType,
                        &fColorSpaceXformHelper);
                args.fFragBuilder->codeAppend(";");
                args.fFragBuilder->codeAppendf("%s = half4(1);", args.fOutputCoverage);
            }
            GrGLSLColorSpaceXformHelper fColorSpaceXformHelper;
        };
        return new GLSLProcessor;
    }

private:
    LatticeGP(GrGpu* gpu, const GrTextureProxy* proxy, sk_sp<GrColorSpaceXform> csxf,
              GrSamplerState::Filter filter, bool wideColor)
            : INHERITED(kLatticeGP_ClassID), fColorSpaceXform(std::move(csxf)) {

        GrSamplerState samplerState = GrSamplerState(GrSamplerState::WrapMode::kClamp,
                                                     filter);
        uint32_t extraSamplerKey = gpu->getExtraSamplerKeyForProgram(samplerState,
                                                                     proxy->backendFormat());

        fSampler.reset(proxy->textureType(), proxy->config(), samplerState,
                       extraSamplerKey);
        this->setTextureSamplerCnt(1);
        fInPosition = {"position", kFloat2_GrVertexAttribType, kFloat2_GrSLType};
        fInTextureCoords = {"textureCoords", kFloat2_GrVertexAttribType, kFloat2_GrSLType};
        fInTextureDomain = {"textureDomain", kFloat4_GrVertexAttribType, kFloat4_GrSLType};
        fInColor = MakeColorAttribute("color", wideColor);
        this->setVertexAttributes(&fInPosition, 4);
    }

    const TextureSampler& onTextureSampler(int) const override { return fSampler; }

    Attribute fInPosition;
    Attribute fInTextureCoords;
    Attribute fInTextureDomain;
    Attribute fInColor;

    sk_sp<GrColorSpaceXform> fColorSpaceXform;
    TextureSampler fSampler;

    typedef GrGeometryProcessor INHERITED;
};

>>>>>>> upstream-releases
class NonAALatticeOp final : public GrMeshDrawOp {
private:
    using Helper = GrSimpleMeshDrawOpHelper;

public:
    DEFINE_OP_CLASS_ID

    static const int kVertsPerRect = 4;
    static const int kIndicesPerRect = 6;

<<<<<<< HEAD
    static std::unique_ptr<GrDrawOp> Make(GrContext* context,
                                          GrPaint&& paint,
                                          const SkMatrix& viewMatrix,
                                          sk_sp<GrTextureProxy> proxy,
                                          sk_sp<GrColorSpaceXform> colorSpaceXForm,
                                          GrSamplerState::Filter filter,
                                          std::unique_ptr<SkLatticeIter> iter,
                                          const SkRect& dst) {
        SkASSERT(proxy);
        return Helper::FactoryHelper<NonAALatticeOp>(context, std::move(paint), viewMatrix,
                                                     std::move(proxy),
                                                     std::move(colorSpaceXForm), filter,
                                                     std::move(iter), dst);
||||||| merged common ancestors
    static std::unique_ptr<GrDrawOp> Make(GrPaint&& paint, const SkMatrix& viewMatrix,
                                          int imageWidth, int imageHeight,
                                          std::unique_ptr<SkLatticeIter> iter, const SkRect& dst) {
        return Helper::FactoryHelper<NonAALatticeOp>(std::move(paint), viewMatrix, imageWidth,
                                                     imageHeight, std::move(iter), dst);
=======
    static std::unique_ptr<GrDrawOp> Make(GrRecordingContext* context,
                                          GrPaint&& paint,
                                          const SkMatrix& viewMatrix,
                                          sk_sp<GrTextureProxy> proxy,
                                          sk_sp<GrColorSpaceXform> colorSpaceXForm,
                                          GrSamplerState::Filter filter,
                                          std::unique_ptr<SkLatticeIter> iter,
                                          const SkRect& dst) {
        SkASSERT(proxy);
        return Helper::FactoryHelper<NonAALatticeOp>(context, std::move(paint), viewMatrix,
                                                     std::move(proxy),
                                                     std::move(colorSpaceXForm), filter,
                                                     std::move(iter), dst);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    NonAALatticeOp(Helper::MakeArgs& helperArgs, GrColor color, const SkMatrix& viewMatrix,
                   sk_sp<GrTextureProxy> proxy, sk_sp<GrColorSpaceXform> colorSpaceXform,
                   GrSamplerState::Filter filter, std::unique_ptr<SkLatticeIter> iter,
                   const SkRect& dst)
            : INHERITED(ClassID())
            , fHelper(helperArgs, GrAAType::kNone)
            , fProxy(std::move(proxy))
            , fColorSpaceXform(std::move(colorSpaceXform))
            , fFilter(filter) {
||||||| merged common ancestors
    NonAALatticeOp(Helper::MakeArgs& helperArgs, GrColor color, const SkMatrix& viewMatrix,
                   int imageWidth, int imageHeight, std::unique_ptr<SkLatticeIter> iter,
                   const SkRect& dst)
            : INHERITED(ClassID()), fHelper(helperArgs, GrAAType::kNone) {
=======
    NonAALatticeOp(Helper::MakeArgs& helperArgs, const SkPMColor4f& color,
                   const SkMatrix& viewMatrix, sk_sp<GrTextureProxy> proxy,
                   sk_sp<GrColorSpaceXform> colorSpaceXform, GrSamplerState::Filter filter,
                   std::unique_ptr<SkLatticeIter> iter, const SkRect& dst)
            : INHERITED(ClassID())
            , fHelper(helperArgs, GrAAType::kNone)
            , fProxy(std::move(proxy))
            , fColorSpaceXform(std::move(colorSpaceXform))
            , fFilter(filter) {
>>>>>>> upstream-releases
        Patch& patch = fPatches.push_back();
        patch.fViewMatrix = viewMatrix;
        patch.fColor = color;
        patch.fIter = std::move(iter);
        patch.fDst = dst;

        // setup bounds
        this->setTransformedBounds(patch.fDst, viewMatrix, HasAABloat::kNo, IsZeroArea::kNo);
        fWideColor = !SkPMColor4fFitsInBytes(color);
    }

    const char* name() const override { return "NonAALatticeOp"; }

<<<<<<< HEAD
    void visitProxies(const VisitProxyFunc& func) const override {
        func(fProxy.get());
||||||| merged common ancestors
    void visitProxies(const VisitProxyFunc& func) const override {
=======
    void visitProxies(const VisitProxyFunc& func, VisitorType) const override {
        func(fProxy.get());
>>>>>>> upstream-releases
        fHelper.visitProxies(func);
    }

#ifdef SK_DEBUG
    SkString dumpInfo() const override {
        SkString str;

        for (int i = 0; i < fPatches.count(); ++i) {
            str.appendf("%d: Color: 0x%08x Dst [L: %.2f, T: %.2f, R: %.2f, B: %.2f]\n", i,
                        fPatches[i].fColor.toBytes_RGBA(), fPatches[i].fDst.fLeft,
                        fPatches[i].fDst.fTop, fPatches[i].fDst.fRight, fPatches[i].fDst.fBottom);
        }

        str += fHelper.dumpInfo();
        str += INHERITED::dumpInfo();
        return str;
    }
#endif

    FixedFunctionFlags fixedFunctionFlags() const override { return fHelper.fixedFunctionFlags(); }

<<<<<<< HEAD
    RequiresDstTexture finalize(const GrCaps& caps, const GrAppliedClip* clip) override {
        auto opaque = GrColorIsOpaque(fPatches[0].fColor) && GrPixelConfigIsOpaque(fProxy->config())
                              ? GrProcessorAnalysisColor::Opaque::kYes
                              : GrProcessorAnalysisColor::Opaque::kNo;
        auto analysisColor = GrProcessorAnalysisColor(opaque);
        auto result = fHelper.xpRequiresDstTexture(
                caps, clip, GrProcessorAnalysisCoverage::kNone, &analysisColor);
        analysisColor.isConstant(&fPatches[0].fColor);
        return result;
||||||| merged common ancestors
    RequiresDstTexture finalize(const GrCaps& caps, const GrAppliedClip* clip,
                                GrPixelConfigIsClamped dstIsClamped) override {
        return fHelper.xpRequiresDstTexture(caps, clip, dstIsClamped,
                                            GrProcessorAnalysisCoverage::kNone,
                                            &fPatches.front().fColor);
=======
    GrProcessorSet::Analysis finalize(
            const GrCaps& caps, const GrAppliedClip* clip, GrFSAAType fsaaType) override {
        auto opaque = fPatches[0].fColor.isOpaque() && GrPixelConfigIsOpaque(fProxy->config())
                              ? GrProcessorAnalysisColor::Opaque::kYes
                              : GrProcessorAnalysisColor::Opaque::kNo;
        auto analysisColor = GrProcessorAnalysisColor(opaque);
        auto result = fHelper.finalizeProcessors(
                caps, clip, fsaaType, GrProcessorAnalysisCoverage::kNone, &analysisColor);
        analysisColor.isConstant(&fPatches[0].fColor);
        return result;
>>>>>>> upstream-releases
    }

private:
    void onPrepareDraws(Target* target) override {
<<<<<<< HEAD
        auto gp = LatticeGP::Make(fProxy.get(), fColorSpaceXform, fFilter);
||||||| merged common ancestors
        sk_sp<GrGeometryProcessor> gp(create_gp());
=======
        GrGpu* gpu = target->resourceProvider()->priv().gpu();
        auto gp = LatticeGP::Make(gpu, fProxy.get(), fColorSpaceXform, fFilter, fWideColor);
>>>>>>> upstream-releases
        if (!gp) {
            SkDebugf("Couldn't create GrGeometryProcessor\n");
            return;
        }

<<<<<<< HEAD
        static constexpr size_t kVertexStide =
                sizeof(SkPoint) + sizeof(SkPoint) + sizeof(SkRect) + sizeof(uint32_t);
        SkASSERT(kVertexStide == gp->debugOnly_vertexStride());

||||||| merged common ancestors
        size_t vertexStride = gp->getVertexStride();
=======
>>>>>>> upstream-releases
        int patchCnt = fPatches.count();
        int numRects = 0;
        for (int i = 0; i < patchCnt; i++) {
            numRects += fPatches[i].fIter->numRectsToDraw();
        }

        if (!numRects) {
            return;
        }

        const size_t kVertexStride = gp->vertexStride();
        sk_sp<const GrBuffer> indexBuffer = target->resourceProvider()->refQuadIndexBuffer();
<<<<<<< HEAD
        PatternHelper helper(target, GrPrimitiveType::kTriangles, kVertexStide, indexBuffer.get(),
                             kVertsPerRect, kIndicesPerRect, numRects);
        void* vertices = helper.vertices();
        if (!vertices || !indexBuffer) {
||||||| merged common ancestors
        PatternHelper helper(GrPrimitiveType::kTriangles);
        void* vertices = helper.init(target, vertexStride, indexBuffer.get(), kVertsPerRect,
                                     kIndicesPerRect, numRects);
        if (!vertices || !indexBuffer) {
=======
        if (!indexBuffer) {
            SkDebugf("Could not allocate indices\n");
            return;
        }
        PatternHelper helper(target, GrPrimitiveType::kTriangles, kVertexStride,
                             std::move(indexBuffer), kVertsPerRect, kIndicesPerRect, numRects);
        GrVertexWriter vertices{helper.vertices()};
        if (!vertices.fPtr) {
>>>>>>> upstream-releases
            SkDebugf("Could not allocate vertices\n");
            return;
        }

        for (int i = 0; i < patchCnt; i++) {
            const Patch& patch = fPatches[i];

            GrVertexColor patchColor(patch.fColor, fWideColor);

            // Apply the view matrix here if it is scale-translate.  Otherwise, we need to
            // wait until we've created the dst rects.
            bool isScaleTranslate = patch.fViewMatrix.isScaleTranslate();
            if (isScaleTranslate) {
                patch.fIter->mapDstScaleTranslate(patch.fViewMatrix);
            }

<<<<<<< HEAD
            SkIRect srcR;
            SkRect dstR;
            intptr_t patchVerts = verts;
            Sk4f scales(1.f / fProxy->width(), 1.f / fProxy->height(),
                        1.f / fProxy->width(), 1.f / fProxy->height());
            static const Sk4f kDomainOffsets(0.5f, 0.5f, -0.5f, -0.5f);
            static const Sk4f kFlipOffsets(0.f, 1, 0.f, 1.f);
            static const Sk4f kFlipMuls(1.f, -1.f, 1.f, -1.f);
||||||| merged common ancestors
            SkRect srcR, dstR;
            intptr_t patchVerts = verts;
=======
            SkIRect srcR;
            SkRect dstR;
            SkPoint* patchPositions = reinterpret_cast<SkPoint*>(vertices.fPtr);
            Sk4f scales(1.f / fProxy->width(), 1.f / fProxy->height(),
                        1.f / fProxy->width(), 1.f / fProxy->height());
            static const Sk4f kDomainOffsets(0.5f, 0.5f, -0.5f, -0.5f);
            static const Sk4f kFlipOffsets(0.f, 1.f, 0.f, 1.f);
            static const Sk4f kFlipMuls(1.f, -1.f, 1.f, -1.f);
>>>>>>> upstream-releases
            while (patch.fIter->next(&srcR, &dstR)) {
<<<<<<< HEAD
                auto vertices = reinterpret_cast<LatticeGP::Vertex*>(verts);
                SkPointPriv::SetRectTriStrip(&vertices->fPosition, dstR, kVertexStide);
                Sk4f coords(SkIntToScalar(srcR.fLeft), SkIntToScalar(srcR.fTop),
                            SkIntToScalar(srcR.fRight), SkIntToScalar(srcR.fBottom));
                Sk4f domain = coords + kDomainOffsets;
                coords *= scales;
                domain *= scales;
                if (fProxy->origin() == kBottomLeft_GrSurfaceOrigin) {
                    coords = kFlipMuls * coords + kFlipOffsets;
                    domain = SkNx_shuffle<0, 3, 2, 1>(kFlipMuls * domain + kFlipOffsets);
||||||| merged common ancestors
                SkPoint* positions = reinterpret_cast<SkPoint*>(verts);
                SkPointPriv::SetRectTriStrip(positions, dstR.fLeft, dstR.fTop, dstR.fRight,
                                             dstR.fBottom, vertexStride);

                // Setup local coords
                static const int kLocalOffset = sizeof(SkPoint) + sizeof(GrColor);
                SkPoint* coords = reinterpret_cast<SkPoint*>(verts + kLocalOffset);
                SkPointPriv::SetRectTriStrip(coords, srcR.fLeft, srcR.fTop, srcR.fRight,
                                             srcR.fBottom, vertexStride);

                static const int kColorOffset = sizeof(SkPoint);
                GrColor* vertColor = reinterpret_cast<GrColor*>(verts + kColorOffset);
                for (int j = 0; j < 4; ++j) {
                    *vertColor = patch.fColor;
                    vertColor = (GrColor*)((intptr_t)vertColor + vertexStride);
=======
                Sk4f coords(SkIntToScalar(srcR.fLeft), SkIntToScalar(srcR.fTop),
                            SkIntToScalar(srcR.fRight), SkIntToScalar(srcR.fBottom));
                Sk4f domain = coords + kDomainOffsets;
                coords *= scales;
                domain *= scales;
                if (fProxy->origin() == kBottomLeft_GrSurfaceOrigin) {
                    coords = kFlipMuls * coords + kFlipOffsets;
                    domain = SkNx_shuffle<0, 3, 2, 1>(kFlipMuls * domain + kFlipOffsets);
>>>>>>> upstream-releases
                }
<<<<<<< HEAD
                SkPointPriv::SetRectTriStrip(&vertices->fTextureCoords, coords[0], coords[1],
                                             coords[2], coords[3], kVertexStide);
                for (int j = 0; j < kVertsPerRect; ++j) {
                    vertices[j].fTextureDomain = {domain[0], domain[1], domain[2], domain[3]};
                }

                for (int j = 0; j < kVertsPerRect; ++j) {
                    vertices[j].fColor = patch.fColor;
                }
                verts += kVertsPerRect * kVertexStide;
||||||| merged common ancestors
                verts += kVertsPerRect * vertexStride;
=======
                SkRect texDomain;
                SkRect texCoords;
                domain.store(&texDomain);
                coords.store(&texCoords);

                vertices.writeQuad(GrVertexWriter::TriStripFromRect(dstR),
                                   GrVertexWriter::TriStripFromRect(texCoords),
                                   texDomain,
                                   patchColor);
>>>>>>> upstream-releases
            }

            // If we didn't handle it above, apply the matrix here.
            if (!isScaleTranslate) {
<<<<<<< HEAD
                SkPoint* positions = reinterpret_cast<SkPoint*>(patchVerts);
                SkMatrixPriv::MapPointsWithStride(patch.fViewMatrix, positions, kVertexStide,
||||||| merged common ancestors
                SkPoint* positions = reinterpret_cast<SkPoint*>(patchVerts);
                SkMatrixPriv::MapPointsWithStride(patch.fViewMatrix, positions, vertexStride,
=======
                SkMatrixPriv::MapPointsWithStride(patch.fViewMatrix, patchPositions, kVertexStride,
>>>>>>> upstream-releases
                                                  kVertsPerRect * patch.fIter->numRectsToDraw());
            }
        }
<<<<<<< HEAD
        auto pipe = fHelper.makePipeline(target, 1);
        pipe.fFixedDynamicState->fPrimitiveProcessorTextures[0] = fProxy.get();
        helper.recordDraw(target, std::move(gp), pipe.fPipeline, pipe.fFixedDynamicState);
||||||| merged common ancestors
        helper.recordDraw(target, gp.get(), fHelper.makePipeline(target));
=======
        auto fixedDynamicState = target->makeFixedDynamicState(1);
        fixedDynamicState->fPrimitiveProcessorTextures[0] = fProxy.get();
        helper.recordDraw(target, std::move(gp), fixedDynamicState);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    CombineResult onCombineIfPossible(GrOp* t, const GrCaps& caps) override {
||||||| merged common ancestors
    bool onCombineIfPossible(GrOp* t, const GrCaps& caps) override {
=======
    void onExecute(GrOpFlushState* flushState, const SkRect& chainBounds) override {
        fHelper.executeDrawsAndUploads(this, flushState, chainBounds);
    }

    CombineResult onCombineIfPossible(GrOp* t, const GrCaps& caps) override {
>>>>>>> upstream-releases
        NonAALatticeOp* that = t->cast<NonAALatticeOp>();
        if (fProxy != that->fProxy) {
            return CombineResult::kCannotCombine;
        }
        if (fFilter != that->fFilter) {
            return CombineResult::kCannotCombine;
        }
        if (GrColorSpaceXform::Equals(fColorSpaceXform.get(), that->fColorSpaceXform.get())) {
            return CombineResult::kCannotCombine;
        }
        if (!fHelper.isCompatible(that->fHelper, caps, this->bounds(), that->bounds())) {
            return CombineResult::kCannotCombine;
        }

        fPatches.move_back_n(that->fPatches.count(), that->fPatches.begin());
<<<<<<< HEAD
        this->joinBounds(*that);
        return CombineResult::kMerged;
||||||| merged common ancestors
        this->joinBounds(*that);
        return true;
=======
        fWideColor |= that->fWideColor;
        return CombineResult::kMerged;
>>>>>>> upstream-releases
    }

    struct Patch {
        SkMatrix fViewMatrix;
        std::unique_ptr<SkLatticeIter> fIter;
        SkRect fDst;
        SkPMColor4f fColor;
    };

    Helper fHelper;
    SkSTArray<1, Patch, true> fPatches;
<<<<<<< HEAD
    sk_sp<GrTextureProxy> fProxy;
    sk_sp<GrColorSpaceXform> fColorSpaceXform;
    GrSamplerState::Filter fFilter;
||||||| merged common ancestors
    int fImageWidth;
    int fImageHeight;
=======
    sk_sp<GrTextureProxy> fProxy;
    sk_sp<GrColorSpaceXform> fColorSpaceXform;
    GrSamplerState::Filter fFilter;
    bool fWideColor;
>>>>>>> upstream-releases

    typedef GrMeshDrawOp INHERITED;
};

}  // anonymous namespace

namespace GrLatticeOp {
<<<<<<< HEAD
std::unique_ptr<GrDrawOp> MakeNonAA(GrContext* context,
                                    GrPaint&& paint,
                                    const SkMatrix& viewMatrix,
                                    sk_sp<GrTextureProxy> proxy,
                                    sk_sp<GrColorSpaceXform> colorSpaceXform,
                                    GrSamplerState::Filter filter,
                                    std::unique_ptr<SkLatticeIter> iter,
||||||| merged common ancestors
std::unique_ptr<GrDrawOp> MakeNonAA(GrPaint&& paint, const SkMatrix& viewMatrix, int imageWidth,
                                    int imageHeight, std::unique_ptr<SkLatticeIter> iter,
=======
std::unique_ptr<GrDrawOp> MakeNonAA(GrRecordingContext* context,
                                    GrPaint&& paint,
                                    const SkMatrix& viewMatrix,
                                    sk_sp<GrTextureProxy> proxy,
                                    sk_sp<GrColorSpaceXform> colorSpaceXform,
                                    GrSamplerState::Filter filter,
                                    std::unique_ptr<SkLatticeIter> iter,
>>>>>>> upstream-releases
                                    const SkRect& dst) {
    return NonAALatticeOp::Make(context, std::move(paint), viewMatrix, std::move(proxy),
                                std::move(colorSpaceXform), filter, std::move(iter), dst);
}
};

#if GR_TEST_UTILS
<<<<<<< HEAD
#include "GrContextPriv.h"
#include "GrProxyProvider.h"
||||||| merged common ancestors
=======
#include "GrProxyProvider.h"
#include "GrRecordingContextPriv.h"
>>>>>>> upstream-releases

/** Randomly divides subset into count divs. */
static void init_random_divs(int divs[], int count, int subsetStart, int subsetStop,
                             SkRandom* random) {
    // Rules for lattice divs: Must be strictly increasing and in the range
    // [subsetStart, subsetStop).
    // Not terribly efficient alg for generating random divs:
    // 1) Start with minimum legal pixels between each div.
    // 2) Randomly assign the remaining pixels of the subset to divs.
    // 3) Convert from pixel counts to div offsets.

    // 1) Initially each divs[i] represents the number of pixels between
    // div i-1 and i. The initial div is allowed to be at subsetStart. There
    // must be one pixel spacing between subsequent divs.
    divs[0] = 0;
    for (int i = 1; i < count; ++i) {
        divs[i] = 1;
    }
    // 2) Assign the remaining subset pixels to fall
    int subsetLength = subsetStop - subsetStart;
    for (int i = 0; i < subsetLength - count; ++i) {
        // +1 because count divs means count+1 intervals.
        int entry = random->nextULessThan(count + 1);
        // We don't have an entry to  to store the count after the last div
        if (entry < count) {
            divs[entry]++;
        }
    }
    // 3) Now convert the counts between divs to pixel indices, incorporating the subset's offset.
    int offset = subsetStart;
    for (int i = 0; i < count; ++i) {
        divs[i] += offset;
        offset = divs[i];
    }
}

GR_DRAW_OP_TEST_DEFINE(NonAALatticeOp) {
    SkCanvas::Lattice lattice;
    // We loop because our random lattice code can produce an invalid lattice in the case where
    // there is a single div separator in both x and y and both are aligned with the left and top
    // edge of the image subset, respectively.
    std::unique_ptr<int[]> xdivs;
    std::unique_ptr<int[]> ydivs;
    std::unique_ptr<SkCanvas::Lattice::RectType[]> flags;
    std::unique_ptr<SkColor[]> colors;
    SkIRect subset;
<<<<<<< HEAD
    GrSurfaceDesc desc;
    desc.fConfig = kRGBA_8888_GrPixelConfig;
    desc.fWidth = random->nextRangeU(1, 1000);
    desc.fHeight = random->nextRangeU(1, 1000);
    GrSurfaceOrigin origin =
            random->nextBool() ? kTopLeft_GrSurfaceOrigin : kBottomLeft_GrSurfaceOrigin;
    auto proxy = context->contextPriv().proxyProvider()->createProxy(
            desc, origin, SkBackingFit::kExact, SkBudgeted::kYes);

||||||| merged common ancestors
=======
    GrSurfaceDesc desc;
    desc.fConfig = kRGBA_8888_GrPixelConfig;
    desc.fWidth = random->nextRangeU(1, 1000);
    desc.fHeight = random->nextRangeU(1, 1000);
    GrSurfaceOrigin origin =
            random->nextBool() ? kTopLeft_GrSurfaceOrigin : kBottomLeft_GrSurfaceOrigin;
    const GrBackendFormat format =
            context->priv().caps()->getBackendFormatFromColorType(kRGBA_8888_SkColorType);
    auto proxy = context->priv().proxyProvider()->createProxy(
            format, desc, origin, SkBackingFit::kExact, SkBudgeted::kYes);

>>>>>>> upstream-releases
    do {
        if (random->nextBool()) {
            subset.fLeft = random->nextULessThan(desc.fWidth);
            subset.fRight = random->nextRangeU(subset.fLeft + 1, desc.fWidth);
            subset.fTop = random->nextULessThan(desc.fHeight);
            subset.fBottom = random->nextRangeU(subset.fTop + 1, desc.fHeight);
        } else {
            subset.setXYWH(0, 0, desc.fWidth, desc.fHeight);
        }
        // SkCanvas::Lattice allows bounds to be null. However, SkCanvas creates a temp Lattice with
        // a non-null bounds before creating a SkLatticeIter since SkLatticeIter requires a bounds.
        lattice.fBounds = &subset;
        lattice.fXCount = random->nextRangeU(1, subset.width());
        lattice.fYCount = random->nextRangeU(1, subset.height());
        xdivs.reset(new int[lattice.fXCount]);
        ydivs.reset(new int[lattice.fYCount]);
        init_random_divs(xdivs.get(), lattice.fXCount, subset.fLeft, subset.fRight, random);
        init_random_divs(ydivs.get(), lattice.fYCount, subset.fTop, subset.fBottom, random);
        lattice.fXDivs = xdivs.get();
        lattice.fYDivs = ydivs.get();
        bool hasFlags = random->nextBool();
        if (hasFlags) {
            int n = (lattice.fXCount + 1) * (lattice.fYCount + 1);
            flags.reset(new SkCanvas::Lattice::RectType[n]);
            colors.reset(new SkColor[n]);
            for (int i = 0; i < n; ++i) {
                flags[i] = random->nextBool() ? SkCanvas::Lattice::kTransparent
                                              : SkCanvas::Lattice::kDefault;
            }
            lattice.fRectTypes = flags.get();
            lattice.fColors = colors.get();
        } else {
            lattice.fRectTypes = nullptr;
            lattice.fColors = nullptr;
        }
    } while (!SkLatticeIter::Valid(desc.fWidth, desc.fHeight, lattice));
    SkRect dst;
    dst.fLeft = random->nextRangeScalar(-2000.5f, 1000.f);
    dst.fTop = random->nextRangeScalar(-2000.5f, 1000.f);
    dst.fRight = dst.fLeft + random->nextRangeScalar(0.5f, 1000.f);
    dst.fBottom = dst.fTop + random->nextRangeScalar(0.5f, 1000.f);
    std::unique_ptr<SkLatticeIter> iter(new SkLatticeIter(lattice, dst));
    SkMatrix viewMatrix = GrTest::TestMatrixPreservesRightAngles(random);
    auto csxf = GrTest::TestColorXform(random);
    GrSamplerState::Filter filter =
            random->nextBool() ? GrSamplerState::Filter::kNearest : GrSamplerState::Filter::kBilerp;
    return NonAALatticeOp::Make(context, std::move(paint), viewMatrix, std::move(proxy),
                                std::move(csxf), filter, std::move(iter), dst);
}

#endif
