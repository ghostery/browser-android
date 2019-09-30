/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef SkPDFMakeToUnicodeCmap_DEFINED
#define SkPDFMakeToUnicodeCmap_DEFINED

#include "SkPDFFont.h"
#include "SkStream.h"

<<<<<<< HEAD
sk_sp<SkPDFStream> SkPDFMakeToUnicodeCmap(
        const SkUnichar* glyphToUnicode,
        const SkBitSet* subset,
||||||| merged common ancestors
sk_sp<SkPDFStream> SkPDFMakeToUnicodeCmap(
        const SkTDArray<SkUnichar>& glyphToUnicode,
        const SkBitSet* subset,
=======
std::unique_ptr<SkStreamAsset> SkPDFMakeToUnicodeCmap(
        const SkUnichar* glyphToUnicode,
        const SkPDFGlyphUse* subset,
>>>>>>> upstream-releases
        bool multiByteGlyphs,
        SkGlyphID firstGlyphID,
        SkGlyphID lastGlyphID);

// Exposed for unit testing.
<<<<<<< HEAD
void SkPDFAppendCmapSections(const SkUnichar* glyphToUnicode,
                             const SkBitSet* subset,
||||||| merged common ancestors
void SkPDFAppendCmapSections(const SkTDArray<SkUnichar>& glyphToUnicode,
                             const SkBitSet* subset,
=======
void SkPDFAppendCmapSections(const SkUnichar* glyphToUnicode,
                             const SkPDFGlyphUse* subset,
>>>>>>> upstream-releases
                             SkDynamicMemoryWStream* cmap,
                             bool multiByteGlyphs,
                             SkGlyphID firstGlyphID,
                             SkGlyphID lastGlyphID);

#endif  // SkPDFMakeToUnicodeCmap_DEFINED
