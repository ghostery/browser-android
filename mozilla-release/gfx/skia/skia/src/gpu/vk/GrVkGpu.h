/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrVkGpu_DEFINED
#define GrVkGpu_DEFINED

#include "GrGpu.h"
<<<<<<< HEAD
#include "vk/GrVkBackendContext.h"
||||||| merged common ancestors
#include "GrGpuFactory.h"
#include "vk/GrVkBackendContext.h"
=======
>>>>>>> upstream-releases
#include "GrVkCaps.h"
#include "GrVkCopyManager.h"
#include "GrVkIndexBuffer.h"
#include "GrVkMemory.h"
#include "GrVkResourceProvider.h"
#include "GrVkSemaphore.h"
#include "GrVkVertexBuffer.h"
#include "GrVkUtil.h"
#include "vk/GrVkBackendContext.h"
#include "vk/GrVkTypes.h"

class GrPipeline;

class GrVkBufferImpl;
<<<<<<< HEAD
class GrVkGpuRTCommandBuffer;
class GrVkGpuTextureCommandBuffer;
class GrVkMemoryAllocator;
||||||| merged common ancestors
=======
class GrVkCommandPool;
class GrVkGpuRTCommandBuffer;
class GrVkGpuTextureCommandBuffer;
class GrVkMemoryAllocator;
>>>>>>> upstream-releases
class GrVkPipeline;
class GrVkPipelineState;
class GrVkPrimaryCommandBuffer;
class GrVkRenderPass;
class GrVkSecondaryCommandBuffer;
class GrVkTexture;
struct GrVkInterface;

namespace SkSL {
    class Compiler;
}

class GrVkGpu : public GrGpu {
public:
    static sk_sp<GrGpu> Make(const GrVkBackendContext&, const GrContextOptions&, GrContext*);

    ~GrVkGpu() override;

    void disconnect(DisconnectType) override;

    const GrVkInterface* vkInterface() const { return fInterface.get(); }
    const GrVkCaps& vkCaps() const { return *fVkCaps; }

    GrVkMemoryAllocator* memoryAllocator() const { return fMemoryAllocator.get(); }

    VkPhysicalDevice physicalDevice() const { return fPhysicalDevice; }
    VkDevice device() const { return fDevice; }
    VkQueue  queue() const { return fQueue; }
<<<<<<< HEAD
    uint32_t  queueIndex() const { return fQueueIndex; }
    VkCommandPool cmdPool() const { return fCmdPool; }
    VkPhysicalDeviceProperties physicalDeviceProperties() const {
||||||| merged common ancestors
    VkCommandPool cmdPool() const { return fCmdPool; }
    VkPhysicalDeviceProperties physicalDeviceProperties() const {
=======
    uint32_t  queueIndex() const { return fQueueIndex; }
    GrVkCommandPool* cmdPool() const { return fCmdPool; }
    const VkPhysicalDeviceProperties& physicalDeviceProperties() const {
>>>>>>> upstream-releases
        return fPhysDevProps;
    }
    const VkPhysicalDeviceMemoryProperties& physicalDeviceMemoryProperties() const {
        return fPhysDevMemProps;
    }

    GrVkResourceProvider& resourceProvider() { return fResourceProvider; }

    GrVkPrimaryCommandBuffer* currentCommandBuffer() { return fCurrentCmdBuffer; }

    enum SyncQueue {
        kForce_SyncQueue,
        kSkip_SyncQueue
    };

    void xferBarrier(GrRenderTarget*, GrXferBarrierType) override {}

#if GR_TEST_UTILS
    GrBackendTexture createTestingOnlyBackendTexture(const void* pixels, int w, int h,
                                                     GrColorType colorType, bool isRenderTarget,
                                                     GrMipMapped, size_t rowBytes = 0) override;
    bool isTestingOnlyBackendTexture(const GrBackendTexture&) const override;
    void deleteTestingOnlyBackendTexture(const GrBackendTexture&) override;

    GrBackendRenderTarget createTestingOnlyBackendRenderTarget(int w, int h, GrColorType) override;
    void deleteTestingOnlyBackendRenderTarget(const GrBackendRenderTarget&) override;

    void testingOnly_flushGpuAndSync() override;
#endif

    GrStencilAttachment* createStencilAttachmentForRenderTarget(const GrRenderTarget*,
                                                                int width,
                                                                int height) override;

<<<<<<< HEAD
    GrGpuRTCommandBuffer* getCommandBuffer(
            GrRenderTarget*, GrSurfaceOrigin,
||||||| merged common ancestors
    void clearStencil(GrRenderTarget* target, int clearValue) override;

    GrGpuRTCommandBuffer* createCommandBuffer(
            GrRenderTarget*, GrSurfaceOrigin,
=======
    GrGpuRTCommandBuffer* getCommandBuffer(
            GrRenderTarget*, GrSurfaceOrigin, const SkRect&,
>>>>>>> upstream-releases
            const GrGpuRTCommandBuffer::LoadAndStoreInfo&,
            const GrGpuRTCommandBuffer::StencilLoadAndStoreInfo&) override;

    GrGpuTextureCommandBuffer* getCommandBuffer(GrTexture*, GrSurfaceOrigin) override;


    void addMemoryBarrier(VkPipelineStageFlags srcStageMask,
                          VkPipelineStageFlags dstStageMask,
                          bool byRegion,
                          VkMemoryBarrier* barrier) const;
    void addBufferMemoryBarrier(const GrVkResource*,
                                VkPipelineStageFlags srcStageMask,
                                VkPipelineStageFlags dstStageMask,
                                bool byRegion,
                                VkBufferMemoryBarrier* barrier) const;
    void addImageMemoryBarrier(const GrVkResource*,
                               VkPipelineStageFlags srcStageMask,
                               VkPipelineStageFlags dstStageMask,
                               bool byRegion,
                               VkImageMemoryBarrier* barrier) const;

    SkSL::Compiler* shaderCompiler() const {
        return fCompiler;
    }

<<<<<<< HEAD
    bool onRegenerateMipMapLevels(GrTexture* tex) override;

||||||| merged common ancestors
=======
    bool onRegenerateMipMapLevels(GrTexture* tex) override;

    void resolveRenderTargetNoFlush(GrRenderTarget* target) {
        this->internalResolveRenderTarget(target, false);
    }

>>>>>>> upstream-releases
    void onResolveRenderTarget(GrRenderTarget* target) override {
        // This resolve is called when we are preparing an msaa surface for external I/O. It is
        // called after flushing, so we need to make sure we submit the command buffer after doing
        // the resolve so that the resolve actually happens.
        this->internalResolveRenderTarget(target, true);
    }

    void submitSecondaryCommandBuffer(const SkTArray<GrVkSecondaryCommandBuffer*>&,
                                      const GrVkRenderPass*,
                                      const VkClearValue* colorClear,
                                      GrVkRenderTarget*, GrSurfaceOrigin,
                                      const SkIRect& bounds);

    void submit(GrGpuCommandBuffer*) override;

    GrFence SK_WARN_UNUSED_RESULT insertFence() override;
    bool waitFence(GrFence, uint64_t timeout) override;
    void deleteFence(GrFence) const override;

    sk_sp<GrSemaphore> SK_WARN_UNUSED_RESULT makeSemaphore(bool isOwned) override;
    sk_sp<GrSemaphore> wrapBackendSemaphore(const GrBackendSemaphore& semaphore,
                                            GrResourceProvider::SemaphoreWrapType wrapType,
                                            GrWrapOwnership ownership) override;
    void insertSemaphore(sk_sp<GrSemaphore> semaphore) override;
    void waitSemaphore(sk_sp<GrSemaphore> semaphore) override;

    // These match the definitions in SkDrawable, from whence they came
    typedef void* SubmitContext;
    typedef void (*SubmitProc)(SubmitContext submitContext);

    // Adds an SkDrawable::GpuDrawHandler that we will delete the next time we submit the primary
    // command buffer to the gpu.
    void addDrawable(std::unique_ptr<SkDrawable::GpuDrawHandler> drawable);

<<<<<<< HEAD
||||||| merged common ancestors
    void generateMipmap(GrVkTexture* tex, GrSurfaceOrigin texOrigin);

=======
    sk_sp<GrSemaphore> prepareTextureForCrossContextUsage(GrTexture*) override;

>>>>>>> upstream-releases
    void copyBuffer(GrVkBuffer* srcBuffer, GrVkBuffer* dstBuffer, VkDeviceSize srcOffset,
                    VkDeviceSize dstOffset, VkDeviceSize size);
    bool updateBuffer(GrVkBuffer* buffer, const void* src, VkDeviceSize offset, VkDeviceSize size);

<<<<<<< HEAD
||||||| merged common ancestors
    // Heaps
    enum Heap {
        kLinearImage_Heap = 0,
        // We separate out small (i.e., <= 16K) images to reduce fragmentation
        // in the main heap.
        kOptimalImage_Heap,
        kSmallOptimalImage_Heap,
        // We have separate vertex and image heaps, because it's possible that
        // a given Vulkan driver may allocate them separately.
        kVertexBuffer_Heap,
        kIndexBuffer_Heap,
        kUniformBuffer_Heap,
        kTexelBuffer_Heap,
        kCopyReadBuffer_Heap,
        kCopyWriteBuffer_Heap,

        kLastHeap = kCopyWriteBuffer_Heap
    };
    static const int kHeapCount = kLastHeap + 1;

    GrVkHeap* getHeap(Heap heap) const { return fHeaps[heap].get(); }

=======
    uint32_t getExtraSamplerKeyForProgram(const GrSamplerState&,
                                          const GrBackendFormat& format) override;

    enum PersistentCacheKeyType : uint32_t {
        kShader_PersistentCacheKeyType = 0,
        kPipelineCache_PersistentCacheKeyType = 1,
    };

    void storeVkPipelineCacheData() override;

>>>>>>> upstream-releases
private:
<<<<<<< HEAD
    GrVkGpu(GrContext*, const GrContextOptions&, const GrVkBackendContext&,
            sk_sp<const GrVkInterface>);
||||||| merged common ancestors
    GrVkGpu(GrContext*, const GrContextOptions&, sk_sp<const GrVkBackendContext> backendContext);
=======
    GrVkGpu(GrContext*, const GrContextOptions&, const GrVkBackendContext&,
            sk_sp<const GrVkInterface>, uint32_t instanceVersion, uint32_t physicalDeviceVersion);
>>>>>>> upstream-releases

    void onResetContext(uint32_t resetBits) override {}

    void destroyResources();

    sk_sp<GrTexture> onCreateTexture(const GrSurfaceDesc&, SkBudgeted, const GrMipLevel[],
                                     int mipLevelCount) override;

    sk_sp<GrTexture> onWrapBackendTexture(const GrBackendTexture&, GrWrapOwnership, GrWrapCacheable,
                                          GrIOType) override;
    sk_sp<GrTexture> onWrapRenderableBackendTexture(const GrBackendTexture&,
                                                    int sampleCnt,
                                                    GrWrapOwnership,
                                                    GrWrapCacheable) override;
    sk_sp<GrRenderTarget> onWrapBackendRenderTarget(const GrBackendRenderTarget&) override;

    sk_sp<GrRenderTarget> onWrapBackendTextureAsRenderTarget(const GrBackendTexture&,
                                                             int sampleCnt) override;

<<<<<<< HEAD
    GrBuffer* onCreateBuffer(size_t size, GrBufferType type, GrAccessPattern,
                             const void* data) override;

    bool onReadPixels(GrSurface* surface, int left, int top, int width, int height, GrColorType,
                      void* buffer, size_t rowBytes) override;
||||||| merged common ancestors
    GrBuffer* onCreateBuffer(size_t size, GrBufferType type, GrAccessPattern,
                             const void* data) override;

    bool onGetReadPixelsInfo(GrSurface*, GrSurfaceOrigin, int width, int height, size_t rowBytes,
                             GrColorType, DrawPreference*, ReadPixelTempDrawInfo*) override;

    bool onGetWritePixelsInfo(GrSurface*, GrSurfaceOrigin, int width, int height, GrColorType,
                              DrawPreference*, WritePixelTempDrawInfo*) override;
=======
    sk_sp<GrRenderTarget> onWrapVulkanSecondaryCBAsRenderTarget(const SkImageInfo&,
                                                                const GrVkDrawableInfo&) override;

    sk_sp<GrGpuBuffer> onCreateBuffer(size_t size, GrGpuBufferType type, GrAccessPattern,
                                      const void* data) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool onWritePixels(GrSurface* surface, int left, int top, int width, int height, GrColorType,
                       const GrMipLevel texels[], int mipLevelCount) override;
||||||| merged common ancestors
    bool onReadPixels(GrSurface* surface, GrSurfaceOrigin, int left, int top, int width, int height,
                      GrColorType, void* buffer, size_t rowBytes) override;

    bool onWritePixels(GrSurface* surface, GrSurfaceOrigin, int left, int top, int width,
                       int height, GrColorType, const GrMipLevel texels[],
                       int mipLevelCount) override;
=======
    bool onReadPixels(GrSurface* surface, int left, int top, int width, int height, GrColorType,
                      void* buffer, size_t rowBytes) override;

    bool onWritePixels(GrSurface* surface, int left, int top, int width, int height, GrColorType,
                       const GrMipLevel texels[], int mipLevelCount) override;
>>>>>>> upstream-releases

    bool onTransferPixels(GrTexture*, int left, int top, int width, int height, GrColorType,
                          GrGpuBuffer* transferBuffer, size_t offset, size_t rowBytes) override;

    bool onCopySurface(GrSurface* dst, GrSurfaceOrigin dstOrigin, GrSurface* src,
                       GrSurfaceOrigin srcOrigin, const SkIRect& srcRect,
                       const SkIPoint& dstPoint, bool canDiscardOutsideDstRect) override;

    void onFinishFlush(GrSurfaceProxy*, SkSurface::BackendSurfaceAccess access,
                       SkSurface::FlushFlags flags, bool insertedSemaphores) override;

    // Ends and submits the current command buffer to the queue and then creates a new command
    // buffer and begins it. If sync is set to kForce_SyncQueue, the function will wait for all
    // work in the queue to finish before returning. If this GrVkGpu object has any semaphores in
    // fSemaphoreToSignal, we will add those signal semaphores to the submission of this command
    // buffer. If this GrVkGpu object has any semaphores in fSemaphoresToWaitOn, we will add those
    // wait semaphores to the submission of this command buffer.
    void submitCommandBuffer(SyncQueue sync);

    void internalResolveRenderTarget(GrRenderTarget*, bool requiresSubmit);

    void copySurfaceAsCopyImage(GrSurface* dst, GrSurfaceOrigin dstOrigin,
                                GrSurface* src, GrSurfaceOrigin srcOrigin,
                                GrVkImage* dstImage, GrVkImage* srcImage,
                                const SkIRect& srcRect,
                                const SkIPoint& dstPoint);

    void copySurfaceAsBlit(GrSurface* dst, GrSurfaceOrigin dstOrigin,
                           GrSurface* src, GrSurfaceOrigin srcOrigin,
                           GrVkImage* dstImage, GrVkImage* srcImage,
                           const SkIRect& srcRect,
                           const SkIPoint& dstPoint);

    void copySurfaceAsResolve(GrSurface* dst, GrSurfaceOrigin dstOrigin,
                              GrSurface* src, GrSurfaceOrigin srcOrigin,
                              const SkIRect& srcRect,
                              const SkIPoint& dstPoint);

    // helpers for onCreateTexture and writeTexturePixels
<<<<<<< HEAD
    bool uploadTexDataLinear(GrVkTexture* tex, int left, int top, int width, int height,
                             GrColorType colorType, const void* data, size_t rowBytes);
    bool uploadTexDataOptimal(GrVkTexture* tex, int left, int top, int width, int height,
                              GrColorType colorType, const GrMipLevel texels[], int mipLevelCount);

||||||| merged common ancestors
    bool uploadTexDataLinear(GrVkTexture* tex, GrSurfaceOrigin texOrigin, int left, int top,
                             int width, int height, GrColorType colorType, const void* data,
                             size_t rowBytes);
    bool uploadTexDataOptimal(GrVkTexture* tex, GrSurfaceOrigin texOrigin, int left, int top,
                              int width, int height, GrColorType colorType,
                              const GrMipLevel texels[], int mipLevelCount);

=======
    bool uploadTexDataLinear(GrVkTexture* tex, int left, int top, int width, int height,
                             GrColorType colorType, const void* data, size_t rowBytes);
    bool uploadTexDataOptimal(GrVkTexture* tex, int left, int top, int width, int height,
                              GrColorType colorType, const GrMipLevel texels[], int mipLevelCount);
    bool uploadTexDataCompressed(GrVkTexture* tex, int left, int top, int width, int height,
                                 GrColorType dataColorType, const GrMipLevel texels[],
                                 int mipLevelCount);
>>>>>>> upstream-releases
    void resolveImage(GrSurface* dst, GrVkRenderTarget* src, const SkIRect& srcRect,
                      const SkIPoint& dstPoint);

<<<<<<< HEAD
#if GR_TEST_UTILS
    bool createTestingOnlyVkImage(GrPixelConfig config, int w, int h, bool texturable,
                                  bool renderable, GrMipMapped mipMapped, const void* srcData,
                                  size_t srcRowBytes, GrVkImageInfo* info);
#endif
||||||| merged common ancestors
    sk_sp<const GrVkBackendContext> fBackendContext;
    sk_sp<GrVkCaps>                 fVkCaps;

    // These Vulkan objects are provided by the client, and also stored in fBackendContext.
    // They're copied here for convenient access.
    VkDevice                                     fDevice;
    VkQueue                                      fQueue;    // Must be Graphics queue
=======
#if GR_TEST_UTILS
    bool createTestingOnlyVkImage(GrPixelConfig config, int w, int h, bool texturable,
                                  bool renderable, GrMipMapped mipMapped, const void* srcData,
                                  size_t srcRowBytes, GrVkImageInfo* info);
#endif

    sk_sp<const GrVkInterface>                            fInterface;
    sk_sp<GrVkMemoryAllocator>                            fMemoryAllocator;
    sk_sp<GrVkCaps>                                       fVkCaps;

    VkInstance                                            fInstance;
    VkPhysicalDevice                                      fPhysicalDevice;
    VkDevice                                              fDevice;
    VkQueue                                               fQueue;    // Must be Graphics queue
    uint32_t                                              fQueueIndex;
>>>>>>> upstream-releases

<<<<<<< HEAD
    sk_sp<const GrVkInterface>             fInterface;
    sk_sp<GrVkMemoryAllocator>             fMemoryAllocator;
    sk_sp<GrVkCaps>                        fVkCaps;
||||||| merged common ancestors
    // Created by GrVkGpu
    GrVkResourceProvider                         fResourceProvider;
    VkCommandPool                                fCmdPool;
=======
    // Created by GrVkGpu
    GrVkResourceProvider                                  fResourceProvider;
>>>>>>> upstream-releases

<<<<<<< HEAD
    VkInstance                             fInstance;
    VkPhysicalDevice                       fPhysicalDevice;
    VkDevice                               fDevice;
    VkQueue                                fQueue;    // Must be Graphics queue
    uint32_t                               fQueueIndex;
||||||| merged common ancestors
    GrVkPrimaryCommandBuffer*                    fCurrentCmdBuffer;
=======
    GrVkCommandPool*                                      fCmdPool;
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Created by GrVkGpu
    GrVkResourceProvider                   fResourceProvider;
    VkCommandPool                          fCmdPool;
||||||| merged common ancestors
    SkSTArray<1, GrVkSemaphore::Resource*>       fSemaphoresToWaitOn;
    SkSTArray<1, GrVkSemaphore::Resource*>       fSemaphoresToSignal;
=======
    // just a raw pointer; object's lifespan is managed by fCmdPool
    GrVkPrimaryCommandBuffer*                             fCurrentCmdBuffer;
>>>>>>> upstream-releases

<<<<<<< HEAD
    GrVkPrimaryCommandBuffer*              fCurrentCmdBuffer;
||||||| merged common ancestors
    VkPhysicalDeviceProperties                   fPhysDevProps;
    VkPhysicalDeviceMemoryProperties             fPhysDevMemProps;
=======
    SkSTArray<1, GrVkSemaphore::Resource*>                fSemaphoresToWaitOn;
    SkSTArray<1, GrVkSemaphore::Resource*>                fSemaphoresToSignal;
>>>>>>> upstream-releases

<<<<<<< HEAD
    SkSTArray<1, GrVkSemaphore::Resource*> fSemaphoresToWaitOn;
    SkSTArray<1, GrVkSemaphore::Resource*> fSemaphoresToSignal;
||||||| merged common ancestors
    std::unique_ptr<GrVkHeap>                    fHeaps[kHeapCount];
=======
    SkTArray<std::unique_ptr<SkDrawable::GpuDrawHandler>> fDrawables;
>>>>>>> upstream-releases

<<<<<<< HEAD
    VkPhysicalDeviceProperties             fPhysDevProps;
    VkPhysicalDeviceMemoryProperties       fPhysDevMemProps;
||||||| merged common ancestors
    GrVkCopyManager                              fCopyManager;
=======
    VkPhysicalDeviceProperties                            fPhysDevProps;
    VkPhysicalDeviceMemoryProperties                      fPhysDevMemProps;
>>>>>>> upstream-releases

<<<<<<< HEAD
    GrVkCopyManager                        fCopyManager;
||||||| merged common ancestors
#ifdef SK_ENABLE_VK_LAYERS
    // For reporting validation layer errors
    VkDebugReportCallbackEXT               fCallback;
#endif
=======
    GrVkCopyManager                                       fCopyManager;
>>>>>>> upstream-releases

    // compiler used for compiling sksl into spirv. We only want to create the compiler once since
    // there is significant overhead to the first compile of any compiler.
<<<<<<< HEAD
    SkSL::Compiler*                        fCompiler;
||||||| merged common ancestors
    SkSL::Compiler* fCompiler;
=======
    SkSL::Compiler*                                       fCompiler;
>>>>>>> upstream-releases

    // We need a bool to track whether or not we've already disconnected all the gpu resources from
    // vulkan context.
<<<<<<< HEAD
    bool                                   fDisconnected;

    std::unique_ptr<GrVkGpuRTCommandBuffer>      fCachedRTCommandBuffer;
    std::unique_ptr<GrVkGpuTextureCommandBuffer> fCachedTexCommandBuffer;
||||||| merged common ancestors
    bool fDisconnected;
=======
    bool                                                  fDisconnected;

    std::unique_ptr<GrVkGpuRTCommandBuffer>               fCachedRTCommandBuffer;
    std::unique_ptr<GrVkGpuTextureCommandBuffer>          fCachedTexCommandBuffer;
>>>>>>> upstream-releases

    typedef GrGpu INHERITED;
};

#endif
