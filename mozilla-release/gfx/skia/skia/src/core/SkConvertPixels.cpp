/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkColorData.h"
#include "SkColorSpacePriv.h"
#include "SkColorSpaceXformSteps.h"
#include "SkConvertPixels.h"
#include "SkHalf.h"
#include "SkImageInfoPriv.h"
#include "SkOpts.h"
#include "SkRasterPipeline.h"

static bool rect_memcpy(const SkImageInfo& dstInfo,       void* dstPixels, size_t dstRB,
                        const SkImageInfo& srcInfo, const void* srcPixels, size_t srcRB,
                        const SkColorSpaceXformSteps& steps) {
    // We can copy the pixels when no color type, alpha type, or color space changes.
    if (dstInfo.colorType() != srcInfo.colorType()) {
        return false;
    }
    if (dstInfo.colorType() != kAlpha_8_SkColorType
            && steps.flags.mask() != 0b00000) {
        return false;
    }

    SkRectMemcpy(dstPixels, dstRB,
                 srcPixels, srcRB, dstInfo.minRowBytes(), dstInfo.height());
    return true;
}

<<<<<<< HEAD
static bool swizzle_and_multiply(const SkImageInfo& dstInfo,       void* dstPixels, size_t dstRB,
                                 const SkImageInfo& srcInfo, const void* srcPixels, size_t srcRB,
                                 const SkColorSpaceXformSteps& steps) {
    auto is_8888 = [](SkColorType ct) {
        return ct == kRGBA_8888_SkColorType || ct == kBGRA_8888_SkColorType;
    };
    if (!is_8888(dstInfo.colorType()) ||
        !is_8888(srcInfo.colorType()) ||
        steps.flags.linearize || steps.flags.gamut_transform || steps.flags.encode) {
        return false;
    }

    // It'd be kind of silly for us to both...
    SkASSERT(!(steps.flags.premul && steps.flags.unpremul));
||||||| merged common ancestors
// Fast Path 2: Simple swizzles and premuls.
enum AlphaVerb {
    kNothing_AlphaVerb,
    kPremul_AlphaVerb,
    kUnpremul_AlphaVerb,
};

template <bool kSwapRB>
static void wrap_unpremultiply(uint32_t* dst, const void* src, int count) {
    SkUnpremultiplyRow<kSwapRB>(dst, (const uint32_t*) src, count);
}
=======
static bool swizzle_or_premul(const SkImageInfo& dstInfo,       void* dstPixels, size_t dstRB,
                              const SkImageInfo& srcInfo, const void* srcPixels, size_t srcRB,
                              const SkColorSpaceXformSteps& steps) {
    auto is_8888 = [](SkColorType ct) {
        return ct == kRGBA_8888_SkColorType || ct == kBGRA_8888_SkColorType;
    };
    if (!is_8888(dstInfo.colorType()) ||
        !is_8888(srcInfo.colorType()) ||
        steps.flags.linearize         ||
        steps.flags.gamut_transform   ||
        steps.flags.unpremul          ||
        steps.flags.encode) {
        return false;
    }
>>>>>>> upstream-releases

    const bool swapRB = dstInfo.colorType() != srcInfo.colorType();

<<<<<<< HEAD
    void (*fn)(uint32_t*, const uint32_t*, int) = nullptr;

    if (steps.flags.premul) {
        fn = swapRB ? SkOpts::RGBA_to_bgrA
                    : SkOpts::RGBA_to_rgbA;
    } else if (steps.flags.unpremul) {
        fn = swapRB ? SkUnpremultiplyRow<true>
                    : SkUnpremultiplyRow<false>;
    } else {
        // If we're not swizzling, we ought to have used rect_memcpy().
        SkASSERT(swapRB);
        fn = SkOpts::RGBA_to_BGRA;
||||||| merged common ancestors
    switch (alphaVerb) {
        case kNothing_AlphaVerb:
            // If we do not need to swap or multiply, we should hit the memcpy case.
            SkASSERT(swapRB);
            proc = SkOpts::RGBA_to_BGRA;
            break;
        case kPremul_AlphaVerb:
            proc = swapRB ? SkOpts::RGBA_to_bgrA : SkOpts::RGBA_to_rgbA;
            break;
        case kUnpremul_AlphaVerb:
            proc = swapRB ? wrap_unpremultiply<true> : wrap_unpremultiply<false>;
            break;
=======
    void (*fn)(uint32_t*, const uint32_t*, int) = nullptr;

    if (steps.flags.premul) {
        fn = swapRB ? SkOpts::RGBA_to_bgrA
                    : SkOpts::RGBA_to_rgbA;
    } else {
        // If we're not swizzling, we ought to have used rect_memcpy().
        SkASSERT(swapRB);
        fn = SkOpts::RGBA_to_BGRA;
>>>>>>> upstream-releases
    }

    for (int y = 0; y < dstInfo.height(); y++) {
        fn((uint32_t*)dstPixels, (const uint32_t*)srcPixels, dstInfo.width());
        dstPixels = SkTAddOffset<void>(dstPixels, dstRB);
        srcPixels = SkTAddOffset<const void>(srcPixels, srcRB);
    }
    return true;
}

static bool convert_to_alpha8(const SkImageInfo& dstInfo,       void* vdst, size_t dstRB,
                              const SkImageInfo& srcInfo, const void*  src, size_t srcRB,
                              const SkColorSpaceXformSteps&) {
    if (dstInfo.colorType() != kAlpha_8_SkColorType) {
        return false;
    }
    auto dst = (uint8_t*)vdst;

    switch (srcInfo.colorType()) {
        case kUnknown_SkColorType:
        case kAlpha_8_SkColorType: {
            // Unknown should never happen.
            // Alpha8 should have been handled by rect_memcpy().
            SkASSERT(false);
            return false;
        }

        case kGray_8_SkColorType:
        case kRGB_565_SkColorType:
        case kRGB_888x_SkColorType:
        case kRGB_101010x_SkColorType: {
            for (int y = 0; y < srcInfo.height(); ++y) {
               memset(dst, 0xFF, srcInfo.width());
               dst = SkTAddOffset<uint8_t>(dst, dstRB);
            }
            return true;
        }

        case kARGB_4444_SkColorType: {
            auto src16 = (const uint16_t*) src;
            for (int y = 0; y < srcInfo.height(); y++) {
                for (int x = 0; x < srcInfo.width(); x++) {
                    dst[x] = SkPacked4444ToA32(src16[x]);
                }
                dst = SkTAddOffset<uint8_t>(dst, dstRB);
                src16 = SkTAddOffset<const uint16_t>(src16, srcRB);
            }
            return true;
        }

        case kBGRA_8888_SkColorType:
        case kRGBA_8888_SkColorType: {
            auto src32 = (const uint32_t*) src;
            for (int y = 0; y < srcInfo.height(); y++) {
                for (int x = 0; x < srcInfo.width(); x++) {
                    dst[x] = src32[x] >> 24;
                }
                dst = SkTAddOffset<uint8_t>(dst, dstRB);
                src32 = SkTAddOffset<const uint32_t>(src32, srcRB);
            }
            return true;
        }

        case kRGBA_1010102_SkColorType: {
            auto src32 = (const uint32_t*) src;
            for (int y = 0; y < srcInfo.height(); y++) {
                for (int x = 0; x < srcInfo.width(); x++) {
                    dst[x] = (src32[x] >> 30) * 0x55;
                }
                dst = SkTAddOffset<uint8_t>(dst, dstRB);
                src32 = SkTAddOffset<const uint32_t>(src32, srcRB);
            }
            return true;
        }
<<<<<<< HEAD

||||||| merged common ancestors
=======

        case kRGBA_F16Norm_SkColorType:
>>>>>>> upstream-releases
        case kRGBA_F16_SkColorType: {
            auto src64 = (const uint64_t*) src;
            for (int y = 0; y < srcInfo.height(); y++) {
                for (int x = 0; x < srcInfo.width(); x++) {
                    dst[x] = (uint8_t) (255.0f * SkHalfToFloat(src64[x] >> 48));
                }
                dst = SkTAddOffset<uint8_t>(dst, dstRB);
                src64 = SkTAddOffset<const uint64_t>(src64, srcRB);
            }
            return true;
        }

        case kRGBA_F32_SkColorType: {
            auto rgba = (const float*)src;
            for (int y = 0; y < srcInfo.height(); y++) {
                for (int x = 0; x < srcInfo.width(); x++) {
                    dst[x] = (uint8_t)(255.0f * rgba[4*x+3]);
                }
                dst  = SkTAddOffset<uint8_t>(dst, dstRB);
                rgba = SkTAddOffset<const float>(rgba, srcRB);
            }
            return true;
        }
    }
    return false;
}

// Default: Use the pipeline.
static void convert_with_pipeline(const SkImageInfo& dstInfo, void* dstRow, size_t dstRB,
                                  const SkImageInfo& srcInfo, const void* srcRow, size_t srcRB,
                                  const SkColorSpaceXformSteps& steps) {

    SkRasterPipeline_MemoryCtx src = { (void*)srcRow, (int)(srcRB / srcInfo.bytesPerPixel()) },
                               dst = { (void*)dstRow, (int)(dstRB / dstInfo.bytesPerPixel()) };

    SkRasterPipeline_<256> pipeline;
<<<<<<< HEAD
    pipeline.append_load(srcInfo.colorType(), &src);
    steps.apply(&pipeline);

    pipeline.append_gamut_clamp_if_normalized(dstInfo);

    // We'll dither if we're decreasing precision below 32-bit.
    float dither_rate = 0.0f;
    if (srcInfo.bytesPerPixel() > dstInfo.bytesPerPixel()) {
        switch (dstInfo.colorType()) {
            case   kRGB_565_SkColorType: dither_rate = 1/63.0f; break;
            case kARGB_4444_SkColorType: dither_rate = 1/15.0f; break;
            default:                     dither_rate =    0.0f; break;
        }
    }
    if (dither_rate > 0) {
        pipeline.append(SkRasterPipeline::dither, &dither_rate);
    }

    pipeline.append_store(dstInfo.colorType(), &dst);
||||||| merged common ancestors
    switch (srcInfo.colorType()) {
        case kRGBA_8888_SkColorType:
            pipeline.append(SkRasterPipeline::load_8888, &src);
            break;
        case kBGRA_8888_SkColorType:
            pipeline.append(SkRasterPipeline::load_bgra, &src);
            break;
        case kRGB_565_SkColorType:
            pipeline.append(SkRasterPipeline::load_565, &src);
            break;
        case kRGBA_F16_SkColorType:
            pipeline.append(SkRasterPipeline::load_f16, &src);
            break;
        case kGray_8_SkColorType:
            pipeline.append(SkRasterPipeline::load_g8, &src);
            break;
        case kARGB_4444_SkColorType:
            pipeline.append(SkRasterPipeline::load_4444, &src);
            break;
        default:
            SkASSERT(false);
            break;
    }

    SkAlphaType premulState = srcInfo.alphaType();
    if (kPremul_SkAlphaType == premulState && SkTransferFunctionBehavior::kIgnore == behavior) {
        pipeline.append(SkRasterPipeline::unpremul);
        premulState = kUnpremul_SkAlphaType;
    }

    SkColorSpaceTransferFn srcFn;
    if (isColorAware && srcInfo.gammaCloseToSRGB()) {
        pipeline.append(SkRasterPipeline::from_srgb);
    } else if (isColorAware && !srcInfo.colorSpace()->gammaIsLinear()) {
        SkAssertResult(srcInfo.colorSpace()->isNumericalTransferFn(&srcFn));
        if (is_just_gamma(srcFn)) {
            pipeline.append(SkRasterPipeline::gamma, &srcFn.fG);
        } else {
            pipeline.append(SkRasterPipeline::parametric_r, &srcFn);
            pipeline.append(SkRasterPipeline::parametric_g, &srcFn);
            pipeline.append(SkRasterPipeline::parametric_b, &srcFn);
        }
    }

    float matrix[12];
    if (isColorAware) {
        append_gamut_transform(&pipeline, matrix, srcInfo.colorSpace(), dstInfo.colorSpace(),
                               premulState);
    }

    SkAlphaType dat = dstInfo.alphaType();
    if (SkTransferFunctionBehavior::kRespect == behavior) {
        if (kPremul_SkAlphaType == premulState && kUnpremul_SkAlphaType == dat) {
            pipeline.append(SkRasterPipeline::unpremul);
            premulState = kUnpremul_SkAlphaType;
        } else if (kUnpremul_SkAlphaType == premulState && kPremul_SkAlphaType == dat) {
            pipeline.append(SkRasterPipeline::premul);
            premulState = kPremul_SkAlphaType;
        }
    }

    SkColorSpaceTransferFn dstFn;
    if (isColorAware && dstInfo.gammaCloseToSRGB()) {
        pipeline.append(SkRasterPipeline::to_srgb);
    } else if (isColorAware && !dstInfo.colorSpace()->gammaIsLinear()) {
        SkAssertResult(dstInfo.colorSpace()->isNumericalTransferFn(&dstFn));
        dstFn = dstFn.invert();
        if (is_just_gamma(dstFn)) {
            pipeline.append(SkRasterPipeline::gamma, &dstFn.fG);
        } else {
            pipeline.append(SkRasterPipeline::parametric_r, &dstFn);
            pipeline.append(SkRasterPipeline::parametric_g, &dstFn);
            pipeline.append(SkRasterPipeline::parametric_b, &dstFn);
        }
    }

    if (kUnpremul_SkAlphaType == premulState && kPremul_SkAlphaType == dat &&
        SkTransferFunctionBehavior::kIgnore == behavior)
    {
        pipeline.append(SkRasterPipeline::premul);
        premulState = kPremul_SkAlphaType;
    }

    // The final premul state must equal the dst alpha type.  Note that if we are "converting"
    // opaque to another alpha type, there's no need to worry about multiplication.
    SkASSERT(premulState == dat || kOpaque_SkAlphaType == srcInfo.alphaType());

    // We'll dither if we're decreasing precision below 32-bit.
    float dither_rate = 0.0f;
    if (srcInfo.bytesPerPixel() > dstInfo.bytesPerPixel()) {
        switch (dstInfo.colorType()) {
            case   kRGB_565_SkColorType: dither_rate = 1/63.0f; break;
            case kARGB_4444_SkColorType: dither_rate = 1/15.0f; break;
            default:                     dither_rate =    0.0f; break;
        }
    }
    if (dither_rate > 0) {
        pipeline.append(SkRasterPipeline::dither, &dither_rate);
    }

    switch (dstInfo.colorType()) {
        case kRGBA_8888_SkColorType:
            pipeline.append(SkRasterPipeline::store_8888, &dst);
            break;
        case kBGRA_8888_SkColorType:
            pipeline.append(SkRasterPipeline::store_bgra, &dst);
            break;
        case kRGB_565_SkColorType:
            pipeline.append(SkRasterPipeline::store_565, &dst);
            break;
        case kRGBA_F16_SkColorType:
            pipeline.append(SkRasterPipeline::store_f16, &dst);
            break;
        case kARGB_4444_SkColorType:
            pipeline.append(SkRasterPipeline::store_4444, &dst);
            break;
        default:
            SkASSERT(false);
            break;
    }

=======
    pipeline.append_load(srcInfo.colorType(), &src);
    steps.apply(&pipeline, srcInfo.colorType());

    pipeline.append_gamut_clamp_if_normalized(dstInfo);

    pipeline.append_store(dstInfo.colorType(), &dst);
>>>>>>> upstream-releases
    pipeline.run(0,0, srcInfo.width(), srcInfo.height());
}

void SkConvertPixels(const SkImageInfo& dstInfo,       void* dstPixels, size_t dstRB,
                     const SkImageInfo& srcInfo, const void* srcPixels, size_t srcRB) {
    SkASSERT(dstInfo.dimensions() == srcInfo.dimensions());
    SkASSERT(SkImageInfoValidConversion(dstInfo, srcInfo));

    SkColorSpaceXformSteps steps{srcInfo.colorSpace(), srcInfo.alphaType(),
                                 dstInfo.colorSpace(), dstInfo.alphaType()};

<<<<<<< HEAD
    for (auto fn : {rect_memcpy, swizzle_and_multiply, convert_to_alpha8}) {
        if (fn(dstInfo, dstPixels, dstRB, srcInfo, srcPixels, srcRB, steps)) {
            return;
        }
||||||| merged common ancestors
    // Fast Path 2: Simple swizzles and premuls.
    if (4 == srcInfo.bytesPerPixel() && 4 == dstInfo.bytesPerPixel() && !isColorAware) {
        swizzle_and_multiply(dstInfo, dstPixels, dstRB, srcInfo, srcPixels, srcRB);
        return;
    }

    // Fast Path 3: Color space xform.
    if (isColorAware && optimized_color_xform(dstInfo, srcInfo, behavior)) {
        apply_color_xform(dstInfo, dstPixels, dstRB, srcInfo, srcPixels, srcRB, behavior);
        return;
    }

    // Fast Path 4: Alpha 8 dsts.
    if (kAlpha_8_SkColorType == dstInfo.colorType()) {
        convert_to_alpha8((uint8_t*) dstPixels, dstRB, srcInfo, srcPixels, srcRB, ctable);
        return;
=======
    for (auto fn : {rect_memcpy, swizzle_or_premul, convert_to_alpha8}) {
        if (fn(dstInfo, dstPixels, dstRB, srcInfo, srcPixels, srcRB, steps)) {
            return;
        }
>>>>>>> upstream-releases
    }
    convert_with_pipeline(dstInfo, dstPixels, dstRB, srcInfo, srcPixels, srcRB, steps);
}
