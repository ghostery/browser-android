/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef gfx_layers_ipc_ImageBridgeParent_h_
#define gfx_layers_ipc_ImageBridgeParent_h_

#include <stddef.h>  // for size_t
#include <stdint.h>  // for uint32_t, uint64_t
#include "CompositableTransactionParent.h"
#include "mozilla/Assertions.h"  // for MOZ_ASSERT_HELPER2
#include "mozilla/Attributes.h"  // for override
#include "mozilla/ipc/ProtocolUtils.h"
#include "mozilla/ipc/SharedMemory.h"  // for SharedMemory, etc
#include "mozilla/layers/CompositorThread.h"
#include "mozilla/layers/PImageBridgeParent.h"
#include "nsISupportsImpl.h"
#include "nsTArrayForwardDeclare.h"  // for InfallibleTArray

class MessageLoop;

namespace base {
class Thread;
}  // namespace base

namespace mozilla {
namespace ipc {
class Shmem;
}  // namespace ipc

namespace layers {

struct ImageCompositeNotificationInfo;

/**
 * ImageBridgeParent is the manager Protocol of async Compositables.
 */
class ImageBridgeParent final : public PImageBridgeParent,
                                public CompositableParentManager,
                                public ShmemAllocator {
 public:
  typedef InfallibleTArray<CompositableOperation> EditArray;
  typedef InfallibleTArray<OpDestroy> OpDestroyArray;

 protected:
  ImageBridgeParent(MessageLoop* aLoop, ProcessId aChildProcessId);

<<<<<<< HEAD
 public:
  ~ImageBridgeParent();
||||||| merged common ancestors
public:
  ~ImageBridgeParent();
=======
 public:
  virtual ~ImageBridgeParent();
>>>>>>> upstream-releases

  /**
   * Creates the globals of ImageBridgeParent.
   */
  static void Setup();

  static ImageBridgeParent* CreateSameProcess();
  static bool CreateForGPUProcess(Endpoint<PImageBridgeParent>&& aEndpoint);
  static bool CreateForContent(Endpoint<PImageBridgeParent>&& aEndpoint);
  static void Shutdown();

  ShmemAllocator* AsShmemAllocator() override { return this; }

  void ActorDestroy(ActorDestroyReason aWhy) override;

  // CompositableParentManager
<<<<<<< HEAD
  virtual void SendAsyncMessage(
      const InfallibleTArray<AsyncParentMessageData>& aMessage) override;
||||||| merged common ancestors
  virtual void SendAsyncMessage(const InfallibleTArray<AsyncParentMessageData>& aMessage) override;
=======
  void SendAsyncMessage(
      const InfallibleTArray<AsyncParentMessageData>& aMessage) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void NotifyNotUsed(PTextureParent* aTexture,
                             uint64_t aTransactionId) override;
||||||| merged common ancestors
  virtual void NotifyNotUsed(PTextureParent* aTexture, uint64_t aTransactionId) override;
=======
  void NotifyNotUsed(PTextureParent* aTexture,
                     uint64_t aTransactionId) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual base::ProcessId GetChildProcessId() override { return OtherPid(); }
||||||| merged common ancestors
  virtual base::ProcessId GetChildProcessId() override
  {
    return OtherPid();
  }
=======
  base::ProcessId GetChildProcessId() override { return OtherPid(); }
>>>>>>> upstream-releases

  // PImageBridge
<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvUpdate(
      EditArray&& aEdits, OpDestroyArray&& aToDestroy,
      const uint64_t& aFwdTransactionId) override;

  virtual PTextureParent* AllocPTextureParent(
      const SurfaceDescriptor& aSharedData, const ReadLockDescriptor& aReadLock,
      const LayersBackend& aLayersBackend, const TextureFlags& aFlags,
      const uint64_t& aSerial,
      const wr::MaybeExternalImageId& aExternalImageId) override;
  virtual bool DeallocPTextureParent(PTextureParent* actor) override;

  virtual mozilla::ipc::IPCResult RecvNewCompositable(
      const CompositableHandle& aHandle, const TextureInfo& aInfo,
      const LayersBackend& aLayersBackend) override;
  virtual mozilla::ipc::IPCResult RecvReleaseCompositable(
      const CompositableHandle& aHandle) override;

  PMediaSystemResourceManagerParent* AllocPMediaSystemResourceManagerParent()
      override;
  bool DeallocPMediaSystemResourceManagerParent(
      PMediaSystemResourceManagerParent* aActor) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvUpdate(EditArray&& aEdits, OpDestroyArray&& aToDestroy,
                                          const uint64_t& aFwdTransactionId) override;

  virtual PTextureParent* AllocPTextureParent(const SurfaceDescriptor& aSharedData,
                                              const ReadLockDescriptor& aReadLock,
                                              const LayersBackend& aLayersBackend,
                                              const TextureFlags& aFlags,
                                              const uint64_t& aSerial,
                                              const wr::MaybeExternalImageId& aExternalImageId) override;
  virtual bool DeallocPTextureParent(PTextureParent* actor) override;

  virtual mozilla::ipc::IPCResult RecvNewCompositable(const CompositableHandle& aHandle,
                                                      const TextureInfo& aInfo,
                                                      const LayersBackend& aLayersBackend) override;
  virtual mozilla::ipc::IPCResult RecvReleaseCompositable(const CompositableHandle& aHandle) override;

  PMediaSystemResourceManagerParent* AllocPMediaSystemResourceManagerParent() override;
  bool DeallocPMediaSystemResourceManagerParent(PMediaSystemResourceManagerParent* aActor) override;
=======
  mozilla::ipc::IPCResult RecvUpdate(EditArray&& aEdits,
                                     OpDestroyArray&& aToDestroy,
                                     const uint64_t& aFwdTransactionId);

  PTextureParent* AllocPTextureParent(
      const SurfaceDescriptor& aSharedData, const ReadLockDescriptor& aReadLock,
      const LayersBackend& aLayersBackend, const TextureFlags& aFlags,
      const uint64_t& aSerial,
      const wr::MaybeExternalImageId& aExternalImageId);
  bool DeallocPTextureParent(PTextureParent* actor);

  mozilla::ipc::IPCResult RecvNewCompositable(
      const CompositableHandle& aHandle, const TextureInfo& aInfo,
      const LayersBackend& aLayersBackend);
  mozilla::ipc::IPCResult RecvReleaseCompositable(
      const CompositableHandle& aHandle);

  PMediaSystemResourceManagerParent* AllocPMediaSystemResourceManagerParent();
  bool DeallocPMediaSystemResourceManagerParent(
      PMediaSystemResourceManagerParent* aActor);
>>>>>>> upstream-releases

  // Shutdown step 1
  mozilla::ipc::IPCResult RecvWillClose();

  MessageLoop* GetMessageLoop() const { return mMessageLoop; }

  // ShmemAllocator

  bool AllocShmem(size_t aSize, ipc::SharedMemory::SharedMemoryType aType,
                  ipc::Shmem* aShmem) override;

  bool AllocUnsafeShmem(size_t aSize, ipc::SharedMemory::SharedMemoryType aType,
                        ipc::Shmem* aShmem) override;

  void DeallocShmem(ipc::Shmem& aShmem) override;

  bool IsSameProcess() const override;

  static already_AddRefed<ImageBridgeParent> GetInstance(ProcessId aId);

  static bool NotifyImageComposites(
      nsTArray<ImageCompositeNotificationInfo>& aNotifications);

  bool UsesImageBridge() const override { return true; }

  bool IPCOpen() const override { return !mClosed; }

 protected:
  void Bind(Endpoint<PImageBridgeParent>&& aEndpoint);

 private:
  static void ShutdownInternal();

  void DeferredDestroy();
  MessageLoop* mMessageLoop;
  // This keeps us alive until ActorDestroy(), at which point we do a
  // deferred destruction of ourselves.
  RefPtr<ImageBridgeParent> mSelfRef;

  bool mClosed;

  /**
   * Map of all living ImageBridgeParent instances
   */
  typedef std::map<base::ProcessId, ImageBridgeParent*> ImageBridgeMap;
  static ImageBridgeMap sImageBridges;

  RefPtr<CompositorThreadHolder> mCompositorThreadHolder;
};

}  // namespace layers
}  // namespace mozilla

#endif  // gfx_layers_ipc_ImageBridgeParent_h_
