/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrCCAtlas.h"

#include "GrCaps.h"
#include "GrOnFlushResourceProvider.h"
#include "GrProxyProvider.h"
#include "GrRectanizer_skyline.h"
#include "GrRenderTargetContext.h"
#include "GrTexture.h"
#include "GrTextureProxy.h"
#include "SkIPoint16.h"
#include "SkMakeUnique.h"
#include "SkMathPriv.h"
<<<<<<< HEAD
||||||| merged common ancestors
#include "ccpr/GrCCCoverageProcessor.h"
#include "ccpr/GrCCPathParser.h"
#include "ops/GrDrawOp.h"

static constexpr int kAtlasMinSize = 1024;
static constexpr int kPadding = 1;
=======
#include "ccpr/GrCCPathCache.h"
#include <atomic>
>>>>>>> upstream-releases

class GrCCAtlas::Node {
public:
    Node(std::unique_ptr<Node> previous, int l, int t, int r, int b)
            : fPrevious(std::move(previous)), fX(l), fY(t), fRectanizer(r - l, b - t) {}

    Node* previous() const { return fPrevious.get(); }

    bool addRect(int w, int h, SkIPoint16* loc, int maxAtlasSize) {
        // Pad all paths except those that are expected to take up an entire physical texture.
        if (w < maxAtlasSize) {
            w = SkTMin(w + kPadding, maxAtlasSize);
        }
        if (h < maxAtlasSize) {
            h = SkTMin(h + kPadding, maxAtlasSize);
        }
        if (!fRectanizer.addRect(w, h, loc)) {
            return false;
        }
        loc->fX += fX;
        loc->fY += fY;
        return true;
    }

private:
    const std::unique_ptr<Node> fPrevious;
    const int fX, fY;
    GrRectanizerSkyline fRectanizer;
};

<<<<<<< HEAD
GrCCAtlas::GrCCAtlas(GrPixelConfig pixelConfig, const Specs& specs, const GrCaps& caps)
        : fMaxTextureSize(SkTMax(SkTMax(specs.fMinHeight, specs.fMinWidth),
                                 specs.fMaxPreferredTextureSize)) {
||||||| merged common ancestors
class GrCCAtlas::DrawCoverageCountOp : public GrDrawOp {
public:
    DEFINE_OP_CLASS_ID

    DrawCoverageCountOp(sk_sp<const GrCCPathParser> parser, CoverageCountBatchID batchID,
                        const SkISize& drawBounds)
            : INHERITED(ClassID())
            , fParser(std::move(parser))
            , fBatchID(batchID)
            , fDrawBounds(drawBounds) {
        this->setBounds(SkRect::MakeIWH(fDrawBounds.width(), fDrawBounds.height()),
                        GrOp::HasAABloat::kNo, GrOp::IsZeroArea::kNo);
    }

    // GrDrawOp interface.
    const char* name() const override { return "GrCCAtlas::DrawCoverageCountOp"; }
    FixedFunctionFlags fixedFunctionFlags() const override { return FixedFunctionFlags::kNone; }
    RequiresDstTexture finalize(const GrCaps&, const GrAppliedClip*,
                                GrPixelConfigIsClamped) override { return RequiresDstTexture::kNo; }
    bool onCombineIfPossible(GrOp* other, const GrCaps& caps) override { return false; }
    void onPrepare(GrOpFlushState*) override {}
    void onExecute(GrOpFlushState* flushState) override {
        fParser->drawCoverageCount(flushState, fBatchID,
                                   SkIRect::MakeWH(fDrawBounds.width(), fDrawBounds.height()));
    }

private:
    const sk_sp<const GrCCPathParser> fParser;
    const CoverageCountBatchID fBatchID;
    const SkISize fDrawBounds;

    typedef GrDrawOp INHERITED;
};

GrCCAtlas::GrCCAtlas(const GrCaps& caps, int minSize)
        : fMaxAtlasSize(SkTMax(minSize, caps.maxPreferredRenderTargetSize())) {
=======
GrCCAtlas::GrCCAtlas(CoverageType coverageType, const Specs& specs, const GrCaps& caps)
        : fCoverageType(coverageType)
        , fMaxTextureSize(SkTMax(SkTMax(specs.fMinHeight, specs.fMinWidth),
                                 specs.fMaxPreferredTextureSize)) {
>>>>>>> upstream-releases
    // Caller should have cropped any paths to the destination render target instead of asking for
    // an atlas larger than maxRenderTargetSize.
    SkASSERT(fMaxTextureSize <= caps.maxTextureSize());
    SkASSERT(specs.fMaxPreferredTextureSize > 0);

    // Begin with the first pow2 dimensions whose area is theoretically large enough to contain the
    // pending paths, favoring height over width if necessary.
    int log2area = SkNextLog2(SkTMax(specs.fApproxNumPixels, 1));
    fHeight = 1 << ((log2area + 1) / 2);
    fWidth = 1 << (log2area / 2);

    fWidth = SkTClamp(fWidth, specs.fMinTextureSize, specs.fMaxPreferredTextureSize);
    fHeight = SkTClamp(fHeight, specs.fMinTextureSize, specs.fMaxPreferredTextureSize);

    if (fWidth < specs.fMinWidth || fHeight < specs.fMinHeight) {
        // They want to stuff a particularly large path into the atlas. Just punt and go with their
        // min width and height. The atlas will grow as needed.
        fWidth = SkTMin(specs.fMinWidth + kPadding, fMaxTextureSize);
        fHeight = SkTMin(specs.fMinHeight + kPadding, fMaxTextureSize);
    }

    fTopNode = skstd::make_unique<Node>(nullptr, 0, 0, fWidth, fHeight);
<<<<<<< HEAD

    fTextureProxy = GrProxyProvider::MakeFullyLazyProxy(
            [this, pixelConfig](GrResourceProvider* resourceProvider) {
                    if (!resourceProvider) {
                        return sk_sp<GrTexture>();
                    }
                    if (!fBackingTexture) {
                        GrSurfaceDesc desc;
                        desc.fFlags = kRenderTarget_GrSurfaceFlag;
                        desc.fWidth = fWidth;
                        desc.fHeight = fHeight;
                        desc.fConfig = pixelConfig;
                        fBackingTexture = resourceProvider->createTexture(desc, SkBudgeted::kYes);
                    }
                    return fBackingTexture;
            },
            GrProxyProvider::Renderable::kYes, kTextureOrigin, pixelConfig, caps);
||||||| merged common ancestors
=======

    GrColorType colorType = (CoverageType::kFP16_CoverageCount == fCoverageType)
            ? GrColorType::kAlpha_F16 : GrColorType::kAlpha_8;
    const GrBackendFormat format =
            caps.getBackendFormatFromGrColorType(colorType, GrSRGBEncoded::kNo);
    GrPixelConfig pixelConfig = (CoverageType::kFP16_CoverageCount == fCoverageType)
            ? kAlpha_half_GrPixelConfig : kAlpha_8_GrPixelConfig;

    fTextureProxy = GrProxyProvider::MakeFullyLazyProxy(
            [this, pixelConfig](GrResourceProvider* resourceProvider) {
                    if (!resourceProvider) {
                        return sk_sp<GrTexture>();
                    }
                    if (!fBackingTexture) {
                        GrSurfaceDesc desc;
                        desc.fFlags = kRenderTarget_GrSurfaceFlag;
                        desc.fWidth = fWidth;
                        desc.fHeight = fHeight;
                        desc.fConfig = pixelConfig;
                        fBackingTexture = resourceProvider->createTexture(desc, SkBudgeted::kYes);
                    }
                    return fBackingTexture;
            },
            format, GrProxyProvider::Renderable::kYes, kTextureOrigin, pixelConfig, caps);
>>>>>>> upstream-releases
}

GrCCAtlas::~GrCCAtlas() {
}

bool GrCCAtlas::addRect(const SkIRect& devIBounds, SkIVector* offset) {
    // This can't be called anymore once makeRenderTargetContext() has been called.
    SkASSERT(!fTextureProxy->isInstantiated());

    SkIPoint16 location;
    if (!this->internalPlaceRect(devIBounds.width(), devIBounds.height(), &location)) {
        return false;
    }
    offset->set(location.x() - devIBounds.left(), location.y() - devIBounds.top());

    fDrawBounds.fWidth = SkTMax(fDrawBounds.width(), location.x() + devIBounds.width());
    fDrawBounds.fHeight = SkTMax(fDrawBounds.height(), location.y() + devIBounds.height());
    return true;
}

bool GrCCAtlas::internalPlaceRect(int w, int h, SkIPoint16* loc) {
    for (Node* node = fTopNode.get(); node; node = node->previous()) {
        if (node->addRect(w, h, loc, fMaxTextureSize)) {
            return true;
        }
    }

    // The rect didn't fit. Grow the atlas and try again.
    do {
        if (fWidth == fMaxTextureSize && fHeight == fMaxTextureSize) {
            return false;
        }
        if (fHeight <= fWidth) {
            int top = fHeight;
            fHeight = SkTMin(fHeight * 2, fMaxTextureSize);
            fTopNode = skstd::make_unique<Node>(std::move(fTopNode), 0, top, fWidth, fHeight);
        } else {
            int left = fWidth;
            fWidth = SkTMin(fWidth * 2, fMaxTextureSize);
            fTopNode = skstd::make_unique<Node>(std::move(fTopNode), left, 0, fWidth, fHeight);
        }
    } while (!fTopNode->addRect(w, h, loc, fMaxTextureSize));

    return true;
}

<<<<<<< HEAD
void GrCCAtlas::setFillBatchID(int id) {
    // This can't be called anymore once makeRenderTargetContext() has been called.
    SkASSERT(!fTextureProxy->isInstantiated());
    fFillBatchID = id;
}

void GrCCAtlas::setStrokeBatchID(int id) {
    // This can't be called anymore once makeRenderTargetContext() has been called.
    SkASSERT(!fTextureProxy->isInstantiated());
    fStrokeBatchID = id;
}

static uint32_t next_atlas_unique_id() {
    static int32_t nextID;
    return sk_atomic_inc(&nextID);
}

const GrUniqueKey& GrCCAtlas::getOrAssignUniqueKey(GrOnFlushResourceProvider* onFlushRP) {
    static const GrUniqueKey::Domain kAtlasDomain = GrUniqueKey::GenerateDomain();

    if (!fUniqueKey.isValid()) {
        GrUniqueKey::Builder builder(&fUniqueKey, kAtlasDomain, 1, "CCPR Atlas");
        builder[0] = next_atlas_unique_id();
        builder.finish();

        if (fTextureProxy->isInstantiated()) {
            onFlushRP->assignUniqueKeyToProxy(fUniqueKey, fTextureProxy.get());
        }
    }
    return fUniqueKey;
}

sk_sp<GrCCAtlas::CachedAtlasInfo> GrCCAtlas::refOrMakeCachedAtlasInfo() {
    if (!fCachedAtlasInfo) {
        fCachedAtlasInfo = sk_make_sp<CachedAtlasInfo>();
    }
    return fCachedAtlasInfo;
}

sk_sp<GrRenderTargetContext> GrCCAtlas::makeRenderTargetContext(
        GrOnFlushResourceProvider* onFlushRP, sk_sp<GrTexture> backingTexture) {
    SkASSERT(!fTextureProxy->isInstantiated());  // This method should only be called once.
    // Caller should have cropped any paths to the destination render target instead of asking for
    // an atlas larger than maxRenderTargetSize.
    SkASSERT(SkTMax(fHeight, fWidth) <= fMaxTextureSize);
    SkASSERT(fMaxTextureSize <= onFlushRP->caps()->maxRenderTargetSize());

    if (backingTexture) {
        SkASSERT(backingTexture->config() == kAlpha_half_GrPixelConfig);
        SkASSERT(backingTexture->width() == fWidth);
        SkASSERT(backingTexture->height() == fHeight);
        fBackingTexture = std::move(backingTexture);
    }

    sk_sp<GrRenderTargetContext> rtc =
            onFlushRP->makeRenderTargetContext(fTextureProxy, nullptr, nullptr);
||||||| merged common ancestors
sk_sp<GrRenderTargetContext> GrCCAtlas::finalize(GrOnFlushResourceProvider* onFlushRP,
                                                 sk_sp<const GrCCPathParser> parser) {
    SkASSERT(fCoverageCountBatchID);
    SkASSERT(!fTextureProxy);

    GrSurfaceDesc desc;
    desc.fFlags = kRenderTarget_GrSurfaceFlag;
    desc.fOrigin = kTopLeft_GrSurfaceOrigin;
    desc.fWidth = fWidth;
    desc.fHeight = fHeight;
    desc.fConfig = kAlpha_half_GrPixelConfig;
    sk_sp<GrRenderTargetContext> rtc = onFlushRP->makeRenderTargetContext(desc, nullptr, nullptr);
=======
void GrCCAtlas::setFillBatchID(int id) {
    // This can't be called anymore once makeRenderTargetContext() has been called.
    SkASSERT(!fTextureProxy->isInstantiated());
    fFillBatchID = id;
}

void GrCCAtlas::setStrokeBatchID(int id) {
    // This can't be called anymore once makeRenderTargetContext() has been called.
    SkASSERT(!fTextureProxy->isInstantiated());
    fStrokeBatchID = id;
}

static uint32_t next_atlas_unique_id() {
    static std::atomic<uint32_t> nextID;
    return nextID++;
}

sk_sp<GrCCCachedAtlas> GrCCAtlas::refOrMakeCachedAtlas(GrOnFlushResourceProvider* onFlushRP) {
    if (!fCachedAtlas) {
        static const GrUniqueKey::Domain kAtlasDomain = GrUniqueKey::GenerateDomain();

        GrUniqueKey atlasUniqueKey;
        GrUniqueKey::Builder builder(&atlasUniqueKey, kAtlasDomain, 1, "CCPR Atlas");
        builder[0] = next_atlas_unique_id();
        builder.finish();

        onFlushRP->assignUniqueKeyToProxy(atlasUniqueKey, fTextureProxy.get());

        fCachedAtlas = sk_make_sp<GrCCCachedAtlas>(fCoverageType, atlasUniqueKey, fTextureProxy);
    }

    SkASSERT(fCachedAtlas->coverageType() == fCoverageType);
    SkASSERT(fCachedAtlas->getOnFlushProxy() == fTextureProxy.get());
    return fCachedAtlas;
}

sk_sp<GrRenderTargetContext> GrCCAtlas::makeRenderTargetContext(
        GrOnFlushResourceProvider* onFlushRP, sk_sp<GrTexture> backingTexture) {
    SkASSERT(!fTextureProxy->isInstantiated());  // This method should only be called once.
    // Caller should have cropped any paths to the destination render target instead of asking for
    // an atlas larger than maxRenderTargetSize.
    SkASSERT(SkTMax(fHeight, fWidth) <= fMaxTextureSize);
    SkASSERT(fMaxTextureSize <= onFlushRP->caps()->maxRenderTargetSize());

    if (backingTexture) {
        SkASSERT(backingTexture->config() == kAlpha_half_GrPixelConfig);
        SkASSERT(backingTexture->width() == fWidth);
        SkASSERT(backingTexture->height() == fHeight);
        fBackingTexture = std::move(backingTexture);
    }

    sk_sp<GrRenderTargetContext> rtc =
            onFlushRP->makeRenderTargetContext(fTextureProxy, nullptr, nullptr);
>>>>>>> upstream-releases
    if (!rtc) {
        SkDebugf("WARNING: failed to allocate a %ix%i atlas. Some paths will not be drawn.\n",
                 fWidth, fHeight);
        return nullptr;
    }

    if (fUniqueKey.isValid()) {
        onFlushRP->assignUniqueKeyToProxy(fUniqueKey, fTextureProxy.get());
    }

    SkIRect clearRect = SkIRect::MakeSize(fDrawBounds);
<<<<<<< HEAD
    rtc->clear(&clearRect, 0, GrRenderTargetContext::CanClearFullscreen::kYes);
||||||| merged common ancestors
    rtc->clear(&clearRect, 0, GrRenderTargetContext::CanClearFullscreen::kYes);

    auto op = skstd::make_unique<DrawCoverageCountOp>(std::move(parser), fCoverageCountBatchID,
                                                      fDrawBounds);
    rtc->addDrawOp(GrNoClip(), std::move(op));

    fTextureProxy = sk_ref_sp(rtc->asTextureProxy());
=======
    rtc->clear(&clearRect, SK_PMColor4fTRANSPARENT,
               GrRenderTargetContext::CanClearFullscreen::kYes);
>>>>>>> upstream-releases
    return rtc;
}
<<<<<<< HEAD

GrCCAtlas* GrCCAtlasStack::addRect(const SkIRect& devIBounds, SkIVector* devToAtlasOffset) {
    GrCCAtlas* retiredAtlas = nullptr;
    if (fAtlases.empty() || !fAtlases.back().addRect(devIBounds, devToAtlasOffset)) {
        // The retired atlas is out of room and can't grow any bigger.
        retiredAtlas = !fAtlases.empty() ? &fAtlases.back() : nullptr;
        fAtlases.emplace_back(fPixelConfig, fSpecs, *fCaps);
        SkASSERT(devIBounds.width() <= fSpecs.fMinWidth);
        SkASSERT(devIBounds.height() <= fSpecs.fMinHeight);
        SkAssertResult(fAtlases.back().addRect(devIBounds, devToAtlasOffset));
    }
    return retiredAtlas;
}
||||||| merged common ancestors
=======

GrCCAtlas* GrCCAtlasStack::addRect(const SkIRect& devIBounds, SkIVector* devToAtlasOffset) {
    GrCCAtlas* retiredAtlas = nullptr;
    if (fAtlases.empty() || !fAtlases.back().addRect(devIBounds, devToAtlasOffset)) {
        // The retired atlas is out of room and can't grow any bigger.
        retiredAtlas = !fAtlases.empty() ? &fAtlases.back() : nullptr;
        fAtlases.emplace_back(fCoverageType, fSpecs, *fCaps);
        SkASSERT(devIBounds.width() <= fSpecs.fMinWidth);
        SkASSERT(devIBounds.height() <= fSpecs.fMinHeight);
        SkAssertResult(fAtlases.back().addRect(devIBounds, devToAtlasOffset));
    }
    return retiredAtlas;
}
>>>>>>> upstream-releases
