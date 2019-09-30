/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MEDIAENGINEDEFAULT_H_
#define MEDIAENGINEDEFAULT_H_

#include "nsINamed.h"
#include "nsITimer.h"

#include "nsAutoPtr.h"
#include "nsCOMPtr.h"
#include "DOMMediaStream.h"
#include "nsComponentManagerUtils.h"
#include "mozilla/Mutex.h"

#include "VideoUtils.h"
#include "MediaEngine.h"
#include "MediaEnginePrefs.h"
#include "VideoSegment.h"
#include "AudioSegment.h"
#include "StreamTracks.h"
#include "MediaEngineSource.h"
#include "MediaStreamGraph.h"

namespace mozilla {

namespace layers {
class ImageContainer;
}  // namespace layers

class MediaEngineDefault;

/**
 * The default implementation of the MediaEngine interface.
 */
class MediaEngineDefaultVideoSource : public MediaEngineSource {
 public:
  MediaEngineDefaultVideoSource();

  nsString GetName() const override;
  nsCString GetUUID() const override;
  nsString GetGroupId() const override;

  nsresult Allocate(const dom::MediaTrackConstraints& aConstraints,
                    const MediaEnginePrefs& aPrefs, const nsString& aDeviceId,
                    const ipc::PrincipalInfo& aPrincipalInfo,
                    const char** aOutBadConstraint) override;
<<<<<<< HEAD
  void SetTrack(const RefPtr<const AllocationHandle>& aHandle,
                const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
                const PrincipalHandle& aPrincipal) override;
  nsresult Start(const RefPtr<const AllocationHandle>& aHandle) override;
  nsresult Reconfigure(const RefPtr<AllocationHandle>& aHandle,
                       const dom::MediaTrackConstraints& aConstraints,
||||||| merged common ancestors
  nsresult SetTrack(const RefPtr<const AllocationHandle>& aHandle,
                    const RefPtr<SourceMediaStream>& aStream,
                    TrackID aTrackID,
                    const PrincipalHandle& aPrincipal) override;
  nsresult Start(const RefPtr<const AllocationHandle>& aHandle) override;
  nsresult Reconfigure(const RefPtr<AllocationHandle>& aHandle,
                       const dom::MediaTrackConstraints& aConstraints,
=======
  void SetTrack(const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
                const PrincipalHandle& aPrincipal) override;
  nsresult Start() override;
  nsresult Reconfigure(const dom::MediaTrackConstraints& aConstraints,
>>>>>>> upstream-releases
                       const MediaEnginePrefs& aPrefs,
                       const nsString& aDeviceId,
                       const char** aOutBadConstraint) override;
<<<<<<< HEAD
  nsresult Stop(const RefPtr<const AllocationHandle>& aHandle) override;
  nsresult Deallocate(const RefPtr<const AllocationHandle>& aHandle) override;
  void Pull(const RefPtr<const AllocationHandle>& aHandle,
            const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
            StreamTime aEndOfAppendedData, StreamTime aDesiredTime,
            const PrincipalHandle& aPrincipalHandle) override;
||||||| merged common ancestors
  nsresult Stop(const RefPtr<const AllocationHandle>& aHandle) override;
  nsresult Deallocate(const RefPtr<const AllocationHandle>& aHandle) override;
  void Pull(const RefPtr<const AllocationHandle>& aHandle,
            const RefPtr<SourceMediaStream>& aStream,
            TrackID aTrackID,
            StreamTime aDesiredTime,
            const PrincipalHandle& aPrincipalHandle) override;
=======
  nsresult Stop() override;
  nsresult Deallocate() override;
>>>>>>> upstream-releases

  uint32_t GetBestFitnessDistance(
      const nsTArray<const NormalizedConstraintSet*>& aConstraintSets,
      const nsString& aDeviceId) const override;
  void GetSettings(dom::MediaTrackSettings& aOutSettings) const override;

  bool IsFake() const override { return true; }

  dom::MediaSourceEnum GetMediaSource() const override {
    return dom::MediaSourceEnum::Camera;
  }

 protected:
  ~MediaEngineDefaultVideoSource();

  /**
   * Called by mTimer when it's time to generate a new frame.
   */
  void GenerateFrame();

  nsCOMPtr<nsITimer> mTimer;

  RefPtr<layers::ImageContainer> mImageContainer;

  // Current state of this source.
  MediaEngineSourceState mState = kReleased;
  RefPtr<layers::Image> mImage;
  RefPtr<SourceMediaStream> mStream;
  TrackID mTrackID = TRACK_NONE;
  PrincipalHandle mPrincipalHandle = PRINCIPAL_HANDLE_NONE;

  MediaEnginePrefs mOpts;
  int mCb = 16;
  int mCr = 16;

  // Main thread only.
  const RefPtr<media::Refcountable<dom::MediaTrackSettings>> mSettings;

 private:
  const nsString mName;
};

class AudioSourcePullListener;

class MediaEngineDefaultAudioSource : public MediaEngineSource {
 public:
  MediaEngineDefaultAudioSource();

  nsString GetName() const override;
  nsCString GetUUID() const override;
  nsString GetGroupId() const override;

  nsresult Allocate(const dom::MediaTrackConstraints& aConstraints,
                    const MediaEnginePrefs& aPrefs, const nsString& aDeviceId,
                    const ipc::PrincipalInfo& aPrincipalInfo,
                    const char** aOutBadConstraint) override;
<<<<<<< HEAD
  void SetTrack(const RefPtr<const AllocationHandle>& aHandle,
                const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
                const PrincipalHandle& aPrincipal) override;
  nsresult Start(const RefPtr<const AllocationHandle>& aHandle) override;
  nsresult Reconfigure(const RefPtr<AllocationHandle>& aHandle,
                       const dom::MediaTrackConstraints& aConstraints,
||||||| merged common ancestors
  nsresult SetTrack(const RefPtr<const AllocationHandle>& aHandle,
                    const RefPtr<SourceMediaStream>& aStream,
                    TrackID aTrackID,
                    const PrincipalHandle& aPrincipal) override;
  nsresult Start(const RefPtr<const AllocationHandle>& aHandle) override;
  nsresult Reconfigure(const RefPtr<AllocationHandle>& aHandle,
                       const dom::MediaTrackConstraints& aConstraints,
=======
  void SetTrack(const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
                const PrincipalHandle& aPrincipal) override;
  nsresult Start() override;
  nsresult Reconfigure(const dom::MediaTrackConstraints& aConstraints,
>>>>>>> upstream-releases
                       const MediaEnginePrefs& aPrefs,
                       const nsString& aDeviceId,
                       const char** aOutBadConstraint) override;
<<<<<<< HEAD
  nsresult Stop(const RefPtr<const AllocationHandle>& aHandle) override;
  nsresult Deallocate(const RefPtr<const AllocationHandle>& aHandle) override;
  void inline AppendToSegment(AudioSegment& aSegment, TrackTicks aSamples,
                              const PrincipalHandle& aPrincipalHandle);
  void Pull(const RefPtr<const AllocationHandle>& aHandle,
            const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
            StreamTime aEndOfAppendedData, StreamTime aDesiredTime,
            const PrincipalHandle& aPrincipalHandle) override;

  bool IsFake() const override { return true; }
||||||| merged common ancestors
  nsresult Stop(const RefPtr<const AllocationHandle>& aHandle) override;
  nsresult Deallocate(const RefPtr<const AllocationHandle>& aHandle) override;
  void inline AppendToSegment(AudioSegment& aSegment,
                              TrackTicks aSamples,
                              const PrincipalHandle& aPrincipalHandle);
  void Pull(const RefPtr<const AllocationHandle>& aHandle,
            const RefPtr<SourceMediaStream>& aStream,
            TrackID aTrackID,
            StreamTime aDesiredTime,
            const PrincipalHandle& aPrincipalHandle) override;

  bool IsFake() const override
  {
    return true;
  }
=======
  nsresult Stop() override;
  nsresult Deallocate() override;

  bool IsFake() const override { return true; }
>>>>>>> upstream-releases

  dom::MediaSourceEnum GetMediaSource() const override {
    return dom::MediaSourceEnum::Microphone;
  }

  uint32_t GetBestFitnessDistance(
      const nsTArray<const NormalizedConstraintSet*>& aConstraintSets,
      const nsString& aDeviceId) const override;
  void GetSettings(dom::MediaTrackSettings& aOutSettings) const override;

<<<<<<< HEAD
  bool IsAvailable() const;

 protected:
||||||| merged common ancestors
  bool IsAvailable() const;

protected:
=======
 protected:
>>>>>>> upstream-releases
  ~MediaEngineDefaultAudioSource();

  // Current state of this source.
  MediaEngineSourceState mState = kReleased;
  RefPtr<SourceMediaStream> mStream;
  TrackID mTrackID = TRACK_NONE;
<<<<<<< HEAD

  // Accessed in Pull (from MSG thread)
  TrackTicks mLastNotify = 0;
  uint32_t mFreq = 1000;  // ditto

  // Created on Start, then accessed from Pull (MSG thread)
  nsAutoPtr<SineWaveGenerator> mSineGenerator;
||||||| merged common ancestors

  // Accessed in Pull (from MSG thread)
  TrackTicks mLastNotify = 0;
  uint32_t mFreq = 1000; // ditto

  // Created on Start, then accessed from Pull (MSG thread)
  nsAutoPtr<SineWaveGenerator> mSineGenerator;
=======
  PrincipalHandle mPrincipalHandle = PRINCIPAL_HANDLE_NONE;
  uint32_t mFrequency = 1000;
  RefPtr<AudioSourcePullListener> mPullListener;
>>>>>>> upstream-releases
};

class MediaEngineDefault : public MediaEngine {
 public:
  MediaEngineDefault() = default;

  void EnumerateDevices(uint64_t aWindowId, dom::MediaSourceEnum, MediaSinkEnum,
                        nsTArray<RefPtr<MediaDevice>>*) override;
  void Shutdown() override {}

 private:
  ~MediaEngineDefault() = default;
<<<<<<< HEAD

  // WindowID -> Array of devices.
  nsClassHashtable<nsUint64HashKey, nsTArray<RefPtr<MediaEngineSource>>>
      mVSources;
  nsClassHashtable<nsUint64HashKey,
                   nsTArray<RefPtr<MediaEngineDefaultAudioSource>>>
      mASources;
||||||| merged common ancestors

  // WindowID -> Array of devices.
  nsClassHashtable<nsUint64HashKey,
                   nsTArray<RefPtr<MediaEngineSource>>> mVSources;
  nsClassHashtable<nsUint64HashKey,
                   nsTArray<RefPtr<MediaEngineDefaultAudioSource>>> mASources;
=======
>>>>>>> upstream-releases
};

}  // namespace mozilla

#endif /* NSMEDIAENGINEDEFAULT_H_ */
