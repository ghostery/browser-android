/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#include "GrBackendSurface.h"

#include "gl/GrGLUtil.h"

#ifdef SK_VULKAN
#include "vk/GrVkImageLayout.h"
#include "vk/GrVkTypes.h"
#include "vk/GrVkUtil.h"
#endif
<<<<<<< HEAD
#ifdef SK_METAL
#include "mtl/GrMtlTypes.h"
#endif
||||||| merged common ancestors
=======
#ifdef SK_METAL
#include "mtl/GrMtlTypes.h"
#include "mtl/GrMtlCppUtil.h"
#endif
>>>>>>> upstream-releases

GrBackendFormat::GrBackendFormat(GrGLenum format, GrGLenum target)
        : fBackend(GrBackendApi::kOpenGL)
        , fValid(true)
        , fGLFormat(format) {
    switch (target) {
        case GR_GL_TEXTURE_2D:
            fTextureType = GrTextureType::k2D;
            break;
        case GR_GL_TEXTURE_RECTANGLE:
            fTextureType = GrTextureType::kRectangle;
            break;
        case GR_GL_TEXTURE_EXTERNAL:
            fTextureType = GrTextureType::kExternal;
            break;
        default:
            SK_ABORT("Unexpected texture target");
    }
}

const GrGLenum* GrBackendFormat::getGLFormat() const {
    if (this->isValid() && GrBackendApi::kOpenGL == fBackend) {
        return &fGLFormat;
    }
    return nullptr;
}

const GrGLenum* GrBackendFormat::getGLTarget() const {
    if (this->isValid() && GrBackendApi::kOpenGL == fBackend) {
        static constexpr GrGLenum k2D = GR_GL_TEXTURE_2D;
        static constexpr GrGLenum kRect = GR_GL_TEXTURE_RECTANGLE;
        static constexpr GrGLenum kExternal = GR_GL_TEXTURE_EXTERNAL;
        switch (fTextureType) {
            case GrTextureType::k2D:
                return &k2D;
            case GrTextureType::kRectangle:
                return &kRect;
            case GrTextureType::kExternal:
                return &kExternal;
        }
    }
    return nullptr;
}

<<<<<<< HEAD
GrBackendFormat::GrBackendFormat(VkFormat vkFormat)
        : fBackend(kVulkan_GrBackend)
#ifdef SK_VULKAN
||||||| merged common ancestors
#ifdef SK_VULKAN
GrBackendFormat::GrBackendFormat(VkFormat vkFormat)
        : fBackend(kVulkan_GrBackend)
=======
GrBackendFormat GrBackendFormat::MakeVk(const GrVkYcbcrConversionInfo& ycbcrInfo) {
#ifdef SK_BUILD_FOR_ANDROID
    return GrBackendFormat(VK_FORMAT_UNDEFINED, ycbcrInfo);
#else
    return GrBackendFormat();
#endif
}

GrBackendFormat::GrBackendFormat(VkFormat vkFormat, const GrVkYcbcrConversionInfo& ycbcrInfo)
        : fBackend(GrBackendApi::kVulkan)
#ifdef SK_VULKAN
>>>>>>> upstream-releases
        , fValid(true)
<<<<<<< HEAD
#else
        ,fValid(false)
#endif
        , fVkFormat(vkFormat) {
||||||| merged common ancestors
        , fVkFormat(vkFormat) {
=======
#else
        , fValid(false)
#endif
        , fTextureType(GrTextureType::k2D) {
    fVk.fFormat = vkFormat;
    fVk.fYcbcrConversionInfo = ycbcrInfo;
>>>>>>> upstream-releases
}

const VkFormat* GrBackendFormat::getVkFormat() const {
    if (this->isValid() && GrBackendApi::kVulkan == fBackend) {
        return &fVk.fFormat;
    }
    return nullptr;
}

const GrVkYcbcrConversionInfo* GrBackendFormat::getVkYcbcrConversionInfo() const {
    if (this->isValid() && GrBackendApi::kVulkan == fBackend) {
        return &fVk.fYcbcrConversionInfo;
    }
    return nullptr;
}

#ifdef SK_METAL
GrBackendFormat::GrBackendFormat(GrMTLPixelFormat mtlFormat)
        : fBackend(GrBackendApi::kMetal)
        , fValid(true)
        , fMtlFormat(mtlFormat)
        , fTextureType(GrTextureType::k2D) {
}

const GrMTLPixelFormat* GrBackendFormat::getMtlFormat() const {
    if (this->isValid() && GrBackendApi::kMetal == fBackend) {
        return &fMtlFormat;
    }
    return nullptr;
}

#ifdef SK_METAL
GrBackendFormat::GrBackendFormat(GrMTLPixelFormat mtlFormat)
        : fBackend(kMetal_GrBackend)
        , fValid(true)
        , fMtlFormat(mtlFormat) {
}

const GrMTLPixelFormat* GrBackendFormat::getMtlFormat() const {
    if (this->isValid() && kMetal_GrBackend == fBackend) {
        return &fMtlFormat;
    }
    return nullptr;
}
#endif

GrBackendFormat::GrBackendFormat(GrPixelConfig config)
        : fBackend(GrBackendApi::kMock)
        , fValid(true)
        , fMockFormat(config)
        , fTextureType(GrTextureType::k2D) {
}

const GrPixelConfig* GrBackendFormat::getMockFormat() const {
    if (this->isValid() && GrBackendApi::kMock == fBackend) {
        return &fMockFormat;
    }
    return nullptr;
}

<<<<<<< HEAD
||||||| merged common ancestors
#ifdef SK_VULKAN
=======
GrBackendFormat GrBackendFormat::makeTexture2D() const {
    // TODO: once we support ycbcr conversions in Vulkan we need to check if we are using an
    // external format since they will not be able to be made into a Texture2D.
    GrBackendFormat copy = *this;
    copy.fTextureType = GrTextureType::k2D;
    return copy;
}

bool GrBackendFormat::operator==(const GrBackendFormat& that) const {
    // Invalid GrBackendFormats are never equal to anything.
    if (!fValid || !that.fValid) {
        return false;
    }

    if (fBackend != that.fBackend) {
        return false;
    }

    switch (fBackend) {
        case GrBackendApi::kOpenGL:
            return fGLFormat == that.fGLFormat;
        case GrBackendApi::kVulkan:
#ifdef SK_VULKAN
            return fVk.fFormat == that.fVk.fFormat &&
                   fVk.fYcbcrConversionInfo == that.fVk.fYcbcrConversionInfo;
#endif
            break;
#ifdef SK_METAL
        case GrBackendApi::kMetal:
            return fMtlFormat == that.fMtlFormat;
#endif
            break;
        case GrBackendApi::kMock:
            return fMockFormat == that.fMockFormat;
        default:
            SK_ABORT("Unknown GrBackend");
    }
    return false;
}

>>>>>>> upstream-releases
GrBackendTexture::GrBackendTexture(int width,
                                   int height,
                                   const GrVkImageInfo& vkInfo)
#ifdef SK_VULKAN
        : GrBackendTexture(width, height, vkInfo,
                           sk_sp<GrVkImageLayout>(new GrVkImageLayout(vkInfo.fImageLayout))) {}
#else
        : fIsValid(false) {}
#endif

#ifdef SK_VULKAN
GrBackendTexture::GrBackendTexture(int width,
                                   int height,
<<<<<<< HEAD
                                   const GrVkImageInfo& vkInfo,
                                   sk_sp<GrVkImageLayout> layout)
        : fIsValid(true)
        , fWidth(width)
        , fHeight(height)
        , fConfig(kUnknown_GrPixelConfig)
        , fMipMapped(GrMipMapped(vkInfo.fLevelCount > 1))
        , fBackend(kVulkan_GrBackend)
        , fVkInfo(vkInfo, layout.release()) {
}
#endif
||||||| merged common ancestors
                                   GrPixelConfig config,
                                   const GrGLTextureInfo& glInfo)
        : GrBackendTexture(width, height, config, GrMipMapped::kNo, glInfo) {}
=======
                                   const GrVkImageInfo& vkInfo,
                                   sk_sp<GrVkImageLayout> layout)
        : fIsValid(true)
        , fWidth(width)
        , fHeight(height)
        , fConfig(kUnknown_GrPixelConfig)
        , fMipMapped(GrMipMapped(vkInfo.fLevelCount > 1))
        , fBackend(GrBackendApi::kVulkan)
        , fVkInfo(vkInfo, layout.release()) {
}
#endif
>>>>>>> upstream-releases

#ifdef SK_METAL
GrBackendTexture::GrBackendTexture(int width,
                                   int height,
                                   GrMipMapped mipMapped,
                                   const GrMtlTextureInfo& mtlInfo)
        : fIsValid(true)
        , fWidth(width)
        , fHeight(height)
        , fConfig(GrPixelConfig::kUnknown_GrPixelConfig)
        , fMipMapped(mipMapped)
<<<<<<< HEAD
        , fBackend(kMetal_GrBackend)
        , fMtlInfo(mtlInfo) {}
#endif
||||||| merged common ancestors
        , fBackend(kOpenGL_GrBackend)
        , fGLInfo(glInfo) {}
=======
        , fBackend(GrBackendApi::kMetal)
        , fMtlInfo(mtlInfo) {}
#endif
>>>>>>> upstream-releases

GrBackendTexture::GrBackendTexture(int width,
                                   int height,
                                   GrMipMapped mipMapped,
                                   const GrGLTextureInfo& glInfo)
        : fIsValid(true)
        , fWidth(width)
        , fHeight(height)
        , fConfig(kUnknown_GrPixelConfig)
        , fMipMapped(mipMapped)
        , fBackend(GrBackendApi::kOpenGL)
        , fGLInfo(glInfo) {}

GrBackendTexture::GrBackendTexture(int width,
                                   int height,
                                   GrMipMapped mipMapped,
                                   const GrMockTextureInfo& mockInfo)
        : fIsValid(true)
        , fWidth(width)
        , fHeight(height)
        , fConfig(mockInfo.fConfig)
        , fMipMapped(mipMapped)
        , fBackend(GrBackendApi::kMock)
        , fMockInfo(mockInfo) {}

<<<<<<< HEAD
GrBackendTexture::~GrBackendTexture() {
    this->cleanup();
}

void GrBackendTexture::cleanup() {
#ifdef SK_VULKAN
    if (this->isValid() && kVulkan_GrBackend == fBackend) {
        fVkInfo.cleanup();
    }
#endif
}

GrBackendTexture::GrBackendTexture(const GrBackendTexture& that) : fIsValid(false) {
    *this = that;
}

GrBackendTexture& GrBackendTexture::operator=(const GrBackendTexture& that) {
    if (!that.isValid()) {
        this->cleanup();
        fIsValid = false;
        return *this;
    }
    fWidth = that.fWidth;
    fHeight = that.fHeight;
    fConfig = that.fConfig;
    fMipMapped = that.fMipMapped;
    fBackend = that.fBackend;

    switch (that.fBackend) {
        case kOpenGL_GrBackend:
            fGLInfo = that.fGLInfo;
            break;
        case kVulkan_GrBackend:
#ifdef SK_VULKAN
            fVkInfo.assign(that.fVkInfo, this->isValid());
#endif
            break;
#ifdef SK_METAL
        case kMetal_GrBackend:
            fMtlInfo = that.fMtlInfo;
            break;
#endif
        case kMock_GrBackend:
            fMockInfo = that.fMockInfo;
            break;
        default:
            SK_ABORT("Unknown GrBackend");
    }
    fIsValid = that.fIsValid;
    return *this;
}

bool GrBackendTexture::getVkImageInfo(GrVkImageInfo* outInfo) const {
#ifdef SK_VULKAN
    if (this->isValid() && kVulkan_GrBackend == fBackend) {
        *outInfo = fVkInfo.snapImageInfo();
        return true;
    }
#endif
    return false;
}

void GrBackendTexture::setVkImageLayout(VkImageLayout layout) {
#ifdef SK_VULKAN
    if (this->isValid() && kVulkan_GrBackend == fBackend) {
        fVkInfo.setImageLayout(layout);
    }
#endif
}

// We need a stubbed version of GrVkImageLayout for non vulkan builds
#ifndef SK_VULKAN
class GrVkImageLayout : public SkRefCnt {
    GrVkImageLayout(VkImageLayout layout) {}
};
#endif

sk_sp<GrVkImageLayout> GrBackendTexture::getGrVkImageLayout() const {
||||||| merged common ancestors
=======
GrBackendTexture::~GrBackendTexture() {
    this->cleanup();
}

void GrBackendTexture::cleanup() {
>>>>>>> upstream-releases
#ifdef SK_VULKAN
<<<<<<< HEAD
    if (this->isValid() && kVulkan_GrBackend == fBackend) {
        return fVkInfo.getGrVkImageLayout();
||||||| merged common ancestors
const GrVkImageInfo* GrBackendTexture::getVkImageInfo() const {
    if (this->isValid() && kVulkan_GrBackend == fBackend) {
        return &fVkInfo;
=======
    if (this->isValid() && GrBackendApi::kVulkan == fBackend) {
        fVkInfo.cleanup();
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
#endif
    return nullptr;
||||||| merged common ancestors
    return nullptr;
=======
#endif
>>>>>>> upstream-releases
}
<<<<<<< HEAD

#ifdef SK_METAL
bool GrBackendTexture::getMtlTextureInfo(GrMtlTextureInfo* outInfo) const {
    if (this->isValid() && kMetal_GrBackend == fBackend) {
        *outInfo = fMtlInfo;
        return true;
    }
    return false;
}
||||||| merged common ancestors
=======

GrBackendTexture::GrBackendTexture(const GrBackendTexture& that) : fIsValid(false) {
    *this = that;
}

GrBackendTexture& GrBackendTexture::operator=(const GrBackendTexture& that) {
    if (!that.isValid()) {
        this->cleanup();
        fIsValid = false;
        return *this;
    }
    fWidth = that.fWidth;
    fHeight = that.fHeight;
    fConfig = that.fConfig;
    fMipMapped = that.fMipMapped;
    fBackend = that.fBackend;

    switch (that.fBackend) {
        case GrBackendApi::kOpenGL:
            fGLInfo = that.fGLInfo;
            break;
        case GrBackendApi::kVulkan:
#ifdef SK_VULKAN
            fVkInfo.assign(that.fVkInfo, this->isValid());
>>>>>>> upstream-releases
#endif
            break;
#ifdef SK_METAL
        case GrBackendApi::kMetal:
            fMtlInfo = that.fMtlInfo;
            break;
#endif
        case GrBackendApi::kMock:
            fMockInfo = that.fMockInfo;
            break;
        default:
            SK_ABORT("Unknown GrBackend");
    }
    fIsValid = that.fIsValid;
    return *this;
}

<<<<<<< HEAD
bool GrBackendTexture::getGLTextureInfo(GrGLTextureInfo* outInfo) const {
    if (this->isValid() && kOpenGL_GrBackend == fBackend) {
        *outInfo = fGLInfo;
        return true;
||||||| merged common ancestors
const GrGLTextureInfo* GrBackendTexture::getGLTextureInfo() const {
    if (this->isValid() && kOpenGL_GrBackend == fBackend) {
        return &fGLInfo;
=======
bool GrBackendTexture::getVkImageInfo(GrVkImageInfo* outInfo) const {
#ifdef SK_VULKAN
    if (this->isValid() && GrBackendApi::kVulkan == fBackend) {
        *outInfo = fVkInfo.snapImageInfo();
        return true;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    return false;
||||||| merged common ancestors
    return nullptr;
=======
#endif
    return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool GrBackendTexture::getMockTextureInfo(GrMockTextureInfo* outInfo) const {
    if (this->isValid() && kMock_GrBackend == fBackend) {
        *outInfo = fMockInfo;
        return true;
||||||| merged common ancestors
const GrMockTextureInfo* GrBackendTexture::getMockTextureInfo() const {
    if (this->isValid() && kMock_GrBackend == fBackend) {
        return &fMockInfo;
=======
void GrBackendTexture::setVkImageLayout(VkImageLayout layout) {
#ifdef SK_VULKAN
    if (this->isValid() && GrBackendApi::kVulkan == fBackend) {
        fVkInfo.setImageLayout(layout);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    return false;
||||||| merged common ancestors
    return nullptr;
=======
#endif
}

// We need a stubbed version of GrVkImageLayout for non vulkan builds
#ifndef SK_VULKAN
class GrVkImageLayout : public SkRefCnt {
    GrVkImageLayout(VkImageLayout layout) {}
};
#endif

sk_sp<GrVkImageLayout> GrBackendTexture::getGrVkImageLayout() const {
#ifdef SK_VULKAN
    if (this->isValid() && GrBackendApi::kVulkan == fBackend) {
        return fVkInfo.getGrVkImageLayout();
    }
#endif
    return nullptr;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
#if GR_TEST_UTILS
bool GrBackendTexture::TestingOnly_Equals(const GrBackendTexture& t0, const GrBackendTexture& t1) {
    if (!t0.isValid() || !t1.isValid()) {
        return false; // two invalid backend textures are not considered equal
    }

    if (t0.fWidth != t1.fWidth ||
        t0.fHeight != t1.fHeight ||
        t0.fConfig != t1.fConfig ||
        t0.fMipMapped != t1.fMipMapped ||
        t0.fBackend != t1.fBackend) {
        return false;
    }
||||||| merged common ancestors
////////////////////////////////////////////////////////////////////////////////////////////////////
=======
#ifdef SK_METAL
bool GrBackendTexture::getMtlTextureInfo(GrMtlTextureInfo* outInfo) const {
    if (this->isValid() && GrBackendApi::kMetal == fBackend) {
        *outInfo = fMtlInfo;
        return true;
    }
    return false;
}
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
    switch (t0.fBackend) {
    case kOpenGL_GrBackend:
        return t0.fGLInfo == t1.fGLInfo;
    case kMock_GrBackend:
        return t0.fMockInfo == t1.fMockInfo;
    case kVulkan_GrBackend:
||||||| merged common ancestors
=======
bool GrBackendTexture::getGLTextureInfo(GrGLTextureInfo* outInfo) const {
    if (this->isValid() && GrBackendApi::kOpenGL == fBackend) {
        *outInfo = fGLInfo;
        return true;
    }
    return false;
}

bool GrBackendTexture::getMockTextureInfo(GrMockTextureInfo* outInfo) const {
    if (this->isValid() && GrBackendApi::kMock == fBackend) {
        *outInfo = fMockInfo;
        return true;
    }
    return false;
}

GrBackendFormat GrBackendTexture::getBackendFormat() const {
    if (!this->isValid()) {
        return GrBackendFormat();
    }
    switch (fBackend) {
        case GrBackendApi::kOpenGL:
            return GrBackendFormat::MakeGL(fGLInfo.fFormat, fGLInfo.fTarget);
>>>>>>> upstream-releases
#ifdef SK_VULKAN
<<<<<<< HEAD
        return t0.fVkInfo == t1.fVkInfo;
#else
        // fall through
#endif
    case kMetal_GrBackend:
#ifdef SK_METAL
        return t0.fMtlInfo == t1.fMtlInfo;
#else
        // fall through
#endif
    default:
        return false;
    }

    SkASSERT(0);
    return false;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

||||||| merged common ancestors
=======
        case GrBackendApi::kVulkan: {
            auto info = fVkInfo.snapImageInfo();
            if (info.fYcbcrConversionInfo.isValid()) {
                SkASSERT(info.fFormat == VK_FORMAT_UNDEFINED);
                return GrBackendFormat::MakeVk(info.fYcbcrConversionInfo);
            }
            return GrBackendFormat::MakeVk(info.fFormat);
        }
#endif
#ifdef SK_METAL
        case GrBackendApi::kMetal: {
            GrMtlTextureInfo mtlInfo;
            SkAssertResult(this->getMtlTextureInfo(&mtlInfo));
            return GrBackendFormat::MakeMtl(GrGetMTLPixelFormatFromMtlTextureInfo(mtlInfo));
        }
#endif
        case GrBackendApi::kMock:
            return GrBackendFormat::MakeMock(fMockInfo.fConfig);
        default:
            return GrBackendFormat();
    }
}

#if GR_TEST_UTILS
bool GrBackendTexture::TestingOnly_Equals(const GrBackendTexture& t0, const GrBackendTexture& t1) {
    if (!t0.isValid() || !t1.isValid()) {
        return false; // two invalid backend textures are not considered equal
    }

    if (t0.fWidth != t1.fWidth ||
        t0.fHeight != t1.fHeight ||
        t0.fConfig != t1.fConfig ||
        t0.fMipMapped != t1.fMipMapped ||
        t0.fBackend != t1.fBackend) {
        return false;
    }

    switch (t0.fBackend) {
    case GrBackendApi::kOpenGL:
        return t0.fGLInfo == t1.fGLInfo;
    case GrBackendApi::kMock:
        return t0.fMockInfo == t1.fMockInfo;
    case GrBackendApi::kVulkan:
#ifdef SK_VULKAN
        return t0.fVkInfo == t1.fVkInfo;
#else
        // fall through
#endif
    case GrBackendApi::kMetal:
#ifdef SK_METAL
        return t0.fMtlInfo == t1.fMtlInfo;
#else
        // fall through
#endif
    default:
        return false;
    }

    SkASSERT(0);
    return false;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

>>>>>>> upstream-releases
GrBackendRenderTarget::GrBackendRenderTarget(int width,
                                             int height,
                                             int sampleCnt,
                                             int stencilBits,
                                             const GrVkImageInfo& vkInfo)
        : GrBackendRenderTarget(width, height, sampleCnt, vkInfo) {
    // This is a deprecated constructor that takes a bogus stencil bits.
    SkASSERT(0 == stencilBits);
}

GrBackendRenderTarget::GrBackendRenderTarget(int width,
                                             int height,
                                             int sampleCnt,
                                             const GrVkImageInfo& vkInfo)
#ifdef SK_VULKAN
        : GrBackendRenderTarget(width, height, sampleCnt, vkInfo,
                                sk_sp<GrVkImageLayout>(new GrVkImageLayout(vkInfo.fImageLayout))) {}
#else
        : fIsValid(false) {}
#endif

#ifdef SK_VULKAN
GrBackendRenderTarget::GrBackendRenderTarget(int width,
                                             int height,
                                             int sampleCnt,
                                             const GrVkImageInfo& vkInfo,
                                             sk_sp<GrVkImageLayout> layout)
        : fIsValid(true)
        , fWidth(width)
        , fHeight(height)
        , fSampleCnt(SkTMax(1, sampleCnt))
<<<<<<< HEAD
        , fStencilBits(0)  // We always create stencil buffers internally for vulkan
        , fConfig(kUnknown_GrPixelConfig)
        , fBackend(kVulkan_GrBackend)
        , fVkInfo(vkInfo, layout.release()) {}
#endif

#ifdef SK_METAL
GrBackendRenderTarget::GrBackendRenderTarget(int width,
                                             int height,
                                             int sampleCnt,
                                             const GrMtlTextureInfo& mtlInfo)
        : fIsValid(true)
        , fWidth(width)
        , fHeight(height)
        , fSampleCnt(SkTMax(1, sampleCnt))
        , fStencilBits(0)
        , fConfig(GrPixelConfig::kUnknown_GrPixelConfig)
        , fBackend(kMetal_GrBackend)
        , fMtlInfo(mtlInfo) {}
||||||| merged common ancestors
        , fStencilBits(stencilBits)
        , fConfig(GrVkFormatToPixelConfig(vkInfo.fFormat))
        , fBackend(kVulkan_GrBackend)
        , fVkInfo(vkInfo) {}
=======
        , fStencilBits(0)  // We always create stencil buffers internally for vulkan
        , fConfig(kUnknown_GrPixelConfig)
        , fBackend(GrBackendApi::kVulkan)
        , fVkInfo(vkInfo, layout.release()) {}
#endif

#ifdef SK_METAL
GrBackendRenderTarget::GrBackendRenderTarget(int width,
                                             int height,
                                             int sampleCnt,
                                             const GrMtlTextureInfo& mtlInfo)
        : fIsValid(true)
        , fWidth(width)
        , fHeight(height)
        , fSampleCnt(SkTMax(1, sampleCnt))
        , fStencilBits(0)
        , fConfig(GrPixelConfig::kUnknown_GrPixelConfig)
        , fBackend(GrBackendApi::kMetal)
        , fMtlInfo(mtlInfo) {}
>>>>>>> upstream-releases
#endif

GrBackendRenderTarget::GrBackendRenderTarget(int width,
                                             int height,
                                             int sampleCnt,
                                             int stencilBits,
                                             const GrGLFramebufferInfo& glInfo)
        : fIsValid(true)
        , fWidth(width)
        , fHeight(height)
        , fSampleCnt(SkTMax(1, sampleCnt))
        , fStencilBits(stencilBits)
<<<<<<< HEAD
        , fConfig(kUnknown_GrPixelConfig)
        , fBackend(kOpenGL_GrBackend)
        , fGLInfo(glInfo) {}
||||||| merged common ancestors
        , fConfig(config)
        , fBackend(kOpenGL_GrBackend)
        , fGLInfo(glInfo) {}
=======
        , fConfig(kUnknown_GrPixelConfig)
        , fBackend(GrBackendApi::kOpenGL)
        , fGLInfo(glInfo) {
    fIsValid = SkToBool(glInfo.fFormat); // the glInfo must have a valid format
}
>>>>>>> upstream-releases

GrBackendRenderTarget::GrBackendRenderTarget(int width,
                                             int height,
                                             int sampleCnt,
                                             int stencilBits,
                                             const GrMockRenderTargetInfo& mockInfo)
        : fIsValid(true)
        , fWidth(width)
        , fHeight(height)
        , fSampleCnt(SkTMax(1, sampleCnt))
        , fStencilBits(stencilBits)
<<<<<<< HEAD
        , fConfig(mockInfo.fConfig)
        , fMockInfo(mockInfo) {}
||||||| merged common ancestors
        , fConfig(GrGLSizedFormatToPixelConfig(glInfo.fFormat))
        , fBackend(kOpenGL_GrBackend)
        , fGLInfo(glInfo) {}
=======
        , fConfig(mockInfo.fConfig)
        , fMockInfo(mockInfo) {}

GrBackendRenderTarget::~GrBackendRenderTarget() {
    this->cleanup();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
GrBackendRenderTarget::~GrBackendRenderTarget() {
    this->cleanup();
}

void GrBackendRenderTarget::cleanup() {
||||||| merged common ancestors
=======
void GrBackendRenderTarget::cleanup() {
>>>>>>> upstream-releases
#ifdef SK_VULKAN
<<<<<<< HEAD
    if (this->isValid() && kVulkan_GrBackend == fBackend) {
        fVkInfo.cleanup();
||||||| merged common ancestors
const GrVkImageInfo* GrBackendRenderTarget::getVkImageInfo() const {
    if (kVulkan_GrBackend == fBackend) {
        return &fVkInfo;
=======
    if (this->isValid() && GrBackendApi::kVulkan == fBackend) {
        fVkInfo.cleanup();
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
#endif
||||||| merged common ancestors
    return nullptr;
=======
#endif
}

GrBackendRenderTarget::GrBackendRenderTarget(const GrBackendRenderTarget& that) : fIsValid(false) {
    *this = that;
}

GrBackendRenderTarget& GrBackendRenderTarget::operator=(const GrBackendRenderTarget& that) {
    if (!that.isValid()) {
        this->cleanup();
        fIsValid = false;
        return *this;
    }
    fWidth = that.fWidth;
    fHeight = that.fHeight;
    fSampleCnt = that.fSampleCnt;
    fStencilBits = that.fStencilBits;
    fConfig = that.fConfig;
    fBackend = that.fBackend;

    switch (that.fBackend) {
        case GrBackendApi::kOpenGL:
            fGLInfo = that.fGLInfo;
            break;
        case GrBackendApi::kVulkan:
#ifdef SK_VULKAN
            fVkInfo.assign(that.fVkInfo, this->isValid());
#endif
            break;
#ifdef SK_METAL
        case GrBackendApi::kMetal:
            fMtlInfo = that.fMtlInfo;
            break;
#endif
        case GrBackendApi::kMock:
            fMockInfo = that.fMockInfo;
            break;
        default:
            SK_ABORT("Unknown GrBackend");
    }
    fIsValid = that.fIsValid;
    return *this;
>>>>>>> upstream-releases
}
<<<<<<< HEAD

GrBackendRenderTarget::GrBackendRenderTarget(const GrBackendRenderTarget& that) : fIsValid(false) {
    *this = that;
}

GrBackendRenderTarget& GrBackendRenderTarget::operator=(const GrBackendRenderTarget& that) {
    if (!that.isValid()) {
        this->cleanup();
        fIsValid = false;
        return *this;
    }
    fWidth = that.fWidth;
    fHeight = that.fHeight;
    fSampleCnt = that.fSampleCnt;
    fStencilBits = that.fStencilBits;
    fConfig = that.fConfig;
    fBackend = that.fBackend;

    switch (that.fBackend) {
        case kOpenGL_GrBackend:
            fGLInfo = that.fGLInfo;
            break;
        case kVulkan_GrBackend:
#ifdef SK_VULKAN
            fVkInfo.assign(that.fVkInfo, this->isValid());
||||||| merged common ancestors
=======

bool GrBackendRenderTarget::getVkImageInfo(GrVkImageInfo* outInfo) const {
#ifdef SK_VULKAN
    if (this->isValid() && GrBackendApi::kVulkan == fBackend) {
        *outInfo = fVkInfo.snapImageInfo();
        return true;
    }
>>>>>>> upstream-releases
#endif
<<<<<<< HEAD
            break;
#ifdef SK_METAL
        case kMetal_GrBackend:
            fMtlInfo = that.fMtlInfo;
            break;
#endif
        case kMock_GrBackend:
            fMockInfo = that.fMockInfo;
            break;
        default:
            SK_ABORT("Unknown GrBackend");
    }
    fIsValid = that.fIsValid;
    return *this;
}

bool GrBackendRenderTarget::getVkImageInfo(GrVkImageInfo* outInfo) const {
#ifdef SK_VULKAN
    if (this->isValid() && kVulkan_GrBackend == fBackend) {
        *outInfo = fVkInfo.snapImageInfo();
        return true;
    }
#endif
    return false;
}

void GrBackendRenderTarget::setVkImageLayout(VkImageLayout layout) {
#ifdef SK_VULKAN
    if (this->isValid() && kVulkan_GrBackend == fBackend) {
        fVkInfo.setImageLayout(layout);
    }
#endif
}
||||||| merged common ancestors
=======
    return false;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
sk_sp<GrVkImageLayout> GrBackendRenderTarget::getGrVkImageLayout() const {
#ifdef SK_VULKAN
    if (this->isValid() && kVulkan_GrBackend == fBackend) {
        return fVkInfo.getGrVkImageLayout();
||||||| merged common ancestors
const GrGLFramebufferInfo* GrBackendRenderTarget::getGLFramebufferInfo() const {
    if (kOpenGL_GrBackend == fBackend) {
        return &fGLInfo;
=======
void GrBackendRenderTarget::setVkImageLayout(VkImageLayout layout) {
#ifdef SK_VULKAN
    if (this->isValid() && GrBackendApi::kVulkan == fBackend) {
        fVkInfo.setImageLayout(layout);
    }
#endif
}

sk_sp<GrVkImageLayout> GrBackendRenderTarget::getGrVkImageLayout() const {
#ifdef SK_VULKAN
    if (this->isValid() && GrBackendApi::kVulkan == fBackend) {
        return fVkInfo.getGrVkImageLayout();
>>>>>>> upstream-releases
    }
#endif
    return nullptr;
}

<<<<<<< HEAD
#ifdef SK_METAL
bool GrBackendRenderTarget::getMtlTextureInfo(GrMtlTextureInfo* outInfo) const {
    if (this->isValid() && kMetal_GrBackend == fBackend) {
        *outInfo = fMtlInfo;
        return true;
    }
    return false;
}
#endif

bool GrBackendRenderTarget::getGLFramebufferInfo(GrGLFramebufferInfo* outInfo) const {
    if (this->isValid() && kOpenGL_GrBackend == fBackend) {
        *outInfo = fGLInfo;
        return true;
    }
    return false;
}

bool GrBackendRenderTarget::getMockRenderTargetInfo(GrMockRenderTargetInfo* outInfo) const {
    if (this->isValid() && kMock_GrBackend == fBackend) {
        *outInfo = fMockInfo;
        return true;
    }
    return false;
}

#if GR_TEST_UTILS
bool GrBackendRenderTarget::TestingOnly_Equals(const GrBackendRenderTarget& r0,
                                               const GrBackendRenderTarget& r1) {
    if (!r0.isValid() || !r1.isValid()) {
        return false; // two invalid backend rendertargets are not considered equal
    }

    if (r0.fWidth != r1.fWidth ||
        r0.fHeight != r1.fHeight ||
        r0.fSampleCnt != r1.fSampleCnt ||
        r0.fStencilBits != r1.fStencilBits ||
        r0.fConfig != r1.fConfig ||
        r0.fBackend != r1.fBackend) {
        return false;
    }

    switch (r0.fBackend) {
    case kOpenGL_GrBackend:
        return r0.fGLInfo == r1.fGLInfo;
    case kMock_GrBackend:
        return r0.fMockInfo == r1.fMockInfo;
    case kVulkan_GrBackend:
#ifdef SK_VULKAN
        return r0.fVkInfo == r1.fVkInfo;
#else
        // fall through
#endif
    case kMetal_GrBackend:
#ifdef SK_METAL
        return r0.fMtlInfo == r1.fMtlInfo;
#else
        // fall through
#endif
    default:
        return false;
    }

    SkASSERT(0);
    return false;
}
#endif
||||||| merged common ancestors
=======
#ifdef SK_METAL
bool GrBackendRenderTarget::getMtlTextureInfo(GrMtlTextureInfo* outInfo) const {
    if (this->isValid() && GrBackendApi::kMetal == fBackend) {
        *outInfo = fMtlInfo;
        return true;
    }
    return false;
}
#endif

bool GrBackendRenderTarget::getGLFramebufferInfo(GrGLFramebufferInfo* outInfo) const {
    if (this->isValid() && GrBackendApi::kOpenGL == fBackend) {
        *outInfo = fGLInfo;
        return true;
    }
    return false;
}

bool GrBackendRenderTarget::getMockRenderTargetInfo(GrMockRenderTargetInfo* outInfo) const {
    if (this->isValid() && GrBackendApi::kMock == fBackend) {
        *outInfo = fMockInfo;
        return true;
    }
    return false;
}

#if GR_TEST_UTILS
bool GrBackendRenderTarget::TestingOnly_Equals(const GrBackendRenderTarget& r0,
                                               const GrBackendRenderTarget& r1) {
    if (!r0.isValid() || !r1.isValid()) {
        return false; // two invalid backend rendertargets are not considered equal
    }

    if (r0.fWidth != r1.fWidth ||
        r0.fHeight != r1.fHeight ||
        r0.fSampleCnt != r1.fSampleCnt ||
        r0.fStencilBits != r1.fStencilBits ||
        r0.fConfig != r1.fConfig ||
        r0.fBackend != r1.fBackend) {
        return false;
    }

    switch (r0.fBackend) {
    case GrBackendApi::kOpenGL:
        return r0.fGLInfo == r1.fGLInfo;
    case GrBackendApi::kMock:
        return r0.fMockInfo == r1.fMockInfo;
    case GrBackendApi::kVulkan:
#ifdef SK_VULKAN
        return r0.fVkInfo == r1.fVkInfo;
#else
        // fall through
#endif
    case GrBackendApi::kMetal:
#ifdef SK_METAL
        return r0.fMtlInfo == r1.fMtlInfo;
#else
        // fall through
#endif
    default:
        return false;
    }

    SkASSERT(0);
    return false;
}
#endif
>>>>>>> upstream-releases
