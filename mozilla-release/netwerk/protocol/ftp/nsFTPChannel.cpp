/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=4 sts=2 sw=2 et cin: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsFTPChannel.h"
#include "nsFtpConnectionThread.h"  // defines nsFtpState

#include "nsThreadUtils.h"
#include "mozilla/Attributes.h"

using namespace mozilla;
using namespace mozilla::net;
extern LazyLogModule gFTPLog;

// There are two transport connections established for an
// ftp connection. One is used for the command channel , and
// the other for the data channel. The command channel is the first
// connection made and is used to negotiate the second, data, channel.
// The data channel is driven by the command channel and is either
// initiated by the server (PORT command) or by the client (PASV command).
// Client initiation is the most common case and is attempted first.

//-----------------------------------------------------------------------------

<<<<<<< HEAD
NS_IMPL_ISUPPORTS_INHERITED(nsFtpChannel, nsBaseChannel, nsIUploadChannel,
                            nsIResumableChannel, nsIFTPChannel,
                            nsIProxiedChannel, nsIForcePendingChannel,
||||||| merged common ancestors
NS_IMPL_ISUPPORTS_INHERITED(nsFtpChannel,
                            nsBaseChannel,
                            nsIUploadChannel,
                            nsIResumableChannel,
                            nsIFTPChannel,
                            nsIProxiedChannel,
                            nsIForcePendingChannel,
=======
NS_IMPL_ISUPPORTS_INHERITED(nsFtpChannel, nsBaseChannel, nsIUploadChannel,
                            nsIResumableChannel, nsIFTPChannel,
                            nsIProxiedChannel, nsIForcePendingChannel,
                            nsISupportsWeakReference,
>>>>>>> upstream-releases
                            nsIChannelWithDivertableParentListener)

//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
nsFtpChannel::SetUploadStream(nsIInputStream *stream,
                              const nsACString &contentType,
                              int64_t contentLength) {
  NS_ENSURE_TRUE(!Pending(), NS_ERROR_IN_PROGRESS);
||||||| merged common ancestors
nsFtpChannel::SetUploadStream(nsIInputStream *stream,
                              const nsACString &contentType,
                              int64_t contentLength)
{
    NS_ENSURE_TRUE(!Pending(), NS_ERROR_IN_PROGRESS);
=======
nsFtpChannel::SetUploadStream(nsIInputStream* stream,
                              const nsACString& contentType,
                              int64_t contentLength) {
  NS_ENSURE_TRUE(!Pending(), NS_ERROR_IN_PROGRESS);
>>>>>>> upstream-releases

  mUploadStream = stream;

  // NOTE: contentLength is intentionally ignored here.

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFtpChannel::GetUploadStream(nsIInputStream **stream) {
  NS_ENSURE_ARG_POINTER(stream);
  *stream = mUploadStream;
  NS_IF_ADDREF(*stream);
  return NS_OK;
||||||| merged common ancestors
nsFtpChannel::GetUploadStream(nsIInputStream **stream)
{
    NS_ENSURE_ARG_POINTER(stream);
    *stream = mUploadStream;
    NS_IF_ADDREF(*stream);
    return NS_OK;
=======
nsFtpChannel::GetUploadStream(nsIInputStream** aStream) {
  NS_ENSURE_ARG_POINTER(aStream);
  nsCOMPtr<nsIInputStream> stream = mUploadStream;
  stream.forget(aStream);
  return NS_OK;
>>>>>>> upstream-releases
}

//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
nsFtpChannel::ResumeAt(uint64_t aStartPos, const nsACString &aEntityID) {
  NS_ENSURE_TRUE(!Pending(), NS_ERROR_IN_PROGRESS);
  mEntityID = aEntityID;
  mStartPos = aStartPos;
  mResumeRequested = (mStartPos || !mEntityID.IsEmpty());
  return NS_OK;
||||||| merged common ancestors
nsFtpChannel::ResumeAt(uint64_t aStartPos, const nsACString& aEntityID)
{
    NS_ENSURE_TRUE(!Pending(), NS_ERROR_IN_PROGRESS);
    mEntityID = aEntityID;
    mStartPos = aStartPos;
    mResumeRequested = (mStartPos || !mEntityID.IsEmpty());
    return NS_OK;
=======
nsFtpChannel::ResumeAt(uint64_t aStartPos, const nsACString& aEntityID) {
  NS_ENSURE_TRUE(!Pending(), NS_ERROR_IN_PROGRESS);
  mEntityID = aEntityID;
  mStartPos = aStartPos;
  mResumeRequested = (mStartPos || !mEntityID.IsEmpty());
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFtpChannel::GetEntityID(nsACString &entityID) {
  if (mEntityID.IsEmpty()) return NS_ERROR_NOT_RESUMABLE;
||||||| merged common ancestors
nsFtpChannel::GetEntityID(nsACString& entityID)
{
    if (mEntityID.IsEmpty())
      return NS_ERROR_NOT_RESUMABLE;
=======
nsFtpChannel::GetEntityID(nsACString& entityID) {
  if (mEntityID.IsEmpty()) return NS_ERROR_NOT_RESUMABLE;
>>>>>>> upstream-releases

  entityID = mEntityID;
  return NS_OK;
}

//-----------------------------------------------------------------------------
NS_IMETHODIMP
<<<<<<< HEAD
nsFtpChannel::GetProxyInfo(nsIProxyInfo **aProxyInfo) {
  *aProxyInfo = ProxyInfo();
  NS_IF_ADDREF(*aProxyInfo);
  return NS_OK;
||||||| merged common ancestors
nsFtpChannel::GetProxyInfo(nsIProxyInfo** aProxyInfo)
{
    *aProxyInfo = ProxyInfo();
    NS_IF_ADDREF(*aProxyInfo);
    return NS_OK;
=======
nsFtpChannel::GetProxyInfo(nsIProxyInfo** aProxyInfo) {
  nsCOMPtr<nsIProxyInfo> info = ProxyInfo();
  info.forget(aProxyInfo);
  return NS_OK;
>>>>>>> upstream-releases
}

//-----------------------------------------------------------------------------

<<<<<<< HEAD
nsresult nsFtpChannel::OpenContentStream(bool async, nsIInputStream **result,
                                         nsIChannel **channel) {
  if (!async) return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsresult
nsFtpChannel::OpenContentStream(bool async, nsIInputStream **result,
                                nsIChannel** channel)
{
    if (!async)
        return NS_ERROR_NOT_IMPLEMENTED;

    nsFtpState *state = new nsFtpState();
    if (!state)
        return NS_ERROR_OUT_OF_MEMORY;
    NS_ADDREF(state);

    nsresult rv = state->Init(this);
    if (NS_FAILED(rv)) {
        NS_RELEASE(state);
        return rv;
    }
=======
nsresult nsFtpChannel::OpenContentStream(bool async, nsIInputStream** result,
                                         nsIChannel** channel) {
  if (!async) return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsFtpState *state = new nsFtpState();
  if (!state) return NS_ERROR_OUT_OF_MEMORY;
  NS_ADDREF(state);
||||||| merged common ancestors
    *result = state;
    return NS_OK;
}
=======
  RefPtr<nsFtpState> state = new nsFtpState();
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult rv = state->Init(this);
  if (NS_FAILED(rv)) {
    NS_RELEASE(state);
    return rv;
  }

  *result = state;
  return NS_OK;
||||||| merged common ancestors
bool
nsFtpChannel::GetStatusArg(nsresult status, nsString &statusArg)
{
    nsAutoCString host;
    URI()->GetHost(host);
    CopyUTF8toUTF16(host, statusArg);
    return true;
=======
  nsresult rv = state->Init(this);
  if (NS_FAILED(rv)) {
    return rv;
  }

  state.forget(result);
  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool nsFtpChannel::GetStatusArg(nsresult status, nsString &statusArg) {
  nsAutoCString host;
  URI()->GetHost(host);
  CopyUTF8toUTF16(host, statusArg);
  return true;
||||||| merged common ancestors
void
nsFtpChannel::OnCallbacksChanged()
{
    mFTPEventSink = nullptr;
=======
bool nsFtpChannel::GetStatusArg(nsresult status, nsString& statusArg) {
  nsAutoCString host;
  URI()->GetHost(host);
  CopyUTF8toUTF16(host, statusArg);
  return true;
>>>>>>> upstream-releases
}

void nsFtpChannel::OnCallbacksChanged() { mFTPEventSink = nullptr; }

//-----------------------------------------------------------------------------

namespace {

<<<<<<< HEAD
class FTPEventSinkProxy final : public nsIFTPEventSink {
  ~FTPEventSinkProxy() = default;

 public:
  explicit FTPEventSinkProxy(nsIFTPEventSink *aTarget)
      : mTarget(aTarget), mEventTarget(GetCurrentThreadEventTarget()) {}

  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIFTPEVENTSINK

  class OnFTPControlLogRunnable : public Runnable {
   public:
    OnFTPControlLogRunnable(nsIFTPEventSink *aTarget, bool aServer,
                            const char *aMessage)
        : mozilla::Runnable("FTPEventSinkProxy::OnFTPControlLogRunnable"),
          mTarget(aTarget),
          mServer(aServer),
          mMessage(aMessage) {}

    NS_DECL_NSIRUNNABLE

   private:
||||||| merged common ancestors
class FTPEventSinkProxy final : public nsIFTPEventSink
{
    ~FTPEventSinkProxy() = default;

public:
    explicit FTPEventSinkProxy(nsIFTPEventSink* aTarget)
        : mTarget(aTarget)
        , mEventTarget(GetCurrentThreadEventTarget())
    { }

    NS_DECL_THREADSAFE_ISUPPORTS
    NS_DECL_NSIFTPEVENTSINK

    class OnFTPControlLogRunnable : public Runnable
    {
    public:
      OnFTPControlLogRunnable(nsIFTPEventSink* aTarget,
                              bool aServer,
                              const char* aMessage)
        : mozilla::Runnable("FTPEventSinkProxy::OnFTPControlLogRunnable")
        , mTarget(aTarget)
        , mServer(aServer)
        , mMessage(aMessage)
      {
      }

      NS_DECL_NSIRUNNABLE

    private:
        nsCOMPtr<nsIFTPEventSink> mTarget;
        bool mServer;
        nsCString mMessage;
    };

private:
=======
class FTPEventSinkProxy final : public nsIFTPEventSink {
  ~FTPEventSinkProxy() = default;

 public:
  explicit FTPEventSinkProxy(nsIFTPEventSink* aTarget)
      : mTarget(aTarget), mEventTarget(GetCurrentThreadEventTarget()) {}

  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIFTPEVENTSINK

  class OnFTPControlLogRunnable : public Runnable {
   public:
    OnFTPControlLogRunnable(nsIFTPEventSink* aTarget, bool aServer,
                            const char* aMessage)
        : mozilla::Runnable("FTPEventSinkProxy::OnFTPControlLogRunnable"),
          mTarget(aTarget),
          mServer(aServer),
          mMessage(aMessage) {}

    NS_DECL_NSIRUNNABLE

   private:
>>>>>>> upstream-releases
    nsCOMPtr<nsIFTPEventSink> mTarget;
    bool mServer;
    nsCString mMessage;
  };

 private:
  nsCOMPtr<nsIFTPEventSink> mTarget;
  nsCOMPtr<nsIEventTarget> mEventTarget;
};

NS_IMPL_ISUPPORTS(FTPEventSinkProxy, nsIFTPEventSink)

NS_IMETHODIMP
<<<<<<< HEAD
FTPEventSinkProxy::OnFTPControlLog(bool aServer, const char *aMsg) {
  RefPtr<OnFTPControlLogRunnable> r =
      new OnFTPControlLogRunnable(mTarget, aServer, aMsg);
  return mEventTarget->Dispatch(r, NS_DISPATCH_NORMAL);
||||||| merged common ancestors
FTPEventSinkProxy::OnFTPControlLog(bool aServer, const char* aMsg)
{
    RefPtr<OnFTPControlLogRunnable> r =
        new OnFTPControlLogRunnable(mTarget, aServer, aMsg);
    return mEventTarget->Dispatch(r, NS_DISPATCH_NORMAL);
=======
FTPEventSinkProxy::OnFTPControlLog(bool aServer, const char* aMsg) {
  RefPtr<OnFTPControlLogRunnable> r =
      new OnFTPControlLogRunnable(mTarget, aServer, aMsg);
  return mEventTarget->Dispatch(r, NS_DISPATCH_NORMAL);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
FTPEventSinkProxy::OnFTPControlLogRunnable::Run() {
  mTarget->OnFTPControlLog(mServer, mMessage.get());
  return NS_OK;
}

<<<<<<< HEAD
}  // namespace

void nsFtpChannel::GetFTPEventSink(nsCOMPtr<nsIFTPEventSink> &aResult) {
  if (!mFTPEventSink) {
    nsCOMPtr<nsIFTPEventSink> ftpSink;
    GetCallback(ftpSink);
    if (ftpSink) {
      mFTPEventSink = new FTPEventSinkProxy(ftpSink);
||||||| merged common ancestors
} // namespace

void
nsFtpChannel::GetFTPEventSink(nsCOMPtr<nsIFTPEventSink> &aResult)
{
    if (!mFTPEventSink) {
        nsCOMPtr<nsIFTPEventSink> ftpSink;
        GetCallback(ftpSink);
        if (ftpSink) {
            mFTPEventSink = new FTPEventSinkProxy(ftpSink);
        }
=======
}  // namespace

void nsFtpChannel::GetFTPEventSink(nsCOMPtr<nsIFTPEventSink>& aResult) {
  if (!mFTPEventSink) {
    nsCOMPtr<nsIFTPEventSink> ftpSink;
    GetCallback(ftpSink);
    if (ftpSink) {
      mFTPEventSink = new FTPEventSinkProxy(ftpSink);
>>>>>>> upstream-releases
    }
  }
  aResult = mFTPEventSink;
}

NS_IMETHODIMP
nsFtpChannel::ForcePending(bool aForcePending) {
  // Set true here so IsPending will return true.
  // Required for callback diversion from child back to parent. In such cases
  // OnStopRequest can be called in the parent before callbacks are diverted
  // back from the child to the listener in the parent.
  mForcePending = aForcePending;

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFtpChannel::IsPending(bool *result) {
||||||| merged common ancestors
nsFtpChannel::IsPending(bool *result)
{
=======
nsFtpChannel::IsPending(bool* result) {
>>>>>>> upstream-releases
  *result = Pending();
  return NS_OK;
}

bool nsFtpChannel::Pending() const {
  return nsBaseChannel::Pending() || mForcePending;
}

NS_IMETHODIMP
nsFtpChannel::Suspend() {
  LOG(("nsFtpChannel::Suspend [this=%p]\n", this));

  nsresult rv = SuspendInternal();

  nsresult rvParentChannel = NS_OK;
  if (mParentChannel) {
    rvParentChannel = mParentChannel->SuspendMessageDiversion();
  }

  return NS_FAILED(rv) ? rv : rvParentChannel;
}

NS_IMETHODIMP
nsFtpChannel::Resume() {
  LOG(("nsFtpChannel::Resume [this=%p]\n", this));

  nsresult rv = ResumeInternal();

  nsresult rvParentChannel = NS_OK;
  if (mParentChannel) {
    rvParentChannel = mParentChannel->ResumeMessageDiversion();
  }

  return NS_FAILED(rv) ? rv : rvParentChannel;
}

//-----------------------------------------------------------------------------
// AChannelHasDivertableParentChannelAsListener internal functions
//-----------------------------------------------------------------------------

NS_IMETHODIMP
<<<<<<< HEAD
nsFtpChannel::MessageDiversionStarted(
    ADivertableParentChannel *aParentChannel) {
||||||| merged common ancestors
nsFtpChannel::MessageDiversionStarted(ADivertableParentChannel *aParentChannel)
{
=======
nsFtpChannel::MessageDiversionStarted(
    ADivertableParentChannel* aParentChannel) {
>>>>>>> upstream-releases
  MOZ_ASSERT(!mParentChannel);
  mParentChannel = aParentChannel;
  // If the channel is suspended, propagate that info to the parent's mEventQ.
  uint32_t suspendCount = mSuspendCount;
  while (suspendCount--) {
    mParentChannel->SuspendMessageDiversion();
  }
  return NS_OK;
}

NS_IMETHODIMP
nsFtpChannel::MessageDiversionStop() {
  LOG(("nsFtpChannel::MessageDiversionStop [this=%p]", this));
  MOZ_ASSERT(mParentChannel);
  mParentChannel = nullptr;
  return NS_OK;
}

NS_IMETHODIMP
nsFtpChannel::SuspendInternal() {
  LOG(("nsFtpChannel::SuspendInternal [this=%p]\n", this));
  ++mSuspendCount;
  return nsBaseChannel::Suspend();
}

NS_IMETHODIMP
nsFtpChannel::ResumeInternal() {
  LOG(("nsFtpChannel::ResumeInternal [this=%p]\n", this));
  NS_ENSURE_TRUE(mSuspendCount > 0, NS_ERROR_UNEXPECTED);
  --mSuspendCount;
  return nsBaseChannel::Resume();
}
