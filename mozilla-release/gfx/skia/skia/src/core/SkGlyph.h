/*
 * Copyright 2006 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkGlyph_DEFINED
#define SkGlyph_DEFINED

#include "SkChecksum.h"
#include "SkFixed.h"
#include "SkMask.h"
<<<<<<< HEAD
#include "SkTo.h"
||||||| merged common ancestors
=======
#include "SkPath.h"
#include "SkTo.h"
>>>>>>> upstream-releases
#include "SkTypes.h"

<<<<<<< HEAD
class SkPath;
class SkGlyphCache;
||||||| merged common ancestors

class SkPath;
class SkGlyphCache;
=======
class SkArenaAlloc;
class SkStrike;
class SkScalerContext;
>>>>>>> upstream-releases

// needs to be != to any valid SkMask::Format
#define MASK_FORMAT_UNKNOWN         (0xFF)
#define MASK_FORMAT_JUST_ADVANCE    MASK_FORMAT_UNKNOWN

#define kMaxGlyphWidth (1<<13)

/** (glyph-index or unicode-point) + subpixel-pos */
struct SkPackedID {
    static constexpr uint32_t kImpossibleID = ~0;
    enum {
        kSubBits = 2,
        kSubMask = ((1 << kSubBits) - 1),
        kSubShift = 24, // must be large enough for glyphs and unichars
        kCodeMask = ((1 << kSubShift) - 1),
        // relative offsets for X and Y subpixel bits
        kSubShiftX = kSubBits,
        kSubShiftY = 0
    };

    SkPackedID(uint32_t code) {
        SkASSERT(code <= kCodeMask);
        SkASSERT(code != kImpossibleID);
        fID = code;
    }

    SkPackedID(uint32_t code, SkFixed x, SkFixed y) {
        SkASSERT(code <= kCodeMask);
        x = FixedToSub(x);
        y = FixedToSub(y);
        uint32_t ID = (x << (kSubShift + kSubShiftX)) |
                      (y << (kSubShift + kSubShiftY)) |
                      code;
        SkASSERT(ID != kImpossibleID);
        fID = ID;
    }

    constexpr SkPackedID() : fID(kImpossibleID) {}

    bool operator==(const SkPackedID& that) const {
        return fID == that.fID;
    }
    bool operator!=(const SkPackedID& that) const {
        return !(*this == that);
    }
    bool operator<(SkPackedID that) const {
        return this->fID < that.fID;
    }

    uint32_t code() const {
        return fID & kCodeMask;
    }

<<<<<<< HEAD
    uint32_t getPackedID() const {
        return fID;
    }

||||||| merged common ancestors
=======
    uint32_t value() const {
        return fID;
    }

>>>>>>> upstream-releases
    SkFixed getSubXFixed() const {
        return SubToFixed(ID2SubX(fID));
    }

    SkFixed getSubYFixed() const {
        return SubToFixed(ID2SubY(fID));
    }

    uint32_t hash() const {
        return SkChecksum::CheapMix(fID);
    }

    SkString dump() const {
        SkString str;
        str.appendf("code: %d, x: %d, y:%d", code(), getSubXFixed(), getSubYFixed());
        return str;
    }

private:
    static unsigned ID2SubX(uint32_t id) {
        return id >> (kSubShift + kSubShiftX);
    }

    static unsigned ID2SubY(uint32_t id) {
        return (id >> (kSubShift + kSubShiftY)) & kSubMask;
    }

    static unsigned FixedToSub(SkFixed n) {
        return (n >> (16 - kSubBits)) & kSubMask;
    }

    static SkFixed SubToFixed(unsigned sub) {
        SkASSERT(sub <= kSubMask);
        return sub << (16 - kSubBits);
    }

    uint32_t fID;
};

struct SkPackedGlyphID : public SkPackedID {
    SkPackedGlyphID(SkGlyphID code) : SkPackedID(code) { }
    SkPackedGlyphID(SkGlyphID code, SkFixed x, SkFixed y) : SkPackedID(code, x, y) { }
<<<<<<< HEAD
    SkPackedGlyphID(SkGlyphID code, SkIPoint pt) : SkPackedID(code, pt.x(), pt.y()) { }
    SkPackedGlyphID() : SkPackedID() { }
||||||| merged common ancestors
    SkPackedGlyphID() : SkPackedID() { }
=======
    SkPackedGlyphID(SkGlyphID code, SkIPoint pt) : SkPackedID(code, pt.x(), pt.y()) { }
    constexpr SkPackedGlyphID() = default;
>>>>>>> upstream-releases
    SkGlyphID code() const {
        return SkTo<SkGlyphID>(SkPackedID::code());
    }
};

<<<<<<< HEAD
struct SkPackedUnicharID : public SkPackedID {
    SkPackedUnicharID(SkUnichar code) : SkPackedID(code) { }
    SkPackedUnicharID(SkUnichar code, SkFixed x, SkFixed y) : SkPackedID(code, x, y) { }
    SkPackedUnicharID() : SkPackedID() { }
    SkUnichar code() const {
        return SkTo<SkUnichar>(SkPackedID::code());
    }
};

||||||| merged common ancestors
struct SkPackedUnicharID : public SkPackedID {
    SkPackedUnicharID(SkUnichar code) : SkPackedID(code) { }
    SkPackedUnicharID(SkUnichar code, SkFixed x, SkFixed y) : SkPackedID(code, x, y) { }
    SkPackedUnicharID() : SkPackedID() { }
    SkUnichar code() const {
        return SkTo<SkUnichar>(SkPackedID::code());
    }
};

SK_BEGIN_REQUIRE_DENSE
=======
>>>>>>> upstream-releases
class SkGlyph {
    struct PathData;

public:
<<<<<<< HEAD
    static const SkFixed kSubpixelRound = SK_FixedHalf >> SkPackedID::kSubBits;
    void* fImage;
    PathData* fPathData;
    float       fAdvanceX, fAdvanceY;

    uint16_t    fWidth, fHeight;
    int16_t     fTop, fLeft;
    int8_t      fForceBW;

    uint8_t     fMaskFormat;
||||||| merged common ancestors
    static const SkFixed kSubpixelRound = SK_FixedHalf >> SkPackedID::kSubBits;
    void*       fImage;
    PathData*   fPathData;
    float       fAdvanceX, fAdvanceY;

    uint16_t    fWidth, fHeight;
    int16_t     fTop, fLeft;

    uint8_t     fMaskFormat;
    int8_t      fRsbDelta, fLsbDelta;  // used by auto-kerning
    int8_t      fForceBW;

    void initWithGlyphID(SkPackedGlyphID glyph_id) {
        fID             = glyph_id;
        fImage          = nullptr;
        fPathData       = nullptr;
        fMaskFormat     = MASK_FORMAT_UNKNOWN;
        fForceBW        = 0;
    }
=======
    constexpr explicit SkGlyph(SkPackedGlyphID id) : fID{id} {}
    static constexpr SkFixed kSubpixelRound = SK_FixedHalf >> SkPackedID::kSubBits;

    bool isEmpty() const { return fWidth == 0 || fHeight == 0; }
    bool isJustAdvance() const { return MASK_FORMAT_JUST_ADVANCE == fMaskFormat; }
    bool isFullMetrics() const { return MASK_FORMAT_JUST_ADVANCE != fMaskFormat; }
    SkGlyphID getGlyphID() const { return fID.code(); }
    SkPackedGlyphID getPackedID() const { return fID; }
    SkFixed getSubXFixed() const { return fID.getSubXFixed(); }
    SkFixed getSubYFixed() const { return fID.getSubYFixed(); }

    size_t formatAlignment() const;
    size_t allocImage(SkArenaAlloc* alloc);
    size_t rowBytes() const;
    size_t computeImageSize() const;
    size_t rowBytesUsingFormat(SkMask::Format format) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
    void initWithGlyphID(SkPackedGlyphID glyph_id);
||||||| merged common ancestors
    static size_t BitsToBytes(size_t bits) {
        return (bits + 7) >> 3;
    }
=======
    // Call this to set all of the metrics fields to 0 (e.g. if the scaler
    // encounters an error measuring a glyph). Note: this does not alter the
    // fImage, fPath, fID, fMaskFormat fields.
    void zeroMetrics();
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool isEmpty() const {
        return fWidth == 0 || fHeight == 0;
    }
||||||| merged common ancestors
    /**
     *  Compute the rowbytes for the specified width and mask-format.
     */
    static unsigned ComputeRowBytes(unsigned width, SkMask::Format format) {
        unsigned rb = width;
        switch (format) {
        case SkMask::kBW_Format:
            rb = BitsToBytes(rb);
            break;
        case SkMask::kA8_Format:
            rb = SkAlign4(rb);
            break;
        case SkMask::k3D_Format:
            rb = SkAlign4(rb);
            break;
        case SkMask::kARGB32_Format:
            rb <<= 2;
            break;
        case SkMask::kLCD16_Format:
            rb = SkAlign4(rb << 1);
            break;
        default:
            SK_ABORT("Unknown mask format.");
            break;
        }
        return rb;
    }
=======
    void toMask(SkMask* mask) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
    size_t formatAlignment() const;
    size_t allocImage(SkArenaAlloc* alloc);
||||||| merged common ancestors
    size_t allocImage(SkArenaAlloc* alloc) {
        size_t allocSize;
        switch (static_cast<SkMask::Format>(fMaskFormat)) {
        case SkMask::kBW_Format:
            allocSize = BitsToBytes(fWidth) * fHeight;
            fImage = alloc->makeArrayDefault<char>(allocSize);
            break;
        case SkMask::kA8_Format:
            allocSize = SkAlign4(fWidth) * fHeight;
            fImage = alloc->makeArrayDefault<char>(allocSize);
            break;
        case SkMask::k3D_Format:
            allocSize = SkAlign4(fWidth) * fHeight * 3;
            fImage = alloc->makeArrayDefault<char>(allocSize);
            break;
        case SkMask::kARGB32_Format:
            allocSize = fWidth * fHeight;
            fImage = alloc->makeArrayDefault<uint32_t>(fWidth * fHeight);
            allocSize *= sizeof(uint32_t);
            break;
        case SkMask::kLCD16_Format:
            allocSize = SkAlign2(fWidth) * fHeight;
            fImage = alloc->makeArrayDefault<uint16_t>(allocSize);
            allocSize *= sizeof(uint16_t);
            break;
        default:
            SK_ABORT("Unknown mask format.");
            break;
        }
        return allocSize;
    }
=======
    SkPath* addPath(SkScalerContext*, SkArenaAlloc*);
>>>>>>> upstream-releases

<<<<<<< HEAD
    size_t rowBytes() const;
    size_t rowBytesUsingFormat(SkMask::Format format) const;
||||||| merged common ancestors
    unsigned rowBytes() const {
        return ComputeRowBytes(fWidth, (SkMask::Format)fMaskFormat);
    }
=======
    SkPath* path() const {
        return fPathData != nullptr && fPathData->fHasPath ? &fPathData->fPath : nullptr;
    }
>>>>>>> upstream-releases

    // Returns the size allocated on the arena.
    size_t copyImageData(const SkGlyph& from, SkArenaAlloc* alloc);

    void*     fImage    = nullptr;

    // Path data has tricky state. If the glyph isEmpty, then fPathData should always be nullptr,
    // else if fPathData is not null, then a path has been requested. The fPath field of fPathData
    // may still be null after the request meaning that there is no path for this glyph.
    PathData* fPathData = nullptr;

    // The advance for this glyph.
    float     fAdvanceX = 0,
              fAdvanceY = 0;

    // The width and height of the glyph mask.
    uint16_t  fWidth  = 0,
              fHeight = 0;

    // The offset from the glyphs origin on the baseline to the top left of the glyph mask.
    int16_t   fTop  = 0,
              fLeft = 0;

    // Used by the GDI scaler to track state.
    int8_t    fForceBW = 0;

    // This is a combination of SkMask::Format and SkGlyph state. The SkGlyph can be in one of two
    // states, just the advances have been calculated, and all the metrics are available. The
    // illegal mask format is used to signal that only the advances are available.
    uint8_t   fMaskFormat = MASK_FORMAT_UNKNOWN;

private:

<<<<<<< HEAD
    /** Returns the size allocated on the arena.
     */
    size_t copyImageData(const SkGlyph& from, SkArenaAlloc* alloc) {
        fMaskFormat = from.fMaskFormat;
        fWidth = from.fWidth;
        fHeight = from.fHeight;
        fLeft = from.fLeft;
        fTop = from.fTop;
        fForceBW = from.fForceBW;

        if (from.fImage != nullptr) {
            auto imageSize = this->allocImage(alloc);
            SkASSERT(imageSize == from.computeImageSize());

            memcpy(fImage, from.fImage, imageSize);
            return imageSize;
        }

        return 0u;
    }

    class HashTraits {
    public:
        static SkPackedGlyphID GetKey(const SkGlyph& glyph) {
            return glyph.fID;
        }
        static uint32_t Hash(SkPackedGlyphID glyphId) {
            return glyphId.hash();
        }
||||||| merged common ancestors
    class HashTraits {
    public:
        static SkPackedGlyphID GetKey(const SkGlyph& glyph) {
            return glyph.fID;
        }
        static uint32_t Hash(SkPackedGlyphID glyphId) {
            return glyphId.hash();
        }
=======
    // Support horizontal and vertical skipping strike-through / underlines.
    // The caller walks the linked list looking for a match. For a horizontal underline,
    // the fBounds contains the top and bottom of the underline. The fInterval pair contains the
    // beginning and end of of the intersection of the bounds and the glyph's path.
    // If interval[0] >= interval[1], no intesection was found.
    struct Intercept {
        Intercept* fNext;
        SkScalar   fBounds[2];    // for horz underlines, the boundaries in Y
        SkScalar   fInterval[2];  // the outside intersections of the axis and the glyph
>>>>>>> upstream-releases
    };

<<<<<<< HEAD
 private:
    // TODO(herb) remove friend statement after SkGlyphCache cleanup.
    friend class SkGlyphCache;
||||||| merged common ancestors
 private:
    // TODO(herb) remove friend statement after SkGlyphCache cleanup.
    friend class SkGlyphCache;

// FIXME - This is needed because the Android frame work directly accesses fID.
// Remove when fID accesses are cleaned up.
#ifdef SK_BUILD_FOR_ANDROID_FRAMEWORK
  public:
#endif
=======
    struct PathData {
        Intercept* fIntercept{nullptr};
        SkPath     fPath;
        bool       fHasPath{false};
    };

    // TODO(herb) remove friend statement after SkStrike cleanup.
    friend class SkStrike;
>>>>>>> upstream-releases
    SkPackedGlyphID fID;
};

#endif
