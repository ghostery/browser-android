/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/CSSPseudoElement.h"
#include "mozilla/dom/CSSPseudoElementBinding.h"
#include "mozilla/dom/Element.h"
#include "mozilla/dom/KeyframeEffectBinding.h"
#include "mozilla/AnimationComparator.h"

namespace mozilla {
namespace dom {

NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE(CSSPseudoElement, mOriginatingElement)

NS_IMPL_CYCLE_COLLECTION_ROOT_NATIVE(CSSPseudoElement, AddRef)
NS_IMPL_CYCLE_COLLECTION_UNROOT_NATIVE(CSSPseudoElement, Release)

<<<<<<< HEAD
CSSPseudoElement::CSSPseudoElement(Element* aElement,
                                   CSSPseudoElementType aType)
    : mParentElement(aElement), mPseudoType(aType) {
||||||| merged common ancestors
CSSPseudoElement::CSSPseudoElement(Element* aElement,
                                   CSSPseudoElementType aType)
  : mParentElement(aElement)
  , mPseudoType(aType)
{
=======
CSSPseudoElement::CSSPseudoElement(dom::Element* aElement,
                                   PseudoStyleType aType)
    : mOriginatingElement(aElement), mPseudoType(aType) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aElement);
<<<<<<< HEAD
  MOZ_ASSERT(aType == CSSPseudoElementType::after ||
                 aType == CSSPseudoElementType::before,
||||||| merged common ancestors
  MOZ_ASSERT(aType == CSSPseudoElementType::after ||
             aType == CSSPseudoElementType::before,
=======
  MOZ_ASSERT(aType == PseudoStyleType::after ||
                 aType == PseudoStyleType::before ||
                 aType == PseudoStyleType::marker,
>>>>>>> upstream-releases
             "Unexpected Pseudo Type");
}

CSSPseudoElement::~CSSPseudoElement() {
  // Element might have been unlinked already, so we have to do null check.
<<<<<<< HEAD
  if (mParentElement) {
    mParentElement->DeleteProperty(
        GetCSSPseudoElementPropertyAtom(mPseudoType));
||||||| merged common ancestors
  if (mParentElement) {
    mParentElement->DeleteProperty(
      GetCSSPseudoElementPropertyAtom(mPseudoType));
=======
  if (mOriginatingElement) {
    mOriginatingElement->DeleteProperty(
        GetCSSPseudoElementPropertyAtom(mPseudoType));
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
ParentObject CSSPseudoElement::GetParentObject() const {
  return mParentElement->GetParentObject();
||||||| merged common ancestors
ParentObject
CSSPseudoElement::GetParentObject() const
{
  return mParentElement->GetParentObject();
=======
ParentObject CSSPseudoElement::GetParentObject() const {
  return mOriginatingElement->GetParentObject();
>>>>>>> upstream-releases
}

JSObject* CSSPseudoElement::WrapObject(JSContext* aCx,
                                       JS::Handle<JSObject*> aGivenProto) {
  return CSSPseudoElement_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD
void CSSPseudoElement::GetAnimations(const AnimationFilter& filter,
                                     nsTArray<RefPtr<Animation>>& aRetVal) {
  nsIDocument* doc = mParentElement->GetComposedDoc();
||||||| merged common ancestors
void
CSSPseudoElement::GetAnimations(const AnimationFilter& filter,
                                nsTArray<RefPtr<Animation>>& aRetVal)
{
  nsIDocument* doc = mParentElement->GetComposedDoc();
=======
void CSSPseudoElement::GetAnimations(const GetAnimationsOptions& aOptions,
                                     nsTArray<RefPtr<Animation>>& aRetVal) {
  Document* doc = mOriginatingElement->GetComposedDoc();
>>>>>>> upstream-releases
  if (doc) {
    // We don't need to explicitly flush throttled animations here, since
    // updating the animation style of (pseudo-)elements will never affect the
    // set of running animations and it's only the set of running animations
    // that is important here.
    doc->FlushPendingNotifications(
        ChangesToFlush(FlushType::Style, false /* flush animations */));
  }

  Element::GetAnimationsUnsorted(mOriginatingElement, mPseudoType, aRetVal);
  aRetVal.Sort(AnimationPtrComparator<RefPtr<Animation>>());
}

already_AddRefed<Animation> CSSPseudoElement::Animate(
    JSContext* aContext, JS::Handle<JSObject*> aKeyframes,
    const UnrestrictedDoubleOrKeyframeAnimationOptions& aOptions,
    ErrorResult& aError) {
  Nullable<ElementOrCSSPseudoElement> target;
  target.SetValue().SetAsCSSPseudoElement() = this;
  return Element::Animate(target, aContext, aKeyframes, aOptions, aError);
}

<<<<<<< HEAD
/* static */ already_AddRefed<CSSPseudoElement>
CSSPseudoElement::GetCSSPseudoElement(Element* aElement,
                                      CSSPseudoElementType aType) {
||||||| merged common ancestors
/* static */ already_AddRefed<CSSPseudoElement>
CSSPseudoElement::GetCSSPseudoElement(Element* aElement,
                                      CSSPseudoElementType aType)
{
=======
/* static */
already_AddRefed<CSSPseudoElement> CSSPseudoElement::GetCSSPseudoElement(
    dom::Element* aElement, PseudoStyleType aType) {
>>>>>>> upstream-releases
  if (!aElement) {
    return nullptr;
  }

  nsAtom* propName = CSSPseudoElement::GetCSSPseudoElementPropertyAtom(aType);
  RefPtr<CSSPseudoElement> pseudo =
      static_cast<CSSPseudoElement*>(aElement->GetProperty(propName));
  if (pseudo) {
    return pseudo.forget();
  }

  // CSSPseudoElement is a purely external interface created on-demand, and
  // when all references from script to the pseudo are dropped, we can drop the
  // CSSPseudoElement object, so use a non-owning reference from Element to
  // CSSPseudoElement.
  pseudo = new CSSPseudoElement(aElement, aType);
  nsresult rv = aElement->SetProperty(propName, pseudo, nullptr, true);
  if (NS_FAILED(rv)) {
    NS_WARNING("SetProperty failed");
    return nullptr;
  }
  return pseudo.forget();
}

<<<<<<< HEAD
/* static */ nsAtom* CSSPseudoElement::GetCSSPseudoElementPropertyAtom(
    CSSPseudoElementType aType) {
||||||| merged common ancestors
/* static */ nsAtom*
CSSPseudoElement::GetCSSPseudoElementPropertyAtom(CSSPseudoElementType aType)
{
=======
/* static */
nsAtom* CSSPseudoElement::GetCSSPseudoElementPropertyAtom(
    PseudoStyleType aType) {
>>>>>>> upstream-releases
  switch (aType) {
    case PseudoStyleType::before:
      return nsGkAtoms::cssPseudoElementBeforeProperty;

    case PseudoStyleType::after:
      return nsGkAtoms::cssPseudoElementAfterProperty;

    case PseudoStyleType::marker:
      return nsGkAtoms::cssPseudoElementMarkerProperty;

    default:
<<<<<<< HEAD
      MOZ_ASSERT_UNREACHABLE(
          "Should not try to get CSSPseudoElement "
          "other than ::before or ::after");
||||||| merged common ancestors
      MOZ_ASSERT_UNREACHABLE("Should not try to get CSSPseudoElement "
                             "other than ::before or ::after");
=======
      MOZ_ASSERT_UNREACHABLE(
          "Should not try to get CSSPseudoElement "
          "other than ::before, ::after or ::marker");
>>>>>>> upstream-releases
      return nullptr;
  }
}

}  // namespace dom
}  // namespace mozilla
