/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkTextToPathIter_DEFINED
#define SkTextToPathIter_DEFINED

<<<<<<< HEAD
||||||| merged common ancestors
#include "SkAutoKern.h"
=======
#include "SkFontPriv.h"
>>>>>>> upstream-releases
#include "SkPaint.h"
<<<<<<< HEAD
#include "SkStrikeCache.h"

||||||| merged common ancestors

class SkGlyphCache;
=======
#include "SkStrikeCache.h"
>>>>>>> upstream-releases

class SkTextBaseIter {
<<<<<<< HEAD
protected:
    SkTextBaseIter(const char text[], size_t length, const SkPaint& paint,
                   bool applyStrokeAndPathEffects);

    SkExclusiveStrikePtr fCache;
    SkPaint              fPaint;
    SkScalar             fScale;
    SkScalar             fPrevAdvance;
    const char*          fText;
    const char*          fStop;
    SkPaint::GlyphCacheProc fGlyphCacheProc;

    SkScalar        fXPos;      // accumulated xpos, returned in next
    int             fXYIndex;   // cache for horizontal -vs- vertical text
};

class SkTextToPathIter : SkTextBaseIter {
||||||| merged common ancestors
protected:
    SkTextBaseIter(const char text[], size_t length, const SkPaint& paint,
                   bool applyStrokeAndPathEffects);
    ~SkTextBaseIter();

    SkGlyphCache*   fCache;
    SkPaint         fPaint;
    SkScalar        fScale;
    SkScalar        fPrevAdvance;
    const char*     fText;
    const char*     fStop;
    SkPaint::GlyphCacheProc fGlyphCacheProc;

    SkScalar        fXPos;      // accumulated xpos, returned in next
    SkAutoKern      fAutoKern;
    int             fXYIndex;   // cache for horizontal -vs- vertical text
};

class SkTextToPathIter : SkTextBaseIter {
=======
>>>>>>> upstream-releases
public:
    const SkFont&   getFont() const { return fFont; }
    const SkPaint&  getPaint() const { return fPaint; }
    SkScalar        getPathScale() const { return fScale; }

protected:
    SkTextBaseIter(const SkGlyphID glyphs[], int count, const SkFont&, const SkPaint*);

    SkExclusiveStrikePtr fCache;
    SkFont               fFont;
    SkPaint              fPaint;
    SkScalar             fScale;
    SkScalar             fPrevAdvance;
    const SkGlyphID*     fGlyphs;
    const SkGlyphID*     fStop;

    SkScalar        fXPos;      // accumulated xpos, returned in next
};

class SkTextInterceptsIter : SkTextBaseIter {
public:
    enum class TextType {
        kText,
        kPosText
    };

    SkTextInterceptsIter(const SkGlyphID glyphs[], int count, const SkFont& font,
                         const SkPaint* paint, const SkScalar bounds[2], SkScalar x, SkScalar y,
                         TextType textType)
         : SkTextBaseIter(glyphs, count, font, paint)
    {
        fBoundsBase[0] = bounds[0];
        fBoundsBase[1] = bounds[1];
        this->setPosition(x, y);
    }

    /**
     *  Returns false when all of the text has been consumed
     */
    bool next(SkScalar* array, int* count);

    void setPosition(SkScalar x, SkScalar y) {
<<<<<<< HEAD
        SkScalar xOffset = TextType::kText == fTextType && fXYIndex ? fXPos : 0;
        if (TextType::kPosText == fTextType
                && fPaint.getTextAlign() != SkPaint::kLeft_Align) { // need to measure first
            const char* text = fText;
            const SkGlyph& glyph = fGlyphCacheProc(fCache.get(), &text, fStop);
            SkScalar width = (&glyph.fAdvanceX)[0] * fScale;
            if (fPaint.getTextAlign() == SkPaint::kCenter_Align) {
                width = SkScalarHalf(width);
            }
            xOffset = width;
        }

||||||| merged common ancestors
        SkScalar xOffset = TextType::kText == fTextType && fXYIndex ? fXPos : 0;
        if (TextType::kPosText == fTextType
                && fPaint.getTextAlign() != SkPaint::kLeft_Align) { // need to measure first
            const char* text = fText;
            const SkGlyph& glyph = fGlyphCacheProc(fCache, &text);
            SkScalar width = (&glyph.fAdvanceX)[0] * fScale;
            if (fPaint.getTextAlign() == SkPaint::kCenter_Align) {
                width = SkScalarHalf(width);
            }
            xOffset = width;
        }

=======
        SkScalar xOffset = 0;
>>>>>>> upstream-releases
        for (int i = 0; i < (int) SK_ARRAY_COUNT(fBounds); ++i) {
            SkScalar bound = fBoundsBase[i] - y;
            fBounds[i] = bound / fScale;
        }

        fXPos = xOffset + x;
        fPrevAdvance = 0;
    }

private:
    SkScalar fBounds[2];
    SkScalar fBoundsBase[2];
};

#endif
