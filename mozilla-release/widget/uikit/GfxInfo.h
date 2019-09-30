/* vim: se cin sw=2 ts=2 et : */
/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __mozilla_widget_GfxInfo_h__
#define __mozilla_widget_GfxInfo_h__

#include "GfxInfoBase.h"
#include "GfxDriverInfo.h"

#include "nsString.h"
#include "mozilla/UniquePtr.h"

namespace mozilla {

namespace gl {
class GLContext;
}

namespace widget {

class GfxInfo : public GfxInfoBase {
 private:
  ~GfxInfo();

 public:
  GfxInfo();

  // We only declare the subset of nsIGfxInfo that we actually implement. The
  // rest is brought forward from GfxInfoBase.
<<<<<<< HEAD
  NS_IMETHOD GetD2DEnabled(bool *aD2DEnabled) override;
  NS_IMETHOD GetDWriteEnabled(bool *aDWriteEnabled) override;
  NS_IMETHOD GetDWriteVersion(nsAString &aDwriteVersion) override;
  NS_IMETHOD GetCleartypeParameters(nsAString &aCleartypeParams) override;
  NS_IMETHOD GetAdapterDescription(nsAString &aAdapterDescription) override;
  NS_IMETHOD GetAdapterDriver(nsAString &aAdapterDriver) override;
  NS_IMETHOD GetAdapterVendorID(nsAString &aAdapterVendorID) override;
  NS_IMETHOD GetAdapterDeviceID(nsAString &aAdapterDeviceID) override;
  NS_IMETHOD GetAdapterSubsysID(nsAString &aAdapterSubsysID) override;
  NS_IMETHOD GetAdapterRAM(nsAString &aAdapterRAM) override;
  NS_IMETHOD GetAdapterDriverVersion(nsAString &aAdapterDriverVersion) override;
  NS_IMETHOD GetAdapterDriverDate(nsAString &aAdapterDriverDate) override;
  NS_IMETHOD GetAdapterDescription2(nsAString &aAdapterDescription) override;
  NS_IMETHOD GetAdapterDriver2(nsAString &aAdapterDriver) override;
  NS_IMETHOD GetAdapterVendorID2(nsAString &aAdapterVendorID) override;
  NS_IMETHOD GetAdapterDeviceID2(nsAString &aAdapterDeviceID) override;
  NS_IMETHOD GetAdapterSubsysID2(nsAString &aAdapterSubsysID) override;
  NS_IMETHOD GetAdapterRAM2(nsAString &aAdapterRAM) override;
  NS_IMETHOD GetAdapterDriverVersion2(
      nsAString &aAdapterDriverVersion) override;
  NS_IMETHOD GetAdapterDriverDate2(nsAString &aAdapterDriverDate) override;
  NS_IMETHOD GetIsGPU2Active(bool *aIsGPU2Active) override;
||||||| merged common ancestors
  NS_IMETHOD GetD2DEnabled(bool *aD2DEnabled) override;
  NS_IMETHOD GetDWriteEnabled(bool *aDWriteEnabled) override;
  NS_IMETHOD GetDWriteVersion(nsAString & aDwriteVersion) override;
  NS_IMETHOD GetCleartypeParameters(nsAString & aCleartypeParams) override;
  NS_IMETHOD GetAdapterDescription(nsAString & aAdapterDescription) override;
  NS_IMETHOD GetAdapterDriver(nsAString & aAdapterDriver) override;
  NS_IMETHOD GetAdapterVendorID(nsAString & aAdapterVendorID) override;
  NS_IMETHOD GetAdapterDeviceID(nsAString & aAdapterDeviceID) override;
  NS_IMETHOD GetAdapterSubsysID(nsAString & aAdapterSubsysID) override;
  NS_IMETHOD GetAdapterRAM(nsAString & aAdapterRAM) override;
  NS_IMETHOD GetAdapterDriverVersion(nsAString & aAdapterDriverVersion) override;
  NS_IMETHOD GetAdapterDriverDate(nsAString & aAdapterDriverDate) override;
  NS_IMETHOD GetAdapterDescription2(nsAString & aAdapterDescription) override;
  NS_IMETHOD GetAdapterDriver2(nsAString & aAdapterDriver) override;
  NS_IMETHOD GetAdapterVendorID2(nsAString & aAdapterVendorID) override;
  NS_IMETHOD GetAdapterDeviceID2(nsAString & aAdapterDeviceID) override;
  NS_IMETHOD GetAdapterSubsysID2(nsAString & aAdapterSubsysID) override;
  NS_IMETHOD GetAdapterRAM2(nsAString & aAdapterRAM) override;
  NS_IMETHOD GetAdapterDriverVersion2(nsAString & aAdapterDriverVersion) override;
  NS_IMETHOD GetAdapterDriverDate2(nsAString & aAdapterDriverDate) override;
  NS_IMETHOD GetIsGPU2Active(bool *aIsGPU2Active) override;
=======
  NS_IMETHOD GetD2DEnabled(bool* aD2DEnabled) override;
  NS_IMETHOD GetDWriteEnabled(bool* aDWriteEnabled) override;
  NS_IMETHOD GetDWriteVersion(nsAString& aDwriteVersion) override;
  NS_IMETHOD GetCleartypeParameters(nsAString& aCleartypeParams) override;
  NS_IMETHOD GetAdapterDescription(nsAString& aAdapterDescription) override;
  NS_IMETHOD GetAdapterDriver(nsAString& aAdapterDriver) override;
  NS_IMETHOD GetAdapterVendorID(nsAString& aAdapterVendorID) override;
  NS_IMETHOD GetAdapterDeviceID(nsAString& aAdapterDeviceID) override;
  NS_IMETHOD GetAdapterSubsysID(nsAString& aAdapterSubsysID) override;
  NS_IMETHOD GetAdapterRAM(nsAString& aAdapterRAM) override;
  NS_IMETHOD GetAdapterDriverVersion(nsAString& aAdapterDriverVersion) override;
  NS_IMETHOD GetAdapterDriverDate(nsAString& aAdapterDriverDate) override;
  NS_IMETHOD GetAdapterDescription2(nsAString& aAdapterDescription) override;
  NS_IMETHOD GetAdapterDriver2(nsAString& aAdapterDriver) override;
  NS_IMETHOD GetAdapterVendorID2(nsAString& aAdapterVendorID) override;
  NS_IMETHOD GetAdapterDeviceID2(nsAString& aAdapterDeviceID) override;
  NS_IMETHOD GetAdapterSubsysID2(nsAString& aAdapterSubsysID) override;
  NS_IMETHOD GetAdapterRAM2(nsAString& aAdapterRAM) override;
  NS_IMETHOD GetAdapterDriverVersion2(
      nsAString& aAdapterDriverVersion) override;
  NS_IMETHOD GetAdapterDriverDate2(nsAString& aAdapterDriverDate) override;
  NS_IMETHOD GetIsGPU2Active(bool* aIsGPU2Active) override;
>>>>>>> upstream-releases
  using GfxInfoBase::GetFeatureStatus;
  using GfxInfoBase::GetFeatureSuggestedDriverVersion;

#ifdef DEBUG
  NS_DECL_ISUPPORTS_INHERITED
  NS_DECL_NSIGFXINFODEBUG
#endif

<<<<<<< HEAD
 protected:
  virtual nsresult GetFeatureStatusImpl(
      int32_t aFeature, int32_t *aStatus, nsAString &aSuggestedDriverVersion,
      const nsTArray<GfxDriverInfo> &aDriverInfo,
      OperatingSystem *aOS = nullptr);
  virtual const nsTArray<GfxDriverInfo> &GetGfxDriverInfo();
||||||| merged common ancestors
protected:

  virtual nsresult GetFeatureStatusImpl(int32_t aFeature,
                                        int32_t *aStatus,
                                        nsAString & aSuggestedDriverVersion,
                                        const nsTArray<GfxDriverInfo>& aDriverInfo,
                                        OperatingSystem* aOS = nullptr);
  virtual const nsTArray<GfxDriverInfo>& GetGfxDriverInfo();
=======
 protected:
  virtual nsresult GetFeatureStatusImpl(
      int32_t aFeature, int32_t* aStatus, nsAString& aSuggestedDriverVersion,
      const nsTArray<GfxDriverInfo>& aDriverInfo,
      OperatingSystem* aOS = nullptr);
  virtual const nsTArray<GfxDriverInfo>& GetGfxDriverInfo();
>>>>>>> upstream-releases
};

}  // namespace widget
}  // namespace mozilla

#endif /* __mozilla_widget_GfxInfo_h__ */
