/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsMemoryReporterManager.h"
#include "MemoryReportRequest.h"
<<<<<<< HEAD
#include "mozilla/RDDParent.h"
#include "mozilla/Unused.h"
#include "mozilla/dom/ContentChild.h"
#include "mozilla/gfx/GPUParent.h"
||||||| merged common ancestors
#include "mozilla/Unused.h"
#include "mozilla/dom/ContentChild.h"
#include "mozilla/gfx/GPUParent.h"
=======
#include "mozilla/ipc/FileDescriptor.h"
#include "mozilla/ipc/FileDescriptorUtils.h"

using namespace mozilla::ipc;
>>>>>>> upstream-releases

namespace mozilla {
namespace dom {

MemoryReportRequestHost::MemoryReportRequestHost(uint32_t aGeneration)
    : mGeneration(aGeneration), mSuccess(false) {
  MOZ_COUNT_CTOR(MemoryReportRequestHost);
  mReporterManager = nsMemoryReporterManager::GetOrCreate();
  NS_WARNING_ASSERTION(mReporterManager, "GetOrCreate failed");
}

void MemoryReportRequestHost::RecvReport(const MemoryReport& aReport) {
  // Skip reports from older generations. We need to do this here since we
  // could receive older reports from a subprocesses before it acknowledges
  // a new request, and we only track one active request per process.
  if (aReport.generation() != mGeneration) {
    return;
  }

  if (mReporterManager) {
    mReporterManager->HandleChildReport(mGeneration, aReport);
  }
}

void MemoryReportRequestHost::Finish(uint32_t aGeneration) {
  // Skip reports from older generations. See the comment in RecvReport.
  if (mGeneration != aGeneration) {
    return;
  }
  mSuccess = true;
}

MemoryReportRequestHost::~MemoryReportRequestHost() {
  MOZ_COUNT_DTOR(MemoryReportRequestHost);

  if (mReporterManager) {
    mReporterManager->EndProcessReport(mGeneration, mSuccess);
    mReporterManager = nullptr;
  }
}

NS_IMPL_ISUPPORTS(MemoryReportRequestClient, nsIRunnable)

<<<<<<< HEAD
/* static */ void MemoryReportRequestClient::Start(
    uint32_t aGeneration, bool aAnonymize, bool aMinimizeMemoryUsage,
    const MaybeFileDesc& aDMDFile, const nsACString& aProcessString) {
||||||| merged common ancestors
/* static */ void
MemoryReportRequestClient::Start(uint32_t aGeneration,
                                 bool aAnonymize,
                                 bool aMinimizeMemoryUsage,
                                 const MaybeFileDesc& aDMDFile,
                                 const nsACString& aProcessString)
{
=======
/* static */ void MemoryReportRequestClient::Start(
    uint32_t aGeneration, bool aAnonymize, bool aMinimizeMemoryUsage,
    const Maybe<FileDescriptor>& aDMDFile, const nsACString& aProcessString,
    const ReportCallback& aReportCallback,
    const FinishCallback& aFinishCallback) {
>>>>>>> upstream-releases
  RefPtr<MemoryReportRequestClient> request = new MemoryReportRequestClient(
<<<<<<< HEAD
      aGeneration, aAnonymize, aDMDFile, aProcessString);
||||||| merged common ancestors
    aGeneration,
    aAnonymize,
    aDMDFile,
    aProcessString);
=======
      aGeneration, aAnonymize, aDMDFile, aProcessString, aReportCallback,
      aFinishCallback);
>>>>>>> upstream-releases

  DebugOnly<nsresult> rv;
  if (aMinimizeMemoryUsage) {
    nsCOMPtr<nsIMemoryReporterManager> mgr =
        do_GetService("@mozilla.org/memory-reporter-manager;1");
    rv = mgr->MinimizeMemoryUsage(request);
    // mgr will eventually call actor->Run()
  } else {
    rv = request->Run();
  }

  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "actor operation failed");
}

<<<<<<< HEAD
MemoryReportRequestClient::MemoryReportRequestClient(
    uint32_t aGeneration, bool aAnonymize, const MaybeFileDesc& aDMDFile,
    const nsACString& aProcessString)
    : mGeneration(aGeneration),
      mAnonymize(aAnonymize),
      mProcessString(aProcessString) {
  if (aDMDFile.type() == MaybeFileDesc::TFileDescriptor) {
    mDMDFile = aDMDFile.get_FileDescriptor();
||||||| merged common ancestors
MemoryReportRequestClient::MemoryReportRequestClient(uint32_t aGeneration,
                                                     bool aAnonymize,
                                                     const MaybeFileDesc& aDMDFile,
                                                     const nsACString& aProcessString)
 : mGeneration(aGeneration),
   mAnonymize(aAnonymize),
   mProcessString(aProcessString)
{
  if (aDMDFile.type() == MaybeFileDesc::TFileDescriptor) {
    mDMDFile = aDMDFile.get_FileDescriptor();
=======
MemoryReportRequestClient::MemoryReportRequestClient(
    uint32_t aGeneration, bool aAnonymize,
    const Maybe<FileDescriptor>& aDMDFile, const nsACString& aProcessString,
    const ReportCallback& aReportCallback,
    const FinishCallback& aFinishCallback)
    : mGeneration(aGeneration),
      mAnonymize(aAnonymize),
      mProcessString(aProcessString),
      mReportCallback(aReportCallback),
      mFinishCallback(aFinishCallback) {
  if (aDMDFile.isSome()) {
    mDMDFile = aDMDFile.value();
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
MemoryReportRequestClient::~MemoryReportRequestClient() {}
||||||| merged common ancestors
MemoryReportRequestClient::~MemoryReportRequestClient()
{
}
=======
MemoryReportRequestClient::~MemoryReportRequestClient() {}

class HandleReportCallback final : public nsIHandleReportCallback {
 public:
  using ReportCallback = typename MemoryReportRequestClient::ReportCallback;
>>>>>>> upstream-releases

<<<<<<< HEAD
class HandleReportCallback final : public nsIHandleReportCallback {
 public:
||||||| merged common ancestors
class HandleReportCallback final : public nsIHandleReportCallback
{
public:
=======
>>>>>>> upstream-releases
  NS_DECL_ISUPPORTS

  explicit HandleReportCallback(uint32_t aGeneration,
<<<<<<< HEAD
                                const nsACString& aProcess)
      : mGeneration(aGeneration), mProcess(aProcess) {}
||||||| merged common ancestors
                                const nsACString& aProcess)
  : mGeneration(aGeneration)
  , mProcess(aProcess)
  { }
=======
                                const nsACString& aProcess,
                                const ReportCallback& aReportCallback)
      : mGeneration(aGeneration),
        mProcess(aProcess),
        mReportCallback(aReportCallback) {}
>>>>>>> upstream-releases

  NS_IMETHOD Callback(const nsACString& aProcess, const nsACString& aPath,
                      int32_t aKind, int32_t aUnits, int64_t aAmount,
                      const nsACString& aDescription,
<<<<<<< HEAD
                      nsISupports* aUnused) override {
    MemoryReport memreport(mProcess, nsCString(aPath), aKind, aUnits, aAmount,
                           mGeneration, nsCString(aDescription));
    switch (XRE_GetProcessType()) {
      case GeckoProcessType_Content:
        ContentChild::GetSingleton()->SendAddMemoryReport(memreport);
        break;
      case GeckoProcessType_GPU:
        Unused << gfx::GPUParent::GetSingleton()->SendAddMemoryReport(
            memreport);
        break;
      case GeckoProcessType_RDD:
        Unused << RDDParent::GetSingleton()->SendAddMemoryReport(memreport);
        break;
      default:
        MOZ_ASSERT_UNREACHABLE("Unhandled process type");
    }
||||||| merged common ancestors
                      nsISupports* aUnused) override
  {
    MemoryReport memreport(mProcess, nsCString(aPath), aKind, aUnits,
                           aAmount, mGeneration, nsCString(aDescription));
    switch (XRE_GetProcessType()) {
      case GeckoProcessType_Content:
        ContentChild::GetSingleton()->SendAddMemoryReport(memreport);
        break;
      case GeckoProcessType_GPU:
        Unused << gfx::GPUParent::GetSingleton()->SendAddMemoryReport(memreport);
        break;
      default:
        MOZ_ASSERT_UNREACHABLE("Unhandled process type");
    }
=======
                      nsISupports* aUnused) override {
    MemoryReport memreport(mProcess, nsCString(aPath), aKind, aUnits, aAmount,
                           mGeneration, nsCString(aDescription));
    mReportCallback(memreport);
>>>>>>> upstream-releases
    return NS_OK;
  }

 private:
  ~HandleReportCallback() = default;

  uint32_t mGeneration;
  const nsCString mProcess;
  ReportCallback mReportCallback;
};

<<<<<<< HEAD
NS_IMPL_ISUPPORTS(HandleReportCallback, nsIHandleReportCallback)
||||||| merged common ancestors
NS_IMPL_ISUPPORTS(
  HandleReportCallback
, nsIHandleReportCallback
)
=======
NS_IMPL_ISUPPORTS(HandleReportCallback, nsIHandleReportCallback)

class FinishReportingCallback final : public nsIFinishReportingCallback {
 public:
  using FinishCallback = typename MemoryReportRequestClient::FinishCallback;
>>>>>>> upstream-releases

<<<<<<< HEAD
class FinishReportingCallback final : public nsIFinishReportingCallback {
 public:
||||||| merged common ancestors
class FinishReportingCallback final : public nsIFinishReportingCallback
{
public:
=======
>>>>>>> upstream-releases
  NS_DECL_ISUPPORTS

<<<<<<< HEAD
  explicit FinishReportingCallback(uint32_t aGeneration)
      : mGeneration(aGeneration) {}
||||||| merged common ancestors
  explicit FinishReportingCallback(uint32_t aGeneration)
  : mGeneration(aGeneration)
  {
  }
=======
  explicit FinishReportingCallback(uint32_t aGeneration,
                                   const FinishCallback& aFinishCallback)
      : mGeneration(aGeneration), mFinishCallback(aFinishCallback) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  NS_IMETHOD Callback(nsISupports* aUnused) override {
    bool sent = false;
    switch (XRE_GetProcessType()) {
      case GeckoProcessType_Content:
        sent =
            ContentChild::GetSingleton()->SendFinishMemoryReport(mGeneration);
        break;
      case GeckoProcessType_GPU:
        sent =
            gfx::GPUParent::GetSingleton()->SendFinishMemoryReport(mGeneration);
        break;
      case GeckoProcessType_RDD:
        sent = RDDParent::GetSingleton()->SendFinishMemoryReport(mGeneration);
        break;
      default:
        MOZ_ASSERT_UNREACHABLE("Unhandled process type");
    }
    return sent ? NS_OK : NS_ERROR_FAILURE;
||||||| merged common ancestors
  NS_IMETHOD Callback(nsISupports* aUnused) override
  {
    bool sent = false;
    switch (XRE_GetProcessType()) {
      case GeckoProcessType_Content:
        sent = ContentChild::GetSingleton()->SendFinishMemoryReport(mGeneration);
        break;
      case GeckoProcessType_GPU:
        sent = gfx::GPUParent::GetSingleton()->SendFinishMemoryReport(mGeneration);
        break;
      default:
        MOZ_ASSERT_UNREACHABLE("Unhandled process type");
    }
    return sent ? NS_OK : NS_ERROR_FAILURE;
=======
  NS_IMETHOD Callback(nsISupports* aUnused) override {
    return mFinishCallback(mGeneration) ? NS_OK : NS_ERROR_FAILURE;
>>>>>>> upstream-releases
  }

 private:
  ~FinishReportingCallback() = default;

  uint32_t mGeneration;
  FinishCallback mFinishCallback;
};

NS_IMPL_ISUPPORTS(FinishReportingCallback, nsIFinishReportingCallback)

NS_IMETHODIMP MemoryReportRequestClient::Run() {
  nsCOMPtr<nsIMemoryReporterManager> mgr =
      do_GetService("@mozilla.org/memory-reporter-manager;1");

  // Run the reporters.  The callback will turn each measurement into a
  // MemoryReport.
  RefPtr<HandleReportCallback> handleReport =
<<<<<<< HEAD
      new HandleReportCallback(mGeneration, mProcessString);
||||||| merged common ancestors
    new HandleReportCallback(mGeneration, mProcessString);
=======
      new HandleReportCallback(mGeneration, mProcessString, mReportCallback);
>>>>>>> upstream-releases
  RefPtr<FinishReportingCallback> finishReporting =
<<<<<<< HEAD
      new FinishReportingCallback(mGeneration);
||||||| merged common ancestors
    new FinishReportingCallback(mGeneration);
=======
      new FinishReportingCallback(mGeneration, mFinishCallback);
>>>>>>> upstream-releases

  nsresult rv = mgr->GetReportsForThisProcessExtended(
      handleReport, nullptr, mAnonymize, FileDescriptorToFILE(mDMDFile, "wb"),
      finishReporting, nullptr);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                       "GetReportsForThisProcessExtended failed");
  return rv;
}

}  // namespace dom
}  // namespace mozilla
