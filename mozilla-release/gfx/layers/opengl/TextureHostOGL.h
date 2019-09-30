/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_TEXTUREOGL_H
#define MOZILLA_GFX_TEXTUREOGL_H

#include <stddef.h>  // for size_t
#include <stdint.h>  // for uint64_t
#include "CompositableHost.h"
#include "GLContextTypes.h"  // for GLContext
#include "GLDefs.h"          // for GLenum, LOCAL_GL_CLAMP_TO_EDGE, etc
#include "GLTextureImage.h"  // for TextureImage
#include "gfxTypes.h"
#include "mozilla/GfxMessageUtils.h"         // for gfxContentType
#include "mozilla/Assertions.h"              // for MOZ_ASSERT, etc
#include "mozilla/Attributes.h"              // for override
#include "mozilla/RefPtr.h"                  // for RefPtr
#include "mozilla/gfx/Matrix.h"              // for Matrix4x4
#include "mozilla/gfx/Point.h"               // for IntSize, IntPoint
#include "mozilla/gfx/Types.h"               // for SurfaceFormat, etc
#include "mozilla/layers/CompositorOGL.h"    // for CompositorOGL
#include "mozilla/layers/CompositorTypes.h"  // for TextureFlags
<<<<<<< HEAD
#include "mozilla/layers/LayersSurfaces.h"   // for SurfaceDescriptor
#include "mozilla/layers/TextureHost.h"      // for TextureHost, etc
#include "mozilla/mozalloc.h"                // for operator delete, etc
#include "mozilla/webrender/RenderThread.h"
#include "nsCOMPtr.h"          // for already_AddRefed
#include "nsDebug.h"           // for NS_WARNING
#include "nsISupportsImpl.h"   // for TextureImage::Release, etc
#include "nsRegionFwd.h"       // for nsIntRegion
#include "OGLShaderProgram.h"  // for ShaderProgramType, etc
||||||| merged common ancestors
#include "mozilla/layers/LayersSurfaces.h"  // for SurfaceDescriptor
#include "mozilla/layers/TextureHost.h"  // for TextureHost, etc
#include "mozilla/mozalloc.h"           // for operator delete, etc
#include "nsCOMPtr.h"                   // for already_AddRefed
#include "nsDebug.h"                    // for NS_WARNING
#include "nsISupportsImpl.h"            // for TextureImage::Release, etc
#include "nsRegionFwd.h"                // for nsIntRegion
#include "OGLShaderProgram.h"           // for ShaderProgramType, etc
=======
#include "mozilla/layers/LayersSurfaces.h"   // for SurfaceDescriptor
#include "mozilla/layers/TextureHost.h"      // for TextureHost, etc
#include "mozilla/mozalloc.h"                // for operator delete, etc
#include "mozilla/webrender/RenderThread.h"
#include "nsCOMPtr.h"         // for already_AddRefed
#include "nsDebug.h"          // for NS_WARNING
#include "nsISupportsImpl.h"  // for TextureImage::Release, etc
#include "nsRegionFwd.h"      // for nsIntRegion
>>>>>>> upstream-releases

#ifdef MOZ_WIDGET_ANDROID
#  include "GeneratedJNIWrappers.h"
#  include "AndroidSurfaceTexture.h"
#endif

namespace mozilla {
namespace gfx {
class DataSourceSurface;
}  // namespace gfx

namespace layers {

class Compositor;
class CompositorOGL;
class TextureImageTextureSourceOGL;
class GLTextureSource;

<<<<<<< HEAD
inline void ApplySamplingFilterToBoundTexture(
    gl::GLContext* aGL, gfx::SamplingFilter aSamplingFilter,
    GLuint aTarget = LOCAL_GL_TEXTURE_2D) {
  GLenum filter =
      (aSamplingFilter == gfx::SamplingFilter::POINT ? LOCAL_GL_NEAREST
                                                     : LOCAL_GL_LINEAR);
||||||| merged common ancestors
inline void ApplySamplingFilterToBoundTexture(gl::GLContext* aGL,
                                              gfx::SamplingFilter aSamplingFilter,
                                              GLuint aTarget = LOCAL_GL_TEXTURE_2D)
{
  GLenum filter =
    (aSamplingFilter == gfx::SamplingFilter::POINT ? LOCAL_GL_NEAREST : LOCAL_GL_LINEAR);
=======
void ApplySamplingFilterToBoundTexture(gl::GLContext* aGL,
                                       gfx::SamplingFilter aSamplingFilter,
                                       GLuint aTarget = LOCAL_GL_TEXTURE_2D);
>>>>>>> upstream-releases

already_AddRefed<TextureHost> CreateTextureHostOGL(
    const SurfaceDescriptor& aDesc, ISurfaceAllocator* aDeallocator,
    LayersBackend aBackend, TextureFlags aFlags);

/*
 * TextureHost implementations for the OpenGL backend.
 *
 * Note that it is important to be careful about the ownership model with
 * the OpenGL backend, due to some widget limitation on Linux: before
 * the nsBaseWidget associated with our OpenGL context has been completely
 * deleted, every resource belonging to the OpenGL context MUST have been
 * released. At the moment the teardown sequence happens in the middle of
 * the nsBaseWidget's destructor, meaning that at a given moment we must be
 * able to easily find and release all the GL resources.
 * The point is: be careful about the ownership model and limit the number
 * of objects sharing references to GL resources to make the tear down
 * sequence as simple as possible.
 */

/**
 * TextureSourceOGL provides the necessary API for CompositorOGL to composite
 * a TextureSource.
 */
class TextureSourceOGL {
 public:
  TextureSourceOGL()
      : mCachedSamplingFilter(gfx::SamplingFilter::GOOD),
        mHasCachedSamplingFilter(false) {}

  virtual bool IsValid() const = 0;

  virtual void BindTexture(GLenum aTextureUnit,
                           gfx::SamplingFilter aSamplingFilter) = 0;

  // To be overridden in textures that need this. This method will be called
  // when the compositor has used the texture to draw. This allows us to set
  // a fence with glFenceSync which we can wait on later to ensure the GPU
  // is done with the draw calls using that texture. We would like to be able
  // to simply use glFinishObjectAPPLE, but this returns earlier than
  // expected with nvidia drivers.
  virtual void MaybeFenceTexture() {}

  virtual gfx::IntSize GetSize() const = 0;

  virtual GLenum GetTextureTarget() const { return LOCAL_GL_TEXTURE_2D; }

  virtual gfx::SurfaceFormat GetFormat() const = 0;

  virtual GLenum GetWrapMode() const = 0;

  virtual gfx::Matrix4x4 GetTextureTransform() { return gfx::Matrix4x4(); }

  virtual TextureImageTextureSourceOGL* AsTextureImageTextureSource() {
    return nullptr;
  }

  virtual GLTextureSource* AsGLTextureSource() { return nullptr; }

  void SetSamplingFilter(gl::GLContext* aGL,
                         gfx::SamplingFilter aSamplingFilter) {
    if (mHasCachedSamplingFilter && mCachedSamplingFilter == aSamplingFilter) {
      return;
    }
    mHasCachedSamplingFilter = true;
    mCachedSamplingFilter = aSamplingFilter;
    ApplySamplingFilterToBoundTexture(aGL, aSamplingFilter, GetTextureTarget());
  }

  void ClearCachedFilter() { mHasCachedSamplingFilter = false; }

 private:
  gfx::SamplingFilter mCachedSamplingFilter;
  bool mHasCachedSamplingFilter;
};

/**
 * A TextureSource backed by a TextureImage.
 *
 * Depending on the underlying TextureImage, may support texture tiling, so
 * make sure to check AsBigImageIterator() and use the texture accordingly.
 *
 * This TextureSource can be used without a TextureHost and manage it's own
 * GL texture(s).
 */
<<<<<<< HEAD
class TextureImageTextureSourceOGL final : public DataTextureSource,
                                           public TextureSourceOGL,
                                           public BigImageIterator {
 public:
  explicit TextureImageTextureSourceOGL(
      CompositorOGL* aCompositor, TextureFlags aFlags = TextureFlags::DEFAULT)
      : mGL(aCompositor->gl()), mFlags(aFlags), mIterating(false) {}

  virtual const char* Name() const override {
    return "TextureImageTextureSourceOGL";
  }
||||||| merged common ancestors
class TextureImageTextureSourceOGL final : public DataTextureSource
                                         , public TextureSourceOGL
                                         , public BigImageIterator
{
public:
  explicit TextureImageTextureSourceOGL(CompositorOGL *aCompositor,
                                        TextureFlags aFlags = TextureFlags::DEFAULT)
    : mGL(aCompositor->gl())
    , mFlags(aFlags)
    , mIterating(false)
  {}

  virtual const char* Name() const override { return "TextureImageTextureSourceOGL"; }
=======
class TextureImageTextureSourceOGL final : public DataTextureSource,
                                           public TextureSourceOGL,
                                           public BigImageIterator {
 public:
  explicit TextureImageTextureSourceOGL(
      CompositorOGL* aCompositor, TextureFlags aFlags = TextureFlags::DEFAULT);

  const char* Name() const override { return "TextureImageTextureSourceOGL"; }
>>>>>>> upstream-releases
  // DataTextureSource

  bool Update(gfx::DataSourceSurface* aSurface,
              nsIntRegion* aDestRegion = nullptr,
              gfx::IntPoint* aSrcOffset = nullptr) override;

  void EnsureBuffer(const gfx::IntSize& aSize, gfxContentType aContentType);

<<<<<<< HEAD
  virtual TextureImageTextureSourceOGL* AsTextureImageTextureSource() override {
    return this;
  }
||||||| merged common ancestors
  virtual TextureImageTextureSourceOGL* AsTextureImageTextureSource() override { return this; }
=======
  TextureImageTextureSourceOGL* AsTextureImageTextureSource() override {
    return this;
  }
>>>>>>> upstream-releases

  // TextureSource

<<<<<<< HEAD
  virtual void DeallocateDeviceData() override {
    mTexImage = nullptr;
    SetUpdateSerial(0);
  }
||||||| merged common ancestors
  virtual void DeallocateDeviceData() override
  {
    mTexImage = nullptr;
    SetUpdateSerial(0);
  }
=======
  void DeallocateDeviceData() override;
>>>>>>> upstream-releases

  TextureSourceOGL* AsSourceOGL() override { return this; }

  void BindTexture(GLenum aTextureUnit,
                   gfx::SamplingFilter aSamplingFilter) override;

  gfx::IntSize GetSize() const override;

  gfx::SurfaceFormat GetFormat() const override;

  bool IsValid() const override { return !!mTexImage; }

<<<<<<< HEAD
  virtual void SetTextureSourceProvider(
      TextureSourceProvider* aProvider) override;
||||||| merged common ancestors
  virtual void SetTextureSourceProvider(TextureSourceProvider* aProvider) override;
=======
  void SetTextureSourceProvider(TextureSourceProvider* aProvider) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual GLenum GetWrapMode() const override {
    return mTexImage->GetWrapMode();
  }
||||||| merged common ancestors
  virtual GLenum GetWrapMode() const override
  {
    return mTexImage->GetWrapMode();
  }
=======
  GLenum GetWrapMode() const override { return mTexImage->GetWrapMode(); }
>>>>>>> upstream-releases

  // BigImageIterator

  BigImageIterator* AsBigImageIterator() override { return this; }

<<<<<<< HEAD
  virtual void BeginBigImageIteration() override {
||||||| merged common ancestors
  virtual void BeginBigImageIteration() override
  {
=======
  void BeginBigImageIteration() override {
>>>>>>> upstream-releases
    mTexImage->BeginBigImageIteration();
    mIterating = true;
  }

<<<<<<< HEAD
  virtual void EndBigImageIteration() override { mIterating = false; }
||||||| merged common ancestors
  virtual void EndBigImageIteration() override
  {
    mIterating = false;
  }
=======
  void EndBigImageIteration() override { mIterating = false; }
>>>>>>> upstream-releases

  gfx::IntRect GetTileRect() override;

<<<<<<< HEAD
  virtual size_t GetTileCount() override { return mTexImage->GetTileCount(); }
||||||| merged common ancestors
  virtual size_t GetTileCount() override
  {
    return mTexImage->GetTileCount();
  }
=======
  size_t GetTileCount() override { return mTexImage->GetTileCount(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool NextTile() override { return mTexImage->NextTile(); }
||||||| merged common ancestors
  virtual bool NextTile() override
  {
    return mTexImage->NextTile();
  }
=======
  bool NextTile() override { return mTexImage->NextTile(); }

 protected:
  ~TextureImageTextureSourceOGL();
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
||||||| merged common ancestors
protected:
=======
>>>>>>> upstream-releases
  RefPtr<gl::TextureImage> mTexImage;
  RefPtr<gl::GLContext> mGL;
  RefPtr<CompositorOGL> mCompositor;
  TextureFlags mFlags;
  bool mIterating;
};

/**
 * A texture source for GL textures.
 *
 * It does not own any GL texture, and attaches its shared handle to one of
 * the compositor's temporary textures when binding.
 *
 * The shared texture handle is owned by the TextureHost.
 */
class GLTextureSource : public DataTextureSource, public TextureSourceOGL {
 public:
  GLTextureSource(TextureSourceProvider* aProvider, GLuint aTextureHandle,
                  GLenum aTarget, gfx::IntSize aSize,
                  gfx::SurfaceFormat aFormat);

  virtual ~GLTextureSource();

  const char* Name() const override { return "GLTextureSource"; }

  GLTextureSource* AsGLTextureSource() override { return this; }

  TextureSourceOGL* AsSourceOGL() override { return this; }

  void BindTexture(GLenum activetex,
                   gfx::SamplingFilter aSamplingFilter) override;

  bool IsValid() const override;

  gfx::IntSize GetSize() const override { return mSize; }

  gfx::SurfaceFormat GetFormat() const override { return mFormat; }

  GLenum GetTextureTarget() const override { return mTextureTarget; }

  GLenum GetWrapMode() const override { return LOCAL_GL_CLAMP_TO_EDGE; }

  void DeallocateDeviceData() override;

<<<<<<< HEAD
  virtual void SetTextureSourceProvider(
      TextureSourceProvider* aProvider) override;
||||||| merged common ancestors
  virtual void SetTextureSourceProvider(TextureSourceProvider* aProvider) override;
=======
  void SetTextureSourceProvider(TextureSourceProvider* aProvider) override;
>>>>>>> upstream-releases

  void SetSize(gfx::IntSize aSize) { mSize = aSize; }

  void SetFormat(gfx::SurfaceFormat aFormat) { mFormat = aFormat; }

  GLuint GetTextureHandle() const { return mTextureHandle; }

  gl::GLContext* gl() const { return mGL; }

<<<<<<< HEAD
  virtual bool Update(gfx::DataSourceSurface* aSurface,
                      nsIntRegion* aDestRegion = nullptr,
                      gfx::IntPoint* aSrcOffset = nullptr) override {
||||||| merged common ancestors
  virtual bool Update(gfx::DataSourceSurface* aSurface,
                      nsIntRegion* aDestRegion = nullptr,
                      gfx::IntPoint* aSrcOffset = nullptr) override
  {
=======
  bool Update(gfx::DataSourceSurface* aSurface,
              nsIntRegion* aDestRegion = nullptr,
              gfx::IntPoint* aSrcOffset = nullptr) override {
>>>>>>> upstream-releases
    return false;
  }

 protected:
  void DeleteTextureHandle();

  RefPtr<gl::GLContext> mGL;
  RefPtr<CompositorOGL> mCompositor;
  GLuint mTextureHandle;
  GLenum mTextureTarget;
  gfx::IntSize mSize;
  gfx::SurfaceFormat mFormat;
};

// This texture source try to wrap "aSurface" in ctor for compositor direct
// access. Since we can't know the timing for gpu buffer access, the surface
// should be alive until the ~ClientStorageTextureSource(). And if we try to
// update the surface we mapped before, we need to call Sync() to make sure
// the surface is not used by compositor.
class DirectMapTextureSource : public GLTextureSource {
 public:
  DirectMapTextureSource(TextureSourceProvider* aProvider,
                         gfx::DataSourceSurface* aSurface);
  ~DirectMapTextureSource();

  bool Update(gfx::DataSourceSurface* aSurface,
              nsIntRegion* aDestRegion = nullptr,
              gfx::IntPoint* aSrcOffset = nullptr) override;

  bool IsDirectMap() override { return true; }

  // If aBlocking is false, check if this texture is no longer being used
  // by the GPU - if aBlocking is true, this will block until the GPU is
  // done with it.
  bool Sync(bool aBlocking) override;

  void MaybeFenceTexture() override;

 private:
  bool UpdateInternal(gfx::DataSourceSurface* aSurface,
                      nsIntRegion* aDestRegion, gfx::IntPoint* aSrcOffset,
                      bool aInit);

  GLsync mSync;
};

class GLTextureHost : public TextureHost {
 public:
  GLTextureHost(TextureFlags aFlags, GLuint aTextureHandle, GLenum aTarget,
                GLsync aSync, gfx::IntSize aSize, bool aHasAlpha);

  virtual ~GLTextureHost();

  // We don't own anything.
  void DeallocateDeviceData() override {}

<<<<<<< HEAD
  virtual void SetTextureSourceProvider(
      TextureSourceProvider* aProvider) override;
||||||| merged common ancestors
  virtual void SetTextureSourceProvider(TextureSourceProvider* aProvider) override;
=======
  void SetTextureSourceProvider(TextureSourceProvider* aProvider) override;
>>>>>>> upstream-releases

  bool Lock() override;

  void Unlock() override {}

  gfx::SurfaceFormat GetFormat() const override;

<<<<<<< HEAD
  virtual bool BindTextureSource(
      CompositableTextureSourceRef& aTexture) override {
||||||| merged common ancestors
  virtual bool BindTextureSource(CompositableTextureSourceRef& aTexture) override
  {
=======
  bool BindTextureSource(CompositableTextureSourceRef& aTexture) override {
>>>>>>> upstream-releases
    aTexture = mTextureSource;
    return !!aTexture;
  }

<<<<<<< HEAD
  virtual already_AddRefed<gfx::DataSourceSurface> GetAsSurface() override {
    return nullptr;  // XXX - implement this (for MOZ_DUMP_PAINTING)
||||||| merged common ancestors
  virtual already_AddRefed<gfx::DataSourceSurface> GetAsSurface() override
  {
    return nullptr; // XXX - implement this (for MOZ_DUMP_PAINTING)
=======
  already_AddRefed<gfx::DataSourceSurface> GetAsSurface() override {
    return nullptr;  // XXX - implement this (for MOZ_DUMP_PAINTING)
>>>>>>> upstream-releases
  }

  gl::GLContext* gl() const;

  gfx::IntSize GetSize() const override { return mSize; }

  const char* Name() override { return "GLTextureHost"; }

 protected:
  const GLuint mTexture;
  const GLenum mTarget;
  GLsync mSync;
  const gfx::IntSize mSize;
  const bool mHasAlpha;
  RefPtr<GLTextureSource> mTextureSource;
};

////////////////////////////////////////////////////////////////////////
// SurfaceTexture

#ifdef MOZ_WIDGET_ANDROID

class SurfaceTextureSource : public TextureSource, public TextureSourceOGL {
 public:
  SurfaceTextureSource(TextureSourceProvider* aProvider,
                       java::GeckoSurfaceTexture::Ref& aSurfTex,
                       gfx::SurfaceFormat aFormat, GLenum aTarget,
                       GLenum aWrapMode, gfx::IntSize aSize,
                       bool aIgnoreTransform);

  const char* Name() const override { return "SurfaceTextureSource"; }

  TextureSourceOGL* AsSourceOGL() override { return this; }

  void BindTexture(GLenum activetex,
                   gfx::SamplingFilter aSamplingFilter) override;

  bool IsValid() const override;

  gfx::IntSize GetSize() const override { return mSize; }

  gfx::SurfaceFormat GetFormat() const override { return mFormat; }

  gfx::Matrix4x4 GetTextureTransform() override;

  GLenum GetTextureTarget() const override { return mTextureTarget; }

  GLenum GetWrapMode() const override { return mWrapMode; }

  void DeallocateDeviceData() override;

<<<<<<< HEAD
  virtual void SetTextureSourceProvider(
      TextureSourceProvider* aProvider) override;
||||||| merged common ancestors
  virtual void SetTextureSourceProvider(TextureSourceProvider* aProvider) override;
=======
  void SetTextureSourceProvider(TextureSourceProvider* aProvider) override;
>>>>>>> upstream-releases

  gl::GLContext* gl() const { return mGL; }

 protected:
  RefPtr<gl::GLContext> mGL;
  mozilla::java::GeckoSurfaceTexture::GlobalRef mSurfTex;
  const gfx::SurfaceFormat mFormat;
  const GLenum mTextureTarget;
  const GLenum mWrapMode;
  const gfx::IntSize mSize;
  const bool mIgnoreTransform;
};

class SurfaceTextureHost : public TextureHost {
 public:
  SurfaceTextureHost(TextureFlags aFlags,
                     mozilla::java::GeckoSurfaceTexture::Ref& aSurfTex,
                     gfx::IntSize aSize, gfx::SurfaceFormat aFormat,
                     bool aContinuousUpdate, bool aIgnoreTransform);

  virtual ~SurfaceTextureHost();

<<<<<<< HEAD
  virtual void PrepareTextureSource(
      CompositableTextureSourceRef& aTexture) override;
||||||| merged common ancestors
  virtual void PrepareTextureSource(CompositableTextureSourceRef& aTexture) override;
=======
  void PrepareTextureSource(CompositableTextureSourceRef& aTexture) override;
>>>>>>> upstream-releases

  void DeallocateDeviceData() override;

<<<<<<< HEAD
  virtual void SetTextureSourceProvider(
      TextureSourceProvider* aProvider) override;
||||||| merged common ancestors
  virtual void SetTextureSourceProvider(TextureSourceProvider* aProvider) override;
=======
  void SetTextureSourceProvider(TextureSourceProvider* aProvider) override;
>>>>>>> upstream-releases

  bool Lock() override;

  gfx::SurfaceFormat GetFormat() const override;

  void NotifyNotUsed() override;

<<<<<<< HEAD
  virtual bool BindTextureSource(
      CompositableTextureSourceRef& aTexture) override {
||||||| merged common ancestors
  virtual bool BindTextureSource(CompositableTextureSourceRef& aTexture) override
  {
=======
  bool BindTextureSource(CompositableTextureSourceRef& aTexture) override {
>>>>>>> upstream-releases
    aTexture = mTextureSource;
    return !!aTexture;
  }

<<<<<<< HEAD
  virtual already_AddRefed<gfx::DataSourceSurface> GetAsSurface() override {
    return nullptr;  // XXX - implement this (for MOZ_DUMP_PAINTING)
||||||| merged common ancestors
  virtual already_AddRefed<gfx::DataSourceSurface> GetAsSurface() override
  {
    return nullptr; // XXX - implement this (for MOZ_DUMP_PAINTING)
=======
  already_AddRefed<gfx::DataSourceSurface> GetAsSurface() override {
    return nullptr;  // XXX - implement this (for MOZ_DUMP_PAINTING)
>>>>>>> upstream-releases
  }

  gl::GLContext* gl() const;

  gfx::IntSize GetSize() const override { return mSize; }

  const char* Name() override { return "SurfaceTextureHost"; }

  SurfaceTextureHost* AsSurfaceTextureHost() override { return this; }

  void CreateRenderTexture(
      const wr::ExternalImageId& aExternalImageId) override;

  void PushResourceUpdates(wr::TransactionBuilder& aResources,
                           ResourceUpdateOp aOp,
                           const Range<wr::ImageKey>& aImageKeys,
                           const wr::ExternalImageId& aExtID) override;

  void PushDisplayItems(wr::DisplayListBuilder& aBuilder,
                        const wr::LayoutRect& aBounds,
                        const wr::LayoutRect& aClip, wr::ImageRendering aFilter,
                        const Range<wr::ImageKey>& aImageKeys) override;

<<<<<<< HEAD
  virtual void CreateRenderTexture(
      const wr::ExternalImageId& aExternalImageId) override;

  virtual void PushResourceUpdates(wr::TransactionBuilder& aResources,
                                   ResourceUpdateOp aOp,
                                   const Range<wr::ImageKey>& aImageKeys,
                                   const wr::ExternalImageId& aExtID) override;

  virtual void PushDisplayItems(wr::DisplayListBuilder& aBuilder,
                                const wr::LayoutRect& aBounds,
                                const wr::LayoutRect& aClip,
                                wr::ImageRendering aFilter,
                                const Range<wr::ImageKey>& aImageKeys) override;

 protected:
||||||| merged common ancestors
protected:
=======
  bool SupportsWrNativeTexture() override { return true; }

 protected:
>>>>>>> upstream-releases
  bool EnsureAttached();

  mozilla::java::GeckoSurfaceTexture::GlobalRef mSurfTex;
  const gfx::IntSize mSize;
  const gfx::SurfaceFormat mFormat;
  bool mContinuousUpdate;
  const bool mIgnoreTransform;
  RefPtr<CompositorOGL> mCompositor;
  RefPtr<SurfaceTextureSource> mTextureSource;
};

#endif  // MOZ_WIDGET_ANDROID

////////////////////////////////////////////////////////////////////////
// EGLImage

class EGLImageTextureSource : public TextureSource, public TextureSourceOGL {
 public:
  EGLImageTextureSource(TextureSourceProvider* aProvider, EGLImage aImage,
                        gfx::SurfaceFormat aFormat, GLenum aTarget,
                        GLenum aWrapMode, gfx::IntSize aSize);

  const char* Name() const override { return "EGLImageTextureSource"; }

  TextureSourceOGL* AsSourceOGL() override { return this; }

  void BindTexture(GLenum activetex,
                   gfx::SamplingFilter aSamplingFilter) override;

  bool IsValid() const override;

  gfx::IntSize GetSize() const override { return mSize; }

  gfx::SurfaceFormat GetFormat() const override { return mFormat; }

  gfx::Matrix4x4 GetTextureTransform() override;

  GLenum GetTextureTarget() const override { return mTextureTarget; }

  GLenum GetWrapMode() const override { return mWrapMode; }

  // We don't own anything.
  void DeallocateDeviceData() override {}

<<<<<<< HEAD
  virtual void SetTextureSourceProvider(
      TextureSourceProvider* aProvider) override;
||||||| merged common ancestors
  virtual void SetTextureSourceProvider(TextureSourceProvider* aProvider) override;
=======
  void SetTextureSourceProvider(TextureSourceProvider* aProvider) override;
>>>>>>> upstream-releases

  gl::GLContext* gl() const { return mGL; }

 protected:
  RefPtr<gl::GLContext> mGL;
  RefPtr<CompositorOGL> mCompositor;
  const EGLImage mImage;
  const gfx::SurfaceFormat mFormat;
  const GLenum mTextureTarget;
  const GLenum mWrapMode;
  const gfx::IntSize mSize;
};

class EGLImageTextureHost final : public TextureHost {
 public:
  EGLImageTextureHost(TextureFlags aFlags, EGLImage aImage, EGLSync aSync,
                      gfx::IntSize aSize, bool hasAlpha);

  virtual ~EGLImageTextureHost();

  // We don't own anything.
  void DeallocateDeviceData() override {}

  void SetTextureSourceProvider(TextureSourceProvider* aProvider) override;

  bool Lock() override;

  void Unlock() override;

  gfx::SurfaceFormat GetFormat() const override;

<<<<<<< HEAD
  virtual bool BindTextureSource(
      CompositableTextureSourceRef& aTexture) override {
||||||| merged common ancestors
  virtual bool BindTextureSource(CompositableTextureSourceRef& aTexture) override
  {
=======
  bool BindTextureSource(CompositableTextureSourceRef& aTexture) override {
>>>>>>> upstream-releases
    aTexture = mTextureSource;
    return !!aTexture;
  }

<<<<<<< HEAD
  virtual already_AddRefed<gfx::DataSourceSurface> GetAsSurface() override {
    return nullptr;  // XXX - implement this (for MOZ_DUMP_PAINTING)
||||||| merged common ancestors
  virtual already_AddRefed<gfx::DataSourceSurface> GetAsSurface() override
  {
    return nullptr; // XXX - implement this (for MOZ_DUMP_PAINTING)
=======
  already_AddRefed<gfx::DataSourceSurface> GetAsSurface() override {
    return nullptr;  // XXX - implement this (for MOZ_DUMP_PAINTING)
>>>>>>> upstream-releases
  }

  gl::GLContext* gl() const;

  gfx::IntSize GetSize() const override { return mSize; }

  const char* Name() override { return "EGLImageTextureHost"; }

  void CreateRenderTexture(
      const wr::ExternalImageId& aExternalImageId) override;

  void PushResourceUpdates(wr::TransactionBuilder& aResources,
                           ResourceUpdateOp aOp,
                           const Range<wr::ImageKey>& aImageKeys,
                           const wr::ExternalImageId& aExtID) override;

  void PushDisplayItems(wr::DisplayListBuilder& aBuilder,
                        const wr::LayoutRect& aBounds,
                        const wr::LayoutRect& aClip, wr::ImageRendering aFilter,
                        const Range<wr::ImageKey>& aImageKeys) override;

 protected:
  const EGLImage mImage;
  const EGLSync mSync;
  const gfx::IntSize mSize;
  const bool mHasAlpha;
  RefPtr<EGLImageTextureSource> mTextureSource;
};

}  // namespace layers
}  // namespace mozilla

#endif /* MOZILLA_GFX_TEXTUREOGL_H */
