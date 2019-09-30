/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_X11TEXTUREHOST__H
#define MOZILLA_GFX_X11TEXTUREHOST__H

#include "mozilla/layers/TextureHost.h"
#include "mozilla/layers/LayersSurfaces.h"
#include "mozilla/gfx/Types.h"

#include "gfxXlibSurface.h"

namespace mozilla {
namespace layers {

class X11TextureSource : public TextureSource {
 public:
  // Called when the underlying X surface has been changed.
  // Useful for determining whether to rebind a GLXPixmap to a texture.
  virtual void Updated() = 0;

  const char* Name() const override { return "X11TextureSource"; }
};

// TextureHost for Xlib-backed TextureSources.
class X11TextureHost : public TextureHost {
 public:
  X11TextureHost(TextureFlags aFlags, const SurfaceDescriptorX11& aDescriptor);

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

  gfx::IntSize GetSize() const override;

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

  already_AddRefed<gfx::DataSourceSurface> GetAsSurface() override;

#ifdef MOZ_LAYERS_HAVE_LOG
  const char* Name() override { return "X11TextureHost"; }
#endif

<<<<<<< HEAD
 protected:
  virtual void UpdatedInternal(const nsIntRegion*) override {
    if (mTextureSource) mTextureSource->Updated();
||||||| merged common ancestors
protected:
  virtual void UpdatedInternal(const nsIntRegion*) override
  {
    if (mTextureSource)
      mTextureSource->Updated();
=======
 protected:
  void UpdatedInternal(const nsIntRegion*) override {
    if (mTextureSource) mTextureSource->Updated();
>>>>>>> upstream-releases
  }

  RefPtr<Compositor> mCompositor;
  RefPtr<X11TextureSource> mTextureSource;
  RefPtr<gfxXlibSurface> mSurface;
};

}  // namespace layers
}  // namespace mozilla

#endif  // MOZILLA_GFX_X11TEXTUREHOST__H
