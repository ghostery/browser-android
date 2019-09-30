/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

<<<<<<< HEAD
#include "mozilla/AbstractThread.h"
#include "mozilla/CheckedInt.h"
#include "mozilla/gfx/Point.h"
#include "mozilla/SyncRunnable.h"
#include "nsProxyRelease.h"

#include "AudioSegment.h"
||||||| merged common ancestors
#include "mozilla/AbstractThread.h"
#include "mozilla/CheckedInt.h"
#include "mozilla/gfx/Point.h"
#include "mozilla/SyncRunnable.h"

#include "AudioSegment.h"
=======
>>>>>>> upstream-releases
#include "DecodedStream.h"
#include "AudioSegment.h"
#include "MediaData.h"
#include "MediaQueue.h"
#include "MediaStreamGraph.h"
#include "MediaStreamListener.h"
#include "OutputStreamManager.h"
#include "SharedBuffer.h"
#include "VideoSegment.h"
#include "VideoUtils.h"
#include "mozilla/AbstractThread.h"
#include "mozilla/CheckedInt.h"
#include "mozilla/SyncRunnable.h"
#include "mozilla/gfx/Point.h"
#include "nsProxyRelease.h"

namespace mozilla {

using media::NullableTimeUnit;
using media::TimeUnit;

/*
 * A container class to make it easier to pass the playback info all the
 * way to DecodedStreamGraphListener from DecodedStream.
 */
struct PlaybackInfoInit {
  TimeUnit mStartTime;
  MediaInfo mInfo;
  TrackID mAudioTrackID;
  TrackID mVideoTrackID;
};

<<<<<<< HEAD
class DecodedStreamGraphListener;

class DecodedStreamTrackListener : public MediaStreamTrackListener {
 public:
  DecodedStreamTrackListener(DecodedStreamGraphListener* aGraphListener,
                             SourceMediaStream* aStream, TrackID aTrackID);

  void NotifyOutput(MediaStreamGraph* aGraph,
                    StreamTime aCurrentTrackTime) override;
  void NotifyEnded() override;

 private:
  const RefPtr<DecodedStreamGraphListener> mGraphListener;
  const RefPtr<SourceMediaStream> mStream;
  const mozilla::TrackID mTrackID;
};

class DecodedStreamGraphListener {
  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(DecodedStreamGraphListener)
 public:
  DecodedStreamGraphListener(SourceMediaStream* aStream, TrackID aAudioTrackID,
                             MozPromiseHolder<GenericPromise>&& aAudioEndHolder,
                             TrackID aVideoTrackID,
                             MozPromiseHolder<GenericPromise>&& aVideoEndHolder,
                             AbstractThread* aMainThread)
      : mAudioTrackListener(IsTrackIDExplicit(aAudioTrackID)
                                ? MakeRefPtr<DecodedStreamTrackListener>(
                                      this, aStream, aAudioTrackID)
                                : nullptr),
        mAudioTrackID(aAudioTrackID),
        mAudioEndHolder(std::move(aAudioEndHolder)),
        mVideoTrackListener(IsTrackIDExplicit(aVideoTrackID)
                                ? MakeRefPtr<DecodedStreamTrackListener>(
                                      this, aStream, aVideoTrackID)
                                : nullptr),
        mVideoTrackID(aVideoTrackID),
        mVideoEndHolder(std::move(aVideoEndHolder)),
        mStream(aStream),
        mAbstractMainThread(aMainThread) {
    MOZ_ASSERT(NS_IsMainThread());
    if (mAudioTrackListener) {
      mStream->AddTrackListener(mAudioTrackListener, mAudioTrackID);
    } else {
      mAudioEndHolder.ResolveIfExists(true, __func__);
    }

    if (mVideoTrackListener) {
      mStream->AddTrackListener(mVideoTrackListener, mVideoTrackID);
    } else {
      mVideoEndHolder.ResolveIfExists(true, __func__);
||||||| merged common ancestors
class DecodedStreamGraphListener : public MediaStreamListener {
public:
  DecodedStreamGraphListener(MediaStream* aStream,
                             MozPromiseHolder<GenericPromise>&& aPromise,
                             AbstractThread* aMainThread)
    : mMutex("DecodedStreamGraphListener::mMutex")
    , mStream(aStream)
    , mAbstractMainThread(aMainThread)
  {
    mFinishPromise = std::move(aPromise);
  }

  void NotifyOutput(MediaStreamGraph* aGraph, GraphTime aCurrentTime) override
  {
    MutexAutoLock lock(mMutex);
    if (mStream) {
      int64_t t = mStream->StreamTimeToMicroseconds(
        mStream->GraphTimeToStreamTime(aCurrentTime));
      mOnOutput.Notify(t);
=======
class DecodedStreamGraphListener;

class DecodedStreamTrackListener : public MediaStreamTrackListener {
 public:
  DecodedStreamTrackListener(DecodedStreamGraphListener* aGraphListener,
                             SourceMediaStream* aStream, TrackID aTrackID);

  void NotifyOutput(MediaStreamGraph* aGraph,
                    StreamTime aCurrentTrackTime) override;
  void NotifyEnded() override;

 private:
  const RefPtr<DecodedStreamGraphListener> mGraphListener;
  const RefPtr<SourceMediaStream> mStream;
  const TrackID mTrackID;
};

class DecodedStreamGraphListener {
  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(DecodedStreamGraphListener)
 public:
  DecodedStreamGraphListener(
      SourceMediaStream* aStream, TrackID aAudioTrackID,
      MozPromiseHolder<DecodedStream::EndedPromise>&& aAudioEndedHolder,
      TrackID aVideoTrackID,
      MozPromiseHolder<DecodedStream::EndedPromise>&& aVideoEndedHolder,
      AbstractThread* aMainThread)
      : mAudioTrackListener(IsTrackIDExplicit(aAudioTrackID)
                                ? MakeRefPtr<DecodedStreamTrackListener>(
                                      this, aStream, aAudioTrackID)
                                : nullptr),
        mAudioEndedHolder(std::move(aAudioEndedHolder)),
        mVideoTrackListener(IsTrackIDExplicit(aVideoTrackID)
                                ? MakeRefPtr<DecodedStreamTrackListener>(
                                      this, aStream, aVideoTrackID)
                                : nullptr),
        mVideoEndedHolder(std::move(aVideoEndedHolder)),
        mStream(aStream),
        mAudioTrackID(aAudioTrackID),
        mVideoTrackID(aVideoTrackID),
        mAbstractMainThread(aMainThread) {
    MOZ_ASSERT(NS_IsMainThread());
    if (mAudioTrackListener) {
      mStream->AddTrackListener(mAudioTrackListener, mAudioTrackID);
    } else {
      mAudioEnded = true;
      mAudioEndedHolder.ResolveIfExists(true, __func__);
    }

    if (mVideoTrackListener) {
      mStream->AddTrackListener(mVideoTrackListener, mVideoTrackID);
    } else {
      mVideoEnded = true;
      mVideoEndedHolder.ResolveIfExists(true, __func__);
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  void NotifyOutput(TrackID aTrackID, StreamTime aCurrentTrackTime) {
    if (aTrackID != mAudioTrackID && mAudioTrackID != TRACK_NONE) {
      // Only audio playout drives the clock forward, if present.
      return;
||||||| merged common ancestors
  void NotifyEvent(MediaStreamGraph* aGraph, MediaStreamGraphEvent event) override
  {
    if (event == MediaStreamGraphEvent::EVENT_FINISHED) {
      aGraph->DispatchToMainThreadAfterStreamStateUpdate(
        NewRunnableMethod("DecodedStreamGraphListener::DoNotifyFinished",
                          this,
                          &DecodedStreamGraphListener::DoNotifyFinished));
=======
  void NotifyOutput(TrackID aTrackID, StreamTime aCurrentTrackTime) {
    if (aTrackID == mAudioTrackID) {
      if (aCurrentTrackTime >= mAudioEnd) {
        mStream->EndTrack(mAudioTrackID);
      }
    } else if (aTrackID == mVideoTrackID) {
      if (aCurrentTrackTime >= mVideoEnd) {
        mStream->EndTrack(mVideoTrackID);
      }
    } else {
      MOZ_CRASH("Unexpected TrackID");
    }
    if (aTrackID != mAudioTrackID && mAudioTrackID != TRACK_NONE &&
        !mAudioEnded) {
      // Only audio playout drives the clock forward, if present and live.
      return;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    mOnOutput.Notify(mStream->StreamTimeToMicroseconds(aCurrentTrackTime));
||||||| merged common ancestors
=======
    MOZ_ASSERT_IF(aTrackID == mAudioTrackID, !mAudioEnded);
    MOZ_ASSERT_IF(aTrackID == mVideoTrackID, !mVideoEnded);
    mOnOutput.Notify(mStream->StreamTimeToMicroseconds(aCurrentTrackTime));
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  TrackID AudioTrackID() const { return mAudioTrackID; }

  TrackID VideoTrackID() const { return mVideoTrackID; }

  void DoNotifyTrackEnded(TrackID aTrackID) {
||||||| merged common ancestors
  void DoNotifyFinished()
  {
=======
  void NotifyEnded(TrackID aTrackID) {
    if (aTrackID == mAudioTrackID) {
      mAudioEnded = true;
    } else if (aTrackID == mVideoTrackID) {
      mVideoEnded = true;
    } else {
      MOZ_CRASH("Unexpected TrackID");
    }
    mStream->Graph()->DispatchToMainThreadStableState(
        NewRunnableMethod<TrackID>(
            "DecodedStreamGraphListener::DoNotifyTrackEnded", this,
            &DecodedStreamGraphListener::DoNotifyTrackEnded, aTrackID));
  }

  TrackID AudioTrackID() const { return mAudioTrackID; }

  TrackID VideoTrackID() const { return mVideoTrackID; }

  /**
   * Tell the graph listener to end the given track after it has seen at least
   * aEnd worth of output reported as processed by the graph.
   *
   * A StreamTime of STREAM_TIME_MAX indicates that the track has no end and is
   * the default.
   *
   * This method of ending tracks is needed because the MediaStreamGraph
   * processes ended tracks (through SourceMediaStream::EndTrack) at the
   * beginning of an iteration, but waits until the end of the iteration to
   * process any ControlMessages. When such a ControlMessage is a listener that
   * is to be added to a track that has ended in its very first iteration, the
   * track ends before the listener tracking this ending is added. This can lead
   * to a MediaStreamTrack ending on main thread (it uses another listener)
   * before the listeners to render the track get added, potentially meaning a
   * media element doesn't progress before reaching the end although data was
   * available.
   *
   * Callable from any thread.
   */
  void EndTrackAt(TrackID aTrackID, StreamTime aEnd) {
    if (aTrackID == mAudioTrackID) {
      mAudioEnd = aEnd;
    } else if (aTrackID == mVideoTrackID) {
      mVideoEnd = aEnd;
    } else {
      MOZ_CRASH("Unexpected TrackID");
    }
  }

  void DoNotifyTrackEnded(TrackID aTrackID) {
>>>>>>> upstream-releases
    MOZ_ASSERT(NS_IsMainThread());
<<<<<<< HEAD
    if (aTrackID == mAudioTrackID) {
      mAudioEndHolder.ResolveIfExists(true, __func__);
    } else if (aTrackID == mVideoTrackID) {
      mVideoEndHolder.ResolveIfExists(true, __func__);
    } else {
      MOZ_CRASH("Unexpected track id");
    }
||||||| merged common ancestors
    mFinishPromise.ResolveIfExists(true, __func__);
=======
    if (aTrackID == mAudioTrackID) {
      mAudioEndedHolder.ResolveIfExists(true, __func__);
    } else if (aTrackID == mVideoTrackID) {
      mVideoEndedHolder.ResolveIfExists(true, __func__);
    } else {
      MOZ_CRASH("Unexpected track id");
    }
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  void Forget() {
    MOZ_ASSERT(NS_IsMainThread());

    if (mAudioTrackListener && !mStream->IsDestroyed()) {
      mStream->EndTrack(mAudioTrackID);
      mStream->RemoveTrackListener(mAudioTrackListener, mAudioTrackID);
    }
    mAudioTrackListener = nullptr;
    mAudioEndHolder.ResolveIfExists(false, __func__);

    if (mVideoTrackListener && !mStream->IsDestroyed()) {
      mStream->EndTrack(mVideoTrackID);
      mStream->RemoveTrackListener(mVideoTrackListener, mVideoTrackID);
    }
    mVideoTrackListener = nullptr;
    mVideoEndHolder.ResolveIfExists(false, __func__);
||||||| merged common ancestors
  void Forget()
  {
    RefPtr<DecodedStreamGraphListener> self = this;
    mAbstractMainThread->Dispatch(
      NS_NewRunnableFunction("DecodedStreamGraphListener::Forget", [self]() {
        MOZ_ASSERT(NS_IsMainThread());
        self->mFinishPromise.ResolveIfExists(true, __func__);
      }));
    MutexAutoLock lock(mMutex);
    mStream = nullptr;
=======
  void Forget() {
    MOZ_ASSERT(NS_IsMainThread());

    if (mAudioTrackListener && !mStream->IsDestroyed()) {
      mStream->EndTrack(mAudioTrackID);
      mStream->RemoveTrackListener(mAudioTrackListener, mAudioTrackID);
    }
    mAudioTrackListener = nullptr;
    mAudioEndedHolder.ResolveIfExists(false, __func__);

    if (mVideoTrackListener && !mStream->IsDestroyed()) {
      mStream->EndTrack(mVideoTrackID);
      mStream->RemoveTrackListener(mVideoTrackListener, mVideoTrackID);
    }
    mVideoTrackListener = nullptr;
    mVideoEndedHolder.ResolveIfExists(false, __func__);
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  MediaEventSource<int64_t>& OnOutput() { return mOnOutput; }

 private:
  ~DecodedStreamGraphListener() {
    MOZ_ASSERT(mAudioEndHolder.IsEmpty());
    MOZ_ASSERT(mVideoEndHolder.IsEmpty());
||||||| merged common ancestors
  MediaEventSource<int64_t>& OnOutput()
  {
    return mOnOutput;
=======
  MediaEventSource<int64_t>& OnOutput() { return mOnOutput; }

 private:
  ~DecodedStreamGraphListener() {
    MOZ_ASSERT(mAudioEndedHolder.IsEmpty());
    MOZ_ASSERT(mVideoEndedHolder.IsEmpty());
>>>>>>> upstream-releases
  }

  MediaEventProducer<int64_t> mOnOutput;

  // Main thread only.
<<<<<<< HEAD
  RefPtr<DecodedStreamTrackListener> mAudioTrackListener;
  const TrackID mAudioTrackID;
  MozPromiseHolder<GenericPromise> mAudioEndHolder;
  RefPtr<DecodedStreamTrackListener> mVideoTrackListener;
  const TrackID mVideoTrackID;
  MozPromiseHolder<GenericPromise> mVideoEndHolder;
||||||| merged common ancestors
  MozPromiseHolder<GenericPromise> mFinishPromise;
=======
  RefPtr<DecodedStreamTrackListener> mAudioTrackListener;
  MozPromiseHolder<DecodedStream::EndedPromise> mAudioEndedHolder;
  RefPtr<DecodedStreamTrackListener> mVideoTrackListener;
  MozPromiseHolder<DecodedStream::EndedPromise> mVideoEndedHolder;

  // Graph thread only.
  bool mAudioEnded = false;
  bool mVideoEnded = false;
>>>>>>> upstream-releases

<<<<<<< HEAD
  const RefPtr<SourceMediaStream> mStream;
||||||| merged common ancestors
=======
  // Any thread.
  const RefPtr<SourceMediaStream> mStream;
  const TrackID mAudioTrackID;
  Atomic<StreamTime> mAudioEnd{STREAM_TIME_MAX};
  const TrackID mVideoTrackID;
  Atomic<StreamTime> mVideoEnd{STREAM_TIME_MAX};
>>>>>>> upstream-releases
  const RefPtr<AbstractThread> mAbstractMainThread;
};

<<<<<<< HEAD
DecodedStreamTrackListener::DecodedStreamTrackListener(
    DecodedStreamGraphListener* aGraphListener, SourceMediaStream* aStream,
    mozilla::TrackID aTrackID)
    : mGraphListener(aGraphListener), mStream(aStream), mTrackID(aTrackID) {}

void DecodedStreamTrackListener::NotifyOutput(MediaStreamGraph* aGraph,
                                              StreamTime aCurrentTrackTime) {
  mGraphListener->NotifyOutput(mTrackID, aCurrentTrackTime);
}

void DecodedStreamTrackListener::NotifyEnded() {
  mStream->Graph()->DispatchToMainThreadAfterStreamStateUpdate(
      NewRunnableMethod<mozilla::TrackID>(
          "DecodedStreamGraphListener::DoNotifyTrackEnded", mGraphListener,
          &DecodedStreamGraphListener::DoNotifyTrackEnded, mTrackID));
||||||| merged common ancestors
static void
UpdateStreamSuspended(AbstractThread* aMainThread, MediaStream* aStream, bool aBlocking)
{
  if (NS_IsMainThread()) {
    if (aBlocking) {
      aStream->Suspend();
    } else {
      aStream->Resume();
    }
  } else {
    nsCOMPtr<nsIRunnable> r;
    if (aBlocking) {
      r = NewRunnableMethod(
        "MediaStream::Suspend", aStream, &MediaStream::Suspend);
    } else {
      r =
        NewRunnableMethod("MediaStream::Resume", aStream, &MediaStream::Resume);
    }
    aMainThread->Dispatch(r.forget());
  }
=======
DecodedStreamTrackListener::DecodedStreamTrackListener(
    DecodedStreamGraphListener* aGraphListener, SourceMediaStream* aStream,
    TrackID aTrackID)
    : mGraphListener(aGraphListener), mStream(aStream), mTrackID(aTrackID) {}

void DecodedStreamTrackListener::NotifyOutput(MediaStreamGraph* aGraph,
                                              StreamTime aCurrentTrackTime) {
  mGraphListener->NotifyOutput(mTrackID, aCurrentTrackTime);
}

void DecodedStreamTrackListener::NotifyEnded() {
  mGraphListener->NotifyEnded(mTrackID);
>>>>>>> upstream-releases
}

/*
 * All MediaStream-related data is protected by the decoder's monitor.
 * We have at most one DecodedStreamDaata per MediaDecoder. Its stream
 * is used as the input for each ProcessedMediaStream created by calls to
 * captureStream(UntilEnded). Seeking creates a new source stream, as does
 * replaying after the input as ended. In the latter case, the new source is
 * not connected to streams created by captureStreamUntilEnded.
 */
<<<<<<< HEAD
class DecodedStreamData {
 public:
  DecodedStreamData(OutputStreamManager* aOutputStreamManager,
                    PlaybackInfoInit&& aInit,
                    MozPromiseHolder<GenericPromise>&& aAudioPromise,
                    MozPromiseHolder<GenericPromise>&& aVideoPromise,
                    AbstractThread* aMainThread);
||||||| merged common ancestors
class DecodedStreamData {
public:
  DecodedStreamData(OutputStreamManager* aOutputStreamManager,
                    PlaybackInfoInit&& aInit,
                    MozPromiseHolder<GenericPromise>&& aPromise,
                    AbstractThread* aMainThread);
=======
class DecodedStreamData final {
 public:
  DecodedStreamData(
      OutputStreamManager* aOutputStreamManager, PlaybackInfoInit&& aInit,
      MozPromiseHolder<DecodedStream::EndedPromise>&& aAudioEndedPromise,
      MozPromiseHolder<DecodedStream::EndedPromise>&& aVideoEndedPromise,
      AbstractThread* aMainThread);
>>>>>>> upstream-releases
  ~DecodedStreamData();
  MediaEventSource<int64_t>& OnOutput();
  void Forget();
  void GetDebugInfo(dom::DecodedStreamDataDebugInfo& aInfo);

  void WriteVideoToSegment(layers::Image* aImage, const TimeUnit& aStart,
                           const TimeUnit& aEnd,
                           const gfx::IntSize& aIntrinsicSize,
                           const TimeStamp& aTimeStamp, VideoSegment* aOutput,
                           const PrincipalHandle& aPrincipalHandle);

  /* The following group of fields are protected by the decoder's monitor
   * and can be read or written on any thread.
   */
  // Count of audio frames written to the stream
  int64_t mAudioFramesWritten;
<<<<<<< HEAD
  // Count of video frames written to the stream in the stream's rate
  StreamTime mStreamVideoWritten;
  // Count of audio frames written to the stream in the stream's rate
  StreamTime mStreamAudioWritten;
  // mNextVideoTime is the end timestamp for the last packet sent to the stream.
  // Therefore video packets starting at or after this time need to be copied
||||||| merged common ancestors
  // mNextVideoTime is the end timestamp for the last packet sent to the stream.
  // Therefore video packets starting at or after this time need to be copied
=======
  // Count of video frames written to the stream in the stream's rate
  StreamTime mStreamVideoWritten;
  // Count of audio frames written to the stream in the stream's rate
  StreamTime mStreamAudioWritten;
  // mNextAudioTime is the end timestamp for the last packet sent to the stream.
  // Therefore audio packets starting at or after this time need to be copied
>>>>>>> upstream-releases
  // to the output stream.
  TimeUnit mNextAudioTime;
  // mLastVideoStartTime is the start timestamp for the last packet sent to the
  // stream. Therefore video packets starting after this time need to be copied
  // to the output stream.
  NullableTimeUnit mLastVideoStartTime;
  // mLastVideoEndTime is the end timestamp for the last packet sent to the
  // stream. It is used to adjust durations of chunks sent to the output stream
  // when there are overlaps in VideoData.
  NullableTimeUnit mLastVideoEndTime;
  // The timestamp of the last frame, so we can ensure time never goes
  // backwards.
  TimeStamp mLastVideoTimeStamp;
  // The last video image sent to the stream. Useful if we need to replicate
  // the image.
  RefPtr<layers::Image> mLastVideoImage;
  gfx::IntSize mLastVideoImageDisplaySize;
  bool mHaveSentFinishAudio;
  bool mHaveSentFinishVideo;

  // The decoder is responsible for calling Destroy() on this stream.
  const RefPtr<SourceMediaStream> mStream;
  const RefPtr<DecodedStreamGraphListener> mListener;
<<<<<<< HEAD
  // True if we need to send a compensation video frame to ensure the
  // StreamTime going forward.
  bool mEOSVideoCompensation;
||||||| merged common ancestors
  bool mPlaying;
  // True if we need to send a compensation video frame to ensure the
  // StreamTime going forward.
  bool mEOSVideoCompensation;
=======
>>>>>>> upstream-releases

  const RefPtr<OutputStreamManager> mOutputStreamManager;
  const RefPtr<AbstractThread> mAbstractMainThread;
};

<<<<<<< HEAD
DecodedStreamData::DecodedStreamData(
    OutputStreamManager* aOutputStreamManager, PlaybackInfoInit&& aInit,
    MozPromiseHolder<GenericPromise>&& aAudioPromise,
    MozPromiseHolder<GenericPromise>&& aVideoPromise,
    AbstractThread* aMainThread)
    : mAudioFramesWritten(0),
      mStreamVideoWritten(0),
      mStreamAudioWritten(0),
      mNextVideoTime(aInit.mStartTime),
      mNextAudioTime(aInit.mStartTime),
      mHaveSentFinishAudio(false),
      mHaveSentFinishVideo(false),
      mStream(aOutputStreamManager->mSourceStream),
      // DecodedStreamGraphListener will resolve these promises.
      mListener(MakeRefPtr<DecodedStreamGraphListener>(
          mStream, aInit.mAudioTrackID, std::move(aAudioPromise),
          aInit.mVideoTrackID, std::move(aVideoPromise), aMainThread)),
      mEOSVideoCompensation(false),
      mOutputStreamManager(aOutputStreamManager),
      mAbstractMainThread(aMainThread) {
  MOZ_ASSERT(NS_IsMainThread());
  // Initialize tracks on main thread and in the MediaStreamGraph.
  // Tracks on main thread may have been created early in OutputStreamManager
  // by the state machine, since creating them here is async from the js call.
  // If they were pre-created in OutputStreamManager and the MediaInfo has
  // changed since then, we end them and create new tracks.
  if (!mOutputStreamManager->HasTracks(aInit.mAudioTrackID,
                                       aInit.mVideoTrackID)) {
    // Because these tracks were pre-allocated, we also have to increment the
    // internal track allocator by the same number of tracks, so we don't risk
    // a TrackID collision.
    for (size_t i = 0; i < mOutputStreamManager->NumberOfTracks(); ++i) {
      Unused << mOutputStreamManager->AllocateNextTrackID();
    }
    mOutputStreamManager->RemoveTracks();
  }
  if (IsTrackIDExplicit(aInit.mAudioTrackID)) {
    if (!mOutputStreamManager->HasTrack(aInit.mAudioTrackID)) {
      mOutputStreamManager->AddTrack(aInit.mAudioTrackID, MediaSegment::AUDIO);
    }
    mStream->AddAudioTrack(aInit.mAudioTrackID, aInit.mInfo.mAudio.mRate,
                           new AudioSegment());
  }
  if (IsTrackIDExplicit(aInit.mVideoTrackID)) {
    if (!mOutputStreamManager->HasTrack(aInit.mVideoTrackID)) {
      mOutputStreamManager->AddTrack(aInit.mVideoTrackID, MediaSegment::VIDEO);
    }
    mStream->AddTrack(aInit.mVideoTrackID, new VideoSegment());
  }
}

DecodedStreamData::~DecodedStreamData() { MOZ_ASSERT(NS_IsMainThread()); }

MediaEventSource<int64_t>& DecodedStreamData::OnOutput() {
  return mListener->OnOutput();
}
||||||| merged common ancestors
DecodedStreamData::DecodedStreamData(OutputStreamManager* aOutputStreamManager,
                                     PlaybackInfoInit&& aInit,
                                     MozPromiseHolder<GenericPromise>&& aPromise,
                                     AbstractThread* aMainThread)
  : mAudioFramesWritten(0)
  , mNextVideoTime(aInit.mStartTime)
  , mNextAudioTime(aInit.mStartTime)
  , mHaveSentFinish(false)
  , mHaveSentFinishAudio(false)
  , mHaveSentFinishVideo(false)
  , mStream(aOutputStreamManager->Graph()->CreateSourceStream())
  // DecodedStreamGraphListener will resolve this promise.
  , mListener(new DecodedStreamGraphListener(mStream, std::move(aPromise), aMainThread))
  // mPlaying is initially true because MDSM won't start playback until playing
  // becomes true. This is consistent with the settings of AudioSink.
  , mPlaying(true)
  , mEOSVideoCompensation(false)
  , mOutputStreamManager(aOutputStreamManager)
  , mAbstractMainThread(aMainThread)
{
  mStream->AddListener(mListener);
  TrackID audioTrack = TRACK_NONE;
  TrackID videoTrack = TRACK_NONE;

  // Initialize tracks.
  if (aInit.mInfo.HasAudio()) {
    audioTrack = aInit.mInfo.mAudio.mTrackId;
    mStream->AddAudioTrack(audioTrack,
                           aInit.mInfo.mAudio.mRate,
                           0, new AudioSegment());
  }
  if (aInit.mInfo.HasVideo()) {
    videoTrack = aInit.mInfo.mVideo.mTrackId;
    mStream->AddTrack(videoTrack, 0, new VideoSegment());
  }

  mOutputStreamManager->Connect(mStream, audioTrack, videoTrack);
}

DecodedStreamData::~DecodedStreamData()
{
  mOutputStreamManager->Disconnect();
  mStream->Destroy();
}

MediaEventSource<int64_t>&
DecodedStreamData::OnOutput()
{
  return mListener->OnOutput();
}

void
DecodedStreamData::SetPlaying(bool aPlaying)
{
  if (mPlaying != aPlaying) {
    mPlaying = aPlaying;
    UpdateStreamSuspended(mAbstractMainThread, mStream, !mPlaying);
  }
}

void
DecodedStreamData::Forget()
{
  mListener->Forget();
}
=======
DecodedStreamData::DecodedStreamData(
    OutputStreamManager* aOutputStreamManager, PlaybackInfoInit&& aInit,
    MozPromiseHolder<DecodedStream::EndedPromise>&& aAudioEndedPromise,
    MozPromiseHolder<DecodedStream::EndedPromise>&& aVideoEndedPromise,
    AbstractThread* aMainThread)
    : mAudioFramesWritten(0),
      mStreamVideoWritten(0),
      mStreamAudioWritten(0),
      mNextAudioTime(aInit.mStartTime),
      mHaveSentFinishAudio(false),
      mHaveSentFinishVideo(false),
      mStream(aOutputStreamManager->mSourceStream),
      // DecodedStreamGraphListener will resolve these promises.
      mListener(MakeRefPtr<DecodedStreamGraphListener>(
          mStream, aInit.mAudioTrackID, std::move(aAudioEndedPromise),
          aInit.mVideoTrackID, std::move(aVideoEndedPromise), aMainThread)),
      mOutputStreamManager(aOutputStreamManager),
      mAbstractMainThread(aMainThread) {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_DIAGNOSTIC_ASSERT(
      mOutputStreamManager->HasTracks(aInit.mAudioTrackID, aInit.mVideoTrackID),
      "Tracks must be pre-created on main thread");
  if (IsTrackIDExplicit(aInit.mAudioTrackID)) {
    mStream->AddAudioTrack(aInit.mAudioTrackID, aInit.mInfo.mAudio.mRate,
                           new AudioSegment());
  }
  if (IsTrackIDExplicit(aInit.mVideoTrackID)) {
    mStream->AddTrack(aInit.mVideoTrackID, new VideoSegment());
  }
}

DecodedStreamData::~DecodedStreamData() { MOZ_ASSERT(NS_IsMainThread()); }

MediaEventSource<int64_t>& DecodedStreamData::OnOutput() {
  return mListener->OnOutput();
}

void DecodedStreamData::Forget() { mListener->Forget(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
void DecodedStreamData::Forget() { mListener->Forget(); }

nsCString DecodedStreamData::GetDebugInfo() {
  return nsPrintfCString(
      "DecodedStreamData=%p mAudioFramesWritten=%" PRId64
      " mStreamAudioWritten=%" PRId64 " mStreamVideoWritten=%" PRId64
      " mNextAudioTime=%" PRId64 " mNextVideoTime=%" PRId64
      "mHaveSentFinishAudio=%d mHaveSentFinishVideo=%d",
      this, mAudioFramesWritten, mStreamAudioWritten, mStreamVideoWritten,
      mNextAudioTime.ToMicroseconds(), mNextVideoTime.ToMicroseconds(),
      mHaveSentFinishAudio, mHaveSentFinishVideo);
||||||| merged common ancestors
nsCString
DecodedStreamData::GetDebugInfo()
{
  return nsPrintfCString(
    "DecodedStreamData=%p mPlaying=%d mAudioFramesWritten=%" PRId64
    " mNextAudioTime=%" PRId64 " mNextVideoTime=%" PRId64 " mHaveSentFinish=%d "
    "mHaveSentFinishAudio=%d mHaveSentFinishVideo=%d",
    this, mPlaying, mAudioFramesWritten, mNextAudioTime.ToMicroseconds(),
    mNextVideoTime.ToMicroseconds(), mHaveSentFinish, mHaveSentFinishAudio,
    mHaveSentFinishVideo);
=======
void DecodedStreamData::GetDebugInfo(dom::DecodedStreamDataDebugInfo& aInfo) {
  aInfo.mInstance = NS_ConvertUTF8toUTF16(nsPrintfCString("%p", this));
  aInfo.mAudioFramesWritten = mAudioFramesWritten;
  aInfo.mStreamAudioWritten = mStreamAudioWritten;
  aInfo.mNextAudioTime = mNextAudioTime.ToMicroseconds();
  aInfo.mLastVideoStartTime =
      mLastVideoStartTime.valueOr(TimeUnit::FromMicroseconds(-1))
          .ToMicroseconds();
  aInfo.mLastVideoEndTime =
      mLastVideoEndTime.valueOr(TimeUnit::FromMicroseconds(-1))
          .ToMicroseconds();
  aInfo.mHaveSentFinishAudio = mHaveSentFinishAudio;
  aInfo.mHaveSentFinishVideo = mHaveSentFinishVideo;
>>>>>>> upstream-releases
}

DecodedStream::DecodedStream(AbstractThread* aOwnerThread,
                             AbstractThread* aMainThread,
                             MediaQueue<AudioData>& aAudioQueue,
                             MediaQueue<VideoData>& aVideoQueue,
                             OutputStreamManager* aOutputStreamManager,
<<<<<<< HEAD
                             const bool& aSameOrigin)
    : mOwnerThread(aOwnerThread),
      mAbstractMainThread(aMainThread),
      mOutputStreamManager(aOutputStreamManager),
      mWatchManager(this, mOwnerThread),
      mPlaying(false, "DecodedStream::mPlaying"),
      mSameOrigin(aSameOrigin),
      mPrincipalHandle(aOwnerThread, PRINCIPAL_HANDLE_NONE,
                       "DecodedStream::mPrincipalHandle (Mirror)"),
      mAudioQueue(aAudioQueue),
      mVideoQueue(aVideoQueue) {
  mPrincipalHandle.Connect(mOutputStreamManager->CanonicalPrincipalHandle());

  mWatchManager.Watch(mPlaying, &DecodedStream::PlayingChanged);

  PlayingChanged();  // Notify of the initial state
}

DecodedStream::~DecodedStream() {
||||||| merged common ancestors
                             const bool& aSameOrigin,
                             const PrincipalHandle& aPrincipalHandle)
  : mOwnerThread(aOwnerThread)
  , mAbstractMainThread(aMainThread)
  , mOutputStreamManager(aOutputStreamManager)
  , mPlaying(false)
  , mSameOrigin(aSameOrigin)
  , mPrincipalHandle(aPrincipalHandle)
  , mAudioQueue(aAudioQueue)
  , mVideoQueue(aVideoQueue)
{
}

DecodedStream::~DecodedStream()
{
=======
                             const bool& aSameOrigin)
    : mOwnerThread(aOwnerThread),
      mAbstractMainThread(aMainThread),
      mOutputStreamManager(aOutputStreamManager),
      mWatchManager(this, mOwnerThread),
      mPlaying(false, "DecodedStream::mPlaying"),
      mSameOrigin(aSameOrigin),
      mPrincipalHandle(aOwnerThread, PRINCIPAL_HANDLE_NONE,
                       "DecodedStream::mPrincipalHandle (Mirror)"),
      mAudioQueue(aAudioQueue),
      mVideoQueue(aVideoQueue) {
  mPrincipalHandle.Connect(mOutputStreamManager->CanonicalPrincipalHandle());

  mWatchManager.Watch(mPlaying, &DecodedStream::PlayingChanged);
  PlayingChanged();  // Notify of the initial state
}

DecodedStream::~DecodedStream() {
>>>>>>> upstream-releases
  MOZ_ASSERT(mStartTime.isNothing(), "playback should've ended.");
  NS_ProxyRelease("DecodedStream::mOutputStreamManager", mAbstractMainThread,
                  do_AddRef(mOutputStreamManager));
}

<<<<<<< HEAD
const media::MediaSink::PlaybackParams& DecodedStream::GetPlaybackParams()
    const {
||||||| merged common ancestors
const media::MediaSink::PlaybackParams&
DecodedStream::GetPlaybackParams() const
{
=======
const MediaSink::PlaybackParams& DecodedStream::GetPlaybackParams() const {
>>>>>>> upstream-releases
  AssertOwnerThread();
  return mParams;
}

void DecodedStream::SetPlaybackParams(const PlaybackParams& aParams) {
  AssertOwnerThread();
  mParams = aParams;
}

<<<<<<< HEAD
RefPtr<GenericPromise> DecodedStream::OnEnded(TrackType aType) {
||||||| merged common ancestors
RefPtr<GenericPromise>
DecodedStream::OnEnded(TrackType aType)
{
=======
RefPtr<DecodedStream::EndedPromise> DecodedStream::OnEnded(TrackType aType) {
>>>>>>> upstream-releases
  AssertOwnerThread();
  MOZ_ASSERT(mStartTime.isSome());

  if (aType == TrackInfo::kAudioTrack && mInfo.HasAudio()) {
<<<<<<< HEAD
    return mAudioEndPromise;
||||||| merged common ancestors
    // TODO: we should return a promise which is resolved when the audio track
    // is finished. For now this promise is resolved when the whole stream is
    // finished.
    return mFinishPromise;
=======
    return mAudioEndedPromise;
>>>>>>> upstream-releases
  } else if (aType == TrackInfo::kVideoTrack && mInfo.HasVideo()) {
<<<<<<< HEAD
    return mVideoEndPromise;
||||||| merged common ancestors
    return mFinishPromise;
=======
    return mVideoEndedPromise;
>>>>>>> upstream-releases
  }
  return nullptr;
}

nsresult DecodedStream::Start(const TimeUnit& aStartTime,
                              const MediaInfo& aInfo) {
  AssertOwnerThread();
  MOZ_ASSERT(mStartTime.isNothing(), "playback already started.");

  mStartTime.emplace(aStartTime);
  mLastOutputTime = TimeUnit::Zero();
  mInfo = aInfo;
  mPlaying = true;
  ConnectListener();

  class R : public Runnable {
<<<<<<< HEAD
    typedef MozPromiseHolder<GenericPromise> Promise;

   public:
    R(PlaybackInfoInit&& aInit, Promise&& aAudioPromise,
      Promise&& aVideoPromise, OutputStreamManager* aManager,
      AbstractThread* aMainThread)
        : Runnable("CreateDecodedStreamData"),
          mInit(std::move(aInit)),
          mAudioPromise(std::move(aAudioPromise)),
          mVideoPromise(std::move(aVideoPromise)),
          mOutputStreamManager(aManager),
          mAbstractMainThread(aMainThread) {}
    NS_IMETHOD Run() override {
||||||| merged common ancestors
    typedef MozPromiseHolder<GenericPromise> Promise;
  public:
    R(PlaybackInfoInit&& aInit, Promise&& aPromise,
      OutputStreamManager* aManager, AbstractThread* aMainThread)
      : Runnable("CreateDecodedStreamData")
      , mInit(std::move(aInit))
      , mOutputStreamManager(aManager)
      , mAbstractMainThread(aMainThread)
    {
      mPromise = std::move(aPromise);
    }
    NS_IMETHOD Run() override
    {
=======
    typedef MozPromiseHolder<MediaSink::EndedPromise> Promise;

   public:
    R(PlaybackInfoInit&& aInit, Promise&& aAudioEndedPromise,
      Promise&& aVideoEndedPromise, OutputStreamManager* aManager,
      AbstractThread* aMainThread)
        : Runnable("CreateDecodedStreamData"),
          mInit(std::move(aInit)),
          mAudioEndedPromise(std::move(aAudioEndedPromise)),
          mVideoEndedPromise(std::move(aVideoEndedPromise)),
          mOutputStreamManager(aManager),
          mAbstractMainThread(aMainThread) {}
    NS_IMETHOD Run() override {
>>>>>>> upstream-releases
      MOZ_ASSERT(NS_IsMainThread());
      // No need to create a source stream when there are no output streams.
      // This happens when RemoveOutput() is called immediately after
      // StartPlayback().
      if (mOutputStreamManager->IsEmpty()) {
        // Resolve the promise to indicate the end of playback.
<<<<<<< HEAD
        mAudioPromise.Resolve(true, __func__);
        mVideoPromise.Resolve(true, __func__);
||||||| merged common ancestors
        mPromise.Resolve(true, __func__);
=======
        mAudioEndedPromise.Resolve(true, __func__);
        mVideoEndedPromise.Resolve(true, __func__);
>>>>>>> upstream-releases
        return NS_OK;
      }
<<<<<<< HEAD
      mInit.mAudioTrackID = mInit.mInfo.HasAudio()
                                ? mOutputStreamManager->AllocateNextTrackID()
                                : TRACK_NONE;
      mInit.mVideoTrackID = mInit.mInfo.HasVideo()
                                ? mOutputStreamManager->AllocateNextTrackID()
                                : TRACK_NONE;
||||||| merged common ancestors
=======
      if (mInit.mInfo.HasAudio() &&
          !mOutputStreamManager->HasTrackType(MediaSegment::AUDIO)) {
        mOutputStreamManager->AddTrack(MediaSegment::AUDIO);
      }
      if (mInit.mInfo.HasVideo() &&
          !mOutputStreamManager->HasTrackType(MediaSegment::VIDEO)) {
        mOutputStreamManager->AddTrack(MediaSegment::VIDEO);
      }
      mInit.mAudioTrackID =
          mOutputStreamManager->GetLiveTrackIDFor(MediaSegment::AUDIO);
      mInit.mVideoTrackID =
          mOutputStreamManager->GetLiveTrackIDFor(MediaSegment::VIDEO);
>>>>>>> upstream-releases
      mData = MakeUnique<DecodedStreamData>(
<<<<<<< HEAD
          mOutputStreamManager, std::move(mInit), std::move(mAudioPromise),
          std::move(mVideoPromise), mAbstractMainThread);
||||||| merged common ancestors
        mOutputStreamManager, std::move(mInit), std::move(mPromise), mAbstractMainThread);
=======
          mOutputStreamManager, std::move(mInit), std::move(mAudioEndedPromise),
          std::move(mVideoEndedPromise), mAbstractMainThread);
>>>>>>> upstream-releases
      return NS_OK;
    }
    UniquePtr<DecodedStreamData> ReleaseData() { return std::move(mData); }

   private:
    PlaybackInfoInit mInit;
<<<<<<< HEAD
    Promise mAudioPromise;
    Promise mVideoPromise;
||||||| merged common ancestors
    Promise mPromise;
=======
    Promise mAudioEndedPromise;
    Promise mVideoEndedPromise;
>>>>>>> upstream-releases
    RefPtr<OutputStreamManager> mOutputStreamManager;
    UniquePtr<DecodedStreamData> mData;
    const RefPtr<AbstractThread> mAbstractMainThread;
  };

<<<<<<< HEAD
  MozPromiseHolder<GenericPromise> audioHolder;
  mAudioEndPromise = audioHolder.Ensure(__func__);
  MozPromiseHolder<GenericPromise> videoHolder;
  mVideoEndPromise = videoHolder.Ensure(__func__);
  PlaybackInfoInit init{aStartTime, aInfo, TRACK_INVALID, TRACK_INVALID};
  nsCOMPtr<nsIRunnable> r =
      new R(std::move(init), std::move(audioHolder), std::move(videoHolder),
            mOutputStreamManager, mAbstractMainThread);
||||||| merged common ancestors
  MozPromiseHolder<GenericPromise> promise;
  mFinishPromise = promise.Ensure(__func__);
  PlaybackInfoInit init {
    aStartTime, aInfo
  };
  nsCOMPtr<nsIRunnable> r =
    new R(std::move(init), std::move(promise), mOutputStreamManager, mAbstractMainThread);
=======
  MozPromiseHolder<DecodedStream::EndedPromise> audioEndedHolder;
  mAudioEndedPromise = audioEndedHolder.Ensure(__func__);
  MozPromiseHolder<DecodedStream::EndedPromise> videoEndedHolder;
  mVideoEndedPromise = videoEndedHolder.Ensure(__func__);
  PlaybackInfoInit init{aStartTime, aInfo, TRACK_INVALID, TRACK_INVALID};
  nsCOMPtr<nsIRunnable> r = new R(std::move(init), std::move(audioEndedHolder),
                                  std::move(videoEndedHolder),
                                  mOutputStreamManager, mAbstractMainThread);
>>>>>>> upstream-releases
  SyncRunnable::DispatchToThread(
<<<<<<< HEAD
      SystemGroup::EventTargetFor(mozilla::TaskCategory::Other), r);
||||||| merged common ancestors
    SystemGroup::EventTargetFor(mozilla::TaskCategory::Other), r);
=======
      SystemGroup::EventTargetFor(TaskCategory::Other), r);
>>>>>>> upstream-releases
  mData = static_cast<R*>(r.get())->ReleaseData();

  if (mData) {
    mInfo.mAudio.mTrackId = mData->mListener->AudioTrackID();
    mInfo.mVideo.mTrackId = mData->mListener->VideoTrackID();
    mOutputListener = mData->OnOutput().Connect(mOwnerThread, this,
                                                &DecodedStream::NotifyOutput);
    SendData();
  }
  return NS_OK;
}

void DecodedStream::Stop() {
  AssertOwnerThread();
  MOZ_ASSERT(mStartTime.isSome(), "playback not started.");

<<<<<<< HEAD
  mStreamTimeOffset += SentDuration();
  mStartTime.reset();
||||||| merged common ancestors
  mStartTime.reset();
=======
>>>>>>> upstream-releases
  DisconnectListener();
<<<<<<< HEAD
  mAudioEndPromise = nullptr;
  mVideoEndPromise = nullptr;
||||||| merged common ancestors
  mFinishPromise = nullptr;
=======
  ResetVideo(mPrincipalHandle);
  mStreamTimeOffset += SentDuration();
  mStartTime.reset();
  mAudioEndedPromise = nullptr;
  mVideoEndedPromise = nullptr;
>>>>>>> upstream-releases

  // Clear mData immediately when this playback session ends so we won't
  // send data to the wrong stream in SendData() in next playback session.
  DestroyData(std::move(mData));
}

bool DecodedStream::IsStarted() const {
  AssertOwnerThread();
  return mStartTime.isSome();
}

bool DecodedStream::IsPlaying() const {
  AssertOwnerThread();
  return IsStarted() && mPlaying;
}

void DecodedStream::Shutdown() {
  AssertOwnerThread();
  mPrincipalHandle.DisconnectIfConnected();
  mWatchManager.Shutdown();
}

void DecodedStream::DestroyData(UniquePtr<DecodedStreamData>&& aData) {
  AssertOwnerThread();

  if (!aData) {
    return;
  }

  mOutputListener.Disconnect();

<<<<<<< HEAD
  NS_DispatchToMainThread(
      NS_NewRunnableFunction("DecodedStream::DestroyData",
                             [data = std::move(aData)]() { data->Forget(); }));
||||||| merged common ancestors
  DecodedStreamData* data = aData.release();
  data->Forget();
  nsCOMPtr<nsIRunnable> r = NS_NewRunnableFunction("DecodedStream::DestroyData",
                                                   [=]() { delete data; });
  NS_DispatchToMainThread(r.forget());
=======
  NS_DispatchToMainThread(NS_NewRunnableFunction(
      "DecodedStream::DestroyData",
      [data = std::move(aData), manager = mOutputStreamManager]() {
        data->Forget();
        manager->RemoveTracks();
      }));
>>>>>>> upstream-releases
}

void DecodedStream::SetPlaying(bool aPlaying) {
  AssertOwnerThread();

  // Resume/pause matters only when playback started.
  if (mStartTime.isNothing()) {
    return;
  }

  mPlaying = aPlaying;
}

void DecodedStream::SetVolume(double aVolume) {
  AssertOwnerThread();
  mParams.mVolume = aVolume;
}

void DecodedStream::SetPlaybackRate(double aPlaybackRate) {
  AssertOwnerThread();
  mParams.mPlaybackRate = aPlaybackRate;
}

void DecodedStream::SetPreservesPitch(bool aPreservesPitch) {
  AssertOwnerThread();
  mParams.mPreservesPitch = aPreservesPitch;
}

static void SendStreamAudio(DecodedStreamData* aStream,
                            const TimeUnit& aStartTime, AudioData* aData,
                            AudioSegment* aOutput, uint32_t aRate,
                            const PrincipalHandle& aPrincipalHandle) {
  // The amount of audio frames that is used to fuzz rounding errors.
  static const int64_t AUDIO_FUZZ_FRAMES = 1;

  MOZ_ASSERT(aData);
  AudioData* audio = aData;
  // This logic has to mimic AudioSink closely to make sure we write
  // the exact same silences
  CheckedInt64 audioWrittenOffset =
      aStream->mAudioFramesWritten + TimeUnitToFrames(aStartTime, aRate);
  CheckedInt64 frameOffset = TimeUnitToFrames(audio->mTime, aRate);

  if (!audioWrittenOffset.isValid() || !frameOffset.isValid() ||
      // ignore packet that we've already processed
      audio->GetEndTime() <= aStream->mNextAudioTime) {
    return;
  }

  if (audioWrittenOffset.value() + AUDIO_FUZZ_FRAMES < frameOffset.value()) {
    int64_t silentFrames = frameOffset.value() - audioWrittenOffset.value();
    // Write silence to catch up
    AudioSegment silence;
    silence.InsertNullDataAtStart(silentFrames);
    aStream->mAudioFramesWritten += silentFrames;
    audioWrittenOffset += silentFrames;
    aOutput->AppendFrom(&silence);
  }

  // Always write the whole sample without truncation to be consistent with
  // DecodedAudioDataSink::PlayFromAudioQueue()
  audio->EnsureAudioBuffer();
  RefPtr<SharedBuffer> buffer = audio->mAudioBuffer;
  AudioDataValue* bufferData = static_cast<AudioDataValue*>(buffer->Data());
  AutoTArray<const AudioDataValue*, 2> channels;
  for (uint32_t i = 0; i < audio->mChannels; ++i) {
    channels.AppendElement(bufferData + i * audio->Frames());
  }
<<<<<<< HEAD
  aOutput->AppendFrames(buffer.forget(), channels, audio->mFrames,
                        aPrincipalHandle);
  aStream->mAudioFramesWritten += audio->mFrames;
||||||| merged common ancestors
  aOutput->AppendFrames(buffer.forget(), channels, audio->mFrames, aPrincipalHandle);
  aStream->mAudioFramesWritten += audio->mFrames;
=======
  aOutput->AppendFrames(buffer.forget(), channels, audio->Frames(),
                        aPrincipalHandle);
  aStream->mAudioFramesWritten += audio->Frames();
>>>>>>> upstream-releases

  aStream->mNextAudioTime = audio->GetEndTime();
}

void DecodedStream::SendAudio(double aVolume, bool aIsSameOrigin,
                              const PrincipalHandle& aPrincipalHandle) {
  AssertOwnerThread();

  if (!mInfo.HasAudio()) {
    return;
  }

  if (mData->mHaveSentFinishAudio) {
    return;
  }

  AudioSegment output;
  uint32_t rate = mInfo.mAudio.mRate;
  AutoTArray<RefPtr<AudioData>, 10> audio;
  TrackID audioTrackId = mInfo.mAudio.mTrackId;
  SourceMediaStream* sourceStream = mData->mStream;

  // It's OK to hold references to the AudioData because AudioData
  // is ref-counted.
  mAudioQueue.GetElementsAfter(mData->mNextAudioTime, &audio);
  for (uint32_t i = 0; i < audio.Length(); ++i) {
    SendStreamAudio(mData.get(), mStartTime.ref(), audio[i], &output, rate,
                    aPrincipalHandle);
  }

  output.ApplyVolume(aVolume);

  if (!aIsSameOrigin) {
    output.ReplaceWithDisabled();
  }

  // |mNextAudioTime| is updated as we process each audio sample in
  // SendStreamAudio().
  if (output.GetDuration() > 0) {
    mData->mStreamAudioWritten +=
        sourceStream->AppendToTrack(audioTrackId, &output);
  }

  if (mAudioQueue.IsFinished() && !mData->mHaveSentFinishAudio) {
    mData->mListener->EndTrackAt(audioTrackId, mData->mStreamAudioWritten);
    mData->mHaveSentFinishAudio = true;
  }
}

<<<<<<< HEAD
static void WriteVideoToMediaStream(MediaStream* aStream, layers::Image* aImage,
                                    const TimeUnit& aEnd,
                                    const TimeUnit& aStart,
                                    const mozilla::gfx::IntSize& aIntrinsicSize,
                                    const TimeStamp& aTimeStamp,
                                    VideoSegment* aOutput,
                                    const PrincipalHandle& aPrincipalHandle) {
||||||| merged common ancestors
static void
WriteVideoToMediaStream(MediaStream* aStream,
                        layers::Image* aImage,
                        const TimeUnit& aEnd,
                        const TimeUnit& aStart,
                        const mozilla::gfx::IntSize& aIntrinsicSize,
                        const TimeStamp& aTimeStamp,
                        VideoSegment* aOutput,
                        const PrincipalHandle& aPrincipalHandle)
{
=======
void DecodedStreamData::WriteVideoToSegment(
    layers::Image* aImage, const TimeUnit& aStart, const TimeUnit& aEnd,
    const gfx::IntSize& aIntrinsicSize, const TimeStamp& aTimeStamp,
    VideoSegment* aOutput, const PrincipalHandle& aPrincipalHandle) {
>>>>>>> upstream-releases
  RefPtr<layers::Image> image = aImage;
<<<<<<< HEAD
  auto end = aStream->MicrosecondsToStreamTimeRoundDown(aEnd.ToMicroseconds());
  auto start =
      aStream->MicrosecondsToStreamTimeRoundDown(aStart.ToMicroseconds());
  StreamTime duration = end - start;
  aOutput->AppendFrame(image.forget(), duration, aIntrinsicSize,
                       aPrincipalHandle, false, aTimeStamp);
}

static bool ZeroDurationAtLastChunk(VideoSegment& aInput) {
||||||| merged common ancestors
  auto end = aStream->MicrosecondsToStreamTimeRoundDown(aEnd.ToMicroseconds());
  auto start = aStream->MicrosecondsToStreamTimeRoundDown(aStart.ToMicroseconds());
  StreamTime duration = end - start;
  aOutput->AppendFrame(image.forget(), duration, aIntrinsicSize,
                       aPrincipalHandle, false, aTimeStamp);
}

static bool
ZeroDurationAtLastChunk(VideoSegment& aInput)
{
=======
  auto end = mStream->MicrosecondsToStreamTimeRoundDown(aEnd.ToMicroseconds());
  auto start =
      mStream->MicrosecondsToStreamTimeRoundDown(aStart.ToMicroseconds());
  aOutput->AppendFrame(image.forget(), aIntrinsicSize, aPrincipalHandle, false,
                       aTimeStamp);
  // Extend this so we get accurate durations for all frames.
  // Because this track is pushed, we need durations so the graph can track
  // when playout of the track has finished.
  aOutput->ExtendLastFrameBy(end - start);

  mLastVideoStartTime = Some(aStart);
  mLastVideoEndTime = Some(aEnd);
  mLastVideoTimeStamp = aTimeStamp;
}

static bool ZeroDurationAtLastChunk(VideoSegment& aInput) {
>>>>>>> upstream-releases
  // Get the last video frame's start time in VideoSegment aInput.
  // If the start time is equal to the duration of aInput, means the last video
  // frame's duration is zero.
  StreamTime lastVideoStratTime;
  aInput.GetLastFrame(&lastVideoStratTime);
  return lastVideoStratTime == aInput.GetDuration();
}

<<<<<<< HEAD
void DecodedStream::SendVideo(bool aIsSameOrigin,
                              const PrincipalHandle& aPrincipalHandle) {
||||||| merged common ancestors
void
DecodedStream::SendVideo(bool aIsSameOrigin, const PrincipalHandle& aPrincipalHandle)
{
=======
void DecodedStream::ResetVideo(const PrincipalHandle& aPrincipalHandle) {
>>>>>>> upstream-releases
  AssertOwnerThread();

  if (!mData) {
    return;
  }

  if (!mInfo.HasVideo()) {
    return;
  }

<<<<<<< HEAD
  if (mData->mHaveSentFinishVideo) {
    return;
  }

||||||| merged common ancestors
=======
  VideoSegment resetter;
  TimeStamp currentTime;
  TimeUnit currentPosition = GetPosition(&currentTime);

  // Giving direct consumers a frame (really *any* frame, so in this case:
  // nullptr) at an earlier time than the previous, will signal to that consumer
  // to discard any frames ahead in time of the new frame. To be honest, this is
  // an ugly hack because the direct listeners of the MediaStreamGraph do not
  // have an API that supports clearing the future frames. ImageContainer and
  // VideoFrameContainer do though, and we will need to move to a similar API
  // for video tracks as part of bug 1493618.
  resetter.AppendFrame(nullptr, mData->mLastVideoImageDisplaySize,
                       aPrincipalHandle, false, currentTime);
  mData->mStream->AppendToTrack(mInfo.mVideo.mTrackId, &resetter);

  // Consumer buffers have been reset. We now set the next time to the start
  // time of the current frame, so that it can be displayed again on resuming.
  if (RefPtr<VideoData> v = mVideoQueue.PeekFront()) {
    mData->mLastVideoStartTime = Some(v->mTime - TimeUnit::FromMicroseconds(1));
    mData->mLastVideoEndTime = Some(v->mTime);
  } else {
    // There was no current frame in the queue. We set the next time to the
    // current time, so we at least don't resume starting in the future.
    mData->mLastVideoStartTime =
        Some(currentPosition - TimeUnit::FromMicroseconds(1));
    mData->mLastVideoEndTime = Some(currentPosition);
  }

  mData->mLastVideoTimeStamp = currentTime;
}

void DecodedStream::SendVideo(bool aIsSameOrigin,
                              const PrincipalHandle& aPrincipalHandle) {
  AssertOwnerThread();

  if (!mInfo.HasVideo()) {
    return;
  }

  if (mData->mHaveSentFinishVideo) {
    return;
  }

>>>>>>> upstream-releases
  VideoSegment output;
  TrackID videoTrackId = mInfo.mVideo.mTrackId;
  AutoTArray<RefPtr<VideoData>, 10> video;
  SourceMediaStream* sourceStream = mData->mStream;

  // It's OK to hold references to the VideoData because VideoData
  // is ref-counted.
  mVideoQueue.GetElementsAfter(
      mData->mLastVideoStartTime.valueOr(mStartTime.ref()), &video);

  TimeStamp currentTime;
  TimeUnit currentPosition = GetPosition(&currentTime);

<<<<<<< HEAD
  // tracksStartTimeStamp might be null when the SourceMediaStream not yet
  // be added to MediaStreamGraph.
  TimeStamp tracksStartTimeStamp =
      sourceStream->GetStreamTracksStrartTimeStamp();
  if (tracksStartTimeStamp.IsNull()) {
    tracksStartTimeStamp = TimeStamp::Now();
||||||| merged common ancestors
  // tracksStartTimeStamp might be null when the SourceMediaStream not yet
  // be added to MediaStreamGraph.
  TimeStamp tracksStartTimeStamp = sourceStream->GetStreamTracksStrartTimeStamp();
  if (tracksStartTimeStamp.IsNull()) {
    tracksStartTimeStamp = TimeStamp::Now();
=======
  if (mData->mLastVideoTimeStamp.IsNull()) {
    mData->mLastVideoTimeStamp = currentTime;
>>>>>>> upstream-releases
  }

  for (uint32_t i = 0; i < video.Length(); ++i) {
    VideoData* v = video[i];
    TimeUnit lastStart = mData->mLastVideoStartTime.valueOr(
        mStartTime.ref() - TimeUnit::FromMicroseconds(1));
    TimeUnit lastEnd = mData->mLastVideoEndTime.valueOr(mStartTime.ref());

    if (lastEnd < v->mTime) {
      // Write last video frame to catch up. mLastVideoImage can be null here
      // which is fine, it just means there's no video.

      // TODO: |mLastVideoImage| should come from the last image rendered
      // by the state machine. This will avoid the black frame when capture
      // happens in the middle of playback (especially in th middle of a
      // video frame). E.g. if we have a video frame that is 30 sec long
      // and capture happens at 15 sec, we'll have to append a black frame
      // that is 15 sec long.
<<<<<<< HEAD
      WriteVideoToMediaStream(sourceStream, mData->mLastVideoImage, v->mTime,
                              mData->mNextVideoTime,
                              mData->mLastVideoImageDisplaySize,
                              tracksStartTimeStamp + v->mTime.ToTimeDuration(),
                              &output, aPrincipalHandle);
      mData->mNextVideoTime = v->mTime;
||||||| merged common ancestors
      WriteVideoToMediaStream(sourceStream, mData->mLastVideoImage, v->mTime,
        mData->mNextVideoTime, mData->mLastVideoImageDisplaySize,
        tracksStartTimeStamp + v->mTime.ToTimeDuration(),
        &output, aPrincipalHandle);
      mData->mNextVideoTime = v->mTime;
=======
      TimeStamp t =
          std::max(mData->mLastVideoTimeStamp,
                   currentTime + (lastEnd - currentPosition).ToTimeDuration());
      mData->WriteVideoToSegment(mData->mLastVideoImage, lastEnd, v->mTime,
                                 mData->mLastVideoImageDisplaySize, t, &output,
                                 aPrincipalHandle);
      lastEnd = v->mTime;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (mData->mNextVideoTime < v->GetEndTime()) {
      WriteVideoToMediaStream(
          sourceStream, v->mImage, v->GetEndTime(), mData->mNextVideoTime,
          v->mDisplay, tracksStartTimeStamp + v->GetEndTime().ToTimeDuration(),
          &output, aPrincipalHandle);
      mData->mNextVideoTime = v->GetEndTime();
||||||| merged common ancestors
    if (mData->mNextVideoTime < v->GetEndTime()) {
      WriteVideoToMediaStream(sourceStream, v->mImage, v->GetEndTime(),
        mData->mNextVideoTime, v->mDisplay,
        tracksStartTimeStamp + v->GetEndTime().ToTimeDuration(),
        &output, aPrincipalHandle);
      mData->mNextVideoTime = v->GetEndTime();
=======
    if (lastStart < v->mTime) {
      // This frame starts after the last frame's start. Note that this could be
      // before the last frame's end time for some videos. This only matters for
      // the track's lifetime in the MSG, as rendering is based on timestamps,
      // aka frame start times.
      TimeStamp t =
          std::max(mData->mLastVideoTimeStamp,
                   currentTime + (lastEnd - currentPosition).ToTimeDuration());
      TimeUnit end = std::max(
          v->GetEndTime(),
          lastEnd + TimeUnit::FromMicroseconds(
                        sourceStream->StreamTimeToMicroseconds(1) + 1));
>>>>>>> upstream-releases
      mData->mLastVideoImage = v->mImage;
      mData->mLastVideoImageDisplaySize = v->mDisplay;
      mData->WriteVideoToSegment(v->mImage, lastEnd, end, v->mDisplay, t,
                                 &output, aPrincipalHandle);
    }
  }

  // Check the output is not empty.
  bool compensateEOS = false;
  if (output.GetLastFrame()) {
    compensateEOS = ZeroDurationAtLastChunk(output);
  }

  if (!aIsSameOrigin) {
    output.ReplaceWithDisabled();
  }

  if (output.GetDuration() > 0) {
    mData->mStreamVideoWritten +=
        sourceStream->AppendToTrack(videoTrackId, &output);
  }

  if (mVideoQueue.IsFinished() && !mData->mHaveSentFinishVideo) {
    if (!mData->mLastVideoImage) {
      // We have video, but the video queue finished before we received any
      // frame. We insert a black frame to progress any consuming
      // HTMLMediaElement. This mirrors the behavior of VideoSink.

      // Force a frame - can be null
      compensateEOS = true;
      // Force frame to be black
      aIsSameOrigin = false;
      // Override the frame's size (will be 0x0 otherwise)
      mData->mLastVideoImageDisplaySize = mInfo.mVideo.mDisplay;
    }
    if (compensateEOS) {
      VideoSegment endSegment;
      // Calculate the deviation clock time from DecodedStream.
<<<<<<< HEAD
      auto deviation =
          FromMicroseconds(sourceStream->StreamTimeToMicroseconds(1));
      WriteVideoToMediaStream(
          sourceStream, mData->mLastVideoImage,
          mData->mNextVideoTime + deviation, mData->mNextVideoTime,
          mData->mLastVideoImageDisplaySize,
          tracksStartTimeStamp +
              (mData->mNextVideoTime + deviation).ToTimeDuration(),
          &endSegment, aPrincipalHandle);
      mData->mNextVideoTime += deviation;
||||||| merged common ancestors
      auto deviation = FromMicroseconds(sourceStream->StreamTimeToMicroseconds(1));
      WriteVideoToMediaStream(sourceStream, mData->mLastVideoImage,
        mData->mNextVideoTime + deviation, mData->mNextVideoTime,
        mData->mLastVideoImageDisplaySize,
        tracksStartTimeStamp + (mData->mNextVideoTime + deviation).ToTimeDuration(),
        &endSegment, aPrincipalHandle);
      mData->mNextVideoTime += deviation;
=======
      // We round the nr of microseconds up, because WriteVideoToSegment
      // will round the conversion from microseconds to StreamTime down.
      auto deviation = TimeUnit::FromMicroseconds(
          sourceStream->StreamTimeToMicroseconds(1) + 1);
      auto start = mData->mLastVideoEndTime.valueOr(mStartTime.ref());
      mData->WriteVideoToSegment(
          mData->mLastVideoImage, start, start + deviation,
          mData->mLastVideoImageDisplaySize,
          currentTime + (start + deviation - currentPosition).ToTimeDuration(),
          &endSegment, aPrincipalHandle);
>>>>>>> upstream-releases
      MOZ_ASSERT(endSegment.GetDuration() > 0);
      if (!aIsSameOrigin) {
        endSegment.ReplaceWithDisabled();
      }
      mData->mStreamVideoWritten +=
          sourceStream->AppendToTrack(videoTrackId, &endSegment);
    }
    mData->mListener->EndTrackAt(videoTrackId, mData->mStreamVideoWritten);
    mData->mHaveSentFinishVideo = true;
  }
}

StreamTime DecodedStream::SentDuration() {
  AssertOwnerThread();

  if (!mData) {
    return 0;
  }

  return std::max(mData->mStreamAudioWritten, mData->mStreamVideoWritten);
}

void DecodedStream::SendData() {
  AssertOwnerThread();

  // Not yet created on the main thread. MDSM will try again later.
  if (!mData) {
    return;
  }

<<<<<<< HEAD
||||||| merged common ancestors
  // Nothing to do when the stream is finished.
  if (mData->mHaveSentFinish) {
    return;
  }

=======
  if (!mPlaying) {
    return;
  }

>>>>>>> upstream-releases
  SendAudio(mParams.mVolume, mSameOrigin, mPrincipalHandle);
  SendVideo(mSameOrigin, mPrincipalHandle);
}

TimeUnit DecodedStream::GetEndTime(TrackType aType) const {
  AssertOwnerThread();
  if (aType == TrackInfo::kAudioTrack && mInfo.HasAudio() && mData) {
    auto t = mStartTime.ref() +
             FramesToTimeUnit(mData->mAudioFramesWritten, mInfo.mAudio.mRate);
    if (t.IsValid()) {
      return t;
    }
  } else if (aType == TrackInfo::kVideoTrack && mData) {
    return mData->mLastVideoEndTime.valueOr(mStartTime.ref());
  }
  return TimeUnit::Zero();
}

TimeUnit DecodedStream::GetPosition(TimeStamp* aTimeStamp) const {
  AssertOwnerThread();
  // This is only called after MDSM starts playback. So mStartTime is
  // guaranteed to be something.
  MOZ_ASSERT(mStartTime.isSome());
  if (aTimeStamp) {
    *aTimeStamp = TimeStamp::Now();
  }
  return mStartTime.ref() + mLastOutputTime;
}

void DecodedStream::NotifyOutput(int64_t aTime) {
  AssertOwnerThread();
  TimeUnit time = TimeUnit::FromMicroseconds(aTime);
  if (time == mLastOutputTime) {
    return;
  }
  MOZ_ASSERT(mLastOutputTime < time);
  mLastOutputTime = time;
  auto currentTime = GetPosition();

  // Remove audio samples that have been played by MSG from the queue.
  RefPtr<AudioData> a = mAudioQueue.PeekFront();
  for (; a && a->mTime < currentTime;) {
    RefPtr<AudioData> releaseMe = mAudioQueue.PopFront();
    a = mAudioQueue.PeekFront();
  }
}

<<<<<<< HEAD
void DecodedStream::PlayingChanged() {
  AssertOwnerThread();

  mAbstractMainThread->Dispatch(NewRunnableMethod<bool>(
      "OutputStreamManager::SetPlaying", mOutputStreamManager,
      &OutputStreamManager::SetPlaying, mPlaying));
}

void DecodedStream::ConnectListener() {
||||||| merged common ancestors
void
DecodedStream::ConnectListener()
{
=======
void DecodedStream::PlayingChanged() {
  AssertOwnerThread();

  if (!mPlaying) {
    // On seek or pause we discard future frames.
    ResetVideo(mPrincipalHandle);
  }

  mAbstractMainThread->Dispatch(NewRunnableMethod<bool>(
      "OutputStreamManager::SetPlaying", mOutputStreamManager,
      &OutputStreamManager::SetPlaying, mPlaying));
}

void DecodedStream::ConnectListener() {
>>>>>>> upstream-releases
  AssertOwnerThread();

  mAudioPushListener = mAudioQueue.PushEvent().Connect(
      mOwnerThread, this, &DecodedStream::SendData);
  mAudioFinishListener = mAudioQueue.FinishEvent().Connect(
      mOwnerThread, this, &DecodedStream::SendData);
  mVideoPushListener = mVideoQueue.PushEvent().Connect(
      mOwnerThread, this, &DecodedStream::SendData);
  mVideoFinishListener = mVideoQueue.FinishEvent().Connect(
<<<<<<< HEAD
      mOwnerThread, this, &DecodedStream::SendData);
||||||| merged common ancestors
    mOwnerThread, this, &DecodedStream::SendData);
=======
      mOwnerThread, this, &DecodedStream::SendData);
  mWatchManager.Watch(mPlaying, &DecodedStream::SendData);
>>>>>>> upstream-releases
}

void DecodedStream::DisconnectListener() {
  AssertOwnerThread();

  mAudioPushListener.Disconnect();
  mVideoPushListener.Disconnect();
  mAudioFinishListener.Disconnect();
  mVideoFinishListener.Disconnect();
  mWatchManager.Unwatch(mPlaying, &DecodedStream::SendData);
}

<<<<<<< HEAD
nsCString DecodedStream::GetDebugInfo() {
||||||| merged common ancestors
nsCString
DecodedStream::GetDebugInfo()
{
=======
void DecodedStream::GetDebugInfo(dom::MediaSinkDebugInfo& aInfo) {
>>>>>>> upstream-releases
  AssertOwnerThread();
  int64_t startTime = mStartTime.isSome() ? mStartTime->ToMicroseconds() : -1;
<<<<<<< HEAD
  auto str =
      nsPrintfCString("DecodedStream=%p mStartTime=%" PRId64
                      " mLastOutputTime=%" PRId64 " mPlaying=%d mData=%p",
                      this, startTime, mLastOutputTime.ToMicroseconds(),
                      mPlaying.Ref(), mData.get());
||||||| merged common ancestors
  auto str =
    nsPrintfCString("DecodedStream=%p mStartTime=%" PRId64
                    " mLastOutputTime=%" PRId64 " mPlaying=%d mData=%p",
                    this,
                    startTime,
                    mLastOutputTime.ToMicroseconds(),
                    mPlaying,
                    mData.get());
=======
  aInfo.mDecodedStream.mInstance =
      NS_ConvertUTF8toUTF16(nsPrintfCString("%p", this));
  aInfo.mDecodedStream.mStartTime = startTime;
  aInfo.mDecodedStream.mLastOutputTime = mLastOutputTime.ToMicroseconds();
  aInfo.mDecodedStream.mPlaying = mPlaying.Ref();
  auto lastAudio = mAudioQueue.PeekBack();
  aInfo.mDecodedStream.mLastAudio =
      lastAudio ? lastAudio->GetEndTime().ToMicroseconds() : -1;
  aInfo.mDecodedStream.mAudioQueueFinished = mAudioQueue.IsFinished();
  aInfo.mDecodedStream.mAudioQueueSize = mAudioQueue.GetSize();
>>>>>>> upstream-releases
  if (mData) {
    mData->GetDebugInfo(aInfo.mDecodedStream.mData);
  }
}

}  // namespace mozilla
