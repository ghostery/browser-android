/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* representation of a value for a SMIL-animated CSS property */

#ifndef mozilla_SMILCSSValueType_h
#define mozilla_SMILCSSValueType_h

#include "mozilla/Attributes.h"
#include "mozilla/SMILType.h"
#include "nsCSSPropertyID.h"
#include "nsStringFwd.h"

namespace mozilla {
struct AnimationValue;
class DeclarationBlock;
namespace dom {
class Element;
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILCSSValueType.h
}  // namespace dom
}  // namespace mozilla
||||||| merged common ancestors
} // namespace dom
} // namespace mozilla
=======
}  // namespace dom
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILCSSValueType.h

/*
 * SMILCSSValueType: Represents a SMIL-animated CSS value.
 */
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILCSSValueType.h
class nsSMILCSSValueType : public nsISMILType {
 public:
  typedef mozilla::dom::Element Element;
  typedef mozilla::AnimationValue AnimationValue;
||||||| merged common ancestors
class nsSMILCSSValueType : public nsISMILType
{
public:
  typedef mozilla::dom::Element Element;
  typedef mozilla::AnimationValue AnimationValue;
=======
class SMILCSSValueType : public SMILType {
 public:
  // Singleton for SMILValue objects to hold onto.
  static SMILCSSValueType sSingleton;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILCSSValueType.h

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILCSSValueType.h
  // Singleton for nsSMILValue objects to hold onto.
  static nsSMILCSSValueType sSingleton;

 protected:
  // nsISMILType Methods
||||||| merged common ancestors
  // Singleton for nsSMILValue objects to hold onto.
  static nsSMILCSSValueType sSingleton;

protected:
  // nsISMILType Methods
=======
 protected:
  // SMILType Methods
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILCSSValueType.h
  // -------------------
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILCSSValueType.h
  void Init(nsSMILValue& aValue) const override;
  void Destroy(nsSMILValue&) const override;
  nsresult Assign(nsSMILValue& aDest, const nsSMILValue& aSrc) const override;
  bool IsEqual(const nsSMILValue& aLeft,
               const nsSMILValue& aRight) const override;
  nsresult Add(nsSMILValue& aDest, const nsSMILValue& aValueToAdd,
||||||| merged common ancestors
  void     Init(nsSMILValue& aValue) const override;
  void     Destroy(nsSMILValue&) const override;
  nsresult Assign(nsSMILValue& aDest,
                  const nsSMILValue& aSrc) const override;
  bool     IsEqual(const nsSMILValue& aLeft,
                   const nsSMILValue& aRight) const override;
  nsresult Add(nsSMILValue& aDest,
               const nsSMILValue& aValueToAdd,
=======
  void Init(SMILValue& aValue) const override;
  void Destroy(SMILValue&) const override;
  nsresult Assign(SMILValue& aDest, const SMILValue& aSrc) const override;
  bool IsEqual(const SMILValue& aLeft, const SMILValue& aRight) const override;
  nsresult Add(SMILValue& aDest, const SMILValue& aValueToAdd,
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILCSSValueType.h
               uint32_t aCount) const override;
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILCSSValueType.h
  nsresult SandwichAdd(nsSMILValue& aDest,
                       const nsSMILValue& aValueToAdd) const override;
  nsresult ComputeDistance(const nsSMILValue& aFrom, const nsSMILValue& aTo,
||||||| merged common ancestors
  nsresult SandwichAdd(nsSMILValue& aDest,
                       const nsSMILValue& aValueToAdd) const override;
  nsresult ComputeDistance(const nsSMILValue& aFrom,
                           const nsSMILValue& aTo,
=======
  nsresult SandwichAdd(SMILValue& aDest,
                       const SMILValue& aValueToAdd) const override;
  nsresult ComputeDistance(const SMILValue& aFrom, const SMILValue& aTo,
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILCSSValueType.h
                           double& aDistance) const override;
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILCSSValueType.h
  nsresult Interpolate(const nsSMILValue& aStartVal, const nsSMILValue& aEndVal,
                       double aUnitDistance,
                       nsSMILValue& aResult) const override;
||||||| merged common ancestors
  nsresult Interpolate(const nsSMILValue& aStartVal,
                       const nsSMILValue& aEndVal,
                       double aUnitDistance,
                       nsSMILValue& aResult) const override;
=======
  nsresult Interpolate(const SMILValue& aStartVal, const SMILValue& aEndVal,
                       double aUnitDistance, SMILValue& aResult) const override;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILCSSValueType.h

 public:
  // Helper Methods
  // --------------
  /**
   * Sets up the given SMILValue to represent the given string value.  The
   * string is interpreted as a value for the given property on the given
   * element.
   *
   * On failure, this method leaves aValue.mType == SMILNullType::sSingleton.
   * Otherwise, this method leaves aValue.mType == this class's singleton.
   *
   * @param       aPropID         The property for which we're parsing a value.
   * @param       aTargetElement  The target element to whom the property/value
   *                              setting applies.
   * @param       aString         The string to be parsed as a CSS value.
   * @param [out] aValue          The SMILValue to be populated. Should
   *                              initially be null-typed.
   * @param [out] aIsContextSensitive Set to true if |aString| may produce
   *                                  a different |aValue| depending on other
   *                                  CSS properties on |aTargetElement|
   *                                  or its ancestors (e.g. 'inherit).
   *                                  false otherwise. May be nullptr.
   *                                  Not set if the method fails.
   * @pre  aValue.IsNull()
   * @post aValue.IsNull() || aValue.mType == SMILCSSValueType::sSingleton
   */
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILCSSValueType.h
  static void ValueFromString(nsCSSPropertyID aPropID, Element* aTargetElement,
                              const nsAString& aString, nsSMILValue& aValue,
||||||| merged common ancestors
  static void ValueFromString(nsCSSPropertyID aPropID,
                              Element* aTargetElement,
                              const nsAString& aString,
                              nsSMILValue& aValue,
=======
  static void ValueFromString(nsCSSPropertyID aPropID,
                              dom::Element* aTargetElement,
                              const nsAString& aString, SMILValue& aValue,
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILCSSValueType.h
                              bool* aIsContextSensitive);

  /**
   * Creates a SMILValue to wrap the given animation value.
   *
   * @param aPropID         The property that |aValue| corresponds to.
   * @param aTargetElement  The target element to which the animation value
   *                        applies.
   * @param aValue          The animation value to use.
   * @return                A new SMILValue. On failure, returns a
   *                        SMILValue with the null type (i.e. rv.IsNull()
   *                        returns true).
   */
  static SMILValue ValueFromAnimationValue(nsCSSPropertyID aPropID,
                                           dom::Element* aTargetElement,
                                           const AnimationValue& aValue);

  /**
   * Sets the relevant property values in the declaration block.
   *
   * Returns whether the declaration changed.
   */
  static bool SetPropertyValues(const SMILValue&, mozilla::DeclarationBlock&);

  /**
   * Return the CSS property animated by the specified value.
   *
   * @param   aValue   The SMILValue to examine.
   * @return           The nsCSSPropertyID enum value of the property animated
   *                   by |aValue|, or eCSSProperty_UNKNOWN if the type of
   *                   |aValue| is not SMILCSSValueType.
   */
  static nsCSSPropertyID PropertyFromValue(const SMILValue& aValue);

  /**
   * If |aValue| is an empty value, converts it to a suitable zero value by
   * matching the type of value stored in |aValueToMatch|.
   *
   * There is no indication if this method fails. If a suitable zero value could
   * not be created, |aValue| is simply unmodified.
   *
   * @param aValue        The SMILValue (of type SMILCSSValueType) to
   *                      possibly update.
   * @param aValueToMatch A SMILValue (of type SMILCSSValueType) for which
   *                      a corresponding zero value will be created if |aValue|
   *                      is empty.
   */
  static void FinalizeValue(SMILValue& aValue, const SMILValue& aValueToMatch);

 private:
  // Private constructor: prevent instances beyond my singleton.
  constexpr SMILCSSValueType() = default;
};

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILCSSValueType.h
#endif  // NS_SMILCSSVALUETYPE_H_
||||||| merged common ancestors
#endif // NS_SMILCSSVALUETYPE_H_
=======
}  // namespace mozilla

#endif  // mozilla_SMILCSSValueType_h
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILCSSValueType.h
