/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_ASURFACE_H
#define GFX_ASURFACE_H

#include "mozilla/MemoryReporting.h"
#include "mozilla/UniquePtr.h"

#include "gfxPoint.h"
#include "gfxRect.h"
#include "gfxTypes.h"
#include "nscore.h"
#include "nsSize.h"
#include "mozilla/gfx/Rect.h"

#include "nsStringFwd.h"

class gfxImageSurface;

template <typename T>
struct already_AddRefed;

/**
 * A surface is something you can draw on. Instantiate a subclass of this
 * abstract class, and use gfxContext to draw on this surface.
 */
class gfxASurface {
 public:
#ifdef MOZILLA_INTERNAL_API
  nsrefcnt AddRef(void);
  nsrefcnt Release(void);
#else
  virtual nsrefcnt AddRef(void);
  virtual nsrefcnt Release(void);
#endif

<<<<<<< HEAD
 public:
  /** Wrap the given cairo surface and return a gfxASurface for it.
   * This adds a reference to csurf (owned by the returned gfxASurface).
   */
  static already_AddRefed<gfxASurface> Wrap(
      cairo_surface_t *csurf,
      const mozilla::gfx::IntSize &aSize = mozilla::gfx::IntSize(-1, -1));

  /*** this DOES NOT addref the surface */
  cairo_surface_t *CairoSurface() { return mSurface; }

  gfxSurfaceType GetType() const;

  gfxContentType GetContentType() const;

  void SetDeviceOffset(const gfxPoint &offset);
  gfxPoint GetDeviceOffset() const;

  void Flush() const;
  void MarkDirty();
  void MarkDirty(const gfxRect &r);
||||||| merged common ancestors
public:

    /** Wrap the given cairo surface and return a gfxASurface for it.
     * This adds a reference to csurf (owned by the returned gfxASurface).
     */
    static already_AddRefed<gfxASurface> Wrap(cairo_surface_t *csurf, const mozilla::gfx::IntSize& aSize = mozilla::gfx::IntSize(-1, -1));

    /*** this DOES NOT addref the surface */
    cairo_surface_t *CairoSurface() {
        return mSurface;
    }

    gfxSurfaceType GetType() const;
=======
 public:
  /** Wrap the given cairo surface and return a gfxASurface for it.
   * This adds a reference to csurf (owned by the returned gfxASurface).
   */
  static already_AddRefed<gfxASurface> Wrap(
      cairo_surface_t* csurf,
      const mozilla::gfx::IntSize& aSize = mozilla::gfx::IntSize(-1, -1));

  /*** this DOES NOT addref the surface */
  cairo_surface_t* CairoSurface() { return mSurface; }

  gfxSurfaceType GetType() const;

  gfxContentType GetContentType() const;

  void SetDeviceOffset(const gfxPoint& offset);
  gfxPoint GetDeviceOffset() const;

  void Flush() const;
  void MarkDirty();
  void MarkDirty(const gfxRect& r);
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Printing backend functions */
  virtual nsresult BeginPrinting(const nsAString &aTitle,
                                 const nsAString &aPrintToFileName);
  virtual nsresult EndPrinting();
  virtual nsresult AbortPrinting();
  virtual nsresult BeginPage();
  virtual nsresult EndPage();
||||||| merged common ancestors
    gfxContentType GetContentType() const;

    void SetDeviceOffset(const gfxPoint& offset);
    gfxPoint GetDeviceOffset() const;

    void Flush() const;
    void MarkDirty();
    void MarkDirty(const gfxRect& r);

    /* Printing backend functions */
    virtual nsresult BeginPrinting(const nsAString& aTitle, const nsAString& aPrintToFileName);
    virtual nsresult EndPrinting();
    virtual nsresult AbortPrinting();
    virtual nsresult BeginPage();
    virtual nsresult EndPage();
=======
  /* Printing backend functions */
  virtual nsresult BeginPrinting(const nsAString& aTitle,
                                 const nsAString& aPrintToFileName);
  virtual nsresult EndPrinting();
  virtual nsresult AbortPrinting();
  virtual nsresult BeginPage();
  virtual nsresult EndPage();
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetData(const cairo_user_data_key_t *key, void *user_data,
               thebes_destroy_func_t destroy);
  void *GetData(const cairo_user_data_key_t *key);
||||||| merged common ancestors
    void SetData(const cairo_user_data_key_t *key,
                 void *user_data,
                 thebes_destroy_func_t destroy);
    void *GetData(const cairo_user_data_key_t *key);
=======
  void SetData(const cairo_user_data_key_t* key, void* user_data,
               thebes_destroy_func_t destroy);
  void* GetData(const cairo_user_data_key_t* key);
>>>>>>> upstream-releases

  virtual void Finish();

<<<<<<< HEAD
  /**
   * Create an offscreen surface that can be efficiently copied into
   * this surface (at least if tiling is not involved).
   * Returns null on error.
   */
  virtual already_AddRefed<gfxASurface> CreateSimilarSurface(
      gfxContentType aType, const mozilla::gfx::IntSize &aSize);
||||||| merged common ancestors
    /**
     * Create an offscreen surface that can be efficiently copied into
     * this surface (at least if tiling is not involved).
     * Returns null on error.
     */
    virtual already_AddRefed<gfxASurface> CreateSimilarSurface(gfxContentType aType,
                                                               const mozilla::gfx::IntSize& aSize);
=======
  /**
   * Create an offscreen surface that can be efficiently copied into
   * this surface (at least if tiling is not involved).
   * Returns null on error.
   */
  virtual already_AddRefed<gfxASurface> CreateSimilarSurface(
      gfxContentType aType, const mozilla::gfx::IntSize& aSize);
>>>>>>> upstream-releases

  /**
   * Returns an image surface for this surface, or nullptr if not supported.
   * This will not copy image data, just wraps an image surface around
   * pixel data already available in memory.
   */
  virtual already_AddRefed<gfxImageSurface> GetAsImageSurface();

  /**
   * Creates a new ARGB32 image surface with the same contents as this surface.
   * Returns null on error.
   */
  already_AddRefed<gfxImageSurface> CopyToARGB32ImageSurface();

  int CairoStatus();

  static gfxContentType ContentFromFormat(gfxImageFormat format);

  /**
   * Record number of bytes for given surface type.  Use positive bytes
   * for allocations and negative bytes for deallocations.
   */
  static void RecordMemoryUsedForSurfaceType(gfxSurfaceType aType,
                                             int32_t aBytes);

  /**
   * Same as above, but use current surface type as returned by GetType().
   * The bytes will be accumulated until RecordMemoryFreed is called,
   * in which case the value that was recorded for this surface will
   * be freed.
   */
  void RecordMemoryUsed(int32_t aBytes);
  void RecordMemoryFreed();

  virtual int32_t KnownMemoryUsed() { return mBytesRecorded; }

  virtual size_t SizeOfExcludingThis(mozilla::MallocSizeOf aMallocSizeOf) const;
  virtual size_t SizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf) const;
  // gfxASurface has many sub-classes.  This method indicates if a sub-class
  // is capable of measuring its own size accurately.  If not, the caller
  // must fall back to a computed size.  (Note that gfxASurface can actually
  // measure itself, but we must |return false| here because it serves as the
  // (conservative) default for all the sub-classes.  Therefore, this
  // function should only be called on a |gfxASurface*| that actually points
  // to a sub-class of gfxASurface.)
  virtual bool SizeOfIsMeasured() const { return false; }

  static int32_t BytePerPixelFromFormat(gfxImageFormat format);

  virtual const mozilla::gfx::IntSize GetSize() const;

  virtual mozilla::gfx::SurfaceFormat GetSurfaceFormat() const;

<<<<<<< HEAD
  void SetOpaqueRect(const gfxRect &aRect);
||||||| merged common ancestors
    void SetOpaqueRect(const gfxRect& aRect);
=======
  void SetOpaqueRect(const gfxRect& aRect);
>>>>>>> upstream-releases

<<<<<<< HEAD
  const gfxRect &GetOpaqueRect() {
    if (!!mOpaqueRect) return *mOpaqueRect;
    return GetEmptyOpaqueRect();
  }
||||||| merged common ancestors
    const gfxRect& GetOpaqueRect() {
        if (!!mOpaqueRect)
            return *mOpaqueRect;
        return GetEmptyOpaqueRect();
    }
=======
  const gfxRect& GetOpaqueRect() {
    if (!!mOpaqueRect) return *mOpaqueRect;
    return GetEmptyOpaqueRect();
  }
>>>>>>> upstream-releases

  static uint8_t BytesPerPixel(gfxImageFormat aImageFormat);

 protected:
  gfxASurface();

<<<<<<< HEAD
  static gfxASurface *GetSurfaceWrapper(cairo_surface_t *csurf);
  static void SetSurfaceWrapper(cairo_surface_t *csurf, gfxASurface *asurf);
||||||| merged common ancestors
    static gfxASurface* GetSurfaceWrapper(cairo_surface_t *csurf);
    static void SetSurfaceWrapper(cairo_surface_t *csurf, gfxASurface *asurf);
=======
  static gfxASurface* GetSurfaceWrapper(cairo_surface_t* csurf);
  static void SetSurfaceWrapper(cairo_surface_t* csurf, gfxASurface* asurf);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // NB: Init() *must* be called from within subclass's
  // constructors.  It's unsafe to call it after the ctor finishes;
  // leaks and use-after-frees are possible.
  void Init(cairo_surface_t *surface, bool existingSurface = false);

  // out-of-line helper to allow GetOpaqueRect() to be inlined
  // without including gfxRect.h here
  static const gfxRect &GetEmptyOpaqueRect();

  virtual ~gfxASurface();

  cairo_surface_t *mSurface;
  mozilla::UniquePtr<gfxRect> mOpaqueRect;

 private:
  static void SurfaceDestroyFunc(void *data);
||||||| merged common ancestors
    // NB: Init() *must* be called from within subclass's
    // constructors.  It's unsafe to call it after the ctor finishes;
    // leaks and use-after-frees are possible.
    void Init(cairo_surface_t *surface, bool existingSurface = false);

    // out-of-line helper to allow GetOpaqueRect() to be inlined
    // without including gfxRect.h here
    static const gfxRect& GetEmptyOpaqueRect();

    virtual ~gfxASurface();

    cairo_surface_t *mSurface;
    mozilla::UniquePtr<gfxRect> mOpaqueRect;

private:
    static void SurfaceDestroyFunc(void *data);
=======
  // NB: Init() *must* be called from within subclass's
  // constructors.  It's unsafe to call it after the ctor finishes;
  // leaks and use-after-frees are possible.
  void Init(cairo_surface_t* surface, bool existingSurface = false);

  // out-of-line helper to allow GetOpaqueRect() to be inlined
  // without including gfxRect.h here
  static const gfxRect& GetEmptyOpaqueRect();

  virtual ~gfxASurface();

  cairo_surface_t* mSurface;
  mozilla::UniquePtr<gfxRect> mOpaqueRect;

 private:
  static void SurfaceDestroyFunc(void* data);
>>>>>>> upstream-releases

  int32_t mFloatingRefs;
  int32_t mBytesRecorded;

 protected:
  bool mSurfaceValid;
};

/**
 * An Unknown surface; used to wrap unknown cairo_surface_t returns from cairo
 */
class gfxUnknownSurface : public gfxASurface {
<<<<<<< HEAD
 public:
  gfxUnknownSurface(cairo_surface_t *surf, const mozilla::gfx::IntSize &aSize)
      : mSize(aSize) {
    Init(surf, true);
  }

  virtual ~gfxUnknownSurface() {}
  virtual const mozilla::gfx::IntSize GetSize() const override { return mSize; }

 private:
  mozilla::gfx::IntSize mSize;
||||||| merged common ancestors
public:
    gfxUnknownSurface(cairo_surface_t *surf, const mozilla::gfx::IntSize& aSize)
        : mSize(aSize)
    {
        Init(surf, true);
    }

    virtual ~gfxUnknownSurface() { }
    virtual const mozilla::gfx::IntSize GetSize() const override { return mSize; }

private:
    mozilla::gfx::IntSize mSize;
=======
 public:
  gfxUnknownSurface(cairo_surface_t* surf, const mozilla::gfx::IntSize& aSize)
      : mSize(aSize) {
    Init(surf, true);
  }

  virtual ~gfxUnknownSurface() = default;
  const mozilla::gfx::IntSize GetSize() const override { return mSize; }

 private:
  mozilla::gfx::IntSize mSize;
>>>>>>> upstream-releases
};

#endif /* GFX_ASURFACE_H */
