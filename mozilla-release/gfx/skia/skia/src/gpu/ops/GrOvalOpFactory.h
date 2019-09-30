/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrOvalOpFactory_DEFINED
#define GrOvalOpFactory_DEFINED

#include "GrColor.h"
#include "SkRefCnt.h"

class GrContext;
class GrDrawOp;
class GrPaint;
class GrRecordingContext;
class GrShaderCaps;
class GrStyle;
class SkMatrix;
struct SkRect;
class SkRRect;
class SkStrokeRec;

/*
 * This namespace wraps helper functions that draw ovals, rrects, and arcs (filled & stroked)
 */
class GrOvalOpFactory {
public:
<<<<<<< HEAD
    static std::unique_ptr<GrDrawOp> MakeOvalOp(GrContext*,
                                                GrPaint&&,
||||||| merged common ancestors
    static std::unique_ptr<GrDrawOp> MakeOvalOp(GrPaint&&,
=======
    static std::unique_ptr<GrDrawOp> MakeOvalOp(GrRecordingContext*,
                                                GrPaint&&,
>>>>>>> upstream-releases
                                                const SkMatrix&,
                                                const SkRect& oval,
                                                const GrStyle& style,
                                                const GrShaderCaps*);

<<<<<<< HEAD
    static std::unique_ptr<GrDrawOp> MakeRRectOp(GrContext*,
                                                 GrPaint&&,
||||||| merged common ancestors
    static std::unique_ptr<GrDrawOp> MakeRRectOp(GrPaint&&,
=======
    static std::unique_ptr<GrDrawOp> MakeRRectOp(GrRecordingContext*,
                                                 GrPaint&&,
>>>>>>> upstream-releases
                                                 const SkMatrix&,
                                                 const SkRRect&,
                                                 const SkStrokeRec&,
                                                 const GrShaderCaps*);

<<<<<<< HEAD
    static std::unique_ptr<GrDrawOp> MakeArcOp(GrContext*,
                                               GrPaint&&,
||||||| merged common ancestors
    static std::unique_ptr<GrDrawOp> MakeArcOp(GrPaint&&,
=======
    static std::unique_ptr<GrDrawOp> MakeArcOp(GrRecordingContext*,
                                               GrPaint&&,
>>>>>>> upstream-releases
                                               const SkMatrix&,
                                               const SkRect& oval,
                                               SkScalar startAngle,
                                               SkScalar sweepAngle,
                                               bool useCenter,
                                               const GrStyle&,
                                               const GrShaderCaps*);
};

#endif  // GrOvalOpFactory_DEFINED
