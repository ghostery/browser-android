/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrTextureOp.h"
<<<<<<< HEAD

||||||| merged common ancestors
=======
#include <new>
>>>>>>> upstream-releases
#include "GrAppliedClip.h"
#include "GrCaps.h"
#include "GrContext.h"
#include "GrContextPriv.h"
#include "GrDrawOpTest.h"
#include "GrGeometryProcessor.h"
<<<<<<< HEAD
#include "GrMemoryPool.h"
||||||| merged common ancestors
=======
#include "GrGpu.h"
#include "GrMemoryPool.h"
>>>>>>> upstream-releases
#include "GrMeshDrawOp.h"
#include "GrOpFlushState.h"
#include "GrQuad.h"
#include "GrQuadPerEdgeAA.h"
#include "GrRecordingContext.h"
#include "GrRecordingContextPriv.h"
#include "GrResourceProvider.h"
#include "GrResourceProviderPriv.h"
#include "GrShaderCaps.h"
#include "GrTexture.h"
#include "GrTexturePriv.h"
#include "GrTextureProxy.h"
#include "SkGr.h"
#include "SkMathPriv.h"
#include "SkMatrixPriv.h"
#include "SkPoint.h"
#include "SkPoint3.h"
<<<<<<< HEAD
#include "SkTo.h"
#include "glsl/GrGLSLColorSpaceXformHelper.h"
#include "glsl/GrGLSLFragmentShaderBuilder.h"
#include "glsl/GrGLSLGeometryProcessor.h"
||||||| merged common ancestors
#include "glsl/GrGLSLColorSpaceXformHelper.h"
#include "glsl/GrGLSLFragmentShaderBuilder.h"
#include "glsl/GrGLSLGeometryProcessor.h"
=======
#include "SkRectPriv.h"
#include "SkTo.h"
>>>>>>> upstream-releases
#include "glsl/GrGLSLVarying.h"
<<<<<<< HEAD
#include "glsl/GrGLSLVertexGeoBuilder.h"
#include <new>
||||||| merged common ancestors
#include "glsl/GrGLSLVertexGeoBuilder.h"
=======
>>>>>>> upstream-releases

namespace {

<<<<<<< HEAD
enum class Domain : bool { kNo = false, kYes = true };

/**
 * Geometry Processor that draws a texture modulated by a vertex color (though, this is meant to be
 * the same value across all vertices of a quad and uses flat interpolation when available). This is
 * used by TextureOp below.
 */
class TextureGeometryProcessor : public GrGeometryProcessor {
public:
    template <typename Pos> struct VertexCommon {
        using Position = Pos;
        Position fPosition;
        GrColor fColor;
        SkPoint fTextureCoords;
    };

    template <typename Pos, Domain D> struct OptionalDomainVertex;
    template <typename Pos>
    struct OptionalDomainVertex<Pos, Domain::kNo> : VertexCommon<Pos> {
        static constexpr Domain kDomain = Domain::kNo;
    };
    template <typename Pos>
    struct OptionalDomainVertex<Pos, Domain::kYes> : VertexCommon<Pos> {
        static constexpr Domain kDomain = Domain::kYes;
        SkRect fTextureDomain;
    };

    template <typename Pos, Domain D, GrAA> struct OptionalAAVertex;
    template <typename Pos, Domain D>
    struct OptionalAAVertex<Pos, D, GrAA::kNo> : OptionalDomainVertex<Pos, D> {
        static constexpr GrAA kAA = GrAA::kNo;
    };
    template <typename Pos, Domain D>
    struct OptionalAAVertex<Pos, D, GrAA::kYes> : OptionalDomainVertex<Pos, D> {
        static constexpr GrAA kAA = GrAA::kYes;
        SkPoint3 fEdges[4];
    };

    template <typename Pos, Domain D, GrAA AA>
    using Vertex = OptionalAAVertex<Pos, D, AA>;

    static sk_sp<GrGeometryProcessor> Make(GrTextureType textureType, GrPixelConfig textureConfig,
                                           const GrSamplerState::Filter filter,
                                           sk_sp<GrColorSpaceXform> textureColorSpaceXform,
                                           sk_sp<GrColorSpaceXform> paintColorSpaceXform,
                                           bool coverageAA, bool perspective, Domain domain,
                                           const GrShaderCaps& caps) {
        return sk_sp<TextureGeometryProcessor>(new TextureGeometryProcessor(
                textureType, textureConfig, filter, std::move(textureColorSpaceXform),
                std::move(paintColorSpaceXform), coverageAA, perspective, domain, caps));
    }

    const char* name() const override { return "TextureGeometryProcessor"; }

    void getGLSLProcessorKey(const GrShaderCaps&, GrProcessorKeyBuilder* b) const override {
        b->add32(GrColorSpaceXform::XformKey(fTextureColorSpaceXform.get()));
        b->add32(GrColorSpaceXform::XformKey(fPaintColorSpaceXform.get()));
        uint32_t x = this->usesCoverageEdgeAA() ? 0 : 1;
        x |= kFloat3_GrVertexAttribType == fPositions.cpuType() ? 0 : 2;
        x |= fDomain.isInitialized() ? 4 : 0;
        b->add32(x);
    }

    GrGLSLPrimitiveProcessor* createGLSLInstance(const GrShaderCaps& caps) const override {
        class GLSLProcessor : public GrGLSLGeometryProcessor {
        public:
            void setData(const GrGLSLProgramDataManager& pdman, const GrPrimitiveProcessor& proc,
                         FPCoordTransformIter&& transformIter) override {
                const auto& textureGP = proc.cast<TextureGeometryProcessor>();
                this->setTransformDataHelper(SkMatrix::I(), pdman, &transformIter);
                fTextureColorSpaceXformHelper.setData(
                        pdman, textureGP.fTextureColorSpaceXform.get());
                fPaintColorSpaceXformHelper.setData(pdman, textureGP.fPaintColorSpaceXform.get());
            }

        private:
            void onEmitCode(EmitArgs& args, GrGPArgs* gpArgs) override {
                using Interpolation = GrGLSLVaryingHandler::Interpolation;
                const auto& textureGP = args.fGP.cast<TextureGeometryProcessor>();
                fTextureColorSpaceXformHelper.emitCode(
                        args.fUniformHandler, textureGP.fTextureColorSpaceXform.get());
                fPaintColorSpaceXformHelper.emitCode(
                        args.fUniformHandler, textureGP.fPaintColorSpaceXform.get(),
                        kVertex_GrShaderFlag);
                if (kFloat2_GrVertexAttribType == textureGP.fPositions.cpuType()) {
                    args.fVaryingHandler->setNoPerspective();
                }
                args.fVaryingHandler->emitAttributes(textureGP);
                gpArgs->fPositionVar = textureGP.fPositions.asShaderVar();

                this->emitTransforms(args.fVertBuilder,
                                     args.fVaryingHandler,
                                     args.fUniformHandler,
                                     textureGP.fTextureCoords.asShaderVar(),
                                     args.fFPCoordTransformHandler);
                if (fPaintColorSpaceXformHelper.isNoop()) {
                    args.fVaryingHandler->addPassThroughAttribute(
                            textureGP.fColors, args.fOutputColor, Interpolation::kCanBeFlat);
                } else {
                    GrGLSLVarying varying(kHalf4_GrSLType);
                    args.fVaryingHandler->addVarying("color", &varying);
                    args.fVertBuilder->codeAppend("half4 color = ");
                    args.fVertBuilder->appendColorGamutXform(textureGP.fColors.name(),
                                                             &fPaintColorSpaceXformHelper);
                    args.fVertBuilder->codeAppend(";");
                    args.fVertBuilder->codeAppendf("%s = half4(color.rgb * color.a, color.a);",
                                                   varying.vsOut());
                    args.fFragBuilder->codeAppendf("%s = %s;", args.fOutputColor, varying.fsIn());
                }
                args.fFragBuilder->codeAppend("float2 texCoord;");
                args.fVaryingHandler->addPassThroughAttribute(textureGP.fTextureCoords, "texCoord");
                if (textureGP.fDomain.isInitialized()) {
                    args.fFragBuilder->codeAppend("float4 domain;");
                    args.fVaryingHandler->addPassThroughAttribute(
                            textureGP.fDomain, "domain",
                            GrGLSLVaryingHandler::Interpolation::kCanBeFlat);
                    args.fFragBuilder->codeAppend(
                            "texCoord = clamp(texCoord, domain.xy, domain.zw);");
                }
                args.fFragBuilder->codeAppendf("%s = ", args.fOutputColor);
                args.fFragBuilder->appendTextureLookupAndModulate(
                        args.fOutputColor, args.fTexSamplers[0], "texCoord", kFloat2_GrSLType,
                        &fTextureColorSpaceXformHelper);
                args.fFragBuilder->codeAppend(";");
                if (textureGP.usesCoverageEdgeAA()) {
                    bool mulByFragCoordW = false;
                    GrGLSLVarying aaDistVarying(kFloat4_GrSLType,
                                                GrGLSLVarying::Scope::kVertToFrag);
                    if (kFloat3_GrVertexAttribType == textureGP.fPositions.cpuType()) {
                        args.fVaryingHandler->addVarying("aaDists", &aaDistVarying);
                        // The distance from edge equation e to homogeneous point p=sk_Position
                        // is e.x*p.x/p.w + e.y*p.y/p.w + e.z. However, we want screen space
                        // interpolation of this distance. We can do this by multiplying the
                        // varying in the VS by p.w and then multiplying by sk_FragCoord.w in
                        // the FS. So we output e.x*p.x + e.y*p.y + e.z * p.w
                        args.fVertBuilder->codeAppendf(
                                R"(%s = float4(dot(aaEdge0, %s), dot(aaEdge1, %s),
                                               dot(aaEdge2, %s), dot(aaEdge3, %s));)",
                                aaDistVarying.vsOut(), textureGP.fPositions.name(),
                                textureGP.fPositions.name(), textureGP.fPositions.name(),
                                textureGP.fPositions.name());
                        mulByFragCoordW = true;
                    } else {
                        args.fVaryingHandler->addVarying("aaDists", &aaDistVarying);
                        args.fVertBuilder->codeAppendf(
                                R"(%s = float4(dot(aaEdge0.xy, %s.xy) + aaEdge0.z,
                                               dot(aaEdge1.xy, %s.xy) + aaEdge1.z,
                                               dot(aaEdge2.xy, %s.xy) + aaEdge2.z,
                                               dot(aaEdge3.xy, %s.xy) + aaEdge3.z);)",
                                aaDistVarying.vsOut(), textureGP.fPositions.name(),
                                textureGP.fPositions.name(), textureGP.fPositions.name(),
                                textureGP.fPositions.name());
                    }
                    args.fFragBuilder->codeAppendf(
                            "float mindist = min(min(%s.x, %s.y), min(%s.z, %s.w));",
                            aaDistVarying.fsIn(), aaDistVarying.fsIn(), aaDistVarying.fsIn(),
                            aaDistVarying.fsIn());
                    if (mulByFragCoordW) {
                        args.fFragBuilder->codeAppend("mindist *= sk_FragCoord.w;");
                    }
                    args.fFragBuilder->codeAppendf("%s = float4(saturate(mindist));",
                                                   args.fOutputCoverage);
                } else {
                    args.fFragBuilder->codeAppendf("%s = float4(1);", args.fOutputCoverage);
                }
            }
            GrGLSLColorSpaceXformHelper fTextureColorSpaceXformHelper;
            GrGLSLColorSpaceXformHelper fPaintColorSpaceXformHelper;
        };
        return new GLSLProcessor;
    }

    bool usesCoverageEdgeAA() const { return SkToBool(fAAEdges[0].isInitialized()); }

private:
    TextureGeometryProcessor(GrTextureType textureType, GrPixelConfig textureConfig,
                             GrSamplerState::Filter filter,
                             sk_sp<GrColorSpaceXform> textureColorSpaceXform,
                             sk_sp<GrColorSpaceXform> paintColorSpaceXform, bool coverageAA,
                             bool perspective, Domain domain, const GrShaderCaps& caps)
            : INHERITED(kTextureGeometryProcessor_ClassID)
            , fTextureColorSpaceXform(std::move(textureColorSpaceXform))
            , fPaintColorSpaceXform(std::move(paintColorSpaceXform))
            , fSampler(textureType, textureConfig, filter) {
        this->setTextureSamplerCnt(1);

        if (perspective) {
            fPositions = {"position", kFloat3_GrVertexAttribType, kFloat3_GrSLType};
        } else {
            fPositions = {"position", kFloat2_GrVertexAttribType, kFloat2_GrSLType};
        }
        fColors = {"color", kUByte4_norm_GrVertexAttribType, kHalf4_GrSLType};
        fTextureCoords = {"textureCoords", kFloat2_GrVertexAttribType, kFloat2_GrSLType};
        int vertexAttributeCnt = 3;

        if (domain == Domain::kYes) {
            fDomain = {"domain", kFloat4_GrVertexAttribType, kFloat4_GrSLType};
            ++vertexAttributeCnt;
        }
        if (coverageAA) {
            fAAEdges[0] = {"aaEdge0", kFloat3_GrVertexAttribType, kFloat3_GrSLType};
            fAAEdges[1] = {"aaEdge1", kFloat3_GrVertexAttribType, kFloat3_GrSLType};
            fAAEdges[2] = {"aaEdge2", kFloat3_GrVertexAttribType, kFloat3_GrSLType};
            fAAEdges[3] = {"aaEdge3", kFloat3_GrVertexAttribType, kFloat3_GrSLType};
            vertexAttributeCnt += 4;
        }
        this->setVertexAttributeCnt(vertexAttributeCnt);
    }

    const Attribute& onVertexAttribute(int i) const override {
        return IthInitializedAttribute(i, fPositions, fColors, fTextureCoords, fDomain, fAAEdges[0],
                                       fAAEdges[1], fAAEdges[2], fAAEdges[3]);
||||||| merged common ancestors
/**
 * Geometry Processor that draws a texture modulated by a vertex color (though, this is meant to be
 * the same value across all vertices of a quad and uses flat interpolation when available). This is
 * used by TextureOp below.
 */
class TextureGeometryProcessor : public GrGeometryProcessor {
public:
    struct Vertex {
        SkPoint fPosition;
        SkPoint fTextureCoords;
        GrColor fColor;
    };
    struct AAVertex {
        SkPoint fPosition;
        SkPoint fTextureCoords;
        SkPoint3 fEdges[4];
        GrColor fColor;
    };
    struct MultiTextureVertex {
        SkPoint fPosition;
        int fTextureIdx;
        SkPoint fTextureCoords;
        GrColor fColor;
    };
    struct AAMultiTextureVertex {
        SkPoint fPosition;
        int fTextureIdx;
        SkPoint fTextureCoords;
        SkPoint3 fEdges[4];
        GrColor fColor;
    };

    // Maximum number of textures supported by this op. Must also be checked against the caps
    // limit. These numbers were based on some limited experiments on a HP Z840 and Pixel XL 2016
    // and could probably use more tuning.
#ifdef SK_BUILD_FOR_ANDROID
    static constexpr int kMaxTextures = 4;
#else
    static constexpr int kMaxTextures = 8;
#endif

    static int SupportsMultitexture(const GrShaderCaps& caps) {
        return caps.integerSupport() && caps.maxFragmentSamplers() > 1;
    }

    static sk_sp<GrGeometryProcessor> Make(sk_sp<GrTextureProxy> proxies[], int proxyCnt,
                                           sk_sp<GrColorSpaceXform> csxf, bool coverageAA,
                                           const GrSamplerState::Filter filters[],
                                           const GrShaderCaps& caps) {
        // We use placement new to avoid always allocating space for kMaxTextures TextureSampler
        // instances.
        int samplerCnt = NumSamplersToUse(proxyCnt, caps);
        size_t size = sizeof(TextureGeometryProcessor) + sizeof(TextureSampler) * (samplerCnt - 1);
        void* mem = GrGeometryProcessor::operator new(size);
        return sk_sp<TextureGeometryProcessor>(new (mem) TextureGeometryProcessor(
                proxies, proxyCnt, samplerCnt, std::move(csxf), coverageAA, filters, caps));
    }

    ~TextureGeometryProcessor() override {
        int cnt = this->numTextureSamplers();
        for (int i = 1; i < cnt; ++i) {
            fSamplers[i].~TextureSampler();
        }
    }

    const char* name() const override { return "TextureGeometryProcessor"; }

    void getGLSLProcessorKey(const GrShaderCaps&, GrProcessorKeyBuilder* b) const override {
        b->add32(GrColorSpaceXform::XformKey(fColorSpaceXform.get()));
        b->add32(static_cast<uint32_t>(this->usesCoverageEdgeAA()));
    }

    GrGLSLPrimitiveProcessor* createGLSLInstance(const GrShaderCaps& caps) const override {
        class GLSLProcessor : public GrGLSLGeometryProcessor {
        public:
            void setData(const GrGLSLProgramDataManager& pdman, const GrPrimitiveProcessor& proc,
                         FPCoordTransformIter&& transformIter) override {
                const auto& textureGP = proc.cast<TextureGeometryProcessor>();
                this->setTransformDataHelper(SkMatrix::I(), pdman, &transformIter);
                if (fColorSpaceXformHelper.isValid()) {
                    fColorSpaceXformHelper.setData(pdman, textureGP.fColorSpaceXform.get());
                }
            }

        private:
            void onEmitCode(EmitArgs& args, GrGPArgs* gpArgs) override {
                using Interpolation = GrGLSLVaryingHandler::Interpolation;
                const auto& textureGP = args.fGP.cast<TextureGeometryProcessor>();
                fColorSpaceXformHelper.emitCode(
                        args.fUniformHandler, textureGP.fColorSpaceXform.get());
                args.fVaryingHandler->setNoPerspective();
                args.fVaryingHandler->emitAttributes(textureGP);
                this->writeOutputPosition(args.fVertBuilder, gpArgs, textureGP.fPositions.fName);
                this->emitTransforms(args.fVertBuilder,
                                     args.fVaryingHandler,
                                     args.fUniformHandler,
                                     textureGP.fTextureCoords.asShaderVar(),
                                     args.fFPCoordTransformHandler);
                args.fVaryingHandler->addPassThroughAttribute(&textureGP.fColors,
                                                              args.fOutputColor,
                                                              Interpolation::kCanBeFlat);
                args.fFragBuilder->codeAppend("float2 texCoord;");
                args.fVaryingHandler->addPassThroughAttribute(&textureGP.fTextureCoords,
                                                              "texCoord");
                if (textureGP.numTextureSamplers() > 1) {
                    // If this changes to float, reconsider Interpolation::kMustBeFlat.
                    SkASSERT(kInt_GrVertexAttribType == textureGP.fTextureIdx.fType);
                    SkASSERT(args.fShaderCaps->integerSupport());
                    args.fFragBuilder->codeAppend("int texIdx;");
                    args.fVaryingHandler->addPassThroughAttribute(&textureGP.fTextureIdx, "texIdx",
                                                                  Interpolation::kMustBeFlat);
                    args.fFragBuilder->codeAppend("switch (texIdx) {");
                    for (int i = 0; i < textureGP.numTextureSamplers(); ++i) {
                        args.fFragBuilder->codeAppendf("case %d: %s = ", i, args.fOutputColor);
                        args.fFragBuilder->appendTextureLookupAndModulate(args.fOutputColor,
                                                                          args.fTexSamplers[i],
                                                                          "texCoord",
                                                                          kFloat2_GrSLType,
                                                                          &fColorSpaceXformHelper);
                        args.fFragBuilder->codeAppend("; break;");
                    }
                    args.fFragBuilder->codeAppend("}");
                } else {
                    args.fFragBuilder->codeAppendf("%s = ", args.fOutputColor);
                    args.fFragBuilder->appendTextureLookupAndModulate(args.fOutputColor,
                                                                      args.fTexSamplers[0],
                                                                      "texCoord",
                                                                      kFloat2_GrSLType,
                                                                      &fColorSpaceXformHelper);
                }
                args.fFragBuilder->codeAppend(";");
                if (textureGP.usesCoverageEdgeAA()) {
                    const char* aaDistName = nullptr;
                    // When interpolation is innacurate we perform the evaluation of the edge
                    // equations in the fragment shader rather than interpolating values computed
                    // in the vertex shader.
                    if (!args.fShaderCaps->interpolantsAreInaccurate()) {
                        GrGLSLVarying aaDistVarying(kFloat4_GrSLType,
                                                    GrGLSLVarying::Scope::kVertToFrag);
                        args.fVaryingHandler->addVarying("aaDists", &aaDistVarying);
                        args.fVertBuilder->codeAppendf(
                                R"(%s = float4(dot(aaEdge0.xy, %s.xy) + aaEdge0.z,
                                               dot(aaEdge1.xy, %s.xy) + aaEdge1.z,
                                               dot(aaEdge2.xy, %s.xy) + aaEdge2.z,
                                               dot(aaEdge3.xy, %s.xy) + aaEdge3.z);)",
                                aaDistVarying.vsOut(), textureGP.fPositions.fName,
                                textureGP.fPositions.fName, textureGP.fPositions.fName,
                                textureGP.fPositions.fName);
                        aaDistName = aaDistVarying.fsIn();
                    } else {
                        GrGLSLVarying aaEdgeVarying[4]{
                                {kFloat3_GrSLType, GrGLSLVarying::Scope::kVertToFrag},
                                {kFloat3_GrSLType, GrGLSLVarying::Scope::kVertToFrag},
                                {kFloat3_GrSLType, GrGLSLVarying::Scope::kVertToFrag},
                                {kFloat3_GrSLType, GrGLSLVarying::Scope::kVertToFrag}
                        };
                        for (int i = 0; i < 4; ++i) {
                            SkString name;
                            name.printf("aaEdge%d", i);
                            args.fVaryingHandler->addVarying(name.c_str(), &aaEdgeVarying[i],
                                                             Interpolation::kCanBeFlat);
                            args.fVertBuilder->codeAppendf(
                                    "%s = aaEdge%d;", aaEdgeVarying[i].vsOut(), i);
                        }
                        args.fFragBuilder->codeAppendf(
                                R"(float4 aaDists = float4(dot(%s.xy, sk_FragCoord.xy) + %s.z,
                                                           dot(%s.xy, sk_FragCoord.xy) + %s.z,
                                                           dot(%s.xy, sk_FragCoord.xy) + %s.z,
                                                           dot(%s.xy, sk_FragCoord.xy) + %s.z);)",
                        aaEdgeVarying[0].fsIn(), aaEdgeVarying[0].fsIn(),
                        aaEdgeVarying[1].fsIn(), aaEdgeVarying[1].fsIn(),
                        aaEdgeVarying[2].fsIn(), aaEdgeVarying[2].fsIn(),
                        aaEdgeVarying[3].fsIn(), aaEdgeVarying[3].fsIn());
                        aaDistName = "aaDists";
                    }
                    args.fFragBuilder->codeAppendf(
                            "float mindist = min(min(%s.x, %s.y), min(%s.z, %s.w));",
                            aaDistName, aaDistName, aaDistName, aaDistName);
                    args.fFragBuilder->codeAppendf("%s = float4(clamp(mindist, 0, 1));",
                                                   args.fOutputCoverage);
                } else {
                    args.fFragBuilder->codeAppendf("%s = float4(1);", args.fOutputCoverage);
                }
            }
            GrGLSLColorSpaceXformHelper fColorSpaceXformHelper;
        };
        return new GLSLProcessor;
    }

    bool usesCoverageEdgeAA() const { return SkToBool(fAAEdges[0].isInitialized()); }

private:
    // This exists to reduce the number of shaders generated. It does some rounding of sampler
    // counts.
    static int NumSamplersToUse(int numRealProxies, const GrShaderCaps& caps) {
        SkASSERT(numRealProxies > 0 && numRealProxies <= kMaxTextures &&
                 numRealProxies <= caps.maxFragmentSamplers());
        if (1 == numRealProxies) {
            return 1;
        }
        if (numRealProxies <= 4) {
            return 4;
        }
        // Round to the next power of 2 and then clamp to kMaxTextures and the max allowed by caps.
        return SkTMin(SkNextPow2(numRealProxies), SkTMin(kMaxTextures, caps.maxFragmentSamplers()));
    }

    TextureGeometryProcessor(sk_sp<GrTextureProxy> proxies[], int proxyCnt, int samplerCnt,
                             sk_sp<GrColorSpaceXform> csxf, bool coverageAA,
                             const GrSamplerState::Filter filters[], const GrShaderCaps& caps)
            : INHERITED(kTextureGeometryProcessor_ClassID), fColorSpaceXform(std::move(csxf)) {
        SkASSERT(proxyCnt > 0 && samplerCnt >= proxyCnt);
        fPositions = this->addVertexAttrib("position", kFloat2_GrVertexAttribType);
        fSamplers[0].reset(std::move(proxies[0]), filters[0]);
        this->addTextureSampler(&fSamplers[0]);
        for (int i = 1; i < proxyCnt; ++i) {
            // This class has one sampler built in, the rest come from memory this processor was
            // placement-newed into and so haven't been constructed.
            new (&fSamplers[i]) TextureSampler(std::move(proxies[i]), filters[i]);
            this->addTextureSampler(&fSamplers[i]);
        }
        if (samplerCnt > 1) {
            // Here we initialize any extra samplers by repeating the last one samplerCnt - proxyCnt
            // times.
            GrTextureProxy* dupeProxy = fSamplers[proxyCnt - 1].proxy();
            for (int i = proxyCnt; i < samplerCnt; ++i) {
                new (&fSamplers[i]) TextureSampler(sk_ref_sp(dupeProxy), filters[proxyCnt - 1]);
                this->addTextureSampler(&fSamplers[i]);
            }
            SkASSERT(caps.integerSupport());
            fTextureIdx = this->addVertexAttrib("textureIdx", kInt_GrVertexAttribType);
        }

        fTextureCoords = this->addVertexAttrib("textureCoords", kFloat2_GrVertexAttribType);
        if (coverageAA) {
            fAAEdges[0] = this->addVertexAttrib("aaEdge0", kFloat3_GrVertexAttribType);
            fAAEdges[1] = this->addVertexAttrib("aaEdge1", kFloat3_GrVertexAttribType);
            fAAEdges[2] = this->addVertexAttrib("aaEdge2", kFloat3_GrVertexAttribType);
            fAAEdges[3] = this->addVertexAttrib("aaEdge3", kFloat3_GrVertexAttribType);
        }
        fColors = this->addVertexAttrib("color", kUByte4_norm_GrVertexAttribType);
=======
using Domain = GrQuadPerEdgeAA::Domain;
using VertexSpec = GrQuadPerEdgeAA::VertexSpec;
using ColorType = GrQuadPerEdgeAA::ColorType;

// if normalizing the domain then pass 1/width, 1/height, 1 for iw, ih, h. Otherwise pass
// 1, 1, and height.
static SkRect compute_domain(Domain domain, GrSamplerState::Filter filter, GrSurfaceOrigin origin,
                             const SkRect& srcRect, float iw, float ih, float h) {
    static constexpr SkRect kLargeRect = {-100000, -100000, 1000000, 1000000};
    if (domain == Domain::kNo) {
        // Either the quad has no domain constraint and is batched with a domain constrained op
        // (in which case we want a domain that doesn't restrict normalized tex coords), or the
        // entire op doesn't use the domain, in which case the returned value is ignored.
        return kLargeRect;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    const TextureSampler& onTextureSampler(int) const override { return fSampler; }

    Attribute fPositions;
    Attribute fColors;
    Attribute fTextureCoords;
    Attribute fDomain;
    Attribute fAAEdges[4];
    sk_sp<GrColorSpaceXform> fTextureColorSpaceXform;
    sk_sp<GrColorSpaceXform> fPaintColorSpaceXform;
    TextureSampler fSampler;

    typedef GrGeometryProcessor INHERITED;
};

// This computes the four edge equations for a quad, then outsets them and optionally computes a new
// quad as the intersection points of the outset edges. 'x' and 'y' contain the original points as
// input and the outset points as output. 'a', 'b', and 'c' are the edge equation coefficients on
// output.
static void compute_quad_edges_and_outset_vertices(GrQuadAAFlags aaFlags, Sk4f* x, Sk4f* y, Sk4f* a,
                                                   Sk4f* b, Sk4f* c, bool outsetCorners) {
    SkASSERT(GrQuadAAFlags::kNone != aaFlags);

    static constexpr auto fma = SkNx_fma<4, float>;
    // These rotate the points/edge values either clockwise or counterclockwise assuming tri strip
    // order.
    auto nextCW  = [](const Sk4f& v) { return SkNx_shuffle<2, 0, 3, 1>(v); };
    auto nextCCW = [](const Sk4f& v) { return SkNx_shuffle<1, 3, 0, 2>(v); };

    // Compute edge equations for the quad.
    auto xnext = nextCCW(*x);
    auto ynext = nextCCW(*y);
    // xdiff and ydiff will comprise the normalized vectors pointing along each quad edge.
    auto xdiff = xnext - *x;
    auto ydiff = ynext - *y;
    auto invLengths = fma(xdiff, xdiff, ydiff * ydiff).rsqrt();
    xdiff *= invLengths;
    ydiff *= invLengths;

    // Use above vectors to compute edge equations.
    *c = fma(xnext, *y,  -ynext * *x) * invLengths;
    // Make sure the edge equations have their normals facing into the quad in device space.
    auto test = fma(ydiff, nextCW(*x), fma(-xdiff, nextCW(*y), *c));
    if ((test < Sk4f(0)).anyTrue()) {
        *a = -ydiff;
        *b = xdiff;
        *c = -*c;
    } else {
        *a = ydiff;
        *b = -xdiff;
    }
    // Outset the edge equations so aa coverage evaluates to zero half a pixel away from the
    // original quad edge.
    *c += 0.5f;

    if (aaFlags != GrQuadAAFlags::kAll) {
        // This order is the same order the edges appear in xdiff/ydiff and therefore as the
        // edges in a/b/c.
        auto mask = Sk4f(GrQuadAAFlags::kLeft & aaFlags ? 1.f : 0.f,
                         GrQuadAAFlags::kBottom & aaFlags ? 1.f : 0.f,
                         GrQuadAAFlags::kTop & aaFlags ? 1.f : 0.f,
                         GrQuadAAFlags::kRight & aaFlags ? 1.f : 0.f);
        // Outset edge equations for masked out edges another pixel so that they always evaluate
        // >= 1.
        *c += (1.f - mask);
        if (outsetCorners) {
            // Do the vertex outset.
            mask *= 0.5f;
            auto maskCW = nextCW(mask);
            *x += maskCW * -xdiff + mask * nextCW(xdiff);
            *y += maskCW * -ydiff + mask * nextCW(ydiff);
        }
    } else if (outsetCorners) {
        *x += 0.5f * (-xdiff + nextCW(xdiff));
        *y += 0.5f * (-ydiff + nextCW(ydiff));
    }
}

namespace {
// This is a class soley so it can be partially specialized (functions cannot be).
template <typename V, GrAA AA = V::kAA, typename Position = typename V::Position>
class VertexAAHandler;

template<typename V> class VertexAAHandler<V, GrAA::kNo, SkPoint> {
public:
    static void AssignPositionsAndTexCoords(V* vertices, const GrPerspQuad& quad,
                                            GrQuadAAFlags aaFlags, const SkRect& texRect) {
        // Should be kNone for non-AA and kAll for MSAA.
        SkASSERT(aaFlags == GrQuadAAFlags::kNone || aaFlags == GrQuadAAFlags::kAll);
        SkASSERT((quad.w4f() == Sk4f(1.f)).allTrue());
        SkPointPriv::SetRectTriStrip(&vertices[0].fTextureCoords, texRect, sizeof(V));
        for (int i = 0; i < 4; ++i) {
            vertices[i].fPosition = {quad.x(i), quad.y(i)};
        }
||||||| merged common ancestors
    Attribute fPositions;
    Attribute fTextureIdx;
    Attribute fTextureCoords;
    Attribute fColors;
    Attribute fAAEdges[4];
    sk_sp<GrColorSpaceXform> fColorSpaceXform;
    TextureSampler fSamplers[1];

    typedef GrGeometryProcessor INHERITED;
};

namespace {
// This is a class soley so it can be partially specialized (functions cannot be).
template<GrAA, typename Vertex> class VertexAAHandler;

template<typename Vertex> class VertexAAHandler<GrAA::kNo, Vertex> {
public:
    static void AssignPositionsAndTexCoords(Vertex* vertices, const GrQuad& quad,
                                            const SkRect& texRect) {
        vertices[0].fPosition = quad.point(0);
        vertices[0].fTextureCoords = {texRect.fLeft, texRect.fTop};
        vertices[1].fPosition = quad.point(1);
        vertices[1].fTextureCoords = {texRect.fLeft, texRect.fBottom};
        vertices[2].fPosition = quad.point(2);
        vertices[2].fTextureCoords = {texRect.fRight, texRect.fTop};
        vertices[3].fPosition = quad.point(3);
        vertices[3].fTextureCoords = {texRect.fRight, texRect.fBottom};
=======
    auto ltrb = Sk4f::Load(&srcRect);
    if (filter == GrSamplerState::Filter::kBilerp) {
        auto rblt = SkNx_shuffle<2, 3, 0, 1>(ltrb);
        auto whwh = (rblt - ltrb).abs();
        auto c = (rblt + ltrb) * 0.5f;
        static const Sk4f kOffsets = {0.5f, 0.5f, -0.5f, -0.5f};
        ltrb = (whwh < 1.f).thenElse(c, ltrb + kOffsets);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
};

template<typename V> class VertexAAHandler<V, GrAA::kNo, SkPoint3> {
public:
    static void AssignPositionsAndTexCoords(V* vertices, const GrPerspQuad& quad,
                                            GrQuadAAFlags aaFlags, const SkRect& texRect) {
        // Should be kNone for non-AA and kAll for MSAA.
        SkASSERT(aaFlags == GrQuadAAFlags::kNone || aaFlags == GrQuadAAFlags::kAll);
        SkPointPriv::SetRectTriStrip(&vertices[0].fTextureCoords, texRect, sizeof(V));
        for (int i = 0; i < 4; ++i) {
            vertices[i].fPosition = quad.point(i);
        }
    }
};

template<typename V> class VertexAAHandler<V, GrAA::kYes, SkPoint> {
public:
    static void AssignPositionsAndTexCoords(V* vertices, const GrPerspQuad& quad,
                                            GrQuadAAFlags aaFlags, const SkRect& texRect) {
        SkASSERT((quad.w4f() == Sk4f(1.f)).allTrue());
        auto x = quad.x4f();
        auto y = quad.y4f();
        Sk4f a, b, c;
        compute_quad_edges_and_outset_vertices(aaFlags, &x, &y, &a, &b, &c, true);

        for (int i = 0; i < 4; ++i) {
            vertices[i].fPosition = {x[i], y[i]};
            for (int j = 0; j < 4; ++j) {
                vertices[i].fEdges[j]  = {a[j], b[j], c[j]};
            }
        }

        AssignTexCoords(vertices, quad, texRect);
    }

private:
    static void AssignTexCoords(V* vertices, const GrPerspQuad& quad, const SkRect& tex) {
        SkMatrix q = SkMatrix::MakeAll(quad.x(0), quad.x(1), quad.x(2),
                                       quad.y(0), quad.y(1), quad.y(2),
                                             1.f,       1.f,       1.f);
        SkMatrix qinv;
        if (!q.invert(&qinv)) {
            return;
        }
        SkMatrix t = SkMatrix::MakeAll(tex.fLeft,    tex.fLeft, tex.fRight,
                                        tex.fTop,  tex.fBottom,   tex.fTop,
                                             1.f,          1.f,        1.f);
        SkMatrix map;
        map.setConcat(t, qinv);
        SkMatrixPriv::MapPointsWithStride(map, &vertices[0].fTextureCoords, sizeof(V),
                                          &vertices[0].fPosition, sizeof(V), 4);
    }
};

template<typename V> class VertexAAHandler<V, GrAA::kYes, SkPoint3> {
public:
    static void AssignPositionsAndTexCoords(V* vertices, const GrPerspQuad& quad,
                                            GrQuadAAFlags aaFlags, const SkRect& texRect) {
        auto x = quad.x4f();
        auto y = quad.y4f();
        auto iw = quad.iw4f();

        Sk4f a, b, c;
        auto x2d = x * iw;
        auto y2d = y * iw;
        compute_quad_edges_and_outset_vertices(aaFlags, &x2d, &y2d, &a, &b, &c, false);
        auto w = quad.w4f();
        static const float kOutset = 0.5f;
        if ((GrQuadAAFlags::kLeft | GrQuadAAFlags::kRight) & aaFlags) {
            // For each entry in x the equivalent entry in opX is the left/right opposite and so on.
            Sk4f opX = SkNx_shuffle<2, 3, 0, 1>(x);
            Sk4f opW = SkNx_shuffle<2, 3, 0, 1>(w);
            Sk4f opY = SkNx_shuffle<2, 3, 0, 1>(y);
            // vx/vy holds the device space left-to-right vectors along top and bottom of the quad.
            Sk2f vx = SkNx_shuffle<2, 3>(x2d) - SkNx_shuffle<0, 1>(x2d);
            Sk2f vy = SkNx_shuffle<2, 3>(y2d) - SkNx_shuffle<0, 1>(y2d);
            Sk2f len = SkNx_fma(vx, vx, vy * vy).sqrt();
            // For each device space corner, devP, label its left/right opposite device space point
            // opDevPt. The new device space point is opDevPt + s (devPt - opDevPt) where s is
            // (length(devPt - opDevPt) + 0.5) / length(devPt - opDevPt);
            Sk4f s = SkNx_shuffle<0, 1, 0, 1>((len + kOutset) / len);
            // Compute t in homogeneous space from s using similar triangles so that we can produce
            // homogeneous outset vertices for perspective-correct interpolation.
            Sk4f sOpW = s * opW;
            Sk4f t = sOpW / (sOpW + (1.f - s) * w);
            // mask is used to make the t values be 1 when the left/right side is not antialiased.
            Sk4f mask(GrQuadAAFlags::kLeft & aaFlags  ? 1.f : 0.f,
                      GrQuadAAFlags::kLeft & aaFlags  ? 1.f : 0.f,
                      GrQuadAAFlags::kRight & aaFlags ? 1.f : 0.f,
                      GrQuadAAFlags::kRight & aaFlags ? 1.f : 0.f);
            t = t * mask + (1.f - mask);
            x = opX + t * (x - opX);
            y = opY + t * (y - opY);
            w = opW + t * (w - opW);
            if ((GrQuadAAFlags::kTop | GrQuadAAFlags::kBottom) & aaFlags) {
                // Update the 2D points for the top/bottom calculation.
                iw = w.invert();
                x2d = x * iw;
                y2d = y * iw;
            }
        }

        if ((GrQuadAAFlags::kTop | GrQuadAAFlags::kBottom) & aaFlags) {
            // This operates the same as above but for top/bottom rather than left/right.
            Sk4f opX = SkNx_shuffle<1, 0, 3, 2>(x);
            Sk4f opW = SkNx_shuffle<1, 0, 3, 2>(w);
            Sk4f opY = SkNx_shuffle<1, 0, 3, 2>(y);

            Sk2f vx = SkNx_shuffle<1, 3>(x2d) - SkNx_shuffle<0, 2>(x2d);
            Sk2f vy = SkNx_shuffle<1, 3>(y2d) - SkNx_shuffle<0, 2>(y2d);
            Sk2f len = SkNx_fma(vx, vx, vy * vy).sqrt();

            Sk4f s = SkNx_shuffle<0, 0, 1, 1>((len + kOutset) / len);

            Sk4f sOpW = s * opW;
            Sk4f t = sOpW / (sOpW + (1.f - s) * w);

            Sk4f mask(GrQuadAAFlags::kTop    & aaFlags ? 1.f : 0.f,
                      GrQuadAAFlags::kBottom & aaFlags ? 1.f : 0.f,
                      GrQuadAAFlags::kTop    & aaFlags ? 1.f : 0.f,
                      GrQuadAAFlags::kBottom & aaFlags ? 1.f : 0.f);
            t = t * mask + (1.f - mask);
            x = opX + t * (x - opX);
            y = opY + t * (y - opY);
            w = opW + t * (w - opW);
        }
        for (int i = 0; i < 4; ++i) {
            vertices[i].fPosition = {x[i], y[i], w[i]};
            for (int j = 0; j < 4; ++j) {
                vertices[i].fEdges[j] = {a[j], b[j], c[j]};
            }
        }

        AssignTexCoords(vertices, quad, texRect);
    }

private:
    static void AssignTexCoords(V* vertices, const GrPerspQuad& quad, const SkRect& tex) {
        SkMatrix q = SkMatrix::MakeAll(quad.x(0), quad.x(1), quad.x(2),
                                       quad.y(0), quad.y(1), quad.y(2),
                                       quad.w(0), quad.w(1), quad.w(2));
        SkMatrix qinv;
        if (!q.invert(&qinv)) {
            return;
        }
        SkMatrix t = SkMatrix::MakeAll(tex.fLeft, tex.fLeft,   tex.fRight,
                                       tex.fTop,  tex.fBottom, tex.fTop,
                                       1.f,       1.f,         1.f);
        SkMatrix map;
        map.setConcat(t, qinv);
        SkPoint3 tempTexCoords[4];
        SkMatrixPriv::MapHomogeneousPointsWithStride(map, tempTexCoords, sizeof(SkPoint3),
                                                     &vertices[0].fPosition, sizeof(V), 4);
        for (int i = 0; i < 4; ++i) {
            auto invW = 1.f / tempTexCoords[i].fZ;
            vertices[i].fTextureCoords.fX = tempTexCoords[i].fX * invW;
            vertices[i].fTextureCoords.fY = tempTexCoords[i].fY * invW;
        }
    }
};

template <typename V, Domain D = V::kDomain> struct DomainAssigner;

template <typename V> struct DomainAssigner<V, Domain::kYes> {
    static void Assign(V* vertices, Domain domain, GrSamplerState::Filter filter,
                       const SkRect& srcRect, GrSurfaceOrigin origin, float iw, float ih) {
        static constexpr SkRect kLargeRect = {-2, -2, 2, 2};
        SkRect domainRect;
        if (domain == Domain::kYes) {
            auto ltrb = Sk4f::Load(&srcRect);
            if (filter == GrSamplerState::Filter::kBilerp) {
                auto rblt = SkNx_shuffle<2, 3, 0, 1>(ltrb);
                auto whwh = (rblt - ltrb).abs();
                auto c = (rblt + ltrb) * 0.5f;
                static const Sk4f kOffsets = {0.5f, 0.5f, -0.5f, -0.5f};
                ltrb = (whwh < 1.f).thenElse(c, ltrb + kOffsets);
            }
            ltrb *= Sk4f(iw, ih, iw, ih);
            if (origin == kBottomLeft_GrSurfaceOrigin) {
                static const Sk4f kMul = {1.f, -1.f, 1.f, -1.f};
                static const Sk4f kAdd = {0.f, 1.f, 0.f, 1.f};
                ltrb = SkNx_shuffle<0, 3, 2, 1>(kMul * ltrb + kAdd);
            }
            ltrb.store(&domainRect);
        } else {
            domainRect = kLargeRect;
        }
        for (int i = 0; i < 4; ++i) {
            vertices[i].fTextureDomain = domainRect;
        }
||||||| merged common ancestors
};

template<typename Vertex> class VertexAAHandler<GrAA::kYes, Vertex> {
public:
    static void AssignPositionsAndTexCoords(Vertex* vertices, const GrQuad& quad,
                                            const SkRect& texRect) {
        // We compute the four edge equations for quad, then outset them and compute a new quad
        // as the intersection points of the outset edges.

        // GrQuad is in tristip order but we want the points to be in a fan order, so swap 2 and 3.
        Sk4f xs(quad.point(0).fX, quad.point(1).fX, quad.point(3).fX, quad.point(2).fX);
        Sk4f ys(quad.point(0).fY, quad.point(1).fY, quad.point(3).fY, quad.point(2).fY);
        Sk4f xsrot = SkNx_shuffle<1, 2, 3, 0>(xs);
        Sk4f ysrot = SkNx_shuffle<1, 2, 3, 0>(ys);
        Sk4f normXs = ysrot - ys;
        Sk4f normYs = xs - xsrot;
        Sk4f ds = xsrot * ys - ysrot * xs;
        Sk4f invNormLengths = (normXs * normXs + normYs * normYs).rsqrt();
        float test = normXs[0] * xs[2] + normYs[0] * ys[2] + ds[0];
        // Make sure the edge equations have their normals facing into the quad in device space
        if (test < 0) {
            invNormLengths = -invNormLengths;
        }
        normXs *= invNormLengths;
        normYs *= invNormLengths;
        ds *= invNormLengths;

        // Here is the bloat. This makes our edge equations compute coverage without requiring a
        // half pixel offset and is also used to compute the bloated quad that will cover all
        // pixels.
        ds += Sk4f(0.5f);

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                vertices[j].fEdges[i].fX = normXs[i];
                vertices[j].fEdges[i].fY = normYs[i];
                vertices[j].fEdges[i].fZ = ds[i];
            }
        }

        // Reverse the process to compute the points of the bloated quad from the edge equations.
        // This time the inputs don't have 1s as their third coord and we want to homogenize rather
        // than normalize the output since we need a GrQuad with 2D points.
        xsrot = SkNx_shuffle<3, 0, 1, 2>(normXs);
        ysrot = SkNx_shuffle<3, 0, 1, 2>(normYs);
        Sk4f dsrot = SkNx_shuffle<3, 0, 1, 2>(ds);
        xs = ysrot * ds - normYs * dsrot;
        ys = normXs * dsrot - xsrot * ds;
        ds = xsrot * normYs - ysrot * normXs;
        ds = ds.invert();
        xs *= ds;
        ys *= ds;

        // Go back to tri strip order when writing out the bloated quad to vertex positions.
        vertices[0].fPosition = {xs[0], ys[0]};
        vertices[1].fPosition = {xs[1], ys[1]};
        vertices[3].fPosition = {xs[2], ys[2]};
        vertices[2].fPosition = {xs[3], ys[3]};

        AssignTexCoords(vertices, quad, texRect);
    }

private:
    static void AssignTexCoords(Vertex* vertices, const GrQuad& quad, const SkRect& tex) {
        SkMatrix q = SkMatrix::MakeAll(quad.point(0).fX, quad.point(1).fX, quad.point(2).fX,
                                       quad.point(0).fY, quad.point(1).fY, quad.point(2).fY,
                                                    1.f,              1.f,              1.f);
        SkMatrix qinv;
        if (!q.invert(&qinv)) {
            return;
        }
        SkMatrix t = SkMatrix::MakeAll(tex.fLeft,    tex.fLeft, tex.fRight,
                                        tex.fTop,  tex.fBottom,   tex.fTop,
                                             1.f,          1.f,        1.f);
        SkMatrix map;
        map.setConcat(t, qinv);
        SkMatrixPriv::MapPointsWithStride(map, &vertices[0].fTextureCoords, sizeof(Vertex),
                                          &vertices[0].fPosition, sizeof(Vertex), 4);
    }
};

template <typename Vertex, bool IsMultiTex> struct TexIdAssigner;

template <typename Vertex> struct TexIdAssigner<Vertex, true> {
    static void Assign(Vertex* vertices, int textureIdx) {
        vertices[0].fTextureIdx = textureIdx;
        vertices[1].fTextureIdx = textureIdx;
        vertices[2].fTextureIdx = textureIdx;
        vertices[3].fTextureIdx = textureIdx;
=======
    ltrb *= Sk4f(iw, ih, iw, ih);
    if (origin == kBottomLeft_GrSurfaceOrigin) {
        static const Sk4f kMul = {1.f, -1.f, 1.f, -1.f};
        const Sk4f kAdd = {0.f, h, 0.f, h};
        ltrb = SkNx_shuffle<0, 3, 2, 1>(kMul * ltrb + kAdd);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
template <typename V> struct DomainAssigner<V, Domain::kNo> {
    static void Assign(V*, Domain domain, GrSamplerState::Filter, const SkRect&, GrSurfaceOrigin,
                       float iw, float ih) {
        SkASSERT(domain == Domain::kNo);
    }
};

}  // anonymous namespace
||||||| merged common ancestors
template <typename Vertex> struct TexIdAssigner<Vertex, false> {
    static void Assign(Vertex* vertices, int textureIdx) {}
};
}  // anonymous namespace
=======
    SkRect domainRect;
    ltrb.store(&domainRect);
    return domainRect;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename V>
static void tessellate_quad(const GrPerspQuad& devQuad, GrQuadAAFlags aaFlags,
                            const SkRect& srcRect, GrColor color, GrSurfaceOrigin origin,
                            GrSamplerState::Filter filter, V* vertices, SkScalar iw, SkScalar ih,
                            Domain domain) {
||||||| merged common ancestors
template <typename Vertex, bool IsMultiTex, GrAA AA>
static void tessellate_quad(const GrQuad& devQuad, const SkRect& srcRect, GrColor color,
                            GrSurfaceOrigin origin, Vertex* vertices, SkScalar iw, SkScalar ih,
                            int textureIdx) {
=======
// If normalizing the src quad then pass 1/width, 1/height, 1 for iw, ih, h. Otherwise pass
// 1, 1, and height.
static GrPerspQuad compute_src_quad_from_rect(GrSurfaceOrigin origin, const SkRect& srcRect,
                                              float iw, float ih, float h) {
    // Convert the pixel-space src rectangle into normalized texture coordinates
>>>>>>> upstream-releases
    SkRect texRect = {
        iw * srcRect.fLeft,
        ih * srcRect.fTop,
        iw * srcRect.fRight,
        ih * srcRect.fBottom
    };
    if (origin == kBottomLeft_GrSurfaceOrigin) {
        texRect.fTop = h - texRect.fTop;
        texRect.fBottom = h - texRect.fBottom;
    }
<<<<<<< HEAD
    VertexAAHandler<V>::AssignPositionsAndTexCoords(vertices, devQuad, aaFlags, texRect);
    vertices[0].fColor = color;
    vertices[1].fColor = color;
    vertices[2].fColor = color;
    vertices[3].fColor = color;
    DomainAssigner<V>::Assign(vertices, domain, filter, srcRect, origin, iw, ih);
||||||| merged common ancestors
    VertexAAHandler<AA, Vertex>::AssignPositionsAndTexCoords(vertices, devQuad, texRect);
    vertices[0].fColor = color;
    vertices[1].fColor = color;
    vertices[2].fColor = color;
    vertices[3].fColor = color;
    TexIdAssigner<Vertex, IsMultiTex>::Assign(vertices, textureIdx);
=======
    return GrPerspQuad(texRect);
>>>>>>> upstream-releases
}
<<<<<<< HEAD

||||||| merged common ancestors
=======
// Normalizes logical src coords and corrects for origin
static GrPerspQuad compute_src_quad(GrSurfaceOrigin origin, const GrPerspQuad& srcQuad,
                                    float iw, float ih, float h) {
    // The src quad should not have any perspective
    SkASSERT(!srcQuad.hasPerspective());
    Sk4f xs = srcQuad.x4f() * iw;
    Sk4f ys = srcQuad.y4f() * ih;
    if (origin == kBottomLeft_GrSurfaceOrigin) {
        ys = h - ys;
    }
    return GrPerspQuad(xs, ys);
}

>>>>>>> upstream-releases
/**
 * Op that implements GrTextureOp::Make. It draws textured quads. Each quad can modulate against a
 * the texture by color. The blend with the destination is always src-over. The edges are non-AA.
 */
class TextureOp final : public GrMeshDrawOp {
public:
<<<<<<< HEAD
    static std::unique_ptr<GrDrawOp> Make(GrContext* context,
                                          sk_sp<GrTextureProxy> proxy,
                                          GrSamplerState::Filter filter,
                                          GrColor color,
                                          const SkRect& srcRect,
                                          const SkRect& dstRect,
                                          GrAAType aaType,
                                          GrQuadAAFlags aaFlags,
                                          SkCanvas::SrcRectConstraint constraint,
                                          const SkMatrix& viewMatrix,
                                          sk_sp<GrColorSpaceXform> textureColorSpaceXform,
                                          sk_sp<GrColorSpaceXform> paintColorSpaceXform) {
        GrOpMemoryPool* pool = context->contextPriv().opMemoryPool();

        return pool->allocate<TextureOp>(
                std::move(proxy), filter, color, srcRect, dstRect, aaType, aaFlags, constraint,
                viewMatrix, std::move(textureColorSpaceXform), std::move(paintColorSpaceXform));
||||||| merged common ancestors
    static std::unique_ptr<GrDrawOp> Make(sk_sp<GrTextureProxy> proxy,
                                          GrSamplerState::Filter filter, GrColor color,
                                          const SkRect& srcRect, const SkRect& dstRect,
                                          GrAAType aaType, const SkMatrix& viewMatrix,
                                          sk_sp<GrColorSpaceXform> csxf, bool allowSRBInputs) {
        return std::unique_ptr<GrDrawOp>(new TextureOp(std::move(proxy), filter, color, srcRect,
                                                       dstRect, aaType, viewMatrix, std::move(csxf),
                                                       allowSRBInputs));
=======
    static std::unique_ptr<GrDrawOp> Make(GrRecordingContext* context,
                                          sk_sp<GrTextureProxy> proxy,
                                          GrSamplerState::Filter filter,
                                          const SkPMColor4f& color,
                                          const SkRect& srcRect,
                                          const SkRect& dstRect,
                                          GrAAType aaType,
                                          GrQuadAAFlags aaFlags,
                                          SkCanvas::SrcRectConstraint constraint,
                                          const SkMatrix& viewMatrix,
                                          sk_sp<GrColorSpaceXform> textureColorSpaceXform) {
        GrPerspQuad dstQuad = GrPerspQuad::MakeFromRect(dstRect, viewMatrix);
        GrQuadType dstQuadType = GrQuadTypeForTransformedRect(viewMatrix);

        if (dstQuadType == GrQuadType::kRect) {
            // Disable filtering if possible (note AA optimizations for rects are automatically
            // handled above in GrResolveAATypeForQuad).
            if (filter != GrSamplerState::Filter::kNearest &&
                !GrTextureOp::GetFilterHasEffect(viewMatrix, srcRect, dstRect)) {
                filter = GrSamplerState::Filter::kNearest;
            }
        }

        GrOpMemoryPool* pool = context->priv().opMemoryPool();
        // srcRect provides both local coords and domain (if needed), so use nullptr for srcQuad
        return pool->allocate<TextureOp>(
                std::move(proxy), filter, color, dstQuad, dstQuadType, srcRect, constraint,
                nullptr, GrQuadType::kRect, aaType, aaFlags, std::move(textureColorSpaceXform));
    }
    static std::unique_ptr<GrDrawOp> Make(GrRecordingContext* context,
                                          sk_sp<GrTextureProxy> proxy,
                                          GrSamplerState::Filter filter,
                                          const SkPMColor4f& color,
                                          const SkPoint srcQuad[4],
                                          const SkPoint dstQuad[4],
                                          GrAAType aaType,
                                          GrQuadAAFlags aaFlags,
                                          const SkRect* domain,
                                          const SkMatrix& viewMatrix,
                                          sk_sp<GrColorSpaceXform> textureColorSpaceXform) {
        GrPerspQuad grDstQuad = GrPerspQuad::MakeFromSkQuad(dstQuad, viewMatrix);
        GrQuadType dstQuadType = viewMatrix.hasPerspective() ? GrQuadType::kPerspective
                                                             : GrQuadType::kStandard;
        GrPerspQuad grSrcQuad = GrPerspQuad::MakeFromSkQuad(srcQuad, SkMatrix::I());

        // If constraint remains fast, the value in srcRect will be ignored since srcQuads provides
        // the local coordinates and a domain won't be used.
        SkRect srcRect = SkRect::MakeEmpty();
        SkCanvas::SrcRectConstraint constraint = SkCanvas::kFast_SrcRectConstraint;
        if (domain) {
            srcRect = *domain;
            constraint = SkCanvas::kStrict_SrcRectConstraint;
        }

        GrOpMemoryPool* pool = context->priv().opMemoryPool();
        // Pass domain as srcRect if provided, but send srcQuad as a GrPerspQuad for local coords
        return pool->allocate<TextureOp>(
                std::move(proxy), filter, color, grDstQuad, dstQuadType, srcRect, constraint,
                &grSrcQuad, GrQuadType::kStandard, aaType, aaFlags,
                std::move(textureColorSpaceXform));
    }
    static std::unique_ptr<GrDrawOp> Make(GrRecordingContext* context,
                                          const GrRenderTargetContext::TextureSetEntry set[],
                                          int cnt, GrSamplerState::Filter filter, GrAAType aaType,
                                          const SkMatrix& viewMatrix,
                                          sk_sp<GrColorSpaceXform> textureColorSpaceXform) {
        size_t size = sizeof(TextureOp) + sizeof(Proxy) * (cnt - 1);
        GrOpMemoryPool* pool = context->priv().opMemoryPool();
        void* mem = pool->allocate(size);
        return std::unique_ptr<GrDrawOp>(new (mem) TextureOp(set, cnt, filter, aaType, viewMatrix,
                                                             std::move(textureColorSpaceXform)));
>>>>>>> upstream-releases
    }

    ~TextureOp() override {
<<<<<<< HEAD
        if (fFinalized) {
            fProxy->completedRead();
        } else {
            fProxy->unref();
||||||| merged common ancestors
        if (fFinalized) {
            auto proxies = this->proxies();
            for (int i = 0; i < fProxyCnt; ++i) {
                proxies[i]->completedRead();
            }
            if (fProxyCnt > 1) {
                delete[] reinterpret_cast<const char*>(proxies);
            }
        } else {
            SkASSERT(1 == fProxyCnt);
            fProxy0->unref();
=======
        for (unsigned p = 0; p < fProxyCnt; ++p) {
            if (fFinalized) {
                fProxies[p].fProxy->completedRead();
            } else {
                fProxies[p].fProxy->unref();
            }
>>>>>>> upstream-releases
        }
    }

    const char* name() const override { return "TextureOp"; }

<<<<<<< HEAD
    void visitProxies(const VisitProxyFunc& func) const override { func(fProxy); }
||||||| merged common ancestors
    void visitProxies(const VisitProxyFunc& func) const override {
        auto proxies = this->proxies();
        for (int i = 0; i < fProxyCnt; ++i) {
            func(proxies[i]);
        }
    }
=======
    void visitProxies(const VisitProxyFunc& func, VisitorType visitor) const override {
        if (visitor == VisitorType::kAllocatorGather && fCanSkipAllocatorGather) {
            return;
        }
        for (unsigned p = 0; p < fProxyCnt; ++p) {
            func(fProxies[p].fProxy);
        }
    }
>>>>>>> upstream-releases

#ifdef SK_DEBUG
    SkString dumpInfo() const override {
        SkString str;
<<<<<<< HEAD
        str.appendf("# draws: %d\n", fDraws.count());
        str.appendf("Proxy ID: %d, Filter: %d\n", fProxy->uniqueID().asUInt(),
                    static_cast<int>(fFilter));
        for (int i = 0; i < fDraws.count(); ++i) {
            const Draw& draw = fDraws[i];
            str.appendf(
                    "%d: Color: 0x%08x, TexRect [L: %.2f, T: %.2f, R: %.2f, B: %.2f] "
                    "Quad [(%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)]\n",
                    i, draw.color(), draw.srcRect().fLeft, draw.srcRect().fTop,
                    draw.srcRect().fRight, draw.srcRect().fBottom, draw.quad().point(0).fX,
                    draw.quad().point(0).fY, draw.quad().point(1).fX, draw.quad().point(1).fY,
                    draw.quad().point(2).fX, draw.quad().point(2).fY, draw.quad().point(3).fX,
                    draw.quad().point(3).fY);
||||||| merged common ancestors
        str.appendf("AllowSRGBInputs: %d\n", fAllowSRGBInputs);
        str.appendf("# draws: %d\n", fDraws.count());
        auto proxies = this->proxies();
        for (int i = 0; i < fProxyCnt; ++i) {
            str.appendf("Proxy ID %d: %d, Filter: %d\n", i, proxies[i]->uniqueID().asUInt(),
                        static_cast<int>(this->filters()[i]));
        }
        for (int i = 0; i < fDraws.count(); ++i) {
            const Draw& draw = fDraws[i];
            str.appendf(
                    "%d: Color: 0x%08x, ProxyIdx: %d, TexRect [L: %.2f, T: %.2f, R: %.2f, B: %.2f] "
                    "Quad [(%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)]\n",
                    i, draw.fColor, draw.fTextureIdx, draw.fSrcRect.fLeft, draw.fSrcRect.fTop,
                    draw.fSrcRect.fRight, draw.fSrcRect.fBottom, draw.fQuad.points()[0].fX,
                    draw.fQuad.points()[0].fY, draw.fQuad.points()[1].fX, draw.fQuad.points()[1].fY,
                    draw.fQuad.points()[2].fX, draw.fQuad.points()[2].fY, draw.fQuad.points()[3].fX,
                    draw.fQuad.points()[3].fY);
=======
        str.appendf("# draws: %d\n", fQuads.count());
        int q = 0;
        for (unsigned p = 0; p < fProxyCnt; ++p) {
            str.appendf("Proxy ID: %d, Filter: %d\n", fProxies[p].fProxy->uniqueID().asUInt(),
                        static_cast<int>(fFilter));
            for (int i = 0; i < fProxies[p].fQuadCnt; ++i, ++q) {
                GrPerspQuad quad = fQuads[q];
                const ColorDomainAndAA& info = fQuads.metadata(i);
                str.appendf(
                        "%d: Color: 0x%08x, TexRect [L: %.2f, T: %.2f, R: %.2f, B: %.2f] "
                        "Quad [(%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)]\n",
                        i, info.fColor.toBytes_RGBA(), info.fSrcRect.fLeft, info.fSrcRect.fTop,
                        info.fSrcRect.fRight, info.fSrcRect.fBottom, quad.point(0).fX,
                        quad.point(0).fY, quad.point(1).fX, quad.point(1).fY,
                        quad.point(2).fX, quad.point(2).fY, quad.point(3).fX,
                        quad.point(3).fY);
            }
>>>>>>> upstream-releases
        }
        str += INHERITED::dumpInfo();
        return str;
    }
#endif

<<<<<<< HEAD
    RequiresDstTexture finalize(const GrCaps& caps, const GrAppliedClip* clip) override {
||||||| merged common ancestors
    RequiresDstTexture finalize(const GrCaps& caps, const GrAppliedClip* clip,
                                GrPixelConfigIsClamped dstIsClamped) override {
=======
    GrProcessorSet::Analysis finalize(const GrCaps&, const GrAppliedClip*, GrFSAAType) override {
>>>>>>> upstream-releases
        SkASSERT(!fFinalized);
        fFinalized = true;
<<<<<<< HEAD
        fProxy->addPendingRead();
        fProxy->unref();
        return RequiresDstTexture::kNo;
||||||| merged common ancestors
        fProxy0->addPendingRead();
        fProxy0->unref();
        return RequiresDstTexture::kNo;
=======
        for (unsigned p = 0; p < fProxyCnt; ++p) {
            fProxies[p].fProxy->addPendingRead();
            fProxies[p].fProxy->unref();
        }
        return GrProcessorSet::EmptySetAnalysis();
>>>>>>> upstream-releases
    }

    FixedFunctionFlags fixedFunctionFlags() const override {
        return this->aaType() == GrAAType::kMSAA ? FixedFunctionFlags::kUsesHWAA
                                                 : FixedFunctionFlags::kNone;
    }

    DEFINE_OP_CLASS_ID

private:
<<<<<<< HEAD
    friend class ::GrOpMemoryPool;

    TextureOp(sk_sp<GrTextureProxy> proxy, GrSamplerState::Filter filter, GrColor color,
              const SkRect& srcRect, const SkRect& dstRect, GrAAType aaType, GrQuadAAFlags aaFlags,
              SkCanvas::SrcRectConstraint constraint, const SkMatrix& viewMatrix,
              sk_sp<GrColorSpaceXform> textureColorSpaceXform,
              sk_sp<GrColorSpaceXform> paintColorSpaceXform)
||||||| merged common ancestors

    // This is used in a heursitic for choosing a code path. We don't care what happens with
    // really large rects, infs, nans, etc.
#if defined(__clang__) && (__clang_major__ * 1000 + __clang_minor__) >= 3007
__attribute__((no_sanitize("float-cast-overflow")))
#endif
    size_t RectSizeAsSizeT(const SkRect& rect) {;
        return static_cast<size_t>(SkTMax(rect.width(), 1.f) * SkTMax(rect.height(), 1.f));
    }

    static constexpr int kMaxTextures = TextureGeometryProcessor::kMaxTextures;

    TextureOp(sk_sp<GrTextureProxy> proxy, GrSamplerState::Filter filter, GrColor color,
              const SkRect& srcRect, const SkRect& dstRect, GrAAType aaType,
              const SkMatrix& viewMatrix, sk_sp<GrColorSpaceXform> csxf, bool allowSRGBInputs)
=======
    friend class ::GrOpMemoryPool;

    // dstQuad and dstQuadType should be the geometry transformed by the view matrix.
    // srcRect represents original src rect and will be used as the domain when constraint is strict
    // If srcQuad is provided, it will be used for the local coords instead of srcRect, although
    // srcRect will still specify the domain constraint if needed.
    TextureOp(sk_sp<GrTextureProxy> proxy, GrSamplerState::Filter filter, const SkPMColor4f& color,
              const GrPerspQuad& dstQuad, GrQuadType dstQuadType,
              const SkRect& srcRect, SkCanvas::SrcRectConstraint constraint,
              const GrPerspQuad* srcQuad, GrQuadType srcQuadType, GrAAType aaType,
              GrQuadAAFlags aaFlags, sk_sp<GrColorSpaceXform> textureColorSpaceXform)
            : INHERITED(ClassID())
            , fTextureColorSpaceXform(std::move(textureColorSpaceXform))
            , fFilter(static_cast<unsigned>(filter))
            , fFinalized(0) {
        // Clean up disparities between the overall aa type and edge configuration and apply
        // optimizations based on the rect and matrix when appropriate
        GrResolveAATypeForQuad(aaType, aaFlags, dstQuad, dstQuadType, &aaType, &aaFlags);
        fAAType = static_cast<unsigned>(aaType);

        // We expect our caller to have already caught this optimization.
        SkASSERT(!srcRect.contains(proxy->getWorstCaseBoundsRect()) ||
                 constraint == SkCanvas::kFast_SrcRectConstraint);

        // We may have had a strict constraint with nearest filter solely due to possible AA bloat.
        // If we don't have (or determined we don't need) coverage AA then we can skip using a
        // domain.
        if (constraint == SkCanvas::kStrict_SrcRectConstraint &&
            this->filter() == GrSamplerState::Filter::kNearest &&
            aaType != GrAAType::kCoverage) {
            constraint = SkCanvas::kFast_SrcRectConstraint;
        }

        Domain domain = constraint == SkCanvas::kStrict_SrcRectConstraint ? Domain::kYes
                                                                          : Domain::kNo;
        // Initially, if srcQuad is provided it will always be at index 0 of fSrcQuads
        fQuads.push_back(dstQuad, dstQuadType, {color, srcRect, srcQuad ? 0 : -1, domain, aaFlags});
        if (srcQuad) {
            fSrcQuads.push_back(*srcQuad, srcQuadType);
        }
        fProxyCnt = 1;
        fProxies[0] = {proxy.release(), 1};
        auto bounds = dstQuad.bounds(dstQuadType);
        this->setBounds(bounds, HasAABloat(aaType == GrAAType::kCoverage), IsZeroArea::kNo);
        fDomain = static_cast<unsigned>(domain);
        fWideColor = !SkPMColor4fFitsInBytes(color);
        fCanSkipAllocatorGather =
                static_cast<unsigned>(fProxies[0].fProxy->canSkipResourceAllocator());
    }
    TextureOp(const GrRenderTargetContext::TextureSetEntry set[], int cnt,
              GrSamplerState::Filter filter, GrAAType aaType, const SkMatrix& viewMatrix,
              sk_sp<GrColorSpaceXform> textureColorSpaceXform)
>>>>>>> upstream-releases
            : INHERITED(ClassID())
<<<<<<< HEAD
            , fTextureColorSpaceXform(std::move(textureColorSpaceXform))
            , fPaintColorSpaceXform(std::move(paintColorSpaceXform))
            , fProxy(proxy.release())
            , fFilter(filter)
            , fAAType(static_cast<unsigned>(aaType))
            , fFinalized(0) {
        switch (aaType) {
            case GrAAType::kNone:
                aaFlags = GrQuadAAFlags::kNone;
                break;
            case GrAAType::kCoverage:
                if (aaFlags == GrQuadAAFlags::kNone) {
                    fAAType = static_cast<unsigned>(GrAAType::kNone);
                }
                break;
            case GrAAType::kMSAA:
                aaFlags = GrQuadAAFlags::kAll;
                break;
            case GrAAType::kMixedSamples:
                SK_ABORT("Should not use mixed sample AA");
                break;
        }
        fPerspective = viewMatrix.hasPerspective();
        auto quad = GrPerspQuad(dstRect, viewMatrix);
        auto bounds = quad.bounds();
        // We expect our caller to have already caught this optimization.
        SkASSERT(!srcRect.contains(fProxy->getWorstCaseBoundsRect()) ||
                 constraint == SkCanvas::kFast_SrcRectConstraint);
        if (viewMatrix.rectStaysRect()) {
            // Disable filtering when there is no scaling or fractional translation.
            // Disable coverage AA when rect falls on integers in device space.
            if (SkScalarIsInt(bounds.fLeft) && SkScalarIsInt(bounds.fTop) &&
                SkScalarIsInt(bounds.fRight) && SkScalarIsInt(bounds.fBottom)) {
                if (viewMatrix.isScaleTranslate()) {
                    if (bounds.width() == srcRect.width() && bounds.height() == srcRect.height()) {
                        fFilter = GrSamplerState::Filter::kNearest;
                    }
                } else {
                    if (bounds.width() == srcRect.height() && bounds.height() == srcRect.width()) {
                        fFilter = GrSamplerState::Filter::kNearest;
                    }
                }
                if (GrAAType::kCoverage == this->aaType()) {
                    fAAType = static_cast<unsigned>(GrAAType::kNone);
                    aaFlags = GrQuadAAFlags::kNone;
                }
            }
        }
        // We may have had a strict constraint with nearest filter solely due to possible AA bloat.
        // If we don't have (or determined we don't need) coverage AA then we can skip using a
        // domain.
        if (constraint == SkCanvas::kStrict_SrcRectConstraint &&
            fFilter == GrSamplerState::Filter::kNearest &&
            this->aaType() != GrAAType::kCoverage) {
            constraint = SkCanvas::kFast_SrcRectConstraint;
        }
        const auto& draw = fDraws.emplace_back(srcRect, quad, aaFlags, constraint, color);
        this->setBounds(bounds, HasAABloat::kNo, IsZeroArea::kNo);
        fDomain = static_cast<bool>(draw.domain());
    }

    template <typename Pos, Domain D, GrAA AA>
    void tess(void* v, const GrGeometryProcessor* gp) const {
        using Vertex = TextureGeometryProcessor::Vertex<Pos, D, AA>;
        SkASSERT(gp->debugOnly_vertexStride() == sizeof(Vertex));
        auto vertices = static_cast<Vertex*>(v);
        auto origin = fProxy->origin();
        const auto* texture = fProxy->peekTexture();
        float iw = 1.f / texture->width();
        float ih = 1.f / texture->height();

        for (const auto& draw : fDraws) {
            tessellate_quad<Vertex>(draw.quad(), draw.aaFlags(), draw.srcRect(), draw.color(),
                                    origin, fFilter, vertices, iw, ih, draw.domain());
            vertices += 4;
        }
    }
||||||| merged common ancestors
            , fColorSpaceXform(std::move(csxf))
            , fProxy0(proxy.release())
            , fFilter0(filter)
            , fProxyCnt(1)
            , fAAType(static_cast<unsigned>(aaType))
            , fFinalized(0)
            , fAllowSRGBInputs(allowSRGBInputs ? 1 : 0) {
        SkASSERT(aaType != GrAAType::kMixedSamples);
        Draw& draw = fDraws.push_back();
        draw.fSrcRect = srcRect;
        draw.fTextureIdx = 0;
        draw.fColor = color;
        draw.fQuad.setFromMappedRect(dstRect, viewMatrix);
        SkRect bounds;
        bounds.setBounds(draw.fQuad.points(), 4);
        this->setBounds(bounds, HasAABloat::kNo, IsZeroArea::kNo);

        fMaxApproxDstPixelArea = RectSizeAsSizeT(bounds);
    }
=======
            , fTextureColorSpaceXform(std::move(textureColorSpaceXform))
            , fFilter(static_cast<unsigned>(filter))
            , fFinalized(0) {
        fProxyCnt = SkToUInt(cnt);
        SkRect bounds = SkRectPriv::MakeLargestInverted();
        GrAAType overallAAType = GrAAType::kNone; // aa type maximally compatible with all dst rects
        bool mustFilter = false;
        fCanSkipAllocatorGather = static_cast<unsigned>(true);
        // Most dst rects are transformed by the same view matrix, so their quad types start
        // identical, unless an entry provides a dstClip or additional transform that changes it.
        // The quad list will automatically adapt to that.
        fQuads.reserve(cnt, GrQuadTypeForTransformedRect(viewMatrix));

        for (unsigned p = 0; p < fProxyCnt; ++p) {
            fProxies[p].fProxy = SkRef(set[p].fProxy.get());
            fProxies[p].fQuadCnt = 1;
            SkASSERT(fProxies[p].fProxy->textureType() == fProxies[0].fProxy->textureType());
            SkASSERT(fProxies[p].fProxy->config() == fProxies[0].fProxy->config());
            if (!fProxies[p].fProxy->canSkipResourceAllocator()) {
                fCanSkipAllocatorGather = static_cast<unsigned>(false);
            }
>>>>>>> upstream-releases

<<<<<<< HEAD
    void onPrepareDraws(Target* target) override {
        bool hasPerspective = false;
        Domain domain = Domain::kNo;
        int numOps = 0;
        for (const auto& op : ChainRange<TextureOp>(this)) {
            ++numOps;
            hasPerspective |= op.fPerspective;
            if (op.fDomain) {
                domain = Domain::kYes;
            }
            if (!op.fProxy->instantiate(target->resourceProvider())) {
                return;
||||||| merged common ancestors
    void onPrepareDraws(Target* target) override {
        sk_sp<GrTextureProxy> proxiesSPs[kMaxTextures];
        auto proxies = this->proxies();
        auto filters = this->filters();
        for (int i = 0; i < fProxyCnt; ++i) {
            if (!proxies[i]->instantiate(target->resourceProvider())) {
                return;
=======
            SkMatrix ctm = viewMatrix;
            if (set[p].fPreViewMatrix) {
                ctm.preConcat(*set[p].fPreViewMatrix);
>>>>>>> upstream-releases
            }
<<<<<<< HEAD
        }
||||||| merged common ancestors
            proxiesSPs[i] = sk_ref_sp(proxies[i]);
        }
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
        bool coverageAA = GrAAType::kCoverage == this->aaType();
        sk_sp<GrGeometryProcessor> gp = TextureGeometryProcessor::Make(
                fProxy->textureType(), fProxy->config(), fFilter,
                std::move(fTextureColorSpaceXform), std::move(fPaintColorSpaceXform), coverageAA,
                hasPerspective, domain, *target->caps().shaderCaps());
        GrPipeline::InitArgs args;
        args.fProxy = target->proxy();
        args.fCaps = &target->caps();
        args.fResourceProvider = target->resourceProvider();
        args.fFlags = 0;
        if (GrAAType::kMSAA == this->aaType()) {
            args.fFlags |= GrPipeline::kHWAntialias_Flag;
        }
||||||| merged common ancestors
        bool coverageAA = GrAAType::kCoverage == this->aaType();
        sk_sp<GrGeometryProcessor> gp =
                TextureGeometryProcessor::Make(proxiesSPs, fProxyCnt, std::move(fColorSpaceXform),
                                               coverageAA, filters, *target->caps().shaderCaps());
        GrPipeline::InitArgs args;
        args.fProxy = target->proxy();
        args.fCaps = &target->caps();
        args.fResourceProvider = target->resourceProvider();
        args.fFlags = 0;
        if (fAllowSRGBInputs) {
            args.fFlags |= GrPipeline::kAllowSRGBInputs_Flag;
        }
        if (GrAAType::kMSAA == this->aaType()) {
            args.fFlags |= GrPipeline::kHWAntialias_Flag;
        }
=======
            // Use dstRect unless dstClip is provided, which is assumed to be a quad
            auto quad = set[p].fDstClipQuad == nullptr ?
                    GrPerspQuad::MakeFromRect(set[p].fDstRect, ctm) :
                    GrPerspQuad::MakeFromSkQuad(set[p].fDstClipQuad, ctm);
            GrQuadType quadType = GrQuadTypeForTransformedRect(ctm);
            if (set[p].fDstClipQuad && quadType != GrQuadType::kPerspective) {
                quadType = GrQuadType::kStandard;
            }
>>>>>>> upstream-releases

<<<<<<< HEAD
        auto clip = target->detachAppliedClip();
        // We'll use a dynamic state array for the GP textures when there are multiple ops.
        // Otherwise, we use fixed dynamic state to specify the single op's proxy.
        GrPipeline::DynamicStateArrays* dynamicStateArrays = nullptr;
        GrPipeline::FixedDynamicState* fixedDynamicState;
        if (numOps > 1) {
            dynamicStateArrays = target->allocDynamicStateArrays(numOps, 1, false);
            fixedDynamicState = target->allocFixedDynamicState(clip.scissorState().rect(), 0);
        } else {
            fixedDynamicState = target->allocFixedDynamicState(clip.scissorState().rect(), 1);
            fixedDynamicState->fPrimitiveProcessorTextures[0] = fProxy;
        }
        const auto* pipeline =
                target->allocPipeline(args, GrProcessorSet::MakeEmptySet(), std::move(clip));
        using TessFn = decltype(&TextureOp::tess<SkPoint, Domain::kNo, GrAA::kNo>);
#define TESS_FN_AND_VERTEX_SIZE(Point, Domain, AA)                          \
    {                                                                       \
        &TextureOp::tess<Point, Domain, AA>,                                \
                sizeof(TextureGeometryProcessor::Vertex<Point, Domain, AA>) \
    }
        static constexpr struct {
            TessFn fTessFn;
            size_t fVertexSize;
        } kTessFnsAndVertexSizes[] = {
                TESS_FN_AND_VERTEX_SIZE(SkPoint,  Domain::kNo,  GrAA::kNo),
                TESS_FN_AND_VERTEX_SIZE(SkPoint,  Domain::kNo,  GrAA::kYes),
                TESS_FN_AND_VERTEX_SIZE(SkPoint,  Domain::kYes, GrAA::kNo),
                TESS_FN_AND_VERTEX_SIZE(SkPoint,  Domain::kYes, GrAA::kYes),
                TESS_FN_AND_VERTEX_SIZE(SkPoint3, Domain::kNo,  GrAA::kNo),
                TESS_FN_AND_VERTEX_SIZE(SkPoint3, Domain::kNo,  GrAA::kYes),
                TESS_FN_AND_VERTEX_SIZE(SkPoint3, Domain::kYes, GrAA::kNo),
                TESS_FN_AND_VERTEX_SIZE(SkPoint3, Domain::kYes, GrAA::kYes),
        };
#undef TESS_FN_AND_VERTEX_SIZE
        int tessFnIdx = 0;
        tessFnIdx |= coverageAA               ? 0x1 : 0x0;
        tessFnIdx |= (domain == Domain::kYes) ? 0x2 : 0x0;
        tessFnIdx |= hasPerspective           ? 0x4 : 0x0;

        SkASSERT(kTessFnsAndVertexSizes[tessFnIdx].fVertexSize == gp->debugOnly_vertexStride());

        GrMesh* meshes = target->allocMeshes(numOps);
        int i = 0;
        for (const auto& op : ChainRange<TextureOp>(this)) {
            int vstart;
            const GrBuffer* vbuffer;
            void* vdata = target->makeVertexSpace(kTessFnsAndVertexSizes[tessFnIdx].fVertexSize,
                                                  4 * op.fDraws.count(), &vbuffer, &vstart);
            if (!vdata) {
                SkDebugf("Could not allocate vertices\n");
                return;
||||||| merged common ancestors
        const GrPipeline* pipeline = target->allocPipeline(args, GrProcessorSet::MakeEmptySet(),
                                                           target->detachAppliedClip());
        int vstart;
        const GrBuffer* vbuffer;
        void* vdata = target->makeVertexSpace(gp->getVertexStride(), 4 * fDraws.count(), &vbuffer,
                                              &vstart);
        if (!vdata) {
            SkDebugf("Could not allocate vertices\n");
            return;
        }
        if (1 == fProxyCnt) {
            GrSurfaceOrigin origin = proxies[0]->origin();
            GrTexture* texture = proxies[0]->priv().peekTexture();
            float iw = 1.f / texture->width();
            float ih = 1.f / texture->height();
            if (coverageAA) {
                SkASSERT(gp->getVertexStride() == sizeof(TextureGeometryProcessor::AAVertex));
                auto vertices = static_cast<TextureGeometryProcessor::AAVertex*>(vdata);
                for (int i = 0; i < fDraws.count(); ++i) {
                    tessellate_quad<TextureGeometryProcessor::AAVertex, false, GrAA::kYes>(
                            fDraws[i].fQuad, fDraws[i].fSrcRect, fDraws[i].fColor, origin,
                            vertices + 4 * i, iw, ih, 0);
                }
            } else {
                SkASSERT(gp->getVertexStride() == sizeof(TextureGeometryProcessor::Vertex));
                auto vertices = static_cast<TextureGeometryProcessor::Vertex*>(vdata);
                for (int i = 0; i < fDraws.count(); ++i) {
                    tessellate_quad<TextureGeometryProcessor::Vertex, false, GrAA::kNo>(
                            fDraws[i].fQuad, fDraws[i].fSrcRect, fDraws[i].fColor, origin,
                            vertices + 4 * i, iw, ih, 0);
                }
            }
        } else {
            GrTexture* textures[kMaxTextures];
            float iw[kMaxTextures];
            float ih[kMaxTextures];
            for (int t = 0; t < fProxyCnt; ++t) {
                textures[t] = proxies[t]->priv().peekTexture();
                iw[t] = 1.f / textures[t]->width();
                ih[t] = 1.f / textures[t]->height();
=======
            bounds.joinPossiblyEmptyRect(quad.bounds(quadType));
            GrQuadAAFlags aaFlags;
            // Don't update the overall aaType, might be inappropriate for some of the quads
            GrAAType aaForQuad;
            GrResolveAATypeForQuad(aaType, set[p].fAAFlags, quad, quadType, &aaForQuad, &aaFlags);
            // Resolve sets aaForQuad to aaType or None, there is never a change between aa methods
            SkASSERT(aaForQuad == GrAAType::kNone || aaForQuad == aaType);
            if (overallAAType == GrAAType::kNone && aaForQuad != GrAAType::kNone) {
                overallAAType = aaType;
            }
            if (!mustFilter && this->filter() != GrSamplerState::Filter::kNearest) {
                mustFilter = quadType != GrQuadType::kRect ||
                             GrTextureOp::GetFilterHasEffect(ctm, set[p].fSrcRect,
                                                             set[p].fDstRect);
>>>>>>> upstream-releases
            }
<<<<<<< HEAD

            (op.*(kTessFnsAndVertexSizes[tessFnIdx].fTessFn))(vdata, gp.get());

            if (op.fDraws.count() > 1) {
                meshes[i].setPrimitiveType(GrPrimitiveType::kTriangles);
                sk_sp<const GrBuffer> ibuffer = target->resourceProvider()->refQuadIndexBuffer();
                if (!ibuffer) {
                    SkDebugf("Could not allocate quad indices\n");
                    return;
                }
                meshes[i].setIndexedPatterned(ibuffer.get(), 6, 4, op.fDraws.count(),
                                              GrResourceProvider::QuadCountOfQuadBuffer());
            } else {
                meshes[i].setPrimitiveType(GrPrimitiveType::kTriangleStrip);
                meshes[i].setNonIndexedNonInstanced(4);
||||||| merged common ancestors
            if (coverageAA) {
                SkASSERT(gp->getVertexStride() ==
                         sizeof(TextureGeometryProcessor::AAMultiTextureVertex));
                auto vertices = static_cast<TextureGeometryProcessor::AAMultiTextureVertex*>(vdata);
                for (int i = 0; i < fDraws.count(); ++i) {
                    auto tidx = fDraws[i].fTextureIdx;
                    GrSurfaceOrigin origin = proxies[tidx]->origin();
                    tessellate_quad<TextureGeometryProcessor::AAMultiTextureVertex, true,
                                    GrAA::kYes>(fDraws[i].fQuad, fDraws[i].fSrcRect,
                                                fDraws[i].fColor, origin, vertices + 4 * i,
                                                iw[tidx], ih[tidx], tidx);
                }
            } else {
                SkASSERT(gp->getVertexStride() ==
                         sizeof(TextureGeometryProcessor::MultiTextureVertex));
                auto vertices = static_cast<TextureGeometryProcessor::MultiTextureVertex*>(vdata);
                for (int i = 0; i < fDraws.count(); ++i) {
                    auto tidx = fDraws[i].fTextureIdx;
                    GrSurfaceOrigin origin = proxies[tidx]->origin();
                    tessellate_quad<TextureGeometryProcessor::MultiTextureVertex, true, GrAA::kNo>(
                            fDraws[i].fQuad, fDraws[i].fSrcRect, fDraws[i].fColor, origin,
                            vertices + 4 * i, iw[tidx], ih[tidx], tidx);
                }
=======
            float alpha = SkTPin(set[p].fAlpha, 0.f, 1.f);
            SkPMColor4f color{alpha, alpha, alpha, alpha};
            int srcQuadIndex = -1;
            if (set[p].fDstClipQuad) {
                // Derive new source coordinates that match dstClip's relative locations in dstRect,
                // but with respect to srcRect
                SkPoint srcQuad[4];
                GrMapRectPoints(set[p].fDstRect, set[p].fSrcRect, set[p].fDstClipQuad, srcQuad, 4);
                fSrcQuads.push_back(GrPerspQuad::MakeFromSkQuad(srcQuad, SkMatrix::I()),
                                    GrQuadType::kStandard);
                srcQuadIndex = fSrcQuads.count() - 1;
>>>>>>> upstream-releases
            }
<<<<<<< HEAD
            meshes[i].setVertexData(vbuffer, vstart);
            if (dynamicStateArrays) {
                dynamicStateArrays->fPrimitiveProcessorTextures[i] = op.fProxy;
            }
            ++i;
||||||| merged common ancestors
        }
        GrPrimitiveType primitiveType =
                fDraws.count() > 1 ? GrPrimitiveType::kTriangles : GrPrimitiveType::kTriangleStrip;
        GrMesh mesh(primitiveType);
        if (fDraws.count() > 1) {
            sk_sp<const GrBuffer> ibuffer = target->resourceProvider()->refQuadIndexBuffer();
            if (!ibuffer) {
                SkDebugf("Could not allocate quad indices\n");
                return;
            }
            mesh.setIndexedPatterned(ibuffer.get(), 6, 4, fDraws.count(),
                                     GrResourceProvider::QuadCountOfQuadBuffer());
        } else {
            mesh.setNonIndexedNonInstanced(4);
=======
            fQuads.push_back(quad, quadType,
                             {color, set[p].fSrcRect, srcQuadIndex, Domain::kNo, aaFlags});
        }
        fAAType = static_cast<unsigned>(overallAAType);
        if (!mustFilter) {
            fFilter = static_cast<unsigned>(GrSamplerState::Filter::kNearest);
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        target->draw(std::move(gp), pipeline, fixedDynamicState, dynamicStateArrays, meshes,
                     numOps);
||||||| merged common ancestors
        mesh.setVertexData(vbuffer, vstart);
        target->draw(gp.get(), pipeline, mesh);
=======
        this->setBounds(bounds, HasAABloat(this->aaType() == GrAAType::kCoverage), IsZeroArea::kNo);
        fDomain = static_cast<unsigned>(false);
        fWideColor = static_cast<unsigned>(false);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    CombineResult onCombineIfPossible(GrOp* t, const GrCaps& caps) override {
        const auto* that = t->cast<TextureOp>();
        if (!GrColorSpaceXform::Equals(fTextureColorSpaceXform.get(),
                                       that->fTextureColorSpaceXform.get())) {
            return CombineResult::kCannotCombine;
||||||| merged common ancestors
    bool onCombineIfPossible(GrOp* t, const GrCaps& caps) override {
        const auto* that = t->cast<TextureOp>();
        const auto& shaderCaps = *caps.shaderCaps();
        if (!GrColorSpaceXform::Equals(fColorSpaceXform.get(), that->fColorSpaceXform.get())) {
            return false;
=======
    void tess(void* v, const VertexSpec& spec, const GrTextureProxy* proxy, int start,
              int cnt) const {
        TRACE_EVENT0("skia", TRACE_FUNC);
        auto origin = proxy->origin();
        const auto* texture = proxy->peekTexture();
        float iw, ih, h;
        if (proxy->textureType() == GrTextureType::kRectangle) {
            iw = ih = 1.f;
            h = texture->height();
        } else {
            iw = 1.f / texture->width();
            ih = 1.f / texture->height();
            h = 1.f;
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        if (!GrColorSpaceXform::Equals(fPaintColorSpaceXform.get(),
                                       that->fPaintColorSpaceXform.get())) {
            return CombineResult::kCannotCombine;
||||||| merged common ancestors
        if (this->aaType() != that->aaType()) {
            return false;
=======

        for (int i = start; i < start + cnt; ++i) {
            const GrPerspQuad& device = fQuads[i];
            const ColorDomainAndAA& info = fQuads.metadata(i);

            GrPerspQuad srcQuad = info.fSrcQuadIndex >= 0 ?
                    compute_src_quad(origin, fSrcQuads[info.fSrcQuadIndex], iw, ih, h) :
                    compute_src_quad_from_rect(origin, info.fSrcRect, iw, ih, h);
            SkRect domain =
                    compute_domain(info.domain(), this->filter(), origin, info.fSrcRect, iw, ih, h);
            v = GrQuadPerEdgeAA::Tessellate(v, spec, device, info.fColor, srcQuad, domain,
                                            info.aaFlags());
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        // TODO: Should we allow kNone and kCoverage to merge by upgrading kNone to kCoverage?
        // If we allowed chaining the head op would have to pre-iterate to determine the aa-type.
        if (this->aaType() != that->aaType()) {
            return CombineResult::kCannotCombine;
||||||| merged common ancestors
        // Because of an issue where GrColorSpaceXform adds the same function every time it is used
        // in a texture lookup, we only allow multiple textures when there is no transform.
        if (TextureGeometryProcessor::SupportsMultitexture(shaderCaps) && !fColorSpaceXform &&
            fMaxApproxDstPixelArea <= shaderCaps.disableImageMultitexturingDstRectAreaThreshold() &&
            that->fMaxApproxDstPixelArea <=
                    shaderCaps.disableImageMultitexturingDstRectAreaThreshold()) {
            int map[kMaxTextures];
            int numNewProxies = this->mergeProxies(that, map, shaderCaps);
            if (numNewProxies < 0) {
                return false;
            }
            if (1 == fProxyCnt && numNewProxies) {
                void* mem = new char[(sizeof(GrSamplerState::Filter) + sizeof(GrTextureProxy*)) *
                                     kMaxTextures];
                auto proxies = reinterpret_cast<GrTextureProxy**>(mem);
                auto filters = reinterpret_cast<GrSamplerState::Filter*>(proxies + kMaxTextures);
                proxies[0] = fProxy0;
                filters[0] = fFilter0;
                fProxyArray = proxies;
            }
            fProxyCnt += numNewProxies;
            auto thisProxies = fProxyArray;
            auto thatProxies = that->proxies();
            auto thatFilters = that->filters();
            auto thisFilters = reinterpret_cast<GrSamplerState::Filter*>(thisProxies +
                    kMaxTextures);
            for (int i = 0; i < that->fProxyCnt; ++i) {
                if (map[i] < 0) {
                    thatProxies[i]->addPendingRead();

                    thisProxies[-map[i]] = thatProxies[i];
                    thisFilters[-map[i]] = thatFilters[i];
                    map[i] = -map[i];
                }
            }
            int firstNewDraw = fDraws.count();
            fDraws.push_back_n(that->fDraws.count(), that->fDraws.begin());
            for (int i = firstNewDraw; i < fDraws.count(); ++i) {
                fDraws[i].fTextureIdx = map[fDraws[i].fTextureIdx];
            }
        } else {
            // We can get here when one of the ops is already multitextured but the other cannot
            // be because of the dst rect size.
            if (fProxyCnt > 1 || that->fProxyCnt > 1) {
                return false;
            }
            if (fProxy0->uniqueID() != that->fProxy0->uniqueID() || fFilter0 != that->fFilter0) {
                return false;
            }
            fDraws.push_back_n(that->fDraws.count(), that->fDraws.begin());
        }
        this->joinBounds(*that);
        fMaxApproxDstPixelArea = SkTMax(that->fMaxApproxDstPixelArea, fMaxApproxDstPixelArea);
        return true;
    }

    /**
     * Determines a mapping of indices from that's proxy array to this's proxy array. A negative map
     * value means that's proxy should be added to this's proxy array at the absolute value of
     * the map entry. If it is determined that the ops shouldn't combine their proxies then a
     * negative value is returned. Otherwise, return value indicates the number of proxies that have
     * to be added to this op or, equivalently, the number of negative entries in map.
     */
    int mergeProxies(const TextureOp* that, int map[kMaxTextures], const GrShaderCaps& caps) const {
        std::fill_n(map, kMaxTextures, -kMaxTextures);
        int sharedProxyCnt = 0;
        auto thisProxies = this->proxies();
        auto thisFilters = this->filters();
        auto thatProxies = that->proxies();
        auto thatFilters = that->filters();
        for (int i = 0; i < fProxyCnt; ++i) {
            for (int j = 0; j < that->fProxyCnt; ++j) {
                if (thisProxies[i]->uniqueID() == thatProxies[j]->uniqueID()) {
                    if (thisFilters[i] != thatFilters[j]) {
                        // In GL we don't currently support using the same texture with different
                        // samplers. If we added support for sampler objects and a cap bit to know
                        // it's ok to use different filter modes then we could support this.
                        // Otherwise, we could also only allow a single filter mode for each op
                        // instance.
                        return -1;
                    }
                    map[j] = i;
                    ++sharedProxyCnt;
                    break;
                }
            }
=======
    }

    void onPrepareDraws(Target* target) override {
        TRACE_EVENT0("skia", TRACE_FUNC);
        GrQuadType quadType = GrQuadType::kRect;
        GrQuadType srcQuadType = GrQuadType::kRect;
        Domain domain = Domain::kNo;
        bool wideColor = false;
        int numProxies = 0;
        int numTotalQuads = 0;
        auto textureType = fProxies[0].fProxy->textureType();
        auto config = fProxies[0].fProxy->config();
        GrAAType aaType = this->aaType();
        for (const auto& op : ChainRange<TextureOp>(this)) {
            if (op.fQuads.quadType() > quadType) {
                quadType = op.fQuads.quadType();
            }
            if (op.fSrcQuads.quadType() > srcQuadType) {
                // Should only become more general if there are quads to use instead of fSrcRect
                SkASSERT(op.fSrcQuads.count() > 0);
                srcQuadType = op.fSrcQuads.quadType();
            }
            if (op.fDomain) {
                domain = Domain::kYes;
            }
            wideColor |= op.fWideColor;
            numProxies += op.fProxyCnt;
            for (unsigned p = 0; p < op.fProxyCnt; ++p) {
                numTotalQuads += op.fProxies[p].fQuadCnt;
                auto* proxy = op.fProxies[p].fProxy;
                if (!proxy->instantiate(target->resourceProvider())) {
                    return;
                }
                SkASSERT(proxy->config() == config);
                SkASSERT(proxy->textureType() == textureType);
            }
            if (op.aaType() == GrAAType::kCoverage) {
                SkASSERT(aaType == GrAAType::kCoverage || aaType == GrAAType::kNone);
                aaType = GrAAType::kCoverage;
            }
        }

        VertexSpec vertexSpec(quadType, wideColor ? ColorType::kHalf : ColorType::kByte,
                              srcQuadType, /* hasLocal */ true, domain, aaType,
                              /* alpha as coverage */ true);

        GrSamplerState samplerState = GrSamplerState(GrSamplerState::WrapMode::kClamp,
                                                     this->filter());
        GrGpu* gpu = target->resourceProvider()->priv().gpu();
        uint32_t extraSamplerKey = gpu->getExtraSamplerKeyForProgram(
                samplerState, fProxies[0].fProxy->backendFormat());

        sk_sp<GrGeometryProcessor> gp = GrQuadPerEdgeAA::MakeTexturedProcessor(
                vertexSpec, *target->caps().shaderCaps(),
                textureType, config, samplerState, extraSamplerKey,
                std::move(fTextureColorSpaceXform));

        // We'll use a dynamic state array for the GP textures when there are multiple ops.
        // Otherwise, we use fixed dynamic state to specify the single op's proxy.
        GrPipeline::DynamicStateArrays* dynamicStateArrays = nullptr;
        GrPipeline::FixedDynamicState* fixedDynamicState;
        if (numProxies > 1) {
            dynamicStateArrays = target->allocDynamicStateArrays(numProxies, 1, false);
            fixedDynamicState = target->makeFixedDynamicState(0);
        } else {
            fixedDynamicState = target->makeFixedDynamicState(1);
            fixedDynamicState->fPrimitiveProcessorTextures[0] = fProxies[0].fProxy;
        }

        size_t vertexSize = gp->vertexStride();

        GrMesh* meshes = target->allocMeshes(numProxies);
        sk_sp<const GrBuffer> vbuffer;
        int vertexOffsetInBuffer = 0;
        int numQuadVerticesLeft = numTotalQuads * vertexSpec.verticesPerQuad();
        int numAllocatedVertices = 0;
        void* vdata = nullptr;

        int m = 0;
        for (const auto& op : ChainRange<TextureOp>(this)) {
            int q = 0;
            for (unsigned p = 0; p < op.fProxyCnt; ++p) {
                int quadCnt = op.fProxies[p].fQuadCnt;
                auto* proxy = op.fProxies[p].fProxy;
                int meshVertexCnt = quadCnt * vertexSpec.verticesPerQuad();
                if (numAllocatedVertices < meshVertexCnt) {
                    vdata = target->makeVertexSpaceAtLeast(
                            vertexSize, meshVertexCnt, numQuadVerticesLeft, &vbuffer,
                            &vertexOffsetInBuffer, &numAllocatedVertices);
                    SkASSERT(numAllocatedVertices <= numQuadVerticesLeft);
                    if (!vdata) {
                        SkDebugf("Could not allocate vertices\n");
                        return;
                    }
                }
                SkASSERT(numAllocatedVertices >= meshVertexCnt);

                op.tess(vdata, vertexSpec, proxy, q, quadCnt);

                if (!GrQuadPerEdgeAA::ConfigureMeshIndices(target, &(meshes[m]), vertexSpec,
                                                           quadCnt)) {
                    SkDebugf("Could not allocate indices");
                    return;
                }
                meshes[m].setVertexData(vbuffer, vertexOffsetInBuffer);
                if (dynamicStateArrays) {
                    dynamicStateArrays->fPrimitiveProcessorTextures[m] = proxy;
                }
                ++m;
                numAllocatedVertices -= meshVertexCnt;
                numQuadVerticesLeft -= meshVertexCnt;
                vertexOffsetInBuffer += meshVertexCnt;
                vdata = reinterpret_cast<char*>(vdata) + vertexSize * meshVertexCnt;
                q += quadCnt;
            }
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        if (fFilter != that->fFilter) {
            return CombineResult::kCannotCombine;
||||||| merged common ancestors
        int actualMaxTextures = SkTMin(caps.maxFragmentSamplers(), kMaxTextures);
        int newProxyCnt = that->fProxyCnt - sharedProxyCnt;
        if (newProxyCnt + fProxyCnt > actualMaxTextures) {
            return -1;
=======
        SkASSERT(!numQuadVerticesLeft);
        SkASSERT(!numAllocatedVertices);
        target->recordDraw(
                std::move(gp), meshes, numProxies, fixedDynamicState, dynamicStateArrays);
    }

    void onExecute(GrOpFlushState* flushState, const SkRect& chainBounds) override {
        auto pipelineFlags = (GrAAType::kMSAA == this->aaType())
                ? GrPipeline::kHWAntialias_Flag
                : 0;
        flushState->executeDrawsAndUploadsForMeshDrawOp(
                this, chainBounds, GrProcessorSet::MakeEmptySet(), pipelineFlags);
    }

    CombineResult onCombineIfPossible(GrOp* t, const GrCaps& caps) override {
        TRACE_EVENT0("skia", TRACE_FUNC);
        const auto* that = t->cast<TextureOp>();
        if (!GrColorSpaceXform::Equals(fTextureColorSpaceXform.get(),
                                       that->fTextureColorSpaceXform.get())) {
            return CombineResult::kCannotCombine;
        }
        bool upgradeToCoverageAAOnMerge = false;
        if (this->aaType() != that->aaType()) {
            if (!((this->aaType() == GrAAType::kCoverage && that->aaType() == GrAAType::kNone) ||
                  (that->aaType() == GrAAType::kCoverage && this->aaType() == GrAAType::kNone))) {
                return CombineResult::kCannotCombine;
            }
            upgradeToCoverageAAOnMerge = true;
        }
        if (fFilter != that->fFilter) {
            return CombineResult::kCannotCombine;
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        if (fProxy->uniqueID() != that->fProxy->uniqueID() || that->isChained()) {
            // We can't merge across different proxies (and we're disallowed from merging when
            // 'that' is chained. Check if we can be chained with 'that'.
            if (fProxy->config() == that->fProxy->config() &&
                fProxy->textureType() == that->fProxy->textureType() &&
                caps.dynamicStateArrayGeometryProcessorTextureSupport()) {
                return CombineResult::kMayChain;
||||||| merged common ancestors
        GrPixelConfig config = thisProxies[0]->config();
        int nextSlot = fProxyCnt;
        for (int j = 0; j < that->fProxyCnt; ++j) {
            // We want to avoid making many shaders because of different permutations of shader
            // based swizzle and sampler types. The approach taken here is to require the configs to
            // be the same and to only allow already instantiated proxies that have the most
            // common sampler type. Otherwise we don't merge.
            if (thatProxies[j]->config() != config) {
                return -1;
            }
            if (GrTexture* tex = thatProxies[j]->priv().peekTexture()) {
                if (tex->texturePriv().samplerType() != kTexture2DSampler_GrSLType) {
                    return -1;
                }
            }
            if (map[j] < 0) {
                map[j] = -(nextSlot++);
=======
        auto thisProxy = fProxies[0].fProxy;
        auto thatProxy = that->fProxies[0].fProxy;
        if (fProxyCnt > 1 || that->fProxyCnt > 1 ||
            thisProxy->uniqueID() != thatProxy->uniqueID()) {
            // We can't merge across different proxies. Check if 'this' can be chained with 'that'.
            if (GrTextureProxy::ProxiesAreCompatibleAsDynamicState(thisProxy, thatProxy) &&
                caps.dynamicStateArrayGeometryProcessorTextureSupport()) {
                return CombineResult::kMayChain;
            }
            return CombineResult::kCannotCombine;
        }

        fDomain |= that->fDomain;
        fWideColor |= that->fWideColor;
        if (upgradeToCoverageAAOnMerge) {
            fAAType = static_cast<unsigned>(GrAAType::kCoverage);
        }

        // Concatenate quad lists together, updating the fSrcQuadIndex in the appended quads
        // to account for the new starting index in fSrcQuads
        int srcQuadOffset = fSrcQuads.count();
        int oldQuadCount = fQuads.count();

        fSrcQuads.concat(that->fSrcQuads);
        fQuads.concat(that->fQuads);
        fProxies[0].fQuadCnt += that->fQuads.count();

        if (that->fSrcQuads.count() > 0) {
            // Some of the concatenated quads pointed to fSrcQuads, so adjust the indices for the
            // newly appended quads
            for (int i = oldQuadCount; i < fQuads.count(); ++i) {
                if (fQuads.metadata(i).fSrcQuadIndex >= 0) {
                    fQuads.metadata(i).fSrcQuadIndex += srcQuadOffset;
                }
            }
        }

        // Confirm all tracked state makes sense when in debug builds
#ifdef SK_DEBUG
        SkASSERT(fSrcQuads.count() <= fQuads.count());
        for (int i = 0; i < fQuads.count(); ++i) {
            int srcIndex = fQuads.metadata(i).fSrcQuadIndex;
            if (srcIndex >= 0) {
                // Make sure it points to a valid index, in the right region of the list
                SkASSERT(srcIndex < fSrcQuads.count());
                SkASSERT((i < oldQuadCount && srcIndex < srcQuadOffset) ||
                         (i >= oldQuadCount && srcIndex >= srcQuadOffset));
>>>>>>> upstream-releases
            }
            return CombineResult::kCannotCombine;
        }
<<<<<<< HEAD
        fDraws.push_back_n(that->fDraws.count(), that->fDraws.begin());
        this->joinBounds(*that);
        fPerspective |= that->fPerspective;
        fDomain |= that->fDomain;
        return CombineResult::kMerged;
||||||| merged common ancestors
        return newProxyCnt;
=======
#endif
        return CombineResult::kMerged;
>>>>>>> upstream-releases
    }

    GrAAType aaType() const { return static_cast<GrAAType>(fAAType); }
<<<<<<< HEAD

    class Draw {
    public:
        Draw(const SkRect& srcRect, const GrPerspQuad& quad, GrQuadAAFlags aaFlags,
             SkCanvas::SrcRectConstraint constraint, GrColor color)
                : fSrcRect(srcRect)
                , fQuad(quad)
                , fColor(color)
                , fHasDomain(constraint == SkCanvas::kStrict_SrcRectConstraint)
                , fAAFlags(static_cast<unsigned>(aaFlags)) {
            SkASSERT(fAAFlags == static_cast<unsigned>(aaFlags));
||||||| merged common ancestors

    GrTextureProxy* const* proxies() const { return fProxyCnt > 1 ? fProxyArray : &fProxy0; }

    const GrSamplerState::Filter* filters() const {
        if (fProxyCnt > 1) {
            return reinterpret_cast<const GrSamplerState::Filter*>(fProxyArray + kMaxTextures);
=======
    GrSamplerState::Filter filter() const { return static_cast<GrSamplerState::Filter>(fFilter); }

    struct ColorDomainAndAA {
        // Special constructor to convert enums into the packed bits, which should not delete
        // the implicit move constructor (but it does require us to declare an empty ctor for
        // use with the GrTQuadList).
        ColorDomainAndAA(const SkPMColor4f& color, const SkRect& srcRect, int srcQuadIndex,
                         Domain hasDomain, GrQuadAAFlags aaFlags)
                : fColor(color)
                , fSrcRect(srcRect)
                , fSrcQuadIndex(srcQuadIndex)
                , fHasDomain(static_cast<unsigned>(hasDomain))
                , fAAFlags(static_cast<unsigned>(aaFlags)) {
            SkASSERT(fHasDomain == static_cast<unsigned>(hasDomain));
            SkASSERT(fAAFlags == static_cast<unsigned>(aaFlags));
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        const GrPerspQuad& quad() const { return fQuad; }
        const SkRect& srcRect() const { return fSrcRect; }
        GrColor color() const { return fColor; }
        Domain domain() const { return Domain(fHasDomain); }
        GrQuadAAFlags aaFlags() const { return static_cast<GrQuadAAFlags>(fAAFlags); }
||||||| merged common ancestors
        return &fFilter0;
    }
=======
        ColorDomainAndAA() = default;
>>>>>>> upstream-releases

<<<<<<< HEAD
    private:
||||||| merged common ancestors
    struct Draw {
=======
        SkPMColor4f fColor;
        // Even if fSrcQuadIndex provides source coords, use fSrcRect for domain constraint
>>>>>>> upstream-releases
        SkRect fSrcRect;
<<<<<<< HEAD
        GrPerspQuad fQuad;
        GrColor fColor;
        unsigned fHasDomain : 1;
        unsigned fAAFlags : 4;
||||||| merged common ancestors
        int fTextureIdx;
        GrQuad fQuad;
        GrColor fColor;
=======
        // If >= 0, use to access fSrcQuads instead of fSrcRect for the source coordinates
        int fSrcQuadIndex;
        unsigned fHasDomain : 1;
        unsigned fAAFlags : 4;

        Domain domain() const { return Domain(fHasDomain); }
        GrQuadAAFlags aaFlags() const { return static_cast<GrQuadAAFlags>(fAAFlags); }
>>>>>>> upstream-releases
    };
<<<<<<< HEAD
    SkSTArray<1, Draw, true> fDraws;
    sk_sp<GrColorSpaceXform> fTextureColorSpaceXform;
    sk_sp<GrColorSpaceXform> fPaintColorSpaceXform;
    GrTextureProxy* fProxy;
    GrSamplerState::Filter fFilter;
||||||| merged common ancestors
    SkSTArray<1, Draw, true> fDraws;
    sk_sp<GrColorSpaceXform> fColorSpaceXform;
    // Initially we store a single proxy ptr and a single filter. If we grow to have more than
    // one proxy we instead store pointers to dynamically allocated arrays of size kMaxTextures
    // followed by kMaxTextures filters.
    union {
        GrTextureProxy* fProxy0;
        GrTextureProxy** fProxyArray;
    };
    size_t fMaxApproxDstPixelArea;
    GrSamplerState::Filter fFilter0;
    uint8_t fProxyCnt;
=======
    struct Proxy {
        GrTextureProxy* fProxy;
        int fQuadCnt;
    };
    GrTQuadList<ColorDomainAndAA> fQuads;
    // The majority of texture ops will not track a complete src quad so this is indexed separately
    // and may be of different size to fQuads.
    GrQuadList fSrcQuads;
    sk_sp<GrColorSpaceXform> fTextureColorSpaceXform;
    unsigned fFilter : 2;
>>>>>>> upstream-releases
    unsigned fAAType : 2;
<<<<<<< HEAD
    unsigned fPerspective : 1;
    unsigned fDomain : 1;
||||||| merged common ancestors
=======
    unsigned fDomain : 1;
    unsigned fWideColor : 1;
>>>>>>> upstream-releases
    // Used to track whether fProxy is ref'ed or has a pending IO after finalize() is called.
    unsigned fFinalized : 1;
<<<<<<< HEAD
||||||| merged common ancestors
    unsigned fAllowSRGBInputs : 1;
=======
    unsigned fCanSkipAllocatorGather : 1;
    unsigned fProxyCnt : 32 - 8;
    Proxy fProxies[1];

    static_assert(kGrQuadTypeCount <= 4, "GrQuadType does not fit in 2 bits");
>>>>>>> upstream-releases

    typedef GrMeshDrawOp INHERITED;
};

}  // anonymous namespace

namespace GrTextureOp {

<<<<<<< HEAD
std::unique_ptr<GrDrawOp> Make(GrContext* context,
                               sk_sp<GrTextureProxy> proxy,
                               GrSamplerState::Filter filter,
                               GrColor color,
                               const SkRect& srcRect,
                               const SkRect& dstRect,
                               GrAAType aaType,
                               GrQuadAAFlags aaFlags,
                               SkCanvas::SrcRectConstraint constraint,
                               const SkMatrix& viewMatrix,
                               sk_sp<GrColorSpaceXform> textureColorSpaceXform,
                               sk_sp<GrColorSpaceXform> paintColorSpaceXform) {
    return TextureOp::Make(context, std::move(proxy), filter, color, srcRect, dstRect, aaType,
                           aaFlags, constraint, viewMatrix, std::move(textureColorSpaceXform),
                           std::move(paintColorSpaceXform));
||||||| merged common ancestors
std::unique_ptr<GrDrawOp> Make(sk_sp<GrTextureProxy> proxy, GrSamplerState::Filter filter,
                               GrColor color, const SkRect& srcRect, const SkRect& dstRect,
                               GrAAType aaType, const SkMatrix& viewMatrix,
                               sk_sp<GrColorSpaceXform> csxf, bool allowSRGBInputs) {
    SkASSERT(!viewMatrix.hasPerspective());
    return TextureOp::Make(std::move(proxy), filter, color, srcRect, dstRect, aaType, viewMatrix,
                           std::move(csxf), allowSRGBInputs);
=======
std::unique_ptr<GrDrawOp> Make(GrRecordingContext* context,
                               sk_sp<GrTextureProxy> proxy,
                               GrSamplerState::Filter filter,
                               const SkPMColor4f& color,
                               const SkRect& srcRect,
                               const SkRect& dstRect,
                               GrAAType aaType,
                               GrQuadAAFlags aaFlags,
                               SkCanvas::SrcRectConstraint constraint,
                               const SkMatrix& viewMatrix,
                               sk_sp<GrColorSpaceXform> textureColorSpaceXform) {
    return TextureOp::Make(context, std::move(proxy), filter, color, srcRect, dstRect, aaType,
                           aaFlags, constraint, viewMatrix, std::move(textureColorSpaceXform));
}

std::unique_ptr<GrDrawOp> MakeQuad(GrRecordingContext* context,
                                  sk_sp<GrTextureProxy> proxy,
                                  GrSamplerState::Filter filter,
                                  const SkPMColor4f& color,
                                  const SkPoint srcQuad[4],
                                  const SkPoint dstQuad[4],
                                  GrAAType aaType,
                                  GrQuadAAFlags aaFlags,
                                  const SkRect* domain,
                                  const SkMatrix& viewMatrix,
                                  sk_sp<GrColorSpaceXform> textureXform) {
    return TextureOp::Make(context, std::move(proxy), filter, color, srcQuad, dstQuad, aaType,
                           aaFlags, domain, viewMatrix, std::move(textureXform));
}

std::unique_ptr<GrDrawOp> MakeSet(GrRecordingContext* context,
                                  const GrRenderTargetContext::TextureSetEntry set[],
                                  int cnt,
                                  GrSamplerState::Filter filter,
                                  GrAAType aaType,
                                  const SkMatrix& viewMatrix,
                                  sk_sp<GrColorSpaceXform> textureColorSpaceXform) {
    return TextureOp::Make(context, set, cnt, filter, aaType, viewMatrix,
                           std::move(textureColorSpaceXform));
}

bool GetFilterHasEffect(const SkMatrix& viewMatrix, const SkRect& srcRect, const SkRect& dstRect) {
    // Hypothetically we could disable bilerp filtering when flipping or rotating 90 degrees, but
    // that makes the math harder and we don't want to increase the overhead of the checks
    if (!viewMatrix.isScaleTranslate() ||
        viewMatrix.getScaleX() < 0.0f || viewMatrix.getScaleY() < 0.0f) {
        return true;
    }

    // Given the matrix conditions ensured above, this computes the device space coordinates for
    // the top left corner of dstRect and its size.
    SkScalar dw = viewMatrix.getScaleX() * dstRect.width();
    SkScalar dh = viewMatrix.getScaleY() * dstRect.height();
    SkScalar dl = viewMatrix.getScaleX() * dstRect.fLeft + viewMatrix.getTranslateX();
    SkScalar dt = viewMatrix.getScaleY() * dstRect.fTop + viewMatrix.getTranslateY();

    // Disable filtering when there is no scaling of the src rect and the src rect and dst rect
    // align fractionally. If we allow inverted src rects this logic needs to consider that.
    SkASSERT(srcRect.isSorted());
    return dw != srcRect.width() || dh != srcRect.height() ||
           SkScalarFraction(dl) != SkScalarFraction(srcRect.fLeft) ||
           SkScalarFraction(dt) != SkScalarFraction(srcRect.fTop);
>>>>>>> upstream-releases
}

}  // namespace GrTextureOp

#if GR_TEST_UTILS
#include "GrProxyProvider.h"
#include "GrRecordingContext.h"
#include "GrRecordingContextPriv.h"

GR_DRAW_OP_TEST_DEFINE(TextureOp) {
    GrSurfaceDesc desc;
    desc.fConfig = kRGBA_8888_GrPixelConfig;
    desc.fHeight = random->nextULessThan(90) + 10;
    desc.fWidth = random->nextULessThan(90) + 10;
    auto origin = random->nextBool() ? kTopLeft_GrSurfaceOrigin : kBottomLeft_GrSurfaceOrigin;
    GrMipMapped mipMapped = random->nextBool() ? GrMipMapped::kYes : GrMipMapped::kNo;
    SkBackingFit fit = SkBackingFit::kExact;
    if (mipMapped == GrMipMapped::kNo) {
        fit = random->nextBool() ? SkBackingFit::kApprox : SkBackingFit::kExact;
    }

<<<<<<< HEAD
    GrProxyProvider* proxyProvider = context->contextPriv().proxyProvider();
    sk_sp<GrTextureProxy> proxy = proxyProvider->createProxy(desc, origin, mipMapped, fit,
                                                             SkBudgeted::kNo,
                                                             GrInternalSurfaceFlags::kNone);
||||||| merged common ancestors
    GrProxyProvider* proxyProvider = context->contextPriv().proxyProvider();
    sk_sp<GrTextureProxy> proxy = proxyProvider->createProxy(desc, fit, SkBudgeted::kNo);
=======
    const GrBackendFormat format =
            context->priv().caps()->getBackendFormatFromColorType(kRGBA_8888_SkColorType);

    GrProxyProvider* proxyProvider = context->priv().proxyProvider();
    sk_sp<GrTextureProxy> proxy = proxyProvider->createProxy(format, desc, origin, mipMapped, fit,
                                                             SkBudgeted::kNo,
                                                             GrInternalSurfaceFlags::kNone);
>>>>>>> upstream-releases

    SkRect rect = GrTest::TestRect(random);
    SkRect srcRect;
    srcRect.fLeft = random->nextRangeScalar(0.f, proxy->width() / 2.f);
    srcRect.fRight = random->nextRangeScalar(0.f, proxy->width()) + proxy->width() / 2.f;
    srcRect.fTop = random->nextRangeScalar(0.f, proxy->height() / 2.f);
    srcRect.fBottom = random->nextRangeScalar(0.f, proxy->height()) + proxy->height() / 2.f;
    SkMatrix viewMatrix = GrTest::TestMatrixPreservesRightAngles(random);
    SkPMColor4f color = SkPMColor4f::FromBytes_RGBA(SkColorToPremulGrColor(random->nextU()));
    GrSamplerState::Filter filter = (GrSamplerState::Filter)random->nextULessThan(
            static_cast<uint32_t>(GrSamplerState::Filter::kMipMap) + 1);
<<<<<<< HEAD
    while (mipMapped == GrMipMapped::kNo && filter == GrSamplerState::Filter::kMipMap) {
        filter = (GrSamplerState::Filter)random->nextULessThan(
                static_cast<uint32_t>(GrSamplerState::Filter::kMipMap) + 1);
    }
    auto texXform = GrTest::TestColorXform(random);
    auto paintXform = GrTest::TestColorXform(random);
||||||| merged common ancestors
    auto csxf = GrTest::TestColorXform(random);
    bool allowSRGBInputs = random->nextBool();
=======
    while (mipMapped == GrMipMapped::kNo && filter == GrSamplerState::Filter::kMipMap) {
        filter = (GrSamplerState::Filter)random->nextULessThan(
                static_cast<uint32_t>(GrSamplerState::Filter::kMipMap) + 1);
    }
    auto texXform = GrTest::TestColorXform(random);
>>>>>>> upstream-releases
    GrAAType aaType = GrAAType::kNone;
    if (random->nextBool()) {
        aaType = (fsaaType == GrFSAAType::kUnifiedMSAA) ? GrAAType::kMSAA : GrAAType::kCoverage;
    }
<<<<<<< HEAD
    GrQuadAAFlags aaFlags = GrQuadAAFlags::kNone;
    aaFlags |= random->nextBool() ? GrQuadAAFlags::kLeft : GrQuadAAFlags::kNone;
    aaFlags |= random->nextBool() ? GrQuadAAFlags::kTop : GrQuadAAFlags::kNone;
    aaFlags |= random->nextBool() ? GrQuadAAFlags::kRight : GrQuadAAFlags::kNone;
    aaFlags |= random->nextBool() ? GrQuadAAFlags::kBottom : GrQuadAAFlags::kNone;
    auto constraint = random->nextBool() ? SkCanvas::kStrict_SrcRectConstraint
                                         : SkCanvas::kFast_SrcRectConstraint;
    return GrTextureOp::Make(context, std::move(proxy), filter, color, srcRect, rect, aaType,
                             aaFlags, constraint, viewMatrix, std::move(texXform),
                             std::move(paintXform));
||||||| merged common ancestors
    return GrTextureOp::Make(std::move(proxy), filter, color, srcRect, rect, aaType, viewMatrix,
                             std::move(csxf), allowSRGBInputs);
=======
    GrQuadAAFlags aaFlags = GrQuadAAFlags::kNone;
    aaFlags |= random->nextBool() ? GrQuadAAFlags::kLeft : GrQuadAAFlags::kNone;
    aaFlags |= random->nextBool() ? GrQuadAAFlags::kTop : GrQuadAAFlags::kNone;
    aaFlags |= random->nextBool() ? GrQuadAAFlags::kRight : GrQuadAAFlags::kNone;
    aaFlags |= random->nextBool() ? GrQuadAAFlags::kBottom : GrQuadAAFlags::kNone;
    auto constraint = random->nextBool() ? SkCanvas::kStrict_SrcRectConstraint
                                         : SkCanvas::kFast_SrcRectConstraint;
    return GrTextureOp::Make(context, std::move(proxy), filter, color, srcRect, rect, aaType,
                             aaFlags, constraint, viewMatrix, std::move(texXform));
>>>>>>> upstream-releases
}

#endif
