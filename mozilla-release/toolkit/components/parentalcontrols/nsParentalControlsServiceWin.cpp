/* -*- Mode: C++; tab-width: 2; indent-tabs-mode:nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsParentalControlsService.h"
#include "nsString.h"
#include "nsIArray.h"
#include "nsIWidget.h"
#include "nsIInterfaceRequestor.h"
#include "nsIInterfaceRequestorUtils.h"
#include "nsIFile.h"
#include "nsILocalFileWin.h"
#include "nsArrayUtils.h"
#include "nsIXULAppInfo.h"
#include "mozilla/UniquePtr.h"
#include "mozilla/WindowsVersion.h"

using namespace mozilla;

NS_IMPL_ISUPPORTS(nsParentalControlsService, nsIParentalControlsService)

nsParentalControlsService::nsParentalControlsService()
    : mEnabled(false), mProvider(0), mPC(nullptr) {
  HRESULT hr;
  CoInitialize(nullptr);
  hr = CoCreateInstance(__uuidof(WindowsParentalControls), nullptr,
                        CLSCTX_INPROC, IID_PPV_ARGS(&mPC));
  if (FAILED(hr)) return;

  RefPtr<IWPCSettings> wpcs;
  if (FAILED(mPC->GetUserSettings(nullptr, getter_AddRefs(wpcs)))) {
    // Not available on this os or not enabled for this user account or we're
    // running as admin
    mPC->Release();
    mPC = nullptr;
    return;
  }

  DWORD settings = 0;
  wpcs->GetRestrictions(&settings);

  // If we can't determine specifically whether Web Filtering is on/off (i.e.
  // we're on Windows < 8), then assume it's on unless no restrictions are set.
  bool enable = IsWin8OrLater() ? settings & WPCFLAG_WEB_FILTERED
                                : settings != WPCFLAG_NO_RESTRICTION;

  if (enable) {
    mEnabled = true;
  }
}

nsParentalControlsService::~nsParentalControlsService() {
  if (mPC) mPC->Release();

  if (mProvider) {
    EventUnregister(mProvider);
  }
}

//------------------------------------------------------------------------

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

  if (mEnabled) *aResult = true;

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
  *aResult = false;

  if (!mEnabled) return NS_ERROR_NOT_AVAILABLE;

  RefPtr<IWPCWebSettings> wpcws;
  if (SUCCEEDED(mPC->GetWebSettings(nullptr, getter_AddRefs(wpcws)))) {
    DWORD settings = 0;
    wpcws->GetSettings(&settings);
    if (settings == WPCFLAG_WEB_SETTING_DOWNLOADSBLOCKED) *aResult = true;
  }

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
  *aResult = false;

  if (!mEnabled) return NS_ERROR_NOT_AVAILABLE;

  // Check the general purpose logging flag
  RefPtr<IWPCSettings> wpcs;
  if (SUCCEEDED(mPC->GetUserSettings(nullptr, getter_AddRefs(wpcs)))) {
    BOOL enabled = FALSE;
    wpcs->IsLoggingRequired(&enabled);
    if (enabled) *aResult = true;
  }

  return NS_OK;
}

// Post a log event to the system
NS_IMETHODIMP
<<<<<<< HEAD
nsParentalControlsService::Log(int16_t aEntryType, bool blocked,
                               nsIURI *aSource, nsIFile *aTarget) {
  if (!mEnabled) return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsParentalControlsService::Log(int16_t aEntryType, bool blocked, nsIURI *aSource, nsIFile *aTarget)
{
  if (!mEnabled)
    return NS_ERROR_NOT_AVAILABLE;
=======
nsParentalControlsService::Log(int16_t aEntryType, bool blocked,
                               nsIURI* aSource, nsIFile* aTarget) {
  if (!mEnabled) return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases

  NS_ENSURE_ARG_POINTER(aSource);

  // Confirm we should be logging
  bool enabled;
  GetLoggingEnabled(&enabled);
  if (!enabled) return NS_ERROR_NOT_AVAILABLE;

  // Register a Vista log event provider associated with the parental controls
  // channel.
  if (!mProvider) {
    if (EventRegister(&WPCPROV, nullptr, nullptr, &mProvider) != ERROR_SUCCESS)
      return NS_ERROR_OUT_OF_MEMORY;
  }

  switch (aEntryType) {
    case ePCLog_URIVisit:
      // Not needed, Vista's web content filter handles this for us
      break;
    case ePCLog_FileDownload:
      LogFileDownload(blocked, aSource, aTarget);
      break;
    default:
      break;
  }

  return NS_OK;
}

// Override a single URI
NS_IMETHODIMP
<<<<<<< HEAD
nsParentalControlsService::RequestURIOverride(
    nsIURI *aTarget, nsIInterfaceRequestor *aWindowContext, bool *_retval) {
||||||| merged common ancestors
nsParentalControlsService::RequestURIOverride(nsIURI *aTarget, nsIInterfaceRequestor *aWindowContext, bool *_retval)
{
=======
nsParentalControlsService::RequestURIOverride(
    nsIURI* aTarget, nsIInterfaceRequestor* aWindowContext, bool* _retval) {
>>>>>>> upstream-releases
  *_retval = false;

  if (!mEnabled) return NS_ERROR_NOT_AVAILABLE;

  NS_ENSURE_ARG_POINTER(aTarget);

  nsAutoCString spec;
  aTarget->GetSpec(spec);
  if (spec.IsEmpty()) return NS_ERROR_INVALID_ARG;

  HWND hWnd = nullptr;
  // If we have a native window, use its handle instead
  nsCOMPtr<nsIWidget> widget(do_GetInterface(aWindowContext));
  if (widget) hWnd = (HWND)widget->GetNativeData(NS_NATIVE_WINDOW);
  if (hWnd == nullptr) hWnd = GetDesktopWindow();

  BOOL ret;
  RefPtr<IWPCWebSettings> wpcws;
  if (SUCCEEDED(mPC->GetWebSettings(nullptr, getter_AddRefs(wpcws)))) {
    wpcws->RequestURLOverride(hWnd, NS_ConvertUTF8toUTF16(spec).get(), 0,
                              nullptr, &ret);
    *_retval = ret;
  }

  return NS_OK;
}

// Override a web page
NS_IMETHODIMP
<<<<<<< HEAD
nsParentalControlsService::RequestURIOverrides(
    nsIArray *aTargets, nsIInterfaceRequestor *aWindowContext, bool *_retval) {
||||||| merged common ancestors
nsParentalControlsService::RequestURIOverrides(nsIArray *aTargets, nsIInterfaceRequestor *aWindowContext, bool *_retval)
{
=======
nsParentalControlsService::RequestURIOverrides(
    nsIArray* aTargets, nsIInterfaceRequestor* aWindowContext, bool* _retval) {
>>>>>>> upstream-releases
  *_retval = false;

  if (!mEnabled) return NS_ERROR_NOT_AVAILABLE;

  NS_ENSURE_ARG_POINTER(aTargets);

  uint32_t arrayLength = 0;
  aTargets->GetLength(&arrayLength);
  if (!arrayLength) return NS_ERROR_INVALID_ARG;

  if (arrayLength == 1) {
    nsCOMPtr<nsIURI> uri = do_QueryElementAt(aTargets, 0);
    if (!uri) return NS_ERROR_INVALID_ARG;
    return RequestURIOverride(uri, aWindowContext, _retval);
  }

  HWND hWnd = nullptr;
  // If we have a native window, use its handle instead
  nsCOMPtr<nsIWidget> widget(do_GetInterface(aWindowContext));
  if (widget) hWnd = (HWND)widget->GetNativeData(NS_NATIVE_WINDOW);
  if (hWnd == nullptr) hWnd = GetDesktopWindow();

  // The first entry should be the root uri
  nsAutoCString rootSpec;
  nsCOMPtr<nsIURI> rootURI = do_QueryElementAt(aTargets, 0);
  if (!rootURI) return NS_ERROR_INVALID_ARG;

  rootURI->GetSpec(rootSpec);
  if (rootSpec.IsEmpty()) return NS_ERROR_INVALID_ARG;

  // Allocate an array of sub uri
  int32_t count = arrayLength - 1;
  auto arrUrls = MakeUnique<LPCWSTR[]>(count);

  uint32_t uriIdx = 0, idx;
  for (idx = 1; idx < arrayLength; idx++) {
    nsCOMPtr<nsIURI> uri = do_QueryElementAt(aTargets, idx);
    if (!uri) continue;

    nsAutoCString subURI;
    if (NS_FAILED(uri->GetSpec(subURI))) continue;

    arrUrls[uriIdx] = (LPCWSTR)UTF8ToNewUnicode(subURI);  // allocation
    if (!arrUrls[uriIdx]) continue;

    uriIdx++;
  }

  if (!uriIdx) return NS_ERROR_INVALID_ARG;

  BOOL ret;
  RefPtr<IWPCWebSettings> wpcws;
  if (SUCCEEDED(mPC->GetWebSettings(nullptr, getter_AddRefs(wpcws)))) {
    wpcws->RequestURLOverride(hWnd, NS_ConvertUTF8toUTF16(rootSpec).get(),
<<<<<<< HEAD
                              uriIdx, (LPCWSTR *)arrUrls.get(), &ret);
    *_retval = ret;
||||||| merged common ancestors
                             uriIdx, (LPCWSTR*)arrUrls.get(), &ret);
   *_retval = ret;
=======
                              uriIdx, (LPCWSTR*)arrUrls.get(), &ret);
    *_retval = ret;
>>>>>>> upstream-releases
  }

  // Free up the allocated strings in our array
<<<<<<< HEAD
  for (idx = 0; idx < uriIdx; idx++) free((void *)arrUrls[idx]);
||||||| merged common ancestors
  for (idx = 0; idx < uriIdx; idx++)
    free((void*)arrUrls[idx]);
=======
  for (idx = 0; idx < uriIdx; idx++) free((void*)arrUrls[idx]);
>>>>>>> upstream-releases

  return NS_OK;
}

//------------------------------------------------------------------------

// Sends a file download event to the Vista Event Log
<<<<<<< HEAD
void nsParentalControlsService::LogFileDownload(bool blocked, nsIURI *aSource,
                                                nsIFile *aTarget) {
||||||| merged common ancestors
void
nsParentalControlsService::LogFileDownload(bool blocked, nsIURI *aSource, nsIFile *aTarget)
{
=======
void nsParentalControlsService::LogFileDownload(bool blocked, nsIURI* aSource,
                                                nsIFile* aTarget) {
>>>>>>> upstream-releases
  nsAutoCString curi;

  // Note, EventDataDescCreate is a macro defined in the headers, not a function

  aSource->GetSpec(curi);
  nsAutoString uri = NS_ConvertUTF8toUTF16(curi);

  // Get the name of the currently running process
  nsCOMPtr<nsIXULAppInfo> appInfo =
      do_GetService("@mozilla.org/xre/app-info;1");
  nsAutoCString asciiAppName;
  if (appInfo) appInfo->GetName(asciiAppName);
  nsAutoString appName = NS_ConvertUTF8toUTF16(asciiAppName);

  static const WCHAR fill[] = L"";

  // See wpcevent.h and msdn for event formats
  EVENT_DATA_DESCRIPTOR eventData[WPC_ARGS_FILEDOWNLOADEVENT_CARGS];
  DWORD dwBlocked = blocked;

<<<<<<< HEAD
  EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_URL],
                      (const void *)uri.get(),
                      ((ULONG)uri.Length() + 1) * sizeof(WCHAR));
  EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_APPNAME],
                      (const void *)appName.get(),
                      ((ULONG)appName.Length() + 1) * sizeof(WCHAR));
  EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_VERSION],
                      (const void *)fill, sizeof(fill));
  EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_BLOCKED],
                      (const void *)&dwBlocked, sizeof(dwBlocked));

  nsCOMPtr<nsILocalFileWin> local(do_QueryInterface(aTarget));  // May be null
||||||| merged common ancestors
  EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_URL], (const void*)uri.get(),
                      ((ULONG)uri.Length()+1)*sizeof(WCHAR));
  EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_APPNAME], (const void*)appName.get(),
                      ((ULONG)appName.Length()+1)*sizeof(WCHAR));
  EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_VERSION], (const void*)fill, sizeof(fill));
  EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_BLOCKED], (const void*)&dwBlocked,
                      sizeof(dwBlocked));

  nsCOMPtr<nsILocalFileWin> local(do_QueryInterface(aTarget)); // May be null
=======
  EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_URL],
                      (const void*)uri.get(),
                      ((ULONG)uri.Length() + 1) * sizeof(WCHAR));
  EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_APPNAME],
                      (const void*)appName.get(),
                      ((ULONG)appName.Length() + 1) * sizeof(WCHAR));
  EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_VERSION],
                      (const void*)fill, sizeof(fill));
  EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_BLOCKED],
                      (const void*)&dwBlocked, sizeof(dwBlocked));

  nsCOMPtr<nsILocalFileWin> local(do_QueryInterface(aTarget));  // May be null
>>>>>>> upstream-releases
  if (local) {
    nsAutoString path;
    local->GetCanonicalPath(path);
<<<<<<< HEAD
    EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_PATH],
                        (const void *)path.get(),
                        ((ULONG)path.Length() + 1) * sizeof(WCHAR));
  } else {
    EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_PATH],
                        (const void *)fill, sizeof(fill));
||||||| merged common ancestors
    EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_PATH], (const void*)path.get(),
                        ((ULONG)path.Length()+1)*sizeof(WCHAR));
  }
  else {
    EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_PATH], (const void*)fill, sizeof(fill));
=======
    EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_PATH],
                        (const void*)path.get(),
                        ((ULONG)path.Length() + 1) * sizeof(WCHAR));
  } else {
    EventDataDescCreate(&eventData[WPC_ARGS_FILEDOWNLOADEVENT_PATH],
                        (const void*)fill, sizeof(fill));
>>>>>>> upstream-releases
  }

  EventWrite(mProvider, &WPCEVENT_WEB_FILEDOWNLOAD, ARRAYSIZE(eventData),
             eventData);
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
