/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef GrVkPipelineState_DEFINED
#define GrVkPipelineState_DEFINED

#include "GrVkDescriptorSetManager.h"
#include "GrVkPipelineStateDataManager.h"
#include "glsl/GrGLSLProgramBuilder.h"
<<<<<<< HEAD
#include "vk/GrVkDefines.h"
||||||| merged common ancestors

#include "vk/GrVkDefines.h"
=======
#include "vk/GrVkTypes.h"
>>>>>>> upstream-releases

class GrPipeline;
class GrStencilSettings;
class GrVkBufferView;
class GrVkCommandBuffer;
class GrVkDescriptorPool;
class GrVkDescriptorSet;
class GrVkGpu;
class GrVkImageView;
class GrVkPipeline;
class GrVkPipelineLayout;
class GrVkSampler;
class GrVkTexture;
class GrVkUniformBuffer;

/**
 * This class holds onto a GrVkPipeline object that we use for draws. Besides storing the acutal
 * GrVkPipeline object, this class is also responsible handling all uniforms, descriptors, samplers,
 * and other similar objects that are used along with the VkPipeline in the draw. This includes both
 * allocating and freeing these objects, as well as updating their values.
 */
class GrVkPipelineState : public SkRefCnt {
public:
<<<<<<< HEAD
    using UniformInfoArray = GrVkPipelineStateDataManager::UniformInfoArray;
    using UniformHandle = GrGLSLProgramDataManager::UniformHandle;

    GrVkPipelineState(
            GrVkGpu* gpu,
            GrVkPipeline* pipeline,
            VkPipelineLayout layout,
            const GrVkDescriptorSetManager::Handle& samplerDSHandle,
            const GrGLSLBuiltinUniformHandles& builtinUniformHandles,
            const UniformInfoArray& uniforms,
            uint32_t geometryUniformSize,
            uint32_t fragmentUniformSize,
            uint32_t numSamplers,
            std::unique_ptr<GrGLSLPrimitiveProcessor> geometryProcessor,
            std::unique_ptr<GrGLSLXferProcessor> xferProcessor,
            std::unique_ptr<std::unique_ptr<GrGLSLFragmentProcessor>[]> fragmentProcessors,
            int fFragmentProcessorCnt);
||||||| merged common ancestors
    typedef GrGLSLProgramBuilder::BuiltinUniformHandles BuiltinUniformHandles;

    ~GrVkPipelineState();
=======
    using UniformInfoArray = GrVkPipelineStateDataManager::UniformInfoArray;
    using UniformHandle = GrGLSLProgramDataManager::UniformHandle;
>>>>>>> upstream-releases

<<<<<<< HEAD
    ~GrVkPipelineState();
||||||| merged common ancestors
    GrVkPipeline* vkPipeline() const { return fPipeline; }
=======
    GrVkPipelineState(
            GrVkGpu* gpu,
            GrVkPipeline* pipeline,
            VkPipelineLayout layout,
            const GrVkDescriptorSetManager::Handle& samplerDSHandle,
            const GrGLSLBuiltinUniformHandles& builtinUniformHandles,
            const UniformInfoArray& uniforms,
            uint32_t geometryUniformSize,
            uint32_t fragmentUniformSize,
            const UniformInfoArray& samplers,
            std::unique_ptr<GrGLSLPrimitiveProcessor> geometryProcessor,
            std::unique_ptr<GrGLSLXferProcessor> xferProcessor,
            std::unique_ptr<std::unique_ptr<GrGLSLFragmentProcessor>[]> fragmentProcessors,
            int fFragmentProcessorCnt);
>>>>>>> upstream-releases

<<<<<<< HEAD
    void setAndBindUniforms(GrVkGpu*, const GrPrimitiveProcessor&, const GrPipeline&,
                            GrVkCommandBuffer*);
    /**
     * This must be called after setAndBindUniforms() since that function invalidates texture
     * bindings.
     */
    void setAndBindTextures(GrVkGpu*, const GrPrimitiveProcessor&, const GrPipeline&,
                            const GrTextureProxy* const primitiveProcessorTextures[],
                            GrVkCommandBuffer*);
||||||| merged common ancestors
    void setData(GrVkGpu*, const GrPrimitiveProcessor&, const GrPipeline&);
=======
    ~GrVkPipelineState();
>>>>>>> upstream-releases

<<<<<<< HEAD
    void bindPipeline(const GrVkGpu* gpu, GrVkCommandBuffer* commandBuffer);
||||||| merged common ancestors
    void bind(const GrVkGpu* gpu, GrVkCommandBuffer* commandBuffer);
=======
    void setAndBindUniforms(GrVkGpu*, const GrRenderTarget*, GrSurfaceOrigin,
                            const GrPrimitiveProcessor&, const GrPipeline&, GrVkCommandBuffer*);
    /**
     * This must be called after setAndBindUniforms() since that function invalidates texture
     * bindings.
     */
    void setAndBindTextures(GrVkGpu*, const GrPrimitiveProcessor&, const GrPipeline&,
                            const GrTextureProxy* const primitiveProcessorTextures[],
                            GrVkCommandBuffer*);
>>>>>>> upstream-releases

<<<<<<< HEAD
    void addUniformResources(GrVkCommandBuffer&, GrVkSampler*[], GrVkTexture*[], int numTextures);
||||||| merged common ancestors
    void addUniformResources(GrVkCommandBuffer&);
=======
    void bindPipeline(const GrVkGpu* gpu, GrVkCommandBuffer* commandBuffer);
>>>>>>> upstream-releases

    void addUniformResources(GrVkCommandBuffer&, GrVkSampler*[], GrVkTexture*[], int numTextures);

<<<<<<< HEAD
||||||| merged common ancestors
    // This releases resources that only a given instance of a GrVkPipelineState needs to hold onto
    // and don't need to survive across new uses of the GrVkPipelineState.
    void freeTempResources(const GrVkGpu* gpu);

=======
    void freeGPUResources(GrVkGpu* gpu);

>>>>>>> upstream-releases
    void abandonGPUResources();

private:
    void writeUniformBuffers(const GrVkGpu* gpu);

    /**
     * We use the RT's size and origin to adjust from Skia device space to vulkan normalized device
     * space and to make device space positions have the correct origin for processors that require
     * them.
     */
    struct RenderTargetState {
        SkISize         fRenderTargetSize;
        GrSurfaceOrigin fRenderTargetOrigin;

        RenderTargetState() { this->invalidate(); }
        void invalidate() {
            fRenderTargetSize.fWidth = -1;
            fRenderTargetSize.fHeight = -1;
            fRenderTargetOrigin = (GrSurfaceOrigin)-1;
        }

        /**
        * Gets a float4 that adjusts the position from Skia device coords to Vulkans normalized device
        * coords. Assuming the transformed position, pos, is a homogeneous float3, the vec, v, is
        * applied as such:
        * pos.x = dot(v.xy, pos.xz)
        * pos.y = dot(v.zw, pos.yz)
        */
        void getRTAdjustmentVec(float* destVec) {
            destVec[0] = 2.f / fRenderTargetSize.fWidth;
            destVec[1] = -1.f;
            if (kBottomLeft_GrSurfaceOrigin == fRenderTargetOrigin) {
                destVec[2] = -2.f / fRenderTargetSize.fHeight;
                destVec[3] = 1.f;
            } else {
                destVec[2] = 2.f / fRenderTargetSize.fHeight;
                destVec[3] = -1.f;
            }
        }
    };

    // Helper for setData() that sets the view matrix and loads the render target height uniform
    void setRenderTargetState(const GrRenderTarget*, GrSurfaceOrigin);

    // GrVkResources
    GrVkPipeline* fPipeline;

    // Used for binding DescriptorSets to the command buffer but does not need to survive during
    // command buffer execution. Thus this is not need to be a GrVkResource.
    GrVkPipelineLayout* fPipelineLayout;

    // The DescriptorSets need to survive until the gpu has finished all draws that use them.
    // However, they will only be freed by the descriptor pool. Thus by simply keeping the
    // descriptor pool alive through the draw, the descritor sets will also stay alive. Thus we do
    // not need a GrVkResource versions of VkDescriptorSet. We hold on to these in the
    // GrVkPipelineState since we update the descriptor sets and bind them at separate times;
    VkDescriptorSet fDescriptorSets[3];

    const GrVkDescriptorSet* fUniformDescriptorSet;
    const GrVkDescriptorSet* fSamplerDescriptorSet;

    const GrVkDescriptorSetManager::Handle fSamplerDSHandle;
<<<<<<< HEAD
||||||| merged common ancestors
    const GrVkDescriptorSetManager::Handle fTexelBufferDSHandle;
=======

    SkSTArray<4, const GrVkSampler*>   fImmutableSamplers;
>>>>>>> upstream-releases

    std::unique_ptr<GrVkUniformBuffer> fGeometryUniformBuffer;
    std::unique_ptr<GrVkUniformBuffer> fFragmentUniformBuffer;

    // Tracks the current render target uniforms stored in the vertex buffer.
    RenderTargetState fRenderTargetState;
    GrGLSLBuiltinUniformHandles fBuiltinUniformHandles;

    // Processors in the GrVkPipelineState
    std::unique_ptr<GrGLSLPrimitiveProcessor> fGeometryProcessor;
    std::unique_ptr<GrGLSLXferProcessor> fXferProcessor;
    std::unique_ptr<std::unique_ptr<GrGLSLFragmentProcessor>[]> fFragmentProcessors;
    int fFragmentProcessorCnt;

    GrVkPipelineStateDataManager fDataManager;

    int fNumSamplers;
};

#endif
