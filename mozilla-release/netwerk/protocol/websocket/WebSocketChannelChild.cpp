/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=8 et tw=80 : */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "WebSocketLog.h"
#include "base/compiler_specific.h"
#include "mozilla/dom/BrowserChild.h"
#include "mozilla/net/NeckoChild.h"
#include "WebSocketChannelChild.h"
#include "nsContentUtils.h"
#include "nsIBrowserChild.h"
#include "nsNetUtil.h"
#include "mozilla/ipc/IPCStreamUtils.h"
#include "mozilla/ipc/URIUtils.h"
#include "mozilla/ipc/BackgroundUtils.h"
#include "mozilla/net/ChannelEventQueue.h"
#include "SerializedLoadContext.h"

using namespace mozilla::ipc;

namespace mozilla {
namespace net {

NS_IMPL_ADDREF(WebSocketChannelChild)

NS_IMETHODIMP_(MozExternalRefCountType) WebSocketChannelChild::Release() {
  MOZ_ASSERT(0 != mRefCnt, "dup release");
  --mRefCnt;
  NS_LOG_RELEASE(this, mRefCnt, "WebSocketChannelChild");

  if (mRefCnt == 1) {
    MaybeReleaseIPCObject();
    return mRefCnt;
  }

  if (mRefCnt == 0) {
    mRefCnt = 1; /* stabilize */
    delete this;
    return 0;
  }
  return mRefCnt;
}

NS_INTERFACE_MAP_BEGIN(WebSocketChannelChild)
  NS_INTERFACE_MAP_ENTRY(nsIWebSocketChannel)
  NS_INTERFACE_MAP_ENTRY(nsIProtocolHandler)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIWebSocketChannel)
  NS_INTERFACE_MAP_ENTRY(nsIThreadRetargetableRequest)
NS_INTERFACE_MAP_END

WebSocketChannelChild::WebSocketChannelChild(bool aEncrypted)
    : NeckoTargetHolder(nullptr),
      mIPCState(Closed),
      mMutex("WebSocketChannelChild::mMutex") {
  MOZ_ASSERT(NS_IsMainThread(), "not main thread");

  LOG(("WebSocketChannelChild::WebSocketChannelChild() %p\n", this));
  mEncrypted = aEncrypted;
  mEventQ = new ChannelEventQueue(static_cast<nsIWebSocketChannel*>(this));
}

WebSocketChannelChild::~WebSocketChannelChild() {
  LOG(("WebSocketChannelChild::~WebSocketChannelChild() %p\n", this));
}

void WebSocketChannelChild::AddIPDLReference() {
  MOZ_ASSERT(NS_IsMainThread());

  {
    MutexAutoLock lock(mMutex);
    MOZ_ASSERT(mIPCState == Closed,
               "Attempt to retain more than one IPDL reference");
    mIPCState = Opened;
  }

  AddRef();
}

void WebSocketChannelChild::ReleaseIPDLReference() {
  MOZ_ASSERT(NS_IsMainThread());

  {
    MutexAutoLock lock(mMutex);
    MOZ_ASSERT(mIPCState != Closed,
               "Attempt to release nonexistent IPDL reference");
    mIPCState = Closed;
  }

  Release();
}

void WebSocketChannelChild::MaybeReleaseIPCObject() {
  {
    MutexAutoLock lock(mMutex);
    if (mIPCState != Opened) {
      return;
    }

    mIPCState = Closing;
  }

  if (!NS_IsMainThread()) {
    nsCOMPtr<nsIEventTarget> target = GetNeckoTarget();
    MOZ_ALWAYS_SUCCEEDS(target->Dispatch(
        NewRunnableMethod("WebSocketChannelChild::MaybeReleaseIPCObject", this,
                          &WebSocketChannelChild::MaybeReleaseIPCObject),
        NS_DISPATCH_NORMAL));
    return;
  }

  SendDeleteSelf();
}

void WebSocketChannelChild::GetEffectiveURL(nsAString& aEffectiveURL) const {
  aEffectiveURL = mEffectiveURL;
}

bool WebSocketChannelChild::IsEncrypted() const { return mEncrypted; }

<<<<<<< HEAD
class WrappedChannelEvent : public Runnable {
 public:
  explicit WrappedChannelEvent(ChannelEvent* aChannelEvent)
      : Runnable("net::WrappedChannelEvent"), mChannelEvent(aChannelEvent) {
    MOZ_RELEASE_ASSERT(aChannelEvent);
  }
  NS_IMETHOD Run() override {
    mChannelEvent->Run();
    return NS_OK;
  }

 private:
  nsAutoPtr<ChannelEvent> mChannelEvent;
||||||| merged common ancestors
class WrappedChannelEvent : public Runnable
{
public:
  explicit WrappedChannelEvent(ChannelEvent* aChannelEvent)
    : Runnable("net::WrappedChannelEvent")
    , mChannelEvent(aChannelEvent)
  {
    MOZ_RELEASE_ASSERT(aChannelEvent);
  }
  NS_IMETHOD Run() override
  {
    mChannelEvent->Run();
    return NS_OK;
  }
private:
  nsAutoPtr<ChannelEvent> mChannelEvent;
=======
class WebSocketEvent {
 public:
  WebSocketEvent() { MOZ_COUNT_CTOR(WebSocketEvent); }
  virtual ~WebSocketEvent() { MOZ_COUNT_DTOR(WebSocketEvent); }
  virtual void Run(WebSocketChannelChild* aChild) = 0;
>>>>>>> upstream-releases
};

<<<<<<< HEAD
void WebSocketChannelChild::DispatchToTargetThread(
    ChannelEvent* aChannelEvent) {
  MOZ_RELEASE_ASSERT(NS_IsMainThread());
  MOZ_RELEASE_ASSERT(mTargetThread);
  MOZ_RELEASE_ASSERT(aChannelEvent);
||||||| merged common ancestors
void
WebSocketChannelChild::DispatchToTargetThread(ChannelEvent *aChannelEvent)
{
  MOZ_RELEASE_ASSERT(NS_IsMainThread());
  MOZ_RELEASE_ASSERT(mTargetThread);
  MOZ_RELEASE_ASSERT(aChannelEvent);
=======
class WrappedWebSocketEvent : public Runnable {
 public:
  WrappedWebSocketEvent(WebSocketChannelChild* aChild,
                        WebSocketEvent* aWebSocketEvent)
      : Runnable("net::WrappedWebSocketEvent"),
        mChild(aChild),
        mWebSocketEvent(aWebSocketEvent) {
    MOZ_RELEASE_ASSERT(aWebSocketEvent);
  }
  NS_IMETHOD Run() override {
    mWebSocketEvent->Run(mChild);
    return NS_OK;
  }
>>>>>>> upstream-releases

 private:
  RefPtr<WebSocketChannelChild> mChild;
  nsAutoPtr<WebSocketEvent> mWebSocketEvent;
};

<<<<<<< HEAD
class EventTargetDispatcher : public ChannelEvent {
 public:
  EventTargetDispatcher(ChannelEvent* aChannelEvent,
||||||| merged common ancestors
class EventTargetDispatcher : public ChannelEvent
{
public:
  EventTargetDispatcher(ChannelEvent* aChannelEvent,
=======
class EventTargetDispatcher : public ChannelEvent {
 public:
  EventTargetDispatcher(WebSocketChannelChild* aChild,
                        WebSocketEvent* aWebSocketEvent,
>>>>>>> upstream-releases
                        nsIEventTarget* aEventTarget)
<<<<<<< HEAD
      : mChannelEvent(aChannelEvent), mEventTarget(aEventTarget) {}
||||||| merged common ancestors
    : mChannelEvent(aChannelEvent)
    , mEventTarget(aEventTarget)
  {}
=======
      : mChild(aChild),
        mWebSocketEvent(aWebSocketEvent),
        mEventTarget(aEventTarget) {}
>>>>>>> upstream-releases

  void Run() override {
    if (mEventTarget) {
      mEventTarget->Dispatch(
          new WrappedWebSocketEvent(mChild, mWebSocketEvent.forget()),
          NS_DISPATCH_NORMAL);
      return;
    }

    mWebSocketEvent->Run(mChild);
  }

  already_AddRefed<nsIEventTarget> GetEventTarget() override {
    nsCOMPtr<nsIEventTarget> target = mEventTarget;
    if (!target) {
      target = GetMainThreadEventTarget();
    }
    return target.forget();
  }

<<<<<<< HEAD
 private:
  nsAutoPtr<ChannelEvent> mChannelEvent;
||||||| merged common ancestors
private:
  nsAutoPtr<ChannelEvent> mChannelEvent;
=======
 private:
  // The lifetime of the child is ensured by ChannelEventQueue.
  WebSocketChannelChild* mChild;
  nsAutoPtr<WebSocketEvent> mWebSocketEvent;
>>>>>>> upstream-releases
  nsCOMPtr<nsIEventTarget> mEventTarget;
};

<<<<<<< HEAD
class StartEvent : public ChannelEvent {
||||||| merged common ancestors
class StartEvent : public ChannelEvent
{
=======
class StartEvent : public WebSocketEvent {
>>>>>>> upstream-releases
 public:
<<<<<<< HEAD
  StartEvent(WebSocketChannelChild* aChild, const nsCString& aProtocol,
             const nsCString& aExtensions, const nsString& aEffectiveURL,
             bool aEncrypted)
      : mChild(aChild),
        mProtocol(aProtocol),
        mExtensions(aExtensions),
        mEffectiveURL(aEffectiveURL),
        mEncrypted(aEncrypted) {}

  void Run() override {
    mChild->OnStart(mProtocol, mExtensions, mEffectiveURL, mEncrypted);
  }
||||||| merged common ancestors
  StartEvent(WebSocketChannelChild* aChild,
             const nsCString& aProtocol,
             const nsCString& aExtensions,
             const nsString& aEffectiveURL,
             bool aEncrypted)
  : mChild(aChild)
  , mProtocol(aProtocol)
  , mExtensions(aExtensions)
  , mEffectiveURL(aEffectiveURL)
  , mEncrypted(aEncrypted)
  {}

  void Run() override
  {
    mChild->OnStart(mProtocol, mExtensions, mEffectiveURL, mEncrypted);
  }
=======
  StartEvent(const nsCString& aProtocol, const nsCString& aExtensions,
             const nsString& aEffectiveURL, bool aEncrypted,
             uint64_t aHttpChannelId)
      : mProtocol(aProtocol),
        mExtensions(aExtensions),
        mEffectiveURL(aEffectiveURL),
        mEncrypted(aEncrypted),
        mHttpChannelId(aHttpChannelId) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  already_AddRefed<nsIEventTarget> GetEventTarget() override {
    return do_AddRef(GetCurrentThreadEventTarget());
||||||| merged common ancestors
  already_AddRefed<nsIEventTarget> GetEventTarget() override
  {
    return do_AddRef(GetCurrentThreadEventTarget());
=======
  void Run(WebSocketChannelChild* aChild) override {
    aChild->OnStart(mProtocol, mExtensions, mEffectiveURL, mEncrypted,
                    mHttpChannelId);
>>>>>>> upstream-releases
  }

 private:
  nsCString mProtocol;
  nsCString mExtensions;
  nsString mEffectiveURL;
  bool mEncrypted;
  uint64_t mHttpChannelId;
};

<<<<<<< HEAD
mozilla::ipc::IPCResult WebSocketChannelChild::RecvOnStart(
    const nsCString& aProtocol, const nsCString& aExtensions,
    const nsString& aEffectiveURL, const bool& aEncrypted) {
  mEventQ->RunOrEnqueue(new EventTargetDispatcher(
      new StartEvent(this, aProtocol, aExtensions, aEffectiveURL, aEncrypted),
      mTargetThread));
||||||| merged common ancestors
mozilla::ipc::IPCResult
WebSocketChannelChild::RecvOnStart(const nsCString& aProtocol,
                                   const nsCString& aExtensions,
                                   const nsString& aEffectiveURL,
                                   const bool& aEncrypted)
{
  mEventQ->RunOrEnqueue(
    new EventTargetDispatcher(new StartEvent(this, aProtocol, aExtensions,
                                             aEffectiveURL, aEncrypted),
                              mTargetThread));
=======
mozilla::ipc::IPCResult WebSocketChannelChild::RecvOnStart(
    const nsCString& aProtocol, const nsCString& aExtensions,
    const nsString& aEffectiveURL, const bool& aEncrypted,
    const uint64_t& aHttpChannelId) {
  mEventQ->RunOrEnqueue(new EventTargetDispatcher(
      this,
      new StartEvent(aProtocol, aExtensions, aEffectiveURL, aEncrypted,
                     aHttpChannelId),
      mTargetThread));
>>>>>>> upstream-releases

  return IPC_OK();
}

<<<<<<< HEAD
void WebSocketChannelChild::OnStart(const nsCString& aProtocol,
                                    const nsCString& aExtensions,
                                    const nsString& aEffectiveURL,
                                    const bool& aEncrypted) {
||||||| merged common ancestors
void
WebSocketChannelChild::OnStart(const nsCString& aProtocol,
                               const nsCString& aExtensions,
                               const nsString& aEffectiveURL,
                               const bool& aEncrypted)
{
=======
void WebSocketChannelChild::OnStart(const nsCString& aProtocol,
                                    const nsCString& aExtensions,
                                    const nsString& aEffectiveURL,
                                    const bool& aEncrypted,
                                    const uint64_t& aHttpChannelId) {
>>>>>>> upstream-releases
  LOG(("WebSocketChannelChild::RecvOnStart() %p\n", this));
  SetProtocol(aProtocol);
  mNegotiatedExtensions = aExtensions;
  mEffectiveURL = aEffectiveURL;
  mEncrypted = aEncrypted;
  mHttpChannelId = aHttpChannelId;

  if (mListenerMT) {
    AutoEventEnqueuer ensureSerialDispatch(mEventQ);
    nsresult rv = mListenerMT->mListener->OnStart(mListenerMT->mContext);
    if (NS_FAILED(rv)) {
      LOG(
          ("WebSocketChannelChild::OnStart "
           "mListenerMT->mListener->OnStart() failed with error 0x%08" PRIx32,
           static_cast<uint32_t>(rv)));
    }
  }
}

<<<<<<< HEAD
class StopEvent : public ChannelEvent {
||||||| merged common ancestors
class StopEvent : public ChannelEvent
{
=======
class StopEvent : public WebSocketEvent {
>>>>>>> upstream-releases
 public:
<<<<<<< HEAD
  StopEvent(WebSocketChannelChild* aChild, const nsresult& aStatusCode)
      : mChild(aChild), mStatusCode(aStatusCode) {}

  void Run() override { mChild->OnStop(mStatusCode); }
||||||| merged common ancestors
  StopEvent(WebSocketChannelChild* aChild,
            const nsresult& aStatusCode)
  : mChild(aChild)
  , mStatusCode(aStatusCode)
  {}

  void Run() override
  {
    mChild->OnStop(mStatusCode);
  }
=======
  explicit StopEvent(const nsresult& aStatusCode) : mStatusCode(aStatusCode) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  already_AddRefed<nsIEventTarget> GetEventTarget() override {
    return do_AddRef(GetCurrentThreadEventTarget());
||||||| merged common ancestors
  already_AddRefed<nsIEventTarget> GetEventTarget() override
  {
    return do_AddRef(GetCurrentThreadEventTarget());
=======
  void Run(WebSocketChannelChild* aChild) override {
    aChild->OnStop(mStatusCode);
>>>>>>> upstream-releases
  }

 private:
  nsresult mStatusCode;
};

<<<<<<< HEAD
mozilla::ipc::IPCResult WebSocketChannelChild::RecvOnStop(
    const nsresult& aStatusCode) {
  mEventQ->RunOrEnqueue(new EventTargetDispatcher(
      new StopEvent(this, aStatusCode), mTargetThread));
||||||| merged common ancestors
mozilla::ipc::IPCResult
WebSocketChannelChild::RecvOnStop(const nsresult& aStatusCode)
{
  mEventQ->RunOrEnqueue(
    new EventTargetDispatcher(new StopEvent(this, aStatusCode),
                              mTargetThread));
=======
mozilla::ipc::IPCResult WebSocketChannelChild::RecvOnStop(
    const nsresult& aStatusCode) {
  mEventQ->RunOrEnqueue(new EventTargetDispatcher(
      this, new StopEvent(aStatusCode), mTargetThread));
>>>>>>> upstream-releases

  return IPC_OK();
}

void WebSocketChannelChild::OnStop(const nsresult& aStatusCode) {
  LOG(("WebSocketChannelChild::RecvOnStop() %p\n", this));
  if (mListenerMT) {
    AutoEventEnqueuer ensureSerialDispatch(mEventQ);
    nsresult rv =
        mListenerMT->mListener->OnStop(mListenerMT->mContext, aStatusCode);
    if (NS_FAILED(rv)) {
      LOG(
          ("WebSocketChannel::OnStop "
           "mListenerMT->mListener->OnStop() failed with error 0x%08" PRIx32,
           static_cast<uint32_t>(rv)));
    }
  }
}

<<<<<<< HEAD
class MessageEvent : public ChannelEvent {
||||||| merged common ancestors
class MessageEvent : public ChannelEvent
{
=======
class MessageEvent : public WebSocketEvent {
>>>>>>> upstream-releases
 public:
<<<<<<< HEAD
  MessageEvent(WebSocketChannelChild* aChild, const nsCString& aMessage,
               bool aBinary)
      : mChild(aChild), mMessage(aMessage), mBinary(aBinary) {}

  void Run() override {
||||||| merged common ancestors
  MessageEvent(WebSocketChannelChild* aChild,
               const nsCString& aMessage,
               bool aBinary)
  : mChild(aChild)
  , mMessage(aMessage)
  , mBinary(aBinary)
  {}

  void Run() override
  {
=======
  MessageEvent(const nsCString& aMessage, bool aBinary)
      : mMessage(aMessage), mBinary(aBinary) {}

  void Run(WebSocketChannelChild* aChild) override {
>>>>>>> upstream-releases
    if (!mBinary) {
      aChild->OnMessageAvailable(mMessage);
    } else {
      aChild->OnBinaryMessageAvailable(mMessage);
    }
  }

<<<<<<< HEAD
  already_AddRefed<nsIEventTarget> GetEventTarget() override {
    return do_AddRef(GetCurrentThreadEventTarget());
  }

||||||| merged common ancestors
  already_AddRefed<nsIEventTarget> GetEventTarget() override
  {
    return do_AddRef(GetCurrentThreadEventTarget());
  }

=======
>>>>>>> upstream-releases
 private:
  nsCString mMessage;
  bool mBinary;
};

<<<<<<< HEAD
mozilla::ipc::IPCResult WebSocketChannelChild::RecvOnMessageAvailable(
    const nsCString& aMsg) {
  mEventQ->RunOrEnqueue(new EventTargetDispatcher(
      new MessageEvent(this, aMsg, false), mTargetThread));
||||||| merged common ancestors
mozilla::ipc::IPCResult
WebSocketChannelChild::RecvOnMessageAvailable(const nsCString& aMsg)
{
  mEventQ->RunOrEnqueue(
    new EventTargetDispatcher(new MessageEvent(this, aMsg, false),
                              mTargetThread));
=======
mozilla::ipc::IPCResult WebSocketChannelChild::RecvOnMessageAvailable(
    const nsCString& aMsg) {
  mEventQ->RunOrEnqueue(new EventTargetDispatcher(
      this, new MessageEvent(aMsg, false), mTargetThread));
>>>>>>> upstream-releases

  return IPC_OK();
}

void WebSocketChannelChild::OnMessageAvailable(const nsCString& aMsg) {
  LOG(("WebSocketChannelChild::RecvOnMessageAvailable() %p\n", this));
  if (mListenerMT) {
    AutoEventEnqueuer ensureSerialDispatch(mEventQ);
    nsresult rv =
        mListenerMT->mListener->OnMessageAvailable(mListenerMT->mContext, aMsg);
    if (NS_FAILED(rv)) {
      LOG(
          ("WebSocketChannelChild::OnMessageAvailable "
           "mListenerMT->mListener->OnMessageAvailable() "
           "failed with error 0x%08" PRIx32,
           static_cast<uint32_t>(rv)));
    }
  }
}

<<<<<<< HEAD
mozilla::ipc::IPCResult WebSocketChannelChild::RecvOnBinaryMessageAvailable(
    const nsCString& aMsg) {
  mEventQ->RunOrEnqueue(new EventTargetDispatcher(
      new MessageEvent(this, aMsg, true), mTargetThread));
||||||| merged common ancestors
mozilla::ipc::IPCResult
WebSocketChannelChild::RecvOnBinaryMessageAvailable(const nsCString& aMsg)
{
  mEventQ->RunOrEnqueue(
    new EventTargetDispatcher(new MessageEvent(this, aMsg, true),
                              mTargetThread));
=======
mozilla::ipc::IPCResult WebSocketChannelChild::RecvOnBinaryMessageAvailable(
    const nsCString& aMsg) {
  mEventQ->RunOrEnqueue(new EventTargetDispatcher(
      this, new MessageEvent(aMsg, true), mTargetThread));
>>>>>>> upstream-releases

  return IPC_OK();
}

void WebSocketChannelChild::OnBinaryMessageAvailable(const nsCString& aMsg) {
  LOG(("WebSocketChannelChild::RecvOnBinaryMessageAvailable() %p\n", this));
  if (mListenerMT) {
    AutoEventEnqueuer ensureSerialDispatch(mEventQ);
    nsresult rv = mListenerMT->mListener->OnBinaryMessageAvailable(
        mListenerMT->mContext, aMsg);
    if (NS_FAILED(rv)) {
      LOG(
          ("WebSocketChannelChild::OnBinaryMessageAvailable "
           "mListenerMT->mListener->OnBinaryMessageAvailable() "
           "failed with error 0x%08" PRIx32,
           static_cast<uint32_t>(rv)));
    }
  }
}

<<<<<<< HEAD
class AcknowledgeEvent : public ChannelEvent {
||||||| merged common ancestors
class AcknowledgeEvent : public ChannelEvent
{
=======
class AcknowledgeEvent : public WebSocketEvent {
>>>>>>> upstream-releases
 public:
<<<<<<< HEAD
  AcknowledgeEvent(WebSocketChannelChild* aChild, const uint32_t& aSize)
      : mChild(aChild), mSize(aSize) {}

  void Run() override { mChild->OnAcknowledge(mSize); }
||||||| merged common ancestors
  AcknowledgeEvent(WebSocketChannelChild* aChild,
                   const uint32_t& aSize)
  : mChild(aChild)
  , mSize(aSize)
  {}

  void Run() override
  {
    mChild->OnAcknowledge(mSize);
  }
=======
  explicit AcknowledgeEvent(const uint32_t& aSize) : mSize(aSize) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  already_AddRefed<nsIEventTarget> GetEventTarget() override {
    return do_AddRef(GetCurrentThreadEventTarget());
||||||| merged common ancestors
  already_AddRefed<nsIEventTarget> GetEventTarget() override
  {
    return do_AddRef(GetCurrentThreadEventTarget());
=======
  void Run(WebSocketChannelChild* aChild) override {
    aChild->OnAcknowledge(mSize);
>>>>>>> upstream-releases
  }

 private:
  uint32_t mSize;
};

<<<<<<< HEAD
mozilla::ipc::IPCResult WebSocketChannelChild::RecvOnAcknowledge(
    const uint32_t& aSize) {
  mEventQ->RunOrEnqueue(new EventTargetDispatcher(
      new AcknowledgeEvent(this, aSize), mTargetThread));
||||||| merged common ancestors
mozilla::ipc::IPCResult
WebSocketChannelChild::RecvOnAcknowledge(const uint32_t& aSize)
{
  mEventQ->RunOrEnqueue(
    new EventTargetDispatcher(new AcknowledgeEvent(this, aSize),
                              mTargetThread));
=======
mozilla::ipc::IPCResult WebSocketChannelChild::RecvOnAcknowledge(
    const uint32_t& aSize) {
  mEventQ->RunOrEnqueue(new EventTargetDispatcher(
      this, new AcknowledgeEvent(aSize), mTargetThread));
>>>>>>> upstream-releases

  return IPC_OK();
}

void WebSocketChannelChild::OnAcknowledge(const uint32_t& aSize) {
  LOG(("WebSocketChannelChild::RecvOnAcknowledge() %p\n", this));
  if (mListenerMT) {
    AutoEventEnqueuer ensureSerialDispatch(mEventQ);
    nsresult rv =
        mListenerMT->mListener->OnAcknowledge(mListenerMT->mContext, aSize);
    if (NS_FAILED(rv)) {
      LOG(
          ("WebSocketChannel::OnAcknowledge "
           "mListenerMT->mListener->OnAcknowledge() "
           "failed with error 0x%08" PRIx32,
           static_cast<uint32_t>(rv)));
    }
  }
}

<<<<<<< HEAD
class ServerCloseEvent : public ChannelEvent {
||||||| merged common ancestors
class ServerCloseEvent : public ChannelEvent
{
=======
class ServerCloseEvent : public WebSocketEvent {
>>>>>>> upstream-releases
 public:
<<<<<<< HEAD
  ServerCloseEvent(WebSocketChannelChild* aChild, const uint16_t aCode,
                   const nsCString& aReason)
      : mChild(aChild), mCode(aCode), mReason(aReason) {}
||||||| merged common ancestors
  ServerCloseEvent(WebSocketChannelChild* aChild,
                   const uint16_t aCode,
                   const nsCString &aReason)
  : mChild(aChild)
  , mCode(aCode)
  , mReason(aReason)
  {}

  void Run() override
  {
    mChild->OnServerClose(mCode, mReason);
  }
=======
  ServerCloseEvent(const uint16_t aCode, const nsCString& aReason)
      : mCode(aCode), mReason(aReason) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Run() override { mChild->OnServerClose(mCode, mReason); }

  already_AddRefed<nsIEventTarget> GetEventTarget() override {
    return do_AddRef(GetCurrentThreadEventTarget());
||||||| merged common ancestors
  already_AddRefed<nsIEventTarget> GetEventTarget() override
  {
    return do_AddRef(GetCurrentThreadEventTarget());
=======
  void Run(WebSocketChannelChild* aChild) override {
    aChild->OnServerClose(mCode, mReason);
>>>>>>> upstream-releases
  }

 private:
  uint16_t mCode;
  nsCString mReason;
};

<<<<<<< HEAD
mozilla::ipc::IPCResult WebSocketChannelChild::RecvOnServerClose(
    const uint16_t& aCode, const nsCString& aReason) {
  mEventQ->RunOrEnqueue(new EventTargetDispatcher(
      new ServerCloseEvent(this, aCode, aReason), mTargetThread));
||||||| merged common ancestors
mozilla::ipc::IPCResult
WebSocketChannelChild::RecvOnServerClose(const uint16_t& aCode,
                                         const nsCString& aReason)
{
  mEventQ->RunOrEnqueue(
    new EventTargetDispatcher(new ServerCloseEvent(this, aCode, aReason),
                              mTargetThread));
=======
mozilla::ipc::IPCResult WebSocketChannelChild::RecvOnServerClose(
    const uint16_t& aCode, const nsCString& aReason) {
  mEventQ->RunOrEnqueue(new EventTargetDispatcher(
      this, new ServerCloseEvent(aCode, aReason), mTargetThread));
>>>>>>> upstream-releases

  return IPC_OK();
}

void WebSocketChannelChild::OnServerClose(const uint16_t& aCode,
                                          const nsCString& aReason) {
  LOG(("WebSocketChannelChild::RecvOnServerClose() %p\n", this));
  if (mListenerMT) {
    AutoEventEnqueuer ensureSerialDispatch(mEventQ);
    DebugOnly<nsresult> rv = mListenerMT->mListener->OnServerClose(
        mListenerMT->mContext, aCode, aReason);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
  }
}

void WebSocketChannelChild::SetupNeckoTarget() {
  mNeckoTarget = nsContentUtils::GetEventTargetByLoadInfo(
      mLoadInfo, TaskCategory::Network);
  if (!mNeckoTarget) {
    return;
  }

  gNeckoChild->SetEventTargetForActor(this, mNeckoTarget);
}

NS_IMETHODIMP
WebSocketChannelChild::AsyncOpen(nsIURI* aURI, const nsACString& aOrigin,
                                 uint64_t aInnerWindowID,
                                 nsIWebSocketListener* aListener,
                                 nsISupports* aContext) {
  LOG(("WebSocketChannelChild::AsyncOpen() %p\n", this));

  MOZ_ASSERT(NS_IsMainThread(), "not main thread");
  MOZ_ASSERT((aURI && !mIsServerSide) || (!aURI && mIsServerSide),
             "Invalid aURI for WebSocketChannelChild::AsyncOpen");
  MOZ_ASSERT(aListener && !mListenerMT,
             "Invalid state for WebSocketChannelChild::AsyncOpen");

<<<<<<< HEAD
  mozilla::dom::TabChild* tabChild = nullptr;
  nsCOMPtr<nsITabChild> iTabChild;
  NS_QueryNotificationCallbacks(mCallbacks, mLoadGroup, NS_GET_IID(nsITabChild),
                                getter_AddRefs(iTabChild));
  if (iTabChild) {
    tabChild = static_cast<mozilla::dom::TabChild*>(iTabChild.get());
||||||| merged common ancestors
  mozilla::dom::TabChild* tabChild = nullptr;
  nsCOMPtr<nsITabChild> iTabChild;
  NS_QueryNotificationCallbacks(mCallbacks, mLoadGroup,
                                NS_GET_IID(nsITabChild),
                                getter_AddRefs(iTabChild));
  if (iTabChild) {
    tabChild = static_cast<mozilla::dom::TabChild*>(iTabChild.get());
=======
  mozilla::dom::BrowserChild* browserChild = nullptr;
  nsCOMPtr<nsIBrowserChild> iBrowserChild;
  NS_QueryNotificationCallbacks(mCallbacks, mLoadGroup,
                                NS_GET_IID(nsIBrowserChild),
                                getter_AddRefs(iBrowserChild));
  if (iBrowserChild) {
    browserChild =
        static_cast<mozilla::dom::BrowserChild*>(iBrowserChild.get());
>>>>>>> upstream-releases
  }
  if (MissingRequiredBrowserChild(browserChild, "websocket")) {
    return NS_ERROR_ILLEGAL_VALUE;
  }

  ContentChild* cc = static_cast<ContentChild*>(gNeckoChild->Manager());
  if (cc->IsShuttingDown()) {
    return NS_ERROR_FAILURE;
  }

  // Corresponding release in DeallocPWebSocket
  AddIPDLReference();

  Maybe<URIParams> uri;
  Maybe<LoadInfoArgs> loadInfoArgs;
  Maybe<PTransportProviderChild*> transportProvider;

  if (!mIsServerSide) {
    uri.emplace(URIParams());
    SerializeURI(aURI, uri.ref());
    nsresult rv = LoadInfoToLoadInfoArgs(mLoadInfo, &loadInfoArgs);
    NS_ENSURE_SUCCESS(rv, rv);

    transportProvider = Nothing();
  } else {
    MOZ_ASSERT(mServerTransportProvider);
    PTransportProviderChild* ipcChild;
    nsresult rv = mServerTransportProvider->GetIPCChild(&ipcChild);
    NS_ENSURE_SUCCESS(rv, rv);

    transportProvider = Some(ipcChild);
  }

  // This must be called before sending constructor message.
  SetupNeckoTarget();

<<<<<<< HEAD
  gNeckoChild->SendPWebSocketConstructor(
      this, tabChild, IPC::SerializedLoadContext(this), mSerial);
||||||| merged common ancestors
  gNeckoChild->SendPWebSocketConstructor(this, tabChild,
                                         IPC::SerializedLoadContext(this),
                                         mSerial);
=======
  gNeckoChild->SendPWebSocketConstructor(
      this, browserChild, IPC::SerializedLoadContext(this), mSerial);
>>>>>>> upstream-releases
  if (!SendAsyncOpen(uri, nsCString(aOrigin), aInnerWindowID, mProtocol,
                     mEncrypted, mPingInterval, mClientSetPingInterval,
                     mPingResponseTimeout, mClientSetPingTimeout, loadInfoArgs,
                     transportProvider, mNegotiatedExtensions)) {
    return NS_ERROR_UNEXPECTED;
  }

  if (mIsServerSide) {
    mServerTransportProvider = nullptr;
  }

  mOriginalURI = aURI;
  mURI = mOriginalURI;
  mListenerMT = new ListenerAndContextContainer(aListener, aContext);
  mOrigin = aOrigin;
  mWasOpened = 1;

  return NS_OK;
}

class CloseEvent : public Runnable {
 public:
  CloseEvent(WebSocketChannelChild* aChild, uint16_t aCode,
             const nsACString& aReason)
      : Runnable("net::CloseEvent"),
        mChild(aChild),
        mCode(aCode),
        mReason(aReason) {
    MOZ_RELEASE_ASSERT(!NS_IsMainThread());
    MOZ_ASSERT(aChild);
  }
  NS_IMETHOD Run() override {
    MOZ_RELEASE_ASSERT(NS_IsMainThread());
    mChild->Close(mCode, mReason);
    return NS_OK;
  }

 private:
  RefPtr<WebSocketChannelChild> mChild;
  uint16_t mCode;
  nsCString mReason;
};

NS_IMETHODIMP
WebSocketChannelChild::Close(uint16_t code, const nsACString& reason) {
  if (!NS_IsMainThread()) {
    MOZ_RELEASE_ASSERT(mTargetThread->IsOnCurrentThread());
    nsCOMPtr<nsIEventTarget> target = GetNeckoTarget();
    return target->Dispatch(new CloseEvent(this, code, reason),
                            NS_DISPATCH_NORMAL);
  }
  LOG(("WebSocketChannelChild::Close() %p\n", this));

  {
    MutexAutoLock lock(mMutex);
    if (mIPCState != Opened) {
      return NS_ERROR_UNEXPECTED;
    }
  }

  if (!SendClose(code, nsCString(reason))) {
    return NS_ERROR_UNEXPECTED;
  }

  return NS_OK;
}

class MsgEvent : public Runnable {
 public:
  MsgEvent(WebSocketChannelChild* aChild, const nsACString& aMsg,
           bool aBinaryMsg)
      : Runnable("net::MsgEvent"),
        mChild(aChild),
        mMsg(aMsg),
        mBinaryMsg(aBinaryMsg) {
    MOZ_RELEASE_ASSERT(!NS_IsMainThread());
    MOZ_ASSERT(aChild);
  }
  NS_IMETHOD Run() override {
    MOZ_RELEASE_ASSERT(NS_IsMainThread());
    if (mBinaryMsg) {
      mChild->SendBinaryMsg(mMsg);
    } else {
      mChild->SendMsg(mMsg);
    }
    return NS_OK;
  }

 private:
  RefPtr<WebSocketChannelChild> mChild;
  nsCString mMsg;
  bool mBinaryMsg;
};

NS_IMETHODIMP
WebSocketChannelChild::SendMsg(const nsACString& aMsg) {
  if (!NS_IsMainThread()) {
    MOZ_RELEASE_ASSERT(IsOnTargetThread());
    nsCOMPtr<nsIEventTarget> target = GetNeckoTarget();
    return target->Dispatch(new MsgEvent(this, aMsg, false),
                            NS_DISPATCH_NORMAL);
  }
  LOG(("WebSocketChannelChild::SendMsg() %p\n", this));

  {
    MutexAutoLock lock(mMutex);
    if (mIPCState != Opened) {
      return NS_ERROR_UNEXPECTED;
    }
  }

  if (!SendSendMsg(nsCString(aMsg))) {
    return NS_ERROR_UNEXPECTED;
  }

  return NS_OK;
}

NS_IMETHODIMP
WebSocketChannelChild::SendBinaryMsg(const nsACString& aMsg) {
  if (!NS_IsMainThread()) {
    MOZ_RELEASE_ASSERT(IsOnTargetThread());
    nsCOMPtr<nsIEventTarget> target = GetNeckoTarget();
    return target->Dispatch(new MsgEvent(this, aMsg, true), NS_DISPATCH_NORMAL);
  }
  LOG(("WebSocketChannelChild::SendBinaryMsg() %p\n", this));

  {
    MutexAutoLock lock(mMutex);
    if (mIPCState != Opened) {
      return NS_ERROR_UNEXPECTED;
    }
  }

  if (!SendSendBinaryMsg(nsCString(aMsg))) {
    return NS_ERROR_UNEXPECTED;
  }

  return NS_OK;
}

class BinaryStreamEvent : public Runnable {
 public:
  BinaryStreamEvent(WebSocketChannelChild* aChild, nsIInputStream* aStream,
                    uint32_t aLength)
      : Runnable("net::BinaryStreamEvent"),
        mChild(aChild),
        mStream(aStream),
        mLength(aLength) {
    MOZ_RELEASE_ASSERT(!NS_IsMainThread());
    MOZ_ASSERT(aChild);
  }
  NS_IMETHOD Run() override {
    MOZ_ASSERT(NS_IsMainThread());
    nsresult rv = mChild->SendBinaryStream(mStream, mLength);
    if (NS_FAILED(rv)) {
      LOG(
          ("WebSocketChannelChild::BinaryStreamEvent %p "
           "SendBinaryStream failed (%08" PRIx32 ")\n",
           this, static_cast<uint32_t>(rv)));
    }
    return NS_OK;
  }

 private:
  RefPtr<WebSocketChannelChild> mChild;
  nsCOMPtr<nsIInputStream> mStream;
  uint32_t mLength;
};

NS_IMETHODIMP
WebSocketChannelChild::SendBinaryStream(nsIInputStream* aStream,
                                        uint32_t aLength) {
  if (!NS_IsMainThread()) {
    MOZ_RELEASE_ASSERT(mTargetThread->IsOnCurrentThread());
    nsCOMPtr<nsIEventTarget> target = GetNeckoTarget();
    return target->Dispatch(new BinaryStreamEvent(this, aStream, aLength),
                            NS_DISPATCH_NORMAL);
  }

  LOG(("WebSocketChannelChild::SendBinaryStream() %p\n", this));

  AutoIPCStream autoStream;
  autoStream.Serialize(aStream, static_cast<mozilla::dom::ContentChild*>(
                                    gNeckoChild->Manager()));

  {
    MutexAutoLock lock(mMutex);
    if (mIPCState != Opened) {
      return NS_ERROR_UNEXPECTED;
    }
  }

  if (!SendSendBinaryStream(autoStream.TakeValue(), aLength)) {
    return NS_ERROR_UNEXPECTED;
  }

  return NS_OK;
}

NS_IMETHODIMP
WebSocketChannelChild::GetSecurityInfo(nsISupports** aSecurityInfo) {
  LOG(("WebSocketChannelChild::GetSecurityInfo() %p\n", this));
  return NS_ERROR_NOT_AVAILABLE;
}

bool WebSocketChannelChild::IsOnTargetThread() {
  MOZ_ASSERT(mTargetThread);
  bool isOnTargetThread = false;
  nsresult rv = mTargetThread->IsOnCurrentThread(&isOnTargetThread);
  MOZ_ASSERT(NS_SUCCEEDED(rv));
  return NS_FAILED(rv) ? false : isOnTargetThread;
}

}  // namespace net
}  // namespace mozilla
