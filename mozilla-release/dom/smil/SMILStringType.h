/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_SMILStringType_h
#define mozilla_SMILStringType_h

#include "mozilla/Attributes.h"
#include "mozilla/SMILType.h"

namespace mozilla {

<<<<<<< HEAD
class SMILStringType : public nsISMILType {
 public:
  // Singleton for nsSMILValue objects to hold onto.
  static SMILStringType* Singleton() {
||||||| merged common ancestors
class SMILStringType : public nsISMILType
{
public:
  // Singleton for nsSMILValue objects to hold onto.
  static SMILStringType*
  Singleton()
  {
=======
class SMILStringType : public SMILType {
 public:
  // Singleton for SMILValue objects to hold onto.
  static SMILStringType* Singleton() {
>>>>>>> upstream-releases
    static SMILStringType sSingleton;
    return &sSingleton;
  }

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
  virtual void Init(nsSMILValue& aValue) const override;
  virtual void Destroy(nsSMILValue& aValue) const override;
  virtual nsresult Assign(nsSMILValue& aDest, const nsSMILValue& aSrc) const override;
  virtual bool IsEqual(const nsSMILValue& aLeft,
                       const nsSMILValue& aRight) const override;
  virtual nsresult Add(nsSMILValue& aDest, const nsSMILValue& aValueToAdd,
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
  virtual nsresult ComputeDistance(const SMILValue& aFrom, const SMILValue& aTo,
                                   double& aDistance) const override;
<<<<<<< HEAD
  virtual nsresult Interpolate(const nsSMILValue& aStartVal,
                               const nsSMILValue& aEndVal, double aUnitDistance,
                               nsSMILValue& aResult) const override;
||||||| merged common ancestors
  virtual nsresult Interpolate(const nsSMILValue& aStartVal,
                               const nsSMILValue& aEndVal,
                               double aUnitDistance,
                               nsSMILValue& aResult) const override;
=======
  virtual nsresult Interpolate(const SMILValue& aStartVal,
                               const SMILValue& aEndVal, double aUnitDistance,
                               SMILValue& aResult) const override;
>>>>>>> upstream-releases

 private:
  // Private constructor: prevent instances beyond my singleton.
  constexpr SMILStringType() = default;
};

}  // namespace mozilla

<<<<<<< HEAD
#endif  // MOZILLA_SMILSTRINGTYPE_H_
||||||| merged common ancestors
#endif // MOZILLA_SMILSTRINGTYPE_H_
=======
#endif  // mozilla_SMILStringType_h
>>>>>>> upstream-releases
