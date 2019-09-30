/* -*- Mode: C++; tab-width: 2; indent-tabs-mode:nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsParentalControlsService.h"
#include "nsString.h"
#include "nsIFile.h"
#include "FennecJNIWrappers.h"

namespace java = mozilla::java;

NS_IMPL_ISUPPORTS(nsParentalControlsService, nsIParentalControlsService)

nsParentalControlsService::nsParentalControlsService() : mEnabled(false) {
  if (mozilla::jni::IsFennec()) {
    mEnabled = java::Restrictions::IsUserRestricted();
  }
}

nsParentalControlsService::~nsParentalControlsService() {}

NS_IMETHODIMP
<<<<<<< HEAD
nsParentalControlsService::GetParentalControlsEnabled(bool *aResult) {
||||||| merged common ancestors
nsParentalControlsService::GetParentalControlsEnabled(bool *aResult)
{
=======
nsParentalControlsService::GetParentalControlsEnabled(bool* aResult) {
>>>>>>> upstream-releases
  *aResult = mEnabled;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsParentalControlsService::GetBlockFileDownloadsEnabled(bool *aResult) {
||||||| merged common ancestors
nsParentalControlsService::GetBlockFileDownloadsEnabled(bool *aResult)
{
=======
nsParentalControlsService::GetBlockFileDownloadsEnabled(bool* aResult) {
>>>>>>> upstream-releases
  // NOTE: isAllowed returns the opposite intention, so we need to flip it
  bool res;
  IsAllowed(nsIParentalControlsService::DOWNLOAD, NULL, &res);
  *aResult = !res;

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsParentalControlsService::GetLoggingEnabled(bool *aResult) {
||||||| merged common ancestors
nsParentalControlsService::GetLoggingEnabled(bool *aResult)
{
=======
nsParentalControlsService::GetLoggingEnabled(bool* aResult) {
>>>>>>> upstream-releases
  // Android doesn't currently have any method of logging restricted actions.
  *aResult = false;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsParentalControlsService::Log(int16_t aEntryType, bool aBlocked,
                               nsIURI *aSource, nsIFile *aTarget) {
||||||| merged common ancestors
nsParentalControlsService::Log(int16_t aEntryType,
                               bool aBlocked,
                               nsIURI *aSource,
                               nsIFile *aTarget)
{
=======
nsParentalControlsService::Log(int16_t aEntryType, bool aBlocked,
                               nsIURI* aSource, nsIFile* aTarget) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_AVAILABLE;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsParentalControlsService::RequestURIOverride(
    nsIURI *aTarget, nsIInterfaceRequestor *aWindowContext, bool *_retval) {
||||||| merged common ancestors
nsParentalControlsService::RequestURIOverride(nsIURI *aTarget,
                                              nsIInterfaceRequestor *aWindowContext,
                                              bool *_retval)
{
=======
nsParentalControlsService::RequestURIOverride(
    nsIURI* aTarget, nsIInterfaceRequestor* aWindowContext, bool* _retval) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_AVAILABLE;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsParentalControlsService::RequestURIOverrides(
    nsIArray *aTargets, nsIInterfaceRequestor *aWindowContext, bool *_retval) {
||||||| merged common ancestors
nsParentalControlsService::RequestURIOverrides(nsIArray *aTargets,
                                               nsIInterfaceRequestor *aWindowContext,
                                               bool *_retval)
{
=======
nsParentalControlsService::RequestURIOverrides(
    nsIArray* aTargets, nsIInterfaceRequestor* aWindowContext, bool* _retval) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_AVAILABLE;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsParentalControlsService::IsAllowed(int16_t aAction, nsIURI *aUri,
                                     bool *_retval) {
||||||| merged common ancestors
nsParentalControlsService::IsAllowed(int16_t aAction,
                                     nsIURI *aUri,
                                     bool *_retval)
{
=======
nsParentalControlsService::IsAllowed(int16_t aAction, nsIURI* aUri,
                                     bool* _retval) {
>>>>>>> upstream-releases
  nsresult rv = NS_OK;
  *_retval = true;

  if (!mEnabled) {
    return rv;
  }

  if (mozilla::jni::IsFennec()) {
    nsAutoCString url;
    if (aUri) {
      rv = aUri->GetSpec(url);
      NS_ENSURE_SUCCESS(rv, rv);
    }

    *_retval =
        java::Restrictions::IsAllowed(aAction, NS_ConvertUTF8toUTF16(url));
    return rv;
  }

  return NS_ERROR_NOT_AVAILABLE;
}
