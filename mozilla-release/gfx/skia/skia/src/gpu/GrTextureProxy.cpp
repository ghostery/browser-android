/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrTextureProxy.h"
#include "GrTextureProxyPriv.h"

#include "GrContext.h"
#include "GrContextPriv.h"
#include "GrDeferredProxyUploader.h"
#include "GrProxyProvider.h"
#include "GrSurfacePriv.h"
#include "GrTexturePriv.h"

<<<<<<< HEAD
// Deferred version - with data
GrTextureProxy::GrTextureProxy(const GrSurfaceDesc& srcDesc, GrMipMapped mipMapped,
                               GrTextureType textureType, SkBackingFit fit, SkBudgeted budgeted,
                               const void* srcData, size_t /*rowBytes*/,
                               GrInternalSurfaceFlags surfaceFlags)
        : INHERITED(srcDesc, kTopLeft_GrSurfaceOrigin, fit, budgeted, surfaceFlags)
||||||| merged common ancestors
// Deferred version
GrTextureProxy::GrTextureProxy(const GrSurfaceDesc& srcDesc, GrMipMapped mipMapped,
                               SkBackingFit fit, SkBudgeted budgeted, const void* srcData,
                               size_t /*rowBytes*/, uint32_t flags)
        : INHERITED(srcDesc, fit, budgeted, flags)
=======
// Deferred version - with data
GrTextureProxy::GrTextureProxy(const GrBackendFormat& format, const GrSurfaceDesc& srcDesc,
                               GrMipMapped mipMapped, SkBackingFit fit, SkBudgeted budgeted,
                               const void* srcData, size_t /*rowBytes*/,
                               GrInternalSurfaceFlags surfaceFlags)
        : INHERITED(format, srcDesc, kTopLeft_GrSurfaceOrigin, fit, budgeted, surfaceFlags)
>>>>>>> upstream-releases
        , fMipMapped(mipMapped)
        , fTextureType(textureType)
        , fProxyProvider(nullptr)
        , fDeferredUploader(nullptr) {
    SkASSERT(!srcData);  // currently handled in Make()
}

<<<<<<< HEAD
// Deferred version - no data
GrTextureProxy::GrTextureProxy(const GrSurfaceDesc& srcDesc, GrSurfaceOrigin origin,
                               GrMipMapped mipMapped, GrTextureType textureType, SkBackingFit fit,
                               SkBudgeted budgeted, GrInternalSurfaceFlags surfaceFlags)
        : INHERITED(srcDesc, origin, fit, budgeted, surfaceFlags)
        , fMipMapped(mipMapped)
        , fTextureType(textureType)
        , fProxyProvider(nullptr)
        , fDeferredUploader(nullptr) {}

||||||| merged common ancestors
=======
// Deferred version - no data
GrTextureProxy::GrTextureProxy(const GrBackendFormat& format, const GrSurfaceDesc& srcDesc,
                               GrSurfaceOrigin origin, GrMipMapped mipMapped,
                               SkBackingFit fit, SkBudgeted budgeted,
                               GrInternalSurfaceFlags surfaceFlags)
        : INHERITED(format, srcDesc, origin, fit, budgeted, surfaceFlags)
        , fMipMapped(mipMapped)
        , fProxyProvider(nullptr)
        , fDeferredUploader(nullptr) {}

>>>>>>> upstream-releases
// Lazy-callback version
GrTextureProxy::GrTextureProxy(LazyInstantiateCallback&& callback, LazyInstantiationType lazyType,
<<<<<<< HEAD
                               const GrSurfaceDesc& desc, GrSurfaceOrigin origin,
                               GrMipMapped mipMapped, GrTextureType textureType, SkBackingFit fit,
                               SkBudgeted budgeted, GrInternalSurfaceFlags surfaceFlags)
        : INHERITED(std::move(callback), lazyType, desc, origin, fit, budgeted, surfaceFlags)
||||||| merged common ancestors
                               const GrSurfaceDesc& desc, GrMipMapped mipMapped, SkBackingFit fit,
                               SkBudgeted budgeted, uint32_t flags)
        : INHERITED(std::move(callback), lazyType, desc, fit, budgeted, flags)
=======
                               const GrBackendFormat& format, const GrSurfaceDesc& desc,
                               GrSurfaceOrigin origin, GrMipMapped mipMapped, SkBackingFit fit,
                               SkBudgeted budgeted, GrInternalSurfaceFlags surfaceFlags)
        : INHERITED(std::move(callback), lazyType, format, desc, origin, fit, budgeted,
                    surfaceFlags)
>>>>>>> upstream-releases
        , fMipMapped(mipMapped)
        , fTextureType(textureType)
        , fProxyProvider(nullptr)
        , fDeferredUploader(nullptr) {}

// Wrapped version
GrTextureProxy::GrTextureProxy(sk_sp<GrSurface> surf, GrSurfaceOrigin origin)
        : INHERITED(std::move(surf), origin, SkBackingFit::kExact)
        , fMipMapped(fTarget->asTexture()->texturePriv().mipMapped())
        , fTextureType(fTarget->asTexture()->texturePriv().textureType())
        , fProxyProvider(nullptr)
        , fDeferredUploader(nullptr) {
    if (fTarget->getUniqueKey().isValid()) {
        fProxyProvider = fTarget->asTexture()->getContext()->priv().proxyProvider();
        fProxyProvider->adoptUniqueKeyFromSurface(this, fTarget);
    }
}

GrTextureProxy::~GrTextureProxy() {
    // Due to the order of cleanup the GrSurface this proxy may have wrapped may have gone away
    // at this point. Zero out the pointer so the cache invalidation code doesn't try to use it.
    fTarget = nullptr;
<<<<<<< HEAD

    // In DDL-mode, uniquely keyed proxies keep their key even after their originating
    // proxy provider has gone away. In that case there is noone to send the invalid key
    // message to (Note: in this case we don't want to remove its cached resource).
    if (fUniqueKey.isValid() && fProxyProvider) {
        fProxyProvider->processInvalidProxyUniqueKey(fUniqueKey, this, false);
||||||| merged common ancestors
    if (fUniqueKey.isValid()) {
        fProxyProvider->processInvalidProxyUniqueKey(fUniqueKey, this, false);
=======

    // In DDL-mode, uniquely keyed proxies keep their key even after their originating
    // proxy provider has gone away. In that case there is noone to send the invalid key
    // message to (Note: in this case we don't want to remove its cached resource).
    if (fUniqueKey.isValid() && fProxyProvider) {
        fProxyProvider->processInvalidUniqueKey(fUniqueKey, this,
                                                GrProxyProvider::InvalidateGPUResource::kNo);
>>>>>>> upstream-releases
    } else {
        SkASSERT(!fProxyProvider);
    }
}

bool GrTextureProxy::instantiate(GrResourceProvider* resourceProvider) {
    if (LazyState::kNot != this->lazyInstantiationState()) {
        return false;
    }
    if (!this->instantiateImpl(resourceProvider, 1, /* needsStencil = */ false,
                               kNone_GrSurfaceFlags, fMipMapped,
                               fUniqueKey.isValid() ? &fUniqueKey : nullptr)) {
        return false;
    }

    SkASSERT(!fTarget->asRenderTarget());
    SkASSERT(fTarget->asTexture());
    return true;
}

sk_sp<GrSurface> GrTextureProxy::createSurface(GrResourceProvider* resourceProvider) const {
    sk_sp<GrSurface> surface= this->createSurfaceImpl(resourceProvider, 1,
                                                      /* needsStencil = */ false,
                                                      kNone_GrSurfaceFlags,
                                                      fMipMapped);
    if (!surface) {
        return nullptr;
    }

    SkASSERT(!surface->asRenderTarget());
    SkASSERT(surface->asTexture());
    return surface;
}

void GrTextureProxyPriv::setDeferredUploader(std::unique_ptr<GrDeferredProxyUploader> uploader) {
    SkASSERT(!fTextureProxy->fDeferredUploader);
    fTextureProxy->fDeferredUploader = std::move(uploader);
}

void GrTextureProxyPriv::scheduleUpload(GrOpFlushState* flushState) {
    // The texture proxy's contents may already have been uploaded or instantiation may have failed
    if (fTextureProxy->fDeferredUploader && fTextureProxy->fTarget) {
        fTextureProxy->fDeferredUploader->scheduleUpload(flushState, fTextureProxy);
    }
}

void GrTextureProxyPriv::resetDeferredUploader() {
    SkASSERT(fTextureProxy->fDeferredUploader);
    fTextureProxy->fDeferredUploader.reset();
}

GrSamplerState::Filter GrTextureProxy::highestFilterMode() const {
    return this->hasRestrictedSampling() ? GrSamplerState::Filter::kBilerp
                                         : GrSamplerState::Filter::kMipMap;
}

GrMipMapped GrTextureProxy::mipMapped() const {
    if (this->isInstantiated()) {
        return this->peekTexture()->texturePriv().mipMapped();
    }
    return fMipMapped;
}

size_t GrTextureProxy::onUninstantiatedGpuMemorySize() const {
    return GrSurface::ComputeSize(this->config(), this->width(), this->height(), 1,
<<<<<<< HEAD
                                  this->proxyMipMapped(), !this->priv().isExact());
||||||| merged common ancestors
                                  this->texPriv().proxyMipMapped(), !this->priv().isExact());
=======
                                  this->proxyMipMapped(), !this->priv().isExact());
}

bool GrTextureProxy::ProxiesAreCompatibleAsDynamicState(const GrTextureProxy* first,
                                                        const GrTextureProxy* second) {
    return first->config() == second->config() &&
           first->textureType() == second->textureType() &&
           first->backendFormat() == second->backendFormat();
>>>>>>> upstream-releases
}

void GrTextureProxy::setUniqueKey(GrProxyProvider* proxyProvider, const GrUniqueKey& key) {
    SkASSERT(key.isValid());
    SkASSERT(!fUniqueKey.isValid()); // proxies can only ever get one uniqueKey

    if (fTarget) {
        if (!fTarget->getUniqueKey().isValid()) {
            fTarget->resourcePriv().setUniqueKey(key);
        }
        SkASSERT(fTarget->getUniqueKey() == key);
    }

    fUniqueKey = key;
    fProxyProvider = proxyProvider;
}

void GrTextureProxy::clearUniqueKey() {
    fUniqueKey.reset();
    fProxyProvider = nullptr;
}

#ifdef SK_DEBUG
void GrTextureProxy::onValidateSurface(const GrSurface* surface) {
    SkASSERT(!surface->asRenderTarget());

    // Anything that is checked here should be duplicated in GrTextureRenderTargetProxy's version
    SkASSERT(surface->asTexture());
    SkASSERT(GrMipMapped::kNo == this->proxyMipMapped() ||
             GrMipMapped::kYes == surface->asTexture()->texturePriv().mipMapped());
<<<<<<< HEAD
    SkASSERT(surface->asTexture()->texturePriv().textureType() == fTextureType);
||||||| merged common ancestors
=======

    SkASSERT(surface->asTexture()->texturePriv().textureType() == this->textureType());

    GrInternalSurfaceFlags proxyFlags = fSurfaceFlags;
    GrInternalSurfaceFlags surfaceFlags = surface->surfacePriv().flags();
    SkASSERT((proxyFlags & GrInternalSurfaceFlags::kTextureMask) ==
             (surfaceFlags & GrInternalSurfaceFlags::kTextureMask));
>>>>>>> upstream-releases
}

#endif

