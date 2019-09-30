/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* implementation of SMILType for use by <animateMotion> element */

#ifndef MOZILLA_SVGMOTIONSMILTYPE_H_
#define MOZILLA_SVGMOTIONSMILTYPE_H_

#include "mozilla/gfx/2D.h"
#include "mozilla/Attributes.h"
#include "mozilla/SMILType.h"

namespace mozilla {

class SMILValue;

/**
 * MotionRotateType: Enum to indicate the type of our "rotate" attribute.
 */
enum RotateType {
  eRotateType_Explicit,    // for e.g. rotate="45"/"45deg"/"0.785rad"
  eRotateType_Auto,        // for rotate="auto"
  eRotateType_AutoReverse  // for rotate="auto-reverse"
};

/**
 * SVGMotionSMILType: Implements the SMILType interface for SMIL animations
 * from <animateMotion>.
 *
 * NOTE: Even though there's technically no "motion" attribute, we behave in
 * many ways as if there were, for simplicity.
 */
<<<<<<< HEAD
class SVGMotionSMILType : public nsISMILType {
||||||| merged common ancestors
class SVGMotionSMILType : public nsISMILType
{
=======
class SVGMotionSMILType : public SMILType {
>>>>>>> upstream-releases
  typedef mozilla::gfx::Path Path;

<<<<<<< HEAD
 public:
  // Singleton for nsSMILValue objects to hold onto.
||||||| merged common ancestors
public:
  // Singleton for nsSMILValue objects to hold onto.
=======
 public:
  // Singleton for SMILValue objects to hold onto.
>>>>>>> upstream-releases
  static SVGMotionSMILType sSingleton;

<<<<<<< HEAD
 protected:
  // nsISMILType Methods
||||||| merged common ancestors
protected:
  // nsISMILType Methods
=======
 protected:
  // SMILType Methods
>>>>>>> upstream-releases
  // -------------------
<<<<<<< HEAD
  virtual void Init(nsSMILValue& aValue) const override;
  virtual void Destroy(nsSMILValue& aValue) const override;
  virtual nsresult Assign(nsSMILValue& aDest,
                          const nsSMILValue& aSrc) const override;
  virtual bool IsEqual(const nsSMILValue& aLeft,
                       const nsSMILValue& aRight) const override;
  virtual nsresult Add(nsSMILValue& aDest, const nsSMILValue& aValueToAdd,
||||||| merged common ancestors
  virtual void     Init(nsSMILValue& aValue) const override;
  virtual void     Destroy(nsSMILValue& aValue) const override;
  virtual nsresult Assign(nsSMILValue& aDest, const nsSMILValue& aSrc) const override;
  virtual bool     IsEqual(const nsSMILValue& aLeft,
                           const nsSMILValue& aRight) const override;
  virtual nsresult Add(nsSMILValue& aDest,
                       const nsSMILValue& aValueToAdd,
=======
  virtual void Init(SMILValue& aValue) const override;
  virtual void Destroy(SMILValue& aValue) const override;
  virtual nsresult Assign(SMILValue& aDest,
                          const SMILValue& aSrc) const override;
  virtual bool IsEqual(const SMILValue& aLeft,
                       const SMILValue& aRight) const override;
  virtual nsresult Add(SMILValue& aDest, const SMILValue& aValueToAdd,
>>>>>>> upstream-releases
                       uint32_t aCount) const override;
  virtual nsresult SandwichAdd(SMILValue& aDest,
                               const SMILValue& aValueToAdd) const override;
  virtual nsresult ComputeDistance(const SMILValue& aFrom, const SMILValue& aTo,
                                   double& aDistance) const override;
<<<<<<< HEAD
  virtual nsresult Interpolate(const nsSMILValue& aStartVal,
                               const nsSMILValue& aEndVal, double aUnitDistance,
                               nsSMILValue& aResult) const override;

 public:
  // Used to generate a transform matrix from an <animateMotion> nsSMILValue.
  static gfx::Matrix CreateMatrix(const nsSMILValue& aSMILVal);
||||||| merged common ancestors
  virtual nsresult Interpolate(const nsSMILValue& aStartVal,
                               const nsSMILValue& aEndVal,
                               double aUnitDistance,
                               nsSMILValue& aResult) const override;
public:
  // Used to generate a transform matrix from an <animateMotion> nsSMILValue.
  static gfx::Matrix CreateMatrix(const nsSMILValue& aSMILVal);
=======
  virtual nsresult Interpolate(const SMILValue& aStartVal,
                               const SMILValue& aEndVal, double aUnitDistance,
                               SMILValue& aResult) const override;

 public:
  // Used to generate a transform matrix from an <animateMotion> SMILValue.
  static gfx::Matrix CreateMatrix(const SMILValue& aSMILVal);
>>>>>>> upstream-releases

  // Used to generate a SMILValue for the point at the given distance along
  // the given path.
<<<<<<< HEAD
  static nsSMILValue ConstructSMILValue(Path* aPath, float aDist,
                                        RotateType aRotateType,
                                        float aRotateAngle);
||||||| merged common ancestors
  static nsSMILValue ConstructSMILValue(Path* aPath,
                                        float aDist,
                                        RotateType aRotateType,
                                        float aRotateAngle);
=======
  static SMILValue ConstructSMILValue(Path* aPath, float aDist,
                                      RotateType aRotateType,
                                      float aRotateAngle);
>>>>>>> upstream-releases

 private:
  // Private constructor: prevent instances beyond my singleton.
  constexpr SVGMotionSMILType() = default;
};

}  // namespace mozilla

#endif  // MOZILLA_SVGMOTIONSMILTYPE_H_
