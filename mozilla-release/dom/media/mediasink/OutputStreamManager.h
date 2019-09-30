/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef OutputStreamManager_h
#define OutputStreamManager_h

#include "mozilla/CORSMode.h"
#include "mozilla/RefPtr.h"
#include "mozilla/StateMirroring.h"
#include "nsTArray.h"
#include "TrackID.h"

namespace mozilla {

class DOMMediaStream;
class MediaInputPort;
class MediaStream;
class OutputStreamManager;
class ProcessedMediaStream;
class SourceMediaStream;

namespace dom {
class MediaStreamTrack;
}

class OutputStreamData {
 public:
  OutputStreamData(OutputStreamManager* aManager,
                   AbstractThread* aAbstractMainThread,
                   DOMMediaStream* aDOMStream);
  OutputStreamData(const OutputStreamData& aOther) = delete;
  OutputStreamData(OutputStreamData&& aOther) = delete;
  ~OutputStreamData();
<<<<<<< HEAD

  // Creates and adds a MediaStreamTrack to mDOMStream so that we can feed data
  // to it. For a true aAsyncAddTrack we will dispatch a task to add the
  // created track to mDOMStream, as is required by spec for the "addtrack"
  // event.
  void AddTrack(TrackID aTrackID, MediaSegment::Type aType,
                nsIPrincipal* aPrincipal, CORSMode aCORSMode,
                bool aAsyncAddTrack);
  // Ends the MediaStreamTrack with aTrackID. Calling this with a TrackID that
  // doesn't exist in mDOMStream is an error.
  void RemoveTrack(TrackID aTrackID);

  void SetPrincipal(nsIPrincipal* aPrincipal);

  // The source stream DecodedStream is feeding tracks to.
  const RefPtr<OutputStreamManager> mManager;
  const RefPtr<AbstractThread> mAbstractMainThread;
  // The DOMMediaStream we add tracks to and represent.
  const RefPtr<DOMMediaStream> mDOMStream;
  // The input stream of mDOMStream.
  const RefPtr<ProcessedMediaStream> mInputStream;

 private:
  // mPort connects mSourceStream to mInputStream.
  const RefPtr<MediaInputPort> mPort;

  // Tracks that have been added and not yet removed.
  nsTArray<RefPtr<dom::MediaStreamTrack>> mTracks;
||||||| merged common ancestors
  void Init(OutputStreamManager* aOwner,
            ProcessedMediaStream* aStream,
            TrackID aNextAvailableTrackID);

  // Connect the given input stream's audio and video tracks to mStream.
  // Return false is mStream is already destroyed, otherwise true.
  bool Connect(MediaStream* aStream, TrackID aAudioTrackID, TrackID aVideoTrackID);
  // Disconnect mStream from its input stream.
  // Return false is mStream is already destroyed, otherwise true.
  bool Disconnect();
  // Return true if aStream points to the same object as mStream.
  // Used by OutputStreamManager to remove an output stream.
  bool Equals(MediaStream* aStream) const;
  // Return the graph mStream belongs to.
  MediaStreamGraph* Graph() const;
  // The next TrackID that will not cause a collision in mStream.
  TrackID NextAvailableTrackID() const;

private:
  OutputStreamManager* mOwner;
  RefPtr<ProcessedMediaStream> mStream;
  // mPort connects an input stream to our mStream.
  nsTArray<RefPtr<MediaInputPort>> mPorts;
  // For guaranteeing TrackID uniqueness in our mStream.
  TrackID mNextAvailableTrackID = TRACK_INVALID;
=======

  // Creates and adds a MediaStreamTrack to mDOMStream so that we can feed data
  // to it. For a true aAsyncAddTrack we will dispatch a task to add the
  // created track to mDOMStream, as is required by spec for the "addtrack"
  // event.
  void AddTrack(TrackID aTrackID, MediaSegment::Type aType,
                nsIPrincipal* aPrincipal, CORSMode aCORSMode,
                bool aAsyncAddTrack);
  // Ends the MediaStreamTrack with aTrackID. Calling this with a TrackID that
  // doesn't exist in mDOMStream is an error.
  void RemoveTrack(TrackID aTrackID);

  void SetPrincipal(nsIPrincipal* aPrincipal);

  // The source stream DecodedStream is feeding tracks to.
  const RefPtr<OutputStreamManager> mManager;
  const RefPtr<AbstractThread> mAbstractMainThread;
  // The DOMMediaStream we add tracks to and represent.
  const WeakPtr<DOMMediaStream> mDOMStream;
  // The input stream of mDOMStream.
  const RefPtr<ProcessedMediaStream> mInputStream;

 private:
  // mPort connects mSourceStream to mInputStream.
  const RefPtr<MediaInputPort> mPort;

  // Tracks that have been added and not yet removed.
  nsTArray<WeakPtr<dom::MediaStreamTrack>> mTracks;
>>>>>>> upstream-releases
};

class OutputStreamManager {
  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(OutputStreamManager);

 public:
  explicit OutputStreamManager(SourceMediaStream* aSourceStream,
                               TrackID aNextTrackID, nsIPrincipal* aPrincipal,
                               CORSMode aCORSMode,
                               AbstractThread* aAbstractMainThread);
  // Add the output stream to the collection.
  void Add(DOMMediaStream* aDOMStream);
  // Remove the output stream from the collection.
<<<<<<< HEAD
  void Remove(DOMMediaStream* aDOMStream);
  // Returns true if aTrackID has been added to all output streams.
  bool HasTrack(TrackID aTrackID);
  // Returns true if the given tracks and no others are currently live.
  // Use a non-explicit TrackID to make it ignored for that type.
  bool HasTracks(TrackID aAudioTrack, TrackID aVideoTrack);
  // Returns the number of live tracks.
  size_t NumberOfTracks();
  // Add aTrackID to all output streams.
  void AddTrack(TrackID aTrackID, MediaSegment::Type aType);
  // Remove aTrackID from all output streams.
  void RemoveTrack(TrackID aTrackID);
  // Remove all added tracks from all output streams.
  void RemoveTracks();
  // Disconnect mSourceStream from all output streams.
||||||| merged common ancestors
  void Remove(MediaStream* aStream);
  // Clear all output streams from the collection.
  void Clear();
  // The next TrackID that will not cause a collision in aOutputStream.
  TrackID NextAvailableTrackIDFor(MediaStream* aOutputStream) const;
  // Return true if the collection empty.
  bool IsEmpty() const
  {
    MOZ_ASSERT(NS_IsMainThread());
    return mStreams.IsEmpty();
  }
  // Connect the given input stream's tracks to all output streams.
  void Connect(MediaStream* aStream,
               TrackID aAudioTrackID,
               TrackID aVideoTrackID);
  // Disconnect the input stream to all output streams.
=======
  void Remove(DOMMediaStream* aDOMStream);
  // Returns true if there's a live track of the given type.
  bool HasTrackType(MediaSegment::Type aType);
  // Returns true if the given tracks and no others are currently live.
  // Use a non-explicit TrackID to make it ignored for that type.
  bool HasTracks(TrackID aAudioTrack, TrackID aVideoTrack);
  // Returns the number of live tracks.
  size_t NumberOfTracks();
  // Add a track to all output streams.
  void AddTrack(MediaSegment::Type aType);
  // Remove all currently live tracks from all output streams.
  void RemoveTracks();
  // Disconnect mSourceStream from all output streams.
>>>>>>> upstream-releases
  void Disconnect();
<<<<<<< HEAD
  // The principal handle for the underlying decoder.
  AbstractCanonical<PrincipalHandle>* CanonicalPrincipalHandle();
  // Called when the underlying decoder's principal has changed.
  void SetPrincipal(nsIPrincipal* aPrincipal);
  // The CORSMode for the media element owning the decoder.
  AbstractCanonical<CORSMode>* CanonicalCORSMode();
  // Called when the CORSMode for the media element owning the decoder has
  // changed.
  void SetCORSMode(CORSMode aCORSMode);
  // Returns the track id that would be used the next time a track is allocated.
  TrackID NextTrackID() const;
  // Like NextTrackID() but advances internal state, so the next call returns a
  // new unique TrackID.
  TrackID AllocateNextTrackID();
  // Called by DecodedStream when its playing state changes. While not playing
  // we suspend mSourceStream.
  void SetPlaying(bool aPlaying);
  // Return true if the collection of output streams is empty.
  bool IsEmpty() const {
||||||| merged common ancestors
  // Return the graph these streams belong to or null if empty.
  MediaStreamGraph* Graph() const
  {
=======
  // The principal handle for the underlying decoder.
  AbstractCanonical<PrincipalHandle>* CanonicalPrincipalHandle();
  // Called when the underlying decoder's principal has changed.
  void SetPrincipal(nsIPrincipal* aPrincipal);
  // The CORSMode for the media element owning the decoder.
  AbstractCanonical<CORSMode>* CanonicalCORSMode();
  // Called when the CORSMode for the media element owning the decoder has
  // changed.
  void SetCORSMode(CORSMode aCORSMode);
  // Returns the track id that would be used the next time a track is added.
  TrackID NextTrackID() const;
  // Returns the TrackID for the currently live track of the given type, or
  // TRACK_NONE otherwise.
  TrackID GetLiveTrackIDFor(MediaSegment::Type aType) const;
  // Called by DecodedStream when its playing state changes. While not playing
  // we suspend mSourceStream.
  void SetPlaying(bool aPlaying);
  // Return true if the collection of output streams is empty.
  bool IsEmpty() const {
>>>>>>> upstream-releases
    MOZ_ASSERT(NS_IsMainThread());
    return mStreams.IsEmpty();
  }

  // Keep the source stream so we can connect the output streams that
  // are added after Connect().
<<<<<<< HEAD
  const RefPtr<SourceMediaStream> mSourceStream;
  const RefPtr<AbstractThread> mAbstractMainThread;

 private:
  ~OutputStreamManager() = default;
  struct StreamComparator {
    static bool Equals(const UniquePtr<OutputStreamData>& aData,
                       DOMMediaStream* aStream) {
      return aData->mDOMStream == aStream;
    }
  };
  struct TrackIDComparator {
    static bool Equals(const Pair<TrackID, MediaSegment::Type>& aLiveTrack,
                       TrackID aTrackID) {
      return aLiveTrack.first() == aTrackID;
    }
  };
  struct TrackTypeComparator {
    static bool Equals(const Pair<TrackID, MediaSegment::Type>& aLiveTrack,
                       const Pair<TrackID, MediaSegment::Type>& aOther) {
      return aLiveTrack.first() == aOther.first() &&
             aLiveTrack.second() == aOther.second();
    }
  };
  nsTArray<UniquePtr<OutputStreamData>> mStreams;
  nsTArray<Pair<TrackID, MediaSegment::Type>> mLiveTracks;
  Canonical<PrincipalHandle> mPrincipalHandle;
  nsCOMPtr<nsIPrincipal> mPrincipal;
  const CORSMode mCORSMode;
  TrackID mNextTrackID;
  bool mPlaying;
||||||| merged common ancestors
  RefPtr<MediaStream> mInputStream;
  TrackID mInputAudioTrackID = TRACK_INVALID;
  TrackID mInputVideoTrackID = TRACK_INVALID;
  nsTArray<OutputStreamData> mStreams;
=======
  const RefPtr<SourceMediaStream> mSourceStream;
  const RefPtr<AbstractThread> mAbstractMainThread;

 private:
  ~OutputStreamManager() = default;
  struct StreamComparator {
    static bool Equals(const UniquePtr<OutputStreamData>& aData,
                       DOMMediaStream* aStream) {
      return aData->mDOMStream == aStream;
    }
  };
  struct TrackIDComparator {
    static bool Equals(const Pair<TrackID, MediaSegment::Type>& aLiveTrack,
                       TrackID aTrackID) {
      return aLiveTrack.first() == aTrackID;
    }
  };
  struct TrackTypeComparator {
    static bool Equals(const Pair<TrackID, MediaSegment::Type>& aLiveTrack,
                       MediaSegment::Type aType) {
      return aLiveTrack.second() == aType;
    }
  };
  struct TrackComparator {
    static bool Equals(const Pair<TrackID, MediaSegment::Type>& aLiveTrack,
                       const Pair<TrackID, MediaSegment::Type>& aOther) {
      return aLiveTrack.first() == aOther.first() &&
             aLiveTrack.second() == aOther.second();
    }
  };

  // Goes through mStreams and removes any entries that have been destroyed.
  void AutoRemoveDestroyedStreams();

  // Remove aTrackID from all output streams.
  void RemoveTrack(TrackID aTrackID);

  nsTArray<UniquePtr<OutputStreamData>> mStreams;
  nsTArray<Pair<TrackID, MediaSegment::Type>> mLiveTracks;
  Canonical<PrincipalHandle> mPrincipalHandle;
  nsCOMPtr<nsIPrincipal> mPrincipal;
  const CORSMode mCORSMode;
  TrackID mNextTrackID;
  bool mPlaying;
>>>>>>> upstream-releases
};

}  // namespace mozilla

#endif  // OutputStreamManager_h
