/*
 * Copyright 2012 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkImageEncoderFns_DEFINED
#define SkImageEncoderFns_DEFINED

#include "../../third_party/skcms/skcms.h"
#include "SkColor.h"
#include "SkColorData.h"
#include "SkICC.h"
#include "SkTypes.h"

typedef void (*transform_scanline_proc)(char* dst, const char* src, int width, int bpp);

static inline void transform_scanline_memcpy(char* dst, const char* src, int width, int bpp) {
    memcpy(dst, src, width * bpp);
}

static inline void transform_scanline_A8_to_GrayAlpha(char* dst, const char* src, int width, int) {
    for (int i = 0; i < width; i++) {
        *dst++ = 0;
        *dst++ = *src++;
    }
}


static void skcms(char* dst, const char* src, int n,
                  skcms_PixelFormat srcFmt, skcms_AlphaFormat srcAlpha,
                  skcms_PixelFormat dstFmt, skcms_AlphaFormat dstAlpha) {
    SkAssertResult(skcms_Transform(src, srcFmt, srcAlpha, nullptr,
                                   dst, dstFmt, dstAlpha, nullptr, n));
}

static inline void transform_scanline_gray(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_G_8,     skcms_AlphaFormat_Unpremul,
          skcms_PixelFormat_RGB_888, skcms_AlphaFormat_Unpremul);
}

static inline void transform_scanline_565(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_BGR_565, skcms_AlphaFormat_Unpremul,
          skcms_PixelFormat_RGB_888, skcms_AlphaFormat_Unpremul);
}

static inline void transform_scanline_RGBX(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_RGBA_8888, skcms_AlphaFormat_Unpremul,
          skcms_PixelFormat_RGB_888  , skcms_AlphaFormat_Unpremul);
}

static inline void transform_scanline_BGRX(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_BGRA_8888, skcms_AlphaFormat_Unpremul,
          skcms_PixelFormat_RGB_888  , skcms_AlphaFormat_Unpremul);
}

static inline void transform_scanline_444(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_ABGR_4444, skcms_AlphaFormat_Unpremul,
          skcms_PixelFormat_RGB_888  , skcms_AlphaFormat_Unpremul);
}

static inline void transform_scanline_rgbA(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_RGBA_8888, skcms_AlphaFormat_PremulAsEncoded,
          skcms_PixelFormat_RGBA_8888, skcms_AlphaFormat_Unpremul);
}

static inline void transform_scanline_bgrA(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_BGRA_8888, skcms_AlphaFormat_PremulAsEncoded,
          skcms_PixelFormat_RGBA_8888, skcms_AlphaFormat_Unpremul);
}

<<<<<<< HEAD
/**
 * Premultiply RGBA to rgbA.
 */
static inline void transform_scanline_to_premul_legacy(char* SK_RESTRICT dst,
                                                       const char* SK_RESTRICT src,
                                                       int width, int, const SkPMColor*) {
    SkOpts::RGBA_to_rgbA((uint32_t*)dst, (const uint32_t*)src, width);
||||||| merged common ancestors
template <bool kIsRGBA>
static inline void transform_scanline_unpremultiply_sRGB(void* dst, const void* src, int width) {
    SkJumper_MemoryCtx src_ctx = { (void*)src, 0 },
                       dst_ctx = { (void*)dst, 0 };
    SkRasterPipeline_<256> p;
    if (kIsRGBA) {
        p.append(SkRasterPipeline::load_8888, &src_ctx);
    } else {
        p.append(SkRasterPipeline::load_bgra, &src_ctx);
    }

    p.append(SkRasterPipeline::from_srgb);
    p.append(SkRasterPipeline::unpremul);
    p.append(SkRasterPipeline::to_srgb);
    p.append(SkRasterPipeline::store_8888, &dst_ctx);
    p.run(0,0, width,1);
}

/**
 * Premultiply RGBA to rgbA.
 */
static inline void transform_scanline_to_premul_legacy(char* SK_RESTRICT dst,
                                                       const char* SK_RESTRICT src,
                                                       int width, int, const SkPMColor*) {
    SkOpts::RGBA_to_rgbA((uint32_t*)dst, (const uint32_t*)src, width);
=======
static inline void transform_scanline_to_premul_legacy(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_RGBA_8888, skcms_AlphaFormat_Unpremul,
          skcms_PixelFormat_RGBA_8888, skcms_AlphaFormat_PremulAsEncoded);
}

static inline void transform_scanline_BGRA(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_BGRA_8888, skcms_AlphaFormat_Unpremul,
          skcms_PixelFormat_RGBA_8888, skcms_AlphaFormat_Unpremul);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/**
 * Transform from kUnpremul, kBGRA_8888_SkColorType to 4-bytes-per-pixel unpremultiplied RGBA.
 */
static inline void transform_scanline_BGRA(char* SK_RESTRICT dst, const char* SK_RESTRICT src,
                                           int width, int, const SkPMColor*) {
    const uint32_t* srcP = (const SkPMColor*)src;
    for (int i = 0; i < width; i++) {
        uint32_t c = *srcP++;
        *dst++ = (c >> 16) & 0xFF;
        *dst++ = (c >>  8) & 0xFF;
        *dst++ = (c >>  0) & 0xFF;
        *dst++ = (c >> 24) & 0xFF;
    }
||||||| merged common ancestors
/**
 * Premultiply RGBA to rgbA linearly.
 */
static inline void transform_scanline_to_premul_linear(char* SK_RESTRICT dst,
                                                       const char* SK_RESTRICT src,
                                                       int width, int, const SkPMColor*) {
    SkJumper_MemoryCtx src_ctx = { (void*)src, 0 },
                       dst_ctx = { (void*)dst, 0 };
    SkRasterPipeline_<256> p;
    p.append(SkRasterPipeline::load_8888, &src_ctx);
    p.append(SkRasterPipeline::from_srgb);
    p.append(SkRasterPipeline::premul);
    p.append(SkRasterPipeline::to_srgb);
    p.append(SkRasterPipeline::store_8888, &dst_ctx);
    p.run(0,0, width,1);
}

/**
 * Transform from kPremul, kRGBA_8888_SkColorType to 4-bytes-per-pixel unpremultiplied RGBA.
 */
static inline void transform_scanline_srgbA(char* SK_RESTRICT dst, const char* SK_RESTRICT src,
                                            int width, int, const SkPMColor*) {
    transform_scanline_unpremultiply_sRGB<true>(dst, src, width);
}

/**
 * Transform from kPremul, kBGRA_8888_SkColorType to 4-bytes-per-pixel unpremultiplied RGBA.
 */
static inline void transform_scanline_sbgrA(char* SK_RESTRICT dst, const char* SK_RESTRICT src,
                                            int width, int, const SkPMColor*) {
    transform_scanline_unpremultiply_sRGB<false>(dst, src, width);
}

/**
 * Transform from kUnpremul, kBGRA_8888_SkColorType to 4-bytes-per-pixel unpremultiplied RGBA.
 */
static inline void transform_scanline_BGRA(char* SK_RESTRICT dst, const char* SK_RESTRICT src,
                                           int width, int, const SkPMColor*) {
    const uint32_t* srcP = (const SkPMColor*)src;
    for (int i = 0; i < width; i++) {
        uint32_t c = *srcP++;
        *dst++ = (c >> 16) & 0xFF;
        *dst++ = (c >>  8) & 0xFF;
        *dst++ = (c >>  0) & 0xFF;
        *dst++ = (c >> 24) & 0xFF;
    }
=======
static inline void transform_scanline_4444(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_ABGR_4444, skcms_AlphaFormat_PremulAsEncoded,
          skcms_PixelFormat_RGBA_8888, skcms_AlphaFormat_Unpremul);
}

static inline void transform_scanline_101010x(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_RGBA_1010102, skcms_AlphaFormat_Unpremul,
          skcms_PixelFormat_RGB_161616BE, skcms_AlphaFormat_Unpremul);
}

static inline void transform_scanline_1010102(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_RGBA_1010102,    skcms_AlphaFormat_Unpremul,
          skcms_PixelFormat_RGBA_16161616BE, skcms_AlphaFormat_Unpremul);
}

static inline void transform_scanline_1010102_premul(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_RGBA_1010102,    skcms_AlphaFormat_PremulAsEncoded,
          skcms_PixelFormat_RGBA_16161616BE, skcms_AlphaFormat_Unpremul);
>>>>>>> upstream-releases
}

static inline void transform_scanline_F16(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_RGBA_hhhh,       skcms_AlphaFormat_Unpremul,
          skcms_PixelFormat_RGBA_16161616BE, skcms_AlphaFormat_Unpremul);
}

static inline void transform_scanline_F16_premul(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_RGBA_hhhh,       skcms_AlphaFormat_PremulAsEncoded,
          skcms_PixelFormat_RGBA_16161616BE, skcms_AlphaFormat_Unpremul);
}

static inline void transform_scanline_F16_to_8888(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_RGBA_hhhh, skcms_AlphaFormat_Unpremul,
          skcms_PixelFormat_RGBA_8888, skcms_AlphaFormat_Unpremul);
}

static inline void transform_scanline_F16_premul_to_8888(char* dst,
                                                         const char* src,
                                                         int width,
                                                         int) {
    skcms(dst, src, width,
          skcms_PixelFormat_RGBA_hhhh, skcms_AlphaFormat_PremulAsEncoded,
          skcms_PixelFormat_RGBA_8888, skcms_AlphaFormat_Unpremul);
}

static inline void transform_scanline_F16_to_premul_8888(char* dst,
                                                         const char* src,
                                                         int width,
                                                         int) {
    skcms(dst, src, width,
          skcms_PixelFormat_RGBA_hhhh, skcms_AlphaFormat_Unpremul,
          skcms_PixelFormat_RGBA_8888, skcms_AlphaFormat_PremulAsEncoded);
}

<<<<<<< HEAD
/**
 * Transform from kRGBA_F16 to 8-bytes-per-pixel RGBA.
 */
static inline void transform_scanline_F16(char* SK_RESTRICT dst, const char* SK_RESTRICT src,
                                          int width, int, const SkPMColor*) {
    SkJumper_MemoryCtx src_ctx = { (void*)src, 0 },
                       dst_ctx = { (void*)dst, 0 };
    SkRasterPipeline_<256> p;
    p.append(SkRasterPipeline::load_f16, &src_ctx);
    p.append(SkRasterPipeline::clamp_0);  // F16 values may be out of [0,1] range, so clamp.
    p.append(SkRasterPipeline::clamp_1);
    p.append(SkRasterPipeline::store_u16_be, &dst_ctx);
    p.run(0,0, width,1);
}

/**
 * Transform from kPremul, kRGBA_F16 to 8-bytes-per-pixel RGBA.
 */
static inline void transform_scanline_F16_premul(char* SK_RESTRICT dst, const char* SK_RESTRICT src,
                                                 int width, int, const SkPMColor*) {
    SkJumper_MemoryCtx src_ctx = { (void*)src, 0 },
                       dst_ctx = { (void*)dst, 0 };
    SkRasterPipeline_<256> p;
    p.append(SkRasterPipeline::load_f16, &src_ctx);
    p.append(SkRasterPipeline::unpremul);
    p.append(SkRasterPipeline::clamp_0);  // F16 values may be out of [0,1] range, so clamp.
    p.append(SkRasterPipeline::clamp_1);
    p.append(SkRasterPipeline::store_u16_be, &dst_ctx);
    p.run(0,0, width,1);
}

/**
 * Transform from kRGBA_F16 to 4-bytes-per-pixel RGBA.
 */
static inline void transform_scanline_F16_to_8888(char* SK_RESTRICT dst,
                                                  const char* SK_RESTRICT src, int width, int,
                                                  const SkPMColor*) {
    SkJumper_MemoryCtx src_ctx = { (void*)src, 0 },
                       dst_ctx = { (void*)dst, 0 };
    SkRasterPipeline_<256> p;
    p.append(SkRasterPipeline::load_f16, &src_ctx);
    p.append(SkRasterPipeline::clamp_0);  // F16 values may be out of [0,1] range, so clamp.
    p.append(SkRasterPipeline::clamp_1);
    p.append(SkRasterPipeline::store_8888, &dst_ctx);
    p.run(0,0, width,1);
||||||| merged common ancestors
/**
 * Transform from kRGBA_F16 to 8-bytes-per-pixel RGBA.
 */
static inline void transform_scanline_F16(char* SK_RESTRICT dst, const char* SK_RESTRICT src,
                                          int width, int, const SkPMColor*) {
    SkJumper_MemoryCtx src_ctx = { (void*)src, 0 },
                       dst_ctx = { (void*)dst, 0 };
    SkRasterPipeline_<256> p;
    p.append(SkRasterPipeline::load_f16, &src_ctx);
    p.append(SkRasterPipeline::clamp_0);  // F16 values may be out of [0,1] range, so clamp.
    p.append(SkRasterPipeline::clamp_1);
    p.append(SkRasterPipeline::to_srgb);
    p.append(SkRasterPipeline::store_u16_be, &dst_ctx);
    p.run(0,0, width,1);
}

/**
 * Transform from kPremul, kRGBA_F16 to 8-bytes-per-pixel RGBA.
 */
static inline void transform_scanline_F16_premul(char* SK_RESTRICT dst, const char* SK_RESTRICT src,
                                                 int width, int, const SkPMColor*) {
    SkJumper_MemoryCtx src_ctx = { (void*)src, 0 },
                       dst_ctx = { (void*)dst, 0 };
    SkRasterPipeline_<256> p;
    p.append(SkRasterPipeline::load_f16, &src_ctx);
    p.append(SkRasterPipeline::unpremul);
    p.append(SkRasterPipeline::clamp_0);  // F16 values may be out of [0,1] range, so clamp.
    p.append(SkRasterPipeline::clamp_1);
    p.append(SkRasterPipeline::to_srgb);
    p.append(SkRasterPipeline::store_u16_be, &dst_ctx);
    p.run(0,0, width,1);
}

/**
 * Transform from kRGBA_F16 to 4-bytes-per-pixel RGBA.
 */
static inline void transform_scanline_F16_to_8888(char* SK_RESTRICT dst,
                                                  const char* SK_RESTRICT src, int width, int,
                                                  const SkPMColor*) {
    SkJumper_MemoryCtx src_ctx = { (void*)src, 0 },
                       dst_ctx = { (void*)dst, 0 };
    SkRasterPipeline_<256> p;
    p.append(SkRasterPipeline::load_f16, &src_ctx);
    p.append(SkRasterPipeline::clamp_0);  // F16 values may be out of [0,1] range, so clamp.
    p.append(SkRasterPipeline::clamp_1);
    p.append(SkRasterPipeline::to_srgb);
    p.append(SkRasterPipeline::store_8888, &dst_ctx);
    p.run(0,0, width,1);
=======
static inline void transform_scanline_F32(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_RGBA_ffff,       skcms_AlphaFormat_Unpremul,
          skcms_PixelFormat_RGBA_16161616BE, skcms_AlphaFormat_Unpremul);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/**
 * Transform from kPremul, kRGBA_F16 to 4-bytes-per-pixel RGBA.
 */
static inline void transform_scanline_F16_premul_to_8888(char* SK_RESTRICT dst,
                                                         const char* SK_RESTRICT src, int width,
                                                         int, const SkPMColor*) {
    SkJumper_MemoryCtx src_ctx = { (void*)src, 0 },
                       dst_ctx = { (void*)dst, 0 };
    SkRasterPipeline_<256> p;
    p.append(SkRasterPipeline::load_f16, &src_ctx);
    p.append(SkRasterPipeline::unpremul);
    p.append(SkRasterPipeline::clamp_0);  // F16 values may be out of [0,1] range, so clamp.
    p.append(SkRasterPipeline::clamp_1);
    p.append(SkRasterPipeline::store_8888, &dst_ctx);
    p.run(0,0, width,1);
}

/**
 * Transform from kUnpremul, kRGBA_F16 to premultiplied rgbA 8888.
 */
static inline void transform_scanline_F16_to_premul_8888(char* SK_RESTRICT dst,
        const char* SK_RESTRICT src, int width, int, const SkPMColor*) {
    SkJumper_MemoryCtx src_ctx = { (void*)src, 0 },
                       dst_ctx = { (void*)dst, 0 };
    SkRasterPipeline_<256> p;
    p.append(SkRasterPipeline::load_f16, &src_ctx);
    p.append(SkRasterPipeline::clamp_0);  // F16 values may be out of [0,1] range, so clamp.
    p.append(SkRasterPipeline::clamp_1);
    p.append(SkRasterPipeline::premul);
    p.append(SkRasterPipeline::store_8888, &dst_ctx);
    p.run(0,0, width,1);
||||||| merged common ancestors
/**
 * Transform from kPremul, kRGBA_F16 to 4-bytes-per-pixel RGBA.
 */
static inline void transform_scanline_F16_premul_to_8888(char* SK_RESTRICT dst,
                                                         const char* SK_RESTRICT src, int width,
                                                         int, const SkPMColor*) {
    SkJumper_MemoryCtx src_ctx = { (void*)src, 0 },
                       dst_ctx = { (void*)dst, 0 };
    SkRasterPipeline_<256> p;
    p.append(SkRasterPipeline::load_f16, &src_ctx);
    p.append(SkRasterPipeline::unpremul);
    p.append(SkRasterPipeline::clamp_0);  // F16 values may be out of [0,1] range, so clamp.
    p.append(SkRasterPipeline::clamp_1);
    p.append(SkRasterPipeline::to_srgb);
    p.append(SkRasterPipeline::store_8888, &dst_ctx);
    p.run(0,0, width,1);
}

/**
 * Transform from kUnpremul, kRGBA_F16 to premultiplied rgbA 8888.
 */
static inline void transform_scanline_F16_to_premul_8888(char* SK_RESTRICT dst,
        const char* SK_RESTRICT src, int width, int, const SkPMColor*) {
    SkJumper_MemoryCtx src_ctx = { (void*)src, 0 },
                       dst_ctx = { (void*)dst, 0 };
    SkRasterPipeline_<256> p;
    p.append(SkRasterPipeline::load_f16, &src_ctx);
    p.append(SkRasterPipeline::clamp_0);  // F16 values may be out of [0,1] range, so clamp.
    p.append(SkRasterPipeline::clamp_1);
    p.append(SkRasterPipeline::premul);
    p.append(SkRasterPipeline::to_srgb);
    p.append(SkRasterPipeline::store_8888, &dst_ctx);
    p.run(0,0, width,1);
=======
static inline void transform_scanline_F32_premul(char* dst, const char* src, int width, int) {
    skcms(dst, src, width,
          skcms_PixelFormat_RGBA_ffff,       skcms_AlphaFormat_PremulAsEncoded,
          skcms_PixelFormat_RGBA_16161616BE, skcms_AlphaFormat_Unpremul);
>>>>>>> upstream-releases
}

/**
 * Transform from kRGBA_F32 to 8-bytes-per-pixel RGBA.
 */
static inline void transform_scanline_F32(char* SK_RESTRICT dst, const char* SK_RESTRICT src,
                                          int width, int, const SkPMColor*) {
    SkJumper_MemoryCtx src_ctx = { (void*)src, 0 },
                       dst_ctx = { (void*)dst, 0 };
    SkRasterPipeline_<256> p;
    p.append(SkRasterPipeline::load_f32, &src_ctx);
    p.append(SkRasterPipeline::clamp_0);  // F32 values may be out of [0,1] range, so clamp.
    p.append(SkRasterPipeline::clamp_1);
    p.append(SkRasterPipeline::store_u16_be, &dst_ctx);
    p.run(0,0, width,1);
}

/**
 * Transform from kPremul, kRGBA_F32 to 8-bytes-per-pixel RGBA.
 */
static inline void transform_scanline_F32_premul(char* SK_RESTRICT dst, const char* SK_RESTRICT src,
                                                 int width, int, const SkPMColor*) {
    SkJumper_MemoryCtx src_ctx = { (void*)src, 0 },
                       dst_ctx = { (void*)dst, 0 };
    SkRasterPipeline_<256> p;
    p.append(SkRasterPipeline::load_f32, &src_ctx);
    p.append(SkRasterPipeline::unpremul);
    p.append(SkRasterPipeline::clamp_0);  // F32 values may be out of [0,1] range, so clamp.
    p.append(SkRasterPipeline::clamp_1);
    p.append(SkRasterPipeline::store_u16_be, &dst_ctx);
    p.run(0,0, width,1);
}

static inline sk_sp<SkData> icc_from_color_space(const SkImageInfo& info) {
    SkColorSpace* cs = info.colorSpace();
    if (!cs) {
        return nullptr;
    }

<<<<<<< HEAD
    SkColorSpaceTransferFn fn;
    SkMatrix44 toXYZD50;
||||||| merged common ancestors
    sk_sp<SkColorSpace> owned;
    if (kRGBA_F16_SkColorType == info.colorType()) {
        owned = cs->makeSRGBGamma();
        cs = owned.get();
    }

    SkColorSpaceTransferFn fn;
    SkMatrix44 toXYZD50(SkMatrix44::kUninitialized_Constructor);
=======
    skcms_TransferFunction fn;
    skcms_Matrix3x3 toXYZD50;
>>>>>>> upstream-releases
    if (cs->isNumericalTransferFn(&fn) && cs->toXYZD50(&toXYZD50)) {
        return SkWriteICCProfile(fn, toXYZD50);
    }
    return nullptr;
}

#endif  // SkImageEncoderFns_DEFINED
