/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrYUVtoRGBEffect_DEFINED
#define GrYUVtoRGBEffect_DEFINED

#include "SkTypes.h"

#include "GrFragmentProcessor.h"
#include "GrCoordTransform.h"

#include "SkYUVAIndex.h"

class GrYUVtoRGBEffect : public GrFragmentProcessor {
public:
<<<<<<< HEAD
    static std::unique_ptr<GrFragmentProcessor> Make(const sk_sp<GrTextureProxy> proxies[],
                                                     const SkYUVAIndex indices[4],
                                                     SkYUVColorSpace yuvColorSpace);
    SkString dumpInfo() const override;

    const SkMatrix44& colorSpaceMatrix() const { return fColorSpaceMatrix; }
    const SkYUVAIndex& yuvaIndex(int i) const { return fYUVAIndices[i]; }

||||||| merged common ancestors
    static std::unique_ptr<GrFragmentProcessor> Make(sk_sp<GrTextureProxy> yProxy,
                                                     sk_sp<GrTextureProxy> uProxy,
                                                     sk_sp<GrTextureProxy> vProxy,
                                                     const SkISize sizes[3],
                                                     SkYUVColorSpace colorSpace, bool nv12);
    SkMatrix44 ySamplerTransform() const { return fYSamplerTransform; }
    SkMatrix44 uSamplerTransform() const { return fUSamplerTransform; }
    SkMatrix44 vSamplerTransform() const { return fVSamplerTransform; }
    SkMatrix44 colorSpaceMatrix() const { return fColorSpaceMatrix; }
    bool nv12() const { return fNv12; }
    static std::unique_ptr<GrFragmentProcessor> Make(
            sk_sp<GrTextureProxy> ySampler, SkMatrix44 ySamplerTransform,
            sk_sp<GrTextureProxy> uSampler, SkMatrix44 uSamplerTransform,
            sk_sp<GrTextureProxy> vSampler, SkMatrix44 vSamplerTransform,
            SkMatrix44 colorSpaceMatrix, bool nv12, GrSamplerState uvSamplerParams) {
        return std::unique_ptr<GrFragmentProcessor>(new GrYUVtoRGBEffect(
                ySampler, ySamplerTransform, uSampler, uSamplerTransform, vSampler,
                vSamplerTransform, colorSpaceMatrix, nv12, uvSamplerParams));
    }
=======
    static std::unique_ptr<GrFragmentProcessor> Make(const sk_sp<GrTextureProxy> proxies[],
                                                     const SkYUVAIndex indices[4],
                                                     SkYUVColorSpace yuvColorSpace,
                                                     GrSamplerState::Filter filterMode);
#ifdef SK_DEBUG
    SkString dumpInfo() const override;
#endif

    const SkMatrix44& colorSpaceMatrix() const { return fColorSpaceMatrix; }
    const SkYUVAIndex& yuvaIndex(int i) const { return fYUVAIndices[i]; }

>>>>>>> upstream-releases
    GrYUVtoRGBEffect(const GrYUVtoRGBEffect& src);
    std::unique_ptr<GrFragmentProcessor> clone() const override;
    const char* name() const override { return "YUVtoRGBEffect"; }

private:
    GrYUVtoRGBEffect(const sk_sp<GrTextureProxy> proxies[], const SkSize scales[],
                     const GrSamplerState::Filter filterModes[], int numPlanes,
                     const SkYUVAIndex yuvaIndices[4], const SkMatrix44& colorSpaceMatrix)
            : INHERITED(kGrYUVtoRGBEffect_ClassID, kNone_OptimizationFlags)
<<<<<<< HEAD
            , fColorSpaceMatrix(colorSpaceMatrix) {
        for (int i = 0; i < numPlanes; ++i) {
            fSamplers[i].reset(std::move(proxies[i]),
                               GrSamplerState(GrSamplerState::WrapMode::kClamp, filterModes[i]));
            fSamplerTransforms[i] = SkMatrix::MakeScale(scales[i].width(), scales[i].height());
            fSamplerCoordTransforms[i].reset(fSamplerTransforms[i], fSamplers[i].proxy(), true);
        }

        this->setTextureSamplerCnt(numPlanes);
        for (int i = 0; i < numPlanes; ++i) {
            this->addCoordTransform(&fSamplerCoordTransforms[i]);
        }

        memcpy(fYUVAIndices, yuvaIndices, sizeof(fYUVAIndices));
||||||| merged common ancestors
            , fYSampler(std::move(ySampler))
            , fYSamplerTransform(ySamplerTransform)
            , fUSampler(std::move(uSampler), uvSamplerParams)
            , fUSamplerTransform(uSamplerTransform)
            , fVSampler(std::move(vSampler), uvSamplerParams)
            , fVSamplerTransform(vSamplerTransform)
            , fColorSpaceMatrix(colorSpaceMatrix)
            , fNv12(nv12)
            , fYSamplerCoordTransform(ySamplerTransform, fYSampler.proxy())
            , fUSamplerCoordTransform(uSamplerTransform, fUSampler.proxy())
            , fVSamplerCoordTransform(vSamplerTransform, fVSampler.proxy()) {
        this->addTextureSampler(&fYSampler);
        this->addTextureSampler(&fUSampler);
        this->addTextureSampler(&fVSampler);
        this->addCoordTransform(&fYSamplerCoordTransform);
        this->addCoordTransform(&fUSamplerCoordTransform);
        this->addCoordTransform(&fVSamplerCoordTransform);
=======
            , fColorSpaceMatrix(colorSpaceMatrix) {
        for (int i = 0; i < numPlanes; ++i) {
            fSamplers[i].reset(std::move(proxies[i]),
                               GrSamplerState(GrSamplerState::WrapMode::kClamp, filterModes[i]));
            fSamplerTransforms[i] = SkMatrix::MakeScale(scales[i].width(), scales[i].height());
            fSamplerCoordTransforms[i] =
                    GrCoordTransform(fSamplerTransforms[i], fSamplers[i].proxy());
        }

        this->setTextureSamplerCnt(numPlanes);
        for (int i = 0; i < numPlanes; ++i) {
            this->addCoordTransform(&fSamplerCoordTransforms[i]);
        }

        memcpy(fYUVAIndices, yuvaIndices, sizeof(fYUVAIndices));
>>>>>>> upstream-releases
    }
    GrGLSLFragmentProcessor* onCreateGLSLInstance() const override;
    void onGetGLSLProcessorKey(const GrShaderCaps&, GrProcessorKeyBuilder*) const override;
    bool onIsEqual(const GrFragmentProcessor&) const override;
    const TextureSampler& onTextureSampler(int) const override;
    GR_DECLARE_FRAGMENT_PROCESSOR_TEST

    TextureSampler   fSamplers[4];
    SkMatrix44       fSamplerTransforms[4];
    GrCoordTransform fSamplerCoordTransforms[4];
    SkYUVAIndex      fYUVAIndices[4];
    SkMatrix44       fColorSpaceMatrix;

    typedef GrFragmentProcessor INHERITED;
};
#endif
