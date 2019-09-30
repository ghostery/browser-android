/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrVkTexture_DEFINED
#define GrVkTexture_DEFINED

#include "GrTexture.h"
#include "GrVkImage.h"
#include "vk/GrVkTypes.h"

class GrVkGpu;
class GrVkImageView;
struct GrVkImageInfo;

class GrVkTexture : public GrTexture, public virtual GrVkImage {
public:
    static sk_sp<GrVkTexture> MakeNewTexture(GrVkGpu*,
                                             SkBudgeted budgeted,
                                             const GrSurfaceDesc&,
                                             const GrVkImage::ImageDesc&,
                                             GrMipMapsStatus);

<<<<<<< HEAD
    static sk_sp<GrVkTexture> MakeWrappedTexture(GrVkGpu*, const GrSurfaceDesc&,
                                                 GrWrapOwnership, const GrVkImageInfo&,
                                                 sk_sp<GrVkImageLayout>);
||||||| merged common ancestors
    static sk_sp<GrVkTexture> MakeWrappedTexture(GrVkGpu*, const GrSurfaceDesc&,
                                                 GrWrapOwnership, const GrVkImageInfo*);
=======
    static sk_sp<GrVkTexture> MakeWrappedTexture(GrVkGpu*, const GrSurfaceDesc&, GrWrapOwnership,
                                                 GrWrapCacheable, GrIOType, const GrVkImageInfo&,
                                                 sk_sp<GrVkImageLayout>);
>>>>>>> upstream-releases

    ~GrVkTexture() override;

    GrBackendTexture getBackendTexture() const override;

    GrBackendFormat backendFormat() const override { return this->getBackendFormat(); }

    void textureParamsModified() override {}

<<<<<<< HEAD
    const GrVkImageView* textureView();

    // In Vulkan we call the release proc after we are finished with the underlying
    // GrVkImage::Resource object (which occurs after the GPU has finsihed all work on it).
    void setRelease(sk_sp<GrReleaseProcHelper> releaseHelper) override {
        // Forward the release proc on to GrVkImage
        this->setResourceRelease(std::move(releaseHelper));
    }
||||||| merged common ancestors
    const GrVkImageView* textureView(bool allowSRGB);

    bool reallocForMipmap(GrVkGpu* gpu, uint32_t mipLevels);

    // In Vulkan we call the release proc after we are finished with the underlying
    // GrVkImage::Resource object (which occurs after the GPU has finsihed all work on it).
    void setRelease(sk_sp<GrReleaseProcHelper> releaseHelper) override {
        // Forward the release proc on to GrVkImage
        this->setResourceRelease(std::move(releaseHelper));
    }
=======
    const GrVkImageView* textureView();

    void addIdleProc(sk_sp<GrRefCntedCallback>, IdleState) override;
    void callIdleProcsOnBehalfOfResource();
>>>>>>> upstream-releases

protected:
    GrVkTexture(GrVkGpu*, const GrSurfaceDesc&, const GrVkImageInfo&, sk_sp<GrVkImageLayout>,
                const GrVkImageView*, GrMipMapsStatus, GrBackendObjectOwnership);

    GrVkGpu* getVkGpu() const;

    void onAbandon() override;
    void onRelease() override;

    bool onStealBackendTexture(GrBackendTexture*, SkImage::BackendTextureReleaseProc*) override {
        return false;
    }

    void willRemoveLastRefOrPendingIO() override;

private:
<<<<<<< HEAD
    enum Wrapped { kWrapped };
    GrVkTexture(GrVkGpu*, SkBudgeted, const GrSurfaceDesc&, const GrVkImageInfo&,
                sk_sp<GrVkImageLayout> layout, const GrVkImageView* imageView,
||||||| merged common ancestors
    enum Wrapped { kWrapped };
    GrVkTexture(GrVkGpu*, SkBudgeted, const GrSurfaceDesc&,
                const GrVkImageInfo&, const GrVkImageView* imageView,
=======
    GrVkTexture(GrVkGpu*, SkBudgeted, const GrSurfaceDesc&, const GrVkImageInfo&,
                sk_sp<GrVkImageLayout> layout, const GrVkImageView* imageView,
>>>>>>> upstream-releases
                GrMipMapsStatus);
<<<<<<< HEAD
    GrVkTexture(GrVkGpu*, Wrapped, const GrSurfaceDesc&, const GrVkImageInfo&,
                sk_sp<GrVkImageLayout> layout, const GrVkImageView* imageView, GrMipMapsStatus,
                GrBackendObjectOwnership);
||||||| merged common ancestors
    GrVkTexture(GrVkGpu*, Wrapped, const GrSurfaceDesc&,
                const GrVkImageInfo&, const GrVkImageView* imageView, GrMipMapsStatus,
                GrBackendObjectOwnership);
=======
    GrVkTexture(GrVkGpu*, const GrSurfaceDesc&, const GrVkImageInfo&, sk_sp<GrVkImageLayout>,
                const GrVkImageView*, GrMipMapsStatus, GrBackendObjectOwnership, GrWrapCacheable,
                GrIOType);

    // In Vulkan we call the release proc after we are finished with the underlying
    // GrVkImage::Resource object (which occurs after the GPU has finished all work on it).
    void onSetRelease(sk_sp<GrRefCntedCallback> releaseHelper) override {
        // Forward the release proc on to GrVkImage
        this->setResourceRelease(std::move(releaseHelper));
    }

    void removeFinishIdleProcs();
>>>>>>> upstream-releases

<<<<<<< HEAD
    const GrVkImageView*     fTextureView;
||||||| merged common ancestors
    const GrVkImageView*     fTextureView;
    const GrVkImageView*     fLinearTextureView;
=======
    const GrVkImageView* fTextureView;
>>>>>>> upstream-releases

    typedef GrTexture INHERITED;
};

#endif
