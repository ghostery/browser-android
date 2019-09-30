/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/basictypes.h"

/* This must occur *after* layers/PLayerTransaction.h to avoid typedefs
 * conflicts. */
#include "mozilla/ArrayUtils.h"

#include "pratom.h"
#include "prenv.h"

#include "jsfriendapi.h"

#include "nsPluginHost.h"
#include "nsNPAPIPlugin.h"
#include "nsNPAPIPluginInstance.h"
#include "nsNPAPIPluginStreamListener.h"
#include "nsPluginStreamListenerPeer.h"
#include "nsIServiceManager.h"
#include "nsThreadUtils.h"
#include "mozilla/CycleCollectedJSContext.h"  // for nsAutoMicroTask
#include "mozilla/Preferences.h"
#include "nsPluginInstanceOwner.h"

#include "nsPluginsDir.h"
#include "nsPluginLogging.h"

#include "nsPIDOMWindow.h"
#include "nsGlobalWindow.h"
#include "mozilla/dom/Document.h"
#include "nsIContent.h"
#include "nsIIDNService.h"
#include "nsIScriptGlobalObject.h"
#include "nsIScriptContext.h"
#include "nsDOMJSUtils.h"
#include "nsIPrincipal.h"
#include "nsWildCard.h"
#include "nsContentUtils.h"
#include "mozilla/dom/Element.h"
#include "mozilla/dom/ScriptSettings.h"
#include "mozilla/dom/ToJSValue.h"
#include "nsIXULRuntime.h"
#include "nsIXPConnect.h"

#include "nsIObserverService.h"
#include <prinrval.h>

#ifdef MOZ_WIDGET_COCOA
#  include <Carbon/Carbon.h>
#  include <ApplicationServices/ApplicationServices.h>
#  include <OpenGL/OpenGL.h>
#  include "nsCocoaFeatures.h"
#  include "PluginUtilsOSX.h"
#endif

// needed for nppdf plugin
#if (MOZ_WIDGET_GTK)
#  include <gdk/gdk.h>
#  include <gdk/gdkx.h>
#endif

#include "nsJSUtils.h"
#include "nsJSNPRuntime.h"
#include "nsIHttpAuthManager.h"
#include "nsICookieService.h"
#include "nsILoadContext.h"
#include "nsIDocShell.h"

#include "nsNetUtil.h"
#include "nsNetCID.h"

#include "mozilla/Mutex.h"
#include "mozilla/PluginLibrary.h"
using mozilla::PluginLibrary;

#include "mozilla/plugins/PluginModuleParent.h"
using mozilla::plugins::PluginModuleChromeParent;
using mozilla::plugins::PluginModuleContentParent;

#ifdef MOZ_X11
#  include "mozilla/X11Util.h"
#endif

#ifdef XP_WIN
#  include <windows.h>
#  include "mozilla/WindowsVersion.h"
#  ifdef ACCESSIBILITY
#    include "mozilla/a11y/Compatibility.h"
#  endif
#endif

#include "nsIAudioChannelAgent.h"
#include "AudioChannelService.h"

using namespace mozilla;
using namespace mozilla::plugins::parent;
using mozilla::dom::Document;

// We should make this const...
static NPNetscapeFuncs sBrowserFuncs = {
    sizeof(sBrowserFuncs),
    (NP_VERSION_MAJOR << 8) + NP_VERSION_MINOR,
    _geturl,
    _posturl,
    _requestread,
    nullptr,  // _newstream, unimplemented
    nullptr,  // _write, unimplemented
    nullptr,  // _destroystream, unimplemented
    _status,
    _useragent,
    _memalloc,
    _memfree,
    _memflush,
    _reloadplugins,
    _getJavaEnv,
    _getJavaPeer,
    _geturlnotify,
    _posturlnotify,
    _getvalue,
    _setvalue,
    _invalidaterect,
    _invalidateregion,
    _forceredraw,
    _getstringidentifier,
    _getstringidentifiers,
    _getintidentifier,
    _identifierisstring,
    _utf8fromidentifier,
    _intfromidentifier,
    _createobject,
    _retainobject,
    _releaseobject,
    _invoke,
    _invokeDefault,
    _evaluate,
    _getproperty,
    _setproperty,
    _removeproperty,
    _hasproperty,
    _hasmethod,
    _releasevariantvalue,
    _setexception,
    _pushpopupsenabledstate,
    _poppopupsenabledstate,
    _enumerate,
    nullptr,  // pluginthreadasynccall, not used
    _construct,
    _getvalueforurl,
    _setvalueforurl,
    nullptr,  // NPN GetAuthenticationInfo, not supported
    _scheduletimer,
    _unscheduletimer,
    _popupcontextmenu,
    _convertpoint,
    nullptr,  // handleevent, unimplemented
    nullptr,  // unfocusinstance, unimplemented
    _urlredirectresponse,
    _initasyncsurface,
    _finalizeasyncsurface,
    _setcurrentasyncsurface};

// POST/GET stream type
enum eNPPStreamTypeInternal {
  eNPPStreamTypeInternal_Get,
  eNPPStreamTypeInternal_Post
};

void NS_NotifyBeginPluginCall(NSPluginCallReentry aReentryState) {
  nsNPAPIPluginInstance::BeginPluginCall(aReentryState);
}

void NS_NotifyPluginCall(NSPluginCallReentry aReentryState) {
  nsNPAPIPluginInstance::EndPluginCall(aReentryState);
}

<<<<<<< HEAD
nsNPAPIPlugin::nsNPAPIPlugin() {
  memset((void *)&mPluginFuncs, 0, sizeof(mPluginFuncs));
||||||| merged common ancestors
nsNPAPIPlugin::nsNPAPIPlugin()
{
  memset((void*)&mPluginFuncs, 0, sizeof(mPluginFuncs));
=======
nsNPAPIPlugin::nsNPAPIPlugin() {
  memset((void*)&mPluginFuncs, 0, sizeof(mPluginFuncs));
>>>>>>> upstream-releases
  mPluginFuncs.size = sizeof(mPluginFuncs);
  mPluginFuncs.version = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;

  mLibrary = nullptr;
}

nsNPAPIPlugin::~nsNPAPIPlugin() {
  delete mLibrary;
  mLibrary = nullptr;
}

<<<<<<< HEAD
void nsNPAPIPlugin::PluginCrashed(const nsAString &pluginDumpID,
                                  const nsAString &browserDumpID) {
||||||| merged common ancestors
void
nsNPAPIPlugin::PluginCrashed(const nsAString& pluginDumpID,
                             const nsAString& browserDumpID)
{
=======
void nsNPAPIPlugin::PluginCrashed(const nsAString& pluginDumpID,
                                  const nsAString& browserDumpID) {
>>>>>>> upstream-releases
  RefPtr<nsPluginHost> host = nsPluginHost::GetInst();
  host->PluginCrashed(this, pluginDumpID, browserDumpID);
}

<<<<<<< HEAD
inline PluginLibrary *GetNewPluginLibrary(nsPluginTag *aPluginTag) {
||||||| merged common ancestors
inline PluginLibrary*
GetNewPluginLibrary(nsPluginTag *aPluginTag)
{
=======
inline PluginLibrary* GetNewPluginLibrary(nsPluginTag* aPluginTag) {
>>>>>>> upstream-releases
  AUTO_PROFILER_LABEL("GetNewPluginLibrary", OTHER);

  if (!aPluginTag) {
    return nullptr;
  }

  if (XRE_IsContentProcess()) {
    return PluginModuleContentParent::LoadModule(aPluginTag->mId, aPluginTag);
  }

  return PluginModuleChromeParent::LoadModule(aPluginTag->mFullPath.get(),
                                              aPluginTag->mId, aPluginTag);
}

<<<<<<< HEAD
// Creates an nsNPAPIPlugin object. One nsNPAPIPlugin object exists per plugin
// (not instance).
nsresult nsNPAPIPlugin::CreatePlugin(nsPluginTag *aPluginTag,
                                     nsNPAPIPlugin **aResult) {
||||||| merged common ancestors
// Creates an nsNPAPIPlugin object. One nsNPAPIPlugin object exists per plugin (not instance).
nsresult
nsNPAPIPlugin::CreatePlugin(nsPluginTag *aPluginTag, nsNPAPIPlugin** aResult)
{
=======
// Creates an nsNPAPIPlugin object. One nsNPAPIPlugin object exists per plugin
// (not instance).
nsresult nsNPAPIPlugin::CreatePlugin(nsPluginTag* aPluginTag,
                                     nsNPAPIPlugin** aResult) {
>>>>>>> upstream-releases
  AUTO_PROFILER_LABEL("nsNPAPIPlugin::CreatePlugin", OTHER);
  *aResult = nullptr;

  if (!aPluginTag) {
    return NS_ERROR_FAILURE;
  }

  RefPtr<nsNPAPIPlugin> plugin = new nsNPAPIPlugin();

  PluginLibrary *pluginLib = GetNewPluginLibrary(aPluginTag);
  if (!pluginLib) {
    return NS_ERROR_FAILURE;
  }

#if defined(XP_MACOSX)
  if (!pluginLib->HasRequiredFunctions()) {
    NS_WARNING(
        "Not all necessary functions exposed by plugin, it will not load.");
    delete pluginLib;
    return NS_ERROR_FAILURE;
  }
#endif

  plugin->mLibrary = pluginLib;
  pluginLib->SetPlugin(plugin);

// Exchange NPAPI entry points.
#if defined(XP_WIN)
  // NP_GetEntryPoints must be called before NP_Initialize on Windows.
  NPError pluginCallError;
  nsresult rv =
      pluginLib->NP_GetEntryPoints(&plugin->mPluginFuncs, &pluginCallError);
  if (rv != NS_OK || pluginCallError != NPERR_NO_ERROR) {
    return NS_ERROR_FAILURE;
  }

  // NP_Initialize must be called after NP_GetEntryPoints on Windows.
  rv = pluginLib->NP_Initialize(&sBrowserFuncs, &pluginCallError);
  if (rv != NS_OK || pluginCallError != NPERR_NO_ERROR) {
    return NS_ERROR_FAILURE;
  }
#elif defined(XP_MACOSX)
  // NP_Initialize must be called before NP_GetEntryPoints on Mac OS X.
  // We need to match WebKit's behavior.
  NPError pluginCallError;
  nsresult rv = pluginLib->NP_Initialize(&sBrowserFuncs, &pluginCallError);
  if (rv != NS_OK || pluginCallError != NPERR_NO_ERROR) {
    return NS_ERROR_FAILURE;
  }

  rv = pluginLib->NP_GetEntryPoints(&plugin->mPluginFuncs, &pluginCallError);
  if (rv != NS_OK || pluginCallError != NPERR_NO_ERROR) {
    return NS_ERROR_FAILURE;
  }
#else
  NPError pluginCallError;
  nsresult rv = pluginLib->NP_Initialize(&sBrowserFuncs, &plugin->mPluginFuncs,
                                         &pluginCallError);
  if (rv != NS_OK || pluginCallError != NPERR_NO_ERROR) {
    return NS_ERROR_FAILURE;
  }
#endif

  plugin.forget(aResult);
  return NS_OK;
}

<<<<<<< HEAD
PluginLibrary *nsNPAPIPlugin::GetLibrary() { return mLibrary; }
||||||| merged common ancestors
PluginLibrary*
nsNPAPIPlugin::GetLibrary()
{
  return mLibrary;
}
=======
PluginLibrary* nsNPAPIPlugin::GetLibrary() { return mLibrary; }
>>>>>>> upstream-releases

<<<<<<< HEAD
NPPluginFuncs *nsNPAPIPlugin::PluginFuncs() { return &mPluginFuncs; }
||||||| merged common ancestors
NPPluginFuncs*
nsNPAPIPlugin::PluginFuncs()
{
  return &mPluginFuncs;
}
=======
NPPluginFuncs* nsNPAPIPlugin::PluginFuncs() { return &mPluginFuncs; }
>>>>>>> upstream-releases

nsresult nsNPAPIPlugin::Shutdown() {
  NPP_PLUGIN_LOG(PLUGIN_LOG_BASIC,
                 ("NPP Shutdown to be called: this=%p\n", this));

  NPError shutdownError;
  mLibrary->NP_Shutdown(&shutdownError);

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsNPAPIPlugin::RetainStream(NPStream *pstream,
                                     nsISupports **aRetainedPeer) {
  if (!aRetainedPeer) return NS_ERROR_NULL_POINTER;
||||||| merged common ancestors
nsresult
nsNPAPIPlugin::RetainStream(NPStream *pstream, nsISupports **aRetainedPeer)
{
  if (!aRetainedPeer)
    return NS_ERROR_NULL_POINTER;
=======
nsresult nsNPAPIPlugin::RetainStream(NPStream* pstream,
                                     nsISupports** aRetainedPeer) {
  if (!aRetainedPeer) return NS_ERROR_NULL_POINTER;
>>>>>>> upstream-releases

  *aRetainedPeer = nullptr;

  if (!pstream || !pstream->ndata) return NS_ERROR_NULL_POINTER;

<<<<<<< HEAD
  nsNPAPIStreamWrapper *streamWrapper =
      static_cast<nsNPAPIStreamWrapper *>(pstream->ndata);
  nsNPAPIPluginStreamListener *listener = streamWrapper->GetStreamListener();
||||||| merged common ancestors
  nsNPAPIStreamWrapper* streamWrapper = static_cast<nsNPAPIStreamWrapper*>(pstream->ndata);
  nsNPAPIPluginStreamListener* listener = streamWrapper->GetStreamListener();
=======
  nsNPAPIStreamWrapper* streamWrapper =
      static_cast<nsNPAPIStreamWrapper*>(pstream->ndata);
  nsNPAPIPluginStreamListener* listener = streamWrapper->GetStreamListener();
>>>>>>> upstream-releases
  if (!listener) {
    return NS_ERROR_NULL_POINTER;
  }

  nsIStreamListener *streamListener = listener->GetStreamListenerPeer();
  if (!streamListener) {
    return NS_ERROR_NULL_POINTER;
  }

  *aRetainedPeer = streamListener;
  NS_ADDREF(*aRetainedPeer);
  return NS_OK;
}

// Create a new NPP GET or POST (given in the type argument) url
// stream that may have a notify callback
<<<<<<< HEAD
NPError MakeNewNPAPIStreamInternal(NPP npp, const char *relativeURL,
                                   const char *target,
                                   eNPPStreamTypeInternal type,
                                   bool bDoNotify = false,
                                   void *notifyData = nullptr, uint32_t len = 0,
                                   const char *buf = nullptr) {
  if (!npp) return NPERR_INVALID_INSTANCE_ERROR;
||||||| merged common ancestors
NPError
MakeNewNPAPIStreamInternal(NPP npp, const char *relativeURL, const char *target,
                          eNPPStreamTypeInternal type,
                          bool bDoNotify = false,
                          void *notifyData = nullptr, uint32_t len = 0,
                          const char *buf = nullptr)
{
  if (!npp)
    return NPERR_INVALID_INSTANCE_ERROR;
=======
NPError MakeNewNPAPIStreamInternal(NPP npp, const char* relativeURL,
                                   const char* target,
                                   eNPPStreamTypeInternal type,
                                   bool bDoNotify = false,
                                   void* notifyData = nullptr, uint32_t len = 0,
                                   const char* buf = nullptr) {
  if (!npp) return NPERR_INVALID_INSTANCE_ERROR;
>>>>>>> upstream-releases

  PluginDestructionGuard guard(npp);

<<<<<<< HEAD
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)npp->ndata;
  if (!inst || !inst->IsRunning()) return NPERR_INVALID_INSTANCE_ERROR;
||||||| merged common ancestors
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *) npp->ndata;
  if (!inst || !inst->IsRunning())
    return NPERR_INVALID_INSTANCE_ERROR;
=======
  nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)npp->ndata;
  if (!inst || !inst->IsRunning()) return NPERR_INVALID_INSTANCE_ERROR;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIPluginHost> pluginHostCOM =
      do_GetService(MOZ_PLUGIN_HOST_CONTRACTID);
  nsPluginHost *pluginHost = static_cast<nsPluginHost *>(pluginHostCOM.get());
||||||| merged common ancestors
  nsCOMPtr<nsIPluginHost> pluginHostCOM = do_GetService(MOZ_PLUGIN_HOST_CONTRACTID);
  nsPluginHost *pluginHost = static_cast<nsPluginHost*>(pluginHostCOM.get());
=======
  nsCOMPtr<nsIPluginHost> pluginHostCOM =
      do_GetService(MOZ_PLUGIN_HOST_CONTRACTID);
  nsPluginHost* pluginHost = static_cast<nsPluginHost*>(pluginHostCOM.get());
>>>>>>> upstream-releases
  if (!pluginHost) {
    return NPERR_GENERIC_ERROR;
  }

  RefPtr<nsNPAPIPluginStreamListener> listener;
  // Set aCallNotify here to false.  If pluginHost->GetURL or PostURL fail,
  // the listener's destructor will do the notification while we are about to
  // return a failure code.
  // Call SetCallNotify(true) below after we are sure we cannot return a failure
  // code.
  if (!target) {
    inst->NewStreamListener(relativeURL, notifyData, getter_AddRefs(listener));
    if (listener) {
      listener->SetCallNotify(false);
    }
  }

  switch (type) {
    case eNPPStreamTypeInternal_Get: {
      if (NS_FAILED(pluginHost->GetURL(inst, relativeURL, target, listener,
                                       nullptr, nullptr, false)))
        return NPERR_GENERIC_ERROR;
      break;
    }
    case eNPPStreamTypeInternal_Post: {
      if (NS_FAILED(pluginHost->PostURL(inst, relativeURL, len, buf, target,
                                        listener, nullptr, nullptr, false, 0,
                                        nullptr)))
        return NPERR_GENERIC_ERROR;
      break;
    }
    default:
      NS_ERROR("how'd I get here");
  }

  if (listener) {
    // SetCallNotify(bDoNotify) here, see comment above.
    listener->SetCallNotify(bDoNotify);
  }

  return NPERR_NO_ERROR;
}

#if defined(MOZ_MEMORY) && defined(XP_WIN)
extern "C" size_t malloc_usable_size(const void* ptr);
#endif

namespace {

static char* gNPPException;

<<<<<<< HEAD
static nsIDocument *GetDocumentFromNPP(NPP npp) {
||||||| merged common ancestors
static nsIDocument *
GetDocumentFromNPP(NPP npp)
{
=======
static Document* GetDocumentFromNPP(NPP npp) {
>>>>>>> upstream-releases
  NS_ENSURE_TRUE(npp, nullptr);

  nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)npp->ndata;
  NS_ENSURE_TRUE(inst, nullptr);

  PluginDestructionGuard guard(inst);

  RefPtr<nsPluginInstanceOwner> owner = inst->GetOwner();
  NS_ENSURE_TRUE(owner, nullptr);

  nsCOMPtr<Document> doc;
  owner->GetDocument(getter_AddRefs(doc));

  return doc;
}

<<<<<<< HEAD
static NPIdentifier doGetIdentifier(JSContext *cx, const NPUTF8 *name) {
||||||| merged common ancestors
static NPIdentifier
doGetIdentifier(JSContext *cx, const NPUTF8* name)
{
=======
static NPIdentifier doGetIdentifier(JSContext* cx, const NPUTF8* name) {
>>>>>>> upstream-releases
  NS_ConvertUTF8toUTF16 utf16name(name);

<<<<<<< HEAD
  JSString *str =
      ::JS_AtomizeAndPinUCStringN(cx, utf16name.get(), utf16name.Length());
||||||| merged common ancestors
  JSString *str = ::JS_AtomizeAndPinUCStringN(cx, utf16name.get(), utf16name.Length());
=======
  JSString* str =
      ::JS_AtomizeAndPinUCStringN(cx, utf16name.get(), utf16name.Length());
>>>>>>> upstream-releases

  if (!str) return nullptr;

  return StringToNPIdentifier(cx, str);
}

#if defined(MOZ_MEMORY) && defined(XP_WIN)
BOOL InHeap(HANDLE hHeap, LPVOID lpMem) {
  BOOL success = FALSE;
  PROCESS_HEAP_ENTRY he;
  he.lpData = nullptr;
  while (HeapWalk(hHeap, &he) != 0) {
    if (he.lpData == lpMem) {
      success = TRUE;
      break;
    }
  }
  HeapUnlock(hHeap);
  return success;
}
#endif

} /* anonymous namespace */

NPPExceptionAutoHolder::NPPExceptionAutoHolder()
    : mOldException(gNPPException) {
  gNPPException = nullptr;
}

NPPExceptionAutoHolder::~NPPExceptionAutoHolder() {
  NS_ASSERTION(!gNPPException, "NPP exception not properly cleared!");

  gNPPException = mOldException;
}

NPP NPPStack::sCurrentNPP = nullptr;

<<<<<<< HEAD
const char *PeekException() { return gNPPException; }
||||||| merged common ancestors
const char *
PeekException()
{
  return gNPPException;
}
=======
const char* PeekException() { return gNPPException; }
>>>>>>> upstream-releases

void PopException() {
  NS_ASSERTION(gNPPException, "Uh, no NPP exception to pop!");

  if (gNPPException) {
    free(gNPPException);

    gNPPException = nullptr;
  }
}

//
// Static callbacks that get routed back through the new C++ API
//

namespace mozilla {
namespace plugins {
namespace parent {

<<<<<<< HEAD
NPError _geturl(NPP npp, const char *relativeURL, const char *target) {
||||||| merged common ancestors
NPError
_geturl(NPP npp, const char* relativeURL, const char* target)
{
=======
NPError _geturl(NPP npp, const char* relativeURL, const char* target) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_geturl called from the wrong thread\n"));
    return NPERR_INVALID_PARAM;
  }

<<<<<<< HEAD
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL, ("NPN_GetURL: npp=%p, target=%s, url=%s\n",
                                     (void *)npp, target, relativeURL));
||||||| merged common ancestors
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL,
  ("NPN_GetURL: npp=%p, target=%s, url=%s\n", (void *)npp, target,
   relativeURL));
=======
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL, ("NPN_GetURL: npp=%p, target=%s, url=%s\n",
                                     (void*)npp, target, relativeURL));
>>>>>>> upstream-releases

  PluginDestructionGuard guard(npp);

  return MakeNewNPAPIStreamInternal(npp, relativeURL, target,
                                    eNPPStreamTypeInternal_Get);
}

<<<<<<< HEAD
NPError _geturlnotify(NPP npp, const char *relativeURL, const char *target,
                      void *notifyData) {
||||||| merged common ancestors
NPError
_geturlnotify(NPP npp, const char* relativeURL, const char* target,
              void* notifyData)
{
=======
NPError _geturlnotify(NPP npp, const char* relativeURL, const char* target,
                      void* notifyData) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_geturlnotify called from the wrong thread\n"));
    return NPERR_INVALID_PARAM;
  }

  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL,
<<<<<<< HEAD
                 ("NPN_GetURLNotify: npp=%p, target=%s, notify=%p, url=%s\n",
                  (void *)npp, target, notifyData, relativeURL));
||||||| merged common ancestors
    ("NPN_GetURLNotify: npp=%p, target=%s, notify=%p, url=%s\n", (void*)npp,
     target, notifyData, relativeURL));
=======
                 ("NPN_GetURLNotify: npp=%p, target=%s, notify=%p, url=%s\n",
                  (void*)npp, target, notifyData, relativeURL));
>>>>>>> upstream-releases

  PluginDestructionGuard guard(npp);

  return MakeNewNPAPIStreamInternal(
      npp, relativeURL, target, eNPPStreamTypeInternal_Get, true, notifyData);
}

<<<<<<< HEAD
NPError _posturlnotify(NPP npp, const char *relativeURL, const char *target,
                       uint32_t len, const char *buf, NPBool file,
                       void *notifyData) {
||||||| merged common ancestors
NPError
_posturlnotify(NPP npp, const char *relativeURL, const char *target,
               uint32_t len, const char *buf, NPBool file, void *notifyData)
{
=======
NPError _posturlnotify(NPP npp, const char* relativeURL, const char* target,
                       uint32_t len, const char* buf, NPBool file,
                       void* notifyData) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_posturlnotify called from the wrong thread\n"));
    return NPERR_INVALID_PARAM;
  }
  if (!buf) return NPERR_INVALID_PARAM;

<<<<<<< HEAD
  NPN_PLUGIN_LOG(
      PLUGIN_LOG_NORMAL,
      ("NPN_PostURLNotify: npp=%p, target=%s, len=%d, file=%d, "
       "notify=%p, url=%s, buf=%s\n",
       (void *)npp, target, len, file, notifyData, relativeURL, buf));
||||||| merged common ancestors
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL,
                 ("NPN_PostURLNotify: npp=%p, target=%s, len=%d, file=%d, "
                  "notify=%p, url=%s, buf=%s\n",
                  (void*)npp, target, len, file, notifyData, relativeURL,
                  buf));
=======
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL,
                 ("NPN_PostURLNotify: npp=%p, target=%s, len=%d, file=%d, "
                  "notify=%p, url=%s, buf=%s\n",
                  (void*)npp, target, len, file, notifyData, relativeURL, buf));
>>>>>>> upstream-releases

  PluginDestructionGuard guard(npp);

  return MakeNewNPAPIStreamInternal(npp, relativeURL, target,
                                    eNPPStreamTypeInternal_Post, true,
                                    notifyData, len, buf);
}

<<<<<<< HEAD
NPError _posturl(NPP npp, const char *relativeURL, const char *target,
                 uint32_t len, const char *buf, NPBool file) {
||||||| merged common ancestors
NPError
_posturl(NPP npp, const char *relativeURL, const char *target,
         uint32_t len, const char *buf, NPBool file)
{
=======
NPError _posturl(NPP npp, const char* relativeURL, const char* target,
                 uint32_t len, const char* buf, NPBool file) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_posturl called from the wrong thread\n"));
    return NPERR_INVALID_PARAM;
  }
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL,
                 ("NPN_PostURL: npp=%p, target=%s, file=%d, len=%d, url=%s, "
                  "buf=%s\n",
                  (void *)npp, target, file, len, relativeURL, buf));

  PluginDestructionGuard guard(npp);

  return MakeNewNPAPIStreamInternal(npp, relativeURL, target,
                                    eNPPStreamTypeInternal_Post, false, nullptr,
                                    len, buf);
}

<<<<<<< HEAD
void _status(NPP npp, const char *message) {
||||||| merged common ancestors
void
_status(NPP npp, const char *message)
{
=======
void _status(NPP npp, const char* message) {
>>>>>>> upstream-releases
  // NPN_Status is no longer supported.
}

<<<<<<< HEAD
void _memfree(void *ptr) {
||||||| merged common ancestors
void
_memfree (void *ptr)
{
=======
void _memfree(void* ptr) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_memfree called from the wrong thread\n"));
  }
  NPN_PLUGIN_LOG(PLUGIN_LOG_NOISY, ("NPN_MemFree: ptr=%p\n", ptr));

  if (ptr) free(ptr);
}

uint32_t _memflush(uint32_t size) {
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_memflush called from the wrong thread\n"));
  }
  NPN_PLUGIN_LOG(PLUGIN_LOG_NOISY, ("NPN_MemFlush: size=%d\n", size));

  nsMemory::HeapMinimize(true);
  return 0;
}

void _reloadplugins(NPBool reloadPages) {
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_reloadplugins called from the wrong thread\n"));
    return;
  }
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL,
                 ("NPN_ReloadPlugins: reloadPages=%d\n", reloadPages));

  nsCOMPtr<nsIPluginHost> pluginHost(do_GetService(MOZ_PLUGIN_HOST_CONTRACTID));
  if (!pluginHost) return;

  pluginHost->ReloadPlugins();
}

<<<<<<< HEAD
void _invalidaterect(NPP npp, NPRect *invalidRect) {
||||||| merged common ancestors
void
_invalidaterect(NPP npp, NPRect *invalidRect)
{
=======
void _invalidaterect(NPP npp, NPRect* invalidRect) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_invalidaterect called from the wrong thread\n"));
    return;
  }
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL,
                 ("NPN_InvalidateRect: npp=%p, top=%d, left=%d, bottom=%d, "
<<<<<<< HEAD
                  "right=%d\n",
                  (void *)npp, invalidRect->top, invalidRect->left,
                  invalidRect->bottom, invalidRect->right));
||||||| merged common ancestors
                  "right=%d\n", (void *)npp, invalidRect->top,
                  invalidRect->left, invalidRect->bottom, invalidRect->right));
=======
                  "right=%d\n",
                  (void*)npp, invalidRect->top, invalidRect->left,
                  invalidRect->bottom, invalidRect->right));
>>>>>>> upstream-releases

  if (!npp || !npp->ndata) {
    NS_WARNING("_invalidaterect: npp or npp->ndata == 0");
    return;
  }

<<<<<<< HEAD
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)npp->ndata;
||||||| merged common ancestors
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance*)npp->ndata;
=======
  nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)npp->ndata;
>>>>>>> upstream-releases

  PluginDestructionGuard guard(inst);

  inst->InvalidateRect((NPRect*)invalidRect);
}

void _invalidateregion(NPP npp, NPRegion invalidRegion) {
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_invalidateregion called from the wrong thread\n"));
    return;
  }
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL,
                 ("NPN_InvalidateRegion: npp=%p, region=%p\n", (void *)npp,
                  (void *)invalidRegion));

  if (!npp || !npp->ndata) {
    NS_WARNING("_invalidateregion: npp or npp->ndata == 0");
    return;
  }

<<<<<<< HEAD
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)npp->ndata;
||||||| merged common ancestors
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance*)npp->ndata;
=======
  nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)npp->ndata;
>>>>>>> upstream-releases

  PluginDestructionGuard guard(inst);

  inst->InvalidateRegion((NPRegion)invalidRegion);
}

void _forceredraw(NPP npp) {}

<<<<<<< HEAD
NPObject *_getwindowobject(NPP npp) {
||||||| merged common ancestors
NPObject*
_getwindowobject(NPP npp)
{
=======
NPObject* _getwindowobject(NPP npp) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_getwindowobject called from the wrong thread\n"));
    return nullptr;
  }

  // The window want to return here is the outer window, *not* the inner (since
  // we don't know what the plugin will do with it).
<<<<<<< HEAD
  nsIDocument *doc = GetDocumentFromNPP(npp);
||||||| merged common ancestors
  nsIDocument* doc = GetDocumentFromNPP(npp);
=======
  Document* doc = GetDocumentFromNPP(npp);
>>>>>>> upstream-releases
  NS_ENSURE_TRUE(doc, nullptr);
  nsCOMPtr<nsPIDOMWindowOuter> outer = doc->GetWindow();
  NS_ENSURE_TRUE(outer, nullptr);

<<<<<<< HEAD
  JS::Rooted<JSObject *> windowProxy(
      dom::RootingCx(), nsGlobalWindowOuter::Cast(outer)->GetGlobalJSObject());
  JS::Rooted<JSObject *> global(dom::RootingCx(),
                                JS::GetNonCCWObjectGlobal(windowProxy));
||||||| merged common ancestors
  JS::Rooted<JSObject*> windowProxy(dom::RootingCx(),
                                    nsGlobalWindowOuter::Cast(outer)->GetGlobalJSObject());
  JS::Rooted<JSObject*> global(dom::RootingCx(),
                               JS::GetNonCCWObjectGlobal(windowProxy));
=======
  JS::Rooted<JSObject*> windowProxy(
      dom::RootingCx(), nsGlobalWindowOuter::Cast(outer)->GetGlobalJSObject());
  JS::Rooted<JSObject*> global(dom::RootingCx(),
                               JS::GetNonCCWObjectGlobal(windowProxy));
>>>>>>> upstream-releases
  return nsJSObjWrapper::GetNewOrUsed(npp, windowProxy, global);
}

<<<<<<< HEAD
NPObject *_getpluginelement(NPP npp) {
||||||| merged common ancestors
NPObject*
_getpluginelement(NPP npp)
{
=======
NPObject* _getpluginelement(NPP npp) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_getpluginelement called from the wrong thread\n"));
    return nullptr;
  }

<<<<<<< HEAD
  nsNPAPIPluginInstance *inst =
      static_cast<nsNPAPIPluginInstance *>(npp->ndata);
  if (!inst) return nullptr;
||||||| merged common ancestors
  nsNPAPIPluginInstance* inst = static_cast<nsNPAPIPluginInstance*>(npp->ndata);
  if (!inst)
    return nullptr;
=======
  nsNPAPIPluginInstance* inst = static_cast<nsNPAPIPluginInstance*>(npp->ndata);
  if (!inst) return nullptr;
>>>>>>> upstream-releases

  RefPtr<dom::Element> element;
  inst->GetDOMElement(getter_AddRefs(element));

  if (!element) return nullptr;

  Document* doc = GetDocumentFromNPP(npp);
  if (NS_WARN_IF(!doc)) {
    return nullptr;
  }

  dom::AutoJSAPI jsapi;
  if (NS_WARN_IF(!jsapi.Init(doc->GetInnerWindow()))) {
    return nullptr;
  }
  JSContext *cx = jsapi.cx();

  nsCOMPtr<nsIXPConnect> xpc(nsIXPConnect::XPConnect());
  NS_ENSURE_TRUE(xpc, nullptr);

  JS::RootedValue val(cx);
  if (!ToJSValue(cx, element, &val)) {
    return nullptr;
  }

  if (NS_WARN_IF(!val.isObject())) {
    return nullptr;
  }

  JS::RootedObject obj(cx, &val.toObject());
  JS::RootedObject global(cx, JS::CurrentGlobalOrNull(cx));
  return nsJSObjWrapper::GetNewOrUsed(npp, obj, global);
}

<<<<<<< HEAD
NPIdentifier _getstringidentifier(const NPUTF8 *name) {
||||||| merged common ancestors
NPIdentifier
_getstringidentifier(const NPUTF8* name)
{
=======
NPIdentifier _getstringidentifier(const NPUTF8* name) {
>>>>>>> upstream-releases
  if (!name) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_getstringidentifier: passed null name"));
    return nullptr;
  }
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_getstringidentifier called from the wrong thread\n"));
  }

  AutoSafeJSContext cx;
  return doGetIdentifier(cx, name);
}

<<<<<<< HEAD
void _getstringidentifiers(const NPUTF8 **names, int32_t nameCount,
                           NPIdentifier *identifiers) {
||||||| merged common ancestors
void
_getstringidentifiers(const NPUTF8** names, int32_t nameCount,
                      NPIdentifier *identifiers)
{
=======
void _getstringidentifiers(const NPUTF8** names, int32_t nameCount,
                           NPIdentifier* identifiers) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_getstringidentifiers called from the wrong thread\n"));
  }

  AutoSafeJSContext cx;

  for (int32_t i = 0; i < nameCount; ++i) {
    if (names[i]) {
      identifiers[i] = doGetIdentifier(cx, names[i]);
    } else {
      NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                     ("NPN_getstringidentifiers: passed null name"));
      identifiers[i] = nullptr;
    }
  }
}

NPIdentifier _getintidentifier(int32_t intid) {
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_getstringidentifier called from the wrong thread\n"));
  }
  return IntToNPIdentifier(intid);
}

<<<<<<< HEAD
NPUTF8 *_utf8fromidentifier(NPIdentifier id) {
||||||| merged common ancestors
NPUTF8*
_utf8fromidentifier(NPIdentifier id)
{
=======
NPUTF8* _utf8fromidentifier(NPIdentifier id) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_utf8fromidentifier called from the wrong thread\n"));
  }
  if (!id) return nullptr;

  if (!NPIdentifierIsString(id)) {
    return nullptr;
  }

  JSString* str = NPIdentifierToString(id);
  nsAutoString autoStr;
  AssignJSFlatString(autoStr, JS_ASSERT_STRING_IS_FLAT(str));

  return ToNewUTF8String(autoStr);
}

int32_t _intfromidentifier(NPIdentifier id) {
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_intfromidentifier called from the wrong thread\n"));
  }

  if (!NPIdentifierIsInt(id)) {
    return INT32_MIN;
  }

  return NPIdentifierToInt(id);
}

bool _identifierisstring(NPIdentifier id) {
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_identifierisstring called from the wrong thread\n"));
  }

  return NPIdentifierIsString(id);
}

<<<<<<< HEAD
NPObject *_createobject(NPP npp, NPClass *aClass) {
||||||| merged common ancestors
NPObject*
_createobject(NPP npp, NPClass* aClass)
{
=======
NPObject* _createobject(NPP npp, NPClass* aClass) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_createobject called from the wrong thread\n"));
    return nullptr;
  }
  if (!npp) {
    NS_ERROR("Null npp passed to _createobject()!");

    return nullptr;
  }

  PluginDestructionGuard guard(npp);

  if (!aClass) {
    NS_ERROR("Null class passed to _createobject()!");

    return nullptr;
  }

  NPPAutoPusher nppPusher(npp);

  NPObject* npobj;

  if (aClass->allocate) {
    npobj = aClass->allocate(npp, aClass);
  } else {
<<<<<<< HEAD
    npobj = (NPObject *)malloc(sizeof(NPObject));
||||||| merged common ancestors
    npobj = (NPObject*) malloc(sizeof(NPObject));
=======
    npobj = (NPObject*)malloc(sizeof(NPObject));
>>>>>>> upstream-releases
  }

  if (npobj) {
    npobj->_class = aClass;
    npobj->referenceCount = 1;
    NS_LOG_ADDREF(npobj, 1, "BrowserNPObject", sizeof(NPObject));
  }

  NPN_PLUGIN_LOG(PLUGIN_LOG_NOISY,
                 ("Created NPObject %p, NPClass %p\n", npobj, aClass));

  return npobj;
}

<<<<<<< HEAD
NPObject *_retainobject(NPObject *npobj) {
||||||| merged common ancestors
NPObject*
_retainobject(NPObject* npobj)
{
=======
NPObject* _retainobject(NPObject* npobj) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_retainobject called from the wrong thread\n"));
  }
  if (npobj) {
#ifdef NS_BUILD_REFCNT_LOGGING
    int32_t refCnt =
#endif
<<<<<<< HEAD
        PR_ATOMIC_INCREMENT((int32_t *)&npobj->referenceCount);
||||||| merged common ancestors
      PR_ATOMIC_INCREMENT((int32_t*)&npobj->referenceCount);
=======
        PR_ATOMIC_INCREMENT((int32_t*)&npobj->referenceCount);
>>>>>>> upstream-releases
    NS_LOG_ADDREF(npobj, refCnt, "BrowserNPObject", sizeof(NPObject));
  }

  return npobj;
}

<<<<<<< HEAD
void _releaseobject(NPObject *npobj) {
||||||| merged common ancestors
void
_releaseobject(NPObject* npobj)
{
=======
void _releaseobject(NPObject* npobj) {
>>>>>>> upstream-releases
  // If nothing is passed, just return, even if we're on the wrong thread.
  if (!npobj) {
    return;
  }

  int32_t refCnt = PR_ATOMIC_DECREMENT((int32_t *)&npobj->referenceCount);
  NS_LOG_RELEASE(npobj, refCnt, "BrowserNPObject");

  if (refCnt == 0) {
    nsNPObjWrapper::OnDestroy(npobj);

    NPN_PLUGIN_LOG(PLUGIN_LOG_NOISY,
                   ("Deleting NPObject %p, refcount hit 0\n", npobj));

    if (npobj->_class && npobj->_class->deallocate) {
      npobj->_class->deallocate(npobj);
    } else {
      free(npobj);
    }
  }
}

<<<<<<< HEAD
bool _invoke(NPP npp, NPObject *npobj, NPIdentifier method,
             const NPVariant *args, uint32_t argCount, NPVariant *result) {
||||||| merged common ancestors
bool
_invoke(NPP npp, NPObject* npobj, NPIdentifier method, const NPVariant *args,
        uint32_t argCount, NPVariant *result)
{
=======
bool _invoke(NPP npp, NPObject* npobj, NPIdentifier method,
             const NPVariant* args, uint32_t argCount, NPVariant* result) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_invoke called from the wrong thread\n"));
    return false;
  }
  if (!npp || !npobj || !npobj->_class || !npobj->_class->invoke) return false;

  PluginDestructionGuard guard(npp);

  NPPExceptionAutoHolder nppExceptionHolder;
  NPPAutoPusher nppPusher(npp);

  NPN_PLUGIN_LOG(PLUGIN_LOG_NOISY,
                 ("NPN_Invoke(npp %p, npobj %p, method %p, args %d\n", npp,
                  npobj, method, argCount));

  return npobj->_class->invoke(npobj, method, args, argCount, result);
}

<<<<<<< HEAD
bool _invokeDefault(NPP npp, NPObject *npobj, const NPVariant *args,
                    uint32_t argCount, NPVariant *result) {
||||||| merged common ancestors
bool
_invokeDefault(NPP npp, NPObject* npobj, const NPVariant *args,
               uint32_t argCount, NPVariant *result)
{
=======
bool _invokeDefault(NPP npp, NPObject* npobj, const NPVariant* args,
                    uint32_t argCount, NPVariant* result) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_invokedefault called from the wrong thread\n"));
    return false;
  }
  if (!npp || !npobj || !npobj->_class || !npobj->_class->invokeDefault)
    return false;

  NPPExceptionAutoHolder nppExceptionHolder;
  NPPAutoPusher nppPusher(npp);

  NPN_PLUGIN_LOG(
      PLUGIN_LOG_NOISY,
      ("NPN_InvokeDefault(npp %p, npobj %p, args %d\n", npp, npobj, argCount));

  return npobj->_class->invokeDefault(npobj, args, argCount, result);
}

<<<<<<< HEAD
bool _evaluate(NPP npp, NPObject *npobj, NPString *script, NPVariant *result) {
||||||| merged common ancestors
bool
_evaluate(NPP npp, NPObject* npobj, NPString *script, NPVariant *result)
{
=======
bool _evaluate(NPP npp, NPObject* npobj, NPString* script, NPVariant* result) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_evaluate called from the wrong thread\n"));
    return false;
  }
  if (!npp) return false;

  NPPAutoPusher nppPusher(npp);

  Document* doc = GetDocumentFromNPP(npp);
  NS_ENSURE_TRUE(doc, false);

<<<<<<< HEAD
  nsGlobalWindowInner *win = nsGlobalWindowInner::Cast(doc->GetInnerWindow());
  if (NS_WARN_IF(!win || !win->FastGetGlobalJSObject())) {
||||||| merged common ancestors
  nsGlobalWindowInner* win = nsGlobalWindowInner::Cast(doc->GetInnerWindow());
  if (NS_WARN_IF(!win || !win->FastGetGlobalJSObject())) {
=======
  nsGlobalWindowInner* win = nsGlobalWindowInner::Cast(doc->GetInnerWindow());
  if (NS_WARN_IF(!win || !win->HasJSGlobal())) {
>>>>>>> upstream-releases
    return false;
  }

  nsAutoMicroTask mt;
  dom::AutoEntryScript aes(win, "NPAPI NPN_evaluate");
  JSContext *cx = aes.cx();

  JS::Rooted<JSObject *> obj(cx, nsNPObjWrapper::GetNewOrUsed(npp, cx, npobj));

  if (!obj) {
    return false;
  }

  obj = js::ToWindowIfWindowProxy(obj);
  MOZ_ASSERT(obj, "ToWindowIfWindowProxy should never return null");

  if (result) {
    // Initialize the out param to void
    VOID_TO_NPVARIANT(*result);
  }

  if (!script || !script->UTF8Length || !script->UTF8Characters) {
    // Nothing to evaluate.

    return true;
  }

  NS_ConvertUTF8toUTF16 utf16script(script->UTF8Characters, script->UTF8Length);

  nsIPrincipal* principal = doc->NodePrincipal();

  nsAutoCString specStr;
  const char* spec;

  nsCOMPtr<nsIURI> uri;
  principal->GetURI(getter_AddRefs(uri));

  if (uri) {
    uri->GetSpec(specStr);
    spec = specStr.get();
  } else {
    // No URI in a principal means it's the system principal. If the
    // document URI is a chrome:// URI, pass that in as the URI of the
    // script, else pass in null for the filename as there's no way to
    // know where this document really came from. Passing in null here
    // also means that the script gets treated by XPConnect as if it
    // needs additional protection, which is what we want for unknown
    // chrome code anyways.

    uri = doc->GetDocumentURI();
    bool isChrome = false;

    if (uri && NS_SUCCEEDED(uri->SchemeIs("chrome", &isChrome)) && isChrome) {
      uri->GetSpec(specStr);
      spec = specStr.get();
    } else {
      spec = nullptr;
    }
  }

  NPN_PLUGIN_LOG(PLUGIN_LOG_NOISY,
                 ("NPN_Evaluate(npp %p, npobj %p, script <<<%s>>>) called\n",
                  npp, npobj, script->UTF8Characters));

  JS::CompileOptions options(cx);
  options.setFileAndLine(spec, 0);
  JS::Rooted<JS::Value> rval(cx);
  JS::RootedVector<JSObject*> scopeChain(cx);
  if (!JS_IsGlobalObject(obj) && !scopeChain.append(obj)) {
    return false;
  }
  // nsNPObjWrapper::GetNewOrUsed returns an object in the current compartment
  // of the JSContext (it might be a CCW).
  MOZ_RELEASE_ASSERT(
      js::GetObjectCompartment(obj) == js::GetContextCompartment(cx),
      "nsNPObjWrapper::GetNewOrUsed must wrap its return value");
  obj = JS::CurrentGlobalOrNull(cx);
  MOZ_ASSERT(obj);
  nsresult rv = NS_OK;
  {
    nsJSUtils::ExecutionContext exec(cx, obj);
    exec.SetScopeChain(scopeChain);
    exec.Compile(options, utf16script);
    rv = exec.ExecScript(&rval);
  }

  if (!JS_WrapValue(cx, &rval)) {
    return false;
  }

  return NS_SUCCEEDED(rv) &&
         (!result || JSValToNPVariant(npp, cx, rval, result));
}

<<<<<<< HEAD
bool _getproperty(NPP npp, NPObject *npobj, NPIdentifier property,
                  NPVariant *result) {
||||||| merged common ancestors
bool
_getproperty(NPP npp, NPObject* npobj, NPIdentifier property,
             NPVariant *result)
{
=======
bool _getproperty(NPP npp, NPObject* npobj, NPIdentifier property,
                  NPVariant* result) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_getproperty called from the wrong thread\n"));
    return false;
  }
  if (!npp || !npobj || !npobj->_class || !npobj->_class->getProperty)
    return false;

  NPPExceptionAutoHolder nppExceptionHolder;
  NPPAutoPusher nppPusher(npp);

  NPN_PLUGIN_LOG(PLUGIN_LOG_NOISY,
                 ("NPN_GetProperty(npp %p, npobj %p, property %p) called\n",
                  npp, npobj, property));

  if (!npobj->_class->getProperty(npobj, property, result)) return false;

  return true;
}

<<<<<<< HEAD
bool _setproperty(NPP npp, NPObject *npobj, NPIdentifier property,
                  const NPVariant *value) {
||||||| merged common ancestors
bool
_setproperty(NPP npp, NPObject* npobj, NPIdentifier property,
             const NPVariant *value)
{
=======
bool _setproperty(NPP npp, NPObject* npobj, NPIdentifier property,
                  const NPVariant* value) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_setproperty called from the wrong thread\n"));
    return false;
  }
  if (!npp || !npobj || !npobj->_class || !npobj->_class->setProperty)
    return false;

  NPPExceptionAutoHolder nppExceptionHolder;
  NPPAutoPusher nppPusher(npp);

  NPN_PLUGIN_LOG(PLUGIN_LOG_NOISY,
                 ("NPN_SetProperty(npp %p, npobj %p, property %p) called\n",
                  npp, npobj, property));

  return npobj->_class->setProperty(npobj, property, value);
}

<<<<<<< HEAD
bool _removeproperty(NPP npp, NPObject *npobj, NPIdentifier property) {
||||||| merged common ancestors
bool
_removeproperty(NPP npp, NPObject* npobj, NPIdentifier property)
{
=======
bool _removeproperty(NPP npp, NPObject* npobj, NPIdentifier property) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_removeproperty called from the wrong thread\n"));
    return false;
  }
  if (!npp || !npobj || !npobj->_class || !npobj->_class->removeProperty)
    return false;

  NPPExceptionAutoHolder nppExceptionHolder;
  NPPAutoPusher nppPusher(npp);

  NPN_PLUGIN_LOG(PLUGIN_LOG_NOISY,
                 ("NPN_RemoveProperty(npp %p, npobj %p, property %p) called\n",
                  npp, npobj, property));

  return npobj->_class->removeProperty(npobj, property);
}

<<<<<<< HEAD
bool _hasproperty(NPP npp, NPObject *npobj, NPIdentifier propertyName) {
||||||| merged common ancestors
bool
_hasproperty(NPP npp, NPObject* npobj, NPIdentifier propertyName)
{
=======
bool _hasproperty(NPP npp, NPObject* npobj, NPIdentifier propertyName) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_hasproperty called from the wrong thread\n"));
    return false;
  }
  if (!npp || !npobj || !npobj->_class || !npobj->_class->hasProperty)
    return false;

  NPPExceptionAutoHolder nppExceptionHolder;
  NPPAutoPusher nppPusher(npp);

  NPN_PLUGIN_LOG(PLUGIN_LOG_NOISY,
                 ("NPN_HasProperty(npp %p, npobj %p, property %p) called\n",
                  npp, npobj, propertyName));

  return npobj->_class->hasProperty(npobj, propertyName);
}

<<<<<<< HEAD
bool _hasmethod(NPP npp, NPObject *npobj, NPIdentifier methodName) {
||||||| merged common ancestors
bool
_hasmethod(NPP npp, NPObject* npobj, NPIdentifier methodName)
{
=======
bool _hasmethod(NPP npp, NPObject* npobj, NPIdentifier methodName) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_hasmethod called from the wrong thread\n"));
    return false;
  }
  if (!npp || !npobj || !npobj->_class || !npobj->_class->hasMethod)
    return false;

  NPPExceptionAutoHolder nppExceptionHolder;
  NPPAutoPusher nppPusher(npp);

  NPN_PLUGIN_LOG(PLUGIN_LOG_NOISY,
                 ("NPN_HasMethod(npp %p, npobj %p, property %p) called\n", npp,
                  npobj, methodName));

  return npobj->_class->hasMethod(npobj, methodName);
}

<<<<<<< HEAD
bool _enumerate(NPP npp, NPObject *npobj, NPIdentifier **identifier,
                uint32_t *count) {
||||||| merged common ancestors
bool
_enumerate(NPP npp, NPObject *npobj, NPIdentifier **identifier,
           uint32_t *count)
{
=======
bool _enumerate(NPP npp, NPObject* npobj, NPIdentifier** identifier,
                uint32_t* count) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_enumerate called from the wrong thread\n"));
    return false;
  }
  if (!npp || !npobj || !npobj->_class) return false;

  NPN_PLUGIN_LOG(PLUGIN_LOG_NOISY,
                 ("NPN_Enumerate(npp %p, npobj %p) called\n", npp, npobj));

  if (!NP_CLASS_STRUCT_VERSION_HAS_ENUM(npobj->_class) ||
      !npobj->_class->enumerate) {
    *identifier = 0;
    *count = 0;
    return true;
  }

  NPPExceptionAutoHolder nppExceptionHolder;
  NPPAutoPusher nppPusher(npp);

  return npobj->_class->enumerate(npobj, identifier, count);
}

<<<<<<< HEAD
bool _construct(NPP npp, NPObject *npobj, const NPVariant *args,
                uint32_t argCount, NPVariant *result) {
||||||| merged common ancestors
bool
_construct(NPP npp, NPObject* npobj, const NPVariant *args,
               uint32_t argCount, NPVariant *result)
{
=======
bool _construct(NPP npp, NPObject* npobj, const NPVariant* args,
                uint32_t argCount, NPVariant* result) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_construct called from the wrong thread\n"));
    return false;
  }
  if (!npp || !npobj || !npobj->_class ||
      !NP_CLASS_STRUCT_VERSION_HAS_CTOR(npobj->_class) ||
      !npobj->_class->construct) {
    return false;
  }

  NPPExceptionAutoHolder nppExceptionHolder;
  NPPAutoPusher nppPusher(npp);

  return npobj->_class->construct(npobj, args, argCount, result);
}

<<<<<<< HEAD
void _releasevariantvalue(NPVariant *variant) {
||||||| merged common ancestors
void
_releasevariantvalue(NPVariant* variant)
{
=======
void _releasevariantvalue(NPVariant* variant) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_releasevariantvalue called from the wrong thread\n"));
  }
  switch (variant->type) {
<<<<<<< HEAD
    case NPVariantType_Void:
    case NPVariantType_Null:
    case NPVariantType_Bool:
    case NPVariantType_Int32:
    case NPVariantType_Double:
      break;
    case NPVariantType_String: {
      const NPString *s = &NPVARIANT_TO_STRING(*variant);
||||||| merged common ancestors
  case NPVariantType_Void :
  case NPVariantType_Null :
  case NPVariantType_Bool :
  case NPVariantType_Int32 :
  case NPVariantType_Double :
    break;
  case NPVariantType_String :
    {
      const NPString *s = &NPVARIANT_TO_STRING(*variant);
=======
    case NPVariantType_Void:
    case NPVariantType_Null:
    case NPVariantType_Bool:
    case NPVariantType_Int32:
    case NPVariantType_Double:
      break;
    case NPVariantType_String: {
      const NPString* s = &NPVARIANT_TO_STRING(*variant);
>>>>>>> upstream-releases

      if (s->UTF8Characters) {
#if defined(MOZ_MEMORY) && defined(XP_WIN)
<<<<<<< HEAD
        if (malloc_usable_size((void *)s->UTF8Characters) != 0) {
          free((void *)s->UTF8Characters);
||||||| merged common ancestors
        if (malloc_usable_size((void *)s->UTF8Characters) != 0) {
          free((void*)s->UTF8Characters);
=======
        if (malloc_usable_size((void*)s->UTF8Characters) != 0) {
          free((void*)s->UTF8Characters);
>>>>>>> upstream-releases
        } else {
          void* p = (void*)s->UTF8Characters;
          DWORD nheaps = 0;
          AutoTArray<HANDLE, 50> heaps;
          nheaps = GetProcessHeaps(0, heaps.Elements());
          heaps.AppendElements(nheaps);
          GetProcessHeaps(nheaps, heaps.Elements());
          for (DWORD i = 0; i < nheaps; i++) {
            if (InHeap(heaps[i], p)) {
              HeapFree(heaps[i], 0, p);
              break;
            }
          }
        }
#else
        free((void*)s->UTF8Characters);
#endif
      }
      break;
    }
<<<<<<< HEAD
    case NPVariantType_Object: {
      NPObject *npobj = NPVARIANT_TO_OBJECT(*variant);
||||||| merged common ancestors
  case NPVariantType_Object:
    {
      NPObject *npobj = NPVARIANT_TO_OBJECT(*variant);
=======
    case NPVariantType_Object: {
      NPObject* npobj = NPVARIANT_TO_OBJECT(*variant);
>>>>>>> upstream-releases

      if (npobj) _releaseobject(npobj);

      break;
    }
    default:
      NS_ERROR("Unknown NPVariant type!");
  }

  VOID_TO_NPVARIANT(*variant);
}

<<<<<<< HEAD
void _setexception(NPObject *npobj, const NPUTF8 *message) {
||||||| merged common ancestors
void
_setexception(NPObject* npobj, const NPUTF8 *message)
{
=======
void _setexception(NPObject* npobj, const NPUTF8* message) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_setexception called from the wrong thread\n"));
    return;
  }

  if (!message) return;

  if (gNPPException) {
    // If a plugin throws multiple exceptions, we'll only report the
    // last one for now.
    free(gNPPException);
  }

  gNPPException = strdup(message);
}

<<<<<<< HEAD
NPError _getvalue(NPP npp, NPNVariable variable, void *result) {
||||||| merged common ancestors
NPError
_getvalue(NPP npp, NPNVariable variable, void *result)
{
=======
NPError _getvalue(NPP npp, NPNVariable variable, void* result) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_getvalue called from the wrong thread\n"));
    return NPERR_INVALID_PARAM;
  }
<<<<<<< HEAD
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL, ("NPN_GetValue: npp=%p, var=%d\n",
                                     (void *)npp, (int)variable));
||||||| merged common ancestors
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL, ("NPN_GetValue: npp=%p, var=%d\n",
                                     (void*)npp, (int)variable));
=======
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL,
                 ("NPN_GetValue: npp=%p, var=%d\n", (void*)npp, (int)variable));
>>>>>>> upstream-releases

  nsresult res;

  PluginDestructionGuard guard(npp);

  // Cast NPNVariable enum to int to avoid warnings about including switch
  // cases for android_npapi.h's non-standard ANPInterface values.
  switch (static_cast<int>(variable)) {
#if defined(XP_UNIX) && !defined(XP_MACOSX)
<<<<<<< HEAD
    case NPNVxDisplay: {
#if defined(MOZ_X11)
      if (npp) {
        nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)npp->ndata;
        bool windowless = false;
        inst->IsWindowless(&windowless);
        // The documentation on the types for many variables in NP(N|P)_GetValue
        // is vague.  Often boolean values are NPBool (1 byte), but
        // https://developer.mozilla.org/en/XEmbed_Extension_for_Mozilla_Plugins
        // treats NPPVpluginNeedsXEmbed as PRBool (int), and
        // on x86/32-bit, flash stores to this using |movl 0x1,&needsXEmbed|.
        // thus we can't use NPBool for needsXEmbed, or the three bytes above
        // it on the stack would get clobbered. so protect with the larger bool.
        int needsXEmbed = 0;
        if (!windowless) {
          res = inst->GetValueFromPlugin(NPPVpluginNeedsXEmbed, &needsXEmbed);
          // If the call returned an error code make sure we still use our
          // default value.
          if (NS_FAILED(res)) {
            needsXEmbed = 0;
          }
        }
        if (windowless || needsXEmbed) {
          (*(Display **)result) = mozilla::DefaultXDisplay();
          return NPERR_NO_ERROR;
||||||| merged common ancestors
  case NPNVxDisplay : {
#if defined(MOZ_X11)
    if (npp) {
      nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *) npp->ndata;
      bool windowless = false;
      inst->IsWindowless(&windowless);
      // The documentation on the types for many variables in NP(N|P)_GetValue
      // is vague.  Often boolean values are NPBool (1 byte), but
      // https://developer.mozilla.org/en/XEmbed_Extension_for_Mozilla_Plugins
      // treats NPPVpluginNeedsXEmbed as PRBool (int), and
      // on x86/32-bit, flash stores to this using |movl 0x1,&needsXEmbed|.
      // thus we can't use NPBool for needsXEmbed, or the three bytes above
      // it on the stack would get clobbered. so protect with the larger bool.
      int needsXEmbed = 0;
      if (!windowless) {
        res = inst->GetValueFromPlugin(NPPVpluginNeedsXEmbed, &needsXEmbed);
        // If the call returned an error code make sure we still use our default value.
        if (NS_FAILED(res)) {
          needsXEmbed = 0;
=======
    case NPNVxDisplay: {
#  if defined(MOZ_X11)
      if (npp) {
        nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)npp->ndata;
        bool windowless = false;
        inst->IsWindowless(&windowless);
        // The documentation on the types for many variables in NP(N|P)_GetValue
        // is vague.  Often boolean values are NPBool (1 byte), but
        // https://developer.mozilla.org/en/XEmbed_Extension_for_Mozilla_Plugins
        // treats NPPVpluginNeedsXEmbed as PRBool (int), and
        // on x86/32-bit, flash stores to this using |movl 0x1,&needsXEmbed|.
        // thus we can't use NPBool for needsXEmbed, or the three bytes above
        // it on the stack would get clobbered. so protect with the larger bool.
        int needsXEmbed = 0;
        if (!windowless) {
          res = inst->GetValueFromPlugin(NPPVpluginNeedsXEmbed, &needsXEmbed);
          // If the call returned an error code make sure we still use our
          // default value.
          if (NS_FAILED(res)) {
            needsXEmbed = 0;
          }
        }
        if (windowless || needsXEmbed) {
          (*(Display**)result) = mozilla::DefaultXDisplay();
          return NPERR_NO_ERROR;
>>>>>>> upstream-releases
        }
      }
<<<<<<< HEAD
#endif
      return NPERR_GENERIC_ERROR;
    }
||||||| merged common ancestors
      if (windowless || needsXEmbed) {
        (*(Display **)result) = mozilla::DefaultXDisplay();
        return NPERR_NO_ERROR;
      }
    }
#endif
    return NPERR_GENERIC_ERROR;
  }
=======
#  endif
      return NPERR_GENERIC_ERROR;
    }
>>>>>>> upstream-releases

    case NPNVxtAppContext:
      return NPERR_GENERIC_ERROR;
#endif

#if defined(XP_WIN) || defined(MOZ_WIDGET_GTK)
    case NPNVnetscapeWindow: {
      if (!npp || !npp->ndata) return NPERR_INVALID_INSTANCE_ERROR;

<<<<<<< HEAD
      nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)npp->ndata;
||||||| merged common ancestors
    nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *) npp->ndata;
=======
      nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)npp->ndata;
>>>>>>> upstream-releases

      RefPtr<nsPluginInstanceOwner> owner = inst->GetOwner();
      NS_ENSURE_TRUE(owner, NPERR_NO_ERROR);

      if (NS_SUCCEEDED(owner->GetNetscapeWindow(result))) {
        return NPERR_NO_ERROR;
      }
      return NPERR_GENERIC_ERROR;
    }
#endif

<<<<<<< HEAD
    case NPNVjavascriptEnabledBool: {
      *(NPBool *)result = false;
      bool js = false;
      res = Preferences::GetBool("javascript.enabled", &js);
      if (NS_SUCCEEDED(res)) {
        *(NPBool *)result = js;
      }
      return NPERR_NO_ERROR;
||||||| merged common ancestors
  case NPNVjavascriptEnabledBool: {
    *(NPBool*)result = false;
    bool js = false;
    res = Preferences::GetBool("javascript.enabled", &js);
    if (NS_SUCCEEDED(res)) {
      *(NPBool*)result = js;
=======
    case NPNVjavascriptEnabledBool: {
      *(NPBool*)result = false;
      bool js = false;
      res = Preferences::GetBool("javascript.enabled", &js);
      if (NS_SUCCEEDED(res)) {
        *(NPBool*)result = js;
      }
      return NPERR_NO_ERROR;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    case NPNVasdEnabledBool:
      *(NPBool *)result = false;
      return NPERR_NO_ERROR;
||||||| merged common ancestors
  case NPNVasdEnabledBool:
    *(NPBool*)result = false;
    return NPERR_NO_ERROR;
=======
    case NPNVasdEnabledBool:
      *(NPBool*)result = false;
      return NPERR_NO_ERROR;
>>>>>>> upstream-releases

    case NPNVisOfflineBool: {
      bool offline = false;
      nsCOMPtr<nsIIOService> ioservice =
          do_GetService(NS_IOSERVICE_CONTRACTID, &res);
      if (NS_SUCCEEDED(res)) res = ioservice->GetOffline(&offline);
      if (NS_FAILED(res)) return NPERR_GENERIC_ERROR;

<<<<<<< HEAD
      *(NPBool *)result = offline;
      return NPERR_NO_ERROR;
    }
||||||| merged common ancestors
    *(NPBool*)result = offline;
    return NPERR_NO_ERROR;
  }
=======
      *(NPBool*)result = offline;
      return NPERR_NO_ERROR;
    }
>>>>>>> upstream-releases

    case NPNVToolkit: {
#ifdef MOZ_WIDGET_GTK
<<<<<<< HEAD
      *((NPNToolkitType *)result) = NPNVGtk2;
||||||| merged common ancestors
    *((NPNToolkitType*)result) = NPNVGtk2;
=======
      *((NPNToolkitType*)result) = NPNVGtk2;
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
      if (*(NPNToolkitType *)result) return NPERR_NO_ERROR;
||||||| merged common ancestors
    if (*(NPNToolkitType*)result)
        return NPERR_NO_ERROR;
=======
      if (*(NPNToolkitType*)result) return NPERR_NO_ERROR;
>>>>>>> upstream-releases

      return NPERR_GENERIC_ERROR;
    }

    case NPNVSupportsXEmbedBool: {
#ifdef MOZ_WIDGET_GTK
<<<<<<< HEAD
      *(NPBool *)result = true;
||||||| merged common ancestors
    *(NPBool*)result = true;
=======
      *(NPBool*)result = true;
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
      *(NPBool *)result = false;
||||||| merged common ancestors
    *(NPBool*)result = false;
=======
      *(NPBool*)result = false;
>>>>>>> upstream-releases
#endif
      return NPERR_NO_ERROR;
    }

<<<<<<< HEAD
    case NPNVWindowNPObject: {
      *(NPObject **)result = _getwindowobject(npp);
||||||| merged common ancestors
  case NPNVWindowNPObject: {
    *(NPObject **)result = _getwindowobject(npp);
=======
    case NPNVWindowNPObject: {
      *(NPObject**)result = _getwindowobject(npp);
>>>>>>> upstream-releases

<<<<<<< HEAD
      return *(NPObject **)result ? NPERR_NO_ERROR : NPERR_GENERIC_ERROR;
    }
||||||| merged common ancestors
    return *(NPObject **)result ? NPERR_NO_ERROR : NPERR_GENERIC_ERROR;
  }
=======
      return *(NPObject**)result ? NPERR_NO_ERROR : NPERR_GENERIC_ERROR;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case NPNVPluginElementNPObject: {
      *(NPObject **)result = _getpluginelement(npp);
||||||| merged common ancestors
  case NPNVPluginElementNPObject: {
    *(NPObject **)result = _getpluginelement(npp);
=======
    case NPNVPluginElementNPObject: {
      *(NPObject**)result = _getpluginelement(npp);
>>>>>>> upstream-releases

<<<<<<< HEAD
      return *(NPObject **)result ? NPERR_NO_ERROR : NPERR_GENERIC_ERROR;
    }
||||||| merged common ancestors
    return *(NPObject **)result ? NPERR_NO_ERROR : NPERR_GENERIC_ERROR;
  }
=======
      return *(NPObject**)result ? NPERR_NO_ERROR : NPERR_GENERIC_ERROR;
    }
>>>>>>> upstream-releases

    case NPNVSupportsWindowless: {
#if defined(XP_WIN) || defined(XP_MACOSX) || \
    (defined(MOZ_X11) && defined(MOZ_WIDGET_GTK))
<<<<<<< HEAD
      *(NPBool *)result = true;
||||||| merged common ancestors
    *(NPBool*)result = true;
=======
      *(NPBool*)result = true;
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
      *(NPBool *)result = false;
||||||| merged common ancestors
    *(NPBool*)result = false;
=======
      *(NPBool*)result = false;
>>>>>>> upstream-releases
#endif
      return NPERR_NO_ERROR;
    }

<<<<<<< HEAD
    case NPNVprivateModeBool: {
      bool privacy;
      nsNPAPIPluginInstance *inst =
          static_cast<nsNPAPIPluginInstance *>(npp->ndata);
      if (!inst) return NPERR_GENERIC_ERROR;
||||||| merged common ancestors
    nsresult rv = inst->IsPrivateBrowsing(&privacy);
    if (NS_FAILED(rv))
      return NPERR_GENERIC_ERROR;
    *(NPBool*)result = (NPBool)privacy;
    return NPERR_NO_ERROR;
  }
=======
    case NPNVprivateModeBool: {
      bool privacy;
      nsNPAPIPluginInstance* inst =
          static_cast<nsNPAPIPluginInstance*>(npp->ndata);
      if (!inst) return NPERR_GENERIC_ERROR;
>>>>>>> upstream-releases

<<<<<<< HEAD
      nsresult rv = inst->IsPrivateBrowsing(&privacy);
      if (NS_FAILED(rv)) return NPERR_GENERIC_ERROR;
      *(NPBool *)result = (NPBool)privacy;
      return NPERR_NO_ERROR;
||||||| merged common ancestors
  case NPNVdocumentOrigin: {
    nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)npp->ndata;
    if (!inst) {
      return NPERR_GENERIC_ERROR;
=======
      nsresult rv = inst->IsPrivateBrowsing(&privacy);
      if (NS_FAILED(rv)) return NPERR_GENERIC_ERROR;
      *(NPBool*)result = (NPBool)privacy;
      return NPERR_NO_ERROR;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    case NPNVdocumentOrigin: {
      nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)npp->ndata;
      if (!inst) {
        return NPERR_GENERIC_ERROR;
      }
||||||| merged common ancestors
    RefPtr<dom::Element> element;
    inst->GetDOMElement(getter_AddRefs(element));
    if (!element) {
      return NPERR_GENERIC_ERROR;
    }
=======
    case NPNVdocumentOrigin: {
      nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)npp->ndata;
      if (!inst) {
        return NPERR_GENERIC_ERROR;
      }
>>>>>>> upstream-releases

      RefPtr<dom::Element> element;
      inst->GetDOMElement(getter_AddRefs(element));
      if (!element) {
        return NPERR_GENERIC_ERROR;
      }

<<<<<<< HEAD
      nsIPrincipal *principal = element->NodePrincipal();
||||||| merged common ancestors
    nsAutoString utf16Origin;
    res = nsContentUtils::GetUTFOrigin(principal, utf16Origin);
    if (NS_FAILED(res)) {
      return NPERR_GENERIC_ERROR;
    }
=======
      nsIPrincipal* principal = element->NodePrincipal();
>>>>>>> upstream-releases

      nsAutoString utf16Origin;
      res = nsContentUtils::GetUTFOrigin(principal, utf16Origin);
      if (NS_FAILED(res)) {
        return NPERR_GENERIC_ERROR;
      }

      nsCOMPtr<nsIIDNService> idnService =
          do_GetService(NS_IDNSERVICE_CONTRACTID);
      if (!idnService) {
        return NPERR_GENERIC_ERROR;
      }

<<<<<<< HEAD
      // This is a bit messy: we convert to UTF-8 here, but then
      // nsIDNService::Normalize will convert back to UTF-16 for processing,
      // and back to UTF-8 again to return the result.
      // Alternative: perhaps we should add a NormalizeUTF16 version of the API,
      // and just convert to UTF-8 for the final return (resulting in one
      // encoding form conversion instead of three).
      NS_ConvertUTF16toUTF8 utf8Origin(utf16Origin);
      nsAutoCString normalizedUTF8Origin;
      res = idnService->Normalize(utf8Origin, normalizedUTF8Origin);
      if (NS_FAILED(res)) {
        return NPERR_GENERIC_ERROR;
      }

      *(char **)result = ToNewCString(normalizedUTF8Origin);
      return *(char **)result ? NPERR_NO_ERROR : NPERR_GENERIC_ERROR;
    }
||||||| merged common ancestors
    *(char**)result = ToNewCString(normalizedUTF8Origin);
    return *(char**)result ? NPERR_NO_ERROR : NPERR_GENERIC_ERROR;
  }
=======
      // This is a bit messy: we convert to UTF-8 here, but then
      // nsIDNService::Normalize will convert back to UTF-16 for processing,
      // and back to UTF-8 again to return the result.
      // Alternative: perhaps we should add a NormalizeUTF16 version of the API,
      // and just convert to UTF-8 for the final return (resulting in one
      // encoding form conversion instead of three).
      NS_ConvertUTF16toUTF8 utf8Origin(utf16Origin);
      nsAutoCString normalizedUTF8Origin;
      res = idnService->Normalize(utf8Origin, normalizedUTF8Origin);
      if (NS_FAILED(res)) {
        return NPERR_GENERIC_ERROR;
      }

      *(char**)result = ToNewCString(normalizedUTF8Origin);
      return *(char**)result ? NPERR_NO_ERROR : NPERR_GENERIC_ERROR;
    }
>>>>>>> upstream-releases

#ifdef XP_MACOSX
<<<<<<< HEAD
    case NPNVpluginDrawingModel: {
      if (npp) {
        nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)npp->ndata;
        if (inst) {
          NPDrawingModel drawingModel;
          inst->GetDrawingModel((int32_t *)&drawingModel);
          *(NPDrawingModel *)result = drawingModel;
          return NPERR_NO_ERROR;
        }
||||||| merged common ancestors
  case NPNVpluginDrawingModel: {
    if (npp) {
      nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance*)npp->ndata;
      if (inst) {
        NPDrawingModel drawingModel;
        inst->GetDrawingModel((int32_t*)&drawingModel);
        *(NPDrawingModel*)result = drawingModel;
        return NPERR_NO_ERROR;
=======
    case NPNVpluginDrawingModel: {
      if (npp) {
        nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)npp->ndata;
        if (inst) {
          NPDrawingModel drawingModel;
          inst->GetDrawingModel((int32_t*)&drawingModel);
          *(NPDrawingModel*)result = drawingModel;
          return NPERR_NO_ERROR;
        }
>>>>>>> upstream-releases
      }
      return NPERR_GENERIC_ERROR;
    }

<<<<<<< HEAD
#ifndef NP_NO_QUICKDRAW
    case NPNVsupportsQuickDrawBool: {
      *(NPBool *)result = false;
||||||| merged common ancestors
#ifndef NP_NO_QUICKDRAW
  case NPNVsupportsQuickDrawBool: {
    *(NPBool*)result = false;
=======
#  ifndef NP_NO_QUICKDRAW
    case NPNVsupportsQuickDrawBool: {
      *(NPBool*)result = false;
>>>>>>> upstream-releases

<<<<<<< HEAD
      return NPERR_NO_ERROR;
    }
#endif
||||||| merged common ancestors
    return NPERR_NO_ERROR;
  }
#endif
=======
      return NPERR_NO_ERROR;
    }
#  endif
>>>>>>> upstream-releases

<<<<<<< HEAD
    case NPNVsupportsCoreGraphicsBool: {
      *(NPBool *)result = true;
||||||| merged common ancestors
  case NPNVsupportsCoreGraphicsBool: {
    *(NPBool*)result = true;
=======
    case NPNVsupportsCoreGraphicsBool: {
      *(NPBool*)result = true;
>>>>>>> upstream-releases

      return NPERR_NO_ERROR;
    }

<<<<<<< HEAD
    case NPNVsupportsCoreAnimationBool: {
      *(NPBool *)result = true;
||||||| merged common ancestors
  case NPNVsupportsCoreAnimationBool: {
    *(NPBool*)result = true;
=======
    case NPNVsupportsCoreAnimationBool: {
      *(NPBool*)result = true;
>>>>>>> upstream-releases

      return NPERR_NO_ERROR;
    }

<<<<<<< HEAD
    case NPNVsupportsInvalidatingCoreAnimationBool: {
      *(NPBool *)result = true;
||||||| merged common ancestors
  case NPNVsupportsInvalidatingCoreAnimationBool: {
    *(NPBool*)result = true;
=======
    case NPNVsupportsInvalidatingCoreAnimationBool: {
      *(NPBool*)result = true;
>>>>>>> upstream-releases

      return NPERR_NO_ERROR;
    }

<<<<<<< HEAD
    case NPNVsupportsCompositingCoreAnimationPluginsBool: {
      *(NPBool *)result = PR_TRUE;
||||||| merged common ancestors
  case NPNVsupportsCompositingCoreAnimationPluginsBool: {
    *(NPBool*)result = PR_TRUE;
=======
    case NPNVsupportsCompositingCoreAnimationPluginsBool: {
      *(NPBool*)result = PR_TRUE;
>>>>>>> upstream-releases

      return NPERR_NO_ERROR;
    }

<<<<<<< HEAD
#ifndef NP_NO_CARBON
    case NPNVsupportsCarbonBool: {
      *(NPBool *)result = false;
||||||| merged common ancestors
#ifndef NP_NO_CARBON
  case NPNVsupportsCarbonBool: {
    *(NPBool*)result = false;
=======
#  ifndef NP_NO_CARBON
    case NPNVsupportsCarbonBool: {
      *(NPBool*)result = false;
>>>>>>> upstream-releases

<<<<<<< HEAD
      return NPERR_NO_ERROR;
    }
#endif
    case NPNVsupportsCocoaBool: {
      *(NPBool *)result = true;
||||||| merged common ancestors
    return NPERR_NO_ERROR;
  }
#endif
  case NPNVsupportsCocoaBool: {
    *(NPBool*)result = true;
=======
      return NPERR_NO_ERROR;
    }
#  endif
    case NPNVsupportsCocoaBool: {
      *(NPBool*)result = true;
>>>>>>> upstream-releases

      return NPERR_NO_ERROR;
    }

<<<<<<< HEAD
    case NPNVsupportsUpdatedCocoaTextInputBool: {
      *(NPBool *)result = true;
      return NPERR_NO_ERROR;
    }
||||||| merged common ancestors
  case NPNVsupportsUpdatedCocoaTextInputBool: {
    *(NPBool*)result = true;
    return NPERR_NO_ERROR;
  }
=======
    case NPNVsupportsUpdatedCocoaTextInputBool: {
      *(NPBool*)result = true;
      return NPERR_NO_ERROR;
    }
>>>>>>> upstream-releases
#endif

#if defined(XP_MACOSX) || defined(XP_WIN)
<<<<<<< HEAD
    case NPNVcontentsScaleFactor: {
      nsNPAPIPluginInstance *inst =
          (nsNPAPIPluginInstance *)(npp ? npp->ndata : nullptr);
      double scaleFactor = inst ? inst->GetContentsScaleFactor() : 1.0;
      *(double *)result = scaleFactor;
      return NPERR_NO_ERROR;
    }
||||||| merged common ancestors
  case NPNVcontentsScaleFactor: {
    nsNPAPIPluginInstance *inst =
      (nsNPAPIPluginInstance *) (npp ? npp->ndata : nullptr);
    double scaleFactor = inst ? inst->GetContentsScaleFactor() : 1.0;
    *(double*)result = scaleFactor;
    return NPERR_NO_ERROR;
  }
=======
    case NPNVcontentsScaleFactor: {
      nsNPAPIPluginInstance* inst =
          (nsNPAPIPluginInstance*)(npp ? npp->ndata : nullptr);
      double scaleFactor = inst ? inst->GetContentsScaleFactor() : 1.0;
      *(double*)result = scaleFactor;
      return NPERR_NO_ERROR;
    }
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
    case NPNVCSSZoomFactor: {
      nsNPAPIPluginInstance *inst =
          (nsNPAPIPluginInstance *)(npp ? npp->ndata : nullptr);
      double scaleFactor = inst ? inst->GetCSSZoomFactor() : 1.0;
      *(double *)result = scaleFactor;
      return NPERR_NO_ERROR;
    }

    // we no longer hand out any XPCOM objects
    case NPNVDOMElement:
    case NPNVDOMWindow:
    case NPNVserviceManager:
      // old XPCOM objects, no longer supported, but null out the out
      // param to avoid crashing plugins that still try to use this.
      *(nsISupports **)result = nullptr;
      MOZ_FALLTHROUGH;

    default:
      NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL,
                     ("NPN_getvalue unhandled get value: %d\n", variable));
      return NPERR_GENERIC_ERROR;
||||||| merged common ancestors
  case NPNVCSSZoomFactor: {
    nsNPAPIPluginInstance *inst =
      (nsNPAPIPluginInstance *) (npp ? npp->ndata : nullptr);
    double scaleFactor = inst ? inst->GetCSSZoomFactor() : 1.0;
    *(double*)result = scaleFactor;
    return NPERR_NO_ERROR;
  }

  // we no longer hand out any XPCOM objects
  case NPNVDOMElement:
  case NPNVDOMWindow:
  case NPNVserviceManager:
    // old XPCOM objects, no longer supported, but null out the out
    // param to avoid crashing plugins that still try to use this.
    *(nsISupports**)result = nullptr;
    MOZ_FALLTHROUGH;

  default:
    NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL, ("NPN_getvalue unhandled get value: %d\n", variable));
    return NPERR_GENERIC_ERROR;
=======
    case NPNVCSSZoomFactor: {
      nsNPAPIPluginInstance* inst =
          (nsNPAPIPluginInstance*)(npp ? npp->ndata : nullptr);
      double scaleFactor = inst ? inst->GetCSSZoomFactor() : 1.0;
      *(double*)result = scaleFactor;
      return NPERR_NO_ERROR;
    }

    // we no longer hand out any XPCOM objects
    case NPNVDOMElement:
    case NPNVDOMWindow:
    case NPNVserviceManager:
      // old XPCOM objects, no longer supported, but null out the out
      // param to avoid crashing plugins that still try to use this.
      *(nsISupports**)result = nullptr;
      MOZ_FALLTHROUGH;

    default:
      NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL,
                     ("NPN_getvalue unhandled get value: %d\n", variable));
      return NPERR_GENERIC_ERROR;
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
NPError _setvalue(NPP npp, NPPVariable variable, void *result) {
||||||| merged common ancestors
NPError
_setvalue(NPP npp, NPPVariable variable, void *result)
{
=======
NPError _setvalue(NPP npp, NPPVariable variable, void* result) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_setvalue called from the wrong thread\n"));
    return NPERR_INVALID_PARAM;
  }
<<<<<<< HEAD
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL, ("NPN_SetValue: npp=%p, var=%d\n",
                                     (void *)npp, (int)variable));
||||||| merged common ancestors
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL, ("NPN_SetValue: npp=%p, var=%d\n",
                                     (void*)npp, (int)variable));
=======
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL,
                 ("NPN_SetValue: npp=%p, var=%d\n", (void*)npp, (int)variable));
>>>>>>> upstream-releases

  if (!npp) return NPERR_INVALID_INSTANCE_ERROR;

<<<<<<< HEAD
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)npp->ndata;
||||||| merged common ancestors
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *) npp->ndata;
=======
  nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)npp->ndata;
>>>>>>> upstream-releases

  NS_ASSERTION(inst, "null instance");

  if (!inst) return NPERR_INVALID_INSTANCE_ERROR;

  PluginDestructionGuard guard(inst);

  // Cast NPNVariable enum to int to avoid warnings about including switch
  // cases for android_npapi.h's non-standard ANPInterface values.
  switch (static_cast<int>(variable)) {
    // we should keep backward compatibility with NPAPI where the
    // actual pointer value is checked rather than its content
    // when passing booleans
    case NPPVpluginWindowBool: {
#ifdef XP_MACOSX
      // This setting doesn't apply to OS X (only to Windows and Unix/Linux).
      // See https://developer.mozilla.org/En/NPN_SetValue#section_5.  Return
      // NPERR_NO_ERROR here to conform to other browsers' behavior on OS X
      // (e.g. Safari and Opera).
      return NPERR_NO_ERROR;
#else
      NPBool bWindowless = (result == nullptr);
      return inst->SetWindowless(bWindowless);
#endif
    }
    case NPPVpluginTransparentBool: {
      NPBool bTransparent = (result != nullptr);
      return inst->SetTransparent(bTransparent);
    }

    case NPPVjavascriptPushCallerBool: {
      return NPERR_NO_ERROR;
    }

    case NPPVpluginKeepLibraryInMemory: {
      NPBool bCached = (result != nullptr);
      inst->SetCached(bCached);
      return NPERR_NO_ERROR;
    }

    case NPPVpluginUsesDOMForCursorBool: {
      bool useDOMForCursor = (result != nullptr);
      return inst->SetUsesDOMForCursor(useDOMForCursor);
    }

    case NPPVpluginIsPlayingAudio: {
      const bool isPlaying = result;

<<<<<<< HEAD
      nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)npp->ndata;
||||||| merged common ancestors
      nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*) npp->ndata;
=======
      nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)npp->ndata;
>>>>>>> upstream-releases
      MOZ_ASSERT(inst);

      if (!isPlaying && !inst->HasAudioChannelAgent()) {
        return NPERR_NO_ERROR;
      }

      if (isPlaying) {
        inst->NotifyStartedPlaying();
      } else {
        inst->NotifyStoppedPlaying();
      }

      return NPERR_NO_ERROR;
    }

    case NPPVpluginDrawingModel: {
      if (inst) {
        inst->SetDrawingModel((NPDrawingModel)NS_PTR_TO_INT32(result));
        return NPERR_NO_ERROR;
      }
      return NPERR_GENERIC_ERROR;
    }

#ifdef XP_MACOSX
    case NPPVpluginEventModel: {
      if (inst) {
        inst->SetEventModel((NPEventModel)NS_PTR_TO_INT32(result));
        return NPERR_NO_ERROR;
      } else {
        return NPERR_GENERIC_ERROR;
      }
    }
#endif
    default:
      return NPERR_GENERIC_ERROR;
  }
}

<<<<<<< HEAD
NPError _requestread(NPStream *pstream, NPByteRange *rangeList) {
||||||| merged common ancestors
NPError
_requestread(NPStream *pstream, NPByteRange *rangeList)
{
=======
NPError _requestread(NPStream* pstream, NPByteRange* rangeList) {
>>>>>>> upstream-releases
  return NPERR_STREAM_NOT_SEEKABLE;
}

// Deprecated, only stubbed out
<<<<<<< HEAD
void * /* OJI type: JRIEnv* */
_getJavaEnv() {
||||||| merged common ancestors
void* /* OJI type: JRIEnv* */
_getJavaEnv()
{
=======
void* /* OJI type: JRIEnv* */
_getJavaEnv() {
>>>>>>> upstream-releases
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL, ("NPN_GetJavaEnv\n"));
  return nullptr;
}

<<<<<<< HEAD
const char *_useragent(NPP npp) {
||||||| merged common ancestors
const char *
_useragent(NPP npp)
{
=======
const char* _useragent(NPP npp) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_useragent called from the wrong thread\n"));
    return nullptr;
  }
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL, ("NPN_UserAgent: npp=%p\n", (void *)npp));

<<<<<<< HEAD
  nsCOMPtr<nsIPluginHost> pluginHostCOM(
      do_GetService(MOZ_PLUGIN_HOST_CONTRACTID));
  nsPluginHost *pluginHost = static_cast<nsPluginHost *>(pluginHostCOM.get());
||||||| merged common ancestors
  nsCOMPtr<nsIPluginHost> pluginHostCOM(do_GetService(MOZ_PLUGIN_HOST_CONTRACTID));
  nsPluginHost *pluginHost = static_cast<nsPluginHost*>(pluginHostCOM.get());
=======
  nsCOMPtr<nsIPluginHost> pluginHostCOM(
      do_GetService(MOZ_PLUGIN_HOST_CONTRACTID));
  nsPluginHost* pluginHost = static_cast<nsPluginHost*>(pluginHostCOM.get());
>>>>>>> upstream-releases
  if (!pluginHost) {
    return nullptr;
  }

  const char* retstr;
  nsresult rv = pluginHost->UserAgent(&retstr);
  if (NS_FAILED(rv)) return nullptr;

  return retstr;
}

<<<<<<< HEAD
void *_memalloc(uint32_t size) {
||||||| merged common ancestors
void *
_memalloc (uint32_t size)
{
=======
void* _memalloc(uint32_t size) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL,
                   ("NPN_memalloc called from the wrong thread\n"));
  }
  NPN_PLUGIN_LOG(PLUGIN_LOG_NOISY, ("NPN_MemAlloc: size=%d\n", size));
  return moz_xmalloc(size);
}

// Deprecated, only stubbed out
<<<<<<< HEAD
void * /* OJI type: jref */
_getJavaPeer(NPP npp) {
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL, ("NPN_GetJavaPeer: npp=%p\n", (void *)npp));
||||||| merged common ancestors
void* /* OJI type: jref */
_getJavaPeer(NPP npp)
{
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL, ("NPN_GetJavaPeer: npp=%p\n", (void*)npp));
=======
void* /* OJI type: jref */
_getJavaPeer(NPP npp) {
  NPN_PLUGIN_LOG(PLUGIN_LOG_NORMAL, ("NPN_GetJavaPeer: npp=%p\n", (void*)npp));
>>>>>>> upstream-releases
  return nullptr;
}

void _pushpopupsenabledstate(NPP npp, NPBool enabled) {
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(
        PLUGIN_LOG_ALWAYS,
        ("NPN_pushpopupsenabledstate called from the wrong thread\n"));
    return;
  }
<<<<<<< HEAD
  nsNPAPIPluginInstance *inst =
      npp ? (nsNPAPIPluginInstance *)npp->ndata : nullptr;
  if (!inst) return;
||||||| merged common ancestors
  nsNPAPIPluginInstance *inst = npp ? (nsNPAPIPluginInstance *)npp->ndata : nullptr;
  if (!inst)
    return;
=======
  nsNPAPIPluginInstance* inst =
      npp ? (nsNPAPIPluginInstance*)npp->ndata : nullptr;
  if (!inst) return;
>>>>>>> upstream-releases

  inst->PushPopupsEnabledState(enabled);
}

void _poppopupsenabledstate(NPP npp) {
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(
        PLUGIN_LOG_ALWAYS,
        ("NPN_poppopupsenabledstate called from the wrong thread\n"));
    return;
  }
<<<<<<< HEAD
  nsNPAPIPluginInstance *inst =
      npp ? (nsNPAPIPluginInstance *)npp->ndata : nullptr;
  if (!inst) return;
||||||| merged common ancestors
  nsNPAPIPluginInstance *inst = npp ? (nsNPAPIPluginInstance *)npp->ndata : nullptr;
  if (!inst)
    return;
=======
  nsNPAPIPluginInstance* inst =
      npp ? (nsNPAPIPluginInstance*)npp->ndata : nullptr;
  if (!inst) return;
>>>>>>> upstream-releases

  inst->PopPopupsEnabledState();
}

<<<<<<< HEAD
NPError _getvalueforurl(NPP instance, NPNURLVariable variable, const char *url,
                        char **value, uint32_t *len) {
||||||| merged common ancestors
NPError
_getvalueforurl(NPP instance, NPNURLVariable variable, const char *url,
                char **value, uint32_t *len)
{
=======
NPError _getvalueforurl(NPP instance, NPNURLVariable variable, const char* url,
                        char** value, uint32_t* len) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_getvalueforurl called from the wrong thread\n"));
    return NPERR_GENERIC_ERROR;
  }

  if (!instance) {
    return NPERR_INVALID_PARAM;
  }

  if (!url || !*url || !len) {
    return NPERR_INVALID_URL;
  }

  *len = 0;

  switch (variable) {
    case NPNURLVProxy:
      // NPNURLVProxy is no longer supported.
      *value = nullptr;
      return NPERR_GENERIC_ERROR;

    case NPNURLVCookie:
      // NPNURLVCookie is no longer supported.
      *value = nullptr;
      return NPERR_GENERIC_ERROR;

    default:
        // Fall through and return an error...
        ;
  }

  return NPERR_GENERIC_ERROR;
}

<<<<<<< HEAD
NPError _setvalueforurl(NPP instance, NPNURLVariable variable, const char *url,
                        const char *value, uint32_t len) {
||||||| merged common ancestors
NPError
_setvalueforurl(NPP instance, NPNURLVariable variable, const char *url,
                const char *value, uint32_t len)
{
=======
NPError _setvalueforurl(NPP instance, NPNURLVariable variable, const char* url,
                        const char* value, uint32_t len) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_setvalueforurl called from the wrong thread\n"));
    return NPERR_GENERIC_ERROR;
  }

  if (!instance) {
    return NPERR_INVALID_PARAM;
  }

  if (!url || !*url) {
    return NPERR_INVALID_URL;
  }

  switch (variable) {
    case NPNURLVCookie:
      // NPNURLVCookie is no longer supported.
      return NPERR_GENERIC_ERROR;

    case NPNURLVProxy:
      // We don't support setting proxy values, fall through...
    default:
        // Fall through and return an error...
        ;
  }

  return NPERR_GENERIC_ERROR;
}

uint32_t _scheduletimer(NPP instance, uint32_t interval, NPBool repeat,
                        PluginTimerFunc timerFunc) {
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_scheduletimer called from the wrong thread\n"));
    return 0;
  }

<<<<<<< HEAD
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)instance->ndata;
  if (!inst) return 0;
||||||| merged common ancestors
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)instance->ndata;
  if (!inst)
    return 0;
=======
  nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)instance->ndata;
  if (!inst) return 0;
>>>>>>> upstream-releases

  return inst->ScheduleTimer(interval, repeat, timerFunc);
}

void _unscheduletimer(NPP instance, uint32_t timerID) {
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_unscheduletimer called from the wrong thread\n"));
    return;
  }

<<<<<<< HEAD
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)instance->ndata;
  if (!inst) return;
||||||| merged common ancestors
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)instance->ndata;
  if (!inst)
    return;
=======
  nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)instance->ndata;
  if (!inst) return;
>>>>>>> upstream-releases

  inst->UnscheduleTimer(timerID);
}

<<<<<<< HEAD
NPError _popupcontextmenu(NPP instance, NPMenu *menu) {
||||||| merged common ancestors
NPError
_popupcontextmenu(NPP instance, NPMenu* menu)
{
=======
NPError _popupcontextmenu(NPP instance, NPMenu* menu) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_popupcontextmenu called from the wrong thread\n"));
    return 0;
  }

#ifdef MOZ_WIDGET_COCOA
  nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)instance->ndata;

  double pluginX, pluginY;
  double screenX, screenY;

<<<<<<< HEAD
  const NPCocoaEvent *currentEvent =
      static_cast<NPCocoaEvent *>(inst->GetCurrentEvent());
||||||| merged common ancestors
  const NPCocoaEvent* currentEvent = static_cast<NPCocoaEvent*>(inst->GetCurrentEvent());
=======
  const NPCocoaEvent* currentEvent =
      static_cast<NPCocoaEvent*>(inst->GetCurrentEvent());
>>>>>>> upstream-releases
  if (!currentEvent) {
    return NPERR_GENERIC_ERROR;
  }

  // Ensure that the events has an x/y value.
  if (currentEvent->type != NPCocoaEventMouseDown &&
      currentEvent->type != NPCocoaEventMouseUp &&
      currentEvent->type != NPCocoaEventMouseMoved &&
      currentEvent->type != NPCocoaEventMouseEntered &&
      currentEvent->type != NPCocoaEventMouseExited &&
      currentEvent->type != NPCocoaEventMouseDragged) {
    return NPERR_GENERIC_ERROR;
  }

  pluginX = currentEvent->data.mouse.pluginX;
  pluginY = currentEvent->data.mouse.pluginY;

  if ((pluginX < 0.0) || (pluginY < 0.0)) return NPERR_GENERIC_ERROR;

  NPBool success =
      _convertpoint(instance, pluginX, pluginY, NPCoordinateSpacePlugin,
                    &screenX, &screenY, NPCoordinateSpaceScreen);

  if (success) {
    return mozilla::plugins::PluginUtilsOSX::ShowCocoaContextMenu(
        menu, screenX, screenY, nullptr, nullptr);
  } else {
    NS_WARNING("Convertpoint failed, could not created contextmenu.");
    return NPERR_GENERIC_ERROR;
  }
#else
  NS_WARNING("Not supported on this platform!");
  return NPERR_GENERIC_ERROR;
#endif
}

<<<<<<< HEAD
NPBool _convertpoint(NPP instance, double sourceX, double sourceY,
                     NPCoordinateSpace sourceSpace, double *destX,
                     double *destY, NPCoordinateSpace destSpace) {
||||||| merged common ancestors
NPBool
_convertpoint(NPP instance, double sourceX, double sourceY, NPCoordinateSpace sourceSpace, double *destX, double *destY, NPCoordinateSpace destSpace)
{
=======
NPBool _convertpoint(NPP instance, double sourceX, double sourceY,
                     NPCoordinateSpace sourceSpace, double* destX,
                     double* destY, NPCoordinateSpace destSpace) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_convertpoint called from the wrong thread\n"));
    return 0;
  }

<<<<<<< HEAD
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)instance->ndata;
  if (!inst) return false;
||||||| merged common ancestors
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)instance->ndata;
  if (!inst)
    return false;
=======
  nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)instance->ndata;
  if (!inst) return false;
>>>>>>> upstream-releases

  return inst->ConvertPoint(sourceX, sourceY, sourceSpace, destX, destY,
                            destSpace);
}

<<<<<<< HEAD
void _urlredirectresponse(NPP instance, void *notifyData, NPBool allow) {
||||||| merged common ancestors
void
_urlredirectresponse(NPP instance, void* notifyData, NPBool allow)
{
=======
void _urlredirectresponse(NPP instance, void* notifyData, NPBool allow) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    NPN_PLUGIN_LOG(PLUGIN_LOG_ALWAYS,
                   ("NPN_convertpoint called from the wrong thread\n"));
    return;
  }

  nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)instance->ndata;
  if (!inst) {
    return;
  }

  inst->URLRedirectResponse(notifyData, allow);
}

<<<<<<< HEAD
NPError _initasyncsurface(NPP instance, NPSize *size, NPImageFormat format,
                          void *initData, NPAsyncSurface *surface) {
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)instance->ndata;
||||||| merged common ancestors
NPError
_initasyncsurface(NPP instance, NPSize *size, NPImageFormat format, void *initData, NPAsyncSurface *surface)
{
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)instance->ndata;
=======
NPError _initasyncsurface(NPP instance, NPSize* size, NPImageFormat format,
                          void* initData, NPAsyncSurface* surface) {
  nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)instance->ndata;
>>>>>>> upstream-releases
  if (!inst) {
    return NPERR_GENERIC_ERROR;
  }

  return inst->InitAsyncSurface(size, format, initData, surface);
}

<<<<<<< HEAD
NPError _finalizeasyncsurface(NPP instance, NPAsyncSurface *surface) {
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)instance->ndata;
||||||| merged common ancestors
NPError
_finalizeasyncsurface(NPP instance, NPAsyncSurface *surface)
{
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)instance->ndata;
=======
NPError _finalizeasyncsurface(NPP instance, NPAsyncSurface* surface) {
  nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)instance->ndata;
>>>>>>> upstream-releases
  if (!inst) {
    return NPERR_GENERIC_ERROR;
  }

  return inst->FinalizeAsyncSurface(surface);
}

<<<<<<< HEAD
void _setcurrentasyncsurface(NPP instance, NPAsyncSurface *surface,
                             NPRect *changed) {
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)instance->ndata;
||||||| merged common ancestors
void
_setcurrentasyncsurface(NPP instance, NPAsyncSurface *surface, NPRect *changed)
{
  nsNPAPIPluginInstance *inst = (nsNPAPIPluginInstance *)instance->ndata;
=======
void _setcurrentasyncsurface(NPP instance, NPAsyncSurface* surface,
                             NPRect* changed) {
  nsNPAPIPluginInstance* inst = (nsNPAPIPluginInstance*)instance->ndata;
>>>>>>> upstream-releases
  if (!inst) {
    return;
  }

  inst->SetCurrentAsyncSurface(surface, changed);
}

} /* namespace parent */
} /* namespace plugins */
} /* namespace mozilla */
