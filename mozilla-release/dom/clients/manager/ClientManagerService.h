/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef _mozilla_dom_ClientManagerService_h
#define _mozilla_dom_ClientManagerService_h

#include "ClientOpPromise.h"
#include "nsDataHashtable.h"

namespace mozilla {

namespace ipc {

class PrincipalInfo;

}  // namespace ipc

namespace dom {

class ClientManagerParent;
class ClientSourceParent;
class ContentParent;

// Define a singleton service to manage client activity throughout the
// browser.  This service runs on the PBackground thread.  To interact
// it with it please use the ClientManager and ClientHandle classes.
class ClientManagerService final {
  // Store the ClientSourceParent objects in a hash table.  We want to
  // optimize for insertion, removal, and lookup by UUID.
  nsDataHashtable<nsIDHashKey, ClientSourceParent*> mSourceTable;

  nsTArray<ClientManagerParent*> mManagerList;

  bool mShutdown;

  ClientManagerService();
  ~ClientManagerService();

  void Shutdown();

 public:
  static already_AddRefed<ClientManagerService> GetOrCreateInstance();

  // Returns nullptr if the service is not already created.
<<<<<<< HEAD
  static already_AddRefed<ClientManagerService> GetInstance();
||||||| merged common ancestors
  static already_AddRefed<ClientManagerService>
  GetInstance();
=======
  static already_AddRefed<ClientManagerService> GetInstance();

  bool AddSource(ClientSourceParent* aSource);
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool AddSource(ClientSourceParent* aSource);
||||||| merged common ancestors
  bool
  AddSource(ClientSourceParent* aSource);
=======
  bool RemoveSource(ClientSourceParent* aSource);
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool RemoveSource(ClientSourceParent* aSource);
||||||| merged common ancestors
  bool
  RemoveSource(ClientSourceParent* aSource);
=======
  ClientSourceParent* FindSource(
      const nsID& aID, const mozilla::ipc::PrincipalInfo& aPrincipalInfo);
>>>>>>> upstream-releases

<<<<<<< HEAD
  ClientSourceParent* FindSource(
      const nsID& aID, const mozilla::ipc::PrincipalInfo& aPrincipalInfo);
||||||| merged common ancestors
  ClientSourceParent*
  FindSource(const nsID& aID,
             const mozilla::ipc::PrincipalInfo& aPrincipalInfo);
=======
  void AddManager(ClientManagerParent* aManager);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void AddManager(ClientManagerParent* aManager);
||||||| merged common ancestors
  void
  AddManager(ClientManagerParent* aManager);
=======
  void RemoveManager(ClientManagerParent* aManager);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void RemoveManager(ClientManagerParent* aManager);
||||||| merged common ancestors
  void
  RemoveManager(ClientManagerParent* aManager);
=======
  RefPtr<ClientOpPromise> Navigate(const ClientNavigateArgs& aArgs);
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<ClientOpPromise> Navigate(const ClientNavigateArgs& aArgs);
||||||| merged common ancestors
  RefPtr<ClientOpPromise>
  Navigate(const ClientNavigateArgs& aArgs);
=======
  RefPtr<ClientOpPromise> MatchAll(const ClientMatchAllArgs& aArgs);
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<ClientOpPromise> MatchAll(const ClientMatchAllArgs& aArgs);
||||||| merged common ancestors
  RefPtr<ClientOpPromise>
  MatchAll(const ClientMatchAllArgs& aArgs);
=======
  RefPtr<ClientOpPromise> Claim(const ClientClaimArgs& aArgs);
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<ClientOpPromise> Claim(const ClientClaimArgs& aArgs);
||||||| merged common ancestors
  RefPtr<ClientOpPromise>
  Claim(const ClientClaimArgs& aArgs);
=======
  RefPtr<ClientOpPromise> GetInfoAndState(
      const ClientGetInfoAndStateArgs& aArgs);
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<ClientOpPromise> GetInfoAndState(
      const ClientGetInfoAndStateArgs& aArgs);
||||||| merged common ancestors
  RefPtr<ClientOpPromise>
  GetInfoAndState(const ClientGetInfoAndStateArgs& aArgs);
=======
  RefPtr<ClientOpPromise> OpenWindow(
      const ClientOpenWindowArgs& aArgs,
      already_AddRefed<ContentParent> aSourceProcess);
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<ClientOpPromise> OpenWindow(
      const ClientOpenWindowArgs& aArgs,
      already_AddRefed<ContentParent> aSourceProcess);
||||||| merged common ancestors
  RefPtr<ClientOpPromise>
  OpenWindow(const ClientOpenWindowArgs& aArgs,
             already_AddRefed<ContentParent> aSourceProcess);
=======
  bool HasWindow(const Maybe<ContentParentId>& aContentParentId,
                 const mozilla::ipc::PrincipalInfo& aPrincipalInfo,
                 const nsID& aClientId);
>>>>>>> upstream-releases

  NS_INLINE_DECL_REFCOUNTING(mozilla::dom::ClientManagerService)
};

}  // namespace dom
}  // namespace mozilla

#endif  // _mozilla_dom_ClientManagerService_h
