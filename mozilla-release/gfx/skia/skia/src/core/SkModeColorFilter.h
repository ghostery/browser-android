/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

<<<<<<< HEAD
#include "SkColorFilter.h"
#include "SkFlattenable.h"

||||||| merged common ancestors
#include "SkColorFilter.h"

=======
>>>>>>> upstream-releases
#ifndef SkModeColorFilter_DEFINED
#define SkModeColorFilter_DEFINED

#include "SkColorFilter.h"
#include "SkFlattenable.h"

class SkModeColorFilter : public SkColorFilter {
public:
    static sk_sp<SkColorFilter> Make(SkColor color, SkBlendMode mode) {
        return sk_sp<SkColorFilter>(new SkModeColorFilter(color, mode));
    }

    bool asColorMode(SkColor*, SkBlendMode*) const override;
    uint32_t getFlags() const override;

<<<<<<< HEAD
    Factory getFactory() const override { return CreateProc; }

||||||| merged common ancestors
#ifndef SK_IGNORE_TO_STRING
    void toString(SkString* str) const override;
#endif

=======
>>>>>>> upstream-releases
#if SK_SUPPORT_GPU
    std::unique_ptr<GrFragmentProcessor> asFragmentProcessor(
            GrRecordingContext*, const GrColorSpaceInfo&) const override;
#endif

protected:
    SkModeColorFilter(SkColor color, SkBlendMode mode);

    void flatten(SkWriteBuffer&) const override;

    void onAppendStages(SkRasterPipeline*, SkColorSpace*, SkArenaAlloc*,
                        bool shaderIsOpaque) const override;

    sk_sp<SkColorFilter> onMakeColorSpace(SkColorSpaceXformer*) const override;

private:
<<<<<<< HEAD
    static sk_sp<SkFlattenable> CreateProc(SkReadBuffer&);
    friend class SkFlattenable::PrivateInitializer;

||||||| merged common ancestors
=======
    SK_FLATTENABLE_HOOKS(SkModeColorFilter)

>>>>>>> upstream-releases
    SkColor     fColor;
    SkBlendMode fMode;

    friend class SkColorFilter;

    typedef SkColorFilter INHERITED;
};

#endif
