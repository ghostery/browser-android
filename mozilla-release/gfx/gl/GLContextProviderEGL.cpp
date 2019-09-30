/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if defined(MOZ_WIDGET_GTK)
<<<<<<< HEAD
#define GET_NATIVE_WINDOW_FROM_REAL_WIDGET(aWidget) \
  ((EGLNativeWindowType)aWidget->GetNativeData(NS_NATIVE_EGL_WINDOW))
#define GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget)     \
  ((EGLNativeWindowType)aWidget->RealWidget()->GetNativeData( \
      NS_NATIVE_EGL_WINDOW))
||||||| merged common ancestors
    #define GET_NATIVE_WINDOW_FROM_REAL_WIDGET(aWidget) ((EGLNativeWindowType)aWidget->GetNativeData(NS_NATIVE_EGL_WINDOW))
    #define GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget) ((EGLNativeWindowType)aWidget->RealWidget()->GetNativeData(NS_NATIVE_EGL_WINDOW))
=======
#  define GET_NATIVE_WINDOW_FROM_REAL_WIDGET(aWidget) \
    ((EGLNativeWindowType)aWidget->GetNativeData(NS_NATIVE_EGL_WINDOW))
#  define GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget) \
    (aWidget->AsX11()->GetEGLNativeWindow())
>>>>>>> upstream-releases
#elif defined(MOZ_WIDGET_ANDROID)
<<<<<<< HEAD
#define GET_NATIVE_WINDOW_FROM_REAL_WIDGET(aWidget) \
  ((EGLNativeWindowType)aWidget->GetNativeData(NS_JAVA_SURFACE))
#define GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget) \
  (aWidget->AsAndroid()->GetEGLNativeWindow())
||||||| merged common ancestors
    #define GET_NATIVE_WINDOW_FROM_REAL_WIDGET(aWidget) ((EGLNativeWindowType)aWidget->GetNativeData(NS_JAVA_SURFACE))
    #define GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget) (aWidget->AsAndroid()->GetEGLNativeWindow())
=======
#  define GET_NATIVE_WINDOW_FROM_REAL_WIDGET(aWidget) \
    ((EGLNativeWindowType)aWidget->GetNativeData(NS_JAVA_SURFACE))
#  define GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget) \
    (aWidget->AsAndroid()->GetEGLNativeWindow())
>>>>>>> upstream-releases
#elif defined(XP_WIN)
<<<<<<< HEAD
#define GET_NATIVE_WINDOW_FROM_REAL_WIDGET(aWidget) \
  ((EGLNativeWindowType)aWidget->GetNativeData(NS_NATIVE_WINDOW))
#define GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget) \
  ((EGLNativeWindowType)aWidget->AsWindows()->GetHwnd())
||||||| merged common ancestors
    #define GET_NATIVE_WINDOW_FROM_REAL_WIDGET(aWidget) ((EGLNativeWindowType)aWidget->GetNativeData(NS_NATIVE_WINDOW))
    #define GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget) ((EGLNativeWindowType)aWidget->AsWindows()->GetHwnd())
=======
#  define GET_NATIVE_WINDOW_FROM_REAL_WIDGET(aWidget) \
    ((EGLNativeWindowType)aWidget->GetNativeData(NS_NATIVE_WINDOW))
#  define GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget) \
    ((EGLNativeWindowType)aWidget->AsWindows()->GetHwnd())
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
#define GET_NATIVE_WINDOW_FROM_REAL_WIDGET(aWidget) \
  ((EGLNativeWindowType)aWidget->GetNativeData(NS_NATIVE_WINDOW))
#define GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget) \
  ((EGLNativeWindowType)aWidget->RealWidget()->GetNativeData(NS_NATIVE_WINDOW))
||||||| merged common ancestors
    #define GET_NATIVE_WINDOW_FROM_REAL_WIDGET(aWidget) ((EGLNativeWindowType)aWidget->GetNativeData(NS_NATIVE_WINDOW))
    #define GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget) ((EGLNativeWindowType)aWidget->RealWidget()->GetNativeData(NS_NATIVE_WINDOW))
=======
#  define GET_NATIVE_WINDOW_FROM_REAL_WIDGET(aWidget) \
    ((EGLNativeWindowType)aWidget->GetNativeData(NS_NATIVE_WINDOW))
#  define GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget)     \
    ((EGLNativeWindowType)aWidget->RealWidget()->GetNativeData( \
        NS_NATIVE_WINDOW))
>>>>>>> upstream-releases
#endif

#if defined(XP_UNIX)
<<<<<<< HEAD
#ifdef MOZ_WIDGET_ANDROID
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "mozilla/widget/AndroidCompositorWidget.h"
#endif
||||||| merged common ancestors
    #ifdef MOZ_WIDGET_ANDROID
        #include <android/native_window.h>
        #include <android/native_window_jni.h>
        #include "mozilla/widget/AndroidCompositorWidget.h"
    #endif
=======
#  ifdef MOZ_WIDGET_ANDROID
#    include <android/native_window.h>
#    include <android/native_window_jni.h>
#    include "mozilla/widget/AndroidCompositorWidget.h"
#  endif
>>>>>>> upstream-releases

<<<<<<< HEAD
#define GLES2_LIB "libGLESv2.so"
#define GLES2_LIB2 "libGLESv2.so.2"
||||||| merged common ancestors
    #define GLES2_LIB "libGLESv2.so"
    #define GLES2_LIB2 "libGLESv2.so.2"
=======
#  define GLES2_LIB "libGLESv2.so"
#  define GLES2_LIB2 "libGLESv2.so.2"
>>>>>>> upstream-releases

#elif defined(XP_WIN)
<<<<<<< HEAD
#include "mozilla/widget/WinCompositorWidget.h"
#include "nsIFile.h"
||||||| merged common ancestors
    #include "mozilla/widget/WinCompositorWidget.h"
    #include "nsIFile.h"
=======
#  include "mozilla/widget/WinCompositorWidget.h"
#  include "nsIFile.h"
>>>>>>> upstream-releases

<<<<<<< HEAD
#define GLES2_LIB "libGLESv2.dll"
||||||| merged common ancestors
    #define GLES2_LIB "libGLESv2.dll"
=======
#  define GLES2_LIB "libGLESv2.dll"
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
||||||| merged common ancestors
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN 1
    #endif
=======
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN 1
#  endif
>>>>>>> upstream-releases

<<<<<<< HEAD
#include <windows.h>
||||||| merged common ancestors
    #include <windows.h>
=======
#  include <windows.h>
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
#error "Platform not recognized"
||||||| merged common ancestors
    #error "Platform not recognized"
=======
#  error "Platform not recognized"
>>>>>>> upstream-releases
#endif

#include "gfxASurface.h"
#include "gfxCrashReporterUtils.h"
#include "gfxFailure.h"
#include "gfxPlatform.h"
#include "gfxUtils.h"
#include "GLBlitHelper.h"
#include "GLContextEGL.h"
#include "GLContextProvider.h"
#include "GLLibraryEGL.h"
#include "LayersLogging.h"
#include "mozilla/ArrayUtils.h"
#include "mozilla/Preferences.h"
#include "mozilla/gfx/gfxVars.h"
#include "mozilla/layers/CompositorOptions.h"
#include "mozilla/widget/CompositorWidget.h"
#include "nsDebug.h"
#include "nsIWidget.h"
#include "nsThreadUtils.h"
#include "ScopedGLHelpers.h"
#include "TextureImageEGL.h"

#if defined(MOZ_WIDGET_GTK)
#  include "mozilla/widget/GtkCompositorWidget.h"
#endif

#if defined(MOZ_WAYLAND)
#  include "nsAutoPtr.h"
#  include "nsDataHashtable.h"

#  include <gtk/gtk.h>
#  include <gdk/gdkx.h>
#  include <gdk/gdkwayland.h>
#  include <wayland-egl.h>
#  include <dlfcn.h>
#endif

using namespace mozilla::gfx;

namespace mozilla {
namespace gl {

using namespace mozilla::widget;

#if defined(MOZ_WAYLAND)
class WaylandGLSurface {
 public:
  WaylandGLSurface(struct wl_surface* aWaylandSurface,
                   struct wl_egl_window* aEGLWindow);
  ~WaylandGLSurface();

 private:
  struct wl_surface* mWaylandSurface;
  struct wl_egl_window* mEGLWindow;
};

<<<<<<< HEAD
static nsDataHashtable<nsPtrHashKey<void>, WaylandGLSurface*> sWaylandGLSurface;

void DeleteWaylandGLSurface(EGLSurface surface) {
  // We're running on Wayland which means our EGLSurface may
  // have attached Wayland backend data which must be released.
  if (!GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
    auto entry = sWaylandGLSurface.Lookup(surface);
    if (entry) {
      delete entry.Data();
      entry.Remove();
||||||| merged common ancestors
static nsDataHashtable<nsPtrHashKey<void>, WaylandGLSurface*>
        sWaylandGLSurface;

void
DeleteWaylandGLSurface(EGLSurface surface)
{
    // We're running on Wayland which means our EGLSurface may
    // have attached Wayland backend data which must be released.
    if (GDK_IS_WAYLAND_DISPLAY(gdk_display_get_default())) {
        auto entry = sWaylandGLSurface.Lookup(surface);
        if (entry) {
            delete entry.Data();
            entry.Remove();
        }
=======
static nsDataHashtable<nsPtrHashKey<void>, WaylandGLSurface*> sWaylandGLSurface;

void DeleteWaylandGLSurface(EGLSurface surface) {
  // We're running on Wayland which means our EGLSurface may
  // have attached Wayland backend data which must be released.
  if (!GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
    auto entry = sWaylandGLSurface.Lookup(surface);
    if (entry) {
      delete entry.Data();
      entry.Remove();
>>>>>>> upstream-releases
    }
  }
}
#endif

#define ADD_ATTR_2(_array, _k, _v) \
  do {                             \
    (_array).AppendElement(_k);    \
    (_array).AppendElement(_v);    \
  } while (0)

#define ADD_ATTR_1(_array, _k)  \
  do {                          \
    (_array).AppendElement(_k); \
  } while (0)

<<<<<<< HEAD
static bool CreateConfig(EGLConfig* aConfig, bool aEnableDepthBuffer);
||||||| merged common ancestors
static bool
CreateConfig(EGLConfig* aConfig, bool aEnableDepthBuffer);
=======
static bool CreateConfig(GLLibraryEGL*, EGLConfig* aConfig,
                         bool aEnableDepthBuffer);
>>>>>>> upstream-releases

// append three zeros at the end of attribs list to work around
// EGL implementation bugs that iterate until they find 0, instead of
// EGL_NONE. See bug 948406.
#define EGL_ATTRIBS_LIST_SAFE_TERMINATION_WORKING_AROUND_BUGS \
  LOCAL_EGL_NONE, 0, 0, 0

static EGLint kTerminationAttribs[] = {
    EGL_ATTRIBS_LIST_SAFE_TERMINATION_WORKING_AROUND_BUGS};

static int next_power_of_two(int v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;

  return v;
}

static bool is_power_of_two(int v) {
  NS_ASSERTION(v >= 0, "bad value");

  if (v == 0) return true;

  return (v & (v - 1)) == 0;
}

<<<<<<< HEAD
static void DestroySurface(EGLSurface oldSurface) {
  auto* egl = gl::GLLibraryEGL::Get();

  if (oldSurface != EGL_NO_SURFACE) {
    // TODO: This breaks TLS MakeCurrent caching.
    egl->fMakeCurrent(EGL_DISPLAY(), EGL_NO_SURFACE, EGL_NO_SURFACE,
                      EGL_NO_CONTEXT);
    egl->fDestroySurface(EGL_DISPLAY(), oldSurface);
||||||| merged common ancestors
static void
DestroySurface(EGLSurface oldSurface) {
    auto* egl = gl::GLLibraryEGL::Get();

    if (oldSurface != EGL_NO_SURFACE) {
        // TODO: This breaks TLS MakeCurrent caching.
        egl->fMakeCurrent(EGL_DISPLAY(),
                          EGL_NO_SURFACE, EGL_NO_SURFACE,
                          EGL_NO_CONTEXT);
        egl->fDestroySurface(EGL_DISPLAY(), oldSurface);
=======
static void DestroySurface(GLLibraryEGL* const egl,
                           const EGLSurface oldSurface) {
  if (oldSurface != EGL_NO_SURFACE) {
    // TODO: This breaks TLS MakeCurrent caching.
    egl->fMakeCurrent(egl->Display(), EGL_NO_SURFACE, EGL_NO_SURFACE,
                      EGL_NO_CONTEXT);
    egl->fDestroySurface(egl->Display(), oldSurface);
>>>>>>> upstream-releases
#if defined(MOZ_WAYLAND)
    DeleteWaylandGLSurface(oldSurface);
#endif
  }
}

<<<<<<< HEAD
static EGLSurface CreateFallbackSurface(const EGLConfig& config) {
  nsCString discardFailureId;
  if (!GLLibraryEGL::EnsureInitialized(false, &discardFailureId)) {
    gfxCriticalNote << "Failed to load EGL library 3!";
    return EGL_NO_SURFACE;
  }

  auto* egl = gl::GLLibraryEGL::Get();

  if (egl->IsExtensionSupported(GLLibraryEGL::KHR_surfaceless_context)) {
    // We don't need a PBuffer surface in this case
    return EGL_NO_SURFACE;
  }

  std::vector<EGLint> pbattrs;
  pbattrs.push_back(LOCAL_EGL_WIDTH);
  pbattrs.push_back(1);
  pbattrs.push_back(LOCAL_EGL_HEIGHT);
  pbattrs.push_back(1);

  for (const auto& cur : kTerminationAttribs) {
    pbattrs.push_back(cur);
  }

  EGLSurface surface =
      egl->fCreatePbufferSurface(EGL_DISPLAY(), config, pbattrs.data());
  if (!surface) {
    MOZ_CRASH("Failed to create fallback EGLSurface");
  }

  return surface;
||||||| merged common ancestors
static EGLSurface
CreateFallbackSurface(const EGLConfig& config)
{
    nsCString discardFailureId;
    if (!GLLibraryEGL::EnsureInitialized(false, &discardFailureId)) {
        gfxCriticalNote << "Failed to load EGL library 3!";
        return EGL_NO_SURFACE;
    }

    auto* egl = gl::GLLibraryEGL::Get();

    if (egl->IsExtensionSupported(GLLibraryEGL::KHR_surfaceless_context)) {
        // We don't need a PBuffer surface in this case
        return EGL_NO_SURFACE;
    }

    std::vector<EGLint> pbattrs;
    pbattrs.push_back(LOCAL_EGL_WIDTH); pbattrs.push_back(1);
    pbattrs.push_back(LOCAL_EGL_HEIGHT); pbattrs.push_back(1);

    for (const auto& cur : kTerminationAttribs) {
        pbattrs.push_back(cur);
    }

    EGLSurface surface = egl->fCreatePbufferSurface(EGL_DISPLAY(), config, pbattrs.data());
    if (!surface) {
        MOZ_CRASH("Failed to create fallback EGLSurface");
    }

    return surface;
=======
static EGLSurface CreateFallbackSurface(GLLibraryEGL* const egl,
                                        const EGLConfig& config) {
  nsCString discardFailureId;
  if (!GLLibraryEGL::EnsureInitialized(false, &discardFailureId)) {
    gfxCriticalNote << "Failed to load EGL library 3!";
    return EGL_NO_SURFACE;
  }

  if (egl->IsExtensionSupported(GLLibraryEGL::KHR_surfaceless_context)) {
    // We don't need a PBuffer surface in this case
    return EGL_NO_SURFACE;
  }

  std::vector<EGLint> pbattrs;
  pbattrs.push_back(LOCAL_EGL_WIDTH);
  pbattrs.push_back(1);
  pbattrs.push_back(LOCAL_EGL_HEIGHT);
  pbattrs.push_back(1);

  for (const auto& cur : kTerminationAttribs) {
    pbattrs.push_back(cur);
  }

  EGLSurface surface =
      egl->fCreatePbufferSurface(egl->Display(), config, pbattrs.data());
  if (!surface) {
    MOZ_CRASH("Failed to create fallback EGLSurface");
  }

  return surface;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static EGLSurface CreateSurfaceFromNativeWindow(EGLNativeWindowType window,
                                                const EGLConfig& config) {
  MOZ_ASSERT(window);
  auto* egl = gl::GLLibraryEGL::Get();
  EGLSurface newSurface = EGL_NO_SURFACE;
||||||| merged common ancestors
static EGLSurface
CreateSurfaceFromNativeWindow(EGLNativeWindowType window, const EGLConfig& config)
{
    MOZ_ASSERT(window);
    auto* egl = gl::GLLibraryEGL::Get();
    EGLSurface newSurface = EGL_NO_SURFACE;
=======
static EGLSurface CreateSurfaceFromNativeWindow(
    GLLibraryEGL* const egl, const EGLNativeWindowType window,
    const EGLConfig config) {
  MOZ_ASSERT(window);
  EGLSurface newSurface = EGL_NO_SURFACE;
>>>>>>> upstream-releases

#ifdef MOZ_WIDGET_ANDROID
  JNIEnv* const env = jni::GetEnvForThread();
  ANativeWindow* const nativeWindow =
      ANativeWindow_fromSurface(env, reinterpret_cast<jobject>(window));
  newSurface = egl->fCreateWindowSurface(egl->fGetDisplay(EGL_DEFAULT_DISPLAY),
                                         config, nativeWindow, 0);
  ANativeWindow_release(nativeWindow);
#else
<<<<<<< HEAD
  newSurface = egl->fCreateWindowSurface(EGL_DISPLAY(), config, window, 0);
||||||| merged common ancestors
    newSurface = egl->fCreateWindowSurface(EGL_DISPLAY(), config,
                                           window, 0);
=======
  newSurface = egl->fCreateWindowSurface(egl->Display(), config, window, 0);
>>>>>>> upstream-releases
#endif
  return newSurface;
}

/* GLContextEGLFactory class was added as a friend of GLContextEGL
 * so that it could access  GLContextEGL::CreateGLContext. This was
 * done so that a new function would not need to be added to the shared
 * GLContextProvider interface.
 */
class GLContextEGLFactory {
<<<<<<< HEAD
 public:
  static already_AddRefed<GLContext> Create(EGLNativeWindowType aWindow,
                                            bool aWebRender);

 private:
  GLContextEGLFactory() {}
  ~GLContextEGLFactory() {}
};
||||||| merged common ancestors
public:
    static already_AddRefed<GLContext> Create(EGLNativeWindowType aWindow,
                                              bool aWebRender);
private:
    GLContextEGLFactory(){}
    ~GLContextEGLFactory(){}
};

already_AddRefed<GLContext>
GLContextEGLFactory::Create(EGLNativeWindowType aWindow,
                            bool aWebRender)
{
    nsCString discardFailureId;
    if (!GLLibraryEGL::EnsureInitialized(false, &discardFailureId)) {
        gfxCriticalNote << "Failed to load EGL library 3!";
        return nullptr;
    }

    auto* egl = gl::GLLibraryEGL::Get();
    bool doubleBuffered = true;

    EGLConfig config;
    if (aWebRender && egl->IsANGLE()) {
        // Force enable alpha channel to make sure ANGLE use correct framebuffer formart
        const int bpp = 32;
        const bool withDepth = true;
        if (!CreateConfig(&config, bpp, withDepth)) {
            gfxCriticalNote << "Failed to create EGLConfig for WebRender ANGLE!";
            return nullptr;
        }
    } else {
        if (!CreateConfig(&config, aWebRender)) {
            gfxCriticalNote << "Failed to create EGLConfig!";
            return nullptr;
        }
    }

    EGLSurface surface = EGL_NO_SURFACE;
    if (aWindow) {
        surface = mozilla::gl::CreateSurfaceFromNativeWindow(aWindow, config);
    }
=======
 public:
  static already_AddRefed<GLContext> Create(EGLNativeWindowType aWindow,
                                            bool aWebRender);

 private:
  GLContextEGLFactory() {}
  ~GLContextEGLFactory() {}
};

already_AddRefed<GLContext> GLContextEGLFactory::Create(
    EGLNativeWindowType aWindow, bool aWebRender) {
  nsCString discardFailureId;
  if (!GLLibraryEGL::EnsureInitialized(false, &discardFailureId)) {
    gfxCriticalNote << "Failed to load EGL library 3!";
    return nullptr;
  }

  auto* egl = gl::GLLibraryEGL::Get();
  bool doubleBuffered = true;

  EGLConfig config;
  if (aWebRender && egl->IsANGLE()) {
    // Force enable alpha channel to make sure ANGLE use correct framebuffer
    // formart
    const int bpp = 32;
    const bool withDepth = true;
    if (!CreateConfig(egl, &config, bpp, withDepth)) {
      gfxCriticalNote << "Failed to create EGLConfig for WebRender ANGLE!";
      return nullptr;
    }
  } else {
    if (!CreateConfig(egl, &config, aWebRender)) {
      gfxCriticalNote << "Failed to create EGLConfig!";
      return nullptr;
    }
  }

  EGLSurface surface = EGL_NO_SURFACE;
  if (aWindow) {
    surface = mozilla::gl::CreateSurfaceFromNativeWindow(egl, aWindow, config);
  }

  CreateContextFlags flags = CreateContextFlags::NONE;
  if (aWebRender) {
    flags |= CreateContextFlags::PREFER_ES3;
  }
  SurfaceCaps caps = SurfaceCaps::Any();
  RefPtr<GLContextEGL> gl = GLContextEGL::CreateGLContext(
      egl, flags, caps, false, config, surface, &discardFailureId);
  if (!gl) {
    const auto err = egl->fGetError();
    gfxCriticalNote << "Failed to create EGLContext!: " << gfx::hexa(err);
    mozilla::gl::DestroySurface(egl, surface);
    return nullptr;
  }

  gl->MakeCurrent();
  gl->SetIsDoubleBuffered(doubleBuffered);
>>>>>>> upstream-releases

<<<<<<< HEAD
already_AddRefed<GLContext> GLContextEGLFactory::Create(
    EGLNativeWindowType aWindow, bool aWebRender) {
  nsCString discardFailureId;
  if (!GLLibraryEGL::EnsureInitialized(false, &discardFailureId)) {
    gfxCriticalNote << "Failed to load EGL library 3!";
    return nullptr;
  }

  auto* egl = gl::GLLibraryEGL::Get();
  bool doubleBuffered = true;

  EGLConfig config;
  if (aWebRender && egl->IsANGLE()) {
    // Force enable alpha channel to make sure ANGLE use correct framebuffer
    // formart
    const int bpp = 32;
    const bool withDepth = true;
    if (!CreateConfig(&config, bpp, withDepth)) {
      gfxCriticalNote << "Failed to create EGLConfig for WebRender ANGLE!";
      return nullptr;
    }
  } else {
    if (!CreateConfig(&config, aWebRender)) {
      gfxCriticalNote << "Failed to create EGLConfig!";
      return nullptr;
    }
  }

  EGLSurface surface = EGL_NO_SURFACE;
  if (aWindow) {
    surface = mozilla::gl::CreateSurfaceFromNativeWindow(aWindow, config);
  }

  CreateContextFlags flags = CreateContextFlags::NONE;
  if (aWebRender) {
    flags |= CreateContextFlags::PREFER_ES3;
  }
  SurfaceCaps caps = SurfaceCaps::Any();
  RefPtr<GLContextEGL> gl = GLContextEGL::CreateGLContext(
      flags, caps, false, config, surface, &discardFailureId);
  if (!gl) {
    gfxCriticalNote << "Failed to create EGLContext!";
    mozilla::gl::DestroySurface(surface);
    return nullptr;
  }

  gl->MakeCurrent();
  gl->SetIsDoubleBuffered(doubleBuffered);
  if (aWebRender && egl->IsANGLE()) {
    MOZ_ASSERT(doubleBuffered);
    egl->fSwapInterval(EGL_DISPLAY(), 0);
  }
  return gl.forget();
||||||| merged common ancestors
    CreateContextFlags flags = CreateContextFlags::NONE;
    if (aWebRender) {
        flags |= CreateContextFlags::PREFER_ES3;
    }
    SurfaceCaps caps = SurfaceCaps::Any();
    RefPtr<GLContextEGL> gl = GLContextEGL::CreateGLContext(flags, caps, false, config,
                                                            surface, &discardFailureId);
    if (!gl) {
        gfxCriticalNote << "Failed to create EGLContext!";
        mozilla::gl::DestroySurface(surface);
        return nullptr;
    }

    gl->MakeCurrent();
    gl->SetIsDoubleBuffered(doubleBuffered);
    if (aWebRender && egl->IsANGLE()) {
        MOZ_ASSERT(doubleBuffered);
        egl->fSwapInterval(EGL_DISPLAY(), 0);
    }
    return gl.forget();
=======
#if defined(MOZ_WAYLAND)
  if (surface != EGL_NO_SURFACE &&
      !GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
    // Make eglSwapBuffers() non-blocking on wayland
    egl->fSwapInterval(egl->Display(), 0);
  }
#endif
  if (aWebRender && egl->IsANGLE()) {
    MOZ_ASSERT(doubleBuffered);
    egl->fSwapInterval(egl->Display(), 0);
  }
  return gl.forget();
}

#if defined(MOZ_WAYLAND) || defined(MOZ_WIDGET_ANDROID)
/* static */
EGLSurface GLContextEGL::CreateEGLSurfaceForCompositorWidget(
    widget::CompositorWidget* aCompositorWidget, const EGLConfig aConfig) {
  nsCString discardFailureId;
  if (!GLLibraryEGL::EnsureInitialized(false, &discardFailureId)) {
    gfxCriticalNote << "Failed to load EGL library 6!";
    return EGL_NO_SURFACE;
  }

  MOZ_ASSERT(aCompositorWidget);
  EGLNativeWindowType window =
      GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aCompositorWidget);
  if (!window) {
    gfxCriticalNote << "window is null";
    return EGL_NO_SURFACE;
  }

  const auto& egl = GLLibraryEGL::Get();
  return mozilla::gl::CreateSurfaceFromNativeWindow(egl, window, aConfig);
>>>>>>> upstream-releases
}
#endif

<<<<<<< HEAD
GLContextEGL::GLContextEGL(CreateContextFlags flags, const SurfaceCaps& caps,
                           bool isOffscreen, EGLConfig config,
                           EGLSurface surface, EGLContext context)
    : GLContext(flags, caps, nullptr, isOffscreen, false),
      mConfig(config),
      mEgl(gl::GLLibraryEGL::Get()),
      mSurface(surface),
      mFallbackSurface(CreateFallbackSurface(config)),
      mContext(context) {
||||||| merged common ancestors
GLContextEGL::GLContextEGL(CreateContextFlags flags, const SurfaceCaps& caps,
                           bool isOffscreen, EGLConfig config, EGLSurface surface,
                           EGLContext context)
    : GLContext(flags, caps, nullptr, isOffscreen, false)
    , mConfig(config)
    , mEgl(gl::GLLibraryEGL::Get())
    , mSurface(surface)
    , mFallbackSurface(CreateFallbackSurface(config))
    , mContext(context)
{
=======
GLContextEGL::GLContextEGL(GLLibraryEGL* const egl, CreateContextFlags flags,
                           const SurfaceCaps& caps, bool isOffscreen,
                           EGLConfig config, EGLSurface surface,
                           EGLContext context)
    : GLContext(flags, caps, nullptr, isOffscreen, false),
      mEgl(egl),
      mConfig(config),
      mContext(context),
      mSurface(surface),
      mFallbackSurface(CreateFallbackSurface(mEgl, mConfig)) {
>>>>>>> upstream-releases
#ifdef DEBUG
<<<<<<< HEAD
  printf_stderr("Initializing context %p surface %p on display %p\n", mContext,
                mSurface, EGL_DISPLAY());
||||||| merged common ancestors
    printf_stderr("Initializing context %p surface %p on display %p\n", mContext, mSurface, EGL_DISPLAY());
=======
  printf_stderr("Initializing context %p surface %p on display %p\n", mContext,
                mSurface, mEgl->Display());
>>>>>>> upstream-releases
#endif
}

<<<<<<< HEAD
void
GLContextEGL::OnMarkDestroyed() {
  if (mSurfaceOverride != EGL_NO_SURFACE) {
    SetEGLSurfaceOverride(EGL_NO_SURFACE);
  }
}
||||||| merged common ancestors
GLContextEGL::~GLContextEGL()
{
    MarkDestroyed();
=======
void GLContextEGL::OnMarkDestroyed() {
  if (mSurfaceOverride != EGL_NO_SURFACE) {
    SetEGLSurfaceOverride(EGL_NO_SURFACE);
  }
}
>>>>>>> upstream-releases

GLContextEGL::~GLContextEGL() {
  MarkDestroyed();

  // Wrapped context should not destroy eglContext/Surface
  if (!mOwnsContext) {
    return;
  }

#ifdef DEBUG
<<<<<<< HEAD
  printf_stderr("Destroying context %p surface %p on display %p\n", mContext,
                mSurface, EGL_DISPLAY());
||||||| merged common ancestors
    printf_stderr("Destroying context %p surface %p on display %p\n", mContext, mSurface, EGL_DISPLAY());
=======
  printf_stderr("Destroying context %p surface %p on display %p\n", mContext,
                mSurface, mEgl->Display());
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  mEgl->fDestroyContext(EGL_DISPLAY(), mContext);
||||||| merged common ancestors
    mEgl->fDestroyContext(EGL_DISPLAY(), mContext);
=======
  mEgl->fDestroyContext(mEgl->Display(), mContext);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::gl::DestroySurface(mSurface);
  mozilla::gl::DestroySurface(mFallbackSurface);
||||||| merged common ancestors
    mozilla::gl::DestroySurface(mSurface);
    mozilla::gl::DestroySurface(mFallbackSurface);
=======
  mozilla::gl::DestroySurface(mEgl, mSurface);
  mozilla::gl::DestroySurface(mEgl, mFallbackSurface);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool GLContextEGL::Init() {
#if defined(ANDROID)
  // We can't use LoadApitraceLibrary here because the GLContext
  // expects its own handle to the GL library
  if (!OpenLibrary(APITRACE_LIB))
#endif
    if (!OpenLibrary(GLES2_LIB)) {
#if defined(XP_UNIX)
      if (!OpenLibrary(GLES2_LIB2)) {
        NS_WARNING("Couldn't load GLES2 LIB.");
        return false;
      }
#endif
    }

  SetupLookupFunction();
  if (!InitWithPrefix("gl", true)) return false;

  bool current = MakeCurrent();
  if (!current) {
    gfx::LogFailure(
        NS_LITERAL_CSTRING("Couldn't get device attachments for device."));
    return false;
  }
||||||| merged common ancestors
bool
GLContextEGL::Init()
{
#if defined(ANDROID)
    // We can't use LoadApitraceLibrary here because the GLContext
    // expects its own handle to the GL library
    if (!OpenLibrary(APITRACE_LIB))
#endif
        if (!OpenLibrary(GLES2_LIB)) {
#if defined(XP_UNIX)
            if (!OpenLibrary(GLES2_LIB2)) {
                NS_WARNING("Couldn't load GLES2 LIB.");
                return false;
            }
#endif
        }

    SetupLookupFunction();
    if (!InitWithPrefix("gl", true))
        return false;

    bool current = MakeCurrent();
    if (!current) {
        gfx::LogFailure(NS_LITERAL_CSTRING(
            "Couldn't get device attachments for device."));
        return false;
    }
=======
bool GLContextEGL::Init() {
  if (!GLContext::Init()) return false;
>>>>>>> upstream-releases

<<<<<<< HEAD
  static_assert(sizeof(GLint) >= sizeof(int32_t),
                "GLint is smaller than int32_t");
  mMaxTextureImageSize = INT32_MAX;
||||||| merged common ancestors
    static_assert(sizeof(GLint) >= sizeof(int32_t), "GLint is smaller than int32_t");
    mMaxTextureImageSize = INT32_MAX;
=======
  bool current = MakeCurrent();
  if (!current) {
    gfx::LogFailure(
        NS_LITERAL_CSTRING("Couldn't get device attachments for device."));
    return false;
  }
>>>>>>> upstream-releases

  mShareWithEGLImage = mEgl->HasKHRImageBase() &&
                       mEgl->HasKHRImageTexture2D() &&
                       IsExtensionSupported(OES_EGL_image);

  return true;
}

bool GLContextEGL::BindTexImage() {
  if (!mSurface) return false;

  if (mBound && !ReleaseTexImage()) return false;

<<<<<<< HEAD
  EGLBoolean success = mEgl->fBindTexImage(EGL_DISPLAY(), (EGLSurface)mSurface,
                                           LOCAL_EGL_BACK_BUFFER);
  if (success == LOCAL_EGL_FALSE) return false;
||||||| merged common ancestors
    EGLBoolean success = mEgl->fBindTexImage(EGL_DISPLAY(),
        (EGLSurface)mSurface, LOCAL_EGL_BACK_BUFFER);
    if (success == LOCAL_EGL_FALSE)
        return false;
=======
  EGLBoolean success = mEgl->fBindTexImage(
      mEgl->Display(), (EGLSurface)mSurface, LOCAL_EGL_BACK_BUFFER);
  if (success == LOCAL_EGL_FALSE) return false;
>>>>>>> upstream-releases

  mBound = true;
  return true;
}

bool GLContextEGL::ReleaseTexImage() {
  if (!mBound) return true;

  if (!mSurface) return false;

<<<<<<< HEAD
  EGLBoolean success;
  success = mEgl->fReleaseTexImage(EGL_DISPLAY(), (EGLSurface)mSurface,
                                   LOCAL_EGL_BACK_BUFFER);
  if (success == LOCAL_EGL_FALSE) return false;
||||||| merged common ancestors
    EGLBoolean success;
    success = mEgl->fReleaseTexImage(EGL_DISPLAY(),
                                     (EGLSurface)mSurface,
                                     LOCAL_EGL_BACK_BUFFER);
    if (success == LOCAL_EGL_FALSE)
        return false;
=======
  EGLBoolean success;
  success = mEgl->fReleaseTexImage(mEgl->Display(), (EGLSurface)mSurface,
                                   LOCAL_EGL_BACK_BUFFER);
  if (success == LOCAL_EGL_FALSE) return false;
>>>>>>> upstream-releases

  mBound = false;
  return true;
}

void GLContextEGL::SetEGLSurfaceOverride(EGLSurface surf) {
  if (Screen()) {
    /* Blit `draw` to `read` if we need to, before we potentially juggle
     * `read` around. If we don't, we might attach a different `read`,
     * and *then* hit AssureBlitted, which will blit a dirty `draw` onto
     * the wrong `read`!
     */
    Screen()->AssureBlitted();
  }

  mSurfaceOverride = surf;
  DebugOnly<bool> ok = MakeCurrent(true);
  MOZ_ASSERT(ok);
}

<<<<<<< HEAD
bool GLContextEGL::MakeCurrentImpl() const {
  EGLSurface surface =
      (mSurfaceOverride != EGL_NO_SURFACE) ? mSurfaceOverride : mSurface;
  if (!surface) {
    surface = mFallbackSurface;
  }

  const bool succeeded =
      mEgl->fMakeCurrent(EGL_DISPLAY(), surface, surface, mContext);
  if (!succeeded) {
    const auto eglError = mEgl->fGetError();
    if (eglError == LOCAL_EGL_CONTEXT_LOST) {
      OnContextLostError();
    } else {
      NS_WARNING("Failed to make GL context current!");
||||||| merged common ancestors
bool
GLContextEGL::MakeCurrentImpl() const
{
    EGLSurface surface = (mSurfaceOverride != EGL_NO_SURFACE) ? mSurfaceOverride
                                                              : mSurface;
    if (!surface) {
        surface = mFallbackSurface;
    }

    const bool succeeded = mEgl->fMakeCurrent(EGL_DISPLAY(), surface, surface,
                                              mContext);
    if (!succeeded) {
        const auto eglError = mEgl->fGetError();
        if (eglError == LOCAL_EGL_CONTEXT_LOST) {
            OnContextLostError();
        } else {
            NS_WARNING("Failed to make GL context current!");
=======
bool GLContextEGL::MakeCurrentImpl() const {
  EGLSurface surface =
      (mSurfaceOverride != EGL_NO_SURFACE) ? mSurfaceOverride : mSurface;
  if (!surface) {
    surface = mFallbackSurface;
  }

  const bool succeeded =
      mEgl->fMakeCurrent(mEgl->Display(), surface, surface, mContext);
  if (!succeeded) {
    const auto eglError = mEgl->fGetError();
    if (eglError == LOCAL_EGL_CONTEXT_LOST) {
      OnContextLostError();
    } else {
      NS_WARNING("Failed to make GL context current!");
>>>>>>> upstream-releases
#ifdef DEBUG
      printf_stderr("EGL Error: 0x%04x\n", eglError);
#endif
    }
  }

  return succeeded;
}

bool GLContextEGL::IsCurrentImpl() const {
  return mEgl->fGetCurrentContext() == mContext;
}

<<<<<<< HEAD
bool GLContextEGL::RenewSurface(CompositorWidget* aWidget) {
  if (!mOwnsContext) {
    return false;
  }
  // unconditionally release the surface and create a new one. Don't try to
  // optimize this away. If we get here, then by definition we know that we want
  // to get a new surface.
  ReleaseSurface();
  MOZ_ASSERT(aWidget);

  EGLNativeWindowType nativeWindow =
      GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget);
  if (nativeWindow) {
    mSurface =
        mozilla::gl::CreateSurfaceFromNativeWindow(nativeWindow, mConfig);
    if (!mSurface) {
      NS_WARNING("Failed to create EGLSurface from native window");
      return false;
||||||| merged common ancestors
bool
GLContextEGL::RenewSurface(CompositorWidget* aWidget) {
    if (!mOwnsContext) {
        return false;
    }
    // unconditionally release the surface and create a new one. Don't try to optimize this away.
    // If we get here, then by definition we know that we want to get a new surface.
    ReleaseSurface();
    MOZ_ASSERT(aWidget);

    EGLNativeWindowType nativeWindow = GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget);
    if (nativeWindow) {
        mSurface = mozilla::gl::CreateSurfaceFromNativeWindow(nativeWindow, mConfig);
        if (!mSurface) {
            NS_WARNING("Failed to create EGLSurface from native window");
            return false;
        }
=======
bool GLContextEGL::RenewSurface(CompositorWidget* aWidget) {
  if (!mOwnsContext) {
    return false;
  }
  // unconditionally release the surface and create a new one. Don't try to
  // optimize this away. If we get here, then by definition we know that we want
  // to get a new surface.
  ReleaseSurface();
  MOZ_ASSERT(aWidget);

  EGLNativeWindowType nativeWindow =
      GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aWidget);
  if (nativeWindow) {
    mSurface =
        mozilla::gl::CreateSurfaceFromNativeWindow(mEgl, nativeWindow, mConfig);
    if (!mSurface) {
      NS_WARNING("Failed to create EGLSurface from native window");
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

  return MakeCurrent(true);
||||||| merged common ancestors

    return MakeCurrent(true);
=======
  }
  const bool ok = MakeCurrent(true);
  MOZ_ASSERT(ok);
#if defined(MOZ_WAYLAND)
  if (mSurface && !GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
    // Make eglSwapBuffers() non-blocking on wayland
    mEgl->fSwapInterval(mEgl->Display(), 0);
  }
#endif
  return ok;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void GLContextEGL::ReleaseSurface() {
  if (mOwnsContext) {
    mozilla::gl::DestroySurface(mSurface);
  }
  if (mSurface == mSurfaceOverride) {
    mSurfaceOverride = EGL_NO_SURFACE;
  }
  mSurface = EGL_NO_SURFACE;
||||||| merged common ancestors
void
GLContextEGL::ReleaseSurface() {
    if (mOwnsContext) {
        mozilla::gl::DestroySurface(mSurface);
    }
    if (mSurface == mSurfaceOverride) {
        mSurfaceOverride = EGL_NO_SURFACE;
    }
    mSurface = EGL_NO_SURFACE;
=======
void GLContextEGL::ReleaseSurface() {
  if (mOwnsContext) {
    mozilla::gl::DestroySurface(mEgl, mSurface);
  }
  if (mSurface == mSurfaceOverride) {
    mSurfaceOverride = EGL_NO_SURFACE;
  }
  mSurface = EGL_NO_SURFACE;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool GLContextEGL::SetupLookupFunction() {
  mLookupFunc = mEgl->GetLookupFunction();
  return true;
||||||| merged common ancestors
bool
GLContextEGL::SetupLookupFunction()
{
    mLookupFunc = mEgl->GetLookupFunction();
    return true;
=======
Maybe<SymbolLoader> GLContextEGL::GetSymbolLoader() const {
  return mEgl->GetSymbolLoader();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool GLContextEGL::SwapBuffers() {
  EGLSurface surface =
      mSurfaceOverride != EGL_NO_SURFACE ? mSurfaceOverride : mSurface;
  if (surface) {
    return mEgl->fSwapBuffers(EGL_DISPLAY(), surface);
  } else {
    return false;
  }
||||||| merged common ancestors
bool
GLContextEGL::SwapBuffers()
{
    EGLSurface surface = mSurfaceOverride != EGL_NO_SURFACE
                          ? mSurfaceOverride
                          : mSurface;
    if (surface) {
        return mEgl->fSwapBuffers(EGL_DISPLAY(), surface);
    } else {
        return false;
    }
=======
bool GLContextEGL::SwapBuffers() {
  EGLSurface surface =
      mSurfaceOverride != EGL_NO_SURFACE ? mSurfaceOverride : mSurface;
  if (surface) {
    return mEgl->fSwapBuffers(mEgl->Display(), surface);
  } else {
    return false;
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void GLContextEGL::GetWSIInfo(nsCString* const out) const {
  out->AppendLiteral("EGL_VENDOR: ");
  out->Append((const char*)mEgl->fQueryString(EGL_DISPLAY(), LOCAL_EGL_VENDOR));
||||||| merged common ancestors
void
GLContextEGL::GetWSIInfo(nsCString* const out) const
{
    out->AppendLiteral("EGL_VENDOR: ");
    out->Append((const char*)mEgl->fQueryString(EGL_DISPLAY(), LOCAL_EGL_VENDOR));
=======
void GLContextEGL::GetWSIInfo(nsCString* const out) const {
  out->AppendLiteral("EGL_VENDOR: ");
  out->Append(
      (const char*)mEgl->fQueryString(mEgl->Display(), LOCAL_EGL_VENDOR));
>>>>>>> upstream-releases

<<<<<<< HEAD
  out->AppendLiteral("\nEGL_VERSION: ");
  out->Append(
      (const char*)mEgl->fQueryString(EGL_DISPLAY(), LOCAL_EGL_VERSION));
||||||| merged common ancestors
    out->AppendLiteral("\nEGL_VERSION: ");
    out->Append((const char*)mEgl->fQueryString(EGL_DISPLAY(), LOCAL_EGL_VERSION));
=======
  out->AppendLiteral("\nEGL_VERSION: ");
  out->Append(
      (const char*)mEgl->fQueryString(mEgl->Display(), LOCAL_EGL_VERSION));
>>>>>>> upstream-releases

<<<<<<< HEAD
  out->AppendLiteral("\nEGL_EXTENSIONS: ");
  out->Append(
      (const char*)mEgl->fQueryString(EGL_DISPLAY(), LOCAL_EGL_EXTENSIONS));
||||||| merged common ancestors
    out->AppendLiteral("\nEGL_EXTENSIONS: ");
    out->Append((const char*)mEgl->fQueryString(EGL_DISPLAY(), LOCAL_EGL_EXTENSIONS));
=======
  out->AppendLiteral("\nEGL_EXTENSIONS: ");
  out->Append(
      (const char*)mEgl->fQueryString(mEgl->Display(), LOCAL_EGL_EXTENSIONS));
>>>>>>> upstream-releases

#ifndef ANDROID  // This query will crash some old android.
  out->AppendLiteral("\nEGL_EXTENSIONS(nullptr): ");
  out->Append((const char*)mEgl->fQueryString(nullptr, LOCAL_EGL_EXTENSIONS));
#endif
}

// hold a reference to the given surface
// for the lifetime of this context.
void GLContextEGL::HoldSurface(gfxASurface* aSurf) { mThebesSurface = aSurf; }

#define LOCAL_EGL_CONTEXT_PROVOKING_VERTEX_DONT_CARE_MOZ 0x6000

<<<<<<< HEAD
already_AddRefed<GLContextEGL> GLContextEGL::CreateGLContext(
    CreateContextFlags flags, const SurfaceCaps& caps, bool isOffscreen,
    EGLConfig config, EGLSurface surface, nsACString* const out_failureId) {
  auto* egl = gl::GLLibraryEGL::Get();
||||||| merged common ancestors
already_AddRefed<GLContextEGL>
GLContextEGL::CreateGLContext(CreateContextFlags flags,
                const SurfaceCaps& caps,
                bool isOffscreen,
                EGLConfig config,
                EGLSurface surface,
                nsACString* const out_failureId)
{
    auto* egl = gl::GLLibraryEGL::Get();

    if (egl->fBindAPI(LOCAL_EGL_OPENGL_ES_API) == LOCAL_EGL_FALSE) {
        *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_ES");
        NS_WARNING("Failed to bind API to GLES!");
        return nullptr;
    }
=======
already_AddRefed<GLContextEGL> GLContextEGL::CreateGLContext(
    GLLibraryEGL* const egl, CreateContextFlags flags, const SurfaceCaps& caps,
    bool isOffscreen, EGLConfig config, EGLSurface surface,
    nsACString* const out_failureId) {
  if (egl->fBindAPI(LOCAL_EGL_OPENGL_ES_API) == LOCAL_EGL_FALSE) {
    *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_ES");
    NS_WARNING("Failed to bind API to GLES!");
    return nullptr;
  }

  std::vector<EGLint> required_attribs;
  required_attribs.push_back(LOCAL_EGL_CONTEXT_CLIENT_VERSION);
  if (flags & CreateContextFlags::PREFER_ES3) {
    required_attribs.push_back(3);
  } else {
    required_attribs.push_back(2);
  }

  const auto debugFlags = GLContext::ChooseDebugFlags(flags);
  if (!debugFlags && flags & CreateContextFlags::NO_VALIDATION &&
      egl->IsExtensionSupported(GLLibraryEGL::KHR_create_context_no_error)) {
    required_attribs.push_back(LOCAL_EGL_CONTEXT_OPENGL_NO_ERROR_KHR);
    required_attribs.push_back(LOCAL_EGL_TRUE);
  }

  if (flags & CreateContextFlags::PROVOKING_VERTEX_DONT_CARE &&
      egl->IsExtensionSupported(
          GLLibraryEGL::MOZ_create_context_provoking_vertex_dont_care)) {
    required_attribs.push_back(
        LOCAL_EGL_CONTEXT_PROVOKING_VERTEX_DONT_CARE_MOZ);
    required_attribs.push_back(LOCAL_EGL_TRUE);
  }

  std::vector<EGLint> robustness_attribs;
  std::vector<EGLint> rbab_attribs;  // RBAB: Robust Buffer Access Behavior
  if (flags & CreateContextFlags::PREFER_ROBUSTNESS) {
    if (egl->IsExtensionSupported(
            GLLibraryEGL::EXT_create_context_robustness)) {
      robustness_attribs = required_attribs;
      robustness_attribs.push_back(
          LOCAL_EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_EXT);
      robustness_attribs.push_back(LOCAL_EGL_LOSE_CONTEXT_ON_RESET_EXT);

      // Don't enable robust buffer access on Adreno 630 devices.
      // It causes the linking of some shaders to fail. See bug 1485441.
      nsCOMPtr<nsIGfxInfo> gfxInfo = services::GetGfxInfo();
      nsAutoString renderer;
      gfxInfo->GetAdapterDeviceID(renderer);
      if (renderer.Find("Adreno (TM) 630") == -1) {
        rbab_attribs = robustness_attribs;
        rbab_attribs.push_back(LOCAL_EGL_CONTEXT_OPENGL_ROBUST_ACCESS_EXT);
        rbab_attribs.push_back(LOCAL_EGL_TRUE);
      }
    } else if (egl->IsExtensionSupported(GLLibraryEGL::KHR_create_context)) {
      robustness_attribs = required_attribs;
      robustness_attribs.push_back(
          LOCAL_EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR);
      robustness_attribs.push_back(LOCAL_EGL_LOSE_CONTEXT_ON_RESET_KHR);

      rbab_attribs = robustness_attribs;
      rbab_attribs.push_back(LOCAL_EGL_CONTEXT_FLAGS_KHR);
      rbab_attribs.push_back(LOCAL_EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT_KHR);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (egl->fBindAPI(LOCAL_EGL_OPENGL_ES_API) == LOCAL_EGL_FALSE) {
    *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_ES");
    NS_WARNING("Failed to bind API to GLES!");
    return nullptr;
  }

  std::vector<EGLint> required_attribs;
  required_attribs.push_back(LOCAL_EGL_CONTEXT_CLIENT_VERSION);
  if (flags & CreateContextFlags::PREFER_ES3) {
    required_attribs.push_back(3);
  } else {
    required_attribs.push_back(2);
  }

  const auto debugFlags = GLContext::ChooseDebugFlags(flags);
  if (!debugFlags && flags & CreateContextFlags::NO_VALIDATION &&
      egl->IsExtensionSupported(GLLibraryEGL::KHR_create_context_no_error)) {
    required_attribs.push_back(LOCAL_EGL_CONTEXT_OPENGL_NO_ERROR_KHR);
    required_attribs.push_back(LOCAL_EGL_TRUE);
  }

  if (flags & CreateContextFlags::PROVOKING_VERTEX_DONT_CARE &&
      egl->IsExtensionSupported(
          GLLibraryEGL::MOZ_create_context_provoking_vertex_dont_care)) {
    required_attribs.push_back(
        LOCAL_EGL_CONTEXT_PROVOKING_VERTEX_DONT_CARE_MOZ);
    required_attribs.push_back(LOCAL_EGL_TRUE);
  }

  std::vector<EGLint> robustness_attribs;
  std::vector<EGLint> rbab_attribs;  // RBAB: Robust Buffer Access Behavior
  if (flags & CreateContextFlags::PREFER_ROBUSTNESS) {
    if (egl->IsExtensionSupported(
            GLLibraryEGL::EXT_create_context_robustness)) {
      robustness_attribs = required_attribs;
      robustness_attribs.push_back(
          LOCAL_EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_EXT);
      robustness_attribs.push_back(LOCAL_EGL_LOSE_CONTEXT_ON_RESET_EXT);

      // Don't enable robust buffer access on Adreno 630 devices.
      // It causes the linking of some shaders to fail. See bug 1485441.
      nsCOMPtr<nsIGfxInfo> gfxInfo = services::GetGfxInfo();
      nsAutoString renderer;
      gfxInfo->GetAdapterDeviceID(renderer);
      if (renderer.Find("Adreno (TM) 630") == -1) {
        rbab_attribs = robustness_attribs;
        rbab_attribs.push_back(LOCAL_EGL_CONTEXT_OPENGL_ROBUST_ACCESS_EXT);
        rbab_attribs.push_back(LOCAL_EGL_TRUE);
      }
    } else if (egl->IsExtensionSupported(GLLibraryEGL::KHR_create_context)) {
      robustness_attribs = required_attribs;
      robustness_attribs.push_back(
          LOCAL_EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR);
      robustness_attribs.push_back(LOCAL_EGL_LOSE_CONTEXT_ON_RESET_KHR);

      rbab_attribs = robustness_attribs;
      rbab_attribs.push_back(LOCAL_EGL_CONTEXT_FLAGS_KHR);
      rbab_attribs.push_back(LOCAL_EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT_KHR);
    }
  }
||||||| merged common ancestors
    std::vector<EGLint> required_attribs;
    required_attribs.push_back(LOCAL_EGL_CONTEXT_CLIENT_VERSION);
    if (flags & CreateContextFlags::PREFER_ES3) {
        required_attribs.push_back(3);
    } else {
        required_attribs.push_back(2);
    }
=======
  const auto fnCreate = [&](const std::vector<EGLint>& attribs) {
    auto terminated_attribs = attribs;
>>>>>>> upstream-releases

<<<<<<< HEAD
  const auto fnCreate = [&](const std::vector<EGLint>& attribs) {
    auto terminated_attribs = attribs;
||||||| merged common ancestors
    const auto debugFlags = GLContext::ChooseDebugFlags(flags);
    if (!debugFlags &&
        flags & CreateContextFlags::NO_VALIDATION &&
        egl->IsExtensionSupported(GLLibraryEGL::KHR_create_context_no_error))
    {
        required_attribs.push_back(LOCAL_EGL_CONTEXT_OPENGL_NO_ERROR_KHR);
        required_attribs.push_back(LOCAL_EGL_TRUE);
    }
=======
    for (const auto& cur : kTerminationAttribs) {
      terminated_attribs.push_back(cur);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    for (const auto& cur : kTerminationAttribs) {
      terminated_attribs.push_back(cur);
    }
||||||| merged common ancestors
    if (flags & CreateContextFlags::PROVOKING_VERTEX_DONT_CARE &&
        egl->IsExtensionSupported(GLLibraryEGL::MOZ_create_context_provoking_vertex_dont_care))
    {
        required_attribs.push_back(LOCAL_EGL_CONTEXT_PROVOKING_VERTEX_DONT_CARE_MOZ);
        required_attribs.push_back(LOCAL_EGL_TRUE);
    }
=======
    return egl->fCreateContext(egl->Display(), config, EGL_NO_CONTEXT,
                               terminated_attribs.data());
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
    return egl->fCreateContext(EGL_DISPLAY(), config, EGL_NO_CONTEXT,
                               terminated_attribs.data());
  };
||||||| merged common ancestors
    std::vector<EGLint> robustness_attribs;
    std::vector<EGLint> rbab_attribs; // RBAB: Robust Buffer Access Behavior
    if (flags & CreateContextFlags::PREFER_ROBUSTNESS) {
        if (egl->IsExtensionSupported(GLLibraryEGL::EXT_create_context_robustness)) {
            robustness_attribs = required_attribs;
            robustness_attribs.push_back(LOCAL_EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_EXT);
            robustness_attribs.push_back(LOCAL_EGL_LOSE_CONTEXT_ON_RESET_EXT);

            // Don't enable robust buffer access on Adreno 630 devices.
            // It causes the linking of some shaders to fail. See bug 1485441.
            nsCOMPtr<nsIGfxInfo> gfxInfo = services::GetGfxInfo();
            nsAutoString renderer;
            gfxInfo->GetAdapterDeviceID(renderer);
            if (renderer.Find("Adreno (TM) 630") == -1) {
                rbab_attribs = robustness_attribs;
                rbab_attribs.push_back(LOCAL_EGL_CONTEXT_OPENGL_ROBUST_ACCESS_EXT);
                rbab_attribs.push_back(LOCAL_EGL_TRUE);
            }
        } else if (egl->IsExtensionSupported(GLLibraryEGL::KHR_create_context)) {
            robustness_attribs = required_attribs;
            robustness_attribs.push_back(LOCAL_EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR);
            robustness_attribs.push_back(LOCAL_EGL_LOSE_CONTEXT_ON_RESET_KHR);

            rbab_attribs = robustness_attribs;
            rbab_attribs.push_back(LOCAL_EGL_CONTEXT_FLAGS_KHR);
            rbab_attribs.push_back(LOCAL_EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT_KHR);
        }
    }
=======
  EGLContext context;
  do {
    if (!rbab_attribs.empty()) {
      context = fnCreate(rbab_attribs);
      if (context) break;
      NS_WARNING("Failed to create EGLContext with rbab_attribs");
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  EGLContext context;
  do {
    if (!rbab_attribs.empty()) {
      context = fnCreate(rbab_attribs);
      if (context) break;
      NS_WARNING("Failed to create EGLContext with rbab_attribs");
    }

    if (!robustness_attribs.empty()) {
      context = fnCreate(robustness_attribs);
      if (context) break;
      NS_WARNING("Failed to create EGLContext with robustness_attribs");
    }
||||||| merged common ancestors
    const auto fnCreate = [&](const std::vector<EGLint>& attribs) {
        auto terminated_attribs = attribs;

        for (const auto& cur : kTerminationAttribs) {
            terminated_attribs.push_back(cur);
        }
=======
    if (!robustness_attribs.empty()) {
      context = fnCreate(robustness_attribs);
      if (context) break;
      NS_WARNING("Failed to create EGLContext with robustness_attribs");
    }
>>>>>>> upstream-releases

    context = fnCreate(required_attribs);
    if (context) break;
    NS_WARNING("Failed to create EGLContext with required_attribs");

    *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_CREATE");
    return nullptr;
  } while (false);
  MOZ_ASSERT(context);

<<<<<<< HEAD
  RefPtr<GLContextEGL> glContext =
      new GLContextEGL(flags, caps, isOffscreen, config, surface, context);
  if (!glContext->Init()) {
    *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_INIT");
    return nullptr;
  }
||||||| merged common ancestors
        context = fnCreate(required_attribs);
        if (context)
            break;
        NS_WARNING("Failed to create EGLContext with required_attribs");

        *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_CREATE");
        return nullptr;
    } while (false);
    MOZ_ASSERT(context);

    RefPtr<GLContextEGL> glContext = new GLContextEGL(flags, caps, isOffscreen, config,
                                                      surface, context);
    if (!glContext->Init()) {
        *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_INIT");
        return nullptr;
    }
=======
  RefPtr<GLContextEGL> glContext =
      new GLContextEGL(egl, flags, caps, isOffscreen, config, surface, context);
  if (!glContext->Init()) {
    *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_INIT");
    return nullptr;
  }
>>>>>>> upstream-releases

  return glContext.forget();
}

<<<<<<< HEAD
EGLSurface GLContextEGL::CreatePBufferSurfaceTryingPowerOfTwo(
    EGLConfig config, EGLenum bindToTextureFormat,
    mozilla::gfx::IntSize& pbsize) {
  nsTArray<EGLint> pbattrs(16);
  EGLSurface surface = nullptr;
||||||| merged common ancestors
EGLSurface
GLContextEGL::CreatePBufferSurfaceTryingPowerOfTwo(EGLConfig config,
                                                   EGLenum bindToTextureFormat,
                                                   mozilla::gfx::IntSize& pbsize)
{
    nsTArray<EGLint> pbattrs(16);
    EGLSurface surface = nullptr;
=======
// static
EGLSurface GLContextEGL::CreatePBufferSurfaceTryingPowerOfTwo(
    GLLibraryEGL* const egl, EGLConfig config, EGLenum bindToTextureFormat,
    mozilla::gfx::IntSize& pbsize) {
  nsTArray<EGLint> pbattrs(16);
  EGLSurface surface = nullptr;
>>>>>>> upstream-releases

TRY_AGAIN_POWER_OF_TWO:
<<<<<<< HEAD
  pbattrs.Clear();
  pbattrs.AppendElement(LOCAL_EGL_WIDTH);
  pbattrs.AppendElement(pbsize.width);
  pbattrs.AppendElement(LOCAL_EGL_HEIGHT);
  pbattrs.AppendElement(pbsize.height);

  if (bindToTextureFormat != LOCAL_EGL_NONE) {
    pbattrs.AppendElement(LOCAL_EGL_TEXTURE_TARGET);
    pbattrs.AppendElement(LOCAL_EGL_TEXTURE_2D);

    pbattrs.AppendElement(LOCAL_EGL_TEXTURE_FORMAT);
    pbattrs.AppendElement(bindToTextureFormat);
  }

  for (const auto& cur : kTerminationAttribs) {
    pbattrs.AppendElement(cur);
  }

  auto* egl = gl::GLLibraryEGL::Get();

  surface = egl->fCreatePbufferSurface(EGL_DISPLAY(), config, &pbattrs[0]);
  if (!surface) {
    if (!is_power_of_two(pbsize.width) || !is_power_of_two(pbsize.height)) {
      if (!is_power_of_two(pbsize.width))
        pbsize.width = next_power_of_two(pbsize.width);
      if (!is_power_of_two(pbsize.height))
        pbsize.height = next_power_of_two(pbsize.height);

      NS_WARNING("Failed to create pbuffer, trying power of two dims");
      goto TRY_AGAIN_POWER_OF_TWO;
||||||| merged common ancestors
    pbattrs.Clear();
    pbattrs.AppendElement(LOCAL_EGL_WIDTH); pbattrs.AppendElement(pbsize.width);
    pbattrs.AppendElement(LOCAL_EGL_HEIGHT); pbattrs.AppendElement(pbsize.height);

    if (bindToTextureFormat != LOCAL_EGL_NONE) {
        pbattrs.AppendElement(LOCAL_EGL_TEXTURE_TARGET);
        pbattrs.AppendElement(LOCAL_EGL_TEXTURE_2D);

        pbattrs.AppendElement(LOCAL_EGL_TEXTURE_FORMAT);
        pbattrs.AppendElement(bindToTextureFormat);
=======
  pbattrs.Clear();
  pbattrs.AppendElement(LOCAL_EGL_WIDTH);
  pbattrs.AppendElement(pbsize.width);
  pbattrs.AppendElement(LOCAL_EGL_HEIGHT);
  pbattrs.AppendElement(pbsize.height);

  if (bindToTextureFormat != LOCAL_EGL_NONE) {
    pbattrs.AppendElement(LOCAL_EGL_TEXTURE_TARGET);
    pbattrs.AppendElement(LOCAL_EGL_TEXTURE_2D);

    pbattrs.AppendElement(LOCAL_EGL_TEXTURE_FORMAT);
    pbattrs.AppendElement(bindToTextureFormat);
  }

  for (const auto& cur : kTerminationAttribs) {
    pbattrs.AppendElement(cur);
  }

  surface = egl->fCreatePbufferSurface(egl->Display(), config, &pbattrs[0]);
  if (!surface) {
    if (!is_power_of_two(pbsize.width) || !is_power_of_two(pbsize.height)) {
      if (!is_power_of_two(pbsize.width))
        pbsize.width = next_power_of_two(pbsize.width);
      if (!is_power_of_two(pbsize.height))
        pbsize.height = next_power_of_two(pbsize.height);

      NS_WARNING("Failed to create pbuffer, trying power of two dims");
      goto TRY_AGAIN_POWER_OF_TWO;
>>>>>>> upstream-releases
    }

    NS_WARNING("Failed to create pbuffer surface");
    return nullptr;
  }

  return surface;
}

#if defined(MOZ_WAYLAND)
WaylandGLSurface::WaylandGLSurface(struct wl_surface* aWaylandSurface,
                                   struct wl_egl_window* aEGLWindow)
    : mWaylandSurface(aWaylandSurface), mEGLWindow(aEGLWindow) {}

WaylandGLSurface::~WaylandGLSurface() {
  wl_egl_window_destroy(mEGLWindow);
  wl_surface_destroy(mWaylandSurface);
}

<<<<<<< HEAD
EGLSurface GLContextEGL::CreateWaylandBufferSurface(
    EGLConfig config, mozilla::gfx::IntSize& pbsize) {
  // Available as of GTK 3.8+
  static auto sGdkWaylandDisplayGetWlCompositor =
      (wl_compositor * (*)(GdkDisplay*))
          dlsym(RTLD_DEFAULT, "gdk_wayland_display_get_wl_compositor");
||||||| merged common ancestors
        NS_WARNING("Failed to create pbuffer surface");
        return nullptr;
    }
=======
// static
EGLSurface GLContextEGL::CreateWaylandBufferSurface(
    GLLibraryEGL* const egl, EGLConfig config, mozilla::gfx::IntSize& pbsize) {
  // Available as of GTK 3.8+
  static auto sGdkWaylandDisplayGetWlCompositor =
      (wl_compositor * (*)(GdkDisplay*))
          dlsym(RTLD_DEFAULT, "gdk_wayland_display_get_wl_compositor");
>>>>>>> upstream-releases

  if (!sGdkWaylandDisplayGetWlCompositor) return nullptr;

  struct wl_compositor* compositor =
      sGdkWaylandDisplayGetWlCompositor(gdk_display_get_default());
  struct wl_surface* wlsurface = wl_compositor_create_surface(compositor);
  struct wl_egl_window* eglwindow =
      wl_egl_window_create(wlsurface, pbsize.width, pbsize.height);

<<<<<<< HEAD
  auto* egl = gl::GLLibraryEGL::Get();
  EGLSurface surface =
      egl->fCreateWindowSurface(EGL_DISPLAY(), config, eglwindow, 0);
||||||| merged common ancestors
WaylandGLSurface::~WaylandGLSurface()
{
    wl_egl_window_destroy(mEGLWindow);
    wl_surface_destroy(mWaylandSurface);
}
=======
  EGLSurface surface =
      egl->fCreateWindowSurface(egl->Display(), config, eglwindow, 0);
>>>>>>> upstream-releases

  if (surface) {
    WaylandGLSurface* waylandData = new WaylandGLSurface(wlsurface, eglwindow);
    auto entry = sWaylandGLSurface.LookupForAdd(surface);
    entry.OrInsert([&waylandData]() { return waylandData; });
  }

  return surface;
}
#endif

static const EGLint kEGLConfigAttribsOffscreenPBuffer[] = {
    LOCAL_EGL_SURFACE_TYPE, LOCAL_EGL_PBUFFER_BIT, LOCAL_EGL_RENDERABLE_TYPE,
    LOCAL_EGL_OPENGL_ES2_BIT,
    // Old versions of llvmpipe seem to need this to properly create the pbuffer
    // (bug 981856)
    LOCAL_EGL_RED_SIZE, 8, LOCAL_EGL_GREEN_SIZE, 8, LOCAL_EGL_BLUE_SIZE, 8,
    LOCAL_EGL_ALPHA_SIZE, 0,
    EGL_ATTRIBS_LIST_SAFE_TERMINATION_WORKING_AROUND_BUGS};

static const EGLint kEGLConfigAttribsRGB16[] = {
    LOCAL_EGL_SURFACE_TYPE,
    LOCAL_EGL_WINDOW_BIT,
    LOCAL_EGL_RENDERABLE_TYPE,
    LOCAL_EGL_OPENGL_ES2_BIT,
    LOCAL_EGL_RED_SIZE,
    5,
    LOCAL_EGL_GREEN_SIZE,
    6,
    LOCAL_EGL_BLUE_SIZE,
    5,
    LOCAL_EGL_ALPHA_SIZE,
    0,
    EGL_ATTRIBS_LIST_SAFE_TERMINATION_WORKING_AROUND_BUGS};

static const EGLint kEGLConfigAttribsRGB24[] = {
    LOCAL_EGL_SURFACE_TYPE,
    LOCAL_EGL_WINDOW_BIT,
    LOCAL_EGL_RENDERABLE_TYPE,
    LOCAL_EGL_OPENGL_ES2_BIT,
    LOCAL_EGL_RED_SIZE,
    8,
    LOCAL_EGL_GREEN_SIZE,
    8,
    LOCAL_EGL_BLUE_SIZE,
    8,
    LOCAL_EGL_ALPHA_SIZE,
    0,
    EGL_ATTRIBS_LIST_SAFE_TERMINATION_WORKING_AROUND_BUGS};

static const EGLint kEGLConfigAttribsRGBA32[] = {
<<<<<<< HEAD
    LOCAL_EGL_SURFACE_TYPE,
    LOCAL_EGL_WINDOW_BIT,
    LOCAL_EGL_RENDERABLE_TYPE,
    LOCAL_EGL_OPENGL_ES2_BIT,
    LOCAL_EGL_RED_SIZE,
    8,
    LOCAL_EGL_GREEN_SIZE,
    8,
    LOCAL_EGL_BLUE_SIZE,
    8,
    LOCAL_EGL_ALPHA_SIZE,
    8,
    EGL_ATTRIBS_LIST_SAFE_TERMINATION_WORKING_AROUND_BUGS};

bool CreateConfig(EGLConfig* aConfig, int32_t depth, bool aEnableDepthBuffer) {
  EGLConfig configs[64];
  const EGLint* attribs;
  EGLint ncfg = ArrayLength(configs);

  switch (depth) {
    case 16:
      attribs = kEGLConfigAttribsRGB16;
      break;
    case 24:
      attribs = kEGLConfigAttribsRGB24;
      break;
    case 32:
      attribs = kEGLConfigAttribsRGBA32;
      break;
    default:
      NS_ERROR("Unknown pixel depth");
      return false;
  }

  auto* egl = gl::GLLibraryEGL::Get();

  if (!egl->fChooseConfig(EGL_DISPLAY(), attribs, configs, ncfg, &ncfg) ||
      ncfg < 1) {
    return false;
  }

  for (int j = 0; j < ncfg; ++j) {
    EGLConfig config = configs[j];
    EGLint r, g, b, a;
    if (egl->fGetConfigAttrib(EGL_DISPLAY(), config, LOCAL_EGL_RED_SIZE, &r) &&
        egl->fGetConfigAttrib(EGL_DISPLAY(), config, LOCAL_EGL_GREEN_SIZE,
                              &g) &&
        egl->fGetConfigAttrib(EGL_DISPLAY(), config, LOCAL_EGL_BLUE_SIZE, &b) &&
        egl->fGetConfigAttrib(EGL_DISPLAY(), config, LOCAL_EGL_ALPHA_SIZE,
                              &a) &&
        ((depth == 16 && r == 5 && g == 6 && b == 5) ||
         (depth == 24 && r == 8 && g == 8 && b == 8) ||
         (depth == 32 && r == 8 && g == 8 && b == 8 && a == 8))) {
      EGLint z;
      if (aEnableDepthBuffer) {
        if (!egl->fGetConfigAttrib(EGL_DISPLAY(), config, LOCAL_EGL_DEPTH_SIZE,
                                   &z) ||
            z != 24) {
          continue;
||||||| merged common ancestors
    LOCAL_EGL_SURFACE_TYPE,    LOCAL_EGL_WINDOW_BIT,
    LOCAL_EGL_RENDERABLE_TYPE, LOCAL_EGL_OPENGL_ES2_BIT,
    LOCAL_EGL_RED_SIZE,        8,
    LOCAL_EGL_GREEN_SIZE,      8,
    LOCAL_EGL_BLUE_SIZE,       8,
    LOCAL_EGL_ALPHA_SIZE,      8,
    EGL_ATTRIBS_LIST_SAFE_TERMINATION_WORKING_AROUND_BUGS
};

bool
CreateConfig(EGLConfig* aConfig, int32_t depth, bool aEnableDepthBuffer)
{
    EGLConfig configs[64];
    const EGLint* attribs;
    EGLint ncfg = ArrayLength(configs);

    switch (depth) {
        case 16:
            attribs = kEGLConfigAttribsRGB16;
            break;
        case 24:
            attribs = kEGLConfigAttribsRGB24;
            break;
        case 32:
            attribs = kEGLConfigAttribsRGBA32;
            break;
        default:
            NS_ERROR("Unknown pixel depth");
            return false;
    }

    auto* egl = gl::GLLibraryEGL::Get();

    if (!egl->fChooseConfig(EGL_DISPLAY(), attribs,
                            configs, ncfg, &ncfg) ||
        ncfg < 1) {
        return false;
    }

    for (int j = 0; j < ncfg; ++j) {
        EGLConfig config = configs[j];
        EGLint r, g, b, a;
        if (egl->fGetConfigAttrib(EGL_DISPLAY(), config,
                                  LOCAL_EGL_RED_SIZE, &r) &&
            egl->fGetConfigAttrib(EGL_DISPLAY(), config,
                                  LOCAL_EGL_GREEN_SIZE, &g) &&
            egl->fGetConfigAttrib(EGL_DISPLAY(), config,
                                  LOCAL_EGL_BLUE_SIZE, &b) &&
            egl->fGetConfigAttrib(EGL_DISPLAY(), config,
                                  LOCAL_EGL_ALPHA_SIZE, &a) &&
            ((depth == 16 && r == 5 && g == 6 && b == 5) ||
             (depth == 24 && r == 8 && g == 8 && b == 8) ||
             (depth == 32 && r == 8 && g == 8 && b == 8 && a == 8)))
        {
            EGLint z;
            if (aEnableDepthBuffer) {
                if (!egl->fGetConfigAttrib(EGL_DISPLAY(), config, LOCAL_EGL_DEPTH_SIZE, &z) ||
                    z != 24) {
                    continue;
                }
            }
            *aConfig = config;
            return true;
=======
    LOCAL_EGL_SURFACE_TYPE,
    LOCAL_EGL_WINDOW_BIT,
    LOCAL_EGL_RENDERABLE_TYPE,
    LOCAL_EGL_OPENGL_ES2_BIT,
    LOCAL_EGL_RED_SIZE,
    8,
    LOCAL_EGL_GREEN_SIZE,
    8,
    LOCAL_EGL_BLUE_SIZE,
    8,
    LOCAL_EGL_ALPHA_SIZE,
    8,
    EGL_ATTRIBS_LIST_SAFE_TERMINATION_WORKING_AROUND_BUGS};

bool CreateConfig(GLLibraryEGL* const egl, EGLConfig* aConfig, int32_t depth,
                  bool aEnableDepthBuffer) {
  EGLConfig configs[64];
  const EGLint* attribs;
  EGLint ncfg = ArrayLength(configs);

  switch (depth) {
    case 16:
      attribs = kEGLConfigAttribsRGB16;
      break;
    case 24:
      attribs = kEGLConfigAttribsRGB24;
      break;
    case 32:
      attribs = kEGLConfigAttribsRGBA32;
      break;
    default:
      NS_ERROR("Unknown pixel depth");
      return false;
  }

  if (!egl->fChooseConfig(egl->Display(), attribs, configs, ncfg, &ncfg) ||
      ncfg < 1) {
    return false;
  }

  for (int j = 0; j < ncfg; ++j) {
    EGLConfig config = configs[j];
    EGLint r, g, b, a;
    if (egl->fGetConfigAttrib(egl->Display(), config, LOCAL_EGL_RED_SIZE, &r) &&
        egl->fGetConfigAttrib(egl->Display(), config, LOCAL_EGL_GREEN_SIZE,
                              &g) &&
        egl->fGetConfigAttrib(egl->Display(), config, LOCAL_EGL_BLUE_SIZE,
                              &b) &&
        egl->fGetConfigAttrib(egl->Display(), config, LOCAL_EGL_ALPHA_SIZE,
                              &a) &&
        ((depth == 16 && r == 5 && g == 6 && b == 5) ||
         (depth == 24 && r == 8 && g == 8 && b == 8) ||
         (depth == 32 && r == 8 && g == 8 && b == 8 && a == 8))) {
      EGLint z;
      if (aEnableDepthBuffer) {
        if (!egl->fGetConfigAttrib(egl->Display(), config, LOCAL_EGL_DEPTH_SIZE,
                                   &z) ||
            z != 24) {
          continue;
>>>>>>> upstream-releases
        }
      }
      *aConfig = config;
      return true;
    }
  }
  return false;
}

// Return true if a suitable EGLConfig was found and pass it out
// through aConfig.  Return false otherwise.
//
// NB: It's entirely legal for the returned EGLConfig to be valid yet
// have the value null.
<<<<<<< HEAD
static bool CreateConfig(EGLConfig* aConfig, bool aEnableDepthBuffer) {
  int32_t depth = gfxVars::ScreenDepth();
  if (!CreateConfig(aConfig, depth, aEnableDepthBuffer)) {
||||||| merged common ancestors
static bool
CreateConfig(EGLConfig* aConfig, bool aEnableDepthBuffer)
{
    int32_t depth = gfxVars::ScreenDepth();
    if (!CreateConfig(aConfig, depth, aEnableDepthBuffer)) {
=======
static bool CreateConfig(GLLibraryEGL* const egl, EGLConfig* const aConfig,
                         const bool aEnableDepthBuffer) {
  int32_t depth = gfxVars::ScreenDepth();
  if (!CreateConfig(egl, aConfig, depth, aEnableDepthBuffer)) {
>>>>>>> upstream-releases
#ifdef MOZ_WIDGET_ANDROID
<<<<<<< HEAD
    // Bug 736005
    // Android doesn't always support 16 bit so also try 24 bit
    if (depth == 16) {
      return CreateConfig(aConfig, 24, aEnableDepthBuffer);
    }
    // Bug 970096
    // Some devices that have 24 bit screens only support 16 bit OpenGL?
    if (depth == 24) {
      return CreateConfig(aConfig, 16, aEnableDepthBuffer);
||||||| merged common ancestors
        // Bug 736005
        // Android doesn't always support 16 bit so also try 24 bit
        if (depth == 16) {
            return CreateConfig(aConfig, 24, aEnableDepthBuffer);
        }
        // Bug 970096
        // Some devices that have 24 bit screens only support 16 bit OpenGL?
        if (depth == 24) {
            return CreateConfig(aConfig, 16, aEnableDepthBuffer);
        }
#endif
        return false;
    } else {
        return true;
=======
    // Bug 736005
    // Android doesn't always support 16 bit so also try 24 bit
    if (depth == 16) {
      return CreateConfig(egl, aConfig, 24, aEnableDepthBuffer);
    }
    // Bug 970096
    // Some devices that have 24 bit screens only support 16 bit OpenGL?
    if (depth == 24) {
      return CreateConfig(egl, aConfig, 16, aEnableDepthBuffer);
>>>>>>> upstream-releases
    }
#endif
    return false;
  } else {
    return true;
  }
}

already_AddRefed<GLContext> GLContextProviderEGL::CreateWrappingExisting(
    void* aContext, void* aSurface) {
  nsCString discardFailureId;
  if (!GLLibraryEGL::EnsureInitialized(false, &discardFailureId)) {
    MOZ_CRASH("GFX: Failed to load EGL library 2!");
    return nullptr;
  }

  if (!aContext || !aSurface) return nullptr;

<<<<<<< HEAD
  SurfaceCaps caps = SurfaceCaps::Any();
  EGLConfig config = EGL_NO_CONFIG;
  RefPtr<GLContextEGL> gl =
      new GLContextEGL(CreateContextFlags::NONE, caps, false, config,
                       (EGLSurface)aSurface, (EGLContext)aContext);
  gl->SetIsDoubleBuffered(true);
  gl->mOwnsContext = false;
||||||| merged common ancestors
    SurfaceCaps caps = SurfaceCaps::Any();
    EGLConfig config = EGL_NO_CONFIG;
    RefPtr<GLContextEGL> gl = new GLContextEGL(CreateContextFlags::NONE, caps, false,
                                               config, (EGLSurface)aSurface,
                                               (EGLContext)aContext);
    gl->SetIsDoubleBuffered(true);
    gl->mOwnsContext = false;
=======
  const auto& egl = GLLibraryEGL::Get();
  SurfaceCaps caps = SurfaceCaps::Any();
  EGLConfig config = EGL_NO_CONFIG;
  RefPtr<GLContextEGL> gl =
      new GLContextEGL(egl, CreateContextFlags::NONE, caps, false, config,
                       (EGLSurface)aSurface, (EGLContext)aContext);
  gl->SetIsDoubleBuffered(true);
  gl->mOwnsContext = false;
>>>>>>> upstream-releases

  return gl.forget();
}

<<<<<<< HEAD
already_AddRefed<GLContext> GLContextProviderEGL::CreateForCompositorWidget(
    CompositorWidget* aCompositorWidget, bool aForceAccelerated) {
  MOZ_ASSERT(aCompositorWidget);
  return GLContextEGLFactory::Create(
      GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aCompositorWidget),
      aCompositorWidget->GetCompositorOptions().UseWebRender());
||||||| merged common ancestors
already_AddRefed<GLContext>
GLContextProviderEGL::CreateForCompositorWidget(CompositorWidget* aCompositorWidget, bool aForceAccelerated)
{
    MOZ_ASSERT(aCompositorWidget);
    return GLContextEGLFactory::Create(GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aCompositorWidget),
                                       aCompositorWidget->GetCompositorOptions().UseWebRender());
=======
already_AddRefed<GLContext> GLContextProviderEGL::CreateForCompositorWidget(
    CompositorWidget* aCompositorWidget, bool aWebRender,
    bool aForceAccelerated) {
  EGLNativeWindowType window = nullptr;
  if (aCompositorWidget) {
    window = GET_NATIVE_WINDOW_FROM_COMPOSITOR_WIDGET(aCompositorWidget);
  }
  return GLContextEGLFactory::Create(window, aWebRender);
>>>>>>> upstream-releases
}

already_AddRefed<GLContext> GLContextProviderEGL::CreateForWindow(
    nsIWidget* aWidget, bool aWebRender, bool aForceAccelerated) {
  MOZ_ASSERT(aWidget);
  return GLContextEGLFactory::Create(
      GET_NATIVE_WINDOW_FROM_REAL_WIDGET(aWidget), aWebRender);
}

#if defined(MOZ_WIDGET_ANDROID)
EGLSurface GLContextEGL::CreateCompatibleSurface(void* aWindow) {
  if (mConfig == EGL_NO_CONFIG) {
    MOZ_CRASH("GFX: Failed with invalid EGLConfig 2!");
  }

  return GLContextProviderEGL::CreateEGLSurface(aWindow, mConfig);
}

<<<<<<< HEAD
/* static */ EGLSurface GLContextProviderEGL::CreateEGLSurface(
    void* aWindow, EGLConfig aConfig) {
  // NOTE: aWindow is an ANativeWindow
  nsCString discardFailureId;
  if (!GLLibraryEGL::EnsureInitialized(false, &discardFailureId)) {
    MOZ_CRASH("GFX: Failed to load EGL library 4!");
  }
  auto* egl = gl::GLLibraryEGL::Get();
  EGLConfig config = aConfig;
  if (!config && !CreateConfig(&config, /* aEnableDepthBuffer */ false)) {
    MOZ_CRASH("GFX: Failed to create EGLConfig 2!");
  }

  MOZ_ASSERT(aWindow);

  EGLSurface surface =
      egl->fCreateWindowSurface(EGL_DISPLAY(), config, aWindow, 0);
  if (surface == EGL_NO_SURFACE) {
    MOZ_CRASH("GFX: Failed to create EGLSurface 2!");
  }

  return surface;
||||||| merged common ancestors
/* static */ EGLSurface
GLContextProviderEGL::CreateEGLSurface(void* aWindow, EGLConfig aConfig)
{
    // NOTE: aWindow is an ANativeWindow
    nsCString discardFailureId;
    if (!GLLibraryEGL::EnsureInitialized(false, &discardFailureId)) {
        MOZ_CRASH("GFX: Failed to load EGL library 4!");
    }
    auto* egl = gl::GLLibraryEGL::Get();
    EGLConfig config = aConfig;
    if (!config && !CreateConfig(&config, /* aEnableDepthBuffer */ false)) {
        MOZ_CRASH("GFX: Failed to create EGLConfig 2!");
    }

    MOZ_ASSERT(aWindow);

    EGLSurface surface = egl->fCreateWindowSurface(EGL_DISPLAY(), config, aWindow,
                                                   0);
    if (surface == EGL_NO_SURFACE) {
        MOZ_CRASH("GFX: Failed to create EGLSurface 2!");
    }

    return surface;
=======
/* static */
EGLSurface GLContextProviderEGL::CreateEGLSurface(void* aWindow,
                                                  EGLConfig aConfig) {
  // NOTE: aWindow is an ANativeWindow
  nsCString discardFailureId;
  if (!GLLibraryEGL::EnsureInitialized(false, &discardFailureId)) {
    MOZ_CRASH("GFX: Failed to load EGL library 4!");
  }
  auto* egl = gl::GLLibraryEGL::Get();
  EGLConfig config = aConfig;
  if (!config && !CreateConfig(egl, &config, /* aEnableDepthBuffer */ false)) {
    MOZ_CRASH("GFX: Failed to create EGLConfig 2!");
  }

  MOZ_ASSERT(aWindow);

  EGLSurface surface =
      egl->fCreateWindowSurface(egl->Display(), config, aWindow, 0);
  if (surface == EGL_NO_SURFACE) {
    MOZ_CRASH("GFX: Failed to create EGLSurface 2!");
  }

  return surface;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ void GLContextProviderEGL::DestroyEGLSurface(EGLSurface surface) {
  nsCString discardFailureId;
  if (!GLLibraryEGL::EnsureInitialized(false, &discardFailureId)) {
    MOZ_CRASH("GFX: Failed to load EGL library 5!");
  }
  auto* egl = gl::GLLibraryEGL::Get();
  egl->fDestroySurface(EGL_DISPLAY(), surface);
||||||| merged common ancestors
/* static */ void
GLContextProviderEGL::DestroyEGLSurface(EGLSurface surface)
{
    nsCString discardFailureId;
    if (!GLLibraryEGL::EnsureInitialized(false, &discardFailureId)) {
        MOZ_CRASH("GFX: Failed to load EGL library 5!");
    }
    auto* egl = gl::GLLibraryEGL::Get();
    egl->fDestroySurface(EGL_DISPLAY(), surface);
=======
/* static */
void GLContextProviderEGL::DestroyEGLSurface(EGLSurface surface) {
  nsCString discardFailureId;
  if (!GLLibraryEGL::EnsureInitialized(false, &discardFailureId)) {
    MOZ_CRASH("GFX: Failed to load EGL library 5!");
  }
  auto* egl = gl::GLLibraryEGL::Get();
  egl->fDestroySurface(egl->Display(), surface);
>>>>>>> upstream-releases
}
#endif  // defined(ANDROID)

static void FillContextAttribs(bool alpha, bool depth, bool stencil, bool bpp16,
                               bool es3, nsTArray<EGLint>* out) {
  out->AppendElement(LOCAL_EGL_SURFACE_TYPE);
#if defined(MOZ_WAYLAND)
  if (!GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
    // Wayland on desktop does not support PBuffer or FBO.
    // We create a dummy wl_egl_window instead.
    out->AppendElement(LOCAL_EGL_WINDOW_BIT);
  } else {
    out->AppendElement(LOCAL_EGL_PBUFFER_BIT);
  }
#else
  out->AppendElement(LOCAL_EGL_PBUFFER_BIT);
#endif

  out->AppendElement(LOCAL_EGL_RENDERABLE_TYPE);
  if (es3) {
    out->AppendElement(LOCAL_EGL_OPENGL_ES3_BIT_KHR);
  } else {
    out->AppendElement(LOCAL_EGL_OPENGL_ES2_BIT);
  }

  out->AppendElement(LOCAL_EGL_RED_SIZE);
  if (bpp16) {
    out->AppendElement(alpha ? 4 : 5);
  } else {
    out->AppendElement(8);
  }

  out->AppendElement(LOCAL_EGL_GREEN_SIZE);
  if (bpp16) {
    out->AppendElement(alpha ? 4 : 6);
  } else {
    out->AppendElement(8);
  }

  out->AppendElement(LOCAL_EGL_BLUE_SIZE);
  if (bpp16) {
    out->AppendElement(alpha ? 4 : 5);
  } else {
    out->AppendElement(8);
  }

  out->AppendElement(LOCAL_EGL_ALPHA_SIZE);
  if (alpha) {
    out->AppendElement(bpp16 ? 4 : 8);
  } else {
    out->AppendElement(0);
  }

  out->AppendElement(LOCAL_EGL_DEPTH_SIZE);
  out->AppendElement(depth ? 16 : 0);

  out->AppendElement(LOCAL_EGL_STENCIL_SIZE);
  out->AppendElement(stencil ? 8 : 0);

  // EGL_ATTRIBS_LIST_SAFE_TERMINATION_WORKING_AROUND_BUGS
  out->AppendElement(LOCAL_EGL_NONE);
  out->AppendElement(0);

  out->AppendElement(0);
  out->AppendElement(0);
}

static GLint GetAttrib(GLLibraryEGL* egl, EGLConfig config, EGLint attrib) {
  EGLint bits = 0;
  egl->fGetConfigAttrib(egl->Display(), config, attrib, &bits);
  MOZ_ASSERT(egl->fGetError() == LOCAL_EGL_SUCCESS);

  return bits;
}

static EGLConfig ChooseConfig(GLLibraryEGL* egl, CreateContextFlags flags,
                              const SurfaceCaps& minCaps,
                              SurfaceCaps* const out_configCaps) {
  nsTArray<EGLint> configAttribList;
  FillContextAttribs(
      minCaps.alpha, minCaps.depth, minCaps.stencil, minCaps.bpp16,
      bool(flags & CreateContextFlags::PREFER_ES3), &configAttribList);

  const EGLint* configAttribs = configAttribList.Elements();

  // We're guaranteed to get at least minCaps, and the sorting dictated by the
  // spec for eglChooseConfig reasonably assures that a reasonable 'best' config
  // is on top.
  const EGLint kMaxConfigs = 1;
  EGLConfig configs[kMaxConfigs];
  EGLint foundConfigs = 0;
  if (!egl->fChooseConfig(egl->Display(), configAttribs, configs, kMaxConfigs,
                          &foundConfigs) ||
      foundConfigs == 0) {
    return EGL_NO_CONFIG;
  }

  EGLConfig config = configs[0];

  *out_configCaps = minCaps;  // Pick up any preserve, etc.
  out_configCaps->color = true;
  out_configCaps->alpha = bool(GetAttrib(egl, config, LOCAL_EGL_ALPHA_SIZE));
  out_configCaps->depth = bool(GetAttrib(egl, config, LOCAL_EGL_DEPTH_SIZE));
  out_configCaps->stencil =
      bool(GetAttrib(egl, config, LOCAL_EGL_STENCIL_SIZE));
  out_configCaps->bpp16 = (GetAttrib(egl, config, LOCAL_EGL_RED_SIZE) < 8);

  return config;
}

<<<<<<< HEAD
/*static*/ already_AddRefed<GLContextEGL>
GLContextEGL::CreateEGLPBufferOffscreenContext(
    CreateContextFlags flags, const mozilla::gfx::IntSize& size,
    const SurfaceCaps& minCaps, nsACString* const out_failureId) {
  bool forceEnableHardware =
      bool(flags & CreateContextFlags::FORCE_ENABLE_HARDWARE);
  if (!GLLibraryEGL::EnsureInitialized(forceEnableHardware, out_failureId)) {
    return nullptr;
  }

  auto* egl = gl::GLLibraryEGL::Get();
  SurfaceCaps configCaps;
  EGLConfig config = ChooseConfig(egl, flags, minCaps, &configCaps);
  if (config == EGL_NO_CONFIG) {
    *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_NO_CONFIG");
    NS_WARNING("Failed to find a compatible config.");
    return nullptr;
  }
||||||| merged common ancestors
/*static*/ already_AddRefed<GLContextEGL>
GLContextEGL::CreateEGLPBufferOffscreenContext(CreateContextFlags flags,
                                               const mozilla::gfx::IntSize& size,
                                               const SurfaceCaps& minCaps,
                                               nsACString* const out_failureId)
{
    bool forceEnableHardware = bool(flags & CreateContextFlags::FORCE_ENABLE_HARDWARE);
    if (!GLLibraryEGL::EnsureInitialized(forceEnableHardware, out_failureId)) {
        return nullptr;
    }

    auto* egl = gl::GLLibraryEGL::Get();
    SurfaceCaps configCaps;
    EGLConfig config = ChooseConfig(egl, flags, minCaps, &configCaps);
    if (config == EGL_NO_CONFIG) {
        *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_NO_CONFIG");
        NS_WARNING("Failed to find a compatible config.");
        return nullptr;
    }
=======
/*static*/
already_AddRefed<GLContextEGL> GLContextEGL::CreateEGLPBufferOffscreenContext(
    CreateContextFlags flags, const mozilla::gfx::IntSize& size,
    const SurfaceCaps& minCaps, nsACString* const out_failureId) {
  bool forceEnableHardware =
      bool(flags & CreateContextFlags::FORCE_ENABLE_HARDWARE);
  if (!GLLibraryEGL::EnsureInitialized(forceEnableHardware, out_failureId)) {
    return nullptr;
  }

  auto* egl = gl::GLLibraryEGL::Get();
  SurfaceCaps configCaps;
  EGLConfig config = ChooseConfig(egl, flags, minCaps, &configCaps);
  if (config == EGL_NO_CONFIG) {
    *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_NO_CONFIG");
    NS_WARNING("Failed to find a compatible config.");
    return nullptr;
  }
>>>>>>> upstream-releases

  if (GLContext::ShouldSpew()) {
    egl->DumpEGLConfig(config);
  }

  mozilla::gfx::IntSize pbSize(size);
  EGLSurface surface = nullptr;
#if defined(MOZ_WAYLAND)
<<<<<<< HEAD
  if (!GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
    surface = GLContextEGL::CreateWaylandBufferSurface(config, pbSize);
  } else
||||||| merged common ancestors
    if (GDK_IS_WAYLAND_DISPLAY(gdk_display_get_default())) {
        surface = GLContextEGL::CreateWaylandBufferSurface(config, pbSize);
    } else
=======
  if (!GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
    surface = GLContextEGL::CreateWaylandBufferSurface(egl, config, pbSize);
  } else
>>>>>>> upstream-releases
#endif
<<<<<<< HEAD
  {
    surface = GLContextEGL::CreatePBufferSurfaceTryingPowerOfTwo(
        config, LOCAL_EGL_NONE, pbSize);
  }
  if (!surface) {
    *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_POT");
    NS_WARNING("Failed to create PBuffer for context!");
    return nullptr;
  }
||||||| merged common ancestors
    {
        surface = GLContextEGL::CreatePBufferSurfaceTryingPowerOfTwo(config,
                                                                     LOCAL_EGL_NONE,
                                                                     pbSize);
    }
    if (!surface) {
        *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_POT");
        NS_WARNING("Failed to create PBuffer for context!");
        return nullptr;
    }
=======
  {
    surface = GLContextEGL::CreatePBufferSurfaceTryingPowerOfTwo(
        egl, config, LOCAL_EGL_NONE, pbSize);
  }
  if (!surface) {
    *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_POT");
    NS_WARNING("Failed to create PBuffer for context!");
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<GLContextEGL> gl = GLContextEGL::CreateGLContext(
      flags, configCaps, true, config, surface, out_failureId);
  if (!gl) {
    NS_WARNING("Failed to create GLContext from PBuffer");
    egl->fDestroySurface(egl->Display(), surface);
||||||| merged common ancestors
    RefPtr<GLContextEGL> gl = GLContextEGL::CreateGLContext(flags, configCaps, true,
                                                            config, surface,
                                                            out_failureId);
    if (!gl) {
        NS_WARNING("Failed to create GLContext from PBuffer");
        egl->fDestroySurface(egl->Display(), surface);
=======
  RefPtr<GLContextEGL> gl = GLContextEGL::CreateGLContext(
      egl, flags, configCaps, true, config, surface, out_failureId);
  if (!gl) {
    NS_WARNING("Failed to create GLContext from PBuffer");
    egl->fDestroySurface(egl->Display(), surface);
>>>>>>> upstream-releases
#if defined(MOZ_WAYLAND)
    DeleteWaylandGLSurface(surface);
#endif
    return nullptr;
  }

  return gl.forget();
}

<<<<<<< HEAD
/*static*/ already_AddRefed<GLContext> GLContextProviderEGL::CreateHeadless(
    CreateContextFlags flags, nsACString* const out_failureId) {
  mozilla::gfx::IntSize dummySize = mozilla::gfx::IntSize(16, 16);
  SurfaceCaps dummyCaps = SurfaceCaps::Any();
  return GLContextEGL::CreateEGLPBufferOffscreenContext(
      flags, dummySize, dummyCaps, out_failureId);
||||||| merged common ancestors
/*static*/ already_AddRefed<GLContext>
GLContextProviderEGL::CreateHeadless(CreateContextFlags flags,
                                     nsACString* const out_failureId)
{
    mozilla::gfx::IntSize dummySize = mozilla::gfx::IntSize(16, 16);
    SurfaceCaps dummyCaps = SurfaceCaps::Any();
    return GLContextEGL::CreateEGLPBufferOffscreenContext(flags, dummySize, dummyCaps,
                                                          out_failureId);
=======
/*static*/
already_AddRefed<GLContext> GLContextProviderEGL::CreateHeadless(
    CreateContextFlags flags, nsACString* const out_failureId) {
  mozilla::gfx::IntSize dummySize = mozilla::gfx::IntSize(16, 16);
  SurfaceCaps dummyCaps = SurfaceCaps::Any();
  return GLContextEGL::CreateEGLPBufferOffscreenContext(
      flags, dummySize, dummyCaps, out_failureId);
>>>>>>> upstream-releases
}

// Under EGL, on Android, pbuffers are supported fine, though
// often without the ability to texture from them directly.
<<<<<<< HEAD
/*static*/ already_AddRefed<GLContext> GLContextProviderEGL::CreateOffscreen(
    const mozilla::gfx::IntSize& size, const SurfaceCaps& minCaps,
    CreateContextFlags flags, nsACString* const out_failureId) {
  bool forceEnableHardware =
      bool(flags & CreateContextFlags::FORCE_ENABLE_HARDWARE);
  if (!GLLibraryEGL::EnsureInitialized(
          forceEnableHardware, out_failureId)) {  // Needed for IsANGLE().
    return nullptr;
  }
||||||| merged common ancestors
/*static*/ already_AddRefed<GLContext>
GLContextProviderEGL::CreateOffscreen(const mozilla::gfx::IntSize& size,
                                      const SurfaceCaps& minCaps,
                                      CreateContextFlags flags,
                                      nsACString* const out_failureId)
{
    bool forceEnableHardware = bool(flags & CreateContextFlags::FORCE_ENABLE_HARDWARE);
    if (!GLLibraryEGL::EnsureInitialized(forceEnableHardware, out_failureId)) { // Needed for IsANGLE().
        return nullptr;
    }

    auto* egl = gl::GLLibraryEGL::Get();
    bool canOffscreenUseHeadless = true;
    if (egl->IsANGLE()) {
        // ANGLE needs to use PBuffers.
        canOffscreenUseHeadless = false;
    }
=======
/*static*/
already_AddRefed<GLContext> GLContextProviderEGL::CreateOffscreen(
    const mozilla::gfx::IntSize& size, const SurfaceCaps& minCaps,
    CreateContextFlags flags, nsACString* const out_failureId) {
  bool forceEnableHardware =
      bool(flags & CreateContextFlags::FORCE_ENABLE_HARDWARE);
  if (!GLLibraryEGL::EnsureInitialized(
          forceEnableHardware, out_failureId)) {  // Needed for IsANGLE().
    return nullptr;
  }
>>>>>>> upstream-releases

  auto* egl = gl::GLLibraryEGL::Get();
  bool canOffscreenUseHeadless = true;
  if (egl->IsANGLE()) {
    // ANGLE needs to use PBuffers.
    canOffscreenUseHeadless = false;
<<<<<<< HEAD
  }

#if defined(MOZ_WIDGET_ANDROID)
  // Using a headless context loses the SurfaceCaps
  // which can cause a loss of depth and/or stencil
  canOffscreenUseHeadless = false;
#endif  //  defined(MOZ_WIDGET_ANDROID)
||||||| merged common ancestors
#endif //  defined(MOZ_WIDGET_ANDROID)

    RefPtr<GLContext> gl;
    SurfaceCaps minOffscreenCaps = minCaps;
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<GLContext> gl;
  SurfaceCaps minOffscreenCaps = minCaps;
||||||| merged common ancestors
    if (canOffscreenUseHeadless) {
        gl = CreateHeadless(flags, out_failureId);
        if (!gl) {
            return nullptr;
        }
    } else {
        SurfaceCaps minBackbufferCaps = minOffscreenCaps;
        if (minOffscreenCaps.antialias) {
            minBackbufferCaps.antialias = false;
            minBackbufferCaps.depth = false;
            minBackbufferCaps.stencil = false;
        }
=======
#if defined(MOZ_WIDGET_ANDROID)
  // Using a headless context loses the SurfaceCaps
  // which can cause a loss of depth and/or stencil
  canOffscreenUseHeadless = false;
#endif  //  defined(MOZ_WIDGET_ANDROID)
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (canOffscreenUseHeadless) {
    gl = CreateHeadless(flags, out_failureId);
    if (!gl) {
      return nullptr;
    }
  } else {
    SurfaceCaps minBackbufferCaps = minOffscreenCaps;
    if (minOffscreenCaps.antialias) {
      minBackbufferCaps.antialias = false;
      minBackbufferCaps.depth = false;
      minBackbufferCaps.stencil = false;
    }
||||||| merged common ancestors
        gl = GLContextEGL::CreateEGLPBufferOffscreenContext(flags, size,
                                                            minBackbufferCaps,
                                                            out_failureId);
        if (!gl)
            return nullptr;

        // Pull the actual resulting caps to ensure that our offscreen matches our
        // backbuffer.
        minOffscreenCaps.alpha = gl->Caps().alpha;
        if (!minOffscreenCaps.antialias) {
            // Only update these if we don't have AA. If we do have AA, we ignore
            // backbuffer depth/stencil.
            minOffscreenCaps.depth = gl->Caps().depth;
            minOffscreenCaps.stencil = gl->Caps().stencil;
        }
    }
=======
  RefPtr<GLContext> gl;
  SurfaceCaps minOffscreenCaps = minCaps;
>>>>>>> upstream-releases

<<<<<<< HEAD
    gl = GLContextEGL::CreateEGLPBufferOffscreenContext(
        flags, size, minBackbufferCaps, out_failureId);
    if (!gl) return nullptr;

    // Pull the actual resulting caps to ensure that our offscreen matches our
    // backbuffer.
    minOffscreenCaps.alpha = gl->Caps().alpha;
    if (!minOffscreenCaps.antialias) {
      // Only update these if we don't have AA. If we do have AA, we ignore
      // backbuffer depth/stencil.
      minOffscreenCaps.depth = gl->Caps().depth;
      minOffscreenCaps.stencil = gl->Caps().stencil;
||||||| merged common ancestors
    // Init the offscreen with the updated offscreen caps.
    if (!gl->InitOffscreen(size, minOffscreenCaps)) {
        *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_OFFSCREEN");
        return nullptr;
=======
  if (canOffscreenUseHeadless) {
    gl = CreateHeadless(flags, out_failureId);
    if (!gl) {
      return nullptr;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

  // Init the offscreen with the updated offscreen caps.
  if (!gl->InitOffscreen(size, minOffscreenCaps)) {
    *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_OFFSCREEN");
    return nullptr;
  }

  return gl.forget();
}

// Don't want a global context on Android as 1) share groups across 2 threads
// fail on many Tegra drivers (bug 759225) and 2) some mobile devices have a
// very strict limit on global number of GL contexts (bug 754257) and 3) each
// EGL context eats 750k on B2G (bug 813783)
/*static*/ GLContext* GLContextProviderEGL::GetGlobalContext() {
  return nullptr;
||||||| merged common ancestors

    return gl.forget();
}

// Don't want a global context on Android as 1) share groups across 2 threads fail on many Tegra drivers (bug 759225)
// and 2) some mobile devices have a very strict limit on global number of GL contexts (bug 754257)
// and 3) each EGL context eats 750k on B2G (bug 813783)
/*static*/ GLContext*
GLContextProviderEGL::GetGlobalContext()
{
    return nullptr;
=======
  } else {
    gl = GLContextEGL::CreateEGLPBufferOffscreenContext(
        flags, size, minOffscreenCaps, out_failureId);
    if (!gl) return nullptr;

    // Pull the actual resulting caps to ensure that our offscreen matches our
    // backbuffer.
    minOffscreenCaps.alpha = gl->Caps().alpha;
    minOffscreenCaps.depth = gl->Caps().depth;
    minOffscreenCaps.stencil = gl->Caps().stencil;
  }

  // Init the offscreen with the updated offscreen caps.
  if (!gl->InitOffscreen(size, minOffscreenCaps)) {
    *out_failureId = NS_LITERAL_CSTRING("FEATURE_FAILURE_EGL_OFFSCREEN");
    return nullptr;
  }

  return gl.forget();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/*static*/ void GLContextProviderEGL::Shutdown() {
  const RefPtr<GLLibraryEGL> egl = GLLibraryEGL::Get();
  if (egl) {
    egl->Shutdown();
  }
||||||| merged common ancestors
/*static*/ void
GLContextProviderEGL::Shutdown()
{
    const RefPtr<GLLibraryEGL> egl = GLLibraryEGL::Get();
    if (egl) {
        egl->Shutdown();
    }
=======
// Don't want a global context on Android as 1) share groups across 2 threads
// fail on many Tegra drivers (bug 759225) and 2) some mobile devices have a
// very strict limit on global number of GL contexts (bug 754257) and 3) each
// EGL context eats 750k on B2G (bug 813783)
/*static*/
GLContext* GLContextProviderEGL::GetGlobalContext() { return nullptr; }

/*static*/
void GLContextProviderEGL::Shutdown() {
  const RefPtr<GLLibraryEGL> egl = GLLibraryEGL::Get();
  if (egl) {
    egl->Shutdown();
  }
>>>>>>> upstream-releases
}

} /* namespace gl */
} /* namespace mozilla */

#undef EGL_ATTRIBS_LIST_SAFE_TERMINATION_WORKING_AROUND_BUGS
