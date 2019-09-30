/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrMtlTexture.h"

#include "GrMtlGpu.h"
#include "GrMtlUtil.h"
#include "GrTexturePriv.h"

GrMtlTexture::GrMtlTexture(GrMtlGpu* gpu,
                           SkBudgeted budgeted,
                           const GrSurfaceDesc& desc,
                           id<MTLTexture> texture,
                           GrMipMapsStatus mipMapsStatus)
        : GrSurface(gpu, desc)
        , INHERITED(gpu, desc, GrTextureType::k2D, mipMapsStatus)
        , fTexture(texture) {
    SkASSERT((GrMipMapsStatus::kNotAllocated == mipMapsStatus) == (1 == texture.mipmapLevelCount));
    this->registerWithCache(budgeted);
}

<<<<<<< HEAD
GrMtlTexture::GrMtlTexture(GrMtlGpu* gpu,
                           Wrapped,
                           const GrSurfaceDesc& desc,
                           id<MTLTexture> texture,
                           GrMipMapsStatus mipMapsStatus)
        : GrSurface(gpu, desc)
        , INHERITED(gpu, desc, GrTextureType::k2D, mipMapsStatus)
        , fTexture(texture) {
    SkASSERT((GrMipMapsStatus::kNotAllocated == mipMapsStatus) == (1 == texture.mipmapLevelCount));
    this->registerWithCacheWrapped();
||||||| merged common ancestors
// This method parallels GrTextureProxy::highestFilterMode
static inline GrSamplerState::Filter highest_filter_mode(GrPixelConfig config) {
    return GrSamplerState::Filter::kMipMap;
=======
GrMtlTexture::GrMtlTexture(GrMtlGpu* gpu,
                           Wrapped,
                           const GrSurfaceDesc& desc,
                           id<MTLTexture> texture,
                           GrMipMapsStatus mipMapsStatus,
                           GrWrapCacheable cacheable,
                           GrIOType ioType)
        : GrSurface(gpu, desc)
        , INHERITED(gpu, desc, GrTextureType::k2D, mipMapsStatus)
        , fTexture(texture) {
    SkASSERT((GrMipMapsStatus::kNotAllocated == mipMapsStatus) == (1 == texture.mipmapLevelCount));
    if (ioType == kRead_GrIOType) {
        this->setReadOnly();
    }
    this->registerWithCacheWrapped(cacheable);
>>>>>>> upstream-releases
}

GrMtlTexture::GrMtlTexture(GrMtlGpu* gpu,
                           const GrSurfaceDesc& desc,
                           id<MTLTexture> texture,
                           GrMipMapsStatus mipMapsStatus)
        : GrSurface(gpu, desc)
        , INHERITED(gpu, desc, GrTextureType::k2D, mipMapsStatus)
        , fTexture(texture) {
<<<<<<< HEAD
    SkASSERT((GrMipMapsStatus::kNotAllocated == mipMapsStatus) == (1 == texture.mipmapLevelCount));
}

sk_sp<GrMtlTexture> GrMtlTexture::CreateNewTexture(GrMtlGpu* gpu, SkBudgeted budgeted,
                                                   const GrSurfaceDesc& desc,
                                                   MTLTextureDescriptor* texDesc,
                                                   GrMipMapsStatus mipMapsStatus) {
    if (desc.fSampleCnt > 1) {
        SkASSERT(false); // Currently we don't support msaa
        return nullptr;
    }
    id<MTLTexture> texture = [gpu->device() newTextureWithDescriptor:texDesc];
    SkASSERT(nil != texture);
    SkASSERT(MTLTextureUsageShaderRead & texture.usage);
    return sk_sp<GrMtlTexture>(new GrMtlTexture(gpu, budgeted, desc, texture, mipMapsStatus));
}

sk_sp<GrMtlTexture> GrMtlTexture::MakeWrappedTexture(GrMtlGpu* gpu,
                                                     const GrSurfaceDesc& desc,
                                                     id<MTLTexture> texture) {
    if (desc.fSampleCnt > 1) {
        SkASSERT(false); // Currently we don't support msaa
        return nullptr;
    }
    SkASSERT(nil != texture);
    SkASSERT(MTLTextureUsageShaderRead & texture.usage);
    GrMipMapsStatus mipMapsStatus = texture.mipmapLevelCount > 1 ? GrMipMapsStatus::kValid
                                                                 : GrMipMapsStatus::kNotAllocated;
    return sk_sp<GrMtlTexture>(new GrMtlTexture(gpu, kWrapped, desc, texture, mipMapsStatus));
||||||| merged common ancestors
=======
    SkASSERT((GrMipMapsStatus::kNotAllocated == mipMapsStatus) == (1 == texture.mipmapLevelCount));
}

sk_sp<GrMtlTexture> GrMtlTexture::CreateNewTexture(GrMtlGpu* gpu, SkBudgeted budgeted,
                                                   const GrSurfaceDesc& desc,
                                                   MTLTextureDescriptor* texDesc,
                                                   GrMipMapsStatus mipMapsStatus) {
    if (desc.fSampleCnt > 1) {
        SkASSERT(false); // Currently we don't support msaa
        return nullptr;
    }
    id<MTLTexture> texture = [gpu->device() newTextureWithDescriptor:texDesc];
    SkASSERT(nil != texture);
    SkASSERT(MTLTextureUsageShaderRead & texture.usage);
    return sk_sp<GrMtlTexture>(new GrMtlTexture(gpu, budgeted, desc, texture, mipMapsStatus));
}

sk_sp<GrMtlTexture> GrMtlTexture::MakeWrappedTexture(GrMtlGpu* gpu,
                                                     const GrSurfaceDesc& desc,
                                                     id<MTLTexture> texture,
                                                     GrWrapCacheable cacheable,
                                                     GrIOType ioType) {
    if (desc.fSampleCnt > 1) {
        SkASSERT(false); // Currently we don't support msaa
        return nullptr;
    }
    SkASSERT(nil != texture);
    SkASSERT(MTLTextureUsageShaderRead & texture.usage);
    GrMipMapsStatus mipMapsStatus = texture.mipmapLevelCount > 1 ? GrMipMapsStatus::kValid
                                                                 : GrMipMapsStatus::kNotAllocated;
    return sk_sp<GrMtlTexture>(new GrMtlTexture(gpu, kWrapped, desc, texture, mipMapsStatus,
                                                cacheable, ioType));
>>>>>>> upstream-releases
}

GrMtlTexture::~GrMtlTexture() {
    SkASSERT(nil == fTexture);
}

GrMtlGpu* GrMtlTexture::getMtlGpu() const {
    SkASSERT(!this->wasDestroyed());
    return static_cast<GrMtlGpu*>(this->getGpu());
}

<<<<<<< HEAD
GrBackendTexture GrMtlTexture::getBackendTexture() const {
    GrMipMapped mipMapped = fTexture.mipmapLevelCount > 1 ? GrMipMapped::kYes
                                                          : GrMipMapped::kNo;
    GrMtlTextureInfo info;
    info.fTexture = GrGetPtrFromId(fTexture);
    return GrBackendTexture(this->width(), this->height(), mipMapped, info);
||||||| merged common ancestors
GrBackendObject GrMtlTexture::getTextureHandle() const {
    void* voidTex = (__bridge_retained void*)fTexture;
    return (GrBackendObject)voidTex;
}

GrBackendTexture GrMtlTexture::getBackendTexture() const {
    return GrBackendTexture(); // invalid
=======
GrBackendTexture GrMtlTexture::getBackendTexture() const {
    GrMipMapped mipMapped = fTexture.mipmapLevelCount > 1 ? GrMipMapped::kYes
                                                          : GrMipMapped::kNo;
    GrMtlTextureInfo info;
    info.fTexture = GrGetPtrFromId(fTexture);
    return GrBackendTexture(this->width(), this->height(), mipMapped, info);
}

GrBackendFormat GrMtlTexture::backendFormat() const {
    return GrBackendFormat::MakeMtl(fTexture.pixelFormat);
>>>>>>> upstream-releases
}

