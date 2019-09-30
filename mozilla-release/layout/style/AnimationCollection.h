/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_AnimationCollection_h
#define mozilla_AnimationCollection_h

#include "mozilla/dom/Animation.h"
#include "mozilla/dom/Element.h"
#include "mozilla/Assertions.h"
#include "mozilla/LinkedList.h"
#include "mozilla/RefPtr.h"
#include "nsCSSPseudoElements.h"
#include "nsDOMMutationObserver.h"
#include "nsTArray.h"

class nsAtom;
class nsIFrame;
class nsPresContext;

namespace mozilla {

// Traits class to define the specific atoms used when storing specializations
// of AnimationCollection as a property on an Element (e.g. which atom
// to use when storing an AnimationCollection<CSSAnimation> for a ::before
// pseudo-element).
template <class AnimationType>
struct AnimationTypeTraits {};

template <class AnimationType>
class AnimationCollection
    : public LinkedListElement<AnimationCollection<AnimationType>> {
  typedef AnimationCollection<AnimationType> SelfType;
  typedef AnimationTypeTraits<AnimationType> TraitsType;

  AnimationCollection(dom::Element* aElement, nsAtom* aElementProperty)
<<<<<<< HEAD
      : mElement(aElement),
        mElementProperty(aElementProperty)
#ifdef DEBUG
        ,
        mCalledPropertyDtor(false)
#endif
  {
||||||| merged common ancestors
    : mElement(aElement)
    , mElementProperty(aElementProperty)
#ifdef DEBUG
    , mCalledPropertyDtor(false)
#endif
  {
=======
      : mElement(aElement), mElementProperty(aElementProperty) {
>>>>>>> upstream-releases
    MOZ_COUNT_CTOR(AnimationCollection);
  }

 public:
  ~AnimationCollection() {
    MOZ_ASSERT(mCalledPropertyDtor,
               "must call destructor through element property dtor");
    MOZ_COUNT_DTOR(AnimationCollection);
    LinkedListElement<SelfType>::remove();
  }

<<<<<<< HEAD
  void Destroy() {
||||||| merged common ancestors
  void Destroy()
  {
=======
  void Destroy() {
    mCalledDestroy = true;

>>>>>>> upstream-releases
    // This will call our destructor.
    mElement->DeleteProperty(mElementProperty);
  }

  static void PropertyDtor(void* aObject, nsAtom* aPropertyName,
                           void* aPropertyValue, void* aData);

  // Get the collection of animations for the given |aElement| and
  // |aPseudoType|.
<<<<<<< HEAD
  static AnimationCollection<AnimationType>* GetAnimationCollection(
      const dom::Element* aElement, CSSPseudoElementType aPseudoType);
||||||| merged common ancestors
  static AnimationCollection<AnimationType>*
    GetAnimationCollection(const dom::Element* aElement,
                           CSSPseudoElementType aPseudoType);
=======
  static AnimationCollection<AnimationType>* GetAnimationCollection(
      const dom::Element* aElement, PseudoStyleType aPseudoType);
>>>>>>> upstream-releases

  // Given the frame |aFrame| with possibly animated content, finds its
  // associated collection of animations. If |aFrame| is a generated content
  // frame, this function may examine the parent frame to search for such
  // animations.
  static AnimationCollection<AnimationType>* GetAnimationCollection(
      const nsIFrame* aFrame);

  // Get the collection of animations for the given |aElement| and
  // |aPseudoType| or create it if it does not already exist.
  //
  // We'll set the outparam |aCreatedCollection| to true if we have
  // to create the collection and we successfully do so. Otherwise,
  // we'll set it to false.
<<<<<<< HEAD
  static AnimationCollection<AnimationType>* GetOrCreateAnimationCollection(
      dom::Element* aElement, CSSPseudoElementType aPseudoType,
      bool* aCreatedCollection);
||||||| merged common ancestors
  static AnimationCollection<AnimationType>*
    GetOrCreateAnimationCollection(dom::Element* aElement,
                                   CSSPseudoElementType aPseudoType,
                                   bool* aCreatedCollection);
=======
  static AnimationCollection<AnimationType>* GetOrCreateAnimationCollection(
      dom::Element* aElement, PseudoStyleType aPseudoType,
      bool* aCreatedCollection);
>>>>>>> upstream-releases

  dom::Element* mElement;

  // the atom we use in mElement's prop table (must be a static atom,
  // i.e., in an atom list)
  nsAtom* mElementProperty;

  InfallibleTArray<RefPtr<AnimationType>> mAnimations;

<<<<<<< HEAD
 private:
  static nsAtom* GetPropertyAtomForPseudoType(CSSPseudoElementType aPseudoType);
||||||| merged common ancestors

private:
  static nsAtom* GetPropertyAtomForPseudoType(
    CSSPseudoElementType aPseudoType);
=======
 private:
  static nsAtom* GetPropertyAtomForPseudoType(PseudoStyleType aPseudoType);

  // We distinguish between destroying this by calling Destroy() vs directly
  // calling DeleteProperty on an element.
  //
  // The former case represents regular updating due to style changes and should
  // trigger subsequent restyles.
  //
  // The latter case represents document tear-down or other DOM surgery in
  // which case we should not trigger restyles.
  bool mCalledDestroy = false;
>>>>>>> upstream-releases

#ifdef DEBUG
  bool mCalledPropertyDtor = false;
#endif
};

}  // namespace mozilla

#endif  // mozilla_AnimationCollection_h
