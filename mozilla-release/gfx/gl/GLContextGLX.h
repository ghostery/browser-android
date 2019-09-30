/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GLCONTEXTGLX_H_
#define GLCONTEXTGLX_H_

#include "GLContext.h"
#include "GLXLibrary.h"
#include "mozilla/X11Util.h"

class gfxXlibSurface;

namespace mozilla {
namespace gl {

class GLContextGLX : public GLContext {
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(GLContextGLX, override)
  static already_AddRefed<GLContextGLX> CreateGLContext(
      CreateContextFlags flags, const SurfaceCaps& caps, bool isOffscreen,
      Display* display, GLXDrawable drawable, GLXFBConfig cfg,
      bool deleteDrawable, gfxXlibSurface* pixmap);

  static bool FindVisual(Display* display, int screen, bool useWebRender,
                         bool useAlpha, int* const out_visualId);

  // Finds a GLXFBConfig compatible with the provided window.
  static bool FindFBConfigForWindow(
      Display* display, int screen, Window window,
      ScopedXFree<GLXFBConfig>* const out_scopedConfigArr,
      GLXFBConfig* const out_config, int* const out_visid, bool aWebRender);

<<<<<<< HEAD
  ~GLContextGLX() override;
||||||| merged common ancestors
    ~GLContextGLX();
=======
  virtual ~GLContextGLX();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual GLContextType GetContextType() const override {
    return GLContextType::GLX;
  }
||||||| merged common ancestors
    virtual GLContextType GetContextType() const override { return GLContextType::GLX; }
=======
  GLContextType GetContextType() const override { return GLContextType::GLX; }
>>>>>>> upstream-releases

  static GLContextGLX* Cast(GLContext* gl) {
    MOZ_ASSERT(gl->GetContextType() == GLContextType::GLX);
    return static_cast<GLContextGLX*>(gl);
  }

  bool Init() override;

<<<<<<< HEAD
  virtual bool MakeCurrentImpl() const override;
||||||| merged common ancestors
    virtual bool MakeCurrentImpl() const override;
=======
  bool MakeCurrentImpl() const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool IsCurrentImpl() const override;
||||||| merged common ancestors
    virtual bool IsCurrentImpl() const override;
=======
  bool IsCurrentImpl() const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool SetupLookupFunction() override;
||||||| merged common ancestors
    virtual bool SetupLookupFunction() override;
=======
  Maybe<SymbolLoader> GetSymbolLoader() const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool IsDoubleBuffered() const override;
||||||| merged common ancestors
    virtual bool IsDoubleBuffered() const override;
=======
  bool IsDoubleBuffered() const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool SwapBuffers() override;
||||||| merged common ancestors
    virtual bool SwapBuffers() override;
=======
  bool SwapBuffers() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void GetWSIInfo(nsCString* const out) const override;
||||||| merged common ancestors
    virtual void GetWSIInfo(nsCString* const out) const override;
=======
  void GetWSIInfo(nsCString* const out) const override;
>>>>>>> upstream-releases

  // Overrides the current GLXDrawable backing the context and makes the
  // context current.
  bool OverrideDrawable(GLXDrawable drawable);

  // Undoes the effect of a drawable override.
  bool RestoreDrawable();

 private:
  friend class GLContextProviderGLX;

  GLContextGLX(CreateContextFlags flags, const SurfaceCaps& caps,
               bool isOffscreen, Display* aDisplay, GLXDrawable aDrawable,
               GLXContext aContext, bool aDeleteDrawable, bool aDoubleBuffered,
               gfxXlibSurface* aPixmap);

  GLXContext mContext;
  Display* mDisplay;
  GLXDrawable mDrawable;
  bool mDeleteDrawable;
  bool mDoubleBuffered;

  GLXLibrary* mGLX;

  RefPtr<gfxXlibSurface> mPixmap;
  bool mOwnsContext = true;
};

}  // namespace gl
}  // namespace mozilla

#endif  // GLCONTEXTGLX_H_
