/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsClipboard.h"
#include "FennecJNIWrappers.h"
#include "nsISupportsPrimitives.h"
#include "nsCOMPtr.h"
#include "nsComponentManagerUtils.h"
#include "nsPrimitiveHelpers.h"

using namespace mozilla;

NS_IMPL_ISUPPORTS(nsClipboard, nsIClipboard)

/* The Android clipboard only supports text and doesn't support mime types
 * so we assume all clipboard data is text/unicode for now. Documentation
 * indicates that support for other data types is planned for future
 * releases.
 */

nsClipboard::nsClipboard() {}

NS_IMETHODIMP
<<<<<<< HEAD
nsClipboard::SetData(nsITransferable *aTransferable, nsIClipboardOwner *anOwner,
                     int32_t aWhichClipboard) {
  if (aWhichClipboard != kGlobalClipboard) return NS_ERROR_NOT_IMPLEMENTED;

  nsCOMPtr<nsISupports> tmp;
  nsresult rv =
      aTransferable->GetTransferData(kUnicodeMime, getter_AddRefs(tmp));
  NS_ENSURE_SUCCESS(rv, rv);
  nsCOMPtr<nsISupportsString> supportsString = do_QueryInterface(tmp);
  // No support for non-text data
  NS_ENSURE_TRUE(supportsString, NS_ERROR_NOT_IMPLEMENTED);
  nsAutoString buffer;
  supportsString->GetData(buffer);

  java::Clipboard::SetText(java::GeckoAppShell::GetApplicationContext(),
                           buffer);
  return NS_OK;
||||||| merged common ancestors
nsClipboard::SetData(nsITransferable *aTransferable,
                     nsIClipboardOwner *anOwner, int32_t aWhichClipboard)
{
  if (aWhichClipboard != kGlobalClipboard)
    return NS_ERROR_NOT_IMPLEMENTED;

  nsCOMPtr<nsISupports> tmp;
  uint32_t len;
  nsresult rv  = aTransferable->GetTransferData(kUnicodeMime, getter_AddRefs(tmp),
                                                &len);
  NS_ENSURE_SUCCESS(rv, rv);
  nsCOMPtr<nsISupportsString> supportsString = do_QueryInterface(tmp);
  // No support for non-text data
  NS_ENSURE_TRUE(supportsString, NS_ERROR_NOT_IMPLEMENTED);
  nsAutoString buffer;
  supportsString->GetData(buffer);

  java::Clipboard::SetText(java::GeckoAppShell::GetApplicationContext(),
                           buffer);
  return NS_OK;
=======
nsClipboard::SetData(nsITransferable* aTransferable, nsIClipboardOwner* anOwner,
                     int32_t aWhichClipboard) {
  if (aWhichClipboard != kGlobalClipboard) return NS_ERROR_NOT_IMPLEMENTED;

  if (!jni::IsAvailable()) {
    return NS_ERROR_NOT_AVAILABLE;
  }

  nsTArray<nsCString> flavors;
  aTransferable->FlavorsTransferableCanImport(flavors);

  nsAutoString html;
  nsAutoString text;

  for (auto& flavorStr : flavors) {
    if (flavorStr.EqualsLiteral(kUnicodeMime)) {
      nsCOMPtr<nsISupports> item;
      nsresult rv =
          aTransferable->GetTransferData(kUnicodeMime, getter_AddRefs(item));
      if (NS_WARN_IF(NS_FAILED(rv))) {
        continue;
      }
      nsCOMPtr<nsISupportsString> supportsString = do_QueryInterface(item);
      if (supportsString) {
        supportsString->GetData(text);
      }
    } else if (flavorStr.EqualsLiteral(kHTMLMime)) {
      nsCOMPtr<nsISupports> item;
      nsresult rv =
          aTransferable->GetTransferData(kHTMLMime, getter_AddRefs(item));
      if (NS_WARN_IF(NS_FAILED(rv))) {
        continue;
      }
      nsCOMPtr<nsISupportsString> supportsString = do_QueryInterface(item);
      if (supportsString) {
        supportsString->GetData(html);
      }
    }
  }

  if (!html.IsEmpty() &&
      java::Clipboard::SetHTML(GeckoAppShell::GetApplicationContext(), text,
                               html)) {
    return NS_OK;
  }
  if (!text.IsEmpty() &&
      java::Clipboard::SetText(GeckoAppShell::GetApplicationContext(), text)) {
    return NS_OK;
  }

  return NS_ERROR_FAILURE;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsClipboard::GetData(nsITransferable *aTransferable, int32_t aWhichClipboard) {
  if (aWhichClipboard != kGlobalClipboard) return NS_ERROR_NOT_IMPLEMENTED;

  nsAutoString buffer;
  if (!AndroidBridge::Bridge()) return NS_ERROR_NOT_IMPLEMENTED;
  if (!AndroidBridge::Bridge()->GetClipboardText(buffer))
    return NS_ERROR_UNEXPECTED;

  nsresult rv;
  nsCOMPtr<nsISupportsString> dataWrapper =
      do_CreateInstance(NS_SUPPORTS_STRING_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = dataWrapper->SetData(buffer);
  NS_ENSURE_SUCCESS(rv, rv);

  // If our data flavor has already been added, this will fail. But we don't
  // care
  aTransferable->AddDataFlavor(kUnicodeMime);

  nsCOMPtr<nsISupports> nsisupportsDataWrapper = do_QueryInterface(dataWrapper);
  rv = aTransferable->SetTransferData(kUnicodeMime, nsisupportsDataWrapper);
  NS_ENSURE_SUCCESS(rv, rv);
||||||| merged common ancestors
nsClipboard::GetData(nsITransferable *aTransferable, int32_t aWhichClipboard)
{
  if (aWhichClipboard != kGlobalClipboard)
    return NS_ERROR_NOT_IMPLEMENTED;

  nsAutoString buffer;
  if (!AndroidBridge::Bridge())
    return NS_ERROR_NOT_IMPLEMENTED;
  if (!AndroidBridge::Bridge()->GetClipboardText(buffer))
    return NS_ERROR_UNEXPECTED;

  nsresult rv;
  nsCOMPtr<nsISupportsString> dataWrapper =
    do_CreateInstance(NS_SUPPORTS_STRING_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = dataWrapper->SetData(buffer);
  NS_ENSURE_SUCCESS(rv, rv);

  // If our data flavor has already been added, this will fail. But we don't care
  aTransferable->AddDataFlavor(kUnicodeMime);

  nsCOMPtr<nsISupports> nsisupportsDataWrapper =
    do_QueryInterface(dataWrapper);
  rv = aTransferable->SetTransferData(kUnicodeMime, nsisupportsDataWrapper,
                                      buffer.Length() * sizeof(char16_t));
  NS_ENSURE_SUCCESS(rv, rv);
=======
nsClipboard::GetData(nsITransferable* aTransferable, int32_t aWhichClipboard) {
  if (aWhichClipboard != kGlobalClipboard) return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases

  if (!jni::IsAvailable()) {
    return NS_ERROR_NOT_AVAILABLE;
  }

  nsTArray<nsCString> flavors;
  aTransferable->FlavorsTransferableCanImport(flavors);

  for (auto& flavorStr : flavors) {
    if (flavorStr.EqualsLiteral(kUnicodeMime) ||
        flavorStr.EqualsLiteral(kHTMLMime)) {
      auto text =
          Clipboard::GetData(GeckoAppShell::GetApplicationContext(), flavorStr);
      if (!text) {
        continue;
      }
      nsString buffer = text->ToString();
      if (buffer.IsEmpty()) {
        continue;
      }
      nsCOMPtr<nsISupports> wrapper;
      nsPrimitiveHelpers::CreatePrimitiveForData(flavorStr, buffer.get(),
                                                 buffer.Length() * 2,
                                                 getter_AddRefs(wrapper));
      if (wrapper) {
        aTransferable->SetTransferData(flavorStr.get(), wrapper);
        return NS_OK;
      }
    }
  }

  return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsClipboard::EmptyClipboard(int32_t aWhichClipboard) {
  if (aWhichClipboard != kGlobalClipboard) return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsClipboard::EmptyClipboard(int32_t aWhichClipboard)
{
  if (aWhichClipboard != kGlobalClipboard)
    return NS_ERROR_NOT_IMPLEMENTED;
=======
nsClipboard::EmptyClipboard(int32_t aWhichClipboard) {
  if (aWhichClipboard != kGlobalClipboard) return NS_ERROR_NOT_IMPLEMENTED;

  if (!jni::IsAvailable()) {
    return NS_ERROR_NOT_AVAILABLE;
  }

>>>>>>> upstream-releases
  java::Clipboard::ClearText(java::GeckoAppShell::GetApplicationContext());

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsClipboard::HasDataMatchingFlavors(const char **aFlavorList, uint32_t aLength,
                                    int32_t aWhichClipboard, bool *aHasText) {
||||||| merged common ancestors
nsClipboard::HasDataMatchingFlavors(const char **aFlavorList,
                                    uint32_t aLength, int32_t aWhichClipboard,
                                    bool *aHasText)
{
=======
nsClipboard::HasDataMatchingFlavors(const nsTArray<nsCString>& aFlavorList,
                                    int32_t aWhichClipboard, bool* aHasText) {
>>>>>>> upstream-releases
  *aHasText = false;
<<<<<<< HEAD
  if (aWhichClipboard != kGlobalClipboard) return NS_ERROR_NOT_IMPLEMENTED;

  for (uint32_t k = 0; k < aLength; k++) {
    if (strcmp(aFlavorList[k], kUnicodeMime) == 0) {
      *aHasText = java::Clipboard::HasText(
          java::GeckoAppShell::GetApplicationContext());
      break;
||||||| merged common ancestors
  if (aWhichClipboard != kGlobalClipboard)
    return NS_ERROR_NOT_IMPLEMENTED;

  for (uint32_t k = 0; k < aLength; k++) {
    if (strcmp(aFlavorList[k], kUnicodeMime) == 0) {
      *aHasText = java::Clipboard::HasText(
          java::GeckoAppShell::GetApplicationContext());
      break;
=======
  if (aWhichClipboard != kGlobalClipboard) return NS_ERROR_NOT_IMPLEMENTED;

  if (!jni::IsAvailable()) {
    return NS_ERROR_NOT_AVAILABLE;
  }

  for (auto& flavor : aFlavorList) {
    bool hasData =
        java::Clipboard::HasData(java::GeckoAppShell::GetApplicationContext(),
                                 NS_ConvertASCIItoUTF16(flavor));
    if (hasData) {
      *aHasText = true;
      return NS_OK;
>>>>>>> upstream-releases
    }
  }

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsClipboard::SupportsSelectionClipboard(bool *aIsSupported) {
||||||| merged common ancestors
nsClipboard::SupportsSelectionClipboard(bool *aIsSupported)
{
=======
nsClipboard::SupportsSelectionClipboard(bool* aIsSupported) {
>>>>>>> upstream-releases
  *aIsSupported = false;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsClipboard::SupportsFindClipboard(bool *_retval) {
||||||| merged common ancestors
nsClipboard::SupportsFindClipboard(bool* _retval)
{
=======
nsClipboard::SupportsFindClipboard(bool* _retval) {
>>>>>>> upstream-releases
  *_retval = false;
  return NS_OK;
}
