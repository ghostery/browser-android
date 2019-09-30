/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ClientOpenWindowOpChild.h"
#include "ClientOpenWindowUtils.h"
#include "mozilla/SystemGroup.h"

namespace mozilla {
namespace dom {

<<<<<<< HEAD
already_AddRefed<ClientOpPromise> ClientOpenWindowOpChild::DoOpenWindow(
    const ClientOpenWindowArgs& aArgs) {
  RefPtr<ClientOpPromise> ref = ClientOpenWindowInCurrentProcess(aArgs);
  return ref.forget();
||||||| merged common ancestors
already_AddRefed<ClientOpPromise>
ClientOpenWindowOpChild::DoOpenWindow(const ClientOpenWindowArgs& aArgs)
{
  RefPtr<ClientOpPromise> ref =
    ClientOpenWindowInCurrentProcess(aArgs);
  return ref.forget();
=======
RefPtr<ClientOpPromise> ClientOpenWindowOpChild::DoOpenWindow(
    const ClientOpenWindowArgs& aArgs) {
  return ClientOpenWindowInCurrentProcess(aArgs);
>>>>>>> upstream-releases
}

void ClientOpenWindowOpChild::ActorDestroy(ActorDestroyReason aReason) {
  mPromiseRequestHolder.DisconnectIfExists();
}

<<<<<<< HEAD
void ClientOpenWindowOpChild::Init(const ClientOpenWindowArgs& aArgs) {
  RefPtr<ClientOpPromise> promise = DoOpenWindow(aArgs);
  promise
      ->Then(SystemGroup::EventTargetFor(TaskCategory::Other), __func__,
             [this](const ClientOpResult& aResult) {
               mPromiseRequestHolder.Complete();
               PClientOpenWindowOpChild::Send__delete__(this, aResult);
             },
             [this](nsresult aResult) {
               mPromiseRequestHolder.Complete();
               PClientOpenWindowOpChild::Send__delete__(this, aResult);
             })
      ->Track(mPromiseRequestHolder);
||||||| merged common ancestors
void
ClientOpenWindowOpChild::Init(const ClientOpenWindowArgs& aArgs)
{
  RefPtr<ClientOpPromise> promise = DoOpenWindow(aArgs);
  promise->Then(SystemGroup::EventTargetFor(TaskCategory::Other), __func__,
    [this] (const ClientOpResult& aResult) {
      mPromiseRequestHolder.Complete();
      PClientOpenWindowOpChild::Send__delete__(this, aResult);
    }, [this] (nsresult aResult) {
      mPromiseRequestHolder.Complete();
      PClientOpenWindowOpChild::Send__delete__(this, aResult);
  })->Track(mPromiseRequestHolder);
=======
void ClientOpenWindowOpChild::Init(const ClientOpenWindowArgs& aArgs) {
  DoOpenWindow(aArgs)
      ->Then(
          SystemGroup::EventTargetFor(TaskCategory::Other), __func__,
          [this](const ClientOpResult& aResult) {
            mPromiseRequestHolder.Complete();
            PClientOpenWindowOpChild::Send__delete__(this, aResult);
          },
          [this](nsresult aResult) {
            mPromiseRequestHolder.Complete();
            PClientOpenWindowOpChild::Send__delete__(this, aResult);
          })
      ->Track(mPromiseRequestHolder);
>>>>>>> upstream-releases
}

}  // namespace dom
}  // namespace mozilla
