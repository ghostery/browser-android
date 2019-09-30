/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_indexeddb_actorsparent_h__
#define mozilla_dom_indexeddb_actorsparent_h__

<<<<<<< HEAD
template <class>
struct already_AddRefed;
||||||| merged common ancestors
template <class> struct already_AddRefed;
=======
#include "nscore.h"

#include <stdint.h>

#include "mozilla/dom/indexedDB/PermissionRequestBase.h"
#include "mozilla/dom/PBrowserParent.h"

template <class>
struct already_AddRefed;
>>>>>>> upstream-releases
class nsIPrincipal;

namespace mozilla {
namespace dom {

class Element;
class FileHandleThreadPool;

namespace quota {

class Client;

}  // namespace quota

namespace indexedDB {

class FileManager;
class LoggingInfo;
class PBackgroundIDBFactoryParent;
class PBackgroundIndexedDBUtilsParent;

<<<<<<< HEAD
PBackgroundIDBFactoryParent* AllocPBackgroundIDBFactoryParent(
    const LoggingInfo& aLoggingInfo);
||||||| merged common ancestors
PBackgroundIDBFactoryParent*
AllocPBackgroundIDBFactoryParent(const LoggingInfo& aLoggingInfo);
=======
PBackgroundIDBFactoryParent* AllocPBackgroundIDBFactoryParent(
    const LoggingInfo& aLoggingInfo);

bool RecvPBackgroundIDBFactoryConstructor(PBackgroundIDBFactoryParent* aActor,
                                          const LoggingInfo& aLoggingInfo);

bool DeallocPBackgroundIDBFactoryParent(PBackgroundIDBFactoryParent* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool RecvPBackgroundIDBFactoryConstructor(PBackgroundIDBFactoryParent* aActor,
                                          const LoggingInfo& aLoggingInfo);
||||||| merged common ancestors
bool
RecvPBackgroundIDBFactoryConstructor(PBackgroundIDBFactoryParent* aActor,
                                     const LoggingInfo& aLoggingInfo);
=======
PBackgroundIndexedDBUtilsParent* AllocPBackgroundIndexedDBUtilsParent();
>>>>>>> upstream-releases

<<<<<<< HEAD
bool DeallocPBackgroundIDBFactoryParent(PBackgroundIDBFactoryParent* aActor);
||||||| merged common ancestors
bool
DeallocPBackgroundIDBFactoryParent(PBackgroundIDBFactoryParent* aActor);
=======
bool DeallocPBackgroundIndexedDBUtilsParent(
    PBackgroundIndexedDBUtilsParent* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
PBackgroundIndexedDBUtilsParent* AllocPBackgroundIndexedDBUtilsParent();
||||||| merged common ancestors
PBackgroundIndexedDBUtilsParent*
AllocPBackgroundIndexedDBUtilsParent();
=======
bool RecvFlushPendingFileDeletions();
>>>>>>> upstream-releases

<<<<<<< HEAD
bool DeallocPBackgroundIndexedDBUtilsParent(
    PBackgroundIndexedDBUtilsParent* aActor);
||||||| merged common ancestors
bool
DeallocPBackgroundIndexedDBUtilsParent(PBackgroundIndexedDBUtilsParent* aActor);
=======
already_AddRefed<mozilla::dom::quota::Client> CreateQuotaClient();
>>>>>>> upstream-releases

<<<<<<< HEAD
bool RecvFlushPendingFileDeletions();
||||||| merged common ancestors
bool
RecvFlushPendingFileDeletions();
=======
FileHandleThreadPool* GetFileHandleThreadPool();
>>>>>>> upstream-releases

<<<<<<< HEAD
PIndexedDBPermissionRequestParent* AllocPIndexedDBPermissionRequestParent(
    Element* aOwnerElement, nsIPrincipal* aPrincipal);
||||||| merged common ancestors
PIndexedDBPermissionRequestParent*
AllocPIndexedDBPermissionRequestParent(Element* aOwnerElement,
                                       nsIPrincipal* aPrincipal);
=======
class PermissionRequestHelper final : public PermissionRequestBase {
 public:
  PermissionRequestHelper(
      Element* aOwnerElement, nsIPrincipal* aPrincipal,
      PBrowserParent::IndexedDBPermissionRequestResolver& aResolver)
      : PermissionRequestBase(aOwnerElement, aPrincipal),
        mResolver(aResolver) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool RecvPIndexedDBPermissionRequestConstructor(
    PIndexedDBPermissionRequestParent* aActor);
||||||| merged common ancestors
bool
RecvPIndexedDBPermissionRequestConstructor(
                                     PIndexedDBPermissionRequestParent* aActor);
=======
 protected:
  ~PermissionRequestHelper() override = default;
>>>>>>> upstream-releases

<<<<<<< HEAD
bool DeallocPIndexedDBPermissionRequestParent(
    PIndexedDBPermissionRequestParent* aActor);
||||||| merged common ancestors
bool
DeallocPIndexedDBPermissionRequestParent(
                                     PIndexedDBPermissionRequestParent* aActor);
=======
 private:
  PBrowserParent::IndexedDBPermissionRequestResolver mResolver;
>>>>>>> upstream-releases

<<<<<<< HEAD
already_AddRefed<mozilla::dom::quota::Client> CreateQuotaClient();
||||||| merged common ancestors
already_AddRefed<mozilla::dom::quota::Client>
CreateQuotaClient();
=======
  void OnPromptComplete(PermissionValue aPermissionValue) override;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
FileHandleThreadPool* GetFileHandleThreadPool();
||||||| merged common ancestors
FileHandleThreadPool*
GetFileHandleThreadPool();
=======
nsresult AsyncDeleteFile(FileManager* aFileManager, int64_t aFileId);
>>>>>>> upstream-releases

}  // namespace indexedDB
}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_indexeddb_actorsparent_h__
