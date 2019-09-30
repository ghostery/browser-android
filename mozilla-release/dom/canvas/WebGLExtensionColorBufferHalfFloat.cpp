/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "WebGLExtensions.h"

#include "GLContext.h"
#include "mozilla/dom/WebGLRenderingContextBinding.h"
#include "WebGLContext.h"
#include "WebGLFormats.h"

namespace mozilla {

WebGLExtensionColorBufferHalfFloat::WebGLExtensionColorBufferHalfFloat(
    WebGLContext* webgl)
    : WebGLExtensionBase(webgl) {
  MOZ_ASSERT(IsSupported(webgl), "Don't construct extension if unsupported.");

  auto& fua = webgl->mFormatUsage;

<<<<<<< HEAD
  auto fnUpdateUsage = [&fua](GLenum sizedFormat,
                              webgl::EffectiveFormat effFormat) {
    auto usage = fua->EditUsage(effFormat);
    usage->SetRenderable();
    fua->AllowRBFormat(sizedFormat, usage);
  };
||||||| merged common ancestors
    auto fnUpdateUsage = [&fua](GLenum sizedFormat, webgl::EffectiveFormat effFormat) {
        auto usage = fua->EditUsage(effFormat);
        usage->SetRenderable();
        fua->AllowRBFormat(sizedFormat, usage);
    };
=======
  auto fnUpdateUsage = [&fua](GLenum sizedFormat,
                              webgl::EffectiveFormat effFormat,
                              const bool renderable) {
    auto usage = fua->EditUsage(effFormat);
    if (renderable) {
      usage->SetRenderable();
    }
    fua->AllowRBFormat(sizedFormat, usage, renderable);
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
#define FOO(x) fnUpdateUsage(LOCAL_GL_##x, webgl::EffectiveFormat::x)
||||||| merged common ancestors
#define FOO(x) fnUpdateUsage(LOCAL_GL_ ## x, webgl::EffectiveFormat::x)
=======
#define FOO(x, y) fnUpdateUsage(LOCAL_GL_##x, webgl::EffectiveFormat::x, y)
>>>>>>> upstream-releases

<<<<<<< HEAD
  FOO(RGBA16F);
  FOO(RGB16F);
||||||| merged common ancestors
    FOO(RGBA16F);
    FOO(RGB16F);
=======
  FOO(RGBA16F, true);
  FOO(RGB16F, false);  // It's not required, thus not portable. (Also there's a
                       // wicked driver bug on Mac+Intel)
>>>>>>> upstream-releases

#undef FOO
}

<<<<<<< HEAD
WebGLExtensionColorBufferHalfFloat::~WebGLExtensionColorBufferHalfFloat() {}
||||||| merged common ancestors
WebGLExtensionColorBufferHalfFloat::~WebGLExtensionColorBufferHalfFloat()
{
}
=======
WebGLExtensionColorBufferHalfFloat::~WebGLExtensionColorBufferHalfFloat() {}

bool WebGLExtensionColorBufferHalfFloat::IsSupported(
    const WebGLContext* webgl) {
  if (webgl->IsWebGL2()) return false;
>>>>>>> upstream-releases

<<<<<<< HEAD
bool WebGLExtensionColorBufferHalfFloat::IsSupported(
    const WebGLContext* webgl) {
  const auto& gl = webgl->gl;
  return gl->IsSupported(gl::GLFeature::renderbuffer_color_half_float) &&
         gl->IsSupported(gl::GLFeature::frag_color_float);
||||||| merged common ancestors
bool
WebGLExtensionColorBufferHalfFloat::IsSupported(const WebGLContext* webgl)
{
    const auto& gl = webgl->gl;
    return gl->IsSupported(gl::GLFeature::renderbuffer_color_half_float) &&
           gl->IsSupported(gl::GLFeature::frag_color_float);
=======
  const auto& gl = webgl->gl;
  return gl->IsSupported(gl::GLFeature::renderbuffer_color_half_float) &&
         gl->IsSupported(gl::GLFeature::frag_color_float);
>>>>>>> upstream-releases
}

IMPL_WEBGL_EXTENSION_GOOP(WebGLExtensionColorBufferHalfFloat,
                          EXT_color_buffer_half_float)

}  // namespace mozilla
