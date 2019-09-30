/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; tab-width: 4; -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GLScreenBuffer.h"

#include <cstring>
#include "CompositorTypes.h"
#include "mozilla/StaticPrefs.h"
#include "GLContext.h"
#include "GLBlitHelper.h"
#include "GLReadTexImageHelper.h"
#include "SharedSurfaceEGL.h"
#include "SharedSurfaceGL.h"
#include "ScopedGLHelpers.h"
#include "gfx2DGlue.h"
#include "../layers/ipc/ShadowLayers.h"
#include "mozilla/layers/TextureForwarder.h"
#include "mozilla/layers/TextureClientSharedSurface.h"

#ifdef XP_WIN
#  include "SharedSurfaceANGLE.h"         // for SurfaceFactory_ANGLEShareHandle
#  include "SharedSurfaceD3D11Interop.h"  // for SurfaceFactory_D3D11Interop
#  include "mozilla/gfx/DeviceManagerDx.h"
#endif

#ifdef XP_MACOSX
#  include "SharedSurfaceIO.h"
#endif

#ifdef MOZ_X11
#  include "GLXLibrary.h"
#  include "SharedSurfaceGLX.h"
#endif

namespace mozilla {
namespace gl {

using gfx::SurfaceFormat;

<<<<<<< HEAD
UniquePtr<GLScreenBuffer> GLScreenBuffer::Create(GLContext* gl,
                                                 const gfx::IntSize& size,
                                                 const SurfaceCaps& caps) {
  UniquePtr<GLScreenBuffer> ret;
  if (caps.antialias && !gl->IsSupported(GLFeature::framebuffer_multisample)) {
    return ret;
  }

  layers::TextureFlags flags = layers::TextureFlags::ORIGIN_BOTTOM_LEFT;
  if (!caps.premultAlpha) {
    flags |= layers::TextureFlags::NON_PREMULTIPLIED;
  }

  UniquePtr<SurfaceFactory> factory =
      MakeUnique<SurfaceFactory_Basic>(gl, caps, flags);

  ret.reset(new GLScreenBuffer(gl, caps, std::move(factory)));
  return ret;
}

/* static */ UniquePtr<SurfaceFactory> GLScreenBuffer::CreateFactory(
    GLContext* gl, const SurfaceCaps& caps,
    KnowsCompositor* compositorConnection, const layers::TextureFlags& flags) {
  LayersIPCChannel* ipcChannel = compositorConnection->GetTextureForwarder();
  const layers::LayersBackend backend =
      compositorConnection->GetCompositorBackendType();
  const bool useANGLE = compositorConnection->GetCompositorUseANGLE();

  const bool useGl =
      !gfxPrefs::WebGLForceLayersReadback() &&
      (backend == layers::LayersBackend::LAYERS_OPENGL ||
       (backend == layers::LayersBackend::LAYERS_WR && !useANGLE));
  const bool useD3D =
      !gfxPrefs::WebGLForceLayersReadback() &&
      (backend == layers::LayersBackend::LAYERS_D3D11 ||
       (backend == layers::LayersBackend::LAYERS_WR && useANGLE));

  UniquePtr<SurfaceFactory> factory = nullptr;
  if (useGl) {
||||||| merged common ancestors
UniquePtr<GLScreenBuffer>
GLScreenBuffer::Create(GLContext* gl,
                       const gfx::IntSize& size,
                       const SurfaceCaps& caps)
{
    UniquePtr<GLScreenBuffer> ret;
    if (caps.antialias &&
        !gl->IsSupported(GLFeature::framebuffer_multisample))
    {
        return ret;
    }

    layers::TextureFlags flags = layers::TextureFlags::ORIGIN_BOTTOM_LEFT;
    if (!caps.premultAlpha) {
        flags |= layers::TextureFlags::NON_PREMULTIPLIED;
    }

    UniquePtr<SurfaceFactory> factory = MakeUnique<SurfaceFactory_Basic>(gl, caps, flags);

    ret.reset( new GLScreenBuffer(gl, caps, std::move(factory)) );
    return ret;
}

/* static */ UniquePtr<SurfaceFactory>
GLScreenBuffer::CreateFactory(GLContext* gl,
                              const SurfaceCaps& caps,
                              KnowsCompositor* compositorConnection,
                              const layers::TextureFlags& flags)
{
    LayersIPCChannel* ipcChannel = compositorConnection->GetTextureForwarder();
    const layers::LayersBackend backend = compositorConnection->GetCompositorBackendType();
    const bool useANGLE = compositorConnection->GetCompositorUseANGLE();

    const bool useGl = !gfxPrefs::WebGLForceLayersReadback() &&
                       (backend == layers::LayersBackend::LAYERS_OPENGL ||
                       (backend == layers::LayersBackend::LAYERS_WR && !useANGLE));
    const bool useD3D = !gfxPrefs::WebGLForceLayersReadback() &&
                        (backend == layers::LayersBackend::LAYERS_D3D11 ||
                        (backend == layers::LayersBackend::LAYERS_WR && useANGLE));

    UniquePtr<SurfaceFactory> factory = nullptr;
    if (useGl) {
=======
UniquePtr<GLScreenBuffer> GLScreenBuffer::Create(GLContext* gl,
                                                 const gfx::IntSize& size,
                                                 const SurfaceCaps& caps) {
  UniquePtr<GLScreenBuffer> ret;

  layers::TextureFlags flags = layers::TextureFlags::ORIGIN_BOTTOM_LEFT;
  if (!caps.premultAlpha) {
    flags |= layers::TextureFlags::NON_PREMULTIPLIED;
  }

  UniquePtr<SurfaceFactory> factory =
      MakeUnique<SurfaceFactory_Basic>(gl, caps, flags);

  ret.reset(new GLScreenBuffer(gl, caps, std::move(factory)));
  return ret;
}

/* static */
UniquePtr<SurfaceFactory> GLScreenBuffer::CreateFactory(
    GLContext* gl, const SurfaceCaps& caps,
    KnowsCompositor* compositorConnection, const layers::TextureFlags& flags) {
  LayersIPCChannel* ipcChannel = compositorConnection->GetTextureForwarder();
  const layers::LayersBackend backend =
      compositorConnection->GetCompositorBackendType();
  const bool useANGLE = compositorConnection->GetCompositorUseANGLE();

  const bool useGl =
      !StaticPrefs::webgl_force_layers_readback() &&
      (backend == layers::LayersBackend::LAYERS_OPENGL ||
       (backend == layers::LayersBackend::LAYERS_WR && !useANGLE));
  const bool useD3D =
      !StaticPrefs::webgl_force_layers_readback() &&
      (backend == layers::LayersBackend::LAYERS_D3D11 ||
       (backend == layers::LayersBackend::LAYERS_WR && useANGLE));

  UniquePtr<SurfaceFactory> factory = nullptr;
  if (useGl) {
>>>>>>> upstream-releases
#if defined(XP_MACOSX)
    factory = SurfaceFactory_IOSurface::Create(gl, caps, ipcChannel, flags);
#elif defined(MOZ_X11)
    if (sGLXLibrary.UseTextureFromPixmap())
      factory = SurfaceFactory_GLXDrawable::Create(gl, caps, ipcChannel, flags);
#elif defined(MOZ_WIDGET_UIKIT)
    factory = MakeUnique<SurfaceFactory_GLTexture>(mGLContext, caps, ipcChannel,
                                                   mFlags);
#elif defined(MOZ_WIDGET_ANDROID)
<<<<<<< HEAD
    if (XRE_IsParentProcess() && !gfxPrefs::WebGLSurfaceTextureEnabled()) {
      factory = SurfaceFactory_EGLImage::Create(gl, caps, ipcChannel, flags);
    } else {
      factory =
          SurfaceFactory_SurfaceTexture::Create(gl, caps, ipcChannel, flags);
    }
||||||| merged common ancestors
        if (XRE_IsParentProcess() && !gfxPrefs::WebGLSurfaceTextureEnabled()) {
            factory = SurfaceFactory_EGLImage::Create(gl, caps, ipcChannel, flags);
        } else {
            factory = SurfaceFactory_SurfaceTexture::Create(gl, caps, ipcChannel, flags);
        }
=======
    if (XRE_IsParentProcess() && !StaticPrefs::webgl_enable_surface_texture()) {
      factory = SurfaceFactory_EGLImage::Create(gl, caps, ipcChannel, flags);
    } else {
      factory =
          SurfaceFactory_SurfaceTexture::Create(gl, caps, ipcChannel, flags);
    }
>>>>>>> upstream-releases
#else
    if (gl->GetContextType() == GLContextType::EGL) {
      if (XRE_IsParentProcess()) {
        factory = SurfaceFactory_EGLImage::Create(gl, caps, ipcChannel, flags);
      }
    }
#endif
  } else if (useD3D) {
#ifdef XP_WIN
<<<<<<< HEAD
    // Enable surface sharing only if ANGLE and compositing devices
    // are both WARP or both not WARP
    gfx::DeviceManagerDx* dm = gfx::DeviceManagerDx::Get();
    if (gl->IsANGLE() && (gl->IsWARP() == dm->IsWARP()) &&
        dm->TextureSharingWorks()) {
      factory =
          SurfaceFactory_ANGLEShareHandle::Create(gl, caps, ipcChannel, flags);
||||||| merged common ancestors
        // Enable surface sharing only if ANGLE and compositing devices
        // are both WARP or both not WARP
        gfx::DeviceManagerDx* dm = gfx::DeviceManagerDx::Get();
        if (gl->IsANGLE() &&
            (gl->IsWARP() == dm->IsWARP()) &&
             dm->TextureSharingWorks())
        {
            factory = SurfaceFactory_ANGLEShareHandle::Create(gl, caps, ipcChannel, flags);
        }

        if (!factory && gfxPrefs::WebGLDXGLEnabled()) {
            factory = SurfaceFactory_D3D11Interop::Create(gl, caps, ipcChannel, flags);
        }
#endif
=======
    // Ensure devices initialization. SharedSurfaceANGLE and
    // SharedSurfaceD3D11Interop use them. The devices are lazily initialized
    // with WebRender to reduce memory usage.
    gfxPlatform::GetPlatform()->EnsureDevicesInitialized();

    // Enable surface sharing only if ANGLE and compositing devices
    // are both WARP or both not WARP
    gfx::DeviceManagerDx* dm = gfx::DeviceManagerDx::Get();
    if (gl->IsANGLE() && (gl->IsWARP() == dm->IsWARP()) &&
        dm->TextureSharingWorks()) {
      factory =
          SurfaceFactory_ANGLEShareHandle::Create(gl, caps, ipcChannel, flags);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (!factory && gfxPrefs::WebGLDXGLEnabled()) {
      factory =
          SurfaceFactory_D3D11Interop::Create(gl, caps, ipcChannel, flags);
||||||| merged common ancestors
#ifdef MOZ_X11
    if (!factory && sGLXLibrary.UseTextureFromPixmap()) {
        factory = SurfaceFactory_GLXDrawable::Create(gl, caps, ipcChannel, flags);
=======
    if (!factory && StaticPrefs::webgl_dxgl_enabled()) {
      factory =
          SurfaceFactory_D3D11Interop::Create(gl, caps, ipcChannel, flags);
>>>>>>> upstream-releases
    }
#endif
<<<<<<< HEAD
  }

#ifdef MOZ_X11
  if (!factory && sGLXLibrary.UseTextureFromPixmap()) {
    factory = SurfaceFactory_GLXDrawable::Create(gl, caps, ipcChannel, flags);
  }
#endif
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return factory;
||||||| merged common ancestors
    return factory;
=======
#ifdef MOZ_X11
  if (!factory && sGLXLibrary.UseTextureFromPixmap()) {
    factory = SurfaceFactory_GLXDrawable::Create(gl, caps, ipcChannel, flags);
  }
#endif

  return factory;
>>>>>>> upstream-releases
}

GLScreenBuffer::GLScreenBuffer(GLContext* gl, const SurfaceCaps& caps,
                               UniquePtr<SurfaceFactory> factory)
    : mGL(gl),
      mCaps(caps),
      mFactory(std::move(factory)),
      mNeedsBlit(true),
      mUserReadBufferMode(LOCAL_GL_BACK),
      mUserDrawBufferMode(LOCAL_GL_BACK),
      mUserDrawFB(0),
      mUserReadFB(0),
      mInternalDrawFB(0),
      mInternalReadFB(0)
#ifdef DEBUG
      ,
      mInInternalMode_DrawFB(true),
      mInInternalMode_ReadFB(true)
#endif
{
<<<<<<< HEAD
}

GLScreenBuffer::~GLScreenBuffer() {
  mFactory = nullptr;
  mDraw = nullptr;
  mRead = nullptr;
||||||| merged common ancestors
    mFactory = nullptr;
    mDraw = nullptr;
    mRead = nullptr;
=======
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!mBack) return;
||||||| merged common ancestors
    if (!mBack)
        return;
=======
GLScreenBuffer::~GLScreenBuffer() {
  mFactory = nullptr;
  mRead = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Detach mBack cleanly.
  mBack->Surf()->ProducerRelease();
||||||| merged common ancestors
    // Detach mBack cleanly.
    mBack->Surf()->ProducerRelease();
=======
  if (!mBack) return;

  // Detach mBack cleanly.
  mBack->Surf()->ProducerRelease();
>>>>>>> upstream-releases
}

void GLScreenBuffer::BindAsFramebuffer(GLContext* const gl,
                                       GLenum target) const {
  GLuint drawFB = DrawFB();
  GLuint readFB = ReadFB();

  if (!gl->IsSupported(GLFeature::split_framebuffer)) {
    MOZ_ASSERT(drawFB == readFB);
    gl->raw_fBindFramebuffer(target, readFB);
    return;
  }

  switch (target) {
    case LOCAL_GL_FRAMEBUFFER:
      gl->raw_fBindFramebuffer(LOCAL_GL_DRAW_FRAMEBUFFER_EXT, drawFB);
      gl->raw_fBindFramebuffer(LOCAL_GL_READ_FRAMEBUFFER_EXT, readFB);
      break;

    case LOCAL_GL_DRAW_FRAMEBUFFER_EXT:
      gl->raw_fBindFramebuffer(LOCAL_GL_DRAW_FRAMEBUFFER_EXT, drawFB);
      break;

    case LOCAL_GL_READ_FRAMEBUFFER_EXT:
      gl->raw_fBindFramebuffer(LOCAL_GL_READ_FRAMEBUFFER_EXT, readFB);
      break;

    default:
      MOZ_CRASH("GFX: Bad `target` for BindFramebuffer.");
  }
}

void GLScreenBuffer::BindFB(GLuint fb) {
  GLuint drawFB = DrawFB();
  GLuint readFB = ReadFB();

  mUserDrawFB = fb;
  mUserReadFB = fb;
  mInternalDrawFB = (fb == 0) ? drawFB : fb;
  mInternalReadFB = (fb == 0) ? readFB : fb;

  if (mInternalDrawFB == mInternalReadFB) {
    mGL->raw_fBindFramebuffer(LOCAL_GL_FRAMEBUFFER, mInternalDrawFB);
  } else {
    MOZ_ASSERT(mGL->IsSupported(GLFeature::split_framebuffer));
    mGL->raw_fBindFramebuffer(LOCAL_GL_DRAW_FRAMEBUFFER_EXT, mInternalDrawFB);
    mGL->raw_fBindFramebuffer(LOCAL_GL_READ_FRAMEBUFFER_EXT, mInternalReadFB);
  }

#ifdef DEBUG
  mInInternalMode_DrawFB = false;
  mInInternalMode_ReadFB = false;
#endif
}

void GLScreenBuffer::BindDrawFB(GLuint fb) {
  MOZ_ASSERT(mGL->IsSupported(GLFeature::split_framebuffer));

  GLuint drawFB = DrawFB();
  mUserDrawFB = fb;
  mInternalDrawFB = (fb == 0) ? drawFB : fb;

  mGL->raw_fBindFramebuffer(LOCAL_GL_DRAW_FRAMEBUFFER_EXT, mInternalDrawFB);

#ifdef DEBUG
  mInInternalMode_DrawFB = false;
#endif
}

void GLScreenBuffer::BindReadFB(GLuint fb) {
  MOZ_ASSERT(mGL->IsSupported(GLFeature::split_framebuffer));

  GLuint readFB = ReadFB();
  mUserReadFB = fb;
  mInternalReadFB = (fb == 0) ? readFB : fb;

  mGL->raw_fBindFramebuffer(LOCAL_GL_READ_FRAMEBUFFER_EXT, mInternalReadFB);

#ifdef DEBUG
  mInInternalMode_ReadFB = false;
#endif
}

void GLScreenBuffer::BindFB_Internal(GLuint fb) {
  mInternalDrawFB = mUserDrawFB = fb;
  mInternalReadFB = mUserReadFB = fb;
  mGL->raw_fBindFramebuffer(LOCAL_GL_FRAMEBUFFER, mInternalDrawFB);

#ifdef DEBUG
  mInInternalMode_DrawFB = true;
  mInInternalMode_ReadFB = true;
#endif
}

void GLScreenBuffer::BindDrawFB_Internal(GLuint fb) {
  MOZ_ASSERT(mGL->IsSupported(GLFeature::split_framebuffer));

  mInternalDrawFB = mUserDrawFB = fb;
  mGL->raw_fBindFramebuffer(LOCAL_GL_DRAW_FRAMEBUFFER_EXT, mInternalDrawFB);

#ifdef DEBUG
  mInInternalMode_DrawFB = true;
#endif
}

void GLScreenBuffer::BindReadFB_Internal(GLuint fb) {
  MOZ_ASSERT(mGL->IsSupported(GLFeature::split_framebuffer));

  mInternalReadFB = mUserReadFB = fb;
  mGL->raw_fBindFramebuffer(LOCAL_GL_READ_FRAMEBUFFER_EXT, mInternalReadFB);

#ifdef DEBUG
  mInInternalMode_ReadFB = true;
#endif
}

GLuint GLScreenBuffer::GetDrawFB() const {
#ifdef DEBUG
  MOZ_ASSERT(!mInInternalMode_DrawFB);

  // Don't need a branch here, because:
  // LOCAL_GL_DRAW_FRAMEBUFFER_BINDING_EXT == LOCAL_GL_FRAMEBUFFER_BINDING ==
  // 0x8CA6 We use raw_ here because this is debug code and we need to see what
  // the driver thinks.
  GLuint actual = 0;
  mGL->raw_fGetIntegerv(LOCAL_GL_DRAW_FRAMEBUFFER_BINDING_EXT, (GLint*)&actual);

  GLuint predicted = mInternalDrawFB;
  if (predicted != actual && !mGL->CheckContextLost()) {
    printf_stderr("Misprediction: Bound draw FB predicted: %d. Was: %d.\n",
                  predicted, actual);
    MOZ_ASSERT(false, "Draw FB binding misprediction!");
  }
#endif

  return mUserDrawFB;
}

GLuint GLScreenBuffer::GetReadFB() const {
#ifdef DEBUG
  MOZ_ASSERT(!mInInternalMode_ReadFB);

  // We use raw_ here because this is debug code and we need to see what
  // the driver thinks.
  GLuint actual = 0;
  if (mGL->IsSupported(GLFeature::split_framebuffer))
    mGL->raw_fGetIntegerv(LOCAL_GL_READ_FRAMEBUFFER_BINDING_EXT,
                          (GLint*)&actual);
  else
    mGL->raw_fGetIntegerv(LOCAL_GL_FRAMEBUFFER_BINDING, (GLint*)&actual);

  GLuint predicted = mInternalReadFB;
  if (predicted != actual && !mGL->CheckContextLost()) {
    printf_stderr("Misprediction: Bound read FB predicted: %d. Was: %d.\n",
                  predicted, actual);
    MOZ_ASSERT(false, "Read FB binding misprediction!");
  }
#endif

  return mUserReadFB;
}

GLuint GLScreenBuffer::GetFB() const {
  MOZ_ASSERT(GetDrawFB() == GetReadFB());
  return GetDrawFB();
}

void GLScreenBuffer::DeletingFB(GLuint fb) {
  if (fb == mInternalDrawFB) {
    mInternalDrawFB = 0;
    mUserDrawFB = 0;
  }
  if (fb == mInternalReadFB) {
    mInternalReadFB = 0;
    mUserReadFB = 0;
  }
}

void GLScreenBuffer::AfterDrawCall() {
  if (mUserDrawFB != 0) return;

<<<<<<< HEAD
  RequireBlit();
}

void GLScreenBuffer::BeforeReadCall() {
  if (mUserReadFB != 0) return;

  AssureBlitted();
||||||| merged common ancestors
void
GLScreenBuffer::AfterDrawCall()
{
    if (mUserDrawFB != 0)
        return;

    RequireBlit();
=======
  RequireBlit();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool GLScreenBuffer::CopyTexImage2D(GLenum target, GLint level,
                                    GLenum internalformat, GLint x, GLint y,
                                    GLsizei width, GLsizei height,
                                    GLint border) {
  SharedSurface* surf;
  if (GetReadFB() == 0) {
    surf = SharedSurf();
  } else {
    surf = mGL->mFBOMapping[GetReadFB()];
  }
  if (surf) {
    return surf->CopyTexImage2D(target, level, internalformat, x, y, width,
                                height, border);
  }
||||||| merged common ancestors
void
GLScreenBuffer::BeforeReadCall()
{
    if (mUserReadFB != 0)
        return;
=======
void GLScreenBuffer::BeforeReadCall() {
  if (mUserReadFB != 0) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return false;
||||||| merged common ancestors
    AssureBlitted();
=======
  AssureBlitted();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool GLScreenBuffer::ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height,
                                GLenum format, GLenum type, GLvoid* pixels) {
  // If the currently bound framebuffer is backed by a SharedSurface
  // then it might want to override how we read pixel data from it.
  // This is normally only the default framebuffer, but we can also
  // have SharedSurfaces bound to other framebuffers when doing
  // readback for BasicLayers.
  SharedSurface* surf;
  if (GetReadFB() == 0) {
    surf = SharedSurf();
  } else {
    surf = mGL->mFBOMapping[GetReadFB()];
  }
  if (surf) {
    return surf->ReadPixels(x, y, width, height, format, type, pixels);
  }
||||||| merged common ancestors
bool
GLScreenBuffer::CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x,
                               GLint y, GLsizei width, GLsizei height, GLint border)
{
    SharedSurface* surf;
    if (GetReadFB() == 0) {
        surf = SharedSurf();
    } else {
        surf = mGL->mFBOMapping[GetReadFB()];
    }
    if (surf) {
        return surf->CopyTexImage2D(target, level, internalformat,  x, y, width, height, border);
    }
=======
bool GLScreenBuffer::CopyTexImage2D(GLenum target, GLint level,
                                    GLenum internalformat, GLint x, GLint y,
                                    GLsizei width, GLsizei height,
                                    GLint border) {
  SharedSurface* surf;
  if (GetReadFB() == 0) {
    surf = SharedSurf();
  } else {
    surf = mGL->mFBOMapping[GetReadFB()];
  }
  if (surf) {
    return surf->CopyTexImage2D(target, level, internalformat, x, y, width,
                                height, border);
  }
>>>>>>> upstream-releases

  return false;
}

<<<<<<< HEAD
void GLScreenBuffer::RequireBlit() { mNeedsBlit = true; }

void GLScreenBuffer::AssureBlitted() {
  if (!mNeedsBlit) return;

  if (mDraw) {
    GLuint drawFB = DrawFB();
    GLuint readFB = ReadFB();

    MOZ_ASSERT(drawFB != 0);
    MOZ_ASSERT(drawFB != readFB);
    MOZ_ASSERT(mGL->IsSupported(GLFeature::split_framebuffer));
    MOZ_ASSERT(mDraw->mSize == mRead->Size());

    ScopedBindFramebuffer boundFB(mGL);
    ScopedGLState scissor(mGL, LOCAL_GL_SCISSOR_TEST, false);

    BindReadFB_Internal(drawFB);
    BindDrawFB_Internal(readFB);

    if (mGL->IsSupported(GLFeature::framebuffer_blit)) {
      const gfx::IntSize& srcSize = mDraw->mSize;
      const gfx::IntSize& destSize = mRead->Size();

      mGL->raw_fBlitFramebuffer(0, 0, srcSize.width, srcSize.height, 0, 0,
                                destSize.width, destSize.height,
                                LOCAL_GL_COLOR_BUFFER_BIT, LOCAL_GL_NEAREST);
    } else if (mGL->IsExtensionSupported(
                   GLContext::APPLE_framebuffer_multisample)) {
      mGL->fResolveMultisampleFramebufferAPPLE();
    } else {
      MOZ_CRASH("GFX: No available blit methods.");
    }
    // Done!
  }
||||||| merged common ancestors
bool
GLScreenBuffer::ReadPixels(GLint x, GLint y,
                           GLsizei width, GLsizei height,
                           GLenum format, GLenum type,
                           GLvoid* pixels)
{
    // If the currently bound framebuffer is backed by a SharedSurface
    // then it might want to override how we read pixel data from it.
    // This is normally only the default framebuffer, but we can also
    // have SharedSurfaces bound to other framebuffers when doing
    // readback for BasicLayers.
    SharedSurface* surf;
    if (GetReadFB() == 0) {
        surf = SharedSurf();
    } else {
        surf = mGL->mFBOMapping[GetReadFB()];
    }
    if (surf) {
        return surf->ReadPixels(x, y, width, height, format, type, pixels);
    }
=======
bool GLScreenBuffer::ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height,
                                GLenum format, GLenum type, GLvoid* pixels) {
  // If the currently bound framebuffer is backed by a SharedSurface
  // then it might want to override how we read pixel data from it.
  // This is normally only the default framebuffer, but we can also
  // have SharedSurfaces bound to other framebuffers when doing
  // readback for BasicLayers.
  SharedSurface* surf;
  if (GetReadFB() == 0) {
    surf = SharedSurf();
  } else {
    surf = mGL->mFBOMapping[GetReadFB()];
  }
  if (surf) {
    return surf->ReadPixels(x, y, width, height, format, type, pixels);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  mNeedsBlit = false;
||||||| merged common ancestors
    return false;
=======
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void GLScreenBuffer::Morph(UniquePtr<SurfaceFactory> newFactory) {
  MOZ_RELEASE_ASSERT(newFactory, "newFactory must not be null");
  mFactory = std::move(newFactory);
}
||||||| merged common ancestors
void
GLScreenBuffer::RequireBlit()
{
    mNeedsBlit = true;
}
=======
void GLScreenBuffer::RequireBlit() { mNeedsBlit = true; }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool GLScreenBuffer::Attach(SharedSurface* surf, const gfx::IntSize& size) {
  ScopedBindFramebuffer autoFB(mGL);
||||||| merged common ancestors
void
GLScreenBuffer::AssureBlitted()
{
    if (!mNeedsBlit)
        return;

    if (mDraw) {
        GLuint drawFB = DrawFB();
        GLuint readFB = ReadFB();

        MOZ_ASSERT(drawFB != 0);
        MOZ_ASSERT(drawFB != readFB);
        MOZ_ASSERT(mGL->IsSupported(GLFeature::split_framebuffer));
        MOZ_ASSERT(mDraw->mSize == mRead->Size());

        ScopedBindFramebuffer boundFB(mGL);
        ScopedGLState scissor(mGL, LOCAL_GL_SCISSOR_TEST, false);

        BindReadFB_Internal(drawFB);
        BindDrawFB_Internal(readFB);

        if (mGL->IsSupported(GLFeature::framebuffer_blit)) {
            const gfx::IntSize&  srcSize = mDraw->mSize;
            const gfx::IntSize& destSize = mRead->Size();

            mGL->raw_fBlitFramebuffer(0, 0,  srcSize.width,  srcSize.height,
                                      0, 0, destSize.width, destSize.height,
                                      LOCAL_GL_COLOR_BUFFER_BIT,
                                      LOCAL_GL_NEAREST);
        } else if (mGL->IsExtensionSupported(GLContext::APPLE_framebuffer_multisample)) {
            mGL->fResolveMultisampleFramebufferAPPLE();
        } else {
            MOZ_CRASH("GFX: No available blit methods.");
        }
        // Done!
    }
=======
void GLScreenBuffer::AssureBlitted() {
  if (!mNeedsBlit) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  const bool readNeedsUnlock = (mRead && SharedSurf());
  if (readNeedsUnlock) {
    SharedSurf()->UnlockProd();
  }
||||||| merged common ancestors
    mNeedsBlit = false;
}
=======
  mNeedsBlit = false;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  surf->LockProd();
||||||| merged common ancestors
void
GLScreenBuffer::Morph(UniquePtr<SurfaceFactory> newFactory)
{
    MOZ_RELEASE_ASSERT(newFactory, "newFactory must not be null");
    mFactory = std::move(newFactory);
}
=======
void GLScreenBuffer::Morph(UniquePtr<SurfaceFactory> newFactory) {
  MOZ_RELEASE_ASSERT(newFactory, "newFactory must not be null");
  mFactory = std::move(newFactory);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mRead && surf->mAttachType == SharedSurf()->mAttachType &&
      size == Size()) {
    // Same size, same type, ready for reuse!
    mRead->Attach(surf);
  } else {
    // Else something changed, so resize:
    UniquePtr<DrawBuffer> draw;
    bool drawOk = true;
||||||| merged common ancestors
bool
GLScreenBuffer::Attach(SharedSurface* surf, const gfx::IntSize& size)
{
    ScopedBindFramebuffer autoFB(mGL);
=======
bool GLScreenBuffer::Attach(SharedSurface* surf, const gfx::IntSize& size) {
  ScopedBindFramebuffer autoFB(mGL);
>>>>>>> upstream-releases

<<<<<<< HEAD
    /* Don't change out the draw buffer unless we resize. In the
     * preserveDrawingBuffer:true case, prior contents of the buffer must
     * be retained. If we're using a draw buffer, it's an MSAA buffer, so
     * even if we copy the previous frame into the (single-sampled) read
     * buffer, if we need to re-resolve from draw to read (as triggered by
     * drawing), we'll need the old MSAA content to still be in the draw
     * buffer.
     */
    if (!mDraw || size != Size())
      drawOk = CreateDraw(size, &draw);  // Can be null.
||||||| merged common ancestors
    const bool readNeedsUnlock = (mRead && SharedSurf());
    if (readNeedsUnlock) {
        SharedSurf()->UnlockProd();
    }
=======
  const bool readNeedsUnlock = (mRead && SharedSurf());
  if (readNeedsUnlock) {
    SharedSurf()->UnlockProd();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    UniquePtr<ReadBuffer> read = CreateRead(surf);
    bool readOk = !!read;
||||||| merged common ancestors
    surf->LockProd();
=======
  surf->LockProd();
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!drawOk || !readOk) {
      surf->UnlockProd();
      if (readNeedsUnlock) {
        SharedSurf()->LockProd();
      }
      return false;
||||||| merged common ancestors
    if (mRead &&
        surf->mAttachType == SharedSurf()->mAttachType &&
        size == Size())
    {
        // Same size, same type, ready for reuse!
        mRead->Attach(surf);
    } else {
        // Else something changed, so resize:
        UniquePtr<DrawBuffer> draw;
        bool drawOk = true;

        /* Don't change out the draw buffer unless we resize. In the
         * preserveDrawingBuffer:true case, prior contents of the buffer must
         * be retained. If we're using a draw buffer, it's an MSAA buffer, so
         * even if we copy the previous frame into the (single-sampled) read
         * buffer, if we need to re-resolve from draw to read (as triggered by
         * drawing), we'll need the old MSAA content to still be in the draw
         * buffer.
         */
        if (!mDraw || size != Size())
            drawOk = CreateDraw(size, &draw);  // Can be null.

        UniquePtr<ReadBuffer> read = CreateRead(surf);
        bool readOk = !!read;

        if (!drawOk || !readOk) {
            surf->UnlockProd();
            if (readNeedsUnlock) {
                SharedSurf()->LockProd();
            }
            return false;
        }

        if (draw)
            mDraw = std::move(draw);

        mRead = std::move(read);
=======
  if (mRead && surf->mAttachType == SharedSurf()->mAttachType &&
      size == Size()) {
    // Same size, same type, ready for reuse!
    mRead->Attach(surf);
  } else {
    UniquePtr<ReadBuffer> read = CreateRead(surf);
    if (!read) {
      surf->UnlockProd();
      if (readNeedsUnlock) {
        SharedSurf()->LockProd();
      }
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (draw) mDraw = std::move(draw);
||||||| merged common ancestors
    // Check that we're all set up.
    MOZ_ASSERT(SharedSurf() == surf);
=======
    mRead = std::move(read);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    mRead = std::move(read);
  }
||||||| merged common ancestors
    // Update the ReadBuffer mode.
    if (mGL->IsSupported(gl::GLFeature::read_buffer)) {
        BindFB(0);
        mRead->SetReadBuffer(mUserReadBufferMode);
    }
=======
  // Check that we're all set up.
  MOZ_ASSERT(SharedSurf() == surf);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Check that we're all set up.
  MOZ_ASSERT(SharedSurf() == surf);
||||||| merged common ancestors
    // Update the DrawBuffer mode.
    if (mGL->IsSupported(gl::GLFeature::draw_buffers)) {
        BindFB(0);
        SetDrawBuffer(mUserDrawBufferMode);
    }
=======
  // Update the ReadBuffer mode.
  if (mGL->IsSupported(gl::GLFeature::read_buffer)) {
    BindFB(0);
    mRead->SetReadBuffer(mUserReadBufferMode);
  }

  // Update the DrawBuffer mode.
  if (mGL->IsSupported(gl::GLFeature::draw_buffers)) {
    BindFB(0);
    SetDrawBuffer(mUserDrawBufferMode);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Update the ReadBuffer mode.
  if (mGL->IsSupported(gl::GLFeature::read_buffer)) {
    BindFB(0);
    mRead->SetReadBuffer(mUserReadBufferMode);
  }
||||||| merged common ancestors
    RequireBlit();
=======
  RequireBlit();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Update the DrawBuffer mode.
  if (mGL->IsSupported(gl::GLFeature::draw_buffers)) {
    BindFB(0);
    SetDrawBuffer(mUserDrawBufferMode);
  }

  RequireBlit();

  return true;
||||||| merged common ancestors
    return true;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool GLScreenBuffer::Swap(const gfx::IntSize& size) {
  RefPtr<SharedSurfaceTextureClient> newBack = mFactory->NewTexClient(size);
  if (!newBack) return false;

  // In the case of DXGL interop, the new surface needs to be acquired before
  // it is attached so that the interop surface is locked, which populates
  // the GL renderbuffer. This results in the renderbuffer being ready and
  // attachment to framebuffer succeeds in Attach() call.
  newBack->Surf()->ProducerAcquire();

  if (!Attach(newBack->Surf(), size)) {
    newBack->Surf()->ProducerRelease();
    return false;
  }
  // Attach was successful.

  mFront = mBack;
  mBack = newBack;

  if (ShouldPreserveBuffer() && mFront && mBack && !mDraw) {
    auto src = mFront->Surf();
    auto dest = mBack->Surf();

    // uint32_t srcPixel = ReadPixel(src);
    // uint32_t destPixel = ReadPixel(dest);
    // printf_stderr("Before: src: 0x%08x, dest: 0x%08x\n", srcPixel,
    // destPixel);
#ifdef DEBUG
    GLContext::LocalErrorScope errorScope(*mGL);
#endif
||||||| merged common ancestors
bool
GLScreenBuffer::Swap(const gfx::IntSize& size)
{
    RefPtr<SharedSurfaceTextureClient> newBack = mFactory->NewTexClient(size);
    if (!newBack)
        return false;

    // In the case of DXGL interop, the new surface needs to be acquired before
    // it is attached so that the interop surface is locked, which populates
    // the GL renderbuffer. This results in the renderbuffer being ready and
    // attachment to framebuffer succeeds in Attach() call.
    newBack->Surf()->ProducerAcquire();

    if (!Attach(newBack->Surf(), size)) {
        newBack->Surf()->ProducerRelease();
        return false;
    }
    // Attach was successful.

    mFront = mBack;
    mBack = newBack;

    if (ShouldPreserveBuffer() &&
        mFront &&
        mBack &&
        !mDraw)
    {
        auto src  = mFront->Surf();
        auto dest = mBack->Surf();

        //uint32_t srcPixel = ReadPixel(src);
        //uint32_t destPixel = ReadPixel(dest);
        //printf_stderr("Before: src: 0x%08x, dest: 0x%08x\n", srcPixel, destPixel);
#ifdef DEBUG
        GLContext::LocalErrorScope errorScope(*mGL);
#endif
=======
bool GLScreenBuffer::Swap(const gfx::IntSize& size) {
  RefPtr<SharedSurfaceTextureClient> newBack = mFactory->NewTexClient(size);
  if (!newBack) return false;

  // In the case of DXGL interop, the new surface needs to be acquired before
  // it is attached so that the interop surface is locked, which populates
  // the GL renderbuffer. This results in the renderbuffer being ready and
  // attachment to framebuffer succeeds in Attach() call.
  newBack->Surf()->ProducerAcquire();

  if (!Attach(newBack->Surf(), size)) {
    newBack->Surf()->ProducerRelease();
    return false;
  }
  // Attach was successful.

  mFront = mBack;
  mBack = newBack;
>>>>>>> upstream-releases

<<<<<<< HEAD
    SharedSurface::ProdCopy(src, dest, mFactory.get());
||||||| merged common ancestors
        SharedSurface::ProdCopy(src, dest, mFactory.get());
=======
  if (ShouldPreserveBuffer() && mFront && mBack) {
    auto src = mFront->Surf();
    auto dest = mBack->Surf();
>>>>>>> upstream-releases

    // uint32_t srcPixel = ReadPixel(src);
    // uint32_t destPixel = ReadPixel(dest);
    // printf_stderr("Before: src: 0x%08x, dest: 0x%08x\n", srcPixel,
    // destPixel);
#ifdef DEBUG
<<<<<<< HEAD
    MOZ_ASSERT(!errorScope.GetError());
||||||| merged common ancestors
        MOZ_ASSERT(!errorScope.GetError());
=======
    GLContext::LocalErrorScope errorScope(*mGL);
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
    // srcPixel = ReadPixel(src);
    // destPixel = ReadPixel(dest);
    // printf_stderr("After: src: 0x%08x, dest: 0x%08x\n", srcPixel, destPixel);
  }
||||||| merged common ancestors
        //srcPixel = ReadPixel(src);
        //destPixel = ReadPixel(dest);
        //printf_stderr("After: src: 0x%08x, dest: 0x%08x\n", srcPixel, destPixel);
    }
=======
    if (!SharedSurface::ProdCopy(src, dest, mFactory.get())) {
      newBack->Surf()->ProducerRelease();
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // XXX: We would prefer to fence earlier on platforms that don't need
  // the full ProducerAcquire/ProducerRelease semantics, so that the fence
  // doesn't include the copy operation. Unfortunately, the current API
  // doesn't expose a good way to do that.
  if (mFront) {
    mFront->Surf()->ProducerRelease();
  }
||||||| merged common ancestors
    // XXX: We would prefer to fence earlier on platforms that don't need
    // the full ProducerAcquire/ProducerRelease semantics, so that the fence
    // doesn't include the copy operation. Unfortunately, the current API
    // doesn't expose a good way to do that.
    if (mFront) {
        mFront->Surf()->ProducerRelease();
    }
=======
#ifdef DEBUG
    MOZ_ASSERT(!errorScope.GetError());
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
    // srcPixel = ReadPixel(src);
    // destPixel = ReadPixel(dest);
    // printf_stderr("After: src: 0x%08x, dest: 0x%08x\n", srcPixel, destPixel);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool GLScreenBuffer::PublishFrame(const gfx::IntSize& size) {
  AssureBlitted();
||||||| merged common ancestors
bool
GLScreenBuffer::PublishFrame(const gfx::IntSize& size)
{
    AssureBlitted();
=======
  // XXX: We would prefer to fence earlier on platforms that don't need
  // the full ProducerAcquire/ProducerRelease semantics, so that the fence
  // doesn't include the copy operation. Unfortunately, the current API
  // doesn't expose a good way to do that.
  if (mFront) {
    mFront->Surf()->ProducerRelease();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool good = Swap(size);
  return good;
||||||| merged common ancestors
    bool good = Swap(size);
    return good;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool GLScreenBuffer::Resize(const gfx::IntSize& size) {
  RefPtr<SharedSurfaceTextureClient> newBack = mFactory->NewTexClient(size);
  if (!newBack) return false;
||||||| merged common ancestors
bool
GLScreenBuffer::Resize(const gfx::IntSize& size)
{
    RefPtr<SharedSurfaceTextureClient> newBack = mFactory->NewTexClient(size);
    if (!newBack)
        return false;
=======
bool GLScreenBuffer::PublishFrame(const gfx::IntSize& size) {
  AssureBlitted();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!Attach(newBack->Surf(), size)) return false;
||||||| merged common ancestors
    if (!Attach(newBack->Surf(), size))
        return false;
=======
  bool good = Swap(size);
  return good;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mBack) mBack->Surf()->ProducerRelease();
||||||| merged common ancestors
    if (mBack)
        mBack->Surf()->ProducerRelease();
=======
bool GLScreenBuffer::Resize(const gfx::IntSize& size) {
  RefPtr<SharedSurfaceTextureClient> newBack = mFactory->NewTexClient(size);
  if (!newBack) return false;
>>>>>>> upstream-releases

<<<<<<< HEAD
  mBack = newBack;
||||||| merged common ancestors
    mBack = newBack;
=======
  if (!Attach(newBack->Surf(), size)) return false;
>>>>>>> upstream-releases

<<<<<<< HEAD
  mBack->Surf()->ProducerAcquire();
||||||| merged common ancestors
    mBack->Surf()->ProducerAcquire();
=======
  if (mBack) mBack->Surf()->ProducerRelease();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
  mBack = newBack;
>>>>>>> upstream-releases

<<<<<<< HEAD
bool GLScreenBuffer::CreateDraw(const gfx::IntSize& size,
                                UniquePtr<DrawBuffer>* out_buffer) {
  GLContext* gl = mFactory->mGL;
  const GLFormats& formats = mFactory->mFormats;
  const SurfaceCaps& caps = mFactory->DrawCaps();
||||||| merged common ancestors
bool
GLScreenBuffer::CreateDraw(const gfx::IntSize& size,
                           UniquePtr<DrawBuffer>* out_buffer)
{
    GLContext* gl = mFactory->mGL;
    const GLFormats& formats = mFactory->mFormats;
    const SurfaceCaps& caps = mFactory->DrawCaps();
=======
  mBack->Surf()->ProducerAcquire();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return DrawBuffer::Create(gl, caps, formats, size, out_buffer);
||||||| merged common ancestors
    return DrawBuffer::Create(gl, caps, formats, size, out_buffer);
=======
  return true;
>>>>>>> upstream-releases
}

UniquePtr<ReadBuffer> GLScreenBuffer::CreateRead(SharedSurface* surf) {
  GLContext* gl = mFactory->mGL;
  const GLFormats& formats = mFactory->mFormats;
  const SurfaceCaps& caps = mFactory->ReadCaps();

  return ReadBuffer::Create(gl, caps, formats, surf);
}

void GLScreenBuffer::SetDrawBuffer(GLenum mode) {
  MOZ_ASSERT(mGL->IsSupported(gl::GLFeature::draw_buffers));
  MOZ_ASSERT(GetDrawFB() == 0);

  if (!mGL->IsSupported(GLFeature::draw_buffers)) return;

  mUserDrawBufferMode = mode;

<<<<<<< HEAD
  GLuint fb = mDraw ? mDraw->mFB : mRead->mFB;
  GLenum internalMode;
||||||| merged common ancestors
    GLuint fb = mDraw ? mDraw->mFB : mRead->mFB;
    GLenum internalMode;
=======
  GLuint fb = mRead->mFB;
  GLenum internalMode;
>>>>>>> upstream-releases

  switch (mode) {
    case LOCAL_GL_BACK:
      internalMode = (fb == 0) ? LOCAL_GL_BACK : LOCAL_GL_COLOR_ATTACHMENT0;
      break;

    case LOCAL_GL_NONE:
      internalMode = LOCAL_GL_NONE;
      break;

    default:
      MOZ_CRASH("GFX: Bad value.");
  }

  mGL->MakeCurrent();
  mGL->fDrawBuffers(1, &internalMode);
}

void GLScreenBuffer::SetReadBuffer(GLenum mode) {
  MOZ_ASSERT(mGL->IsSupported(gl::GLFeature::read_buffer));
  MOZ_ASSERT(GetReadFB() == 0);

  mUserReadBufferMode = mode;
  mRead->SetReadBuffer(mUserReadBufferMode);
}

<<<<<<< HEAD
bool GLScreenBuffer::IsDrawFramebufferDefault() const {
  if (!mDraw) return IsReadFramebufferDefault();
  return mDraw->mFB == 0;
||||||| merged common ancestors
bool
GLScreenBuffer::IsDrawFramebufferDefault() const
{
    if (!mDraw)
        return IsReadFramebufferDefault();
    return mDraw->mFB == 0;
=======
bool GLScreenBuffer::IsDrawFramebufferDefault() const {
  return IsReadFramebufferDefault();
>>>>>>> upstream-releases
}

bool GLScreenBuffer::IsReadFramebufferDefault() const {
  return SharedSurf()->mAttachType == AttachmentType::Screen;
}

uint32_t GLScreenBuffer::DepthBits() const {
  const GLFormats& formats = mFactory->mFormats;

  if (formats.depth == LOCAL_GL_DEPTH_COMPONENT16) return 16;

  return 24;
}

////////////////////////////////////////////////////////////////////////
// Utils

<<<<<<< HEAD
static void RenderbufferStorageBySamples(GLContext* aGL, GLsizei aSamples,
                                         GLenum aInternalFormat,
                                         const gfx::IntSize& aSize) {
  if (aSamples) {
    aGL->fRenderbufferStorageMultisample(LOCAL_GL_RENDERBUFFER, aSamples,
                                         aInternalFormat, aSize.width,
                                         aSize.height);
  } else {
    aGL->fRenderbufferStorage(LOCAL_GL_RENDERBUFFER, aInternalFormat,
                              aSize.width, aSize.height);
  }
}

static GLuint CreateRenderbuffer(GLContext* aGL, GLenum aFormat,
                                 GLsizei aSamples, const gfx::IntSize& aSize) {
  GLuint rb = 0;
  aGL->fGenRenderbuffers(1, &rb);
  ScopedBindRenderbuffer autoRB(aGL, rb);
||||||| merged common ancestors
static void
RenderbufferStorageBySamples(GLContext* aGL, GLsizei aSamples,
                             GLenum aInternalFormat, const gfx::IntSize& aSize)
{
    if (aSamples) {
        aGL->fRenderbufferStorageMultisample(LOCAL_GL_RENDERBUFFER,
                                             aSamples,
                                             aInternalFormat,
                                             aSize.width, aSize.height);
    } else {
        aGL->fRenderbufferStorage(LOCAL_GL_RENDERBUFFER,
                                  aInternalFormat,
                                  aSize.width, aSize.height);
    }
}

static GLuint
CreateRenderbuffer(GLContext* aGL, GLenum aFormat, GLsizei aSamples,
                   const gfx::IntSize& aSize)
{
    GLuint rb = 0;
    aGL->fGenRenderbuffers(1, &rb);
    ScopedBindRenderbuffer autoRB(aGL, rb);
=======
static void CreateRenderbuffersForOffscreen(GLContext* const aGL,
                                            const GLFormats& aFormats,
                                            const gfx::IntSize& aSize,
                                            GLuint* const aDepthRB,
                                            GLuint* const aStencilRB) {
  const auto fnCreateRenderbuffer = [&](const GLenum sizedFormat) {
    GLuint rb = 0;
    aGL->fGenRenderbuffers(1, &rb);
    ScopedBindRenderbuffer autoRB(aGL, rb);
>>>>>>> upstream-releases

<<<<<<< HEAD
  RenderbufferStorageBySamples(aGL, aSamples, aFormat, aSize);

  return rb;
}
||||||| merged common ancestors
    RenderbufferStorageBySamples(aGL, aSamples, aFormat, aSize);

    return rb;
}
=======
    aGL->fRenderbufferStorage(LOCAL_GL_RENDERBUFFER, sizedFormat, aSize.width,
                              aSize.height);
    return rb;
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
static void CreateRenderbuffersForOffscreen(
    GLContext* aGL, const GLFormats& aFormats, const gfx::IntSize& aSize,
    bool aMultisample, GLuint* aColorMSRB, GLuint* aDepthRB,
    GLuint* aStencilRB) {
  GLsizei samples = aMultisample ? aFormats.samples : 0;
  if (aColorMSRB) {
    MOZ_ASSERT(aFormats.samples > 0);
    MOZ_ASSERT(aFormats.color_rbFormat);

    GLenum colorFormat = aFormats.color_rbFormat;
    if (aGL->IsANGLE()) {
      MOZ_ASSERT(colorFormat == LOCAL_GL_RGBA8);
      colorFormat = LOCAL_GL_BGRA8_EXT;
    }
||||||| merged common ancestors
static void
CreateRenderbuffersForOffscreen(GLContext* aGL, const GLFormats& aFormats,
                                const gfx::IntSize& aSize, bool aMultisample,
                                GLuint* aColorMSRB, GLuint* aDepthRB,
                                GLuint* aStencilRB)
{
    GLsizei samples = aMultisample ? aFormats.samples : 0;
    if (aColorMSRB) {
        MOZ_ASSERT(aFormats.samples > 0);
        MOZ_ASSERT(aFormats.color_rbFormat);

        GLenum colorFormat = aFormats.color_rbFormat;
        if (aGL->IsANGLE()) {
            MOZ_ASSERT(colorFormat == LOCAL_GL_RGBA8);
            colorFormat = LOCAL_GL_BGRA8_EXT;
        }

        *aColorMSRB = CreateRenderbuffer(aGL, colorFormat, samples, aSize);
    }
=======
  if (aDepthRB && aStencilRB && aFormats.depthStencil) {
    *aDepthRB = fnCreateRenderbuffer(aFormats.depthStencil);
    *aStencilRB = *aDepthRB;
  } else {
    if (aDepthRB) {
      MOZ_ASSERT(aFormats.depth);
>>>>>>> upstream-releases

<<<<<<< HEAD
    *aColorMSRB = CreateRenderbuffer(aGL, colorFormat, samples, aSize);
  }

  if (aDepthRB && aStencilRB && aFormats.depthStencil) {
    *aDepthRB = CreateRenderbuffer(aGL, aFormats.depthStencil, samples, aSize);
    *aStencilRB = *aDepthRB;
  } else {
    if (aDepthRB) {
      MOZ_ASSERT(aFormats.depth);
||||||| merged common ancestors
    if (aDepthRB &&
        aStencilRB &&
        aFormats.depthStencil)
    {
        *aDepthRB = CreateRenderbuffer(aGL, aFormats.depthStencil, samples, aSize);
        *aStencilRB = *aDepthRB;
    } else {
        if (aDepthRB) {
            MOZ_ASSERT(aFormats.depth);

            *aDepthRB = CreateRenderbuffer(aGL, aFormats.depth, samples, aSize);
        }
=======
      *aDepthRB = fnCreateRenderbuffer(aFormats.depth);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
      *aDepthRB = CreateRenderbuffer(aGL, aFormats.depth, samples, aSize);
    }
||||||| merged common ancestors
        if (aStencilRB) {
            MOZ_ASSERT(aFormats.stencil);
=======
    if (aStencilRB) {
      MOZ_ASSERT(aFormats.stencil);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (aStencilRB) {
      MOZ_ASSERT(aFormats.stencil);

      *aStencilRB = CreateRenderbuffer(aGL, aFormats.stencil, samples, aSize);
||||||| merged common ancestors
            *aStencilRB = CreateRenderbuffer(aGL, aFormats.stencil, samples, aSize);
        }
=======
      *aStencilRB = fnCreateRenderbuffer(aFormats.stencil);
>>>>>>> upstream-releases
    }
  }
}

////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
// DrawBuffer

bool DrawBuffer::Create(GLContext* const gl, const SurfaceCaps& caps,
                        const GLFormats& formats, const gfx::IntSize& size,
                        UniquePtr<DrawBuffer>* out_buffer) {
  MOZ_ASSERT(out_buffer);
  *out_buffer = nullptr;

  if (!caps.color) {
    MOZ_ASSERT(!caps.alpha && !caps.depth && !caps.stencil);

    // Nothing is needed.
    return true;
  }

  if (caps.antialias) {
    if (formats.samples == 0) return false;  // Can't create it.
||||||| merged common ancestors
// DrawBuffer

bool
DrawBuffer::Create(GLContext* const gl,
                   const SurfaceCaps& caps,
                   const GLFormats& formats,
                   const gfx::IntSize& size,
                   UniquePtr<DrawBuffer>* out_buffer)
{
    MOZ_ASSERT(out_buffer);
    *out_buffer = nullptr;

    if (!caps.color) {
        MOZ_ASSERT(!caps.alpha && !caps.depth && !caps.stencil);

        // Nothing is needed.
        return true;
    }

    if (caps.antialias) {
        if (formats.samples == 0)
            return false; // Can't create it.
=======
// ReadBuffer
>>>>>>> upstream-releases

<<<<<<< HEAD
    MOZ_ASSERT(uint32_t(formats.samples) <= gl->MaxSamples());
  }
||||||| merged common ancestors
        MOZ_ASSERT(uint32_t(formats.samples) <= gl->MaxSamples());
    }
=======
UniquePtr<ReadBuffer> ReadBuffer::Create(GLContext* gl, const SurfaceCaps& caps,
                                         const GLFormats& formats,
                                         SharedSurface* surf) {
  MOZ_ASSERT(surf);
>>>>>>> upstream-releases

<<<<<<< HEAD
  GLuint colorMSRB = 0;
  GLuint depthRB = 0;
  GLuint stencilRB = 0;
||||||| merged common ancestors
    GLuint colorMSRB = 0;
    GLuint depthRB   = 0;
    GLuint stencilRB = 0;
=======
  if (surf->mAttachType == AttachmentType::Screen) {
    // Don't need anything. Our read buffer will be the 'screen'.
    return UniquePtr<ReadBuffer>(new ReadBuffer(gl, 0, 0, 0, surf));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  GLuint* pColorMSRB = caps.antialias ? &colorMSRB : nullptr;
  GLuint* pDepthRB = caps.depth ? &depthRB : nullptr;
  GLuint* pStencilRB = caps.stencil ? &stencilRB : nullptr;
||||||| merged common ancestors
    GLuint* pColorMSRB = caps.antialias ? &colorMSRB : nullptr;
    GLuint* pDepthRB   = caps.depth     ? &depthRB   : nullptr;
    GLuint* pStencilRB = caps.stencil   ? &stencilRB : nullptr;
=======
  GLuint depthRB = 0;
  GLuint stencilRB = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!formats.color_rbFormat) pColorMSRB = nullptr;
||||||| merged common ancestors
    if (!formats.color_rbFormat)
        pColorMSRB = nullptr;
=======
  GLuint* pDepthRB = caps.depth ? &depthRB : nullptr;
  GLuint* pStencilRB = caps.stencil ? &stencilRB : nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (pDepthRB && pStencilRB) {
    if (!formats.depth && !formats.depthStencil) pDepthRB = nullptr;
||||||| merged common ancestors
    if (pDepthRB && pStencilRB) {
        if (!formats.depth && !formats.depthStencil)
            pDepthRB = nullptr;
=======
  GLContext::LocalErrorScope localError(*gl);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!formats.stencil && !formats.depthStencil) pStencilRB = nullptr;
  } else {
    if (!formats.depth) pDepthRB = nullptr;
||||||| merged common ancestors
        if (!formats.stencil && !formats.depthStencil)
            pStencilRB = nullptr;
    } else {
        if (!formats.depth)
            pDepthRB = nullptr;
=======
  CreateRenderbuffersForOffscreen(gl, formats, surf->mSize, pDepthRB,
                                  pStencilRB);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!formats.stencil) pStencilRB = nullptr;
  }
||||||| merged common ancestors
        if (!formats.stencil)
            pStencilRB = nullptr;
    }
=======
  GLuint colorTex = 0;
  GLuint colorRB = 0;
  GLenum target = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  GLContext::LocalErrorScope localError(*gl);
||||||| merged common ancestors
    GLContext::LocalErrorScope localError(*gl);
=======
  switch (surf->mAttachType) {
    case AttachmentType::GLTexture:
      colorTex = surf->ProdTexture();
      target = surf->ProdTextureTarget();
      break;
    case AttachmentType::GLRenderbuffer:
      colorRB = surf->ProdRenderbuffer();
      break;
    default:
      MOZ_CRASH("GFX: Unknown attachment type, create?");
  }
  MOZ_ASSERT(colorTex || colorRB);
>>>>>>> upstream-releases

<<<<<<< HEAD
  CreateRenderbuffersForOffscreen(gl, formats, size, caps.antialias, pColorMSRB,
                                  pDepthRB, pStencilRB);
||||||| merged common ancestors
    CreateRenderbuffersForOffscreen(gl, formats, size, caps.antialias,
                                    pColorMSRB, pDepthRB, pStencilRB);
=======
  GLuint fb = 0;
  gl->fGenFramebuffers(1, &fb);
  gl->AttachBuffersToFB(colorTex, colorRB, depthRB, stencilRB, fb, target);
  gl->mFBOMapping[fb] = surf;
>>>>>>> upstream-releases

<<<<<<< HEAD
  GLuint fb = 0;
  gl->fGenFramebuffers(1, &fb);
  gl->AttachBuffersToFB(0, colorMSRB, depthRB, stencilRB, fb);
||||||| merged common ancestors
    GLuint fb = 0;
    gl->fGenFramebuffers(1, &fb);
    gl->AttachBuffersToFB(0, colorMSRB, depthRB, stencilRB, fb);
=======
  UniquePtr<ReadBuffer> ret(new ReadBuffer(gl, fb, depthRB, stencilRB, surf));
>>>>>>> upstream-releases

<<<<<<< HEAD
  const GLsizei samples = formats.samples;
  UniquePtr<DrawBuffer> ret(
      new DrawBuffer(gl, size, samples, fb, colorMSRB, depthRB, stencilRB));
||||||| merged common ancestors
    const GLsizei samples = formats.samples;
    UniquePtr<DrawBuffer> ret( new DrawBuffer(gl, size, samples, fb, colorMSRB,
                                              depthRB, stencilRB) );
=======
  GLenum err = localError.GetError();
  MOZ_ASSERT_IF(err != LOCAL_GL_NO_ERROR, err == LOCAL_GL_OUT_OF_MEMORY);
  if (err) return nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  GLenum err = localError.GetError();
  MOZ_ASSERT_IF(err != LOCAL_GL_NO_ERROR, err == LOCAL_GL_OUT_OF_MEMORY);
  if (err || !gl->IsFramebufferComplete(fb)) return false;
||||||| merged common ancestors
    GLenum err = localError.GetError();
    MOZ_ASSERT_IF(err != LOCAL_GL_NO_ERROR, err == LOCAL_GL_OUT_OF_MEMORY);
    if (err || !gl->IsFramebufferComplete(fb))
        return false;
=======
  const bool needsAcquire = !surf->IsProducerAcquired();
  if (needsAcquire) {
    surf->ProducerReadAcquire();
  }
  const bool isComplete = gl->IsFramebufferComplete(fb);
  if (needsAcquire) {
    surf->ProducerReadRelease();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *out_buffer = std::move(ret);
  return true;
}
||||||| merged common ancestors
    *out_buffer = std::move(ret);
    return true;
}
=======
  if (!isComplete) return nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
DrawBuffer::~DrawBuffer() {
  if (!mGL->MakeCurrent()) return;

  GLuint fb = mFB;
  GLuint rbs[] = {
      mColorMSRB, mDepthRB,
      (mStencilRB != mDepthRB) ? mStencilRB
                               : 0,  // Don't double-delete DEPTH_STENCIL RBs.
  };

  mGL->fDeleteFramebuffers(1, &fb);
  mGL->fDeleteRenderbuffers(3, rbs);
||||||| merged common ancestors
DrawBuffer::~DrawBuffer()
{
    if (!mGL->MakeCurrent())
        return;

    GLuint fb = mFB;
    GLuint rbs[] = {
        mColorMSRB,
        mDepthRB,
        (mStencilRB != mDepthRB) ? mStencilRB : 0, // Don't double-delete DEPTH_STENCIL RBs.
    };

    mGL->fDeleteFramebuffers(1, &fb);
    mGL->fDeleteRenderbuffers(3, rbs);
=======
  return ret;
>>>>>>> upstream-releases
}

ReadBuffer::~ReadBuffer() {
  if (!mGL->MakeCurrent()) return;

<<<<<<< HEAD
UniquePtr<ReadBuffer> ReadBuffer::Create(GLContext* gl, const SurfaceCaps& caps,
                                         const GLFormats& formats,
                                         SharedSurface* surf) {
  MOZ_ASSERT(surf);

  if (surf->mAttachType == AttachmentType::Screen) {
    // Don't need anything. Our read buffer will be the 'screen'.
    return UniquePtr<ReadBuffer>(new ReadBuffer(gl, 0, 0, 0, surf));
  }
||||||| merged common ancestors
UniquePtr<ReadBuffer>
ReadBuffer::Create(GLContext* gl,
                   const SurfaceCaps& caps,
                   const GLFormats& formats,
                   SharedSurface* surf)
{
    MOZ_ASSERT(surf);

    if (surf->mAttachType == AttachmentType::Screen) {
        // Don't need anything. Our read buffer will be the 'screen'.
        return UniquePtr<ReadBuffer>( new ReadBuffer(gl, 0, 0, 0,
                                                     surf) );
    }
=======
  GLuint fb = mFB;
  GLuint rbs[] = {
      mDepthRB,
      (mStencilRB != mDepthRB) ? mStencilRB
                               : 0,  // Don't double-delete DEPTH_STENCIL RBs.
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
  GLuint depthRB = 0;
  GLuint stencilRB = 0;
||||||| merged common ancestors
    GLuint depthRB = 0;
    GLuint stencilRB = 0;
=======
  mGL->fDeleteFramebuffers(1, &fb);
  mGL->fDeleteRenderbuffers(2, rbs);
>>>>>>> upstream-releases

<<<<<<< HEAD
  GLuint* pDepthRB = caps.depth ? &depthRB : nullptr;
  GLuint* pStencilRB = caps.stencil ? &stencilRB : nullptr;

  GLContext::LocalErrorScope localError(*gl);
||||||| merged common ancestors
    GLuint* pDepthRB   = caps.depth   ? &depthRB   : nullptr;
    GLuint* pStencilRB = caps.stencil ? &stencilRB : nullptr;

    GLContext::LocalErrorScope localError(*gl);
=======
  mGL->mFBOMapping.erase(mFB);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  CreateRenderbuffersForOffscreen(gl, formats, surf->mSize, caps.antialias,
                                  nullptr, pDepthRB, pStencilRB);
||||||| merged common ancestors
    CreateRenderbuffersForOffscreen(gl, formats, surf->mSize, caps.antialias,
                                    nullptr, pDepthRB, pStencilRB);
=======
void ReadBuffer::Attach(SharedSurface* surf) {
  MOZ_ASSERT(surf && mSurf);
  MOZ_ASSERT(surf->mAttachType == mSurf->mAttachType);
  MOZ_ASSERT(surf->mSize == mSurf->mSize);
>>>>>>> upstream-releases

<<<<<<< HEAD
  GLuint colorTex = 0;
  GLuint colorRB = 0;
  GLenum target = 0;
||||||| merged common ancestors
    GLuint colorTex = 0;
    GLuint colorRB = 0;
    GLenum target = 0;
=======
  // Nothing else is needed for AttachType Screen.
  if (surf->mAttachType != AttachmentType::Screen) {
    GLuint colorTex = 0;
    GLuint colorRB = 0;
    GLenum target = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  switch (surf->mAttachType) {
    case AttachmentType::GLTexture:
      colorTex = surf->ProdTexture();
      target = surf->ProdTextureTarget();
      break;
    case AttachmentType::GLRenderbuffer:
      colorRB = surf->ProdRenderbuffer();
      break;
    default:
      MOZ_CRASH("GFX: Unknown attachment type, create?");
  }
  MOZ_ASSERT(colorTex || colorRB);

  GLuint fb = 0;
  gl->fGenFramebuffers(1, &fb);
  gl->AttachBuffersToFB(colorTex, colorRB, depthRB, stencilRB, fb, target);
  gl->mFBOMapping[fb] = surf;

  UniquePtr<ReadBuffer> ret(new ReadBuffer(gl, fb, depthRB, stencilRB, surf));

  GLenum err = localError.GetError();
  MOZ_ASSERT_IF(err != LOCAL_GL_NO_ERROR, err == LOCAL_GL_OUT_OF_MEMORY);
  if (err) return nullptr;

  const bool needsAcquire = !surf->IsProducerAcquired();
  if (needsAcquire) {
    surf->ProducerReadAcquire();
  }
  const bool isComplete = gl->IsFramebufferComplete(fb);
  if (needsAcquire) {
    surf->ProducerReadRelease();
  }
||||||| merged common ancestors
    switch (surf->mAttachType) {
    case AttachmentType::GLTexture:
        colorTex = surf->ProdTexture();
        target = surf->ProdTextureTarget();
        break;
    case AttachmentType::GLRenderbuffer:
        colorRB = surf->ProdRenderbuffer();
        break;
    default:
        MOZ_CRASH("GFX: Unknown attachment type, create?");
    }
    MOZ_ASSERT(colorTex || colorRB);

    GLuint fb = 0;
    gl->fGenFramebuffers(1, &fb);
    gl->AttachBuffersToFB(colorTex, colorRB, depthRB, stencilRB, fb, target);
    gl->mFBOMapping[fb] = surf;

    UniquePtr<ReadBuffer> ret( new ReadBuffer(gl, fb, depthRB,
                                              stencilRB, surf) );

    GLenum err = localError.GetError();
    MOZ_ASSERT_IF(err != LOCAL_GL_NO_ERROR, err == LOCAL_GL_OUT_OF_MEMORY);
    if (err)
        return nullptr;

    const bool needsAcquire = !surf->IsProducerAcquired();
    if (needsAcquire) {
        surf->ProducerReadAcquire();
    }
    const bool isComplete = gl->IsFramebufferComplete(fb);
    if (needsAcquire) {
        surf->ProducerReadRelease();
    }
=======
    switch (surf->mAttachType) {
      case AttachmentType::GLTexture:
        colorTex = surf->ProdTexture();
        target = surf->ProdTextureTarget();
        break;
      case AttachmentType::GLRenderbuffer:
        colorRB = surf->ProdRenderbuffer();
        break;
      default:
        MOZ_CRASH("GFX: Unknown attachment type, attach?");
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!isComplete) return nullptr;
||||||| merged common ancestors
    if (!isComplete)
        return nullptr;
=======
    mGL->AttachBuffersToFB(colorTex, colorRB, 0, 0, mFB, target);
    mGL->mFBOMapping[mFB] = surf;
    MOZ_GL_ASSERT(mGL, mGL->IsFramebufferComplete(mFB));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return ret;
||||||| merged common ancestors
    return ret;
=======
  mSurf = surf;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
ReadBuffer::~ReadBuffer() {
  if (!mGL->MakeCurrent()) return;
||||||| merged common ancestors
ReadBuffer::~ReadBuffer()
{
    if (!mGL->MakeCurrent())
        return;
=======
const gfx::IntSize& ReadBuffer::Size() const { return mSurf->mSize; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  GLuint fb = mFB;
  GLuint rbs[] = {
      mDepthRB,
      (mStencilRB != mDepthRB) ? mStencilRB
                               : 0,  // Don't double-delete DEPTH_STENCIL RBs.
  };
||||||| merged common ancestors
    GLuint fb = mFB;
    GLuint rbs[] = {
        mDepthRB,
        (mStencilRB != mDepthRB) ? mStencilRB : 0, // Don't double-delete DEPTH_STENCIL RBs.
    };
=======
void ReadBuffer::SetReadBuffer(GLenum userMode) const {
  if (!mGL->IsSupported(GLFeature::read_buffer)) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  mGL->fDeleteFramebuffers(1, &fb);
  mGL->fDeleteRenderbuffers(2, rbs);
||||||| merged common ancestors
    mGL->fDeleteFramebuffers(1, &fb);
    mGL->fDeleteRenderbuffers(2, rbs);
=======
  GLenum internalMode;
>>>>>>> upstream-releases

<<<<<<< HEAD
  mGL->mFBOMapping.erase(mFB);
}

void ReadBuffer::Attach(SharedSurface* surf) {
  MOZ_ASSERT(surf && mSurf);
  MOZ_ASSERT(surf->mAttachType == mSurf->mAttachType);
  MOZ_ASSERT(surf->mSize == mSurf->mSize);

  // Nothing else is needed for AttachType Screen.
  if (surf->mAttachType != AttachmentType::Screen) {
    GLuint colorTex = 0;
    GLuint colorRB = 0;
    GLenum target = 0;

    switch (surf->mAttachType) {
      case AttachmentType::GLTexture:
        colorTex = surf->ProdTexture();
        target = surf->ProdTextureTarget();
        break;
      case AttachmentType::GLRenderbuffer:
        colorRB = surf->ProdRenderbuffer();
        break;
      default:
        MOZ_CRASH("GFX: Unknown attachment type, attach?");
    }

    mGL->AttachBuffersToFB(colorTex, colorRB, 0, 0, mFB, target);
    mGL->mFBOMapping[mFB] = surf;
    MOZ_GL_ASSERT(mGL, mGL->IsFramebufferComplete(mFB));
  }

  mSurf = surf;
}

const gfx::IntSize& ReadBuffer::Size() const { return mSurf->mSize; }

void ReadBuffer::SetReadBuffer(GLenum userMode) const {
  if (!mGL->IsSupported(GLFeature::read_buffer)) return;

  GLenum internalMode;

  switch (userMode) {
||||||| merged common ancestors
    mGL->mFBOMapping.erase(mFB);
}

void
ReadBuffer::Attach(SharedSurface* surf)
{
    MOZ_ASSERT(surf && mSurf);
    MOZ_ASSERT(surf->mAttachType == mSurf->mAttachType);
    MOZ_ASSERT(surf->mSize == mSurf->mSize);

    // Nothing else is needed for AttachType Screen.
    if (surf->mAttachType != AttachmentType::Screen) {
        GLuint colorTex = 0;
        GLuint colorRB = 0;
        GLenum target = 0;

        switch (surf->mAttachType) {
        case AttachmentType::GLTexture:
            colorTex = surf->ProdTexture();
            target = surf->ProdTextureTarget();
            break;
        case AttachmentType::GLRenderbuffer:
            colorRB = surf->ProdRenderbuffer();
            break;
        default:
            MOZ_CRASH("GFX: Unknown attachment type, attach?");
        }

        mGL->AttachBuffersToFB(colorTex, colorRB, 0, 0, mFB, target);
        mGL->mFBOMapping[mFB] = surf;
        MOZ_ASSERT(mGL->IsFramebufferComplete(mFB));
    }

    mSurf = surf;
}

const gfx::IntSize&
ReadBuffer::Size() const
{
    return mSurf->mSize;
}

void
ReadBuffer::SetReadBuffer(GLenum userMode) const
{
    if (!mGL->IsSupported(GLFeature::read_buffer))
        return;

    GLenum internalMode;

    switch (userMode) {
=======
  switch (userMode) {
>>>>>>> upstream-releases
    case LOCAL_GL_BACK:
    case LOCAL_GL_FRONT:
      internalMode = (mFB == 0) ? userMode : LOCAL_GL_COLOR_ATTACHMENT0;
      break;

    case LOCAL_GL_NONE:
      internalMode = LOCAL_GL_NONE;
      break;

    default:
      MOZ_CRASH("GFX: Bad value.");
  }

  mGL->MakeCurrent();
  mGL->fReadBuffer(internalMode);
}

} /* namespace gl */
} /* namespace mozilla */
