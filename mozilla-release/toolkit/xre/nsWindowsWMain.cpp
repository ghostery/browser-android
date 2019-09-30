/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsWindowsWMain_cpp
#define nsWindowsWMain_cpp

// This file is a .cpp file meant to be included in nsBrowserApp.cpp and other
// similar bootstrap code. It converts wide-character windows wmain into UTF-8
// narrow-character strings.

#ifndef XP_WIN
#  error This file only makes sense on Windows.
#endif

#include "mozilla/Char16.h"
#include "nsUTF8Utils.h"

#include <windows.h>

#ifdef __MINGW32__

/* MingW currently does not implement a wide version of the
   startup routines.  Workaround is to implement something like
   it ourselves.  See bug 411826 */

#  include <shellapi.h>

int wmain(int argc, WCHAR** argv);

<<<<<<< HEAD
int main(int argc, char **argv) {
||||||| merged common ancestors
int main(int argc, char **argv)
{
=======
int main(int argc, char** argv) {
>>>>>>> upstream-releases
  LPWSTR commandLine = GetCommandLineW();
  int argcw = 0;
<<<<<<< HEAD
  LPWSTR *argvw = CommandLineToArgvW(commandLine, &argcw);
  if (!argvw) return 127;
||||||| merged common ancestors
  LPWSTR *argvw = CommandLineToArgvW(commandLine, &argcw);
  if (!argvw)
    return 127;
=======
  LPWSTR* argvw = CommandLineToArgvW(commandLine, &argcw);
  if (!argvw) return 127;
>>>>>>> upstream-releases

  int result = wmain(argcw, argvw);
  LocalFree(argvw);
  return result;
}
#endif /* __MINGW32__ */

#define main NS_internal_main

#ifndef XRE_WANT_ENVIRON
int main(int argc, char** argv);
#else
int main(int argc, char** argv, char** envp);
#endif

static void SanitizeEnvironmentVariables() {
  DWORD bufferSize = GetEnvironmentVariableW(L"PATH", nullptr, 0);
  if (bufferSize) {
<<<<<<< HEAD
    wchar_t *originalPath = new wchar_t[bufferSize];
    if (bufferSize - 1 ==
        GetEnvironmentVariableW(L"PATH", originalPath, bufferSize)) {
||||||| merged common ancestors
    wchar_t* originalPath = new wchar_t[bufferSize];
    if (bufferSize - 1 == GetEnvironmentVariableW(L"PATH", originalPath,
                                                  bufferSize)) {
=======
    wchar_t* originalPath = new wchar_t[bufferSize];
    if (bufferSize - 1 ==
        GetEnvironmentVariableW(L"PATH", originalPath, bufferSize)) {
>>>>>>> upstream-releases
      bufferSize = ExpandEnvironmentStringsW(originalPath, nullptr, 0);
      if (bufferSize) {
<<<<<<< HEAD
        wchar_t *newPath = new wchar_t[bufferSize];
        if (ExpandEnvironmentStringsW(originalPath, newPath, bufferSize)) {
||||||| merged common ancestors
        wchar_t* newPath = new wchar_t[bufferSize];
        if (ExpandEnvironmentStringsW(originalPath,
                                      newPath,
                                      bufferSize)) {
=======
        wchar_t* newPath = new wchar_t[bufferSize];
        if (ExpandEnvironmentStringsW(originalPath, newPath, bufferSize)) {
>>>>>>> upstream-releases
          SetEnvironmentVariableW(L"PATH", newPath);
        }
        delete[] newPath;
      }
    }
    delete[] originalPath;
  }
}

<<<<<<< HEAD
static char *AllocConvertUTF16toUTF8(char16ptr_t arg) {
||||||| merged common ancestors
static char*
AllocConvertUTF16toUTF8(char16ptr_t arg)
{
=======
static char* AllocConvertUTF16toUTF8(char16ptr_t arg) {
>>>>>>> upstream-releases
  // be generous... UTF16 units can expand up to 3 UTF8 units
  size_t len = wcslen(arg);
  // ConvertUTF16toUTF8 requires +1. Let's do that here, too, lacking
  // knowledge of Windows internals.
  size_t dstLen = len * 3 + 1;
<<<<<<< HEAD
  char *s = new char[dstLen + 1];  // Another +1 for zero terminator
  if (!s) return nullptr;
||||||| merged common ancestors
  char* s = new char[dstLen + 1]; // Another +1 for zero terminator
  if (!s)
    return nullptr;
=======
  char* s = new char[dstLen + 1];  // Another +1 for zero terminator
  if (!s) return nullptr;
>>>>>>> upstream-releases

  int written =
      ::WideCharToMultiByte(CP_UTF8, 0, arg, len, s, dstLen, nullptr, nullptr);
  s[written] = 0;
  return s;
}

<<<<<<< HEAD
static void FreeAllocStrings(int argc, char **argv) {
||||||| merged common ancestors
static void
FreeAllocStrings(int argc, char **argv)
{
=======
static void FreeAllocStrings(int argc, char** argv) {
>>>>>>> upstream-releases
  while (argc) {
    --argc;
    delete[] argv[argc];
  }

  delete[] argv;
}

<<<<<<< HEAD
int wmain(int argc, WCHAR **argv) {
||||||| merged common ancestors
int wmain(int argc, WCHAR **argv)
{
=======
int wmain(int argc, WCHAR** argv) {
>>>>>>> upstream-releases
  SanitizeEnvironmentVariables();
  SetDllDirectoryW(L"");

  // Only run this code if LauncherProcessWin.h was included beforehand, thus
  // signalling that the hosting process should support launcher mode.
#if defined(mozilla_LauncherProcessWin_h)
  mozilla::Maybe<int> launcherResult =
      mozilla::LauncherMain(argc, argv, sAppData);
  if (launcherResult) {
    return launcherResult.value();
  }
#endif  // defined(mozilla_LauncherProcessWin_h)

<<<<<<< HEAD
  char **argvConverted = new char *[argc + 1];
  if (!argvConverted) return 127;
||||||| merged common ancestors
  char **argvConverted = new char*[argc + 1];
  if (!argvConverted)
    return 127;
=======
  char** argvConverted = new char*[argc + 1];
  if (!argvConverted) return 127;
>>>>>>> upstream-releases

  for (int i = 0; i < argc; ++i) {
    argvConverted[i] = AllocConvertUTF16toUTF8(argv[i]);
    if (!argvConverted[i]) {
      return 127;
    }
  }
  argvConverted[argc] = nullptr;

  // need to save argvConverted copy for later deletion.
<<<<<<< HEAD
  char **deleteUs = new char *[argc + 1];
||||||| merged common ancestors
  char **deleteUs = new char*[argc+1];
=======
  char** deleteUs = new char*[argc + 1];
>>>>>>> upstream-releases
  if (!deleteUs) {
    FreeAllocStrings(argc, argvConverted);
    return 127;
  }
  for (int i = 0; i < argc; i++) deleteUs[i] = argvConverted[i];
#ifndef XRE_WANT_ENVIRON
  int result = main(argc, argvConverted);
#else
  // Force creation of the multibyte _environ variable.
  getenv("PATH");
  int result = main(argc, argvConverted, _environ);
#endif

  delete[] argvConverted;
  FreeAllocStrings(argc, deleteUs);

  return result;
}

#endif  // nsWindowsWMain_cpp
