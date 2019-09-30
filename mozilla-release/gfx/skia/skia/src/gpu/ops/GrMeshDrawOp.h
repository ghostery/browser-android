/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrMeshDrawOp_DEFINED
#define GrMeshDrawOp_DEFINED

#include "GrAppliedClip.h"
#include "GrDrawOp.h"
#include "GrGeometryProcessor.h"
#include "GrMesh.h"
<<<<<<< HEAD
||||||| merged common ancestors
#include "GrPendingProgramElement.h"

#include "SkTLList.h"
=======
#include <type_traits>
>>>>>>> upstream-releases

class GrAtlasManager;
class GrCaps;
<<<<<<< HEAD
class GrGlyphCache;
||||||| merged common ancestors
=======
class GrStrikeCache;
>>>>>>> upstream-releases
class GrOpFlushState;

/**
 * Base class for mesh-drawing GrDrawOps.
 */
class GrMeshDrawOp : public GrDrawOp {
public:
    /** Abstract interface that represents a destination for a GrMeshDrawOp. */
    class Target;

protected:
    GrMeshDrawOp(uint32_t classID);

    /** Helper for rendering repeating meshes using a patterned index buffer. This class creates the
        space for the vertices and flushes the draws to the GrMeshDrawOp::Target. */
    class PatternHelper {
    public:
<<<<<<< HEAD
        PatternHelper(Target*, GrPrimitiveType, size_t vertexStride, const GrBuffer*,
                      int verticesPerRepetition, int indicesPerRepetition, int repeatCount);

        /** Called to issue draws to the GrMeshDrawOp::Target.*/
        void recordDraw(Target*, sk_sp<const GrGeometryProcessor>, const GrPipeline*,
                        const GrPipeline::FixedDynamicState*) const;
||||||| merged common ancestors
        PatternHelper(GrPrimitiveType primitiveType) : fMesh(primitiveType) {}
        /** Returns the allocated storage for the vertices. The caller should populate the vertices
            before calling recordDraws(). */
        void* init(Target*, size_t vertexStride, const GrBuffer*, int verticesPerRepetition,
                   int indicesPerRepetition, int repeatCount);
=======
        PatternHelper(Target*, GrPrimitiveType, size_t vertexStride,
                      sk_sp<const GrBuffer> indexBuffer, int verticesPerRepetition,
                      int indicesPerRepetition, int repeatCount);

        /** Called to issue draws to the GrMeshDrawOp::Target.*/
        void recordDraw(Target*, sk_sp<const GrGeometryProcessor>) const;
        void recordDraw(Target*, sk_sp<const GrGeometryProcessor>,
                        const GrPipeline::FixedDynamicState*) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
        void* vertices() const { return fVertices; }

    protected:
        PatternHelper() = default;
        void init(Target*, GrPrimitiveType, size_t vertexStride, const GrBuffer*,
                  int verticesPerRepetition, int indicesPerRepetition, int repeatCount);
||||||| merged common ancestors
        /** Call after init() to issue draws to the GrMeshDrawOp::Target.*/
        void recordDraw(Target*, const GrGeometryProcessor*, const GrPipeline*);
=======
        void* vertices() const { return fVertices; }

    protected:
        PatternHelper() = default;
        void init(Target*, GrPrimitiveType, size_t vertexStride, sk_sp<const GrBuffer> indexBuffer,
                  int verticesPerRepetition, int indicesPerRepetition, int repeatCount);
>>>>>>> upstream-releases

    private:
        void* fVertices = nullptr;
        GrMesh* fMesh = nullptr;
    };

    static const int kVerticesPerQuad = 4;
    static const int kIndicesPerQuad = 6;

    /** A specialization of InstanceHelper for quad rendering. */
    class QuadHelper : private PatternHelper {
    public:
        QuadHelper() = delete;
        QuadHelper(Target* target, size_t vertexStride, int quadsToDraw);

        using PatternHelper::recordDraw;
        using PatternHelper::vertices;

    private:
        typedef PatternHelper INHERITED;
    };

private:
    void onPrepare(GrOpFlushState* state) final;
    virtual void onPrepareDraws(Target*) = 0;
    typedef GrDrawOp INHERITED;
};

class GrMeshDrawOp::Target {
public:
    virtual ~Target() {}

    /** Adds a draw of a mesh. */
<<<<<<< HEAD
    virtual void draw(sk_sp<const GrGeometryProcessor>,
                      const GrPipeline*,
                      const GrPipeline::FixedDynamicState*,
                      const GrPipeline::DynamicStateArrays*,
                      const GrMesh[],
                      int meshCount) = 0;
    /** Helper for drawing a single GrMesh. */
    void draw(sk_sp<const GrGeometryProcessor> gp,
              const GrPipeline* pipeline,
              const GrPipeline::FixedDynamicState* fixedDynamicState,
              const GrMesh* mesh) {
        this->draw(std::move(gp), pipeline, fixedDynamicState, nullptr, mesh, 1);
    }
||||||| merged common ancestors
    virtual void draw(const GrGeometryProcessor*, const GrPipeline*, const GrMesh&) = 0;
=======
    virtual void recordDraw(
            sk_sp<const GrGeometryProcessor>, const GrMesh[], int meshCnt,
            const GrPipeline::FixedDynamicState*, const GrPipeline::DynamicStateArrays*) = 0;

    /**
     * Helper for drawing GrMesh(es) with zero primProc textures and no dynamic state besides the
     * scissor clip.
     */
    void recordDraw(sk_sp<const GrGeometryProcessor> gp, const GrMesh meshes[], int meshCnt = 1) {
        static constexpr int kZeroPrimProcTextures = 0;
        auto fixedDynamicState = this->makeFixedDynamicState(kZeroPrimProcTextures);
        this->recordDraw(std::move(gp), meshes, meshCnt, fixedDynamicState, nullptr);
    }
>>>>>>> upstream-releases

    /**
     * Makes space for vertex data. The returned pointer is the location where vertex data
     * should be written. On return the buffer that will hold the data as well as an offset into
     * the buffer (in 'vertexSize' units) where the data will be placed.
     */
    virtual void* makeVertexSpace(size_t vertexSize, int vertexCount, sk_sp<const GrBuffer>*,
                                  int* startVertex) = 0;

    /**
     * Makes space for index data. The returned pointer is the location where index data
     * should be written. On return the buffer that will hold the data as well as an offset into
     * the buffer (in uint16_t units) where the data will be placed.
     */
    virtual uint16_t* makeIndexSpace(int indexCount, sk_sp<const GrBuffer>*, int* startIndex) = 0;

    /**
     * This is similar to makeVertexSpace. It allows the caller to use up to 'actualVertexCount'
     * vertices in the returned pointer, which may exceed 'minVertexCount'.
     * 'fallbackVertexCount' is the maximum number of vertices that should be allocated if a new
     * buffer is allocated on behalf of this request.
     */
    virtual void* makeVertexSpaceAtLeast(size_t vertexSize, int minVertexCount,
                                         int fallbackVertexCount, sk_sp<const GrBuffer>*,
                                         int* startVertex, int* actualVertexCount) = 0;

    /**
     * This is similar to makeIndexSpace. It allows the caller to use up to 'actualIndexCount'
     * indices in the returned pointer, which may exceed 'minIndexCount'.
     * 'fallbackIndexCount' is the maximum number of indices that should be allocated if a new
     * buffer is allocated on behalf of this request.
     */
    virtual uint16_t* makeIndexSpaceAtLeast(int minIndexCount, int fallbackIndexCount,
                                            sk_sp<const GrBuffer>*, int* startIndex,
                                            int* actualIndexCount) = 0;

    /** Helpers for ops which over-allocate and then return excess data to the pool. */
    virtual void putBackIndices(int indices) = 0;
    virtual void putBackVertices(int vertices, size_t vertexStride) = 0;

    GrMesh* allocMesh(GrPrimitiveType primitiveType) {
        return this->allocator()->make<GrMesh>(primitiveType);
    }

<<<<<<< HEAD
    GrMesh* allocMesh(GrPrimitiveType primitiveType) {
        return this->pipelineArena()->make<GrMesh>(primitiveType);
    }

    GrMesh* allocMeshes(int n) { return this->pipelineArena()->makeArray<GrMesh>(n); }

    GrPipeline::FixedDynamicState* allocFixedDynamicState(const SkIRect& rect,
                                                          int numPrimitiveProcessorTextures = 0);

    GrPipeline::DynamicStateArrays* allocDynamicStateArrays(int numMeshes,
                                                            int numPrimitiveProcessorTextures,
                                                            bool allocScissors);

    GrTextureProxy** allocPrimitiveProcessorTextureArray(int n) {
        SkASSERT(n > 0);
        return this->pipelineArena()->makeArrayDefault<GrTextureProxy*>(n);
    }

    // Once we have C++17 structured bindings make this just be a tuple because then we can do:
    //      auto [pipeline, fixedDynamicState] = target->makePipeline(...);
    // in addition to:
    //      std::tie(flushInfo.fPipeline, flushInfo.fFixedState) = target->makePipeline(...);
    struct PipelineAndFixedDynamicState {
        const GrPipeline* fPipeline;
        GrPipeline::FixedDynamicState* fFixedDynamicState;
    };

    /**
     * Helper that makes a pipeline targeting the op's render target that incorporates the op's
     * GrAppliedClip and uses a fixed dynamic state.
     */
    PipelineAndFixedDynamicState makePipeline(uint32_t pipelineFlags, GrProcessorSet&&,
                                              GrAppliedClip&&,
                                              int numPrimitiveProcessorTextures = 0);
||||||| merged common ancestors
    /**
     * Helper that makes a pipeline targeting the op's render target that incorporates the op's
     * GrAppliedClip.
     */
    GrPipeline* makePipeline(uint32_t pipelineFlags, GrProcessorSet&& processorSet,
                             GrAppliedClip&& clip) {
        GrPipeline::InitArgs pipelineArgs;
        pipelineArgs.fFlags = pipelineFlags;
        pipelineArgs.fProxy = this->proxy();
        pipelineArgs.fDstProxy = this->dstProxy();
        pipelineArgs.fCaps = &this->caps();
        pipelineArgs.fResourceProvider = this->resourceProvider();
        return this->allocPipeline(pipelineArgs, std::move(processorSet), std::move(clip));
    }
=======
    GrMesh* allocMeshes(int n) { return this->allocator()->makeArray<GrMesh>(n); }

    GrPipeline::DynamicStateArrays* allocDynamicStateArrays(int numMeshes,
                                                            int numPrimitiveProcessorTextures,
                                                            bool allocScissors);

    GrPipeline::FixedDynamicState* makeFixedDynamicState(int numPrimitiveProcessorTextures);
>>>>>>> upstream-releases

    virtual GrRenderTargetProxy* proxy() const = 0;

    virtual const GrAppliedClip* appliedClip() = 0;
    virtual GrAppliedClip detachAppliedClip() = 0;

    virtual const GrXferProcessor::DstProxy& dstProxy() const = 0;

    virtual GrResourceProvider* resourceProvider() const = 0;
<<<<<<< HEAD
    uint32_t contextUniqueID() const { return this->resourceProvider()->contextUniqueID(); }

    virtual GrGlyphCache* glyphCache() const = 0;
    virtual GrAtlasManager* atlasManager() const = 0;
||||||| merged common ancestors
=======
    uint32_t contextUniqueID() const { return this->resourceProvider()->contextUniqueID(); }

    virtual GrStrikeCache* glyphCache() const = 0;
    virtual GrAtlasManager* atlasManager() const = 0;
>>>>>>> upstream-releases

    virtual const GrCaps& caps() const = 0;

    virtual GrDeferredUploadTarget* deferredUploadTarget() = 0;

private:
    virtual SkArenaAlloc* allocator() = 0;
};

#endif
