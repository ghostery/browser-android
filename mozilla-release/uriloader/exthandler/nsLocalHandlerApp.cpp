/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim:expandtab:shiftwidth=2:tabstop=2:cin:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsLocalHandlerApp.h"
#include "nsIURI.h"
#include "nsIProcess.h"

// XXX why does nsMIMEInfoImpl have a threadsafe nsISupports?  do we need one
// here too?
NS_IMPL_ISUPPORTS(nsLocalHandlerApp, nsILocalHandlerApp, nsIHandlerApp)

////////////////////////////////////////////////////////////////////////////////
//// nsIHandlerApp

<<<<<<< HEAD
NS_IMETHODIMP nsLocalHandlerApp::GetName(nsAString &aName) {
||||||| merged common ancestors
NS_IMETHODIMP nsLocalHandlerApp::GetName(nsAString& aName)
{
=======
NS_IMETHODIMP nsLocalHandlerApp::GetName(nsAString& aName) {
>>>>>>> upstream-releases
  if (mName.IsEmpty() && mExecutable) {
    // Don't want to cache this, just in case someone resets the app
    // without changing the description....
    mExecutable->GetLeafName(aName);
  } else {
    aName.Assign(mName);
  }

  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP nsLocalHandlerApp::SetName(const nsAString &aName) {
||||||| merged common ancestors
NS_IMETHODIMP nsLocalHandlerApp::SetName(const nsAString & aName)
{
=======
NS_IMETHODIMP nsLocalHandlerApp::SetName(const nsAString& aName) {
>>>>>>> upstream-releases
  mName.Assign(aName);

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLocalHandlerApp::SetDetailedDescription(const nsAString &aDescription) {
||||||| merged common ancestors
nsLocalHandlerApp::SetDetailedDescription(const nsAString & aDescription)
{
=======
nsLocalHandlerApp::SetDetailedDescription(const nsAString& aDescription) {
>>>>>>> upstream-releases
  mDetailedDescription.Assign(aDescription);

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLocalHandlerApp::GetDetailedDescription(nsAString &aDescription) {
||||||| merged common ancestors
nsLocalHandlerApp::GetDetailedDescription(nsAString& aDescription)
{
=======
nsLocalHandlerApp::GetDetailedDescription(nsAString& aDescription) {
>>>>>>> upstream-releases
  aDescription.Assign(mDetailedDescription);

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLocalHandlerApp::Equals(nsIHandlerApp *aHandlerApp, bool *_retval) {
||||||| merged common ancestors
nsLocalHandlerApp::Equals(nsIHandlerApp *aHandlerApp, bool *_retval)
{
=======
nsLocalHandlerApp::Equals(nsIHandlerApp* aHandlerApp, bool* _retval) {
>>>>>>> upstream-releases
  NS_ENSURE_ARG_POINTER(aHandlerApp);

  *_retval = false;

  // If the handler app isn't a local handler app, then it's not the same app.
  nsCOMPtr<nsILocalHandlerApp> localHandlerApp = do_QueryInterface(aHandlerApp);
  if (!localHandlerApp) return NS_OK;

  // If either handler app doesn't have an executable, then they aren't
  // the same app.
  nsCOMPtr<nsIFile> executable;
  nsresult rv = localHandlerApp->GetExecutable(getter_AddRefs(executable));
  if (NS_FAILED(rv)) return rv;

  // Equality for two empty nsIHandlerApp
  if (!executable && !mExecutable) {
    *_retval = true;
    return NS_OK;
  }

  // At least one is set so they are not equal
  if (!mExecutable || !executable) return NS_OK;

  // Check the command line parameter list lengths
  uint32_t len;
  localHandlerApp->GetParameterCount(&len);
  if (mParameters.Length() != len) return NS_OK;

  // Check the command line params lists
  for (uint32_t idx = 0; idx < mParameters.Length(); idx++) {
    nsAutoString param;
    if (NS_FAILED(localHandlerApp->GetParameter(idx, param)) ||
        !param.Equals(mParameters[idx]))
      return NS_OK;
  }

  return executable->Equals(mExecutable, _retval);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLocalHandlerApp::LaunchWithURI(nsIURI *aURI,
                                 nsIInterfaceRequestor *aWindowContext) {
||||||| merged common ancestors
nsLocalHandlerApp::LaunchWithURI(nsIURI *aURI,
                                 nsIInterfaceRequestor *aWindowContext)
{
=======
nsLocalHandlerApp::LaunchWithURI(nsIURI* aURI,
                                 nsIInterfaceRequestor* aWindowContext) {
>>>>>>> upstream-releases
  // pass the entire URI to the handler.
  nsAutoCString spec;
  aURI->GetAsciiSpec(spec);
  return LaunchWithIProcess(spec);
}

<<<<<<< HEAD
nsresult nsLocalHandlerApp::LaunchWithIProcess(const nsCString &aArg) {
||||||| merged common ancestors
nsresult
nsLocalHandlerApp::LaunchWithIProcess(const nsCString& aArg)
{
=======
nsresult nsLocalHandlerApp::LaunchWithIProcess(const nsCString& aArg) {
>>>>>>> upstream-releases
  nsresult rv;
  nsCOMPtr<nsIProcess> process = do_CreateInstance(NS_PROCESS_CONTRACTID, &rv);
  if (NS_FAILED(rv)) return rv;

  if (NS_FAILED(rv = process->Init(mExecutable))) return rv;

  const char* string = aArg.get();

  return process->Run(false, &string, 1);
}

////////////////////////////////////////////////////////////////////////////////
//// nsILocalHandlerApp

NS_IMETHODIMP
<<<<<<< HEAD
nsLocalHandlerApp::GetExecutable(nsIFile **aExecutable) {
||||||| merged common ancestors
nsLocalHandlerApp::GetExecutable(nsIFile **aExecutable)
{
=======
nsLocalHandlerApp::GetExecutable(nsIFile** aExecutable) {
>>>>>>> upstream-releases
  NS_IF_ADDREF(*aExecutable = mExecutable);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLocalHandlerApp::SetExecutable(nsIFile *aExecutable) {
||||||| merged common ancestors
nsLocalHandlerApp::SetExecutable(nsIFile *aExecutable)
{
=======
nsLocalHandlerApp::SetExecutable(nsIFile* aExecutable) {
>>>>>>> upstream-releases
  mExecutable = aExecutable;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLocalHandlerApp::GetParameterCount(uint32_t *aParameterCount) {
||||||| merged common ancestors
nsLocalHandlerApp::GetParameterCount(uint32_t *aParameterCount)
{
=======
nsLocalHandlerApp::GetParameterCount(uint32_t* aParameterCount) {
>>>>>>> upstream-releases
  *aParameterCount = mParameters.Length();
  return NS_OK;
}

NS_IMETHODIMP
nsLocalHandlerApp::ClearParameters() {
  mParameters.Clear();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLocalHandlerApp::AppendParameter(const nsAString &aParam) {
||||||| merged common ancestors
nsLocalHandlerApp::AppendParameter(const nsAString & aParam)
{
=======
nsLocalHandlerApp::AppendParameter(const nsAString& aParam) {
>>>>>>> upstream-releases
  mParameters.AppendElement(aParam);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLocalHandlerApp::GetParameter(uint32_t parameterIndex, nsAString &_retval) {
  if (mParameters.Length() <= parameterIndex) return NS_ERROR_INVALID_ARG;
||||||| merged common ancestors
nsLocalHandlerApp::GetParameter(uint32_t parameterIndex, nsAString & _retval)
{
  if (mParameters.Length() <= parameterIndex)
    return NS_ERROR_INVALID_ARG;
=======
nsLocalHandlerApp::GetParameter(uint32_t parameterIndex, nsAString& _retval) {
  if (mParameters.Length() <= parameterIndex) return NS_ERROR_INVALID_ARG;
>>>>>>> upstream-releases

  _retval.Assign(mParameters[parameterIndex]);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsLocalHandlerApp::ParameterExists(const nsAString &aParam, bool *_retval) {
||||||| merged common ancestors
nsLocalHandlerApp::ParameterExists(const nsAString & aParam, bool *_retval)
{
=======
nsLocalHandlerApp::ParameterExists(const nsAString& aParam, bool* _retval) {
>>>>>>> upstream-releases
  *_retval = mParameters.Contains(aParam);
  return NS_OK;
}
