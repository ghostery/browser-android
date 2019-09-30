/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrYUVtoRGBEffect.h"

static const float kJPEGConversionMatrix[16] = {
    1.0f,  0.0f,       1.402f,    -0.703749f,
    1.0f, -0.344136f, -0.714136f,  0.531211f,
    1.0f,  1.772f,     0.0f,      -0.889475f,
    0.0f,  0.0f,       0.0f,       1.0
};

static const float kRec601ConversionMatrix[16] = {
    1.164f,  0.0f,    1.596f, -0.87075f,
    1.164f, -0.391f, -0.813f,  0.52925f,
    1.164f,  2.018f,  0.0f,   -1.08175f,
    0.0f,    0.0f,    0.0f,    1.0
};

static const float kRec709ConversionMatrix[16] = {
<<<<<<< HEAD
    1.164f,  0.0f,    1.793f, -0.96925f,
    1.164f, -0.213f, -0.533f,  0.30025f,
    1.164f,  2.112f,  0.0f,   -1.12875f,
    0.0f,    0.0f,    0.0f,    1.0f
};

std::unique_ptr<GrFragmentProcessor> GrYUVtoRGBEffect::Make(const sk_sp<GrTextureProxy> proxies[],
                                                            const SkYUVAIndex yuvaIndices[4],
                                                            SkYUVColorSpace yuvColorSpace) {
    int numPlanes;
    SkAssertResult(SkYUVAIndex::AreValidIndices(yuvaIndices, &numPlanes));

    const SkISize YSize = proxies[yuvaIndices[SkYUVAIndex::kY_Index].fIndex]->isize();

    GrSamplerState::Filter filterModes[4];
    SkSize scales[4];
    for (int i = 0; i < numPlanes; ++i) {
        SkISize size = proxies[i]->isize();
        scales[i] = SkSize::Make(SkIntToScalar(size.width()) / SkIntToScalar(YSize.width()),
                                 SkIntToScalar(size.height()) / SkIntToScalar(YSize.height()));
        filterModes[i] = (size == YSize) ? GrSamplerState::Filter::kNearest
                                         : GrSamplerState::Filter::kBilerp;
    }

    SkMatrix44 mat;
    switch (yuvColorSpace) {
||||||| merged common ancestors
        1.164f, 0.0f,   1.793f, -0.96925f, 1.164f, -0.213f, -0.533f, 0.30025f,
        1.164f, 2.112f, 0.0f,   -1.12875f, 0.0f,   0.0f,    0.0f,    1.0f};

std::unique_ptr<GrFragmentProcessor> GrYUVtoRGBEffect::Make(sk_sp<GrTextureProxy> yProxy,
                                                            sk_sp<GrTextureProxy>
                                                                    uProxy,
                                                            sk_sp<GrTextureProxy>
                                                                    vProxy,
                                                            const SkISize sizes[3],
                                                            SkYUVColorSpace colorSpace,
                                                            bool nv12) {
    SkScalar w[3], h[3];
    w[0] = SkIntToScalar(sizes[0].fWidth);
    h[0] = SkIntToScalar(sizes[0].fHeight);
    w[1] = SkIntToScalar(sizes[1].fWidth);
    h[1] = SkIntToScalar(sizes[1].fHeight);
    w[2] = SkIntToScalar(sizes[2].fWidth);
    h[2] = SkIntToScalar(sizes[2].fHeight);
    SkMatrix yTransform = SkMatrix::I();
    SkMatrix uTransform = SkMatrix::MakeScale(w[1] / w[0], h[1] / h[0]);
    SkMatrix vTransform = SkMatrix::MakeScale(w[2] / w[0], h[2] / h[0]);
    GrSamplerState::Filter uvFilterMode =
            ((sizes[1].fWidth != sizes[0].fWidth) || (sizes[1].fHeight != sizes[0].fHeight) ||
             (sizes[2].fWidth != sizes[0].fWidth) || (sizes[2].fHeight != sizes[0].fHeight))
                    ? GrSamplerState::Filter::kBilerp
                    : GrSamplerState::Filter::kNearest;
    SkMatrix44 mat(SkMatrix44::kUninitialized_Constructor);
    switch (colorSpace) {
=======
    1.164f,  0.0f,    1.793f, -0.96925f,
    1.164f, -0.213f, -0.533f,  0.30025f,
    1.164f,  2.112f,  0.0f,   -1.12875f,
    0.0f,    0.0f,    0.0f,    1.0f
};

std::unique_ptr<GrFragmentProcessor> GrYUVtoRGBEffect::Make(const sk_sp<GrTextureProxy> proxies[],
                                                            const SkYUVAIndex yuvaIndices[4],
                                                            SkYUVColorSpace yuvColorSpace,
                                                            GrSamplerState::Filter filterMode) {
    int numPlanes;
    SkAssertResult(SkYUVAIndex::AreValidIndices(yuvaIndices, &numPlanes));

    const SkISize YSize = proxies[yuvaIndices[SkYUVAIndex::kY_Index].fIndex]->isize();

    GrSamplerState::Filter minimizeFilterMode = GrSamplerState::Filter::kMipMap == filterMode ?
                                                GrSamplerState::Filter::kMipMap :
                                                GrSamplerState::Filter::kBilerp;

    GrSamplerState::Filter filterModes[4];
    SkSize scales[4];
    for (int i = 0; i < numPlanes; ++i) {
        SkISize size = proxies[i]->isize();
        scales[i] = SkSize::Make(SkIntToScalar(size.width()) / SkIntToScalar(YSize.width()),
                                 SkIntToScalar(size.height()) / SkIntToScalar(YSize.height()));
        filterModes[i] = (size == YSize) ? filterMode : minimizeFilterMode;
    }

    SkMatrix44 mat;
    switch (yuvColorSpace) {
>>>>>>> upstream-releases
        case kJPEG_SkYUVColorSpace:
            mat.setColMajorf(kJPEGConversionMatrix);
            break;
        case kRec601_SkYUVColorSpace:
            mat.setColMajorf(kRec601ConversionMatrix);
            break;
        case kRec709_SkYUVColorSpace:
            mat.setColMajorf(kRec709ConversionMatrix);
            break;
    }
    return std::unique_ptr<GrFragmentProcessor>(new GrYUVtoRGBEffect(
<<<<<<< HEAD
            proxies, scales, filterModes, numPlanes, yuvaIndices, mat));
}

SkString GrYUVtoRGBEffect::dumpInfo() const {
    SkString str;
    for (int i = 0; i < this->numTextureSamplers(); ++i) {
        str.appendf("%d: %d %d ", i,
                    this->textureSampler(i).proxy()->uniqueID().asUInt(),
                    this->textureSampler(i).proxy()->underlyingUniqueID().asUInt());
    }
    str.appendf("\n");

    return str;
||||||| merged common ancestors
            std::move(yProxy), yTransform, std::move(uProxy), uTransform, std::move(vProxy),
            vTransform, mat, nv12, GrSamplerState(GrSamplerState::WrapMode::kClamp, uvFilterMode)));
=======
            proxies, scales, filterModes, numPlanes, yuvaIndices, mat));
}

#ifdef SK_DEBUG
SkString GrYUVtoRGBEffect::dumpInfo() const {
    SkString str;
    for (int i = 0; i < this->numTextureSamplers(); ++i) {
        str.appendf("%d: %d %d ", i,
                    this->textureSampler(i).proxy()->uniqueID().asUInt(),
                    this->textureSampler(i).proxy()->underlyingUniqueID().asUInt());
    }
    str.appendf("\n");

    return str;
>>>>>>> upstream-releases
}
#endif

#include "glsl/GrGLSLFragmentProcessor.h"
#include "glsl/GrGLSLFragmentShaderBuilder.h"
#include "glsl/GrGLSLProgramBuilder.h"
#include "GrTexture.h"
#include "SkSLCPP.h"
#include "SkSLUtil.h"
class GrGLSLYUVtoRGBEffect : public GrGLSLFragmentProcessor {
public:
    GrGLSLYUVtoRGBEffect() {}
    void emitCode(EmitArgs& args) override {
        GrGLSLFPFragmentBuilder* fragBuilder = args.fFragBuilder;
        const GrYUVtoRGBEffect& _outer = args.fFp.cast<GrYUVtoRGBEffect>();
        (void)_outer;

        auto colorSpaceMatrix = _outer.colorSpaceMatrix();
        (void)colorSpaceMatrix;
        fColorSpaceMatrixVar =
                args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kHalf4x4_GrSLType,
                                                 kDefault_GrSLPrecision, "colorSpaceMatrix");

        int numSamplers = args.fTexSamplers.count();

        SkString coords[4];
        for (int i = 0; i < numSamplers; ++i) {
            coords[i] = fragBuilder->ensureCoords2D(args.fTransformedCoords[i]);
        }

        for (int i = 0; i < numSamplers; ++i) {
            fragBuilder->codeAppendf(
                "half4 tmp%d = texture(%s, %s).%s;",
                    i,
                    fragBuilder->getProgramBuilder()->samplerVariable(args.fTexSamplers[i]).c_str(),
                    coords[i].c_str(),
                    fragBuilder->getProgramBuilder()->samplerSwizzle(args.fTexSamplers[i]).c_str());
        }

        static const char kChannelToChar[4] = { 'x', 'y', 'z', 'w' };

        fragBuilder->codeAppendf(
<<<<<<< HEAD
            "half4 yuvOne = half4(tmp%d.%c, tmp%d.%c, tmp%d.%c, 1.0) * %s;",
                _outer.yuvaIndex(0).fIndex, kChannelToChar[(int)_outer.yuvaIndex(0).fChannel],
                _outer.yuvaIndex(1).fIndex, kChannelToChar[(int)_outer.yuvaIndex(1).fChannel],
                _outer.yuvaIndex(2).fIndex, kChannelToChar[(int)_outer.yuvaIndex(2).fChannel],
||||||| merged common ancestors
                "@if (%s) {\n    %s = half4(texture(%s, %s).%s.x, texture(%s, %s).%s.xy, 1.0) * "
                "%s;\n} else {\n    %s = half4(texture(%s, %s).%s.x, texture(%s, %s).%s.x, "
                "texture(%s, %s).%s.x, 1.0) * %s;\n}\n",
                (_outer.nv12() ? "true" : "false"), args.fOutputColor,
                fragBuilder->getProgramBuilder()->samplerVariable(args.fTexSamplers[0]).c_str(),
                sk_TransformedCoords2D_0.c_str(),
                fragBuilder->getProgramBuilder()->samplerSwizzle(args.fTexSamplers[0]).c_str(),
                fragBuilder->getProgramBuilder()->samplerVariable(args.fTexSamplers[1]).c_str(),
                sk_TransformedCoords2D_1.c_str(),
                fragBuilder->getProgramBuilder()->samplerSwizzle(args.fTexSamplers[1]).c_str(),
                args.fUniformHandler->getUniformCStr(fColorSpaceMatrixVar), args.fOutputColor,
                fragBuilder->getProgramBuilder()->samplerVariable(args.fTexSamplers[0]).c_str(),
                sk_TransformedCoords2D_0.c_str(),
                fragBuilder->getProgramBuilder()->samplerSwizzle(args.fTexSamplers[0]).c_str(),
                fragBuilder->getProgramBuilder()->samplerVariable(args.fTexSamplers[1]).c_str(),
                sk_TransformedCoords2D_1.c_str(),
                fragBuilder->getProgramBuilder()->samplerSwizzle(args.fTexSamplers[1]).c_str(),
                fragBuilder->getProgramBuilder()->samplerVariable(args.fTexSamplers[2]).c_str(),
                sk_TransformedCoords2D_2.c_str(),
                fragBuilder->getProgramBuilder()->samplerSwizzle(args.fTexSamplers[2]).c_str(),
=======
            "half4 yuvOne = half4(half(tmp%d.%c), half(tmp%d.%c), half(tmp%d.%c), 1.0) * %s;",
                _outer.yuvaIndex(0).fIndex, kChannelToChar[(int)_outer.yuvaIndex(0).fChannel],
                _outer.yuvaIndex(1).fIndex, kChannelToChar[(int)_outer.yuvaIndex(1).fChannel],
                _outer.yuvaIndex(2).fIndex, kChannelToChar[(int)_outer.yuvaIndex(2).fChannel],
>>>>>>> upstream-releases
                args.fUniformHandler->getUniformCStr(fColorSpaceMatrixVar));
<<<<<<< HEAD


        if (_outer.yuvaIndex(3).fIndex >= 0) {
            fragBuilder->codeAppendf(
                "float a = tmp%d.%c;", _outer.yuvaIndex(3).fIndex,
                                       kChannelToChar[(int)_outer.yuvaIndex(3).fChannel]);
        } else {
            fragBuilder->codeAppendf("float a = 1.0;");
        }

        fragBuilder->codeAppendf("%s = half4(yuvOne.xyz, a);", args.fOutputColor);
||||||| merged common ancestors
=======


        if (_outer.yuvaIndex(3).fIndex >= 0) {
            fragBuilder->codeAppendf(
                "half a = tmp%d.%c;", _outer.yuvaIndex(3).fIndex,
                                       kChannelToChar[(int)_outer.yuvaIndex(3).fChannel]);
            // premultiply alpha
            fragBuilder->codeAppend("yuvOne *= a;");
        } else {
            fragBuilder->codeAppendf("half a = 1.0;");
        }

        fragBuilder->codeAppendf("%s = half4(yuvOne.xyz, a);", args.fOutputColor);
>>>>>>> upstream-releases
    }

private:
    void onSetData(const GrGLSLProgramDataManager& pdman,
                   const GrFragmentProcessor& _proc) override {
        const GrYUVtoRGBEffect& _outer = _proc.cast<GrYUVtoRGBEffect>();
        { pdman.setSkMatrix44(fColorSpaceMatrixVar, (_outer.colorSpaceMatrix())); }
    }
    UniformHandle fColorSpaceMatrixVar;
};
GrGLSLFragmentProcessor* GrYUVtoRGBEffect::onCreateGLSLInstance() const {
    return new GrGLSLYUVtoRGBEffect();
}
void GrYUVtoRGBEffect::onGetGLSLProcessorKey(const GrShaderCaps& caps,
                                             GrProcessorKeyBuilder* b) const {
    b->add32(this->numTextureSamplers());

    uint32_t packed = 0;
    for (int i = 0; i < 4; ++i) {
        if (this->yuvaIndex(i).fIndex < 0) {
            continue;
        }

        uint8_t index = this->yuvaIndex(i).fIndex;
        uint8_t chann = (uint8_t) this->yuvaIndex(i).fChannel;

        SkASSERT(index < 4 && chann < 4);

        packed |= (index | (chann << 2)) << (i * 4);
    }
    b->add32(packed);
}
bool GrYUVtoRGBEffect::onIsEqual(const GrFragmentProcessor& other) const {
    const GrYUVtoRGBEffect& that = other.cast<GrYUVtoRGBEffect>();

    for (int i = 0; i < 4; ++i) {
        if (fYUVAIndices[i] != that.fYUVAIndices[i]) {
            return false;
        }
    }

    for (int i = 0; i < this->numTextureSamplers(); ++i) {
        // 'fSamplers' is checked by the base class
        if (fSamplerTransforms[i] != that.fSamplerTransforms[i]) {
            return false;
        }
    }

    if (fColorSpaceMatrix != that.fColorSpaceMatrix) {
        return false;
    }

    return true;
}
GrYUVtoRGBEffect::GrYUVtoRGBEffect(const GrYUVtoRGBEffect& src)
        : INHERITED(kGrYUVtoRGBEffect_ClassID, src.optimizationFlags())
        , fColorSpaceMatrix(src.fColorSpaceMatrix) {
    int numPlanes = src.numTextureSamplers();
    for (int i = 0; i < numPlanes; ++i) {
        fSamplers[i].reset(sk_ref_sp(src.fSamplers[i].proxy()), src.fSamplers[i].samplerState());
        fSamplerTransforms[i] = src.fSamplerTransforms[i];
        fSamplerCoordTransforms[i] = src.fSamplerCoordTransforms[i];
    }

    this->setTextureSamplerCnt(numPlanes);
    for (int i = 0; i < numPlanes; ++i) {
        this->addCoordTransform(&fSamplerCoordTransforms[i]);
    }

    memcpy(fYUVAIndices, src.fYUVAIndices, sizeof(fYUVAIndices));
}
std::unique_ptr<GrFragmentProcessor> GrYUVtoRGBEffect::clone() const {
    return std::unique_ptr<GrFragmentProcessor>(new GrYUVtoRGBEffect(*this));
}
const GrFragmentProcessor::TextureSampler& GrYUVtoRGBEffect::onTextureSampler(int index) const {
    SkASSERT(index < this->numTextureSamplers());
    return fSamplers[index];
}
