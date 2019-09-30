/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrTextureMaker.h"

#include "GrColorSpaceXform.h"
#include "GrGpu.h"
#include "GrProxyProvider.h"
<<<<<<< HEAD

sk_sp<GrTextureProxy> GrTextureMaker::onRefTextureProxyForParams(const GrSamplerState& params,
                                                                 SkColorSpace* dstColorSpace,
                                                                 sk_sp<SkColorSpace>* texColorSpace,
                                                                 bool willBeMipped,
                                                                 SkScalar scaleAdjust[2]) {
    if (this->width() > fContext->contextPriv().caps()->maxTextureSize() ||
        this->height() > fContext->contextPriv().caps()->maxTextureSize()) {
        return nullptr;
||||||| merged common ancestors

sk_sp<GrTextureProxy> GrTextureMaker::refTextureProxyForParams(const GrSamplerState& params,
                                                               SkColorSpace* dstColorSpace,
                                                               sk_sp<SkColorSpace>* texColorSpace,
                                                               SkScalar scaleAdjust[2]) {
    CopyParams copyParams;
    bool willBeMipped = params.filter() == GrSamplerState::Filter::kMipMap;

    if (!fContext->caps()->mipMapSupport()) {
        willBeMipped = false;
=======
#include "GrRecordingContext.h"
#include "GrRecordingContextPriv.h"

sk_sp<GrTextureProxy> GrTextureMaker::onRefTextureProxyForParams(const GrSamplerState& params,
                                                                 bool willBeMipped,
                                                                 SkScalar scaleAdjust[2]) {
    if (this->width() > this->context()->priv().caps()->maxTextureSize() ||
        this->height() > this->context()->priv().caps()->maxTextureSize()) {
        return nullptr;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    CopyParams copyParams;

    if (texColorSpace) {
        *texColorSpace = this->getColorSpace(dstColorSpace);
    }
||||||| merged common ancestors
    if (texColorSpace) {
        *texColorSpace = this->getColorSpace(dstColorSpace);
    }
=======
    CopyParams copyParams;
>>>>>>> upstream-releases

<<<<<<< HEAD
    sk_sp<GrTextureProxy> original(this->refOriginalTextureProxy(willBeMipped, dstColorSpace,
||||||| merged common ancestors
    GrGpu* gpu = fContext->contextPriv().getGpu();
    sk_sp<GrTextureProxy> original(this->refOriginalTextureProxy(willBeMipped, dstColorSpace,
=======
    sk_sp<GrTextureProxy> original(this->refOriginalTextureProxy(willBeMipped,
>>>>>>> upstream-releases
                                                                 AllowedTexGenType::kCheap));
    bool needsCopyForMipsOnly = false;
    if (original) {
<<<<<<< HEAD
        if (!params.isRepeated() ||
            !GrGpu::IsACopyNeededForRepeatWrapMode(fContext->contextPriv().caps(), original.get(),
                                                   original->width(), original->height(),
                                                   params.filter(), &copyParams, scaleAdjust)) {
            needsCopyForMipsOnly = GrGpu::IsACopyNeededForMips(fContext->contextPriv().caps(),
                                                               original.get(), params.filter(),
                                                               &copyParams);
            if (!needsCopyForMipsOnly) {
                return original;
            }
||||||| merged common ancestors
        if (!gpu->isACopyNeededForTextureParams(original.get(), params, &copyParams, scaleAdjust)) {
            return original;
=======
        if (!params.isRepeated() ||
            !GrGpu::IsACopyNeededForRepeatWrapMode(this->context()->priv().caps(), original.get(),
                                                   original->width(), original->height(),
                                                   params.filter(), &copyParams, scaleAdjust)) {
            needsCopyForMipsOnly = GrGpu::IsACopyNeededForMips(this->context()->priv().caps(),
                                                               original.get(), params.filter(),
                                                               &copyParams);
            if (!needsCopyForMipsOnly) {
                return original;
            }
>>>>>>> upstream-releases
        }
    } else {
<<<<<<< HEAD
        if (!params.isRepeated() ||
            !GrGpu::IsACopyNeededForRepeatWrapMode(fContext->contextPriv().caps(), nullptr,
                                                   this->width(), this->height(),
                                                   params.filter(), &copyParams, scaleAdjust)) {
            return this->refOriginalTextureProxy(willBeMipped, dstColorSpace,
                                                 AllowedTexGenType::kAny);
||||||| merged common ancestors
        if (!gpu->isACopyNeededForTextureParams(this->width(), this->height(),
                                                params, &copyParams, scaleAdjust)) {
            return this->refOriginalTextureProxy(willBeMipped, dstColorSpace,
                                                 AllowedTexGenType::kAny);
=======
        if (!params.isRepeated() ||
            !GrGpu::IsACopyNeededForRepeatWrapMode(this->context()->priv().caps(), nullptr,
                                                   this->width(), this->height(),
                                                   params.filter(), &copyParams, scaleAdjust)) {
            return this->refOriginalTextureProxy(willBeMipped, AllowedTexGenType::kAny);
>>>>>>> upstream-releases
        }
    }

    GrProxyProvider* proxyProvider = this->context()->priv().proxyProvider();

    GrSurfaceOrigin origOrigin = original ? original->origin() : kTopLeft_GrSurfaceOrigin;
    GrUniqueKey copyKey;
    this->makeCopyKey(copyParams, &copyKey);
    sk_sp<GrTextureProxy> cachedProxy;
    if (copyKey.isValid()) {
        cachedProxy = proxyProvider->findOrCreateProxyByUniqueKey(copyKey, origOrigin);
        if (cachedProxy && (!willBeMipped || GrMipMapped::kYes == cachedProxy->mipMapped())) {
            return cachedProxy;
        }
    }

    sk_sp<GrTextureProxy> source;
    if (original) {
        source = std::move(original);
    } else if (cachedProxy) {
        source = cachedProxy;
    } else {
        // Since we will be copying this texture there is no reason to make it mipped
<<<<<<< HEAD
        source = this->refOriginalTextureProxy(false, dstColorSpace,
                                               AllowedTexGenType::kAny);
||||||| merged common ancestors
        result = this->refOriginalTextureProxy(false, dstColorSpace,
                                               AllowedTexGenType::kAny);
=======
        source = this->refOriginalTextureProxy(false, AllowedTexGenType::kAny);
>>>>>>> upstream-releases
    }

    if (!source) {
        return nullptr;
    }

<<<<<<< HEAD
    sk_sp<GrTextureProxy> result = CopyOnGpu(fContext, source, copyParams, willBeMipped);
||||||| merged common ancestors
    result = CopyOnGpu(fContext, std::move(result), copyParams, willBeMipped);
=======
    sk_sp<GrTextureProxy> result = CopyOnGpu(this->context(), source, copyParams, willBeMipped);
>>>>>>> upstream-releases

    if (!result) {
        // If we were unable to make a copy and we only needed a copy for mips, then we will return
        // the source texture here and require that the GPU backend is able to fall back to using
        // bilerp if mips are required.
        if (needsCopyForMipsOnly) {
            return source;
        }
        return nullptr;
    }

    if (copyKey.isValid()) {
        SkASSERT(result->origin() == origOrigin);
        if (cachedProxy) {
            SkASSERT(GrMipMapped::kYes == result->mipMapped() &&
                     GrMipMapped::kNo == cachedProxy->mipMapped());
            // If we had a cachedProxy, that means there already is a proxy in the cache which
            // matches the key, but it does not have mip levels and we require them. Thus we must
            // remove the unique key from that proxy.
            SkASSERT(cachedProxy->getUniqueKey() == copyKey);
            proxyProvider->removeUniqueKeyFromProxy(cachedProxy.get());
        }
        proxyProvider->assignUniqueKeyToProxy(copyKey, result.get());
<<<<<<< HEAD
        this->didCacheCopy(copyKey, proxyProvider->contextUniqueID());
||||||| merged common ancestors
        this->didCacheCopy(copyKey);
=======
        this->didCacheCopy(copyKey, proxyProvider->contextID());
>>>>>>> upstream-releases
    }
    return result;
}

std::unique_ptr<GrFragmentProcessor> GrTextureMaker::createFragmentProcessor(
        const SkMatrix& textureMatrix,
        const SkRect& constraintRect,
        FilterConstraint filterConstraint,
        bool coordsLimitedToConstraintRect,
        const GrSamplerState::Filter* filterOrNullForBicubic) {
    const GrSamplerState::Filter* fmForDetermineDomain = filterOrNullForBicubic;
    if (filterOrNullForBicubic && GrSamplerState::Filter::kMipMap == *filterOrNullForBicubic &&
        kYes_FilterConstraint == filterConstraint) {
        // TODO: Here we should force a copy restricted to the constraintRect since MIP maps will
        // read outside the constraint rect. However, as in the adjuster case, we aren't currently
        // doing that.
        // We instead we compute the domain as though were bilerping which is only correct if we
        // only sample level 0.
        static const GrSamplerState::Filter kBilerp = GrSamplerState::Filter::kBilerp;
        fmForDetermineDomain = &kBilerp;
    }

    SkScalar scaleAdjust[2] = { 1.0f, 1.0f };
    sk_sp<GrTextureProxy> proxy(this->refTextureProxyForParams(filterOrNullForBicubic,
                                                               scaleAdjust));
    if (!proxy) {
        return nullptr;
    }
    SkMatrix adjustedMatrix = textureMatrix;
    adjustedMatrix.postScale(scaleAdjust[0], scaleAdjust[1]);

    SkRect domain;
    DomainMode domainMode =
        DetermineDomainMode(constraintRect, filterConstraint, coordsLimitedToConstraintRect,
                            proxy.get(), fmForDetermineDomain, &domain);
    SkASSERT(kTightCopy_DomainMode != domainMode);
<<<<<<< HEAD
    auto fp = CreateFragmentProcessorForDomainAndFilter(std::move(proxy), adjustedMatrix,
                                                        domainMode, domain, filterOrNullForBicubic);
    return GrColorSpaceXformEffect::Make(std::move(fp), texColorSpace.get(), this->alphaType(),
                                         dstColorSpace);
||||||| merged common ancestors
    GrPixelConfig config = proxy->config();
    auto fp = CreateFragmentProcessorForDomainAndFilter(std::move(proxy), adjustedMatrix,
                                                        domainMode, domain, filterOrNullForBicubic);
    return GrColorSpaceXformEffect::Make(std::move(fp), texColorSpace.get(), config, dstColorSpace);
}

sk_sp<GrTextureProxy> GrTextureMaker::generateTextureProxyForParams(const CopyParams& copyParams,
                                                                    bool willBeMipped,
                                                                    SkColorSpace* dstColorSpace) {
    sk_sp<GrTextureProxy> original(this->refOriginalTextureProxy(willBeMipped, dstColorSpace,
                                                                 AllowedTexGenType::kAny));
    if (!original) {
        return nullptr;
    }

    return CopyOnGpu(fContext, std::move(original), copyParams, willBeMipped);
=======
    return this->createFragmentProcessorForDomainAndFilter(
            std::move(proxy), adjustedMatrix, domainMode, domain, filterOrNullForBicubic);
>>>>>>> upstream-releases
}
