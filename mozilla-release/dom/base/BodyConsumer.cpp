/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "BodyConsumer.h"

#include "mozilla/dom/BlobBinding.h"
#include "mozilla/dom/BlobURLProtocolHandler.h"
#include "mozilla/dom/BodyUtil.h"
#include "mozilla/dom/File.h"
#include "mozilla/dom/FileBinding.h"
#include "mozilla/dom/FileCreatorHelper.h"
#include "mozilla/dom/MutableBlobStreamListener.h"
#include "mozilla/dom/Promise.h"
#include "mozilla/dom/PromiseNativeHandler.h"
#include "mozilla/dom/WorkerCommon.h"
#include "mozilla/dom/WorkerPrivate.h"
#include "mozilla/dom/WorkerRef.h"
#include "mozilla/dom/WorkerRunnable.h"
#include "mozilla/dom/WorkerScope.h"
#include "mozilla/ipc/PBackgroundSharedTypes.h"
#include "nsIThreadRetargetableRequest.h"
#include "nsIStreamLoader.h"
#include "nsProxyRelease.h"

// Undefine the macro of CreateFile to avoid FileCreatorHelper#CreateFile being
// replaced by FileCreatorHelper#CreateFileW.
#ifdef CreateFile
#  undef CreateFile
#endif

namespace mozilla {
namespace dom {

namespace {

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
class BeginConsumeBodyRunnable final : public Runnable {
 public:
  BeginConsumeBodyRunnable(FetchBodyConsumer<Derived>* aConsumer,
||||||| merged common ancestors
template <class Derived>
class BeginConsumeBodyRunnable final : public Runnable
{
public:
  BeginConsumeBodyRunnable(FetchBodyConsumer<Derived>* aConsumer,
=======
class BeginConsumeBodyRunnable final : public Runnable {
 public:
  BeginConsumeBodyRunnable(BodyConsumer* aConsumer,
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
                           ThreadSafeWorkerRef* aWorkerRef)
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
      : Runnable("BeginConsumeBodyRunnable"),
        mFetchBodyConsumer(aConsumer),
        mWorkerRef(aWorkerRef) {}
||||||| merged common ancestors
    : Runnable("BeginConsumeBodyRunnable")
    , mFetchBodyConsumer(aConsumer)
    , mWorkerRef(aWorkerRef)
  { }
=======
      : Runnable("BeginConsumeBodyRunnable"),
        mBodyConsumer(aConsumer),
        mWorkerRef(aWorkerRef) {}
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp

  NS_IMETHOD
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
  Run() override {
    mFetchBodyConsumer->BeginConsumeBodyMainThread(mWorkerRef);
||||||| merged common ancestors
  Run() override
  {
    mFetchBodyConsumer->BeginConsumeBodyMainThread(mWorkerRef);
=======
  Run() override {
    mBodyConsumer->BeginConsumeBodyMainThread(mWorkerRef);
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
    return NS_OK;
  }

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
 private:
  RefPtr<FetchBodyConsumer<Derived>> mFetchBodyConsumer;
||||||| merged common ancestors
private:
  RefPtr<FetchBodyConsumer<Derived>> mFetchBodyConsumer;
=======
 private:
  RefPtr<BodyConsumer> mBodyConsumer;
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  RefPtr<ThreadSafeWorkerRef> mWorkerRef;
};

/*
 * Called on successfully reading the complete stream.
 */
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
class ContinueConsumeBodyRunnable final : public MainThreadWorkerRunnable {
  RefPtr<FetchBodyConsumer<Derived>> mFetchBodyConsumer;
||||||| merged common ancestors
template <class Derived>
class ContinueConsumeBodyRunnable final : public MainThreadWorkerRunnable
{
  RefPtr<FetchBodyConsumer<Derived>> mFetchBodyConsumer;
=======
class ContinueConsumeBodyRunnable final : public MainThreadWorkerRunnable {
  RefPtr<BodyConsumer> mBodyConsumer;
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  nsresult mStatus;
  uint32_t mLength;
  uint8_t* mResult;

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
 public:
  ContinueConsumeBodyRunnable(FetchBodyConsumer<Derived>* aFetchBodyConsumer,
                              WorkerPrivate* aWorkerPrivate, nsresult aStatus,
                              uint32_t aLength, uint8_t* aResult)
      : MainThreadWorkerRunnable(aWorkerPrivate),
        mFetchBodyConsumer(aFetchBodyConsumer),
        mStatus(aStatus),
        mLength(aLength),
        mResult(aResult) {
||||||| merged common ancestors
public:
  ContinueConsumeBodyRunnable(FetchBodyConsumer<Derived>* aFetchBodyConsumer,
                              WorkerPrivate* aWorkerPrivate,
                              nsresult aStatus, uint32_t aLength,
                              uint8_t* aResult)
    : MainThreadWorkerRunnable(aWorkerPrivate)
    , mFetchBodyConsumer(aFetchBodyConsumer)
    , mStatus(aStatus)
    , mLength(aLength)
    , mResult(aResult)
  {
=======
 public:
  ContinueConsumeBodyRunnable(BodyConsumer* aBodyConsumer,
                              WorkerPrivate* aWorkerPrivate, nsresult aStatus,
                              uint32_t aLength, uint8_t* aResult)
      : MainThreadWorkerRunnable(aWorkerPrivate),
        mBodyConsumer(aBodyConsumer),
        mStatus(aStatus),
        mLength(aLength),
        mResult(aResult) {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
    MOZ_ASSERT(NS_IsMainThread());
  }

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
  bool WorkerRun(JSContext* aCx, WorkerPrivate* aWorkerPrivate) override {
    mFetchBodyConsumer->ContinueConsumeBody(mStatus, mLength, mResult);
||||||| merged common ancestors
  bool
  WorkerRun(JSContext* aCx, WorkerPrivate* aWorkerPrivate) override
  {
    mFetchBodyConsumer->ContinueConsumeBody(mStatus, mLength, mResult);
=======
  bool WorkerRun(JSContext* aCx, WorkerPrivate* aWorkerPrivate) override {
    mBodyConsumer->ContinueConsumeBody(mStatus, mLength, mResult);
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
    return true;
  }
};

// ControlRunnable used to complete the releasing of resources on the worker
// thread when already shutting down.
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
class AbortConsumeBodyControlRunnable final
    : public MainThreadWorkerControlRunnable {
  RefPtr<FetchBodyConsumer<Derived>> mFetchBodyConsumer;
||||||| merged common ancestors
template <class Derived>
class AbortConsumeBodyControlRunnable final : public MainThreadWorkerControlRunnable
{
  RefPtr<FetchBodyConsumer<Derived>> mFetchBodyConsumer;
=======
class AbortConsumeBodyControlRunnable final
    : public MainThreadWorkerControlRunnable {
  RefPtr<BodyConsumer> mBodyConsumer;
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
 public:
  AbortConsumeBodyControlRunnable(
      FetchBodyConsumer<Derived>* aFetchBodyConsumer,
      WorkerPrivate* aWorkerPrivate)
      : MainThreadWorkerControlRunnable(aWorkerPrivate),
        mFetchBodyConsumer(aFetchBodyConsumer) {
||||||| merged common ancestors
public:
  AbortConsumeBodyControlRunnable(FetchBodyConsumer<Derived>* aFetchBodyConsumer,
                                  WorkerPrivate* aWorkerPrivate)
    : MainThreadWorkerControlRunnable(aWorkerPrivate)
    , mFetchBodyConsumer(aFetchBodyConsumer)
  {
=======
 public:
  AbortConsumeBodyControlRunnable(BodyConsumer* aBodyConsumer,
                                  WorkerPrivate* aWorkerPrivate)
      : MainThreadWorkerControlRunnable(aWorkerPrivate),
        mBodyConsumer(aBodyConsumer) {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
    MOZ_ASSERT(NS_IsMainThread());
  }

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
  bool WorkerRun(JSContext* aCx, WorkerPrivate* aWorkerPrivate) override {
    mFetchBodyConsumer->ContinueConsumeBody(NS_BINDING_ABORTED, 0, nullptr,
                                            true /* shutting down */);
||||||| merged common ancestors
  bool
  WorkerRun(JSContext* aCx, WorkerPrivate* aWorkerPrivate) override
  {
    mFetchBodyConsumer->ContinueConsumeBody(NS_BINDING_ABORTED, 0, nullptr,
                                            true /* shutting down */);
=======
  bool WorkerRun(JSContext* aCx, WorkerPrivate* aWorkerPrivate) override {
    mBodyConsumer->ContinueConsumeBody(NS_BINDING_ABORTED, 0, nullptr,
                                       true /* shutting down */);
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
    return true;
  }
};

/*
 * In case of failure to create a stream pump or dispatch stream completion to
 * worker, ensure we cleanup properly. Thread agnostic.
 */
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
class MOZ_STACK_CLASS AutoFailConsumeBody final {
 public:
  AutoFailConsumeBody(FetchBodyConsumer<Derived>* aBodyConsumer,
||||||| merged common ancestors
template <class Derived>
class MOZ_STACK_CLASS AutoFailConsumeBody final
{
public:
  AutoFailConsumeBody(FetchBodyConsumer<Derived>* aBodyConsumer,
=======
class MOZ_STACK_CLASS AutoFailConsumeBody final {
 public:
  AutoFailConsumeBody(BodyConsumer* aBodyConsumer,
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
                      ThreadSafeWorkerRef* aWorkerRef)
      : mBodyConsumer(aBodyConsumer), mWorkerRef(aWorkerRef) {}

  ~AutoFailConsumeBody() {
    AssertIsOnMainThread();

    if (!mBodyConsumer) {
      return;
    }

    // Web Worker
    if (mWorkerRef) {
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
      RefPtr<AbortConsumeBodyControlRunnable<Derived>> r =
          new AbortConsumeBodyControlRunnable<Derived>(mBodyConsumer,
                                                       mWorkerRef->Private());
||||||| merged common ancestors
      RefPtr<AbortConsumeBodyControlRunnable<Derived>> r =
        new AbortConsumeBodyControlRunnable<Derived>(mBodyConsumer,
                                                     mWorkerRef->Private());
=======
      RefPtr<AbortConsumeBodyControlRunnable> r =
          new AbortConsumeBodyControlRunnable(mBodyConsumer,
                                              mWorkerRef->Private());
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
      if (!r->Dispatch()) {
        MOZ_CRASH("We are going to leak");
      }
      return;
    }

    // Main-thread
    mBodyConsumer->ContinueConsumeBody(NS_ERROR_FAILURE, 0, nullptr);
  }

  void DontFail() { mBodyConsumer = nullptr; }

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
 private:
  RefPtr<FetchBodyConsumer<Derived>> mBodyConsumer;
||||||| merged common ancestors
private:
  RefPtr<FetchBodyConsumer<Derived>> mBodyConsumer;
=======
 private:
  RefPtr<BodyConsumer> mBodyConsumer;
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  RefPtr<ThreadSafeWorkerRef> mWorkerRef;
};

/*
 * Called on successfully reading the complete stream for Blob.
 */
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
class ContinueConsumeBlobBodyRunnable final : public MainThreadWorkerRunnable {
  RefPtr<FetchBodyConsumer<Derived>> mFetchBodyConsumer;
||||||| merged common ancestors
template <class Derived>
class ContinueConsumeBlobBodyRunnable final : public MainThreadWorkerRunnable
{
  RefPtr<FetchBodyConsumer<Derived>> mFetchBodyConsumer;
=======
class ContinueConsumeBlobBodyRunnable final : public MainThreadWorkerRunnable {
  RefPtr<BodyConsumer> mBodyConsumer;
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  RefPtr<BlobImpl> mBlobImpl;

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
 public:
  ContinueConsumeBlobBodyRunnable(
      FetchBodyConsumer<Derived>* aFetchBodyConsumer,
      WorkerPrivate* aWorkerPrivate, BlobImpl* aBlobImpl)
      : MainThreadWorkerRunnable(aWorkerPrivate),
        mFetchBodyConsumer(aFetchBodyConsumer),
        mBlobImpl(aBlobImpl) {
||||||| merged common ancestors
public:
  ContinueConsumeBlobBodyRunnable(FetchBodyConsumer<Derived>* aFetchBodyConsumer,
                                  WorkerPrivate* aWorkerPrivate,
                                  BlobImpl* aBlobImpl)
    : MainThreadWorkerRunnable(aWorkerPrivate)
    , mFetchBodyConsumer(aFetchBodyConsumer)
    , mBlobImpl(aBlobImpl)
  {
=======
 public:
  ContinueConsumeBlobBodyRunnable(BodyConsumer* aBodyConsumer,
                                  WorkerPrivate* aWorkerPrivate,
                                  BlobImpl* aBlobImpl)
      : MainThreadWorkerRunnable(aWorkerPrivate),
        mBodyConsumer(aBodyConsumer),
        mBlobImpl(aBlobImpl) {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
    MOZ_ASSERT(NS_IsMainThread());
    MOZ_ASSERT(mBlobImpl);
  }

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
  bool WorkerRun(JSContext* aCx, WorkerPrivate* aWorkerPrivate) override {
    mFetchBodyConsumer->ContinueConsumeBlobBody(mBlobImpl);
||||||| merged common ancestors
  bool
  WorkerRun(JSContext* aCx, WorkerPrivate* aWorkerPrivate) override
  {
    mFetchBodyConsumer->ContinueConsumeBlobBody(mBlobImpl);
=======
  bool WorkerRun(JSContext* aCx, WorkerPrivate* aWorkerPrivate) override {
    mBodyConsumer->ContinueConsumeBlobBody(mBlobImpl);
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
    return true;
  }
};

// ControlRunnable used to complete the releasing of resources on the worker
// thread when already shutting down.
class AbortConsumeBlobBodyControlRunnable final
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
    : public MainThreadWorkerControlRunnable {
  RefPtr<FetchBodyConsumer<Derived>> mFetchBodyConsumer;
||||||| merged common ancestors
  : public MainThreadWorkerControlRunnable
{
  RefPtr<FetchBodyConsumer<Derived>> mFetchBodyConsumer;
=======
    : public MainThreadWorkerControlRunnable {
  RefPtr<BodyConsumer> mBodyConsumer;
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
 public:
  AbortConsumeBlobBodyControlRunnable(
      FetchBodyConsumer<Derived>* aFetchBodyConsumer,
      WorkerPrivate* aWorkerPrivate)
      : MainThreadWorkerControlRunnable(aWorkerPrivate),
        mFetchBodyConsumer(aFetchBodyConsumer) {
||||||| merged common ancestors
public:
  AbortConsumeBlobBodyControlRunnable(FetchBodyConsumer<Derived>* aFetchBodyConsumer,
                                      WorkerPrivate* aWorkerPrivate)
    : MainThreadWorkerControlRunnable(aWorkerPrivate)
    , mFetchBodyConsumer(aFetchBodyConsumer)
  {
=======
 public:
  AbortConsumeBlobBodyControlRunnable(BodyConsumer* aBodyConsumer,
                                      WorkerPrivate* aWorkerPrivate)
      : MainThreadWorkerControlRunnable(aWorkerPrivate),
        mBodyConsumer(aBodyConsumer) {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
    MOZ_ASSERT(NS_IsMainThread());
  }

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
  bool WorkerRun(JSContext* aCx, WorkerPrivate* aWorkerPrivate) override {
    mFetchBodyConsumer->ContinueConsumeBlobBody(nullptr,
                                                true /* shutting down */);
||||||| merged common ancestors
  bool
  WorkerRun(JSContext* aCx, WorkerPrivate* aWorkerPrivate) override
  {
    mFetchBodyConsumer->ContinueConsumeBlobBody(nullptr,
                                                true /* shutting down */);
=======
  bool WorkerRun(JSContext* aCx, WorkerPrivate* aWorkerPrivate) override {
    mBodyConsumer->ContinueConsumeBlobBody(nullptr, true /* shutting down */);
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
    return true;
  }
};

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
class ConsumeBodyDoneObserver final : public nsIStreamLoaderObserver,
                                      public MutableBlobStorageCallback {
 public:
||||||| merged common ancestors
template <class Derived>
class ConsumeBodyDoneObserver final : public nsIStreamLoaderObserver
                                    , public MutableBlobStorageCallback
{
public:
=======
class ConsumeBodyDoneObserver final : public nsIStreamLoaderObserver,
                                      public MutableBlobStorageCallback {
 public:
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  NS_DECL_THREADSAFE_ISUPPORTS

  ConsumeBodyDoneObserver(BodyConsumer* aBodyConsumer,
                          ThreadSafeWorkerRef* aWorkerRef)
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
      : mFetchBodyConsumer(aFetchBodyConsumer), mWorkerRef(aWorkerRef) {}
||||||| merged common ancestors
    : mFetchBodyConsumer(aFetchBodyConsumer)
    , mWorkerRef(aWorkerRef)
  { }
=======
      : mBodyConsumer(aBodyConsumer), mWorkerRef(aWorkerRef) {}
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp

  NS_IMETHOD
  OnStreamComplete(nsIStreamLoader* aLoader, nsISupports* aCtxt,
                   nsresult aStatus, uint32_t aResultLength,
                   const uint8_t* aResult) override {
    MOZ_ASSERT(NS_IsMainThread());

    // The loading is completed. Let's nullify the pump before continuing the
    // consuming of the body.
    mBodyConsumer->NullifyConsumeBodyPump();

    uint8_t* nonconstResult = const_cast<uint8_t*>(aResult);

    // Main-thread.
    if (!mWorkerRef) {
      mBodyConsumer->ContinueConsumeBody(aStatus, aResultLength,
                                         nonconstResult);
      // The caller is responsible for data.
      return NS_SUCCESS_ADOPTED_DATA;
    }

    // Web Worker.
    {
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
      RefPtr<ContinueConsumeBodyRunnable<Derived>> r =
          new ContinueConsumeBodyRunnable<Derived>(
              mFetchBodyConsumer, mWorkerRef->Private(), aStatus, aResultLength,
              nonconstResult);
||||||| merged common ancestors
      RefPtr<ContinueConsumeBodyRunnable<Derived>> r =
        new ContinueConsumeBodyRunnable<Derived>(mFetchBodyConsumer,
                                                 mWorkerRef->Private(),
                                                 aStatus,
                                                 aResultLength,
                                                 nonconstResult);
=======
      RefPtr<ContinueConsumeBodyRunnable> r = new ContinueConsumeBodyRunnable(
          mBodyConsumer, mWorkerRef->Private(), aStatus, aResultLength,
          nonconstResult);
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
      if (r->Dispatch()) {
        // The caller is responsible for data.
        return NS_SUCCESS_ADOPTED_DATA;
      }
    }

    // The worker is shutting down. Let's use a control runnable to complete the
    // shutting down procedure.

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
    RefPtr<AbortConsumeBodyControlRunnable<Derived>> r =
        new AbortConsumeBodyControlRunnable<Derived>(mFetchBodyConsumer,
                                                     mWorkerRef->Private());
||||||| merged common ancestors
    RefPtr<AbortConsumeBodyControlRunnable<Derived>> r =
      new AbortConsumeBodyControlRunnable<Derived>(mFetchBodyConsumer,
                                                   mWorkerRef->Private());
=======
    RefPtr<AbortConsumeBodyControlRunnable> r =
        new AbortConsumeBodyControlRunnable(mBodyConsumer,
                                            mWorkerRef->Private());
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
    if (NS_WARN_IF(!r->Dispatch())) {
      return NS_ERROR_FAILURE;
    }

    // We haven't taken ownership of the data.
    return NS_OK;
  }

  virtual void BlobStoreCompleted(MutableBlobStorage* aBlobStorage, Blob* aBlob,
                                  nsresult aRv) override {
    // On error.
    if (NS_FAILED(aRv)) {
      OnStreamComplete(nullptr, nullptr, aRv, 0, nullptr);
      return;
    }

    // The loading is completed. Let's nullify the pump before continuing the
    // consuming of the body.
    mBodyConsumer->NullifyConsumeBodyPump();

    mBodyConsumer->OnBlobResult(aBlob, mWorkerRef);
  }

 private:
  ~ConsumeBodyDoneObserver() = default;

  RefPtr<BodyConsumer> mBodyConsumer;
  RefPtr<ThreadSafeWorkerRef> mWorkerRef;
};

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
NS_IMPL_ADDREF(ConsumeBodyDoneObserver<Derived>)
template <class Derived>
NS_IMPL_RELEASE(ConsumeBodyDoneObserver<Derived>)
template <class Derived>
NS_INTERFACE_MAP_BEGIN(ConsumeBodyDoneObserver<Derived>)
NS_INTERFACE_MAP_ENTRY(nsIStreamLoaderObserver)
    NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIStreamLoaderObserver)
NS_INTERFACE_MAP_END

}  // anonymous

template <class Derived>
/* static */ already_AddRefed<Promise> FetchBodyConsumer<Derived>::Create(
    nsIGlobalObject* aGlobal, nsIEventTarget* aMainThreadEventTarget,
    FetchBody<Derived>* aBody, nsIInputStream* aBodyStream,
    AbortSignalImpl* aSignalImpl, FetchConsumeType aType, ErrorResult& aRv) {
  MOZ_ASSERT(aBody);
  MOZ_ASSERT(aBodyStream);
  MOZ_ASSERT(aMainThreadEventTarget);
||||||| merged common ancestors
template <class Derived>
NS_IMPL_ADDREF(ConsumeBodyDoneObserver<Derived>)
template <class Derived>
NS_IMPL_RELEASE(ConsumeBodyDoneObserver<Derived>)
template <class Derived>
NS_INTERFACE_MAP_BEGIN(ConsumeBodyDoneObserver<Derived>)
  NS_INTERFACE_MAP_ENTRY(nsIStreamLoaderObserver)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIStreamLoaderObserver)
NS_INTERFACE_MAP_END

} // anonymous

template <class Derived>
/* static */ already_AddRefed<Promise>
FetchBodyConsumer<Derived>::Create(nsIGlobalObject* aGlobal,
                                   nsIEventTarget* aMainThreadEventTarget,
                                   FetchBody<Derived>* aBody,
                                   AbortSignalImpl* aSignalImpl,
                                   FetchConsumeType aType,
                                   ErrorResult& aRv)
{
  MOZ_ASSERT(aBody);
  MOZ_ASSERT(aMainThreadEventTarget);
=======
NS_IMPL_ISUPPORTS(ConsumeBodyDoneObserver, nsIStreamLoaderObserver)
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
||||||| merged common ancestors
  nsCOMPtr<nsIInputStream> bodyStream;
  aBody->DerivedClass()->GetBody(getter_AddRefs(bodyStream));
  if (!bodyStream) {
    aRv = NS_NewCStringInputStream(getter_AddRefs(bodyStream), EmptyCString());
    if (NS_WARN_IF(aRv.Failed())) {
      return nullptr;
    }
  }

=======
}  // namespace

/* static */ already_AddRefed<Promise> BodyConsumer::Create(
    nsIGlobalObject* aGlobal, nsIEventTarget* aMainThreadEventTarget,
    nsIInputStream* aBodyStream, AbortSignalImpl* aSignalImpl,
    ConsumeType aType, const nsACString& aBodyBlobURISpec,
    const nsAString& aBodyLocalPath, const nsACString& aBodyMimeType,
    MutableBlobStorage::MutableBlobStorageType aBlobStorageType,
    ErrorResult& aRv) {
  MOZ_ASSERT(aBodyStream);
  MOZ_ASSERT(aMainThreadEventTarget);

>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  RefPtr<Promise> promise = Promise::Create(aGlobal, aRv);
  if (aRv.Failed()) {
    return nullptr;
  }

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
  RefPtr<FetchBodyConsumer<Derived>> consumer = new FetchBodyConsumer<Derived>(
      aMainThreadEventTarget, aGlobal, aBody, aBodyStream, promise, aType);
||||||| merged common ancestors
  RefPtr<FetchBodyConsumer<Derived>> consumer =
    new FetchBodyConsumer<Derived>(aMainThreadEventTarget, aGlobal,
                                   aBody, bodyStream, promise,
                                   aType);
=======
  RefPtr<BodyConsumer> consumer = new BodyConsumer(
      aMainThreadEventTarget, aGlobal, aBodyStream, promise, aType,
      aBodyBlobURISpec, aBodyLocalPath, aBodyMimeType, aBlobStorageType);
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp

  RefPtr<ThreadSafeWorkerRef> workerRef;

  if (!NS_IsMainThread()) {
    WorkerPrivate* workerPrivate = GetCurrentThreadWorkerPrivate();
    MOZ_ASSERT(workerPrivate);

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
    RefPtr<StrongWorkerRef> strongWorkerRef = StrongWorkerRef::Create(
        workerPrivate, "FetchBodyConsumer",
        [consumer]() { consumer->ShutDownMainThreadConsuming(); });
||||||| merged common ancestors
    RefPtr<StrongWorkerRef> strongWorkerRef =
      StrongWorkerRef::Create(workerPrivate, "FetchBodyConsumer",
                              [consumer]() {
        consumer->ShutDownMainThreadConsuming();
      });
=======
    RefPtr<StrongWorkerRef> strongWorkerRef = StrongWorkerRef::Create(
        workerPrivate, "BodyConsumer",
        [consumer]() { consumer->ShutDownMainThreadConsuming(); });
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
    if (NS_WARN_IF(!strongWorkerRef)) {
      aRv.Throw(NS_ERROR_FAILURE);
      return nullptr;
    }

    workerRef = new ThreadSafeWorkerRef(strongWorkerRef);
  } else {
    nsCOMPtr<nsIObserverService> os = mozilla::services::GetObserverService();
    if (NS_WARN_IF(!os)) {
      aRv.Throw(NS_ERROR_FAILURE);
      return nullptr;
    }

    aRv = os->AddObserver(consumer, DOM_WINDOW_DESTROYED_TOPIC, true);
    if (NS_WARN_IF(aRv.Failed())) {
      return nullptr;
    }

    aRv = os->AddObserver(consumer, DOM_WINDOW_FROZEN_TOPIC, true);
    if (NS_WARN_IF(aRv.Failed())) {
      return nullptr;
    }
  }

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
  nsCOMPtr<nsIRunnable> r =
      new BeginConsumeBodyRunnable<Derived>(consumer, workerRef);
||||||| merged common ancestors
  nsCOMPtr<nsIRunnable> r =
    new BeginConsumeBodyRunnable<Derived>(consumer, workerRef);
=======
  nsCOMPtr<nsIRunnable> r = new BeginConsumeBodyRunnable(consumer, workerRef);
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  aRv = aMainThreadEventTarget->Dispatch(r.forget(), NS_DISPATCH_NORMAL);
  if (NS_WARN_IF(aRv.Failed())) {
    return nullptr;
  }

  if (aSignalImpl) {
    consumer->Follow(aSignalImpl);
  }

  return promise.forget();
}

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
void FetchBodyConsumer<Derived>::ReleaseObject() {
||||||| merged common ancestors
template <class Derived>
void
FetchBodyConsumer<Derived>::ReleaseObject()
{
=======
void BodyConsumer::ReleaseObject() {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  AssertIsOnTargetThread();

  if (NS_IsMainThread()) {
    nsCOMPtr<nsIObserverService> os = mozilla::services::GetObserverService();
    if (os) {
      os->RemoveObserver(this, DOM_WINDOW_DESTROYED_TOPIC);
      os->RemoveObserver(this, DOM_WINDOW_FROZEN_TOPIC);
    }
  }

  mGlobal = nullptr;

  Unfollow();
}

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
FetchBodyConsumer<Derived>::FetchBodyConsumer(
    nsIEventTarget* aMainThreadEventTarget, nsIGlobalObject* aGlobalObject,
    FetchBody<Derived>* aBody, nsIInputStream* aBodyStream, Promise* aPromise,
    FetchConsumeType aType)
    : mTargetThread(NS_GetCurrentThread()),
      mMainThreadEventTarget(aMainThreadEventTarget)
#ifdef DEBUG
      ,
      mBody(aBody)
#endif
      ,
      mBodyStream(aBodyStream),
      mBlobStorageType(MutableBlobStorage::eOnlyInMemory),
      mBodyBlobURISpec(aBody ? aBody->BodyBlobURISpec() : VoidCString()),
      mBodyLocalPath(aBody ? aBody->BodyLocalPath() : VoidString()),
      mGlobal(aGlobalObject),
      mConsumeType(aType),
      mConsumePromise(aPromise),
      mBodyConsumed(false),
      mShuttingDown(false) {
||||||| merged common ancestors
template <class Derived>
FetchBodyConsumer<Derived>::FetchBodyConsumer(nsIEventTarget* aMainThreadEventTarget,
                                              nsIGlobalObject* aGlobalObject,
                                              FetchBody<Derived>* aBody,
                                              nsIInputStream* aBodyStream,
                                              Promise* aPromise,
                                              FetchConsumeType aType)
  : mTargetThread(NS_GetCurrentThread())
  , mMainThreadEventTarget(aMainThreadEventTarget)
#ifdef DEBUG
  , mBody(aBody)
#endif
  , mBodyStream(aBodyStream)
  , mBlobStorageType(MutableBlobStorage::eOnlyInMemory)
  , mBodyBlobURISpec(aBody ? aBody->BodyBlobURISpec() : VoidCString())
  , mBodyLocalPath(aBody ? aBody->BodyLocalPath() : VoidString())
  , mGlobal(aGlobalObject)
  , mConsumeType(aType)
  , mConsumePromise(aPromise)
  , mBodyConsumed(false)
  , mShuttingDown(false)
{
=======
BodyConsumer::BodyConsumer(
    nsIEventTarget* aMainThreadEventTarget, nsIGlobalObject* aGlobalObject,
    nsIInputStream* aBodyStream, Promise* aPromise, ConsumeType aType,
    const nsACString& aBodyBlobURISpec, const nsAString& aBodyLocalPath,
    const nsACString& aBodyMimeType,
    MutableBlobStorage::MutableBlobStorageType aBlobStorageType)
    : mTargetThread(NS_GetCurrentThread()),
      mMainThreadEventTarget(aMainThreadEventTarget),
      mBodyStream(aBodyStream),
      mBlobStorageType(aBlobStorageType),
      mBodyMimeType(aBodyMimeType),
      mBodyBlobURISpec(aBodyBlobURISpec),
      mBodyLocalPath(aBodyLocalPath),
      mGlobal(aGlobalObject),
      mConsumeType(aType),
      mConsumePromise(aPromise),
      mBodyConsumed(false),
      mShuttingDown(false) {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  MOZ_ASSERT(aMainThreadEventTarget);
  MOZ_ASSERT(aBodyStream);
  MOZ_ASSERT(aPromise);
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp

  const mozilla::UniquePtr<mozilla::ipc::PrincipalInfo>& principalInfo =
      aBody->DerivedClass()->GetPrincipalInfo();
  // We support temporary file for blobs only if the principal is known and
  // it's system or content not in private Browsing.
  if (principalInfo &&
      (principalInfo->type() ==
           mozilla::ipc::PrincipalInfo::TSystemPrincipalInfo ||
       (principalInfo->type() ==
            mozilla::ipc::PrincipalInfo::TContentPrincipalInfo &&
        principalInfo->get_ContentPrincipalInfo().attrs().mPrivateBrowsingId ==
            0))) {
    mBlobStorageType = MutableBlobStorage::eCouldBeInTemporaryFile;
  }

  mBodyMimeType = aBody->MimeType();
||||||| merged common ancestors

  const mozilla::UniquePtr<mozilla::ipc::PrincipalInfo>& principalInfo =
    aBody->DerivedClass()->GetPrincipalInfo();
  // We support temporary file for blobs only if the principal is known and
  // it's system or content not in private Browsing.
  if (principalInfo &&
      (principalInfo->type() == mozilla::ipc::PrincipalInfo::TSystemPrincipalInfo ||
       (principalInfo->type() == mozilla::ipc::PrincipalInfo::TContentPrincipalInfo &&
        principalInfo->get_ContentPrincipalInfo().attrs().mPrivateBrowsingId == 0))) {
    mBlobStorageType = MutableBlobStorage::eCouldBeInTemporaryFile;
  }

  mBodyMimeType = aBody->MimeType();
=======
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
}

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
FetchBodyConsumer<Derived>::~FetchBodyConsumer() {}
||||||| merged common ancestors
template <class Derived>
FetchBodyConsumer<Derived>::~FetchBodyConsumer()
{
}
=======
BodyConsumer::~BodyConsumer() = default;
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
void FetchBodyConsumer<Derived>::AssertIsOnTargetThread() const {
||||||| merged common ancestors
template <class Derived>
void
FetchBodyConsumer<Derived>::AssertIsOnTargetThread() const
{
=======
void BodyConsumer::AssertIsOnTargetThread() const {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  MOZ_ASSERT(NS_GetCurrentThread() == mTargetThread);
}

namespace {

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
class FileCreationHandler final : public PromiseNativeHandler {
 public:
||||||| merged common ancestors
template <class Derived>
class FileCreationHandler final : public PromiseNativeHandler
{
public:
=======
class FileCreationHandler final : public PromiseNativeHandler {
 public:
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  NS_DECL_THREADSAFE_ISUPPORTS

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
  static void Create(Promise* aPromise, FetchBodyConsumer<Derived>* aConsumer,
                     ThreadSafeWorkerRef* aWorkerRef) {
||||||| merged common ancestors
  static void
  Create(Promise* aPromise, FetchBodyConsumer<Derived>* aConsumer)
  {
=======
  static void Create(Promise* aPromise, BodyConsumer* aConsumer,
                     ThreadSafeWorkerRef* aWorkerRef) {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
    AssertIsOnMainThread();
    MOZ_ASSERT(aPromise);

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
    RefPtr<FileCreationHandler> handler =
        new FileCreationHandler<Derived>(aConsumer, aWorkerRef);
||||||| merged common ancestors
    RefPtr<FileCreationHandler> handler = new FileCreationHandler<Derived>(aConsumer);
=======
    RefPtr<FileCreationHandler> handler =
        new FileCreationHandler(aConsumer, aWorkerRef);
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
    aPromise->AppendNativeHandler(handler);
  }

  void ResolvedCallback(JSContext* aCx, JS::Handle<JS::Value> aValue) override {
    AssertIsOnMainThread();

    if (NS_WARN_IF(!aValue.isObject())) {
      mConsumer->OnBlobResult(nullptr, mWorkerRef);
      return;
    }

    RefPtr<Blob> blob;
    if (NS_WARN_IF(NS_FAILED(UNWRAP_OBJECT(Blob, &aValue.toObject(), blob)))) {
      mConsumer->OnBlobResult(nullptr, mWorkerRef);
      return;
    }

    mConsumer->OnBlobResult(blob, mWorkerRef);
  }

  void RejectedCallback(JSContext* aCx, JS::Handle<JS::Value> aValue) override {
    AssertIsOnMainThread();

    mConsumer->OnBlobResult(nullptr, mWorkerRef);
  }

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
 private:
  FileCreationHandler<Derived>(FetchBodyConsumer<Derived>* aConsumer,
                               ThreadSafeWorkerRef* aWorkerRef)
      : mConsumer(aConsumer), mWorkerRef(aWorkerRef) {
||||||| merged common ancestors
private:
  explicit FileCreationHandler<Derived>(FetchBodyConsumer<Derived>* aConsumer)
    : mConsumer(aConsumer)
  {
=======
 private:
  FileCreationHandler(BodyConsumer* aConsumer, ThreadSafeWorkerRef* aWorkerRef)
      : mConsumer(aConsumer), mWorkerRef(aWorkerRef) {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
    AssertIsOnMainThread();
    MOZ_ASSERT(aConsumer);
  }

  ~FileCreationHandler() = default;

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
  RefPtr<FetchBodyConsumer<Derived>> mConsumer;
  RefPtr<ThreadSafeWorkerRef> mWorkerRef;
||||||| merged common ancestors
  RefPtr<FetchBodyConsumer<Derived>> mConsumer;
=======
  RefPtr<BodyConsumer> mConsumer;
  RefPtr<ThreadSafeWorkerRef> mWorkerRef;
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
};

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
NS_IMPL_ADDREF(FileCreationHandler<Derived>)
template <class Derived>
NS_IMPL_RELEASE(FileCreationHandler<Derived>)
template <class Derived>
NS_INTERFACE_MAP_BEGIN(FileCreationHandler<Derived>)
NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

}  // namespace

template <class Derived>
nsresult FetchBodyConsumer<Derived>::GetBodyLocalFile(nsIFile** aFile) const {
||||||| merged common ancestors
template <class Derived>
NS_IMPL_ADDREF(FileCreationHandler<Derived>)
template <class Derived>
NS_IMPL_RELEASE(FileCreationHandler<Derived>)
template <class Derived>
NS_INTERFACE_MAP_BEGIN(FileCreationHandler<Derived>)
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

} // namespace

template <class Derived>
nsresult
FetchBodyConsumer<Derived>::GetBodyLocalFile(nsIFile** aFile) const
{
=======
NS_IMPL_ISUPPORTS0(FileCreationHandler)

}  // namespace

nsresult BodyConsumer::GetBodyLocalFile(nsIFile** aFile) const {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  AssertIsOnMainThread();

  if (!mBodyLocalPath.Length()) {
    return NS_OK;
  }

  nsresult rv;
  nsCOMPtr<nsIFile> file = do_CreateInstance("@mozilla.org/file/local;1", &rv);
  if (NS_FAILED(rv)) {
    return rv;
  }

  rv = file->InitWithPath(mBodyLocalPath);
  NS_ENSURE_SUCCESS(rv, rv);

  bool exists;
  rv = file->Exists(&exists);
  NS_ENSURE_SUCCESS(rv, rv);
  if (!exists) {
    return NS_ERROR_FILE_NOT_FOUND;
  }

  bool isDir;
  rv = file->IsDirectory(&isDir);
  NS_ENSURE_SUCCESS(rv, rv);
  if (isDir) {
    return NS_ERROR_FILE_IS_DIRECTORY;
  }

  file.forget(aFile);
  return NS_OK;
}

/*
 * BeginConsumeBodyMainThread() will automatically reject the consume promise
 * and clean up on any failures, so there is no need for callers to do so,
 * reflected in a lack of error return code.
 */
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
void FetchBodyConsumer<Derived>::BeginConsumeBodyMainThread(
    ThreadSafeWorkerRef* aWorkerRef) {
||||||| merged common ancestors
template <class Derived>
void
FetchBodyConsumer<Derived>::BeginConsumeBodyMainThread(ThreadSafeWorkerRef* aWorkerRef)
{
=======
void BodyConsumer::BeginConsumeBodyMainThread(ThreadSafeWorkerRef* aWorkerRef) {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  AssertIsOnMainThread();

  AutoFailConsumeBody autoReject(this, aWorkerRef);

  if (mShuttingDown) {
    // We haven't started yet, but we have been terminated. AutoFailConsumeBody
    // will dispatch a runnable to release resources.
    return;
  }

  if (mConsumeType == CONSUME_BLOB) {
    nsresult rv;

    // If we're trying to consume a blob, and the request was for a blob URI,
    // then just consume that URI's blob instance.
    if (!mBodyBlobURISpec.IsEmpty()) {
      RefPtr<BlobImpl> blobImpl;
      rv = NS_GetBlobForBlobURISpec(mBodyBlobURISpec, getter_AddRefs(blobImpl));
      if (NS_WARN_IF(NS_FAILED(rv)) || !blobImpl) {
        return;
      }
      autoReject.DontFail();
      DispatchContinueConsumeBlobBody(blobImpl, aWorkerRef);
      return;
    }

    // If we're trying to consume a blob, and the request was for a local
    // file, then generate and return a File blob.
    nsCOMPtr<nsIFile> file;
    rv = GetBodyLocalFile(getter_AddRefs(file));
    if (!NS_WARN_IF(NS_FAILED(rv)) && file) {
      ChromeFilePropertyBag bag;
      bag.mType = NS_ConvertUTF8toUTF16(mBodyMimeType);

      ErrorResult error;
      RefPtr<Promise> promise =
          FileCreatorHelper::CreateFile(mGlobal, file, bag, true, error);
      if (NS_WARN_IF(error.Failed())) {
        return;
      }

      autoReject.DontFail();
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
      FileCreationHandler<Derived>::Create(promise, this, aWorkerRef);
||||||| merged common ancestors
=======
      FileCreationHandler::Create(promise, this, aWorkerRef);
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
      return;
    }
  }

  nsCOMPtr<nsIInputStreamPump> pump;
  nsresult rv =
      NS_NewInputStreamPump(getter_AddRefs(pump), mBodyStream.forget(), 0, 0,
                            false, mMainThreadEventTarget);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return;
  }

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
  RefPtr<ConsumeBodyDoneObserver<Derived>> p =
      new ConsumeBodyDoneObserver<Derived>(this, aWorkerRef);
||||||| merged common ancestors
  RefPtr<ConsumeBodyDoneObserver<Derived>> p =
   new ConsumeBodyDoneObserver<Derived>(this, aWorkerRef);
=======
  RefPtr<ConsumeBodyDoneObserver> p =
      new ConsumeBodyDoneObserver(this, aWorkerRef);
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp

  nsCOMPtr<nsIStreamListener> listener;
  if (mConsumeType == CONSUME_BLOB) {
    listener = new MutableBlobStreamListener(
        mBlobStorageType, nullptr, mBodyMimeType, p, mMainThreadEventTarget);
  } else {
    nsCOMPtr<nsIStreamLoader> loader;
    rv = NS_NewStreamLoader(getter_AddRefs(loader), p);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return;
    }

    listener = loader;
  }

  rv = pump->AsyncRead(listener, nullptr);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return;
  }

  // Now that everything succeeded, we can assign the pump to a pointer that
  // stays alive for the lifetime of the BodyConsumer.
  mConsumeBodyPump = pump;

  // It is ok for retargeting to fail and reads to happen on the main thread.
  autoReject.DontFail();

  // Try to retarget, otherwise fall back to main thread.
  nsCOMPtr<nsIThreadRetargetableRequest> rr = do_QueryInterface(pump);
  if (rr) {
    nsCOMPtr<nsIEventTarget> sts =
        do_GetService(NS_STREAMTRANSPORTSERVICE_CONTRACTID);
    rv = rr->RetargetDeliveryTo(sts);
    if (NS_FAILED(rv)) {
      NS_WARNING("Retargeting failed");
    }
  }
}

/*
 * OnBlobResult() is called when a blob body is ready to be consumed (when its
 * network transfer completes in BeginConsumeBodyRunnable or its local File has
 * been wrapped by FileCreationHandler). The blob is sent to the target thread
 * and ContinueConsumeBody is called.
 */
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
void FetchBodyConsumer<Derived>::OnBlobResult(Blob* aBlob,
                                              ThreadSafeWorkerRef* aWorkerRef) {
  AssertIsOnMainThread();

  DispatchContinueConsumeBlobBody(aBlob ? aBlob->Impl() : nullptr, aWorkerRef);
}

template <class Derived>
void FetchBodyConsumer<Derived>::DispatchContinueConsumeBlobBody(
    BlobImpl* aBlobImpl, ThreadSafeWorkerRef* aWorkerRef) {
  AssertIsOnMainThread();
||||||| merged common ancestors
template <class Derived>
void
FetchBodyConsumer<Derived>::OnBlobResult(Blob* aBlob, ThreadSafeWorkerRef* aWorkerRef)
{
  MOZ_ASSERT(aBlob);
=======
void BodyConsumer::OnBlobResult(Blob* aBlob, ThreadSafeWorkerRef* aWorkerRef) {
  AssertIsOnMainThread();

  DispatchContinueConsumeBlobBody(aBlob ? aBlob->Impl() : nullptr, aWorkerRef);
}

void BodyConsumer::DispatchContinueConsumeBlobBody(
    BlobImpl* aBlobImpl, ThreadSafeWorkerRef* aWorkerRef) {
  AssertIsOnMainThread();
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp

  // Main-thread.
  if (!aWorkerRef) {
    if (aBlobImpl) {
      ContinueConsumeBlobBody(aBlobImpl);
    } else {
      ContinueConsumeBody(NS_ERROR_DOM_ABORT_ERR, 0, nullptr);
    }
    return;
  }

  // Web Worker.
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
  if (aBlobImpl) {
    RefPtr<ContinueConsumeBlobBodyRunnable<Derived>> r =
        new ContinueConsumeBlobBodyRunnable<Derived>(
            this, aWorkerRef->Private(), aBlobImpl);

    if (r->Dispatch()) {
      return;
    }
  } else {
    RefPtr<ContinueConsumeBodyRunnable<Derived>> r =
        new ContinueConsumeBodyRunnable<Derived>(
            this, aWorkerRef->Private(), NS_ERROR_DOM_ABORT_ERR, 0, nullptr);
||||||| merged common ancestors
  {
    RefPtr<ContinueConsumeBlobBodyRunnable<Derived>> r =
      new ContinueConsumeBlobBodyRunnable<Derived>(this, aWorkerRef->Private(),
                                                   aBlob->Impl());
=======
  if (aBlobImpl) {
    RefPtr<ContinueConsumeBlobBodyRunnable> r =
        new ContinueConsumeBlobBodyRunnable(this, aWorkerRef->Private(),
                                            aBlobImpl);

    if (r->Dispatch()) {
      return;
    }
  } else {
    RefPtr<ContinueConsumeBodyRunnable> r = new ContinueConsumeBodyRunnable(
        this, aWorkerRef->Private(), NS_ERROR_DOM_ABORT_ERR, 0, nullptr);
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp

    if (r->Dispatch()) {
      return;
    }
  }

  // The worker is shutting down. Let's use a control runnable to complete the
  // shutting down procedure.

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
  RefPtr<AbortConsumeBlobBodyControlRunnable<Derived>> r =
      new AbortConsumeBlobBodyControlRunnable<Derived>(this,
                                                       aWorkerRef->Private());
||||||| merged common ancestors
  RefPtr<AbortConsumeBlobBodyControlRunnable<Derived>> r =
    new AbortConsumeBlobBodyControlRunnable<Derived>(this,
                                                     aWorkerRef->Private());
=======
  RefPtr<AbortConsumeBlobBodyControlRunnable> r =
      new AbortConsumeBlobBodyControlRunnable(this, aWorkerRef->Private());
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp

  Unused << NS_WARN_IF(!r->Dispatch());
}

/*
 * ContinueConsumeBody() is to be called on the target thread whenever the
 * final result of the fetch is known. The fetch promise is resolved or
 * rejected based on whether the fetch succeeded, and the body can be
 * converted into the expected type of JS object.
 */
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
void FetchBodyConsumer<Derived>::ContinueConsumeBody(nsresult aStatus,
                                                     uint32_t aResultLength,
                                                     uint8_t* aResult,
                                                     bool aShuttingDown) {
||||||| merged common ancestors
template <class Derived>
void
FetchBodyConsumer<Derived>::ContinueConsumeBody(nsresult aStatus,
                                                uint32_t aResultLength,
                                                uint8_t* aResult,
                                                bool aShuttingDown)
{
=======
void BodyConsumer::ContinueConsumeBody(nsresult aStatus, uint32_t aResultLength,
                                       uint8_t* aResult, bool aShuttingDown) {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  AssertIsOnTargetThread();

  // This makes sure that we free the data correctly.
  auto autoFree = mozilla::MakeScopeExit([&] { free(aResult); });

  if (mBodyConsumed) {
    return;
  }
  mBodyConsumed = true;

  MOZ_ASSERT(mConsumePromise);
  RefPtr<Promise> localPromise = mConsumePromise.forget();

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
  RefPtr<FetchBodyConsumer<Derived>> self = this;
  auto autoReleaseObject =
      mozilla::MakeScopeExit([self] { self->ReleaseObject(); });
||||||| merged common ancestors
  RefPtr<FetchBodyConsumer<Derived>> self = this;
  auto autoReleaseObject = mozilla::MakeScopeExit([self] {
    self->ReleaseObject();
  });
=======
  RefPtr<BodyConsumer> self = this;
  auto autoReleaseObject =
      mozilla::MakeScopeExit([self] { self->ReleaseObject(); });
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp

  if (aShuttingDown) {
    // If shutting down, we don't want to resolve any promise.
    return;
  }

  if (NS_WARN_IF(NS_FAILED(aStatus))) {
    // Per
    // https://fetch.spec.whatwg.org/#concept-read-all-bytes-from-readablestream
    // Decoding errors should reject with a TypeError
    if (aStatus == NS_ERROR_INVALID_CONTENT_ENCODING) {
      IgnoredErrorResult rv;
      rv.ThrowTypeError<MSG_DOM_DECODING_FAILED>();
      localPromise->MaybeReject(rv);
    } else {
      localPromise->MaybeReject(NS_ERROR_DOM_ABORT_ERR);
    }
  }

  // Don't warn here since we warned above.
  if (NS_FAILED(aStatus)) {
    return;
  }

  // Finish successfully consuming body according to type.
  MOZ_ASSERT(aResult);

  AutoJSAPI jsapi;
  if (!jsapi.Init(mGlobal)) {
    localPromise->MaybeReject(NS_ERROR_UNEXPECTED);
    return;
  }

  JSContext* cx = jsapi.cx();
  ErrorResult error;

  switch (mConsumeType) {
    case CONSUME_ARRAYBUFFER: {
      JS::Rooted<JSObject*> arrayBuffer(cx);
      BodyUtil::ConsumeArrayBuffer(cx, &arrayBuffer, aResultLength, aResult,
                                   error);

      if (!error.Failed()) {
        JS::Rooted<JS::Value> val(cx);
        val.setObjectOrNull(arrayBuffer);

        localPromise->MaybeResolve(val);
        // ArrayBuffer takes over ownership.
        aResult = nullptr;
      }
      break;
    }
    case CONSUME_BLOB: {
      MOZ_CRASH("This should not happen.");
      break;
    }
    case CONSUME_FORMDATA: {
      nsCString data;
      data.Adopt(reinterpret_cast<char*>(aResult), aResultLength);
      aResult = nullptr;

      RefPtr<dom::FormData> fd =
          BodyUtil::ConsumeFormData(mGlobal, mBodyMimeType, data, error);
      if (!error.Failed()) {
        localPromise->MaybeResolve(fd);
      }
      break;
    }
    case CONSUME_TEXT:
      // fall through handles early exit.
    case CONSUME_JSON: {
      nsString decoded;
      if (NS_SUCCEEDED(
              BodyUtil::ConsumeText(aResultLength, aResult, decoded))) {
        if (mConsumeType == CONSUME_TEXT) {
          localPromise->MaybeResolve(decoded);
        } else {
          JS::Rooted<JS::Value> json(cx);
          BodyUtil::ConsumeJson(cx, &json, decoded, error);
          if (!error.Failed()) {
            localPromise->MaybeResolve(json);
          }
        }
      };
      break;
    }
    default:
      MOZ_ASSERT_UNREACHABLE("Unexpected consume body type");
  }

  error.WouldReportJSException();
  if (error.Failed()) {
    localPromise->MaybeReject(error);
  }
}

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
void FetchBodyConsumer<Derived>::ContinueConsumeBlobBody(BlobImpl* aBlobImpl,
                                                         bool aShuttingDown) {
||||||| merged common ancestors
template <class Derived>
void
FetchBodyConsumer<Derived>::ContinueConsumeBlobBody(BlobImpl* aBlobImpl,
                                                    bool aShuttingDown)
{
=======
void BodyConsumer::ContinueConsumeBlobBody(BlobImpl* aBlobImpl,
                                           bool aShuttingDown) {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  AssertIsOnTargetThread();
  MOZ_ASSERT(mConsumeType == CONSUME_BLOB);

  if (mBodyConsumed) {
    return;
  }
  mBodyConsumed = true;

  MOZ_ASSERT(mConsumePromise);
  RefPtr<Promise> localPromise = mConsumePromise.forget();

  if (!aShuttingDown) {
    RefPtr<dom::Blob> blob = dom::Blob::Create(mGlobal, aBlobImpl);
    MOZ_ASSERT(blob);

    localPromise->MaybeResolve(blob);
  }

  ReleaseObject();
}

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
void FetchBodyConsumer<Derived>::ShutDownMainThreadConsuming() {
||||||| merged common ancestors
template <class Derived>
void
FetchBodyConsumer<Derived>::ShutDownMainThreadConsuming()
{
=======
void BodyConsumer::ShutDownMainThreadConsuming() {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  if (!NS_IsMainThread()) {
    RefPtr<BodyConsumer> self = this;

    nsCOMPtr<nsIRunnable> r = NS_NewRunnableFunction(
<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
        "FetchBodyConsumer::ShutDownMainThreadConsuming",
        [self]() { self->ShutDownMainThreadConsuming(); });
||||||| merged common ancestors
      "FetchBodyConsumer::ShutDownMainThreadConsuming",
      [self] () { self->ShutDownMainThreadConsuming(); });
=======
        "BodyConsumer::ShutDownMainThreadConsuming",
        [self]() { self->ShutDownMainThreadConsuming(); });
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp

    mMainThreadEventTarget->Dispatch(r.forget(), NS_DISPATCH_NORMAL);
    return;
  }

  // We need this because maybe, mConsumeBodyPump has not been created yet. We
  // must be sure that we don't try to do it.
  mShuttingDown = true;

  if (mConsumeBodyPump) {
    mConsumeBodyPump->Cancel(NS_BINDING_ABORTED);
    mConsumeBodyPump = nullptr;
  }
}

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
NS_IMETHODIMP FetchBodyConsumer<Derived>::Observe(nsISupports* aSubject,
                                                  const char* aTopic,
                                                  const char16_t* aData) {
||||||| merged common ancestors
template <class Derived>
NS_IMETHODIMP
FetchBodyConsumer<Derived>::Observe(nsISupports* aSubject,
                                    const char* aTopic,
                                    const char16_t* aData)
{
=======
NS_IMETHODIMP BodyConsumer::Observe(nsISupports* aSubject, const char* aTopic,
                                    const char16_t* aData) {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  AssertIsOnMainThread();

  MOZ_ASSERT((strcmp(aTopic, DOM_WINDOW_FROZEN_TOPIC) == 0) ||
             (strcmp(aTopic, DOM_WINDOW_DESTROYED_TOPIC) == 0));

  nsCOMPtr<nsPIDOMWindowInner> window = do_QueryInterface(mGlobal);
  if (SameCOMIdentity(aSubject, window)) {
    ContinueConsumeBody(NS_BINDING_ABORTED, 0, nullptr);
  }

  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
void FetchBodyConsumer<Derived>::Abort() {
||||||| merged common ancestors
template <class Derived>
void
FetchBodyConsumer<Derived>::Abort()
{
=======
void BodyConsumer::Abort() {
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp
  AssertIsOnTargetThread();
  ShutDownMainThreadConsuming();
  ContinueConsumeBody(NS_ERROR_DOM_ABORT_ERR, 0, nullptr);
}

<<<<<<< HEAD:mozilla-release/dom/fetch/FetchConsumer.cpp
template <class Derived>
NS_IMPL_ADDREF(FetchBodyConsumer<Derived>)

template <class Derived>
NS_IMPL_RELEASE(FetchBodyConsumer<Derived>)

template <class Derived>
NS_IMPL_QUERY_INTERFACE(FetchBodyConsumer<Derived>, nsIObserver,
                        nsISupportsWeakReference)
||||||| merged common ancestors
template <class Derived>
NS_IMPL_ADDREF(FetchBodyConsumer<Derived>)

template <class Derived>
NS_IMPL_RELEASE(FetchBodyConsumer<Derived>)

template <class Derived>
NS_IMPL_QUERY_INTERFACE(FetchBodyConsumer<Derived>,
                        nsIObserver,
                        nsISupportsWeakReference)
=======
NS_IMPL_ISUPPORTS(BodyConsumer, nsIObserver, nsISupportsWeakReference)
>>>>>>> upstream-releases:mozilla-release/dom/base/BodyConsumer.cpp

}  // namespace dom
}  // namespace mozilla
