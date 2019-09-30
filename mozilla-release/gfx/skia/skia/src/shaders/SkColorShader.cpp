/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkArenaAlloc.h"
#include "SkColorShader.h"
#include "SkColorSpace.h"
#include "SkColorSpacePriv.h"
#include "SkColorSpaceXformSteps.h"
#include "SkRasterPipeline.h"
#include "SkReadBuffer.h"
#include "SkUtils.h"

SkColorShader::SkColorShader(SkColor c) : fColor(c) {}

bool SkColorShader::isOpaque() const {
    return SkColorGetA(fColor) == 255;
}

sk_sp<SkFlattenable> SkColorShader::CreateProc(SkReadBuffer& buffer) {
    return sk_make_sp<SkColorShader>(buffer.readColor());
}

void SkColorShader::flatten(SkWriteBuffer& buffer) const {
    buffer.writeColor(fColor);
}

<<<<<<< HEAD
uint32_t SkColorShader::ColorShaderContext::getFlags() const {
    return fFlags;
}

SkShaderBase::Context* SkColorShader::onMakeContext(const ContextRec& rec,
                                                    SkArenaAlloc* alloc) const {
    return alloc->make<ColorShaderContext>(*this, rec);
}

SkColorShader::ColorShaderContext::ColorShaderContext(const SkColorShader& shader,
                                                      const ContextRec& rec)
    : INHERITED(shader, rec)
{
    SkColor color = shader.fColor;
    unsigned a = SkAlphaMul(SkColorGetA(color), SkAlpha255To256(rec.fPaint->getAlpha()));

    unsigned r = SkColorGetR(color);
    unsigned g = SkColorGetG(color);
    unsigned b = SkColorGetB(color);

    if (a != 255) {
        r = SkMulDiv255Round(r, a);
        g = SkMulDiv255Round(g, a);
        b = SkMulDiv255Round(b, a);
    }
    fPMColor = SkPackARGB32(a, r, g, b);

    SkColor4f c4 = SkColor4f::FromColor(shader.fColor);
    c4.fA *= rec.fPaint->getAlpha() / 255.0f;
    fPMColor4f = c4.premul();

    fFlags = kConstInY32_Flag;
    if (255 == a) {
        fFlags |= kOpaqueAlpha_Flag;
    }
}

void SkColorShader::ColorShaderContext::shadeSpan(int x, int y, SkPMColor span[], int count) {
    sk_memset32(span, fPMColor, count);
}

void SkColorShader::ColorShaderContext::shadeSpan4f(int x, int y, SkPMColor4f span[], int count) {
    for (int i = 0; i < count; ++i) {
        span[i] = fPMColor4f;
    }
}

||||||| merged common ancestors
uint32_t SkColorShader::ColorShaderContext::getFlags() const {
    return fFlags;
}

SkShaderBase::Context* SkColorShader::onMakeContext(const ContextRec& rec,
                                                    SkArenaAlloc* alloc) const {
    return alloc->make<ColorShaderContext>(*this, rec);
}

SkColorShader::ColorShaderContext::ColorShaderContext(const SkColorShader& shader,
                                                      const ContextRec& rec)
    : INHERITED(shader, rec)
{
    SkColor color = shader.fColor;
    unsigned a = SkAlphaMul(SkColorGetA(color), SkAlpha255To256(rec.fPaint->getAlpha()));

    unsigned r = SkColorGetR(color);
    unsigned g = SkColorGetG(color);
    unsigned b = SkColorGetB(color);

    if (a != 255) {
        r = SkMulDiv255Round(r, a);
        g = SkMulDiv255Round(g, a);
        b = SkMulDiv255Round(b, a);
    }
    fPMColor = SkPackARGB32(a, r, g, b);

    SkColor4f c4 = SkColor4f::FromColor(shader.fColor);
    c4.fA *= rec.fPaint->getAlpha() / 255.0f;
    fPM4f = c4.premul();

    fFlags = kConstInY32_Flag;
    if (255 == a) {
        fFlags |= kOpaqueAlpha_Flag;
    }
}

void SkColorShader::ColorShaderContext::shadeSpan(int x, int y, SkPMColor span[], int count) {
    sk_memset32(span, fPMColor, count);
}

void SkColorShader::ColorShaderContext::shadeSpan4f(int x, int y, SkPM4f span[], int count) {
    for (int i = 0; i < count; ++i) {
        span[i] = fPM4f;
    }
}

=======
>>>>>>> upstream-releases
SkShader::GradientType SkColorShader::asAGradient(GradientInfo* info) const {
    if (info) {
        if (info->fColors && info->fColorCount >= 1) {
            info->fColors[0] = fColor;
        }
        info->fColorCount = 1;
        info->fTileMode = SkShader::kRepeat_TileMode;
    }
    return kColor_GradientType;
}

<<<<<<< HEAD
#if SK_SUPPORT_GPU

#include "SkGr.h"
#include "effects/GrConstColorProcessor.h"
std::unique_ptr<GrFragmentProcessor> SkColorShader::asFragmentProcessor(
        const GrFPArgs& args) const {
    SkPMColor4f color = SkColorToPMColor4f(fColor, *args.fDstColorSpaceInfo);
    return GrConstColorProcessor::Make(color, GrConstColorProcessor::InputMode::kModulateA);
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

static unsigned unit_to_byte(float unit) {
    SkASSERT(unit >= 0 && unit <= 1);
    return (unsigned)(unit * 255 + 0.5);
}

static SkColor to_skcolor(SkColor4f color, SkColorSpace* cs) {
    SkColorSpaceXformSteps steps{cs                 , kUnpremul_SkAlphaType,
                                 sk_srgb_singleton(), kUnpremul_SkAlphaType};
    steps.apply(color.vec());
    color = color.pin();
    return SkColorSetARGB(unit_to_byte(color.fA), unit_to_byte(color.fR),
                          unit_to_byte(color.fG), unit_to_byte(color.fB));
}

||||||| merged common ancestors
#if SK_SUPPORT_GPU

#include "SkGr.h"
#include "effects/GrConstColorProcessor.h"
std::unique_ptr<GrFragmentProcessor> SkColorShader::asFragmentProcessor(
        const GrFPArgs& args) const {
    GrColor4f color = SkColorToPremulGrColor4f(fColor, *args.fDstColorSpaceInfo);
    return GrConstColorProcessor::Make(color, GrConstColorProcessor::InputMode::kModulateA);
}

#endif

#ifndef SK_IGNORE_TO_STRING
void SkColorShader::toString(SkString* str) const {
    str->append("SkColorShader: (");

    str->append("Color: ");
    str->appendHex(fColor);

    this->INHERITED::toString(str);

    str->append(")");
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

static unsigned unit_to_byte(float unit) {
    SkASSERT(unit >= 0 && unit <= 1);
    return (unsigned)(unit * 255 + 0.5);
}

static SkColor unit_to_skcolor(const SkColor4f& unit, SkColorSpace* cs) {
    return SkColorSetARGB(unit_to_byte(unit.fA), unit_to_byte(unit.fR),
                          unit_to_byte(unit.fG), unit_to_byte(unit.fB));
}

=======
>>>>>>> upstream-releases
SkColor4Shader::SkColor4Shader(const SkColor4f& color, sk_sp<SkColorSpace> space)
    : fColorSpace(std::move(space))
<<<<<<< HEAD
    , fColor4(color)
    , fCachedByteColor(to_skcolor(color, fColorSpace.get()))
||||||| merged common ancestors
    , fColor4(color)
    , fCachedByteColor(unit_to_skcolor(color.pin(), space.get()))
=======
    , fColor(color)
>>>>>>> upstream-releases
{}

sk_sp<SkFlattenable> SkColor4Shader::CreateProc(SkReadBuffer& buffer) {
    SkColor4f color;
    sk_sp<SkColorSpace> colorSpace;
    buffer.readColor4f(&color);
    if (buffer.readBool()) {
        sk_sp<SkData> data = buffer.readByteArrayAsData();
        colorSpace = data ? SkColorSpace::Deserialize(data->data(), data->size()) : nullptr;
    }
    return SkShader::MakeColorShader(color, std::move(colorSpace));
}

void SkColor4Shader::flatten(SkWriteBuffer& buffer) const {
<<<<<<< HEAD
    buffer.writeColor4f(fColor4);
    sk_sp<SkData> colorSpaceData = fColorSpace ? fColorSpace->serialize() : nullptr;
    if (colorSpaceData) {
        buffer.writeBool(true);
        buffer.writeDataAsByteArray(colorSpaceData.get());
    } else {
        buffer.writeBool(false);
    }
||||||| merged common ancestors
    buffer.writeColor4f(fColor4);
    buffer.writeBool(false);    // TODO how do we flatten colorspaces?
=======
    buffer.writeColor4f(fColor);
    sk_sp<SkData> colorSpaceData = fColorSpace ? fColorSpace->serialize() : nullptr;
    if (colorSpaceData) {
        buffer.writeBool(true);
        buffer.writeDataAsByteArray(colorSpaceData.get());
    } else {
        buffer.writeBool(false);
    }
>>>>>>> upstream-releases
}


sk_sp<SkShader> SkColor4Shader::onMakeColorSpace(SkColorSpaceXformer* xformer) const {
    SkColor4f color = fColor;
    SkColorSpaceXformSteps(fColorSpace.get(),    kUnpremul_SkAlphaType,
                           xformer->dst().get(), kUnpremul_SkAlphaType).apply(color.vec());
    return SkShader::MakeColorShader(color.toSkColor());
}

<<<<<<< HEAD
SkColor4Shader::Color4Context::Color4Context(const SkColor4Shader& shader,
                                                      const ContextRec& rec)
: INHERITED(shader, rec)
{
    SkColor color = shader.fCachedByteColor;
    unsigned a = SkAlphaMul(SkColorGetA(color), SkAlpha255To256(rec.fPaint->getAlpha()));

    unsigned r = SkColorGetR(color);
    unsigned g = SkColorGetG(color);
    unsigned b = SkColorGetB(color);

    if (a != 255) {
        r = SkMulDiv255Round(r, a);
        g = SkMulDiv255Round(g, a);
        b = SkMulDiv255Round(b, a);
    }
    fPMColor = SkPackARGB32(a, r, g, b);

    SkColor4f c4 = shader.fColor4;
    c4.fA *= rec.fPaint->getAlpha() * (1 / 255.0f);
    fPMColor4f = c4.premul();

    fFlags = kConstInY32_Flag;
    if (255 == a) {
        fFlags |= kOpaqueAlpha_Flag;
||||||| merged common ancestors
SkColor4Shader::Color4Context::Color4Context(const SkColor4Shader& shader,
                                                      const ContextRec& rec)
: INHERITED(shader, rec)
{
    SkColor color = shader.fCachedByteColor;
    unsigned a = SkAlphaMul(SkColorGetA(color), SkAlpha255To256(rec.fPaint->getAlpha()));

    unsigned r = SkColorGetR(color);
    unsigned g = SkColorGetG(color);
    unsigned b = SkColorGetB(color);

    if (a != 255) {
        r = SkMulDiv255Round(r, a);
        g = SkMulDiv255Round(g, a);
        b = SkMulDiv255Round(b, a);
    }
    fPMColor = SkPackARGB32(a, r, g, b);

    SkColor4f c4 = shader.fColor4;
    c4.fA *= rec.fPaint->getAlpha() * (1 / 255.0f);
    fPM4f = c4.premul();

    fFlags = kConstInY32_Flag;
    if (255 == a) {
        fFlags |= kOpaqueAlpha_Flag;
=======
sk_sp<SkShader> SkShader::MakeColorShader(const SkColor4f& color, sk_sp<SkColorSpace> space) {
    if (!SkScalarsAreFinite(color.vec(), 4)) {
        return nullptr;
>>>>>>> upstream-releases
    }
    return sk_make_sp<SkColor4Shader>(color, std::move(space));
}

<<<<<<< HEAD
void SkColor4Shader::Color4Context::shadeSpan(int x, int y, SkPMColor span[], int count) {
    sk_memset32(span, fPMColor, count);
}

void SkColor4Shader::Color4Context::shadeSpan4f(int x, int y, SkPMColor4f span[], int count) {
    for (int i = 0; i < count; ++i) {
        span[i] = fPMColor4f;
    }
||||||| merged common ancestors
void SkColor4Shader::Color4Context::shadeSpan(int x, int y, SkPMColor span[], int count) {
    sk_memset32(span, fPMColor, count);
}

void SkColor4Shader::Color4Context::shadeSpan4f(int x, int y, SkPM4f span[], int count) {
    for (int i = 0; i < count; ++i) {
        span[i] = fPM4f;
    }
=======
bool SkColorShader::onAppendStages(const StageRec& rec) const {
    SkColor4f color = SkColor4f::FromColor(fColor);
    SkColorSpaceXformSteps(sk_srgb_singleton(), kUnpremul_SkAlphaType,
                           rec.fDstCS,          kUnpremul_SkAlphaType).apply(color.vec());
    rec.fPipeline->append_constant_color(rec.fAlloc, color.premul().vec());
    return true;
>>>>>>> upstream-releases
}

bool SkColor4Shader::onAppendStages(const StageRec& rec) const {
    SkColor4f color = fColor;
    SkColorSpaceXformSteps(fColorSpace.get(), kUnpremul_SkAlphaType,
                           rec.fDstCS,        kUnpremul_SkAlphaType).apply(color.vec());
    rec.fPipeline->append_constant_color(rec.fAlloc, color.premul().vec());
    return true;
}

#if SK_SUPPORT_GPU

#include "GrColorSpaceInfo.h"
#include "GrColorSpaceXform.h"
#include "SkGr.h"
#include "effects/GrConstColorProcessor.h"

std::unique_ptr<GrFragmentProcessor> SkColorShader::asFragmentProcessor(
        const GrFPArgs& args) const {
    SkPMColor4f color = SkColorToPMColor4f(fColor, *args.fDstColorSpaceInfo);
    return GrConstColorProcessor::Make(color, GrConstColorProcessor::InputMode::kModulateA);
}

std::unique_ptr<GrFragmentProcessor> SkColor4Shader::asFragmentProcessor(
        const GrFPArgs& args) const {
<<<<<<< HEAD
    SkColorSpaceXformSteps steps{ fColorSpace.get(),                     kUnpremul_SkAlphaType,
                                  args.fDstColorSpaceInfo->colorSpace(), kUnpremul_SkAlphaType };
    SkColor4f color = fColor4;
    steps.apply(color.vec());
||||||| merged common ancestors
    // Construct an xform assuming float inputs. The color space can have a transfer function on
    // it, which will be applied below.
    auto colorSpaceXform = GrColorSpaceXform::Make(fColorSpace.get(), kRGBA_float_GrPixelConfig,
                                                   args.fDstColorSpaceInfo->colorSpace());
    GrColor4f color = GrColor4f::FromSkColor4f(fColor4);
    if (colorSpaceXform) {
        color = colorSpaceXform->clampedXform(color);
    }
=======
    SkColorSpaceXformSteps steps{ fColorSpace.get(),                     kUnpremul_SkAlphaType,
                                  args.fDstColorSpaceInfo->colorSpace(), kUnpremul_SkAlphaType };
    SkColor4f color = fColor;
    steps.apply(color.vec());
>>>>>>> upstream-releases
    return GrConstColorProcessor::Make(color.premul(),
                                       GrConstColorProcessor::InputMode::kModulateA);
}

#endif
<<<<<<< HEAD

sk_sp<SkShader> SkColor4Shader::onMakeColorSpace(SkColorSpaceXformer* xformer) const {
    return SkShader::MakeColorShader(xformer->apply(fCachedByteColor));
}

sk_sp<SkShader> SkShader::MakeColorShader(const SkColor4f& color, sk_sp<SkColorSpace> space) {
    if (!SkScalarsAreFinite(color.vec(), 4)) {
        return nullptr;
    }
    return sk_make_sp<SkColor4Shader>(color, std::move(space));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool SkColorShader::onAppendStages(const StageRec& rec) const {
    rec.fPipeline->append_constant_color(rec.fAlloc,
            premul_in_dst_colorspace(fColor, rec.fDstCS));
    return true;
}

bool SkColor4Shader::onAppendStages(const StageRec& rec) const {
    rec.fPipeline->append_constant_color(rec.fAlloc,
            premul_in_dst_colorspace(fColor4, fColorSpace.get(), rec.fDstCS));
    return true;
}
||||||| merged common ancestors

#ifndef SK_IGNORE_TO_STRING
void SkColor4Shader::toString(SkString* str) const {
    str->append("SkColor4Shader: (");

    str->append("RGBA:");
    for (int i = 0; i < 4; ++i) {
        str->appendf(" %g", fColor4.vec()[i]);
    }
    str->append(" )");
}
#endif

sk_sp<SkShader> SkColor4Shader::onMakeColorSpace(SkColorSpaceXformer* xformer) const {
    return SkShader::MakeColorShader(xformer->apply(fCachedByteColor));
}

sk_sp<SkShader> SkShader::MakeColorShader(const SkColor4f& color, sk_sp<SkColorSpace> space) {
    if (!SkScalarsAreFinite(color.vec(), 4)) {
        return nullptr;
    }
    return sk_make_sp<SkColor4Shader>(color, std::move(space));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool SkColorShader::onAppendStages(const StageRec& rec) const {
    rec.fPipeline->append_constant_color(rec.fAlloc, SkPM4f_from_SkColor(fColor, rec.fDstCS));
    return true;
}

bool SkColor4Shader::onAppendStages(const StageRec& rec) const {
    rec.fPipeline->append_constant_color(
                     rec.fAlloc, to_colorspace(fColor4, fColorSpace.get(), rec.fDstCS).premul());
    return true;
}
=======
>>>>>>> upstream-releases
