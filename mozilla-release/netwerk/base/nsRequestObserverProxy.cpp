/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/DebugOnly.h"

#include "nscore.h"
#include "nsRequestObserverProxy.h"
#include "nsIRequest.h"
#include "nsAutoPtr.h"
#include "mozilla/Logging.h"
#include "mozilla/IntegerPrintfMacros.h"

namespace mozilla {
namespace net {

static LazyLogModule gRequestObserverProxyLog("nsRequestObserverProxy");

#undef LOG
#define LOG(args) MOZ_LOG(gRequestObserverProxyLog, LogLevel::Debug, args)

//-----------------------------------------------------------------------------
// nsARequestObserverEvent internal class...
//-----------------------------------------------------------------------------

<<<<<<< HEAD
nsARequestObserverEvent::nsARequestObserverEvent(nsIRequest *request)
    : Runnable("net::nsARequestObserverEvent"), mRequest(request) {
  MOZ_ASSERT(mRequest, "null pointer");
||||||| merged common ancestors
nsARequestObserverEvent::nsARequestObserverEvent(nsIRequest* request)
  : Runnable("net::nsARequestObserverEvent")
  , mRequest(request)
{
    MOZ_ASSERT(mRequest, "null pointer");
=======
nsARequestObserverEvent::nsARequestObserverEvent(nsIRequest* request)
    : Runnable("net::nsARequestObserverEvent"), mRequest(request) {
  MOZ_ASSERT(mRequest, "null pointer");
>>>>>>> upstream-releases
}

//-----------------------------------------------------------------------------
// nsOnStartRequestEvent internal class...
//-----------------------------------------------------------------------------

class nsOnStartRequestEvent : public nsARequestObserverEvent {
  RefPtr<nsRequestObserverProxy> mProxy;

<<<<<<< HEAD
 public:
  nsOnStartRequestEvent(nsRequestObserverProxy *proxy, nsIRequest *request)
      : nsARequestObserverEvent(request), mProxy(proxy) {
    MOZ_ASSERT(mProxy, "null pointer");
  }
||||||| merged common ancestors
    virtual ~nsOnStartRequestEvent() = default;
=======
 public:
  nsOnStartRequestEvent(nsRequestObserverProxy* proxy, nsIRequest* request)
      : nsARequestObserverEvent(request), mProxy(proxy) {
    MOZ_ASSERT(mProxy, "null pointer");
  }
>>>>>>> upstream-releases

  virtual ~nsOnStartRequestEvent() = default;

  NS_IMETHOD Run() override {
    LOG(("nsOnStartRequestEvent::HandleEvent [req=%p]\n", mRequest.get()));

    if (!mProxy->mObserver) {
      MOZ_ASSERT_UNREACHABLE(
          "already handled onStopRequest event "
          "(observer is null)");
      return NS_OK;
    }

<<<<<<< HEAD
    LOG(("handle startevent=%p\n", this));
    nsresult rv = mProxy->mObserver->OnStartRequest(mRequest, mProxy->mContext);
    if (NS_FAILED(rv)) {
      LOG(("OnStartRequest failed [rv=%" PRIx32 "] canceling request!\n",
           static_cast<uint32_t>(rv)));
      rv = mRequest->Cancel(rv);
      NS_ASSERTION(NS_SUCCEEDED(rv), "Cancel failed for request!");
||||||| merged common ancestors
        return NS_OK;
=======
    LOG(("handle startevent=%p\n", this));
    nsresult rv = mProxy->mObserver->OnStartRequest(mRequest);
    if (NS_FAILED(rv)) {
      LOG(("OnStartRequest failed [rv=%" PRIx32 "] canceling request!\n",
           static_cast<uint32_t>(rv)));
      rv = mRequest->Cancel(rv);
      NS_ASSERTION(NS_SUCCEEDED(rv), "Cancel failed for request!");
>>>>>>> upstream-releases
    }

    return NS_OK;
  }
};

//-----------------------------------------------------------------------------
// nsOnStopRequestEvent internal class...
//-----------------------------------------------------------------------------

<<<<<<< HEAD
class nsOnStopRequestEvent : public nsARequestObserverEvent {
  RefPtr<nsRequestObserverProxy> mProxy;
||||||| merged common ancestors
class nsOnStopRequestEvent : public nsARequestObserverEvent
{
    RefPtr<nsRequestObserverProxy> mProxy;
public:
    nsOnStopRequestEvent(nsRequestObserverProxy *proxy,
                         nsIRequest *request)
        : nsARequestObserverEvent(request)
        , mProxy(proxy)
    {
        MOZ_ASSERT(mProxy, "null pointer");
    }
=======
class nsOnStopRequestEvent : public nsARequestObserverEvent {
  RefPtr<nsRequestObserverProxy> mProxy;

 public:
  nsOnStopRequestEvent(nsRequestObserverProxy* proxy, nsIRequest* request)
      : nsARequestObserverEvent(request), mProxy(proxy) {
    MOZ_ASSERT(mProxy, "null pointer");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  nsOnStopRequestEvent(nsRequestObserverProxy *proxy, nsIRequest *request)
      : nsARequestObserverEvent(request), mProxy(proxy) {
    MOZ_ASSERT(mProxy, "null pointer");
  }
||||||| merged common ancestors
    virtual ~nsOnStopRequestEvent() = default;
=======
  virtual ~nsOnStopRequestEvent() = default;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual ~nsOnStopRequestEvent() = default;
||||||| merged common ancestors
    NS_IMETHOD Run() override
    {
        LOG(("nsOnStopRequestEvent::HandleEvent [req=%p]\n", mRequest.get()));
=======
  NS_IMETHOD Run() override {
    LOG(("nsOnStopRequestEvent::HandleEvent [req=%p]\n", mRequest.get()));
>>>>>>> upstream-releases

<<<<<<< HEAD
  NS_IMETHOD Run() override {
    LOG(("nsOnStopRequestEvent::HandleEvent [req=%p]\n", mRequest.get()));
||||||| merged common ancestors
        nsMainThreadPtrHandle<nsIRequestObserver> observer = mProxy->mObserver;
        if (!observer) {
            MOZ_ASSERT_UNREACHABLE("already handled onStopRequest event "
                                   "(observer is null)");
            return NS_OK;
        }
        // Do not allow any more events to be handled after OnStopRequest
        mProxy->mObserver = nullptr;
=======
    nsMainThreadPtrHandle<nsIRequestObserver> observer = mProxy->mObserver;
    if (!observer) {
      MOZ_ASSERT_UNREACHABLE(
          "already handled onStopRequest event "
          "(observer is null)");
      return NS_OK;
    }
    // Do not allow any more events to be handled after OnStopRequest
    mProxy->mObserver = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
    nsMainThreadPtrHandle<nsIRequestObserver> observer = mProxy->mObserver;
    if (!observer) {
      MOZ_ASSERT_UNREACHABLE(
          "already handled onStopRequest event "
          "(observer is null)");
      return NS_OK;
    }
    // Do not allow any more events to be handled after OnStopRequest
    mProxy->mObserver = nullptr;
||||||| merged common ancestors
        nsresult status = NS_OK;
        DebugOnly<nsresult> rv = mRequest->GetStatus(&status);
        NS_ASSERTION(NS_SUCCEEDED(rv), "GetStatus failed for request!");
=======
    nsresult status = NS_OK;
    DebugOnly<nsresult> rv = mRequest->GetStatus(&status);
    NS_ASSERTION(NS_SUCCEEDED(rv), "GetStatus failed for request!");
>>>>>>> upstream-releases

<<<<<<< HEAD
    nsresult status = NS_OK;
    DebugOnly<nsresult> rv = mRequest->GetStatus(&status);
    NS_ASSERTION(NS_SUCCEEDED(rv), "GetStatus failed for request!");
||||||| merged common ancestors
        LOG(("handle stopevent=%p\n", this));
        (void) observer->OnStopRequest(mRequest, mProxy->mContext, status);
=======
    LOG(("handle stopevent=%p\n", this));
    (void)observer->OnStopRequest(mRequest, status);
>>>>>>> upstream-releases

<<<<<<< HEAD
    LOG(("handle stopevent=%p\n", this));
    (void)observer->OnStopRequest(mRequest, mProxy->mContext, status);

    return NS_OK;
  }
||||||| merged common ancestors
        return NS_OK;
    }
=======
    return NS_OK;
  }
>>>>>>> upstream-releases
};

//-----------------------------------------------------------------------------
// nsRequestObserverProxy::nsISupports implementation...
//-----------------------------------------------------------------------------

NS_IMPL_ISUPPORTS(nsRequestObserverProxy, nsIRequestObserver,
                  nsIRequestObserverProxy)

//-----------------------------------------------------------------------------
// nsRequestObserverProxy::nsIRequestObserver implementation...
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
nsRequestObserverProxy::OnStartRequest(nsIRequest *request,
                                       nsISupports *context) {
  MOZ_ASSERT(!context || context == mContext);
  LOG(("nsRequestObserverProxy::OnStartRequest [this=%p req=%p]\n", this,
       request));

  nsOnStartRequestEvent *ev = new nsOnStartRequestEvent(this, request);
  if (!ev) return NS_ERROR_OUT_OF_MEMORY;

  LOG(("post startevent=%p\n", ev));
  nsresult rv = FireEvent(ev);
  if (NS_FAILED(rv)) delete ev;
  return rv;
||||||| merged common ancestors
nsRequestObserverProxy::OnStartRequest(nsIRequest *request,
                                       nsISupports *context)
{
    MOZ_ASSERT(!context || context == mContext);
    LOG(("nsRequestObserverProxy::OnStartRequest [this=%p req=%p]\n", this, request));

    nsOnStartRequestEvent *ev =
        new nsOnStartRequestEvent(this, request);
    if (!ev)
        return NS_ERROR_OUT_OF_MEMORY;

    LOG(("post startevent=%p\n", ev));
    nsresult rv = FireEvent(ev);
    if (NS_FAILED(rv))
        delete ev;
    return rv;
=======
nsRequestObserverProxy::OnStartRequest(nsIRequest* request) {
  LOG(("nsRequestObserverProxy::OnStartRequest [this=%p req=%p]\n", this,
       request));

  nsOnStartRequestEvent* ev = new nsOnStartRequestEvent(this, request);
  if (!ev) return NS_ERROR_OUT_OF_MEMORY;

  LOG(("post startevent=%p\n", ev));
  nsresult rv = FireEvent(ev);
  if (NS_FAILED(rv)) delete ev;
  return rv;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsRequestObserverProxy::OnStopRequest(nsIRequest *request, nsISupports *context,
                                      nsresult status) {
  MOZ_ASSERT(!context || context == mContext);
  LOG(("nsRequestObserverProxy: OnStopRequest [this=%p req=%p status=%" PRIx32
       "]\n",
       this, request, static_cast<uint32_t>(status)));

  // The status argument is ignored because, by the time the OnStopRequestEvent
  // is actually processed, the status of the request may have changed :-(
  // To make sure that an accurate status code is always used, GetStatus() is
  // called when the OnStopRequestEvent is actually processed (see above).

  nsOnStopRequestEvent *ev = new nsOnStopRequestEvent(this, request);
  if (!ev) return NS_ERROR_OUT_OF_MEMORY;

  LOG(("post stopevent=%p\n", ev));
  nsresult rv = FireEvent(ev);
  if (NS_FAILED(rv)) delete ev;
  return rv;
||||||| merged common ancestors
nsRequestObserverProxy::OnStopRequest(nsIRequest *request,
                                      nsISupports *context,
                                      nsresult status)
{
    MOZ_ASSERT(!context || context == mContext);
    LOG(("nsRequestObserverProxy: OnStopRequest [this=%p req=%p status=%" PRIx32 "]\n",
         this, request, static_cast<uint32_t>(status)));

    // The status argument is ignored because, by the time the OnStopRequestEvent
    // is actually processed, the status of the request may have changed :-(
    // To make sure that an accurate status code is always used, GetStatus() is
    // called when the OnStopRequestEvent is actually processed (see above).

    nsOnStopRequestEvent *ev =
        new nsOnStopRequestEvent(this, request);
    if (!ev)
        return NS_ERROR_OUT_OF_MEMORY;

    LOG(("post stopevent=%p\n", ev));
    nsresult rv = FireEvent(ev);
    if (NS_FAILED(rv))
        delete ev;
    return rv;
=======
nsRequestObserverProxy::OnStopRequest(nsIRequest* request, nsresult status) {
  LOG(("nsRequestObserverProxy: OnStopRequest [this=%p req=%p status=%" PRIx32
       "]\n",
       this, request, static_cast<uint32_t>(status)));

  // The status argument is ignored because, by the time the OnStopRequestEvent
  // is actually processed, the status of the request may have changed :-(
  // To make sure that an accurate status code is always used, GetStatus() is
  // called when the OnStopRequestEvent is actually processed (see above).

  nsOnStopRequestEvent* ev = new nsOnStopRequestEvent(this, request);
  if (!ev) return NS_ERROR_OUT_OF_MEMORY;

  LOG(("post stopevent=%p\n", ev));
  nsresult rv = FireEvent(ev);
  if (NS_FAILED(rv)) delete ev;
  return rv;
>>>>>>> upstream-releases
}

//-----------------------------------------------------------------------------
// nsRequestObserverProxy::nsIRequestObserverProxy implementation...
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
nsRequestObserverProxy::Init(nsIRequestObserver *observer,
                             nsISupports *context) {
  NS_ENSURE_ARG_POINTER(observer);
  mObserver = new nsMainThreadPtrHolder<nsIRequestObserver>(
||||||| merged common ancestors
nsRequestObserverProxy::Init(nsIRequestObserver *observer, nsISupports *context)
{
    NS_ENSURE_ARG_POINTER(observer);
    mObserver = new nsMainThreadPtrHolder<nsIRequestObserver>(
=======
nsRequestObserverProxy::Init(nsIRequestObserver* observer,
                             nsISupports* context) {
  NS_ENSURE_ARG_POINTER(observer);
  mObserver = new nsMainThreadPtrHolder<nsIRequestObserver>(
>>>>>>> upstream-releases
      "nsRequestObserverProxy::mObserver", observer);
  mContext = new nsMainThreadPtrHolder<nsISupports>(
      "nsRequestObserverProxy::mContext", context);

  return NS_OK;
}

//-----------------------------------------------------------------------------
// nsRequestObserverProxy implementation...
//-----------------------------------------------------------------------------

<<<<<<< HEAD
nsresult nsRequestObserverProxy::FireEvent(nsARequestObserverEvent *event) {
  nsCOMPtr<nsIEventTarget> mainThread(GetMainThreadEventTarget());
  return mainThread->Dispatch(event, NS_DISPATCH_NORMAL);
||||||| merged common ancestors
nsresult
nsRequestObserverProxy::FireEvent(nsARequestObserverEvent *event)
{
    nsCOMPtr<nsIEventTarget> mainThread(GetMainThreadEventTarget());
    return mainThread->Dispatch(event, NS_DISPATCH_NORMAL);
=======
nsresult nsRequestObserverProxy::FireEvent(nsARequestObserverEvent* event) {
  nsCOMPtr<nsIEventTarget> mainThread(GetMainThreadEventTarget());
  return mainThread->Dispatch(event, NS_DISPATCH_NORMAL);
>>>>>>> upstream-releases
}

}  // namespace net
}  // namespace mozilla
