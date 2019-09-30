/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "EffectSet.h"
#include "mozilla/dom/Element.h"  // For Element
#include "mozilla/RestyleManager.h"
<<<<<<< HEAD
#include "nsCSSPseudoElements.h"         // For CSSPseudoElementType
#include "nsCycleCollectionNoteChild.h"  // For CycleCollectionNoteChild
||||||| merged common ancestors
#include "nsCSSPseudoElements.h" // For CSSPseudoElementType
#include "nsCycleCollectionNoteChild.h" // For CycleCollectionNoteChild
=======
#include "nsCSSPseudoElements.h"         // For PseudoStyleType
#include "nsCycleCollectionNoteChild.h"  // For CycleCollectionNoteChild
>>>>>>> upstream-releases
#include "nsPresContext.h"
#include "nsLayoutUtils.h"

namespace mozilla {

<<<<<<< HEAD
/* static */ void EffectSet::PropertyDtor(void* aObject, nsAtom* aPropertyName,
                                          void* aPropertyValue, void* aData) {
||||||| merged common ancestors
/* static */ void
EffectSet::PropertyDtor(void* aObject, nsAtom* aPropertyName,
                        void* aPropertyValue, void* aData)
{
=======
/* static */
void EffectSet::PropertyDtor(void* aObject, nsAtom* aPropertyName,
                             void* aPropertyValue, void* aData) {
>>>>>>> upstream-releases
  EffectSet* effectSet = static_cast<EffectSet*>(aPropertyValue);

#ifdef DEBUG
  MOZ_ASSERT(!effectSet->mCalledPropertyDtor, "Should not call dtor twice");
  effectSet->mCalledPropertyDtor = true;
#endif

  delete effectSet;
}

void EffectSet::Traverse(nsCycleCollectionTraversalCallback& aCallback) {
  for (auto iter = mEffects.Iter(); !iter.Done(); iter.Next()) {
    CycleCollectionNoteChild(aCallback, iter.Get()->GetKey(),
                             "EffectSet::mEffects[]", aCallback.Flags());
  }
}

<<<<<<< HEAD
/* static */ EffectSet* EffectSet::GetEffectSet(
    const dom::Element* aElement, CSSPseudoElementType aPseudoType) {
||||||| merged common ancestors
/* static */ EffectSet*
EffectSet::GetEffectSet(const dom::Element* aElement,
                        CSSPseudoElementType aPseudoType)
{
=======
/* static */
EffectSet* EffectSet::GetEffectSet(const dom::Element* aElement,
                                   PseudoStyleType aPseudoType) {
>>>>>>> upstream-releases
  if (!aElement->MayHaveAnimations()) {
    return nullptr;
  }

  nsAtom* propName = GetEffectSetPropertyAtom(aPseudoType);
  return static_cast<EffectSet*>(aElement->GetProperty(propName));
}

<<<<<<< HEAD
/* static */ EffectSet* EffectSet::GetEffectSet(const nsIFrame* aFrame) {
  Maybe<NonOwningAnimationTarget> target =
      EffectCompositor::GetAnimationElementAndPseudoForFrame(aFrame);

  if (!target) {
    return nullptr;
  }

  return GetEffectSet(target->mElement, target->mPseudoType);
}

/* static */ EffectSet* EffectSet::GetOrCreateEffectSet(
    dom::Element* aElement, CSSPseudoElementType aPseudoType) {
||||||| merged common ancestors
/* static */ EffectSet*
EffectSet::GetEffectSet(const nsIFrame* aFrame)
{
  Maybe<NonOwningAnimationTarget> target =
    EffectCompositor::GetAnimationElementAndPseudoForFrame(aFrame);

  if (!target) {
    return nullptr;
  }

  return GetEffectSet(target->mElement, target->mPseudoType);
}

/* static */ EffectSet*
EffectSet::GetOrCreateEffectSet(dom::Element* aElement,
                                CSSPseudoElementType aPseudoType)
{
=======
/* static */
EffectSet* EffectSet::GetOrCreateEffectSet(dom::Element* aElement,
                                           PseudoStyleType aPseudoType) {
>>>>>>> upstream-releases
  EffectSet* effectSet = GetEffectSet(aElement, aPseudoType);
  if (effectSet) {
    return effectSet;
  }

  nsAtom* propName = GetEffectSetPropertyAtom(aPseudoType);
  effectSet = new EffectSet();

  nsresult rv = aElement->SetProperty(propName, effectSet,
                                      &EffectSet::PropertyDtor, true);
  if (NS_FAILED(rv)) {
    NS_WARNING("SetProperty failed");
    // The set must be destroyed via PropertyDtor, otherwise
    // mCalledPropertyDtor assertion is triggered in destructor.
    EffectSet::PropertyDtor(aElement, propName, effectSet, nullptr);
    return nullptr;
  }

  aElement->SetMayHaveAnimations();

  return effectSet;
}

<<<<<<< HEAD
/* static */ void EffectSet::DestroyEffectSet(
    dom::Element* aElement, CSSPseudoElementType aPseudoType) {
||||||| merged common ancestors
/* static */ void
EffectSet::DestroyEffectSet(dom::Element* aElement,
                            CSSPseudoElementType aPseudoType)
{
=======
/* static */
EffectSet* EffectSet::GetEffectSetForFrame(
    const nsIFrame* aFrame, const nsCSSPropertyIDSet& aProperties) {
  MOZ_ASSERT(aFrame);

  // Transform animations are run on the primary frame (but stored on the
  // content associated with the style frame).
  const nsIFrame* frameToQuery = nullptr;
  if (aProperties.IsSubsetOf(nsCSSPropertyIDSet::TransformLikeProperties())) {
    // Make sure to return nullptr if we're looking for transform animations on
    // the inner table frame.
    if (!aFrame->IsFrameOfType(nsIFrame::eSupportsCSSTransforms)) {
      return nullptr;
    }
    frameToQuery = nsLayoutUtils::GetStyleFrame(aFrame);
  } else {
    MOZ_ASSERT(
        !aProperties.Intersects(nsCSSPropertyIDSet::TransformLikeProperties()),
        "We should have only transform properties or no transform properties");
    // We don't need to explicitly return nullptr when |aFrame| is NOT the style
    // frame since there will be no effect set in that case.
    frameToQuery = aFrame;
  }

  Maybe<NonOwningAnimationTarget> target =
      EffectCompositor::GetAnimationElementAndPseudoForFrame(frameToQuery);
  if (!target) {
    return nullptr;
  }

  return GetEffectSet(target->mElement, target->mPseudoType);
}

/* static */
EffectSet* EffectSet::GetEffectSetForFrame(const nsIFrame* aFrame,
                                           DisplayItemType aDisplayItemType) {
  return EffectSet::GetEffectSetForFrame(
      aFrame, LayerAnimationInfo::GetCSSPropertiesFor(aDisplayItemType));
}

/* static */
EffectSet* EffectSet::GetEffectSetForStyleFrame(const nsIFrame* aStyleFrame) {
  Maybe<NonOwningAnimationTarget> target =
      EffectCompositor::GetAnimationElementAndPseudoForFrame(aStyleFrame);

  if (!target) {
    return nullptr;
  }

  return GetEffectSet(target->mElement, target->mPseudoType);
}

/* static */
void EffectSet::DestroyEffectSet(dom::Element* aElement,
                                 PseudoStyleType aPseudoType) {
>>>>>>> upstream-releases
  nsAtom* propName = GetEffectSetPropertyAtom(aPseudoType);
  EffectSet* effectSet =
      static_cast<EffectSet*>(aElement->GetProperty(propName));
  if (!effectSet) {
    return;
  }

  MOZ_ASSERT(!effectSet->IsBeingEnumerated(),
             "Should not destroy an effect set while it is being enumerated");
  effectSet = nullptr;

  aElement->DeleteProperty(propName);
}

void EffectSet::UpdateAnimationGeneration(nsPresContext* aPresContext) {
  mAnimationGeneration =
      aPresContext->RestyleManager()->GetAnimationGeneration();
}

<<<<<<< HEAD
/* static */ nsAtom** EffectSet::GetEffectSetPropertyAtoms() {
  static nsAtom* effectSetPropertyAtoms[] = {
||||||| merged common ancestors
/* static */ nsAtom**
EffectSet::GetEffectSetPropertyAtoms()
{
  static nsAtom* effectSetPropertyAtoms[] =
    {
=======
/* static */
nsAtom** EffectSet::GetEffectSetPropertyAtoms() {
  static nsAtom* effectSetPropertyAtoms[] = {
>>>>>>> upstream-releases
      nsGkAtoms::animationEffectsProperty,
      nsGkAtoms::animationEffectsForBeforeProperty,
<<<<<<< HEAD
      nsGkAtoms::animationEffectsForAfterProperty, nullptr};
||||||| merged common ancestors
      nsGkAtoms::animationEffectsForAfterProperty,
      nullptr
    };
=======
      nsGkAtoms::animationEffectsForAfterProperty,
      nsGkAtoms::animationEffectsForMarkerProperty, nullptr};
>>>>>>> upstream-releases

  return effectSetPropertyAtoms;
}

<<<<<<< HEAD
/* static */ nsAtom* EffectSet::GetEffectSetPropertyAtom(
    CSSPseudoElementType aPseudoType) {
||||||| merged common ancestors
/* static */ nsAtom*
EffectSet::GetEffectSetPropertyAtom(CSSPseudoElementType aPseudoType)
{
=======
/* static */
nsAtom* EffectSet::GetEffectSetPropertyAtom(PseudoStyleType aPseudoType) {
>>>>>>> upstream-releases
  switch (aPseudoType) {
    case PseudoStyleType::NotPseudo:
      return nsGkAtoms::animationEffectsProperty;

    case PseudoStyleType::before:
      return nsGkAtoms::animationEffectsForBeforeProperty;

    case PseudoStyleType::after:
      return nsGkAtoms::animationEffectsForAfterProperty;

    case PseudoStyleType::marker:
      return nsGkAtoms::animationEffectsForMarkerProperty;

    default:
<<<<<<< HEAD
      MOZ_ASSERT_UNREACHABLE(
          "Should not try to get animation effects for "
          "a pseudo other that :before or :after");
||||||| merged common ancestors
      MOZ_ASSERT_UNREACHABLE("Should not try to get animation effects for "
                             "a pseudo other that :before or :after");
=======
      MOZ_ASSERT_UNREACHABLE(
          "Should not try to get animation effects for "
          "a pseudo other that :before, :after or ::marker");
>>>>>>> upstream-releases
      return nullptr;
  }
}

void EffectSet::AddEffect(dom::KeyframeEffect& aEffect) {
  if (!mEffects.EnsureInserted(&aEffect)) {
    return;
  }

  MarkCascadeNeedsUpdate();
}

void EffectSet::RemoveEffect(dom::KeyframeEffect& aEffect) {
  if (!mEffects.EnsureRemoved(&aEffect)) {
    return;
  }

  MarkCascadeNeedsUpdate();
}

}  // namespace mozilla
