/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* Helper class to help with generating anonymous path elements for
   <animateMotion> elements to use. */

#ifndef MOZILLA_SVGMOTIONSMILPATHUTILS_H_
#define MOZILLA_SVGMOTIONSMILPATHUTILS_H_

#include "mozilla/Attributes.h"
#include "mozilla/RefPtr.h"
#include "mozilla/SMILParserUtils.h"
#include "mozilla/gfx/2D.h"
#include "gfxPlatform.h"
#include "nsDebug.h"
#include "nsStringFwd.h"
#include "nsTArray.h"

namespace mozilla {

<<<<<<< HEAD
class SVGMotionSMILPathUtils {
||||||| merged common ancestors
class SVGMotionSMILPathUtils
{
=======
namespace dom {
class SVGElement;
}

class SVGMotionSMILPathUtils {
>>>>>>> upstream-releases
  typedef mozilla::gfx::DrawTarget DrawTarget;
  typedef mozilla::gfx::Path Path;
  typedef mozilla::gfx::PathBuilder PathBuilder;

 public:
  // Class to assist in generating a Path, based on
  // coordinates in the <animateMotion> from/by/to/values attributes.
  class PathGenerator {
<<<<<<< HEAD
   public:
    explicit PathGenerator(const nsSVGElement* aSVGElement)
        : mSVGElement(aSVGElement), mHaveReceivedCommands(false) {
||||||| merged common ancestors
  public:
    explicit PathGenerator(const nsSVGElement* aSVGElement)
      : mSVGElement(aSVGElement),
        mHaveReceivedCommands(false)
    {
=======
   public:
    explicit PathGenerator(const dom::SVGElement* aSVGElement)
        : mSVGElement(aSVGElement), mHaveReceivedCommands(false) {
>>>>>>> upstream-releases
      RefPtr<DrawTarget> drawTarget =
          gfxPlatform::GetPlatform()->ScreenReferenceDrawTarget();
      NS_ASSERTION(
          gfxPlatform::GetPlatform()->SupportsAzureContentForDrawTarget(
              drawTarget),
          "Should support Moz2D content drawing");

      mPathBuilder = drawTarget->CreatePathBuilder();
    }

    // Methods for adding various path commands to output path.
    // Note: aCoordPairStr is expected to be a whitespace and/or
    // comma-separated x,y coordinate-pair -- see description of
    // "the specified values for from, by, to, and values" at
    //    http://www.w3.org/TR/SVG11/animate.html#AnimateMotionElement
    void MoveToOrigin();
    bool MoveToAbsolute(const nsAString& aCoordPairStr);
    bool LineToAbsolute(const nsAString& aCoordPairStr,
                        double& aSegmentDistance);
    bool LineToRelative(const nsAString& aCoordPairStr,
                        double& aSegmentDistance);

    // Accessor to let clients check if we've received any commands yet.
    inline bool HaveReceivedCommands() { return mHaveReceivedCommands; }
    // Accessor to get the finalized path
    already_AddRefed<Path> GetResultingPath();

   protected:
    // Helper methods
<<<<<<< HEAD
    bool ParseCoordinatePair(const nsAString& aStr, float& aXVal, float& aYVal);
||||||| merged common ancestors
    bool ParseCoordinatePair(const nsAString& aStr,
                               float& aXVal, float& aYVal);
=======
    bool ParseCoordinatePair(const nsAString& aCoordPairStr, float& aXVal,
                             float& aYVal);
>>>>>>> upstream-releases

    // Member data
<<<<<<< HEAD
    const nsSVGElement* mSVGElement;  // context for converting to user units
||||||| merged common ancestors
    const nsSVGElement* mSVGElement; // context for converting to user units
=======
    const dom::SVGElement* mSVGElement;  // context for converting to user units
>>>>>>> upstream-releases
    RefPtr<PathBuilder> mPathBuilder;
    bool mHaveReceivedCommands;
  };

  // Class to assist in passing each subcomponent of a |values| attribute to
  // a PathGenerator, for generating a corresponding Path.
<<<<<<< HEAD
  class MOZ_STACK_CLASS MotionValueParser
      : public nsSMILParserUtils::GenericValueParser {
   public:
||||||| merged common ancestors
  class MOZ_STACK_CLASS MotionValueParser :
    public nsSMILParserUtils::GenericValueParser
  {
  public:
=======
  class MOZ_STACK_CLASS MotionValueParser
      : public SMILParserUtils::GenericValueParser {
   public:
>>>>>>> upstream-releases
    MotionValueParser(PathGenerator* aPathGenerator,
                      FallibleTArray<double>* aPointDistances)
        : mPathGenerator(aPathGenerator),
          mPointDistances(aPointDistances),
          mDistanceSoFar(0.0) {
      MOZ_ASSERT(mPointDistances->IsEmpty(),
                 "expecting point distances array to start empty");
    }

    // SMILParserUtils::GenericValueParser interface
    virtual bool Parse(const nsAString& aValueStr) override;

   protected:
    PathGenerator* mPathGenerator;
    FallibleTArray<double>* mPointDistances;
    double mDistanceSoFar;
  };
};

}  // namespace mozilla

#endif  // MOZILLA_SVGMOTIONSMILPATHUTILS_H_
