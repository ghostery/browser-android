/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrMtlRenderTarget.h"

#include "GrMtlGpu.h"
#include "GrMtlUtil.h"

<<<<<<< HEAD
GrMtlRenderTarget::GrMtlRenderTarget(GrMtlGpu* gpu,
                                     SkBudgeted budgeted,
                                     const GrSurfaceDesc& desc,
                                     id<MTLTexture> renderTexture)
        : GrSurface(gpu, desc)
        , GrRenderTarget(gpu, desc)
        , fRenderTexture(renderTexture)
        , fResolveTexture(nil) {
    SkASSERT(1 == desc.fSampleCnt);
    this->registerWithCache(budgeted);
||||||| merged common ancestors
sk_sp<GrMtlRenderTarget> GrMtlRenderTarget::CreateNewRenderTarget(GrMtlGpu* gpu,
                                                                  const GrSurfaceDesc& desc,
                                                                  SkBudgeted budgeted) {
    MTLPixelFormat format;
    if (!GrPixelConfigToMTLFormat(desc.fConfig, &format)) {
        return nullptr;
    }

    if (desc.fSampleCnt) {
        return nullptr;
    }

    MTLTextureDescriptor* descriptor = [[MTLTextureDescriptor alloc] init];
    descriptor.textureType = MTLTextureType2D;
    descriptor.pixelFormat = format;
    descriptor.width = desc.fWidth;
    descriptor.height = desc.fHeight;
    descriptor.depth = 1;
    descriptor.mipmapLevelCount = 1;
    descriptor.sampleCount = 1;
    descriptor.arrayLength = 1;
    // descriptor.resourceOptions This looks to be set by setting cpuCacheMode and storageModes
    descriptor.cpuCacheMode = MTLCPUCacheModeWriteCombined;
    // RenderTargets never need to be mapped so their storage mode is set to private
    descriptor.storageMode = MTLStorageModePrivate;

    descriptor.usage = MTLTextureUsageRenderTarget;

    id<MTLTexture> texture = [gpu->device() newTextureWithDescriptor:descriptor];

    return sk_sp<GrMtlRenderTarget>(new GrMtlRenderTarget(gpu, desc, budgeted, texture));
=======
// Called for wrapped non-texture render targets.
GrMtlRenderTarget::GrMtlRenderTarget(GrMtlGpu* gpu,
                                     const GrSurfaceDesc& desc,
                                     id<MTLTexture> renderTexture,
                                     Wrapped)
        : GrSurface(gpu, desc)
        , GrRenderTarget(gpu, desc)
        , fRenderTexture(renderTexture)
        , fResolveTexture(nil) {
    SkASSERT(1 == desc.fSampleCnt);
    this->registerWithCacheWrapped(GrWrapCacheable::kNo);
>>>>>>> upstream-releases
}

// Called by subclass constructors.
GrMtlRenderTarget::GrMtlRenderTarget(GrMtlGpu* gpu,
                                     const GrSurfaceDesc& desc,
                                     id<MTLTexture> renderTexture)
        : GrSurface(gpu, desc)
        , GrRenderTarget(gpu, desc)
        , fRenderTexture(renderTexture)
        , fResolveTexture(nil) {
<<<<<<< HEAD
    SkASSERT(1 == desc.fSampleCnt);
}

sk_sp<GrMtlRenderTarget>
GrMtlRenderTarget::MakeWrappedRenderTarget(GrMtlGpu* gpu, const GrSurfaceDesc& desc,
                                           id<MTLTexture> renderTexture) {
    SkASSERT(nil != renderTexture);
    SkASSERT(1 == renderTexture.mipmapLevelCount);
    SkASSERT(MTLTextureUsageRenderTarget & renderTexture.usage);
    return sk_sp<GrMtlRenderTarget>(new GrMtlRenderTarget(gpu, SkBudgeted::kNo,
                                                          desc, renderTexture));
||||||| merged common ancestors
=======
    SkASSERT(1 == desc.fSampleCnt);
}

sk_sp<GrMtlRenderTarget>
GrMtlRenderTarget::MakeWrappedRenderTarget(GrMtlGpu* gpu, const GrSurfaceDesc& desc,
                                           id<MTLTexture> renderTexture) {
    SkASSERT(nil != renderTexture);
    SkASSERT(1 == renderTexture.mipmapLevelCount);
    SkASSERT(MTLTextureUsageRenderTarget & renderTexture.usage);
    return sk_sp<GrMtlRenderTarget>(new GrMtlRenderTarget(gpu, desc, renderTexture, kWrapped));
>>>>>>> upstream-releases
}

GrMtlRenderTarget::~GrMtlRenderTarget() {
    SkASSERT(nil == fRenderTexture);
    SkASSERT(nil == fResolveTexture);
}

<<<<<<< HEAD

GrBackendRenderTarget GrMtlRenderTarget::getBackendRenderTarget() const {
    GrMtlTextureInfo info;
    info.fTexture = GrGetPtrFromId(fRenderTexture);
    return GrBackendRenderTarget(this->width(), this->height(), fRenderTexture.sampleCount, info);
}

||||||| merged common ancestors
=======
GrBackendRenderTarget GrMtlRenderTarget::getBackendRenderTarget() const {
    GrMtlTextureInfo info;
    info.fTexture = GrGetPtrFromId(fRenderTexture);
    return GrBackendRenderTarget(this->width(), this->height(), fRenderTexture.sampleCount, info);
}

GrBackendFormat GrMtlRenderTarget::backendFormat() const {
    return GrBackendFormat::MakeMtl(fRenderTexture.pixelFormat);
}

>>>>>>> upstream-releases
GrMtlGpu* GrMtlRenderTarget::getMtlGpu() const {
    SkASSERT(!this->wasDestroyed());
    return static_cast<GrMtlGpu*>(this->getGpu());
}

void GrMtlRenderTarget::onAbandon() {
    fRenderTexture = nil;
    fResolveTexture = nil;
    INHERITED::onAbandon();
}

void GrMtlRenderTarget::onRelease() {
    fRenderTexture = nil;
    fResolveTexture = nil;
    INHERITED::onRelease();
}

bool GrMtlRenderTarget::completeStencilAttachment() {
    return true;
}

