/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "WebGLContext.h"

#include <algorithm>
#include "GLSLANG/ShaderLang.h"
#include "CanvasUtils.h"
#include "GLContext.h"
#include "jsfriendapi.h"
#include "mozilla/CheckedInt.h"
#include "mozilla/Preferences.h"
<<<<<<< HEAD
||||||| merged common ancestors
#include "mozilla/Services.h"
=======
#include "mozilla/StaticPrefs.h"
>>>>>>> upstream-releases
#include "nsIObserverService.h"
#include "nsPrintfCString.h"
#include "WebGLActiveInfo.h"
#include "WebGLBuffer.h"
#include "WebGLContextUtils.h"
#include "WebGLFramebuffer.h"
#include "WebGLProgram.h"
#include "WebGLRenderbuffer.h"
#include "WebGLSampler.h"
#include "WebGLShader.h"
#include "WebGLTexture.h"
#include "WebGLUniformLocation.h"
#include "WebGLValidateStrings.h"
#include "WebGLVertexArray.h"
#include "WebGLVertexAttribData.h"

#if defined(MOZ_WIDGET_COCOA)
#  include "nsCocoaFeatures.h"
#endif

////////////////////
// Minimum value constants defined in GLES 2.0.25 $6.2 "State Tables":
const uint32_t kMinMaxVertexAttribs = 8;            // Page 164
const uint32_t kMinMaxVertexUniformVectors = 128;   // Page 164
const uint32_t kMinMaxFragmentUniformVectors = 16;  // Page 164
const uint32_t kMinMaxVaryingVectors = 8;           // Page 164

const uint32_t kMinMaxVertexTextureImageUnits = 0;    // Page 164
const uint32_t kMinMaxFragmentTextureImageUnits = 8;  // Page 164
const uint32_t kMinMaxCombinedTextureImageUnits = 8;  // Page 164

const uint32_t kMinMaxColorAttachments = 4;
const uint32_t kMinMaxDrawBuffers = 4;

// These few deviate from the spec: (The minimum values in the spec are
// ridiculously low)
const uint32_t kMinMaxTextureSize = 1024;        // ES2 spec says `64` (p162)
const uint32_t kMinMaxCubeMapTextureSize = 512;  // ES2 spec says `16` (p162)
const uint32_t kMinMaxRenderbufferSize = 1024;   // ES2 spec says `1` (p164)

// Minimum value constants defined in GLES 3.0.4 $6.2 "State Tables":
const uint32_t kMinMax3DTextureSize = 256;
const uint32_t kMinMaxArrayTextureLayers = 256;

////////////////////
// "Common" but usable values to avoid WebGL fingerprinting:
const uint32_t kCommonMaxTextureSize = 2048;
const uint32_t kCommonMaxCubeMapTextureSize = 2048;
const uint32_t kCommonMaxRenderbufferSize = 2048;

const uint32_t kCommonMaxVertexTextureImageUnits = 8;
const uint32_t kCommonMaxFragmentTextureImageUnits = 8;
const uint32_t kCommonMaxCombinedTextureImageUnits = 16;

const uint32_t kCommonMaxVertexAttribs = 16;
const uint32_t kCommonMaxVertexUniformVectors = 256;
const uint32_t kCommonMaxFragmentUniformVectors = 224;
const uint32_t kCommonMaxVaryingVectors = 8;

const uint32_t kCommonMaxViewportDims = 4096;

// The following ranges came from a 2013 Moto E and an old macbook.
const float kCommonAliasedPointSizeRangeMin = 1;
const float kCommonAliasedPointSizeRangeMax = 63;
const float kCommonAliasedLineWidthRangeMin = 1;
const float kCommonAliasedLineWidthRangeMax = 1;

template <class T>
static bool RestrictCap(T* const cap, const T restrictedVal) {
  if (*cap < restrictedVal) {
    return false;  // already too low!
  }

  *cap = restrictedVal;
  return true;
}

////////////////////

namespace mozilla {

bool WebGLContext::ValidateBlendEquationEnum(GLenum mode, const char* info) {
  switch (mode) {
    case LOCAL_GL_FUNC_ADD:
    case LOCAL_GL_FUNC_SUBTRACT:
    case LOCAL_GL_FUNC_REVERSE_SUBTRACT:
      return true;

    case LOCAL_GL_MIN:
    case LOCAL_GL_MAX:
      if (IsWebGL2() ||
          IsExtensionEnabled(WebGLExtensionID::EXT_blend_minmax)) {
        return true;
      }

      break;

    default:
      break;
  }

  ErrorInvalidEnumInfo(info, mode);
  return false;
}

bool WebGLContext::ValidateBlendFuncEnumsCompatibility(GLenum sfactor,
                                                       GLenum dfactor,
                                                       const char* info) {
  bool sfactorIsConstantColor = sfactor == LOCAL_GL_CONSTANT_COLOR ||
                                sfactor == LOCAL_GL_ONE_MINUS_CONSTANT_COLOR;
  bool sfactorIsConstantAlpha = sfactor == LOCAL_GL_CONSTANT_ALPHA ||
                                sfactor == LOCAL_GL_ONE_MINUS_CONSTANT_ALPHA;
  bool dfactorIsConstantColor = dfactor == LOCAL_GL_CONSTANT_COLOR ||
                                dfactor == LOCAL_GL_ONE_MINUS_CONSTANT_COLOR;
  bool dfactorIsConstantAlpha = dfactor == LOCAL_GL_CONSTANT_ALPHA ||
                                dfactor == LOCAL_GL_ONE_MINUS_CONSTANT_ALPHA;
  if ((sfactorIsConstantColor && dfactorIsConstantAlpha) ||
      (dfactorIsConstantColor && sfactorIsConstantAlpha)) {
    ErrorInvalidOperation(
        "%s are mutually incompatible, see section 6.8 in"
        " the WebGL 1.0 spec",
        info);
    return false;
  }

  return true;
}

bool WebGLContext::ValidateStencilOpEnum(GLenum action, const char* info) {
  switch (action) {
    case LOCAL_GL_KEEP:
    case LOCAL_GL_ZERO:
    case LOCAL_GL_REPLACE:
    case LOCAL_GL_INCR:
    case LOCAL_GL_INCR_WRAP:
    case LOCAL_GL_DECR:
    case LOCAL_GL_DECR_WRAP:
    case LOCAL_GL_INVERT:
      return true;

    default:
      ErrorInvalidEnumInfo(info, action);
      return false;
  }
}

bool WebGLContext::ValidateFaceEnum(const GLenum face) {
  switch (face) {
    case LOCAL_GL_FRONT:
    case LOCAL_GL_BACK:
    case LOCAL_GL_FRONT_AND_BACK:
      return true;

    default:
      ErrorInvalidEnumInfo("face", face);
      return false;
  }
}

<<<<<<< HEAD
bool WebGLContext::ValidateUniformLocation(
    const WebGLUniformLocation* const loc) {
  /* GLES 2.0.25, p38:
   *   If the value of location is -1, the Uniform* commands will silently
   *   ignore the data passed in, and the current uniform values will not be
   *   changed.
   */
  if (!loc) return false;

  if (!ValidateObjectAllowDeleted("loc", *loc)) return false;

  if (!mCurrentProgram) {
    ErrorInvalidOperation("No program is currently bound.");
    return false;
  }

  return loc->ValidateForProgram(mCurrentProgram);
}
||||||| merged common ancestors
bool
WebGLContext::ValidateUniformLocation(const WebGLUniformLocation* const loc)
{
    /* GLES 2.0.25, p38:
     *   If the value of location is -1, the Uniform* commands will silently
     *   ignore the data passed in, and the current uniform values will not be
     *   changed.
     */
    if (!loc)
        return false;

    if (!ValidateObjectAllowDeleted("loc", *loc))
        return false;

    if (!mCurrentProgram) {
        ErrorInvalidOperation("No program is currently bound.");
        return false;
    }

    return loc->ValidateForProgram(mCurrentProgram);
}
=======
bool WebGLContext::ValidateUniformLocation(
    const WebGLUniformLocation* const loc) {
  /* GLES 2.0.25, p38:
   *   If the value of location is -1, the Uniform* commands will silently
   *   ignore the data passed in, and the current uniform values will not be
   *   changed.
   */
  if (!loc) return false;
>>>>>>> upstream-releases

<<<<<<< HEAD
bool WebGLContext::ValidateAttribArraySetter(uint32_t setterElemSize,
                                             uint32_t arrayLength) {
  if (IsContextLost()) return false;
||||||| merged common ancestors
bool
WebGLContext::ValidateAttribArraySetter(uint32_t setterElemSize, uint32_t arrayLength)
{
    if (IsContextLost())
        return false;
=======
  if (!ValidateObjectAllowDeleted("loc", *loc)) return false;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (arrayLength < setterElemSize) {
    ErrorInvalidValue("Array must have >= %d elements.", setterElemSize);
    return false;
  }
||||||| merged common ancestors
    if (arrayLength < setterElemSize) {
        ErrorInvalidValue("Array must have >= %d elements.", setterElemSize);
        return false;
    }
=======
  if (!mCurrentProgram) {
    ErrorInvalidOperation("No program is currently bound.");
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  return loc->ValidateForProgram(mCurrentProgram);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool WebGLContext::ValidateUniformSetter(
    const WebGLUniformLocation* const loc, const uint8_t setterElemSize,
    const webgl::AttribBaseType setterType) {
  if (IsContextLost()) return false;
||||||| merged common ancestors
bool
WebGLContext::ValidateUniformSetter(const WebGLUniformLocation* const loc,
                                    const uint8_t setterElemSize,
                                    const webgl::AttribBaseType setterType)
{
    if (IsContextLost())
        return false;
=======
bool WebGLContext::ValidateAttribArraySetter(uint32_t setterElemSize,
                                             uint32_t arrayLength) {
  if (IsContextLost()) return false;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!ValidateUniformLocation(loc)) return false;

  if (!loc->ValidateSizeAndType(setterElemSize, setterType)) return false;
||||||| merged common ancestors
    if (!ValidateUniformLocation(loc))
        return false;

    if (!loc->ValidateSizeAndType(setterElemSize, setterType))
        return false;
=======
  if (arrayLength < setterElemSize) {
    ErrorInvalidValue("Array must have >= %d elements.", setterElemSize);
    return false;
  }
>>>>>>> upstream-releases

  return true;
}

<<<<<<< HEAD
bool WebGLContext::ValidateUniformArraySetter(
    const WebGLUniformLocation* const loc, const uint8_t setterElemSize,
    const webgl::AttribBaseType setterType, const uint32_t setterArraySize,
    uint32_t* const out_numElementsToUpload) {
  if (IsContextLost()) return false;

  if (!ValidateUniformLocation(loc)) return false;

  if (!loc->ValidateSizeAndType(setterElemSize, setterType)) return false;
||||||| merged common ancestors
bool
WebGLContext::ValidateUniformArraySetter(const WebGLUniformLocation* const loc,
                                         const uint8_t setterElemSize,
                                         const webgl::AttribBaseType setterType,
                                         const uint32_t setterArraySize,
                                         uint32_t* const out_numElementsToUpload)
{
    if (IsContextLost())
        return false;

    if (!ValidateUniformLocation(loc))
        return false;

    if (!loc->ValidateSizeAndType(setterElemSize, setterType))
        return false;
=======
bool WebGLContext::ValidateUniformSetter(
    const WebGLUniformLocation* const loc, const uint8_t setterElemSize,
    const webgl::AttribBaseType setterType) {
  if (IsContextLost()) return false;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!loc->ValidateArrayLength(setterElemSize, setterArraySize)) return false;
||||||| merged common ancestors
    if (!loc->ValidateArrayLength(setterElemSize, setterArraySize))
        return false;
=======
  if (!ValidateUniformLocation(loc)) return false;
>>>>>>> upstream-releases

<<<<<<< HEAD
  const auto& elemCount = loc->mInfo->mActiveInfo->mElemCount;
  MOZ_ASSERT(elemCount > loc->mArrayIndex);
  const uint32_t uniformElemCount = elemCount - loc->mArrayIndex;
||||||| merged common ancestors
    const auto& elemCount = loc->mInfo->mActiveInfo->mElemCount;
    MOZ_ASSERT(elemCount > loc->mArrayIndex);
    const uint32_t uniformElemCount = elemCount - loc->mArrayIndex;
=======
  if (!loc->ValidateSizeAndType(setterElemSize, setterType)) return false;
>>>>>>> upstream-releases

<<<<<<< HEAD
  *out_numElementsToUpload =
      std::min(uniformElemCount, setterArraySize / setterElemSize);
  return true;
||||||| merged common ancestors
    *out_numElementsToUpload = std::min(uniformElemCount,
                                        setterArraySize / setterElemSize);
    return true;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool WebGLContext::ValidateUniformMatrixArraySetter(
    const WebGLUniformLocation* const loc, const uint8_t setterCols,
    const uint8_t setterRows, const webgl::AttribBaseType setterType,
    const uint32_t setterArraySize, const bool setterTranspose,
    uint32_t* const out_numElementsToUpload) {
  const uint8_t setterElemSize = setterCols * setterRows;

  if (IsContextLost()) return false;
||||||| merged common ancestors
bool
WebGLContext::ValidateUniformMatrixArraySetter(const WebGLUniformLocation* const loc,
                                               const uint8_t setterCols,
                                               const uint8_t setterRows,
                                               const webgl::AttribBaseType setterType,
                                               const uint32_t setterArraySize,
                                               const bool setterTranspose,
                                               uint32_t* const out_numElementsToUpload)
{
    const uint8_t setterElemSize = setterCols * setterRows;

    if (IsContextLost())
        return false;
=======
bool WebGLContext::ValidateUniformArraySetter(
    const WebGLUniformLocation* const loc, const uint8_t setterElemSize,
    const webgl::AttribBaseType setterType, const uint32_t setterArraySize,
    uint32_t* const out_numElementsToUpload) {
  if (IsContextLost()) return false;
>>>>>>> upstream-releases

  if (!ValidateUniformLocation(loc)) return false;

  if (!loc->ValidateSizeAndType(setterElemSize, setterType)) return false;

  if (!loc->ValidateArrayLength(setterElemSize, setterArraySize)) return false;

<<<<<<< HEAD
  if (setterTranspose && !IsWebGL2()) {
    ErrorInvalidValue("`transpose` must be false.");
    return false;
  }
||||||| merged common ancestors
    if (setterTranspose && !IsWebGL2()) {
        ErrorInvalidValue("`transpose` must be false.");
        return false;
    }
=======
  const auto& elemCount = loc->mInfo->mActiveInfo->mElemCount;
  MOZ_ASSERT(elemCount > loc->mArrayIndex);
  const uint32_t uniformElemCount = elemCount - loc->mArrayIndex;
>>>>>>> upstream-releases

<<<<<<< HEAD
  const auto& elemCount = loc->mInfo->mActiveInfo->mElemCount;
  MOZ_ASSERT(elemCount > loc->mArrayIndex);
  const uint32_t uniformElemCount = elemCount - loc->mArrayIndex;

  *out_numElementsToUpload =
      std::min(uniformElemCount, setterArraySize / setterElemSize);
  return true;
||||||| merged common ancestors
    const auto& elemCount = loc->mInfo->mActiveInfo->mElemCount;
    MOZ_ASSERT(elemCount > loc->mArrayIndex);
    const uint32_t uniformElemCount = elemCount - loc->mArrayIndex;

    *out_numElementsToUpload = std::min(uniformElemCount,
                                        setterArraySize / setterElemSize);
    return true;
=======
  *out_numElementsToUpload =
      std::min(uniformElemCount, setterArraySize / setterElemSize);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool WebGLContext::InitAndValidateGL(FailureReason* const out_failReason) {
  MOZ_RELEASE_ASSERT(gl, "GFX: GL not initialized");
||||||| merged common ancestors
bool
WebGLContext::InitAndValidateGL(FailureReason* const out_failReason)
{
    MOZ_RELEASE_ASSERT(gl, "GFX: GL not initialized");

    // Unconditionally create a new format usage authority. This is
    // important when restoring contexts and extensions need to add
    // formats back into the authority.
    mFormatUsage = CreateFormatUsage(gl);
    if (!mFormatUsage) {
        *out_failReason = { "FEATURE_FAILURE_WEBGL_FORMAT",
                            "Failed to create mFormatUsage." };
        return false;
    }

    GLenum error = gl->fGetError();
    if (error != LOCAL_GL_NO_ERROR) {
        const nsPrintfCString reason("GL error 0x%x occurred during OpenGL context"
                                     " initialization, before WebGL initialization!",
                                     error);
        *out_failReason = { "FEATURE_FAILURE_WEBGL_GLERR_1", reason };
        return false;
    }

    mDisableExtensions = gfxPrefs::WebGLDisableExtensions();
    mLoseContextOnMemoryPressure = gfxPrefs::WebGLLoseContextOnMemoryPressure();
    mCanLoseContextInForeground = gfxPrefs::WebGLCanLoseContextInForeground();

    // These are the default values, see 6.2 State tables in the
    // OpenGL ES 2.0.25 spec.
    mColorWriteMask = 0x0f;
    mDriverColorMask = mColorWriteMask;
    mColorClearValue[0] = 0.f;
    mColorClearValue[1] = 0.f;
    mColorClearValue[2] = 0.f;
    mColorClearValue[3] = 0.f;
    mDepthWriteMask = true;
    mDepthClearValue = 1.f;
    mStencilClearValue = 0;
    mStencilRefFront = 0;
    mStencilRefBack = 0;

    mLineWidth = 1.0;

    /*
    // Technically, we should be setting mStencil[...] values to
    // `allOnes`, but either ANGLE breaks or the SGX540s on Try break.
    GLuint stencilBits = 0;
    gl->GetUIntegerv(LOCAL_GL_STENCIL_BITS, &stencilBits);
    GLuint allOnes = ~(UINT32_MAX << stencilBits);
    mStencilValueMaskFront = allOnes;
    mStencilValueMaskBack  = allOnes;
    mStencilWriteMaskFront = allOnes;
    mStencilWriteMaskBack  = allOnes;
    */

    gl->GetUIntegerv(LOCAL_GL_STENCIL_VALUE_MASK,      &mStencilValueMaskFront);
    gl->GetUIntegerv(LOCAL_GL_STENCIL_BACK_VALUE_MASK, &mStencilValueMaskBack);
    gl->GetUIntegerv(LOCAL_GL_STENCIL_WRITEMASK,       &mStencilWriteMaskFront);
    gl->GetUIntegerv(LOCAL_GL_STENCIL_BACK_WRITEMASK,  &mStencilWriteMaskBack);

    AssertUintParamCorrect(gl, LOCAL_GL_STENCIL_VALUE_MASK,      mStencilValueMaskFront);
    AssertUintParamCorrect(gl, LOCAL_GL_STENCIL_BACK_VALUE_MASK, mStencilValueMaskBack);
    AssertUintParamCorrect(gl, LOCAL_GL_STENCIL_WRITEMASK,       mStencilWriteMaskFront);
    AssertUintParamCorrect(gl, LOCAL_GL_STENCIL_BACK_WRITEMASK,  mStencilWriteMaskBack);

    mDitherEnabled = true;
    mRasterizerDiscardEnabled = false;
    mScissorTestEnabled = false;

    mDepthTestEnabled = 0;
    mDriverDepthTest = false;
    mStencilTestEnabled = 0;
    mDriverStencilTest = false;

    mGenerateMipmapHint = LOCAL_GL_DONT_CARE;

    // Bindings, etc.
    mActiveTexture = 0;
    mDefaultFB_DrawBuffer0 = LOCAL_GL_BACK;
    mDefaultFB_ReadBuffer = LOCAL_GL_BACK;

    mWebGLError = LOCAL_GL_NO_ERROR;

    mBound2DTextures.Clear();
    mBoundCubeMapTextures.Clear();
    mBound3DTextures.Clear();
    mBound2DArrayTextures.Clear();
    mBoundSamplers.Clear();

    mBoundArrayBuffer = nullptr;
    mCurrentProgram = nullptr;

    mBoundDrawFramebuffer = nullptr;
    mBoundReadFramebuffer = nullptr;
    mBoundRenderbuffer = nullptr;

    gl->GetUIntegerv(LOCAL_GL_MAX_VERTEX_ATTRIBS, &mGLMaxVertexAttribs);

    if (mGLMaxVertexAttribs < 8) {
        const nsPrintfCString reason("GL_MAX_VERTEX_ATTRIBS: %d is < 8!",
                                     mGLMaxVertexAttribs);
        *out_failReason = { "FEATURE_FAILURE_WEBGL_V_ATRB", reason };
        return false;
    }

    // Note: GL_MAX_TEXTURE_UNITS is fixed at 4 for most desktop hardware,
    // even though the hardware supports much more.  The
    // GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS value is the accurate value.
    mGLMaxCombinedTextureImageUnits = gl->GetIntAs<GLuint>(LOCAL_GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
    mGLMaxTextureUnits = mGLMaxCombinedTextureImageUnits;

    if (mGLMaxCombinedTextureImageUnits < 8) {
        const nsPrintfCString reason("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: %u is < 8!",
                                     mGLMaxTextureUnits);
        *out_failReason = { "FEATURE_FAILURE_WEBGL_T_UNIT", reason };
        return false;
    }

    mBound2DTextures.SetLength(mGLMaxTextureUnits);
    mBoundCubeMapTextures.SetLength(mGLMaxTextureUnits);
    mBound3DTextures.SetLength(mGLMaxTextureUnits);
    mBound2DArrayTextures.SetLength(mGLMaxTextureUnits);
    mBoundSamplers.SetLength(mGLMaxTextureUnits);

    gl->fGetIntegerv(LOCAL_GL_MAX_VIEWPORT_DIMS, (GLint*)mGLMaxViewportDims);

    ////////////////

    gl->fGetIntegerv(LOCAL_GL_MAX_TEXTURE_SIZE, (GLint*)&mGLMaxTextureSize);
    gl->fGetIntegerv(LOCAL_GL_MAX_CUBE_MAP_TEXTURE_SIZE, (GLint*)&mGLMaxCubeMapTextureSize);
    gl->fGetIntegerv(LOCAL_GL_MAX_RENDERBUFFER_SIZE, (GLint*)&mGLMaxRenderbufferSize);

    if (!gl->GetPotentialInteger(LOCAL_GL_MAX_3D_TEXTURE_SIZE, (GLint*)&mGLMax3DTextureSize))
        mGLMax3DTextureSize = 0;
    if (!gl->GetPotentialInteger(LOCAL_GL_MAX_ARRAY_TEXTURE_LAYERS, (GLint*)&mGLMaxArrayTextureLayers))
        mGLMaxArrayTextureLayers = 0;

    gl->GetUIntegerv(LOCAL_GL_MAX_TEXTURE_IMAGE_UNITS, &mGLMaxFragmentTextureImageUnits);
    gl->GetUIntegerv(LOCAL_GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &mGLMaxVertexTextureImageUnits);

    ////////////////
=======
bool WebGLContext::ValidateUniformMatrixArraySetter(
    const WebGLUniformLocation* const loc, const uint8_t setterCols,
    const uint8_t setterRows, const webgl::AttribBaseType setterType,
    const uint32_t setterArraySize, const bool setterTranspose,
    uint32_t* const out_numElementsToUpload) {
  const uint8_t setterElemSize = setterCols * setterRows;

  if (IsContextLost()) return false;

  if (!ValidateUniformLocation(loc)) return false;

  if (!loc->ValidateSizeAndType(setterElemSize, setterType)) return false;

  if (!loc->ValidateArrayLength(setterElemSize, setterArraySize)) return false;

  if (setterTranspose && !IsWebGL2()) {
    ErrorInvalidValue("`transpose` must be false.");
    return false;
  }

  const auto& elemCount = loc->mInfo->mActiveInfo->mElemCount;
  MOZ_ASSERT(elemCount > loc->mArrayIndex);
  const uint32_t uniformElemCount = elemCount - loc->mArrayIndex;

  *out_numElementsToUpload =
      std::min(uniformElemCount, setterArraySize / setterElemSize);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Unconditionally create a new format usage authority. This is
  // important when restoring contexts and extensions need to add
  // formats back into the authority.
  mFormatUsage = CreateFormatUsage(gl);
  if (!mFormatUsage) {
    *out_failReason = {"FEATURE_FAILURE_WEBGL_FORMAT",
                       "Failed to create mFormatUsage."};
    return false;
  }

  GLenum error = gl->fGetError();
  if (error != LOCAL_GL_NO_ERROR) {
    const nsPrintfCString reason(
        "GL error 0x%x occurred during OpenGL context"
        " initialization, before WebGL initialization!",
        error);
    *out_failReason = {"FEATURE_FAILURE_WEBGL_GLERR_1", reason};
    return false;
  }

  mDisableExtensions = gfxPrefs::WebGLDisableExtensions();
  mLoseContextOnMemoryPressure = gfxPrefs::WebGLLoseContextOnMemoryPressure();
  mCanLoseContextInForeground = gfxPrefs::WebGLCanLoseContextInForeground();

  // These are the default values, see 6.2 State tables in the
  // OpenGL ES 2.0.25 spec.
  mColorWriteMask = 0x0f;
  mDriverColorMask = mColorWriteMask;
  mColorClearValue[0] = 0.f;
  mColorClearValue[1] = 0.f;
  mColorClearValue[2] = 0.f;
  mColorClearValue[3] = 0.f;
  mDepthWriteMask = true;
  mDepthClearValue = 1.f;
  mStencilClearValue = 0;
  mStencilRefFront = 0;
  mStencilRefBack = 0;

  mLineWidth = 1.0;

  /*
  // Technically, we should be setting mStencil[...] values to
  // `allOnes`, but either ANGLE breaks or the SGX540s on Try break.
  GLuint stencilBits = 0;
  gl->GetUIntegerv(LOCAL_GL_STENCIL_BITS, &stencilBits);
  GLuint allOnes = ~(UINT32_MAX << stencilBits);
  mStencilValueMaskFront = allOnes;
  mStencilValueMaskBack  = allOnes;
  mStencilWriteMaskFront = allOnes;
  mStencilWriteMaskBack  = allOnes;
  */

  gl->GetUIntegerv(LOCAL_GL_STENCIL_VALUE_MASK, &mStencilValueMaskFront);
  gl->GetUIntegerv(LOCAL_GL_STENCIL_BACK_VALUE_MASK, &mStencilValueMaskBack);
  gl->GetUIntegerv(LOCAL_GL_STENCIL_WRITEMASK, &mStencilWriteMaskFront);
  gl->GetUIntegerv(LOCAL_GL_STENCIL_BACK_WRITEMASK, &mStencilWriteMaskBack);

  AssertUintParamCorrect(gl, LOCAL_GL_STENCIL_VALUE_MASK,
                         mStencilValueMaskFront);
  AssertUintParamCorrect(gl, LOCAL_GL_STENCIL_BACK_VALUE_MASK,
                         mStencilValueMaskBack);
  AssertUintParamCorrect(gl, LOCAL_GL_STENCIL_WRITEMASK,
                         mStencilWriteMaskFront);
  AssertUintParamCorrect(gl, LOCAL_GL_STENCIL_BACK_WRITEMASK,
                         mStencilWriteMaskBack);

  mDitherEnabled = true;
  mRasterizerDiscardEnabled = false;
  mScissorTestEnabled = false;

  mDepthTestEnabled = 0;
  mDriverDepthTest = false;
  mStencilTestEnabled = 0;
  mDriverStencilTest = false;

  mGenerateMipmapHint = LOCAL_GL_DONT_CARE;

  // Bindings, etc.
  mActiveTexture = 0;
  mDefaultFB_DrawBuffer0 = LOCAL_GL_BACK;
  mDefaultFB_ReadBuffer = LOCAL_GL_BACK;

  mWebGLError = LOCAL_GL_NO_ERROR;

  mBound2DTextures.Clear();
  mBoundCubeMapTextures.Clear();
  mBound3DTextures.Clear();
  mBound2DArrayTextures.Clear();
  mBoundSamplers.Clear();

  mBoundArrayBuffer = nullptr;
  mCurrentProgram = nullptr;

  mBoundDrawFramebuffer = nullptr;
  mBoundReadFramebuffer = nullptr;
  mBoundRenderbuffer = nullptr;

  gl->GetUIntegerv(LOCAL_GL_MAX_VERTEX_ATTRIBS, &mGLMaxVertexAttribs);

  if (mGLMaxVertexAttribs < 8) {
    const nsPrintfCString reason("GL_MAX_VERTEX_ATTRIBS: %d is < 8!",
                                 mGLMaxVertexAttribs);
    *out_failReason = {"FEATURE_FAILURE_WEBGL_V_ATRB", reason};
    return false;
  }

  // Note: GL_MAX_TEXTURE_UNITS is fixed at 4 for most desktop hardware,
  // even though the hardware supports much more.  The
  // GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS value is the accurate value.
  mGLMaxCombinedTextureImageUnits =
      gl->GetIntAs<GLuint>(LOCAL_GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
  mGLMaxTextureUnits = mGLMaxCombinedTextureImageUnits;

  if (mGLMaxCombinedTextureImageUnits < 8) {
    const nsPrintfCString reason(
        "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: %u is < 8!", mGLMaxTextureUnits);
    *out_failReason = {"FEATURE_FAILURE_WEBGL_T_UNIT", reason};
    return false;
  }

  mBound2DTextures.SetLength(mGLMaxTextureUnits);
  mBoundCubeMapTextures.SetLength(mGLMaxTextureUnits);
  mBound3DTextures.SetLength(mGLMaxTextureUnits);
  mBound2DArrayTextures.SetLength(mGLMaxTextureUnits);
  mBoundSamplers.SetLength(mGLMaxTextureUnits);

  gl->fGetIntegerv(LOCAL_GL_MAX_VIEWPORT_DIMS, (GLint*)mGLMaxViewportDims);

  ////////////////

  gl->fGetIntegerv(LOCAL_GL_MAX_TEXTURE_SIZE, (GLint*)&mGLMaxTextureSize);
  gl->fGetIntegerv(LOCAL_GL_MAX_CUBE_MAP_TEXTURE_SIZE,
                   (GLint*)&mGLMaxCubeMapTextureSize);
  gl->fGetIntegerv(LOCAL_GL_MAX_RENDERBUFFER_SIZE,
                   (GLint*)&mGLMaxRenderbufferSize);

  if (!gl->GetPotentialInteger(LOCAL_GL_MAX_3D_TEXTURE_SIZE,
                               (GLint*)&mGLMax3DTextureSize))
    mGLMax3DTextureSize = 0;
  if (!gl->GetPotentialInteger(LOCAL_GL_MAX_ARRAY_TEXTURE_LAYERS,
                               (GLint*)&mGLMaxArrayTextureLayers))
    mGLMaxArrayTextureLayers = 0;

  gl->GetUIntegerv(LOCAL_GL_MAX_TEXTURE_IMAGE_UNITS,
                   &mGLMaxFragmentTextureImageUnits);
  gl->GetUIntegerv(LOCAL_GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
                   &mGLMaxVertexTextureImageUnits);

  ////////////////

  mGLMaxColorAttachments = 1;
  mGLMaxDrawBuffers = 1;

  if (IsWebGL2()) {
    UpdateMaxDrawBuffers();
  }

  ////////////////

  if (gl->IsSupported(gl::GLFeature::ES2_compatibility)) {
    gl->GetUIntegerv(LOCAL_GL_MAX_FRAGMENT_UNIFORM_VECTORS,
                     &mGLMaxFragmentUniformVectors);
    gl->GetUIntegerv(LOCAL_GL_MAX_VERTEX_UNIFORM_VECTORS,
                     &mGLMaxVertexUniformVectors);
    gl->GetUIntegerv(LOCAL_GL_MAX_VARYING_VECTORS, &mGLMaxVaryingVectors);
  } else {
    gl->GetUIntegerv(LOCAL_GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
                     &mGLMaxFragmentUniformVectors);
    mGLMaxFragmentUniformVectors /= 4;
    gl->GetUIntegerv(LOCAL_GL_MAX_VERTEX_UNIFORM_COMPONENTS,
                     &mGLMaxVertexUniformVectors);
    mGLMaxVertexUniformVectors /= 4;

    /* We are now going to try to read GL_MAX_VERTEX_OUTPUT_COMPONENTS
     * and GL_MAX_FRAGMENT_INPUT_COMPONENTS, however these constants
     * only entered the OpenGL standard at OpenGL 3.2. So we will try
     * reading, and check OpenGL error for INVALID_ENUM.
     *
     * On the public_webgl list, "problematic GetParameter pnames"
     * thread, the following formula was given:
     *   maxVaryingVectors = min(GL_MAX_VERTEX_OUTPUT_COMPONENTS,
     *                           GL_MAX_FRAGMENT_INPUT_COMPONENTS) / 4
     */
    uint32_t maxVertexOutputComponents = 0;
    uint32_t maxFragmentInputComponents = 0;
    bool ok = true;
    ok &= gl->GetPotentialInteger(LOCAL_GL_MAX_VERTEX_OUTPUT_COMPONENTS,
                                  (GLint*)&maxVertexOutputComponents);
    ok &= gl->GetPotentialInteger(LOCAL_GL_MAX_FRAGMENT_INPUT_COMPONENTS,
                                  (GLint*)&maxFragmentInputComponents);
    if (ok) {
      mGLMaxVaryingVectors =
          std::min(maxVertexOutputComponents, maxFragmentInputComponents) / 4;
||||||| merged common ancestors
    mGLMaxColorAttachments = 1;
    mGLMaxDrawBuffers = 1;

    if (IsWebGL2()) {
        UpdateMaxDrawBuffers();
    }

    ////////////////

    if (gl->IsSupported(gl::GLFeature::ES2_compatibility)) {
        gl->GetUIntegerv(LOCAL_GL_MAX_FRAGMENT_UNIFORM_VECTORS, &mGLMaxFragmentUniformVectors);
        gl->GetUIntegerv(LOCAL_GL_MAX_VERTEX_UNIFORM_VECTORS, &mGLMaxVertexUniformVectors);
        gl->GetUIntegerv(LOCAL_GL_MAX_VARYING_VECTORS, &mGLMaxVaryingVectors);
=======
bool WebGLContext::InitAndValidateGL(FailureReason* const out_failReason) {
  MOZ_RELEASE_ASSERT(gl, "GFX: GL not initialized");

  // Unconditionally create a new format usage authority. This is
  // important when restoring contexts and extensions need to add
  // formats back into the authority.
  mFormatUsage = CreateFormatUsage(gl);
  if (!mFormatUsage) {
    *out_failReason = {"FEATURE_FAILURE_WEBGL_FORMAT",
                       "Failed to create mFormatUsage."};
    return false;
  }

  GLenum error = gl->fGetError();
  if (error != LOCAL_GL_NO_ERROR) {
    const nsPrintfCString reason(
        "GL error 0x%x occurred during OpenGL context"
        " initialization, before WebGL initialization!",
        error);
    *out_failReason = {"FEATURE_FAILURE_WEBGL_GLERR_1", reason};
    return false;
  }

  mDisableExtensions = StaticPrefs::webgl_disable_extensions();
  mLoseContextOnMemoryPressure =
      StaticPrefs::webgl_lose_context_on_memory_pressure();
  mCanLoseContextInForeground =
      StaticPrefs::webgl_can_lose_context_in_foreground();

  // These are the default values, see 6.2 State tables in the
  // OpenGL ES 2.0.25 spec.
  mColorWriteMask = 0x0f;
  mDriverColorMask = mColorWriteMask;
  mColorClearValue[0] = 0.f;
  mColorClearValue[1] = 0.f;
  mColorClearValue[2] = 0.f;
  mColorClearValue[3] = 0.f;
  mDepthWriteMask = true;
  mDepthClearValue = 1.f;
  mStencilClearValue = 0;
  mStencilRefFront = 0;
  mStencilRefBack = 0;

  mLineWidth = 1.0;

  /*
  // Technically, we should be setting mStencil[...] values to
  // `allOnes`, but either ANGLE breaks or the SGX540s on Try break.
  GLuint stencilBits = 0;
  gl->GetUIntegerv(LOCAL_GL_STENCIL_BITS, &stencilBits);
  GLuint allOnes = ~(UINT32_MAX << stencilBits);
  mStencilValueMaskFront = allOnes;
  mStencilValueMaskBack  = allOnes;
  mStencilWriteMaskFront = allOnes;
  mStencilWriteMaskBack  = allOnes;
  */

  gl->GetUIntegerv(LOCAL_GL_STENCIL_VALUE_MASK, &mStencilValueMaskFront);
  gl->GetUIntegerv(LOCAL_GL_STENCIL_BACK_VALUE_MASK, &mStencilValueMaskBack);
  gl->GetUIntegerv(LOCAL_GL_STENCIL_WRITEMASK, &mStencilWriteMaskFront);
  gl->GetUIntegerv(LOCAL_GL_STENCIL_BACK_WRITEMASK, &mStencilWriteMaskBack);

  AssertUintParamCorrect(gl, LOCAL_GL_STENCIL_VALUE_MASK,
                         mStencilValueMaskFront);
  AssertUintParamCorrect(gl, LOCAL_GL_STENCIL_BACK_VALUE_MASK,
                         mStencilValueMaskBack);
  AssertUintParamCorrect(gl, LOCAL_GL_STENCIL_WRITEMASK,
                         mStencilWriteMaskFront);
  AssertUintParamCorrect(gl, LOCAL_GL_STENCIL_BACK_WRITEMASK,
                         mStencilWriteMaskBack);

  mDitherEnabled = true;
  mRasterizerDiscardEnabled = false;
  mScissorTestEnabled = false;

  mDepthTestEnabled = 0;
  mDriverDepthTest = false;
  mStencilTestEnabled = 0;
  mDriverStencilTest = false;

  mGenerateMipmapHint = LOCAL_GL_DONT_CARE;

  // Bindings, etc.
  mActiveTexture = 0;
  mDefaultFB_DrawBuffer0 = LOCAL_GL_BACK;
  mDefaultFB_ReadBuffer = LOCAL_GL_BACK;

  mWebGLError = LOCAL_GL_NO_ERROR;

  mBound2DTextures.Clear();
  mBoundCubeMapTextures.Clear();
  mBound3DTextures.Clear();
  mBound2DArrayTextures.Clear();
  mBoundSamplers.Clear();

  mBoundArrayBuffer = nullptr;
  mCurrentProgram = nullptr;

  mBoundDrawFramebuffer = nullptr;
  mBoundReadFramebuffer = nullptr;
  mBoundRenderbuffer = nullptr;

  gl->GetUIntegerv(LOCAL_GL_MAX_VERTEX_ATTRIBS, &mGLMaxVertexAttribs);

  if (mGLMaxVertexAttribs < 8) {
    const nsPrintfCString reason("GL_MAX_VERTEX_ATTRIBS: %d is < 8!",
                                 mGLMaxVertexAttribs);
    *out_failReason = {"FEATURE_FAILURE_WEBGL_V_ATRB", reason};
    return false;
  }

  // Note: GL_MAX_TEXTURE_UNITS is fixed at 4 for most desktop hardware,
  // even though the hardware supports much more.  The
  // GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS value is the accurate value.
  mGLMaxCombinedTextureImageUnits =
      gl->GetIntAs<GLuint>(LOCAL_GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
  mGLMaxTextureUnits = mGLMaxCombinedTextureImageUnits;

  if (mGLMaxCombinedTextureImageUnits < 8) {
    const nsPrintfCString reason(
        "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: %u is < 8!", mGLMaxTextureUnits);
    *out_failReason = {"FEATURE_FAILURE_WEBGL_T_UNIT", reason};
    return false;
  }

  mBound2DTextures.SetLength(mGLMaxTextureUnits);
  mBoundCubeMapTextures.SetLength(mGLMaxTextureUnits);
  mBound3DTextures.SetLength(mGLMaxTextureUnits);
  mBound2DArrayTextures.SetLength(mGLMaxTextureUnits);
  mBoundSamplers.SetLength(mGLMaxTextureUnits);

  gl->fGetIntegerv(LOCAL_GL_MAX_VIEWPORT_DIMS, (GLint*)mGLMaxViewportDims);

  ////////////////

  gl->fGetIntegerv(LOCAL_GL_MAX_TEXTURE_SIZE, (GLint*)&mGLMaxTextureSize);
  gl->fGetIntegerv(LOCAL_GL_MAX_CUBE_MAP_TEXTURE_SIZE,
                   (GLint*)&mGLMaxCubeMapTextureSize);
  gl->fGetIntegerv(LOCAL_GL_MAX_RENDERBUFFER_SIZE,
                   (GLint*)&mGLMaxRenderbufferSize);

  if (!gl->GetPotentialInteger(LOCAL_GL_MAX_3D_TEXTURE_SIZE,
                               (GLint*)&mGLMax3DTextureSize))
    mGLMax3DTextureSize = 0;
  if (!gl->GetPotentialInteger(LOCAL_GL_MAX_ARRAY_TEXTURE_LAYERS,
                               (GLint*)&mGLMaxArrayTextureLayers))
    mGLMaxArrayTextureLayers = 0;

  (void)gl->GetPotentialInteger(LOCAL_GL_MAX_VIEWS_OVR,
                                (GLint*)&mGLMaxMultiviewViews);

  gl->GetUIntegerv(LOCAL_GL_MAX_TEXTURE_IMAGE_UNITS,
                   &mGLMaxFragmentTextureImageUnits);
  gl->GetUIntegerv(LOCAL_GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
                   &mGLMaxVertexTextureImageUnits);

  ////////////////

  mGLMaxColorAttachments = 1;
  mGLMaxDrawBuffers = 1;

  if (IsWebGL2()) {
    UpdateMaxDrawBuffers();
  }

  ////////////////

  if (gl->IsGLES()) {
    mGLMaxFragmentUniformVectors =
        gl->GetIntAs<uint32_t>(LOCAL_GL_MAX_FRAGMENT_UNIFORM_VECTORS);
    mGLMaxVertexUniformVectors =
        gl->GetIntAs<uint32_t>(LOCAL_GL_MAX_VERTEX_UNIFORM_VECTORS);
    if (gl->Version() >= 300) {
      mGLMaxVertexOutputVectors =
          gl->GetIntAs<uint32_t>(LOCAL_GL_MAX_VERTEX_OUTPUT_COMPONENTS) / 4;
      mGLMaxFragmentInputVectors =
          gl->GetIntAs<uint32_t>(LOCAL_GL_MAX_FRAGMENT_INPUT_COMPONENTS) / 4;
    } else {
      mGLMaxFragmentInputVectors =
          gl->GetIntAs<uint32_t>(LOCAL_GL_MAX_VARYING_VECTORS);
      mGLMaxVertexOutputVectors = mGLMaxFragmentInputVectors;
    }
  } else {
    mGLMaxFragmentUniformVectors =
        gl->GetIntAs<uint32_t>(LOCAL_GL_MAX_FRAGMENT_UNIFORM_COMPONENTS) / 4;
    mGLMaxVertexUniformVectors =
        gl->GetIntAs<uint32_t>(LOCAL_GL_MAX_VERTEX_UNIFORM_COMPONENTS) / 4;

    if (gl->Version() >= 320) {
      mGLMaxVertexOutputVectors =
          gl->GetIntAs<uint32_t>(LOCAL_GL_MAX_VERTEX_OUTPUT_COMPONENTS) / 4;
      mGLMaxFragmentInputVectors =
          gl->GetIntAs<uint32_t>(LOCAL_GL_MAX_FRAGMENT_INPUT_COMPONENTS) / 4;
>>>>>>> upstream-releases
    } else {
<<<<<<< HEAD
      mGLMaxVaryingVectors = 16;
      // 16 = 64/4, and 64 is the min value for
      // maxVertexOutputComponents in the OpenGL 3.2 spec.
||||||| merged common ancestors
        gl->GetUIntegerv(LOCAL_GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &mGLMaxFragmentUniformVectors);
        mGLMaxFragmentUniformVectors /= 4;
        gl->GetUIntegerv(LOCAL_GL_MAX_VERTEX_UNIFORM_COMPONENTS, &mGLMaxVertexUniformVectors);
        mGLMaxVertexUniformVectors /= 4;

        /* We are now going to try to read GL_MAX_VERTEX_OUTPUT_COMPONENTS
         * and GL_MAX_FRAGMENT_INPUT_COMPONENTS, however these constants
         * only entered the OpenGL standard at OpenGL 3.2. So we will try
         * reading, and check OpenGL error for INVALID_ENUM.
         *
         * On the public_webgl list, "problematic GetParameter pnames"
         * thread, the following formula was given:
         *   maxVaryingVectors = min(GL_MAX_VERTEX_OUTPUT_COMPONENTS,
         *                           GL_MAX_FRAGMENT_INPUT_COMPONENTS) / 4
         */
        uint32_t maxVertexOutputComponents = 0;
        uint32_t maxFragmentInputComponents = 0;
        bool ok = true;
        ok &= gl->GetPotentialInteger(LOCAL_GL_MAX_VERTEX_OUTPUT_COMPONENTS,
                                      (GLint*)&maxVertexOutputComponents);
        ok &= gl->GetPotentialInteger(LOCAL_GL_MAX_FRAGMENT_INPUT_COMPONENTS,
                                      (GLint*)&maxFragmentInputComponents);
        if (ok) {
            mGLMaxVaryingVectors = std::min(maxVertexOutputComponents,
                                            maxFragmentInputComponents) / 4;
        } else {
            mGLMaxVaryingVectors = 16;
            // 16 = 64/4, and 64 is the min value for
            // maxVertexOutputComponents in the OpenGL 3.2 spec.
        }
=======
      // Same enum val as GL2's GL_MAX_VARYING_FLOATS.
      mGLMaxFragmentInputVectors =
          gl->GetIntAs<uint32_t>(LOCAL_GL_MAX_VARYING_COMPONENTS) / 4;
      mGLMaxVertexOutputVectors = mGLMaxFragmentInputVectors;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  ////////////////

  gl->fGetFloatv(LOCAL_GL_ALIASED_LINE_WIDTH_RANGE, mGLAliasedLineWidthRange);

  const GLenum driverPName = gl->IsCoreProfile()
                                 ? LOCAL_GL_POINT_SIZE_RANGE
                                 : LOCAL_GL_ALIASED_POINT_SIZE_RANGE;
  gl->fGetFloatv(driverPName, mGLAliasedPointSizeRange);
||||||| merged common ancestors
    ////////////////

    gl->fGetFloatv(LOCAL_GL_ALIASED_LINE_WIDTH_RANGE, mGLAliasedLineWidthRange);

    const GLenum driverPName = gl->IsCoreProfile() ? LOCAL_GL_POINT_SIZE_RANGE
                                                   : LOCAL_GL_ALIASED_POINT_SIZE_RANGE;
    gl->fGetFloatv(driverPName, mGLAliasedPointSizeRange);

    ////////////////
=======
  ////////////////
>>>>>>> upstream-releases

<<<<<<< HEAD
  ////////////////
||||||| merged common ancestors
    if (gfxPrefs::WebGLMinCapabilityMode()) {
        bool ok = true;
=======
  gl->fGetFloatv(LOCAL_GL_ALIASED_LINE_WIDTH_RANGE, mGLAliasedLineWidthRange);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (gfxPrefs::WebGLMinCapabilityMode()) {
    bool ok = true;
||||||| merged common ancestors
        ok &= RestrictCap(&mGLMaxVertexTextureImageUnits  , kMinMaxVertexTextureImageUnits);
        ok &= RestrictCap(&mGLMaxFragmentTextureImageUnits, kMinMaxFragmentTextureImageUnits);
        ok &= RestrictCap(&mGLMaxCombinedTextureImageUnits, kMinMaxCombinedTextureImageUnits);
=======
  const GLenum driverPName = gl->IsCoreProfile()
                                 ? LOCAL_GL_POINT_SIZE_RANGE
                                 : LOCAL_GL_ALIASED_POINT_SIZE_RANGE;
  gl->fGetFloatv(driverPName, mGLAliasedPointSizeRange);
>>>>>>> upstream-releases

<<<<<<< HEAD
    ok &= RestrictCap(&mGLMaxVertexTextureImageUnits,
                      kMinMaxVertexTextureImageUnits);
    ok &= RestrictCap(&mGLMaxFragmentTextureImageUnits,
                      kMinMaxFragmentTextureImageUnits);
    ok &= RestrictCap(&mGLMaxCombinedTextureImageUnits,
                      kMinMaxCombinedTextureImageUnits);
||||||| merged common ancestors
        ok &= RestrictCap(&mGLMaxVertexAttribs         , kMinMaxVertexAttribs);
        ok &= RestrictCap(&mGLMaxVertexUniformVectors  , kMinMaxVertexUniformVectors);
        ok &= RestrictCap(&mGLMaxFragmentUniformVectors, kMinMaxFragmentUniformVectors);
        ok &= RestrictCap(&mGLMaxVaryingVectors        , kMinMaxVaryingVectors);
=======
  ////////////////
>>>>>>> upstream-releases

<<<<<<< HEAD
    ok &= RestrictCap(&mGLMaxVertexAttribs, kMinMaxVertexAttribs);
    ok &= RestrictCap(&mGLMaxVertexUniformVectors, kMinMaxVertexUniformVectors);
    ok &= RestrictCap(&mGLMaxFragmentUniformVectors,
                      kMinMaxFragmentUniformVectors);
    ok &= RestrictCap(&mGLMaxVaryingVectors, kMinMaxVaryingVectors);
||||||| merged common ancestors
        ok &= RestrictCap(&mGLMaxColorAttachments, kMinMaxColorAttachments);
        ok &= RestrictCap(&mGLMaxDrawBuffers     , kMinMaxDrawBuffers);
=======
  if (StaticPrefs::webgl_min_capability_mode()) {
    bool ok = true;
>>>>>>> upstream-releases

<<<<<<< HEAD
    ok &= RestrictCap(&mGLMaxColorAttachments, kMinMaxColorAttachments);
    ok &= RestrictCap(&mGLMaxDrawBuffers, kMinMaxDrawBuffers);
||||||| merged common ancestors
        ok &= RestrictCap(&mGLMaxTextureSize       , kMinMaxTextureSize);
        ok &= RestrictCap(&mGLMaxCubeMapTextureSize, kMinMaxCubeMapTextureSize);
        ok &= RestrictCap(&mGLMax3DTextureSize     , kMinMax3DTextureSize);
=======
    ok &= RestrictCap(&mGLMaxVertexTextureImageUnits,
                      kMinMaxVertexTextureImageUnits);
    ok &= RestrictCap(&mGLMaxFragmentTextureImageUnits,
                      kMinMaxFragmentTextureImageUnits);
    ok &= RestrictCap(&mGLMaxCombinedTextureImageUnits,
                      kMinMaxCombinedTextureImageUnits);
>>>>>>> upstream-releases

<<<<<<< HEAD
    ok &= RestrictCap(&mGLMaxTextureSize, kMinMaxTextureSize);
    ok &= RestrictCap(&mGLMaxCubeMapTextureSize, kMinMaxCubeMapTextureSize);
    ok &= RestrictCap(&mGLMax3DTextureSize, kMinMax3DTextureSize);
||||||| merged common ancestors
        ok &= RestrictCap(&mGLMaxArrayTextureLayers, kMinMaxArrayTextureLayers);
        ok &= RestrictCap(&mGLMaxRenderbufferSize  , kMinMaxRenderbufferSize);
=======
    ok &= RestrictCap(&mGLMaxVertexAttribs, kMinMaxVertexAttribs);
    ok &= RestrictCap(&mGLMaxVertexUniformVectors, kMinMaxVertexUniformVectors);
    ok &= RestrictCap(&mGLMaxFragmentUniformVectors,
                      kMinMaxFragmentUniformVectors);
    ok &= RestrictCap(&mGLMaxVertexOutputVectors, kMinMaxVaryingVectors);
    ok &= RestrictCap(&mGLMaxFragmentInputVectors, kMinMaxVaryingVectors);
>>>>>>> upstream-releases

<<<<<<< HEAD
    ok &= RestrictCap(&mGLMaxArrayTextureLayers, kMinMaxArrayTextureLayers);
    ok &= RestrictCap(&mGLMaxRenderbufferSize, kMinMaxRenderbufferSize);
||||||| merged common ancestors
        if (!ok) {
            GenerateWarning("Unable to restrict WebGL limits to minimums.");
            return false;
        }
=======
    ok &= RestrictCap(&mGLMaxColorAttachments, kMinMaxColorAttachments);
    ok &= RestrictCap(&mGLMaxDrawBuffers, kMinMaxDrawBuffers);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!ok) {
      GenerateWarning("Unable to restrict WebGL limits to minimums.");
      return false;
    }
||||||| merged common ancestors
        mDisableFragHighP = true;
    } else if (nsContentUtils::ShouldResistFingerprinting()) {
        bool ok = true;

        ok &= RestrictCap(&mGLMaxTextureSize       , kCommonMaxTextureSize);
        ok &= RestrictCap(&mGLMaxCubeMapTextureSize, kCommonMaxCubeMapTextureSize);
        ok &= RestrictCap(&mGLMaxRenderbufferSize  , kCommonMaxRenderbufferSize);

        ok &= RestrictCap(&mGLMaxVertexTextureImageUnits  , kCommonMaxVertexTextureImageUnits);
        ok &= RestrictCap(&mGLMaxFragmentTextureImageUnits, kCommonMaxFragmentTextureImageUnits);
        ok &= RestrictCap(&mGLMaxCombinedTextureImageUnits, kCommonMaxCombinedTextureImageUnits);

        ok &= RestrictCap(&mGLMaxVertexAttribs         , kCommonMaxVertexAttribs);
        ok &= RestrictCap(&mGLMaxVertexUniformVectors  , kCommonMaxVertexUniformVectors);
        ok &= RestrictCap(&mGLMaxFragmentUniformVectors, kCommonMaxFragmentUniformVectors);
        ok &= RestrictCap(&mGLMaxVaryingVectors        , kCommonMaxVaryingVectors);

        ok &= RestrictCap(&mGLAliasedLineWidthRange[0], kCommonAliasedLineWidthRangeMin);
        ok &= RestrictCap(&mGLAliasedLineWidthRange[1], kCommonAliasedLineWidthRangeMax);
        ok &= RestrictCap(&mGLAliasedPointSizeRange[0], kCommonAliasedPointSizeRangeMin);
        ok &= RestrictCap(&mGLAliasedPointSizeRange[1], kCommonAliasedPointSizeRangeMax);

        ok &= RestrictCap(&mGLMaxViewportDims[0], kCommonMaxViewportDims);
        ok &= RestrictCap(&mGLMaxViewportDims[1], kCommonMaxViewportDims);
=======
    ok &= RestrictCap(&mGLMaxTextureSize, kMinMaxTextureSize);
    ok &= RestrictCap(&mGLMaxCubeMapTextureSize, kMinMaxCubeMapTextureSize);
    ok &= RestrictCap(&mGLMax3DTextureSize, kMinMax3DTextureSize);

    ok &= RestrictCap(&mGLMaxArrayTextureLayers, kMinMaxArrayTextureLayers);
    ok &= RestrictCap(&mGLMaxRenderbufferSize, kMinMaxRenderbufferSize);

    if (!ok) {
      GenerateWarning("Unable to restrict WebGL limits to minimums.");
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    mDisableFragHighP = true;
  } else if (nsContentUtils::ShouldResistFingerprinting()) {
    bool ok = true;

    ok &= RestrictCap(&mGLMaxTextureSize, kCommonMaxTextureSize);
    ok &= RestrictCap(&mGLMaxCubeMapTextureSize, kCommonMaxCubeMapTextureSize);
    ok &= RestrictCap(&mGLMaxRenderbufferSize, kCommonMaxRenderbufferSize);

    ok &= RestrictCap(&mGLMaxVertexTextureImageUnits,
                      kCommonMaxVertexTextureImageUnits);
    ok &= RestrictCap(&mGLMaxFragmentTextureImageUnits,
                      kCommonMaxFragmentTextureImageUnits);
    ok &= RestrictCap(&mGLMaxCombinedTextureImageUnits,
                      kCommonMaxCombinedTextureImageUnits);

    ok &= RestrictCap(&mGLMaxVertexAttribs, kCommonMaxVertexAttribs);
    ok &= RestrictCap(&mGLMaxVertexUniformVectors,
                      kCommonMaxVertexUniformVectors);
    ok &= RestrictCap(&mGLMaxFragmentUniformVectors,
                      kCommonMaxFragmentUniformVectors);
    ok &= RestrictCap(&mGLMaxVaryingVectors, kCommonMaxVaryingVectors);

    ok &= RestrictCap(&mGLAliasedLineWidthRange[0],
                      kCommonAliasedLineWidthRangeMin);
    ok &= RestrictCap(&mGLAliasedLineWidthRange[1],
                      kCommonAliasedLineWidthRangeMax);
    ok &= RestrictCap(&mGLAliasedPointSizeRange[0],
                      kCommonAliasedPointSizeRangeMin);
    ok &= RestrictCap(&mGLAliasedPointSizeRange[1],
                      kCommonAliasedPointSizeRangeMax);

    ok &= RestrictCap(&mGLMaxViewportDims[0], kCommonMaxViewportDims);
    ok &= RestrictCap(&mGLMaxViewportDims[1], kCommonMaxViewportDims);

    if (!ok) {
      GenerateWarning(
          "Unable to restrict WebGL limits in order to resist fingerprinting");
      return false;
||||||| merged common ancestors
        if (!ok) {
            GenerateWarning("Unable to restrict WebGL limits in order to resist fingerprinting");
            return false;
        }
=======
    mDisableFragHighP = true;
  } else if (ShouldResistFingerprinting()) {
    bool ok = true;

    ok &= RestrictCap(&mGLMaxTextureSize, kCommonMaxTextureSize);
    ok &= RestrictCap(&mGLMaxCubeMapTextureSize, kCommonMaxCubeMapTextureSize);
    ok &= RestrictCap(&mGLMaxRenderbufferSize, kCommonMaxRenderbufferSize);

    ok &= RestrictCap(&mGLMaxVertexTextureImageUnits,
                      kCommonMaxVertexTextureImageUnits);
    ok &= RestrictCap(&mGLMaxFragmentTextureImageUnits,
                      kCommonMaxFragmentTextureImageUnits);
    ok &= RestrictCap(&mGLMaxCombinedTextureImageUnits,
                      kCommonMaxCombinedTextureImageUnits);

    ok &= RestrictCap(&mGLMaxVertexAttribs, kCommonMaxVertexAttribs);
    ok &= RestrictCap(&mGLMaxVertexUniformVectors,
                      kCommonMaxVertexUniformVectors);
    ok &= RestrictCap(&mGLMaxFragmentUniformVectors,
                      kCommonMaxFragmentUniformVectors);
    ok &= RestrictCap(&mGLMaxVertexOutputVectors, kCommonMaxVaryingVectors);
    ok &= RestrictCap(&mGLMaxFragmentInputVectors, kCommonMaxVaryingVectors);

    ok &= RestrictCap(&mGLAliasedLineWidthRange[0],
                      kCommonAliasedLineWidthRangeMin);
    ok &= RestrictCap(&mGLAliasedLineWidthRange[1],
                      kCommonAliasedLineWidthRangeMax);
    ok &= RestrictCap(&mGLAliasedPointSizeRange[0],
                      kCommonAliasedPointSizeRangeMin);
    ok &= RestrictCap(&mGLAliasedPointSizeRange[1],
                      kCommonAliasedPointSizeRangeMax);

    ok &= RestrictCap(&mGLMaxViewportDims[0], kCommonMaxViewportDims);
    ok &= RestrictCap(&mGLMaxViewportDims[1], kCommonMaxViewportDims);

    if (!ok) {
      GenerateWarning(
          "Unable to restrict WebGL limits in order to resist fingerprinting");
      return false;
>>>>>>> upstream-releases
    }
  }

  ////////////////

  if (gl->IsCompatibilityProfile()) {
    gl->fEnable(LOCAL_GL_POINT_SPRITE);
  }

  if (!gl->IsGLES()) {
    gl->fEnable(LOCAL_GL_PROGRAM_POINT_SIZE);
  }

#ifdef XP_MACOSX
  if (gl->WorkAroundDriverBugs() && gl->Vendor() == gl::GLVendor::ATI &&
      !nsCocoaFeatures::IsAtLeastVersion(10, 9)) {
    // The Mac ATI driver, in all known OSX version up to and including
    // 10.8, renders points sprites upside-down. (Apple bug 11778921)
    gl->fPointParameterf(LOCAL_GL_POINT_SPRITE_COORD_ORIGIN,
                         LOCAL_GL_LOWER_LEFT);
  }
#endif

<<<<<<< HEAD
  if (gl->IsSupported(gl::GLFeature::seamless_cube_map_opt_in)) {
    gl->fEnable(LOCAL_GL_TEXTURE_CUBE_MAP_SEAMLESS);
  }

  // Check the shader validator pref
  mBypassShaderValidation = gfxPrefs::WebGLBypassShaderValidator();
||||||| merged common ancestors
    if (gl->IsSupported(gl::GLFeature::seamless_cube_map_opt_in)) {
        gl->fEnable(LOCAL_GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }

    // Check the shader validator pref
    mBypassShaderValidation = gfxPrefs::WebGLBypassShaderValidator();
=======
  if (gl->IsSupported(gl::GLFeature::seamless_cube_map_opt_in)) {
    gl->fEnable(LOCAL_GL_TEXTURE_CUBE_MAP_SEAMLESS);
  }
>>>>>>> upstream-releases

  // initialize shader translator
  if (!sh::Initialize()) {
    *out_failReason = {"FEATURE_FAILURE_WEBGL_GLSL",
                       "GLSL translator initialization failed!"};
    return false;
  }

  // Mesa can only be detected with the GL_VERSION string, of the form
  // "2.1 Mesa 7.11.0"
  const char* versionStr = (const char*)(gl->fGetString(LOCAL_GL_VERSION));
  mIsMesa = strstr(versionStr, "Mesa");

  // Notice that the point of calling fGetError here is not only to check for
  // errors, but also to reset the error flags so that a subsequent WebGL
  // getError call will give the correct result.
  error = gl->fGetError();
  if (error != LOCAL_GL_NO_ERROR) {
    const nsPrintfCString reason(
        "GL error 0x%x occurred during WebGL context"
        " initialization!",
        error);
    *out_failReason = {"FEATURE_FAILURE_WEBGL_GLERR_2", reason};
    return false;
  }

  if (IsWebGL2() && !InitWebGL2(out_failReason)) {
    // Todo: Bug 898404: Only allow WebGL2 on GL>=3.0 on desktop GL.
    return false;
  }

<<<<<<< HEAD
  if (!gl->IsSupported(GLFeature::vertex_array_object)) {
    *out_failReason = {"FEATURE_FAILURE_WEBGL_VAOS",
                       "Requires vertex_array_object."};
    return false;
  }

  // OpenGL core profiles remove the default VAO object from version
  // 4.0.0. We create a default VAO for all core profiles,
  // regardless of version.
  //
  // GL Spec 4.0.0:
  // (https://www.opengl.org/registry/doc/glspec40.core.20100311.pdf)
  // in Section E.2.2 "Removed Features", pg 397: "[...] The default
  // vertex array object (the name zero) is also deprecated. [...]"
  mDefaultVertexArray = WebGLVertexArray::Create(this);
  mDefaultVertexArray->BindVertexArray();
  mDefaultVertexArray->mAttribs.SetLength(mGLMaxVertexAttribs);

  mPixelStore_FlipY = false;
  mPixelStore_PremultiplyAlpha = false;
  mPixelStore_ColorspaceConversion = BROWSER_DEFAULT_WEBGL;
  mPixelStore_RequireFastPath = false;

  // GLES 3.0.4, p259:
  mPixelStore_UnpackImageHeight = 0;
  mPixelStore_UnpackSkipImages = 0;
  mPixelStore_UnpackRowLength = 0;
  mPixelStore_UnpackSkipRows = 0;
  mPixelStore_UnpackSkipPixels = 0;
  mPixelStore_UnpackAlignment = 4;
  mPixelStore_PackRowLength = 0;
  mPixelStore_PackSkipRows = 0;
  mPixelStore_PackSkipPixels = 0;
  mPixelStore_PackAlignment = 4;

  mPrimRestartTypeBytes = 0;

  mGenericVertexAttribTypes.assign(mGLMaxVertexAttribs,
                                   webgl::AttribBaseType::Float);
  mGenericVertexAttribTypeInvalidator.InvalidateCaches();

  static const float kDefaultGenericVertexAttribData[4] = {0, 0, 0, 1};
  memcpy(mGenericVertexAttrib0Data, kDefaultGenericVertexAttribData,
         sizeof(mGenericVertexAttrib0Data));

  mFakeVertexAttrib0BufferObject = 0;

  mNeedsIndexValidation =
      !gl->IsSupported(gl::GLFeature::robust_buffer_access_behavior);
  switch (gfxPrefs::WebGLForceIndexValidation()) {
||||||| merged common ancestors
    mPixelStore_FlipY = false;
    mPixelStore_PremultiplyAlpha = false;
    mPixelStore_ColorspaceConversion = BROWSER_DEFAULT_WEBGL;
    mPixelStore_RequireFastPath = false;

    // GLES 3.0.4, p259:
    mPixelStore_UnpackImageHeight = 0;
    mPixelStore_UnpackSkipImages = 0;
    mPixelStore_UnpackRowLength = 0;
    mPixelStore_UnpackSkipRows = 0;
    mPixelStore_UnpackSkipPixels = 0;
    mPixelStore_UnpackAlignment = 4;
    mPixelStore_PackRowLength = 0;
    mPixelStore_PackSkipRows = 0;
    mPixelStore_PackSkipPixels = 0;
    mPixelStore_PackAlignment = 4;

    mPrimRestartTypeBytes = 0;

    mGenericVertexAttribTypes.assign(mGLMaxVertexAttribs,
                                     webgl::AttribBaseType::Float);
    mGenericVertexAttribTypeInvalidator.InvalidateCaches();

    static const float kDefaultGenericVertexAttribData[4] = { 0, 0, 0, 1 };
    memcpy(mGenericVertexAttrib0Data, kDefaultGenericVertexAttribData,
           sizeof(mGenericVertexAttrib0Data));

    mFakeVertexAttrib0BufferObject = 0;

    mNeedsIndexValidation = !gl->IsSupported(gl::GLFeature::robust_buffer_access_behavior);
    switch (gfxPrefs::WebGLForceIndexValidation()) {
=======
  if (!gl->IsSupported(GLFeature::vertex_array_object)) {
    *out_failReason = {"FEATURE_FAILURE_WEBGL_VAOS",
                       "Requires vertex_array_object."};
    return false;
  }

  // OpenGL core profiles remove the default VAO object from version
  // 4.0.0. We create a default VAO for all core profiles,
  // regardless of version.
  //
  // GL Spec 4.0.0:
  // (https://www.opengl.org/registry/doc/glspec40.core.20100311.pdf)
  // in Section E.2.2 "Removed Features", pg 397: "[...] The default
  // vertex array object (the name zero) is also deprecated. [...]"
  mDefaultVertexArray = WebGLVertexArray::Create(this);
  mDefaultVertexArray->BindVertexArray();
  mDefaultVertexArray->mAttribs.resize(mGLMaxVertexAttribs);

  mPixelStore_FlipY = false;
  mPixelStore_PremultiplyAlpha = false;
  mPixelStore_ColorspaceConversion = BROWSER_DEFAULT_WEBGL;
  mPixelStore_RequireFastPath = false;

  // GLES 3.0.4, p259:
  mPixelStore_UnpackImageHeight = 0;
  mPixelStore_UnpackSkipImages = 0;
  mPixelStore_UnpackRowLength = 0;
  mPixelStore_UnpackSkipRows = 0;
  mPixelStore_UnpackSkipPixels = 0;
  mPixelStore_UnpackAlignment = 4;
  mPixelStore_PackRowLength = 0;
  mPixelStore_PackSkipRows = 0;
  mPixelStore_PackSkipPixels = 0;
  mPixelStore_PackAlignment = 4;

  mPrimRestartTypeBytes = 0;

  mGenericVertexAttribTypes.assign(mGLMaxVertexAttribs,
                                   webgl::AttribBaseType::Float);
  mGenericVertexAttribTypeInvalidator.InvalidateCaches();

  static const float kDefaultGenericVertexAttribData[4] = {0, 0, 0, 1};
  memcpy(mGenericVertexAttrib0Data, kDefaultGenericVertexAttribData,
         sizeof(mGenericVertexAttrib0Data));

  mFakeVertexAttrib0BufferObject = 0;

  mNeedsIndexValidation =
      !gl->IsSupported(gl::GLFeature::robust_buffer_access_behavior);
  switch (StaticPrefs::webgl_force_index_validation()) {
>>>>>>> upstream-releases
    case -1:
      mNeedsIndexValidation = false;
      break;
    case 1:
      mNeedsIndexValidation = true;
      break;
    default:
<<<<<<< HEAD
      MOZ_ASSERT(gfxPrefs::WebGLForceIndexValidation() == 0);
      break;
  }
||||||| merged common ancestors
        MOZ_ASSERT(gfxPrefs::WebGLForceIndexValidation() == 0);
        break;
    }
=======
      MOZ_ASSERT(StaticPrefs::webgl_force_index_validation() == 0);
      break;
  }
>>>>>>> upstream-releases

  return true;
}

<<<<<<< HEAD
bool WebGLContext::ValidateFramebufferTarget(GLenum target) {
  bool isValid = true;
  switch (target) {
||||||| merged common ancestors
bool
WebGLContext::ValidateFramebufferTarget(GLenum target)
{
    bool isValid = true;
    switch (target) {
=======
bool WebGLContext::ValidateFramebufferTarget(GLenum target) const {
  bool isValid = true;
  switch (target) {
>>>>>>> upstream-releases
    case LOCAL_GL_FRAMEBUFFER:
      break;

    case LOCAL_GL_DRAW_FRAMEBUFFER:
    case LOCAL_GL_READ_FRAMEBUFFER:
      isValid = IsWebGL2();
      break;

    default:
      isValid = false;
      break;
  }

  if (MOZ_LIKELY(isValid)) {
    return true;
  }

  ErrorInvalidEnumArg("target", target);
  return false;
}

}  // namespace mozilla
