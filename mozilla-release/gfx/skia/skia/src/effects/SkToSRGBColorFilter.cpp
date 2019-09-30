/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkColorSpacePriv.h"
#include "SkColorSpaceXformSteps.h"
#include "SkRasterPipeline.h"
#include "SkReadBuffer.h"
#include "SkString.h"
#include "SkToSRGBColorFilter.h"
#include "SkWriteBuffer.h"

#if SK_SUPPORT_GPU
    #include "GrColorSpaceXform.h"
#endif

void SkToSRGBColorFilter::onAppendStages(SkRasterPipeline* p,
                                         SkColorSpace* /*dst color space*/,
                                         SkArenaAlloc* alloc,
                                         bool shaderIsOpaque) const {
<<<<<<< HEAD
    alloc->make<SkColorSpaceXformSteps>(fSrcColorSpace.get(), kPremul_SkAlphaType,
                                        sk_srgb_singleton() , kPremul_SkAlphaType)
        ->apply(p);
||||||| merged common ancestors
    // Step 1: Linearize by undoing the src transfer function.
    // Linear and sRGB will return true to isNumericalTransferFn(), so we check them first.
    SkColorSpaceTransferFn srcFn;
    if (fSrcColorSpace->gammaIsLinear()) {
        // Nothing to do.
    } else if (fSrcColorSpace->gammaCloseToSRGB()) {
        p->append(SkRasterPipeline::from_srgb);
    } else if (fSrcColorSpace->isNumericalTransferFn(&srcFn)) {
        auto copy = alloc->make<SkColorSpaceTransferFn>(srcFn);
        p->append(SkRasterPipeline::parametric_r, copy);
        p->append(SkRasterPipeline::parametric_g, copy);
        p->append(SkRasterPipeline::parametric_b, copy);
    } else {
        SkDEBUGFAIL("Looks like we got a table transfer function here, quite unexpectedly.");
        // TODO: If we really need to handle this, we can, but I don't think Ganesh does.
    }

    // Step 2: Transform to sRGB gamut, without clamping.
    // TODO: because...
    float* gamut_transform = alloc->makeArrayDefault<float>(12);
    (void)append_gamut_transform_noclamp(p,
                                         gamut_transform,
                                         fSrcColorSpace.get(),
                                         SkColorSpace::MakeSRGB().get());

    // Step 3: Back to sRGB encoding.
    p->append(SkRasterPipeline::to_srgb);
=======
    bool shaderIsNormalized = false;
    alloc->make<SkColorSpaceXformSteps>(fSrcColorSpace.get(), kPremul_SkAlphaType,
                                        sk_srgb_singleton() , kPremul_SkAlphaType)
        ->apply(p, shaderIsNormalized);
>>>>>>> upstream-releases
}

sk_sp<SkColorFilter> SkToSRGBColorFilter::Make(sk_sp<SkColorSpace> srcColorSpace) {
    if (!srcColorSpace || srcColorSpace->isSRGB()) {
        return nullptr;
    } else {
        return sk_sp<SkColorFilter>(new SkToSRGBColorFilter(std::move(srcColorSpace)));
    }
}

SkToSRGBColorFilter::SkToSRGBColorFilter(sk_sp<SkColorSpace> srcColorSpace)
        : fSrcColorSpace(std::move(srcColorSpace)) {
    SkASSERT(fSrcColorSpace);
}

sk_sp<SkFlattenable> SkToSRGBColorFilter::CreateProc(SkReadBuffer& buffer) {
    auto data = buffer.readByteArrayAsData();
    return data ? Make(SkColorSpace::Deserialize(data->data(), data->size())) : nullptr;
}

void SkToSRGBColorFilter::flatten(SkWriteBuffer& buffer) const {
    buffer.writeDataAsByteArray(fSrcColorSpace->serialize().get());
}

#if SK_SUPPORT_GPU
std::unique_ptr<GrFragmentProcessor> SkToSRGBColorFilter::asFragmentProcessor(
<<<<<<< HEAD
        GrContext*, const GrColorSpaceInfo&) const {
    return GrColorSpaceXformEffect::Make(fSrcColorSpace.get(), kPremul_SkAlphaType,
                                         sk_srgb_singleton(),  kPremul_SkAlphaType);
||||||| merged common ancestors
        GrContext*, const GrColorSpaceInfo&) const {
    return GrNonlinearColorSpaceXformEffect::Make(fSrcColorSpace.get(),
                                                  SkColorSpace::MakeSRGB().get());
=======
        GrRecordingContext*, const GrColorSpaceInfo&) const {
    return GrColorSpaceXformEffect::Make(fSrcColorSpace.get(), kPremul_SkAlphaType,
                                         sk_srgb_singleton(),  kPremul_SkAlphaType);
>>>>>>> upstream-releases
}
#endif
