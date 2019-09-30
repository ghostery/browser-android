/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "LauncherProcessWin.h"

<<<<<<< HEAD
#include <io.h>  // For printf_stderr
||||||| merged common ancestors
#include <io.h> // For printf_stderr
=======
>>>>>>> upstream-releases
#include <string.h>

#include "mozilla/Attributes.h"
#include "mozilla/CmdLineAndEnvUtils.h"
#include "mozilla/DebugOnly.h"
#include "mozilla/DynamicallyLinkedFunctionPtr.h"
#include "mozilla/glue/Debug.h"
#include "mozilla/LauncherResult.h"
#include "mozilla/Maybe.h"
#include "mozilla/SafeMode.h"
<<<<<<< HEAD
#include "mozilla/Sprintf.h"  // For printf_stderr
||||||| merged common ancestors
#include "mozilla/Sprintf.h" // For printf_stderr
=======
>>>>>>> upstream-releases
#include "mozilla/UniquePtr.h"
#include "mozilla/WindowsConsole.h"
#include "mozilla/WindowsVersion.h"
#include "mozilla/WinHeaderOnlyUtils.h"
#include "nsWindowsHelpers.h"

#include <windows.h>
#include <processthreadsapi.h>

#include "DllBlocklistWin.h"
<<<<<<< HEAD
#include "ErrorHandler.h"
#include "LauncherResult.h"
||||||| merged common ancestors
=======
#include "ErrorHandler.h"
>>>>>>> upstream-releases
#include "LaunchUnelevated.h"
#include "ProcThreadAttributes.h"

#if defined(MOZ_LAUNCHER_PROCESS)
#  include "mozilla/LauncherRegistryInfo.h"
#  include "SameBinary.h"
#endif  // defined(MOZ_LAUNCHER_PROCESS)

/**
 * At this point the child process has been created in a suspended state. Any
 * additional startup work (eg, blocklist setup) should go here.
 *
 * @return Ok if browser startup should proceed
 */
<<<<<<< HEAD
static mozilla::LauncherVoidResult PostCreationSetup(HANDLE aChildProcess,
                                                     HANDLE aChildMainThread,
                                                     const bool aIsSafeMode) {
||||||| merged common ancestors
static bool
PostCreationSetup(HANDLE aChildProcess, HANDLE aChildMainThread,
                  const bool aIsSafeMode)
{
=======
static mozilla::LauncherVoidResult PostCreationSetup(
    const wchar_t* aFullImagePath, HANDLE aChildProcess,
    HANDLE aChildMainThread, const bool aIsSafeMode) {
>>>>>>> upstream-releases
  // The launcher process's DLL blocking code is incompatible with ASAN because
  // it is able to execute before ASAN itself has even initialized.
  // Also, the AArch64 build doesn't yet have a working interceptor.
#if defined(MOZ_ASAN) || defined(_M_ARM64)
  return mozilla::Ok();
#else
<<<<<<< HEAD
  return mozilla::InitializeDllBlocklistOOP(aChildProcess);
#endif  // defined(MOZ_ASAN) || defined(_M_ARM64)
||||||| merged common ancestors
  return mozilla::InitializeDllBlocklistOOP(aChildProcess);
#endif // defined(MOZ_ASAN) || defined(_M_ARM64)
=======
  return mozilla::InitializeDllBlocklistOOP(aFullImagePath, aChildProcess);
#endif  // defined(MOZ_ASAN) || defined(_M_ARM64)
>>>>>>> upstream-releases
}

<<<<<<< HEAD
#if !defined( \
    PROCESS_CREATION_MITIGATION_POLICY_IMAGE_LOAD_PREFER_SYSTEM32_ALWAYS_ON)
#define PROCESS_CREATION_MITIGATION_POLICY_IMAGE_LOAD_PREFER_SYSTEM32_ALWAYS_ON \
  (0x00000001ULL << 60)
#endif  // !defined(PROCESS_CREATION_MITIGATION_POLICY_IMAGE_LOAD_PREFER_SYSTEM32_ALWAYS_ON)
||||||| merged common ancestors
#if !defined(PROCESS_CREATION_MITIGATION_POLICY_IMAGE_LOAD_PREFER_SYSTEM32_ALWAYS_ON)
# define PROCESS_CREATION_MITIGATION_POLICY_IMAGE_LOAD_PREFER_SYSTEM32_ALWAYS_ON (0x00000001ULL << 60)
#endif // !defined(PROCESS_CREATION_MITIGATION_POLICY_IMAGE_LOAD_PREFER_SYSTEM32_ALWAYS_ON)
=======
#if !defined( \
    PROCESS_CREATION_MITIGATION_POLICY_IMAGE_LOAD_PREFER_SYSTEM32_ALWAYS_ON)
#  define PROCESS_CREATION_MITIGATION_POLICY_IMAGE_LOAD_PREFER_SYSTEM32_ALWAYS_ON \
    (0x00000001ULL << 60)
#endif  // !defined(PROCESS_CREATION_MITIGATION_POLICY_IMAGE_LOAD_PREFER_SYSTEM32_ALWAYS_ON)

#if !defined(PROCESS_CREATION_MITIGATION_POLICY_CONTROL_FLOW_GUARD_ALWAYS_OFF)
#  define PROCESS_CREATION_MITIGATION_POLICY_CONTROL_FLOW_GUARD_ALWAYS_OFF \
    (0x00000002ULL << 40)
#endif  // !defined(PROCESS_CREATION_MITIGATION_POLICY_CONTROL_FLOW_GUARD_ALWAYS_OFF)
>>>>>>> upstream-releases

#if (_WIN32_WINNT < 0x0602)
BOOL WINAPI
SetProcessMitigationPolicy(PROCESS_MITIGATION_POLICY aMitigationPolicy,
                           PVOID aBuffer, SIZE_T aBufferLen);
#endif  // (_WIN32_WINNT >= 0x0602)

/**
 * Any mitigation policies that should be set on the browser process should go
 * here.
 */
static void SetMitigationPolicies(mozilla::ProcThreadAttributes& aAttrs,
                                  const bool aIsSafeMode) {
  if (mozilla::IsWin10AnniversaryUpdateOrLater()) {
<<<<<<< HEAD
    aAttrs.AddMitigationPolicy(
        PROCESS_CREATION_MITIGATION_POLICY_IMAGE_LOAD_PREFER_SYSTEM32_ALWAYS_ON);
  }
}

static mozilla::LauncherFlags ProcessCmdLine(int& aArgc, wchar_t* aArgv[]) {
||||||| merged common ancestors
    aAttrs.AddMitigationPolicy(PROCESS_CREATION_MITIGATION_POLICY_IMAGE_LOAD_PREFER_SYSTEM32_ALWAYS_ON);
  }
}

static void
ShowError(DWORD aError = ::GetLastError())
{
  if (aError == ERROR_SUCCESS) {
    return;
  }

  LPWSTR rawMsgBuf = nullptr;
  DWORD result = ::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                                  FORMAT_MESSAGE_FROM_SYSTEM |
                                  FORMAT_MESSAGE_IGNORE_INSERTS, nullptr,
                                  aError, 0, reinterpret_cast<LPWSTR>(&rawMsgBuf),
                                  0, nullptr);
  if (!result) {
    return;
  }

  ::MessageBoxW(nullptr, rawMsgBuf, L"Firefox", MB_OK | MB_ICONERROR);
  ::LocalFree(rawMsgBuf);
}

static mozilla::LauncherFlags
ProcessCmdLine(int& aArgc, wchar_t* aArgv[])
{
=======
    aAttrs.AddMitigationPolicy(
        PROCESS_CREATION_MITIGATION_POLICY_IMAGE_LOAD_PREFER_SYSTEM32_ALWAYS_ON);
  }

#if defined(_M_ARM64)
  // Disable CFG on older versions of ARM64 Windows to avoid a crash in COM.
  if (!mozilla::IsWin10Sep2018UpdateOrLater()) {
    aAttrs.AddMitigationPolicy(
        PROCESS_CREATION_MITIGATION_POLICY_CONTROL_FLOW_GUARD_ALWAYS_OFF);
  }
#endif  // defined(_M_ARM64)
}

static mozilla::LauncherFlags ProcessCmdLine(int& aArgc, wchar_t* aArgv[]) {
>>>>>>> upstream-releases
  mozilla::LauncherFlags result = mozilla::LauncherFlags::eNone;

  if (mozilla::CheckArg(aArgc, aArgv, L"wait-for-browser",
                        static_cast<const wchar_t**>(nullptr),
                        mozilla::CheckArgFlag::RemoveArg) ==
          mozilla::ARG_FOUND ||
      mozilla::CheckArg(aArgc, aArgv, L"marionette",
                        static_cast<const wchar_t**>(nullptr),
                        mozilla::CheckArgFlag::None) == mozilla::ARG_FOUND ||
      mozilla::CheckArg(aArgc, aArgv, L"headless",
                        static_cast<const wchar_t**>(nullptr),
                        mozilla::CheckArgFlag::None) == mozilla::ARG_FOUND ||
      mozilla::EnvHasValue("MOZ_AUTOMATION") ||
      mozilla::EnvHasValue("MOZ_HEADLESS")) {
    result |= mozilla::LauncherFlags::eWaitForBrowser;
  }

<<<<<<< HEAD
  if (mozilla::CheckArg(
          aArgc, aArgv, L"no-deelevate", static_cast<const wchar_t**>(nullptr),
          mozilla::CheckArgFlag::CheckOSInt |
              mozilla::CheckArgFlag::RemoveArg) == mozilla::ARG_FOUND) {
||||||| merged common ancestors
  if (mozilla::CheckArg(aArgc, aArgv, L"no-deelevate",
                        static_cast<const wchar_t**>(nullptr),
                        mozilla::CheckArgFlag::CheckOSInt |
                        mozilla::CheckArgFlag::RemoveArg) == mozilla::ARG_FOUND) {
=======
  if (mozilla::CheckArg(aArgc, aArgv, L"no-deelevate") == mozilla::ARG_FOUND) {
>>>>>>> upstream-releases
    result |= mozilla::LauncherFlags::eNoDeelevate;
  }

  return result;
}

<<<<<<< HEAD
// Duplicated from xpcom glue. Ideally this should be shared.
static void printf_stderr(const char* fmt, ...) {
  if (IsDebuggerPresent()) {
    char buf[2048];
    va_list args;
    va_start(args, fmt);
    VsprintfLiteral(buf, fmt, args);
    va_end(args);
    OutputDebugStringA(buf);
  }

  FILE* fp = _fdopen(_dup(2), "a");
  if (!fp) return;

  va_list args;
  va_start(args, fmt);
  vfprintf(fp, fmt, args);
  va_end(args);

  fclose(fp);
}

static void MaybeBreakForBrowserDebugging() {
||||||| merged common ancestors
// Duplicated from xpcom glue. Ideally this should be shared.
static void
printf_stderr(const char *fmt, ...)
{
  if (IsDebuggerPresent()) {
    char buf[2048];
    va_list args;
    va_start(args, fmt);
    VsprintfLiteral(buf, fmt, args);
    va_end(args);
    OutputDebugStringA(buf);
  }

  FILE *fp = _fdopen(_dup(2), "a");
  if (!fp)
      return;

  va_list args;
  va_start(args, fmt);
  vfprintf(fp, fmt, args);
  va_end(args);

  fclose(fp);
}

static void
MaybeBreakForBrowserDebugging()
{
=======
static void MaybeBreakForBrowserDebugging() {
>>>>>>> upstream-releases
  if (mozilla::EnvHasValue("MOZ_DEBUG_BROWSER_PROCESS")) {
    ::DebugBreak();
    return;
  }

  const wchar_t* pauseLenS = _wgetenv(L"MOZ_DEBUG_BROWSER_PAUSE");
  if (!pauseLenS || !(*pauseLenS)) {
    return;
  }

  DWORD pauseLenMs = wcstoul(pauseLenS, nullptr, 10) * 1000;
  printf_stderr("\n\nBROWSERBROWSERBROWSERBROWSER\n  debug me @ %lu\n\n",
                ::GetCurrentProcessId());
  ::Sleep(pauseLenMs);
}

static bool DoLauncherProcessChecks(int& argc, wchar_t** argv) {
  // NB: We run all tests in this function instead of returning early in order
  // to ensure that all side effects take place, such as clearing environment
  // variables.
  bool result = false;

<<<<<<< HEAD
static mozilla::LauncherResult<bool> IsSameBinaryAsParentProcess() {
  mozilla::LauncherResult<DWORD> parentPid = mozilla::nt::GetParentProcessId();
  if (parentPid.isErr()) {
    return LAUNCHER_ERROR_FROM_RESULT(parentPid);
||||||| merged common ancestors
static bool
IsSameBinaryAsParentProcess()
{
  mozilla::Maybe<DWORD> parentPid = mozilla::nt::GetParentProcessId();
  if (!parentPid) {
    // If NtQueryInformationProcess failed (in GetParentProcessId()),
    // we should not behave as the launcher process because it will also
    // likely to fail in child processes.
    MOZ_CRASH("NtQueryInformationProcess failed");
=======
#if defined(MOZ_LAUNCHER_PROCESS)
  mozilla::LauncherResult<bool> isSame = mozilla::IsSameBinaryAsParentProcess();
  if (isSame.isOk()) {
    result = !isSame.unwrap();
  } else {
    HandleLauncherError(isSame.unwrapErr());
>>>>>>> upstream-releases
  }
#endif  // defined(MOZ_LAUNCHER_PROCESS)

<<<<<<< HEAD
  nsAutoHandle parentProcess(::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,
                                           FALSE, parentPid.unwrap()));
  if (!parentProcess.get()) {
    DWORD err = ::GetLastError();
    if (err == ERROR_INVALID_PARAMETER) {
      // The process identified by parentPid has already exited. This is a
      // common case when the parent process is not Firefox, thus we should
      // return false instead of erroring out.
      return false;
    }

    return LAUNCHER_ERROR_FROM_WIN32(err);
||||||| merged common ancestors
  nsAutoHandle parentProcess(::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,
                                           FALSE, parentPid.value()));
  if (!parentProcess.get()) {
    // If OpenProcess failed, the parent process may not be present,
    // may be already terminated, etc. So we will have to behave as the
    // launcher proces in this case.
    return false;
=======
  if (mozilla::EnvHasValue("MOZ_LAUNCHER_PROCESS")) {
    mozilla::SaveToEnv("MOZ_LAUNCHER_PROCESS=");
    result = true;
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  WCHAR parentExe[MAX_PATH + 1] = {};
  DWORD parentExeLen = mozilla::ArrayLength(parentExe);
  if (!::QueryFullProcessImageNameW(parentProcess.get(), PROCESS_NAME_NATIVE,
                                    parentExe, &parentExeLen)) {
    return LAUNCHER_ERROR_FROM_LAST();
  }
||||||| merged common ancestors
  WCHAR parentExe[MAX_PATH + 1] = {};
  DWORD parentExeLen = mozilla::ArrayLength(parentExe);
  if (!::QueryFullProcessImageNameW(parentProcess.get(), PROCESS_NAME_NATIVE,
                                    parentExe, &parentExeLen)) {
    // If QueryFullProcessImageNameW failed, we should not behave as the
    // launcher process for the same reason as NtQueryInformationProcess.
    MOZ_CRASH("QueryFullProcessImageNameW failed");
  }
=======
  result |= mozilla::CheckArg(
                argc, argv, L"launcher", static_cast<const wchar_t**>(nullptr),
                mozilla::CheckArgFlag::RemoveArg) == mozilla::ARG_FOUND;
>>>>>>> upstream-releases

<<<<<<< HEAD
  WCHAR ourExe[MAX_PATH + 1] = {};
  DWORD ourExeOk =
      ::GetModuleFileNameW(nullptr, ourExe, mozilla::ArrayLength(ourExe));
  if (!ourExeOk || ourExeOk == mozilla::ArrayLength(ourExe)) {
    return LAUNCHER_ERROR_FROM_LAST();
  }
||||||| merged common ancestors
  WCHAR ourExe[MAX_PATH + 1] = {};
  DWORD ourExeOk = ::GetModuleFileNameW(nullptr, ourExe,
                                        mozilla::ArrayLength(ourExe));
  if (!ourExeOk || ourExeOk == mozilla::ArrayLength(ourExe)) {
    // If GetModuleFileNameW failed, we should not behave as the launcher
    // process for the same reason as NtQueryInformationProcess.
    MOZ_CRASH("GetModuleFileNameW failed");
  }
=======
  return result;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::WindowsErrorResult<bool> isSame =
      mozilla::DoPathsPointToIdenticalFile(parentExe, ourExe,
                                           mozilla::PathType::eNtPath);
  if (isSame.isErr()) {
    return LAUNCHER_ERROR_FROM_MOZ_WINDOWS_ERROR(isSame.unwrapErr());
||||||| merged common ancestors
  mozilla::Maybe<bool> isSame =
    mozilla::DoPathsPointToIdenticalFile(parentExe, ourExe,
                                         mozilla::eNtPath);
  if (!isSame) {
    // If DoPathsPointToIdenticalFile failed, we should not behave as the
    // launcher process for the same reason as NtQueryInformationProcess.
    MOZ_CRASH("DoPathsPointToIdenticalFile failed");
=======
static mozilla::Maybe<bool> RunAsLauncherProcess(int& argc, wchar_t** argv) {
  // return fast when we're a child process.
  // (The remainder of this function has some side effects that are
  // undesirable for content processes)
  if (mozilla::CheckArg(argc, argv, L"contentproc",
                        static_cast<const wchar_t**>(nullptr),
                        mozilla::CheckArgFlag::None) == mozilla::ARG_FOUND) {
    return mozilla::Some(false);
>>>>>>> upstream-releases
  }
<<<<<<< HEAD

  return isSame.unwrap();
}
||||||| merged common ancestors
  return isSame.value();
}
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
#endif  // defined(MOZ_LAUNCHER_PROCESS)
||||||| merged common ancestors
#endif // defined(MOZ_LAUNCHER_PROCESS)
=======
  bool runAsLauncher = DoLauncherProcessChecks(argc, argv);
>>>>>>> upstream-releases

#if defined(MOZ_LAUNCHER_PROCESS)
  bool forceLauncher =
      runAsLauncher &&
      mozilla::CheckArg(argc, argv, L"force-launcher",
                        static_cast<const wchar_t**>(nullptr),
                        mozilla::CheckArgFlag::RemoveArg) == mozilla::ARG_FOUND;

<<<<<<< HEAD
bool RunAsLauncherProcess(int& argc, wchar_t** argv) {
  // NB: We run all tests in this function instead of returning early in order
  // to ensure that all side effects take place, such as clearing environment
  // variables.
  bool result = false;
||||||| merged common ancestors
bool
RunAsLauncherProcess(int& argc, wchar_t** argv)
{
  // NB: We run all tests in this function instead of returning early in order
  // to ensure that all side effects take place, such as clearing environment
  // variables.
  bool result = false;
=======
  mozilla::LauncherRegistryInfo::ProcessType desiredType =
      runAsLauncher ? mozilla::LauncherRegistryInfo::ProcessType::Launcher
                    : mozilla::LauncherRegistryInfo::ProcessType::Browser;
>>>>>>> upstream-releases

<<<<<<< HEAD
#if defined(MOZ_LAUNCHER_PROCESS)
  LauncherResult<bool> isSame = IsSameBinaryAsParentProcess();
  if (isSame.isOk()) {
    result = !isSame.unwrap();
  } else {
    HandleLauncherError(isSame.unwrapErr());
  }
#endif  // defined(MOZ_LAUNCHER_PROCESS)
||||||| merged common ancestors
#if defined(MOZ_LAUNCHER_PROCESS)
  result = !IsSameBinaryAsParentProcess();
#endif // defined(MOZ_LAUNCHER_PROCESS)
=======
  mozilla::LauncherRegistryInfo::CheckOption checkOption =
      forceLauncher ? mozilla::LauncherRegistryInfo::CheckOption::Force
                    : mozilla::LauncherRegistryInfo::CheckOption::Default;
>>>>>>> upstream-releases

  mozilla::LauncherRegistryInfo regInfo;
  mozilla::LauncherResult<mozilla::LauncherRegistryInfo::ProcessType>
      runAsType = regInfo.Check(desiredType, checkOption);

  if (runAsType.isErr()) {
    mozilla::HandleLauncherError(runAsType);
    return mozilla::Nothing();
  }

<<<<<<< HEAD
  result |=
      CheckArg(argc, argv, L"launcher", static_cast<const wchar_t**>(nullptr),
               CheckArgFlag::RemoveArg) == ARG_FOUND;
||||||| merged common ancestors
  result |= CheckArg(argc, argv, L"launcher",
                     static_cast<const wchar_t**>(nullptr),
                     CheckArgFlag::RemoveArg) == ARG_FOUND;
=======
  runAsLauncher = runAsType.unwrap() ==
                  mozilla::LauncherRegistryInfo::ProcessType::Launcher;
#endif  // defined(MOZ_LAUNCHER_PROCESS)
>>>>>>> upstream-releases

  if (!runAsLauncher) {
    // In this case, we will be proceeding to run as the browser.
    // We should check MOZ_DEBUG_BROWSER_* env vars.
    MaybeBreakForBrowserDebugging();
  }

  return mozilla::Some(runAsLauncher);
}

<<<<<<< HEAD
int LauncherMain(int argc, wchar_t* argv[]) {
||||||| merged common ancestors
int
LauncherMain(int argc, wchar_t* argv[])
{
=======
namespace mozilla {

Maybe<int> LauncherMain(int& argc, wchar_t* argv[],
                        const StaticXREAppData& aAppData) {
  EnsureCommandlineSafe(argc, argv);

  SetLauncherErrorAppData(aAppData);

  if (CheckArg(argc, argv, L"log-launcher-error",
               static_cast<const wchar_t**>(nullptr),
               mozilla::CheckArgFlag::RemoveArg) == ARG_FOUND) {
    SetLauncherErrorForceEventLog();
  }

  Maybe<bool> runAsLauncher = RunAsLauncherProcess(argc, argv);
  if (!runAsLauncher || !runAsLauncher.value()) {
    return Nothing();
  }

>>>>>>> upstream-releases
  // Make sure that the launcher process itself has image load policies set
  if (IsWin10AnniversaryUpdateOrLater()) {
    const DynamicallyLinkedFunctionPtr<decltype(&SetProcessMitigationPolicy)>
        pSetProcessMitigationPolicy(L"kernel32.dll",
                                    "SetProcessMitigationPolicy");
    if (pSetProcessMitigationPolicy) {
      PROCESS_MITIGATION_IMAGE_LOAD_POLICY imgLoadPol = {};
      imgLoadPol.PreferSystem32Images = 1;

      DebugOnly<BOOL> setOk = pSetProcessMitigationPolicy(
          ProcessImageLoadPolicy, &imgLoadPol, sizeof(imgLoadPol));
      MOZ_ASSERT(setOk);
    }
  }

  mozilla::UseParentConsole();

  if (!SetArgv0ToFullBinaryPath(argv)) {
<<<<<<< HEAD
    HandleLauncherError(LAUNCHER_ERROR_GENERIC());
    return 1;
||||||| merged common ancestors
    ShowError();
    return 1;
=======
    HandleLauncherError(LAUNCHER_ERROR_GENERIC());
    return Nothing();
>>>>>>> upstream-releases
  }

  LauncherFlags flags = ProcessCmdLine(argc, argv);

  nsAutoHandle mediumIlToken;
<<<<<<< HEAD
  LauncherResult<ElevationState> elevationState =
      GetElevationState(flags, mediumIlToken);
  if (elevationState.isErr()) {
    HandleLauncherError(elevationState);
    return 1;
||||||| merged common ancestors
  Maybe<ElevationState> elevationState = GetElevationState(flags, mediumIlToken);
  if (!elevationState) {
    return 1;
=======
  LauncherResult<ElevationState> elevationState =
      GetElevationState(flags, mediumIlToken);
  if (elevationState.isErr()) {
    HandleLauncherError(elevationState);
    return Nothing();
>>>>>>> upstream-releases
  }

  // If we're elevated, we should relaunch ourselves as a normal user.
  // Note that we only call LaunchUnelevated when we don't need to wait for the
  // browser process.
  if (elevationState.unwrap() == ElevationState::eElevated &&
      !(flags &
        (LauncherFlags::eWaitForBrowser | LauncherFlags::eNoDeelevate)) &&
      !mediumIlToken.get()) {
<<<<<<< HEAD
    LauncherVoidResult launchedUnelevated = LaunchUnelevated(argc, argv);
    bool failed = launchedUnelevated.isErr();
    if (failed) {
      HandleLauncherError(launchedUnelevated);
    }

    return failed;
||||||| merged common ancestors
    return !LaunchUnelevated(argc, argv);
=======
    LauncherVoidResult launchedUnelevated = LaunchUnelevated(argc, argv);
    bool failed = launchedUnelevated.isErr();
    if (failed) {
      HandleLauncherError(launchedUnelevated);
      return Nothing();
    }

    return Some(0);
>>>>>>> upstream-releases
  }

  // Now proceed with setting up the parameters for process creation
  UniquePtr<wchar_t[]> cmdLine(MakeCommandLine(argc, argv));
  if (!cmdLine) {
<<<<<<< HEAD
    HandleLauncherError(LAUNCHER_ERROR_GENERIC());
    return 1;
||||||| merged common ancestors
    return 1;
=======
    HandleLauncherError(LAUNCHER_ERROR_GENERIC());
    return Nothing();
>>>>>>> upstream-releases
  }

  const Maybe<bool> isSafeMode =
      IsSafeModeRequested(argc, argv, SafeModeFlag::NoKeyPressCheck);
  if (!isSafeMode) {
<<<<<<< HEAD
    HandleLauncherError(LAUNCHER_ERROR_FROM_WIN32(ERROR_INVALID_PARAMETER));
    return 1;
||||||| merged common ancestors
    ShowError(ERROR_INVALID_PARAMETER);
    return 1;
=======
    HandleLauncherError(LAUNCHER_ERROR_FROM_WIN32(ERROR_INVALID_PARAMETER));
    return Nothing();
>>>>>>> upstream-releases
  }

  ProcThreadAttributes attrs;
  SetMitigationPolicies(attrs, isSafeMode.value());

  HANDLE stdHandles[] = {::GetStdHandle(STD_INPUT_HANDLE),
                         ::GetStdHandle(STD_OUTPUT_HANDLE),
                         ::GetStdHandle(STD_ERROR_HANDLE)};

  attrs.AddInheritableHandles(stdHandles);

  DWORD creationFlags = CREATE_SUSPENDED | CREATE_UNICODE_ENVIRONMENT;

  STARTUPINFOEXW siex;
<<<<<<< HEAD
  LauncherResult<bool> attrsOk = attrs.AssignTo(siex);
  if (attrsOk.isErr()) {
    HandleLauncherError(attrsOk);
    return 1;
||||||| merged common ancestors
  Maybe<bool> attrsOk = attrs.AssignTo(siex);
  if (!attrsOk) {
    ShowError();
    return 1;
=======
  LauncherResult<bool> attrsOk = attrs.AssignTo(siex);
  if (attrsOk.isErr()) {
    HandleLauncherError(attrsOk);
    return Nothing();
>>>>>>> upstream-releases
  }

  BOOL inheritHandles = FALSE;

  if (attrsOk.unwrap()) {
    creationFlags |= EXTENDED_STARTUPINFO_PRESENT;

    if (attrs.HasInheritableHandles()) {
      siex.StartupInfo.dwFlags |= STARTF_USESTDHANDLES;
      siex.StartupInfo.hStdInput = stdHandles[0];
      siex.StartupInfo.hStdOutput = stdHandles[1];
      siex.StartupInfo.hStdError = stdHandles[2];

      // Since attrsOk == true, we have successfully set the handle inheritance
      // whitelist policy, so only the handles added to attrs will be inherited.
      inheritHandles = TRUE;
    }
  }

  PROCESS_INFORMATION pi = {};
  BOOL createOk;

  if (mediumIlToken.get()) {
    createOk =
        ::CreateProcessAsUserW(mediumIlToken.get(), argv[0], cmdLine.get(),
                               nullptr, nullptr, inheritHandles, creationFlags,
                               nullptr, nullptr, &siex.StartupInfo, &pi);
  } else {
    createOk = ::CreateProcessW(argv[0], cmdLine.get(), nullptr, nullptr,
                                inheritHandles, creationFlags, nullptr, nullptr,
                                &siex.StartupInfo, &pi);
  }

  if (!createOk) {
<<<<<<< HEAD
    HandleLauncherError(LAUNCHER_ERROR_FROM_LAST());
    return 1;
||||||| merged common ancestors
    ShowError();
    return 1;
=======
    HandleLauncherError(LAUNCHER_ERROR_FROM_LAST());
    return Nothing();
>>>>>>> upstream-releases
  }

  nsAutoHandle process(pi.hProcess);
  nsAutoHandle mainThread(pi.hThread);

<<<<<<< HEAD
  LauncherVoidResult setupResult =
      PostCreationSetup(process.get(), mainThread.get(), isSafeMode.value());
  if (setupResult.isErr()) {
    HandleLauncherError(setupResult);
    ::TerminateProcess(process.get(), 1);
    return 1;
  }

  if (::ResumeThread(mainThread.get()) == static_cast<DWORD>(-1)) {
    HandleLauncherError(LAUNCHER_ERROR_FROM_LAST());
||||||| merged common ancestors
  if (!PostCreationSetup(process.get(), mainThread.get(), isSafeMode.value()) ||
      ::ResumeThread(mainThread.get()) == static_cast<DWORD>(-1)) {
    ShowError();
=======
  LauncherVoidResult setupResult = PostCreationSetup(
      argv[0], process.get(), mainThread.get(), isSafeMode.value());
  if (setupResult.isErr()) {
    HandleLauncherError(setupResult);
    ::TerminateProcess(process.get(), 1);
    return Nothing();
  }

  if (::ResumeThread(mainThread.get()) == static_cast<DWORD>(-1)) {
    HandleLauncherError(LAUNCHER_ERROR_FROM_LAST());
>>>>>>> upstream-releases
    ::TerminateProcess(process.get(), 1);
    return Nothing();
  }

  if (flags & LauncherFlags::eWaitForBrowser) {
    DWORD exitCode;
    if (::WaitForSingleObject(process.get(), INFINITE) == WAIT_OBJECT_0 &&
        ::GetExitCodeProcess(process.get(), &exitCode)) {
      // Propagate the browser process's exit code as our exit code.
      return Some(static_cast<int>(exitCode));
    }
  } else {
    const DWORD timeout =
        ::IsDebuggerPresent() ? INFINITE : kWaitForInputIdleTimeoutMS;

    // Keep the current process around until the callback process has created
    // its message queue, to avoid the launched process's windows being forced
    // into the background.
    mozilla::WaitForInputIdle(process.get(), timeout);
  }

  return Some(0);
}

}  // namespace mozilla
