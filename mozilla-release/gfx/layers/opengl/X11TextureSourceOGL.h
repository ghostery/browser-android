/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_X11TEXTURESOURCEOGL__H
#define MOZILLA_GFX_X11TEXTURESOURCEOGL__H

#ifdef MOZ_X11

#  include "mozilla/layers/CompositorOGL.h"
#  include "mozilla/layers/TextureHostOGL.h"
#  include "mozilla/layers/X11TextureHost.h"
#  include "mozilla/gfx/2D.h"

namespace mozilla {
namespace layers {

// TextureSource for Xlib-backed surfaces.
class X11TextureSourceOGL : public TextureSourceOGL, public X11TextureSource {
 public:
  X11TextureSourceOGL(CompositorOGL* aCompositor, gfxXlibSurface* aSurface);
  ~X11TextureSourceOGL();

  X11TextureSourceOGL* AsSourceOGL() override { return this; }

<<<<<<< HEAD
  virtual bool IsValid() const override { return !!gl(); };
||||||| merged common ancestors
  virtual bool IsValid() const override { return !!gl(); } ;
=======
  bool IsValid() const override { return !!gl(); };
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void BindTexture(GLenum aTextureUnit,
                           gfx::SamplingFilter aSamplingFilter) override;
||||||| merged common ancestors
  virtual void BindTexture(GLenum aTextureUnit, gfx::SamplingFilter aSamplingFilter) override;
=======
  void BindTexture(GLenum aTextureUnit,
                   gfx::SamplingFilter aSamplingFilter) override;
>>>>>>> upstream-releases

  gfx::IntSize GetSize() const override;

<<<<<<< HEAD
  virtual GLenum GetTextureTarget() const override {
    return LOCAL_GL_TEXTURE_2D;
  }
||||||| merged common ancestors
  virtual GLenum GetTextureTarget() const override { return LOCAL_GL_TEXTURE_2D; }
=======
  GLenum GetTextureTarget() const override { return LOCAL_GL_TEXTURE_2D; }
>>>>>>> upstream-releases

  gfx::SurfaceFormat GetFormat() const override;

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

  void Updated() override { mUpdated = true; }

  gl::GLContext* gl() const { return mGL; }

  static gfx::SurfaceFormat ContentTypeToSurfaceFormat(gfxContentType aType);

 protected:
  RefPtr<gl::GLContext> mGL;
  RefPtr<gfxXlibSurface> mSurface;
  RefPtr<gfx::SourceSurface> mSourceSurface;
  GLuint mTexture;
  bool mUpdated;
};

}  // namespace layers
}  // namespace mozilla

#endif

#endif  // MOZILLA_GFX_X11TEXTURESOURCEOGL__H
