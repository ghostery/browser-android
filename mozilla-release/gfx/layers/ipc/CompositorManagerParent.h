/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_COMPOSITORMANAGERPARENT_H
#define MOZILLA_GFX_COMPOSITORMANAGERPARENT_H

#include <stdint.h>               // for uint32_t
#include "mozilla/Attributes.h"   // for override
#include "mozilla/StaticPtr.h"    // for StaticRefPtr
#include "mozilla/StaticMutex.h"  // for StaticMutex
#include "mozilla/RefPtr.h"       // for already_AddRefed
#include "mozilla/layers/PCompositorManagerParent.h"
#include "nsTArray.h"  // for AutoTArray

namespace mozilla {
namespace layers {

class CompositorBridgeParent;
class CompositorThreadHolder;

#ifndef DEBUG
#  define COMPOSITOR_MANAGER_PARENT_EXPLICIT_SHUTDOWN
#endif

class CompositorManagerParent final : public PCompositorManagerParent {
  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(CompositorManagerParent)

 public:
  static already_AddRefed<CompositorManagerParent> CreateSameProcess();
  static bool Create(Endpoint<PCompositorManagerParent>&& aEndpoint,
                     bool aIsRoot);
  static void Shutdown();

  static already_AddRefed<CompositorBridgeParent>
  CreateSameProcessWidgetCompositorBridge(CSSToLayoutDeviceScale aScale,
                                          const CompositorOptions& aOptions,
                                          bool aUseExternalSurfaceSize,
                                          const gfx::IntSize& aSurfaceSize);

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvAddSharedSurface(
      const wr::ExternalImageId& aId,
      const SurfaceDescriptorShared& aDesc) override;
  mozilla::ipc::IPCResult RecvRemoveSharedSurface(
      const wr::ExternalImageId& aId) override;
  mozilla::ipc::IPCResult RecvReportSharedSurfacesMemory(
      ReportSharedSurfacesMemoryResolver&&) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult RecvAddSharedSurface(const wr::ExternalImageId& aId,
                                               const SurfaceDescriptorShared& aDesc) override;
  mozilla::ipc::IPCResult RecvRemoveSharedSurface(const wr::ExternalImageId& aId) override;
  mozilla::ipc::IPCResult RecvReportSharedSurfacesMemory(ReportSharedSurfacesMemoryResolver&&) override;
=======
  mozilla::ipc::IPCResult RecvAddSharedSurface(
      const wr::ExternalImageId& aId, const SurfaceDescriptorShared& aDesc);
  mozilla::ipc::IPCResult RecvRemoveSharedSurface(
      const wr::ExternalImageId& aId);
  mozilla::ipc::IPCResult RecvReportSharedSurfacesMemory(
      ReportSharedSurfacesMemoryResolver&&);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvNotifyMemoryPressure();

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvReportMemory(
      ReportMemoryResolver&&) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvReportMemory(ReportMemoryResolver&&) override;
=======
  mozilla::ipc::IPCResult RecvReportMemory(ReportMemoryResolver&&);
>>>>>>> upstream-releases

  void BindComplete(bool aIsRoot);
  void ActorDestroy(ActorDestroyReason aReason) override;

<<<<<<< HEAD
  bool DeallocPCompositorBridgeParent(PCompositorBridgeParent* aActor) override;
  PCompositorBridgeParent* AllocPCompositorBridgeParent(
      const CompositorBridgeOptions& aOpt) override;
||||||| merged common ancestors
  bool DeallocPCompositorBridgeParent(PCompositorBridgeParent* aActor) override;
  PCompositorBridgeParent* AllocPCompositorBridgeParent(const CompositorBridgeOptions& aOpt) override;
=======
  bool DeallocPCompositorBridgeParent(PCompositorBridgeParent* aActor);
  PCompositorBridgeParent* AllocPCompositorBridgeParent(
      const CompositorBridgeOptions& aOpt);
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
||||||| merged common ancestors
private:
=======
  static void NotifyWebRenderError(wr::WebRenderError aError);

 private:
>>>>>>> upstream-releases
  static StaticRefPtr<CompositorManagerParent> sInstance;
  static StaticMutex sMutex;

#ifdef COMPOSITOR_MANAGER_PARENT_EXPLICIT_SHUTDOWN
  static StaticAutoPtr<nsTArray<CompositorManagerParent*>> sActiveActors;
  static void ShutdownInternal();
#endif

  CompositorManagerParent();
  virtual ~CompositorManagerParent();

  void Bind(Endpoint<PCompositorManagerParent>&& aEndpoint, bool aIsRoot);

  void ActorDealloc() override;

  void DeferredDestroy();

  RefPtr<CompositorThreadHolder> mCompositorThreadHolder;

  AutoTArray<RefPtr<CompositorBridgeParent>, 1> mPendingCompositorBridges;
};

}  // namespace layers
}  // namespace mozilla

#endif
