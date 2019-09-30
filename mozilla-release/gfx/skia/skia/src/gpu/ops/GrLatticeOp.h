/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GLatticeOp_DEFINED
#define GLatticeOp_DEFINED

#include <memory>
#include "GrSamplerState.h"
#include "SkRefCnt.h"

<<<<<<< HEAD
class GrContext;
||||||| merged common ancestors
=======
class GrColorSpaceXform;
>>>>>>> upstream-releases
class GrDrawOp;
class GrPaint;
class SkLatticeIter;
<<<<<<< HEAD
class GrTextureProxy;
class GrColorSpaceXform;
||||||| merged common ancestors
=======
class GrRecordingContext;
class GrTextureProxy;
>>>>>>> upstream-releases
class SkMatrix;
struct SkRect;

namespace GrLatticeOp {
<<<<<<< HEAD
std::unique_ptr<GrDrawOp> MakeNonAA(GrContext*,
                                    GrPaint&&,
                                    const SkMatrix& viewMatrix,
                                    sk_sp<GrTextureProxy>,
                                    sk_sp<GrColorSpaceXform>,
                                    GrSamplerState::Filter,
                                    std::unique_ptr<SkLatticeIter>,
||||||| merged common ancestors
std::unique_ptr<GrDrawOp> MakeNonAA(GrPaint&& paint, const SkMatrix& viewMatrix, int imageWidth,
                                    int imageHeight, std::unique_ptr<SkLatticeIter> iter,
=======
std::unique_ptr<GrDrawOp> MakeNonAA(GrRecordingContext*,
                                    GrPaint&&,
                                    const SkMatrix& viewMatrix,
                                    sk_sp<GrTextureProxy>,
                                    sk_sp<GrColorSpaceXform>,
                                    GrSamplerState::Filter,
                                    std::unique_ptr<SkLatticeIter>,
>>>>>>> upstream-releases
                                    const SkRect& dst);
};

#endif
