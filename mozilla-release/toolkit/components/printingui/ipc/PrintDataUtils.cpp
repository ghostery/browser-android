/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=8 et tw=80 : */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "PrintDataUtils.h"
#include "nsIPrintSettings.h"
#include "nsIServiceManager.h"
#include "nsIWebBrowserPrint.h"
#include "nsString.h"

namespace mozilla {
namespace embedding {

/**
 * MockWebBrowserPrint is a mostly useless implementation of nsIWebBrowserPrint,
 * but wraps a PrintData so that it's able to return information to print
 * settings dialogs that need an nsIWebBrowserPrint to interrogate.
 */

NS_IMPL_ISUPPORTS(MockWebBrowserPrint, nsIWebBrowserPrint);

<<<<<<< HEAD
MockWebBrowserPrint::MockWebBrowserPrint(const PrintData &aData)
    : mData(aData) {}
||||||| merged common ancestors
MockWebBrowserPrint::MockWebBrowserPrint(const PrintData &aData)
  : mData(aData)
{
}
=======
MockWebBrowserPrint::MockWebBrowserPrint(const PrintData& aData)
    : mData(aData) {}
>>>>>>> upstream-releases

MockWebBrowserPrint::~MockWebBrowserPrint() {}

NS_IMETHODIMP
<<<<<<< HEAD
MockWebBrowserPrint::GetGlobalPrintSettings(
    nsIPrintSettings **aGlobalPrintSettings) {
||||||| merged common ancestors
MockWebBrowserPrint::GetGlobalPrintSettings(nsIPrintSettings **aGlobalPrintSettings)
{
=======
MockWebBrowserPrint::GetGlobalPrintSettings(
    nsIPrintSettings** aGlobalPrintSettings) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
MockWebBrowserPrint::GetCurrentPrintSettings(
    nsIPrintSettings **aCurrentPrintSettings) {
||||||| merged common ancestors
MockWebBrowserPrint::GetCurrentPrintSettings(nsIPrintSettings **aCurrentPrintSettings)
{
=======
MockWebBrowserPrint::GetCurrentPrintSettings(
    nsIPrintSettings** aCurrentPrintSettings) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
MockWebBrowserPrint::GetDoingPrint(bool *aDoingPrint) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
MockWebBrowserPrint::GetDoingPrint(bool *aDoingPrint)
{
  return NS_ERROR_NOT_IMPLEMENTED;
=======
MockWebBrowserPrint::GetDocumentName(nsAString& aDocName) {
  // The only consumer that cares about this is the OS X printing dialog.
  aDocName = mData.printJobName();
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
MockWebBrowserPrint::GetDoingPrintPreview(bool *aDoingPrintPreview) {
||||||| merged common ancestors
MockWebBrowserPrint::GetDoingPrintPreview(bool *aDoingPrintPreview)
{
=======
MockWebBrowserPrint::GetDoingPrint(bool* aDoingPrint) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
MockWebBrowserPrint::GetIsFramesetDocument(bool *aIsFramesetDocument) {
  *aIsFramesetDocument = mData.isFramesetDocument();
  return NS_OK;
}

NS_IMETHODIMP
MockWebBrowserPrint::GetIsFramesetFrameSelected(
    bool *aIsFramesetFrameSelected) {
  *aIsFramesetFrameSelected = mData.isFramesetFrameSelected();
  return NS_OK;
||||||| merged common ancestors
MockWebBrowserPrint::GetIsFramesetDocument(bool *aIsFramesetDocument)
{
  *aIsFramesetDocument = mData.isFramesetDocument();
  return NS_OK;
}

NS_IMETHODIMP
MockWebBrowserPrint::GetIsFramesetFrameSelected(bool *aIsFramesetFrameSelected)
{
  *aIsFramesetFrameSelected = mData.isFramesetFrameSelected();
  return NS_OK;
=======
MockWebBrowserPrint::GetDoingPrintPreview(bool* aDoingPrintPreview) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
MockWebBrowserPrint::GetIsIFrameSelected(bool *aIsIFrameSelected) {
||||||| merged common ancestors
MockWebBrowserPrint::GetIsIFrameSelected(bool *aIsIFrameSelected)
{
=======
MockWebBrowserPrint::GetIsIFrameSelected(bool* aIsIFrameSelected) {
>>>>>>> upstream-releases
  *aIsIFrameSelected = mData.isIFrameSelected();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
MockWebBrowserPrint::GetIsRangeSelection(bool *aIsRangeSelection) {
||||||| merged common ancestors
MockWebBrowserPrint::GetIsRangeSelection(bool *aIsRangeSelection)
{
=======
MockWebBrowserPrint::GetIsRangeSelection(bool* aIsRangeSelection) {
>>>>>>> upstream-releases
  *aIsRangeSelection = mData.isRangeSelection();
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
MockWebBrowserPrint::GetPrintPreviewNumPages(int32_t *aPrintPreviewNumPages) {
||||||| merged common ancestors
MockWebBrowserPrint::GetPrintPreviewNumPages(int32_t *aPrintPreviewNumPages)
{
=======
MockWebBrowserPrint::GetPrintPreviewNumPages(int32_t* aPrintPreviewNumPages) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
MockWebBrowserPrint::Print(nsIPrintSettings *aThePrintSettings,
                           nsIWebProgressListener *aWPListener) {
||||||| merged common ancestors
MockWebBrowserPrint::Print(nsIPrintSettings* aThePrintSettings,
                           nsIWebProgressListener* aWPListener)
{
=======
MockWebBrowserPrint::Print(nsIPrintSettings* aThePrintSettings,
                           nsIWebProgressListener* aWPListener) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
MockWebBrowserPrint::PrintPreview(nsIPrintSettings *aThePrintSettings,
                                  mozIDOMWindowProxy *aChildDOMWin,
                                  nsIWebProgressListener *aWPListener) {
||||||| merged common ancestors
MockWebBrowserPrint::PrintPreview(nsIPrintSettings* aThePrintSettings,
                                  mozIDOMWindowProxy* aChildDOMWin,
                                  nsIWebProgressListener* aWPListener)
{
=======
MockWebBrowserPrint::PrintPreview(nsIPrintSettings* aThePrintSettings,
                                  mozIDOMWindowProxy* aChildDOMWin,
                                  nsIWebProgressListener* aWPListener) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
MockWebBrowserPrint::PrintPreviewNavigate(int16_t aNavType, int32_t aPageNum) {
||||||| merged common ancestors
MockWebBrowserPrint::PrintPreviewNavigate(int16_t aNavType,
                                          int32_t aPageNum)
{
=======
MockWebBrowserPrint::PrintPreviewScrollToPage(int16_t aNavType,
                                              int32_t aPageNum) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
MockWebBrowserPrint::Cancel() { return NS_ERROR_NOT_IMPLEMENTED; }

NS_IMETHODIMP
<<<<<<< HEAD
MockWebBrowserPrint::EnumerateDocumentNames(uint32_t *aCount,
                                            char16_t ***aResult) {
  *aCount = 0;
  *aResult = nullptr;

  if (mData.printJobName().IsEmpty()) {
    return NS_OK;
  }

  // The only consumer that cares about this is the OS X printing
  // dialog, and even then, it only cares about the first document
  // name. That's why we only send a single document name through
  // PrintData.
  char16_t **array = (char16_t **)moz_xmalloc(sizeof(char16_t *));
  array[0] = ToNewUnicode(mData.printJobName());

  *aCount = 1;
  *aResult = array;
  return NS_OK;
}

NS_IMETHODIMP
MockWebBrowserPrint::ExitPrintPreview() { return NS_ERROR_NOT_IMPLEMENTED; }
||||||| merged common ancestors
MockWebBrowserPrint::EnumerateDocumentNames(uint32_t* aCount,
                                            char16_t*** aResult)
{
  *aCount = 0;
  *aResult = nullptr;

  if (mData.printJobName().IsEmpty()) {
    return NS_OK;
  }

  // The only consumer that cares about this is the OS X printing
  // dialog, and even then, it only cares about the first document
  // name. That's why we only send a single document name through
  // PrintData.
  char16_t** array = (char16_t**) moz_xmalloc(sizeof(char16_t*));
  array[0] = ToNewUnicode(mData.printJobName());

  *aCount = 1;
  *aResult = array;
  return NS_OK;
}

NS_IMETHODIMP
MockWebBrowserPrint::ExitPrintPreview()
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

} // namespace embedding
} // namespace mozilla
=======
MockWebBrowserPrint::ExitPrintPreview() { return NS_ERROR_NOT_IMPLEMENTED; }
>>>>>>> upstream-releases

}  // namespace embedding
}  // namespace mozilla
