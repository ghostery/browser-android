/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrDefaultGeoProcFactory.h"

#include "GrCaps.h"
#include "SkRefCnt.h"
#include "glsl/GrGLSLColorSpaceXformHelper.h"
#include "glsl/GrGLSLFragmentShaderBuilder.h"
#include "glsl/GrGLSLGeometryProcessor.h"
#include "glsl/GrGLSLVertexGeoBuilder.h"
#include "glsl/GrGLSLVarying.h"
#include "glsl/GrGLSLUniformHandler.h"
#include "glsl/GrGLSLUtil.h"

/*
 * The default Geometry Processor simply takes position and multiplies it by the uniform view
 * matrix. It also leaves coverage untouched.  Behind the scenes, we may add per vertex color or
 * local coords.
 */

enum GPFlag {
    kColorAttribute_GPFlag          = 0x1,
    kColorAttributeIsSkColor_GPFlag = 0x2,
<<<<<<< HEAD
    kLocalCoordAttribute_GPFlag     = 0x4,
    kCoverageAttribute_GPFlag       = 0x8,
    kBonesAttribute_GPFlag          = 0x10,
||||||| merged common ancestors
    kLocalCoordAttribute_GPFlag     = 0x4,
    kCoverageAttribute_GPFlag       = 0x8,

    kLinearizeColorAttribute_GPFlag = 0x10,
=======
    kColorAttributeIsWide_GPFlag    = 0x4,
    kLocalCoordAttribute_GPFlag     = 0x8,
    kCoverageAttribute_GPFlag       = 0x10,
    kCoverageAttributeTweak_GPFlag  = 0x20,
>>>>>>> upstream-releases
};

static constexpr int kNumVec2sPerBone = 3; // Our bone matrices are 3x2 matrices passed in as
                                           // vec2s in column major order, and thus there are 3
                                           // vec2s per bone.

class DefaultGeoProc : public GrGeometryProcessor {
public:
<<<<<<< HEAD
    static sk_sp<GrGeometryProcessor> Make(const GrShaderCaps* shaderCaps,
                                           uint32_t gpTypeFlags,
                                           GrColor color,
||||||| merged common ancestors
    static sk_sp<GrGeometryProcessor> Make(uint32_t gpTypeFlags,
                                           GrColor color,
=======
    static sk_sp<GrGeometryProcessor> Make(const GrShaderCaps* shaderCaps,
                                           uint32_t gpTypeFlags,
                                           const SkPMColor4f& color,
>>>>>>> upstream-releases
                                           sk_sp<GrColorSpaceXform> colorSpaceXform,
                                           const SkMatrix& viewMatrix,
                                           const SkMatrix& localMatrix,
                                           bool localCoordsWillBeRead,
                                           uint8_t coverage,
                                           const float* bones,
                                           int boneCount) {
        return sk_sp<GrGeometryProcessor>(new DefaultGeoProc(
<<<<<<< HEAD
                shaderCaps, gpTypeFlags, color, std::move(colorSpaceXform), viewMatrix, localMatrix,
                coverage, localCoordsWillBeRead, bones, boneCount));
||||||| merged common ancestors
                gpTypeFlags, color, std::move(colorSpaceXform), viewMatrix, localMatrix, coverage,
                localCoordsWillBeRead));
=======
                shaderCaps, gpTypeFlags, color, std::move(colorSpaceXform), viewMatrix, localMatrix,
                coverage, localCoordsWillBeRead));
>>>>>>> upstream-releases
    }

    const char* name() const override { return "DefaultGeometryProcessor"; }

<<<<<<< HEAD
    GrColor color() const { return fColor; }
    bool hasVertexColor() const { return fInColor.isInitialized(); }
||||||| merged common ancestors
    const Attribute* inPosition() const { return fInPosition; }
    const Attribute* inColor() const { return fInColor; }
    const Attribute* inLocalCoords() const { return fInLocalCoords; }
    const Attribute* inCoverage() const { return fInCoverage; }
    GrColor color() const { return fColor; }
    bool hasVertexColor() const { return SkToBool(fInColor); }
=======
    const SkPMColor4f& color() const { return fColor; }
    bool hasVertexColor() const { return fInColor.isInitialized(); }
>>>>>>> upstream-releases
    const SkMatrix& viewMatrix() const { return fViewMatrix; }
    const SkMatrix& localMatrix() const { return fLocalMatrix; }
    bool localCoordsWillBeRead() const { return fLocalCoordsWillBeRead; }
    uint8_t coverage() const { return fCoverage; }
<<<<<<< HEAD
    bool hasVertexCoverage() const { return fInCoverage.isInitialized(); }
    const float* bones() const { return fBones; }
    int boneCount() const { return fBoneCount; }
    bool hasBones() const { return SkToBool(fBones); }
||||||| merged common ancestors
    bool hasVertexCoverage() const { return SkToBool(fInCoverage); }
    bool linearizeColor() const {
        // Linearization should only happen with SkColor
        bool linearize = SkToBool(fFlags & kLinearizeColorAttribute_GPFlag);
        SkASSERT(!linearize || (fFlags & kColorAttributeIsSkColor_GPFlag));
        return linearize;
    }
=======
    bool hasVertexCoverage() const { return fInCoverage.isInitialized(); }
>>>>>>> upstream-releases

    class GLSLProcessor : public GrGLSLGeometryProcessor {
    public:
        GLSLProcessor()
            : fViewMatrix(SkMatrix::InvalidMatrix())
            , fColor(SK_PMColor4fILLEGAL)
            , fCoverage(0xff) {}

        void onEmitCode(EmitArgs& args, GrGPArgs* gpArgs) override {
            const DefaultGeoProc& gp = args.fGP.cast<DefaultGeoProc>();
            GrGLSLVertexBuilder* vertBuilder = args.fVertBuilder;
            GrGLSLFPFragmentBuilder* fragBuilder = args.fFragBuilder;
            GrGLSLVaryingHandler* varyingHandler = args.fVaryingHandler;
            GrGLSLUniformHandler* uniformHandler = args.fUniformHandler;

            // emit attributes
            varyingHandler->emitAttributes(gp);

            bool tweakAlpha = SkToBool(gp.fFlags & kCoverageAttributeTweak_GPFlag);
            SkASSERT(!tweakAlpha || gp.hasVertexCoverage());

            // Setup pass through color
            if (gp.hasVertexColor() || tweakAlpha) {
                GrGLSLVarying varying(kHalf4_GrSLType);
                varyingHandler->addVarying("color", &varying);

<<<<<<< HEAD
                // There are several optional steps to process the color. Start with the attribute:
                vertBuilder->codeAppendf("half4 color = %s;", gp.fInColor.name());
||||||| merged common ancestors
                // There are several optional steps to process the color. Start with the attribute:
                vertBuilder->codeAppendf("half4 color = %s;", gp.inColor()->fName);

                // Linearize
                if (gp.linearizeColor()) {
                    SkString srgbFuncName;
                    static const GrShaderVar gSrgbArgs[] = {
                        GrShaderVar("x", kHalf_GrSLType),
                    };
                    vertBuilder->emitFunction(kHalf_GrSLType,
                                              "srgb_to_linear",
                                              SK_ARRAY_COUNT(gSrgbArgs),
                                              gSrgbArgs,
                                              "return (x <= 0.04045) ? (x / 12.92) "
                                              ": pow((x + 0.055) / 1.055, 2.4);",
                                              &srgbFuncName);
                    vertBuilder->codeAppendf("color = half4(%s(%s.r), %s(%s.g), %s(%s.b), %s.a);",
                                             srgbFuncName.c_str(), gp.inColor()->fName,
                                             srgbFuncName.c_str(), gp.inColor()->fName,
                                             srgbFuncName.c_str(), gp.inColor()->fName,
                                             gp.inColor()->fName);
                }
=======
                // There are several optional steps to process the color. Start with the attribute,
                // or with uniform color (in the case of folding coverage into a uniform color):
                if (gp.hasVertexColor()) {
                    vertBuilder->codeAppendf("half4 color = %s;", gp.fInColor.name());
                } else {
                    const char* colorUniformName;
                    fColorUniform = uniformHandler->addUniform(kVertex_GrShaderFlag,
                                                               kHalf4_GrSLType,
                                                               "Color",
                                                               &colorUniformName);
                    vertBuilder->codeAppendf("half4 color = %s;", colorUniformName);
                }
>>>>>>> upstream-releases

                // For SkColor, do a red/blue swap, possible color space conversion, and premul
                if (gp.fFlags & kColorAttributeIsSkColor_GPFlag) {
                    vertBuilder->codeAppend("color = color.bgra;");

                    if (gp.fColorSpaceXform) {
                        fColorSpaceHelper.emitCode(uniformHandler, gp.fColorSpaceXform.get(),
                                                   kVertex_GrShaderFlag);
                        SkString xformedColor;
                        vertBuilder->appendColorGamutXform(&xformedColor, "color",
                                                           &fColorSpaceHelper);
                        vertBuilder->codeAppendf("color = %s;", xformedColor.c_str());
                    }
<<<<<<< HEAD

                    vertBuilder->codeAppend("color = half4(color.rgb * color.a, color.a);");
||||||| merged common ancestors
=======

                    vertBuilder->codeAppend("color = half4(color.rgb * color.a, color.a);");
                }

                // Optionally fold coverage into alpha (color).
                if (tweakAlpha) {
                    vertBuilder->codeAppendf("color = color * %s;", gp.fInCoverage.name());
>>>>>>> upstream-releases
                }

                vertBuilder->codeAppendf("%s = color;\n", varying.vsOut());
                fragBuilder->codeAppendf("%s = %s;", args.fOutputColor, varying.fsIn());
            } else {
                this->setupUniformColor(fragBuilder, uniformHandler, args.fOutputColor,
                                        &fColorUniform);
            }

            // Setup bone transforms
            // NOTE: This code path is currently unused. Benchmarks have found that for all
            // reasonable cases of skinned vertices, the overhead involved in copying and uploading
            // bone data makes performing the transformations on the CPU faster than doing so on
            // the GPU. This is being kept here in case that changes.
            const char* transformedPositionName = gp.fInPosition.name();
            if (gp.hasBones()) {
                // Set up the uniform for the bones.
                const char* vertBonesUniformName;
                fBonesUniform = uniformHandler->addUniformArray(kVertex_GrShaderFlag,
                                                                kFloat2_GrSLType,
                                                                "Bones",
                                                                kMaxBones * kNumVec2sPerBone,
                                                                &vertBonesUniformName);

                // Set up the bone application function.
                SkString applyBoneFunctionName;
                this->emitApplyBoneFunction(vertBuilder,
                                            vertBonesUniformName,
                                            &applyBoneFunctionName);

                // Apply the world transform to the position first.
                vertBuilder->codeAppendf(
                        "float2 worldPosition = %s(0, %s);"
                        "float2 transformedPosition = float2(0, 0);"
                        "for (int i = 0; i < 4; i++) {",
                        applyBoneFunctionName.c_str(),
                        gp.fInPosition.name());

                // If the GPU supports unsigned integers, then we can read the index. Otherwise,
                // we have to estimate it given the float representation.
                if (args.fShaderCaps->unsignedSupport()) {
                    vertBuilder->codeAppendf(
                        "    byte index = %s[i];",
                        gp.fInBoneIndices.name());
                } else {
                    vertBuilder->codeAppendf(
                        "    byte index = byte(floor(%s[i] * 255 + 0.5));",
                        gp.fInBoneIndices.name());
                }

                // Get the weight and apply the transformation.
                vertBuilder->codeAppendf(
                        "    float weight = %s[i];"
                        "    transformedPosition += %s(index, worldPosition) * weight;"
                        "}",
                        gp.fInBoneWeights.name(),
                        applyBoneFunctionName.c_str());
                transformedPositionName = "transformedPosition";
            }

            // Setup position
            this->writeOutputPosition(vertBuilder,
                                      uniformHandler,
                                      gpArgs,
<<<<<<< HEAD
                                      transformedPositionName,
||||||| merged common ancestors
                                      gp.inPosition()->fName,
=======
                                      gp.fInPosition.name(),
>>>>>>> upstream-releases
                                      gp.viewMatrix(),
                                      &fViewMatrixUniform);

            if (gp.fInLocalCoords.isInitialized()) {
                // emit transforms with explicit local coords
                this->emitTransforms(vertBuilder,
                                     varyingHandler,
                                     uniformHandler,
                                     gp.fInLocalCoords.asShaderVar(),
                                     gp.localMatrix(),
                                     args.fFPCoordTransformHandler);
            } else {
                // emit transforms with position
                this->emitTransforms(vertBuilder,
                                     varyingHandler,
                                     uniformHandler,
                                     gp.fInPosition.asShaderVar(),
                                     gp.localMatrix(),
                                     args.fFPCoordTransformHandler);
            }

            // Setup coverage as pass through
            if (gp.hasVertexCoverage() && !tweakAlpha) {
                fragBuilder->codeAppendf("half alpha = 1.0;");
                varyingHandler->addPassThroughAttribute(gp.fInCoverage, "alpha");
                fragBuilder->codeAppendf("%s = half4(alpha);", args.fOutputCoverage);
            } else if (gp.coverage() == 0xff) {
                fragBuilder->codeAppendf("%s = half4(1);", args.fOutputCoverage);
            } else {
                const char* fragCoverage;
                fCoverageUniform = uniformHandler->addUniform(kFragment_GrShaderFlag,
                                                              kHalf_GrSLType,
                                                              "Coverage",
                                                              &fragCoverage);
                fragBuilder->codeAppendf("%s = half4(%s);", args.fOutputCoverage, fragCoverage);
            }
        }

        static inline void GenKey(const GrGeometryProcessor& gp,
                                  const GrShaderCaps&,
                                  GrProcessorKeyBuilder* b) {
            const DefaultGeoProc& def = gp.cast<DefaultGeoProc>();
            uint32_t key = def.fFlags;
<<<<<<< HEAD
            key |= (def.coverage() == 0xff) ? 0x20 : 0;
            key |= (def.localCoordsWillBeRead() && def.localMatrix().hasPerspective()) ? 0x40 : 0x0;
||||||| merged common ancestors
            key |= (def.coverage() == 0xff) ? 0x10 : 0;
            key |= (def.localCoordsWillBeRead() && def.localMatrix().hasPerspective()) ? 0x20 : 0x0;
=======
            key |= (def.coverage() == 0xff) ? 0x80 : 0;
            key |= (def.localCoordsWillBeRead() && def.localMatrix().hasPerspective()) ? 0x100 : 0;
>>>>>>> upstream-releases
            key |= ComputePosKey(def.viewMatrix()) << 20;
            b->add32(key);
            b->add32(GrColorSpaceXform::XformKey(def.fColorSpaceXform.get()));
        }

        void setData(const GrGLSLProgramDataManager& pdman,
                     const GrPrimitiveProcessor& gp,
                     FPCoordTransformIter&& transformIter) override {
            const DefaultGeoProc& dgp = gp.cast<DefaultGeoProc>();

            if (!dgp.viewMatrix().isIdentity() && !fViewMatrix.cheapEqualTo(dgp.viewMatrix())) {
                fViewMatrix = dgp.viewMatrix();
                float viewMatrix[3 * 3];
                GrGLSLGetMatrix<3>(viewMatrix, fViewMatrix);
                pdman.setMatrix3f(fViewMatrixUniform, viewMatrix);
            }

            if (!dgp.hasVertexColor() && dgp.color() != fColor) {
                pdman.set4fv(fColorUniform, 1, dgp.color().vec());
                fColor = dgp.color();
            }

            if (dgp.coverage() != fCoverage && !dgp.hasVertexCoverage()) {
                pdman.set1f(fCoverageUniform, GrNormalizeByteToFloat(dgp.coverage()));
                fCoverage = dgp.coverage();
            }
            this->setTransformDataHelper(dgp.fLocalMatrix, pdman, &transformIter);

<<<<<<< HEAD
            fColorSpaceHelper.setData(pdman, dgp.fColorSpaceXform.get());

            if (dgp.hasBones()) {
                pdman.set2fv(fBonesUniform, dgp.boneCount() * kNumVec2sPerBone, dgp.bones());
            }
||||||| merged common ancestors
            if (dgp.linearizeColor() && dgp.fColorSpaceXform) {
                fColorSpaceHelper.setData(pdman, dgp.fColorSpaceXform.get());
            }
=======
            fColorSpaceHelper.setData(pdman, dgp.fColorSpaceXform.get());
>>>>>>> upstream-releases
        }

    private:
        void emitApplyBoneFunction(GrGLSLVertexBuilder* vertBuilder,
                                   const char* vertBonesUniformName,
                                   SkString* funcName) {
                // The bone matrices are passed in as 3x2 matrices in column-major order as groups
                // of 3 float2s. This code takes those float2s and performs the matrix operation on
                // a given matrix and float2.
                const GrShaderVar gApplyBoneArgs[] = {
                    GrShaderVar("index", kByte_GrSLType),
                    GrShaderVar("vec", kFloat2_GrSLType),
                };
                SkString body;
                body.appendf(
                    "    float2 c0 = %s[index * 3];"
                    "    float2 c1 = %s[index * 3 + 1];"
                    "    float2 c2 = %s[index * 3 + 2];"
                    "    float x = c0.x * vec.x + c1.x * vec.y + c2.x;"
                    "    float y = c0.y * vec.x + c1.y * vec.y + c2.y;"
                    "    return float2(x, y);",
                    vertBonesUniformName,
                    vertBonesUniformName,
                    vertBonesUniformName);
                vertBuilder->emitFunction(kFloat2_GrSLType,
                                          "applyBone",
                                          SK_ARRAY_COUNT(gApplyBoneArgs),
                                          gApplyBoneArgs,
                                          body.c_str(),
                                          funcName);
        }

    private:
        SkMatrix fViewMatrix;
        SkPMColor4f fColor;
        uint8_t fCoverage;
        UniformHandle fViewMatrixUniform;
        UniformHandle fColorUniform;
        UniformHandle fCoverageUniform;
        UniformHandle fBonesUniform;
        GrGLSLColorSpaceXformHelper fColorSpaceHelper;

        typedef GrGLSLGeometryProcessor INHERITED;
    };

    void getGLSLProcessorKey(const GrShaderCaps& caps, GrProcessorKeyBuilder* b) const override {
        GLSLProcessor::GenKey(*this, caps, b);
    }

    GrGLSLPrimitiveProcessor* createGLSLInstance(const GrShaderCaps&) const override {
        return new GLSLProcessor();
    }

private:
<<<<<<< HEAD
    DefaultGeoProc(const GrShaderCaps* shaderCaps,
                   uint32_t gpTypeFlags,
                   GrColor color,
||||||| merged common ancestors
    DefaultGeoProc(uint32_t gpTypeFlags,
                   GrColor color,
=======
    DefaultGeoProc(const GrShaderCaps* shaderCaps,
                   uint32_t gpTypeFlags,
                   const SkPMColor4f& color,
>>>>>>> upstream-releases
                   sk_sp<GrColorSpaceXform> colorSpaceXform,
                   const SkMatrix& viewMatrix,
                   const SkMatrix& localMatrix,
                   uint8_t coverage,
                   bool localCoordsWillBeRead,
                   const float* bones,
                   int boneCount)
            : INHERITED(kDefaultGeoProc_ClassID)
            , fColor(color)
            , fViewMatrix(viewMatrix)
            , fLocalMatrix(localMatrix)
            , fCoverage(coverage)
            , fFlags(gpTypeFlags)
            , fLocalCoordsWillBeRead(localCoordsWillBeRead)
<<<<<<< HEAD
            , fColorSpaceXform(std::move(colorSpaceXform))
            , fBones(bones)
            , fBoneCount(boneCount) {
        fInPosition = {"inPosition", kFloat2_GrVertexAttribType, kFloat2_GrSLType};
        int cnt = 1;
||||||| merged common ancestors
            , fColorSpaceXform(std::move(colorSpaceXform)) {
        fInPosition = &this->addVertexAttrib("inPosition", kFloat2_GrVertexAttribType);
=======
            , fColorSpaceXform(std::move(colorSpaceXform)) {
        fInPosition = {"inPosition", kFloat2_GrVertexAttribType, kFloat2_GrSLType};
>>>>>>> upstream-releases
        if (fFlags & kColorAttribute_GPFlag) {
<<<<<<< HEAD
            fInColor = {"inColor", kUByte4_norm_GrVertexAttribType, kHalf4_GrSLType};
            ++cnt;
||||||| merged common ancestors
            fInColor = &this->addVertexAttrib("inColor", kUByte4_norm_GrVertexAttribType);
=======
            fInColor = MakeColorAttribute("inColor",
                                          SkToBool(fFlags & kColorAttributeIsWide_GPFlag));
>>>>>>> upstream-releases
        }
        if (fFlags & kLocalCoordAttribute_GPFlag) {
<<<<<<< HEAD
            fInLocalCoords = {"inLocalCoord", kFloat2_GrVertexAttribType,
                                              kFloat2_GrSLType};
            ++cnt;
||||||| merged common ancestors
            fInLocalCoords = &this->addVertexAttrib("inLocalCoord", kFloat2_GrVertexAttribType);
            this->setHasExplicitLocalCoords();
=======
            fInLocalCoords = {"inLocalCoord", kFloat2_GrVertexAttribType,
                                              kFloat2_GrSLType};
>>>>>>> upstream-releases
        }
        if (fFlags & kCoverageAttribute_GPFlag) {
<<<<<<< HEAD
            fInCoverage = {"inCoverage", kFloat_GrVertexAttribType, kHalf_GrSLType};
            ++cnt;
        }
        if (fFlags & kBonesAttribute_GPFlag) {
            SkASSERT(bones && (boneCount > 0));
            // GLSL 1.10 and 1.20 don't support integer attributes.
            GrVertexAttribType indicesCPUType = kByte4_GrVertexAttribType;
            GrSLType indicesGPUType = kByte4_GrSLType;
            if (!shaderCaps->unsignedSupport()) {
                indicesCPUType = kUByte4_norm_GrVertexAttribType;
                indicesGPUType = kHalf4_GrSLType;
            }
            fInBoneIndices = {"inBoneIndices", indicesCPUType, indicesGPUType};
            ++cnt;
            fInBoneWeights = {"inBoneWeights", kUByte4_norm_GrVertexAttribType,
                                               kHalf4_GrSLType};
            ++cnt;
||||||| merged common ancestors
            fInCoverage = &this->addVertexAttrib("inCoverage", kHalf_GrVertexAttribType);
=======
            fInCoverage = {"inCoverage", kFloat_GrVertexAttribType, kHalf_GrSLType};
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        this->setVertexAttributeCnt(cnt);
||||||| merged common ancestors
=======
        this->setVertexAttributes(&fInPosition, 4);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    const Attribute& onVertexAttribute(int i) const override {
        return IthInitializedAttribute(i,
                                       fInPosition,
                                       fInColor,
                                       fInLocalCoords,
                                       fInCoverage,
                                       fInBoneIndices,
                                       fInBoneWeights);
    }

    Attribute fInPosition;
    Attribute fInColor;
    Attribute fInLocalCoords;
    Attribute fInCoverage;
    Attribute fInBoneIndices;
    Attribute fInBoneWeights;
    GrColor fColor;
||||||| merged common ancestors
    const Attribute* fInPosition = nullptr;
    const Attribute* fInColor = nullptr;
    const Attribute* fInLocalCoords = nullptr;
    const Attribute* fInCoverage = nullptr;
    GrColor fColor;
=======
    Attribute fInPosition;
    Attribute fInColor;
    Attribute fInLocalCoords;
    Attribute fInCoverage;
    SkPMColor4f fColor;
>>>>>>> upstream-releases
    SkMatrix fViewMatrix;
    SkMatrix fLocalMatrix;
    uint8_t fCoverage;
    uint32_t fFlags;
    bool fLocalCoordsWillBeRead;
    sk_sp<GrColorSpaceXform> fColorSpaceXform;
    const float* fBones;
    int fBoneCount;

    GR_DECLARE_GEOMETRY_PROCESSOR_TEST

    typedef GrGeometryProcessor INHERITED;
};

GR_DEFINE_GEOMETRY_PROCESSOR_TEST(DefaultGeoProc);

#if GR_TEST_UTILS
static constexpr int kNumFloatsPerBone = 6;
static constexpr int kTestBoneCount = 4;
static constexpr float kTestBones[kTestBoneCount * kNumFloatsPerBone] = {
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
};

sk_sp<GrGeometryProcessor> DefaultGeoProc::TestCreate(GrProcessorTestData* d) {
    uint32_t flags = 0;
    if (d->fRandom->nextBool()) {
        flags |= kColorAttribute_GPFlag;
    }
    if (d->fRandom->nextBool()) {
        flags |= kColorAttributeIsSkColor_GPFlag;
    }
    if (d->fRandom->nextBool()) {
        flags |= kColorAttributeIsWide_GPFlag;
    }
    if (d->fRandom->nextBool()) {
        flags |= kCoverageAttribute_GPFlag;
        if (d->fRandom->nextBool()) {
            flags |= kCoverageAttributeTweak_GPFlag;
        }
    }
    if (d->fRandom->nextBool()) {
        flags |= kLocalCoordAttribute_GPFlag;
    }
    if (d->fRandom->nextBool()) {
        flags |= kBonesAttribute_GPFlag;
    }

<<<<<<< HEAD
    return DefaultGeoProc::Make(d->caps()->shaderCaps(),
                                flags,
                                GrRandomColor(d->fRandom),
||||||| merged common ancestors
    return DefaultGeoProc::Make(flags,
                                GrRandomColor(d->fRandom),
=======
    return DefaultGeoProc::Make(d->caps()->shaderCaps(),
                                flags,
                                SkPMColor4f::FromBytes_RGBA(GrRandomColor(d->fRandom)),
>>>>>>> upstream-releases
                                GrTest::TestColorXform(d->fRandom),
                                GrTest::TestMatrix(d->fRandom),
                                GrTest::TestMatrix(d->fRandom),
                                d->fRandom->nextBool(),
                                GrRandomCoverage(d->fRandom),
                                kTestBones,
                                kTestBoneCount);
}
#endif

sk_sp<GrGeometryProcessor> GrDefaultGeoProcFactory::Make(const GrShaderCaps* shaderCaps,
                                                         const Color& color,
                                                         const Coverage& coverage,
                                                         const LocalCoords& localCoords,
                                                         const SkMatrix& viewMatrix) {
    uint32_t flags = 0;
    if (Color::kPremulGrColorAttribute_Type == color.fType) {
        flags |= kColorAttribute_GPFlag;
    } else if (Color::kUnpremulSkColorAttribute_Type == color.fType) {
        flags |= kColorAttribute_GPFlag | kColorAttributeIsSkColor_GPFlag;
    } else if (Color::kPremulWideColorAttribute_Type == color.fType) {
        flags |= kColorAttribute_GPFlag | kColorAttributeIsWide_GPFlag;
    }
<<<<<<< HEAD
    flags |= coverage.fType == Coverage::kAttribute_Type ? kCoverageAttribute_GPFlag : 0;
||||||| merged common ancestors
    if (color.fLinearize) {
        // It only makes sense to linearize SkColors (which are always sRGB). GrColor values should
        // have been linearized and gamut-converted during paint conversion
        SkASSERT(Color::kUnpremulSkColorAttribute_Type == color.fType);
        flags |= kLinearizeColorAttribute_GPFlag;
    }
    flags |= coverage.fType == Coverage::kAttribute_Type ? kCoverageAttribute_GPFlag : 0;
=======
    if (Coverage::kAttribute_Type == coverage.fType) {
        flags |= kCoverageAttribute_GPFlag;
    } else if (Coverage::kAttributeTweakAlpha_Type == coverage.fType) {
        flags |= kCoverageAttribute_GPFlag | kCoverageAttributeTweak_GPFlag;
    }
>>>>>>> upstream-releases
    flags |= localCoords.fType == LocalCoords::kHasExplicit_Type ? kLocalCoordAttribute_GPFlag : 0;

    uint8_t inCoverage = coverage.fCoverage;
    bool localCoordsWillBeRead = localCoords.fType != LocalCoords::kUnused_Type;

<<<<<<< HEAD
    GrColor inColor = color.fColor;
    return DefaultGeoProc::Make(shaderCaps,
                                flags,
                                inColor,
||||||| merged common ancestors
    GrColor inColor = color.fColor;
    return DefaultGeoProc::Make(flags,
                                inColor,
=======
    return DefaultGeoProc::Make(shaderCaps,
                                flags,
                                color.fColor,
>>>>>>> upstream-releases
                                color.fColorSpaceXform,
                                viewMatrix,
                                localCoords.fMatrix ? *localCoords.fMatrix : SkMatrix::I(),
                                localCoordsWillBeRead,
                                inCoverage,
                                nullptr,
                                0);
}

sk_sp<GrGeometryProcessor> GrDefaultGeoProcFactory::MakeForDeviceSpace(
                                                                     const GrShaderCaps* shaderCaps,
                                                                     const Color& color,
                                                                     const Coverage& coverage,
                                                                     const LocalCoords& localCoords,
                                                                     const SkMatrix& viewMatrix) {
    SkMatrix invert = SkMatrix::I();
    if (LocalCoords::kUnused_Type != localCoords.fType) {
        SkASSERT(LocalCoords::kUsePosition_Type == localCoords.fType);
        if (!viewMatrix.isIdentity() && !viewMatrix.invert(&invert)) {
            return nullptr;
        }

        if (localCoords.hasLocalMatrix()) {
            invert.postConcat(*localCoords.fMatrix);
        }
    }

    LocalCoords inverted(LocalCoords::kUsePosition_Type, &invert);
<<<<<<< HEAD
    return Make(shaderCaps, color, coverage, inverted, SkMatrix::I());
}

sk_sp<GrGeometryProcessor> GrDefaultGeoProcFactory::MakeWithBones(const GrShaderCaps* shaderCaps,
                                                                  const Color& color,
                                                                  const Coverage& coverage,
                                                                  const LocalCoords& localCoords,
                                                                  const Bones& bones,
                                                                  const SkMatrix& viewMatrix) {
    uint32_t flags = 0;
    if (Color::kPremulGrColorAttribute_Type == color.fType) {
        flags |= kColorAttribute_GPFlag;
    } else if (Color::kUnpremulSkColorAttribute_Type == color.fType) {
        flags |= kColorAttribute_GPFlag | kColorAttributeIsSkColor_GPFlag;
    }
    flags |= coverage.fType == Coverage::kAttribute_Type ? kCoverageAttribute_GPFlag : 0;
    flags |= localCoords.fType == LocalCoords::kHasExplicit_Type ? kLocalCoordAttribute_GPFlag : 0;
    flags |= kBonesAttribute_GPFlag;

    uint8_t inCoverage = coverage.fCoverage;
    bool localCoordsWillBeRead = localCoords.fType != LocalCoords::kUnused_Type;

    GrColor inColor = color.fColor;
    return DefaultGeoProc::Make(shaderCaps,
                                flags,
                                inColor,
                                color.fColorSpaceXform,
                                viewMatrix,
                                localCoords.fMatrix ? *localCoords.fMatrix : SkMatrix::I(),
                                localCoordsWillBeRead,
                                inCoverage,
                                bones.fBones,
                                bones.fBoneCount);
||||||| merged common ancestors
    return Make(color, coverage, inverted, SkMatrix::I());
=======
    return Make(shaderCaps, color, coverage, inverted, SkMatrix::I());
>>>>>>> upstream-releases
}
