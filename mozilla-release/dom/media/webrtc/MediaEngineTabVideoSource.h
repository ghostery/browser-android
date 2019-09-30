/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MediaEngine.h"
#include "ImageContainer.h"
#include "nsITimer.h"
#include "mozilla/Mutex.h"
#include "mozilla/UniquePtr.h"
#include "nsITabSource.h"

namespace mozilla {

<<<<<<< HEAD
class MediaEngineTabVideoSource : public MediaEngineSource {
 public:
||||||| merged common ancestors
class MediaEngineTabVideoSource : public MediaEngineSource
{
public:
=======
namespace layers {
class ImageContainer;
}

class MediaEngineTabVideoSource : public MediaEngineSource {
 public:
>>>>>>> upstream-releases
  MediaEngineTabVideoSource();

  nsString GetName() const override;
  nsCString GetUUID() const override;
  nsString GetGroupId() const override;

  bool GetScary() const override { return true; }

  dom::MediaSourceEnum GetMediaSource() const override {
    return dom::MediaSourceEnum::Browser;
  }

  nsresult Allocate(const dom::MediaTrackConstraints& aConstraints,
                    const MediaEnginePrefs& aPrefs, const nsString& aDeviceId,
                    const ipc::PrincipalInfo& aPrincipalInfo,
                    const char** aOutBadConstraint) override;
<<<<<<< HEAD
  nsresult Deallocate(const RefPtr<const AllocationHandle>& aHandle) override;
  void SetTrack(const RefPtr<const AllocationHandle>& aHandle,
                const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
                const PrincipalHandle& aPrincipal) override;
  nsresult Start(const RefPtr<const AllocationHandle>& aHandle) override;
  nsresult Reconfigure(const RefPtr<AllocationHandle>& aHandle,
                       const dom::MediaTrackConstraints& aConstraints,
||||||| merged common ancestors
  nsresult Deallocate(const RefPtr<const AllocationHandle>& aHandle) override;
  nsresult SetTrack(const RefPtr<const AllocationHandle>& aHandle,
                    const RefPtr<SourceMediaStream>& aStream,
                    TrackID aTrackID,
                    const PrincipalHandle& aPrincipal) override;
  nsresult Start(const RefPtr<const AllocationHandle>& aHandle) override;
  nsresult Reconfigure(const RefPtr<AllocationHandle>& aHandle,
                       const dom::MediaTrackConstraints& aConstraints,
=======
  nsresult Deallocate() override;
  void SetTrack(const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
                const PrincipalHandle& aPrincipal) override;
  nsresult Start() override;
  nsresult Reconfigure(const dom::MediaTrackConstraints& aConstraints,
>>>>>>> upstream-releases
                       const MediaEnginePrefs& aPrefs,
                       const nsString& aDeviceId,
                       const char** aOutBadConstraint) override;
<<<<<<< HEAD
  nsresult FocusOnSelectedSource(
      const RefPtr<const AllocationHandle>& aHandle) override;
  nsresult Stop(const RefPtr<const AllocationHandle>& aHandle) override;

  void Pull(const RefPtr<const AllocationHandle>& aHandle,
            const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
            StreamTime aEndOfAppendedData, StreamTime aDesiredTime,
            const PrincipalHandle& aPrincipalHandle) override;
||||||| merged common ancestors
  nsresult FocusOnSelectedSource(const RefPtr<const AllocationHandle>& aHandle) override;
  nsresult Stop(const RefPtr<const AllocationHandle>& aHandle) override;

  void Pull(const RefPtr<const AllocationHandle>& aHandle,
            const RefPtr<SourceMediaStream>& aStream,
            TrackID aTrackID,
            StreamTime aDesiredTime,
            const PrincipalHandle& aPrincipalHandle) override;
=======
  nsresult FocusOnSelectedSource() override;
  nsresult Stop() override;
>>>>>>> upstream-releases

  uint32_t GetBestFitnessDistance(
      const nsTArray<const NormalizedConstraintSet*>& aConstraintSets,
      const nsString& aDeviceId) const override {
    return 0;
  }

  void GetSettings(dom::MediaTrackSettings& aOutSettings) const override;

  void Draw();

  class StartRunnable : public Runnable {
<<<<<<< HEAD
   public:
    explicit StartRunnable(MediaEngineTabVideoSource* videoSource)
        : Runnable("MediaEngineTabVideoSource::StartRunnable"),
          mVideoSource(videoSource) {}
||||||| merged common ancestors
  public:
    explicit StartRunnable(MediaEngineTabVideoSource *videoSource)
      : Runnable("MediaEngineTabVideoSource::StartRunnable")
      , mVideoSource(videoSource)
    {}
=======
   public:
    StartRunnable(MediaEngineTabVideoSource* videoSource,
                  SourceMediaStream* aStream, TrackID aTrackID,
                  const PrincipalHandle& aPrincipal)
        : Runnable("MediaEngineTabVideoSource::StartRunnable"),
          mVideoSource(videoSource),
          mStream(aStream),
          mTrackID(aTrackID),
          mPrincipal(aPrincipal) {}
>>>>>>> upstream-releases
    NS_IMETHOD Run() override;
    const RefPtr<MediaEngineTabVideoSource> mVideoSource;
    const RefPtr<SourceMediaStream> mStream;
    const TrackID mTrackID;
    const PrincipalHandle mPrincipal;
  };

  class StopRunnable : public Runnable {
   public:
    explicit StopRunnable(MediaEngineTabVideoSource* videoSource)
        : Runnable("MediaEngineTabVideoSource::StopRunnable"),
          mVideoSource(videoSource) {}
    NS_IMETHOD Run() override;
    const RefPtr<MediaEngineTabVideoSource> mVideoSource;
  };

  class InitRunnable : public Runnable {
<<<<<<< HEAD
   public:
    explicit InitRunnable(MediaEngineTabVideoSource* videoSource)
        : Runnable("MediaEngineTabVideoSource::InitRunnable"),
          mVideoSource(videoSource) {}
||||||| merged common ancestors
  public:
    explicit InitRunnable(MediaEngineTabVideoSource *videoSource)
      : Runnable("MediaEngineTabVideoSource::InitRunnable")
      , mVideoSource(videoSource)
    {}
=======
   public:
    InitRunnable(MediaEngineTabVideoSource* videoSource,
                 SourceMediaStream* aStream, TrackID aTrackID,
                 const PrincipalHandle& aPrincipal)
        : Runnable("MediaEngineTabVideoSource::InitRunnable"),
          mVideoSource(videoSource),
          mStream(aStream),
          mTrackID(aTrackID),
          mPrincipal(aPrincipal) {}
>>>>>>> upstream-releases
    NS_IMETHOD Run() override;
    const RefPtr<MediaEngineTabVideoSource> mVideoSource;
    const RefPtr<SourceMediaStream> mStream;
    const TrackID mTrackID;
    const PrincipalHandle mPrincipal;
  };

  class DestroyRunnable : public Runnable {
   public:
    explicit DestroyRunnable(MediaEngineTabVideoSource* videoSource)
        : Runnable("MediaEngineTabVideoSource::DestroyRunnable"),
          mVideoSource(videoSource) {}
    NS_IMETHOD Run() override;
    const RefPtr<MediaEngineTabVideoSource> mVideoSource;
  };

<<<<<<< HEAD
 protected:
  ~MediaEngineTabVideoSource() {}
||||||| merged common ancestors
protected:
  ~MediaEngineTabVideoSource() {}
=======
 protected:
  ~MediaEngineTabVideoSource() = default;
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
||||||| merged common ancestors
private:
=======
 private:
  // These are accessed only on main thread.
>>>>>>> upstream-releases
  int32_t mBufWidthMax = 0;
  int32_t mBufHeightMax = 0;
  int64_t mWindowId = 0;
  bool mScrollWithPage = false;
  int32_t mViewportOffsetX = 0;
  int32_t mViewportOffsetY = 0;
  int32_t mViewportWidth = 0;
  int32_t mViewportHeight = 0;
  int32_t mTimePerFrame = 0;
  RefPtr<layers::ImageContainer> mImageContainer;
  // The current settings of this source.
  // Members are main thread only.
  const RefPtr<media::Refcountable<dom::MediaTrackSettings>> mSettings;

  nsCOMPtr<nsPIDOMWindowOuter> mWindow;
  nsCOMPtr<nsITimer> mTimer;
  nsCOMPtr<nsITabSource> mTabSource;
  RefPtr<SourceMediaStream> mStreamMain;
  TrackID mTrackIDMain = TRACK_NONE;
  PrincipalHandle mPrincipalHandleMain = PRINCIPAL_HANDLE_NONE;
  // If this is set, we will run despite mWindow == nullptr.
  bool mBlackedoutWindow = false;

  // Current state of this source. Accessed on owning thread only.
  MediaEngineSourceState mState = kReleased;
  // mStream and mTrackID are set in SetTrack() to keep track of what to end
  // in Deallocate(). Owning thread only.
  RefPtr<SourceMediaStream> mStream;
  TrackID mTrackID = TRACK_NONE;
  PrincipalHandle mPrincipalHandle = PRINCIPAL_HANDLE_NONE;
};

}  // namespace mozilla
