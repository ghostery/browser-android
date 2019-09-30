/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFXPLATFORMFONTLIST_H_
#define GFXPLATFORMFONTLIST_H_

#include "nsClassHashtable.h"
#include "nsDataHashtable.h"
#include "nsRefPtrHashtable.h"
#include "nsTHashtable.h"

#include "gfxFontUtils.h"
#include "gfxFontInfoLoader.h"
#include "gfxFont.h"
#include "gfxFontConstants.h"
#include "gfxPlatform.h"
#include "gfxFontFamilyList.h"
#include "SharedFontList.h"

#include "nsIMemoryReporter.h"
#include "mozilla/Attributes.h"
#include "mozilla/FontPropertyTypes.h"
#include "mozilla/MemoryReporting.h"
#include "mozilla/Mutex.h"
#include "mozilla/RangedArray.h"
#include "nsLanguageAtomService.h"

<<<<<<< HEAD
class CharMapHashKey : public PLDHashEntryHdr {
 public:
  typedef gfxCharacterMap* KeyType;
  typedef const gfxCharacterMap* KeyTypePointer;

  explicit CharMapHashKey(const gfxCharacterMap* aCharMap)
      : mCharMap(const_cast<gfxCharacterMap*>(aCharMap)) {
    MOZ_COUNT_CTOR(CharMapHashKey);
  }
  CharMapHashKey(const CharMapHashKey& toCopy) : mCharMap(toCopy.mCharMap) {
    MOZ_COUNT_CTOR(CharMapHashKey);
  }
  ~CharMapHashKey() { MOZ_COUNT_DTOR(CharMapHashKey); }

  gfxCharacterMap* GetKey() const { return mCharMap; }

  bool KeyEquals(const gfxCharacterMap* aCharMap) const {
    NS_ASSERTION(!aCharMap->mBuildOnTheFly && !mCharMap->mBuildOnTheFly,
                 "custom cmap used in shared cmap hashtable");
    // cmaps built on the fly never match
    if (aCharMap->mHash != mCharMap->mHash) {
      return false;
    }
    return mCharMap->Equals(aCharMap);
  }

  static const gfxCharacterMap* KeyToPointer(gfxCharacterMap* aCharMap) {
    return aCharMap;
  }
  static PLDHashNumber HashKey(const gfxCharacterMap* aCharMap) {
    return aCharMap->mHash;
  }

  enum { ALLOW_MEMMOVE = true };

 protected:
  // charMaps are not owned by the shared cmap cache, but it will be notified
  // by gfxCharacterMap::Release() when an entry is about to be deleted
  gfxCharacterMap* MOZ_NON_OWNING_REF mCharMap;
||||||| merged common ancestors
class CharMapHashKey : public PLDHashEntryHdr
{
public:
    typedef gfxCharacterMap* KeyType;
    typedef const gfxCharacterMap* KeyTypePointer;

    explicit CharMapHashKey(const gfxCharacterMap *aCharMap) :
        mCharMap(const_cast<gfxCharacterMap*>(aCharMap))
    {
        MOZ_COUNT_CTOR(CharMapHashKey);
    }
    CharMapHashKey(const CharMapHashKey& toCopy) :
        mCharMap(toCopy.mCharMap)
    {
        MOZ_COUNT_CTOR(CharMapHashKey);
    }
    ~CharMapHashKey()
    {
        MOZ_COUNT_DTOR(CharMapHashKey);
    }

    gfxCharacterMap* GetKey() const { return mCharMap; }

    bool KeyEquals(const gfxCharacterMap *aCharMap) const {
        NS_ASSERTION(!aCharMap->mBuildOnTheFly && !mCharMap->mBuildOnTheFly,
                     "custom cmap used in shared cmap hashtable");
        // cmaps built on the fly never match
        if (aCharMap->mHash != mCharMap->mHash)
        {
            return false;
        }
        return mCharMap->Equals(aCharMap);
    }

    static const gfxCharacterMap* KeyToPointer(gfxCharacterMap *aCharMap) {
        return aCharMap;
    }
    static PLDHashNumber HashKey(const gfxCharacterMap *aCharMap) {
        return aCharMap->mHash;
    }

    enum { ALLOW_MEMMOVE = true };

protected:
    // charMaps are not owned by the shared cmap cache, but it will be notified
    // by gfxCharacterMap::Release() when an entry is about to be deleted
    gfxCharacterMap* MOZ_NON_OWNING_REF mCharMap;
=======
class CharMapHashKey : public PLDHashEntryHdr {
 public:
  typedef gfxCharacterMap* KeyType;
  typedef const gfxCharacterMap* KeyTypePointer;

  explicit CharMapHashKey(const gfxCharacterMap* aCharMap)
      : mCharMap(const_cast<gfxCharacterMap*>(aCharMap)) {
    MOZ_COUNT_CTOR(CharMapHashKey);
  }
  CharMapHashKey(const CharMapHashKey& toCopy) : mCharMap(toCopy.mCharMap) {
    MOZ_COUNT_CTOR(CharMapHashKey);
  }
  ~CharMapHashKey() { MOZ_COUNT_DTOR(CharMapHashKey); }

  gfxCharacterMap* GetKey() const { return mCharMap; }

  bool KeyEquals(const gfxCharacterMap* aCharMap) const {
    NS_ASSERTION(!aCharMap->mBuildOnTheFly && !mCharMap->mBuildOnTheFly,
                 "custom cmap used in shared cmap hashtable");
    // cmaps built on the fly never match
    if (aCharMap->mHash != mCharMap->mHash) {
      return false;
    }
    return mCharMap->Equals(aCharMap);
  }

  static const gfxCharacterMap* KeyToPointer(gfxCharacterMap* aCharMap) {
    return aCharMap;
  }
  static PLDHashNumber HashKey(const gfxCharacterMap* aCharMap) {
    return aCharMap->mHash;
  }

  enum { ALLOW_MEMMOVE = true };

 protected:
  // charMaps are not owned by the shared cmap cache, but it will be notified
  // by gfxCharacterMap::Release() when an entry is about to be deleted
  gfxCharacterMap* MOZ_NON_OWNING_REF mCharMap;
};

/**
 * A helper class used to create a SharedBitSet instance in a FontList's shared
 * memory, while ensuring that we avoid bloating memory by avoiding creating
 * duplicate instances.
 */
class ShmemCharMapHashEntry final : public PLDHashEntryHdr {
 public:
  typedef const gfxSparseBitSet* KeyType;
  typedef const gfxSparseBitSet* KeyTypePointer;

  /**
   * Creation from a gfxSparseBitSet creates not only the ShmemCharMapHashEntry
   * itself, but also a SharedBitSet in shared memory.
   * Only the parent process creates and manages these entries.
   */
  explicit ShmemCharMapHashEntry(const gfxSparseBitSet* aCharMap);

  ShmemCharMapHashEntry(const ShmemCharMapHashEntry& aOther)
      : mList(aOther.mList), mCharMap(aOther.mCharMap), mHash(aOther.mHash) {}

  ~ShmemCharMapHashEntry() = default;

  /**
   * Return a shared-memory Pointer that refers to the wrapped SharedBitSet.
   * This can be passed to content processes to give them access to the same
   * SharedBitSet as the parent stored.
   */
  mozilla::fontlist::Pointer GetCharMap() const { return mCharMap; }

  bool KeyEquals(KeyType aCharMap) const {
    // mHash is a 32-bit Adler checksum of the bitset; if it doesn't match we
    // can immediately reject it as non-matching, but if it is equal we still
    // need to do a full equality check below.
    if (mHash != aCharMap->GetChecksum()) {
      return false;
    }

    return static_cast<const SharedBitSet*>(mCharMap.ToPtr(mList))
        ->Equals(aCharMap);
  }

  static KeyTypePointer KeyToPointer(KeyType aCharMap) { return aCharMap; }
  static PLDHashNumber HashKey(KeyType aCharMap) {
    return aCharMap->GetChecksum();
  }

  enum { ALLOW_MEMMOVE = true };

 private:
  // charMaps are stored in the shared memory that FontList objects point to,
  // and are never deleted until the FontList (all referencing font lists,
  // actually) have gone away.
  mozilla::fontlist::FontList* mList;
  mozilla::fontlist::Pointer mCharMap;
  uint32_t mHash;
>>>>>>> upstream-releases
};

// gfxPlatformFontList is an abstract class for the global font list on the
// system; concrete subclasses for each platform implement the actual interface
// to the system fonts. This class exists because we cannot rely on the platform
// font-finding APIs to behave in sensible/similar ways, particularly with rich,
// complex OpenType families, so we do our own font family/style management here
// instead.

// Much of this is based on the old gfxQuartzFontCache, but adapted for use on
// all platforms.

struct FontListSizes {
<<<<<<< HEAD
  uint32_t mFontListSize;  // size of the font list and dependent objects
                           // (font family and face names, etc), but NOT
                           // including the font table cache and the cmaps
  uint32_t
      mFontTableCacheSize;  // memory used for the gfxFontEntry table caches
  uint32_t mCharMapsSize;   // memory used for cmap coverage info
  uint32_t mLoaderSize;     // memory used for (platform-specific) loader
||||||| merged common ancestors
    uint32_t mFontListSize; // size of the font list and dependent objects
                            // (font family and face names, etc), but NOT
                            // including the font table cache and the cmaps
    uint32_t mFontTableCacheSize; // memory used for the gfxFontEntry table caches
    uint32_t mCharMapsSize; // memory used for cmap coverage info
    uint32_t mLoaderSize;   // memory used for (platform-specific) loader
=======
  uint32_t mFontListSize;  // size of the font list and dependent objects
                           // (font family and face names, etc), but NOT
                           // including the font table cache and the cmaps
  uint32_t
      mFontTableCacheSize;  // memory used for the gfxFontEntry table caches
  uint32_t mCharMapsSize;   // memory used for cmap coverage info
  uint32_t mLoaderSize;     // memory used for (platform-specific) loader
  uint32_t mSharedSize;     // shared-memory use (reported by parent only)
>>>>>>> upstream-releases
};

class gfxUserFontSet;

<<<<<<< HEAD
class gfxPlatformFontList : public gfxFontInfoLoader {
  friend class InitOtherFamilyNamesRunnable;

 public:
  typedef mozilla::StretchRange StretchRange;
  typedef mozilla::SlantStyleRange SlantStyleRange;
  typedef mozilla::WeightRange WeightRange;
  typedef mozilla::unicode::Script Script;
||||||| merged common ancestors
class gfxPlatformFontList : public gfxFontInfoLoader
{
    friend class InitOtherFamilyNamesRunnable;
=======
class gfxPlatformFontList : public gfxFontInfoLoader {
  friend class InitOtherFamilyNamesRunnable;
>>>>>>> upstream-releases

<<<<<<< HEAD
  static gfxPlatformFontList* PlatformFontList() { return sPlatformFontList; }
||||||| merged common ancestors
public:
    typedef mozilla::StretchRange StretchRange;
    typedef mozilla::SlantStyleRange SlantStyleRange;
    typedef mozilla::WeightRange WeightRange;
    typedef mozilla::unicode::Script Script;
=======
 public:
  typedef mozilla::StretchRange StretchRange;
  typedef mozilla::SlantStyleRange SlantStyleRange;
  typedef mozilla::WeightRange WeightRange;
  typedef mozilla::unicode::Script Script;
>>>>>>> upstream-releases

<<<<<<< HEAD
  static nsresult Init() {
    NS_ASSERTION(!sPlatformFontList, "What's this doing here?");
    gfxPlatform::GetPlatform()->CreatePlatformFontList();
    if (!sPlatformFontList) {
      return NS_ERROR_OUT_OF_MEMORY;
    }
    return NS_OK;
  }

  static void Shutdown() {
    delete sPlatformFontList;
    sPlatformFontList = nullptr;
  }

  virtual ~gfxPlatformFontList();

  // initialize font lists
  nsresult InitFontList();

  virtual void GetFontList(nsAtom* aLangGroup, const nsACString& aGenericFamily,
                           nsTArray<nsString>& aListOfFonts);

  void UpdateFontList();

  virtual void ClearLangGroupPrefFonts();

  virtual void GetFontFamilyList(nsTArray<RefPtr<gfxFontFamily>>& aFamilyArray);

  gfxFontEntry* SystemFindFontForChar(uint32_t aCh, uint32_t aNextCh,
                                      Script aRunScript,
                                      const gfxFontStyle* aStyle);

  // Flags to control optional behaviors in FindAndAddFamilies. The sense
  // of the bit flags have been chosen such that the default parameter of
  // FindFamiliesFlags(0) in FindFamily will give the most commonly-desired
  // behavior, and only a few callsites need to explicitly pass other values.
  enum class FindFamiliesFlags {
    // If set, "other" (e.g. localized) family names should be loaded
    // immediately; if clear, InitOtherFamilyNames is allowed to defer
    // loading to avoid blocking.
    eForceOtherFamilyNamesLoading = 1 << 0,

    // If set, FindAndAddFamilies should not check for legacy "styled
    // family" names to add to the font list. This is used to avoid
    // a recursive search when using FindFamily to find a potential base
    // family name for a styled variant.
    eNoSearchForLegacyFamilyNames = 1 << 1,

    // If set, FindAndAddFamilies will not add a missing entry to
    // mOtherNamesMissed
    eNoAddToNamesMissedWhenSearching = 1 << 2
  };

  // Find family(ies) matching aFamily and append to the aOutput array
  // (there may be multiple results in the case of fontconfig aliases, etc).
  // Return true if any match was found and appended, false if none.
  virtual bool FindAndAddFamilies(const nsACString& aFamily,
                                  nsTArray<FamilyAndGeneric>* aOutput,
                                  FindFamiliesFlags aFlags,
                                  gfxFontStyle* aStyle = nullptr,
                                  gfxFloat aDevToCssSize = 1.0);

  gfxFontEntry* FindFontForFamily(const nsACString& aFamily,
                                  const gfxFontStyle* aStyle);

  // name lookup table methods

  void AddOtherFamilyName(gfxFontFamily* aFamilyEntry,
                          nsCString& aOtherFamilyName);

  void AddFullname(gfxFontEntry* aFontEntry, const nsCString& aFullname);

  void AddPostscriptName(gfxFontEntry* aFontEntry,
                         const nsCString& aPostscriptName);

  bool NeedFullnamePostscriptNames() { return mExtraNames != nullptr; }

  // pure virtual functions, to be provided by concrete subclasses

  // get the system default font family
  gfxFontFamily* GetDefaultFont(const gfxFontStyle* aStyle);

  /**
   * Look up a font by name on the host platform.
   *
   * Note that the style attributes (weight, stretch, style) are NOT used in
   * selecting the platform font, which is looked up by name only; these are
   * values to be recorded in the new font entry.
   */
  virtual gfxFontEntry* LookupLocalFont(const nsACString& aFontName,
                                        WeightRange aWeightForEntry,
                                        StretchRange aStretchForEntry,
                                        SlantStyleRange aStyleForEntry) = 0;

  /**
   * Create a new platform font from downloaded data (@font-face).
   *
   * Note that the style attributes (weight, stretch, style) are NOT related
   * (necessarily) to any values within the font resource itself; these are
   * values to be recorded in the new font entry and used for face selection,
   * in place of whatever inherent style attributes the resource may have.
   *
   * This method takes ownership of the data block passed in as aFontData,
   * and must ensure it is free()'d when no longer required.
   */
  virtual gfxFontEntry* MakePlatformFont(const nsACString& aFontName,
                                         WeightRange aWeightForEntry,
                                         StretchRange aStretchForEntry,
                                         SlantStyleRange aStyleForEntry,
                                         const uint8_t* aFontData,
                                         uint32_t aLength) = 0;

  // get the standard family name on the platform for a given font name
  // (platforms may override, eg Mac)
  virtual bool GetStandardFamilyName(const nsCString& aFontName,
                                     nsACString& aFamilyName);

  // get the default font name which is available on the system from
  // font.name-list.*.  if there are no available fonts in the pref,
  // returns nullptr.
  gfxFontFamily* GetDefaultFontFamily(const nsACString& aLangGroup,
                                      const nsACString& aGenericFamily);

  virtual void AddSizeOfExcludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                      FontListSizes* aSizes) const;
  virtual void AddSizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                      FontListSizes* aSizes) const;

  // search for existing cmap that matches the input
  // return the input if no match is found
  gfxCharacterMap* FindCharMap(gfxCharacterMap* aCmap);

  // add a cmap to the shared cmap set
  gfxCharacterMap* AddCmap(const gfxCharacterMap* aCharMap);

  // remove the cmap from the shared cmap set
  void RemoveCmap(const gfxCharacterMap* aCharMap);

  // keep track of userfont sets to notify when global fontlist changes occur
  void AddUserFontSet(gfxUserFontSet* aUserFontSet) {
    mUserFontSetList.PutEntry(aUserFontSet);
  }

  void RemoveUserFontSet(gfxUserFontSet* aUserFontSet) {
    mUserFontSetList.RemoveEntry(aUserFontSet);
  }

  static const gfxFontEntry::ScriptRange sComplexScriptRanges[];

  void GetFontlistInitInfo(uint32_t& aNumInits, uint32_t& aLoaderState) {
    aNumInits = mFontlistInitCount;
    aLoaderState = (uint32_t)mState;
  }

  virtual void AddGenericFonts(mozilla::FontFamilyType aGenericType,
                               nsAtom* aLanguage,
                               nsTArray<FamilyAndGeneric>& aFamilyList);

  nsTArray<RefPtr<gfxFontFamily>>* GetPrefFontsLangGroup(
      mozilla::FontFamilyType aGenericType, eFontPrefLang aPrefLang);

  // in some situations, need to make decisions about ambiguous characters, may
  // need to look at multiple pref langs
  void GetLangPrefs(eFontPrefLang aPrefLangs[], uint32_t& aLen,
                    eFontPrefLang aCharLang, eFontPrefLang aPageLang);

  // convert a lang group to enum constant (i.e. "zh-TW" ==>
  // eFontPrefLang_ChineseTW)
  static eFontPrefLang GetFontPrefLangFor(const char* aLang);

  // convert a lang group atom to enum constant
  static eFontPrefLang GetFontPrefLangFor(nsAtom* aLang);

  // convert an enum constant to a lang group atom
  static nsAtom* GetLangGroupForPrefLang(eFontPrefLang aLang);

  // convert a enum constant to lang group string (i.e. eFontPrefLang_ChineseTW
  // ==> "zh-TW")
  static const char* GetPrefLangName(eFontPrefLang aLang);

  // map a Unicode range (based on char code) to a font language for Preferences
  static eFontPrefLang GetFontPrefLangFor(uint8_t aUnicodeRange);

  // returns true if a pref lang is CJK
  static bool IsLangCJK(eFontPrefLang aLang);

  // helper method to add a pref lang to an array, if not already in array
  static void AppendPrefLang(eFontPrefLang aPrefLangs[], uint32_t& aLen,
                             eFontPrefLang aAddLang);

  // default serif/sans-serif choice based on font.default.xxx prefs
  mozilla::FontFamilyType GetDefaultGeneric(eFontPrefLang aLang);

  // Returns true if the font family whitelist is not empty.
  bool IsFontFamilyWhitelistActive();

  static void FontWhitelistPrefChanged(const char* aPref, void* aClosure);

  bool AddWithLegacyFamilyName(const nsACString& aLegacyName,
                               gfxFontEntry* aFontEntry);

  static const char* GetGenericName(mozilla::FontFamilyType aGenericType);

 protected:
  class InitOtherFamilyNamesRunnable : public mozilla::CancelableRunnable {
   public:
    InitOtherFamilyNamesRunnable()
        : CancelableRunnable(
              "gfxPlatformFontList::InitOtherFamilyNamesRunnable"),
          mIsCanceled(false) {}
||||||| merged common ancestors
    static gfxPlatformFontList* PlatformFontList() {
        return sPlatformFontList;
    }
=======
  // For font family lists loaded from user preferences (prefs such as
  // font.name-list.<generic>.<langGroup>) that map CSS generics to
  // platform-specific font families.
  typedef nsTArray<FontFamily> PrefFontList;
>>>>>>> upstream-releases

<<<<<<< HEAD
    NS_IMETHOD Run() override {
      if (mIsCanceled) {
        return NS_OK;
      }
||||||| merged common ancestors
    static nsresult Init() {
        NS_ASSERTION(!sPlatformFontList, "What's this doing here?");
        gfxPlatform::GetPlatform()->CreatePlatformFontList();
        if (!sPlatformFontList) {
            return NS_ERROR_OUT_OF_MEMORY;
        }
        return NS_OK;
    }
=======
  static gfxPlatformFontList* PlatformFontList() { return sPlatformFontList; }
>>>>>>> upstream-releases

<<<<<<< HEAD
      gfxPlatformFontList* fontList = gfxPlatformFontList::PlatformFontList();
      if (!fontList) {
        return NS_OK;
      }
||||||| merged common ancestors
    static void Shutdown() {
        delete sPlatformFontList;
        sPlatformFontList = nullptr;
    }
=======
  static nsresult Init() {
    NS_ASSERTION(!sPlatformFontList, "What's this doing here?");
    gfxPlatform::GetPlatform()->CreatePlatformFontList();
    if (!sPlatformFontList) {
      return NS_ERROR_OUT_OF_MEMORY;
    }
    return NS_OK;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      fontList->InitOtherFamilyNamesInternal(true);
||||||| merged common ancestors
    virtual ~gfxPlatformFontList();

    // initialize font lists
    nsresult InitFontList();

    virtual void GetFontList(nsAtom *aLangGroup,
                             const nsACString& aGenericFamily,
                             nsTArray<nsString>& aListOfFonts);

    void UpdateFontList();

    virtual void ClearLangGroupPrefFonts();

    virtual void GetFontFamilyList(nsTArray<RefPtr<gfxFontFamily> >& aFamilyArray);

    gfxFontEntry*
    SystemFindFontForChar(uint32_t aCh, uint32_t aNextCh,
                          Script aRunScript,
                          const gfxFontStyle* aStyle);

    // Flags to control optional behaviors in FindAndAddFamilies. The sense
    // of the bit flags have been chosen such that the default parameter of
    // FindFamiliesFlags(0) in FindFamily will give the most commonly-desired
    // behavior, and only a few callsites need to explicitly pass other values.
    enum class FindFamiliesFlags {
        // If set, "other" (e.g. localized) family names should be loaded
        // immediately; if clear, InitOtherFamilyNames is allowed to defer
        // loading to avoid blocking.
        eForceOtherFamilyNamesLoading = 1 << 0,
        
        // If set, FindAndAddFamilies should not check for legacy "styled
        // family" names to add to the font list. This is used to avoid
        // a recursive search when using FindFamily to find a potential base
        // family name for a styled variant.
        eNoSearchForLegacyFamilyNames = 1 << 1,

        // If set, FindAndAddFamilies will not add a missing entry to mOtherNamesMissed
        eNoAddToNamesMissedWhenSearching = 1 << 2
    };

    // Find family(ies) matching aFamily and append to the aOutput array
    // (there may be multiple results in the case of fontconfig aliases, etc).
    // Return true if any match was found and appended, false if none.
    virtual bool
    FindAndAddFamilies(const nsACString& aFamily,
                       nsTArray<FamilyAndGeneric>* aOutput,
                       FindFamiliesFlags aFlags,
                       gfxFontStyle* aStyle = nullptr,
                       gfxFloat aDevToCssSize = 1.0);

    gfxFontEntry* FindFontForFamily(const nsACString& aFamily,
                                    const gfxFontStyle* aStyle);

    // name lookup table methods

    void AddOtherFamilyName(gfxFontFamily *aFamilyEntry, nsCString& aOtherFamilyName);

    void AddFullname(gfxFontEntry *aFontEntry, const nsCString& aFullname);

    void AddPostscriptName(gfxFontEntry *aFontEntry, const nsCString& aPostscriptName);

    bool NeedFullnamePostscriptNames() { return mExtraNames != nullptr; }

    // pure virtual functions, to be provided by concrete subclasses

    // get the system default font family
    gfxFontFamily* GetDefaultFont(const gfxFontStyle* aStyle);

    /**
     * Look up a font by name on the host platform.
     *
     * Note that the style attributes (weight, stretch, style) are NOT used in
     * selecting the platform font, which is looked up by name only; these are
     * values to be recorded in the new font entry.
     */
    virtual gfxFontEntry* LookupLocalFont(const nsACString& aFontName,
                                          WeightRange aWeightForEntry,
                                          StretchRange aStretchForEntry,
                                          SlantStyleRange aStyleForEntry) = 0;

    /**
     * Create a new platform font from downloaded data (@font-face).
     *
     * Note that the style attributes (weight, stretch, style) are NOT related
     * (necessarily) to any values within the font resource itself; these are
     * values to be recorded in the new font entry and used for face selection,
     * in place of whatever inherent style attributes the resource may have.
     *
     * This method takes ownership of the data block passed in as aFontData,
     * and must ensure it is free()'d when no longer required.
     */
    virtual gfxFontEntry* MakePlatformFont(const nsACString& aFontName,
                                           WeightRange aWeightForEntry,
                                           StretchRange aStretchForEntry,
                                           SlantStyleRange aStyleForEntry,
                                           const uint8_t* aFontData,
                                           uint32_t aLength) = 0;

    // get the standard family name on the platform for a given font name
    // (platforms may override, eg Mac)
    virtual bool GetStandardFamilyName(const nsCString& aFontName, nsACString& aFamilyName);

    // get the default font name which is available on the system from
    // font.name-list.*.  if there are no available fonts in the pref,
    // returns nullptr.
    gfxFontFamily* GetDefaultFontFamily(const nsACString& aLangGroup,
                                        const nsACString& aGenericFamily);

    virtual void AddSizeOfExcludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                        FontListSizes* aSizes) const;
    virtual void AddSizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                        FontListSizes* aSizes) const;

    // search for existing cmap that matches the input
    // return the input if no match is found
    gfxCharacterMap* FindCharMap(gfxCharacterMap *aCmap);

    // add a cmap to the shared cmap set
    gfxCharacterMap* AddCmap(const gfxCharacterMap *aCharMap);

    // remove the cmap from the shared cmap set
    void RemoveCmap(const gfxCharacterMap *aCharMap);

    // keep track of userfont sets to notify when global fontlist changes occur
    void AddUserFontSet(gfxUserFontSet *aUserFontSet) {
        mUserFontSetList.PutEntry(aUserFontSet);
    }
=======
  static void Shutdown() {
    delete sPlatformFontList;
    sPlatformFontList = nullptr;
  }

  virtual ~gfxPlatformFontList();

  // initialize font lists
  nsresult InitFontList();

  void FontListChanged();

  /**
   * Gathers (from a platform's underlying font system) the information needed
   * to initialize a fontlist::Family with its Face members.
   */
  virtual void GetFacesInitDataForFamily(
      const mozilla::fontlist::Family* aFamily,
      nsTArray<mozilla::fontlist::Face::InitData>& aFaces) const {}

  virtual void GetFontList(nsAtom* aLangGroup, const nsACString& aGenericFamily,
                           nsTArray<nsString>& aListOfFonts);

  void UpdateFontList();

  virtual void ClearLangGroupPrefFonts();

  virtual void GetFontFamilyList(nsTArray<RefPtr<gfxFontFamily>>& aFamilyArray);

  gfxFontEntry* SystemFindFontForChar(uint32_t aCh, uint32_t aNextCh,
                                      Script aRunScript,
                                      const gfxFontStyle* aStyle);

  // Flags to control optional behaviors in FindAndAddFamilies. The sense
  // of the bit flags have been chosen such that the default parameter of
  // FindFamiliesFlags(0) in FindFamily will give the most commonly-desired
  // behavior, and only a few callsites need to explicitly pass other values.
  enum class FindFamiliesFlags {
    // If set, "other" (e.g. localized) family names should be loaded
    // immediately; if clear, InitOtherFamilyNames is allowed to defer
    // loading to avoid blocking.
    eForceOtherFamilyNamesLoading = 1 << 0,

    // If set, FindAndAddFamilies should not check for legacy "styled
    // family" names to add to the font list. This is used to avoid
    // a recursive search when using FindFamily to find a potential base
    // family name for a styled variant.
    eNoSearchForLegacyFamilyNames = 1 << 1,

    // If set, FindAndAddFamilies will not add a missing entry to
    // mOtherNamesMissed
    eNoAddToNamesMissedWhenSearching = 1 << 2,

    // If set, the family name was quoted and so must not be treated as a CSS
    // generic.
    eQuotedFamilyName = 1 << 3
  };

  // Find family(ies) matching aFamily and append to the aOutput array
  // (there may be multiple results in the case of fontconfig aliases, etc).
  // Return true if any match was found and appended, false if none.
  virtual bool FindAndAddFamilies(mozilla::StyleGenericFontFamily aGeneric,
                                  const nsACString& aFamily,
                                  nsTArray<FamilyAndGeneric>* aOutput,
                                  FindFamiliesFlags aFlags,
                                  gfxFontStyle* aStyle = nullptr,
                                  gfxFloat aDevToCssSize = 1.0);

  gfxFontEntry* FindFontForFamily(const nsACString& aFamily,
                                  const gfxFontStyle* aStyle);

  mozilla::fontlist::FontList* SharedFontList() const {
    return mSharedFontList.get();
  }

  // The aPid and aOut parameters are declared here with generic types
  // because (on Windows) we cannot include the proper headers, as they
  // result in build failure due to (indirect) inclusion of windows.h
  // in generated bindings code.
  void ShareFontListShmBlockToProcess(
      uint32_t aGeneration, uint32_t aIndex, /*base::ProcessId*/ uint32_t aPid,
      /*mozilla::ipc::SharedMemoryBasic::Handle*/ void* aOut);

  void SetCharacterMap(uint32_t aGeneration,
                       const mozilla::fontlist::Pointer& aFacePtr,
                       const gfxSparseBitSet& aMap);

  void SetupFamilyCharMap(uint32_t aGeneration,
                          const mozilla::fontlist::Pointer& aFamilyPtr);

  MOZ_MUST_USE bool InitializeFamily(mozilla::fontlist::Family* aFamily);
  void InitializeFamily(uint32_t aGeneration, uint32_t aFamilyIndex);

  // name lookup table methods

  void AddOtherFamilyName(gfxFontFamily* aFamilyEntry,
                          nsCString& aOtherFamilyName);

  void AddFullname(gfxFontEntry* aFontEntry, const nsCString& aFullname);

  void AddPostscriptName(gfxFontEntry* aFontEntry,
                         const nsCString& aPostscriptName);

  bool NeedFullnamePostscriptNames() { return mExtraNames != nullptr; }

  /**
   * Read PSName and FullName of the given face, for src:local lookup,
   * returning true if actually implemented and succeeded.
   */
  virtual bool ReadFaceNames(mozilla::fontlist::Family* aFamily,
                             mozilla::fontlist::Face* aFace, nsCString& aPSName,
                             nsCString& aFullName) {
    return false;
  }

  // initialize localized family names
  void InitOtherFamilyNames(bool aDeferOtherFamilyNamesLoading);
  void InitOtherFamilyNames(uint32_t aGeneration, bool aDefer);

  // pure virtual functions, to be provided by concrete subclasses

  // get the system default font family
  FontFamily GetDefaultFont(const gfxFontStyle* aStyle);

  /**
   * Look up a font by name on the host platform.
   *
   * Note that the style attributes (weight, stretch, style) are NOT used in
   * selecting the platform font, which is looked up by name only; these are
   * values to be recorded in the new font entry.
   */
  virtual gfxFontEntry* LookupLocalFont(const nsACString& aFontName,
                                        WeightRange aWeightForEntry,
                                        StretchRange aStretchForEntry,
                                        SlantStyleRange aStyleForEntry) = 0;

  /**
   * Create a new platform font from downloaded data (@font-face).
   *
   * Note that the style attributes (weight, stretch, style) are NOT related
   * (necessarily) to any values within the font resource itself; these are
   * values to be recorded in the new font entry and used for face selection,
   * in place of whatever inherent style attributes the resource may have.
   *
   * This method takes ownership of the data block passed in as aFontData,
   * and must ensure it is free()'d when no longer required.
   */
  virtual gfxFontEntry* MakePlatformFont(const nsACString& aFontName,
                                         WeightRange aWeightForEntry,
                                         StretchRange aStretchForEntry,
                                         SlantStyleRange aStyleForEntry,
                                         const uint8_t* aFontData,
                                         uint32_t aLength) = 0;

  // get the standard family name on the platform for a given font name
  // (platforms may override, eg Mac)
  virtual bool GetStandardFamilyName(const nsCString& aFontName,
                                     nsACString& aFamilyName);

  // get the default font name which is available on the system from
  // font.name-list.*.  if there are no available fonts in the pref,
  // returns an empty FamilyAndGeneric record.
  FamilyAndGeneric GetDefaultFontFamily(const nsACString& aLangGroup,
                                        const nsACString& aGenericFamily);

  virtual void AddSizeOfExcludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                      FontListSizes* aSizes) const;
  virtual void AddSizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf,
                                      FontListSizes* aSizes) const;

  mozilla::fontlist::Pointer GetShmemCharMap(const gfxSparseBitSet* aCmap);

  // search for existing cmap that matches the input
  // return the input if no match is found
  gfxCharacterMap* FindCharMap(gfxCharacterMap* aCmap);

  // add a cmap to the shared cmap set
  gfxCharacterMap* AddCmap(const gfxCharacterMap* aCharMap);

  // remove the cmap from the shared cmap set
  void RemoveCmap(const gfxCharacterMap* aCharMap);
>>>>>>> upstream-releases

<<<<<<< HEAD
      return NS_OK;
    }
||||||| merged common ancestors
    void RemoveUserFontSet(gfxUserFontSet *aUserFontSet) {
        mUserFontSetList.RemoveEntry(aUserFontSet);
    }
=======
  // keep track of userfont sets to notify when global fontlist changes occur
  void AddUserFontSet(gfxUserFontSet* aUserFontSet) {
    mUserFontSetList.PutEntry(aUserFontSet);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    virtual nsresult Cancel() override {
      mIsCanceled = true;
||||||| merged common ancestors
    static const gfxFontEntry::ScriptRange sComplexScriptRanges[];
=======
  void RemoveUserFontSet(gfxUserFontSet* aUserFontSet) {
    mUserFontSetList.RemoveEntry(aUserFontSet);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      return NS_OK;
    }
||||||| merged common ancestors
    void GetFontlistInitInfo(uint32_t& aNumInits, uint32_t& aLoaderState) {
        aNumInits = mFontlistInitCount;
        aLoaderState = (uint32_t) mState;
    }
=======
  static const gfxFontEntry::ScriptRange sComplexScriptRanges[];
>>>>>>> upstream-releases

<<<<<<< HEAD
   private:
    bool mIsCanceled;
  };

  class MemoryReporter final : public nsIMemoryReporter {
    ~MemoryReporter() {}

   public:
    NS_DECL_ISUPPORTS
    NS_DECL_NSIMEMORYREPORTER
  };

  template <bool ForNameList>
  class PrefNameMaker final : public nsAutoCString {
    void Init(const nsACString& aGeneric, const nsACString& aLangGroup) {
      if (ForNameList) {
        AssignLiteral("font.name-list.");
      } else {
        AssignLiteral("font.name.");
      }
      Append(aGeneric);
      if (!aLangGroup.IsEmpty()) {
        Append('.');
        Append(aLangGroup);
      }
    }
||||||| merged common ancestors
    virtual void
    AddGenericFonts(mozilla::FontFamilyType aGenericType,
                    nsAtom* aLanguage,
                    nsTArray<FamilyAndGeneric>& aFamilyList);

    nsTArray<RefPtr<gfxFontFamily>>*
    GetPrefFontsLangGroup(mozilla::FontFamilyType aGenericType,
                          eFontPrefLang aPrefLang);

    // in some situations, need to make decisions about ambiguous characters, may need to look at multiple pref langs
    void GetLangPrefs(eFontPrefLang aPrefLangs[], uint32_t &aLen, eFontPrefLang aCharLang, eFontPrefLang aPageLang);

    // convert a lang group to enum constant (i.e. "zh-TW" ==> eFontPrefLang_ChineseTW)
    static eFontPrefLang GetFontPrefLangFor(const char* aLang);

    // convert a lang group atom to enum constant
    static eFontPrefLang GetFontPrefLangFor(nsAtom *aLang);

    // convert an enum constant to a lang group atom
    static nsAtom* GetLangGroupForPrefLang(eFontPrefLang aLang);

    // convert a enum constant to lang group string (i.e. eFontPrefLang_ChineseTW ==> "zh-TW")
    static const char* GetPrefLangName(eFontPrefLang aLang);

    // map a Unicode range (based on char code) to a font language for Preferences
    static eFontPrefLang GetFontPrefLangFor(uint8_t aUnicodeRange);

    // returns true if a pref lang is CJK
    static bool IsLangCJK(eFontPrefLang aLang);

    // helper method to add a pref lang to an array, if not already in array
    static void AppendPrefLang(eFontPrefLang aPrefLangs[], uint32_t& aLen, eFontPrefLang aAddLang);

    // default serif/sans-serif choice based on font.default.xxx prefs
    mozilla::FontFamilyType
    GetDefaultGeneric(eFontPrefLang aLang);

    // Returns true if the font family whitelist is not empty.
    bool IsFontFamilyWhitelistActive();

    static void FontWhitelistPrefChanged(const char *aPref, void *aClosure);

    bool AddWithLegacyFamilyName(const nsACString& aLegacyName,
                                 gfxFontEntry* aFontEntry);

    static const char* GetGenericName(mozilla::FontFamilyType aGenericType);

protected:
    class InitOtherFamilyNamesRunnable : public mozilla::CancelableRunnable
    {
    public:
        InitOtherFamilyNamesRunnable()
            : CancelableRunnable("gfxPlatformFontList::InitOtherFamilyNamesRunnable")
            , mIsCanceled(false)
        {
        }

        NS_IMETHOD Run() override
        {
            if (mIsCanceled) {
                return NS_OK;
            }

            gfxPlatformFontList* fontList = gfxPlatformFontList::PlatformFontList();
            if (!fontList) {
                return NS_OK;
            }

            fontList->InitOtherFamilyNamesInternal(true);

            return NS_OK;
        }

        virtual nsresult Cancel() override
        {
            mIsCanceled = true;

            return NS_OK;
        }

    private:
        bool mIsCanceled;
    };

    class MemoryReporter final : public nsIMemoryReporter
    {
        ~MemoryReporter() {}
    public:
        NS_DECL_ISUPPORTS
        NS_DECL_NSIMEMORYREPORTER
    };

    template<bool ForNameList>
    class PrefNameMaker final : public nsAutoCString
    {
        void Init(const nsACString& aGeneric, const nsACString& aLangGroup)
        {
            if (ForNameList) {
                AssignLiteral("font.name-list.");
            } else {
                AssignLiteral("font.name.");
            }
            Append(aGeneric);
            if (!aLangGroup.IsEmpty()) {
                Append('.');
                Append(aLangGroup);
            }
        }

    public:
        PrefNameMaker(const nsACString& aGeneric,
                      const nsACString& aLangGroup)
        {
            Init(aGeneric, aLangGroup);
        }

        PrefNameMaker(const char* aGeneric,
                      const char* aLangGroup)
        {
            Init(nsDependentCString(aGeneric), nsDependentCString(aLangGroup));
        }

        PrefNameMaker(const char* aGeneric,
                      nsAtom* aLangGroup)
        {
            if (aLangGroup) {
                Init(nsDependentCString(aGeneric), nsAtomCString(aLangGroup));
            } else {
                Init(nsDependentCString(aGeneric), nsAutoCString());
            }
        }
    };

    typedef PrefNameMaker<false> NamePref;
    typedef PrefNameMaker<true>  NameListPref;

    explicit gfxPlatformFontList(bool aNeedFullnamePostscriptNames = true);

    static gfxPlatformFontList *sPlatformFontList;

    // Convenience method to return the first matching family (if any) as found
    // by FindAndAddFamilies().
    gfxFontFamily*
    FindFamily(const nsACString& aFamily,
               FindFamiliesFlags aFlags = FindFamiliesFlags(0),
               gfxFontStyle* aStyle = nullptr,
               gfxFloat aDevToCssSize = 1.0)
    {
        AutoTArray<FamilyAndGeneric,1> families;
        return FindAndAddFamilies(aFamily,
                                  &families,
                                  aFlags,
                                  aStyle,
                                  aDevToCssSize)
               ? families[0].mFamily : nullptr;
    }
=======
  void GetFontlistInitInfo(uint32_t& aNumInits, uint32_t& aLoaderState) {
    aNumInits = mFontlistInitCount;
    aLoaderState = (uint32_t)mState;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
   public:
    PrefNameMaker(const nsACString& aGeneric, const nsACString& aLangGroup) {
      Init(aGeneric, aLangGroup);
    }
||||||| merged common ancestors
    // Lookup family name in global family list without substitutions or
    // localized family name lookup. Used for common font fallback families.
    gfxFontFamily* FindFamilyByCanonicalName(const nsACString& aFamily) {
        nsAutoCString key;
        gfxFontFamily *familyEntry;
        GenerateFontListKey(aFamily, key);
        if ((familyEntry = mFontFamilies.GetWeak(key))) {
            return CheckFamily(familyEntry);
        }
        return nullptr;
    }
=======
  virtual void AddGenericFonts(mozilla::StyleGenericFontFamily aGenericType,
                               nsAtom* aLanguage,
                               nsTArray<FamilyAndGeneric>& aFamilyList);
>>>>>>> upstream-releases

<<<<<<< HEAD
    PrefNameMaker(const char* aGeneric, const char* aLangGroup) {
      Init(nsDependentCString(aGeneric), nsDependentCString(aLangGroup));
    }
||||||| merged common ancestors
    // returns default font for a given character, null otherwise
    gfxFontEntry* CommonFontFallback(uint32_t aCh, uint32_t aNextCh,
                                     Script aRunScript,
                                     const gfxFontStyle* aMatchStyle,
                                     gfxFontFamily** aMatchedFamily);

    // Search fonts system-wide for a given character, null if not found.
    gfxFontEntry* GlobalFontFallback(const uint32_t aCh,
                                     Script aRunScript,
                                     const gfxFontStyle* aMatchStyle,
                                     uint32_t& aCmapCount,
                                     gfxFontFamily** aMatchedFamily);

    // Platform-specific implementation of global font fallback, if any;
    // this may return nullptr in which case the default cmap-based fallback
    // will be performed.
    virtual gfxFontEntry*
    PlatformGlobalFontFallback(const uint32_t aCh,
                               Script aRunScript,
                               const gfxFontStyle* aMatchStyle,
                               gfxFontFamily** aMatchedFamily)
    {
        return nullptr;
    }
=======
  /**
   * Given a Face from the shared font list, return a gfxFontEntry usable
   * by the current process. This returns a cached entry if available,
   * otherwise it calls the (platform-specific) CreateFontEntry method to
   * make one, and adds it to the cache.
   */
  gfxFontEntry* GetOrCreateFontEntry(mozilla::fontlist::Face* aFace,
                                     const mozilla::fontlist::Family* aFamily);

  PrefFontList* GetPrefFontsLangGroup(
      mozilla::StyleGenericFontFamily aGenericType, eFontPrefLang aPrefLang);

  // in some situations, need to make decisions about ambiguous characters, may
  // need to look at multiple pref langs
  void GetLangPrefs(eFontPrefLang aPrefLangs[], uint32_t& aLen,
                    eFontPrefLang aCharLang, eFontPrefLang aPageLang);
>>>>>>> upstream-releases

<<<<<<< HEAD
    PrefNameMaker(const char* aGeneric, nsAtom* aLangGroup) {
      if (aLangGroup) {
        Init(nsDependentCString(aGeneric), nsAtomCString(aLangGroup));
      } else {
        Init(nsDependentCString(aGeneric), nsAutoCString());
      }
    }
  };

  typedef PrefNameMaker<false> NamePref;
  typedef PrefNameMaker<true> NameListPref;

  explicit gfxPlatformFontList(bool aNeedFullnamePostscriptNames = true);

  static gfxPlatformFontList* sPlatformFontList;

  // Convenience method to return the first matching family (if any) as found
  // by FindAndAddFamilies().
  gfxFontFamily* FindFamily(const nsACString& aFamily,
                            FindFamiliesFlags aFlags = FindFamiliesFlags(0),
                            gfxFontStyle* aStyle = nullptr,
                            gfxFloat aDevToCssSize = 1.0) {
    AutoTArray<FamilyAndGeneric, 1> families;
    return FindAndAddFamilies(aFamily, &families, aFlags, aStyle, aDevToCssSize)
               ? families[0].mFamily
               : nullptr;
  }

  // Lookup family name in global family list without substitutions or
  // localized family name lookup. Used for common font fallback families.
  gfxFontFamily* FindFamilyByCanonicalName(const nsACString& aFamily) {
    nsAutoCString key;
    gfxFontFamily* familyEntry;
    GenerateFontListKey(aFamily, key);
    if ((familyEntry = mFontFamilies.GetWeak(key))) {
      return CheckFamily(familyEntry);
    }
    return nullptr;
  }
||||||| merged common ancestors
    // whether system-based font fallback is used or not
    // if system fallback is used, no need to load all cmaps
    virtual bool UsesSystemFallback() { return false; }
=======
  // convert a lang group to enum constant (i.e. "zh-TW" ==>
  // eFontPrefLang_ChineseTW)
  static eFontPrefLang GetFontPrefLangFor(const char* aLang);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // returns default font for a given character, null otherwise
  gfxFontEntry* CommonFontFallback(uint32_t aCh, uint32_t aNextCh,
                                   Script aRunScript,
                                   const gfxFontStyle* aMatchStyle,
                                   gfxFontFamily** aMatchedFamily);
||||||| merged common ancestors
    void AppendCJKPrefLangs(eFontPrefLang aPrefLangs[], uint32_t &aLen,
                            eFontPrefLang aCharLang, eFontPrefLang aPageLang);
=======
  // convert a lang group atom to enum constant
  static eFontPrefLang GetFontPrefLangFor(nsAtom* aLang);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Search fonts system-wide for a given character, null if not found.
  gfxFontEntry* GlobalFontFallback(const uint32_t aCh, Script aRunScript,
                                   const gfxFontStyle* aMatchStyle,
                                   uint32_t& aCmapCount,
                                   gfxFontFamily** aMatchedFamily);
||||||| merged common ancestors
    // verifies that a family contains a non-zero font count
    gfxFontFamily* CheckFamily(gfxFontFamily *aFamily);
=======
  // convert an enum constant to a lang group atom
  static nsAtom* GetLangGroupForPrefLang(eFontPrefLang aLang);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Platform-specific implementation of global font fallback, if any;
  // this may return nullptr in which case the default cmap-based fallback
  // will be performed.
  virtual gfxFontEntry* PlatformGlobalFontFallback(
      const uint32_t aCh, Script aRunScript, const gfxFontStyle* aMatchStyle,
      gfxFontFamily** aMatchedFamily) {
    return nullptr;
  }
||||||| merged common ancestors
    // initialize localized family names
    void InitOtherFamilyNames(bool aDeferOtherFamilyNamesLoading);
    void InitOtherFamilyNamesInternal(bool aDeferOtherFamilyNamesLoading);
    void CancelInitOtherFamilyNamesTask();
=======
  // convert a enum constant to lang group string (i.e. eFontPrefLang_ChineseTW
  // ==> "zh-TW")
  static const char* GetPrefLangName(eFontPrefLang aLang);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // whether system-based font fallback is used or not
  // if system fallback is used, no need to load all cmaps
  virtual bool UsesSystemFallback() { return false; }
||||||| merged common ancestors
    // search through font families, looking for a given name, initializing
    // facename lists along the way. first checks all families with names
    // close to face name, then searchs all families if not found.
    gfxFontEntry* SearchFamiliesForFaceName(const nsACString& aFaceName);
=======
  // map a char code to a font language for Preferences
  static eFontPrefLang GetFontPrefLangFor(uint32_t aCh);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void AppendCJKPrefLangs(eFontPrefLang aPrefLangs[], uint32_t& aLen,
                          eFontPrefLang aCharLang, eFontPrefLang aPageLang);
||||||| merged common ancestors
    // helper method for finding fullname/postscript names in facename lists
    gfxFontEntry* FindFaceName(const nsACString& aFaceName);
=======
  // returns true if a pref lang is CJK
  static bool IsLangCJK(eFontPrefLang aLang);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // verifies that a family contains a non-zero font count
  gfxFontFamily* CheckFamily(gfxFontFamily* aFamily);
||||||| merged common ancestors
    // look up a font by name, for cases where platform font list
    // maintains explicit mappings of fullname/psname ==> font
    virtual gfxFontEntry* LookupInFaceNameLists(const nsACString& aFontName);
=======
  // helper method to add a pref lang to an array, if not already in array
  static void AppendPrefLang(eFontPrefLang aPrefLangs[], uint32_t& aLen,
                             eFontPrefLang aAddLang);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // initialize localized family names
  void InitOtherFamilyNames(bool aDeferOtherFamilyNamesLoading);
  void InitOtherFamilyNamesInternal(bool aDeferOtherFamilyNamesLoading);
  void CancelInitOtherFamilyNamesTask();
||||||| merged common ancestors
    // commonly used fonts for which the name table should be loaded at startup
    virtual void PreloadNamesList();
=======
  // default serif/sans-serif choice based on font.default.xxx prefs
  mozilla::StyleGenericFontFamily GetDefaultGeneric(eFontPrefLang aLang);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // search through font families, looking for a given name, initializing
  // facename lists along the way. first checks all families with names
  // close to face name, then searchs all families if not found.
  gfxFontEntry* SearchFamiliesForFaceName(const nsACString& aFaceName);
||||||| merged common ancestors
    // load the bad underline blacklist from pref.
    void LoadBadUnderlineList();
=======
  // Returns true if the font family whitelist is not empty.
  bool IsFontFamilyWhitelistActive();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // helper method for finding fullname/postscript names in facename lists
  gfxFontEntry* FindFaceName(const nsACString& aFaceName);
||||||| merged common ancestors
    void GenerateFontListKey(const nsACString& aKeyName, nsACString& aResult);
=======
  static void FontWhitelistPrefChanged(const char* aPref, void* aClosure);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // look up a font by name, for cases where platform font list
  // maintains explicit mappings of fullname/psname ==> font
  virtual gfxFontEntry* LookupInFaceNameLists(const nsACString& aFontName);
||||||| merged common ancestors
    virtual void GetFontFamilyNames(nsTArray<nsCString>& aFontFamilyNames);
=======
  bool AddWithLegacyFamilyName(const nsACString& aLegacyName,
                               gfxFontEntry* aFontEntry);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // commonly used fonts for which the name table should be loaded at startup
  virtual void PreloadNamesList();
||||||| merged common ancestors
    // helper function to map lang to lang group
    nsAtom* GetLangGroup(nsAtom* aLanguage);
=======
  static const char* GetGenericName(
      mozilla::StyleGenericFontFamily aGenericType);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // load the bad underline blacklist from pref.
  void LoadBadUnderlineList();
||||||| merged common ancestors
    // gfxFontInfoLoader overrides, used to load in font cmaps
    virtual void InitLoader() override;
    virtual bool LoadFontInfo() override;
    virtual void CleanupLoader() override;
=======
 protected:
  class InitOtherFamilyNamesRunnable : public mozilla::CancelableRunnable {
   public:
    InitOtherFamilyNamesRunnable()
        : CancelableRunnable(
              "gfxPlatformFontList::InitOtherFamilyNamesRunnable"),
          mIsCanceled(false) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  void GenerateFontListKey(const nsACString& aKeyName, nsACString& aResult);
||||||| merged common ancestors
    // read the loader initialization prefs, and start it
    void GetPrefsAndStartLoader();
=======
    NS_IMETHOD Run() override {
      if (mIsCanceled) {
        return NS_OK;
      }

      gfxPlatformFontList* fontList = gfxPlatformFontList::PlatformFontList();
      if (!fontList) {
        return NS_OK;
      }

      fontList->InitOtherFamilyNamesInternal(true);

      return NS_OK;
    }

    nsresult Cancel() override {
      mIsCanceled = true;

      return NS_OK;
    }

   private:
    bool mIsCanceled;
  };

  class MemoryReporter final : public nsIMemoryReporter {
    ~MemoryReporter() {}

   public:
    NS_DECL_ISUPPORTS
    NS_DECL_NSIMEMORYREPORTER
  };

  template <bool ForNameList>
  class PrefNameMaker final : public nsAutoCString {
    void Init(const nsACString& aGeneric, const nsACString& aLangGroup) {
      if (ForNameList) {
        AssignLiteral("font.name-list.");
      } else {
        AssignLiteral("font.name.");
      }
      Append(aGeneric);
      if (!aLangGroup.IsEmpty()) {
        Append('.');
        Append(aLangGroup);
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void GetFontFamilyNames(nsTArray<nsCString>& aFontFamilyNames);

  // helper function to map lang to lang group
  nsAtom* GetLangGroup(nsAtom* aLanguage);

  // gfxFontInfoLoader overrides, used to load in font cmaps
  virtual void InitLoader() override;
  virtual bool LoadFontInfo() override;
  virtual void CleanupLoader() override;

  // read the loader initialization prefs, and start it
  void GetPrefsAndStartLoader();

  // for font list changes that affect all documents
  void ForceGlobalReflow() { gfxPlatform::ForceGlobalReflow(); }
||||||| merged common ancestors
    // for font list changes that affect all documents
    void ForceGlobalReflow() {
        gfxPlatform::ForceGlobalReflow();
    }
=======
   public:
    PrefNameMaker(const nsACString& aGeneric, const nsACString& aLangGroup) {
      Init(aGeneric, aLangGroup);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void RebuildLocalFonts();
||||||| merged common ancestors
    void RebuildLocalFonts();
=======
    PrefNameMaker(const char* aGeneric, const char* aLangGroup) {
      Init(nsDependentCString(aGeneric), nsDependentCString(aLangGroup));
    }

    PrefNameMaker(const char* aGeneric, nsAtom* aLangGroup) {
      if (aLangGroup) {
        Init(nsDependentCString(aGeneric), nsAtomCString(aLangGroup));
      } else {
        Init(nsDependentCString(aGeneric), nsAutoCString());
      }
    }
  };

  typedef PrefNameMaker<false> NamePref;
  typedef PrefNameMaker<true> NameListPref;

  explicit gfxPlatformFontList(bool aNeedFullnamePostscriptNames = true);

  static gfxPlatformFontList* sPlatformFontList;

  /**
   * Convenience method to return the first matching family (if any) as found
   * by FindAndAddFamilies(). The family will be initialized (synchronously)
   * if this has not already been done, so the returned pointer, if non-null,
   * is ready for use.
   */
  mozilla::fontlist::Family* FindSharedFamily(
      const nsACString& aFamily,
      FindFamiliesFlags aFlags = FindFamiliesFlags(0),
      gfxFontStyle* aStyle = nullptr, gfxFloat aDevToCssSize = 1.0);

  gfxFontFamily* FindUnsharedFamily(
      const nsACString& aFamily,
      FindFamiliesFlags aFlags = FindFamiliesFlags(0),
      gfxFontStyle* aStyle = nullptr, gfxFloat aDevToCssSize = 1.0) {
    if (SharedFontList()) {
      return nullptr;
    }
    AutoTArray<FamilyAndGeneric, 1> families;
    if (FindAndAddFamilies(mozilla::StyleGenericFontFamily::None, aFamily,
                           &families, aFlags, aStyle, aDevToCssSize)) {
      return families[0].mFamily.mUnshared;
    }
    return nullptr;
  }

  FontFamily FindFamily(const nsACString& aFamily,
                        FindFamiliesFlags aFlags = FindFamiliesFlags(0),
                        gfxFontStyle* aStyle = nullptr,
                        gfxFloat aDevToCssSize = 1.0) {
    if (SharedFontList()) {
      return FontFamily(
          FindSharedFamily(aFamily, aFlags, aStyle, aDevToCssSize));
    }
    return FontFamily(
        FindUnsharedFamily(aFamily, aFlags, aStyle, aDevToCssSize));
  }

  // Lookup family name in global family list without substitutions or
  // localized family name lookup. Used for common font fallback families.
  gfxFontFamily* FindFamilyByCanonicalName(const nsACString& aFamily) {
    nsAutoCString key;
    gfxFontFamily* familyEntry;
    GenerateFontListKey(aFamily, key);
    if ((familyEntry = mFontFamilies.GetWeak(key))) {
      return CheckFamily(familyEntry);
    }
    return nullptr;
  }

  // returns default font for a given character, null otherwise
  gfxFontEntry* CommonFontFallback(uint32_t aCh, uint32_t aNextCh,
                                   Script aRunScript,
                                   const gfxFontStyle* aMatchStyle,
                                   FontFamily* aMatchedFamily);

  // Search fonts system-wide for a given character, null if not found.
  gfxFontEntry* GlobalFontFallback(const uint32_t aCh, Script aRunScript,
                                   const gfxFontStyle* aMatchStyle,
                                   uint32_t& aCmapCount,
                                   FontFamily* aMatchedFamily);

  // Platform-specific implementation of global font fallback, if any;
  // this may return nullptr in which case the default cmap-based fallback
  // will be performed.
  virtual gfxFontEntry* PlatformGlobalFontFallback(
      const uint32_t aCh, Script aRunScript, const gfxFontStyle* aMatchStyle,
      FontFamily* aMatchedFamily) {
    return nullptr;
  }

  // whether system-based font fallback is used or not
  // if system fallback is used, no need to load all cmaps
  virtual bool UsesSystemFallback() { return false; }

  void AppendCJKPrefLangs(eFontPrefLang aPrefLangs[], uint32_t& aLen,
                          eFontPrefLang aCharLang, eFontPrefLang aPageLang);

  // verifies that a family contains a non-zero font count
  gfxFontFamily* CheckFamily(gfxFontFamily* aFamily);

  // initialize localized family names
  void InitOtherFamilyNamesInternal(bool aDeferOtherFamilyNamesLoading);
  void CancelInitOtherFamilyNamesTask();

  // search through font families, looking for a given name, initializing
  // facename lists along the way. first checks all families with names
  // close to face name, then searchs all families if not found.
  gfxFontEntry* SearchFamiliesForFaceName(const nsACString& aFaceName);

  // helper method for finding fullname/postscript names in facename lists
  gfxFontEntry* FindFaceName(const nsACString& aFaceName);

  // look up a font by name, for cases where platform font list
  // maintains explicit mappings of fullname/psname ==> font
  virtual gfxFontEntry* LookupInFaceNameLists(const nsACString& aFontName);

  gfxFontEntry* LookupInSharedFaceNameList(const nsACString& aFaceName,
                                           WeightRange aWeightForEntry,
                                           StretchRange aStretchForEntry,
                                           SlantStyleRange aStyleForEntry);

  // commonly used fonts for which the name table should be loaded at startup
  virtual void PreloadNamesList();

  // load the bad underline blacklist from pref.
  void LoadBadUnderlineList();
>>>>>>> upstream-releases

<<<<<<< HEAD
  void ResolveGenericFontNames(
      mozilla::FontFamilyType aGenericType, eFontPrefLang aPrefLang,
      nsTArray<RefPtr<gfxFontFamily>>* aGenericFamilies);
||||||| merged common ancestors
    void
    ResolveGenericFontNames(mozilla::FontFamilyType aGenericType,
                            eFontPrefLang aPrefLang,
                            nsTArray<RefPtr<gfxFontFamily>>* aGenericFamilies);
=======
  void GenerateFontListKey(const nsACString& aKeyName, nsACString& aResult);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void ResolveEmojiFontNames(nsTArray<RefPtr<gfxFontFamily>>* aGenericFamilies);
||||||| merged common ancestors
    void
    ResolveEmojiFontNames(nsTArray<RefPtr<gfxFontFamily>>* aGenericFamilies);
=======
  virtual void GetFontFamilyNames(nsTArray<nsCString>& aFontFamilyNames);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void GetFontFamiliesFromGenericFamilies(
      nsTArray<nsCString>& aGenericFamilies, nsAtom* aLangGroup,
      nsTArray<RefPtr<gfxFontFamily>>* aFontFamilies);
||||||| merged common ancestors
    void
    GetFontFamiliesFromGenericFamilies(
        nsTArray<nsCString>& aGenericFamilies,
        nsAtom* aLangGroup,
        nsTArray<RefPtr<gfxFontFamily>>* aFontFamilies);
=======
  // helper function to map lang to lang group
  nsAtom* GetLangGroup(nsAtom* aLanguage);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual nsresult InitFontListForPlatform() = 0;
||||||| merged common ancestors
    virtual nsresult InitFontListForPlatform() = 0;
=======
  // gfxFontInfoLoader overrides, used to load in font cmaps
  void InitLoader() override;
  bool LoadFontInfo() override;
  void CleanupLoader() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void ApplyWhitelist();
||||||| merged common ancestors
    void ApplyWhitelist();
=======
  // read the loader initialization prefs, and start it
  void GetPrefsAndStartLoader();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Create a new gfxFontFamily of the appropriate subclass for the platform,
  // used when AddWithLegacyFamilyName needs to create a new family.
  virtual gfxFontFamily* CreateFontFamily(const nsACString& aName) const = 0;
||||||| merged common ancestors
    // Create a new gfxFontFamily of the appropriate subclass for the platform,
    // used when AddWithLegacyFamilyName needs to create a new family.
    virtual gfxFontFamily* CreateFontFamily(const nsACString& aName) const = 0;
=======
  // for font list changes that affect all documents
  void ForceGlobalReflow();
>>>>>>> upstream-releases

<<<<<<< HEAD
  typedef nsRefPtrHashtable<nsCStringHashKey, gfxFontFamily> FontFamilyTable;
  typedef nsRefPtrHashtable<nsCStringHashKey, gfxFontEntry> FontEntryTable;
||||||| merged common ancestors
    typedef nsRefPtrHashtable<nsCStringHashKey, gfxFontFamily> FontFamilyTable;
    typedef nsRefPtrHashtable<nsCStringHashKey, gfxFontEntry> FontEntryTable;
=======
  void RebuildLocalFonts();

  void ResolveGenericFontNames(mozilla::StyleGenericFontFamily aGenericType,
                               eFontPrefLang aPrefLang,
                               PrefFontList* aGenericFamilies);

  void ResolveEmojiFontNames(PrefFontList* aGenericFamilies);

  void GetFontFamiliesFromGenericFamilies(
      mozilla::StyleGenericFontFamily aGenericType,
      nsTArray<nsCString>& aGenericNameFamilies, nsAtom* aLangGroup,
      PrefFontList* aFontFamilies);

  virtual nsresult InitFontListForPlatform() = 0;
  virtual void InitSharedFontListForPlatform() {}

  virtual gfxFontEntry* CreateFontEntry(
      mozilla::fontlist::Face* aFace,
      const mozilla::fontlist::Family* aFamily) {
    return nullptr;
  }

  /**
   * Methods to apply the font.system.whitelist anti-fingerprinting pref,
   * by filtering the list of installed fonts so that only whitelisted families
   * are exposed.
   * There are separate implementations of this for the per-process font list
   * and for the shared-memory font list.
   */
  void ApplyWhitelist();
  void ApplyWhitelist(nsTArray<mozilla::fontlist::Family::InitData>& aFamilies);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // used by memory reporter to accumulate sizes of family names in the table
  static size_t SizeOfFontFamilyTableExcludingThis(
      const FontFamilyTable& aTable, mozilla::MallocSizeOf aMallocSizeOf);
  static size_t SizeOfFontEntryTableExcludingThis(
      const FontEntryTable& aTable, mozilla::MallocSizeOf aMallocSizeOf);
||||||| merged common ancestors
    // used by memory reporter to accumulate sizes of family names in the table
    static size_t
    SizeOfFontFamilyTableExcludingThis(const FontFamilyTable& aTable,
                                       mozilla::MallocSizeOf aMallocSizeOf);
    static size_t
    SizeOfFontEntryTableExcludingThis(const FontEntryTable& aTable,
                                      mozilla::MallocSizeOf aMallocSizeOf);
=======
  // Create a new gfxFontFamily of the appropriate subclass for the platform,
  // used when AddWithLegacyFamilyName needs to create a new family.
  virtual gfxFontFamily* CreateFontFamily(const nsACString& aName) const = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Platform-specific helper for GetDefaultFont(...).
  virtual gfxFontFamily* GetDefaultFontForPlatform(
      const gfxFontStyle* aStyle) = 0;
||||||| merged common ancestors
    // Platform-specific helper for GetDefaultFont(...).
    virtual gfxFontFamily*
    GetDefaultFontForPlatform(const gfxFontStyle* aStyle) = 0;
=======
  /**
   * For the post-startup font info loader task.
   * Perform platform-specific work to read alternate names (if any) for a
   * font family, recording them in mAliasTable. Once alternate names have been
   * loaded for all families, the accumulated records are stored in the shared
   * font list's mAliases list.
   * Some platforms (currently Linux/fontconfig) may load alternate names as
   * part of initially populating the font list with family records, in which
   * case this method is unused.
   */
  virtual void ReadFaceNamesForFamily(mozilla::fontlist::Family* aFamily,
                                      bool aNeedFullnamePostscriptNames) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Protects mFontFamilies.
  mozilla::Mutex mFontFamiliesMutex;
||||||| merged common ancestors
    // Protects mFontFamilies.
    mozilla::Mutex mFontFamiliesMutex;
=======
  typedef nsRefPtrHashtable<nsCStringHashKey, gfxFontFamily> FontFamilyTable;
  typedef nsRefPtrHashtable<nsCStringHashKey, gfxFontEntry> FontEntryTable;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // canonical family name ==> family entry (unique, one name per family entry)
  FontFamilyTable mFontFamilies;
||||||| merged common ancestors
    // canonical family name ==> family entry (unique, one name per family entry)
    FontFamilyTable mFontFamilies;
=======
  // used by memory reporter to accumulate sizes of family names in the table
  static size_t SizeOfFontFamilyTableExcludingThis(
      const FontFamilyTable& aTable, mozilla::MallocSizeOf aMallocSizeOf);
  static size_t SizeOfFontEntryTableExcludingThis(
      const FontEntryTable& aTable, mozilla::MallocSizeOf aMallocSizeOf);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // other family name ==> family entry (not unique, can have multiple names per
  // family entry, only names *other* than the canonical names are stored here)
  FontFamilyTable mOtherFamilyNames;
||||||| merged common ancestors
    // other family name ==> family entry (not unique, can have multiple names per
    // family entry, only names *other* than the canonical names are stored here)
    FontFamilyTable mOtherFamilyNames;
=======
  // Platform-specific helper for GetDefaultFont(...).
  virtual FontFamily GetDefaultFontForPlatform(const gfxFontStyle* aStyle) = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // flag set after InitOtherFamilyNames is called upon first name lookup miss
  bool mOtherFamilyNamesInitialized;
||||||| merged common ancestors
    // flag set after InitOtherFamilyNames is called upon first name lookup miss
    bool mOtherFamilyNamesInitialized;
=======
  // Protects mFontFamilies.
  mozilla::Mutex mFontFamiliesMutex;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The pending InitOtherFamilyNames() task.
  RefPtr<mozilla::CancelableRunnable> mPendingOtherFamilyNameTask;
||||||| merged common ancestors
    // The pending InitOtherFamilyNames() task.
    RefPtr<mozilla::CancelableRunnable> mPendingOtherFamilyNameTask;
=======
  // canonical family name ==> family entry (unique, one name per family entry)
  FontFamilyTable mFontFamilies;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // flag set after fullname and Postcript name lists are populated
  bool mFaceNameListsInitialized;
||||||| merged common ancestors
    // flag set after fullname and Postcript name lists are populated
    bool mFaceNameListsInitialized;
=======
  // other family name ==> family entry (not unique, can have multiple names per
  // family entry, only names *other* than the canonical names are stored here)
  FontFamilyTable mOtherFamilyNames;
>>>>>>> upstream-releases

<<<<<<< HEAD
  struct ExtraNames {
    ExtraNames() : mFullnames(64), mPostscriptNames(64) {}
||||||| merged common ancestors
    struct ExtraNames {
      ExtraNames() : mFullnames(64), mPostscriptNames(64) {}
=======
  // flag set after InitOtherFamilyNames is called upon first name lookup miss
  bool mOtherFamilyNamesInitialized;
>>>>>>> upstream-releases

<<<<<<< HEAD
    // fullname ==> font entry (unique, one name per font entry)
    FontEntryTable mFullnames;
    // Postscript name ==> font entry (unique, one name per font entry)
    FontEntryTable mPostscriptNames;
  };
  mozilla::UniquePtr<ExtraNames> mExtraNames;
||||||| merged common ancestors
      // fullname ==> font entry (unique, one name per font entry)
      FontEntryTable mFullnames;
      // Postscript name ==> font entry (unique, one name per font entry)
      FontEntryTable mPostscriptNames;
    };
    mozilla::UniquePtr<ExtraNames> mExtraNames;
=======
  // The pending InitOtherFamilyNames() task.
  RefPtr<mozilla::CancelableRunnable> mPendingOtherFamilyNameTask;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // face names missed when face name loading takes a long time
  mozilla::UniquePtr<nsTHashtable<nsCStringHashKey>> mFaceNamesMissed;
||||||| merged common ancestors
    // face names missed when face name loading takes a long time
    mozilla::UniquePtr<nsTHashtable<nsCStringHashKey> > mFaceNamesMissed;
=======
  // flag set after fullname and Postcript name lists are populated
  bool mFaceNameListsInitialized;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // localized family names missed when face name loading takes a long time
  mozilla::UniquePtr<nsTHashtable<nsCStringHashKey>> mOtherNamesMissed;
||||||| merged common ancestors
    // localized family names missed when face name loading takes a long time
    mozilla::UniquePtr<nsTHashtable<nsCStringHashKey> > mOtherNamesMissed;
=======
  struct ExtraNames {
    ExtraNames() : mFullnames(64), mPostscriptNames(64) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  typedef nsTArray<RefPtr<gfxFontFamily>> PrefFontList;
  typedef mozilla::RangedArray<mozilla::UniquePtr<PrefFontList>,
                               mozilla::eFamily_generic_first,
                               mozilla::eFamily_generic_count>
      PrefFontsForLangGroup;
  mozilla::RangedArray<PrefFontsForLangGroup, eFontPrefLang_First,
                       eFontPrefLang_Count>
      mLangGroupPrefFonts;
  mozilla::UniquePtr<PrefFontList> mEmojiPrefFont;
||||||| merged common ancestors
    typedef nsTArray<RefPtr<gfxFontFamily>> PrefFontList;
    typedef mozilla::RangedArray<mozilla::UniquePtr<PrefFontList>,
                                 mozilla::eFamily_generic_first,
                                 mozilla::eFamily_generic_count> PrefFontsForLangGroup;
    mozilla::RangedArray<PrefFontsForLangGroup,
                         eFontPrefLang_First,
                         eFontPrefLang_Count> mLangGroupPrefFonts;
    mozilla::UniquePtr<PrefFontList> mEmojiPrefFont;
=======
    // fullname ==> font entry (unique, one name per font entry)
    FontEntryTable mFullnames;
    // Postscript name ==> font entry (unique, one name per font entry)
    FontEntryTable mPostscriptNames;
  };
  mozilla::UniquePtr<ExtraNames> mExtraNames;

  // face names missed when face name loading takes a long time
  mozilla::UniquePtr<nsTHashtable<nsCStringHashKey>> mFaceNamesMissed;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // when system-wide font lookup fails for a character, cache it to skip future
  // searches
  gfxSparseBitSet mCodepointsWithNoFonts;
||||||| merged common ancestors
    // when system-wide font lookup fails for a character, cache it to skip future searches
    gfxSparseBitSet mCodepointsWithNoFonts;
=======
  // localized family names missed when face name loading takes a long time
  mozilla::UniquePtr<nsTHashtable<nsCStringHashKey>> mOtherNamesMissed;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // the family to use for U+FFFD fallback, to avoid expensive search every time
  // on pages with lots of problems
  RefPtr<gfxFontFamily> mReplacementCharFallbackFamily;
||||||| merged common ancestors
    // the family to use for U+FFFD fallback, to avoid expensive search every time
    // on pages with lots of problems
    RefPtr<gfxFontFamily> mReplacementCharFallbackFamily;
=======
  typedef mozilla::RangedArray<mozilla::UniquePtr<PrefFontList>,
                               size_t(mozilla::StyleGenericFontFamily::None),
                               size_t(
                                   mozilla::StyleGenericFontFamily::MozEmoji)>
      PrefFontsForLangGroup;
  mozilla::RangedArray<PrefFontsForLangGroup, eFontPrefLang_First,
                       eFontPrefLang_Count>
      mLangGroupPrefFonts;
  mozilla::UniquePtr<PrefFontList> mEmojiPrefFont;

  // when system-wide font lookup fails for a character, cache it to skip future
  // searches
  gfxSparseBitSet mCodepointsWithNoFonts;

  // the family to use for U+FFFD fallback, to avoid expensive search every time
  // on pages with lots of problems
  FontFamily mReplacementCharFallbackFamily;

  // Sorted array of lowercased family names; use ContainsSorted to test
  nsTArray<nsCString> mBadUnderlineFamilyNames;

  // character map data shared across families
  // contains weak ptrs to cmaps shared by font entry objects
  nsTHashtable<CharMapHashKey> mSharedCmaps;

  nsTHashtable<ShmemCharMapHashEntry> mShmemCharMaps;

  // data used as part of the font cmap loading process
  nsTArray<RefPtr<gfxFontFamily>> mFontFamiliesToLoad;
  uint32_t mStartIndex;
  uint32_t mNumFamilies;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Sorted array of lowercased family names; use ContainsSorted to test
  nsTArray<nsCString> mBadUnderlineFamilyNames;
||||||| merged common ancestors
    nsTHashtable<nsCStringHashKey> mBadUnderlineFamilyNames;
=======
  // xxx - info for diagnosing no default font aborts
  // see bugs 636957, 1070983, 1189129
  uint32_t mFontlistInitCount;  // num times InitFontList called
>>>>>>> upstream-releases

<<<<<<< HEAD
  // character map data shared across families
  // contains weak ptrs to cmaps shared by font entry objects
  nsTHashtable<CharMapHashKey> mSharedCmaps;
||||||| merged common ancestors
    // character map data shared across families
    // contains weak ptrs to cmaps shared by font entry objects
    nsTHashtable<CharMapHashKey> mSharedCmaps;
=======
  nsTHashtable<nsPtrHashKey<gfxUserFontSet>> mUserFontSetList;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // data used as part of the font cmap loading process
  nsTArray<RefPtr<gfxFontFamily>> mFontFamiliesToLoad;
  uint32_t mStartIndex;
  uint32_t mNumFamilies;
||||||| merged common ancestors
    // data used as part of the font cmap loading process
    nsTArray<RefPtr<gfxFontFamily> > mFontFamiliesToLoad;
    uint32_t mStartIndex;
    uint32_t mNumFamilies;
=======
  nsLanguageAtomService* mLangService;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // xxx - info for diagnosing no default font aborts
  // see bugs 636957, 1070983, 1189129
  uint32_t mFontlistInitCount;  // num times InitFontList called
||||||| merged common ancestors
    // xxx - info for diagnosing no default font aborts
    // see bugs 636957, 1070983, 1189129
    uint32_t mFontlistInitCount; // num times InitFontList called
=======
  nsTArray<uint32_t> mCJKPrefLangs;
  nsTArray<mozilla::StyleGenericFontFamily> mDefaultGenericsLangGroup;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsTHashtable<nsPtrHashKey<gfxUserFontSet>> mUserFontSetList;
||||||| merged common ancestors
    nsTHashtable<nsPtrHashKey<gfxUserFontSet> > mUserFontSetList;
=======
  mozilla::UniquePtr<mozilla::fontlist::FontList> mSharedFontList;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsLanguageAtomService* mLangService;
||||||| merged common ancestors
    nsLanguageAtomService* mLangService;
=======
  nsClassHashtable<nsCStringHashKey, nsTArray<mozilla::fontlist::Pointer>>
      mAliasTable;
  nsDataHashtable<nsCStringHashKey, mozilla::fontlist::LocalFaceRec::InitData>
      mLocalNameTable;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsTArray<uint32_t> mCJKPrefLangs;
  nsTArray<mozilla::FontFamilyType> mDefaultGenericsLangGroup;
||||||| merged common ancestors
    nsTArray<uint32_t> mCJKPrefLangs;
    nsTArray<mozilla::FontFamilyType> mDefaultGenericsLangGroup;
=======
  nsRefPtrHashtable<nsPtrHashKey<mozilla::fontlist::Face>, gfxFontEntry>
      mFontEntries;
>>>>>>> upstream-releases

  bool mFontFamilyWhitelistActive;
};

MOZ_MAKE_ENUM_CLASS_BITWISE_OPERATORS(gfxPlatformFontList::FindFamiliesFlags)

#endif /* GFXPLATFORMFONTLIST_H_ */
