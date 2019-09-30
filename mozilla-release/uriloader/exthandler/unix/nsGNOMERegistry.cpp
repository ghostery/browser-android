/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsGNOMERegistry.h"
#include "nsString.h"
#include "nsIComponentManager.h"
#include "nsMIMEInfoUnix.h"
#include "nsAutoPtr.h"
#include "nsIGIOService.h"

<<<<<<< HEAD
/* static */ bool nsGNOMERegistry::HandlerExists(const char* aProtocolScheme) {
||||||| merged common ancestors
/* static */ bool
nsGNOMERegistry::HandlerExists(const char *aProtocolScheme)
{
=======
/* static */
bool nsGNOMERegistry::HandlerExists(const char* aProtocolScheme) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIGIOService> giovfs = do_GetService(NS_GIOSERVICE_CONTRACTID);
  if (!giovfs) {
    return false;
  }

  nsCOMPtr<nsIHandlerApp> app;
  return NS_SUCCEEDED(giovfs->GetAppForURIScheme(
      nsDependentCString(aProtocolScheme), getter_AddRefs(app)));
}

// XXX Check HandlerExists() before calling LoadURL.

<<<<<<< HEAD
/* static */ nsresult nsGNOMERegistry::LoadURL(nsIURI* aURL) {
||||||| merged common ancestors
/* static */ nsresult
nsGNOMERegistry::LoadURL(nsIURI *aURL)
{
=======
/* static */
nsresult nsGNOMERegistry::LoadURL(nsIURI* aURL) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIGIOService> giovfs = do_GetService(NS_GIOSERVICE_CONTRACTID);
  if (!giovfs) {
    return NS_ERROR_FAILURE;
  }

  return giovfs->ShowURI(aURL);
}

<<<<<<< HEAD
/* static */ void nsGNOMERegistry::GetAppDescForScheme(
    const nsACString& aScheme, nsAString& aDesc) {
||||||| merged common ancestors
/* static */ void
nsGNOMERegistry::GetAppDescForScheme(const nsACString& aScheme,
                                     nsAString& aDesc)
{
=======
/* static */
void nsGNOMERegistry::GetAppDescForScheme(const nsACString& aScheme,
                                          nsAString& aDesc) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIGIOService> giovfs = do_GetService(NS_GIOSERVICE_CONTRACTID);
  if (!giovfs) return;

  nsCOMPtr<nsIHandlerApp> app;
  if (NS_FAILED(giovfs->GetAppForURIScheme(aScheme, getter_AddRefs(app))))
    return;

  app->GetName(aDesc);
}

<<<<<<< HEAD
/* static */ already_AddRefed<nsMIMEInfoBase> nsGNOMERegistry::GetFromExtension(
    const nsACString& aFileExt) {
||||||| merged common ancestors

/* static */ already_AddRefed<nsMIMEInfoBase>
nsGNOMERegistry::GetFromExtension(const nsACString& aFileExt)
{
=======
/* static */
already_AddRefed<nsMIMEInfoBase> nsGNOMERegistry::GetFromExtension(
    const nsACString& aFileExt) {
>>>>>>> upstream-releases
  nsAutoCString mimeType;
  nsCOMPtr<nsIGIOService> giovfs = do_GetService(NS_GIOSERVICE_CONTRACTID);
  if (!giovfs) {
    return nullptr;
  }

  // Get the MIME type from the extension, then call GetFromType to
  // fill in the MIMEInfo.
  if (NS_FAILED(giovfs->GetMimeTypeFromExtension(aFileExt, mimeType)) ||
      mimeType.EqualsLiteral("application/octet-stream")) {
    return nullptr;
  }

  RefPtr<nsMIMEInfoBase> mi = GetFromType(mimeType);
  if (mi) {
    mi->AppendExtension(aFileExt);
  }

  return mi.forget();
}

<<<<<<< HEAD
/* static */ already_AddRefed<nsMIMEInfoBase> nsGNOMERegistry::GetFromType(
    const nsACString& aMIMEType) {
||||||| merged common ancestors
/* static */ already_AddRefed<nsMIMEInfoBase>
nsGNOMERegistry::GetFromType(const nsACString& aMIMEType)
{
=======
/* static */
already_AddRefed<nsMIMEInfoBase> nsGNOMERegistry::GetFromType(
    const nsACString& aMIMEType) {
>>>>>>> upstream-releases
  RefPtr<nsMIMEInfoUnix> mimeInfo = new nsMIMEInfoUnix(aMIMEType);
  NS_ENSURE_TRUE(mimeInfo, nullptr);

  nsAutoString name;
  nsAutoCString description;

  nsCOMPtr<nsIGIOService> giovfs = do_GetService(NS_GIOSERVICE_CONTRACTID);
  if (!giovfs) {
    return nullptr;
  }

  nsCOMPtr<nsIHandlerApp> handlerApp;
  if (NS_FAILED(
          giovfs->GetAppForMimeType(aMIMEType, getter_AddRefs(handlerApp))) ||
      !handlerApp) {
    return nullptr;
  }
  handlerApp->GetName(name);
  giovfs->GetDescriptionForMimeType(aMIMEType, description);

  mimeInfo->SetDefaultDescription(name);
  mimeInfo->SetPreferredAction(nsIMIMEInfo::useSystemDefault);
  mimeInfo->SetDescription(NS_ConvertUTF8toUTF16(description));

  return mimeInfo.forget();
}
