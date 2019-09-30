/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_SMILIntegerType_h
#define mozilla_SMILIntegerType_h

#include "mozilla/Attributes.h"
#include "mozilla/SMILType.h"

namespace mozilla {

<<<<<<< HEAD
class SMILIntegerType : public nsISMILType {
 public:
  virtual void Init(nsSMILValue& aValue) const override;
  virtual void Destroy(nsSMILValue& aValue) const override;
  virtual nsresult Assign(nsSMILValue& aDest,
                          const nsSMILValue& aSrc) const override;
  virtual bool IsEqual(const nsSMILValue& aLeft,
                       const nsSMILValue& aRight) const override;
  virtual nsresult Add(nsSMILValue& aDest, const nsSMILValue& aValueToAdd,
||||||| merged common ancestors
class SMILIntegerType : public nsISMILType
{
public:
  virtual void Init(nsSMILValue& aValue) const override;
  virtual void Destroy(nsSMILValue& aValue) const override;
  virtual nsresult Assign(nsSMILValue& aDest, const nsSMILValue& aSrc) const override;
  virtual bool IsEqual(const nsSMILValue& aLeft,
                       const nsSMILValue& aRight) const override;
  virtual nsresult Add(nsSMILValue& aDest, const nsSMILValue& aValueToAdd,
=======
class SMILIntegerType : public SMILType {
 public:
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

  static SMILIntegerType* Singleton() {
||||||| merged common ancestors
  virtual nsresult Interpolate(const nsSMILValue& aStartVal,
                               const nsSMILValue& aEndVal,
                               double aUnitDistance,
                               nsSMILValue& aResult) const override;

  static SMILIntegerType*
  Singleton()
  {
=======
  virtual nsresult Interpolate(const SMILValue& aStartVal,
                               const SMILValue& aEndVal, double aUnitDistance,
                               SMILValue& aResult) const override;

  static SMILIntegerType* Singleton() {
>>>>>>> upstream-releases
    static SMILIntegerType sSingleton;
    return &sSingleton;
  }

<<<<<<< HEAD
 private:
  constexpr SMILIntegerType() {}
||||||| merged common ancestors
private:
  constexpr SMILIntegerType() {}
=======
 private:
  constexpr SMILIntegerType() = default;
>>>>>>> upstream-releases
};

}  // namespace mozilla

<<<<<<< HEAD
#endif  // MOZILLA_SMILINTEGERTYPE_H_
||||||| merged common ancestors
#endif // MOZILLA_SMILINTEGERTYPE_H_
=======
#endif  // mozilla_SMILIntegerType_h
>>>>>>> upstream-releases
