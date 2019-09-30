/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrTextureRenderTargetProxy.h"

#include "GrCaps.h"
#include "GrTexture.h"
#include "GrTexturePriv.h"
#include "GrTextureProxyPriv.h"
#include "GrRenderTarget.h"
#include "GrSurfacePriv.h"
#include "GrSurfaceProxyPriv.h"

// Deferred version
// This class is virtually derived from GrSurfaceProxy (via both GrTextureProxy and
// GrRenderTargetProxy) so its constructor must be explicitly called.
GrTextureRenderTargetProxy::GrTextureRenderTargetProxy(const GrCaps& caps,
                                                       const GrBackendFormat& format,
                                                       const GrSurfaceDesc& desc,
                                                       GrSurfaceOrigin origin,
                                                       GrMipMapped mipMapped,
                                                       GrTextureType textureType,
                                                       SkBackingFit fit,
                                                       SkBudgeted budgeted,
<<<<<<< HEAD
                                                       GrInternalSurfaceFlags surfaceFlags)
        : GrSurfaceProxy(desc, origin, fit, budgeted, surfaceFlags)
||||||| merged common ancestors
                                                       uint32_t flags)
        : GrSurfaceProxy(desc, fit, budgeted, flags)
=======
                                                       GrInternalSurfaceFlags surfaceFlags)
        : GrSurfaceProxy(format, desc, origin, fit, budgeted, surfaceFlags)
>>>>>>> upstream-releases
        // for now textures w/ data are always wrapped
<<<<<<< HEAD
        , GrRenderTargetProxy(caps, desc, origin, fit, budgeted, surfaceFlags)
        , GrTextureProxy(desc, origin, mipMapped, textureType, fit, budgeted, surfaceFlags) {}
||||||| merged common ancestors
        , GrTextureProxy(desc, mipMapped, fit, budgeted, nullptr, 0, flags)
        , GrRenderTargetProxy(caps, desc, fit, budgeted, flags) {
}
=======
        , GrRenderTargetProxy(caps, format, desc, origin, fit, budgeted, surfaceFlags)
        , GrTextureProxy(format, desc, origin, mipMapped, fit, budgeted, surfaceFlags) {}
>>>>>>> upstream-releases

// Lazy-callback version
GrTextureRenderTargetProxy::GrTextureRenderTargetProxy(LazyInstantiateCallback&& callback,
                                                       LazyInstantiationType lazyType,
                                                       const GrBackendFormat& format,
                                                       const GrSurfaceDesc& desc,
                                                       GrSurfaceOrigin origin,
                                                       GrMipMapped mipMapped,
                                                       GrTextureType textureType,
                                                       SkBackingFit fit,
                                                       SkBudgeted budgeted,
<<<<<<< HEAD
                                                       GrInternalSurfaceFlags surfaceFlags)
        : GrSurfaceProxy(std::move(callback), lazyType, desc, origin, fit, budgeted, surfaceFlags)
||||||| merged common ancestors
                                                       uint32_t flags,
                                                       GrRenderTargetFlags renderTargetFlags)
        : GrSurfaceProxy(std::move(callback), lazyType, desc, fit, budgeted, flags)
=======
                                                       GrInternalSurfaceFlags surfaceFlags)
        : GrSurfaceProxy(std::move(callback), lazyType, format, desc, origin, fit, budgeted,
                         surfaceFlags)
>>>>>>> upstream-releases
        // Since we have virtual inheritance, we initialize GrSurfaceProxy directly. Send null
        // callbacks to the texture and RT proxies simply to route to the appropriate constructors.
<<<<<<< HEAD
        , GrRenderTargetProxy(LazyInstantiateCallback(), lazyType, desc, origin, fit, budgeted,
                              surfaceFlags)
        , GrTextureProxy(LazyInstantiateCallback(), lazyType, desc, origin, mipMapped, textureType,
                         fit, budgeted, surfaceFlags) {}
||||||| merged common ancestors
        , GrTextureProxy(LazyInstantiateCallback(), lazyType, desc, mipMapped, fit, budgeted, flags)
        , GrRenderTargetProxy(LazyInstantiateCallback(), lazyType, desc, fit, budgeted, flags,
                              renderTargetFlags) {
}
=======
        , GrRenderTargetProxy(LazyInstantiateCallback(), lazyType, format, desc, origin, fit,
                              budgeted, surfaceFlags, WrapsVkSecondaryCB::kNo)
        , GrTextureProxy(LazyInstantiateCallback(), lazyType, format, desc, origin, mipMapped,
                         fit, budgeted, surfaceFlags) {}
>>>>>>> upstream-releases

// Wrapped version
// This class is virtually derived from GrSurfaceProxy (via both GrTextureProxy and
// GrRenderTargetProxy) so its constructor must be explicitly called.
GrTextureRenderTargetProxy::GrTextureRenderTargetProxy(sk_sp<GrSurface> surf,
                                                       GrSurfaceOrigin origin)
        : GrSurfaceProxy(surf, origin, SkBackingFit::kExact)
        , GrRenderTargetProxy(surf, origin)
        , GrTextureProxy(surf, origin) {
    SkASSERT(surf->asTexture());
    SkASSERT(surf->asRenderTarget());
}

size_t GrTextureRenderTargetProxy::onUninstantiatedGpuMemorySize() const {
    int colorSamplesPerPixel = this->numColorSamples();
    if (colorSamplesPerPixel > 1) {
        // Add one to account for the resolve buffer.
        ++colorSamplesPerPixel;
    }

    // TODO: do we have enough information to improve this worst case estimate?
    return GrSurface::ComputeSize(this->config(), this->width(), this->height(),
                                  colorSamplesPerPixel, this->proxyMipMapped(),
                                  !this->priv().isExact());
}

bool GrTextureRenderTargetProxy::instantiate(GrResourceProvider* resourceProvider) {
    if (LazyState::kNot != this->lazyInstantiationState()) {
        return false;
    }
    static constexpr GrSurfaceDescFlags kDescFlags = kRenderTarget_GrSurfaceFlag;

    const GrUniqueKey& key = this->getUniqueKey();

    if (!this->instantiateImpl(resourceProvider, this->numStencilSamples(), this->needsStencil(),
                               kDescFlags, this->mipMapped(), key.isValid() ? &key : nullptr)) {
        return false;
    }
    if (key.isValid()) {
        SkASSERT(key == this->getUniqueKey());
    }

    SkASSERT(fTarget->asRenderTarget());
    SkASSERT(fTarget->asTexture());

    return true;
}

sk_sp<GrSurface> GrTextureRenderTargetProxy::createSurface(
                                                    GrResourceProvider* resourceProvider) const {
    static constexpr GrSurfaceDescFlags kDescFlags = kRenderTarget_GrSurfaceFlag;

    sk_sp<GrSurface> surface = this->createSurfaceImpl(resourceProvider, this->numStencilSamples(),
                                                       this->needsStencil(), kDescFlags,
                                                       this->mipMapped());
    if (!surface) {
        return nullptr;
    }
    SkASSERT(surface->asRenderTarget());
    SkASSERT(surface->asTexture());

    return surface;
}

#ifdef SK_DEBUG
void GrTextureRenderTargetProxy::onValidateSurface(const GrSurface* surface) {
    // Anything checked here should also be checking the GrTextureProxy version
    SkASSERT(surface->asTexture());
    SkASSERT(GrMipMapped::kNo == this->proxyMipMapped() ||
             GrMipMapped::kYes == surface->asTexture()->texturePriv().mipMapped());

    // Anything checked here should also be checking the GrRenderTargetProxy version
    SkASSERT(surface->asRenderTarget());
    SkASSERT(surface->asRenderTarget()->numStencilSamples() == this->numStencilSamples());
<<<<<<< HEAD

    GrInternalSurfaceFlags proxyFlags = fSurfaceFlags;
    GrInternalSurfaceFlags surfaceFlags = surface->surfacePriv().flags();
    SkASSERT((proxyFlags & GrInternalSurfaceFlags::kRenderTargetMask) ==
             (surfaceFlags & GrInternalSurfaceFlags::kRenderTargetMask));
    SkASSERT(surface->asTexture()->texturePriv().textureType() == this->textureType());
||||||| merged common ancestors
=======

    SkASSERT(surface->asTexture()->texturePriv().textureType() == this->textureType());

    GrInternalSurfaceFlags proxyFlags = fSurfaceFlags;
    GrInternalSurfaceFlags surfaceFlags = surface->surfacePriv().flags();

    // Only non-RT textures can be read only.
    SkASSERT(!(proxyFlags & GrInternalSurfaceFlags::kReadOnly));
    SkASSERT(!(surfaceFlags & GrInternalSurfaceFlags::kReadOnly));

    SkASSERT((proxyFlags & GrInternalSurfaceFlags::kRenderTargetMask) ==
             (surfaceFlags & GrInternalSurfaceFlags::kRenderTargetMask));
    SkASSERT((proxyFlags & GrInternalSurfaceFlags::kTextureMask) ==
             (surfaceFlags & GrInternalSurfaceFlags::kTextureMask));
>>>>>>> upstream-releases
}
#endif

