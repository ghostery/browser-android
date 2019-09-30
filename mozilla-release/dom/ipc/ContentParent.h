/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_ContentParent_h
#define mozilla_dom_ContentParent_h

#include "mozilla/dom/PContentParent.h"
#include "mozilla/dom/CPOWManagerGetter.h"
#include "mozilla/dom/ipc/IdType.h"
#include "mozilla/dom/RemoteBrowser.h"
#include "mozilla/gfx/gfxVarReceiver.h"
#include "mozilla/gfx/GPUProcessListener.h"
<<<<<<< HEAD
#include "mozilla/ipc/GeckoChildProcessHost.h"
||||||| merged common ancestors
=======
#include "mozilla/ipc/BackgroundUtils.h"
#include "mozilla/ipc/GeckoChildProcessHost.h"
#include "mozilla/ipc/PParentToChildStreamParent.h"
#include "mozilla/ipc/PChildToParentStreamParent.h"
>>>>>>> upstream-releases
#include "mozilla/Attributes.h"
#include "mozilla/FileUtils.h"
#include "mozilla/HalTypes.h"
#include "mozilla/LinkedList.h"
#include "mozilla/MemoryReportingProcess.h"
#include "mozilla/StaticPtr.h"
#include "mozilla/TimeStamp.h"
#include "mozilla/Variant.h"
#include "mozilla/UniquePtr.h"

#include "nsDataHashtable.h"
#include "nsPluginTags.h"
#include "nsFrameMessageManager.h"
#include "nsHashKeys.h"
#include "nsIInterfaceRequestor.h"
#include "nsIObserver.h"
#include "nsIRemoteTab.h"
#include "nsIThreadInternal.h"
#include "nsIDOMGeoPositionCallback.h"
#include "nsIDOMGeoPositionErrorCallback.h"
#include "nsRefPtrHashtable.h"
#include "PermissionMessageUtils.h"
#include "DriverCrashGuard.h"
#include "nsIReferrerInfo.h"

#define CHILD_PROCESS_SHUTDOWN_MESSAGE \
  NS_LITERAL_STRING("child-process-shutdown")

// These must match the similar ones in E10SUtils.jsm and ProcInfo.h.
// Process names as reported by about:memory are defined in
// ContentChild:RecvRemoteType.  Add your value there too or it will be called
// "Web Content".
#define DEFAULT_REMOTE_TYPE "web"
#define FILE_REMOTE_TYPE "file"
#define EXTENSION_REMOTE_TYPE "extension"
#define PRIVILEGEDABOUT_REMOTE_TYPE "privilegedabout"
#define PRIVILEGEDMOZILLA_REMOTE_TYPE "privilegedmozilla"

// This must start with the DEFAULT_REMOTE_TYPE above.
#define LARGE_ALLOCATION_REMOTE_TYPE "webLargeAllocation"

class nsConsoleService;
class nsIContentProcessInfo;
class nsICycleCollectorLogSink;
class nsIDumpGCAndCCLogsCallback;
class nsIRemoteTab;
class nsITimer;
class ParentIdleListener;
class nsIWidget;

namespace mozilla {
class PRemoteSpellcheckEngineParent;

#if defined(XP_LINUX) && defined(MOZ_SANDBOX)
class SandboxBroker;
class SandboxBrokerPolicyFactory;
#endif

class PreallocatedProcessManagerImpl;

using mozilla::loader::PScriptCacheParent;

namespace embedding {
class PrintingParent;
}

namespace ipc {
class CrashReporterHost;
class PFileDescriptorSetParent;
class URIParams;
class TestShellParent;
#ifdef FUZZING
class ProtocolFuzzerHelper;
#endif
}  // namespace ipc

namespace jsipc {
class PJavaScriptParent;
<<<<<<< HEAD
}  // namespace jsipc
||||||| merged common ancestors
} // namespace jsipc
=======
class CpowEntry;
}  // namespace jsipc
>>>>>>> upstream-releases

namespace layers {
struct TextureFactoryIdentifier;
}  // namespace layers

namespace dom {

class BrowsingContextGroup;
class Element;
class BrowserParent;
class ClonedMessageData;
class MemoryReport;
class TabContext;
class GetFilesHelper;
class MemoryReportRequestHost;
struct CancelContentJSOptions;

#define NS_CONTENTPARENT_IID                         \
  {                                                  \
    0xeeec9ebf, 0x8ecf, 0x4e38, {                    \
      0x81, 0xda, 0xb7, 0x34, 0x13, 0x7e, 0xac, 0xf3 \
    }                                                \
  }

<<<<<<< HEAD
class ContentParent final : public PContentParent,
                            public nsIContentParent,
                            public nsIObserver,
                            public nsIDOMGeoPositionCallback,
                            public nsIDOMGeoPositionErrorCallback,
                            public nsIInterfaceRequestor,
                            public gfx::gfxVarReceiver,
                            public mozilla::LinkedListElement<ContentParent>,
                            public gfx::GPUProcessListener,
                            public mozilla::MemoryReportingProcess {
  typedef mozilla::ipc::GeckoChildProcessHost GeckoChildProcessHost;
  typedef mozilla::ipc::OptionalURIParams OptionalURIParams;
||||||| merged common ancestors
class ContentParent final : public PContentParent
                          , public nsIContentParent
                          , public nsIObserver
                          , public nsIDOMGeoPositionCallback
                          , public nsIDOMGeoPositionErrorCallback
                          , public nsIInterfaceRequestor
                          , public gfx::gfxVarReceiver
                          , public mozilla::LinkedListElement<ContentParent>
                          , public gfx::GPUProcessListener
                          , public mozilla::MemoryReportingProcess
{
  typedef mozilla::ipc::OptionalURIParams OptionalURIParams;
=======
class ContentParent final : public PContentParent,
                            public nsIObserver,
                            public nsIDOMGeoPositionCallback,
                            public nsIDOMGeoPositionErrorCallback,
                            public nsIInterfaceRequestor,
                            public gfx::gfxVarReceiver,
                            public mozilla::LinkedListElement<ContentParent>,
                            public gfx::GPUProcessListener,
                            public mozilla::MemoryReportingProcess,
                            public mozilla::dom::ipc::MessageManagerCallback,
                            public CPOWManagerGetter,
                            public mozilla::ipc::IShmemAllocator {
  typedef mozilla::ipc::GeckoChildProcessHost GeckoChildProcessHost;
>>>>>>> upstream-releases
  typedef mozilla::ipc::PFileDescriptorSetParent PFileDescriptorSetParent;
  typedef mozilla::ipc::TestShellParent TestShellParent;
  typedef mozilla::ipc::URIParams URIParams;
  typedef mozilla::ipc::PrincipalInfo PrincipalInfo;
  typedef mozilla::dom::ClonedMessageData ClonedMessageData;
  typedef mozilla::dom::BrowsingContextGroup BrowsingContextGroup;

  friend class mozilla::PreallocatedProcessManagerImpl;
  friend class PContentParent;
#ifdef FUZZING
  friend class mozilla::ipc::ProtocolFuzzerHelper;
#endif

<<<<<<< HEAD
 public:
  virtual bool IsContentParent() const override { return true; }
||||||| merged common ancestors
public:

  virtual bool IsContentParent() const override { return true; }
=======
 public:
  using LaunchError = GeckoChildProcessHost::LaunchError;
  using LaunchPromise =
      GeckoChildProcessHost::LaunchPromise<RefPtr<ContentParent>>;

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_CONTENTPARENT_IID)
>>>>>>> upstream-releases

  using LaunchError = GeckoChildProcessHost::LaunchError;
  using LaunchPromise =
      GeckoChildProcessHost::LaunchPromise<RefPtr<ContentParent>>;

  /**
   * Create a subprocess suitable for use later as a content process.
   */
  static RefPtr<LaunchPromise> PreallocateProcess();

  /**
   * Start up the content-process machinery.  This might include
   * scheduling pre-launch tasks.
   */
  static void StartUp();

  /** Shut down the content-process machinery. */
  static void ShutDown();

  static uint32_t GetPoolSize(const nsAString& aContentProcessType);

  static uint32_t GetMaxProcessCount(const nsAString& aContentProcessType);

  static bool IsMaxProcessCountReached(const nsAString& aContentProcessType);

  static void ReleaseCachedProcesses();

  /**
   * Picks a random content parent from |aContentParents| with a given |aOpener|
   * respecting the index limit set by |aMaxContentParents|.
   * Returns null if non available.
   */
  static already_AddRefed<ContentParent> MinTabSelect(
      const nsTArray<ContentParent*>& aContentParents, ContentParent* aOpener,
      int32_t maxContentParents);

  /**
   * Get or create a content process for:
   * 1. browser iframe
   * 2. remote xul <browser>
   * 3. normal iframe
   */
  static already_AddRefed<ContentParent> GetNewOrUsedBrowserProcess(
      Element* aFrameElement, const nsAString& aRemoteType,
      hal::ProcessPriority aPriority =
          hal::ProcessPriority::PROCESS_PRIORITY_FOREGROUND,
      ContentParent* aOpener = nullptr, bool aPreferUsed = false);

  /**
   * Get or create a content process for a JS plugin. aPluginID is the id of the
   * JS plugin
   * (@see nsFakePlugin::mId). There is a maximum of one process per JS plugin.
   */
  static already_AddRefed<ContentParent> GetNewOrUsedJSPluginProcess(
      uint32_t aPluginID, const hal::ProcessPriority& aPriority);

  /**
   * Get or create a content process for the given TabContext.  aFrameElement
   * should be the frame/iframe element with which this process will
   * associated.
   */
<<<<<<< HEAD
  static TabParent* CreateBrowser(const TabContext& aContext,
                                  Element* aFrameElement,
                                  ContentParent* aOpenerContentParent,
                                  TabParent* aSameTabGroupAs,
                                  uint64_t aNextTabParentId);
||||||| merged common ancestors
  static TabParent*
  CreateBrowser(const TabContext& aContext,
                Element* aFrameElement,
                ContentParent* aOpenerContentParent,
                TabParent* aSameTabGroupAs,
                uint64_t aNextTabParentId);
=======
  static already_AddRefed<RemoteBrowser> CreateBrowser(
      const TabContext& aContext, Element* aFrameElement,
      const nsAString& aRemoteType, BrowsingContext* aBrowsingContext,
      ContentParent* aOpenerContentParent, BrowserParent* aSameTabGroupAs,
      uint64_t aNextRemoteTabId);
>>>>>>> upstream-releases

  static void GetAll(nsTArray<ContentParent*>& aArray);

  static void GetAllEvenIfDead(nsTArray<ContentParent*>& aArray);

  static void BroadcastStringBundle(const StringBundleDescriptor&);

  static void BroadcastFontListChanged();

  const nsAString& GetRemoteType() const;

  virtual void DoGetRemoteType(nsAString& aRemoteType,
                               ErrorResult& aError) const override {
    aRemoteType = GetRemoteType();
  }

  enum CPIteratorPolicy { eLive, eAll };

  class ContentParentIterator {
   private:
    ContentParent* mCurrent;
    CPIteratorPolicy mPolicy;

   public:
    ContentParentIterator(CPIteratorPolicy aPolicy, ContentParent* aCurrent)
        : mCurrent(aCurrent), mPolicy(aPolicy) {}

    ContentParentIterator begin() {
      // Move the cursor to the first element that matches the policy.
      while (mPolicy != eAll && mCurrent && !mCurrent->IsAlive()) {
        mCurrent = mCurrent->LinkedListElement<ContentParent>::getNext();
      }

      return *this;
    }
    ContentParentIterator end() {
      return ContentParentIterator(mPolicy, nullptr);
    }

    const ContentParentIterator& operator++() {
      MOZ_ASSERT(mCurrent);
      do {
        mCurrent = mCurrent->LinkedListElement<ContentParent>::getNext();
      } while (mPolicy != eAll && mCurrent && !mCurrent->IsAlive());

      return *this;
    }

    bool operator!=(const ContentParentIterator& aOther) {
      MOZ_ASSERT(mPolicy == aOther.mPolicy);
      return mCurrent != aOther.mCurrent;
    }

    ContentParent* operator*() { return mCurrent; }
  };

  static ContentParentIterator AllProcesses(CPIteratorPolicy aPolicy) {
    ContentParent* first =
        sContentParents ? sContentParents->getFirst() : nullptr;
    return ContentParentIterator(aPolicy, first);
  }

  static bool IgnoreIPCPrincipal();

  static void NotifyUpdatedDictionaries();

  static void NotifyUpdatedFonts();
  static void NotifyRebuildFontList();

#if defined(XP_WIN)
  /**
   * Windows helper for firing off an update window request to a plugin
   * instance.
   *
   * aWidget - the eWindowType_plugin_ipc_chrome widget associated with
   *           this plugin window.
   */
  static void SendAsyncUpdate(nsIWidget* aWidget);
#endif

  // Let managees query if it is safe to send messages.
  bool IsDestroyed() const { return !mIPCOpen; }

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvCreateChildProcess(
      const IPCTabContext& aContext, const hal::ProcessPriority& aPriority,
      const TabId& aOpenerTabId, const TabId& aTabId, ContentParentId* aCpId,
      bool* aIsForBrowser) override;

  virtual mozilla::ipc::IPCResult RecvBridgeToChildProcess(
      const ContentParentId& aCpId,
      Endpoint<PContentBridgeParent>* aEndpoint) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvCreateChildProcess(const IPCTabContext& aContext,
                                                         const hal::ProcessPriority& aPriority,
                                                         const TabId& aOpenerTabId,
                                                         const TabId& aTabId,
                                                         ContentParentId* aCpId,
                                                         bool* aIsForBrowser) override;

  virtual mozilla::ipc::IPCResult RecvBridgeToChildProcess(const ContentParentId& aCpId,
                                                           Endpoint<PContentBridgeParent>* aEndpoint) override;
=======
  mozilla::ipc::IPCResult RecvOpenRecordReplayChannel(
      const uint32_t& channelId, FileDescriptor* connection);
  mozilla::ipc::IPCResult RecvCreateReplayingProcess(
      const uint32_t& aChannelId);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvOpenRecordReplayChannel(
      const uint32_t& channelId, FileDescriptor* connection) override;
  virtual mozilla::ipc::IPCResult RecvCreateReplayingProcess(
      const uint32_t& aChannelId) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvOpenRecordReplayChannel(const uint32_t& channelId,
                                                              FileDescriptor* connection) override;
  virtual mozilla::ipc::IPCResult RecvCreateReplayingProcess(const uint32_t& aChannelId) override;
=======
  mozilla::ipc::IPCResult RecvCreateGMPService();
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvLoadPlugin(
      const uint32_t& aPluginId, nsresult* aRv, uint32_t* aRunID,
      Endpoint<PPluginModuleParent>* aEndpoint);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvLoadPlugin(
      const uint32_t& aPluginId, nsresult* aRv, uint32_t* aRunID,
      Endpoint<PPluginModuleParent>* aEndpoint) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvLoadPlugin(const uint32_t& aPluginId, nsresult* aRv,
                                                 uint32_t* aRunID,
                                                 Endpoint<PPluginModuleParent>* aEndpoint) override;
=======
  mozilla::ipc::IPCResult RecvMaybeReloadPlugins();
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvConnectPluginBridge(
      const uint32_t& aPluginId, nsresult* aRv,
      Endpoint<PPluginModuleParent>* aEndpoint);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvConnectPluginBridge(
      const uint32_t& aPluginId, nsresult* aRv,
      Endpoint<PPluginModuleParent>* aEndpoint) override;

  virtual mozilla::ipc::IPCResult RecvLaunchRDDProcess(
      nsresult* aRv, Endpoint<PRemoteDecoderManagerChild>* aEndpoint) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvConnectPluginBridge(const uint32_t& aPluginId,
                                                          nsresult* aRv,
                                                          Endpoint<PPluginModuleParent>* aEndpoint) override;
=======
  mozilla::ipc::IPCResult RecvLaunchRDDProcess(
      nsresult* aRv, Endpoint<PRemoteDecoderManagerChild>* aEndpoint);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvUngrabPointer(
      const uint32_t& aTime) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvUngrabPointer(const uint32_t& aTime) override;
=======
  mozilla::ipc::IPCResult RecvUngrabPointer(const uint32_t& aTime);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvRemovePermission(
      const IPC::Principal& aPrincipal, const nsCString& aPermissionType,
      nsresult* aRv) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvRemovePermission(const IPC::Principal& aPrincipal,
                                                       const nsCString& aPermissionType,
                                                       nsresult* aRv) override;
=======
  mozilla::ipc::IPCResult RecvRemovePermission(const IPC::Principal& aPrincipal,
                                               const nsCString& aPermissionType,
                                               nsresult* aRv);
>>>>>>> upstream-releases

  NS_DECL_CYCLE_COLLECTION_CLASS_AMBIGUOUS(ContentParent, nsIObserver)

  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_NSIOBSERVER
  NS_DECL_NSIDOMGEOPOSITIONCALLBACK
  NS_DECL_NSIDOMGEOPOSITIONERRORCALLBACK
  NS_DECL_NSIINTERFACEREQUESTOR

  /**
   * MessageManagerCallback methods that we override.
   */
  virtual bool DoLoadMessageManagerScript(const nsAString& aURL,
                                          bool aRunInGlobalScope) override;

  virtual nsresult DoSendAsyncMessage(JSContext* aCx, const nsAString& aMessage,
                                      StructuredCloneData& aData,
                                      JS::Handle<JSObject*> aCpows,
                                      nsIPrincipal* aPrincipal) override;

  /** Notify that a tab is beginning its destruction sequence. */
  static void NotifyTabDestroying(const TabId& aTabId,
                                  const ContentParentId& aCpId);

  /** Notify that a tab was destroyed during normal operation. */
  void NotifyTabDestroyed(const TabId& aTabId, bool aNotifiedDestroying);

  TestShellParent* CreateTestShell();

  bool DestroyTestShell(TestShellParent* aTestShell);

  TestShellParent* GetTestShellSingleton();

  jsipc::CPOWManager* GetCPOWManager() override;

  static void UnregisterRemoteFrame(const TabId& aTabId,
                                    const ContentParentId& aCpId,
                                    bool aMarkedDestroying);

  // This method can be called on any thread.
  void RegisterRemoteWorkerActor();

  // This method _must_ be called on main-thread because it can start the
  // shutting down of the content process.
  void UnregisterRemoveWorkerActor();

  void ReportChildAlreadyBlocked();

  bool RequestRunToCompletion();

  void UpdateCookieStatus(nsIChannel* aChannel);

  bool IsLaunching() const {
    return mLifecycleState == LifecycleState::LAUNCHING;
  }
  bool IsAlive() const override;
  bool IsDead() const { return mLifecycleState == LifecycleState::DEAD; }

<<<<<<< HEAD
  virtual bool IsForBrowser() const override { return mIsForBrowser; }
  virtual bool IsForJSPlugin() const override {
||||||| merged common ancestors
  virtual bool IsForBrowser() const override
  {
    return mIsForBrowser;
  }
  virtual bool IsForJSPlugin() const override
  {
=======
  bool IsForBrowser() const { return mIsForBrowser; }
  bool IsForJSPlugin() const {
>>>>>>> upstream-releases
    return mJSPluginID != nsFakePluginTag::NOT_JSPLUGIN;
  }

  GeckoChildProcessHost* Process() const { return mSubprocess; }

<<<<<<< HEAD
  ContentParent* Opener() const { return mOpener; }
  nsIContentProcessInfo* ScriptableHelper() const { return mScriptableHelper; }
||||||| merged common ancestors
  ContentParent* Opener() const
  {
    return mOpener;
  }
  nsIContentProcessInfo* ScriptableHelper() const
  {
    return mScriptableHelper;
  }
=======
  ContentParent* Opener() const { return mOpener; }
  nsIContentProcessInfo* ScriptableHelper() const { return mScriptableHelper; }

  mozilla::dom::ProcessMessageManager* GetMessageManager() const {
    return mMessageManager;
  }
>>>>>>> upstream-releases

  bool NeedsPermissionsUpdate(const nsACString& aPermissionKey) const;

  /**
   * Kill our subprocess and make sure it dies.  Should only be used
   * in emergency situations since it bypasses the normal shutdown
   * process.
   *
   * WARNING: aReason appears in telemetry, so any new value passed in requires
   * data review.
   */
  void KillHard(const char* aWhy);

  ContentParentId ChildID() const { return mChildID; }

  /**
   * Get a user-friendly name for this ContentParent.  We make no guarantees
   * about this name: It might not be unique, apps can spoof special names,
   * etc.  So please don't use this name to make any decisions about the
   * ContentParent based on the value returned here.
   */
  void FriendlyName(nsAString& aName, bool aAnonymize = false);

  virtual void OnChannelError() override;

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvInitCrashReporter(
      Shmem&& aShmem, const NativeThreadId& aThreadId) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvInitCrashReporter(Shmem&& aShmem, const NativeThreadId& aThreadId) override;
=======
  mozilla::ipc::IPCResult RecvInitCrashReporter(
      Shmem&& aShmem, const NativeThreadId& aThreadId);
>>>>>>> upstream-releases

  PNeckoParent* AllocPNeckoParent();

  virtual mozilla::ipc::IPCResult RecvPNeckoConstructor(
      PNeckoParent* aActor) override {
    return PContentParent::RecvPNeckoConstructor(aActor);
  }

  PPrintingParent* AllocPPrintingParent();

  bool DeallocPPrintingParent(PPrintingParent* aActor);

#if defined(NS_PRINTING)
  /**
   * @return the PrintingParent for this ContentParent.
   */
  already_AddRefed<embedding::PrintingParent> GetPrintingParent();
#endif

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvInitStreamFilter(
      const uint64_t& aChannelId, const nsString& aAddonId,
      InitStreamFilterResolver&& aResolver) override;

  virtual PChildToParentStreamParent* AllocPChildToParentStreamParent()
      override;
  virtual bool DeallocPChildToParentStreamParent(
      PChildToParentStreamParent* aActor) override;

  virtual PParentToChildStreamParent* SendPParentToChildStreamConstructor(
      PParentToChildStreamParent*) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvInitStreamFilter(const uint64_t& aChannelId,
                       const nsString& aAddonId,
                       InitStreamFilterResolver&& aResolver) override;

  virtual PChildToParentStreamParent* AllocPChildToParentStreamParent() override;
  virtual bool
  DeallocPChildToParentStreamParent(PChildToParentStreamParent* aActor) override;

  virtual PParentToChildStreamParent*
  SendPParentToChildStreamConstructor(PParentToChildStreamParent*) override;
=======
  mozilla::ipc::IPCResult RecvInitStreamFilter(
      const uint64_t& aChannelId, const nsString& aAddonId,
      InitStreamFilterResolver&& aResolver);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PFileDescriptorSetParent* SendPFileDescriptorSetConstructor(
      const FileDescriptor&) override;
||||||| merged common ancestors
  virtual PFileDescriptorSetParent*
  SendPFileDescriptorSetConstructor(const FileDescriptor&) override;
=======
  PChildToParentStreamParent* AllocPChildToParentStreamParent();
  bool DeallocPChildToParentStreamParent(PChildToParentStreamParent* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PParentToChildStreamParent* AllocPParentToChildStreamParent()
      override;
  virtual bool DeallocPParentToChildStreamParent(
      PParentToChildStreamParent* aActor) override;
||||||| merged common ancestors
  virtual PParentToChildStreamParent* AllocPParentToChildStreamParent() override;
  virtual bool
  DeallocPParentToChildStreamParent(PParentToChildStreamParent* aActor) override;
=======
  PParentToChildStreamParent* AllocPParentToChildStreamParent();
  bool DeallocPParentToChildStreamParent(PParentToChildStreamParent* aActor);
>>>>>>> upstream-releases

  PHalParent* AllocPHalParent();

  virtual mozilla::ipc::IPCResult RecvPHalConstructor(
      PHalParent* aActor) override {
    return PContentParent::RecvPHalConstructor(aActor);
  }

  PHeapSnapshotTempFileHelperParent* AllocPHeapSnapshotTempFileHelperParent();

<<<<<<< HEAD
  virtual PJavaScriptParent* AllocPJavaScriptParent() override;
||||||| merged common ancestors
  virtual PJavaScriptParent*
  AllocPJavaScriptParent() override;
=======
  PJavaScriptParent* AllocPJavaScriptParent();
>>>>>>> upstream-releases

  virtual mozilla::ipc::IPCResult RecvPJavaScriptConstructor(
      PJavaScriptParent* aActor) override {
    return PContentParent::RecvPJavaScriptConstructor(aActor);
  }

<<<<<<< HEAD
  virtual PRemoteSpellcheckEngineParent* AllocPRemoteSpellcheckEngineParent()
      override;
||||||| merged common ancestors
  virtual PRemoteSpellcheckEngineParent* AllocPRemoteSpellcheckEngineParent() override;
=======
  PRemoteSpellcheckEngineParent* AllocPRemoteSpellcheckEngineParent();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvRecordingDeviceEvents(
      const nsString& aRecordingStatus, const nsString& aPageURL,
      const bool& aIsAudio, const bool& aIsVideo) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvRecordingDeviceEvents(const nsString& aRecordingStatus,
                                                            const nsString& aPageURL,
                                                            const bool& aIsAudio,
                                                            const bool& aIsVideo) override;
=======
  mozilla::ipc::IPCResult RecvRecordingDeviceEvents(
      const nsString& aRecordingStatus, const nsString& aPageURL,
      const bool& aIsAudio, const bool& aIsVideo);
>>>>>>> upstream-releases

  bool CycleCollectWithLogs(bool aDumpAllTraces,
                            nsICycleCollectorLogSink* aSink,
                            nsIDumpGCAndCCLogsCallback* aCallback);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvUnregisterRemoteFrame(
      const TabId& aTabId, const ContentParentId& aCpId,
      const bool& aMarkedDestroying) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvUnregisterRemoteFrame(const TabId& aTabId,
                                                            const ContentParentId& aCpId,
                                                            const bool& aMarkedDestroying) override;
=======
  mozilla::ipc::IPCResult RecvUnregisterRemoteFrame(
      const TabId& aTabId, const ContentParentId& aCpId,
      const bool& aMarkedDestroying);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyTabDestroying(
      const TabId& aTabId, const ContentParentId& aCpId) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifyTabDestroying(const TabId& aTabId,
                                                          const ContentParentId& aCpId) override;
=======
  mozilla::ipc::IPCResult RecvNotifyTabDestroying(const TabId& aTabId,
                                                  const ContentParentId& aCpId);
>>>>>>> upstream-releases

  nsTArray<TabContext> GetManagedTabContext();

<<<<<<< HEAD
  virtual POfflineCacheUpdateParent* AllocPOfflineCacheUpdateParent(
      const URIParams& aManifestURI, const URIParams& aDocumentURI,
      const PrincipalInfo& aLoadingPrincipalInfo,
      const bool& aStickDocument) override;
||||||| merged common ancestors
  virtual POfflineCacheUpdateParent*
  AllocPOfflineCacheUpdateParent(const URIParams& aManifestURI,
                                 const URIParams& aDocumentURI,
                                 const PrincipalInfo& aLoadingPrincipalInfo,
                                 const bool& aStickDocument) override;
=======
  POfflineCacheUpdateParent* AllocPOfflineCacheUpdateParent(
      const URIParams& aManifestURI, const URIParams& aDocumentURI,
      const PrincipalInfo& aLoadingPrincipalInfo, const bool& aStickDocument);
>>>>>>> upstream-releases

  virtual mozilla::ipc::IPCResult RecvPOfflineCacheUpdateConstructor(
      POfflineCacheUpdateParent* aActor, const URIParams& aManifestURI,
      const URIParams& aDocumentURI, const PrincipalInfo& aLoadingPrincipal,
      const bool& stickDocument) override;

<<<<<<< HEAD
  virtual bool DeallocPOfflineCacheUpdateParent(
      POfflineCacheUpdateParent* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPOfflineCacheUpdateParent(POfflineCacheUpdateParent* aActor) override;
=======
  bool DeallocPOfflineCacheUpdateParent(POfflineCacheUpdateParent* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvSetOfflinePermission(
      const IPC::Principal& principal) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvSetOfflinePermission(const IPC::Principal& principal) override;
=======
  mozilla::ipc::IPCResult RecvSetOfflinePermission(
      const IPC::Principal& principal);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvFinishShutdown();

  void MaybeInvokeDragSession(BrowserParent* aParent);

<<<<<<< HEAD
  virtual PContentPermissionRequestParent* AllocPContentPermissionRequestParent(
      const InfallibleTArray<PermissionRequest>& aRequests,
      const IPC::Principal& aPrincipal,
      const IPC::Principal& aTopLevelPrincipal, const bool& aIsTrusted,
      const TabId& aTabId) override;
||||||| merged common ancestors
  virtual PContentPermissionRequestParent*
  AllocPContentPermissionRequestParent(const InfallibleTArray<PermissionRequest>& aRequests,
                                       const IPC::Principal& aPrincipal,
                                       const bool& aIsTrusted,
                                       const TabId& aTabId) override;
=======
  PContentPermissionRequestParent* AllocPContentPermissionRequestParent(
      const InfallibleTArray<PermissionRequest>& aRequests,
      const IPC::Principal& aPrincipal,
      const IPC::Principal& aTopLevelPrincipal,
      const bool& aIsHandlingUserInput, const bool& aDocumentHasUserInput,
      const DOMTimeStamp& aPageLoadTimestamp, const TabId& aTabId);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPContentPermissionRequestParent(
      PContentPermissionRequestParent* actor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPContentPermissionRequestParent(PContentPermissionRequestParent* actor) override;
=======
  bool DeallocPContentPermissionRequestParent(
      PContentPermissionRequestParent* actor);
>>>>>>> upstream-releases

  virtual bool HandleWindowsMessages(const Message& aMsg) const override;

  void ForkNewProcess(bool aBlocking);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvCreateWindow(
      PBrowserParent* aThisTabParent, PBrowserParent* aNewTab,
      const uint32_t& aChromeFlags, const bool& aCalledFromJS,
      const bool& aPositionSpecified, const bool& aSizeSpecified,
      const OptionalURIParams& aURIToLoad, const nsCString& aFeatures,
      const nsCString& aBaseURI, const float& aFullZoom,
      const IPC::Principal& aTriggeringPrincipal,
      const uint32_t& aReferrerPolicy,
      CreateWindowResolver&& aResolve) override;

  virtual mozilla::ipc::IPCResult RecvCreateWindowInDifferentProcess(
      PBrowserParent* aThisTab, const uint32_t& aChromeFlags,
      const bool& aCalledFromJS, const bool& aPositionSpecified,
      const bool& aSizeSpecified, const OptionalURIParams& aURIToLoad,
      const nsCString& aFeatures, const nsCString& aBaseURI,
      const float& aFullZoom, const nsString& aName,
      const IPC::Principal& aTriggeringPrincipal,
      const uint32_t& aReferrerPolicy) override;

  static void BroadcastBlobURLRegistration(
      const nsACString& aURI, BlobImpl* aBlobImpl, nsIPrincipal* aPrincipal,
      ContentParent* aIgnoreThisCP = nullptr);

  static void BroadcastBlobURLUnregistration(
      const nsACString& aURI, ContentParent* aIgnoreThisCP = nullptr);

  virtual mozilla::ipc::IPCResult RecvStoreAndBroadcastBlobURLRegistration(
      const nsCString& aURI, const IPCBlob& aBlob,
      const Principal& aPrincipal) override;

  virtual mozilla::ipc::IPCResult RecvUnstoreAndBroadcastBlobURLUnregistration(
      const nsCString& aURI) override;

  virtual mozilla::ipc::IPCResult RecvGetA11yContentId(
      uint32_t* aContentId) override;

  virtual mozilla::ipc::IPCResult RecvA11yHandlerControl(
      const uint32_t& aPid,
      const IHandlerControlHolder& aHandlerControl) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvCreateWindow(PBrowserParent* aThisTabParent,
                   PBrowserParent* aNewTab,
                   layout::PRenderFrameParent* aRenderFrame,
                   const uint32_t& aChromeFlags,
                   const bool& aCalledFromJS,
                   const bool& aPositionSpecified,
                   const bool& aSizeSpecified,
                   const OptionalURIParams& aURIToLoad,
                   const nsCString& aFeatures,
                   const nsCString& aBaseURI,
                   const float& aFullZoom,
                   const IPC::Principal& aTriggeringPrincipal,
                   const uint32_t& aReferrerPolicy,
                   CreateWindowResolver&& aResolve) override;

  virtual mozilla::ipc::IPCResult RecvCreateWindowInDifferentProcess(
    PBrowserParent* aThisTab,
    const uint32_t& aChromeFlags,
    const bool& aCalledFromJS,
    const bool& aPositionSpecified,
    const bool& aSizeSpecified,
    const OptionalURIParams& aURIToLoad,
    const nsCString& aFeatures,
    const nsCString& aBaseURI,
    const float& aFullZoom,
    const nsString& aName,
    const IPC::Principal& aTriggeringPrincipal,
    const uint32_t& aReferrerPolicy) override;

  static bool AllocateLayerTreeId(TabParent* aTabParent, layers::LayersId* aId);

  static void
  BroadcastBlobURLRegistration(const nsACString& aURI,
                               BlobImpl* aBlobImpl,
                               nsIPrincipal* aPrincipal,
                               ContentParent* aIgnoreThisCP = nullptr);

  static void
  BroadcastBlobURLUnregistration(const nsACString& aURI,
                                 ContentParent* aIgnoreThisCP = nullptr);

  virtual mozilla::ipc::IPCResult
  RecvStoreAndBroadcastBlobURLRegistration(const nsCString& aURI,
                                           const IPCBlob& aBlob,
                                           const Principal& aPrincipal) override;

  virtual mozilla::ipc::IPCResult
  RecvUnstoreAndBroadcastBlobURLUnregistration(const nsCString& aURI) override;

  virtual mozilla::ipc::IPCResult
  RecvGetA11yContentId(uint32_t* aContentId) override;

  virtual mozilla::ipc::IPCResult
  RecvA11yHandlerControl(const uint32_t& aPid,
                         const IHandlerControlHolder& aHandlerControl) override;
=======
  mozilla::ipc::IPCResult RecvCreateWindow(
      PBrowserParent* aThisBrowserParent, PBrowserParent* aNewTab,
      const uint32_t& aChromeFlags, const bool& aCalledFromJS,
      const bool& aPositionSpecified, const bool& aSizeSpecified,
      const Maybe<URIParams>& aURIToLoad, const nsCString& aFeatures,
      const float& aFullZoom, const IPC::Principal& aTriggeringPrincipal,
      nsIContentSecurityPolicy* aCsp, nsIReferrerInfo* aReferrerInfo,
      CreateWindowResolver&& aResolve);

  mozilla::ipc::IPCResult RecvCreateWindowInDifferentProcess(
      PBrowserParent* aThisTab, const uint32_t& aChromeFlags,
      const bool& aCalledFromJS, const bool& aPositionSpecified,
      const bool& aSizeSpecified, const Maybe<URIParams>& aURIToLoad,
      const nsCString& aFeatures, const float& aFullZoom, const nsString& aName,
      nsIPrincipal* aTriggeringPrincipal, nsIContentSecurityPolicy* aCsp,
      nsIReferrerInfo* aReferrerInfo);

  static void BroadcastBlobURLRegistration(
      const nsACString& aURI, BlobImpl* aBlobImpl, nsIPrincipal* aPrincipal,
      ContentParent* aIgnoreThisCP = nullptr);

  static void BroadcastBlobURLUnregistration(
      const nsACString& aURI, ContentParent* aIgnoreThisCP = nullptr);

  mozilla::ipc::IPCResult RecvStoreAndBroadcastBlobURLRegistration(
      const nsCString& aURI, const IPCBlob& aBlob, const Principal& aPrincipal);

  mozilla::ipc::IPCResult RecvUnstoreAndBroadcastBlobURLUnregistration(
      const nsCString& aURI);

  mozilla::ipc::IPCResult RecvGetA11yContentId(uint32_t* aContentId);

  mozilla::ipc::IPCResult RecvA11yHandlerControl(
      const uint32_t& aPid, const IHandlerControlHolder& aHandlerControl);
>>>>>>> upstream-releases

  virtual int32_t Pid() const override;

  // PURLClassifierParent.
<<<<<<< HEAD
  virtual PURLClassifierParent* AllocPURLClassifierParent(
      const Principal& aPrincipal, const bool& aUseTrackingProtection,
      bool* aSuccess) override;
  virtual mozilla::ipc::IPCResult RecvPURLClassifierConstructor(
      PURLClassifierParent* aActor, const Principal& aPrincipal,
      const bool& aUseTrackingProtection, bool* aSuccess) override;
||||||| merged common ancestors
  virtual PURLClassifierParent*
  AllocPURLClassifierParent(const Principal& aPrincipal,
                            const bool& aUseTrackingProtection,
                            bool* aSuccess) override;
  virtual mozilla::ipc::IPCResult
  RecvPURLClassifierConstructor(PURLClassifierParent* aActor,
                                const Principal& aPrincipal,
                                const bool& aUseTrackingProtection,
                                bool* aSuccess) override;
=======
  PURLClassifierParent* AllocPURLClassifierParent(const Principal& aPrincipal,
                                                  bool* aSuccess);
  virtual mozilla::ipc::IPCResult RecvPURLClassifierConstructor(
      PURLClassifierParent* aActor, const Principal& aPrincipal,
      bool* aSuccess) override;
>>>>>>> upstream-releases

  // PURLClassifierLocalParent.
<<<<<<< HEAD
  virtual PURLClassifierLocalParent* AllocPURLClassifierLocalParent(
      const URIParams& aURI, const nsCString& aTables) override;
  virtual mozilla::ipc::IPCResult RecvPURLClassifierLocalConstructor(
      PURLClassifierLocalParent* aActor, const URIParams& aURI,
      const nsCString& aTables) override;

  virtual PLoginReputationParent* AllocPLoginReputationParent(
      const URIParams& aURI) override;

  virtual mozilla::ipc::IPCResult RecvPLoginReputationConstructor(
      PLoginReputationParent* aActor, const URIParams& aURI) override;

  virtual bool DeallocPLoginReputationParent(
      PLoginReputationParent* aActor) override;

  virtual bool SendActivate(PBrowserParent* aTab) override {
    return PContentParent::SendActivate(aTab);
  }
||||||| merged common ancestors
  virtual PURLClassifierLocalParent*
  AllocPURLClassifierLocalParent(const URIParams& aURI,
                                 const nsCString& aTables) override;
  virtual mozilla::ipc::IPCResult
  RecvPURLClassifierLocalConstructor(PURLClassifierLocalParent* aActor,
                                     const URIParams& aURI,
                                     const nsCString& aTables) override;

  virtual PLoginReputationParent*
  AllocPLoginReputationParent(const URIParams& aURI) override;

  virtual mozilla::ipc::IPCResult
  RecvPLoginReputationConstructor(PLoginReputationParent* aActor,
                                  const URIParams& aURI) override;

  virtual bool
  DeallocPLoginReputationParent(PLoginReputationParent* aActor) override;

  virtual bool SendActivate(PBrowserParent* aTab) override
  {
    return PContentParent::SendActivate(aTab);
  }
=======
  PURLClassifierLocalParent* AllocPURLClassifierLocalParent(
      const URIParams& aURI,
      const nsTArray<IPCURLClassifierFeature>& aFeatures);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool SendDeactivate(PBrowserParent* aTab) override {
    return PContentParent::SendDeactivate(aTab);
  }
||||||| merged common ancestors
  virtual bool SendDeactivate(PBrowserParent* aTab) override
  {
    return PContentParent::SendDeactivate(aTab);
  }
=======
  virtual mozilla::ipc::IPCResult RecvPURLClassifierLocalConstructor(
      PURLClassifierLocalParent* aActor, const URIParams& aURI,
      nsTArray<IPCURLClassifierFeature>&& aFeatures) override;

  PLoginReputationParent* AllocPLoginReputationParent(const URIParams& aURI);

  virtual mozilla::ipc::IPCResult RecvPLoginReputationConstructor(
      PLoginReputationParent* aActor, const URIParams& aURI) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPURLClassifierLocalParent(
      PURLClassifierLocalParent* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPURLClassifierLocalParent(PURLClassifierLocalParent* aActor) override;
=======
  bool DeallocPLoginReputationParent(PLoginReputationParent* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPURLClassifierParent(
      PURLClassifierParent* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPURLClassifierParent(PURLClassifierParent* aActor) override;
=======
  PSessionStorageObserverParent* AllocPSessionStorageObserverParent();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvClassifyLocal(
      const URIParams& aURI, const nsCString& aTables, nsresult* aRv,
      nsTArray<nsCString>* aResults) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvClassifyLocal(const URIParams& aURI,
                    const nsCString& aTables,
                    nsresult* aRv,
                    nsTArray<nsCString>* aResults) override;
=======
  virtual mozilla::ipc::IPCResult RecvPSessionStorageObserverConstructor(
      PSessionStorageObserverParent* aActor) override;

  bool DeallocPSessionStorageObserverParent(
      PSessionStorageObserverParent* aActor);

  bool DeallocPURLClassifierLocalParent(PURLClassifierLocalParent* aActor);

  bool DeallocPURLClassifierParent(PURLClassifierParent* aActor);
>>>>>>> upstream-releases

  // Use the PHangMonitor channel to ask the child to repaint a tab.
<<<<<<< HEAD
  void PaintTabWhileInterruptingJS(TabParent* aTabParent, bool aForceRepaint,
||||||| merged common ancestors
  void PaintTabWhileInterruptingJS(TabParent* aTabParent,
                                   bool aForceRepaint,
=======
  void PaintTabWhileInterruptingJS(BrowserParent* aBrowserParent,
                                   bool aForceRepaint,
>>>>>>> upstream-releases
                                   const layers::LayersObserverEpoch& aEpoch);

  void CancelContentJSExecutionIfRunning(
      BrowserParent* aBrowserParent,
      nsIRemoteTab::NavigationType aNavigationType,
      const CancelContentJSOptions& aCancelContentJSOptions);

  // This function is called when we are about to load a document from an
  // HTTP(S) or FTP channel for a content process.  It is a useful place
  // to start to kick off work as early as possible in response to such
  // document loads.
  nsresult AboutToLoadHttpFtpDocumentForChild(nsIChannel* aChannel);

  nsresult TransmitPermissionsForPrincipal(nsIPrincipal* aPrincipal);

  void OnCompositorDeviceReset() override;

<<<<<<< HEAD
  virtual PClientOpenWindowOpParent* AllocPClientOpenWindowOpParent(
      const ClientOpenWindowArgs& aArgs) override;
||||||| merged common ancestors
  virtual PClientOpenWindowOpParent*
  AllocPClientOpenWindowOpParent(const ClientOpenWindowArgs& aArgs) override;
=======
  PClientOpenWindowOpParent* AllocPClientOpenWindowOpParent(
      const ClientOpenWindowArgs& aArgs);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPClientOpenWindowOpParent(
      PClientOpenWindowOpParent* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPClientOpenWindowOpParent(PClientOpenWindowOpParent* aActor) override;
=======
  bool DeallocPClientOpenWindowOpParent(PClientOpenWindowOpParent* aActor);
>>>>>>> upstream-releases

  static hal::ProcessPriority GetInitialProcessPriority(Element* aFrameElement);

  // Control the priority of the IPC messages for input events.
  void SetInputPriorityEventEnabled(bool aEnabled);
  bool IsInputPriorityEventEnabled() { return mIsInputPriorityEventEnabled; }

  static bool IsInputEventQueueSupported();

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvAttachBrowsingContext(
      const BrowsingContextId& aParentContextId,
      const BrowsingContextId& aOpenerId, const BrowsingContextId& aContextId,
      const nsString& aName) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvAttachBrowsingContext(
    const BrowsingContextId& aParentContextId,
    const BrowsingContextId& aContextId,
    const nsString& aName) override;
=======
  mozilla::ipc::IPCResult RecvAttachBrowsingContext(
      BrowsingContext::IPCInitializer&& aInit);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvDetachBrowsingContext(
      const BrowsingContextId& aContextId, const bool& aMoveToBFCache) override;

  virtual mozilla::ipc::IPCResult RecvSetOpenerBrowsingContext(
      const BrowsingContextId& aContextId,
      const BrowsingContextId& aOpenerContextId) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvDetachBrowsingContext(
    const BrowsingContextId& aContextId,
    const bool& aMoveToBFCache) override;
=======
  mozilla::ipc::IPCResult RecvDetachBrowsingContext(BrowsingContext* aContext);
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
||||||| merged common ancestors
protected:
=======
  mozilla::ipc::IPCResult RecvCacheBrowsingContextChildren(
      BrowsingContext* aContext);

  mozilla::ipc::IPCResult RecvRestoreBrowsingContextChildren(
      BrowsingContext* aContext, BrowsingContext::Children&& aChildren);

  mozilla::ipc::IPCResult RecvWindowClose(BrowsingContext* aContext,
                                          bool aTrustedCaller);
  mozilla::ipc::IPCResult RecvWindowFocus(BrowsingContext* aContext);
  mozilla::ipc::IPCResult RecvWindowBlur(BrowsingContext* aContext);
  mozilla::ipc::IPCResult RecvWindowPostMessage(
      BrowsingContext* aContext, const ClonedMessageData& aMessage,
      const PostMessageData& aData);

  FORWARD_SHMEM_ALLOCATOR_TO(PContentParent)

 protected:
>>>>>>> upstream-releases
  void OnChannelConnected(int32_t pid) override;

  void ActorDestroy(ActorDestroyReason why) override;
  void ActorDealloc() override;

  bool ShouldContinueFromReplyTimeout() override;

  void OnVarChanged(const GfxVarUpdate& aVar) override;
  void OnCompositorUnexpectedShutdown() override;

 private:
  /**
   * A map of the remote content process type to a list of content parents
   * currently available to host *new* tabs/frames of that type.
   *
   * If a content process is identified as troubled or dead, it will be
   * removed from this list, but will still be in the sContentParents list for
   * the GetAll/GetAllEvenIfDead APIs.
   */
  static nsClassHashtable<nsStringHashKey, nsTArray<ContentParent*>>*
      sBrowserContentParents;
  static nsTArray<ContentParent*>* sPrivateContent;
<<<<<<< HEAD
  static nsDataHashtable<nsUint32HashKey, ContentParent*>*
      sJSPluginContentParents;
  static StaticAutoPtr<LinkedList<ContentParent>> sContentParents;

  static ContentBridgeParent* CreateContentBridgeParent(
      const TabContext& aContext, const hal::ProcessPriority& aPriority,
      const TabId& aOpenerTabId, const TabId& aTabId);

#if defined(XP_MACOSX) && defined(MOZ_CONTENT_SANDBOX)
  // Cached Mac sandbox params used when launching content processes.
  static StaticAutoPtr<std::vector<std::string>> sMacSandboxParams;
#endif

  // Hide the raw constructor methods since we don't want client code
  // using them.
  virtual PBrowserParent* SendPBrowserConstructor(
      PBrowserParent* actor, const TabId& aTabId, const TabId& aSameTabGroupsAs,
      const IPCTabContext& context, const uint32_t& chromeFlags,
      const ContentParentId& aCpId, const bool& aIsForBrowser) override;
  using PContentParent::SendPTestShellConstructor;
||||||| merged common ancestors
  static nsDataHashtable<nsUint32HashKey, ContentParent*> *sJSPluginContentParents;
  static StaticAutoPtr<LinkedList<ContentParent> > sContentParents;

  static ContentBridgeParent* CreateContentBridgeParent(const TabContext& aContext,
                                                        const hal::ProcessPriority& aPriority,
                                                        const TabId& aOpenerTabId,
                                                        const TabId& aTabId);

  // Hide the raw constructor methods since we don't want client code
  // using them.
  virtual PBrowserParent* SendPBrowserConstructor(
      PBrowserParent* actor,
      const TabId& aTabId,
      const TabId& aSameTabGroupsAs,
      const IPCTabContext& context,
      const uint32_t& chromeFlags,
      const ContentParentId& aCpId,
      const bool& aIsForBrowser) override;
  using PContentParent::SendPTestShellConstructor;
=======
  static nsDataHashtable<nsUint32HashKey, ContentParent*>*
      sJSPluginContentParents;
  static StaticAutoPtr<LinkedList<ContentParent>> sContentParents;

#if defined(XP_MACOSX) && defined(MOZ_SANDBOX)
  // Cached Mac sandbox params used when launching content processes.
  static StaticAutoPtr<std::vector<std::string>> sMacSandboxParams;
#endif
>>>>>>> upstream-releases

  // Set aLoadUri to true to load aURIToLoad and to false to only create the
  // window. aURIToLoad should always be provided, if available, to ensure
  // compatibility with GeckoView.
<<<<<<< HEAD
  mozilla::ipc::IPCResult CommonCreateWindow(
      PBrowserParent* aThisTab, bool aSetOpener, const uint32_t& aChromeFlags,
      const bool& aCalledFromJS, const bool& aPositionSpecified,
      const bool& aSizeSpecified, nsIURI* aURIToLoad,
      const nsCString& aFeatures, const nsCString& aBaseURI,
      const float& aFullZoom, uint64_t aNextTabParentId, const nsString& aName,
      nsresult& aResult, nsCOMPtr<nsITabParent>& aNewTabParent,
      bool* aWindowIsNew, int32_t& aOpenLocation,
      nsIPrincipal* aTriggeringPrincipal, uint32_t aReferrerPolicy,
      bool aLoadUri);

  FORWARD_SHMEM_ALLOCATOR_TO(PContentParent)

  enum RecordReplayState { eNotRecordingOrReplaying, eRecording, eReplaying };
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  CommonCreateWindow(PBrowserParent* aThisTab,
                     bool aSetOpener,
                     const uint32_t& aChromeFlags,
                     const bool& aCalledFromJS,
                     const bool& aPositionSpecified,
                     const bool& aSizeSpecified,
                     nsIURI* aURIToLoad,
                     const nsCString& aFeatures,
                     const nsCString& aBaseURI,
                     const float& aFullZoom,
                     uint64_t aNextTabParentId,
                     const nsString& aName,
                     nsresult& aResult,
                     nsCOMPtr<nsITabParent>& aNewTabParent,
                     bool* aWindowIsNew,
                     int32_t& aOpenLocation,
                     nsIPrincipal* aTriggeringPrincipal,
                     uint32_t aReferrerPolicy,
                     bool aLoadUri);

  FORWARD_SHMEM_ALLOCATOR_TO(PContentParent)

  enum RecordReplayState
  {
    eNotRecordingOrReplaying,
    eRecording,
    eReplaying
  };
=======
  mozilla::ipc::IPCResult CommonCreateWindow(
      PBrowserParent* aThisTab, bool aSetOpener, const uint32_t& aChromeFlags,
      const bool& aCalledFromJS, const bool& aPositionSpecified,
      const bool& aSizeSpecified, nsIURI* aURIToLoad,
      const nsCString& aFeatures, const float& aFullZoom,
      uint64_t aNextRemoteTabId, const nsString& aName, nsresult& aResult,
      nsCOMPtr<nsIRemoteTab>& aNewRemoteTab, bool* aWindowIsNew,
      int32_t& aOpenLocation, nsIPrincipal* aTriggeringPrincipal,
      nsIReferrerInfo* aReferrerInfo, bool aLoadUri,
      nsIContentSecurityPolicy* aCsp);

  enum RecordReplayState { eNotRecordingOrReplaying, eRecording, eReplaying };
>>>>>>> upstream-releases

  explicit ContentParent(int32_t aPluginID)
      : ContentParent(nullptr, EmptyString(), eNotRecordingOrReplaying,
                      EmptyString(), aPluginID) {}
  ContentParent(ContentParent* aOpener, const nsAString& aRemoteType,
                RecordReplayState aRecordReplayState = eNotRecordingOrReplaying,
                const nsAString& aRecordingFile = EmptyString())
      : ContentParent(aOpener, aRemoteType, aRecordReplayState, aRecordingFile,
                      nsFakePluginTag::NOT_JSPLUGIN) {}

  ContentParent(ContentParent* aOpener, const nsAString& aRemoteType,
                RecordReplayState aRecordReplayState,
                const nsAString& aRecordingFile, int32_t aPluginID);

  // Launch the subprocess and associated initialization.
  // Returns false if the process fails to start.
  // Deprecated in favor of LaunchSubprocessAsync.
  bool LaunchSubprocessSync(hal::ProcessPriority aInitialPriority);

  // Launch the subprocess and associated initialization;
  // returns a promise and signals failure by rejecting.
  // OS-level launching work is dispatched to another thread, but some
  // initialization (creating IPDL actors, etc.; see Init()) is run on
  // the main thread.
  RefPtr<LaunchPromise> LaunchSubprocessAsync(
      hal::ProcessPriority aInitialPriority);

  // Common implementation of LaunchSubprocess{Sync,Async}.
  void LaunchSubprocessInternal(
      hal::ProcessPriority aInitialPriority,
      mozilla::Variant<bool*, RefPtr<LaunchPromise>*>&& aRetval);

  // Common initialization after sub process launch.
  void InitInternal(ProcessPriority aPriority);

  // Generate a minidump for the child process and one for the main process
  void GeneratePairedMinidump(const char* aReason);

  virtual ~ContentParent();

  void Init();

  // Some information could be sent to content very early, it
  // should be send from this function. This function should only be
  // called after the process has been transformed to browser.
  void ForwardKnownInfo();

  /**
   * We might want to reuse barely used content processes if certain criteria
   * are met.
   */
  bool TryToRecycle();

  /**
   * Removing it from the static array so it won't be returned for new tabs in
   * GetNewOrUsedBrowserProcess.
   */
  void RemoveFromList();

  /**
   * Decide whether the process should be kept alive even when it would normally
   * be shut down, for example when all its tabs are closed.
   */
  bool ShouldKeepProcessAlive() const;

  /**
   * Mark this ContentParent as "troubled". This means that it is still alive,
   * but it won't be returned for new tabs in GetNewOrUsedBrowserProcess.
   */
  void MarkAsTroubled();

  /**
   * Mark this ContentParent as dead for the purposes of Get*().
   * This method is idempotent.
   */
  void MarkAsDead();

  /**
   * How we will shut down this ContentParent and its subprocess.
   */
  enum ShutDownMethod {
    // Send a shutdown message and wait for FinishShutdown call back.
    SEND_SHUTDOWN_MESSAGE,
    // Close the channel ourselves and let the subprocess clean up itself.
    CLOSE_CHANNEL,
    // Close the channel with error and let the subprocess clean up itself.
    CLOSE_CHANNEL_WITH_ERROR,
  };

  /**
   * Exit the subprocess and vamoose.  After this call IsAlive()
   * will return false and this ContentParent will not be returned
   * by the Get*() funtions.  However, the shutdown sequence itself
   * may be asynchronous.
   *
   * If aMethod is CLOSE_CHANNEL_WITH_ERROR and this is the first call
   * to ShutDownProcess, then we'll close our channel using CloseWithError()
   * rather than vanilla Close().  CloseWithError() indicates to IPC that this
   * is an abnormal shutdown (e.g. a crash).
   */
  void ShutDownProcess(ShutDownMethod aMethod);

  // Perform any steps necesssary to gracefully shtudown the message
  // manager and null out mMessageManager.
  void ShutDownMessageManager();

  // Start the force-kill timer on shutdown.
  void StartForceKillTimer();

  // Ensure that the permissions for the giben Permission key are set in the
  // content process.
  //
  // See nsIPermissionManager::GetPermissionsForKey for more information on
  // these keys.
  void EnsurePermissionsByKey(const nsCString& aKey);

  static void ForceKillTimerCallback(nsITimer* aTimer, void* aClosure);

<<<<<<< HEAD
||||||| merged common ancestors
  static bool AllocateLayerTreeId(ContentParent* aContent,
                                  TabParent* aTopLevel, const TabId& aTabId,
                                  layers::LayersId* aId);

=======
  bool CanOpenBrowser(const IPCTabContext& aContext);

>>>>>>> upstream-releases
  /**
   * Get or create the corresponding content parent array to
   * |aContentProcessType|.
   */
<<<<<<< HEAD
  static nsTArray<ContentParent*>& GetOrCreatePool(
      const nsAString& aContentProcessType);

  virtual mozilla::ipc::IPCResult RecvInitBackground(
      Endpoint<mozilla::ipc::PBackgroundParent>&& aEndpoint) override;
||||||| merged common ancestors
  static nsTArray<ContentParent*>& GetOrCreatePool(const nsAString& aContentProcessType);

  virtual mozilla::ipc::IPCResult RecvInitBackground(Endpoint<mozilla::ipc::PBackgroundParent>&& aEndpoint) override;
=======
  static nsTArray<ContentParent*>& GetOrCreatePool(
      const nsAString& aContentProcessType);
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvAddMemoryReport(
      const MemoryReport& aReport) override;
  mozilla::ipc::IPCResult RecvFinishMemoryReport(
      const uint32_t& aGeneration) override;
  mozilla::ipc::IPCResult RecvAddPerformanceMetrics(
      const nsID& aID, nsTArray<PerformanceInfo>&& aMetrics) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult RecvAddMemoryReport(const MemoryReport& aReport) override;
  mozilla::ipc::IPCResult RecvFinishMemoryReport(const uint32_t& aGeneration) override;
  mozilla::ipc::IPCResult RecvAddPerformanceMetrics(const nsID& aID, nsTArray<PerformanceInfo>&& aMetrics) override;
=======
  mozilla::ipc::IPCResult RecvInitBackground(
      Endpoint<mozilla::ipc::PBackgroundParent>&& aEndpoint);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPJavaScriptParent(
      mozilla::jsipc::PJavaScriptParent*) override;
||||||| merged common ancestors
  virtual bool
  DeallocPJavaScriptParent(mozilla::jsipc::PJavaScriptParent*) override;
=======
  mozilla::ipc::IPCResult RecvAddMemoryReport(const MemoryReport& aReport);
  mozilla::ipc::IPCResult RecvFinishMemoryReport(const uint32_t& aGeneration);
  mozilla::ipc::IPCResult RecvAddPerformanceMetrics(
      const nsID& aID, nsTArray<PerformanceInfo>&& aMetrics);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPRemoteSpellcheckEngineParent(
      PRemoteSpellcheckEngineParent*) override;
||||||| merged common ancestors
  virtual bool
  DeallocPRemoteSpellcheckEngineParent(PRemoteSpellcheckEngineParent*) override;
=======
  bool DeallocPJavaScriptParent(mozilla::jsipc::PJavaScriptParent*);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PBrowserParent* AllocPBrowserParent(
      const TabId& aTabId, const TabId& aSameTabGroupAs,
      const IPCTabContext& aContext, const uint32_t& aChromeFlags,
      const ContentParentId& aCpId, const bool& aIsForBrowser) override;
||||||| merged common ancestors
  virtual PBrowserParent* AllocPBrowserParent(const TabId& aTabId,
                                              const TabId& aSameTabGroupAs,
                                              const IPCTabContext& aContext,
                                              const uint32_t& aChromeFlags,
                                              const ContentParentId& aCpId,
                                              const bool& aIsForBrowser) override;
=======
  bool DeallocPRemoteSpellcheckEngineParent(PRemoteSpellcheckEngineParent*);
>>>>>>> upstream-releases

  PBrowserParent* AllocPBrowserParent(const TabId& aTabId,
                                      const TabId& aSameTabGroupAs,
                                      const IPCTabContext& aContext,
                                      const uint32_t& aChromeFlags,
                                      const ContentParentId& aCpId,
                                      BrowsingContext* aBrowsingContext,
                                      const bool& aIsForBrowser);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPBrowserConstructor(
      PBrowserParent* actor, const TabId& tabId, const TabId& sameTabGroupAs,
      const IPCTabContext& context, const uint32_t& chromeFlags,
      const ContentParentId& cpId, const bool& isForBrowser) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvPBrowserConstructor(PBrowserParent* actor,
                          const TabId& tabId,
                          const TabId& sameTabGroupAs,
                          const IPCTabContext& context,
                          const uint32_t& chromeFlags,
                          const ContentParentId& cpId,
                          const bool& isForBrowser) override;
=======
  bool DeallocPBrowserParent(PBrowserParent* frame);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PIPCBlobInputStreamParent* SendPIPCBlobInputStreamConstructor(
      PIPCBlobInputStreamParent* aActor, const nsID& aID,
      const uint64_t& aSize) override;
||||||| merged common ancestors
  virtual PIPCBlobInputStreamParent*
  SendPIPCBlobInputStreamConstructor(PIPCBlobInputStreamParent* aActor,
                                     const nsID& aID,
                                     const uint64_t& aSize) override;
=======
  mozilla::ipc::IPCResult RecvConstructPopupBrowser(
      ManagedEndpoint<PBrowserParent>&& actor, const TabId& tabId,
      const IPCTabContext& context, BrowsingContext* aBrowsingContext,
      const uint32_t& chromeFlags);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PIPCBlobInputStreamParent* AllocPIPCBlobInputStreamParent(
      const nsID& aID, const uint64_t& aSize) override;
||||||| merged common ancestors
  virtual PIPCBlobInputStreamParent*
  AllocPIPCBlobInputStreamParent(const nsID& aID,
                                 const uint64_t& aSize) override;
=======
  PIPCBlobInputStreamParent* AllocPIPCBlobInputStreamParent(
      const nsID& aID, const uint64_t& aSize);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPIPCBlobInputStreamParent(
      PIPCBlobInputStreamParent* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPIPCBlobInputStreamParent(PIPCBlobInputStreamParent* aActor) override;
=======
  bool DeallocPIPCBlobInputStreamParent(PIPCBlobInputStreamParent* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvIsSecureURI(
      const uint32_t& aType, const URIParams& aURI, const uint32_t& aFlags,
      const OriginAttributes& aOriginAttributes, bool* aIsSecureURI) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvIsSecureURI(const uint32_t& aType, const URIParams& aURI,
                                                  const uint32_t& aFlags,
                                                  const OriginAttributes& aOriginAttributes,
                                                  bool* aIsSecureURI) override;
=======
  mozilla::ipc::IPCResult RecvIsSecureURI(
      const uint32_t& aType, const URIParams& aURI, const uint32_t& aFlags,
      const OriginAttributes& aOriginAttributes, bool* aIsSecureURI);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvAccumulateMixedContentHSTS(
      const URIParams& aURI, const bool& aActive,
      const OriginAttributes& aOriginAttributes) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvAccumulateMixedContentHSTS(const URIParams& aURI,
                                                                 const bool& aActive,
                                                                 const OriginAttributes& aOriginAttributes) override;
=======
  mozilla::ipc::IPCResult RecvAccumulateMixedContentHSTS(
      const URIParams& aURI, const bool& aActive,
      const OriginAttributes& aOriginAttributes);
>>>>>>> upstream-releases

  bool DeallocPHalParent(PHalParent*);

<<<<<<< HEAD
  virtual bool DeallocPHeapSnapshotTempFileHelperParent(
      PHeapSnapshotTempFileHelperParent*) override;
||||||| merged common ancestors
  virtual bool
  DeallocPHeapSnapshotTempFileHelperParent(PHeapSnapshotTempFileHelperParent*) override;
=======
  bool DeallocPHeapSnapshotTempFileHelperParent(
      PHeapSnapshotTempFileHelperParent*);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PCycleCollectWithLogsParent* AllocPCycleCollectWithLogsParent(
      const bool& aDumpAllTraces, const FileDescriptor& aGCLog,
      const FileDescriptor& aCCLog) override;
||||||| merged common ancestors
  virtual PCycleCollectWithLogsParent*
  AllocPCycleCollectWithLogsParent(const bool& aDumpAllTraces,
                                   const FileDescriptor& aGCLog,
                                   const FileDescriptor& aCCLog) override;
=======
  PCycleCollectWithLogsParent* AllocPCycleCollectWithLogsParent(
      const bool& aDumpAllTraces, const FileDescriptor& aGCLog,
      const FileDescriptor& aCCLog);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPCycleCollectWithLogsParent(
      PCycleCollectWithLogsParent* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPCycleCollectWithLogsParent(PCycleCollectWithLogsParent* aActor) override;
=======
  bool DeallocPCycleCollectWithLogsParent(PCycleCollectWithLogsParent* aActor);
>>>>>>> upstream-releases

  PTestShellParent* AllocPTestShellParent();

  bool DeallocPTestShellParent(PTestShellParent* shell);

<<<<<<< HEAD
  virtual PScriptCacheParent* AllocPScriptCacheParent(
      const FileDescOrError& cacheFile, const bool& wantCacheData) override;
||||||| merged common ancestors
  virtual PScriptCacheParent*
  AllocPScriptCacheParent(const FileDescOrError& cacheFile,
                          const bool& wantCacheData) override;
=======
  PScriptCacheParent* AllocPScriptCacheParent(const FileDescOrError& cacheFile,
                                              const bool& wantCacheData);
>>>>>>> upstream-releases

  bool DeallocPScriptCacheParent(PScriptCacheParent* shell);

  bool DeallocPNeckoParent(PNeckoParent* necko);

<<<<<<< HEAD
  virtual PPSMContentDownloaderParent* AllocPPSMContentDownloaderParent(
      const uint32_t& aCertType) override;
||||||| merged common ancestors
  virtual PPSMContentDownloaderParent*
  AllocPPSMContentDownloaderParent(const uint32_t& aCertType) override;
=======
  PPSMContentDownloaderParent* AllocPPSMContentDownloaderParent(
      const uint32_t& aCertType);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPPSMContentDownloaderParent(
      PPSMContentDownloaderParent* aDownloader) override;
||||||| merged common ancestors
  virtual bool
  DeallocPPSMContentDownloaderParent(PPSMContentDownloaderParent* aDownloader) override;
=======
  bool DeallocPPSMContentDownloaderParent(
      PPSMContentDownloaderParent* aDownloader);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PExternalHelperAppParent* AllocPExternalHelperAppParent(
      const OptionalURIParams& aUri, const nsCString& aMimeContentType,
      const nsCString& aContentDisposition,
      const uint32_t& aContentDispositionHint,
      const nsString& aContentDispositionFilename, const bool& aForceSave,
      const int64_t& aContentLength, const bool& aWasFileChannel,
      const OptionalURIParams& aReferrer, PBrowserParent* aBrowser) override;
||||||| merged common ancestors
  virtual PExternalHelperAppParent*
  AllocPExternalHelperAppParent(const OptionalURIParams& aUri,
                                const nsCString& aMimeContentType,
                                const nsCString& aContentDisposition,
                                const uint32_t& aContentDispositionHint,
                                const nsString& aContentDispositionFilename,
                                const bool& aForceSave,
                                const int64_t& aContentLength,
                                const bool& aWasFileChannel,
                                const OptionalURIParams& aReferrer,
                                PBrowserParent* aBrowser) override;
=======
  PExternalHelperAppParent* AllocPExternalHelperAppParent(
      const Maybe<URIParams>& aUri,
      const Maybe<mozilla::net::LoadInfoArgs>& aLoadInfoArgs,
      const nsCString& aMimeContentType, const nsCString& aContentDisposition,
      const uint32_t& aContentDispositionHint,
      const nsString& aContentDispositionFilename, const bool& aForceSave,
      const int64_t& aContentLength, const bool& aWasFileChannel,
      const Maybe<URIParams>& aReferrer, PBrowserParent* aBrowser);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPExternalHelperAppParent(
      PExternalHelperAppParent* aService) override;
||||||| merged common ancestors
  virtual bool
  DeallocPExternalHelperAppParent(PExternalHelperAppParent* aService) override;
=======
  bool DeallocPExternalHelperAppParent(PExternalHelperAppParent* aService);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvPExternalHelperAppConstructor(
      PExternalHelperAppParent* actor, const Maybe<URIParams>& uri,
      const Maybe<LoadInfoArgs>& loadInfoArgs,
      const nsCString& aMimeContentType, const nsCString& aContentDisposition,
      const uint32_t& aContentDispositionHint,
      const nsString& aContentDispositionFilename, const bool& aForceSave,
      const int64_t& aContentLength, const bool& aWasFileChannel,
      const Maybe<URIParams>& aReferrer, PBrowserParent* aBrowser) override;

  PHandlerServiceParent* AllocPHandlerServiceParent();

  bool DeallocPHandlerServiceParent(PHandlerServiceParent*);

  PMediaParent* AllocPMediaParent();

  bool DeallocPMediaParent(PMediaParent* aActor);

  PPresentationParent* AllocPPresentationParent();

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPPresentationConstructor(
      PPresentationParent* aActor) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvPPresentationConstructor(PPresentationParent* aActor) override;
=======
  bool DeallocPPresentationParent(PPresentationParent* aActor);
>>>>>>> upstream-releases

  virtual mozilla::ipc::IPCResult RecvPPresentationConstructor(
      PPresentationParent* aActor) override;

<<<<<<< HEAD
  virtual bool DeallocPSpeechSynthesisParent(
      PSpeechSynthesisParent* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPSpeechSynthesisParent(PSpeechSynthesisParent* aActor) override;
=======
  PSpeechSynthesisParent* AllocPSpeechSynthesisParent();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPSpeechSynthesisConstructor(
      PSpeechSynthesisParent* aActor) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvPSpeechSynthesisConstructor(PSpeechSynthesisParent* aActor) override;
=======
  bool DeallocPSpeechSynthesisParent(PSpeechSynthesisParent* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PWebBrowserPersistDocumentParent*
  AllocPWebBrowserPersistDocumentParent(
      PBrowserParent* aBrowser, const uint64_t& aOuterWindowID) override;
||||||| merged common ancestors
  virtual PWebBrowserPersistDocumentParent*
  AllocPWebBrowserPersistDocumentParent(PBrowserParent* aBrowser,
                                        const uint64_t& aOuterWindowID) override;
=======
  virtual mozilla::ipc::IPCResult RecvPSpeechSynthesisConstructor(
      PSpeechSynthesisParent* aActor) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPWebBrowserPersistDocumentParent(
      PWebBrowserPersistDocumentParent* aActor) override;
||||||| merged common ancestors
  virtual bool
  DeallocPWebBrowserPersistDocumentParent(PWebBrowserPersistDocumentParent* aActor) override;
=======
  PWebBrowserPersistDocumentParent* AllocPWebBrowserPersistDocumentParent(
      PBrowserParent* aBrowser, const uint64_t& aOuterWindowID);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvGetGfxVars(
      InfallibleTArray<GfxVarUpdate>* aVars) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvGetGfxVars(InfallibleTArray<GfxVarUpdate>* aVars) override;
=======
  bool DeallocPWebBrowserPersistDocumentParent(
      PWebBrowserPersistDocumentParent* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvReadFontList(
      InfallibleTArray<FontListEntry>* retValue) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvReadFontList(InfallibleTArray<FontListEntry>* retValue) override;
=======
  mozilla::ipc::IPCResult RecvGetGfxVars(InfallibleTArray<GfxVarUpdate>* aVars);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvSetClipboard(
      const IPCDataTransfer& aDataTransfer, const bool& aIsPrivateData,
      const IPC::Principal& aRequestingPrincipal,
      const uint32_t& aContentPolicyType,
      const int32_t& aWhichClipboard) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvSetClipboard(const IPCDataTransfer& aDataTransfer,
                                                   const bool& aIsPrivateData,
                                                   const IPC::Principal& aRequestingPrincipal,
                                                   const uint32_t& aContentPolicyType,
                                                   const int32_t& aWhichClipboard) override;
=======
  mozilla::ipc::IPCResult RecvReadFontList(
      InfallibleTArray<FontListEntry>* retValue);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvGetClipboard(
      nsTArray<nsCString>&& aTypes, const int32_t& aWhichClipboard,
      IPCDataTransfer* aDataTransfer) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvGetClipboard(nsTArray<nsCString>&& aTypes,
                                                   const int32_t& aWhichClipboard,
                                                   IPCDataTransfer* aDataTransfer) override;
=======
  mozilla::ipc::IPCResult RecvSetClipboard(
      const IPCDataTransfer& aDataTransfer, const bool& aIsPrivateData,
      const IPC::Principal& aRequestingPrincipal,
      const uint32_t& aContentPolicyType, const int32_t& aWhichClipboard);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvEmptyClipboard(
      const int32_t& aWhichClipboard) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvEmptyClipboard(const int32_t& aWhichClipboard) override;
=======
  mozilla::ipc::IPCResult RecvGetClipboard(nsTArray<nsCString>&& aTypes,
                                           const int32_t& aWhichClipboard,
                                           IPCDataTransfer* aDataTransfer);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvClipboardHasType(
      nsTArray<nsCString>&& aTypes, const int32_t& aWhichClipboard,
      bool* aHasType) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvClipboardHasType(nsTArray<nsCString>&& aTypes,
                                                       const int32_t& aWhichClipboard,
                                                       bool* aHasType) override;
=======
  mozilla::ipc::IPCResult RecvEmptyClipboard(const int32_t& aWhichClipboard);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvGetExternalClipboardFormats(
      const int32_t& aWhichClipboard, const bool& aPlainTextOnly,
      nsTArray<nsCString>* aTypes) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvGetExternalClipboardFormats(const int32_t& aWhichClipboard,
                                                                  const bool& aPlainTextOnly,
                                                                  nsTArray<nsCString>* aTypes) override;
=======
  mozilla::ipc::IPCResult RecvClipboardHasType(nsTArray<nsCString>&& aTypes,
                                               const int32_t& aWhichClipboard,
                                               bool* aHasType);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPlaySound(const URIParams& aURI) override;
  virtual mozilla::ipc::IPCResult RecvBeep() override;
  virtual mozilla::ipc::IPCResult RecvPlayEventSound(
      const uint32_t& aEventId) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvPlaySound(const URIParams& aURI) override;
  virtual mozilla::ipc::IPCResult RecvBeep() override;
  virtual mozilla::ipc::IPCResult RecvPlayEventSound(const uint32_t& aEventId) override;

  virtual mozilla::ipc::IPCResult RecvGetIconForExtension(const nsCString& aFileExt,
                                                          const uint32_t& aIconSize,
                                                          InfallibleTArray<uint8_t>* bits) override;
=======
  mozilla::ipc::IPCResult RecvGetExternalClipboardFormats(
      const int32_t& aWhichClipboard, const bool& aPlainTextOnly,
      nsTArray<nsCString>* aTypes);

  mozilla::ipc::IPCResult RecvPlaySound(const URIParams& aURI);
  mozilla::ipc::IPCResult RecvBeep();
  mozilla::ipc::IPCResult RecvPlayEventSound(const uint32_t& aEventId);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvGetIconForExtension(
      const nsCString& aFileExt, const uint32_t& aIconSize,
      InfallibleTArray<uint8_t>* bits) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvGetShowPasswordSetting(bool* showPassword) override;
=======
  mozilla::ipc::IPCResult RecvGetIconForExtension(
      const nsCString& aFileExt, const uint32_t& aIconSize,
      InfallibleTArray<uint8_t>* bits);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvGetShowPasswordSetting(
      bool* showPassword) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvStartVisitedQuery(const URIParams& uri) override;
=======
  mozilla::ipc::IPCResult RecvGetShowPasswordSetting(bool* showPassword);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvStartVisitedQuery(
      const URIParams& uri) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvVisitURI(const URIParams& uri,
                                               const OptionalURIParams& referrer,
                                               const uint32_t& flags) override;
=======
  mozilla::ipc::IPCResult RecvStartVisitedQuery(const URIParams& uri);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvSetURITitle(
      const URIParams& uri, const nsString& title) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvSetURITitle(const URIParams& uri,
                                                  const nsString& title) override;
=======
  mozilla::ipc::IPCResult RecvSetURITitle(const URIParams& uri,
                                          const nsString& title);
>>>>>>> upstream-releases

  bool HasNotificationPermission(const IPC::Principal& aPrincipal);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvShowAlert(
      nsIAlertNotification* aAlert) override;

  virtual mozilla::ipc::IPCResult RecvCloseAlert(
      const nsString& aName, const IPC::Principal& aPrincipal) override;

  virtual mozilla::ipc::IPCResult RecvDisableNotifications(
      const IPC::Principal& aPrincipal) override;

  virtual mozilla::ipc::IPCResult RecvOpenNotificationSettings(
      const IPC::Principal& aPrincipal) override;

  virtual mozilla::ipc::IPCResult RecvLoadURIExternal(
      const URIParams& uri, PBrowserParent* windowContext) override;
  virtual mozilla::ipc::IPCResult RecvExtProtocolChannelConnectParent(
      const uint32_t& registrarId) override;

  virtual mozilla::ipc::IPCResult RecvSyncMessage(
      const nsString& aMsg, const ClonedMessageData& aData,
      InfallibleTArray<CpowEntry>&& aCpows, const IPC::Principal& aPrincipal,
      nsTArray<StructuredCloneData>* aRetvals) override;

  virtual mozilla::ipc::IPCResult RecvRpcMessage(
      const nsString& aMsg, const ClonedMessageData& aData,
      InfallibleTArray<CpowEntry>&& aCpows, const IPC::Principal& aPrincipal,
      nsTArray<StructuredCloneData>* aRetvals) override;

  virtual mozilla::ipc::IPCResult RecvAsyncMessage(
      const nsString& aMsg, InfallibleTArray<CpowEntry>&& aCpows,
      const IPC::Principal& aPrincipal,
      const ClonedMessageData& aData) override;

  virtual mozilla::ipc::IPCResult RecvAddGeolocationListener(
      const IPC::Principal& aPrincipal, const bool& aHighAccuracy) override;
  virtual mozilla::ipc::IPCResult RecvRemoveGeolocationListener() override;

  virtual mozilla::ipc::IPCResult RecvSetGeolocationHigherAccuracy(
      const bool& aEnable) override;

  virtual mozilla::ipc::IPCResult RecvConsoleMessage(
      const nsString& aMessage) override;

  virtual mozilla::ipc::IPCResult RecvScriptError(
      const nsString& aMessage, const nsString& aSourceName,
      const nsString& aSourceLine, const uint32_t& aLineNumber,
      const uint32_t& aColNumber, const uint32_t& aFlags,
      const nsCString& aCategory, const bool& aIsFromPrivateWindow) override;

  virtual mozilla::ipc::IPCResult RecvScriptErrorWithStack(
      const nsString& aMessage, const nsString& aSourceName,
      const nsString& aSourceLine, const uint32_t& aLineNumber,
      const uint32_t& aColNumber, const uint32_t& aFlags,
      const nsCString& aCategory, const bool& aIsFromPrivateWindow,
      const ClonedMessageData& aStack) override;

 private:
  mozilla::ipc::IPCResult RecvScriptErrorInternal(
      const nsString& aMessage, const nsString& aSourceName,
      const nsString& aSourceLine, const uint32_t& aLineNumber,
      const uint32_t& aColNumber, const uint32_t& aFlags,
      const nsCString& aCategory, const bool& aIsFromPrivateWindow,
      const ClonedMessageData* aStack = nullptr);

 public:
  virtual mozilla::ipc::IPCResult RecvPrivateDocShellsExist(
      const bool& aExist) override;

  virtual mozilla::ipc::IPCResult RecvFirstIdle() override;

  virtual mozilla::ipc::IPCResult RecvDeviceReset() override;

  virtual mozilla::ipc::IPCResult RecvKeywordToURI(
      const nsCString& aKeyword, nsString* aProviderName,
      RefPtr<nsIInputStream>* aPostData, OptionalURIParams* aURI) override;

  virtual mozilla::ipc::IPCResult RecvNotifyKeywordSearchLoading(
      const nsString& aProvider, const nsString& aKeyword) override;

  virtual mozilla::ipc::IPCResult RecvCopyFavicon(
      const URIParams& aOldURI, const URIParams& aNewURI,
      const IPC::Principal& aLoadingPrincipal,
      const bool& aInPrivateBrowsing) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvShowAlert(nsIAlertNotification* aAlert) override;

  virtual mozilla::ipc::IPCResult RecvCloseAlert(const nsString& aName,
                                                 const IPC::Principal& aPrincipal) override;

  virtual mozilla::ipc::IPCResult RecvDisableNotifications(const IPC::Principal& aPrincipal) override;

  virtual mozilla::ipc::IPCResult RecvOpenNotificationSettings(const IPC::Principal& aPrincipal) override;

  virtual mozilla::ipc::IPCResult RecvLoadURIExternal(const URIParams& uri,
                                                      PBrowserParent* windowContext) override;
  virtual mozilla::ipc::IPCResult RecvExtProtocolChannelConnectParent(const uint32_t& registrarId) override;

  virtual mozilla::ipc::IPCResult RecvSyncMessage(const nsString& aMsg,
                                                  const ClonedMessageData& aData,
                                                  InfallibleTArray<CpowEntry>&& aCpows,
                                                  const IPC::Principal& aPrincipal,
                                                  nsTArray<StructuredCloneData>* aRetvals) override;

  virtual mozilla::ipc::IPCResult RecvRpcMessage(const nsString& aMsg,
                                                 const ClonedMessageData& aData,
                                                 InfallibleTArray<CpowEntry>&& aCpows,
                                                 const IPC::Principal& aPrincipal,
                                                 nsTArray<StructuredCloneData>* aRetvals) override;

  virtual mozilla::ipc::IPCResult RecvAsyncMessage(const nsString& aMsg,
                                                   InfallibleTArray<CpowEntry>&& aCpows,
                                                   const IPC::Principal& aPrincipal,
                                                   const ClonedMessageData& aData) override;

  virtual mozilla::ipc::IPCResult RecvAddGeolocationListener(const IPC::Principal& aPrincipal,
                                                             const bool& aHighAccuracy) override;
  virtual mozilla::ipc::IPCResult RecvRemoveGeolocationListener() override;

  virtual mozilla::ipc::IPCResult RecvSetGeolocationHigherAccuracy(const bool& aEnable) override;

  virtual mozilla::ipc::IPCResult RecvConsoleMessage(const nsString& aMessage) override;

  virtual mozilla::ipc::IPCResult RecvScriptError(const nsString& aMessage,
                                                  const nsString& aSourceName,
                                                  const nsString& aSourceLine,
                                                  const uint32_t& aLineNumber,
                                                  const uint32_t& aColNumber,
                                                  const uint32_t& aFlags,
                                                  const nsCString& aCategory,
                                                  const bool& aIsFromPrivateWindow) override;

  virtual mozilla::ipc::IPCResult RecvScriptErrorWithStack(const nsString& aMessage,
                                                           const nsString& aSourceName,
                                                           const nsString& aSourceLine,
                                                           const uint32_t& aLineNumber,
                                                           const uint32_t& aColNumber,
                                                           const uint32_t& aFlags,
                                                           const nsCString& aCategory,
                                                           const bool& aIsFromPrivateWindow,
                                                           const ClonedMessageData& aStack) override;

private:
  mozilla::ipc::IPCResult RecvScriptErrorInternal(const nsString& aMessage,
                                                  const nsString& aSourceName,
                                                  const nsString& aSourceLine,
                                                  const uint32_t& aLineNumber,
                                                  const uint32_t& aColNumber,
                                                  const uint32_t& aFlags,
                                                  const nsCString& aCategory,
                                                  const bool& aIsFromPrivateWindow,
                                                  const ClonedMessageData* aStack = nullptr);

public:
  virtual mozilla::ipc::IPCResult RecvPrivateDocShellsExist(const bool& aExist) override;

  virtual mozilla::ipc::IPCResult RecvFirstIdle() override;

  virtual mozilla::ipc::IPCResult RecvDeviceReset() override;

  virtual mozilla::ipc::IPCResult RecvKeywordToURI(const nsCString& aKeyword,
                                                   nsString* aProviderName,
                                                   RefPtr<nsIInputStream>* aPostData,
                                                   OptionalURIParams* aURI) override;

  virtual mozilla::ipc::IPCResult RecvNotifyKeywordSearchLoading(const nsString &aProvider,
                                                                 const nsString &aKeyword) override;

  virtual mozilla::ipc::IPCResult RecvCopyFavicon(const URIParams& aOldURI,
                                                  const URIParams& aNewURI,
                                                  const IPC::Principal& aLoadingPrincipal,
                                                  const bool& aInPrivateBrowsing) override;
=======
  mozilla::ipc::IPCResult RecvShowAlert(nsIAlertNotification* aAlert);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvCloseAlert(const nsString& aName,
                                         const IPC::Principal& aPrincipal);

  mozilla::ipc::IPCResult RecvDisableNotifications(
      const IPC::Principal& aPrincipal);

  mozilla::ipc::IPCResult RecvOpenNotificationSettings(
      const IPC::Principal& aPrincipal);

  mozilla::ipc::IPCResult RecvNotificationEvent(
      const nsString& aType, const NotificationEventData& aData);

  mozilla::ipc::IPCResult RecvLoadURIExternal(const URIParams& uri,
                                              PBrowserParent* windowContext);
  mozilla::ipc::IPCResult RecvExtProtocolChannelConnectParent(
      const uint32_t& registrarId);

  mozilla::ipc::IPCResult RecvSyncMessage(
      const nsString& aMsg, const ClonedMessageData& aData,
      InfallibleTArray<CpowEntry>&& aCpows, const IPC::Principal& aPrincipal,
      nsTArray<StructuredCloneData>* aRetvals);

  mozilla::ipc::IPCResult RecvRpcMessage(
      const nsString& aMsg, const ClonedMessageData& aData,
      InfallibleTArray<CpowEntry>&& aCpows, const IPC::Principal& aPrincipal,
      nsTArray<StructuredCloneData>* aRetvals);

  mozilla::ipc::IPCResult RecvAsyncMessage(const nsString& aMsg,
                                           InfallibleTArray<CpowEntry>&& aCpows,
                                           const IPC::Principal& aPrincipal,
                                           const ClonedMessageData& aData);

  // MOZ_CAN_RUN_SCRIPT_BOUNDARY because we don't have MOZ_CAN_RUN_SCRIPT bits
  // in IPC code yet.
  MOZ_CAN_RUN_SCRIPT_BOUNDARY
  mozilla::ipc::IPCResult RecvAddGeolocationListener(
      const IPC::Principal& aPrincipal, const bool& aHighAccuracy);
  mozilla::ipc::IPCResult RecvRemoveGeolocationListener();

  // MOZ_CAN_RUN_SCRIPT_BOUNDARY because we don't have MOZ_CAN_RUN_SCRIPT bits
  // in IPC code yet.
  MOZ_CAN_RUN_SCRIPT_BOUNDARY
  mozilla::ipc::IPCResult RecvSetGeolocationHigherAccuracy(const bool& aEnable);

  mozilla::ipc::IPCResult RecvConsoleMessage(const nsString& aMessage);

  mozilla::ipc::IPCResult RecvScriptError(
      const nsString& aMessage, const nsString& aSourceName,
      const nsString& aSourceLine, const uint32_t& aLineNumber,
      const uint32_t& aColNumber, const uint32_t& aFlags,
      const nsCString& aCategory, const bool& aIsFromPrivateWindow,
      const bool& aIsFromChromeContext);

  mozilla::ipc::IPCResult RecvScriptErrorWithStack(
      const nsString& aMessage, const nsString& aSourceName,
      const nsString& aSourceLine, const uint32_t& aLineNumber,
      const uint32_t& aColNumber, const uint32_t& aFlags,
      const nsCString& aCategory, const bool& aIsFromPrivateWindow,
      const bool& aIsFromChromeContext, const ClonedMessageData& aStack);

 private:
  mozilla::ipc::IPCResult RecvScriptErrorInternal(
      const nsString& aMessage, const nsString& aSourceName,
      const nsString& aSourceLine, const uint32_t& aLineNumber,
      const uint32_t& aColNumber, const uint32_t& aFlags,
      const nsCString& aCategory, const bool& aIsFromPrivateWindow,
      const bool& aIsFromChromeContext,
      const ClonedMessageData* aStack = nullptr);

 public:
  mozilla::ipc::IPCResult RecvPrivateDocShellsExist(const bool& aExist);

  mozilla::ipc::IPCResult RecvCommitBrowsingContextTransaction(
      BrowsingContext* aContext, BrowsingContext::Transaction&& aTransaction,
      BrowsingContext::FieldEpochs&& aEpochs);

  mozilla::ipc::IPCResult RecvFirstIdle();

  mozilla::ipc::IPCResult RecvDeviceReset();

  mozilla::ipc::IPCResult RecvKeywordToURI(const nsCString& aKeyword,
                                           nsString* aProviderName,
                                           RefPtr<nsIInputStream>* aPostData,
                                           Maybe<URIParams>* aURI);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvGraphicsError(
      const nsCString& aError) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvAllocateLayerTreeId(const ContentParentId& aCpId,
                                                          const TabId& aTabId,
                                                          layers::LayersId* aId) override;

  virtual mozilla::ipc::IPCResult RecvDeallocateLayerTreeId(const ContentParentId& aCpId,
                                                            const layers::LayersId& aId) override;

  virtual mozilla::ipc::IPCResult RecvGraphicsError(const nsCString& aError) override;
=======
  mozilla::ipc::IPCResult RecvNotifyKeywordSearchLoading(
      const nsString& aProvider, const nsString& aKeyword);

  mozilla::ipc::IPCResult RecvCopyFavicon(
      const URIParams& aOldURI, const URIParams& aNewURI,
      const IPC::Principal& aLoadingPrincipal, const bool& aInPrivateBrowsing);

  virtual void ProcessingError(Result aCode, const char* aMsgName) override;

  mozilla::ipc::IPCResult RecvGraphicsError(const nsCString& aError);

  mozilla::ipc::IPCResult RecvBeginDriverCrashGuard(const uint32_t& aGuardType,
                                                    bool* aOutCrashed);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvBeginDriverCrashGuard(
      const uint32_t& aGuardType, bool* aOutCrashed) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvBeginDriverCrashGuard(const uint32_t& aGuardType,
                            bool* aOutCrashed) override;
=======
  mozilla::ipc::IPCResult RecvEndDriverCrashGuard(const uint32_t& aGuardType);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvEndDriverCrashGuard(
      const uint32_t& aGuardType) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvEndDriverCrashGuard(const uint32_t& aGuardType) override;
=======
  mozilla::ipc::IPCResult RecvAddIdleObserver(const uint64_t& observerId,
                                              const uint32_t& aIdleTimeInS);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvAddIdleObserver(
      const uint64_t& observerId, const uint32_t& aIdleTimeInS) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvAddIdleObserver(const uint64_t& observerId,
                                                      const uint32_t& aIdleTimeInS) override;
=======
  mozilla::ipc::IPCResult RecvRemoveIdleObserver(const uint64_t& observerId,
                                                 const uint32_t& aIdleTimeInS);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvRemoveIdleObserver(
      const uint64_t& observerId, const uint32_t& aIdleTimeInS) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvRemoveIdleObserver(const uint64_t& observerId,
                                                         const uint32_t& aIdleTimeInS) override;
=======
  mozilla::ipc::IPCResult RecvBackUpXResources(
      const FileDescriptor& aXSocketFd);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvBackUpXResources(
      const FileDescriptor& aXSocketFd) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvBackUpXResources(const FileDescriptor& aXSocketFd) override;
=======
  mozilla::ipc::IPCResult RecvRequestAnonymousTemporaryFile(
      const uint64_t& aID);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvRequestAnonymousTemporaryFile(
      const uint64_t& aID) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvRequestAnonymousTemporaryFile(const uint64_t& aID) override;
=======
  mozilla::ipc::IPCResult RecvCreateAudioIPCConnection(
      CreateAudioIPCConnectionResolver&& aResolver);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvCreateAudioIPCConnection(
      CreateAudioIPCConnectionResolver&& aResolver) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvCreateAudioIPCConnection(CreateAudioIPCConnectionResolver&& aResolver) override;
=======
  PFileDescriptorSetParent* AllocPFileDescriptorSetParent(
      const mozilla::ipc::FileDescriptor&);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvKeygenProcessValue(
      const nsString& oldValue, const nsString& challenge,
      const nsString& keytype, const nsString& keyparams,
      nsString* newValue) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvKeygenProcessValue(const nsString& oldValue, const nsString& challenge,
                         const nsString& keytype, const nsString& keyparams,
                         nsString* newValue) override;
=======
  bool DeallocPFileDescriptorSetParent(PFileDescriptorSetParent*);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvKeygenProvideContent(
      nsString* aAttribute, nsTArray<nsString>* aContent) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvKeygenProvideContent(nsString* aAttribute,
                           nsTArray<nsString>* aContent) override;
=======
  PWebrtcGlobalParent* AllocPWebrtcGlobalParent();
  bool DeallocPWebrtcGlobalParent(PWebrtcGlobalParent* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PFileDescriptorSetParent* AllocPFileDescriptorSetParent(
      const mozilla::ipc::FileDescriptor&) override;
||||||| merged common ancestors
  virtual PFileDescriptorSetParent*
  AllocPFileDescriptorSetParent(const mozilla::ipc::FileDescriptor&) override;
=======
  mozilla::ipc::IPCResult RecvUpdateDropEffect(const uint32_t& aDragAction,
                                               const uint32_t& aDropEffect);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPFileDescriptorSetParent(
      PFileDescriptorSetParent*) override;
||||||| merged common ancestors
  virtual bool
  DeallocPFileDescriptorSetParent(PFileDescriptorSetParent*) override;
=======
  mozilla::ipc::IPCResult RecvShutdownProfile(const nsCString& aProfile);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PWebrtcGlobalParent* AllocPWebrtcGlobalParent() override;
  virtual bool DeallocPWebrtcGlobalParent(PWebrtcGlobalParent* aActor) override;
||||||| merged common ancestors
  virtual PWebrtcGlobalParent* AllocPWebrtcGlobalParent() override;
  virtual bool DeallocPWebrtcGlobalParent(PWebrtcGlobalParent *aActor) override;
=======
  mozilla::ipc::IPCResult RecvGetGraphicsDeviceInitData(
      ContentDeviceData* aOut);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvUpdateDropEffect(
      const uint32_t& aDragAction, const uint32_t& aDropEffect) override;
||||||| merged common ancestors
=======
  mozilla::ipc::IPCResult RecvGetFontListShmBlock(
      const uint32_t& aGeneration, const uint32_t& aIndex,
      mozilla::ipc::SharedMemoryBasic::Handle* aOut);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvShutdownProfile(
      const nsCString& aProfile) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvUpdateDropEffect(const uint32_t& aDragAction,
                                                       const uint32_t& aDropEffect) override;
=======
  mozilla::ipc::IPCResult RecvInitializeFamily(const uint32_t& aGeneration,
                                               const uint32_t& aFamilyIndex);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvGetGraphicsDeviceInitData(
      ContentDeviceData* aOut) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvShutdownProfile(const nsCString& aProfile) override;
=======
  mozilla::ipc::IPCResult RecvSetCharacterMap(
      const uint32_t& aGeneration, const mozilla::fontlist::Pointer& aFacePtr,
      const gfxSparseBitSet& aMap);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvGetAndroidSystemInfo(
      AndroidSystemInfo* aInfo) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvGetGraphicsDeviceInitData(ContentDeviceData* aOut) override;
=======
  mozilla::ipc::IPCResult RecvInitOtherFamilyNames(const uint32_t& aGeneration,
                                                   const bool& aDefer,
                                                   bool* aLoaded);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyBenchmarkResult(
      const nsString& aCodecName, const uint32_t& aDecodeFPS) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvGetAndroidSystemInfo(AndroidSystemInfo* aInfo) override;
=======
  mozilla::ipc::IPCResult RecvSetupFamilyCharMap(
      const uint32_t& aGeneration,
      const mozilla::fontlist::Pointer& aFamilyPtr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyPushObservers(
      const nsCString& aScope, const IPC::Principal& aPrincipal,
      const nsString& aMessageId) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifyBenchmarkResult(const nsString& aCodecName,
                                                            const uint32_t& aDecodeFPS) override;
=======
  mozilla::ipc::IPCResult RecvNotifyBenchmarkResult(const nsString& aCodecName,
                                                    const uint32_t& aDecodeFPS);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyPushObserversWithData(
      const nsCString& aScope, const IPC::Principal& aPrincipal,
      const nsString& aMessageId, InfallibleTArray<uint8_t>&& aData) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifyPushObservers(const nsCString& aScope,
                                                          const IPC::Principal& aPrincipal,
                                                          const nsString& aMessageId) override;
=======
  mozilla::ipc::IPCResult RecvNotifyPushObservers(
      const nsCString& aScope, const IPC::Principal& aPrincipal,
      const nsString& aMessageId);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyPushSubscriptionChangeObservers(
      const nsCString& aScope, const IPC::Principal& aPrincipal) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifyPushObserversWithData(const nsCString& aScope,
                                                                  const IPC::Principal& aPrincipal,
                                                                  const nsString& aMessageId,
                                                                  InfallibleTArray<uint8_t>&& aData) override;
=======
  mozilla::ipc::IPCResult RecvNotifyPushObserversWithData(
      const nsCString& aScope, const IPC::Principal& aPrincipal,
      const nsString& aMessageId, InfallibleTArray<uint8_t>&& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyPushSubscriptionModifiedObservers(
      const nsCString& aScope, const IPC::Principal& aPrincipal) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifyPushSubscriptionChangeObservers(const nsCString& aScope,
                                                                            const IPC::Principal& aPrincipal) override;
=======
  mozilla::ipc::IPCResult RecvNotifyPushSubscriptionChangeObservers(
      const nsCString& aScope, const IPC::Principal& aPrincipal);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvGetFilesRequest(
      const nsID& aID, const nsString& aDirectoryPath,
      const bool& aRecursiveFlag) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvNotifyPushSubscriptionModifiedObservers(const nsCString& aScope,
                                                                              const IPC::Principal& aPrincipal) override;
=======
  mozilla::ipc::IPCResult RecvPushError(const nsCString& aScope,
                                        const IPC::Principal& aPrincipal,
                                        const nsString& aMessage,
                                        const uint32_t& aFlags);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvDeleteGetFilesRequest(
      const nsID& aID) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvGetFilesRequest(const nsID& aID,
                                                      const nsString& aDirectoryPath,
                                                      const bool& aRecursiveFlag) override;
=======
  mozilla::ipc::IPCResult RecvNotifyPushSubscriptionModifiedObservers(
      const nsCString& aScope, const IPC::Principal& aPrincipal);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvFileCreationRequest(
      const nsID& aID, const nsString& aFullPath, const nsString& aType,
      const nsString& aName, const bool& aLastModifiedPassed,
      const int64_t& aLastModified, const bool& aExistenceCheck,
      const bool& aIsFromNsIFile) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvDeleteGetFilesRequest(const nsID& aID) override;

  virtual mozilla::ipc::IPCResult
  RecvFileCreationRequest(const nsID& aID, const nsString& aFullPath,
                          const nsString& aType, const nsString& aName,
                          const bool& aLastModifiedPassed,
                          const int64_t& aLastModified,
                          const bool& aExistenceCheck,
                          const bool& aIsFromNsIFile) override;
=======
  mozilla::ipc::IPCResult RecvGetFilesRequest(const nsID& aID,
                                              const nsString& aDirectoryPath,
                                              const bool& aRecursiveFlag);

  mozilla::ipc::IPCResult RecvDeleteGetFilesRequest(const nsID& aID);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvAccumulateChildHistograms(
      InfallibleTArray<HistogramAccumulation>&& aAccumulations) override;
  virtual mozilla::ipc::IPCResult RecvAccumulateChildKeyedHistograms(
      InfallibleTArray<KeyedHistogramAccumulation>&& aAccumulations) override;
  virtual mozilla::ipc::IPCResult RecvUpdateChildScalars(
      InfallibleTArray<ScalarAction>&& aScalarActions) override;
  virtual mozilla::ipc::IPCResult RecvUpdateChildKeyedScalars(
      InfallibleTArray<KeyedScalarAction>&& aScalarActions) override;
  virtual mozilla::ipc::IPCResult RecvRecordChildEvents(
      nsTArray<ChildEventData>&& events) override;
  virtual mozilla::ipc::IPCResult RecvRecordDiscardedData(
      const DiscardedData& aDiscardedData) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvAccumulateChildHistograms(
    InfallibleTArray<HistogramAccumulation>&& aAccumulations) override;
  virtual mozilla::ipc::IPCResult RecvAccumulateChildKeyedHistograms(
    InfallibleTArray<KeyedHistogramAccumulation>&& aAccumulations) override;
  virtual mozilla::ipc::IPCResult RecvUpdateChildScalars(
    InfallibleTArray<ScalarAction>&& aScalarActions) override;
  virtual mozilla::ipc::IPCResult RecvUpdateChildKeyedScalars(
    InfallibleTArray<KeyedScalarAction>&& aScalarActions) override;
  virtual mozilla::ipc::IPCResult RecvRecordChildEvents(
    nsTArray<ChildEventData>&& events) override;
  virtual mozilla::ipc::IPCResult RecvRecordDiscardedData(
    const DiscardedData& aDiscardedData) override;
=======
  mozilla::ipc::IPCResult RecvAccumulateChildHistograms(
      InfallibleTArray<HistogramAccumulation>&& aAccumulations);
  mozilla::ipc::IPCResult RecvAccumulateChildKeyedHistograms(
      InfallibleTArray<KeyedHistogramAccumulation>&& aAccumulations);
  mozilla::ipc::IPCResult RecvUpdateChildScalars(
      InfallibleTArray<ScalarAction>&& aScalarActions);
  mozilla::ipc::IPCResult RecvUpdateChildKeyedScalars(
      InfallibleTArray<KeyedScalarAction>&& aScalarActions);
  mozilla::ipc::IPCResult RecvRecordChildEvents(
      nsTArray<ChildEventData>&& events);
  mozilla::ipc::IPCResult RecvRecordDiscardedData(
      const DiscardedData& aDiscardedData);
  mozilla::ipc::IPCResult RecvRecordOrigin(const uint32_t& aMetricId,
                                           const nsCString& aOrigin);
  mozilla::ipc::IPCResult RecvReportContentBlockingLog(
      const IPCStream& aIPCStream);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvBHRThreadHang(
      const HangDetails& aHangDetails) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult RecvBHRThreadHang(
    const HangDetails& aHangDetails) override;
=======
  mozilla::ipc::IPCResult RecvBHRThreadHang(const HangDetails& aHangDetails);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvFirstPartyStorageAccessGrantedForOrigin(
      const Principal& aParentPrincipal, const Principal& aTrackingPrincipal,
      const nsCString& aTrackingOrigin, const nsCString& aGrantedOrigin,
      const int& aAllowMode,
      FirstPartyStorageAccessGrantedForOriginResolver&& aResolver) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvFirstPartyStorageAccessGrantedForOrigin(const Principal& aParentPrincipal,
                                              const nsCString& aTrackingOrigin,
                                              const nsCString& aGrantedOrigin,
                                              FirstPartyStorageAccessGrantedForOriginResolver&& aResolver) override;
=======
  mozilla::ipc::IPCResult RecvFirstPartyStorageAccessGrantedForOrigin(
      const Principal& aParentPrincipal, const Principal& aTrackingPrincipal,
      const nsCString& aTrackingOrigin, const nsCString& aGrantedOrigin,
      const int& aAllowMode,
      FirstPartyStorageAccessGrantedForOriginResolver&& aResolver);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvStoreUserInteractionAsPermission(
      const Principal& aPrincipal) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvStoreUserInteractionAsPermission(const Principal& aPrincipal) override;
=======
  mozilla::ipc::IPCResult RecvStoreUserInteractionAsPermission(
      const Principal& aPrincipal);
>>>>>>> upstream-releases

  // Notify the ContentChild to enable the input event prioritization when
  // initializing.
  void MaybeEnableRemoteInputEventQueue();

<<<<<<< HEAD
#if defined(XP_MACOSX) && defined(MOZ_CONTENT_SANDBOX)
  void AppendSandboxParams(std::vector<std::string>& aArgs);
  void AppendDynamicSandboxParams(std::vector<std::string>& aArgs);
#endif

 public:
||||||| merged common ancestors
public:
=======
#if defined(XP_MACOSX) && defined(MOZ_SANDBOX)
  void AppendSandboxParams(std::vector<std::string>& aArgs);
  void AppendDynamicSandboxParams(std::vector<std::string>& aArgs);
#endif

 public:
>>>>>>> upstream-releases
  void SendGetFilesResponseAndForget(const nsID& aID,
                                     const GetFilesResponseResult& aResult);

  bool SendRequestMemoryReport(const uint32_t& aGeneration,
                               const bool& aAnonymize,
                               const bool& aMinimizeMemoryUsage,
                               const Maybe<FileDescriptor>& aDMDFile) override;

  nsresult SaveRecording(nsIFile* aFile, bool* aRetval);

  bool IsRecordingOrReplaying() const {
    return mRecordReplayState != eNotRecordingOrReplaying;
  }

<<<<<<< HEAD
 private:
  // Released in ActorDestroy; deliberately not exposed to the CC.
||||||| merged common ancestors
private:
  // Released in ActorDestroy; deliberately not exposed to the CC.
=======
  void OnBrowsingContextGroupSubscribe(BrowsingContextGroup* aGroup);
  void OnBrowsingContextGroupUnsubscribe(BrowsingContextGroup* aGroup);

  void UpdateNetworkLinkType();

  static bool ShouldSyncPreference(const char16_t* aData);

 private:
  // Released in ActorDealloc; deliberately not exposed to the CC.
>>>>>>> upstream-releases
  RefPtr<ContentParent> mSelfRef;

  // If you add strong pointers to cycle collected objects here, be sure to
  // release these objects in ShutDownProcess.  See the comment there for more
  // details.

  GeckoChildProcessHost* mSubprocess;
  const TimeStamp mLaunchTS;  // used to calculate time to start content process
  TimeStamp mLaunchYieldTS;   // used to calculate async launch main thread time
  TimeStamp mActivateTS;
  ContentParent* mOpener;

  nsString mRemoteType;

  ContentParentId mChildID;
  int32_t mGeolocationWatchID;

  // This contains the id for the JS plugin (@see nsFakePluginTag) if this is
  // the ContentParent for a process containing iframes for that JS plugin. If
  // this is not a ContentParent for a JS plugin then it contains the value
  // nsFakePluginTag::NOT_JSPLUGIN.
  int32_t mJSPluginID;

  // After we initiate shutdown, we also start a timer to ensure
  // that even content processes that are 100% blocked (say from
  // SIGSTOP), are still killed eventually.  This task enforces that
  // timer.
  nsCOMPtr<nsITimer> mForceKillTimer;

  // Number of active remote workers. This value is increased when a
  // RemoteWorkerParent actor is created for this ContentProcess and it is
  // decreased when the actor is destroyed.
  // It's touched on PBackground thread and on main-thread.
  Atomic<uint32_t> mRemoteWorkerActors;

  // How many tabs we're waiting to finish their destruction
  // sequence.  Precisely, how many BrowserParents have called
  // NotifyTabDestroying() but not called NotifyTabDestroyed().
  int32_t mNumDestroyingTabs;
<<<<<<< HEAD

  // The process starts in the LAUNCHING state, and transitions to
  // ALIVE once it can accept IPC messages.  It remains ALIVE only
  // while remote content is being actively used from this process.
  // After the state becaomes DEAD, some previously scheduled IPC
  // traffic may still pass through.
  enum class LifecycleState : uint8_t {
    LAUNCHING,
    ALIVE,
    DEAD,
  };

  LifecycleState mLifecycleState;

  bool mShuttingDown;
||||||| merged common ancestors
  // True only while this process is in "good health" and may be used for
  // new remote tabs.
  bool mIsAvailable;
  // True only while remote content is being actively used from this process.
  // After mIsAlive goes to false, some previously scheduled IPC traffic may
  // still pass through.
  bool mIsAlive;

  bool mShuttingDown;
=======

  // The process starts in the LAUNCHING state, and transitions to
  // ALIVE once it can accept IPC messages.  It remains ALIVE only
  // while remote content is being actively used from this process.
  // After the state becaomes DEAD, some previously scheduled IPC
  // traffic may still pass through.
  enum class LifecycleState : uint8_t {
    LAUNCHING,
    ALIVE,
    DEAD,
  };

  LifecycleState mLifecycleState;

>>>>>>> upstream-releases
  bool mIsForBrowser;

  // Whether this process is recording or replaying its execution, and any
  // associated recording file.
  RecordReplayState mRecordReplayState;
  nsString mRecordingFile;

  // When recording or replaying, the child process is a middleman. This vector
  // stores any replaying children we have spawned on behalf of that middleman,
  // indexed by their record/replay channel ID.
  Vector<mozilla::ipc::GeckoChildProcessHost*> mReplayingChildren;

  // These variables track whether we've called Close() and KillHard() on our
  // channel.
  bool mCalledClose;
  bool mCalledKillHard;
  bool mCreatedPairedMinidumps;
  bool mShutdownPending;
  bool mIPCOpen;

  // True if the input event queue on the main thread of the content process is
  // enabled.
  bool mIsRemoteInputEventQueueEnabled;

  // True if we send input events with input priority. Otherwise, we send input
  // events with normal priority.
  bool mIsInputPriorityEventEnabled;

  RefPtr<nsConsoleService> mConsoleService;
  nsConsoleService* GetConsoleService();
  nsCOMPtr<nsIContentProcessInfo> mScriptableHelper;

  nsTArray<nsCOMPtr<nsIObserver>> mIdleListeners;

#ifdef MOZ_X11
  // Dup of child's X socket, used to scope its resources to this
  // object instead of the child process's lifetime.
  ScopedClose mChildXSocketFdDup;
#endif

  PProcessHangMonitorParent* mHangMonitorActor;

  UniquePtr<gfx::DriverCrashGuard> mDriverCrashGuard;
  UniquePtr<MemoryReportRequestHost> mMemoryReportRequest;

#if defined(XP_LINUX) && defined(MOZ_SANDBOX)
  mozilla::UniquePtr<SandboxBroker> mSandboxBroker;
  static mozilla::UniquePtr<SandboxBrokerPolicyFactory>
      sSandboxBrokerPolicyFactory;
#endif

#ifdef NS_PRINTING
  RefPtr<embedding::PrintingParent> mPrintingParent;
#endif

  // This hashtable is used to run GetFilesHelper objects in the parent process.
  // GetFilesHelper can be aborted by receiving RecvDeleteGetFilesRequest.
  nsRefPtrHashtable<nsIDHashKey, GetFilesHelper> mGetFilesPendingRequests;

  nsTHashtable<nsCStringHashKey> mActivePermissionKeys;

  nsTArray<nsCString> mBlobURLs;

  UniquePtr<mozilla::ipc::CrashReporterHost> mCrashReporter;

<<<<<<< HEAD
  // Collects any pref changes that occur during process launch (after
  // the initial map is passed in command-line arguments) to be sent
  // when the process can receive IPC messages.
  nsTArray<Pref> mQueuedPrefs;

  static uint64_t sNextTabParentId;
  static nsDataHashtable<nsUint64HashKey, TabParent*> sNextTabParents;

#if defined(XP_MACOSX) && defined(MOZ_CONTENT_SANDBOX)
  // When set to true, indicates that content processes should
  // initialize their sandbox during startup instead of waiting
  // for the SetProcessSandbox IPDL message.
  static bool sEarlySandboxInit;
#endif
||||||| merged common ancestors
  static uint64_t sNextTabParentId;
  static nsDataHashtable<nsUint64HashKey, TabParent*> sNextTabParents;
=======
  // Collects any pref changes that occur during process launch (after
  // the initial map is passed in command-line arguments) to be sent
  // when the process can receive IPC messages.
  nsTArray<Pref> mQueuedPrefs;

  RefPtr<mozilla::dom::ProcessMessageManager> mMessageManager;

  static uint64_t sNextRemoteTabId;
  static nsDataHashtable<nsUint64HashKey, BrowserParent*> sNextBrowserParents;

#if defined(XP_MACOSX) && defined(MOZ_SANDBOX)
  // When set to true, indicates that content processes should
  // initialize their sandbox during startup instead of waiting
  // for the SetProcessSandbox IPDL message.
  static bool sEarlySandboxInit;
#endif

  nsTHashtable<nsRefPtrHashKey<BrowsingContextGroup>> mGroups;
>>>>>>> upstream-releases
};

<<<<<<< HEAD
}  // namespace dom
}  // namespace mozilla
||||||| merged common ancestors
} // namespace dom
} // namespace mozilla
=======
NS_DEFINE_STATIC_IID_ACCESSOR(ContentParent, NS_CONTENTPARENT_IID)

const nsDependentSubstring RemoteTypePrefix(
    const nsAString& aContentProcessType);
}  // namespace dom
}  // namespace mozilla
>>>>>>> upstream-releases

class ParentIdleListener : public nsIObserver {
  friend class mozilla::dom::ContentParent;

 public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIOBSERVER

  ParentIdleListener(mozilla::dom::ContentParent* aParent, uint64_t aObserver,
                     uint32_t aTime)
      : mParent(aParent), mObserver(aObserver), mTime(aTime) {}

 private:
  virtual ~ParentIdleListener() {}

  RefPtr<mozilla::dom::ContentParent> mParent;
  uint64_t mObserver;
  uint32_t mTime;
};

#endif  // mozilla_dom_ContentParent_h
