/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_PresentationParent_h__
#define mozilla_dom_PresentationParent_h__

#include "mozilla/dom/ipc/IdType.h"
#include "mozilla/dom/PPresentationBuilderParent.h"
#include "mozilla/dom/PPresentationParent.h"
#include "mozilla/dom/PPresentationRequestParent.h"
#include "nsIPresentationListener.h"
#include "nsIPresentationService.h"
#include "nsIPresentationSessionTransportBuilder.h"

namespace mozilla {
namespace dom {

class PresentationParent final : public PPresentationParent,
                                 public nsIPresentationAvailabilityListener,
                                 public nsIPresentationSessionListener,
                                 public nsIPresentationRespondingListener {
 public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPRESENTATIONAVAILABILITYLISTENER
  NS_DECL_NSIPRESENTATIONSESSIONLISTENER
  NS_DECL_NSIPRESENTATIONRESPONDINGLISTENER

  PresentationParent();

  bool Init(ContentParentId aContentParentId);

  bool RegisterTransportBuilder(const nsString& aSessionId,
                                const uint8_t& aRole);

  virtual void ActorDestroy(ActorDestroyReason aWhy) override;

  virtual mozilla::ipc::IPCResult RecvPPresentationRequestConstructor(
      PPresentationRequestParent* aActor,
      const PresentationIPCRequest& aRequest) override;

<<<<<<< HEAD
  virtual PPresentationRequestParent* AllocPPresentationRequestParent(
      const PresentationIPCRequest& aRequest) override;
||||||| merged common ancestors
  virtual PPresentationRequestParent*
  AllocPPresentationRequestParent(const PresentationIPCRequest& aRequest) override;
=======
  PPresentationRequestParent* AllocPPresentationRequestParent(
      const PresentationIPCRequest& aRequest);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPPresentationRequestParent(
      PPresentationRequestParent* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPPresentationRequestParent(PPresentationRequestParent* aActor) override;
=======
  bool DeallocPPresentationRequestParent(PPresentationRequestParent* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PPresentationBuilderParent* AllocPPresentationBuilderParent(
      const nsString& aSessionId, const uint8_t& aRole) override;
||||||| merged common ancestors
  virtual PPresentationBuilderParent*
  AllocPPresentationBuilderParent(const nsString& aSessionId,
                                  const uint8_t& aRole) override;
=======
  PPresentationBuilderParent* AllocPPresentationBuilderParent(
      const nsString& aSessionId, const uint8_t& aRole);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPPresentationBuilderParent(
      PPresentationBuilderParent* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPPresentationBuilderParent(
    PPresentationBuilderParent* aActor) override;
=======
  bool DeallocPPresentationBuilderParent(PPresentationBuilderParent* aActor);
>>>>>>> upstream-releases

  virtual mozilla::ipc::IPCResult Recv__delete__() override;

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvRegisterAvailabilityHandler(
      nsTArray<nsString>&& aAvailabilityUrls) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvRegisterAvailabilityHandler(
    nsTArray<nsString>&& aAvailabilityUrls) override;
=======
  mozilla::ipc::IPCResult RecvRegisterAvailabilityHandler(
      nsTArray<nsString>&& aAvailabilityUrls);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvUnregisterAvailabilityHandler(
      nsTArray<nsString>&& aAvailabilityUrls) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvUnregisterAvailabilityHandler(
    nsTArray<nsString>&& aAvailabilityUrls) override;
=======
  mozilla::ipc::IPCResult RecvUnregisterAvailabilityHandler(
      nsTArray<nsString>&& aAvailabilityUrls);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvRegisterSessionHandler(
      const nsString& aSessionId, const uint8_t& aRole) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvRegisterSessionHandler(const nsString& aSessionId,
                                                             const uint8_t& aRole) override;
=======
  mozilla::ipc::IPCResult RecvRegisterSessionHandler(const nsString& aSessionId,
                                                     const uint8_t& aRole);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvUnregisterSessionHandler(
      const nsString& aSessionId, const uint8_t& aRole) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvUnregisterSessionHandler(const nsString& aSessionId,
                                                               const uint8_t& aRole) override;
=======
  mozilla::ipc::IPCResult RecvUnregisterSessionHandler(
      const nsString& aSessionId, const uint8_t& aRole);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvRegisterRespondingHandler(
      const uint64_t& aWindowId) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvRegisterRespondingHandler(const uint64_t& aWindowId) override;
=======
  mozilla::ipc::IPCResult RecvRegisterRespondingHandler(
      const uint64_t& aWindowId);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvUnregisterRespondingHandler(
      const uint64_t& aWindowId) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvUnregisterRespondingHandler(const uint64_t& aWindowId) override;
=======
  mozilla::ipc::IPCResult RecvUnregisterRespondingHandler(
      const uint64_t& aWindowId);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyReceiverReady(
      const nsString& aSessionId, const uint64_t& aWindowId,
      const bool& aIsLoading) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifyReceiverReady(const nsString& aSessionId,
                                                          const uint64_t& aWindowId,
                                                          const bool& aIsLoading) override;
=======
  mozilla::ipc::IPCResult RecvNotifyReceiverReady(const nsString& aSessionId,
                                                  const uint64_t& aWindowId,
                                                  const bool& aIsLoading);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyTransportClosed(
      const nsString& aSessionId, const uint8_t& aRole,
      const nsresult& aReason) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifyTransportClosed(const nsString& aSessionId,
                                                            const uint8_t& aRole,
                                                            const nsresult& aReason) override;
=======
  mozilla::ipc::IPCResult RecvNotifyTransportClosed(const nsString& aSessionId,
                                                    const uint8_t& aRole,
                                                    const nsresult& aReason);
>>>>>>> upstream-releases

 private:
  virtual ~PresentationParent();

  bool mActorDestroyed = false;
  nsCOMPtr<nsIPresentationService> mService;
  nsTArray<nsString> mSessionIdsAtController;
  nsTArray<nsString> mSessionIdsAtReceiver;
  nsTArray<uint64_t> mWindowIds;
  ContentParentId mChildId;
  nsTArray<nsString> mContentAvailabilityUrls;
};

class PresentationRequestParent final : public PPresentationRequestParent,
                                        public nsIPresentationServiceCallback {
  friend class PresentationParent;

 public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPRESENTATIONSERVICECALLBACK

  explicit PresentationRequestParent(nsIPresentationService* aService,
                                     ContentParentId aContentParentId);

  virtual void ActorDestroy(ActorDestroyReason aWhy) override;

 private:
  virtual ~PresentationRequestParent();

  nsresult SendResponse(nsresult aResult);

  nsresult DoRequest(const StartSessionRequest& aRequest);

  nsresult DoRequest(const SendSessionMessageRequest& aRequest);

  nsresult DoRequest(const CloseSessionRequest& aRequest);

  nsresult DoRequest(const TerminateSessionRequest& aRequest);

  nsresult DoRequest(const ReconnectSessionRequest& aRequest);

  nsresult DoRequest(const BuildTransportRequest& aRequest);

  bool mActorDestroyed = false;
  bool mNeedRegisterBuilder = false;
  nsString mSessionId;
  nsCOMPtr<nsIPresentationService> mService;
  ContentParentId mChildId;
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_PresentationParent_h__
