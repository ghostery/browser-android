/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkPDFBitmap.h"

#include "SkColorData.h"
#include "SkData.h"
#include "SkDeflate.h"
#include "SkExecutor.h"
#include "SkImage.h"
#include "SkImageInfoPriv.h"
#include "SkJpegInfo.h"
#include "SkPDFDocumentPriv.h"
#include "SkPDFTypes.h"
#include "SkPDFUtils.h"
#include "SkStream.h"
<<<<<<< HEAD
#include "SkTo.h"
#include "SkUnPreMultiply.h"

bool image_compute_is_opaque(const SkImage* image) {
    if (image->isOpaque()) {
        return true;
    }
    // keep output PDF small at cost of possible resource use.
    SkBitmap bm;
    // if image can not be read, treat as transparent.
    return SkPDFUtils::ToBitmap(image, &bm) && SkBitmap::ComputeIsOpaque(bm);
}

////////////////////////////////////////////////////////////////////////////////

static const char kStreamBegin[] = " stream\n";

static const char kStreamEnd[] = "\nendstream";
||||||| merged common ancestors
#include "SkUnPreMultiply.h"

bool image_compute_is_opaque(const SkImage* image) {
    if (image->isOpaque()) {
        return true;
    }
    // keep output PDF small at cost of possible resource use.
    SkBitmap bm;
    // if image can not be read, treat as transparent.
    return SkPDFUtils::ToBitmap(image, &bm) && SkBitmap::ComputeIsOpaque(bm);
}

////////////////////////////////////////////////////////////////////////////////

static const char kStreamBegin[] = " stream\n";

static const char kStreamEnd[] = "\nendstream";
=======
#include "SkTo.h"
>>>>>>> upstream-releases

////////////////////////////////////////////////////////////////////////////////

// write a single byte to a stream n times.
static void fill_stream(SkWStream* out, char value, size_t n) {
    char buffer[4096];
    memset(buffer, value, sizeof(buffer));
    for (size_t i = 0; i < n / sizeof(buffer); ++i) {
        out->write(buffer, sizeof(buffer));
    }
    out->write(buffer, n % sizeof(buffer));
}

/* It is necessary to average the color component of transparent
   pixels with their surrounding neighbors since the PDF renderer may
   separately re-sample the alpha and color channels when the image is
   not displayed at its native resolution. Since an alpha of zero
   gives no information about the color component, the pathological
   case is a white image with sharp transparency bounds - the color
   channel goes to black, and the should-be-transparent pixels are
   rendered as grey because of the separate soft mask and color
   resizing. e.g.: gm/bitmappremul.cpp */
static SkColor get_neighbor_avg_color(const SkPixmap& bm, int xOrig, int yOrig) {
    SkASSERT(kBGRA_8888_SkColorType == bm.colorType());
    unsigned r = 0, g = 0, b = 0, n = 0;
    // Clamp the range to the edge of the bitmap.
    int ymin = SkTMax(0, yOrig - 1);
    int ymax = SkTMin(yOrig + 1, bm.height() - 1);
    int xmin = SkTMax(0, xOrig - 1);
    int xmax = SkTMin(xOrig + 1, bm.width() - 1);
    for (int y = ymin; y <= ymax; ++y) {
        const SkColor* scanline = bm.addr32(0, y);
        for (int x = xmin; x <= xmax; ++x) {
            SkColor color = scanline[x];
            if (color != SK_ColorTRANSPARENT) {
                r += SkColorGetR(color);
                g += SkColorGetG(color);
                b += SkColorGetB(color);
                n++;
            }
        }
    }
    return n > 0 ? SkColorSetRGB(SkToU8(r / n), SkToU8(g / n), SkToU8(b / n))
                 : SK_ColorTRANSPARENT;
}

template <typename T>
static void emit_image_stream(SkPDFDocument* doc,
                              SkPDFIndirectReference ref,
                              T writeStream,
                              SkISize size,
                              const char* colorSpace,
                              SkPDFIndirectReference sMask,
                              int length,
                              bool isJpeg) {
    SkPDFDict pdfDict("XObject");
    pdfDict.insertName("Subtype", "Image");
    pdfDict.insertInt("Width", size.width());
    pdfDict.insertInt("Height", size.height());
    pdfDict.insertName("ColorSpace", colorSpace);
    if (sMask) {
        pdfDict.insertRef("SMask", sMask);
    }
    pdfDict.insertInt("BitsPerComponent", 8);
    #ifdef SK_PDF_BASE85_BINARY
    auto filters = SkPDFMakeArray();
    filters->appendName("ASCII85Decode");
    filters->appendName(isJpeg ? "DCTDecode" : "FlateDecode");
    pdfDict.insertObject("Filter", std::move(filters));
    #else
    pdfDict.insertName("Filter", isJpeg ? "DCTDecode" : "FlateDecode");
    #endif
    if (isJpeg) {
        pdfDict.insertInt("ColorTransform", 0);
    }
    pdfDict.insertInt("Length", length);
    doc->emitStream(pdfDict, std::move(writeStream), ref);
}

static void do_deflated_alpha(const SkPixmap& pm, SkPDFDocument* doc, SkPDFIndirectReference ref) {
    SkDynamicMemoryWStream buffer;
    SkDeflateWStream deflateWStream(&buffer);
    if (kAlpha_8_SkColorType == pm.colorType()) {
        SkASSERT(pm.rowBytes() == (size_t)pm.width());
        buffer.write(pm.addr8(), pm.width() * pm.height());
    } else {
        SkASSERT(pm.alphaType() == kUnpremul_SkAlphaType);
        SkASSERT(pm.colorType() == kBGRA_8888_SkColorType);
        SkASSERT(pm.rowBytes() == (size_t)pm.width() * 4);
        const uint32_t* ptr = pm.addr32();
        const uint32_t* stop = ptr + pm.height() * pm.width();

        uint8_t byteBuffer[4092];
        uint8_t* bufferStop = byteBuffer + SK_ARRAY_COUNT(byteBuffer);
        uint8_t* dst = byteBuffer;
        while (ptr != stop) {
            *dst++ = 0xFF & ((*ptr++) >> SK_BGRA_A32_SHIFT);
            if (dst == bufferStop) {
                deflateWStream.write(byteBuffer, sizeof(byteBuffer));
                dst = byteBuffer;
            }
        }
        deflateWStream.write(byteBuffer, dst - byteBuffer);
    }
    deflateWStream.finalize();

    #ifdef SK_PDF_BASE85_BINARY
    SkPDFUtils::Base85Encode(buffer.detachAsStream(), &buffer);
    #endif
    int length = SkToInt(buffer.bytesWritten());
    emit_image_stream(doc, ref, [&buffer](SkWStream* stream) { buffer.writeToAndReset(stream); },
                      pm.info().dimensions(), "DeviceGray", SkPDFIndirectReference(),
                      length, false);
}

static void do_deflated_image(const SkPixmap& pm,
                              SkPDFDocument* doc,
                              bool isOpaque,
                              SkPDFIndirectReference ref) {
    SkPDFIndirectReference sMask;
    if (!isOpaque) {
        sMask = doc->reserveRef();
    }
    SkDynamicMemoryWStream buffer;
    SkDeflateWStream deflateWStream(&buffer);
    const char* colorSpace = "DeviceGray";
    switch (pm.colorType()) {
        case kAlpha_8_SkColorType:
            fill_stream(&deflateWStream, '\x00', pm.width() * pm.height());
            break;
        case kGray_8_SkColorType:
            SkASSERT(sMask.fValue = -1);
            SkASSERT(pm.rowBytes() == (size_t)pm.width());
            deflateWStream.write(pm.addr8(), pm.width() * pm.height());
            break;
        default:
            colorSpace = "DeviceRGB";
            SkASSERT(pm.alphaType() == kUnpremul_SkAlphaType);
            SkASSERT(pm.colorType() == kBGRA_8888_SkColorType);
            SkASSERT(pm.rowBytes() == (size_t)pm.width() * 4);
            uint8_t byteBuffer[3072];
            static_assert(SK_ARRAY_COUNT(byteBuffer) % 3 == 0, "");
            uint8_t* bufferStop = byteBuffer + SK_ARRAY_COUNT(byteBuffer);
            uint8_t* dst = byteBuffer;
            for (int y = 0; y < pm.height(); ++y) {
                const SkColor* src = pm.addr32(0, y);
                for (int x = 0; x < pm.width(); ++x) {
                    SkColor color = *src++;
                    if (SkColorGetA(color) == SK_AlphaTRANSPARENT) {
                        color = get_neighbor_avg_color(pm, x, y);
                    }
                    *dst++ = SkColorGetR(color);
                    *dst++ = SkColorGetG(color);
                    *dst++ = SkColorGetB(color);
                    if (dst == bufferStop) {
                        deflateWStream.write(byteBuffer, sizeof(byteBuffer));
                        dst = byteBuffer;
                    }
                }
            }
            deflateWStream.write(byteBuffer, dst - byteBuffer);
    }
    deflateWStream.finalize();
    #ifdef SK_PDF_BASE85_BINARY
    SkPDFUtils::Base85Encode(buffer.detachAsStream(), &buffer);
    #endif
    int length = SkToInt(buffer.bytesWritten());
    emit_image_stream(doc, ref, [&buffer](SkWStream* stream) { buffer.writeToAndReset(stream); },
                      pm.info().dimensions(), colorSpace, sMask, length, false);
    if (!isOpaque) {
        do_deflated_alpha(pm, doc, sMask);
    }
}

static bool do_jpeg(sk_sp<SkData> data, SkPDFDocument* doc, SkISize size,
                    SkPDFIndirectReference ref) {
    SkISize jpegSize;
    SkEncodedInfo::Color jpegColorType;
    SkEncodedOrigin exifOrientation;
    if (!SkGetJpegInfo(data->data(), data->size(), &jpegSize,
                       &jpegColorType, &exifOrientation)) {
        return false;
    }
    bool yuv = jpegColorType == SkEncodedInfo::kYUV_Color;
    bool goodColorType = yuv || jpegColorType == SkEncodedInfo::kGray_Color;
    if (jpegSize != size  // Sanity check.
            || !goodColorType
            || kTopLeft_SkEncodedOrigin != exifOrientation) {
        return false;
    }
    #ifdef SK_PDF_BASE85_BINARY
    SkDynamicMemoryWStream buffer;
    SkPDFUtils::Base85Encode(SkMemoryStream::MakeDirect(data->data(), data->size()), &buffer);
    data = buffer.detachAsData();
    #endif

    emit_image_stream(doc, ref,
                      [&data](SkWStream* dst) { dst->write(data->data(), data->size()); },
                      jpegSize, yuv ? "DeviceRGB" : "DeviceGray",
                      SkPDFIndirectReference(), SkToInt(data->size()), true);
    return true;
}

static SkBitmap to_pixels(const SkImage* image) {
    SkBitmap bm;
    int w = image->width(),
        h = image->height();
    switch (image->colorType()) {
        case kAlpha_8_SkColorType:
            bm.allocPixels(SkImageInfo::MakeA8(w, h));
            break;
        case kGray_8_SkColorType:
            bm.allocPixels(SkImageInfo::Make(w, h, kGray_8_SkColorType, kOpaque_SkAlphaType));
            break;
        default: {
            // TODO: makeColorSpace(sRGB) or actually tag the images
            SkAlphaType at = bm.isOpaque() ? kOpaque_SkAlphaType : kUnpremul_SkAlphaType;
            bm.allocPixels(SkImageInfo::Make(w, h, kBGRA_8888_SkColorType, at));
        }
    }
    if (!image->readPixels(bm.pixmap(), 0, 0)) {
        bm.eraseColor(SkColorSetARGB(0xFF, 0, 0, 0));
    }
<<<<<<< HEAD
    void drop() override { fImage = nullptr; fSMask = nullptr; }
    PDFDefaultBitmap(sk_sp<SkImage> image, sk_sp<SkPDFObject> smask)
        : fImage(std::move(image)), fSMask(std::move(smask)) { SkASSERT(fImage); }

private:
    sk_sp<SkImage> fImage;
    sk_sp<SkPDFObject> fSMask;
};
}  // namespace

////////////////////////////////////////////////////////////////////////////////

namespace {
/**
 *  This PDFObject assumes that its constructor was handed YUV or
 *  Grayscale JFIF Jpeg-encoded data that can be directly embedded
 *  into a PDF.
 */
class PDFJpegBitmap final : public SkPDFObject {
public:
    SkISize fSize;
    sk_sp<SkData> fData;
    bool fIsYUV;
    PDFJpegBitmap(SkISize size, sk_sp<SkData> data, bool isYUV)
        : fSize(size), fData(std::move(data)), fIsYUV(isYUV) { SkASSERT(fData); }
    void emitObject(SkWStream*, const SkPDFObjNumMap&) const override;
    void drop() override { fData = nullptr; }
};

void PDFJpegBitmap::emitObject(SkWStream* stream,
                               const SkPDFObjNumMap& objNumMap) const {
    SkASSERT(fData);
    SkPDFDict pdfDict("XObject");
    pdfDict.insertName("Subtype", "Image");
    pdfDict.insertInt("Width", fSize.width());
    pdfDict.insertInt("Height", fSize.height());
    if (fIsYUV) {
        pdfDict.insertName("ColorSpace", "DeviceRGB");
    } else {
        pdfDict.insertName("ColorSpace", "DeviceGray");
    }
    pdfDict.insertInt("BitsPerComponent", 8);
    pdfDict.insertName("Filter", "DCTDecode");
    pdfDict.insertInt("ColorTransform", 0);
    pdfDict.insertInt("Length", SkToInt(fData->size()));
    pdfDict.emitObject(stream, objNumMap);
    stream->writeText(kStreamBegin);
    stream->write(fData->data(), fData->size());
    stream->writeText(kStreamEnd);
||||||| merged common ancestors
    void drop() override { fImage = nullptr; fSMask = nullptr; }
    PDFDefaultBitmap(sk_sp<SkImage> image, sk_sp<SkPDFObject> smask)
        : fImage(std::move(image)), fSMask(std::move(smask)) { SkASSERT(fImage); }

private:
    sk_sp<SkImage> fImage;
    sk_sp<SkPDFObject> fSMask;
};
}  // namespace

////////////////////////////////////////////////////////////////////////////////

namespace {
/**
 *  This PDFObject assumes that its constructor was handed YUV or
 *  Grayscale JFIF Jpeg-encoded data that can be directly embedded
 *  into a PDF.
 */
class PDFJpegBitmap final : public SkPDFObject {
public:
    SkISize fSize;
    sk_sp<SkData> fData;
    bool fIsYUV;
    PDFJpegBitmap(SkISize size, SkData* data, bool isYUV)
        : fSize(size), fData(SkRef(data)), fIsYUV(isYUV) { SkASSERT(data); }
    void emitObject(SkWStream*, const SkPDFObjNumMap&) const override;
    void drop() override { fData = nullptr; }
};

void PDFJpegBitmap::emitObject(SkWStream* stream,
                               const SkPDFObjNumMap& objNumMap) const {
    SkASSERT(fData);
    SkPDFDict pdfDict("XObject");
    pdfDict.insertName("Subtype", "Image");
    pdfDict.insertInt("Width", fSize.width());
    pdfDict.insertInt("Height", fSize.height());
    if (fIsYUV) {
        pdfDict.insertName("ColorSpace", "DeviceRGB");
    } else {
        pdfDict.insertName("ColorSpace", "DeviceGray");
    }
    pdfDict.insertInt("BitsPerComponent", 8);
    pdfDict.insertName("Filter", "DCTDecode");
    pdfDict.insertInt("ColorTransform", 0);
    pdfDict.insertInt("Length", SkToInt(fData->size()));
    pdfDict.emitObject(stream, objNumMap);
    stream->writeText(kStreamBegin);
    stream->write(fData->data(), fData->size());
    stream->writeText(kStreamEnd);
=======
    return bm;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
////////////////////////////////////////////////////////////////////////////////
sk_sp<PDFJpegBitmap> make_jpeg_bitmap(sk_sp<SkData> data, SkISize size) {
    SkISize jpegSize;
    SkEncodedInfo::Color jpegColorType;
    SkEncodedOrigin exifOrientation;
    if (data && SkGetJpegInfo(data->data(), data->size(), &jpegSize,
                              &jpegColorType, &exifOrientation)) {
        bool yuv = jpegColorType == SkEncodedInfo::kYUV_Color;
        bool goodColorType = yuv || jpegColorType == SkEncodedInfo::kGray_Color;
        if (jpegSize == size  // Sanity check.
                && goodColorType
                && kTopLeft_SkEncodedOrigin == exifOrientation) {
            // hold on to data, not image.
            #ifdef SK_PDF_IMAGE_STATS
            gJpegImageObjects.fetch_add(1);
            #endif
            return sk_make_sp<PDFJpegBitmap>(jpegSize, std::move(data), yuv);
        }
||||||| merged common ancestors
////////////////////////////////////////////////////////////////////////////////

sk_sp<SkPDFObject> SkPDFCreateBitmapObject(sk_sp<SkImage> image, int encodingQuality) {
    SkASSERT(image);
    SkASSERT(encodingQuality >= 0);
    sk_sp<SkData> data = image->refEncodedData();
    SkJFIFInfo info;
    if (data && SkIsJFIF(data.get(), &info)) {
        bool yuv = info.fType == SkJFIFInfo::kYCbCr;
        if (info.fSize == image->dimensions()) {  // Sanity check.
            // hold on to data, not image.
            #ifdef SK_PDF_IMAGE_STATS
            gJpegImageObjects.fetch_add(1);
            #endif
            return sk_make_sp<PDFJpegBitmap>(info.fSize, data.get(), yuv);
        }
=======
void serialize_image(const SkImage* img,
                     int encodingQuality,
                     SkPDFDocument* doc,
                     SkPDFIndirectReference ref) {
    SkASSERT(img);
    SkASSERT(doc);
    SkASSERT(encodingQuality >= 0);
    SkISize dimensions = img->dimensions();
    sk_sp<SkData> data = img->refEncodedData();
    if (data && do_jpeg(std::move(data), doc, dimensions, ref)) {
        return;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    return nullptr;
}

sk_sp<SkPDFObject> SkPDFCreateBitmapObject(sk_sp<SkImage> image, int encodingQuality) {
    SkASSERT(image);
    SkASSERT(encodingQuality >= 0);
    SkISize dimensions = image->dimensions();
    sk_sp<SkData> data = image->refEncodedData();
    if (auto jpeg = make_jpeg_bitmap(std::move(data), dimensions)) {
        return std::move(jpeg);
    }

    const bool isOpaque = image_compute_is_opaque(image.get());

||||||| merged common ancestors

    const bool isOpaque = image_compute_is_opaque(image.get());

=======
    SkBitmap bm = to_pixels(img);
    SkPixmap pm = bm.pixmap();
    bool isOpaque = pm.isOpaque() || pm.computeIsOpaque();
>>>>>>> upstream-releases
    if (encodingQuality <= 100 && isOpaque) {
<<<<<<< HEAD
        data = image->encodeToData(SkEncodedImageFormat::kJPEG, encodingQuality);
        if (auto jpeg = make_jpeg_bitmap(std::move(data), dimensions)) {
            return std::move(jpeg);
||||||| merged common ancestors
        data = image->encodeToData(SkEncodedImageFormat::kJPEG, encodingQuality);
        if (data && SkIsJFIF(data.get(), &info)) {
            bool yuv = info.fType == SkJFIFInfo::kYCbCr;
            if (info.fSize == image->dimensions()) {  // Sanity check.
                return sk_make_sp<PDFJpegBitmap>(info.fSize, data.get(), yuv);
            }
=======
        sk_sp<SkData> data = img->encodeToData(SkEncodedImageFormat::kJPEG, encodingQuality);
        if (data && do_jpeg(std::move(data), doc, dimensions, ref)) {
            return;
>>>>>>> upstream-releases
        }
    }
    do_deflated_image(pm, doc, isOpaque, ref);
}

SkPDFIndirectReference SkPDFSerializeImage(const SkImage* img,
                                           SkPDFDocument* doc,
                                           int encodingQuality) {
    SkASSERT(img);
    SkASSERT(doc);
    SkPDFIndirectReference ref = doc->reserveRef();
    if (SkExecutor* executor = doc->executor()) {
        SkRef(img);
        doc->incrementJobCount();
        executor->add([img, encodingQuality, doc, ref]() {
            serialize_image(img, encodingQuality, doc, ref);
            SkSafeUnref(img);
            doc->signalJobComplete();
        });
        return ref;
    }
    serialize_image(img, encodingQuality, doc, ref);
    return ref;
}
