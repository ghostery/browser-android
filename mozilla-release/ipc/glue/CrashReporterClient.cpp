/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "CrashReporterClient.h"
#include "CrashReporterMetadataShmem.h"
#include "nsISupportsImpl.h"

namespace mozilla {
namespace ipc {

StaticMutex CrashReporterClient::sLock;
StaticRefPtr<CrashReporterClient> CrashReporterClient::sClientSingleton;

CrashReporterClient::CrashReporterClient(const Shmem& aShmem)
    : mMetadata(new CrashReporterMetadataShmem(aShmem)) {
  MOZ_COUNT_CTOR(CrashReporterClient);
}

CrashReporterClient::~CrashReporterClient() {
  MOZ_COUNT_DTOR(CrashReporterClient);
}

<<<<<<< HEAD
void CrashReporterClient::AnnotateCrashReport(CrashReporter::Annotation aKey,
                                              const nsCString& aData) {
||||||| merged common ancestors
void
CrashReporterClient::AnnotateCrashReport(CrashReporter::Annotation aKey,
                                         const nsCString& aData)
{
=======
void CrashReporterClient::AnnotateCrashReport(CrashReporter::Annotation aKey,
                                              const nsACString& aData) {
>>>>>>> upstream-releases
  StaticMutexAutoLock lock(sLock);
  mMetadata->AnnotateCrashReport(aKey, aData);
}

<<<<<<< HEAD
void CrashReporterClient::AppendAppNotes(const nsCString& aData) {
||||||| merged common ancestors
void
CrashReporterClient::AppendAppNotes(const nsCString& aData)
{
=======
void CrashReporterClient::AppendAppNotes(const nsACString& aData) {
>>>>>>> upstream-releases
  StaticMutexAutoLock lock(sLock);
  mMetadata->AppendAppNotes(aData);
}

<<<<<<< HEAD
/* static */ void CrashReporterClient::InitSingletonWithShmem(
    const Shmem& aShmem) {
||||||| merged common ancestors
/* static */ void
CrashReporterClient::InitSingletonWithShmem(const Shmem& aShmem)
{
=======
/* static */
void CrashReporterClient::InitSingletonWithShmem(const Shmem& aShmem) {
>>>>>>> upstream-releases
  {
    StaticMutexAutoLock lock(sLock);

    MOZ_ASSERT(!sClientSingleton);
    sClientSingleton = new CrashReporterClient(aShmem);
  }

  CrashReporter::NotifyCrashReporterClientCreated();
}

<<<<<<< HEAD
/* static */ void CrashReporterClient::DestroySingleton() {
||||||| merged common ancestors
/* static */ void
CrashReporterClient::DestroySingleton()
{
=======
/* static */
void CrashReporterClient::DestroySingleton() {
>>>>>>> upstream-releases
  StaticMutexAutoLock lock(sLock);
  sClientSingleton = nullptr;
}

<<<<<<< HEAD
/* static */ RefPtr<CrashReporterClient> CrashReporterClient::GetSingleton() {
||||||| merged common ancestors
/* static */ RefPtr<CrashReporterClient>
CrashReporterClient::GetSingleton()
{
=======
/* static */
RefPtr<CrashReporterClient> CrashReporterClient::GetSingleton() {
>>>>>>> upstream-releases
  StaticMutexAutoLock lock(sLock);
  return sClientSingleton;
}

}  // namespace ipc
}  // namespace mozilla
