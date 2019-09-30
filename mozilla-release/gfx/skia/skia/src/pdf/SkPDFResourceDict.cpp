/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkPDFResourceDict.h"
#include "SkPDFTypes.h"
#include "SkStream.h"

// Sanity check that the values of enum ResourceType correspond to the
// expected values as defined in the arrays below.
// If these are failing, you may need to update the kResourceTypePrefixes
// and kResourceTypeNames arrays below.
static_assert(0 == (int)SkPDFResourceType::kExtGState, "resource_type_mismatch");
static_assert(1 == (int)SkPDFResourceType::kPattern,   "resource_type_mismatch");
static_assert(2 == (int)SkPDFResourceType::kXObject,   "resource_type_mismatch");
static_assert(3 == (int)SkPDFResourceType::kFont,      "resource_type_mismatch");

// One extra character for the Prefix.
constexpr size_t kMaxResourceNameLength = 1 + SkStrAppendS32_MaxSize;

<<<<<<< HEAD
// returns pointer just past end of what's written into `dst`.
static char* get_resource_name(char dst[kMaxResourceNameLength], SkPDFResourceType type, int key) {
    static const char kResourceTypePrefixes[] = {
        'G',  // kExtGState
        'P',  // kPattern
        'X',  // kXObject
        'F'   // kFont
    };
    SkASSERT((unsigned)type < SK_ARRAY_COUNT(kResourceTypePrefixes));
    dst[0] = kResourceTypePrefixes[(unsigned)type];
    return SkStrAppendS32(dst + 1, key);
}

void SkPDFWriteResourceName(SkWStream* dst, SkPDFResourceType type, int key) {
    // One extra character for the leading '/'.
    char buffer[1 + kMaxResourceNameLength];
    buffer[0] = '/';
    char* end = get_resource_name(buffer + 1, type, key);
    dst->write(buffer, (size_t)(end - buffer));
||||||| merged common ancestors
static const char* resource_type_names[] = {
        "ExtGState",
        "Pattern",
        "XObject",
        "Font"
};

char SkPDFResourceDict::GetResourceTypePrefix(
        SkPDFResourceDict::SkPDFResourceType type) {
    SkASSERT(type >= 0);
    SkASSERT(type < SkPDFResourceDict::kResourceTypeCount);

    return resource_type_prefixes[type];
}

static const char* get_resource_type_name(
        SkPDFResourceDict::SkPDFResourceType type) {
    SkASSERT(type >= 0);
    SkASSERT(type < SK_ARRAY_COUNT(resource_type_names));

    return resource_type_names[type];
=======
// returns pointer just past end of what's written into `dst`.
static char* get_resource_name(char dst[kMaxResourceNameLength], SkPDFResourceType type, int key) {
    static const char kResourceTypePrefixes[] = {
        'G',  // kExtGState
        'P',  // kPattern
        'X',  // kXObject
        'F'   // kFont
    };
    SkASSERT((unsigned)type < SK_ARRAY_COUNT(kResourceTypePrefixes));
    dst[0] = kResourceTypePrefixes[(unsigned)type];
    return SkStrAppendS32(dst + 1, key);
}

void SkPDFWriteResourceName(SkWStream* dst, SkPDFResourceType type, int key) {
    // One extra character for the leading '/'.
    char buffer[1 + kMaxResourceNameLength];
    buffer[0] = '/';
    char* end = get_resource_name(buffer + 1, type, key);
    dst->write(buffer, (size_t)(end - buffer));
}

static const char* resource_name(SkPDFResourceType type) {
    static const char* kResourceTypeNames[] = {
        "ExtGState",
        "Pattern",
        "XObject",
        "Font"
    };
    SkASSERT((unsigned)type < SK_ARRAY_COUNT(kResourceTypeNames));
    return kResourceTypeNames[(unsigned)type];
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename T>
static void add_subdict(std::vector<sk_sp<T>> resourceList,
                        SkPDFResourceType type,
                        SkPDFDict* dst) {
    if (0 == resourceList.size()) {
        return;
    }
    auto resources = sk_make_sp<SkPDFDict>();
    for (size_t i = 0; i < resourceList.size(); i++) {
        char buffer[kMaxResourceNameLength];
        char* end = get_resource_name(buffer, type, SkToInt(i));
        resources->insertObjRef(SkString(buffer, (size_t)(end - buffer)),
                                std::move(resourceList[i]));
||||||| merged common ancestors
SkString SkPDFResourceDict::getResourceName(
        SkPDFResourceDict::SkPDFResourceType type, int key) {
    return SkStringPrintf("%c%d", SkPDFResourceDict::GetResourceTypePrefix(type), key);
}

static void add_subdict(
        const SkTDArray<SkPDFObject*>& resourceList,
        SkPDFResourceDict::SkPDFResourceType type,
        SkPDFDict* dst) {
    if (0 == resourceList.count()) {
        return;
    }
    auto resources = sk_make_sp<SkPDFDict>();
    for (int i = 0; i < resourceList.count(); i++) {
        resources->insertObjRef(SkPDFResourceDict::getResourceName(type, i),
                                sk_ref_sp(resourceList[i]));
=======
static SkString resource(SkPDFResourceType type, int index) {
    char buffer[kMaxResourceNameLength];
    char* end = get_resource_name(buffer, type, index);
    return SkString(buffer, (size_t)(end - buffer));
}

static void add_subdict(const std::vector<SkPDFIndirectReference>& resourceList,
                        SkPDFResourceType type,
                        SkPDFDict* dst) {
    if (!resourceList.empty()) {
        auto resources = SkPDFMakeDict();
        for (SkPDFIndirectReference ref : resourceList) {
            resources->insertRef(resource(type, ref.fValue), ref);
        }
        dst->insertObject(resource_name(type), std::move(resources));
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    static const char* kResourceTypeNames[] = {
        "ExtGState",
        "Pattern",
        "XObject",
        "Font"
    };
    SkASSERT((unsigned)type < SK_ARRAY_COUNT(kResourceTypeNames));
    dst->insertObject(kResourceTypeNames[(unsigned)type], std::move(resources));
||||||| merged common ancestors
    dst->insertObject(get_resource_type_name(type), std::move(resources));
=======
>>>>>>> upstream-releases
}

<<<<<<< HEAD
sk_sp<SkPDFDict> SkPDFMakeResourceDict(std::vector<sk_sp<SkPDFObject>> graphicStateResources,
                                       std::vector<sk_sp<SkPDFObject>> shaderResources,
                                       std::vector<sk_sp<SkPDFObject>> xObjectResources,
                                       std::vector<sk_sp<SkPDFFont>> fontResources) {
    auto dict = sk_make_sp<SkPDFDict>();
    add_subdict(std::move(graphicStateResources), SkPDFResourceType::kExtGState, dict.get());
    add_subdict(std::move(shaderResources),       SkPDFResourceType::kPattern,   dict.get());
    add_subdict(std::move(xObjectResources),      SkPDFResourceType::kXObject,   dict.get());
    add_subdict(std::move(fontResources),         SkPDFResourceType::kFont,      dict.get());
||||||| merged common ancestors
sk_sp<SkPDFDict> SkPDFResourceDict::Make(
        const SkTDArray<SkPDFObject*>* gStateResources,
        const SkTDArray<SkPDFObject*>* patternResources,
        const SkTDArray<SkPDFObject*>* xObjectResources,
        const SkTDArray<SkPDFObject*>* fontResources) {
    auto dict = sk_make_sp<SkPDFDict>();
    static const char kProcs[][7] = {
        "PDF", "Text", "ImageB", "ImageC", "ImageI"};
    auto procSets = sk_make_sp<SkPDFArray>();

    procSets->reserve(SK_ARRAY_COUNT(kProcs));
    for (size_t i = 0; i < SK_ARRAY_COUNT(kProcs); i++) {
        procSets->appendName(kProcs[i]);
    }
    dict->insertObject("ProcSets", std::move(procSets));

    if (gStateResources) {
        add_subdict(*gStateResources, kExtGState_ResourceType, dict.get());
    }
    if (patternResources) {
        add_subdict(*patternResources, kPattern_ResourceType, dict.get());
    }
    if (xObjectResources) {
        add_subdict(*xObjectResources, kXObject_ResourceType, dict.get());
    }
    if (fontResources) {
        add_subdict(*fontResources, kFont_ResourceType, dict.get());
    }
=======
static std::unique_ptr<SkPDFArray> make_proc_set() {
    auto procSets = SkPDFMakeArray();
    static const char kProcs[][7] = { "PDF", "Text", "ImageB", "ImageC", "ImageI"};
    procSets->reserve(SK_ARRAY_COUNT(kProcs));
    for (const char* proc : kProcs) {
        procSets->appendName(proc);
    }
    return procSets;
}

std::unique_ptr<SkPDFDict> SkPDFMakeResourceDict(
        const std::vector<SkPDFIndirectReference>& graphicStateResources,
        const std::vector<SkPDFIndirectReference>& shaderResources,
        const std::vector<SkPDFIndirectReference>& xObjectResources,
        const std::vector<SkPDFIndirectReference>& fontResources) {
    auto dict = SkPDFMakeDict();
    dict->insertObject("ProcSets", make_proc_set());
    add_subdict(graphicStateResources, SkPDFResourceType::kExtGState, dict.get());
    add_subdict(shaderResources,       SkPDFResourceType::kPattern,   dict.get());
    add_subdict(xObjectResources,      SkPDFResourceType::kXObject,   dict.get());
    add_subdict(fontResources,         SkPDFResourceType::kFont,      dict.get());
>>>>>>> upstream-releases
    return dict;
}
