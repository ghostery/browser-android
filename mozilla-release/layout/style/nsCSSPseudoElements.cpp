/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* atom list for CSS pseudo-elements */

#include "nsCSSPseudoElements.h"

#include "mozilla/ArrayUtils.h"

#include "nsCSSAnonBoxes.h"
#include "nsDOMString.h"
#include "nsGkAtomConsts.h"
#include "nsStaticAtomUtils.h"

using namespace mozilla;

// Flags data for each of the pseudo-elements.
/* static */ const uint32_t nsCSSPseudoElements::kPseudoElementFlags[] = {
#define CSS_PSEUDO_ELEMENT(name_, value_, flags_) flags_,
#include "nsCSSPseudoElementList.h"
#undef CSS_PSEUDO_ELEMENT
};

<<<<<<< HEAD
static nsStaticAtom* GetAtomBase() {
||||||| merged common ancestors
static nsStaticAtom*
GetAtomBase()
{
=======
/* static */
nsStaticAtom* nsCSSPseudoElements::GetAtomBase() {
>>>>>>> upstream-releases
  return const_cast<nsStaticAtom*>(
      nsGkAtoms::GetAtomByIndex(kAtomIndex_PseudoElements));
}

bool nsCSSPseudoElements::IsPseudoElement(nsAtom* aAtom) {
  return nsStaticAtomUtils::IsMember(aAtom, GetAtomBase(),
                                     kAtomCount_PseudoElements);
}

<<<<<<< HEAD
/* static */ bool nsCSSPseudoElements::IsCSS2PseudoElement(nsAtom* aAtom) {
||||||| merged common ancestors
/* static */ bool
nsCSSPseudoElements::IsCSS2PseudoElement(nsAtom *aAtom)
{
=======
/* static */
bool nsCSSPseudoElements::IsCSS2PseudoElement(nsAtom* aAtom) {
>>>>>>> upstream-releases
  // We don't implement this using PseudoElementHasFlags because callers
  // want to pass things that could be anon boxes.
<<<<<<< HEAD
  NS_ASSERTION(nsCSSPseudoElements::IsPseudoElement(aAtom) ||
                   nsCSSAnonBoxes::IsAnonBox(aAtom),
               "must be pseudo element or anon box");
||||||| merged common ancestors
  NS_ASSERTION(nsCSSPseudoElements::IsPseudoElement(aAtom) ||
               nsCSSAnonBoxes::IsAnonBox(aAtom),
               "must be pseudo element or anon box");
=======
  MOZ_ASSERT(IsPseudoElement(aAtom), "must be pseudo element");
>>>>>>> upstream-releases
  bool result = aAtom == nsCSSPseudoElements::after() ||
<<<<<<< HEAD
                aAtom == nsCSSPseudoElements::before() ||
                aAtom == nsCSSPseudoElements::firstLetter() ||
                aAtom == nsCSSPseudoElements::firstLine();
  NS_ASSERTION(
      nsCSSAnonBoxes::IsAnonBox(aAtom) ||
          result == PseudoElementHasFlags(
                        GetPseudoType(aAtom, EnabledState::eIgnoreEnabledState),
                        CSS_PSEUDO_ELEMENT_IS_CSS2),
      "result doesn't match flags");
||||||| merged common ancestors
                  aAtom == nsCSSPseudoElements::before() ||
                  aAtom == nsCSSPseudoElements::firstLetter() ||
                  aAtom == nsCSSPseudoElements::firstLine();
  NS_ASSERTION(nsCSSAnonBoxes::IsAnonBox(aAtom) ||
               result == PseudoElementHasFlags(
                   GetPseudoType(aAtom, EnabledState::eIgnoreEnabledState),
                   CSS_PSEUDO_ELEMENT_IS_CSS2),
               "result doesn't match flags");
=======
                aAtom == nsCSSPseudoElements::before() ||
                aAtom == nsCSSPseudoElements::firstLetter() ||
                aAtom == nsCSSPseudoElements::firstLine();
  NS_ASSERTION(
      result == PseudoElementHasFlags(
                    GetPseudoType(aAtom, EnabledState::IgnoreEnabledState),
                    CSS_PSEUDO_ELEMENT_IS_CSS2),
      "result doesn't match flags");
>>>>>>> upstream-releases
  return result;
}

<<<<<<< HEAD
/* static */ CSSPseudoElementType nsCSSPseudoElements::GetPseudoType(
    nsAtom* aAtom, EnabledState aEnabledState) {
  Maybe<uint32_t> index = nsStaticAtomUtils::Lookup(aAtom, GetAtomBase(),
                                                    kAtomCount_PseudoElements);
||||||| merged common ancestors
/* static */ CSSPseudoElementType
nsCSSPseudoElements::GetPseudoType(nsAtom* aAtom, EnabledState aEnabledState)
{
  Maybe<uint32_t> index =
    nsStaticAtomUtils::Lookup(aAtom, GetAtomBase(), kAtomCount_PseudoElements);
=======
/* static */
PseudoStyleType nsCSSPseudoElements::GetPseudoType(nsAtom* aAtom,
                                                   EnabledState aEnabledState) {
  Maybe<uint32_t> index = nsStaticAtomUtils::Lookup(aAtom, GetAtomBase(),
                                                    kAtomCount_PseudoElements);
>>>>>>> upstream-releases
  if (index.isSome()) {
    auto type = static_cast<Type>(*index);
    return IsEnabled(type, aEnabledState) ? type : Type::NotPseudo;
  }

  return Type::NotPseudo;
}

<<<<<<< HEAD
/* static */ nsAtom* nsCSSPseudoElements::GetPseudoAtom(Type aType) {
  MOZ_ASSERT(aType < Type::Count, "Unexpected type");
||||||| merged common ancestors
/* static */ nsAtom*
nsCSSPseudoElements::GetPseudoAtom(Type aType)
{
  MOZ_ASSERT(aType < Type::Count, "Unexpected type");
=======
/* static */
nsAtom* nsCSSPseudoElements::GetPseudoAtom(Type aType) {
  MOZ_ASSERT(PseudoStyle::IsPseudoElement(aType), "Unexpected type");
>>>>>>> upstream-releases
  size_t index = kAtomIndex_PseudoElements + static_cast<size_t>(aType);
  return nsGkAtoms::GetAtomByIndex(index);
}

<<<<<<< HEAD
/* static */ already_AddRefed<nsAtom> nsCSSPseudoElements::GetPseudoAtom(
    const nsAString& aPseudoElement) {
||||||| merged common ancestors
/* static */ already_AddRefed<nsAtom>
nsCSSPseudoElements::GetPseudoAtom(const nsAString& aPseudoElement)
{
=======
/* static */
already_AddRefed<nsAtom> nsCSSPseudoElements::GetPseudoAtom(
    const nsAString& aPseudoElement) {
>>>>>>> upstream-releases
  if (DOMStringIsNull(aPseudoElement) || aPseudoElement.IsEmpty() ||
      aPseudoElement.First() != char16_t(':')) {
    return nullptr;
  }

  // deal with two-colon forms of aPseudoElt
  nsAString::const_iterator start, end;
  aPseudoElement.BeginReading(start);
  aPseudoElement.EndReading(end);
  NS_ASSERTION(start != end, "aPseudoElement is not empty!");
  ++start;
  bool haveTwoColons = true;
  if (start == end || *start != char16_t(':')) {
    --start;
    haveTwoColons = false;
  }
  RefPtr<nsAtom> pseudo = NS_Atomize(Substring(start, end));
  MOZ_ASSERT(pseudo);

  // There aren't any non-CSS2 pseudo-elements with a single ':'
  if (!haveTwoColons &&
      (!IsPseudoElement(pseudo) || !IsCSS2PseudoElement(pseudo))) {
    // XXXbz I'd really rather we threw an exception or something, but
    // the DOM spec sucks.
    return nullptr;
  }

  return pseudo.forget();
}

<<<<<<< HEAD
/* static */ bool nsCSSPseudoElements::PseudoElementSupportsUserActionState(
    const Type aType) {
||||||| merged common ancestors
/* static */ bool
nsCSSPseudoElements::PseudoElementSupportsUserActionState(const Type aType)
{
=======
/* static */
bool nsCSSPseudoElements::PseudoElementSupportsUserActionState(
    const Type aType) {
>>>>>>> upstream-releases
  return PseudoElementHasFlags(aType,
                               CSS_PSEUDO_ELEMENT_SUPPORTS_USER_ACTION_STATE);
}

<<<<<<< HEAD
/* static */ nsString nsCSSPseudoElements::PseudoTypeAsString(
    Type aPseudoType) {
||||||| merged common ancestors
/* static */ nsString
nsCSSPseudoElements::PseudoTypeAsString(Type aPseudoType)
{
=======
/* static */
nsString nsCSSPseudoElements::PseudoTypeAsString(Type aPseudoType) {
>>>>>>> upstream-releases
  switch (aPseudoType) {
    case PseudoStyleType::before:
      return NS_LITERAL_STRING("::before");
    case PseudoStyleType::after:
      return NS_LITERAL_STRING("::after");
    case PseudoStyleType::marker:
      return NS_LITERAL_STRING("::marker");
    default:
      MOZ_ASSERT(aPseudoType == PseudoStyleType::NotPseudo,
                 "Unexpected pseudo type");
      return EmptyString();
  }
}

#ifdef DEBUG
<<<<<<< HEAD
/* static */ void nsCSSPseudoElements::AssertAtoms() {
||||||| merged common ancestors
/* static */ void
nsCSSPseudoElements::AssertAtoms()
{
=======
/* static */
void nsCSSPseudoElements::AssertAtoms() {
>>>>>>> upstream-releases
  nsStaticAtom* base = GetAtomBase();
#  define CSS_PSEUDO_ELEMENT(name_, value_, flags_)                   \
    {                                                                 \
      RefPtr<nsAtom> atom = NS_Atomize(value_);                       \
      size_t index = static_cast<size_t>(PseudoStyleType::name_);     \
      MOZ_ASSERT(atom == nsGkAtoms::PseudoElement_##name_,            \
                 "Static atom for " #name_ " has incorrect value");   \
      MOZ_ASSERT(atom == &base[index],                                \
                 "Static atom for " #name_ " not at expected index"); \
    }
#  include "nsCSSPseudoElementList.h"
#  undef CSS_PSEUDO_ELEMENT
}
#endif
