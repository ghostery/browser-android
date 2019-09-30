/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkInternalAtlasTextContext.h"
#include "GrContext.h"
#include "GrContextPriv.h"
#include "SkAtlasTextContext.h"
#include "SkAtlasTextRenderer.h"
<<<<<<< HEAD
#include "text/GrGlyphCache.h"
||||||| merged common ancestors
#include "text/GrAtlasGlyphCache.h"
=======
#include "text/GrStrikeCache.h"
>>>>>>> upstream-releases

SkAtlasTextRenderer* SkGetAtlasTextRendererFromInternalContext(
        class SkInternalAtlasTextContext& internal) {
    return internal.renderer();
}

//////////////////////////////////////////////////////////////////////////////

std::unique_ptr<SkInternalAtlasTextContext> SkInternalAtlasTextContext::Make(
        sk_sp<SkAtlasTextRenderer> renderer) {
    return std::unique_ptr<SkInternalAtlasTextContext>(
            new SkInternalAtlasTextContext(std::move(renderer)));
}

SkInternalAtlasTextContext::SkInternalAtlasTextContext(sk_sp<SkAtlasTextRenderer> renderer)
        : fRenderer(std::move(renderer)) {
    GrContextOptions options;
    options.fAllowMultipleGlyphCacheTextures = GrContextOptions::Enable::kNo;
    options.fMinDistanceFieldFontSize = 0.f;
    options.fGlyphsAsPathsFontSize = SK_ScalarInfinity;
    options.fDistanceFieldGlyphVerticesAlwaysHaveW = GrContextOptions::Enable::kYes;
    fGrContext = GrContext::MakeMock(nullptr, options);
}

SkInternalAtlasTextContext::~SkInternalAtlasTextContext() {
    if (fDistanceFieldAtlas.fProxy) {
#ifdef SK_DEBUG
<<<<<<< HEAD
        auto atlasManager = fGrContext->contextPriv().getAtlasManager();
        if (atlasManager) {
            unsigned int numProxies;
            atlasManager->getProxies(kA8_GrMaskFormat, &numProxies);
            SkASSERT(1 == numProxies);
        }
||||||| merged common ancestors
        auto atlasGlyphCache = fGrContext->contextPriv().getAtlasGlyphCache();
        unsigned int numProxies;
        atlasGlyphCache->getProxies(kA8_GrMaskFormat, &numProxies);
        SkASSERT(1 == numProxies);
=======
        auto atlasManager = fGrContext->priv().getAtlasManager();
        if (atlasManager) {
            unsigned int numProxies;
            atlasManager->getProxies(kA8_GrMaskFormat, &numProxies);
            SkASSERT(1 == numProxies);
        }
>>>>>>> upstream-releases
#endif
        fRenderer->deleteTexture(fDistanceFieldAtlas.fTextureHandle);
    }
}

<<<<<<< HEAD
GrGlyphCache* SkInternalAtlasTextContext::glyphCache() {
    return fGrContext->contextPriv().getGlyphCache();
||||||| merged common ancestors
GrAtlasGlyphCache* SkInternalAtlasTextContext::atlasGlyphCache() {
    return fGrContext->contextPriv().getAtlasGlyphCache();
=======
GrStrikeCache* SkInternalAtlasTextContext::glyphCache() {
    return fGrContext->priv().getGrStrikeCache();
>>>>>>> upstream-releases
}

GrTextBlobCache* SkInternalAtlasTextContext::textBlobCache() {
    return fGrContext->priv().getTextBlobCache();
}

GrDeferredUploadToken SkInternalAtlasTextContext::addInlineUpload(
        GrDeferredTextureUploadFn&& upload) {
    auto token = fTokenTracker.nextDrawToken();
    fInlineUploads.append(&fArena, InlineUpload{std::move(upload), token});
    return token;
}

GrDeferredUploadToken SkInternalAtlasTextContext::addASAPUpload(
        GrDeferredTextureUploadFn&& upload) {
    fASAPUploads.append(&fArena, std::move(upload));
    return fTokenTracker.nextTokenToFlush();
}

void SkInternalAtlasTextContext::recordDraw(const void* srcVertexData, int glyphCnt,
                                            const SkMatrix& matrix, void* targetHandle) {
    auto vertexDataSize = sizeof(SkAtlasTextRenderer::SDFVertex) * 4 * glyphCnt;
    auto vertexData = fArena.makeArrayDefault<char>(vertexDataSize);
    memcpy(vertexData, srcVertexData, vertexDataSize);
    for (int i = 0; i < 4 * glyphCnt; ++i) {
        auto* vertex = reinterpret_cast<SkAtlasTextRenderer::SDFVertex*>(vertexData) + i;
        // GrTextContext encodes a texture index into the lower bit of each texture coord.
        // This isn't expected by SkAtlasTextRenderer subclasses.
        vertex->fTextureCoordX /= 2;
        vertex->fTextureCoordY /= 2;
        matrix.mapHomogeneousPoints(&vertex->fPosition, &vertex->fPosition, 1);
    }
    fDraws.append(&fArena,
                  Draw{glyphCnt, fTokenTracker.issueDrawToken(), targetHandle, vertexData});
}

void SkInternalAtlasTextContext::flush() {
<<<<<<< HEAD
    auto* atlasManager = fGrContext->contextPriv().getAtlasManager();
||||||| merged common ancestors
    auto* atlasGlyphCache = fGrContext->contextPriv().getAtlasGlyphCache();
=======
    auto* atlasManager = fGrContext->priv().getAtlasManager();
>>>>>>> upstream-releases
    if (!fDistanceFieldAtlas.fProxy) {
        unsigned int numProxies;
        fDistanceFieldAtlas.fProxy = atlasManager->getProxies(kA8_GrMaskFormat, &numProxies)->get();
        SkASSERT(1 == numProxies);
        fDistanceFieldAtlas.fTextureHandle =
                fRenderer->createTexture(SkAtlasTextRenderer::AtlasFormat::kA8,
                                         fDistanceFieldAtlas.fProxy->width(),
                                         fDistanceFieldAtlas.fProxy->height());
    }
    GrDeferredTextureUploadWritePixelsFn writePixelsFn =
            [this](GrTextureProxy* proxy, int left, int top, int width, int height,
                   GrColorType colorType, const void* data, size_t rowBytes) -> bool {
        SkASSERT(GrColorType::kAlpha_8 == colorType);
        SkASSERT(proxy == this->fDistanceFieldAtlas.fProxy);
        void* handle = fDistanceFieldAtlas.fTextureHandle;
        this->fRenderer->setTextureData(handle, data, left, top, width, height, rowBytes);
        return true;
    };
    for (const auto& upload : fASAPUploads) {
        upload(writePixelsFn);
    }
    auto inlineUpload = fInlineUploads.begin();
    for (const auto& draw : fDraws) {
        while (inlineUpload != fInlineUploads.end() && inlineUpload->fToken == draw.fToken) {
            inlineUpload->fUpload(writePixelsFn);
            ++inlineUpload;
        }
        auto vertices = reinterpret_cast<const SkAtlasTextRenderer::SDFVertex*>(draw.fVertexData);
        fRenderer->drawSDFGlyphs(draw.fTargetHandle, fDistanceFieldAtlas.fTextureHandle, vertices,
                                 draw.fGlyphCnt);
        fTokenTracker.flushToken();
    }
    fASAPUploads.reset();
    fInlineUploads.reset();
    fDraws.reset();
    fArena.reset();
}
