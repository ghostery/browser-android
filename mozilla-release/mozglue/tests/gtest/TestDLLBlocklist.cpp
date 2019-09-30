/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <windows.h>
#include <winternl.h>

#include <process.h>

#include "gtest/gtest.h"

#include "mozilla/ArrayUtils.h"
#include "mozilla/Char16.h"
#include "nsDirectoryServiceDefs.h"
#include "nsDirectoryServiceUtils.h"
#include "nsString.h"
#include "nsTArray.h"
#include "nsWindowsHelpers.h"

<<<<<<< HEAD
static nsDependentSubstring MakeString(PUNICODE_STRING aOther) {
  size_t numChars = aOther->Length / sizeof(WCHAR);
  return nsDependentSubstring((const char16_t*)aOther->Buffer, numChars);
}
||||||| merged common ancestors
static nsDependentSubstring
MakeString(PUNICODE_STRING aOther)
{
  size_t numChars = aOther->Length / sizeof(WCHAR);
  return nsDependentSubstring((const char16_t *)aOther->Buffer, numChars);
}
=======
static nsString GetFullPath(const nsAString& aLeaf) {
  nsCOMPtr<nsIFile> f;
>>>>>>> upstream-releases

<<<<<<< HEAD
static void DllLoadHook(bool aDllLoaded, NTSTATUS aStatus, HANDLE aDllBase,
                        PUNICODE_STRING aDllName) {
  nsDependentSubstring str = MakeString(aDllName);
  if (StringEndsWith(str, nsDependentString(sDllName),
                     nsCaseInsensitiveStringComparator())) {
    if (aDllLoaded) {
      sDllWasLoaded = true;
    } else {
      sDllWasBlocked = true;
    }
  }
}
||||||| merged common ancestors
static void
DllLoadHook(bool aDllLoaded, NTSTATUS aStatus, HANDLE aDllBase,
            PUNICODE_STRING aDllName)
{
  nsDependentSubstring str = MakeString(aDllName);
  if (StringEndsWith(str, nsDependentString(sDllName),
                     nsCaseInsensitiveStringComparator())) {
    if (aDllLoaded) {
      sDllWasLoaded = true;
    } else {
      sDllWasBlocked = true;
    }
  }
}
=======
  EXPECT_TRUE(NS_SUCCEEDED(
      NS_GetSpecialDirectory(NS_OS_CURRENT_WORKING_DIR, getter_AddRefs(f))));
>>>>>>> upstream-releases

<<<<<<< HEAD
static nsString GetFullPath(const char16_t* leaf) {
  nsCOMPtr<nsIFile> f;
  EXPECT_TRUE(NS_SUCCEEDED(
      NS_GetSpecialDirectory(NS_OS_CURRENT_WORKING_DIR, getter_AddRefs(f))));
  EXPECT_TRUE(NS_SUCCEEDED(f->Append(nsDependentString(leaf))));
||||||| merged common ancestors
static nsString
GetFullPath(const char16_t* leaf)
{
  nsCOMPtr<nsIFile> f;
  EXPECT_TRUE(NS_SUCCEEDED(NS_GetSpecialDirectory(NS_OS_CURRENT_WORKING_DIR, getter_AddRefs(f))));
  EXPECT_TRUE(NS_SUCCEEDED(f->Append(nsDependentString(leaf))));
=======
  EXPECT_TRUE(NS_SUCCEEDED(f->Append(aLeaf)));
>>>>>>> upstream-releases

  bool exists;
  EXPECT_TRUE(NS_SUCCEEDED(f->Exists(&exists)) && exists);

  nsString ret;
  EXPECT_TRUE(NS_SUCCEEDED(f->GetPath(ret)));
  return ret;
}

<<<<<<< HEAD
TEST(TestDllBlocklist, BlockDllByName) {
  sDllWasBlocked = false;
  sDllWasLoaded = false;
  DllBlocklist_SetDllLoadHook(DllLoadHook);
  auto undoHooks =
      mozilla::MakeScopeExit([&]() { DllBlocklist_SetDllLoadHook(nullptr); });

||||||| merged common ancestors
TEST(TestDllBlocklist, BlockDllByName)
{
  sDllWasBlocked = false;
  sDllWasLoaded = false;
  DllBlocklist_SetDllLoadHook(DllLoadHook);
  auto undoHooks = mozilla::MakeScopeExit([&](){
    DllBlocklist_SetDllLoadHook(nullptr);
  });

=======
TEST(TestDllBlocklist, BlockDllByName)
{
>>>>>>> upstream-releases
  // The DLL name has capital letters, so this also tests that the comparison
  // is case-insensitive.
  NS_NAMED_LITERAL_STRING(kLeafName, "TestDllBlocklist_MatchByName.dll");
  nsString dllPath = GetFullPath(kLeafName);

  nsModuleHandle hDll(::LoadLibraryW(dllPath.get()));

  EXPECT_TRUE(!hDll);
  EXPECT_TRUE(!::GetModuleHandleW(kLeafName.get()));
}

<<<<<<< HEAD
TEST(TestDllBlocklist, BlockDllByVersion) {
  sDllWasBlocked = false;
  sDllWasLoaded = false;
  DllBlocklist_SetDllLoadHook(DllLoadHook);
  auto undoHooks =
      mozilla::MakeScopeExit([&]() { DllBlocklist_SetDllLoadHook(nullptr); });

  sDllName = u"TestDllBlocklist_MatchByVersion.dll";
  nsString dllPath = GetFullPath(sDllName);

  auto hDll = ::LoadLibraryW((char16ptr_t)dllPath.get());
  if (hDll) {
    EXPECT_TRUE(!"DLL was loaded but should have been blocked.");
  }
||||||| merged common ancestors
TEST(TestDllBlocklist, BlockDllByVersion)
{
  sDllWasBlocked = false;
  sDllWasLoaded = false;
  DllBlocklist_SetDllLoadHook(DllLoadHook);
  auto undoHooks = mozilla::MakeScopeExit([&](){
    DllBlocklist_SetDllLoadHook(nullptr);
  });

  sDllName = u"TestDllBlocklist_MatchByVersion.dll";
  nsString dllPath = GetFullPath(sDllName);

  auto hDll = ::LoadLibraryW((char16ptr_t)dllPath.get());
  if (hDll) {
    EXPECT_TRUE(!"DLL was loaded but should have been blocked.");
  }
=======
TEST(TestDllBlocklist, BlockDllByVersion)
{
  NS_NAMED_LITERAL_STRING(kLeafName, "TestDllBlocklist_MatchByVersion.dll");
  nsString dllPath = GetFullPath(kLeafName);
>>>>>>> upstream-releases

  nsModuleHandle hDll(::LoadLibraryW(dllPath.get()));

  EXPECT_TRUE(!hDll);
  EXPECT_TRUE(!::GetModuleHandleW(kLeafName.get()));
}

<<<<<<< HEAD
TEST(TestDllBlocklist, AllowDllByVersion) {
  sDllWasBlocked = false;
  sDllWasLoaded = false;
  DllBlocklist_SetDllLoadHook(DllLoadHook);
  auto undoHooks =
      mozilla::MakeScopeExit([&]() { DllBlocklist_SetDllLoadHook(nullptr); });

  sDllName = u"TestDllBlocklist_AllowByVersion.dll";
  nsString dllPath = GetFullPath(sDllName);

  auto hDll = ::LoadLibraryW((char16ptr_t)dllPath.get());
  if (!hDll) {
    EXPECT_TRUE(!"DLL was blocked but should have been loaded.");
  }
||||||| merged common ancestors
TEST(TestDllBlocklist, AllowDllByVersion)
{
  sDllWasBlocked = false;
  sDllWasLoaded = false;
  DllBlocklist_SetDllLoadHook(DllLoadHook);
  auto undoHooks = mozilla::MakeScopeExit([&](){
    DllBlocklist_SetDllLoadHook(nullptr);
  });

  sDllName = u"TestDllBlocklist_AllowByVersion.dll";
  nsString dllPath = GetFullPath(sDllName);

  auto hDll = ::LoadLibraryW((char16ptr_t)dllPath.get());
  if (!hDll) {
    EXPECT_TRUE(!"DLL was blocked but should have been loaded.");
  }
=======
TEST(TestDllBlocklist, AllowDllByVersion)
{
  NS_NAMED_LITERAL_STRING(kLeafName, "TestDllBlocklist_AllowByVersion.dll");
  nsString dllPath = GetFullPath(kLeafName);
>>>>>>> upstream-releases

  nsModuleHandle hDll(::LoadLibraryW(dllPath.get()));

  EXPECT_TRUE(!!hDll);
  EXPECT_TRUE(!!::GetModuleHandleW(kLeafName.get()));
}

<<<<<<< HEAD
TEST(TestDllBlocklist, BlocklistIntegrity) {
  auto msg = DllBlocklist_TestBlocklistIntegrity();
  EXPECT_FALSE(msg) << msg;
||||||| merged common ancestors
TEST(TestDllBlocklist, BlocklistIntegrity)
{
  auto msg = DllBlocklist_TestBlocklistIntegrity();
  EXPECT_FALSE(msg) << msg;
=======
#define DLL_BLOCKLIST_ENTRY(name, ...) {name, __VA_ARGS__},
#define DLL_BLOCKLIST_STRING_TYPE const char*
#include "mozilla/WindowsDllBlocklistDefs.h"

TEST(TestDllBlocklist, BlocklistIntegrity)
{
  nsTArray<DLL_BLOCKLIST_STRING_TYPE> dupes;
  DECLARE_POINTER_TO_FIRST_DLL_BLOCKLIST_ENTRY(pFirst);
  DECLARE_POINTER_TO_LAST_DLL_BLOCKLIST_ENTRY(pLast);

  EXPECT_FALSE(pLast->name || pLast->maxVersion || pLast->flags);

  for (size_t i = 0; i < mozilla::ArrayLength(gWindowsDllBlocklist) - 1; ++i) {
    auto pEntry = pFirst + i;

    // Validate name
    EXPECT_TRUE(!!pEntry->name);
    EXPECT_GT(strlen(pEntry->name), 3);

    // Check the filename for valid characters.
    for (auto pch = pEntry->name; *pch != 0; ++pch) {
      EXPECT_FALSE(*pch >= 'A' && *pch <= 'Z');
    }

    // Check for duplicate entries
    for (auto&& dupe : dupes) {
      EXPECT_NE(stricmp(dupe, pEntry->name), 0);
    }

    dupes.AppendElement(pEntry->name);
  }
}

TEST(TestDllBlocklist, BlockThreadWithLoadLibraryEntryPoint)
{
  // Only supported on Nightly
#if defined(NIGHTLY_BUILD)
  using ThreadProc = unsigned(__stdcall*)(void*);

  NS_NAMED_LITERAL_STRING(kLeafNameW, "TestDllBlocklist_MatchByVersion.dll");

  nsString fullPathW = GetFullPath(kLeafNameW);
  EXPECT_FALSE(fullPathW.IsEmpty());

  nsAutoHandle threadW(reinterpret_cast<HANDLE>(
      _beginthreadex(nullptr, 0, reinterpret_cast<ThreadProc>(&::LoadLibraryW),
                     (void*)fullPathW.get(), 0, nullptr)));

  EXPECT_TRUE(!!threadW);
  EXPECT_EQ(::WaitForSingleObject(threadW, INFINITE), WAIT_OBJECT_0);

  DWORD exitCode;
  EXPECT_TRUE(::GetExitCodeThread(threadW, &exitCode) && !exitCode);
  EXPECT_TRUE(!::GetModuleHandleW(kLeafNameW.get()));

  const NS_LossyConvertUTF16toASCII fullPathA(fullPathW);
  EXPECT_FALSE(fullPathA.IsEmpty());

  nsAutoHandle threadA(reinterpret_cast<HANDLE>(
      _beginthreadex(nullptr, 0, reinterpret_cast<ThreadProc>(&::LoadLibraryA),
                     (void*)fullPathA.get(), 0, nullptr)));

  EXPECT_TRUE(!!threadA);
  EXPECT_EQ(::WaitForSingleObject(threadA, INFINITE), WAIT_OBJECT_0);
  EXPECT_TRUE(::GetExitCodeThread(threadA, &exitCode) && !exitCode);
  EXPECT_TRUE(!::GetModuleHandleW(kLeafNameW.get()));
#endif  // defined(NIGHTLY_BUILD)
>>>>>>> upstream-releases
}
