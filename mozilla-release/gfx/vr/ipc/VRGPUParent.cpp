/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "VRGPUParent.h"

#include "mozilla/ipc/ProcessChild.h"

namespace mozilla {
namespace gfx {

using namespace ipc;

<<<<<<< HEAD
VRGPUParent::VRGPUParent(ProcessId aChildProcessId) {
||||||| merged common ancestors

VRGPUParent::VRGPUParent(ProcessId aChildProcessId)
{
=======
VRGPUParent::VRGPUParent(ProcessId aChildProcessId) : mClosed(false) {
>>>>>>> upstream-releases
  MOZ_COUNT_CTOR(VRGPUParent);
  MOZ_ASSERT(NS_IsMainThread());

  SetOtherProcessId(aChildProcessId);
}

<<<<<<< HEAD
void VRGPUParent::ActorDestroy(ActorDestroyReason aWhy) {
||||||| merged common ancestors
void
VRGPUParent::ActorDestroy(ActorDestroyReason aWhy)
{
=======
VRGPUParent::~VRGPUParent() { MOZ_COUNT_DTOR(VRGPUParent); }

void VRGPUParent::ActorDestroy(ActorDestroyReason aWhy) {
>>>>>>> upstream-releases
#if !defined(MOZ_WIDGET_ANDROID)
  if (mVRService) {
    mVRService->Stop();
    mVRService = nullptr;
  }
#endif

  mClosed = true;
  MessageLoop::current()->PostTask(
      NewRunnableMethod("gfx::VRGPUParent::DeferredDestroy", this,
                        &VRGPUParent::DeferredDestroy));
}

void VRGPUParent::DeferredDestroy() { mSelfRef = nullptr; }

<<<<<<< HEAD
/* static */ RefPtr<VRGPUParent> VRGPUParent::CreateForGPU(
    Endpoint<PVRGPUParent>&& aEndpoint) {
||||||| merged common ancestors
/* static */ RefPtr<VRGPUParent>
VRGPUParent::CreateForGPU(Endpoint<PVRGPUParent>&& aEndpoint)
{
=======
/* static */
RefPtr<VRGPUParent> VRGPUParent::CreateForGPU(
    Endpoint<PVRGPUParent>&& aEndpoint) {
>>>>>>> upstream-releases
  RefPtr<VRGPUParent> vcp = new VRGPUParent(aEndpoint.OtherPid());
  MessageLoop::current()->PostTask(NewRunnableMethod<Endpoint<PVRGPUParent>&&>(
      "gfx::VRGPUParent::Bind", vcp, &VRGPUParent::Bind, std::move(aEndpoint)));

  return vcp.forget();
}

void VRGPUParent::Bind(Endpoint<PVRGPUParent>&& aEndpoint) {
  if (!aEndpoint.Bind(this)) {
    return;
  }

  mSelfRef = this;
}

mozilla::ipc::IPCResult VRGPUParent::RecvStartVRService() {
#if !defined(MOZ_WIDGET_ANDROID)
  mVRService = VRService::Create();
  MOZ_ASSERT(mVRService);

  mVRService->Start();
#endif

  return IPC_OK();
}

mozilla::ipc::IPCResult VRGPUParent::RecvStopVRService() {
#if !defined(MOZ_WIDGET_ANDROID)
  if (mVRService) {
    mVRService->Stop();
    mVRService = nullptr;
  }
#endif

  return IPC_OK();
}

<<<<<<< HEAD
}  // namespace gfx
}  // namespace mozilla
||||||| merged common ancestors
} // namespace gfx
} // namespace mozilla
=======
bool VRGPUParent::IsClosed() { return mClosed; }

}  // namespace gfx
}  // namespace mozilla
>>>>>>> upstream-releases
