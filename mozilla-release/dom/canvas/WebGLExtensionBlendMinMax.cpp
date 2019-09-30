/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "WebGLExtensions.h"

#include "GLContext.h"
#include "mozilla/dom/WebGLRenderingContextBinding.h"
#include "WebGLContext.h"

namespace mozilla {

WebGLExtensionBlendMinMax::WebGLExtensionBlendMinMax(WebGLContext* webgl)
    : WebGLExtensionBase(webgl) {
  MOZ_ASSERT(IsSupported(webgl), "Don't construct extension if unsupported.");
}

<<<<<<< HEAD
WebGLExtensionBlendMinMax::~WebGLExtensionBlendMinMax() {}
||||||| merged common ancestors
WebGLExtensionBlendMinMax::~WebGLExtensionBlendMinMax()
{
}
=======
WebGLExtensionBlendMinMax::~WebGLExtensionBlendMinMax() {}

bool WebGLExtensionBlendMinMax::IsSupported(const WebGLContext* webgl) {
  if (webgl->IsWebGL2()) return false;
>>>>>>> upstream-releases

<<<<<<< HEAD
bool WebGLExtensionBlendMinMax::IsSupported(const WebGLContext* webgl) {
  return webgl->GL()->IsSupported(gl::GLFeature::blend_minmax);
||||||| merged common ancestors
bool
WebGLExtensionBlendMinMax::IsSupported(const WebGLContext* webgl)
{
    return webgl->GL()->IsSupported(gl::GLFeature::blend_minmax);
=======
  return webgl->GL()->IsSupported(gl::GLFeature::blend_minmax);
>>>>>>> upstream-releases
}

IMPL_WEBGL_EXTENSION_GOOP(WebGLExtensionBlendMinMax, EXT_blend_minmax)

}  // namespace mozilla
