/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_MessagePortChild_h
#define mozilla_dom_MessagePortChild_h

#include "mozilla/Assertions.h"
#include "mozilla/dom/PMessagePortChild.h"
#include "nsISupportsImpl.h"

namespace mozilla {
namespace dom {

class MessagePort;

<<<<<<< HEAD
class MessagePortChild final : public PMessagePortChild {
 public:
||||||| merged common ancestors
class MessagePortChild final : public PMessagePortChild
{
public:
=======
class MessagePortChild final : public PMessagePortChild {
  friend class PMessagePortChild;

 public:
>>>>>>> upstream-releases
  NS_INLINE_DECL_REFCOUNTING(MessagePortChild)

  MessagePortChild();

  void SetPort(MessagePort* aPort) { mPort = aPort; }

 private:
  ~MessagePortChild() { MOZ_ASSERT(!mPort); }

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvEntangled(
      nsTArray<ClonedMessageData>&& aMessages) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvEntangled(nsTArray<ClonedMessageData>&& aMessages) override;
=======
  mozilla::ipc::IPCResult RecvEntangled(
      nsTArray<ClonedMessageData>&& aMessages);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvReceiveData(
      nsTArray<ClonedMessageData>&& aMessages) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvReceiveData(nsTArray<ClonedMessageData>&& aMessages) override;
=======
  mozilla::ipc::IPCResult RecvReceiveData(
      nsTArray<ClonedMessageData>&& aMessages);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvStopSendingDataConfirmed();

  virtual void ActorDestroy(ActorDestroyReason aWhy) override;

  // This is a raw pointer because this child is owned by this MessagePort.
  MessagePort* mPort;
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_MessagePortChild_h
