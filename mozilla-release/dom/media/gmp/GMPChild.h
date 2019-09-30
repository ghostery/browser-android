/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GMPChild_h_
#define GMPChild_h_

#include "mozilla/gmp/PGMPChild.h"
#include "mozilla/Pair.h"
#include "GMPTimerChild.h"
#include "GMPStorageChild.h"
#include "GMPLoader.h"
#include "gmp-entrypoints.h"
#include "prlink.h"

namespace mozilla {
namespace gmp {

class GMPContentChild;

<<<<<<< HEAD
class GMPChild : public PGMPChild {
 public:
||||||| merged common ancestors
class GMPChild : public PGMPChild
{
public:
=======
class GMPChild : public PGMPChild {
  friend class PGMPChild;

 public:
>>>>>>> upstream-releases
  GMPChild();
  virtual ~GMPChild();

  bool Init(const nsAString& aPluginPath, base::ProcessId aParentPid,
            MessageLoop* aIOLoop, IPC::Channel* aChannel);
  MessageLoop* GMPMessageLoop();

  // Main thread only.
  GMPTimerChild* GetGMPTimers();
  GMPStorageChild* GetGMPStorage();

#if defined(XP_MACOSX) && defined(MOZ_SANDBOX)
  bool SetMacSandboxInfo(bool aAllowWindowServer);
#endif

 private:
  friend class GMPContentChild;

  bool GetUTF8LibPath(nsACString& aOutLibPath);

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvProvideStorageId(
      const nsCString& aStorageId) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult RecvProvideStorageId(const nsCString& aStorageId) override;
=======
  mozilla::ipc::IPCResult RecvProvideStorageId(const nsCString& aStorageId);
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult AnswerStartPlugin(const nsString& aAdapter);
  mozilla::ipc::IPCResult RecvPreloadLibs(const nsCString& aLibs);

  PGMPTimerChild* AllocPGMPTimerChild();
  bool DeallocPGMPTimerChild(PGMPTimerChild* aActor);

  PGMPStorageChild* AllocPGMPStorageChild();
  bool DeallocPGMPStorageChild(PGMPStorageChild* aActor);

  void GMPContentChildActorDestroy(GMPContentChild* aGMPContentChild);

  mozilla::ipc::IPCResult RecvCrashPluginNow();
  mozilla::ipc::IPCResult RecvCloseActive();

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvInitGMPContentChild(
      Endpoint<PGMPContentChild>&& aEndpoint) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult RecvInitGMPContentChild(Endpoint<PGMPContentChild>&& aEndpoint) override;
=======
  mozilla::ipc::IPCResult RecvInitGMPContentChild(
      Endpoint<PGMPContentChild>&& aEndpoint);
>>>>>>> upstream-releases

  void ActorDestroy(ActorDestroyReason aWhy) override;
  void ProcessingError(Result aCode, const char* aReason) override;

  GMPErr GetAPI(const char* aAPIName, void* aHostAPI, void** aPluginAPI,
                uint32_t aDecryptorId = 0);

  nsTArray<Pair<nsCString, nsCString>> MakeCDMHostVerificationPaths();

  nsTArray<UniquePtr<GMPContentChild>> mGMPContentChildren;

  RefPtr<GMPTimerChild> mTimerChild;
  RefPtr<GMPStorageChild> mStorage;

  MessageLoop* mGMPMessageLoop;
  nsString mPluginPath;
  nsCString mStorageId;
  UniquePtr<GMPLoader> mGMPLoader;
};

}  // namespace gmp
}  // namespace mozilla

#endif  // GMPChild_h_
