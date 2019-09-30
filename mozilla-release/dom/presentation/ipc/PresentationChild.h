/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_PresentationChild_h
#define mozilla_dom_PresentationChild_h

#include "mozilla/dom/PPresentationBuilderChild.h"
#include "mozilla/dom/PPresentationChild.h"
#include "mozilla/dom/PPresentationRequestChild.h"

class nsIPresentationServiceCallback;

namespace mozilla {
namespace dom {

class PresentationIPCService;

class PresentationChild final : public PPresentationChild {
 public:
  explicit PresentationChild(PresentationIPCService* aService);

  virtual void ActorDestroy(ActorDestroyReason aWhy) override;

<<<<<<< HEAD
  virtual PPresentationRequestChild* AllocPPresentationRequestChild(
      const PresentationIPCRequest& aRequest) override;
||||||| merged common ancestors
  virtual PPresentationRequestChild*
  AllocPPresentationRequestChild(const PresentationIPCRequest& aRequest) override;
=======
  PPresentationRequestChild* AllocPPresentationRequestChild(
      const PresentationIPCRequest& aRequest);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPPresentationRequestChild(
      PPresentationRequestChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPPresentationRequestChild(PPresentationRequestChild* aActor) override;
=======
  bool DeallocPPresentationRequestChild(PPresentationRequestChild* aActor);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvPPresentationBuilderConstructor(
      PPresentationBuilderChild* aActor, const nsString& aSessionId,
      const uint8_t& aRole) override;

<<<<<<< HEAD
  virtual PPresentationBuilderChild* AllocPPresentationBuilderChild(
      const nsString& aSessionId, const uint8_t& aRole) override;
||||||| merged common ancestors
  virtual PPresentationBuilderChild*
  AllocPPresentationBuilderChild(const nsString& aSessionId, const uint8_t& aRole) override;
=======
  PPresentationBuilderChild* AllocPPresentationBuilderChild(
      const nsString& aSessionId, const uint8_t& aRole);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPPresentationBuilderChild(
      PPresentationBuilderChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPPresentationBuilderChild(PPresentationBuilderChild* aActor) override;
=======
  bool DeallocPPresentationBuilderChild(PPresentationBuilderChild* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyAvailableChange(
      nsTArray<nsString>&& aAvailabilityUrls, const bool& aAvailable) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvNotifyAvailableChange(nsTArray<nsString>&& aAvailabilityUrls,
                            const bool& aAvailable) override;
=======
  mozilla::ipc::IPCResult RecvNotifyAvailableChange(
      nsTArray<nsString>&& aAvailabilityUrls, const bool& aAvailable);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifySessionStateChange(
      const nsString& aSessionId, const uint16_t& aState,
      const nsresult& aReason) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvNotifySessionStateChange(const nsString& aSessionId,
                               const uint16_t& aState,
                               const nsresult& aReason) override;
=======
  mozilla::ipc::IPCResult RecvNotifySessionStateChange(
      const nsString& aSessionId, const uint16_t& aState,
      const nsresult& aReason);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyMessage(
      const nsString& aSessionId, const nsCString& aData,
      const bool& aIsBinary) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvNotifyMessage(const nsString& aSessionId,
                    const nsCString& aData,
                    const bool& aIsBinary) override;
=======
  mozilla::ipc::IPCResult RecvNotifyMessage(const nsString& aSessionId,
                                            const nsCString& aData,
                                            const bool& aIsBinary);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifySessionConnect(
      const uint64_t& aWindowId, const nsString& aSessionId) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvNotifySessionConnect(const uint64_t& aWindowId,
                           const nsString& aSessionId) override;
=======
  mozilla::ipc::IPCResult RecvNotifySessionConnect(const uint64_t& aWindowId,
                                                   const nsString& aSessionId);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyCloseSessionTransport(
      const nsString& aSessionId, const uint8_t& aRole,
      const nsresult& aReason) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvNotifyCloseSessionTransport(const nsString& aSessionId,
                                  const uint8_t& aRole,
                                  const nsresult& aReason) override;
=======
  mozilla::ipc::IPCResult RecvNotifyCloseSessionTransport(
      const nsString& aSessionId, const uint8_t& aRole,
      const nsresult& aReason);
>>>>>>> upstream-releases

 private:
  virtual ~PresentationChild();

  bool mActorDestroyed = false;
  RefPtr<PresentationIPCService> mService;
};

class PresentationRequestChild final : public PPresentationRequestChild {
  friend class PresentationChild;

 public:
  explicit PresentationRequestChild(nsIPresentationServiceCallback* aCallback);

  virtual void ActorDestroy(ActorDestroyReason aWhy) override;

  virtual mozilla::ipc::IPCResult Recv__delete__(
      const nsresult& aResult) override;

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyRequestUrlSelected(
      const nsString& aUrl) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvNotifyRequestUrlSelected(const nsString& aUrl) override;
=======
  mozilla::ipc::IPCResult RecvNotifyRequestUrlSelected(const nsString& aUrl);
>>>>>>> upstream-releases

 private:
  virtual ~PresentationRequestChild();

  bool mActorDestroyed = false;
  nsCOMPtr<nsIPresentationServiceCallback> mCallback;
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_PresentationChild_h
