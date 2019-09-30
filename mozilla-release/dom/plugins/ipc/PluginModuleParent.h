/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: sw=2 ts=4 et :
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_plugins_PluginModuleParent_h
#define mozilla_plugins_PluginModuleParent_h

#include "base/process.h"
#include "mozilla/FileUtils.h"
#include "mozilla/HangAnnotations.h"
#include "mozilla/PluginLibrary.h"
#include "mozilla/plugins/PluginProcessParent.h"
#include "mozilla/plugins/PPluginModuleParent.h"
#include "mozilla/plugins/PluginMessageUtils.h"
#include "mozilla/plugins/PluginTypes.h"
#include "mozilla/ipc/TaskFactory.h"
#include "mozilla/TimeStamp.h"
#include "mozilla/Unused.h"
#include "npapi.h"
#include "npfunctions.h"
#include "nsExceptionHandler.h"
#include "nsDataHashtable.h"
#include "nsHashKeys.h"
#include "nsIObserver.h"
#ifdef XP_WIN
#  include "nsWindowsHelpers.h"
#endif

class nsPluginTag;

namespace mozilla {

namespace ipc {
class CrashReporterHost;
}  // namespace ipc
namespace layers {
class TextureClientRecycleAllocator;
}  // namespace layers

namespace plugins {
//-----------------------------------------------------------------------------

class BrowserStreamParent;
class PluginInstanceParent;

#ifdef XP_WIN
class PluginHangUIParent;
class FunctionBrokerParent;
#endif
#ifdef MOZ_CRASHREPORTER_INJECTOR
class FinishInjectorInitTask;
#endif

/**
 * PluginModuleParent
 *
 * This class implements the NPP API from the perspective of the rest
 * of Gecko, forwarding NPP calls along to the child process that is
 * actually running the plugin.
 *
 * This class /also/ implements a version of the NPN API, because the
 * child process needs to make these calls back into Gecko proper.
 * This class is responsible for "actually" making those function calls.
 *
 * If a plugin is running, there will always be one PluginModuleParent for it in
 * the chrome process. In addition, any content process using the plugin will
 * have its own PluginModuleParent. The subclasses PluginModuleChromeParent and
 * PluginModuleContentParent implement functionality that is specific to one
 * case or the other.
 */
class PluginModuleParent : public PPluginModuleParent,
                           public PluginLibrary
#ifdef MOZ_CRASHREPORTER_INJECTOR
    ,
                           public CrashReporter::InjectorCrashCallback
#endif
{
<<<<<<< HEAD
 protected:
  typedef mozilla::PluginLibrary PluginLibrary;
||||||| merged common ancestors
protected:
    typedef mozilla::PluginLibrary PluginLibrary;
=======
  friend class PPluginModuleParent;
>>>>>>> upstream-releases

<<<<<<< HEAD
  PPluginInstanceParent* AllocPPluginInstanceParent(
      const nsCString& aMimeType, const InfallibleTArray<nsCString>& aNames,
      const InfallibleTArray<nsCString>& aValues) override;
||||||| merged common ancestors
    PPluginInstanceParent*
    AllocPPluginInstanceParent(const nsCString& aMimeType,
                               const InfallibleTArray<nsCString>& aNames,
                               const InfallibleTArray<nsCString>& aValues)
                               override;
=======
 protected:
  typedef mozilla::PluginLibrary PluginLibrary;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPPluginInstanceParent(
      PPluginInstanceParent* aActor) override;
||||||| merged common ancestors
    virtual bool
    DeallocPPluginInstanceParent(PPluginInstanceParent* aActor) override;
=======
  PPluginInstanceParent* AllocPPluginInstanceParent(
      const nsCString& aMimeType, const InfallibleTArray<nsCString>& aNames,
      const InfallibleTArray<nsCString>& aValues);
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  explicit PluginModuleParent(bool aIsChrome);
  virtual ~PluginModuleParent();
||||||| merged common ancestors
public:
    explicit PluginModuleParent(bool aIsChrome);
    virtual ~PluginModuleParent();
=======
  bool DeallocPPluginInstanceParent(PPluginInstanceParent* aActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool IsChrome() const { return mIsChrome; }
||||||| merged common ancestors
    bool IsChrome() const { return mIsChrome; }
=======
 public:
  explicit PluginModuleParent(bool aIsChrome);
  virtual ~PluginModuleParent();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void SetPlugin(nsNPAPIPlugin* plugin) override { mPlugin = plugin; }
||||||| merged common ancestors
    virtual void SetPlugin(nsNPAPIPlugin* plugin) override
    {
        mPlugin = plugin;
    }
=======
  bool IsChrome() const { return mIsChrome; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void ActorDestroy(ActorDestroyReason why) override;
||||||| merged common ancestors
    virtual void ActorDestroy(ActorDestroyReason why) override;
=======
  virtual void SetPlugin(nsNPAPIPlugin* plugin) override { mPlugin = plugin; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const NPNetscapeFuncs* GetNetscapeFuncs() { return mNPNIface; }
||||||| merged common ancestors
    const NPNetscapeFuncs* GetNetscapeFuncs() {
        return mNPNIface;
    }
=======
  virtual void ActorDestroy(ActorDestroyReason why) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool OkToCleanup() const { return !IsOnCxxStack(); }
||||||| merged common ancestors
    bool OkToCleanup() const {
        return !IsOnCxxStack();
    }
=======
  const NPNetscapeFuncs* GetNetscapeFuncs() { return mNPNIface; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void ProcessRemoteNativeEventsInInterruptCall() override;
||||||| merged common ancestors
    void ProcessRemoteNativeEventsInInterruptCall() override;
=======
  bool OkToCleanup() const { return !IsOnCxxStack(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual nsresult GetRunID(uint32_t* aRunID) override;
  virtual void SetHasLocalInstance() override { mHadLocalInstance = true; }
||||||| merged common ancestors
    virtual nsresult GetRunID(uint32_t* aRunID) override;
    virtual void SetHasLocalInstance() override {
        mHadLocalInstance = true;
    }
=======
  void ProcessRemoteNativeEventsInInterruptCall() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  int GetQuirks() { return mQuirks; }
||||||| merged common ancestors
    int GetQuirks() { return mQuirks; }
=======
  virtual nsresult GetRunID(uint32_t* aRunID) override;
  virtual void SetHasLocalInstance() override { mHadLocalInstance = true; }
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  virtual mozilla::ipc::RacyInterruptPolicy MediateInterruptRace(
      const MessageInfo& parent, const MessageInfo& child) override {
    return MediateRace(parent, child);
  }
||||||| merged common ancestors
protected:
    virtual mozilla::ipc::RacyInterruptPolicy
    MediateInterruptRace(const MessageInfo& parent,
                         const MessageInfo& child) override
    {
        return MediateRace(parent, child);
    }
=======
  int GetQuirks() { return mQuirks; }

 protected:
  virtual mozilla::ipc::RacyInterruptPolicy MediateInterruptRace(
      const MessageInfo& parent, const MessageInfo& child) override {
    return MediateRace(parent, child);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvBackUpXResources(
      const FileDescriptor& aXSocketFd) override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult
    RecvBackUpXResources(const FileDescriptor& aXSocketFd) override;
=======
  mozilla::ipc::IPCResult RecvBackUpXResources(
      const FileDescriptor& aXSocketFd);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult AnswerProcessSomeEvents() override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult AnswerProcessSomeEvents() override;
=======
  mozilla::ipc::IPCResult AnswerProcessSomeEvents();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvProcessNativeEventsInInterruptCall()
      override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult
    RecvProcessNativeEventsInInterruptCall() override;
=======
  mozilla::ipc::IPCResult RecvProcessNativeEventsInInterruptCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPluginShowWindow(
      const uint32_t& aWindowId, const bool& aModal, const int32_t& aX,
      const int32_t& aY, const size_t& aWidth, const size_t& aHeight) override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult
    RecvPluginShowWindow(const uint32_t& aWindowId, const bool& aModal,
                         const int32_t& aX, const int32_t& aY,
                         const size_t& aWidth, const size_t& aHeight) override;
=======
  mozilla::ipc::IPCResult RecvPluginShowWindow(
      const uint32_t& aWindowId, const bool& aModal, const int32_t& aX,
      const int32_t& aY, const double& aWidth, const double& aHeight);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPluginHideWindow(
      const uint32_t& aWindowId) override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult
    RecvPluginHideWindow(const uint32_t& aWindowId) override;
=======
  mozilla::ipc::IPCResult RecvPluginHideWindow(const uint32_t& aWindowId);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvSetCursor(
      const NSCursorInfo& aCursorInfo) override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult
    RecvSetCursor(const NSCursorInfo& aCursorInfo) override;
=======
  mozilla::ipc::IPCResult RecvSetCursor(const NSCursorInfo& aCursorInfo);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvShowCursor(const bool& aShow) override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult
    RecvShowCursor(const bool& aShow) override;
=======
  mozilla::ipc::IPCResult RecvShowCursor(const bool& aShow);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPushCursor(
      const NSCursorInfo& aCursorInfo) override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult
    RecvPushCursor(const NSCursorInfo& aCursorInfo) override;
=======
  mozilla::ipc::IPCResult RecvPushCursor(const NSCursorInfo& aCursorInfo);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvPopCursor() override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult
    RecvPopCursor() override;
=======
  mozilla::ipc::IPCResult RecvPopCursor();
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNPN_SetException(
      const nsCString& aMessage) override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult
    RecvNPN_SetException(const nsCString& aMessage) override;
=======
  mozilla::ipc::IPCResult RecvNPN_SetException(const nsCString& aMessage);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNPN_ReloadPlugins(
      const bool& aReloadPages) override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult
    RecvNPN_ReloadPlugins(const bool& aReloadPages) override;
=======
  mozilla::ipc::IPCResult RecvNPN_ReloadPlugins(const bool& aReloadPages);
>>>>>>> upstream-releases

  static BrowserStreamParent* StreamCast(NPP instance, NPStream* s);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult
  AnswerNPN_SetValue_NPPVpluginRequiresAudioDeviceChanges(
      const bool& shouldRegister, NPError* result) override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult
    AnswerNPN_SetValue_NPPVpluginRequiresAudioDeviceChanges(
                                        const bool& shouldRegister,
                                        NPError* result) override;
=======
  virtual mozilla::ipc::IPCResult
  AnswerNPN_SetValue_NPPVpluginRequiresAudioDeviceChanges(
      const bool& shouldRegister, NPError* result);
>>>>>>> upstream-releases

 protected:
  void SetChildTimeout(const int32_t aChildTimeout);
  static void TimeoutChanged(const char* aPref, PluginModuleParent* aModule);

  virtual void UpdatePluginTimeout() {}

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyContentModuleDestroyed() override {
    return IPC_OK();
  }
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult RecvNotifyContentModuleDestroyed() override { return IPC_OK(); }
=======
  virtual mozilla::ipc::IPCResult RecvNotifyContentModuleDestroyed() {
    return IPC_OK();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvReturnClearSiteData(
      const NPError& aRv, const uint64_t& aCallbackId) override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult RecvReturnClearSiteData(const NPError& aRv,
                                                            const uint64_t& aCallbackId) override;
=======
  mozilla::ipc::IPCResult RecvReturnClearSiteData(const NPError& aRv,
                                                  const uint64_t& aCallbackId);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvReturnSitesWithData(
      nsTArray<nsCString>&& aSites, const uint64_t& aCallbackId) override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult RecvReturnSitesWithData(nsTArray<nsCString>&& aSites,
                                                            const uint64_t& aCallbackId) override;
=======
  mozilla::ipc::IPCResult RecvReturnSitesWithData(nsTArray<nsCString>&& aSites,
                                                  const uint64_t& aCallbackId);
>>>>>>> upstream-releases

  void SetPluginFuncs(NPPluginFuncs* aFuncs);

  nsresult NPP_NewInternal(NPMIMEType pluginType, NPP instance,
                           InfallibleTArray<nsCString>& names,
                           InfallibleTArray<nsCString>& values,
                           NPSavedData* saved, NPError* error);

  // NPP-like API that Gecko calls are trampolined into.  These
  // messages then get forwarded along to the plugin instance,
  // and then eventually the child process.

  static NPError NPP_Destroy(NPP instance, NPSavedData** save);

  static NPError NPP_SetWindow(NPP instance, NPWindow* window);
  static NPError NPP_NewStream(NPP instance, NPMIMEType type, NPStream* stream,
                               NPBool seekable, uint16_t* stype);
  static NPError NPP_DestroyStream(NPP instance, NPStream* stream,
                                   NPReason reason);
  static int32_t NPP_WriteReady(NPP instance, NPStream* stream);
  static int32_t NPP_Write(NPP instance, NPStream* stream, int32_t offset,
                           int32_t len, void* buffer);
  static void NPP_Print(NPP instance, NPPrint* platformPrint);
  static int16_t NPP_HandleEvent(NPP instance, void* event);
  static void NPP_URLNotify(NPP instance, const char* url, NPReason reason,
                            void* notifyData);
  static NPError NPP_GetValue(NPP instance, NPPVariable variable,
                              void* ret_value);
  static NPError NPP_SetValue(NPP instance, NPNVariable variable, void* value);
  static void NPP_URLRedirectNotify(NPP instance, const char* url,
                                    int32_t status, void* notifyData);

  virtual bool HasRequiredFunctions() override;
  virtual nsresult AsyncSetWindow(NPP aInstance, NPWindow* aWindow) override;
  virtual nsresult GetImageContainer(
      NPP aInstance, mozilla::layers::ImageContainer** aContainer) override;
  virtual nsresult GetImageSize(NPP aInstance, nsIntSize* aSize) override;
  virtual void DidComposite(NPP aInstance) override;
  virtual bool IsOOP() override { return true; }
  virtual nsresult SetBackgroundUnknown(NPP instance) override;
  virtual nsresult BeginUpdateBackground(NPP instance, const nsIntRect& aRect,
                                         DrawTarget** aDrawTarget) override;
  virtual nsresult EndUpdateBackground(NPP instance,
                                       const nsIntRect& aRect) override;

#if defined(XP_WIN)
  virtual nsresult GetScrollCaptureContainer(
      NPP aInstance, mozilla::layers::ImageContainer** aContainer) override;
#endif

  virtual nsresult HandledWindowedPluginKeyEvent(
      NPP aInstance, const mozilla::NativeEventData& aNativeKeyData,
      bool aIsConsumed) override;

#if defined(XP_UNIX) && !defined(XP_MACOSX)
  virtual nsresult NP_Initialize(NPNetscapeFuncs* bFuncs, NPPluginFuncs* pFuncs,
                                 NPError* error) override;
#else
  virtual nsresult NP_Initialize(NPNetscapeFuncs* bFuncs,
                                 NPError* error) override;
#endif
  virtual nsresult NP_Shutdown(NPError* error) override;

  virtual nsresult NP_GetMIMEDescription(const char** mimeDesc) override;
  virtual nsresult NP_GetValue(void* future, NPPVariable aVariable,
                               void* aValue, NPError* error) override;
#if defined(XP_WIN) || defined(XP_MACOSX)
  virtual nsresult NP_GetEntryPoints(NPPluginFuncs* pFuncs,
                                     NPError* error) override;
#endif
  virtual nsresult NPP_New(NPMIMEType pluginType, NPP instance, int16_t argc,
                           char* argn[], char* argv[], NPSavedData* saved,
                           NPError* error) override;
  virtual nsresult NPP_ClearSiteData(
      const char* site, uint64_t flags, uint64_t maxAge,
      nsCOMPtr<nsIClearSiteDataCallback> callback) override;
  virtual nsresult NPP_GetSitesWithData(
      nsCOMPtr<nsIGetSitesWithDataCallback> callback) override;

 private:
  std::map<uint64_t, nsCOMPtr<nsIClearSiteDataCallback>>
      mClearSiteDataCallbacks;
  std::map<uint64_t, nsCOMPtr<nsIGetSitesWithDataCallback>>
      mSitesWithDataCallbacks;

  nsCString mPluginFilename;
  int mQuirks;
  void InitQuirksModes(const nsCString& aMimeType);

 public:
#if defined(XP_MACOSX)
  virtual nsresult IsRemoteDrawingCoreAnimation(NPP instance,
                                                bool* aDrawing) override;
#endif
#if defined(XP_MACOSX) || defined(XP_WIN)
  virtual nsresult ContentsScaleFactorChanged(
      NPP instance, double aContentsScaleFactor) override;
#endif

  layers::TextureClientRecycleAllocator*
  EnsureTextureAllocatorForDirectBitmap();
  layers::TextureClientRecycleAllocator* EnsureTextureAllocatorForDXGISurface();

 protected:
  void NotifyFlashHang();
  void NotifyPluginCrashed();
  void OnInitFailure();
  bool DoShutdown(NPError* error);

  bool GetSetting(NPNVariable aVariable);
  void GetSettings(PluginSettings* aSettings);

  bool mIsChrome;
  bool mShutdown;
  bool mHadLocalInstance;
  bool mClearSiteDataSupported;
  bool mGetSitesWithDataSupported;
  NPNetscapeFuncs* mNPNIface;
  NPPluginFuncs* mNPPIface;
  nsNPAPIPlugin* mPlugin;
  ipc::TaskFactory<PluginModuleParent> mTaskFactory;
  nsString mHangID;
  nsCString mPluginName;
  nsCString mPluginVersion;
  int32_t mSandboxLevel;
  bool mIsFlashPlugin;

#ifdef MOZ_X11
  // Dup of plugin's X socket, used to scope its resources to this
  // object instead of the plugin process's lifetime
  ScopedClose mPluginXSocketFdDup;
#endif

  bool GetPluginDetails();

  uint32_t mRunID;

  RefPtr<layers::TextureClientRecycleAllocator>
      mTextureAllocatorForDirectBitmap;
  RefPtr<layers::TextureClientRecycleAllocator> mTextureAllocatorForDXGISurface;

  /**
   * This mutex protects the crash reporter when the Plugin Hang UI event
   * handler is executing off main thread. It is intended to protect both
   * the mCrashReporter variable in addition to the CrashReporterHost object
   * that mCrashReporter refers to.
   */
  mozilla::Mutex mCrashReporterMutex;
  UniquePtr<ipc::CrashReporterHost> mCrashReporter;
};

class PluginModuleContentParent : public PluginModuleParent {
 public:
  explicit PluginModuleContentParent();

  static PluginLibrary* LoadModule(uint32_t aPluginId, nsPluginTag* aPluginTag);

  virtual ~PluginModuleContentParent();

#if defined(XP_WIN) || defined(XP_MACOSX)
  nsresult NP_Initialize(NPNetscapeFuncs* bFuncs, NPError* error) override;
#endif

 private:
  static void Initialize(Endpoint<PPluginModuleParent>&& aEndpoint);

  virtual bool ShouldContinueFromReplyTimeout() override;
  virtual void OnExitedSyncSend() override;

#ifdef MOZ_CRASHREPORTER_INJECTOR
  void OnCrash(DWORD processID) override {}
#endif

  static PluginModuleContentParent* sSavedModuleParent;

  uint32_t mPluginId;
};

class PluginModuleChromeParent : public PluginModuleParent,
                                 public mozilla::BackgroundHangAnnotator {
  friend class mozilla::ipc::CrashReporterHost;

 public:
  /**
   * LoadModule
   *
   * This may or may not launch a plugin child process,
   * and may or may not be very expensive.
   */
  static PluginLibrary* LoadModule(const char* aFilePath, uint32_t aPluginId,
                                   nsPluginTag* aPluginTag);

  virtual ~PluginModuleChromeParent();

  /*
   * Takes a full multi-process dump including the plugin process and the
   * content process. If aBrowserDumpId is not empty then the browser dump
   * associated with it will be paired to the resulting minidump.
   * Takes ownership of the file associated with aBrowserDumpId.
   *
   * @param aContentPid PID of the e10s content process from which a hang was
   *   reported. May be kInvalidProcessId if not applicable.
   * @param aBrowserDumpId (optional) previously taken browser dump id. If
   *   provided TakeFullMinidump will use this dump file instead of
   *   generating a new one. If not provided a browser dump will be taken at
   *   the time of this call.
   * @param aDumpId Returns the ID of the newly generated crash dump. Left
   *   untouched upon failure.
   */
  void TakeFullMinidump(base::ProcessId aContentPid,
                        const nsAString& aBrowserDumpId, nsString& aDumpId);

  /*
   * Terminates the plugin process associated with this plugin module. Also
   * generates appropriate crash reports unless an existing one is provided.
   * Takes ownership of the file associated with aDumpId on success.
   *
   * @param aMsgLoop the main message pump associated with the module
   *   protocol.
   * @param aContentPid PID of the e10s content process from which a hang was
   *   reported. May be kInvalidProcessId if not applicable.
   * @param aMonitorDescription a string describing the hang monitor that
   *   is making this call. This string is added to the crash reporter
   *   annotations for the plugin process.
   * @param aDumpId (optional) previously taken dump id. If provided
   *   TerminateChildProcess will use this dump file instead of generating a
   *   multi-process crash report. If not provided a multi-process dump will
   *   be taken at the time of this call.
   */
  void TerminateChildProcess(MessageLoop* aMsgLoop, base::ProcessId aContentPid,
                             const nsCString& aMonitorDescription,
                             const nsAString& aDumpId);

#ifdef XP_WIN
  /**
   * Called by Plugin Hang UI to notify that the user has clicked continue.
   * Used for chrome hang annotations.
   */
  void OnHangUIContinue();

  void EvaluateHangUIState(const bool aReset);
#endif  // XP_WIN

  void CachedSettingChanged();

 private:
  virtual void EnteredCxxStack() override;

  void ExitedCxxStack() override;

  mozilla::ipc::IProtocol* GetInvokingProtocol();
  PluginInstanceParent* GetManagingInstance(mozilla::ipc::IProtocol* aProtocol);

  virtual void AnnotateHang(
      mozilla::BackgroundHangAnnotations& aAnnotations) override;

  virtual bool ShouldContinueFromReplyTimeout() override;

<<<<<<< HEAD
  void ProcessFirstMinidump();
  void WriteExtraDataForMinidump();
||||||| merged common ancestors
    void ProcessFirstMinidump();
    void WriteExtraDataForMinidump();
=======
  void ProcessFirstMinidump();
  void AddCrashAnnotations();
>>>>>>> upstream-releases

  PluginProcessParent* Process() const { return mSubprocess; }
  base::ProcessHandle ChildProcessHandle() {
    return mSubprocess->GetChildProcessHandle();
  }

#if defined(XP_UNIX) && !defined(XP_MACOSX)
  virtual nsresult NP_Initialize(NPNetscapeFuncs* bFuncs, NPPluginFuncs* pFuncs,
                                 NPError* error) override;
#else
  virtual nsresult NP_Initialize(NPNetscapeFuncs* bFuncs,
                                 NPError* error) override;
#endif

#if defined(XP_WIN) || defined(XP_MACOSX)
  virtual nsresult NP_GetEntryPoints(NPPluginFuncs* pFuncs,
                                     NPError* error) override;
#endif

  virtual void ActorDestroy(ActorDestroyReason why) override;

  // aFilePath is UTF8, not native!
  explicit PluginModuleChromeParent(const char* aFilePath, uint32_t aPluginId,
                                    int32_t aSandboxLevel);

  void CleanupFromTimeout(const bool aByHangUI);

  virtual void UpdatePluginTimeout() override;

  void RegisterSettingsCallbacks();
  void UnregisterSettingsCallbacks();

  bool InitCrashReporter();

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvNotifyContentModuleDestroyed() override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult RecvNotifyContentModuleDestroyed() override;
=======
  mozilla::ipc::IPCResult RecvNotifyContentModuleDestroyed() override;
>>>>>>> upstream-releases

  static void CachedSettingChanged(const char* aPref,
                                   PluginModuleChromeParent* aModule);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult
  AnswerNPN_SetValue_NPPVpluginRequiresAudioDeviceChanges(
      const bool& shouldRegister, NPError* result) override;
||||||| merged common ancestors
    virtual mozilla::ipc::IPCResult
    AnswerNPN_SetValue_NPPVpluginRequiresAudioDeviceChanges(
                                        const bool& shouldRegister,
                                        NPError* result) override;
=======
  mozilla::ipc::IPCResult
  AnswerNPN_SetValue_NPPVpluginRequiresAudioDeviceChanges(
      const bool& shouldRegister, NPError* result) override;
>>>>>>> upstream-releases

  PluginProcessParent* mSubprocess;
  uint32_t mPluginId;

  ipc::TaskFactory<PluginModuleChromeParent> mChromeTaskFactory;

  enum HangAnnotationFlags {
    kInPluginCall = (1u << 0),
    kHangUIShown = (1u << 1),
    kHangUIContinued = (1u << 2),
    kHangUIDontShow = (1u << 3)
  };
  Atomic<uint32_t> mHangAnnotationFlags;
#ifdef XP_WIN
  InfallibleTArray<float> mPluginCpuUsageOnHang;
  PluginHangUIParent* mHangUIParent;
  bool mHangUIEnabled;
  bool mIsTimerReset;

  /**
   * Launches the Plugin Hang UI.
   *
   * @return true if plugin-hang-ui.exe has been successfully launched.
   *         false if the Plugin Hang UI is disabled, already showing,
   *               or the launch failed.
   */
  bool LaunchHangUI();

  /**
   * Finishes the Plugin Hang UI and cancels if it is being shown to the user.
   */
  void FinishHangUI();

  FunctionBrokerParent* mBrokerParent;
#endif

#ifdef MOZ_CRASHREPORTER_INJECTOR
  friend class mozilla::plugins::FinishInjectorInitTask;

  void InitializeInjector();
  void DoInjection(const nsAutoHandle& aSnapshot);
  static DWORD WINAPI GetToolhelpSnapshot(LPVOID aContext);

  void OnCrash(DWORD processID) override;

  DWORD mFlashProcess1;
  DWORD mFlashProcess2;
  RefPtr<mozilla::plugins::FinishInjectorInitTask> mFinishInitTask;
#endif

  void OnProcessLaunched(const bool aSucceeded);

  class LaunchedTask : public LaunchCompleteTask {
   public:
    explicit LaunchedTask(PluginModuleChromeParent* aModule)
        : mModule(aModule) {
      MOZ_ASSERT(aModule);
    }

    NS_IMETHOD Run() override {
      mModule->OnProcessLaunched(mLaunchSucceeded);
      return NS_OK;
    }

   private:
    PluginModuleChromeParent* mModule;
  };

  friend class LaunchedTask;

  nsCOMPtr<nsIObserver> mPluginOfflineObserver;
  bool mIsBlocklisted;
  bool mIsCleaningFromTimeout;
};

}  // namespace plugins
}  // namespace mozilla

#endif  // mozilla_plugins_PluginModuleParent_h
