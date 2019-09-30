/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_WINDOWSDWRITEFONTS_H
#define GFX_WINDOWSDWRITEFONTS_H

#include "mozilla/MemoryReporting.h"
#include "mozilla/UniquePtr.h"
#include <dwrite_1.h>

#include "gfxFont.h"
#include "gfxUserFontSet.h"
#include "nsDataHashtable.h"
#include "nsHashKeys.h"

#include "mozilla/gfx/gfxVars.h"
#include "mozilla/gfx/UnscaledFontDWrite.h"

struct _cairo_font_face;
typedef _cairo_font_face cairo_font_face_t;

/**
 * \brief Class representing a font face for a font entry.
 */
<<<<<<< HEAD
class gfxDWriteFont : public gfxFont {
 public:
  gfxDWriteFont(const RefPtr<mozilla::gfx::UnscaledFontDWrite> &aUnscaledFont,
                gfxFontEntry *aFontEntry, const gfxFontStyle *aFontStyle,
                RefPtr<IDWriteFontFace> aFontFace = nullptr,
                AntialiasOption = kAntialiasDefault);
  ~gfxDWriteFont();
||||||| merged common ancestors
class gfxDWriteFont : public gfxFont
{
public:
    gfxDWriteFont(const RefPtr<mozilla::gfx::UnscaledFontDWrite>& aUnscaledFont,
                  gfxFontEntry *aFontEntry,
                  const gfxFontStyle *aFontStyle,
                  RefPtr<IDWriteFontFace> aFontFace = nullptr,
                  AntialiasOption = kAntialiasDefault);
    ~gfxDWriteFont();
=======
class gfxDWriteFont : public gfxFont {
 public:
  gfxDWriteFont(const RefPtr<mozilla::gfx::UnscaledFontDWrite>& aUnscaledFont,
                gfxFontEntry* aFontEntry, const gfxFontStyle* aFontStyle,
                RefPtr<IDWriteFontFace> aFontFace = nullptr,
                AntialiasOption = kAntialiasDefault);
  ~gfxDWriteFont();
>>>>>>> upstream-releases

  static void UpdateSystemTextQuality();
  static void SystemTextQualityChanged();

  mozilla::UniquePtr<gfxFont> CopyWithAntialiasOption(
      AntialiasOption anAAOption) override;

<<<<<<< HEAD
  virtual uint32_t GetSpaceGlyph() override;
||||||| merged common ancestors
    virtual uint32_t GetSpaceGlyph() override;
=======
  uint32_t GetSpaceGlyph() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool SetupCairoFont(DrawTarget *aDrawTarget) override;
||||||| merged common ancestors
    virtual bool SetupCairoFont(DrawTarget* aDrawTarget) override;
=======
  bool SetupCairoFont(DrawTarget* aDrawTarget) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool AllowSubpixelAA() override { return mAllowManualShowGlyphs; }
||||||| merged common ancestors
    virtual bool AllowSubpixelAA() override
    { return mAllowManualShowGlyphs; }
=======
  bool AllowSubpixelAA() override { return mAllowManualShowGlyphs; }
>>>>>>> upstream-releases

  bool IsValid() const;

<<<<<<< HEAD
  IDWriteFontFace *GetFontFace();
||||||| merged common ancestors
    IDWriteFontFace *GetFontFace();
=======
  IDWriteFontFace* GetFontFace();
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* override Measure to add padding for antialiasing */
  virtual RunMetrics Measure(
      const gfxTextRun *aTextRun, uint32_t aStart, uint32_t aEnd,
      BoundingBoxType aBoundingBoxType,
      DrawTarget *aDrawTargetForTightBoundingBox, Spacing *aSpacing,
      mozilla::gfx::ShapedTextFlags aOrientation) override;
||||||| merged common ancestors
    /* override Measure to add padding for antialiasing */
    virtual RunMetrics Measure(const gfxTextRun *aTextRun,
                               uint32_t aStart, uint32_t aEnd,
                               BoundingBoxType aBoundingBoxType,
                               DrawTarget *aDrawTargetForTightBoundingBox,
                               Spacing *aSpacing,
                               mozilla::gfx::ShapedTextFlags aOrientation) override;
=======
  /* override Measure to add padding for antialiasing */
  RunMetrics Measure(const gfxTextRun* aTextRun, uint32_t aStart, uint32_t aEnd,
                     BoundingBoxType aBoundingBoxType,
                     DrawTarget* aDrawTargetForTightBoundingBox,
                     Spacing* aSpacing,
                     mozilla::gfx::ShapedTextFlags aOrientation) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool ProvidesGlyphWidths() const override;
||||||| merged common ancestors
    virtual bool ProvidesGlyphWidths() const override;
=======
  bool ProvidesGlyphWidths() const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual int32_t GetGlyphWidth(DrawTarget &aDrawTarget,
                                uint16_t aGID) override;
||||||| merged common ancestors
    virtual int32_t GetGlyphWidth(DrawTarget& aDrawTarget,
                                  uint16_t aGID) override;
=======
  int32_t GetGlyphWidth(uint16_t aGID) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void AddSizeOfExcludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                      FontCacheSizes *aSizes) const override;
  virtual void AddSizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                      FontCacheSizes *aSizes) const override;
||||||| merged common ancestors
    virtual void AddSizeOfExcludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                        FontCacheSizes* aSizes) const override;
    virtual void AddSizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                        FontCacheSizes* aSizes) const override;
=======
  void AddSizeOfExcludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                              FontCacheSizes* aSizes) const override;
  void AddSizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                              FontCacheSizes* aSizes) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual FontType GetType() const override { return FONT_TYPE_DWRITE; }
||||||| merged common ancestors
    virtual FontType GetType() const override { return FONT_TYPE_DWRITE; }
=======
  FontType GetType() const override { return FONT_TYPE_DWRITE; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual already_AddRefed<mozilla::gfx::ScaledFont> GetScaledFont(
      mozilla::gfx::DrawTarget *aTarget) override;
||||||| merged common ancestors
    virtual already_AddRefed<mozilla::gfx::ScaledFont>
    GetScaledFont(mozilla::gfx::DrawTarget *aTarget) override;
=======
  already_AddRefed<mozilla::gfx::ScaledFont> GetScaledFont(
      mozilla::gfx::DrawTarget* aTarget) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  cairo_scaled_font_t *InitCairoScaledFont();
||||||| merged common ancestors
protected:
    cairo_scaled_font_t *InitCairoScaledFont();
=======
 protected:
  cairo_scaled_font_t* InitCairoScaledFont();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual const Metrics &GetHorizontalMetrics() override;
||||||| merged common ancestors
    virtual const Metrics& GetHorizontalMetrics() override;
=======
  const Metrics& GetHorizontalMetrics() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool GetFakeMetricsForArialBlack(DWRITE_FONT_METRICS *aFontMetrics);
||||||| merged common ancestors
    bool GetFakeMetricsForArialBlack(DWRITE_FONT_METRICS *aFontMetrics);
=======
  bool GetFakeMetricsForArialBlack(DWRITE_FONT_METRICS* aFontMetrics);
>>>>>>> upstream-releases

  void ComputeMetrics(AntialiasOption anAAOption);

  bool HasBitmapStrikeForSize(uint32_t aSize);

<<<<<<< HEAD
  cairo_font_face_t *CairoFontFace();
||||||| merged common ancestors
    cairo_font_face_t *CairoFontFace();
=======
  cairo_font_face_t* CairoFontFace();
>>>>>>> upstream-releases

  gfxFloat MeasureGlyphWidth(uint16_t aGlyph);

  DWRITE_MEASURING_MODE GetMeasuringMode();
  bool GetForceGDIClassic();

  RefPtr<IDWriteFontFace> mFontFace;
  RefPtr<IDWriteFontFace1> mFontFace1;  // may be unavailable on older DWrite

<<<<<<< HEAD
  cairo_font_face_t *mCairoFontFace;
||||||| merged common ancestors
    cairo_font_face_t *mCairoFontFace;
=======
  cairo_font_face_t* mCairoFontFace;
>>>>>>> upstream-releases

<<<<<<< HEAD
  Metrics *mMetrics;
||||||| merged common ancestors
    Metrics *mMetrics;
=======
  Metrics* mMetrics;
>>>>>>> upstream-releases

  // cache of glyph widths in 16.16 fixed-point pixels
  mozilla::UniquePtr<nsDataHashtable<nsUint32HashKey, int32_t>> mGlyphWidths;

  uint32_t mSpaceGlyph;

  bool mUseSubpixelPositions;
  bool mAllowManualShowGlyphs;

  // Used to record the sUseClearType setting at the time mAzureScaledFont
  // was set up, so we can tell if it's stale and needs to be re-created.
  bool mAzureScaledFontUsedClearType;

  bool UsingClearType() {
    return mozilla::gfx::gfxVars::SystemTextQuality() == CLEARTYPE_QUALITY;
  }
};

#endif
