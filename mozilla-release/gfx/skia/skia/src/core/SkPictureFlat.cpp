/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkPictureFlat.h"
#include "SkChecksum.h"
#include "SkColorFilter.h"
#include "SkDrawLooper.h"
#include "SkMaskFilter.h"
#include "SkShader.h"
#include "SkTypeface.h"

///////////////////////////////////////////////////////////////////////////////

<<<<<<< HEAD
SkTypefacePlayback::~SkTypefacePlayback() {}

void SkTypefacePlayback::setCount(size_t count) {
||||||| merged common ancestors
SkTypefacePlayback::SkTypefacePlayback() : fCount(0), fArray(nullptr) {}

SkTypefacePlayback::~SkTypefacePlayback() {
    this->reset(nullptr);
}

void SkTypefacePlayback::reset(const SkRefCntSet* rec) {
    for (int i = 0; i < fCount; i++) {
        SkASSERT(fArray[i]);
        fArray[i]->unref();
    }
    delete[] fArray;

    if (rec!= nullptr && rec->count() > 0) {
        fCount = rec->count();
        fArray = new SkRefCnt* [fCount];
        rec->copyToArray(fArray);
        for (int i = 0; i < fCount; i++) {
            fArray[i]->ref();
        }
    } else {
        fCount = 0;
        fArray = nullptr;
    }
}

void SkTypefacePlayback::setCount(int count) {
    this->reset(nullptr);

=======
void SkTypefacePlayback::setCount(size_t count) {
>>>>>>> upstream-releases
    fCount = count;
    fArray.reset(new sk_sp<SkTypeface>[count]);
}
