/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_2D_HELPERS_H_
#define MOZILLA_GFX_2D_HELPERS_H_

#include "2D.h"

namespace mozilla {
namespace gfx {

<<<<<<< HEAD
class AutoRestoreTransform {
||||||| merged common ancestors
class AutoRestoreTransform
{
=======
class AutoRestoreTransform final {
>>>>>>> upstream-releases
 public:
<<<<<<< HEAD
  AutoRestoreTransform() {}
||||||| merged common ancestors
  AutoRestoreTransform()
  {
  }
=======
  AutoRestoreTransform() = default;
>>>>>>> upstream-releases

<<<<<<< HEAD
  explicit AutoRestoreTransform(DrawTarget *aTarget)
      : mDrawTarget(aTarget), mOldTransform(aTarget->GetTransform()) {}
||||||| merged common ancestors
  explicit AutoRestoreTransform(DrawTarget *aTarget)
   : mDrawTarget(aTarget),
     mOldTransform(aTarget->GetTransform())
  {
  }
=======
  explicit AutoRestoreTransform(DrawTarget* aTarget)
      : mDrawTarget(aTarget), mOldTransform(aTarget->GetTransform()) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Init(DrawTarget *aTarget) {
||||||| merged common ancestors
  void Init(DrawTarget *aTarget)
  {
=======
  void Init(DrawTarget* aTarget) {
>>>>>>> upstream-releases
    MOZ_ASSERT(!mDrawTarget || aTarget == mDrawTarget);
    if (!mDrawTarget) {
      mDrawTarget = aTarget;
      mOldTransform = aTarget->GetTransform();
    }
  }

  ~AutoRestoreTransform() {
    if (mDrawTarget) {
      mDrawTarget->SetTransform(mOldTransform);
    }
  }

 private:
  RefPtr<DrawTarget> mDrawTarget;
  Matrix mOldTransform;
};

<<<<<<< HEAD
class AutoPopClips {
 public:
  explicit AutoPopClips(DrawTarget *aTarget)
      : mDrawTarget(aTarget), mPushCount(0) {
||||||| merged common ancestors
class AutoPopClips
{
public:
  explicit AutoPopClips(DrawTarget *aTarget)
    : mDrawTarget(aTarget)
    , mPushCount(0)
  {
=======
class AutoPopClips final {
 public:
  explicit AutoPopClips(DrawTarget* aTarget)
      : mDrawTarget(aTarget), mPushCount(0) {
>>>>>>> upstream-releases
    MOZ_ASSERT(mDrawTarget);
  }

  ~AutoPopClips() { PopAll(); }

<<<<<<< HEAD
  void PushClip(const Path *aPath) {
||||||| merged common ancestors
  void PushClip(const Path *aPath)
  {
=======
  void PushClip(const Path* aPath) {
>>>>>>> upstream-releases
    mDrawTarget->PushClip(aPath);
    ++mPushCount;
  }

<<<<<<< HEAD
  void PushClipRect(const Rect &aRect) {
||||||| merged common ancestors
  void PushClipRect(const Rect &aRect)
  {
=======
  void PushClipRect(const Rect& aRect) {
>>>>>>> upstream-releases
    mDrawTarget->PushClipRect(aRect);
    ++mPushCount;
  }

  void PopClip() {
    MOZ_ASSERT(mPushCount > 0);
    mDrawTarget->PopClip();
    --mPushCount;
  }

  void PopAll() {
    while (mPushCount-- > 0) {
      mDrawTarget->PopClip();
    }
  }

 private:
  RefPtr<DrawTarget> mDrawTarget;
  int32_t mPushCount;
};

}  // namespace gfx
}  // namespace mozilla

#endif  // MOZILLA_GFX_2D_HELPERS_H_
