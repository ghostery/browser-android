/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* atom list for CSS anonymous boxes */

#ifndef nsCSSAnonBoxes_h___
#define nsCSSAnonBoxes_h___

#include "nsAtom.h"
#include "nsGkAtoms.h"
#include "mozilla/PseudoStyleType.h"

class nsCSSAnonBoxes {
<<<<<<< HEAD
 public:
  static bool IsAnonBox(nsAtom* aAtom);
||||||| merged common ancestors
public:
  static bool IsAnonBox(nsAtom *aAtom);
=======
  using PseudoStyleType = mozilla::PseudoStyleType;
  using PseudoStyle = mozilla::PseudoStyle;

 public:
>>>>>>> upstream-releases
#ifdef MOZ_XUL
  static bool IsTreePseudoElement(nsAtom* aPseudo);
#endif
<<<<<<< HEAD
  static bool IsNonElement(nsAtom* aPseudo) {
    return aPseudo == nsCSSAnonBoxes::mozText() ||
           aPseudo == nsCSSAnonBoxes::oofPlaceholder() ||
           aPseudo == nsCSSAnonBoxes::firstLetterContinuation();
||||||| merged common ancestors
  static bool IsNonElement(nsAtom* aPseudo)
  {
    return aPseudo == nsCSSAnonBoxes::mozText() ||
           aPseudo == nsCSSAnonBoxes::oofPlaceholder() ||
           aPseudo == nsCSSAnonBoxes::firstLetterContinuation();
=======
  static bool IsNonElement(PseudoStyleType aPseudo) {
    return aPseudo == PseudoStyleType::mozText ||
           aPseudo == PseudoStyleType::oofPlaceholder ||
           aPseudo == PseudoStyleType::firstLetterContinuation;
>>>>>>> upstream-releases
  }

  enum class NonInheriting : uint8_t {
#define CSS_ANON_BOX(_name, _value) /* nothing */
#define CSS_NON_INHERITING_ANON_BOX(_name, _value) _name,
#include "nsCSSAnonBoxList.h"
#undef CSS_NON_INHERITING_ANON_BOX
#undef CSS_ANON_BOX
    _Count
  };

<<<<<<< HEAD
  // Be careful using this: if we have a lot of non-inheriting anon box types it
  // might not be very fast.  We may want to think of ways to handle that
  // (e.g. by moving to an enum instead of an atom, like we did for
  // pseudo-elements, or by adding a new value of the pseudo-element enum for
  // non-inheriting anon boxes or something).
  static bool IsNonInheritingAnonBox(nsAtom* aPseudo) {
    return
#define CSS_ANON_BOX(_name, _value) /* nothing */
#define CSS_NON_INHERITING_ANON_BOX(_name, _value) \
  nsGkAtoms::AnonBox_##_name == aPseudo ||
#include "nsCSSAnonBoxList.h"
#undef CSS_NON_INHERITING_ANON_BOX
#undef CSS_ANON_BOX
        false;
  }

#ifdef DEBUG
  // NOTE(emilio): DEBUG only because this does a pretty slow linear search. Try
  // to use IsNonInheritingAnonBox if you know the atom is an anon box already
  // or, even better, nothing like this.  Note that this function returns true
  // for wrapper anon boxes as well, since they're all inheriting.
  static bool IsInheritingAnonBox(nsAtom* aPseudo) {
    return
#define CSS_ANON_BOX(_name, _value) nsGkAtoms::AnonBox_##_name == aPseudo ||
#define CSS_NON_INHERITING_ANON_BOX(_name, _value) /* nothing */
#include "nsCSSAnonBoxList.h"
#undef CSS_NON_INHERITING_ANON_BOX
#undef CSS_ANON_BOX
        false;
  }
#endif  // DEBUG

  // This function is rather slow; you probably don't want to use it outside
  // asserts unless you have to.
  static bool IsWrapperAnonBox(nsAtom* aPseudo) {
    // We commonly get null passed here, and want to quickly return false for
    // it.
    if (!aPseudo) {
      return false;
    }
    return
#define CSS_ANON_BOX(_name, _value) /* nothing */
#define CSS_WRAPPER_ANON_BOX(_name, _value) \
  nsGkAtoms::AnonBox_##_name == aPseudo ||
#define CSS_NON_INHERITING_ANON_BOX(_name, _value) /* nothing */
#include "nsCSSAnonBoxList.h"
#undef CSS_NON_INHERITING_ANON_BOX
#undef CSS_WRAPPER_ANON_BOX
#undef CSS_ANON_BOX
        false;
  }

||||||| merged common ancestors
  // Be careful using this: if we have a lot of non-inheriting anon box types it
  // might not be very fast.  We may want to think of ways to handle that
  // (e.g. by moving to an enum instead of an atom, like we did for
  // pseudo-elements, or by adding a new value of the pseudo-element enum for
  // non-inheriting anon boxes or something).
  static bool IsNonInheritingAnonBox(nsAtom* aPseudo)
  {
    return
#define CSS_ANON_BOX(_name, _value) /* nothing */
#define CSS_NON_INHERITING_ANON_BOX(_name, _value) \
      nsGkAtoms::AnonBox_##_name == aPseudo ||
#include "nsCSSAnonBoxList.h"
#undef CSS_NON_INHERITING_ANON_BOX
#undef CSS_ANON_BOX
      false;
  }

#ifdef DEBUG
  // NOTE(emilio): DEBUG only because this does a pretty slow linear search. Try
  // to use IsNonInheritingAnonBox if you know the atom is an anon box already
  // or, even better, nothing like this.  Note that this function returns true
  // for wrapper anon boxes as well, since they're all inheriting.
  static bool IsInheritingAnonBox(nsAtom* aPseudo)
  {
    return
#define CSS_ANON_BOX(_name, _value) nsGkAtoms::AnonBox_##_name == aPseudo ||
#define CSS_NON_INHERITING_ANON_BOX(_name, _value) /* nothing */
#include "nsCSSAnonBoxList.h"
#undef CSS_NON_INHERITING_ANON_BOX
#undef CSS_ANON_BOX
      false;
  }
#endif // DEBUG

  // This function is rather slow; you probably don't want to use it outside
  // asserts unless you have to.
  static bool IsWrapperAnonBox(nsAtom* aPseudo) {
    // We commonly get null passed here, and want to quickly return false for
    // it.
    return aPseudo &&
      (
#define CSS_ANON_BOX(_name, _value) /* nothing */
#define CSS_WRAPPER_ANON_BOX(_name, _value) nsGkAtoms::AnonBox_##_name == aPseudo ||
#define CSS_NON_INHERITING_ANON_BOX(_name, _value) /* nothing */
#include "nsCSSAnonBoxList.h"
#undef CSS_NON_INHERITING_ANON_BOX
#undef CSS_WRAPPER_ANON_BOX
#undef CSS_ANON_BOX
       false);
  }

=======
>>>>>>> upstream-releases
  // Get the NonInheriting type for a given pseudo tag.  The pseudo tag must
  // test true for IsNonInheritingAnonBox.
  static NonInheriting NonInheritingTypeForPseudoType(PseudoStyleType aType) {
    MOZ_ASSERT(PseudoStyle::IsNonInheritingAnonBox(aType));
    static_assert(sizeof(PseudoStyleType) == sizeof(uint8_t), "");
    return static_cast<NonInheriting>(
        static_cast<uint8_t>(aType) -
        static_cast<uint8_t>(PseudoStyleType::NonInheritingAnonBoxesStart));
  }

#ifdef DEBUG
  static nsStaticAtom* GetAtomBase();
  static void AssertAtoms();
#endif

// Alias nsCSSAnonBoxes::foo() to nsGkAtoms::AnonBox_foo.
#define CSS_ANON_BOX(name_, value_)                       \
  static nsCSSAnonBoxPseudoStaticAtom* name_() {          \
    return const_cast<nsCSSAnonBoxPseudoStaticAtom*>(     \
        static_cast<const nsCSSAnonBoxPseudoStaticAtom*>( \
            nsGkAtoms::AnonBox_##name_));                 \
  }
#include "nsCSSAnonBoxList.h"
#undef CSS_ANON_BOX
};

#endif /* nsCSSAnonBoxes_h___ */
