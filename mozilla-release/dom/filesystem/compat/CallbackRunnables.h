/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_ErrorCallbackRunnable_h
#define mozilla_dom_ErrorCallbackRunnable_h

#include "FileSystemDirectoryEntry.h"
#include "mozilla/dom/BindingDeclarations.h"
#include "mozilla/dom/PromiseNativeHandler.h"

class nsIGlobalObject;

namespace mozilla {
namespace dom {

class FileSystemEntriesCallback;

class EntryCallbackRunnable final : public Runnable {
 public:
  EntryCallbackRunnable(FileSystemEntryCallback* aCallback,
                        FileSystemEntry* aEntry);

  // MOZ_CAN_RUN_SCRIPT_BOUNDARY until Runnable::Run is MOZ_CAN_RUN_SCRIPT.  See
  // bug 1535398.
  MOZ_CAN_RUN_SCRIPT_BOUNDARY NS_IMETHOD Run() override;

<<<<<<< HEAD
 private:
  RefPtr<FileSystemEntryCallback> mCallback;
  RefPtr<FileSystemEntry> mEntry;
||||||| merged common ancestors
private:
  RefPtr<FileSystemEntryCallback> mCallback;
  RefPtr<FileSystemEntry> mEntry;
=======
 private:
  const RefPtr<FileSystemEntryCallback> mCallback;
  const RefPtr<FileSystemEntry> mEntry;
>>>>>>> upstream-releases
};

class ErrorCallbackRunnable final : public Runnable {
 public:
  ErrorCallbackRunnable(nsIGlobalObject* aGlobalObject,
                        ErrorCallback* aCallback, nsresult aError);

  // MOZ_CAN_RUN_SCRIPT_BOUNDARY until Runnable::Run is MOZ_CAN_RUN_SCRIPT.  See
  // bug 1535398.
  MOZ_CAN_RUN_SCRIPT_BOUNDARY NS_IMETHOD Run() override;

 private:
  nsCOMPtr<nsIGlobalObject> mGlobal;
  const RefPtr<ErrorCallback> mCallback;
  nsresult mError;
};

class EmptyEntriesCallbackRunnable final : public Runnable {
 public:
  explicit EmptyEntriesCallbackRunnable(FileSystemEntriesCallback* aCallback);

  // MOZ_CAN_RUN_SCRIPT_BOUNDARY until Runnable::Run is MOZ_CAN_RUN_SCRIPT.  See
  // bug 1535398.
  MOZ_CAN_RUN_SCRIPT_BOUNDARY NS_IMETHOD Run() override;

<<<<<<< HEAD
 private:
  RefPtr<FileSystemEntriesCallback> mCallback;
||||||| merged common ancestors
private:
  RefPtr<FileSystemEntriesCallback> mCallback;
=======
 private:
  const RefPtr<FileSystemEntriesCallback> mCallback;
>>>>>>> upstream-releases
};

class GetEntryHelper final : public PromiseNativeHandler {
 public:
  NS_DECL_ISUPPORTS

  GetEntryHelper(FileSystemDirectoryEntry* aParentEntry, Directory* aDirectory,
                 nsTArray<nsString>& aParts, FileSystem* aFileSystem,
                 FileSystemEntryCallback* aSuccessCallback,
                 ErrorCallback* aErrorCallback,
                 FileSystemDirectoryEntry::GetInternalType aType);

  void Run();

<<<<<<< HEAD
  virtual void ResolvedCallback(JSContext* aCx,
                                JS::Handle<JS::Value> aValue) override;
||||||| merged common ancestors
  virtual void
  ResolvedCallback(JSContext* aCx, JS::Handle<JS::Value> aValue) override;
=======
  MOZ_CAN_RUN_SCRIPT
  virtual void ResolvedCallback(JSContext* aCx,
                                JS::Handle<JS::Value> aValue) override;
>>>>>>> upstream-releases

  virtual void RejectedCallback(JSContext* aCx,
                                JS::Handle<JS::Value> aValue) override;

 private:
  ~GetEntryHelper();

  void Error(nsresult aError);

  void ContinueRunning(JSObject* aObj);

<<<<<<< HEAD
  void CompleteOperation(JSObject* aObj);
||||||| merged common ancestors
  void
  CompleteOperation(JSObject* aObj);
=======
  MOZ_CAN_RUN_SCRIPT void CompleteOperation(JSObject* aObj);
>>>>>>> upstream-releases

  RefPtr<FileSystemDirectoryEntry> mParentEntry;
  RefPtr<Directory> mDirectory;
  nsTArray<nsString> mParts;
  RefPtr<FileSystem> mFileSystem;

  const RefPtr<FileSystemEntryCallback> mSuccessCallback;
  RefPtr<ErrorCallback> mErrorCallback;

  FileSystemDirectoryEntry::GetInternalType mType;
};

class FileSystemEntryCallbackHelper {
 public:
  static void Call(
      nsIGlobalObject* aGlobalObject,
      const Optional<OwningNonNull<FileSystemEntryCallback>>& aEntryCallback,
      FileSystemEntry* aEntry);
};

class ErrorCallbackHelper {
 public:
  static void Call(nsIGlobalObject* aGlobal,
                   const Optional<OwningNonNull<ErrorCallback>>& aErrorCallback,
                   nsresult aError);
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_CallbackRunnables_h
