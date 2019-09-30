/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkPaintPriv_DEFINED
#define SkPaintPriv_DEFINED

<<<<<<< HEAD
#include "SkImageInfo.h"
#include "SkMatrix.h"
#include "SkPaint.h"
#include "SkTypeface.h"
||||||| merged common ancestors
#include "SkImageInfo.h"
#include "SkPaint.h"
#include "SkMatrix.h"
=======
#include "SkPaint.h"
>>>>>>> upstream-releases

<<<<<<< HEAD
class SkBitmap;
class SkImage;
class SkReadBuffer;
class SkWriteBuffer;
||||||| merged common ancestors
class SkBitmap;
class SkImage;
=======
class SkFont;
class SkReadBuffer;
class SkWriteBuffer;

enum SkReadPaintResult {
    kFailed_ReadPaint,
    kSuccess_JustPaint,
    kSuccess_PaintAndFont,
};
>>>>>>> upstream-releases

class SkPaintPriv {
public:
    enum ShaderOverrideOpacity {
        kNone_ShaderOverrideOpacity,        //!< there is no overriding shader (bitmap or image)
        kOpaque_ShaderOverrideOpacity,      //!< the overriding shader is opaque
        kNotOpaque_ShaderOverrideOpacity,   //!< the overriding shader may not be opaque
    };

    /**
     *  Returns true if drawing with this paint (or nullptr) will ovewrite all affected pixels.
     *
     *  Note: returns conservative true, meaning it may return false even though the paint might
     *        in fact overwrite its pixels.
     */
    static bool Overwrites(const SkPaint* paint, ShaderOverrideOpacity);

    static bool ShouldDither(const SkPaint&, SkColorType);

    /*
     * The luminance color is used to determine which Gamma Canonical color to map to.  This is
     * really only used by backends which want to cache glyph masks, and need some way to know if
     * they need to generate new masks based off a given color.
     */
    static SkColor ComputeLuminanceColor(const SkPaint&);

    /** Serializes SkPaint into a buffer. A companion unflatten() call
    can reconstitute the paint at a later time.

    @param buffer  SkWriteBuffer receiving the flattened SkPaint data
    */
    static void Flatten(const SkPaint& paint, SkWriteBuffer& buffer);

    /** Populates SkPaint, typically from a serialized stream, created by calling
        flatten() at an earlier time.

        SkReadBuffer class is not public, so unflatten() cannot be meaningfully called
        by the client.

        Older formats also stored font info in the serialized data. On success, this
        returns if it deserialized just a paint, or both a font and paint. The font
        param is optional.

        @param buffer  serialized data describing SkPaint content
        @return        false if the buffer contains invalid data
    */
    static SkReadPaintResult Unflatten(SkPaint* paint, SkReadBuffer& buffer, SkFont* font);

<<<<<<< HEAD
    // returns -1 if buffer is invalid for specified encoding
    static int ValidCountText(const void* text, size_t length, SkPaint::TextEncoding);

    static SkTypeface* GetTypefaceOrDefault(const SkPaint& paint) {
        return paint.getTypeface() ? paint.getTypeface() : SkTypeface::GetDefaultTypeface();
    }

    static sk_sp<SkTypeface> RefTypefaceOrDefault(const SkPaint& paint) {
        return paint.getTypeface() ? paint.refTypeface() : SkTypeface::MakeDefault();
    }

    /** Serializes SkPaint into a buffer. A companion unflatten() call
    can reconstitute the paint at a later time.

    @param buffer  SkWriteBuffer receiving the flattened SkPaint data
    */
    static void Flatten(const SkPaint& paint, SkWriteBuffer& buffer);

    /** Populates SkPaint, typically from a serialized stream, created by calling
    flatten() at an earlier time.

    SkReadBuffer class is not public, so unflatten() cannot be meaningfully called
    by the client.

    @param buffer  serialized data describing SkPaint content
    @return        false if the buffer contains invalid data
    */
    static bool Unflatten(SkPaint* paint, SkReadBuffer& buffer);

||||||| merged common ancestors
    // returns 0 if buffer is invalid for specified encoding
    static int ValidCountText(const void* text, size_t length, SkPaint::TextEncoding);
=======
private:
    static SkReadPaintResult Unflatten_PreV68(SkPaint* paint, SkReadBuffer& buffer, SkFont*);
>>>>>>> upstream-releases
};

#endif
