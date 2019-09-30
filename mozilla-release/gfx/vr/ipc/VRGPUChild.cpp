/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "VRGPUChild.h"
<<<<<<< HEAD

||||||| merged common ancestors


=======
#include "mozilla/layers/CompositorThread.h"

>>>>>>> upstream-releases
namespace mozilla {
namespace gfx {

static StaticRefPtr<VRGPUChild> sVRGPUChildSingleton;

<<<<<<< HEAD
/* static */ bool VRGPUChild::InitForGPUProcess(
    Endpoint<PVRGPUChild>&& aEndpoint) {
||||||| merged common ancestors
/* static */ bool
VRGPUChild::InitForGPUProcess(Endpoint<PVRGPUChild>&& aEndpoint)
{
=======
/* static */
bool VRGPUChild::InitForGPUProcess(Endpoint<PVRGPUChild>&& aEndpoint) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(!sVRGPUChildSingleton);

  RefPtr<VRGPUChild> child(new VRGPUChild());
  if (!aEndpoint.Bind(child)) {
    return false;
  }
  sVRGPUChildSingleton = child;

  RefPtr<Runnable> task =
      NS_NewRunnableFunction("VRGPUChild::SendStartVRService", []() -> void {
        VRGPUChild* vrGPUChild = VRGPUChild::Get();
        vrGPUChild->SendStartVRService();
      });

  NS_DispatchToMainThread(task.forget());

  return true;
}

<<<<<<< HEAD
/* static */ bool VRGPUChild::IsCreated() { return !!sVRGPUChildSingleton; }
||||||| merged common ancestors
/* static */ bool
VRGPUChild::IsCreated()
{
  return !!sVRGPUChildSingleton;
}
=======
/* static */
bool VRGPUChild::IsCreated() { return !!sVRGPUChildSingleton; }
>>>>>>> upstream-releases

<<<<<<< HEAD
/* static */ VRGPUChild* VRGPUChild::Get() {
||||||| merged common ancestors
/* static */ VRGPUChild*
VRGPUChild::Get()
{
=======
/* static */
VRGPUChild* VRGPUChild::Get() {
>>>>>>> upstream-releases
  MOZ_ASSERT(IsCreated(), "VRGPUChild haven't initialized yet.");
  return sVRGPUChildSingleton;
}

<<<<<<< HEAD
/*static*/ void VRGPUChild::ShutDown() {
||||||| merged common ancestors
/*static*/ void
VRGPUChild::ShutDown()
{
=======
/*static*/
void VRGPUChild::Shutdown() {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
  if (sVRGPUChildSingleton && !sVRGPUChildSingleton->IsClosed()) {
    sVRGPUChildSingleton->Close();
  }
  sVRGPUChildSingleton = nullptr;
}

<<<<<<< HEAD
}  // namespace gfx
}  // namespace mozilla
||||||| merged common ancestors
} // namespace gfx
} // namespace mozilla
=======
void VRGPUChild::ActorDestroy(ActorDestroyReason aWhy) {
  VRManager* vm = VRManager::Get();
  mozilla::layers::CompositorThreadHolder::Loop()->PostTask(
      NewRunnableMethod("VRGPUChild::ActorDestroy", vm, &VRManager::Shutdown));

  mClosed = true;
}

bool VRGPUChild::IsClosed() { return mClosed; }

}  // namespace gfx
}  // namespace mozilla
>>>>>>> upstream-releases
