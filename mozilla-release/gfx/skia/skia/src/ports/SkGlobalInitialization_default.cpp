/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

<<<<<<< HEAD
#include "SkFlattenablePriv.h"
#include "SkMaskFilter.h"
#include "../../src/effects/SkDashImpl.h"
#include "SkGradientShader.h"
||||||| merged common ancestors
#include "../../src/effects/SkDashImpl.h"
#include "SkGradientShader.h"
#include "SkImageSource.h"

// Security note:
//
// As new subclasses are added here, they should be reviewed by chrome security before they
// support deserializing cross-process: chrome-security@google.com. SampleFilterFuzz.cpp should
// also be amended to exercise the new subclass.
//
// See SkReadBuffer::isCrossProcess() and SkPicture::PictureIOSecurityPrecautionsEnabled()
//
=======
#include "SkFlattenable.h"
>>>>>>> upstream-releases

#if defined(SK_DISABLE_EFFECT_DESERIALIZATION)

    void SkFlattenable::PrivateInitializer::InitEffects() {}
    void SkFlattenable::PrivateInitializer::InitImageFilters() {}

#else

    #include "../../src/effects/SkDashImpl.h"
    #include "SkGradientShader.h"
    #include "SkMaskFilter.h"
    #include "SkImageFilter.h"

    /*
     *  Register most effects for deserialization.
     *
     *  None of these are strictly required for Skia to operate, so if you're
     *  not using deserialization yourself, you can define
     *  SK_DISABLE_EFFECT_SERIALIZATION, or modify/replace this file as needed.
     */
    void SkFlattenable::PrivateInitializer::InitEffects() {
        // Shaders.
        SkGradientShader::RegisterFlattenables();

        // Mask filters.
        SkMaskFilter::RegisterFlattenables();

        // Path effects.
        SK_REGISTER_FLATTENABLE(SkDashImpl);
    }

<<<<<<< HEAD
    // Mask filters.
    SkMaskFilter::InitializeFlattenables();

    // PathEffect
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkDashImpl)
}
||||||| merged common ancestors
    // PathEffect
    SK_DEFINE_FLATTENABLE_REGISTRAR_ENTRY(SkDashImpl)

    // ImageFilter
    SkImageFilter::InitializeFlattenables();
}
=======
    /*
     *  Register SkImageFilters for deserialization.
     *
     *  None of these are strictly required for Skia to operate, so if you're
     *  not using deserialization yourself, you can define
     *  SK_DISABLE_EFFECT_SERIALIZATION, or modify/replace this file as needed.
     */
    void SkFlattenable::PrivateInitializer::InitImageFilters() {
        SkImageFilter::RegisterFlattenables();
    }

#endif
>>>>>>> upstream-releases
