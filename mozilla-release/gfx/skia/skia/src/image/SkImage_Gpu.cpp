/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

<<<<<<< HEAD
#include "GrAHardwareBufferImageGenerator.h"
||||||| merged common ancestors
#include "SkAutoPixmapStorage.h"
=======
#include <cstddef>
#include <cstring>
#include <type_traits>

#include "GrAHardwareBufferImageGenerator.h"
#include "GrAHardwareBufferUtils.h"
>>>>>>> upstream-releases
#include "GrBackendSurface.h"
#include "GrBackendTextureImageGenerator.h"
#include "GrBitmapTextureMaker.h"
#include "GrCaps.h"
#include "GrClip.h"
#include "GrColorSpaceXform.h"
#include "GrContext.h"
#include "GrContextPriv.h"
#include "GrDrawingManager.h"
#include "GrGpu.h"
#include "GrImageTextureMaker.h"
#include "GrProxyProvider.h"
#include "GrRecordingContext.h"
#include "GrRecordingContextPriv.h"
#include "GrRenderTargetContext.h"
#include "GrResourceProvider.h"
#include "GrResourceProviderPriv.h"
#include "GrSemaphore.h"
#include "GrSurfacePriv.h"
#include "GrTexture.h"
<<<<<<< HEAD
#include "GrTextureAdjuster.h"
||||||| merged common ancestors
=======
#include "GrTextureAdjuster.h"
#include "GrTextureContext.h"
>>>>>>> upstream-releases
#include "GrTexturePriv.h"
#include "GrTextureProxy.h"
#include "GrTextureProxyPriv.h"
#include "SkAutoPixmapStorage.h"
#include "SkBitmapCache.h"
#include "SkCanvas.h"
#include "SkGr.h"
#include "SkImageInfoPriv.h"
#include "SkImage_Gpu.h"
#include "SkMipMap.h"
<<<<<<< HEAD
#include "SkPixelRef.h"
||||||| merged common ancestors
#include "SkPixelRef.h"
#include "SkReadPixelsRec.h"
=======
#include "SkScopeExit.h"
>>>>>>> upstream-releases
#include "SkTraceEvent.h"
<<<<<<< HEAD
#include "SkYUVAIndex.h"
#include "effects/GrYUVtoRGBEffect.h"
#include "gl/GrGLTexture.h"
||||||| merged common ancestors
=======
#include "effects/GrYUVtoRGBEffect.h"
#include "gl/GrGLTexture.h"
>>>>>>> upstream-releases

<<<<<<< HEAD
SkImage_Gpu::SkImage_Gpu(sk_sp<GrContext> context, uint32_t uniqueID, SkAlphaType at,
                         sk_sp<GrTextureProxy> proxy, sk_sp<SkColorSpace> colorSpace,
                         SkBudgeted budgeted)
        : INHERITED(std::move(context), proxy->worstCaseWidth(), proxy->worstCaseHeight(), uniqueID,
                    at, budgeted, colorSpace)
        , fProxy(std::move(proxy)) {}
||||||| merged common ancestors
SkImage_Gpu::SkImage_Gpu(GrContext* context, uint32_t uniqueID, SkAlphaType at,
                         sk_sp<GrTextureProxy> proxy,
                         sk_sp<SkColorSpace> colorSpace, SkBudgeted budgeted)
    : INHERITED(proxy->worstCaseWidth(), proxy->worstCaseHeight(), uniqueID)
    , fContext(context)
    , fProxy(std::move(proxy))
    , fAlphaType(at)
    , fBudgeted(budgeted)
    , fColorSpace(std::move(colorSpace))
    , fAddedRasterVersionToCache(false) {
}
=======
SkImage_Gpu::SkImage_Gpu(sk_sp<GrContext> context, uint32_t uniqueID, SkAlphaType at,
                         sk_sp<GrTextureProxy> proxy, sk_sp<SkColorSpace> colorSpace)
        : INHERITED(std::move(context), proxy->worstCaseWidth(), proxy->worstCaseHeight(), uniqueID,
                    at, colorSpace)
        , fProxy(std::move(proxy)) {}
>>>>>>> upstream-releases

SkImage_Gpu::~SkImage_Gpu() {}

SkImageInfo SkImage_Gpu::onImageInfo() const {
    SkColorType colorType;
    if (!GrPixelConfigToColorType(fProxy->config(), &colorType)) {
        colorType = kUnknown_SkColorType;
    }

<<<<<<< HEAD
    return SkImageInfo::Make(fProxy->width(), fProxy->height(), colorType, fAlphaType, fColorSpace);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

static sk_sp<SkImage> new_wrapped_texture_common(GrContext* ctx,
                                                 const GrBackendTexture& backendTex,
                                                 GrSurfaceOrigin origin,
                                                 SkAlphaType at, sk_sp<SkColorSpace> colorSpace,
                                                 GrWrapOwnership ownership,
                                                 SkImage::TextureReleaseProc releaseProc,
                                                 SkImage::ReleaseContext releaseCtx) {
    if (!backendTex.isValid() || backendTex.width() <= 0 || backendTex.height() <= 0) {
        return nullptr;
    }

    GrProxyProvider* proxyProvider = ctx->contextPriv().proxyProvider();
    sk_sp<GrTextureProxy> proxy = proxyProvider->wrapBackendTexture(backendTex, origin, ownership,
                                                                    releaseProc, releaseCtx);
    if (!proxy) {
        return nullptr;
    }
    return sk_make_sp<SkImage_Gpu>(sk_ref_sp(ctx), kNeedNewImageUniqueID, at, std::move(proxy),
                                   std::move(colorSpace), SkBudgeted::kNo);
||||||| merged common ancestors
bool SkImage_Gpu::getROPixels(SkBitmap* dst, SkColorSpace*, CachingHint chint) const {
    // The SkColorSpace parameter "dstColorSpace" is really just a hint about how/where the bitmap
    // will be used. The client doesn't expect that we convert to that color space, it's intended
    // for codec-backed images, to drive our decoding heuristic. In theory we *could* read directly
    // into that color space (to save the client some effort in whatever they're about to do), but
    // that would make our use of the bitmap cache incorrect (or much less efficient, assuming we
    // rolled the dstColorSpace into the key).
    const auto desc = SkBitmapCacheDesc::Make(this);
    if (SkBitmapCache::Find(desc, dst)) {
        SkASSERT(dst->getGenerationID() == this->uniqueID());
        SkASSERT(dst->isImmutable());
        SkASSERT(dst->getPixels());
        return true;
    }

    SkBitmapCache::RecPtr rec = nullptr;
    SkPixmap pmap;
    if (kAllow_CachingHint == chint) {
        rec = SkBitmapCache::Alloc(desc, this->onImageInfo(), &pmap);
        if (!rec) {
            return false;
        }
    } else {
        if (!dst->tryAllocPixels(this->onImageInfo()) || !dst->peekPixels(&pmap)) {
            return false;
        }
    }

    sk_sp<GrSurfaceContext> sContext = fContext->contextPriv().makeWrappedSurfaceContext(
                                                                                    fProxy,
                                                                                    fColorSpace);
    if (!sContext) {
        return false;
    }

    if (!sContext->readPixels(pmap.info(), pmap.writable_addr(), pmap.rowBytes(), 0, 0)) {
        return false;
    }

    if (rec) {
        SkBitmapCache::Add(std::move(rec), dst);
        fAddedRasterVersionToCache.store(true);
    }
    return true;
=======
    return SkImageInfo::Make(fProxy->width(), fProxy->height(), colorType, fAlphaType, fColorSpace);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
sk_sp<SkImage> SkImage::MakeFromTexture(GrContext* ctx,
                                        const GrBackendTexture& tex, GrSurfaceOrigin origin,
                                        SkColorType ct, SkAlphaType at, sk_sp<SkColorSpace> cs,
                                        TextureReleaseProc releaseP, ReleaseContext releaseC) {
    if (!ctx) {
||||||| merged common ancestors
sk_sp<GrTextureProxy> SkImage_Gpu::asTextureProxyRef(GrContext* context,
                                                     const GrSamplerState& params,
                                                     SkColorSpace* dstColorSpace,
                                                     sk_sp<SkColorSpace>* texColorSpace,
                                                     SkScalar scaleAdjust[2]) const {
    if (context != fContext) {
        SkASSERT(0);
=======
sk_sp<SkImage> SkImage_Gpu::onMakeColorTypeAndColorSpace(GrRecordingContext* context,
                                                         SkColorType targetCT,
                                                         sk_sp<SkColorSpace> targetCS) const {
    if (!context || !fContext->priv().matches(context)) {
>>>>>>> upstream-releases
        return nullptr;
    }
<<<<<<< HEAD
    GrBackendTexture texCopy = tex;
    if (!SkImage_GpuBase::ValidateBackendTexture(ctx, texCopy, &texCopy.fConfig, ct, at, cs)) {
        return nullptr;
    }
    return new_wrapped_texture_common(ctx, texCopy, origin, at, std::move(cs),
                                      kBorrow_GrWrapOwnership, releaseP, releaseC);
}

sk_sp<SkImage> SkImage::MakeFromAdoptedTexture(GrContext* ctx,
                                               const GrBackendTexture& tex, GrSurfaceOrigin origin,
                                               SkColorType ct, SkAlphaType at,
                                               sk_sp<SkColorSpace> cs) {
    if (!ctx || !ctx->contextPriv().resourceProvider()) {
        // We have a DDL context and we don't support adopted textures for them.
        return nullptr;
    }
    GrBackendTexture texCopy = tex;
    if (!SkImage_GpuBase::ValidateBackendTexture(ctx, texCopy, &texCopy.fConfig, ct, at, cs)) {
        return nullptr;
    }
    return new_wrapped_texture_common(ctx, texCopy, origin, at, std::move(cs),
                                      kAdopt_GrWrapOwnership, nullptr, nullptr);
}

sk_sp<SkImage> SkImage_Gpu::ConvertYUVATexturesToRGB(
        GrContext* ctx, SkYUVColorSpace yuvColorSpace, const GrBackendTexture yuvaTextures[],
        const SkYUVAIndex yuvaIndices[4], SkISize size, GrSurfaceOrigin origin,
        SkBudgeted isBudgeted, GrRenderTargetContext* renderTargetContext) {
    SkASSERT(renderTargetContext);

    GrProxyProvider* proxyProvider = ctx->contextPriv().proxyProvider();

    // We need to make a copy of the input backend textures because we need to preserve the result
    // of validate_backend_texture.
    GrBackendTexture yuvaTexturesCopy[4];

    bool nv12 = (yuvaIndices[1].fIndex == yuvaIndices[2].fIndex);
||||||| merged common ancestors

    if (texColorSpace) {
        *texColorSpace = this->fColorSpace;
    }

    GrTextureAdjuster adjuster(fContext, fProxy, this->alphaType(), this->uniqueID(),
                               this->fColorSpace.get());
    return adjuster.refTextureProxySafeForParams(params, scaleAdjust);
}

static void apply_premul(const SkImageInfo& info, void* pixels, size_t rowBytes) {
    switch (info.colorType()) {
        case kRGBA_8888_SkColorType:
        case kBGRA_8888_SkColorType:
            break;
        default:
            return; // nothing to do
    }

    // SkColor is not necesarily RGBA or BGRA, but it is one of them on little-endian,
    // and in either case, the alpha-byte is always in the same place, so we can safely call
    // SkPreMultiplyColor()
    //
    SkColor* row = (SkColor*)pixels;
    for (int y = 0; y < info.height(); ++y) {
        for (int x = 0; x < info.width(); ++x) {
            row[x] = SkPreMultiplyColor(row[x]);
        }
        row = (SkColor*)((char*)(row) + rowBytes);
    }
}

GrBackendObject SkImage_Gpu::onGetTextureHandle(bool flushPendingGrContextIO,
                                                GrSurfaceOrigin* origin) const {
    SkASSERT(fProxy);

    if (!fContext->contextPriv().resourceProvider() && !fProxy->priv().isInstantiated()) {
        // This image was created with a DDL context and cannot be instantiated. Thus we return 0
        // here which is considered invalid for all backends.
        return 0;
    }

    if (GrSurfaceProxy::LazyState::kNot != fProxy->lazyInstantiationState()) {
        SkASSERT(fContext->contextPriv().resourceProvider());
        fProxy->priv().doLazyInstantiation(fContext->contextPriv().resourceProvider());
        if (!fProxy->priv().isInstantiated()) {
            // We failed to instantiate the lazy proxy. Thus we return 0 here which is considered
            // invalid for all backends.
            return 0;
        }
    }

    if (!fProxy->instantiate(fContext->contextPriv().resourceProvider())) {
        return 0;
    }
=======

    auto xform = GrColorSpaceXformEffect::Make(fColorSpace.get(), fAlphaType,
                                               targetCS.get(), fAlphaType);
    SkASSERT(xform || targetCT != this->colorType());
>>>>>>> upstream-releases

<<<<<<< HEAD
    for (int i = 0; i < 4; ++i) {
        // Validate that the yuvaIndices refer to valid backend textures.
        const SkYUVAIndex& yuvaIndex = yuvaIndices[i];
        if (i == 3 && yuvaIndex.fIndex == -1) {
            // Meaning the A plane isn't passed in.
            continue;
        }
        if (yuvaIndex.fIndex == -1 || yuvaIndex.fIndex > 3) {
            // Y plane, U plane, and V plane must refer to image sources being passed in. There are
            // at most 4 images sources being passed in, could not have a index more than 3.
            return nullptr;
        }

        SkColorType ct = kUnknown_SkColorType;
        if (SkYUVAIndex::kA_Index == i) {
            // The A plane is always kAlpha8 (for now)
            ct = kAlpha_8_SkColorType;
        } else {
            // The UV planes can either be interleaved or planar. If interleaved the Y plane
            // will have RBGA color type.
            ct = nv12 ? kRGBA_8888_SkColorType : kAlpha_8_SkColorType;
        }
||||||| merged common ancestors
    GrTexture* texture = fProxy->priv().peekTexture();

    if (texture) {
        if (flushPendingGrContextIO) {
            fContext->contextPriv().prepareSurfaceForExternalIO(fProxy.get());
        }
        if (origin) {
            *origin = fProxy->origin();
        }
        return texture->getTextureHandle();
    }
    return 0;
}

GrTexture* SkImage_Gpu::onGetTexture() const {
    GrTextureProxy* proxy = this->peekProxy();
    if (!proxy) {
        return nullptr;
    }
=======
    sk_sp<GrTextureProxy> proxy = this->asTextureProxyRef(context);

    GrBackendFormat format = proxy->backendFormat().makeTexture2D();
    if (!format.isValid()) {
        return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (!yuvaTexturesCopy[yuvaIndex.fIndex].isValid()) {
            yuvaTexturesCopy[yuvaIndex.fIndex] = yuvaTextures[yuvaIndex.fIndex];
||||||| merged common ancestors
    if (!proxy->instantiate(fContext->contextPriv().resourceProvider())) {
        return nullptr;
    }
=======
    sk_sp<GrRenderTargetContext> renderTargetContext(
        context->priv().makeDeferredRenderTargetContextWithFallback(
            format, SkBackingFit::kExact, this->width(), this->height(),
            SkColorType2GrPixelConfig(targetCT), nullptr));
    if (!renderTargetContext) {
        return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
            // TODO: Instead of using assumption about whether it is NV12 format to guess colorType,
            // actually use channel information here.
            if (!ValidateBackendTexture(ctx, yuvaTexturesCopy[yuvaIndex.fIndex],
                                        &yuvaTexturesCopy[yuvaIndex.fIndex].fConfig,
                                        ct, kPremul_SkAlphaType, nullptr)) {
                return nullptr;
            }
        }

        // TODO: Check that for each plane, the channel actually exist in the image source we are
        // reading from.
    }

    sk_sp<GrTextureProxy> tempTextureProxies[4];
    for (int i = 0; i < 4; ++i) {
        // Fill in tempTextureProxies to avoid duplicate texture proxies.
        int textureIndex = yuvaIndices[i].fIndex;

        // Safely ignore since this means we are missing the A plane.
        if (textureIndex == -1) {
            SkASSERT(SkYUVAIndex::kA_Index == i);
            continue;
        }

        if (!tempTextureProxies[textureIndex]) {
            SkASSERT(yuvaTexturesCopy[textureIndex].isValid());
            tempTextureProxies[textureIndex] =
                    proxyProvider->wrapBackendTexture(yuvaTexturesCopy[textureIndex], origin);
            if (!tempTextureProxies[textureIndex]) {
                return nullptr;
            }
        }
    }

    const int width = size.width();
    const int height = size.height();

    GrPaint paint;
    paint.setPorterDuffXPFactory(SkBlendMode::kSrc);
    // TODO: Modify the fragment processor to sample from different channel instead of taking nv12
    // bool.
    paint.addColorFragmentProcessor(GrYUVtoRGBEffect::Make(tempTextureProxies, yuvaIndices,
                                                           yuvColorSpace));

    const SkRect rect = SkRect::MakeIWH(width, height);

    renderTargetContext->drawRect(GrNoClip(), std::move(paint), GrAA::kNo, SkMatrix::I(), rect);
||||||| merged common ancestors
    return proxy->priv().peekTexture();
}

bool SkImage_Gpu::onReadPixels(const SkImageInfo& dstInfo, void* dstPixels, size_t dstRB,
                               int srcX, int srcY, CachingHint) const {
    if (!SkImageInfoValidConversion(dstInfo, this->onImageInfo())) {
        return false;
    }

    SkReadPixelsRec rec(dstInfo, dstPixels, dstRB, srcX, srcY);
    if (!rec.trim(this->width(), this->height())) {
        return false;
    }

    // TODO: this seems to duplicate code in GrTextureContext::onReadPixels and
    // GrRenderTargetContext::onReadPixels
    uint32_t flags = 0;
    if (kUnpremul_SkAlphaType == rec.fInfo.alphaType() && kPremul_SkAlphaType == fAlphaType) {
        // let the GPU perform this transformation for us
        flags = GrContextPriv::kUnpremul_PixelOpsFlag;
    }

    // This hack allows us to call makeNonTextureImage on images with arbitrary color spaces.
    // Otherwise, we'll be unable to create a render target context.
    // TODO: This shouldn't be necessary - we need more robust support for images (and surfaces)
    // with arbitrary color spaces. Unfortunately, this is one spot where we go from image to
    // surface (rather than the opposite), and our lenient image rules break our (currently) more
    // strict surface rules.
    // GrSurfaceContext::readPixels does not make use of the context's color space. However, we
    // don't allow creating a surface context for a sRGB GrPixelConfig unless the color space has
    // sRGB gamma. So we choose null for non-SRGB GrPixelConfigs and sRGB for sRGB GrPixelConfigs.
    sk_sp<SkColorSpace> surfaceColorSpace = fColorSpace;
    if (!flags) {
        if (!dstInfo.colorSpace() ||
            SkColorSpace::Equals(fColorSpace.get(), dstInfo.colorSpace())) {
            if (GrPixelConfigIsSRGB(fProxy->config())) {
                surfaceColorSpace = SkColorSpace::MakeSRGB();
            } else {
                surfaceColorSpace = nullptr;
            }
        }
    }

    sk_sp<GrSurfaceContext> sContext = fContext->contextPriv().makeWrappedSurfaceContext(
            fProxy, surfaceColorSpace);
    if (!sContext) {
        return false;
    }

    if (!sContext->readPixels(rec.fInfo, rec.fPixels, rec.fRowBytes, rec.fX, rec.fY, flags)) {
        return false;
    }

    // do we have to manually fix-up the alpha channel?
    //      src         dst
    //      unpremul    premul      fix manually
    //      premul      unpremul    done by kUnpremul_PixelOpsFlag
    // all other combos need to change.
    //
    // Should this be handled by Ganesh? todo:?
    //
    if (kPremul_SkAlphaType == rec.fInfo.alphaType() && kUnpremul_SkAlphaType == fAlphaType) {
        apply_premul(rec.fInfo, rec.fPixels, rec.fRowBytes);
    }
    return true;
}

sk_sp<SkImage> SkImage_Gpu::onMakeSubset(const SkIRect& subset) const {
    GrSurfaceDesc desc;
    desc.fOrigin = fProxy->origin();
    desc.fWidth = subset.width();
    desc.fHeight = subset.height();
    desc.fConfig = fProxy->config();

    sk_sp<GrSurfaceContext> sContext(fContext->contextPriv().makeDeferredSurfaceContext(
                                                                        desc,
                                                                        GrMipMapped::kNo,
                                                                        SkBackingFit::kExact,
                                                                        fBudgeted));
    if (!sContext) {
        return nullptr;
    }
=======
    GrPaint paint;
    paint.setPorterDuffXPFactory(SkBlendMode::kSrc);
    paint.addColorTextureProcessor(std::move(proxy), SkMatrix::I());
    if (xform) {
        paint.addColorFragmentProcessor(std::move(xform));
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!renderTargetContext->asSurfaceProxy()) {
||||||| merged common ancestors
    if (!sContext->copy(fProxy.get(), subset, SkIPoint::Make(0, 0))) {
=======
    renderTargetContext->drawRect(GrNoClip(), std::move(paint), GrAA::kNo, SkMatrix::I(),
                                  SkRect::MakeIWH(this->width(), this->height()));
    if (!renderTargetContext->asTextureProxy()) {
>>>>>>> upstream-releases
        return nullptr;
    }

<<<<<<< HEAD
    // DDL TODO: in the promise image version we must not flush here
    ctx->contextPriv().flushSurfaceWrites(renderTargetContext->asSurfaceProxy());
||||||| merged common ancestors
    // MDB: this call is okay bc we know 'sContext' was kExact
    return sk_make_sp<SkImage_Gpu>(fContext, kNeedNewImageUniqueID,
                                   fAlphaType, sContext->asTextureProxyRef(),
                                   fColorSpace, fBudgeted);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
=======
    // MDB: this call is okay bc we know 'renderTargetContext' was exact
    return sk_make_sp<SkImage_Gpu>(fContext, kNeedNewImageUniqueID, fAlphaType,
                                   renderTargetContext->asTextureProxyRef(), std::move(targetCS));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
>>>>>>> upstream-releases

<<<<<<< HEAD
    // MDB: this call is okay bc we know 'renderTargetContext' was exact
    return sk_make_sp<SkImage_Gpu>(sk_ref_sp(ctx), kNeedNewImageUniqueID, kOpaque_SkAlphaType,
                                   renderTargetContext->asTextureProxyRef(),
                                   renderTargetContext->colorSpaceInfo().refColorSpace(),
                                   isBudgeted);
}
||||||| merged common ancestors
static sk_sp<SkImage> new_wrapped_texture_common(GrContext* ctx,
                                                 const GrBackendTexture& backendTex,
                                                 GrSurfaceOrigin origin,
                                                 SkAlphaType at, sk_sp<SkColorSpace> colorSpace,
                                                 GrWrapOwnership ownership,
                                                 SkImage::TextureReleaseProc releaseProc,
                                                 SkImage::ReleaseContext releaseCtx) {
    if (backendTex.width() <= 0 || backendTex.height() <= 0) {
        return nullptr;
    }
=======
static sk_sp<SkImage> new_wrapped_texture_common(GrContext* ctx,
                                                 const GrBackendTexture& backendTex,
                                                 GrSurfaceOrigin origin,
                                                 SkAlphaType at, sk_sp<SkColorSpace> colorSpace,
                                                 GrWrapOwnership ownership,
                                                 SkImage::TextureReleaseProc releaseProc,
                                                 SkImage::ReleaseContext releaseCtx) {
    if (!backendTex.isValid() || backendTex.width() <= 0 || backendTex.height() <= 0) {
        return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
sk_sp<SkImage> SkImage::MakeFromYUVATexturesCopy(GrContext* ctx,
                                                 SkYUVColorSpace yuvColorSpace,
                                                 const GrBackendTexture yuvaTextures[],
                                                 const SkYUVAIndex yuvaIndices[4],
                                                 SkISize imageSize,
                                                 GrSurfaceOrigin imageOrigin,
                                                 sk_sp<SkColorSpace> imageColorSpace) {
    const int width = imageSize.width();
    const int height = imageSize.height();

    // Needs to create a render target in order to draw to it for the yuv->rgb conversion.
    sk_sp<GrRenderTargetContext> renderTargetContext(
            ctx->contextPriv().makeDeferredRenderTargetContext(
                    SkBackingFit::kExact, width, height, kRGBA_8888_GrPixelConfig,
                    std::move(imageColorSpace), 1, GrMipMapped::kNo, imageOrigin));
    if (!renderTargetContext) {
||||||| merged common ancestors
    GrProxyProvider* proxyProvider = ctx->contextPriv().proxyProvider();
    sk_sp<GrTextureProxy> proxy = proxyProvider->createWrappedTextureProxy(
                                        backendTex, origin, ownership, releaseProc, releaseCtx);
    if (!proxy) {
=======
    GrProxyProvider* proxyProvider = ctx->priv().proxyProvider();
    sk_sp<GrTextureProxy> proxy =
            proxyProvider->wrapBackendTexture(backendTex, origin, ownership, GrWrapCacheable::kNo,
                                              kRead_GrIOType, releaseProc, releaseCtx);
    if (!proxy) {
>>>>>>> upstream-releases
        return nullptr;
    }
<<<<<<< HEAD

    return SkImage_Gpu::ConvertYUVATexturesToRGB(ctx, yuvColorSpace, yuvaTextures, yuvaIndices,
                                                 imageSize, imageOrigin, SkBudgeted::kYes,
                                                 renderTargetContext.get());
}

sk_sp<SkImage> SkImage::MakeFromYUVATexturesCopyWithExternalBackend(
        GrContext* ctx,
        SkYUVColorSpace yuvColorSpace,
        const GrBackendTexture yuvaTextures[],
        const SkYUVAIndex yuvaIndices[4],
        SkISize imageSize,
        GrSurfaceOrigin imageOrigin,
        const GrBackendTexture& backendTexture,
        sk_sp<SkColorSpace> imageColorSpace) {
    GrBackendTexture backendTextureCopy = backendTexture;

    if (!SkImage_Gpu::ValidateBackendTexture(ctx, backendTextureCopy, &backendTextureCopy.fConfig,
                                             kRGBA_8888_SkColorType, kPremul_SkAlphaType, nullptr)) {
        return nullptr;
    }

    // Needs to create a render target with external texture
    // in order to draw to it for the yuv->rgb conversion.
    sk_sp<GrRenderTargetContext> renderTargetContext(
            ctx->contextPriv().makeBackendTextureRenderTargetContext(backendTextureCopy,
                                                                     imageOrigin, 1,
                                                                     std::move(imageColorSpace)));

    if (!renderTargetContext) {
||||||| merged common ancestors

    return sk_make_sp<SkImage_Gpu>(ctx, kNeedNewImageUniqueID,
                                   at, std::move(proxy), std::move(colorSpace), SkBudgeted::kNo);
}

sk_sp<SkImage> SkImage::MakeFromTexture(GrContext* ctx,
                                        const GrBackendTexture& tex, GrSurfaceOrigin origin,
                                        SkAlphaType at, sk_sp<SkColorSpace> cs,
                                        TextureReleaseProc releaseP, ReleaseContext releaseC) {
    if (!ctx) {
        return nullptr;
    }
    return new_wrapped_texture_common(ctx, tex, origin, at, std::move(cs), kBorrow_GrWrapOwnership,
                                      releaseP, releaseC);
}

bool validate_backend_texture(GrContext* ctx, const GrBackendTexture& tex, GrPixelConfig* config,
                              SkColorType ct, SkAlphaType at, sk_sp<SkColorSpace> cs) {
    // TODO: Create a SkImageColorInfo struct for color, alpha, and color space so we don't need to
    // create a fake image info here.
    SkImageInfo info = SkImageInfo::Make(1, 1, ct, at, cs);
    if (!SkImageInfoIsValidAllowNumericalCS(info)) {
        return false;
    }

    return ctx->caps()->validateBackendTexture(tex, ct, config);
}

sk_sp<SkImage> SkImage::MakeFromTexture(GrContext* ctx,
                                        const GrBackendTexture& tex, GrSurfaceOrigin origin,
                                        SkColorType ct, SkAlphaType at, sk_sp<SkColorSpace> cs,
                                        TextureReleaseProc releaseP, ReleaseContext releaseC) {
    if (!ctx) {
        return nullptr;
    }
    GrBackendTexture texCopy = tex;
    if (!validate_backend_texture(ctx, texCopy, &texCopy.fConfig, ct, at, cs)) {
=======
    return sk_make_sp<SkImage_Gpu>(sk_ref_sp(ctx), kNeedNewImageUniqueID, at, std::move(proxy),
                                   std::move(colorSpace));
}

sk_sp<SkImage> SkImage::MakeFromTexture(GrContext* ctx,
                                        const GrBackendTexture& tex, GrSurfaceOrigin origin,
                                        SkColorType ct, SkAlphaType at, sk_sp<SkColorSpace> cs,
                                        TextureReleaseProc releaseP, ReleaseContext releaseC) {
    if (!ctx) {
        return nullptr;
    }
    GrBackendTexture texCopy = tex;
    if (!SkImage_GpuBase::ValidateBackendTexture(ctx, texCopy, &texCopy.fConfig, ct, at, cs)) {
>>>>>>> upstream-releases
        return nullptr;
    }
<<<<<<< HEAD

    return SkImage_Gpu::ConvertYUVATexturesToRGB(ctx, yuvColorSpace, yuvaTextures, yuvaIndices,
                                                 imageSize, imageOrigin, SkBudgeted::kNo,
                                                 renderTargetContext.get());
||||||| merged common ancestors
    return MakeFromTexture(ctx, texCopy, origin, at, cs, releaseP, releaseC);
=======
    return new_wrapped_texture_common(ctx, texCopy, origin, at, std::move(cs),
                                      kBorrow_GrWrapOwnership, releaseP, releaseC);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
sk_sp<SkImage> SkImage::MakeFromYUVTexturesCopy(GrContext* ctx, SkYUVColorSpace yuvColorSpace,
                                                const GrBackendTexture yuvTextures[3],
                                                GrSurfaceOrigin imageOrigin,
                                                sk_sp<SkColorSpace> imageColorSpace) {
    // TODO: SkImageSourceChannel input is being ingored right now. Setup correctly in the future.
    SkYUVAIndex yuvaIndices[4] = {
            SkYUVAIndex{0, SkColorChannel::kR},
            SkYUVAIndex{1, SkColorChannel::kR},
            SkYUVAIndex{2, SkColorChannel::kR},
            SkYUVAIndex{-1, SkColorChannel::kA}};
    SkISize size{yuvTextures[0].width(), yuvTextures[0].height()};
    return SkImage_Gpu::MakeFromYUVATexturesCopy(ctx, yuvColorSpace, yuvTextures, yuvaIndices,
                                                 size, imageOrigin, std::move(imageColorSpace));
}

sk_sp<SkImage> SkImage::MakeFromYUVTexturesCopyWithExternalBackend(
        GrContext* ctx, SkYUVColorSpace yuvColorSpace, const GrBackendTexture yuvTextures[3],
        GrSurfaceOrigin imageOrigin, const GrBackendTexture& backendTexture,
        sk_sp<SkColorSpace> imageColorSpace) {
    SkYUVAIndex yuvaIndices[4] = {
            SkYUVAIndex{0, SkColorChannel::kR},
            SkYUVAIndex{1, SkColorChannel::kR},
            SkYUVAIndex{2, SkColorChannel::kR},
            SkYUVAIndex{-1, SkColorChannel::kA}};
    SkISize size{yuvTextures[0].width(), yuvTextures[0].height()};
    return SkImage_Gpu::MakeFromYUVATexturesCopyWithExternalBackend(
            ctx, yuvColorSpace, yuvTextures, yuvaIndices, size, imageOrigin, backendTexture,
            std::move(imageColorSpace));
}

sk_sp<SkImage> SkImage::MakeFromNV12TexturesCopy(GrContext* ctx, SkYUVColorSpace yuvColorSpace,
                                                 const GrBackendTexture nv12Textures[2],
                                                 GrSurfaceOrigin imageOrigin,
                                                 sk_sp<SkColorSpace> imageColorSpace) {
    // TODO: SkImageSourceChannel input is being ingored right now. Setup correctly in the future.
    SkYUVAIndex yuvaIndices[4] = {
            SkYUVAIndex{0, SkColorChannel::kR},
            SkYUVAIndex{1, SkColorChannel::kR},
            SkYUVAIndex{1, SkColorChannel::kG},
            SkYUVAIndex{-1, SkColorChannel::kA}};
    SkISize size{nv12Textures[0].width(), nv12Textures[0].height()};
    return SkImage_Gpu::MakeFromYUVATexturesCopy(ctx, yuvColorSpace, nv12Textures, yuvaIndices,
                                                 size, imageOrigin, std::move(imageColorSpace));
}

sk_sp<SkImage> SkImage::MakeFromNV12TexturesCopyWithExternalBackend(
        GrContext* ctx,
        SkYUVColorSpace yuvColorSpace,
        const GrBackendTexture nv12Textures[2],
        GrSurfaceOrigin imageOrigin,
        const GrBackendTexture& backendTexture,
        sk_sp<SkColorSpace> imageColorSpace) {
    SkYUVAIndex yuvaIndices[4] = {
            SkYUVAIndex{0, SkColorChannel::kR},
            SkYUVAIndex{1, SkColorChannel::kR},
            SkYUVAIndex{1, SkColorChannel::kG},
            SkYUVAIndex{-1, SkColorChannel::kA}};
    SkISize size{nv12Textures[0].width(), nv12Textures[0].height()};
    return SkImage_Gpu::MakeFromYUVATexturesCopyWithExternalBackend(
            ctx, yuvColorSpace, nv12Textures, yuvaIndices, size, imageOrigin, backendTexture,
            std::move(imageColorSpace));
}

static sk_sp<SkImage> create_image_from_producer(GrContext* context, GrTextureProducer* producer,
                                                 SkAlphaType at, uint32_t id,
                                                 SkColorSpace* dstColorSpace,
                                                 GrMipMapped mipMapped) {
    sk_sp<SkColorSpace> texColorSpace;
    sk_sp<GrTextureProxy> proxy(producer->refTextureProxy(mipMapped, dstColorSpace,
                                                          &texColorSpace));
    if (!proxy) {
||||||| merged common ancestors
sk_sp<SkImage> SkImage::MakeFromAdoptedTexture(GrContext* ctx,
                                               const GrBackendTexture& tex, GrSurfaceOrigin origin,
                                               SkAlphaType at, sk_sp<SkColorSpace> cs) {
    if (!ctx->contextPriv().resourceProvider()) {
        // We have a DDL context and we don't support adopted textures for them.
=======
sk_sp<SkImage> SkImage::MakeFromAdoptedTexture(GrContext* ctx,
                                               const GrBackendTexture& tex, GrSurfaceOrigin origin,
                                               SkColorType ct, SkAlphaType at,
                                               sk_sp<SkColorSpace> cs) {
    if (!ctx || !ctx->priv().resourceProvider()) {
        // We have a DDL context and we don't support adopted textures for them.
>>>>>>> upstream-releases
        return nullptr;
    }
<<<<<<< HEAD
    return sk_make_sp<SkImage_Gpu>(sk_ref_sp(context), id, at, std::move(proxy),
                                   std::move(texColorSpace), SkBudgeted::kNo);
}

sk_sp<SkImage> SkImage::makeTextureImage(GrContext* context, SkColorSpace* dstColorSpace,
                                         GrMipMapped mipMapped) const {
    if (!context) {
||||||| merged common ancestors
    return new_wrapped_texture_common(ctx, tex, origin, at, std::move(cs), kAdopt_GrWrapOwnership,
                                      nullptr, nullptr);
}

sk_sp<SkImage> SkImage::MakeFromAdoptedTexture(GrContext* ctx,
                                               const GrBackendTexture& tex, GrSurfaceOrigin origin,
                                               SkColorType ct, SkAlphaType at,
                                               sk_sp<SkColorSpace> cs) {
    GrBackendTexture texCopy = tex;
    if (!validate_backend_texture(ctx, texCopy, &texCopy.fConfig, ct, at, cs)) {
=======
    GrBackendTexture texCopy = tex;
    if (!SkImage_GpuBase::ValidateBackendTexture(ctx, texCopy, &texCopy.fConfig, ct, at, cs)) {
>>>>>>> upstream-releases
        return nullptr;
    }
<<<<<<< HEAD
    if (GrContext* incumbent = as_IB(this)->context()) {
        if (incumbent != context) {
            return nullptr;
        }
        sk_sp<GrTextureProxy> proxy = as_IB(this)->asTextureProxyRef();
        SkASSERT(proxy);
        if (GrMipMapped::kNo == mipMapped || proxy->mipMapped() == mipMapped) {
            return sk_ref_sp(const_cast<SkImage*>(this));
        }
        GrTextureAdjuster adjuster(context, std::move(proxy), this->alphaType(),
                                   this->uniqueID(), this->colorSpace());
        return create_image_from_producer(context, &adjuster, this->alphaType(),
                                          this->uniqueID(), dstColorSpace, mipMapped);
    }

    if (this->isLazyGenerated()) {
        GrImageTextureMaker maker(context, this, kDisallow_CachingHint);
        return create_image_from_producer(context, &maker, this->alphaType(),
                                          this->uniqueID(), dstColorSpace, mipMapped);
||||||| merged common ancestors
    return MakeFromAdoptedTexture(ctx, texCopy, origin, at, cs);
}

static GrBackendTexture make_backend_texture_from_handle(GrBackend backend,
                                                         int width, int height,
                                                         GrPixelConfig config,
                                                         GrBackendObject handle) {
    switch (backend) {
        case kOpenGL_GrBackend: {
            const GrGLTextureInfo* glInfo = (const GrGLTextureInfo*)(handle);
            return GrBackendTexture(width, height, config, *glInfo);
        }
#ifdef SK_VULKAN
        case kVulkan_GrBackend: {
            const GrVkImageInfo* vkInfo = (const GrVkImageInfo*)(handle);
            return GrBackendTexture(width, height, *vkInfo);
        }
#endif
        case kMock_GrBackend: {
            const GrMockTextureInfo* mockInfo = (const GrMockTextureInfo*)(handle);
            return GrBackendTexture(width, height, config, *mockInfo);
        }
        default:
            return GrBackendTexture();
    }
}

static bool are_yuv_sizes_valid(const SkISize yuvSizes[], bool nv12) {
    if (yuvSizes[0].fWidth <= 0 || yuvSizes[0].fHeight <= 0 ||
        yuvSizes[1].fWidth <= 0 || yuvSizes[1].fHeight <= 0) {
        return false;
    }
    if (!nv12 && (yuvSizes[2].fWidth <= 0 || yuvSizes[2].fHeight <= 0)) {
        return false;
=======
    return new_wrapped_texture_common(ctx, texCopy, origin, at, std::move(cs),
                                      kAdopt_GrWrapOwnership, nullptr, nullptr);
}

sk_sp<SkImage> SkImage::MakeFromCompressed(GrContext* context, sk_sp<SkData> data,
                                           int width, int height, CompressionType type) {
    // create the backing texture
    GrSurfaceDesc desc;
    desc.fFlags = kNone_GrSurfaceFlags;
    desc.fWidth = width;
    desc.fHeight = height;
    switch (type) {
        case kETC1_CompressionType:
            desc.fConfig = kRGB_ETC1_GrPixelConfig;
            break;
        default:
            desc.fConfig = kUnknown_GrPixelConfig;
            break;
>>>>>>> upstream-releases
    }
    desc.fSampleCnt = 1;

<<<<<<< HEAD
    if (const SkBitmap* bmp = as_IB(this)->onPeekBitmap()) {
        GrBitmapTextureMaker maker(context, *bmp);
        return create_image_from_producer(context, &maker, this->alphaType(),
                                          this->uniqueID(), dstColorSpace, mipMapped);
    }
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
||||||| merged common ancestors
    return true;
}

static sk_sp<SkImage> make_from_yuv_textures_copy(GrContext* ctx, SkYUVColorSpace colorSpace,
                                                  bool nv12,
                                                  const GrBackendTexture yuvBackendTextures[],
                                                  const SkISize yuvSizes[],
                                                  GrSurfaceOrigin origin,
                                                  sk_sp<SkColorSpace> imageColorSpace) {
    GrProxyProvider* proxyProvider = ctx->contextPriv().proxyProvider();
=======
    GrProxyProvider* proxyProvider = context->priv().proxyProvider();
    sk_sp<GrTextureProxy> proxy = proxyProvider->createProxy(std::move(data), desc);
>>>>>>> upstream-releases

<<<<<<< HEAD
static GrTextureType TextureTypeFromBackendFormat(const GrBackendFormat& backendFormat) {
    if (const GrGLenum* target = backendFormat.getGLTarget()) {
        return GrGLTexture::TextureTypeFromTarget(*target);
    }
    return GrTextureType::k2D;
}

sk_sp<SkImage> SkImage_Gpu::MakePromiseTexture(GrContext* context,
                                               const GrBackendFormat& backendFormat,
                                               int width,
                                               int height,
                                               GrMipMapped mipMapped,
                                               GrSurfaceOrigin origin,
                                               SkColorType colorType,
                                               SkAlphaType alphaType,
                                               sk_sp<SkColorSpace> colorSpace,
                                               TextureFulfillProc textureFulfillProc,
                                               TextureReleaseProc textureReleaseProc,
                                               PromiseDoneProc promiseDoneProc,
                                               TextureContext textureContext) {
    // The contract here is that if 'promiseDoneProc' is passed in it should always be called,
    // even if creation of the SkImage fails.
    if (!promiseDoneProc) {
||||||| merged common ancestors
    if (!are_yuv_sizes_valid(yuvSizes, nv12)) {
=======
    if (!proxy) {
>>>>>>> upstream-releases
        return nullptr;
    }

<<<<<<< HEAD
    SkPromiseImageHelper promiseHelper(textureFulfillProc, textureReleaseProc, promiseDoneProc,
                                       textureContext);
||||||| merged common ancestors
    sk_sp<GrTextureProxy> yProxy = proxyProvider->createWrappedTextureProxy(yuvBackendTextures[0],
                                                                            origin);
    sk_sp<GrTextureProxy> uProxy = proxyProvider->createWrappedTextureProxy(yuvBackendTextures[1],
                                                                            origin);
    sk_sp<GrTextureProxy> vProxy;
=======
    return sk_make_sp<SkImage_Gpu>(sk_ref_sp(context), kNeedNewImageUniqueID, kOpaque_SkAlphaType,
                                   std::move(proxy), nullptr);
}

sk_sp<SkImage> SkImage_Gpu::ConvertYUVATexturesToRGB(GrContext* ctx, SkYUVColorSpace yuvColorSpace,
                                                     const GrBackendTexture yuvaTextures[],
                                                     const SkYUVAIndex yuvaIndices[4], SkISize size,
                                                     GrSurfaceOrigin origin,
                                                     GrRenderTargetContext* renderTargetContext) {
    SkASSERT(renderTargetContext);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!context) {
||||||| merged common ancestors
    if (nv12) {
        vProxy = uProxy;
    } else {
        vProxy = proxyProvider->createWrappedTextureProxy(yuvBackendTextures[2], origin);
    }
    if (!yProxy || !uProxy || !vProxy) {
=======
    int numTextures;
    if (!SkYUVAIndex::AreValidIndices(yuvaIndices, &numTextures)) {
>>>>>>> upstream-releases
        return nullptr;
    }

<<<<<<< HEAD
    if (width <= 0 || height <= 0) {
||||||| merged common ancestors
    const int width = yuvSizes[0].fWidth;
    const int height = yuvSizes[0].fHeight;

    // Needs to be a render target in order to draw to it for the yuv->rgb conversion.
    sk_sp<GrRenderTargetContext> renderTargetContext(ctx->makeDeferredRenderTargetContext(
            SkBackingFit::kExact, width, height, kRGBA_8888_GrPixelConfig,
            std::move(imageColorSpace), 1, GrMipMapped::kNo, origin));
    if (!renderTargetContext) {
=======
    sk_sp<GrTextureProxy> tempTextureProxies[4];
    if (!SkImage_GpuBase::MakeTempTextureProxies(ctx, yuvaTextures, numTextures, yuvaIndices,
                                                 origin, tempTextureProxies)) {
>>>>>>> upstream-releases
        return nullptr;
    }

<<<<<<< HEAD
    if (!textureFulfillProc || !textureReleaseProc) {
        return nullptr;
    }

    SkImageInfo info = SkImageInfo::Make(width, height, colorType, alphaType, colorSpace);
    if (!SkImageInfoIsValid(info)) {
        return nullptr;
    }
    GrPixelConfig config = kUnknown_GrPixelConfig;
    if (!context->contextPriv().caps()->getConfigFromBackendFormat(backendFormat, colorType,
                                                                   &config)) {
||||||| merged common ancestors
    GrPaint paint;
    paint.setPorterDuffXPFactory(SkBlendMode::kSrc);
    paint.addColorFragmentProcessor(GrYUVtoRGBEffect::Make(yProxy, uProxy, vProxy,
                                                           yuvSizes, colorSpace, nv12));

    const SkRect rect = SkRect::MakeIWH(width, height);

    renderTargetContext->drawRect(GrNoClip(), std::move(paint), GrAA::kNo, SkMatrix::I(), rect);

    if (!renderTargetContext->asSurfaceProxy()) {
=======
    const SkRect rect = SkRect::MakeIWH(size.width(), size.height());
    if (!RenderYUVAToRGBA(ctx, renderTargetContext, rect, yuvColorSpace, nullptr,
                          tempTextureProxies, yuvaIndices)) {
>>>>>>> upstream-releases
        return nullptr;
    }

<<<<<<< HEAD
    GrTextureType textureType = TextureTypeFromBackendFormat(backendFormat);
||||||| merged common ancestors
    // MDB: this call is okay bc we know 'renderTargetContext' was exact
    return sk_make_sp<SkImage_Gpu>(ctx, kNeedNewImageUniqueID, kOpaque_SkAlphaType,
                                   renderTargetContext->asTextureProxyRef(),
                                   renderTargetContext->colorSpaceInfo().refColorSpace(),
                                   SkBudgeted::kYes);
}
=======
    SkAlphaType at = GetAlphaTypeFromYUVAIndices(yuvaIndices);
    // MDB: this call is okay bc we know 'renderTargetContext' was exact
    return sk_make_sp<SkImage_Gpu>(sk_ref_sp(ctx), kNeedNewImageUniqueID, at,
                                   renderTargetContext->asTextureProxyRef(),
                                   renderTargetContext->colorSpaceInfo().refColorSpace());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (mipMapped == GrMipMapped::kYes && GrTextureTypeHasRestrictedSampling(textureType)) {
        // It is invalid to have a GL_TEXTURE_EXTERNAL or GL_TEXTURE_RECTANGLE and have mips as
        // well.
||||||| merged common ancestors
static sk_sp<SkImage> make_from_yuv_objects_copy(GrContext* ctx, SkYUVColorSpace colorSpace,
                                                 bool nv12,
                                                 const GrBackendObject yuvTextureHandles[],
                                                 const SkISize yuvSizes[],
                                                 GrSurfaceOrigin origin,
                                                 sk_sp<SkColorSpace> imageColorSpace) {
    if (!are_yuv_sizes_valid(yuvSizes, nv12)) {
=======
sk_sp<SkImage> SkImage::MakeFromYUVATexturesCopy(GrContext* ctx,
                                                 SkYUVColorSpace yuvColorSpace,
                                                 const GrBackendTexture yuvaTextures[],
                                                 const SkYUVAIndex yuvaIndices[4],
                                                 SkISize imageSize,
                                                 GrSurfaceOrigin imageOrigin,
                                                 sk_sp<SkColorSpace> imageColorSpace) {
    const int width = imageSize.width();
    const int height = imageSize.height();

    const GrBackendFormat format =
            ctx->priv().caps()->getBackendFormatFromColorType(kRGBA_8888_SkColorType);

    // Needs to create a render target in order to draw to it for the yuv->rgb conversion.
    sk_sp<GrRenderTargetContext> renderTargetContext(
            ctx->priv().makeDeferredRenderTargetContext(
                    format, SkBackingFit::kExact, width, height, kRGBA_8888_GrPixelConfig,
                    std::move(imageColorSpace), 1, GrMipMapped::kNo, imageOrigin));
    if (!renderTargetContext) {
>>>>>>> upstream-releases
        return nullptr;
    }

<<<<<<< HEAD
    GrProxyProvider* proxyProvider = context->contextPriv().proxyProvider();

    GrSurfaceDesc desc;
    desc.fWidth = width;
    desc.fHeight = height;
    desc.fConfig = config;

    sk_sp<GrTextureProxy> proxy = proxyProvider->createLazyProxy(
            [promiseHelper, config](GrResourceProvider* resourceProvider) mutable {
                if (!resourceProvider) {
                    promiseHelper.reset();
                    return sk_sp<GrTexture>();
                }

                return promiseHelper.getTexture(resourceProvider, config);
            },
            desc, origin, mipMapped, textureType, GrInternalSurfaceFlags::kNone,
            SkBackingFit::kExact, SkBudgeted::kNo,
            GrSurfaceProxy::LazyInstantiationType::kUninstantiate);
||||||| merged common ancestors
    GrBackendTexture backendTextures[3];

    const GrPixelConfig kConfig = nv12 ? kRGBA_8888_GrPixelConfig : kAlpha_8_GrPixelConfig;
=======
    return SkImage_Gpu::ConvertYUVATexturesToRGB(ctx, yuvColorSpace, yuvaTextures, yuvaIndices,
                                                 imageSize, imageOrigin, renderTargetContext.get());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!proxy) {
        return nullptr;
    }
||||||| merged common ancestors
    GrBackend backend = ctx->contextPriv().getBackend();
    backendTextures[0] = make_backend_texture_from_handle(backend,
                                                          yuvSizes[0].fWidth,
                                                          yuvSizes[0].fHeight,
                                                          kConfig,
                                                          yuvTextureHandles[0]);
    backendTextures[1] = make_backend_texture_from_handle(backend,
                                                          yuvSizes[1].fWidth,
                                                          yuvSizes[1].fHeight,
                                                          kConfig,
                                                          yuvTextureHandles[1]);
=======
sk_sp<SkImage> SkImage::MakeFromYUVATexturesCopyWithExternalBackend(
        GrContext* ctx,
        SkYUVColorSpace yuvColorSpace,
        const GrBackendTexture yuvaTextures[],
        const SkYUVAIndex yuvaIndices[4],
        SkISize imageSize,
        GrSurfaceOrigin imageOrigin,
        const GrBackendTexture& backendTexture,
        sk_sp<SkColorSpace> imageColorSpace) {
    GrBackendTexture backendTextureCopy = backendTexture;
>>>>>>> upstream-releases

<<<<<<< HEAD
    return sk_make_sp<SkImage_Gpu>(sk_ref_sp(context), kNeedNewImageUniqueID, alphaType,
                                   std::move(proxy), std::move(colorSpace), SkBudgeted::kNo);
}

sk_sp<SkImage> SkImage_Gpu::MakePromiseYUVATexture(GrContext* context,
                                                   SkYUVColorSpace yuvColorSpace,
                                                   const GrBackendFormat yuvaFormats[],
                                                   const SkYUVAIndex yuvaIndices[4],
                                                   int imageWidth,
                                                   int imageHeight,
                                                   GrSurfaceOrigin imageOrigin,
                                                   sk_sp<SkColorSpace> imageColorSpace,
                                                   TextureFulfillProc textureFulfillProc,
                                                   TextureReleaseProc textureReleaseProc,
                                                   PromiseDoneProc promiseDoneProc,
                                                   TextureContext textureContexts[]) {
    // The contract here is that if 'promiseDoneProc' is passed in it should always be called,
    // even if creation of the SkImage fails.
    if (!promiseDoneProc) {
        return nullptr;
||||||| merged common ancestors
    if (!nv12) {
        backendTextures[2] = make_backend_texture_from_handle(backend,
                                                              yuvSizes[2].fWidth,
                                                              yuvSizes[2].fHeight,
                                                              kConfig,
                                                              yuvTextureHandles[2]);
=======
    SkAlphaType at = SkImage_GpuBase::GetAlphaTypeFromYUVAIndices(yuvaIndices);
    if (!SkImage_Gpu::ValidateBackendTexture(ctx, backendTextureCopy, &backendTextureCopy.fConfig,
                                             kRGBA_8888_SkColorType, at, nullptr)) {
        return nullptr;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    // Temporarily work around an MSVC compiler bug. Copying the arrays directly into the lambda
    // doesn't work on some older tool chains
    struct {
        GrPixelConfig fConfigs[4] = { kUnknown_GrPixelConfig, kUnknown_GrPixelConfig,
                                      kUnknown_GrPixelConfig, kUnknown_GrPixelConfig };
        SkPromiseImageHelper fPromiseHelpers[4];
        SkYUVAIndex fLocalIndices[4];
    } params;

    // Determine which of the slots in 'yuvaFormats' and 'textureContexts' are actually used
    bool slotUsed[4] = { false, false, false, false };
    for (int i = 0; i < 4; ++i) {
        if (yuvaIndices[i].fIndex < 0) {
            SkASSERT(SkYUVAIndex::kA_Index == i); // We had better have YUV channels
            continue;
        }
||||||| merged common ancestors
    return make_from_yuv_textures_copy(ctx, colorSpace, nv12,
                                       backendTextures, yuvSizes, origin,
                                       std::move(imageColorSpace));
}
=======
    // Needs to create a render target with external texture
    // in order to draw to it for the yuv->rgb conversion.
    sk_sp<GrRenderTargetContext> renderTargetContext(
            ctx->priv().makeBackendTextureRenderTargetContext(backendTextureCopy,
                                                              imageOrigin, 1,
                                                              std::move(imageColorSpace)));
>>>>>>> upstream-releases

<<<<<<< HEAD
        SkASSERT(yuvaIndices[i].fIndex < 4);
        slotUsed[yuvaIndices[i].fIndex] = true;
    }
||||||| merged common ancestors
sk_sp<SkImage> SkImage::MakeFromYUVTexturesCopy(GrContext* ctx, SkYUVColorSpace colorSpace,
                                                const GrBackendObject yuvTextureHandles[3],
                                                const SkISize yuvSizes[3], GrSurfaceOrigin origin,
                                                sk_sp<SkColorSpace> imageColorSpace) {
    return make_from_yuv_objects_copy(ctx, colorSpace, false, yuvTextureHandles, yuvSizes, origin,
                                      std::move(imageColorSpace));
}
=======
    if (!renderTargetContext) {
        return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    for (int i = 0; i < 4; ++i) {
        params.fLocalIndices[i] = yuvaIndices[i];
||||||| merged common ancestors
sk_sp<SkImage> SkImage::MakeFromNV12TexturesCopy(GrContext* ctx, SkYUVColorSpace colorSpace,
                                                 const GrBackendObject yuvTextureHandles[2],
                                                 const SkISize yuvSizes[2],
                                                 GrSurfaceOrigin origin,
                                                 sk_sp<SkColorSpace> imageColorSpace) {
    return make_from_yuv_objects_copy(ctx, colorSpace, true, yuvTextureHandles, yuvSizes, origin,
                                      std::move(imageColorSpace));
}
=======
    return SkImage_Gpu::ConvertYUVATexturesToRGB(ctx, yuvColorSpace, yuvaTextures, yuvaIndices,
                                                 imageSize, imageOrigin, renderTargetContext.get());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (slotUsed[i]) {
            params.fPromiseHelpers[i].set(textureFulfillProc, textureReleaseProc,
                                          promiseDoneProc, textureContexts[i]);
        }
    }

    // DDL TODO: we need to create a SkImage_GpuYUVA here! This implementation just
    // returns the Y-plane.
    if (!context) {
        return nullptr;
    }

    if (imageWidth <= 0 || imageHeight <= 0) {
||||||| merged common ancestors
sk_sp<SkImage> SkImage::MakeFromYUVTexturesCopy(GrContext* ctx, SkYUVColorSpace colorSpace,
                                                const GrBackendTexture yuvBackendTextures[3],
                                                const SkISize yuvSizes[3], GrSurfaceOrigin origin,
                                                sk_sp<SkColorSpace> imageColorSpace) {
    return make_from_yuv_textures_copy(ctx, colorSpace, false, yuvBackendTextures, yuvSizes, origin,
                                       std::move(imageColorSpace));
}

sk_sp<SkImage> SkImage::MakeFromNV12TexturesCopy(GrContext* ctx, SkYUVColorSpace colorSpace,
                                                 const GrBackendTexture yuvBackendTextures[2],
                                                 const SkISize yuvSizes[2],
                                                 GrSurfaceOrigin origin,
                                                 sk_sp<SkColorSpace> imageColorSpace) {
    return make_from_yuv_textures_copy(ctx, colorSpace, true, yuvBackendTextures, yuvSizes, origin,
                                       std::move(imageColorSpace));
}

static sk_sp<SkImage> create_image_from_maker(GrContext* context, GrTextureMaker* maker,
                                              SkAlphaType at, uint32_t id,
                                              SkColorSpace* dstColorSpace) {
    sk_sp<SkColorSpace> texColorSpace;
    sk_sp<GrTextureProxy> proxy(maker->refTextureProxyForParams(
            GrSamplerState::ClampNearest(), dstColorSpace, &texColorSpace, nullptr));
    if (!proxy) {
=======
sk_sp<SkImage> SkImage::MakeFromYUVTexturesCopy(GrContext* ctx, SkYUVColorSpace yuvColorSpace,
                                                const GrBackendTexture yuvTextures[3],
                                                GrSurfaceOrigin imageOrigin,
                                                sk_sp<SkColorSpace> imageColorSpace) {
    // TODO: SkImageSourceChannel input is being ingored right now. Setup correctly in the future.
    SkYUVAIndex yuvaIndices[4] = {
            SkYUVAIndex{0, SkColorChannel::kR},
            SkYUVAIndex{1, SkColorChannel::kR},
            SkYUVAIndex{2, SkColorChannel::kR},
            SkYUVAIndex{-1, SkColorChannel::kA}};
    SkISize size{yuvTextures[0].width(), yuvTextures[0].height()};
    return SkImage_Gpu::MakeFromYUVATexturesCopy(ctx, yuvColorSpace, yuvTextures, yuvaIndices,
                                                 size, imageOrigin, std::move(imageColorSpace));
}

sk_sp<SkImage> SkImage::MakeFromYUVTexturesCopyWithExternalBackend(
        GrContext* ctx, SkYUVColorSpace yuvColorSpace, const GrBackendTexture yuvTextures[3],
        GrSurfaceOrigin imageOrigin, const GrBackendTexture& backendTexture,
        sk_sp<SkColorSpace> imageColorSpace) {
    SkYUVAIndex yuvaIndices[4] = {
            SkYUVAIndex{0, SkColorChannel::kR},
            SkYUVAIndex{1, SkColorChannel::kR},
            SkYUVAIndex{2, SkColorChannel::kR},
            SkYUVAIndex{-1, SkColorChannel::kA}};
    SkISize size{yuvTextures[0].width(), yuvTextures[0].height()};
    return SkImage_Gpu::MakeFromYUVATexturesCopyWithExternalBackend(
            ctx, yuvColorSpace, yuvTextures, yuvaIndices, size, imageOrigin, backendTexture,
            std::move(imageColorSpace));
}

sk_sp<SkImage> SkImage::MakeFromNV12TexturesCopy(GrContext* ctx, SkYUVColorSpace yuvColorSpace,
                                                 const GrBackendTexture nv12Textures[2],
                                                 GrSurfaceOrigin imageOrigin,
                                                 sk_sp<SkColorSpace> imageColorSpace) {
    // TODO: SkImageSourceChannel input is being ingored right now. Setup correctly in the future.
    SkYUVAIndex yuvaIndices[4] = {
            SkYUVAIndex{0, SkColorChannel::kR},
            SkYUVAIndex{1, SkColorChannel::kR},
            SkYUVAIndex{1, SkColorChannel::kG},
            SkYUVAIndex{-1, SkColorChannel::kA}};
    SkISize size{nv12Textures[0].width(), nv12Textures[0].height()};
    return SkImage_Gpu::MakeFromYUVATexturesCopy(ctx, yuvColorSpace, nv12Textures, yuvaIndices,
                                                 size, imageOrigin, std::move(imageColorSpace));
}

sk_sp<SkImage> SkImage::MakeFromNV12TexturesCopyWithExternalBackend(
        GrContext* ctx,
        SkYUVColorSpace yuvColorSpace,
        const GrBackendTexture nv12Textures[2],
        GrSurfaceOrigin imageOrigin,
        const GrBackendTexture& backendTexture,
        sk_sp<SkColorSpace> imageColorSpace) {
    SkYUVAIndex yuvaIndices[4] = {
            SkYUVAIndex{0, SkColorChannel::kR},
            SkYUVAIndex{1, SkColorChannel::kR},
            SkYUVAIndex{1, SkColorChannel::kG},
            SkYUVAIndex{-1, SkColorChannel::kA}};
    SkISize size{nv12Textures[0].width(), nv12Textures[0].height()};
    return SkImage_Gpu::MakeFromYUVATexturesCopyWithExternalBackend(
            ctx, yuvColorSpace, nv12Textures, yuvaIndices, size, imageOrigin, backendTexture,
            std::move(imageColorSpace));
}

static sk_sp<SkImage> create_image_from_producer(GrContext* context, GrTextureProducer* producer,
                                                 SkAlphaType at, uint32_t id,
                                                 GrMipMapped mipMapped) {
    sk_sp<GrTextureProxy> proxy(producer->refTextureProxy(mipMapped));
    if (!proxy) {
>>>>>>> upstream-releases
        return nullptr;
    }
<<<<<<< HEAD
||||||| merged common ancestors
    return sk_make_sp<SkImage_Gpu>(context, id, at,
                                   std::move(proxy), std::move(texColorSpace), SkBudgeted::kNo);
}
=======
    return sk_make_sp<SkImage_Gpu>(sk_ref_sp(context), id, at, std::move(proxy),
                                   sk_ref_sp(producer->colorSpace()));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!textureFulfillProc || !textureReleaseProc) {
||||||| merged common ancestors
sk_sp<SkImage> SkImage::makeTextureImage(GrContext* context, SkColorSpace* dstColorSpace) const {
    if (!context) {
=======
sk_sp<SkImage> SkImage::makeTextureImage(GrContext* context, SkColorSpace* dstColorSpace,
                                         GrMipMapped mipMapped) const {
    if (!context) {
>>>>>>> upstream-releases
        return nullptr;
    }
<<<<<<< HEAD

    SkImageInfo info = SkImageInfo::Make(imageWidth, imageHeight, kRGBA_8888_SkColorType,
                                         kPremul_SkAlphaType, imageColorSpace);
    if (!SkImageInfoIsValid(info)) {
        return nullptr;
||||||| merged common ancestors
    if (GrContext* incumbent = as_IB(this)->context()) {
        return incumbent == context ? sk_ref_sp(const_cast<SkImage*>(this)) : nullptr;
=======

    if (this->isTextureBacked()) {
        if (!as_IB(this)->context()->priv().matches(context)) {
            return nullptr;
        }

        sk_sp<GrTextureProxy> proxy = as_IB(this)->asTextureProxyRef(context);
        SkASSERT(proxy);
        if (GrMipMapped::kNo == mipMapped || proxy->mipMapped() == mipMapped) {
            return sk_ref_sp(const_cast<SkImage*>(this));
        }
        GrTextureAdjuster adjuster(context, std::move(proxy), this->alphaType(),
                                   this->uniqueID(), this->colorSpace());
        return create_image_from_producer(context, &adjuster, this->alphaType(),
                                          this->uniqueID(), mipMapped);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    for (int i = 0; i < 4; ++i) {
        if (slotUsed[i]) {
            // DDL TODO: This (the kAlpha_8) only works for non-NV12 YUV textures
            if (!context->contextPriv().caps()->getConfigFromBackendFormat(yuvaFormats[i],
                                                                           kAlpha_8_SkColorType,
                                                                           &params.fConfigs[i])) {
                return nullptr;
            }
        }
||||||| merged common ancestors
    if (this->isLazyGenerated()) {
        GrImageTextureMaker maker(context, this, kDisallow_CachingHint);
        return create_image_from_maker(context, &maker, this->alphaType(),
                                       this->uniqueID(), dstColorSpace);
=======
    if (this->isLazyGenerated()) {
        GrImageTextureMaker maker(context, this, kDisallow_CachingHint);
        return create_image_from_producer(context, &maker, this->alphaType(),
                                          this->uniqueID(), mipMapped);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    GrSurfaceDesc desc;
    desc.fFlags = kNone_GrSurfaceFlags;
    desc.fWidth = imageWidth;
    desc.fHeight = imageHeight;
    // Hack since we're just returning the Y-plane
    desc.fConfig = params.fConfigs[params.fLocalIndices[SkYUVAIndex::kY_Index].fIndex];
    desc.fSampleCnt = 1;

    GrProxyProvider* proxyProvider = context->contextPriv().proxyProvider();

    sk_sp<GrTextureProxy> proxy = proxyProvider->createLazyProxy(
            [params](GrResourceProvider* resourceProvider) mutable {
                if (!resourceProvider) {
                    for (int i = 0; i < 4; ++i) {
                        if (params.fPromiseHelpers[i].isValid()) {
                            params.fPromiseHelpers[i].reset();
                        }
                    }
                    return sk_sp<GrTexture>();
                }

                // We need to collect the YUVA planes as backend textures (vs. GrTextures) to
                // feed into the SkImage_GpuYUVA factory.
                GrBackendTexture yuvaTextures[4];
                for (int i = 0; i < 4; ++i) {
                    if (params.fPromiseHelpers[i].isValid()) {
                        sk_sp<GrTexture> tmp = params.fPromiseHelpers[i].getTexture(
                            resourceProvider, params.fConfigs[i]);
                        if (!tmp) {
                            return sk_sp<GrTexture>();
                        }
                        yuvaTextures[i] = tmp->getBackendTexture();
                    }
                }

#if 1
                // For the time being, simply return the Y-plane. The reason for this is that
                // this lazy proxy is instantiated at flush time, after the sort, therefore
                // we cannot be introducing a new opList (in order to render the YUV texture).
                int yIndex = params.fLocalIndices[SkYUVAIndex::kY_Index].fIndex;
                return params.fPromiseHelpers[yIndex].getTexture(resourceProvider,
                                                                 params.fConfigs[yIndex]);
#else
                GrGpu* gpu = resourceProvider->priv().gpu();
                GrContext* context = gpu->getContext();

                sk_sp<SkImage> tmp = SkImage_Gpu::MakeFromYUVATexturesCopyImpl(context,
                                                                               yuvColorSpace,
                                                                               yuvaTextures,
                                                                               localIndices,
                                                                               imageSize,
                                                                               imageOrigin,
                                                                               imageColorSpace);
                if (!tmp) {
                    return sk_sp<GrTexture>();
                }
                return sk_ref_sp<GrTexture>(tmp->getTexture());
#endif
            },
            desc, imageOrigin, GrMipMapped::kNo, GrTextureType::k2D,
            GrInternalSurfaceFlags::kNone, SkBackingFit::kExact, SkBudgeted::kNo,
            GrSurfaceProxy::LazyInstantiationType::kUninstantiate);

    if (!proxy) {
        return nullptr;
||||||| merged common ancestors
    if (const SkBitmap* bmp = as_IB(this)->onPeekBitmap()) {
        GrBitmapTextureMaker maker(context, *bmp);
        return create_image_from_maker(context, &maker, this->alphaType(),
                                       this->uniqueID(), dstColorSpace);
=======
    if (const SkBitmap* bmp = as_IB(this)->onPeekBitmap()) {
        GrBitmapTextureMaker maker(context, *bmp);
        return create_image_from_producer(context, &maker, this->alphaType(),
                                          this->uniqueID(), mipMapped);
>>>>>>> upstream-releases
    }

    return sk_make_sp<SkImage_Gpu>(sk_ref_sp(context), kNeedNewImageUniqueID, kPremul_SkAlphaType,
                                   std::move(proxy), std::move(imageColorSpace), SkBudgeted::kNo);
}

<<<<<<< HEAD
///////////////////////////////////////////////////////////////////////////////////////////////////

||||||| merged common ancestors
=======
///////////////////////////////////////////////////////////////////////////////////////////////////

sk_sp<SkImage> SkImage_Gpu::MakePromiseTexture(GrContext* context,
                                               const GrBackendFormat& backendFormat,
                                               int width,
                                               int height,
                                               GrMipMapped mipMapped,
                                               GrSurfaceOrigin origin,
                                               SkColorType colorType,
                                               SkAlphaType alphaType,
                                               sk_sp<SkColorSpace> colorSpace,
                                               PromiseImageTextureFulfillProc textureFulfillProc,
                                               PromiseImageTextureReleaseProc textureReleaseProc,
                                               PromiseImageTextureDoneProc textureDoneProc,
                                               PromiseImageTextureContext textureContext) {
    // The contract here is that if 'promiseDoneProc' is passed in it should always be called,
    // even if creation of the SkImage fails. Once we call MakePromiseImageLazyProxy it takes
    // responsibility for calling the done proc.
    if (!textureDoneProc) {
        return nullptr;
    }
    SkScopeExit callDone([textureDoneProc, textureContext]() { textureDoneProc(textureContext); });

    SkImageInfo info = SkImageInfo::Make(width, height, colorType, alphaType, colorSpace);
    if (!SkImageInfoIsValid(info)) {
        return nullptr;
    }

    if (!context) {
        return nullptr;
    }

    if (width <= 0 || height <= 0) {
        return nullptr;
    }

    GrPixelConfig config =
            context->priv().caps()->getConfigFromBackendFormat(backendFormat, colorType);
    if (config == kUnknown_GrPixelConfig) {
        return nullptr;
    }

    callDone.clear();
    auto proxy = MakePromiseImageLazyProxy(context, width, height, origin, config, backendFormat,
                                           mipMapped, textureFulfillProc, textureReleaseProc,
                                           textureDoneProc, textureContext);
    if (!proxy) {
        return nullptr;
    }
    return sk_make_sp<SkImage_Gpu>(sk_ref_sp(context), kNeedNewImageUniqueID, alphaType,
                                   std::move(proxy), std::move(colorSpace));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

>>>>>>> upstream-releases
sk_sp<SkImage> SkImage::MakeCrossContextFromEncoded(GrContext* context, sk_sp<SkData> encoded,
                                                    bool buildMips, SkColorSpace* dstColorSpace,
                                                    bool limitToMaxTextureSize) {
    sk_sp<SkImage> codecImage = SkImage::MakeFromEncoded(std::move(encoded));
    if (!codecImage) {
        return nullptr;
    }

    // Some backends or drivers don't support (safely) moving resources between contexts
<<<<<<< HEAD
    if (!context || !context->contextPriv().caps()->crossContextTextureSupport()) {
||||||| merged common ancestors
    if (!context || !context->caps()->crossContextTextureSupport()) {
=======
    if (!context || !context->priv().caps()->crossContextTextureSupport()) {
>>>>>>> upstream-releases
        return codecImage;
    }

<<<<<<< HEAD
    auto maxTextureSize = context->contextPriv().caps()->maxTextureSize();
    if (limitToMaxTextureSize &&
        (codecImage->width() > maxTextureSize || codecImage->height() > maxTextureSize)) {
        SkAutoPixmapStorage pmap;
        SkImageInfo info = as_IB(codecImage)->onImageInfo();
        if (!dstColorSpace) {
            info = info.makeColorSpace(nullptr);
        }
        if (!pmap.tryAlloc(info) || !codecImage->readPixels(pmap, 0, 0, kDisallow_CachingHint)) {
            return nullptr;
        }
        return MakeCrossContextFromPixmap(context, pmap, buildMips, dstColorSpace, true);
    }

||||||| merged common ancestors
=======
    // If non-power-of-two mipmapping isn't supported, ignore the client's request
    if (!context->priv().caps()->mipMapSupport()) {
        buildMips = false;
    }

    auto maxTextureSize = context->priv().caps()->maxTextureSize();
    if (limitToMaxTextureSize &&
        (codecImage->width() > maxTextureSize || codecImage->height() > maxTextureSize)) {
        SkAutoPixmapStorage pmap;
        SkImageInfo info = as_IB(codecImage)->onImageInfo();
        if (!dstColorSpace) {
            info = info.makeColorSpace(nullptr);
        }
        if (!pmap.tryAlloc(info) || !codecImage->readPixels(pmap, 0, 0, kDisallow_CachingHint)) {
            return nullptr;
        }
        return MakeCrossContextFromPixmap(context, pmap, buildMips, dstColorSpace, true);
    }

>>>>>>> upstream-releases
    // Turn the codec image into a GrTextureProxy
    GrImageTextureMaker maker(context, codecImage.get(), kDisallow_CachingHint);
    GrSamplerState samplerState(
            GrSamplerState::WrapMode::kClamp,
            buildMips ? GrSamplerState::Filter::kMipMap : GrSamplerState::Filter::kBilerp);
    SkScalar scaleAdjust[2] = { 1.0f, 1.0f };
    sk_sp<GrTextureProxy> proxy(maker.refTextureProxyForParams(samplerState, scaleAdjust));
    // Given that we disable mipmaps if non-power-of-two mipmapping isn't supported, we always
    // expect the created texture to be unscaled.
    SkASSERT(scaleAdjust[0] == 1.0f && scaleAdjust[1] == 1.0f);
    if (!proxy) {
        return codecImage;
    }

    if (!proxy->instantiate(context->priv().resourceProvider())) {
        return codecImage;
    }
    sk_sp<GrTexture> texture = sk_ref_sp(proxy->peekTexture());

    // Flush any writes or uploads
    context->priv().prepareSurfaceForExternalIO(proxy.get());

    GrGpu* gpu = context->priv().getGpu();
    sk_sp<GrSemaphore> sema = gpu->prepareTextureForCrossContextUsage(texture.get());

    auto gen = GrBackendTextureImageGenerator::Make(std::move(texture), proxy->origin(),
<<<<<<< HEAD
                                                    std::move(sema),
                                                    as_IB(codecImage)->onImageInfo().colorType(),
                                                    codecImage->alphaType(),
                                                    std::move(texColorSpace));
||||||| merged common ancestors
                                                    std::move(sema), codecImage->alphaType(),
                                                    std::move(texColorSpace));
=======
                                                    std::move(sema),
                                                    as_IB(codecImage)->onImageInfo().colorType(),
                                                    codecImage->alphaType(),
                                                    codecImage->refColorSpace());
>>>>>>> upstream-releases
    return SkImage::MakeFromGenerator(std::move(gen));
}

sk_sp<SkImage> SkImage::MakeCrossContextFromPixmap(GrContext* context,
                                                   const SkPixmap& originalPixmap, bool buildMips,
                                                   SkColorSpace* dstColorSpace,
                                                   bool limitToMaxTextureSize) {
    // Some backends or drivers don't support (safely) moving resources between contexts
<<<<<<< HEAD
    if (!context || !context->contextPriv().caps()->crossContextTextureSupport()) {
        return SkImage::MakeRasterCopy(originalPixmap);
||||||| merged common ancestors
    if (!context || !context->caps()->crossContextTextureSupport()) {
        return SkImage::MakeRasterCopy(pixmap);
=======
    if (!context || !context->priv().caps()->crossContextTextureSupport()) {
        return SkImage::MakeRasterCopy(originalPixmap);
>>>>>>> upstream-releases
    }

    // If we don't have access to the resource provider and gpu (i.e. in a DDL context) we will not
    // be able to make everything needed for a GPU CrossContext image. Thus return a raster copy
    // instead.
<<<<<<< HEAD
    if (!context->contextPriv().resourceProvider()) {
        return SkImage::MakeRasterCopy(originalPixmap);
    }

    const SkPixmap* pixmap = &originalPixmap;
    SkAutoPixmapStorage resized;
    int maxTextureSize = context->contextPriv().caps()->maxTextureSize();
    int maxDim = SkTMax(originalPixmap.width(), originalPixmap.height());
    if (limitToMaxTextureSize && maxDim > maxTextureSize) {
        float scale = static_cast<float>(maxTextureSize) / maxDim;
        int newWidth = SkTMin(static_cast<int>(originalPixmap.width() * scale), maxTextureSize);
        int newHeight = SkTMin(static_cast<int>(originalPixmap.height() * scale), maxTextureSize);
        SkImageInfo info = originalPixmap.info().makeWH(newWidth, newHeight);
        if (!resized.tryAlloc(info) || !originalPixmap.scalePixels(resized, kLow_SkFilterQuality)) {
            return nullptr;
        }
        pixmap = &resized;
||||||| merged common ancestors
    if (!context->contextPriv().resourceProvider()) {
        return SkImage::MakeRasterCopy(pixmap);
=======
    if (!context->priv().resourceProvider()) {
        return SkImage::MakeRasterCopy(originalPixmap);
    }

    // If non-power-of-two mipmapping isn't supported, ignore the client's request
    if (!context->priv().caps()->mipMapSupport()) {
        buildMips = false;
    }

    const SkPixmap* pixmap = &originalPixmap;
    SkAutoPixmapStorage resized;
    int maxTextureSize = context->priv().caps()->maxTextureSize();
    int maxDim = SkTMax(originalPixmap.width(), originalPixmap.height());
    if (limitToMaxTextureSize && maxDim > maxTextureSize) {
        float scale = static_cast<float>(maxTextureSize) / maxDim;
        int newWidth = SkTMin(static_cast<int>(originalPixmap.width() * scale), maxTextureSize);
        int newHeight = SkTMin(static_cast<int>(originalPixmap.height() * scale), maxTextureSize);
        SkImageInfo info = originalPixmap.info().makeWH(newWidth, newHeight);
        if (!resized.tryAlloc(info) || !originalPixmap.scalePixels(resized, kLow_SkFilterQuality)) {
            return nullptr;
        }
        pixmap = &resized;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    GrProxyProvider* proxyProvider = context->contextPriv().proxyProvider();
||||||| merged common ancestors

    GrProxyProvider* proxyProvider = context->contextPriv().proxyProvider();
=======
    GrProxyProvider* proxyProvider = context->priv().proxyProvider();
>>>>>>> upstream-releases
    // Turn the pixmap into a GrTextureProxy
    sk_sp<GrTextureProxy> proxy;
    if (buildMips) {
        SkBitmap bmp;
        bmp.installPixels(*pixmap);
        proxy = proxyProvider->createMipMapProxyFromBitmap(bmp);
    } else {
        if (SkImageInfoIsValid(pixmap->info())) {
            ATRACE_ANDROID_FRAMEWORK("Upload Texture [%ux%u]", pixmap->width(), pixmap->height());
            // We don't need a release proc on the data in pixmap since we know we are in a
            // GrContext that has a resource provider. Thus the createTextureProxy call will
            // immediately upload the data.
            sk_sp<SkImage> image = SkImage::MakeFromRaster(*pixmap, nullptr, nullptr);
            proxy = proxyProvider->createTextureProxy(std::move(image), kNone_GrSurfaceFlags, 1,
                                                      SkBudgeted::kYes, SkBackingFit::kExact);
        }
    }

    if (!proxy) {
        return SkImage::MakeRasterCopy(*pixmap);
    }

    sk_sp<GrTexture> texture = sk_ref_sp(proxy->peekTexture());

    // Flush any writes or uploads
    context->priv().prepareSurfaceForExternalIO(proxy.get());
    GrGpu* gpu = context->priv().getGpu();

    sk_sp<GrSemaphore> sema = gpu->prepareTextureForCrossContextUsage(texture.get());

    auto gen = GrBackendTextureImageGenerator::Make(std::move(texture), proxy->origin(),
                                                    std::move(sema), pixmap->colorType(),
                                                    pixmap->alphaType(),
                                                    pixmap->info().refColorSpace());
    return SkImage::MakeFromGenerator(std::move(gen));
}

#if defined(SK_BUILD_FOR_ANDROID) && __ANDROID_API__ >= 26
sk_sp<SkImage> SkImage::MakeFromAHardwareBuffer(AHardwareBuffer* graphicBuffer, SkAlphaType at,
                                                sk_sp<SkColorSpace> cs,
                                                GrSurfaceOrigin surfaceOrigin) {
    auto gen = GrAHardwareBufferImageGenerator::Make(graphicBuffer, at, cs, surfaceOrigin);
    return SkImage::MakeFromGenerator(std::move(gen));
}

sk_sp<SkImage> SkImage::MakeFromAHardwareBufferWithData(GrContext* context,
                                                        const SkPixmap& pixmap,
                                                        AHardwareBuffer* hardwareBuffer,
                                                        GrSurfaceOrigin surfaceOrigin) {
    AHardwareBuffer_Desc bufferDesc;
    AHardwareBuffer_describe(hardwareBuffer, &bufferDesc);

    if (!SkToBool(bufferDesc.usage & AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE)) {
        return nullptr;
    }

    GrBackendFormat backendFormat = GrAHardwareBufferUtils::GetBackendFormat(context,
                                                                             hardwareBuffer,
                                                                             bufferDesc.format,
                                                                             true);

    if (!backendFormat.isValid()) {
        return nullptr;
    }

    GrAHardwareBufferUtils::DeleteImageProc deleteImageProc = nullptr;
    GrAHardwareBufferUtils::DeleteImageCtx deleteImageCtx = nullptr;

    GrBackendTexture backendTexture =
            GrAHardwareBufferUtils::MakeBackendTexture(context, hardwareBuffer,
                                                       bufferDesc.width, bufferDesc.height,
                                                       &deleteImageProc, &deleteImageCtx,
                                                       false, backendFormat, true);
    if (!backendTexture.isValid()) {
        return nullptr;
    }
    SkASSERT(deleteImageProc);

    SkColorType colorType =
            GrAHardwareBufferUtils::GetSkColorTypeFromBufferFormat(bufferDesc.format);

    backendTexture.fConfig = context->priv().caps()->getConfigFromBackendFormat(backendFormat,
                                                                                colorType);

    GrProxyProvider* proxyProvider = context->priv().proxyProvider();
    if (!proxyProvider) {
        deleteImageProc(deleteImageCtx);
        return nullptr;
    }

    sk_sp<GrTextureProxy> proxy =
            proxyProvider->wrapBackendTexture(backendTexture, surfaceOrigin,
                                              kBorrow_GrWrapOwnership, GrWrapCacheable::kNo,
                                              kRW_GrIOType, deleteImageProc, deleteImageCtx);
    if (!proxy) {
        deleteImageProc(deleteImageCtx);
        return nullptr;
    }

    sk_sp<SkColorSpace> cs = pixmap.refColorSpace();
    SkAlphaType at =  pixmap.alphaType();

    sk_sp<SkImage> image = sk_make_sp<SkImage_Gpu>(sk_ref_sp(context), kNeedNewImageUniqueID, at,
                                                   proxy, cs);
    if (!image) {
        return nullptr;
    }

    GrDrawingManager* drawingManager = context->priv().drawingManager();
    if (!drawingManager) {
        return nullptr;
    }

    sk_sp<GrTextureContext> texContext = drawingManager->makeTextureContext(proxy, cs);
    if (!texContext) {
        return nullptr;
    }

    SkImageInfo srcInfo = SkImageInfo::Make(bufferDesc.width, bufferDesc.height, colorType, at,
                                            std::move(cs));
    texContext->writePixels(srcInfo, pixmap.addr(0, 0), pixmap.rowBytes(), 0, 0);

    drawingManager->flush(proxy.get(), SkSurface::BackendSurfaceAccess::kNoAccess,
                          SkSurface::kSyncCpu_FlushFlag, 0, nullptr);

    return image;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

bool SkImage::MakeBackendTextureFromSkImage(GrContext* ctx,
                                            sk_sp<SkImage> image,
                                            GrBackendTexture* backendTexture,
                                            BackendTextureReleaseProc* releaseProc) {
    if (!image || !ctx || !backendTexture || !releaseProc) {
        return false;
    }

    // Ensure we have a texture backed image.
    if (!image->isTextureBacked()) {
        image = image->makeTextureImage(ctx, nullptr);
        if (!image) {
            return false;
        }
    }
    GrTexture* texture = image->getTexture();
    if (!texture) {
        // In context-loss cases, we may not have a texture.
        return false;
    }

    // If the image's context doesn't match the provided context, fail.
    if (texture->getContext() != ctx) {
        return false;
    }

    // Flush any pending IO on the texture.
    ctx->priv().prepareSurfaceForExternalIO(as_IB(image)->peekProxy());
    SkASSERT(!texture->surfacePriv().hasPendingIO());

    // We must make a copy of the image if the image is not unique, if the GrTexture owned by the
    // image is not unique, or if the texture wraps an external object.
    if (!image->unique() || !texture->surfacePriv().hasUniqueRef() ||
        texture->resourcePriv().refsWrappedObjects()) {
        // onMakeSubset will always copy the image.
        image = as_IB(image)->onMakeSubset(ctx, image->bounds());
        if (!image) {
            return false;
        }

        texture = image->getTexture();
        if (!texture) {
            return false;
        }

        // Flush to ensure that the copy is completed before we return the texture.
        ctx->priv().prepareSurfaceForExternalIO(as_IB(image)->peekProxy());
        SkASSERT(!texture->surfacePriv().hasPendingIO());
    }

    SkASSERT(!texture->resourcePriv().refsWrappedObjects());
    SkASSERT(texture->surfacePriv().hasUniqueRef());
    SkASSERT(image->unique());

    // Take a reference to the GrTexture and release the image.
    sk_sp<GrTexture> textureRef(SkSafeRef(texture));
    image = nullptr;

    // Steal the backend texture from the GrTexture, releasing the GrTexture in the process.
    return GrTexture::StealBackendTexture(std::move(textureRef), backendTexture, releaseProc);
}
