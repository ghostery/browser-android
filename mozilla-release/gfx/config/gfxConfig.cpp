/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "gfxConfig.h"
#include "mozilla/UniquePtr.h"
#include "mozilla/Unused.h"
#include "mozilla/gfx/GPUParent.h"
#include "mozilla/gfx/GraphicsMessages.h"
#include "plstr.h"

namespace mozilla {
namespace gfx {

static UniquePtr<gfxConfig> sConfig;

/* static */ FeatureState& gfxConfig::GetFeature(Feature aFeature) {
  return sConfig->GetState(aFeature);
}

<<<<<<< HEAD
/* static */ bool gfxConfig::IsEnabled(Feature aFeature) {
||||||| merged common ancestors
/* static */ bool
gfxConfig::IsEnabled(Feature aFeature)
{
=======
/* static */
bool gfxConfig::IsEnabled(Feature aFeature) {
>>>>>>> upstream-releases
  const FeatureState& state = sConfig->GetState(aFeature);
  return state.IsEnabled();
}

<<<<<<< HEAD
/* static */ bool gfxConfig::IsDisabledByDefault(Feature aFeature) {
||||||| merged common ancestors
/* static */ bool
gfxConfig::IsDisabledByDefault(Feature aFeature)
{
=======
/* static */
bool gfxConfig::IsDisabledByDefault(Feature aFeature) {
>>>>>>> upstream-releases
  const FeatureState& state = sConfig->GetState(aFeature);
  return state.DisabledByDefault();
}

<<<<<<< HEAD
/* static */ bool gfxConfig::IsForcedOnByUser(Feature aFeature) {
||||||| merged common ancestors
/* static */ bool
gfxConfig::IsForcedOnByUser(Feature aFeature)
{
=======
/* static */
bool gfxConfig::IsForcedOnByUser(Feature aFeature) {
>>>>>>> upstream-releases
  const FeatureState& state = sConfig->GetState(aFeature);
  return state.IsForcedOnByUser();
}

<<<<<<< HEAD
/* static */ FeatureStatus gfxConfig::GetValue(Feature aFeature) {
||||||| merged common ancestors
/* static */ FeatureStatus
gfxConfig::GetValue(Feature aFeature)
{
=======
/* static */
FeatureStatus gfxConfig::GetValue(Feature aFeature) {
>>>>>>> upstream-releases
  const FeatureState& state = sConfig->GetState(aFeature);
  return state.GetValue();
}

<<<<<<< HEAD
/* static */ bool gfxConfig::SetDefault(Feature aFeature, bool aEnable,
                                        FeatureStatus aDisableStatus,
                                        const char* aDisableMessage) {
||||||| merged common ancestors
/* static */ bool
gfxConfig::SetDefault(Feature aFeature,
                      bool aEnable,
                      FeatureStatus aDisableStatus,
                      const char* aDisableMessage)
{
=======
/* static */
bool gfxConfig::SetDefault(Feature aFeature, bool aEnable,
                           FeatureStatus aDisableStatus,
                           const char* aDisableMessage) {
>>>>>>> upstream-releases
  FeatureState& state = sConfig->GetState(aFeature);
  return state.SetDefault(aEnable, aDisableStatus, aDisableMessage);
}

<<<<<<< HEAD
/* static */ void gfxConfig::DisableByDefault(Feature aFeature,
                                              FeatureStatus aDisableStatus,
                                              const char* aDisableMessage,
                                              const nsACString& aFailureId) {
||||||| merged common ancestors
/* static */ void
gfxConfig::DisableByDefault(Feature aFeature,
                            FeatureStatus aDisableStatus,
                            const char* aDisableMessage,
                            const nsACString& aFailureId)
{
=======
/* static */
void gfxConfig::DisableByDefault(Feature aFeature, FeatureStatus aDisableStatus,
                                 const char* aDisableMessage,
                                 const nsACString& aFailureId) {
>>>>>>> upstream-releases
  FeatureState& state = sConfig->GetState(aFeature);
  state.DisableByDefault(aDisableStatus, aDisableMessage, aFailureId);
}

<<<<<<< HEAD
/* static */ void gfxConfig::EnableByDefault(Feature aFeature) {
||||||| merged common ancestors
/* static */ void
gfxConfig::EnableByDefault(Feature aFeature)
{
=======
/* static */
void gfxConfig::EnableByDefault(Feature aFeature) {
>>>>>>> upstream-releases
  FeatureState& state = sConfig->GetState(aFeature);
  state.EnableByDefault();
}

<<<<<<< HEAD
/* static */ void gfxConfig::SetDefaultFromPref(Feature aFeature,
                                                const char* aPrefName,
                                                bool aIsEnablePref,
                                                bool aDefaultValue) {
||||||| merged common ancestors
/* static */ void
gfxConfig::SetDefaultFromPref(Feature aFeature,
                              const char* aPrefName,
                              bool aIsEnablePref,
                              bool aDefaultValue)
{
=======
/* static */
void gfxConfig::SetDefaultFromPref(Feature aFeature, const char* aPrefName,
                                   bool aIsEnablePref, bool aDefaultValue) {
>>>>>>> upstream-releases
  FeatureState& state = sConfig->GetState(aFeature);
  return state.SetDefaultFromPref(aPrefName, aIsEnablePref, aDefaultValue);
}

<<<<<<< HEAD
/* static */ bool gfxConfig::InitOrUpdate(Feature aFeature, bool aEnable,
                                          FeatureStatus aDisableStatus,
                                          const char* aDisableMessage) {
||||||| merged common ancestors
/* static */ bool
gfxConfig::InitOrUpdate(Feature aFeature,
                        bool aEnable,
                        FeatureStatus aDisableStatus,
                        const char* aDisableMessage)
{
=======
/* static */
bool gfxConfig::InitOrUpdate(Feature aFeature, bool aEnable,
                             FeatureStatus aDisableStatus,
                             const char* aDisableMessage) {
>>>>>>> upstream-releases
  FeatureState& state = sConfig->GetState(aFeature);
  return state.InitOrUpdate(aEnable, aDisableStatus, aDisableMessage);
}

<<<<<<< HEAD
/* static */ void gfxConfig::SetFailed(Feature aFeature, FeatureStatus aStatus,
                                       const char* aMessage,
                                       const nsACString& aFailureId) {
||||||| merged common ancestors
/* static */ void
gfxConfig::SetFailed(Feature aFeature, FeatureStatus aStatus, const char* aMessage,
                     const nsACString& aFailureId)
{
=======
/* static */
void gfxConfig::SetFailed(Feature aFeature, FeatureStatus aStatus,
                          const char* aMessage, const nsACString& aFailureId) {
>>>>>>> upstream-releases
  FeatureState& state = sConfig->GetState(aFeature);
  state.SetFailed(aStatus, aMessage, aFailureId);
}

<<<<<<< HEAD
/* static */ void gfxConfig::Disable(Feature aFeature, FeatureStatus aStatus,
                                     const char* aMessage,
                                     const nsACString& aFailureId) {
||||||| merged common ancestors
/* static */ void
gfxConfig::Disable(Feature aFeature, FeatureStatus aStatus, const char* aMessage,
                   const nsACString& aFailureId)
{
=======
/* static */
void gfxConfig::Disable(Feature aFeature, FeatureStatus aStatus,
                        const char* aMessage, const nsACString& aFailureId) {
>>>>>>> upstream-releases
  FeatureState& state = sConfig->GetState(aFeature);
  state.Disable(aStatus, aMessage, aFailureId);
}

<<<<<<< HEAD
/* static */ void gfxConfig::UserEnable(Feature aFeature,
                                        const char* aMessage) {
||||||| merged common ancestors
/* static */ void
gfxConfig::UserEnable(Feature aFeature, const char* aMessage)
{
=======
/* static */
void gfxConfig::UserEnable(Feature aFeature, const char* aMessage) {
>>>>>>> upstream-releases
  FeatureState& state = sConfig->GetState(aFeature);
  state.UserEnable(aMessage);
}

<<<<<<< HEAD
/* static */ void gfxConfig::UserForceEnable(Feature aFeature,
                                             const char* aMessage) {
||||||| merged common ancestors
/* static */ void
gfxConfig::UserForceEnable(Feature aFeature, const char* aMessage)
{
=======
/* static */
void gfxConfig::UserForceEnable(Feature aFeature, const char* aMessage) {
>>>>>>> upstream-releases
  FeatureState& state = sConfig->GetState(aFeature);
  state.UserForceEnable(aMessage);
}

<<<<<<< HEAD
/* static */ void gfxConfig::UserDisable(Feature aFeature, const char* aMessage,
                                         const nsACString& aFailureId) {
||||||| merged common ancestors
/* static */ void
gfxConfig::UserDisable(Feature aFeature, const char* aMessage, const nsACString& aFailureId)
{
=======
/* static */
void gfxConfig::UserDisable(Feature aFeature, const char* aMessage,
                            const nsACString& aFailureId) {
>>>>>>> upstream-releases
  FeatureState& state = sConfig->GetState(aFeature);
  state.UserDisable(aMessage, aFailureId);
}

<<<<<<< HEAD
/* static */ void gfxConfig::Reenable(Feature aFeature, Fallback aFallback) {
||||||| merged common ancestors
/* static */ void
gfxConfig::Reenable(Feature aFeature, Fallback aFallback)
{
=======
/* static */
void gfxConfig::Reenable(Feature aFeature, Fallback aFallback) {
>>>>>>> upstream-releases
  FeatureState& state = sConfig->GetState(aFeature);
  MOZ_ASSERT(IsFeatureStatusFailure(state.GetValue()));

  const char* message = state.GetRuntimeMessage();
  EnableFallback(aFallback, message);
  state.SetRuntime(FeatureStatus::Available, nullptr);
}

<<<<<<< HEAD
/* static */ void gfxConfig::Reset(Feature aFeature) {
||||||| merged common ancestors
/* static */ void
gfxConfig::Reset(Feature aFeature)
{
=======
/* static */
void gfxConfig::Reset(Feature aFeature) {
>>>>>>> upstream-releases
  FeatureState& state = sConfig->GetState(aFeature);
  state.Reset();
}

<<<<<<< HEAD
/* static */ void gfxConfig::Inherit(Feature aFeature, FeatureStatus aStatus) {
||||||| merged common ancestors
/* static */ void
gfxConfig::Inherit(Feature aFeature, FeatureStatus aStatus)
{
=======
/* static */
void gfxConfig::Inherit(Feature aFeature, FeatureStatus aStatus) {
>>>>>>> upstream-releases
  FeatureState& state = sConfig->GetState(aFeature);

  state.Reset();

  switch (aStatus) {
    case FeatureStatus::Unused:
      break;
    case FeatureStatus::Available:
      gfxConfig::EnableByDefault(aFeature);
      break;
    case FeatureStatus::ForceEnabled:
      gfxConfig::EnableByDefault(aFeature);
      gfxConfig::UserForceEnable(aFeature, "Inherited from parent process");
      break;
    default:
      gfxConfig::SetDefault(aFeature, false, aStatus,
                            "Disabled in parent process");
      break;
  }
}

<<<<<<< HEAD
/* static */ bool gfxConfig::UseFallback(Fallback aFallback) {
||||||| merged common ancestors
/* static */ bool
gfxConfig::UseFallback(Fallback aFallback)
{
=======
/* static */
bool gfxConfig::UseFallback(Fallback aFallback) {
>>>>>>> upstream-releases
  return sConfig->UseFallbackImpl(aFallback);
}

<<<<<<< HEAD
/* static */ void gfxConfig::EnableFallback(Fallback aFallback,
                                            const char* aMessage) {
||||||| merged common ancestors
/* static */ void
gfxConfig::EnableFallback(Fallback aFallback, const char* aMessage)
{
=======
/* static */
void gfxConfig::EnableFallback(Fallback aFallback, const char* aMessage) {
>>>>>>> upstream-releases
  if (!NS_IsMainThread()) {
    nsCString message(aMessage);
    NS_DispatchToMainThread(
        NS_NewRunnableFunction("gfxConfig::EnableFallback", [=]() -> void {
          gfxConfig::EnableFallback(aFallback, message.get());
        }));
    return;
  }

  if (XRE_IsGPUProcess()) {
    nsCString message(aMessage);
    Unused << GPUParent::GetSingleton()->SendUsedFallback(aFallback, message);
    return;
  }

  sConfig->EnableFallbackImpl(aFallback, aMessage);
}

bool gfxConfig::UseFallbackImpl(Fallback aFallback) const {
  return !!(mFallbackBits & (uint64_t(1) << uint64_t(aFallback)));
}

void gfxConfig::EnableFallbackImpl(Fallback aFallback, const char* aMessage) {
  if (!UseFallbackImpl(aFallback)) {
    MOZ_ASSERT(mNumFallbackLogEntries < kNumFallbacks);

    FallbackLogEntry& entry = mFallbackLog[mNumFallbackLogEntries];
    mNumFallbackLogEntries++;

    entry.mFallback = aFallback;
    PL_strncpyz(entry.mMessage, aMessage, sizeof(entry.mMessage));
  }
  mFallbackBits |= (uint64_t(1) << uint64_t(aFallback));
}

struct FeatureInfo {
  const char* name;
  const char* description;
};
static const FeatureInfo sFeatureInfo[] = {
#define FOR_EACH_FEATURE(name, type, desc) {#name, desc},
    GFX_FEATURE_MAP(FOR_EACH_FEATURE)
#undef FOR_EACH_FEATURE
        {nullptr, nullptr}};

<<<<<<< HEAD
/* static */ void gfxConfig::ForEachFeature(
    const FeatureIterCallback& aCallback) {
||||||| merged common ancestors
/* static */ void
gfxConfig::ForEachFeature(const FeatureIterCallback& aCallback)
{
=======
/* static */
void gfxConfig::ForEachFeature(const FeatureIterCallback& aCallback) {
>>>>>>> upstream-releases
  for (size_t i = 0; i < kNumFeatures; i++) {
    FeatureState& state = GetFeature(static_cast<Feature>(i));
    if (!state.IsInitialized()) {
      continue;
    }

    aCallback(sFeatureInfo[i].name, sFeatureInfo[i].description, state);
  }
}

static const char* sFallbackNames[] = {
#define FOR_EACH_FALLBACK(name) #name,
    GFX_FALLBACK_MAP(FOR_EACH_FALLBACK)
#undef FOR_EACH_FALLBACK
        nullptr};

<<<<<<< HEAD
/* static  */ void gfxConfig::ForEachFallback(
    const FallbackIterCallback& aCallback) {
||||||| merged common ancestors
/* static  */ void
gfxConfig::ForEachFallback(const FallbackIterCallback& aCallback)
{
=======
/* static  */
void gfxConfig::ForEachFallback(const FallbackIterCallback& aCallback) {
>>>>>>> upstream-releases
  sConfig->ForEachFallbackImpl(aCallback);
}

void gfxConfig::ForEachFallbackImpl(const FallbackIterCallback& aCallback) {
  for (size_t i = 0; i < mNumFallbackLogEntries; i++) {
    const FallbackLogEntry& entry = mFallbackLog[i];
    aCallback(sFallbackNames[size_t(entry.mFallback)], entry.mMessage);
  }
}

/* static */ const nsCString& gfxConfig::GetFailureId(Feature aFeature) {
  const FeatureState& state = sConfig->GetState(aFeature);
  return state.GetFailureId();
}

<<<<<<< HEAD
/* static */ void gfxConfig::ImportChange(Feature aFeature,
                                          const FeatureChange& aChange) {
  if (aChange.type() == FeatureChange::Tnull_t) {
||||||| merged common ancestors
/* static */ void
gfxConfig::ImportChange(Feature aFeature, const FeatureChange& aChange)
{
  if (aChange.type() == FeatureChange::Tnull_t) {
=======
/* static */
void gfxConfig::ImportChange(Feature aFeature,
                             const Maybe<FeatureFailure>& aChange) {
  if (aChange.isNothing()) {
>>>>>>> upstream-releases
    return;
  }

<<<<<<< HEAD
  const FeatureFailure& failure = aChange.get_FeatureFailure();
  gfxConfig::SetFailed(aFeature, failure.status(), failure.message().get(),
                       failure.failureId());
||||||| merged common ancestors
  const FeatureFailure& failure = aChange.get_FeatureFailure();
  gfxConfig::SetFailed(
    aFeature,
    failure.status(),
    failure.message().get(),
    failure.failureId());
=======
  const FeatureFailure& failure = aChange.ref();
  gfxConfig::SetFailed(aFeature, failure.status(), failure.message().get(),
                       failure.failureId());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ void gfxConfig::Init() {
  sConfig = mozilla::MakeUnique<gfxConfig>();
}
||||||| merged common ancestors
/* static */ void
gfxConfig::Init()
{
  sConfig = mozilla::MakeUnique<gfxConfig>();
}
=======
/* static */
void gfxConfig::Init() { sConfig = mozilla::MakeUnique<gfxConfig>(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
/* static */ void gfxConfig::Shutdown() { sConfig = nullptr; }
||||||| merged common ancestors
/* static */ void
gfxConfig::Shutdown()
{
  sConfig = nullptr;
}
=======
/* static */
void gfxConfig::Shutdown() { sConfig = nullptr; }
>>>>>>> upstream-releases

}  // namespace gfx
}  // namespace mozilla
