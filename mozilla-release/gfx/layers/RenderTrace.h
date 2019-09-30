/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// This is a general tool that will let you visualize platform operation.
// Currently used for the layer system, the general syntax allows this
// tools to be adapted to trace other operations.
//
// For the front end see: https://github.com/staktrace/rendertrace

// Uncomment this line to enable RENDERTRACE
//#define MOZ_RENDERTRACE

#ifndef GFX_RENDERTRACE_H
#define GFX_RENDERTRACE_H

#include "nsRect.h"
#include "mozilla/gfx/Matrix.h"

namespace mozilla {
namespace layers {

class Layer;

<<<<<<< HEAD
void RenderTraceLayers(Layer *aLayer, const char *aColor,
                       const gfx::Matrix4x4 aRootTransform = gfx::Matrix4x4(),
                       bool aReset = true);
||||||| merged common ancestors
void RenderTraceLayers(Layer *aLayer, const char *aColor, const gfx::Matrix4x4 aRootTransform = gfx::Matrix4x4(), bool aReset = true);
=======
void RenderTraceLayers(Layer* aLayer, const char* aColor,
                       const gfx::Matrix4x4 aRootTransform = gfx::Matrix4x4(),
                       bool aReset = true);
>>>>>>> upstream-releases

<<<<<<< HEAD
void RenderTraceInvalidateStart(Layer *aLayer, const char *aColor,
                                const gfx::IntRect aRect);
void RenderTraceInvalidateEnd(Layer *aLayer, const char *aColor);
||||||| merged common ancestors
void RenderTraceInvalidateStart(Layer *aLayer, const char *aColor, const gfx::IntRect aRect);
void RenderTraceInvalidateEnd(Layer *aLayer, const char *aColor);
=======
void RenderTraceInvalidateStart(Layer* aLayer, const char* aColor,
                                const gfx::IntRect aRect);
void RenderTraceInvalidateEnd(Layer* aLayer, const char* aColor);
>>>>>>> upstream-releases

void renderTraceEventStart(const char* aComment, const char* aColor);
void renderTraceEventEnd(const char* aComment, const char* aColor);
void renderTraceEventEnd(const char* aColor);

struct RenderTraceScope {
<<<<<<< HEAD
 public:
  RenderTraceScope(const char *aComment, const char *aColor)
      : mComment(aComment), mColor(aColor) {
||||||| merged common ancestors
public:
  RenderTraceScope(const char *aComment, const char *aColor)
    : mComment(aComment)
    , mColor(aColor)
  {
=======
 public:
  RenderTraceScope(const char* aComment, const char* aColor)
      : mComment(aComment), mColor(aColor) {
>>>>>>> upstream-releases
    renderTraceEventStart(mComment, mColor);
  }
<<<<<<< HEAD
  ~RenderTraceScope() { renderTraceEventEnd(mComment, mColor); }

 private:
  const char *mComment;
  const char *mColor;
||||||| merged common ancestors
  ~RenderTraceScope() {
    renderTraceEventEnd(mComment, mColor);
  }
private:
  const char *mComment;
  const char *mColor;
=======
  ~RenderTraceScope() { renderTraceEventEnd(mComment, mColor); }

 private:
  const char* mComment;
  const char* mColor;
>>>>>>> upstream-releases
};

#ifndef MOZ_RENDERTRACE
<<<<<<< HEAD
inline void RenderTraceLayers(Layer *aLayer, const char *aColor,
                              const gfx::Matrix4x4 aRootTransform,
                              bool aReset) {}
||||||| merged common ancestors
inline void RenderTraceLayers(Layer *aLayer, const char *aColor, const gfx::Matrix4x4 aRootTransform, bool aReset)
{}
=======
inline void RenderTraceLayers(Layer* aLayer, const char* aColor,
                              const gfx::Matrix4x4 aRootTransform,
                              bool aReset) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
inline void RenderTraceInvalidateStart(Layer *aLayer, const char *aColor,
                                       const gfx::IntRect aRect) {}
||||||| merged common ancestors
inline void RenderTraceInvalidateStart(Layer *aLayer, const char *aColor, const gfx::IntRect aRect)
{}
=======
inline void RenderTraceInvalidateStart(Layer* aLayer, const char* aColor,
                                       const gfx::IntRect aRect) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
inline void RenderTraceInvalidateEnd(Layer *aLayer, const char *aColor) {}
||||||| merged common ancestors
inline void RenderTraceInvalidateEnd(Layer *aLayer, const char *aColor)
{}
=======
inline void RenderTraceInvalidateEnd(Layer* aLayer, const char* aColor) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
inline void renderTraceEventStart(const char *aComment, const char *aColor) {}
||||||| merged common ancestors
inline void renderTraceEventStart(const char *aComment, const char *aColor)
{}
=======
inline void renderTraceEventStart(const char* aComment, const char* aColor) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
inline void renderTraceEventEnd(const char *aComment, const char *aColor) {}
||||||| merged common ancestors
inline void renderTraceEventEnd(const char *aComment, const char *aColor)
{}
=======
inline void renderTraceEventEnd(const char* aComment, const char* aColor) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
inline void renderTraceEventEnd(const char *aColor) {}
||||||| merged common ancestors
inline void renderTraceEventEnd(const char *aColor)
{}
=======
inline void renderTraceEventEnd(const char* aColor) {}
>>>>>>> upstream-releases

#endif  // MOZ_RENDERTRACE

}  // namespace layers
}  // namespace mozilla

#endif  // GFX_RENDERTRACE_H
