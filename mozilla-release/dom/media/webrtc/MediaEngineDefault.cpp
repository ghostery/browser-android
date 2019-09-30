/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MediaEngineDefault.h"

#include "ImageContainer.h"
#include "ImageTypes.h"
#include "Layers.h"
#include "MediaStreamGraph.h"
#include "MediaStreamListener.h"
#include "MediaTrackConstraints.h"
#include "mozilla/dom/File.h"
#include "mozilla/UniquePtr.h"
#include "nsCOMPtr.h"
#include "nsContentUtils.h"
#include "nsIFile.h"
#include "nsIFilePicker.h"
#include "nsIPrefBranch.h"
#include "nsIPrefService.h"
#include "SineWaveGenerator.h"
#include "Tracing.h"

#ifdef MOZ_WIDGET_ANDROID
#  include "nsISupportsUtils.h"
#endif

#ifdef MOZ_WEBRTC
#  include "YuvStamper.h"
#endif

#define DEFAULT_AUDIO_TIMER_MS 10
namespace mozilla {

using namespace mozilla::gfx;
using dom::MediaSourceEnum;
using dom::MediaTrackConstraints;
using dom::MediaTrackSettings;
using dom::VideoFacingModeEnum;

static nsString DefaultVideoName() {
  // For the purpose of testing we allow to change the name of the fake device
  // by pref.
  nsAutoString cameraNameFromPref;
  nsresult rv;
  // Here it is preferred a "hard" block, provided by the combination of Await &
  // InvokeAsync, instead of "soft" block, provided by sync dispatch which
  // allows the waiting thread to spin its event loop. The latter would allow
  // miltiple enumeration requests being processed out-of-order.
  media::Await(
      do_AddRef(SystemGroup::EventTargetFor(TaskCategory::Other)),
      InvokeAsync(
          SystemGroup::EventTargetFor(TaskCategory::Other), __func__, [&]() {
            rv = Preferences::GetString("media.getusermedia.fake-camera-name",
                                        cameraNameFromPref);
            return GenericPromise::CreateAndResolve(true, __func__);
          }));

  if (NS_SUCCEEDED(rv)) {
    return std::move(cameraNameFromPref);
  }
  return NS_LITERAL_STRING(u"Default Video Device");
}

/**
 * Default video source.
 */

MediaEngineDefaultVideoSource::MediaEngineDefaultVideoSource()
<<<<<<< HEAD
    : mTimer(nullptr), mMutex("MediaEngineDefaultVideoSource::mMutex") {}
||||||| merged common ancestors
  : mTimer(nullptr)
  , mMutex("MediaEngineDefaultVideoSource::mMutex")
{}
=======
    : mTimer(nullptr),
      mSettings(MakeAndAddRef<media::Refcountable<MediaTrackSettings>>()),
      mName(DefaultVideoName()) {
  mSettings->mWidth.Construct(
      int32_t(MediaEnginePrefs::DEFAULT_43_VIDEO_WIDTH));
  mSettings->mHeight.Construct(
      int32_t(MediaEnginePrefs::DEFAULT_43_VIDEO_HEIGHT));
  mSettings->mFrameRate.Construct(double(MediaEnginePrefs::DEFAULT_VIDEO_FPS));
  mSettings->mFacingMode.Construct(
      NS_ConvertASCIItoUTF16(dom::VideoFacingModeEnumValues::strings
                                 [uint8_t(VideoFacingModeEnum::Environment)]
                                     .value));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
MediaEngineDefaultVideoSource::~MediaEngineDefaultVideoSource() {}
||||||| merged common ancestors
MediaEngineDefaultVideoSource::~MediaEngineDefaultVideoSource()
{}
=======
MediaEngineDefaultVideoSource::~MediaEngineDefaultVideoSource() = default;
>>>>>>> upstream-releases

<<<<<<< HEAD
nsString MediaEngineDefaultVideoSource::GetName() const {
  return NS_LITERAL_STRING(u"Default Video Device");
}
||||||| merged common ancestors
nsString
MediaEngineDefaultVideoSource::GetName() const
{
  return NS_LITERAL_STRING(u"Default Video Device");
}
=======
nsString MediaEngineDefaultVideoSource::GetName() const { return mName; }
>>>>>>> upstream-releases

nsCString MediaEngineDefaultVideoSource::GetUUID() const {
  return NS_LITERAL_CSTRING("1041FCBD-3F12-4F7B-9E9B-1EC556DD5676");
}

<<<<<<< HEAD
uint32_t MediaEngineDefaultVideoSource::GetBestFitnessDistance(
||||||| merged common ancestors
uint32_t
MediaEngineDefaultVideoSource::GetBestFitnessDistance(
=======
nsString MediaEngineDefaultVideoSource::GetGroupId() const {
  return NS_LITERAL_STRING(u"Default Video Group");
}

uint32_t MediaEngineDefaultVideoSource::GetBestFitnessDistance(
>>>>>>> upstream-releases
    const nsTArray<const NormalizedConstraintSet*>& aConstraintSets,
    const nsString& aDeviceId) const {
  AssertIsOnOwningThread();

  uint32_t distance = 0;
#ifdef MOZ_WEBRTC
  for (const auto* cs : aConstraintSets) {
    distance =
        MediaConstraintsHelper::GetMinimumFitnessDistance(*cs, aDeviceId);
    break;  // distance is read from first entry only
  }
#endif
  return distance;
}

<<<<<<< HEAD
nsresult MediaEngineDefaultVideoSource::Allocate(
    const dom::MediaTrackConstraints& aConstraints,
    const MediaEnginePrefs& aPrefs, const nsString& aDeviceId,
    const mozilla::ipc::PrincipalInfo& aPrincipalInfo,
    AllocationHandle** aOutHandle, const char** aOutBadConstraint) {
||||||| merged common ancestors
nsresult
MediaEngineDefaultVideoSource::Allocate(const dom::MediaTrackConstraints &aConstraints,
                                        const MediaEnginePrefs &aPrefs,
                                        const nsString& aDeviceId,
                                        const mozilla::ipc::PrincipalInfo& aPrincipalInfo,
                                        AllocationHandle** aOutHandle,
                                        const char** aOutBadConstraint)
{
=======
void MediaEngineDefaultVideoSource::GetSettings(
    MediaTrackSettings& aOutSettings) const {
  MOZ_ASSERT(NS_IsMainThread());
  aOutSettings = *mSettings;
}

nsresult MediaEngineDefaultVideoSource::Allocate(
    const MediaTrackConstraints& aConstraints, const MediaEnginePrefs& aPrefs,
    const nsString& aDeviceId,
    const mozilla::ipc::PrincipalInfo& aPrincipalInfo,
    const char** aOutBadConstraint) {
>>>>>>> upstream-releases
  AssertIsOnOwningThread();

  MOZ_ASSERT(mState == kReleased);

  FlattenedConstraints c(aConstraints);

  // Mock failure for automated tests.
  if (c.mDeviceId.mIdeal.find(NS_LITERAL_STRING("bad device")) !=
      c.mDeviceId.mIdeal.end()) {
    return NS_ERROR_FAILURE;
  }

  // emulator debug is very, very slow; reduce load on it with smaller/slower
  // fake video
  mOpts = aPrefs;
  mOpts.mWidth =
      c.mWidth.Get(aPrefs.mWidth ? aPrefs.mWidth :
#ifdef DEBUG
                                 MediaEnginePrefs::DEFAULT_43_VIDEO_WIDTH / 2
#else
                                 MediaEnginePrefs::DEFAULT_43_VIDEO_WIDTH
#endif
      );
  mOpts.mHeight =
      c.mHeight.Get(aPrefs.mHeight ? aPrefs.mHeight :
#ifdef DEBUG
                                   MediaEnginePrefs::DEFAULT_43_VIDEO_HEIGHT / 2
#else
                                   MediaEnginePrefs::DEFAULT_43_VIDEO_HEIGHT
#endif
      );
  mOpts.mWidth = std::max(160, std::min(mOpts.mWidth, 4096)) & ~1;
  mOpts.mHeight = std::max(90, std::min(mOpts.mHeight, 2160)) & ~1;

  NS_DispatchToMainThread(NS_NewRunnableFunction(
      __func__, [settings = mSettings, frameRate = mOpts.mFPS,
                 width = mOpts.mWidth, height = mOpts.mHeight]() {
        settings->mFrameRate.Value() = frameRate;
        settings->mWidth.Value() = width;
        settings->mHeight.Value() = height;
      }));

  mState = kAllocated;
  return NS_OK;
}

<<<<<<< HEAD
nsresult MediaEngineDefaultVideoSource::Deallocate(
    const RefPtr<const AllocationHandle>& aHandle) {
||||||| merged common ancestors
nsresult
MediaEngineDefaultVideoSource::Deallocate(const RefPtr<const AllocationHandle>& aHandle)
{
=======
nsresult MediaEngineDefaultVideoSource::Deallocate() {
>>>>>>> upstream-releases
  AssertIsOnOwningThread();

  MOZ_ASSERT(!mImage);
  MOZ_ASSERT(mState == kStopped || mState == kAllocated);

  if (mStream && IsTrackIDExplicit(mTrackID)) {
    mStream->EndTrack(mTrackID);
    mStream = nullptr;
    mTrackID = TRACK_NONE;
    mPrincipalHandle = PRINCIPAL_HANDLE_NONE;
  }
  mState = kReleased;
  mImageContainer = nullptr;

  return NS_OK;
}

static void AllocateSolidColorFrame(layers::PlanarYCbCrData& aData, int aWidth,
                                    int aHeight, int aY, int aCb, int aCr) {
  MOZ_ASSERT(!(aWidth & 1));
  MOZ_ASSERT(!(aHeight & 1));
  // Allocate a single frame with a solid color
  int yLen = aWidth * aHeight;
  int cbLen = yLen >> 2;
  int crLen = cbLen;
  uint8_t* frame = (uint8_t*)malloc(yLen + cbLen + crLen);
  memset(frame, aY, yLen);
  memset(frame + yLen, aCb, cbLen);
  memset(frame + yLen + cbLen, aCr, crLen);

  aData.mYChannel = frame;
  aData.mYSize = IntSize(aWidth, aHeight);
  aData.mYStride = aWidth;
  aData.mCbCrStride = aWidth >> 1;
  aData.mCbChannel = frame + yLen;
  aData.mCrChannel = aData.mCbChannel + cbLen;
  aData.mCbCrSize = IntSize(aWidth >> 1, aHeight >> 1);
  aData.mPicX = 0;
  aData.mPicY = 0;
  aData.mPicSize = IntSize(aWidth, aHeight);
  aData.mStereoMode = StereoMode::MONO;
}

static void ReleaseFrame(layers::PlanarYCbCrData& aData) {
  free(aData.mYChannel);
}

<<<<<<< HEAD
void MediaEngineDefaultVideoSource::SetTrack(
    const RefPtr<const AllocationHandle>& aHandle,
    const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
    const PrincipalHandle& aPrincipal) {
||||||| merged common ancestors
nsresult
MediaEngineDefaultVideoSource::SetTrack(const RefPtr<const AllocationHandle>& aHandle,
                                        const RefPtr<SourceMediaStream>& aStream,
                                        TrackID aTrackID,
                                        const PrincipalHandle& aPrincipal)
{
=======
void MediaEngineDefaultVideoSource::SetTrack(
    const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
    const PrincipalHandle& aPrincipal) {
>>>>>>> upstream-releases
  AssertIsOnOwningThread();

  MOZ_ASSERT(mState == kAllocated);
  MOZ_ASSERT(!mStream);
  MOZ_ASSERT(mTrackID == TRACK_NONE);

<<<<<<< HEAD
  {
    MutexAutoLock lock(mMutex);
    mStream = aStream;
    mTrackID = aTrackID;
  }
  aStream->AddTrack(aTrackID, new VideoSegment(),
||||||| merged common ancestors
  {
    MutexAutoLock lock(mMutex);
    mStream = aStream;
    mTrackID = aTrackID;
  }
  aStream->AddTrack(aTrackID, 0, new VideoSegment(),
=======
  mStream = aStream;
  mTrackID = aTrackID;
  mPrincipalHandle = aPrincipal;
  aStream->AddTrack(aTrackID, new VideoSegment(),
>>>>>>> upstream-releases
                    SourceMediaStream::ADDTRACK_QUEUED);
}

<<<<<<< HEAD
nsresult MediaEngineDefaultVideoSource::Start(
    const RefPtr<const AllocationHandle>& aHandle) {
||||||| merged common ancestors
nsresult
MediaEngineDefaultVideoSource::Start(const RefPtr<const AllocationHandle>& aHandle)
{
=======
nsresult MediaEngineDefaultVideoSource::Start() {
>>>>>>> upstream-releases
  AssertIsOnOwningThread();

  MOZ_ASSERT(mState == kAllocated || mState == kStopped);
  MOZ_ASSERT(mStream, "SetTrack() must happen before Start()");
  MOZ_ASSERT(IsTrackIDExplicit(mTrackID),
             "SetTrack() must happen before Start()");

  mTimer = NS_NewTimer();
  if (!mTimer) {
    return NS_ERROR_FAILURE;
  }

  if (!mImageContainer) {
    mImageContainer = layers::LayerManager::CreateImageContainer(
        layers::ImageContainer::ASYNCHRONOUS);
  }

  // Start timer for subsequent frames
  uint32_t interval;
#if defined(MOZ_WIDGET_ANDROID) && defined(DEBUG)
  // emulator debug is very, very slow and has problems dealing with realtime
  // audio inputs
  interval = 10 * (1000 / mOpts.mFPS);
#else
  interval = 1000 / mOpts.mFPS;
#endif
<<<<<<< HEAD
  mTimer->InitWithNamedFuncCallback(
      [](nsITimer* aTimer, void* aClosure) {
        RefPtr<MediaEngineDefaultVideoSource> source =
            static_cast<MediaEngineDefaultVideoSource*>(aClosure);
        source->GenerateFrame();
      },
      this, interval, nsITimer::TYPE_REPEATING_SLACK,
      "MediaEngineDefaultVideoSource::GenerateFrame");

  MutexAutoLock lock(mMutex);
||||||| merged common ancestors
  mTimer->InitWithNamedFuncCallback([](nsITimer* aTimer, void* aClosure) {
      RefPtr<MediaEngineDefaultVideoSource> source =
        static_cast<MediaEngineDefaultVideoSource*>(aClosure);
      source->GenerateFrame();
    }, this, interval, nsITimer::TYPE_REPEATING_SLACK,
    "MediaEngineDefaultVideoSource::GenerateFrame");

  MutexAutoLock lock(mMutex);
=======
  mTimer->InitWithNamedFuncCallback(
      [](nsITimer* aTimer, void* aClosure) {
        RefPtr<MediaEngineDefaultVideoSource> source =
            static_cast<MediaEngineDefaultVideoSource*>(aClosure);
        source->GenerateFrame();
      },
      this, interval, nsITimer::TYPE_REPEATING_SLACK,
      "MediaEngineDefaultVideoSource::GenerateFrame");

>>>>>>> upstream-releases
  mState = kStarted;
  return NS_OK;
}

<<<<<<< HEAD
nsresult MediaEngineDefaultVideoSource::Stop(
    const RefPtr<const AllocationHandle>& aHandle) {
||||||| merged common ancestors
nsresult
MediaEngineDefaultVideoSource::Stop(const RefPtr<const AllocationHandle>& aHandle)
{
=======
nsresult MediaEngineDefaultVideoSource::Stop() {
>>>>>>> upstream-releases
  AssertIsOnOwningThread();

  if (mState == kStopped || mState == kAllocated) {
    return NS_OK;
  }

  MOZ_ASSERT(mState == kStarted);
  MOZ_ASSERT(mTimer);
  MOZ_ASSERT(mStream);
  MOZ_ASSERT(IsTrackIDExplicit(mTrackID));

  mTimer->Cancel();
  mTimer = nullptr;

  mState = kStopped;

  return NS_OK;
}

<<<<<<< HEAD
nsresult MediaEngineDefaultVideoSource::Reconfigure(
    const RefPtr<AllocationHandle>& aHandle,
    const dom::MediaTrackConstraints& aConstraints,
    const MediaEnginePrefs& aPrefs, const nsString& aDeviceId,
    const char** aOutBadConstraint) {
||||||| merged common ancestors
nsresult
MediaEngineDefaultVideoSource::Reconfigure(
    const RefPtr<AllocationHandle>& aHandle,
    const dom::MediaTrackConstraints& aConstraints,
    const MediaEnginePrefs &aPrefs,
    const nsString& aDeviceId,
    const char** aOutBadConstraint)
{
=======
nsresult MediaEngineDefaultVideoSource::Reconfigure(
    const MediaTrackConstraints& aConstraints, const MediaEnginePrefs& aPrefs,
    const nsString& aDeviceId, const char** aOutBadConstraint) {
>>>>>>> upstream-releases
  return NS_OK;
}

void MediaEngineDefaultVideoSource::GenerateFrame() {
  AssertIsOnOwningThread();

  // Update the target color
  if (mCr <= 16) {
    if (mCb < 240) {
      mCb++;
    } else {
      mCr++;
    }
  } else if (mCb >= 240) {
    if (mCr < 240) {
      mCr++;
    } else {
      mCb--;
    }
  } else if (mCr >= 240) {
    if (mCb > 16) {
      mCb--;
    } else {
      mCr--;
    }
  } else {
    mCr--;
  }

  // Allocate a single solid color image
  RefPtr<layers::PlanarYCbCrImage> ycbcr_image =
      mImageContainer->CreatePlanarYCbCrImage();
  layers::PlanarYCbCrData data;
  AllocateSolidColorFrame(data, mOpts.mWidth, mOpts.mHeight, 0x80, mCb, mCr);

#ifdef MOZ_WEBRTC
  uint64_t timestamp = PR_Now();
  YuvStamper::Encode(mOpts.mWidth, mOpts.mHeight, mOpts.mWidth, data.mYChannel,
                     reinterpret_cast<unsigned char*>(&timestamp),
                     sizeof(timestamp), 0, 0);
#endif

  bool setData = ycbcr_image->CopyData(data);
  MOZ_ASSERT(setData);

  // SetData copies data, so we can free the frame
  ReleaseFrame(data);

  if (!setData) {
    return;
  }

<<<<<<< HEAD
  MutexAutoLock lock(mMutex);
  mImage = std::move(ycbcr_image);
}

void MediaEngineDefaultVideoSource::Pull(
    const RefPtr<const AllocationHandle>& aHandle,
    const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
    StreamTime aEndOfAppendedData, StreamTime aDesiredTime,
    const PrincipalHandle& aPrincipalHandle) {
  TRACE_AUDIO_CALLBACK_COMMENT("SourceMediaStream %p track %i", aStream.get(),
                               aTrackID);
  // AppendFrame takes ownership of `segment`
||||||| merged common ancestors
  MutexAutoLock lock(mMutex);
  mImage = std::move(ycbcr_image);
}

void
MediaEngineDefaultVideoSource::Pull(const RefPtr<const AllocationHandle>& aHandle,
                                    const RefPtr<SourceMediaStream>& aStream,
                                    TrackID aTrackID,
                                    StreamTime aDesiredTime,
                                    const PrincipalHandle& aPrincipalHandle)
{
  TRACE_AUDIO_CALLBACK_COMMENT("SourceMediaStream %p track %i",
                               aStream.get(), aTrackID);
  // AppendFrame takes ownership of `segment`
=======
>>>>>>> upstream-releases
  VideoSegment segment;
  segment.AppendFrame(ycbcr_image.forget(),
                      gfx::IntSize(mOpts.mWidth, mOpts.mHeight),
                      mPrincipalHandle);
  mStream->AppendToTrack(mTrackID, &segment);
}

// This class is created on the media thread, as part of Start(), then entirely
// self-sustained until destruction, just forwarding calls to Pull().
class AudioSourcePullListener : public MediaStreamTrackListener {
 public:
  AudioSourcePullListener(RefPtr<SourceMediaStream> aStream, TrackID aTrackID,
                          const PrincipalHandle& aPrincipalHandle,
                          uint32_t aFrequency)
      : mStream(std::move(aStream)),
        mTrackID(aTrackID),
        mPrincipalHandle(aPrincipalHandle),
        mSineGenerator(
            MakeUnique<SineWaveGenerator>(mStream->GraphRate(), aFrequency)) {
    MOZ_COUNT_CTOR(AudioSourcePullListener);
  }

<<<<<<< HEAD
  StreamTime delta = aDesiredTime - aEndOfAppendedData;
  MOZ_ASSERT(delta > 0);

  // nullptr images are allowed
  IntSize size(mOpts.mWidth, mOpts.mHeight);
  segment.AppendFrame(image.forget(), delta, size, aPrincipalHandle);
  // This can fail if either a) we haven't added the track yet, or b)
  // we've removed or finished the track.
  aStream->AppendToTrack(aTrackID, &segment);
}
||||||| merged common ancestors
  StreamTime delta = aDesiredTime - aStream->GetEndOfAppendedData(aTrackID);
  if (delta > 0) {
    // nullptr images are allowed
    IntSize size(mOpts.mWidth, mOpts.mHeight);
    segment.AppendFrame(image.forget(), delta, size, aPrincipalHandle);
    // This can fail if either a) we haven't added the track yet, or b)
    // we've removed or finished the track.
    aStream->AppendToTrack(aTrackID, &segment);
  }
}
=======
  ~AudioSourcePullListener() { MOZ_COUNT_DTOR(AudioSourcePullListener); }

  void NotifyPull(MediaStreamGraph* aGraph, StreamTime aEndOfAppendedData,
                  StreamTime aDesiredTime) override;

  const RefPtr<SourceMediaStream> mStream;
  const TrackID mTrackID;
  const PrincipalHandle mPrincipalHandle;
  const UniquePtr<SineWaveGenerator> mSineGenerator;
};
>>>>>>> upstream-releases

/**
 * Default audio source.
 */

<<<<<<< HEAD
MediaEngineDefaultAudioSource::MediaEngineDefaultAudioSource()
    : mMutex("MediaEngineDefaultAudioSource::mMutex") {}
||||||| merged common ancestors
MediaEngineDefaultAudioSource::MediaEngineDefaultAudioSource()
  : mMutex("MediaEngineDefaultAudioSource::mMutex")
{}
=======
MediaEngineDefaultAudioSource::MediaEngineDefaultAudioSource() = default;
>>>>>>> upstream-releases

<<<<<<< HEAD
MediaEngineDefaultAudioSource::~MediaEngineDefaultAudioSource() {}
||||||| merged common ancestors
MediaEngineDefaultAudioSource::~MediaEngineDefaultAudioSource()
{}
=======
MediaEngineDefaultAudioSource::~MediaEngineDefaultAudioSource() = default;
>>>>>>> upstream-releases

nsString MediaEngineDefaultAudioSource::GetName() const {
  return NS_LITERAL_STRING(u"Default Audio Device");
}

nsCString MediaEngineDefaultAudioSource::GetUUID() const {
  return NS_LITERAL_CSTRING("B7CBD7C1-53EF-42F9-8353-73F61C70C092");
}

<<<<<<< HEAD
uint32_t MediaEngineDefaultAudioSource::GetBestFitnessDistance(
||||||| merged common ancestors
uint32_t
MediaEngineDefaultAudioSource::GetBestFitnessDistance(
=======
nsString MediaEngineDefaultAudioSource::GetGroupId() const {
  return NS_LITERAL_STRING(u"Default Audio Group");
}

uint32_t MediaEngineDefaultAudioSource::GetBestFitnessDistance(
>>>>>>> upstream-releases
    const nsTArray<const NormalizedConstraintSet*>& aConstraintSets,
    const nsString& aDeviceId) const {
  uint32_t distance = 0;
#ifdef MOZ_WEBRTC
  for (const auto* cs : aConstraintSets) {
    distance =
        MediaConstraintsHelper::GetMinimumFitnessDistance(*cs, aDeviceId);
    break;  // distance is read from first entry only
  }
#endif
  return distance;
}

<<<<<<< HEAD
bool MediaEngineDefaultAudioSource::IsAvailable() const {
  AssertIsOnOwningThread();

  return mState == kReleased;
||||||| merged common ancestors
bool
MediaEngineDefaultAudioSource::IsAvailable() const
{
  AssertIsOnOwningThread();

  return mState == kReleased;
=======
void MediaEngineDefaultAudioSource::GetSettings(
    MediaTrackSettings& aOutSettings) const {
  MOZ_ASSERT(NS_IsMainThread());
  aOutSettings.mAutoGainControl.Construct(false);
  aOutSettings.mEchoCancellation.Construct(false);
  aOutSettings.mNoiseSuppression.Construct(false);
  aOutSettings.mChannelCount.Construct(1);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult MediaEngineDefaultAudioSource::Allocate(
    const dom::MediaTrackConstraints& aConstraints,
    const MediaEnginePrefs& aPrefs, const nsString& aDeviceId,
    const mozilla::ipc::PrincipalInfo& aPrincipalInfo,
    AllocationHandle** aOutHandle, const char** aOutBadConstraint) {
||||||| merged common ancestors
nsresult
MediaEngineDefaultAudioSource::Allocate(const dom::MediaTrackConstraints &aConstraints,
                                        const MediaEnginePrefs &aPrefs,
                                        const nsString& aDeviceId,
                                        const mozilla::ipc::PrincipalInfo& aPrincipalInfo,
                                        AllocationHandle** aOutHandle,
                                        const char** aOutBadConstraint)
{
=======
nsresult MediaEngineDefaultAudioSource::Allocate(
    const MediaTrackConstraints& aConstraints, const MediaEnginePrefs& aPrefs,
    const nsString& aDeviceId,
    const mozilla::ipc::PrincipalInfo& aPrincipalInfo,
    const char** aOutBadConstraint) {
>>>>>>> upstream-releases
  AssertIsOnOwningThread();

  MOZ_ASSERT(mState == kReleased);

  // Mock failure for automated tests.
  if (aConstraints.mDeviceId.WasPassed() &&
      aConstraints.mDeviceId.Value().IsString() &&
      aConstraints.mDeviceId.Value().GetAsString().EqualsASCII("bad device")) {
    return NS_ERROR_FAILURE;
  }

  mFrequency = aPrefs.mFreq ? aPrefs.mFreq : 1000;

  mState = kAllocated;
  return NS_OK;
}

<<<<<<< HEAD
nsresult MediaEngineDefaultAudioSource::Deallocate(
    const RefPtr<const AllocationHandle>& aHandle) {
||||||| merged common ancestors
nsresult
MediaEngineDefaultAudioSource::Deallocate(const RefPtr<const AllocationHandle>& aHandle)
{
=======
nsresult MediaEngineDefaultAudioSource::Deallocate() {
>>>>>>> upstream-releases
  AssertIsOnOwningThread();

  MOZ_ASSERT(mState == kStopped || mState == kAllocated);

  if (mStream && IsTrackIDExplicit(mTrackID)) {
    mStream->EndTrack(mTrackID);
    mStream = nullptr;
    mTrackID = TRACK_NONE;
    mPrincipalHandle = PRINCIPAL_HANDLE_NONE;
  }
  mState = kReleased;
  return NS_OK;
}

<<<<<<< HEAD
void MediaEngineDefaultAudioSource::SetTrack(
    const RefPtr<const AllocationHandle>& aHandle,
    const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
    const PrincipalHandle& aPrincipal) {
||||||| merged common ancestors
nsresult
MediaEngineDefaultAudioSource::SetTrack(const RefPtr<const AllocationHandle>& aHandle,
                                        const RefPtr<SourceMediaStream>& aStream,
                                        TrackID aTrackID,
                                        const PrincipalHandle& aPrincipal)
{
=======
void MediaEngineDefaultAudioSource::SetTrack(
    const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
    const PrincipalHandle& aPrincipal) {
>>>>>>> upstream-releases
  AssertIsOnOwningThread();

  MOZ_ASSERT(mState == kAllocated);
  MOZ_ASSERT(!mStream);
  MOZ_ASSERT(mTrackID == TRACK_NONE);

  // AddAudioTrack will take ownership of segment
  mStream = aStream;
  mTrackID = aTrackID;
<<<<<<< HEAD
  aStream->AddAudioTrack(aTrackID, aStream->GraphRate(), new AudioSegment(),
||||||| merged common ancestors
  aStream->AddAudioTrack(aTrackID,
                         aStream->GraphRate(),
                         0,
                         new AudioSegment(),
=======
  mPrincipalHandle = aPrincipal;
  aStream->AddAudioTrack(aTrackID, aStream->GraphRate(), new AudioSegment(),
>>>>>>> upstream-releases
                         SourceMediaStream::ADDTRACK_QUEUED);
}

<<<<<<< HEAD
nsresult MediaEngineDefaultAudioSource::Start(
    const RefPtr<const AllocationHandle>& aHandle) {
||||||| merged common ancestors
nsresult
MediaEngineDefaultAudioSource::Start(const RefPtr<const AllocationHandle>& aHandle)
{
=======
nsresult MediaEngineDefaultAudioSource::Start() {
>>>>>>> upstream-releases
  AssertIsOnOwningThread();

  MOZ_ASSERT(mState == kAllocated || mState == kStopped);
  MOZ_ASSERT(mStream, "SetTrack() must happen before Start()");
  MOZ_ASSERT(IsTrackIDExplicit(mTrackID),
             "SetTrack() must happen before Start()");

  if (!mPullListener) {
    mPullListener = MakeAndAddRef<AudioSourcePullListener>(
        mStream, mTrackID, mPrincipalHandle, mFrequency);
  }

<<<<<<< HEAD
  {
    MutexAutoLock lock(mMutex);
    mState = kStarted;
  }

  NS_DispatchToMainThread(
      NS_NewRunnableFunction(__func__, [stream = mStream, track = mTrackID]() {
        if (stream->IsDestroyed()) {
          return;
        }
        stream->SetPullingEnabled(track, true);
      }));

||||||| merged common ancestors
  MutexAutoLock lock(mMutex);
  mState = kStarted;
=======
  mState = kStarted;

  NS_DispatchToMainThread(NS_NewRunnableFunction(
      __func__,
      [stream = mStream, track = mTrackID, listener = mPullListener]() {
        if (stream->IsDestroyed()) {
          return;
        }
        stream->AddTrackListener(listener, track);
        stream->SetPullingEnabled(track, true);
      }));

>>>>>>> upstream-releases
  return NS_OK;
}

<<<<<<< HEAD
nsresult MediaEngineDefaultAudioSource::Stop(
    const RefPtr<const AllocationHandle>& aHandle) {
||||||| merged common ancestors
nsresult
MediaEngineDefaultAudioSource::Stop(const RefPtr<const AllocationHandle>& aHandle)
{
=======
nsresult MediaEngineDefaultAudioSource::Stop() {
>>>>>>> upstream-releases
  AssertIsOnOwningThread();

  if (mState == kStopped || mState == kAllocated) {
    return NS_OK;
  }
  MOZ_ASSERT(mState == kStarted);
<<<<<<< HEAD

  {
    MutexAutoLock lock(mMutex);
    mState = kStopped;
  }

  NS_DispatchToMainThread(
      NS_NewRunnableFunction(__func__, [stream = mStream, track = mTrackID]() {
        if (stream->IsDestroyed()) {
          return;
        }
        stream->SetPullingEnabled(track, false);
      }));
||||||| merged common ancestors

  MutexAutoLock lock(mMutex);
  mState = kStopped;
=======
  mState = kStopped;

  NS_DispatchToMainThread(
      NS_NewRunnableFunction(__func__, [stream = mStream, track = mTrackID,
                                        listener = std::move(mPullListener)]() {
        if (stream->IsDestroyed()) {
          return;
        }
        stream->RemoveTrackListener(listener, track);
        stream->SetPullingEnabled(track, false);
      }));
>>>>>>> upstream-releases
  return NS_OK;
}

<<<<<<< HEAD
nsresult MediaEngineDefaultAudioSource::Reconfigure(
    const RefPtr<AllocationHandle>& aHandle,
    const dom::MediaTrackConstraints& aConstraints,
    const MediaEnginePrefs& aPrefs, const nsString& aDeviceId,
    const char** aOutBadConstraint) {
||||||| merged common ancestors
nsresult
MediaEngineDefaultAudioSource::Reconfigure(
    const RefPtr<AllocationHandle>& aHandle,
    const dom::MediaTrackConstraints& aConstraints,
    const MediaEnginePrefs &aPrefs,
    const nsString& aDeviceId,
    const char** aOutBadConstraint)
{
=======
nsresult MediaEngineDefaultAudioSource::Reconfigure(
    const MediaTrackConstraints& aConstraints, const MediaEnginePrefs& aPrefs,
    const nsString& aDeviceId, const char** aOutBadConstraint) {
>>>>>>> upstream-releases
  return NS_OK;
}

<<<<<<< HEAD
void MediaEngineDefaultAudioSource::AppendToSegment(
    AudioSegment& aSegment, TrackTicks aSamples,
    const PrincipalHandle& aPrincipalHandle) {
  RefPtr<SharedBuffer> buffer =
      SharedBuffer::Create(aSamples * sizeof(int16_t));
||||||| merged common ancestors
void
MediaEngineDefaultAudioSource::AppendToSegment(AudioSegment& aSegment,
                                               TrackTicks aSamples,
                                               const PrincipalHandle& aPrincipalHandle)
{
  RefPtr<SharedBuffer> buffer = SharedBuffer::Create(aSamples * sizeof(int16_t));
=======
void AudioSourcePullListener::NotifyPull(MediaStreamGraph* aGraph,
                                         StreamTime aEndOfAppendedData,
                                         StreamTime aDesiredTime) {
  TRACE_AUDIO_CALLBACK_COMMENT("SourceMediaStream %p track %i", mStream.get(),
                               mTrackID);
  AudioSegment segment;
  TrackTicks delta = aDesiredTime - aEndOfAppendedData;
  RefPtr<SharedBuffer> buffer = SharedBuffer::Create(delta * sizeof(int16_t));
>>>>>>> upstream-releases
  int16_t* dest = static_cast<int16_t*>(buffer->Data());
<<<<<<< HEAD

  mSineGenerator->generate(dest, aSamples);
  AutoTArray<const int16_t*, 1> channels;
||||||| merged common ancestors

  mSineGenerator->generate(dest, aSamples);
  AutoTArray<const int16_t*,1> channels;
=======
  mSineGenerator->generate(dest, delta);
  AutoTArray<const int16_t*, 1> channels;
>>>>>>> upstream-releases
  channels.AppendElement(dest);
  segment.AppendFrames(buffer.forget(), channels, delta, mPrincipalHandle);
  mStream->AppendToTrack(mTrackID, &segment);
}

<<<<<<< HEAD
void MediaEngineDefaultAudioSource::Pull(
    const RefPtr<const AllocationHandle>& aHandle,
    const RefPtr<SourceMediaStream>& aStream, TrackID aTrackID,
    StreamTime aEndOfAppendedData, StreamTime aDesiredTime,
    const PrincipalHandle& aPrincipalHandle) {
  TRACE_AUDIO_CALLBACK_COMMENT("SourceMediaStream %p track %i", aStream.get(),
                               aTrackID);
  AudioSegment segment;
  // avoid accumulating rounding errors
  TrackTicks desired =
      aStream->TimeToTicksRoundUp(aStream->GraphRate(), aDesiredTime);
  TrackTicks delta = desired - mLastNotify;
  mLastNotify += delta;
  AppendToSegment(segment, delta, aPrincipalHandle);
  aStream->AppendToTrack(aTrackID, &segment);
}

void MediaEngineDefault::EnumerateDevices(
    uint64_t aWindowId, dom::MediaSourceEnum aMediaSource,
    MediaSinkEnum aMediaSink, nsTArray<RefPtr<MediaDevice>>* aDevices) {
||||||| merged common ancestors
void
MediaEngineDefaultAudioSource::Pull(const RefPtr<const AllocationHandle>& aHandle,
                                    const RefPtr<SourceMediaStream>& aStream,
                                    TrackID aTrackID,
                                    StreamTime aDesiredTime,
                                    const PrincipalHandle& aPrincipalHandle)
{
  TRACE_AUDIO_CALLBACK_COMMENT("SourceMediaStream %p track %i",
                               aStream.get(), aTrackID);
  AudioSegment segment;
  // avoid accumulating rounding errors
  TrackTicks desired = aStream->TimeToTicksRoundUp(aStream->GraphRate(), aDesiredTime);
  TrackTicks delta = desired - mLastNotify;
  mLastNotify += delta;
  AppendToSegment(segment, delta, aPrincipalHandle);
  aStream->AppendToTrack(aTrackID, &segment);
}

void
MediaEngineDefault::EnumerateDevices(uint64_t aWindowId,
                                     dom::MediaSourceEnum aMediaSource,
                                     MediaSinkEnum aMediaSink,
                                     nsTArray<RefPtr<MediaDevice>>* aDevices)
{
=======
void MediaEngineDefault::EnumerateDevices(
    uint64_t aWindowId, MediaSourceEnum aMediaSource, MediaSinkEnum aMediaSink,
    nsTArray<RefPtr<MediaDevice>>* aDevices) {
>>>>>>> upstream-releases
  AssertIsOnOwningThread();

  switch (aMediaSource) {
    case MediaSourceEnum::Camera: {
      // Only supports camera video sources. See Bug 1038241.
<<<<<<< HEAD

      // We once had code here to find a VideoSource with the same settings and
      // re-use that. This is no longer possible since the resolution gets set
      // in Allocate().

      nsTArray<RefPtr<MediaEngineSource>>* devicesForThisWindow =
          mVSources.LookupOrAdd(aWindowId);
||||||| merged common ancestors

      // We once had code here to find a VideoSource with the same settings and
      // re-use that. This is no longer possible since the resolution gets set
      // in Allocate().

      nsTArray<RefPtr<MediaEngineSource>>*
        devicesForThisWindow = mVSources.LookupOrAdd(aWindowId);
=======
>>>>>>> upstream-releases
      auto newSource = MakeRefPtr<MediaEngineDefaultVideoSource>();
      aDevices->AppendElement(MakeRefPtr<MediaDevice>(
<<<<<<< HEAD
          newSource, newSource->GetName(),
          NS_ConvertUTF8toUTF16(newSource->GetUUID()), NS_LITERAL_STRING("")));
||||||| merged common ancestors
                                newSource,
                                newSource->GetName(),
                                NS_ConvertUTF8toUTF16(newSource->GetUUID()),
                                NS_LITERAL_STRING("")));
=======
          newSource, newSource->GetName(),
          NS_ConvertUTF8toUTF16(newSource->GetUUID()), newSource->GetGroupId(),
          NS_LITERAL_STRING("")));
>>>>>>> upstream-releases
      return;
    }
<<<<<<< HEAD
    case dom::MediaSourceEnum::Microphone: {
      nsTArray<RefPtr<MediaEngineDefaultAudioSource>>* devicesForThisWindow =
          mASources.LookupOrAdd(aWindowId);
      for (const RefPtr<MediaEngineDefaultAudioSource>& source :
           *devicesForThisWindow) {
        if (source->IsAvailable()) {
          aDevices->AppendElement(MakeRefPtr<MediaDevice>(
              source, source->GetName(),
              NS_ConvertUTF8toUTF16(source->GetUUID()), NS_LITERAL_STRING("")));
        }
      }

      if (aDevices->IsEmpty()) {
        // All streams are currently busy, just make a new one.
        auto newSource = MakeRefPtr<MediaEngineDefaultAudioSource>();
        devicesForThisWindow->AppendElement(newSource);
        aDevices->AppendElement(
            MakeRefPtr<MediaDevice>(newSource, newSource->GetName(),
                                    NS_ConvertUTF8toUTF16(newSource->GetUUID()),
                                    NS_LITERAL_STRING("")));
      }
||||||| merged common ancestors
    case dom::MediaSourceEnum::Microphone: {
      nsTArray<RefPtr<MediaEngineDefaultAudioSource>>*
        devicesForThisWindow = mASources.LookupOrAdd(aWindowId);
      for (const RefPtr<MediaEngineDefaultAudioSource>& source : *devicesForThisWindow) {
        if (source->IsAvailable()) {
          aDevices->AppendElement(MakeRefPtr<MediaDevice>(
                                    source,
                                    source->GetName(),
                                    NS_ConvertUTF8toUTF16(source->GetUUID()),
                                    NS_LITERAL_STRING("")));
        }
      }

      if (aDevices->IsEmpty()) {
        // All streams are currently busy, just make a new one.
        auto newSource = MakeRefPtr<MediaEngineDefaultAudioSource>();
        devicesForThisWindow->AppendElement(newSource);
        aDevices->AppendElement(MakeRefPtr<MediaDevice>(
                                  newSource,
                                  newSource->GetName(),
                                  NS_ConvertUTF8toUTF16(newSource->GetUUID()),
                                  NS_LITERAL_STRING("")));
      }
=======
    case MediaSourceEnum::Microphone: {
      auto newSource = MakeRefPtr<MediaEngineDefaultAudioSource>();
      aDevices->AppendElement(MakeRefPtr<MediaDevice>(
          newSource, newSource->GetName(),
          NS_ConvertUTF8toUTF16(newSource->GetUUID()), newSource->GetGroupId(),
          NS_LITERAL_STRING("")));
>>>>>>> upstream-releases
      return;
    }
    default:
      MOZ_ASSERT_UNREACHABLE("Unsupported source type");
      return;
  }

  if (aMediaSink == MediaSinkEnum::Speaker) {
    NS_WARNING("No default implementation for MediaSinkEnum::Speaker");
  }
}

<<<<<<< HEAD
void MediaEngineDefault::ReleaseResourcesForWindow(uint64_t aWindowId) {
  nsTArray<RefPtr<MediaEngineDefaultAudioSource>>* audioDevicesForThisWindow =
      mASources.Get(aWindowId);

  if (audioDevicesForThisWindow) {
    for (const RefPtr<MediaEngineDefaultAudioSource>& source :
         *audioDevicesForThisWindow) {
      source->Shutdown();
    }
  }

  mASources.Remove(aWindowId);

  nsTArray<RefPtr<MediaEngineSource>>* videoDevicesForThisWindow =
      mVSources.Get(aWindowId);

  if (videoDevicesForThisWindow) {
    for (const RefPtr<MediaEngineSource>& source : *videoDevicesForThisWindow) {
      source->Shutdown();
    }
  }

  mVSources.Remove(aWindowId);
}

void MediaEngineDefault::Shutdown() {
  AssertIsOnOwningThread();

  for (auto iter = mVSources.Iter(); !iter.Done(); iter.Next()) {
    for (const RefPtr<MediaEngineSource>& source : *iter.UserData()) {
      if (source) {
        source->Shutdown();
      }
    }
  }
  for (auto iter = mASources.Iter(); !iter.Done(); iter.Next()) {
    for (const RefPtr<MediaEngineDefaultAudioSource>& source :
         *iter.UserData()) {
      if (source) {
        source->Shutdown();
      }
    }
  }
  mVSources.Clear();
  mASources.Clear();
};

}  // namespace mozilla
||||||| merged common ancestors
void
MediaEngineDefault::ReleaseResourcesForWindow(uint64_t aWindowId)
{
  nsTArray<RefPtr<MediaEngineDefaultAudioSource>>* audioDevicesForThisWindow =
   mASources.Get(aWindowId);

  if (audioDevicesForThisWindow) {
    for (const RefPtr<MediaEngineDefaultAudioSource>& source :
         *audioDevicesForThisWindow) {
      source->Shutdown();
    }
  }

  mASources.Remove(aWindowId);

  nsTArray<RefPtr<MediaEngineSource>>* videoDevicesForThisWindow =
    mVSources.Get(aWindowId);

  if (videoDevicesForThisWindow) {
    for (const RefPtr<MediaEngineSource>& source :
         *videoDevicesForThisWindow) {
      source->Shutdown();
    }
  }

  mVSources.Remove(aWindowId);
}

void
MediaEngineDefault::Shutdown()
{
  AssertIsOnOwningThread();

  for (auto iter = mVSources.Iter(); !iter.Done(); iter.Next()) {
    for (const RefPtr<MediaEngineSource>& source : *iter.UserData()) {
      if (source) {
        source->Shutdown();
      }
    }
  }
  for (auto iter = mASources.Iter(); !iter.Done(); iter.Next()) {
    for (const RefPtr<MediaEngineDefaultAudioSource>& source : *iter.UserData()) {
      if (source) {
        source->Shutdown();
      }
    }
  }
  mVSources.Clear();
  mASources.Clear();
};

} // namespace mozilla
=======
}  // namespace mozilla
>>>>>>> upstream-releases
