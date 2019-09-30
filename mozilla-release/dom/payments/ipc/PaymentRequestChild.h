/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_PaymentRequestChild_h
#define mozilla_dom_PaymentRequestChild_h

#include "mozilla/dom/PPaymentRequestChild.h"

namespace mozilla {
namespace dom {

class PaymentRequest;

<<<<<<< HEAD
class PaymentRequestChild final : public PPaymentRequestChild {
 public:
||||||| merged common ancestors
class PaymentRequestChild final : public PPaymentRequestChild
{
public:
=======
class PaymentRequestChild final : public PPaymentRequestChild {
  friend class PPaymentRequestChild;

 public:
>>>>>>> upstream-releases
  explicit PaymentRequestChild(PaymentRequest* aRequest);

  void MaybeDelete(bool aCanBeInManager);

  nsresult RequestPayment(const IPCPaymentActionRequest& aAction);

<<<<<<< HEAD
 protected:
  mozilla::ipc::IPCResult RecvRespondPayment(
      const IPCPaymentActionResponse& aResponse) override;
||||||| merged common ancestors
protected:
  mozilla::ipc::IPCResult
  RecvRespondPayment(const IPCPaymentActionResponse& aResponse) override;
=======
 protected:
  mozilla::ipc::IPCResult RecvRespondPayment(
      const IPCPaymentActionResponse& aResponse);

  mozilla::ipc::IPCResult RecvChangeShippingAddress(
      const nsString& aRequestId, const IPCPaymentAddress& aAddress);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvChangeShippingAddress(
      const nsString& aRequestId, const IPCPaymentAddress& aAddress) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvChangeShippingAddress(const nsString& aRequestId,
                            const IPCPaymentAddress& aAddress) override;
=======
  mozilla::ipc::IPCResult RecvChangeShippingOption(const nsString& aRequestId,
                                                   const nsString& aOption);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvChangeShippingOption(
      const nsString& aRequestId, const nsString& aOption) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvChangeShippingOption(const nsString& aRequestId,
                           const nsString& aOption) override;
=======
  mozilla::ipc::IPCResult RecvChangePayerDetail(const nsString& aRequestId,
                                                const nsString& aPayerName,
                                                const nsString& aPayerEmail,
                                                const nsString& aPayerPhone);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvChangePayerDetail(
      const nsString& aRequestId, const nsString& aPayerName,
      const nsString& aPayerEmail, const nsString& aPayerPhone) override;

  mozilla::ipc::IPCResult RecvChangePaymentMethod(
      const nsString& aRequestId, const nsString& aMethodName,
      const IPCMethodChangeDetails& aMethodDetails) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvChangePayerDetail(const nsString& aRequestId,
                        const nsString& aPayerName,
                        const nsString& aPayerEmail,
                        const nsString& aPayerPhone) override;
=======
  mozilla::ipc::IPCResult RecvChangePaymentMethod(
      const nsString& aRequestId, const nsString& aMethodName,
      const IPCMethodChangeDetails& aMethodDetails);
>>>>>>> upstream-releases

  void ActorDestroy(ActorDestroyReason aWhy) override;

 private:
  ~PaymentRequestChild() = default;

  void DetachFromRequest(bool aCanBeInManager);

  PaymentRequest* MOZ_NON_OWNING_REF mRequest;
};

}  // end of namespace dom
}  // end of namespace mozilla

#endif
