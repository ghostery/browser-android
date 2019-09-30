/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_PresentationBuilderParent_h__
#define mozilla_dom_PresentationBuilderParent_h__

#include "mozilla/dom/PPresentationBuilderParent.h"
#include "PresentationParent.h"
#include "nsIPresentationSessionTransportBuilder.h"

namespace mozilla {
namespace dom {

class PresentationBuilderParent final
    : public PPresentationBuilderParent,
      public nsIPresentationDataChannelSessionTransportBuilder {
 public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPRESENTATIONSESSIONTRANSPORTBUILDER
  NS_DECL_NSIPRESENTATIONDATACHANNELSESSIONTRANSPORTBUILDER

  explicit PresentationBuilderParent(PresentationParent* aParent);

  mozilla::ipc::IPCResult RecvSendOffer(const nsString& aSDP);

  mozilla::ipc::IPCResult RecvSendAnswer(const nsString& aSDP);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvSendIceCandidate(
      const nsString& aCandidate) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvSendIceCandidate(const nsString& aCandidate) override;
=======
  mozilla::ipc::IPCResult RecvSendIceCandidate(const nsString& aCandidate);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvClose(const nsresult& aReason);

  virtual void ActorDestroy(ActorDestroyReason aWhy) override;

  mozilla::ipc::IPCResult RecvOnSessionTransport();

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvOnSessionTransportError(
      const nsresult& aReason) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvOnSessionTransportError(const nsresult& aReason) override;
=======
  mozilla::ipc::IPCResult RecvOnSessionTransportError(const nsresult& aReason);
>>>>>>> upstream-releases

 private:
  virtual ~PresentationBuilderParent();
  bool mNeedDestroyActor = false;
  RefPtr<PresentationParent> mParent;
  nsCOMPtr<nsIPresentationSessionTransportBuilderListener> mBuilderListener;
  nsCOMPtr<nsIPresentationSessionTransport> mIPCSessionTransport;
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_PresentationBuilderParent_h__
