/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

<<<<<<< HEAD
#include "SkBitmapProcShader.h"
#include "SkColorFilter.h"
#include "SkColorFilterShader.h"
#include "SkColorShader.h"
#include "SkComposeShader.h"
#include "SkEmptyShader.h"
#include "SkFlattenablePriv.h"
#include "SkImageShader.h"
#include "SkLocalMatrixShader.h"
#include "SkMatrixImageFilter.h"
||||||| merged common ancestors
#include "SkBitmapProcShader.h"
#include "SkColorFilter.h"
#include "SkColorFilterShader.h"
#include "SkColorShader.h"
#include "SkComposeShader.h"
#include "SkEmptyShader.h"
#include "SkFlattenable.h"
#include "SkImageShader.h"
#include "SkLocalMatrixShader.h"
#include "SkMatrixImageFilter.h"
=======
#include "SkFlattenable.h"
>>>>>>> upstream-releases
#include "SkOnce.h"

<<<<<<< HEAD
/*
 *  Registers all of the required effects subclasses for picture deserialization.
 *
 *  Optional subclasses (e.g. Blur) should be registered in the ports/ version of this file,
 *  inside the InitEffects() method.
 */
void SkFlattenable::PrivateInitializer::InitCore() {
    // Shader
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkColorFilterShader)
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkColorShader)
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkColor4Shader)
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkComposeShader)
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkEmptyShader)
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkLocalMatrixShader)
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkPictureShader)


    // ImageFilter
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkMatrixImageFilter)

    SkColorFilter::InitializeFlattenables();
    SkPathEffect::InitializeFlattenables();
    SkShaderBase::InitializeFlattenables();

    // Drawable
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkRecordedDrawable)

    // Now initialize any optional/additional effects (implemented in src/ports)
    InitEffects();
    InitImageFilters();

    // Finalize flattenable initialization.
    SkFlattenable::Finalize();
};

void SkFlattenable::InitializeFlattenablesIfNeeded() {
||||||| merged common ancestors
/*
 *  Registers all of the required effects subclasses for picture deserialization.
 *
 *  Optional subclasses (e.g. Blur) should be registered in the ports/ version of this file,
 *  inside the InitEffects() method.
 */
void SkFlattenable::PrivateInitializer::InitCore() {
    // Shader
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkColorFilterShader)
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkColorShader)
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkColor4Shader)
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkComposeShader)
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkEmptyShader)
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkLocalMatrixShader)
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkPictureShader)


    // ImageFilter
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkMatrixImageFilter)

    SkColorFilter::InitializeFlattenables();
    SkPathEffect::InitializeFlattenables();
    SkShaderBase::InitializeFlattenables();

    // Drawable
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkRecordedDrawable)

    // Now initialize any optional/additional effects (implemented in src/ports)
    InitEffects();

    // Finalize flattenable initialization.
    SkFlattenable::Finalize();
};

void SkFlattenable::InitializeFlattenablesIfNeeded() {
=======
void SkFlattenable::RegisterFlattenablesIfNeeded() {
>>>>>>> upstream-releases
    static SkOnce once;
    once([]{
        SkFlattenable::PrivateInitializer::InitEffects();
        SkFlattenable::PrivateInitializer::InitImageFilters();
        SkFlattenable::Finalize();
    });
}
