/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrRegionOp.h"

#include "GrCaps.h"
#include "GrDefaultGeoProcFactory.h"
#include "GrDrawOpTest.h"
#include "GrMeshDrawOp.h"
#include "GrOpFlushState.h"
#include "GrResourceProvider.h"
#include "GrSimpleMeshDrawOpHelper.h"
#include "GrVertexWriter.h"
#include "SkMatrixPriv.h"
#include "SkRegion.h"

static const int kVertsPerInstance = 4;
static const int kIndicesPerInstance = 6;

<<<<<<< HEAD
static sk_sp<GrGeometryProcessor> make_gp(const GrShaderCaps* shaderCaps,
                                          const SkMatrix& viewMatrix) {
||||||| merged common ancestors
static sk_sp<GrGeometryProcessor> make_gp(const SkMatrix& viewMatrix) {
=======
static sk_sp<GrGeometryProcessor> make_gp(const GrShaderCaps* shaderCaps,
                                          const SkMatrix& viewMatrix,
                                          bool wideColor) {
>>>>>>> upstream-releases
    using namespace GrDefaultGeoProcFactory;
<<<<<<< HEAD
    return GrDefaultGeoProcFactory::Make(shaderCaps, Color::kPremulGrColorAttribute_Type,
                                         Coverage::kSolid_Type, LocalCoords::kUsePosition_Type,
                                         viewMatrix);
||||||| merged common ancestors
    return GrDefaultGeoProcFactory::Make(Color::kPremulGrColorAttribute_Type, Coverage::kSolid_Type,
                                         LocalCoords::kUsePosition_Type, viewMatrix);
=======
    Color::Type colorType =
        wideColor ? Color::kPremulWideColorAttribute_Type : Color::kPremulGrColorAttribute_Type;
    return GrDefaultGeoProcFactory::Make(shaderCaps, colorType, Coverage::kSolid_Type,
                                         LocalCoords::kUsePosition_Type, viewMatrix);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void tesselate_region(intptr_t vertices,
                             size_t vertexStride,
                             GrColor color,
                             const SkRegion& region) {
    SkRegion::Iterator iter(region);

    intptr_t verts = vertices;
    while (!iter.done()) {
        SkRect rect = SkRect::Make(iter.rect());
        SkPoint* position = (SkPoint*)verts;
        SkPointPriv::SetRectTriStrip(position, rect, vertexStride);

        static const int kColorOffset = sizeof(SkPoint);
        GrColor* vertColor = reinterpret_cast<GrColor*>(verts + kColorOffset);
        for (int i = 0; i < kVertsPerInstance; i++) {
            *vertColor = color;
            vertColor = (GrColor*)((intptr_t)vertColor + vertexStride);
        }

        verts += vertexStride * kVertsPerInstance;
        iter.next();
    }
}

||||||| merged common ancestors
static void tesselate_region(intptr_t vertices,
                             size_t vertexStride,
                             GrColor color,
                             const SkRegion& region) {
    SkRegion::Iterator iter(region);

    intptr_t verts = vertices;
    while (!iter.done()) {
        SkRect rect = SkRect::Make(iter.rect());
        SkPoint* position = (SkPoint*)verts;
        SkPointPriv::SetRectTriStrip(position, rect.fLeft, rect.fTop, rect.fRight, rect.fBottom,
                vertexStride);

        static const int kColorOffset = sizeof(SkPoint);
        GrColor* vertColor = reinterpret_cast<GrColor*>(verts + kColorOffset);
        for (int i = 0; i < kVertsPerInstance; i++) {
            *vertColor = color;
            vertColor = (GrColor*)((intptr_t)vertColor + vertexStride);
        }

        verts += vertexStride * kVertsPerInstance;
        iter.next();
    }
}

=======
>>>>>>> upstream-releases
namespace {

class RegionOp final : public GrMeshDrawOp {
private:
    using Helper = GrSimpleMeshDrawOpHelperWithStencil;

public:
    DEFINE_OP_CLASS_ID

<<<<<<< HEAD
    static std::unique_ptr<GrDrawOp> Make(GrContext* context,
                                          GrPaint&& paint,
                                          const SkMatrix& viewMatrix,
                                          const SkRegion& region,
                                          GrAAType aaType,
||||||| merged common ancestors
    static std::unique_ptr<GrDrawOp> Make(GrPaint&& paint, const SkMatrix& viewMatrix,
                                          const SkRegion& region, GrAAType aaType,
=======
    static std::unique_ptr<GrDrawOp> Make(GrRecordingContext* context,
                                          GrPaint&& paint,
                                          const SkMatrix& viewMatrix,
                                          const SkRegion& region,
                                          GrAAType aaType,
>>>>>>> upstream-releases
                                          const GrUserStencilSettings* stencilSettings = nullptr) {
        return Helper::FactoryHelper<RegionOp>(context, std::move(paint), viewMatrix, region,
                                               aaType, stencilSettings);
    }

    RegionOp(const Helper::MakeArgs& helperArgs, const SkPMColor4f& color,
             const SkMatrix& viewMatrix, const SkRegion& region, GrAAType aaType,
             const GrUserStencilSettings* stencilSettings)
            : INHERITED(ClassID())
            , fHelper(helperArgs, aaType, stencilSettings)
            , fViewMatrix(viewMatrix) {
        RegionInfo& info = fRegions.push_back();
        info.fColor = color;
        info.fRegion = region;

        SkRect bounds = SkRect::Make(region.getBounds());
        this->setTransformedBounds(bounds, viewMatrix, HasAABloat::kNo, IsZeroArea::kNo);
        fWideColor = !SkPMColor4fFitsInBytes(color);
    }

    const char* name() const override { return "GrRegionOp"; }

    void visitProxies(const VisitProxyFunc& func, VisitorType) const override {
        fHelper.visitProxies(func);
    }

#ifdef SK_DEBUG
    SkString dumpInfo() const override {
        SkString str;
        str.appendf("# combined: %d\n", fRegions.count());
        for (int i = 0; i < fRegions.count(); ++i) {
            const RegionInfo& info = fRegions[i];
            str.appendf("%d: Color: 0x%08x, Region with %d rects\n", i, info.fColor.toBytes_RGBA(),
                        info.fRegion.computeRegionComplexity());
        }
        str += fHelper.dumpInfo();
        str += INHERITED::dumpInfo();
        return str;
    }
#endif

    FixedFunctionFlags fixedFunctionFlags() const override { return fHelper.fixedFunctionFlags(); }

<<<<<<< HEAD
    RequiresDstTexture finalize(const GrCaps& caps, const GrAppliedClip* clip) override {
        return fHelper.xpRequiresDstTexture(caps, clip, GrProcessorAnalysisCoverage::kNone,
                                            &fRegions[0].fColor);
||||||| merged common ancestors
    RequiresDstTexture finalize(const GrCaps& caps, const GrAppliedClip* clip,
                                GrPixelConfigIsClamped dstIsClamped) override {
        return fHelper.xpRequiresDstTexture(caps, clip, dstIsClamped,
                                            GrProcessorAnalysisCoverage::kNone, &fRegions[0].fColor);
=======
    GrProcessorSet::Analysis finalize(
            const GrCaps& caps, const GrAppliedClip* clip, GrFSAAType fsaaType) override {
        return fHelper.finalizeProcessors(
                caps, clip, fsaaType, GrProcessorAnalysisCoverage::kNone, &fRegions[0].fColor);
>>>>>>> upstream-releases
    }

private:
    void onPrepareDraws(Target* target) override {
<<<<<<< HEAD
        sk_sp<GrGeometryProcessor> gp = make_gp(target->caps().shaderCaps(), fViewMatrix);
||||||| merged common ancestors
        sk_sp<GrGeometryProcessor> gp = make_gp(fViewMatrix);
=======
        sk_sp<GrGeometryProcessor> gp = make_gp(target->caps().shaderCaps(), fViewMatrix,
                                                fWideColor);
>>>>>>> upstream-releases
        if (!gp) {
            SkDebugf("Couldn't create GrGeometryProcessor\n");
            return;
        }
<<<<<<< HEAD
        static constexpr size_t kVertexStride = sizeof(GrDefaultGeoProcFactory::PositionColorAttr);
        SkASSERT(kVertexStride == gp->debugOnly_vertexStride());
||||||| merged common ancestors
        SkASSERT(gp->getVertexStride() == sizeof(GrDefaultGeoProcFactory::PositionColorAttr));
=======
>>>>>>> upstream-releases

        int numRegions = fRegions.count();
        int numRects = 0;
        for (int i = 0; i < numRegions; i++) {
            numRects += fRegions[i].fRegion.computeRegionComplexity();
        }

        if (!numRects) {
            return;
        }
<<<<<<< HEAD
        sk_sp<const GrBuffer> indexBuffer = target->resourceProvider()->refQuadIndexBuffer();
        PatternHelper helper(target, GrPrimitiveType::kTriangles, kVertexStride, indexBuffer.get(),
                             kVertsPerInstance, kIndicesPerInstance, numRects);
        void* vertices = helper.vertices();
        if (!vertices || !indexBuffer) {
||||||| merged common ancestors
        size_t vertexStride = gp->getVertexStride();
        sk_sp<const GrBuffer> indexBuffer = target->resourceProvider()->refQuadIndexBuffer();
        PatternHelper helper(GrPrimitiveType::kTriangles);
        void* vertices =
                helper.init(target, vertexStride, indexBuffer.get(), kVertsPerInstance,
                            kIndicesPerInstance, numRects);
        if (!vertices || !indexBuffer) {
=======
        sk_sp<const GrGpuBuffer> indexBuffer = target->resourceProvider()->refQuadIndexBuffer();
        if (!indexBuffer) {
            SkDebugf("Could not allocate indices\n");
            return;
        }
        PatternHelper helper(target, GrPrimitiveType::kTriangles, gp->vertexStride(),
                             std::move(indexBuffer), kVertsPerInstance, kIndicesPerInstance,
                             numRects);
        GrVertexWriter vertices{helper.vertices()};
        if (!vertices.fPtr) {
>>>>>>> upstream-releases
            SkDebugf("Could not allocate vertices\n");
            return;
        }

        for (int i = 0; i < numRegions; i++) {
<<<<<<< HEAD
            tesselate_region(verts, kVertexStride, fRegions[i].fColor, fRegions[i].fRegion);
            int numRectsInRegion = fRegions[i].fRegion.computeRegionComplexity();
            verts += numRectsInRegion * kVertsPerInstance * kVertexStride;
||||||| merged common ancestors
            tesselate_region(verts, vertexStride, fRegions[i].fColor, fRegions[i].fRegion);
            int numRectsInRegion = fRegions[i].fRegion.computeRegionComplexity();
            verts += numRectsInRegion * kVertsPerInstance * vertexStride;
=======
            GrVertexColor color(fRegions[i].fColor, fWideColor);
            SkRegion::Iterator iter(fRegions[i].fRegion);
            while (!iter.done()) {
                SkRect rect = SkRect::Make(iter.rect());
                vertices.writeQuad(GrVertexWriter::TriStripFromRect(rect), color);
                iter.next();
            }
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        auto pipe = fHelper.makePipeline(target);
        helper.recordDraw(target, std::move(gp), pipe.fPipeline, pipe.fFixedDynamicState);
||||||| merged common ancestors
        helper.recordDraw(target, gp.get(), fHelper.makePipeline(target));
=======
        helper.recordDraw(target, std::move(gp));
    }

    void onExecute(GrOpFlushState* flushState, const SkRect& chainBounds) override {
        fHelper.executeDrawsAndUploads(this, flushState, chainBounds);
>>>>>>> upstream-releases
    }

    CombineResult onCombineIfPossible(GrOp* t, const GrCaps& caps) override {
        RegionOp* that = t->cast<RegionOp>();
        if (!fHelper.isCompatible(that->fHelper, caps, this->bounds(), that->bounds())) {
            return CombineResult::kCannotCombine;
        }

        if (fViewMatrix != that->fViewMatrix) {
            return CombineResult::kCannotCombine;
        }

        fRegions.push_back_n(that->fRegions.count(), that->fRegions.begin());
<<<<<<< HEAD
        this->joinBounds(*that);
        return CombineResult::kMerged;
||||||| merged common ancestors
        this->joinBounds(*that);
        return true;
=======
        fWideColor |= that->fWideColor;
        return CombineResult::kMerged;
>>>>>>> upstream-releases
    }

    struct RegionInfo {
        SkPMColor4f fColor;
        SkRegion fRegion;
    };

    Helper fHelper;
    SkMatrix fViewMatrix;
    SkSTArray<1, RegionInfo, true> fRegions;
    bool fWideColor;

    typedef GrMeshDrawOp INHERITED;
};

}  // anonymous namespace

namespace GrRegionOp {

<<<<<<< HEAD
std::unique_ptr<GrDrawOp> Make(GrContext* context,
                               GrPaint&& paint,
                               const SkMatrix& viewMatrix,
                               const SkRegion& region,
                               GrAAType aaType,
                               const GrUserStencilSettings* stencilSettings) {
||||||| merged common ancestors
std::unique_ptr<GrDrawOp> Make(GrPaint&& paint, const SkMatrix& viewMatrix, const SkRegion& region,
                               GrAAType aaType, const GrUserStencilSettings* stencilSettings) {
=======
std::unique_ptr<GrDrawOp> Make(GrRecordingContext* context,
                               GrPaint&& paint,
                               const SkMatrix& viewMatrix,
                               const SkRegion& region,
                               GrAAType aaType,
                               const GrUserStencilSettings* stencilSettings) {
>>>>>>> upstream-releases
    if (aaType != GrAAType::kNone && aaType != GrAAType::kMSAA) {
        return nullptr;
    }
    return RegionOp::Make(context, std::move(paint), viewMatrix, region, aaType, stencilSettings);
}
}

#if GR_TEST_UTILS

GR_DRAW_OP_TEST_DEFINE(RegionOp) {
    SkRegion region;
    int n = random->nextULessThan(200);
    for (int i = 0; i < n; ++i) {
        SkIPoint center;
        center.fX = random->nextULessThan(1000);
        center.fY = random->nextULessThan(1000);
        int w = random->nextRangeU(10, 1000);
        int h = random->nextRangeU(10, 1000);
        SkIRect rect = {center.fX - w / 2, center.fY - h / 2, center.fX + w / 2, center.fY + h / 2};
        SkRegion::Op op;
        if (i == 0) {
            op = SkRegion::kReplace_Op;
        } else {
            // Pick an other than replace.
            GR_STATIC_ASSERT(SkRegion::kLastOp == SkRegion::kReplace_Op);
            op = (SkRegion::Op)random->nextULessThan(SkRegion::kLastOp);
        }
        region.op(rect, op);
    }
    SkMatrix viewMatrix = GrTest::TestMatrix(random);
    GrAAType aaType = GrAAType::kNone;
    if (GrFSAAType::kUnifiedMSAA == fsaaType && random->nextBool()) {
        aaType = GrAAType::kMSAA;
    }
    return RegionOp::Make(context, std::move(paint), viewMatrix, region, aaType,
                          GrGetRandomStencil(random, context));
}

#endif
