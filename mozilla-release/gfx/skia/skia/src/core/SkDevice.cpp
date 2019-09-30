/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkDevice.h"

#include "SkColorFilter.h"
#include "SkDraw.h"
<<<<<<< HEAD
#include "SkGlyphRun.h"
||||||| merged common ancestors
#include "SkDrawFilter.h"
=======
#include "SkDrawable.h"
#include "SkGlyphRun.h"
>>>>>>> upstream-releases
#include "SkImageFilter.h"
#include "SkImageFilterCache.h"
#include "SkImagePriv.h"
#include "SkImage_Base.h"
#include "SkLatticeIter.h"
#include "SkLocalMatrixShader.h"
#include "SkMakeUnique.h"
#include "SkMatrixPriv.h"
#include "SkPatchUtils.h"
#include "SkPathMeasure.h"
#include "SkPathPriv.h"
#include "SkRSXform.h"
#include "SkRasterClip.h"
#include "SkShader.h"
#include "SkSpecialImage.h"
#include "SkTLazy.h"
<<<<<<< HEAD
#include "SkTextBlobPriv.h"
#include "SkTextToPathIter.h"
#include "SkTo.h"
||||||| merged common ancestors
#include "SkTextBlobRunIterator.h"
#include "SkTextToPathIter.h"
=======
#include "SkTextBlobPriv.h"
#include "SkTo.h"
>>>>>>> upstream-releases
#include "SkUtils.h"
#include "SkVertices.h"

SkBaseDevice::SkBaseDevice(const SkImageInfo& info, const SkSurfaceProps& surfaceProps)
    : fInfo(info)
    , fSurfaceProps(surfaceProps)
{
    fOrigin = {0, 0};
    fCTM.reset();
}

void SkBaseDevice::setOrigin(const SkMatrix& globalCTM, int x, int y) {
    fOrigin.set(x, y);
    fCTM = globalCTM;
    fCTM.postTranslate(SkIntToScalar(-x), SkIntToScalar(-y));
}

void SkBaseDevice::setGlobalCTM(const SkMatrix& ctm) {
    fCTM = ctm;
    if (fOrigin.fX | fOrigin.fY) {
        fCTM.postTranslate(-SkIntToScalar(fOrigin.fX), -SkIntToScalar(fOrigin.fY));
    }
}

bool SkBaseDevice::clipIsWideOpen() const {
    if (kRect_ClipType == this->onGetClipType()) {
        SkRegion rgn;
        this->onAsRgnClip(&rgn);
        SkASSERT(rgn.isRect());
        return rgn.getBounds() == SkIRect::MakeWH(this->width(), this->height());
    } else {
        return false;
    }
}

SkPixelGeometry SkBaseDevice::CreateInfo::AdjustGeometry(const SkImageInfo& info,
                                                         TileUsage tileUsage,
                                                         SkPixelGeometry geo,
                                                         bool preserveLCDText) {
    switch (tileUsage) {
        case kPossible_TileUsage:
            // (we think) for compatibility with old clients, we assume this layer can support LCD
            // even though they may not have marked it as opaque... seems like we should update
            // our callers (reed/robertphilips).
            break;
        case kNever_TileUsage:
            if (!preserveLCDText) {
                geo = kUnknown_SkPixelGeometry;
            }
            break;
    }
    return geo;
}

static inline bool is_int(float x) {
    return x == (float) sk_float_round2int(x);
}

void SkBaseDevice::drawRegion(const SkRegion& region, const SkPaint& paint) {
    const SkMatrix& ctm = this->ctm();
    bool isNonTranslate = ctm.getType() & ~(SkMatrix::kTranslate_Mask);
    bool complexPaint = paint.getStyle() != SkPaint::kFill_Style || paint.getMaskFilter() ||
                        paint.getPathEffect();
    bool antiAlias = paint.isAntiAlias() && (!is_int(ctm.getTranslateX()) ||
                                             !is_int(ctm.getTranslateY()));
    if (isNonTranslate || complexPaint || antiAlias) {
        SkPath path;
        region.getBoundaryPath(&path);
        path.setIsVolatile(true);
        return this->drawPath(path, paint, true);
    }

    SkRegion::Iterator it(region);
    while (!it.done()) {
        this->drawRect(SkRect::Make(it.rect()), paint);
        it.next();
    }
}

void SkBaseDevice::drawArc(const SkRect& oval, SkScalar startAngle,
                           SkScalar sweepAngle, bool useCenter, const SkPaint& paint) {
    SkPath path;
    bool isFillNoPathEffect = SkPaint::kFill_Style == paint.getStyle() && !paint.getPathEffect();
    SkPathPriv::CreateDrawArcPath(&path, oval, startAngle, sweepAngle, useCenter,
                                  isFillNoPathEffect);
    this->drawPath(path, paint);
}

void SkBaseDevice::drawDRRect(const SkRRect& outer,
                              const SkRRect& inner, const SkPaint& paint) {
    SkPath path;
    path.addRRect(outer);
    path.addRRect(inner);
    path.setFillType(SkPath::kEvenOdd_FillType);
    path.setIsVolatile(true);

<<<<<<< HEAD
    this->drawPath(path, paint, true);
||||||| merged common ancestors
    const SkMatrix* preMatrix = nullptr;
    const bool pathIsMutable = true;
    this->drawPath(path, paint, preMatrix, pathIsMutable);
=======
    this->drawPath(path, paint, true);
}

void SkBaseDevice::drawEdgeAARect(const SkRect& r, SkCanvas::QuadAAFlags aa, SkColor color,
                                  SkBlendMode mode) {
    SkPaint paint;
    paint.setColor(color);
    paint.setBlendMode(mode);
    paint.setAntiAlias(aa == SkCanvas::kAll_QuadAAFlags);

    this->drawRect(r, paint);
>>>>>>> upstream-releases
}

void SkBaseDevice::drawPatch(const SkPoint cubics[12], const SkColor colors[4],
                             const SkPoint texCoords[4], SkBlendMode bmode, const SkPaint& paint) {
    SkISize lod = SkPatchUtils::GetLevelOfDetail(cubics, &this->ctm());
    auto vertices = SkPatchUtils::MakeVertices(cubics, colors, texCoords, lod.width(), lod.height(),
                                               this->imageInfo().colorSpace());
    if (vertices) {
<<<<<<< HEAD
        this->drawVertices(vertices.get(), nullptr, 0, bmode, paint);
    }
}

void SkBaseDevice::drawImage(const SkImage* image, SkScalar x, SkScalar y,
                             const SkPaint& paint) {
    SkBitmap bm;
    if (as_IB(image)->getROPixels(&bm, this->imageInfo().colorSpace())) {
        this->drawBitmap(bm, x, y, paint);
||||||| merged common ancestors
        this->drawVertices(vertices.get(), bmode, paint);
    }
}

void SkBaseDevice::drawTextBlob(const SkTextBlob* blob, SkScalar x, SkScalar y,
                                const SkPaint &paint, SkDrawFilter* drawFilter) {

    SkPaint runPaint = paint;

    SkTextBlobRunIterator it(blob);
    for (;!it.done(); it.next()) {
        size_t textLen = it.glyphCount() * sizeof(uint16_t);
        const SkPoint& offset = it.offset();
        // applyFontToPaint() always overwrites the exact same attributes,
        // so it is safe to not re-seed the paint for this reason.
        it.applyFontToPaint(&runPaint);

        if (drawFilter && !drawFilter->filter(&runPaint, SkDrawFilter::kText_Type)) {
            // A false return from filter() means we should abort the current draw.
            runPaint = paint;
            continue;
        }

        runPaint.setFlags(this->filterTextFlags(runPaint));

        switch (it.positioning()) {
        case SkTextBlob::kDefault_Positioning:
            this->drawText(it.glyphs(), textLen, x + offset.x(), y + offset.y(), runPaint);
            break;
        case SkTextBlob::kHorizontal_Positioning:
            this->drawPosText(it.glyphs(), textLen, it.pos(), 1,
                              SkPoint::Make(x, y + offset.y()), runPaint);
            break;
        case SkTextBlob::kFull_Positioning:
            this->drawPosText(it.glyphs(), textLen, it.pos(), 2,
                              SkPoint::Make(x, y), runPaint);
            break;
        default:
            SK_ABORT("unhandled positioning mode");
        }

        if (drawFilter) {
            // A draw filter may change the paint arbitrarily, so we must re-seed in this case.
            runPaint = paint;
        }
    }
}

void SkBaseDevice::drawImage(const SkImage* image, SkScalar x, SkScalar y,
                             const SkPaint& paint) {
    SkBitmap bm;
    if (as_IB(image)->getROPixels(&bm, this->imageInfo().colorSpace())) {
        this->drawBitmap(bm, x, y, paint);
=======
        this->drawVertices(vertices.get(), nullptr, 0, bmode, paint);
>>>>>>> upstream-releases
    }
}

void SkBaseDevice::drawImageRect(const SkImage* image, const SkRect* src,
                                 const SkRect& dst, const SkPaint& paint,
                                 SkCanvas::SrcRectConstraint constraint) {
    SkBitmap bm;
    if (as_IB(image)->getROPixels(&bm)) {
        this->drawBitmapRect(bm, src, dst, paint, constraint);
    }
}

void SkBaseDevice::drawImageNine(const SkImage* image, const SkIRect& center,
                                 const SkRect& dst, const SkPaint& paint) {
    SkLatticeIter iter(image->width(), image->height(), center, dst);

    SkRect srcR, dstR;
    while (iter.next(&srcR, &dstR)) {
        this->drawImageRect(image, &srcR, dstR, paint, SkCanvas::kStrict_SrcRectConstraint);
    }
}

void SkBaseDevice::drawBitmapNine(const SkBitmap& bitmap, const SkIRect& center,
                                  const SkRect& dst, const SkPaint& paint) {
    SkLatticeIter iter(bitmap.width(), bitmap.height(), center, dst);

    SkRect srcR, dstR;
    while (iter.next(&srcR, &dstR)) {
        this->drawBitmapRect(bitmap, &srcR, dstR, paint, SkCanvas::kStrict_SrcRectConstraint);
    }
}

void SkBaseDevice::drawImageLattice(const SkImage* image,
                                    const SkCanvas::Lattice& lattice, const SkRect& dst,
                                    const SkPaint& paint) {
    SkLatticeIter iter(lattice, dst);

    SkRect srcR, dstR;
    SkColor c;
    bool isFixedColor = false;
    const SkImageInfo info = SkImageInfo::Make(1, 1, kBGRA_8888_SkColorType, kUnpremul_SkAlphaType);

    while (iter.next(&srcR, &dstR, &isFixedColor, &c)) {
          if (isFixedColor || (srcR.width() <= 1.0f && srcR.height() <= 1.0f &&
                               image->readPixels(info, &c, 4, srcR.fLeft, srcR.fTop))) {
              // Fast draw with drawRect, if this is a patch containing a single color
              // or if this is a patch containing a single pixel.
              if (0 != c || !paint.isSrcOver()) {
                   SkPaint paintCopy(paint);
                   int alpha = SkAlphaMul(SkColorGetA(c), SkAlpha255To256(paint.getAlpha()));
                   paintCopy.setColor(SkColorSetA(c, alpha));
                   this->drawRect(dstR, paintCopy);
              }
        } else {
            this->drawImageRect(image, &srcR, dstR, paint, SkCanvas::kStrict_SrcRectConstraint);
        }
    }
}

void SkBaseDevice::drawImageSet(const SkCanvas::ImageSetEntry images[], int count,
                                SkFilterQuality filterQuality, SkBlendMode mode) {
    SkPaint paint;
    paint.setFilterQuality(SkTPin(filterQuality, kNone_SkFilterQuality, kLow_SkFilterQuality));
    paint.setBlendMode(mode);
    for (int i = 0; i < count; ++i) {
        // TODO: Handle per-edge AA. Right now this mirrors the SkiaRenderer component of Chrome
        // which turns off antialiasing unless all four edges should be antialiased. This avoids
        // seaming in tiled composited layers.
        paint.setAntiAlias(images[i].fAAFlags == SkCanvas::kAll_QuadAAFlags);
        paint.setAlpha(SkToUInt(SkTClamp(SkScalarRoundToInt(images[i].fAlpha * 255), 0, 255)));
        this->drawImageRect(images[i].fImage.get(), &images[i].fSrcRect, images[i].fDstRect, paint,
                            SkCanvas::kFast_SrcRectConstraint);
    }
}

void SkBaseDevice::drawBitmapLattice(const SkBitmap& bitmap,
                                     const SkCanvas::Lattice& lattice, const SkRect& dst,
                                     const SkPaint& paint) {
    SkLatticeIter iter(lattice, dst);

    SkRect srcR, dstR;
    while (iter.next(&srcR, &dstR)) {
        this->drawBitmapRect(bitmap, &srcR, dstR, paint, SkCanvas::kStrict_SrcRectConstraint);
    }
}

static SkPoint* quad_to_tris(SkPoint tris[6], const SkPoint quad[4]) {
    tris[0] = quad[0];
    tris[1] = quad[1];
    tris[2] = quad[2];

    tris[3] = quad[0];
    tris[4] = quad[2];
    tris[5] = quad[3];

    return tris + 6;
}

void SkBaseDevice::drawAtlas(const SkImage* atlas, const SkRSXform xform[],
                             const SkRect tex[], const SkColor colors[], int quadCount,
                             SkBlendMode mode, const SkPaint& paint) {
    const int triCount = quadCount << 1;
    const int vertexCount = triCount * 3;
    uint32_t flags = SkVertices::kHasTexCoords_BuilderFlag;
    if (colors) {
        flags |= SkVertices::kHasColors_BuilderFlag;
    }
    SkVertices::Builder builder(SkVertices::kTriangles_VertexMode, vertexCount, 0, flags);

    SkPoint* vPos = builder.positions();
    SkPoint* vTex = builder.texCoords();
    SkColor* vCol = builder.colors();
    for (int i = 0; i < quadCount; ++i) {
        SkPoint tmp[4];
        xform[i].toQuad(tex[i].width(), tex[i].height(), tmp);
        vPos = quad_to_tris(vPos, tmp);

        tex[i].toQuad(tmp);
        vTex = quad_to_tris(vTex, tmp);

        if (colors) {
            sk_memset32(vCol, colors[i], 6);
            vCol += 6;
        }
    }
    SkPaint p(paint);
    p.setShader(atlas->makeShader());
<<<<<<< HEAD
    this->drawVertices(builder.detach().get(), nullptr, 0, mode, p);
||||||| merged common ancestors
    this->drawVertices(builder.detach().get(), mode, p);
=======
    this->drawVertices(builder.detach().get(), nullptr, 0, mode, p);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SkBaseDevice::drawDrawable(SkDrawable* drawable, const SkMatrix* matrix, SkCanvas* canvas) {
    drawable->draw(canvas, matrix);
>>>>>>> upstream-releases
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SkBaseDevice::drawSpecial(SkSpecialImage*, int x, int y, const SkPaint&,
                               SkImage*, const SkMatrix&) {}
sk_sp<SkSpecialImage> SkBaseDevice::makeSpecial(const SkBitmap&) { return nullptr; }
sk_sp<SkSpecialImage> SkBaseDevice::makeSpecial(const SkImage*) { return nullptr; }
sk_sp<SkSpecialImage> SkBaseDevice::snapSpecial() { return nullptr; }

///////////////////////////////////////////////////////////////////////////////////////////////////

bool SkBaseDevice::readPixels(const SkPixmap& pm, int x, int y) {
    return this->onReadPixels(pm, x, y);
}

bool SkBaseDevice::writePixels(const SkPixmap& pm, int x, int y) {
    return this->onWritePixels(pm, x, y);
}

bool SkBaseDevice::onWritePixels(const SkPixmap&, int, int) {
    return false;
}

bool SkBaseDevice::onReadPixels(const SkPixmap&, int x, int y) {
    return false;
}

bool SkBaseDevice::accessPixels(SkPixmap* pmap) {
    SkPixmap tempStorage;
    if (nullptr == pmap) {
        pmap = &tempStorage;
    }
    return this->onAccessPixels(pmap);
}

bool SkBaseDevice::peekPixels(SkPixmap* pmap) {
    SkPixmap tempStorage;
    if (nullptr == pmap) {
        pmap = &tempStorage;
    }
    return this->onPeekPixels(pmap);
}

//////////////////////////////////////////////////////////////////////////////////////////

#include "SkUtils.h"

<<<<<<< HEAD
void SkBaseDevice::drawGlyphRunRSXform(SkGlyphRun* run, const SkRSXform* xform) {
    const SkMatrix originalCTM = this->ctm();
    if (!originalCTM.isFinite() || !SkScalarIsFinite(run->paint().getTextSize()) ||
        !SkScalarIsFinite(run->paint().getTextScaleX()) ||
        !SkScalarIsFinite(run->paint().getTextSkewX())) {
||||||| merged common ancestors
    // nothing to draw
    if (text == nullptr || byteLength == 0) {
=======
void SkBaseDevice::drawGlyphRunRSXform(const SkFont& font, const SkGlyphID glyphs[],
                                       const SkRSXform xform[], int count, SkPoint origin,
                                       const SkPaint& paint) {
    const SkMatrix originalCTM = this->ctm();
    if (!originalCTM.isFinite() || !SkScalarIsFinite(font.getSize()) ||
        !SkScalarIsFinite(font.getScaleX()) ||
        !SkScalarIsFinite(font.getSkewX())) {
>>>>>>> upstream-releases
        return;
    }
<<<<<<< HEAD
    sk_sp<SkShader> shader = sk_ref_sp(run->mutablePaint()->getShader());
    auto perGlyph = [this, &xform, &originalCTM, shader] (
            SkGlyphRun* glyphRun, SkPaint* runPaint) {
        SkMatrix ctm;
        ctm.setRSXform(*xform++);

        // We want to rotate each glyph by the rsxform, but we don't want to rotate "space"
        // (i.e. the shader that cares about the ctm) so we have to undo our little ctm trick
        // with a localmatrixshader so that the shader draws as if there was no change to the ctm.
        if (shader) {
            SkMatrix inverse;
            if (ctm.invert(&inverse)) {
                runPaint->setShader(shader->makeWithLocalMatrix(inverse));
            } else {
                runPaint->setShader(nullptr);  // can't handle this xform
||||||| merged common ancestors

    SkTextToPathIter    iter((const char*)text, byteLength, paint, true);
    SkPathMeasure       meas(follow, false);
    SkScalar            hOffset = 0;

    // need to measure first
    if (paint.getTextAlign() != SkPaint::kLeft_Align) {
        SkScalar pathLen = meas.getLength();
        if (paint.getTextAlign() == SkPaint::kCenter_Align) {
            pathLen = SkScalarHalf(pathLen);
        }
        hOffset += pathLen;
    }

    const SkPath*   iterPath;
    SkScalar        xpos;
    SkMatrix        scaledMatrix;
    SkScalar        scale = iter.getPathScale();

    scaledMatrix.setScale(scale, scale);

    while (iter.next(&iterPath, &xpos)) {
        if (iterPath) {
            SkPath      tmp;
            SkMatrix    m(scaledMatrix);

            tmp.setIsVolatile(true);
            m.postTranslate(xpos + hOffset, 0);
            if (matrix) {
                m.postConcat(*matrix);
=======

    SkPoint sharedPos{0, 0};    // we're at the origin
    SkGlyphID glyphID;
    SkGlyphRun glyphRun{
        font,
        SkSpan<const SkPoint>{&sharedPos, 1},
        SkSpan<const SkGlyphID>{&glyphID, 1},
        SkSpan<const char>{},
        SkSpan<const uint32_t>{}
    };

    for (int i = 0; i < count; i++) {
        glyphID = glyphs[i];
        // now "glyphRun" is pointing at the current glyphID

        SkMatrix ctm;
        ctm.setRSXform(xform[i]).postTranslate(origin.fX, origin.fY);

        // We want to rotate each glyph by the rsxform, but we don't want to rotate "space"
        // (i.e. the shader that cares about the ctm) so we have to undo our little ctm trick
        // with a localmatrixshader so that the shader draws as if there was no change to the ctm.
        SkPaint transformingPaint{paint};
        auto shader = transformingPaint.getShader();
        if (shader) {
            SkMatrix inverse;
            if (ctm.invert(&inverse)) {
                transformingPaint.setShader(shader->makeWithLocalMatrix(inverse));
            } else {
                transformingPaint.setShader(nullptr);  // can't handle this xform
>>>>>>> upstream-releases
            }
        }

<<<<<<< HEAD
        ctm.setConcat(originalCTM, ctm);
        this->setCTM(ctm);
        SkGlyphRunList glyphRunList{glyphRun};
        this->drawGlyphRunList(glyphRunList);
    };
    run->eachGlyphToGlyphRun(perGlyph);
    run->mutablePaint()->setShader(shader);
    this->setCTM(originalCTM);
||||||| merged common ancestors
    SkMatrix localM, currM;
    const void* stopText = (const char*)text + len;
    while ((const char*)text < (const char*)stopText) {
        localM.setRSXform(*xform++);
        currM.setConcat(this->ctm(), localM);
        SkAutoDeviceCTMRestore adc(this, currM);

        int subLen = proc((const char*)text);
        this->drawText(text, subLen, 0, 0, paint);
        text = (const char*)text + subLen;
    }
=======
        ctm.setConcat(originalCTM, ctm);
        this->setCTM(ctm);

        this->drawGlyphRunList(SkGlyphRunList{glyphRun, transformingPaint});
    }
    this->setCTM(originalCTM);
>>>>>>> upstream-releases
}

//////////////////////////////////////////////////////////////////////////////////////////

<<<<<<< HEAD
sk_sp<SkSurface> SkBaseDevice::makeSurface(SkImageInfo const&, SkSurfaceProps const&) {
||||||| merged common ancestors
uint32_t SkBaseDevice::filterTextFlags(const SkPaint& paint) const {
    uint32_t flags = paint.getFlags();

    if (!paint.isLCDRenderText() || !paint.isAntiAlias()) {
        return flags;
    }

    if (kUnknown_SkPixelGeometry == fSurfaceProps.pixelGeometry()
        || this->onShouldDisableLCD(paint)) {

        flags &= ~SkPaint::kLCDRenderText_Flag;
        flags |= SkPaint::kGenA8FromLCD_Flag;
    }

    return flags;
}

sk_sp<SkSurface> SkBaseDevice::makeSurface(SkImageInfo const&, SkSurfaceProps const&) {
=======
sk_sp<SkSurface> SkBaseDevice::makeSurface(SkImageInfo const&, SkSurfaceProps const&) {
    return nullptr;
}

sk_sp<SkSpecialImage> SkBaseDevice::snapBackImage(const SkIRect&) {
>>>>>>> upstream-releases
    return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////

void SkBaseDevice::LogDrawScaleFactor(const SkMatrix& view, const SkMatrix& srcToDst,
                                      SkFilterQuality filterQuality) {
#if SK_HISTOGRAMS_ENABLED
    SkMatrix matrix = SkMatrix::Concat(view, srcToDst);
    enum ScaleFactor {
        kUpscale_ScaleFactor,
        kNoScale_ScaleFactor,
        kDownscale_ScaleFactor,
        kLargeDownscale_ScaleFactor,

        kLast_ScaleFactor = kLargeDownscale_ScaleFactor
    };

    float rawScaleFactor = matrix.getMinScale();

    ScaleFactor scaleFactor;
    if (rawScaleFactor < 0.5f) {
        scaleFactor = kLargeDownscale_ScaleFactor;
    } else if (rawScaleFactor < 1.0f) {
        scaleFactor = kDownscale_ScaleFactor;
    } else if (rawScaleFactor > 1.0f) {
        scaleFactor = kUpscale_ScaleFactor;
    } else {
        scaleFactor = kNoScale_ScaleFactor;
    }

    switch (filterQuality) {
        case kNone_SkFilterQuality:
            SK_HISTOGRAM_ENUMERATION("DrawScaleFactor.NoneFilterQuality", scaleFactor,
                                     kLast_ScaleFactor + 1);
            break;
        case kLow_SkFilterQuality:
            SK_HISTOGRAM_ENUMERATION("DrawScaleFactor.LowFilterQuality", scaleFactor,
                                     kLast_ScaleFactor + 1);
            break;
        case kMedium_SkFilterQuality:
            SK_HISTOGRAM_ENUMERATION("DrawScaleFactor.MediumFilterQuality", scaleFactor,
                                     kLast_ScaleFactor + 1);
            break;
        case kHigh_SkFilterQuality:
            SK_HISTOGRAM_ENUMERATION("DrawScaleFactor.HighFilterQuality", scaleFactor,
                                     kLast_ScaleFactor + 1);
            break;
    }

    // Also log filter quality independent scale factor.
    SK_HISTOGRAM_ENUMERATION("DrawScaleFactor.AnyFilterQuality", scaleFactor,
                             kLast_ScaleFactor + 1);

    // Also log an overall histogram of filter quality.
    SK_HISTOGRAM_ENUMERATION("FilterQuality", filterQuality, kLast_SkFilterQuality + 1);
#endif
}

