/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_VR_GPU_PARENT_H
#define GFX_VR_GPU_PARENT_H

#include "mozilla/gfx/PVRGPUParent.h"
#include "VRService.h"

namespace mozilla {
namespace gfx {

class VRGPUParent final : public PVRGPUParent {
  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(VRGPUParent)

<<<<<<< HEAD
 public:
  explicit VRGPUParent(ProcessId aChildProcessId);
||||||| merged common ancestors
public:
  explicit VRGPUParent(ProcessId aChildProcessId);
=======
  friend class PVRGPUParent;
>>>>>>> upstream-releases

 public:
  static RefPtr<VRGPUParent> CreateForGPU(Endpoint<PVRGPUParent>&& aEndpoint);
  virtual void ActorDestroy(ActorDestroyReason aWhy) override;
  bool IsClosed();

<<<<<<< HEAD
 protected:
  ~VRGPUParent() {}

||||||| merged common ancestors
protected:
  ~VRGPUParent() {}

=======
 protected:
>>>>>>> upstream-releases
  void Bind(Endpoint<PVRGPUParent>&& aEndpoint);
  mozilla::ipc::IPCResult RecvStartVRService();
  mozilla::ipc::IPCResult RecvStopVRService();

 private:
  explicit VRGPUParent(ProcessId aChildProcessId);
  ~VRGPUParent();

<<<<<<< HEAD
 private:
||||||| merged common ancestors
private:
=======
>>>>>>> upstream-releases
  void DeferredDestroy();

  RefPtr<VRGPUParent> mSelfRef;
#if !defined(MOZ_WIDGET_ANDROID)
  RefPtr<VRService> mVRService;
#endif
  bool mClosed;

  DISALLOW_COPY_AND_ASSIGN(VRGPUParent);
};

}  // namespace gfx
}  // namespace mozilla

#endif  // GFX_VR_CONTENT_PARENT_H
