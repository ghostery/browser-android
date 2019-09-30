/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_2D_GLUE_H
#define GFX_2D_GLUE_H

#include "gfxMatrix.h"
#include "gfxPoint.h"
#include "gfxRect.h"
#include "gfxTypes.h"
#include "mozilla/gfx/Matrix.h"
#include "mozilla/gfx/Rect.h"
#include "mozilla/gfx/2D.h"

namespace mozilla {
namespace gfx {

<<<<<<< HEAD
inline Rect ToRect(const gfxRect &aRect) {
  return Rect(Float(aRect.X()), Float(aRect.Y()), Float(aRect.Width()),
              Float(aRect.Height()));
||||||| merged common ancestors
inline Rect ToRect(const gfxRect &aRect)
{
  return Rect(Float(aRect.X()), Float(aRect.Y()),
              Float(aRect.Width()), Float(aRect.Height()));
=======
inline Rect ToRect(const gfxRect& aRect) {
  return Rect(Float(aRect.X()), Float(aRect.Y()), Float(aRect.Width()),
              Float(aRect.Height()));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline RectDouble ToRectDouble(const gfxRect &aRect) {
||||||| merged common ancestors
inline RectDouble ToRectDouble(const gfxRect &aRect)
{
=======
inline RectDouble ToRectDouble(const gfxRect& aRect) {
>>>>>>> upstream-releases
  return RectDouble(aRect.X(), aRect.Y(), aRect.Width(), aRect.Height());
}

<<<<<<< HEAD
inline Matrix ToMatrix(const gfxMatrix &aMatrix) {
||||||| merged common ancestors
inline Matrix ToMatrix(const gfxMatrix &aMatrix)
{
=======
inline Matrix ToMatrix(const gfxMatrix& aMatrix) {
>>>>>>> upstream-releases
  return Matrix(Float(aMatrix._11), Float(aMatrix._12), Float(aMatrix._21),
                Float(aMatrix._22), Float(aMatrix._31), Float(aMatrix._32));
}

<<<<<<< HEAD
inline gfxMatrix ThebesMatrix(const Matrix &aMatrix) {
  return gfxMatrix(aMatrix._11, aMatrix._12, aMatrix._21, aMatrix._22,
                   aMatrix._31, aMatrix._32);
||||||| merged common ancestors
inline gfxMatrix ThebesMatrix(const Matrix &aMatrix)
{
  return gfxMatrix(aMatrix._11, aMatrix._12, aMatrix._21,
                   aMatrix._22, aMatrix._31, aMatrix._32);
=======
inline gfxMatrix ThebesMatrix(const Matrix& aMatrix) {
  return gfxMatrix(aMatrix._11, aMatrix._12, aMatrix._21, aMatrix._22,
                   aMatrix._31, aMatrix._32);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline Point ToPoint(const gfxPoint &aPoint) {
||||||| merged common ancestors
inline Point ToPoint(const gfxPoint &aPoint)
{
=======
inline Point ToPoint(const gfxPoint& aPoint) {
>>>>>>> upstream-releases
  return Point(Float(aPoint.x), Float(aPoint.y));
}

<<<<<<< HEAD
inline Size ToSize(const gfxSize &aSize) {
||||||| merged common ancestors
inline Size ToSize(const gfxSize &aSize)
{
=======
inline Size ToSize(const gfxSize& aSize) {
>>>>>>> upstream-releases
  return Size(Float(aSize.width), Float(aSize.height));
}

<<<<<<< HEAD
inline gfxPoint ThebesPoint(const Point &aPoint) {
||||||| merged common ancestors
inline gfxPoint ThebesPoint(const Point &aPoint)
{
=======
inline gfxPoint ThebesPoint(const Point& aPoint) {
>>>>>>> upstream-releases
  return gfxPoint(aPoint.x, aPoint.y);
}

<<<<<<< HEAD
inline gfxSize ThebesSize(const Size &aSize) {
||||||| merged common ancestors
inline gfxSize ThebesSize(const Size &aSize)
{
=======
inline gfxSize ThebesSize(const Size& aSize) {
>>>>>>> upstream-releases
  return gfxSize(aSize.width, aSize.height);
}

<<<<<<< HEAD
inline gfxRect ThebesRect(const Rect &aRect) {
||||||| merged common ancestors
inline gfxRect ThebesRect(const Rect &aRect)
{
=======
inline gfxRect ThebesRect(const Rect& aRect) {
>>>>>>> upstream-releases
  return gfxRect(aRect.X(), aRect.Y(), aRect.Width(), aRect.Height());
}

<<<<<<< HEAD
inline gfxRect ThebesRect(const IntRect &aRect) {
||||||| merged common ancestors
inline gfxRect ThebesRect(const IntRect &aRect)
{
=======
inline gfxRect ThebesRect(const IntRect& aRect) {
>>>>>>> upstream-releases
  return gfxRect(aRect.X(), aRect.Y(), aRect.Width(), aRect.Height());
}

<<<<<<< HEAD
inline gfxRect ThebesRect(const RectDouble &aRect) {
||||||| merged common ancestors
inline gfxRect ThebesRect(const RectDouble &aRect)
{
=======
inline gfxRect ThebesRect(const RectDouble& aRect) {
>>>>>>> upstream-releases
  return gfxRect(aRect.X(), aRect.Y(), aRect.Width(), aRect.Height());
}

inline gfxImageFormat SurfaceFormatToImageFormat(SurfaceFormat aFormat) {
  switch (aFormat) {
    case SurfaceFormat::B8G8R8A8:
      return SurfaceFormat::A8R8G8B8_UINT32;
    case SurfaceFormat::B8G8R8X8:
      return SurfaceFormat::X8R8G8B8_UINT32;
    case SurfaceFormat::R5G6B5_UINT16:
      return SurfaceFormat::R5G6B5_UINT16;
    case SurfaceFormat::A8:
      return SurfaceFormat::A8;
    default:
      return SurfaceFormat::UNKNOWN;
  }
}

inline SurfaceFormat ImageFormatToSurfaceFormat(gfxImageFormat aFormat) {
  switch (aFormat) {
    case SurfaceFormat::A8R8G8B8_UINT32:
      return SurfaceFormat::B8G8R8A8;
    case SurfaceFormat::X8R8G8B8_UINT32:
      return SurfaceFormat::B8G8R8X8;
    case SurfaceFormat::R5G6B5_UINT16:
      return SurfaceFormat::R5G6B5_UINT16;
    case SurfaceFormat::A8:
      return SurfaceFormat::A8;
    default:
    case SurfaceFormat::UNKNOWN:
      return SurfaceFormat::B8G8R8A8;
  }
}

<<<<<<< HEAD
inline gfxContentType ContentForFormat(const SurfaceFormat &aFormat) {
||||||| merged common ancestors
inline gfxContentType ContentForFormat(const SurfaceFormat &aFormat)
{
=======
inline gfxContentType ContentForFormat(const SurfaceFormat& aFormat) {
>>>>>>> upstream-releases
  switch (aFormat) {
    case SurfaceFormat::R5G6B5_UINT16:
    case SurfaceFormat::B8G8R8X8:
    case SurfaceFormat::R8G8B8X8:
      return gfxContentType::COLOR;
    case SurfaceFormat::A8:
      return gfxContentType::ALPHA;
    case SurfaceFormat::B8G8R8A8:
    case SurfaceFormat::R8G8B8A8:
    default:
      return gfxContentType::COLOR_ALPHA;
  }
}

}  // namespace gfx
}  // namespace mozilla

#endif
