/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/ContentParent.h"
#include "mozilla/Hal.h"
#include "mozilla/HalWakeLock.h"
#include "mozilla/ClearOnShutdown.h"
#include "mozilla/ModuleUtils.h"
#include "mozilla/Preferences.h"
#include "nsIDOMWakeLockListener.h"
#include "nsIDOMWindow.h"
#include "nsIObserverService.h"
#include "PowerManagerService.h"
#include "WakeLock.h"

// For _exit().
#ifdef XP_WIN
#  include <process.h>
#else
#  include <unistd.h>
#endif

namespace mozilla {
namespace dom {
namespace power {

using namespace hal;

NS_IMPL_ISUPPORTS(PowerManagerService, nsIPowerManagerService)

/* static */
StaticRefPtr<PowerManagerService> PowerManagerService::sSingleton;

<<<<<<< HEAD
/* static */ already_AddRefed<PowerManagerService>
PowerManagerService::GetInstance() {
||||||| merged common ancestors
/* static */ already_AddRefed<PowerManagerService>
PowerManagerService::GetInstance()
{
=======
/* static */
already_AddRefed<PowerManagerService> PowerManagerService::GetInstance() {
>>>>>>> upstream-releases
  if (!sSingleton) {
    sSingleton = new PowerManagerService();
    sSingleton->Init();
    ClearOnShutdown(&sSingleton);
  }

  RefPtr<PowerManagerService> service = sSingleton.get();
  return service.forget();
}

void PowerManagerService::Init() { RegisterWakeLockObserver(this); }

PowerManagerService::~PowerManagerService() {
  UnregisterWakeLockObserver(this);
}

<<<<<<< HEAD
void PowerManagerService::ComputeWakeLockState(
    const WakeLockInformation &aWakeLockInfo, nsAString &aState) {
||||||| merged common ancestors
void
PowerManagerService::ComputeWakeLockState(const WakeLockInformation& aWakeLockInfo,
                                          nsAString &aState)
{
=======
void PowerManagerService::ComputeWakeLockState(
    const WakeLockInformation& aWakeLockInfo, nsAString& aState) {
>>>>>>> upstream-releases
  WakeLockState state = hal::ComputeWakeLockState(aWakeLockInfo.numLocks(),
                                                  aWakeLockInfo.numHidden());
  switch (state) {
    case WAKE_LOCK_STATE_UNLOCKED:
      aState.AssignLiteral("unlocked");
      break;
    case WAKE_LOCK_STATE_HIDDEN:
      aState.AssignLiteral("locked-background");
      break;
    case WAKE_LOCK_STATE_VISIBLE:
      aState.AssignLiteral("locked-foreground");
      break;
  }
}

<<<<<<< HEAD
void PowerManagerService::Notify(const WakeLockInformation &aWakeLockInfo) {
||||||| merged common ancestors
void
PowerManagerService::Notify(const WakeLockInformation& aWakeLockInfo)
{
=======
void PowerManagerService::Notify(const WakeLockInformation& aWakeLockInfo) {
>>>>>>> upstream-releases
  nsAutoString state;
  ComputeWakeLockState(aWakeLockInfo, state);

  /**
   * Copy the listeners list before we walk through the callbacks
   * because the callbacks may install new listeners. We expect no
   * more than one listener per window, so it shouldn't be too long.
   */
  AutoTArray<nsCOMPtr<nsIDOMMozWakeLockListener>, 2> listeners(
      mWakeLockListeners);

  for (uint32_t i = 0; i < listeners.Length(); ++i) {
    listeners[i]->Callback(aWakeLockInfo.topic(), state);
  }
}

NS_IMETHODIMP
<<<<<<< HEAD
PowerManagerService::AddWakeLockListener(nsIDOMMozWakeLockListener *aListener) {
  if (mWakeLockListeners.Contains(aListener)) return NS_OK;
||||||| merged common ancestors
PowerManagerService::AddWakeLockListener(nsIDOMMozWakeLockListener *aListener)
{
  if (mWakeLockListeners.Contains(aListener))
    return NS_OK;
=======
PowerManagerService::AddWakeLockListener(nsIDOMMozWakeLockListener* aListener) {
  if (mWakeLockListeners.Contains(aListener)) return NS_OK;
>>>>>>> upstream-releases

  mWakeLockListeners.AppendElement(aListener);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
PowerManagerService::RemoveWakeLockListener(
    nsIDOMMozWakeLockListener *aListener) {
||||||| merged common ancestors
PowerManagerService::RemoveWakeLockListener(nsIDOMMozWakeLockListener *aListener)
{
=======
PowerManagerService::RemoveWakeLockListener(
    nsIDOMMozWakeLockListener* aListener) {
>>>>>>> upstream-releases
  mWakeLockListeners.RemoveElement(aListener);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
PowerManagerService::GetWakeLockState(const nsAString &aTopic,
                                      nsAString &aState) {
||||||| merged common ancestors
PowerManagerService::GetWakeLockState(const nsAString &aTopic, nsAString &aState)
{
=======
PowerManagerService::GetWakeLockState(const nsAString& aTopic,
                                      nsAString& aState) {
>>>>>>> upstream-releases
  WakeLockInformation info;
  GetWakeLockInfo(aTopic, &info);

  ComputeWakeLockState(info, aState);

  return NS_OK;
}

<<<<<<< HEAD
already_AddRefed<WakeLock> PowerManagerService::NewWakeLock(
    const nsAString &aTopic, nsPIDOMWindowInner *aWindow,
    mozilla::ErrorResult &aRv) {
||||||| merged common ancestors
already_AddRefed<WakeLock>
PowerManagerService::NewWakeLock(const nsAString& aTopic,
                                 nsPIDOMWindowInner* aWindow,
                                 mozilla::ErrorResult& aRv)
{
=======
already_AddRefed<WakeLock> PowerManagerService::NewWakeLock(
    const nsAString& aTopic, nsPIDOMWindowInner* aWindow,
    mozilla::ErrorResult& aRv) {
>>>>>>> upstream-releases
  RefPtr<WakeLock> wakelock = new WakeLock();
  aRv = wakelock->Init(aTopic, aWindow);
  if (aRv.Failed()) {
    return nullptr;
  }

  return wakelock.forget();
}

NS_IMETHODIMP
<<<<<<< HEAD
PowerManagerService::NewWakeLock(const nsAString &aTopic,
                                 mozIDOMWindow *aWindow,
                                 nsIWakeLock **aWakeLock) {
||||||| merged common ancestors
PowerManagerService::NewWakeLock(const nsAString &aTopic,
                                 mozIDOMWindow *aWindow,
                                 nsISupports **aWakeLock)
{
=======
PowerManagerService::NewWakeLock(const nsAString& aTopic,
                                 mozIDOMWindow* aWindow,
                                 nsIWakeLock** aWakeLock) {
>>>>>>> upstream-releases
  mozilla::ErrorResult rv;
  RefPtr<WakeLock> wakelock =
      NewWakeLock(aTopic, nsPIDOMWindowInner::From(aWindow), rv);
  if (rv.Failed()) {
    return rv.StealNSResult();
  }

  wakelock.forget(aWakeLock);
  return NS_OK;
}

<<<<<<< HEAD
already_AddRefed<WakeLock> PowerManagerService::NewWakeLockOnBehalfOfProcess(
    const nsAString &aTopic, ContentParent *aContentParent) {
||||||| merged common ancestors
already_AddRefed<WakeLock>
PowerManagerService::NewWakeLockOnBehalfOfProcess(const nsAString& aTopic,
                                                  ContentParent* aContentParent)
{
=======
already_AddRefed<WakeLock> PowerManagerService::NewWakeLockOnBehalfOfProcess(
    const nsAString& aTopic, ContentParent* aContentParent) {
>>>>>>> upstream-releases
  RefPtr<WakeLock> wakelock = new WakeLock();
  nsresult rv = wakelock->Init(aTopic, aContentParent);
  NS_ENSURE_SUCCESS(rv, nullptr);
  return wakelock.forget();
}

<<<<<<< HEAD
}  // namespace power
}  // namespace dom
}  // namespace mozilla
||||||| merged common ancestors
} // namespace power
} // namespace dom
} // namespace mozilla
=======
}  // namespace power
}  // namespace dom
}  // namespace mozilla

NS_DEFINE_NAMED_CID(NS_POWERMANAGERSERVICE_CID);

NS_GENERIC_FACTORY_SINGLETON_CONSTRUCTOR(
    nsIPowerManagerService,
    mozilla::dom::power::PowerManagerService::GetInstance)

static const mozilla::Module::CIDEntry kPowerManagerCIDs[] = {
    // clang-format off
  { &kNS_POWERMANAGERSERVICE_CID, false, nullptr, nsIPowerManagerServiceConstructor, mozilla::Module::ALLOW_IN_GPU_RDD_AND_SOCKET_PROCESS },
  { nullptr }
    // clang-format on
};

static const mozilla::Module::ContractIDEntry kPowerManagerContracts[] = {
    // clang-format off
  { POWERMANAGERSERVICE_CONTRACTID, &kNS_POWERMANAGERSERVICE_CID, mozilla::Module::ALLOW_IN_GPU_RDD_AND_SOCKET_PROCESS },
  { nullptr }
    // clang-format on
};

// We mark the power module as being available in the GPU process because the
// appshell depends on the power manager service.
extern const mozilla::Module kPowerManagerModule = {
    mozilla::Module::kVersion,
    kPowerManagerCIDs,
    kPowerManagerContracts,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    mozilla::Module::ALLOW_IN_GPU_RDD_AND_SOCKET_PROCESS};
>>>>>>> upstream-releases
