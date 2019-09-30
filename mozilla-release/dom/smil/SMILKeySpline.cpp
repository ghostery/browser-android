/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SMILKeySpline.h"
#include <stdint.h>
#include <math.h>

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILKeySpline.cpp
#define NEWTON_ITERATIONS 4
#define NEWTON_MIN_SLOPE 0.02
#define SUBDIVISION_PRECISION 0.0000001
||||||| merged common ancestors
#define NEWTON_ITERATIONS          4
#define NEWTON_MIN_SLOPE           0.02
#define SUBDIVISION_PRECISION      0.0000001
=======
namespace mozilla {

#define NEWTON_ITERATIONS 4
#define NEWTON_MIN_SLOPE 0.02
#define SUBDIVISION_PRECISION 0.0000001
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILKeySpline.cpp
#define SUBDIVISION_MAX_ITERATIONS 10

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILKeySpline.cpp
const double nsSMILKeySpline::kSampleStepSize =
    1.0 / double(kSplineTableSize - 1);

void nsSMILKeySpline::Init(double aX1, double aY1, double aX2, double aY2) {
||||||| merged common ancestors
const double nsSMILKeySpline::kSampleStepSize =
                                        1.0 / double(kSplineTableSize - 1);

void
nsSMILKeySpline::Init(double aX1,
                      double aY1,
                      double aX2,
                      double aY2)
{
=======
const double SMILKeySpline::kSampleStepSize =
    1.0 / double(kSplineTableSize - 1);

void SMILKeySpline::Init(double aX1, double aY1, double aX2, double aY2) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILKeySpline.cpp
  mX1 = aX1;
  mY1 = aY1;
  mX2 = aX2;
  mY2 = aY2;

  if (mX1 != mY1 || mX2 != mY2) CalcSampleValues();
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILKeySpline.cpp
double nsSMILKeySpline::GetSplineValue(double aX) const {
  if (mX1 == mY1 && mX2 == mY2) return aX;
||||||| merged common ancestors
double
nsSMILKeySpline::GetSplineValue(double aX) const
{
  if (mX1 == mY1 && mX2 == mY2)
    return aX;
=======
double SMILKeySpline::GetSplineValue(double aX) const {
  if (mX1 == mY1 && mX2 == mY2) return aX;
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILKeySpline.cpp

  return CalcBezier(GetTForX(aX), mY1, mY2);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILKeySpline.cpp
void nsSMILKeySpline::GetSplineDerivativeValues(double aX, double& aDX,
                                                double& aDY) const {
||||||| merged common ancestors
void
nsSMILKeySpline::GetSplineDerivativeValues(double aX, double& aDX, double& aDY) const
{
=======
void SMILKeySpline::GetSplineDerivativeValues(double aX, double& aDX,
                                              double& aDY) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILKeySpline.cpp
  double t = GetTForX(aX);
  aDX = GetSlope(t, mX1, mX2);
  aDY = GetSlope(t, mY1, mY2);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILKeySpline.cpp
void nsSMILKeySpline::CalcSampleValues() {
||||||| merged common ancestors
void
nsSMILKeySpline::CalcSampleValues()
{
=======
void SMILKeySpline::CalcSampleValues() {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILKeySpline.cpp
  for (uint32_t i = 0; i < kSplineTableSize; ++i) {
    mSampleValues[i] = CalcBezier(double(i) * kSampleStepSize, mX1, mX2);
  }
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILKeySpline.cpp
/*static*/ double nsSMILKeySpline::CalcBezier(double aT, double aA1,
                                              double aA2) {
||||||| merged common ancestors
/*static*/ double
nsSMILKeySpline::CalcBezier(double aT,
                            double aA1,
                            double aA2)
{
=======
/*static*/
double SMILKeySpline::CalcBezier(double aT, double aA1, double aA2) {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILKeySpline.cpp
  // use Horner's scheme to evaluate the Bezier polynomial
  return ((A(aA1, aA2) * aT + B(aA1, aA2)) * aT + C(aA1)) * aT;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILKeySpline.cpp
/*static*/ double nsSMILKeySpline::GetSlope(double aT, double aA1, double aA2) {
  return 3.0 * A(aA1, aA2) * aT * aT + 2.0 * B(aA1, aA2) * aT + C(aA1);
||||||| merged common ancestors
/*static*/ double
nsSMILKeySpline::GetSlope(double aT,
                          double aA1,
                          double aA2)
{
  return 3.0 * A(aA1, aA2)*aT*aT + 2.0 * B(aA1, aA2) * aT + C(aA1);
=======
/*static*/
double SMILKeySpline::GetSlope(double aT, double aA1, double aA2) {
  return 3.0 * A(aA1, aA2) * aT * aT + 2.0 * B(aA1, aA2) * aT + C(aA1);
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILKeySpline.cpp
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILKeySpline.cpp
double nsSMILKeySpline::GetTForX(double aX) const {
||||||| merged common ancestors
double
nsSMILKeySpline::GetTForX(double aX) const
{
=======
double SMILKeySpline::GetTForX(double aX) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILKeySpline.cpp
  // Early return when aX == 1.0 to avoid floating-point inaccuracies.
  if (aX == 1.0) {
    return 1.0;
  }
  // Find interval where t lies
  double intervalStart = 0.0;
  const double* currentSample = &mSampleValues[1];
  const double* const lastSample = &mSampleValues[kSplineTableSize - 1];
  for (; currentSample != lastSample && *currentSample <= aX; ++currentSample) {
    intervalStart += kSampleStepSize;
  }
  --currentSample;  // t now lies between *currentSample and *currentSample+1

  // Interpolate to provide an initial guess for t
  double dist = (aX - *currentSample) / (*(currentSample + 1) - *currentSample);
  double guessForT = intervalStart + dist * kSampleStepSize;

  // Check the slope to see what strategy to use. If the slope is too small
  // Newton-Raphson iteration won't converge on a root so we use bisection
  // instead.
  double initialSlope = GetSlope(guessForT, mX1, mX2);
  if (initialSlope >= NEWTON_MIN_SLOPE) {
    return NewtonRaphsonIterate(aX, guessForT);
  }
  if (initialSlope == 0.0) {
    return guessForT;
  }
  return BinarySubdivide(aX, intervalStart, intervalStart + kSampleStepSize);
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILKeySpline.cpp
double nsSMILKeySpline::NewtonRaphsonIterate(double aX, double aGuessT) const {
||||||| merged common ancestors
double
nsSMILKeySpline::NewtonRaphsonIterate(double aX, double aGuessT) const
{
=======
double SMILKeySpline::NewtonRaphsonIterate(double aX, double aGuessT) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILKeySpline.cpp
  // Refine guess with Newton-Raphson iteration
  for (uint32_t i = 0; i < NEWTON_ITERATIONS; ++i) {
    // We're trying to find where f(t) = aX,
    // so we're actually looking for a root for: CalcBezier(t) - aX
    double currentX = CalcBezier(aGuessT, mX1, mX2) - aX;
    double currentSlope = GetSlope(aGuessT, mX1, mX2);

    if (currentSlope == 0.0) return aGuessT;

    aGuessT -= currentX / currentSlope;
  }

  return aGuessT;
}

<<<<<<< HEAD:mozilla-release/dom/smil/nsSMILKeySpline.cpp
double nsSMILKeySpline::BinarySubdivide(double aX, double aA, double aB) const {
||||||| merged common ancestors
double
nsSMILKeySpline::BinarySubdivide(double aX, double aA, double aB) const
{
=======
double SMILKeySpline::BinarySubdivide(double aX, double aA, double aB) const {
>>>>>>> upstream-releases:mozilla-release/dom/smil/SMILKeySpline.cpp
  double currentX;
  double currentT;
  uint32_t i = 0;

  do {
    currentT = aA + (aB - aA) / 2.0;
    currentX = CalcBezier(currentT, mX1, mX2) - aX;

    if (currentX > 0.0) {
      aB = currentT;
    } else {
      aA = currentT;
    }
  } while (fabs(currentX) > SUBDIVISION_PRECISION &&
           ++i < SUBDIVISION_MAX_ITERATIONS);

  return currentT;
}

}  // namespace mozilla
