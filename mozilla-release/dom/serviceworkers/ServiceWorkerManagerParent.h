/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_ServiceWorkerManagerParent_h
#define mozilla_dom_ServiceWorkerManagerParent_h

#include "mozilla/dom/PServiceWorkerManagerParent.h"

namespace mozilla {

class OriginAttributes;

namespace ipc {
class BackgroundParentImpl;
}  // namespace ipc

namespace dom {

class ServiceWorkerManagerService;

class ServiceWorkerManagerParent final : public PServiceWorkerManagerParent {
  friend class mozilla::ipc::BackgroundParentImpl;
  friend class PServiceWorkerManagerParent;

 public:
  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(ServiceWorkerManagerParent)

  uint64_t ID() const { return mID; }

 private:
  ServiceWorkerManagerParent();
  ~ServiceWorkerManagerParent();

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvRegister(
      const ServiceWorkerRegistrationData& aData) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvRegister(
    const ServiceWorkerRegistrationData& aData) override;
=======
  mozilla::ipc::IPCResult RecvRegister(
      const ServiceWorkerRegistrationData& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvUnregister(
      const PrincipalInfo& aPrincipalInfo, const nsString& aScope) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvUnregister(const PrincipalInfo& aPrincipalInfo,
                                                 const nsString& aScope) override;
=======
  mozilla::ipc::IPCResult RecvUnregister(const PrincipalInfo& aPrincipalInfo,
                                         const nsString& aScope);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPropagateSoftUpdate(
      const OriginAttributes& aOriginAttributes,
      const nsString& aScope) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvPropagateSoftUpdate(const OriginAttributes& aOriginAttributes,
                                                          const nsString& aScope) override;
=======
  mozilla::ipc::IPCResult RecvPropagateSoftUpdate(
      const OriginAttributes& aOriginAttributes, const nsString& aScope);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPropagateUnregister(
      const PrincipalInfo& aPrincipalInfo, const nsString& aScope) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvPropagateUnregister(const PrincipalInfo& aPrincipalInfo,
                                                          const nsString& aScope) override;
=======
  mozilla::ipc::IPCResult RecvPropagateUnregister(
      const PrincipalInfo& aPrincipalInfo, const nsString& aScope);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPropagateRemove(
      const nsCString& aHost) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvPropagateRemove(const nsCString& aHost) override;
=======
  mozilla::ipc::IPCResult RecvPropagateRemove(const nsCString& aHost);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvPropagateRemoveAll();

  mozilla::ipc::IPCResult RecvShutdown();

<<<<<<< HEAD
  virtual PServiceWorkerUpdaterParent* AllocPServiceWorkerUpdaterParent(
      const OriginAttributes& aOriginAttributes,
      const nsCString& aScope) override;
||||||| merged common ancestors
  virtual PServiceWorkerUpdaterParent*
  AllocPServiceWorkerUpdaterParent(const OriginAttributes& aOriginAttributes,
                                   const nsCString& aScope) override;
=======
  PServiceWorkerUpdaterParent* AllocPServiceWorkerUpdaterParent(
      const OriginAttributes& aOriginAttributes, const nsCString& aScope);
>>>>>>> upstream-releases

  virtual mozilla::ipc::IPCResult RecvPServiceWorkerUpdaterConstructor(
      PServiceWorkerUpdaterParent* aActor,
      const OriginAttributes& aOriginAttributes,
      const nsCString& aScope) override;

<<<<<<< HEAD
  virtual bool DeallocPServiceWorkerUpdaterParent(
      PServiceWorkerUpdaterParent* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPServiceWorkerUpdaterParent(PServiceWorkerUpdaterParent* aActor) override;
=======
  bool DeallocPServiceWorkerUpdaterParent(PServiceWorkerUpdaterParent* aActor);
>>>>>>> upstream-releases

  virtual void ActorDestroy(ActorDestroyReason aWhy) override;

  RefPtr<ServiceWorkerManagerService> mService;

  // We use this ID in the Service in order to avoid the sending of messages to
  // ourself.
  uint64_t mID;
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_ServiceWorkerManagerParent_h
