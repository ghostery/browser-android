/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrProxyProvider.h"

#include "GrCaps.h"
#include "GrContext.h"
#include "GrContextPriv.h"
#include "GrImageContext.h"
#include "GrImageContextPriv.h"
#include "GrRenderTarget.h"
#include "GrResourceKey.h"
#include "GrResourceProvider.h"
#include "GrSurfaceProxy.h"
#include "GrSurfaceProxyPriv.h"
#include "GrTexture.h"
#include "GrTextureProxyCacheAccess.h"
#include "GrTextureRenderTargetProxy.h"
#include "../private/GrSingleOwner.h"
#include "SkAutoPixmapStorage.h"
#include "SkBitmap.h"
#include "SkGr.h"
#include "SkImage.h"
#include "SkImage_Base.h"
#include "SkImageInfoPriv.h"
#include "SkImagePriv.h"
#include "SkMipMap.h"
#include "SkTraceEvent.h"

#define ASSERT_SINGLE_OWNER \
<<<<<<< HEAD
    SkDEBUGCODE(GrSingleOwner::AutoEnforce debug_SingleOwner(fSingleOwner);)

GrProxyProvider::GrProxyProvider(GrResourceProvider* resourceProvider,
                                 GrResourceCache* resourceCache,
                                 sk_sp<const GrCaps> caps,
                                 GrSingleOwner* owner)
        : fResourceProvider(resourceProvider)
        , fResourceCache(resourceCache)
        , fAbandoned(false)
        , fCaps(caps)
        , fContextUniqueID(resourceCache->contextUniqueID())
#ifdef SK_DEBUG
        , fSingleOwner(owner)
#endif
{
    SkASSERT(fResourceProvider);
    SkASSERT(fResourceCache);
    SkASSERT(fCaps);
    SkASSERT(fSingleOwner);
}
||||||| merged common ancestors
    SkDEBUGCODE(GrSingleOwner::AutoEnforce debug_SingleOwner(fSingleOwner);)

GrProxyProvider::GrProxyProvider(GrResourceProvider* resourceProvider,
                                 GrResourceCache* resourceCache,
                                 sk_sp<const GrCaps> caps,
                                 GrSingleOwner* owner)
        : fResourceProvider(resourceProvider)
        , fResourceCache(resourceCache)
        , fAbandoned(false)
        , fCaps(caps)
#ifdef SK_DEBUG
        , fSingleOwner(owner)
#endif
{
=======
    SkDEBUGCODE(GrSingleOwner::AutoEnforce debug_SingleOwner(fImageContext->priv().singleOwner());)
>>>>>>> upstream-releases

<<<<<<< HEAD
GrProxyProvider::GrProxyProvider(uint32_t contextUniqueID,
                                 sk_sp<const GrCaps> caps,
                                 GrSingleOwner* owner)
        : fResourceProvider(nullptr)
        , fResourceCache(nullptr)
        , fAbandoned(false)
        , fCaps(caps)
        , fContextUniqueID(contextUniqueID)
#ifdef SK_DEBUG
        , fSingleOwner(owner)
#endif
{
    SkASSERT(fContextUniqueID != SK_InvalidUniqueID);
    SkASSERT(fCaps);
    SkASSERT(fSingleOwner);
}
||||||| merged common ancestors
}
=======
GrProxyProvider::GrProxyProvider(GrImageContext* imageContext) : fImageContext(imageContext) {}
>>>>>>> upstream-releases

GrProxyProvider::~GrProxyProvider() {
<<<<<<< HEAD
    if (fResourceCache) {
        // In DDL-mode a proxy provider can still have extant uniquely keyed proxies (since
        // they need their unique keys to, potentially, find a cached resource when the
        // DDL is played) but, in non-DDL-mode they should all have been cleaned up by this point.
        SkASSERT(!fUniquelyKeyedProxies.count());
    }
||||||| merged common ancestors
    SkASSERT(!fUniquelyKeyedProxies.count());
=======
    if (this->renderingDirectly()) {
        // In DDL-mode a proxy provider can still have extant uniquely keyed proxies (since
        // they need their unique keys to, potentially, find a cached resource when the
        // DDL is played) but, in non-DDL-mode they should all have been cleaned up by this point.
        SkASSERT(!fUniquelyKeyedProxies.count());
    }
>>>>>>> upstream-releases
}

bool GrProxyProvider::assignUniqueKeyToProxy(const GrUniqueKey& key, GrTextureProxy* proxy) {
    ASSERT_SINGLE_OWNER
    SkASSERT(key.isValid());
    if (this->isAbandoned() || !proxy) {
        return false;
    }

<<<<<<< HEAD
    // If there is already a GrResource with this key then the caller has violated the normal
    // usage pattern of uniquely keyed resources (e.g., they have created one w/o first seeing
    // if it already existed in the cache).
    SkASSERT(!fResourceCache || !fResourceCache->findAndRefUniqueResource(key));

||||||| merged common ancestors
    // If there is already a GrResource with this key then the caller has violated the normal
    // usage pattern of uniquely keyed resources (e.g., they have created one w/o first seeing
    // if it already existed in the cache).
    SkASSERT(!fResourceCache || !fResourceCache->findAndRefUniqueResource(key));

    // Uncached resources can never have a unique key, unless they're wrapped resources. Wrapped
    // resources are a special case: the unique keys give us a weak ref so that we can reuse the
    // same resource (rather than re-wrapping). When a wrapped resource is no longer referenced,
    // it will always be released - it is never converted to a scratch resource.
    if (SkBudgeted::kNo == proxy->isBudgeted() &&
                    (!proxy->priv().isInstantiated() ||
                     !proxy->priv().peekSurface()->resourcePriv().refsWrappedObjects())) {
        return false;
    }

=======
#ifdef SK_DEBUG
    {
        GrContext* direct = fImageContext->priv().asDirectContext();
        if (direct) {
            GrResourceCache* resourceCache = direct->priv().getResourceCache();
            // If there is already a GrResource with this key then the caller has violated the
            // normal usage pattern of uniquely keyed resources (e.g., they have created one w/o
            // first seeing if it already existed in the cache).
            SkASSERT(!resourceCache->findAndRefUniqueResource(key));
        }
    }
#endif

>>>>>>> upstream-releases
    SkASSERT(!fUniquelyKeyedProxies.find(key));     // multiple proxies can't get the same key

    proxy->cacheAccess().setUniqueKey(this, key);
    SkASSERT(proxy->getUniqueKey() == key);
    fUniquelyKeyedProxies.add(proxy);
    return true;
}

void GrProxyProvider::adoptUniqueKeyFromSurface(GrTextureProxy* proxy, const GrSurface* surf) {
    SkASSERT(surf->getUniqueKey().isValid());
    proxy->cacheAccess().setUniqueKey(this, surf->getUniqueKey());
    SkASSERT(proxy->getUniqueKey() == surf->getUniqueKey());
    // multiple proxies can't get the same key
    SkASSERT(!fUniquelyKeyedProxies.find(surf->getUniqueKey()));
    fUniquelyKeyedProxies.add(proxy);
}

void GrProxyProvider::removeUniqueKeyFromProxy(GrTextureProxy* proxy) {
    ASSERT_SINGLE_OWNER
    SkASSERT(proxy);
    SkASSERT(proxy->getUniqueKey().isValid());

    if (this->isAbandoned()) {
        return;
    }
<<<<<<< HEAD

    this->processInvalidProxyUniqueKey(key, proxy, true);
||||||| merged common ancestors
    this->processInvalidProxyUniqueKey(key, proxy, true);
=======

    this->processInvalidUniqueKey(proxy->getUniqueKey(), proxy, InvalidateGPUResource::kYes);
>>>>>>> upstream-releases
}

sk_sp<GrTextureProxy> GrProxyProvider::findProxyByUniqueKey(const GrUniqueKey& key,
                                                            GrSurfaceOrigin origin) {
    ASSERT_SINGLE_OWNER

    if (this->isAbandoned()) {
        return nullptr;
    }

    sk_sp<GrTextureProxy> result = sk_ref_sp(fUniquelyKeyedProxies.find(key));
    if (result) {
        SkASSERT(result->origin() == origin);
    }
    return result;
}

///////////////////////////////////////////////////////////////////////////////

#if GR_TEST_UTILS
sk_sp<GrTextureProxy> GrProxyProvider::testingOnly_createInstantiatedProxy(
        const GrSurfaceDesc& desc, GrSurfaceOrigin origin, SkBackingFit fit, SkBudgeted budgeted) {
    GrContext* direct = fImageContext->priv().asDirectContext();
    if (!direct) {
        return nullptr;
    }

    GrResourceProvider* resourceProvider = direct->priv().resourceProvider();
    sk_sp<GrTexture> tex;

    if (SkBackingFit::kApprox == fit) {
        tex = resourceProvider->createApproxTexture(desc, GrResourceProvider::Flags::kNone);
    } else {
        tex = resourceProvider->createTexture(desc, budgeted, GrResourceProvider::Flags::kNone);
    }
    if (!tex) {
        return nullptr;
    }

    return this->createWrapped(std::move(tex), origin);
}

sk_sp<GrTextureProxy> GrProxyProvider::testingOnly_createWrapped(sk_sp<GrTexture> tex,
                                                                 GrSurfaceOrigin origin) {
    return this->createWrapped(std::move(tex), origin);
}
#endif

sk_sp<GrTextureProxy> GrProxyProvider::createWrapped(sk_sp<GrTexture> tex, GrSurfaceOrigin origin) {
#ifdef SK_DEBUG
    if (tex->getUniqueKey().isValid()) {
        SkASSERT(!this->findProxyByUniqueKey(tex->getUniqueKey(), origin));
    }
#endif

    if (tex->asRenderTarget()) {
        return sk_sp<GrTextureProxy>(new GrTextureRenderTargetProxy(std::move(tex), origin));
    } else {
        return sk_sp<GrTextureProxy>(new GrTextureProxy(std::move(tex), origin));
    }
}

sk_sp<GrTextureProxy> GrProxyProvider::findOrCreateProxyByUniqueKey(const GrUniqueKey& key,
                                                                    GrSurfaceOrigin origin) {
    ASSERT_SINGLE_OWNER

    if (this->isAbandoned()) {
        return nullptr;
    }

    sk_sp<GrTextureProxy> result = this->findProxyByUniqueKey(key, origin);
    if (result) {
        return result;
    }

    GrContext* direct = fImageContext->priv().asDirectContext();
    if (!direct) {
        return nullptr;
    }

    GrResourceCache* resourceCache = direct->priv().getResourceCache();

    GrGpuResource* resource = resourceCache->findAndRefUniqueResource(key);
    if (!resource) {
        return nullptr;
    }

    sk_sp<GrTexture> texture(static_cast<GrSurface*>(resource)->asTexture());
    SkASSERT(texture);

    result = this->createWrapped(std::move(texture), origin);
    SkASSERT(result->getUniqueKey() == key);
    // createWrapped should've added this for us
    SkASSERT(fUniquelyKeyedProxies.find(key));
    return result;
}

sk_sp<GrTextureProxy> GrProxyProvider::createTextureProxy(sk_sp<SkImage> srcImage,
                                                          GrSurfaceDescFlags descFlags,
                                                          int sampleCnt,
                                                          SkBudgeted budgeted,
                                                          SkBackingFit fit,
                                                          GrInternalSurfaceFlags surfaceFlags) {
    ASSERT_SINGLE_OWNER
    SkASSERT(srcImage);

    if (this->isAbandoned()) {
        return nullptr;
    }

    SkImageInfo info = as_IB(srcImage)->onImageInfo();
    GrPixelConfig config = SkImageInfo2GrPixelConfig(info);

    if (kUnknown_GrPixelConfig == config) {
        return nullptr;
    }

<<<<<<< HEAD
    if (!this->caps()->isConfigTexturable(config)) {
        SkBitmap copy8888;
        if (!copy8888.tryAllocPixels(info.makeColorType(kRGBA_8888_SkColorType)) ||
            !srcImage->readPixels(copy8888.pixmap(), 0, 0)) {
            return nullptr;
        }
        copy8888.setImmutable();
        srcImage = SkMakeImageFromRasterBitmap(copy8888, kNever_SkCopyPixelsMode);
        config = kRGBA_8888_GrPixelConfig;
    }

    if (SkToBool(descFlags & kRenderTarget_GrSurfaceFlag)) {
||||||| merged common ancestors
    if (SkToBool(flags & kRenderTarget_GrSurfaceFlag)) {
=======
    GrBackendFormat format = this->caps()->getBackendFormatFromColorType(info.colorType());
    if (!format.isValid()) {
        return nullptr;
    }

    if (!this->caps()->isConfigTexturable(config)) {
        SkBitmap copy8888;
        if (!copy8888.tryAllocPixels(info.makeColorType(kRGBA_8888_SkColorType)) ||
            !srcImage->readPixels(copy8888.pixmap(), 0, 0)) {
            return nullptr;
        }
        copy8888.setImmutable();
        srcImage = SkMakeImageFromRasterBitmap(copy8888, kNever_SkCopyPixelsMode);
        config = kRGBA_8888_GrPixelConfig;
    }

    if (SkToBool(descFlags & kRenderTarget_GrSurfaceFlag)) {
>>>>>>> upstream-releases
        sampleCnt = this->caps()->getRenderTargetSampleCount(sampleCnt, config);
        if (!sampleCnt) {
            return nullptr;
        }
    }

<<<<<<< HEAD
    if (SkToBool(descFlags & kRenderTarget_GrSurfaceFlag)) {
        if (fCaps->usesMixedSamples() && sampleCnt > 1) {
            surfaceFlags |= GrInternalSurfaceFlags::kMixedSampled;
        }
        if (fCaps->maxWindowRectangles() > 0) {
            surfaceFlags |= GrInternalSurfaceFlags::kWindowRectsSupport;
||||||| merged common ancestors
    GrRenderTargetFlags renderTargetFlags = GrRenderTargetFlags::kNone;
    if (SkToBool(flags & kRenderTarget_GrSurfaceFlag)) {
        if (fCaps->usesMixedSamples() && sampleCnt > 1) {
            renderTargetFlags |= GrRenderTargetFlags::kMixedSampled;
        }
        if (fCaps->maxWindowRectangles() > 0) {
            renderTargetFlags |= GrRenderTargetFlags::kWindowRectsSupport;
=======
    if (SkToBool(descFlags & kRenderTarget_GrSurfaceFlag)) {
        if (this->caps()->usesMixedSamples() && sampleCnt > 1) {
            surfaceFlags |= GrInternalSurfaceFlags::kMixedSampled;
>>>>>>> upstream-releases
        }
    }

    GrSurfaceDesc desc;
    desc.fWidth = srcImage->width();
    desc.fHeight = srcImage->height();
    desc.fFlags = descFlags;
    desc.fSampleCnt = sampleCnt;
    desc.fConfig = config;

    sk_sp<GrTextureProxy> proxy = this->createLazyProxy(
<<<<<<< HEAD
            [desc, budgeted, srcImage, fit, surfaceFlags](GrResourceProvider* resourceProvider) {
                if (!resourceProvider) {
                    // Nothing to clean up here. Once the proxy (and thus lambda) is deleted the ref
                    // on srcImage will be released.
                    return sk_sp<GrTexture>();
                }
||||||| merged common ancestors
            [desc, budgeted, srcImage, fit]
            (GrResourceProvider* resourceProvider) {
                if (!resourceProvider) {
                    // Nothing to clean up here. Once the proxy (and thus lambda) is deleted the ref
                    // on srcImage will be released.
                    return sk_sp<GrTexture>();
                }
=======
            [desc, budgeted, srcImage, fit, surfaceFlags](GrResourceProvider* resourceProvider) {
>>>>>>> upstream-releases
                SkPixmap pixMap;
                SkAssertResult(srcImage->peekPixels(&pixMap));
                GrMipLevel mipLevel = { pixMap.addr(), pixMap.rowBytes() };

<<<<<<< HEAD
                auto resourceProviderFlags = GrResourceProvider::Flags::kNone;
                if (surfaceFlags & GrInternalSurfaceFlags::kNoPendingIO) {
                    resourceProviderFlags |= GrResourceProvider::Flags::kNoPendingIO;
                }
                return resourceProvider->createTexture(desc, budgeted, fit, mipLevel,
                                                       resourceProviderFlags);
            },
            desc, kTopLeft_GrSurfaceOrigin, GrMipMapped::kNo, GrTextureType::k2D, surfaceFlags, fit,
            budgeted);

    if (!proxy) {
        return nullptr;
    }
||||||| merged common ancestors
                return resourceProvider->createTexture(desc, budgeted, fit, mipLevel);
            }, desc, GrMipMapped::kNo, renderTargetFlags, fit, budgeted);
=======
                auto resourceProviderFlags = GrResourceProvider::Flags::kNone;
                if (surfaceFlags & GrInternalSurfaceFlags::kNoPendingIO) {
                    resourceProviderFlags |= GrResourceProvider::Flags::kNoPendingIO;
                }
                return resourceProvider->createTexture(desc, budgeted, fit, mipLevel,
                                                       resourceProviderFlags);
            },
            format, desc, kTopLeft_GrSurfaceOrigin, GrMipMapped::kNo, surfaceFlags, fit, budgeted);

    if (!proxy) {
        return nullptr;
    }

    GrContext* direct = fImageContext->priv().asDirectContext();
    if (direct) {
        GrResourceProvider* resourceProvider = direct->priv().resourceProvider();
>>>>>>> upstream-releases

        // In order to reuse code we always create a lazy proxy. When we aren't in DDL mode however
        // we're better off instantiating the proxy immediately here.
        if (!proxy->priv().doLazyInstantiation(resourceProvider)) {
            return nullptr;
        }
    }

    SkASSERT(proxy->width() == desc.fWidth);
    SkASSERT(proxy->height() == desc.fHeight);
    return proxy;
}

<<<<<<< HEAD
sk_sp<GrTextureProxy> GrProxyProvider::createMipMapProxy(const GrSurfaceDesc& desc,
                                                         GrSurfaceOrigin origin,
||||||| merged common ancestors
sk_sp<GrTextureProxy> GrProxyProvider::createMipMapProxy(const GrSurfaceDesc& desc,
=======
sk_sp<GrTextureProxy> GrProxyProvider::createMipMapProxy(const GrBackendFormat& format,
                                                         const GrSurfaceDesc& desc,
                                                         GrSurfaceOrigin origin,
>>>>>>> upstream-releases
                                                         SkBudgeted budgeted) {
    ASSERT_SINGLE_OWNER

    if (this->isAbandoned()) {
        return nullptr;
    }

<<<<<<< HEAD
    return this->createProxy(desc, origin, GrMipMapped::kYes, SkBackingFit::kExact, budgeted,
                             GrInternalSurfaceFlags::kNone);
||||||| merged common ancestors
    return this->createProxy(desc, GrMipMapped::kYes, SkBackingFit::kExact, budgeted, 0);
=======
    return this->createProxy(format, desc, origin, GrMipMapped::kYes, SkBackingFit::kExact,
                             budgeted, GrInternalSurfaceFlags::kNone);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
sk_sp<GrTextureProxy> GrProxyProvider::createMipMapProxyFromBitmap(const SkBitmap& bitmap) {
    if (!SkImageInfoIsValid(bitmap.info())) {
||||||| merged common ancestors
sk_sp<GrTextureProxy> GrProxyProvider::createMipMapProxyFromBitmap(const SkBitmap& bitmap,
                                                                   SkColorSpace* dstColorSpace) {
    SkDestinationSurfaceColorMode mipColorMode = dstColorSpace
        ? SkDestinationSurfaceColorMode::kGammaAndColorSpaceAware
        : SkDestinationSurfaceColorMode::kLegacy;

    if (!SkImageInfoIsValid(bitmap.info(), mipColorMode)) {
        return nullptr;
    }

    SkPixmap pixmap;
    if (!bitmap.peekPixels(&pixmap)) {
        return nullptr;
    }

    ATRACE_ANDROID_FRAMEWORK("Upload MipMap Texture [%ux%u]", pixmap.width(), pixmap.height());
    sk_sp<SkMipMap> mipmaps(SkMipMap::Build(pixmap, mipColorMode, nullptr));
    if (!mipmaps) {
=======
sk_sp<GrTextureProxy> GrProxyProvider::createMipMapProxyFromBitmap(const SkBitmap& bitmap) {
    ASSERT_SINGLE_OWNER

    if (this->isAbandoned()) {
        return nullptr;
    }

    if (!SkImageInfoIsValid(bitmap.info())) {
>>>>>>> upstream-releases
        return nullptr;
    }

    ATRACE_ANDROID_FRAMEWORK("Upload MipMap Texture [%ux%u]", bitmap.width(), bitmap.height());

    // In non-ddl we will always instantiate right away. Thus we never want to copy the SkBitmap
    // even if its mutable. In ddl, if the bitmap is mutable then we must make a copy since the
    // upload of the data to the gpu can happen at anytime and the bitmap may change by then.
<<<<<<< HEAD
    SkCopyPixelsMode copyMode = this->recordingDDL() ? kIfMutable_SkCopyPixelsMode
                                                     : kNever_SkCopyPixelsMode;
||||||| merged common ancestors
    SkCopyPixelsMode copyMode = this->mutableBitmapsNeedCopy() ? kIfMutable_SkCopyPixelsMode
                                                               : kNever_SkCopyPixelsMode;
=======
    SkCopyPixelsMode copyMode = this->renderingDirectly() ? kNever_SkCopyPixelsMode
                                                          : kIfMutable_SkCopyPixelsMode;
>>>>>>> upstream-releases
    sk_sp<SkImage> baseLevel = SkMakeImageFromRasterBitmap(bitmap, copyMode);
    if (!baseLevel) {
        return nullptr;
    }

<<<<<<< HEAD
    // This was never going to have mips anyway
    if (0 == SkMipMap::ComputeLevelCount(baseLevel->width(), baseLevel->height())) {
        return this->createTextureProxy(baseLevel, kNone_GrSurfaceFlags, 1, SkBudgeted::kYes,
                                        SkBackingFit::kExact);
    }
||||||| merged common ancestors
    GrSurfaceDesc desc = GrImageInfoToSurfaceDesc(pixmap.info(), *this->caps());
=======
    // This was never going to have mips anyway
    if (0 == SkMipMap::ComputeLevelCount(baseLevel->width(), baseLevel->height())) {
        return this->createTextureProxy(baseLevel, kNone_GrSurfaceFlags, 1, SkBudgeted::kYes,
                                        SkBackingFit::kExact);
    }

    const GrBackendFormat format =
                            this->caps()->getBackendFormatFromColorType(bitmap.info().colorType());
    if (!format.isValid()) {
        return nullptr;
    }
>>>>>>> upstream-releases

    GrSurfaceDesc desc = GrImageInfoToSurfaceDesc(bitmap.info());
    if (!this->caps()->isConfigTexturable(desc.fConfig)) {
        SkBitmap copy8888;
        if (!copy8888.tryAllocPixels(bitmap.info().makeColorType(kRGBA_8888_SkColorType)) ||
            !bitmap.readPixels(copy8888.pixmap())) {
            return nullptr;
        }
        copy8888.setImmutable();
        baseLevel = SkMakeImageFromRasterBitmap(copy8888, kNever_SkCopyPixelsMode);
        desc.fConfig = kRGBA_8888_GrPixelConfig;
    }

    SkPixmap pixmap;
    SkAssertResult(baseLevel->peekPixels(&pixmap));
    sk_sp<SkMipMap> mipmaps(SkMipMap::Build(pixmap, nullptr));
    if (!mipmaps) {
        return nullptr;
    }

    sk_sp<GrTextureProxy> proxy = this->createLazyProxy(
<<<<<<< HEAD
            [desc, baseLevel, mipmaps](GrResourceProvider* resourceProvider) {
                if (!resourceProvider) {
                    return sk_sp<GrTexture>();
                }

||||||| merged common ancestors
            [desc, baseLevel, mipmaps, mipColorMode]
            (GrResourceProvider* resourceProvider) {
                if (!resourceProvider) {
                    return sk_sp<GrTexture>();
                }

=======
            [desc, baseLevel, mipmaps](GrResourceProvider* resourceProvider) {
>>>>>>> upstream-releases
                const int mipLevelCount = mipmaps->countLevels() + 1;
                std::unique_ptr<GrMipLevel[]> texels(new GrMipLevel[mipLevelCount]);

                SkPixmap pixmap;
                SkAssertResult(baseLevel->peekPixels(&pixmap));

                // DDL TODO: Instead of copying all this info into GrMipLevels we should just plumb
                // the use of SkMipMap down through Ganesh.
                texels[0].fPixels = pixmap.addr();
                texels[0].fRowBytes = pixmap.rowBytes();

                for (int i = 1; i < mipLevelCount; ++i) {
                    SkMipMap::Level generatedMipLevel;
                    mipmaps->getLevel(i - 1, &generatedMipLevel);
                    texels[i].fPixels = generatedMipLevel.fPixmap.addr();
                    texels[i].fRowBytes = generatedMipLevel.fPixmap.rowBytes();
                    SkASSERT(texels[i].fPixels);
                }

                return resourceProvider->createTexture(desc, SkBudgeted::kYes, texels.get(),
<<<<<<< HEAD
                                                       mipLevelCount);
            },
            desc, kTopLeft_GrSurfaceOrigin, GrMipMapped::kYes, GrTextureType::k2D,
            SkBackingFit::kExact, SkBudgeted::kYes);

    if (!proxy) {
        return nullptr;
    }
||||||| merged common ancestors
                                                       mipLevelCount, mipColorMode);
            }, desc, GrMipMapped::kYes, SkBackingFit::kExact, SkBudgeted::kYes);
=======
                                                       mipLevelCount);
            },
            format, desc, kTopLeft_GrSurfaceOrigin, GrMipMapped::kYes, SkBackingFit::kExact,
            SkBudgeted::kYes);
>>>>>>> upstream-releases

    if (!proxy) {
        return nullptr;
    }

    GrContext* direct = fImageContext->priv().asDirectContext();
    if (direct) {
        GrResourceProvider* resourceProvider = direct->priv().resourceProvider();
        // In order to reuse code we always create a lazy proxy. When we aren't in DDL mode however
        // we're better off instantiating the proxy immediately here.
        if (!proxy->priv().doLazyInstantiation(resourceProvider)) {
            return nullptr;
        }
    }
    return proxy;
}

<<<<<<< HEAD
sk_sp<GrTextureProxy> GrProxyProvider::createProxy(const GrSurfaceDesc& desc,
                                                   GrSurfaceOrigin origin,
||||||| merged common ancestors
sk_sp<GrTextureProxy> GrProxyProvider::createProxy(const GrSurfaceDesc& desc,
=======
sk_sp<GrTextureProxy> GrProxyProvider::createProxy(const GrBackendFormat& format,
                                                   const GrSurfaceDesc& desc,
                                                   GrSurfaceOrigin origin,
>>>>>>> upstream-releases
                                                   GrMipMapped mipMapped,
                                                   SkBackingFit fit,
                                                   SkBudgeted budgeted,
                                                   GrInternalSurfaceFlags surfaceFlags) {
    if (GrMipMapped::kYes == mipMapped) {
        // SkMipMap doesn't include the base level in the level count so we have to add 1
        int mipCount = SkMipMap::ComputeLevelCount(desc.fWidth, desc.fHeight) + 1;
        if (1 == mipCount) {
            mipMapped = GrMipMapped::kNo;
        }
    }

    if (!this->caps()->validateSurfaceDesc(desc, mipMapped)) {
        return nullptr;
    }
    GrSurfaceDesc copyDesc = desc;
    if (desc.fFlags & kRenderTarget_GrSurfaceFlag) {
        copyDesc.fSampleCnt =
                this->caps()->getRenderTargetSampleCount(desc.fSampleCnt, desc.fConfig);
    }

    if (copyDesc.fFlags & kRenderTarget_GrSurfaceFlag) {
        // We know anything we instantiate later from this deferred path will be
        // both texturable and renderable
<<<<<<< HEAD
        return sk_sp<GrTextureProxy>(new GrTextureRenderTargetProxy(*this->caps(), copyDesc, origin,
                                                                    mipMapped, GrTextureType::k2D,
                                                                    fit, budgeted, surfaceFlags));
||||||| merged common ancestors
        return sk_sp<GrTextureProxy>(
                new GrTextureRenderTargetProxy(*this->caps(), copyDesc, mipMapped, fit, budgeted,
                                               flags));
=======
        return sk_sp<GrTextureProxy>(new GrTextureRenderTargetProxy(*this->caps(), format, copyDesc,
                                                                    origin, mipMapped,
                                                                    fit, budgeted, surfaceFlags));
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    return sk_sp<GrTextureProxy>(new GrTextureProxy(copyDesc, origin, mipMapped, GrTextureType::k2D,
                                                    fit, budgeted, surfaceFlags));
||||||| merged common ancestors
    return sk_sp<GrTextureProxy>(new GrTextureProxy(copyDesc, mipMapped, fit, budgeted, nullptr, 0,
                                                    flags));
=======
    return sk_sp<GrTextureProxy>(new GrTextureProxy(format, copyDesc, origin, mipMapped,
                                                    fit, budgeted, surfaceFlags));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
sk_sp<GrTextureProxy> GrProxyProvider::wrapBackendTexture(const GrBackendTexture& backendTex,
                                                          GrSurfaceOrigin origin,
                                                          GrWrapOwnership ownership,
                                                          ReleaseProc releaseProc,
                                                          ReleaseContext releaseCtx) {
    if (this->isAbandoned()) {
||||||| merged common ancestors
sk_sp<GrTextureProxy> GrProxyProvider::createWrappedTextureProxy(
                                                         const GrBackendTexture& backendTex,
                                                         GrSurfaceOrigin origin,
                                                         GrWrapOwnership ownership,
                                                         ReleaseProc releaseProc,
                                                         ReleaseContext releaseCtx) {
    if (this->isAbandoned()) {
=======
sk_sp<GrTextureProxy> GrProxyProvider::createProxy(sk_sp<SkData> data, const GrSurfaceDesc& desc) {
    if (!this->caps()->isConfigTexturable(desc.fConfig)) {
>>>>>>> upstream-releases
        return nullptr;
    }

<<<<<<< HEAD
    // This is only supported on a direct GrContext.
    if (!fResourceProvider) {
        return nullptr;
    }

    sk_sp<GrTexture> tex = fResourceProvider->wrapBackendTexture(backendTex, ownership);
    if (!tex) {
        return nullptr;
    }

    sk_sp<GrReleaseProcHelper> releaseHelper;
    if (releaseProc) {
        releaseHelper.reset(new GrReleaseProcHelper(releaseProc, releaseCtx));
        // This gives the texture a ref on the releaseHelper
        tex->setRelease(releaseHelper);
    }
||||||| merged common ancestors
    GrSurfaceDesc desc;
    desc.fOrigin = origin;
    desc.fWidth = backendTex.width();
    desc.fHeight = backendTex.height();
    desc.fConfig = backendTex.config();
    GrMipMapped mipMapped = backendTex.hasMipMaps() ? GrMipMapped::kYes : GrMipMapped::kNo;

    sk_sp<GrReleaseProcHelper> releaseHelper;
    if (releaseProc) {
        releaseHelper.reset(new GrReleaseProcHelper(releaseProc, releaseCtx));
    }
=======
    const GrColorType ct = GrPixelConfigToColorType(desc.fConfig);
    const GrBackendFormat format =
                            this->caps()->getBackendFormatFromGrColorType(ct, GrSRGBEncoded::kNo);
>>>>>>> upstream-releases

<<<<<<< HEAD
    SkASSERT(!tex->asRenderTarget());  // Strictly a GrTexture
    // Make sure we match how we created the proxy with SkBudgeted::kNo
    SkASSERT(SkBudgeted::kNo == tex->resourcePriv().isBudgeted());

    return sk_sp<GrTextureProxy>(new GrTextureProxy(std::move(tex), origin));
||||||| merged common ancestors
    sk_sp<GrTextureProxy> proxy = this->createLazyProxy(
            [backendTex, ownership, releaseHelper]
            (GrResourceProvider* resourceProvider) {
                if (!resourceProvider) {
                    // If this had a releaseHelper it will get unrefed when we delete this lambda
                    // and will call the release proc so that the client knows they can free the
                    // underlying backend object.
                    return sk_sp<GrTexture>();
                }

                sk_sp<GrTexture> tex = resourceProvider->wrapBackendTexture(backendTex,
                                                                            ownership);
                if (!tex) {
                    return sk_sp<GrTexture>();
                }
                if (releaseHelper) {
                    // This gives the texture a ref on the releaseHelper
                    tex->setRelease(releaseHelper);
                }
                SkASSERT(!tex->asRenderTarget());   // Strictly a GrTexture
                // Make sure we match how we created the proxy with SkBudgeted::kNo
                SkASSERT(SkBudgeted::kNo == tex->resourcePriv().isBudgeted());

                return tex;
            }, desc, mipMapped, SkBackingFit::kExact, SkBudgeted::kNo);

    if (fResourceProvider) {
        // In order to reuse code we always create a lazy proxy. When we aren't in DDL mode however,
        // we're better off instantiating the proxy immediately here.
        if (!proxy->priv().doLazyInstantiation(fResourceProvider)) {
            return nullptr;
        }
    }
    return proxy;
=======
    sk_sp<GrTextureProxy> proxy = this->createLazyProxy(
        [desc, data](GrResourceProvider* resourceProvider) {
            GrMipLevel texels;
            texels.fPixels = data->data();
            texels.fRowBytes = GrBytesPerPixel(desc.fConfig)*desc.fWidth;
            return resourceProvider->createTexture(desc, SkBudgeted::kYes, &texels, 1);
        },
        format, desc, kTopLeft_GrSurfaceOrigin, GrMipMapped::kNo, SkBackingFit::kExact,
        SkBudgeted::kYes);

    if (!proxy) {
        return nullptr;
    }

    GrContext* direct = fImageContext->priv().asDirectContext();
    if (direct) {
        GrResourceProvider* resourceProvider = direct->priv().resourceProvider();
        // In order to reuse code we always create a lazy proxy. When we aren't in DDL mode however
        // we're better off instantiating the proxy immediately here.
        if (!proxy->priv().doLazyInstantiation(resourceProvider)) {
            return nullptr;
        }
    }
    return proxy;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
sk_sp<GrTextureProxy> GrProxyProvider::wrapRenderableBackendTexture(
        const GrBackendTexture& backendTex, GrSurfaceOrigin origin, int sampleCnt,
        GrWrapOwnership ownership) {
||||||| merged common ancestors
sk_sp<GrTextureProxy> GrProxyProvider::createWrappedTextureProxy(const GrBackendTexture& backendTex,
                                                                 GrSurfaceOrigin origin,
                                                                 int sampleCnt) {
=======
sk_sp<GrTextureProxy> GrProxyProvider::wrapBackendTexture(const GrBackendTexture& backendTex,
                                                          GrSurfaceOrigin origin,
                                                          GrWrapOwnership ownership,
                                                          GrWrapCacheable cacheable,
                                                          GrIOType ioType,
                                                          ReleaseProc releaseProc,
                                                          ReleaseContext releaseCtx) {
    SkASSERT(ioType != kWrite_GrIOType);
>>>>>>> upstream-releases
    if (this->isAbandoned()) {
        return nullptr;
    }

<<<<<<< HEAD
    // This is only supported on a direct GrContext.
    if (!fResourceProvider) {
        return nullptr;
    }

    sampleCnt = this->caps()->getRenderTargetSampleCount(sampleCnt, backendTex.config());
    if (!sampleCnt) {
||||||| merged common ancestors
    sampleCnt = this->caps()->getRenderTargetSampleCount(sampleCnt, backendTex.config());
    if (!sampleCnt) {
=======
    // This is only supported on a direct GrContext.
    GrContext* direct = fImageContext->priv().asDirectContext();
    if (!direct) {
>>>>>>> upstream-releases
        return nullptr;
    }

<<<<<<< HEAD
    sk_sp<GrTexture> tex =
            fResourceProvider->wrapRenderableBackendTexture(backendTex, sampleCnt, ownership);
    if (!tex) {
        return nullptr;
||||||| merged common ancestors
    GrSurfaceDesc desc;
    desc.fOrigin = origin;
    desc.fWidth = backendTex.width();
    desc.fHeight = backendTex.height();
    desc.fConfig = backendTex.config();
    desc.fFlags = kRenderTarget_GrSurfaceFlag;
    desc.fSampleCnt = sampleCnt;
    GrMipMapped mipMapped = backendTex.hasMipMaps() ? GrMipMapped::kYes : GrMipMapped::kNo;

    GrRenderTargetFlags renderTargetFlags = GrRenderTargetFlags::kNone;
    if (fCaps->usesMixedSamples() && sampleCnt > 1) {
        renderTargetFlags |= GrRenderTargetFlags::kMixedSampled;
    }
    if (fCaps->maxWindowRectangles() > 0) {
        renderTargetFlags |= GrRenderTargetFlags::kWindowRectsSupport;
=======
    GrResourceProvider* resourceProvider = direct->priv().resourceProvider();

    sk_sp<GrTexture> tex =
            resourceProvider->wrapBackendTexture(backendTex, ownership, cacheable, ioType);
    if (!tex) {
        return nullptr;
    }

    if (releaseProc) {
        tex->setRelease(releaseProc, releaseCtx);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    SkASSERT(tex->asRenderTarget());  // A GrTextureRenderTarget
    // Make sure we match how we created the proxy with SkBudgeted::kNo
    SkASSERT(SkBudgeted::kNo == tex->resourcePriv().isBudgeted());
||||||| merged common ancestors
    sk_sp<GrTextureProxy> proxy = this->createLazyProxy(
            [backendTex, sampleCnt] (GrResourceProvider* resourceProvider) {
                if (!resourceProvider) {
                    return sk_sp<GrTexture>();
                }

                sk_sp<GrTexture> tex = resourceProvider->wrapRenderableBackendTexture(backendTex,
                                                                                      sampleCnt);
                if (!tex) {
                    return sk_sp<GrTexture>();
                }
                SkASSERT(tex->asRenderTarget());   // A GrTextureRenderTarget
                // Make sure we match how we created the proxy with SkBudgeted::kNo
                SkASSERT(SkBudgeted::kNo == tex->resourcePriv().isBudgeted());

                return tex;
            }, desc, mipMapped, renderTargetFlags, SkBackingFit::kExact, SkBudgeted::kNo);
=======
    SkASSERT(!tex->asRenderTarget());  // Strictly a GrTexture
    // Make sure we match how we created the proxy with SkBudgeted::kNo
    SkASSERT(GrBudgetedType::kBudgeted != tex->resourcePriv().budgetedType());

    return sk_sp<GrTextureProxy>(new GrTextureProxy(std::move(tex), origin));
}

sk_sp<GrTextureProxy> GrProxyProvider::wrapRenderableBackendTexture(
        const GrBackendTexture& backendTex, GrSurfaceOrigin origin, int sampleCnt,
        GrWrapOwnership ownership, GrWrapCacheable cacheable, ReleaseProc releaseProc,
        ReleaseContext releaseCtx) {
    if (this->isAbandoned()) {
        return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    return sk_sp<GrTextureProxy>(new GrTextureRenderTargetProxy(std::move(tex), origin));
}
||||||| merged common ancestors
    if (fResourceProvider) {
        // In order to reuse code we always create a lazy proxy. When we aren't in DDL mode however,
        // we're better off instantiating the proxy immediately here.
        if (!proxy->priv().doLazyInstantiation(fResourceProvider)) {
            return nullptr;
        }
    }
    return proxy;
}
=======
    // This is only supported on a direct GrContext.
    GrContext* direct = fImageContext->priv().asDirectContext();
    if (!direct) {
        return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
sk_sp<GrSurfaceProxy> GrProxyProvider::wrapBackendRenderTarget(
        const GrBackendRenderTarget& backendRT, GrSurfaceOrigin origin) {
    if (this->isAbandoned()) {
||||||| merged common ancestors
sk_sp<GrSurfaceProxy> GrProxyProvider::createWrappedRenderTargetProxy(
                                                             const GrBackendRenderTarget& backendRT,
                                                             GrSurfaceOrigin origin) {
    if (this->isAbandoned()) {
=======
    GrResourceProvider* resourceProvider = direct->priv().resourceProvider();

    sampleCnt = this->caps()->getRenderTargetSampleCount(sampleCnt, backendTex.config());
    if (!sampleCnt) {
>>>>>>> upstream-releases
        return nullptr;
    }

<<<<<<< HEAD
    // This is only supported on a direct GrContext.
    if (!fResourceProvider) {
        return nullptr;
||||||| merged common ancestors
    GrSurfaceDesc desc;
    desc.fOrigin = origin;
    desc.fWidth = backendRT.width();
    desc.fHeight = backendRT.height();
    desc.fConfig = backendRT.config();
    desc.fFlags = kRenderTarget_GrSurfaceFlag;
    desc.fSampleCnt = backendRT.sampleCnt();

    GrRenderTargetFlags renderTargetFlags = GrRenderTargetFlags::kNone;
    if (fCaps->isMixedSamplesSupportedForRT(backendRT) && backendRT.sampleCnt() > 1) {
        renderTargetFlags |= GrRenderTargetFlags::kMixedSampled;
    }
    if (fCaps->isWindowRectanglesSupportedForRT(backendRT)) {
        renderTargetFlags |= GrRenderTargetFlags::kWindowRectsSupport;
=======
    sk_sp<GrTexture> tex = resourceProvider->wrapRenderableBackendTexture(backendTex, sampleCnt,
                                                                          ownership, cacheable);
    if (!tex) {
        return nullptr;
    }

    if (releaseProc) {
        tex->setRelease(releaseProc, releaseCtx);
    }

    SkASSERT(tex->asRenderTarget());  // A GrTextureRenderTarget
    // Make sure we match how we created the proxy with SkBudgeted::kNo
    SkASSERT(GrBudgetedType::kBudgeted != tex->resourcePriv().budgetedType());

    return sk_sp<GrTextureProxy>(new GrTextureRenderTargetProxy(std::move(tex), origin));
}

sk_sp<GrSurfaceProxy> GrProxyProvider::wrapBackendRenderTarget(
        const GrBackendRenderTarget& backendRT, GrSurfaceOrigin origin, ReleaseProc releaseProc,
        ReleaseContext releaseCtx) {
    if (this->isAbandoned()) {
        return nullptr;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    sk_sp<GrRenderTarget> rt = fResourceProvider->wrapBackendRenderTarget(backendRT);
    if (!rt) {
        return nullptr;
||||||| merged common ancestors
    sk_sp<GrRenderTargetProxy> proxy = this->createLazyRenderTargetProxy(
            [backendRT] (GrResourceProvider* resourceProvider) {
                if (!resourceProvider) {
                    return sk_sp<GrRenderTarget>();
                }

                sk_sp<GrRenderTarget> rt = resourceProvider->wrapBackendRenderTarget(backendRT);
                if (!rt) {
                    return sk_sp<GrRenderTarget>();
                }
                SkASSERT(!rt->asTexture());   // A GrRenderTarget that's not textureable
                SkASSERT(!rt->getUniqueKey().isValid());
                // Make sure we match how we created the proxy with SkBudgeted::kNo
                SkASSERT(SkBudgeted::kNo == rt->resourcePriv().isBudgeted());

                return rt;
            }, desc, renderTargetFlags, Textureable::kNo, GrMipMapped::kNo, SkBackingFit::kExact,
               SkBudgeted::kNo);

    if (fResourceProvider) {
        // In order to reuse code we always create a lazy proxy. When we aren't in DDL mode however,
        // we're better off instantiating the proxy immediately here.
        if (!proxy->priv().doLazyInstantiation(fResourceProvider)) {
            return nullptr;
        }
=======
    // This is only supported on a direct GrContext.
    GrContext* direct = fImageContext->priv().asDirectContext();
    if (!direct) {
        return nullptr;
    }

    GrResourceProvider* resourceProvider = direct->priv().resourceProvider();

    sk_sp<GrRenderTarget> rt = resourceProvider->wrapBackendRenderTarget(backendRT);
    if (!rt) {
        return nullptr;
    }

    if (releaseProc) {
        rt->setRelease(releaseProc, releaseCtx);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    SkASSERT(!rt->asTexture());  // A GrRenderTarget that's not textureable
    SkASSERT(!rt->getUniqueKey().isValid());
    // Make sure we match how we created the proxy with SkBudgeted::kNo
    SkASSERT(SkBudgeted::kNo == rt->resourcePriv().isBudgeted());

    return sk_sp<GrRenderTargetProxy>(new GrRenderTargetProxy(std::move(rt), origin));
||||||| merged common ancestors
    return proxy;
=======

    SkASSERT(!rt->asTexture());  // A GrRenderTarget that's not textureable
    SkASSERT(!rt->getUniqueKey().isValid());
    // Make sure we match how we created the proxy with SkBudgeted::kNo
    SkASSERT(GrBudgetedType::kBudgeted != rt->resourcePriv().budgetedType());

    return sk_sp<GrRenderTargetProxy>(new GrRenderTargetProxy(std::move(rt), origin));
>>>>>>> upstream-releases
}

sk_sp<GrSurfaceProxy> GrProxyProvider::wrapBackendTextureAsRenderTarget(
        const GrBackendTexture& backendTex, GrSurfaceOrigin origin, int sampleCnt) {
    if (this->isAbandoned()) {
        return nullptr;
    }

<<<<<<< HEAD
    // This is only supported on a direct GrContext.
    if (!fResourceProvider) {
||||||| merged common ancestors
    sampleCnt = this->caps()->getRenderTargetSampleCount(sampleCnt, backendTex.config());
    if (!sampleCnt) {
=======
    // This is only supported on a direct GrContext.
    GrContext* direct = fImageContext->priv().asDirectContext();
    if (!direct) {
>>>>>>> upstream-releases
        return nullptr;
    }

<<<<<<< HEAD
    sk_sp<GrRenderTarget> rt =
            fResourceProvider->wrapBackendTextureAsRenderTarget(backendTex, sampleCnt);
    if (!rt) {
        return nullptr;
||||||| merged common ancestors
    GrSurfaceDesc desc;
    desc.fOrigin = origin;
    desc.fWidth = backendTex.width();
    desc.fHeight = backendTex.height();
    desc.fConfig = backendTex.config();
    desc.fFlags = kRenderTarget_GrSurfaceFlag;
    desc.fSampleCnt = sampleCnt;

    GrRenderTargetFlags renderTargetFlags = GrRenderTargetFlags::kNone;
    if (fCaps->usesMixedSamples() && sampleCnt > 1) {
        renderTargetFlags |= GrRenderTargetFlags::kMixedSampled;
    }
    if (fCaps->maxWindowRectangles() > 0) {
        renderTargetFlags |= GrRenderTargetFlags::kWindowRectsSupport;
=======
    GrResourceProvider* resourceProvider = direct->priv().resourceProvider();

    sk_sp<GrRenderTarget> rt =
            resourceProvider->wrapBackendTextureAsRenderTarget(backendTex, sampleCnt);
    if (!rt) {
        return nullptr;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    SkASSERT(!rt->asTexture());  // A GrRenderTarget that's not textureable
    SkASSERT(!rt->getUniqueKey().isValid());
    // Make sure we match how we created the proxy with SkBudgeted::kNo
    SkASSERT(SkBudgeted::kNo == rt->resourcePriv().isBudgeted());
||||||| merged common ancestors
=======
    SkASSERT(!rt->asTexture());  // A GrRenderTarget that's not textureable
    SkASSERT(!rt->getUniqueKey().isValid());
    // This proxy should be unbudgeted because we're just wrapping an external resource
    SkASSERT(GrBudgetedType::kBudgeted != rt->resourcePriv().budgetedType());
>>>>>>> upstream-releases

<<<<<<< HEAD
    return sk_sp<GrSurfaceProxy>(new GrRenderTargetProxy(std::move(rt), origin));
}
||||||| merged common ancestors
    sk_sp<GrRenderTargetProxy> proxy = this->createLazyRenderTargetProxy(
            [backendTex, sampleCnt] (GrResourceProvider* resourceProvider) {
                if (!resourceProvider) {
                    return sk_sp<GrRenderTarget>();
                }

                sk_sp<GrRenderTarget> rt = resourceProvider->wrapBackendTextureAsRenderTarget(
                        backendTex, sampleCnt);
                if (!rt) {
                    return sk_sp<GrRenderTarget>();
                }
                SkASSERT(!rt->asTexture());   // A GrRenderTarget that's not textureable
                SkASSERT(!rt->getUniqueKey().isValid());
                // Make sure we match how we created the proxy with SkBudgeted::kNo
                SkASSERT(SkBudgeted::kNo == rt->resourcePriv().isBudgeted());

                return rt;
            }, desc, renderTargetFlags, Textureable::kNo, GrMipMapped::kNo, SkBackingFit::kExact,
               SkBudgeted::kNo);
=======
    return sk_sp<GrSurfaceProxy>(new GrRenderTargetProxy(std::move(rt), origin));
}

sk_sp<GrRenderTargetProxy> GrProxyProvider::wrapVulkanSecondaryCBAsRenderTarget(
        const SkImageInfo& imageInfo, const GrVkDrawableInfo& vkInfo) {
    if (this->isAbandoned()) {
        return nullptr;
    }

    // This is only supported on a direct GrContext.
    GrContext* direct = fImageContext->priv().asDirectContext();
    if (!direct) {
        return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
sk_sp<GrTextureProxy> GrProxyProvider::createLazyProxy(LazyInstantiateCallback&& callback,
                                                       const GrSurfaceDesc& desc,
                                                       GrSurfaceOrigin origin,
                                                       GrMipMapped mipMapped,
                                                       GrTextureType textureType,
                                                       SkBackingFit fit,
                                                       SkBudgeted budgeted) {
    return this->createLazyProxy(std::move(callback), desc, origin, mipMapped, textureType,
                                 GrInternalSurfaceFlags::kNone, fit, budgeted);
||||||| merged common ancestors
    if (fResourceProvider) {
        // In order to reuse code we always create a lazy proxy. When we aren't in DDL mode however,
        // we're better off instantiating the proxy immediately here.
        if (!proxy->priv().doLazyInstantiation(fResourceProvider)) {
            return nullptr;
        }
    }
    return proxy;
=======
    GrResourceProvider* resourceProvider = direct->priv().resourceProvider();

    sk_sp<GrRenderTarget> rt = resourceProvider->wrapVulkanSecondaryCBAsRenderTarget(imageInfo,
                                                                                     vkInfo);
    if (!rt) {
        return nullptr;
    }

    SkASSERT(!rt->asTexture());  // A GrRenderTarget that's not textureable
    SkASSERT(!rt->getUniqueKey().isValid());
    // This proxy should be unbudgeted because we're just wrapping an external resource
    SkASSERT(GrBudgetedType::kBudgeted != rt->resourcePriv().budgetedType());

    // All Vulkan surfaces uses top left origins.
    return sk_sp<GrRenderTargetProxy>(
            new GrRenderTargetProxy(std::move(rt),
                                    kTopLeft_GrSurfaceOrigin,
                                    GrRenderTargetProxy::WrapsVkSecondaryCB::kYes));
}

sk_sp<GrTextureProxy> GrProxyProvider::createLazyProxy(LazyInstantiateCallback&& callback,
                                                       const GrBackendFormat& format,
                                                       const GrSurfaceDesc& desc,
                                                       GrSurfaceOrigin origin,
                                                       GrMipMapped mipMapped,
                                                       SkBackingFit fit,
                                                       SkBudgeted budgeted) {
    return this->createLazyProxy(std::move(callback), format, desc, origin, mipMapped,
                                 GrInternalSurfaceFlags::kNone, fit, budgeted);
>>>>>>> upstream-releases
}

sk_sp<GrTextureProxy> GrProxyProvider::createLazyProxy(LazyInstantiateCallback&& callback,
                                                       const GrBackendFormat& format,
                                                       const GrSurfaceDesc& desc,
                                                       GrSurfaceOrigin origin,
                                                       GrMipMapped mipMapped,
<<<<<<< HEAD
                                                       GrTextureType textureType,
                                                       GrInternalSurfaceFlags surfaceFlags,
                                                       SkBackingFit fit,
                                                       SkBudgeted budgeted) {
    // For non-ddl draws always make lazy proxy's single use.
    LazyInstantiationType lazyType = fResourceProvider ? LazyInstantiationType::kSingleUse
                                                       : LazyInstantiationType::kMultipleUse;
    return this->createLazyProxy(std::move(callback), desc, origin, mipMapped, textureType,
                                 surfaceFlags, fit, budgeted, lazyType);
||||||| merged common ancestors
                                                       SkBackingFit fit, SkBudgeted budgeted) {
    return this->createLazyProxy(std::move(callback), desc, mipMapped, GrRenderTargetFlags::kNone,
                                 fit, budgeted);
=======
                                                       GrInternalSurfaceFlags surfaceFlags,
                                                       SkBackingFit fit,
                                                       SkBudgeted budgeted) {
    // For non-ddl draws always make lazy proxy's single use.
    LazyInstantiationType lazyType = this->renderingDirectly() ? LazyInstantiationType::kSingleUse
                                                               : LazyInstantiationType::kMultipleUse;
    return this->createLazyProxy(std::move(callback), format, desc, origin, mipMapped, surfaceFlags,
                                 fit, budgeted, lazyType);
>>>>>>> upstream-releases
}

sk_sp<GrTextureProxy> GrProxyProvider::createLazyProxy(LazyInstantiateCallback&& callback,
                                                       const GrBackendFormat& format,
                                                       const GrSurfaceDesc& desc,
                                                       GrSurfaceOrigin origin,
                                                       GrMipMapped mipMapped,
<<<<<<< HEAD
                                                       GrTextureType textureType,
                                                       GrInternalSurfaceFlags surfaceFlags,
                                                       SkBackingFit fit,
                                                       SkBudgeted budgeted,
                                                       LazyInstantiationType lazyType) {
||||||| merged common ancestors
                                                       GrRenderTargetFlags renderTargetFlags,
                                                       SkBackingFit fit, SkBudgeted budgeted) {
=======
                                                       GrInternalSurfaceFlags surfaceFlags,
                                                       SkBackingFit fit,
                                                       SkBudgeted budgeted,
                                                       LazyInstantiationType lazyType) {
>>>>>>> upstream-releases
    SkASSERT((desc.fWidth <= 0 && desc.fHeight <= 0) ||
             (desc.fWidth > 0 && desc.fHeight > 0));
<<<<<<< HEAD

    if (desc.fWidth > fCaps->maxTextureSize() || desc.fHeight > fCaps->maxTextureSize()) {
        return nullptr;
    }

||||||| merged common ancestors
    uint32_t flags = GrResourceProvider::kNoPendingIO_Flag;
=======

    if (desc.fWidth > this->caps()->maxTextureSize() ||
        desc.fHeight > this->caps()->maxTextureSize()) {
        return nullptr;
    }

>>>>>>> upstream-releases

#ifdef SK_DEBUG
    if (SkToBool(kRenderTarget_GrSurfaceFlag & desc.fFlags)) {
<<<<<<< HEAD
        if (SkToBool(surfaceFlags & GrInternalSurfaceFlags::kMixedSampled)) {
            SkASSERT(fCaps->usesMixedSamples() && desc.fSampleCnt > 1);
        }
        if (SkToBool(surfaceFlags & GrInternalSurfaceFlags::kWindowRectsSupport)) {
            SkASSERT(fCaps->maxWindowRectangles() > 0);
||||||| merged common ancestors
        if (SkToBool(renderTargetFlags & GrRenderTargetFlags::kMixedSampled)) {
            SkASSERT(fCaps->usesMixedSamples() && desc.fSampleCnt > 1);
        }
        if (SkToBool(renderTargetFlags & GrRenderTargetFlags::kWindowRectsSupport)) {
            SkASSERT(fCaps->maxWindowRectangles() > 0);
=======
        if (SkToBool(surfaceFlags & GrInternalSurfaceFlags::kMixedSampled)) {
            SkASSERT(this->caps()->usesMixedSamples() && desc.fSampleCnt > 1);
>>>>>>> upstream-releases
        }
    }
#endif

<<<<<<< HEAD
    return sk_sp<GrTextureProxy>(
            SkToBool(kRenderTarget_GrSurfaceFlag & desc.fFlags)
                    ? new GrTextureRenderTargetProxy(std::move(callback), lazyType, desc, origin,
                                                     mipMapped, textureType, fit, budgeted,
                                                     surfaceFlags)
                    : new GrTextureProxy(std::move(callback), lazyType, desc, origin, mipMapped,
                                         textureType, fit, budgeted, surfaceFlags));
||||||| merged common ancestors
    using LazyInstantiationType = GrSurfaceProxy::LazyInstantiationType;
    // For non-ddl draws always make lazy proxy's single use.
    LazyInstantiationType lazyType = fResourceProvider ? LazyInstantiationType::kSingleUse
                                                       : LazyInstantiationType::kMultipleUse;

    return sk_sp<GrTextureProxy>(SkToBool(kRenderTarget_GrSurfaceFlag & desc.fFlags) ?
                                 new GrTextureRenderTargetProxy(std::move(callback), lazyType, desc,
                                                                mipMapped, fit, budgeted, flags,
                                                                renderTargetFlags) :
                                 new GrTextureProxy(std::move(callback), lazyType, desc, mipMapped,
                                                    fit, budgeted, flags));
=======
    return sk_sp<GrTextureProxy>(
            SkToBool(kRenderTarget_GrSurfaceFlag & desc.fFlags)
                    ? new GrTextureRenderTargetProxy(std::move(callback), lazyType, format, desc,
                                                     origin, mipMapped, fit, budgeted, surfaceFlags)
                    : new GrTextureProxy(std::move(callback), lazyType, format, desc, origin,
                                         mipMapped, fit, budgeted, surfaceFlags));
>>>>>>> upstream-releases
}

sk_sp<GrRenderTargetProxy> GrProxyProvider::createLazyRenderTargetProxy(
<<<<<<< HEAD
        LazyInstantiateCallback&& callback, const GrSurfaceDesc& desc, GrSurfaceOrigin origin,
        GrInternalSurfaceFlags surfaceFlags, const TextureInfo* textureInfo, SkBackingFit fit,
        SkBudgeted budgeted) {
||||||| merged common ancestors
                                                LazyInstantiateCallback&& callback,
                                                const GrSurfaceDesc& desc,
                                                GrRenderTargetFlags renderTargetFlags,
                                                Textureable textureable,
                                                GrMipMapped mipMapped,
                                                SkBackingFit fit, SkBudgeted budgeted) {
=======
        LazyInstantiateCallback&& callback, const GrBackendFormat& format,
        const GrSurfaceDesc& desc, GrSurfaceOrigin origin, GrInternalSurfaceFlags surfaceFlags,
        const TextureInfo* textureInfo, SkBackingFit fit, SkBudgeted budgeted,
        bool wrapsVkSecondaryCB) {
>>>>>>> upstream-releases
    SkASSERT((desc.fWidth <= 0 && desc.fHeight <= 0) ||
             (desc.fWidth > 0 && desc.fHeight > 0));
<<<<<<< HEAD

    if (desc.fWidth > fCaps->maxRenderTargetSize() || desc.fHeight > fCaps->maxRenderTargetSize()) {
        return nullptr;
    }

||||||| merged common ancestors
=======

    if (desc.fWidth > this->caps()->maxRenderTargetSize() ||
        desc.fHeight > this->caps()->maxRenderTargetSize()) {
        return nullptr;
    }

>>>>>>> upstream-releases
    SkASSERT(SkToBool(kRenderTarget_GrSurfaceFlag & desc.fFlags));

#ifdef SK_DEBUG
<<<<<<< HEAD
    if (SkToBool(surfaceFlags & GrInternalSurfaceFlags::kMixedSampled)) {
        SkASSERT(fCaps->usesMixedSamples() && desc.fSampleCnt > 1);
    }
    if (SkToBool(surfaceFlags & GrInternalSurfaceFlags::kWindowRectsSupport)) {
        SkASSERT(fCaps->maxWindowRectangles() > 0);
||||||| merged common ancestors
    if (SkToBool(renderTargetFlags & GrRenderTargetFlags::kMixedSampled)) {
        SkASSERT(fCaps->usesMixedSamples() && desc.fSampleCnt > 1);
    }
    if (SkToBool(renderTargetFlags & GrRenderTargetFlags::kWindowRectsSupport)) {
        SkASSERT(fCaps->maxWindowRectangles() > 0);
=======
    if (SkToBool(surfaceFlags & GrInternalSurfaceFlags::kMixedSampled)) {
        SkASSERT(this->caps()->usesMixedSamples() && desc.fSampleCnt > 1);
>>>>>>> upstream-releases
    }
#endif

    using LazyInstantiationType = GrSurfaceProxy::LazyInstantiationType;
    // For non-ddl draws always make lazy proxy's single use.
    LazyInstantiationType lazyType = this->renderingDirectly() ? LazyInstantiationType::kSingleUse
                                                               : LazyInstantiationType::kMultipleUse;

<<<<<<< HEAD
    if (textureInfo) {
        return sk_sp<GrRenderTargetProxy>(new GrTextureRenderTargetProxy(
                std::move(callback), lazyType, desc, origin, textureInfo->fMipMapped,
                textureInfo->fTextureType, fit, budgeted, surfaceFlags));
||||||| merged common ancestors
    if (Textureable::kYes == textureable) {
        return sk_sp<GrRenderTargetProxy>(new GrTextureRenderTargetProxy(std::move(callback),
                                                                         lazyType, desc, mipMapped,
                                                                         fit, budgeted, flags,
                                                                         renderTargetFlags));
=======
    if (textureInfo) {
        // Wrapped vulkan secondary command buffers don't support texturing since we won't have an
        // actual VkImage to texture from.
        SkASSERT(!wrapsVkSecondaryCB);
        return sk_sp<GrRenderTargetProxy>(new GrTextureRenderTargetProxy(
                std::move(callback), lazyType, format, desc, origin, textureInfo->fMipMapped,
                fit, budgeted, surfaceFlags));
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    return sk_sp<GrRenderTargetProxy>(new GrRenderTargetProxy(
            std::move(callback), lazyType, desc, origin, fit, budgeted, surfaceFlags));
||||||| merged common ancestors
    return sk_sp<GrRenderTargetProxy>(new GrRenderTargetProxy(std::move(callback), lazyType, desc,
                                                              fit, budgeted, flags,
                                                              renderTargetFlags));
=======
    GrRenderTargetProxy::WrapsVkSecondaryCB vkSCB =
            wrapsVkSecondaryCB ? GrRenderTargetProxy::WrapsVkSecondaryCB::kYes
                               : GrRenderTargetProxy::WrapsVkSecondaryCB::kNo;

    return sk_sp<GrRenderTargetProxy>(new GrRenderTargetProxy(
            std::move(callback), lazyType, format, desc, origin, fit, budgeted, surfaceFlags,
            vkSCB));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
sk_sp<GrTextureProxy> GrProxyProvider::MakeFullyLazyProxy(LazyInstantiateCallback&& callback,
                                                          Renderable renderable,
                                                          GrSurfaceOrigin origin,
                                                          GrPixelConfig config,
                                                          const GrCaps& caps) {
||||||| merged common ancestors
sk_sp<GrTextureProxy> GrProxyProvider::createFullyLazyProxy(LazyInstantiateCallback&& callback,
                                                            Renderable renderable,
                                                            GrSurfaceOrigin origin,
                                                            GrPixelConfig config) {
=======
sk_sp<GrTextureProxy> GrProxyProvider::MakeFullyLazyProxy(LazyInstantiateCallback&& callback,
                                                          const GrBackendFormat& format,
                                                          Renderable renderable,
                                                          GrSurfaceOrigin origin,
                                                          GrPixelConfig config,
                                                          const GrCaps& caps) {
>>>>>>> upstream-releases
    GrSurfaceDesc desc;
    GrInternalSurfaceFlags surfaceFlags = GrInternalSurfaceFlags::kNoPendingIO;
    if (Renderable::kYes == renderable) {
        desc.fFlags = kRenderTarget_GrSurfaceFlag;
<<<<<<< HEAD
        if (caps.maxWindowRectangles() > 0) {
            surfaceFlags |= GrInternalSurfaceFlags::kWindowRectsSupport;
        }
||||||| merged common ancestors
        if (fCaps->maxWindowRectangles() > 0) {
            renderTargetFlags |= GrRenderTargetFlags::kWindowRectsSupport;
        }
=======
>>>>>>> upstream-releases
    }
    desc.fWidth = -1;
    desc.fHeight = -1;
    desc.fConfig = config;
    desc.fSampleCnt = 1;

<<<<<<< HEAD
    static constexpr auto kTextureType = GrTextureType::k2D;
    return sk_sp<GrTextureProxy>(
            (Renderable::kYes == renderable)
                    ? new GrTextureRenderTargetProxy(
                              std::move(callback), LazyInstantiationType::kSingleUse, desc, origin,
                              GrMipMapped::kNo, kTextureType, SkBackingFit::kApprox,
                              SkBudgeted::kYes, surfaceFlags)
                    : new GrTextureProxy(std::move(callback), LazyInstantiationType::kSingleUse,
                                         desc, origin, GrMipMapped::kNo, kTextureType,
                                         SkBackingFit::kApprox, SkBudgeted::kYes, surfaceFlags));
||||||| merged common ancestors
    return this->createLazyProxy(std::move(callback), desc, GrMipMapped::kNo, renderTargetFlags,
                                 SkBackingFit::kApprox, SkBudgeted::kYes);

=======
    return sk_sp<GrTextureProxy>(
            (Renderable::kYes == renderable)
                    ? new GrTextureRenderTargetProxy(
                              std::move(callback), LazyInstantiationType::kSingleUse, format, desc,
                              origin, GrMipMapped::kNo, SkBackingFit::kApprox, SkBudgeted::kYes,
                              surfaceFlags)
                    : new GrTextureProxy(std::move(callback), LazyInstantiationType::kSingleUse,
                                         format, desc, origin, GrMipMapped::kNo,
                                         SkBackingFit::kApprox, SkBudgeted::kYes, surfaceFlags));
>>>>>>> upstream-releases
}

bool GrProxyProvider::IsFunctionallyExact(GrSurfaceProxy* proxy) {
    const bool isInstantiated = proxy->isInstantiated();
    // A proxy is functionally exact if:
    //   it is exact (obvs)
    //   when it is instantiated it will be exact (i.e., power of two dimensions)
    //   it is already instantiated and the proxy covers the entire backing surface
    return proxy->priv().isExact() ||
           (!isInstantiated && SkIsPow2(proxy->width()) && SkIsPow2(proxy->height())) ||
           (isInstantiated && proxy->worstCaseWidth() == proxy->width() &&
                              proxy->worstCaseHeight() == proxy->height());
}

void GrProxyProvider::processInvalidUniqueKey(const GrUniqueKey& key, GrTextureProxy* proxy,
                                              InvalidateGPUResource invalidateGPUResource) {
    SkASSERT(key.isValid());

    if (!proxy) {
        proxy = fUniquelyKeyedProxies.find(key);
    }
    SkASSERT(!proxy || proxy->getUniqueKey() == key);

    // Locate the corresponding GrGpuResource (if it needs to be invalidated) before clearing the
    // proxy's unique key. We must do it in this order because 'key' may alias the proxy's key.
    sk_sp<GrGpuResource> invalidGpuResource;
    if (InvalidateGPUResource::kYes == invalidateGPUResource) {
        if (proxy && proxy->isInstantiated()) {
            invalidGpuResource = sk_ref_sp(proxy->peekSurface());
        }
        if (!invalidGpuResource) {
            GrContext* direct = fImageContext->priv().asDirectContext();
            if (direct) {
                GrResourceProvider* resourceProvider = direct->priv().resourceProvider();
                invalidGpuResource = resourceProvider->findByUniqueKey<GrGpuResource>(key);
            }
        }
        SkASSERT(!invalidGpuResource || invalidGpuResource->getUniqueKey() == key);
    }

    // Note: this method is called for the whole variety of GrGpuResources so often 'key'
    // will not be in 'fUniquelyKeyedProxies'.
    if (proxy) {
        fUniquelyKeyedProxies.remove(key);
        proxy->cacheAccess().clearUniqueKey();
    }

    if (invalidGpuResource) {
        invalidGpuResource->resourcePriv().removeUniqueKey();
    }
}

uint32_t GrProxyProvider::contextID() const {
    return fImageContext->priv().contextID();
}

const GrCaps* GrProxyProvider::caps() const {
    return fImageContext->priv().caps();
}

<<<<<<< HEAD
    if (invalidateSurface && proxy->isInstantiated()) {
        GrSurface* surface = proxy->peekSurface();
        if (surface) {
            surface->resourcePriv().removeUniqueKey();
        }
||||||| merged common ancestors
    if (invalidateSurface && proxy->priv().isInstantiated()) {
        GrSurface* surface = proxy->priv().peekSurface();
        if (surface) {
            surface->resourcePriv().removeUniqueKey();
        }
=======
sk_sp<const GrCaps> GrProxyProvider::refCaps() const {
    return fImageContext->priv().refCaps();
}

bool GrProxyProvider::isAbandoned() const {
    return fImageContext->priv().abandoned();
}

void GrProxyProvider::orphanAllUniqueKeys() {
    UniquelyKeyedProxyHash::Iter iter(&fUniquelyKeyedProxies);
    for (UniquelyKeyedProxyHash::Iter iter(&fUniquelyKeyedProxies); !iter.done(); ++iter) {
        GrTextureProxy& tmp = *iter;

        tmp.fProxyProvider = nullptr;
>>>>>>> upstream-releases
    }
}

void GrProxyProvider::orphanAllUniqueKeys() {
    UniquelyKeyedProxyHash::Iter iter(&fUniquelyKeyedProxies);
    for (UniquelyKeyedProxyHash::Iter iter(&fUniquelyKeyedProxies); !iter.done(); ++iter) {
        GrTextureProxy& tmp = *iter;

        tmp.fProxyProvider = nullptr;
    }
}

void GrProxyProvider::removeAllUniqueKeys() {
    UniquelyKeyedProxyHash::Iter iter(&fUniquelyKeyedProxies);
    for (UniquelyKeyedProxyHash::Iter iter(&fUniquelyKeyedProxies); !iter.done(); ++iter) {
        GrTextureProxy& tmp = *iter;

        this->processInvalidUniqueKey(tmp.getUniqueKey(), &tmp, InvalidateGPUResource::kNo);
    }
    SkASSERT(!fUniquelyKeyedProxies.count());
}

bool GrProxyProvider::renderingDirectly() const {
    return fImageContext->priv().asDirectContext();
}
