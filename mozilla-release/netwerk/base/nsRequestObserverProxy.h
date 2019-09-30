/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsRequestObserverProxy_h__
#define nsRequestObserverProxy_h__

#include "nsIRequestObserver.h"
#include "nsIRequestObserverProxy.h"
#include "nsIRequest.h"
#include "nsThreadUtils.h"
#include "nsCOMPtr.h"
#include "nsProxyRelease.h"

namespace mozilla {
namespace net {

class nsARequestObserverEvent;

class nsRequestObserverProxy final : public nsIRequestObserverProxy {
  ~nsRequestObserverProxy() = default;

 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIREQUESTOBSERVER
  NS_DECL_NSIREQUESTOBSERVERPROXY

  nsRequestObserverProxy() = default;

<<<<<<< HEAD
  nsIRequestObserver *Observer() { return mObserver; }
||||||| merged common ancestors
    nsIRequestObserver *Observer() { return mObserver; }
=======
  nsIRequestObserver* Observer() { return mObserver; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult FireEvent(nsARequestObserverEvent *);
||||||| merged common ancestors
    nsresult FireEvent(nsARequestObserverEvent *);
=======
  nsresult FireEvent(nsARequestObserverEvent*);
>>>>>>> upstream-releases

 protected:
  nsMainThreadPtrHandle<nsIRequestObserver> mObserver;
  nsMainThreadPtrHandle<nsISupports> mContext;

  friend class nsOnStartRequestEvent;
  friend class nsOnStopRequestEvent;
};

<<<<<<< HEAD
class nsARequestObserverEvent : public Runnable {
 public:
  explicit nsARequestObserverEvent(nsIRequest *);
||||||| merged common ancestors
class nsARequestObserverEvent : public Runnable
{
public:
    explicit nsARequestObserverEvent(nsIRequest *);
=======
class nsARequestObserverEvent : public Runnable {
 public:
  explicit nsARequestObserverEvent(nsIRequest*);
>>>>>>> upstream-releases

 protected:
  virtual ~nsARequestObserverEvent() = default;

  nsCOMPtr<nsIRequest> mRequest;
};

}  // namespace net
}  // namespace mozilla

#endif  // nsRequestObserverProxy_h__
