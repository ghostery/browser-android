/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GLCONTEXTEGL_H_
#define GLCONTEXTEGL_H_

#include "GLContext.h"
#include "GLLibraryEGL.h"

class gfxASurface;
namespace mozilla {
namespace widget {
class CompositorWidget;
}  // namespace widget
namespace gl {

class GLContextEGL : public GLContext {
  friend class TextureImageEGL;

<<<<<<< HEAD
  static already_AddRefed<GLContextEGL> CreateGLContext(
      CreateContextFlags flags, const SurfaceCaps& caps, bool isOffscreen,
      EGLConfig config, EGLSurface surface, nsACString* const out_failureId);
||||||| merged common ancestors
    static already_AddRefed<GLContextEGL>
    CreateGLContext(CreateContextFlags flags,
                    const SurfaceCaps& caps,
                    bool isOffscreen,
                    EGLConfig config,
                    EGLSurface surface,
                    nsACString* const out_failureId);
=======
  static already_AddRefed<GLContextEGL> CreateGLContext(
      GLLibraryEGL*, CreateContextFlags flags, const SurfaceCaps& caps,
      bool isOffscreen, EGLConfig config, EGLSurface surface,
      nsACString* const out_failureId);
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(GLContextEGL, override)
  GLContextEGL(CreateContextFlags flags, const SurfaceCaps& caps,
               bool isOffscreen, EGLConfig config, EGLSurface surface,
               EGLContext context);
||||||| merged common ancestors
public:
    MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(GLContextEGL, override)
    GLContextEGL(CreateContextFlags flags,
                 const SurfaceCaps& caps,
                 bool isOffscreen,
                 EGLConfig config,
                 EGLSurface surface,
                 EGLContext context);
=======
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(GLContextEGL, override)
  GLContextEGL(GLLibraryEGL*, CreateContextFlags flags, const SurfaceCaps& caps,
               bool isOffscreen, EGLConfig config, EGLSurface surface,
               EGLContext context);
>>>>>>> upstream-releases

  ~GLContextEGL();

  virtual GLContextType GetContextType() const override {
    return GLContextType::EGL;
  }

  static GLContextEGL* Cast(GLContext* gl) {
    MOZ_ASSERT(gl->GetContextType() == GLContextType::EGL);
    return static_cast<GLContextEGL*>(gl);
  }

  bool Init() override;

  virtual bool IsDoubleBuffered() const override { return mIsDoubleBuffered; }

  void SetIsDoubleBuffered(bool aIsDB) { mIsDoubleBuffered = aIsDB; }

<<<<<<< HEAD
  virtual bool IsANGLE() const override {
    return GLLibraryEGL::Get()->IsANGLE();
  }
||||||| merged common ancestors
    virtual bool IsANGLE() const override {
        return GLLibraryEGL::Get()->IsANGLE();
    }
=======
  virtual bool IsANGLE() const override { return mEgl->IsANGLE(); }
  virtual bool IsWARP() const override { return mEgl->IsWARP(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool IsWARP() const override { return GLLibraryEGL::Get()->IsWARP(); }
||||||| merged common ancestors
    virtual bool IsWARP() const override {
        return GLLibraryEGL::Get()->IsWARP();
    }
=======
  virtual bool BindTexImage() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool BindTexImage() override;
||||||| merged common ancestors
    virtual bool BindTexImage() override;
=======
  virtual bool ReleaseTexImage() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool ReleaseTexImage() override;
||||||| merged common ancestors
    virtual bool ReleaseTexImage() override;
=======
  void SetEGLSurfaceOverride(EGLSurface surf);
  EGLSurface GetEGLSurfaceOverride() { return mSurfaceOverride; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetEGLSurfaceOverride(EGLSurface surf);
  EGLSurface GetEGLSurfaceOverride() { return mSurfaceOverride; }
||||||| merged common ancestors
    void SetEGLSurfaceOverride(EGLSurface surf);
    EGLSurface GetEGLSurfaceOverride() {
        return mSurfaceOverride;
    }
=======
  virtual bool MakeCurrentImpl() const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool MakeCurrentImpl() const override;
||||||| merged common ancestors
    virtual bool MakeCurrentImpl() const override;
=======
  virtual bool IsCurrentImpl() const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool IsCurrentImpl() const override;
||||||| merged common ancestors
    virtual bool IsCurrentImpl() const override;
=======
  virtual bool RenewSurface(widget::CompositorWidget* aWidget) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool RenewSurface(widget::CompositorWidget* aWidget) override;
||||||| merged common ancestors
    virtual bool RenewSurface(widget::CompositorWidget* aWidget) override;
=======
  virtual void ReleaseSurface() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void ReleaseSurface() override;
||||||| merged common ancestors
    virtual void ReleaseSurface() override;
=======
  Maybe<SymbolLoader> GetSymbolLoader() const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool SetupLookupFunction() override;
||||||| merged common ancestors
    virtual bool SetupLookupFunction() override;
=======
  virtual bool SwapBuffers() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool SwapBuffers() override;
||||||| merged common ancestors
    virtual bool SwapBuffers() override;
=======
  virtual void GetWSIInfo(nsCString* const out) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void GetWSIInfo(nsCString* const out) const override;
||||||| merged common ancestors
    virtual void GetWSIInfo(nsCString* const out) const override;
=======
  // hold a reference to the given surface
  // for the lifetime of this context.
  void HoldSurface(gfxASurface* aSurf);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // hold a reference to the given surface
  // for the lifetime of this context.
  void HoldSurface(gfxASurface* aSurf);
||||||| merged common ancestors
    // hold a reference to the given surface
    // for the lifetime of this context.
    void HoldSurface(gfxASurface* aSurf);
=======
  EGLSurface GetEGLSurface() const { return mSurface; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  EGLSurface GetEGLSurface() const { return mSurface; }
||||||| merged common ancestors
    EGLSurface GetEGLSurface() const {
        return mSurface;
    }
=======
  bool BindTex2DOffscreen(GLContext* aOffscreen);
  void UnbindTex2DOffscreen(GLContext* aOffscreen);
  void BindOffscreenFramebuffer();
>>>>>>> upstream-releases

<<<<<<< HEAD
  EGLDisplay GetEGLDisplay() const { return GLLibraryEGL::Get()->Display(); }
||||||| merged common ancestors
    EGLDisplay GetEGLDisplay() const {
        return GLLibraryEGL::Get()->Display();
    }
=======
  void Destroy();
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool BindTex2DOffscreen(GLContext* aOffscreen);
  void UnbindTex2DOffscreen(GLContext* aOffscreen);
  void BindOffscreenFramebuffer();
||||||| merged common ancestors
    bool BindTex2DOffscreen(GLContext* aOffscreen);
    void UnbindTex2DOffscreen(GLContext* aOffscreen);
    void BindOffscreenFramebuffer();
=======
  static already_AddRefed<GLContextEGL> CreateEGLPBufferOffscreenContext(
      CreateContextFlags flags, const gfx::IntSize& size,
      const SurfaceCaps& minCaps, nsACString* const out_FailureId);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Destroy();

  static already_AddRefed<GLContextEGL> CreateEGLPBufferOffscreenContext(
      CreateContextFlags flags, const gfx::IntSize& size,
      const SurfaceCaps& minCaps, nsACString* const out_FailureId);

 protected:
  friend class GLContextProviderEGL;
  friend class GLContextEGLFactory;

  virtual void OnMarkDestroyed() override;

 public:
  const EGLConfig mConfig;

 protected:
  const RefPtr<GLLibraryEGL> mEgl;
  EGLSurface mSurface;
  const EGLSurface mFallbackSurface;

 public:
  const EGLContext mContext;

 protected:
  EGLSurface mSurfaceOverride = EGL_NO_SURFACE;
  RefPtr<gfxASurface> mThebesSurface;
  bool mBound = false;

  bool mIsPBuffer = false;
  bool mIsDoubleBuffered = false;
  bool mCanBindToTexture = false;
  bool mShareWithEGLImage = false;
  bool mOwnsContext = true;

  static EGLSurface CreatePBufferSurfaceTryingPowerOfTwo(
      EGLConfig config, EGLenum bindToTextureFormat, gfx::IntSize& pbsize);
||||||| merged common ancestors
    void Destroy();

    static already_AddRefed<GLContextEGL>
    CreateEGLPBufferOffscreenContext(CreateContextFlags flags,
                                     const gfx::IntSize& size,
                                     const SurfaceCaps& minCaps,
                                     nsACString* const out_FailureId);

protected:
    friend class GLContextProviderEGL;
    friend class GLContextEGLFactory;

public:
    const EGLConfig mConfig;
protected:
    const RefPtr<GLLibraryEGL> mEgl;
    EGLSurface mSurface;
    const EGLSurface mFallbackSurface;
public:
    const EGLContext mContext;
protected:
    EGLSurface mSurfaceOverride = EGL_NO_SURFACE;
    RefPtr<gfxASurface> mThebesSurface;
    bool mBound = false;

    bool mIsPBuffer = false;
    bool mIsDoubleBuffered = false;
    bool mCanBindToTexture = false;
    bool mShareWithEGLImage = false;
    bool mOwnsContext = true;

    static EGLSurface CreatePBufferSurfaceTryingPowerOfTwo(EGLConfig config,
                                                           EGLenum bindToTextureFormat,
                                                           gfx::IntSize& pbsize);
=======
#if defined(MOZ_WAYLAND) || defined(MOZ_WIDGET_ANDROID)
  static EGLSurface CreateEGLSurfaceForCompositorWidget(
      widget::CompositorWidget* aCompositorWidget, const EGLConfig aConfig);
#endif
 protected:
  friend class GLContextProviderEGL;
  friend class GLContextEGLFactory;

  virtual void OnMarkDestroyed() override;

 public:
  const RefPtr<GLLibraryEGL> mEgl;
  const EGLConfig mConfig;
  const EGLContext mContext;

 protected:
  EGLSurface mSurface;
  const EGLSurface mFallbackSurface;

  EGLSurface mSurfaceOverride = EGL_NO_SURFACE;
  RefPtr<gfxASurface> mThebesSurface;
  bool mBound = false;

  bool mIsPBuffer = false;
  bool mIsDoubleBuffered = false;
  bool mCanBindToTexture = false;
  bool mShareWithEGLImage = false;
  bool mOwnsContext = true;

  static EGLSurface CreatePBufferSurfaceTryingPowerOfTwo(
      GLLibraryEGL*, EGLConfig config, EGLenum bindToTextureFormat,
      gfx::IntSize& pbsize);
>>>>>>> upstream-releases
#if defined(MOZ_WAYLAND)
<<<<<<< HEAD
  static EGLSurface CreateWaylandBufferSurface(EGLConfig config,
                                               gfx::IntSize& pbsize);
||||||| merged common ancestors
    static EGLSurface CreateWaylandBufferSurface(EGLConfig config,
                                                 gfx::IntSize& pbsize);
=======
  static EGLSurface CreateWaylandBufferSurface(GLLibraryEGL*, EGLConfig config,
                                               gfx::IntSize& pbsize);
>>>>>>> upstream-releases
#endif
#if defined(MOZ_WIDGET_ANDROID)
 public:
  EGLSurface CreateCompatibleSurface(void* aWindow);
#endif  // defined(MOZ_WIDGET_ANDROID)
};

bool CreateConfig(GLLibraryEGL*, EGLConfig* config, int32_t depth,
                  bool enableDepthBuffer);

}  // namespace gl
}  // namespace mozilla

#endif  // GLCONTEXTEGL_H_
