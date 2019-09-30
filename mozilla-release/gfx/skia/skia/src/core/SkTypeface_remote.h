/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkRemoteTypeface_DEFINED
#define SkRemoteTypeface_DEFINED

#include "SkAdvancedTypefaceMetrics.h"
#include "SkDescriptor.h"
#include "SkFontDescriptor.h"
#include "SkFontStyle.h"
#include "SkPaint.h"
#include "SkRemoteGlyphCache.h"
#include "SkScalerContext.h"
#include "SkTypeface.h"

class SkTypefaceProxy;
class SkStrikeCache;

class SkScalerContextProxy : public SkScalerContext {
public:
<<<<<<< HEAD
    SkScalerContextProxy(sk_sp<SkTypeface> tf,
                         const SkScalerContextEffects& effects,
                         const SkDescriptor* desc,
                         sk_sp<SkStrikeClient::DiscardableHandleManager> manager);

    void initCache(SkGlyphCache*, SkStrikeCache*);
||||||| merged common ancestors
    SkScalerContextProxy(
            sk_sp<SkTypeface> tf,
            const SkScalerContextEffects& effects,
            const SkDescriptor* desc,
            SkRemoteScalerContext* rsc);
=======
    SkScalerContextProxy(sk_sp<SkTypeface> tf,
                         const SkScalerContextEffects& effects,
                         const SkDescriptor* desc,
                         sk_sp<SkStrikeClient::DiscardableHandleManager> manager);

    void initCache(SkStrike*, SkStrikeCache*);
>>>>>>> upstream-releases

protected:
    unsigned generateGlyphCount() override;
    uint16_t generateCharToGlyph(SkUnichar) override;
    bool generateAdvance(SkGlyph* glyph) override;
    void generateMetrics(SkGlyph* glyph) override;
    void generateImage(const SkGlyph& glyph) override;
<<<<<<< HEAD
    bool generatePath(SkGlyphID glyphID, SkPath* path) override;
    void generateFontMetrics(SkPaint::FontMetrics* metrics) override;
    SkTypefaceProxy* getProxyTypeface() const;
||||||| merged common ancestors
    void generatePath(SkGlyphID glyphID, SkPath* path) override;
    void generateFontMetrics(SkPaint::FontMetrics* metrics) override;
=======
    bool generatePath(SkGlyphID glyphID, SkPath* path) override;
    void generateFontMetrics(SkFontMetrics* metrics) override;
    SkTypefaceProxy* getProxyTypeface() const;
>>>>>>> upstream-releases

private:
<<<<<<< HEAD
    sk_sp<SkStrikeClient::DiscardableHandleManager> fDiscardableManager;
    SkGlyphCache* fCache = nullptr;
    SkStrikeCache* fStrikeCache = nullptr;
||||||| merged common ancestors
    // Copied from SkGlyphCache
    // so we don't grow our arrays a lot
    static constexpr size_t kMinGlyphCount = 8;
    static constexpr size_t kMinGlyphImageSize = 16 /* height */ * 8 /* width */;
    static constexpr size_t kMinAllocAmount = kMinGlyphImageSize * kMinGlyphCount;
    SkArenaAlloc  fAlloc{kMinAllocAmount};

    SkTypefaceProxy* typefaceProxy();
    SkRemoteScalerContext* const fRemote;
=======
    sk_sp<SkStrikeClient::DiscardableHandleManager> fDiscardableManager;
    SkStrike* fCache = nullptr;
    SkStrikeCache* fStrikeCache = nullptr;
>>>>>>> upstream-releases
    typedef SkScalerContext INHERITED;
};

class SkTypefaceProxy : public SkTypeface {
public:
    SkTypefaceProxy(SkFontID fontId,
                    int glyphCount,
                    const SkFontStyle& style,
                    bool isFixed,
                    sk_sp<SkStrikeClient::DiscardableHandleManager> manager,
                    bool isLogging = true)
            : INHERITED{style, false}
            , fFontId{fontId}
            , fGlyphCount{glyphCount}
            , fIsLogging{isLogging}
            , fDiscardableManager{std::move(manager)} {}
    SkFontID remoteTypefaceID() const {return fFontId;}
    int glyphCount() const {return fGlyphCount;}
    bool isLogging() const {return fIsLogging;}

protected:
    int onGetUPEM() const override { SK_ABORT("Should never be called."); return 0; }
    std::unique_ptr<SkStreamAsset> onOpenStream(int* ttcIndex) const override {
        SK_ABORT("Should never be called.");
        return nullptr;
    }
    std::unique_ptr<SkFontData> onMakeFontData() const override {
        SK_ABORT("Should never be called.");
        return nullptr;
    }
    sk_sp<SkTypeface> onMakeClone(const SkFontArguments& args) const override {
        SK_ABORT("Should never be called.");
        return nullptr;
    }
    int onGetVariationDesignPosition(SkFontArguments::VariationPosition::Coordinate coordinates[],
                                     int coordinateCount) const override {
        SK_ABORT("Should never be called.");
        return 0;
    }
    int onGetVariationDesignParameters(SkFontParameters::Variation::Axis parameters[],
                                       int parameterCount) const override {
        SK_ABORT("Should never be called.");
        return 0;
    }
    void onGetFamilyName(SkString* familyName) const override {
        // Used by SkStrikeCache::DumpMemoryStatistics.
        *familyName = "";
    }
    SkTypeface::LocalizedStrings* onCreateFamilyNameIterator() const override {
        SK_ABORT("Should never be called.");
        return nullptr;
    }
    int onGetTableTags(SkFontTableTag tags[]) const override {
        SK_ABORT("Should never be called.");
        return 0;
    }
    size_t onGetTableData(SkFontTableTag, size_t offset, size_t length, void* data) const override {
        SK_ABORT("Should never be called.");
        return 0;
    }
    SkScalerContext* onCreateScalerContext(const SkScalerContextEffects& effects,
                                           const SkDescriptor* desc) const override {
        return new SkScalerContextProxy(sk_ref_sp(const_cast<SkTypefaceProxy*>(this)), effects,
                                        desc, fDiscardableManager);
    }
    void onFilterRec(SkScalerContextRec* rec) const override {
        // The rec filtering is already applied by the server when generating
        // the glyphs.
    }
    void onGetFontDescriptor(SkFontDescriptor*, bool*) const override {
        SK_ABORT("Should never be called.");
    }
    void getGlyphToUnicodeMap(SkUnichar*) const override {
        SK_ABORT("Should never be called.");
    }

    std::unique_ptr<SkAdvancedTypefaceMetrics> onGetAdvancedMetrics() const override {
        SK_ABORT("Should never be called.");
        return nullptr;
    }
    int onCharsToGlyphs(const void* chars, Encoding,
                        uint16_t glyphs[], int glyphCount) const override {
        SK_ABORT("Should never be called.");
        return 0;
    }
    int onCountGlyphs() const override {
        return this->glyphCount();
    }

    void* onGetCTFontRef() const override {
        SK_ABORT("Should never be called.");
        return nullptr;
    }

private:
    const SkFontID                                  fFontId;
    const int                                       fGlyphCount;
    const bool                                      fIsLogging;
    sk_sp<SkStrikeClient::DiscardableHandleManager> fDiscardableManager;


    typedef SkTypeface INHERITED;
};

#endif  // SkRemoteTypeface_DEFINED
