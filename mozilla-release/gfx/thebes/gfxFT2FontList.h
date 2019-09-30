/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_FT2FONTLIST_H
#define GFX_FT2FONTLIST_H

#include "mozilla/MemoryReporting.h"
#include "gfxPlatformFontList.h"
#include "mozilla/gfx/UnscaledFontFreeType.h"

namespace mozilla {
namespace dom {
class FontListEntry;
};
};  // namespace mozilla
using mozilla::dom::FontListEntry;

class FontNameCache;
typedef struct FT_FaceRec_* FT_Face;
class nsZipArchive;
class WillShutdownObserver;

class FT2FontEntry : public gfxFontEntry {
 public:
  explicit FT2FontEntry(const nsACString& aFaceName)
      : gfxFontEntry(aFaceName),
        mFTFace(nullptr),
        mFontFace(nullptr),
        mFTFontIndex(0) {}

  ~FT2FontEntry();

  gfxFontEntry* Clone() const override;

  const nsCString& GetName() const { return Name(); }

  // create a font entry for a downloaded font
  static FT2FontEntry* CreateFontEntry(
      const nsACString& aFontName, WeightRange aWeight, StretchRange aStretch,
      SlantStyleRange aStyle, const uint8_t* aFontData, uint32_t aLength);

  // create a font entry representing an installed font, identified by
  // a FontListEntry; the freetype and cairo faces will not be instantiated
  // until actually needed
  static FT2FontEntry* CreateFontEntry(const FontListEntry& aFLE);

  // Create a font entry for a given freetype face; if it is an installed font,
  // also record the filename and index.
  // aFontData (if non-nullptr) is NS_Malloc'ed data that aFace depends on,
  // to be freed after the face is destroyed.
  // aLength is the length of aFontData.
  static FT2FontEntry* CreateFontEntry(FT_Face aFace, const char* aFilename,
                                       uint8_t aIndex, const nsACString& aName,
                                       const uint8_t* aFontData = nullptr,
                                       uint32_t aLength = 0);

  gfxFont* CreateFontInstance(const gfxFontStyle* aFontStyle) override;

  // Create (if necessary) and return the cairo_font_face for this font.
  // This may fail and return null, so caller must be prepared to handle this.
  // If a style is passed, any variationSettings in the style will be applied
  // to the resulting font face.
  cairo_font_face_t* CairoFontFace(const gfxFontStyle* aStyle = nullptr);

  // Create a cairo_scaled_font for this face, with the given style.
  // This may fail and return null, so caller must be prepared to handle this.
  cairo_scaled_font_t* CreateScaledFont(const gfxFontStyle* aStyle);

  nsresult ReadCMAP(FontInfoData* aFontInfoData = nullptr) override;

<<<<<<< HEAD
  virtual hb_blob_t* GetFontTable(uint32_t aTableTag) override;
||||||| merged common ancestors
    virtual hb_blob_t* GetFontTable(uint32_t aTableTag) override;
=======
  hb_blob_t* GetFontTable(uint32_t aTableTag) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual nsresult CopyFontTable(uint32_t aTableTag,
                                 nsTArray<uint8_t>& aBuffer) override;
||||||| merged common ancestors
    virtual nsresult CopyFontTable(uint32_t aTableTag,
                                   nsTArray<uint8_t>& aBuffer) override;
=======
  nsresult CopyFontTable(uint32_t aTableTag,
                         nsTArray<uint8_t>& aBuffer) override;
>>>>>>> upstream-releases

  bool HasVariations() override;
  void GetVariationAxes(
      nsTArray<gfxFontVariationAxis>& aVariationAxes) override;
  void GetVariationInstances(
      nsTArray<gfxFontVariationInstance>& aInstances) override;

  // Check for various kinds of brokenness, and set flags on the entry
  // accordingly so that we avoid using bad font tables
  void CheckForBrokenFont(gfxFontFamily* aFamily);

  FT_MM_Var* GetMMVar() override;

<<<<<<< HEAD
  virtual void AddSizeOfExcludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                      FontListSizes* aSizes) const override;
  virtual void AddSizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                      FontListSizes* aSizes) const override;
||||||| merged common ancestors
    virtual void AddSizeOfExcludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                        FontListSizes* aSizes) const override;
    virtual void AddSizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                        FontListSizes* aSizes) const override;
=======
  void AddSizeOfExcludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                              FontListSizes* aSizes) const override;
  void AddSizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                              FontListSizes* aSizes) const override;
>>>>>>> upstream-releases

  FT_Face mFTFace;
  cairo_font_face_t* mFontFace;

  FT_MM_Var* mMMVar = nullptr;

  nsCString mFilename;
  uint8_t mFTFontIndex;

  mozilla::ThreadSafeWeakPtr<mozilla::gfx::UnscaledFontFreeType> mUnscaledFont;

  bool mHasVariations = false;
  bool mHasVariationsInitialized = false;
  bool mMMVarInitialized = false;
};

class FT2FontFamily : public gfxFontFamily {
 public:
  explicit FT2FontFamily(const nsACString& aName) : gfxFontFamily(aName) {}

  // Append this family's faces to the IPC fontlist
  void AddFacesToFontList(InfallibleTArray<FontListEntry>* aFontList);
};

class gfxFT2FontList : public gfxPlatformFontList {
 public:
  gfxFT2FontList();
  virtual ~gfxFT2FontList();

<<<<<<< HEAD
  virtual gfxFontEntry* LookupLocalFont(
      const nsACString& aFontName, WeightRange aWeightForEntry,
      StretchRange aStretchForEntry, SlantStyleRange aStyleForEntry) override;
||||||| merged common ancestors
    virtual gfxFontEntry* LookupLocalFont(const nsACString& aFontName,
                                          WeightRange aWeightForEntry,
                                          StretchRange aStretchForEntry,
                                          SlantStyleRange aStyleForEntry) override;
=======
  gfxFontEntry* LookupLocalFont(const nsACString& aFontName,
                                WeightRange aWeightForEntry,
                                StretchRange aStretchForEntry,
                                SlantStyleRange aStyleForEntry) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual gfxFontEntry* MakePlatformFont(const nsACString& aFontName,
                                         WeightRange aWeightForEntry,
                                         StretchRange aStretchForEntry,
                                         SlantStyleRange aStyleForEntry,
                                         const uint8_t* aFontData,
                                         uint32_t aLength) override;
||||||| merged common ancestors
    virtual gfxFontEntry* MakePlatformFont(const nsACString& aFontName,
                                           WeightRange aWeightForEntry,
                                           StretchRange aStretchForEntry,
                                           SlantStyleRange aStyleForEntry,
                                           const uint8_t* aFontData,
                                           uint32_t aLength) override;
=======
  gfxFontEntry* MakePlatformFont(const nsACString& aFontName,
                                 WeightRange aWeightForEntry,
                                 StretchRange aStretchForEntry,
                                 SlantStyleRange aStyleForEntry,
                                 const uint8_t* aFontData,
                                 uint32_t aLength) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void GetSystemFontList(InfallibleTArray<FontListEntry>* retValue);
||||||| merged common ancestors
    void GetSystemFontList(InfallibleTArray<FontListEntry>* retValue);
=======
  void WriteCache();
>>>>>>> upstream-releases

<<<<<<< HEAD
  static gfxFT2FontList* PlatformFontList() {
    return static_cast<gfxFT2FontList*>(
        gfxPlatformFontList::PlatformFontList());
  }
||||||| merged common ancestors
    static gfxFT2FontList* PlatformFontList() {
        return static_cast<gfxFT2FontList*>(gfxPlatformFontList::PlatformFontList());
    }
=======
  void GetSystemFontList(InfallibleTArray<FontListEntry>* retValue);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void GetFontFamilyList(
      nsTArray<RefPtr<gfxFontFamily> >& aFamilyArray) override;
||||||| merged common ancestors
    virtual void GetFontFamilyList(nsTArray<RefPtr<gfxFontFamily> >& aFamilyArray) override;
=======
  static gfxFT2FontList* PlatformFontList() {
    return static_cast<gfxFT2FontList*>(
        gfxPlatformFontList::PlatformFontList());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  gfxFontFamily* CreateFontFamily(const nsACString& aName) const override;
||||||| merged common ancestors
    gfxFontFamily* CreateFontFamily(const nsACString& aName) const override;
=======
  void GetFontFamilyList(
      nsTArray<RefPtr<gfxFontFamily> >& aFamilyArray) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void WillShutdown();
||||||| merged common ancestors
    void WillShutdown();
=======
  gfxFontFamily* CreateFontFamily(const nsACString& aName) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  typedef enum { kUnknown, kStandard } StandardFile;
||||||| merged common ancestors
protected:
    typedef enum {
        kUnknown,
        kStandard
    } StandardFile;
=======
  void WillShutdown();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // initialize font lists
  virtual nsresult InitFontListForPlatform() override;
||||||| merged common ancestors
    // initialize font lists
    virtual nsresult InitFontListForPlatform() override;
=======
 protected:
  typedef enum { kUnknown, kStandard } StandardFile;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void AppendFaceFromFontListEntry(const FontListEntry& aFLE,
                                   StandardFile aStdFile);
||||||| merged common ancestors
    void AppendFaceFromFontListEntry(const FontListEntry& aFLE,
                                     StandardFile aStdFile);
=======
  // initialize font lists
  nsresult InitFontListForPlatform() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void AppendFacesFromFontFile(const nsCString& aFileName,
                               FontNameCache* aCache, StandardFile aStdFile);
||||||| merged common ancestors
    void AppendFacesFromFontFile(const nsCString& aFileName,
                                 FontNameCache *aCache,
                                 StandardFile aStdFile);
=======
  void AppendFaceFromFontListEntry(const FontListEntry& aFLE,
                                   StandardFile aStdFile);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void AppendFacesFromOmnijarEntry(nsZipArchive* aReader,
                                   const nsCString& aEntryName,
                                   FontNameCache* aCache, bool aJarChanged);
||||||| merged common ancestors
    void AppendFacesFromOmnijarEntry(nsZipArchive *aReader,
                                     const nsCString& aEntryName,
                                     FontNameCache *aCache,
                                     bool aJarChanged);
=======
  void AppendFacesFromFontFile(const nsCString& aFileName,
                               FontNameCache* aCache, StandardFile aStdFile);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // the defaults here are suitable for reading bundled fonts from omnijar
  void AppendFacesFromCachedFaceList(const nsCString& aFileName,
                                     const nsCString& aFaceList,
                                     StandardFile aStdFile = kStandard);
||||||| merged common ancestors
    // the defaults here are suitable for reading bundled fonts from omnijar
    void AppendFacesFromCachedFaceList(const nsCString& aFileName,
                                       const nsCString& aFaceList,
                                       StandardFile aStdFile = kStandard);
=======
  void AppendFacesFromOmnijarEntry(nsZipArchive* aReader,
                                   const nsCString& aEntryName,
                                   FontNameCache* aCache, bool aJarChanged);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void AddFaceToList(const nsCString& aEntryName, uint32_t aIndex,
                     StandardFile aStdFile, FT_Face aFace,
                     nsCString& aFaceList);
||||||| merged common ancestors
    void AddFaceToList(const nsCString& aEntryName, uint32_t aIndex,
                       StandardFile aStdFile,
                       FT_Face aFace, nsCString& aFaceList);
=======
  // the defaults here are suitable for reading bundled fonts from omnijar
  void AppendFacesFromCachedFaceList(const nsCString& aFileName,
                                     const nsCString& aFaceList,
                                     StandardFile aStdFile = kStandard);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void FindFonts();
||||||| merged common ancestors
    void FindFonts();
=======
  void AddFaceToList(const nsCString& aEntryName, uint32_t aIndex,
                     StandardFile aStdFile, FT_Face aFace,
                     nsCString& aFaceList);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void FindFontsInOmnijar(FontNameCache* aCache);
||||||| merged common ancestors
    void FindFontsInOmnijar(FontNameCache *aCache);
=======
  void FindFonts();
>>>>>>> upstream-releases

<<<<<<< HEAD
  void FindFontsInDir(const nsCString& aDir, FontNameCache* aFNC);
||||||| merged common ancestors
    void FindFontsInDir(const nsCString& aDir, FontNameCache* aFNC);
=======
  void FindFontsInOmnijar(FontNameCache* aCache);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual gfxFontFamily* GetDefaultFontForPlatform(
      const gfxFontStyle* aStyle) override;
||||||| merged common ancestors
    virtual gfxFontFamily*
    GetDefaultFontForPlatform(const gfxFontStyle* aStyle) override;
=======
  void FindFontsInDir(const nsCString& aDir, FontNameCache* aFNC);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsTHashtable<nsCStringHashKey> mSkipSpaceLookupCheckFamilies;
||||||| merged common ancestors
    nsTHashtable<nsCStringHashKey> mSkipSpaceLookupCheckFamilies;

private:
    mozilla::UniquePtr<FontNameCache> mFontNameCache;
    int64_t mJarModifiedTime;
    RefPtr<WillShutdownObserver> mObserver;
=======
  FontFamily GetDefaultFontForPlatform(const gfxFontStyle* aStyle) override;

  nsTHashtable<nsCStringHashKey> mSkipSpaceLookupCheckFamilies;
>>>>>>> upstream-releases

 private:
  mozilla::UniquePtr<FontNameCache> mFontNameCache;
  int64_t mJarModifiedTime;
  RefPtr<WillShutdownObserver> mObserver;
};

#endif /* GFX_FT2FONTLIST_H */
