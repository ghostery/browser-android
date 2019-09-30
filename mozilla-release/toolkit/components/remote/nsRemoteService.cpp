/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:expandtab:shiftwidth=2:tabstop=8:
 */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifdef XP_UNIX
#  include <sys/types.h>
#  include <pwd.h>
#endif

#ifdef MOZ_WIDGET_GTK
#  include "nsGTKRemoteServer.h"
#  include "nsXRemoteClient.h"
#  ifdef MOZ_ENABLE_DBUS
#    include "nsDBusRemoteServer.h"
#    include "nsDBusRemoteClient.h"
#  endif
#elif defined(XP_WIN)
#  include "nsWinRemoteServer.h"
#  include "nsWinRemoteClient.h"
#endif
#include "nsRemoteService.h"

<<<<<<< HEAD
#include "nsIServiceManager.h"
#include "nsIAppShellService.h"
#include "nsAppShellCID.h"
#include "nsInterfaceHashtable.h"
#include "mozilla/ModuleUtils.h"
#include "nsGTKToolkit.h"
#include "nsICommandLineRunner.h"
#include "nsCommandLine.h"
||||||| merged common ancestors
#include "nsIServiceManager.h"
#include "nsIAppShellService.h"
#include "nsAppShellCID.h"
#include "nsInterfaceHashtable.h"
#include "mozilla/ModuleUtils.h"
#include "nsIWeakReference.h"
#include "nsGTKToolkit.h"
#include "nsICommandLineRunner.h"
#include "nsCommandLine.h"
=======
#include "nsAutoPtr.h"
>>>>>>> upstream-releases
#include "nsString.h"
#include "nsServiceManagerUtils.h"
#include "mozilla/ModuleUtils.h"
#include "SpecialSystemDirectory.h"
#include "mozilla/CmdLineAndEnvUtils.h"

<<<<<<< HEAD
NS_IMPL_ISUPPORTS(nsRemoteService, nsIRemoteService, nsIObserver)
||||||| merged common ancestors
NS_IMPL_ISUPPORTS(nsRemoteService,
                  nsIRemoteService,
                  nsIObserver)
=======
// Time to wait for the remoting service to start
#define START_TIMEOUT_SEC 5
#define START_SLEEP_MSEC 100
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
nsRemoteService::Startup(const char* aAppName, const char* aProfileName) {
  bool useX11Remote = GDK_IS_X11_DISPLAY(gdk_display_get_default());

#if defined(MOZ_ENABLE_DBUS)
  if (!useX11Remote) {
    nsresult rv;
    mDBusRemoteService = new nsDBusRemoteService();
    rv = mDBusRemoteService->Startup(aAppName, aProfileName);
    if (NS_FAILED(rv)) {
      mDBusRemoteService = nullptr;
    }
  }
#endif
  if (useX11Remote) {
    mGtkRemoteService = new nsGTKRemoteService();
    mGtkRemoteService->Startup(aAppName, aProfileName);
  }
||||||| merged common ancestors
NS_IMETHODIMP
nsRemoteService::Startup(const char* aAppName, const char* aProfileName)
{
#if defined(MOZ_ENABLE_DBUS) && defined(MOZ_WAYLAND)
    nsresult rv;
    mDBusRemoteService = new nsDBusRemoteService();
    rv = mDBusRemoteService->Startup(aAppName, aProfileName);
    if (NS_FAILED(rv)) {
        mDBusRemoteService = nullptr;
    }
#elif !defined(MOZ_WAYLAND)
    mGtkRemoteService = new nsGTKRemoteService();
    mGtkRemoteService->Startup(aAppName, aProfileName);
#endif
=======
using namespace mozilla;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!mDBusRemoteService && !mGtkRemoteService) return NS_ERROR_FAILURE;
||||||| merged common ancestors
    if (!mDBusRemoteService && !mGtkRemoteService)
        return NS_ERROR_FAILURE;
=======
extern int gArgc;
extern char** gArgv;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIObserverService> obs(
      do_GetService("@mozilla.org/observer-service;1"));
  if (obs) {
    obs->AddObserver(this, "xpcom-shutdown", false);
    obs->AddObserver(this, "quit-application", false);
  }
||||||| merged common ancestors
    nsCOMPtr<nsIObserverService> obs(do_GetService("@mozilla.org/observer-service;1"));
    if (obs) {
        obs->AddObserver(this, "xpcom-shutdown", false);
        obs->AddObserver(this, "quit-application", false);
    }
=======
using namespace mozilla;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return NS_OK;
}
||||||| merged common ancestors
    return NS_OK;
}
=======
NS_IMPL_ISUPPORTS(nsRemoteService, nsIObserver)
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
nsRemoteService::RegisterWindow(mozIDOMWindow* aWindow) {
  // Note: RegisterWindow() is not implemented/needed by DBus service.
  if (mGtkRemoteService) {
    mGtkRemoteService->RegisterWindow(aWindow);
  }
  return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
nsRemoteService::RegisterWindow(mozIDOMWindow* aWindow)
{
    // Note: RegisterWindow() is not implemented/needed by DBus service.
    if (mGtkRemoteService) {
        mGtkRemoteService->RegisterWindow(aWindow);
    }
    return NS_OK;
=======
nsRemoteService::nsRemoteService(const char* aProgram) : mProgram(aProgram) {
  ToLowerCase(mProgram);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
nsRemoteService::Shutdown() {
#if defined(MOZ_ENABLE_DBUS)
  if (mDBusRemoteService) {
    mDBusRemoteService->Shutdown();
    mDBusRemoteService = nullptr;
  }
#endif
  if (mGtkRemoteService) {
    mGtkRemoteService->Shutdown();
    mGtkRemoteService = nullptr;
  }
  return NS_OK;
}
||||||| merged common ancestors
NS_IMETHODIMP
nsRemoteService::Shutdown()
{
#if defined(MOZ_ENABLE_DBUS) && defined(MOZ_WAYLAND)
    if (mDBusRemoteService) {
        mDBusRemoteService->Shutdown();
        mDBusRemoteService = nullptr;
    }
#endif
    if (mGtkRemoteService) {
        mGtkRemoteService->Shutdown();
        mGtkRemoteService = nullptr;
    }
    return NS_OK;
}
=======
void nsRemoteService::SetProfile(nsACString& aProfile) { mProfile = aProfile; }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsRemoteService::~nsRemoteService() { Shutdown(); }
||||||| merged common ancestors
nsRemoteService::~nsRemoteService()
{
    Shutdown();
}
=======
void nsRemoteService::LockStartup() {
  nsCOMPtr<nsIFile> mutexDir;
  nsresult rv = GetSpecialSystemDirectory(OS_TemporaryDirectory,
                                          getter_AddRefs(mutexDir));
  if (NS_SUCCEEDED(rv)) {
    mutexDir->AppendNative(mProgram);
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
nsRemoteService::Observe(nsISupports* aSubject, const char* aTopic,
                         const char16_t* aData) {
  // This can be xpcom-shutdown or quit-application, but it's the same either
  // way.
  Shutdown();
  return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
nsRemoteService::Observe(nsISupports* aSubject,
                          const char *aTopic,
                          const char16_t *aData)
{
    // This can be xpcom-shutdown or quit-application, but it's the same either
    // way.
    Shutdown();
    return NS_OK;
=======
    rv = mutexDir->Create(nsIFile::DIRECTORY_TYPE, 0700);
    if (NS_SUCCEEDED(rv) || rv == NS_ERROR_FILE_ALREADY_EXISTS) {
      mRemoteLockDir = mutexDir;
    }
  }

  if (mRemoteLockDir) {
    const mozilla::TimeStamp epoch = mozilla::TimeStamp::Now();
    do {
      rv = mRemoteLock.Lock(mRemoteLockDir, nullptr);
      if (NS_SUCCEEDED(rv)) break;
      PR_Sleep(START_SLEEP_MSEC);
    } while ((mozilla::TimeStamp::Now() - epoch) <
             mozilla::TimeDuration::FromSeconds(START_TIMEOUT_SEC));
    if (NS_FAILED(rv)) {
      NS_WARNING("Cannot lock remote start mutex");
    }
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Set desktop startup ID to the passed ID, if there is one, so that any created
// windows get created with the right window manager metadata, and any windows
// that get new tabs and are activated also get the right WM metadata.
// The timestamp will be used if there is no desktop startup ID, or if we're
// raising an existing window rather than showing a new window for the first
// time.
void nsRemoteService::SetDesktopStartupIDOrTimestamp(
    const nsACString& aDesktopStartupID, uint32_t aTimestamp) {
  nsGTKToolkit* toolkit = nsGTKToolkit::GetToolkit();
  if (!toolkit) return;
||||||| merged common ancestors
// Set desktop startup ID to the passed ID, if there is one, so that any created
// windows get created with the right window manager metadata, and any windows
// that get new tabs and are activated also get the right WM metadata.
// The timestamp will be used if there is no desktop startup ID, or if we're
// raising an existing window rather than showing a new window for the first time.
void
nsRemoteService::SetDesktopStartupIDOrTimestamp(const nsACString& aDesktopStartupID,
                                                uint32_t aTimestamp) {
  nsGTKToolkit* toolkit = nsGTKToolkit::GetToolkit();
  if (!toolkit)
    return;
=======
void nsRemoteService::UnlockStartup() {
  if (mRemoteLockDir) {
    mRemoteLock.Unlock();
    mRemoteLock.Cleanup();
>>>>>>> upstream-releases

    mRemoteLockDir->Remove(false);
    mRemoteLockDir = nullptr;
  }
}

<<<<<<< HEAD
static bool FindExtensionParameterInCommand(const char* aParameterName,
                                            const nsACString& aCommand,
                                            char aSeparator,
                                            nsACString* aValue) {
  nsAutoCString searchFor;
  searchFor.Append(aSeparator);
  searchFor.Append(aParameterName);
  searchFor.Append('=');

  nsACString::const_iterator start, end;
  aCommand.BeginReading(start);
  aCommand.EndReading(end);
  if (!FindInReadable(searchFor, start, end)) return false;

  nsACString::const_iterator charStart, charEnd;
  charStart = end;
  aCommand.EndReading(charEnd);
  nsACString::const_iterator idStart = charStart, idEnd;
  if (FindCharInReadable(aSeparator, charStart, charEnd)) {
    idEnd = charStart;
  } else {
    idEnd = charEnd;
||||||| merged common ancestors
static bool
FindExtensionParameterInCommand(const char* aParameterName,
                                const nsACString& aCommand,
                                char aSeparator,
                                nsACString* aValue)
{
  nsAutoCString searchFor;
  searchFor.Append(aSeparator);
  searchFor.Append(aParameterName);
  searchFor.Append('=');

  nsACString::const_iterator start, end;
  aCommand.BeginReading(start);
  aCommand.EndReading(end);
  if (!FindInReadable(searchFor, start, end))
    return false;

  nsACString::const_iterator charStart, charEnd;
  charStart = end;
  aCommand.EndReading(charEnd);
  nsACString::const_iterator idStart = charStart, idEnd;
  if (FindCharInReadable(aSeparator, charStart, charEnd)) {
    idEnd = charStart;
  } else {
    idEnd = charEnd;
=======
RemoteResult nsRemoteService::StartClient(const char* aDesktopStartupID) {
  if (mProfile.IsEmpty()) {
    return REMOTE_NOT_FOUND;
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
const char* nsRemoteService::HandleCommandLine(const char* aBuffer,
                                               nsIDOMWindow* aWindow,
                                               uint32_t aTimestamp) {
  nsCOMPtr<nsICommandLineRunner> cmdline(new nsCommandLine());
||||||| merged common ancestors
const char*
nsRemoteService::HandleCommandLine(const char* aBuffer, nsIDOMWindow* aWindow,
                                   uint32_t aTimestamp)
{
  nsCOMPtr<nsICommandLineRunner> cmdline(new nsCommandLine());
=======
  nsAutoPtr<nsRemoteClient> client;
>>>>>>> upstream-releases

#ifdef MOZ_WIDGET_GTK
  bool useX11Remote = GDK_IS_X11_DISPLAY(gdk_display_get_default());

<<<<<<< HEAD
  int32_t argc = TO_LITTLE_ENDIAN32(*reinterpret_cast<const int32_t*>(aBuffer));
  const char* wd = aBuffer + ((argc + 1) * sizeof(int32_t));
||||||| merged common ancestors
  int32_t argc = TO_LITTLE_ENDIAN32(*reinterpret_cast<const int32_t*>(aBuffer));
  const char *wd   = aBuffer + ((argc + 1) * sizeof(int32_t));
=======
#  if defined(MOZ_ENABLE_DBUS)
  if (!useX11Remote) {
    client = new nsDBusRemoteClient();
  }
#  endif
  if (useX11Remote) {
    client = new nsXRemoteClient();
  }
#elif defined(XP_WIN)
  client = new nsWinRemoteClient();
#else
  return REMOTE_NOT_FOUND;
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIFile> lf;
  nsresult rv =
      NS_NewNativeLocalFile(nsDependentCString(wd), true, getter_AddRefs(lf));
  if (NS_FAILED(rv)) return "509 internal error";
||||||| merged common ancestors
  nsCOMPtr<nsIFile> lf;
  nsresult rv = NS_NewNativeLocalFile(nsDependentCString(wd), true,
                                      getter_AddRefs(lf));
  if (NS_FAILED(rv))
    return "509 internal error";
=======
  nsresult rv = client ? client->Init() : NS_ERROR_FAILURE;
  if (NS_FAILED(rv)) return REMOTE_NOT_FOUND;
>>>>>>> upstream-releases

  nsCString response;
  bool success = false;
  rv = client->SendCommandLine(mProgram.get(), mProfile.get(), gArgc, gArgv,
                               aDesktopStartupID, getter_Copies(response),
                               &success);
  // did the command fail?
  if (!success) return REMOTE_NOT_FOUND;

<<<<<<< HEAD
  const char** argv = (const char**)malloc(sizeof(char*) * argc);
  if (!argv) return "509 internal error";
||||||| merged common ancestors
  const char **argv = (const char**) malloc(sizeof(char*) * argc);
  if (!argv) return "509 internal error";
=======
  // The "command not parseable" error is returned when the
  // nsICommandLineHandler throws a NS_ERROR_ABORT.
  if (response.EqualsLiteral("500 command not parseable"))
    return REMOTE_ARG_BAD;
>>>>>>> upstream-releases

<<<<<<< HEAD
  const int32_t* offset = reinterpret_cast<const int32_t*>(aBuffer) + 1;
||||||| merged common ancestors
  const int32_t *offset = reinterpret_cast<const int32_t*>(aBuffer) + 1;
=======
  if (NS_FAILED(rv)) return REMOTE_NOT_FOUND;
>>>>>>> upstream-releases

  return REMOTE_FOUND;
}

<<<<<<< HEAD
    if (i == 0) {
      nsDependentCString cmd(argv[0]);
      FindExtensionParameterInCommand("DESKTOP_STARTUP_ID", cmd, ' ',
                                      &desktopStartupID);
    }
||||||| merged common ancestors
    if (i == 0) {
      nsDependentCString cmd(argv[0]);
      FindExtensionParameterInCommand("DESKTOP_STARTUP_ID",
                                      cmd, ' ',
                                      &desktopStartupID);
    }
=======
void nsRemoteService::StartupServer() {
  if (mRemoteServer) {
    return;
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  rv = cmdline->Init(argc, argv, lf, nsICommandLine::STATE_REMOTE_AUTO);

  free(argv);
  if (NS_FAILED(rv)) {
    return "509 internal error";
||||||| merged common ancestors
  rv = cmdline->Init(argc, argv, lf, nsICommandLine::STATE_REMOTE_AUTO);

  free (argv);
  if (NS_FAILED(rv)) {
    return "509 internal error";
=======
  if (mProfile.IsEmpty()) {
    return;
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  if (aWindow) cmdline->SetWindowContext(aWindow);

  SetDesktopStartupIDOrTimestamp(desktopStartupID, aTimestamp);
||||||| merged common ancestors
  if (aWindow)
    cmdline->SetWindowContext(aWindow);

  SetDesktopStartupIDOrTimestamp(desktopStartupID, aTimestamp);
=======
#ifdef MOZ_WIDGET_GTK
  bool useX11Remote = GDK_IS_X11_DISPLAY(gdk_display_get_default());
>>>>>>> upstream-releases

#  if defined(MOZ_ENABLE_DBUS)
  if (!useX11Remote) {
    mRemoteServer = MakeUnique<nsDBusRemoteServer>();
  }
#  endif
  if (useX11Remote) {
    mRemoteServer = MakeUnique<nsGTKRemoteServer>();
  }
#elif defined(XP_WIN)
  mRemoteServer = MakeUnique<nsWinRemoteServer>();
#else
  return;
#endif

<<<<<<< HEAD
  if (NS_ERROR_ABORT == rv) return "500 command not parseable";
||||||| merged common ancestors
  if (NS_ERROR_ABORT == rv)
    return "500 command not parseable";
=======
  nsresult rv = mRemoteServer->Startup(mProgram.get(), mProfile.get());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (NS_FAILED(rv)) return "509 internal error";
||||||| merged common ancestors
  if (NS_FAILED(rv))
    return "509 internal error";
=======
  if (NS_FAILED(rv)) {
    mRemoteServer = nullptr;
    return;
  }
>>>>>>> upstream-releases

  nsCOMPtr<nsIObserverService> obs(
      do_GetService("@mozilla.org/observer-service;1"));
  if (obs) {
    obs->AddObserver(this, "xpcom-shutdown", false);
    obs->AddObserver(this, "quit-application", false);
  }
}

<<<<<<< HEAD
// {C0773E90-5799-4eff-AD03-3EBCD85624AC}
#define NS_REMOTESERVICE_CID                        \
  {                                                 \
    0xc0773e90, 0x5799, 0x4eff, {                   \
      0xad, 0x3, 0x3e, 0xbc, 0xd8, 0x56, 0x24, 0xac \
    }                                               \
  }
||||||| merged common ancestors
// {C0773E90-5799-4eff-AD03-3EBCD85624AC}
#define NS_REMOTESERVICE_CID \
  { 0xc0773e90, 0x5799, 0x4eff, { 0xad, 0x3, 0x3e, 0xbc, 0xd8, 0x56, 0x24, 0xac } }
=======
void nsRemoteService::ShutdownServer() { mRemoteServer = nullptr; }
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_GENERIC_FACTORY_CONSTRUCTOR(nsRemoteService)
NS_DEFINE_NAMED_CID(NS_REMOTESERVICE_CID);

static const mozilla::Module::CIDEntry kRemoteCIDs[] = {
    {&kNS_REMOTESERVICE_CID, false, nullptr, nsRemoteServiceConstructor},
    {nullptr}};

static const mozilla::Module::ContractIDEntry kRemoteContracts[] = {
    {"@mozilla.org/toolkit/remote-service;1", &kNS_REMOTESERVICE_CID},
    {nullptr}};

static const mozilla::Module kRemoteModule = {mozilla::Module::kVersion,
                                              kRemoteCIDs, kRemoteContracts};
||||||| merged common ancestors
NS_GENERIC_FACTORY_CONSTRUCTOR(nsRemoteService)
NS_DEFINE_NAMED_CID(NS_REMOTESERVICE_CID);

static const mozilla::Module::CIDEntry kRemoteCIDs[] = {
  { &kNS_REMOTESERVICE_CID, false, nullptr, nsRemoteServiceConstructor },
  { nullptr }
};

static const mozilla::Module::ContractIDEntry kRemoteContracts[] = {
  { "@mozilla.org/toolkit/remote-service;1", &kNS_REMOTESERVICE_CID },
  { nullptr }
};

static const mozilla::Module kRemoteModule = {
  mozilla::Module::kVersion,
  kRemoteCIDs,
  kRemoteContracts
};
=======
nsRemoteService::~nsRemoteService() {
  UnlockStartup();
  ShutdownServer();
}
>>>>>>> upstream-releases

NS_IMETHODIMP
nsRemoteService::Observe(nsISupports* aSubject, const char* aTopic,
                         const char16_t* aData) {
  // This can be xpcom-shutdown or quit-application, but it's the same either
  // way.
  ShutdownServer();
  return NS_OK;
}
