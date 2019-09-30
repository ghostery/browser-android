/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "OutputStreamManager.h"

#include "DOMMediaStream.h"
#include "MediaStreamGraph.h"
#include "mozilla/dom/MediaStreamTrack.h"
#include "nsContentUtils.h"

namespace mozilla {

#define LOG(level, msg, ...) \
  MOZ_LOG(gMediaDecoderLog, level, (msg, ##__VA_ARGS__))

class DecodedStreamTrackSource : public dom::MediaStreamTrackSource {
 public:
  NS_DECL_ISUPPORTS_INHERITED
  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(DecodedStreamTrackSource,
                                           dom::MediaStreamTrackSource)

  explicit DecodedStreamTrackSource(OutputStreamManager* aManager,
                                    OutputStreamData* aData, TrackID aTrackID,
                                    nsIPrincipal* aPrincipal,
                                    CORSMode aCORSMode,
                                    AbstractThread* aAbstractMainThread)
      : dom::MediaStreamTrackSource(aPrincipal, nsString()),
        mCORSMode(aCORSMode) {
    MOZ_ASSERT(NS_IsMainThread());
  }

  dom::MediaSourceEnum GetMediaSource() const override {
    return dom::MediaSourceEnum::Other;
  }

  CORSMode GetCORSMode() const override {
    MOZ_ASSERT(NS_IsMainThread());
    return mCORSMode;
  }

  void Stop() override {
    MOZ_ASSERT(NS_IsMainThread());

    // We don't notify the source that a track was stopped since it will keep
    // producing tracks until the element ends. The decoder also needs the
    // tracks it created to be live at the source since the decoder's clock is
    // based on MediaStreams during capture.
  }

  void Disable() override {}

  void Enable() override {}

  void SetPrincipal(nsIPrincipal* aPrincipal) {
    MOZ_ASSERT(NS_IsMainThread());
    mPrincipal = aPrincipal;
    PrincipalChanged();
  }

 protected:
  virtual ~DecodedStreamTrackSource() { MOZ_ASSERT(NS_IsMainThread()); }

  const CORSMode mCORSMode;
};

NS_IMPL_ADDREF_INHERITED(DecodedStreamTrackSource, dom::MediaStreamTrackSource)
NS_IMPL_RELEASE_INHERITED(DecodedStreamTrackSource, dom::MediaStreamTrackSource)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(DecodedStreamTrackSource)
NS_INTERFACE_MAP_END_INHERITING(dom::MediaStreamTrackSource)
NS_IMPL_CYCLE_COLLECTION_INHERITED(DecodedStreamTrackSource,
                                   dom::MediaStreamTrackSource)

OutputStreamData::OutputStreamData(OutputStreamManager* aManager,
                                   AbstractThread* aAbstractMainThread,
                                   DOMMediaStream* aDOMStream)
    : mManager(aManager),
      mAbstractMainThread(aAbstractMainThread),
      mDOMStream(aDOMStream),
      mInputStream(mDOMStream->GetInputStream()->AsProcessedStream()),
      mPort(mInputStream->AllocateInputPort(mManager->mSourceStream)) {
  MOZ_ASSERT(NS_IsMainThread());
}

OutputStreamData::~OutputStreamData() {
  MOZ_ASSERT(NS_IsMainThread());

<<<<<<< HEAD
  // During cycle collection, MediaStream can be destroyed and send
  // its Destroy message before this decoder is destroyed. So we have to
  // be careful not to send any messages after the Destroy().
  if (mInputStream->IsDestroyed()) {
    return;
||||||| merged common ancestors
  // During cycle collection, DOMMediaStream can be destroyed and send
  // its Destroy message before this decoder is destroyed. So we have to
  // be careful not to send any messages after the Destroy().
  if (mStream->IsDestroyed()) {
    return false;
=======
  // Disconnect any existing port.
  if (mPort) {
    mPort->Destroy();
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
  // Disconnect any existing port.
  if (mPort) {
    mPort->Destroy();
||||||| merged common ancestors
  // Disconnect any existing port.
  for (RefPtr<MediaInputPort>& port : mPorts) {
    port->Destroy();
=======
void OutputStreamData::AddTrack(TrackID aTrackID, MediaSegment::Type aType,
                                nsIPrincipal* aPrincipal, CORSMode aCORSMode,
                                bool aAsyncAddTrack) {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_DIAGNOSTIC_ASSERT(mDOMStream);

  LOG(LogLevel::Debug, "Adding output %s track with id %d to MediaStream %p%s",
      aType == MediaSegment::AUDIO ? "audio" : "video", aTrackID,
      mDOMStream.get(), aAsyncAddTrack ? " (async)" : "");

  RefPtr<dom::MediaStreamTrackSource> source = new DecodedStreamTrackSource(
      mManager, this, aTrackID, aPrincipal, aCORSMode, mAbstractMainThread);
  RefPtr<dom::MediaStreamTrack> track =
      mDOMStream->CreateDOMTrack(aTrackID, aType, source);
  mTracks.AppendElement(track.get());
  if (aAsyncAddTrack) {
    GetMainThreadEventTarget()->Dispatch(
        NewRunnableMethod<RefPtr<dom::MediaStreamTrack>>(
            "DOMMediaStream::AddTrackInternal", mDOMStream.get(),
            &DOMMediaStream::AddTrackInternal, track));
  } else {
    mDOMStream->AddTrackInternal(track);
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
void OutputStreamData::AddTrack(TrackID aTrackID, MediaSegment::Type aType,
                                nsIPrincipal* aPrincipal, CORSMode aCORSMode,
                                bool aAsyncAddTrack) {
  MOZ_ASSERT(NS_IsMainThread());

  LOG(LogLevel::Debug, "Adding output %s track with id %d to MediaStream %p%s",
      aType == MediaSegment::AUDIO ? "audio" : "video", aTrackID,
      mDOMStream.get(), aAsyncAddTrack ? " (async)" : "");

  RefPtr<dom::MediaStreamTrackSource> source = new DecodedStreamTrackSource(
      mManager, this, aTrackID, aPrincipal, aCORSMode, mAbstractMainThread);
  RefPtr<dom::MediaStreamTrack> track =
      mDOMStream->CreateDOMTrack(aTrackID, aType, source);
  mTracks.AppendElement(track);
  if (aAsyncAddTrack) {
    GetMainThreadEventTarget()->Dispatch(
        NewRunnableMethod<RefPtr<dom::MediaStreamTrack>>(
            "DOMMediaStream::AddTrackInternal", mDOMStream,
            &DOMMediaStream::AddTrackInternal, track));
  } else {
    mDOMStream->AddTrackInternal(track);
  }
||||||| merged common ancestors
bool
OutputStreamData::Equals(MediaStream* aStream) const
{
  return mStream == aStream;
=======
void OutputStreamData::RemoveTrack(TrackID aTrackID) {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_DIAGNOSTIC_ASSERT(mDOMStream);

  LOG(LogLevel::Debug, "Removing output track with id %d from MediaStream %p",
      aTrackID, mDOMStream.get());

  RefPtr<dom::MediaStreamTrack> track =
      mDOMStream->FindOwnedDOMTrack(mInputStream, aTrackID);
  MOZ_DIAGNOSTIC_ASSERT(track);
  mTracks.RemoveElement(track);
  GetMainThreadEventTarget()->Dispatch(
      NewRunnableMethod("MediaStreamTrack::OverrideEnded", track,
                        &dom::MediaStreamTrack::OverrideEnded));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void OutputStreamData::RemoveTrack(TrackID aTrackID) {
  MOZ_ASSERT(NS_IsMainThread());

  LOG(LogLevel::Debug, "Removing output track with id %d from MediaStream %p",
      aTrackID, mDOMStream.get());

  RefPtr<dom::MediaStreamTrack> track =
      mDOMStream->FindOwnedDOMTrack(mInputStream, aTrackID);
  MOZ_DIAGNOSTIC_ASSERT(track);
  mTracks.RemoveElement(track);
  GetMainThreadEventTarget()->Dispatch(
      NewRunnableMethod("MediaStreamTrack::OverrideEnded", track,
                        &dom::MediaStreamTrack::OverrideEnded));
||||||| merged common ancestors
MediaStreamGraph*
OutputStreamData::Graph() const
{
  return mStream->Graph();
=======
void OutputStreamData::SetPrincipal(nsIPrincipal* aPrincipal) {
  MOZ_DIAGNOSTIC_ASSERT(mDOMStream);
  for (const WeakPtr<dom::MediaStreamTrack>& track : mTracks) {
    MOZ_DIAGNOSTIC_ASSERT(track);
    DecodedStreamTrackSource& source =
        static_cast<DecodedStreamTrackSource&>(track->GetSource());
    source.SetPrincipal(aPrincipal);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void OutputStreamData::SetPrincipal(nsIPrincipal* aPrincipal) {
  for (const RefPtr<dom::MediaStreamTrack>& track : mTracks) {
    DecodedStreamTrackSource& source =
        static_cast<DecodedStreamTrackSource&>(track->GetSource());
    source.SetPrincipal(aPrincipal);
  }
||||||| merged common ancestors
TrackID
OutputStreamData::NextAvailableTrackID() const
{
  return mNextAvailableTrackID;
=======
OutputStreamManager::OutputStreamManager(SourceMediaStream* aSourceStream,
                                         TrackID aNextTrackID,
                                         nsIPrincipal* aPrincipal,
                                         CORSMode aCORSMode,
                                         AbstractThread* aAbstractMainThread)
    : mSourceStream(aSourceStream),
      mAbstractMainThread(aAbstractMainThread),
      mPrincipalHandle(
          aAbstractMainThread,
          aPrincipal ? MakePrincipalHandle(aPrincipal) : PRINCIPAL_HANDLE_NONE,
          "OutputStreamManager::mPrincipalHandle (Canonical)"),
      mPrincipal(aPrincipal),
      mCORSMode(aCORSMode),
      mNextTrackID(aNextTrackID),
      mPlaying(true)  // mSourceStream always starts non-suspended
{
  MOZ_ASSERT(NS_IsMainThread());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
OutputStreamManager::OutputStreamManager(SourceMediaStream* aSourceStream,
                                         TrackID aNextTrackID,
                                         nsIPrincipal* aPrincipal,
                                         CORSMode aCORSMode,
                                         AbstractThread* aAbstractMainThread)
    : mSourceStream(aSourceStream),
      mAbstractMainThread(aAbstractMainThread),
      mPrincipalHandle(
          aAbstractMainThread,
          aPrincipal ? MakePrincipalHandle(aPrincipal) : PRINCIPAL_HANDLE_NONE,
          "OutputStreamManager::mPrincipalHandle (Canonical)"),
      mPrincipal(aPrincipal),
      mCORSMode(aCORSMode),
      mNextTrackID(aNextTrackID),
      mPlaying(true)  // mSourceStream always starts non-suspended
{
||||||| merged common ancestors
void
OutputStreamManager::Add(ProcessedMediaStream* aStream,
                         TrackID aNextAvailableTrackID,
                         bool aFinishWhenEnded)
{
=======
void OutputStreamManager::Add(DOMMediaStream* aDOMStream) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
<<<<<<< HEAD
}

void OutputStreamManager::Add(DOMMediaStream* aDOMStream) {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(!mSourceStream->IsDestroyed());
||||||| merged common ancestors
=======
  MOZ_ASSERT(!mSourceStream->IsDestroyed());
>>>>>>> upstream-releases
  // All streams must belong to the same graph.
<<<<<<< HEAD
  MOZ_ASSERT(mSourceStream->Graph() == aDOMStream->GetInputStream()->Graph());

  LOG(LogLevel::Info, "Adding MediaStream %p", aDOMStream);
||||||| merged common ancestors
  MOZ_ASSERT(!Graph() || Graph() == aStream->Graph());
=======
  MOZ_ASSERT(mSourceStream->Graph() == aDOMStream->GetInputStream()->Graph());

  LOG(LogLevel::Info, "Adding MediaStream %p", aDOMStream);

  OutputStreamData* p = mStreams
                            .AppendElement(new OutputStreamData(
                                this, mAbstractMainThread, aDOMStream))
                            ->get();
  for (const Pair<TrackID, MediaSegment::Type>& pair : mLiveTracks) {
    p->AddTrack(pair.first(), pair.second(), mPrincipal, mCORSMode, false);
  }
}

void OutputStreamManager::Remove(DOMMediaStream* aDOMStream) {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(!mSourceStream->IsDestroyed());

  LOG(LogLevel::Info, "Removing MediaStream %p", aDOMStream);

  AutoRemoveDestroyedStreams();
  mStreams.ApplyIf(
      aDOMStream, 0, StreamComparator(),
      [&](const UniquePtr<OutputStreamData>& aData) {
        for (const Pair<TrackID, MediaSegment::Type>& pair : mLiveTracks) {
          aData->RemoveTrack(pair.first());
        }
      },
      []() { MOZ_ASSERT_UNREACHABLE("Didn't exist"); });
  DebugOnly<bool> rv = mStreams.RemoveElement(aDOMStream, StreamComparator());
  MOZ_ASSERT(rv);
}

bool OutputStreamManager::HasTrackType(MediaSegment::Type aType) {
  MOZ_ASSERT(NS_IsMainThread());
>>>>>>> upstream-releases

<<<<<<< HEAD
  OutputStreamData* p = mStreams
                            .AppendElement(new OutputStreamData(
                                this, mAbstractMainThread, aDOMStream))
                            ->get();
  for (const Pair<TrackID, MediaSegment::Type>& pair : mLiveTracks) {
    p->AddTrack(pair.first(), pair.second(), mPrincipal, mCORSMode, false);
||||||| merged common ancestors
  // Ensure that aStream finishes the moment mDecodedStream does.
  if (aFinishWhenEnded) {
    aStream->QueueSetAutofinish(true);
=======
  return mLiveTracks.Contains(aType, TrackTypeComparator());
}

bool OutputStreamManager::HasTracks(TrackID aAudioTrack, TrackID aVideoTrack) {
  MOZ_ASSERT(NS_IsMainThread());

  size_t nrExpectedTracks = 0;
  bool asExpected = true;
  if (IsTrackIDExplicit(aAudioTrack)) {
    Unused << ++nrExpectedTracks;
    asExpected = asExpected && mLiveTracks.Contains(
                                   MakePair(aAudioTrack, MediaSegment::AUDIO),
                                   TrackComparator());
  }
  if (IsTrackIDExplicit(aVideoTrack)) {
    Unused << ++nrExpectedTracks;
    asExpected = asExpected && mLiveTracks.Contains(
                                   MakePair(aVideoTrack, MediaSegment::VIDEO),
                                   TrackComparator());
>>>>>>> upstream-releases
  }
<<<<<<< HEAD
}

void OutputStreamManager::Remove(DOMMediaStream* aDOMStream) {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(!mSourceStream->IsDestroyed());

  LOG(LogLevel::Info, "Removing MediaStream %p", aDOMStream);

  mStreams.ApplyIf(
      aDOMStream, 0, StreamComparator(),
      [&](const UniquePtr<OutputStreamData>& aData) {
        for (const Pair<TrackID, MediaSegment::Type>& pair : mLiveTracks) {
          aData->RemoveTrack(pair.first());
        }
      },
      []() { MOZ_ASSERT_UNREACHABLE("Didn't exist"); });
  DebugOnly<bool> rv = mStreams.RemoveElement(aDOMStream, StreamComparator());
  MOZ_ASSERT(rv);
}
||||||| merged common ancestors
=======
  asExpected = asExpected && mLiveTracks.Length() == nrExpectedTracks;
  return asExpected;
}

size_t OutputStreamManager::NumberOfTracks() {
  MOZ_ASSERT(NS_IsMainThread());
  return mLiveTracks.Length();
}

void OutputStreamManager::AddTrack(MediaSegment::Type aType) {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(!mSourceStream->IsDestroyed());
  MOZ_ASSERT(!HasTrackType(aType),
             "Cannot have two tracks of the same type at the same time");
>>>>>>> upstream-releases

<<<<<<< HEAD
bool OutputStreamManager::HasTrack(TrackID aTrackID) {
  MOZ_ASSERT(NS_IsMainThread());
||||||| merged common ancestors
  OutputStreamData* p = mStreams.AppendElement();
  p->Init(this, aStream, aNextAvailableTrackID);
=======
  TrackID id = mNextTrackID++;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return mLiveTracks.Contains(aTrackID, TrackIDComparator());
}

bool OutputStreamManager::HasTracks(TrackID aAudioTrack, TrackID aVideoTrack) {
  MOZ_ASSERT(NS_IsMainThread());

  size_t nrExpectedTracks = 0;
  bool asExpected = true;
  if (IsTrackIDExplicit(aAudioTrack)) {
    Unused << ++nrExpectedTracks;
    asExpected = asExpected && mLiveTracks.Contains(
                                   MakePair(aAudioTrack, MediaSegment::AUDIO),
                                   TrackTypeComparator());
||||||| merged common ancestors
  // Connect to the input stream if we have one. Otherwise the output stream
  // will be connected in Connect().
  if (mInputStream) {
    p->Connect(mInputStream, mInputAudioTrackID, mInputVideoTrackID);
=======
  LOG(LogLevel::Info, "Adding %s track with id %d",
      aType == MediaSegment::AUDIO ? "audio" : "video", id);

  mLiveTracks.AppendElement(MakePair(id, aType));
  AutoRemoveDestroyedStreams();
  for (const auto& data : mStreams) {
    data->AddTrack(id, aType, mPrincipal, mCORSMode, true);
>>>>>>> upstream-releases
  }
  if (IsTrackIDExplicit(aVideoTrack)) {
    Unused << ++nrExpectedTracks;
    asExpected = asExpected && mLiveTracks.Contains(
                                   MakePair(aVideoTrack, MediaSegment::VIDEO),
                                   TrackTypeComparator());
  }
  asExpected = asExpected && mLiveTracks.Length() == nrExpectedTracks;
  return asExpected;
}

<<<<<<< HEAD
size_t OutputStreamManager::NumberOfTracks() {
||||||| merged common ancestors
void
OutputStreamManager::Remove(MediaStream* aStream)
{
=======
void OutputStreamManager::AutoRemoveDestroyedStreams() {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
<<<<<<< HEAD
  return mLiveTracks.Length();
}

void OutputStreamManager::AddTrack(TrackID aTrackID, MediaSegment::Type aType) {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(!mSourceStream->IsDestroyed());
  MOZ_ASSERT(!HasTrack(aTrackID));

  LOG(LogLevel::Info, "Adding %s track with id %d",
      aType == MediaSegment::AUDIO ? "audio" : "video", aTrackID);

  mLiveTracks.AppendElement(MakePair(aTrackID, aType));
  for (const auto& data : mStreams) {
    data->AddTrack(aTrackID, aType, mPrincipal, mCORSMode, true);
||||||| merged common ancestors
  for (int32_t i = mStreams.Length() - 1; i >= 0; --i) {
    if (mStreams[i].Equals(aStream)) {
      mStreams.RemoveElementAt(i);
      break;
    }
=======
  for (size_t i = mStreams.Length(); i > 0; --i) {
    const auto& data = mStreams[i - 1];
    if (!data->mDOMStream || !data->mDOMStream->GetInputStream()) {
      // If the mDOMStream WeakPtr is now null, mDOMStream has been destructed.
      // If mDOMStream's input stream is now null, it has been unlinked but not
      // yet destructed. In both cases we can consider it dead.
      mStreams.RemoveElementAt(i - 1);
    }
>>>>>>> upstream-releases
  }
}

void OutputStreamManager::RemoveTrack(TrackID aTrackID) {
  MOZ_ASSERT(NS_IsMainThread());
<<<<<<< HEAD
  MOZ_ASSERT(!mSourceStream->IsDestroyed());
  LOG(LogLevel::Info, "Removing track with id %d", aTrackID);
  DebugOnly<bool> rv = mLiveTracks.RemoveElement(aTrackID, TrackIDComparator());
  MOZ_ASSERT(rv);
  for (const auto& data : mStreams) {
    data->RemoveTrack(aTrackID);
  }
||||||| merged common ancestors
  mStreams.Clear();
=======
  MOZ_ASSERT(!mSourceStream->IsDestroyed());
  LOG(LogLevel::Info, "Removing track with id %d", aTrackID);
  DebugOnly<bool> rv = mLiveTracks.RemoveElement(aTrackID, TrackIDComparator());
  MOZ_ASSERT(rv);
  AutoRemoveDestroyedStreams();
  for (const auto& data : mStreams) {
    data->RemoveTrack(aTrackID);
  }
>>>>>>> upstream-releases
}

void OutputStreamManager::RemoveTracks() {
  MOZ_ASSERT(NS_IsMainThread());
<<<<<<< HEAD
  MOZ_ASSERT(!mSourceStream->IsDestroyed());
  for (const Pair<TrackID, MediaSegment::Type>& pair : mLiveTracks) {
    for (const auto& data : mStreams) {
      data->RemoveTrack(pair.first());
    }
||||||| merged common ancestors
  for (const OutputStreamData& out : mStreams) {
    if (out.Equals(aOutputStream)) {
      return out.NextAvailableTrackID();
    }
=======
  nsTArray<Pair<TrackID, MediaSegment::Type>> liveTracks(mLiveTracks);
  for (const auto& pair : liveTracks) {
    RemoveTrack(pair.first());
>>>>>>> upstream-releases
  }
<<<<<<< HEAD
  mLiveTracks.Clear();
||||||| merged common ancestors
  return TRACK_INVALID;
=======
>>>>>>> upstream-releases
}

void OutputStreamManager::Disconnect() {
  MOZ_ASSERT(NS_IsMainThread());
<<<<<<< HEAD
  nsTArray<Pair<TrackID, MediaSegment::Type>> liveTracks(mLiveTracks);
  for (const auto& pair : liveTracks) {
    RemoveTrack(pair.first());
  }
  MOZ_ASSERT(mLiveTracks.IsEmpty());
  nsTArray<RefPtr<DOMMediaStream>> domStreams(mStreams.Length());
  for (const auto& data : mStreams) {
    domStreams.AppendElement(data->mDOMStream);
  }
  for (auto& domStream : domStreams) {
    Remove(domStream);
  }
  MOZ_ASSERT(mStreams.IsEmpty());
  if (!mSourceStream->IsDestroyed()) {
    mSourceStream->Destroy();
||||||| merged common ancestors
  mInputStream = aStream;
  mInputAudioTrackID = aAudioTrackID;
  mInputVideoTrackID = aVideoTrackID;
  for (int32_t i = mStreams.Length() - 1; i >= 0; --i) {
    if (!mStreams[i].Connect(aStream, mInputAudioTrackID, mInputVideoTrackID)) {
      // Probably the DOMMediaStream was GCed. Clean up.
      mStreams.RemoveElementAt(i);
    }
=======
  RemoveTracks();
  MOZ_ASSERT(mLiveTracks.IsEmpty());
  AutoRemoveDestroyedStreams();
  nsTArray<RefPtr<DOMMediaStream>> domStreams(mStreams.Length());
  for (const auto& data : mStreams) {
    domStreams.AppendElement(data->mDOMStream);
  }
  for (auto& domStream : domStreams) {
    Remove(domStream);
  }
  MOZ_ASSERT(mStreams.IsEmpty());
  if (!mSourceStream->IsDestroyed()) {
    mSourceStream->Destroy();
  }
}

AbstractCanonical<PrincipalHandle>*
OutputStreamManager::CanonicalPrincipalHandle() {
  return &mPrincipalHandle;
}

void OutputStreamManager::SetPrincipal(nsIPrincipal* aPrincipal) {
  MOZ_ASSERT(NS_IsMainThread());
  nsCOMPtr<nsIPrincipal> principal = mPrincipal;
  if (nsContentUtils::CombineResourcePrincipals(&principal, aPrincipal)) {
    mPrincipal = principal;
    AutoRemoveDestroyedStreams();
    for (const UniquePtr<OutputStreamData>& data : mStreams) {
      data->SetPrincipal(mPrincipal);
    }
    mPrincipalHandle = MakePrincipalHandle(principal);
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
AbstractCanonical<PrincipalHandle>*
OutputStreamManager::CanonicalPrincipalHandle() {
  return &mPrincipalHandle;
}

void OutputStreamManager::SetPrincipal(nsIPrincipal* aPrincipal) {
||||||| merged common ancestors
void
OutputStreamManager::Disconnect()
{
=======
TrackID OutputStreamManager::NextTrackID() const {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
<<<<<<< HEAD
  nsCOMPtr<nsIPrincipal> principal = mPrincipal;
  if (nsContentUtils::CombineResourcePrincipals(&principal, aPrincipal)) {
    mPrincipal = principal;
    for (const UniquePtr<OutputStreamData>& data : mStreams) {
      data->SetPrincipal(mPrincipal);
||||||| merged common ancestors
  mInputStream = nullptr;
  mInputAudioTrackID = TRACK_INVALID;
  mInputVideoTrackID = TRACK_INVALID;
  for (int32_t i = mStreams.Length() - 1; i >= 0; --i) {
    if (!mStreams[i].Disconnect()) {
      // Probably the DOMMediaStream was GCed. Clean up.
      mStreams.RemoveElementAt(i);
=======
  return mNextTrackID;
}

TrackID OutputStreamManager::GetLiveTrackIDFor(MediaSegment::Type aType) const {
  MOZ_ASSERT(NS_IsMainThread());
  for (const auto& pair : mLiveTracks) {
    if (pair.second() == aType) {
      return pair.first();
>>>>>>> upstream-releases
    }
    mPrincipalHandle = MakePrincipalHandle(principal);
  }
}

TrackID OutputStreamManager::NextTrackID() const {
  MOZ_ASSERT(NS_IsMainThread());
  return mNextTrackID;
}

TrackID OutputStreamManager::AllocateNextTrackID() {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_RELEASE_ASSERT(IsTrackIDExplicit(mNextTrackID));
  return mNextTrackID++;
}

void OutputStreamManager::SetPlaying(bool aPlaying) {
  MOZ_ASSERT(NS_IsMainThread());
  if (mPlaying == aPlaying) {
    return;
  }

  mPlaying = aPlaying;
  if (mPlaying) {
    mSourceStream->Resume();
  } else {
    mSourceStream->Suspend();
  }
  return TRACK_NONE;
}

void OutputStreamManager::SetPlaying(bool aPlaying) {
  MOZ_ASSERT(NS_IsMainThread());
  if (mPlaying == aPlaying) {
    return;
  }

  mPlaying = aPlaying;
  if (mPlaying) {
    mSourceStream->Resume();
  } else {
    mSourceStream->Suspend();
  }
}

#undef LOG

}  // namespace mozilla
