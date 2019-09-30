/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GLManager.h"
#include "CompositorOGL.h"  // for CompositorOGL
#include "GLContext.h"      // for GLContext
#include "OGLShaderProgram.h"
#include "mozilla/Attributes.h"         // for override
#include "mozilla/RefPtr.h"             // for RefPtr
#include "mozilla/layers/Compositor.h"  // for Compositor
#include "mozilla/layers/LayerManagerComposite.h"
#include "mozilla/layers/LayersTypes.h"
#include "mozilla/mozalloc.h"  // for operator new, etc

using namespace mozilla::gl;

namespace mozilla {
namespace layers {

class GLManagerCompositor : public GLManager {
 public:
  explicit GLManagerCompositor(CompositorOGL* aCompositor)
      : mImpl(aCompositor) {}

<<<<<<< HEAD
  virtual GLContext* gl() const override { return mImpl->gl(); }
||||||| merged common ancestors
  virtual GLContext* gl() const override
  {
    return mImpl->gl();
  }
=======
  GLContext* gl() const override { return mImpl->gl(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void ActivateProgram(ShaderProgramOGL* aProg) override {
||||||| merged common ancestors
  virtual void ActivateProgram(ShaderProgramOGL *aProg) override
  {
=======
  void ActivateProgram(ShaderProgramOGL* aProg) override {
>>>>>>> upstream-releases
    mImpl->ActivateProgram(aProg);
  }

<<<<<<< HEAD
  virtual ShaderProgramOGL* GetProgram(GLenum aTarget,
                                       gfx::SurfaceFormat aFormat) override {
||||||| merged common ancestors
  virtual ShaderProgramOGL* GetProgram(GLenum aTarget, gfx::SurfaceFormat aFormat) override
  {
=======
  ShaderProgramOGL* GetProgram(GLenum aTarget,
                               gfx::SurfaceFormat aFormat) override {
>>>>>>> upstream-releases
    ShaderConfigOGL config = ShaderConfigFromTargetAndFormat(aTarget, aFormat);
    return mImpl->GetShaderProgramFor(config);
  }

<<<<<<< HEAD
  virtual const gfx::Matrix4x4& GetProjMatrix() const override {
||||||| merged common ancestors
  virtual const gfx::Matrix4x4& GetProjMatrix() const override
  {
=======
  const gfx::Matrix4x4& GetProjMatrix() const override {
>>>>>>> upstream-releases
    return mImpl->GetProjMatrix();
  }

<<<<<<< HEAD
  virtual void BindAndDrawQuad(ShaderProgramOGL* aProg,
                               const gfx::Rect& aLayerRect,
                               const gfx::Rect& aTextureRect) override {
||||||| merged common ancestors
  virtual void BindAndDrawQuad(ShaderProgramOGL *aProg,
                               const gfx::Rect& aLayerRect,
                               const gfx::Rect& aTextureRect) override
  {
=======
  void BindAndDrawQuad(ShaderProgramOGL* aProg, const gfx::Rect& aLayerRect,
                       const gfx::Rect& aTextureRect) override {
>>>>>>> upstream-releases
    mImpl->BindAndDrawQuad(aProg, aLayerRect, aTextureRect);
  }

 private:
  RefPtr<CompositorOGL> mImpl;
};

<<<<<<< HEAD
/* static */ GLManager* GLManager::CreateGLManager(
    LayerManagerComposite* aManager) {
  if (aManager && aManager->GetCompositor()->GetBackendType() ==
                      LayersBackend::LAYERS_OPENGL) {
    return new GLManagerCompositor(
        aManager->GetCompositor()->AsCompositorOGL());
||||||| merged common ancestors
/* static */ GLManager*
GLManager::CreateGLManager(LayerManagerComposite* aManager)
{
  if (aManager && aManager->GetCompositor()->GetBackendType() == LayersBackend::LAYERS_OPENGL) {
    return new GLManagerCompositor(aManager->GetCompositor()->AsCompositorOGL());
=======
/* static */
GLManager* GLManager::CreateGLManager(LayerManagerComposite* aManager) {
  if (aManager && aManager->GetCompositor()->GetBackendType() ==
                      LayersBackend::LAYERS_OPENGL) {
    return new GLManagerCompositor(
        aManager->GetCompositor()->AsCompositorOGL());
>>>>>>> upstream-releases
  }
  return nullptr;
}

}  // namespace layers
}  // namespace mozilla
