/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkColorFilter.h"
#include "SkPaintPriv.h"
#include "SkPaint.h"
#include "SkShaderBase.h"
<<<<<<< HEAD
#include "SkUTF.h"
||||||| merged common ancestors
#include "SkUtils.h"
=======
>>>>>>> upstream-releases
#include "SkXfermodePriv.h"

static bool changes_alpha(const SkPaint& paint) {
    SkColorFilter* cf = paint.getColorFilter();
    return cf && !(cf->getFlags() & SkColorFilter::kAlphaUnchanged_Flag);
}

bool SkPaintPriv::Overwrites(const SkPaint* paint, ShaderOverrideOpacity overrideOpacity) {
    if (!paint) {
        // No paint means we default to SRC_OVER, so we overwrite iff our shader-override
        // is opaque, or we don't have one.
        return overrideOpacity != kNotOpaque_ShaderOverrideOpacity;
    }

    SkXfermode::SrcColorOpacity opacityType = SkXfermode::kUnknown_SrcColorOpacity;

    if (!changes_alpha(*paint)) {
        const unsigned paintAlpha = paint->getAlpha();
        if (0xff == paintAlpha && overrideOpacity != kNotOpaque_ShaderOverrideOpacity &&
            (!paint->getShader() || paint->getShader()->isOpaque()))
        {
            opacityType = SkXfermode::kOpaque_SrcColorOpacity;
        } else if (0 == paintAlpha) {
            if (overrideOpacity == kNone_ShaderOverrideOpacity && !paint->getShader()) {
                opacityType = SkXfermode::kTransparentBlack_SrcColorOpacity;
            } else {
                opacityType = SkXfermode::kTransparentAlpha_SrcColorOpacity;
            }
        }
    }

    return SkXfermode::IsOpaque(paint->getBlendMode(), opacityType);
}

bool SkPaintPriv::ShouldDither(const SkPaint& p, SkColorType dstCT) {
    // The paint dither flag can veto.
    if (!p.isDither()) {
        return false;
    }

    // We always dither 565 or 4444 when requested.
    if (dstCT == kRGB_565_SkColorType || dstCT == kARGB_4444_SkColorType) {
        return true;
    }

    // Otherwise, dither is only needed for non-const paints.
    return p.getImageFilter() || p.getMaskFilter()
        || !p.getShader() || !as_SB(p.getShader())->isConstant();
}

<<<<<<< HEAD
int SkPaintPriv::ValidCountText(const void* text, size_t length, SkPaint::TextEncoding encoding) {
    switch (encoding) {
        case SkPaint::kUTF8_TextEncoding: return SkUTF::CountUTF8((const char*)text, length);
        case SkPaint::kUTF16_TextEncoding: return SkUTF::CountUTF16((const uint16_t*)text, length);
        case SkPaint::kUTF32_TextEncoding: return SkUTF::CountUTF32((const int32_t*)text, length);
        case SkPaint::kGlyphID_TextEncoding:
            if (!SkIsAlign2(intptr_t(text)) || !SkIsAlign2(length)) {
                return -1;
            }
            return length >> 1;
||||||| merged common ancestors
int SkPaintPriv::ValidCountText(const void* text, size_t length, SkPaint::TextEncoding encoding) {
    if (length == 0) {
        return 0;
    }
    switch (encoding) {
        case SkPaint::kUTF8_TextEncoding: return SkUTF8_CountUnichars(text, length);
        case SkPaint::kUTF16_TextEncoding: return SkUTF16_CountUnichars(text, length);
        case SkPaint::kUTF32_TextEncoding: return SkUTF32_CountUnichars(text, length);
        case SkPaint::kGlyphID_TextEncoding:
            if (SkIsAlign2(intptr_t(text)) && SkIsAlign2(length)) {
                return length >> 1;
            }
            break;
=======
// return true if the paint is just a single color (i.e. not a shader). If its
// a shader, then we can't compute a const luminance for it :(
static bool just_a_color(const SkPaint& paint, SkColor* color) {
    SkColor c = paint.getColor();

    const auto* shader = as_SB(paint.getShader());
    if (shader && !shader->asLuminanceColor(&c)) {
        return false;
    }
    if (paint.getColorFilter()) {
        c = paint.getColorFilter()->filterColor(c);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    return -1;
||||||| merged common ancestors
    return 0;
=======
    if (color) {
        *color = c;
    }
    return true;
>>>>>>> upstream-releases
}

SkColor SkPaintPriv::ComputeLuminanceColor(const SkPaint& paint) {
    SkColor c;
    if (!just_a_color(paint, &c)) {
        c = SkColorSetRGB(0x7F, 0x80, 0x7F);
    }
    return c;
}
