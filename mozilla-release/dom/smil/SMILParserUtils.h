/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_SMILParserUtils_h
#define mozilla_SMILParserUtils_h

#include "nsTArray.h"
#include "nsStringFwd.h"

namespace mozilla {

class SMILAttr;
class SMILKeySpline;
class SMILRepeatCount;
class SMILTimeValue;
class SMILTimeValueSpecParams;
class SMILValue;

namespace dom {
class SVGAnimationElement;
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILParserUtils.h
}  // namespace dom
}  // namespace mozilla
||||||| merged common ancestors
} // namespace dom
} // namespace mozilla
=======
}  // namespace dom
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILParserUtils.h

/**
 * Common parsing utilities for the SMIL module. There is little re-use here; it
 * simply serves to simplify other classes by moving parsing outside and to aid
 * unit testing.
 */
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILParserUtils.h
class nsSMILParserUtils {
 public:
||||||| merged common ancestors
class nsSMILParserUtils
{
public:
=======
class SMILParserUtils {
 public:
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILParserUtils.h
  // Abstract helper-class for assisting in parsing |values| attribute
  class MOZ_STACK_CLASS GenericValueParser {
   public:
    virtual bool Parse(const nsAString& aValueStr) = 0;
  };

  static const nsDependentSubstring TrimWhitespace(const nsAString& aString);

  static bool ParseKeySplines(const nsAString& aSpec,
                              FallibleTArray<SMILKeySpline>& aKeySplines);

  // Used for parsing the |keyTimes| and |keyPoints| attributes.
  static bool ParseSemicolonDelimitedProgressList(
      const nsAString& aSpec, bool aNonDecreasing,
      FallibleTArray<double>& aArray);

  static bool ParseValues(const nsAString& aSpec,
                          const mozilla::dom::SVGAnimationElement* aSrcElement,
                          const SMILAttr& aAttribute,
                          FallibleTArray<SMILValue>& aValuesArray,
                          bool& aPreventCachingOfSandwich);

  // Generic method that will run some code on each sub-section of an animation
  // element's "values" list.
  static bool ParseValuesGeneric(const nsAString& aSpec,
                                 GenericValueParser& aParser);

  static bool ParseRepeatCount(const nsAString& aSpec,
                               SMILRepeatCount& aResult);

  static bool ParseTimeValueSpecParams(const nsAString& aSpec,
                                       SMILTimeValueSpecParams& aResult);

  /*
   * Parses a clock value as defined in the SMIL Animation specification.
   * If parsing succeeds the returned value will be a non-negative, definite
   * time value i.e. IsDefinite will return true.
   *
   * @param aSpec    The string containing a clock value, e.g. "10s"
   * @param aResult  The parsed result. [OUT]
   * @return true if parsing succeeded, otherwise false.
   */
<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILParserUtils.h
  static bool ParseClockValue(const nsAString& aSpec, nsSMILTimeValue* aResult);
||||||| merged common ancestors
  static bool ParseClockValue(const nsAString& aSpec,
                              nsSMILTimeValue* aResult);
=======
  static bool ParseClockValue(const nsAString& aSpec, SMILTimeValue* aResult);
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILParserUtils.h

  /*
   * This method checks whether the given string looks like a negative number.
   * Specifically, it checks whether the string looks matches the pattern
   * "[whitespace]*-[numeral].*" If the string matches this pattern, this
   * method returns the index of the first character after the '-' sign
   * (i.e. the index of the absolute value).  If not, this method returns -1.
   */
  static int32_t CheckForNegativeNumber(const nsAString& aStr);
};

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILParserUtils.h
#endif  // NS_SMILPARSERUTILS_H_
||||||| merged common ancestors
#endif // NS_SMILPARSERUTILS_H_
=======
}  // namespace mozilla

#endif  // mozilla_SMILParserUtils_h
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILParserUtils.h
