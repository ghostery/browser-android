/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=8 et tw=80 : */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// HttpLog.h should generally be included first
#include "HttpLog.h"

#include "Http2Session.h"
#include "nsHttp.h"
#include "nsHttpHandler.h"
#include "nsHttpRequestHead.h"
#include "TCPFastOpen.h"
#include "nsISocketProvider.h"
#include "nsSocketProviderService.h"
#include "nsISSLSocketControl.h"
#include "nsISocketTransport.h"
#include "nsISupportsPriority.h"
#include "nsNetAddr.h"
#include "prerror.h"
#include "prio.h"
#include "TunnelUtils.h"
#include "nsNetCID.h"
#include "nsServiceManagerUtils.h"
#include "nsComponentManagerUtils.h"
#include "nsSocketTransport2.h"
#include "nsSocketTransportService2.h"
#include "mozilla/Mutex.h"

namespace mozilla {
namespace net {

static PRDescIdentity sLayerIdentity;
static PRIOMethods sLayerMethods;
static PRIOMethods* sLayerMethodsPtr = nullptr;

TLSFilterTransaction::TLSFilterTransaction(nsAHttpTransaction* aWrapped,
                                           const char* aTLSHost,
                                           int32_t aTLSPort,
<<<<<<< HEAD
                                           nsAHttpSegmentReader *aReader,
                                           nsAHttpSegmentWriter *aWriter)
    : mTransaction(aWrapped),
      mEncryptedTextUsed(0),
      mEncryptedTextSize(0),
      mSegmentReader(aReader),
      mSegmentWriter(aWriter),
      mFilterReadCode(NS_ERROR_NOT_INITIALIZED),
      mForce(false),
      mReadSegmentBlocked(false),
      mNudgeCounter(0) {
||||||| merged common ancestors
                                           nsAHttpSegmentReader *aReader,
                                           nsAHttpSegmentWriter *aWriter)
  : mTransaction(aWrapped)
  , mEncryptedTextUsed(0)
  , mEncryptedTextSize(0)
  , mSegmentReader(aReader)
  , mSegmentWriter(aWriter)
  , mFilterReadCode(NS_ERROR_NOT_INITIALIZED)
  , mForce(false)
  , mReadSegmentBlocked(false)
  , mNudgeCounter(0)
{
=======
                                           nsAHttpSegmentReader* aReader,
                                           nsAHttpSegmentWriter* aWriter)
    : mTransaction(aWrapped),
      mEncryptedTextUsed(0),
      mEncryptedTextSize(0),
      mSegmentReader(aReader),
      mSegmentWriter(aWriter),
      mFilterReadCode(NS_ERROR_NOT_INITIALIZED),
      mForce(false),
      mReadSegmentReturnValue(NS_OK),
      mCloseReason(NS_ERROR_UNEXPECTED),
      mNudgeCounter(0) {
>>>>>>> upstream-releases
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  LOG(("TLSFilterTransaction ctor %p\n", this));

  nsCOMPtr<nsISocketProvider> provider;
  nsCOMPtr<nsISocketProviderService> spserv =
      nsSocketProviderService::GetOrCreate();

  if (spserv) {
    spserv->GetSocketProvider("ssl", getter_AddRefs(provider));
  }

  // Install an NSPR layer to handle getpeername() with a failure. This is kind
  // of silly, but the default one used by the pipe asserts when called and the
  // nss code calls it to see if we are connected to a real socket or not.
  if (!sLayerMethodsPtr) {
    // one time initialization
    sLayerIdentity = PR_GetUniqueIdentity("TLSFilterTransaction Layer");
    sLayerMethods = *PR_GetDefaultIOMethods();
    sLayerMethods.getpeername = GetPeerName;
    sLayerMethods.getsocketoption = GetSocketOption;
    sLayerMethods.setsocketoption = SetSocketOption;
    sLayerMethods.read = FilterRead;
    sLayerMethods.write = FilterWrite;
    sLayerMethods.send = FilterSend;
    sLayerMethods.recv = FilterRecv;
    sLayerMethods.close = FilterClose;
    sLayerMethodsPtr = &sLayerMethods;
  }

  mFD = PR_CreateIOLayerStub(sLayerIdentity, &sLayerMethods);

  if (provider && mFD) {
    mFD->secret = reinterpret_cast<PRFilePrivate*>(this);
    provider->AddToSocket(PR_AF_INET, aTLSHost, aTLSPort, nullptr,
                          OriginAttributes(), 0, 0, mFD,
                          getter_AddRefs(mSecInfo));
  }

  if (mTransaction) {
    nsCOMPtr<nsIInterfaceRequestor> callbacks;
    mTransaction->GetSecurityCallbacks(getter_AddRefs(callbacks));
    nsCOMPtr<nsISSLSocketControl> secCtrl(do_QueryInterface(mSecInfo));
    if (secCtrl) {
      secCtrl->SetNotificationCallbacks(callbacks);
    }
  }
}

TLSFilterTransaction::~TLSFilterTransaction() {
  LOG(("TLSFilterTransaction dtor %p\n", this));
  Cleanup();
}

<<<<<<< HEAD
void TLSFilterTransaction::Cleanup() {
||||||| merged common ancestors
void
TLSFilterTransaction::Cleanup()
{
=======
void TLSFilterTransaction::Cleanup() {
  LOG(("TLSFilterTransaction::Cleanup %p", this));

>>>>>>> upstream-releases
  if (mTransaction) {
    mTransaction->Close(NS_ERROR_ABORT);
    mTransaction = nullptr;
  }

  if (mFD) {
    PR_Close(mFD);
    mFD = nullptr;
  }
  mSecInfo = nullptr;
  if (mTimer) {
    mTimer->Cancel();
    mTimer = nullptr;
  }
}

<<<<<<< HEAD
void TLSFilterTransaction::Close(nsresult aReason) {
||||||| merged common ancestors
void
TLSFilterTransaction::Close(nsresult aReason)
{
=======
void TLSFilterTransaction::Close(nsresult aReason) {
  LOG(("TLSFilterTransaction::Close %p %" PRIx32, this,
       static_cast<uint32_t>(aReason)));

>>>>>>> upstream-releases
  if (!mTransaction) {
    return;
  }

  if (mTimer) {
    mTimer->Cancel();
    mTimer = nullptr;
  }
  mTransaction->Close(aReason);
  mTransaction = nullptr;
<<<<<<< HEAD

  RefPtr<NullHttpTransaction> baseTrans(do_QueryReferent(mWeakTrans));
  SpdyConnectTransaction *trans =
      baseTrans ? baseTrans->QuerySpdyConnectTransaction() : nullptr;

  LOG(("TLSFilterTransaction::Close %p aReason=%" PRIx32 " trans=%p\n", this,
       static_cast<uint32_t>(aReason), trans));

  if (trans) {
    trans->Close(aReason);
    trans = nullptr;
  }
||||||| merged common ancestors
=======

  if (gHttpHandler->Bug1563538()) {
    if (NS_FAILED(aReason)) {
      mCloseReason = aReason;
    } else {
      mCloseReason = NS_BASE_STREAM_CLOSED;
    }
  } else {
    MOZ_ASSERT(NS_ERROR_UNEXPECTED == mCloseReason);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult TLSFilterTransaction::OnReadSegment(const char *aData, uint32_t aCount,
                                             uint32_t *outCountRead) {
  LOG(("TLSFilterTransaction %p OnReadSegment %d (buffered %d)\n", this, aCount,
       mEncryptedTextUsed));
||||||| merged common ancestors
nsresult
TLSFilterTransaction::OnReadSegment(const char *aData,
                                    uint32_t aCount,
                                    uint32_t *outCountRead)
{
  LOG(("TLSFilterTransaction %p OnReadSegment %d (buffered %d)\n",
       this, aCount, mEncryptedTextUsed));
=======
nsresult TLSFilterTransaction::OnReadSegment(const char* aData, uint32_t aCount,
                                             uint32_t* outCountRead) {
  LOG(("TLSFilterTransaction %p OnReadSegment %d (buffered %d)\n", this, aCount,
       mEncryptedTextUsed));
>>>>>>> upstream-releases

  mReadSegmentReturnValue = NS_OK;
  MOZ_ASSERT(mSegmentReader);
  if (!mSecInfo) {
    return NS_ERROR_FAILURE;
  }

  nsresult rv;
  *outCountRead = 0;

  // get rid of buffer first
  if (mEncryptedTextUsed) {
    rv = mSegmentReader->CommitToSegmentSize(mEncryptedTextUsed, mForce);
    if (rv == NS_BASE_STREAM_WOULD_BLOCK) {
      return rv;
    }

    uint32_t amt;
    rv = mSegmentReader->OnReadSegment(mEncryptedText.get(), mEncryptedTextUsed,
                                       &amt);
    if (NS_FAILED(rv)) {
      return rv;
    }

    mEncryptedTextUsed -= amt;
    if (mEncryptedTextUsed) {
      memmove(mEncryptedText.get(), &mEncryptedText[amt], mEncryptedTextUsed);
      return NS_OK;
    }
  }

  // encrypt for network write
  // write aData down the SSL layer into the FilterWrite() method where it will
  // be queued into mEncryptedText. We need to copy it like this in order to
  // guarantee atomic writes

  EnsureBuffer(mEncryptedText, aCount + 4096, 0, mEncryptedTextSize);

  while (aCount > 0) {
    int32_t written = PR_Write(mFD, aData, aCount);
    LOG(("TLSFilterTransaction %p OnReadSegment PRWrite(%d) = %d %d\n", this,
         aCount, written, PR_GetError() == PR_WOULD_BLOCK_ERROR));

    if (written < 1) {
      if (*outCountRead) {
        return NS_OK;
      }
      // mTransaction ReadSegments actually obscures this code, so
      // keep it in a member var for this::ReadSegments to inspect. Similar
      // to nsHttpConnection::mSocketOutCondition
<<<<<<< HEAD
      PRErrorCode code = PR_GetError();
      mReadSegmentBlocked = (code == PR_WOULD_BLOCK_ERROR);
      if (mReadSegmentBlocked) {
        return NS_BASE_STREAM_WOULD_BLOCK;
      }

      nsresult rv = ErrorAccordingToNSPR(code);
      Close(rv);
      return rv;
||||||| merged common ancestors
      mReadSegmentBlocked = (PR_GetError() == PR_WOULD_BLOCK_ERROR);
      return mReadSegmentBlocked ? NS_BASE_STREAM_WOULD_BLOCK : NS_ERROR_FAILURE;
=======
      PRErrorCode code = PR_GetError();
      mReadSegmentReturnValue = ErrorAccordingToNSPR(code);

      return mReadSegmentReturnValue;
>>>>>>> upstream-releases
    }
    aCount -= written;
    aData += written;
    *outCountRead += written;
    mNudgeCounter = 0;
  }

  LOG(("TLSFilterTransaction %p OnReadSegment2 (buffered %d)\n", this,
       mEncryptedTextUsed));

  uint32_t amt = 0;
  if (mEncryptedTextUsed) {
    // If we are tunneled on spdy CommitToSegmentSize will prevent partial
    // writes that could interfere with multiplexing. H1 is fine with
    // partial writes.
    rv = mSegmentReader->CommitToSegmentSize(mEncryptedTextUsed, mForce);
    if (rv != NS_BASE_STREAM_WOULD_BLOCK) {
      rv = mSegmentReader->OnReadSegment(mEncryptedText.get(),
                                         mEncryptedTextUsed, &amt);
    }

    if (rv == NS_BASE_STREAM_WOULD_BLOCK) {
      // return OK because all the data was consumed and stored in this buffer
      // It is fine if the connection is null.  We are likely a websocket and
      // thus writing push is ensured by the caller.
      if (Connection()) {
        Connection()->TransactionHasDataToWrite(this);
      }
      return NS_OK;
    } else if (NS_FAILED(rv)) {
      return rv;
    }
  }

  if (amt == mEncryptedTextUsed) {
    mEncryptedText = nullptr;
    mEncryptedTextUsed = 0;
    mEncryptedTextSize = 0;
  } else {
    memmove(mEncryptedText.get(), &mEncryptedText[amt],
            mEncryptedTextUsed - amt);
    mEncryptedTextUsed -= amt;
  }
  return NS_OK;
}

<<<<<<< HEAD
int32_t TLSFilterTransaction::FilterOutput(const char *aBuf, int32_t aAmount) {
  EnsureBuffer(mEncryptedText, mEncryptedTextUsed + aAmount, mEncryptedTextUsed,
               mEncryptedTextSize);
||||||| merged common ancestors
int32_t
TLSFilterTransaction::FilterOutput(const char *aBuf, int32_t aAmount)
{
  EnsureBuffer(mEncryptedText, mEncryptedTextUsed + aAmount,
               mEncryptedTextUsed, mEncryptedTextSize);
=======
int32_t TLSFilterTransaction::FilterOutput(const char* aBuf, int32_t aAmount) {
  EnsureBuffer(mEncryptedText, mEncryptedTextUsed + aAmount, mEncryptedTextUsed,
               mEncryptedTextSize);
>>>>>>> upstream-releases
  memcpy(&mEncryptedText[mEncryptedTextUsed], aBuf, aAmount);
  mEncryptedTextUsed += aAmount;
  return aAmount;
}

nsresult TLSFilterTransaction::CommitToSegmentSize(uint32_t size,
                                                   bool forceCommitment) {
  if (!mSegmentReader) {
    return NS_ERROR_FAILURE;
  }

  // pad the commit by a little bit to leave room for encryption overhead
  // this isn't foolproof and we may still have to buffer, but its a good start
  mForce = forceCommitment;
  return mSegmentReader->CommitToSegmentSize(size + 1024, forceCommitment);
}

<<<<<<< HEAD
nsresult TLSFilterTransaction::OnWriteSegment(char *aData, uint32_t aCount,
                                              uint32_t *outCountRead) {
||||||| merged common ancestors
nsresult
TLSFilterTransaction::OnWriteSegment(char *aData,
                                     uint32_t aCount,
                                     uint32_t *outCountRead)
{

=======
nsresult TLSFilterTransaction::OnWriteSegment(char* aData, uint32_t aCount,
                                              uint32_t* outCountRead) {
>>>>>>> upstream-releases
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  MOZ_ASSERT(mSegmentWriter);
  LOG(("TLSFilterTransaction::OnWriteSegment %p max=%d\n", this, aCount));
  if (!mSecInfo) {
    return NS_ERROR_FAILURE;
  }

  // this will call through to FilterInput to get data from the higher
  // level connection before removing the local TLS layer
  mFilterReadCode = NS_OK;
  int32_t bytesRead = PR_Read(mFD, aData, aCount);
  if (bytesRead == -1) {
    PRErrorCode code = PR_GetError();
    if (code == PR_WOULD_BLOCK_ERROR) {
      return NS_BASE_STREAM_WOULD_BLOCK;
    }
<<<<<<< HEAD
    nsresult rv = ErrorAccordingToNSPR(code);
    Close(rv);
    return rv;
||||||| merged common ancestors
    return NS_ERROR_FAILURE;
=======
    // If reading from the socket succeeded (NS_SUCCEEDED(mFilterReadCode)),
    // but the nss layer encountered an error remember the error.
    if (NS_SUCCEEDED(mFilterReadCode)) {
      mFilterReadCode = ErrorAccordingToNSPR(code);
      LOG(("TLSFilterTransaction::OnWriteSegment %p nss error %" PRIx32 ".\n",
           this, static_cast<uint32_t>(mFilterReadCode)));
    }
    return mFilterReadCode;
>>>>>>> upstream-releases
  }
  *outCountRead = bytesRead;

  if (NS_SUCCEEDED(mFilterReadCode) && !bytesRead) {
    LOG(
        ("TLSFilterTransaction::OnWriteSegment %p "
         "Second layer of TLS stripping results in STREAM_CLOSED\n",
         this));
    mFilterReadCode = NS_BASE_STREAM_CLOSED;
  }

  LOG(("TLSFilterTransaction::OnWriteSegment %p rv=%" PRIx32 " didread=%d "
       "2 layers of ssl stripped to plaintext\n",
       this, static_cast<uint32_t>(mFilterReadCode), bytesRead));
  return mFilterReadCode;
}

<<<<<<< HEAD
int32_t TLSFilterTransaction::FilterInput(char *aBuf, int32_t aAmount) {
||||||| merged common ancestors
int32_t
TLSFilterTransaction::FilterInput(char *aBuf, int32_t aAmount)
{
=======
int32_t TLSFilterTransaction::FilterInput(char* aBuf, int32_t aAmount) {
>>>>>>> upstream-releases
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  MOZ_ASSERT(mSegmentWriter);
  LOG(("TLSFilterTransaction::FilterInput max=%d\n", aAmount));

  uint32_t outCountRead = 0;
  mFilterReadCode =
      mSegmentWriter->OnWriteSegment(aBuf, aAmount, &outCountRead);
  if (NS_SUCCEEDED(mFilterReadCode) && outCountRead) {
    LOG(("TLSFilterTransaction::FilterInput rv=%" PRIx32
         " read=%d input from net "
         "1 layer stripped, 1 still on\n",
         static_cast<uint32_t>(mFilterReadCode), outCountRead));
    if (mReadSegmentReturnValue == NS_BASE_STREAM_WOULD_BLOCK) {
      mNudgeCounter = 0;
    }
  }
  if (mFilterReadCode == NS_BASE_STREAM_WOULD_BLOCK) {
    PR_SetError(PR_WOULD_BLOCK_ERROR, 0);
    return -1;
  }
  return outCountRead;
}

<<<<<<< HEAD
nsresult TLSFilterTransaction::ReadSegments(nsAHttpSegmentReader *aReader,
                                            uint32_t aCount,
                                            uint32_t *outCountRead) {
||||||| merged common ancestors
nsresult
TLSFilterTransaction::ReadSegments(nsAHttpSegmentReader *aReader,
                                   uint32_t aCount, uint32_t *outCountRead)
{
=======
nsresult TLSFilterTransaction::ReadSegments(nsAHttpSegmentReader* aReader,
                                            uint32_t aCount,
                                            uint32_t* outCountRead) {
>>>>>>> upstream-releases
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  LOG(("TLSFilterTransaction::ReadSegments %p max=%d\n", this, aCount));

  if (!mTransaction) {
    return mCloseReason;
  }

  mReadSegmentReturnValue = NS_OK;
  mSegmentReader = aReader;
  nsresult rv = mTransaction->ReadSegments(this, aCount, outCountRead);
  LOG(("TLSFilterTransaction %p called trans->ReadSegments rv=%" PRIx32 " %d\n",
       this, static_cast<uint32_t>(rv), *outCountRead));
  if (NS_SUCCEEDED(rv) &&
      (mReadSegmentReturnValue == NS_BASE_STREAM_WOULD_BLOCK)) {
    LOG(("TLSFilterTransaction %p read segment blocked found rv=%" PRIx32 "\n",
         this, static_cast<uint32_t>(rv)));
    if (Connection()) {
      Unused << Connection()->ForceSend();
    }
  }

  return NS_SUCCEEDED(rv) ? mReadSegmentReturnValue : rv;
}

<<<<<<< HEAD
nsresult TLSFilterTransaction::WriteSegments(nsAHttpSegmentWriter *aWriter,
                                             uint32_t aCount,
                                             uint32_t *outCountWritten) {
||||||| merged common ancestors
nsresult
TLSFilterTransaction::WriteSegments(nsAHttpSegmentWriter *aWriter,
                                    uint32_t aCount, uint32_t *outCountWritten)
{
=======
nsresult TLSFilterTransaction::WriteSegmentsAgain(nsAHttpSegmentWriter* aWriter,
                                                  uint32_t aCount,
                                                  uint32_t* outCountWritten,
                                                  bool* again) {
>>>>>>> upstream-releases
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  LOG(("TLSFilterTransaction::WriteSegmentsAgain %p max=%d\n", this, aCount));

  if (!mTransaction) {
    return mCloseReason;
  }

  mSegmentWriter = aWriter;

  nsresult rv =
      mTransaction->WriteSegmentsAgain(this, aCount, outCountWritten, again);

  if (NS_SUCCEEDED(rv) && !(*outCountWritten) && NS_FAILED(mFilterReadCode)) {
    // nsPipe turns failures into silent OK.. undo that!
    rv = mFilterReadCode;
    if (Connection() && (mFilterReadCode == NS_BASE_STREAM_WOULD_BLOCK)) {
      Unused << Connection()->ResumeRecv();
    }
  }
  LOG(("TLSFilterTransaction %p called trans->WriteSegments rv=%" PRIx32
       " %d\n",
       this, static_cast<uint32_t>(rv), *outCountWritten));
  return rv;
}

<<<<<<< HEAD
nsresult TLSFilterTransaction::GetTransactionSecurityInfo(
    nsISupports **outSecInfo) {
||||||| merged common ancestors
nsresult
TLSFilterTransaction::GetTransactionSecurityInfo(nsISupports **outSecInfo)
{
=======
nsresult TLSFilterTransaction::WriteSegments(nsAHttpSegmentWriter* aWriter,
                                             uint32_t aCount,
                                             uint32_t* outCountWritten) {
  bool again = false;
  return WriteSegmentsAgain(aWriter, aCount, outCountWritten, &again);
}

nsresult TLSFilterTransaction::GetTransactionSecurityInfo(
    nsISupports** outSecInfo) {
>>>>>>> upstream-releases
  if (!mSecInfo) {
    return NS_ERROR_FAILURE;
  }

  nsCOMPtr<nsISupports> temp(mSecInfo);
  temp.forget(outSecInfo);
  return NS_OK;
}

<<<<<<< HEAD
nsresult TLSFilterTransaction::NudgeTunnel(NudgeTunnelCallback *aCallback) {
||||||| merged common ancestors
nsresult
TLSFilterTransaction::NudgeTunnel(NudgeTunnelCallback *aCallback)
{
=======
nsresult TLSFilterTransaction::NudgeTunnel(NudgeTunnelCallback* aCallback) {
>>>>>>> upstream-releases
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  LOG(("TLSFilterTransaction %p NudgeTunnel\n", this));
  mNudgeCallback = nullptr;

  if (!mSecInfo) {
    return NS_ERROR_FAILURE;
  }

  nsCOMPtr<nsISSLSocketControl> ssl(do_QueryInterface(mSecInfo));
  nsresult rv = ssl ? ssl->DriveHandshake() : NS_ERROR_FAILURE;
  if (NS_FAILED(rv) && rv != NS_BASE_STREAM_WOULD_BLOCK) {
    // fatal handshake failure
    LOG(("TLSFilterTransaction %p Fatal Handshake Failure: %d\n", this,
         PR_GetError()));
    return NS_ERROR_FAILURE;
  }

  uint32_t notUsed;
  Unused << OnReadSegment("", 0, &notUsed);

  // The SSL Layer does some unusual things with PR_Poll that makes it a bad
  // match for multiplexed SSL sessions. We work around this by manually polling
  // for the moment during the brief handshake phase or otherwise blocked on
  // write. Thankfully this is a pretty unusual state. NSPR doesn't help us here
  // - asserting when polling without the NSPR IO layer on the bottom of the
  // stack. As a follow-on we can do some NSPR and maybe libssl changes to make
  // this more event driven, but this is acceptable for getting started.

  uint32_t counter = mNudgeCounter++;
  uint32_t delay;

  if (!counter) {
    delay = 0;
  } else if (counter < 8) {  // up to 48ms at 6
    delay = 6;
  } else if (counter < 34) {  // up to 499 ms at 17ms
    delay = 17;
  } else {  // after that at 51ms (3 old windows ticks)
    delay = 51;
  }

  if (!mTimer) {
    mTimer = NS_NewTimer();
  }

  mNudgeCallback = aCallback;
  if (!mTimer || NS_FAILED(mTimer->InitWithCallback(this, delay,
                                                    nsITimer::TYPE_ONE_SHOT))) {
    return StartTimerCallback();
  }

  LOG(("TLSFilterTransaction %p NudgeTunnel timer started\n", this));
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
TLSFilterTransaction::Notify(nsITimer *timer) {
||||||| merged common ancestors
TLSFilterTransaction::Notify(nsITimer *timer)
{
=======
TLSFilterTransaction::Notify(nsITimer* timer) {
>>>>>>> upstream-releases
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  LOG(("TLSFilterTransaction %p NudgeTunnel notify\n", this));

  if (timer != mTimer) {
    return NS_ERROR_UNEXPECTED;
  }
  nsresult rv = StartTimerCallback();
  if (NS_FAILED(rv)) {
    Close(rv);
  }
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
TLSFilterTransaction::GetName(nsACString &aName) {
||||||| merged common ancestors
TLSFilterTransaction::GetName(nsACString& aName)
{
=======
TLSFilterTransaction::GetName(nsACString& aName) {
>>>>>>> upstream-releases
  aName.AssignLiteral("TLSFilterTransaction");
  return NS_OK;
}

nsresult TLSFilterTransaction::StartTimerCallback() {
  LOG(("TLSFilterTransaction %p NudgeTunnel StartTimerCallback %p\n", this,
       mNudgeCallback.get()));

  if (mNudgeCallback) {
    // This class can be called re-entrantly, so cleanup m* before ->on()
    RefPtr<NudgeTunnelCallback> cb(mNudgeCallback);
    mNudgeCallback = nullptr;
    return cb->OnTunnelNudged(this);
  }
  return NS_OK;
}

<<<<<<< HEAD
PRStatus TLSFilterTransaction::GetPeerName(PRFileDesc *aFD, PRNetAddr *addr) {
||||||| merged common ancestors
PRStatus
TLSFilterTransaction::GetPeerName(PRFileDesc *aFD, PRNetAddr*addr)
{
=======
bool TLSFilterTransaction::HasDataToRecv() {
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  if (!mFD) {
    return false;
  }
  int32_t n = 0;
  char c;
  n = PR_Recv(mFD, &c, 1, PR_MSG_PEEK, 0);
  return n > 0;
}

PRStatus TLSFilterTransaction::GetPeerName(PRFileDesc* aFD, PRNetAddr* addr) {
>>>>>>> upstream-releases
  NetAddr peeraddr;
<<<<<<< HEAD
  TLSFilterTransaction *self =
      reinterpret_cast<TLSFilterTransaction *>(aFD->secret);
||||||| merged common ancestors
  TLSFilterTransaction *self = reinterpret_cast<TLSFilterTransaction *>(aFD->secret);
=======
  TLSFilterTransaction* self =
      reinterpret_cast<TLSFilterTransaction*>(aFD->secret);
>>>>>>> upstream-releases

  if (!self->mTransaction ||
      NS_FAILED(self->mTransaction->Connection()->Transport()->GetPeerAddr(
          &peeraddr))) {
    return PR_FAILURE;
  }
  NetAddrToPRNetAddr(&peeraddr, addr);
  return PR_SUCCESS;
}

<<<<<<< HEAD
PRStatus TLSFilterTransaction::GetSocketOption(PRFileDesc *aFD,
                                               PRSocketOptionData *aOpt) {
||||||| merged common ancestors
PRStatus
TLSFilterTransaction::GetSocketOption(PRFileDesc *aFD, PRSocketOptionData *aOpt)
{
=======
PRStatus TLSFilterTransaction::GetSocketOption(PRFileDesc* aFD,
                                               PRSocketOptionData* aOpt) {
>>>>>>> upstream-releases
  if (aOpt->option == PR_SockOpt_Nonblocking) {
    aOpt->value.non_blocking = PR_TRUE;
    return PR_SUCCESS;
  }
  return PR_FAILURE;
}

<<<<<<< HEAD
PRStatus TLSFilterTransaction::SetSocketOption(PRFileDesc *aFD,
                                               const PRSocketOptionData *aOpt) {
||||||| merged common ancestors
PRStatus
TLSFilterTransaction::SetSocketOption(PRFileDesc *aFD, const PRSocketOptionData *aOpt)
{
=======
PRStatus TLSFilterTransaction::SetSocketOption(PRFileDesc* aFD,
                                               const PRSocketOptionData* aOpt) {
>>>>>>> upstream-releases
  return PR_FAILURE;
}

<<<<<<< HEAD
PRStatus TLSFilterTransaction::FilterClose(PRFileDesc *aFD) {
||||||| merged common ancestors
PRStatus
TLSFilterTransaction::FilterClose(PRFileDesc *aFD)
{
=======
PRStatus TLSFilterTransaction::FilterClose(PRFileDesc* aFD) {
>>>>>>> upstream-releases
  return PR_SUCCESS;
}

<<<<<<< HEAD
int32_t TLSFilterTransaction::FilterWrite(PRFileDesc *aFD, const void *aBuf,
                                          int32_t aAmount) {
  TLSFilterTransaction *self =
      reinterpret_cast<TLSFilterTransaction *>(aFD->secret);
  return self->FilterOutput(static_cast<const char *>(aBuf), aAmount);
||||||| merged common ancestors
int32_t
TLSFilterTransaction::FilterWrite(PRFileDesc *aFD, const void *aBuf, int32_t aAmount)
{
  TLSFilterTransaction *self = reinterpret_cast<TLSFilterTransaction *>(aFD->secret);
  return self->FilterOutput(static_cast<const char *>(aBuf), aAmount);
=======
int32_t TLSFilterTransaction::FilterWrite(PRFileDesc* aFD, const void* aBuf,
                                          int32_t aAmount) {
  TLSFilterTransaction* self =
      reinterpret_cast<TLSFilterTransaction*>(aFD->secret);
  return self->FilterOutput(static_cast<const char*>(aBuf), aAmount);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
int32_t TLSFilterTransaction::FilterSend(PRFileDesc *aFD, const void *aBuf,
                                         int32_t aAmount, int, PRIntervalTime) {
||||||| merged common ancestors
int32_t
TLSFilterTransaction::FilterSend(PRFileDesc *aFD, const void *aBuf, int32_t aAmount,
                                  int , PRIntervalTime)
{
=======
int32_t TLSFilterTransaction::FilterSend(PRFileDesc* aFD, const void* aBuf,
                                         int32_t aAmount, int, PRIntervalTime) {
>>>>>>> upstream-releases
  return FilterWrite(aFD, aBuf, aAmount);
}

<<<<<<< HEAD
int32_t TLSFilterTransaction::FilterRead(PRFileDesc *aFD, void *aBuf,
                                         int32_t aAmount) {
  TLSFilterTransaction *self =
      reinterpret_cast<TLSFilterTransaction *>(aFD->secret);
  return self->FilterInput(static_cast<char *>(aBuf), aAmount);
||||||| merged common ancestors
int32_t
TLSFilterTransaction::FilterRead(PRFileDesc *aFD, void *aBuf, int32_t aAmount)
{
  TLSFilterTransaction *self = reinterpret_cast<TLSFilterTransaction *>(aFD->secret);
  return self->FilterInput(static_cast<char *>(aBuf), aAmount);
=======
int32_t TLSFilterTransaction::FilterRead(PRFileDesc* aFD, void* aBuf,
                                         int32_t aAmount) {
  TLSFilterTransaction* self =
      reinterpret_cast<TLSFilterTransaction*>(aFD->secret);
  return self->FilterInput(static_cast<char*>(aBuf), aAmount);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
int32_t TLSFilterTransaction::FilterRecv(PRFileDesc *aFD, void *aBuf,
                                         int32_t aAmount, int, PRIntervalTime) {
||||||| merged common ancestors
int32_t
TLSFilterTransaction::FilterRecv(PRFileDesc *aFD, void *aBuf, int32_t aAmount,
                                  int , PRIntervalTime)
{
=======
int32_t TLSFilterTransaction::FilterRecv(PRFileDesc* aFD, void* aBuf,
                                         int32_t aAmount, int, PRIntervalTime) {
>>>>>>> upstream-releases
  return FilterRead(aFD, aBuf, aAmount);
}

/////
// The other methods of TLSFilterTransaction just call mTransaction->method
/////

<<<<<<< HEAD
void TLSFilterTransaction::SetConnection(nsAHttpConnection *aConnection) {
||||||| merged common ancestors
void
TLSFilterTransaction::SetConnection(nsAHttpConnection *aConnection)
{
=======
void TLSFilterTransaction::SetConnection(nsAHttpConnection* aConnection) {
>>>>>>> upstream-releases
  if (!mTransaction) {
    return;
  }

  mTransaction->SetConnection(aConnection);
}

<<<<<<< HEAD
nsAHttpConnection *TLSFilterTransaction::Connection() {
||||||| merged common ancestors
nsAHttpConnection *
TLSFilterTransaction::Connection()
{
=======
nsAHttpConnection* TLSFilterTransaction::Connection() {
>>>>>>> upstream-releases
  if (!mTransaction) {
    return nullptr;
  }
  return mTransaction->Connection();
}

<<<<<<< HEAD
void TLSFilterTransaction::GetSecurityCallbacks(nsIInterfaceRequestor **outCB) {
||||||| merged common ancestors
void
TLSFilterTransaction::GetSecurityCallbacks(nsIInterfaceRequestor **outCB)
{
=======
void TLSFilterTransaction::GetSecurityCallbacks(nsIInterfaceRequestor** outCB) {
>>>>>>> upstream-releases
  if (!mTransaction) {
    return;
  }
  mTransaction->GetSecurityCallbacks(outCB);
}

<<<<<<< HEAD
void TLSFilterTransaction::OnTransportStatus(nsITransport *aTransport,
                                             nsresult aStatus,
                                             int64_t aProgress) {
||||||| merged common ancestors
void
TLSFilterTransaction::OnTransportStatus(nsITransport* aTransport,
                                        nsresult aStatus, int64_t aProgress)
{
=======
void TLSFilterTransaction::OnTransportStatus(nsITransport* aTransport,
                                             nsresult aStatus,
                                             int64_t aProgress) {
>>>>>>> upstream-releases
  if (!mTransaction) {
    return;
  }
  mTransaction->OnTransportStatus(aTransport, aStatus, aProgress);
}

<<<<<<< HEAD
nsHttpConnectionInfo *TLSFilterTransaction::ConnectionInfo() {
||||||| merged common ancestors
nsHttpConnectionInfo *
TLSFilterTransaction::ConnectionInfo()
{
=======
nsHttpConnectionInfo* TLSFilterTransaction::ConnectionInfo() {
>>>>>>> upstream-releases
  if (!mTransaction) {
    return nullptr;
  }
  return mTransaction->ConnectionInfo();
}

bool TLSFilterTransaction::IsDone() {
  if (!mTransaction) {
    return true;
  }
  return mTransaction->IsDone();
}

nsresult TLSFilterTransaction::Status() {
  if (!mTransaction) {
    return NS_ERROR_UNEXPECTED;
  }

  return mTransaction->Status();
}

uint32_t TLSFilterTransaction::Caps() {
  if (!mTransaction) {
    return 0;
  }

  return mTransaction->Caps();
}

void TLSFilterTransaction::SetDNSWasRefreshed() {
  if (!mTransaction) {
    return;
  }

  mTransaction->SetDNSWasRefreshed();
}

void TLSFilterTransaction::SetProxyConnectFailed() {
  if (!mTransaction) {
    return;
  }

  mTransaction->SetProxyConnectFailed();
}

<<<<<<< HEAD
nsHttpRequestHead *TLSFilterTransaction::RequestHead() {
||||||| merged common ancestors
nsHttpRequestHead *
TLSFilterTransaction::RequestHead()
{
=======
nsHttpRequestHead* TLSFilterTransaction::RequestHead() {
>>>>>>> upstream-releases
  if (!mTransaction) {
    return nullptr;
  }

  return mTransaction->RequestHead();
}

uint32_t TLSFilterTransaction::Http1xTransactionCount() {
  if (!mTransaction) {
    return 0;
  }

  return mTransaction->Http1xTransactionCount();
}

<<<<<<< HEAD
nsresult TLSFilterTransaction::TakeSubTransactions(
    nsTArray<RefPtr<nsAHttpTransaction> > &outTransactions) {
||||||| merged common ancestors
nsresult
TLSFilterTransaction::TakeSubTransactions(
  nsTArray<RefPtr<nsAHttpTransaction> > &outTransactions)
{
=======
nsresult TLSFilterTransaction::TakeSubTransactions(
    nsTArray<RefPtr<nsAHttpTransaction> >& outTransactions) {
>>>>>>> upstream-releases
  LOG(("TLSFilterTransaction::TakeSubTransactions [this=%p] mTransaction %p\n",
       this, mTransaction.get()));

  if (!mTransaction) {
    return NS_ERROR_UNEXPECTED;
  }

  if (mTransaction->TakeSubTransactions(outTransactions) ==
      NS_ERROR_NOT_IMPLEMENTED) {
    outTransactions.AppendElement(mTransaction);
  }
  mTransaction = nullptr;

  return NS_OK;
}

<<<<<<< HEAD
nsresult TLSFilterTransaction::SetProxiedTransaction(
    nsAHttpTransaction *aTrans, nsAHttpTransaction *aSpdyConnectTransaction) {
  LOG(
      ("TLSFilterTransaction::SetProxiedTransaction [this=%p] aTrans=%p, "
       "aSpdyConnectTransaction=%p\n",
       this, aTrans, aSpdyConnectTransaction));
||||||| merged common ancestors
nsresult
TLSFilterTransaction::SetProxiedTransaction(nsAHttpTransaction *aTrans)
{
  LOG(("TLSFilterTransaction::SetProxiedTransaction [this=%p] aTrans=%p\n",
       this, aTrans));
=======
nsresult TLSFilterTransaction::SetProxiedTransaction(
    nsAHttpTransaction* aTrans, nsAHttpTransaction* aSpdyConnectTransaction) {
  LOG(
      ("TLSFilterTransaction::SetProxiedTransaction [this=%p] aTrans=%p, "
       "aSpdyConnectTransaction=%p\n",
       this, aTrans, aSpdyConnectTransaction));
>>>>>>> upstream-releases

  mTransaction = aTrans;

  // Reverting mCloseReason to the default value for consistency to indicate we
  // are no longer in closed state.
  mCloseReason = NS_ERROR_UNEXPECTED;

  nsCOMPtr<nsIInterfaceRequestor> callbacks;
  mTransaction->GetSecurityCallbacks(getter_AddRefs(callbacks));
  nsCOMPtr<nsISSLSocketControl> secCtrl(do_QueryInterface(mSecInfo));
  if (secCtrl && callbacks) {
    secCtrl->SetNotificationCallbacks(callbacks);
  }

  mWeakTrans = do_GetWeakReference(aSpdyConnectTransaction);

  return NS_OK;
}

bool TLSFilterTransaction::IsNullTransaction() {
  if (!mTransaction) {
    return false;
  }
  return mTransaction->IsNullTransaction();
}

<<<<<<< HEAD
NullHttpTransaction *TLSFilterTransaction::QueryNullTransaction() {
||||||| merged common ancestors
NullHttpTransaction *
TLSFilterTransaction::QueryNullTransaction()
{
=======
NullHttpTransaction* TLSFilterTransaction::QueryNullTransaction() {
>>>>>>> upstream-releases
  if (!mTransaction) {
    return nullptr;
  }
  return mTransaction->QueryNullTransaction();
}

<<<<<<< HEAD
nsHttpTransaction *TLSFilterTransaction::QueryHttpTransaction() {
||||||| merged common ancestors
nsHttpTransaction *
TLSFilterTransaction::QueryHttpTransaction()
{
=======
nsHttpTransaction* TLSFilterTransaction::QueryHttpTransaction() {
>>>>>>> upstream-releases
  if (!mTransaction) {
    return nullptr;
  }
  return mTransaction->QueryHttpTransaction();
}

class SocketInWrapper : public nsIAsyncInputStream,
                        public nsAHttpSegmentWriter {
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_FORWARD_NSIASYNCINPUTSTREAM(mStream->)

<<<<<<< HEAD
  SocketInWrapper(nsIAsyncInputStream *aWrapped, TLSFilterTransaction *aFilter)
      : mStream(aWrapped), mTLSFilter(aFilter) {}
||||||| merged common ancestors
  SocketInWrapper(nsIAsyncInputStream *aWrapped, TLSFilterTransaction *aFilter)
    : mStream(aWrapped)
    , mTLSFilter(aFilter)
  { }
=======
  SocketInWrapper(nsIAsyncInputStream* aWrapped, TLSFilterTransaction* aFilter)
      : mStream(aWrapped), mTLSFilter(aFilter) {}
>>>>>>> upstream-releases

  NS_IMETHOD Close() override {
    mTLSFilter = nullptr;
    return mStream->Close();
  }

<<<<<<< HEAD
  NS_IMETHOD Available(uint64_t *_retval) override {
||||||| merged common ancestors
  NS_IMETHOD Available(uint64_t *_retval) override
  {
=======
  NS_IMETHOD Available(uint64_t* _retval) override {
>>>>>>> upstream-releases
    return mStream->Available(_retval);
  }

<<<<<<< HEAD
  NS_IMETHOD IsNonBlocking(bool *_retval) override {
||||||| merged common ancestors
  NS_IMETHOD IsNonBlocking(bool *_retval) override
  {
=======
  NS_IMETHOD IsNonBlocking(bool* _retval) override {
>>>>>>> upstream-releases
    return mStream->IsNonBlocking(_retval);
  }

<<<<<<< HEAD
  NS_IMETHOD ReadSegments(nsWriteSegmentFun aWriter, void *aClosure,
                          uint32_t aCount, uint32_t *_retval) override {
||||||| merged common ancestors
  NS_IMETHOD ReadSegments(nsWriteSegmentFun aWriter, void *aClosure, uint32_t aCount, uint32_t *_retval) override
  {
=======
  NS_IMETHOD ReadSegments(nsWriteSegmentFun aWriter, void* aClosure,
                          uint32_t aCount, uint32_t* _retval) override {
>>>>>>> upstream-releases
    return mStream->ReadSegments(aWriter, aClosure, aCount, _retval);
  }

  // finally, ones that don't get forwarded :)
<<<<<<< HEAD
  NS_IMETHOD Read(char *aBuf, uint32_t aCount, uint32_t *_retval) override;
  virtual nsresult OnWriteSegment(char *segment, uint32_t count,
                                  uint32_t *countWritten) override;
||||||| merged common ancestors
  NS_IMETHOD Read(char *aBuf, uint32_t aCount, uint32_t *_retval) override;
  virtual nsresult OnWriteSegment(char *segment, uint32_t count, uint32_t *countWritten) override;
=======
  NS_IMETHOD Read(char* aBuf, uint32_t aCount, uint32_t* _retval) override;
  virtual nsresult OnWriteSegment(char* segment, uint32_t count,
                                  uint32_t* countWritten) override;
>>>>>>> upstream-releases

 private:
  virtual ~SocketInWrapper() = default;
  ;

  nsCOMPtr<nsIAsyncInputStream> mStream;
  RefPtr<TLSFilterTransaction> mTLSFilter;
};

<<<<<<< HEAD
nsresult SocketInWrapper::OnWriteSegment(char *segment, uint32_t count,
                                         uint32_t *countWritten) {
  LOG(("SocketInWrapper OnWriteSegment %d %p filter=%p\n", count, this,
       mTLSFilter.get()));
||||||| merged common ancestors
nsresult
SocketInWrapper::OnWriteSegment(char *segment, uint32_t count, uint32_t *countWritten)
{
  LOG(("SocketInWrapper OnWriteSegment %d %p filter=%p\n", count, this, mTLSFilter.get()));
=======
nsresult SocketInWrapper::OnWriteSegment(char* segment, uint32_t count,
                                         uint32_t* countWritten) {
  LOG(("SocketInWrapper OnWriteSegment %d %p filter=%p\n", count, this,
       mTLSFilter.get()));
>>>>>>> upstream-releases

  nsresult rv = mStream->Read(segment, count, countWritten);
  LOG(("SocketInWrapper OnWriteSegment %p wrapped read %" PRIx32 " %d\n", this,
       static_cast<uint32_t>(rv), *countWritten));
  return rv;
}

NS_IMETHODIMP
<<<<<<< HEAD
SocketInWrapper::Read(char *aBuf, uint32_t aCount, uint32_t *_retval) {
  LOG(("SocketInWrapper Read %d %p filter=%p\n", aCount, this,
       mTLSFilter.get()));
||||||| merged common ancestors
SocketInWrapper::Read(char *aBuf, uint32_t aCount, uint32_t *_retval)
{
  LOG(("SocketInWrapper Read %d %p filter=%p\n", aCount, this, mTLSFilter.get()));
=======
SocketInWrapper::Read(char* aBuf, uint32_t aCount, uint32_t* _retval) {
  LOG(("SocketInWrapper Read %d %p filter=%p\n", aCount, this,
       mTLSFilter.get()));
>>>>>>> upstream-releases

  if (!mTLSFilter) {
    return NS_ERROR_UNEXPECTED;  // protect potentially dangling mTLSFilter
  }

  // mTLSFilter->mSegmentWriter MUST be this at ctor time
  return mTLSFilter->OnWriteSegment(aBuf, aCount, _retval);
}

class SocketOutWrapper : public nsIAsyncOutputStream,
                         public nsAHttpSegmentReader {
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_FORWARD_NSIASYNCOUTPUTSTREAM(mStream->)

<<<<<<< HEAD
  SocketOutWrapper(nsIAsyncOutputStream *aWrapped,
                   TLSFilterTransaction *aFilter)
      : mStream(aWrapped), mTLSFilter(aFilter) {}
||||||| merged common ancestors
  SocketOutWrapper(nsIAsyncOutputStream *aWrapped, TLSFilterTransaction *aFilter)
    : mStream(aWrapped)
    , mTLSFilter(aFilter)
  { }
=======
  SocketOutWrapper(nsIAsyncOutputStream* aWrapped,
                   TLSFilterTransaction* aFilter)
      : mStream(aWrapped), mTLSFilter(aFilter) {}
>>>>>>> upstream-releases

  NS_IMETHOD Close() override {
    mTLSFilter = nullptr;
    return mStream->Close();
  }

  NS_IMETHOD Flush() override { return mStream->Flush(); }

<<<<<<< HEAD
  NS_IMETHOD IsNonBlocking(bool *_retval) override {
||||||| merged common ancestors
  NS_IMETHOD IsNonBlocking(bool *_retval) override
  {
=======
  NS_IMETHOD IsNonBlocking(bool* _retval) override {
>>>>>>> upstream-releases
    return mStream->IsNonBlocking(_retval);
  }

<<<<<<< HEAD
  NS_IMETHOD WriteSegments(nsReadSegmentFun aReader, void *aClosure,
                           uint32_t aCount, uint32_t *_retval) override {
||||||| merged common ancestors
  NS_IMETHOD WriteSegments(nsReadSegmentFun aReader, void *aClosure, uint32_t aCount, uint32_t *_retval) override
  {
=======
  NS_IMETHOD WriteSegments(nsReadSegmentFun aReader, void* aClosure,
                           uint32_t aCount, uint32_t* _retval) override {
>>>>>>> upstream-releases
    return mStream->WriteSegments(aReader, aClosure, aCount, _retval);
  }

<<<<<<< HEAD
  NS_IMETHOD WriteFrom(nsIInputStream *aFromStream, uint32_t aCount,
                       uint32_t *_retval) override {
||||||| merged common ancestors
  NS_IMETHOD WriteFrom(nsIInputStream *aFromStream, uint32_t aCount, uint32_t *_retval) override
  {
=======
  NS_IMETHOD WriteFrom(nsIInputStream* aFromStream, uint32_t aCount,
                       uint32_t* _retval) override {
>>>>>>> upstream-releases
    return mStream->WriteFrom(aFromStream, aCount, _retval);
  }

  // finally, ones that don't get forwarded :)
<<<<<<< HEAD
  NS_IMETHOD Write(const char *aBuf, uint32_t aCount,
                   uint32_t *_retval) override;
  virtual nsresult OnReadSegment(const char *segment, uint32_t count,
                                 uint32_t *countRead) override;
||||||| merged common ancestors
  NS_IMETHOD Write(const char *aBuf, uint32_t aCount, uint32_t *_retval) override;
  virtual nsresult OnReadSegment(const char *segment, uint32_t count, uint32_t *countRead) override;
=======
  NS_IMETHOD Write(const char* aBuf, uint32_t aCount,
                   uint32_t* _retval) override;
  virtual nsresult OnReadSegment(const char* segment, uint32_t count,
                                 uint32_t* countRead) override;
>>>>>>> upstream-releases

 private:
  virtual ~SocketOutWrapper() = default;
  ;

  nsCOMPtr<nsIAsyncOutputStream> mStream;
  RefPtr<TLSFilterTransaction> mTLSFilter;
};

<<<<<<< HEAD
nsresult SocketOutWrapper::OnReadSegment(const char *segment, uint32_t count,
                                         uint32_t *countWritten) {
||||||| merged common ancestors
nsresult
SocketOutWrapper::OnReadSegment(const char *segment, uint32_t count, uint32_t *countWritten)
{
=======
nsresult SocketOutWrapper::OnReadSegment(const char* segment, uint32_t count,
                                         uint32_t* countWritten) {
>>>>>>> upstream-releases
  return mStream->Write(segment, count, countWritten);
}

NS_IMETHODIMP
<<<<<<< HEAD
SocketOutWrapper::Write(const char *aBuf, uint32_t aCount, uint32_t *_retval) {
  LOG(("SocketOutWrapper Write %d %p filter=%p\n", aCount, this,
       mTLSFilter.get()));
||||||| merged common ancestors
SocketOutWrapper::Write(const char *aBuf, uint32_t aCount, uint32_t *_retval)
{
  LOG(("SocketOutWrapper Write %d %p filter=%p\n", aCount, this, mTLSFilter.get()));
=======
SocketOutWrapper::Write(const char* aBuf, uint32_t aCount, uint32_t* _retval) {
  LOG(("SocketOutWrapper Write %d %p filter=%p\n", aCount, this,
       mTLSFilter.get()));
>>>>>>> upstream-releases

  // mTLSFilter->mSegmentReader MUST be this at ctor time
  if (!mTLSFilter) {
    return NS_ERROR_UNEXPECTED;  // protect potentially dangling mTLSFilter
  }

  return mTLSFilter->OnReadSegment(aBuf, aCount, _retval);
}

<<<<<<< HEAD
void TLSFilterTransaction::newIODriver(nsIAsyncInputStream *aSocketIn,
                                       nsIAsyncOutputStream *aSocketOut,
                                       nsIAsyncInputStream **outSocketIn,
                                       nsIAsyncOutputStream **outSocketOut) {
  SocketInWrapper *inputWrapper = new SocketInWrapper(aSocketIn, this);
||||||| merged common ancestors
void
TLSFilterTransaction::newIODriver(nsIAsyncInputStream *aSocketIn,
                                  nsIAsyncOutputStream *aSocketOut,
                                  nsIAsyncInputStream **outSocketIn,
                                  nsIAsyncOutputStream **outSocketOut)
{
  SocketInWrapper *inputWrapper = new SocketInWrapper(aSocketIn, this);
=======
void TLSFilterTransaction::newIODriver(nsIAsyncInputStream* aSocketIn,
                                       nsIAsyncOutputStream* aSocketOut,
                                       nsIAsyncInputStream** outSocketIn,
                                       nsIAsyncOutputStream** outSocketOut) {
  SocketInWrapper* inputWrapper = new SocketInWrapper(aSocketIn, this);
>>>>>>> upstream-releases
  mSegmentWriter = inputWrapper;
  nsCOMPtr<nsIAsyncInputStream> newIn(inputWrapper);
  newIn.forget(outSocketIn);

  SocketOutWrapper* outputWrapper = new SocketOutWrapper(aSocketOut, this);
  mSegmentReader = outputWrapper;
  nsCOMPtr<nsIAsyncOutputStream> newOut(outputWrapper);
  newOut.forget(outSocketOut);
}

<<<<<<< HEAD
SpdyConnectTransaction *TLSFilterTransaction::QuerySpdyConnectTransaction() {
||||||| merged common ancestors
SpdyConnectTransaction *
TLSFilterTransaction::QuerySpdyConnectTransaction()
{
=======
SpdyConnectTransaction* TLSFilterTransaction::QuerySpdyConnectTransaction() {
>>>>>>> upstream-releases
  if (!mTransaction) {
    return nullptr;
  }
  return mTransaction->QuerySpdyConnectTransaction();
}

<<<<<<< HEAD
class SocketTransportShim : public nsISocketTransport {
 public:
||||||| merged common ancestors
class SocketTransportShim : public nsISocketTransport
{
public:
=======
class WeakTransProxy final : public nsISupports {
 public:
  NS_DECL_THREADSAFE_ISUPPORTS

  explicit WeakTransProxy(SpdyConnectTransaction* aTrans) {
    MOZ_ASSERT(OnSocketThread());
    mWeakTrans = do_GetWeakReference(aTrans);
  }

  already_AddRefed<NullHttpTransaction> QueryTransaction() {
    MOZ_ASSERT(OnSocketThread());
    RefPtr<NullHttpTransaction> trans = do_QueryReferent(mWeakTrans);
    return trans.forget();
  }

 private:
  ~WeakTransProxy() { MOZ_ASSERT(OnSocketThread()); }

  nsWeakPtr mWeakTrans;
};

NS_IMPL_ISUPPORTS(WeakTransProxy, nsISupports)

class WeakTransFreeProxy final : public Runnable {
 public:
  explicit WeakTransFreeProxy(WeakTransProxy* proxy)
      : Runnable("WeakTransFreeProxy"), mProxy(proxy) {}

  NS_IMETHOD Run() override {
    MOZ_ASSERT(OnSocketThread());
    mProxy = nullptr;
    return NS_OK;
  }

  void Dispatch() {
    MOZ_ASSERT(!OnSocketThread());
    nsCOMPtr<nsIEventTarget> sts =
        do_GetService("@mozilla.org/network/socket-transport-service;1");
    Unused << sts->Dispatch(this, nsIEventTarget::DISPATCH_NORMAL);
  }

 private:
  RefPtr<WeakTransProxy> mProxy;
};

class SocketTransportShim : public nsISocketTransport {
 public:
>>>>>>> upstream-releases
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSITRANSPORT
  NS_DECL_NSISOCKETTRANSPORT

<<<<<<< HEAD
  explicit SocketTransportShim(nsISocketTransport *aWrapped, bool aIsWebsocket)
      : mWrapped(aWrapped), mIsWebsocket(aIsWebsocket){};
||||||| merged common ancestors
  explicit SocketTransportShim(nsISocketTransport *aWrapped)
    : mWrapped(aWrapped)
  {};
=======
  explicit SocketTransportShim(SpdyConnectTransaction* aTrans,
                               nsISocketTransport* aWrapped, bool aIsWebsocket)
      : mWrapped(aWrapped), mIsWebsocket(aIsWebsocket) {
    mWeakTrans = new WeakTransProxy(aTrans);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  virtual ~SocketTransportShim() = default;
||||||| merged common ancestors
private:
  virtual ~SocketTransportShim() = default;;
=======
 private:
  virtual ~SocketTransportShim() {
    if (!OnSocketThread()) {
      RefPtr<WeakTransFreeProxy> p = new WeakTransFreeProxy(mWeakTrans);
      mWeakTrans = nullptr;
      p->Dispatch();
    }
  }
>>>>>>> upstream-releases

  nsCOMPtr<nsISocketTransport> mWrapped;
<<<<<<< HEAD
  bool mIsWebsocket;
  nsCOMPtr<nsIInterfaceRequestor> mSecurityCallbacks;
};

class WeakTransProxy final : public nsISupports {
 public:
  NS_DECL_THREADSAFE_ISUPPORTS

  explicit WeakTransProxy(SpdyConnectTransaction *aTrans) {
    MOZ_ASSERT(OnSocketThread());
    mWeakTrans = do_GetWeakReference(aTrans);
  }

  already_AddRefed<NullHttpTransaction> QueryTransaction() {
    MOZ_ASSERT(OnSocketThread());
    RefPtr<NullHttpTransaction> trans = do_QueryReferent(mWeakTrans);
    return trans.forget();
  }

 private:
  ~WeakTransProxy() { MOZ_ASSERT(OnSocketThread()); }

  nsWeakPtr mWeakTrans;
};

NS_IMPL_ISUPPORTS(WeakTransProxy, nsISupports)

class WeakTransFreeProxy final : public Runnable {
 public:
  explicit WeakTransFreeProxy(WeakTransProxy *proxy)
      : Runnable("WeakTransFreeProxy"), mProxy(proxy) {}

  NS_IMETHOD Run() override {
    MOZ_ASSERT(OnSocketThread());
    mProxy = nullptr;
    return NS_OK;
  }

  void Dispatch() {
    MOZ_ASSERT(!OnSocketThread());
    nsCOMPtr<nsIEventTarget> sts =
        do_GetService("@mozilla.org/network/socket-transport-service;1");
    Unused << sts->Dispatch(this, nsIEventTarget::DISPATCH_NORMAL);
  }

 private:
  RefPtr<WeakTransProxy> mProxy;
||||||| merged common ancestors
=======
  bool mIsWebsocket;
  nsCOMPtr<nsIInterfaceRequestor> mSecurityCallbacks;
  RefPtr<WeakTransProxy> mWeakTrans;  // SpdyConnectTransaction *
>>>>>>> upstream-releases
};

class OutputStreamShim : public nsIAsyncOutputStream {
 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIOUTPUTSTREAM
  NS_DECL_NSIASYNCOUTPUTSTREAM

  friend class SpdyConnectTransaction;
  friend class WebsocketHasDataToWrite;
  friend class OutputCloseTransaction;

<<<<<<< HEAD
  OutputStreamShim(SpdyConnectTransaction *aTrans, bool aIsWebsocket)
      : mCallback(nullptr),
        mStatus(NS_OK),
        mMutex("OutputStreamShim"),
        mIsWebsocket(aIsWebsocket) {
    mWeakTrans = new WeakTransProxy(aTrans);
||||||| merged common ancestors
  explicit OutputStreamShim(SpdyConnectTransaction *aTrans)
    : mCallback(nullptr)
    , mStatus(NS_OK)
  {
    mWeakTrans = do_GetWeakReference(aTrans);
=======
  OutputStreamShim(SpdyConnectTransaction* aTrans, bool aIsWebsocket)
      : mCallback(nullptr),
        mStatus(NS_OK),
        mMutex("OutputStreamShim"),
        mIsWebsocket(aIsWebsocket) {
    mWeakTrans = new WeakTransProxy(aTrans);
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  nsIOutputStreamCallback *GetCallback();
||||||| merged common ancestors
private:
  virtual ~OutputStreamShim() = default;;
=======
  nsIOutputStreamCallback* GetCallback();

 private:
  virtual ~OutputStreamShim() {
    if (!OnSocketThread()) {
      RefPtr<WeakTransFreeProxy> p = new WeakTransFreeProxy(mWeakTrans);
      mWeakTrans = nullptr;
      p->Dispatch();
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  virtual ~OutputStreamShim() {
    if (!OnSocketThread()) {
      RefPtr<WeakTransFreeProxy> p = new WeakTransFreeProxy(mWeakTrans);
      mWeakTrans = nullptr;
      p->Dispatch();
    }
  }

  RefPtr<WeakTransProxy> mWeakTrans;  // SpdyConnectTransaction *
  nsIOutputStreamCallback *mCallback;
||||||| merged common ancestors
  nsWeakPtr mWeakTrans; // SpdyConnectTransaction *
  nsIOutputStreamCallback *mCallback;
=======
  RefPtr<WeakTransProxy> mWeakTrans;  // SpdyConnectTransaction *
  nsIOutputStreamCallback* mCallback;
>>>>>>> upstream-releases
  nsresult mStatus;
  mozilla::Mutex mMutex;

  // Websockets
  bool mIsWebsocket;
  nsresult CallTransactionHasDataToWrite();
  nsresult CloseTransaction(nsresult reason);
};

class InputStreamShim : public nsIAsyncInputStream {
 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIINPUTSTREAM
  NS_DECL_NSIASYNCINPUTSTREAM

  friend class SpdyConnectTransaction;
  friend class InputCloseTransaction;

<<<<<<< HEAD
  InputStreamShim(SpdyConnectTransaction *aTrans, bool aIsWebsocket)
      : mCallback(nullptr),
        mStatus(NS_OK),
        mMutex("InputStreamShim"),
        mIsWebsocket(aIsWebsocket) {
    mWeakTrans = new WeakTransProxy(aTrans);
||||||| merged common ancestors
  explicit InputStreamShim(SpdyConnectTransaction *aTrans)
    : mCallback(nullptr)
    , mStatus(NS_OK)
  {
    mWeakTrans = do_GetWeakReference(aTrans);
=======
  InputStreamShim(SpdyConnectTransaction* aTrans, bool aIsWebsocket)
      : mCallback(nullptr),
        mStatus(NS_OK),
        mMutex("InputStreamShim"),
        mIsWebsocket(aIsWebsocket) {
    mWeakTrans = new WeakTransProxy(aTrans);
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  nsIInputStreamCallback *GetCallback();
||||||| merged common ancestors
private:
  virtual ~InputStreamShim() = default;;
=======
  nsIInputStreamCallback* GetCallback();
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  virtual ~InputStreamShim() {
    if (!OnSocketThread()) {
      RefPtr<WeakTransFreeProxy> p = new WeakTransFreeProxy(mWeakTrans);
      mWeakTrans = nullptr;
      p->Dispatch();
    }
  }

  RefPtr<WeakTransProxy> mWeakTrans;  // SpdyConnectTransaction *
  nsIInputStreamCallback *mCallback;
||||||| merged common ancestors
  nsWeakPtr mWeakTrans; // SpdyConnectTransaction *
  nsIInputStreamCallback *mCallback;
=======
 private:
  virtual ~InputStreamShim() {
    if (!OnSocketThread()) {
      RefPtr<WeakTransFreeProxy> p = new WeakTransFreeProxy(mWeakTrans);
      mWeakTrans = nullptr;
      p->Dispatch();
    }
  }

  RefPtr<WeakTransProxy> mWeakTrans;  // SpdyConnectTransaction *
  nsIInputStreamCallback* mCallback;
>>>>>>> upstream-releases
  nsresult mStatus;
  mozilla::Mutex mMutex;

  // Websockets
  bool mIsWebsocket;
  nsresult CloseTransaction(nsresult reason);
};

<<<<<<< HEAD
SpdyConnectTransaction::SpdyConnectTransaction(
    nsHttpConnectionInfo *ci, nsIInterfaceRequestor *callbacks, uint32_t caps,
    nsHttpTransaction *trans, nsAHttpConnection *session, bool isWebsocket)
    : NullHttpTransaction(ci, callbacks, caps | NS_HTTP_ALLOW_KEEPALIVE),
      mConnectStringOffset(0),
      mSession(session),
      mSegmentReader(nullptr),
      mInputDataSize(0),
      mInputDataUsed(0),
      mInputDataOffset(0),
      mOutputDataSize(0),
      mOutputDataUsed(0),
      mOutputDataOffset(0),
      mForcePlainText(false),
      mIsWebsocket(isWebsocket),
      mConnRefTaken(false),
      mCreateShimErrorCalled(false) {
||||||| merged common ancestors
SpdyConnectTransaction::SpdyConnectTransaction(nsHttpConnectionInfo *ci,
                                               nsIInterfaceRequestor *callbacks,
                                               uint32_t caps,
                                               nsHttpTransaction *trans,
                                               nsAHttpConnection *session)
  : NullHttpTransaction(ci, callbacks, caps | NS_HTTP_ALLOW_KEEPALIVE)
  , mConnectStringOffset(0)
  , mSession(session)
  , mSegmentReader(nullptr)
  , mInputDataSize(0)
  , mInputDataUsed(0)
  , mInputDataOffset(0)
  , mOutputDataSize(0)
  , mOutputDataUsed(0)
  , mOutputDataOffset(0)
  , mForcePlainText(false)
{
=======
SpdyConnectTransaction::SpdyConnectTransaction(
    nsHttpConnectionInfo* ci, nsIInterfaceRequestor* callbacks, uint32_t caps,
    nsHttpTransaction* trans, nsAHttpConnection* session, bool isWebsocket)
    : NullHttpTransaction(ci, callbacks, caps | NS_HTTP_ALLOW_KEEPALIVE),
      mConnectStringOffset(0),
      mSession(session),
      mSegmentReader(nullptr),
      mInputDataSize(0),
      mInputDataUsed(0),
      mInputDataOffset(0),
      mOutputDataSize(0),
      mOutputDataUsed(0),
      mOutputDataOffset(0),
      mForcePlainText(false),
      mIsWebsocket(isWebsocket),
      mConnRefTaken(false),
      mCreateShimErrorCalled(false) {
>>>>>>> upstream-releases
  LOG(("SpdyConnectTransaction ctor %p\n", this));

  mTimestampSyn = TimeStamp::Now();
  mRequestHead = new nsHttpRequestHead();
  if (mIsWebsocket) {
    // Ensure our request head has all the websocket headers duplicated from the
    // original transaction before calling the boilerplate stuff to create the
    // rest of the CONNECT headers.
    trans->RequestHead()->Enter();
    mRequestHead->SetHeaders(trans->RequestHead()->Headers());
    trans->RequestHead()->Exit();
  }
  DebugOnly<nsresult> rv = nsHttpConnection::MakeConnectString(
      trans, mRequestHead, mConnectString, mIsWebsocket);
  MOZ_ASSERT(NS_SUCCEEDED(rv));
  mDrivingTransaction = trans;
}

SpdyConnectTransaction::~SpdyConnectTransaction() {
  LOG(("SpdyConnectTransaction dtor %p\n", this));

  if (mDrivingTransaction) {
    // requeue it I guess. This should be gone.
    mDrivingTransaction->SetH2WSTransaction(nullptr);
    Unused << gHttpHandler->InitiateTransaction(
        mDrivingTransaction, mDrivingTransaction->Priority());
  }
}

void SpdyConnectTransaction::ForcePlainText() {
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  MOZ_ASSERT(!mInputDataUsed && !mInputDataSize && !mInputDataOffset);
  MOZ_ASSERT(!mForcePlainText);
  MOZ_ASSERT(!mTunnelTransport, "call before mapstreamtohttpconnection");
  MOZ_ASSERT(!mIsWebsocket);

  mForcePlainText = true;
}

<<<<<<< HEAD
void SpdyConnectTransaction::MapStreamToHttpConnection(
    nsISocketTransport *aTransport, nsHttpConnectionInfo *aConnInfo) {
||||||| merged common ancestors
void
SpdyConnectTransaction::MapStreamToHttpConnection(nsISocketTransport *aTransport,
                                                  nsHttpConnectionInfo *aConnInfo)
{
=======
void SpdyConnectTransaction::MapStreamToHttpConnection(
    nsISocketTransport* aTransport, nsHttpConnectionInfo* aConnInfo,
    int32_t httpResponseCode) {
>>>>>>> upstream-releases
  mConnInfo = aConnInfo;

<<<<<<< HEAD
  mTunnelTransport = new SocketTransportShim(aTransport, mIsWebsocket);
  mTunnelStreamIn = new InputStreamShim(this, mIsWebsocket);
  mTunnelStreamOut = new OutputStreamShim(this, mIsWebsocket);
||||||| merged common ancestors
  mTunnelTransport = new SocketTransportShim(aTransport);
  mTunnelStreamIn = new InputStreamShim(this);
  mTunnelStreamOut = new OutputStreamShim(this);
=======
  mTunnelTransport = new SocketTransportShim(this, aTransport, mIsWebsocket);
  mTunnelStreamIn = new InputStreamShim(this, mIsWebsocket);
  mTunnelStreamOut = new OutputStreamShim(this, mIsWebsocket);
>>>>>>> upstream-releases
  mTunneledConn = new nsHttpConnection();

  switch (httpResponseCode) {
    case 404:
      CreateShimError(NS_ERROR_UNKNOWN_HOST);
      break;
    case 407:
      CreateShimError(NS_ERROR_PROXY_AUTHENTICATION_FAILED);
      break;
    case 429:
      CreateShimError(NS_ERROR_TOO_MANY_REQUESTS);
      break;
    case 502:
      CreateShimError(NS_ERROR_PROXY_BAD_GATEWAY);
      break;
    case 504:
      CreateShimError(NS_ERROR_PROXY_GATEWAY_TIMEOUT);
      break;
    default:
      break;
  }

  // this new http connection has a specific hashkey (i.e. to a particular
  // host via the tunnel) and is associated with the tunnel streams
<<<<<<< HEAD
  LOG(("SpdyConnectTransaction new httpconnection %p %s\n", mTunneledConn.get(),
       aConnInfo->HashKey().get()));
||||||| merged common ancestors
  LOG(("SpdyConnectTransaction new httpconnection %p %s\n",
       mTunneledConn.get(), aConnInfo->HashKey().get()));
=======
  LOG(("SpdyConnectTransaction %p new httpconnection %p %s\n", this,
       mTunneledConn.get(), aConnInfo->HashKey().get()));
>>>>>>> upstream-releases

  nsCOMPtr<nsIInterfaceRequestor> callbacks;
  GetSecurityCallbacks(getter_AddRefs(callbacks));
  mTunneledConn->SetTransactionCaps(Caps());
  MOZ_ASSERT(aConnInfo->UsingHttpsProxy() || mIsWebsocket);
  TimeDuration rtt = TimeStamp::Now() - mTimestampSyn;
  DebugOnly<nsresult> rv = mTunneledConn->Init(
      aConnInfo, gHttpHandler->ConnMgr()->MaxRequestDelay(), mTunnelTransport,
      mTunnelStreamIn, mTunnelStreamOut, true, callbacks,
      PR_MillisecondsToInterval(static_cast<uint32_t>(rtt.ToMilliseconds())));
  MOZ_ASSERT(NS_SUCCEEDED(rv));
  if (mForcePlainText) {
    mTunneledConn->ForcePlainText();
  } else if (mIsWebsocket) {
    LOG(("SpdyConnectTransaction::MapStreamToHttpConnection %p websocket",
         this));
    // Let the root transaction know about us, so it can pass our own conn
    // to the websocket.
    mDrivingTransaction->SetH2WSTransaction(this);
  } else {
    mTunneledConn->SetupSecondaryTLS(this);
    mTunneledConn->SetInSpdyTunnel(true);
  }

  // make the originating transaction stick to the tunneled conn
  RefPtr<nsAHttpConnection> wrappedConn =
      gHttpHandler->ConnMgr()->MakeConnectionHandle(mTunneledConn);
  mDrivingTransaction->SetConnection(wrappedConn);
  mDrivingTransaction->MakeSticky();

  if (!mIsWebsocket) {
    // jump the priority and start the dispatcher
    Unused << gHttpHandler->InitiateTransaction(
        mDrivingTransaction, nsISupportsPriority::PRIORITY_HIGHEST - 60);
    mDrivingTransaction = nullptr;
  }
}

<<<<<<< HEAD
nsresult SpdyConnectTransaction::Flush(uint32_t count, uint32_t *countRead) {
||||||| merged common ancestors
nsresult
SpdyConnectTransaction::Flush(uint32_t count, uint32_t *countRead)
{
=======
nsresult SpdyConnectTransaction::Flush(uint32_t count, uint32_t* countRead) {
>>>>>>> upstream-releases
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  LOG(("SpdyConnectTransaction::Flush %p count %d avail %d\n", this, count,
       mOutputDataUsed - mOutputDataOffset));

  if (!mSegmentReader) {
    return NS_ERROR_UNEXPECTED;
  }

  *countRead = 0;
  count = std::min(count, (mOutputDataUsed - mOutputDataOffset));
  if (count) {
    nsresult rv;
    rv = mSegmentReader->OnReadSegment(&mOutputData[mOutputDataOffset], count,
                                       countRead);
    if (NS_FAILED(rv) && (rv != NS_BASE_STREAM_WOULD_BLOCK)) {
      LOG(("SpdyConnectTransaction::Flush %p Error %" PRIx32 "\n", this,
           static_cast<uint32_t>(rv)));
      CreateShimError(rv);
      return rv;
    }
  }

  mOutputDataOffset += *countRead;
  if (mOutputDataOffset == mOutputDataUsed) {
    mOutputDataOffset = mOutputDataUsed = 0;
  }
  if (!(*countRead)) {
    return NS_BASE_STREAM_WOULD_BLOCK;
  }

  if (mOutputDataUsed != mOutputDataOffset) {
    LOG(("SpdyConnectTransaction::Flush %p Incomplete %d\n", this,
         mOutputDataUsed - mOutputDataOffset));
    mSession->TransactionHasDataToWrite(this);
  }

  return NS_OK;
}

<<<<<<< HEAD
nsresult SpdyConnectTransaction::ReadSegments(nsAHttpSegmentReader *reader,
                                              uint32_t count,
                                              uint32_t *countRead) {
||||||| merged common ancestors
nsresult
SpdyConnectTransaction::ReadSegments(nsAHttpSegmentReader *reader,
                                     uint32_t count,
                                     uint32_t *countRead)
{
=======
nsresult SpdyConnectTransaction::ReadSegments(nsAHttpSegmentReader* reader,
                                              uint32_t count,
                                              uint32_t* countRead) {
>>>>>>> upstream-releases
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  LOG(("SpdyConnectTransaction::ReadSegments %p count %d conn %p\n", this,
       count, mTunneledConn.get()));

  mSegmentReader = reader;

  // spdy stream carrying tunnel is not setup yet.
  if (!mTunneledConn) {
    uint32_t toWrite = mConnectString.Length() - mConnectStringOffset;
    toWrite = std::min(toWrite, count);
    *countRead = toWrite;
    if (toWrite) {
      nsresult rv = mSegmentReader->OnReadSegment(
          mConnectString.BeginReading() + mConnectStringOffset, toWrite,
          countRead);
      if (NS_FAILED(rv) && (rv != NS_BASE_STREAM_WOULD_BLOCK)) {
        LOG(
            ("SpdyConnectTransaction::ReadSegments %p OnReadSegmentError "
             "%" PRIx32 "\n",
             this, static_cast<uint32_t>(rv)));
        CreateShimError(rv);
      } else {
        mConnectStringOffset += toWrite;
        if (mConnectString.Length() == mConnectStringOffset) {
          mConnectString.Truncate();
          mConnectStringOffset = 0;
        }
      }
      return rv;
    }

    LOG(("SpdyConnectTransaciton::ReadSegments %p connect request consumed",
         this));
    return NS_BASE_STREAM_WOULD_BLOCK;
  }

  if (mForcePlainText) {
    // this path just ignores sending the request so that we can
    // send a synthetic reply in writesegments()
    LOG(
        ("SpdyConnectTransaciton::ReadSegments %p dropping %d output bytes "
         "due to synthetic reply\n",
         this, mOutputDataUsed - mOutputDataOffset));
    *countRead = mOutputDataUsed - mOutputDataOffset;
    mOutputDataOffset = mOutputDataUsed = 0;
    mTunneledConn->DontReuse();
    return NS_OK;
  }

  *countRead = 0;
<<<<<<< HEAD
  nsresult rv = Flush(count, countRead);
  nsIOutputStreamCallback *cb = mTunnelStreamOut->GetCallback();
  if (!cb && !(*countRead)) {
||||||| merged common ancestors
  Unused << Flush(count, countRead);
  if (!mTunnelStreamOut->mCallback) {
=======
  nsresult rv = Flush(count, countRead);
  nsIOutputStreamCallback* cb = mTunnelStreamOut->GetCallback();
  if (!cb && !(*countRead)) {
>>>>>>> upstream-releases
    return NS_BASE_STREAM_WOULD_BLOCK;
  }

  if (cb) {
    // See if there is any more data available
    rv = cb->OnOutputStreamReady(mTunnelStreamOut);
    if (NS_FAILED(rv)) {
      return rv;
    }

    // Write out anything that may have come out of the stream just above
    uint32_t subtotal;
    count -= *countRead;
    rv = Flush(count, &subtotal);
    *countRead += subtotal;
  }

  return rv;
}

<<<<<<< HEAD
void SpdyConnectTransaction::CreateShimError(nsresult code) {
||||||| merged common ancestors
void
SpdyConnectTransaction::CreateShimError(nsresult code)
{
=======
void SpdyConnectTransaction::CreateShimError(nsresult code) {
  LOG(("SpdyConnectTransaction::CreateShimError %p 0x%08" PRIx32, this,
       static_cast<uint32_t>(code)));

>>>>>>> upstream-releases
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  MOZ_ASSERT(NS_FAILED(code));

  MOZ_ASSERT(!mCreateShimErrorCalled);
  if (mCreateShimErrorCalled) {
    return;
  }
  mCreateShimErrorCalled = true;

  if (mTunnelStreamOut && NS_SUCCEEDED(mTunnelStreamOut->mStatus)) {
    mTunnelStreamOut->mStatus = code;
  }

  if (mTunnelStreamIn && NS_SUCCEEDED(mTunnelStreamIn->mStatus)) {
    mTunnelStreamIn->mStatus = code;
  }

<<<<<<< HEAD
  if (mTunnelStreamIn) {
    nsIInputStreamCallback *cb = mTunnelStreamIn->GetCallback();
    if (cb) {
      cb->OnInputStreamReady(mTunnelStreamIn);
    }
||||||| merged common ancestors
  if (mTunnelStreamIn && mTunnelStreamIn->mCallback) {
    mTunnelStreamIn->mCallback->OnInputStreamReady(mTunnelStreamIn);
=======
  if (mTunnelStreamIn) {
    nsIInputStreamCallback* cb = mTunnelStreamIn->GetCallback();
    if (cb) {
      cb->OnInputStreamReady(mTunnelStreamIn);
    }
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  if (mTunnelStreamOut) {
    nsIOutputStreamCallback *cb = mTunnelStreamOut->GetCallback();
    if (cb) {
      cb->OnOutputStreamReady(mTunnelStreamOut);
    }
||||||| merged common ancestors
  if (mTunnelStreamOut && mTunnelStreamOut->mCallback) {
    mTunnelStreamOut->mCallback->OnOutputStreamReady(mTunnelStreamOut);
=======
  if (mTunnelStreamOut) {
    nsIOutputStreamCallback* cb = mTunnelStreamOut->GetCallback();
    if (cb) {
      cb->OnOutputStreamReady(mTunnelStreamOut);
    }
>>>>>>> upstream-releases
  }
  mCreateShimErrorCalled = false;
}

<<<<<<< HEAD
nsresult SpdyConnectTransaction::WriteDataToBuffer(nsAHttpSegmentWriter *writer,
                                                   uint32_t count,
                                                   uint32_t *countWritten) {
  EnsureBuffer(mInputData, mInputDataUsed + count, mInputDataUsed,
               mInputDataSize);
  nsresult rv =
      writer->OnWriteSegment(&mInputData[mInputDataUsed], count, countWritten);
||||||| merged common ancestors
nsresult
SpdyConnectTransaction::WriteSegments(nsAHttpSegmentWriter *writer,
                                      uint32_t count,
                                      uint32_t *countWritten)
{
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  LOG(("SpdyConnectTransaction::WriteSegments %p max=%d cb=%p\n",
       this, count, mTunneledConn ? mTunnelStreamIn->mCallback : nullptr));

  // first call into the tunnel stream to get the demux'd data out of the
  // spdy session.
  EnsureBuffer(mInputData, mInputDataUsed + count, mInputDataUsed, mInputDataSize);
  nsresult rv = writer->OnWriteSegment(&mInputData[mInputDataUsed],
                                       count, countWritten);
=======
nsresult SpdyConnectTransaction::WriteDataToBuffer(nsAHttpSegmentWriter* writer,
                                                   uint32_t count,
                                                   uint32_t* countWritten) {
  EnsureBuffer(mInputData, mInputDataUsed + count, mInputDataUsed,
               mInputDataSize);
  nsresult rv =
      writer->OnWriteSegment(&mInputData[mInputDataUsed], count, countWritten);
>>>>>>> upstream-releases
  if (NS_FAILED(rv)) {
    if (rv != NS_BASE_STREAM_WOULD_BLOCK) {
      LOG(
          ("SpdyConnectTransaction::WriteSegments wrapped writer %p Error "
           "%" PRIx32 "\n",
           this, static_cast<uint32_t>(rv)));
      CreateShimError(rv);
    }
    return rv;
  }
  mInputDataUsed += *countWritten;
  LOG(
      ("SpdyConnectTransaction %p %d new bytes [%d total] of ciphered data "
       "buffered\n",
       this, *countWritten, mInputDataUsed - mInputDataOffset));

<<<<<<< HEAD
  return rv;
}

nsresult SpdyConnectTransaction::WriteSegments(nsAHttpSegmentWriter *writer,
                                               uint32_t count,
                                               uint32_t *countWritten) {
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  nsIInputStreamCallback *cb =
      mTunneledConn ? mTunnelStreamIn->GetCallback() : nullptr;
  LOG(("SpdyConnectTransaction::WriteSegments %p max=%d cb=%p\n", this, count,
       cb));

  // For websockets, we need to forward the initial response through to the base
  // transaction so the normal websocket plumbing can do all the things it needs
  // to do.
  if (mIsWebsocket) {
    return WebsocketWriteSegments(writer, count, countWritten);
  }

  // first call into the tunnel stream to get the demux'd data out of the
  // spdy session.
  nsresult rv = WriteDataToBuffer(writer, count, countWritten);
  if (NS_FAILED(rv)) {
    return rv;
  }

  if (!mTunneledConn || !cb) {
||||||| merged common ancestors
  if (!mTunneledConn || !mTunnelStreamIn->mCallback) {
=======
  return rv;
}

nsresult SpdyConnectTransaction::WriteSegments(nsAHttpSegmentWriter* writer,
                                               uint32_t count,
                                               uint32_t* countWritten) {
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  nsIInputStreamCallback* cb =
      mTunneledConn ? mTunnelStreamIn->GetCallback() : nullptr;
  LOG(("SpdyConnectTransaction::WriteSegments %p max=%d cb=%p\n", this, count,
       cb));

  // For websockets, we need to forward the initial response through to the base
  // transaction so the normal websocket plumbing can do all the things it needs
  // to do.
  if (mIsWebsocket) {
    return WebsocketWriteSegments(writer, count, countWritten);
  }

  // first call into the tunnel stream to get the demux'd data out of the
  // spdy session.
  nsresult rv = WriteDataToBuffer(writer, count, countWritten);
  if (NS_FAILED(rv)) {
    return rv;
  }

  if (!mTunneledConn || !cb) {
>>>>>>> upstream-releases
    return NS_BASE_STREAM_WOULD_BLOCK;
  }

  rv = cb->OnInputStreamReady(mTunnelStreamIn);
  LOG(
      ("SpdyConnectTransaction::WriteSegments %p "
       "after InputStreamReady callback %d total of ciphered data buffered "
       "rv=%" PRIx32 "\n",
       this, mInputDataUsed - mInputDataOffset, static_cast<uint32_t>(rv)));
  LOG(
      ("SpdyConnectTransaction::WriteSegments %p "
       "goodput %p out %" PRId64 "\n",
       this, mTunneledConn.get(), mTunneledConn->ContentBytesWritten()));
  if (NS_SUCCEEDED(rv) && !mTunneledConn->ContentBytesWritten()) {
<<<<<<< HEAD
    nsIOutputStreamCallback *ocb = mTunnelStreamOut->GetCallback();
    mTunnelStreamOut->AsyncWait(ocb, 0, 0, nullptr);
  }
  return rv;
}

nsresult SpdyConnectTransaction::WebsocketWriteSegments(
    nsAHttpSegmentWriter *writer, uint32_t count, uint32_t *countWritten) {
  MOZ_ASSERT(mIsWebsocket);
  if (mDrivingTransaction && !mDrivingTransaction->IsDone()) {
    // Transaction hasn't received end of headers yet, so keep passing data to
    // it until it has. Then we can take over.
    nsresult rv =
        mDrivingTransaction->WriteSegments(writer, count, countWritten);
    if (NS_SUCCEEDED(rv) && mDrivingTransaction->IsDone() && !mConnRefTaken) {
      mDrivingTransaction->Close(NS_OK);
    }
||||||| merged common ancestors
    mTunnelStreamOut->AsyncWait(mTunnelStreamOut->mCallback, 0, 0, nullptr);
=======
    nsIOutputStreamCallback* ocb = mTunnelStreamOut->GetCallback();
    mTunnelStreamOut->AsyncWait(ocb, 0, 0, nullptr);
>>>>>>> upstream-releases
  }

  if (!mConnRefTaken) {
    // Force driving transaction to finish so the websocket channel can get its
    // notifications correctly and start driving.
    MOZ_ASSERT(mDrivingTransaction);
    mDrivingTransaction->Close(NS_OK);
  }

  nsresult rv = WriteDataToBuffer(writer, count, countWritten);
  if (NS_SUCCEEDED(rv)) {
    if (!mTunneledConn || !mTunnelStreamIn->GetCallback()) {
      return NS_BASE_STREAM_WOULD_BLOCK;
    }
    nsIInputStreamCallback *cb = mTunnelStreamIn->GetCallback();
    rv = cb->OnInputStreamReady(mTunnelStreamIn);
  }

  return rv;
}

<<<<<<< HEAD
bool SpdyConnectTransaction::ConnectedReadyForInput() {
  return mTunneledConn && mTunnelStreamIn->GetCallback();
||||||| merged common ancestors
bool
SpdyConnectTransaction::ConnectedReadyForInput()
{
  return mTunneledConn && mTunnelStreamIn->mCallback;
=======
nsresult SpdyConnectTransaction::WebsocketWriteSegments(
    nsAHttpSegmentWriter* writer, uint32_t count, uint32_t* countWritten) {
  MOZ_ASSERT(mIsWebsocket);
  if (mDrivingTransaction && !mDrivingTransaction->IsDone()) {
    // Transaction hasn't received end of headers yet, so keep passing data to
    // it until it has. Then we can take over.
    nsresult rv =
        mDrivingTransaction->WriteSegments(writer, count, countWritten);
    if (NS_SUCCEEDED(rv) && mDrivingTransaction->IsDone() && !mConnRefTaken) {
      mDrivingTransaction->Close(NS_OK);
    }
  }

  if (!mConnRefTaken) {
    // Force driving transaction to finish so the websocket channel can get its
    // notifications correctly and start driving.
    MOZ_ASSERT(mDrivingTransaction);
    mDrivingTransaction->Close(NS_OK);
  }

  nsresult rv = WriteDataToBuffer(writer, count, countWritten);
  if (NS_SUCCEEDED(rv)) {
    if (!mTunneledConn || !mTunnelStreamIn->GetCallback()) {
      return NS_BASE_STREAM_WOULD_BLOCK;
    }
    nsIInputStreamCallback* cb = mTunnelStreamIn->GetCallback();
    rv = cb->OnInputStreamReady(mTunnelStreamIn);
  }

  return rv;
}

bool SpdyConnectTransaction::ConnectedReadyForInput() {
  return mTunneledConn && mTunnelStreamIn->GetCallback();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsHttpRequestHead *SpdyConnectTransaction::RequestHead() {
||||||| merged common ancestors
nsHttpRequestHead *
SpdyConnectTransaction::RequestHead()
{
=======
nsHttpRequestHead* SpdyConnectTransaction::RequestHead() {
>>>>>>> upstream-releases
  return mRequestHead;
}

void SpdyConnectTransaction::Close(nsresult code) {
  LOG(("SpdyConnectTransaction close %p %" PRIx32 "\n", this,
       static_cast<uint32_t>(code)));

  if (mIsWebsocket && mDrivingTransaction) {
    mDrivingTransaction->SetH2WSTransaction(nullptr);
    if (!mConnRefTaken) {
      // This indicates that the websocket failed to set up, so just close down
      // the transaction as usual.
      mDrivingTransaction->Close(code);
      mDrivingTransaction = nullptr;
    }
  }
  NullHttpTransaction::Close(code);
  if (NS_FAILED(code) && (code != NS_BASE_STREAM_WOULD_BLOCK)) {
    CreateShimError(code);
  } else {
    CreateShimError(NS_BASE_STREAM_CLOSED);
  }
}

<<<<<<< HEAD
void SpdyConnectTransaction::SetConnRefTaken() {
  mConnRefTaken = true;
  mDrivingTransaction = nullptr;  // Just in case
}

nsIOutputStreamCallback *OutputStreamShim::GetCallback() {
  mozilla::MutexAutoLock lock(mMutex);
  return mCallback;
}

class WebsocketHasDataToWrite final : public Runnable {
 public:
  explicit WebsocketHasDataToWrite(OutputStreamShim *shim)
      : Runnable("WebsocketHasDataToWrite"), mShim(shim) {}

  ~WebsocketHasDataToWrite() = default;

  NS_IMETHOD Run() override { return mShim->CallTransactionHasDataToWrite(); }

  MOZ_MUST_USE nsresult Dispatch() {
    if (OnSocketThread()) {
      return Run();
    }

    nsCOMPtr<nsIEventTarget> sts =
        do_GetService("@mozilla.org/network/socket-transport-service;1");
    return sts->Dispatch(this, nsIEventTarget::DISPATCH_NORMAL);
  }

 private:
  RefPtr<OutputStreamShim> mShim;
};

||||||| merged common ancestors
=======
void SpdyConnectTransaction::SetConnRefTaken() {
  mConnRefTaken = true;
  mDrivingTransaction = nullptr;  // Just in case
}

nsIOutputStreamCallback* OutputStreamShim::GetCallback() {
  mozilla::MutexAutoLock lock(mMutex);
  return mCallback;
}

class WebsocketHasDataToWrite final : public Runnable {
 public:
  explicit WebsocketHasDataToWrite(OutputStreamShim* shim)
      : Runnable("WebsocketHasDataToWrite"), mShim(shim) {}

  ~WebsocketHasDataToWrite() = default;

  NS_IMETHOD Run() override { return mShim->CallTransactionHasDataToWrite(); }

  MOZ_MUST_USE nsresult Dispatch() {
    if (OnSocketThread()) {
      return Run();
    }

    nsCOMPtr<nsIEventTarget> sts =
        do_GetService("@mozilla.org/network/socket-transport-service;1");
    return sts->Dispatch(this, nsIEventTarget::DISPATCH_NORMAL);
  }

 private:
  RefPtr<OutputStreamShim> mShim;
};

>>>>>>> upstream-releases
NS_IMETHODIMP
<<<<<<< HEAD
OutputStreamShim::AsyncWait(nsIOutputStreamCallback *callback,
                            unsigned int flags, unsigned int requestedCount,
                            nsIEventTarget *target) {
  if (mIsWebsocket) {
    // With websockets, AsyncWait may be called from the main thread, but the
    // target is on the socket thread. That's all we really care about.
    nsCOMPtr<nsIEventTarget> sts =
        do_GetService(NS_SOCKETTRANSPORTSERVICE_CONTRACTID);
    MOZ_ASSERT((!target && !callback) || (target == sts));
    if (target && (target != sts)) {
      return NS_ERROR_FAILURE;
    }
  } else {
    MOZ_ASSERT(OnSocketThread(), "not on socket thread");
    bool currentThread;
||||||| merged common ancestors
OutputStreamShim::AsyncWait(nsIOutputStreamCallback *callback,
                            unsigned int, unsigned int, nsIEventTarget *target)
{
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  bool currentThread;
=======
OutputStreamShim::AsyncWait(nsIOutputStreamCallback* callback,
                            unsigned int flags, unsigned int requestedCount,
                            nsIEventTarget* target) {
  if (mIsWebsocket) {
    // With websockets, AsyncWait may be called from the main thread, but the
    // target is on the socket thread. That's all we really care about.
    nsCOMPtr<nsIEventTarget> sts =
        do_GetService(NS_SOCKETTRANSPORTSERVICE_CONTRACTID);
    MOZ_ASSERT((!target && !callback) || (target == sts));
    if (target && (target != sts)) {
      return NS_ERROR_FAILURE;
    }
  } else {
    MOZ_ASSERT(OnSocketThread(), "not on socket thread");
    bool currentThread;
>>>>>>> upstream-releases

    if (target && (NS_FAILED(target->IsOnCurrentThread(&currentThread)) ||
                   !currentThread)) {
      return NS_ERROR_FAILURE;
    }
  }

  LOG(("OutputStreamShim::AsyncWait %p callback %p\n", this, callback));

  {
    mozilla::MutexAutoLock lock(mMutex);
    mCallback = callback;
  }

  RefPtr<WebsocketHasDataToWrite> wsdw = new WebsocketHasDataToWrite(this);
  Unused << wsdw->Dispatch();

  return NS_OK;
}

<<<<<<< HEAD
class OutputCloseTransaction final : public Runnable {
 public:
  OutputCloseTransaction(OutputStreamShim *shim, nsresult reason)
      : Runnable("OutputCloseTransaction"), mShim(shim), mReason(reason) {}

  ~OutputCloseTransaction() = default;

  NS_IMETHOD Run() override { return mShim->CloseTransaction(mReason); }

 private:
  RefPtr<OutputStreamShim> mShim;
  nsresult mReason;
};

||||||| merged common ancestors
=======
class OutputCloseTransaction final : public Runnable {
 public:
  OutputCloseTransaction(OutputStreamShim* shim, nsresult reason)
      : Runnable("OutputCloseTransaction"), mShim(shim), mReason(reason) {}

  ~OutputCloseTransaction() = default;

  NS_IMETHOD Run() override { return mShim->CloseTransaction(mReason); }

 private:
  RefPtr<OutputStreamShim> mShim;
  nsresult mReason;
};

>>>>>>> upstream-releases
NS_IMETHODIMP
OutputStreamShim::CloseWithStatus(nsresult reason) {
  if (!OnSocketThread()) {
    RefPtr<OutputCloseTransaction> oct =
        new OutputCloseTransaction(this, reason);
    nsCOMPtr<nsIEventTarget> sts =
        do_GetService("@mozilla.org/network/socket-transport-service;1");
    return sts->Dispatch(oct, nsIEventTarget::DISPATCH_NORMAL);
  }

  return CloseTransaction(reason);
}

nsresult OutputStreamShim::CloseTransaction(nsresult reason) {
  MOZ_ASSERT(OnSocketThread());
  RefPtr<NullHttpTransaction> baseTrans = mWeakTrans->QueryTransaction();
  if (!baseTrans) {
    return NS_ERROR_FAILURE;
  }
  SpdyConnectTransaction* trans = baseTrans->QuerySpdyConnectTransaction();
  MOZ_ASSERT(trans);
  if (!trans) {
    return NS_ERROR_UNEXPECTED;
  }

  trans->mSession->CloseTransaction(trans, reason);
  return NS_OK;
}

NS_IMETHODIMP
OutputStreamShim::Close() { return CloseWithStatus(NS_OK); }

NS_IMETHODIMP
OutputStreamShim::Flush() {
  MOZ_ASSERT(OnSocketThread());
  RefPtr<NullHttpTransaction> baseTrans = mWeakTrans->QueryTransaction();
  if (!baseTrans) {
    return NS_ERROR_FAILURE;
  }
  SpdyConnectTransaction* trans = baseTrans->QuerySpdyConnectTransaction();
  MOZ_ASSERT(trans);
  if (!trans) {
    return NS_ERROR_UNEXPECTED;
  }

  uint32_t count = trans->mOutputDataUsed - trans->mOutputDataOffset;
  if (!count) {
    return NS_OK;
  }

  uint32_t countRead;
  nsresult rv = trans->Flush(count, &countRead);
  LOG(("OutputStreamShim::Flush %p before %d after %d\n", this, count,
       trans->mOutputDataUsed - trans->mOutputDataOffset));
  return rv;
}

<<<<<<< HEAD
nsresult OutputStreamShim::CallTransactionHasDataToWrite() {
  MOZ_ASSERT(OnSocketThread());
  RefPtr<NullHttpTransaction> baseTrans = mWeakTrans->QueryTransaction();
  if (!baseTrans) {
    return NS_ERROR_FAILURE;
  }
  SpdyConnectTransaction *trans = baseTrans->QuerySpdyConnectTransaction();
  MOZ_ASSERT(trans);
  if (!trans) {
    return NS_ERROR_UNEXPECTED;
  }
  trans->mSession->TransactionHasDataToWrite(trans);
  return NS_OK;
}

||||||| merged common ancestors
=======
nsresult OutputStreamShim::CallTransactionHasDataToWrite() {
  MOZ_ASSERT(OnSocketThread());
  RefPtr<NullHttpTransaction> baseTrans = mWeakTrans->QueryTransaction();
  if (!baseTrans) {
    return NS_ERROR_FAILURE;
  }
  SpdyConnectTransaction* trans = baseTrans->QuerySpdyConnectTransaction();
  MOZ_ASSERT(trans);
  if (!trans) {
    return NS_ERROR_UNEXPECTED;
  }
  trans->mSession->TransactionHasDataToWrite(trans);
  return NS_OK;
}

>>>>>>> upstream-releases
NS_IMETHODIMP
<<<<<<< HEAD
OutputStreamShim::Write(const char *aBuf, uint32_t aCount, uint32_t *_retval) {
||||||| merged common ancestors
OutputStreamShim::Write(const char * aBuf, uint32_t aCount, uint32_t *_retval)
{
=======
OutputStreamShim::Write(const char* aBuf, uint32_t aCount, uint32_t* _retval) {
>>>>>>> upstream-releases
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");

  if (NS_FAILED(mStatus)) {
    return mStatus;
  }

  RefPtr<NullHttpTransaction> baseTrans = mWeakTrans->QueryTransaction();
  if (!baseTrans) {
    return NS_ERROR_FAILURE;
  }
  SpdyConnectTransaction* trans = baseTrans->QuerySpdyConnectTransaction();
  MOZ_ASSERT(trans);
  if (!trans) {
    return NS_ERROR_UNEXPECTED;
  }

  if ((trans->mOutputDataUsed + aCount) >= 512000) {
    *_retval = 0;
    // time for some flow control;
    return NS_BASE_STREAM_WOULD_BLOCK;
  }

  EnsureBuffer(trans->mOutputData, trans->mOutputDataUsed + aCount,
               trans->mOutputDataUsed, trans->mOutputDataSize);
  memcpy(&trans->mOutputData[trans->mOutputDataUsed], aBuf, aCount);
  trans->mOutputDataUsed += aCount;
  *_retval = aCount;
  LOG(("OutputStreamShim::Write %p new %d total %d\n", this, aCount,
       trans->mOutputDataUsed));

  trans->mSession->TransactionHasDataToWrite(trans);

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
OutputStreamShim::WriteFrom(nsIInputStream *aFromStream, uint32_t aCount,
                            uint32_t *_retval) {
  if (mIsWebsocket) {
    LOG3(("WARNING: OutputStreamShim::WriteFrom %p", this));
  }
||||||| merged common ancestors
OutputStreamShim::WriteFrom(nsIInputStream *aFromStream, uint32_t aCount, uint32_t *_retval)
{
=======
OutputStreamShim::WriteFrom(nsIInputStream* aFromStream, uint32_t aCount,
                            uint32_t* _retval) {
  if (mIsWebsocket) {
    LOG3(("WARNING: OutputStreamShim::WriteFrom %p", this));
  }
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
OutputStreamShim::WriteSegments(nsReadSegmentFun aReader, void *aClosure,
                                uint32_t aCount, uint32_t *_retval) {
  if (mIsWebsocket) {
    LOG3(("WARNING: OutputStreamShim::WriteSegments %p", this));
  }
||||||| merged common ancestors
OutputStreamShim::WriteSegments(nsReadSegmentFun aReader, void *aClosure, uint32_t aCount, uint32_t *_retval)
{
=======
OutputStreamShim::WriteSegments(nsReadSegmentFun aReader, void* aClosure,
                                uint32_t aCount, uint32_t* _retval) {
  if (mIsWebsocket) {
    LOG3(("WARNING: OutputStreamShim::WriteSegments %p", this));
  }
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
OutputStreamShim::IsNonBlocking(bool *_retval) {
||||||| merged common ancestors
OutputStreamShim::IsNonBlocking(bool *_retval)
{
=======
OutputStreamShim::IsNonBlocking(bool* _retval) {
>>>>>>> upstream-releases
  *_retval = true;
  return NS_OK;
}

<<<<<<< HEAD
nsIInputStreamCallback *InputStreamShim::GetCallback() {
  mozilla::MutexAutoLock lock(mMutex);
  return mCallback;
}

||||||| merged common ancestors
=======
nsIInputStreamCallback* InputStreamShim::GetCallback() {
  mozilla::MutexAutoLock lock(mMutex);
  return mCallback;
}

>>>>>>> upstream-releases
NS_IMETHODIMP
<<<<<<< HEAD
InputStreamShim::AsyncWait(nsIInputStreamCallback *callback, unsigned int flags,
                           unsigned int requestedCount,
                           nsIEventTarget *target) {
  if (mIsWebsocket) {
    // With websockets, AsyncWait may be called from the main thread, but the
    // target is on the socket thread. That's all we really care about.
    nsCOMPtr<nsIEventTarget> sts =
        do_GetService(NS_SOCKETTRANSPORTSERVICE_CONTRACTID);
    MOZ_ASSERT((!target && !callback) || (target == sts));
    if (target && (target != sts)) {
      return NS_ERROR_FAILURE;
    }
  } else {
    MOZ_ASSERT(OnSocketThread(), "not on socket thread");
    bool currentThread;
||||||| merged common ancestors
InputStreamShim::AsyncWait(nsIInputStreamCallback *callback,
                           unsigned int, unsigned int, nsIEventTarget *target)
{
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");
  bool currentThread;
=======
InputStreamShim::AsyncWait(nsIInputStreamCallback* callback, unsigned int flags,
                           unsigned int requestedCount,
                           nsIEventTarget* target) {
  if (mIsWebsocket) {
    // With websockets, AsyncWait may be called from the main thread, but the
    // target is on the socket thread. That's all we really care about.
    nsCOMPtr<nsIEventTarget> sts =
        do_GetService(NS_SOCKETTRANSPORTSERVICE_CONTRACTID);
    MOZ_ASSERT((!target && !callback) || (target == sts));
    if (target && (target != sts)) {
      return NS_ERROR_FAILURE;
    }
  } else {
    MOZ_ASSERT(OnSocketThread(), "not on socket thread");
    bool currentThread;
>>>>>>> upstream-releases

    if (target && (NS_FAILED(target->IsOnCurrentThread(&currentThread)) ||
                   !currentThread)) {
      return NS_ERROR_FAILURE;
    }
  }

  LOG(("InputStreamShim::AsyncWait %p callback %p\n", this, callback));

  {
    mozilla::MutexAutoLock lock(mMutex);
    mCallback = callback;
  }

  return NS_OK;
}

<<<<<<< HEAD
class InputCloseTransaction final : public Runnable {
 public:
  InputCloseTransaction(InputStreamShim *shim, nsresult reason)
      : Runnable("InputCloseTransaction"), mShim(shim), mReason(reason) {}

  ~InputCloseTransaction() = default;

  NS_IMETHOD Run() override { return mShim->CloseTransaction(mReason); }

 private:
  RefPtr<InputStreamShim> mShim;
  nsresult mReason;
};

||||||| merged common ancestors
=======
class InputCloseTransaction final : public Runnable {
 public:
  InputCloseTransaction(InputStreamShim* shim, nsresult reason)
      : Runnable("InputCloseTransaction"), mShim(shim), mReason(reason) {}

  ~InputCloseTransaction() = default;

  NS_IMETHOD Run() override { return mShim->CloseTransaction(mReason); }

 private:
  RefPtr<InputStreamShim> mShim;
  nsresult mReason;
};

>>>>>>> upstream-releases
NS_IMETHODIMP
InputStreamShim::CloseWithStatus(nsresult reason) {
  if (!OnSocketThread()) {
    RefPtr<InputCloseTransaction> ict = new InputCloseTransaction(this, reason);
    nsCOMPtr<nsIEventTarget> sts =
        do_GetService("@mozilla.org/network/socket-transport-service;1");
    return sts->Dispatch(ict, nsIEventTarget::DISPATCH_NORMAL);
  }

  return CloseTransaction(reason);
}

nsresult InputStreamShim::CloseTransaction(nsresult reason) {
  MOZ_ASSERT(OnSocketThread());
  RefPtr<NullHttpTransaction> baseTrans = mWeakTrans->QueryTransaction();
  if (!baseTrans) {
    return NS_ERROR_FAILURE;
  }
  SpdyConnectTransaction* trans = baseTrans->QuerySpdyConnectTransaction();
  MOZ_ASSERT(trans);
  if (!trans) {
    return NS_ERROR_UNEXPECTED;
  }

  trans->mSession->CloseTransaction(trans, reason);
  return NS_OK;
}

NS_IMETHODIMP
InputStreamShim::Close() { return CloseWithStatus(NS_OK); }

NS_IMETHODIMP
<<<<<<< HEAD
InputStreamShim::Available(uint64_t *_retval) {
  RefPtr<NullHttpTransaction> baseTrans = mWeakTrans->QueryTransaction();
||||||| merged common ancestors
InputStreamShim::Available(uint64_t *_retval)
{
  RefPtr<NullHttpTransaction> baseTrans(do_QueryReferent(mWeakTrans));
=======
InputStreamShim::Available(uint64_t* _retval) {
  RefPtr<NullHttpTransaction> baseTrans = mWeakTrans->QueryTransaction();
>>>>>>> upstream-releases
  if (!baseTrans) {
    return NS_ERROR_FAILURE;
  }
  SpdyConnectTransaction* trans = baseTrans->QuerySpdyConnectTransaction();
  MOZ_ASSERT(trans);
  if (!trans) {
    return NS_ERROR_UNEXPECTED;
  }

  *_retval = trans->mInputDataUsed - trans->mInputDataOffset;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
InputStreamShim::Read(char *aBuf, uint32_t aCount, uint32_t *_retval) {
||||||| merged common ancestors
InputStreamShim::Read(char *aBuf, uint32_t aCount, uint32_t *_retval)
{
=======
InputStreamShim::Read(char* aBuf, uint32_t aCount, uint32_t* _retval) {
>>>>>>> upstream-releases
  MOZ_ASSERT(OnSocketThread(), "not on socket thread");

  if (NS_FAILED(mStatus)) {
    return mStatus;
  }

  RefPtr<NullHttpTransaction> baseTrans = mWeakTrans->QueryTransaction();
  if (!baseTrans) {
    return NS_ERROR_FAILURE;
  }
  SpdyConnectTransaction* trans = baseTrans->QuerySpdyConnectTransaction();
  MOZ_ASSERT(trans);
  if (!trans) {
    return NS_ERROR_UNEXPECTED;
  }

  uint32_t avail = trans->mInputDataUsed - trans->mInputDataOffset;
  uint32_t tocopy = std::min(aCount, avail);
  *_retval = tocopy;
  memcpy(aBuf, &trans->mInputData[trans->mInputDataOffset], tocopy);
  trans->mInputDataOffset += tocopy;
  if (trans->mInputDataOffset == trans->mInputDataUsed) {
    trans->mInputDataOffset = trans->mInputDataUsed = 0;
  }

  return tocopy ? NS_OK : NS_BASE_STREAM_WOULD_BLOCK;
}

NS_IMETHODIMP
<<<<<<< HEAD
InputStreamShim::ReadSegments(nsWriteSegmentFun aWriter, void *aClosure,
                              uint32_t aCount, uint32_t *_retval) {
  if (mIsWebsocket) {
    LOG3(("WARNING: InputStreamShim::ReadSegments %p", this));
  }
||||||| merged common ancestors
InputStreamShim::ReadSegments(nsWriteSegmentFun aWriter, void *aClosure,
                              uint32_t aCount, uint32_t *_retval)
{
=======
InputStreamShim::ReadSegments(nsWriteSegmentFun aWriter, void* aClosure,
                              uint32_t aCount, uint32_t* _retval) {
  if (mIsWebsocket) {
    LOG3(("WARNING: InputStreamShim::ReadSegments %p", this));
  }
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
InputStreamShim::IsNonBlocking(bool *_retval) {
||||||| merged common ancestors
InputStreamShim::IsNonBlocking(bool *_retval)
{
=======
InputStreamShim::IsNonBlocking(bool* _retval) {
>>>>>>> upstream-releases
  *_retval = true;
  return NS_OK;
}

NS_IMETHODIMP
SocketTransportShim::SetKeepaliveEnabled(bool aKeepaliveEnabled) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::SetKeepaliveEnabled %p called", this));
  }
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
SocketTransportShim::SetKeepaliveVals(int32_t keepaliveIdleTime,
                                      int32_t keepaliveRetryInterval) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::SetKeepaliveVals %p called", this));
  }
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
SocketTransportShim::GetSecurityCallbacks(
    nsIInterfaceRequestor **aSecurityCallbacks) {
  if (mIsWebsocket) {
    nsCOMPtr<nsIInterfaceRequestor> out(mSecurityCallbacks);
    *aSecurityCallbacks = out.forget().take();
    return NS_OK;
  }

  return mWrapped->GetSecurityCallbacks(aSecurityCallbacks);
}

NS_IMETHODIMP
SocketTransportShim::SetSecurityCallbacks(
    nsIInterfaceRequestor *aSecurityCallbacks) {
  if (mIsWebsocket) {
    mSecurityCallbacks = aSecurityCallbacks;
    return NS_OK;
  }
||||||| merged common ancestors
SocketTransportShim::SetSecurityCallbacks(nsIInterfaceRequestor *aSecurityCallbacks)
{
=======
SocketTransportShim::GetSecurityCallbacks(
    nsIInterfaceRequestor** aSecurityCallbacks) {
  if (mIsWebsocket) {
    nsCOMPtr<nsIInterfaceRequestor> out(mSecurityCallbacks);
    *aSecurityCallbacks = out.forget().take();
    return NS_OK;
  }

  return mWrapped->GetSecurityCallbacks(aSecurityCallbacks);
}

NS_IMETHODIMP
SocketTransportShim::SetSecurityCallbacks(
    nsIInterfaceRequestor* aSecurityCallbacks) {
  if (mIsWebsocket) {
    mSecurityCallbacks = aSecurityCallbacks;
    return NS_OK;
  }
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
SocketTransportShim::OpenInputStream(uint32_t aFlags, uint32_t aSegmentSize,
<<<<<<< HEAD
                                     uint32_t aSegmentCount,
                                     nsIInputStream **_retval) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::OpenInputStream %p", this));
  }
||||||| merged common ancestors
                                     uint32_t aSegmentCount, nsIInputStream * *_retval)
{
=======
                                     uint32_t aSegmentCount,
                                     nsIInputStream** _retval) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::OpenInputStream %p", this));
  }
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
SocketTransportShim::OpenOutputStream(uint32_t aFlags, uint32_t aSegmentSize,
<<<<<<< HEAD
                                      uint32_t aSegmentCount,
                                      nsIOutputStream **_retval) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::OpenOutputStream %p", this));
  }
||||||| merged common ancestors
                                      uint32_t aSegmentCount, nsIOutputStream * *_retval)
{
=======
                                      uint32_t aSegmentCount,
                                      nsIOutputStream** _retval) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::OpenOutputStream %p", this));
  }
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
SocketTransportShim::Close(nsresult aReason) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::Close %p", this));
  }
||||||| merged common ancestors
SocketTransportShim::Close(nsresult aReason)
{
=======
SocketTransportShim::Close(nsresult aReason) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::Close %p", this));
  } else {
    LOG(("SocketTransportShim::Close %p", this));
  }

  if (gHttpHandler->Bug1563538()) {
    // Must always post, because mSession->CloseTransaction releases the
    // Http2Stream which is still on stack.
    RefPtr<SocketTransportShim> self(this);

    nsCOMPtr<nsIEventTarget> sts =
        do_GetService("@mozilla.org/network/socket-transport-service;1");
    Unused << sts->Dispatch(NS_NewRunnableFunction(
        "SocketTransportShim::Close", [self = std::move(self), aReason]() {
          RefPtr<NullHttpTransaction> baseTrans =
              self->mWeakTrans->QueryTransaction();
          if (!baseTrans) {
            return;
          }
          SpdyConnectTransaction* trans =
              baseTrans->QuerySpdyConnectTransaction();
          MOZ_ASSERT(trans);
          if (!trans) {
            return;
          }

          trans->mSession->CloseTransaction(trans, aReason);
        }));
    return NS_OK;
  }

>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
SocketTransportShim::SetEventSink(nsITransportEventSink *aSink,
                                  nsIEventTarget *aEventTarget) {
  if (mIsWebsocket) {
    // Need to pretend, since websockets expect this to work
    return NS_OK;
  }
||||||| merged common ancestors
SocketTransportShim::SetEventSink(nsITransportEventSink *aSink, nsIEventTarget *aEventTarget)
{
=======
SocketTransportShim::SetEventSink(nsITransportEventSink* aSink,
                                  nsIEventTarget* aEventTarget) {
  if (mIsWebsocket) {
    // Need to pretend, since websockets expect this to work
    return NS_OK;
  }
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
SocketTransportShim::Bind(NetAddr *aLocalAddr) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::Bind %p", this));
  }
||||||| merged common ancestors
SocketTransportShim::Bind(NetAddr *aLocalAddr)
{
=======
SocketTransportShim::Bind(NetAddr* aLocalAddr) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::Bind %p", this));
  }
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
SocketTransportShim::GetFirstRetryError(nsresult *aFirstRetryError) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::GetFirstRetryError %p", this));
  }
||||||| merged common ancestors
SocketTransportShim::GetFirstRetryError(nsresult *aFirstRetryError)
{
=======
SocketTransportShim::GetFirstRetryError(nsresult* aFirstRetryError) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::GetFirstRetryError %p", this));
  }
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
<<<<<<< HEAD
SocketTransportShim::GetEsniUsed(bool *aEsniUsed) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::GetEsniUsed %p", this));
  }
||||||| merged common ancestors
SocketTransportShim::GetEsniUsed(bool *aEsniUsed)
{
=======
SocketTransportShim::GetEsniUsed(bool* aEsniUsed) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::GetEsniUsed %p", this));
  }
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

<<<<<<< HEAD
#define FWD_TS_PTR(fx, ts) \
  NS_IMETHODIMP            \
  SocketTransportShim::fx(ts *arg) { return mWrapped->fx(arg); }
||||||| merged common ancestors
=======
NS_IMETHODIMP
SocketTransportShim::ResolvedByTRR(bool* aResolvedByTRR) {
  if (mIsWebsocket) {
    LOG3(("WARNING: SocketTransportShim::IsTRR %p", this));
  }
  return NS_ERROR_NOT_IMPLEMENTED;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
#define FWD_TS_ADDREF(fx, ts) \
  NS_IMETHODIMP               \
  SocketTransportShim::fx(ts **arg) { return mWrapped->fx(arg); }
||||||| merged common ancestors
#define FWD_TS_PTR(fx, ts) NS_IMETHODIMP \
SocketTransportShim::fx(ts *arg) { return mWrapped->fx(arg); }

#define FWD_TS_ADDREF(fx, ts) NS_IMETHODIMP \
SocketTransportShim::fx(ts **arg) { return mWrapped->fx(arg); }
=======
#define FWD_TS_PTR(fx, ts) \
  NS_IMETHODIMP            \
  SocketTransportShim::fx(ts* arg) { return mWrapped->fx(arg); }

#define FWD_TS_ADDREF(fx, ts) \
  NS_IMETHODIMP               \
  SocketTransportShim::fx(ts** arg) { return mWrapped->fx(arg); }
>>>>>>> upstream-releases

#define FWD_TS(fx, ts) \
  NS_IMETHODIMP        \
  SocketTransportShim::fx(ts arg) { return mWrapped->fx(arg); }

FWD_TS_PTR(GetKeepaliveEnabled, bool);
FWD_TS_PTR(GetSendBufferSize, uint32_t);
FWD_TS(SetSendBufferSize, uint32_t);
FWD_TS_PTR(GetPort, int32_t);
FWD_TS_PTR(GetPeerAddr, mozilla::net::NetAddr);
FWD_TS_PTR(GetSelfAddr, mozilla::net::NetAddr);
FWD_TS_ADDREF(GetScriptablePeerAddr, nsINetAddr);
FWD_TS_ADDREF(GetScriptableSelfAddr, nsINetAddr);
FWD_TS_ADDREF(GetSecurityInfo, nsISupports);
FWD_TS_PTR(IsAlive, bool);
FWD_TS_PTR(GetConnectionFlags, uint32_t);
FWD_TS(SetConnectionFlags, uint32_t);
FWD_TS_PTR(GetTlsFlags, uint32_t);
FWD_TS(SetTlsFlags, uint32_t);
FWD_TS_PTR(GetRecvBufferSize, uint32_t);
FWD_TS(SetRecvBufferSize, uint32_t);
FWD_TS_PTR(GetResetIPFamilyPreference, bool);

<<<<<<< HEAD
nsresult SocketTransportShim::GetOriginAttributes(
    mozilla::OriginAttributes *aOriginAttributes) {
||||||| merged common ancestors
nsresult
SocketTransportShim::GetOriginAttributes(mozilla::OriginAttributes* aOriginAttributes)
{
=======
nsresult SocketTransportShim::GetOriginAttributes(
    mozilla::OriginAttributes* aOriginAttributes) {
>>>>>>> upstream-releases
  return mWrapped->GetOriginAttributes(aOriginAttributes);
}

<<<<<<< HEAD
nsresult SocketTransportShim::SetOriginAttributes(
    const mozilla::OriginAttributes &aOriginAttributes) {
||||||| merged common ancestors
nsresult
SocketTransportShim::SetOriginAttributes(const mozilla::OriginAttributes& aOriginAttributes)
{
=======
nsresult SocketTransportShim::SetOriginAttributes(
    const mozilla::OriginAttributes& aOriginAttributes) {
>>>>>>> upstream-releases
  return mWrapped->SetOriginAttributes(aOriginAttributes);
}

NS_IMETHODIMP
<<<<<<< HEAD
SocketTransportShim::GetScriptableOriginAttributes(
    JSContext *aCx, JS::MutableHandle<JS::Value> aOriginAttributes) {
||||||| merged common ancestors
SocketTransportShim::GetScriptableOriginAttributes(JSContext* aCx,
  JS::MutableHandle<JS::Value> aOriginAttributes)
{
=======
SocketTransportShim::GetScriptableOriginAttributes(
    JSContext* aCx, JS::MutableHandle<JS::Value> aOriginAttributes) {
>>>>>>> upstream-releases
  return mWrapped->GetScriptableOriginAttributes(aCx, aOriginAttributes);
}

NS_IMETHODIMP
<<<<<<< HEAD
SocketTransportShim::SetScriptableOriginAttributes(
    JSContext *aCx, JS::Handle<JS::Value> aOriginAttributes) {
||||||| merged common ancestors
SocketTransportShim::SetScriptableOriginAttributes(JSContext* aCx,
  JS::Handle<JS::Value> aOriginAttributes)
{
=======
SocketTransportShim::SetScriptableOriginAttributes(
    JSContext* aCx, JS::Handle<JS::Value> aOriginAttributes) {
>>>>>>> upstream-releases
  return mWrapped->SetScriptableOriginAttributes(aCx, aOriginAttributes);
}

NS_IMETHODIMP
<<<<<<< HEAD
SocketTransportShim::GetHost(nsACString &aHost) {
||||||| merged common ancestors
SocketTransportShim::GetHost(nsACString & aHost)
{
=======
SocketTransportShim::GetHost(nsACString& aHost) {
>>>>>>> upstream-releases
  return mWrapped->GetHost(aHost);
}

NS_IMETHODIMP
<<<<<<< HEAD
SocketTransportShim::GetTimeout(uint32_t aType, uint32_t *_retval) {
||||||| merged common ancestors
SocketTransportShim::GetTimeout(uint32_t aType, uint32_t *_retval)
{
=======
SocketTransportShim::GetTimeout(uint32_t aType, uint32_t* _retval) {
>>>>>>> upstream-releases
  return mWrapped->GetTimeout(aType, _retval);
}

NS_IMETHODIMP
SocketTransportShim::SetTimeout(uint32_t aType, uint32_t aValue) {
  return mWrapped->SetTimeout(aType, aValue);
}

NS_IMETHODIMP
SocketTransportShim::SetReuseAddrPort(bool aReuseAddrPort) {
  return mWrapped->SetReuseAddrPort(aReuseAddrPort);
}

NS_IMETHODIMP
<<<<<<< HEAD
SocketTransportShim::GetQoSBits(uint8_t *aQoSBits) {
||||||| merged common ancestors
SocketTransportShim::GetQoSBits(uint8_t *aQoSBits)
{
=======
SocketTransportShim::SetLinger(bool aPolarity, int16_t aTimeout) {
  return mWrapped->SetLinger(aPolarity, aTimeout);
}

NS_IMETHODIMP
SocketTransportShim::GetQoSBits(uint8_t* aQoSBits) {
>>>>>>> upstream-releases
  return mWrapped->GetQoSBits(aQoSBits);
}

NS_IMETHODIMP
SocketTransportShim::SetQoSBits(uint8_t aQoSBits) {
  return mWrapped->SetQoSBits(aQoSBits);
}

NS_IMETHODIMP
<<<<<<< HEAD
SocketTransportShim::SetFastOpenCallback(TCPFastOpen *aFastOpen) {
||||||| merged common ancestors
SocketTransportShim::SetFastOpenCallback(TCPFastOpen *aFastOpen)
{
=======
SocketTransportShim::SetFastOpenCallback(TCPFastOpen* aFastOpen) {
>>>>>>> upstream-releases
  return mWrapped->SetFastOpenCallback(aFastOpen);
}

NS_IMPL_ISUPPORTS(TLSFilterTransaction, nsITimerCallback, nsINamed)
NS_IMPL_ISUPPORTS(SocketTransportShim, nsISocketTransport, nsITransport)
NS_IMPL_ISUPPORTS(InputStreamShim, nsIInputStream, nsIAsyncInputStream)
NS_IMPL_ISUPPORTS(OutputStreamShim, nsIOutputStream, nsIAsyncOutputStream)
NS_IMPL_ISUPPORTS(SocketInWrapper, nsIAsyncInputStream)
NS_IMPL_ISUPPORTS(SocketOutWrapper, nsIAsyncOutputStream)

}  // namespace net
}  // namespace mozilla
