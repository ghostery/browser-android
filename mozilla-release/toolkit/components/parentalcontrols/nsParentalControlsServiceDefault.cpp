/* -*- Mode: C++; tab-width: 2; indent-tabs-mode:nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsParentalControlsService.h"
#include "nsString.h"
#include "nsIFile.h"
#include "mozilla/Unused.h"

NS_IMPL_ISUPPORTS(nsParentalControlsService, nsIParentalControlsService)

nsParentalControlsService::nsParentalControlsService() : mEnabled(false) {
  mozilla::Unused << mEnabled;
}

nsParentalControlsService::~nsParentalControlsService() = default;

NS_IMETHODIMP
<<<<<<< HEAD
nsParentalControlsService::GetParentalControlsEnabled(bool *aResult) {
||||||| merged common ancestors
nsParentalControlsService::GetParentalControlsEnabled(bool *aResult)
{
=======
nsParentalControlsService::GetParentalControlsEnabled(bool* aResult) {
>>>>>>> upstream-releases
  *aResult = false;
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
  return NS_ERROR_NOT_AVAILABLE;
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
  return NS_ERROR_NOT_AVAILABLE;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsParentalControlsService::Log(int16_t aEntryType, bool blocked,
                               nsIURI *aSource, nsIFile *aTarget) {
||||||| merged common ancestors
nsParentalControlsService::Log(int16_t aEntryType,
                               bool blocked,
                               nsIURI *aSource,
                               nsIFile *aTarget)
{
=======
nsParentalControlsService::Log(int16_t aEntryType, bool blocked,
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
  return NS_ERROR_NOT_AVAILABLE;
}
