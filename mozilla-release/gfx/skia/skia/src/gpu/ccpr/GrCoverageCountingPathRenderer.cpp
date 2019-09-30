/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrCoverageCountingPathRenderer.h"

#include "GrCaps.h"
#include "GrClip.h"
#include "GrProxyProvider.h"
#include "SkMakeUnique.h"
#include "SkPathOps.h"
#include "ccpr/GrCCClipProcessor.h"
#include "ccpr/GrCCDrawPathsOp.h"
#include "ccpr/GrCCPathCache.h"

using PathInstance = GrCCPathProcessor::Instance;

bool GrCoverageCountingPathRenderer::IsSupported(const GrCaps& caps) {
    const GrShaderCaps& shaderCaps = *caps.shaderCaps();
    return caps.instanceAttribSupport() && shaderCaps.integerSupport() &&
           shaderCaps.floatIs32Bits() && GrCaps::kNone_MapFlags != caps.mapBufferFlags() &&
           caps.isConfigTexturable(kAlpha_half_GrPixelConfig) &&
           caps.isConfigRenderable(kAlpha_half_GrPixelConfig) &&
<<<<<<< HEAD
           caps.isConfigTexturable(kAlpha_8_GrPixelConfig) &&
           caps.isConfigRenderable(kAlpha_8_GrPixelConfig) &&
||||||| merged common ancestors
=======
           caps.isConfigTexturable(kAlpha_8_GrPixelConfig) &&
           caps.isConfigRenderable(kAlpha_8_GrPixelConfig) &&
           caps.halfFloatVertexAttributeSupport() &&
>>>>>>> upstream-releases
           !caps.blacklistCoverageCounting();
}

sk_sp<GrCoverageCountingPathRenderer> GrCoverageCountingPathRenderer::CreateIfSupported(
<<<<<<< HEAD
        const GrCaps& caps, AllowCaching allowCaching) {
    return sk_sp<GrCoverageCountingPathRenderer>(
            IsSupported(caps) ? new GrCoverageCountingPathRenderer(allowCaching) : nullptr);
||||||| merged common ancestors
        const GrCaps& caps, bool drawCachablePaths) {
    auto ccpr = IsSupported(caps) ? new GrCoverageCountingPathRenderer(drawCachablePaths) : nullptr;
    return sk_sp<GrCoverageCountingPathRenderer>(ccpr);
=======
        const GrCaps& caps, AllowCaching allowCaching, uint32_t contextUniqueID) {
    return sk_sp<GrCoverageCountingPathRenderer>((IsSupported(caps))
            ? new GrCoverageCountingPathRenderer(allowCaching, contextUniqueID)
            : nullptr);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
GrCoverageCountingPathRenderer::GrCoverageCountingPathRenderer(AllowCaching allowCaching) {
    if (AllowCaching::kYes == allowCaching) {
        fPathCache = skstd::make_unique<GrCCPathCache>();
||||||| merged common ancestors
GrPathRenderer::CanDrawPath GrCoverageCountingPathRenderer::onCanDrawPath(
        const CanDrawPathArgs& args) const {
    if (args.fShape->hasUnstyledKey() && !fDrawCachablePaths) {
        return CanDrawPath::kNo;
=======
GrCoverageCountingPathRenderer::GrCoverageCountingPathRenderer(AllowCaching allowCaching,
                                                               uint32_t contextUniqueID) {
    if (AllowCaching::kYes == allowCaching) {
        fPathCache = skstd::make_unique<GrCCPathCache>(contextUniqueID);
>>>>>>> upstream-releases
    }
}

GrCCPerOpListPaths* GrCoverageCountingPathRenderer::lookupPendingPaths(uint32_t opListID) {
    auto it = fPendingPaths.find(opListID);
    if (fPendingPaths.end() == it) {
        sk_sp<GrCCPerOpListPaths> paths = sk_make_sp<GrCCPerOpListPaths>();
        it = fPendingPaths.insert(std::make_pair(opListID, std::move(paths))).first;
    }
    return it->second.get();
}

GrPathRenderer::CanDrawPath GrCoverageCountingPathRenderer::onCanDrawPath(
        const CanDrawPathArgs& args) const {
    const GrShape& shape = *args.fShape;
    if (GrAAType::kCoverage != args.fAAType || shape.style().hasPathEffect() ||
        args.fViewMatrix->hasPerspective() || shape.inverseFilled()) {
        return CanDrawPath::kNo;
    }

    SkPath path;
    shape.asPath(&path);

    const SkStrokeRec& stroke = shape.style().strokeRec();
    switch (stroke.getStyle()) {
        case SkStrokeRec::kFill_Style: {
            SkRect devBounds;
            args.fViewMatrix->mapRect(&devBounds, path.getBounds());

            SkIRect clippedIBounds;
            devBounds.roundOut(&clippedIBounds);
            if (!clippedIBounds.intersect(*args.fClipConservativeBounds)) {
                // The path is completely clipped away. Our code will eventually notice this before
                // doing any real work.
                return CanDrawPath::kYes;
            }

            int64_t numPixels = sk_64_mul(clippedIBounds.height(), clippedIBounds.width());
            if (path.countVerbs() > 1000 && path.countPoints() > numPixels) {
                // This is a complicated path that has more vertices than pixels! Let's let the SW
                // renderer have this one: It will probably be faster and a bitmap will require less
                // total memory on the GPU than CCPR instance buffers would for the raw path data.
                return CanDrawPath::kNo;
            }

            if (numPixels > 256 * 256) {
                // Large paths can blow up the atlas fast. And they are not ideal for a two-pass
                // rendering algorithm. Give the simpler direct renderers a chance before we commit
                // to drawing it.
                return CanDrawPath::kAsBackup;
            }

            if (args.fShape->hasUnstyledKey() && path.countVerbs() > 50) {
                // Complex paths do better cached in an SDF, if the renderer will accept them.
                return CanDrawPath::kAsBackup;
            }

            return CanDrawPath::kYes;
        }

        case SkStrokeRec::kStroke_Style:
            if (!args.fViewMatrix->isSimilarity()) {
                // The stroker currently only supports rigid-body transfoms for the stroke lines
                // themselves. This limitation doesn't affect hairlines since their stroke lines are
                // defined relative to device space.
                return CanDrawPath::kNo;
            }
            // fallthru
        case SkStrokeRec::kHairline_Style: {
            float inflationRadius;
            GetStrokeDevWidth(*args.fViewMatrix, stroke, &inflationRadius);
            if (!(inflationRadius <= kMaxBoundsInflationFromStroke)) {
                // Let extremely wide strokes be converted to fill paths and drawn by the CCPR
                // filler instead. (Cast the logic negatively in order to also catch r=NaN.)
                return CanDrawPath::kNo;
            }
            SkASSERT(!SkScalarIsNaN(inflationRadius));
            if (SkPathPriv::ConicWeightCnt(path)) {
                // The stroker does not support conics yet.
                return CanDrawPath::kNo;
            }
            return CanDrawPath::kYes;
        }

        case SkStrokeRec::kStrokeAndFill_Style:
            return CanDrawPath::kNo;
    }

    SK_ABORT("Invalid stroke style.");
    return CanDrawPath::kNo;
}

bool GrCoverageCountingPathRenderer::onDrawPath(const DrawPathArgs& args) {
    SkASSERT(!fFlushing);

    SkIRect clipIBounds;
    GrRenderTargetContext* rtc = args.fRenderTargetContext;
    args.fClip->getConservativeBounds(rtc->width(), rtc->height(), &clipIBounds, nullptr);

    auto op = GrCCDrawPathsOp::Make(args.fContext, clipIBounds, *args.fViewMatrix, *args.fShape,
                                    std::move(args.fPaint));
    this->recordOp(std::move(op), args);
    return true;
}

<<<<<<< HEAD
void GrCoverageCountingPathRenderer::recordOp(std::unique_ptr<GrCCDrawPathsOp> opHolder,
                                              const DrawPathArgs& args) {
    if (GrCCDrawPathsOp* op = opHolder.get()) {
        GrRenderTargetContext* rtc = args.fRenderTargetContext;
        if (uint32_t opListID = rtc->addDrawOp(*args.fClip, std::move(opHolder))) {
            op->wasRecorded(sk_ref_sp(this->lookupPendingPaths(opListID)));
        }
||||||| merged common ancestors
void CCPR::DrawPathsOp::wasRecorded(GrRenderTargetOpList* opList) {
    SkASSERT(!fCCPR->fFlushing);
    SkASSERT(!fOwningRTPendingPaths);
    fOwningRTPendingPaths = &fCCPR->fRTPendingPathsMap[opList->uniqueID()];
    fOwningRTPendingPaths->fDrawOps.addToTail(this);
}

bool GrCoverageCountingPathRenderer::canMakeClipProcessor(const SkPath& deviceSpacePath) const {
    if (!fDrawCachablePaths && !deviceSpacePath.isVolatile()) {
        return false;
    }

    if (SkPathPriv::ConicWeightCnt(deviceSpacePath)) {
        return false;
=======
void GrCoverageCountingPathRenderer::recordOp(std::unique_ptr<GrCCDrawPathsOp> op,
                                              const DrawPathArgs& args) {
    if (op) {
        auto addToOwningPerOpListPaths = [this](GrOp* op, uint32_t opListID) {
            op->cast<GrCCDrawPathsOp>()->addToOwningPerOpListPaths(
                    sk_ref_sp(this->lookupPendingPaths(opListID)));
        };
        args.fRenderTargetContext->addDrawOp(*args.fClip, std::move(op), addToOwningPerOpListPaths);
>>>>>>> upstream-releases
    }
}

std::unique_ptr<GrFragmentProcessor> GrCoverageCountingPathRenderer::makeClipProcessor(
        uint32_t opListID, const SkPath& deviceSpacePath, const SkIRect& accessRect, int rtWidth,
        int rtHeight, const GrCaps& caps) {
    using MustCheckBounds = GrCCClipProcessor::MustCheckBounds;

    SkASSERT(!fFlushing);

    GrCCClipPath& clipPath =
            this->lookupPendingPaths(opListID)->fClipPaths[deviceSpacePath.getGenerationID()];
    if (!clipPath.isInitialized()) {
        // This ClipPath was just created during lookup. Initialize it.
        const SkRect& pathDevBounds = deviceSpacePath.getBounds();
        if (SkTMax(pathDevBounds.height(), pathDevBounds.width()) > kPathCropThreshold) {
            // The path is too large. Crop it or analytic AA can run out of fp32 precision.
            SkPath croppedPath;
            int maxRTSize = caps.maxRenderTargetSize();
            CropPath(deviceSpacePath, SkIRect::MakeWH(maxRTSize, maxRTSize), &croppedPath);
            clipPath.init(croppedPath, accessRect, rtWidth, rtHeight, caps);
        } else {
            clipPath.init(deviceSpacePath, accessRect, rtWidth, rtHeight, caps);
        }
    } else {
        clipPath.addAccess(accessRect);
    }

    bool mustCheckBounds = !clipPath.pathDevIBounds().contains(accessRect);
    return skstd::make_unique<GrCCClipProcessor>(&clipPath, MustCheckBounds(mustCheckBounds),
                                                 deviceSpacePath.getFillType());
}

void GrCoverageCountingPathRenderer::preFlush(GrOnFlushResourceProvider* onFlushRP,
                                              const uint32_t* opListIDs, int numOpListIDs,
<<<<<<< HEAD
                                              SkTArray<sk_sp<GrRenderTargetContext>>* out) {
    using DoCopiesToCache = GrCCDrawPathsOp::DoCopiesToCache;
||||||| merged common ancestors
                                              SkTArray<sk_sp<GrRenderTargetContext>>* results) {
    using PathInstance = GrCCPathProcessor::Instance;

=======
                                              SkTArray<sk_sp<GrRenderTargetContext>>* out) {
    using DoCopiesToA8Coverage = GrCCDrawPathsOp::DoCopiesToA8Coverage;
>>>>>>> upstream-releases
    SkASSERT(!fFlushing);
    SkASSERT(fFlushingPaths.empty());
    SkDEBUGCODE(fFlushing = true);

<<<<<<< HEAD
    // Dig up the stashed atlas from the previous flush (if any) so we can attempt to copy any
    // reusable paths out of it and into the resource cache. We also need to clear its unique key.
    sk_sp<GrTextureProxy> stashedAtlasProxy;
    if (fStashedAtlasKey.isValid()) {
        stashedAtlasProxy = onFlushRP->findOrCreateProxyByUniqueKey(fStashedAtlasKey,
                                                                    GrCCAtlas::kTextureOrigin);
        if (stashedAtlasProxy) {
            // Instantiate the proxy so we can clear the underlying texture's unique key.
            onFlushRP->instatiateProxy(stashedAtlasProxy.get());
            onFlushRP->removeUniqueKeyFromProxy(fStashedAtlasKey, stashedAtlasProxy.get());
        } else {
            fStashedAtlasKey.reset();  // Indicate there is no stashed atlas to copy from.
        }
    }

    if (fPendingPaths.empty()) {
        fStashedAtlasKey.reset();
||||||| merged common ancestors
    if (fRTPendingPathsMap.empty()) {
=======
    if (fPathCache) {
        fPathCache->doPreFlushProcessing();
    }

    if (fPendingPaths.empty()) {
>>>>>>> upstream-releases
        return;  // Nothing to draw.
    }

    GrCCPerFlushResourceSpecs specs;
    int maxPreferredRTSize = onFlushRP->caps()->maxPreferredRenderTargetSize();
    specs.fCopyAtlasSpecs.fMaxPreferredTextureSize = SkTMin(2048, maxPreferredRTSize);
    SkASSERT(0 == specs.fCopyAtlasSpecs.fMinTextureSize);
    specs.fRenderedAtlasSpecs.fMaxPreferredTextureSize = maxPreferredRTSize;
    specs.fRenderedAtlasSpecs.fMinTextureSize = SkTMin(512, maxPreferredRTSize);

    // Move the per-opList paths that are about to be flushed from fPendingPaths to fFlushingPaths,
    // and count them up so we can preallocate buffers.
    fFlushingPaths.reserve(numOpListIDs);
    for (int i = 0; i < numOpListIDs; ++i) {
        auto iter = fPendingPaths.find(opListIDs[i]);
        if (fPendingPaths.end() == iter) {
            continue;  // No paths on this opList.
        }

<<<<<<< HEAD
        fFlushingPaths.push_back(std::move(iter->second));
        fPendingPaths.erase(iter);

        for (GrCCDrawPathsOp* op : fFlushingPaths.back()->fDrawOps) {
            op->accountForOwnPaths(fPathCache.get(), onFlushRP, fStashedAtlasKey, &specs);
        }
        for (const auto& clipsIter : fFlushingPaths.back()->fClipPaths) {
            clipsIter.second.accountForOwnPath(&specs);
||||||| merged common ancestors
        maxTotalPaths += rtPendingPaths.fClipPaths.size();
        SkDEBUGCODE(numClipPaths += rtPendingPaths.fClipPaths.size());
        for (const auto& clipsIter : rtPendingPaths.fClipPaths) {
            const SkPath& path = clipsIter.second.deviceSpacePath();
            maxPathPoints = SkTMax(path.countPoints(), maxPathPoints);
            numSkPoints += path.countPoints();
            numSkVerbs += path.countVerbs();
=======
        fFlushingPaths.push_back(std::move(iter->second));
        fPendingPaths.erase(iter);

        for (GrCCDrawPathsOp* op : fFlushingPaths.back()->fDrawOps) {
            op->accountForOwnPaths(fPathCache.get(), onFlushRP, &specs);
        }
        for (const auto& clipsIter : fFlushingPaths.back()->fClipPaths) {
            clipsIter.second.accountForOwnPath(&specs);
>>>>>>> upstream-releases
        }
    }
    fStashedAtlasKey.reset();

    if (specs.isEmpty()) {
        return;  // Nothing to draw.
    }

<<<<<<< HEAD
    // Determine if there are enough reusable paths from last flush for it to be worth our time to
    // copy them to cached atlas(es).
    int numCopies = specs.fNumCopiedPaths[GrCCPerFlushResourceSpecs::kFillIdx] +
                    specs.fNumCopiedPaths[GrCCPerFlushResourceSpecs::kStrokeIdx];
    DoCopiesToCache doCopies = DoCopiesToCache(numCopies > 100 ||
                                               specs.fCopyAtlasSpecs.fApproxNumPixels > 256 * 256);
    if (numCopies && DoCopiesToCache::kNo == doCopies) {
        specs.convertCopiesToRenders();
        SkASSERT(!specs.fNumCopiedPaths[GrCCPerFlushResourceSpecs::kFillIdx]);
        SkASSERT(!specs.fNumCopiedPaths[GrCCPerFlushResourceSpecs::kStrokeIdx]);
||||||| merged common ancestors
    // Allocate GPU buffers.
    fPerFlushIndexBuffer = GrCCPathProcessor::FindIndexBuffer(onFlushRP);
    if (!fPerFlushIndexBuffer) {
        SkDebugf("WARNING: failed to allocate ccpr path index buffer.\n");
        return;
    }

    fPerFlushVertexBuffer = GrCCPathProcessor::FindVertexBuffer(onFlushRP);
    if (!fPerFlushVertexBuffer) {
        SkDebugf("WARNING: failed to allocate ccpr path vertex buffer.\n");
        return;
=======
    // Determine if there are enough reusable paths from last flush for it to be worth our time to
    // copy them to cached atlas(es).
    int numCopies = specs.fNumCopiedPaths[GrCCPerFlushResourceSpecs::kFillIdx] +
                    specs.fNumCopiedPaths[GrCCPerFlushResourceSpecs::kStrokeIdx];
    auto doCopies = DoCopiesToA8Coverage(numCopies > 100 ||
                                         specs.fCopyAtlasSpecs.fApproxNumPixels > 256 * 256);
    if (numCopies && DoCopiesToA8Coverage::kNo == doCopies) {
        specs.cancelCopies();
>>>>>>> upstream-releases
    }

    auto resources = sk_make_sp<GrCCPerFlushResources>(onFlushRP, specs);
    if (!resources->isMapped()) {
        return;  // Some allocation failed.
    }

<<<<<<< HEAD
    // Layout the atlas(es) and parse paths.
    for (const auto& flushingPaths : fFlushingPaths) {
        for (GrCCDrawPathsOp* op : flushingPaths->fDrawOps) {
            op->setupResources(onFlushRP, resources.get(), doCopies);
        }
        for (auto& clipsIter : flushingPaths->fClipPaths) {
            clipsIter.second.renderPathInAtlas(resources.get(), onFlushRP);
||||||| merged common ancestors
    PathInstance* pathInstanceData = static_cast<PathInstance*>(fPerFlushInstanceBuffer->map());
    SkASSERT(pathInstanceData);
    int pathInstanceIdx = 0;

    fPerFlushPathParser = sk_make_sp<GrCCPathParser>(maxTotalPaths, maxPathPoints, numSkPoints,
                                                     numSkVerbs);
    SkDEBUGCODE(int skippedTotalPaths = 0);

    // Allocate atlas(es) and fill out GPU instance buffers.
    for (int i = 0; i < numOpListIDs; ++i) {
        auto it = fRTPendingPathsMap.find(opListIDs[i]);
        if (fRTPendingPathsMap.end() == it) {
            continue;
        }
        RTPendingPaths& rtPendingPaths = it->second;

        SkTInternalLList<DrawPathsOp>::Iter drawOpsIter;
        drawOpsIter.init(rtPendingPaths.fDrawOps,
                         SkTInternalLList<DrawPathsOp>::Iter::kHead_IterStart);
        while (DrawPathsOp* op = drawOpsIter.get()) {
            pathInstanceIdx = op->setupResources(onFlushRP, pathInstanceData, pathInstanceIdx);
            drawOpsIter.next();
            SkDEBUGCODE(skippedTotalPaths += op->numSkippedInstances_debugOnly());
        }

        for (auto& clipsIter : rtPendingPaths.fClipPaths) {
            clipsIter.second.placePathInAtlas(this, onFlushRP, fPerFlushPathParser.get());
=======
    // Layout the atlas(es) and parse paths.
    for (const auto& flushingPaths : fFlushingPaths) {
        for (GrCCDrawPathsOp* op : flushingPaths->fDrawOps) {
            op->setupResources(fPathCache.get(), onFlushRP, resources.get(), doCopies);
        }
        for (auto& clipsIter : flushingPaths->fClipPaths) {
            clipsIter.second.renderPathInAtlas(resources.get(), onFlushRP);
>>>>>>> upstream-releases
        }
    }

<<<<<<< HEAD
    // Allocate resources and then render the atlas(es).
    if (!resources->finalize(onFlushRP, std::move(stashedAtlasProxy), out)) {
||||||| merged common ancestors
    fPerFlushInstanceBuffer->unmap();

    SkASSERT(pathInstanceIdx == maxTotalPaths - skippedTotalPaths - numClipPaths);

    if (!fPerFlushAtlases.empty()) {
        auto coverageCountBatchID = fPerFlushPathParser->closeCurrentBatch();
        fPerFlushAtlases.back().setCoverageCountBatchID(coverageCountBatchID);
    }

    if (!fPerFlushPathParser->finalize(onFlushRP)) {
        SkDebugf("WARNING: failed to allocate GPU buffers for CCPR. No paths will be drawn.\n");
=======
    if (fPathCache) {
        // Purge invalidated textures from previous atlases *before* calling finalize(). That way,
        // the underlying textures objects can be freed up and reused for the next atlases.
        fPathCache->purgeInvalidatedAtlasTextures(onFlushRP);
    }

    // Allocate resources and then render the atlas(es).
    if (!resources->finalize(onFlushRP, out)) {
>>>>>>> upstream-releases
        return;
    }
    // Verify the stashed atlas got released so its texture could be recycled.
    SkASSERT(!stashedAtlasProxy);

    // Commit flushing paths to the resources once they are successfully completed.
    for (auto& flushingPaths : fFlushingPaths) {
        SkASSERT(!flushingPaths->fFlushResources);
        flushingPaths->fFlushResources = resources;
    }
}

<<<<<<< HEAD
void GrCoverageCountingPathRenderer::postFlush(GrDeferredUploadToken, const uint32_t* opListIDs,
                                               int numOpListIDs) {
    SkASSERT(fFlushing);
    SkASSERT(!fStashedAtlasKey.isValid());  // Should have been cleared in preFlush().

    if (!fFlushingPaths.empty()) {
        // Note the stashed atlas's key for next flush, if any.
        auto resources = fFlushingPaths.front()->fFlushResources.get();
        if (resources && resources->hasStashedAtlas()) {
            fStashedAtlasKey = resources->stashedAtlasKey();
        }
||||||| merged common ancestors
int CCPR::DrawPathsOp::setupResources(GrOnFlushResourceProvider* onFlushRP,
                                      GrCCPathProcessor::Instance* pathInstanceData,
                                      int pathInstanceIdx) {
    GrCCPathParser* parser = fCCPR->fPerFlushPathParser.get();
    const GrCCAtlas* currentAtlas = nullptr;
    SkASSERT(fInstanceCount > 0);
    SkASSERT(-1 == fBaseInstance);
    fBaseInstance = pathInstanceIdx;

    for (const SingleDraw* draw = this->head(); draw; draw = draw->fNext) {
        // parsePath gives us two tight bounding boxes: one in device space, as well as a second
        // one rotated an additional 45 degrees. The path vertex shader uses these two bounding
        // boxes to generate an octagon that circumscribes the path.
        SkRect devBounds, devBounds45;
        parser->parsePath(draw->fMatrix, draw->fPath, &devBounds, &devBounds45);

        SkIRect devIBounds;
        devBounds.roundOut(&devIBounds);

        int16_t offsetX, offsetY;
        GrCCAtlas* atlas = fCCPR->placeParsedPathInAtlas(onFlushRP, draw->fClipIBounds, devIBounds,
                                                         &offsetX, &offsetY);
        if (!atlas) {
            SkDEBUGCODE(++fNumSkippedInstances);
            continue;
        }
        if (currentAtlas != atlas) {
            if (currentAtlas) {
                this->addAtlasBatch(currentAtlas, pathInstanceIdx);
            }
            currentAtlas = atlas;
        }
=======
void GrCoverageCountingPathRenderer::postFlush(GrDeferredUploadToken, const uint32_t* opListIDs,
                                               int numOpListIDs) {
    SkASSERT(fFlushing);
>>>>>>> upstream-releases

<<<<<<< HEAD
        // In DDL mode these aren't guaranteed to be deleted so we must clear out the perFlush
        // resources manually.
        for (auto& flushingPaths : fFlushingPaths) {
            flushingPaths->fFlushResources = nullptr;
        }
||||||| merged common ancestors
        const SkMatrix& m = draw->fMatrix;
        pathInstanceData[pathInstanceIdx++] = {
                devBounds,
                devBounds45,
                {{m.getScaleX(), m.getSkewY(), m.getSkewX(), m.getScaleY()}},
                {{m.getTranslateX(), m.getTranslateY()}},
                {{offsetX, offsetY}},
                draw->fColor};
    }
=======
    if (!fFlushingPaths.empty()) {
        // In DDL mode these aren't guaranteed to be deleted so we must clear out the perFlush
        // resources manually.
        for (auto& flushingPaths : fFlushingPaths) {
            flushingPaths->fFlushResources = nullptr;
        }
>>>>>>> upstream-releases

        // We wait to erase these until after flush, once Ops and FPs are done accessing their data.
        fFlushingPaths.reset();
    }

    SkDEBUGCODE(fFlushing = false);
}

<<<<<<< HEAD
void GrCoverageCountingPathRenderer::CropPath(const SkPath& path, const SkIRect& cropbox,
                                              SkPath* out) {
    SkPath cropboxPath;
    cropboxPath.addRect(SkRect::Make(cropbox));
    if (!Op(cropboxPath, path, kIntersect_SkPathOp, out)) {
        // This can fail if the PathOps encounter NaN or infinities.
        out->reset();
    }
    out->setIsVolatile(true);
||||||| merged common ancestors
GrCCAtlas* GrCoverageCountingPathRenderer::placeParsedPathInAtlas(
        GrOnFlushResourceProvider* onFlushRP,
        const SkIRect& clipIBounds,
        const SkIRect& pathIBounds,
        int16_t* atlasOffsetX,
        int16_t* atlasOffsetY) {
    using ScissorMode = GrCCPathParser::ScissorMode;

    ScissorMode scissorMode;
    SkIRect clippedPathIBounds;
    if (clipIBounds.contains(pathIBounds)) {
        clippedPathIBounds = pathIBounds;
        scissorMode = ScissorMode::kNonScissored;
    } else if (clippedPathIBounds.intersect(clipIBounds, pathIBounds)) {
        scissorMode = ScissorMode::kScissored;
    } else {
        fPerFlushPathParser->discardParsedPath();
        return nullptr;
    }

    SkIPoint16 atlasLocation;
    int h = clippedPathIBounds.height(), w = clippedPathIBounds.width();
    if (fPerFlushAtlases.empty() || !fPerFlushAtlases.back().addRect(w, h, &atlasLocation)) {
        if (!fPerFlushAtlases.empty()) {
            // The atlas is out of room and can't grow any bigger.
            auto coverageCountBatchID = fPerFlushPathParser->closeCurrentBatch();
            fPerFlushAtlases.back().setCoverageCountBatchID(coverageCountBatchID);
        }
        fPerFlushAtlases.emplace_back(*onFlushRP->caps(), SkTMax(w, h));
        SkAssertResult(fPerFlushAtlases.back().addRect(w, h, &atlasLocation));
    }

    *atlasOffsetX = atlasLocation.x() - static_cast<int16_t>(clippedPathIBounds.left());
    *atlasOffsetY = atlasLocation.y() - static_cast<int16_t>(clippedPathIBounds.top());
    fPerFlushPathParser->saveParsedPath(scissorMode, clippedPathIBounds, *atlasOffsetX,
                                        *atlasOffsetY);

    return &fPerFlushAtlases.back();
=======
void GrCoverageCountingPathRenderer::purgeCacheEntriesOlderThan(
        GrProxyProvider* proxyProvider, const GrStdSteadyClock::time_point& purgeTime) {
    if (fPathCache) {
        fPathCache->purgeEntriesOlderThan(proxyProvider, purgeTime);
    }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
float GrCoverageCountingPathRenderer::GetStrokeDevWidth(const SkMatrix& m,
                                                        const SkStrokeRec& stroke,
                                                        float* inflationRadius) {
    float strokeDevWidth;
    if (stroke.isHairlineStyle()) {
        strokeDevWidth = 1;
    } else {
        SkASSERT(SkStrokeRec::kStroke_Style == stroke.getStyle());
        SkASSERT(m.isSimilarity());  // Otherwise matrixScaleFactor = m.getMaxScale().
        float matrixScaleFactor = SkVector::Length(m.getScaleX(), m.getSkewY());
        strokeDevWidth = stroke.getWidth() * matrixScaleFactor;
||||||| merged common ancestors
void CCPR::DrawPathsOp::onExecute(GrOpFlushState* flushState) {
    SkASSERT(fCCPR->fFlushing);
    SkASSERT(flushState->rtCommandBuffer());

    if (!fCCPR->fPerFlushResourcesAreValid) {
        return;  // Setup failed.
    }

    SkASSERT(fBaseInstance >= 0);  // Make sure setupResources has been called.

    GrPipeline::InitArgs initArgs;
    initArgs.fFlags = fSRGBFlags;
    initArgs.fProxy = flushState->drawOpArgs().fProxy;
    initArgs.fCaps = &flushState->caps();
    initArgs.fResourceProvider = flushState->resourceProvider();
    initArgs.fDstProxy = flushState->drawOpArgs().fDstProxy;
    GrPipeline pipeline(initArgs, std::move(fProcessors), flushState->detachAppliedClip());

    int baseInstance = fBaseInstance;

    for (int i = 0; i < fAtlasBatches.count(); baseInstance = fAtlasBatches[i++].fEndInstanceIdx) {
        const AtlasBatch& batch = fAtlasBatches[i];
        SkASSERT(batch.fEndInstanceIdx > baseInstance);

        if (!batch.fAtlas->textureProxy()) {
            continue;  // Atlas failed to allocate.
        }

        GrCCPathProcessor pathProc(flushState->resourceProvider(),
                                   sk_ref_sp(batch.fAtlas->textureProxy()), this->getFillType());

        GrMesh mesh(GrCCPathProcessor::MeshPrimitiveType(flushState->caps()));
        mesh.setIndexedInstanced(fCCPR->fPerFlushIndexBuffer.get(),
                                 GrCCPathProcessor::NumIndicesPerInstance(flushState->caps()),
                                 fCCPR->fPerFlushInstanceBuffer.get(),
                                 batch.fEndInstanceIdx - baseInstance, baseInstance);
        mesh.setVertexData(fCCPR->fPerFlushVertexBuffer.get());

        flushState->rtCommandBuffer()->draw(pipeline, pathProc, &mesh, nullptr, 1, this->bounds());
=======
void GrCoverageCountingPathRenderer::CropPath(const SkPath& path, const SkIRect& cropbox,
                                              SkPath* out) {
    SkPath cropboxPath;
    cropboxPath.addRect(SkRect::Make(cropbox));
    if (!Op(cropboxPath, path, kIntersect_SkPathOp, out)) {
        // This can fail if the PathOps encounter NaN or infinities.
        out->reset();
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (inflationRadius) {
        // Inflate for a minimum stroke width of 1. In some cases when the stroke is less than 1px
        // wide, we may inflate it to 1px and instead reduce the opacity.
        *inflationRadius = SkStrokeRec::GetInflationRadius(
                stroke.getJoin(), stroke.getMiter(), stroke.getCap(), SkTMax(strokeDevWidth, 1.f));
||||||| merged common ancestors

    SkASSERT(baseInstance == fBaseInstance + fInstanceCount - fNumSkippedInstances);
}

void GrCoverageCountingPathRenderer::postFlush(GrDeferredUploadToken, const uint32_t* opListIDs,
                                               int numOpListIDs) {
    SkASSERT(fFlushing);
    fPerFlushAtlases.reset();
    fPerFlushPathParser.reset();
    fPerFlushInstanceBuffer.reset();
    fPerFlushVertexBuffer.reset();
    fPerFlushIndexBuffer.reset();
    // We wait to erase these until after flush, once Ops and FPs are done accessing their data.
    for (int i = 0; i < numOpListIDs; ++i) {
        fRTPendingPathsMap.erase(opListIDs[i]);
=======
    out->setIsVolatile(true);
}

float GrCoverageCountingPathRenderer::GetStrokeDevWidth(const SkMatrix& m,
                                                        const SkStrokeRec& stroke,
                                                        float* inflationRadius) {
    float strokeDevWidth;
    if (stroke.isHairlineStyle()) {
        strokeDevWidth = 1;
    } else {
        SkASSERT(SkStrokeRec::kStroke_Style == stroke.getStyle());
        SkASSERT(m.isSimilarity());  // Otherwise matrixScaleFactor = m.getMaxScale().
        float matrixScaleFactor = SkVector::Length(m.getScaleX(), m.getSkewY());
        strokeDevWidth = stroke.getWidth() * matrixScaleFactor;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    return strokeDevWidth;
||||||| merged common ancestors
    SkDEBUGCODE(fFlushing = false);
=======
    if (inflationRadius) {
        // Inflate for a minimum stroke width of 1. In some cases when the stroke is less than 1px
        // wide, we may inflate it to 1px and instead reduce the opacity.
        *inflationRadius = SkStrokeRec::GetInflationRadius(
                stroke.getJoin(), stroke.getMiter(), stroke.getCap(), SkTMax(strokeDevWidth, 1.f));
    }
    return strokeDevWidth;
>>>>>>> upstream-releases
}
