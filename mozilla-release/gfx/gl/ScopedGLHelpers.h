/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef SCOPEDGLHELPERS_H_
#define SCOPEDGLHELPERS_H_

#include "GLDefs.h"
#include "mozilla/UniquePtr.h"

namespace mozilla {
namespace gl {

class GLContext;

#ifdef DEBUG
bool IsContextCurrent(GLContext* gl);
#endif

<<<<<<< HEAD
// RAII via CRTP!
template <class Derived>
struct ScopedGLWrapper {
 private:
  bool mIsUnwrapped;

 protected:
  GLContext* const mGL;

  explicit ScopedGLWrapper(GLContext* gl) : mIsUnwrapped(false), mGL(gl) {
    MOZ_ASSERT(&ScopedGLWrapper<Derived>::Unwrap == &Derived::Unwrap);
    MOZ_ASSERT(&Derived::UnwrapImpl);
  }

  virtual ~ScopedGLWrapper() {
    if (!mIsUnwrapped) Unwrap();
  }

 public:
  void Unwrap() {
    MOZ_ASSERT(!mIsUnwrapped);

    Derived* derived = static_cast<Derived*>(this);
    derived->UnwrapImpl();

    mIsUnwrapped = true;
  }
};

||||||| merged common ancestors
//RAII via CRTP!
template <class Derived>
struct ScopedGLWrapper
{
private:
    bool mIsUnwrapped;

protected:
    GLContext* const mGL;

    explicit ScopedGLWrapper(GLContext* gl)
        : mIsUnwrapped(false)
        , mGL(gl)
    {
        MOZ_ASSERT(&ScopedGLWrapper<Derived>::Unwrap == &Derived::Unwrap);
        MOZ_ASSERT(&Derived::UnwrapImpl);
        MOZ_ASSERT(IsContextCurrent(mGL));
    }

    virtual ~ScopedGLWrapper() {
        if (!mIsUnwrapped)
            Unwrap();
    }

public:
    void Unwrap() {
        MOZ_ASSERT(!mIsUnwrapped);
        MOZ_ASSERT(IsContextCurrent(mGL));

        Derived* derived = static_cast<Derived*>(this);
        derived->UnwrapImpl();

        mIsUnwrapped = true;
    }
};

=======
>>>>>>> upstream-releases
// Wraps glEnable/Disable.
<<<<<<< HEAD
struct ScopedGLState : public ScopedGLWrapper<ScopedGLState> {
  friend struct ScopedGLWrapper<ScopedGLState>;

 protected:
  const GLenum mCapability;
  bool mOldState;

 public:
  // Use |newState = true| to enable, |false| to disable.
  ScopedGLState(GLContext* aGL, GLenum aCapability, bool aNewState);
  // variant that doesn't change state; simply records existing state to be
  // restored by the destructor
  ScopedGLState(GLContext* aGL, GLenum aCapability);

 protected:
  void UnwrapImpl();
||||||| merged common ancestors
struct ScopedGLState
    : public ScopedGLWrapper<ScopedGLState>
{
    friend struct ScopedGLWrapper<ScopedGLState>;

protected:
    const GLenum mCapability;
    bool mOldState;

public:
    // Use |newState = true| to enable, |false| to disable.
    ScopedGLState(GLContext* aGL, GLenum aCapability, bool aNewState);
    // variant that doesn't change state; simply records existing state to be
    // restored by the destructor
    ScopedGLState(GLContext* aGL, GLenum aCapability);

protected:
    void UnwrapImpl();
=======
struct ScopedGLState final {
 private:
  GLContext* const mGL;
  const GLenum mCapability;
  bool mOldState;

 public:
  // Use |newState = true| to enable, |false| to disable.
  ScopedGLState(GLContext* aGL, GLenum aCapability, bool aNewState);
  // variant that doesn't change state; simply records existing state to be
  // restored by the destructor
  ScopedGLState(GLContext* aGL, GLenum aCapability);
  ~ScopedGLState();
>>>>>>> upstream-releases
};

// Saves and restores with GetUserBoundFB and BindUserFB.
<<<<<<< HEAD
struct ScopedBindFramebuffer : public ScopedGLWrapper<ScopedBindFramebuffer> {
  friend struct ScopedGLWrapper<ScopedBindFramebuffer>;

 protected:
  GLuint mOldReadFB;
  GLuint mOldDrawFB;

 private:
  void Init();

 public:
  explicit ScopedBindFramebuffer(GLContext* aGL);
  ScopedBindFramebuffer(GLContext* aGL, GLuint aNewFB);

 protected:
  void UnwrapImpl();
||||||| merged common ancestors
struct ScopedBindFramebuffer
    : public ScopedGLWrapper<ScopedBindFramebuffer>
{
    friend struct ScopedGLWrapper<ScopedBindFramebuffer>;

protected:
    GLuint mOldReadFB;
    GLuint mOldDrawFB;

private:
    void Init();

public:
    explicit ScopedBindFramebuffer(GLContext* aGL);
    ScopedBindFramebuffer(GLContext* aGL, GLuint aNewFB);

protected:
    void UnwrapImpl();
=======
struct ScopedBindFramebuffer final {
 private:
  GLContext* const mGL;
  GLuint mOldReadFB;
  GLuint mOldDrawFB;

  void Init();

 public:
  explicit ScopedBindFramebuffer(GLContext* aGL);
  ScopedBindFramebuffer(GLContext* aGL, GLuint aNewFB);
  ~ScopedBindFramebuffer();
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ScopedBindTextureUnit : public ScopedGLWrapper<ScopedBindTextureUnit> {
  friend struct ScopedGLWrapper<ScopedBindTextureUnit>;
||||||| merged common ancestors
struct ScopedBindTextureUnit
    : public ScopedGLWrapper<ScopedBindTextureUnit>
{
    friend struct ScopedGLWrapper<ScopedBindTextureUnit>;
=======
struct ScopedBindTextureUnit final {
 private:
  GLContext* const mGL;
  GLenum mOldTexUnit;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  GLenum mOldTexUnit;

 public:
  ScopedBindTextureUnit(GLContext* aGL, GLenum aTexUnit);

 protected:
  void UnwrapImpl();
||||||| merged common ancestors
protected:
    GLenum mOldTexUnit;

public:
    ScopedBindTextureUnit(GLContext* aGL, GLenum aTexUnit);

protected:
    void UnwrapImpl();
=======
 public:
  ScopedBindTextureUnit(GLContext* aGL, GLenum aTexUnit);
  ~ScopedBindTextureUnit();
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ScopedTexture : public ScopedGLWrapper<ScopedTexture> {
  friend struct ScopedGLWrapper<ScopedTexture>;
||||||| merged common ancestors
=======
struct ScopedTexture final {
 private:
  GLContext* const mGL;
  GLuint mTexture;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  GLuint mTexture;

 public:
  explicit ScopedTexture(GLContext* aGL);

  GLuint Texture() const { return mTexture; }
  operator GLuint() const { return mTexture; }

 protected:
  void UnwrapImpl();
||||||| merged common ancestors
struct ScopedTexture
    : public ScopedGLWrapper<ScopedTexture>
{
    friend struct ScopedGLWrapper<ScopedTexture>;

protected:
    GLuint mTexture;

public:
    explicit ScopedTexture(GLContext* aGL);

    GLuint Texture() const { return mTexture; }
    operator GLuint() const { return mTexture; }

protected:
    void UnwrapImpl();
=======
 public:
  explicit ScopedTexture(GLContext* aGL);
  ~ScopedTexture();

  GLuint Texture() const { return mTexture; }
  operator GLuint() const { return mTexture; }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ScopedFramebuffer : public ScopedGLWrapper<ScopedFramebuffer> {
  friend struct ScopedGLWrapper<ScopedFramebuffer>;
||||||| merged common ancestors
=======
struct ScopedFramebuffer final {
 private:
  GLContext* const mGL;
  GLuint mFB;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  GLuint mFB;

 public:
  explicit ScopedFramebuffer(GLContext* aGL);
  GLuint FB() { return mFB; }

 protected:
  void UnwrapImpl();
||||||| merged common ancestors
struct ScopedFramebuffer
    : public ScopedGLWrapper<ScopedFramebuffer>
{
    friend struct ScopedGLWrapper<ScopedFramebuffer>;

protected:
    GLuint mFB;

public:
    explicit ScopedFramebuffer(GLContext* aGL);
    GLuint FB() { return mFB; }

protected:
    void UnwrapImpl();
=======
 public:
  explicit ScopedFramebuffer(GLContext* aGL);
  ~ScopedFramebuffer();

  const auto& FB() const { return mFB; }
  operator GLuint() const { return mFB; }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ScopedRenderbuffer : public ScopedGLWrapper<ScopedRenderbuffer> {
  friend struct ScopedGLWrapper<ScopedRenderbuffer>;
||||||| merged common ancestors
=======
struct ScopedRenderbuffer final {
 private:
  GLContext* const mGL;
  GLuint mRB;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  GLuint mRB;
||||||| merged common ancestors
struct ScopedRenderbuffer
    : public ScopedGLWrapper<ScopedRenderbuffer>
{
    friend struct ScopedGLWrapper<ScopedRenderbuffer>;

protected:
    GLuint mRB;
=======
 public:
  explicit ScopedRenderbuffer(GLContext* aGL);
  ~ScopedRenderbuffer();
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  explicit ScopedRenderbuffer(GLContext* aGL);
  GLuint RB() { return mRB; }

 protected:
  void UnwrapImpl();
||||||| merged common ancestors
public:
    explicit ScopedRenderbuffer(GLContext* aGL);
    GLuint RB() { return mRB; }

protected:
    void UnwrapImpl();
=======
  GLuint RB() { return mRB; }
  operator GLuint() const { return mRB; }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ScopedBindTexture : public ScopedGLWrapper<ScopedBindTexture> {
  friend struct ScopedGLWrapper<ScopedBindTexture>;
||||||| merged common ancestors
=======
struct ScopedBindTexture final {
 private:
  GLContext* const mGL;
  const GLenum mTarget;
  const GLuint mOldTex;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  const GLenum mTarget;
  const GLuint mOldTex;

 public:
  ScopedBindTexture(GLContext* aGL, GLuint aNewTex,
                    GLenum aTarget = LOCAL_GL_TEXTURE_2D);

 protected:
  void UnwrapImpl();
||||||| merged common ancestors
struct ScopedBindTexture
    : public ScopedGLWrapper<ScopedBindTexture>
{
    friend struct ScopedGLWrapper<ScopedBindTexture>;

protected:
    const GLenum mTarget;
    const GLuint mOldTex;

public:
    ScopedBindTexture(GLContext* aGL, GLuint aNewTex,
                      GLenum aTarget = LOCAL_GL_TEXTURE_2D);

protected:
    void UnwrapImpl();
=======
 public:
  ScopedBindTexture(GLContext* aGL, GLuint aNewTex,
                    GLenum aTarget = LOCAL_GL_TEXTURE_2D);
  ~ScopedBindTexture();
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ScopedBindRenderbuffer : public ScopedGLWrapper<ScopedBindRenderbuffer> {
  friend struct ScopedGLWrapper<ScopedBindRenderbuffer>;
||||||| merged common ancestors
=======
struct ScopedBindRenderbuffer final {
 private:
  GLContext* const mGL;
  GLuint mOldRB;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  GLuint mOldRB;

 private:
  void Init();

 public:
  explicit ScopedBindRenderbuffer(GLContext* aGL);
||||||| merged common ancestors
struct ScopedBindRenderbuffer
    : public ScopedGLWrapper<ScopedBindRenderbuffer>
{
    friend struct ScopedGLWrapper<ScopedBindRenderbuffer>;

protected:
    GLuint mOldRB;

private:
    void Init();

public:
    explicit ScopedBindRenderbuffer(GLContext* aGL);
=======
 private:
  void Init();
>>>>>>> upstream-releases

<<<<<<< HEAD
  ScopedBindRenderbuffer(GLContext* aGL, GLuint aNewRB);

 protected:
  void UnwrapImpl();
||||||| merged common ancestors
    ScopedBindRenderbuffer(GLContext* aGL, GLuint aNewRB);

protected:
    void UnwrapImpl();
=======
 public:
  explicit ScopedBindRenderbuffer(GLContext* aGL);
  ScopedBindRenderbuffer(GLContext* aGL, GLuint aNewRB);
  ~ScopedBindRenderbuffer();
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ScopedFramebufferForTexture
    : public ScopedGLWrapper<ScopedFramebufferForTexture> {
  friend struct ScopedGLWrapper<ScopedFramebufferForTexture>;

 protected:
  bool mComplete;  // True if the framebuffer we create is complete.
  GLuint mFB;

 public:
  ScopedFramebufferForTexture(GLContext* aGL, GLuint aTexture,
                              GLenum aTarget = LOCAL_GL_TEXTURE_2D);
||||||| merged common ancestors

struct ScopedFramebufferForTexture
    : public ScopedGLWrapper<ScopedFramebufferForTexture>
{
    friend struct ScopedGLWrapper<ScopedFramebufferForTexture>;

protected:
    bool mComplete; // True if the framebuffer we create is complete.
    GLuint mFB;

public:
    ScopedFramebufferForTexture(GLContext* aGL, GLuint aTexture,
                                GLenum aTarget = LOCAL_GL_TEXTURE_2D);
=======
struct ScopedFramebufferForTexture final {
 private:
  GLContext* const mGL;
  bool mComplete;  // True if the framebuffer we create is complete.
  GLuint mFB;

 public:
  ScopedFramebufferForTexture(GLContext* aGL, GLuint aTexture,
                              GLenum aTarget = LOCAL_GL_TEXTURE_2D);
  ~ScopedFramebufferForTexture();
>>>>>>> upstream-releases

  bool IsComplete() const { return mComplete; }

<<<<<<< HEAD
  GLuint FB() const {
    MOZ_GL_ASSERT(mGL, IsComplete());
    return mFB;
  }

 protected:
  void UnwrapImpl();
||||||| merged common ancestors
    GLuint FB() const {
        MOZ_ASSERT(IsComplete());
        return mFB;
    }

protected:
    void UnwrapImpl();
=======
  GLuint FB() const {
    MOZ_GL_ASSERT(mGL, IsComplete());
    return mFB;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ScopedFramebufferForRenderbuffer
    : public ScopedGLWrapper<ScopedFramebufferForRenderbuffer> {
  friend struct ScopedGLWrapper<ScopedFramebufferForRenderbuffer>;

 protected:
  bool mComplete;  // True if the framebuffer we create is complete.
  GLuint mFB;

 public:
  ScopedFramebufferForRenderbuffer(GLContext* aGL, GLuint aRB);

  bool IsComplete() const { return mComplete; }

  GLuint FB() const { return mFB; }

 protected:
  void UnwrapImpl();
||||||| merged common ancestors
struct ScopedFramebufferForRenderbuffer
    : public ScopedGLWrapper<ScopedFramebufferForRenderbuffer>
{
    friend struct ScopedGLWrapper<ScopedFramebufferForRenderbuffer>;

protected:
    bool mComplete; // True if the framebuffer we create is complete.
    GLuint mFB;

public:
    ScopedFramebufferForRenderbuffer(GLContext* aGL, GLuint aRB);

    bool IsComplete() const {
        return mComplete;
    }

    GLuint FB() const {
        return mFB;
    }

protected:
    void UnwrapImpl();
=======
struct ScopedFramebufferForRenderbuffer final {
 private:
  GLContext* const mGL;
  bool mComplete;  // True if the framebuffer we create is complete.
  GLuint mFB;

 public:
  ScopedFramebufferForRenderbuffer(GLContext* aGL, GLuint aRB);
  ~ScopedFramebufferForRenderbuffer();

  bool IsComplete() const { return mComplete; }
  GLuint FB() const {
    MOZ_GL_ASSERT(mGL, IsComplete());
    return mFB;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ScopedViewportRect : public ScopedGLWrapper<ScopedViewportRect> {
  friend struct ScopedGLWrapper<ScopedViewportRect>;

 protected:
  GLint mSavedViewportRect[4];
||||||| merged common ancestors
struct ScopedViewportRect
    : public ScopedGLWrapper<ScopedViewportRect>
{
    friend struct ScopedGLWrapper<ScopedViewportRect>;

protected:
    GLint mSavedViewportRect[4];
=======
struct ScopedViewportRect final {
 private:
  GLContext* const mGL;
  GLint mSavedViewportRect[4];
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  ScopedViewportRect(GLContext* aGL, GLint x, GLint y, GLsizei width,
                     GLsizei height);

 protected:
  void UnwrapImpl();
||||||| merged common ancestors
public:
    ScopedViewportRect(GLContext* aGL, GLint x, GLint y, GLsizei width, GLsizei height);

protected:
    void UnwrapImpl();
=======
 public:
  ScopedViewportRect(GLContext* aGL, GLint x, GLint y, GLsizei width,
                     GLsizei height);
  ~ScopedViewportRect();
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ScopedScissorRect : public ScopedGLWrapper<ScopedScissorRect> {
  friend struct ScopedGLWrapper<ScopedScissorRect>;

 protected:
  GLint mSavedScissorRect[4];

 public:
  ScopedScissorRect(GLContext* aGL, GLint x, GLint y, GLsizei width,
                    GLsizei height);
  explicit ScopedScissorRect(GLContext* aGL);
||||||| merged common ancestors
struct ScopedScissorRect
    : public ScopedGLWrapper<ScopedScissorRect>
{
    friend struct ScopedGLWrapper<ScopedScissorRect>;

protected:
    GLint mSavedScissorRect[4];

public:
    ScopedScissorRect(GLContext* aGL, GLint x, GLint y, GLsizei width, GLsizei height);
    explicit ScopedScissorRect(GLContext* aGL);
=======
struct ScopedScissorRect final {
 private:
  GLContext* const mGL;
  GLint mSavedScissorRect[4];
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  void UnwrapImpl();
||||||| merged common ancestors
protected:
    void UnwrapImpl();
=======
 public:
  ScopedScissorRect(GLContext* aGL, GLint x, GLint y, GLsizei width,
                    GLsizei height);
  explicit ScopedScissorRect(GLContext* aGL);
  ~ScopedScissorRect();
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ScopedVertexAttribPointer
    : public ScopedGLWrapper<ScopedVertexAttribPointer> {
  friend struct ScopedGLWrapper<ScopedVertexAttribPointer>;

 protected:
  GLuint mAttribIndex;
  GLint mAttribEnabled;
  GLint mAttribSize;
  GLint mAttribStride;
  GLint mAttribType;
  GLint mAttribNormalized;
  GLint mAttribBufferBinding;
  void* mAttribPointer;
  GLuint mBoundBuffer;

 public:
  ScopedVertexAttribPointer(GLContext* aGL, GLuint index, GLint size,
                            GLenum type, realGLboolean normalized,
                            GLsizei stride, GLuint buffer,
                            const GLvoid* pointer);
  explicit ScopedVertexAttribPointer(GLContext* aGL, GLuint index);

 protected:
  void WrapImpl(GLuint index);
  void UnwrapImpl();
||||||| merged common ancestors
struct ScopedVertexAttribPointer
    : public ScopedGLWrapper<ScopedVertexAttribPointer>
{
    friend struct ScopedGLWrapper<ScopedVertexAttribPointer>;

protected:
    GLuint mAttribIndex;
    GLint mAttribEnabled;
    GLint mAttribSize;
    GLint mAttribStride;
    GLint mAttribType;
    GLint mAttribNormalized;
    GLint mAttribBufferBinding;
    void* mAttribPointer;
    GLuint mBoundBuffer;

public:
    ScopedVertexAttribPointer(GLContext* aGL, GLuint index, GLint size, GLenum type, realGLboolean normalized,
                              GLsizei stride, GLuint buffer, const GLvoid* pointer);
    explicit ScopedVertexAttribPointer(GLContext* aGL, GLuint index);

protected:
    void WrapImpl(GLuint index);
    void UnwrapImpl();
=======
struct ScopedVertexAttribPointer final {
 private:
  GLContext* const mGL;
  GLuint mAttribIndex;
  GLint mAttribEnabled;
  GLint mAttribSize;
  GLint mAttribStride;
  GLint mAttribType;
  GLint mAttribNormalized;
  GLint mAttribBufferBinding;
  void* mAttribPointer;
  GLuint mBoundBuffer;

 public:
  ScopedVertexAttribPointer(GLContext* aGL, GLuint index, GLint size,
                            GLenum type, realGLboolean normalized,
                            GLsizei stride, GLuint buffer,
                            const GLvoid* pointer);
  explicit ScopedVertexAttribPointer(GLContext* aGL, GLuint index);
  ~ScopedVertexAttribPointer();

 private:
  void WrapImpl(GLuint index);
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ScopedPackState : public ScopedGLWrapper<ScopedPackState> {
  friend struct ScopedGLWrapper<ScopedPackState>;
||||||| merged common ancestors
struct ScopedPackState
    : public ScopedGLWrapper<ScopedPackState>
{
    friend struct ScopedGLWrapper<ScopedPackState>;
=======
struct ScopedPackState final {
 private:
  GLContext* const mGL;
  GLint mAlignment;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  GLint mAlignment;
||||||| merged common ancestors
protected:
    GLint mAlignment;
=======
  GLuint mPixelBuffer;
  GLint mRowLength;
  GLint mSkipPixels;
  GLint mSkipRows;
>>>>>>> upstream-releases

<<<<<<< HEAD
  GLuint mPixelBuffer;
  GLint mRowLength;
  GLint mSkipPixels;
  GLint mSkipRows;
||||||| merged common ancestors
    GLuint mPixelBuffer;
    GLint mRowLength;
    GLint mSkipPixels;
    GLint mSkipRows;
=======
 public:
  explicit ScopedPackState(GLContext* gl);
  ~ScopedPackState();
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  explicit ScopedPackState(GLContext* gl);

 protected:
  void UnwrapImpl();
||||||| merged common ancestors
public:
    explicit ScopedPackState(GLContext* gl);

protected:
    void UnwrapImpl();
=======
  // Returns whether the stride was handled successfully.
  bool SetForWidthAndStrideRGBA(GLsizei aWidth, GLsizei aStride);
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ResetUnpackState : public ScopedGLWrapper<ResetUnpackState> {
  friend struct ScopedGLWrapper<ResetUnpackState>;

 protected:
  GLuint mAlignment;

  GLuint mPBO;
  GLuint mRowLength;
  GLuint mImageHeight;
  GLuint mSkipPixels;
  GLuint mSkipRows;
  GLuint mSkipImages;

 public:
  explicit ResetUnpackState(GLContext* gl);

 protected:
  void UnwrapImpl();
||||||| merged common ancestors
struct ResetUnpackState
    : public ScopedGLWrapper<ResetUnpackState>
{
    friend struct ScopedGLWrapper<ResetUnpackState>;

protected:
    GLuint mAlignment;

    GLuint mPBO;
    GLuint mRowLength;
    GLuint mImageHeight;
    GLuint mSkipPixels;
    GLuint mSkipRows;
    GLuint mSkipImages;

public:
    explicit ResetUnpackState(GLContext* gl);

protected:
    void UnwrapImpl();
=======
struct ResetUnpackState final {
 private:
  GLContext* const mGL;
  GLuint mAlignment;

  GLuint mPBO;
  GLuint mRowLength;
  GLuint mImageHeight;
  GLuint mSkipPixels;
  GLuint mSkipRows;
  GLuint mSkipImages;

 public:
  explicit ResetUnpackState(GLContext* gl);
  ~ResetUnpackState();
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct ScopedBindPBO final : public ScopedGLWrapper<ScopedBindPBO> {
  friend struct ScopedGLWrapper<ScopedBindPBO>;

 protected:
  const GLenum mTarget;
  const GLuint mPBO;

 public:
  ScopedBindPBO(GLContext* gl, GLenum target);
||||||| merged common ancestors
struct ScopedBindPBO final
    : public ScopedGLWrapper<ScopedBindPBO>
{
    friend struct ScopedGLWrapper<ScopedBindPBO>;

protected:
    const GLenum mTarget;
    const GLuint mPBO;

public:
    ScopedBindPBO(GLContext* gl, GLenum target);
=======
struct ScopedBindPBO final {
 private:
  GLContext* const mGL;
  const GLenum mTarget;
  const GLuint mPBO;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  void UnwrapImpl();
||||||| merged common ancestors
protected:
    void UnwrapImpl();
=======
 public:
  ScopedBindPBO(GLContext* gl, GLenum target);
  ~ScopedBindPBO();
>>>>>>> upstream-releases
};

} /* namespace gl */
} /* namespace mozilla */

#endif /* SCOPEDGLHELPERS_H_ */
