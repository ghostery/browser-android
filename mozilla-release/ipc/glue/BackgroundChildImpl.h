/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_ipc_backgroundchildimpl_h__
#define mozilla_ipc_backgroundchildimpl_h__

#include "mozilla/Attributes.h"
#include "mozilla/ipc/PBackgroundChild.h"
#include "nsRefPtrHashtable.h"
#include "nsAutoPtr.h"

namespace mozilla {
namespace dom {

class IDBFileHandle;

namespace indexedDB {

class ThreadLocal;

}  // namespace indexedDB
}  // namespace dom

namespace ipc {

// Instances of this class should never be created directly. This class is meant
// to be inherited in BackgroundImpl.
class BackgroundChildImpl : public PBackgroundChild {
 public:
  class ThreadLocal;

  // Get the ThreadLocal for the current thread if
  // BackgroundChild::GetOrCreateForCurrentThread() has been called and true was
  // returned (e.g. a valid PBackgroundChild actor has been created or is in the
  // process of being created). Otherwise this function returns null.
  // This functions is implemented in BackgroundImpl.cpp.
  static ThreadLocal* GetThreadLocalForCurrentThread();

 protected:
  BackgroundChildImpl();
  virtual ~BackgroundChildImpl();

  virtual void ProcessingError(Result aCode, const char* aReason) override;

  virtual void ActorDestroy(ActorDestroyReason aWhy) override;

  virtual PBackgroundTestChild* AllocPBackgroundTestChild(
      const nsCString& aTestArg) override;

  virtual bool DeallocPBackgroundTestChild(
      PBackgroundTestChild* aActor) override;

  virtual PBackgroundIDBFactoryChild* AllocPBackgroundIDBFactoryChild(
      const LoggingInfo& aLoggingInfo) override;

  virtual bool DeallocPBackgroundIDBFactoryChild(
      PBackgroundIDBFactoryChild* aActor) override;

  virtual PBackgroundIndexedDBUtilsChild* AllocPBackgroundIndexedDBUtilsChild()
      override;

  virtual bool DeallocPBackgroundIndexedDBUtilsChild(
      PBackgroundIndexedDBUtilsChild* aActor) override;

  virtual PBackgroundSDBConnectionChild* AllocPBackgroundSDBConnectionChild(
      const PrincipalInfo& aPrincipalInfo) override;

  virtual bool DeallocPBackgroundSDBConnectionChild(
      PBackgroundSDBConnectionChild* aActor) override;

  virtual PBackgroundLSDatabaseChild* AllocPBackgroundLSDatabaseChild(
      const PrincipalInfo& aPrincipalInfo, const uint32_t& aPrivateBrowsingId,
      const uint64_t& aDatastoreId) override;

  virtual bool DeallocPBackgroundLSDatabaseChild(
      PBackgroundLSDatabaseChild* aActor) override;

  virtual PBackgroundLSObserverChild* AllocPBackgroundLSObserverChild(
      const uint64_t& aObserverId) override;

  virtual bool DeallocPBackgroundLSObserverChild(
      PBackgroundLSObserverChild* aActor) override;

  virtual PBackgroundLSRequestChild* AllocPBackgroundLSRequestChild(
      const LSRequestParams& aParams) override;

  virtual bool DeallocPBackgroundLSRequestChild(
      PBackgroundLSRequestChild* aActor) override;

  virtual PBackgroundLSSimpleRequestChild* AllocPBackgroundLSSimpleRequestChild(
      const LSSimpleRequestParams& aParams) override;

  virtual bool DeallocPBackgroundLSSimpleRequestChild(
      PBackgroundLSSimpleRequestChild* aActor) override;

  virtual PBackgroundLocalStorageCacheChild*
<<<<<<< HEAD
  AllocPBackgroundLocalStorageCacheChild(
      const PrincipalInfo& aPrincipalInfo, const nsCString& aOriginKey,
      const uint32_t& aPrivateBrowsingId) override;

  virtual bool DeallocPBackgroundLocalStorageCacheChild(
      PBackgroundLocalStorageCacheChild* aActor) override;
||||||| merged common ancestors
  AllocPBackgroundLocalStorageCacheChild(const PrincipalInfo& aPrincipalInfo,
                                         const nsCString& aOriginKey,
                                         const uint32_t& aPrivateBrowsingId)
                                         override;
=======
  AllocPBackgroundLocalStorageCacheChild(
      const PrincipalInfo& aPrincipalInfo, const nsCString& aOriginKey,
      const uint32_t& aPrivateBrowsingId) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PBackgroundStorageChild* AllocPBackgroundStorageChild(
      const nsString& aProfilePath) override;
||||||| merged common ancestors
  virtual bool
  DeallocPBackgroundLocalStorageCacheChild(
                                       PBackgroundLocalStorageCacheChild* aActor)
                                       override;
=======
  virtual bool DeallocPBackgroundLocalStorageCacheChild(
      PBackgroundLocalStorageCacheChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPBackgroundStorageChild(
      PBackgroundStorageChild* aActor) override;
||||||| merged common ancestors
  virtual PBackgroundStorageChild*
  AllocPBackgroundStorageChild(const nsString& aProfilePath) override;
=======
  virtual PBackgroundStorageChild* AllocPBackgroundStorageChild(
      const nsString& aProfilePath) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PPendingIPCBlobChild* AllocPPendingIPCBlobChild(
      const IPCBlob& aBlob) override;
||||||| merged common ancestors
  virtual bool
  DeallocPBackgroundStorageChild(PBackgroundStorageChild* aActor) override;
=======
  virtual bool DeallocPBackgroundStorageChild(
      PBackgroundStorageChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPPendingIPCBlobChild(
      PPendingIPCBlobChild* aActor) override;
||||||| merged common ancestors
  virtual PPendingIPCBlobChild*
  AllocPPendingIPCBlobChild(const IPCBlob& aBlob) override;
=======
  virtual PPendingIPCBlobChild* AllocPPendingIPCBlobChild(
      const IPCBlob& aBlob) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PIPCBlobInputStreamChild* AllocPIPCBlobInputStreamChild(
      const nsID& aID, const uint64_t& aSize) override;
||||||| merged common ancestors
  virtual bool
  DeallocPPendingIPCBlobChild(PPendingIPCBlobChild* aActor) override;
=======
  virtual bool DeallocPPendingIPCBlobChild(
      PPendingIPCBlobChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPIPCBlobInputStreamChild(
      PIPCBlobInputStreamChild* aActor) override;
||||||| merged common ancestors
  virtual PIPCBlobInputStreamChild*
  AllocPIPCBlobInputStreamChild(const nsID& aID,
                                const uint64_t& aSize) override;
=======
  virtual PIPCBlobInputStreamChild* AllocPIPCBlobInputStreamChild(
      const nsID& aID, const uint64_t& aSize) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PTemporaryIPCBlobChild* AllocPTemporaryIPCBlobChild() override;
||||||| merged common ancestors
  virtual bool
  DeallocPIPCBlobInputStreamChild(PIPCBlobInputStreamChild* aActor) override;
=======
  virtual bool DeallocPIPCBlobInputStreamChild(
      PIPCBlobInputStreamChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPTemporaryIPCBlobChild(
      PTemporaryIPCBlobChild* aActor) override;
||||||| merged common ancestors
  virtual PTemporaryIPCBlobChild*
  AllocPTemporaryIPCBlobChild() override;
=======
  virtual PTemporaryIPCBlobChild* AllocPTemporaryIPCBlobChild() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::dom::PRemoteWorkerChild* AllocPRemoteWorkerChild(
      const RemoteWorkerData& aData) override;
||||||| merged common ancestors
  virtual bool
  DeallocPTemporaryIPCBlobChild(PTemporaryIPCBlobChild* aActor) override;
=======
  virtual bool DeallocPTemporaryIPCBlobChild(
      PTemporaryIPCBlobChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPRemoteWorkerConstructor(
      PRemoteWorkerChild* aActor, const RemoteWorkerData& aData) override;
||||||| merged common ancestors
  virtual PFileDescriptorSetChild*
  AllocPFileDescriptorSetChild(const FileDescriptor& aFileDescriptor)
                               override;
=======
  virtual PFileCreatorChild* AllocPFileCreatorChild(
      const nsString& aFullPath, const nsString& aType, const nsString& aName,
      const Maybe<int64_t>& aLastModified, const bool& aExistenceCheck,
      const bool& aIsFromNsIFile) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPRemoteWorkerChild(
      mozilla::dom::PRemoteWorkerChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPFileDescriptorSetChild(PFileDescriptorSetChild* aActor) override;
=======
  virtual bool DeallocPFileCreatorChild(PFileCreatorChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::dom::PRemoteWorkerServiceChild*
  AllocPRemoteWorkerServiceChild() override;
||||||| merged common ancestors
  virtual PCamerasChild*
  AllocPCamerasChild() override;
=======
  virtual mozilla::dom::PRemoteWorkerChild* AllocPRemoteWorkerChild(
      const RemoteWorkerData& aData) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPRemoteWorkerServiceChild(
      mozilla::dom::PRemoteWorkerServiceChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPCamerasChild(PCamerasChild* aActor) override;
=======
  virtual mozilla::ipc::IPCResult RecvPRemoteWorkerConstructor(
      PRemoteWorkerChild* aActor, const RemoteWorkerData& aData) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::dom::PSharedWorkerChild* AllocPSharedWorkerChild(
      const mozilla::dom::RemoteWorkerData& aData, const uint64_t& aWindowID,
      const mozilla::dom::MessagePortIdentifier& aPortIdentifier) override;
||||||| merged common ancestors
  virtual PVsyncChild*
  AllocPVsyncChild() override;
=======
  virtual bool DeallocPRemoteWorkerChild(
      mozilla::dom::PRemoteWorkerChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPSharedWorkerChild(
      mozilla::dom::PSharedWorkerChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPVsyncChild(PVsyncChild* aActor) override;
=======
  virtual mozilla::dom::PRemoteWorkerServiceChild*
  AllocPRemoteWorkerServiceChild() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PFileDescriptorSetChild* AllocPFileDescriptorSetChild(
      const FileDescriptor& aFileDescriptor) override;
||||||| merged common ancestors
  virtual PUDPSocketChild*
  AllocPUDPSocketChild(const OptionalPrincipalInfo& aPrincipalInfo,
                       const nsCString& aFilter) override;
  virtual bool
  DeallocPUDPSocketChild(PUDPSocketChild* aActor) override;
=======
  virtual bool DeallocPRemoteWorkerServiceChild(
      mozilla::dom::PRemoteWorkerServiceChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPFileDescriptorSetChild(
      PFileDescriptorSetChild* aActor) override;
||||||| merged common ancestors
  virtual PBroadcastChannelChild*
  AllocPBroadcastChannelChild(const PrincipalInfo& aPrincipalInfo,
                              const nsCString& aOrigin,
                              const nsString& aChannel) override;
=======
  virtual mozilla::dom::PSharedWorkerChild* AllocPSharedWorkerChild(
      const mozilla::dom::RemoteWorkerData& aData, const uint64_t& aWindowID,
      const mozilla::dom::MessagePortIdentifier& aPortIdentifier) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PCamerasChild* AllocPCamerasChild() override;
||||||| merged common ancestors
  virtual bool
  DeallocPBroadcastChannelChild(PBroadcastChannelChild* aActor) override;
=======
  virtual bool DeallocPSharedWorkerChild(
      mozilla::dom::PSharedWorkerChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPCamerasChild(PCamerasChild* aActor) override;
||||||| merged common ancestors
  virtual PServiceWorkerManagerChild*
  AllocPServiceWorkerManagerChild() override;
=======
  virtual PFileDescriptorSetChild* AllocPFileDescriptorSetChild(
      const FileDescriptor& aFileDescriptor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PVsyncChild* AllocPVsyncChild() override;
||||||| merged common ancestors
  virtual bool
  DeallocPServiceWorkerManagerChild(PServiceWorkerManagerChild* aActor) override;
=======
  virtual bool DeallocPFileDescriptorSetChild(
      PFileDescriptorSetChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPVsyncChild(PVsyncChild* aActor) override;
||||||| merged common ancestors
  virtual dom::cache::PCacheStorageChild*
  AllocPCacheStorageChild(const dom::cache::Namespace& aNamespace,
                          const PrincipalInfo& aPrincipalInfo) override;
=======
  virtual PCamerasChild* AllocPCamerasChild() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PUDPSocketChild* AllocPUDPSocketChild(
      const OptionalPrincipalInfo& aPrincipalInfo,
      const nsCString& aFilter) override;
  virtual bool DeallocPUDPSocketChild(PUDPSocketChild* aActor) override;

  virtual PBroadcastChannelChild* AllocPBroadcastChannelChild(
      const PrincipalInfo& aPrincipalInfo, const nsCString& aOrigin,
      const nsString& aChannel) override;

  virtual bool DeallocPBroadcastChannelChild(
      PBroadcastChannelChild* aActor) override;

  virtual PServiceWorkerManagerChild* AllocPServiceWorkerManagerChild()
      override;

  virtual bool DeallocPServiceWorkerManagerChild(
      PServiceWorkerManagerChild* aActor) override;

  virtual dom::cache::PCacheStorageChild* AllocPCacheStorageChild(
      const dom::cache::Namespace& aNamespace,
      const PrincipalInfo& aPrincipalInfo) override;

  virtual bool DeallocPCacheStorageChild(
      dom::cache::PCacheStorageChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPCacheStorageChild(dom::cache::PCacheStorageChild* aActor) override;
=======
  virtual bool DeallocPCamerasChild(PCamerasChild* aActor) override;
>>>>>>> upstream-releases

  virtual PVsyncChild* AllocPVsyncChild() override;

  virtual bool DeallocPVsyncChild(PVsyncChild* aActor) override;

<<<<<<< HEAD
  virtual bool DeallocPCacheChild(dom::cache::PCacheChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPCacheChild(dom::cache::PCacheChild* aActor) override;
=======
  virtual PUDPSocketChild* AllocPUDPSocketChild(
      const Maybe<PrincipalInfo>& aPrincipalInfo,
      const nsCString& aFilter) override;
  virtual bool DeallocPUDPSocketChild(PUDPSocketChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual dom::cache::PCacheStreamControlChild* AllocPCacheStreamControlChild()
      override;
||||||| merged common ancestors
  virtual dom::cache::PCacheStreamControlChild*
  AllocPCacheStreamControlChild() override;
=======
  virtual PBroadcastChannelChild* AllocPBroadcastChannelChild(
      const PrincipalInfo& aPrincipalInfo, const nsCString& aOrigin,
      const nsString& aChannel) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPCacheStreamControlChild(
      dom::cache::PCacheStreamControlChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPCacheStreamControlChild(dom::cache::PCacheStreamControlChild* aActor) override;
=======
  virtual bool DeallocPBroadcastChannelChild(
      PBroadcastChannelChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PMessagePortChild* AllocPMessagePortChild(
      const nsID& aUUID, const nsID& aDestinationUUID,
      const uint32_t& aSequenceID) override;
||||||| merged common ancestors
  virtual PMessagePortChild*
  AllocPMessagePortChild(const nsID& aUUID, const nsID& aDestinationUUID,
                         const uint32_t& aSequenceID) override;
=======
  virtual PServiceWorkerManagerChild* AllocPServiceWorkerManagerChild()
      override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPMessagePortChild(PMessagePortChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPMessagePortChild(PMessagePortChild* aActor) override;
=======
  virtual bool DeallocPServiceWorkerManagerChild(
      PServiceWorkerManagerChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PChildToParentStreamChild* AllocPChildToParentStreamChild() override;
||||||| merged common ancestors
  virtual PChildToParentStreamChild*
  AllocPChildToParentStreamChild() override;
=======
  virtual dom::cache::PCacheStorageChild* AllocPCacheStorageChild(
      const dom::cache::Namespace& aNamespace,
      const PrincipalInfo& aPrincipalInfo) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPChildToParentStreamChild(
      PChildToParentStreamChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPChildToParentStreamChild(PChildToParentStreamChild* aActor) override;
=======
  virtual bool DeallocPCacheStorageChild(
      dom::cache::PCacheStorageChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PParentToChildStreamChild* AllocPParentToChildStreamChild() override;
||||||| merged common ancestors
  virtual PParentToChildStreamChild*
  AllocPParentToChildStreamChild() override;
=======
  virtual dom::cache::PCacheChild* AllocPCacheChild() override;

  virtual bool DeallocPCacheChild(dom::cache::PCacheChild* aActor) override;

  virtual dom::cache::PCacheStreamControlChild* AllocPCacheStreamControlChild()
      override;

  virtual bool DeallocPCacheStreamControlChild(
      dom::cache::PCacheStreamControlChild* aActor) override;

  virtual PMessagePortChild* AllocPMessagePortChild(
      const nsID& aUUID, const nsID& aDestinationUUID,
      const uint32_t& aSequenceID) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPParentToChildStreamChild(
      PParentToChildStreamChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPParentToChildStreamChild(PParentToChildStreamChild* aActor) override;
=======
  virtual bool DeallocPMessagePortChild(PMessagePortChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PAsmJSCacheEntryChild* AllocPAsmJSCacheEntryChild(
      const dom::asmjscache::OpenMode& aOpenMode,
      const dom::asmjscache::WriteParams& aWriteParams,
      const PrincipalInfo& aPrincipalInfo) override;
||||||| merged common ancestors
  virtual PAsmJSCacheEntryChild*
  AllocPAsmJSCacheEntryChild(const dom::asmjscache::OpenMode& aOpenMode,
                             const dom::asmjscache::WriteParams& aWriteParams,
                             const PrincipalInfo& aPrincipalInfo) override;
=======
  virtual PChildToParentStreamChild* AllocPChildToParentStreamChild() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPAsmJSCacheEntryChild(
      PAsmJSCacheEntryChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPAsmJSCacheEntryChild(PAsmJSCacheEntryChild* aActor) override;
=======
  virtual bool DeallocPChildToParentStreamChild(
      PChildToParentStreamChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PQuotaChild* AllocPQuotaChild() override;
||||||| merged common ancestors
  virtual PQuotaChild*
  AllocPQuotaChild() override;
=======
  virtual PParentToChildStreamChild* AllocPParentToChildStreamChild() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPQuotaChild(PQuotaChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPQuotaChild(PQuotaChild* aActor) override;
=======
  virtual bool DeallocPParentToChildStreamChild(
      PParentToChildStreamChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PFileSystemRequestChild* AllocPFileSystemRequestChild(
      const FileSystemParams&) override;
||||||| merged common ancestors
  virtual PFileSystemRequestChild*
  AllocPFileSystemRequestChild(const FileSystemParams&) override;
=======
  virtual PQuotaChild* AllocPQuotaChild() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPFileSystemRequestChild(
      PFileSystemRequestChild*) override;
||||||| merged common ancestors
  virtual bool
  DeallocPFileSystemRequestChild(PFileSystemRequestChild*) override;
=======
  virtual bool DeallocPQuotaChild(PQuotaChild* aActor) override;

  virtual PFileSystemRequestChild* AllocPFileSystemRequestChild(
      const FileSystemParams&) override;

  virtual bool DeallocPFileSystemRequestChild(
      PFileSystemRequestChild*) override;
>>>>>>> upstream-releases

  // Gamepad API Background IPC
  virtual PGamepadEventChannelChild* AllocPGamepadEventChannelChild() override;

  virtual bool DeallocPGamepadEventChannelChild(
      PGamepadEventChannelChild* aActor) override;

  virtual PGamepadTestChannelChild* AllocPGamepadTestChannelChild() override;

  virtual bool DeallocPGamepadTestChannelChild(
      PGamepadTestChannelChild* aActor) override;

  virtual PClientManagerChild* AllocPClientManagerChild() override;

  virtual bool DeallocPClientManagerChild(PClientManagerChild* aActor) override;

#ifdef EARLY_BETA_OR_EARLIER
  virtual void OnChannelReceivedMessage(const Message& aMsg) override;
#endif

<<<<<<< HEAD
  virtual PWebAuthnTransactionChild* AllocPWebAuthnTransactionChild() override;

  virtual bool DeallocPWebAuthnTransactionChild(
      PWebAuthnTransactionChild* aActor) override;
||||||| merged common ancestors
  virtual PWebAuthnTransactionChild*
  AllocPWebAuthnTransactionChild() override;

  virtual bool
  DeallocPWebAuthnTransactionChild(PWebAuthnTransactionChild* aActor) override;
=======
  virtual PWebAuthnTransactionChild* AllocPWebAuthnTransactionChild() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PHttpBackgroundChannelChild* AllocPHttpBackgroundChannelChild(
      const uint64_t& aChannelId) override;
||||||| merged common ancestors
  virtual PHttpBackgroundChannelChild*
  AllocPHttpBackgroundChannelChild(const uint64_t& aChannelId) override;
=======
  virtual bool DeallocPWebAuthnTransactionChild(
      PWebAuthnTransactionChild* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPHttpBackgroundChannelChild(
      PHttpBackgroundChannelChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPHttpBackgroundChannelChild(PHttpBackgroundChannelChild* aActor) override;
=======
  virtual PHttpBackgroundChannelChild* AllocPHttpBackgroundChannelChild(
      const uint64_t& aChannelId) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool GetMessageSchedulerGroups(const Message& aMsg,
                                 SchedulerGroupSet& aGroups) override;
||||||| merged common ancestors
  bool
  GetMessageSchedulerGroups(const Message& aMsg, SchedulerGroupSet& aGroups) override;
=======
  virtual bool DeallocPHttpBackgroundChannelChild(
      PHttpBackgroundChannelChild* aActor) override;
>>>>>>> upstream-releases

  virtual PMIDIPortChild* AllocPMIDIPortChild(
      const MIDIPortInfo& aPortInfo, const bool& aSysexEnabled) override;
  virtual bool DeallocPMIDIPortChild(PMIDIPortChild*) override;

  virtual PMIDIManagerChild* AllocPMIDIManagerChild() override;
  virtual bool DeallocPMIDIManagerChild(PMIDIManagerChild*) override;

  virtual PServiceWorkerChild* AllocPServiceWorkerChild(
      const IPCServiceWorkerDescriptor&) override;

  virtual bool DeallocPServiceWorkerChild(PServiceWorkerChild*) override;

  virtual PServiceWorkerContainerChild* AllocPServiceWorkerContainerChild()
      override;

  virtual bool DeallocPServiceWorkerContainerChild(
      PServiceWorkerContainerChild*) override;

  virtual PServiceWorkerRegistrationChild* AllocPServiceWorkerRegistrationChild(
      const IPCServiceWorkerRegistrationDescriptor&) override;

  virtual bool DeallocPServiceWorkerRegistrationChild(
      PServiceWorkerRegistrationChild*) override;

  virtual PEndpointForReportChild* AllocPEndpointForReportChild(
      const nsString& aGroupName, const PrincipalInfo& aPrincipalInfo) override;

  virtual bool DeallocPEndpointForReportChild(
      PEndpointForReportChild* aActor) override;
};

class BackgroundChildImpl::ThreadLocal final {
  friend class nsAutoPtr<ThreadLocal>;

 public:
  nsAutoPtr<mozilla::dom::indexedDB::ThreadLocal> mIndexedDBThreadLocal;
  mozilla::dom::IDBFileHandle* mCurrentFileHandle;

 public:
  ThreadLocal();

 private:
  // Only destroyed by nsAutoPtr<ThreadLocal>.
  ~ThreadLocal();
};

}  // namespace ipc
}  // namespace mozilla

#endif  // mozilla_ipc_backgroundchildimpl_h__
