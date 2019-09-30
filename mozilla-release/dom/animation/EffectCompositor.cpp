/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "EffectCompositor.h"

#include <bitset>
#include <initializer_list>

#include "mozilla/dom/Animation.h"
#include "mozilla/dom/Element.h"
#include "mozilla/dom/KeyframeEffect.h"
#include "mozilla/AnimationComparator.h"
#include "mozilla/AnimationPerformanceWarning.h"
#include "mozilla/AnimationTarget.h"
#include "mozilla/AnimationUtils.h"
#include "mozilla/AutoRestore.h"
#include "mozilla/ComputedStyleInlines.h"
#include "mozilla/EffectSet.h"
<<<<<<< HEAD
||||||| merged common ancestors
#include "mozilla/LayerAnimationInfo.h"
=======
#include "mozilla/LayerAnimationInfo.h"
#include "mozilla/PresShell.h"
#include "mozilla/PresShellInlines.h"
>>>>>>> upstream-releases
#include "mozilla/RestyleManager.h"
#include "mozilla/ServoBindings.h"  // Servo_GetProperties_Overriding_Animation
#include "mozilla/ServoStyleSet.h"
#include "mozilla/StyleAnimationValue.h"
#include "mozilla/TypeTraits.h"  // For std::forward<>
#include "nsContentUtils.h"
#include "nsCSSPseudoElements.h"
#include "nsCSSPropertyIDSet.h"
#include "nsCSSProps.h"
#include "nsDisplayItemTypes.h"
#include "nsAtom.h"
#include "nsLayoutUtils.h"
#include "nsTArray.h"
#include "PendingAnimationTracker.h"

using mozilla::dom::Animation;
using mozilla::dom::Element;
using mozilla::dom::KeyframeEffect;

namespace mozilla {

NS_IMPL_CYCLE_COLLECTION_CLASS(EffectCompositor)

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN(EffectCompositor)
  for (auto& elementSet : tmp->mElementsToRestyle) {
    elementSet.Clear();
  }
NS_IMPL_CYCLE_COLLECTION_UNLINK_END

NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN(EffectCompositor)
  for (auto& elementSet : tmp->mElementsToRestyle) {
    for (auto iter = elementSet.Iter(); !iter.Done(); iter.Next()) {
      CycleCollectionNoteChild(cb, iter.Key().mElement,
                               "EffectCompositor::mElementsToRestyle[]",
                               cb.Flags());
    }
  }
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

NS_IMPL_CYCLE_COLLECTION_ROOT_NATIVE(EffectCompositor, AddRef)
NS_IMPL_CYCLE_COLLECTION_UNROOT_NATIVE(EffectCompositor, Release)

<<<<<<< HEAD
/* static */ bool EffectCompositor::AllowCompositorAnimationsOnFrame(
    const nsIFrame* aFrame, const EffectSet& aEffects,
    AnimationPerformanceWarning::Type& aWarning /* out */) {
  if (aFrame->RefusedAsyncAnimation()) {
    return false;
||||||| merged common ancestors
namespace {
enum class MatchForCompositor {
  // This animation matches and should run on the compositor if possible.
  Yes,
  // This (not currently playing) animation matches and can be run on the
  // compositor if there are other animations for this property that return
  // 'Yes'.
  IfNeeded,
  // This animation does not match or can't be run on the compositor.
  No,
  // This animation does not match or can't be run on the compositor and,
  // furthermore, its presence means we should not run any animations for this
  // property on the compositor.
  NoAndBlockThisProperty
};
}

static MatchForCompositor
IsMatchForCompositor(const KeyframeEffect& aEffect,
                     nsCSSPropertyID aProperty,
                     const nsIFrame* aFrame)
{
  const Animation* animation = aEffect.GetAnimation();
  MOZ_ASSERT(animation);

  if (!animation->IsRelevant()) {
    return MatchForCompositor::No;
  }

  AnimationPerformanceWarning::Type warningType;
  if (animation->ShouldBeSynchronizedWithMainThread(aProperty, aFrame,
                                                    warningType)) {
    EffectCompositor::SetPerformanceWarning(
      aFrame, aProperty,
      AnimationPerformanceWarning(warningType));
    // For a given |aFrame|, we don't want some animations of |aProperty| to
    // run on the compositor and others to run on the main thread, so if any
    // need to be synchronized with the main thread, run them all there.
    return MatchForCompositor::NoAndBlockThisProperty;
=======
/* static */
bool EffectCompositor::AllowCompositorAnimationsOnFrame(
    const nsIFrame* aFrame,
    AnimationPerformanceWarning::Type& aWarning /* out */) {
  if (aFrame->RefusedAsyncAnimation()) {
    return false;
>>>>>>> upstream-releases
  }

  if (!nsLayoutUtils::AreAsyncAnimationsEnabled()) {
    if (nsLayoutUtils::IsAnimationLoggingEnabled()) {
      nsCString message;
      message.AppendLiteral(
          "Performance warning: Async animations are "
          "disabled");
      AnimationUtils::LogAsyncAnimationFailure(message);
    }
    return false;
  }

  // Disable async animations if we have a rendering observer that
  // depends on our content (svg masking, -moz-element etc) so that
  // it gets updated correctly.
  nsIContent* content = aFrame->GetContent();
  while (content) {
    if (content->HasRenderingObservers()) {
      aWarning = AnimationPerformanceWarning::Type::HasRenderingObserver;
      return false;
    }
    content = content->GetParent();
  }

  return true;
}

// Helper function to factor out the common logic from
// GetAnimationsForCompositor and HasAnimationsForCompositor.
//
// Takes an optional array to fill with eligible animations.
//
// Returns true if there are eligible animations, false otherwise.
<<<<<<< HEAD
bool FindAnimationsForCompositor(
    const nsIFrame* aFrame, nsCSSPropertyID aProperty,
    nsTArray<RefPtr<dom::Animation>>* aMatches /*out*/) {
||||||| merged common ancestors
bool
FindAnimationsForCompositor(const nsIFrame* aFrame,
                            nsCSSPropertyID aProperty,
                            nsTArray<RefPtr<dom::Animation>>* aMatches /*out*/)
{
=======
bool FindAnimationsForCompositor(
    const nsIFrame* aFrame, const nsCSSPropertyIDSet& aPropertySet,
    nsTArray<RefPtr<dom::Animation>>* aMatches /*out*/) {
  MOZ_ASSERT(
      aPropertySet.IsSubsetOf(LayerAnimationInfo::GetCSSPropertiesFor(
          DisplayItemType::TYPE_TRANSFORM)) ||
          aPropertySet.IsSubsetOf(LayerAnimationInfo::GetCSSPropertiesFor(
              DisplayItemType::TYPE_OPACITY)) ||
          aPropertySet.IsSubsetOf(LayerAnimationInfo::GetCSSPropertiesFor(
              DisplayItemType::TYPE_BACKGROUND_COLOR)),
      "Should be the subset of transform-like properties, or opacity, "
      "or background color");

>>>>>>> upstream-releases
  MOZ_ASSERT(!aMatches || aMatches->IsEmpty(),
             "Matches array, if provided, should be empty");

  EffectSet* effects = EffectSet::GetEffectSetForFrame(aFrame, aPropertySet);
  if (!effects || effects->IsEmpty()) {
    return false;
  }

  // First check for newly-started transform animations that should be
  // synchronized with geometric animations. We need to do this before any
  // other early returns (the one above is ok) since we can only check this
  // state when the animation is newly-started.
  if (aPropertySet.Intersects(LayerAnimationInfo::GetCSSPropertiesFor(
          DisplayItemType::TYPE_TRANSFORM))) {
    PendingAnimationTracker* tracker =
        aFrame->PresContext()->Document()->GetPendingAnimationTracker();
    if (tracker) {
      tracker->MarkAnimationsThatMightNeedSynchronization();
    }
  }

<<<<<<< HEAD
  // If the property will be added to the animations level of the cascade but
  // there is an !important rule for that property in the cascade then the
  // animation will not be applied since the !important rule overrides it.
  if (effects->PropertiesWithImportantRules().HasProperty(aProperty) &&
      effects->PropertiesForAnimationsLevel().HasProperty(aProperty)) {
    return false;
  }

  AnimationPerformanceWarning::Type warning =
      AnimationPerformanceWarning::Type::None;
  if (!EffectCompositor::AllowCompositorAnimationsOnFrame(aFrame, *effects,
                                                          warning)) {
    if (warning != AnimationPerformanceWarning::Type::None) {
      EffectCompositor::SetPerformanceWarning(
          aFrame, aProperty, AnimationPerformanceWarning(warning));
    }
||||||| merged common ancestors
  // If the property will be added to the animations level of the cascade but
  // there is an !important rule for that property in the cascade then the
  // animation will not be applied since the !important rule overrides it.
  if (effects->PropertiesWithImportantRules().HasProperty(aProperty) &&
      effects->PropertiesForAnimationsLevel().HasProperty(aProperty)) {
    return false;
  }

  if (aFrame->RefusedAsyncAnimation()) {
=======
  AnimationPerformanceWarning::Type warning =
      AnimationPerformanceWarning::Type::None;
  if (!EffectCompositor::AllowCompositorAnimationsOnFrame(aFrame, warning)) {
    if (warning != AnimationPerformanceWarning::Type::None) {
      EffectCompositor::SetPerformanceWarning(
          aFrame, aPropertySet, AnimationPerformanceWarning(warning));
    }
>>>>>>> upstream-releases
    return false;
  }

  // The animation cascade will almost always be up-to-date by this point
  // but there are some cases such as when we are restoring the refresh driver
  // from test control after seeking where it might not be the case.
  //
  // Those cases are probably not important but just to be safe, let's make
  // sure the cascade is up to date since if it *is* up to date, this is
  // basically a no-op.
  Maybe<NonOwningAnimationTarget> pseudoElement =
<<<<<<< HEAD
      EffectCompositor::GetAnimationElementAndPseudoForFrame(aFrame);
||||||| merged common ancestors
    EffectCompositor::GetAnimationElementAndPseudoForFrame(aFrame);
=======
      EffectCompositor::GetAnimationElementAndPseudoForFrame(
          nsLayoutUtils::GetStyleFrame(aFrame));
>>>>>>> upstream-releases
  MOZ_ASSERT(pseudoElement,
             "We have a valid element for the frame, if we don't we should "
             "have bailed out at above the call to EffectSet::GetEffectSet");
  EffectCompositor::MaybeUpdateCascadeResults(pseudoElement->mElement,
                                              pseudoElement->mPseudoType);

  bool foundRunningAnimations = false;
  for (KeyframeEffect* effect : *effects) {
<<<<<<< HEAD
    AnimationPerformanceWarning::Type effectWarning =
        AnimationPerformanceWarning::Type::None;
    KeyframeEffect::MatchForCompositor matchResult =
        effect->IsMatchForCompositor(aProperty, aFrame, *effects,
                                     effectWarning);
    if (effectWarning != AnimationPerformanceWarning::Type::None) {
      EffectCompositor::SetPerformanceWarning(
          aFrame, aProperty, AnimationPerformanceWarning(effectWarning));
    }
||||||| merged common ancestors
    MatchForCompositor matchResult =
      IsMatchForCompositor(*effect, aProperty, aFrame);
=======
    AnimationPerformanceWarning::Type effectWarning =
        AnimationPerformanceWarning::Type::None;
    KeyframeEffect::MatchForCompositor matchResult =
        effect->IsMatchForCompositor(aPropertySet, aFrame, *effects,
                                     effectWarning);
    if (effectWarning != AnimationPerformanceWarning::Type::None) {
      EffectCompositor::SetPerformanceWarning(
          aFrame, aPropertySet, AnimationPerformanceWarning(effectWarning));
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (matchResult ==
        KeyframeEffect::MatchForCompositor::NoAndBlockThisProperty) {
      // For a given |aFrame|, we don't want some animations of |aProperty| to
      // run on the compositor and others to run on the main thread, so if any
      // need to be synchronized with the main thread, run them all there.
||||||| merged common ancestors
    if (matchResult == MatchForCompositor::NoAndBlockThisProperty) {
      // For a given |aFrame|, we don't want some animations of |aProperty| to
      // run on the compositor and others to run on the main thread, so if any
      // need to be synchronized with the main thread, run them all there.
=======
    if (matchResult ==
        KeyframeEffect::MatchForCompositor::NoAndBlockThisProperty) {
      // For a given |aFrame|, we don't want some animations of |aPropertySet|
      // to run on the compositor and others to run on the main thread, so if
      // any need to be synchronized with the main thread, run them all there.
>>>>>>> upstream-releases
      if (aMatches) {
        aMatches->Clear();
      }
      return false;
    }

    if (matchResult == KeyframeEffect::MatchForCompositor::No) {
      continue;
    }

    if (aMatches) {
      aMatches->AppendElement(effect->GetAnimation());
    }

    if (matchResult == KeyframeEffect::MatchForCompositor::Yes) {
      foundRunningAnimations = true;
    }
  }

  // If all animations we added were not currently playing animations, don't
  // send them to the compositor.
  if (aMatches && !foundRunningAnimations) {
    aMatches->Clear();
  }

  MOZ_ASSERT(!foundRunningAnimations || !aMatches || !aMatches->IsEmpty(),
             "If return value is true, matches array should be non-empty");

  if (aMatches && foundRunningAnimations) {
    aMatches->Sort(AnimationPtrComparator<RefPtr<dom::Animation>>());
  }

  return foundRunningAnimations;
}

<<<<<<< HEAD
void EffectCompositor::RequestRestyle(dom::Element* aElement,
                                      CSSPseudoElementType aPseudoType,
                                      RestyleType aRestyleType,
                                      CascadeLevel aCascadeLevel) {
||||||| merged common ancestors
void
EffectCompositor::RequestRestyle(dom::Element* aElement,
                                 CSSPseudoElementType aPseudoType,
                                 RestyleType aRestyleType,
                                 CascadeLevel aCascadeLevel)
{
=======
void EffectCompositor::RequestRestyle(dom::Element* aElement,
                                      PseudoStyleType aPseudoType,
                                      RestyleType aRestyleType,
                                      CascadeLevel aCascadeLevel) {
>>>>>>> upstream-releases
  if (!mPresContext) {
    // Pres context will be null after the effect compositor is disconnected.
    return;
  }

  // Ignore animations on orphaned elements and elements in documents without
  // a pres shell (e.g. XMLHttpRequest responseXML documents).
  if (!nsContentUtils::GetPresShellForContent(aElement)) {
    return;
  }

  auto& elementsToRestyle = mElementsToRestyle[aCascadeLevel];
  PseudoElementHashEntry::KeyType key = {aElement, aPseudoType};

  if (aRestyleType == RestyleType::Throttled) {
    elementsToRestyle.LookupForAdd(key).OrInsert([]() { return false; });
    mPresContext->PresShell()->SetNeedThrottledAnimationFlush();
  } else {
    bool skipRestyle;
    // Update hashtable first in case PostRestyleForAnimation mutates it.
    // (It shouldn't, but just to be sure.)
    if (auto p = elementsToRestyle.LookupForAdd(key)) {
      skipRestyle = p.Data();
      p.Data() = true;
    } else {
      skipRestyle = false;
      p.OrInsert([]() { return true; });
    }

    if (!skipRestyle) {
      PostRestyleForAnimation(aElement, aPseudoType, aCascadeLevel);
    }
  }

  if (aRestyleType == RestyleType::Layer) {
    mPresContext->RestyleManager()->IncrementAnimationGeneration();
    EffectSet* effectSet = EffectSet::GetEffectSet(aElement, aPseudoType);
    if (effectSet) {
      effectSet->UpdateAnimationGeneration(mPresContext);
    }
  }
}

<<<<<<< HEAD
void EffectCompositor::PostRestyleForAnimation(dom::Element* aElement,
                                               CSSPseudoElementType aPseudoType,
                                               CascadeLevel aCascadeLevel) {
||||||| merged common ancestors
void
EffectCompositor::PostRestyleForAnimation(dom::Element* aElement,
                                          CSSPseudoElementType aPseudoType,
                                          CascadeLevel aCascadeLevel)
{
=======
void EffectCompositor::PostRestyleForAnimation(dom::Element* aElement,
                                               PseudoStyleType aPseudoType,
                                               CascadeLevel aCascadeLevel) {
>>>>>>> upstream-releases
  if (!mPresContext) {
    return;
  }

  dom::Element* element = GetElementToRestyle(aElement, aPseudoType);
  if (!element) {
    return;
  }

<<<<<<< HEAD
  nsRestyleHint hint = aCascadeLevel == CascadeLevel::Transitions
                           ? eRestyle_CSSTransitions
                           : eRestyle_CSSAnimations;
||||||| merged common ancestors
  nsRestyleHint hint = aCascadeLevel == CascadeLevel::Transitions ?
                                        eRestyle_CSSTransitions :
                                        eRestyle_CSSAnimations;
=======
  RestyleHint hint = aCascadeLevel == CascadeLevel::Transitions
                         ? StyleRestyleHint_RESTYLE_CSS_TRANSITIONS
                         : StyleRestyleHint_RESTYLE_CSS_ANIMATIONS;
>>>>>>> upstream-releases

  MOZ_ASSERT(NS_IsMainThread(),
             "Restyle request during restyling should be requested only on "
             "the main-thread. e.g. after the parallel traversal");
  if (ServoStyleSet::IsInServoTraversal() || mIsInPreTraverse) {
    MOZ_ASSERT(hint == StyleRestyleHint_RESTYLE_CSS_ANIMATIONS ||
               hint == StyleRestyleHint_RESTYLE_CSS_TRANSITIONS);

    // We can't call Servo_NoteExplicitHints here since AtomicRefCell does not
    // allow us mutate ElementData of the |aElement| in SequentialTask.
    // Instead we call Servo_NoteExplicitHints for the element in PreTraverse()
    // which will be called right before the second traversal that we do for
    // updating CSS animations.
    // In that case PreTraverse() will return true so that we know to do the
    // second traversal so we don't need to post any restyle requests to the
    // PresShell.
    return;
  }

  MOZ_ASSERT(!mPresContext->RestyleManager()->IsInStyleRefresh());

  mPresContext->PresShell()->RestyleForAnimation(element, hint);
}

void EffectCompositor::PostRestyleForThrottledAnimations() {
  for (size_t i = 0; i < kCascadeLevelCount; i++) {
    CascadeLevel cascadeLevel = CascadeLevel(i);
    auto& elementSet = mElementsToRestyle[cascadeLevel];

    for (auto iter = elementSet.Iter(); !iter.Done(); iter.Next()) {
      bool& postedRestyle = iter.Data();
      if (postedRestyle) {
        continue;
      }

      PostRestyleForAnimation(iter.Key().mElement, iter.Key().mPseudoType,
                              cascadeLevel);
      postedRestyle = true;
    }
  }
}

void EffectCompositor::ClearRestyleRequestsFor(Element* aElement) {
  MOZ_ASSERT(aElement);

  auto& elementsToRestyle = mElementsToRestyle[CascadeLevel::Animations];

<<<<<<< HEAD
  CSSPseudoElementType pseudoType = aElement->GetPseudoElementType();
  if (pseudoType == CSSPseudoElementType::NotPseudo) {
    PseudoElementHashEntry::KeyType notPseudoKey = {
        aElement, CSSPseudoElementType::NotPseudo};
    PseudoElementHashEntry::KeyType beforePseudoKey = {
        aElement, CSSPseudoElementType::before};
    PseudoElementHashEntry::KeyType afterPseudoKey = {
        aElement, CSSPseudoElementType::after};
||||||| merged common ancestors
  CSSPseudoElementType pseudoType = aElement->GetPseudoElementType();
  if (pseudoType == CSSPseudoElementType::NotPseudo) {
    PseudoElementHashEntry::KeyType notPseudoKey =
      { aElement, CSSPseudoElementType::NotPseudo };
    PseudoElementHashEntry::KeyType beforePseudoKey =
      { aElement, CSSPseudoElementType::before };
    PseudoElementHashEntry::KeyType afterPseudoKey =
      { aElement, CSSPseudoElementType::after };
=======
  PseudoStyleType pseudoType = aElement->GetPseudoElementType();
  if (pseudoType == PseudoStyleType::NotPseudo) {
    PseudoElementHashEntry::KeyType notPseudoKey = {aElement,
                                                    PseudoStyleType::NotPseudo};
    PseudoElementHashEntry::KeyType beforePseudoKey = {aElement,
                                                       PseudoStyleType::before};
    PseudoElementHashEntry::KeyType afterPseudoKey = {aElement,
                                                      PseudoStyleType::after};
    PseudoElementHashEntry::KeyType markerPseudoKey = {aElement,
                                                       PseudoStyleType::marker};
>>>>>>> upstream-releases

    elementsToRestyle.Remove(notPseudoKey);
    elementsToRestyle.Remove(beforePseudoKey);
    elementsToRestyle.Remove(afterPseudoKey);
    elementsToRestyle.Remove(markerPseudoKey);
  } else if (pseudoType == PseudoStyleType::before ||
             pseudoType == PseudoStyleType::after ||
             pseudoType == PseudoStyleType::marker) {
    Element* parentElement = aElement->GetParentElement();
    MOZ_ASSERT(parentElement);
    PseudoElementHashEntry::KeyType key = {parentElement, pseudoType};
    elementsToRestyle.Remove(key);
  }
}

<<<<<<< HEAD
void EffectCompositor::UpdateEffectProperties(
    const ComputedStyle* aStyle, Element* aElement,
    CSSPseudoElementType aPseudoType) {
||||||| merged common ancestors
void
EffectCompositor::UpdateEffectProperties(const ComputedStyle* aStyle,
                                         Element* aElement,
                                         CSSPseudoElementType aPseudoType)
{
=======
void EffectCompositor::UpdateEffectProperties(const ComputedStyle* aStyle,
                                              Element* aElement,
                                              PseudoStyleType aPseudoType) {
>>>>>>> upstream-releases
  EffectSet* effectSet = EffectSet::GetEffectSet(aElement, aPseudoType);
  if (!effectSet) {
    return;
  }

  // Style context (Gecko) or computed values (Stylo) change might cause CSS
  // cascade level, e.g removing !important, so we should update the cascading
  // result.
  effectSet->MarkCascadeNeedsUpdate();

  for (KeyframeEffect* effect : *effectSet) {
    effect->UpdateProperties(aStyle);
  }
}

namespace {
class EffectCompositeOrderComparator {
 public:
  bool Equals(const KeyframeEffect* a, const KeyframeEffect* b) const {
    return a == b;
  }

  bool LessThan(const KeyframeEffect* a, const KeyframeEffect* b) const {
    MOZ_ASSERT(a->GetAnimation() && b->GetAnimation());
    MOZ_ASSERT(
        Equals(a, b) ||
        a->GetAnimation()->HasLowerCompositeOrderThan(*b->GetAnimation()) !=
<<<<<<< HEAD
            b->GetAnimation()->HasLowerCompositeOrderThan(*a->GetAnimation()));
    return a->GetAnimation()->HasLowerCompositeOrderThan(*b->GetAnimation());
  }
};
}  // namespace
||||||| merged common ancestors
          b->GetAnimation()->HasLowerCompositeOrderThan(*a->GetAnimation()));
      return a->GetAnimation()->HasLowerCompositeOrderThan(*b->GetAnimation());
    }
  };
}
=======
            b->GetAnimation()->HasLowerCompositeOrderThan(*a->GetAnimation()));
    return a->GetAnimation()->HasLowerCompositeOrderThan(*b->GetAnimation());
  }
};
}  // namespace

static void ComposeSortedEffects(
    const nsTArray<KeyframeEffect*>& aSortedEffects,
    const EffectSet* aEffectSet, EffectCompositor::CascadeLevel aCascadeLevel,
    RawServoAnimationValueMap* aAnimationValues) {
  // If multiple animations affect the same property, animations with higher
  // composite order (priority) override or add to animations with lower
  // priority.
  nsCSSPropertyIDSet propertiesToSkip;
  if (aEffectSet) {
    propertiesToSkip =
        aCascadeLevel == EffectCompositor::CascadeLevel::Animations
            ? aEffectSet->PropertiesForAnimationsLevel().Inverse()
            : aEffectSet->PropertiesForAnimationsLevel();
  }

  for (KeyframeEffect* effect : aSortedEffects) {
    effect->GetAnimation()->ComposeStyle(*aAnimationValues, propertiesToSkip);
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool EffectCompositor::GetServoAnimationRule(
    const dom::Element* aElement, CSSPseudoElementType aPseudoType,
    CascadeLevel aCascadeLevel,
    RawServoAnimationValueMapBorrowedMut aAnimationValues) {
||||||| merged common ancestors
bool
EffectCompositor::GetServoAnimationRule(
  const dom::Element* aElement,
  CSSPseudoElementType aPseudoType,
  CascadeLevel aCascadeLevel,
  RawServoAnimationValueMapBorrowedMut aAnimationValues)
{
=======
bool EffectCompositor::GetServoAnimationRule(
    const dom::Element* aElement, PseudoStyleType aPseudoType,
    CascadeLevel aCascadeLevel, RawServoAnimationValueMap* aAnimationValues) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aAnimationValues);
  MOZ_ASSERT(mPresContext && mPresContext->IsDynamic(),
             "Should not be in print preview");
  // Gecko_GetAnimationRule should have already checked this
  MOZ_ASSERT(nsContentUtils::GetPresShellForContent(aElement),
             "Should not be trying to run animations on elements in documents"
             " without a pres shell (e.g. XMLHttpRequest documents)");

  EffectSet* effectSet = EffectSet::GetEffectSet(aElement, aPseudoType);
  if (!effectSet) {
    return false;
  }

  // Get a list of effects sorted by composite order.
  nsTArray<KeyframeEffect*> sortedEffectList(effectSet->Count());
  for (KeyframeEffect* effect : *effectSet) {
    sortedEffectList.AppendElement(effect);
  }
  sortedEffectList.Sort(EffectCompositeOrderComparator());

<<<<<<< HEAD
  // If multiple animations affect the same property, animations with higher
  // composite order (priority) override or add or animations with lower
  // priority.
  const nsCSSPropertyIDSet propertiesToSkip =
      aCascadeLevel == CascadeLevel::Animations
          ? effectSet->PropertiesForAnimationsLevel().Inverse()
          : effectSet->PropertiesForAnimationsLevel();
  for (KeyframeEffect* effect : sortedEffectList) {
    effect->GetAnimation()->ComposeStyle(*aAnimationValues, propertiesToSkip);
  }
||||||| merged common ancestors
  // If multiple animations affect the same property, animations with higher
  // composite order (priority) override or add or animations with lower
  // priority.
  const nsCSSPropertyIDSet propertiesToSkip =
    aCascadeLevel == CascadeLevel::Animations
      ? effectSet->PropertiesForAnimationsLevel().Inverse()
      : effectSet->PropertiesForAnimationsLevel();
  for (KeyframeEffect* effect : sortedEffectList) {
    effect->GetAnimation()->ComposeStyle(*aAnimationValues, propertiesToSkip);
  }
=======
  ComposeSortedEffects(sortedEffectList, effectSet, aCascadeLevel,
                       aAnimationValues);
>>>>>>> upstream-releases

  MOZ_ASSERT(effectSet == EffectSet::GetEffectSet(aElement, aPseudoType),
             "EffectSet should not change while composing style");

  return true;
}

<<<<<<< HEAD
/* static */ dom::Element* EffectCompositor::GetElementToRestyle(
    dom::Element* aElement, CSSPseudoElementType aPseudoType) {
  if (aPseudoType == CSSPseudoElementType::NotPseudo) {
||||||| merged common ancestors
/* static */ dom::Element*
EffectCompositor::GetElementToRestyle(dom::Element* aElement,
                                      CSSPseudoElementType aPseudoType)
{
  if (aPseudoType == CSSPseudoElementType::NotPseudo) {
=======
bool EffectCompositor::ComposeServoAnimationRuleForEffect(
    KeyframeEffect& aEffect, CascadeLevel aCascadeLevel,
    RawServoAnimationValueMap* aAnimationValues) {
  MOZ_ASSERT(aAnimationValues);
  MOZ_ASSERT(mPresContext && mPresContext->IsDynamic(),
             "Should not be in print preview");

  Maybe<NonOwningAnimationTarget> target = aEffect.GetTarget();
  if (!target) {
    return false;
  }

  // Don't try to compose animations for elements in documents without a pres
  // shell (e.g. XMLHttpRequest documents).
  if (!nsContentUtils::GetPresShellForContent(target->mElement)) {
    return false;
  }

  // GetServoAnimationRule is called as part of the regular style resolution
  // where the cascade results are updated in the pre-traversal as needed.
  // This function, however, is only called when committing styles so we
  // need to ensure the cascade results are up-to-date manually.
  EffectCompositor::MaybeUpdateCascadeResults(target->mElement,
                                              target->mPseudoType);

  EffectSet* effectSet =
      EffectSet::GetEffectSet(target->mElement, target->mPseudoType);

  // Get a list of effects sorted by composite order up to and including
  // |aEffect|, even if it is not in the EffectSet.
  auto comparator = EffectCompositeOrderComparator();
  nsTArray<KeyframeEffect*> sortedEffectList(effectSet ? effectSet->Count() + 1
                                                       : 1);
  if (effectSet) {
    for (KeyframeEffect* effect : *effectSet) {
      if (comparator.LessThan(effect, &aEffect)) {
        sortedEffectList.AppendElement(effect);
      }
    }
    sortedEffectList.Sort(comparator);
  }
  sortedEffectList.AppendElement(&aEffect);

  ComposeSortedEffects(sortedEffectList, effectSet, aCascadeLevel,
                       aAnimationValues);

  MOZ_ASSERT(effectSet ==
                 EffectSet::GetEffectSet(target->mElement, target->mPseudoType),
             "EffectSet should not change while composing style");

  return true;
}

/* static */ dom::Element* EffectCompositor::GetElementToRestyle(
    dom::Element* aElement, PseudoStyleType aPseudoType) {
  if (aPseudoType == PseudoStyleType::NotPseudo) {
>>>>>>> upstream-releases
    return aElement;
  }

  if (aPseudoType == PseudoStyleType::before) {
    return nsLayoutUtils::GetBeforePseudo(aElement);
  }

  if (aPseudoType == PseudoStyleType::after) {
    return nsLayoutUtils::GetAfterPseudo(aElement);
  }

<<<<<<< HEAD
  MOZ_ASSERT_UNREACHABLE(
      "Should not try to get the element to restyle for "
      "a pseudo other that :before or :after");
||||||| merged common ancestors
  MOZ_ASSERT_UNREACHABLE("Should not try to get the element to restyle for "
                         "a pseudo other that :before or :after");
=======
  if (aPseudoType == PseudoStyleType::marker) {
    return nsLayoutUtils::GetMarkerPseudo(aElement);
  }

  MOZ_ASSERT_UNREACHABLE(
      "Should not try to get the element to restyle for "
      "a pseudo other that :before, :after or ::marker");
>>>>>>> upstream-releases
  return nullptr;
}

bool EffectCompositor::HasPendingStyleUpdates() const {
  for (auto& elementSet : mElementsToRestyle) {
    if (elementSet.Count()) {
      return true;
    }
  }

  return false;
}

<<<<<<< HEAD
/* static */ bool EffectCompositor::HasAnimationsForCompositor(
    const nsIFrame* aFrame, nsCSSPropertyID aProperty) {
  return FindAnimationsForCompositor(aFrame, aProperty, nullptr);
||||||| merged common ancestors

/* static */ bool
EffectCompositor::HasAnimationsForCompositor(const nsIFrame* aFrame,
                                             nsCSSPropertyID aProperty)
{
  return FindAnimationsForCompositor(aFrame, aProperty, nullptr);
=======
/* static */
bool EffectCompositor::HasAnimationsForCompositor(const nsIFrame* aFrame,
                                                  DisplayItemType aType) {
  return FindAnimationsForCompositor(
      aFrame, LayerAnimationInfo::GetCSSPropertiesFor(aType), nullptr);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ nsTArray<RefPtr<dom::Animation>>
EffectCompositor::GetAnimationsForCompositor(const nsIFrame* aFrame,
                                             nsCSSPropertyID aProperty) {
||||||| merged common ancestors
/* static */ nsTArray<RefPtr<dom::Animation>>
EffectCompositor::GetAnimationsForCompositor(const nsIFrame* aFrame,
                                             nsCSSPropertyID aProperty)
{
=======
/* static */
nsTArray<RefPtr<dom::Animation>> EffectCompositor::GetAnimationsForCompositor(
    const nsIFrame* aFrame, const nsCSSPropertyIDSet& aPropertySet) {
>>>>>>> upstream-releases
  nsTArray<RefPtr<dom::Animation>> result;

#ifdef DEBUG
  bool foundSome =
#endif
<<<<<<< HEAD
      FindAnimationsForCompositor(aFrame, aProperty, &result);
||||||| merged common ancestors
    FindAnimationsForCompositor(aFrame, aProperty, &result);
=======
      FindAnimationsForCompositor(aFrame, aPropertySet, &result);
>>>>>>> upstream-releases
  MOZ_ASSERT(!foundSome || !result.IsEmpty(),
             "If return value is true, matches array should be non-empty");

  return result;
}

<<<<<<< HEAD
/* static */ void EffectCompositor::ClearIsRunningOnCompositor(
    const nsIFrame* aFrame, nsCSSPropertyID aProperty) {
  EffectSet* effects = EffectSet::GetEffectSet(aFrame);
||||||| merged common ancestors
/* static */ void
EffectCompositor::ClearIsRunningOnCompositor(const nsIFrame *aFrame,
                                             nsCSSPropertyID aProperty)
{
  EffectSet* effects = EffectSet::GetEffectSet(aFrame);
=======
/* static */
void EffectCompositor::ClearIsRunningOnCompositor(const nsIFrame* aFrame,
                                                  DisplayItemType aType) {
  EffectSet* effects = EffectSet::GetEffectSetForFrame(aFrame, aType);
>>>>>>> upstream-releases
  if (!effects) {
    return;
  }

  const nsCSSPropertyIDSet& propertySet =
      LayerAnimationInfo::GetCSSPropertiesFor(aType);
  for (KeyframeEffect* effect : *effects) {
    effect->SetIsRunningOnCompositor(propertySet, false);
  }
}

<<<<<<< HEAD
/* static */ void EffectCompositor::MaybeUpdateCascadeResults(
    Element* aElement, CSSPseudoElementType aPseudoType) {
||||||| merged common ancestors
/* static */ void
EffectCompositor::MaybeUpdateCascadeResults(Element* aElement,
                                            CSSPseudoElementType aPseudoType)
{
=======
/* static */
void EffectCompositor::MaybeUpdateCascadeResults(Element* aElement,
                                                 PseudoStyleType aPseudoType) {
>>>>>>> upstream-releases
  EffectSet* effects = EffectSet::GetEffectSet(aElement, aPseudoType);
  if (!effects || !effects->CascadeNeedsUpdate()) {
    return;
  }

  UpdateCascadeResults(*effects, aElement, aPseudoType);

  MOZ_ASSERT(!effects->CascadeNeedsUpdate(), "Failed to update cascade state");
}

<<<<<<< HEAD
/* static */ Maybe<NonOwningAnimationTarget>
EffectCompositor::GetAnimationElementAndPseudoForFrame(const nsIFrame* aFrame) {
||||||| merged common ancestors
/* static */ Maybe<NonOwningAnimationTarget>
EffectCompositor::GetAnimationElementAndPseudoForFrame(const nsIFrame* aFrame)
{
=======
/* static */
Maybe<NonOwningAnimationTarget>
EffectCompositor::GetAnimationElementAndPseudoForFrame(const nsIFrame* aFrame) {
>>>>>>> upstream-releases
  // Always return the same object to benefit from return-value optimization.
  Maybe<NonOwningAnimationTarget> result;

<<<<<<< HEAD
  CSSPseudoElementType pseudoType = aFrame->Style()->GetPseudoType();
||||||| merged common ancestors
  CSSPseudoElementType pseudoType =
    aFrame->Style()->GetPseudoType();
=======
  PseudoStyleType pseudoType = aFrame->Style()->GetPseudoType();
>>>>>>> upstream-releases

  if (pseudoType != PseudoStyleType::NotPseudo &&
      pseudoType != PseudoStyleType::before &&
      pseudoType != PseudoStyleType::after &&
      pseudoType != PseudoStyleType::marker) {
    return result;
  }

  nsIContent* content = aFrame->GetContent();
  if (!content) {
    return result;
  }

  if (pseudoType == PseudoStyleType::before ||
      pseudoType == PseudoStyleType::after ||
      pseudoType == PseudoStyleType::marker) {
    content = content->GetParent();
    if (!content) {
      return result;
    }
  }

  if (!content->IsElement()) {
    return result;
  }

  result.emplace(content->AsElement(), pseudoType);

  return result;
}

<<<<<<< HEAD
/* static */ nsCSSPropertyIDSet EffectCompositor::GetOverriddenProperties(
    EffectSet& aEffectSet, Element* aElement,
    CSSPseudoElementType aPseudoType) {
||||||| merged common ancestors

/* static */ nsCSSPropertyIDSet
EffectCompositor::GetOverriddenProperties(EffectSet& aEffectSet,
                                          Element* aElement,
                                          CSSPseudoElementType aPseudoType)
{
=======
/* static */
nsCSSPropertyIDSet EffectCompositor::GetOverriddenProperties(
    EffectSet& aEffectSet, Element* aElement, PseudoStyleType aPseudoType) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aElement, "Should have an element to get style data from");

  nsCSSPropertyIDSet result;

  Element* elementToRestyle = GetElementToRestyle(aElement, aPseudoType);
  if (!elementToRestyle) {
    return result;
  }

  static constexpr size_t compositorAnimatableCount =
      nsCSSPropertyIDSet::CompositorAnimatableCount();
  AutoTArray<nsCSSPropertyID, compositorAnimatableCount> propertiesToTrack;
  {
    nsCSSPropertyIDSet propertiesToTrackAsSet;
    for (KeyframeEffect* effect : aEffectSet) {
      for (const AnimationProperty& property : effect->Properties()) {
        if (nsCSSProps::PropHasFlags(property.mProperty,
                                     CSSPropFlags::CanAnimateOnCompositor) &&
            !propertiesToTrackAsSet.HasProperty(property.mProperty)) {
          propertiesToTrackAsSet.AddProperty(property.mProperty);
          propertiesToTrack.AppendElement(property.mProperty);
        }
      }
      // Skip iterating over the rest of the effects if we've already
      // found all the compositor-animatable properties.
      if (propertiesToTrack.Length() == compositorAnimatableCount) {
        break;
      }
    }
  }

  if (propertiesToTrack.IsEmpty()) {
    return result;
  }

  Servo_GetProperties_Overriding_Animation(elementToRestyle, &propertiesToTrack,
                                           &result);
  return result;
}

<<<<<<< HEAD
/* static */ void EffectCompositor::UpdateCascadeResults(
    EffectSet& aEffectSet, Element* aElement,
    CSSPseudoElementType aPseudoType) {
||||||| merged common ancestors
/* static */ void
EffectCompositor::UpdateCascadeResults(EffectSet& aEffectSet,
                                       Element* aElement,
                                       CSSPseudoElementType aPseudoType)
{
=======
/* static */
void EffectCompositor::UpdateCascadeResults(EffectSet& aEffectSet,
                                            Element* aElement,
                                            PseudoStyleType aPseudoType) {
>>>>>>> upstream-releases
  MOZ_ASSERT(EffectSet::GetEffectSet(aElement, aPseudoType) == &aEffectSet,
             "Effect set should correspond to the specified (pseudo-)element");
  if (aEffectSet.IsEmpty()) {
    aEffectSet.MarkCascadeUpdated();
    return;
  }

  // Get a list of effects sorted by composite order.
  nsTArray<KeyframeEffect*> sortedEffectList(aEffectSet.Count());
  for (KeyframeEffect* effect : aEffectSet) {
    sortedEffectList.AppendElement(effect);
  }
  sortedEffectList.Sort(EffectCompositeOrderComparator());

  // Get properties that override the *animations* level of the cascade.
  //
  // We only do this for properties that we can animate on the compositor
  // since we will apply other properties on the main thread where the usual
  // cascade applies.
  nsCSSPropertyIDSet overriddenProperties =
      GetOverriddenProperties(aEffectSet, aElement, aPseudoType);

  nsCSSPropertyIDSet& propertiesWithImportantRules =
      aEffectSet.PropertiesWithImportantRules();
  nsCSSPropertyIDSet& propertiesForAnimationsLevel =
      aEffectSet.PropertiesForAnimationsLevel();

  static constexpr nsCSSPropertyIDSet compositorAnimatables =
      nsCSSPropertyIDSet::CompositorAnimatables();
  // Record which compositor-animatable properties were originally set so we can
  // compare for changes later.
  nsCSSPropertyIDSet prevCompositorPropertiesWithImportantRules =
      propertiesWithImportantRules.Intersect(compositorAnimatables);

  nsCSSPropertyIDSet prevPropertiesForAnimationsLevel =
      propertiesForAnimationsLevel;

  propertiesWithImportantRules.Empty();
  propertiesForAnimationsLevel.Empty();

  nsCSSPropertyIDSet propertiesForTransitionsLevel;

  for (const KeyframeEffect* effect : sortedEffectList) {
    MOZ_ASSERT(effect->GetAnimation(),
               "Effects on a target element should have an Animation");
    CascadeLevel cascadeLevel = effect->GetAnimation()->CascadeLevel();

    for (const AnimationProperty& prop : effect->Properties()) {
      if (overriddenProperties.HasProperty(prop.mProperty)) {
        propertiesWithImportantRules.AddProperty(prop.mProperty);
      }

      switch (cascadeLevel) {
        case EffectCompositor::CascadeLevel::Animations:
          propertiesForAnimationsLevel.AddProperty(prop.mProperty);
          break;
        case EffectCompositor::CascadeLevel::Transitions:
          propertiesForTransitionsLevel.AddProperty(prop.mProperty);
          break;
      }
    }
  }

  aEffectSet.MarkCascadeUpdated();

  nsPresContext* presContext = nsContentUtils::GetContextForContent(aElement);
  if (!presContext) {
    return;
  }

  // If properties for compositor are newly overridden by !important rules, or
  // released from being overridden by !important rules, we need to update
  // layers for animations level because it's a trigger to send animations to
  // the compositor or pull animations back from the compositor.
  if (!prevCompositorPropertiesWithImportantRules.Equals(
          propertiesWithImportantRules.Intersect(compositorAnimatables))) {
    presContext->EffectCompositor()->RequestRestyle(
        aElement, aPseudoType, EffectCompositor::RestyleType::Layer,
        EffectCompositor::CascadeLevel::Animations);
  }

  // If we have transition properties and if the same propery for animations
  // level is newly added or removed, we need to update the transition level
  // rule since the it will be added/removed from the rule tree.
  nsCSSPropertyIDSet changedPropertiesForAnimationLevel =
      prevPropertiesForAnimationsLevel.Xor(propertiesForAnimationsLevel);
  nsCSSPropertyIDSet commonProperties = propertiesForTransitionsLevel.Intersect(
      changedPropertiesForAnimationLevel);
  if (!commonProperties.IsEmpty()) {
    EffectCompositor::RestyleType restyleType =
        changedPropertiesForAnimationLevel.Intersects(compositorAnimatables)
            ? EffectCompositor::RestyleType::Standard
            : EffectCompositor::RestyleType::Layer;
    presContext->EffectCompositor()->RequestRestyle(
        aElement, aPseudoType, restyleType,
        EffectCompositor::CascadeLevel::Transitions);
  }
}

<<<<<<< HEAD
/* static */ void EffectCompositor::SetPerformanceWarning(
    const nsIFrame* aFrame, nsCSSPropertyID aProperty,
    const AnimationPerformanceWarning& aWarning) {
  EffectSet* effects = EffectSet::GetEffectSet(aFrame);
||||||| merged common ancestors
/* static */ void
EffectCompositor::SetPerformanceWarning(
  const nsIFrame *aFrame,
  nsCSSPropertyID aProperty,
  const AnimationPerformanceWarning& aWarning)
{
  EffectSet* effects = EffectSet::GetEffectSet(aFrame);
=======
/* static */
void EffectCompositor::SetPerformanceWarning(
    const nsIFrame* aFrame, const nsCSSPropertyIDSet& aPropertySet,
    const AnimationPerformanceWarning& aWarning) {
  EffectSet* effects = EffectSet::GetEffectSetForFrame(aFrame, aPropertySet);
>>>>>>> upstream-releases
  if (!effects) {
    return;
  }

  for (KeyframeEffect* effect : *effects) {
    effect->SetPerformanceWarning(aPropertySet, aWarning);
  }
}

bool EffectCompositor::PreTraverse(ServoTraversalFlags aFlags) {
  return PreTraverseInSubtree(aFlags, nullptr);
}

bool EffectCompositor::PreTraverseInSubtree(ServoTraversalFlags aFlags,
                                            Element* aRoot) {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(!aRoot || nsContentUtils::GetPresShellForContent(aRoot),
             "Traversal root, if provided, should be bound to a display "
             "document");

  // Convert the root element to the parent element if the root element is
  // pseudo since we check each element in mElementsToRestyle is in the subtree
  // of the root element later in this function, but for pseudo elements the
  // element in mElementsToRestyle is the parent of the pseudo.
<<<<<<< HEAD
  if (aRoot && (aRoot->IsGeneratedContentContainerForBefore() ||
                aRoot->IsGeneratedContentContainerForAfter())) {
||||||| merged common ancestors
  if (aRoot &&
      (aRoot->IsGeneratedContentContainerForBefore() ||
       aRoot->IsGeneratedContentContainerForAfter())) {
=======
  if (aRoot && (aRoot->IsGeneratedContentContainerForBefore() ||
                aRoot->IsGeneratedContentContainerForAfter() ||
                aRoot->IsGeneratedContentContainerForMarker())) {
>>>>>>> upstream-releases
    aRoot = aRoot->GetParentElement();
  }

  AutoRestore<bool> guard(mIsInPreTraverse);
  mIsInPreTraverse = true;

  // We need to force flush all throttled animations if we also have
  // non-animation restyles (since we'll want the up-to-date animation style
  // when we go to process them so we can trigger transitions correctly), and
  // if we are currently flushing all throttled animation restyles.
  bool flushThrottledRestyles =
      (aRoot && aRoot->HasDirtyDescendantsForServo()) ||
      (aFlags & ServoTraversalFlags::FlushThrottledAnimations);

  using ElementsToRestyleIterType =
      nsDataHashtable<PseudoElementHashEntry, bool>::Iterator;
  auto getNeededRestyleTarget =
      [&](const ElementsToRestyleIterType& aIter) -> NonOwningAnimationTarget {
    NonOwningAnimationTarget returnTarget;

    // If aIter.Data() is false, the element only requested a throttled
    // (skippable) restyle, so we can skip it if flushThrottledRestyles is not
    // true.
    if (!flushThrottledRestyles && !aIter.Data()) {
      return returnTarget;
    }

    const NonOwningAnimationTarget& target = aIter.Key();

    // Skip elements in documents without a pres shell. Normally we filter out
    // such elements in RequestRestyle but it can happen that, after adding
    // them to mElementsToRestyle, they are transferred to a different document.
    //
    // We will drop them from mElementsToRestyle at the end of the next full
    // document restyle (at the end of this function) but for consistency with
    // how we treat such elements in RequestRestyle, we just ignore them here.
    if (!nsContentUtils::GetPresShellForContent(target.mElement)) {
      return returnTarget;
    }

    // Ignore restyles that aren't in the flattened tree subtree rooted at
    // aRoot.
    if (aRoot && !nsContentUtils::ContentIsFlattenedTreeDescendantOfForStyle(
                     target.mElement, aRoot)) {
      return returnTarget;
    }

    returnTarget = target;
    return returnTarget;
  };

  bool foundElementsNeedingRestyle = false;

  nsTArray<NonOwningAnimationTarget> elementsWithCascadeUpdates;
  for (size_t i = 0; i < kCascadeLevelCount; ++i) {
    CascadeLevel cascadeLevel = CascadeLevel(i);
    auto& elementSet = mElementsToRestyle[cascadeLevel];
    for (auto iter = elementSet.Iter(); !iter.Done(); iter.Next()) {
      const NonOwningAnimationTarget& target = getNeededRestyleTarget(iter);
      if (!target.mElement) {
        continue;
      }

      EffectSet* effects =
          EffectSet::GetEffectSet(target.mElement, target.mPseudoType);
      if (!effects || !effects->CascadeNeedsUpdate()) {
        continue;
      }

      elementsWithCascadeUpdates.AppendElement(target);
    }
  }

  for (const NonOwningAnimationTarget& target : elementsWithCascadeUpdates) {
    MaybeUpdateCascadeResults(target.mElement, target.mPseudoType);
  }
  elementsWithCascadeUpdates.Clear();

  for (size_t i = 0; i < kCascadeLevelCount; ++i) {
    CascadeLevel cascadeLevel = CascadeLevel(i);
    auto& elementSet = mElementsToRestyle[cascadeLevel];
    for (auto iter = elementSet.Iter(); !iter.Done(); iter.Next()) {
      const NonOwningAnimationTarget& target = getNeededRestyleTarget(iter);
      if (!target.mElement) {
        continue;
      }

      // We need to post restyle hints even if the target is not in EffectSet to
      // ensure the final restyling for removed animations.
      // We can't call PostRestyleEvent directly here since we are still in the
      // middle of the servo traversal.
<<<<<<< HEAD
      mPresContext->RestyleManager()->PostRestyleEventForAnimations(
          target.mElement, target.mPseudoType,
          cascadeLevel == CascadeLevel::Transitions ? eRestyle_CSSTransitions
                                                    : eRestyle_CSSAnimations);
||||||| merged common ancestors
      mPresContext->RestyleManager()->
        PostRestyleEventForAnimations(target.mElement,
                                      target.mPseudoType,
                                      cascadeLevel == CascadeLevel::Transitions
                                        ? eRestyle_CSSTransitions
                                        : eRestyle_CSSAnimations);
=======
      mPresContext->RestyleManager()->PostRestyleEventForAnimations(
          target.mElement, target.mPseudoType,
          cascadeLevel == CascadeLevel::Transitions
              ? StyleRestyleHint_RESTYLE_CSS_TRANSITIONS
              : StyleRestyleHint_RESTYLE_CSS_ANIMATIONS);
>>>>>>> upstream-releases

      foundElementsNeedingRestyle = true;

      EffectSet* effects =
          EffectSet::GetEffectSet(target.mElement, target.mPseudoType);
      if (!effects) {
        // Drop EffectSets that have been destroyed.
        iter.Remove();
        continue;
      }

      for (KeyframeEffect* effect : *effects) {
        effect->GetAnimation()->WillComposeStyle();
      }

      // Remove the element from the list of elements to restyle since we are
      // about to restyle it.
      iter.Remove();
    }

    // If this is a full document restyle, then unconditionally clear
    // elementSet in case there are any elements that didn't match above
    // because they were moved to a document without a pres shell after
    // posting an animation restyle.
    if (!aRoot && flushThrottledRestyles) {
      elementSet.Clear();
    }
  }

  return foundElementsNeedingRestyle;
}

<<<<<<< HEAD
bool EffectCompositor::PreTraverse(dom::Element* aElement,
                                   CSSPseudoElementType aPseudoType) {
  MOZ_ASSERT(NS_IsMainThread());

  // If |aElement|'s document does not have a pres shell, e.g. it is document
  // without a browsing context such as we might get from an XMLHttpRequest, we
  // should not run animations on it.
  if (!nsContentUtils::GetPresShellForContent(aElement)) {
    return false;
  }

  bool found = false;
  if (aPseudoType != CSSPseudoElementType::NotPseudo &&
      aPseudoType != CSSPseudoElementType::before &&
      aPseudoType != CSSPseudoElementType::after) {
    return found;
||||||| merged common ancestors
bool
EffectCompositor::PreTraverse(dom::Element* aElement,
                              CSSPseudoElementType aPseudoType)
{
  MOZ_ASSERT(NS_IsMainThread());

  // If |aElement|'s document does not have a pres shell, e.g. it is document
  // without a browsing context such as we might get from an XMLHttpRequest, we
  // should not run animations on it.
  if (!nsContentUtils::GetPresShellForContent(aElement)) {
    return false;
  }

  bool found = false;
  if (aPseudoType != CSSPseudoElementType::NotPseudo &&
      aPseudoType != CSSPseudoElementType::before &&
      aPseudoType != CSSPseudoElementType::after) {
    return found;
=======
void EffectCompositor::NoteElementForReducing(
    const NonOwningAnimationTarget& aTarget) {
  if (!StaticPrefs::dom_animations_api_autoremove_enabled()) {
    return;
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  AutoRestore<bool> guard(mIsInPreTraverse);
  mIsInPreTraverse = true;

  PseudoElementHashEntry::KeyType key = {aElement, aPseudoType};

  // We need to flush all throttled animation restyles too if we also have
  // non-animation restyles (since we'll want the up-to-date animation style
  // when we go to process them so we can trigger transitions correctly).
  Element* elementToRestyle = GetElementToRestyle(aElement, aPseudoType);
  bool flushThrottledRestyles =
      elementToRestyle && elementToRestyle->HasDirtyDescendantsForServo();
||||||| merged common ancestors
  AutoRestore<bool> guard(mIsInPreTraverse);
  mIsInPreTraverse = true;

  PseudoElementHashEntry::KeyType key = { aElement, aPseudoType };

  // We need to flush all throttled animation restyles too if we also have
  // non-animation restyles (since we'll want the up-to-date animation style
  // when we go to process them so we can trigger transitions correctly).
  Element* elementToRestyle = GetElementToRestyle(aElement, aPseudoType);
  bool flushThrottledRestyles = elementToRestyle &&
                                elementToRestyle->HasDirtyDescendantsForServo();
=======
  Unused << mElementsToReduce.put(
      OwningAnimationTarget{aTarget.mElement, aTarget.mPseudoType});
}
>>>>>>> upstream-releases

static void ReduceEffectSet(EffectSet& aEffectSet) {
  // Get a list of effects sorted by composite order.
  nsTArray<KeyframeEffect*> sortedEffectList(aEffectSet.Count());
  for (KeyframeEffect* effect : aEffectSet) {
    sortedEffectList.AppendElement(effect);
  }
  sortedEffectList.Sort(EffectCompositeOrderComparator());

  nsCSSPropertyIDSet setProperties;

  // Iterate in reverse
  for (auto iter = sortedEffectList.rbegin(); iter != sortedEffectList.rend();
       ++iter) {
    MOZ_ASSERT(*iter && (*iter)->GetAnimation(),
               "Effect in an EffectSet should have an animation");
    KeyframeEffect& effect = **iter;
    Animation& animation = *effect.GetAnimation();
    if (animation.IsRemovable() &&
        effect.GetPropertySet().IsSubsetOf(setProperties)) {
      animation.Remove();
    } else if (animation.IsReplaceable()) {
      setProperties |= effect.GetPropertySet();
    }
  }
}

<<<<<<< HEAD
    mPresContext->RestyleManager()->PostRestyleEventForAnimations(
        aElement, aPseudoType,
        cascadeLevel == CascadeLevel::Transitions ? eRestyle_CSSTransitions
                                                  : eRestyle_CSSAnimations);

    EffectSet* effects = EffectSet::GetEffectSet(aElement, aPseudoType);
    if (effects) {
      MaybeUpdateCascadeResults(aElement, aPseudoType);

      for (KeyframeEffect* effect : *effects) {
        effect->GetAnimation()->WillComposeStyle();
      }
||||||| merged common ancestors
    mPresContext->RestyleManager()->
      PostRestyleEventForAnimations(aElement,
                                    aPseudoType,
                                    cascadeLevel == CascadeLevel::Transitions
                                      ? eRestyle_CSSTransitions
                                      : eRestyle_CSSAnimations);

    EffectSet* effects = EffectSet::GetEffectSet(aElement, aPseudoType);
    if (effects) {
      MaybeUpdateCascadeResults(aElement, aPseudoType);

      for (KeyframeEffect* effect : *effects) {
        effect->GetAnimation()->WillComposeStyle();
      }
=======
void EffectCompositor::ReduceAnimations() {
  for (auto iter = mElementsToReduce.iter(); !iter.done(); iter.next()) {
    const OwningAnimationTarget& target = iter.get();
    EffectSet* effectSet =
        EffectSet::GetEffectSet(target.mElement, target.mPseudoType);
    if (effectSet) {
      ReduceEffectSet(*effectSet);
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
}  // namespace mozilla
||||||| merged common ancestors

} // namespace mozilla
=======
  mElementsToReduce.clear();
}

}  // namespace mozilla
>>>>>>> upstream-releases
