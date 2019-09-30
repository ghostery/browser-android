/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _WEBRTC_GLOBAL_H_
#define _WEBRTC_GLOBAL_H_

#include "ipc/IPCMessageUtils.h"
#include "mozilla/dom/BindingDeclarations.h"
#include "mozilla/dom/RTCStatsReportBinding.h"
#include "nsAutoPtr.h"

typedef mozilla::dom::RTCStatsReportInternal StatsReport;
typedef nsTArray<nsAutoPtr<StatsReport>> RTCReports;
typedef mozilla::dom::Sequence<nsString> WebrtcGlobalLog;

namespace mozilla {
namespace dom {
// webidl dictionaries don't have move semantics, which is something that ipdl
// needs for async returns. So, we create a "moveable" subclass that just
// copies. _Really_ lame, but it gets the job done.
struct MovableRTCStatsReportInternal : public RTCStatsReportInternal {
  MovableRTCStatsReportInternal() = default;
  explicit MovableRTCStatsReportInternal(RTCStatsReportInternal&& aReport) {
    RTCStatsReportInternal::operator=(aReport);
  }
  explicit MovableRTCStatsReportInternal(
      const RTCStatsReportInternal& aReport) {
    RTCStatsReportInternal::operator=(aReport);
  }
};
}  // namespace dom
}  // namespace mozilla

namespace IPC {

template <typename T>
struct ParamTraits<mozilla::dom::Sequence<T>> {
  typedef mozilla::dom::Sequence<T> paramType;

  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, static_cast<const FallibleTArray<T>&>(aParam));
  }

  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    return ReadParam(aMsg, aIter, static_cast<FallibleTArray<T>*>(aResult));
  }
};

<<<<<<< HEAD
template <>
struct ParamTraits<mozilla::dom::RTCStatsType>
    : public ContiguousEnumSerializer<mozilla::dom::RTCStatsType,
                                      mozilla::dom::RTCStatsType::Inbound_rtp,
                                      mozilla::dom::RTCStatsType::EndGuard_> {};

template <>
struct ParamTraits<mozilla::dom::RTCStatsIceCandidatePairState>
    : public ContiguousEnumSerializer<
          mozilla::dom::RTCStatsIceCandidatePairState,
          mozilla::dom::RTCStatsIceCandidatePairState::Frozen,
          mozilla::dom::RTCStatsIceCandidatePairState::EndGuard_> {};

template <>
struct ParamTraits<mozilla::dom::RTCIceCandidateType>
    : public ContiguousEnumSerializer<
          mozilla::dom::RTCIceCandidateType,
          mozilla::dom::RTCIceCandidateType::Host,
          mozilla::dom::RTCIceCandidateType::EndGuard_> {};

template <>
struct ParamTraits<mozilla::dom::RTCStatsReportInternal> {
||||||| merged common ancestors
template<>
struct ParamTraits<mozilla::dom::RTCStatsType> :
  public ContiguousEnumSerializer<
    mozilla::dom::RTCStatsType,
    mozilla::dom::RTCStatsType::Inbound_rtp,
    mozilla::dom::RTCStatsType::EndGuard_>
{};

template<>
struct ParamTraits<mozilla::dom::RTCStatsIceCandidatePairState> :
  public ContiguousEnumSerializer<
    mozilla::dom::RTCStatsIceCandidatePairState,
    mozilla::dom::RTCStatsIceCandidatePairState::Frozen,
    mozilla::dom::RTCStatsIceCandidatePairState::EndGuard_>
{};

template<>
struct ParamTraits<mozilla::dom::RTCStatsIceCandidateType> :
  public ContiguousEnumSerializer<
    mozilla::dom::RTCStatsIceCandidateType,
    mozilla::dom::RTCStatsIceCandidateType::Host,
    mozilla::dom::RTCStatsIceCandidateType::EndGuard_>
{};

template<>
struct ParamTraits<mozilla::dom::RTCStatsReportInternal>
{
=======
template <>
struct ParamTraits<mozilla::dom::RTCStatsType>
    : public ContiguousEnumSerializer<mozilla::dom::RTCStatsType,
                                      mozilla::dom::RTCStatsType::Inbound_rtp,
                                      mozilla::dom::RTCStatsType::EndGuard_> {};

template <>
struct ParamTraits<mozilla::dom::RTCStatsIceCandidatePairState>
    : public ContiguousEnumSerializer<
          mozilla::dom::RTCStatsIceCandidatePairState,
          mozilla::dom::RTCStatsIceCandidatePairState::Frozen,
          mozilla::dom::RTCStatsIceCandidatePairState::EndGuard_> {};

template <>
struct ParamTraits<mozilla::dom::RTCIceCandidateType>
    : public ContiguousEnumSerializer<
          mozilla::dom::RTCIceCandidateType,
          mozilla::dom::RTCIceCandidateType::Host,
          mozilla::dom::RTCIceCandidateType::EndGuard_> {};

template <>
struct ParamTraits<mozilla::dom::MovableRTCStatsReportInternal> {
  typedef mozilla::dom::MovableRTCStatsReportInternal paramType;
  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(
        aMsg, static_cast<const mozilla::dom::RTCStatsReportInternal&>(aParam));
  }
  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    return ReadParam(
        aMsg, aIter,
        static_cast<mozilla::dom::RTCStatsReportInternal*>(aResult));
  }
};

template <>
struct ParamTraits<mozilla::dom::RTCStatsReportInternal> {
>>>>>>> upstream-releases
  typedef mozilla::dom::RTCStatsReportInternal paramType;

  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mClosed);
    WriteParam(aMsg, aParam.mIceCandidatePairStats);
    WriteParam(aMsg, aParam.mIceCandidateStats);
    WriteParam(aMsg, aParam.mInboundRtpStreamStats);
    WriteParam(aMsg, aParam.mLocalSdp);
    WriteParam(aMsg, aParam.mOutboundRtpStreamStats);
    WriteParam(aMsg, aParam.mPcid);
    WriteParam(aMsg, aParam.mRemoteInboundRtpStreamStats);
    WriteParam(aMsg, aParam.mRemoteOutboundRtpStreamStats);
    WriteParam(aMsg, aParam.mRemoteSdp);
    WriteParam(aMsg, aParam.mTimestamp);
    WriteParam(aMsg, aParam.mIceRestarts);
    WriteParam(aMsg, aParam.mIceRollbacks);
    WriteParam(aMsg, aParam.mRtpContributingSourceStats);
    WriteParam(aMsg, aParam.mOfferer);
    WriteParam(aMsg, aParam.mTrickledIceCandidateStats);
    WriteParam(aMsg, aParam.mRawLocalCandidates);
    WriteParam(aMsg, aParam.mRawRemoteCandidates);
  }

  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    if (!ReadParam(aMsg, aIter, &(aResult->mClosed)) ||
        !ReadParam(aMsg, aIter, &(aResult->mIceCandidatePairStats)) ||
        !ReadParam(aMsg, aIter, &(aResult->mIceCandidateStats)) ||
        !ReadParam(aMsg, aIter, &(aResult->mInboundRtpStreamStats)) ||
        !ReadParam(aMsg, aIter, &(aResult->mLocalSdp)) ||
        !ReadParam(aMsg, aIter, &(aResult->mOutboundRtpStreamStats)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPcid)) ||
        !ReadParam(aMsg, aIter, &(aResult->mRemoteInboundRtpStreamStats)) ||
        !ReadParam(aMsg, aIter, &(aResult->mRemoteOutboundRtpStreamStats)) ||
        !ReadParam(aMsg, aIter, &(aResult->mRemoteSdp)) ||
        !ReadParam(aMsg, aIter, &(aResult->mTimestamp)) ||
        !ReadParam(aMsg, aIter, &(aResult->mIceRestarts)) ||
        !ReadParam(aMsg, aIter, &(aResult->mIceRollbacks)) ||
        !ReadParam(aMsg, aIter, &(aResult->mRtpContributingSourceStats)) ||
        !ReadParam(aMsg, aIter, &(aResult->mOfferer)) ||
        !ReadParam(aMsg, aIter, &(aResult->mTrickledIceCandidateStats)) ||
        !ReadParam(aMsg, aIter, &(aResult->mRawLocalCandidates)) ||
        !ReadParam(aMsg, aIter, &(aResult->mRawRemoteCandidates))) {
      return false;
    }

    return true;
  }
};

typedef mozilla::dom::RTCStats RTCStats;

static void WriteRTCStats(Message* aMsg, const RTCStats& aParam) {
  // RTCStats base class
  WriteParam(aMsg, aParam.mId);
  WriteParam(aMsg, aParam.mTimestamp);
  WriteParam(aMsg, aParam.mType);
}

static bool ReadRTCStats(const Message* aMsg, PickleIterator* aIter,
                         RTCStats* aResult) {
  // RTCStats base class
  if (!ReadParam(aMsg, aIter, &(aResult->mId)) ||
      !ReadParam(aMsg, aIter, &(aResult->mTimestamp)) ||
      !ReadParam(aMsg, aIter, &(aResult->mType))) {
    return false;
  }

  return true;
}

<<<<<<< HEAD
template <>
struct ParamTraits<mozilla::dom::RTCCodecStats> {
  typedef mozilla::dom::RTCCodecStats paramType;

  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mChannels);
    WriteParam(aMsg, aParam.mClockRate);
    WriteParam(aMsg, aParam.mCodec);
    WriteParam(aMsg, aParam.mParameters);
    WriteParam(aMsg, aParam.mPayloadType);
    WriteRTCStats(aMsg, aParam);
  }

  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    if (!ReadParam(aMsg, aIter, &(aResult->mChannels)) ||
        !ReadParam(aMsg, aIter, &(aResult->mClockRate)) ||
        !ReadParam(aMsg, aIter, &(aResult->mCodec)) ||
        !ReadParam(aMsg, aIter, &(aResult->mParameters)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPayloadType)) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
  }
};

template <>
struct ParamTraits<mozilla::dom::RTCIceCandidatePairStats> {
||||||| merged common ancestors
template<>
struct ParamTraits<mozilla::dom::RTCCodecStats>
{
  typedef mozilla::dom::RTCCodecStats paramType;

  static void Write(Message* aMsg, const paramType& aParam)
  {
    WriteParam(aMsg, aParam.mChannels);
    WriteParam(aMsg, aParam.mClockRate);
    WriteParam(aMsg, aParam.mCodec);
    WriteParam(aMsg, aParam.mParameters);
    WriteParam(aMsg, aParam.mPayloadType);
    WriteRTCStats(aMsg, aParam);
  }

  static bool Read(const Message* aMsg, PickleIterator* aIter, paramType* aResult)
  {
    if (!ReadParam(aMsg, aIter, &(aResult->mChannels)) ||
        !ReadParam(aMsg, aIter, &(aResult->mClockRate)) ||
        !ReadParam(aMsg, aIter, &(aResult->mCodec)) ||
        !ReadParam(aMsg, aIter, &(aResult->mParameters)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPayloadType)) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
 }
};

template<>
struct ParamTraits<mozilla::dom::RTCIceCandidatePairStats>
{
=======
template <>
struct ParamTraits<mozilla::dom::RTCIceCandidatePairStats> {
>>>>>>> upstream-releases
  typedef mozilla::dom::RTCIceCandidatePairStats paramType;

  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mTransportId);
    WriteParam(aMsg, aParam.mLocalCandidateId);
    WriteParam(aMsg, aParam.mPriority);
    WriteParam(aMsg, aParam.mNominated);
    WriteParam(aMsg, aParam.mWritable);
    WriteParam(aMsg, aParam.mReadable);
    WriteParam(aMsg, aParam.mRemoteCandidateId);
    WriteParam(aMsg, aParam.mSelected);
    WriteParam(aMsg, aParam.mComponentId);
    WriteParam(aMsg, aParam.mState);
    WriteParam(aMsg, aParam.mBytesSent);
    WriteParam(aMsg, aParam.mBytesReceived);
    WriteParam(aMsg, aParam.mLastPacketSentTimestamp);
    WriteParam(aMsg, aParam.mLastPacketReceivedTimestamp);
    WriteRTCStats(aMsg, aParam);
  }

  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    if (!ReadParam(aMsg, aIter, &(aResult->mTransportId)) ||
        !ReadParam(aMsg, aIter, &(aResult->mLocalCandidateId)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPriority)) ||
        !ReadParam(aMsg, aIter, &(aResult->mNominated)) ||
        !ReadParam(aMsg, aIter, &(aResult->mWritable)) ||
        !ReadParam(aMsg, aIter, &(aResult->mReadable)) ||
        !ReadParam(aMsg, aIter, &(aResult->mRemoteCandidateId)) ||
        !ReadParam(aMsg, aIter, &(aResult->mSelected)) ||
        !ReadParam(aMsg, aIter, &(aResult->mComponentId)) ||
        !ReadParam(aMsg, aIter, &(aResult->mState)) ||
        !ReadParam(aMsg, aIter, &(aResult->mBytesSent)) ||
        !ReadParam(aMsg, aIter, &(aResult->mBytesReceived)) ||
        !ReadParam(aMsg, aIter, &(aResult->mLastPacketSentTimestamp)) ||
        !ReadParam(aMsg, aIter, &(aResult->mLastPacketReceivedTimestamp)) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
  }
};

template <>
struct ParamTraits<mozilla::dom::RTCIceCandidateStats> {
  typedef mozilla::dom::RTCIceCandidateStats paramType;

  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mCandidateType);
<<<<<<< HEAD
    WriteParam(aMsg, aParam.mPriority);
    WriteParam(aMsg, aParam.mTransportId);
    WriteParam(aMsg, aParam.mAddress);
    WriteParam(aMsg, aParam.mRelayProtocol);
    WriteParam(aMsg, aParam.mPort);
    WriteParam(aMsg, aParam.mProtocol);
    WriteRTCStats(aMsg, aParam);
  }

  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    if (!ReadParam(aMsg, aIter, &(aResult->mCandidateType)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPriority)) ||
        !ReadParam(aMsg, aIter, &(aResult->mTransportId)) ||
        !ReadParam(aMsg, aIter, &(aResult->mAddress)) ||
        !ReadParam(aMsg, aIter, &(aResult->mRelayProtocol)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPort)) ||
        !ReadParam(aMsg, aIter, &(aResult->mProtocol)) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
  }
};

template <>
struct ParamTraits<mozilla::dom::RTCIceComponentStats> {
  typedef mozilla::dom::RTCIceComponentStats paramType;

  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mActiveConnection);
    WriteParam(aMsg, aParam.mBytesReceived);
    WriteParam(aMsg, aParam.mBytesSent);
    WriteParam(aMsg, aParam.mComponent);
||||||| merged common ancestors
    WriteParam(aMsg, aParam.mComponentId);
    WriteParam(aMsg, aParam.mIpAddress);
    WriteParam(aMsg, aParam.mMozLocalTransport);
    WriteParam(aMsg, aParam.mRelayProtocol);
    WriteParam(aMsg, aParam.mPortNumber);
    WriteParam(aMsg, aParam.mTransport);
    WriteRTCStats(aMsg, aParam);
  }

  static bool Read(const Message* aMsg, PickleIterator* aIter, paramType* aResult)
  {
    if (!ReadParam(aMsg, aIter, &(aResult->mCandidateId)) ||
        !ReadParam(aMsg, aIter, &(aResult->mCandidateType)) ||
        !ReadParam(aMsg, aIter, &(aResult->mComponentId)) ||
        !ReadParam(aMsg, aIter, &(aResult->mIpAddress)) ||
        !ReadParam(aMsg, aIter, &(aResult->mMozLocalTransport)) ||
        !ReadParam(aMsg, aIter, &(aResult->mRelayProtocol)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPortNumber)) ||
        !ReadParam(aMsg, aIter, &(aResult->mTransport)) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
 }
};

template<>
struct ParamTraits<mozilla::dom::RTCIceComponentStats>
{
  typedef mozilla::dom::RTCIceComponentStats paramType;

  static void Write(Message* aMsg, const paramType& aParam)
  {
    WriteParam(aMsg, aParam.mActiveConnection);
    WriteParam(aMsg, aParam.mBytesReceived);
    WriteParam(aMsg, aParam.mBytesSent);
    WriteParam(aMsg, aParam.mComponent);
=======
    WriteParam(aMsg, aParam.mPriority);
>>>>>>> upstream-releases
    WriteParam(aMsg, aParam.mTransportId);
    WriteParam(aMsg, aParam.mAddress);
    WriteParam(aMsg, aParam.mRelayProtocol);
    WriteParam(aMsg, aParam.mPort);
    WriteParam(aMsg, aParam.mProtocol);
    WriteRTCStats(aMsg, aParam);
  }

<<<<<<< HEAD
  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    if (!ReadParam(aMsg, aIter, &(aResult->mActiveConnection)) ||
        !ReadParam(aMsg, aIter, &(aResult->mBytesReceived)) ||
        !ReadParam(aMsg, aIter, &(aResult->mBytesSent)) ||
        !ReadParam(aMsg, aIter, &(aResult->mComponent)) ||
||||||| merged common ancestors
  static bool Read(const Message* aMsg, PickleIterator* aIter, paramType* aResult)
  {
    if (!ReadParam(aMsg, aIter, &(aResult->mActiveConnection)) ||
        !ReadParam(aMsg, aIter, &(aResult->mBytesReceived)) ||
        !ReadParam(aMsg, aIter, &(aResult->mBytesSent)) ||
        !ReadParam(aMsg, aIter, &(aResult->mComponent)) ||
=======
  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    if (!ReadParam(aMsg, aIter, &(aResult->mCandidateType)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPriority)) ||
>>>>>>> upstream-releases
        !ReadParam(aMsg, aIter, &(aResult->mTransportId)) ||
        !ReadParam(aMsg, aIter, &(aResult->mAddress)) ||
        !ReadParam(aMsg, aIter, &(aResult->mRelayProtocol)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPort)) ||
        !ReadParam(aMsg, aIter, &(aResult->mProtocol)) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
  }
};

static void WriteRTCRtpStreamStats(
<<<<<<< HEAD
    Message* aMsg, const mozilla::dom::RTCRtpStreamStats& aParam) {
  WriteParam(aMsg, aParam.mBitrateMean);
  WriteParam(aMsg, aParam.mBitrateStdDev);
  WriteParam(aMsg, aParam.mCodecId);
  WriteParam(aMsg, aParam.mFramerateMean);
  WriteParam(aMsg, aParam.mFramerateStdDev);
  WriteParam(aMsg, aParam.mIsRemote);
  WriteParam(aMsg, aParam.mMediaTrackId);
  WriteParam(aMsg, aParam.mMediaType);
  WriteParam(aMsg, aParam.mKind);
  WriteParam(aMsg, aParam.mRemoteId);
  WriteParam(aMsg, aParam.mSsrc);
  WriteParam(aMsg, aParam.mTransportId);
||||||| merged common ancestors
              Message* aMsg,
              const mozilla::dom::RTCRtpStreamStats& aParam)
{
    WriteParam(aMsg, aParam.mBitrateMean);
    WriteParam(aMsg, aParam.mBitrateStdDev);
    WriteParam(aMsg, aParam.mCodecId);
    WriteParam(aMsg, aParam.mFramerateMean);
    WriteParam(aMsg, aParam.mFramerateStdDev);
    WriteParam(aMsg, aParam.mIsRemote);
    WriteParam(aMsg, aParam.mMediaTrackId);
    WriteParam(aMsg, aParam.mMediaType);
    WriteParam(aMsg, aParam.mKind);
    WriteParam(aMsg, aParam.mRemoteId);
    WriteParam(aMsg, aParam.mSsrc);
    WriteParam(aMsg, aParam.mTransportId);
=======
    Message* aMsg, const mozilla::dom::RTCRtpStreamStats& aParam) {
  WriteParam(aMsg, aParam.mSsrc);
  WriteParam(aMsg, aParam.mMediaType);
  WriteParam(aMsg, aParam.mKind);
  WriteParam(aMsg, aParam.mTransportId);
  WriteRTCStats(aMsg, aParam);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool ReadRTCRtpStreamStats(const Message* aMsg, PickleIterator* aIter,
                                  mozilla::dom::RTCRtpStreamStats* aResult) {
  if (!ReadParam(aMsg, aIter, &(aResult->mBitrateMean)) ||
      !ReadParam(aMsg, aIter, &(aResult->mBitrateStdDev)) ||
      !ReadParam(aMsg, aIter, &(aResult->mCodecId)) ||
      !ReadParam(aMsg, aIter, &(aResult->mFramerateMean)) ||
      !ReadParam(aMsg, aIter, &(aResult->mFramerateStdDev)) ||
      !ReadParam(aMsg, aIter, &(aResult->mIsRemote)) ||
      !ReadParam(aMsg, aIter, &(aResult->mMediaTrackId)) ||
      !ReadParam(aMsg, aIter, &(aResult->mMediaType)) ||
      !ReadParam(aMsg, aIter, &(aResult->mKind)) ||
      !ReadParam(aMsg, aIter, &(aResult->mRemoteId)) ||
      !ReadParam(aMsg, aIter, &(aResult->mSsrc)) ||
      !ReadParam(aMsg, aIter, &(aResult->mTransportId))) {
    return false;
  }
||||||| merged common ancestors
static bool ReadRTCRtpStreamStats(
              const Message* aMsg, PickleIterator* aIter,
              mozilla::dom::RTCRtpStreamStats* aResult)
{
  if (!ReadParam(aMsg, aIter, &(aResult->mBitrateMean)) ||
      !ReadParam(aMsg, aIter, &(aResult->mBitrateStdDev)) ||
      !ReadParam(aMsg, aIter, &(aResult->mCodecId)) ||
      !ReadParam(aMsg, aIter, &(aResult->mFramerateMean)) ||
      !ReadParam(aMsg, aIter, &(aResult->mFramerateStdDev)) ||
      !ReadParam(aMsg, aIter, &(aResult->mIsRemote)) ||
      !ReadParam(aMsg, aIter, &(aResult->mMediaTrackId)) ||
      !ReadParam(aMsg, aIter, &(aResult->mMediaType)) ||
      !ReadParam(aMsg, aIter, &(aResult->mKind)) ||
      !ReadParam(aMsg, aIter, &(aResult->mRemoteId)) ||
      !ReadParam(aMsg, aIter, &(aResult->mSsrc)) ||
      !ReadParam(aMsg, aIter, &(aResult->mTransportId))) {
    return false;
  }
=======
static bool ReadRTCRtpStreamStats(const Message* aMsg, PickleIterator* aIter,
                                  mozilla::dom::RTCRtpStreamStats* aResult) {
  return ReadParam(aMsg, aIter, &(aResult->mSsrc)) &&
         ReadParam(aMsg, aIter, &(aResult->mMediaType)) &&
         ReadParam(aMsg, aIter, &(aResult->mKind)) &&
         ReadParam(aMsg, aIter, &(aResult->mTransportId)) &&
         ReadRTCStats(aMsg, aIter, aResult);
}
>>>>>>> upstream-releases

static void WriteRTCReceivedRtpStreamStats(
    Message* aMsg, const mozilla::dom::RTCReceivedRtpStreamStats& aParam) {
  WriteParam(aMsg, aParam.mPacketsReceived);
  WriteParam(aMsg, aParam.mPacketsLost);
  WriteParam(aMsg, aParam.mJitter);
  WriteParam(aMsg, aParam.mDiscardedPackets);
  WriteParam(aMsg, aParam.mPacketsDiscarded);
  WriteRTCRtpStreamStats(aMsg, aParam);
}

<<<<<<< HEAD
template <>
struct ParamTraits<mozilla::dom::RTCInboundRTPStreamStats> {
  typedef mozilla::dom::RTCInboundRTPStreamStats paramType;
||||||| merged common ancestors
template<>
struct ParamTraits<mozilla::dom::RTCInboundRTPStreamStats>
{
  typedef mozilla::dom::RTCInboundRTPStreamStats paramType;
=======
static bool ReadRTCReceivedRtpStreamStats(
    const Message* aMsg, PickleIterator* aIter,
    mozilla::dom::RTCReceivedRtpStreamStats* aResult) {
  return ReadParam(aMsg, aIter, &(aResult->mPacketsReceived)) &&
         ReadParam(aMsg, aIter, &(aResult->mPacketsLost)) &&
         ReadParam(aMsg, aIter, &(aResult->mJitter)) &&
         ReadParam(aMsg, aIter, &(aResult->mDiscardedPackets)) &&
         ReadParam(aMsg, aIter, &(aResult->mPacketsDiscarded)) &&
         ReadRTCRtpStreamStats(aMsg, aIter, aResult);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mBytesReceived);
    WriteParam(aMsg, aParam.mDiscardedPackets);
||||||| merged common ancestors
  static void Write(Message* aMsg, const paramType& aParam)
  {
    WriteParam(aMsg, aParam.mBytesReceived);
    WriteParam(aMsg, aParam.mDiscardedPackets);
=======
template <>
struct ParamTraits<mozilla::dom::RTCInboundRtpStreamStats> {
  typedef mozilla::dom::RTCInboundRtpStreamStats paramType;

  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mRemoteId);
>>>>>>> upstream-releases
    WriteParam(aMsg, aParam.mFramesDecoded);
    WriteParam(aMsg, aParam.mBytesReceived);
    WriteParam(aMsg, aParam.mNackCount);
    WriteParam(aMsg, aParam.mFirCount);
    WriteParam(aMsg, aParam.mPliCount);
    WriteParam(aMsg, aParam.mBitrateMean);
    WriteParam(aMsg, aParam.mBitrateStdDev);
    WriteParam(aMsg, aParam.mFramerateMean);
    WriteParam(aMsg, aParam.mFramerateStdDev);
    WriteRTCReceivedRtpStreamStats(aMsg, aParam);
  }

<<<<<<< HEAD
  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    if (!ReadParam(aMsg, aIter, &(aResult->mBytesReceived)) ||
        !ReadParam(aMsg, aIter, &(aResult->mDiscardedPackets)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesDecoded)) ||
        !ReadParam(aMsg, aIter, &(aResult->mJitter)) ||
        !ReadParam(aMsg, aIter, &(aResult->mMozAvSyncDelay)) ||
        !ReadParam(aMsg, aIter, &(aResult->mMozJitterBufferDelay)) ||
        !ReadParam(aMsg, aIter, &(aResult->mRoundTripTime)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPacketsLost)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPacketsReceived)) ||
        !ReadRTCRtpStreamStats(aMsg, aIter, aResult) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
||||||| merged common ancestors
  static bool Read(const Message* aMsg, PickleIterator* aIter, paramType* aResult)
  {
    if (!ReadParam(aMsg, aIter, &(aResult->mBytesReceived)) ||
        !ReadParam(aMsg, aIter, &(aResult->mDiscardedPackets)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesDecoded)) ||
        !ReadParam(aMsg, aIter, &(aResult->mJitter)) ||
        !ReadParam(aMsg, aIter, &(aResult->mMozAvSyncDelay)) ||
        !ReadParam(aMsg, aIter, &(aResult->mMozJitterBufferDelay)) ||
        !ReadParam(aMsg, aIter, &(aResult->mRoundTripTime)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPacketsLost)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPacketsReceived)) ||
        !ReadRTCRtpStreamStats(aMsg, aIter, aResult) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
=======
  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    return ReadParam(aMsg, aIter, &(aResult->mRemoteId)) &&
           ReadParam(aMsg, aIter, &(aResult->mFramesDecoded)) &&
           ReadParam(aMsg, aIter, &(aResult->mBytesReceived)) &&
           ReadParam(aMsg, aIter, &(aResult->mNackCount)) &&
           ReadParam(aMsg, aIter, &(aResult->mFirCount)) &&
           ReadParam(aMsg, aIter, &(aResult->mPliCount)) &&
           ReadParam(aMsg, aIter, &(aResult->mBitrateMean)) &&
           ReadParam(aMsg, aIter, &(aResult->mBitrateStdDev)) &&
           ReadParam(aMsg, aIter, &(aResult->mFramerateMean)) &&
           ReadParam(aMsg, aIter, &(aResult->mFramerateStdDev)) &&
           ReadRTCReceivedRtpStreamStats(aMsg, aIter, aResult);
>>>>>>> upstream-releases
  }
};

<<<<<<< HEAD
template <>
struct ParamTraits<mozilla::dom::RTCOutboundRTPStreamStats> {
  typedef mozilla::dom::RTCOutboundRTPStreamStats paramType;
||||||| merged common ancestors
template<>
struct ParamTraits<mozilla::dom::RTCOutboundRTPStreamStats>
{
  typedef mozilla::dom::RTCOutboundRTPStreamStats paramType;
=======
static void WriteRTCSentRtpStreamStats(
    Message* aMsg, const mozilla::dom::RTCSentRtpStreamStats& aParam) {
  WriteParam(aMsg, aParam.mPacketsSent);
  WriteParam(aMsg, aParam.mBytesSent);
  WriteRTCRtpStreamStats(aMsg, aParam);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mBytesSent);
    WriteParam(aMsg, aParam.mDroppedFrames);
    WriteParam(aMsg, aParam.mPacketsSent);
    WriteParam(aMsg, aParam.mTargetBitrate);
||||||| merged common ancestors
  static void Write(Message* aMsg, const paramType& aParam)
  {
    WriteParam(aMsg, aParam.mBytesSent);
    WriteParam(aMsg, aParam.mDroppedFrames);
    WriteParam(aMsg, aParam.mPacketsSent);
    WriteParam(aMsg, aParam.mTargetBitrate);
=======
static bool ReadRTCSentRtpStreamStats(
    const Message* aMsg, PickleIterator* aIter,
    mozilla::dom::RTCSentRtpStreamStats* aResult) {
  return ReadParam(aMsg, aIter, &(aResult->mPacketsSent)) &&
         ReadParam(aMsg, aIter, &(aResult->mBytesSent)) &&
         ReadRTCRtpStreamStats(aMsg, aIter, aResult);
}

template <>
struct ParamTraits<mozilla::dom::RTCOutboundRtpStreamStats> {
  typedef mozilla::dom::RTCOutboundRtpStreamStats paramType;

  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mRemoteId);
>>>>>>> upstream-releases
    WriteParam(aMsg, aParam.mFramesEncoded);
    WriteParam(aMsg, aParam.mQpSum);
    WriteParam(aMsg, aParam.mNackCount);
    WriteParam(aMsg, aParam.mFirCount);
    WriteParam(aMsg, aParam.mPliCount);
    WriteParam(aMsg, aParam.mBitrateMean);
    WriteParam(aMsg, aParam.mBitrateStdDev);
    WriteParam(aMsg, aParam.mFramerateMean);
    WriteParam(aMsg, aParam.mFramerateStdDev);
    WriteParam(aMsg, aParam.mDroppedFrames);
    WriteRTCSentRtpStreamStats(aMsg, aParam);
  }

<<<<<<< HEAD
  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    if (!ReadParam(aMsg, aIter, &(aResult->mBytesSent)) ||
        !ReadParam(aMsg, aIter, &(aResult->mDroppedFrames)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPacketsSent)) ||
        !ReadParam(aMsg, aIter, &(aResult->mTargetBitrate)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesEncoded)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFirCount)) ||
        !ReadParam(aMsg, aIter, &(aResult->mNackCount)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPliCount)) ||
        !ReadRTCRtpStreamStats(aMsg, aIter, aResult) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
||||||| merged common ancestors
  static bool Read(const Message* aMsg, PickleIterator* aIter, paramType* aResult)
  {
    if (!ReadParam(aMsg, aIter, &(aResult->mBytesSent)) ||
        !ReadParam(aMsg, aIter, &(aResult->mDroppedFrames)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPacketsSent)) ||
        !ReadParam(aMsg, aIter, &(aResult->mTargetBitrate)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesEncoded)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFirCount)) ||
        !ReadParam(aMsg, aIter, &(aResult->mNackCount)) ||
        !ReadParam(aMsg, aIter, &(aResult->mPliCount)) ||
        !ReadRTCRtpStreamStats(aMsg, aIter, aResult) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
=======
  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    return ReadParam(aMsg, aIter, &(aResult->mRemoteId)) &&
           ReadParam(aMsg, aIter, &(aResult->mFramesEncoded)) &&
           ReadParam(aMsg, aIter, &(aResult->mQpSum)) &&
           ReadParam(aMsg, aIter, &(aResult->mNackCount)) &&
           ReadParam(aMsg, aIter, &(aResult->mFirCount)) &&
           ReadParam(aMsg, aIter, &(aResult->mPliCount)) &&
           ReadParam(aMsg, aIter, &(aResult->mBitrateMean)) &&
           ReadParam(aMsg, aIter, &(aResult->mBitrateStdDev)) &&
           ReadParam(aMsg, aIter, &(aResult->mFramerateMean)) &&
           ReadParam(aMsg, aIter, &(aResult->mFramerateStdDev)) &&
           ReadParam(aMsg, aIter, &(aResult->mDroppedFrames)) &&
           ReadRTCSentRtpStreamStats(aMsg, aIter, aResult);
>>>>>>> upstream-releases
  }
};

<<<<<<< HEAD
template <>
struct ParamTraits<mozilla::dom::RTCMediaStreamStats> {
  typedef mozilla::dom::RTCMediaStreamStats paramType;
||||||| merged common ancestors
template<>
struct ParamTraits<mozilla::dom::RTCMediaStreamStats>
{
  typedef mozilla::dom::RTCMediaStreamStats paramType;
=======
template <>
struct ParamTraits<mozilla::dom::RTCRemoteInboundRtpStreamStats> {
  typedef mozilla::dom::RTCRemoteInboundRtpStreamStats paramType;
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mStreamIdentifier);
    WriteParam(aMsg, aParam.mTrackIds);
    WriteRTCStats(aMsg, aParam);
||||||| merged common ancestors
  static void Write(Message* aMsg, const paramType& aParam)
  {
    WriteParam(aMsg, aParam.mStreamIdentifier);
    WriteParam(aMsg, aParam.mTrackIds);
    WriteRTCStats(aMsg, aParam);
=======
  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mLocalId);
    WriteParam(aMsg, aParam.mBytesReceived);  // To be removed in Bug 1529405
    WriteParam(aMsg, aParam.mRoundTripTime);
    WriteRTCReceivedRtpStreamStats(aMsg, aParam);
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    if (!ReadParam(aMsg, aIter, &(aResult->mStreamIdentifier)) ||
        !ReadParam(aMsg, aIter, &(aResult->mTrackIds)) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
||||||| merged common ancestors
  static bool Read(const Message* aMsg, PickleIterator* aIter, paramType* aResult)
  {
    if (!ReadParam(aMsg, aIter, &(aResult->mStreamIdentifier)) ||
        !ReadParam(aMsg, aIter, &(aResult->mTrackIds)) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
=======
  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    return ReadParam(aMsg, aIter, &(aResult->mLocalId)) &&
           ReadParam(
               aMsg, aIter,
               &(aResult->mBytesReceived)) &&  // To be removed in Bug 1529405
           ReadParam(aMsg, aIter, &(aResult->mRoundTripTime)) &&
           ReadRTCReceivedRtpStreamStats(aMsg, aIter, aResult);
>>>>>>> upstream-releases
  }
};

<<<<<<< HEAD
template <>
struct ParamTraits<mozilla::dom::RTCTransportStats> {
  typedef mozilla::dom::RTCTransportStats paramType;
||||||| merged common ancestors
template<>
struct ParamTraits<mozilla::dom::RTCTransportStats>
{
  typedef mozilla::dom::RTCTransportStats paramType;
=======
template <>
struct ParamTraits<mozilla::dom::RTCRemoteOutboundRtpStreamStats> {
  typedef mozilla::dom::RTCRemoteOutboundRtpStreamStats paramType;
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mBytesReceived);
    WriteParam(aMsg, aParam.mBytesSent);
    WriteRTCStats(aMsg, aParam);
||||||| merged common ancestors
  static void Write(Message* aMsg, const paramType& aParam)
  {
    WriteParam(aMsg, aParam.mBytesReceived);
    WriteParam(aMsg, aParam.mBytesSent);
    WriteRTCStats(aMsg, aParam);
=======
  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mLocalId);
    WriteRTCSentRtpStreamStats(aMsg, aParam);
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    if (!ReadParam(aMsg, aIter, &(aResult->mBytesReceived)) ||
        !ReadParam(aMsg, aIter, &(aResult->mBytesSent)) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
  }
};

template <>
struct ParamTraits<mozilla::dom::RTCMediaStreamTrackStats> {
  typedef mozilla::dom::RTCMediaStreamTrackStats paramType;

  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mAudioLevel);
    WriteParam(aMsg, aParam.mEchoReturnLoss);
    WriteParam(aMsg, aParam.mEchoReturnLossEnhancement);
    WriteParam(aMsg, aParam.mFrameHeight);
    WriteParam(aMsg, aParam.mFrameWidth);
    WriteParam(aMsg, aParam.mFramesCorrupted);
    WriteParam(aMsg, aParam.mFramesDecoded);
    WriteParam(aMsg, aParam.mFramesDropped);
    WriteParam(aMsg, aParam.mFramesPerSecond);
    WriteParam(aMsg, aParam.mFramesReceived);
    WriteParam(aMsg, aParam.mFramesSent);
    WriteParam(aMsg, aParam.mRemoteSource);
    WriteParam(aMsg, aParam.mSsrcIds);
    WriteParam(aMsg, aParam.mTrackIdentifier);
    WriteRTCStats(aMsg, aParam);
  }

  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    if (!ReadParam(aMsg, aIter, &(aResult->mAudioLevel)) ||
        !ReadParam(aMsg, aIter, &(aResult->mEchoReturnLoss)) ||
        !ReadParam(aMsg, aIter, &(aResult->mEchoReturnLossEnhancement)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFrameHeight)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFrameWidth)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesCorrupted)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesDecoded)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesDropped)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesPerSecond)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesReceived)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesSent)) ||
        !ReadParam(aMsg, aIter, &(aResult->mRemoteSource)) ||
        !ReadParam(aMsg, aIter, &(aResult->mSsrcIds)) ||
        !ReadParam(aMsg, aIter, &(aResult->mTrackIdentifier)) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
||||||| merged common ancestors
  static bool Read(const Message* aMsg, PickleIterator* aIter, paramType* aResult)
  {
    if (!ReadParam(aMsg, aIter, &(aResult->mBytesReceived)) ||
        !ReadParam(aMsg, aIter, &(aResult->mBytesSent)) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
  }
};

template<>
struct ParamTraits<mozilla::dom::RTCMediaStreamTrackStats>
{
  typedef mozilla::dom::RTCMediaStreamTrackStats paramType;

  static void Write(Message* aMsg, const paramType& aParam)
  {
    WriteParam(aMsg, aParam.mAudioLevel);
    WriteParam(aMsg, aParam.mEchoReturnLoss);
    WriteParam(aMsg, aParam.mEchoReturnLossEnhancement);
    WriteParam(aMsg, aParam.mFrameHeight);
    WriteParam(aMsg, aParam.mFrameWidth);
    WriteParam(aMsg, aParam.mFramesCorrupted);
    WriteParam(aMsg, aParam.mFramesDecoded);
    WriteParam(aMsg, aParam.mFramesDropped);
    WriteParam(aMsg, aParam.mFramesPerSecond);
    WriteParam(aMsg, aParam.mFramesReceived);
    WriteParam(aMsg, aParam.mFramesSent);
    WriteParam(aMsg, aParam.mRemoteSource);
    WriteParam(aMsg, aParam.mSsrcIds);
    WriteParam(aMsg, aParam.mTrackIdentifier);
    WriteRTCStats(aMsg, aParam);
  }

  static bool Read(const Message* aMsg, PickleIterator* aIter, paramType* aResult)
  {
    if (!ReadParam(aMsg, aIter, &(aResult->mAudioLevel)) ||
        !ReadParam(aMsg, aIter, &(aResult->mEchoReturnLoss)) ||
        !ReadParam(aMsg, aIter, &(aResult->mEchoReturnLossEnhancement)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFrameHeight)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFrameWidth)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesCorrupted)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesDecoded)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesDropped)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesPerSecond)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesReceived)) ||
        !ReadParam(aMsg, aIter, &(aResult->mFramesSent)) ||
        !ReadParam(aMsg, aIter, &(aResult->mRemoteSource)) ||
        !ReadParam(aMsg, aIter, &(aResult->mSsrcIds)) ||
        !ReadParam(aMsg, aIter, &(aResult->mTrackIdentifier)) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }

    return true;
=======
  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    return ReadParam(aMsg, aIter, &(aResult->mLocalId)) &&
           ReadRTCSentRtpStreamStats(aMsg, aIter, aResult);
>>>>>>> upstream-releases
  }
};

template <>
struct ParamTraits<mozilla::dom::RTCRTPContributingSourceStats> {
  typedef mozilla::dom::RTCRTPContributingSourceStats paramType;

  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mContributorSsrc);
    WriteParam(aMsg, aParam.mInboundRtpStreamId);
    WriteRTCStats(aMsg, aParam);
  }

  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    if (!ReadParam(aMsg, aIter, &(aResult->mContributorSsrc)) ||
        !ReadParam(aMsg, aIter, &(aResult->mInboundRtpStreamId)) ||
        !ReadRTCStats(aMsg, aIter, aResult)) {
      return false;
    }
    return true;
  }
};

}  // namespace IPC

#endif  // _WEBRTC_GLOBAL_H_
