/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkImage_Lazy.h"

#include "SkBitmap.h"
#include "SkBitmapCache.h"
#include "SkCachedData.h"
#include "SkData.h"
#include "SkImageGenerator.h"
#include "SkImagePriv.h"
#include "SkNextID.h"

#if SK_SUPPORT_GPU
#include "GrCaps.h"
#include "GrGpuResourcePriv.h"
#include "GrImageTextureMaker.h"
#include "GrResourceKey.h"
#include "GrProxyProvider.h"
#include "GrRecordingContext.h"
#include "GrRecordingContextPriv.h"
#include "GrSamplerState.h"
#include "GrYUVProvider.h"
#include "SkGr.h"
#endif

// Ref-counted tuple(SkImageGenerator, SkMutex) which allows sharing one generator among N images
class SharedGenerator final : public SkNVRefCnt<SharedGenerator> {
public:
    static sk_sp<SharedGenerator> Make(std::unique_ptr<SkImageGenerator> gen) {
        return gen ? sk_sp<SharedGenerator>(new SharedGenerator(std::move(gen))) : nullptr;
    }

    // This is thread safe.  It is a const field set in the constructor.
    const SkImageInfo& getInfo() { return fGenerator->getInfo(); }

private:
    explicit SharedGenerator(std::unique_ptr<SkImageGenerator> gen)
            : fGenerator(std::move(gen)) {
        SkASSERT(fGenerator);
    }

    friend class ScopedGenerator;
    friend class SkImage_Lazy;

    std::unique_ptr<SkImageGenerator> fGenerator;
    SkMutex                           fMutex;
};

///////////////////////////////////////////////////////////////////////////////

SkImage_Lazy::Validator::Validator(sk_sp<SharedGenerator> gen, const SkIRect* subset,
                                   const SkColorType* colorType, sk_sp<SkColorSpace> colorSpace)
        : fSharedGenerator(std::move(gen)) {
    if (!fSharedGenerator) {
        return;
    }

    // The following generator accessors are safe without acquiring the mutex (const getters).
    // TODO: refactor to use a ScopedGenerator instead, for clarity.
    const SkImageInfo& info = fSharedGenerator->fGenerator->getInfo();
    if (info.isEmpty()) {
        fSharedGenerator.reset();
        return;
    }

    fUniqueID = fSharedGenerator->fGenerator->uniqueID();
    const SkIRect bounds = SkIRect::MakeWH(info.width(), info.height());
    if (subset) {
        if (!bounds.contains(*subset)) {
            fSharedGenerator.reset();
            return;
        }
        if (*subset != bounds) {
            // we need a different uniqueID since we really are a subset of the raw generator
            fUniqueID = SkNextID::ImageID();
        }
    } else {
        subset = &bounds;
    }

    fInfo   = info.makeWH(subset->width(), subset->height());
    fOrigin = SkIPoint::Make(subset->x(), subset->y());
    if (colorType || colorSpace) {
        if (colorType) {
            fInfo = fInfo.makeColorType(*colorType);
        }
        if (colorSpace) {
            fInfo = fInfo.makeColorSpace(colorSpace);
        }
        fUniqueID = SkNextID::ImageID();
    }
}

///////////////////////////////////////////////////////////////////////////////

// Helper for exclusive access to a shared generator.
class SkImage_Lazy::ScopedGenerator {
public:
    ScopedGenerator(const sk_sp<SharedGenerator>& gen)
      : fSharedGenerator(gen)
      , fAutoAquire(gen->fMutex) {}

    SkImageGenerator* operator->() const {
        fSharedGenerator->fMutex.assertHeld();
        return fSharedGenerator->fGenerator.get();
    }

    operator SkImageGenerator*() const {
        fSharedGenerator->fMutex.assertHeld();
        return fSharedGenerator->fGenerator.get();
    }

private:
    const sk_sp<SharedGenerator>& fSharedGenerator;
    SkAutoExclusive               fAutoAquire;
};

///////////////////////////////////////////////////////////////////////////////

SkImage_Lazy::SkImage_Lazy(Validator* validator)
        : INHERITED(validator->fInfo.width(), validator->fInfo.height(), validator->fUniqueID)
        , fSharedGenerator(std::move(validator->fSharedGenerator))
        , fInfo(validator->fInfo)
        , fOrigin(validator->fOrigin) {
    SkASSERT(fSharedGenerator);
    fUniqueID = validator->fUniqueID;
}

SkImage_Lazy::~SkImage_Lazy() {
#if SK_SUPPORT_GPU
<<<<<<< HEAD
    for (int i = 0; i < fUniqueKeyInvalidatedMessages.count(); ++i) {
        SkMessageBus<GrUniqueKeyInvalidatedMessage>::Post(*fUniqueKeyInvalidatedMessages[i]);
    }
    fUniqueKeyInvalidatedMessages.deleteAll();
||||||| merged common ancestors
    bool supportsHalfFloat() const {
        return !fCaps || (fCaps->isConfigTexturable(kRGBA_half_GrPixelConfig) &&
                          fCaps->isConfigRenderable(kRGBA_half_GrPixelConfig));
    }

    bool supportsSRGB() const {
        return !fCaps ||
            (fCaps->srgbSupport() && fCaps->isConfigTexturable(kSRGBA_8888_GrPixelConfig));
    }

    bool supportsSBGR() const {
        return !fCaps || fCaps->srgbSupport();
    }
#else
    bool supportsHalfFloat() const { return true; }
    bool supportsSRGB() const { return true; }
    bool supportsSBGR() const { return true; }
=======
    for (int i = 0; i < fUniqueKeyInvalidatedMessages.count(); ++i) {
        SkMessageBus<GrUniqueKeyInvalidatedMessage>::Post(*fUniqueKeyInvalidatedMessages[i]);
    }
    fUniqueKeyInvalidatedMessages.deleteAll();
>>>>>>> upstream-releases
#endif
<<<<<<< HEAD
}

//////////////////////////////////////////////////////////////////////////////////////////////////

static bool check_output_bitmap(const SkBitmap& bitmap, const SkImageInfo& info) {
    SkASSERT(bitmap.isImmutable());
    SkASSERT(bitmap.getPixels());
    SkASSERT(bitmap.colorType() == info.colorType());
    SkASSERT(SkColorSpace::Equals(bitmap.colorSpace(), info.colorSpace()));
    return true;
}

bool SkImage_Lazy::directGeneratePixels(const SkImageInfo& info, void* pixels, size_t rb,
                                        int srcX, int srcY) const {
    ScopedGenerator generator(fSharedGenerator);
    const SkImageInfo& genInfo = generator->getInfo();
    // Currently generators do not natively handle subsets, so check that first.
    if (srcX || srcY || genInfo.width() != info.width() || genInfo.height() != info.height()) {
        return false;
    }

    return generator->getPixels(info, pixels, rb);
||||||| merged common ancestors

    const GrCaps* fCaps;
};

SkImageCacherator::CachedFormat SkImage_Lazy::chooseCacheFormat(SkColorSpace* dstColorSpace,
                                                                const GrCaps* grCaps) const {
    SkColorSpace* cs = fInfo.colorSpace();
    if (!cs || !dstColorSpace) {
        return kLegacy_CachedFormat;
    }

    CacheCaps caps(grCaps);
    switch (fInfo.colorType()) {
        case kUnknown_SkColorType:
        case kAlpha_8_SkColorType:
        case kRGB_565_SkColorType:
        case kARGB_4444_SkColorType:
        case kRGB_888x_SkColorType:
        case kRGBA_1010102_SkColorType:
        case kRGB_101010x_SkColorType:
            // We don't support color space on these formats, so always decode in legacy mode:
            // TODO: Ask the codec to decode these to something else (at least sRGB 8888)?
            return kLegacy_CachedFormat;

        case kGray_8_SkColorType:
            // TODO: What do we do with grayscale sources that have strange color spaces attached?
            // The codecs and color space xform don't handle this correctly (yet), so drop it on
            // the floor. (Also, inflating by a factor of 8 is going to be unfortunate).
            // As it is, we don't directly support sRGB grayscale, so ask the codec to convert
            // it for us. This bypasses some really sketchy code GrUploadPixmapToTexture.
            if (cs->gammaCloseToSRGB() && caps.supportsSRGB()) {
                return kSRGB8888_CachedFormat;
            } else {
                return kLegacy_CachedFormat;
            }

        case kRGBA_8888_SkColorType:
            if (cs->gammaCloseToSRGB()) {
                if (caps.supportsSRGB()) {
                    return kSRGB8888_CachedFormat;
                } else if (caps.supportsHalfFloat()) {
                    return kLinearF16_CachedFormat;
                } else {
                    return kLegacy_CachedFormat;
                }
            } else {
                if (caps.supportsHalfFloat()) {
                    return kLinearF16_CachedFormat;
                } else if (caps.supportsSRGB()) {
                    return kSRGB8888_CachedFormat;
                } else {
                    return kLegacy_CachedFormat;
                }
            }

        case kBGRA_8888_SkColorType:
            // Odd case. sBGRA isn't a real thing, so we may not have this texturable.
            if (caps.supportsSBGR()) {
                if (cs->gammaCloseToSRGB()) {
                    return kSBGR8888_CachedFormat;
                } else if (caps.supportsHalfFloat()) {
                    return kLinearF16_CachedFormat;
                } else if (caps.supportsSRGB()) {
                    return kSRGB8888_CachedFormat;
                } else {
                    // sBGRA support without sRGBA is highly unlikely (impossible?) Nevertheless.
                    return kLegacy_CachedFormat;
                }
            } else {
                if (cs->gammaCloseToSRGB()) {
                    if (caps.supportsSRGB()) {
                        return kSRGB8888_CachedFormat;
                    } else if (caps.supportsHalfFloat()) {
                        return kLinearF16_CachedFormat;
                    } else {
                        return kLegacy_CachedFormat;
                    }
                } else {
                    if (caps.supportsHalfFloat()) {
                        return kLinearF16_CachedFormat;
                    } else if (caps.supportsSRGB()) {
                        return kSRGB8888_CachedFormat;
                    } else {
                        return kLegacy_CachedFormat;
                    }
                }
            }

        case kRGBA_F16_SkColorType:
            if (caps.supportsHalfFloat()) {
                return kLinearF16_CachedFormat;
            } else if (caps.supportsSRGB()) {
                return kSRGB8888_CachedFormat;
            } else {
                return kLegacy_CachedFormat;
            }
    }
    SkDEBUGFAIL("Unreachable");
    return kLegacy_CachedFormat;
}

SkImageInfo SkImage_Lazy::buildCacheInfo(CachedFormat format) const {
    switch (format) {
        case kLegacy_CachedFormat:
            return fInfo.makeColorSpace(nullptr);
        case kLinearF16_CachedFormat:
            return fInfo.makeColorType(kRGBA_F16_SkColorType)
                        .makeColorSpace(fInfo.colorSpace()->makeLinearGamma());
        case kSRGB8888_CachedFormat:
            // If the transfer function is nearly (but not exactly) sRGB, we don't want the codec
            // to bother trans-coding. It would be slow, and do more harm than good visually,
            // so we make sure to leave the colorspace as-is.
            if (fInfo.colorSpace()->gammaCloseToSRGB()) {
                return fInfo.makeColorType(kRGBA_8888_SkColorType);
            } else {
                return fInfo.makeColorType(kRGBA_8888_SkColorType)
                            .makeColorSpace(fInfo.colorSpace()->makeSRGBGamma());
            }
        case kSBGR8888_CachedFormat:
            // See note above about not-quite-sRGB transfer functions.
            if (fInfo.colorSpace()->gammaCloseToSRGB()) {
                return fInfo.makeColorType(kBGRA_8888_SkColorType);
            } else {
                return fInfo.makeColorType(kBGRA_8888_SkColorType)
                            .makeColorSpace(fInfo.colorSpace()->makeSRGBGamma());
            }
        default:
            SkDEBUGFAIL("Invalid cached format");
            return fInfo;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

static bool check_output_bitmap(const SkBitmap& bitmap, uint32_t expectedID) {
    SkASSERT(bitmap.getGenerationID() == expectedID);
    SkASSERT(bitmap.isImmutable());
    SkASSERT(bitmap.getPixels());
    return true;
}

bool SkImage_Lazy::directGeneratePixels(const SkImageInfo& info, void* pixels, size_t rb,
                                        int srcX, int srcY,
                                        SkTransferFunctionBehavior behavior) const {
    ScopedGenerator generator(fSharedGenerator);
    const SkImageInfo& genInfo = generator->getInfo();
    // Currently generators do not natively handle subsets, so check that first.
    if (srcX || srcY || genInfo.width() != info.width() || genInfo.height() != info.height()) {
        return false;
    }

    SkImageGenerator::Options opts;
    // TODO: This should respect the behavior argument.
    opts.fBehavior = SkTransferFunctionBehavior::kIgnore;
    return generator->getPixels(info, pixels, rb, &opts);
=======
>>>>>>> upstream-releases
}

//////////////////////////////////////////////////////////////////////////////////////////////////

<<<<<<< HEAD
bool SkImage_Lazy::lockAsBitmapOnlyIfAlreadyCached(SkBitmap* bitmap,
                                                   const SkImageInfo& dstInfo) const {
    auto desc = SkBitmapCacheDesc::Make(fUniqueID, dstInfo.colorType(), dstInfo.colorSpace(),
                                        SkIRect::MakeSize(fInfo.dimensions()));
    return SkBitmapCache::Find(desc, bitmap) &&
           check_output_bitmap(*bitmap, dstInfo);
}

static bool generate_pixels(SkImageGenerator* gen, const SkPixmap& pmap, int originX, int originY) {
||||||| merged common ancestors
bool SkImage_Lazy::lockAsBitmapOnlyIfAlreadyCached(SkBitmap* bitmap, CachedFormat format) const {
    uint32_t uniqueID = this->getUniqueID(format);
    return SkBitmapCache::Find(SkBitmapCacheDesc::Make(uniqueID,
                                                       fInfo.width(), fInfo.height()), bitmap) &&
           check_output_bitmap(*bitmap, uniqueID);
}

static bool generate_pixels(SkImageGenerator* gen, const SkPixmap& pmap, int originX, int originY,
                            SkTransferFunctionBehavior behavior) {
=======
static bool generate_pixels(SkImageGenerator* gen, const SkPixmap& pmap, int originX, int originY) {
>>>>>>> upstream-releases
    const int genW = gen->getInfo().width();
    const int genH = gen->getInfo().height();
    const SkIRect srcR = SkIRect::MakeWH(genW, genH);
    const SkIRect dstR = SkIRect::MakeXYWH(originX, originY, pmap.width(), pmap.height());
    if (!srcR.contains(dstR)) {
        return false;
    }

    // If they are requesting a subset, we have to have a temp allocation for full image, and
    // then copy the subset into their allocation
    SkBitmap full;
    SkPixmap fullPM;
    const SkPixmap* dstPM = &pmap;
    if (srcR != dstR) {
        if (!full.tryAllocPixels(pmap.info().makeWH(genW, genH))) {
            return false;
        }
        if (!full.peekPixels(&fullPM)) {
            return false;
        }
        dstPM = &fullPM;
    }

    if (!gen->getPixels(dstPM->info(), dstPM->writable_addr(), dstPM->rowBytes())) {
        return false;
    }

    if (srcR != dstR) {
        if (!full.readPixels(pmap, originX, originY)) {
            return false;
        }
    }
    return true;
}

<<<<<<< HEAD
bool SkImage_Lazy::lockAsBitmap(SkBitmap* bitmap, SkImage::CachingHint chint,
                                const SkImageInfo& info) const {
    if (this->lockAsBitmapOnlyIfAlreadyCached(bitmap, info)) {
||||||| merged common ancestors
bool SkImage_Lazy::lockAsBitmap(SkBitmap* bitmap, SkImage::CachingHint chint, CachedFormat format,
                                const SkImageInfo& info,
                                SkTransferFunctionBehavior behavior) const {
    if (this->lockAsBitmapOnlyIfAlreadyCached(bitmap, format)) {
=======
bool SkImage_Lazy::getROPixels(SkBitmap* bitmap, SkImage::CachingHint chint) const {
    auto check_output_bitmap = [bitmap]() {
        SkASSERT(bitmap->isImmutable());
        SkASSERT(bitmap->getPixels());
        (void)bitmap;
    };

    auto desc = SkBitmapCacheDesc::Make(this);
    if (SkBitmapCache::Find(desc, bitmap)) {
        check_output_bitmap();
>>>>>>> upstream-releases
        return true;
    }

<<<<<<< HEAD
    SkBitmap tmpBitmap;
    SkBitmapCache::RecPtr cacheRec;
    SkPixmap pmap;
||||||| merged common ancestors
    uint32_t uniqueID = this->getUniqueID(format);

    SkBitmap tmpBitmap;
    SkBitmapCache::RecPtr cacheRec;
    SkPixmap pmap;
=======
>>>>>>> upstream-releases
    if (SkImage::kAllow_CachingHint == chint) {
<<<<<<< HEAD
        auto desc = SkBitmapCacheDesc::Make(fUniqueID, info.colorType(), info.colorSpace(),
                                            SkIRect::MakeSize(info.dimensions()));
        cacheRec = SkBitmapCache::Alloc(desc, info, &pmap);
        if (!cacheRec) {
||||||| merged common ancestors
        auto desc = SkBitmapCacheDesc::Make(uniqueID, info.width(), info.height());
        cacheRec = SkBitmapCache::Alloc(desc, info, &pmap);
        if (!cacheRec) {
=======
        SkPixmap pmap;
        SkBitmapCache::RecPtr cacheRec = SkBitmapCache::Alloc(desc, fInfo, &pmap);
        if (!cacheRec ||
            !generate_pixels(ScopedGenerator(fSharedGenerator), pmap,
                             fOrigin.x(), fOrigin.y())) {
>>>>>>> upstream-releases
            return false;
        }
        SkBitmapCache::Add(std::move(cacheRec), bitmap);
        this->notifyAddedToRasterCache();
    } else {
        if (!bitmap->tryAllocPixels(fInfo) ||
            !generate_pixels(ScopedGenerator(fSharedGenerator), bitmap->pixmap(),
                             fOrigin.x(), fOrigin.y())) {
            return false;
        }
        bitmap->setImmutable();
    }

<<<<<<< HEAD
    ScopedGenerator generator(fSharedGenerator);
    if (!generate_pixels(generator, pmap, fOrigin.x(), fOrigin.y())) {
        return false;
    }

    if (cacheRec) {
        SkBitmapCache::Add(std::move(cacheRec), bitmap);
        this->notifyAddedToRasterCache();
    } else {
        *bitmap = tmpBitmap;
        bitmap->setImmutable();
    }

    check_output_bitmap(*bitmap, info);
||||||| merged common ancestors
    ScopedGenerator generator(fSharedGenerator);
    if (!generate_pixels(generator, pmap, fOrigin.x(), fOrigin.y(), behavior)) {
        return false;
    }

    if (cacheRec) {
        SkBitmapCache::Add(std::move(cacheRec), bitmap);
        SkASSERT(bitmap->getPixels());  // we're locked
        SkASSERT(bitmap->isImmutable());
        SkASSERT(bitmap->getGenerationID() == uniqueID);
        this->notifyAddedToCache();
    } else {
        *bitmap = tmpBitmap;
        bitmap->pixelRef()->setImmutableWithID(uniqueID);
    }

    check_output_bitmap(*bitmap, uniqueID);
=======
    check_output_bitmap();
>>>>>>> upstream-releases
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

bool SkImage_Lazy::onReadPixels(const SkImageInfo& dstInfo, void* dstPixels, size_t dstRB,
                                int srcX, int srcY, CachingHint chint) const {
    SkBitmap bm;
<<<<<<< HEAD
    if (kDisallow_CachingHint == chint) {
        if (this->lockAsBitmapOnlyIfAlreadyCached(&bm, dstInfo)) {
            return bm.readPixels(dstInfo, dstPixels, dstRB, srcX, srcY);
        } else {
            // Try passing the caller's buffer directly down to the generator. If this fails we
            // may still succeed in the general case, as the generator may prefer some other
            // config, which we could then convert via SkBitmap::readPixels.
            if (this->directGeneratePixels(dstInfo, dstPixels, dstRB, srcX, srcY)) {
                return true;
            }
            // else fall through
        }
    }

    if (this->getROPixels(&bm, dstColorSpace, chint)) {
||||||| merged common ancestors
    if (kDisallow_CachingHint == chint) {
        CachedFormat cacheFormat = this->chooseCacheFormat(dstColorSpace);
        SkImageInfo genPixelsInfo = dstInfo;
        SkTransferFunctionBehavior behavior = getGeneratorBehaviorAndInfo(&genPixelsInfo);
        if (this->lockAsBitmapOnlyIfAlreadyCached(&bm, cacheFormat)) {
            return bm.readPixels(dstInfo, dstPixels, dstRB, srcX, srcY);
        } else {
            // Try passing the caller's buffer directly down to the generator. If this fails we
            // may still succeed in the general case, as the generator may prefer some other
            // config, which we could then convert via SkBitmap::readPixels.
            if (this->directGeneratePixels(genPixelsInfo, dstPixels, dstRB, srcX, srcY, behavior)) {
                return true;
            }
            // else fall through
        }
    }

    if (this->getROPixels(&bm, dstColorSpace, chint)) {
=======
    if (this->getROPixels(&bm, chint)) {
>>>>>>> upstream-releases
        return bm.readPixels(dstInfo, dstPixels, dstRB, srcX, srcY);
    }
    return false;
}

sk_sp<SkData> SkImage_Lazy::onRefEncoded() const {
    ScopedGenerator generator(fSharedGenerator);
    return generator->refEncodedData();
}

<<<<<<< HEAD
bool SkImage_Lazy::getROPixels(SkBitmap* bitmap, SkColorSpace* dstColorSpace,
                               CachingHint chint) const {
    return this->lockAsBitmap(bitmap, chint, fInfo);
}

||||||| merged common ancestors
bool SkImage_Lazy::getROPixels(SkBitmap* bitmap, SkColorSpace* dstColorSpace,
                               CachingHint chint) const {
    CachedFormat cacheFormat = this->chooseCacheFormat(dstColorSpace);
    const SkImageInfo cacheInfo = this->buildCacheInfo(cacheFormat);
    SkImageInfo genPixelsInfo = cacheInfo;
    SkTransferFunctionBehavior behavior = getGeneratorBehaviorAndInfo(&genPixelsInfo);
    return this->lockAsBitmap(bitmap, chint, cacheFormat, genPixelsInfo, behavior);
}

=======
>>>>>>> upstream-releases
bool SkImage_Lazy::onIsValid(GrContext* context) const {
    ScopedGenerator generator(fSharedGenerator);
    return generator->isValid(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

#if SK_SUPPORT_GPU
sk_sp<GrTextureProxy> SkImage_Lazy::asTextureProxyRef(GrRecordingContext* context,
                                                      const GrSamplerState& params,
                                                      SkScalar scaleAdjust[2]) const {
    if (!context) {
        return nullptr;
    }

    GrImageTextureMaker textureMaker(context, this, kAllow_CachingHint);
    return textureMaker.refTextureProxyForParams(params, scaleAdjust);
}
#endif

sk_sp<SkImage> SkImage_Lazy::onMakeSubset(GrRecordingContext* context,
                                          const SkIRect& subset) const {
    SkASSERT(fInfo.bounds().contains(subset));
    SkASSERT(fInfo.bounds() != subset);

    const SkIRect generatorSubset = subset.makeOffset(fOrigin.x(), fOrigin.y());
    const SkColorType colorType = fInfo.colorType();
    Validator validator(fSharedGenerator, &generatorSubset, &colorType, fInfo.refColorSpace());
    return validator ? sk_sp<SkImage>(new SkImage_Lazy(&validator)) : nullptr;
}

<<<<<<< HEAD
sk_sp<SkImage> SkImage_Lazy::onMakeColorSpace(sk_sp<SkColorSpace> target) const {
    SkAutoExclusive autoAquire(fOnMakeColorSpaceMutex);
    if (target && fOnMakeColorSpaceTarget &&
        SkColorSpace::Equals(target.get(), fOnMakeColorSpaceTarget.get())) {
        return fOnMakeColorSpaceResult;
||||||| merged common ancestors
sk_sp<SkImage> SkImage_Lazy::onMakeColorSpace(sk_sp<SkColorSpace> target,
                                              SkColorType targetColorType,
                                              SkTransferFunctionBehavior premulBehavior) const {
    SkAutoExclusive autoAquire(fOnMakeColorSpaceMutex);
    if (target && fOnMakeColorSpaceTarget &&
        SkColorSpace::Equals(target.get(), fOnMakeColorSpaceTarget.get())) {
        return fOnMakeColorSpaceResult;
=======
sk_sp<SkImage> SkImage_Lazy::onMakeColorTypeAndColorSpace(GrRecordingContext*,
                                                          SkColorType targetCT,
                                                          sk_sp<SkColorSpace> targetCS) const {
    SkAutoExclusive autoAquire(fOnMakeColorTypeAndSpaceMutex);
    if (fOnMakeColorTypeAndSpaceResult &&
        targetCT == fOnMakeColorTypeAndSpaceResult->colorType() &&
        SkColorSpace::Equals(targetCS.get(), fOnMakeColorTypeAndSpaceResult->colorSpace())) {
        return fOnMakeColorTypeAndSpaceResult;
>>>>>>> upstream-releases
    }
    const SkIRect generatorSubset =
            SkIRect::MakeXYWH(fOrigin.x(), fOrigin.y(), fInfo.width(), fInfo.height());
    Validator validator(fSharedGenerator, &generatorSubset, &targetCT, targetCS);
    sk_sp<SkImage> result = validator ? sk_sp<SkImage>(new SkImage_Lazy(&validator)) : nullptr;
    if (result) {
        fOnMakeColorTypeAndSpaceResult = result;
    }
    return result;
}

sk_sp<SkImage> SkImage::MakeFromGenerator(std::unique_ptr<SkImageGenerator> generator,
                                          const SkIRect* subset) {
    SkImage_Lazy::Validator
            validator(SharedGenerator::Make(std::move(generator)), subset, nullptr, nullptr);

    return validator ? sk_make_sp<SkImage_Lazy>(&validator) : nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

#if SK_SUPPORT_GPU

<<<<<<< HEAD
void SkImage_Lazy::makeCacheKeyFromOrigKey(const GrUniqueKey& origKey,
                                           GrUniqueKey* cacheKey) const {
    // TODO: Take dstColorSpace, include hash in key
||||||| merged common ancestors
void SkImage_Lazy::makeCacheKeyFromOrigKey(const GrUniqueKey& origKey, CachedFormat format,
                                           GrUniqueKey* cacheKey) {
=======
void SkImage_Lazy::makeCacheKeyFromOrigKey(const GrUniqueKey& origKey,
                                           GrUniqueKey* cacheKey) const {
>>>>>>> upstream-releases
    SkASSERT(!cacheKey->isValid());
    if (origKey.isValid()) {
        static const GrUniqueKey::Domain kDomain = GrUniqueKey::GenerateDomain();
        GrUniqueKey::Builder builder(cacheKey, origKey, kDomain, 0, "Image");
    }
}

class Generator_GrYUVProvider : public GrYUVProvider {
public:
    Generator_GrYUVProvider(SkImageGenerator* gen) : fGen(gen) {}

<<<<<<< HEAD
private:
    uint32_t onGetID() const override { return fGen->uniqueID(); }
    bool onQueryYUV8(SkYUVSizeInfo* sizeInfo, SkYUVColorSpace* colorSpace) const override {
        return fGen->queryYUV8(sizeInfo, colorSpace);
||||||| merged common ancestors
    uint32_t onGetID() override { return fGen->uniqueID(); }
    bool onQueryYUV8(SkYUVSizeInfo* sizeInfo, SkYUVColorSpace* colorSpace) const override {
        return fGen->queryYUV8(sizeInfo, colorSpace);
=======
private:
    uint32_t onGetID() const override { return fGen->uniqueID(); }
    bool onQueryYUVA8(SkYUVASizeInfo* sizeInfo,
                      SkYUVAIndex yuvaIndices[SkYUVAIndex::kIndexCount],
                      SkYUVColorSpace* colorSpace) const override {
        return fGen->queryYUVA8(sizeInfo, yuvaIndices, colorSpace);
>>>>>>> upstream-releases
    }
    bool onGetYUVA8Planes(const SkYUVASizeInfo& sizeInfo,
                          const SkYUVAIndex yuvaIndices[SkYUVAIndex::kIndexCount],
                          void* planes[]) override {
        return fGen->getYUVA8Planes(sizeInfo, yuvaIndices, planes);
    }

    SkImageGenerator* fGen;

    typedef GrYUVProvider INHERITED;
};

static void set_key_on_proxy(GrProxyProvider* proxyProvider,
                             GrTextureProxy* proxy, GrTextureProxy* originalProxy,
                             const GrUniqueKey& key) {
    if (key.isValid()) {
        if (originalProxy && originalProxy->getUniqueKey().isValid()) {
            SkASSERT(originalProxy->getUniqueKey() == key);
            SkASSERT(GrMipMapped::kYes == proxy->mipMapped() &&
                     GrMipMapped::kNo == originalProxy->mipMapped());
            // If we had an originalProxy with a valid key, that means there already is a proxy in
            // the cache which matches the key, but it does not have mip levels and we require them.
            // Thus we must remove the unique key from that proxy.
            SkASSERT(originalProxy->getUniqueKey() == key);
            proxyProvider->removeUniqueKeyFromProxy(originalProxy);
        }
        proxyProvider->assignUniqueKeyToProxy(key, proxy);
    }
}

<<<<<<< HEAD
sk_sp<SkCachedData> SkImage_Lazy::getPlanes(SkYUVSizeInfo* yuvSizeInfo,
                                            SkYUVColorSpace* yuvColorSpace,
                                            const void* planes[3]) {
    ScopedGenerator generator(fSharedGenerator);
    Generator_GrYUVProvider provider(generator);

    sk_sp<SkCachedData> data = provider.getPlanes(yuvSizeInfo, yuvColorSpace, planes);
    if (!data) {
        return nullptr;
||||||| merged common ancestors
sk_sp<SkColorSpace> SkImage_Lazy::getColorSpace(GrContext* ctx, SkColorSpace* dstColorSpace) {
    if (!dstColorSpace) {
        // In legacy mode, we do no modification to the image's color space or encoding.
        // Subsequent legacy drawing is likely to ignore the color space, but some clients
        // may want to know what space the image data is in, so return it.
        return fInfo.refColorSpace();
    } else {
        CachedFormat format = this->chooseCacheFormat(dstColorSpace, ctx->caps());
        SkImageInfo cacheInfo = this->buildCacheInfo(format);
        return cacheInfo.refColorSpace();
=======
sk_sp<SkCachedData> SkImage_Lazy::getPlanes(SkYUVASizeInfo* yuvaSizeInfo,
                                            SkYUVAIndex yuvaIndices[SkYUVAIndex::kIndexCount],
                                            SkYUVColorSpace* yuvColorSpace,
                                            const void* planes[SkYUVASizeInfo::kMaxCount]) {
    ScopedGenerator generator(fSharedGenerator);
    Generator_GrYUVProvider provider(generator);

    sk_sp<SkCachedData> data = provider.getPlanes(yuvaSizeInfo, yuvaIndices, yuvColorSpace, planes);
    if (!data) {
        return nullptr;
>>>>>>> upstream-releases
    }

    return data;
}


/*
 *  We have 4 ways to try to return a texture (in sorted order)
 *
 *  1. Check the cache for a pre-existing one
 *  2. Ask the generator to natively create one
 *  3. Ask the generator to return YUV planes, which the GPU can convert
 *  4. Ask the generator to return RGB(A) data, which the GPU can convert
 */
<<<<<<< HEAD
sk_sp<GrTextureProxy> SkImage_Lazy::lockTextureProxy(
        GrContext* ctx,
        const GrUniqueKey& origKey,
        SkImage::CachingHint chint,
        bool willBeMipped,
        SkColorSpace* dstColorSpace,
        GrTextureMaker::AllowedTexGenType genType) const {
||||||| merged common ancestors
sk_sp<GrTextureProxy> SkImage_Lazy::lockTextureProxy(GrContext* ctx,
                                                     const GrUniqueKey& origKey,
                                                     SkImage::CachingHint chint,
                                                     bool willBeMipped,
                                                     SkColorSpace* dstColorSpace,
                                                     GrTextureMaker::AllowedTexGenType genType) {
=======
sk_sp<GrTextureProxy> SkImage_Lazy::lockTextureProxy(
        GrRecordingContext* ctx,
        const GrUniqueKey& origKey,
        SkImage::CachingHint chint,
        bool willBeMipped,
        GrTextureMaker::AllowedTexGenType genType) const {
>>>>>>> upstream-releases
    // Values representing the various texture lock paths we can take. Used for logging the path
    // taken to a histogram.
    enum LockTexturePath {
        kFailure_LockTexturePath,
        kPreExisting_LockTexturePath,
        kNative_LockTexturePath,
        kCompressed_LockTexturePath, // Deprecated
        kYUV_LockTexturePath,
        kRGBA_LockTexturePath,
    };

    enum { kLockTexturePathCount = kRGBA_LockTexturePath + 1 };

<<<<<<< HEAD
    // Build our texture key.
    // Even though some proxies created here may have a specific origin and use that origin, we do
    // not include that in the key. Since SkImages are meant to be immutable, a given SkImage will
    // always have an associated proxy that is always one origin or the other. It never can change
    // origins. Thus we don't need to include that info in the key iteself.
    // TODO: This needs to include the dstColorSpace.
||||||| merged common ancestors
    // Determine which cached format we're going to use (which may involve decoding to a different
    // info than the generator provides).
    CachedFormat format = this->chooseCacheFormat(dstColorSpace, ctx->caps());

    // Fold the cache format into our texture key
=======
    // Build our texture key.
    // Even though some proxies created here may have a specific origin and use that origin, we do
    // not include that in the key. Since SkImages are meant to be immutable, a given SkImage will
    // always have an associated proxy that is always one origin or the other. It never can change
    // origins. Thus we don't need to include that info in the key iteself.
>>>>>>> upstream-releases
    GrUniqueKey key;
    this->makeCacheKeyFromOrigKey(origKey, &key);

    GrProxyProvider* proxyProvider = ctx->priv().proxyProvider();
    sk_sp<GrTextureProxy> proxy;

    // 1. Check the cache for a pre-existing one
    if (key.isValid()) {
        proxy = proxyProvider->findOrCreateProxyByUniqueKey(key, kTopLeft_GrSurfaceOrigin);
        if (proxy) {
            SK_HISTOGRAM_ENUMERATION("LockTexturePath", kPreExisting_LockTexturePath,
                                     kLockTexturePathCount);
            if (!willBeMipped || GrMipMapped::kYes == proxy->mipMapped()) {
                return proxy;
            }
        }
    }

    // 2. Ask the generator to natively create one
    if (!proxy) {
        ScopedGenerator generator(fSharedGenerator);
        if (GrTextureMaker::AllowedTexGenType::kCheap == genType &&
                SkImageGenerator::TexGenType::kCheap != generator->onCanGenerateTexture()) {
            return nullptr;
        }
        if ((proxy = generator->generateTexture(ctx, fInfo, fOrigin, willBeMipped))) {
            SK_HISTOGRAM_ENUMERATION("LockTexturePath", kNative_LockTexturePath,
                                     kLockTexturePathCount);
            set_key_on_proxy(proxyProvider, proxy.get(), nullptr, key);
            if (!willBeMipped || GrMipMapped::kYes == proxy->mipMapped()) {
<<<<<<< HEAD
                *fUniqueKeyInvalidatedMessages.append() =
                        new GrUniqueKeyInvalidatedMessage(key, ctx->uniqueID());
||||||| merged common ancestors
=======
                *fUniqueKeyInvalidatedMessages.append() =
                        new GrUniqueKeyInvalidatedMessage(key, ctx->priv().contextID());
>>>>>>> upstream-releases
                return proxy;
            }
        }
    }

    // 3. Ask the generator to return YUV planes, which the GPU can convert. If we will be mipping
    //    the texture we fall through here and have the CPU generate the mip maps for us.
<<<<<<< HEAD
    if (!proxy && !willBeMipped && !ctx->contextPriv().disableGpuYUVConversion()) {
        const GrSurfaceDesc desc = GrImageInfoToSurfaceDesc(fInfo);
||||||| merged common ancestors
    if (!proxy && !willBeMipped && !ctx->contextPriv().disableGpuYUVConversion()) {
        const GrSurfaceDesc desc = GrImageInfoToSurfaceDesc(cacheInfo, *ctx->caps());
=======
    if (!proxy && !willBeMipped && !ctx->priv().options().fDisableGpuYUVConversion) {
        const GrSurfaceDesc desc = GrImageInfoToSurfaceDesc(fInfo);

        SkColorType colorType = fInfo.colorType();
        GrBackendFormat format =
                ctx->priv().caps()->getBackendFormatFromColorType(colorType);

>>>>>>> upstream-releases
        ScopedGenerator generator(fSharedGenerator);
        Generator_GrYUVProvider provider(generator);

<<<<<<< HEAD
        // The pixels in the texture will be in the generator's color space. If onMakeColorSpace
        // has been called then this will not match this image's color space. To correct this, apply
        // a color space conversion from the generator's color space to this image's color space.
        SkColorSpace* generatorColorSpace = fSharedGenerator->fGenerator->getInfo().colorSpace();
        SkColorSpace* thisColorSpace = fInfo.colorSpace();
||||||| merged common ancestors
        // The pixels in the texture will be in the generator's color space. If onMakeColorSpace
        // has been called then this will not match this image's color space. To correct this, apply
        // a color space conversion from the generator's color space to this image's color space.
        const SkColorSpace* generatorColorSpace =
                fSharedGenerator->fGenerator->getInfo().colorSpace();
        const SkColorSpace* thisColorSpace = fInfo.colorSpace();
=======
        // The pixels in the texture will be in the generator's color space.
        // If onMakeColorTypeAndColorSpace has been called then this will not match this image's
        // color space. To correct this, apply a color space conversion from the generator's color
        // space to this image's color space.
        SkColorSpace* generatorColorSpace = fSharedGenerator->fGenerator->getInfo().colorSpace();
        SkColorSpace* thisColorSpace = fInfo.colorSpace();
>>>>>>> upstream-releases

<<<<<<< HEAD
        // TODO: Update to create the mipped surface in the YUV generator and draw the base
        // layer directly into the mipped surface.
        proxy = provider.refAsTextureProxy(ctx, desc, generatorColorSpace, thisColorSpace);
||||||| merged common ancestors
        // TODO: Update to create the mipped surface in the YUV generator and draw the base layer
        // directly into the mipped surface.
        proxy = provider.refAsTextureProxy(ctx, desc, generatorColorSpace, thisColorSpace);
=======
        // TODO: Update to create the mipped surface in the YUV generator and draw the base
        // layer directly into the mipped surface.
        proxy = provider.refAsTextureProxy(ctx, format, desc, generatorColorSpace, thisColorSpace);
>>>>>>> upstream-releases
        if (proxy) {
            SK_HISTOGRAM_ENUMERATION("LockTexturePath", kYUV_LockTexturePath,
                                     kLockTexturePathCount);
            set_key_on_proxy(proxyProvider, proxy.get(), nullptr, key);
<<<<<<< HEAD
            *fUniqueKeyInvalidatedMessages.append() =
                    new GrUniqueKeyInvalidatedMessage(key, ctx->uniqueID());
||||||| merged common ancestors
=======
            *fUniqueKeyInvalidatedMessages.append() =
                    new GrUniqueKeyInvalidatedMessage(key, ctx->priv().contextID());
>>>>>>> upstream-releases
            return proxy;
        }
    }

    // 4. Ask the generator to return RGB(A) data, which the GPU can convert
    SkBitmap bitmap;
<<<<<<< HEAD
    if (!proxy && this->lockAsBitmap(&bitmap, chint, fInfo)) {
||||||| merged common ancestors
    if (!proxy && this->lockAsBitmap(&bitmap, chint, format, genPixelsInfo, behavior)) {
=======
    if (!proxy && this->getROPixels(&bitmap, chint)) {
>>>>>>> upstream-releases
        if (willBeMipped) {
            proxy = proxyProvider->createMipMapProxyFromBitmap(bitmap);
        }
        if (!proxy) {
            proxy = GrUploadBitmapToTextureProxy(proxyProvider, bitmap);
        }
        if (proxy && (!willBeMipped || GrMipMapped::kYes == proxy->mipMapped())) {
            SK_HISTOGRAM_ENUMERATION("LockTexturePath", kRGBA_LockTexturePath,
                                     kLockTexturePathCount);
            set_key_on_proxy(proxyProvider, proxy.get(), nullptr, key);
<<<<<<< HEAD
            *fUniqueKeyInvalidatedMessages.append() =
                    new GrUniqueKeyInvalidatedMessage(key, ctx->uniqueID());
||||||| merged common ancestors
=======
            *fUniqueKeyInvalidatedMessages.append() =
                    new GrUniqueKeyInvalidatedMessage(key, ctx->priv().contextID());
>>>>>>> upstream-releases
            return proxy;
        }
    }

    if (proxy) {
        // We need a mipped proxy, but we either found a proxy earlier that wasn't mipped, generated
        // a native non mipped proxy, or generated a non-mipped yuv proxy. Thus we generate a new
        // mipped surface and copy the original proxy into the base layer. We will then let the gpu
        // generate the rest of the mips.
        SkASSERT(willBeMipped);
        SkASSERT(GrMipMapped::kNo == proxy->mipMapped());
<<<<<<< HEAD
        *fUniqueKeyInvalidatedMessages.append() =
                new GrUniqueKeyInvalidatedMessage(key, ctx->uniqueID());
||||||| merged common ancestors
=======
        *fUniqueKeyInvalidatedMessages.append() =
                new GrUniqueKeyInvalidatedMessage(key, ctx->priv().contextID());
>>>>>>> upstream-releases
        if (auto mippedProxy = GrCopyBaseMipMapToTextureProxy(ctx, proxy.get())) {
            set_key_on_proxy(proxyProvider, mippedProxy.get(), proxy.get(), key);
            return mippedProxy;
        }
        // We failed to make a mipped proxy with the base copied into it. This could have
        // been from failure to make the proxy or failure to do the copy. Thus we will fall
        // back to just using the non mipped proxy; See skbug.com/7094.
        return proxy;
    }

    SK_HISTOGRAM_ENUMERATION("LockTexturePath", kFailure_LockTexturePath,
                             kLockTexturePathCount);
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
