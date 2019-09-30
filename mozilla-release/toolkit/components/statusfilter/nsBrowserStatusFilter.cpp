/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsBrowserStatusFilter.h"
#include "mozilla/SystemGroup.h"
#include "nsIChannel.h"
#include "nsITimer.h"
#include "nsIServiceManager.h"
#include "nsString.h"
#include "nsThreadUtils.h"

using namespace mozilla;

//-----------------------------------------------------------------------------
// nsBrowserStatusFilter <public>
//-----------------------------------------------------------------------------

nsBrowserStatusFilter::nsBrowserStatusFilter()
    : mTarget(GetMainThreadEventTarget()),
      mCurProgress(0),
      mMaxProgress(0),
      mCurrentPercentage(0),
      mStatusIsDirty(true),
      mIsLoadingDocument(false),
      mDelayedStatus(false),
      mDelayedProgress(false) {}

nsBrowserStatusFilter::~nsBrowserStatusFilter() {
  if (mTimer) {
    mTimer->Cancel();
  }
}

//-----------------------------------------------------------------------------
// nsBrowserStatusFilter::nsISupports
//-----------------------------------------------------------------------------

NS_IMPL_CYCLE_COLLECTION(nsBrowserStatusFilter, mListener, mTarget)

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(nsBrowserStatusFilter)
  NS_INTERFACE_MAP_ENTRY(nsIWebProgress)
  NS_INTERFACE_MAP_ENTRY(nsIWebProgressListener)
  NS_INTERFACE_MAP_ENTRY(nsIWebProgressListener2)
  NS_INTERFACE_MAP_ENTRY(nsISupportsWeakReference)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIWebProgress)
NS_INTERFACE_MAP_END

NS_IMPL_CYCLE_COLLECTING_ADDREF(nsBrowserStatusFilter)
NS_IMPL_CYCLE_COLLECTING_RELEASE(nsBrowserStatusFilter)

//-----------------------------------------------------------------------------
// nsBrowserStatusFilter::nsIWebProgress
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::AddProgressListener(nsIWebProgressListener *aListener,
                                           uint32_t aNotifyMask) {
  mListener = aListener;
  return NS_OK;
||||||| merged common ancestors
nsBrowserStatusFilter::AddProgressListener(nsIWebProgressListener *aListener,
                                           uint32_t aNotifyMask)
{
    mListener = aListener;
    return NS_OK;
=======
nsBrowserStatusFilter::AddProgressListener(nsIWebProgressListener* aListener,
                                           uint32_t aNotifyMask) {
  mListener = aListener;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::RemoveProgressListener(
    nsIWebProgressListener *aListener) {
  if (aListener == mListener) mListener = nullptr;
  return NS_OK;
||||||| merged common ancestors
nsBrowserStatusFilter::RemoveProgressListener(nsIWebProgressListener *aListener)
{
    if (aListener == mListener)
        mListener = nullptr;
    return NS_OK;
=======
nsBrowserStatusFilter::RemoveProgressListener(
    nsIWebProgressListener* aListener) {
  if (aListener == mListener) mListener = nullptr;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::GetDOMWindow(mozIDOMWindowProxy **aResult) {
  MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetDOMWindow");
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsBrowserStatusFilter::GetDOMWindow(mozIDOMWindowProxy **aResult)
{
    MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetDOMWindow");
    return NS_ERROR_NOT_IMPLEMENTED;
=======
nsBrowserStatusFilter::GetDOMWindow(mozIDOMWindowProxy** aResult) {
  MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetDOMWindow");
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::GetDOMWindowID(uint64_t *aResult) {
  *aResult = 0;
  MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetDOMWindowID");
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsBrowserStatusFilter::GetDOMWindowID(uint64_t *aResult)
{
    *aResult = 0;
    MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetDOMWindowID");
    return NS_ERROR_NOT_IMPLEMENTED;
=======
nsBrowserStatusFilter::GetDOMWindowID(uint64_t* aResult) {
  *aResult = 0;
  MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetDOMWindowID");
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::GetInnerDOMWindowID(uint64_t *aResult) {
  *aResult = 0;
  MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetInnerDOMWindowID");
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsBrowserStatusFilter::GetInnerDOMWindowID(uint64_t *aResult)
{
    *aResult = 0;
    MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetInnerDOMWindowID");
    return NS_ERROR_NOT_IMPLEMENTED;
=======
nsBrowserStatusFilter::GetInnerDOMWindowID(uint64_t* aResult) {
  *aResult = 0;
  MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetInnerDOMWindowID");
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::GetIsTopLevel(bool *aIsTopLevel) {
  *aIsTopLevel = false;
  MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetIsTopLevel");
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsBrowserStatusFilter::GetIsTopLevel(bool *aIsTopLevel)
{
    *aIsTopLevel = false;
    MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetIsTopLevel");
    return NS_ERROR_NOT_IMPLEMENTED;
=======
nsBrowserStatusFilter::GetIsTopLevel(bool* aIsTopLevel) {
  *aIsTopLevel = false;
  MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetIsTopLevel");
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::GetIsLoadingDocument(bool *aIsLoadingDocument) {
  MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetIsLoadingDocument");
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsBrowserStatusFilter::GetIsLoadingDocument(bool *aIsLoadingDocument)
{
    MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetIsLoadingDocument");
    return NS_ERROR_NOT_IMPLEMENTED;
=======
nsBrowserStatusFilter::GetIsLoadingDocument(bool* aIsLoadingDocument) {
  MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetIsLoadingDocument");
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::GetLoadType(uint32_t *aLoadType) {
  *aLoadType = 0;
  MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetLoadType");
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsBrowserStatusFilter::GetLoadType(uint32_t *aLoadType)
{
    *aLoadType = 0;
    MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetLoadType");
    return NS_ERROR_NOT_IMPLEMENTED;
=======
nsBrowserStatusFilter::GetLoadType(uint32_t* aLoadType) {
  *aLoadType = 0;
  MOZ_ASSERT_UNREACHABLE("nsBrowserStatusFilter::GetLoadType");
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::GetTarget(nsIEventTarget **aTarget) {
  nsCOMPtr<nsIEventTarget> target = mTarget;
  target.forget(aTarget);
  return NS_OK;
||||||| merged common ancestors
nsBrowserStatusFilter::GetTarget(nsIEventTarget** aTarget)
{
    nsCOMPtr<nsIEventTarget> target = mTarget;
    target.forget(aTarget);
    return NS_OK;
=======
nsBrowserStatusFilter::GetTarget(nsIEventTarget** aTarget) {
  nsCOMPtr<nsIEventTarget> target = mTarget;
  target.forget(aTarget);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::SetTarget(nsIEventTarget *aTarget) {
  mTarget = aTarget;
  return NS_OK;
||||||| merged common ancestors
nsBrowserStatusFilter::SetTarget(nsIEventTarget* aTarget)
{
    mTarget = aTarget;
    return NS_OK;
=======
nsBrowserStatusFilter::SetTarget(nsIEventTarget* aTarget) {
  mTarget = aTarget;
  return NS_OK;
>>>>>>> upstream-releases
}

//-----------------------------------------------------------------------------
// nsBrowserStatusFilter::nsIWebProgressListener
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::OnStateChange(nsIWebProgress *aWebProgress,
                                     nsIRequest *aRequest, uint32_t aStateFlags,
                                     nsresult aStatus) {
  if (!mListener) return NS_OK;

  if (aStateFlags & STATE_START) {
    // Reset members on beginning of document loading, but we don't want
    // subframe document loading followed by the root document loading
    // resets members accidentally, so for non-toplevel load we check if
    // there hasn't been a document load started.
    if (aStateFlags & STATE_IS_DOCUMENT) {
      bool isTopLevel = false;
      aWebProgress->GetIsTopLevel(&isTopLevel);
      if (!mIsLoadingDocument || isTopLevel) {
        ResetMembers();
      }
      mIsLoadingDocument = true;
||||||| merged common ancestors
nsBrowserStatusFilter::OnStateChange(nsIWebProgress *aWebProgress,
                                     nsIRequest *aRequest,
                                     uint32_t aStateFlags,
                                     nsresult aStatus)
{
    if (!mListener)
        return NS_OK;

    if (aStateFlags & STATE_START) {
        // Reset members on beginning of document loading, but we don't want
        // subframe document loading followed by the root document loading
        // resets members accidentally, so for non-toplevel load we check if
        // there hasn't been a document load started.
        if (aStateFlags & STATE_IS_DOCUMENT) {
            bool isTopLevel = false;
            aWebProgress->GetIsTopLevel(&isTopLevel);
            if (!mIsLoadingDocument || isTopLevel) {
                ResetMembers();
            }
            mIsLoadingDocument = true;
        }
    } else if (aStateFlags & STATE_STOP) {
        // Flush pending status / progress update during document loading.
        if (mIsLoadingDocument) {
            bool isLoadingDocument = true;
            aWebProgress->GetIsLoadingDocument(&isLoadingDocument);
            mIsLoadingDocument &= isLoadingDocument;

            if (mTimer) {
                mTimer->Cancel();
                ProcessTimeout();
            }
        }
    } else {
        // No need to forward this state change.
        return NS_OK;
=======
nsBrowserStatusFilter::OnStateChange(nsIWebProgress* aWebProgress,
                                     nsIRequest* aRequest, uint32_t aStateFlags,
                                     nsresult aStatus) {
  if (!mListener) return NS_OK;

  if (aStateFlags & STATE_START) {
    // Reset members on beginning of document loading, but we don't want
    // subframe document loading followed by the root document loading
    // resets members accidentally, so for non-toplevel load we check if
    // there hasn't been a document load started.
    if (aStateFlags & STATE_IS_DOCUMENT) {
      bool isTopLevel = false;
      aWebProgress->GetIsTopLevel(&isTopLevel);
      if (!mIsLoadingDocument || isTopLevel) {
        ResetMembers();
      }
      mIsLoadingDocument = true;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  } else if (aStateFlags & STATE_STOP) {
    // Flush pending status / progress update during document loading.
    if (mIsLoadingDocument) {
      bool isLoadingDocument = true;
      aWebProgress->GetIsLoadingDocument(&isLoadingDocument);
      mIsLoadingDocument &= isLoadingDocument;

      if (mTimer) {
        mTimer->Cancel();
        ProcessTimeout();
      }
||||||| merged common ancestors

    // Only notify listener for STATE_IS_NETWORK.
    if (aStateFlags & STATE_IS_NETWORK) {
        return mListener->OnStateChange(aWebProgress, aRequest, aStateFlags,
                                        aStatus);
=======
  } else if (aStateFlags & STATE_STOP) {
    // Flush pending status / progress update during document loading.
    if (mIsLoadingDocument) {
      bool isLoadingDocument = true;
      aWebProgress->GetIsLoadingDocument(&isLoadingDocument);
      mIsLoadingDocument &= isLoadingDocument;

      if (mTimer) {
        mTimer->Cancel();
        CallDelayedProgressListeners();

        // CallDelayedProgressListeners() may trigger OnStatusChange and/or
        // OnProgressChange handlers, which can run JS and may even call
        // RemoveProgressListener.
        if (!mListener) {
          return NS_OK;
        }
      }
>>>>>>> upstream-releases
    }
  } else {
    // No need to forward this state change.
    return NS_OK;
<<<<<<< HEAD
  }

  // Only notify listener for STATE_IS_NETWORK.
  if (aStateFlags & STATE_IS_NETWORK) {
    return mListener->OnStateChange(aWebProgress, aRequest, aStateFlags,
                                    aStatus);
  }

  return NS_OK;
||||||| merged common ancestors
=======
  }

  // Only notify listener for STATE_IS_NETWORK or STATE_IS_REDIRECTED_DOCUMENT
  if (aStateFlags & STATE_IS_NETWORK ||
      aStateFlags & STATE_IS_REDIRECTED_DOCUMENT) {
    return mListener->OnStateChange(aWebProgress, aRequest, aStateFlags,
                                    aStatus);
  }

  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
nsBrowserStatusFilter::OnProgressChange(nsIWebProgress* aWebProgress,
                                        nsIRequest* aRequest,
                                        int32_t aCurSelfProgress,
                                        int32_t aMaxSelfProgress,
                                        int32_t aCurTotalProgress,
                                        int32_t aMaxTotalProgress) {
  if (!mListener) return NS_OK;

  //
  // limit frequency of calls to OnProgressChange
  //

  mCurProgress = (int64_t)aCurTotalProgress;
  mMaxProgress = (int64_t)aMaxTotalProgress;

  if (mDelayedProgress) return NS_OK;

  if (!mDelayedStatus) {
    MaybeSendProgress();
    StartDelayTimer();
  }

  mDelayedProgress = true;

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::OnLocationChange(nsIWebProgress *aWebProgress,
                                        nsIRequest *aRequest, nsIURI *aLocation,
                                        uint32_t aFlags) {
  if (!mListener) return NS_OK;

  return mListener->OnLocationChange(aWebProgress, aRequest, aLocation, aFlags);
||||||| merged common ancestors
nsBrowserStatusFilter::OnLocationChange(nsIWebProgress *aWebProgress,
                                        nsIRequest *aRequest,
                                        nsIURI *aLocation,
                                        uint32_t aFlags)
{
    if (!mListener)
        return NS_OK;

    return mListener->OnLocationChange(aWebProgress, aRequest, aLocation,
                                       aFlags);
=======
nsBrowserStatusFilter::OnLocationChange(nsIWebProgress* aWebProgress,
                                        nsIRequest* aRequest, nsIURI* aLocation,
                                        uint32_t aFlags) {
  if (!mListener) return NS_OK;

  return mListener->OnLocationChange(aWebProgress, aRequest, aLocation, aFlags);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::OnStatusChange(nsIWebProgress *aWebProgress,
                                      nsIRequest *aRequest, nsresult aStatus,
                                      const char16_t *aMessage) {
  if (!mListener) return NS_OK;

  //
  // limit frequency of calls to OnStatusChange
  //
  if (mStatusIsDirty || !mCurrentStatusMsg.Equals(aMessage)) {
    mStatusIsDirty = true;
    mStatusMsg = aMessage;
  }
||||||| merged common ancestors
nsBrowserStatusFilter::OnStatusChange(nsIWebProgress *aWebProgress,
                                      nsIRequest *aRequest,
                                      nsresult aStatus,
                                      const char16_t *aMessage)
{
    if (!mListener)
        return NS_OK;

    //
    // limit frequency of calls to OnStatusChange
    //
    if (mStatusIsDirty || !mCurrentStatusMsg.Equals(aMessage)) {
        mStatusIsDirty = true;
        mStatusMsg = aMessage;
    }
=======
nsBrowserStatusFilter::OnStatusChange(nsIWebProgress* aWebProgress,
                                      nsIRequest* aRequest, nsresult aStatus,
                                      const char16_t* aMessage) {
  if (!mListener) return NS_OK;

  //
  // limit frequency of calls to OnStatusChange
  //
  if (mStatusIsDirty || !mCurrentStatusMsg.Equals(aMessage)) {
    mStatusIsDirty = true;
    mStatusMsg = aMessage;
  }
>>>>>>> upstream-releases

  if (mDelayedStatus) return NS_OK;

  if (!mDelayedProgress) {
    MaybeSendStatus();
    StartDelayTimer();
  }

  mDelayedStatus = true;

<<<<<<< HEAD
  return NS_OK;
||||||| merged common ancestors
    return NS_OK;
=======
  return NS_OK;
}

NS_IMETHODIMP
nsBrowserStatusFilter::OnSecurityChange(nsIWebProgress* aWebProgress,
                                        nsIRequest* aRequest, uint32_t aState) {
  if (!mListener) return NS_OK;

  return mListener->OnSecurityChange(aWebProgress, aRequest, aState);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::OnSecurityChange(nsIWebProgress *aWebProgress,
                                        nsIRequest *aRequest, uint32_t aState) {
  if (!mListener) return NS_OK;

  return mListener->OnSecurityChange(aWebProgress, aRequest, aState);
||||||| merged common ancestors
nsBrowserStatusFilter::OnSecurityChange(nsIWebProgress *aWebProgress,
                                        nsIRequest *aRequest,
                                        uint32_t aOldState,
                                        uint32_t aState,
                                        const nsAString& aContentBlockingLogJSON)
{
    if (!mListener)
        return NS_OK;

    return mListener->OnSecurityChange(aWebProgress, aRequest, aOldState,
                                       aState, aContentBlockingLogJSON);
=======
nsBrowserStatusFilter::OnContentBlockingEvent(nsIWebProgress* aWebProgress,
                                              nsIRequest* aRequest,
                                              uint32_t aEvent) {
  if (!mListener) return NS_OK;

  return mListener->OnContentBlockingEvent(aWebProgress, aRequest, aEvent);
>>>>>>> upstream-releases
}

//-----------------------------------------------------------------------------
// nsBrowserStatusFilter::nsIWebProgressListener2
//-----------------------------------------------------------------------------
NS_IMETHODIMP
nsBrowserStatusFilter::OnProgressChange64(nsIWebProgress* aWebProgress,
                                          nsIRequest* aRequest,
                                          int64_t aCurSelfProgress,
                                          int64_t aMaxSelfProgress,
                                          int64_t aCurTotalProgress,
                                          int64_t aMaxTotalProgress) {
  // XXX truncates 64-bit to 32-bit
  return OnProgressChange(aWebProgress, aRequest, (int32_t)aCurSelfProgress,
                          (int32_t)aMaxSelfProgress, (int32_t)aCurTotalProgress,
                          (int32_t)aMaxTotalProgress);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBrowserStatusFilter::OnRefreshAttempted(nsIWebProgress *aWebProgress,
                                          nsIURI *aUri, int32_t aDelay,
                                          bool aSameUri, bool *allowRefresh) {
  nsCOMPtr<nsIWebProgressListener2> listener = do_QueryInterface(mListener);
  if (!listener) {
    *allowRefresh = true;
    return NS_OK;
  }
||||||| merged common ancestors
nsBrowserStatusFilter::OnRefreshAttempted(nsIWebProgress *aWebProgress,
                                          nsIURI *aUri,
                                          int32_t aDelay,
                                          bool aSameUri,
                                          bool *allowRefresh)
{
    nsCOMPtr<nsIWebProgressListener2> listener =
        do_QueryInterface(mListener);
    if (!listener) {
        *allowRefresh = true;
        return NS_OK;
    }
=======
nsBrowserStatusFilter::OnRefreshAttempted(nsIWebProgress* aWebProgress,
                                          nsIURI* aUri, int32_t aDelay,
                                          bool aSameUri, bool* allowRefresh) {
  nsCOMPtr<nsIWebProgressListener2> listener = do_QueryInterface(mListener);
  if (!listener) {
    *allowRefresh = true;
    return NS_OK;
  }
>>>>>>> upstream-releases

  return listener->OnRefreshAttempted(aWebProgress, aUri, aDelay, aSameUri,
                                      allowRefresh);
}

//-----------------------------------------------------------------------------
// nsBrowserStatusFilter <private>
//-----------------------------------------------------------------------------

void nsBrowserStatusFilter::ResetMembers() {
  mMaxProgress = 0;
  mCurProgress = 0;
  mCurrentPercentage = 0;
  mStatusIsDirty = true;
  // We don't reset mIsLoadingDocument here.
  // It's controlled by OnStateChange based on webProgress states.
}

void nsBrowserStatusFilter::MaybeSendProgress() {
  if (mCurProgress > mMaxProgress || mCurProgress <= 0) return;

  // check our percentage
  int32_t percentage = (int32_t) double(mCurProgress) * 100 / mMaxProgress;

  // The progress meter only updates for increases greater than 3 percent
  if (percentage > (mCurrentPercentage + 3)) {
    mCurrentPercentage = percentage;
    // XXX truncates 64-bit to 32-bit
    mListener->OnProgressChange(nullptr, nullptr, 0, 0, (int32_t)mCurProgress,
                                (int32_t)mMaxProgress);
  }
}

void nsBrowserStatusFilter::MaybeSendStatus() {
  if (mStatusIsDirty) {
    mListener->OnStatusChange(nullptr, nullptr, NS_OK, mStatusMsg.get());
    mCurrentStatusMsg = mStatusMsg;
    mStatusIsDirty = false;
  }
}

nsresult nsBrowserStatusFilter::StartDelayTimer() {
  NS_ASSERTION(!DelayInEffect(), "delay should not be in effect");

  return NS_NewTimerWithFuncCallback(getter_AddRefs(mTimer), TimeoutHandler,
                                     this, 160, nsITimer::TYPE_ONE_SHOT,
                                     "nsBrowserStatusFilter::TimeoutHandler",
                                     mTarget);
}

<<<<<<< HEAD
void nsBrowserStatusFilter::ProcessTimeout() {
  mTimer = nullptr;
||||||| merged common ancestors
void
nsBrowserStatusFilter::ProcessTimeout()
{
    mTimer = nullptr;
=======
void nsBrowserStatusFilter::CallDelayedProgressListeners() {
  mTimer = nullptr;
>>>>>>> upstream-releases

  if (!mListener) return;

  if (mDelayedStatus) {
    mDelayedStatus = false;
    MaybeSendStatus();
  }

  if (mDelayedProgress) {
    mDelayedProgress = false;
    MaybeSendProgress();
  }
}

<<<<<<< HEAD
void nsBrowserStatusFilter::TimeoutHandler(nsITimer *aTimer, void *aClosure) {
  nsBrowserStatusFilter *self =
      reinterpret_cast<nsBrowserStatusFilter *>(aClosure);
  if (!self) {
    NS_ERROR("no self");
    return;
  }
||||||| merged common ancestors
void
nsBrowserStatusFilter::TimeoutHandler(nsITimer *aTimer, void *aClosure)
{
    nsBrowserStatusFilter *self = reinterpret_cast<nsBrowserStatusFilter *>(aClosure);
    if (!self) {
        NS_ERROR("no self");
        return;
    }
=======
void nsBrowserStatusFilter::TimeoutHandler(nsITimer* aTimer, void* aClosure) {
  nsBrowserStatusFilter* self =
      reinterpret_cast<nsBrowserStatusFilter*>(aClosure);
  if (!self) {
    NS_ERROR("no self");
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  self->ProcessTimeout();
||||||| merged common ancestors
    self->ProcessTimeout();
=======
  self->CallDelayedProgressListeners();
>>>>>>> upstream-releases
}
