/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "LayerAnimationInfo.h"

#include "nsCSSProps.h"          // For nsCSSProps::PropHasFlags
#include "nsCSSPropertyIDSet.h"  // For nsCSSPropertyIDSet::CompositorAnimatable

namespace mozilla {

<<<<<<< HEAD
/* static */ const Array<DisplayItemType,
                         nsCSSPropertyIDSet::CompositorAnimatableCount()>
    LayerAnimationInfo::sDisplayItemTypes = {
        DisplayItemType::TYPE_BACKGROUND_COLOR,
        DisplayItemType::TYPE_OPACITY,
        DisplayItemType::TYPE_TRANSFORM,
};
||||||| merged common ancestors
/* static */ const LayerAnimationInfo::Record LayerAnimationInfo::sRecords[] =
  { { eCSSProperty_transform,
      DisplayItemType::TYPE_TRANSFORM,
      nsChangeHint_UpdateTransformLayer },
    { eCSSProperty_opacity,
      DisplayItemType::TYPE_OPACITY,
      nsChangeHint_UpdateOpacityLayer } };
=======
/* static */ const Array<
    DisplayItemType, nsCSSPropertyIDSet::CompositorAnimatableDisplayItemCount()>
    LayerAnimationInfo::sDisplayItemTypes = {
        DisplayItemType::TYPE_BACKGROUND_COLOR,
        DisplayItemType::TYPE_OPACITY,
        DisplayItemType::TYPE_TRANSFORM,
};
>>>>>>> upstream-releases

<<<<<<< HEAD
/* static */ DisplayItemType LayerAnimationInfo::GetDisplayItemTypeForProperty(
    nsCSSPropertyID aProperty) {
  switch (aProperty) {
    case eCSSProperty_background_color:
      return DisplayItemType::TYPE_BACKGROUND_COLOR;
    case eCSSProperty_opacity:
      return DisplayItemType::TYPE_OPACITY;
    case eCSSProperty_transform:
      return DisplayItemType::TYPE_TRANSFORM;
    default:
      break;
||||||| merged common ancestors
#ifdef DEBUG
/* static */ void
LayerAnimationInfo::Initialize()
{
  for (const Record& record : sRecords) {
    MOZ_ASSERT(nsCSSProps::PropHasFlags(record.mProperty,
                                        CSSPropFlags::CanAnimateOnCompositor),
               "CSS property with entry in LayerAnimation::sRecords does not "
               "have the CSSPropFlags::CanAnimateOnCompositor flag");
  }

  // Check that every property with the flag for animating on the
  // compositor has an entry in LayerAnimationInfo::sRecords.
  for (nsCSSPropertyID prop = nsCSSPropertyID(0);
       prop < eCSSProperty_COUNT;
       prop = nsCSSPropertyID(prop + 1)) {
    if (nsCSSProps::PropHasFlags(prop,
                                 CSSPropFlags::CanAnimateOnCompositor)) {
      bool found = false;
      for (const Record& record : sRecords) {
        if (record.mProperty == prop) {
          found = true;
          break;
        }
      }
      MOZ_ASSERT(found,
                 "CSS property with the CSSPropFlags::CanAnimateOnCompositor "
                 "flag does not have an entry in LayerAnimationInfo::sRecords");
    }
=======
/* static */
DisplayItemType LayerAnimationInfo::GetDisplayItemTypeForProperty(
    nsCSSPropertyID aProperty) {
  switch (aProperty) {
    case eCSSProperty_background_color:
      return DisplayItemType::TYPE_BACKGROUND_COLOR;
    case eCSSProperty_opacity:
      return DisplayItemType::TYPE_OPACITY;
    case eCSSProperty_transform:
    case eCSSProperty_translate:
    case eCSSProperty_scale:
    case eCSSProperty_rotate:
      return DisplayItemType::TYPE_TRANSFORM;
    default:
      break;
>>>>>>> upstream-releases
  }
  return DisplayItemType::TYPE_ZERO;
}

}  // namespace mozilla
