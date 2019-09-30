/*
* Copyright 2016 Google Inc.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/

#include "GrGpuCommandBuffer.h"

#include "GrCaps.h"
#include "GrContext.h"
#include "GrContextPriv.h"
#include "GrFixedClip.h"
#include "GrGpu.h"
#include "GrMesh.h"
#include "GrPrimitiveProcessor.h"
#include "GrRenderTarget.h"
#include "SkRect.h"

<<<<<<< HEAD
void GrGpuRTCommandBuffer::clear(const GrFixedClip& clip, GrColor color) {
    SkASSERT(fRenderTarget);

||||||| merged common ancestors
void GrGpuRTCommandBuffer::clear(const GrFixedClip& clip, GrColor color) {
#ifdef SK_DEBUG
    GrRenderTarget* rt = fRenderTarget;
    SkASSERT(rt);
    SkASSERT(!clip.scissorEnabled() ||
             (SkIRect::MakeWH(rt->width(), rt->height()).contains(clip.scissorRect()) &&
              SkIRect::MakeWH(rt->width(), rt->height()) != clip.scissorRect()));
#endif
=======
void GrGpuRTCommandBuffer::clear(const GrFixedClip& clip, const SkPMColor4f& color) {
    SkASSERT(fRenderTarget);
    // A clear at this level will always be a true clear, so make sure clears were not supposed to
    // be redirected to draws instead
    SkASSERT(!this->gpu()->caps()->performColorClearsAsDraws());
    SkASSERT(!clip.scissorEnabled() || !this->gpu()->caps()->performPartialClearsAsDraws());
>>>>>>> upstream-releases
    this->onClear(clip, color);
}

void GrGpuRTCommandBuffer::clearStencilClip(const GrFixedClip& clip, bool insideStencilMask) {
    // As above, make sure the stencil clear wasn't supposed to be a draw rect with stencil settings
    SkASSERT(!this->gpu()->caps()->performStencilClearsAsDraws());
    this->onClearStencilClip(clip, insideStencilMask);
}

bool GrGpuRTCommandBuffer::draw(const GrPrimitiveProcessor& primProc, const GrPipeline& pipeline,
                                const GrPipeline::FixedDynamicState* fixedDynamicState,
                                const GrPipeline::DynamicStateArrays* dynamicStateArrays,
                                const GrMesh meshes[], int meshCount, const SkRect& bounds) {
#ifdef SK_DEBUG
    SkASSERT(!primProc.hasInstanceAttributes() || this->gpu()->caps()->instanceAttribSupport());
    for (int i = 0; i < meshCount; ++i) {
        SkASSERT(!GrPrimTypeRequiresGeometryShaderSupport(meshes[i].primitiveType()) ||
                 this->gpu()->caps()->shaderCaps()->geometryShaderSupport());
<<<<<<< HEAD
        SkASSERT(primProc.hasVertexAttributes() == meshes[i].hasVertexData());
        SkASSERT(primProc.hasInstanceAttributes() == meshes[i].isInstanced());
||||||| merged common ancestors
        SkASSERT(primProc.hasVertexAttribs() == meshes[i].hasVertexData());
        SkASSERT(primProc.hasInstanceAttribs() == meshes[i].isInstanced());
=======
        SkASSERT(primProc.hasVertexAttributes() == meshes[i].hasVertexData());
        SkASSERT(primProc.hasInstanceAttributes() == meshes[i].hasInstanceData());
>>>>>>> upstream-releases
    }
#endif
<<<<<<< HEAD
    SkASSERT(!pipeline.isScissorEnabled() || fixedDynamicState ||
             (dynamicStateArrays && dynamicStateArrays->fScissorRects));

    auto resourceProvider = this->gpu()->getContext()->contextPriv().resourceProvider();
||||||| merged common ancestors
    auto resourceProvider = this->gpu()->getContext()->contextPriv().resourceProvider();
=======
    SkASSERT(!pipeline.isScissorEnabled() || fixedDynamicState ||
             (dynamicStateArrays && dynamicStateArrays->fScissorRects));

    auto resourceProvider = this->gpu()->getContext()->priv().resourceProvider();
>>>>>>> upstream-releases

    if (pipeline.isBad()) {
        return false;
    }
<<<<<<< HEAD
    if (fixedDynamicState && fixedDynamicState->fPrimitiveProcessorTextures) {
        for (int i = 0; i < primProc.numTextureSamplers(); ++i) {
            if (!fixedDynamicState->fPrimitiveProcessorTextures[i]->instantiate(resourceProvider)) {
                return false;
            }
        }
    }
    if (dynamicStateArrays && dynamicStateArrays->fPrimitiveProcessorTextures) {
        int n = primProc.numTextureSamplers() * meshCount;
        const auto* textures = dynamicStateArrays->fPrimitiveProcessorTextures;
        for (int i = 0; i < n; ++i) {
            if (!textures[i]->instantiate(resourceProvider)) {
                return false;
            }
        }
    }
||||||| merged common ancestors
=======
    if (fixedDynamicState && fixedDynamicState->fPrimitiveProcessorTextures) {
        for (int i = 0; i < primProc.numTextureSamplers(); ++i) {
            if (!fixedDynamicState->fPrimitiveProcessorTextures[i]->instantiate(resourceProvider)) {
                return false;
            }
        }
    }
    if (dynamicStateArrays && dynamicStateArrays->fPrimitiveProcessorTextures) {
        int n = primProc.numTextureSamplers() * meshCount;
        const auto* textures = dynamicStateArrays->fPrimitiveProcessorTextures;
        for (int i = 0; i < n; ++i) {
            if (!textures[i]->instantiate(resourceProvider)) {
                return false;
            }
        }
#ifdef SK_DEBUG
        SkASSERT(meshCount >= 1);
        const GrTextureProxy* const* primProcProxies =
                dynamicStateArrays->fPrimitiveProcessorTextures;
        for (int i = 0; i < primProc.numTextureSamplers(); ++i) {
            const GrBackendFormat& format = primProcProxies[i]->backendFormat();
            GrTextureType type = primProcProxies[i]->textureType();
            GrPixelConfig config = primProcProxies[i]->config();
            for (int j = 1; j < meshCount; ++j) {
                const GrTextureProxy* testProxy =
                        primProcProxies[j*primProc.numTextureSamplers() + i];
                SkASSERT(testProxy->backendFormat() == format);
                SkASSERT(testProxy->textureType() == type);
                SkASSERT(testProxy->config() == config);
            }
        }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (primProc.numVertexAttributes() > this->gpu()->caps()->maxVertexAttributes()) {
||||||| merged common ancestors
    if (primProc.numAttribs() > this->gpu()->caps()->maxVertexAttributes()) {
=======
    }

    if (primProc.numVertexAttributes() > this->gpu()->caps()->maxVertexAttributes()) {
>>>>>>> upstream-releases
        this->gpu()->stats()->incNumFailedDraws();
        return false;
    }
    this->onDraw(primProc, pipeline, fixedDynamicState, dynamicStateArrays, meshes, meshCount,
                 bounds);
    return true;
}
