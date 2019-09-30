/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsNativeAppSupportBase.h"

nsNativeAppSupportBase::nsNativeAppSupportBase() {}

nsNativeAppSupportBase::~nsNativeAppSupportBase() {}

NS_IMPL_ISUPPORTS(nsNativeAppSupportBase, nsINativeAppSupport)

// Start answer defaults to OK.
NS_IMETHODIMP
<<<<<<< HEAD
nsNativeAppSupportBase::Start(bool *result) {
||||||| merged common ancestors
nsNativeAppSupportBase::Start( bool *result )
{
=======
nsNativeAppSupportBase::Start(bool* result) {
>>>>>>> upstream-releases
  *result = true;
  return NS_OK;
}

NS_IMETHODIMP
nsNativeAppSupportBase::Enable() { return NS_OK; }

NS_IMETHODIMP
<<<<<<< HEAD
nsNativeAppSupportBase::Stop(bool *result) {
  *result = true;
  return NS_OK;
}
||||||| merged common ancestors
nsNativeAppSupportBase::Stop( bool *result )
{
  *result = true;
  return NS_OK;
}
=======
nsNativeAppSupportBase::ReOpen() { return NS_OK; }
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
nsNativeAppSupportBase::Quit() { return NS_OK; }

NS_IMETHODIMP
nsNativeAppSupportBase::ReOpen() { return NS_OK; }

NS_IMETHODIMP
nsNativeAppSupportBase::OnLastWindowClosing() { return NS_OK; }
||||||| merged common ancestors
nsNativeAppSupportBase::Quit()
{
  return NS_OK;
}

NS_IMETHODIMP
nsNativeAppSupportBase::ReOpen()
{
  return NS_OK;
}

NS_IMETHODIMP
nsNativeAppSupportBase::OnLastWindowClosing() {
  return NS_OK;
}
=======
nsNativeAppSupportBase::OnLastWindowClosing() { return NS_OK; }
>>>>>>> upstream-releases
