/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_SMILTargetIdentifier_h
#define mozilla_SMILTargetIdentifier_h

#include "mozilla/dom/Element.h"

namespace mozilla {

/**
 * Struct: SMILTargetIdentifier
 *
 * Tuple of: { Animated Element, Attribute Name }
 *
 * Used in SMILAnimationController as hash key for mapping an animation
 * target to the SMILCompositor for that target.
 *
 * NOTE: Need a nsRefPtr for the element & attribute name, because
 * SMILAnimationController retain its hash table for one sample into the
 * future, and we need to make sure their target isn't deleted in that time.
 */

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTargetIdentifier.h
struct nsSMILTargetIdentifier {
  nsSMILTargetIdentifier()
      : mElement(nullptr),
        mAttributeName(nullptr),
        mAttributeNamespaceID(kNameSpaceID_Unknown) {}
||||||| merged common ancestors
struct nsSMILTargetIdentifier
{
  nsSMILTargetIdentifier()
    : mElement(nullptr), mAttributeName(nullptr),
      mAttributeNamespaceID(kNameSpaceID_Unknown) {}
=======
struct SMILTargetIdentifier {
  SMILTargetIdentifier()
      : mElement(nullptr),
        mAttributeName(nullptr),
        mAttributeNamespaceID(kNameSpaceID_Unknown) {}
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTargetIdentifier.h

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTargetIdentifier.h
  inline bool Equals(const nsSMILTargetIdentifier& aOther) const {
    return (aOther.mElement == mElement &&
            aOther.mAttributeName == mAttributeName &&
||||||| merged common ancestors
  inline bool Equals(const nsSMILTargetIdentifier& aOther) const
  {
    return (aOther.mElement              == mElement &&
            aOther.mAttributeName        == mAttributeName &&
=======
  inline bool Equals(const SMILTargetIdentifier& aOther) const {
    return (aOther.mElement == mElement &&
            aOther.mAttributeName == mAttributeName &&
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTargetIdentifier.h
            aOther.mAttributeNamespaceID == mAttributeNamespaceID);
  }

  RefPtr<mozilla::dom::Element> mElement;
  RefPtr<nsAtom> mAttributeName;
  int32_t mAttributeNamespaceID;
};

/**
 * Class: SMILWeakTargetIdentifier
 *
 * Version of the above struct that uses non-owning pointers.  These are kept
 * private, to ensure that they aren't ever dereferenced (or used at all,
 * outside of Equals()).
 *
 * This is solely for comparisons to determine if a target has changed
 * from one sample to the next.
 */
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTargetIdentifier.h
class nsSMILWeakTargetIdentifier {
 public:
||||||| merged common ancestors
class nsSMILWeakTargetIdentifier
{
public:
=======
class SMILWeakTargetIdentifier {
 public:
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTargetIdentifier.h
  // Trivial constructor
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTargetIdentifier.h
  nsSMILWeakTargetIdentifier() : mElement(nullptr), mAttributeName(nullptr) {}
||||||| merged common ancestors
  nsSMILWeakTargetIdentifier()
    : mElement(nullptr), mAttributeName(nullptr) {}
=======
  SMILWeakTargetIdentifier() : mElement(nullptr), mAttributeName(nullptr) {}
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTargetIdentifier.h

  // Allow us to update a weak identifier to match a given non-weak identifier
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTargetIdentifier.h
  nsSMILWeakTargetIdentifier& operator=(const nsSMILTargetIdentifier& aOther) {
||||||| merged common ancestors
  nsSMILWeakTargetIdentifier&
    operator=(const nsSMILTargetIdentifier& aOther)
  {
=======
  SMILWeakTargetIdentifier& operator=(const SMILTargetIdentifier& aOther) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTargetIdentifier.h
    mElement = aOther.mElement;
    mAttributeName = aOther.mAttributeName;
    return *this;
  }

  // Allow for comparison vs. non-weak identifier
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTargetIdentifier.h
  inline bool Equals(const nsSMILTargetIdentifier& aOther) const {
    return (aOther.mElement == mElement &&
||||||| merged common ancestors
  inline bool Equals(const nsSMILTargetIdentifier& aOther) const
  {
    return (aOther.mElement       == mElement &&
=======
  inline bool Equals(const SMILTargetIdentifier& aOther) const {
    return (aOther.mElement == mElement &&
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTargetIdentifier.h
            aOther.mAttributeName == mAttributeName);
  }

 private:
  const nsIContent* mElement;
  const nsAtom* mAttributeName;
};

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILTargetIdentifier.h
#endif  // NS_SMILTARGETIDENTIFIER_H_
||||||| merged common ancestors
#endif // NS_SMILTARGETIDENTIFIER_H_
=======
}  // namespace mozilla

#endif  // mozilla_SMILTargetIdentifier_h
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILTargetIdentifier.h
