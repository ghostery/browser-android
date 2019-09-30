/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "AnimationPerformanceWarning.h"

#include "nsContentUtils.h"

namespace mozilla {

<<<<<<< HEAD
template <uint32_t N>
nsresult AnimationPerformanceWarning::ToLocalizedStringWithIntParams(
    const char* aKey, nsAString& aLocalizedString) const {
  nsAutoString strings[N];
  const char16_t* charParams[N];
||||||| merged common ancestors
template<uint32_t N> nsresult
AnimationPerformanceWarning::ToLocalizedStringWithIntParams(
  const char* aKey, nsAString& aLocalizedString) const
{
  nsAutoString strings[N];
  const char16_t* charParams[N];
=======
template <uint32_t N>
nsresult AnimationPerformanceWarning::ToLocalizedStringWithIntParams(
    const char* aKey, nsAString& aLocalizedString) const {
  AutoTArray<nsString, N> strings;
>>>>>>> upstream-releases

  MOZ_DIAGNOSTIC_ASSERT(mParams->Length() == N);
  for (size_t i = 0, n = mParams->Length(); i < n; i++) {
    strings.AppendElement()->AppendInt((*mParams)[i]);
  }

  return nsContentUtils::FormatLocalizedString(
      nsContentUtils::eLAYOUT_PROPERTIES, aKey, strings, aLocalizedString);
}

bool AnimationPerformanceWarning::ToLocalizedString(
    nsAString& aLocalizedString) const {
  const char* key = nullptr;

  switch (mType) {
    case Type::ContentTooLarge:
      MOZ_ASSERT(mParams && mParams->Length() == 6,
                 "Parameter's length should be 6 for ContentTooLarge2");

<<<<<<< HEAD
      return NS_SUCCEEDED(ToLocalizedStringWithIntParams<7>(
||||||| merged common ancestors
      return NS_SUCCEEDED(
        ToLocalizedStringWithIntParams<7>(
=======
      return NS_SUCCEEDED(ToLocalizedStringWithIntParams<6>(
>>>>>>> upstream-releases
          "CompositorAnimationWarningContentTooLarge2", aLocalizedString));
    case Type::ContentTooLargeArea:
      MOZ_ASSERT(mParams && mParams->Length() == 2,
                 "Parameter's length should be 2 for ContentTooLargeArea");

<<<<<<< HEAD
      return NS_SUCCEEDED(ToLocalizedStringWithIntParams<3>(
||||||| merged common ancestors
      return NS_SUCCEEDED(
        ToLocalizedStringWithIntParams<3>(
=======
      return NS_SUCCEEDED(ToLocalizedStringWithIntParams<2>(
>>>>>>> upstream-releases
          "CompositorAnimationWarningContentTooLargeArea", aLocalizedString));
    case Type::TransformBackfaceVisibilityHidden:
      key = "CompositorAnimationWarningTransformBackfaceVisibilityHidden";
      break;
    case Type::TransformPreserve3D:
      key = "CompositorAnimationWarningTransformPreserve3D";
      break;
    case Type::TransformSVG:
      key = "CompositorAnimationWarningTransformSVG";
      break;
    case Type::TransformWithGeometricProperties:
      key = "CompositorAnimationWarningTransformWithGeometricProperties";
      break;
    case Type::TransformWithSyncGeometricAnimations:
      key = "CompositorAnimationWarningTransformWithSyncGeometricAnimations";
      break;
    case Type::TransformFrameInactive:
      key = "CompositorAnimationWarningTransformFrameInactive";
      break;
    case Type::TransformIsBlockedByImportantRules:
      key = "CompositorAnimationWarningTransformIsBlockedByImportantRules";
      break;
    case Type::OpacityFrameInactive:
      key = "CompositorAnimationWarningOpacityFrameInactive";
      break;
    case Type::HasRenderingObserver:
      key = "CompositorAnimationWarningHasRenderingObserver";
      break;
    case Type::None:
      MOZ_ASSERT_UNREACHABLE("Uninitialized type shouldn't be used");
      return false;
  }

  nsresult rv = nsContentUtils::GetLocalizedString(
      nsContentUtils::eLAYOUT_PROPERTIES, key, aLocalizedString);
  return NS_SUCCEEDED(rv);
}

}  // namespace mozilla
