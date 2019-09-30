/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/UniquePtr.h"

#include "GLContext.h"
#include "ScopedGLHelpers.h"

namespace mozilla {
namespace gl {

/* ScopedGLState - Wraps glEnable/glDisable. **********************************/

// Use |newState = true| to enable, |false| to disable.
ScopedGLState::ScopedGLState(GLContext* aGL, GLenum aCapability, bool aNewState)
<<<<<<< HEAD
    : ScopedGLWrapper<ScopedGLState>(aGL), mCapability(aCapability) {
  mOldState = mGL->fIsEnabled(mCapability);

  // Early out if we're already in the right state.
  if (aNewState == mOldState) return;

  if (aNewState) {
    mGL->fEnable(mCapability);
  } else {
    mGL->fDisable(mCapability);
  }
||||||| merged common ancestors
    : ScopedGLWrapper<ScopedGLState>(aGL)
    , mCapability(aCapability)
{
    mOldState = mGL->fIsEnabled(mCapability);

    // Early out if we're already in the right state.
    if (aNewState == mOldState)
        return;

    if (aNewState) {
        mGL->fEnable(mCapability);
    } else {
        mGL->fDisable(mCapability);
    }
=======
    : mGL(aGL), mCapability(aCapability) {
  mOldState = mGL->fIsEnabled(mCapability);

  // Early out if we're already in the right state.
  if (aNewState == mOldState) return;

  if (aNewState) {
    mGL->fEnable(mCapability);
  } else {
    mGL->fDisable(mCapability);
  }
>>>>>>> upstream-releases
}

ScopedGLState::ScopedGLState(GLContext* aGL, GLenum aCapability)
<<<<<<< HEAD
    : ScopedGLWrapper<ScopedGLState>(aGL), mCapability(aCapability) {
  mOldState = mGL->fIsEnabled(mCapability);
||||||| merged common ancestors
    : ScopedGLWrapper<ScopedGLState>(aGL)
    , mCapability(aCapability)
{
    mOldState = mGL->fIsEnabled(mCapability);
}

void
ScopedGLState::UnwrapImpl()
{
    if (mOldState) {
        mGL->fEnable(mCapability);
    } else {
        mGL->fDisable(mCapability);
    }
=======
    : mGL(aGL), mCapability(aCapability) {
  mOldState = mGL->fIsEnabled(mCapability);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void ScopedGLState::UnwrapImpl() {
  if (mOldState) {
    mGL->fEnable(mCapability);
  } else {
    mGL->fDisable(mCapability);
  }
}
||||||| merged common ancestors
=======
ScopedGLState::~ScopedGLState() {
  if (mOldState) {
    mGL->fEnable(mCapability);
  } else {
    mGL->fDisable(mCapability);
  }
}
>>>>>>> upstream-releases

/* ScopedBindFramebuffer - Saves and restores with GetUserBoundFB and
 * BindUserFB. */

void ScopedBindFramebuffer::Init() {
  if (mGL->IsSupported(GLFeature::split_framebuffer)) {
    mOldReadFB = mGL->GetReadFB();
    mOldDrawFB = mGL->GetDrawFB();
  } else {
    mOldReadFB = mOldDrawFB = mGL->GetFB();
  }
}

<<<<<<< HEAD
ScopedBindFramebuffer::ScopedBindFramebuffer(GLContext* aGL)
    : ScopedGLWrapper<ScopedBindFramebuffer>(aGL) {
  Init();
||||||| merged common ancestors
ScopedBindFramebuffer::ScopedBindFramebuffer(GLContext* aGL)
    : ScopedGLWrapper<ScopedBindFramebuffer>(aGL)
{
    Init();
=======
ScopedBindFramebuffer::ScopedBindFramebuffer(GLContext* aGL) : mGL(aGL) {
  Init();
>>>>>>> upstream-releases
}

ScopedBindFramebuffer::ScopedBindFramebuffer(GLContext* aGL, GLuint aNewFB)
<<<<<<< HEAD
    : ScopedGLWrapper<ScopedBindFramebuffer>(aGL) {
  Init();
  mGL->BindFB(aNewFB);
||||||| merged common ancestors
    : ScopedGLWrapper<ScopedBindFramebuffer>(aGL)
{
    Init();
    mGL->BindFB(aNewFB);
}

void
ScopedBindFramebuffer::UnwrapImpl()
{
    if (mOldReadFB == mOldDrawFB) {
        mGL->BindFB(mOldDrawFB);
    } else {
        mGL->BindDrawFB(mOldDrawFB);
        mGL->BindReadFB(mOldReadFB);
    }
=======
    : mGL(aGL) {
  Init();
  mGL->BindFB(aNewFB);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void ScopedBindFramebuffer::UnwrapImpl() {
  if (mOldReadFB == mOldDrawFB) {
    mGL->BindFB(mOldDrawFB);
  } else {
    mGL->BindDrawFB(mOldDrawFB);
    mGL->BindReadFB(mOldReadFB);
  }
}
||||||| merged common ancestors
=======
ScopedBindFramebuffer::~ScopedBindFramebuffer() {
  if (mOldReadFB == mOldDrawFB) {
    mGL->BindFB(mOldDrawFB);
  } else {
    mGL->BindDrawFB(mOldDrawFB);
    mGL->BindReadFB(mOldReadFB);
  }
}
>>>>>>> upstream-releases

/* ScopedBindTextureUnit ******************************************************/

ScopedBindTextureUnit::ScopedBindTextureUnit(GLContext* aGL, GLenum aTexUnit)
<<<<<<< HEAD
    : ScopedGLWrapper<ScopedBindTextureUnit>(aGL), mOldTexUnit(0) {
  MOZ_ASSERT(aTexUnit >= LOCAL_GL_TEXTURE0);
  mGL->GetUIntegerv(LOCAL_GL_ACTIVE_TEXTURE, &mOldTexUnit);
  mGL->fActiveTexture(aTexUnit);
||||||| merged common ancestors
    : ScopedGLWrapper<ScopedBindTextureUnit>(aGL)
    , mOldTexUnit(0)
{
    MOZ_ASSERT(aTexUnit >= LOCAL_GL_TEXTURE0);
    mGL->GetUIntegerv(LOCAL_GL_ACTIVE_TEXTURE, &mOldTexUnit);
    mGL->fActiveTexture(aTexUnit);
}

void
ScopedBindTextureUnit::UnwrapImpl() {
    mGL->fActiveTexture(mOldTexUnit);
=======
    : mGL(aGL), mOldTexUnit(0) {
  MOZ_ASSERT(aTexUnit >= LOCAL_GL_TEXTURE0);
  mGL->GetUIntegerv(LOCAL_GL_ACTIVE_TEXTURE, &mOldTexUnit);
  mGL->fActiveTexture(aTexUnit);
}

ScopedBindTextureUnit::~ScopedBindTextureUnit() {
  mGL->fActiveTexture(mOldTexUnit);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void ScopedBindTextureUnit::UnwrapImpl() { mGL->fActiveTexture(mOldTexUnit); }

||||||| merged common ancestors

=======
>>>>>>> upstream-releases
/* ScopedTexture **************************************************************/

<<<<<<< HEAD
ScopedTexture::ScopedTexture(GLContext* aGL)
    : ScopedGLWrapper<ScopedTexture>(aGL), mTexture(0) {
  mGL->fGenTextures(1, &mTexture);
}

void ScopedTexture::UnwrapImpl() { mGL->fDeleteTextures(1, &mTexture); }
||||||| merged common ancestors
ScopedTexture::ScopedTexture(GLContext* aGL)
    : ScopedGLWrapper<ScopedTexture>(aGL)
    , mTexture(0)
{
    mGL->fGenTextures(1, &mTexture);
}

void
ScopedTexture::UnwrapImpl()
{
    mGL->fDeleteTextures(1, &mTexture);
}
=======
ScopedTexture::ScopedTexture(GLContext* aGL) : mGL(aGL), mTexture(0) {
  mGL->fGenTextures(1, &mTexture);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
/* ScopedFramebuffer
 * **************************************************************/
||||||| merged common ancestors
/* ScopedFramebuffer **************************************************************/
=======
ScopedTexture::~ScopedTexture() { mGL->fDeleteTextures(1, &mTexture); }
>>>>>>> upstream-releases

<<<<<<< HEAD
ScopedFramebuffer::ScopedFramebuffer(GLContext* aGL)
    : ScopedGLWrapper<ScopedFramebuffer>(aGL), mFB(0) {
  mGL->fGenFramebuffers(1, &mFB);
||||||| merged common ancestors
ScopedFramebuffer::ScopedFramebuffer(GLContext* aGL)
    : ScopedGLWrapper<ScopedFramebuffer>(aGL)
    , mFB(0)
{
    mGL->fGenFramebuffers(1, &mFB);
}

void
ScopedFramebuffer::UnwrapImpl()
{
    mGL->fDeleteFramebuffers(1, &mFB);
=======
/* ScopedFramebuffer
 * **************************************************************/

ScopedFramebuffer::ScopedFramebuffer(GLContext* aGL) : mGL(aGL), mFB(0) {
  mGL->fGenFramebuffers(1, &mFB);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void ScopedFramebuffer::UnwrapImpl() { mGL->fDeleteFramebuffers(1, &mFB); }
||||||| merged common ancestors
=======
ScopedFramebuffer::~ScopedFramebuffer() { mGL->fDeleteFramebuffers(1, &mFB); }
>>>>>>> upstream-releases

/* ScopedRenderbuffer
 * **************************************************************/

<<<<<<< HEAD
ScopedRenderbuffer::ScopedRenderbuffer(GLContext* aGL)
    : ScopedGLWrapper<ScopedRenderbuffer>(aGL), mRB(0) {
  mGL->fGenRenderbuffers(1, &mRB);
||||||| merged common ancestors
ScopedRenderbuffer::ScopedRenderbuffer(GLContext* aGL)
    : ScopedGLWrapper<ScopedRenderbuffer>(aGL)
    , mRB(0)
{
    mGL->fGenRenderbuffers(1, &mRB);
=======
ScopedRenderbuffer::ScopedRenderbuffer(GLContext* aGL) : mGL(aGL), mRB(0) {
  mGL->fGenRenderbuffers(1, &mRB);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void ScopedRenderbuffer::UnwrapImpl() { mGL->fDeleteRenderbuffers(1, &mRB); }
||||||| merged common ancestors
void
ScopedRenderbuffer::UnwrapImpl()
{
    mGL->fDeleteRenderbuffers(1, &mRB);
}
=======
ScopedRenderbuffer::~ScopedRenderbuffer() {
  mGL->fDeleteRenderbuffers(1, &mRB);
}
>>>>>>> upstream-releases

/* ScopedBindTexture **********************************************************/

static GLuint GetBoundTexture(GLContext* gl, GLenum texTarget) {
  GLenum bindingTarget;
  switch (texTarget) {
    case LOCAL_GL_TEXTURE_2D:
      bindingTarget = LOCAL_GL_TEXTURE_BINDING_2D;
      break;

    case LOCAL_GL_TEXTURE_CUBE_MAP:
      bindingTarget = LOCAL_GL_TEXTURE_BINDING_CUBE_MAP;
      break;

    case LOCAL_GL_TEXTURE_3D:
      bindingTarget = LOCAL_GL_TEXTURE_BINDING_3D;
      break;

    case LOCAL_GL_TEXTURE_2D_ARRAY:
      bindingTarget = LOCAL_GL_TEXTURE_BINDING_2D_ARRAY;
      break;

    case LOCAL_GL_TEXTURE_RECTANGLE_ARB:
      bindingTarget = LOCAL_GL_TEXTURE_BINDING_RECTANGLE_ARB;
      break;

    case LOCAL_GL_TEXTURE_EXTERNAL:
      bindingTarget = LOCAL_GL_TEXTURE_BINDING_EXTERNAL;
      break;

    default:
<<<<<<< HEAD
      MOZ_CRASH("bad texTarget");
  }

  GLuint ret = 0;
  gl->GetUIntegerv(bindingTarget, &ret);
  return ret;
}
||||||| merged common ancestors
        MOZ_CRASH("bad texTarget");
    }

    GLuint ret = 0;
    gl->GetUIntegerv(bindingTarget, &ret);
    return ret;
}
=======
      MOZ_CRASH("bad texTarget");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
ScopedBindTexture::ScopedBindTexture(GLContext* aGL, GLuint aNewTex,
                                     GLenum aTarget)
    : ScopedGLWrapper<ScopedBindTexture>(aGL),
      mTarget(aTarget),
      mOldTex(GetBoundTexture(aGL, aTarget)) {
  mGL->fBindTexture(mTarget, aNewTex);
||||||| merged common ancestors
ScopedBindTexture::ScopedBindTexture(GLContext* aGL, GLuint aNewTex, GLenum aTarget)
        : ScopedGLWrapper<ScopedBindTexture>(aGL)
        , mTarget(aTarget)
        , mOldTex(GetBoundTexture(aGL, aTarget))
{
    mGL->fBindTexture(mTarget, aNewTex);
}

void
ScopedBindTexture::UnwrapImpl()
{
    mGL->fBindTexture(mTarget, mOldTex);
=======
  GLuint ret = 0;
  gl->GetUIntegerv(bindingTarget, &ret);
  return ret;
}

ScopedBindTexture::ScopedBindTexture(GLContext* aGL, GLuint aNewTex,
                                     GLenum aTarget)
    : mGL(aGL), mTarget(aTarget), mOldTex(GetBoundTexture(aGL, aTarget)) {
  mGL->fBindTexture(mTarget, aNewTex);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void ScopedBindTexture::UnwrapImpl() { mGL->fBindTexture(mTarget, mOldTex); }
||||||| merged common ancestors
=======
ScopedBindTexture::~ScopedBindTexture() { mGL->fBindTexture(mTarget, mOldTex); }
>>>>>>> upstream-releases

/* ScopedBindRenderbuffer *****************************************************/

void ScopedBindRenderbuffer::Init() {
  mOldRB = 0;
  mGL->GetUIntegerv(LOCAL_GL_RENDERBUFFER_BINDING, &mOldRB);
}

<<<<<<< HEAD
ScopedBindRenderbuffer::ScopedBindRenderbuffer(GLContext* aGL)
    : ScopedGLWrapper<ScopedBindRenderbuffer>(aGL) {
  Init();
||||||| merged common ancestors
ScopedBindRenderbuffer::ScopedBindRenderbuffer(GLContext* aGL)
        : ScopedGLWrapper<ScopedBindRenderbuffer>(aGL)
{
    Init();
=======
ScopedBindRenderbuffer::ScopedBindRenderbuffer(GLContext* aGL) : mGL(aGL) {
  Init();
>>>>>>> upstream-releases
}

ScopedBindRenderbuffer::ScopedBindRenderbuffer(GLContext* aGL, GLuint aNewRB)
<<<<<<< HEAD
    : ScopedGLWrapper<ScopedBindRenderbuffer>(aGL) {
  Init();
  mGL->fBindRenderbuffer(LOCAL_GL_RENDERBUFFER, aNewRB);
||||||| merged common ancestors
    : ScopedGLWrapper<ScopedBindRenderbuffer>(aGL)
{
    Init();
    mGL->fBindRenderbuffer(LOCAL_GL_RENDERBUFFER, aNewRB);
=======
    : mGL(aGL) {
  Init();
  mGL->fBindRenderbuffer(LOCAL_GL_RENDERBUFFER, aNewRB);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void ScopedBindRenderbuffer::UnwrapImpl() {
  mGL->fBindRenderbuffer(LOCAL_GL_RENDERBUFFER, mOldRB);
||||||| merged common ancestors
void
ScopedBindRenderbuffer::UnwrapImpl()
{
    mGL->fBindRenderbuffer(LOCAL_GL_RENDERBUFFER, mOldRB);
=======
ScopedBindRenderbuffer::~ScopedBindRenderbuffer() {
  mGL->fBindRenderbuffer(LOCAL_GL_RENDERBUFFER, mOldRB);
>>>>>>> upstream-releases
}

/* ScopedFramebufferForTexture ************************************************/
ScopedFramebufferForTexture::ScopedFramebufferForTexture(GLContext* aGL,
                                                         GLuint aTexture,
                                                         GLenum aTarget)
<<<<<<< HEAD
    : ScopedGLWrapper<ScopedFramebufferForTexture>(aGL),
      mComplete(false),
      mFB(0) {
  mGL->fGenFramebuffers(1, &mFB);
  ScopedBindFramebuffer autoFB(aGL, mFB);
  mGL->fFramebufferTexture2D(LOCAL_GL_FRAMEBUFFER, LOCAL_GL_COLOR_ATTACHMENT0,
                             aTarget, aTexture, 0);

  GLenum status = mGL->fCheckFramebufferStatus(LOCAL_GL_FRAMEBUFFER);
  if (status == LOCAL_GL_FRAMEBUFFER_COMPLETE) {
    mComplete = true;
  } else {
||||||| merged common ancestors
    : ScopedGLWrapper<ScopedFramebufferForTexture>(aGL)
    , mComplete(false)
    , mFB(0)
{
    mGL->fGenFramebuffers(1, &mFB);
    ScopedBindFramebuffer autoFB(aGL, mFB);
    mGL->fFramebufferTexture2D(LOCAL_GL_FRAMEBUFFER,
                               LOCAL_GL_COLOR_ATTACHMENT0,
                               aTarget,
                               aTexture,
                               0);

    GLenum status = mGL->fCheckFramebufferStatus(LOCAL_GL_FRAMEBUFFER);
    if (status == LOCAL_GL_FRAMEBUFFER_COMPLETE) {
        mComplete = true;
    } else {
        mGL->fDeleteFramebuffers(1, &mFB);
        mFB = 0;
    }
}

void ScopedFramebufferForTexture::UnwrapImpl()
{
    if (!mFB)
        return;

=======
    : mGL(aGL), mComplete(false), mFB(0) {
  mGL->fGenFramebuffers(1, &mFB);
  ScopedBindFramebuffer autoFB(aGL, mFB);
  mGL->fFramebufferTexture2D(LOCAL_GL_FRAMEBUFFER, LOCAL_GL_COLOR_ATTACHMENT0,
                             aTarget, aTexture, 0);

  GLenum status = mGL->fCheckFramebufferStatus(LOCAL_GL_FRAMEBUFFER);
  if (status == LOCAL_GL_FRAMEBUFFER_COMPLETE) {
    mComplete = true;
  } else {
>>>>>>> upstream-releases
    mGL->fDeleteFramebuffers(1, &mFB);
    mFB = 0;
  }
}

<<<<<<< HEAD
void ScopedFramebufferForTexture::UnwrapImpl() {
  if (!mFB) return;
||||||| merged common ancestors
=======
ScopedFramebufferForTexture::~ScopedFramebufferForTexture() {
  if (!mFB) return;
>>>>>>> upstream-releases

  mGL->fDeleteFramebuffers(1, &mFB);
  mFB = 0;
}

/* ScopedFramebufferForRenderbuffer *******************************************/

<<<<<<< HEAD
ScopedFramebufferForRenderbuffer::ScopedFramebufferForRenderbuffer(
    GLContext* aGL, GLuint aRB)
    : ScopedGLWrapper<ScopedFramebufferForRenderbuffer>(aGL),
      mComplete(false),
      mFB(0) {
  mGL->fGenFramebuffers(1, &mFB);
  ScopedBindFramebuffer autoFB(aGL, mFB);
  mGL->fFramebufferRenderbuffer(LOCAL_GL_FRAMEBUFFER,
                                LOCAL_GL_COLOR_ATTACHMENT0,
                                LOCAL_GL_RENDERBUFFER, aRB);

  GLenum status = mGL->fCheckFramebufferStatus(LOCAL_GL_FRAMEBUFFER);
  if (status == LOCAL_GL_FRAMEBUFFER_COMPLETE) {
    mComplete = true;
  } else {
||||||| merged common ancestors
=======
ScopedFramebufferForRenderbuffer::ScopedFramebufferForRenderbuffer(
    GLContext* aGL, GLuint aRB)
    : mGL(aGL), mComplete(false), mFB(0) {
  mGL->fGenFramebuffers(1, &mFB);
  ScopedBindFramebuffer autoFB(aGL, mFB);
  mGL->fFramebufferRenderbuffer(LOCAL_GL_FRAMEBUFFER,
                                LOCAL_GL_COLOR_ATTACHMENT0,
                                LOCAL_GL_RENDERBUFFER, aRB);

  GLenum status = mGL->fCheckFramebufferStatus(LOCAL_GL_FRAMEBUFFER);
  if (status == LOCAL_GL_FRAMEBUFFER_COMPLETE) {
    mComplete = true;
  } else {
>>>>>>> upstream-releases
    mGL->fDeleteFramebuffers(1, &mFB);
    mFB = 0;
<<<<<<< HEAD
  }
}

void ScopedFramebufferForRenderbuffer::UnwrapImpl() {
  if (!mFB) return;

  mGL->fDeleteFramebuffers(1, &mFB);
  mFB = 0;
||||||| merged common ancestors
=======
  }
}

ScopedFramebufferForRenderbuffer::~ScopedFramebufferForRenderbuffer() {
  if (!mFB) return;

  mGL->fDeleteFramebuffers(1, &mFB);
  mFB = 0;
>>>>>>> upstream-releases
}

/* ScopedViewportRect *********************************************************/

ScopedViewportRect::ScopedViewportRect(GLContext* aGL, GLint x, GLint y,
                                       GLsizei width, GLsizei height)
<<<<<<< HEAD
    : ScopedGLWrapper<ScopedViewportRect>(aGL) {
  mGL->fGetIntegerv(LOCAL_GL_VIEWPORT, mSavedViewportRect);
  mGL->fViewport(x, y, width, height);
||||||| merged common ancestors
  : ScopedGLWrapper<ScopedViewportRect>(aGL)
{
    mGL->fGetIntegerv(LOCAL_GL_VIEWPORT, mSavedViewportRect);
    mGL->fViewport(x, y, width, height);
=======
    : mGL(aGL) {
  mGL->fGetIntegerv(LOCAL_GL_VIEWPORT, mSavedViewportRect);
  mGL->fViewport(x, y, width, height);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void ScopedViewportRect::UnwrapImpl() {
  mGL->fViewport(mSavedViewportRect[0], mSavedViewportRect[1],
                 mSavedViewportRect[2], mSavedViewportRect[3]);
||||||| merged common ancestors
void ScopedViewportRect::UnwrapImpl()
{
    mGL->fViewport(mSavedViewportRect[0],
                   mSavedViewportRect[1],
                   mSavedViewportRect[2],
                   mSavedViewportRect[3]);
=======
ScopedViewportRect::~ScopedViewportRect() {
  mGL->fViewport(mSavedViewportRect[0], mSavedViewportRect[1],
                 mSavedViewportRect[2], mSavedViewportRect[3]);
>>>>>>> upstream-releases
}

/* ScopedScissorRect **********************************************************/

ScopedScissorRect::ScopedScissorRect(GLContext* aGL, GLint x, GLint y,
                                     GLsizei width, GLsizei height)
<<<<<<< HEAD
    : ScopedGLWrapper<ScopedScissorRect>(aGL) {
  mGL->fGetIntegerv(LOCAL_GL_SCISSOR_BOX, mSavedScissorRect);
  mGL->fScissor(x, y, width, height);
||||||| merged common ancestors
  : ScopedGLWrapper<ScopedScissorRect>(aGL)
{
    mGL->fGetIntegerv(LOCAL_GL_SCISSOR_BOX, mSavedScissorRect);
    mGL->fScissor(x, y, width, height);
=======
    : mGL(aGL) {
  mGL->fGetIntegerv(LOCAL_GL_SCISSOR_BOX, mSavedScissorRect);
  mGL->fScissor(x, y, width, height);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
ScopedScissorRect::ScopedScissorRect(GLContext* aGL)
    : ScopedGLWrapper<ScopedScissorRect>(aGL) {
  mGL->fGetIntegerv(LOCAL_GL_SCISSOR_BOX, mSavedScissorRect);
||||||| merged common ancestors
ScopedScissorRect::ScopedScissorRect(GLContext* aGL)
  : ScopedGLWrapper<ScopedScissorRect>(aGL)
{
    mGL->fGetIntegerv(LOCAL_GL_SCISSOR_BOX, mSavedScissorRect);
=======
ScopedScissorRect::ScopedScissorRect(GLContext* aGL) : mGL(aGL) {
  mGL->fGetIntegerv(LOCAL_GL_SCISSOR_BOX, mSavedScissorRect);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void ScopedScissorRect::UnwrapImpl() {
  mGL->fScissor(mSavedScissorRect[0], mSavedScissorRect[1],
                mSavedScissorRect[2], mSavedScissorRect[3]);
||||||| merged common ancestors
void ScopedScissorRect::UnwrapImpl()
{
    mGL->fScissor(mSavedScissorRect[0],
                  mSavedScissorRect[1],
                  mSavedScissorRect[2],
                  mSavedScissorRect[3]);
=======
ScopedScissorRect::~ScopedScissorRect() {
  mGL->fScissor(mSavedScissorRect[0], mSavedScissorRect[1],
                mSavedScissorRect[2], mSavedScissorRect[3]);
>>>>>>> upstream-releases
}

/* ScopedVertexAttribPointer **************************************************/

<<<<<<< HEAD
ScopedVertexAttribPointer::ScopedVertexAttribPointer(
    GLContext* aGL, GLuint index, GLint size, GLenum type,
    realGLboolean normalized, GLsizei stride, GLuint buffer,
    const GLvoid* pointer)
    : ScopedGLWrapper<ScopedVertexAttribPointer>(aGL),
      mAttribEnabled(0),
      mAttribSize(0),
      mAttribStride(0),
      mAttribType(0),
      mAttribNormalized(0),
      mAttribBufferBinding(0),
      mAttribPointer(nullptr),
      mBoundBuffer(0) {
  WrapImpl(index);
  mGL->fBindBuffer(LOCAL_GL_ARRAY_BUFFER, buffer);
  mGL->fVertexAttribPointer(index, size, type, normalized, stride, pointer);
  mGL->fEnableVertexAttribArray(index);
||||||| merged common ancestors
ScopedVertexAttribPointer::ScopedVertexAttribPointer(GLContext* aGL,
                                                     GLuint index,
                                                     GLint size,
                                                     GLenum type,
                                                     realGLboolean normalized,
                                                     GLsizei stride,
                                                     GLuint buffer,
                                                     const GLvoid* pointer)
    : ScopedGLWrapper<ScopedVertexAttribPointer>(aGL)
    , mAttribEnabled(0)
    , mAttribSize(0)
    , mAttribStride(0)
    , mAttribType(0)
    , mAttribNormalized(0)
    , mAttribBufferBinding(0)
    , mAttribPointer(nullptr)
    , mBoundBuffer(0)
{
    WrapImpl(index);
    mGL->fBindBuffer(LOCAL_GL_ARRAY_BUFFER, buffer);
    mGL->fVertexAttribPointer(index, size, type, normalized, stride, pointer);
    mGL->fEnableVertexAttribArray(index);
=======
ScopedVertexAttribPointer::ScopedVertexAttribPointer(
    GLContext* aGL, GLuint index, GLint size, GLenum type,
    realGLboolean normalized, GLsizei stride, GLuint buffer,
    const GLvoid* pointer)
    : mGL(aGL),
      mAttribEnabled(0),
      mAttribSize(0),
      mAttribStride(0),
      mAttribType(0),
      mAttribNormalized(0),
      mAttribBufferBinding(0),
      mAttribPointer(nullptr),
      mBoundBuffer(0) {
  WrapImpl(index);
  mGL->fBindBuffer(LOCAL_GL_ARRAY_BUFFER, buffer);
  mGL->fVertexAttribPointer(index, size, type, normalized, stride, pointer);
  mGL->fEnableVertexAttribArray(index);
>>>>>>> upstream-releases
}

ScopedVertexAttribPointer::ScopedVertexAttribPointer(GLContext* aGL,
                                                     GLuint index)
<<<<<<< HEAD
    : ScopedGLWrapper<ScopedVertexAttribPointer>(aGL),
      mAttribEnabled(0),
      mAttribSize(0),
      mAttribStride(0),
      mAttribType(0),
      mAttribNormalized(0),
      mAttribBufferBinding(0),
      mAttribPointer(nullptr),
      mBoundBuffer(0) {
  WrapImpl(index);
}

void ScopedVertexAttribPointer::WrapImpl(GLuint index) {
  mAttribIndex = index;

  /*
   * mGL->fGetVertexAttribiv takes:
   *  VERTEX_ATTRIB_ARRAY_ENABLED
   *  VERTEX_ATTRIB_ARRAY_SIZE,
   *  VERTEX_ATTRIB_ARRAY_STRIDE,
   *  VERTEX_ATTRIB_ARRAY_TYPE,
   *  VERTEX_ATTRIB_ARRAY_NORMALIZED,
   *  VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
   *  CURRENT_VERTEX_ATTRIB
   *
   * CURRENT_VERTEX_ATTRIB is vertex shader state. \o/
   * Others appear to be vertex array state,
   * or alternatively in the internal vertex array state
   * for a buffer object.
   */

  mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_ENABLED,
                          &mAttribEnabled);
  mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_SIZE,
                          &mAttribSize);
  mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_STRIDE,
                          &mAttribStride);
  mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_TYPE,
                          &mAttribType);
  mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
                          &mAttribNormalized);
  mGL->fGetVertexAttribiv(mAttribIndex,
                          LOCAL_GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                          &mAttribBufferBinding);
  mGL->fGetVertexAttribPointerv(
      mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_POINTER, &mAttribPointer);

  // Note that uniform values are program state, so we don't need to rebind
  // those.

  mGL->GetUIntegerv(LOCAL_GL_ARRAY_BUFFER_BINDING, &mBoundBuffer);
}

void ScopedVertexAttribPointer::UnwrapImpl() {
  mGL->fBindBuffer(LOCAL_GL_ARRAY_BUFFER, mAttribBufferBinding);
  mGL->fVertexAttribPointer(mAttribIndex, mAttribSize, mAttribType,
                            mAttribNormalized, mAttribStride, mAttribPointer);
  if (mAttribEnabled)
    mGL->fEnableVertexAttribArray(mAttribIndex);
  else
    mGL->fDisableVertexAttribArray(mAttribIndex);
  mGL->fBindBuffer(LOCAL_GL_ARRAY_BUFFER, mBoundBuffer);
||||||| merged common ancestors
    : ScopedGLWrapper<ScopedVertexAttribPointer>(aGL)
    , mAttribEnabled(0)
    , mAttribSize(0)
    , mAttribStride(0)
    , mAttribType(0)
    , mAttribNormalized(0)
    , mAttribBufferBinding(0)
    , mAttribPointer(nullptr)
    , mBoundBuffer(0)
{
    WrapImpl(index);
}

void
ScopedVertexAttribPointer::WrapImpl(GLuint index)
{
    mAttribIndex = index;

    /*
     * mGL->fGetVertexAttribiv takes:
     *  VERTEX_ATTRIB_ARRAY_ENABLED
     *  VERTEX_ATTRIB_ARRAY_SIZE,
     *  VERTEX_ATTRIB_ARRAY_STRIDE,
     *  VERTEX_ATTRIB_ARRAY_TYPE,
     *  VERTEX_ATTRIB_ARRAY_NORMALIZED,
     *  VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
     *  CURRENT_VERTEX_ATTRIB
     *
     * CURRENT_VERTEX_ATTRIB is vertex shader state. \o/
     * Others appear to be vertex array state,
     * or alternatively in the internal vertex array state
     * for a buffer object.
    */

    mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_ENABLED, &mAttribEnabled);
    mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_SIZE, &mAttribSize);
    mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_STRIDE, &mAttribStride);
    mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_TYPE, &mAttribType);
    mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &mAttribNormalized);
    mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &mAttribBufferBinding);
    mGL->fGetVertexAttribPointerv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_POINTER, &mAttribPointer);

    // Note that uniform values are program state, so we don't need to rebind those.

    mGL->GetUIntegerv(LOCAL_GL_ARRAY_BUFFER_BINDING, &mBoundBuffer);
}

void
ScopedVertexAttribPointer::UnwrapImpl()
{
    mGL->fBindBuffer(LOCAL_GL_ARRAY_BUFFER, mAttribBufferBinding);
    mGL->fVertexAttribPointer(mAttribIndex, mAttribSize, mAttribType, mAttribNormalized, mAttribStride, mAttribPointer);
    if (mAttribEnabled)
        mGL->fEnableVertexAttribArray(mAttribIndex);
    else
        mGL->fDisableVertexAttribArray(mAttribIndex);
    mGL->fBindBuffer(LOCAL_GL_ARRAY_BUFFER, mBoundBuffer);
=======
    : mGL(aGL),
      mAttribEnabled(0),
      mAttribSize(0),
      mAttribStride(0),
      mAttribType(0),
      mAttribNormalized(0),
      mAttribBufferBinding(0),
      mAttribPointer(nullptr),
      mBoundBuffer(0) {
  WrapImpl(index);
}

void ScopedVertexAttribPointer::WrapImpl(GLuint index) {
  mAttribIndex = index;

  /*
   * mGL->fGetVertexAttribiv takes:
   *  VERTEX_ATTRIB_ARRAY_ENABLED
   *  VERTEX_ATTRIB_ARRAY_SIZE,
   *  VERTEX_ATTRIB_ARRAY_STRIDE,
   *  VERTEX_ATTRIB_ARRAY_TYPE,
   *  VERTEX_ATTRIB_ARRAY_NORMALIZED,
   *  VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
   *  CURRENT_VERTEX_ATTRIB
   *
   * CURRENT_VERTEX_ATTRIB is vertex shader state. \o/
   * Others appear to be vertex array state,
   * or alternatively in the internal vertex array state
   * for a buffer object.
   */

  mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_ENABLED,
                          &mAttribEnabled);
  mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_SIZE,
                          &mAttribSize);
  mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_STRIDE,
                          &mAttribStride);
  mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_TYPE,
                          &mAttribType);
  mGL->fGetVertexAttribiv(mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
                          &mAttribNormalized);
  mGL->fGetVertexAttribiv(mAttribIndex,
                          LOCAL_GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                          &mAttribBufferBinding);
  mGL->fGetVertexAttribPointerv(
      mAttribIndex, LOCAL_GL_VERTEX_ATTRIB_ARRAY_POINTER, &mAttribPointer);

  // Note that uniform values are program state, so we don't need to rebind
  // those.

  mGL->GetUIntegerv(LOCAL_GL_ARRAY_BUFFER_BINDING, &mBoundBuffer);
}

ScopedVertexAttribPointer::~ScopedVertexAttribPointer() {
  mGL->fBindBuffer(LOCAL_GL_ARRAY_BUFFER, mAttribBufferBinding);
  mGL->fVertexAttribPointer(mAttribIndex, mAttribSize, mAttribType,
                            mAttribNormalized, mAttribStride, mAttribPointer);
  if (mAttribEnabled)
    mGL->fEnableVertexAttribArray(mAttribIndex);
  else
    mGL->fDisableVertexAttribArray(mAttribIndex);
  mGL->fBindBuffer(LOCAL_GL_ARRAY_BUFFER, mBoundBuffer);
>>>>>>> upstream-releases
}

////////////////////////////////////////////////////////////////////////
// ScopedPackState

ScopedPackState::ScopedPackState(GLContext* gl)
<<<<<<< HEAD
    : ScopedGLWrapper<ScopedPackState>(gl),
      mAlignment(0),
      mPixelBuffer(0),
      mRowLength(0),
      mSkipPixels(0),
      mSkipRows(0) {
  mGL->fGetIntegerv(LOCAL_GL_PACK_ALIGNMENT, &mAlignment);
||||||| merged common ancestors
    : ScopedGLWrapper<ScopedPackState>(gl)
    , mAlignment(0)
    , mPixelBuffer(0)
    , mRowLength(0)
    , mSkipPixels(0)
    , mSkipRows(0)
{
    mGL->fGetIntegerv(LOCAL_GL_PACK_ALIGNMENT, &mAlignment);
=======
    : mGL(gl),
      mAlignment(0),
      mPixelBuffer(0),
      mRowLength(0),
      mSkipPixels(0),
      mSkipRows(0) {
  mGL->fGetIntegerv(LOCAL_GL_PACK_ALIGNMENT, &mAlignment);

  if (mAlignment != 4) mGL->fPixelStorei(LOCAL_GL_PACK_ALIGNMENT, 4);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mAlignment != 4) mGL->fPixelStorei(LOCAL_GL_PACK_ALIGNMENT, 4);
||||||| merged common ancestors
    if (mAlignment != 4) mGL->fPixelStorei(LOCAL_GL_PACK_ALIGNMENT, 4);
=======
  if (!mGL->HasPBOState()) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!mGL->HasPBOState()) return;
||||||| merged common ancestors
    if (!mGL->HasPBOState())
        return;
=======
  mGL->fGetIntegerv(LOCAL_GL_PIXEL_PACK_BUFFER_BINDING, (GLint*)&mPixelBuffer);
  mGL->fGetIntegerv(LOCAL_GL_PACK_ROW_LENGTH, &mRowLength);
  mGL->fGetIntegerv(LOCAL_GL_PACK_SKIP_PIXELS, &mSkipPixels);
  mGL->fGetIntegerv(LOCAL_GL_PACK_SKIP_ROWS, &mSkipRows);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mGL->fGetIntegerv(LOCAL_GL_PIXEL_PACK_BUFFER_BINDING, (GLint*)&mPixelBuffer);
  mGL->fGetIntegerv(LOCAL_GL_PACK_ROW_LENGTH, &mRowLength);
  mGL->fGetIntegerv(LOCAL_GL_PACK_SKIP_PIXELS, &mSkipPixels);
  mGL->fGetIntegerv(LOCAL_GL_PACK_SKIP_ROWS, &mSkipRows);
||||||| merged common ancestors
    mGL->fGetIntegerv(LOCAL_GL_PIXEL_PACK_BUFFER_BINDING, (GLint*)&mPixelBuffer);
    mGL->fGetIntegerv(LOCAL_GL_PACK_ROW_LENGTH, &mRowLength);
    mGL->fGetIntegerv(LOCAL_GL_PACK_SKIP_PIXELS, &mSkipPixels);
    mGL->fGetIntegerv(LOCAL_GL_PACK_SKIP_ROWS, &mSkipRows);
=======
  if (mPixelBuffer != 0) mGL->fBindBuffer(LOCAL_GL_PIXEL_PACK_BUFFER, 0);
  if (mRowLength != 0) mGL->fPixelStorei(LOCAL_GL_PACK_ROW_LENGTH, 0);
  if (mSkipPixels != 0) mGL->fPixelStorei(LOCAL_GL_PACK_SKIP_PIXELS, 0);
  if (mSkipRows != 0) mGL->fPixelStorei(LOCAL_GL_PACK_SKIP_ROWS, 0);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mPixelBuffer != 0) mGL->fBindBuffer(LOCAL_GL_PIXEL_PACK_BUFFER, 0);
  if (mRowLength != 0) mGL->fPixelStorei(LOCAL_GL_PACK_ROW_LENGTH, 0);
  if (mSkipPixels != 0) mGL->fPixelStorei(LOCAL_GL_PACK_SKIP_PIXELS, 0);
  if (mSkipRows != 0) mGL->fPixelStorei(LOCAL_GL_PACK_SKIP_ROWS, 0);
||||||| merged common ancestors
    if (mPixelBuffer != 0) mGL->fBindBuffer(LOCAL_GL_PIXEL_PACK_BUFFER, 0);
    if (mRowLength != 0)   mGL->fPixelStorei(LOCAL_GL_PACK_ROW_LENGTH, 0);
    if (mSkipPixels != 0)  mGL->fPixelStorei(LOCAL_GL_PACK_SKIP_PIXELS, 0);
    if (mSkipRows != 0)    mGL->fPixelStorei(LOCAL_GL_PACK_SKIP_ROWS, 0);
=======
bool ScopedPackState::SetForWidthAndStrideRGBA(GLsizei aWidth,
                                               GLsizei aStride) {
  MOZ_ASSERT(aStride % 4 == 0, "RGBA data should always be 4-byte aligned");
  MOZ_ASSERT(aStride / 4 >= aWidth, "Stride too small");
  if (aStride / 4 == aWidth) {
    // No special handling needed.
    return true;
  }
  if (mGL->HasPBOState()) {
    // HasPBOState implies support for GL_PACK_ROW_LENGTH.
    mGL->fPixelStorei(LOCAL_GL_PACK_ROW_LENGTH, aStride / 4);
    return true;
  }
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void ScopedPackState::UnwrapImpl() {
  mGL->fPixelStorei(LOCAL_GL_PACK_ALIGNMENT, mAlignment);
||||||| merged common ancestors
void
ScopedPackState::UnwrapImpl()
{
    mGL->fPixelStorei(LOCAL_GL_PACK_ALIGNMENT, mAlignment);
=======
ScopedPackState::~ScopedPackState() {
  mGL->fPixelStorei(LOCAL_GL_PACK_ALIGNMENT, mAlignment);
>>>>>>> upstream-releases

  if (!mGL->HasPBOState()) return;

  mGL->fBindBuffer(LOCAL_GL_PIXEL_PACK_BUFFER, mPixelBuffer);
  mGL->fPixelStorei(LOCAL_GL_PACK_ROW_LENGTH, mRowLength);
  mGL->fPixelStorei(LOCAL_GL_PACK_SKIP_PIXELS, mSkipPixels);
  mGL->fPixelStorei(LOCAL_GL_PACK_SKIP_ROWS, mSkipRows);
}

////////////////////////////////////////////////////////////////////////
// ResetUnpackState

ResetUnpackState::ResetUnpackState(GLContext* gl)
<<<<<<< HEAD
    : ScopedGLWrapper<ResetUnpackState>(gl),
      mAlignment(0),
      mPBO(0),
      mRowLength(0),
      mImageHeight(0),
      mSkipPixels(0),
      mSkipRows(0),
      mSkipImages(0) {
  const auto fnReset = [&](GLenum pname, GLuint val, GLuint* const out_old) {
    mGL->GetUIntegerv(pname, out_old);
    if (*out_old != val) {
      mGL->fPixelStorei(pname, val);
    }
  };

  // Default is 4, but 1 is more useful.
  fnReset(LOCAL_GL_UNPACK_ALIGNMENT, 1, &mAlignment);

  if (!mGL->HasPBOState()) return;

  mGL->GetUIntegerv(LOCAL_GL_PIXEL_UNPACK_BUFFER_BINDING, &mPBO);
  if (mPBO != 0) mGL->fBindBuffer(LOCAL_GL_PIXEL_UNPACK_BUFFER, 0);

  fnReset(LOCAL_GL_UNPACK_ROW_LENGTH, 0, &mRowLength);
  fnReset(LOCAL_GL_UNPACK_IMAGE_HEIGHT, 0, &mImageHeight);
  fnReset(LOCAL_GL_UNPACK_SKIP_PIXELS, 0, &mSkipPixels);
  fnReset(LOCAL_GL_UNPACK_SKIP_ROWS, 0, &mSkipRows);
  fnReset(LOCAL_GL_UNPACK_SKIP_IMAGES, 0, &mSkipImages);
}

void ResetUnpackState::UnwrapImpl() {
  mGL->fPixelStorei(LOCAL_GL_UNPACK_ALIGNMENT, mAlignment);

  if (!mGL->HasPBOState()) return;

  mGL->fBindBuffer(LOCAL_GL_PIXEL_UNPACK_BUFFER, mPBO);

  mGL->fPixelStorei(LOCAL_GL_UNPACK_ROW_LENGTH, mRowLength);
  mGL->fPixelStorei(LOCAL_GL_UNPACK_IMAGE_HEIGHT, mImageHeight);
  mGL->fPixelStorei(LOCAL_GL_UNPACK_SKIP_PIXELS, mSkipPixels);
  mGL->fPixelStorei(LOCAL_GL_UNPACK_SKIP_ROWS, mSkipRows);
  mGL->fPixelStorei(LOCAL_GL_UNPACK_SKIP_IMAGES, mSkipImages);
||||||| merged common ancestors
    : ScopedGLWrapper<ResetUnpackState>(gl)
    , mAlignment(0)
    , mPBO(0)
    , mRowLength(0)
    , mImageHeight(0)
    , mSkipPixels(0)
    , mSkipRows(0)
    , mSkipImages(0)
{
    const auto fnReset = [&](GLenum pname, GLuint val, GLuint* const out_old) {
        mGL->GetUIntegerv(pname, out_old);
        if (*out_old != val) {
            mGL->fPixelStorei(pname, val);
        }
    };

    // Default is 4, but 1 is more useful.
    fnReset(LOCAL_GL_UNPACK_ALIGNMENT, 1, &mAlignment);

    if (!mGL->HasPBOState())
        return;

    mGL->GetUIntegerv(LOCAL_GL_PIXEL_UNPACK_BUFFER_BINDING, &mPBO);
    if (mPBO != 0) mGL->fBindBuffer(LOCAL_GL_PIXEL_UNPACK_BUFFER, 0);

    fnReset(LOCAL_GL_UNPACK_ROW_LENGTH  , 0, &mRowLength);
    fnReset(LOCAL_GL_UNPACK_IMAGE_HEIGHT, 0, &mImageHeight);
    fnReset(LOCAL_GL_UNPACK_SKIP_PIXELS , 0, &mSkipPixels);
    fnReset(LOCAL_GL_UNPACK_SKIP_ROWS   , 0, &mSkipRows);
    fnReset(LOCAL_GL_UNPACK_SKIP_IMAGES , 0, &mSkipImages);
}

void
ResetUnpackState::UnwrapImpl()
{
    mGL->fPixelStorei(LOCAL_GL_UNPACK_ALIGNMENT, mAlignment);

    if (!mGL->HasPBOState())
        return;

    mGL->fBindBuffer(LOCAL_GL_PIXEL_UNPACK_BUFFER, mPBO);

    mGL->fPixelStorei(LOCAL_GL_UNPACK_ROW_LENGTH, mRowLength);
    mGL->fPixelStorei(LOCAL_GL_UNPACK_IMAGE_HEIGHT, mImageHeight);
    mGL->fPixelStorei(LOCAL_GL_UNPACK_SKIP_PIXELS, mSkipPixels);
    mGL->fPixelStorei(LOCAL_GL_UNPACK_SKIP_ROWS, mSkipRows);
    mGL->fPixelStorei(LOCAL_GL_UNPACK_SKIP_IMAGES, mSkipImages);
=======
    : mGL(gl),
      mAlignment(0),
      mPBO(0),
      mRowLength(0),
      mImageHeight(0),
      mSkipPixels(0),
      mSkipRows(0),
      mSkipImages(0) {
  const auto fnReset = [&](GLenum pname, GLuint val, GLuint* const out_old) {
    mGL->GetUIntegerv(pname, out_old);
    if (*out_old != val) {
      mGL->fPixelStorei(pname, val);
    }
  };

  // Default is 4, but 1 is more useful.
  fnReset(LOCAL_GL_UNPACK_ALIGNMENT, 1, &mAlignment);

  if (!mGL->HasPBOState()) return;

  mGL->GetUIntegerv(LOCAL_GL_PIXEL_UNPACK_BUFFER_BINDING, &mPBO);
  if (mPBO != 0) mGL->fBindBuffer(LOCAL_GL_PIXEL_UNPACK_BUFFER, 0);

  fnReset(LOCAL_GL_UNPACK_ROW_LENGTH, 0, &mRowLength);
  fnReset(LOCAL_GL_UNPACK_IMAGE_HEIGHT, 0, &mImageHeight);
  fnReset(LOCAL_GL_UNPACK_SKIP_PIXELS, 0, &mSkipPixels);
  fnReset(LOCAL_GL_UNPACK_SKIP_ROWS, 0, &mSkipRows);
  fnReset(LOCAL_GL_UNPACK_SKIP_IMAGES, 0, &mSkipImages);
}

ResetUnpackState::~ResetUnpackState() {
  mGL->fPixelStorei(LOCAL_GL_UNPACK_ALIGNMENT, mAlignment);

  if (!mGL->HasPBOState()) return;

  mGL->fBindBuffer(LOCAL_GL_PIXEL_UNPACK_BUFFER, mPBO);

  mGL->fPixelStorei(LOCAL_GL_UNPACK_ROW_LENGTH, mRowLength);
  mGL->fPixelStorei(LOCAL_GL_UNPACK_IMAGE_HEIGHT, mImageHeight);
  mGL->fPixelStorei(LOCAL_GL_UNPACK_SKIP_PIXELS, mSkipPixels);
  mGL->fPixelStorei(LOCAL_GL_UNPACK_SKIP_ROWS, mSkipRows);
  mGL->fPixelStorei(LOCAL_GL_UNPACK_SKIP_IMAGES, mSkipImages);
>>>>>>> upstream-releases
}

////////////////////////////////////////////////////////////////////////
// ScopedBindPBO

static GLuint GetPBOBinding(GLContext* gl, GLenum target) {
  if (!gl->HasPBOState()) return 0;

  GLenum targetBinding;
  switch (target) {
    case LOCAL_GL_PIXEL_PACK_BUFFER:
      targetBinding = LOCAL_GL_PIXEL_PACK_BUFFER_BINDING;
      break;

    case LOCAL_GL_PIXEL_UNPACK_BUFFER:
      targetBinding = LOCAL_GL_PIXEL_UNPACK_BUFFER_BINDING;
      break;

    default:
      MOZ_CRASH();
  }

  return gl->GetIntAs<GLuint>(targetBinding);
}

ScopedBindPBO::ScopedBindPBO(GLContext* gl, GLenum target)
<<<<<<< HEAD
    : ScopedGLWrapper<ScopedBindPBO>(gl),
      mTarget(target),
      mPBO(GetPBOBinding(mGL, mTarget)) {}

void ScopedBindPBO::UnwrapImpl() {
  if (!mGL->HasPBOState()) return;

  mGL->fBindBuffer(mTarget, mPBO);
||||||| merged common ancestors
    : ScopedGLWrapper<ScopedBindPBO>(gl)
    , mTarget(target)
    , mPBO(GetPBOBinding(mGL, mTarget))
{ }

void
ScopedBindPBO::UnwrapImpl()
{
    if (!mGL->HasPBOState())
        return;

    mGL->fBindBuffer(mTarget, mPBO);
=======
    : mGL(gl), mTarget(target), mPBO(GetPBOBinding(mGL, mTarget)) {}

ScopedBindPBO::~ScopedBindPBO() {
  if (!mGL->HasPBOState()) return;

  mGL->fBindBuffer(mTarget, mPBO);
>>>>>>> upstream-releases
}

} /* namespace gl */
} /* namespace mozilla */
