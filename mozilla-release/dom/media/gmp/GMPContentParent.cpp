/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GMPContentParent.h"
#include "GMPLog.h"
#include "GMPParent.h"
#include "GMPServiceChild.h"
#include "GMPVideoDecoderParent.h"
#include "GMPVideoEncoderParent.h"
#include "ChromiumCDMParent.h"
#include "mozIGeckoMediaPluginService.h"
#include "mozilla/Logging.h"
#include "mozilla/Unused.h"
#include "base/task.h"

namespace mozilla {
namespace gmp {

static const char* GetBoolString(bool aBool) {
  return aBool ? "true" : "false";
}

GMPContentParent::GMPContentParent(GMPParent* aParent)
<<<<<<< HEAD
    : mParent(aParent), mPluginId(0) {
||||||| merged common ancestors
  : mParent(aParent)
  , mPluginId(0)
{
=======
    : mParent(aParent), mPluginId(0) {
  GMP_LOG("GMPContentParent::GMPContentParent(this=%p), aParent=%p", this,
          aParent);
>>>>>>> upstream-releases
  if (mParent) {
    SetDisplayName(mParent->GetDisplayName());
    SetPluginId(mParent->GetPluginId());
  }
}

<<<<<<< HEAD
GMPContentParent::~GMPContentParent() {}
||||||| merged common ancestors
GMPContentParent::~GMPContentParent()
{
}
=======
GMPContentParent::~GMPContentParent() {
  GMP_LOG(
      "GMPContentParent::~GMPContentParent(this=%p) mVideoDecoders.IsEmpty=%s, "
      "mVideoEncoders.IsEmpty=%s, mChromiumCDMs.IsEmpty=%s, "
      "mCloseBlockerCount=%" PRIu32,
      this, GetBoolString(mVideoDecoders.IsEmpty()),
      GetBoolString(mVideoEncoders.IsEmpty()),
      GetBoolString(mChromiumCDMs.IsEmpty()), mCloseBlockerCount);
}
>>>>>>> upstream-releases

class ReleaseGMPContentParent : public Runnable {
 public:
  explicit ReleaseGMPContentParent(GMPContentParent* aToRelease)
      : Runnable("gmp::ReleaseGMPContentParent"), mToRelease(aToRelease) {}

  NS_IMETHOD Run() override { return NS_OK; }

 private:
  RefPtr<GMPContentParent> mToRelease;
};

<<<<<<< HEAD
void GMPContentParent::ActorDestroy(ActorDestroyReason aWhy) {
  MOZ_ASSERT(mVideoDecoders.IsEmpty() && mVideoEncoders.IsEmpty() &&
||||||| merged common ancestors
void
GMPContentParent::ActorDestroy(ActorDestroyReason aWhy)
{
  MOZ_ASSERT(mVideoDecoders.IsEmpty() &&
             mVideoEncoders.IsEmpty() &&
=======
void GMPContentParent::ActorDestroy(ActorDestroyReason aWhy) {
  GMP_LOG("GMPContentParent::ActorDestroy(this=%p, aWhy=%d)", this,
          static_cast<int>(aWhy));
  MOZ_ASSERT(mVideoDecoders.IsEmpty() && mVideoEncoders.IsEmpty() &&
>>>>>>> upstream-releases
             mChromiumCDMs.IsEmpty());
  NS_DispatchToCurrentThread(new ReleaseGMPContentParent(this));
}

void GMPContentParent::CheckThread() {
  MOZ_ASSERT(GMPEventTarget()->IsOnCurrentThread());
}

<<<<<<< HEAD
void GMPContentParent::ChromiumCDMDestroyed(ChromiumCDMParent* aDecoder) {
||||||| merged common ancestors
void
GMPContentParent::ChromiumCDMDestroyed(ChromiumCDMParent* aDecoder)
{
=======
void GMPContentParent::ChromiumCDMDestroyed(ChromiumCDMParent* aCDM) {
  GMP_LOG("GMPContentParent::ChromiumCDMDestroyed(this=%p, aCDM=%p)", this,
          aCDM);
>>>>>>> upstream-releases
  MOZ_ASSERT(GMPEventTarget()->IsOnCurrentThread());

  MOZ_ALWAYS_TRUE(mChromiumCDMs.RemoveElement(aCDM));
  CloseIfUnused();
}

<<<<<<< HEAD
void GMPContentParent::VideoDecoderDestroyed(GMPVideoDecoderParent* aDecoder) {
||||||| merged common ancestors
void
GMPContentParent::VideoDecoderDestroyed(GMPVideoDecoderParent* aDecoder)
{
=======
void GMPContentParent::VideoDecoderDestroyed(GMPVideoDecoderParent* aDecoder) {
  GMP_LOG("GMPContentParent::VideoDecoderDestroyed(this=%p, aDecoder=%p)", this,
          aDecoder);
>>>>>>> upstream-releases
  MOZ_ASSERT(GMPEventTarget()->IsOnCurrentThread());

  // If the constructor fails, we'll get called before it's added
  Unused << NS_WARN_IF(!mVideoDecoders.RemoveElement(aDecoder));
  CloseIfUnused();
}

<<<<<<< HEAD
void GMPContentParent::VideoEncoderDestroyed(GMPVideoEncoderParent* aEncoder) {
||||||| merged common ancestors
void
GMPContentParent::VideoEncoderDestroyed(GMPVideoEncoderParent* aEncoder)
{
=======
void GMPContentParent::VideoEncoderDestroyed(GMPVideoEncoderParent* aEncoder) {
  GMP_LOG("GMPContentParent::VideoEncoderDestroyed(this=%p, aEncoder=%p)", this,
          aEncoder);
>>>>>>> upstream-releases
  MOZ_ASSERT(GMPEventTarget()->IsOnCurrentThread());

  // If the constructor fails, we'll get called before it's added
  Unused << NS_WARN_IF(!mVideoEncoders.RemoveElement(aEncoder));
  CloseIfUnused();
}

void GMPContentParent::AddCloseBlocker() {
  MOZ_ASSERT(GMPEventTarget()->IsOnCurrentThread());
  ++mCloseBlockerCount;
  GMP_LOG(
      "GMPContentParent::AddCloseBlocker(this=%p) mCloseBlockerCount=%" PRIu32,
      this, mCloseBlockerCount);
}

void GMPContentParent::RemoveCloseBlocker() {
  MOZ_ASSERT(GMPEventTarget()->IsOnCurrentThread());
  --mCloseBlockerCount;
  GMP_LOG(
      "GMPContentParent::RemoveCloseBlocker(this=%p) "
      "mCloseBlockerCount=%" PRIu32,
      this, mCloseBlockerCount);
  CloseIfUnused();
}

<<<<<<< HEAD
void GMPContentParent::CloseIfUnused() {
  if (mVideoDecoders.IsEmpty() && mVideoEncoders.IsEmpty() &&
      mChromiumCDMs.IsEmpty() && mCloseBlockerCount == 0) {
||||||| merged common ancestors
void
GMPContentParent::CloseIfUnused()
{
  if (mVideoDecoders.IsEmpty() &&
      mVideoEncoders.IsEmpty() &&
      mChromiumCDMs.IsEmpty() &&
      mCloseBlockerCount == 0) {
=======
void GMPContentParent::CloseIfUnused() {
  GMP_LOG(
      "GMPContentParent::CloseIfUnused(this=%p) mVideoDecoders.IsEmpty=%s, "
      "mVideoEncoders.IsEmpty=%s, mChromiumCDMs.IsEmpty=%s, "
      "mCloseBlockerCount=%" PRIu32,
      this, GetBoolString(mVideoDecoders.IsEmpty()),
      GetBoolString(mVideoEncoders.IsEmpty()),
      GetBoolString(mChromiumCDMs.IsEmpty()), mCloseBlockerCount);
  if (mVideoDecoders.IsEmpty() && mVideoEncoders.IsEmpty() &&
      mChromiumCDMs.IsEmpty() && mCloseBlockerCount == 0) {
>>>>>>> upstream-releases
    RefPtr<GMPContentParent> toClose;
    if (mParent) {
      toClose = mParent->ForgetGMPContentParent();
    } else {
      toClose = this;
      RefPtr<GeckoMediaPluginServiceChild> gmp(
          GeckoMediaPluginServiceChild::GetSingleton());
      gmp->RemoveGMPContentParent(toClose);
    }
    NS_DispatchToCurrentThread(NewRunnableMethod(
        "gmp::GMPContentParent::Close", toClose, &GMPContentParent::Close));
  }
}

nsCOMPtr<nsISerialEventTarget> GMPContentParent::GMPEventTarget() {
  if (!mGMPEventTarget) {
<<<<<<< HEAD
    nsCOMPtr<mozIGeckoMediaPluginService> mps =
        do_GetService("@mozilla.org/gecko-media-plugin-service;1");
||||||| merged common ancestors
    nsCOMPtr<mozIGeckoMediaPluginService> mps = do_GetService("@mozilla.org/gecko-media-plugin-service;1");
=======
    GMP_LOG("GMPContentParent::GMPEventTarget(this=%p)", this);
    nsCOMPtr<mozIGeckoMediaPluginService> mps =
        do_GetService("@mozilla.org/gecko-media-plugin-service;1");
>>>>>>> upstream-releases
    MOZ_ASSERT(mps);
    if (!mps) {
      return nullptr;
    }
    // Not really safe if we just grab to the mGMPEventTarget, as we don't know
    // what thread we're running on and other threads may be trying to
    // access this without locks!  However, debug only, and primary failure
    // mode outside of compiler-helped TSAN is a leak.  But better would be
    // to use swap() under a lock.
    nsCOMPtr<nsIThread> gmpThread;
    mps->GetThread(getter_AddRefs(gmpThread));
    MOZ_ASSERT(gmpThread);

    mGMPEventTarget = gmpThread->SerialEventTarget();
  }

  return mGMPEventTarget;
}

<<<<<<< HEAD
already_AddRefed<ChromiumCDMParent> GMPContentParent::GetChromiumCDM() {
||||||| merged common ancestors
already_AddRefed<ChromiumCDMParent>
GMPContentParent::GetChromiumCDM()
{
=======
already_AddRefed<ChromiumCDMParent> GMPContentParent::GetChromiumCDM() {
  GMP_LOG("GMPContentParent::GetChromiumCDM(this=%p)", this);
>>>>>>> upstream-releases
  PChromiumCDMParent* actor = SendPChromiumCDMConstructor();
  if (!actor) {
    return nullptr;
  }
  RefPtr<ChromiumCDMParent> parent = static_cast<ChromiumCDMParent*>(actor);

  // TODO: Remove parent from mChromiumCDMs in ChromiumCDMParent::Destroy().
  mChromiumCDMs.AppendElement(parent);

  return parent.forget();
}

<<<<<<< HEAD
nsresult GMPContentParent::GetGMPVideoDecoder(GMPVideoDecoderParent** aGMPVD,
                                              uint32_t aDecryptorId) {
||||||| merged common ancestors
nsresult
GMPContentParent::GetGMPVideoDecoder(GMPVideoDecoderParent** aGMPVD,
                                     uint32_t aDecryptorId)
{
=======
nsresult GMPContentParent::GetGMPVideoDecoder(GMPVideoDecoderParent** aGMPVD,
                                              uint32_t aDecryptorId) {
  GMP_LOG("GMPContentParent::GetGMPVideoDecoder(this=%p)", this);
>>>>>>> upstream-releases
  // returned with one anonymous AddRef that locks it until Destroy
  PGMPVideoDecoderParent* pvdp = SendPGMPVideoDecoderConstructor(aDecryptorId);
  if (!pvdp) {
    return NS_ERROR_FAILURE;
  }
  GMPVideoDecoderParent* vdp = static_cast<GMPVideoDecoderParent*>(pvdp);
  // This addref corresponds to the Proxy pointer the consumer is returned.
  // It's dropped by calling Close() on the interface.
  NS_ADDREF(vdp);
  *aGMPVD = vdp;
  mVideoDecoders.AppendElement(vdp);

  return NS_OK;
}

<<<<<<< HEAD
nsresult GMPContentParent::GetGMPVideoEncoder(GMPVideoEncoderParent** aGMPVE) {
||||||| merged common ancestors
nsresult
GMPContentParent::GetGMPVideoEncoder(GMPVideoEncoderParent** aGMPVE)
{
=======
nsresult GMPContentParent::GetGMPVideoEncoder(GMPVideoEncoderParent** aGMPVE) {
  GMP_LOG("GMPContentParent::GetGMPVideoEncoder(this=%p)", this);
>>>>>>> upstream-releases
  // returned with one anonymous AddRef that locks it until Destroy
  PGMPVideoEncoderParent* pvep = SendPGMPVideoEncoderConstructor();
  if (!pvep) {
    return NS_ERROR_FAILURE;
  }
  GMPVideoEncoderParent* vep = static_cast<GMPVideoEncoderParent*>(pvep);
  // This addref corresponds to the Proxy pointer the consumer is returned.
  // It's dropped by calling Close() on the interface.
  NS_ADDREF(vep);
  *aGMPVE = vep;
  mVideoEncoders.AppendElement(vep);

  return NS_OK;
}

<<<<<<< HEAD
PChromiumCDMParent* GMPContentParent::AllocPChromiumCDMParent() {
||||||| merged common ancestors
PChromiumCDMParent*
GMPContentParent::AllocPChromiumCDMParent()
{
=======
PChromiumCDMParent* GMPContentParent::AllocPChromiumCDMParent() {
  GMP_LOG("GMPContentParent::AllocPChromiumCDMParent(this=%p)", this);
>>>>>>> upstream-releases
  ChromiumCDMParent* parent = new ChromiumCDMParent(this, GetPluginId());
  NS_ADDREF(parent);
  return parent;
}

<<<<<<< HEAD
PGMPVideoDecoderParent* GMPContentParent::AllocPGMPVideoDecoderParent(
    const uint32_t& aDecryptorId) {
||||||| merged common ancestors
PGMPVideoDecoderParent*
GMPContentParent::AllocPGMPVideoDecoderParent(const uint32_t& aDecryptorId)
{
=======
PGMPVideoDecoderParent* GMPContentParent::AllocPGMPVideoDecoderParent(
    const uint32_t& aDecryptorId) {
  GMP_LOG("GMPContentParent::AllocPGMPVideoDecoderParent(this=%p)", this);
>>>>>>> upstream-releases
  GMPVideoDecoderParent* vdp = new GMPVideoDecoderParent(this);
  NS_ADDREF(vdp);
  return vdp;
}

<<<<<<< HEAD
bool GMPContentParent::DeallocPChromiumCDMParent(PChromiumCDMParent* aActor) {
||||||| merged common ancestors
bool
GMPContentParent::DeallocPChromiumCDMParent(PChromiumCDMParent* aActor)
{
=======
bool GMPContentParent::DeallocPChromiumCDMParent(PChromiumCDMParent* aActor) {
  GMP_LOG("GMPContentParent::DeallocPChromiumCDMParent(this=%p, aActor=%p)",
          this, aActor);
>>>>>>> upstream-releases
  ChromiumCDMParent* parent = static_cast<ChromiumCDMParent*>(aActor);
  NS_RELEASE(parent);
  return true;
}

<<<<<<< HEAD
bool GMPContentParent::DeallocPGMPVideoDecoderParent(
    PGMPVideoDecoderParent* aActor) {
||||||| merged common ancestors
bool
GMPContentParent::DeallocPGMPVideoDecoderParent(PGMPVideoDecoderParent* aActor)
{
=======
bool GMPContentParent::DeallocPGMPVideoDecoderParent(
    PGMPVideoDecoderParent* aActor) {
  GMP_LOG("GMPContentParent::DeallocPGMPVideoDecoderParent(this=%p, aActor=%p)",
          this, aActor);
>>>>>>> upstream-releases
  GMPVideoDecoderParent* vdp = static_cast<GMPVideoDecoderParent*>(aActor);
  NS_RELEASE(vdp);
  return true;
}

<<<<<<< HEAD
PGMPVideoEncoderParent* GMPContentParent::AllocPGMPVideoEncoderParent() {
||||||| merged common ancestors
PGMPVideoEncoderParent*
GMPContentParent::AllocPGMPVideoEncoderParent()
{
=======
PGMPVideoEncoderParent* GMPContentParent::AllocPGMPVideoEncoderParent() {
  GMP_LOG("GMPContentParent::AllocPGMPVideoEncoderParent(this=%p)", this);
>>>>>>> upstream-releases
  GMPVideoEncoderParent* vep = new GMPVideoEncoderParent(this);
  NS_ADDREF(vep);
  return vep;
}

<<<<<<< HEAD
bool GMPContentParent::DeallocPGMPVideoEncoderParent(
    PGMPVideoEncoderParent* aActor) {
||||||| merged common ancestors
bool
GMPContentParent::DeallocPGMPVideoEncoderParent(PGMPVideoEncoderParent* aActor)
{
=======
bool GMPContentParent::DeallocPGMPVideoEncoderParent(
    PGMPVideoEncoderParent* aActor) {
  GMP_LOG("GMPContentParent::DeallocPGMPVideoEncoderParent(this=%p, aActor=%p)",
          this, aActor);
>>>>>>> upstream-releases
  GMPVideoEncoderParent* vep = static_cast<GMPVideoEncoderParent*>(aActor);
  NS_RELEASE(vep);
  return true;
}

}  // namespace gmp
}  // namespace mozilla
