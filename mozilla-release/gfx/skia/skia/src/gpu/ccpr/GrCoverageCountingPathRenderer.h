/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrCoverageCountingPathRenderer_DEFINED
#define GrCoverageCountingPathRenderer_DEFINED

#include <map>
#include "GrCCPerOpListPaths.h"
#include "GrOnFlushResourceProvider.h"
#include "GrPathRenderer.h"
#include "GrRenderTargetOpList.h"
#include "ccpr/GrCCPerFlushResources.h"

class GrCCDrawPathsOp;
class GrCCPathCache;

/**
 * This is a path renderer that draws antialiased paths by counting coverage in an offscreen
 * buffer. (See GrCCCoverageProcessor, GrCCPathProcessor.)
 *
 * It also serves as the per-render-target tracker for pending path draws, and at the start of
 * flush, it compiles GPU buffers and renders a "coverage count atlas" for the upcoming paths.
 */
class GrCoverageCountingPathRenderer : public GrPathRenderer, public GrOnFlushCallbackObject {
public:
    static bool IsSupported(const GrCaps&);

    enum class AllowCaching : bool {
        kNo = false,
        kYes = true
    };

<<<<<<< HEAD
    static sk_sp<GrCoverageCountingPathRenderer> CreateIfSupported(const GrCaps&, AllowCaching);
||||||| merged common ancestors
        int setupResources(GrOnFlushResourceProvider*,
                           GrCCPathProcessor::Instance* pathInstanceData, int pathInstanceIdx);
=======
    static sk_sp<GrCoverageCountingPathRenderer> CreateIfSupported(const GrCaps&, AllowCaching,
                                                                   uint32_t contextUniqueID);
>>>>>>> upstream-releases

    using PendingPathsMap = std::map<uint32_t, sk_sp<GrCCPerOpListPaths>>;

    // In DDL mode, Ganesh needs to be able to move the pending GrCCPerOpListPaths to the DDL object
    // (detachPendingPaths) and then return them upon replay (mergePendingPaths).
    PendingPathsMap detachPendingPaths() { return std::move(fPendingPaths); }

    void mergePendingPaths(const PendingPathsMap& paths) {
#ifdef SK_DEBUG
        // Ensure there are no duplicate opList IDs between the incoming path map and ours.
        // This should always be true since opList IDs are globally unique and these are coming
        // from different DDL recordings.
        for (const auto& it : paths) {
            SkASSERT(!fPendingPaths.count(it.first));
        }
#endif

        fPendingPaths.insert(paths.begin(), paths.end());
    }

    std::unique_ptr<GrFragmentProcessor> makeClipProcessor(uint32_t oplistID,
                                                           const SkPath& deviceSpacePath,
                                                           const SkIRect& accessRect, int rtWidth,
                                                           int rtHeight, const GrCaps&);

    // GrOnFlushCallbackObject overrides.
    void preFlush(GrOnFlushResourceProvider*, const uint32_t* opListIDs, int numOpListIDs,
                  SkTArray<sk_sp<GrRenderTargetContext>>* out) override;
    void postFlush(GrDeferredUploadToken, const uint32_t* opListIDs, int numOpListIDs) override;

<<<<<<< HEAD
    void testingOnly_drawPathDirectly(const DrawPathArgs&);
    const GrUniqueKey& testingOnly_getStashedAtlasKey() const;
||||||| merged common ancestors
        const GrCCAtlas* fAtlas = nullptr;
        int16_t fAtlasOffsetX;
        int16_t fAtlasOffsetY;
        SkDEBUGCODE(bool fHasAtlas = false);
=======
    void purgeCacheEntriesOlderThan(GrProxyProvider*, const GrStdSteadyClock::time_point&);
>>>>>>> upstream-releases

    // If a path spans more pixels than this, we need to crop it or else analytic AA can run out of
    // fp32 precision.
    static constexpr float kPathCropThreshold = 1 << 16;

    static void CropPath(const SkPath&, const SkIRect& cropbox, SkPath* out);

    // Maximum inflation of path bounds due to stroking (from width, miter, caps). Strokes wider
    // than this will be converted to fill paths and drawn by the CCPR filler instead.
    static constexpr float kMaxBoundsInflationFromStroke = 4096;

    static float GetStrokeDevWidth(const SkMatrix&, const SkStrokeRec&,
                                   float* inflationRadius = nullptr);

private:
<<<<<<< HEAD
    GrCoverageCountingPathRenderer(AllowCaching);

    // GrPathRenderer overrides.
    StencilSupport onGetStencilSupport(const GrShape&) const override {
        return GrPathRenderer::kNoSupport_StencilSupport;
    }
    CanDrawPath onCanDrawPath(const CanDrawPathArgs&) const override;
    bool onDrawPath(const DrawPathArgs&) override;
||||||| merged common ancestors
    GrCoverageCountingPathRenderer(bool drawCachablePaths)
            : fDrawCachablePaths(drawCachablePaths) {}
=======
    GrCoverageCountingPathRenderer(AllowCaching, uint32_t contextUniqueID);

    // GrPathRenderer overrides.
    StencilSupport onGetStencilSupport(const GrShape&) const override {
        return GrPathRenderer::kNoSupport_StencilSupport;
    }
    CanDrawPath onCanDrawPath(const CanDrawPathArgs&) const override;
    bool onDrawPath(const DrawPathArgs&) override;
>>>>>>> upstream-releases

    GrCCPerOpListPaths* lookupPendingPaths(uint32_t opListID);
    void recordOp(std::unique_ptr<GrCCDrawPathsOp>, const DrawPathArgs&);

    // fPendingPaths holds the GrCCPerOpListPaths objects that have already been created, but not
    // flushed, and those that are still being created. All GrCCPerOpListPaths objects will first
    // reside in fPendingPaths, then be moved to fFlushingPaths during preFlush().
    PendingPathsMap fPendingPaths;

    // fFlushingPaths holds the GrCCPerOpListPaths objects that are currently being flushed.
    // (It will only contain elements when fFlushing is true.)
    SkSTArray<4, sk_sp<GrCCPerOpListPaths>> fFlushingPaths;

<<<<<<< HEAD
    std::unique_ptr<GrCCPathCache> fPathCache;
    GrUniqueKey fStashedAtlasKey;
||||||| merged common ancestors
    // A map from render target ID to the individual render target's pending paths.
    std::map<uint32_t, RTPendingPaths> fRTPendingPathsMap;
    SkDEBUGCODE(int fPendingDrawOpsCount = 0);
=======
    std::unique_ptr<GrCCPathCache> fPathCache;
>>>>>>> upstream-releases

    SkDEBUGCODE(bool fFlushing = false);
<<<<<<< HEAD
||||||| merged common ancestors

    const bool fDrawCachablePaths;
=======

public:
    void testingOnly_drawPathDirectly(const DrawPathArgs&);
    const GrCCPerFlushResources* testingOnly_getCurrentFlushResources();
    const GrCCPathCache* testingOnly_getPathCache() const;
>>>>>>> upstream-releases
};

#endif
