/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_PLATFORM_MAC_H
#define GFX_PLATFORM_MAC_H

#include "nsTArrayForwardDeclare.h"
#include "gfxPlatform.h"
#include "mozilla/LookAndFeel.h"

namespace mozilla {
namespace gfx {
class DrawTarget;
class VsyncSource;
}  // namespace gfx
}  // namespace mozilla

class gfxPlatformMac : public gfxPlatform {
 public:
  gfxPlatformMac();
  virtual ~gfxPlatformMac();

  static gfxPlatformMac* GetPlatform() {
    return (gfxPlatformMac*)gfxPlatform::GetPlatform();
  }

  bool UsesTiling() const override;
  bool ContentUsesTiling() const override;

<<<<<<< HEAD
  virtual already_AddRefed<gfxASurface> CreateOffscreenSurface(
      const IntSize& aSize, gfxImageFormat aFormat) override;
||||||| merged common ancestors
    virtual already_AddRefed<gfxASurface>
      CreateOffscreenSurface(const IntSize& aSize,
                             gfxImageFormat aFormat) override;
=======
  already_AddRefed<gfxASurface> CreateOffscreenSurface(
      const IntSize& aSize, gfxImageFormat aFormat) override;
>>>>>>> upstream-releases

  gfxFontGroup* CreateFontGroup(const mozilla::FontFamilyList& aFontFamilyList,
                                const gfxFontStyle* aStyle,
                                gfxTextPerfMetrics* aTextPerf,
                                gfxUserFontSet* aUserFontSet,
                                gfxFloat aDevToCssSize) override;

<<<<<<< HEAD
  virtual gfxPlatformFontList* CreatePlatformFontList() override;
||||||| merged common ancestors
    virtual gfxPlatformFontList* CreatePlatformFontList() override;
=======
  gfxPlatformFontList* CreatePlatformFontList() override;
>>>>>>> upstream-releases

  void ReadSystemFontList(
      InfallibleTArray<mozilla::dom::SystemFontListEntry>* aFontList) override;

  bool IsFontFormatSupported(uint32_t aFormatFlags) override;

<<<<<<< HEAD
  virtual void GetCommonFallbackFonts(
      uint32_t aCh, uint32_t aNextCh, Script aRunScript,
      nsTArray<const char*>& aFontList) override;
||||||| merged common ancestors
    virtual void GetCommonFallbackFonts(uint32_t aCh, uint32_t aNextCh,
                                        Script aRunScript,
                                        nsTArray<const char*>& aFontList) override;
=======
  void GetCommonFallbackFonts(uint32_t aCh, uint32_t aNextCh, Script aRunScript,
                              nsTArray<const char*>& aFontList) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // lookup the system font for a particular system font type and set
  // the name and style characteristics
  static void LookupSystemFont(mozilla::LookAndFeel::FontID aSystemFontID,
                               nsACString& aSystemFontName,
                               gfxFontStyle& aFontStyle,
                               float aDevPixPerCSSPixel);
||||||| merged common ancestors
    // lookup the system font for a particular system font type and set
    // the name and style characteristics
    static void
    LookupSystemFont(mozilla::LookAndFeel::FontID aSystemFontID,
                     nsACString& aSystemFontName,
                     gfxFontStyle &aFontStyle,
                     float aDevPixPerCSSPixel);
=======
  // lookup the system font for a particular system font type and set
  // the name and style characteristics
  static void LookupSystemFont(mozilla::LookAndFeel::FontID aSystemFontID,
                               nsACString& aSystemFontName,
                               gfxFontStyle& aFontStyle);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool SupportsApzWheelInput() const override { return true; }
||||||| merged common ancestors
    virtual bool SupportsApzWheelInput() const override {
      return true;
    }
=======
  bool SupportsApzWheelInput() const override { return true; }
>>>>>>> upstream-releases

  bool RespectsFontStyleSmoothing() const override {
    // gfxMacFont respects the font smoothing hint.
    return true;
  }

  bool RequiresAcceleratedGLContextForCompositorOGL() const override {
    // On OS X in a VM, unaccelerated CompositorOGL shows black flashes, so we
    // require accelerated GL for CompositorOGL but allow unaccelerated GL for
    // BasicCompositor.
    return true;
  }

<<<<<<< HEAD
  virtual already_AddRefed<mozilla::gfx::VsyncSource>
  CreateHardwareVsyncSource() override;
||||||| merged common ancestors
    virtual already_AddRefed<mozilla::gfx::VsyncSource> CreateHardwareVsyncSource() override;
=======
  already_AddRefed<mozilla::gfx::VsyncSource> CreateHardwareVsyncSource()
      override;
>>>>>>> upstream-releases

  // lower threshold on font anti-aliasing
  uint32_t GetAntiAliasingThreshold() { return mFontAntiAliasingThreshold; }

 protected:
  bool AccelerateLayersByDefault() override;

  BackendPrefsData GetBackendPrefs() const override;

  bool CheckVariationFontSupport() override;

<<<<<<< HEAD
 private:
  virtual void GetPlatformCMSOutputProfile(void*& mem, size_t& size) override;
||||||| merged common ancestors
private:
    virtual void GetPlatformCMSOutputProfile(void* &mem, size_t &size) override;
=======
 private:
  void GetPlatformCMSOutputProfile(void*& mem, size_t& size) override;
>>>>>>> upstream-releases

  // read in the pref value for the lower threshold on font anti-aliasing
  static uint32_t ReadAntiAliasingThreshold();

  uint32_t mFontAntiAliasingThreshold;
};

#endif /* GFX_PLATFORM_MAC_H */
