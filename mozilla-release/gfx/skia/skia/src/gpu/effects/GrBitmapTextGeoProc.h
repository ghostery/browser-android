/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrBitmapTextGeoProc_DEFINED
#define GrBitmapTextGeoProc_DEFINED

#include "GrProcessor.h"
#include "GrGeometryProcessor.h"

class GrGLBitmapTextGeoProc;
class GrInvariantOutput;

/**
 * The output color of this effect is a modulation of the input color and a sample from a texture.
 * It allows explicit specification of the filtering and wrap modes (GrSamplerState). The input
 * coords are a custom attribute.
 */
class GrBitmapTextGeoProc : public GrGeometryProcessor {
public:
    static constexpr int kMaxTextures = 4;

<<<<<<< HEAD
    static sk_sp<GrGeometryProcessor> Make(const GrShaderCaps& caps, GrColor color,
                                           const sk_sp<GrTextureProxy>* proxies,
                                           int numActiveProxies,
||||||| merged common ancestors
    static sk_sp<GrGeometryProcessor> Make(GrColor color,
                                           const sk_sp<GrTextureProxy> proxies[kMaxTextures],
=======
    static sk_sp<GrGeometryProcessor> Make(const GrShaderCaps& caps,
                                           const SkPMColor4f& color, bool wideColor,
                                           const sk_sp<GrTextureProxy>* proxies,
                                           int numActiveProxies,
>>>>>>> upstream-releases
                                           const GrSamplerState& p, GrMaskFormat format,
                                           const SkMatrix& localMatrix, bool usesW) {
        return sk_sp<GrGeometryProcessor>(
<<<<<<< HEAD
            new GrBitmapTextGeoProc(caps, color, proxies, numActiveProxies, p, format,
                                    localMatrix, usesW));
||||||| merged common ancestors
            new GrBitmapTextGeoProc(color, proxies, p, format,
                                    localMatrix, usesLocalCoords));
=======
            new GrBitmapTextGeoProc(caps, color, wideColor, proxies, numActiveProxies, p, format,
                                    localMatrix, usesW));
>>>>>>> upstream-releases
    }

    ~GrBitmapTextGeoProc() override {}

    const char* name() const override { return "Texture"; }

    const Attribute& inPosition() const { return fInPosition; }
    const Attribute& inColor() const { return fInColor; }
    const Attribute& inTextureCoords() const { return fInTextureCoords; }
    GrMaskFormat maskFormat() const { return fMaskFormat; }
<<<<<<< HEAD
    GrColor color() const { return fColor; }
    bool hasVertexColor() const { return fInColor.isInitialized(); }
||||||| merged common ancestors
    GrColor color() const { return fColor; }
    bool hasVertexColor() const { return SkToBool(fInColor); }
=======
    const SkPMColor4f& color() const { return fColor; }
    bool hasVertexColor() const { return fInColor.isInitialized(); }
>>>>>>> upstream-releases
    const SkMatrix& localMatrix() const { return fLocalMatrix; }
    bool usesW() const { return fUsesW; }
    const SkISize& atlasSize() const { return fAtlasSize; }

    void addNewProxies(const sk_sp<GrTextureProxy>*, int numActiveProxies, const GrSamplerState&);

    void getGLSLProcessorKey(const GrShaderCaps& caps, GrProcessorKeyBuilder* b) const override;

    GrGLSLPrimitiveProcessor* createGLSLInstance(const GrShaderCaps& caps) const override;

private:
<<<<<<< HEAD
    GrBitmapTextGeoProc(const GrShaderCaps&, GrColor, const sk_sp<GrTextureProxy>* proxies,
                        int numProxies, const GrSamplerState& params, GrMaskFormat format,
                        const SkMatrix& localMatrix, bool usesW);

    const Attribute& onVertexAttribute(int i) const override;
    const TextureSampler& onTextureSampler(int i) const override { return fTextureSamplers[i]; }
||||||| merged common ancestors
    GrBitmapTextGeoProc(GrColor, const sk_sp<GrTextureProxy> proxies[kMaxTextures],
                        const GrSamplerState& params, GrMaskFormat format,
                        const SkMatrix& localMatrix, bool usesLocalCoords);
=======
    GrBitmapTextGeoProc(const GrShaderCaps&, const SkPMColor4f&, bool wideColor,
                        const sk_sp<GrTextureProxy>* proxies, int numProxies,
                        const GrSamplerState& params, GrMaskFormat format,
                        const SkMatrix& localMatrix, bool usesW);
>>>>>>> upstream-releases

    const TextureSampler& onTextureSampler(int i) const override { return fTextureSamplers[i]; }

    SkPMColor4f      fColor;
    SkMatrix         fLocalMatrix;
    bool             fUsesW;
    SkISize          fAtlasSize;  // size for all textures used with fTextureSamplers[].
    TextureSampler   fTextureSamplers[kMaxTextures];
    Attribute        fInPosition;
    Attribute        fInColor;
    Attribute        fInTextureCoords;
    GrMaskFormat     fMaskFormat;

    GR_DECLARE_GEOMETRY_PROCESSOR_TEST

    typedef GrGeometryProcessor INHERITED;
};

#endif
