/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_GDIFONT_H
#define GFX_GDIFONT_H

#include "mozilla/MemoryReporting.h"
#include "gfxFont.h"
#include "gfxGDIFontList.h"

#include "nsDataHashtable.h"
#include "nsHashKeys.h"

#include "cairo.h"
#include "usp10.h"

<<<<<<< HEAD
class gfxGDIFont : public gfxFont {
 public:
  gfxGDIFont(GDIFontEntry *aFontEntry, const gfxFontStyle *aFontStyle,
             AntialiasOption anAAOption = kAntialiasDefault);
||||||| merged common ancestors
class gfxGDIFont : public gfxFont
{
public:
    gfxGDIFont(GDIFontEntry *aFontEntry,
               const gfxFontStyle *aFontStyle,
               AntialiasOption anAAOption = kAntialiasDefault);
=======
class gfxGDIFont : public gfxFont {
 public:
  gfxGDIFont(GDIFontEntry* aFontEntry, const gfxFontStyle* aFontStyle,
             AntialiasOption anAAOption = kAntialiasDefault);
>>>>>>> upstream-releases

  virtual ~gfxGDIFont();

  HFONT GetHFONT() { return mFont; }

<<<<<<< HEAD
  cairo_font_face_t *CairoFontFace() { return mFontFace; }
||||||| merged common ancestors
    cairo_font_face_t* CairoFontFace() { return mFontFace; }
=======
  cairo_font_face_t* CairoFontFace() { return mFontFace; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* overrides for the pure virtual methods in gfxFont */
  virtual uint32_t GetSpaceGlyph() override;
||||||| merged common ancestors
    /* overrides for the pure virtual methods in gfxFont */
    virtual uint32_t GetSpaceGlyph() override;
=======
  /* overrides for the pure virtual methods in gfxFont */
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
  virtual already_AddRefed<mozilla::gfx::ScaledFont> GetScaledFont(
      DrawTarget *aTarget) override;
||||||| merged common ancestors
    virtual already_AddRefed<mozilla::gfx::ScaledFont>
    GetScaledFont(DrawTarget *aTarget) override;
=======
  already_AddRefed<mozilla::gfx::ScaledFont> GetScaledFont(
      DrawTarget* aTarget) override;
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

  /* required for MathML to suppress effects of ClearType "padding" */
  mozilla::UniquePtr<gfxFont> CopyWithAntialiasOption(
      AntialiasOption anAAOption) override;

<<<<<<< HEAD
  // If the font has a cmap table, we handle it purely with harfbuzz;
  // but if not (e.g. .fon fonts), we'll use a GDI callback to get glyphs.
  virtual bool ProvidesGetGlyph() const override {
    return !mFontEntry->HasCmapTable();
  }
||||||| merged common ancestors
    // If the font has a cmap table, we handle it purely with harfbuzz;
    // but if not (e.g. .fon fonts), we'll use a GDI callback to get glyphs.
    virtual bool ProvidesGetGlyph() const override {
        return !mFontEntry->HasCmapTable();
    }
=======
  // If the font has a cmap table, we handle it purely with harfbuzz;
  // but if not (e.g. .fon fonts), we'll use a GDI callback to get glyphs.
  bool ProvidesGetGlyph() const override { return !mFontEntry->HasCmapTable(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual uint32_t GetGlyph(uint32_t aUnicode, uint32_t aVarSelector) override;
||||||| merged common ancestors
    virtual uint32_t GetGlyph(uint32_t aUnicode,
                              uint32_t aVarSelector) override;
=======
  uint32_t GetGlyph(uint32_t aUnicode, uint32_t aVarSelector) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool ProvidesGlyphWidths() const override { return true; }
||||||| merged common ancestors
    virtual bool ProvidesGlyphWidths() const override { return true; }
=======
  bool ProvidesGlyphWidths() const override { return true; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // get hinted glyph width in pixels as 16.16 fixed-point value
  virtual int32_t GetGlyphWidth(DrawTarget &aDrawTarget,
                                uint16_t aGID) override;
||||||| merged common ancestors
    // get hinted glyph width in pixels as 16.16 fixed-point value
    virtual int32_t GetGlyphWidth(DrawTarget& aDrawTarget,
                                  uint16_t aGID) override;
=======
  // get hinted glyph width in pixels as 16.16 fixed-point value
  int32_t GetGlyphWidth(uint16_t aGID) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void AddSizeOfExcludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                      FontCacheSizes *aSizes) const;
  virtual void AddSizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                      FontCacheSizes *aSizes) const;
||||||| merged common ancestors
    virtual void AddSizeOfExcludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                        FontCacheSizes* aSizes) const;
    virtual void AddSizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                        FontCacheSizes* aSizes) const;
=======
  void AddSizeOfExcludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                              FontCacheSizes* aSizes) const;
  void AddSizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                              FontCacheSizes* aSizes) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual FontType GetType() const override { return FONT_TYPE_GDI; }
||||||| merged common ancestors
    virtual FontType GetType() const override { return FONT_TYPE_GDI; }
=======
  FontType GetType() const override { return FONT_TYPE_GDI; }
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  virtual const Metrics &GetHorizontalMetrics() override;
||||||| merged common ancestors
protected:
    virtual const Metrics& GetHorizontalMetrics() override;
=======
 protected:
  const Metrics& GetHorizontalMetrics() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* override to ensure the cairo font is set up properly */
  bool ShapeText(DrawTarget *aDrawTarget, const char16_t *aText,
                 uint32_t aOffset, uint32_t aLength, Script aScript,
                 bool aVertical, RoundingFlags aRounding,
                 gfxShapedText *aShapedText) override;
||||||| merged common ancestors
    /* override to ensure the cairo font is set up properly */
    bool ShapeText(DrawTarget     *aDrawTarget,
                   const char16_t *aText,
                   uint32_t        aOffset,
                   uint32_t        aLength,
                   Script          aScript,
                   bool            aVertical,
                   RoundingFlags   aRounding,
                   gfxShapedText  *aShapedText) override;
=======
  /* override to ensure the cairo font is set up properly */
  bool ShapeText(DrawTarget* aDrawTarget, const char16_t* aText,
                 uint32_t aOffset, uint32_t aLength, Script aScript,
                 bool aVertical, RoundingFlags aRounding,
                 gfxShapedText* aShapedText) override;
>>>>>>> upstream-releases

  void Initialize();  // creates metrics and Cairo fonts

<<<<<<< HEAD
  // Fill the given LOGFONT record according to our size.
  // (Synthetic italic is *not* handled here, because GDI may not reliably
  // use the face we expect if we tweak the lfItalic field, and because we
  // have generic support for this in gfxFont::Draw instead.)
  void FillLogFont(LOGFONTW &aLogFont, gfxFloat aSize);
||||||| merged common ancestors
    // Fill the given LOGFONT record according to our size.
    // (Synthetic italic is *not* handled here, because GDI may not reliably
    // use the face we expect if we tweak the lfItalic field, and because we
    // have generic support for this in gfxFont::Draw instead.)
    void FillLogFont(LOGFONTW& aLogFont, gfxFloat aSize);
=======
  // Fill the given LOGFONT record according to our size.
  // (Synthetic italic is *not* handled here, because GDI may not reliably
  // use the face we expect if we tweak the lfItalic field, and because we
  // have generic support for this in gfxFont::Draw instead.)
  void FillLogFont(LOGFONTW& aLogFont, gfxFloat aSize);
>>>>>>> upstream-releases

<<<<<<< HEAD
  HFONT mFont;
  cairo_font_face_t *mFontFace;
||||||| merged common ancestors
    HFONT                 mFont;
    cairo_font_face_t    *mFontFace;
=======
  HFONT mFont;
  cairo_font_face_t* mFontFace;
>>>>>>> upstream-releases

<<<<<<< HEAD
  Metrics *mMetrics;
  uint32_t mSpaceGlyph;
||||||| merged common ancestors
    Metrics              *mMetrics;
    uint32_t              mSpaceGlyph;
=======
  Metrics* mMetrics;
  uint32_t mSpaceGlyph;
>>>>>>> upstream-releases

  bool mNeedsSyntheticBold;

  // cache of glyph IDs (used for non-sfnt fonts only)
  mozilla::UniquePtr<nsDataHashtable<nsUint32HashKey, uint32_t> > mGlyphIDs;
  SCRIPT_CACHE mScriptCache;

  // cache of glyph widths in 16.16 fixed-point pixels
  mozilla::UniquePtr<nsDataHashtable<nsUint32HashKey, int32_t> > mGlyphWidths;
};

#endif /* GFX_GDIFONT_H */
