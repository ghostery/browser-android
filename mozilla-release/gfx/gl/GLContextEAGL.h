/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GLCONTEXTEAGL_H_
#define GLCONTEXTEAGL_H_

#include "GLContext.h"

#include <CoreGraphics/CoreGraphics.h>
#include <OpenGLES/EAGL.h>

namespace mozilla {
namespace gl {

class GLContextEAGL : public GLContext {
  friend class GLContextProviderEAGL;

  EAGLContext* const mContext;

 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(GLContextEAGL, override)
  GLContextEAGL(CreateContextFlags flags, const SurfaceCaps& caps,
                EAGLContext* context, GLContext* sharedContext,
                bool isOffscreen, ContextProfile profile);

  ~GLContextEAGL();

  virtual GLContextType GetContextType() const override {
    return GLContextType::EAGL;
  }

  static GLContextEAGL* Cast(GLContext* gl) {
    MOZ_ASSERT(gl->GetContextType() == GLContextType::EAGL);
    return static_cast<GLContextEAGL*>(gl);
  }

<<<<<<< HEAD
  bool Init() override;
||||||| merged common ancestors
    bool Init() override;
=======
  bool AttachToWindow(nsIWidget* aWidget);
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool AttachToWindow(nsIWidget* aWidget);
||||||| merged common ancestors
    bool AttachToWindow(nsIWidget* aWidget);
=======
  EAGLContext* GetEAGLContext() const { return mContext; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  EAGLContext* GetEAGLContext() const { return mContext; }
||||||| merged common ancestors
    EAGLContext* GetEAGLContext() const { return mContext; }
=======
  virtual bool MakeCurrentImpl() const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool MakeCurrentImpl() const override;
||||||| merged common ancestors
    virtual bool MakeCurrentImpl() const override;
=======
  virtual bool IsCurrentImpl() const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool IsCurrentImpl() const override;
||||||| merged common ancestors
    virtual bool IsCurrentImpl() const override;
=======
  Maybe<SymbolLoader> GetSymbolLoader() const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool SetupLookupFunction() override;
||||||| merged common ancestors
    virtual bool SetupLookupFunction() override;
=======
  virtual bool IsDoubleBuffered() const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool IsDoubleBuffered() const override;
||||||| merged common ancestors
    virtual bool IsDoubleBuffered() const override;
=======
  virtual bool SwapBuffers() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool SwapBuffers() override;
||||||| merged common ancestors
    virtual bool SwapBuffers() override;
=======
  virtual void GetWSIInfo(nsCString* const out) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void GetWSIInfo(nsCString* const out) const override;
||||||| merged common ancestors
    virtual void GetWSIInfo(nsCString* const out) const override;
=======
  virtual GLuint GetDefaultFramebuffer() override { return mBackbufferFB; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual GLuint GetDefaultFramebuffer() override { return mBackbufferFB; }
||||||| merged common ancestors
    virtual GLuint GetDefaultFramebuffer() override {
        return mBackbufferFB;
    }
=======
  virtual bool RenewSurface(nsIWidget* aWidget) override {
    // FIXME: should use the passed widget instead of the existing one.
    return RecreateRB();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool RenewSurface(nsIWidget* aWidget) override {
    // FIXME: should use the passed widget instead of the existing one.
    return RecreateRB();
  }
||||||| merged common ancestors
    virtual bool RenewSurface(nsIWidget* aWidget) override {
        // FIXME: should use the passed widget instead of the existing one.
        return RecreateRB();
    }
=======
 private:
  GLuint mBackbufferRB = 0;
  GLuint mBackbufferFB = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  GLuint mBackbufferRB = 0;
  GLuint mBackbufferFB = 0;
||||||| merged common ancestors
private:
    GLuint mBackbufferRB = 0;
    GLuint mBackbufferFB = 0;
=======
  void* mLayer = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void* mLayer = nullptr;

  bool RecreateRB();
||||||| merged common ancestors
    void* mLayer = nullptr;

    bool RecreateRB();
=======
  bool RecreateRB();
>>>>>>> upstream-releases
};

}  // namespace gl
}  // namespace mozilla

#endif  // GLCONTEXTEAGL_H_
