/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrVkTextureRenderTarget.h"

#include "GrTexturePriv.h"
#include "GrVkGpu.h"
#include "GrVkImageView.h"
#include "GrVkUtil.h"

#include "SkMipMap.h"

#include "vk/GrVkTypes.h"

#define VK_CALL(GPU, X) GR_VK_CALL(GPU->vkInterface(), X)

GrVkTextureRenderTarget::GrVkTextureRenderTarget(GrVkGpu* gpu,
                                                 SkBudgeted budgeted,
                                                 const GrSurfaceDesc& desc,
                                                 const GrVkImageInfo& info,
                                                 sk_sp<GrVkImageLayout> layout,
                                                 const GrVkImageView* texView,
                                                 const GrVkImageInfo& msaaInfo,
                                                 sk_sp<GrVkImageLayout> msaaLayout,
                                                 const GrVkImageView* colorAttachmentView,
                                                 const GrVkImageView* resolveAttachmentView,
                                                 GrMipMapsStatus mipMapsStatus)
        : GrSurface(gpu, desc)
<<<<<<< HEAD
        , GrVkImage(info, layout, ownership)
        , GrVkTexture(gpu, desc, info, layout, texView, mipMapsStatus, ownership)
        , GrVkRenderTarget(gpu, desc, info, layout, msaaInfo, std::move(msaaLayout),
                           colorAttachmentView, resolveAttachmentView,
                           GrBackendObjectOwnership::kOwned) {
||||||| merged common ancestors
        , GrVkImage(info, ownership)
        , GrVkTexture(gpu, desc, info, texView, mipMapsStatus, ownership)
        , GrVkRenderTarget(gpu, desc, info, msaaInfo, colorAttachmentView,
                           resolveAttachmentView, GrBackendObjectOwnership::kOwned) {
=======
        , GrVkImage(info, layout, GrBackendObjectOwnership::kOwned)
        , GrVkTexture(gpu, desc, info, layout, texView, mipMapsStatus,
                      GrBackendObjectOwnership::kOwned)
        , GrVkRenderTarget(gpu, desc, info, layout, msaaInfo, std::move(msaaLayout),
                           colorAttachmentView, resolveAttachmentView,
                           GrBackendObjectOwnership::kOwned) {
>>>>>>> upstream-releases
    this->registerWithCache(budgeted);
}

GrVkTextureRenderTarget::GrVkTextureRenderTarget(GrVkGpu* gpu,
                                                 SkBudgeted budgeted,
                                                 const GrSurfaceDesc& desc,
                                                 const GrVkImageInfo& info,
                                                 sk_sp<GrVkImageLayout> layout,
                                                 const GrVkImageView* texView,
                                                 const GrVkImageView* colorAttachmentView,
                                                 GrMipMapsStatus mipMapsStatus)
        : GrSurface(gpu, desc)
<<<<<<< HEAD
        , GrVkImage(info, layout, ownership)
        , GrVkTexture(gpu, desc, info, layout, texView, mipMapsStatus, ownership)
        , GrVkRenderTarget(gpu, desc, info, layout, colorAttachmentView,
                           GrBackendObjectOwnership::kOwned) {
||||||| merged common ancestors
        , GrVkImage(info, ownership)
        , GrVkTexture(gpu, desc, info, texView, mipMapsStatus, ownership)
        , GrVkRenderTarget(gpu, desc, info, colorAttachmentView, GrBackendObjectOwnership::kOwned) {
=======
        , GrVkImage(info, layout, GrBackendObjectOwnership::kOwned)
        , GrVkTexture(gpu, desc, info, layout, texView, mipMapsStatus,
                      GrBackendObjectOwnership::kOwned)
        , GrVkRenderTarget(gpu, desc, info, layout, colorAttachmentView,
                           GrBackendObjectOwnership::kOwned) {
>>>>>>> upstream-releases
    this->registerWithCache(budgeted);
}

GrVkTextureRenderTarget::GrVkTextureRenderTarget(GrVkGpu* gpu,
                                                 const GrSurfaceDesc& desc,
                                                 const GrVkImageInfo& info,
                                                 sk_sp<GrVkImageLayout> layout,
                                                 const GrVkImageView* texView,
                                                 const GrVkImageInfo& msaaInfo,
                                                 sk_sp<GrVkImageLayout> msaaLayout,
                                                 const GrVkImageView* colorAttachmentView,
                                                 const GrVkImageView* resolveAttachmentView,
                                                 GrMipMapsStatus mipMapsStatus,
                                                 GrBackendObjectOwnership ownership,
                                                 GrWrapCacheable cacheable)
        : GrSurface(gpu, desc)
<<<<<<< HEAD
        , GrVkImage(info, layout, ownership)
        , GrVkTexture(gpu, desc, info, layout, texView, mipMapsStatus, ownership)
        , GrVkRenderTarget(gpu, desc, info, layout, msaaInfo, std::move(msaaLayout),
                           colorAttachmentView, resolveAttachmentView, ownership) {
    this->registerWithCacheWrapped();
||||||| merged common ancestors
        , GrVkImage(info, ownership)
        , GrVkTexture(gpu, desc, info, texView, mipMapsStatus, ownership)
        , GrVkRenderTarget(gpu, desc, info, msaaInfo, colorAttachmentView,
                           resolveAttachmentView, ownership) {
    this->registerWithCacheWrapped();
=======
        , GrVkImage(info, layout, ownership)
        , GrVkTexture(gpu, desc, info, layout, texView, mipMapsStatus, ownership)
        , GrVkRenderTarget(gpu, desc, info, layout, msaaInfo, std::move(msaaLayout),
                           colorAttachmentView, resolveAttachmentView, ownership) {
    this->registerWithCacheWrapped(cacheable);
>>>>>>> upstream-releases
}

GrVkTextureRenderTarget::GrVkTextureRenderTarget(GrVkGpu* gpu,
                                                 const GrSurfaceDesc& desc,
                                                 const GrVkImageInfo& info,
                                                 sk_sp<GrVkImageLayout> layout,
                                                 const GrVkImageView* texView,
                                                 const GrVkImageView* colorAttachmentView,
                                                 GrMipMapsStatus mipMapsStatus,
                                                 GrBackendObjectOwnership ownership,
                                                 GrWrapCacheable cacheable)
        : GrSurface(gpu, desc)
<<<<<<< HEAD
        , GrVkImage(info, layout, ownership)
        , GrVkTexture(gpu, desc, info, layout, texView, mipMapsStatus, ownership)
        , GrVkRenderTarget(gpu, desc, info, layout, colorAttachmentView, ownership) {
    this->registerWithCacheWrapped();
||||||| merged common ancestors
        , GrVkImage(info, ownership)
        , GrVkTexture(gpu, desc, info, texView, mipMapsStatus, ownership)
        , GrVkRenderTarget(gpu, desc, info, colorAttachmentView, ownership) {
    this->registerWithCacheWrapped();
=======
        , GrVkImage(info, layout, ownership)
        , GrVkTexture(gpu, desc, info, layout, texView, mipMapsStatus, ownership)
        , GrVkRenderTarget(gpu, desc, info, layout, colorAttachmentView, ownership) {
    this->registerWithCacheWrapped(cacheable);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
sk_sp<GrVkTextureRenderTarget> GrVkTextureRenderTarget::Make(GrVkGpu* gpu,
                                                             const GrSurfaceDesc& desc,
                                                             const GrVkImageInfo& info,
                                                             sk_sp<GrVkImageLayout> layout,
                                                             GrMipMapsStatus mipMapsStatus,
                                                             SkBudgeted budgeted,
                                                             GrBackendObjectOwnership ownership,
                                                             bool isWrapped) {
||||||| merged common ancestors
sk_sp<GrVkTextureRenderTarget> GrVkTextureRenderTarget::Make(GrVkGpu* gpu,
                                                             const GrSurfaceDesc& desc,
                                                             const GrVkImageInfo& info,
                                                             GrMipMapsStatus mipMapsStatus,
                                                             SkBudgeted budgeted,
                                                             GrBackendObjectOwnership ownership,
                                                             bool isWrapped) {
=======
namespace {
struct Views {
    const GrVkImageView* imageView = nullptr;
    const GrVkImageView* colorAttachmentView = nullptr;
    const GrVkImageView* resolveAttachmentView = nullptr;
    GrVkImageInfo msInfo;
    sk_sp<GrVkImageLayout> msLayout;
};
}  // anonymous namespace

static Views create_views(GrVkGpu* gpu, const GrSurfaceDesc& desc, const GrVkImageInfo& info) {
>>>>>>> upstream-releases
    VkImage image = info.fImage;
    // Create the texture ImageView
    Views views;
    views.imageView = GrVkImageView::Create(gpu, image, info.fFormat, GrVkImageView::kColor_Type,
                                            info.fLevelCount, info.fYcbcrConversionInfo);
    if (!views.imageView) {
        return {};
    }

    VkFormat pixelFormat;
    GrPixelConfigToVkFormat(desc.fConfig, &pixelFormat);

    VkImage colorImage;

    // create msaa surface if necessary
<<<<<<< HEAD
    GrVkImageInfo msInfo;
    sk_sp<GrVkImageLayout> msLayout;
    const GrVkImageView* resolveAttachmentView = nullptr;
||||||| merged common ancestors
    GrVkImageInfo msInfo;
    const GrVkImageView* resolveAttachmentView = nullptr;
=======
>>>>>>> upstream-releases
    if (desc.fSampleCnt > 1) {
        GrVkImage::ImageDesc msImageDesc;
        msImageDesc.fImageType = VK_IMAGE_TYPE_2D;
        msImageDesc.fFormat = pixelFormat;
        msImageDesc.fWidth = desc.fWidth;
        msImageDesc.fHeight = desc.fHeight;
        msImageDesc.fLevels = 1;
        msImageDesc.fSamples = desc.fSampleCnt;
        msImageDesc.fImageTiling = VK_IMAGE_TILING_OPTIMAL;
        msImageDesc.fUsageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                  VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                  VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        msImageDesc.fMemProps = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        if (!GrVkImage::InitImageInfo(gpu, msImageDesc, &views.msInfo)) {
            views.imageView->unref(gpu);
            return {};
        }

        // Set color attachment image
        colorImage = views.msInfo.fImage;

        // Create resolve attachment view.
        views.resolveAttachmentView =
                GrVkImageView::Create(gpu, image, pixelFormat, GrVkImageView::kColor_Type,
                                      info.fLevelCount, GrVkYcbcrConversionInfo());
        if (!views.resolveAttachmentView) {
            GrVkImage::DestroyImageInfo(gpu, &views.msInfo);
            views.imageView->unref(gpu);
            return {};
        }
<<<<<<< HEAD
        msLayout.reset(new GrVkImageLayout(msInfo.fImageLayout));
||||||| merged common ancestors
=======
        views.msLayout.reset(new GrVkImageLayout(views.msInfo.fImageLayout));
>>>>>>> upstream-releases
    } else {
        // Set color attachment image
        colorImage = info.fImage;
    }

    views.colorAttachmentView = GrVkImageView::Create(
            gpu, colorImage, pixelFormat, GrVkImageView::kColor_Type, 1, GrVkYcbcrConversionInfo());
    if (!views.colorAttachmentView) {
        if (desc.fSampleCnt > 1) {
<<<<<<< HEAD
            resolveAttachmentView->unref(gpu);
            GrVkImage::DestroyImageInfo(gpu, &msInfo);
        }
        imageView->unref(gpu);
        return nullptr;
    }

    sk_sp<GrVkTextureRenderTarget> texRT;
    if (desc.fSampleCnt > 1) {
        if (!isWrapped) {
            texRT = sk_sp<GrVkTextureRenderTarget>(new GrVkTextureRenderTarget(
                                                      gpu, budgeted, desc,
                                                      info, std::move(layout), imageView, msInfo,
                                                      std::move(msLayout), colorAttachmentView,
                                                      resolveAttachmentView, mipMapsStatus,
                                                      ownership));
        } else {
            texRT = sk_sp<GrVkTextureRenderTarget>(new GrVkTextureRenderTarget(
                                                        gpu, desc,
                                                        info, std::move(layout), imageView, msInfo,
                                                        std::move(msLayout), colorAttachmentView,
                                                        resolveAttachmentView, mipMapsStatus,
                                                        ownership));
        }
    } else {
        if (!isWrapped) {
            texRT = sk_sp<GrVkTextureRenderTarget>(new GrVkTextureRenderTarget(
                                                        gpu, budgeted, desc,
                                                        info, std::move(layout), imageView,
                                                        colorAttachmentView, mipMapsStatus,
                                                        ownership));
        } else {
            texRT = sk_sp<GrVkTextureRenderTarget>(new GrVkTextureRenderTarget(
                                                        gpu, desc,
                                                        info, std::move(layout), imageView,
                                                        colorAttachmentView, mipMapsStatus,
                                                        ownership));
||||||| merged common ancestors
            resolveAttachmentView->unref(gpu);
            GrVkImage::DestroyImageInfo(gpu, &msInfo);
        }
        imageView->unref(gpu);
        return nullptr;
    }

    sk_sp<GrVkTextureRenderTarget> texRT;
    if (desc.fSampleCnt > 1) {
        if (!isWrapped) {
            texRT = sk_sp<GrVkTextureRenderTarget>(new GrVkTextureRenderTarget(
                                                      gpu, budgeted, desc,
                                                      info, imageView, msInfo,
                                                      colorAttachmentView,
                                                      resolveAttachmentView, mipMapsStatus,
                                                      ownership));
        } else {
            texRT = sk_sp<GrVkTextureRenderTarget>(new GrVkTextureRenderTarget(
                                                        gpu, desc,
                                                        info, imageView, msInfo,
                                                        colorAttachmentView,
                                                        resolveAttachmentView, mipMapsStatus,
                                                        ownership));
        }
    } else {
        if (!isWrapped) {
            texRT = sk_sp<GrVkTextureRenderTarget>(new GrVkTextureRenderTarget(
                                                        gpu, budgeted, desc,
                                                        info, imageView,
                                                        colorAttachmentView, mipMapsStatus,
                                                        ownership));
        } else {
            texRT = sk_sp<GrVkTextureRenderTarget>(new GrVkTextureRenderTarget(
                                                        gpu, desc,
                                                        info, imageView,
                                                        colorAttachmentView, mipMapsStatus,
                                                        ownership));
=======
            views.resolveAttachmentView->unref(gpu);
            GrVkImage::DestroyImageInfo(gpu, &views.msInfo);
>>>>>>> upstream-releases
        }
        views.imageView->unref(gpu);
        return {};
    }
    return views;
}

sk_sp<GrVkTextureRenderTarget>
GrVkTextureRenderTarget::MakeNewTextureRenderTarget(GrVkGpu* gpu,
                                                    SkBudgeted budgeted,
                                                    const GrSurfaceDesc& desc,
                                                    const GrVkImage::ImageDesc& imageDesc,
                                                    GrMipMapsStatus mipMapsStatus) {
    SkASSERT(imageDesc.fUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    SkASSERT(imageDesc.fUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT);

    GrVkImageInfo info;
    if (!GrVkImage::InitImageInfo(gpu, imageDesc, &info)) {
        return nullptr;
    }
    sk_sp<GrVkImageLayout> layout(new GrVkImageLayout(info.fImageLayout));

<<<<<<< HEAD
    sk_sp<GrVkTextureRenderTarget> trt = Make(gpu, desc, info, std::move(layout), mipMapsStatus,
                                              budgeted, GrBackendObjectOwnership::kOwned, false);
    if (!trt) {
||||||| merged common ancestors
    sk_sp<GrVkTextureRenderTarget> trt = Make(gpu, desc, info, mipMapsStatus, budgeted,
                                              GrBackendObjectOwnership::kOwned, false);
    if (!trt) {
=======
    Views views = create_views(gpu, desc, info);
    if (!views.colorAttachmentView) {
>>>>>>> upstream-releases
        GrVkImage::DestroyImageInfo(gpu, &info);
        return nullptr;
    }
    if (desc.fSampleCnt > 1) {
        return sk_sp<GrVkTextureRenderTarget>(new GrVkTextureRenderTarget(
                gpu, budgeted, desc, info, std::move(layout), views.imageView, views.msInfo,
                std::move(views.msLayout), views.colorAttachmentView, views.resolveAttachmentView,
                mipMapsStatus));
    } else {
        return sk_sp<GrVkTextureRenderTarget>(new GrVkTextureRenderTarget(
                gpu, budgeted, desc, info, std::move(layout), views.imageView,
                views.colorAttachmentView, mipMapsStatus));
    }
}

<<<<<<< HEAD
sk_sp<GrVkTextureRenderTarget>
GrVkTextureRenderTarget::MakeWrappedTextureRenderTarget(GrVkGpu* gpu,
                                                        const GrSurfaceDesc& desc,
                                                        GrWrapOwnership wrapOwnership,
                                                        const GrVkImageInfo& info,
                                                        sk_sp<GrVkImageLayout> layout) {
||||||| merged common ancestors
sk_sp<GrVkTextureRenderTarget>
GrVkTextureRenderTarget::MakeWrappedTextureRenderTarget(GrVkGpu* gpu,
                                                        const GrSurfaceDesc& desc,
                                                        GrWrapOwnership wrapOwnership,
                                                        const GrVkImageInfo* info) {
    SkASSERT(info);
=======
sk_sp<GrVkTextureRenderTarget> GrVkTextureRenderTarget::MakeWrappedTextureRenderTarget(
        GrVkGpu* gpu,
        const GrSurfaceDesc& desc,
        GrWrapOwnership wrapOwnership,
        GrWrapCacheable cacheable,
        const GrVkImageInfo& info,
        sk_sp<GrVkImageLayout> layout) {
>>>>>>> upstream-releases
    // Wrapped textures require both image and allocation (because they can be mapped)
    SkASSERT(VK_NULL_HANDLE != info.fImage && VK_NULL_HANDLE != info.fAlloc.fMemory);

    GrMipMapsStatus mipMapsStatus = info.fLevelCount > 1 ? GrMipMapsStatus::kDirty
                                                         : GrMipMapsStatus::kNotAllocated;

    GrBackendObjectOwnership ownership = kBorrow_GrWrapOwnership == wrapOwnership
            ? GrBackendObjectOwnership::kBorrowed : GrBackendObjectOwnership::kOwned;
<<<<<<< HEAD

    return Make(gpu, desc, info, std::move(layout), mipMapsStatus, SkBudgeted::kNo, ownership,
                true);
}

bool GrVkTextureRenderTarget::updateForMipmap(GrVkGpu* gpu, const GrVkImageInfo& newInfo) {
    VkFormat pixelFormat;
    GrPixelConfigToVkFormat(this->config(), &pixelFormat);
    if (this->numStencilSamples() > 1) {
        const GrVkImageView* resolveAttachmentView =
                GrVkImageView::Create(gpu,
                                      newInfo.fImage,
                                      pixelFormat,
                                      GrVkImageView::kColor_Type,
                                      newInfo.fLevelCount);
        if (!resolveAttachmentView) {
            return false;
        }
        fResolveAttachmentView->unref(gpu);
        fResolveAttachmentView = resolveAttachmentView;
||||||| merged common ancestors

    return Make(gpu, desc, *info, mipMapsStatus, SkBudgeted::kNo, ownership, true);
}

bool GrVkTextureRenderTarget::updateForMipmap(GrVkGpu* gpu, const GrVkImageInfo& newInfo) {
    VkFormat pixelFormat;
    GrPixelConfigToVkFormat(this->config(), &pixelFormat);
    if (this->numStencilSamples() > 1) {
        const GrVkImageView* resolveAttachmentView =
                GrVkImageView::Create(gpu,
                                      newInfo.fImage,
                                      pixelFormat,
                                      GrVkImageView::kColor_Type,
                                      newInfo.fLevelCount);
        if (!resolveAttachmentView) {
            return false;
        }
        fResolveAttachmentView->unref(gpu);
        fResolveAttachmentView = resolveAttachmentView;
=======
    Views views = create_views(gpu, desc, info);
    if (!views.colorAttachmentView) {
        return nullptr;
    }
    if (desc.fSampleCnt > 1) {
        return sk_sp<GrVkTextureRenderTarget>(new GrVkTextureRenderTarget(
                gpu, desc, info, std::move(layout), views.imageView, views.msInfo,
                std::move(views.msLayout), views.colorAttachmentView, views.resolveAttachmentView,
                mipMapsStatus, ownership, cacheable));
>>>>>>> upstream-releases
    } else {
        return sk_sp<GrVkTextureRenderTarget>(new GrVkTextureRenderTarget(
                gpu, desc, info, std::move(layout), views.imageView, views.colorAttachmentView,
                mipMapsStatus, ownership, cacheable));
    }
}

size_t GrVkTextureRenderTarget::onGpuMemorySize() const {
    int numColorSamples = this->numColorSamples();
    if (numColorSamples > 1) {
        // Add one to account for the resolve VkImage.
        ++numColorSamples;
    }
    return GrSurface::ComputeSize(this->config(), this->width(), this->height(),
                                  numColorSamples,  // TODO: this still correct?
                                  this->texturePriv().mipMapped());
}
