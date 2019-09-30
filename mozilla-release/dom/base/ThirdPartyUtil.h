/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef ThirdPartyUtil_h__
#define ThirdPartyUtil_h__

#include "mozIThirdPartyUtil.h"
#include "mozilla/Attributes.h"
#include "mozilla/dom/Document.h"
#include "nsCOMPtr.h"
#include "nsEffectiveTLDService.h"
#include "nsString.h"
#include "nsPIDOMWindow.h"

class nsIURI;
class nsPIDOMWindowOuter;

<<<<<<< HEAD
class ThirdPartyUtil final : public mozIThirdPartyUtil {
 public:
  NS_DECL_ISUPPORTS
||||||| merged common ancestors
class ThirdPartyUtil final : public mozIThirdPartyUtil
{
public:
  NS_DECL_ISUPPORTS
=======
class ThirdPartyUtil final : public mozIThirdPartyUtil {
 public:
  NS_DECL_THREADSAFE_ISUPPORTS
>>>>>>> upstream-releases
  NS_DECL_MOZITHIRDPARTYUTIL

  nsresult Init();

<<<<<<< HEAD
 private:
  ~ThirdPartyUtil() {}
||||||| merged common ancestors
private:
  ~ThirdPartyUtil() {}
=======
  static void Startup();
  static ThirdPartyUtil* GetInstance();
>>>>>>> upstream-releases

 private:
  ~ThirdPartyUtil();

  bool IsThirdPartyInternal(const nsCString& aFirstDomain,
                            const nsCString& aSecondDomain) {
    // Check strict equality.
    return aFirstDomain != aSecondDomain;
  }
  nsresult IsThirdPartyInternal(const nsCString& aFirstDomain,
<<<<<<< HEAD
                                nsIURI* aSecondURI, bool* aResult);
||||||| merged common ancestors
    nsIURI* aSecondURI, bool* aResult);
=======
                                nsIURI* aSecondURI, bool* aResult);

  nsCString GetBaseDomainFromWindow(nsPIDOMWindowOuter* aWindow);
>>>>>>> upstream-releases

  RefPtr<nsEffectiveTLDService> mTLDService;
};

#endif
