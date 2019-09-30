/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrShadowRRectOp_DEFINED
#define GrShadowRRectOp_DEFINED

#include <memory>
#include "GrColor.h"

class GrContext;
class GrDrawOp;
class GrRecordingContext;

class SkMatrix;
class SkRRect;

namespace GrShadowRRectOp {

<<<<<<< HEAD
std::unique_ptr<GrDrawOp> Make(GrContext*,
                               GrColor,
                               const SkMatrix& viewMatrix,
                               const SkRRect& rrect,
                               SkScalar blurWidth,
                               SkScalar insetWidth);
||||||| merged common ancestors
std::unique_ptr<GrDrawOp> Make(GrColor, const SkMatrix& viewMatrix, const SkRRect& rrect,
                               SkScalar blurWidth, SkScalar insetWidth, SkScalar blurClamp = 1);
=======
std::unique_ptr<GrDrawOp> Make(GrRecordingContext*,
                               GrColor,
                               const SkMatrix& viewMatrix,
                               const SkRRect&,
                               SkScalar blurWidth,
                               SkScalar insetWidth);
>>>>>>> upstream-releases
}

#endif
