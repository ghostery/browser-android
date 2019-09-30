/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrVkTexture.h"

#include "GrTexturePriv.h"
#include "GrVkGpu.h"
#include "GrVkImageView.h"
#include "GrVkTextureRenderTarget.h"
#include "GrVkUtil.h"

#include "vk/GrVkTypes.h"

#define VK_CALL(GPU, X) GR_VK_CALL(GPU->vkInterface(), X)

// Because this class is virtually derived from GrSurface we must explicitly call its constructor.
GrVkTexture::GrVkTexture(GrVkGpu* gpu,
                         SkBudgeted budgeted,
                         const GrSurfaceDesc& desc,
                         const GrVkImageInfo& info,
                         sk_sp<GrVkImageLayout> layout,
                         const GrVkImageView* view,
                         GrMipMapsStatus mipMapsStatus)
        : GrSurface(gpu, desc)
        , GrVkImage(info, std::move(layout), GrBackendObjectOwnership::kOwned)
        , INHERITED(gpu, desc, GrTextureType::k2D, mipMapsStatus)
        , fTextureView(view) {
    SkASSERT((GrMipMapsStatus::kNotAllocated == mipMapsStatus) == (1 == info.fLevelCount));
    this->registerWithCache(budgeted);
    if (GrPixelConfigIsCompressed(desc.fConfig)) {
        this->setReadOnly();
    }
}

<<<<<<< HEAD
GrVkTexture::GrVkTexture(GrVkGpu* gpu,
                         Wrapped,
                         const GrSurfaceDesc& desc,
                         const GrVkImageInfo& info,
                         sk_sp<GrVkImageLayout> layout,
                         const GrVkImageView* view,
                         GrMipMapsStatus mipMapsStatus,
                         GrBackendObjectOwnership ownership)
        : GrSurface(gpu, desc)
        , GrVkImage(info, std::move(layout), ownership)
        , INHERITED(gpu, desc, GrTextureType::k2D, mipMapsStatus)
        , fTextureView(view) {
||||||| merged common ancestors
GrVkTexture::GrVkTexture(GrVkGpu* gpu,
                         Wrapped,
                         const GrSurfaceDesc& desc,
                         const GrVkImageInfo& info,
                         const GrVkImageView* view,
                         GrMipMapsStatus mipMapsStatus,
                         GrBackendObjectOwnership ownership)
    : GrSurface(gpu, desc)
    , GrVkImage(info, ownership)
    , INHERITED(gpu, desc, kTexture2DSampler_GrSLType, highest_filter_mode(desc.fConfig),
                mipMapsStatus)
    , fTextureView(view)
    , fLinearTextureView(nullptr) {
=======
GrVkTexture::GrVkTexture(GrVkGpu* gpu, const GrSurfaceDesc& desc, const GrVkImageInfo& info,
                         sk_sp<GrVkImageLayout> layout, const GrVkImageView* view,
                         GrMipMapsStatus mipMapsStatus, GrBackendObjectOwnership ownership,
                         GrWrapCacheable cacheable, GrIOType ioType)
        : GrSurface(gpu, desc)
        , GrVkImage(info, std::move(layout), ownership)
        , INHERITED(gpu, desc, GrTextureType::k2D, mipMapsStatus)
        , fTextureView(view) {
>>>>>>> upstream-releases
    SkASSERT((GrMipMapsStatus::kNotAllocated == mipMapsStatus) == (1 == info.fLevelCount));
    if (ioType == kRead_GrIOType) {
        this->setReadOnly();
    }
    this->registerWithCacheWrapped(cacheable);
}

// Because this class is virtually derived from GrSurface we must explicitly call its constructor.
GrVkTexture::GrVkTexture(GrVkGpu* gpu,
                         const GrSurfaceDesc& desc,
                         const GrVkImageInfo& info,
                         sk_sp<GrVkImageLayout> layout,
                         const GrVkImageView* view,
                         GrMipMapsStatus mipMapsStatus,
                         GrBackendObjectOwnership ownership)
        : GrSurface(gpu, desc)
        , GrVkImage(info, layout, ownership)
        , INHERITED(gpu, desc, GrTextureType::k2D, mipMapsStatus)
        , fTextureView(view) {
    SkASSERT((GrMipMapsStatus::kNotAllocated == mipMapsStatus) == (1 == info.fLevelCount));
}

sk_sp<GrVkTexture> GrVkTexture::MakeNewTexture(GrVkGpu* gpu, SkBudgeted budgeted,
                                               const GrSurfaceDesc& desc,
                                               const GrVkImage::ImageDesc& imageDesc,
                                               GrMipMapsStatus mipMapsStatus) {
    SkASSERT(imageDesc.fUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT);

    GrVkImageInfo info;
    if (!GrVkImage::InitImageInfo(gpu, imageDesc, &info)) {
        return nullptr;
    }

    const GrVkImageView* imageView = GrVkImageView::Create(
            gpu, info.fImage, info.fFormat, GrVkImageView::kColor_Type, info.fLevelCount,
            info.fYcbcrConversionInfo);
    if (!imageView) {
        GrVkImage::DestroyImageInfo(gpu, &info);
        return nullptr;
    }
    sk_sp<GrVkImageLayout> layout(new GrVkImageLayout(info.fImageLayout));

    return sk_sp<GrVkTexture>(new GrVkTexture(gpu, budgeted, desc, info, std::move(layout),
                                              imageView, mipMapsStatus));
}

sk_sp<GrVkTexture> GrVkTexture::MakeWrappedTexture(GrVkGpu* gpu, const GrSurfaceDesc& desc,
                                                   GrWrapOwnership wrapOwnership,
<<<<<<< HEAD
                                                   const GrVkImageInfo& info,
                                                   sk_sp<GrVkImageLayout> layout) {
||||||| merged common ancestors
                                                   const GrVkImageInfo* info) {
    SkASSERT(info);
=======
                                                   GrWrapCacheable cacheable, GrIOType ioType,
                                                   const GrVkImageInfo& info,
                                                   sk_sp<GrVkImageLayout> layout) {
>>>>>>> upstream-releases
    // Wrapped textures require both image and allocation (because they can be mapped)
    SkASSERT(VK_NULL_HANDLE != info.fImage && VK_NULL_HANDLE != info.fAlloc.fMemory);

<<<<<<< HEAD
    const GrVkImageView* imageView = GrVkImageView::Create(gpu, info.fImage, info.fFormat,
                                                           GrVkImageView::kColor_Type,
                                                           info.fLevelCount);
||||||| merged common ancestors
    const GrVkImageView* imageView = GrVkImageView::Create(gpu, info->fImage, info->fFormat,
                                                           GrVkImageView::kColor_Type,
                                                           info->fLevelCount);
=======
    const GrVkImageView* imageView = GrVkImageView::Create(
            gpu, info.fImage, info.fFormat, GrVkImageView::kColor_Type, info.fLevelCount,
            info.fYcbcrConversionInfo);
>>>>>>> upstream-releases
    if (!imageView) {
        return nullptr;
    }

    GrMipMapsStatus mipMapsStatus = info.fLevelCount > 1 ? GrMipMapsStatus::kValid
                                                         : GrMipMapsStatus::kNotAllocated;

    GrBackendObjectOwnership ownership = kBorrow_GrWrapOwnership == wrapOwnership
            ? GrBackendObjectOwnership::kBorrowed : GrBackendObjectOwnership::kOwned;
<<<<<<< HEAD
    return sk_sp<GrVkTexture>(new GrVkTexture(gpu, kWrapped, desc, info, std::move(layout),
                                              imageView, mipMapsStatus, ownership));
||||||| merged common ancestors
    return sk_sp<GrVkTexture>(new GrVkTexture(gpu, kWrapped, desc, *info, imageView,
                                              mipMapsStatus, ownership));
=======
    return sk_sp<GrVkTexture>(new GrVkTexture(gpu, desc, info, std::move(layout), imageView,
                                              mipMapsStatus, ownership, cacheable, ioType));
>>>>>>> upstream-releases
}

GrVkTexture::~GrVkTexture() {
    // either release or abandon should have been called by the owner of this object.
    SkASSERT(!fTextureView);
}

void GrVkTexture::onRelease() {
    // We're about to be severed from our GrVkResource. If there are "finish" idle procs we have to
    // decide who will handle them. If the resource is still tied to a command buffer we let it
    // handle them. Otherwise, we handle them.
    if (this->hasResource() && this->resource()->isOwnedByCommandBuffer()) {
        this->removeFinishIdleProcs();
    }

    // we create this and don't hand it off, so we should always destroy it
    if (fTextureView) {
        fTextureView->unref(this->getVkGpu());
        fTextureView = nullptr;
    }

    this->releaseImage(this->getVkGpu());

    INHERITED::onRelease();
}

void GrVkTexture::onAbandon() {
    // We're about to be severed from our GrVkResource. If there are "finish" idle procs we have to
    // decide who will handle them. If the resource is still tied to a command buffer we let it
    // handle them. Otherwise, we handle them.
    if (this->hasResource() && this->resource()->isOwnedByCommandBuffer()) {
        this->removeFinishIdleProcs();
    }

    // we create this and don't hand it off, so we should always destroy it
    if (fTextureView) {
        fTextureView->unrefAndAbandon();
        fTextureView = nullptr;
    }

    this->abandonImage();
    INHERITED::onAbandon();
}

GrBackendTexture GrVkTexture::getBackendTexture() const {
    return GrBackendTexture(this->width(), this->height(), fInfo, this->grVkImageLayout());
}

GrVkGpu* GrVkTexture::getVkGpu() const {
    SkASSERT(!this->wasDestroyed());
    return static_cast<GrVkGpu*>(this->getGpu());
}

const GrVkImageView* GrVkTexture::textureView() {
    return fTextureView;
}

<<<<<<< HEAD
||||||| merged common ancestors
bool GrVkTexture::reallocForMipmap(GrVkGpu* gpu, uint32_t mipLevels) {
    if (mipLevels == 1) {
        // don't need to do anything for a 1x1 texture
        return false;
    }

    const GrVkResource* oldResource = this->resource();

    // We shouldn't realloc something that doesn't belong to us
    if (fIsBorrowed) {
        return false;
    }

    bool renderTarget = SkToBool(this->asRenderTarget());

    VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_SAMPLED_BIT;
    if (renderTarget) {
        usageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
    usageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    GrVkImage::ImageDesc imageDesc;
    imageDesc.fImageType = VK_IMAGE_TYPE_2D;
    imageDesc.fFormat = fInfo.fFormat;
    imageDesc.fWidth = this->width();
    imageDesc.fHeight = this->height();
    imageDesc.fLevels = mipLevels;
    imageDesc.fSamples = 1;
    imageDesc.fImageTiling = VK_IMAGE_TILING_OPTIMAL;
    imageDesc.fUsageFlags = usageFlags;
    imageDesc.fMemProps = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    GrVkImageInfo info;
    if (!GrVkImage::InitImageInfo(gpu, imageDesc, &info)) {
        return false;
    }

    // have to create a new image view for new resource
    const GrVkImageView* oldView = fTextureView;
    VkImage image = info.fImage;
    const GrVkImageView* textureView = GrVkImageView::Create(gpu, image, info.fFormat,
                                                             GrVkImageView::kColor_Type, mipLevels);
    if (!textureView) {
        GrVkImage::DestroyImageInfo(gpu, &info);
        return false;
    }

    if (renderTarget) {
        GrVkTextureRenderTarget* texRT = static_cast<GrVkTextureRenderTarget*>(this);
        if (!texRT->updateForMipmap(gpu, info)) {
            GrVkImage::DestroyImageInfo(gpu, &info);
            return false;
        }
    }

    oldResource->unref(gpu);
    oldView->unref(gpu);
    if (fLinearTextureView) {
        fLinearTextureView->unref(gpu);
        fLinearTextureView = nullptr;
    }

    this->setNewResource(info.fImage, info.fAlloc, info.fImageTiling);
    fTextureView = textureView;
    fInfo = info;
    // SetMaxMipMapLevel stores the max level not the number of levels
    this->texturePriv().setMaxMipMapLevel(mipLevels-1);

    return true;
}
=======
void GrVkTexture::addIdleProc(sk_sp<GrRefCntedCallback> idleProc, IdleState type) {
    INHERITED::addIdleProc(idleProc, type);
    if (type == IdleState::kFinished) {
        if (auto* resource = this->resource()) {
            resource->addIdleProc(this, std::move(idleProc));
        }
    }
}

void GrVkTexture::callIdleProcsOnBehalfOfResource() {
    // If we got here then the resource is being removed from its last command buffer and the
    // texture is idle in the cache. Any kFlush idle procs should already have been called. So
    // the texture and resource should have the same set of procs.
    SkASSERT(this->resource());
    SkASSERT(this->resource()->idleProcCnt() == fIdleProcs.count());
#ifdef SK_DEBUG
    for (int i = 0; i < fIdleProcs.count(); ++i) {
        SkASSERT(fIdleProcs[i] == this->resource()->idleProc(i));
    }
#endif
    fIdleProcs.reset();
    this->resource()->resetIdleProcs();
}

void GrVkTexture::willRemoveLastRefOrPendingIO() {
    if (!fIdleProcs.count()) {
        return;
    }
    // This is called when the GrTexture is purgeable. However, we need to check whether the
    // Resource is still owned by any command buffers. If it is then it will call the proc.
    auto* resource = this->hasResource() ? this->resource() : nullptr;
    bool callFinishProcs = !resource || !resource->isOwnedByCommandBuffer();
    if (callFinishProcs) {
        // Everything must go!
        fIdleProcs.reset();
        resource->resetIdleProcs();
    } else {
        // The procs that should be called on flush but not finish are those that are owned
        // by the GrVkTexture and not the Resource. We do this by copying the resource's array
        // and thereby dropping refs to procs we own but the resource does not.
        SkASSERT(resource);
        fIdleProcs.reset(resource->idleProcCnt());
        for (int i = 0; i < fIdleProcs.count(); ++i) {
            fIdleProcs[i] = resource->idleProc(i);
        }
    }
}

void GrVkTexture::removeFinishIdleProcs() {
    // This should only be called by onRelease/onAbandon when we have already checked for a
    // resource.
    const auto* resource = this->resource();
    SkASSERT(resource);
    SkSTArray<4, sk_sp<GrRefCntedCallback>> procsToKeep;
    int resourceIdx = 0;
    // The idle procs that are common between the GrVkTexture and its Resource should be found in
    // the same order.
    for (int i = 0; i < fIdleProcs.count(); ++i) {
        if (fIdleProcs[i] == resource->idleProc(resourceIdx)) {
            ++resourceIdx;
        } else {
            procsToKeep.push_back(fIdleProcs[i]);
        }
    }
    SkASSERT(resourceIdx == resource->idleProcCnt());
    fIdleProcs = procsToKeep;
}
>>>>>>> upstream-releases
