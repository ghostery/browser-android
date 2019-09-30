/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "VideoBridgeChild.h"
#include "VideoBridgeParent.h"
#include "CompositorThread.h"

namespace mozilla {
namespace layers {

StaticRefPtr<VideoBridgeChild> sVideoBridgeToParentProcess;
StaticRefPtr<VideoBridgeChild> sVideoBridgeToGPUProcess;

<<<<<<< HEAD
/* static */ void VideoBridgeChild::Startup() {
  sVideoBridgeChildSingleton = new VideoBridgeChild();
  RefPtr<VideoBridgeParent> parent = new VideoBridgeParent();
||||||| merged common ancestors
/* static */ void
VideoBridgeChild::Startup()
{
  sVideoBridgeChildSingleton = new VideoBridgeChild();
  RefPtr<VideoBridgeParent> parent = new VideoBridgeParent();
=======
/* static */
void VideoBridgeChild::StartupForGPUProcess() {
  ipc::Endpoint<PVideoBridgeParent> parentPipe;
  ipc::Endpoint<PVideoBridgeChild> childPipe;
>>>>>>> upstream-releases

  PVideoBridge::CreateEndpoints(base::GetCurrentProcId(),
                                base::GetCurrentProcId(), &parentPipe,
                                &childPipe);

<<<<<<< HEAD
  sVideoBridgeChildSingleton->Open(parent->GetIPCChannel(), loop,
                                   ipc::ChildSide);
  sVideoBridgeChildSingleton->mIPDLSelfRef = sVideoBridgeChildSingleton;
  parent->SetOtherProcessId(base::GetCurrentProcId());
||||||| merged common ancestors
  sVideoBridgeChildSingleton->Open(parent->GetIPCChannel(),
                                   loop,
                                   ipc::ChildSide);
  sVideoBridgeChildSingleton->mIPDLSelfRef = sVideoBridgeChildSingleton;
  parent->SetOtherProcessId(base::GetCurrentProcId());
=======
  VideoBridgeChild::OpenToGPUProcess(std::move(childPipe));

  CompositorThreadHolder::Loop()->PostTask(
      NewRunnableFunction("gfx::VideoBridgeParent::Open",
                          &VideoBridgeParent::Open, std::move(parentPipe)));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ void VideoBridgeChild::Shutdown() {
  if (sVideoBridgeChildSingleton) {
    sVideoBridgeChildSingleton->Close();
    sVideoBridgeChildSingleton = nullptr;
||||||| merged common ancestors
/* static */ void
VideoBridgeChild::Shutdown()
{
  if (sVideoBridgeChildSingleton) {
    sVideoBridgeChildSingleton->Close();
    sVideoBridgeChildSingleton = nullptr;
=======
void VideoBridgeChild::OpenToParentProcess(
    Endpoint<PVideoBridgeChild>&& aEndpoint) {
  sVideoBridgeToParentProcess = new VideoBridgeChild();

  if (!aEndpoint.Bind(sVideoBridgeToParentProcess)) {
    // We can't recover from this.
    MOZ_CRASH("Failed to bind RemoteDecoderManagerParent to endpoint");
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
VideoBridgeChild::VideoBridgeChild()
    : mMessageLoop(MessageLoop::current()), mCanSend(true) {}
||||||| merged common ancestors
VideoBridgeChild::VideoBridgeChild()
  : mMessageLoop(MessageLoop::current())
  , mCanSend(true)
{
}
=======
void VideoBridgeChild::OpenToGPUProcess(
    Endpoint<PVideoBridgeChild>&& aEndpoint) {
  sVideoBridgeToGPUProcess = new VideoBridgeChild();

  if (!aEndpoint.Bind(sVideoBridgeToGPUProcess)) {
    // We can't recover from this.
    MOZ_CRASH("Failed to bind RemoteDecoderManagerParent to endpoint");
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
VideoBridgeChild::~VideoBridgeChild() {}
||||||| merged common ancestors
VideoBridgeChild::~VideoBridgeChild()
{
}
=======
/* static */
void VideoBridgeChild::Shutdown() {
  if (sVideoBridgeToParentProcess) {
    sVideoBridgeToParentProcess->Close();
    sVideoBridgeToParentProcess = nullptr;
  }
  if (sVideoBridgeToGPUProcess) {
    sVideoBridgeToGPUProcess->Close();
    sVideoBridgeToGPUProcess = nullptr;
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
VideoBridgeChild* VideoBridgeChild::GetSingleton() {
  return sVideoBridgeChildSingleton;
||||||| merged common ancestors
VideoBridgeChild*
VideoBridgeChild::GetSingleton()
{
  return sVideoBridgeChildSingleton;
=======
VideoBridgeChild::VideoBridgeChild()
    : mIPDLSelfRef(this),
      mMessageLoop(MessageLoop::current()),
      mCanSend(true) {}

VideoBridgeChild::~VideoBridgeChild() {}

VideoBridgeChild* VideoBridgeChild::GetSingletonToParentProcess() {
  return sVideoBridgeToParentProcess;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool VideoBridgeChild::AllocUnsafeShmem(
    size_t aSize, ipc::SharedMemory::SharedMemoryType aType,
    ipc::Shmem* aShmem) {
||||||| merged common ancestors
bool
VideoBridgeChild::AllocUnsafeShmem(size_t aSize,
                                   ipc::SharedMemory::SharedMemoryType aType,
                                   ipc::Shmem* aShmem)
{
=======
VideoBridgeChild* VideoBridgeChild::GetSingletonToGPUProcess() {
  return sVideoBridgeToGPUProcess;
}

bool VideoBridgeChild::AllocUnsafeShmem(
    size_t aSize, ipc::SharedMemory::SharedMemoryType aType,
    ipc::Shmem* aShmem) {
>>>>>>> upstream-releases
  return PVideoBridgeChild::AllocUnsafeShmem(aSize, aType, aShmem);
}

bool VideoBridgeChild::AllocShmem(size_t aSize,
                                  ipc::SharedMemory::SharedMemoryType aType,
                                  ipc::Shmem* aShmem) {
  MOZ_ASSERT(CanSend());
  return PVideoBridgeChild::AllocShmem(aSize, aType, aShmem);
}

bool VideoBridgeChild::DeallocShmem(ipc::Shmem& aShmem) {
  return PVideoBridgeChild::DeallocShmem(aShmem);
}

PTextureChild* VideoBridgeChild::AllocPTextureChild(const SurfaceDescriptor&,
                                                    const ReadLockDescriptor&,
                                                    const LayersBackend&,
                                                    const TextureFlags&,
                                                    const uint64_t& aSerial) {
  MOZ_ASSERT(CanSend());
  return TextureClient::CreateIPDLActor();
}

bool VideoBridgeChild::DeallocPTextureChild(PTextureChild* actor) {
  return TextureClient::DestroyIPDLActor(actor);
}

void VideoBridgeChild::ActorDestroy(ActorDestroyReason aWhy) {
  mCanSend = false;
}

<<<<<<< HEAD
void VideoBridgeChild::DeallocPVideoBridgeChild() { mIPDLSelfRef = nullptr; }
||||||| merged common ancestors
void
VideoBridgeChild::DeallocPVideoBridgeChild()
{
  mIPDLSelfRef = nullptr;
}
=======
void VideoBridgeChild::ActorDealloc() { mIPDLSelfRef = nullptr; }
>>>>>>> upstream-releases

PTextureChild* VideoBridgeChild::CreateTexture(
    const SurfaceDescriptor& aSharedData, const ReadLockDescriptor& aReadLock,
    LayersBackend aLayersBackend, TextureFlags aFlags, uint64_t aSerial,
    wr::MaybeExternalImageId& aExternalImageId, nsIEventTarget* aTarget) {
  MOZ_ASSERT(CanSend());
  return SendPTextureConstructor(aSharedData, aReadLock, aLayersBackend, aFlags,
                                 aSerial);
}

bool VideoBridgeChild::IsSameProcess() const {
  return OtherPid() == base::GetCurrentProcId();
}

}  // namespace layers
}  // namespace mozilla
