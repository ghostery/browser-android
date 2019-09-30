/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_ContentChild_h
#define mozilla_dom_ContentChild_h

#include "base/shared_memory.h"
#include "mozilla/Atomics.h"
#include "mozilla/Attributes.h"
#include "mozilla/dom/BrowserBridgeChild.h"
#include "mozilla/dom/PBrowserOrId.h"
#include "mozilla/dom/PContentChild.h"
#include "mozilla/dom/RemoteBrowser.h"
#include "mozilla/dom/CPOWManagerGetter.h"
#include "mozilla/StaticPtr.h"
#include "mozilla/ipc/Shmem.h"
#include "mozilla/jsipc/CrossProcessObjectWrappers.h"
#include "nsAutoPtr.h"
#include "nsHashKeys.h"
#include "nsIObserver.h"
#include "nsTHashtable.h"
#include "nsStringFwd.h"
#include "nsTArrayForwardDeclare.h"
#include "nsRefPtrHashtable.h"

#include "nsIWindowProvider.h"

#if defined(XP_MACOSX) && defined(MOZ_SANDBOX)
#  include "nsIFile.h"
#endif

struct ChromePackage;
class nsIObserver;
struct SubstitutionMapping;
struct OverrideMapping;
class nsIDomainPolicy;
class nsIURIClassifierCallback;
struct LookAndFeelInt;
<<<<<<< HEAD
class nsDocShellLoadState;
||||||| merged common ancestors
class nsDocShellLoadInfo;
=======
class nsDocShellLoadState;
class nsFrameLoader;
>>>>>>> upstream-releases

namespace mozilla {
class RemoteSpellcheckEngineChild;
class ChildProfilerController;

using mozilla::loader::PScriptCacheChild;

#if !defined(XP_WIN)
// Returns whether or not the currently running build is an unpackaged
// developer build. This check is implemented by looking for omni.ja in the
// the obj/dist dir. We use this routine to detect when the build dir will
// use symlinks to the repo and object dir. On Windows, dev builds don't
// use symlinks.
bool IsDevelopmentBuild();
#endif /* !XP_WIN */

<<<<<<< HEAD
#if defined(XP_MACOSX)
// Return the repo directory and the repo object directory respectively. These
// should only be used on Mac developer builds to determine the path to the
// repo or object directory.
nsresult GetRepoDir(nsIFile** aRepoDir);
nsresult GetObjDir(nsIFile** aObjDir);
#endif /* XP_MACOSX */

||||||| merged common ancestors
#if defined(XP_MACOSX)
// Return the repo directory and the repo object directory respectively. These
// should only be used on Mac developer builds to determine the path to the
// repo or object directory.
nsresult GetRepoDir(nsIFile **aRepoDir);
nsresult GetObjDir(nsIFile **aObjDir);
#endif /* XP_MACOSX */

=======
>>>>>>> upstream-releases
namespace ipc {
class URIParams;
}  // namespace ipc

namespace dom {

namespace ipc {
class SharedMap;
}

class AlertObserver;
class ConsoleListener;
class ClonedMessageData;
class BrowserChild;
class GetFilesHelperChild;
class TabContext;

<<<<<<< HEAD
class ContentChild final : public PContentChild,
                           public nsIWindowProvider,
                           public nsIContentChild {
||||||| merged common ancestors
class ContentChild final : public PContentChild
                         , public nsIWindowProvider
                         , public nsIContentChild
{
=======
class ContentChild final : public PContentChild,
                           public nsIWindowProvider,
                           public CPOWManagerGetter,
                           public mozilla::ipc::IShmemAllocator {
>>>>>>> upstream-releases
  typedef mozilla::dom::ClonedMessageData ClonedMessageData;
  typedef mozilla::ipc::FileDescriptor FileDescriptor;
  typedef mozilla::ipc::PFileDescriptorSetChild PFileDescriptorSetChild;
  typedef mozilla::ipc::URIParams URIParams;

<<<<<<< HEAD
 public:
||||||| merged common ancestors
public:
=======
  friend class PContentChild;

 public:
>>>>>>> upstream-releases
  NS_DECL_NSIWINDOWPROVIDER

  ContentChild();
  virtual ~ContentChild();
  NS_IMETHOD QueryInterface(REFNSIID aIID, void** aInstancePtr) override;
  NS_IMETHOD_(MozExternalRefCountType) AddRef(void) override { return 1; }
  NS_IMETHOD_(MozExternalRefCountType) Release(void) override { return 1; }

  struct AppInfo {
    nsCString version;
    nsCString buildID;
    nsCString name;
    nsCString UAName;
    nsCString ID;
    nsCString vendor;
    nsCString sourceURL;
  };

<<<<<<< HEAD
  nsresult ProvideWindowCommon(TabChild* aTabOpener,
                               mozIDOMWindowProxy* aOpener, bool aIframeMoz,
                               uint32_t aChromeFlags, bool aCalledFromJS,
                               bool aPositionSpecified, bool aSizeSpecified,
                               nsIURI* aURI, const nsAString& aName,
                               const nsACString& aFeatures, bool aForceNoOpener,
                               nsDocShellLoadState* aLoadState,
                               bool* aWindowIsNew,
                               mozIDOMWindowProxy** aReturn);

  bool Init(MessageLoop* aIOLoop, base::ProcessId aParentPid,
            const char* aParentBuildID, IPC::Channel* aChannel,
            uint64_t aChildID, bool aIsForBrowser);
||||||| merged common ancestors
  nsresult
  ProvideWindowCommon(TabChild* aTabOpener,
                      mozIDOMWindowProxy* aOpener,
                      bool aIframeMoz,
                      uint32_t aChromeFlags,
                      bool aCalledFromJS,
                      bool aPositionSpecified,
                      bool aSizeSpecified,
                      nsIURI* aURI,
                      const nsAString& aName,
                      const nsACString& aFeatures,
                      bool aForceNoOpener,
                      nsDocShellLoadInfo* aLoadInfo,
                      bool* aWindowIsNew,
                      mozIDOMWindowProxy** aReturn);

  bool Init(MessageLoop* aIOLoop,
            base::ProcessId aParentPid,
            const char* aParentBuildID,
            IPC::Channel* aChannel,
            uint64_t aChildID,
            bool aIsForBrowser);
=======
  nsresult ProvideWindowCommon(
      BrowserChild* aTabOpener, mozIDOMWindowProxy* aParent, bool aIframeMoz,
      uint32_t aChromeFlags, bool aCalledFromJS, bool aPositionSpecified,
      bool aSizeSpecified, nsIURI* aURI, const nsAString& aName,
      const nsACString& aFeatures, bool aForceNoOpener, bool aForceNoReferrer,
      nsDocShellLoadState* aLoadState, bool* aWindowIsNew,
      mozIDOMWindowProxy** aReturn);

  bool Init(MessageLoop* aIOLoop, base::ProcessId aParentPid,
            const char* aParentBuildID, IPC::Channel* aChannel,
            uint64_t aChildID, bool aIsForBrowser);
>>>>>>> upstream-releases

  void InitXPCOM(const XPCOMInitData& aXPCOMInit,
                 const mozilla::dom::ipc::StructuredCloneData& aInitialData);

  void InitSharedUASheets(const Maybe<base::SharedMemoryHandle>& aHandle,
                          uintptr_t aAddress);

  void InitGraphicsDeviceData(const ContentDeviceData& aData);

  static ContentChild* GetSingleton() { return sSingleton; }

  const AppInfo& GetAppInfo() { return mAppInfo; }

  void SetProcessName(const nsAString& aName);

  void GetProcessName(nsAString& aName) const;

  void GetProcessName(nsACString& aName) const;

<<<<<<< HEAD
  void LaunchRDDProcess();

#if defined(XP_MACOSX) && defined(MOZ_CONTENT_SANDBOX)
  void GetProfileDir(nsIFile** aProfileDir) const {
||||||| merged common ancestors
#if defined(XP_MACOSX) && defined(MOZ_CONTENT_SANDBOX)
  void GetProfileDir(nsIFile** aProfileDir) const
  {
=======
  void LaunchRDDProcess();

#if defined(XP_MACOSX) && defined(MOZ_SANDBOX)
  void GetProfileDir(nsIFile** aProfileDir) const {
>>>>>>> upstream-releases
    *aProfileDir = mProfileDir;
    NS_IF_ADDREF(*aProfileDir);
  }

  void SetProfileDir(nsIFile* aProfileDir) { mProfileDir = aProfileDir; }
#endif

  bool IsAlive() const;

  bool IsShuttingDown() const;

  ipc::SharedMap* SharedData() { return mSharedData; };

  static void AppendProcessId(nsACString& aName);

  static void UpdateCookieStatus(nsIChannel* aChannel);

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvInitContentBridgeChild(
      Endpoint<PContentBridgeChild>&& aEndpoint) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvInitContentBridgeChild(Endpoint<PContentBridgeChild>&& aEndpoint) override;
=======
  static already_AddRefed<RemoteBrowser> CreateBrowser(
      nsFrameLoader* aFrameLoader, const TabContext& aContext,
      const nsString& aRemoteType, BrowsingContext* aBrowsingContext);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvInitGMPService(
      Endpoint<PGMPServiceChild>&& aGMPService) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvInitGMPService(Endpoint<PGMPServiceChild>&& aGMPService) override;
=======
  mozilla::ipc::IPCResult RecvInitGMPService(
      Endpoint<PGMPServiceChild>&& aGMPService);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvInitProfiler(
      Endpoint<PProfilerChild>&& aEndpoint) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvInitProfiler(Endpoint<PProfilerChild>&& aEndpoint) override;
=======
  mozilla::ipc::IPCResult RecvInitProfiler(
      Endpoint<PProfilerChild>&& aEndpoint);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvGMPsChanged(
      nsTArray<GMPCapabilityData>&& capabilities) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvGMPsChanged(nsTArray<GMPCapabilityData>&& capabilities) override;
=======
  mozilla::ipc::IPCResult RecvGMPsChanged(
      nsTArray<GMPCapabilityData>&& capabilities);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvInitProcessHangMonitor(
      Endpoint<PProcessHangMonitorChild>&& aHangMonitor) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvInitProcessHangMonitor(Endpoint<PProcessHangMonitorChild>&& aHangMonitor) override;
=======
  mozilla::ipc::IPCResult RecvInitProcessHangMonitor(
      Endpoint<PProcessHangMonitorChild>&& aHangMonitor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvInitRendering(
      Endpoint<PCompositorManagerChild>&& aCompositor,
      Endpoint<PImageBridgeChild>&& aImageBridge,
      Endpoint<PVRManagerChild>&& aVRBridge,
      Endpoint<PVideoDecoderManagerChild>&& aVideoManager,
      nsTArray<uint32_t>&& namespaces) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvInitRendering(
    Endpoint<PCompositorManagerChild>&& aCompositor,
    Endpoint<PImageBridgeChild>&& aImageBridge,
    Endpoint<PVRManagerChild>&& aVRBridge,
    Endpoint<PVideoDecoderManagerChild>&& aVideoManager,
    nsTArray<uint32_t>&& namespaces) override;
=======
  mozilla::ipc::IPCResult RecvInitRendering(
      Endpoint<PCompositorManagerChild>&& aCompositor,
      Endpoint<PImageBridgeChild>&& aImageBridge,
      Endpoint<PVRManagerChild>&& aVRBridge,
      Endpoint<PRemoteDecoderManagerChild>&& aVideoManager,
      nsTArray<uint32_t>&& namespaces);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvRequestPerformanceMetrics(
      const nsID& aID) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvRequestPerformanceMetrics(const nsID& aID) override;
=======
  mozilla::ipc::IPCResult RecvRequestPerformanceMetrics(const nsID& aID);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvReinitRendering(
      Endpoint<PCompositorManagerChild>&& aCompositor,
      Endpoint<PImageBridgeChild>&& aImageBridge,
      Endpoint<PVRManagerChild>&& aVRBridge,
      Endpoint<PVideoDecoderManagerChild>&& aVideoManager,
      nsTArray<uint32_t>&& namespaces) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvReinitRendering(
    Endpoint<PCompositorManagerChild>&& aCompositor,
    Endpoint<PImageBridgeChild>&& aImageBridge,
    Endpoint<PVRManagerChild>&& aVRBridge,
    Endpoint<PVideoDecoderManagerChild>&& aVideoManager,
    nsTArray<uint32_t>&& namespaces) override;
=======
  mozilla::ipc::IPCResult RecvReinitRendering(
      Endpoint<PCompositorManagerChild>&& aCompositor,
      Endpoint<PImageBridgeChild>&& aImageBridge,
      Endpoint<PVRManagerChild>&& aVRBridge,
      Endpoint<PRemoteDecoderManagerChild>&& aVideoManager,
      nsTArray<uint32_t>&& namespaces);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvAudioDefaultDeviceChange();

  mozilla::ipc::IPCResult RecvReinitRenderingForDeviceReset();

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvSetProcessSandbox(
      const MaybeFileDesc& aBroker) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvSetProcessSandbox(const MaybeFileDesc& aBroker) override;
=======
  mozilla::ipc::IPCResult RecvSetProcessSandbox(
      const Maybe<FileDescriptor>& aBroker);
>>>>>>> upstream-releases

  bool DeallocPBrowserChild(PBrowserChild*);

  PIPCBlobInputStreamChild* AllocPIPCBlobInputStreamChild(
      const nsID& aID, const uint64_t& aSize);

<<<<<<< HEAD
  virtual PIPCBlobInputStreamChild* AllocPIPCBlobInputStreamChild(
      const nsID& aID, const uint64_t& aSize) override;
||||||| merged common ancestors
  virtual PIPCBlobInputStreamChild*
  AllocPIPCBlobInputStreamChild(const nsID& aID,
                                const uint64_t& aSize) override;
=======
  bool DeallocPIPCBlobInputStreamChild(PIPCBlobInputStreamChild* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPIPCBlobInputStreamChild(
      PIPCBlobInputStreamChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPIPCBlobInputStreamChild(PIPCBlobInputStreamChild* aActor) override;
=======
  PHalChild* AllocPHalChild();
  bool DeallocPHalChild(PHalChild*);
>>>>>>> upstream-releases

  PHeapSnapshotTempFileHelperChild* AllocPHeapSnapshotTempFileHelperChild();

  bool DeallocPHeapSnapshotTempFileHelperChild(
      PHeapSnapshotTempFileHelperChild*);

<<<<<<< HEAD
  virtual bool DeallocPHeapSnapshotTempFileHelperChild(
      PHeapSnapshotTempFileHelperChild*) override;
||||||| merged common ancestors
  virtual bool
  DeallocPHeapSnapshotTempFileHelperChild(PHeapSnapshotTempFileHelperChild*) override;
=======
  PCycleCollectWithLogsChild* AllocPCycleCollectWithLogsChild(
      const bool& aDumpAllTraces, const FileDescriptor& aGCLog,
      const FileDescriptor& aCCLog);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PCycleCollectWithLogsChild* AllocPCycleCollectWithLogsChild(
      const bool& aDumpAllTraces, const FileDescriptor& aGCLog,
      const FileDescriptor& aCCLog) override;
||||||| merged common ancestors
  virtual PCycleCollectWithLogsChild*
  AllocPCycleCollectWithLogsChild(const bool& aDumpAllTraces,
                                  const FileDescriptor& aGCLog,
                                  const FileDescriptor& aCCLog) override;
=======
  bool DeallocPCycleCollectWithLogsChild(PCycleCollectWithLogsChild* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPCycleCollectWithLogsChild(
      PCycleCollectWithLogsChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPCycleCollectWithLogsChild(PCycleCollectWithLogsChild* aActor) override;
=======
  virtual mozilla::ipc::IPCResult RecvPCycleCollectWithLogsConstructor(
      PCycleCollectWithLogsChild* aChild, const bool& aDumpAllTraces,
      const FileDescriptor& aGCLog, const FileDescriptor& aCCLog) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPCycleCollectWithLogsConstructor(
      PCycleCollectWithLogsChild* aChild, const bool& aDumpAllTraces,
      const FileDescriptor& aGCLog, const FileDescriptor& aCCLog) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvPCycleCollectWithLogsConstructor(PCycleCollectWithLogsChild* aChild,
                                       const bool& aDumpAllTraces,
                                       const FileDescriptor& aGCLog,
                                       const FileDescriptor& aCCLog) override;
=======
  PWebBrowserPersistDocumentChild* AllocPWebBrowserPersistDocumentChild(
      PBrowserChild* aBrowser, const uint64_t& aOuterWindowID);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PWebBrowserPersistDocumentChild* AllocPWebBrowserPersistDocumentChild(
      PBrowserChild* aBrowser, const uint64_t& aOuterWindowID) override;
||||||| merged common ancestors
  virtual PWebBrowserPersistDocumentChild*
  AllocPWebBrowserPersistDocumentChild(PBrowserChild* aBrowser,
                                       const uint64_t& aOuterWindowID) override;
=======
  virtual mozilla::ipc::IPCResult RecvPWebBrowserPersistDocumentConstructor(
      PWebBrowserPersistDocumentChild* aActor, PBrowserChild* aBrowser,
      const uint64_t& aOuterWindowID) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPWebBrowserPersistDocumentConstructor(
      PWebBrowserPersistDocumentChild* aActor, PBrowserChild* aBrowser,
      const uint64_t& aOuterWindowID) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvPWebBrowserPersistDocumentConstructor(PWebBrowserPersistDocumentChild *aActor,
                                            PBrowserChild *aBrowser,
                                            const uint64_t& aOuterWindowID) override;
=======
  bool DeallocPWebBrowserPersistDocumentChild(
      PWebBrowserPersistDocumentChild* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPWebBrowserPersistDocumentChild(
      PWebBrowserPersistDocumentChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPWebBrowserPersistDocumentChild(PWebBrowserPersistDocumentChild* aActor) override;
=======
  PTestShellChild* AllocPTestShellChild();
>>>>>>> upstream-releases

  bool DeallocPTestShellChild(PTestShellChild*);

  virtual mozilla::ipc::IPCResult RecvPTestShellConstructor(
      PTestShellChild*) override;

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPTestShellConstructor(
      PTestShellChild*) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvPTestShellConstructor(PTestShellChild*) override;
=======
  PScriptCacheChild* AllocPScriptCacheChild(const FileDescOrError& cacheFile,
                                            const bool& wantCacheData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PScriptCacheChild* AllocPScriptCacheChild(
      const FileDescOrError& cacheFile, const bool& wantCacheData) override;
||||||| merged common ancestors
  virtual PScriptCacheChild*
  AllocPScriptCacheChild(const FileDescOrError& cacheFile,
                         const bool& wantCacheData) override;
=======
  bool DeallocPScriptCacheChild(PScriptCacheChild*);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPScriptCacheChild(PScriptCacheChild*) override;

  virtual mozilla::ipc::IPCResult RecvPScriptCacheConstructor(
      PScriptCacheChild*, const FileDescOrError& cacheFile,
      const bool& wantCacheData) override;
||||||| merged common ancestors
  virtual bool DeallocPScriptCacheChild(PScriptCacheChild*) override;

  virtual mozilla::ipc::IPCResult
  RecvPScriptCacheConstructor(PScriptCacheChild*,
                              const FileDescOrError& cacheFile,
                              const bool& wantCacheData) override;
=======
  virtual mozilla::ipc::IPCResult RecvPScriptCacheConstructor(
      PScriptCacheChild*, const FileDescOrError& cacheFile,
      const bool& wantCacheData) override;
>>>>>>> upstream-releases

  jsipc::CPOWManager* GetCPOWManager() override;

  PNeckoChild* AllocPNeckoChild();

  bool DeallocPNeckoChild(PNeckoChild*);

  PPrintingChild* AllocPPrintingChild();

  bool DeallocPPrintingChild(PPrintingChild*);

<<<<<<< HEAD
  virtual PChildToParentStreamChild* SendPChildToParentStreamConstructor(
      PChildToParentStreamChild*) override;
||||||| merged common ancestors
  virtual PChildToParentStreamChild*
  SendPChildToParentStreamConstructor(PChildToParentStreamChild*) override;
=======
  PChildToParentStreamChild* SendPChildToParentStreamConstructor(
      PChildToParentStreamChild*);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PChildToParentStreamChild* AllocPChildToParentStreamChild() override;
  virtual bool DeallocPChildToParentStreamChild(
      PChildToParentStreamChild*) override;
||||||| merged common ancestors
  virtual PChildToParentStreamChild* AllocPChildToParentStreamChild() override;
  virtual bool DeallocPChildToParentStreamChild(PChildToParentStreamChild*) override;
=======
  PChildToParentStreamChild* AllocPChildToParentStreamChild();
  bool DeallocPChildToParentStreamChild(PChildToParentStreamChild*);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PParentToChildStreamChild* AllocPParentToChildStreamChild() override;
  virtual bool DeallocPParentToChildStreamChild(
      PParentToChildStreamChild*) override;
||||||| merged common ancestors
  virtual PParentToChildStreamChild* AllocPParentToChildStreamChild() override;
  virtual bool DeallocPParentToChildStreamChild(PParentToChildStreamChild*) override;
=======
  PParentToChildStreamChild* AllocPParentToChildStreamChild();
  bool DeallocPParentToChildStreamChild(PParentToChildStreamChild*);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PPSMContentDownloaderChild* AllocPPSMContentDownloaderChild(
      const uint32_t& aCertType) override;
||||||| merged common ancestors
  virtual PPSMContentDownloaderChild*
  AllocPPSMContentDownloaderChild( const uint32_t& aCertType) override;
=======
  PPSMContentDownloaderChild* AllocPPSMContentDownloaderChild(
      const uint32_t& aCertType);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPPSMContentDownloaderChild(
      PPSMContentDownloaderChild* aDownloader) override;
||||||| merged common ancestors
  virtual bool
  DeallocPPSMContentDownloaderChild(PPSMContentDownloaderChild* aDownloader) override;
=======
  bool DeallocPPSMContentDownloaderChild(
      PPSMContentDownloaderChild* aDownloader);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PExternalHelperAppChild* AllocPExternalHelperAppChild(
      const OptionalURIParams& uri, const nsCString& aMimeContentType,
      const nsCString& aContentDisposition,
      const uint32_t& aContentDispositionHint,
      const nsString& aContentDispositionFilename, const bool& aForceSave,
      const int64_t& aContentLength, const bool& aWasFileChannel,
      const OptionalURIParams& aReferrer, PBrowserChild* aBrowser) override;
||||||| merged common ancestors
  virtual PExternalHelperAppChild*
  AllocPExternalHelperAppChild(const OptionalURIParams& uri,
                               const nsCString& aMimeContentType,
                               const nsCString& aContentDisposition,
                               const uint32_t& aContentDispositionHint,
                               const nsString& aContentDispositionFilename,
                               const bool& aForceSave,
                               const int64_t& aContentLength,
                               const bool& aWasFileChannel,
                               const OptionalURIParams& aReferrer,
                               PBrowserChild* aBrowser) override;
=======
  PExternalHelperAppChild* AllocPExternalHelperAppChild(
      const Maybe<URIParams>& uri,
      const Maybe<mozilla::net::LoadInfoArgs>& aLoadInfoArgs,
      const nsCString& aMimeContentType, const nsCString& aContentDisposition,
      const uint32_t& aContentDispositionHint,
      const nsString& aContentDispositionFilename, const bool& aForceSave,
      const int64_t& aContentLength, const bool& aWasFileChannel,
      const Maybe<URIParams>& aReferrer, PBrowserChild* aBrowser);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPExternalHelperAppChild(
      PExternalHelperAppChild* aService) override;
||||||| merged common ancestors
  virtual bool
  DeallocPExternalHelperAppChild(PExternalHelperAppChild *aService) override;
=======
  bool DeallocPExternalHelperAppChild(PExternalHelperAppChild* aService);
>>>>>>> upstream-releases

  PHandlerServiceChild* AllocPHandlerServiceChild();

  bool DeallocPHandlerServiceChild(PHandlerServiceChild*);

  PMediaChild* AllocPMediaChild();

  bool DeallocPMediaChild(PMediaChild* aActor);

  PPresentationChild* AllocPPresentationChild();

  bool DeallocPPresentationChild(PPresentationChild* aActor);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyPresentationReceiverLaunched(
      PBrowserChild* aIframe, const nsString& aSessionId) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvNotifyPresentationReceiverLaunched(PBrowserChild* aIframe,
                                         const nsString& aSessionId) override;
=======
  mozilla::ipc::IPCResult RecvNotifyPresentationReceiverLaunched(
      PBrowserChild* aIframe, const nsString& aSessionId);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyPresentationReceiverCleanUp(
      const nsString& aSessionId) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvNotifyPresentationReceiverCleanUp(const nsString& aSessionId) override;
=======
  mozilla::ipc::IPCResult RecvNotifyPresentationReceiverCleanUp(
      const nsString& aSessionId);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvNotifyEmptyHTTPCache();

  PSpeechSynthesisChild* AllocPSpeechSynthesisChild();

<<<<<<< HEAD
  virtual bool DeallocPSpeechSynthesisChild(
      PSpeechSynthesisChild* aActor) override;
||||||| merged common ancestors
  virtual bool DeallocPSpeechSynthesisChild(PSpeechSynthesisChild* aActor) override;
=======
  bool DeallocPSpeechSynthesisChild(PSpeechSynthesisChild* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvRegisterChrome(
      InfallibleTArray<ChromePackage>&& packages,
      InfallibleTArray<SubstitutionMapping>&& resources,
      InfallibleTArray<OverrideMapping>&& overrides, const nsCString& locale,
      const bool& reset) override;
  virtual mozilla::ipc::IPCResult RecvRegisterChromeItem(
      const ChromeRegistryItem& item) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvRegisterChrome(InfallibleTArray<ChromePackage>&& packages,
                                                     InfallibleTArray<SubstitutionMapping>&& resources,
                                                     InfallibleTArray<OverrideMapping>&& overrides,
                                                     const nsCString& locale,
                                                     const bool& reset) override;
  virtual mozilla::ipc::IPCResult RecvRegisterChromeItem(const ChromeRegistryItem& item) override;
=======
  mozilla::ipc::IPCResult RecvRegisterChrome(
      InfallibleTArray<ChromePackage>&& packages,
      InfallibleTArray<SubstitutionMapping>&& resources,
      InfallibleTArray<OverrideMapping>&& overrides, const nsCString& locale,
      const bool& reset);
  mozilla::ipc::IPCResult RecvRegisterChromeItem(
      const ChromeRegistryItem& item);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvClearImageCache(
      const bool& privateLoader, const bool& chrome) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvClearImageCache(const bool& privateLoader,
                                                      const bool& chrome) override;
=======
  mozilla::ipc::IPCResult RecvClearImageCache(const bool& privateLoader,
                                              const bool& chrome);
>>>>>>> upstream-releases

  mozilla::jsipc::PJavaScriptChild* AllocPJavaScriptChild();

<<<<<<< HEAD
  virtual bool DeallocPJavaScriptChild(
      mozilla::jsipc::PJavaScriptChild*) override;
||||||| merged common ancestors
  virtual bool DeallocPJavaScriptChild(mozilla::jsipc::PJavaScriptChild*) override;
=======
  bool DeallocPJavaScriptChild(mozilla::jsipc::PJavaScriptChild*);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PRemoteSpellcheckEngineChild* AllocPRemoteSpellcheckEngineChild()
      override;
||||||| merged common ancestors
  virtual PRemoteSpellcheckEngineChild* AllocPRemoteSpellcheckEngineChild() override;
=======
  PRemoteSpellcheckEngineChild* AllocPRemoteSpellcheckEngineChild();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPRemoteSpellcheckEngineChild(
      PRemoteSpellcheckEngineChild*) override;
||||||| merged common ancestors
  virtual bool DeallocPRemoteSpellcheckEngineChild(PRemoteSpellcheckEngineChild*) override;
=======
  bool DeallocPRemoteSpellcheckEngineChild(PRemoteSpellcheckEngineChild*);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvSetOffline(const bool& offline);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvSetConnectivity(
      const bool& connectivity) override;
  virtual mozilla::ipc::IPCResult RecvSetCaptivePortalState(
      const int32_t& state) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvSetConnectivity(const bool& connectivity) override;
  virtual mozilla::ipc::IPCResult RecvSetCaptivePortalState(const int32_t& state) override;
=======
  mozilla::ipc::IPCResult RecvSetConnectivity(const bool& connectivity);
  mozilla::ipc::IPCResult RecvSetCaptivePortalState(const int32_t& state);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvBidiKeyboardNotify(
      const bool& isLangRTL, const bool& haveBidiKeyboards) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvBidiKeyboardNotify(const bool& isLangRTL,
                                                         const bool& haveBidiKeyboards) override;
=======
  mozilla::ipc::IPCResult RecvBidiKeyboardNotify(const bool& isLangRTL,
                                                 const bool& haveBidiKeyboards);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyVisited(
      nsTArray<URIParams>&& aURIs) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifyVisited(nsTArray<URIParams>&& aURIs) override;
=======
  mozilla::ipc::IPCResult RecvNotifyVisited(nsTArray<URIParams>&& aURIs);
>>>>>>> upstream-releases

  // auto remove when alertfinished is received.
<<<<<<< HEAD
  nsresult AddRemoteAlertObserver(const nsString& aData,
                                  nsIObserver* aObserver);
||||||| merged common ancestors
  nsresult AddRemoteAlertObserver(const nsString& aData, nsIObserver* aObserver);
=======
  nsresult AddRemoteAlertObserver(const nsString& aData,
                                  nsIObserver* aObserver);

  mozilla::ipc::IPCResult RecvPreferenceUpdate(const Pref& aPref);
  mozilla::ipc::IPCResult RecvVarUpdate(const GfxVarUpdate& pref);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPreferenceUpdate(
      const Pref& aPref) override;
  virtual mozilla::ipc::IPCResult RecvVarUpdate(
      const GfxVarUpdate& pref) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvPreferenceUpdate(const Pref& aPref) override;
  virtual mozilla::ipc::IPCResult RecvVarUpdate(const GfxVarUpdate& pref) override;
=======
  mozilla::ipc::IPCResult RecvUpdatePerfStatsCollectionMask(
      const uint64_t& aMask);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvDataStoragePut(
      const nsString& aFilename, const DataStorageItem& aItem) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvDataStoragePut(const nsString& aFilename,
                                                     const DataStorageItem& aItem) override;
=======
  mozilla::ipc::IPCResult RecvCollectPerfStatsJSON(
      CollectPerfStatsJSONResolver&& aResolver);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvDataStorageRemove(
      const nsString& aFilename, const nsCString& aKey,
      const DataStorageType& aType) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvDataStorageRemove(const nsString& aFilename,
                                                        const nsCString& aKey,
                                                        const DataStorageType& aType) override;
=======
  mozilla::ipc::IPCResult RecvDataStoragePut(const nsString& aFilename,
                                             const DataStorageItem& aItem);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvDataStorageClear(
      const nsString& aFilename) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvDataStorageClear(const nsString& aFilename) override;
=======
  mozilla::ipc::IPCResult RecvDataStorageRemove(const nsString& aFilename,
                                                const nsCString& aKey,
                                                const DataStorageType& aType);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyAlertsObserver(
      const nsCString& aType, const nsString& aData) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifyAlertsObserver(const nsCString& aType,
                                                           const nsString& aData) override;
=======
  mozilla::ipc::IPCResult RecvDataStorageClear(const nsString& aFilename);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvLoadProcessScript(
      const nsString& aURL) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvLoadProcessScript(const nsString& aURL) override;
=======
  mozilla::ipc::IPCResult RecvNotifyAlertsObserver(const nsCString& aType,
                                                   const nsString& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvAsyncMessage(
      const nsString& aMsg, InfallibleTArray<CpowEntry>&& aCpows,
      const IPC::Principal& aPrincipal,
      const ClonedMessageData& aData) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvAsyncMessage(const nsString& aMsg,
                                                   InfallibleTArray<CpowEntry>&& aCpows,
                                                   const IPC::Principal& aPrincipal,
                                                   const ClonedMessageData& aData) override;
=======
  mozilla::ipc::IPCResult RecvLoadProcessScript(const nsString& aURL);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvRegisterStringBundles(
      nsTArray<StringBundleDescriptor>&& stringBundles) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult RecvRegisterStringBundles(nsTArray<StringBundleDescriptor>&& stringBundles) override;
=======
  mozilla::ipc::IPCResult RecvAsyncMessage(const nsString& aMsg,
                                           InfallibleTArray<CpowEntry>&& aCpows,
                                           const IPC::Principal& aPrincipal,
                                           const ClonedMessageData& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvUpdateSharedData(
      const FileDescriptor& aMapFile, const uint32_t& aMapSize,
      nsTArray<IPCBlob>&& aBlobs, nsTArray<nsCString>&& aChangedKeys) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult RecvUpdateSharedData(const FileDescriptor& aMapFile,
                                               const uint32_t& aMapSize,
                                               nsTArray<IPCBlob>&& aBlobs,
                                               nsTArray<nsCString>&& aChangedKeys) override;
=======
  mozilla::ipc::IPCResult RecvRegisterStringBundles(
      nsTArray<StringBundleDescriptor>&& stringBundles);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvGeolocationUpdate(
      nsIDOMGeoPosition* aPosition) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvGeolocationUpdate(nsIDOMGeoPosition* aPosition) override;
=======
  mozilla::ipc::IPCResult RecvUpdateSharedData(
      const FileDescriptor& aMapFile, const uint32_t& aMapSize,
      nsTArray<IPCBlob>&& aBlobs, nsTArray<nsCString>&& aChangedKeys);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvGeolocationError(
      const uint16_t& errorCode) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvGeolocationError(const uint16_t& errorCode) override;
=======
  mozilla::ipc::IPCResult RecvFontListChanged();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvUpdateDictionaryList(
      InfallibleTArray<nsString>&& aDictionaries) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvUpdateDictionaryList(InfallibleTArray<nsString>&& aDictionaries) override;
=======
  mozilla::ipc::IPCResult RecvGeolocationUpdate(nsIDOMGeoPosition* aPosition);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvUpdateFontList(
      InfallibleTArray<SystemFontListEntry>&& aFontList) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvUpdateFontList(InfallibleTArray<SystemFontListEntry>&& aFontList) override;
=======
  // MOZ_CAN_RUN_SCRIPT_BOUNDARY because we don't have MOZ_CAN_RUN_SCRIPT bits
  // in IPC code yet.
  MOZ_CAN_RUN_SCRIPT_BOUNDARY
  mozilla::ipc::IPCResult RecvGeolocationError(const uint16_t& errorCode);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvUpdateAppLocales(
      nsTArray<nsCString>&& aAppLocales) override;
  virtual mozilla::ipc::IPCResult RecvUpdateRequestedLocales(
      nsTArray<nsCString>&& aRequestedLocales) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvUpdateAppLocales(nsTArray<nsCString>&& aAppLocales) override;
  virtual mozilla::ipc::IPCResult RecvUpdateRequestedLocales(nsTArray<nsCString>&& aRequestedLocales) override;
=======
  mozilla::ipc::IPCResult RecvUpdateDictionaryList(
      InfallibleTArray<nsString>&& aDictionaries);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvClearSiteDataReloadNeeded(
      const nsString& aOrigin) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvClearSiteDataReloadNeeded(const nsString& aOrigin) override;
=======
  mozilla::ipc::IPCResult RecvUpdateFontList(
      InfallibleTArray<SystemFontListEntry>&& aFontList);
  mozilla::ipc::IPCResult RecvRebuildFontList();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvAddPermission(
      const IPC::Permission& permission) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvAddPermission(const IPC::Permission& permission) override;
=======
  mozilla::ipc::IPCResult RecvUpdateAppLocales(
      nsTArray<nsCString>&& aAppLocales);
  mozilla::ipc::IPCResult RecvUpdateRequestedLocales(
      nsTArray<nsCString>&& aRequestedLocales);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvAddPermission(const IPC::Permission& permission);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvFlushMemory(
      const nsString& reason) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvFlushMemory(const nsString& reason) override;
=======
  mozilla::ipc::IPCResult RecvRemoveAllPermissions();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvActivateA11y(
      const uint32_t& aMainChromeTid, const uint32_t& aMsaaID) override;
  virtual mozilla::ipc::IPCResult RecvShutdownA11y() override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvActivateA11y(const uint32_t& aMainChromeTid,
                                                   const uint32_t& aMsaaID) override;
  virtual mozilla::ipc::IPCResult RecvShutdownA11y() override;
=======
  mozilla::ipc::IPCResult RecvFlushMemory(const nsString& reason);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvActivateA11y(const uint32_t& aMainChromeTid,
                                           const uint32_t& aMsaaID);
  mozilla::ipc::IPCResult RecvShutdownA11y();

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvAppInfo(
      const nsCString& version, const nsCString& buildID, const nsCString& name,
      const nsCString& UAName, const nsCString& ID, const nsCString& vendor,
      const nsCString& sourceURL) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvAppInfo(const nsCString& version, const nsCString& buildID,
                                              const nsCString& name, const nsCString& UAName,
                                              const nsCString& ID, const nsCString& vendor,
                                              const nsCString& sourceURL) override;
=======
  mozilla::ipc::IPCResult RecvGarbageCollect();
  mozilla::ipc::IPCResult RecvCycleCollect();
  mozilla::ipc::IPCResult RecvUnlinkGhosts();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvRemoteType(
      const nsString& aRemoteType) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvRemoteType(const nsString& aRemoteType) override;
=======
  mozilla::ipc::IPCResult RecvAppInfo(
      const nsCString& version, const nsCString& buildID, const nsCString& name,
      const nsCString& UAName, const nsCString& ID, const nsCString& vendor,
      const nsCString& sourceURL);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvRemoteType(const nsString& aRemoteType);

  // Call RemoteTypePrefix() on the result to remove URIs if you want to use
  // this for telemetry.
  const nsAString& GetRemoteType() const;

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvInitServiceWorkers(
      const ServiceWorkerConfiguration& aConfig) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvInitServiceWorkers(const ServiceWorkerConfiguration& aConfig) override;
=======
  mozilla::ipc::IPCResult RecvInitServiceWorkers(
      const ServiceWorkerConfiguration& aConfig);

  mozilla::ipc::IPCResult RecvInitBlobURLs(
      nsTArray<BlobURLRegistrationData>&& aRegistations);

  mozilla::ipc::IPCResult RecvInitJSWindowActorInfos(
      nsTArray<JSWindowActorInfo>&& aInfos);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvInitBlobURLs(
      nsTArray<BlobURLRegistrationData>&& aRegistations) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvInitBlobURLs(nsTArray<BlobURLRegistrationData>&& aRegistations) override;
=======
  mozilla::ipc::IPCResult RecvUnregisterJSWindowActor(const nsString& aName);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvLastPrivateDocShellDestroyed();

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyProcessPriorityChanged(
      const hal::ProcessPriority& aPriority) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvNotifyProcessPriorityChanged(const hal::ProcessPriority& aPriority) override;
=======
  mozilla::ipc::IPCResult RecvNotifyProcessPriorityChanged(
      const hal::ProcessPriority& aPriority);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvMinimizeMemoryUsage();

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvLoadAndRegisterSheet(
      const URIParams& aURI, const uint32_t& aType) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvLoadAndRegisterSheet(const URIParams& aURI,
                                                           const uint32_t& aType) override;
=======
  mozilla::ipc::IPCResult RecvLoadAndRegisterSheet(const URIParams& aURI,
                                                   const uint32_t& aType);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvUnregisterSheet(
      const URIParams& aURI, const uint32_t& aType) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvUnregisterSheet(const URIParams& aURI,
                                                      const uint32_t& aType) override;
=======
  mozilla::ipc::IPCResult RecvUnregisterSheet(const URIParams& aURI,
                                              const uint32_t& aType);
>>>>>>> upstream-releases

  void AddIdleObserver(nsIObserver* aObserver, uint32_t aIdleTimeInS);

  void RemoveIdleObserver(nsIObserver* aObserver, uint32_t aIdleTimeInS);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyIdleObserver(
      const uint64_t& aObserver, const nsCString& aTopic,
      const nsString& aData) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifyIdleObserver(const uint64_t& aObserver,
                                                         const nsCString& aTopic,
                                                         const nsString& aData) override;
=======
  mozilla::ipc::IPCResult RecvNotifyIdleObserver(const uint64_t& aObserver,
                                                 const nsCString& aTopic,
                                                 const nsString& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvUpdateWindow(
      const uintptr_t& aChildId) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvUpdateWindow(const uintptr_t& aChildId) override;
=======
  mozilla::ipc::IPCResult RecvUpdateWindow(const uintptr_t& aChildId);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvDomainSetChanged(
      const uint32_t& aSetType, const uint32_t& aChangeType,
      const OptionalURIParams& aDomain) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvDomainSetChanged(const uint32_t& aSetType,
                                                       const uint32_t& aChangeType,
                                                       const OptionalURIParams& aDomain) override;
=======
  mozilla::ipc::IPCResult RecvDomainSetChanged(const uint32_t& aSetType,
                                               const uint32_t& aChangeType,
                                               const Maybe<URIParams>& aDomain);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvShutdown();

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvInvokeDragSession(
      nsTArray<IPCDataTransfer>&& aTransfers, const uint32_t& aAction) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvInvokeDragSession(nsTArray<IPCDataTransfer>&& aTransfers,
                        const uint32_t& aAction) override;
=======
  mozilla::ipc::IPCResult RecvInvokeDragSession(
      nsTArray<IPCDataTransfer>&& aTransfers, const uint32_t& aAction);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvEndDragSession(
      const bool& aDoneDrag, const bool& aUserCancelled,
      const mozilla::LayoutDeviceIntPoint& aEndDragPoint,
      const uint32_t& aKeyModifiers) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvEndDragSession(const bool& aDoneDrag,
                                                     const bool& aUserCancelled,
                                                     const mozilla::LayoutDeviceIntPoint& aEndDragPoint,
                                                     const uint32_t& aKeyModifiers) override;
=======
  MOZ_CAN_RUN_SCRIPT_BOUNDARY
  mozilla::ipc::IPCResult RecvEndDragSession(
      const bool& aDoneDrag, const bool& aUserCancelled,
      const mozilla::LayoutDeviceIntPoint& aEndDragPoint,
      const uint32_t& aKeyModifiers);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPush(const nsCString& aScope,
                                           const IPC::Principal& aPrincipal,
                                           const nsString& aMessageId) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvPush(const nsCString& aScope,
           const IPC::Principal& aPrincipal,
           const nsString& aMessageId) override;
=======
  mozilla::ipc::IPCResult RecvPush(const nsCString& aScope,
                                   const IPC::Principal& aPrincipal,
                                   const nsString& aMessageId);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPushWithData(
      const nsCString& aScope, const IPC::Principal& aPrincipal,
      const nsString& aMessageId, InfallibleTArray<uint8_t>&& aData) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvPushWithData(const nsCString& aScope,
                   const IPC::Principal& aPrincipal,
                   const nsString& aMessageId,
                   InfallibleTArray<uint8_t>&& aData) override;
=======
  mozilla::ipc::IPCResult RecvPushWithData(const nsCString& aScope,
                                           const IPC::Principal& aPrincipal,
                                           const nsString& aMessageId,
                                           InfallibleTArray<uint8_t>&& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPushSubscriptionChange(
      const nsCString& aScope, const IPC::Principal& aPrincipal) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvPushSubscriptionChange(const nsCString& aScope,
                             const IPC::Principal& aPrincipal) override;
=======
  mozilla::ipc::IPCResult RecvPushSubscriptionChange(
      const nsCString& aScope, const IPC::Principal& aPrincipal);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPushError(
      const nsCString& aScope, const IPC::Principal& aPrincipal,
      const nsString& aMessage, const uint32_t& aFlags) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvPushError(const nsCString& aScope, const IPC::Principal& aPrincipal,
                const nsString& aMessage, const uint32_t& aFlags) override;
=======
  mozilla::ipc::IPCResult RecvPushError(const nsCString& aScope,
                                        const IPC::Principal& aPrincipal,
                                        const nsString& aMessage,
                                        const uint32_t& aFlags);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyPushSubscriptionModifiedObservers(
      const nsCString& aScope, const IPC::Principal& aPrincipal) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvNotifyPushSubscriptionModifiedObservers(const nsCString& aScope,
                                              const IPC::Principal& aPrincipal) override;
=======
  mozilla::ipc::IPCResult RecvNotifyPushSubscriptionModifiedObservers(
      const nsCString& aScope, const IPC::Principal& aPrincipal);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvActivate(PBrowserChild* aTab);

  mozilla::ipc::IPCResult RecvDeactivate(PBrowserChild* aTab);

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvRefreshScreens(
      nsTArray<ScreenDetails>&& aScreens) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvRefreshScreens(nsTArray<ScreenDetails>&& aScreens) override;
=======
  mozilla::ipc::IPCResult RecvRefreshScreens(
      nsTArray<ScreenDetails>&& aScreens);

  mozilla::ipc::IPCResult RecvNetworkLinkTypeChange(const uint32_t& aType);
  uint32_t NetworkLinkType() const { return mNetworkLinkType; }
>>>>>>> upstream-releases

  // Get the directory for IndexedDB files. We query the parent for this and
  // cache the value
  nsString& GetIndexedDBPath();

  ContentParentId GetID() const { return mID; }

#if defined(XP_WIN) && defined(ACCESSIBILITY)
  uint32_t GetChromeMainThreadId() const { return mMainChromeTid; }

  uint32_t GetMsaaID() const { return mMsaaID; }
#endif

  bool IsForBrowser() const { return mIsForBrowser; }

<<<<<<< HEAD
  virtual PFileDescriptorSetChild* SendPFileDescriptorSetConstructor(
      const FileDescriptor&) override;

  virtual PFileDescriptorSetChild* AllocPFileDescriptorSetChild(
      const FileDescriptor&) override;
||||||| merged common ancestors
  virtual PFileDescriptorSetChild*
  SendPFileDescriptorSetConstructor(const FileDescriptor&) override;

  virtual PFileDescriptorSetChild*
  AllocPFileDescriptorSetChild(const FileDescriptor&) override;
=======
  PFileDescriptorSetChild* SendPFileDescriptorSetConstructor(
      const FileDescriptor&);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPFileDescriptorSetChild(
      PFileDescriptorSetChild*) override;
||||||| merged common ancestors
  virtual bool
  DeallocPFileDescriptorSetChild(PFileDescriptorSetChild*) override;
=======
  PFileDescriptorSetChild* AllocPFileDescriptorSetChild(const FileDescriptor&);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool SendPBrowserConstructor(
      PBrowserChild* actor, const TabId& aTabId, const TabId& aSameTabGroupAs,
      const IPCTabContext& context, const uint32_t& chromeFlags,
      const ContentParentId& aCpID, const bool& aIsForBrowser) override;
||||||| merged common ancestors
  virtual bool SendPBrowserConstructor(PBrowserChild* actor,
                                       const TabId& aTabId,
                                       const TabId& aSameTabGroupAs,
                                       const IPCTabContext& context,
                                       const uint32_t& chromeFlags,
                                       const ContentParentId& aCpID,
                                       const bool& aIsForBrowser) override;
=======
  bool DeallocPFileDescriptorSetChild(PFileDescriptorSetChild*);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPBrowserConstructor(
      PBrowserChild* aCctor, const TabId& aTabId, const TabId& aSameTabGroupAs,
      const IPCTabContext& aContext, const uint32_t& aChromeFlags,
      const ContentParentId& aCpID, const bool& aIsForBrowser) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvPBrowserConstructor(PBrowserChild* aCctor,
                                                          const TabId& aTabId,
                                                          const TabId& aSameTabGroupAs,
                                                          const IPCTabContext& aContext,
                                                          const uint32_t& aChromeFlags,
                                                          const ContentParentId& aCpID,
                                                          const bool& aIsForBrowser) override;
=======
  mozilla::ipc::IPCResult RecvConstructBrowser(
      ManagedEndpoint<PBrowserChild>&& aBrowserEp, const TabId& aTabId,
      const TabId& aSameTabGroupAs, const IPCTabContext& aContext,
      BrowsingContext* aBrowsingContext, const uint32_t& aChromeFlags,
      const ContentParentId& aCpID, const bool& aIsForBrowser,
      const bool& aIsTopLevel);
>>>>>>> upstream-releases

  FORWARD_SHMEM_ALLOCATOR_TO(PContentChild)

  void GetAvailableDictionaries(InfallibleTArray<nsString>& aDictionaries);

<<<<<<< HEAD
  PBrowserOrId GetBrowserOrId(TabChild* aTabChild);
||||||| merged common ancestors
  PBrowserOrId
  GetBrowserOrId(TabChild* aTabChild);
=======
  PBrowserOrId GetBrowserOrId(BrowserChild* aBrowserChild);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual POfflineCacheUpdateChild* AllocPOfflineCacheUpdateChild(
      const URIParams& manifestURI, const URIParams& documentURI,
      const PrincipalInfo& aLoadingPrincipalInfo,
      const bool& stickDocument) override;
||||||| merged common ancestors
  virtual POfflineCacheUpdateChild*
  AllocPOfflineCacheUpdateChild(const URIParams& manifestURI,
                                const URIParams& documentURI,
                                const PrincipalInfo& aLoadingPrincipalInfo,
                                const bool& stickDocument) override;
=======
  POfflineCacheUpdateChild* AllocPOfflineCacheUpdateChild(
      const URIParams& manifestURI, const URIParams& documentURI,
      const PrincipalInfo& aLoadingPrincipalInfo, const bool& stickDocument);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPOfflineCacheUpdateChild(
      POfflineCacheUpdateChild* offlineCacheUpdate) override;
||||||| merged common ancestors
  virtual bool
  DeallocPOfflineCacheUpdateChild(POfflineCacheUpdateChild* offlineCacheUpdate) override;
=======
  bool DeallocPOfflineCacheUpdateChild(
      POfflineCacheUpdateChild* offlineCacheUpdate);
>>>>>>> upstream-releases

  PWebrtcGlobalChild* AllocPWebrtcGlobalChild();

<<<<<<< HEAD
  virtual bool DeallocPWebrtcGlobalChild(PWebrtcGlobalChild* aActor) override;
||||||| merged common ancestors
  virtual bool DeallocPWebrtcGlobalChild(PWebrtcGlobalChild *aActor) override;
=======
  bool DeallocPWebrtcGlobalChild(PWebrtcGlobalChild* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PContentPermissionRequestChild* AllocPContentPermissionRequestChild(
      const InfallibleTArray<PermissionRequest>& aRequests,
      const IPC::Principal& aPrincipal,
      const IPC::Principal& aTopLevelPrincipal,
      const bool& aIsHandlingUserInput, const TabId& aTabId) override;
  virtual bool DeallocPContentPermissionRequestChild(
      PContentPermissionRequestChild* actor) override;
||||||| merged common ancestors
  virtual PContentPermissionRequestChild*
  AllocPContentPermissionRequestChild(const InfallibleTArray<PermissionRequest>& aRequests,
                                      const IPC::Principal& aPrincipal,
                                      const bool& aIsHandlingUserInput,
                                      const TabId& aTabId) override;
  virtual bool
  DeallocPContentPermissionRequestChild(PContentPermissionRequestChild* actor) override;
=======
  PContentPermissionRequestChild* AllocPContentPermissionRequestChild(
      const InfallibleTArray<PermissionRequest>& aRequests,
      const IPC::Principal& aPrincipal,
      const IPC::Principal& aTopLevelPrincipal,
      const bool& aIsHandlingUserInput, const bool& aDocumentHasUserInput,
      const DOMTimeStamp aPageLoadTimestamp, const TabId& aTabId);
  bool DeallocPContentPermissionRequestChild(
      PContentPermissionRequestChild* actor);
>>>>>>> upstream-releases

  // Windows specific - set up audio session
<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvSetAudioSessionData(
      const nsID& aId, const nsString& aDisplayName,
      const nsString& aIconPath) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvSetAudioSessionData(const nsID& aId,
                          const nsString& aDisplayName,
                          const nsString& aIconPath) override;

=======
  mozilla::ipc::IPCResult RecvSetAudioSessionData(const nsID& aId,
                                                  const nsString& aDisplayName,
                                                  const nsString& aIconPath);
>>>>>>> upstream-releases

  // GetFiles for WebKit/Blink FileSystem API and Directory API must run on the
  // parent process.
  void CreateGetFilesRequest(const nsAString& aDirectoryPath,
                             bool aRecursiveFlag, nsID& aUUID,
                             GetFilesHelperChild* aChild);

  void DeleteGetFilesRequest(nsID& aUUID, GetFilesHelperChild* aChild);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvGetFilesResponse(
      const nsID& aUUID, const GetFilesResponseResult& aResult) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvGetFilesResponse(const nsID& aUUID,
                       const GetFilesResponseResult& aResult) override;
=======
  mozilla::ipc::IPCResult RecvGetFilesResponse(
      const nsID& aUUID, const GetFilesResponseResult& aResult);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvBlobURLRegistration(
      const nsCString& aURI, const IPCBlob& aBlob,
      const IPC::Principal& aPrincipal) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvBlobURLRegistration(const nsCString& aURI, const IPCBlob& aBlob,
                          const IPC::Principal& aPrincipal) override;
=======
  mozilla::ipc::IPCResult RecvBlobURLRegistration(
      const nsCString& aURI, const IPCBlob& aBlob,
      const IPC::Principal& aPrincipal);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvBlobURLUnregistration(
      const nsCString& aURI) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvBlobURLUnregistration(const nsCString& aURI) override;
=======
  mozilla::ipc::IPCResult RecvBlobURLUnregistration(const nsCString& aURI);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvFileCreationResponse(
      const nsID& aUUID, const FileCreationResult& aResult) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvFileCreationResponse(const nsID& aUUID,
                           const FileCreationResult& aResult) override;
=======
  mozilla::ipc::IPCResult RecvRequestMemoryReport(
      const uint32_t& generation, const bool& anonymize,
      const bool& minimizeMemoryUsage, const Maybe<FileDescriptor>& DMDFile);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvRequestMemoryReport(
      const uint32_t& generation, const bool& anonymize,
      const bool& minimizeMemoryUsage, const MaybeFileDesc& DMDFile) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvRequestMemoryReport(
          const uint32_t& generation,
          const bool& anonymize,
          const bool& minimizeMemoryUsage,
          const MaybeFileDesc& DMDFile) override;
=======
  mozilla::ipc::IPCResult RecvSetXPCOMProcessAttributes(
      const XPCOMInitData& aXPCOMInit, const StructuredCloneData& aInitialData,
      nsTArray<LookAndFeelInt>&& aLookAndFeelIntCache,
      nsTArray<SystemFontListEntry>&& aFontList,
      const Maybe<base::SharedMemoryHandle>& aSharedUASheetHandle,
      const uintptr_t& aSharedUASheetAddress);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvSetXPCOMProcessAttributes(
      const XPCOMInitData& aXPCOMInit, const StructuredCloneData& aInitialData,
      nsTArray<LookAndFeelInt>&& aLookAndFeelIntCache,
      nsTArray<SystemFontListEntry>&& aFontList) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvSetXPCOMProcessAttributes(const XPCOMInitData& aXPCOMInit,
                                const StructuredCloneData& aInitialData,
                                nsTArray<LookAndFeelInt>&& aLookAndFeelIntCache,
                                nsTArray<SystemFontListEntry>&& aFontList) override;
=======
  mozilla::ipc::IPCResult RecvProvideAnonymousTemporaryFile(
      const uint64_t& aID, const FileDescOrError& aFD);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvProvideAnonymousTemporaryFile(
      const uint64_t& aID, const FileDescOrError& aFD) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvProvideAnonymousTemporaryFile(const uint64_t& aID, const FileDescOrError& aFD) override;
=======
  mozilla::ipc::IPCResult RecvSetPermissionsWithKey(
      const nsCString& aPermissionKey, nsTArray<IPC::Permission>&& aPerms);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvSetPermissionsWithKey(
      const nsCString& aPermissionKey,
      nsTArray<IPC::Permission>&& aPerms) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvSetPermissionsWithKey(const nsCString& aPermissionKey,
                            nsTArray<IPC::Permission>&& aPerms) override;
=======
  mozilla::ipc::IPCResult RecvShareCodeCoverageMutex(
      const CrossProcessMutexHandle& aHandle);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvShareCodeCoverageMutex(
      const CrossProcessMutexHandle& aHandle) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvShareCodeCoverageMutex(const CrossProcessMutexHandle& aHandle) override;
=======
  mozilla::ipc::IPCResult RecvFlushCodeCoverageCounters(
      FlushCodeCoverageCountersResolver&& aResolver);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvFlushCodeCoverageCounters(
      FlushCodeCoverageCountersResolver&& aResolver) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvFlushCodeCoverageCounters(FlushCodeCoverageCountersResolver&& aResolver) override;
=======
  mozilla::ipc::IPCResult RecvGetMemoryUniqueSetSize(
      GetMemoryUniqueSetSizeResolver&& aResolver);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvSetInputEventQueueEnabled() override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvSetInputEventQueueEnabled() override;
=======
  mozilla::ipc::IPCResult RecvSetInputEventQueueEnabled();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvFlushInputEventQueue() override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvFlushInputEventQueue() override;
=======
  mozilla::ipc::IPCResult RecvFlushInputEventQueue();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvSuspendInputEventQueue() override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvSuspendInputEventQueue() override;
=======
  mozilla::ipc::IPCResult RecvSuspendInputEventQueue();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvResumeInputEventQueue() override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvResumeInputEventQueue() override;
=======
  mozilla::ipc::IPCResult RecvResumeInputEventQueue();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvAddDynamicScalars(
      nsTArray<DynamicScalarDefinition>&& aDefs) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvAddDynamicScalars(nsTArray<DynamicScalarDefinition>&& aDefs) override;
=======
  mozilla::ipc::IPCResult RecvAddDynamicScalars(
      nsTArray<DynamicScalarDefinition>&& aDefs);
>>>>>>> upstream-releases

#if defined(XP_WIN) && defined(ACCESSIBILITY)
  bool SendGetA11yContentId();
#endif  // defined(XP_WIN) && defined(ACCESSIBILITY)

  // Get a reference to the font list passed from the chrome process,
  // for use during gfx initialization.
  InfallibleTArray<mozilla::dom::SystemFontListEntry>& SystemFontList() {
    return mFontList;
  }

  // PURLClassifierChild
<<<<<<< HEAD
  virtual PURLClassifierChild* AllocPURLClassifierChild(
      const Principal& aPrincipal, const bool& aUseTrackingProtection,
      bool* aSuccess) override;
  virtual bool DeallocPURLClassifierChild(PURLClassifierChild* aActor) override;
||||||| merged common ancestors
  virtual PURLClassifierChild*
  AllocPURLClassifierChild(const Principal& aPrincipal,
                           const bool& aUseTrackingProtection,
                           bool* aSuccess) override;
  virtual bool
  DeallocPURLClassifierChild(PURLClassifierChild* aActor) override;
=======
  PURLClassifierChild* AllocPURLClassifierChild(const Principal& aPrincipal,
                                                bool* aSuccess);
  bool DeallocPURLClassifierChild(PURLClassifierChild* aActor);
>>>>>>> upstream-releases

  // PURLClassifierLocalChild
<<<<<<< HEAD
  virtual PURLClassifierLocalChild* AllocPURLClassifierLocalChild(
      const URIParams& aUri, const nsCString& aTables) override;
  virtual bool DeallocPURLClassifierLocalChild(
      PURLClassifierLocalChild* aActor) override;
||||||| merged common ancestors
  virtual PURLClassifierLocalChild*
  AllocPURLClassifierLocalChild(const URIParams& aUri,
                                const nsCString& aTables) override;
  virtual bool
  DeallocPURLClassifierLocalChild(PURLClassifierLocalChild* aActor) override;
=======
  PURLClassifierLocalChild* AllocPURLClassifierLocalChild(
      const URIParams& aUri,
      const nsTArray<IPCURLClassifierFeature>& aFeatures);
  bool DeallocPURLClassifierLocalChild(PURLClassifierLocalChild* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PLoginReputationChild* AllocPLoginReputationChild(
      const URIParams& aUri) override;
||||||| merged common ancestors
  virtual PLoginReputationChild*
  AllocPLoginReputationChild(const URIParams& aUri) override;
=======
  PLoginReputationChild* AllocPLoginReputationChild(const URIParams& aUri);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPLoginReputationChild(
      PLoginReputationChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPLoginReputationChild(PLoginReputationChild* aActor) override;
=======
  bool DeallocPLoginReputationChild(PLoginReputationChild* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsTArray<LookAndFeelInt>& LookAndFeelCache() { return mLookAndFeelCache; }
||||||| merged common ancestors
  nsTArray<LookAndFeelInt>&
  LookAndFeelCache() {
    return mLookAndFeelCache;
  }
=======
  PSessionStorageObserverChild* AllocPSessionStorageObserverChild();

  bool DeallocPSessionStorageObserverChild(
      PSessionStorageObserverChild* aActor);

  nsTArray<LookAndFeelInt>& LookAndFeelCache() { return mLookAndFeelCache; }
>>>>>>> upstream-releases

  /**
   * Helper function for protocols that use the GPU process when available.
   * Overrides FatalError to just be a warning when communicating with the
   * GPU process since we don't want to crash the content process when the
   * GPU process crashes.
   */
  static void FatalErrorIfNotUsingGPUProcess(const char* const aErrorMsg,
                                             base::ProcessId aOtherPid);

<<<<<<< HEAD
  // This method is used by FileCreatorHelper for the creation of a BlobImpl.
  void FileCreationRequest(nsID& aUUID, FileCreatorHelper* aHelper,
                           const nsAString& aFullPath, const nsAString& aType,
                           const nsAString& aName,
                           const Optional<int64_t>& aLastModified,
                           bool aExistenceCheck, bool aIsFromNsIFile);

||||||| merged common ancestors
  // This method is used by FileCreatorHelper for the creation of a BlobImpl.
  void
  FileCreationRequest(nsID& aUUID, FileCreatorHelper* aHelper,
                      const nsAString& aFullPath, const nsAString& aType,
                      const nsAString& aName,
                      const Optional<int64_t>& aLastModified,
                      bool aExistenceCheck, bool aIsFromNsIFile);

=======
>>>>>>> upstream-releases
  typedef std::function<void(PRFileDesc*)> AnonymousTemporaryFileCallback;
<<<<<<< HEAD
  nsresult AsyncOpenAnonymousTemporaryFile(
      const AnonymousTemporaryFileCallback& aCallback);
||||||| merged common ancestors
  nsresult AsyncOpenAnonymousTemporaryFile(const AnonymousTemporaryFileCallback& aCallback);
=======
  nsresult AsyncOpenAnonymousTemporaryFile(
      const AnonymousTemporaryFileCallback& aCallback);

  already_AddRefed<nsIEventTarget> GetEventTargetFor(
      BrowserChild* aBrowserChild);

  mozilla::ipc::IPCResult RecvSetPluginList(
      const uint32_t& aPluginEpoch, nsTArray<PluginTag>&& aPluginTags,
      nsTArray<FakePluginTag>&& aFakePluginTags);

  PClientOpenWindowOpChild* AllocPClientOpenWindowOpChild(
      const ClientOpenWindowArgs& aArgs);

  mozilla::ipc::IPCResult RecvPClientOpenWindowOpConstructor(
      PClientOpenWindowOpChild* aActor,
      const ClientOpenWindowArgs& aArgs) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual already_AddRefed<nsIEventTarget> GetEventTargetFor(
      TabChild* aTabChild) override;
||||||| merged common ancestors
  virtual already_AddRefed<nsIEventTarget> GetEventTargetFor(TabChild* aTabChild) override;
=======
  bool DeallocPClientOpenWindowOpChild(PClientOpenWindowOpChild* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvSetPluginList(
      const uint32_t& aPluginEpoch, nsTArray<PluginTag>&& aPluginTags,
      nsTArray<FakePluginTag>&& aFakePluginTags) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvSetPluginList(const uint32_t& aPluginEpoch,
                    nsTArray<PluginTag>&& aPluginTags,
                    nsTArray<FakePluginTag>&& aFakePluginTags) override;
=======
  mozilla::ipc::IPCResult RecvSaveRecording(const FileDescriptor& aFile);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PClientOpenWindowOpChild* AllocPClientOpenWindowOpChild(
      const ClientOpenWindowArgs& aArgs) override;
||||||| merged common ancestors
  virtual PClientOpenWindowOpChild*
  AllocPClientOpenWindowOpChild(const ClientOpenWindowArgs& aArgs) override;
=======
  mozilla::ipc::IPCResult RecvCrossProcessRedirect(
      const uint32_t& aRegistrarId, nsIURI* aURI, const uint32_t& aNewLoadFlags,
      const Maybe<LoadInfoArgs>& aLoadInfoForwarder, const uint64_t& aChannelId,
      nsIURI* aOriginalURI, const uint64_t& aIdentifier,
      const uint32_t& aRedirectMode);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPClientOpenWindowOpConstructor(
      PClientOpenWindowOpChild* aActor,
      const ClientOpenWindowArgs& aArgs) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvPClientOpenWindowOpConstructor(PClientOpenWindowOpChild* aActor,
                                     const ClientOpenWindowArgs& aArgs) override;
=======
  mozilla::ipc::IPCResult RecvStartDelayedAutoplayMediaComponents(
      BrowsingContext* aContext);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPClientOpenWindowOpChild(
      PClientOpenWindowOpChild* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPClientOpenWindowOpChild(PClientOpenWindowOpChild* aActor) override;
=======
  mozilla::ipc::IPCResult RecvSetMediaMuted(BrowsingContext* aContext,
                                            bool aMuted);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvSaveRecording(
      const FileDescriptor& aFile) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvSaveRecording(const FileDescriptor& aFile) override;
=======
  void HoldBrowsingContextGroup(BrowsingContextGroup* aBCG);
>>>>>>> upstream-releases

#ifdef NIGHTLY_BUILD
  // Fetch the current number of pending input events.
  //
  // NOTE: This method performs an atomic read, and is safe to call from all
  // threads.
  uint32_t GetPendingInputEvents() { return mPendingInputEvents; }
#endif

 private:
  static void ForceKillTimerCallback(nsITimer* aTimer, void* aClosure);
  void StartForceKillTimer();

  void ShutdownInternal();

  mozilla::ipc::IPCResult GetResultForRenderingInitFailure(
      base::ProcessId aOtherPid);

  virtual void ActorDestroy(ActorDestroyReason why) override;

  virtual void ProcessingError(Result aCode, const char* aReason) override;

<<<<<<< HEAD
  virtual already_AddRefed<nsIEventTarget> GetConstructedEventTarget(
      const Message& aMsg) override;
||||||| merged common ancestors
  virtual already_AddRefed<nsIEventTarget>
  GetConstructedEventTarget(const Message& aMsg) override;
=======
  virtual already_AddRefed<nsIEventTarget> GetSpecificMessageEventTarget(
      const Message& aMsg) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual already_AddRefed<nsIEventTarget> GetSpecificMessageEventTarget(
      const Message& aMsg) override;
||||||| merged common ancestors
  virtual already_AddRefed<nsIEventTarget>
  GetSpecificMessageEventTarget(const Message& aMsg) override;
=======
  virtual void OnChannelReceivedMessage(const Message& aMsg) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void OnChannelReceivedMessage(const Message& aMsg) override;
||||||| merged common ancestors
#ifdef NIGHTLY_BUILD
  virtual void
  OnChannelReceivedMessage(const Message& aMsg) override;
=======
  mozilla::ipc::IPCResult RecvAttachBrowsingContext(
      BrowsingContext::IPCInitializer&& aInit);

  mozilla::ipc::IPCResult RecvDetachBrowsingContext(BrowsingContext* aContext);

  mozilla::ipc::IPCResult RecvCacheBrowsingContextChildren(
      BrowsingContext* aContext);
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef NIGHTLY_BUILD
  virtual PContentChild::Result OnMessageReceived(const Message& aMsg) override;
||||||| merged common ancestors
  virtual PContentChild::Result
  OnMessageReceived(const Message& aMsg) override;
=======
  mozilla::ipc::IPCResult RecvRestoreBrowsingContextChildren(
      BrowsingContext* aContext, BrowsingContext::Children&& aChildren);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PContentChild::Result OnMessageReceived(const Message& aMsg,
                                                  Message*& aReply) override;
||||||| merged common ancestors
  virtual PContentChild::Result
  OnMessageReceived(const Message& aMsg, Message*& aReply) override;
=======
  mozilla::ipc::IPCResult RecvRegisterBrowsingContextGroup(
      nsTArray<BrowsingContext::IPCInitializer>&& aInits);

  mozilla::ipc::IPCResult RecvWindowClose(BrowsingContext* aContext,
                                          bool aTrustedCaller);
  mozilla::ipc::IPCResult RecvWindowFocus(BrowsingContext* aContext);
  mozilla::ipc::IPCResult RecvWindowBlur(BrowsingContext* aContext);
  mozilla::ipc::IPCResult RecvWindowPostMessage(
      BrowsingContext* aContext, const ClonedMessageData& aMessage,
      const PostMessageData& aData);

  mozilla::ipc::IPCResult RecvCommitBrowsingContextTransaction(
      BrowsingContext* aContext, BrowsingContext::Transaction&& aTransaction,
      BrowsingContext::FieldEpochs&& aEpochs);

#ifdef NIGHTLY_BUILD
  virtual PContentChild::Result OnMessageReceived(const Message& aMsg) override;
#else
  using PContentChild::OnMessageReceived;
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  InfallibleTArray<nsAutoPtr<AlertObserver>> mAlertObservers;
||||||| merged common ancestors
  InfallibleTArray<nsAutoPtr<AlertObserver> > mAlertObservers;
=======
  virtual PContentChild::Result OnMessageReceived(const Message& aMsg,
                                                  Message*& aReply) override;

  InfallibleTArray<nsAutoPtr<AlertObserver>> mAlertObservers;
>>>>>>> upstream-releases
  RefPtr<ConsoleListener> mConsoleListener;

  nsTHashtable<nsPtrHashKey<nsIObserver>> mIdleObservers;

  InfallibleTArray<nsString> mAvailableDictionaries;

  // Temporary storage for a list of available fonts, passed from the
  // parent process and used to initialize gfx in the child. Currently used
  // only on MacOSX and Linux.
  InfallibleTArray<mozilla::dom::SystemFontListEntry> mFontList;
  // Temporary storage for nsXPLookAndFeel flags.
  nsTArray<LookAndFeelInt> mLookAndFeelCache;

  /**
   * An ID unique to the process containing our corresponding
   * content parent.
   *
   * We expect our content parent to set this ID immediately after opening a
   * channel to us.
   */
  ContentParentId mID;

#if defined(XP_WIN) && defined(ACCESSIBILITY)
  /**
   * The thread ID of the main thread in the chrome process.
   */
  uint32_t mMainChromeTid;

  /**
   * This is an a11y-specific unique id for the content process that is
   * generated by the chrome process.
   */
  uint32_t mMsaaID;
#endif

  AppInfo mAppInfo;

  bool mIsForBrowser;
  nsString mRemoteType = VoidString();
  bool mIsAlive;
  nsString mProcessName;

  static ContentChild* sSingleton;

  class ShutdownCanary;
  static StaticAutoPtr<ShutdownCanary> sShutdownCanary;

  nsCOMPtr<nsIDomainPolicy> mPolicy;
  nsCOMPtr<nsITimer> mForceKillTimer;

  RefPtr<ipc::SharedMap> mSharedData;

#ifdef MOZ_GECKO_PROFILER
  RefPtr<ChildProfilerController> mProfilerController;
#endif

#if defined(XP_MACOSX) && defined(MOZ_SANDBOX)
  nsCOMPtr<nsIFile> mProfileDir;
#endif

  // Hashtable to keep track of the pending GetFilesHelper objects.
  // This GetFilesHelperChild objects are removed when RecvGetFilesResponse is
  // received.
  nsRefPtrHashtable<nsIDHashKey, GetFilesHelperChild> mGetFilesPendingRequests;

<<<<<<< HEAD
  // Hashtable to keep track of the pending file creation.
  // These items are removed when RecvFileCreationResponse is received.
  nsRefPtrHashtable<nsIDHashKey, FileCreatorHelper> mFileCreationPending;

  nsClassHashtable<nsUint64HashKey, AnonymousTemporaryFileCallback>
      mPendingAnonymousTemporaryFiles;
||||||| merged common ancestors
  // Hashtable to keep track of the pending file creation.
  // These items are removed when RecvFileCreationResponse is received.
  nsRefPtrHashtable<nsIDHashKey, FileCreatorHelper> mFileCreationPending;


  nsClassHashtable<nsUint64HashKey, AnonymousTemporaryFileCallback> mPendingAnonymousTemporaryFiles;
=======
  nsClassHashtable<nsUint64HashKey, AnonymousTemporaryFileCallback>
      mPendingAnonymousTemporaryFiles;
>>>>>>> upstream-releases

  mozilla::Atomic<bool> mShuttingDown;

#ifdef NIGHTLY_BUILD
  // NOTE: This member is atomic because it can be accessed from
  // off-main-thread.
  mozilla::Atomic<uint32_t> mPendingInputEvents;
#endif

  uint32_t mNetworkLinkType = 0;

  nsTArray<RefPtr<BrowsingContextGroup>> mBrowsingContextGroupHolder;

  DISALLOW_EVIL_CONSTRUCTORS(ContentChild);
};

uint64_t NextWindowID();

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_ContentChild_h
