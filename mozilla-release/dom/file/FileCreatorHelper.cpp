/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "FileCreatorHelper.h"

#include "mozilla/dom/BindingDeclarations.h"
#include "mozilla/dom/FileBinding.h"
#include "mozilla/dom/FileCreatorChild.h"
#include "mozilla/ipc/BackgroundChild.h"
#include "mozilla/ipc/PBackgroundChild.h"
#include "mozilla/dom/File.h"
#include "mozilla/dom/Promise.h"
#include "mozilla/StaticPrefs.h"
#include "nsContentUtils.h"
#include "nsPIDOMWindow.h"
#include "nsProxyRelease.h"
#include "nsIFile.h"

// Undefine the macro of CreateFile to avoid FileCreatorHelper#CreateFile being
// replaced by FileCreatorHelper#CreateFileW.
#ifdef CreateFile
#  undef CreateFile
#endif

namespace mozilla {
namespace dom {

<<<<<<< HEAD
/* static */ already_AddRefed<Promise> FileCreatorHelper::CreateFile(
    nsIGlobalObject* aGlobalObject, nsIFile* aFile,
    const ChromeFilePropertyBag& aBag, bool aIsFromNsIFile, ErrorResult& aRv) {
||||||| merged common ancestors
/* static */ already_AddRefed<Promise>
FileCreatorHelper::CreateFile(nsIGlobalObject* aGlobalObject,
                              nsIFile* aFile,
                              const ChromeFilePropertyBag& aBag,
                              bool aIsFromNsIFile,
                              ErrorResult& aRv)
{
=======
/* static */
already_AddRefed<Promise> FileCreatorHelper::CreateFile(
    nsIGlobalObject* aGlobalObject, nsIFile* aFile,
    const ChromeFilePropertyBag& aBag, bool aIsFromNsIFile, ErrorResult& aRv) {
>>>>>>> upstream-releases
  MOZ_DIAGNOSTIC_ASSERT(NS_IsMainThread());

  RefPtr<Promise> promise = Promise::Create(aGlobalObject, aRv);
  if (NS_WARN_IF(aRv.Failed())) {
    return nullptr;
  }

<<<<<<< HEAD
  nsCOMPtr<nsPIDOMWindowInner> window = do_QueryInterface(aGlobalObject);

  // Parent process

  if (XRE_IsParentProcess()) {
    RefPtr<File> file =
        CreateFileInternal(window, aFile, aBag, aIsFromNsIFile, aRv);
    if (aRv.Failed()) {
      return nullptr;
    }
    promise->MaybeResolve(file);
    return promise.forget();
  }

  // Content process.

  ContentChild* cc = ContentChild::GetSingleton();
  if (!cc) {
    promise->MaybeReject(NS_ERROR_DOM_INVALID_STATE_ERR);
    return promise.forget();
  }

  if (!cc->GetRemoteType().EqualsLiteral(FILE_REMOTE_TYPE) &&
      !Preferences::GetBool("dom.file.createInChild", false)) {
    // If this pref is not set and the request is received by the parent
    // process, this child is killed for security reason.
    promise->MaybeReject(NS_ERROR_DOM_INVALID_STATE_ERR);
    return promise.forget();
  }

  RefPtr<FileCreatorHelper> helper = new FileCreatorHelper(promise, window);

  // The request is sent to the parent process and it's kept alive by
  // ContentChild.
  helper->SendRequest(aFile, aBag, aIsFromNsIFile, aRv);
  if (NS_WARN_IF(aRv.Failed())) {
    return nullptr;
  }

  return promise.forget();
}

/* static */ already_AddRefed<File> FileCreatorHelper::CreateFileInternal(
    nsPIDOMWindowInner* aWindow, nsIFile* aFile,
    const ChromeFilePropertyBag& aBag, bool aIsFromNsIFile, ErrorResult& aRv) {
  bool lastModifiedPassed = false;
  int64_t lastModified = 0;
  if (aBag.mLastModified.WasPassed()) {
    lastModifiedPassed = true;
    lastModified = aBag.mLastModified.Value();
  }

  RefPtr<BlobImpl> blobImpl;
  aRv = CreateBlobImpl(aFile, aBag.mType, aBag.mName, lastModifiedPassed,
                       lastModified, aBag.mExistenceCheck, aIsFromNsIFile,
                       getter_AddRefs(blobImpl));
  if (aRv.Failed()) {
    return nullptr;
  }

  RefPtr<File> file = File::Create(aWindow, blobImpl);
  return file.forget();
}

FileCreatorHelper::FileCreatorHelper(Promise* aPromise,
                                     nsPIDOMWindowInner* aWindow)
    : mPromise(aPromise), mWindow(aWindow) {
  MOZ_ASSERT(aPromise);
}

FileCreatorHelper::~FileCreatorHelper() {}

void FileCreatorHelper::SendRequest(nsIFile* aFile,
                                    const ChromeFilePropertyBag& aBag,
                                    bool aIsFromNsIFile, ErrorResult& aRv) {
  MOZ_ASSERT(aFile);

  ContentChild* cc = ContentChild::GetSingleton();
  if (NS_WARN_IF(!cc)) {
    aRv.Throw(NS_ERROR_FAILURE);
    return;
  }

  nsID uuid;
  aRv = nsContentUtils::GenerateUUIDInPlace(uuid);
  if (NS_WARN_IF(aRv.Failed())) {
    return;
  }

||||||| merged common ancestors
  nsCOMPtr<nsPIDOMWindowInner> window = do_QueryInterface(aGlobalObject);

  // Parent process

  if (XRE_IsParentProcess()) {
    RefPtr<File> file =
      CreateFileInternal(window, aFile, aBag, aIsFromNsIFile, aRv);
    if (aRv.Failed()) {
      return nullptr;
    }
    promise->MaybeResolve(file);
    return promise.forget();
  }

  // Content process.

  ContentChild* cc = ContentChild::GetSingleton();
  if (!cc) {
    promise->MaybeReject(NS_ERROR_DOM_INVALID_STATE_ERR);
    return promise.forget();
  }

  if (!cc->GetRemoteType().EqualsLiteral(FILE_REMOTE_TYPE) &&
      !Preferences::GetBool("dom.file.createInChild", false)) {
    // If this pref is not set and the request is received by the parent
    // process, this child is killed for security reason.
    promise->MaybeReject(NS_ERROR_DOM_INVALID_STATE_ERR);
    return promise.forget();
  }

  RefPtr<FileCreatorHelper> helper = new FileCreatorHelper(promise, window);

  // The request is sent to the parent process and it's kept alive by
  // ContentChild.
  helper->SendRequest(aFile, aBag, aIsFromNsIFile, aRv);
  if (NS_WARN_IF(aRv.Failed())) {
    return nullptr;
  }

  return promise.forget();
}

/* static */ already_AddRefed<File>
FileCreatorHelper::CreateFileInternal(nsPIDOMWindowInner* aWindow,
                                      nsIFile* aFile,
                                      const ChromeFilePropertyBag& aBag,
                                      bool aIsFromNsIFile,
                                      ErrorResult& aRv)
{
  bool lastModifiedPassed = false;
  int64_t lastModified = 0;
  if (aBag.mLastModified.WasPassed()) {
    lastModifiedPassed = true;
    lastModified = aBag.mLastModified.Value();
  }

  RefPtr<BlobImpl> blobImpl;
  aRv = CreateBlobImpl(aFile, aBag.mType, aBag.mName, lastModifiedPassed,
                       lastModified, aBag.mExistenceCheck, aIsFromNsIFile,
                       getter_AddRefs(blobImpl));
  if (aRv.Failed()) {
     return nullptr;
  }

  RefPtr<File> file = File::Create(aWindow, blobImpl);
  return file.forget();
}

FileCreatorHelper::FileCreatorHelper(Promise* aPromise,
                                     nsPIDOMWindowInner* aWindow)
  : mPromise(aPromise)
  , mWindow(aWindow)
{
  MOZ_ASSERT(aPromise);
}

FileCreatorHelper::~FileCreatorHelper()
{
}

void
FileCreatorHelper::SendRequest(nsIFile* aFile,
                               const ChromeFilePropertyBag& aBag,
                               bool aIsFromNsIFile,
                               ErrorResult& aRv)
{
  MOZ_ASSERT(aFile);

  ContentChild* cc = ContentChild::GetSingleton();
  if (NS_WARN_IF(!cc)) {
    aRv.Throw(NS_ERROR_FAILURE);
    return;
  }

  nsID uuid;
  aRv = nsContentUtils::GenerateUUIDInPlace(uuid);
  if (NS_WARN_IF(aRv.Failed())) {
    return;
  }

=======
>>>>>>> upstream-releases
  nsAutoString path;
  aRv = aFile->GetPath(path);
  if (NS_WARN_IF(aRv.Failed())) {
<<<<<<< HEAD
    return;
  }

  cc->FileCreationRequest(uuid, this, path, aBag.mType, aBag.mName,
                          aBag.mLastModified, aBag.mExistenceCheck,
                          aIsFromNsIFile);
}

void FileCreatorHelper::ResponseReceived(BlobImpl* aBlobImpl, nsresult aRv) {
  if (NS_FAILED(aRv)) {
    mPromise->MaybeReject(aRv);
    return;
  }

  RefPtr<File> file = File::Create(mWindow, aBlobImpl);
  mPromise->MaybeResolve(file);
}

/* static */ nsresult FileCreatorHelper::CreateBlobImplForIPC(
    const nsAString& aPath, const nsAString& aType, const nsAString& aName,
    bool aLastModifiedPassed, int64_t aLastModified, bool aExistenceCheck,
    bool aIsFromNsIFile, BlobImpl** aBlobImpl) {
  nsCOMPtr<nsIFile> file;
  nsresult rv = NS_NewLocalFile(aPath, true, getter_AddRefs(file));
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
||||||| merged common ancestors
    return;
  }

  cc->FileCreationRequest(uuid, this, path, aBag.mType, aBag.mName,
                          aBag.mLastModified, aBag.mExistenceCheck,
                          aIsFromNsIFile);
}

void
FileCreatorHelper::ResponseReceived(BlobImpl* aBlobImpl, nsresult aRv)
{
  if (NS_FAILED(aRv)) {
    mPromise->MaybeReject(aRv);
    return;
  }

  RefPtr<File> file = File::Create(mWindow, aBlobImpl);
  mPromise->MaybeResolve(file);
}

/* static */ nsresult
FileCreatorHelper::CreateBlobImplForIPC(const nsAString& aPath,
                                        const nsAString& aType,
                                        const nsAString& aName,
                                        bool aLastModifiedPassed,
                                        int64_t aLastModified,
                                        bool aExistenceCheck,
                                        bool aIsFromNsIFile,
                                        BlobImpl** aBlobImpl)
{
  nsCOMPtr<nsIFile> file;
  nsresult rv = NS_NewLocalFile(aPath, true, getter_AddRefs(file));
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
=======
    return nullptr;
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  return CreateBlobImpl(file, aType, aName, aLastModifiedPassed, aLastModified,
                        aExistenceCheck, aIsFromNsIFile, aBlobImpl);
}

/* static */ nsresult FileCreatorHelper::CreateBlobImpl(
    nsIFile* aFile, const nsAString& aType, const nsAString& aName,
    bool aLastModifiedPassed, int64_t aLastModified, bool aExistenceCheck,
    bool aIsFromNsIFile, BlobImpl** aBlobImpl) {
  if (!aExistenceCheck) {
    RefPtr<FileBlobImpl> impl = new FileBlobImpl(aFile);

    if (!aName.IsEmpty()) {
      impl->SetName(aName);
    }

    if (!aType.IsEmpty()) {
      impl->SetType(aType);
    }

    if (aLastModifiedPassed) {
      impl->SetLastModified(aLastModified);
    }

    impl.forget(aBlobImpl);
    return NS_OK;
||||||| merged common ancestors
  return CreateBlobImpl(file, aType, aName, aLastModifiedPassed, aLastModified,
                        aExistenceCheck, aIsFromNsIFile, aBlobImpl);
}

/* static */ nsresult
FileCreatorHelper::CreateBlobImpl(nsIFile* aFile,
                                  const nsAString& aType,
                                  const nsAString& aName,
                                  bool aLastModifiedPassed,
                                  int64_t aLastModified,
                                  bool aExistenceCheck,
                                  bool aIsFromNsIFile,
                                  BlobImpl** aBlobImpl)
{
  if (!aExistenceCheck) {
    RefPtr<FileBlobImpl> impl = new FileBlobImpl(aFile);

    if (!aName.IsEmpty()) {
      impl->SetName(aName);
    }

    if (!aType.IsEmpty()) {
      impl->SetType(aType);
    }

    if (aLastModifiedPassed) {
      impl->SetLastModified(aLastModified);
    }

    impl.forget(aBlobImpl);
    return NS_OK;
=======
  // Register this component to PBackground.
  mozilla::ipc::PBackgroundChild* actorChild =
      mozilla::ipc::BackgroundChild::GetOrCreateForCurrentThread();
  if (NS_WARN_IF(!actorChild)) {
    aRv.Throw(NS_ERROR_FAILURE);
    return nullptr;
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  RefPtr<MultipartBlobImpl> impl = new MultipartBlobImpl(EmptyString());
  nsresult rv = impl->InitializeChromeFile(
      aFile, aType, aName, aLastModifiedPassed, aLastModified, aIsFromNsIFile);
  if (NS_FAILED(rv)) {
    return rv;
||||||| merged common ancestors
  RefPtr<MultipartBlobImpl> impl = new MultipartBlobImpl(EmptyString());
  nsresult rv =
    impl->InitializeChromeFile(aFile, aType, aName, aLastModifiedPassed,
                               aLastModified, aIsFromNsIFile);
  if (NS_FAILED(rv)) {
    return rv;
=======
  Maybe<int64_t> lastModified;
  if (aBag.mLastModified.WasPassed()) {
    lastModified.emplace(aBag.mLastModified.Value());
>>>>>>> upstream-releases
  }

  PFileCreatorChild* actor = actorChild->SendPFileCreatorConstructor(
      path, aBag.mType, aBag.mName, lastModified, aBag.mExistenceCheck,
      aIsFromNsIFile);

  static_cast<FileCreatorChild*>(actor)->SetPromise(promise);
  return promise.forget();
}

}  // namespace dom
}  // namespace mozilla
