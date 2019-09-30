/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrYUVProvider.h"
#include "GrClip.h"
<<<<<<< HEAD
#include "GrColorSpaceXform.h"
#include "GrContext.h"
#include "GrContextPriv.h"
||||||| merged common ancestors
#include "GrContext.h"
#include "GrContextPriv.h"
=======
#include "GrColorSpaceXform.h"
>>>>>>> upstream-releases
#include "GrProxyProvider.h"
#include "GrRecordingContext.h"
#include "GrRecordingContextPriv.h"
#include "GrRenderTargetContext.h"
#include "GrTextureProxy.h"
#include "SkAutoMalloc.h"
#include "SkCachedData.h"
#include "SkRefCnt.h"
#include "SkResourceCache.h"
#include "SkYUVPlanesCache.h"
#include "SkYUVAIndex.h"
#include "effects/GrYUVtoRGBEffect.h"

<<<<<<< HEAD
sk_sp<SkCachedData> GrYUVProvider::getPlanes(SkYUVSizeInfo* size,
                                             SkYUVColorSpace* colorSpace,
                                             const void* constPlanes[3]) {
||||||| merged common ancestors
sk_sp<SkCachedData> init_provider(GrYUVProvider* provider, SkYUVPlanesCache::Info* yuvInfo,
                                  void* planes[3]) {
=======
sk_sp<SkCachedData> GrYUVProvider::getPlanes(SkYUVASizeInfo* size,
                                             SkYUVAIndex yuvaIndices[SkYUVAIndex::kIndexCount],
                                             SkYUVColorSpace* colorSpace,
                                             const void* constPlanes[SkYUVASizeInfo::kMaxCount]) {
>>>>>>> upstream-releases
    sk_sp<SkCachedData> data;
<<<<<<< HEAD
    SkYUVPlanesCache::Info yuvInfo;
    data.reset(SkYUVPlanesCache::FindAndRef(this->onGetID(), &yuvInfo));

    void* planes[3];
||||||| merged common ancestors
    data.reset(SkYUVPlanesCache::FindAndRef(provider->onGetID(), yuvInfo));
=======
    SkYUVPlanesCache::Info yuvInfo;
    data.reset(SkYUVPlanesCache::FindAndRef(this->onGetID(), &yuvInfo));

    void* planes[SkYUVASizeInfo::kMaxCount];
>>>>>>> upstream-releases

    if (data.get()) {
<<<<<<< HEAD
        planes[0] = (void*)data->data();
        planes[1] = (uint8_t*)planes[0] + (yuvInfo.fSizeInfo.fWidthBytes[SkYUVSizeInfo::kY] *
                                           yuvInfo.fSizeInfo.fSizes[SkYUVSizeInfo::kY].fHeight);
        planes[2] = (uint8_t*)planes[1] + (yuvInfo.fSizeInfo.fWidthBytes[SkYUVSizeInfo::kU] *
                                           yuvInfo.fSizeInfo.fSizes[SkYUVSizeInfo::kU].fHeight);
||||||| merged common ancestors
        planes[0] = (void*)data->data();
        planes[1] = (uint8_t*)planes[0] + (yuvInfo->fSizeInfo.fWidthBytes[SkYUVSizeInfo::kY] *
                                           yuvInfo->fSizeInfo.fSizes[SkYUVSizeInfo::kY].fHeight);
        planes[2] = (uint8_t*)planes[1] + (yuvInfo->fSizeInfo.fWidthBytes[SkYUVSizeInfo::kU] *
                                           yuvInfo->fSizeInfo.fSizes[SkYUVSizeInfo::kU].fHeight);
=======
        planes[0] = (void*)data->data(); // we should always have at least one plane

        for (int i = 1; i < SkYUVASizeInfo::kMaxCount; ++i) {
            if (!yuvInfo.fSizeInfo.fWidthBytes[i]) {
                SkASSERT(!yuvInfo.fSizeInfo.fWidthBytes[i] &&
                         !yuvInfo.fSizeInfo.fSizes[i].fHeight);
                planes[i] = nullptr;
                continue;
            }

            planes[i] = (uint8_t*)planes[i-1] + (yuvInfo.fSizeInfo.fWidthBytes[i-1] *
                                                 yuvInfo.fSizeInfo.fSizes[i-1].fHeight);
        }
>>>>>>> upstream-releases
    } else {
        // Fetch yuv plane sizes for memory allocation.
<<<<<<< HEAD
        if (!this->onQueryYUV8(&yuvInfo.fSizeInfo, &yuvInfo.fColorSpace)) {
||||||| merged common ancestors
        if (!provider->onQueryYUV8(&yuvInfo->fSizeInfo, &yuvInfo->fColorSpace)) {
=======
        if (!this->onQueryYUVA8(&yuvInfo.fSizeInfo, yuvInfo.fYUVAIndices, &yuvInfo.fColorSpace)) {
>>>>>>> upstream-releases
            return nullptr;
        }

        // Allocate the memory for YUVA
        size_t totalSize(0);
<<<<<<< HEAD
        for (int i = 0; i < 3; i++) {
            totalSize += yuvInfo.fSizeInfo.fWidthBytes[i] * yuvInfo.fSizeInfo.fSizes[i].fHeight;
||||||| merged common ancestors
        for (int i = 0; i < 3; i++) {
            totalSize += yuvInfo->fSizeInfo.fWidthBytes[i] * yuvInfo->fSizeInfo.fSizes[i].fHeight;
=======
        for (int i = 0; i < SkYUVASizeInfo::kMaxCount; i++) {
            SkASSERT((yuvInfo.fSizeInfo.fWidthBytes[i] && yuvInfo.fSizeInfo.fSizes[i].fHeight) ||
                     (!yuvInfo.fSizeInfo.fWidthBytes[i] && !yuvInfo.fSizeInfo.fSizes[i].fHeight));

            totalSize += yuvInfo.fSizeInfo.fWidthBytes[i] * yuvInfo.fSizeInfo.fSizes[i].fHeight;
>>>>>>> upstream-releases
        }

        data.reset(SkResourceCache::NewCachedData(totalSize));

        planes[0] = data->writable_data();
<<<<<<< HEAD
        planes[1] = (uint8_t*)planes[0] + (yuvInfo.fSizeInfo.fWidthBytes[SkYUVSizeInfo::kY] *
                                           yuvInfo.fSizeInfo.fSizes[SkYUVSizeInfo::kY].fHeight);
        planes[2] = (uint8_t*)planes[1] + (yuvInfo.fSizeInfo.fWidthBytes[SkYUVSizeInfo::kU] *
                                           yuvInfo.fSizeInfo.fSizes[SkYUVSizeInfo::kU].fHeight);
||||||| merged common ancestors
        planes[1] = (uint8_t*)planes[0] + (yuvInfo->fSizeInfo.fWidthBytes[SkYUVSizeInfo::kY] *
                                           yuvInfo->fSizeInfo.fSizes[SkYUVSizeInfo::kY].fHeight);
        planes[2] = (uint8_t*)planes[1] + (yuvInfo->fSizeInfo.fWidthBytes[SkYUVSizeInfo::kU] *
                                           yuvInfo->fSizeInfo.fSizes[SkYUVSizeInfo::kU].fHeight);
=======

        for (int i = 1; i < SkYUVASizeInfo::kMaxCount; ++i) {
            if (!yuvInfo.fSizeInfo.fWidthBytes[i]) {
                SkASSERT(!yuvInfo.fSizeInfo.fWidthBytes[i] &&
                         !yuvInfo.fSizeInfo.fSizes[i].fHeight);
                planes[i] = nullptr;
                continue;
            }

            planes[i] = (uint8_t*)planes[i-1] + (yuvInfo.fSizeInfo.fWidthBytes[i-1] *
                                                 yuvInfo.fSizeInfo.fSizes[i-1].fHeight);
        }
>>>>>>> upstream-releases

        // Get the YUV planes.
<<<<<<< HEAD
        if (!this->onGetYUV8Planes(yuvInfo.fSizeInfo, planes)) {
||||||| merged common ancestors
        if (!provider->onGetYUV8Planes(yuvInfo->fSizeInfo, planes)) {
=======
        if (!this->onGetYUVA8Planes(yuvInfo.fSizeInfo, yuvInfo.fYUVAIndices, planes)) {
>>>>>>> upstream-releases
            return nullptr;
        }

        // Decoding is done, cache the resulting YUV planes
        SkYUVPlanesCache::Add(this->onGetID(), data.get(), &yuvInfo);
    }
<<<<<<< HEAD

    *size = yuvInfo.fSizeInfo;
    *colorSpace = yuvInfo.fColorSpace;
    constPlanes[0] = planes[0];
    constPlanes[1] = planes[1];
    constPlanes[2] = planes[2];
||||||| merged common ancestors
=======

    *size = yuvInfo.fSizeInfo;
    memcpy(yuvaIndices, yuvInfo.fYUVAIndices, sizeof(yuvInfo.fYUVAIndices));
    *colorSpace = yuvInfo.fColorSpace;
    constPlanes[0] = planes[0];
    constPlanes[1] = planes[1];
    constPlanes[2] = planes[2];
    constPlanes[3] = planes[3];
>>>>>>> upstream-releases
    return data;
}

void GrYUVProvider::YUVGen_DataReleaseProc(const void*, void* data) {
    SkCachedData* cachedData = static_cast<SkCachedData*>(data);
    SkASSERT(cachedData);
    cachedData->unref();
}

<<<<<<< HEAD
sk_sp<GrTextureProxy> GrYUVProvider::refAsTextureProxy(GrContext* ctx, const GrSurfaceDesc& desc,
                                                       SkColorSpace* srcColorSpace,
                                                       SkColorSpace* dstColorSpace) {
    SkYUVSizeInfo yuvSizeInfo;
    SkYUVColorSpace yuvColorSpace;
    const void* planes[3];

    sk_sp<SkCachedData> dataStorage = this->getPlanes(&yuvSizeInfo, &yuvColorSpace, planes);
||||||| merged common ancestors
sk_sp<GrTextureProxy> GrYUVProvider::refAsTextureProxy(GrContext* ctx, const GrSurfaceDesc& desc,
                                                       const SkColorSpace* srcColorSpace,
                                                       const SkColorSpace* dstColorSpace) {
    SkYUVPlanesCache::Info yuvInfo;
    void* planes[3];

    sk_sp<SkCachedData>  dataStorage = init_provider(this, &yuvInfo, planes);
=======
sk_sp<GrTextureProxy> GrYUVProvider::refAsTextureProxy(GrRecordingContext* ctx,
                                                       const GrBackendFormat& format,
                                                       const GrSurfaceDesc& desc,
                                                       SkColorSpace* srcColorSpace,
                                                       SkColorSpace* dstColorSpace) {
    SkYUVASizeInfo yuvSizeInfo;
    SkYUVAIndex yuvaIndices[SkYUVAIndex::kIndexCount];
    SkYUVColorSpace yuvColorSpace;
    const void* planes[SkYUVASizeInfo::kMaxCount];

    sk_sp<SkCachedData> dataStorage = this->getPlanes(&yuvSizeInfo, yuvaIndices,
                                                      &yuvColorSpace, planes);
>>>>>>> upstream-releases
    if (!dataStorage) {
        return nullptr;
    }

<<<<<<< HEAD
    sk_sp<GrTextureProxy> yuvTextureProxies[3];
    for (int i = 0; i < 3; i++) {
        int componentWidth  = yuvSizeInfo.fSizes[i].fWidth;
        int componentHeight = yuvSizeInfo.fSizes[i].fHeight;
||||||| merged common ancestors
    sk_sp<GrTextureProxy> yuvTextureProxies[3];
    for (int i = 0; i < 3; i++) {
        int componentWidth  = yuvInfo.fSizeInfo.fSizes[i].fWidth;
        int componentHeight = yuvInfo.fSizeInfo.fSizes[i].fHeight;
=======
    sk_sp<GrTextureProxy> yuvTextureProxies[SkYUVASizeInfo::kMaxCount];
    for (int i = 0; i < SkYUVASizeInfo::kMaxCount; ++i) {
        if (yuvSizeInfo.fSizes[i].isEmpty()) {
            SkASSERT(!yuvSizeInfo.fWidthBytes[i]);
            continue;
        }

        int componentWidth  = yuvSizeInfo.fSizes[i].fWidth;
        int componentHeight = yuvSizeInfo.fSizes[i].fHeight;
>>>>>>> upstream-releases
        // If the sizes of the components are not all the same we choose to create exact-match
        // textures for the smaller ones rather than add a texture domain to the draw.
        // TODO: revisit this decision to improve texture reuse?
        SkBackingFit fit =
<<<<<<< HEAD
                (componentWidth  != yuvSizeInfo.fSizes[SkYUVSizeInfo::kY].fWidth) ||
                (componentHeight != yuvSizeInfo.fSizes[SkYUVSizeInfo::kY].fHeight)
||||||| merged common ancestors
                (componentWidth  != yuvInfo.fSizeInfo.fSizes[SkYUVSizeInfo::kY].fWidth) ||
                (componentHeight != yuvInfo.fSizeInfo.fSizes[SkYUVSizeInfo::kY].fHeight)
=======
                (componentWidth  != yuvSizeInfo.fSizes[0].fWidth) ||
                (componentHeight != yuvSizeInfo.fSizes[0].fHeight)
>>>>>>> upstream-releases
                    ? SkBackingFit::kExact : SkBackingFit::kApprox;

        SkImageInfo imageInfo = SkImageInfo::MakeA8(componentWidth, componentHeight);
        SkPixmap pixmap(imageInfo, planes[i], yuvSizeInfo.fWidthBytes[i]);
        SkCachedData* dataStoragePtr = dataStorage.get();
        // We grab a ref to cached yuv data. When the SkImage we create below goes away it will call
        // the YUVGen_DataReleaseProc which will release this ref.
        // DDL TODO: Currently we end up creating a lazy proxy that will hold onto a ref to the
        // SkImage in its lambda. This means that we'll keep the ref on the YUV data around for the
        // life time of the proxy and not just upload. For non-DDL draws we should look into
        // releasing this SkImage after uploads (by deleting the lambda after instantiation).
        dataStoragePtr->ref();
        sk_sp<SkImage> yuvImage = SkImage::MakeFromRaster(pixmap, YUVGen_DataReleaseProc,
                                                          dataStoragePtr);

        auto proxyProvider = ctx->priv().proxyProvider();
        yuvTextureProxies[i] = proxyProvider->createTextureProxy(yuvImage, kNone_GrSurfaceFlags,
                                                                 1, SkBudgeted::kYes, fit);

        SkASSERT(yuvTextureProxies[i]->width() == yuvSizeInfo.fSizes[i].fWidth);
        SkASSERT(yuvTextureProxies[i]->height() == yuvSizeInfo.fSizes[i].fHeight);
    }

    // TODO: investigate preallocating mip maps here
<<<<<<< HEAD
    sk_sp<GrRenderTargetContext> renderTargetContext(
        ctx->contextPriv().makeDeferredRenderTargetContext(
            SkBackingFit::kExact, desc.fWidth, desc.fHeight, desc.fConfig, nullptr,
||||||| merged common ancestors
    sk_sp<GrRenderTargetContext> renderTargetContext(ctx->makeDeferredRenderTargetContext(
            SkBackingFit::kExact, desc.fWidth, desc.fHeight, desc.fConfig, std::move(colorSpace),
=======
    sk_sp<GrRenderTargetContext> renderTargetContext(
        ctx->priv().makeDeferredRenderTargetContext(
            format, SkBackingFit::kExact, desc.fWidth, desc.fHeight, desc.fConfig, nullptr,
>>>>>>> upstream-releases
            desc.fSampleCnt, GrMipMapped::kNo, kTopLeft_GrSurfaceOrigin));
    if (!renderTargetContext) {
        return nullptr;
    }

    // This code path only generates I420 (i.e., 3 separate plane) YUVs
    SkYUVAIndex yuvaIndices[4] = {
        {  0, SkColorChannel::kA },
        {  1, SkColorChannel::kA },
        {  2, SkColorChannel::kA },
        { -1, SkColorChannel::kA }, // no alpha
    };

    GrPaint paint;
<<<<<<< HEAD
    auto yuvToRgbProcessor =
            GrYUVtoRGBEffect::Make(yuvTextureProxies, yuvaIndices, yuvColorSpace);
||||||| merged common ancestors
    auto yuvToRgbProcessor =
            GrYUVtoRGBEffect::Make(std::move(yuvTextureProxies[0]),
                                   std::move(yuvTextureProxies[1]),
                                   std::move(yuvTextureProxies[2]),
                                   yuvInfo.fSizeInfo.fSizes, yuvInfo.fColorSpace, false);
=======
    auto yuvToRgbProcessor = GrYUVtoRGBEffect::Make(yuvTextureProxies, yuvaIndices, yuvColorSpace,
                                                    GrSamplerState::Filter::kNearest);
>>>>>>> upstream-releases
    paint.addColorFragmentProcessor(std::move(yuvToRgbProcessor));

    // If the caller expects the pixels in a different color space than the one from the image,
    // apply a color conversion to do this.
    std::unique_ptr<GrFragmentProcessor> colorConversionProcessor =
            GrColorSpaceXformEffect::Make(srcColorSpace, kOpaque_SkAlphaType,
                                          dstColorSpace, kOpaque_SkAlphaType);
    if (colorConversionProcessor) {
        paint.addColorFragmentProcessor(std::move(colorConversionProcessor));
    }

    paint.setPorterDuffXPFactory(SkBlendMode::kSrc);
<<<<<<< HEAD
    const SkRect r = SkRect::MakeIWH(yuvSizeInfo.fSizes[SkYUVSizeInfo::kY].fWidth,
                                     yuvSizeInfo.fSizes[SkYUVSizeInfo::kY].fHeight);
||||||| merged common ancestors
    const SkRect r = SkRect::MakeIWH(yuvInfo.fSizeInfo.fSizes[SkYUVSizeInfo::kY].fWidth,
                                     yuvInfo.fSizeInfo.fSizes[SkYUVSizeInfo::kY].fHeight);
=======
    const SkRect r = SkRect::MakeIWH(yuvSizeInfo.fSizes[0].fWidth,
                                     yuvSizeInfo.fSizes[0].fHeight);
>>>>>>> upstream-releases

    SkMatrix m = SkEncodedOriginToMatrix(yuvSizeInfo.fOrigin, r.width(), r.height());
    renderTargetContext->drawRect(GrNoClip(), std::move(paint), GrAA::kNo, m, r);

    return renderTargetContext->asTextureProxyRef();
}
