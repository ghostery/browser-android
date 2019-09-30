/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkPDFMetadata_DEFINED
#define SkPDFMetadata_DEFINED

<<<<<<< HEAD
#include "SkPDFDocument.h"
||||||| merged common ancestors
#include "SkDocument.h"
=======
#include "SkPDFDocument.h"
#include "SkPDFTypes.h"
#include "SkUUID.h"
>>>>>>> upstream-releases

class SkPDFObject;

namespace SkPDFMetadata {
<<<<<<< HEAD
sk_sp<SkPDFObject> MakeDocumentInformationDict(const SkPDF::Metadata&);
||||||| merged common ancestors
sk_sp<SkPDFObject> MakeDocumentInformationDict(const SkDocument::PDFMetadata&);
=======
std::unique_ptr<SkPDFObject> MakeDocumentInformationDict(const SkPDF::Metadata&);
>>>>>>> upstream-releases

SkUUID CreateUUID(const SkPDF::Metadata&);

<<<<<<< HEAD
UUID CreateUUID(const SkPDF::Metadata&);
||||||| merged common ancestors
UUID CreateUUID(const SkDocument::PDFMetadata&);
=======
std::unique_ptr<SkPDFObject> MakePdfId(const SkUUID& doc, const SkUUID& instance);
>>>>>>> upstream-releases

<<<<<<< HEAD
sk_sp<SkPDFObject> MakePdfId(const UUID& doc, const UUID& instance);

sk_sp<SkPDFObject> MakeXMPObject(const SkPDF::Metadata&,
                                 const UUID& doc,
                                 const UUID& instance);
||||||| merged common ancestors
sk_sp<SkPDFObject> MakePdfId(const UUID& doc, const UUID& instance);

sk_sp<SkPDFObject> MakeXMPObject(const SkDocument::PDFMetadata&,
                                 const UUID& doc,
                                 const UUID& instance);
=======
SkPDFIndirectReference MakeXMPObject(const SkPDF::Metadata& metadata,
                                     const SkUUID& doc,
                                     const SkUUID& instance,
                                     SkPDFDocument*);
>>>>>>> upstream-releases
}
#endif  // SkPDFMetadata_DEFINED
