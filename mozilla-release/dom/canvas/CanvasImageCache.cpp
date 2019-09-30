/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "CanvasImageCache.h"
#include "nsAutoPtr.h"
#include "nsIImageLoadingContent.h"
#include "nsExpirationTracker.h"
#include "imgIRequest.h"
#include "mozilla/dom/Element.h"
#include "nsTHashtable.h"
#include "mozilla/dom/HTMLCanvasElement.h"
#include "nsContentUtils.h"
#include "mozilla/Preferences.h"
#include "mozilla/SystemGroup.h"
#include "mozilla/gfx/2D.h"
#include "gfx2DGlue.h"

namespace mozilla {

using namespace dom;
using namespace gfx;

/**
 * Used for images specific to this one canvas. Required
 * due to CORS security.
 */
<<<<<<< HEAD
struct ImageCacheKey {
  ImageCacheKey(imgIContainer* aImage, HTMLCanvasElement* aCanvas,
                bool aIsAccelerated)
      : mImage(aImage), mCanvas(aCanvas), mIsAccelerated(aIsAccelerated) {}
||||||| merged common ancestors
struct ImageCacheKey
{
  ImageCacheKey(imgIContainer* aImage,
                HTMLCanvasElement* aCanvas,
                bool aIsAccelerated)
    : mImage(aImage)
    , mCanvas(aCanvas)
    , mIsAccelerated(aIsAccelerated)
  {}
=======
struct ImageCacheKey {
  ImageCacheKey(imgIContainer* aImage, HTMLCanvasElement* aCanvas)
      : mImage(aImage), mCanvas(aCanvas) {}
>>>>>>> upstream-releases
  nsCOMPtr<imgIContainer> mImage;
  HTMLCanvasElement* mCanvas;
};

/**
 * Cache data needs to be separate from the entry
 * for nsExpirationTracker.
 */
struct ImageCacheEntryData {
  ImageCacheEntryData(const ImageCacheEntryData& aOther)
<<<<<<< HEAD
      : mImage(aOther.mImage),
        mCanvas(aOther.mCanvas),
        mIsAccelerated(aOther.mIsAccelerated),
        mSourceSurface(aOther.mSourceSurface),
        mSize(aOther.mSize) {}
||||||| merged common ancestors
    : mImage(aOther.mImage)
    , mCanvas(aOther.mCanvas)
    , mIsAccelerated(aOther.mIsAccelerated)
    , mSourceSurface(aOther.mSourceSurface)
    , mSize(aOther.mSize)
  {}
=======
      : mImage(aOther.mImage),
        mCanvas(aOther.mCanvas),
        mSourceSurface(aOther.mSourceSurface),
        mSize(aOther.mSize) {}
>>>>>>> upstream-releases
  explicit ImageCacheEntryData(const ImageCacheKey& aKey)
<<<<<<< HEAD
      : mImage(aKey.mImage),
        mCanvas(aKey.mCanvas),
        mIsAccelerated(aKey.mIsAccelerated) {}
||||||| merged common ancestors
    : mImage(aKey.mImage)
    , mCanvas(aKey.mCanvas)
    , mIsAccelerated(aKey.mIsAccelerated)
  {}
=======
      : mImage(aKey.mImage), mCanvas(aKey.mCanvas) {}
>>>>>>> upstream-releases

  nsExpirationState* GetExpirationState() { return &mState; }
  size_t SizeInBytes() { return mSize.width * mSize.height * 4; }

  // Key
  nsCOMPtr<imgIContainer> mImage;
  HTMLCanvasElement* mCanvas;
  // Value
  RefPtr<SourceSurface> mSourceSurface;
  IntSize mSize;
  nsExpirationState mState;
};

class ImageCacheEntry : public PLDHashEntryHdr {
 public:
  typedef ImageCacheKey KeyType;
  typedef const ImageCacheKey* KeyTypePointer;

  explicit ImageCacheEntry(const KeyType* aKey)
      : mData(new ImageCacheEntryData(*aKey)) {}
  ImageCacheEntry(const ImageCacheEntry& toCopy)
      : mData(new ImageCacheEntryData(*toCopy.mData)) {}
  ~ImageCacheEntry() {}

<<<<<<< HEAD
  bool KeyEquals(KeyTypePointer key) const {
    return mData->mImage == key->mImage && mData->mCanvas == key->mCanvas &&
           mData->mIsAccelerated == key->mIsAccelerated;
||||||| merged common ancestors
  bool KeyEquals(KeyTypePointer key) const
  {
    return mData->mImage == key->mImage &&
           mData->mCanvas == key->mCanvas &&
           mData->mIsAccelerated == key->mIsAccelerated;
=======
  bool KeyEquals(KeyTypePointer key) const {
    return mData->mImage == key->mImage && mData->mCanvas == key->mCanvas;
>>>>>>> upstream-releases
  }

  static KeyTypePointer KeyToPointer(KeyType& key) { return &key; }
<<<<<<< HEAD
  static PLDHashNumber HashKey(KeyTypePointer key) {
    return HashGeneric(key->mImage.get(), key->mCanvas, key->mIsAccelerated);
||||||| merged common ancestors
  static PLDHashNumber HashKey(KeyTypePointer key)
  {
    return HashGeneric(key->mImage.get(), key->mCanvas, key->mIsAccelerated);
=======
  static PLDHashNumber HashKey(KeyTypePointer key) {
    return HashGeneric(key->mImage.get(), key->mCanvas);
>>>>>>> upstream-releases
  }
  enum { ALLOW_MEMMOVE = true };

  nsAutoPtr<ImageCacheEntryData> mData;
};

/**
 * Used for all images across all canvases.
 */
<<<<<<< HEAD
struct AllCanvasImageCacheKey {
  AllCanvasImageCacheKey(imgIContainer* aImage, bool aIsAccelerated)
      : mImage(aImage), mIsAccelerated(aIsAccelerated) {}
||||||| merged common ancestors
struct AllCanvasImageCacheKey
{
  AllCanvasImageCacheKey(imgIContainer* aImage,
                         bool aIsAccelerated)
    : mImage(aImage)
    , mIsAccelerated(aIsAccelerated)
  {}
=======
struct AllCanvasImageCacheKey {
  explicit AllCanvasImageCacheKey(imgIContainer* aImage) : mImage(aImage) {}
>>>>>>> upstream-releases

  nsCOMPtr<imgIContainer> mImage;
};

class AllCanvasImageCacheEntry : public PLDHashEntryHdr {
 public:
  typedef AllCanvasImageCacheKey KeyType;
  typedef const AllCanvasImageCacheKey* KeyTypePointer;

  explicit AllCanvasImageCacheEntry(const KeyType* aKey)
<<<<<<< HEAD
      : mImage(aKey->mImage), mIsAccelerated(aKey->mIsAccelerated) {}
||||||| merged common ancestors
    : mImage(aKey->mImage)
    , mIsAccelerated(aKey->mIsAccelerated)
  {}
=======
      : mImage(aKey->mImage) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  AllCanvasImageCacheEntry(const AllCanvasImageCacheEntry& toCopy)
      : mImage(toCopy.mImage),
        mIsAccelerated(toCopy.mIsAccelerated),
        mSourceSurface(toCopy.mSourceSurface) {}
||||||| merged common ancestors
  AllCanvasImageCacheEntry(const AllCanvasImageCacheEntry &toCopy)
    : mImage(toCopy.mImage)
    , mIsAccelerated(toCopy.mIsAccelerated)
    , mSourceSurface(toCopy.mSourceSurface)
  {}
=======
  AllCanvasImageCacheEntry(const AllCanvasImageCacheEntry& toCopy)
      : mImage(toCopy.mImage), mSourceSurface(toCopy.mSourceSurface) {}
>>>>>>> upstream-releases

  ~AllCanvasImageCacheEntry() {}

<<<<<<< HEAD
  bool KeyEquals(KeyTypePointer key) const {
    return mImage == key->mImage && mIsAccelerated == key->mIsAccelerated;
  }
||||||| merged common ancestors
  bool KeyEquals(KeyTypePointer key) const
  {
    return mImage == key->mImage &&
           mIsAccelerated == key->mIsAccelerated;
  }
=======
  bool KeyEquals(KeyTypePointer key) const { return mImage == key->mImage; }
>>>>>>> upstream-releases

  static KeyTypePointer KeyToPointer(KeyType& key) { return &key; }
<<<<<<< HEAD
  static PLDHashNumber HashKey(KeyTypePointer key) {
    return HashGeneric(key->mImage.get(), key->mIsAccelerated);
||||||| merged common ancestors
  static PLDHashNumber HashKey(KeyTypePointer key)
  {
    return HashGeneric(key->mImage.get(), key->mIsAccelerated);
=======
  static PLDHashNumber HashKey(KeyTypePointer key) {
    return HashGeneric(key->mImage.get());
>>>>>>> upstream-releases
  }
  enum { ALLOW_MEMMOVE = true };

  nsCOMPtr<imgIContainer> mImage;
  RefPtr<SourceSurface> mSourceSurface;
};

static bool sPrefsInitialized = false;
static int32_t sCanvasImageCacheLimit = 0;

class ImageCacheObserver;

class ImageCache final : public nsExpirationTracker<ImageCacheEntryData, 4> {
 public:
  // We use 3 generations of 1 second each to get a 2-3 seconds timeout.
  enum { GENERATION_MS = 1000 };
  ImageCache();
  ~ImageCache();

  virtual void NotifyExpired(ImageCacheEntryData* aObject) override {
    mTotal -= aObject->SizeInBytes();
    RemoveObject(aObject);

    // Remove from the all canvas cache entry first since nsExpirationTracker
    // will delete aObject.
<<<<<<< HEAD
    mAllCanvasCache.RemoveEntry(
        AllCanvasImageCacheKey(aObject->mImage, aObject->mIsAccelerated));
||||||| merged common ancestors
    mAllCanvasCache.RemoveEntry(AllCanvasImageCacheKey(aObject->mImage, aObject->mIsAccelerated));
=======
    mAllCanvasCache.RemoveEntry(AllCanvasImageCacheKey(aObject->mImage));
>>>>>>> upstream-releases

    // Deleting the entry will delete aObject since the entry owns aObject.
<<<<<<< HEAD
    mCache.RemoveEntry(ImageCacheKey(aObject->mImage, aObject->mCanvas,
                                     aObject->mIsAccelerated));
||||||| merged common ancestors
    mCache.RemoveEntry(ImageCacheKey(aObject->mImage, aObject->mCanvas, aObject->mIsAccelerated));
=======
    mCache.RemoveEntry(ImageCacheKey(aObject->mImage, aObject->mCanvas));
>>>>>>> upstream-releases
  }

  nsTHashtable<ImageCacheEntry> mCache;
  nsTHashtable<AllCanvasImageCacheEntry> mAllCanvasCache;
  size_t mTotal;
  RefPtr<ImageCacheObserver> mImageCacheObserver;
};

static ImageCache* gImageCache = nullptr;

// Listen memory-pressure event for image cache purge.
class ImageCacheObserver final : public nsIObserver {
 public:
  NS_DECL_ISUPPORTS

  explicit ImageCacheObserver(ImageCache* aImageCache)
      : mImageCache(aImageCache) {
    RegisterMemoryPressureEvent();
  }

  void Destroy() {
    UnregisterMemoryPressureEvent();
    mImageCache = nullptr;
  }

  NS_IMETHOD Observe(nsISupports* aSubject, const char* aTopic,
                     const char16_t* aSomeData) override {
    if (!mImageCache || strcmp(aTopic, "memory-pressure")) {
      return NS_OK;
    }

    mImageCache->AgeAllGenerations();
    return NS_OK;
  }

 private:
  virtual ~ImageCacheObserver() {}

  void RegisterMemoryPressureEvent() {
    nsCOMPtr<nsIObserverService> observerService =
        mozilla::services::GetObserverService();

    MOZ_ASSERT(observerService);

    if (observerService) {
      observerService->AddObserver(this, "memory-pressure", false);
    }
  }

  void UnregisterMemoryPressureEvent() {
    nsCOMPtr<nsIObserverService> observerService =
        mozilla::services::GetObserverService();

    // Do not assert on observerService here. This might be triggered by
    // the cycle collector at a late enough time, that XPCOM services are
    // no longer available. See bug 1029504.
    if (observerService) {
      observerService->RemoveObserver(this, "memory-pressure");
    }
  }

  ImageCache* mImageCache;
};

NS_IMPL_ISUPPORTS(ImageCacheObserver, nsIObserver)

class CanvasImageCacheShutdownObserver final : public nsIObserver {
  ~CanvasImageCacheShutdownObserver() {}

 public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIOBSERVER
};

ImageCache::ImageCache()
    : nsExpirationTracker<ImageCacheEntryData, 4>(
          GENERATION_MS, "ImageCache",
          SystemGroup::EventTargetFor(TaskCategory::Other)),
      mTotal(0) {
  if (!sPrefsInitialized) {
    sPrefsInitialized = true;
    Preferences::AddIntVarCache(&sCanvasImageCacheLimit,
                                "canvas.image.cache.limit", 0);
  }
  mImageCacheObserver = new ImageCacheObserver(this);
  MOZ_RELEASE_ASSERT(mImageCacheObserver,
                     "GFX: Can't alloc ImageCacheObserver");
}

ImageCache::~ImageCache() {
  AgeAllGenerations();
  mImageCacheObserver->Destroy();
}

static already_AddRefed<imgIContainer> GetImageContainer(dom::Element* aImage) {
  nsCOMPtr<imgIRequest> request;
  nsCOMPtr<nsIImageLoadingContent> ilc = do_QueryInterface(aImage);
  if (!ilc) {
    return nullptr;
  }

  ilc->GetRequest(nsIImageLoadingContent::CURRENT_REQUEST,
                  getter_AddRefs(request));
  if (!request) {
    return nullptr;
  }

  nsCOMPtr<imgIContainer> imgContainer;
  request->GetImage(getter_AddRefs(imgContainer));
  if (!imgContainer) {
    return nullptr;
  }

  return imgContainer.forget();
}

<<<<<<< HEAD
void CanvasImageCache::NotifyDrawImage(Element* aImage,
                                       HTMLCanvasElement* aCanvas,
                                       SourceSurface* aSource,
                                       const IntSize& aSize,
                                       bool aIsAccelerated) {
||||||| merged common ancestors
void
CanvasImageCache::NotifyDrawImage(Element* aImage,
                                  HTMLCanvasElement* aCanvas,
                                  SourceSurface* aSource,
                                  const IntSize& aSize,
                                  bool aIsAccelerated)
{
=======
void CanvasImageCache::NotifyDrawImage(Element* aImage,
                                       HTMLCanvasElement* aCanvas,
                                       SourceSurface* aSource,
                                       const IntSize& aSize) {
>>>>>>> upstream-releases
  if (!gImageCache) {
    gImageCache = new ImageCache();
    nsContentUtils::RegisterShutdownObserver(
        new CanvasImageCacheShutdownObserver());
  }

  nsCOMPtr<imgIContainer> imgContainer = GetImageContainer(aImage);
  if (!imgContainer) {
    return;
  }

  AllCanvasImageCacheKey allCanvasCacheKey(imgContainer);
  ImageCacheKey canvasCacheKey(imgContainer, aCanvas);
  ImageCacheEntry* entry = gImageCache->mCache.PutEntry(canvasCacheKey);

  if (entry) {
    if (entry->mData->mSourceSurface) {
      // We are overwriting an existing entry.
      gImageCache->mTotal -= entry->mData->SizeInBytes();
      gImageCache->RemoveObject(entry->mData);
      gImageCache->mAllCanvasCache.RemoveEntry(allCanvasCacheKey);
    }

    gImageCache->AddObject(entry->mData);
    entry->mData->mSourceSurface = aSource;
    entry->mData->mSize = aSize;
    gImageCache->mTotal += entry->mData->SizeInBytes();

    AllCanvasImageCacheEntry* allEntry =
        gImageCache->mAllCanvasCache.PutEntry(allCanvasCacheKey);
    if (allEntry) {
      allEntry->mSourceSurface = aSource;
    }
  }

  if (!sCanvasImageCacheLimit) return;

  // Expire the image cache early if its larger than we want it to be.
  while (gImageCache->mTotal > size_t(sCanvasImageCacheLimit))
    gImageCache->AgeOneGeneration();
}

<<<<<<< HEAD
SourceSurface* CanvasImageCache::LookupAllCanvas(Element* aImage,
                                                 bool aIsAccelerated) {
||||||| merged common ancestors
SourceSurface*
CanvasImageCache::LookupAllCanvas(Element* aImage,
                                  bool aIsAccelerated)
{
=======
SourceSurface* CanvasImageCache::LookupAllCanvas(Element* aImage) {
>>>>>>> upstream-releases
  if (!gImageCache) {
    return nullptr;
  }

  nsCOMPtr<imgIContainer> imgContainer = GetImageContainer(aImage);
  if (!imgContainer) {
    return nullptr;
  }

<<<<<<< HEAD
  AllCanvasImageCacheEntry* entry = gImageCache->mAllCanvasCache.GetEntry(
      AllCanvasImageCacheKey(imgContainer, aIsAccelerated));
||||||| merged common ancestors
  AllCanvasImageCacheEntry* entry =
    gImageCache->mAllCanvasCache.GetEntry(AllCanvasImageCacheKey(imgContainer, aIsAccelerated));
=======
  AllCanvasImageCacheEntry* entry = gImageCache->mAllCanvasCache.GetEntry(
      AllCanvasImageCacheKey(imgContainer));
>>>>>>> upstream-releases
  if (!entry) {
    return nullptr;
  }

  return entry->mSourceSurface;
}

<<<<<<< HEAD
SourceSurface* CanvasImageCache::LookupCanvas(Element* aImage,
                                              HTMLCanvasElement* aCanvas,
                                              IntSize* aSizeOut,
                                              bool aIsAccelerated) {
||||||| merged common ancestors
SourceSurface*
CanvasImageCache::LookupCanvas(Element* aImage,
                               HTMLCanvasElement* aCanvas,
                               IntSize* aSizeOut,
                               bool aIsAccelerated)
{
=======
SourceSurface* CanvasImageCache::LookupCanvas(Element* aImage,
                                              HTMLCanvasElement* aCanvas,
                                              IntSize* aSizeOut) {
>>>>>>> upstream-releases
  if (!gImageCache) {
    return nullptr;
  }

  nsCOMPtr<imgIContainer> imgContainer = GetImageContainer(aImage);
  if (!imgContainer) {
    return nullptr;
  }

<<<<<<< HEAD
  ImageCacheEntry* entry = gImageCache->mCache.GetEntry(
      ImageCacheKey(imgContainer, aCanvas, aIsAccelerated));
||||||| merged common ancestors
  ImageCacheEntry* entry =
    gImageCache->mCache.GetEntry(ImageCacheKey(imgContainer, aCanvas, aIsAccelerated));
=======
  ImageCacheEntry* entry =
      gImageCache->mCache.GetEntry(ImageCacheKey(imgContainer, aCanvas));
>>>>>>> upstream-releases
  if (!entry) {
    return nullptr;
  }

  MOZ_ASSERT(aSizeOut);

  gImageCache->MarkUsed(entry->mData);
  *aSizeOut = entry->mData->mSize;
  return entry->mData->mSourceSurface;
}

NS_IMPL_ISUPPORTS(CanvasImageCacheShutdownObserver, nsIObserver)

NS_IMETHODIMP
CanvasImageCacheShutdownObserver::Observe(nsISupports* aSubject,
                                          const char* aTopic,
                                          const char16_t* aData) {
  if (strcmp(aTopic, NS_XPCOM_SHUTDOWN_OBSERVER_ID) == 0) {
    delete gImageCache;
    gImageCache = nullptr;

    nsContentUtils::UnregisterShutdownObserver(this);
  }

  return NS_OK;
}

}  // namespace mozilla
