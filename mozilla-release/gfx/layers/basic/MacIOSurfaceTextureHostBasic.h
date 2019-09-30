/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_MACIOSURFACETEXTUREHOST_BASIC_H
#define MOZILLA_GFX_MACIOSURFACETEXTUREHOST_BASIC_H

#include "mozilla/layers/BasicCompositor.h"
#include "mozilla/layers/TextureHostBasic.h"

class MacIOSurface;

namespace mozilla {
namespace layers {

class BasicCompositor;

/**
 * A texture source meant for use with BasicCompositor.
 *
 * It does not own any GL texture, and attaches its shared handle to one of
 * the compositor's temporary textures when binding.
 */
class MacIOSurfaceTextureSourceBasic : public TextureSourceBasic,
                                       public TextureSource {
 public:
  explicit MacIOSurfaceTextureSourceBasic(MacIOSurface* aSurface);
  virtual ~MacIOSurfaceTextureSourceBasic();

<<<<<<< HEAD
  virtual const char* Name() const override {
    return "MacIOSurfaceTextureSourceBasic";
  }
||||||| merged common ancestors
  virtual const char* Name() const override { return "MacIOSurfaceTextureSourceBasic"; }
=======
  const char* Name() const override { return "MacIOSurfaceTextureSourceBasic"; }
>>>>>>> upstream-releases

  TextureSourceBasic* AsSourceBasic() override { return this; }

  gfx::IntSize GetSize() const override;
  gfx::SurfaceFormat GetFormat() const override;
  gfx::SourceSurface* GetSurface(gfx::DrawTarget* aTarget) override;

<<<<<<< HEAD
  virtual void DeallocateDeviceData() override {}
||||||| merged common ancestors
  virtual void DeallocateDeviceData() override { }
=======
  void DeallocateDeviceData() override {}
>>>>>>> upstream-releases

 protected:
  RefPtr<MacIOSurface> mSurface;
  RefPtr<gfx::SourceSurface> mSourceSurface;
};

/**
 * A TextureHost for shared MacIOSurface
 *
 * Most of the logic actually happens in MacIOSurfaceTextureSourceBasic.
 */
class MacIOSurfaceTextureHostBasic : public TextureHost {
 public:
  MacIOSurfaceTextureHostBasic(
      TextureFlags aFlags, const SurfaceDescriptorMacIOSurface& aDescriptor);

  virtual void SetTextureSourceProvider(
      TextureSourceProvider* aProvider) override;

  bool Lock() override;

  gfx::SurfaceFormat GetFormat() const override;

  virtual bool BindTextureSource(
      CompositableTextureSourceRef& aTexture) override {
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

  gfx::IntSize GetSize() const override;
  MacIOSurface* GetMacIOSurface() override { return mSurface; }

#ifdef MOZ_LAYERS_HAVE_LOG
  const char* Name() override { return "MacIOSurfaceTextureHostBasic"; }
#endif

 protected:
  RefPtr<MacIOSurfaceTextureSourceBasic> mTextureSource;
  RefPtr<MacIOSurface> mSurface;
};

}  // namespace layers
}  // namespace mozilla

#endif  // MOZILLA_GFX_MACIOSURFACETEXTUREHOSTOGL_BASIC_H
