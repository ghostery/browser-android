/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "WebGLContext.h"
#include "WebGLContextUtils.h"
#include "WebGLExtensions.h"
#include "GLContext.h"

#include "nsString.h"
#include "nsContentUtils.h"
#include "mozilla/Preferences.h"
#include "mozilla/dom/BindingDeclarations.h"
#include "AccessCheck.h"

namespace mozilla {

/*static*/ const char* WebGLContext::GetExtensionString(WebGLExtensionID ext) {
  typedef EnumeratedArray<WebGLExtensionID, WebGLExtensionID::Max, const char*>
      names_array_t;

  static names_array_t sExtensionNamesEnumeratedArray;
  static bool initialized = false;

  if (!initialized) {
    initialized = true;

#define WEBGL_EXTENSION_IDENTIFIER(x) \
<<<<<<< HEAD
  sExtensionNamesEnumeratedArray[WebGLExtensionID::x] = #x;

    WEBGL_EXTENSION_IDENTIFIER(ANGLE_instanced_arrays)
    WEBGL_EXTENSION_IDENTIFIER(EXT_blend_minmax)
    WEBGL_EXTENSION_IDENTIFIER(EXT_color_buffer_float)
    WEBGL_EXTENSION_IDENTIFIER(EXT_color_buffer_half_float)
    WEBGL_EXTENSION_IDENTIFIER(EXT_frag_depth)
    WEBGL_EXTENSION_IDENTIFIER(EXT_shader_texture_lod)
    WEBGL_EXTENSION_IDENTIFIER(EXT_sRGB)
    WEBGL_EXTENSION_IDENTIFIER(EXT_texture_compression_bptc)
    WEBGL_EXTENSION_IDENTIFIER(EXT_texture_compression_rgtc)
    WEBGL_EXTENSION_IDENTIFIER(EXT_texture_filter_anisotropic)
    WEBGL_EXTENSION_IDENTIFIER(EXT_disjoint_timer_query)
    WEBGL_EXTENSION_IDENTIFIER(MOZ_debug)
    WEBGL_EXTENSION_IDENTIFIER(OES_element_index_uint)
    WEBGL_EXTENSION_IDENTIFIER(OES_standard_derivatives)
    WEBGL_EXTENSION_IDENTIFIER(OES_texture_float)
    WEBGL_EXTENSION_IDENTIFIER(OES_texture_float_linear)
    WEBGL_EXTENSION_IDENTIFIER(OES_texture_half_float)
    WEBGL_EXTENSION_IDENTIFIER(OES_texture_half_float_linear)
    WEBGL_EXTENSION_IDENTIFIER(OES_vertex_array_object)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_color_buffer_float)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_astc)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_etc)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_etc1)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_pvrtc)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_s3tc)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_s3tc_srgb)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_debug_renderer_info)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_debug_shaders)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_depth_texture)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_draw_buffers)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_lose_context)
||||||| merged common ancestors
        sExtensionNamesEnumeratedArray[WebGLExtensionID::x] = #x;

        WEBGL_EXTENSION_IDENTIFIER(ANGLE_instanced_arrays)
        WEBGL_EXTENSION_IDENTIFIER(EXT_blend_minmax)
        WEBGL_EXTENSION_IDENTIFIER(EXT_color_buffer_float)
        WEBGL_EXTENSION_IDENTIFIER(EXT_color_buffer_half_float)
        WEBGL_EXTENSION_IDENTIFIER(EXT_frag_depth)
        WEBGL_EXTENSION_IDENTIFIER(EXT_shader_texture_lod)
        WEBGL_EXTENSION_IDENTIFIER(EXT_sRGB)
        WEBGL_EXTENSION_IDENTIFIER(EXT_texture_compression_bptc)
        WEBGL_EXTENSION_IDENTIFIER(EXT_texture_compression_rgtc)
        WEBGL_EXTENSION_IDENTIFIER(EXT_texture_filter_anisotropic)
        WEBGL_EXTENSION_IDENTIFIER(EXT_disjoint_timer_query)
        WEBGL_EXTENSION_IDENTIFIER(MOZ_debug)
        WEBGL_EXTENSION_IDENTIFIER(OES_element_index_uint)
        WEBGL_EXTENSION_IDENTIFIER(OES_standard_derivatives)
        WEBGL_EXTENSION_IDENTIFIER(OES_texture_float)
        WEBGL_EXTENSION_IDENTIFIER(OES_texture_float_linear)
        WEBGL_EXTENSION_IDENTIFIER(OES_texture_half_float)
        WEBGL_EXTENSION_IDENTIFIER(OES_texture_half_float_linear)
        WEBGL_EXTENSION_IDENTIFIER(OES_vertex_array_object)
        WEBGL_EXTENSION_IDENTIFIER(WEBGL_color_buffer_float)
        WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_astc)
        WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_etc)
        WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_etc1)
        WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_pvrtc)
        WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_s3tc)
        WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_s3tc_srgb)
        WEBGL_EXTENSION_IDENTIFIER(WEBGL_debug_renderer_info)
        WEBGL_EXTENSION_IDENTIFIER(WEBGL_debug_shaders)
        WEBGL_EXTENSION_IDENTIFIER(WEBGL_depth_texture)
        WEBGL_EXTENSION_IDENTIFIER(WEBGL_draw_buffers)
        WEBGL_EXTENSION_IDENTIFIER(WEBGL_lose_context)
=======
  sExtensionNamesEnumeratedArray[WebGLExtensionID::x] = #x;

    WEBGL_EXTENSION_IDENTIFIER(ANGLE_instanced_arrays)
    WEBGL_EXTENSION_IDENTIFIER(EXT_blend_minmax)
    WEBGL_EXTENSION_IDENTIFIER(EXT_color_buffer_float)
    WEBGL_EXTENSION_IDENTIFIER(EXT_color_buffer_half_float)
    WEBGL_EXTENSION_IDENTIFIER(EXT_disjoint_timer_query)
    WEBGL_EXTENSION_IDENTIFIER(EXT_float_blend)
    WEBGL_EXTENSION_IDENTIFIER(EXT_frag_depth)
    WEBGL_EXTENSION_IDENTIFIER(EXT_shader_texture_lod)
    WEBGL_EXTENSION_IDENTIFIER(EXT_sRGB)
    WEBGL_EXTENSION_IDENTIFIER(EXT_texture_compression_bptc)
    WEBGL_EXTENSION_IDENTIFIER(EXT_texture_compression_rgtc)
    WEBGL_EXTENSION_IDENTIFIER(EXT_texture_filter_anisotropic)
    WEBGL_EXTENSION_IDENTIFIER(MOZ_debug)
    WEBGL_EXTENSION_IDENTIFIER(OES_element_index_uint)
    WEBGL_EXTENSION_IDENTIFIER(OES_fbo_render_mipmap)
    WEBGL_EXTENSION_IDENTIFIER(OES_standard_derivatives)
    WEBGL_EXTENSION_IDENTIFIER(OES_texture_float)
    WEBGL_EXTENSION_IDENTIFIER(OES_texture_float_linear)
    WEBGL_EXTENSION_IDENTIFIER(OES_texture_half_float)
    WEBGL_EXTENSION_IDENTIFIER(OES_texture_half_float_linear)
    WEBGL_EXTENSION_IDENTIFIER(OES_vertex_array_object)
    WEBGL_EXTENSION_IDENTIFIER(OVR_multiview2)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_color_buffer_float)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_astc)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_etc)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_etc1)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_pvrtc)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_s3tc)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_compressed_texture_s3tc_srgb)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_debug_renderer_info)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_debug_shaders)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_depth_texture)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_draw_buffers)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_explicit_present)
    WEBGL_EXTENSION_IDENTIFIER(WEBGL_lose_context)
>>>>>>> upstream-releases

#undef WEBGL_EXTENSION_IDENTIFIER
<<<<<<< HEAD
  }

  return sExtensionNamesEnumeratedArray[ext];
}
||||||| merged common ancestors
    }

    return sExtensionNamesEnumeratedArray[ext];
}
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool WebGLContext::IsExtensionEnabled(WebGLExtensionID ext) const {
  return mExtensions[ext];
||||||| merged common ancestors
bool
WebGLContext::IsExtensionEnabled(WebGLExtensionID ext) const
{
    return mExtensions[ext];
=======
  return sExtensionNamesEnumeratedArray[ext];
>>>>>>> upstream-releases
}

bool WebGLContext::IsExtensionSupported(dom::CallerType callerType,
                                        WebGLExtensionID ext) const {
  bool allowPrivilegedExts = false;

<<<<<<< HEAD
  // Chrome contexts need access to debug information even when
  // webgl.disable-extensions is set. This is used in the graphics
  // section of about:support
  if (callerType == dom::CallerType::System) {
    allowPrivilegedExts = true;
  }
||||||| merged common ancestors
    if (gfxPrefs::WebGLPrivilegedExtensionsEnabled()) {
        allowPrivilegedExts = true;
    }
=======
  // Chrome contexts need access to debug information even when
  // webgl.disable-extensions is set. This is used in the graphics
  // section of about:support
  if (callerType == dom::CallerType::System) {
    allowPrivilegedExts = true;
  }

  if (StaticPrefs::webgl_enable_privileged_extensions()) {
    allowPrivilegedExts = true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (gfxPrefs::WebGLPrivilegedExtensionsEnabled()) {
    allowPrivilegedExts = true;
  }

  if (allowPrivilegedExts) {
    switch (ext) {
      case WebGLExtensionID::EXT_disjoint_timer_query:
        return WebGLExtensionDisjointTimerQuery::IsSupported(this);
      case WebGLExtensionID::MOZ_debug:
        return true;
      case WebGLExtensionID::WEBGL_debug_renderer_info:
        return true;
      case WebGLExtensionID::WEBGL_debug_shaders:
        return true;
      default:
        // For warnings-as-errors.
        break;
||||||| merged common ancestors
    if (allowPrivilegedExts) {
        switch (ext) {
        case WebGLExtensionID::EXT_disjoint_timer_query:
            return WebGLExtensionDisjointTimerQuery::IsSupported(this);
        case WebGLExtensionID::MOZ_debug:
            return true;
        case WebGLExtensionID::WEBGL_debug_renderer_info:
            return true;
        case WebGLExtensionID::WEBGL_debug_shaders:
            return true;
        default:
            // For warnings-as-errors.
            break;
        }
=======
  if (allowPrivilegedExts) {
    switch (ext) {
      case WebGLExtensionID::EXT_disjoint_timer_query:
        return WebGLExtensionDisjointTimerQuery::IsSupported(this);
      case WebGLExtensionID::MOZ_debug:
        return true;
      case WebGLExtensionID::WEBGL_debug_renderer_info:
        return true;
      case WebGLExtensionID::WEBGL_debug_shaders:
        return true;
      default:
        // For warnings-as-errors.
        break;
>>>>>>> upstream-releases
    }
  }

  return IsExtensionSupported(ext);
}

bool WebGLContext::IsExtensionSupported(WebGLExtensionID ext) const {
  if (mDisableExtensions) return false;

<<<<<<< HEAD
  // Extensions for both WebGL 1 and 2.
  switch (ext) {
||||||| merged common ancestors
    // Extensions for both WebGL 1 and 2.
    switch (ext) {
=======
  bool shouldResistFingerprinting = ShouldResistFingerprinting();

  switch (ext) {
>>>>>>> upstream-releases
    // In alphabetical order
    // ANGLE_
    case WebGLExtensionID::ANGLE_instanced_arrays:
      return WebGLExtensionInstancedArrays::IsSupported(this);

    // EXT_
    case WebGLExtensionID::EXT_blend_minmax:
      return WebGLExtensionBlendMinMax::IsSupported(this);

    case WebGLExtensionID::EXT_color_buffer_float:
      return WebGLExtensionEXTColorBufferFloat::IsSupported(this);

    case WebGLExtensionID::EXT_color_buffer_half_float:
      return WebGLExtensionColorBufferHalfFloat::IsSupported(this);

    case WebGLExtensionID::EXT_float_blend:
      return WebGLExtensionFloatBlend::IsSupported(this);

    case WebGLExtensionID::EXT_frag_depth:
      return WebGLExtensionFragDepth::IsSupported(this);

    case WebGLExtensionID::EXT_shader_texture_lod:
      return WebGLExtensionShaderTextureLod::IsSupported(this);

    case WebGLExtensionID::EXT_sRGB:
      return WebGLExtensionSRGB::IsSupported(this);

    case WebGLExtensionID::EXT_texture_compression_bptc:
      return WebGLExtensionCompressedTextureBPTC::IsSupported(this);

    case WebGLExtensionID::EXT_texture_compression_rgtc:
      return WebGLExtensionCompressedTextureRGTC::IsSupported(this);

    case WebGLExtensionID::EXT_texture_filter_anisotropic:
      return gl->IsExtensionSupported(
          gl::GLContext::EXT_texture_filter_anisotropic);

    // OES_
    case WebGLExtensionID::OES_element_index_uint:
      if (IsWebGL2()) return false;
      return gl->IsSupported(gl::GLFeature::element_index_uint);

    case WebGLExtensionID::OES_fbo_render_mipmap:
      return WebGLExtensionFBORenderMipmap::IsSupported(this);

    case WebGLExtensionID::OES_standard_derivatives:
      if (IsWebGL2()) return false;
      return gl->IsSupported(gl::GLFeature::standard_derivatives);

    case WebGLExtensionID::OES_texture_float:
      return WebGLExtensionTextureFloat::IsSupported(this);

    case WebGLExtensionID::OES_texture_float_linear:
<<<<<<< HEAD
      return gl->IsSupported(gl::GLFeature::texture_float_linear);
||||||| merged common ancestors
        return gl->IsSupported(gl::GLFeature::texture_float_linear);
=======
      return gl->IsSupported(gl::GLFeature::texture_float_linear);

    case WebGLExtensionID::OES_texture_half_float:
      return WebGLExtensionTextureHalfFloat::IsSupported(this);

    case WebGLExtensionID::OES_texture_half_float_linear:
      if (IsWebGL2()) return false;
      return gl->IsSupported(gl::GLFeature::texture_half_float_linear);

    case WebGLExtensionID::OES_vertex_array_object:
      return !IsWebGL2();  // Always supported in webgl1.

    // OVR_
    case WebGLExtensionID::OVR_multiview2:
      return WebGLExtensionMultiview::IsSupported(this);
>>>>>>> upstream-releases

    // WEBGL_
    case WebGLExtensionID::WEBGL_color_buffer_float:
      return WebGLExtensionColorBufferFloat::IsSupported(this);

    case WebGLExtensionID::WEBGL_compressed_texture_astc:
<<<<<<< HEAD
      return WebGLExtensionCompressedTextureASTC::IsSupported(this);
||||||| merged common ancestors
        return WebGLExtensionCompressedTextureASTC::IsSupported(this);
=======
      return WebGLExtensionCompressedTextureASTC::IsSupported(this);

>>>>>>> upstream-releases
    case WebGLExtensionID::WEBGL_compressed_texture_etc:
<<<<<<< HEAD
      return gl->IsSupported(gl::GLFeature::ES3_compatibility) &&
             !gl->IsANGLE();
||||||| merged common ancestors
        return gl->IsSupported(gl::GLFeature::ES3_compatibility) &&
               !gl->IsANGLE();
=======
      return gl->IsSupported(gl::GLFeature::ES3_compatibility) &&
             !gl->IsANGLE();

>>>>>>> upstream-releases
    case WebGLExtensionID::WEBGL_compressed_texture_etc1:
<<<<<<< HEAD
      return gl->IsExtensionSupported(
                 gl::GLContext::OES_compressed_ETC1_RGB8_texture) &&
             !gl->IsANGLE();
||||||| merged common ancestors
        return gl->IsExtensionSupported(gl::GLContext::OES_compressed_ETC1_RGB8_texture) &&
               !gl->IsANGLE();
=======
      return gl->IsExtensionSupported(
                 gl::GLContext::OES_compressed_ETC1_RGB8_texture) &&
             !gl->IsANGLE();

>>>>>>> upstream-releases
    case WebGLExtensionID::WEBGL_compressed_texture_pvrtc:
<<<<<<< HEAD
      return gl->IsExtensionSupported(
          gl::GLContext::IMG_texture_compression_pvrtc);
||||||| merged common ancestors
        return gl->IsExtensionSupported(gl::GLContext::IMG_texture_compression_pvrtc);
=======
      return gl->IsExtensionSupported(
          gl::GLContext::IMG_texture_compression_pvrtc);

>>>>>>> upstream-releases
    case WebGLExtensionID::WEBGL_compressed_texture_s3tc:
<<<<<<< HEAD
      return WebGLExtensionCompressedTextureS3TC::IsSupported(this);
||||||| merged common ancestors
        return WebGLExtensionCompressedTextureS3TC::IsSupported(this);
=======
      return WebGLExtensionCompressedTextureS3TC::IsSupported(this);

>>>>>>> upstream-releases
    case WebGLExtensionID::WEBGL_compressed_texture_s3tc_srgb:
<<<<<<< HEAD
      return WebGLExtensionCompressedTextureS3TC_SRGB::IsSupported(this);
||||||| merged common ancestors
        return WebGLExtensionCompressedTextureS3TC_SRGB::IsSupported(this);
=======
      return WebGLExtensionCompressedTextureS3TC_SRGB::IsSupported(this);

>>>>>>> upstream-releases
    case WebGLExtensionID::WEBGL_debug_renderer_info:
<<<<<<< HEAD
      return Preferences::GetBool("webgl.enable-debug-renderer-info", false) &&
             !nsContentUtils::ShouldResistFingerprinting();
||||||| merged common ancestors
        return Preferences::GetBool("webgl.enable-debug-renderer-info", false) &&
               !nsContentUtils::ShouldResistFingerprinting();
=======
      return Preferences::GetBool("webgl.enable-debug-renderer-info", false) &&
             !shouldResistFingerprinting;

>>>>>>> upstream-releases
    case WebGLExtensionID::WEBGL_debug_shaders:
<<<<<<< HEAD
      return !nsContentUtils::ShouldResistFingerprinting();
    case WebGLExtensionID::WEBGL_lose_context:
      // We always support this extension.
      return true;
||||||| merged common ancestors
        return !nsContentUtils::ShouldResistFingerprinting();
    case WebGLExtensionID::WEBGL_lose_context:
        // We always support this extension.
        return true;
=======
      return !shouldResistFingerprinting;
>>>>>>> upstream-releases

<<<<<<< HEAD
    default:
      // For warnings-as-errors.
      break;
  }

  if (IsWebGL2()) {
    // WebGL2-only extensions
    switch (ext) {
      // EXT_
      case WebGLExtensionID::EXT_color_buffer_float:
        return WebGLExtensionEXTColorBufferFloat::IsSupported(this);

      default:
        // For warnings-as-errors.
        break;
    }
  } else {
    // WebGL1-only extensions
    switch (ext) {
      // ANGLE_
      case WebGLExtensionID::ANGLE_instanced_arrays:
        return WebGLExtensionInstancedArrays::IsSupported(this);

      // EXT_
      case WebGLExtensionID::EXT_blend_minmax:
        return WebGLExtensionBlendMinMax::IsSupported(this);
      case WebGLExtensionID::EXT_color_buffer_half_float:
        return WebGLExtensionColorBufferHalfFloat::IsSupported(this);
      case WebGLExtensionID::EXT_frag_depth:
        return WebGLExtensionFragDepth::IsSupported(this);
      case WebGLExtensionID::EXT_shader_texture_lod:
        return WebGLExtensionShaderTextureLod::IsSupported(this);
      case WebGLExtensionID::EXT_sRGB:
        return WebGLExtensionSRGB::IsSupported(this);

      // OES_
      case WebGLExtensionID::OES_element_index_uint:
        return gl->IsSupported(gl::GLFeature::element_index_uint);
      case WebGLExtensionID::OES_standard_derivatives:
        return gl->IsSupported(gl::GLFeature::standard_derivatives);
      case WebGLExtensionID::OES_texture_float:
        return WebGLExtensionTextureFloat::IsSupported(this);
      case WebGLExtensionID::OES_texture_half_float:
        return WebGLExtensionTextureHalfFloat::IsSupported(this);
      case WebGLExtensionID::OES_texture_half_float_linear:
        return gl->IsSupported(gl::GLFeature::texture_half_float_linear);

      case WebGLExtensionID::OES_vertex_array_object:
        return true;

      // WEBGL_
      case WebGLExtensionID::WEBGL_color_buffer_float:
        return WebGLExtensionColorBufferFloat::IsSupported(this);
      case WebGLExtensionID::WEBGL_depth_texture:
        // WEBGL_depth_texture supports DEPTH_STENCIL textures
        if (!gl->IsSupported(gl::GLFeature::packed_depth_stencil)) return false;

        return gl->IsSupported(gl::GLFeature::depth_texture) ||
               gl->IsExtensionSupported(gl::GLContext::ANGLE_depth_texture);
      case WebGLExtensionID::WEBGL_draw_buffers:
        return WebGLExtensionDrawBuffers::IsSupported(this);
      default:
        // For warnings-as-errors.
        break;
    }
||||||| merged common ancestors
    default:
        // For warnings-as-errors.
        break;
    }
=======
    case WebGLExtensionID::WEBGL_depth_texture:
      return WebGLExtensionDepthTexture::IsSupported(this);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (gfxPrefs::WebGLDraftExtensionsEnabled()) {
      /*
      switch (ext) {
      default:
          // For warnings-as-errors.
          break;
      }
      */
    }
  }
||||||| merged common ancestors
    if (IsWebGL2()) {
        // WebGL2-only extensions
        switch (ext) {
        // EXT_
        case WebGLExtensionID::EXT_color_buffer_float:
            return WebGLExtensionEXTColorBufferFloat::IsSupported(this);

        default:
            // For warnings-as-errors.
            break;
        }
    } else {
        // WebGL1-only extensions
        switch (ext) {
        // ANGLE_
        case WebGLExtensionID::ANGLE_instanced_arrays:
            return WebGLExtensionInstancedArrays::IsSupported(this);

        // EXT_
        case WebGLExtensionID::EXT_blend_minmax:
            return WebGLExtensionBlendMinMax::IsSupported(this);
        case WebGLExtensionID::EXT_color_buffer_half_float:
            return WebGLExtensionColorBufferHalfFloat::IsSupported(this);
        case WebGLExtensionID::EXT_frag_depth:
            return WebGLExtensionFragDepth::IsSupported(this);
        case WebGLExtensionID::EXT_shader_texture_lod:
            return WebGLExtensionShaderTextureLod::IsSupported(this);
        case WebGLExtensionID::EXT_sRGB:
            return WebGLExtensionSRGB::IsSupported(this);

        // OES_
        case WebGLExtensionID::OES_element_index_uint:
            return gl->IsSupported(gl::GLFeature::element_index_uint);
        case WebGLExtensionID::OES_standard_derivatives:
            return gl->IsSupported(gl::GLFeature::standard_derivatives);
        case WebGLExtensionID::OES_texture_float:
            return WebGLExtensionTextureFloat::IsSupported(this);
        case WebGLExtensionID::OES_texture_half_float:
            return WebGLExtensionTextureHalfFloat::IsSupported(this);
        case WebGLExtensionID::OES_texture_half_float_linear:
            return gl->IsSupported(gl::GLFeature::texture_half_float_linear);

        case WebGLExtensionID::OES_vertex_array_object:
            return true;

        // WEBGL_
        case WebGLExtensionID::WEBGL_color_buffer_float:
            return WebGLExtensionColorBufferFloat::IsSupported(this);
        case WebGLExtensionID::WEBGL_depth_texture:
            // WEBGL_depth_texture supports DEPTH_STENCIL textures
            if (!gl->IsSupported(gl::GLFeature::packed_depth_stencil))
                return false;

            return gl->IsSupported(gl::GLFeature::depth_texture) ||
                   gl->IsExtensionSupported(gl::GLContext::ANGLE_depth_texture);
        case WebGLExtensionID::WEBGL_draw_buffers:
            return WebGLExtensionDrawBuffers::IsSupported(this);
        default:
            // For warnings-as-errors.
            break;
        }

        if (gfxPrefs::WebGLDraftExtensionsEnabled()) {
            /*
            switch (ext) {
            default:
                // For warnings-as-errors.
                break;
            }
            */
        }
    }
=======
    case WebGLExtensionID::WEBGL_draw_buffers:
      return WebGLExtensionDrawBuffers::IsSupported(this);

    case WebGLExtensionID::WEBGL_explicit_present:
      return WebGLExtensionExplicitPresent::IsSupported(this);

    case WebGLExtensionID::WEBGL_lose_context:
      // We always support this extension.
      return true;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return false;
||||||| merged common ancestors
    return false;
=======
    case WebGLExtensionID::EXT_disjoint_timer_query:
    case WebGLExtensionID::MOZ_debug:
    case WebGLExtensionID::Max:
      return false;
  }

  return false;
>>>>>>> upstream-releases
}

static bool CompareWebGLExtensionName(const nsACString& name,
                                      const char* other) {
  return name.Equals(other, nsCaseInsensitiveCStringComparator());
}

WebGLExtensionBase* WebGLContext::EnableSupportedExtension(
    dom::CallerType callerType, WebGLExtensionID ext) {
  if (!IsExtensionEnabled(ext)) {
    if (!IsExtensionSupported(callerType, ext)) return nullptr;

    EnableExtension(ext);
  }

  return mExtensions[ext];
}

<<<<<<< HEAD
void WebGLContext::GetExtension(JSContext* cx, const nsAString& wideName,
                                JS::MutableHandle<JSObject*> retval,
                                dom::CallerType callerType, ErrorResult& rv) {
  retval.set(nullptr);
  const FuncScope funcScope(*this, "getExtension");
  if (IsContextLost()) return;

  NS_LossyConvertUTF16toASCII name(wideName);

  WebGLExtensionID ext = WebGLExtensionID::Unknown;

  // step 1: figure what extension is wanted
  for (size_t i = 0; i < size_t(WebGLExtensionID::Max); i++) {
    WebGLExtensionID extension = WebGLExtensionID(i);

    if (CompareWebGLExtensionName(name, GetExtensionString(extension))) {
      ext = extension;
      break;
||||||| merged common ancestors
void
WebGLContext::GetExtension(JSContext* cx,
                           const nsAString& wideName,
                           JS::MutableHandle<JSObject*> retval,
                           dom::CallerType callerType,
                           ErrorResult& rv)
{
    retval.set(nullptr);
    const FuncScope funcScope(*this, "getExtension");
    if (IsContextLost())
        return;

    NS_LossyConvertUTF16toASCII name(wideName);

    WebGLExtensionID ext = WebGLExtensionID::Unknown;

    // step 1: figure what extension is wanted
    for (size_t i = 0; i < size_t(WebGLExtensionID::Max); i++) {
        WebGLExtensionID extension = WebGLExtensionID(i);

        if (CompareWebGLExtensionName(name, GetExtensionString(extension))) {
            ext = extension;
            break;
        }
=======
void WebGLContext::GetExtension(JSContext* cx, const nsAString& wideName,
                                JS::MutableHandle<JSObject*> retval,
                                dom::CallerType callerType, ErrorResult& rv) {
  retval.set(nullptr);
  const FuncScope funcScope(*this, "getExtension");
  if (IsContextLost()) return;

  NS_LossyConvertUTF16toASCII name(wideName);

  WebGLExtensionID ext = WebGLExtensionID::Max;

  // step 1: figure what extension is wanted
  for (size_t i = 0; i < size_t(WebGLExtensionID::Max); i++) {
    WebGLExtensionID extension = WebGLExtensionID(i);

    if (CompareWebGLExtensionName(name, GetExtensionString(extension))) {
      ext = extension;
      break;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (ext == WebGLExtensionID::Unknown) return;
||||||| merged common ancestors
    if (ext == WebGLExtensionID::Unknown)
        return;
=======
  if (ext == WebGLExtensionID::Max) return;
>>>>>>> upstream-releases

  // step 2: check if the extension is supported
  if (!IsExtensionSupported(callerType, ext)) return;

<<<<<<< HEAD
  // step 3: if the extension hadn't been previously been created, create it
  // now, thus enabling it
  WebGLExtensionBase* extObj = EnableSupportedExtension(callerType, ext);
  if (!extObj) return;
||||||| merged common ancestors
    // step 3: if the extension hadn't been previously been created, create it now, thus enabling it
    WebGLExtensionBase* extObj = EnableSupportedExtension(callerType, ext);
    if (!extObj)
        return;
=======
  // step 3: if the extension hadn't been previously been created, create it
  // now, thus enabling it
  WebGLExtensionBase* extObj = EnableSupportedExtension(callerType, ext);
  if (!extObj) return;

  // Step 4: Enable any implied extensions.
  switch (ext) {
    case WebGLExtensionID::EXT_color_buffer_float:
      EnableSupportedExtension(callerType, WebGLExtensionID::EXT_float_blend);
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 4: Enable any implied extensions.
  switch (ext) {
||||||| merged common ancestors
    // Step 4: Enable any implied extensions.
    switch (ext) {
=======
>>>>>>> upstream-releases
    case WebGLExtensionID::OES_texture_float:
<<<<<<< HEAD
      EnableSupportedExtension(callerType,
                               WebGLExtensionID::WEBGL_color_buffer_float);
      break;
||||||| merged common ancestors
        EnableSupportedExtension(callerType,
                                 WebGLExtensionID::WEBGL_color_buffer_float);
        break;
=======
      EnableSupportedExtension(callerType,
                               WebGLExtensionID::WEBGL_color_buffer_float);
      EnableSupportedExtension(callerType, WebGLExtensionID::EXT_float_blend);
      break;
>>>>>>> upstream-releases

    case WebGLExtensionID::OES_texture_half_float:
<<<<<<< HEAD
      EnableSupportedExtension(callerType,
                               WebGLExtensionID::EXT_color_buffer_half_float);
      break;
||||||| merged common ancestors
        EnableSupportedExtension(callerType,
                                 WebGLExtensionID::EXT_color_buffer_half_float);
        break;
=======
      EnableSupportedExtension(callerType,
                               WebGLExtensionID::EXT_color_buffer_half_float);
      break;

    case WebGLExtensionID::WEBGL_color_buffer_float:
      EnableSupportedExtension(callerType, WebGLExtensionID::EXT_float_blend);
      break;
>>>>>>> upstream-releases

    default:
      break;
  }

  retval.set(WebGLObjectAsJSObject(cx, extObj, rv));
}

void WebGLContext::EnableExtension(WebGLExtensionID ext) {
  MOZ_ASSERT(IsExtensionEnabled(ext) == false);

  WebGLExtensionBase* obj = nullptr;
  switch (ext) {
    // ANGLE_
    case WebGLExtensionID::ANGLE_instanced_arrays:
      obj = new WebGLExtensionInstancedArrays(this);
      break;

    // EXT_
    case WebGLExtensionID::EXT_blend_minmax:
      obj = new WebGLExtensionBlendMinMax(this);
      break;
    case WebGLExtensionID::EXT_color_buffer_float:
      obj = new WebGLExtensionEXTColorBufferFloat(this);
      break;
    case WebGLExtensionID::EXT_color_buffer_half_float:
      obj = new WebGLExtensionColorBufferHalfFloat(this);
      break;
    case WebGLExtensionID::EXT_disjoint_timer_query:
<<<<<<< HEAD
      obj = new WebGLExtensionDisjointTimerQuery(this);
      break;
||||||| merged common ancestors
        obj = new WebGLExtensionDisjointTimerQuery(this);
        break;
=======
      obj = new WebGLExtensionDisjointTimerQuery(this);
      break;
    case WebGLExtensionID::EXT_float_blend:
      obj = new WebGLExtensionFloatBlend(this);
      break;
>>>>>>> upstream-releases
    case WebGLExtensionID::EXT_frag_depth:
      obj = new WebGLExtensionFragDepth(this);
      break;
    case WebGLExtensionID::EXT_shader_texture_lod:
      obj = new WebGLExtensionShaderTextureLod(this);
      break;
    case WebGLExtensionID::EXT_sRGB:
      obj = new WebGLExtensionSRGB(this);
      break;
    case WebGLExtensionID::EXT_texture_compression_bptc:
      obj = new WebGLExtensionCompressedTextureBPTC(this);
      break;
    case WebGLExtensionID::EXT_texture_compression_rgtc:
      obj = new WebGLExtensionCompressedTextureRGTC(this);
      break;
    case WebGLExtensionID::EXT_texture_filter_anisotropic:
      obj = new WebGLExtensionTextureFilterAnisotropic(this);
      break;

    // MOZ_
    case WebGLExtensionID::MOZ_debug:
      obj = new WebGLExtensionMOZDebug(this);
      break;

    // OES_
    case WebGLExtensionID::OES_element_index_uint:
<<<<<<< HEAD
      obj = new WebGLExtensionElementIndexUint(this);
      break;
||||||| merged common ancestors
        obj = new WebGLExtensionElementIndexUint(this);
        break;
=======
      obj = new WebGLExtensionElementIndexUint(this);
      break;
    case WebGLExtensionID::OES_fbo_render_mipmap:
      obj = new WebGLExtensionFBORenderMipmap(this);
      break;
>>>>>>> upstream-releases
    case WebGLExtensionID::OES_standard_derivatives:
      obj = new WebGLExtensionStandardDerivatives(this);
      break;
    case WebGLExtensionID::OES_texture_float:
      obj = new WebGLExtensionTextureFloat(this);
      break;
    case WebGLExtensionID::OES_texture_float_linear:
      obj = new WebGLExtensionTextureFloatLinear(this);
      break;
    case WebGLExtensionID::OES_texture_half_float:
      obj = new WebGLExtensionTextureHalfFloat(this);
      break;
    case WebGLExtensionID::OES_texture_half_float_linear:
      obj = new WebGLExtensionTextureHalfFloatLinear(this);
      break;
    case WebGLExtensionID::OES_vertex_array_object:
<<<<<<< HEAD
      obj = new WebGLExtensionVertexArray(this);
      break;
||||||| merged common ancestors
        obj = new WebGLExtensionVertexArray(this);
        break;
=======
      obj = new WebGLExtensionVertexArray(this);
      break;

    // WEBGL_
    case WebGLExtensionID::OVR_multiview2:
      obj = new WebGLExtensionMultiview(this);
      break;
>>>>>>> upstream-releases

    // WEBGL_
    case WebGLExtensionID::WEBGL_color_buffer_float:
      obj = new WebGLExtensionColorBufferFloat(this);
      break;
    case WebGLExtensionID::WEBGL_compressed_texture_astc:
      obj = new WebGLExtensionCompressedTextureASTC(this);
      break;
    case WebGLExtensionID::WEBGL_compressed_texture_etc:
      obj = new WebGLExtensionCompressedTextureES3(this);
      break;
    case WebGLExtensionID::WEBGL_compressed_texture_etc1:
      obj = new WebGLExtensionCompressedTextureETC1(this);
      break;
    case WebGLExtensionID::WEBGL_compressed_texture_pvrtc:
      obj = new WebGLExtensionCompressedTexturePVRTC(this);
      break;
    case WebGLExtensionID::WEBGL_compressed_texture_s3tc:
      obj = new WebGLExtensionCompressedTextureS3TC(this);
      break;
    case WebGLExtensionID::WEBGL_compressed_texture_s3tc_srgb:
      obj = new WebGLExtensionCompressedTextureS3TC_SRGB(this);
      break;
    case WebGLExtensionID::WEBGL_debug_renderer_info:
      obj = new WebGLExtensionDebugRendererInfo(this);
      break;
    case WebGLExtensionID::WEBGL_debug_shaders:
      obj = new WebGLExtensionDebugShaders(this);
      break;
    case WebGLExtensionID::WEBGL_depth_texture:
      obj = new WebGLExtensionDepthTexture(this);
      break;
    case WebGLExtensionID::WEBGL_draw_buffers:
<<<<<<< HEAD
      obj = new WebGLExtensionDrawBuffers(this);
      break;
||||||| merged common ancestors
        obj = new WebGLExtensionDrawBuffers(this);
        break;
=======
      obj = new WebGLExtensionDrawBuffers(this);
      break;
    case WebGLExtensionID::WEBGL_explicit_present:
      obj = new WebGLExtensionExplicitPresent(this);
      break;
>>>>>>> upstream-releases
    case WebGLExtensionID::WEBGL_lose_context:
      obj = new WebGLExtensionLoseContext(this);
      break;

<<<<<<< HEAD
    default:
      MOZ_ASSERT(false, "should not get there.");
  }
||||||| merged common ancestors
    default:
        MOZ_ASSERT(false, "should not get there.");
    }
=======
    case WebGLExtensionID::Max:
      MOZ_CRASH();
  }
>>>>>>> upstream-releases

  mExtensions[ext] = obj;
}

void WebGLContext::GetSupportedExtensions(
    dom::Nullable<nsTArray<nsString> >& retval, dom::CallerType callerType) {
  retval.SetNull();
  const FuncScope funcScope(*this, "getSupportedExtensions");
  if (IsContextLost()) return;

  nsTArray<nsString>& arr = retval.SetValue();

  for (size_t i = 0; i < size_t(WebGLExtensionID::Max); i++) {
    const auto extension = WebGLExtensionID(i);
    if (extension == WebGLExtensionID::MOZ_debug)
      continue;  // Hide MOZ_debug from this list.

    if (IsExtensionSupported(callerType, extension)) {
      const char* extStr = GetExtensionString(extension);
      arr.AppendElement(NS_ConvertUTF8toUTF16(extStr));
    }
  }
}

}  // namespace mozilla
