/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_PLATFORM_ANDROID_H
#define GFX_PLATFORM_ANDROID_H

#include "gfxPlatform.h"
#include "gfxUserFontSet.h"
#include "nsCOMPtr.h"
#include "nsTArray.h"

namespace mozilla {
namespace dom {
class FontListEntry;
};
};  // namespace mozilla
using mozilla::dom::FontListEntry;

class gfxAndroidPlatform : public gfxPlatform {
 public:
  gfxAndroidPlatform();
  virtual ~gfxAndroidPlatform();

  static gfxAndroidPlatform* GetPlatform() {
    return (gfxAndroidPlatform*)gfxPlatform::GetPlatform();
  }

<<<<<<< HEAD
  virtual already_AddRefed<gfxASurface> CreateOffscreenSurface(
      const IntSize& aSize, gfxImageFormat aFormat) override;
||||||| merged common ancestors
    virtual already_AddRefed<gfxASurface>
    CreateOffscreenSurface(const IntSize& aSize,
                           gfxImageFormat aFormat) override;
    
    virtual gfxImageFormat GetOffscreenFormat() override { return mOffscreenFormat; }
=======
  already_AddRefed<gfxASurface> CreateOffscreenSurface(
      const IntSize& aSize, gfxImageFormat aFormat) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual gfxImageFormat GetOffscreenFormat() override {
    return mOffscreenFormat;
  }
||||||| merged common ancestors
    // to support IPC font list (sharing between chrome and content)
    void GetSystemFontList(InfallibleTArray<FontListEntry>* retValue);
=======
  gfxImageFormat GetOffscreenFormat() override { return mOffscreenFormat; }
>>>>>>> upstream-releases

  // to support IPC font list (sharing between chrome and content)
  void GetSystemFontList(InfallibleTArray<FontListEntry>* retValue);

<<<<<<< HEAD
  // platform implementations of font functions
  virtual gfxPlatformFontList* CreatePlatformFontList() override;
||||||| merged common ancestors
    virtual void GetCommonFallbackFonts(uint32_t aCh, uint32_t aNextCh,
                                        Script aRunScript,
                                        nsTArray<const char*>& aFontList) override;
=======
  // platform implementations of font functions
  gfxPlatformFontList* CreatePlatformFontList() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void GetCommonFallbackFonts(
      uint32_t aCh, uint32_t aNextCh, Script aRunScript,
      nsTArray<const char*>& aFontList) override;
||||||| merged common ancestors
    gfxFontGroup*
    CreateFontGroup(const mozilla::FontFamilyList& aFontFamilyList,
                    const gfxFontStyle *aStyle,
                    gfxTextPerfMetrics* aTextPerf,
                    gfxUserFontSet *aUserFontSet,
                    gfxFloat aDevToCssSize) override;
=======
  void GetCommonFallbackFonts(uint32_t aCh, uint32_t aNextCh, Script aRunScript,
                              nsTArray<const char*>& aFontList) override;
>>>>>>> upstream-releases

  gfxFontGroup* CreateFontGroup(const mozilla::FontFamilyList& aFontFamilyList,
                                const gfxFontStyle* aStyle,
                                gfxTextPerfMetrics* aTextPerf,
                                gfxUserFontSet* aUserFontSet,
                                gfxFloat aDevToCssSize) override;

<<<<<<< HEAD
  virtual bool FontHintingEnabled() override;
  virtual bool RequiresLinearZoom() override;
||||||| merged common ancestors
    FT_Library GetFTLibrary() override;
=======
  bool FontHintingEnabled() override;
  bool RequiresLinearZoom() override;
>>>>>>> upstream-releases

  FT_Library GetFTLibrary() override;

<<<<<<< HEAD
  virtual already_AddRefed<mozilla::gfx::VsyncSource>
  CreateHardwareVsyncSource() override;
||||||| merged common ancestors
protected:
    bool AccelerateLayersByDefault() override {
      return true;
    }
=======
  already_AddRefed<mozilla::gfx::VsyncSource> CreateHardwareVsyncSource()
      override;
>>>>>>> upstream-releases

 protected:
  bool AccelerateLayersByDefault() override { return true; }

  bool CheckVariationFontSupport() override {
    // We build with in-tree FreeType, so we know it is a new enough
    // version to support variations.
    return true;
  }

 private:
  gfxImageFormat mOffscreenFormat;
};

#endif /* GFX_PLATFORM_ANDROID_H */
