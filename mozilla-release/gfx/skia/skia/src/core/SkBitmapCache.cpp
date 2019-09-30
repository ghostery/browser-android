/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkAtomics.h"
#include "SkBitmapCache.h"
#include "SkBitmapProvider.h"
#include "SkImage.h"
#include "SkResourceCache.h"
#include "SkMipMap.h"
#include "SkPixelRef.h"
#include "SkRect.h"

/**
 *  Use this for bitmapcache and mipmapcache entries.
 */
uint64_t SkMakeResourceCacheSharedIDForBitmap(uint32_t bitmapGenID) {
    uint64_t sharedID = SkSetFourByteTag('b', 'm', 'a', 'p');
    return (sharedID << 32) | bitmapGenID;
}

void SkNotifyBitmapGenIDIsStale(uint32_t bitmapGenID) {
    SkResourceCache::PostPurgeSharedID(SkMakeResourceCacheSharedIDForBitmap(bitmapGenID));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

<<<<<<< HEAD
SkBitmapCacheDesc SkBitmapCacheDesc::Make(uint32_t imageID, SkColorType colorType,
                                          SkColorSpace* colorSpace, const SkIRect& subset) {
||||||| merged common ancestors
/**
 This function finds the bounds of the bitmap *within its pixelRef*.
 If the bitmap lacks a pixelRef, it will return an empty rect, since
 that doesn't make sense.  This may be a useful enough function that
 it should be somewhere else (in SkBitmap?).
 */
static SkIRect get_bounds_from_bitmap(const SkBitmap& bm) {
    if (!(bm.pixelRef())) {
        return SkIRect::MakeEmpty();
    }
    SkIPoint origin = bm.pixelRefOrigin();
    return SkIRect::MakeXYWH(origin.fX, origin.fY, bm.width(), bm.height());
}

/**
 *  This function finds the bounds of the image. Today this is just the entire bounds,
 *  but in the future we may support subsets within an image, in which case this should
 *  return that subset (see get_bounds_from_bitmap).
 */
static SkIRect get_bounds_from_image(const SkImage* image) {
    SkASSERT(image->width() > 0 && image->height() > 0);
    return SkIRect::MakeWH(image->width(), image->height());
}

SkBitmapCacheDesc SkBitmapCacheDesc::Make(uint32_t imageID, int origWidth, int origHeight) {
=======
SkBitmapCacheDesc SkBitmapCacheDesc::Make(uint32_t imageID, const SkIRect& subset) {
>>>>>>> upstream-releases
    SkASSERT(imageID);
<<<<<<< HEAD
    SkASSERT(subset.width() > 0 && subset.height() > 0);
    return { imageID,
             colorType,
             colorSpace ? colorSpace->toXYZD50Hash() : 0,
             colorSpace ? colorSpace->transferFnHash() : 0,
             subset };
}

SkBitmapCacheDesc SkBitmapCacheDesc::Make(const SkBitmap& bm) {
    SkASSERT(bm.pixelRef());
    SkIPoint origin = bm.pixelRefOrigin();
    SkIRect bounds = SkIRect::MakeXYWH(origin.fX, origin.fY, bm.width(), bm.height());
    return Make(bm.getGenerationID(), bm.colorType(), bm.colorSpace(), bounds);
||||||| merged common ancestors
    SkASSERT(origWidth > 0 && origHeight > 0);
    return { imageID, 0, 0, {0, 0, origWidth, origHeight} };
}

SkBitmapCacheDesc SkBitmapCacheDesc::Make(const SkBitmap& bm, int scaledWidth, int scaledHeight) {
    SkASSERT(bm.width() > 0 && bm.height() > 0);
    SkASSERT(scaledWidth > 0 && scaledHeight > 0);
    SkASSERT(scaledWidth != bm.width() || scaledHeight != bm.height());

    return { bm.getGenerationID(), scaledWidth, scaledHeight, get_bounds_from_bitmap(bm) };
}

SkBitmapCacheDesc SkBitmapCacheDesc::Make(const SkBitmap& bm) {
    SkASSERT(bm.width() > 0 && bm.height() > 0);
    SkASSERT(bm.pixelRefOrigin() == SkIPoint::Make(0, 0));

    return { bm.getGenerationID(), 0, 0, get_bounds_from_bitmap(bm) };
}

SkBitmapCacheDesc SkBitmapCacheDesc::Make(const SkImage* image, int scaledWidth, int scaledHeight) {
    SkASSERT(image->width() > 0 && image->height() > 0);
    SkASSERT(scaledWidth > 0 && scaledHeight > 0);

    // If the dimensions are the same, should we set them to 0,0?
    //SkASSERT(scaledWidth != image->width() || scaledHeight != image->height());

    return { image->uniqueID(), scaledWidth, scaledHeight, get_bounds_from_image(image) };
=======
    SkASSERT(subset.width() > 0 && subset.height() > 0);
    return { imageID, subset };
>>>>>>> upstream-releases
}

SkBitmapCacheDesc SkBitmapCacheDesc::Make(const SkImage* image) {
<<<<<<< HEAD
    SkIRect bounds = SkIRect::MakeWH(image->width(), image->height());
    return Make(image->uniqueID(), image->colorType(), image->colorSpace(), bounds);
||||||| merged common ancestors
    SkASSERT(image->width() > 0 && image->height() > 0);

    return { image->uniqueID(), 0, 0, get_bounds_from_image(image) };
=======
    SkIRect bounds = SkIRect::MakeWH(image->width(), image->height());
    return Make(image->uniqueID(), bounds);
>>>>>>> upstream-releases
}

namespace {
static unsigned gBitmapKeyNamespaceLabel;

struct BitmapKey : public SkResourceCache::Key {
public:
    BitmapKey(const SkBitmapCacheDesc& desc) : fDesc(desc) {
        this->init(&gBitmapKeyNamespaceLabel, SkMakeResourceCacheSharedIDForBitmap(fDesc.fImageID),
                   sizeof(fDesc));
    }

    const SkBitmapCacheDesc fDesc;
};
}

//////////////////////
#include "SkDiscardableMemory.h"
#include "SkNextID.h"

void SkBitmapCache_setImmutableWithID(SkPixelRef* pr, uint32_t id) {
    pr->setImmutableWithID(id);
}

class SkBitmapCache::Rec : public SkResourceCache::Rec {
public:
    Rec(const SkBitmapCacheDesc& desc, const SkImageInfo& info, size_t rowBytes,
        std::unique_ptr<SkDiscardableMemory> dm, void* block)
        : fKey(desc)
        , fDM(std::move(dm))
        , fMalloc(block)
        , fInfo(info)
        , fRowBytes(rowBytes)
    {
        SkASSERT(!(fDM && fMalloc));    // can't have both

<<<<<<< HEAD
        // We need an ID to return with the bitmap/pixelref. We can't necessarily use the key/desc
        // ID - lazy images cache the same ID with multiple keys (in different color types).
        fPrUniqueID = SkNextID::ImageID();
        REC_TRACE(" Rec(%d): [%d %d] %d\n",
                  sk_atomic_inc(&gRecCounter), fInfo.width(), fInfo.height(), fPrUniqueID);
||||||| merged common ancestors
        // We need an ID to return with the bitmap/pixelref.
        // If they are not scaling, we can return the same ID as the key/desc
        // If they are scaling, we need a new ID
        if (desc.fScaledWidth == 0 && desc.fScaledHeight == 0) {
            fPrUniqueID = desc.fImageID;
        } else {
            fPrUniqueID = SkNextID::ImageID();
        }
        REC_TRACE(" Rec(%d): [%d %d] %d\n",
                  sk_atomic_inc(&gRecCounter), fInfo.width(), fInfo.height(), fPrUniqueID);
=======
        // We need an ID to return with the bitmap/pixelref. We can't necessarily use the key/desc
        // ID - lazy images cache the same ID with multiple keys (in different color types).
        fPrUniqueID = SkNextID::ImageID();
>>>>>>> upstream-releases
    }

    ~Rec() override {
        SkASSERT(0 == fExternalCounter);
        if (fDM && fDiscardableIsLocked) {
            SkASSERT(fDM->data());
            fDM->unlock();
        }
        sk_free(fMalloc);   // may be null
    }

    const Key& getKey() const override { return fKey; }
    size_t bytesUsed() const override {
        return sizeof(fKey) + fInfo.computeByteSize(fRowBytes);
    }
    bool canBePurged() override {
        SkAutoMutexAcquire ama(fMutex);
        return fExternalCounter == 0;
    }
    void postAddInstall(void* payload) override {
        SkAssertResult(this->install(static_cast<SkBitmap*>(payload)));
    }

    const char* getCategory() const override { return "bitmap"; }
    SkDiscardableMemory* diagnostic_only_getDiscardable() const override {
        return fDM.get();
    }

    static void ReleaseProc(void* addr, void* ctx) {
        Rec* rec = static_cast<Rec*>(ctx);
        SkAutoMutexAcquire ama(rec->fMutex);

        SkASSERT(rec->fExternalCounter > 0);
        rec->fExternalCounter -= 1;
        if (rec->fDM) {
            SkASSERT(rec->fMalloc == nullptr);
            if (rec->fExternalCounter == 0) {
                rec->fDM->unlock();
                rec->fDiscardableIsLocked = false;
            }
        } else {
            SkASSERT(rec->fMalloc != nullptr);
        }
    }

    bool install(SkBitmap* bitmap) {
        SkAutoMutexAcquire ama(fMutex);

        if (!fDM && !fMalloc) {
            return false;
        }

        if (fDM) {
            if (!fDiscardableIsLocked) {
                SkASSERT(fExternalCounter == 0);
                if (!fDM->lock()) {
                    fDM.reset(nullptr);
                    return false;
                }
                fDiscardableIsLocked = true;
            }
            SkASSERT(fDM->data());
        }

        bitmap->installPixels(fInfo, fDM ? fDM->data() : fMalloc, fRowBytes, ReleaseProc, this);
        SkBitmapCache_setImmutableWithID(bitmap->pixelRef(), fPrUniqueID);
        fExternalCounter++;

        return true;
    }

    static bool Finder(const SkResourceCache::Rec& baseRec, void* contextBitmap) {
        Rec* rec = (Rec*)&baseRec;
        SkBitmap* result = (SkBitmap*)contextBitmap;
        return rec->install(result);
    }

private:
    BitmapKey   fKey;

    SkMutex     fMutex;

    // either fDM or fMalloc can be non-null, but not both
    std::unique_ptr<SkDiscardableMemory> fDM;
    void*       fMalloc;

    SkImageInfo fInfo;
    size_t      fRowBytes;
    uint32_t    fPrUniqueID;

    // This field counts the number of external pixelrefs we have created.
    // They notify us when they are destroyed so we can decrement this.
    int  fExternalCounter     = 0;
    bool fDiscardableIsLocked = true;
};

void SkBitmapCache::PrivateDeleteRec(Rec* rec) { delete rec; }

SkBitmapCache::RecPtr SkBitmapCache::Alloc(const SkBitmapCacheDesc& desc, const SkImageInfo& info,
                                           SkPixmap* pmap) {
<<<<<<< HEAD
    // Ensure that the info matches the subset (i.e. the subset is the entire image)
    SkASSERT(info.width() == desc.fSubset.width());
    SkASSERT(info.height() == desc.fSubset.height());
    SkASSERT(info.colorType() == desc.fColorType);
    SkASSERT((info.colorSpace() ? info.colorSpace()->toXYZD50Hash() : 0) == desc.fCSXYZHash);
    SkASSERT((info.colorSpace() ? info.colorSpace()->transferFnHash() : 0) ==
             desc.fCSTransferFnHash);
||||||| merged common ancestors
    // Ensure that the caller is self-consistent:
    //  - if they are scaling, the info matches the scaled size
    //  - if they are not, the info matches the subset (i.e. the subset is the entire image)
    if (desc.fScaledWidth == 0 && desc.fScaledHeight == 0) {
        SkASSERT(info.width() == desc.fSubset.width());
        SkASSERT(info.height() == desc.fSubset.height());
    } else {
        SkASSERT(info.width() == desc.fScaledWidth);
        SkASSERT(info.height() == desc.fScaledHeight);
    }
=======
    // Ensure that the info matches the subset (i.e. the subset is the entire image)
    SkASSERT(info.width() == desc.fSubset.width());
    SkASSERT(info.height() == desc.fSubset.height());
>>>>>>> upstream-releases

    const size_t rb = info.minRowBytes();
    size_t size = info.computeByteSize(rb);
    if (SkImageInfo::ByteSizeOverflowed(size)) {
        return nullptr;
    }

    std::unique_ptr<SkDiscardableMemory> dm;
    void* block = nullptr;

    auto factory = SkResourceCache::GetDiscardableFactory();
    if (factory) {
        dm.reset(factory(size));
    } else {
        block = sk_malloc_canfail(size);
    }
    if (!dm && !block) {
        return nullptr;
    }
    *pmap = SkPixmap(info, dm ? dm->data() : block, rb);
    return RecPtr(new Rec(desc, info, rb, std::move(dm), block));
}

void SkBitmapCache::Add(RecPtr rec, SkBitmap* bitmap) {
    SkResourceCache::Add(rec.release(), bitmap);
}

bool SkBitmapCache::Find(const SkBitmapCacheDesc& desc, SkBitmap* result) {
    desc.validate();
    return SkResourceCache::Find(BitmapKey(desc), SkBitmapCache::Rec::Finder, result);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#define CHECK_LOCAL(localCache, localName, globalName, ...) \
    ((localCache) ? localCache->localName(__VA_ARGS__) : SkResourceCache::globalName(__VA_ARGS__))

namespace {
static unsigned gMipMapKeyNamespaceLabel;

struct MipMapKey : public SkResourceCache::Key {
public:
    MipMapKey(const SkBitmapCacheDesc& desc) : fDesc(desc) {
        this->init(&gMipMapKeyNamespaceLabel, SkMakeResourceCacheSharedIDForBitmap(fDesc.fImageID),
                   sizeof(fDesc));
    }

    const SkBitmapCacheDesc fDesc;
};

struct MipMapRec : public SkResourceCache::Rec {
    MipMapRec(const SkBitmapCacheDesc& desc, const SkMipMap* result)
        : fKey(desc)
        , fMipMap(result)
    {
        fMipMap->attachToCacheAndRef();
    }

    ~MipMapRec() override {
        fMipMap->detachFromCacheAndUnref();
    }

    const Key& getKey() const override { return fKey; }
    size_t bytesUsed() const override { return sizeof(fKey) + fMipMap->size(); }
    const char* getCategory() const override { return "mipmap"; }
    SkDiscardableMemory* diagnostic_only_getDiscardable() const override {
        return fMipMap->diagnostic_only_getDiscardable();
    }

    static bool Finder(const SkResourceCache::Rec& baseRec, void* contextMip) {
        const MipMapRec& rec = static_cast<const MipMapRec&>(baseRec);
        const SkMipMap* mm = SkRef(rec.fMipMap);
        // the call to ref() above triggers a "lock" in the case of discardable memory,
        // which means we can now check for null (in case the lock failed).
        if (nullptr == mm->data()) {
            mm->unref();    // balance our call to ref()
            return false;
        }
        // the call must call unref() when they are done.
        *(const SkMipMap**)contextMip = mm;
        return true;
    }

private:
    MipMapKey       fKey;
    const SkMipMap* fMipMap;
};
}

const SkMipMap* SkMipMapCache::FindAndRef(const SkBitmapCacheDesc& desc,
                                          SkResourceCache* localCache) {
    MipMapKey key(desc);
    const SkMipMap* result;

    if (!CHECK_LOCAL(localCache, find, Find, key, MipMapRec::Finder, &result)) {
        result = nullptr;
    }
    return result;
}

static SkResourceCache::DiscardableFactory get_fact(SkResourceCache* localCache) {
    return localCache ? localCache->GetDiscardableFactory()
                      : SkResourceCache::GetDiscardableFactory();
}

<<<<<<< HEAD
const SkMipMap* SkMipMapCache::AddAndRef(const SkBitmap& src, SkResourceCache* localCache) {
    SkMipMap* mipmap = SkMipMap::Build(src, get_fact(localCache));
||||||| merged common ancestors
const SkMipMap* SkMipMapCache::AddAndRef(const SkBitmap& src,
                                         SkDestinationSurfaceColorMode colorMode,
                                         SkResourceCache* localCache) {
    SkMipMap* mipmap = SkMipMap::Build(src, colorMode, get_fact(localCache));
=======
const SkMipMap* SkMipMapCache::AddAndRef(const SkBitmapProvider& provider,
                                         SkResourceCache* localCache) {
    SkBitmap src;
    if (!provider.asBitmap(&src)) {
        return nullptr;
    }

    SkMipMap* mipmap = SkMipMap::Build(src, get_fact(localCache));
>>>>>>> upstream-releases
    if (mipmap) {
<<<<<<< HEAD
        MipMapRec* rec = new MipMapRec(SkBitmapCacheDesc::Make(src), mipmap);
||||||| merged common ancestors
        MipMapRec* rec = new MipMapRec(src.getGenerationID(), get_bounds_from_bitmap(src),
                                       colorMode, mipmap);
=======
        MipMapRec* rec = new MipMapRec(provider.makeCacheDesc(), mipmap);
>>>>>>> upstream-releases
        CHECK_LOCAL(localCache, add, Add, rec);
        provider.notifyAddedToCache();
    }
    return mipmap;
}
