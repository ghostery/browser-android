/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrMtlRenderTarget_DEFINED
#define GrMtlRenderTarget_DEFINED

#include "GrRenderTarget.h"

#include "GrBackendSurface.h"

#import <Metal/Metal.h>

class GrMtlGpu;

class GrMtlRenderTarget: public GrRenderTarget {
public:
    static sk_sp<GrMtlRenderTarget> MakeWrappedRenderTarget(GrMtlGpu*,
                                                            const GrSurfaceDesc&,
                                                            id<MTLTexture>);

    ~GrMtlRenderTarget() override;

    // override of GrRenderTarget
    ResolveType getResolveType() const override {
        return kCantResolve_ResolveType;
#if 0 // TODO figure this once we support msaa
        if (this->numColorSamples() > 1) {
            return kCanResolve_ResolveType;
        }
        return kAutoResolves_ResolveType;
#endif
    }

    bool canAttemptStencilAttachment() const override {
        return true;
    }

    id<MTLTexture> mtlRenderTexture() const { return fRenderTexture; }

    GrBackendRenderTarget getBackendRenderTarget() const override;

    GrBackendFormat backendFormat() const override;

protected:
    GrMtlRenderTarget(GrMtlGpu* gpu,
                      const GrSurfaceDesc& desc,
                      id<MTLTexture> renderTexture);

    GrMtlGpu* getMtlGpu() const;

    void onAbandon() override;
    void onRelease() override;

    // This accounts for the texture's memory and any MSAA renderbuffer's memory.
    size_t onGpuMemorySize() const override {
        int numColorSamples = this->numColorSamples();
        // TODO: When used as render targets certain formats may actually have a larger size than
        // the base format size. Check to make sure we are reporting the correct value here.
        // The plus 1 is to account for the resolve texture or if not using msaa the RT itself
        if (numColorSamples > 1) {
            ++numColorSamples;
        }
        return GrSurface::ComputeSize(this->config(), this->width(), this->height(),
                                      numColorSamples, GrMipMapped::kNo);
    }

    id<MTLTexture> fRenderTexture;
    id<MTLTexture> fResolveTexture;

private:
    // Extra param to disambiguate from constructor used by subclasses.
    enum Wrapped { kWrapped };
    GrMtlRenderTarget(GrMtlGpu* gpu,
<<<<<<< HEAD
                      SkBudgeted,
                      const GrSurfaceDesc& desc,
||||||| merged common ancestors
                      const GrSurfaceDesc& desc,
                      SkBudgeted,
=======
                      const GrSurfaceDesc& desc,
>>>>>>> upstream-releases
                      id<MTLTexture> renderTexture,
<<<<<<< HEAD
                      id<MTLTexture> resolveTexture);

    GrMtlRenderTarget(GrMtlGpu* gpu,
                      SkBudgeted,
                      const GrSurfaceDesc& desc,
                      id<MTLTexture> renderTexture);

    static sk_sp<GrMtlRenderTarget> Make(GrMtlGpu*,
                                         SkBudgeted,
                                         const GrSurfaceDesc&,
                                         id<MTLTexture> renderTexture,
                                         bool isWrapped);
||||||| merged common ancestors
                      id<MTLTexture> resolveTexture);

    GrMtlRenderTarget(GrMtlGpu* gpu,
                      const GrSurfaceDesc& desc,
                      SkBudgeted,
                      id<MTLTexture> renderTexture);

    static sk_sp<GrMtlRenderTarget> Make(GrMtlGpu*,
                                         const GrSurfaceDesc&,
                                         SkBudgeted,
                                         id<MTLTexture> renderTexture,
                                         bool isWrapped);
=======
                      Wrapped);
>>>>>>> upstream-releases

    bool completeStencilAttachment() override;

    typedef GrRenderTarget INHERITED;
};


#endif

