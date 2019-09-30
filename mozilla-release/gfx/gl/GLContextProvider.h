/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GLCONTEXTPROVIDER_H_
#define GLCONTEXTPROVIDER_H_

#include "GLContextTypes.h"
#include "SurfaceTypes.h"

#include "nsSize.h"  // for gfx::IntSize (needed by GLContextProviderImpl.h below)

class nsIWidget;

namespace mozilla {
namespace widget {
class CompositorWidget;
}
namespace gl {

#define IN_GL_CONTEXT_PROVIDER_H

// Null is always there
#define GL_CONTEXT_PROVIDER_NAME GLContextProviderNull
#include "GLContextProviderImpl.h"
#undef GL_CONTEXT_PROVIDER_NAME

#ifdef XP_WIN
<<<<<<< HEAD
#define GL_CONTEXT_PROVIDER_NAME GLContextProviderWGL
#include "GLContextProviderImpl.h"
#undef GL_CONTEXT_PROVIDER_NAME
#define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderWGL
#define DEFAULT_IMPL WGL
||||||| merged common ancestors
  #define GL_CONTEXT_PROVIDER_NAME GLContextProviderWGL
  #include "GLContextProviderImpl.h"
  #undef GL_CONTEXT_PROVIDER_NAME
  #define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderWGL
  #define DEFAULT_IMPL WGL
=======
#  define GL_CONTEXT_PROVIDER_NAME GLContextProviderWGL
#  include "GLContextProviderImpl.h"
#  undef GL_CONTEXT_PROVIDER_NAME
#  define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderWGL
#  define DEFAULT_IMPL WGL
>>>>>>> upstream-releases
#endif

#ifdef XP_MACOSX
<<<<<<< HEAD
#define GL_CONTEXT_PROVIDER_NAME GLContextProviderCGL
#include "GLContextProviderImpl.h"
#undef GL_CONTEXT_PROVIDER_NAME
#define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderCGL
||||||| merged common ancestors
  #define GL_CONTEXT_PROVIDER_NAME GLContextProviderCGL
  #include "GLContextProviderImpl.h"
  #undef GL_CONTEXT_PROVIDER_NAME
  #define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderCGL
=======
#  define GL_CONTEXT_PROVIDER_NAME GLContextProviderCGL
#  include "GLContextProviderImpl.h"
#  undef GL_CONTEXT_PROVIDER_NAME
#  define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderCGL
>>>>>>> upstream-releases
#endif

#if defined(MOZ_X11)
<<<<<<< HEAD
#define GL_CONTEXT_PROVIDER_NAME GLContextProviderGLX
#include "GLContextProviderImpl.h"
#undef GL_CONTEXT_PROVIDER_NAME
#define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderGLX
||||||| merged common ancestors
  #define GL_CONTEXT_PROVIDER_NAME GLContextProviderGLX
  #include "GLContextProviderImpl.h"
  #undef GL_CONTEXT_PROVIDER_NAME
  #define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderGLX
=======
#  define GL_CONTEXT_PROVIDER_NAME GLContextProviderGLX
#  include "GLContextProviderImpl.h"
#  undef GL_CONTEXT_PROVIDER_NAME
#  define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderGLX
>>>>>>> upstream-releases
#endif

#define GL_CONTEXT_PROVIDER_NAME GLContextProviderEGL
#include "GLContextProviderImpl.h"
#undef GL_CONTEXT_PROVIDER_NAME
#ifndef GL_CONTEXT_PROVIDER_DEFAULT
<<<<<<< HEAD
#define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderEGL
||||||| merged common ancestors
  #define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderEGL
=======
#  define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderEGL
>>>>>>> upstream-releases
#endif

#if defined(MOZ_WAYLAND)
<<<<<<< HEAD
#define GL_CONTEXT_PROVIDER_NAME GLContextProviderWayland
#include "GLContextProviderImpl.h"
#undef GL_CONTEXT_PROVIDER_NAME
#undef GL_CONTEXT_PROVIDER_DEFAULT
#define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderWayland
||||||| merged common ancestors
  #define GL_CONTEXT_PROVIDER_NAME GLContextProviderWayland
  #include "GLContextProviderImpl.h"
  #undef GL_CONTEXT_PROVIDER_NAME
  #undef GL_CONTEXT_PROVIDER_DEFAULT
  #define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderWayland
=======
#  define GL_CONTEXT_PROVIDER_NAME GLContextProviderWayland
#  include "GLContextProviderImpl.h"
#  undef GL_CONTEXT_PROVIDER_NAME
#  undef GL_CONTEXT_PROVIDER_DEFAULT
#  define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderWayland
>>>>>>> upstream-releases
#endif

#if defined(MOZ_WIDGET_UIKIT)
#  define GL_CONTEXT_PROVIDER_NAME GLContextProviderEAGL
#  include "GLContextProviderImpl.h"
#  undef GL_CONTEXT_PROVIDER_NAME
#  ifndef GL_CONTEXT_PROVIDER_DEFAULT
#    define GL_CONTEXT_PROVIDER_DEFAULT GLContextProviderEAGL
#  endif
#endif

#ifdef MOZ_GL_PROVIDER
<<<<<<< HEAD
#define GL_CONTEXT_PROVIDER_NAME MOZ_GL_PROVIDER
#include "GLContextProviderImpl.h"
#undef GL_CONTEXT_PROVIDER_NAME
#define GL_CONTEXT_PROVIDER_DEFAULT MOZ_GL_PROVIDER
||||||| merged common ancestors
  #define GL_CONTEXT_PROVIDER_NAME MOZ_GL_PROVIDER
  #include "GLContextProviderImpl.h"
  #undef GL_CONTEXT_PROVIDER_NAME
  #define GL_CONTEXT_PROVIDER_DEFAULT MOZ_GL_PROVIDER
=======
#  define GL_CONTEXT_PROVIDER_NAME MOZ_GL_PROVIDER
#  include "GLContextProviderImpl.h"
#  undef GL_CONTEXT_PROVIDER_NAME
#  define GL_CONTEXT_PROVIDER_DEFAULT MOZ_GL_PROVIDER
>>>>>>> upstream-releases
#endif

#ifdef GL_CONTEXT_PROVIDER_DEFAULT
typedef GL_CONTEXT_PROVIDER_DEFAULT GLContextProvider;
#else
typedef GLContextProviderNull GLContextProvider;
#endif

#undef IN_GL_CONTEXT_PROVIDER_H

}  // namespace gl
}  // namespace mozilla

#endif
