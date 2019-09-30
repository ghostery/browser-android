/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdlib.h>
#include <stdarg.h>

#include "nsIServiceManager.h"

#include "nsIConsoleService.h"
#include "nsICanvasRenderingContextInternal.h"
#include "nsIHTMLCollection.h"
#include "mozilla/dom/HTMLCanvasElement.h"
#include "mozilla/dom/BrowserChild.h"
#include "mozilla/EventStateManager.h"
#include "mozilla/StaticPrefs.h"
#include "nsIPrincipal.h"

#include "nsGfxCIID.h"

#include "nsTArray.h"

#include "CanvasUtils.h"
#include "mozilla/gfx/Matrix.h"
#include "WebGL2Context.h"

#include "nsIScriptError.h"
#include "nsIScriptObjectPrincipal.h"
#include "nsIPermissionManager.h"
#include "nsIObserverService.h"
#include "mozilla/Services.h"
#include "mozIThirdPartyUtil.h"
#include "nsContentUtils.h"
#include "nsUnicharUtils.h"
#include "nsPrintfCString.h"
#include "nsIConsoleService.h"
#include "jsapi.h"

#define TOPIC_CANVAS_PERMISSIONS_PROMPT "canvas-permissions-prompt"
#define TOPIC_CANVAS_PERMISSIONS_PROMPT_HIDE_DOORHANGER \
  "canvas-permissions-prompt-hide-doorhanger"
#define PERMISSION_CANVAS_EXTRACT_DATA NS_LITERAL_CSTRING("canvas")

using namespace mozilla::gfx;

namespace mozilla {
namespace CanvasUtils {

<<<<<<< HEAD
bool IsImageExtractionAllowed(nsIDocument* aDocument, JSContext* aCx,
                              nsIPrincipal& aPrincipal) {
  // Do the rest of the checks only if privacy.resistFingerprinting is on.
  if (!nsContentUtils::ShouldResistFingerprinting()) {
    return true;
  }
||||||| merged common ancestors
bool IsImageExtractionAllowed(nsIDocument *aDocument, JSContext *aCx, nsIPrincipal& aPrincipal)
{
    // Do the rest of the checks only if privacy.resistFingerprinting is on.
    if (!nsContentUtils::ShouldResistFingerprinting()) {
        return true;
    }
=======
bool IsImageExtractionAllowed(Document* aDocument, JSContext* aCx,
                              nsIPrincipal& aPrincipal) {
  // Do the rest of the checks only if privacy.resistFingerprinting is on.
  if (!nsContentUtils::ShouldResistFingerprinting(aDocument)) {
    return true;
  }
>>>>>>> upstream-releases

  // Don't proceed if we don't have a document or JavaScript context.
  if (!aDocument || !aCx) {
    return false;
  }

  // The system principal can always extract canvas data.
  if (nsContentUtils::IsSystemPrincipal(&aPrincipal)) {
    return true;
  }

  // Allow extension principals.
  auto principal = BasePrincipal::Cast(&aPrincipal);
  if (principal->AddonPolicy() || principal->ContentScriptAddonPolicy()) {
    return true;
  }

  // Get the document URI and its spec.
  nsIURI* docURI = aDocument->GetDocumentURI();
  nsCString docURISpec;
  docURI->GetSpec(docURISpec);

  // Allow local files to extract canvas data.
  bool isFileURL;
  if (NS_SUCCEEDED(docURI->SchemeIs("file", &isFileURL)) && isFileURL) {
    return true;
  }

<<<<<<< HEAD
  // Get calling script file and line for logging.
  JS::AutoFilename scriptFile;
  unsigned scriptLine = 0;
  bool isScriptKnown = false;
  if (JS::DescribeScriptedCaller(aCx, &scriptFile, &scriptLine)) {
    isScriptKnown = true;
    // Don't show canvas prompt for PDF.js
    if (scriptFile.get() &&
        strcmp(scriptFile.get(), "resource://pdf.js/build/pdf.js") == 0) {
      return true;
    }
  }
||||||| merged common ancestors
    // Get calling script file and line for logging.
    JS::AutoFilename scriptFile;
    unsigned scriptLine = 0;
    bool isScriptKnown = false;
    if (JS::DescribeScriptedCaller(aCx, &scriptFile, &scriptLine)) {
        isScriptKnown = true;
        // Don't show canvas prompt for PDF.js
        if (scriptFile.get() &&
                strcmp(scriptFile.get(), "resource://pdf.js/build/pdf.js") == 0) {
            return true;
        }
    }
=======
  // Don't show canvas prompt for PDF.js
  JS::AutoFilename scriptFile;
  if (JS::DescribeScriptedCaller(aCx, &scriptFile) && scriptFile.get() &&
      strcmp(scriptFile.get(), "resource://pdf.js/build/pdf.js") == 0) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsIDocument* topLevelDocument = aDocument->GetTopLevelContentDocument();
  nsIURI* topLevelDocURI =
      topLevelDocument ? topLevelDocument->GetDocumentURI() : nullptr;
  nsCString topLevelDocURISpec;
  if (topLevelDocURI) {
    topLevelDocURI->GetSpec(topLevelDocURISpec);
  }
||||||| merged common ancestors
    nsIDocument* topLevelDocument = aDocument->GetTopLevelContentDocument();
    nsIURI *topLevelDocURI = topLevelDocument ? topLevelDocument->GetDocumentURI() : nullptr;
    nsCString topLevelDocURISpec;
    if (topLevelDocURI) {
        topLevelDocURI->GetSpec(topLevelDocURISpec);
    }
=======
  Document* topLevelDocument = aDocument->GetTopLevelContentDocument();
  nsIURI* topLevelDocURI =
      topLevelDocument ? topLevelDocument->GetDocumentURI() : nullptr;
  nsCString topLevelDocURISpec;
  if (topLevelDocURI) {
    topLevelDocURI->GetSpec(topLevelDocURISpec);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load Third Party Util service.
  nsresult rv;
  nsCOMPtr<mozIThirdPartyUtil> thirdPartyUtil =
      do_GetService(THIRDPARTYUTIL_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, false);

  // Block all third-party attempts to extract canvas.
  bool isThirdParty = true;
  rv = thirdPartyUtil->IsThirdPartyURI(topLevelDocURI, docURI, &isThirdParty);
  NS_ENSURE_SUCCESS(rv, false);
  if (isThirdParty) {
    nsAutoCString message;
    message.AppendPrintf(
        "Blocked third party %s in page %s from extracting canvas data.",
        docURISpec.get(), topLevelDocURISpec.get());
    if (isScriptKnown) {
      message.AppendPrintf(" %s:%u.", scriptFile.get(), scriptLine);
    }
    nsContentUtils::LogMessageToConsole(message.get());
    return false;
  }
||||||| merged common ancestors
    // Load Third Party Util service.
    nsresult rv;
    nsCOMPtr<mozIThirdPartyUtil> thirdPartyUtil =
        do_GetService(THIRDPARTYUTIL_CONTRACTID, &rv);
    NS_ENSURE_SUCCESS(rv, false);

    // Block all third-party attempts to extract canvas.
    bool isThirdParty = true;
    rv = thirdPartyUtil->IsThirdPartyURI(topLevelDocURI, docURI, &isThirdParty);
    NS_ENSURE_SUCCESS(rv, false);
    if (isThirdParty) {
        nsAutoCString message;
        message.AppendPrintf("Blocked third party %s in page %s from extracting canvas data.",
                             docURISpec.get(), topLevelDocURISpec.get());
        if (isScriptKnown) {
            message.AppendPrintf(" %s:%u.", scriptFile.get(), scriptLine);
        }
        nsContentUtils::LogMessageToConsole(message.get());
        return false;
    }
=======
  // Load Third Party Util service.
  nsresult rv;
  nsCOMPtr<mozIThirdPartyUtil> thirdPartyUtil =
      do_GetService(THIRDPARTYUTIL_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, false);

  // Block all third-party attempts to extract canvas.
  bool isThirdParty = true;
  rv = thirdPartyUtil->IsThirdPartyURI(topLevelDocURI, docURI, &isThirdParty);
  NS_ENSURE_SUCCESS(rv, false);
  if (isThirdParty) {
    nsAutoString message;
    message.AppendPrintf("Blocked third party %s from extracting canvas data.",
                         docURISpec.get());
    nsContentUtils::ReportToConsoleNonLocalized(
        message, nsIScriptError::warningFlag, NS_LITERAL_CSTRING("Security"),
        aDocument);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load Permission Manager service.
  nsCOMPtr<nsIPermissionManager> permissionManager =
      do_GetService(NS_PERMISSIONMANAGER_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, false);

  // Check if the site has permission to extract canvas data.
  // Either permit or block extraction if a stored permission setting exists.
  uint32_t permission;
  rv = permissionManager->TestPermission(
      topLevelDocURI, PERMISSION_CANVAS_EXTRACT_DATA, &permission);
  NS_ENSURE_SUCCESS(rv, false);
  switch (permission) {
||||||| merged common ancestors
    // Load Permission Manager service.
    nsCOMPtr<nsIPermissionManager> permissionManager =
        do_GetService(NS_PERMISSIONMANAGER_CONTRACTID, &rv);
    NS_ENSURE_SUCCESS(rv, false);

    // Check if the site has permission to extract canvas data.
    // Either permit or block extraction if a stored permission setting exists.
    uint32_t permission;
    rv = permissionManager->TestPermission(topLevelDocURI,
                                           PERMISSION_CANVAS_EXTRACT_DATA,
                                           &permission);
    NS_ENSURE_SUCCESS(rv, false);
    switch (permission) {
=======
  // Load Permission Manager service.
  nsCOMPtr<nsIPermissionManager> permissionManager =
      do_GetService(NS_PERMISSIONMANAGER_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, false);

  // Check if the site has permission to extract canvas data.
  // Either permit or block extraction if a stored permission setting exists.
  uint32_t permission;
  rv = permissionManager->TestPermissionFromPrincipal(
      principal, PERMISSION_CANVAS_EXTRACT_DATA, &permission);
  NS_ENSURE_SUCCESS(rv, false);
  switch (permission) {
>>>>>>> upstream-releases
    case nsIPermissionManager::ALLOW_ACTION:
      return true;
    case nsIPermissionManager::DENY_ACTION:
      return false;
    default:
<<<<<<< HEAD
      break;
  }
||||||| merged common ancestors
        break;
    }

    // At this point, permission is unknown (nsIPermissionManager::UNKNOWN_ACTION).

    // Check if the request is in response to user input
    if (StaticPrefs::privacy_resistFingerprinting_autoDeclineNoUserInputCanvasPrompts() &&
        !EventStateManager::IsHandlingUserInput()) {
        nsAutoCString message;
        message.AppendPrintf("Blocked %s in page %s from extracting canvas data because no user input was detected.",
                             docURISpec.get(), topLevelDocURISpec.get());
        if (isScriptKnown) {
            message.AppendPrintf(" %s:%u.", scriptFile.get(), scriptLine);
        }
        nsContentUtils::LogMessageToConsole(message.get());
=======
      break;
  }

  // At this point, permission is unknown
  // (nsIPermissionManager::UNKNOWN_ACTION).

  // Check if the request is in response to user input
  bool isAutoBlockCanvas =
      StaticPrefs::
          privacy_resistFingerprinting_autoDeclineNoUserInputCanvasPrompts() &&
      !EventStateManager::IsHandlingUserInput();

  if (isAutoBlockCanvas) {
    nsAutoString message;
    message.AppendPrintf(
        "Blocked %s from extracting canvas data because no user input was "
        "detected.",
        docURISpec.get());
    nsContentUtils::ReportToConsoleNonLocalized(
        message, nsIScriptError::warningFlag, NS_LITERAL_CSTRING("Security"),
        aDocument);
  } else {
    // It was in response to user input, so log and display the prompt.
    nsAutoString message;
    message.AppendPrintf(
        "Blocked %s from extracting canvas data, but prompting the user.",
        docURISpec.get());
    nsContentUtils::ReportToConsoleNonLocalized(
        message, nsIScriptError::warningFlag, NS_LITERAL_CSTRING("Security"),
        aDocument);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // At this point, permission is unknown
  // (nsIPermissionManager::UNKNOWN_ACTION).
||||||| merged common ancestors
        return false;
    }
=======
  // Prompt the user (asynchronous).
  nsPIDOMWindowOuter* win = aDocument->GetWindow();
  nsAutoCString origin;
  rv = principal->GetOrigin(origin);
  NS_ENSURE_SUCCESS(rv, false);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Check if the request is in response to user input
  if (StaticPrefs::
          privacy_resistFingerprinting_autoDeclineNoUserInputCanvasPrompts() &&
      !EventStateManager::IsHandlingUserInput()) {
    nsAutoCString message;
    message.AppendPrintf(
        "Blocked %s in page %s from extracting canvas data because no user "
        "input was detected.",
        docURISpec.get(), topLevelDocURISpec.get());
    if (isScriptKnown) {
      message.AppendPrintf(" %s:%u.", scriptFile.get(), scriptLine);
||||||| merged common ancestors
    // It was in response to user input, so log and display the prompt.
    nsAutoCString message;
    message.AppendPrintf("Blocked %s in page %s from extracting canvas data, but prompting the user.",
                         docURISpec.get(), topLevelDocURISpec.get());
    if (isScriptKnown) {
        message.AppendPrintf(" %s:%u.", scriptFile.get(), scriptLine);
=======
  if (XRE_IsContentProcess()) {
    BrowserChild* browserChild = BrowserChild::GetFrom(win);
    if (browserChild) {
      browserChild->SendShowCanvasPermissionPrompt(origin, isAutoBlockCanvas);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    nsContentUtils::LogMessageToConsole(message.get());

    return false;
  }

  // It was in response to user input, so log and display the prompt.
  nsAutoCString message;
  message.AppendPrintf(
      "Blocked %s in page %s from extracting canvas data, but prompting the "
      "user.",
      docURISpec.get(), topLevelDocURISpec.get());
  if (isScriptKnown) {
    message.AppendPrintf(" %s:%u.", scriptFile.get(), scriptLine);
  }
  nsContentUtils::LogMessageToConsole(message.get());

  // Prompt the user (asynchronous).
  nsPIDOMWindowOuter* win = aDocument->GetWindow();
  if (XRE_IsContentProcess()) {
    TabChild* tabChild = TabChild::GetFrom(win);
    if (tabChild) {
      tabChild->SendShowCanvasPermissionPrompt(topLevelDocURISpec);
    }
  } else {
    nsCOMPtr<nsIObserverService> obs = mozilla::services::GetObserverService();
    if (obs) {
      obs->NotifyObservers(win, TOPIC_CANVAS_PERMISSIONS_PROMPT,
                           NS_ConvertUTF8toUTF16(topLevelDocURISpec).get());
||||||| merged common ancestors
    nsContentUtils::LogMessageToConsole(message.get());

    // Prompt the user (asynchronous).
    nsPIDOMWindowOuter *win = aDocument->GetWindow();
    if (XRE_IsContentProcess()) {
        TabChild* tabChild = TabChild::GetFrom(win);
        if (tabChild) {
            tabChild->SendShowCanvasPermissionPrompt(topLevelDocURISpec);
        }
    } else {
        nsCOMPtr<nsIObserverService> obs = mozilla::services::GetObserverService();
        if (obs) {
            obs->NotifyObservers(win, TOPIC_CANVAS_PERMISSIONS_PROMPT,
                                 NS_ConvertUTF8toUTF16(topLevelDocURISpec).get());
        }
=======
  } else {
    nsCOMPtr<nsIObserverService> obs = mozilla::services::GetObserverService();
    if (obs) {
      obs->NotifyObservers(win,
                           isAutoBlockCanvas
                               ? TOPIC_CANVAS_PERMISSIONS_PROMPT_HIDE_DOORHANGER
                               : TOPIC_CANVAS_PERMISSIONS_PROMPT,
                           NS_ConvertUTF8toUTF16(origin).get());
>>>>>>> upstream-releases
    }
  }

  // We don't extract the image for now -- user may override at prompt.
  return false;
}

bool GetCanvasContextType(const nsAString& str,
                          dom::CanvasContextType* const out_type) {
  if (str.EqualsLiteral("2d")) {
    *out_type = dom::CanvasContextType::Canvas2D;
    return true;
  }

  if (str.EqualsLiteral("webgl") || str.EqualsLiteral("experimental-webgl")) {
    *out_type = dom::CanvasContextType::WebGL1;
    return true;
  }

  if (WebGL2Context::IsSupported()) {
    if (str.EqualsLiteral("webgl2")) {
      *out_type = dom::CanvasContextType::WebGL2;
      return true;
    }
  }

  if (str.EqualsLiteral("bitmaprenderer")) {
    *out_type = dom::CanvasContextType::ImageBitmap;
    return true;
  }

  return false;
}

/**
 * This security check utility might be called from an source that never taints
 * others. For example, while painting a CanvasPattern, which is created from an
 * ImageBitmap, onto a canvas. In this case, the caller could set the CORSUsed
 * true in order to pass this check and leave the aPrincipal to be a nullptr
 * since the aPrincipal is not going to be used.
 */
<<<<<<< HEAD
void DoDrawImageSecurityCheck(dom::HTMLCanvasElement* aCanvasElement,
                              nsIPrincipal* aPrincipal, bool forceWriteOnly,
                              bool CORSUsed) {
  // Callers should ensure that mCanvasElement is non-null before calling this
  if (!aCanvasElement) {
    NS_WARNING("DoDrawImageSecurityCheck called without canvas element!");
    return;
  }
||||||| merged common ancestors
void
DoDrawImageSecurityCheck(dom::HTMLCanvasElement *aCanvasElement,
                         nsIPrincipal *aPrincipal,
                         bool forceWriteOnly,
                         bool CORSUsed)
{
    // Callers should ensure that mCanvasElement is non-null before calling this
    if (!aCanvasElement) {
        NS_WARNING("DoDrawImageSecurityCheck called without canvas element!");
        return;
    }

    if (aCanvasElement->IsWriteOnly() && !aCanvasElement->mExpandedReader) {
        return;
    }
=======
void DoDrawImageSecurityCheck(dom::HTMLCanvasElement* aCanvasElement,
                              nsIPrincipal* aPrincipal, bool forceWriteOnly,
                              bool CORSUsed) {
  // Callers should ensure that mCanvasElement is non-null before calling this
  if (!aCanvasElement) {
    NS_WARNING("DoDrawImageSecurityCheck called without canvas element!");
    return;
  }

  if (aCanvasElement->IsWriteOnly() && !aCanvasElement->mExpandedReader) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (aCanvasElement->IsWriteOnly() && !aCanvasElement->mExpandedReader) {
    return;
  }
||||||| merged common ancestors
    // If we explicitly set WriteOnly just do it and get out
    if (forceWriteOnly) {
        aCanvasElement->SetWriteOnly();
        return;
    }
=======
  // If we explicitly set WriteOnly just do it and get out
  if (forceWriteOnly) {
    aCanvasElement->SetWriteOnly();
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If we explicitly set WriteOnly just do it and get out
  if (forceWriteOnly) {
    aCanvasElement->SetWriteOnly();
    return;
  }
||||||| merged common ancestors
    // No need to do a security check if the image used CORS for the load
    if (CORSUsed)
        return;
=======
  // No need to do a security check if the image used CORS for the load
  if (CORSUsed) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // No need to do a security check if the image used CORS for the load
  if (CORSUsed) return;
||||||| merged common ancestors
    MOZ_ASSERT(aPrincipal, "Must have a principal here");
=======
  MOZ_ASSERT(aPrincipal, "Must have a principal here");
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(aPrincipal, "Must have a principal here");
||||||| merged common ancestors
    if (aCanvasElement->NodePrincipal()->Subsumes(aPrincipal)) {
        // This canvas has access to that image anyway
        return;
    }
=======
  if (aCanvasElement->NodePrincipal()->Subsumes(aPrincipal)) {
    // This canvas has access to that image anyway
    return;
  }

  if (BasePrincipal::Cast(aPrincipal)->AddonPolicy()) {
    // This is a resource from an extension content script principal.

    if (aCanvasElement->mExpandedReader &&
        aCanvasElement->mExpandedReader->Subsumes(aPrincipal)) {
      // This canvas already allows reading from this principal.
      return;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (aCanvasElement->NodePrincipal()->Subsumes(aPrincipal)) {
    // This canvas has access to that image anyway
    return;
  }
||||||| merged common ancestors
    if (BasePrincipal::Cast(aPrincipal)->AddonPolicy()) {
        // This is a resource from an extension content script principal.
=======
    if (!aCanvasElement->mExpandedReader) {
      // Allow future reads from this same princial only.
      aCanvasElement->SetWriteOnly(aPrincipal);
      return;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (BasePrincipal::Cast(aPrincipal)->AddonPolicy()) {
    // This is a resource from an extension content script principal.
||||||| merged common ancestors
        if (aCanvasElement->mExpandedReader &&
            aCanvasElement->mExpandedReader->Subsumes(aPrincipal)) {
            // This canvas already allows reading from this principal.
            return;
        }
=======
    // If we got here, this must be the *second* extension tainting
    // the canvas.  Fall through to mark it WriteOnly for everyone.
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (aCanvasElement->mExpandedReader &&
        aCanvasElement->mExpandedReader->Subsumes(aPrincipal)) {
      // This canvas already allows reading from this principal.
      return;
    }
||||||| merged common ancestors
        if (!aCanvasElement->mExpandedReader) {
            // Allow future reads from this same princial only.
            aCanvasElement->SetWriteOnly(aPrincipal);
            return;
        }
=======
  aCanvasElement->SetWriteOnly();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!aCanvasElement->mExpandedReader) {
      // Allow future reads from this same princial only.
      aCanvasElement->SetWriteOnly(aPrincipal);
      return;
    }
||||||| merged common ancestors
        // If we got here, this must be the *second* extension tainting
        // the canvas.  Fall through to mark it WriteOnly for everyone.
    }
=======
bool CoerceDouble(const JS::Value& v, double* d) {
  if (v.isDouble()) {
    *d = v.toDouble();
  } else if (v.isInt32()) {
    *d = double(v.toInt32());
  } else if (v.isUndefined()) {
    *d = 0.0;
  } else {
    return false;
  }
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    // If we got here, this must be the *second* extension tainting
    // the canvas.  Fall through to mark it WriteOnly for everyone.
  }

  aCanvasElement->SetWriteOnly();
||||||| merged common ancestors
    aCanvasElement->SetWriteOnly();
=======
bool HasDrawWindowPrivilege(JSContext* aCx, JSObject* /* unused */) {
  return nsContentUtils::CallerHasPermission(aCx,
                                             nsGkAtoms::all_urlsPermission);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool CoerceDouble(const JS::Value& v, double* d) {
  if (v.isDouble()) {
    *d = v.toDouble();
  } else if (v.isInt32()) {
    *d = double(v.toInt32());
  } else if (v.isUndefined()) {
    *d = 0.0;
  } else {
    return false;
  }
  return true;
}
||||||| merged common ancestors
bool
CoerceDouble(const JS::Value& v, double* d)
{
    if (v.isDouble()) {
        *d = v.toDouble();
    } else if (v.isInt32()) {
        *d = double(v.toInt32());
    } else if (v.isUndefined()) {
        *d = 0.0;
    } else {
        return false;
    }
    return true;
}
=======
bool CheckWriteOnlySecurity(bool aCORSUsed, nsIPrincipal* aPrincipal,
                            bool aHadCrossOriginRedirects) {
  if (!aPrincipal) {
    return true;
  }

  if (!aCORSUsed) {
    if (aHadCrossOriginRedirects) {
      return true;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool HasDrawWindowPrivilege(JSContext* aCx, JSObject* /* unused */) {
  return nsContentUtils::CallerHasPermission(aCx,
                                             nsGkAtoms::all_urlsPermission);
||||||| merged common ancestors
bool
HasDrawWindowPrivilege(JSContext* aCx, JSObject* /* unused */)
{
  return nsContentUtils::CallerHasPermission(aCx, nsGkAtoms::all_urlsPermission);
=======
    nsIGlobalObject* incumbentSettingsObject = dom::GetIncumbentGlobal();
    if (!incumbentSettingsObject) {
      return true;
    }

    nsIPrincipal* principal = incumbentSettingsObject->PrincipalOrNull();
    if (NS_WARN_IF(!principal) || !(principal->Subsumes(aPrincipal))) {
      return true;
    }
  }

  return false;
>>>>>>> upstream-releases
}

}  // namespace CanvasUtils
}  // namespace mozilla
