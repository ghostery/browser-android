/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsLoadGroup_h__
#define nsLoadGroup_h__

#include "nsILoadGroup.h"
#include "nsILoadGroupChild.h"
#include "nsCOMPtr.h"
#include "nsWeakReference.h"
#include "nsISupportsPriority.h"
#include "PLDHashTable.h"
#include "mozilla/TimeStamp.h"

class nsIRequestContext;
class nsIRequestContextService;
class nsITimedChannel;

namespace mozilla {
namespace net {

class nsLoadGroup : public nsILoadGroup,
                    public nsILoadGroupChild,
                    public nsISupportsPriority,
<<<<<<< HEAD
                    public nsSupportsWeakReference,
                    public nsPILoadGroupInternal {
 public:
  NS_DECL_AGGREGATED
||||||| merged common ancestors
                    public nsSupportsWeakReference,
                    public nsPILoadGroupInternal
{
public:
    NS_DECL_AGGREGATED
=======
                    public nsSupportsWeakReference {
 public:
  NS_DECL_ISUPPORTS
>>>>>>> upstream-releases

  ////////////////////////////////////////////////////////////////////////////
  // nsIRequest methods:
  NS_DECL_NSIREQUEST

<<<<<<< HEAD
  ////////////////////////////////////////////////////////////////////////////
  // nsILoadGroup methods:
  NS_DECL_NSILOADGROUP
  NS_DECL_NSPILOADGROUPINTERNAL
||||||| merged common ancestors
    ////////////////////////////////////////////////////////////////////////////
    // nsILoadGroup methods:
    NS_DECL_NSILOADGROUP
    NS_DECL_NSPILOADGROUPINTERNAL
=======
  ////////////////////////////////////////////////////////////////////////////
  // nsILoadGroup methods:
  NS_DECL_NSILOADGROUP
>>>>>>> upstream-releases

  ////////////////////////////////////////////////////////////////////////////
  // nsILoadGroupChild methods:
  NS_DECL_NSILOADGROUPCHILD

  ////////////////////////////////////////////////////////////////////////////
  // nsISupportsPriority methods:
  NS_DECL_NSISUPPORTSPRIORITY

  ////////////////////////////////////////////////////////////////////////////
  // nsLoadGroup methods:

<<<<<<< HEAD
  explicit nsLoadGroup(nsISupports* outer);
  virtual ~nsLoadGroup();
||||||| merged common ancestors
    explicit nsLoadGroup(nsISupports* outer);
    virtual ~nsLoadGroup();
=======
  nsLoadGroup();
>>>>>>> upstream-releases

  nsresult Init();

<<<<<<< HEAD
 protected:
  nsresult MergeLoadFlags(nsIRequest* aRequest, nsLoadFlags& flags);
  nsresult MergeDefaultLoadFlags(nsIRequest* aRequest, nsLoadFlags& flags);
||||||| merged common ancestors
protected:
    nsresult MergeLoadFlags(nsIRequest *aRequest, nsLoadFlags& flags);
    nsresult MergeDefaultLoadFlags(nsIRequest *aRequest, nsLoadFlags& flags);
=======
 protected:
  virtual ~nsLoadGroup();
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  void TelemetryReport();
  void TelemetryReportChannel(nsITimedChannel* timedChannel,
                              bool defaultRequest);
||||||| merged common ancestors
private:
    void TelemetryReport();
    void TelemetryReportChannel(nsITimedChannel *timedChannel,
                                bool defaultRequest);
=======
  nsresult MergeLoadFlags(nsIRequest* aRequest, nsLoadFlags& flags);
  nsresult MergeDefaultLoadFlags(nsIRequest* aRequest, nsLoadFlags& flags);
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  uint32_t mForegroundCount;
  uint32_t mLoadFlags;
  uint32_t mDefaultLoadFlags;
||||||| merged common ancestors
protected:
    uint32_t                        mForegroundCount;
    uint32_t                        mLoadFlags;
    uint32_t                        mDefaultLoadFlags;
=======
 private:
  void TelemetryReport();
  void TelemetryReportChannel(nsITimedChannel* timedChannel,
                              bool defaultRequest);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsILoadGroup> mLoadGroup;  // load groups can contain load groups
  nsCOMPtr<nsIInterfaceRequestor> mCallbacks;
  nsCOMPtr<nsIRequestContext> mRequestContext;
  nsCOMPtr<nsIRequestContextService> mRequestContextService;
||||||| merged common ancestors
    nsCOMPtr<nsILoadGroup>          mLoadGroup; // load groups can contain load groups
    nsCOMPtr<nsIInterfaceRequestor> mCallbacks;
    nsCOMPtr<nsIRequestContext>  mRequestContext;
    nsCOMPtr<nsIRequestContextService> mRequestContextService;
=======
 protected:
  uint32_t mForegroundCount;
  uint32_t mLoadFlags;
  uint32_t mDefaultLoadFlags;
  int32_t mPriority;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIRequest> mDefaultLoadRequest;
  PLDHashTable mRequests;
||||||| merged common ancestors
    nsCOMPtr<nsIRequest>            mDefaultLoadRequest;
    PLDHashTable                    mRequests;
=======
  nsCOMPtr<nsILoadGroup> mLoadGroup;  // load groups can contain load groups
  nsCOMPtr<nsIInterfaceRequestor> mCallbacks;
  nsCOMPtr<nsIRequestContext> mRequestContext;
  nsCOMPtr<nsIRequestContextService> mRequestContextService;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsWeakPtr mObserver;
  nsWeakPtr mParentLoadGroup;
||||||| merged common ancestors
    nsWeakPtr                       mObserver;
    nsWeakPtr                       mParentLoadGroup;
=======
  nsCOMPtr<nsIRequest> mDefaultLoadRequest;
  PLDHashTable mRequests;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult mStatus;
  int32_t mPriority;
  bool mIsCanceling;
||||||| merged common ancestors
    nsresult                        mStatus;
    int32_t                         mPriority;
    bool                            mIsCanceling;
=======
  nsWeakPtr mObserver;
  nsWeakPtr mParentLoadGroup;
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Telemetry */
  mozilla::TimeStamp mDefaultRequestCreationTime;
  bool mDefaultLoadIsTimed;
  uint32_t mTimedRequests;
  uint32_t mCachedRequests;
||||||| merged common ancestors
    /* Telemetry */
    mozilla::TimeStamp              mDefaultRequestCreationTime;
    bool                            mDefaultLoadIsTimed;
    uint32_t                        mTimedRequests;
    uint32_t                        mCachedRequests;
=======
  nsresult mStatus;
  bool mIsCanceling;
  bool mDefaultLoadIsTimed;
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* For nsPILoadGroupInternal */
  uint32_t mTimedNonCachedRequestsUntilOnEndPageLoad;
||||||| merged common ancestors
    /* For nsPILoadGroupInternal */
    uint32_t                        mTimedNonCachedRequestsUntilOnEndPageLoad;
=======
  /* Telemetry */
  mozilla::TimeStamp mDefaultRequestCreationTime;
  uint32_t mTimedRequests;
  uint32_t mCachedRequests;
>>>>>>> upstream-releases

  nsCString mUserAgentOverrideCache;
};

}  // namespace net
}  // namespace mozilla

#endif  // nsLoadGroup_h__
