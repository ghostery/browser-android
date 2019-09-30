/*
* Copyright 2016 Google Inc.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/

#include "GrVkPipelineState.h"
#include "GrContext.h"
#include "GrContextPriv.h"
#include "GrPipeline.h"
#include "GrRenderTarget.h"
#include "GrTexturePriv.h"
#include "GrVkBufferView.h"
#include "GrVkCommandBuffer.h"
#include "GrVkDescriptorPool.h"
#include "GrVkDescriptorSet.h"
#include "GrVkGpu.h"
#include "GrVkImageView.h"
#include "GrVkMemory.h"
#include "GrVkPipeline.h"
#include "GrVkPipelineLayout.h"
#include "GrVkSampler.h"
#include "GrVkTexture.h"
#include "GrVkUniformBuffer.h"
#include "SkMipMap.h"
#include "glsl/GrGLSLFragmentProcessor.h"
#include "glsl/GrGLSLGeometryProcessor.h"
#include "glsl/GrGLSLXferProcessor.h"

<<<<<<< HEAD
GrVkPipelineState::GrVkPipelineState(
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
        int fragmentProcessorCnt)
        : fPipeline(pipeline)
        , fPipelineLayout(new GrVkPipelineLayout(layout))
        , fUniformDescriptorSet(nullptr)
        , fSamplerDescriptorSet(nullptr)
        , fSamplerDSHandle(samplerDSHandle)
        , fBuiltinUniformHandles(builtinUniformHandles)
        , fGeometryProcessor(std::move(geometryProcessor))
        , fXferProcessor(std::move(xferProcessor))
        , fFragmentProcessors(std::move(fragmentProcessors))
        , fFragmentProcessorCnt(fragmentProcessorCnt)
        , fDataManager(uniforms, geometryUniformSize, fragmentUniformSize) {
||||||| merged common ancestors
=======
GrVkPipelineState::GrVkPipelineState(
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
        int fragmentProcessorCnt)
        : fPipeline(pipeline)
        , fPipelineLayout(new GrVkPipelineLayout(layout))
        , fUniformDescriptorSet(nullptr)
        , fSamplerDescriptorSet(nullptr)
        , fSamplerDSHandle(samplerDSHandle)
        , fBuiltinUniformHandles(builtinUniformHandles)
        , fGeometryProcessor(std::move(geometryProcessor))
        , fXferProcessor(std::move(xferProcessor))
        , fFragmentProcessors(std::move(fragmentProcessors))
        , fFragmentProcessorCnt(fragmentProcessorCnt)
        , fDataManager(uniforms, geometryUniformSize, fragmentUniformSize) {
>>>>>>> upstream-releases
    fDescriptorSets[0] = VK_NULL_HANDLE;
    fDescriptorSets[1] = VK_NULL_HANDLE;
    fDescriptorSets[2] = VK_NULL_HANDLE;

    fGeometryUniformBuffer.reset(GrVkUniformBuffer::Create(gpu, geometryUniformSize));
    fFragmentUniformBuffer.reset(GrVkUniformBuffer::Create(gpu, fragmentUniformSize));

<<<<<<< HEAD
    fNumSamplers = numSamplers;
||||||| merged common ancestors
    fNumSamplers = numSamplers;
    fNumTexelBuffers = numTexelBuffers;
=======
    fNumSamplers = samplers.count();

    for (int i = 0; i < fNumSamplers; ++i) {
        // We store the immutable samplers here and take ownership of the ref from the
        // GrVkUnformHandler.
        fImmutableSamplers.push_back(samplers[i].fImmutableSampler);
    }
>>>>>>> upstream-releases
}

GrVkPipelineState::~GrVkPipelineState() {
    // Must have freed all GPU resources before this is destroyed
    SkASSERT(!fPipeline);
    SkASSERT(!fPipelineLayout);
<<<<<<< HEAD
}

void GrVkPipelineState::freeGPUResources(const GrVkGpu* gpu) {
||||||| merged common ancestors
    SkASSERT(!fSamplers.count());
    SkASSERT(!fTextureViews.count());
    SkASSERT(!fTextures.count());
    SkASSERT(!fBufferViews.count());
    SkASSERT(!fTexelBuffers.count());

    for (int i = 0; i < fFragmentProcessors.count(); ++i) {
        delete fFragmentProcessors[i];
    }
}

void GrVkPipelineState::freeTempResources(const GrVkGpu* gpu) {
    for (int i = 0; i < fSamplers.count(); ++i) {
        fSamplers[i]->unref(gpu);
    }
    fSamplers.rewind();

    for (int i = 0; i < fTextureViews.count(); ++i) {
        fTextureViews[i]->unref(gpu);
    }
    fTextureViews.rewind();

    for (int i = 0; i < fTextures.count(); ++i) {
        fTextures[i]->unref(gpu);
    }
    fTextures.rewind();

    for (int i = 0; i < fBufferViews.count(); ++i) {
        fBufferViews[i]->unref(gpu);
    }
    fBufferViews.rewind();

    for (int i = 0; i < fTexelBuffers.count(); ++i) {
        fTexelBuffers[i]->unref(gpu);
    }
    fTexelBuffers.rewind();
}

void GrVkPipelineState::freeGPUResources(const GrVkGpu* gpu) {
=======
}

void GrVkPipelineState::freeGPUResources(GrVkGpu* gpu) {
>>>>>>> upstream-releases
    if (fPipeline) {
        fPipeline->unref(gpu);
        fPipeline = nullptr;
    }

    if (fPipelineLayout) {
        fPipelineLayout->unref(gpu);
        fPipelineLayout = nullptr;
    }

    if (fGeometryUniformBuffer) {
        fGeometryUniformBuffer->release(gpu);
        fGeometryUniformBuffer.reset();
    }

    if (fFragmentUniformBuffer) {
        fFragmentUniformBuffer->release(gpu);
        fFragmentUniformBuffer.reset();
    }

    if (fUniformDescriptorSet) {
        fUniformDescriptorSet->recycle(const_cast<GrVkGpu*>(gpu));
        fUniformDescriptorSet = nullptr;
    }

    if (fSamplerDescriptorSet) {
        fSamplerDescriptorSet->recycle(const_cast<GrVkGpu*>(gpu));
        fSamplerDescriptorSet = nullptr;
    }
}

void GrVkPipelineState::abandonGPUResources() {
    if (fPipeline) {
        fPipeline->unrefAndAbandon();
        fPipeline = nullptr;
    }

    if (fPipelineLayout) {
        fPipelineLayout->unrefAndAbandon();
        fPipelineLayout = nullptr;
    }

    if (fGeometryUniformBuffer) {
        fGeometryUniformBuffer->abandon();
        fGeometryUniformBuffer.reset();
    }

    if (fFragmentUniformBuffer) {
        fFragmentUniformBuffer->abandon();
        fFragmentUniformBuffer.reset();
    }

    if (fUniformDescriptorSet) {
        fUniformDescriptorSet->unrefAndAbandon();
        fUniformDescriptorSet = nullptr;
    }

    if (fSamplerDescriptorSet) {
        fSamplerDescriptorSet->unrefAndAbandon();
        fSamplerDescriptorSet = nullptr;
    }
}

<<<<<<< HEAD
void GrVkPipelineState::setAndBindUniforms(GrVkGpu* gpu,
                                           const GrPrimitiveProcessor& primProc,
                                           const GrPipeline& pipeline,
                                           GrVkCommandBuffer* commandBuffer) {
    this->setRenderTargetState(pipeline.proxy());

||||||| merged common ancestors
static void append_texture_bindings(
        const GrResourceIOProcessor& processor,
        SkTArray<const GrResourceIOProcessor::TextureSampler*>* textureBindings,
        SkTArray<const GrResourceIOProcessor::BufferAccess*>* bufferAccesses) {
    if (int numTextureSamplers = processor.numTextureSamplers()) {
        const GrResourceIOProcessor::TextureSampler** bindings =
                textureBindings->push_back_n(numTextureSamplers);
        int i = 0;
        do {
            bindings[i] = &processor.textureSampler(i);
        } while (++i < numTextureSamplers);
    }
    if (int numTexelBuffers = processor.numBuffers()) {
        const GrResourceIOProcessor::BufferAccess** accesses  =
                bufferAccesses->push_back_n(numTexelBuffers);
        int i = 0;
        do {
            accesses[i] = &processor.bufferAccess(i);
        } while (++i < numTexelBuffers);
    }
}

void GrVkPipelineState::setData(GrVkGpu* gpu,
                                const GrPrimitiveProcessor& primProc,
                                const GrPipeline& pipeline) {
    // This is here to protect against someone calling setData multiple times in a row without
    // freeing the tempData between calls.
    this->freeTempResources(gpu);

    this->setRenderTargetState(pipeline.proxy());

    SkSTArray<8, const GrResourceIOProcessor::TextureSampler*> textureBindings;
    SkSTArray<8, const GrResourceIOProcessor::BufferAccess*> bufferAccesses;

=======
void GrVkPipelineState::setAndBindUniforms(GrVkGpu* gpu,
                                           const GrRenderTarget* renderTarget,
                                           GrSurfaceOrigin origin,
                                           const GrPrimitiveProcessor& primProc,
                                           const GrPipeline& pipeline,
                                           GrVkCommandBuffer* commandBuffer) {
    this->setRenderTargetState(renderTarget, origin);

>>>>>>> upstream-releases
    fGeometryProcessor->setData(fDataManager, primProc,
                                GrFragmentProcessor::CoordTransformIter(pipeline));
    GrFragmentProcessor::Iter iter(pipeline);
    GrGLSLFragmentProcessor::Iter glslIter(fFragmentProcessors.get(), fFragmentProcessorCnt);
    const GrFragmentProcessor* fp = iter.next();
    GrGLSLFragmentProcessor* glslFP = glslIter.next();
    while (fp && glslFP) {
        glslFP->setData(fDataManager, *fp);
        fp = iter.next();
        glslFP = glslIter.next();
    }
    SkASSERT(!fp && !glslFP);

    {
        SkIPoint offset;
        GrTexture* dstTexture = pipeline.peekDstTexture(&offset);

        fXferProcessor->setData(fDataManager, pipeline.getXferProcessor(), dstTexture, offset);
    }

    // Get new descriptor set
    if (fGeometryUniformBuffer || fFragmentUniformBuffer) {
        int uniformDSIdx = GrVkUniformHandler::kUniformBufferDescSet;
        if (fDataManager.uploadUniformBuffers(
                    gpu, fGeometryUniformBuffer.get(), fFragmentUniformBuffer.get()) ||
            !fUniformDescriptorSet) {
            if (fUniformDescriptorSet) {
                fUniformDescriptorSet->recycle(gpu);
            }
            fUniformDescriptorSet = gpu->resourceProvider().getUniformDescriptorSet();
            fDescriptorSets[uniformDSIdx] = fUniformDescriptorSet->descriptorSet();
            this->writeUniformBuffers(gpu);
        }
        commandBuffer->bindDescriptorSets(gpu, this, fPipelineLayout, uniformDSIdx, 1,
                                          &fDescriptorSets[uniformDSIdx], 0, nullptr);
        if (fUniformDescriptorSet) {
            commandBuffer->addRecycledResource(fUniformDescriptorSet);
        }
        if (fGeometryUniformBuffer) {
            commandBuffer->addRecycledResource(fGeometryUniformBuffer->resource());
        }
        if (fFragmentUniformBuffer) {
            commandBuffer->addRecycledResource(fFragmentUniformBuffer->resource());
        }
    }
}

void GrVkPipelineState::setAndBindTextures(GrVkGpu* gpu,
                                           const GrPrimitiveProcessor& primProc,
                                           const GrPipeline& pipeline,
                                           const GrTextureProxy* const primProcTextures[],
                                           GrVkCommandBuffer* commandBuffer) {
    SkASSERT(primProcTextures || !primProc.numTextureSamplers());

    struct SamplerBindings {
        GrSamplerState fState;
        GrVkTexture* fTexture;
    };
    SkAutoSTMalloc<8, SamplerBindings> samplerBindings(fNumSamplers);
    int currTextureBinding = 0;

    fGeometryProcessor->setData(fDataManager, primProc,
                                GrFragmentProcessor::CoordTransformIter(pipeline));
    for (int i = 0; i < primProc.numTextureSamplers(); ++i) {
        const auto& sampler = primProc.textureSampler(i);
        auto texture = static_cast<GrVkTexture*>(primProcTextures[i]->peekTexture());
        samplerBindings[currTextureBinding++] = {sampler.samplerState(), texture};
    }

    GrFragmentProcessor::Iter iter(pipeline);
    GrGLSLFragmentProcessor::Iter glslIter(fFragmentProcessors.get(), fFragmentProcessorCnt);
    const GrFragmentProcessor* fp = iter.next();
    GrGLSLFragmentProcessor* glslFP = glslIter.next();
    while (fp && glslFP) {
        for (int i = 0; i < fp->numTextureSamplers(); ++i) {
            const auto& sampler = fp->textureSampler(i);
            samplerBindings[currTextureBinding++] =
                    {sampler.samplerState(), static_cast<GrVkTexture*>(sampler.peekTexture())};
        }
        fp = iter.next();
        glslFP = glslIter.next();
    }
    SkASSERT(!fp && !glslFP);

    if (GrTextureProxy* dstTextureProxy = pipeline.dstTextureProxy()) {
        samplerBindings[currTextureBinding++] = {
                GrSamplerState::ClampNearest(),
                static_cast<GrVkTexture*>(dstTextureProxy->peekTexture())};
    }

    // Get new descriptor set
    SkASSERT(fNumSamplers == currTextureBinding);
    if (fNumSamplers) {
        if (fSamplerDescriptorSet) {
            fSamplerDescriptorSet->recycle(gpu);
        }
        fSamplerDescriptorSet = gpu->resourceProvider().getSamplerDescriptorSet(fSamplerDSHandle);
        int samplerDSIdx = GrVkUniformHandler::kSamplerDescSet;
        fDescriptorSets[samplerDSIdx] = fSamplerDescriptorSet->descriptorSet();
<<<<<<< HEAD
        for (int i = 0; i < fNumSamplers; ++i) {
            const GrSamplerState& state = samplerBindings[i].fState;
            GrVkTexture* texture = samplerBindings[i].fTexture;

            const GrVkImageView* textureView = texture->textureView();
            GrVkSampler* sampler = gpu->resourceProvider().findOrCreateCompatibleSampler(
                    state, texture->texturePriv().maxMipMapLevel());

            VkDescriptorImageInfo imageInfo;
            memset(&imageInfo, 0, sizeof(VkDescriptorImageInfo));
            imageInfo.sampler = sampler->sampler();
            imageInfo.imageView = textureView->imageView();
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            VkWriteDescriptorSet writeInfo;
            memset(&writeInfo, 0, sizeof(VkWriteDescriptorSet));
            writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeInfo.pNext = nullptr;
            writeInfo.dstSet = fDescriptorSets[GrVkUniformHandler::kSamplerDescSet];
            writeInfo.dstBinding = i;
            writeInfo.dstArrayElement = 0;
            writeInfo.descriptorCount = 1;
            writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writeInfo.pImageInfo = &imageInfo;
            writeInfo.pBufferInfo = nullptr;
            writeInfo.pTexelBufferView = nullptr;

            GR_VK_CALL(gpu->vkInterface(),
                       UpdateDescriptorSets(gpu->device(), 1, &writeInfo, 0, nullptr));
            commandBuffer->addResource(sampler);
            sampler->unref(gpu);
            commandBuffer->addResource(samplerBindings[i].fTexture->textureView());
            commandBuffer->addResource(samplerBindings[i].fTexture->resource());
        }

        commandBuffer->bindDescriptorSets(gpu, this, fPipelineLayout, samplerDSIdx, 1,
                                          &fDescriptorSets[samplerDSIdx], 0, nullptr);
        commandBuffer->addRecycledResource(fSamplerDescriptorSet);
||||||| merged common ancestors
        this->writeSamplers(gpu, textureBindings, pipeline.getAllowSRGBInputs());
    }

    if (fNumTexelBuffers) {
        if (fTexelBufferDescriptorSet) {
            fTexelBufferDescriptorSet->recycle(gpu);
        }
        fTexelBufferDescriptorSet =
                gpu->resourceProvider().getSamplerDescriptorSet(fTexelBufferDSHandle);
        int texelBufferDSIdx = GrVkUniformHandler::kTexelBufferDescSet;
        fDescriptorSets[texelBufferDSIdx] = fTexelBufferDescriptorSet->descriptorSet();
        this->writeTexelBuffers(gpu, bufferAccesses);
    }

    if (fGeometryUniformBuffer || fFragmentUniformBuffer) {
        if (fDataManager.uploadUniformBuffers(gpu,
                                              fGeometryUniformBuffer.get(),
                                              fFragmentUniformBuffer.get())
            || !fUniformDescriptorSet)
        {
            if (fUniformDescriptorSet) {
                fUniformDescriptorSet->recycle(gpu);
            }
            fUniformDescriptorSet = gpu->resourceProvider().getUniformDescriptorSet();
            int uniformDSIdx = GrVkUniformHandler::kUniformBufferDescSet;
            fDescriptorSets[uniformDSIdx] = fUniformDescriptorSet->descriptorSet();
            this->writeUniformBuffers(gpu);
        }
=======
        for (int i = 0; i < fNumSamplers; ++i) {
            const GrSamplerState& state = samplerBindings[i].fState;
            GrVkTexture* texture = samplerBindings[i].fTexture;

            const GrVkImageView* textureView = texture->textureView();
            const GrVkSampler* sampler = nullptr;
            if (fImmutableSamplers[i]) {
                sampler = fImmutableSamplers[i];
            } else {
                sampler = gpu->resourceProvider().findOrCreateCompatibleSampler(
                    state, texture->ycbcrConversionInfo());
            }
            SkASSERT(sampler);

            VkDescriptorImageInfo imageInfo;
            memset(&imageInfo, 0, sizeof(VkDescriptorImageInfo));
            imageInfo.sampler = sampler->sampler();
            imageInfo.imageView = textureView->imageView();
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            VkWriteDescriptorSet writeInfo;
            memset(&writeInfo, 0, sizeof(VkWriteDescriptorSet));
            writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeInfo.pNext = nullptr;
            writeInfo.dstSet = fDescriptorSets[GrVkUniformHandler::kSamplerDescSet];
            writeInfo.dstBinding = i;
            writeInfo.dstArrayElement = 0;
            writeInfo.descriptorCount = 1;
            writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writeInfo.pImageInfo = &imageInfo;
            writeInfo.pBufferInfo = nullptr;
            writeInfo.pTexelBufferView = nullptr;

            GR_VK_CALL(gpu->vkInterface(),
                       UpdateDescriptorSets(gpu->device(), 1, &writeInfo, 0, nullptr));
            commandBuffer->addResource(sampler);
            if (!fImmutableSamplers[i]) {
                sampler->unref(gpu);
            }
            commandBuffer->addResource(samplerBindings[i].fTexture->textureView());
            commandBuffer->addResource(samplerBindings[i].fTexture->resource());
        }

        commandBuffer->bindDescriptorSets(gpu, this, fPipelineLayout, samplerDSIdx, 1,
                                          &fDescriptorSets[samplerDSIdx], 0, nullptr);
        commandBuffer->addRecycledResource(fSamplerDescriptorSet);
>>>>>>> upstream-releases
    }
}

void set_uniform_descriptor_writes(VkWriteDescriptorSet* descriptorWrite,
                                   VkDescriptorBufferInfo* bufferInfo,
                                   const GrVkUniformBuffer* buffer,
                                   VkDescriptorSet descriptorSet,
                                   uint32_t binding) {

    memset(bufferInfo, 0, sizeof(VkDescriptorBufferInfo));
    bufferInfo->buffer = buffer->buffer();
    bufferInfo->offset = buffer->offset();
    bufferInfo->range = buffer->size();

    memset(descriptorWrite, 0, sizeof(VkWriteDescriptorSet));
    descriptorWrite->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite->pNext = nullptr;
    descriptorWrite->dstSet = descriptorSet;
    descriptorWrite->dstBinding = binding;
    descriptorWrite->dstArrayElement = 0;
    descriptorWrite->descriptorCount = 1;
    descriptorWrite->descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite->pImageInfo = nullptr;
    descriptorWrite->pBufferInfo = bufferInfo;
    descriptorWrite->pTexelBufferView = nullptr;
}

void GrVkPipelineState::writeUniformBuffers(const GrVkGpu* gpu) {
    VkWriteDescriptorSet descriptorWrites[3];
    VkDescriptorBufferInfo bufferInfos[3];

    uint32_t writeCount = 0;

    // Geometry Uniform Buffer
    if (fGeometryUniformBuffer.get()) {
        set_uniform_descriptor_writes(&descriptorWrites[writeCount],
                                      &bufferInfos[writeCount],
                                      fGeometryUniformBuffer.get(),
                                      fDescriptorSets[GrVkUniformHandler::kUniformBufferDescSet],
                                      GrVkUniformHandler::kGeometryBinding);
        ++writeCount;
    }

    // Fragment Uniform Buffer
    if (fFragmentUniformBuffer.get()) {
        set_uniform_descriptor_writes(&descriptorWrites[writeCount],
                                      &bufferInfos[writeCount],
                                      fFragmentUniformBuffer.get(),
                                      fDescriptorSets[GrVkUniformHandler::kUniformBufferDescSet],
                                      GrVkUniformHandler::kFragBinding);
        ++writeCount;
    }

    if (writeCount) {
        GR_VK_CALL(gpu->vkInterface(), UpdateDescriptorSets(gpu->device(),
                                                            writeCount,
                                                            descriptorWrites,
                                                            0, nullptr));
    }
}

<<<<<<< HEAD
void GrVkPipelineState::setRenderTargetState(const GrRenderTargetProxy* proxy) {
    GrRenderTarget* rt = proxy->peekRenderTarget();
||||||| merged common ancestors
void GrVkPipelineState::writeSamplers(
        GrVkGpu* gpu,
        const SkTArray<const GrResourceIOProcessor::TextureSampler*>& textureBindings,
        bool allowSRGBInputs) {
    SkASSERT(fNumSamplers == textureBindings.count());

    for (int i = 0; i < textureBindings.count(); ++i) {
        GrSamplerState state = textureBindings[i]->samplerState();

        GrVkTexture* texture = static_cast<GrVkTexture*>(textureBindings[i]->peekTexture());

        fSamplers.push(gpu->resourceProvider().findOrCreateCompatibleSampler(
                state, texture->texturePriv().maxMipMapLevel()));

        const GrVkResource* textureResource = texture->resource();
        textureResource->ref();
        fTextures.push(textureResource);

        const GrVkImageView* textureView = texture->textureView(allowSRGBInputs);
        textureView->ref();
        fTextureViews.push(textureView);

        VkDescriptorImageInfo imageInfo;
        memset(&imageInfo, 0, sizeof(VkDescriptorImageInfo));
        imageInfo.sampler = fSamplers[i]->sampler();
        imageInfo.imageView = textureView->imageView();
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkWriteDescriptorSet writeInfo;
        memset(&writeInfo, 0, sizeof(VkWriteDescriptorSet));
        writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeInfo.pNext = nullptr;
        writeInfo.dstSet = fDescriptorSets[GrVkUniformHandler::kSamplerDescSet];
        writeInfo.dstBinding = i;
        writeInfo.dstArrayElement = 0;
        writeInfo.descriptorCount = 1;
        writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeInfo.pImageInfo = &imageInfo;
        writeInfo.pBufferInfo = nullptr;
        writeInfo.pTexelBufferView = nullptr;

        GR_VK_CALL(gpu->vkInterface(), UpdateDescriptorSets(gpu->device(),
                                                            1,
                                                            &writeInfo,
                                                            0,
                                                            nullptr));
    }
}

void GrVkPipelineState::writeTexelBuffers(
        GrVkGpu* gpu,
        const SkTArray<const GrResourceIOProcessor::BufferAccess*>& bufferAccesses) {
    SkASSERT(fNumTexelBuffers == bufferAccesses.count());

    for (int i = 0; i < bufferAccesses.count(); ++i) {
        GrPixelConfig config = bufferAccesses[i]->texelConfig();
        VkFormat format;
        SkAssertResult(GrPixelConfigToVkFormat(config, &format));

        GrVkTexelBuffer* buffer = static_cast<GrVkTexelBuffer*>(bufferAccesses[i]->buffer());

        const GrVkBufferView* bufferView = GrVkBufferView::Create(gpu, buffer->buffer(),
                                                                  format, buffer->offset(),
                                                                  buffer->size());
        fBufferViews.push(bufferView);

        const GrVkResource* bufferResource = buffer->resource();
        bufferResource->ref();
        fTexelBuffers.push(bufferResource);

        VkWriteDescriptorSet writeInfo;
        memset(&writeInfo, 0, sizeof(VkWriteDescriptorSet));
        writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeInfo.pNext = nullptr;
        writeInfo.dstSet = fDescriptorSets[GrVkUniformHandler::kTexelBufferDescSet];
        writeInfo.dstBinding = i;
        writeInfo.dstArrayElement = 0;
        writeInfo.descriptorCount = 1;
        writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
        writeInfo.pImageInfo = nullptr;
        writeInfo.pBufferInfo = nullptr;
        VkBufferView vkBufferView = bufferView->bufferView();
        writeInfo.pTexelBufferView = &vkBufferView;

        GR_VK_CALL(gpu->vkInterface(), UpdateDescriptorSets(gpu->device(),
                                                            1,
                                                            &writeInfo,
                                                            0,
                                                            nullptr));
    }
}

void GrVkPipelineState::setRenderTargetState(const GrRenderTargetProxy* proxy) {
    GrRenderTarget* rt = proxy->priv().peekRenderTarget();
=======
void GrVkPipelineState::setRenderTargetState(const GrRenderTarget* rt, GrSurfaceOrigin origin) {
>>>>>>> upstream-releases

    // Load the RT height uniform if it is needed to y-flip gl_FragCoord.
    if (fBuiltinUniformHandles.fRTHeightUni.isValid() &&
        fRenderTargetState.fRenderTargetSize.fHeight != rt->height()) {
        fDataManager.set1f(fBuiltinUniformHandles.fRTHeightUni, SkIntToScalar(rt->height()));
    }

    // set RT adjustment
    SkISize size;
    size.set(rt->width(), rt->height());
    SkASSERT(fBuiltinUniformHandles.fRTAdjustmentUni.isValid());
    if (fRenderTargetState.fRenderTargetOrigin != origin ||
        fRenderTargetState.fRenderTargetSize != size) {
        fRenderTargetState.fRenderTargetSize = size;
        fRenderTargetState.fRenderTargetOrigin = origin;

        float rtAdjustmentVec[4];
        fRenderTargetState.getRTAdjustmentVec(rtAdjustmentVec);
        fDataManager.set4fv(fBuiltinUniformHandles.fRTAdjustmentUni, 1, rtAdjustmentVec);
    }
}

void GrVkPipelineState::bindPipeline(const GrVkGpu* gpu, GrVkCommandBuffer* commandBuffer) {
    commandBuffer->bindPipeline(gpu, fPipeline);
}
