/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// This file is not build directly. Instead, it is included in multiple
// shared objects.

#ifdef nsWindowsRestart_cpp
<<<<<<< HEAD
#error \
    "nsWindowsRestart.cpp is not a header file, and must only be included once."
||||||| merged common ancestors
#error "nsWindowsRestart.cpp is not a header file, and must only be included once."
=======
#  error \
      "nsWindowsRestart.cpp is not a header file, and must only be included once."
>>>>>>> upstream-releases
#else
#  define nsWindowsRestart_cpp
#endif

#include "mozilla/CmdLineAndEnvUtils.h"
#include "nsUTF8Utils.h"

#include <shellapi.h>

// Needed for CreateEnvironmentBlock
#include <userenv.h>
#ifndef __MINGW32__
#  pragma comment(lib, "userenv.lib")
#endif

/**
 * Convert UTF8 to UTF16 without using the normal XPCOM goop, which we
 * can't link to updater.exe.
 */
<<<<<<< HEAD
static char16_t *AllocConvertUTF8toUTF16(const char *arg) {
||||||| merged common ancestors
static char16_t*
AllocConvertUTF8toUTF16(const char *arg)
{
=======
static char16_t* AllocConvertUTF8toUTF16(const char* arg) {
>>>>>>> upstream-releases
  // UTF16 can't be longer in units than UTF8
  size_t len = strlen(arg);
<<<<<<< HEAD
  char16_t *s = new char16_t[(len + 1) * sizeof(char16_t)];
  if (!s) return nullptr;
||||||| merged common ancestors
  char16_t *s = new char16_t[(len + 1) * sizeof(char16_t)];
  if (!s)
    return nullptr;
=======
  char16_t* s = new char16_t[(len + 1) * sizeof(char16_t)];
  if (!s) return nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t dstLen = ::MultiByteToWideChar(CP_UTF8, 0, arg, len,
                                        reinterpret_cast<wchar_t *>(s), len);
||||||| merged common ancestors
  size_t dstLen = ::MultiByteToWideChar(
    CP_UTF8, 0, arg, len, reinterpret_cast<wchar_t*>(s), len);
=======
  size_t dstLen = ::MultiByteToWideChar(CP_UTF8, 0, arg, len,
                                        reinterpret_cast<wchar_t*>(s), len);
>>>>>>> upstream-releases
  s[dstLen] = 0;

  return s;
}

<<<<<<< HEAD
static void FreeAllocStrings(int argc, wchar_t **argv) {
||||||| merged common ancestors
static void
FreeAllocStrings(int argc, wchar_t **argv)
{
=======
static void FreeAllocStrings(int argc, wchar_t** argv) {
>>>>>>> upstream-releases
  while (argc) {
    --argc;
    delete[] argv[argc];
  }

  delete[] argv;
}

<<<<<<< HEAD
static wchar_t **AllocConvertUTF8toUTF16Strings(int argc, char **argv) {
  wchar_t **argvConverted = new wchar_t *[argc];
  if (!argvConverted) return nullptr;
||||||| merged common ancestors
static wchar_t**
AllocConvertUTF8toUTF16Strings(int argc, char **argv)
{
  wchar_t **argvConverted = new wchar_t*[argc];
  if (!argvConverted)
    return nullptr;
=======
static wchar_t** AllocConvertUTF8toUTF16Strings(int argc, char** argv) {
  wchar_t** argvConverted = new wchar_t*[argc];
  if (!argvConverted) return nullptr;
>>>>>>> upstream-releases

  for (int i = 0; i < argc; ++i) {
<<<<<<< HEAD
    argvConverted[i] =
        reinterpret_cast<wchar_t *>(AllocConvertUTF8toUTF16(argv[i]));
||||||| merged common ancestors
    argvConverted[i] = reinterpret_cast<wchar_t*>(AllocConvertUTF8toUTF16(argv[i]));
=======
    argvConverted[i] =
        reinterpret_cast<wchar_t*>(AllocConvertUTF8toUTF16(argv[i]));
>>>>>>> upstream-releases
    if (!argvConverted[i]) {
      FreeAllocStrings(i, argvConverted);
      return nullptr;
    }
  }
  return argvConverted;
}

/**
 * Launch a child process with the specified arguments.
 * @note argv[0] is ignored
 * @note The form of this function that takes char **argv expects UTF-8
 */

<<<<<<< HEAD
BOOL WinLaunchChild(const wchar_t *exePath, int argc, wchar_t **argv,
                    HANDLE userToken = nullptr, HANDLE *hProcess = nullptr);

BOOL WinLaunchChild(const wchar_t *exePath, int argc, char **argv,
                    HANDLE userToken, HANDLE *hProcess) {
  wchar_t **argvConverted = AllocConvertUTF8toUTF16Strings(argc, argv);
  if (!argvConverted) return FALSE;
||||||| merged common ancestors
BOOL
WinLaunchChild(const wchar_t *exePath,
               int argc, wchar_t **argv,
               HANDLE userToken = nullptr,
               HANDLE *hProcess = nullptr);

BOOL
WinLaunchChild(const wchar_t *exePath,
               int argc, char **argv,
               HANDLE userToken,
               HANDLE *hProcess)
{
  wchar_t **argvConverted = AllocConvertUTF8toUTF16Strings(argc, argv);
  if (!argvConverted)
    return FALSE;
=======
BOOL WinLaunchChild(const wchar_t* exePath, int argc, wchar_t** argv,
                    HANDLE userToken = nullptr, HANDLE* hProcess = nullptr);

BOOL WinLaunchChild(const wchar_t* exePath, int argc, char** argv,
                    HANDLE userToken, HANDLE* hProcess) {
  wchar_t** argvConverted = AllocConvertUTF8toUTF16Strings(argc, argv);
  if (!argvConverted) return FALSE;
>>>>>>> upstream-releases

  BOOL ok = WinLaunchChild(exePath, argc, argvConverted, userToken, hProcess);
  FreeAllocStrings(argc, argvConverted);
  return ok;
}

<<<<<<< HEAD
BOOL WinLaunchChild(const wchar_t *exePath, int argc, wchar_t **argv,
                    HANDLE userToken, HANDLE *hProcess) {
||||||| merged common ancestors
BOOL
WinLaunchChild(const wchar_t *exePath,
               int argc,
               wchar_t **argv,
               HANDLE userToken,
               HANDLE *hProcess)
{
=======
BOOL WinLaunchChild(const wchar_t* exePath, int argc, wchar_t** argv,
                    HANDLE userToken, HANDLE* hProcess) {
>>>>>>> upstream-releases
  BOOL ok;

  mozilla::UniquePtr<wchar_t[]> cl(mozilla::MakeCommandLine(argc, argv));
  if (!cl) {
    return FALSE;
  }

  STARTUPINFOW si = {0};
  si.cb = sizeof(STARTUPINFOW);
  si.lpDesktop = const_cast<LPWSTR>(L"winsta0\\Default");
  PROCESS_INFORMATION pi = {0};

  if (userToken == nullptr) {
    ok = CreateProcessW(exePath, cl.get(),
                        nullptr,  // no special security attributes
                        nullptr,  // no special thread attributes
                        FALSE,    // don't inherit filehandles
                        0,        // creation flags
                        nullptr,  // inherit my environment
                        nullptr,  // use my current directory
                        &si, &pi);
  } else {
    // Create an environment block for the process we're about to start using
    // the user's token.
    LPVOID environmentBlock = nullptr;
    if (!CreateEnvironmentBlock(&environmentBlock, userToken, TRUE)) {
      environmentBlock = nullptr;
    }

    ok = CreateProcessAsUserW(userToken, exePath, cl.get(),
                              nullptr,  // no special security attributes
                              nullptr,  // no special thread attributes
                              FALSE,    // don't inherit filehandles
                              0,        // creation flags
                              environmentBlock,
                              nullptr,  // use my current directory
                              &si, &pi);

    if (environmentBlock) {
      DestroyEnvironmentBlock(environmentBlock);
    }
  }

  if (ok) {
    if (hProcess) {
      *hProcess = pi.hProcess;  // the caller now owns the HANDLE
    } else {
      CloseHandle(pi.hProcess);
    }
    CloseHandle(pi.hThread);
  } else {
    LPVOID lpMsgBuf = nullptr;
<<<<<<< HEAD
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                  nullptr, GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf,
                  0, nullptr);
    wprintf(L"Error restarting: %s\n",
            lpMsgBuf ? static_cast<const wchar_t *>(lpMsgBuf) : L"(null)");
    if (lpMsgBuf) LocalFree(lpMsgBuf);
||||||| merged common ancestors
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_IGNORE_INSERTS,
                  nullptr,
                  GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpMsgBuf,
                  0,
                  nullptr);
    wprintf(L"Error restarting: %s\n", lpMsgBuf ? static_cast<const wchar_t*>(lpMsgBuf) : L"(null)");
    if (lpMsgBuf)
      LocalFree(lpMsgBuf);
=======
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                  nullptr, GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf,
                  0, nullptr);
    wprintf(L"Error restarting: %s\n",
            lpMsgBuf ? static_cast<const wchar_t*>(lpMsgBuf) : L"(null)");
    if (lpMsgBuf) LocalFree(lpMsgBuf);
>>>>>>> upstream-releases
  }

  return ok;
}
