/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkDeferredDisplayListRecorder.h"
#include "SkMessageBus.h"
#include "SkDeferredDisplayList.h"
#include "SkSurface.h"
#include "SkSurfaceCharacterization.h"

#if !SK_SUPPORT_GPU
SkDeferredDisplayListRecorder::SkDeferredDisplayListRecorder(const SkSurfaceCharacterization&) {}

SkDeferredDisplayListRecorder::~SkDeferredDisplayListRecorder() {}

bool SkDeferredDisplayListRecorder::init() { return false; }

SkCanvas* SkDeferredDisplayListRecorder::getCanvas() { return nullptr; }

std::unique_ptr<SkDeferredDisplayList> SkDeferredDisplayListRecorder::detach() { return nullptr; }

sk_sp<SkImage> SkDeferredDisplayListRecorder::makePromiseTexture(
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
    return nullptr;
}

sk_sp<SkImage> SkDeferredDisplayListRecorder::makeYUVAPromiseTexture(
        SkYUVColorSpace yuvColorSpace,
        const GrBackendFormat yuvaFormats[],
        const SkISize yuvaSizes[],
        const SkYUVAIndex yuvaIndices[4],
        int imageWidth,
        int imageHeight,
        GrSurfaceOrigin imageOrigin,
        sk_sp<SkColorSpace> imageColorSpace,
        PromiseImageTextureFulfillProc textureFulfillProc,
        PromiseImageTextureReleaseProc textureReleaseProc,
        PromiseImageTextureDoneProc textureDoneProc,
        PromiseImageTextureContext textureContexts[]) {
    return nullptr;
}

#else

<<<<<<< HEAD
#include "SkDeferredDisplayList.h"
#include "SkSurface.h"
#include "SkSurfaceCharacterization.h"

#if !SK_SUPPORT_GPU
SkDeferredDisplayListRecorder::SkDeferredDisplayListRecorder(const SkSurfaceCharacterization&) {}

SkDeferredDisplayListRecorder::~SkDeferredDisplayListRecorder() {}

bool SkDeferredDisplayListRecorder::init() { return false; }

SkCanvas* SkDeferredDisplayListRecorder::getCanvas() { return nullptr; }

std::unique_ptr<SkDeferredDisplayList> SkDeferredDisplayListRecorder::detach() { return nullptr; }

sk_sp<SkImage> SkDeferredDisplayListRecorder::makePromiseTexture(
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
    return nullptr;
}

sk_sp<SkImage> SkDeferredDisplayListRecorder::makeYUVAPromiseTexture(
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
    return nullptr;
}

#else

||||||| merged common ancestors
#if SK_SUPPORT_GPU
=======
>>>>>>> upstream-releases
#include "GrContextPriv.h"
#include "GrProxyProvider.h"
#include "GrRenderTargetContext.h"
#include "GrTexture.h"
<<<<<<< HEAD

||||||| merged common ancestors

#include "SkGpuDevice.h"
=======
>>>>>>> upstream-releases
#include "SkGr.h"
<<<<<<< HEAD
#include "SkImage_Gpu.h"
||||||| merged common ancestors
=======
#include "SkImage_Gpu.h"
#include "SkImage_GpuYUVA.h"
#include "SkMakeUnique.h"
#include "SkPromiseImageTexture.h"
>>>>>>> upstream-releases
#include "SkSurface_Gpu.h"
<<<<<<< HEAD

SkDeferredDisplayListRecorder::SkDeferredDisplayListRecorder(const SkSurfaceCharacterization& c)
        : fCharacterization(c) {
    if (fCharacterization.isValid()) {
        fContext = GrContextPriv::MakeDDL(fCharacterization.refContextInfo());
    }
||||||| merged common ancestors
#endif

#include "SkCanvas.h" // TODO: remove
#include "SkDeferredDisplayList.h"
#include "SkSurface.h"
#include "SkSurfaceCharacterization.h"

SkDeferredDisplayListRecorder::SkDeferredDisplayListRecorder(
                    const SkSurfaceCharacterization& characterization)
        : fCharacterization(characterization) {
=======
#include "SkYUVASizeInfo.h"

SkDeferredDisplayListRecorder::SkDeferredDisplayListRecorder(const SkSurfaceCharacterization& c)
        : fCharacterization(c) {
    if (fCharacterization.isValid()) {
        fContext = GrContextPriv::MakeDDL(fCharacterization.refContextInfo());
    }
>>>>>>> upstream-releases
}

SkDeferredDisplayListRecorder::~SkDeferredDisplayListRecorder() {
<<<<<<< HEAD
    if (fContext) {
        auto proxyProvider = fContext->contextPriv().proxyProvider();
||||||| merged common ancestors
#if SK_SUPPORT_GPU && !defined(SK_RASTER_RECORDER_IMPLEMENTATION)
    auto proxyProvider = fContext->contextPriv().proxyProvider();
=======
    if (fContext) {
        auto proxyProvider = fContext->priv().proxyProvider();
>>>>>>> upstream-releases

        // This allows the uniquely keyed proxies to keep their keys but removes their back
        // pointer to the about-to-be-deleted proxy provider. The proxies will use their
        // unique key to reattach to cached versions of themselves or to appropriately tag new
        // resources (if a cached version was not found). This system operates independent of
        // the replaying context's proxy provider (i.e., these uniquely keyed proxies will not
        // appear in the replaying proxy providers uniquely keyed proxy map). This should be fine
        // since no one else should be trying to reconnect to the orphaned proxies and orphaned
        // proxies from different DDLs that share the same key should simply reconnect to the
        // same cached resource.
        proxyProvider->orphanAllUniqueKeys();
    }
}


bool SkDeferredDisplayListRecorder::init() {
    SkASSERT(fContext);
    SkASSERT(!fLazyProxyData);
    SkASSERT(!fSurface);

    if (!fCharacterization.isValid()) {
        return false;
    }

    fLazyProxyData = sk_sp<SkDeferredDisplayList::LazyProxyData>(
                                                    new SkDeferredDisplayList::LazyProxyData);

<<<<<<< HEAD
    auto proxyProvider = fContext->contextPriv().proxyProvider();
||||||| merged common ancestors
    fSurface = SkSurface::MakeRaster(ii, &fCharacterization.surfaceProps());
    return SkToBool(fSurface.get());
#else
    SkASSERT(!fLazyProxyData);
=======
    auto proxyProvider = fContext->priv().proxyProvider();
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool usesGLFBO0 = fCharacterization.usesGLFBO0();
    if (usesGLFBO0) {
        if (kOpenGL_GrBackend != fContext->contextPriv().getBackend() ||
            fCharacterization.isTextureable()) {
||||||| merged common ancestors
#if SK_SUPPORT_GPU
    if (!fContext) {
        fContext = GrContextPriv::MakeDDL(fCharacterization.contextInfo());
        if (!fContext) {
=======
    bool usesGLFBO0 = fCharacterization.usesGLFBO0();
    if (usesGLFBO0) {
        if (GrBackendApi::kOpenGL != fContext->backend() ||
            fCharacterization.isTextureable()) {
>>>>>>> upstream-releases
            return false;
        }
    }

<<<<<<< HEAD
||||||| merged common ancestors
    fLazyProxyData = sk_sp<SkDeferredDisplayList::LazyProxyData>(
                                                    new SkDeferredDisplayList::LazyProxyData);

    auto proxyProvider = fContext->contextPriv().proxyProvider();

=======
    if (fCharacterization.vulkanSecondaryCBCompatible()) {
        // Because of the restrictive API allowed for a GrVkSecondaryCBDrawContext, we know ahead
        // of time that we don't be able to support certain parameter combinations. Specifially we
        // fail on usesGLFBO0 since we can't mix GL and Vulkan. We can't have a texturable object.
        // And finally the GrVkSecondaryCBDrawContext always assumes a top left origin.
        if (usesGLFBO0 ||
            fCharacterization.isTextureable() ||
            fCharacterization.origin() == kBottomLeft_GrSurfaceOrigin) {
            return false;
        }
    }

>>>>>>> upstream-releases
    GrSurfaceDesc desc;
    desc.fFlags = kRenderTarget_GrSurfaceFlag;
    desc.fWidth = fCharacterization.width();
    desc.fHeight = fCharacterization.height();
    desc.fConfig = fCharacterization.config();
    desc.fSampleCnt = fCharacterization.stencilCount();

    sk_sp<SkDeferredDisplayList::LazyProxyData> lazyProxyData = fLazyProxyData;

    // What we're doing here is we're creating a lazy proxy to back the SkSurface. The lazy
    // proxy, when instantiated, will use the GrRenderTarget that backs the SkSurface that the
    // DDL is being replayed into.

<<<<<<< HEAD
    GrInternalSurfaceFlags surfaceFlags = GrInternalSurfaceFlags::kNone;
    if (fContext->contextPriv().caps()->usesMixedSamples() && desc.fSampleCnt > 1 && !usesGLFBO0) {
        // In GL, FBO 0 never supports mixed samples
        surfaceFlags |= GrInternalSurfaceFlags::kMixedSampled;
    }
    if (fContext->contextPriv().caps()->maxWindowRectangles() > 0 && !usesGLFBO0) {
        // In GL, FBO 0 never supports window rectangles
        surfaceFlags |= GrInternalSurfaceFlags::kWindowRectsSupport;
    }
    if (usesGLFBO0) {
        surfaceFlags |= GrInternalSurfaceFlags::kGLRTFBOIDIs0;
    }
    static constexpr GrProxyProvider::TextureInfo kTextureInfo{GrMipMapped::kNo,
                                                               GrTextureType::k2D};
    const GrProxyProvider::TextureInfo* optionalTextureInfo = nullptr;
    if (fCharacterization.isTextureable()) {
        optionalTextureInfo = &kTextureInfo;
    }

||||||| merged common ancestors
=======
    GrInternalSurfaceFlags surfaceFlags = GrInternalSurfaceFlags::kNone;
    if (fContext->priv().caps()->usesMixedSamples() && desc.fSampleCnt > 1 && !usesGLFBO0) {
        // In GL, FBO 0 never supports mixed samples
        surfaceFlags |= GrInternalSurfaceFlags::kMixedSampled;
    }
    if (usesGLFBO0) {
        surfaceFlags |= GrInternalSurfaceFlags::kGLRTFBOIDIs0;
    }
    static constexpr GrProxyProvider::TextureInfo kTextureInfo{GrMipMapped::kNo,
                                                               GrTextureType::k2D};
    const GrProxyProvider::TextureInfo* optionalTextureInfo = nullptr;
    if (fCharacterization.isTextureable()) {
        optionalTextureInfo = &kTextureInfo;
    }

    const GrBackendFormat format = fContext->priv().caps()->getBackendFormatFromColorType(
            fCharacterization.colorType());

>>>>>>> upstream-releases
    sk_sp<GrRenderTargetProxy> proxy = proxyProvider->createLazyRenderTargetProxy(
            [lazyProxyData](GrResourceProvider* resourceProvider) {
                if (!resourceProvider) {
                    return sk_sp<GrSurface>();
                }

                // The proxy backing the destination surface had better have been instantiated
                // prior to the proxy backing the DLL's surface. Steal its GrRenderTarget.
                SkASSERT(lazyProxyData->fReplayDest->peekSurface());
                return sk_ref_sp<GrSurface>(lazyProxyData->fReplayDest->peekSurface());
            },
            format,
            desc,
            fCharacterization.origin(),
            surfaceFlags,
            optionalTextureInfo,
            SkBackingFit::kExact,
            SkBudgeted::kYes,
            fCharacterization.vulkanSecondaryCBCompatible());

    sk_sp<GrSurfaceContext> c = fContext->priv().makeWrappedSurfaceContext(
                                                                 std::move(proxy),
                                                                 fCharacterization.refColorSpace(),
                                                                 &fCharacterization.surfaceProps());
    fSurface = SkSurface_Gpu::MakeWrappedRenderTarget(fContext.get(),
                                                      sk_ref_sp(c->asRenderTargetContext()));
    return SkToBool(fSurface.get());
}

SkCanvas* SkDeferredDisplayListRecorder::getCanvas() {
    if (!fContext) {
        return nullptr;
    }

    if (!fSurface && !this->init()) {
        return nullptr;
    }

    return fSurface->getCanvas();
}

std::unique_ptr<SkDeferredDisplayList> SkDeferredDisplayListRecorder::detach() {
<<<<<<< HEAD
    if (!fContext) {
        return nullptr;
    }
||||||| merged common ancestors
#ifdef SK_RASTER_RECORDER_IMPLEMENTATION
    sk_sp<SkImage> img = fSurface->makeImageSnapshot();
    fSurface.reset();

    // TODO: need to wrap the opLists associated with the deferred draws
    // in the SkDeferredDisplayList.
    return std::unique_ptr<SkDeferredDisplayList>(
                            new SkDeferredDisplayList(fCharacterization, std::move(img)));
#else
=======
    if (!fContext) {
        return nullptr;
    }

    if (fSurface) {
        SkCanvas* canvas = fSurface->getCanvas();

        canvas->restoreToCount(0);
    }
>>>>>>> upstream-releases

    auto ddl = std::unique_ptr<SkDeferredDisplayList>(
                           new SkDeferredDisplayList(fCharacterization, std::move(fLazyProxyData)));

    fContext->priv().moveOpListsToDDL(ddl.get());

    // We want a new lazy proxy target for each recorded DDL so force the (lazy proxy-backed)
    // SkSurface to be regenerated for each DDL.
    fSurface = nullptr;
    return ddl;
}

<<<<<<< HEAD
sk_sp<SkImage> SkDeferredDisplayListRecorder::makePromiseTexture(
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
    if (!fContext) {
        return nullptr;
    }

    return SkImage_Gpu::MakePromiseTexture(fContext.get(),
                                           backendFormat,
                                           width,
                                           height,
                                           mipMapped,
                                           origin,
                                           colorType,
                                           alphaType,
                                           std::move(colorSpace),
                                           textureFulfillProc,
                                           textureReleaseProc,
                                           promiseDoneProc,
                                           textureContext);
}
||||||| merged common ancestors
#endif // SK_RASTER_RECORDER_IMPLEMENTATION
=======
sk_sp<SkImage> SkDeferredDisplayListRecorder::makePromiseTexture(
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
    if (!fContext) {
        return nullptr;
    }

    return SkImage_Gpu::MakePromiseTexture(fContext.get(),
                                           backendFormat,
                                           width,
                                           height,
                                           mipMapped,
                                           origin,
                                           colorType,
                                           alphaType,
                                           std::move(colorSpace),
                                           textureFulfillProc,
                                           textureReleaseProc,
                                           textureDoneProc,
                                           textureContext);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
sk_sp<SkImage> SkDeferredDisplayListRecorder::makeYUVAPromiseTexture(
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
    if (!fContext) {
        return nullptr;
    }

    return SkImage_Gpu::MakePromiseYUVATexture(fContext.get(),
                                               yuvColorSpace,
                                               yuvaFormats,
                                               yuvaIndices,
                                               imageWidth,
                                               imageHeight,
                                               imageOrigin,
                                               std::move(imageColorSpace),
                                               textureFulfillProc,
                                               textureReleaseProc,
                                               promiseDoneProc,
                                               textureContexts);
||||||| merged common ancestors
=======
sk_sp<SkImage> SkDeferredDisplayListRecorder::makeYUVAPromiseTexture(
        SkYUVColorSpace yuvColorSpace,
        const GrBackendFormat yuvaFormats[],
        const SkISize yuvaSizes[],
        const SkYUVAIndex yuvaIndices[4],
        int imageWidth,
        int imageHeight,
        GrSurfaceOrigin imageOrigin,
        sk_sp<SkColorSpace> imageColorSpace,
        PromiseImageTextureFulfillProc textureFulfillProc,
        PromiseImageTextureReleaseProc textureReleaseProc,
        PromiseImageTextureDoneProc textureDoneProc,
        PromiseImageTextureContext textureContexts[]) {
    if (!fContext) {
        return nullptr;
    }

    return SkImage_GpuYUVA::MakePromiseYUVATexture(fContext.get(),
                                                   yuvColorSpace,
                                                   yuvaFormats,
                                                   yuvaSizes,
                                                   yuvaIndices,
                                                   imageWidth,
                                                   imageHeight,
                                                   imageOrigin,
                                                   std::move(imageColorSpace),
                                                   textureFulfillProc,
                                                   textureReleaseProc,
                                                   textureDoneProc,
                                                   textureContexts);
>>>>>>> upstream-releases
}

#endif
