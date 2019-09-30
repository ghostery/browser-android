/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_ServiceWorkerManagerChild_h
#define mozilla_dom_ServiceWorkerManagerChild_h

#include "mozilla/dom/PServiceWorkerManagerChild.h"
#include "mozilla/ipc/BackgroundUtils.h"

namespace mozilla {

class OriginAttributes;

namespace ipc {
class BackgroundChildImpl;
}  // namespace ipc

namespace dom {

class ServiceWorkerManagerChild final : public PServiceWorkerManagerChild {
  friend class mozilla::ipc::BackgroundChildImpl;

 public:
  NS_INLINE_DECL_REFCOUNTING(ServiceWorkerManagerChild)

  void ManagerShuttingDown() { mShuttingDown = true; }

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyRegister(
      const ServiceWorkerRegistrationData& aData) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifyRegister(const ServiceWorkerRegistrationData& aData)
                                                                       override;
=======
  mozilla::ipc::IPCResult RecvNotifyRegister(
      const ServiceWorkerRegistrationData& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifySoftUpdate(
      const OriginAttributes& aOriginAttributes,
      const nsString& aScope) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifySoftUpdate(const OriginAttributes& aOriginAttributes,
                                                       const nsString& aScope) override;
=======
  mozilla::ipc::IPCResult RecvNotifySoftUpdate(
      const OriginAttributes& aOriginAttributes, const nsString& aScope);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyUnregister(
      const PrincipalInfo& aPrincipalInfo, const nsString& aScope) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifyUnregister(const PrincipalInfo& aPrincipalInfo,
                                                       const nsString& aScope) override;
=======
  mozilla::ipc::IPCResult RecvNotifyUnregister(
      const PrincipalInfo& aPrincipalInfo, const nsString& aScope);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyRemove(
      const nsCString& aHost) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifyRemove(const nsCString& aHost) override;
=======
  mozilla::ipc::IPCResult RecvNotifyRemove(const nsCString& aHost);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvNotifyRemoveAll();

<<<<<<< HEAD
  virtual PServiceWorkerUpdaterChild* AllocPServiceWorkerUpdaterChild(
      const OriginAttributes& originAttributes,
      const nsCString& scope) override;
||||||| merged common ancestors
  virtual PServiceWorkerUpdaterChild*
  AllocPServiceWorkerUpdaterChild(const OriginAttributes& originAttributes,
                                  const nsCString& scope) override;
=======
  PServiceWorkerUpdaterChild* AllocPServiceWorkerUpdaterChild(
      const OriginAttributes& originAttributes, const nsCString& scope);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPServiceWorkerUpdaterChild(
      PServiceWorkerUpdaterChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPServiceWorkerUpdaterChild(PServiceWorkerUpdaterChild* aActor) override;
=======
  bool DeallocPServiceWorkerUpdaterChild(PServiceWorkerUpdaterChild* aActor);
>>>>>>> upstream-releases

 private:
  ServiceWorkerManagerChild() : mShuttingDown(false) {}

  ~ServiceWorkerManagerChild() {}

  bool mShuttingDown;
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_ServiceWorkerManagerChild_h
